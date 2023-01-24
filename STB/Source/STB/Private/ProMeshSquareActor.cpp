// Fill out your copyright notice in the Description page of Project Settings.


#include "ProMeshSquareActor.h"

// Sets default values
AProMeshSquareActor::AProMeshSquareActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));
}
//AProMeshSquareActor::AProMeshSquareActor(UProGenMeshBase* Shape)
//{
//	PrimaryActorTick.bCanEverTick = true;
//	RootComponent = NewObject<UProGenMeshBase>(this, Shape->GetClass());
//}

// Called when the game starts or when spawned
void AProMeshSquareActor::BeginPlay()
{
	Super::BeginPlay();
	//BaseMesh->CreateMesh();
}

// Called every frame
void AProMeshSquareActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

