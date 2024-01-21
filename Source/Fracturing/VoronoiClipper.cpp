// Fill out your copyright notice in the Description page of Project Settings.


#include "VoronoiClipper.h"

VoronoiClipper::VoronoiClipper()
{
}

VoronoiClipper::~VoronoiClipper()
{
}

void VoronoiClipper::ClipSite(VoronoiDiagram diag, TArray<FVector2D> polygon, int site, TArray<FVector2D>& clipped)
{
	pointsIn.Empty();

	pointsIn = polygon;

	int firstEdge, lastEdge;

	//UE_LOG(LogTemp, Warning, TEXT("site : %d"), site);
	//UE_LOG(LogTemp, Warning, TEXT("diag.Sites.Num() : %d"), diag.Sites.Num());
	//UE_LOG(LogTemp, Warning, TEXT("diag.Edges.Num() : %d"), diag.Edges.Num());
	//for (int t = 0; t < diag.FirstEdgeBySite.Num(); t++)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("FirstEdgeBySite%d : %d"), t, diag.FirstEdgeBySite[t]);
	//}

	if (site == diag.Sites.Num() - 1) 
	{
		firstEdge = diag.FirstEdgeBySite[site];
		lastEdge = diag.Edges.Num() - 1;
	}
	else {
		firstEdge = diag.FirstEdgeBySite[site];
		lastEdge = diag.FirstEdgeBySite[site + 1] - 1;
	}

	for (int ei = firstEdge; ei <= lastEdge; ei++) {
		//UE_LOG(LogTemp, Warning, TEXT("TEST"));
		pointsOut.Empty();

		VoronoiDiagram::Edge edge = diag.Edges[ei];

		FVector2D lp, ld;

		if (edge.Type == VoronoiDiagram::EdgeType::RayCCW || edge.Type == VoronoiDiagram::EdgeType::RayCW)
		{
			lp = diag.Vertices[edge.Vert0];
			ld = edge.Direction;
			if (edge.Type == VoronoiDiagram::EdgeType::RayCW) {
				ld *= -1;
			}
		}
		else if (edge.Type == VoronoiDiagram::EdgeType::Segment)
		{
			FVector2D lp0 = diag.Vertices[edge.Vert0];
			FVector2D lp1 = diag.Vertices[edge.Vert1];

			lp = lp0;
			ld = lp1 - lp0;
		}
		else if (edge.Type == VoronoiDiagram::EdgeType::Line)
		{
			UE_LOG(LogTemp, Warning, TEXT("Haven't implemented voronoi halfplanes yet"));
		}
		else {
			check(false);
			return;
		}

		for (int pi0 = 0; pi0 < pointsIn.Num(); pi0++)
		{
			int pi1 = pi0 == pointsIn.Num() - 1 ? 0 : pi0 + 1;

			FVector2D p0 = pointsIn[pi0];
			FVector2D p1 = pointsIn[pi1];

			bool p0Inside = Geom::ToTheLeft(p0, lp, lp + ld);
			bool p1Inside = Geom::ToTheLeft(p1, lp, lp + ld);

			if (p0Inside && p1Inside) 
			{
				pointsOut.Add(p1);
			}
			else if (!p0Inside && !p1Inside) 
			{
				// Do nothing, both are outside
			}
			else {
				FVector2D intersection = Geom::LineLineIntersection(lp, ld.GetSafeNormal(), p0, (p1 - p0).GetSafeNormal());
				
				if (p0Inside) {
					pointsOut.Add(intersection);
				}
				else if (p1Inside) {
					pointsOut.Add(intersection);
					pointsOut.Add(p1);
				}
				else {
					check(false);
				}
			}
		}

		TArray<FVector2D> tmp = pointsIn;
		pointsIn = pointsOut;
		pointsOut = tmp;
	}

	if (clipped.IsEmpty()) 
	{
		clipped = TArray<FVector2D>();
	}
	else 
	{
		clipped.Empty();
	}

	clipped = pointsIn;
}
