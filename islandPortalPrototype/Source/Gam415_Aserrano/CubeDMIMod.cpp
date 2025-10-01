// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeDMIMod.h"
#include "Gam415_AserranoCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ACubeDMIMod::ACubeDMIMod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the collision box component and assign a name for identification in editor
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));

	// Create the static mesh component (visual mesh) and assign it a name
	cubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));

	// Set the root of this actor to the box component so it's the primary transform reference
	RootComponent = boxComp;

	// Attach the cube mesh to the box component so it follows its location and rotation
	cubeMesh->SetupAttachment(boxComp);
}

// Called when the game starts or when spawned
void ACubeDMIMod::BeginPlay()
{
	Super::BeginPlay(); // Call the base class BeginPlay()

	// Bind the overlap event to the OnOverlapBegin function for the box collision component
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ACubeDMIMod::OnOverlapBegin);

	// Check if a base material has been assigned
	if (baseMat)
	{
		// Create a dynamic material instance from the base material so we can modify its parameters at runtime
		dmiMat = UMaterialInstanceDynamic::Create(baseMat, this);
	}

	// Apply the dynamic material to the cube mesh if the mesh exists
	if (cubeMesh)
	{
		cubeMesh->SetMaterial(0, dmiMat);
	}
}

// Called every frame
void ACubeDMIMod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACubeDMIMod::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Attempt to cast the overlapping actor to your custom player character class
	AGam415_AserranoCharacter* overlappedActor = Cast<AGam415_AserranoCharacter>(OtherActor);

	// Proceed only if the cast was successful (meaning the overlapping actor is the correct character type)
	if (overlappedActor)
	{
		// Generate three random float values for RGB color channels between 0.0 and 1.0
		float ranNumX = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
		float ranNumY = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
		float ranNumZ = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);

		// Create a random color using the three random values
		FLinearColor randColor = FLinearColor(ranNumX, ranNumY, ranNumZ, 1.f); // Alpha is always 1 (fully opaque)

		// If the dynamic material is valid, apply the random color to it
		if (dmiMat)
		{
			// Set the "Color" parameter to the random color
			dmiMat->SetVectorParameterValue("Color", randColor);

			// Set the "Darkness" scalar value based on the red channel
			dmiMat->SetScalarParameterValue("Darkness", ranNumX);

			//to check if is set
			if (colorP)
			{

				UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(colorP, OtherComp, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::KeepRelativeOffset, true);

				// Set the Niagara system's color parameter using the random color value
				particleComp->SetNiagaraVariableLinearColor(FString("RandColor"), randColor);

			}
		}
	}
}







