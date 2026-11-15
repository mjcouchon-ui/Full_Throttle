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
	enum : uint16 { Gap = 1, Loop = 2, Twist = 4, Pipe = 8, Wall = 16, Kicker = 32, Landing = 64, Warn = 128, Inverted = 256, Arena = 512,
		HalfPipe = 1024, Coaster = 2048, Building = 4096 };
	// the set pieces the car is held to and keeps its speed through (2.5)
	constexpr uint16 SetPiece = Loop | Twist | Pipe;
}

/** 2.5: the coaster rails at one point: centre x and height over the road's frame (cm). 3.3: three rails (left,
 *  centre, right); you can hop between two that sit side by side at the same height. */
struct FFTRails
{
	static constexpr int32 Num = 3;
	static constexpr float HalfW = 300.f; // half the width of one rail's deck
	static constexpr float Sep = 760.f;   // rail centres side by side
	float X[Num] = { -Sep, 0.f, Sep };
	float H[Num] = { 0.f, 0.f, 0.f };
	FVector2D P(int32 i) const { return FVector2D(X[i], H[i]); }
	/** The rail nearest a point (x, h) in the cross-section. */
	int32 Nearest(const FVector2D& Pt) const
	{
		int32 B = 0;
		for (int32 i = 1; i < Num; ++i) if (FVector2D::DistSquared(Pt, P(i)) < FVector2D::DistSquared(Pt, P(B))) B = i;
		return B;
	}
	bool Adjacent(int32 A, int32 B) const { return A != B && FMath::Abs(X[A] - X[B]) < Sep + 200.f && FMath::Abs(H[A] - H[B]) < 120.f; }
	/** The next rail to the left (Dir -1) or right (+1) of rail From that you can hop to, or -1. */
	int32 Neighbour(int32 From, int32 Dir) const
	{
		int32 B = -1;
		for (int32 j = 0; j < Num; ++j)
		{
			if (j == From || (X[j] - X[From]) * Dir <= 0.f) continue;
			if (B < 0 || FMath::Abs(X[j] - X[From]) < FMath::Abs(X[B] - X[From])) B = j;
		}
		return (B >= 0 && Adjacent(From, B)) ? B : -1;
	}
	bool AnyAdjacent() const { return Adjacent(0, 1) || Adjacent(1, 2) || Adjacent(0, 2); }
};

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

/** What a roadside advert sells; it decides which story beats and districts the planner gives it. */
UENUM()
enum class EFTAdKind : uint8 { Lawyer, Casino, Food, Motel };

/** A design's part in the short stunt sequences: Any, the gag after a landing, or the lawsuit that closes one. */
UENUM()
enum class EFTAdBeat : uint8 { Any, CrashGag, LawsuitThreat };

/** Bayshore's advertising districts, found from the landmark props (the Ferris wheel = pier, the lighthouse = marina). */
namespace FTAdWhere
{
	enum : uint8 { Strip = 1, Pier = 2, Marina = 4 };
}

/** One printed billboard design (a mesh variant with its own face) and where it may appear. */
USTRUCT()
struct FFTAdRule
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere) FString Mesh;                        // asset path of the billboard variant
	UPROPERTY(EditAnywhere) EFTAdKind Kind = EFTAdKind::Lawyer;
	UPROPERTY(EditAnywhere) uint8 Where = FTAdWhere::Strip;      // FTAdWhere bits it may fill (0 = story beats only)
	UPROPERTY(EditAnywhere) bool bNear = false;                  // detailed art: the near band (~44 m out), else ~70 m
	UPROPERTY(EditAnywhere) EFTAdBeat Beat = EFTAdBeat::Any;     // CrashGag: only on a calm run-out after a landing
	UPROPERTY(EditAnywhere) uint8 Landmark = 0;                  // FTAdWhere district whose centre slot it owns, at 1.4x
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

	/** Printed billboards placed by story beat and district (empty = the level's defaults). Off with ft.Ads 0 / -ftads=0. */
	UPROPERTY(EditAnywhere, Category = "Track|Props") TArray<FFTAdRule> Ads;
	UPROPERTY(EditAnywhere, Category = "Track|Props") bool bAds = true;

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
	/** 2.5: height of the driving surface over the road's frame at (d, x), and its slope across (dH/dx).
	 *  Flat road: 0. Half pipe: the U's curve. Coaster: the nearest rail's deck; bSolid = false over thin air. */
	float SurfaceH(float D, float X, float* OutSlope = nullptr, bool* bSolid = nullptr) const;
	/** 2.5: the coaster rails at d (only meaningful where FlagsAt(d) has Coaster). */
	FFTRails RailsAt(float D) const;
	/** 3.3: where something flying sits at (d, x, h): level with the world, over the land's straight line through
	 *  loops, corkscrews and coaster dips (choppers and the flying boss don't loop the loop with the road). */
	FTransform AirTransformAt(float D, float X, float H) const;
	/** 3.3: how high a flyer at d must climb to clear the road there (a loop's top, the coaster's high rails), cm. */
	float AirClearAt(float D) const;
	/** The section a distance falls in, and that section's shape data. */
	const FFTSection* SectionAt(float D, float* OutU = nullptr) const;

	static constexpr float Step = 200.f; // cm between samples

	// ---- tools: the same queries for editor scripts (capture cameras that stand where the chase camera does,
	// scenery checks). Nothing in the game calls these.
	UFUNCTION(BlueprintCallable, Category = "Track|Tools") float ToolLength() const { return Length(); }
	/** The road's frame at distance D (cm): location = road centre, X = forward, Z = the road's own up (banked). */
	UFUNCTION(BlueprintCallable, Category = "Track|Tools") FTransform ToolFrame(float D) const;
	/** The land beside the road at D (ignores loops, pipes and twists). */
	UFUNCTION(BlueprintCallable, Category = "Track|Tools") FVector ToolGround(float D) const { return Sample(D).Ground; }
	UFUNCTION(BlueprintCallable, Category = "Track|Tools") int32 ToolFlags(float D) const { return FlagsAt(D); }
	UFUNCTION(BlueprintCallable, Category = "Track|Tools") float ToolBank(float D) const { return Sample(D).Bank; }
	/** 3.1: the height of the floor under the whole level (Bayshore: its sea level), in the track's own space. */
	UFUNCTION(BlueprintCallable, Category = "Track|Tools") float ToolFloorZ() const { return FloorZ; }
	/** 3.6 test probe (game run with -ftbedprobe): how far a point stands above the bed (the land under and beside the
	 *  road) at road distance D, cm; negative = under the ground. False when the probe is off or the point is past the bed. */
	bool BedClearance(const FVector& WorldP, float D, float& OutAbove) const;

	void Rebuild();
	bool IsBuilt() const { return Frames.Num() > 1; }
	/** 3.0 attract mode: the track was built light around this distance (cm): collision only near it, small
	 *  scenery only within a few km of it (the land, road and big scenery are everywhere). -1 = a full build. */
	float LightD0() const { return LightAt; }
	bool IsLight() const { return LightAt >= 0.f; }
	static constexpr float LightCollBack = 60000.f;
	static constexpr float LightCollAhead = 300000.f;
	static constexpr float LightPropBack = 100000.f;
	static constexpr float LightPropAhead = 700000.f;

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

