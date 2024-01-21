// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "Geom.h"
#include "VoronoiCalculator.h"
#include "VoronoiClipper.h"
#include "GameFramework/Actor.h"
#include "test.generated.h"

UCLASS()
class FRACTURING_API Atest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Atest();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProceduralMeshComponent* SurfaceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector2D> Polygon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Thickness = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinBreakArea = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinImpactToBreak = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector> verts;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector> norms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<int> tris;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int DistributionFactor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int SiteNum;

	int Age;
	float _Area;

	UFUNCTION(BlueprintCallable)
	void OnCollision(AActor* OtherActor, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetVertexs();

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetNormals();
	
	UFUNCTION(BlueprintCallable)
	TArray<int> GetTriangles();

	UFUNCTION(BlueprintCallable)
	void Createmesh();

	UFUNCTION(BlueprintCallable)
	TArray<FVector2D> Break(FVector2D Position);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Reload();
	float Area();
	static float NormalizedRandom(float mean, float stddev);
	void MeshFromPolygon(TArray<FVector2D> polygon, float thickness);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
