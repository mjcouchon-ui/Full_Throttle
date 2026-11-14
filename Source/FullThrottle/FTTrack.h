// The road: integrates a level's 1.x section list into a 3D ribbon (banks, hills, loops, corkscrews,
// pipes, jump gaps), builds its meshes and collision, and answers track-space queries.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FTCore.h"
#include "FTTrack.generated.h"

class UProceduralMeshComponent;
class UInstancedStaticMeshComponent;
class UHierarchicalInstancedStaticMeshComponent;
class UMaterialInterface;
class UStaticMesh;

namespace FTFlag
{
	enum : uint16 { Gap = 1, Loop = 2, Twist = 4, Pipe = 8, Wall = 16, Kicker = 32, Landing = 64, Warn = 128, Inverted = 256, Arena = 512 };
}

struct FFTFrame
{
	FVector Pos = FVector::ZeroVector;   // road centre
	FVector Fwd = FVector::ForwardVector, Right = FVector::RightVector, Up = FVector::UpVector;
	FVector Ground = FVector::ZeroVector; // the land beside the road (ignores loops, pipes and twists)
	FVector GRight = FVector::RightVector;
	float Curve = 0.f;    // eased 1.x curve value
	float Bank = 0.f;     // radians
	float U = 0.f;        // 0..1 inside its section
	int32 Section = 0;
	uint16 Flags = 0;
};

/** One roadside prop type: a mesh scattered along the road by 1.x-style slot rules. */
USTRUCT()
struct FFTPropRule
{
	GENERATED_BODY()
	UPROPERTY() FString Mesh;          // asset path
	UPROPERTY() float XMin = 0.f;      // 1.x across units (9.14 cm), negative = left
	UPROPERTY() float XMax = 0.f;
	UPROPERTY() int32 Every = 1;       // every n-th slot
	UPROPERTY() int32 Phase = 0;
	UPROPERTY() float Chance = 1.f;
	UPROPERTY() bool bBothSides = false;
	UPROPERTY() bool bOnRoad = false;  // sits at road height on the barrier (lamps, poles)
	UPROPERTY() bool bFaceRoad = true;
	UPROPERTY() float Scale = 1.f;
	UPROPERTY() float ScaleJitter = 0.f;
	UPROPERTY() int32 Light = 0;       // 1 = this prop carries a road light
};

UCLASS()
class FULLTHROTTLE_API AFTTrack : public AActor
{
	GENERATED_BODY()
public:
	AFTTrack();

	/** Which level's road this is (0 Bayshore, 1 Chemical Works, 2 Nevada). */
	UPROPERTY(EditAnywhere, Category = "Track")
	int32 LevelIndex = 0;

	/** Build the meshes in the editor too (slower to edit; the game always builds them). */
	UPROPERTY(EditAnywhere, Category = "Track")
	bool bBuildInEditor = true;

	UPROPERTY(EditAnywhere, Category = "Track|Look") TObjectPtr<UMaterialInterface> RoadMaterial;
	UPROPERTY(EditAnywhere, Category = "Track|Look") TObjectPtr<UMaterialInterface> WallMaterial;
	UPROPERTY(EditAnywhere, Category = "Track|Look") TObjectPtr<UMaterialInterface> NeonMaterial;
	UPROPERTY(EditAnywhere, Category = "Track|Look") TObjectPtr<UMaterialInterface> GlassMaterial;
	UPROPERTY(EditAnywhere, Category = "Track|Look") TObjectPtr<UMaterialInterface> GroundMaterial;
	UPROPERTY(EditAnywhere, Category = "Track|Look") TObjectPtr<UMaterialInterface> RibMaterial;
	UPROPERTY(EditAnywhere, Category = "Track|Look") TObjectPtr<UStaticMesh> PillarMesh;

	/** Ground bands either side of the road: x (1.x units) and colour, sorted left to right. */
	UPROPERTY(EditAnywhere, Category = "Track|Look") TArray<FVector2D> GroundProfile;     // (x units, height cm)
	UPROPERTY(EditAnywhere, Category = "Track|Look") TArray<FLinearColor> GroundColours;

	UPROPERTY(EditAnywhere, Category = "Track|Props") TArray<FFTPropRule> Props;

	// ---- queries (track space -> world)
	int32 Num() const { return Frames.Num(); }
	float Length() const { return (Frames.Num() - 1) * Step; }
	const FFTFrame& FrameAt(int32 I) const { return Frames[FMath::Clamp(I, 0, Frames.Num() - 1)]; }
	FFTFrame Sample(float D) const;
	FVector ToWorld(float D, float X, float H) const;
	FTransform TransformAt(float D, float X, float H, float YawOffsetDeg = 0.f) const;
	/** Nearest road distance to a world point, searching around HintD first. Also returns x and h. */
	float Locate(const FVector& P, float HintD, float& OutX, float& OutH) const;
	uint16 FlagsAt(float D) const { return Frames.Num() ? FrameAt(FMath::RoundToInt(D / Step)).Flags : 0; }
	float StartDOfSection(int32 Section) const;
	/** Distance where the next set piece of this kind starts, or -1. */
	float NextFlagStart(float FromD, uint16 Flag, float Within) const;
	const TArray<FVector>& LightSpots() const { return LampSpots; }

	static constexpr float Step = 200.f; // cm between samples

	void Rebuild();
	bool IsBuilt() const { return Frames.Num() > 1; }

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

protected:
	void Integrate();
	void BuildMeshes();
	void BuildProps();

	TArray<FFTFrame> Frames;
	TArray<float> SectionStart; // cm
	TArray<FVector> LampSpots;

	UPROPERTY(Transient) TArray<TObjectPtr<UProceduralMeshComponent>> Chunks;
	UPROPERTY(Transient) TArray<TObjectPtr<UHierarchicalInstancedStaticMeshComponent>> PropComps;
	UPROPERTY(Transient) TObjectPtr<UHierarchicalInstancedStaticMeshComponent> Pillars;
};
