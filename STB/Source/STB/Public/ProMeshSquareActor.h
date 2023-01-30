// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProGenMeshBase.h"
#include "ProMeshSquareActor.generated.h"

UCLASS()
class STB_API AProMeshSquareActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProMeshSquareActor();
	
	/*
	 * The Procedural Mesh which will be utilised to show the player's shape
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actual Mesh")
		UProGenMeshBase* BaseMesh;
	/*If BaseMesh is showing None in Editor, Comment out "BaseMesh = CreateDefaultSubobject<UProGenMeshBase>(TEXT("Please Work For Fuck's Sake!"));" in ProMeshSquareActor.cpp::AProMeshSquareActor()
 Compile; Uncomment; Compile*/

	/*
	 * Event to be called in Blueprints once we find this object in the World
	*/
	UFUNCTION(BlueprintNativeEvent)
		void StartEvent();
	/*
	 * Event called to update the Shape based on the amount of sides it has
	*/
	UFUNCTION(BlueprintNativeEvent)
		void UpdateEvent(int SidesOnShape, UStaticMesh* ColliderMesh);

	UFUNCTION()
		void NewProceduralMeshObject();

};
