// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Wall.generated.h"

/**
 * 
 */
UCLASS()
class STB_API AWall : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void StartMovingWall(FVector InitialWallVector, float TimeForWallHit, float DistanceToTravel);
protected:
	UFUNCTION(BlueprintCallable)
		virtual void Tick(float DeltaSeconds) override;
private:
	bool Move;
	UPROPERTY(VisibleAnywhere)
	float MoveSpeed;
};
