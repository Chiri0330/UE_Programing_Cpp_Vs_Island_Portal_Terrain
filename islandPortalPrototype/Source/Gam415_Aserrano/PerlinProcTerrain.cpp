// Fill out your copyright notice in the Description page of Project Settings.
#include "PerlinProcTerrain.h" // Include the header for our custom terrain class
#include "ProceduralMeshComponent.h" // Include Unreal's procedural mesh component
#include "KismetProceduralMeshLibrary.h" // Include Kismet functions for procedural mesh

// Sets default values
APerlinProcTerrain::APerlinProcTerrain()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // Disable ticking since we don't need updates every frame

	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Procedural Mesh"); // Create the procedural mesh component
	ProcMesh->SetupAttachment(GetRootComponent()); // Attach mesh to the actor's root
}

// Called when the game starts or when spawned
void APerlinProcTerrain::BeginPlay()
{
	Super::BeginPlay(); // Always call parent BeginPlay

	CreateVertices(); // Generate all vertex positions
	CreateTriangles(); // Connect vertices into triangles
	ProcMesh->CreateMeshSection(sectionID, Vertices, Triangles, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>(), true); // Create mesh with our data
	ProcMesh->SetMaterial(0, Mat); // Set the chosen material on the mesh
}

// Called every frame
void APerlinProcTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // Always call parent Tick (here just in case, though tick is off)
}

void APerlinProcTerrain::AlterMesh(FVector impactPoint)
{
	for (int i = 0; i < Vertices.Num(); i++) // Loop through all vertices
	{
		FVector tempVector = impactPoint - this->GetActorLocation(); // Calculate the hit position relative to the actor

		if (FVector(Vertices[i] - tempVector).Size() < radius) // If this vertex is within the radius of impact
		{
			Vertices[i] = Vertices[i] - Depth; // Move the vertex by Depth amount (simulate digging/deformation)
			ProcMesh->UpdateMeshSection(sectionID, Vertices, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>()); // Update the mesh with new vertex positions
		}
	}
}

void APerlinProcTerrain::CreateVertices()
{
	for (int X = 0; X <= XSize; X++) // Loop over X grid points
	{
		for (int Y = 0; Y <= YSize; Y++) // Loop over Y grid points
		{
			float Z = FMath::PerlinNoise2D(FVector2D(X * NoiseScale + 0.1, Y * NoiseScale + 0.1)) * ZMultiplier; // Generate height using Perlin noise
			GEngine->AddOnScreenDebugMessage(-1, 999.0f, FColor::Yellow, FString::Printf(TEXT("Z %f"), Z)); // Print height for debugging
			Vertices.Add(FVector(X * Scale, Y * Scale, Z)); // Add the new vertex position to the array
			UV0.Add(FVector2D(X * UVScale, Y * UVScale)); // Add matching UV coordinates for texturing
		}
	}
}

void APerlinProcTerrain::CreateTriangles()
{
	int Vertex = 0; // Keeps track of current vertex index

	for (int X = 0; X < XSize; X++) // Loop through each grid square in X
	{
		for (int Y = 0; Y < YSize; Y++) // Loop through each grid square in Y
		{
			Triangles.Add(Vertex); // First triangle index 1
			Triangles.Add(Vertex + 1); // First triangle index 2
			Triangles.Add(Vertex + YSize + 1); // First triangle index 3

			Triangles.Add(Vertex + 1); // Second triangle index 1
			Triangles.Add(Vertex + YSize + 2); // Second triangle index 2
			Triangles.Add(Vertex + YSize + 1); // Second triangle index 3

			Vertex++; // Move to next vertex in Y
		}

		Vertex++; // Skip to next row in X
	}
};
