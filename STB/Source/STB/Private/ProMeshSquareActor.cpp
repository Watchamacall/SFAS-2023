// Fill out your copyright notice in the Description page of Project Settings.


#include "ProMeshSquareActor.h"

// Sets default values
AProMeshSquareActor::AProMeshSquareActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SquareMesh = CreateDefaultSubobject<UProGenMeshBase>(TEXT("Square"));
	RootComponent = SquareMesh;
}

// Called when the game starts or when spawned
void AProMeshSquareActor::BeginPlay()
{
	Super::BeginPlay();
	/*SquareMesh->CreateMesh();*/
}

// Called every frame
void AProMeshSquareActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

