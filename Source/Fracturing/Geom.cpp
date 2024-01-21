// Fill out your copyright notice in the Description page of Project Settings.


#include "Geom.h"

Geom::Geom()
{
}

Geom::~Geom()
{
}

bool Geom::AreCoincident(FVector2D a, FVector2D b)
{
	return (a - b).Size() < 0.00001f;
}

bool Geom::ToTheLeft(FVector2D p, FVector2D l0, FVector2D l1)
{
	return  ((l1.X - l0.X) * (p.Y - l0.Y) - (l1.Y - l0.Y) * (p.X - l0.X)) >= 0;
}

bool Geom::ToTheRight(FVector2D p, FVector2D l0, FVector2D l1)
{
	return !ToTheLeft(p, l0, l1);
}

bool Geom::PointInTriangle(FVector2D p, FVector2D c0, FVector2D c1, FVector2D c2)
{
	return ToTheLeft(p, c0, c1)
		&& ToTheLeft(p, c1, c2)
		&& ToTheLeft(p, c2, c0);
}

bool Geom::InsideCircumcircle(FVector2D p, FVector2D c0, FVector2D c1, FVector2D c2)
{
	double ax = c0.X - p.X;
	double ay = c0.Y - p.Y;
	double bx = c1.X - p.X;
	double by = c1.Y - p.Y;
	double cx = c2.X - p.X;
	double cy = c2.Y - p.Y;

	return (
		(ax * ax + ay * ay) * (bx * cy - cx * by) -
		(bx * bx + by * by) * (ax * cy - cx * ay) +
		(cx * cx + cy * cy) * (ax * by - bx * ay)
		) > 0.000001f;
}

FVector2D Geom::RotateRightAngle(FVector2D v)
{
	double x = v.X;
	v.X = -v.Y;
	v.Y = x;

	return v;
}

bool Geom::LineLineIntersection(FVector2D p0, FVector2D v0, FVector2D p1, FVector2D v1, float& m0, float& m1)
{
	float det = (v0.X * v1.Y - v0.Y * v1.X);

	if (abs(det) < 0.001f) {
		m0 = NAN;
		m1 = NAN;
		return false;
	}
	else {
		m0 = ((p0.Y - p1.Y) * v1.X - (p0.X - p1.X) * v1.Y) / det;

		if (abs(v1.X) >= 0.001f) {
			m1 = (p0.X + m0 * v0.X - p1.X) / v1.X;
		}
		else {
			m1 = (p0.Y + m0 * v0.Y - p1.Y) / v1.Y;
		}
		return true;
	}
}

FVector2D Geom::LineLineIntersection(FVector2D p0, FVector2D v0, FVector2D p1, FVector2D v1)
{
	float m0, m1;
	if (LineLineIntersection(p0, v0, p1, v1, m0, m1)) {
		return p0 + m0 * v0;
	}
	else {
		return FVector2D(NAN, NAN);
	}
}

FVector2D Geom::CircumcircleCenter(FVector2D c0, FVector2D c1, FVector2D c2)
{
	FVector2D mp0 = 0.5f * (c0 + c1);
	FVector2D mp1 = 0.5f * (c1 + c2);

	FVector2D v0 = RotateRightAngle(c0 - c1);
	FVector2D v1 = RotateRightAngle(c1 - c2);

	float m0, m1;

	LineLineIntersection(mp0, v0, mp1, v1, m0, m1);

	return mp0 + m0 * v0;
}

FVector2D Geom::TriangleCentroid(FVector2D c0, FVector2D c1, FVector2D c2)
{
	FVector2D val = (1.0f / 3.0f) * (c0 + c1 + c2);
	return val;
}

float Geom::Area(TArray<FVector2D> polygon)
{
	float area = 0.0f;

	int count = polygon.Num();

	for (int i = 0; i < count; i++) {
		int j = (i == count - 1) ? 0 : (i + 1);
		FVector2D p0 = polygon[i];
		p0 /= 182.186; // 추가했음
		FVector2D p1 = polygon[j];
		p1 /= 172.414; // 추가했음

		area += p0.X * p1.Y - p1.Y * p1.X;
		UE_LOG(LogTemp, Warning, TEXT("Area = %f"), 0.5f * area); // 추가했음
	}
	return 0.5f * area;
}
