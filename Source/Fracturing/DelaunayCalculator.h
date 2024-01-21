// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DelaunayTriangulation.h"
#include "Geom.h"

/**
 * 
 */
class FRACTURING_API DelaunayCalculator
{
public:
	DelaunayCalculator();
	~DelaunayCalculator();

	struct TriangleNode
	{
		int P0;
		int P1;
		int P2;

		int C0;
		int C1;
		int C2;

		int A0;
		int A1;
		int A2;

		bool IsLeaf() const {
			return C0 < 0 && C1 < 0 && C2 < 0;
		}

		bool IsInner() const {
			return P0 >= 0 && P1 >= 0 && P2 >= 0;
		}

		TriangleNode(int _P0, int _P1, int _P2) :
			P0(_P0),
			P1(_P1),
			P2(_P2),
			C0(-1),
			C1(-1),
			C2(-1),
			A0(-1),
			A1(-1),
			A2(-1)
		{}

		bool HasEdge(int e0, int e1)
		{
			if (e0 == P0)
			{
				return e1 == P1 || e1 == P2;
			}
			else if (e0 == P1)
			{
				return e1 == P0 || e1 == P2;
			}
			else if (e0 == P2)
			{
				return e1 == P0 || e1 == P1;
			}
			return false;
		}

		int OtherPoint(int p0, int p1)
		{
			if (p0 == P0) {
				if (p1 == P1) return P2;
				else if (p1 == P2) return P1;
				else return -1;
			}
			else if (p0 == P1) {
				if (p1 == P0) return P2;
				else if (p1 == P2) return P0;
				else return -1;
			}
			else if (p0 == P2) {
				if (p1 == P0) return P1;
				else if (p1 == P1) return P0;
				else return -1;
			}
			else return -1;
		}

		int Opposite(int p)
		{
			if (p == P0) return A0;
			else if (p == P1) return A1;
			else if (p == P2) return A2;
			else return 0;
		}

		FString ToString()
		{
			FString Output;

			if (IsLeaf())
			{
				Output = FString::Printf(TEXT("TriangleNode(%d, %d, %d)"), P0, P1, P2);
			}
			else
			{
				Output = FString::Printf(TEXT("TriangleNode(%d, %d, %d, %d, %d, %d)"), P0, P1, P2, C0, C1, C2);
			}
			return Output;
		}
	};

	int highest;
	TArray<FVector2D> verts;

	TArray<int> indices;
	TArray<TriangleNode> triangles;

	DelaunayTriangulation CalculateTriangulation(TArray<FVector2D> _verts);
	void voidCalculateTriangulation(TArray<FVector2D> _verts, DelaunayTriangulation& result);
	bool Higher(int pi0, int pi1);
	void RunBowyerWaston();
	void GenerateResult(DelaunayTriangulation& result);
	void ShuffleIndices();
	int LeafWithEdge(int ti, int e0, int e1);
	bool LegalEdge(int k, int l, int i, int j);
	void LegalizeEdge(int ti0, int ti1, int pi, int li0, int li1);
	int FindTriangleNode(int pi);
	bool PointInTriangle(int pi, int ti);
	bool ToTheLeft(int pi, int li0, int li1);
};
