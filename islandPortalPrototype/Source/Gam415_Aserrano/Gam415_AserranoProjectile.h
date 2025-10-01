// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gam415_AserranoProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS(config=Game)
class AGam415_AserranoProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	//ADD for Color 
// The visible mesh of the projectile (the ball)
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ballMesh;

	// The base decal material that gets spawned on surfaces after impact (used for the splash)
	UPROPERTY(EditAnywhere)
	UMaterial* baseMat;

	// Stores a randomly generated color to apply to both the projectile and the splash
	UPROPERTY(EditAnywhere)
	FLinearColor randColor;

	// The base material used on the projectile mesh (must have a Vector parameter like "ProjColor")
	UPROPERTY(EditAnywhere)
	UMaterialInterface* projMat;

	// A dynamic version of the material that lets us update color at runtime
	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* dmiMat;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* colorP;

public:
	// Constructor: sets default values for this actor
	AGam415_AserranoProjectile();

protected:
	// ADD: Protected section begins here for internal logic (like BeginPlay or custom functions)
	virtual void BeginPlay();


public:
	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

