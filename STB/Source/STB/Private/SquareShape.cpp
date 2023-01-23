// Fill out your copyright notice in the Description page of Project Settings.


#include "SquareShape.h"

USquareShape::USquareShape()
{
	//Set all components for a Square Object when the object is first constructed, can be changed where needed or in a blueprint
	if (ObjectScale == FVector::ZeroVector)
		ObjectScale = FVector(50.f,1.f,50.f);

	if (Triangles.Num() == 0)
		Triangles = { 0,1,2,0,2,3 };

	if (Verticies.Num() == 0)
		Verticies = { FVector(-1,0,-1), FVector(-1,0,1), FVector(1,0,1), FVector(1,0,-1) };

	if (Normals.Num() == 0)
		Normals = { FVector(1,0,0), FVector(1,0,0), FVector(1,0,0), FVector(1,0,0) };

	if (UV0.Num() == 0)
		UV0 = { FVector2D(-10,-10), FVector2D(-10,10), FVector2D(10,10), FVector2D(10,-10) };

	if (Tangents.Num() == 0)
		Tangents = { FProcMeshTangent(0,1,0), FProcMeshTangent(0,1,0), FProcMeshTangent(0,1,0), FProcMeshTangent(0,1,0) };

	if (VertexColours.Num() == 0)
		VertexColours = { FLinearColor(0.75,0.75,0.675,1.0), FLinearColor(0.75,0.75,0.675,1.0), FLinearColor(0.75,0.75,0.675,1.0), FLinearColor(0.75,0.75,0.675,1.0) };

	if (StringToVertexIndex.Num() == 0)
		StringToVertexIndex = TMap<FString, int8>({ {"TopButton", 0}, {"LeftButton", 1}, {"RightButton", 2}, {"BottomButton", 3} });
}
