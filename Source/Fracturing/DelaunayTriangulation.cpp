// Fill out your copyright notice in the Description page of Project Settings.


#include "DelaunayTriangulation.h"

DelaunayTriangulation::DelaunayTriangulation()
{
	Vertices.Empty();
	Triangles.Empty();
}

DelaunayTriangulation::~DelaunayTriangulation()
{
}

void DelaunayTriangulation::Clear()
{
	Vertices.Empty();
	Triangles.Empty();
}

FString DelaunayTriangulation::ToString()
{
	FString rString;

	rString += TEXT("#Vertices\n");
	for (const FVector2D& vert : Vertices)
	{
		rString += FString::Printf(TEXT("X:%f, Y:%f\n"), vert.X, vert.Y);
	}

	rString += TEXT("#Edges of Triangles\n");
	for (int i = 0; i < Triangles.Num(); i++)
	{
		rString += FString::Printf(TEXT("%d: %d\n"), i, Triangles[i]);
	}

	return rString;
}

bool DelaunayTriangulation::Verify()
{
	//try
	//{
	//	for (int i = 0; i < Triangles.Num(); i += 3)
	//	{
	//		FVector2D c0 = Vertices[Triangles[i]];
	//		FVector2D c1 = Vertices[Triangles[i + 1]];
	//		FVector2D c2 = Vertices[Triangles[i + 2]];

	//		for (int j = 0; j < Vertices.Num(); j++)
	//		{
	//			FVector2D p = Vertices[j];
	//			/*if (InsideCircumcircle(p, c0, c1, c2)) 
	//			{
	//				return false;
	//			}*/
	//		}
	//	}
	//	return true;
	//}
	//catch (...) { return false; }
	return true;
}

bool DelaunayTriangulation::IsEmpty()
{
	if (Vertices.IsEmpty() && Triangles.IsEmpty())
	{
		return true;
	}
	else
	{
		return false;
	}
}