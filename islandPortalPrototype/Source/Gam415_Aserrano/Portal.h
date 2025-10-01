// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"   // For Scene Capture 2D components
#include "Engine/TextureRenderTarget2D.h"         // For Render Target 2D textures
#include "Components/BoxComponent.h"              // For Box collision components
#include "Components/ArrowComponent.h"
#include "Portal.generated.h"

class AGam415_AserranoCharacter;

UCLASS()
class GAM415_ASERRANO_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mesh;  // The static mesh for the object

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneCaptureComponent2D* sceneCapture;  // The scene capture camera

	UPROPERTY(EditAnywhere)
	UArrowComponent* rootArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextureRenderTarget2D* renderTarget;  // The render target (texture) for the scene capture

	UPROPERTY(EditAnywhere)
	UBoxComponent* boxComp;  // The collision box for the portal

	UPROPERTY(EditAnywhere)
	APortal* OtherPortal;    // Reference to the paired portal

	UPROPERTY(EditAnywhere)
	UMaterialInterface* mat; // The material used for the portal surface

	UFUNCTION() 
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); // Runs when something enters the portal

	UFUNCTION()
	void SetBool(AGam415_AserranoCharacter* playerChar); // Sets a true/false value on the player (like teleporting)

	UFUNCTION()
	void UpdatePortals();

};
