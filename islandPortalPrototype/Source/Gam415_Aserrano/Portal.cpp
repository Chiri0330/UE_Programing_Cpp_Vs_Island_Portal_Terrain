// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "Gam415_AserranoCharacter.h" // Include the header for the custom character
#include "Kismet/GameplayStatics.h" // Include Unreal's gameplay utility functions


// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh"); // Creates the static mesh (main 3D object)
	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Comp"); // Creates the box collider for detecting overlaps
	sceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("Capture"); // Sets up the scene capture camera
	rootArrow = CreateDefaultSubobject<UArrowComponent>("Root Arrow");

	RootComponent = boxComp; // Set the box collider as the root of the actor
	mesh->SetupAttachment(boxComp); // Attach the mesh to the box collider
	sceneCapture->SetupAttachment(mesh); // Attach the scene capture to the mesh
	rootArrow->SetupAttachment(RootComponent);

	mesh->SetCollisionResponseToAllChannels(ECR_Ignore); // Ignore all collisions for the mesh


}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin); // Set up an event for when something overlaps the box

	mesh->SetHiddenInSceneCapture(true); // Hide the mesh in any scene capture (like a camera or portal view)
	//mesh->bCastStaticShadow(false);
	//mesh->bCastDynamicShadow(false);

	if (mat) // If a material is assigned
	{
		mesh->SetMaterial(0, mat); // Set the mesh's material to the selected one
	}
	
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePortals();

}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGam415_AserranoCharacter* playerChar = Cast<AGam415_AserranoCharacter>(OtherActor); // Try to cast the other actor to our player character

	if (playerChar) // If the cast worked and we found the player
	{
		if (OtherPortal) // If the other portal exists
		{
			if (!playerChar->isTeleporting) // If the player is not already teleporting
			{
				playerChar->isTeleporting = true; // Set teleporting flag to true
				FVector loc = OtherPortal->rootArrow->GetComponentLocation(); // Get the other portal's location
				playerChar->SetActorLocation(loc); // Move the player to the other portal's location

				FTimerHandle TimerHandle; // Handle for managing the timer
				FTimerDelegate TimerDelegate; // Delegate to bind a function for the timer
				TimerDelegate.BindUFunction(this, "SetBool", playerChar); // Bind the SetBool function to the timer delegate
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false); // Set a timer to call SetBool after 1 second (does not loop)

			}
		}
	}
}

void APortal::SetBool(AGam415_AserranoCharacter* playerChar)
{
	if (playerChar)
	{
		playerChar->isTeleporting = false;
	}
}

void APortal::UpdatePortals()
{
	

	FVector Location = this->GetActorLocation() - OtherPortal->GetActorLocation(); // Get the offset between portals
	FVector camLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentLocation(); // Get the player's camera location
	FRotator camRotation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentRotation(); // Get the player's camera rotation
	FVector CombinedLocation = camLocation + Location; // Add two vectors together to get a new position

	sceneCapture->SetWorldLocationAndRotation(CombinedLocation, camRotation); // Move and rotate the scene capture camera
}

