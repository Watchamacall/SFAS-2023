// Search for a Star 2023


#include "STBPlayerController.h"

#include "EngineUtils.h"
#include "ProgressionData.h"
#include "Gameplay.h"
#include "Screen.h"
#include "STBPlayerCameraManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "STB/STBGameModes.h"

#pragma region Input Action Names
const FString ASTBPlayerController::TopButtonActionName = TEXT("TopButton");
const FString ASTBPlayerController::LeftButtonActionName = TEXT("LeftButton");
const FString ASTBPlayerController::RightButtonActionName = TEXT("RightButton");
const FString ASTBPlayerController::BottomButtonActionName = TEXT("BottomButton");
const FString ASTBPlayerController::LeftStickXAxisName = TEXT("LeftX");
const FString ASTBPlayerController::LeftStickYAxisName = TEXT("LeftY");
const FString ASTBPlayerController::RightStickXAxisName = TEXT("RightX");
const FString ASTBPlayerController::RightStickYAxisName = TEXT("RightY");
const FString ASTBPlayerController::LeftTriggerAxisName = TEXT("LeftTrigger");
const FString ASTBPlayerController::RightTriggerAxisName = TEXT("RightTrigger");
const FString ASTBPlayerController::TopButtonAxisName = TEXT("TopButtonAxis");
#pragma endregion

ASTBPlayerController::ASTBPlayerController()
{
	PlayerCameraManagerClass = ASTBPlayerCameraManager::StaticClass();
}

void ASTBPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (APawn* const CurrentPawn = GetPawnOrSpectator())
	{
		if (UPawnMovementComponent* MovementComponent = CurrentPawn->GetMovementComponent())
		{
			// Reset velocity before processing input when orbiting to limit acceleration which 
			// can cause overshooting and jittering as orbit attempts to maintain a fixed radius.
			MovementComponent->Velocity = FVector::ZeroVector;
			MovementComponent->UpdateComponentVelocity();
		}
	}	
}

void ASTBPlayerController::UpdateCameraManager(float DeltaSeconds)
{
	if (ASTBPlayerCameraManager* STBCameraManager = Cast<ASTBPlayerCameraManager>(PlayerCameraManager))
	{
		STBCameraManager->SetLocationAndRotation(LastOrbitPawnLocation, LastOrbitPawnViewRotation);
		STBCameraManager->UpdateCamera(DeltaSeconds);
	}
}

void ASTBPlayerController::UpdateRotation(float DeltaTime)
{
	if(APawn* const CurrentPawn = GetPawnOrSpectator())
	{
		FRotator Rotation = GetControlRotation();

		Rotation.Yaw += RotationInput.Yaw * AxisScale * DeltaTime;
		Rotation.Pitch += RotationInput.Pitch * AxisScale * DeltaTime;

		LastOrbitPawnLocation = OrbitPivot + Rotation.Vector() * OrbitRadius;
		LastOrbitPawnViewRotation = (OrbitPivot - LastOrbitPawnLocation).GetSafeNormal().Rotation();
		CurrentPawn->SetActorLocationAndRotation(LastOrbitPawnLocation, LastOrbitPawnViewRotation);
		SetControlRotation(Rotation);
	}
}

void ASTBPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(IsValid(Gameplay) && CurrentState == ESTBGameMode::Playing)
	{
		const auto Bounds = Gameplay->GetCurrentBallBounds();
		DrawDebugBox(GetWorld(), Bounds.Origin, Bounds.BoxExtent, FColor::Green, false, 0.2f, SDPG_Foreground, 1.0f);		
		DrawDebugSphere(GetWorld(), Gameplay->GetBallLocation(), 20.0f, 10.0f, FColor::Red, false, 0.2f, SDPG_Foreground, 1.0f);


	}
	if (ActorToShow->BaseMesh)
	{
		ActorToShow->BaseMesh->UpdateMesh();
	}
}


void ASTBPlayerController::CreateUI()
{
	Widgets.Init(nullptr, static_cast<int>(ESTBGameMode::NumModes));
	SetupScreen();
	ShowUI(ESTBGameMode::Intro);
}

void ASTBPlayerController::BeginNewGame()
{
	if(Gameplay)
	{
		CurrentPlayerLocation = FVector2D::ZeroVector;
		Gameplay->StartNewGame();
		Gameplay->NextLevel();
	}
}

void ASTBPlayerController::ContinueGame()
{
	if(Gameplay)
	{
		CurrentPlayerLocation = FVector2D::ZeroVector;

		if(Gameplay->GetWin())
		{
			Gameplay->NextLevel();
		}
		else if(Gameplay->GetLives() <= 0)
		{
			ShowUI(ESTBGameMode::GameOver);
		}
		else
		{
			Gameplay->ChooseRandomBallLocation();
		}
	}	
}


