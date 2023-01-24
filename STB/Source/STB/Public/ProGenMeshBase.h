// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "ProGenMeshBase.generated.h"

/**
 * 
 */
UCLASS()
class STB_API UProGenMeshBase : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:
	

	UFUNCTION(BlueprintCallable)
		virtual void CreateMesh();
	UFUNCTION(BlueprintCallable)
		virtual void UpdateMesh();

	UFUNCTION(BlueprintCallable)
		virtual FVector GetVertex(int Index);
	UFUNCTION(BlueprintCallable)
		virtual void SetVertex(int Index, FVector NewVertex);
	UFUNCTION(BlueprintCallable)
		virtual int VertexIndexFromString(FString VertexString);
protected:
	/*
	 * The overall scale of the object
	*/
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector ObjectScale;
	/*
	 * The tries of the object, count must be a multiple of 3
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<int32> Triangles;
	/*
	 * The verticies of the object, can be any number
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FVector> Verticies;
	/*
	 * This sorta exists
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FVector> Normals;
	/*
	 * UV is the map for texturing, more complex objects will have more precise numbers, must match the number of verticies
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FVector2D> UV0;
	/*
	 * This sorta exists
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FProcMeshTangent> Tangents;
	/*
	 * The colour of the objects Verticies
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FLinearColor> VertexColours;

	UPROPERTY(EditAnywhere)
		TMap<FString, int8> StringToVertexIndex;



	virtual void BeginPlay() override;
};
