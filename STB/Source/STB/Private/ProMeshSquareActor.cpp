// Fill out your copyright notice in the Description page of Project Settings.


#include "ProMeshSquareActor.h"

// Sets default values
AProMeshSquareActor::AProMeshSquareActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<UProGenMeshBase>(TEXT("Generated Mesh"));

	BaseMesh = Cast<UProGenMeshBase>(RootComponent);
}

void AProMeshSquareActor::StartEvent_Implementation()
{

}
void AProMeshSquareActor::UpdateEvent_Implementation(int SidesOnShape, UStaticMesh* ColliderMesh)
{}

void AProMeshSquareActor::NewProceduralMeshObject()
{
	BaseMesh = CreateDefaultSubobject<UProGenMeshBase>(TEXT("New Mesh Exist"));
}

