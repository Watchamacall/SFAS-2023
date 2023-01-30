// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "StaticVertexCollider.h"
#include "ProGenMeshBase.generated.h"

USTRUCT(BlueprintType)
struct FXYMinMax
{
	GENERATED_USTRUCT_BODY()
	FXYMinMax() {}
	FXYMinMax(FVector2D NewXYMin, FVector2D NewXYMax)
	{
		XYMin = NewXYMin;
		XYMax = NewXYMax;
	}
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D XYMin;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D XYMax;
};

/**
 * 
 */
UCLASS()
class STB_API UProGenMeshBase : public UProceduralMeshComponent
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable)
		virtual void UpdateAllComponents(FVector ObjectScale, TArray<int32> Triangles, TArray<FVector> Verticies, TArray<UStaticVertexCollider*> NewVertexColliders, TArray<FVector> Normals, TArray<FVector> UV0, TArray<FProcMeshTangent> Tangents, TArray<FLinearColor> VertexColours);
	/*
	 * Creates the initial mesh based on the values first inputted in the Blueprint 
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CreateMesh();
	/*
	 * Updates the mesh's variables based on the number of sides the shape will be, calculates Tries and everything
	*/
	UFUNCTION(BlueprintCallable)
		virtual void UpdateMesh(int SidesOnShape, UStaticMesh* ColliderMesh);
	/*
	 * Updates the mesh internally, would be overriding but Unreal doesn't like same named functions
	*/
	UFUNCTION(BlueprintCallable)
		virtual void UpdateMeshInternally();

	/*
	 * Sets the Vertex positions to randomised values
	*/
	UFUNCTION(BlueprintCallable)
		virtual void SetRandomVertexLocations();

	/*
	 * Get's the vertex based on the Index number provided
	*/
	UFUNCTION(BlueprintCallable)
		virtual FVector GetVertex(int Index);

	/*
	 * Set's the vertex based at the Index with the NewVertex
	*/
	UFUNCTION(BlueprintCallable)
		virtual void SetVertex(int Index, FVector NewVertex);

	/*
	 * Returns the VertexIndex based on the FString provided
	*/
	UFUNCTION(BlueprintCallable)
		virtual int VertexIndexFromString(FString VertexString);

	/*
	 * Sets the Minimum and Maximum the Verticies can move based on the XYMin and XYMax
	*/
	UFUNCTION(BlueprintCallable)
		void SetVertexMinMax(FVector2D XYMin, FVector2D XYMax);
	/*
	 * Returns the XYMinMax Struct for the Vertex based at the Index no.
	*/
	UFUNCTION(BlueprintCallable)
		FXYMinMax GetVertexMinMax(int Index);

protected:
	/*
	 * The overall scale of the object
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector ObjectScale;
	/*
	 * The tries of the object, count must be a multiple of 3
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<int32> Triangles;
	/*
	 * The verticies of the object, can be any number
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FVector> Verticies;
	/*
	 * Holds the  
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<UStaticVertexCollider*> VertexColliders;
	/*
	 * This sorta exists
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FVector> Normals;
	/*
	 * UV is the map for texturing, more complex objects will have more precise numbers, must match the number of verticies
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FVector2D> UV0;
	/*
	 * This sorta exists
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FProcMeshTangent> Tangents;
	/*
	 * The colour of the objects Verticies
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FLinearColor> VertexColours;
	/*
	 * Mapping the Face Buttons to Index Numbers
	*/
	UPROPERTY(EditAnywhere)
		TMap<FString, int8> StringToVertexIndex = TMap<FString, int8>({ {"TopButton", 0}, {"LeftButton", 1}, {"RightButton", 2}, {"BottomButton", 3} });
	/*
	 * The Minimum and Maximum for the Verticies to move
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FXYMinMax> VertexMinMax;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector VertexColliderScale = FVector(.05f, .05f, .05f);
	//TODO: Add more button combinations to this and make it up to like 8 button presses

	virtual void BeginPlay() override;
private:
		UFUNCTION()
			void UVRecount();
};
