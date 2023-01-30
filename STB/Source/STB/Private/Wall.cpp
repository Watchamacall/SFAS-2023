// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"

void AWall::StartMovingWall(FVector InitialWallVector, float TimeForWallHit, float DistanceToTravel)
{
	SetActorLocation(InitialWallVector);
	MoveSpeed = DistanceToTravel / TimeForWallHit;
	Move = true;
}

void AWall::Tick(float DeltaSeconds)
{
	if (Move)
	{
		FVector CurLocation = GetActorLocation();
		FVector NewWallLocation = FVector(CurLocation.X - MoveSpeed, CurLocation.Y, CurLocation.Z);
		SetActorLocation(NewWallLocation);
	}
}
