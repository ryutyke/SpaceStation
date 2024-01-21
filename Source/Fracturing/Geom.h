// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FRACTURING_API Geom
{
public:
	Geom();
	~Geom();

	static bool AreCoincident(FVector2D a, FVector2D b);
	static bool ToTheLeft(FVector2D p, FVector2D l0, FVector2D l1);
	static bool ToTheRight(FVector2D p, FVector2D l0, FVector2D l1);
	static bool PointInTriangle(FVector2D p, FVector2D c0, FVector2D c1, FVector2D c2);
	static bool InsideCircumcircle(FVector2D p, FVector2D c0, FVector2D c1, FVector2D c2);
	static FVector2D RotateRightAngle(FVector2D v);
	static bool LineLineIntersection(FVector2D p0, FVector2D v0, FVector2D p1, FVector2D v1, float& m0, float& m1);
	static FVector2D LineLineIntersection(FVector2D p0, FVector2D v0, FVector2D p1, FVector2D v1);
	static FVector2D CircumcircleCenter(FVector2D c0, FVector2D c1, FVector2D c2);
	static FVector2D TriangleCentroid(FVector2D c0, FVector2D c1, FVector2D c2);
	static float Area(TArray<FVector2D> polygon);
};
