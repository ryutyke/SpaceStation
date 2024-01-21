// Fill out your copyright notice in the Description page of Project Settings.


#include "VectorExtensions.h"

VectorExtensions::VectorExtensions()
{
}

VectorExtensions::~VectorExtensions()
{
}

bool VectorExtensions::IsReal(const float f)
{
	return !FMath::IsFinite(f) && !FMath::IsNaN(f);
}

bool VectorExtensions::IsReal(const FVector2D v2)
{
	return IsReal(v2.X) && IsReal(v2.Y);
}

bool VectorExtensions::IsReal(const FVector v3)
{
	return IsReal(v3.X) && IsReal(v3.Y) && IsReal(v3.Z);
}
