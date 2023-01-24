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
		FVector ObjectScale = FVector::OneVector;
	/*
	 * The tries of the object, count must be a multiple of 3
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<int32> Triangles = TArray<int32>({ 0,1,2,0,2,3 });
	/*
	 * The verticies of the object, can be any number
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FVector> Verticies = TArray<FVector>({ FVector(1.0f,0.0f,1.0f), FVector(1.0f,0.0f,-1.0f), FVector(-1.0f,0.0f,-1.0f), FVector(-1.0f,0.0f,1.0f) });
	/*
	 * This sorta exists
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FVector> Normals = TArray<FVector>({ FVector(1.0f,0.0f,0.0f),FVector(1.0f,0.0f,0.0f),FVector(1.0f,0.0f,0.0f),FVector(1.0f,0.0f,0.0f) });
	/*
	 * UV is the map for texturing, more complex objects will have more precise numbers, must match the number of verticies
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FVector2D> UV0 = TArray<FVector2D>({ FVector2D(10.0f,10.0f),FVector2D(10.0f,-10.0f),FVector2D(-10.0f,-10.0f),FVector2D(-10.0f,10.0f), });
	/*
	 * This sorta exists
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FProcMeshTangent> Tangents = TArray<FProcMeshTangent>({ FProcMeshTangent(0,1,0), FProcMeshTangent(0,1,0), FProcMeshTangent(0,1,0), FProcMeshTangent(0,1,0) });
	/*
	 * The colour of the objects Verticies
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FLinearColor> VertexColours = TArray<FLinearColor>({ FLinearColor(0.75, 0.75, 0.675, 1.0), FLinearColor(0.75, 0.75, 0.675, 1.0), FLinearColor(0.75, 0.75, 0.675, 1.0), FLinearColor(0.75, 0.75, 0.675, 1.0) });

	UPROPERTY(EditAnywhere)
		TMap<FString, int8> StringToVertexIndex = TMap<FString, int8>({ {"TopButton", 0}, {"LeftButton", 1}, {"RightButton", 2}, {"BottomButton", 3} });



	virtual void BeginPlay() override;
};
