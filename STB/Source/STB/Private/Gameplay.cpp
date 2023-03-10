#include "Gameplay.h"
#include "../STBGameModes.h"

UGameplay::UGameplay()
{
	CurrentLevel = 0;
	CurrentLives = 0;
}

void UGameplay::SetLevels(AProgressionData* Data)
{
	Levels = Data;
}

void UGameplay::StartNewGame()
{
	CurrentLevel = 0;
	CurrentLives = StartingLives;
}

void UGameplay::NextLevel()
{
	Levels->CleanupLevel();
	
	if(CurrentLevel < Levels->GetNumLevels())
	{
		const FProgressionLevelData* LevelData = Levels->SpawnLevel(CurrentLevel);

		if (LevelData)
		{
			CurrentBallBounds = LevelData->BallBounds;
			CurrentTolerance = LevelData->Tolerance;

			CurrentTimeUntilImpact = LevelData->TimeTillWallHit;
			
			if (ASTBPlayerController* player = Cast<ASTBPlayerController>(Owner))
			{
				//Player Controlled Mesh Compilation

				FXYMinMax SingleVertBounds = player->GetSingleVertMinMax();

				ActorToShow->UpdateEvent(LevelData->AmountOfSidesOnShape, player->GetColliderMesh());
				ActorToShow->BaseMesh->SetVertexMinMax(SingleVertBounds.XYMin, SingleVertBounds.XYMax);

				//Wall Controlled Mesh Compilation
				WallActorToShow->UpdateEvent(LevelData->AmountOfSidesOnShape, player->GetColliderMesh());
				WallActorToShow->BaseMesh->SetVertexMinMax(SingleVertBounds.XYMin, SingleVertBounds.XYMax);

				player->WallComponent->StartMovingWall(player->WallInitialVector, GetTimeToImpact(), FVector::Dist(ActorToShow->GetActorLocation(), player->WallInitialVector));
			}
			ChooseRandomBallLocation();
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("You won the entire game!"));
		if (ASTBPlayerController* player = Cast<ASTBPlayerController>(Owner))
		{
			player->ShowUI(ESTBGameMode::Win);
		}
	}
}

void UGameplay::ResetLevel()
{
	CurrentLives--;
}

const FBoxSphereBounds& UGameplay::GetCurrentBallBounds() const
{
	return CurrentBallBounds;
}

const FVector& UGameplay::GetBallLocation() const
{
	return BallLocation;
}

int UGameplay::GetTimeToImpact() const
{
	return CurrentTimeUntilImpact;
}

int UGameplay::GetLevel() const
{
	return CurrentLevel;
}

int UGameplay::GetLives() const
{
	return CurrentLives;
}

bool UGameplay::GetWin() const
{
	return bWin;
}

AActor* UGameplay::GetOwner()
{
	return Owner;
}

void UGameplay::SetOwner(AActor* NewOwner)
{
	Owner = NewOwner;
}

bool UGameplay::TryMove(UProGenMeshBase* PlayerPoints, UProGenMeshBase* ActualPoints)
{
	bWin = false;

	for (size_t CurrentVertex = 0; CurrentVertex < PlayerPoints->GetNumSides(); CurrentVertex++)
	{
		const float DistanceBetween = FVector::Dist(PlayerPoints->GetVertex(CurrentVertex), ActualPoints->GetVertex(CurrentVertex));

		if (DistanceBetween <= CurrentTolerance)
		{
			bWin = true;
		}
		else
		{
			bWin = false;
			break;
		}
	}

	bWin ? CurrentLevel = FMath::Clamp(CurrentLevel + 1, 0, Levels->GetNumLevels()) : CurrentLives = FMath::Clamp(CurrentLives - 1, 0, StartingLives);

	return bWin;
}

void UGameplay::ChooseRandomBallLocation()
{
	WallActorToShow->BaseMesh->SetRandomVertexLocations();
	WallActorToShow->BaseMesh->UpdateMeshInternally();
}
