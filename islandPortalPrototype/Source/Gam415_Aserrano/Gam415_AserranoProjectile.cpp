// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gam415_AserranoProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "PerlinProcTerrain.h"

AGam415_AserranoProjectile::AGam415_AserranoProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGam415_AserranoProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;



	// Create the visual mesh component for the projectile and name it "Ball Mesh" for clarity in the editor
	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>("Ball Mesh");



	// Set as root component
	RootComponent = CollisionComp;



	// Attach the ball mesh to the collision component so it follows its position and rotation
	ballMesh->SetupAttachment(CollisionComp);




	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	// THIS ONE LINE FIXES THE CRASH ( Color in the splash and ball matching)
	baseMat = LoadObject<UMaterial>(nullptr, TEXT("/Game/Splatter/Splat1_Mat.Splat1_Mat"));

	// FIX: Load the projectile material so ball color works
	projMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Splatter/Projectile_Color.Projectile_Color"));
}


//ADD For the Projactile
void AGam415_AserranoProjectile::BeginPlay()
{
	// Call the parent class's BeginPlay() to ensure any inherited logic runs first
	Super::BeginPlay();

	// Generate a random RGBA color with full opacity (Alpha = 1.0)
	randColor = FLinearColor(UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), 1.f);


	// Create a dynamic material instance from the projectile's base material
	dmiMat = UMaterialInstanceDynamic::Create(projMat, this);

	// Apply the dynamic material to the projectile mesh
	ballMesh->SetMaterial(0, dmiMat);

	// Set the "ProjColor" parameter in the material to the generated random color
	dmiMat->SetVectorParameterValue("ProjColor", randColor);
}


void AGam415_AserranoProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();

	}

	// Check if the projectile hit a valid actor before spawning the decal
	if (OtherActor != nullptr)
	{

		if (colorP)
		{
			// Spawn the Niagara particle system and attach it to HitComp
			UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(colorP, HitComp, NAME_None, FVector(-20.f, 0.f, 0.f), FRotator(0.f), EAttachLocation::KeepRelativeOffset, true);

			// Set the Niagara system's color parameter using the random color value
			particleComp->SetNiagaraVariableLinearColor(FString("RandomColor"), randColor);

			// Destroy the ball mesh component after spawning the effect
			ballMesh->DestroyComponent();

			// Set the collision component's profile to "NoCollision"
			CollisionComp->BodyInstance.SetCollisionProfileName("NoCollision");
		}

		// Generate a random frame number (used for selecting a frame in a flipbook-style material, if applicable)
		float frameNum = UKismetMathLibrary::RandomFloatInRange(0.f, 3.f);

		// Spawn a decal at the hit location with random size and the surface's normal rotation
		auto Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), baseMat, FVector(UKismetMathLibrary::RandomFloatInRange(20.f, 40.f)), Hit.Location, Hit.Normal.Rotation(), 0.f);

		// Create a dynamic instance of the decal material so we can change its parameters
		auto MatInstance = Decal->CreateDynamicMaterialInstance();

		// Set the decal's color to match the projectile's random color
		MatInstance->SetVectorParameterValue("Color", randColor);

		// Set the frame value to the randomly generated number (for animated/flipbook decals)
		MatInstance->SetScalarParameterValue("Frame", frameNum);


		// Try to convert (cast) the actor we hit into our custom terrain class
		APerlinProcTerrain* procTerrain = Cast<APerlinProcTerrain>(OtherActor);

		// If the cast is successful (the actor is a PerlinProcTerrain)
		if (procTerrain)
		{
			// Call the function to modify the mesh at the hit location
			procTerrain->AlterMesh(Hit.ImpactPoint);
		}

	}
};