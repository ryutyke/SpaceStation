// Fill out your copyright notice in the Description page of Project Settings.


#include "DelaunayCalculator.h"

DelaunayCalculator::DelaunayCalculator()
{
	highest = -1;
	verts.Empty();
	triangles.Empty();
	indices.Empty();
}

DelaunayCalculator::~DelaunayCalculator()
{
}

// vertex 정보를 입력받아 돌로네 삼각분할을 반환
DelaunayTriangulation DelaunayCalculator::CalculateTriangulation(TArray<FVector2D> _verts)
{
	DelaunayTriangulation result;
	voidCalculateTriangulation(_verts, result);
	return result;
}

// vertex 정보를 입력받아 돌로네 삼각분할 계산
void DelaunayCalculator::voidCalculateTriangulation(TArray<FVector2D> _verts, DelaunayTriangulation& result)
{
	if (_verts.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("points"));
	}

	if (_verts.Num() < 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("You need at least 3 points for a triangulation"));
	}

	triangles.Empty();
	this->verts = _verts;

	highest = 0;

	for (int i = 0; i < _verts.Num(); i++)
	{
		if (Higher(highest, i))
		{
			highest = i;
		}
	}

	triangles.Add(TriangleNode(-2, -1, highest));

	RunBowyerWaston();
	GenerateResult(result);

	this->verts.Empty();
}

// 두 점의 Y좌표중 더 높은 점을 찾는 함수
bool DelaunayCalculator::Higher(int pi0, int pi1)
{
	if (pi0 == -2) { return false; }
	else if (pi0 == -1) { return true; }
	else if (pi1 == -2) { return true; }
	else if (pi1 == -1) { return false; }
	else 
	{
		FVector2D p0 = verts[pi0];
		FVector2D p1 = verts[pi1];

		if (p0.Y < p1.Y) { return true; }
		else if (p0.Y > p1.Y) { return false; }
		else { return p0.X < p1.X; }
	}
	return false;
}


// BowyerWaston 알고리즘
void DelaunayCalculator::RunBowyerWaston()
{
	for (int i = 0; i < verts.Num(); i++)
	{
		int pi = i;

		if (pi == highest) continue;

		int ti = FindTriangleNode(pi);

		TriangleNode t = triangles[ti];

		int p0 = t.P0;
		int p1 = t.P1;
		int p2 = t.P2;

		int nti0 = triangles.Num();
		int nti1 = nti0 + 1;
		int nti2 = nti0 + 2;

		TriangleNode nt0 = TriangleNode(pi, p0, p1);
		TriangleNode nt1 = TriangleNode(pi, p1, p2);
		TriangleNode nt2 = TriangleNode(pi, p2, p0);

		nt0.A0 = t.A2;
		nt1.A0 = t.A0;
		nt2.A0 = t.A1;

		nt0.A1 = nti1;
		nt1.A1 = nti2;
		nt2.A1 = nti0;

		nt0.A2 = nti2;
		nt1.A2 = nti0;
		nt2.A2 = nti1;

		t.C0 = nti0;
		t.C1 = nti1;
		t.C2 = nti2;

		triangles[ti] = t;

		triangles.Add(nt0);
		triangles.Add(nt1);
		triangles.Add(nt2);

		if (nt0.A0 != -1) LegalizeEdge(nti0, nt0.A0, pi, p0, p1);
		if (nt1.A0 != -1) LegalizeEdge(nti1, nt1.A0, pi, p1, p2);
		if (nt2.A0 != -1) LegalizeEdge(nti2, nt2.A0, pi, p2, p0);
	}
}

// 계산된 삼각분할 결과를 result에 저장
void DelaunayCalculator::GenerateResult(DelaunayTriangulation& result)
{
	if (result.IsEmpty())
	{
		result = DelaunayTriangulation();
	}

	result.Clear();

	for (int i = 0; i < verts.Num(); i++)
	{
		result.Vertices.Add(verts[i]);
	}

	for (int i = 1; i < triangles.Num(); i++)
	{
		TriangleNode t = triangles[i];

		if (t.IsLeaf() && t.IsInner())
		{
			result.Triangles.Add(t.P0);
			result.Triangles.Add(t.P1);
			result.Triangles.Add(t.P2);
		}
	}
}

// 입력 데이터의 순서를 섞는 함수
void DelaunayCalculator::ShuffleIndices()
{
	indices.Empty();
	indices.Reserve(verts.Num());

	for (int i = 0; i < verts.Num(); i++)
	{
		indices.Add(i);
	}

	check(indices.Num() == verts.Num());

	for (int i = 0; i < verts.Num() - 1; i++)
	{
		int j = FMath::RandRange(i, verts.Num() - 1);

		int tmp = indices[i];
		indices[i] = indices[j];
		indices[j] = tmp;
	}
}

