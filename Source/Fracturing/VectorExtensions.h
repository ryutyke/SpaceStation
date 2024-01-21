// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FRACTURING_API VectorExtensions
{
public:
	VectorExtensions();
	~VectorExtensions();

	static bool IsReal(const float f);
	static bool IsReal(const FVector2D v2);
	static bool IsReal(const FVector v3);
};
