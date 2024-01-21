// Fill out your copyright notice in the Description page of Project Settings.


#include "VoronoiCalculator.h"

VoronoiCalculator::VoronoiCalculator()
{
	pts.Empty();
	delCalc = DelaunayCalculator();
	cmp = PTComparer();
}

VoronoiCalculator::~VoronoiCalculator()
{
}

VoronoiDiagram VoronoiCalculator::CalculateDiagram(TArray<FVector2D> inputVertices)
{
	VoronoiDiagram result;
	voidCalculateDiagram(inputVertices, result);
	return result;
}

void VoronoiCalculator::voidCalculateDiagram(TArray<FVector2D> inputVertices, VoronoiDiagram& result)
{
	if (inputVertices.Num() < 3) {
		UE_LOG(LogTemp, Warning, TEXT("Not implemented for < 3 vertices"));
	}

	// 오류나면 여기도 확인하기
	if (&result == nullptr)
	{
		result = VoronoiDiagram();
	}

	DelaunayTriangulation trig = result.Triangulation;
	result.Clear();

	// Unity 프로파일러 빼놓음
	delCalc.voidCalculateTriangulation(inputVertices, trig);

	pts.Empty();

	TArray<FVector2D> verts = trig.Vertices;
	TArray<int> tris = trig.Triangles;
	TArray<FVector2D> centers = result.Vertices;
	TArray<VoronoiDiagram::Edge> edges = result.Edges;

	//if (tris.Num() > pts.Num()) { pts.SetNum(tris.Num()); }
	//if (tris.Num() > edges.Num()) { edges.SetNum(tris.Num()); }

	for (int ti = 0; ti < tris.Num(); ti += 3)
	{
		FVector2D p0 = verts[tris[ti]];
		FVector2D p1 = verts[tris[ti + 1]];
		FVector2D p2 = verts[tris[ti + 2]];

		check(Geom::ToTheLeft(p2, p0, p1));

		centers.Add(Geom::CircumcircleCenter(p0, p1, p2));
	}

	result.Triangulation = trig;
	result.Sites = trig.Vertices;
	result.Vertices = centers;

	for (int ti = 0; ti < tris.Num(); ti += 3)
	{
		pts.Add(PointTriangle(tris[ti], ti));
		pts.Add(PointTriangle(tris[ti + 1], ti));
		pts.Add(PointTriangle(tris[ti + 2], ti));
	}

	cmp.tris = tris;
	cmp.verts = verts;

	// Unity 프로파일러 빼놓음
	// 여기 Sort 못구현함
	pts.Sort([&](const PointTriangle& pt0, const PointTriangle& pt1) {
		return cmp.Compare(pt0, pt1);
		});

	//for (int t = 0; t < pts.Num(); t++)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("pts%d : %d, %d"), t, pts[t].Point, pts[t].Triangle);
	//}
	//UE_LOG(LogTemp, Warning, TEXT("edges.Num() : %d"), edges.Num());
	//UE_LOG(LogTemp, Warning, TEXT("tris.Num() : %d"), tris.Num());

	cmp.tris.Empty();
	cmp.verts.Empty();


	for (int i = 0; i < pts.Num(); i++)
	{
		result.FirstEdgeBySite.Add(edges.Num());   // 처음엔 0개.

		int start = i;
		int end = -1;

		for (int j = i + 1; j < pts.Num(); j++)
		{
			if (pts[i].Point != pts[j].Point)
			{
				end = j - 1;
				break;
			}
		}

		if (end == -1)
		{
			end = pts.Num() - 1;
		}

		i = end;

		int count = end - start;

		check(count >= 0);

		for (int ptiCurr = start; ptiCurr <= end; ptiCurr++)
		{
			bool isEdge;

			int ptiNext = ptiCurr + 1;

			if (ptiNext > end) { ptiNext = start; }

			PointTriangle ptCurr = pts[ptiCurr];
			PointTriangle ptNext = pts[ptiNext];

			int tiCurr = ptCurr.Triangle;
			int tiNext = ptNext.Triangle;

			FVector2D p0 = verts[ptCurr.Point];

			FVector2D v2nan = FVector2D(NAN, NAN);

			if (count == 0) { isEdge = true; }
			else if (count == 1) {
				FVector2D cCurr = Geom::TriangleCentroid(verts[tris[tiCurr]], verts[tris[tiCurr + 1]], verts[tris[tiCurr + 2]]);
				FVector2D cNext = Geom::TriangleCentroid(verts[tris[tiNext]], verts[tris[tiNext + 1]], verts[tris[tiNext + 2]]);

				isEdge = Geom::ToTheLeft(cCurr, p0, cNext);
			}
			else {
				isEdge = !SharesEdge(tris, tiCurr, tiNext);
			}

			if (isEdge) 
			{
				FVector2D v0, v1;

				if (ptCurr.Point == tris[tiCurr])
				{
					v0 = verts[tris[tiCurr + 2]] - verts[tris[tiCurr + 0]];
				}
				else if (ptCurr.Point == tris[tiCurr + 1]) 
				{
					v0 = verts[tris[tiCurr + 0]] - verts[tris[tiCurr + 1]];
				}
				else {
					check(ptCurr.Point == tris[tiCurr + 2]);
					v0 = verts[tris[tiCurr + 1]] - verts[tris[tiCurr + 2]];
				}

				if (ptNext.Point == tris[tiNext]) 
				{
					v1 = verts[tris[tiNext + 0]] - verts[tris[tiNext + 1]];
				}
				else if (ptNext.Point == tris[tiNext + 1]) 
				{
					v1 = verts[tris[tiNext + 1]] - verts[tris[tiNext + 2]];
				}
				else {
					check(ptNext.Point == tris[tiNext + 2]);
					v1 = verts[tris[tiNext + 2]] - verts[tris[tiNext + 0]];
				}
				edges.Add(VoronoiDiagram::Edge::Edge(
					VoronoiDiagram::EdgeType::RayCCW,
					ptCurr.Point,
					tiCurr / 3,
					-1,
					Geom::RotateRightAngle(v0)
				));

				edges.Add(VoronoiDiagram::Edge::Edge(
					VoronoiDiagram::EdgeType::RayCW,
					ptCurr.Point,
					tiNext / 3,
					-1,
					Geom::RotateRightAngle(v1)
				));
			}
			else {
				if (!Geom::AreCoincident(centers[tiCurr / 3], centers[tiNext / 3]))
				{
					edges.Add(VoronoiDiagram::Edge::Edge(
						VoronoiDiagram::EdgeType::Segment,
						ptCurr.Point,
						tiCurr / 3,
						tiNext / 3,
						v2nan
					));
				}
			}
		}
	}
	result.Edges = edges;
}