protected:
	void Integrate();
	void BuildMeshes();
	void BuildProps();

	/** One planned billboard: its transform, the rule it shows, and its footprint for the keep-out test. */
	struct FAdSpot { FTransform Xf; FVector2D Centre; float Radius = 0.f; int32 Rule = 0; int32 Slot = 0; };
	void PlanAds(const TArray<FFTPropRule>& Rules, const TArray<FFTAdRule>& AdRules, float SlotLen, TArray<FAdSpot>& Out) const;
	/** The level's ground bands (x cm, z cm) and colours, and the sea's edge (0 = no sea). */
	void GroundBands(TArray<FVector2D>& GP, TArray<FLinearColor>& GC, float& WaterX) const;
	/** Height of the land at x cm across from the road (relative to the frame's Ground). */
	float GroundZ(float Xcm) const;

	// 2.7: scenery stays off the road. The road's frames on a 2D grid, so any point can ask "is some part of the
	// road here?", including parts that come back near this one (tight curves, the circuit lapping round).
	void BuildRoadGrid();
	/** How wide/high the road is at frame J: horizontal reach from its centre, the lowest and highest thing on it
	 *  (deck underside .. car room), and the driving floor (what ground must stay under). */
	void RoadSpan(int32 J, float& OutW, float& OutLo, float& OutHi, float& OutFloor) const;
	/** First road frame (outside [SkipLo, SkipHi]) that a yaw-rotated box overlaps, grown by Margin, within the
	 *  height span [ZLo, ZHi]; INDEX_NONE if clear. OutDepth = how far inside it reaches (cm). */
	int32 RoadHitBox(const FVector& Centre, const FQuat& Q, const FVector& HalfExt, float Margin, float ZLo, float ZHi,
		int32 SkipLo, int32 SkipHi, float* OutDepth = nullptr) const;
	/** Does a point of land at P (its top surface) stick up into the road anywhere outside [SkipLo, SkipHi]? */
	bool GroundHitsRoad(const FVector& P, int32 SkipLo, int32 SkipHi) const;
	TMap<FIntPoint, TArray<int32>> RoadCells;
	float MaxRoadW = 0.f;
	float LightAt = -1.f;
	double MeshCreateMs = 0.0, CookMs = 0.0, RegisterMs = 0.0;
	float FloorZ = 0.f; // 3.1: the floor under the whole level, below its lowest land
	/** 3.4: the bed's cuttings, kept for the dressing's Bank rule (FTDress.h). For each frame, where the bed's last
	 *  straight run up to the land's inner edge starts: on the left (X across, Y up) and on the right (Z across, W up),
	 *  in the land's own cross-section (cm from the frame's Ground along GRight, cm above it); and those two inner
	 *  edges. Written by BuildMeshes from the very line the bed is drawn with; nothing of the track reads it. */
	TArray<FVector4f> BedCut;
	FVector2D BedInL = FVector2D::ZeroVector, BedInR = FVector2D::ZeroVector;
	TArray<TArray<FVector2D>> BedProbe; // 3.6: each frame's bed line (x, z over the base path), only with -ftbedprobe

	TArray<FFTFrame> Frames;
	TArray<float> SectionStart; // cm
	TArray<FFTSection> Sections;
	TArray<FVector> LampSpots;

	UPROPERTY(Transient) TArray<TObjectPtr<UProceduralMeshComponent>> Chunks;
	UPROPERTY(Transient) TArray<TObjectPtr<UHierarchicalInstancedStaticMeshComponent>> PropComps;
	UPROPERTY(Transient) TObjectPtr<UHierarchicalInstancedStaticMeshComponent> Pillars;
	/** 3.1: single scenery pieces that need their own component (the volcano's eruptions). */
	UPROPERTY(Transient) TArray<TObjectPtr<class UStaticMeshComponent>> LifeComps;
};
