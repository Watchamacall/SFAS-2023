// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "ProgressionData.generated.h"

USTRUCT(BlueprintType)
struct FProgressionCharacterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<AActor> Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimationAsset> Animation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator Rotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Position;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMeshComponent> SkeletalMesh;	
};

USTRUCT(BlueprintType)
struct FProgressionLevelData
{
	GENERATED_BODY()

	/*
	 * The Actors you want to spawn in to show (DEPRECATED)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FProgressionCharacterData> Characters;
	/*
	 * The Bounds in which the ball can spawn in
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FBoxSphereBounds BallBounds;
	/*
	 * The distance for this level in which the player's mesh can be out (The Lower, The Stricter)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Tolerance;	
	/*
	 * The amount of time in seconds it takes for the wall to reach the Player's Mesh
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float TimeTillWallHit = 0.f;
	/*
	 * The amount of sides that are on the generated Mesh
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int AmountOfSidesOnShape;
};

UCLASS()
class STB_API AProgressionData : public AActor
{
	GENERATED_BODY()

public:

	AProgressionData();

	const FProgressionLevelData* SpawnLevel(int32 Level);
	void CleanupLevel();
	int GetNumLevels() const; 

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FProgressionLevelData> Levels;

#if WITH_EDITOR
	friend class FProgressionDetails;
#endif

private:

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> SpawnedActors;
	
};