void ASTBPlayerController::ShowUI(ESTBGameMode State)
{
	//Static casting the State to an int, allows for accuracy
	const int TargetModeIndex = static_cast<int>(State);

	for(int Count = 0; Count < static_cast<int>(ESTBGameMode::NumModes); ++Count)
	{
		//Shows if the Count is equal to the TargetModeIndex

		const bool bShow = Count == TargetModeIndex;

		Widgets[Count]->Show(bShow);
		
	}
	CurrentState = State;
}

const UGameplay* ASTBPlayerController::GetGameplay() const 
{
	return Gameplay;
}

const FVector2D& ASTBPlayerController::GetCurrentPlayerLocation() const
{
	return CurrentPlayerLocation;
}

FVector2D ASTBPlayerController::GetCurrentBallLocation() const
{
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	FVector2D ScreenLocation = FVector2D::ZeroVector;
	
	if(IsValid(Gameplay))
	{
		const FVector BallLocation = Gameplay->GetBallLocation();
		ProjectWorldLocationToScreen(BallLocation, ScreenLocation, true);
	}
	
	return ScreenLocation - (ViewportSize * 0.5f);
}

bool ASTBPlayerController::TryMove()
{
	return Gameplay->TryMove(CurrentPlayerLocation, GetCurrentBallLocation());
}

void ASTBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Gameplay = NewObject<UGameplay>();
	
	if(IsValid(Gameplay))
	{
		//Creating the actor which will be showing the object the player will be able to move
		if (!ActorToShow)
		{
			ActorToShow = GetWorld()->SpawnActor<AProMeshSquareActor>();
			ActorToShow->SetActorLocation(FVector(0.f, 0.f, 60.f));
			ActorToShow->SetActorRotation(FRotator(0.f, -90.f, 0.f));
		}
		if (!Gameplay->ActorToShow)
		{
			Gameplay->ActorToShow = ActorToShow;
		}

		const TActorIterator<AProgressionData> ProgressionIterator(GetWorld());
		if(ProgressionIterator)
		{
			Gameplay->SetLevels(*ProgressionIterator);
		}
	}	
}

void ASTBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if(InputComponent)
	{
		//Face Button Presses
		InputComponent->BindAction(*TopButtonActionName, IE_Pressed, this, &ASTBPlayerController::TopButtonPress);
		InputComponent->BindAction(*LeftButtonActionName, IE_Pressed, this, &ASTBPlayerController::LeftButtonPress);
		InputComponent->BindAction(*RightButtonActionName, IE_Pressed, this, &ASTBPlayerController::RightButtonPress);
		InputComponent->BindAction(*BottomButtonActionName, IE_Pressed, this, &ASTBPlayerController::BottomButtonPress);

		//Face Button Releases
		InputComponent->BindAction(*TopButtonActionName, IE_Released, this, &ASTBPlayerController::TopButtonRelease);
		InputComponent->BindAction(*LeftButtonActionName, IE_Released, this, &ASTBPlayerController::LeftButtonRelease);
		InputComponent->BindAction(*RightButtonActionName, IE_Released, this, &ASTBPlayerController::RightButtonRelease);
		InputComponent->BindAction(*BottomButtonActionName, IE_Released, this, &ASTBPlayerController::BottomButtonRelease);

		//Left Stick Setup
		InputComponent->BindAxis(*LeftStickXAxisName, this, &ASTBPlayerController::LeftRight);
		InputComponent->BindAxis(*LeftStickYAxisName, this, &ASTBPlayerController::UpDown);
	}
}

void ASTBPlayerController::SetupScreen(ESTBGameMode State, TSubclassOf<UScreen> Class, FName Name)
{
	UScreen* Screen = NewObject<UScreen>(this, Class, Name);
	Screen->bIsFocusable = true;
	Screen->SetOwningPlayer(this);
	Screen->AddToViewport(0);
	Widgets[static_cast<int>(State)] = Screen;
}
void ASTBPlayerController::SetupScreen()
{
	TArray<TSubclassOf<UScreen>> keys;
	UI_Classes.GenerateKeyArray(keys);

	for (size_t i = 0; i < UI_Classes.Num(); i++)
	{
		SetupScreen(*UI_Classes.Find(keys[i]), keys[i], keys[i].Get()->GetFName());
	}
}

