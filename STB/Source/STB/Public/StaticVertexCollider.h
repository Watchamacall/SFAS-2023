// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "StaticVertexCollider.generated.h"

/**
 * 
 */
UCLASS()
class STB_API UStaticVertexCollider : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	int VertexNo;
};
