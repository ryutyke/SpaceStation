// Fill out your copyright notice in the Description page of Project Settings.


#include "VoronoiDiagram.h"

VoronoiDiagram::VoronoiDiagram()
{
	Triangulation.Clear();
	Sites = Triangulation.Vertices;
	Vertices.Empty();
	Edges.Empty();
	FirstEdgeBySite.Empty();
}

VoronoiDiagram::~VoronoiDiagram()
{
}

void VoronoiDiagram::Clear()
{
	Triangulation.Clear();
	Sites.Empty();
	Vertices.Empty();
	Edges.Empty();
	FirstEdgeBySite.Empty();
}
