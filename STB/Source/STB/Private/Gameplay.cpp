#include "Gameplay.h"

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
	//TODO: Add creating object with x sides here, including linking up and displaying with a texture
}

void UGameplay::NextLevel()
{
	Levels->CleanupLevel();
	
	if(CurrentLevel < Levels->GetNumLevels())
	{
		const FProgressionLevelData* LevelData = Levels->SpawnLevel(CurrentLevel);

		if(LevelData)
		{
			CurrentBallBounds = LevelData->BallBounds;
			CurrentRequiredDistance = LevelData->RequiredDistance;
			if (LevelData->Mesh)
			{
				auto MeshClass = LevelData->Mesh;
				if (MeshClass == USquareShape::StaticClass())
				{
					ActorToShow->CreateDefaultSubobject<USquareShape>();
					ActorToShow = LevelData->Mesh->GetDefaultObject<USquareShape>();
				}
				else if (MeshClass == UPentagonShape::StaticClass())
				{
					BaseMesh = LevelData->Mesh->GetDefaultObject<UPentagonShape>();
				}
			}
			ChooseRandomBallLocation();
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

bool UGameplay::TryMove(const FVector2D& PlayerGuess, const FVector2D& BallLocation2D)
{
	bWin = false;
	const float Distance = FVector2D::Distance(BallLocation2D, PlayerGuess);

	if(Distance <= CurrentRequiredDistance)
	{
		CurrentLevel = FMath::Clamp(CurrentLevel + 1, 0, Levels->GetNumLevels());
		bWin = true;
	}
	else
	{
		CurrentLives = FMath::Clamp(CurrentLives - 1, 0, 3);
	}

	return bWin;
}
//TODO: Change this function to instead set the points of an object
void UGameplay::ChooseRandomBallLocation()
{
	BallLocation = CurrentBallBounds.Origin;
	BallLocation.X += FMath::RandRange( -CurrentBallBounds.BoxExtent.X, CurrentBallBounds.BoxExtent.X );
	BallLocation.Y += FMath::RandRange( -CurrentBallBounds.BoxExtent.Y, CurrentBallBounds.BoxExtent.Y );
	BallLocation.Z += FMath::RandRange( -CurrentBallBounds.BoxExtent.Z, CurrentBallBounds.BoxExtent.Z );
}
