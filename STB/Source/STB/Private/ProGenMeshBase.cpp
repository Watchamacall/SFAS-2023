// Fill out your copyright notice in the Description page of Project Settings.


#include "ProGenMeshBase.h"


//DEBUG
void UProGenMeshBase::UpdateAllComponents(FVector NewObjectScale, TArray<int32> NewTriangles, TArray<FVector> NewVerticies, TArray<FVector> NewNormals, TArray<FVector> NewUV0, TArray<FProcMeshTangent> NewTangents, TArray<FLinearColor> NewVertexColours)
{
	this->ObjectScale = NewObjectScale;
	this->Triangles = NewTriangles;
	this->Verticies = NewVerticies;
	this->Normals = NewNormals;
	this->UV0 = TArray<FVector2D>();
	for (size_t i = 0; i < NewUV0.Num(); i++)
	{
		this->UV0.Add(FVector2D(NewUV0[i].X, NewUV0[i].Y));
	}
	this->Tangents = NewTangents;
	this->VertexColours = NewVertexColours;
}

void UProGenMeshBase::CreateMesh()
{
	TArray<FVector> NewVerticies;
	for (size_t i = 0; i < Verticies.Num(); i++)
	{
		Verticies[i] *= ObjectScale;
	}

	CreateMeshSection_LinearColor(0, Verticies, Triangles, Normals, UV0, VertexColours, Tangents, true);
	ContainsPhysicsTriMeshData(true);
}

#pragma region Update Mesh
void UProGenMeshBase::UpdateMesh(int SidesOnShape)
{
	if (SidesOnShape <= 2)
	{
		UE_LOG(LogTemp, Display, TEXT("Shape is 2 or less sides, will crash program!"));
		return;
	}

	//Clearing the fields to allow for new Shapes to be created
	Triangles.Empty();
	Verticies.Empty();
	Normals.Empty();
	UV0.Empty();
	Tangents.Empty();
	VertexColours.Empty();

	//Setting all the Verticies in the shape and equating them to a circle
	float Angle = 360 / SidesOnShape;

	for (size_t CurrentSide = 0; CurrentSide < SidesOnShape; CurrentSide++)
	{
		//Verticies
		float CurAngle = FMath::FMath::DegreesToRadians(Angle * CurrentSide); //Change to Radians since Sin and Cos use Radians and not Degrees
		float X = FMath::Sin(CurAngle);
		float Y = FMath::Cos(CurAngle);
		Verticies.Add(FVector(X, 0.f, Y));

		//Normals
		Normals.Add(FVector(1.f, 0.f, 0.f));
		//UV0
		UV0.Add(FVector2D(X, Y));
		//Tangents
		Tangents.Add(FProcMeshTangent(0.f, 1.f, 0.f));
		//Linear Colour
		VertexColours.Add(FLinearColor(.75f, .75f, .675f, 1.f));
	}
	//Setting all the Triangles
	//Minus the two since we only need Sides - 2 Tries, will cause major issues otherwise
	for (size_t CurrentTri = 0; CurrentTri < SidesOnShape - 2; CurrentTri++)
	{
		Triangles.Add(0);
		Triangles.Add(CurrentTri + 1);
		Triangles.Add(CurrentTri + 2);
	}
	CreateMesh();
	//UpdateMeshSection_LinearColor(0, Verticies, Normals, UV0, VertexColours, Tangents);
}

void UProGenMeshBase::UpdateMeshInternally()
{
	UVRecount();
	UpdateMeshSection_LinearColor(0, Verticies, Normals, UV0, VertexColours, Tangents);
}
#pragma endregion
void UProGenMeshBase::UVRecount()
{
	for (size_t CurrentUV = 0; CurrentUV < UV0.Num(); CurrentUV++)
	{
		UV0[CurrentUV] = FVector2D(Verticies[CurrentUV].X, Verticies[CurrentUV].Z);
	}
}

#pragma region Vertex Get Set
FVector UProGenMeshBase::GetVertex(int Index)
{
	return Verticies[Index];
}

void UProGenMeshBase::SetVertex(int Index, FVector NewVertex)
{
	Verticies[Index] = NewVertex;
}
#pragma endregion
int UProGenMeshBase::VertexIndexFromString(FString VertexString)
{
	return StringToVertexIndex[VertexString];
}

void UProGenMeshBase::SetVertexMinMax(FVector2D XYMin, FVector2D XYMax)
{
	VertexMinMax = TArray<FXYMinMax>();

	for (size_t i = 0; i < Verticies.Num(); i++)
	{
		FXYMinMax NewVertMinMax = FXYMinMax(
			FVector2D(Verticies[i].X + XYMin.X, Verticies[i].Z + XYMin.Y),
			FVector2D(Verticies[i].X + XYMax.X, Verticies[i].Z + XYMax.Y)
		);
		VertexMinMax.Add(NewVertMinMax);
	}
}

FXYMinMax UProGenMeshBase::GetVertexMinMax(int Index)
{
	return VertexMinMax[Index];
}

void UProGenMeshBase::BeginPlay()
{
	Super::BeginPlay();
	CreateMesh();
}