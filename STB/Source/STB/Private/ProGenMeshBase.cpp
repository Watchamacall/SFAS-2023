// Fill out your copyright notice in the Description page of Project Settings.


#include "ProGenMeshBase.h"

//#pragma region Constructors
//UProGenMeshBase::UProGenMeshBase()
//{
//
//}
//UProGenMeshBase::UProGenMeshBase(FVector ObjectScale, TArray<int32> Triangles, TArray<FVector> Verticies, TArray<FVector> Normals, TArray<FVector2D> UV0, TArray<FProcMeshTangent> Tangents, TArray<FLinearColor> VertexColours)
//{
//	this->ObjectScale = ObjectScale;
//	this->Triangles = Triangles;
//	this->Verticies = Verticies;
//	this->Normals = Normals;
//	this->UV0 = UV0;
//	this->Tangents = Tangents;
//	this->VertexColours = VertexColours;
//}
//#pragma endregion
UProGenMeshBase* UProGenMeshBase::CreateMeshBase(UObject* ObjectToAttach, int SidesOnShape, FVector ObjectScale)
{
	UProGenMeshBase* ReturnVariable = NewObject<UProGenMeshBase>(ObjectToAttach);
	ReturnVariable->ObjectScale = ObjectScale;
	
	//Setting all the Verticies in the shape and equating them to a circle
	float Angle = 360 / SidesOnShape;
	
	for (size_t CurrentSide = 0; CurrentSide < SidesOnShape; CurrentSide++)
	{
		//Verticies
		float CurAngle = FMath:: FMath::DegreesToRadians(Angle * CurrentSide); //Change to Radians since Sin and Cos use Radians and not Degrees
		float X = FMath::Sin(CurAngle);
		float Y = FMath::Cos(CurAngle);
		ReturnVariable->Verticies.Add(FVector(X, 0.f, Y));

		//Normals
		ReturnVariable->Normals.Add(FVector(1.f, 0.f, 0.f));
		//UV0
		ReturnVariable->UV0.Add(FVector2D(X, Y));
		//Tangents
		ReturnVariable->Tangents.Add(FProcMeshTangent(0.f, 1.f, 0.f));
		//Linear Colour
		ReturnVariable->VertexColours.Add(FLinearColor(.75f, .75f, .675f, 1.f));
	}
	if (SidesOnShape < 2)
	{
		return ReturnVariable;
	}
	//Setting all the Triangles
	//Minus the two since we only need Sides - 2 Tries, will cause major issues otherwise
	for (size_t CurrentTri = 0; CurrentTri < SidesOnShape - 2; CurrentTri++)
	{
		ReturnVariable->Triangles.Add(0);
		ReturnVariable->Triangles.Add(CurrentTri + 1);
		ReturnVariable->Triangles.Add(CurrentTri + 2);
	}
	return ReturnVariable;
}

void UProGenMeshBase::CreateMesh()
{
	TArray<FVector> NewVerticies;
	for (size_t i = 0; i < Verticies.Num(); i++)
	{
		NewVerticies.Add(Verticies[i] * ObjectScale);
	}

	CreateMeshSection_LinearColor(0, NewVerticies, Triangles, Normals, UV0, VertexColours, Tangents, true);
	ContainsPhysicsTriMeshData(true);
}
void UProGenMeshBase::UpdateMesh()
{
	UVRecount();
	UpdateMeshSection_LinearColor(0, Verticies, Normals, UV0, VertexColours, Tangents);
}

FVector UProGenMeshBase::GetVertex(int Index)
{
	return Verticies[Index];
}

void UProGenMeshBase::SetVertex(int Index, FVector NewVertex)
{
	Verticies[Index] = NewVertex;
}

int UProGenMeshBase::VertexIndexFromString(FString VertexString)
{
	return StringToVertexIndex[VertexString];
}

void UProGenMeshBase::UVRecount()
{
	for (size_t CurrentUV = 0; CurrentUV < UV0.Num(); CurrentUV++)
	{
		UV0[CurrentUV] = FVector2D(Verticies[CurrentUV].X, Verticies[CurrentUV].Z);
	}
}

void UProGenMeshBase::BeginPlay()
{
	Super::BeginPlay();
	CreateMesh();
}