// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VertexCollision.generated.h"

UCLASS()
class STB_API AVertexCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVertexCollision();

public:
	UFUNCTION()
		void SetComponent(int NewVertexNo, UStaticMesh* NewRootMesh);
	int VertexNo = 0;
	UStaticMeshComponent* RootMesh;
};
