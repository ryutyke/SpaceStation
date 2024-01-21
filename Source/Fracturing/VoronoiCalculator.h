// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DelaunayCalculator.h"
#include "Geom.h"
#include "VoronoiDiagram.h"

/**
 * 
 */
class FRACTURING_API VoronoiCalculator
{
public:
	VoronoiCalculator();
	~VoronoiCalculator();

	struct PointTriangle
	{
		int Point;
		int Triangle;

		PointTriangle(int _point, int _triangle) :
			Point(_point),
			Triangle(_triangle)
		{}

		FString ToString()
		{
			return FString::Printf(TEXT("PointTriangle(%d, %d)"), Point, Triangle);
		}
	};

	class PTComparer
	{
	public:
		TArray<FVector2D> verts;
		TArray<int> tris;

		bool Compare(const PointTriangle& pt0, const PointTriangle& pt1) const
		{
			if (pt0.Point > pt1.Point) { return false; }
			else if (pt0.Point < pt1.Point) { return true; }
			else if (pt0.Triangle == pt1.Triangle) {
				check(pt0.Point == pt1.Point);
				return true;
			}
			else {
				return CompareAngles(pt0, pt1);
			}
		}
		bool CompareAngles(const PointTriangle& pt0, const PointTriangle& pt1) const
		{
			check(pt0.Point == pt1.Point);

			// "reference" point
			FVector2D rp = verts[pt0.Point];

			FVector2D p0 = Centroid(pt0) - rp;
			FVector2D p1 = Centroid(pt1) - rp;

			int q0 = ((p0.Y < 0) || ((p0.Y == 0) && (p0.X < 0)));
			int q1 = ((p1.Y < 0) || ((p1.Y == 0) && (p1.Y < 0)));

			if (q0 == q1) {
				int cp = p0.X * p1.Y - p0.Y * p1.X;

				if (cp > 0) { return true; }
				else if (cp < 0) { return false; }
				else { return true; }
			}
			else { return q1 ? true : false; }
		}
		FVector2D Centroid(const PointTriangle& pt) const
		{
			int ti = pt.Triangle;
			return Geom::TriangleCentroid(verts[tris[ti]], verts[tris[ti + 1]], verts[tris[ti + 2]]);
		}
	};

	DelaunayCalculator delCalc;
	PTComparer cmp;
	TArray<PointTriangle> pts;

	VoronoiDiagram CalculateDiagram(TArray<FVector2D> inputVertices);
	void voidCalculateDiagram(TArray<FVector2D> inputVertices, VoronoiDiagram& result);
	static int NonSharedPoint(TArray<int> tris, int ti0, int ti1);
	static bool SharesEdge(TArray<int> tris, int ti0, int ti1);
};