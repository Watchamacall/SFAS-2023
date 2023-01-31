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
#pragma endregion

#pragma region Override Functions
ASTBPlayerController::ASTBPlayerController()
{
	PlayerCameraManagerClass = ASTBPlayerCameraManager::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
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

void ASTBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Gameplay = NewObject<UGameplay>();

	if (IsValid(Gameplay))
	{
		Gameplay->SetOwner(this);
		//Getting the Actor within the map since UProceduralMeshComponent is annoying and won't work if spawned in during runtime, at least I couldn't get it to work

		TArray<AActor*> AllActorToShow;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AProMeshSquareActor::StaticClass(), AllActorToShow);

		for (size_t i = 0; i < AllActorToShow.Num(); i++)
		{
			if (AllActorToShow[i]->ActorHasTag(GeneratedMeshTag))
			{
				ActorToShow = Cast<AProMeshSquareActor>(AllActorToShow[i]);
				ActorToShow->StartEvent();
			}
			else if (AllActorToShow[i]->ActorHasTag(WallMeshTag))
			{
				WallActorToShow = Cast<AProMeshSquareActor>(AllActorToShow[i]);
				WallActorToShow->StartEvent();
			}
		}
		/*const TActorIterator<AProMeshSquareActor> SquareTest(GetWorld());
		if (SquareTest)
		{
			ActorToShow = *SquareTest;
			ActorToShow->StartEvent();
		}*/
		Gameplay->ActorToShow = ActorToShow;
		Gameplay->WallActorToShow = WallActorToShow;

		const TActorIterator<AWall> Wall(GetWorld());
		if (Wall)
		{
			WallComponent = *Wall;
		}

		const TActorIterator<AProgressionData> ProgressionIterator(GetWorld());
		if (ProgressionIterator)
		{
			Gameplay->SetLevels(*ProgressionIterator);
		}
	}
}


void ASTBPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(IsValid(Gameplay) && CurrentState == ESTBGameMode::Playing)
	{
		//TODO: Show where the current points are on the wall as it comes closer, allowing for precision

		//TODO: Add the bonus content
		float Distance = FVector::Dist(ActorToShow->GetActorLocation(), WallComponent->GetActorLocation());
		if (Distance < 1)
		{
			WallComponent->StopMovingWall();
			//TODO: Show the current player's mesh points on the wall behind, try to make it look good, raycast n all that shiz
			if (TryMove())
			{
				UE_LOG(LogTemp, Display, TEXT("Hey so you won well done!"));
			}
		}
		//REMOVE THIS BEFORE PUBLISHING
		const auto Bounds = Gameplay->GetCurrentBallBounds();
		DrawDebugBox(GetWorld(), Bounds.Origin, Bounds.BoxExtent, FColor::Green, false, 0.2f, SDPG_Foreground, 1.0f);		
		DrawDebugSphere(GetWorld(), Gameplay->GetBallLocation(), 20.0f, 10.0f, FColor::Red, false, 0.2f, SDPG_Foreground, 1.0f);

	}

}

void ASTBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
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

#pragma endregion

#pragma region Update Functions
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
	if (APawn* const CurrentPawn = GetPawnOrSpectator())
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
#pragma endregion

#pragma region Game Functions
void ASTBPlayerController::BeginNewGame()
{
	if(Gameplay)
	{
		ActorToShow->BaseMesh->SetWorldLocation(ActorToShowOrigin);
		
		CurrentTimeWallInMotion = 0.f;

		Gameplay->StartNewGame();
		Gameplay->NextLevel();

		WallComponent->StartMovingWall(WallInitialVector, Gameplay->GetTimeToImpact(), FVector::Dist(ActorToShow->GetActorLocation(), WallInitialVector));
	}
}