// 주어진 엣지를 가지고 리프 노드를 찾는 함수
int DelaunayCalculator::LeafWithEdge(int ti, int e0, int e1)
{
	check(triangles[ti].HasEdge(e0, e1));

	while (!triangles[ti].IsLeaf())
	{
		TriangleNode t = triangles[ti];

		if (t.C0 != -1 && triangles[t.C0].HasEdge(e0, e1)) {
			ti = t.C0;
		}
		else if (t.C1 != -1 && triangles[t.C1].HasEdge(e0, e1)) {
			ti = t.C1;
		}
		else if (t.C2 != -1 && triangles[t.C2].HasEdge(e0, e1)) {
			ti = t.C2;
		}
		else {
			check(false);
			UE_LOG(LogTemp, Warning, TEXT("This should never happen"));
		}
	}

	return ti;
}

// 엣지가 적합한지 검사하고 엣지를 적합하게 만드는 함수
bool DelaunayCalculator::LegalEdge(int k, int l, int i, int j)
{
	check(k != highest && k >= 0);

	bool lMagic = l < 0;
	bool iMagic = i < 0;
	bool jMagic = j < 0;

	check(!(iMagic && jMagic));

	if (lMagic) { return true; }
	else if (iMagic)
	{
		check(!jMagic);

		FVector2D p = verts[l];
		FVector2D l0 = verts[k];
		FVector2D l1 = verts[j];

		return Geom::ToTheLeft(p, l0, l1);
	}
	else if (jMagic)
	{
		check(!iMagic);

		FVector2D p = verts[l];
		FVector2D l0 = verts[k];
		FVector2D l1 = verts[i];

		return !Geom::ToTheLeft(p, l0, l1);
	}
	else {
		check(k >= 0 && l >= 0 && i >= 0 && j >= 0);

		FVector2D p = verts[l];
		FVector2D c0 = verts[k];
		FVector2D c1 = verts[i];
		FVector2D c2 = verts[j];

		check(Geom::ToTheLeft(c2, c0, c1));
		check(Geom::ToTheLeft(c2, c1, p));

		return !Geom::InsideCircumcircle(p, c0, c1, c2);
	}
}

void DelaunayCalculator::LegalizeEdge(int ti0, int ti1, int pi, int li0, int li1)
{
	ti1 = LeafWithEdge(ti1, li0, li1);

	TriangleNode t0 = triangles[ti0];
	TriangleNode t1 = triangles[ti1];
	int qi = t1.OtherPoint(li0, li1);

	check(t0.HasEdge(li0, li1));
	check(t1.HasEdge(li0, li1));
	check(t0.IsLeaf());
	check(t1.IsLeaf());
	check(t0.P0 == pi || t0.P1 == pi || t0.P2 == pi);
	check(t1.P0 == qi || t1.P1 == qi || t1.P2 == qi);

	if (!LegalEdge(pi, qi, li0, li1)) {
		int ti2 = triangles.Num();
		int ti3 = ti2 + 1;

		TriangleNode t2 = TriangleNode(pi, li0, qi);
		TriangleNode t3 = TriangleNode(pi, qi, li1);

		t2.A0 = t1.Opposite(li1);
		t2.A1 = ti3;
		t2.A2 = t0.Opposite(li1);

		t3.A0 = t1.Opposite(li0);
		t3.A1 = t0.Opposite(li0);
		t3.A2 = ti2;

		triangles.Add(t2);
		triangles.Add(t3);

		TriangleNode nt0 = triangles[ti0];
		TriangleNode nt1 = triangles[ti1];

		nt0.C0 = ti2;
		nt0.C1 = ti3;

		nt1.C0 = ti2;
		nt1.C1 = ti3;

		triangles[ti0] = nt0;
		triangles[ti1] = nt1;

		if (t2.A0 != -1) LegalizeEdge(ti2, t2.A0, pi, li0, qi);
		if (t3.A0 != -1) LegalizeEdge(ti3, t3.A0, pi, qi, li1);
	}
}

// 특정 점이 속한 삼각형을 찾는 함수
int DelaunayCalculator::FindTriangleNode(int pi)
{
	int curr = 0;

	while (!triangles[curr].IsLeaf()) {
		TriangleNode t = triangles[curr];

		if (t.C0 >= 0 && PointInTriangle(pi, t.C0)) {
			curr = t.C0;
		}
		else if (t.C1 >= 0 && PointInTriangle(pi, t.C1)) {
			curr = t.C1;
		}
		else {
			curr = t.C2;
		}
	}
	return curr;
}

// 특정 점이 주어진 삼각형 내에 있는지 확인하는 함수
bool DelaunayCalculator::PointInTriangle(int pi, int ti)
{
	TriangleNode t = triangles[ti];
	return ToTheLeft(pi, t.P0, t.P1)
		&& ToTheLeft(pi, t.P1, t.P2)
		&& ToTheLeft(pi, t.P2, t.P0);
}

// 세 점의 위치 관계를 판단하여 특정 점이 왼쪽에 있는지 확인하는 함수
bool DelaunayCalculator::ToTheLeft(int pi, int li0, int li1)
{
	if (li0 == -2) 
	{
		return Higher(li1, pi);
	}
	else if (li0 == -1) 
	{
		return Higher(pi, li1);
	}
	else if (li1 == -2) 
	{
		return Higher(pi, li0);
	}
	else if (li1 == -1) 
	{
		return Higher(li0, pi);
	}
	else {
		check(li0 >= 0);
		check(li1 >= 0);

		return Geom::ToTheLeft(verts[pi], verts[li0], verts[li1]);
	}
}