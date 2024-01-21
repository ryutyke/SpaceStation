// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoronoiDiagram.h"
#include "Geom.h"

/**
 * 
 */
class FRACTURING_API VoronoiClipper
{
public:
	VoronoiClipper();
	~VoronoiClipper();

	TArray<FVector2D> pointsIn;
	TArray<FVector2D> pointsOut;

	void ClipSite(VoronoiDiagram diag, TArray<FVector2D> polygon, int site, TArray<FVector2D>& clipped);
};