void ASTBPlayerController::LeftRight(float Value)
{
	if(CurrentState == ESTBGameMode::Playing)
	{
#pragma region Button Movement
		if (TopButton)
		{
			int8 VertexIndex = ActorToShow->BaseMesh->VertexIndexFromString(TopButtonActionName);
			UpdateVertex('X', VertexIndex, Value);
		}
		if (LeftButton)
		{
			int8 VertexIndex = ActorToShow->BaseMesh->VertexIndexFromString(LeftButtonActionName);
			UpdateVertex('X', VertexIndex, Value);
		}
		if (RightButton)
		{
			int8 VertexIndex = ActorToShow->BaseMesh->VertexIndexFromString(RightButtonActionName);
			UpdateVertex('X', VertexIndex, Value);
		}
		if (BottomButton)
		{
			int8 VertexIndex = ActorToShow->BaseMesh->VertexIndexFromString(BottomButtonActionName);
			UpdateVertex('X', VertexIndex, Value);
		}
#pragma endregion
		//TODO: Set this up to utilise the face button presses
		CurrentPlayerLocation.X = FMath::Clamp(CurrentPlayerLocation.X + Value, -PlayerLocationXRange, PlayerLocationXRange);
	}
}

void ASTBPlayerController::UpDown(float Value)
{
	if(CurrentState == ESTBGameMode::Playing)
	{
#pragma region Button Movement
		if (TopButton)
		{
			int8 VertexIndex = ActorToShow->BaseMesh->VertexIndexFromString(TopButtonActionName);
			UpdateVertex('Z', VertexIndex, Value);
		}
		if (LeftButton)
		{
			int8 VertexIndex = ActorToShow->BaseMesh->VertexIndexFromString(LeftButtonActionName);
			UpdateVertex('Z', VertexIndex, Value);
		}
		if (RightButton)
		{
			int8 VertexIndex = ActorToShow->BaseMesh->VertexIndexFromString(RightButtonActionName);
			UpdateVertex('Z', VertexIndex, Value);
		}
		if (BottomButton)
		{
			int8 VertexIndex = ActorToShow->BaseMesh->VertexIndexFromString(BottomButtonActionName);
			UpdateVertex('Z', VertexIndex, Value);
		}
#pragma endregion
		CurrentPlayerLocation.Y = FMath::Clamp(CurrentPlayerLocation.Y + Value, -PlayerLocationXRange, PlayerLocationXRange);
	}
}

void ASTBPlayerController::UpdateVertex(char VectorName, int8 VertexIndex, float Value)
{
	FVector Vertex = ActorToShow->BaseMesh->GetVertex(VertexIndex);

	switch (VectorName)
	{
	case 'X':
		Vertex.X = FMath::Clamp(Vertex.X + Value, -50, 50);
		break;
	case 'Y':
		Vertex.Y = FMath::Clamp(Vertex.Y + Value, -50, 50);
		break;
	case 'Z':
		Vertex.Z = FMath::Clamp(Vertex.Z + Value, -50, 50);
		break;
	default:
		break;
	}
	ActorToShow->BaseMesh->SetVertex(VertexIndex, Vertex);
	ActorToShow->BaseMesh->UpdateMesh();
}

#pragma region Button Presses
void ASTBPlayerController::TopButtonPress()
{
	if(const int Index = static_cast<int>(CurrentState); Index >= 0 && Index < static_cast<int>(ESTBGameMode::NumModes))
	{
		Widgets[Index]->Alt2();
	}
	TopButton = true;

}

void ASTBPlayerController::LeftButtonPress()
{
	if(const int Index = static_cast<int>(CurrentState); Index >= 0 && Index < static_cast<int>(ESTBGameMode::NumModes))
	{
		Widgets[Index]->Alt1();
	}
	LeftButton = true;
}

void ASTBPlayerController::RightButtonPress()
{
	if(const int Index = static_cast<int>(CurrentState); Index >= 0 && Index < static_cast<int>(ESTBGameMode::NumModes))
	{
		Widgets[Index]->Back();
	}
	RightButton = true;
}

void ASTBPlayerController::BottomButtonPress()
{
	if(const int Index = static_cast<int>(CurrentState); Index >= 0 && Index < static_cast<int>(ESTBGameMode::NumModes))
	{
		Widgets[Index]->Select();
	}	
	BottomButton = true;
}
#pragma endregion
#pragma region Button Releases
void ASTBPlayerController::TopButtonRelease()
{
	TopButton = false;
	UE_LOG(LogTemp, Display, TEXT("Released Top Button"))
}
void ASTBPlayerController::LeftButtonRelease()
{
	LeftButton = false;
	UE_LOG(LogTemp, Display, TEXT("Released Left Button"))
}
void ASTBPlayerController::RightButtonRelease()
{
	RightButton = false;
	UE_LOG(LogTemp, Display, TEXT("Released Right Button"))
}
void ASTBPlayerController::BottomButtonRelease()
{
	BottomButton = false;
	UE_LOG(LogTemp, Display, TEXT("Released Bottom Button"))
}