void ASTBPlayerController::ContinueGame()
{
	if(Gameplay)
	{
		ActorToShow->BaseMesh->SetWorldLocation(ActorToShowOrigin);

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

bool ASTBPlayerController::TryMove()
{
	//TODO: Change this function to work with checking the positions/colliders between eachother
	return Gameplay->TryMove(ActorToShow->BaseMesh, WallActorToShow->BaseMesh);
}
#pragma endregion

//TODO: Fix the bug with the UI, if pressed fast enough can allow for more than one UI to be active at once (Make events only occur if the CurrentUI Element has finished it's Animation
#pragma region UI Functions
void ASTBPlayerController::CreateUI()
{
	Widgets.Init(nullptr, static_cast<int>(ESTBGameMode::NumModes));
	SetupScreen();
	ShowUI(ESTBGameMode::Intro);
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

#pragma endregion
#pragma region Get Functions
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
	//TODO: Return an array of FVector giving the coords of all the points in the rendered object
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	FVector2D ScreenLocation = FVector2D::ZeroVector;
	
	if(IsValid(Gameplay))
	{
		const FVector BallLocation = Gameplay->GetBallLocation();
		ProjectWorldLocationToScreen(BallLocation, ScreenLocation, true);
	}
	
	return ScreenLocation - (ViewportSize * 0.5f);
}
FXYMinMax ASTBPlayerController::GetSingleVertMinMax()
{
	return FXYMinMax(SingleVertexXYMovementMin, SingleVertexXYMovementMax);
}
UStaticMesh* ASTBPlayerController::GetColliderMesh()
{
	return ColliderMesh;
}
void ASTBPlayerController::SetSelectedVertex(int NewSelectedVertex)
{
	SelectedVertex = NewSelectedVertex;
}
#pragma endregion
#pragma region Movement Functions
void ASTBPlayerController::LeftRight(float Value)
{
	if(CurrentState == ESTBGameMode::Playing)
	{
#pragma region Button Movement
		//if (TopButton)
		//{
		//	int8 VertexIndex = ActorToShow->BaseMesh->VertexIndexFromString(TopButtonActionName);
		//	UpdateVertex('X', VertexIndex, Value);
		//}
		//if (LeftButton)
		//{
		//	int8 VertexIndex = ActorToShow->BaseMesh->VertexIndexFromString(LeftButtonActionName);
		//	UpdateVertex('X', VertexIndex, Value);
		//}
		//if (RightButton)
		//{
		//	int8 VertexIndex = ActorToShow->BaseMesh->VertexIndexFromString(RightButtonActionName);
		//	UpdateVertex('X', VertexIndex, Value);
		//}
		//if (BottomButton)
		//{
		//	int8 VertexIndex = ActorToShow->BaseMesh->VertexIndexFromString(BottomButtonActionName);
		//	UpdateVertex('X', VertexIndex, Value);
		//}
		//if (TopButton || LeftButton || RightButton || BottomButton) //Exists so buttons can be pressed together and moved at the same time
		//{
		//	return;
		//}
		//If none of the buttons have been pressed
#pragma endregion
		//FVector CurrentLocation = ActorToShow->BaseMesh->GetComponentLocation();
		//CurrentLocation = FVector(CurrentLocation.X , CurrentLocation.Y + ((Value * WholeObjectMoveSpeed) * GetWorld()->GetDeltaSeconds()), CurrentLocation.Z); //Set on Y axis due to the X axis being front
		//ActorToShow->BaseMesh->SetWorldLocation(CurrentLocation);

		//TODO: Add some code here for a condition for a variable and if not that variable, get the Vertex and start moving it based on the values (Use what is in the region Button Movement[FunctionCall])

		//Cursor Movement
		if (SelectedVertex >= 0)
		{
			UpdateVertex('X', SelectedVertex, Value);
		}
		CurrentPlayerLocation.X = FMath::Clamp(CurrentPlayerLocation.X + Value, CursorXYMin.X, CursorXYMax.X);
	}
}

void ASTBPlayerController::UpDown(float Value)
{
	if(CurrentState == ESTBGameMode::Playing)
	{
		//Cursor Movement
		if (SelectedVertex >= 0)
		{
			UpdateVertex('Z', SelectedVertex, Value);
		}
#pragma region Button Movement
		//if (TopButton)
		//{
		//	int8 VertexIndex = ActorToShow->BaseMesh->VertexIndexFromString(TopButtonActionName);
		//	UpdateVertex('Z', VertexIndex, Value);
		//	return;
		//}
		//if (LeftButton)
		//{
		//	int8 VertexIndex = ActorToShow->BaseMesh->VertexIndexFromString(LeftButtonActionName);
		//	UpdateVertex('Z', VertexIndex, Value);
		//	return;
		//}
		//if (RightButton)
		//{
		//	int8 VertexIndex = ActorToShow->BaseMesh->VertexIndexFromString(RightButtonActionName);
		//	UpdateVertex('Z', VertexIndex, Value);
		//	return;
		//}
		//if (BottomButton)
		//{
		//	int8 VertexIndex = ActorToShow->BaseMesh->VertexIndexFromString(BottomButtonActionName);
		//	UpdateVertex('Z', VertexIndex, Value);
		//	return;
		//}
#pragma endregion
		//FVector CurrentLocation = ActorToShow->BaseMesh->GetComponentLocation();
		//CurrentLocation = FVector(CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z + ((Value * WholeObjectMoveSpeed) * GetWorld()->GetDeltaSeconds()));
		//ActorToShow->BaseMesh->SetWorldLocation(CurrentLocation);
		
		//Cursor Movement
		CurrentPlayerLocation.Y = FMath::Clamp(CurrentPlayerLocation.Y - Value, CursorXYMin.Y, CursorXYMax.Y);
	}
}

void ASTBPlayerController::UpdateVertex(char VectorName, int8 VertexIndex, float Value)
{
	FVector Vertex = ActorToShow->BaseMesh->GetVertex(VertexIndex);
	FXYMinMax CurrentVertexMinMax = ActorToShow->BaseMesh->GetVertexMinMax(VertexIndex);

	switch (VectorName)
	{
	case 'X':
		Vertex.X = FMath::Clamp(Vertex.X + ((-Value * VertexMoveSpeed) * GetWorld()->GetDeltaSeconds()), CurrentVertexMinMax.XYMin.X, CurrentVertexMinMax.XYMax.X);
		break;
	case 'Y':
		Vertex.Y = FMath::Clamp(Vertex.Y + ((Value * VertexMoveSpeed) * GetWorld()->GetDeltaSeconds()), -50, 50); //Poor Y
		break;
	case 'Z':
		Vertex.Z = FMath::Clamp(Vertex.Z + ((Value * VertexMoveSpeed)* GetWorld()->GetDeltaSeconds()), CurrentVertexMinMax.XYMin.Y, CurrentVertexMinMax.XYMax.Y);
		break;
	default:
		break;
	}
	ActorToShow->BaseMesh->SetVertex(VertexIndex, Vertex);
	ActorToShow->BaseMesh->UpdateMeshInternally();
}
#pragma endregion
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
	UE_LOG(LogTemp, Display, TEXT("Released Bottom Button"));
	SetSelectedVertex(-1); //Invalidates the Vertex so no Verticies are changed
}
#pragma endregion