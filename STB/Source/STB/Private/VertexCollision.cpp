// Fill out your copyright notice in the Description page of Project Settings.


#include "VertexCollision.h"

// Sets default values
AVertexCollision::AVertexCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AVertexCollision::SetComponent(int NewVertexNo, UStaticMesh* NewRootMesh)
{
	this->VertexNo = NewVertexNo;
	RootMesh = NewObject<UStaticMeshComponent>(this, TEXT("Root Mesh Component"));
	RootMesh->SetStaticMesh(NewRootMesh);
	RootComponent = RootMesh;
}



