// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CubeDMIMod.generated.h"

class UNiagaraSystem;

UCLASS()
class GAM415_ASERRANO_API ACubeDMIMod : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubeDMIMod();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Box collision component to detect overlaps or hits
	UPROPERTY(EditAnywhere)
	UBoxComponent* boxComp;

	// Visual mesh of the actor
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* cubeMesh;

	// Base decal material used for splash effects on impact
	UPROPERTY(EditAnywhere)
	UMaterialInterface* baseMat;

	// Dynamic material instance to update color at runtime
	UPROPERTY()
	UMaterialInstanceDynamic* dmiMat;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* colorP;

	// Triggered when something begins overlapping this actor’s collision component
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
