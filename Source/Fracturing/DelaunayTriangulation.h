// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FRACTURING_API DelaunayTriangulation
{
public:
	DelaunayTriangulation();
	~DelaunayTriangulation();

	TArray<FVector2D> Vertices;
	TArray<int> Triangles;

	void Clear();
	FString ToString();
	bool Verify();
	bool IsEmpty();
};
