// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"

AWall::AWall()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWall::StartMovingWall(FVector InitialWallVector, float TimeForWallHit, float DistanceToTravel)
{
	SetActorLocation(InitialWallVector);
	float MoveSpeedLocal = DistanceToTravel / TimeForWallHit;
	bool MoveLocal = true;
	MoveSpeed = MoveSpeedLocal;
	Move = MoveLocal;
}

void AWall::StopMovingWall()
{
	Move = false;
}
void AWall::Tick(float DeltaSeconds)
{
	if (Move)
	{
		FVector CurLocation = GetActorLocation();
		FVector NewWallLocation = FVector(CurLocation.X - (MoveSpeed * DeltaSeconds), CurLocation.Y, CurLocation.Z);
		SetActorLocation(NewWallLocation);
	}
}
