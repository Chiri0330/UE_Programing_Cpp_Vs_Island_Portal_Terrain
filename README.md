# Portals & Islands – UE5 (C++-First Prototype)

A C++-driven Unreal Engine 5 prototype exploring **real-time portals**, **procedural terrain**, and **runtime material/FX programming**. The project contains three small maps that each demonstrate a focused programming technique; Blueprints are used sparingly for wiring/testing, while **gameplay and systems are implemented primarily in C++**.

- **Behance (screens & write-up):** https://www.behance.net/gallery/235452097/Collect-Resources-Build-PrototypeUE-C-BP
- **Vimeo (gameplay demo):** https://vimeo.com/1122365856
- **GitHub:** https://github.com/Chiri0330/collectResourcesProjectUE_BP_and_Cplusplus.git

---

## 🧭 Playable Maps (Programming Goals)

### 1) **Practice Map – Mirror Portal & Reactive Cube**
- **Goal:** Prove out the portal pipeline and show a small reactive object fully driven from code.
- **What’s coded:**
  - **Portal preview** using `USceneCaptureComponent2D` + `UTextureRenderTarget2D` and a custom **portal material** (screen-aligned UVs, emissive only).
  - **Reactive Cube** actor that **detects player proximity in C++** (overlap/trace), then:
    - Randomizes a `FLinearColor` in code.
    - **Sets a dynamic material parameter** at runtime for both the cube and a **splat decal**.
    - Spawns a Niagara **splash** on approach (color injected from C++).
- **Blueprint usage:** Minimal—only for placing the test actors.

---

### 2) **ProcTerrain Map – Procedural Mesh (C++)**
- **Goal:** Author terrain **entirely in code** with noise and show runtime mesh modification.
- **What’s coded:**
  - `APerlinProcTerrain` generates a grid (X×Y), computes height via **Perlin noise**, then builds triangles with `UProceduralMeshComponent`.
  - **Runtime editing**: a function like `AlterMesh(const FHitResult&)` deforms the mesh around an impact point (simulate digging/impacts).
  - Spawns the **gun** and uses the same **splat color system** when projectiles hit the terrain.
- **Result:** A fully green, stylized terrain with hills/valleys that you can shoot and “paint.”

---

### 3) **Island Map – Two-Way Portals + Weapon**
- **Goal:** Tie it all together—**teleport between two linked portals** and use the weapon system.
- **What’s coded:**
  - **Paired portals** (A ⟷ B):
    - Each portal has a capture component rendering the counterpart’s view to a **shared render target**.
    - Teleport in `OnOverlapBegin` with **transform remapping** (position & facing when stepping through).
    - Per-frame **UpdatePortals()** to keep captures in sync.
  - **Weapon/Projectile System:**
    - `AGun` (spawner/owner) calls `Fire()` → `SpawnActor<AGameProjectile>(…)`.
    - `AGameProjectile` uses `USphereComponent` + `UProjectileMovementComponent` for motion/bounce.
    - On impact, C++ triggers **Niagara splat** and **decal** with a **new random color** each shot; color is pushed via `SetVectorParameterValueOnMaterials/Systems`.
- **Experience:** Walk into a portal → pop out of the other island → pick up the gun → every shot leaves a different-colored splash.

---

## 🧩 Systems & Key Classes (C++)

> Class names below are representative; adjust to your exact names if needed.

### Character & Input
- `AFirstPersonCharacter`
  - Sets up camera & mesh.
  - Binds **Enhanced Input** (move/look/jump/fire).
  - Bridges to `AGun::Fire()` when input is pressed.
- `APlayerController` (optional custom)
  - Applies **Input Mapping Contexts** (IMC_Default, IMC_Weapons).

### Portal System
- `APortal`
  - Components: `UStaticMeshComponent* Mesh`, `USceneCaptureComponent2D* Capture`, `UBoxComponent* BoxComp`.
  - `BeginPlay()` – initialize pair references, material instance, and render target.
  - `Tick()` / `UpdatePortals()` – keep capture aligned and rendering.
  - `OnOverlapBegin()` – teleport the actor and remap orientation.
- **Materials/RTs:** two `UTextureRenderTarget2D` assets (Rt_1, Rt_2) + two materials (Rt_1_Mat, Rt_2_Mat) fed by the captures.

### Procedural Terrain
- `APerlinProcTerrain`
  - `CreateVertices()` – generate grid vertices, height via Perlin.
  - `CreateTriangles()` – index buffer build.
  - `CreateMeshSection()` – write to `UProceduralMeshComponent`.
  - `AlterMesh()` – deform vertices around an impact at runtime and update the mesh section.

### Weapon & Projectile
- `AGun`
  - Holds a `TSubclassOf<AGameProjectile>`.
  - Spawns projectile and passes **random color** to it each fire.
- `AGameProjectile`
  - Components: `USphereComponent* Collision`, `UStaticMeshComponent* BallMesh`, `UProjectileMovementComponent* Movement`.
  - `OnHit()` – stop/bounce, **spawn splat Niagara**, **spawn decal**, set dynamic material/FX color **from C++**.
  - Lifetime & physics fully configured in code.

### Runtime Material & Niagara Parameters
- Use `UMaterialInstanceDynamic` for **per-instance color**:
  ```cpp
  DynMI->SetVectorParameterValue(TEXT("ProjColor"), RandomColor);
