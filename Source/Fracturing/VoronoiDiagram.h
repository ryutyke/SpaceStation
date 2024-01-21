// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DelaunayTriangulation.h"

/**
 * 
 */
class FRACTURING_API VoronoiDiagram
{
public:
	VoronoiDiagram();
	~VoronoiDiagram();

	enum class EdgeType
	{
		Line,
		RayCCW,
		RayCW,
		Segment
	};

	struct Edge
	{
		EdgeType Type;
		int Site;
		int Vert0;
		int Vert1;
		FVector2D Direction;

		Edge(EdgeType _type, int _site, int _vert0, int _vert1, FVector2D _direction) :
			Type(_type),
			Site(_site),
			Vert0(_vert0),
			Vert1(_vert1),
			Direction(_direction)
		{}

		FString ToString()
		{
			if (Type == EdgeType::Segment)
			{
				return FString::Printf(TEXT("VoronoiEdge(Segment, %d, %d, %d)"), Site, Vert0, Vert1);
			}
			else if (Type == EdgeType::Line)
			{
				return FString::Printf(TEXT("VoronoiEdge(Line, %d, %d, (%f, %f))"), Site, Vert0, Direction.X, Direction.Y);
			}
			else {
				return FString::Printf(TEXT("VoronoiEdge(Ray, %d, %d, (%f, %f))"), Site, Vert0, Direction.X, Direction.Y);
			}
		}
	};

	DelaunayTriangulation Triangulation;
	TArray<FVector2D> Sites;
	TArray<FVector2D> Vertices;
	TArray<Edge> Edges;
	TArray<int> FirstEdgeBySite;

	void Clear();
};
