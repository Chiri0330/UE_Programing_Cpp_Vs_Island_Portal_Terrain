// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProcPlane.generated.h"


class UProceduralMeshComponent;

UCLASS()
class GAM415_ASERRANO_API AProcPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProcPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called right after the actor is created in the editor or at runtime
	virtual void PostActorCreated() override;

	// Called after the actor is loaded from disk (e.g., when opening a level)
	virtual void PostLoad() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Stores the positions of the vertices that make up the mesh
	UPROPERTY(EditAnywhere)
	TArray<FVector> Vertices;

	// Stores the index order to connect vertices into triangles
	UPROPERTY(EditAnywhere)
	TArray<int> Triangles;

	UPROPERTY(EditAnywhere)
	TArray<FVector2D> UV0;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* PlaneMat;

	// Declares a function that will create the mesh when called
	UFUNCTION()
	void CreateMesh();

private:
	UProceduralMeshComponent* proMesh;

};
