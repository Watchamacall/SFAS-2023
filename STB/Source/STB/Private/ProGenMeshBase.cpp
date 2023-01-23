// Fill out your copyright notice in the Description page of Project Settings.


#include "ProGenMeshBase.h"

void UProGenMeshBase::CreateMesh()
{
	for (size_t i = 0; i < Verticies.Num(); i++)
	{
		Verticies[i] *= ObjectScale;
	}

	CreateMeshSection_LinearColor(0, Verticies, Triangles, Normals, UV0, VertexColours, Tangents, true);
	ContainsPhysicsTriMeshData(true);
}
void UProGenMeshBase::UpdateMesh()
{
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

void UProGenMeshBase::BeginPlay()
{
	Super::BeginPlay();
	CreateMesh();
}