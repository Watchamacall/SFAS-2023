// Fill out your copyright notice in the Description page of Project Settings.


#include "ProMeshSquareActor.h"

// Sets default values
AProMeshSquareActor::AProMeshSquareActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//TODO: Change this name as it is inappropriate, however for the time it was required
	BaseMesh = CreateDefaultSubobject<UProGenMeshBase>(TEXT("Please Work For Fuck's Sake!"));
	RootComponent = BaseMesh;
}

void AProMeshSquareActor::StartEvent_Implementation()
{

}
void AProMeshSquareActor::UpdateEvent_Implementation(int SidesOnShape)
{}

void AProMeshSquareActor::NewProceduralMeshObject()
{
	BaseMesh = CreateDefaultSubobject<UProGenMeshBase>(TEXT("New Mesh Exist"));
}