int VoronoiCalculator::NonSharedPoint(TArray<int> tris, int ti0, int ti1)
{
	check(SharesEdge(tris, ti0, ti1));

	int x0 = tris[ti0];
	int x1 = tris[ti0 + 1];
	int x2 = tris[ti0 + 2];

	int y0 = tris[ti1];
	int y1 = tris[ti1 + 1];
	int y2 = tris[ti1 + 2];

	if (x0 != y0 && x0 != y1 && x0 != y2) { return x0; }
	if (x1 != y0 && x1 != y1 && x1 != y2) { return x1; }
	if (x2 != y0 && x2 != y1 && x2 != y2) { return x2; }

	check(false);
	return -1;
}

bool VoronoiCalculator::SharesEdge(TArray<int> tris, int ti0, int ti1)
{
	int x0 = tris[ti0];
	int x1 = tris[ti0 + 1];
	int x2 = tris[ti0 + 2];

	int y0 = tris[ti1];
	int y1 = tris[ti1 + 1];
	int y2 = tris[ti1 + 2];

	int n = 0;

	if (x0 == y0 || x0 == y1 || x0 == y2) n++;
	if (x1 == y0 || x1 == y1 || x1 == y2) n++;
	if (x2 == y0 || x2 == y1 || x2 == y2) n++;

	check(n != 3);

	return n >= 2;
}