#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PerlinProcTerrain.generated.h"

// Forward declarations for components and materials
class UProceduralMeshComponent;
class UMaterialInterface;

// This is our custom terrain actor class using Perlin noise
UCLASS()
class GAM415_ASERRANO_API APerlinProcTerrain : public AActor
{
    GENERATED_BODY()

public:
    // Constructor: sets default values when the terrain is created
    APerlinProcTerrain();

    // The width of the terrain in grid units (editable in editor)
    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
    int XSize = 0;

    // The depth of the terrain in grid units (editable in editor)
    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
    int YSize = 0;

    // How tall the terrain features can be (editable in editor and Blueprints)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
    float ZMultiplier = 1.0f;

    // Controls the scale of the Perlin noise pattern (editable)
    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
    float NoiseScale = 1.0f;

    // Used for advanced scaling (must not be zero due to clamp)
    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
    float Scale = 0.000001f;

    // Controls how UVs (texture mapping) are scaled across the surface
    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
    float UVScale = 0.000001f;

    // Optional: could control how much you can dig or change the terrain at a point
    UPROPERTY(EditAnywhere)
    float radius;

    // Lets you define a custom depth as a vector (x, y, z)
    UPROPERTY(EditAnywhere)
    FVector Depth;

protected:
    // This function is called when the game starts or when the actor is spawned
    virtual void BeginPlay() override;

    // The material applied to the mesh surface
    UPROPERTY(EditAnywhere)
    UMaterialInterface* Mat;

public:
    // Called every frame to update the terrain if needed
    virtual void Tick(float DeltaTime) override;

    // Function to change or alter the mesh at a specific point (callable in Blueprints)
    UFUNCTION(BlueprintCallable)
    void AlterMesh(FVector impactPoint);

private:
    // The mesh component that actually draws the terrain
    UProceduralMeshComponent* ProcMesh;

    // List of vertices for the terrain mesh
    TArray<FVector> Vertices;

    // List of triangle indices for mesh faces
    TArray<int> Triangles;

    // List of UV coordinates for texture mapping
    TArray<FVector2D> UV0;

    // List of normal vectors for lighting calculations
    TArray<FVector> Normals;

    // Optional: vertex colors for effects
    TArray<FColor> UpVertexColors;

    // Keeps track of mesh section IDs if you have more than one mesh section
    int sectionID = 0;

    // Helper function to create the vertex positions for the mesh
    void CreateVertices();

    // Helper function to create the triangles (mesh faces) using the vertices
    void CreateTriangles();
};