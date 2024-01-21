// Fill out your copyright notice in the Description page of Project Settings.


#include "test.h"

// Sets default values
Atest::Atest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_Area = -1.0;
	Age = 0;

	DistributionFactor = 4;
	SiteNum = 12;

	SurfaceMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("SurfaceMesh"));
	RootComponent = SurfaceMesh;

	// ���� �ù����̼� ����
	SurfaceMesh->SetSimulatePhysics(true);
	SurfaceMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	SurfaceMesh->bUseComplexAsSimpleCollision = false;
	/*Reload();
	SurfaceMesh->CreateMeshSection(0, verts, tris, norms, TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);*/
}

// Called when the game starts or when spawned
void Atest::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void Atest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Polygon ���� �ʱ�ȭ
void Atest::Reload()
{
	FVector Pos = GetActorLocation();
	//Polygon.Empty();
	if (Polygon.Num() == 0)
	{
		FVector Scale = 1.0f * GetActorScale();
		Polygon.Add(FVector2D(-Scale.X, -Scale.Y));
		Polygon.Add(FVector2D(Scale.X, -Scale.Y));
		Polygon.Add(FVector2D(Scale.X, Scale.Y));
		Polygon.Add(FVector2D(-Scale.X, Scale.Y));
		Thickness = 10.0f;
		SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
	MeshFromPolygon(Polygon, Thickness);
}

void Atest::OnCollision(AActor* OtherActor, const FHitResult& Hit)
{
	if (Age > 5 && Hit.ImpactNormal.Size() > MinImpactToBreak)
	{
		FVector2D ImpactPoint(Hit.ImpactPoint.X - GetActorLocation().X, Hit.ImpactPoint.Y - GetActorLocation().Y);
		Break(ImpactPoint);
	}
}

// Area get �Լ�
float Atest::Area()
{
	if (_Area < 0.0f)
	{
		_Area = Geom::Area(Polygon);
	}
	return _Area;
}

TArray<FVector2D> Atest::Break(FVector2D Position)
{
	float area = Area();
	if (true) //if (area > MinBreakArea)
	{
		VoronoiCalculator calc = VoronoiCalculator();
		VoronoiClipper clip = VoronoiClipper();
		
		TArray<FVector2D> sites;

		float distance = (Position).Size();

		if (distance <= 100)
		{
			sites.SetNum(20);
		}
		else if (100 < distance && distance <= 200)
		{
			sites.SetNum(10);
		}
		else if (200 < distance && distance <= 300)
		{
			sites.SetNum(7);
		}
		else
		{
			sites.SetNum(4);
		}
		// [변경 가능] sites 개수 조절 가능
		//sites.SetNum(SiteNum);

		for (int i = 0; i < sites.Num(); i++)
		{
			float dist = abs(NormalizedRandom(10.0f, 1.0f / 2.0f));
			float angle = 2.0f * PI * FMath::FRand();
			// [변경 가능] 여기 아래 FVector2D 에 곱해지는 값으로 voronoi site 분포 넓히거나 좁힐 수 있음
			sites[i] = Position + DistributionFactor * FVector2D(dist * cos(angle), dist * sin(angle));
		}
		//for (int t = 0; t < sites.Num(); t++)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("sites.X : %f"), sites[t].X);
		//	UE_LOG(LogTemp, Warning, TEXT("sites.Y : %f"), sites[t].Y);
		//}


		VoronoiDiagram diagram = calc.CalculateDiagram(sites);

		TArray<FVector2D> clipped;

		for (int i = 0; i < sites.Num(); i++)
		{
			clip.ClipSite(diagram, Polygon, i, clipped);

			UE_LOG(LogTemp, Warning, TEXT("clipped.Num : %d"), clipped.Num());
			UE_LOG(LogTemp, Warning, TEXT("----------------------"));


			if (clipped.Num() > 0)
			{
				//UE_LOG(LogTemp, Warning, TEXT("clipped.Num : %d"), clipped.Num());

				//for (int t = 0; t < clipped.Num(); t++)
				//{
				//	UE_LOG(LogTemp, Warning, TEXT("clipped.X : %f"), clipped[t].X);
				//	UE_LOG(LogTemp, Warning, TEXT("clipped.Y : %f"), clipped[t].Y);
				//}
				Atest* BreakMesh = GetWorld()->SpawnActor<Atest>(GetClass(), FVector::ZeroVector, FRotator::ZeroRotator);
				BreakMesh->SetActorLocation(GetActorLocation());
				BreakMesh->SetActorRotation(GetActorRotation());

				if (BreakMesh)
				{
					BreakMesh->Thickness = Thickness;
					BreakMesh->Polygon.Empty();
					BreakMesh->Polygon = clipped;

					float ChildArea = BreakMesh->Area();
					BreakMesh->SurfaceMesh->SetMassOverrideInKg(NAME_None, SurfaceMesh->GetMass() * (ChildArea / _Area));
					BreakMesh->Createmesh();
					BreakMesh->SurfaceMesh->SetSimulatePhysics(true);
					//부서진 파편에 총알 안 맞게
					BreakMesh->SurfaceMesh->SetCollisionProfileName(TEXT("NoDamage"));
					// 중력 끄기
					BreakMesh->SurfaceMesh->SetEnableGravity(false);
					// 공간 돌아왔을 때 파편 남아있는 경우 있어서
					BreakMesh->SetLifeSpan(3.0f);
				}
			}
		}
		SetActorHiddenInGame(true);
		Destroy();

		return sites;
	}
	TArray<FVector2D> NullReturn;
	return NullReturn;
}

float Atest::NormalizedRandom(float mean, float stddev)
{
	float u1 = FMath::FRand();
	float u2 = FMath::FRand();

	float randStdNormal = sqrt(-2.0f * log(u1)) * sin(2.0f * PI * u2);

	return mean + stddev * randStdNormal;
}

void Atest::MeshFromPolygon(TArray<FVector2D> polygon, float thickness)
{
	int count = Polygon.Num();

	float ext = Thickness;

	// [변경 가능] verts.Add 에 있는 ext 부호 다 바꾸면 더 잘 보이긴 함
	// Top
	for (int i = 0; i < count; i++)
	{
		verts.Add(FVector(Polygon[i].X, -ext, Polygon[i].Y));
		norms.Add(FVector::ForwardVector);
	}

	// Bottom
	for (int i = 0; i < count; i++)
	{
		verts.Add(FVector(Polygon[i].X, ext, Polygon[i].Y));
		norms.Add(FVector::BackwardVector);
	}

	// sides
	for (int i = 0; i < count; i++) {
		int iNext = i == count - 1 ? 0 : i + 1;

		verts.Add(FVector(Polygon[i].X, -ext, Polygon[i].Y));
		verts.Add(FVector(Polygon[i].X, ext, Polygon[i].Y));
		verts.Add(FVector(Polygon[iNext].X, ext, Polygon[iNext].Y));
		verts.Add(FVector(Polygon[iNext].X, -ext, Polygon[iNext].Y));

		FVector norm = FVector::CrossProduct(FVector(Polygon[iNext].X - Polygon[i].X, ext, Polygon[iNext].Y - Polygon[i].Y), FVector::ForwardVector).GetSafeNormal();

		norms.Add(norm);
		norms.Add(norm);
		norms.Add(norm);
		norms.Add(norm);
	}

	for (int vert = count - 1; vert >= 2; vert--) {
		tris.Add(vert);
		tris.Add(vert - 1);
		tris.Add(0);
	}

	for (int vert = count - 1; vert >= 2; vert--) {
		
		tris.Add(count + vert - 1);
		tris.Add(count + vert);
		tris.Add(count);
	}

	for (int vert = count - 1; vert >= 0; vert--) {
		int si = (2 * count) + (4 * vert);

		tris.Add(si + 2);
		tris.Add(si + 1);
		tris.Add(si);
		
		tris.Add(si + 3);
		tris.Add(si + 2);
		tris.Add(si);
	}
}

TArray<FVector> Atest::GetVertexs()
{
	return verts;
}

TArray<FVector> Atest::GetNormals()
{
	return norms;
}

TArray<int> Atest::GetTriangles()
{
	return tris;
}

void Atest::Createmesh()
{
	Reload();
	SurfaceMesh->CreateMeshSection(0, verts, tris, norms, TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	SurfaceMesh->AddCollisionConvexMesh(verts);
}
