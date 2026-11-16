#include "FTTrack.h"
#include "HAL/IConsoleManager.h"
#include "ProceduralMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "HAL/IConsoleManager.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "FTGameInstance.h"
#include "FTLife.h"
#include "FTDress.h"
#include "Misc/Crc.h"
#include "HAL/PlatformTime.h"

static TAutoConsoleVariable<int32> CVarFTAds(TEXT("ft.Ads"), 1, TEXT("FULL THROTTLE: printed roadside billboards, 1 = on, 0 = off (read when the track builds; for before/after benches, or -ftads=0)"));

namespace
{
	inline float Smooth(float t) { t = FMath::Clamp(t, 0.f, 1.f); return t * t * (3.f - 2.f * t); }
	inline float Smoother(float t) { t = FMath::Clamp(t, 0.f, 1.f); return t * t * t * (t * (t * 6.f - 15.f) + 10.f); }
	constexpr float PipeAxis = 80.f * FT::XU;  // tube axis above the road
	constexpr float PipeR = 175.f * FT::XU;    // glass tube radius
	constexpr float DeckDepth = 80.f;
	constexpr float WallW = 40.f;
	// 2.5 half pipe: a flat floor, then a quarter circle up to 68 degrees at the barrier
	constexpr float HPFlat = 420.f, HPR = 1000.f, HPMaxSin = 0.927f;
	// 2.5 coaster: rail centres side by side, the low guard rails on each deck
	constexpr float RailX0 = 560.f, GuardH = 45.f, GuardW = 22.f;
	// 2.5 buildings over the road
	constexpr float BldgCeil = 1050.f, BldgOut = 1700.f, BldgBelow = 1400.f;

	template <typename T> T* LoadAsset(const TCHAR* Path) { return Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, Path)); }

	/** Mesh buffers for one procedural section. */
	struct FBuf
	{
		TArray<FVector> V; TArray<int32> I; TArray<FVector> N; TArray<FVector2D> UV; TArray<FLinearColor> C;
		int32 Add(const FVector& P, const FVector& Nrm, const FVector2D& Uv, const FLinearColor& Col = FLinearColor::White)
		{ V.Add(P); N.Add(Nrm); UV.Add(Uv); C.Add(Col); return V.Num() - 1; }
		/** Quad a0-a1 (this ring) / b0-b1 (next ring); winding chosen so it faces Nrm. */
		void Quad(int32 a0, int32 a1, int32 b0, int32 b1, const FVector& Nrm)
		{
			const FVector X = FVector::CrossProduct(V[b0] - V[a0], V[a1] - V[a0]);
			// Unreal's front faces wind the other way round: (v1-v0)x(v2-v0) points *away* from the viewer
			if (FVector::DotProduct(X, Nrm) < 0.f) { I.Append({ a0, b0, a1, a1, b0, b1 }); }
			else { I.Append({ a0, a1, b0, a1, b1, b0 }); }
		}
		/** One triangle, wound to face Nrm. */
		void Tri(int32 a, int32 b, int32 c, const FVector& Nrm)
		{
			const FVector X = FVector::CrossProduct(V[b] - V[a], V[c] - V[a]);
			if (FVector::DotProduct(X, Nrm) < 0.f) { I.Append({ a, b, c }); }
			else { I.Append({ a, c, b }); }
		}
	};

	// 3.1: the bed (the land under and right beside the road) and the floor under the whole level
	constexpr float BedClear = 45.f;    // the bed stays this far under every part of the road
	constexpr float BedSlope = 1.6f;    // how steeply a cut's sides may climb back to the land (rise per run)
	constexpr float SkirtOut = 6000.f, SkirtDrop = 6000.f; // the land's outer edge runs on, down to the floor
	constexpr float FloorHalf = 8000000.f; // the floor reaches 80 km past the track: further than any sky dome
	// 3.6: the room over the deck the car and the chase camera take (camera 290 cm over a car riding ~90 cm up, plus
	// its near plane), and how many frames either side a corkscrew's bed looks at (the camera rides ~8-14 m behind
	// the car, where the road has rolled a different amount)
	constexpr float BedRoom = 450.f;
	constexpr int32 TwistBedReach = 7;

	/** One frame's road in the land's own cross-section (x across, z above the land's base path), already lowered by BedClear. */
	struct FBedSec
	{
		TArray<FVector2D> P;
		TArray<int32> Start; // chains: P[Start[c]] up to the next chain's start
		void Begin() { Start.Add(P.Num()); }
		float Lowest() const
		{
			float M = TNumericLimits<float>::Max();
			for (const FVector2D& Q : P) M = FMath::Min(M, (float)Q.Y);
			return M;
		}
		/** The highest the land may stand at x: under every part of the road over x, and under a slope from each of its corners. */
		float Limit(float X, float Slope) const
		{
			float M = TNumericLimits<float>::Max();
			for (int32 c = 0; c < Start.Num(); ++c)
			{
				const int32 a = Start[c], b = c + 1 < Start.Num() ? Start[c + 1] : P.Num();
				for (int32 k = a; k < b; ++k)
				{
					M = FMath::Min(M, (float)P[k].Y + Slope * FMath::Abs(X - (float)P[k].X));
					if (k + 1 < b)
					{
						const float X0 = P[k].X, X1 = P[k + 1].X;
						if ((X - X0) * (X - X1) <= 0.f && FMath::Abs(X1 - X0) > 0.01f)
							M = FMath::Min(M, (float)FMath::Lerp(P[k].Y, P[k + 1].Y, (X - X0) / (X1 - X0)));
					}
				}
			}
			return M;
		}
	};
}

AFTTrack::AFTTrack()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent->SetMobility(EComponentMobility::Static);
}

void AFTTrack::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (bBuildInEditor) Rebuild();
}

void AFTTrack::BeginPlay()
{
	Super::BeginPlay();
	if (!IsBuilt() || Chunks.Num() == 0) Rebuild();
	// 3.1: the air round the camera (dust, snow, embers, bubbles...), one per race
	if (UWorld* W = GetWorld(); W && W->IsGameWorld())
		if (AFTAmbient* Amb = W->SpawnActor<AFTAmbient>()) Amb->Setup(this, LevelIndex);
}

void AFTTrack::Rebuild()
{
	// 3.0: an attract-mode cut asks for a light build around its shot (the map load is what the cut waits for)
	LightAt = -1.f;
	if (UWorld* W = GetWorld(); W && W->IsGameWorld())
		if (const UFTGameInstance* G = Cast<UFTGameInstance>(W->GetGameInstance())) LightAt = G->AttractD0;
	const double T0 = FPlatformTime::Seconds();
	Integrate();
	const double T1 = FPlatformTime::Seconds();
	BuildRoadGrid();
	const double T2 = FPlatformTime::Seconds();
	MeshCreateMs = 0.0; CookMs = 0.0; RegisterMs = 0.0;
	BuildMeshes();
	const double T3 = FPlatformTime::Seconds();
	BuildProps();
	const double T4 = FPlatformTime::Seconds();
	UE_LOG(LogFT, Display, TEXT("FTBUILD level=%d light=%.0f integrate=%.0fms grid=%.0fms meshes=%.0fms (sections+collision %.0fms: cook %.0fms register %.0fms) props=%.0fms total=%.0fms"),
		LevelIndex, LightAt, (T1 - T0) * 1000.0, (T2 - T1) * 1000.0, (T3 - T2) * 1000.0, MeshCreateMs, CookMs, RegisterMs, (T4 - T3) * 1000.0, (T4 - T0) * 1000.0);
}

// ------------------------------------------------------------------------------------------ 2.7 road clearance
static TAutoConsoleVariable<int32> CVarFTClear(TEXT("ft.ClearScenery"), 1,
	TEXT("2.7: keep props, pillars, backdrops and the land off the road (0 = place them as 2.6 did; for before/after checks). Takes effect when a track is rebuilt."));
static TAutoConsoleVariable<int32> CVarFTDressShadows(TEXT("ft.DressShadows"), 1,
	TEXT("FULL THROTTLE 3.3: 1 = a dressed piece casts a shadow only within 70 m of the road and over 2.5 m tall; 0 = every piece but the horizon's casts, as before (for comparison; set it before the level is built)"));
static TAutoConsoleVariable<int32> CVarFTLand(TEXT("ft.Land"), 1,
	TEXT("3.4: 1 = the land and the bed wear the level's own surface (/Game/Maps/<Level>/Materials/MI_Land: slabs, ripples or cracked ground, ft_world.land_materials); 0 = the plain ground, for before/after checks. Takes effect when a track is rebuilt."));
static TAutoConsoleVariable<int32> CVarFTDress(TEXT("ft.Dress"), 1,
	TEXT("3.2: 0 = build tracks without the dressing (districts, sets round landmarks, the horizon), for before/after checks. Takes effect when a track is rebuilt."));
static TAutoConsoleVariable<int32> CVarFTPropWPODist(TEXT("ft.PropWPODist"), 18000,
	TEXT("4.1: cm past which a roadside prop stops moving by its material (people, creatures, fronds, wheels: world position offset). The population pass was written for 180 m (M_People; M_PeopleFar always moves) but nothing set it, so every figure on the level ran its vertex animation every frame. 0 = never stop (4.0). Takes effect when a track is rebuilt."));
static TAutoConsoleVariable<int32> CVarFTPropWPOVel(TEXT("ft.PropWPOVel"), 0,
	TEXT("4.1: 0 = the people and roadside life (M_People, M_Life) draw no motion vectors of their own (velocity pass L1 0.43 -> 0.23 ms, L2 0.38 -> 0.11 ms; a waving arm is small next to the camera's own motion at speed); 1 = they do, as 4.0. Takes effect when a track is rebuilt."));
static TAutoConsoleVariable<int32> CVarFTDressBank(TEXT("ft.DressBank"), 1,
	TEXT("3.4: 0 = the dressing leaves the cut banks inside banked bends bare (no Bank panels), for before/after checks. Takes effect when a track is rebuilt."));
static TAutoConsoleVariable<int32> CVarFTBedRoom(TEXT("ft.BedRoom"), 1,
	TEXT("3.6: 1 = the land under a corkscrew (and anywhere the road leans past 60 degrees) is cut under the room the car and the chase camera take, so upside down they no longer drive through the ground; 0 = cut only under the deck, as 3.5 did (before/after checks). Takes effect when a track is rebuilt."));
namespace { constexpr float RoadCell = 3000.f; inline FIntPoint CellOf(float X, float Y) { return FIntPoint(FMath::FloorToInt(X / RoadCell), FMath::FloorToInt(Y / RoadCell)); } }

void AFTTrack::BuildRoadGrid()
{
	RoadCells.Reset();
	MaxRoadW = 0.f;
	for (int32 i = 0; i < Frames.Num(); ++i)
	{
		RoadCells.FindOrAdd(CellOf(Frames[i].Pos.X, Frames[i].Pos.Y)).Add(i);
		float W, Lo, Hi, Fl;
		RoadSpan(i, W, Lo, Hi, Fl);
		MaxRoadW = FMath::Max(MaxRoadW, W);
	}
}

void AFTTrack::RoadSpan(int32 J, float& W, float& Lo, float& Hi, float& Floor) const
{
	const FFTFrame& F = Frames[J];
	W = FT::Edge + WallW; Floor = F.Pos.Z; Lo = F.Pos.Z - DeckDepth - 60.f; Hi = F.Pos.Z + 900.f;
	// a loop's frames climb (the land is under its bottom ones); a corkscrew rolls the deck's edges up and down
	if (F.Flags & (FTFlag::Loop | FTFlag::Twist)) { Lo = F.Pos.Z - FT::Edge - 200.f; Hi = F.Pos.Z + FT::Edge + 900.f; }
	if (F.Flags & FTFlag::Twist) Floor = F.Pos.Z - 300.f;
	if (F.Flags & FTFlag::Pipe)
	{
		W = PipeR + 150.f;
		const float Az = F.Ground.Z + PipeAxis;
		Lo = FMath::Min(Lo, Az - PipeR - 60.f); Hi = FMath::Max(Hi, Az + PipeR + 150.f); Floor = Lo;
	}
	if (F.Flags & FTFlag::Coaster)
	{
		W = FFTRails::Sep + 2600.f + FFTRails::HalfW + 150.f; Lo = F.Pos.Z - 1300.f - DeckDepth - 60.f; Hi = F.Pos.Z + 2300.f + 900.f; Floor = F.Pos.Z - 1300.f;
		// 3.3: its loop and corkscrews take the road's room for those
		if (F.Flags & (FTFlag::Loop | FTFlag::Twist)) { Lo = FMath::Min(Lo, F.Pos.Z - W - 200.f); Hi = FMath::Max(Hi, F.Pos.Z + W + 900.f); }
	}
	if (F.Flags & FTFlag::HalfPipe) Hi += HPR;
	if (F.Flags & FTFlag::Building) { W = FT::Edge + BldgOut; Hi = F.Pos.Z + BldgCeil + 2500.f; }
}

int32 AFTTrack::RoadHitBox(const FVector& C, const FQuat& Q, const FVector& E, float Margin, float ZLo, float ZHi, int32 SkipLo, int32 SkipHi, float* OutDepth) const
{
	// the box's footprint on the grid, grown by the widest road (a coaster's split rails) and the margin
	const float Reach = FMath::Sqrt(E.X * E.X + E.Y * E.Y) + MaxRoadW + Margin;
	const FIntPoint A = CellOf(C.X - Reach, C.Y - Reach), B = CellOf(C.X + Reach, C.Y + Reach);
	int32 Best = INDEX_NONE; float BestDepth = 0.f;
	for (int32 cx = A.X; cx <= B.X; ++cx)
		for (int32 cy = A.Y; cy <= B.Y; ++cy)
		{
			const TArray<int32>* L = RoadCells.Find(FIntPoint(cx, cy));
			if (!L) continue;
			for (const int32 j : *L)
			{
				if (j >= SkipLo && j <= SkipHi) continue;
				float W, Lo, Hi, Fl;
				RoadSpan(j, W, Lo, Hi, Fl);
				if (ZHi < Lo || ZLo > Hi) continue;
				// the road centre in the box's own frame: distance to the box across the ground
				const FVector Lc = Q.UnrotateVector(Frames[j].Pos - C);
				const float Dx = FMath::Max(0.f, FMath::Abs(Lc.X) - E.X), Dy = FMath::Max(0.f, FMath::Abs(Lc.Y) - E.Y);
				const float Dist = FMath::Sqrt(Dx * Dx + Dy * Dy);
				const float Depth = W + Margin - Dist;
				if (Depth > BestDepth) { BestDepth = Depth; Best = j; }
			}
		}
	if (OutDepth) *OutDepth = BestDepth;
	return Best;
}

bool AFTTrack::GroundHitsRoad(const FVector& P, int32 SkipLo, int32 SkipHi) const
{
	const float Reach = MaxRoadW + 100.f;
	const FIntPoint A = CellOf(P.X - Reach, P.Y - Reach), B = CellOf(P.X + Reach, P.Y + Reach);
	for (int32 cx = A.X; cx <= B.X; ++cx)
		for (int32 cy = A.Y; cy <= B.Y; ++cy)
		{
			const TArray<int32>* L = RoadCells.Find(FIntPoint(cx, cy));
			if (!L) continue;
			for (const int32 j : *L)
			{
				if (j >= SkipLo && j <= SkipHi) continue;
				float W, Lo, Hi, Fl;
				RoadSpan(j, W, Lo, Hi, Fl);
				if (P.Z <= Fl - 30.f) continue; // land under the road is fine; land level with it or over it is not
				if (FVector::DistSquared2D(P, Frames[j].Pos) < FMath::Square(W + 100.f)) return true;
			}
		}
	return false;
}

// ------------------------------------------------------------------------------------------ integrate
void AFTTrack::Integrate()
{
	const FFTLevelDef& L = FTData::Level(LevelIndex);
	TArray<FFTSection> All = L.Opening;
	const int32 ArenaFirst = All.Num();
	All.Append(L.Arena);
	const int32 ArenaLast = All.Num();
	float Total = 0.f;
	for (const FFTSection& S : All) Total += S.Len;
	const float Target = FMath::Max(180000.f, Total + 42000.f);
	while (Total < Target && L.Circuit.Num())
	{
		for (const FFTSection& S : L.Circuit) { All.Add(S); Total += S.Len; }
	}

	SectionStart.Reset();
	{
		float D = 0.f;
		for (const FFTSection& S : All) { SectionStart.Add(D * FT::DU); D += S.Len; }
	}
	Sections = All;

	const float StepD = Step / FT::DU;
	const int32 N = FMath::FloorToInt(Total / StepD) + 1;
	Frames.SetNum(N);

	FVector B = FVector::ZeroVector; // base path (before pipe/twist roll)
	float Yaw = 0.f;
	int32 Sec = 0;
	float SecStartD = 0.f;
	float PrevCurve = 0.f, PrevBank = 0.f;
	float SlidePrev = 0.f;

	for (int32 i = 0; i < N; ++i)
	{
		const float D = i * StepD;
		while (Sec < All.Num() - 1 && D >= SecStartD + All[Sec].Len)
		{
			PrevCurve = All[Sec].Curve; PrevBank = All[Sec].Bank;
			SecStartD += All[Sec].Len; ++Sec; SlidePrev = 0.f;
		}
		const FFTSection& S = All[Sec];
		const float s = D - SecStartD;
		const float u = FMath::Clamp(s / S.Len, 0.f, 1.f);
		const float E = FMath::Min(450.f, 0.45f * S.Len);
		const float k = Smooth(s / FMath::Max(E, 1.f));
		const float Curve = FMath::Lerp(PrevCurve, S.Curve, k);
		const float Bank = FMath::Lerp(PrevBank, S.Bank, k);

		float Pitch = 0.f, Roll = Bank, Axis = 0.f, Slide = 0.f;
		uint16 Flags = 0;
		switch (S.Shape)
		{
		case EFTSec::Hill: Pitch = S.A * FMath::Sin(2.f * PI * S.B * u); break;
		case EFTSec::Rise: Pitch = S.A * (1.f - FMath::Cos(2.f * PI * u)) * 0.5f; break;
		case EFTSec::Fall: Pitch = -S.A * (1.f - FMath::Cos(2.f * PI * u)) * 0.5f; break;
		case EFTSec::Loop:
			Pitch = 2.f * PI * Smoother(u);
			Slide = 360.f * FT::XU * Smooth((u - 0.2f) / 0.6f);
			Flags |= FTFlag::Loop;
			break;
		case EFTSec::Twist: Roll += S.A * 2.f * PI * Smoother(u); Flags |= FTFlag::Twist; break;
		case EFTSec::Pipe: Roll += S.A * 2.f * PI * S.B * Smoother(u); Axis = PipeAxis; Flags |= FTFlag::Pipe; break;
		case EFTSec::Jump:
		{
			const float Ak = S.A * 0.75f, Bk = S.B * 0.75f;
			if (u < 0.22f) { Pitch = Ak * Smooth(u / 0.22f); Flags |= FTFlag::Kicker; }
			else if (u < 0.6f) { Pitch = FMath::Lerp(Ak, -Bk, (u - 0.22f) / 0.38f); Flags |= FTFlag::Gap; }
			else { Pitch = -Bk * (1.f - Smooth((u - 0.6f) / 0.4f)); if (u < 0.68f) Flags |= FTFlag::Landing; }
			break;
		}
		case EFTSec::HalfPipe: Flags |= FTFlag::HalfPipe; break;
		case EFTSec::Coaster:
		{
			// coaster hills (B), and mode 2 rolls the whole frame once round: the two rails corkscrew round each other
			Pitch = S.B * FMath::Sin(4.f * PI * u);
			const int32 Mode = FMath::RoundToInt(FMath::Abs(S.A));
			if (Mode == 2) { Roll += FMath::Sign(S.A) * 2.f * PI * Smoother(u); Flags |= FTFlag::Twist; }
			// 3.3: two corkscrews in one, and a full vertical loop with all three rails (slid sideways like the road's)
			if (Mode == 6) { Roll += (S.A < 0.f ? -1.f : 1.f) * 4.f * PI * Smoother(u); Flags |= FTFlag::Twist; }
			if (Mode == 4)
			{
				Pitch = 2.f * PI * Smoother(u);
				Slide = 400.f * FT::XU * Smooth((u - 0.2f) / 0.6f);
				Flags |= FTFlag::Loop;
			}
			Flags |= FTFlag::Coaster;
			break;
		}
		case EFTSec::Building: Flags |= FTFlag::Building; break;
		default: break;
		}
		Pitch += L.Slope; // the ski level runs downhill all the way
		if (FMath::Abs(S.Bank) >= 0.8f && s >= 270.f) Flags |= FTFlag::Wall;
		if (Sec >= ArenaFirst && Sec < ArenaLast) Flags |= FTFlag::Arena;
		if (L.Arena.Num() == 0 && D >= L.FinaleD) Flags |= FTFlag::Arena;

		const float CP = FMath::Cos(Pitch), SP = FMath::Sin(Pitch), CY = FMath::Cos(Yaw), SY = FMath::Sin(Yaw);
		const FVector Fwd(CP * CY, CP * SY, SP);
		const FVector Right0(-SY, CY, 0.f);
		const FVector Up0 = FVector::CrossProduct(Fwd, Right0);
		const FVector Up = Up0 * FMath::Cos(Roll) + Right0 * FMath::Sin(Roll);
		const FVector Right = Right0 * FMath::Cos(Roll) - Up0 * FMath::Sin(Roll);

		FFTFrame& F = Frames[i];
		F.Fwd = Fwd; F.Right = Right; F.Up = Up;
		F.Pos = B + Up0 * Axis - Up * Axis;
		F.Ground = B; F.GRight = Right0;
		F.Curve = Curve; F.Bank = Bank; F.U = u; F.Section = Sec; F.Flags = Flags;
		if (FVector::DotProduct(Up, FVector::UpVector) < -0.3f) F.Flags |= FTFlag::Inverted;

		// advance
		B += Fwd * Step + Right0 * (Slide - SlidePrev);
		SlidePrev = Slide;
		Yaw += Curve * 1e-3f / 1.5f * StepD;
	}

	// The land beside the road: straight through loops, pipes and corkscrews, dipping under jump gaps.
	int32 i0 = 0;
	while (i0 < N)
	{
		const int32 Sc = Frames[i0].Section;
		int32 i1 = i0;
		while (i1 + 1 < N && Frames[i1 + 1].Section == Sc) ++i1;
		const EFTSec Shape = All[Sc].Shape;
		if (Shape == EFTSec::Loop || Shape == EFTSec::Jump)
		{
			const FVector G0 = Frames[i0].Ground, G1 = Frames[FMath::Min(i1 + 1, N - 1)].Ground;
			for (int32 j = i0; j <= i1; ++j)
			{
				const float t = float(j - i0 + 1) / float(i1 - i0 + 2);
				FVector G = FMath::Lerp(G0, G1, t);
				if (Shape == EFTSec::Jump) G.Z -= 1800.f * FMath::Sin(PI * Smooth((Frames[j].U - 0.12f) / 0.6f));
				Frames[j].Ground = G;
			}
		}
		else if (Shape == EFTSec::Coaster)
		{
			// 3.3: a coaster loop's land runs straight under it, like the road's loops
			if (FMath::RoundToInt(FMath::Abs(All[Sc].A)) == 4)
			{
				const FVector G0 = Frames[i0].Ground, G1 = Frames[FMath::Min(i1 + 1, N - 1)].Ground;
				for (int32 j = i0; j <= i1; ++j) Frames[j].Ground = FMath::Lerp(G0, G1, float(j - i0 + 1) / float(i1 - i0 + 2));
			}
			// the land falls away under the coaster (its pillars are the supports)
			for (int32 j = i0; j <= i1; ++j)
			{
				const float Uj = Frames[j].U;
				Frames[j].Ground.Z -= 2600.f * Smooth(Uj / 0.18f) * Smooth((1.f - Uj) / 0.18f);
			}
		}
		i0 = i1 + 1;
	}
	// warning stripes before set pieces
	for (int32 s = 1; s < All.Num(); ++s)
	{
		const EFTSec Sh = All[s].Shape;
		if ((Sh == EFTSec::Loop || Sh == EFTSec::Jump || Sh == EFTSec::Twist || Sh == EFTSec::Pipe || Sh == EFTSec::HalfPipe || Sh == EFTSec::Coaster) && All[s - 1].Shape != Sh)
		{
			const int32 iS = FMath::RoundToInt(SectionStart[s] / Step);
			for (int32 j = FMath::Max(0, iS - FMath::RoundToInt(260.f * FT::DU / Step)); j < iS && j < N; ++j) Frames[j].Flags |= FTFlag::Warn;
		}
	}
}

FFTFrame AFTTrack::Sample(float D) const
{
	if (Frames.Num() == 0) return FFTFrame();
	const float f = FMath::Clamp(D / Step, 0.f, float(Frames.Num() - 1));
	const int32 a = FMath::FloorToInt(f);
	const int32 b = FMath::Min(a + 1, Frames.Num() - 1);
	const float t = f - a;
	const FFTFrame& A = Frames[a];
	const FFTFrame& Bf = Frames[b];
	FFTFrame R = A;
	R.Pos = FMath::Lerp(A.Pos, Bf.Pos, t);
	R.Ground = FMath::Lerp(A.Ground, Bf.Ground, t);
	R.Fwd = FMath::Lerp(A.Fwd, Bf.Fwd, t).GetSafeNormal();
	R.Up = FMath::Lerp(A.Up, Bf.Up, t).GetSafeNormal();
	// 3.3: never a zero axis (a NaN rotation downstream): fall back on the sample's own
	if (R.Fwd.IsNearlyZero()) R.Fwd = A.Fwd;
	if (R.Up.IsNearlyZero()) R.Up = A.Up;
	R.Right = FVector::CrossProduct(R.Up, R.Fwd).GetSafeNormal();
	if (R.Right.IsNearlyZero()) R.Right = A.Right;
	R.GRight = FMath::Lerp(A.GRight, Bf.GRight, t).GetSafeNormal();
	R.Curve = FMath::Lerp(A.Curve, Bf.Curve, t);
	return R;
}

FVector AFTTrack::ToWorld(float D, float X, float H) const
{
	const FFTFrame F = Sample(D);
	return GetActorTransform().TransformPosition(F.Pos + F.Right * X + F.Up * H);
}

FTransform AFTTrack::TransformAt(float D, float X, float H, float YawOffsetDeg) const
{
	const FFTFrame F = Sample(D);
	FVector Fwd = F.Fwd;
	if (YawOffsetDeg != 0.f) Fwd = Fwd.RotateAngleAxis(YawOffsetDeg, F.Up);
	const FRotator R = FRotationMatrix::MakeFromXZ(Fwd, F.Up).Rotator();
	return FTransform(R, F.Pos + F.Right * X + F.Up * H) * GetActorTransform();
}

FTransform AFTTrack::ToolFrame(float D) const
{
	return Frames.Num() > 1 ? TransformAt(D, 0.f, 0.f) : GetActorTransform();
}

static TAutoConsoleVariable<int32> CVarFTLocateNearest(TEXT("ft.LocateNearest"), 0,
	TEXT("1 = Locate reads across and height in the nearest stored frame, as it did until 15 Nov 2026 (for before/after checks)."));

float AFTTrack::Locate(const FVector& WorldP, float HintD, float& OutX, float& OutH) const
{
	OutX = OutH = 0.f;
	if (Frames.Num() < 2) return 0.f;
	const FVector P = GetActorTransform().InverseTransformPosition(WorldP);
	const int32 Hint = FMath::Clamp(FMath::RoundToInt(HintD / Step), 0, Frames.Num() - 1);
	auto Search = [&](int32 From, int32 To, int32& Best, float& BestD2)
	{
		for (int32 i = FMath::Max(0, From); i <= FMath::Min(To, Frames.Num() - 1); ++i)
		{
			const FFTFrame& F = Frames[i];
			const FVector Dv = P - F.Pos;
			// distance in the road's cross-section plane, ignoring how far along we are
			const float Along = FVector::DotProduct(Dv, F.Fwd);
			const float D2 = Dv.SizeSquared() + Along * Along * 4.f;
			if (D2 < BestD2) { BestD2 = D2; Best = i; }
		}
	};
	int32 Best = Hint; float BestD2 = TNumericLimits<float>::Max();
	Search(Hint - 60, Hint + 120, Best, BestD2);
	if (BestD2 > FMath::Square(6000.f)) Search(0, Frames.Num() - 1, Best, BestD2);
	const FFTFrame& F = Frames[Best];
	const FVector Dv = P - F.Pos;
	const float Along = FMath::Clamp(FVector::DotProduct(Dv, F.Fwd), -Step, Step);
	const float OutD = FMath::Clamp(Best * Step + Along, 0.f, Length());
	// 15 Nov 2026: across and height are read in the frame AT that distance (the one Sample, ToWorld and TransformAt place
	// things with), not in the nearest stored one. Stored frames are 2 m apart, so the point is up to 1 m of road from
	// its nearest one; where the road rolls (corkscrews, the tubes, a banked wall unwinding) that frame is turned by up
	// to 0.06 rad against the road under the point, and 11 m out from the centre line the height came out up to 70 cm
	// wrong, swinging from too high to too low every 2 m of road. The car's glue, its stick and its put-back all act
	// on that height: a car at the road's edge was pressed into the deck, or read as "under the deck" one frame after
	// being put back on it (the owner: "the car seems to clip into the road sometimes"; 915 such put-backs in the
	// logs of 15 Nov). The distance along is as before.
	if (CVarFTLocateNearest.GetValueOnAnyThread() != 0)
	{
		OutX = FVector::DotProduct(Dv, F.Right);
		OutH = FVector::DotProduct(Dv, F.Up);
		return OutD;
	}
	const FFTFrame Fi = Sample(OutD);
	const FVector Di = P - Fi.Pos;
	OutX = FVector::DotProduct(Di, Fi.Right);
	OutH = FVector::DotProduct(Di, Fi.Up);
	return OutD;
}

#if !UE_BUILD_SHIPPING
void AFTTrack::LocateCheck() const
{
	// -ftlocatecheck: a point put on the road with ToWorld must be read back by Locate where it was put. Tried half
	// way between every two stored frames, 11 m out on both sides, at the car's ride height. "nearest" is what the
	// reading in the nearest stored frame gave (Locate until 15 Nov 2026), for comparison.
	float NewX = 0.f, NewH = 0.f, OldX = 0.f, OldH = 0.f, WorstD = 0.f;
	const FTransform Inv = GetActorTransform().Inverse();
	for (int32 i = 0; i + 1 < Frames.Num(); ++i)
		for (int32 s = -1; s <= 1; s += 2)
		{
			const float D = (i + 0.5f) * Step, X = s * 1100.f, H = 94.f;
			const FVector W = ToWorld(D, X, H);
			float Lx = 0.f, Lh = 0.f;
			const float Ld = Locate(W, D, Lx, Lh);
			NewX = FMath::Max(NewX, FMath::Abs(Lx - X)); NewH = FMath::Max(NewH, FMath::Abs(Lh - H));
			const FFTFrame& Fn = FrameAt(FMath::RoundToInt(Ld / Step));
			const FVector Dn = Inv.TransformPosition(W) - Fn.Pos;
			OldX = FMath::Max(OldX, FMath::Abs(FVector::DotProduct(Dn, Fn.Right) - X));
			const float Eh = FMath::Abs(FVector::DotProduct(Dn, Fn.Up) - H);
			if (Eh > OldH) { OldH = Eh; WorstD = D; }
		}
	UE_LOG(LogFT, Display, TEXT("FTLOCATE level=%d points=%d worst across/height off, cm: now %.1f/%.1f nearest-frame %.1f/%.1f (at D=%.0f)"),
		LevelIndex, (Frames.Num() - 1) * 2, NewX, NewH, OldX, OldH, WorstD);
}
#endif

float AFTTrack::StartDOfSection(int32 Section) const
{
	return SectionStart.IsValidIndex(Section) ? SectionStart[Section] : 0.f;
}

const FFTSection* AFTTrack::SectionAt(float D, float* OutU) const
{
	if (Frames.Num() == 0 || Sections.Num() == 0) return nullptr;
	const int32 S = FrameAt(FMath::RoundToInt(D / Step)).Section;
	if (!Sections.IsValidIndex(S)) return nullptr;
	if (OutU)
	{
		const float Len = Sections[S].Len * FT::DU;
		*OutU = FMath::Clamp((D - SectionStart[S]) / FMath::Max(Len, 1.f), 0.f, 1.f);
	}
	return &Sections[S];
}

FFTRails AFTTrack::RailsAt(float D) const
{
	FFTRails R;
	float U = 0.f;
	const FFTSection* S = SectionAt(D, &U);
	if (!S || S->Shape != EFTSec::Coaster) return R;
	const int32 Mode = FMath::RoundToInt(FMath::Abs(S->A));
	const float E = FMath::Square(FMath::Sin(PI * U)); // 0 at both ends: every mode meets the next side by side
	const float Sp = FFTRails::Sep;
	// rails: 0 left, 1 centre (3.3: the third track), 2 right
	switch (Mode)
	{
	case 1: // they split: the left climbs high and wide, the right dives low and wide, the centre rolls over big hills
	{
		const float Wv = 380.f * E * FMath::Sin(6.f * PI * U);
		R.X[0] = -(Sp + 2600.f * E); R.H[0] = 1700.f * E + Wv;
		R.X[2] = Sp + 2600.f * E;    R.H[2] = -900.f * E - Wv;
		R.H[1] = 700.f * E * FMath::Sin(4.f * PI * U);
		break;
	}
	case 2: case 6: // the helix: a little apart while the frame rolls them round each other (6: twice round)
		R.X[0] = -(Sp + 520.f * E); R.X[2] = Sp + 520.f * E;
		break;
	case 3: // the cross-over: the outer two swap sides, one over the top and one underneath the centre rail
	{
		const float Sx = Smoother(U);
		R.X[0] = -Sp * FMath::Cos(PI * Sx); R.H[0] = 1900.f * FMath::Sin(PI * Sx);
		R.X[2] = Sp * FMath::Cos(PI * Sx);  R.H[2] = -1300.f * FMath::Sin(PI * Sx);
		break;
	}
	case 5: // 3.3 the third track corkscrews: the centre rail barrel-rolls twice round the outer pair's middle
	{
		const float Th = 4.f * PI * Smoother(U);
		const float Rr = 1150.f * E;
		R.X[0] = -(Sp + 900.f * E); R.X[2] = Sp + 900.f * E;
		R.X[1] = Rr * FMath::Sin(Th); R.H[1] = Rr * (1.f - FMath::Cos(Th));
		R.H[0] = R.H[2] = -250.f * E;
		break;
	}
	default: break;   // 0 side by side (with the section's hills), 4 the vertical loop (all three loop together)
	}
	return R;
}

FTransform AFTTrack::AirTransformAt(float D, float X, float H) const
{
	const FFTFrame F = Sample(D);
	const FFTFrame F2 = Sample(D + 400.f);
	FVector Fwd = F2.Ground - F.Ground; Fwd.Z *= 0.3f;
	if (!Fwd.Normalize()) Fwd = FVector(F.Fwd.X, F.Fwd.Y, 0.f).GetSafeNormal();
	if (Fwd.IsNearlyZero()) Fwd = FVector(F.GRight.Y, -F.GRight.X, 0.f).GetSafeNormal();
	if (Fwd.IsNearlyZero()) Fwd = FVector::ForwardVector;
	// the coaster's land falls away under it; a flyer keeps to the line the road came in on
	float Drop = 0.f;
	if (F.Flags & FTFlag::Coaster) Drop = 2600.f * Smooth(F.U / 0.18f) * Smooth((1.f - F.U) / 0.18f);
	// and over a jump's gap, where the land dips away under the road
	if (Sections.IsValidIndex(F.Section) && Sections[F.Section].Shape == EFTSec::Jump) Drop = 1800.f * FMath::Sin(PI * Smooth((F.U - 0.12f) / 0.6f));
	const FVector Right = FVector::CrossProduct(FVector::UpVector, Fwd).GetSafeNormal();
	return FTransform(FRotationMatrix::MakeFromXZ(Fwd, FVector::UpVector).Rotator(), F.Ground + Right * X + FVector(0, 0, H + Drop)) * GetActorTransform();
}

float AFTTrack::AirClearAt(float D) const
{
	if (Frames.Num() < 2) return 0.f;
	// anything looping or rolling within 60 m either side: clear its highest point over the land's line
	const int32 C = FMath::RoundToInt(D / Step);
	float Top = 0.f;
	for (int32 i = FMath::Max(0, C - 30); i <= FMath::Min(Frames.Num() - 1, C + 30); i += 2)
	{
		const FFTFrame& F = Frames[i];
		if (F.Flags & (FTFlag::Loop | FTFlag::Twist | FTFlag::Pipe | FTFlag::Coaster))
		{
			float Drop = 0.f;
			if (F.Flags & FTFlag::Coaster) Drop = 2600.f * Smooth(F.U / 0.18f) * Smooth((1.f - F.U) / 0.18f);
			float Room = FT::Edge;
			if (F.Flags & FTFlag::Coaster)
			{
				const FFTRails R = RailsAt(i * Step);
				Room = FMath::Max3(R.H[0], R.H[1], R.H[2]) + 700.f;
				// a loop or a corkscrew on the coaster sweeps the whole ribbon round
				if (F.Flags & (FTFlag::Loop | FTFlag::Twist)) Room = FMath::Max(Room, FFTRails::Sep + 2400.f);
			}
			Top = FMath::Max(Top, F.Pos.Z - (F.Ground.Z + Drop) + Room);
		}
	}
	return Top;
}

float AFTTrack::SurfaceH(float D, float X, float* OutSlope, bool* bSolid) const
{
	if (OutSlope) *OutSlope = 0.f;
	if (bSolid) *bSolid = true;
	float U = 0.f;
	const FFTSection* S = SectionAt(D, &U);
	if (!S) return 0.f;
	if (FlagsAt(D) & FTFlag::Gap) { if (bSolid) *bSolid = false; return 0.f; }
	if (S->Shape == EFTSec::HalfPipe)
	{
		// the U grows in over the first and last 12 % so the road flows into it and out again
		const float K = Smooth(U / 0.12f) * Smooth((1.f - U) / 0.12f) * (S->A > 0.f ? FMath::Min(S->A, 1.f) : 1.f);
		const float Ax = FMath::Abs(X);
		if (Ax <= HPFlat || K <= 0.f) return 0.f;
		const float Sn = FMath::Min((Ax - HPFlat) / HPR, HPMaxSin);
		const float C = FMath::Sqrt(1.f - Sn * Sn);
		float H = HPR * (1.f - C);
		float Sl = Sn / C;
		const float Xmax = HPFlat + HPR * HPMaxSin;
		if (Ax > Xmax) H += (Ax - Xmax) * Sl; // straight on past the curve, up to the barrier
		if (OutSlope) *OutSlope = K * Sl * FMath::Sign(X);
		return K * H;
	}
	if (S->Shape == EFTSec::Coaster)
	{
		const FFTRails R = RailsAt(D);
		int32 Bi = 0;
		for (int32 i = 1; i < FFTRails::Num; ++i) if (FMath::Abs(X - R.X[i]) < FMath::Abs(X - R.X[Bi])) Bi = i;
		if (FMath::Abs(X - R.X[Bi]) > FFTRails::HalfW + 40.f && bSolid) *bSolid = false;
		return R.H[Bi];
	}
	return 0.f;
}

float AFTTrack::NextFlagStart(float FromD, uint16 Flag, float Within) const
{
	const int32 a = FMath::Max(0, FMath::RoundToInt(FromD / Step));
	const int32 b = FMath::Min(Frames.Num() - 1, FMath::RoundToInt((FromD + Within) / Step));
	const bool bInside = (FrameAt(a).Flags & Flag) != 0;
	for (int32 i = a + 1; i <= b; ++i)
	{
		const bool bF = (Frames[i].Flags & Flag) != 0;
		if (bF && (!bInside || !(Frames[i - 1].Flags & Flag))) return i * Step;
	}
	return -1.f;
}

// ------------------------------------------------------------------------------------------ meshes
void AFTTrack::GroundBands(TArray<FVector2D>& GP, TArray<FLinearColor>& GC, float& WaterX) const
{
	GP = GroundProfile;
	GC = GroundColours;
	WaterX = 0.f;
	if (GP.Num() >= 2) return;
	GP.Reset(); GC.Reset();
	auto P = [&](float X, float Z, const TCHAR* Hex) { GP.Add(FVector2D(X, Z)); GC.Add(FTData::Hex(Hex)); };
	if (LevelIndex == 0)
	{
		P(-14000, -420, TEXT("6b5a48")); P(-12800, -300, TEXT("b89466")); P(-11000, -220, TEXT("d9b98a")); P(-1600, -70, TEXT("e0c08e"));
		P(1600, -70, TEXT("c98aa8")); P(6400, -60, TEXT("e8a6c4")); P(7000, -60, TEXT("2f6b52")); P(30000, -250, TEXT("24503e"));
		WaterX = -13400.f;
	}
	else if (LevelIndex == 1)
	{
		P(-30000, -300, TEXT("1d2350")); P(-9000, -120, TEXT("2b3466")); P(-1600, -70, TEXT("3a4478"));
		P(1600, -70, TEXT("3a4478")); P(9000, -120, TEXT("2b3466")); P(30000, -300, TEXT("1d2350"));
	}
	else if (LevelIndex == 2)
	{
		P(-30000, 200, TEXT("a2507a")); P(-12000, -120, TEXT("c9805a")); P(-1600, -70, TEXT("d98a5e"));
		P(1600, -70, TEXT("d98a5e")); P(12000, -120, TEXT("c9805a")); P(30000, 260, TEXT("a2507a"));
	}
	else if (LevelIndex == 3) // volcano: black basalt, ash, scorched red
	{
		P(-30000, 900, TEXT("2a1d26")); P(-9000, -60, TEXT("3b2a30")); P(-1600, -70, TEXT("54393a"));
		P(1600, -70, TEXT("54393a")); P(9000, -60, TEXT("3b2a30")); P(30000, 1200, TEXT("2a1d26"));
	}
	else if (LevelIndex == 4) // theme park: lawns and a sandy promenade
	{
		P(-30000, -300, TEXT("2f6b52")); P(-4200, -80, TEXT("4f9a6a")); P(-1600, -70, TEXT("efcf9f"));
		P(1600, -70, TEXT("efcf9f")); P(4200, -80, TEXT("4f9a6a")); P(30000, -300, TEXT("2f6b52"));
	}
	else if (LevelIndex == 5) // glacier: snowfields rising to the ridges
	{
		P(-30000, 2600, TEXT("bcd6f2")); P(-8000, -30, TEXT("e4f0ff")); P(-1600, -70, TEXT("ffffff"));
		P(1600, -70, TEXT("ffffff")); P(8000, -30, TEXT("e4f0ff")); P(30000, 2600, TEXT("bcd6f2"));
	}
	else if (LevelIndex == 6) // the seabed: pale sand, then the trench walls
	{
		P(-30000, 1400, TEXT("1a3f66")); P(-9000, -100, TEXT("b9a77e")); P(-1600, -70, TEXT("d9c89a"));
		P(1600, -70, TEXT("d9c89a")); P(9000, -100, TEXT("b9a77e")); P(30000, 1400, TEXT("1a3f66"));
	}
	else // midtown: sidewalks and blocks
	{
		P(-30000, -60, TEXT("3a3848")); P(-2700, -40, TEXT("9a98a8")); P(-1600, -70, TEXT("706e80"));
		P(1600, -70, TEXT("706e80")); P(2700, -40, TEXT("9a98a8")); P(30000, -60, TEXT("3a3848"));
	}
}

bool AFTTrack::BedClearance(const FVector& WorldP, float D, float& OutAbove) const
{
	const int32 i = FMath::RoundToInt(D / Step);
	if (!BedProbe.IsValidIndex(i) || BedProbe[i].Num() < 2) return false;
	const FFTFrame& F = Frames[i];
	const FVector R = GetActorTransform().InverseTransformPosition(WorldP) - F.Ground;
	const float X = FVector::DotProduct(R, F.GRight);
	const TArray<FVector2D>& L = BedProbe[i];
	if (X < L[0].X || X > L.Last().X) { OutAbove = R.Z - GroundZ(X); return true; } // over the land beside the road
	for (int32 k = 0; k + 1 < L.Num(); ++k)
		if (X <= L[k + 1].X) { OutAbove = R.Z - FMath::Lerp(L[k].Y, L[k + 1].Y, (X - L[k].X) / FMath::Max(1.0, L[k + 1].X - L[k].X)); return true; }
	return false;
}

float AFTTrack::GroundZ(float Xcm) const
{
	TArray<FVector2D> GP; TArray<FLinearColor> GC; float WaterX;
	GroundBands(GP, GC, WaterX);
	if (GP.Num() < 2) return -60.f;
	if (Xcm <= GP[0].X) return GP[0].Y;
	for (int32 k = 0; k + 1 < GP.Num(); ++k)
	{
		if (Xcm <= GP[k + 1].X)
		{
			const float t = (Xcm - GP[k].X) / FMath::Max(GP[k + 1].X - GP[k].X, 1.f);
			return FMath::Lerp(GP[k].Y, GP[k + 1].Y, t);
		}
	}
	return GP.Last().Y;
}

void AFTTrack::BuildMeshes()
{
	for (UProceduralMeshComponent* C : Chunks) if (C) C->DestroyComponent();
	Chunks.Reset();
	if (Frames.Num() < 2) return;

	const FFTLevelDef& L = FTData::Level(LevelIndex);
	UMaterialInterface* MRoad = RoadMaterial ? RoadMaterial.Get() : LoadAsset<UMaterialInterface>(TEXT("/Game/Core/Materials/MI_Road.MI_Road"));
	UMaterialInterface* MWall = WallMaterial ? WallMaterial.Get() : LoadAsset<UMaterialInterface>(TEXT("/Game/Core/Materials/MI_Wall.MI_Wall"));
	UMaterialInterface* MNeon = NeonMaterial ? NeonMaterial.Get() : LoadAsset<UMaterialInterface>(TEXT("/Game/Core/Materials/MI_RailNeon.MI_RailNeon"));
	UMaterialInterface* MGlass = GlassMaterial ? GlassMaterial.Get() : LoadAsset<UMaterialInterface>(TEXT("/Game/Core/Materials/MI_PipeGlass.MI_PipeGlass"));
	UMaterialInterface* MGround = GroundMaterial ? GroundMaterial.Get() : LoadAsset<UMaterialInterface>(TEXT("/Game/Core/Materials/MI_Ground.MI_Ground"));
	// 3.4: the land's own surface, level by level (ft_world.land_materials): slabs with joints, ripples or cracked
	// ground, drawn in the material over the level's ground colours. On banked turns, elevated stretches, loops and
	// corkscrews the camera sees mostly land, and nothing tall may stand near the road there. A level without the
	// asset keeps the plain ground; so does ft.Land 0.
	if (!GroundMaterial && CVarFTLand.GetValueOnAnyThread() != 0)
	{
		static const TCHAR* LandDirs[8] = { TEXT("Bayshore"), TEXT("ChemicalWorks"), TEXT("Nevada"), TEXT("Volcano"), TEXT("ThemePark"), TEXT("Glacier"), TEXT("Abyss"), TEXT("Metro") };
		const FString LandPath = FString::Printf(TEXT("/Game/Maps/%s/Materials/MI_Land.MI_Land"), LandDirs[FMath::Clamp(LevelIndex, 0, 7)]);
		if (UMaterialInterface* MLand = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *LandPath, nullptr, LOAD_NoWarn | LOAD_Quiet))) MGround = MLand;
	}
	UMaterialInterface* MRib = RibMaterial ? RibMaterial.Get() : LoadAsset<UMaterialInterface>(TEXT("/Game/Core/Materials/MI_PipeRib.MI_PipeRib"));
	UMaterialInterface* MWater = LoadAsset<UMaterialInterface>(TEXT("/Game/Core/Materials/MI_Water.MI_Water"));
	// 2.5: tower blocks over the road (lit windows) and lava
	UMaterialInterface* MBldg = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/Core/Materials/MI_Facade.MI_Facade"), nullptr, LOAD_NoWarn | LOAD_Quiet));
	if (!MBldg) MBldg = LoadAsset<UMaterialInterface>(TEXT("/Game/Core/Materials/M_Windows.M_Windows"));
	UMaterialInterface* MLava = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/Core/Materials/MI_Lava.MI_Lava"), nullptr, LOAD_NoWarn | LOAD_Quiet));
	if (MRoad)
	{
		// each level paints its road in its own colours
		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(MRoad, this);
		MID->SetVectorParameterValue(TEXT("RoadA"), L.RoadA);
		MID->SetVectorParameterValue(TEXT("Line"), L.Line);
		MID->SetVectorParameterValue(TEXT("KerbA"), L.KerbA);
		MID->SetVectorParameterValue(TEXT("KerbB"), L.KerbB);
		MID->SetVectorParameterValue(TEXT("Kicker"), L.Neon2);
		MRoad = MID;
	}

	// ground bands: (x cm, z cm) and colours, defaults per level
	TArray<FVector2D> GP;
	TArray<FLinearColor> GC;
	float WaterX = 0.f; // L1: sea on the left beyond this x
	GroundBands(GP, GC, WaterX);

	// 2.7: the land beside the road stops short of any other part of the road. It used to run its full 300 m
	// out from every frame, so on the inside of a tight curve, or where the circuit comes back past itself, the
	// rising hillside (and Bayshore's sea) cut straight across the road. Reach = how far out it may go each side.
	TArray<float> ReachL, ReachR;
	ReachL.SetNumUninitialized(Frames.Num()); ReachR.SetNumUninitialized(Frames.Num());
	auto HAt = [&GP](float X)
	{
		if (X <= GP[0].X) return (float)GP[0].Y;
		for (int32 k = 0; k + 1 < GP.Num(); ++k)
			if (X <= GP[k + 1].X) return (float)FMath::Lerp(GP[k].Y, GP[k + 1].Y, (X - GP[k].X) / FMath::Max(1.0, GP[k + 1].X - GP[k].X));
		return (float)GP.Last().Y;
	};
	const float SeaZ = GP[0].Y + 120.f, SeaFar = 90000.f;
	int32 LandClipped = 0;
	for (int32 i = 0; i < Frames.Num(); ++i)
	{
		const FFTFrame& F = Frames[i];
		for (int32 sd = 0; sd < 2; ++sd)
		{
			const float Land = sd == 0 ? -GP[0].X : GP.Last().X;
			const float Far = (sd == 0 && WaterX != 0.f) ? SeaFar : Land;
			float Reach = Far;
			for (float X = FT::Edge + 400.f; CVarFTClear.GetValueOnAnyThread() && X <= Far; X += (X < 30000.f ? 600.f : 1500.f))
			{
				const float Xs = sd == 0 ? -X : X;
				const float Z = X <= Land ? HAt(Xs) : SeaZ;
				// the sea and the land under it: the sea's top is what shows
				const float Zt = (sd == 0 && WaterX != 0.f && X >= -WaterX) ? FMath::Max(Z, SeaZ) : Z;
				if (GroundHitsRoad(F.Ground + F.GRight * Xs + FVector(0, 0, Zt), i - 30, i + 30))
				{
					Reach = FMath::Max(FT::Edge + 250.f, X - 600.f);
					break;
				}
			}
			(sd == 0 ? ReachL : ReachR)[i] = Reach;
			if (Reach < Far) ++LandClipped;
		}
	}
	UE_LOG(LogFT, Display, TEXT("FTCLEAR level=%d land: %d of %d frame sides cut short of another part of the road"), LevelIndex, LandClipped, Frames.Num() * 2);

	// ---- 3.1: the bed. The land left the strip the road sits in open (32 m: a pipe's width). A banked turn rolls
	// the road about its centre line, so its low edge sinks 11 m under the land, and the chase camera, rolled with it,
	// looked over the sunken barrier and down through that strip: nothing was there, so the sky's below-the-horizon
	// painting filled half the screen ("the sky comes through the ground on turns"). The bed closes the strip: land
	// level where the road stands above the land, cut down under the road (and a pipe's tube) where it does not,
	// with the cut's sides climbing back to the land. It is looks only (no collision), in the land's material.
	int32 kIn = 0; // GP[kIn], GP[kIn + 1]: the land's inner edges, left and right of the road
	for (int32 k = 0; k + 1 < GP.Num(); ++k) if (GP[k].X < 0.f && GP[k + 1].X > 0.f) { kIn = k; break; }
	const FVector2D InL = GP[kIn], InR = GP[kIn + 1];
	TArray<FBedSec> Secs;
	Secs.SetNum(Frames.Num());
	BedCut.Reset(); BedCut.SetNumZeroed(Frames.Num()); BedInL = InL; BedInR = InR; // 3.4: kept for the dressing's Bank rule
	float LandLow = TNumericLimits<float>::Max();
	for (const FVector2D& Q : GP) LandLow = FMath::Min(LandLow, (float)Q.Y);
	float MinZ = TNumericLimits<float>::Max(); // the lowest land anywhere (the floor goes under it)
	FBox2D Footprint(ForceInit);
	for (int32 j = 0; j < Frames.Num(); ++j)
	{
		const FFTFrame& Fr = Frames[j];
		FBedSec& S = Secs[j];
		Footprint += FVector2D(Fr.Ground.X, Fr.Ground.Y);
		MinZ = FMath::Min(MinZ, (float)Fr.Ground.Z + LandLow);
		if (Fr.Flags & FTFlag::Gap) continue; // no road over a jump's gap
		const float Dj = j * Step;
		auto Put = [&Fr, &S](float X, float H)
		{
			const FVector R = Fr.Pos + Fr.Right * X + Fr.Up * H - Fr.Ground;
			S.P.Add(FVector2D(FVector::DotProduct(R, Fr.GRight), R.Z - BedClear));
		};
		// 3.6 (owner: "the corkscrews clip through the ground"): the deck's underside was all the bed was cut under. Upside
		// down in a corkscrew the car hangs under the deck and the chase camera under the car, so both sat inside the
		// land. Where the road leans past 60 degrees (corkscrews, coaster rolls, loops, pipes) the room over the deck is
		// cut out too. A level or banked road's room is above its deck and never lowers the bed, so turns are untouched.
		const bool bRoom = CVarFTBedRoom.GetValueOnAnyThread() != 0 && ((Fr.Flags & (FTFlag::Twist | FTFlag::Coaster)) || Fr.Up.Z < 0.5f);
		if (Fr.Flags & FTFlag::Coaster)
		{
			const FFTRails R = RailsAt(Dj);
			for (int32 s = 0; s < FFTRails::Num; ++s)   // 3.3: three rails (the PR's bed was written for two)
			{
				const float Xc = R.X[s], Hc = R.H[s], W = FFTRails::HalfW;
				S.Begin();
				Put(Xc - W, Hc + GuardH); Put(Xc - W - GuardW, Hc + GuardH); Put(Xc - W - GuardW, Hc - DeckDepth);
				Put(Xc + W + GuardW, Hc - DeckDepth); Put(Xc + W + GuardW, Hc + GuardH); Put(Xc + W, Hc + GuardH);
				if (bRoom) { S.Begin(); Put(Xc - W, Hc + BedRoom); Put(Xc, Hc + BedRoom); Put(Xc + W, Hc + BedRoom); }
			}
		}
		else
		{
			// the barrier's top, its outer face, the deck's underside column by column, and the other barrier
			const float E = FT::Edge, HL = SurfaceH(Dj, -E), HR = SurfaceH(Dj, E);
			S.Begin();
			Put(-E, HL + FT::WallH); Put(-E - WallW, HL + FT::WallH); Put(-E - WallW, HL - DeckDepth);
			for (int32 k = 0; k <= 12; ++k) { const float X = -E + 2.f * E * k / 12.f; Put(X, SurfaceH(Dj, X) - DeckDepth); }
			Put(E + WallW, HR - DeckDepth); Put(E + WallW, HR + FT::WallH); Put(E, HR + FT::WallH);
			if (bRoom)
			{
				S.Begin();
				for (int32 k = 0; k <= 12; ++k) { const float X = -E + 2.f * E * k / 12.f; Put(X, SurfaceH(Dj, X) + BedRoom); }
			}
		}
		if (Fr.Flags & FTFlag::Pipe)
		{
			// the lower half of the glass tube
			const FVector Uz = FVector::CrossProduct(Fr.Fwd, Fr.GRight).GetSafeNormal();
			S.Begin();
			for (int32 k = 0; k <= 16; ++k)
			{
				const float a = PI + PI * k / 16.f;
				const FVector R = Uz * PipeAxis + (Fr.GRight * FMath::Cos(a) + Uz * FMath::Sin(a)) * (PipeR + 12.f);
				S.P.Add(FVector2D(FVector::DotProduct(R, Fr.GRight), R.Z - BedClear));
			}
		}
		MinZ = FMath::Min(MinZ, (float)Fr.Ground.Z + S.Lowest());
	}
	// the bed's line across at frame i, left edge of the land to right edge: (x, z above the base path)
	auto BedLine = [&](int32 i, TArray<FVector2D>& Out)
	{
		Out.Reset();
		TArray<float> Xs;
		for (const FVector2D& Q : Secs[i].P) if (Q.X > InL.X + 6.f && Q.X < InR.X - 6.f) Xs.Add(Q.X);
		// 3.6: a corkscrew's bed is cut under the road's room for 14 m either side (where the chase camera rides),
		// sampled every 1.5 m across as well as at the road's own points (a frame on its side has them all near x = 0)
		int32 Reach = 1;
		if (CVarFTBedRoom.GetValueOnAnyThread())
			for (const int32 j : { i, FMath::Max(i - TwistBedReach, 0), FMath::Min(i + TwistBedReach, Frames.Num() - 1) })
				if (Frames[j].Flags & FTFlag::Twist) { Reach = TwistBedReach; break; }
		if (Reach > 1)
			for (float X = InL.X + 150.f; X < InR.X - 6.f; X += 150.f) Xs.Add(X);
		Xs.Sort();
		const int32 j0 = FMath::Max(i - Reach, 0), j1 = FMath::Min(i + Reach, Frames.Num() - 1);
		Out.Add(InL);
		for (const float X : Xs)
		{
			if (X - Out.Last().X < 6.f) continue;
			float Z = FMath::Lerp(InL.Y, InR.Y, (X - InL.X) / (InR.X - InL.X));
			// this frame's road and its neighbours' (a pitched, banked frame leans a little along the road)
			for (int32 j = j0; j <= j1; ++j) Z = FMath::Min(Z, Secs[j].Limit(X, BedSlope));
			Out.Add(FVector2D(X, Z));
		}
		Out.Add(InR);
		// points on a straight run add nothing (a level road's bed is four points)
		for (int32 k = 1; k + 1 < Out.Num(); )
		{
			const FVector2D& A = Out[k - 1]; const FVector2D& B = Out[k + 1];
			const double Zc = FMath::Lerp(A.Y, B.Y, (Out[k].X - A.X) / (B.X - A.X));
			if (Out[k].Y >= Zc - 0.05 && Out[k].Y <= Zc + 1.0) Out.RemoveAt(k); else ++k;
		}
	};
	// the floor: one sheet under the whole level, below its lowest land (made after the chunks, below)
	FloorZ = MinZ - 150.f;
	int32 BedTris = 0;
	BedProbe.Reset();
	if (FParse::Param(FCommandLine::Get(), TEXT("ftbedprobe"))) BedProbe.SetNum(Frames.Num());
	// 3.2: the track is locked. A fingerprint of the route (every frame's place and attitude) and of everything solid
	// (the four sections with collision), logged as FTTRACK, so a scenery change can be shown to have left both alone.
	uint32 RouteCrc = 0, SolidCrc = 0; int32 SolidTris = 0;
	for (const FFTFrame& Fr : Frames)
	{
		const double Pk[] = { Fr.Pos.X, Fr.Pos.Y, Fr.Pos.Z, Fr.Fwd.X, Fr.Fwd.Y, Fr.Fwd.Z, Fr.Right.X, Fr.Right.Y, Fr.Right.Z, Fr.Up.X, Fr.Up.Y, Fr.Up.Z, Fr.Ground.X, Fr.Ground.Y, Fr.Ground.Z, (double)Fr.Flags };
		RouteCrc = FCrc::MemCrc32(Pk, sizeof(Pk), RouteCrc);
	}

	const int32 ChunkLen = 200;
	const FLinearColor Neon = L.Rail;
	// 3.4 load time: cooking the road's collision was 0.8 s of every level's build, on the game thread, chunk after chunk.
	// Only the chunks round where this race starts (scene 0, or the scene a test or the career asked for) cook there and
	// then; the rest cook on worker threads and are solid well before the car gets to them (a chunk is 400 m, the window
	// runs 1.2 km past the start, and all of them are done in a fraction of a second). Retakes start later, after it.
	float StartD = 0.f;
	bool bAsyncOk = false;
	if (UWorld* W = GetWorld(); W && W->IsGameWorld() && LightAt < 0.f && !FParse::Param(FCommandLine::Get(), TEXT("ftsynccook")))
	{
		bAsyncOk = true;
		int32 Sc = 0;
		if (const UFTGameInstance* G = Cast<UFTGameInstance>(W->GetGameInstance())) Sc = FMath::Max(0, G->PendingScene);
		const FFTLevelDef& Def = FTData::Level(LevelIndex);
		if (Def.Scenes.Num()) StartD = Def.Scenes[FMath::Clamp(Sc, 0, Def.Scenes.Num() - 1)].D * FT::DU;
	}
	const float SyncFrom = StartD - 2.f * ChunkLen * Step, SyncTo = StartD + 3.f * ChunkLen * Step;
	int32 AsyncChunks = 0;
	for (int32 c0 = 0; c0 < Frames.Num() - 1; c0 += ChunkLen)
	{
		const int32 c1 = FMath::Min(c0 + ChunkLen, Frames.Num() - 1);
		FBuf Deck, Wall, NeonB, Fence, Glass, Rib, Ground, Water, Bldg, Lobby, Lava, Bed;
		TArray<int32> PrevBed, PrevSkirt; TArray<float> PrevBedX; // 3.1: the bed's last ring, and the land's skirts
		TArray<FVector2D> DeckFlags; // UV1 = (warning stripes, kicker paint)
		TArray<int32> Prev[10];
		TArray<TArray<int32>> PrevRings;
		int32 PrevKind = -1, PrevKind1 = -1, PrevKind2 = -1;
		TArray<int32> PrevLobby, PrevCeilLights, PrevRiver;
		for (int32 i = c0; i <= c1; ++i)
		{
			const FFTFrame& F = Frames[i];
			const float V = i * Step / 1000.f;
			const bool bRoad = !(F.Flags & FTFlag::Gap);
			const bool bLink = i > c0 && bRoad && !(Frames[i - 1].Flags & FTFlag::Gap);
			const FLinearColor VC(F.Flags & FTFlag::Warn ? 1.f : 0.f, (F.Flags & (FTFlag::Kicker | FTFlag::Landing)) ? 1.f : 0.f, 0.f, 1.f);
			auto At = [&F](float X, float H) { return F.Pos + F.Right * X + F.Up * H; };
			const float E = FT::Edge;

			// 2.5: a frame's deck is either the road (13 columns across, following the half pipe's U where there is
			// one) or two coaster rails (5 columns each). Where the kind changes, the previous frame gets a second
			// ring in the new kind (it lies in the same plane) so the deck never has a hole at the seam.
			const bool bCoaster = (F.Flags & FTFlag::Coaster) != 0;
			const int32 Kind = bCoaster ? 1 : 0;
			const float Di = i * Step;
			auto Strips = [this](const FFTFrame& Fr, float Dd, int32 K, TArray<TArray<FVector2D>>& Out)
			{
				Out.Reset();
				if (K == 0)
				{
					TArray<FVector2D>& C = Out.AddDefaulted_GetRef();
					for (int32 k = 0; k <= 12; ++k)
					{
						const float X = -FT::Edge + 2.f * FT::Edge * k / 12.f;
						C.Add(FVector2D(X, SurfaceH(Dd, X)));
					}
				}
				else
				{
					const FFTRails R = RailsAt(Dd);
					for (int32 s = 0; s < FFTRails::Num; ++s)
					{
						const float Xc = R.X[s], Hc = R.H[s];
						TArray<FVector2D>& C = Out.AddDefaulted_GetRef();
						for (int32 k = 0; k <= 4; ++k) C.Add(FVector2D(Xc - FFTRails::HalfW + 2.f * FFTRails::HalfW * k / 4.f, Hc));
					}
				}
			};
			// one deck ring per strip: top columns, then bottom L/R, side L (top, bottom), side R (top, bottom)
			auto EmitDeck = [&](const FFTFrame& Fr, float Vv, const TArray<FVector2D>& Cols, const FLinearColor& Vc) -> TArray<int32>
			{
				auto AtF = [&Fr](float X, float H) { return Fr.Pos + Fr.Right * X + Fr.Up * H; };
				TArray<int32> R;
				const int32 Nc = Cols.Num();
				const float X0 = Cols[0].X, X1 = Cols.Last().X, W2 = FMath::Max(X1 - X0, 1.f);
				for (int32 k = 0; k < Nc; ++k)
				{
					const float Sl = k == 0 ? (Cols[1].Y - Cols[0].Y) / (Cols[1].X - Cols[0].X) : (k == Nc - 1 ? (Cols[k].Y - Cols[k - 1].Y) / (Cols[k].X - Cols[k - 1].X) : (Cols[k + 1].Y - Cols[k - 1].Y) / (Cols[k + 1].X - Cols[k - 1].X));
					const FVector Nrm = (Fr.Up - Fr.Right * Sl).GetSafeNormal();
					// the paint (lines, kerbs) is laid out on 0..1 across: a coaster rail is painted as a little road
					R.Add(Deck.Add(AtF(Cols[k].X, Cols[k].Y), Nrm, FVector2D((Cols[k].X - X0) / W2, Vv), Vc));
				}
				const float HL = Cols[0].Y, HR = Cols.Last().Y;
				const float Ex = Nc > 5 ? WallW : 0.f;
				// the underside follows the top, column by column (2.5 first build: one flat quad from edge to edge at
				// the wall-top height, which in a half pipe was an invisible ceiling through the U the car ran into)
				for (int32 k = 0; k < Nc; ++k) R.Add(Deck.Add(AtF(Cols[k].X, Cols[k].Y - DeckDepth), -Fr.Up, FVector2D(float(k) / FMath::Max(Nc - 1, 1), Vv)));
				R.Add(Deck.Add(AtF(X0 - Ex, HL), -Fr.Right, FVector2D(0, Vv))); R.Add(Deck.Add(AtF(X0 - Ex, HL - DeckDepth), -Fr.Right, FVector2D(0.1f, Vv)));
				R.Add(Deck.Add(AtF(X1 + Ex, HR), Fr.Right, FVector2D(0, Vv))); R.Add(Deck.Add(AtF(X1 + Ex, HR - DeckDepth), Fr.Right, FVector2D(0.1f, Vv)));
				return R;
			};
			auto LinkDeck = [&](const TArray<int32>& P, const TArray<int32>& R, int32 Nc)
			{
				for (int32 k = 0; k + 1 < Nc; ++k) Deck.Quad(P[k], P[k + 1], R[k], R[k + 1], (Deck.N[R[k]] + Deck.N[R[k + 1]]).GetSafeNormal());
				for (int32 k = 0; k + 1 < Nc; ++k) Deck.Quad(P[Nc + k], P[Nc + k + 1], R[Nc + k], R[Nc + k + 1], -(Deck.N[R[k]] + Deck.N[R[k + 1]]).GetSafeNormal());
				Deck.Quad(P[2 * Nc], P[2 * Nc + 1], R[2 * Nc], R[2 * Nc + 1], -F.Right);
				Deck.Quad(P[2 * Nc + 2], P[2 * Nc + 3], R[2 * Nc + 2], R[2 * Nc + 3], F.Right);
			};
			TArray<TArray<FVector2D>> Cols;
			Strips(F, Di, Kind, Cols);
			TArray<TArray<int32>> Rings;
			for (const TArray<FVector2D>& C : Cols) Rings.Add(EmitDeck(F, V, C, VC));
			if (bLink)
			{
				if (PrevKind == Kind && PrevRings.Num() == Rings.Num())
				{
					for (int32 s = 0; s < Rings.Num(); ++s) LinkDeck(PrevRings[s], Rings[s], Cols[s].Num());
				}
				else
				{
					// the seam: the previous frame again, in this frame's kind
					const FFTFrame& Fp = Frames[i - 1];
					const FLinearColor VCp(Fp.Flags & FTFlag::Warn ? 1.f : 0.f, 0.f, 0.f, 1.f);
					TArray<TArray<FVector2D>> Cp;
					Strips(Fp, (i - 1) * Step, Kind, Cp);
					for (int32 s = 0; s < Rings.Num(); ++s) LinkDeck(EmitDeck(Fp, (i - 1) * Step / 1000.f, Cp[s], VCp), Rings[s], Cols[s].Num());
				}
			}
			PrevRings = Rings; PrevKind = Kind;
			while (DeckFlags.Num() < Deck.V.Num()) DeckFlags.Add(FVector2D(VC.R, VC.G));

			// barriers: inner face, top, outer face on each side (the road), or low guard rails on each coaster deck
			TArray<FVector2D> Guards; // (x of the inner face, height of its foot), sign packed in the order: L, R per strip
			TArray<float> GuardSide;
			if (Kind == 0)
			{
				Guards.Add(FVector2D(-E, Cols[0][0].Y)); GuardSide.Add(-1.f);
				Guards.Add(FVector2D(E, Cols[0].Last().Y)); GuardSide.Add(1.f);
			}
			else
			{
				for (const TArray<FVector2D>& C : Cols)
				{
					Guards.Add(FVector2D(C[0].X, C[0].Y)); GuardSide.Add(-1.f);
					Guards.Add(FVector2D(C.Last().X, C.Last().Y)); GuardSide.Add(1.f);
				}
			}
			const float GH = Kind == 0 ? FT::WallH : GuardH, GW = Kind == 0 ? WallW : GuardW;
			TArray<int32> W;
			for (int32 g = 0; g < Guards.Num(); ++g)
			{
				const float sgn = GuardSide[g];
				const float Xi = Guards[g].X, Xo = Guards[g].X + sgn * GW, H0 = Guards[g].Y;
				const FVector In = -F.Right * sgn;
				W.Add(Wall.Add(At(Xi, H0), In, FVector2D(0, V))); W.Add(Wall.Add(At(Xi, H0 + GH), In, FVector2D(1, V)));
				W.Add(Wall.Add(At(Xi, H0 + GH), F.Up, FVector2D(0, V))); W.Add(Wall.Add(At(Xo, H0 + GH), F.Up, FVector2D(1, V)));
				W.Add(Wall.Add(At(Xo, H0 + GH), -In, FVector2D(0, V))); W.Add(Wall.Add(At(Xo, H0 - DeckDepth), -In, FVector2D(1, V)));
			}
			if (bLink && PrevKind1 == Kind && Prev[1].Num() == W.Num())
			{
				const TArray<int32>& P = Prev[1];
				for (int32 g = 0; g < Guards.Num(); ++g)
				{
					const int32 o = g * 6;
					const float sgn = GuardSide[g];
					Wall.Quad(P[o + 0], P[o + 1], W[o + 0], W[o + 1], -F.Right * sgn);
					Wall.Quad(P[o + 2], P[o + 3], W[o + 2], W[o + 3], F.Up);
					Wall.Quad(P[o + 4], P[o + 5], W[o + 4], W[o + 5], F.Right * sgn);
				}
			}
			Prev[1] = W; PrevKind1 = Kind;

			// neon along the top of each barrier facing the road (on a coaster: along the top of each guard rail)
			TArray<int32> Nn;
			for (int32 g = 0; g < Guards.Num(); ++g)
			{
				const float sgn = GuardSide[g];
				const FVector In = -F.Right * sgn;
				if (Kind == 0)
				{
					const float X = Guards[g].X - sgn * 2.f;
					Nn.Add(NeonB.Add(At(X, Guards[g].Y + FT::WallH - 32.f), In, FVector2D(0, V), Neon));
					Nn.Add(NeonB.Add(At(X, Guards[g].Y + FT::WallH - 10.f), In, FVector2D(1, V), Neon));
				}
				else
				{
					const float Hn = Guards[g].Y + GuardH + 2.f;
					Nn.Add(NeonB.Add(At(Guards[g].X, Hn), F.Up, FVector2D(0, V), Neon));
					Nn.Add(NeonB.Add(At(Guards[g].X + sgn * GuardW, Hn), F.Up, FVector2D(1, V), Neon));
				}
			}
			if (bLink && PrevKind2 == Kind && Prev[2].Num() == Nn.Num())
			{
				const TArray<int32>& P = Prev[2];
				for (int32 g = 0; g < Guards.Num(); ++g)
					NeonB.Quad(P[g * 2], P[g * 2 + 1], Nn[g * 2], Nn[g * 2 + 1], Kind == 0 ? F.Right * GuardSide[g] : F.Up);
			}
			Prev[2] = Nn; PrevKind2 = Kind;

			// invisible fence above the barrier (collision only) so a car can't sail off a bend
			if (Kind == 0)
			{
				TArray<int32> Fe;
				for (int32 g = 0; g < 2; ++g)
				{
					const float sgn = GuardSide[g];
					const FVector In = -F.Right * sgn;
					Fe.Add(Fence.Add(At(sgn * E, Guards[g].Y + FT::WallH), In, FVector2D(0, V))); Fe.Add(Fence.Add(At(sgn * E, Guards[g].Y + 900.f), In, FVector2D(1, V)));
				}
				if (bLink && Prev[3].Num() == 4 && !(F.Flags & (FTFlag::Loop | FTFlag::Pipe | FTFlag::Twist)))
				{
					const TArray<int32>& P = Prev[3];
					Fence.Quad(P[0], P[1], Fe[0], Fe[1], F.Right);
					Fence.Quad(P[2], P[3], Fe[2], Fe[3], -F.Right);
				}
				Prev[3] = Fe;
			}
			else Prev[3].Reset();

			// 2.5: a tower block over the road: the road runs through its ground floor
			if (F.Flags & FTFlag::Building)
			{
				const FFTSection& BS = Sections[F.Section];
				const float Hb = BS.A > 0.f ? BS.A * 100.f : 5200.f;
				const float Xi = E + WallW, Xo = E + WallW + BldgOut;
				const float Vb = i * Step;
				TArray<int32> Bo;
				// facade: left wall, roof, right wall (the windows material works in world space)
				Bo.Add(Bldg.Add(At(-Xo, -BldgBelow), -F.Right, FVector2D(0, Vb))); Bo.Add(Bldg.Add(At(-Xo, Hb), -F.Right, FVector2D(1, Vb)));
				Bo.Add(Bldg.Add(At(-Xo, Hb), F.Up, FVector2D(0, Vb))); Bo.Add(Bldg.Add(At(Xo, Hb), F.Up, FVector2D(1, Vb)));
				Bo.Add(Bldg.Add(At(Xo, Hb), F.Right, FVector2D(0, Vb))); Bo.Add(Bldg.Add(At(Xo, -BldgBelow), F.Right, FVector2D(1, Vb)));
				// the lobby: ceiling and its inner walls above the barriers
				TArray<int32> Lo;
				Lo.Add(Lobby.Add(At(-Xi, BldgCeil), -F.Up, FVector2D(0, V))); Lo.Add(Lobby.Add(At(Xi, BldgCeil), -F.Up, FVector2D(1, V)));
				Lo.Add(Lobby.Add(At(-Xi, FT::WallH), F.Right, FVector2D(0, V))); Lo.Add(Lobby.Add(At(-Xi, BldgCeil), F.Right, FVector2D(1, V)));
				Lo.Add(Lobby.Add(At(Xi, FT::WallH), -F.Right, FVector2D(0, V))); Lo.Add(Lobby.Add(At(Xi, BldgCeil), -F.Right, FVector2D(1, V)));
				const bool bPrevB = i > c0 && (Frames[i - 1].Flags & FTFlag::Building) && Prev[7].Num() == Bo.Num();
				if (bPrevB)
				{
					const TArray<int32>& P = Prev[7];
					Bldg.Quad(P[0], P[1], Bo[0], Bo[1], -F.Right);
					Bldg.Quad(P[2], P[3], Bo[2], Bo[3], F.Up);
					Bldg.Quad(P[4], P[5], Bo[4], Bo[5], F.Right);
					const TArray<int32>& Q = PrevLobby;
					Lobby.Quad(Q[0], Q[1], Lo[0], Lo[1], -F.Up);
					Lobby.Quad(Q[2], Q[3], Lo[2], Lo[3], F.Right);
					Lobby.Quad(Q[4], Q[5], Lo[4], Lo[5], -F.Right);
				}
				Prev[7] = Bo; PrevLobby = Lo;
				// ceiling light strips
				TArray<int32> Cl;
				for (int32 sgn = -1; sgn <= 1; sgn += 2)
				{
					Cl.Add(NeonB.Add(At(sgn * 650.f - 40.f, BldgCeil - 4.f), -F.Up, FVector2D(0, V), FLinearColor(1.f, 0.95f, 0.85f)));
					Cl.Add(NeonB.Add(At(sgn * 650.f + 40.f, BldgCeil - 4.f), -F.Up, FVector2D(1, V), FLinearColor(1.f, 0.95f, 0.85f)));
				}
				if (bPrevB && PrevCeilLights.Num() == 4)
				{
					NeonB.Quad(PrevCeilLights[0], PrevCeilLights[1], Cl[0], Cl[1], -F.Up);
					NeonB.Quad(PrevCeilLights[2], PrevCeilLights[3], Cl[2], Cl[3], -F.Up);
				}
				PrevCeilLights = Cl;
				// the front and back faces, round the opening
				const bool bFirst = i == 0 || !(Frames[i - 1].Flags & FTFlag::Building);
				const bool bLast = i + 1 >= Frames.Num() || !(Frames[i + 1].Flags & FTFlag::Building);
				if (bFirst || bLast)
				{
					const FVector Nf = bFirst ? -F.Fwd : F.Fwd;
					auto Face = [&](float Xa, float Xb, float Ha, float Hb2)
					{
						const int32 a = Bldg.Add(At(Xa, Ha), Nf, FVector2D(0, 0)), b = Bldg.Add(At(Xb, Ha), Nf, FVector2D(1, 0));
						const int32 c = Bldg.Add(At(Xa, Hb2), Nf, FVector2D(0, 1)), d = Bldg.Add(At(Xb, Hb2), Nf, FVector2D(1, 1));
						Bldg.Quad(a, b, c, d, Nf);
					};
					Face(-Xo, -Xi, -BldgBelow, Hb);
					Face(Xi, Xo, -BldgBelow, Hb);
					Face(-Xi, Xi, BldgCeil, Hb);
					// a lit sign band over the entrance
					const int32 a = NeonB.Add(At(-Xi + 200.f, BldgCeil + 120.f) + Nf * 6.f, Nf, FVector2D(0, 0), L.Neon2), b = NeonB.Add(At(Xi - 200.f, BldgCeil + 120.f) + Nf * 6.f, Nf, FVector2D(1, 0), L.Neon2);
					const int32 c = NeonB.Add(At(-Xi + 200.f, BldgCeil + 330.f) + Nf * 6.f, Nf, FVector2D(0, 1), L.Neon2), d = NeonB.Add(At(Xi - 200.f, BldgCeil + 330.f) + Nf * 6.f, Nf, FVector2D(1, 1), L.Neon2);
					NeonB.Quad(a, b, c, d, Nf);
				}
			}
			else { Prev[7].Reset(); PrevCeilLights.Reset(); }

			// 2.5: lava, glowing in the dips under jump gaps and in rivers beside the road
			if (L.bLava && MLava)
			{
				// the gap's land dips 4 to 18 m; the lake sits in the dip, well under the lips of the road
				const bool bPool = (F.Flags & FTFlag::Gap) != 0;
				TArray<int32> Lv;
				if (bPool)
				{
					const float Zp = F.Ground.Z + 60.f;
					FVector A0 = F.Ground + F.GRight * -9000.f, A1 = F.Ground + F.GRight * 9000.f;
					A0.Z = A1.Z = Zp;
					Lv.Add(Lava.Add(A0, FVector::UpVector, FVector2D(-4.5f, V), FLinearColor(1.f, 0.45f, 0.08f)));
					Lv.Add(Lava.Add(A1, FVector::UpVector, FVector2D(4.5f, V), FLinearColor(1.f, 0.45f, 0.08f)));
					if (Prev[8].Num() == 2) Lava.Quad(Prev[8][0], Prev[8][1], Lv[0], Lv[1], FVector::UpVector);
				}
				Prev[8] = Lv;
				TArray<int32> Rv;
				for (const FVector2D& Band : L.LavaBands)
				{
					for (const float X : { Band.X, Band.Y })
					{
						// on the land's profile at that x
						float Z = GP[0].Y;
						for (int32 k = 0; k + 1 < GP.Num(); ++k)
							if (X >= GP[k].X && X <= GP[k + 1].X) { Z = FMath::Lerp(GP[k].Y, GP[k + 1].Y, (X - GP[k].X) / FMath::Max(1.f, GP[k + 1].X - GP[k].X)); break; }
						Rv.Add(Lava.Add(F.Ground + F.GRight * X + FVector(0, 0, Z + 25.f), FVector::UpVector, FVector2D(X / 2000.f, V), FLinearColor(1.f, 0.4f, 0.06f)));
					}
				}
				if (i > c0 && PrevRiver.Num() == Rv.Num())
					for (int32 k = 0; k + 1 < Rv.Num(); k += 2) Lava.Quad(PrevRiver[k], PrevRiver[k + 1], Rv[k], Rv[k + 1], FVector::UpVector);
				PrevRiver = Rv;
			}

			// glass tube round pipe sections, with lit ribs
			if (F.Flags & FTFlag::Pipe)
			{
				const FFTFrame& Fp = F;
				const FVector Axis = Fp.Ground + FVector::CrossProduct(Fp.Fwd, Fp.GRight).GetSafeNormal() * PipeAxis;
				const FVector Ux = Fp.GRight, Uz = FVector::CrossProduct(Fp.Fwd, Fp.GRight).GetSafeNormal();
				TArray<int32> G;
				const int32 Seg = 28;
				for (int32 k = 0; k <= Seg; ++k)
				{
					const float a = 2.f * PI * k / Seg;
					const FVector Dir = Ux * FMath::Cos(a) + Uz * FMath::Sin(a);
					G.Add(Glass.Add(Axis + Dir * PipeR, -Dir, FVector2D(float(k) / Seg, V)));
				}
				if (i > c0 && (Frames[i - 1].Flags & FTFlag::Pipe) && Prev[4].Num() == G.Num())
				{
					for (int32 k = 0; k < Seg; ++k) Glass.Quad(Prev[4][k], Prev[4][k + 1], G[k], G[k + 1], -(Glass.V[G[k]] - Axis));
				}
				Prev[4] = G;
				if ((i % 6) == 0)
				{
					TArray<int32> Ra, Rb;
					for (int32 k = 0; k <= Seg; ++k)
					{
						const float a = 2.f * PI * k / Seg;
						const FVector Dir = Ux * FMath::Cos(a) + Uz * FMath::Sin(a);
						Ra.Add(Rib.Add(Axis + Dir * (PipeR - 8.f) - Fp.Fwd * 20.f, -Dir, FVector2D(0, 0)));
						Rb.Add(Rib.Add(Axis + Dir * (PipeR - 8.f) + Fp.Fwd * 20.f, -Dir, FVector2D(1, 0)));
					}
					for (int32 k = 0; k < Seg; ++k) Rib.Quad(Ra[k], Ra[k + 1], Rb[k], Rb[k + 1], -(Rib.V[Ra[k]] - Axis));
				}
			}
			else Prev[4].Reset();

			// the land beside the road
			{
				const FVector GUp = FVector::CrossProduct(F.Fwd.GetSafeNormal2D().IsNearlyZero() ? F.GRight.RotateAngleAxis(-90.f, FVector::UpVector) : F.Fwd.GetSafeNormal2D(), F.GRight).GetSafeNormal();
				TArray<int32> Gr;
				for (int32 k = 0; k < GP.Num(); ++k)
				{
					// 2.7: pulled in to this frame's reach (vertices past it fold onto the cut, at the land's height there)
					const float Xk = FMath::Clamp((float)GP[k].X, -ReachL[i], ReachR[i]);
					const float Zk = Xk == (float)GP[k].X ? (float)GP[k].Y : HAt(Xk);
					const FVector P = F.Ground + F.GRight * Xk + FVector::UpVector * Zk;
					Gr.Add(Ground.Add(P, FVector::UpVector, FVector2D(Xk / 2000.f, V), GC[k]));
				}
				if (i > c0)
				{
					const TArray<int32>& P = Prev[5];
					for (int32 k = 0; k + 1 < GP.Num(); ++k)
					{
						if (GP[k].X < 0.f && GP[k + 1].X > 0.f) continue; // the road sits here
						Ground.Quad(P[k], P[k + 1], Gr[k], Gr[k + 1], FVector::UpVector);
					}
				}
				Prev[5] = Gr;
				(void)GUp;
				// 3.1: the land's outer edges run on, down to the floor: a shore into Bayshore's sea, a slope elsewhere.
				// Not where the land was cut short of another part of the road, and not across one.
				// They go with the bed (looks only), so the land's collision is what it always was.
				{
					TArray<int32> Sk; // left: the land's edge, the skirt's foot; right: the same
					for (int32 sd = 0; sd < 2; ++sd)
					{
						const int32 ko = sd == 0 ? 0 : GP.Num() - 1;
						const float Sg = sd == 0 ? -1.f : 1.f;
						const float Xo = FMath::Clamp((float)GP[ko].X, -ReachL[i], ReachR[i]);
						const float Zo = Xo == (float)GP[ko].X ? (float)GP[ko].Y : HAt(Xo);
						float Xs = Xo, Zs = Zo;
						if (Xo == (float)GP[ko].X)
						{
							Xs = Xo + Sg * SkirtOut;
							Zs = FMath::Max(FloorZ - 100.f - (float)F.Ground.Z, Zo - SkirtDrop);
							for (int32 t = 1; t <= 3 && CVarFTClear.GetValueOnAnyThread(); ++t)
							{
								const float u = t / 3.f;
								if (GroundHitsRoad(F.Ground + F.GRight * FMath::Lerp(Xo, Xs, u) + FVector(0, 0, FMath::Lerp(Zo, Zs, u)), i - 30, i + 30)) { Xs = Xo; Zs = Zo; break; }
							}
						}
						const FVector Nrm = Xs == Xo ? FVector::UpVector : (FVector::UpVector - F.GRight * ((Zs - Zo) / (Xs - Xo))).GetSafeNormal();
						Sk.Add(Bed.Add(F.Ground + F.GRight * Xo + FVector::UpVector * Zo, Nrm, FVector2D(Xo / 2000.f, V), GC[ko]));
						Sk.Add(Bed.Add(F.Ground + F.GRight * Xs + FVector::UpVector * Zs, Nrm, FVector2D(Xs / 2000.f, V), GC[ko]));
					}
					if (i > c0 && PrevSkirt.Num() == 4)
					{
						Bed.Quad(PrevSkirt[1], PrevSkirt[0], Sk[1], Sk[0], FVector::UpVector);
						Bed.Quad(PrevSkirt[2], PrevSkirt[3], Sk[2], Sk[3], FVector::UpVector);
						BedTris += 4;
					}
					PrevSkirt = Sk;
				}
				// 3.1: the bed, under and right beside the road (see BedLine above)
				{
					TArray<FVector2D> Line;
					BedLine(i, Line);
					// 3.4: where this ring's last straight run up to each of the land's inner edges starts (a cutting's foot,
					// inside a banked bend). Only remembered, for the dressing's Bank rule: the bed is drawn as it was.
					if (Line.Num() >= 3 && BedCut.IsValidIndex(i)) BedCut[i] = FVector4f((float)Line[1].X, (float)Line[1].Y, (float)Line[Line.Num() - 2].X, (float)Line[Line.Num() - 2].Y);
					if (BedProbe.IsValidIndex(i)) BedProbe[i] = Line;
					TArray<int32> Bd; TArray<float> BdX;
					for (int32 k = 0; k < Line.Num(); ++k)
					{
						const FVector2D& Q = Line[k];
						const float t = FMath::Clamp((float)((Q.X - InL.X) / (InR.X - InL.X)), 0.f, 1.f);
						// a cut's sides get their own slope's normal: they catch the light as banks, and the cel pass inks
						// the lip where a bank meets the level land
						const FVector2D& Q0 = Line[FMath::Max(k - 1, 0)]; const FVector2D& Q1 = Line[FMath::Min(k + 1, Line.Num() - 1)];
						const FVector Nrm = (FVector::UpVector - F.GRight * ((Q1.Y - Q0.Y) / FMath::Max(Q1.X - Q0.X, 1.0))).GetSafeNormal();
						Bd.Add(Bed.Add(F.Ground + F.GRight * Q.X + FVector::UpVector * Q.Y, Nrm, FVector2D(Q.X / 2000.f, V), FMath::Lerp(GC[kIn], GC[kIn + 1], t)));
						BdX.Add((float)Q.X);
					}
					if (i > c0 && PrevBed.Num() > 1)
					{
						// the two rings rarely have the same number of points: zip them, left to right
						int32 a = 0, b = 0;
						while (a + 1 < PrevBed.Num() || b + 1 < Bd.Num())
						{
							const bool bA = b + 1 >= Bd.Num() || (a + 1 < PrevBed.Num() && PrevBedX[a + 1] <= BdX[b + 1]);
							if (bA) { Bed.Tri(PrevBed[a], PrevBed[a + 1], Bd[b], FVector::UpVector); ++a; }
							else { Bed.Tri(PrevBed[a], Bd[b + 1], Bd[b], FVector::UpVector); ++b; }
							++BedTris;
						}
					}
					PrevBed = Bd; PrevBedX = BdX;
				}
				if (WaterX != 0.f && MWater)
				{
					TArray<int32> Wa;
					const float Zs = GP[0].Y + 120.f;
					// 2.7: the sea stops where the land is cut short too (none at all if that is inside the beach)
					const float SeaOut = FMath::Min(ReachL[i], SeaFar), SeaIn = FMath::Min(-WaterX, SeaOut);
					Wa.Add(Water.Add(F.Ground + F.GRight * -SeaOut + FVector(0, 0, Zs), FVector::UpVector, FVector2D(-SeaOut / 2000.f, V)));
					Wa.Add(Water.Add(F.Ground + F.GRight * -SeaIn + FVector(0, 0, Zs), FVector::UpVector, FVector2D(-SeaIn / 2000.f, V)));
					if (i > c0) Water.Quad(Prev[6][0], Prev[6][1], Wa[0], Wa[1], FVector::UpVector);
					Prev[6] = Wa;
				}
			}
		}

		// 3.0 light build (attract mode): collision only on the chunks around the shot; the rest is looks only
		const bool bColl = LightAt < 0.f || (c1 * Step >= LightAt - LightCollBack && c0 * Step <= LightAt + LightCollAhead);
		const double TC0 = FPlatformTime::Seconds();
		UProceduralMeshComponent* PM = NewObject<UProceduralMeshComponent>(this, NAME_None, RF_Transient);
		PM->SetupAttachment(RootComponent);
		// the road under the start must be solid before the car drops onto it; the rest cooks in the background (above)
		PM->bUseAsyncCooking = bAsyncOk && !(c1 * Step >= SyncFrom && c0 * Step <= SyncTo);
		AsyncChunks += PM->bUseAsyncCooking ? 1 : 0;
		PM->bUseComplexAsSimpleCollision = true;
		PM->SetCollisionProfileName(TEXT("BlockAll"));
		PM->SetMobility(EComponentMobility::Static);
		PM->SetCanEverAffectNavigation(false); // no AI navigation here; the race runs in track space
		// 3.4 perf: registered only once its twelve sections are in (below). Registered first, every CreateMeshSection
		// rebuilt the chunk's render proxy and physics state again.
		const TArray<FProcMeshTangent> NoT;
		DeckFlags.SetNumZeroed(Deck.V.Num());
		const TArray<FVector2D> NoUV;
		for (const FBuf* Sb : { &Deck, &Wall, &Fence, &Ground })
		{
			SolidCrc = FCrc::MemCrc32(Sb->V.GetData(), Sb->V.Num() * sizeof(FVector), SolidCrc);
			SolidCrc = FCrc::MemCrc32(Sb->I.GetData(), Sb->I.Num() * sizeof(int32), SolidCrc);
			SolidTris += Sb->I.Num() / 3;
		}
		// 3.1 (sherpa-man) / 3.3: the engine re-cooks a component's whole collision on every CreateMeshSection call (twelve times a
		// chunk here, synchronously). The sections are made without it; the four that are solid get it switched on
		// afterwards and it is cooked once (below). Same collision, a fraction of the level's build time.
		PM->CreateMeshSection_LinearColor(0, Deck.V, Deck.I, Deck.N, Deck.UV, DeckFlags, NoUV, NoUV, Deck.C, NoT, false);
		PM->CreateMeshSection_LinearColor(1, Wall.V, Wall.I, Wall.N, Wall.UV, Wall.C, NoT, false);
		PM->CreateMeshSection_LinearColor(2, NeonB.V, NeonB.I, NeonB.N, NeonB.UV, NeonB.C, NoT, false);
		PM->CreateMeshSection_LinearColor(3, Fence.V, Fence.I, Fence.N, Fence.UV, Fence.C, NoT, false);
		PM->SetMeshSectionVisible(3, false);
		PM->CreateMeshSection_LinearColor(4, Glass.V, Glass.I, Glass.N, Glass.UV, Glass.C, NoT, false);
		PM->CreateMeshSection_LinearColor(5, Rib.V, Rib.I, Rib.N, Rib.UV, Rib.C, NoT, false);
		PM->CreateMeshSection_LinearColor(6, Ground.V, Ground.I, Ground.N, Ground.UV, Ground.C, NoT, false);
		PM->CreateMeshSection_LinearColor(7, Water.V, Water.I, Water.N, Water.UV, Water.C, NoT, false);
		PM->CreateMeshSection_LinearColor(8, Bldg.V, Bldg.I, Bldg.N, Bldg.UV, Bldg.C, NoT, false);
		PM->CreateMeshSection_LinearColor(9, Lobby.V, Lobby.I, Lobby.N, Lobby.UV, Lobby.C, NoT, false);
		PM->CreateMeshSection_LinearColor(10, Lava.V, Lava.I, Lava.N, Lava.UV, Lava.C, NoT, false);
		PM->CreateMeshSection_LinearColor(11, Bed.V, Bed.I, Bed.N, Bed.UV, Bed.C, NoT, false); // 3.1: looks only
		PM->SetMaterial(11, MGround);
		PM->SetMaterial(0, MRoad); PM->SetMaterial(1, MWall); PM->SetMaterial(2, MNeon);
		PM->SetMaterial(4, MGlass); PM->SetMaterial(5, MRib); PM->SetMaterial(6, MGround); PM->SetMaterial(7, MWater);
		PM->SetMaterial(8, MBldg); PM->SetMaterial(9, MWall); PM->SetMaterial(10, MLava);
		PM->SetCastShadow(false); // the road and land only receive shadows (keeps Virtual Shadow Map work low)
		const double TCook = FPlatformTime::Seconds();
		if (bColl)
		{
			for (const int32 Solid : { 0, 1, 3, 6 }) if (FProcMeshSection* Sec = PM->GetProcMeshSection(Solid)) Sec->bEnableCollision = true;
			PM->ClearCollisionConvexMeshes(); // there are none: this is the component's one public way to cook its collision
		}
		else PM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		const double TReg = FPlatformTime::Seconds();
		CookMs += (TReg - TCook) * 1000.0;
		PM->RegisterComponent();
		RegisterMs += (FPlatformTime::Seconds() - TReg) * 1000.0;
		Chunks.Add(PM);
		MeshCreateMs += (FPlatformTime::Seconds() - TC0) * 1000.0;
	}
	UE_LOG(LogFT, Display, TEXT("FTCOOK chunks=%d async=%d start=%.0fm window=%.0f..%.0fm"), Chunks.Num(), AsyncChunks, StartD / 100.f, SyncFrom / 100.f, SyncTo / 100.f);

	// ---- 3.1: the floor. Past the land's edges (and wherever the land is cut short of another part of the road)
	// there was no ground at all: below the horizon you saw the sky dome. One sheet under the whole level, below its
	// lowest land, out past where any sky begins, so that everything under the horizon is the world. Bayshore's is
	// its sea (MI_BaySea, made by ft_world.py); the others take the colour of their land's outer edge.
	{
		UMaterialInterface* MFloor = nullptr;
		if (LevelIndex == 0) MFloor = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/Maps/Bayshore/Materials/MI_BaySea.MI_BaySea"), nullptr, LOAD_NoWarn | LOAD_Quiet));
		const bool bSea = MFloor != nullptr;
		if (!MFloor) MFloor = MGround;
		const FVector2D Cn = Footprint.GetCenter(), Ex = Footprint.GetExtent() + FVector2D(FloorHalf, FloorHalf);
		const FLinearColor Col = bSea ? FLinearColor::White : FMath::Lerp(GC[0], GC.Last(), 0.5f);
		FBuf Fl;
		const int32 A = Fl.Add(FVector(Cn.X - Ex.X, Cn.Y - Ex.Y, FloorZ), FVector::UpVector, FVector2D(0, 0), Col);
		const int32 B = Fl.Add(FVector(Cn.X + Ex.X, Cn.Y - Ex.Y, FloorZ), FVector::UpVector, FVector2D(1, 0), Col);
		const int32 C = Fl.Add(FVector(Cn.X - Ex.X, Cn.Y + Ex.Y, FloorZ), FVector::UpVector, FVector2D(0, 1), Col);
		const int32 D = Fl.Add(FVector(Cn.X + Ex.X, Cn.Y + Ex.Y, FloorZ), FVector::UpVector, FVector2D(1, 1), Col);
		Fl.Quad(A, B, C, D, FVector::UpVector);
		UProceduralMeshComponent* PM = NewObject<UProceduralMeshComponent>(this, NAME_None, RF_Transient);
		PM->SetupAttachment(RootComponent);
		PM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PM->SetMobility(EComponentMobility::Static);
		PM->SetCanEverAffectNavigation(false);
		PM->RegisterComponent();
		PM->CreateMeshSection_LinearColor(0, Fl.V, Fl.I, Fl.N, Fl.UV, Fl.C, TArray<FProcMeshTangent>(), false);
		PM->SetMaterial(0, MFloor);
		PM->SetCastShadow(false);
		PM->bAffectDistanceFieldLighting = false;
		PM->bAffectDynamicIndirectLighting = false;
		Chunks.Add(PM);
		UE_LOG(LogFT, Display, TEXT("FTBED level=%d bed=%d triangles floor z=%.0f (lowest land %.0f) %s"), LevelIndex, BedTris, FloorZ, MinZ, bSea ? TEXT("sea") : TEXT("land colour"));
		UE_LOG(LogFT, Display, TEXT("FTTRACK level=%d frames=%d route=%08x solid=%08x solidTriangles=%d"), LevelIndex, Frames.Num(), RouteCrc, SolidCrc, SolidTris);
#if !UE_BUILD_SHIPPING
		if (FParse::Param(FCommandLine::Get(), TEXT("ftlocatecheck"))) LocateCheck();
#endif
		// The two CRCs are of the raw numbers, so they differ between the editor's binary and a packaged game: the same
		// code compiled another way rounds its last bits differently (seen 15 Nov: every level's CRC differed in the
		// packaged build while frames and triangles matched). This line is the one to compare across them: the route's
		// length, its box, the mean of its frames and of their up vectors, to a tenth of a centimetre.
		{
			double Len = 0.0; FVector Sum = FVector::ZeroVector, UpSum = FVector::ZeroVector; FBox Bx(ForceInit);
			for (int32 i = 0; i < Frames.Num(); ++i)
			{
				if (i) Len += FVector::Dist(Frames[i].Pos, Frames[i - 1].Pos);
				Sum += Frames[i].Pos; UpSum += Frames[i].Up; Bx += Frames[i].Pos;
			}
			const double Nf = FMath::Max(1, Frames.Num());
			UE_LOG(LogFT, Display, TEXT("FTTRACKSUM level=%d length=%.1f min=(%.1f,%.1f,%.1f) max=(%.1f,%.1f,%.1f) mean=(%.1f,%.1f,%.1f) up=(%.4f,%.4f,%.4f)"),
				LevelIndex, Len, Bx.Min.X, Bx.Min.Y, Bx.Min.Z, Bx.Max.X, Bx.Max.Y, Bx.Max.Z, Sum.X / Nf, Sum.Y / Nf, Sum.Z / Nf, UpSum.X / Nf, UpSum.Y / Nf, UpSum.Z / Nf);
		}

		// Under the Sea: the surface, seen from below, well above the highest road (MI_AbyssSurface, ft_world.py). Looks only.
		if (UMaterialInterface* MTop = LevelIndex == 6 ? Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/Maps/Abyss/Materials/MI_AbyssSurface.MI_AbyssSurface"), nullptr, LOAD_NoWarn | LOAD_Quiet)) : nullptr)
		{
			float MaxZ = -TNumericLimits<float>::Max();
			for (const FFTFrame& Fr : Frames) MaxZ = FMath::Max(MaxZ, (float)Fr.Pos.Z);
			const float TopZ = MaxZ + 7000.f;
			FBuf Tp;
			const int32 A2 = Tp.Add(FVector(Cn.X - Ex.X, Cn.Y - Ex.Y, TopZ), -FVector::UpVector, FVector2D(0, 0));
			const int32 B2 = Tp.Add(FVector(Cn.X + Ex.X, Cn.Y - Ex.Y, TopZ), -FVector::UpVector, FVector2D(1, 0));
			const int32 C2 = Tp.Add(FVector(Cn.X - Ex.X, Cn.Y + Ex.Y, TopZ), -FVector::UpVector, FVector2D(0, 1));
			const int32 D2 = Tp.Add(FVector(Cn.X + Ex.X, Cn.Y + Ex.Y, TopZ), -FVector::UpVector, FVector2D(1, 1));
			Tp.Quad(A2, B2, C2, D2, -FVector::UpVector);
			UProceduralMeshComponent* PT = NewObject<UProceduralMeshComponent>(this, NAME_None, RF_Transient);
			PT->SetupAttachment(RootComponent);
			PT->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PT->SetMobility(EComponentMobility::Static);
			PT->SetCanEverAffectNavigation(false);
			PT->RegisterComponent();
			PT->CreateMeshSection_LinearColor(0, Tp.V, Tp.I, Tp.N, Tp.UV, Tp.C, TArray<FProcMeshTangent>(), false);
			PT->SetMaterial(0, MTop);
			PT->SetCastShadow(false);
			PT->bAffectDistanceFieldLighting = false;
			PT->bAffectDynamicIndirectLighting = false;
			Chunks.Add(PT);
		}
	}
}

// ------------------------------------------------------------------------------------------ props
static TArray<FFTPropRule> DefaultProps(int32 Level)
{
	TArray<FFTPropRule> R;
	auto P = [&R](const FString& Mesh, float X0, float X1, int32 Every, int32 Phase, float Chance, bool bOnRoad = false, int32 Light = 0, float Scale = 1.f, float Jit = 0.f, bool bBoth = false)
	{
		FFTPropRule Q; Q.Mesh = Mesh; Q.XMin = X0; Q.XMax = X1; Q.Every = Every; Q.Phase = Phase; Q.Chance = Chance;
		Q.bOnRoad = bOnRoad; Q.Light = Light; Q.Scale = Scale; Q.ScaleJitter = Jit; Q.bBothSides = bBoth;
		R.Add(Q);
	};
	if (Level == 0)
	{
		const FString D = TEXT("/Game/Maps/Bayshore/Props/");
		P(D + TEXT("SM_Lamp"), -153, -153, 2, 0, 1.f, true, 1);
		P(D + TEXT("SM_LampR"), 153, 153, 2, 1, 1.f, true, 1);
		P(D + TEXT("SM_Umbrella2"), -1100, -500, 2, 1, 0.5f);
		P(D + TEXT("SM_Lifeguard2"), -1250, -1250, 7, 6, 1.f);
		P(D + TEXT("SM_Skyline"), 4200, 6500, 5, 2, 1.f, false, 0, 1.6f, 0.2f);
		P(D + TEXT("SM_Palm"), -900, -300, 1, 0, 0.75f, false, 0, 1.f, 0.25f);
		P(D + TEXT("SM_Palm2"), 300, 800, 1, 0, 0.6f, false, 0, 1.f, 0.25f);
		P(D + TEXT("SM_Umbrella"), -1150, -450, 1, 0, 0.5f);
		P(D + TEXT("SM_Lifeguard"), -1250, -1250, 7, 3, 1.f);
		P(D + TEXT("SM_Billboard"), 1050, 1300, 11, 5, 1.f);
		P(D + TEXT("SM_HotelPink"), 1100, 1700, 4, 0, 0.7f);
		P(D + TEXT("SM_HotelMint"), 1100, 1700, 4, 1, 0.7f);
		P(D + TEXT("SM_HotelLav"), 1100, 1700, 4, 2, 0.7f);
		P(D + TEXT("SM_HotelPeach"), 1100, 1700, 4, 3, 0.7f);
		P(D + TEXT("SM_HotelPink"), 2600, 3400, 3, 1, 0.8f, false, 0, 1.3f, 0.3f);
		P(D + TEXT("SM_HotelMint"), 2600, 3400, 3, 2, 0.8f, false, 0, 1.3f, 0.3f);
		P(D + TEXT("SM_Ferris"), -2500, -2500, 23, 7, 1.f);
		P(D + TEXT("SM_Lighthouse"), -2700, -2700, 29, 13, 1.f);
	}
	else if (Level == 1)
	{
		const FString D = TEXT("/Game/Maps/ChemicalWorks/Props/");
		P(D + TEXT("SM_Clamp"), -153, -153, 2, 0, 1.f, true, 1);
		P(D + TEXT("SM_ClampR"), 153, 153, 2, 1, 1.f, true, 1);
		P(D + TEXT("SM_Skyline"), 3800, 6000, 5, 0, 1.f, false, 0, 1.5f, 0.2f, true);
		P(D + TEXT("SM_Tank"), -1400, -600, 1, 0, 0.55f, false, 0, 1.f, 0.2f);
		P(D + TEXT("SM_Tank2"), 600, 1400, 1, 0, 0.45f, false, 0, 1.f, 0.2f);
		P(D + TEXT("SM_Pipework"), 400, 900, 2, 1, 0.7f);
		P(D + TEXT("SM_Vat"), -1300, -500, 3, 2, 0.8f);
		P(D + TEXT("SM_Sphere"), -1600, -1600, 5, 0, 1.f);
		P(D + TEXT("SM_Tower"), 2200, 3600, 3, 0, 0.8f, false, 0, 1.f, 0.2f);
		P(D + TEXT("SM_Stack"), -3800, -2400, 4, 1, 0.9f, false, 0, 1.f, 0.25f);
	}
	else if (Level == 3)
	{
		// MOUNT KALDERA: jagged black rock, lava vents, dead trees and torches on the barriers
		const FString D = TEXT("/Game/Maps/Volcano/Props/");
		P(D + TEXT("SM_Torch"), -153, -153, 2, 0, 1.f, true, 1);
		P(D + TEXT("SM_TorchR"), 153, 153, 2, 1, 1.f, true, 1);
		P(D + TEXT("SM_Spike"), -1500, -260, 1, 0, 0.7f, false, 0, 1.f, 0.45f);
		P(D + TEXT("SM_Spike2"), 260, 1500, 1, 0, 0.7f, false, 0, 1.f, 0.45f);
		P(D + TEXT("SM_Boulder"), -900, -250, 1, 0, 0.45f, false, 0, 1.f, 0.4f, true);
		P(D + TEXT("SM_DeadTree"), 300, 1100, 2, 1, 0.6f, false, 0, 1.f, 0.3f, true);
		P(D + TEXT("SM_Vent"), 700, 1600, 5, 2, 0.9f, false, 1, 1.f, 0.2f, true);
		P(D + TEXT("SM_Crag"), -4600, -2600, 3, 0, 0.85f, false, 0, 1.f, 0.4f, true);
	}
	else if (Level == 4)
	{
		// COASTER COVE: bulb posts, tents, a carousel, the big wheel, balloon stands, a drop tower, coaster lattice
		const FString D = TEXT("/Game/Maps/ThemePark/Props/");
		P(D + TEXT("SM_Bulbs"), -153, -153, 2, 0, 1.f, true, 1);
		P(D + TEXT("SM_BulbsR"), 153, 153, 2, 1, 1.f, true, 1);
		P(D + TEXT("SM_Tent"), -1500, -500, 3, 0, 0.8f, false, 0, 1.f, 0.25f);
		P(D + TEXT("SM_Carousel"), 500, 1400, 5, 2, 0.9f, false, 1);
		P(D + TEXT("SM_Booth"), -480, -330, 4, 1, 0.8f, false, 0, 1.f, 0.f, true);
		P(D + TEXT("SM_Balloons"), 260, 700, 2, 1, 0.6f, false, 0, 1.f, 0.3f, true);
		P(D + TEXT("SM_Tree"), -1300, -300, 1, 0, 0.5f, false, 0, 1.f, 0.3f, true);
		P(TEXT("/Game/Maps/Bayshore/Props/SM_Ferris"), 2400, 2400, 13, 4, 1.f);
		P(D + TEXT("SM_DropTower"), -2600, -2600, 11, 7, 1.f, false, 1);
		P(D + TEXT("SM_Lattice"), 2600, 4200, 4, 0, 0.8f, false, 0, 1.f, 0.25f, true);
	}
	else if (Level == 5)
	{
		// FROSTBITE PEAK: slalom gates on the barriers, snowy pines, ski-lift pylons, chalets, ridges
		const FString D = TEXT("/Game/Maps/Glacier/Props/");
		P(D + TEXT("SM_Gate"), -153, -153, 2, 0, 1.f, true, 1);
		P(D + TEXT("SM_GateR"), 153, 153, 2, 1, 1.f, true, 1);
		P(D + TEXT("SM_Pine"), -1600, -260, 1, 0, 0.8f, false, 0, 1.f, 0.35f);
		P(D + TEXT("SM_Pine2"), 260, 1600, 1, 0, 0.8f, false, 0, 1.f, 0.35f);
		P(D + TEXT("SM_SnowRock"), -1100, -400, 1, 0, 0.35f, false, 0, 1.f, 0.5f, true);
		P(D + TEXT("SM_Lift"), 900, 900, 3, 1, 1.f);
		P(D + TEXT("SM_Chalet"), -1300, -900, 7, 3, 1.f, false, 1);
		P(D + TEXT("SM_Ridge"), -4800, -2800, 3, 0, 0.9f, false, 0, 1.f, 0.4f, true);
	}
	else if (Level == 6)
	{
		// THE DEEP: glowing buoys on the barriers, kelp, coral, rocks, schools of fish, a wreck and the city's domes
		const FString D = TEXT("/Game/Maps/Abyss/Props/");
		P(D + TEXT("SM_Buoy"), -153, -153, 2, 0, 1.f, true, 1);
		P(D + TEXT("SM_BuoyR"), 153, 153, 2, 1, 1.f, true, 1);
		P(D + TEXT("SM_Kelp"), -1300, -260, 1, 0, 0.8f, false, 0, 1.f, 0.4f);
		P(D + TEXT("SM_Kelp2"), 260, 1300, 1, 0, 0.8f, false, 0, 1.f, 0.4f);
		P(D + TEXT("SM_Coral"), -900, -280, 1, 0, 0.5f, false, 0, 1.f, 0.4f, true);
		P(D + TEXT("SM_Coral2"), 280, 1000, 2, 1, 0.6f, false, 0, 1.f, 0.4f, true);
		P(D + TEXT("SM_Fishes"), -700, 700, 3, 2, 0.8f, false, 0, 1.f, 0.3f);
		P(D + TEXT("SM_Wreck"), -2600, -2600, 17, 5, 1.f);
		P(D + TEXT("SM_Dome"), 2400, 3600, 6, 3, 0.9f, false, 1, 1.f, 0.3f);
		P(D + TEXT("SM_SeaRock"), -4600, -2400, 3, 0, 0.85f, false, 0, 1.f, 0.4f, true);
		// 3.1, the living sea (ft_world.py; looks only, all of it): schools that swim their rounds beside the road,
		// a whale and a ray far out, shafts of light coming down, strings of bubbles by the barrier, and the wreck
		// brought near the road every so often with the light on it. After the level's own rules, which keep their places.
		const FString L = TEXT("/Game/Maps/Abyss/Life/");
		P(L + TEXT("SM_FishSchoolA"), 300, 900, 2, 0, 0.75f, false, 0, 1.f, 0.3f, true);
		P(L + TEXT("SM_FishSchoolB"), 500, 1400, 3, 1, 0.7f, false, 0, 1.f, 0.3f, true);
		P(L + TEXT("SM_Whale"), 1300, 1900, 19, 7, 1.f, false, 0, 1.4f, 0.15f);
		P(L + TEXT("SM_Manta"), -1400, -800, 11, 3, 1.f, false, 0, 1.f, 0.2f);
		P(L + TEXT("SM_LightShaft"), 330, 1400, 2, 1, 0.65f, false, 0, 1.f, 0.35f, true);
		P(L + TEXT("SM_Bubbles"), 180, 300, 3, 2, 0.8f, false, 0, 1.f, 0.25f, true);
		P(D + TEXT("SM_Wreck"), -1150, -1150, 17, 13, 1.f, false, 0, 1.5f);
		P(L + TEXT("SM_LightShaftBig"), -1150, -1150, 17, 13, 1.f);
	}
	else if (Level == 7)
	{
		// MIDTOWN: street lamps on the barriers and towers shoulder to shoulder
		const FString D = TEXT("/Game/Maps/Metro/Props/");
		P(D + TEXT("SM_Street"), -153, -153, 2, 0, 1.f, true, 1);
		P(D + TEXT("SM_StreetR"), 153, 153, 2, 1, 1.f, true, 1);
		P(D + TEXT("SM_Tower1"), -460, -360, 1, 0, 1.f, false, 0, 1.f, 0.15f);
		P(D + TEXT("SM_Tower2"), 360, 460, 1, 0, 1.f, false, 0, 1.f, 0.15f);
		P(D + TEXT("SM_Tower3"), -700, -560, 2, 1, 0.9f, false, 0, 1.2f, 0.25f, true);
		P(D + TEXT("SM_Tower4"), 560, 900, 2, 0, 0.9f, false, 0, 1.3f, 0.3f, true);
		P(D + TEXT("SM_Hydrant"), -175, -175, 5, 3, 1.f);
		P(D + TEXT("SM_Newsstand"), 180, 180, 7, 2, 1.f, false, 1);
		P(TEXT("/Game/Maps/Bayshore/Props/SM_Billboard"), 330, 330, 9, 5, 1.f);
		P(D + TEXT("SM_Skyline"), 1800, 4000, 3, 0, 1.f, false, 0, 1.4f, 0.3f, true);
	}
	else
	{
		const FString D = TEXT("/Game/Maps/Nevada/Props/");
		P(D + TEXT("SM_Pole"), -155, -155, 2, 0, 1.f, true);
		P(D + TEXT("SM_PoleR"), 155, 155, 2, 1, 1.f, true);
		P(D + TEXT("SM_Joshua"), -1500, -260, 1, 0, 0.6f, false, 0, 1.f, 0.3f);
		P(D + TEXT("SM_Joshua2"), 260, 1500, 1, 0, 0.6f, false, 0, 1.f, 0.3f);
		P(D + TEXT("SM_Cactus"), -900, -250, 1, 0, 0.4f, false, 0, 1.f, 0.3f, true);
		P(D + TEXT("SM_Rock"), 300, 1800, 1, 0, 0.45f, false, 0, 1.f, 0.5f);
		P(D + TEXT("SM_Rock2"), -1800, -300, 1, 0, 0.45f, false, 0, 1.f, 0.5f);
		P(D + TEXT("SM_Motel"), 650, 650, 13, 0, 1.f, false, 1);
		P(D + TEXT("SM_Gas"), -650, -650, 13, 6, 1.f, false, 1);
		P(D + TEXT("SM_Pylon"), 420, 420, 9, 4, 1.f, false, 1);
		P(D + TEXT("SM_Butte"), -4200, -2600, 3, 0, 0.8f, false, 0, 1.f, 0.4f, true);
	}
	FTLife::ExtraProps(Level, R); // 3.1: the living world's additions, after the level's own rules
	return R;
}

// ------------------------------------------------------------------------------------------ adverts
/** Bayshore's printed billboards (SourceArt/BayshoreBillboards, built by Content/Python/ft_billboards.py). */
static TArray<FFTAdRule> DefaultAds(int32 Level)
{
	TArray<FFTAdRule> R;
	if (Level != 0) return R;
	const FString D = TEXT("/Game/Maps/Bayshore/Billboards/Meshes/SM_BB_");
	auto A = [&](const TCHAR* Stem, EFTAdKind Kind, uint8 Where, bool bNear, EFTAdBeat Beat = EFTAdBeat::Any, uint8 Landmark = 0)
	{
		FFTAdRule Q; Q.Mesh = D + Stem; Q.Kind = Kind; Q.Where = Where; Q.bNear = bNear; Q.Beat = Beat; Q.Landmark = Landmark;
		R.Add(Q);
	};
	using namespace FTAdWhere;
	// Set02: painted at 1774 px per sign, so these take the near band
	A(TEXT("BillableBill"), EFTAdKind::Lawyer, Strip, true);
	A(TEXT("LastDime"), EFTAdKind::Casino, Strip | Marina, true, EFTAdBeat::Any, Marina);
	A(TEXT("CaptainLiability"), EFTAdKind::Food, Pier, true, EFTAdBeat::LawsuitThreat);
	A(TEXT("AlibiInn"), EFTAdKind::Motel, Strip | Marina, true);
	// Set01: about 580 px of source per sign, read from the far band
	A(TEXT("LenFender"), EFTAdKind::Lawyer, Strip, false);
	A(TEXT("DickSettle"), EFTAdKind::Lawyer, Marina, false);
	A(TEXT("RearEndRick"), EFTAdKind::Lawyer, 0, false, EFTAdBeat::CrashGag);
	A(TEXT("LoosePelican"), EFTAdKind::Casino, Marina, false);
	A(TEXT("SecondMortgage"), EFTAdKind::Casino, Strip, false);
	A(TEXT("SinkingFund"), EFTAdKind::Casino, Strip | Marina, false);
	A(TEXT("CaptainShucks"), EFTAdKind::Food, Pier, false);
	A(TEXT("BurnNotice"), EFTAdKind::Food, Pier | Strip, false);
	A(TEXT("BigDicks"), EFTAdKind::Food, Pier | Strip, false, EFTAdBeat::Any, Pier);
	return R;
}

void AFTTrack::PlanAds(const TArray<FFTPropRule>& Rules, const TArray<FFTAdRule>& AdRules, float SlotLen, TArray<FAdSpot>& Out) const
{
	Out.Reset();
	const int32 Slots = FMath::FloorToInt(Length() / SlotLen);
	if (Slots < 6 || AdRules.Num() == 0) return;

	// The other roadside landmarks: the old sunset billboard keeps its slots, the Ferris wheel marks the pier and the
	// lighthouse the marina (both stand on the sea side).
	auto RuleAt = [&](const TCHAR* Suffix, int32 s)
	{
		if (s < 0) return false;
		for (const FFTPropRule& Q : Rules) if (Q.Mesh.EndsWith(Suffix) && ((s + Q.Phase) % FMath::Max(1, Q.Every)) == 0) return true;
		return false;
	};
	auto RuleNear = [&](const TCHAR* Suffix, int32 s, int32 R)
	{
		for (int32 d = -R; d <= R; ++d) if (RuleAt(Suffix, s + d)) return true;
		return false;
	};
	auto District = [&](int32 s) -> uint8
	{
		if (RuleNear(TEXT("/SM_Ferris"), s, 3)) return FTAdWhere::Pier;
		if (RuleNear(TEXT("/SM_Lighthouse"), s, 3)) return FTAdWhere::Marina;
		return FTAdWhere::Strip;
	};

	// A slot can carry a sign when the stretch it is read from and its own stretch hold no set piece, jump cue or
	// warning paint, and the road is not bending hard (the sign would face away from the camera). 2.5's half pipes,
	// coasters and tower blocks are set pieces too.
	const uint16 Bad = FTFlag::Loop | FTFlag::Gap | FTFlag::Pipe | FTFlag::Twist | FTFlag::Kicker | FTFlag::Landing | FTFlag::Warn
		| FTFlag::HalfPipe | FTFlag::Coaster | FTFlag::Building;
	TArray<bool> Clear; Clear.Init(false, Slots);
	for (int32 s = 1; s < Slots - 1; ++s)
	{
		const float D = s * SlotLen;
		bool bOk = FMath::Abs(Sample(D).Curve) < 1.f && !RuleNear(TEXT("/SM_Billboard"), s, 1);
		for (float d = D - 0.9f * SlotLen; bOk && d <= D + 0.4f * SlotLen; d += Step) if (FlagsAt(d) & Bad) bOk = false;
		Clear[s] = bOk;
	}
	auto Calm = [&](int32 s) { return FMath::Abs(Sample(s * SlotLen).Curve) < 0.5f && FMath::Abs(Sample((s - 1) * SlotLen).Curve) < 0.5f; };

	TArray<int32> Taken; Taken.Init(-1, Slots);
	TArray<int32> LastUsed; LastUsed.Init(-100000, AdRules.Num());
	auto Free = [&](int32 s, int32 Gap)
	{
		for (int32 d = -Gap; d <= Gap; ++d) if (Taken.IsValidIndex(s + d) && Taken[s + d] >= 0) return false;
		return true;
	};
	// least recently shown design first, so the designs rotate instead of clumping
	auto Pick = [&](TFunctionRef<bool(const FFTAdRule&)> Ok) -> int32
	{
		int32 Best = -1;
		for (int32 r = 0; r < AdRules.Num(); ++r) if (Ok(AdRules[r]) && (Best < 0 || LastUsed[r] < LastUsed[Best])) Best = r;
		return Best;
	};
	auto Place = [&](int32 s, int32 r, float Scale) -> bool
	{
		const FFTAdRule& A = AdRules[r];
		const uint8 Dist = District(s);
		// pier and marina signs stand on the waterfront (left), the rest along the hotel side (right)
		const float Side = Dist == FTAdWhere::Strip ? 1.f : -1.f;
		FRandomStream Rs(s * 7919 + r * 104729 + LevelIndex * 31 + 977);
		const float Xu = (A.bNear ? 480.f : 760.f) + Rs.FRandRange(-30.f, 30.f);
		const float X = Side * Xu * FT::XU;
		const float D = (s + Rs.FRandRange(-0.12f, 0.12f)) * SlotLen;
		const FFTFrame G = Sample(D);
		FVector Pos = G.Ground + G.GRight * X;
		Pos.Z += GroundZ(X) - 20.f; // footings sink 20 cm into the land band it stands on
		const float HalfW = 640.f * Scale; // half the 12 m face plus its frame
		// clear of every stretch of road: loops, overlapping sections, the far side of bends (2.5: a coaster's split
		// rails and a tower block's facade reach up to ~21 m past the barrier, so those frames keep 22 m more clear)
		const float MinD = FT::Edge + HalfW + 400.f;
		for (int32 i = 0; i < Frames.Num(); i += 2)
		{
			const float R = (Frames[i].Flags & (FTFlag::Coaster | FTFlag::Building)) ? MinD + 2200.f : MinD;
			if (FVector2D::DistSquared(FVector2D(Pos.X, Pos.Y), FVector2D(Frames[i].Pos.X, Frames[i].Pos.Y)) < R * R) return false;
		}
		// turn the face toward the road ~150 m back, where the chase camera first reads it
		const FFTFrame V = Sample(FMath::Max(0.f, D - 1.8f * SlotLen));
		const FVector Dir = FVector(Pos.X - V.Pos.X, Pos.Y - V.Pos.Y, 0.f).GetSafeNormal();
		const float RoadYaw = FVector(G.Fwd.X, G.Fwd.Y, 0.f).GetSafeNormal().Rotation().Yaw;
		const float Turn = FMath::Clamp(FRotator::NormalizeAxis(Dir.Rotation().Yaw - RoadYaw), -40.f, 40.f);
		FAdSpot Sp;
		Sp.Xf = FTransform(FRotator(0.f, RoadYaw + Turn, 0.f), Pos, FVector(Scale));
		Sp.Centre = FVector2D(Pos.X, Pos.Y);
		Sp.Radius = HalfW + 450.f;
		Sp.Rule = r; Sp.Slot = s;
		Out.Add(Sp);
		Taken[s] = r; LastUsed[r] = s;
		return true;
	};
	auto PlaceFirst = [&](int32 From, int32 Within, bool bCalm, int32 r, float Scale) -> int32
	{
		if (r < 0) return -1;
		for (int32 k = FMath::Max(1, From); k < FMath::Min(Slots - 1, From + Within); ++k)
		{
			if (Clear[k] && Free(k, 1) && (!bCalm || Calm(k)) && Place(k, r, Scale)) return k;
		}
		return -1;
	};

	// 1. Short stunt sequences round each jump: an injury lawyer on the run-up (before the warning paint), Rear-End
	//    Rick on a calm run-out after every other landing, a casino farther on for the settlement, and on alternate
	//    jumps the seafood bar promising the next lawsuit.
	struct FJump { float Kick, Land; };
	TArray<FJump> Jumps;
	for (int32 i = 1; i < Frames.Num(); ++i)
	{
		if ((Frames[i].Flags & FTFlag::Kicker) && !(Frames[i - 1].Flags & FTFlag::Kicker)) Jumps.Add({ i * Step, i * Step });
		if (Jumps.Num() && (Frames[i - 1].Flags & FTFlag::Landing) && !(Frames[i].Flags & FTFlag::Landing)) Jumps.Last().Land = i * Step;
	}
	for (int32 j = 0; j < Jumps.Num(); ++j)
	{
		const int32 sKick = FMath::FloorToInt(Jumps[j].Kick / SlotLen);
		const int32 sLand = FMath::CeilToInt(Jumps[j].Land / SlotLen);
		const int32 Lawyer = Pick([](const FFTAdRule& A) { return A.Kind == EFTAdKind::Lawyer && A.Beat == EFTAdBeat::Any; });
		for (int32 s = sKick - 1; Lawyer >= 0 && s >= FMath::Max(1, sKick - 4); --s)
		{
			if (Clear[s] && Free(s, 1) && Place(s, Lawyer, 1.f)) break;
		}
		int32 Cursor = sLand;
		if ((j % 2) == 0)
		{
			const int32 k = PlaceFirst(Cursor, 3, true, Pick([](const FFTAdRule& A) { return A.Beat == EFTAdBeat::CrashGag; }), 1.f);
			if (k >= 0) Cursor = k + 2;
		}
		const int32 kc = PlaceFirst(Cursor, 5, false, Pick([](const FFTAdRule& A) { return A.Kind == EFTAdKind::Casino; }), 1.f);
		if (kc >= 0) Cursor = kc + 2;
		if ((j % 2) == 1) PlaceFirst(Cursor, 6, false, Pick([](const FFTAdRule& A) { return A.Beat == EFTAdBeat::LawsuitThreat; }), 1.f);
	}

	// 2. Landmarks: every third pier and marina gets its big sign (1.4x) beside the Ferris wheel or the lighthouse.
	for (const TPair<uint8, const TCHAR*>& L : { TPair<uint8, const TCHAR*>(FTAdWhere::Pier, TEXT("/SM_Ferris")), TPair<uint8, const TCHAR*>(FTAdWhere::Marina, TEXT("/SM_Lighthouse")) })
	{
		const int32 r = Pick([&L](const FFTAdRule& A) { return A.Landmark == L.Key; });
		if (r < 0) continue;
		int32 Seen = 0;
		for (int32 s = 1; s < Slots - 1; ++s)
		{
			if (!RuleAt(L.Value, s)) continue;
			if ((Seen++ % 3) != 0) continue;
			for (int32 d : { 0, 1, -1, 2 })
			{
				const int32 k = s + d;
				if (Clear.IsValidIndex(k) && Clear[k] && Free(k, 2) && Place(k, r, 1.4f)) break;
			}
		}
	}

	// 3. Fill: the district's designs in rotation, with at least five empty slots (~225 m) either side, so a
	//    headline gets its own glance.
	const int32 FillGap = 5;
	for (int32 s = 2; s < Slots - 1; ++s)
	{
		if (!Clear[s] || !Free(s, FillGap)) continue;
		const uint8 Dist = District(s);
		const int32 r = Pick([Dist](const FFTAdRule& A) { return (A.Where & Dist) != 0; });
		if (r >= 0) Place(s, r, 1.f);
	}

	Out.Sort([](const FAdSpot& A, const FAdSpot& B) { return A.Slot < B.Slot; });
	uint32 H = 2166136261u;
	TSet<int32> Designs;
	for (const FAdSpot& A : Out) { H = (H ^ uint32(A.Slot * 31 + A.Rule)) * 16777619u; Designs.Add(A.Rule); }
	UE_LOG(LogFT, Log, TEXT("FTADS level %d: %d billboards, %d designs, %d jumps, layout %08x"), LevelIndex, Out.Num(), Designs.Num(), Jumps.Num(), H);
}

void AFTTrack::BuildProps()
{
	for (UHierarchicalInstancedStaticMeshComponent* C : PropComps) if (C) C->DestroyComponent();
	for (UStaticMeshComponent* C : LifeComps) if (C) C->DestroyComponent();
	LifeComps.Reset();
	PropComps.Reset();
	if (Pillars) { Pillars->DestroyComponent(); Pillars = nullptr; }
	LampSpots.Reset();
	if (Frames.Num() < 2) return;

	const TArray<FFTPropRule> Rules = Props.Num() ? Props : DefaultProps(LevelIndex);
	int32 WpoCutComps = 0; // 4.1: components whose WPO stops at ft.PropWPODist
	TMap<FString, UHierarchicalInstancedStaticMeshComponent*> ByMesh;
	auto Comp = [&](const FString& Path, float MaxScale) -> UHierarchicalInstancedStaticMeshComponent*
	{
		if (UHierarchicalInstancedStaticMeshComponent** F = ByMesh.Find(Path)) return *F;
		UStaticMesh* M = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *Path, nullptr, LOAD_NoWarn | LOAD_Quiet));
		if (!M) { ByMesh.Add(Path, nullptr); return nullptr; }
		UHierarchicalInstancedStaticMeshComponent* C = NewObject<UHierarchicalInstancedStaticMeshComponent>(this, NAME_None, RF_Transient);
		C->SetupAttachment(RootComponent);
		C->SetStaticMesh(M);
		C->SetMobility(EComponentMobility::Static);
		C->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 2.7: small things stop drawing far away (they were drawn, and shadowed, the whole 25 km of the level);
		// the big scenery (skylines, crags, ridges, the destination) always shows
		const float R = M->GetBounds().SphereRadius * MaxScale;
		const float CullEnd = R > 4000.f ? 0.f : FMath::Clamp(R * 120.f, 60000.f, 400000.f);
		C->SetCullDistances(CullEnd * 0.85f, CullEnd);
		// 3.1: scenery that moves by its material (ft_life.py, ft_world.py: swaying fronds, turning wheels, swimming
		// fish) keeps its cached shadow: the shadow stays where the mesh was built, which nobody can tell at speed,
		// instead of every moving prop redrawing its shadow pages each frame
		C->ShadowCacheInvalidationBehavior = EShadowCacheInvalidationBehavior::Static;
		// 4.1: the population and the roadside life (M_People, M_Life) stop moving out past ft.PropWPODist, as their
		// masters were written for (180 m; their M_PeopleFar / M_LifeFar twins ignore the distance and keep moving).
		// Only those two: the other moving materials (fish, whales, drifting cloud, the eruption) stand far out on
		// purpose and have no far twin, so they keep 4.0's behaviour.
		bool bRoadsideLife = false;
		for (const FStaticMaterial& SMat : M->GetStaticMaterials())
		{
			const UMaterial* Base = SMat.MaterialInterface ? SMat.MaterialInterface->GetMaterial() : nullptr;
			if (Base && (Base->GetFName() == TEXT("M_People") || Base->GetFName() == TEXT("M_Life"))) { bRoadsideLife = true; break; }
		}
		if (bRoadsideLife)
		{
			C->WorldPositionOffsetDisableDistance = FMath::Max(0, CVarFTPropWPODist.GetValueOnGameThread());
			if (CVarFTPropWPOVel.GetValueOnGameThread() == 0) C->bWorldPositionOffsetWritesVelocity = false;
			++WpoCutComps;
		}
		C->RegisterComponent();
		PropComps.Add(C);
		ByMesh.Add(Path, C);
		return C;
	};
	// 2.7: every prop's footprint is tested against the whole road. If it only touches the road it was placed
	// beside (its own barrier) it is pushed out; if it lands on some other part of the road it is left out.
	struct FDrop { int32 Kept = 0, Moved = 0, Dropped = 0; };
	TMap<FString, FDrop> Report;
	// 3.2, Sight (dressed pieces only): the sight line. Whatever stands more than 2 m over a road keeps Sight cm from
	// that road's edge as well, pushed out or left out by the same rules. With 24 m from the centre line, the inside of
	// the tightest bend (128 m radius) still shows 150 m of road ahead past the scenery: a chord that long bulges
	// 21 m inside the centre line. Low things (fences, hedges, parked cars) may stand right behind the barrier.
	auto Clear = [&](UStaticMesh* M, FVector& Pos, const FRotator& Rot, float Sc, const FVector& Out, int32 Own, bool bOnRoad, FDrop& Rp, float Sight = 0.f) -> bool
	{
		if (!M) return false;
		if (!CVarFTClear.GetValueOnAnyThread()) { ++Rp.Kept; return true; }
		const FBoxSphereBounds Bd = M->GetBounds();
		const FQuat Q = Rot.Quaternion();
		for (int32 Try = 0; Try < 4; ++Try)
		{
			const FTransform T(Q, Pos, FVector(Sc));
			const FBox Wb = FBox(Bd.Origin - Bd.BoxExtent, Bd.Origin + Bd.BoxExtent).TransformBy(T);
			const FVector Ctr = T.TransformPosition(Bd.Origin);
			// lamps on the barrier hang over their own road: only other roads count for them
			const int32 SkipLo = bOnRoad ? Own - 30 : INDEX_NONE, SkipHi = bOnRoad ? Own + 30 : INDEX_NONE;
			float Depth = 0.f;
			int32 Hit = RoadHitBox(Ctr, Q, Bd.BoxExtent * Sc, 150.f, Wb.Min.Z, Wb.Max.Z, SkipLo, SkipHi, &Depth);
			// the top is lowered by 2 m and the deck's depth, so a road counts only if the piece stands 2 m over it
			if (Hit == INDEX_NONE && Sight > 0.f) Hit = RoadHitBox(Ctr, Q, Bd.BoxExtent * Sc, Sight, Wb.Min.Z, Wb.Max.Z - 200.f - DeckDepth - 60.f, SkipLo, SkipHi, &Depth);
			if (Hit == INDEX_NONE) { if (Try) ++Rp.Moved; ++Rp.Kept; return true; }
			if (bOnRoad || FMath::Abs(Hit - Own) > 40 || Out.IsNearlyZero()) break;
			Pos += Out * (Depth + 150.f);
		}
		++Rp.Dropped;
		return false;
	};

	const float SlotLen = 330.f * FT::DU;
	const int32 Slots = FMath::FloorToInt(Length() / SlotLen);

	// printed billboards: planned first so palms and umbrellas keep out of their footprint
	TArray<FFTAdRule> AdRules;
	TArray<FAdSpot> AdSpots;
	int32 CmdAds = 1;
	FParse::Value(FCommandLine::Get(), TEXT("ftads="), CmdAds);
	if (bAds && CmdAds != 0 && CVarFTAds.GetValueOnGameThread() != 0)
	{
		// skip designs not built yet. The component is made here, so 2.7's distance cull is sized now, for the largest
		// scale PlanAds uses (the 1.4x landmark signs): a big sign never stops drawing early
		for (const FFTAdRule& A : (Ads.Num() ? Ads : DefaultAds(LevelIndex))) if (Comp(A.Mesh, 1.4f)) AdRules.Add(A);
		PlanAds(Rules, AdRules, SlotLen, AdSpots);
	}
	auto KeptOut = [&AdSpots](const FVector& P)
	{
		for (const FAdSpot& A : AdSpots) if (FVector2D::DistSquared(FVector2D(P.X, P.Y), A.Centre) < A.Radius * A.Radius) return true;
		return false;
	};

	for (int32 s = 1; s < Slots; ++s)
	{
		const float D = s * SlotLen;
		const FFTFrame F = Sample(D);
		if (F.Flags & (FTFlag::Loop | FTFlag::Gap | FTFlag::Pipe | FTFlag::Twist)) continue;
		for (int32 r = 0; r < Rules.Num(); ++r)
		{
			const FFTPropRule& Q = Rules[r];
			if (((s + Q.Phase) % FMath::Max(1, Q.Every)) != 0) continue;
			FRandomStream Rs(s * 7919 + r * 104729 + LevelIndex * 31);
			if (Rs.FRand() > Q.Chance) continue;
			UHierarchicalInstancedStaticMeshComponent* C = Comp(Q.Mesh, Q.Scale * (1.f + Q.ScaleJitter));
			const int32 Sides = Q.bBothSides ? 2 : 1;
			for (int32 sd = 0; sd < Sides; ++sd)
			{
				float Xu = FMath::Lerp(Q.XMin, Q.XMax, Rs.FRand());
				if (sd == 1) Xu = -Xu;
				const float X = Xu * FT::XU;
				const float Dj = D + (Rs.FRand() - 0.5f) * SlotLen * 0.5f;
				const FFTFrame G = Sample(Dj);
				// 2.5: nothing on a coaster's barrier (it has none), nothing growing through a tower block
				if (Q.bOnRoad && (G.Flags & FTFlag::Coaster)) continue;
				if (!Q.bOnRoad && (G.Flags & FTFlag::Building) && FMath::Abs(X) < FT::Edge + 2200.f) continue;
				FVector Pos; FRotator Rot; FVector Out = FVector::ZeroVector;
				if (Q.bOnRoad)
				{
					// mounted on the barrier, arm over the road (the model's left/right variant knows which way)
					Pos = G.Pos + G.Right * X + G.Up * (FT::WallH + SurfaceH(Dj, FMath::Sign(X) * FT::Edge));
					Rot = FRotationMatrix::MakeFromXZ(G.Fwd, G.Up).Rotator();
				}
				else
				{
					// 3.1: where the land falls away from the road (a beach running down to the sea) the prop goes down
					// with it: it used to keep the roadside height and stand in the air. Where the land rises it stays as
					// it was (the crags and ridges were composed half buried).
					Pos = G.Ground + G.GRight * X + FVector(0, 0, FMath::Min(-60.f, GroundZ(X) + 10.f));
					Rot = FVector(G.Fwd.X, G.Fwd.Y, 0.f).GetSafeNormal().Rotation();
					Rot.Yaw += Q.bFaceRoad ? 0.f : Rs.FRandRange(0.f, 360.f);
					if (KeptOut(Pos)) continue;           // clear of the printed billboards
					Out = G.GRight * FMath::Sign(X);      // 2.7: the way out if it touches its own barrier
				}
				const float Sc = Q.Scale * (1.f + (Rs.FRand() - 0.5f) * 2.f * Q.ScaleJitter);
				if (!C) continue;
				// 3.0 light build: small things only within a few km of the shot (big scenery always, it is seen from far)
				if (LightAt >= 0.f && (Dj < LightAt - LightPropBack || Dj > LightAt + LightPropAhead) && C->GetStaticMesh()->GetBounds().SphereRadius * Sc <= 4000.f) continue;
				if (!Clear(C->GetStaticMesh(), Pos, Rot, Sc, Out, FMath::RoundToInt(Dj / Step), Q.bOnRoad, Report.FindOrAdd(FPaths::GetBaseFilename(Q.Mesh)))) continue;
				C->AddInstance(FTransform(Rot, Pos, FVector(Sc)), false);
				if (Q.Light) LampSpots.Add(Q.bOnRoad ? (G.Pos + G.Right * (X * 0.8f) + G.Up * 780.f) : (Pos + FVector(0, 0, 600.f)));
			}
		}
	}

	// ---- 3.2: the dressing (FTDress.h): districts along the route, sets of pieces round landmarks, and the horizon.
	// After the level's own props, so those keep their places. Every piece goes through the same clearance test.
	// ft.Dress 0, or -ftnodress on the command line, leaves it out: the same build gives a before and an after.
	if (CVarFTDress.GetValueOnAnyThread() != 0 && !FParse::Param(FCommandLine::Get(), TEXT("ftnodress")))
	{
		FFTDress Dr;
		FTDress::Build(LevelIndex, Dr);
		TMap<FString, const FFTDressSet*> SetBy;
		for (const FFTDressSet& S : Dr.Sets) SetBy.Add(S.Name, &S);
		int32 Placed = 0, Tried = 0, RuleIdx = 0;
		TSet<UHierarchicalInstancedStaticMeshComponent*> DressOnly; // components the dressing made (not the level's own props)
		TSet<UHierarchicalInstancedStaticMeshComponent*> DressIn;   // every component it put a piece in (for the count in the log)
		const bool bShadowRule = CVarFTDressShadows.GetValueOnAnyThread() != 0;
		const float SightClear = 2400.f - (FT::Edge + WallW); // tall pieces: 24 m from the centre line of an ordinary road
		auto Put = [&](const FString& Mesh, FVector Pos, float YawDeg, float Sc, const FVector& OutDir, float Dj, bool bFar, bool bLight) -> bool
		{
			++Tried;
			UHierarchicalInstancedStaticMeshComponent* C = Comp(Mesh, Sc * 1.3f);
			if (!C) return false;
			if (LightAt >= 0.f && (Dj < LightAt - LightPropBack || Dj > LightAt + LightPropAhead) && C->GetStaticMesh()->GetBounds().SphereRadius * Sc <= 4000.f) return false;
			if (!bFar && KeptOut(Pos)) return false;
			if (bFar && C->GetInstanceCount() == 0) C->SetCastShadow(false); // the horizon throws no shadow on the road
			// 3.3: what a piece's shadow is worth. Every piece used to cast into the virtual shadow maps as non-Nanite
			// geometry, and dense Bayshore overflowed their marking queue (the engine's own warning in the smoke run). A
			// piece casts only where its shadow can reach the road and be seen there: within 70 m of the road, and over
			// 2.5 m tall. Flat things (streets, towels, yards), small things, and everything in the middle distance and
			// beyond cast nothing. A mesh used both near and far casts: the component is one to a mesh. A component the
			// level's own props made first (the same mesh in both) keeps what it had.
			DressIn.Add(C);
			if (!bFar && bShadowRule)
			{
				const FFTFrame Gf = Sample(Dj);
				const bool bCast = FMath::Abs(FVector::DotProduct(Pos - Gf.Ground, Gf.GRight)) < 7000.f
					&& C->GetStaticMesh()->GetBounds().BoxExtent.Z * 2.f * Sc > 250.f;
				if (C->GetInstanceCount() == 0) C->SetCastShadow(bCast);
				else if (bCast && !C->CastShadow && DressOnly.Contains(C)) C->SetCastShadow(true);
				if (C->GetInstanceCount() == 0) DressOnly.Add(C);
			}
			const FRotator Rot(0.f, YawDeg, 0.f);
			if (!Clear(C->GetStaticMesh(), Pos, Rot, Sc, OutDir, FMath::RoundToInt(Dj / Step), false, Report.FindOrAdd(FPaths::GetBaseFilename(Mesh)), SightClear)) return false;
			C->AddInstance(FTransform(Rot, Pos, FVector(Sc)), false);
			if (bLight) LampSpots.Add(Pos + FVector(0, 0, 600.f));
			++Placed;
			return true;
		};
		// a dressed piece stands on the land where it is put, sunk a hand's breadth: up the trench walls and the snow slopes,
		// down the beach (the level's own props keep their old rule: never above the roadside's height)
		auto LandAt = [this](float Xcm) { return GroundZ(Xcm) - 15.f; };
		// 3.4, Bank (FTDress.h): panels on the cutting inside a banked bend. The cutting is read from the bed itself
		// (BedCut: the line the bed was drawn with), so a panel lies on the mesh that is there, at any bank and grade.
		// A panel is one flat piece: its plane goes through the cutting at the panel's middle, its long side runs with
		// the land's edge (the top of the cutting: level, and parallel to the road), and it stands half way towards
		// the cutting at its four corners (seen from the road the cutting is round along its length, like the outside
		// of a drum: under 11 m of panel it falls away about 10 cm at the ends).
		// Through a bend's lead-in and lead-out the cutting is not a rectangle: its top stays at the land's edge while
		// its foot sinks as the bank comes on, so it is deeper at one end of a panel than at the other. The piece is a
		// rectangle, and it is kept INSIDE the cutting at both of its ends: its top edge "top" metres under the land's
		// edge all along (one level line from panel to panel), its bottom edge no lower than where the cutting starts
		// at its shallower end. That leaves a bare wedge at the foot of the deeper end, and the facing steps down
		// along the foot as the cutting deepens, like a wall's footing on falling ground.
		// Where the wedge would be over BankWedge, or the piece would stand more than BankTol off the cutting (the
		// quickest part of a lead-in, where the bank turns as it goes), two pieces of half the length are laid
		// instead: halves, quarters for a half, eighths for a quarter (BankDepth). An eighth (1.4 m of road) takes
		// whatever wedge is left, and what even an eighth cannot lie flat on is left bare and counted as "too bent"
		// in the log. A shorter piece is the panel's own design, squeezed along the road.
		// The same trim applies in the body of a bend, where the bank is constant: a flat piece is a chord of the
		// bend, so the cutting's foot stands a few cm higher at its ends than at its middle, and on a grade the
		// cross-section is not square to the land's edge (a rectangle "Usable" tall would be about 2% too tall at
		// 12 degrees). Such panels come out up to 9 cm shorter than the middle cross-section alone would give on a
		// level bend, and up to 31 cm on the Chemical Works' two climbing hairpins.
		int32 BankPlaced = 0, BankFitted = 0, BankHalved = 0, BankBent = 0, BankRoad = 0, BankShort = 0;
		float BankRun = 0.f;
		const bool bBanks = CVarFTDressBank.GetValueOnAnyThread() != 0;
		// cm off the cutting; how far a panel laps over the next, as a share of its length (a gap showed the bare bank as
		// a pale post every 11 m; lapped, a bend reads as one face. Inside the tightest bend, where the cutting's foot
		// is 3.5% longer than its middle, the lap just closes on a level road. Where that bend also climbs (the
		// Chemical Works' two hairpins, at 2.6 and 3.6 km) the road is hollow along its length and the foot longer
		// still: up to 12 cm of bank shows between two panels' bottom corners, closing 1.5 m up; 0.045 would close
		// it, with 15 cm more lap on every panel of every level); how far every other panel is lifted so that two
		// lapped faces never lie in one plane (a panel's second half half of that again, a half's second quarter a
		// quarter of it, a quarter's second eighth an eighth); and the deepest bare wedge, measured up the cutting,
		// that a piece longer than an eighth may leave at the foot of its deeper end
		constexpr float BankTol = 12.f, BankLap = 0.03f, BankLift = 1.5f, BankWedge = 80.f;
		constexpr int32 BankDepth = 3;   // a whole panel is depth 0; it may be laid as halves (1), quarters (2), eighths (3)
		auto CutAt = [this](float D, int32 Side, FVector& Foot, FVector& Top) -> bool
		{
			const float Fi = FMath::Clamp(D / Step, 0.f, float(Frames.Num() - 1));
			const int32 i0 = FMath::FloorToInt(Fi), i1 = FMath::Min(i0 + 1, Frames.Num() - 1);
			if (!BedCut.IsValidIndex(i0) || !BedCut.IsValidIndex(i1)) return false;
			FVector Ft[2], Tp[2];
			for (int32 k = 0; k < 2; ++k)
			{
				const int32 i = k ? i1 : i0;
				const FFTFrame& Fr = Frames[i];
				const FVector4f& Q = BedCut[i];
				const FVector2D& In = Side > 0 ? BedInR : BedInL;
				Ft[k] = Fr.Ground + Fr.GRight * (Side > 0 ? Q.Z : Q.X) + FVector::UpVector * (Side > 0 ? Q.W : Q.Y);
				Tp[k] = Fr.Ground + Fr.GRight * In.X + FVector::UpVector * In.Y;
			}
			Foot = FMath::Lerp(Ft[0], Ft[1], Fi - i0); Top = FMath::Lerp(Tp[0], Tp[1], Fi - i0);
			return true;
		};
		auto LayBank = [&](const FFTDressRule& Q, int32 s0, int32 s1, int32 Idx)
		{
			if (!bBanks || !BedCut.Num()) return;
			UHierarchicalInstancedStaticMeshComponent* C = Comp(Q.Mesh, 1.5f);
			if (!C) return;
			const FBoxSphereBounds Bd = C->GetStaticMesh()->GetBounds();
			if (Bd.BoxExtent.X < 10.f || Bd.BoxExtent.Y < 10.f) return;
			const int32 Per = FMath::Clamp(Q.Per, 1, 12), Every = FMath::Max(1, Q.Every);
			const float PLen = SlotLen / Per;
			const uint16 Skip = FTFlag::Loop | FTFlag::Gap | FTFlag::Pipe | FTFlag::Twist | FTFlag::Kicker | FTFlag::Landing | FTFlag::HalfPipe | FTFlag::Coaster | FTFlag::Building;
			FDrop& Rp = Report.FindOrAdd(FPaths::GetBaseFilename(Q.Mesh));
			// One flat piece over the road from A0 to A1, on side Sd: a whole panel (Depth 0), a half (1), a quarter (2)
			// or an eighth (3 = BankDepth). Returns 0 = nothing to lay it on here (or the rule does not want it),
			// 1 = laid, 2 = the cutting bends or tapers too much under a piece this long (the caller lays two of half
			// the length, down to eighths), 3 = it would touch another part of the road.
			auto LayOne = [&](float A0, float A1, int32 Sd, int32 Seed, float Lift, int32 Depth) -> int32
			{
				const bool bWhole = Depth == 0;
				const float Ac = (A0 + A1) * 0.5f;
				FVector Fa, Ta, Fc, Tc, Fb, Tb;
				if (!CutAt(A0, Sd, Fa, Ta) || !CutAt(Ac, Sd, Fc, Tc) || !CutAt(A1, Sd, Fb, Tb)) return 0;
				const FVector Run = Tc - Fc;
				if (Run.Z < Run.Size2D() * 0.7f) return 0;                 // no cutting here (it climbs under 35 degrees, or not at all)
				const float RunLen = Run.Size();
				const FVector Yd = Run / RunLen;
				// Where the piece starts, measured up the cutting from its foot. Under a steep bank the cutting's foot is
				// the low barrier's top corner (less the bed's 45 cm). Under a gentle one the bed goes on down under the
				// deck's outer corner, behind the barrier, where nothing is seen: the piece starts no lower than 30 cm
				// under the barrier's top.
				auto StartOf = [&](float D, const FVector& Foot, const FVector& Up) -> float
				{
					const FFTFrame G = Sample(D);
					const float BarTop = (G.Pos + G.Right * (Sd * (FT::Edge + WallW)) + G.Up * (SurfaceH(D, Sd * FT::Edge) + FT::WallH)).Z;
					return FMath::Max(Q.Foot * 100.f, (BarTop - 30.f - Foot.Z) / FMath::Max((float)Up.Z, 0.3f));
				};
				const float Start = StartOf(Ac, Fc, Yd);
				const float Usable = RunLen - Start - Q.Top * 100.f;
				if (bWhole)
				{
					// the rule's own choice (which design, and its chance) is made once, for the whole panel: a shorter
					// piece takes what its panel was given
					if (Usable < FMath::Max(Q.RunMin * 100.f, 100.f) || Usable >= Q.RunMax * 100.f) return 0;
					FRandomStream Rs(Seed * 7919 + Idx * 104729 + LevelIndex * 31 + 1543);
					if (Rs.FRand() > Q.Chance) return 0;
				}
				// A shorter piece lies only where its own middle has two fifths of the run its design was drawn for (a
				// metre at the least): just before a corkscrew the bed's last run is a 2 m strip under the land's edge,
				// and a tall design was laid there alone, squeezed to a ninth of its height.
				else if (Usable < FMath::Max(Q.RunMin * 40.f, 100.f)) return 0;
				// The piece's own frame: x along the land's edge, y up the cutting, z out of it. Its length is the
				// cutting's middle line's, measured along x.
				FVector Xd = Tb - Ta;
				if (!Xd.Normalize(1.f)) return 0;
				const float Chord = FVector::DotProduct((Fb + Tb) * 0.5f - (Fa + Ta) * 0.5f, Xd);
				if (Chord < 50.f) return 0;
				Xd *= float(Sd);                                           // turned round on the left, like every dressed piece
				const FMatrix Rm = FRotationMatrix::MakeFromXY(Xd, Yd);
				const FVector Yp = Rm.GetUnitAxis(EAxis::Y), Zd = Rm.GetUnitAxis(EAxis::Z);
				FVector O = Fc + Yd * (Start + Usable * 0.5f);
				// The cutting at the piece's two ends, in that frame (y measured from O): where it starts and where it
				// must stop under the land's edge. The piece's rectangle is the part both ends have, and the middle too.
				const FVector Ya = (Ta - Fa).GetSafeNormal(), Yb = (Tb - Fb).GetSafeNormal();
				const float Ka = FVector::DotProduct(Ya, Yp), Kb = FVector::DotProduct(Yb, Yp);
				if (Ka < 0.5f || Kb < 0.5f) return Depth < BankDepth ? 2 : 0; // at one end the cutting lies another way altogether
				const float Kc = FVector::DotProduct(Yd, Yp);               // the middle's own limits in the piece's y, like the two ends' (under 1 where the land's edge climbs with the road)
				const float FootA = FVector::DotProduct(Fa - O, Yp), FootB = FVector::DotProduct(Fb - O, Yp);
				const float LoA = FootA + StartOf(A0, Fa, Ya) * Ka, LoB = FootB + StartOf(A1, Fb, Yb) * Kb;
				const float HiA = FVector::DotProduct(Ta - Ya * (Q.Top * 100.f) - O, Yp), HiB = FVector::DotProduct(Tb - Yb * (Q.Top * 100.f) - O, Yp);
				const float Lo = FMath::Max3(LoA, LoB, -Usable * 0.5f * Kc), Hi = FMath::Min3(HiA, HiB, Usable * 0.5f * Kc);
				if (Hi - Lo < 100.f) return Depth < BankDepth ? 2 : 0;      // no room under a piece this long: a shorter one may find some
				if (Depth < BankDepth && Lo - FMath::Min(LoA, LoB) > BankWedge) return 2;   // the wedge at the deeper end's foot: shorter pieces step down closer
				// how far the cutting stands off the piece's plane at the rectangle's four corners
				float Dv[4];
				Dv[0] = FVector::DotProduct(Fa + Ya * ((Lo - FootA) / Ka) - O, Zd); Dv[1] = FVector::DotProduct(Fa + Ya * ((Hi - FootA) / Ka) - O, Zd);
				Dv[2] = FVector::DotProduct(Fb + Yb * ((Lo - FootB) / Kb) - O, Zd); Dv[3] = FVector::DotProduct(Fb + Yb * ((Hi - FootB) / Kb) - O, Zd);
				const float Half = (Dv[0] + Dv[1] + Dv[2] + Dv[3]) * 0.125f;
				float Worst = FMath::Abs(Half);
				for (const float V : Dv) Worst = FMath::Max(Worst, FMath::Abs(V - Half));
				if (Worst > BankTol) { if (Depth >= BankDepth) ++Tried; return 2; }
				O += Yp * ((Hi + Lo) * 0.5f) + Zd * (Half + Lift);
				const FVector Sc3(Chord * (1.f + BankLap) / (Bd.BoxExtent.X * 2.f), (Hi - Lo) / (Bd.BoxExtent.Y * 2.f), 1.f);
				const FQuat Qr = Rm.ToQuat();
				const FTransform T(Qr, O - Qr.RotateVector(FVector(Bd.Origin.X * Sc3.X, Bd.Origin.Y * Sc3.Y, 0.f)), Sc3);
				if (KeptOut(O)) { ++Tried; return 3; }
				if (CVarFTClear.GetValueOnAnyThread())
				{
					// the road's other parts (the route passing itself, a lap beside the last one). The panel's own road
					// is the bed it lies on: 120 m of it either way is not asked.
					const FBox Wb = Bd.GetBox().TransformBy(T);
					const int32 Own = FMath::RoundToInt(Ac / Step);
					if (RoadHitBox(Wb.GetCenter(), FQuat::Identity, Wb.GetExtent(), 150.f, Wb.Min.Z, Wb.Max.Z, Own - 60, Own + 60) != INDEX_NONE) { ++Tried; ++Rp.Dropped; return 3; }
				}
				if (C->GetInstanceCount() == 0) { C->SetCastShadow(false); DressOnly.Add(C); } // flat on a slope: no shadow worth a page
				DressIn.Add(C);
				C->AddInstance(T, false);
				++Tried; ++Placed; ++BankPlaced; ++Rp.Kept; BankRun += Chord;
				return 1;
			};
			for (int32 n = s0 * Per; n < (s1 + 1) * Per; ++n)
			{
				if (((n + Q.Phase) % Every) != 0) continue;
				const float D0 = n * PLen, D1 = D0 + PLen, Dc = D0 + PLen * 0.5f;
				if (D1 > Length() - Step) break;
				if ((FlagsAt(D0) | FlagsAt(Dc) | FlagsAt(D1)) & Skip) continue;
				if (LightAt >= 0.f && (Dc < LightAt - LightPropBack || Dc > LightAt + LightPropAhead)) continue;
				const float Up0 = (n & 1) ? BankLift : 0.f;
				for (int32 Side = -1; Side <= 1; Side += 2) // the low side has the cutting; the other side's "run" is the land, level
				{
					const int32 Res = LayOne(D0, D1, Side, n, Up0, 0);
					if (Res == 0) continue;
					++BankFitted;
					if (Res == 3) ++BankRoad;
					if (Res != 2) continue;
					// Two pieces of half the length instead, and again for a piece that still bends or tapers too much,
					// down to eighths. Every piece has a lift of its own, so that no two lapped faces lie in one plane.
					// A length is counted once under each thing that left some of it bare.
					int32 Laid = 0, Bent = 0, Road = 0, Short = 0;
					TFunction<void(float, float, int32, float)> SplitBank = [&](float PartFrom, float PartTo, int32 PartDepth, float PartLift)
					{
						const float PartMid = (PartFrom + PartTo) * 0.5f;
						for (int32 PartIdx = 0; PartIdx < 2; ++PartIdx)
						{
							const float HalfFrom = PartIdx ? PartMid : PartFrom, HalfTo = PartIdx ? PartTo : PartMid;
							const float HalfLift = PartLift + BankLift * 0.5f * PartIdx / float(1 << (PartDepth - 1));
							const int32 Got = LayOne(HalfFrom, HalfTo, Side, n, HalfLift, PartDepth);
							if (Got == 1) ++Laid;
							else if (Got == 3) ++Road;
							else if (Got == 0) ++Short;
							else if (PartDepth < BankDepth) SplitBank(HalfFrom, HalfTo, PartDepth + 1, HalfLift);
							else ++Bent;
						}
					};
					SplitBank(D0, D1, 1, Up0);
					if (Laid) ++BankHalved;
					if (Bent) ++BankBent;
					if (Road) ++BankRoad;
					if (Short) ++BankShort;
				}
			}
		};
		for (const FFTDressZone& Z : Dr.Zones)
		{
			const int32 s0 = FMath::Max(1, FMath::CeilToInt(Z.FromKm * 100000.f / SlotLen)), s1 = FMath::Min(Slots - 1, FMath::FloorToInt(Z.ToKm * 100000.f / SlotLen));
			for (const FFTDressRule& Q : Z.Rules)
			{
				++RuleIdx;
				if (Q.Kind == EFTDress::Bank) { LayBank(Q, s0, s1, RuleIdx); continue; }
				const bool bSite = Q.Kind == EFTDress::Site, bFar = Q.Kind == EFTDress::Far || Q.bFloor;
				for (int32 s = s0; s <= s1; ++s)
				{
					if (((s + Q.Phase) % FMath::Max(1, Q.Every)) != 0) continue;
					FRandomStream Rs(s * 7919 + RuleIdx * 104729 + LevelIndex * 31 + 977);
					if (Rs.FRand() > Q.Chance) continue;
					const int32 Copies = (!bSite && Q.Side == 0) ? 2 : 1;
					for (int32 c = 0; c < Copies; ++c)
					{
						const float Sd = Q.Side != 0 ? float(Q.Side) : (bSite ? (((s / FMath::Max(1, Q.Every)) % 2) ? 1.f : -1.f) : (c == 0 ? 1.f : -1.f));
						const float Dj = s * SlotLen + (bSite ? 0.f : (Rs.FRand() - 0.5f) * SlotLen * 0.5f);
						const FFTFrame G = Sample(Dj);
						// near things keep out of set pieces; the horizon stays wherever the road goes
						if (!bFar && (G.Flags & (FTFlag::Loop | FTFlag::Gap | FTFlag::Pipe | FTFlag::Twist))) continue;
						const FVector Fw = FVector(G.Fwd.X, G.Fwd.Y, 0.f).GetSafeNormal();
						if (Fw.IsNearlyZero()) continue;
						const FVector OutDir = G.GRight * Sd;
						const float Heading = Fw.Rotation().Yaw + (Sd < 0.f ? 180.f : 0.f); // turned round on the left: the front faces the road
						const float Xm = FMath::Lerp(Q.XMin, Q.XMax, Rs.FRand());
						const float Sc = Q.Scale * (1.f + (Rs.FRand() - 0.5f) * 2.f * Q.Jitter);
						const float Yj = (Rs.FRand() - 0.5f) * 2.f * Q.YawJitter;
						auto NearOk = [&G](float Xc)
						{
							if ((G.Flags & FTFlag::Building) && FMath::Abs(Xc) < FT::Edge + 2200.f) return false; // inside a tower block
							if ((G.Flags & (FTFlag::Kicker | FTFlag::Landing)) && FMath::Abs(Xc) < 4000.f) return false; // a jump's run-up and landing stay open
							return true;
						};
						if (bSite)
						{
							const FFTDressSet* const* Sp = SetBy.Find(Q.Mesh);
							if (!Sp) continue;
							const FVector Lx = Fw * Sd; // the set's own +x
							for (const FFTDressPart& Pt : (*Sp)->Parts)
							{
								const float Xc = (Xm + Pt.Out * Sc) * 100.f * Sd;
								if (!Q.bFloor && !NearOk(Xc)) continue;
								FVector Pos = G.Ground + G.GRight * Xc + Lx * (Pt.Along * Sc * 100.f) + FVector(0, 0, LandAt(Xc) + Pt.Up * Sc * 100.f);
								if (Q.bFloor) Pos.Z = FloorZ + Pt.Up * Sc * 100.f; // afloat: the floor is the level's own height, wherever the road is
								Put(Pt.Mesh, Pos, Heading + Pt.Yaw, Sc * Pt.Scale, OutDir, Dj, Q.bFloor, false);
							}
						}
						else
						{
							const float Xc = Xm * 100.f * Sd;
							if (!bFar && !NearOk(Xc)) continue;
							FVector Pos = G.Ground + G.GRight * Xc + FVector(0, 0, bFar ? -Q.Drop * 100.f : LandAt(Xc));
							if (Q.bFloor) Pos.Z = FloorZ + Q.Drop * 100.f;
							Put(Q.Mesh, Pos, Heading + Yj, Sc, OutDir, Dj, bFar, Q.bLight);
						}
					}
				}
			}
		}
		// how many of the instances in the dressing's components cast a shadow (the level's own props that share a mesh
		// with it are in the count): what the shadow rule above leaves for the shadow maps to draw
		int32 InComps = 0, Casting = 0;
		for (const UHierarchicalInstancedStaticMeshComponent* C : DressIn) { InComps += C->GetInstanceCount(); if (C->CastShadow) Casting += C->GetInstanceCount(); }
		UE_LOG(LogFT, Display, TEXT("FTDRESS level=%d districts=%d sets=%d pieces placed=%d of %d tried shadows=%d of %d"), LevelIndex, Dr.Zones.Num(), Dr.Sets.Num(), Placed, Tried, Casting, InComps);
		// the cut banks: pieces laid (a whole panel, a half, a quarter and an eighth count one each), on how many 11 m
		// lengths of cutting; how many of those lengths were laid in shorter pieces; and the lengths left bare, wholly
		// or in part, because an eighth is still too bent for a flat piece, a piece would have touched another part of
		// the road, or a shorter piece found too little cutting to lie on (where a cutting begins and ends)
		UE_LOG(LogFT, Display, TEXT("FTBANK level=%d pieces laid=%d on %d lengths of cutting (%d in shorter pieces; left bare, wholly or in part: too bent %d, at another part of the road %d, too little cutting %d) along %.0f m%s"),
			LevelIndex, BankPlaced, BankFitted, BankHalved, BankBent, BankRoad, BankShort, BankRun / 100.f, bBanks ? TEXT("") : TEXT(" (ft.DressBank 0)"));
	}

	// PlanAds already keeps every sign clear of the road, so the signs skip 2.7's road test
	for (const FAdSpot& A : AdSpots)
	{
		if (UHierarchicalInstancedStaticMeshComponent* C = Comp(AdRules[A.Rule].Mesh, 1.4f)) C->AddInstance(A.Xf, false);
	}

	// Las Vegas, lit up ahead of the Nevada highway, past the end of the road (2.5: every level can have its
	// destination on the horizon: the volcano, the mountain, the sunken city, the downtown towers)
	const FFTLevelDef& LD = FTData::Level(LevelIndex);
	if (!LD.Backdrop.IsEmpty())
	{
		if (UHierarchicalInstancedStaticMeshComponent* V = Comp(LD.Backdrop, 1000.f)) // never culled
		{
			// 2.7: each one goes where it was meant to (straight ahead of a point on the road, far out), unless the road
			// itself runs through it there: then it is tried further out and a little to either side (scaled up with
			// the distance, so it looks the same size), and left out if nowhere is clear
			FDrop& Rp = Report.FindOrAdd(FPaths::GetBaseFilename(LD.Backdrop));
			TArray<FTransform> BackdropAt; // where each one went (the volcano's eruption stands on the same spots)
			auto Place = [&](const FFTFrame& At, float Dist, float Scale, float Drop)
			{
				const FVector Fw = FVector(At.Fwd.X, At.Fwd.Y, 0.f).GetSafeNormal();
				const FBoxSphereBounds Bd = V->GetStaticMesh()->GetBounds();
				static const float Mul[] = { 1.f, 1.25f, 1.6f, 2.f, 2.5f };
				static const float Ang[] = { 0.f, 12.f, -12.f, 24.f, -24.f, 36.f, -36.f };
				for (const float M : Mul)
					for (const float A : Ang)
					{
						const FVector Dir = Fw.RotateAngleAxis(A, FVector::UpVector);
						const FVector Pos = At.Ground + Dir * Dist * M + FVector(0, 0, Drop);
						const float Sc = Scale * M;
						const FQuat Qr = (-Dir).Rotation().Quaternion();
						const FTransform T(Qr, Pos, FVector(Sc));
						const FBox Wb = FBox(Bd.Origin - Bd.BoxExtent, Bd.Origin + Bd.BoxExtent).TransformBy(T);
						if (!CVarFTClear.GetValueOnAnyThread() || RoadHitBox(T.TransformPosition(Bd.Origin), Qr, Bd.BoxExtent * Sc, 3000.f, Wb.Min.Z, Wb.Max.Z, INDEX_NONE, INDEX_NONE) == INDEX_NONE)
						{
							V->AddInstance(T, false);
							BackdropAt.Add(T);
							++Rp.Kept; if (M != 1.f || A != 0.f) ++Rp.Moved;
							return;
						}
					}
				++Rp.Dropped;
			};
			Place(FrameAt(Frames.Num() - 1), 60000.f, LD.BackdropScale, -200.f);
			Place(Sample(LD.FinaleD * FT::DU), 90000.f, LD.BackdropScale * 1.5f, -200.f);
			// and one a third of the way in, so it is on the horizon early (the new levels)
			const int32 NumEarly = BackdropAt.Num();
			if (LevelIndex >= 3) Place(Sample(LD.FinaleD * FT::DU * 0.35f), 110000.f, LD.BackdropScale * 1.8f, -400.f);
			// 3.1: Mount Kaldera erupts. The eruption is its own mesh over each cone (SM_Eruption, ft_world.py) and its
			// material plays it on a cycle: a rumble, the blast, the plume and the fountains, the aftermath. Each cone has
			// its own second in the cycle (custom primitive data 0); the one beside the early road goes first, as the
			// player comes up to it. Looks only.
			if (LevelIndex == 3)
				if (UStaticMesh* Er = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Maps/Volcano/Life/SM_Eruption.SM_Eruption"), nullptr, LOAD_NoWarn | LOAD_Quiet)))
					for (int32 k = 0; k < BackdropAt.Num(); ++k)
					{
						UStaticMeshComponent* E = NewObject<UStaticMeshComponent>(this, NAME_None, RF_Transient);
						E->SetupAttachment(RootComponent);
						E->SetStaticMesh(Er);
						E->SetMobility(EComponentMobility::Static);
						E->SetRelativeTransform(BackdropAt[k]);
						E->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						E->SetCastShadow(false);
						E->bAffectDistanceFieldLighting = false;
						E->bAffectDynamicIndirectLighting = false;
						E->SetCustomPrimitiveDataFloat(0, k >= NumEarly ? 42.f : (k == 0 ? 30.f : 9.f));
						E->RegisterComponent();
						LifeComps.Add(E);
					}
		}
	}

	// pillars under the deck where it stands above the ground
	int32 PillarsDropped = 0;
	UStaticMesh* PillarM = PillarMesh ? PillarMesh.Get() : Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder")));
	if (PillarM)
	{
		Pillars = NewObject<UHierarchicalInstancedStaticMeshComponent>(this, NAME_None, RF_Transient);
		Pillars->SetupAttachment(RootComponent);
		Pillars->SetStaticMesh(PillarM);
		Pillars->SetMobility(EComponentMobility::Static);
		Pillars->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (UMaterialInterface* MW = LoadAsset<UMaterialInterface>(TEXT("/Game/Core/Materials/MI_Pillar.MI_Pillar"))) Pillars->SetMaterial(0, MW);
		Pillars->RegisterComponent();
		for (int32 i = 10; i < Frames.Num(); i += 20)
		{
			const FFTFrame& F = Frames[i];
			if (F.Flags & (FTFlag::Pipe | FTFlag::Twist)) continue;
			if (F.Up.Z < 0.5f) continue;
			if (LightAt >= 0.f && (i * Step < LightAt - LightPropBack || i * Step > LightAt + LightPropAhead)) continue;
			// 2.5: a coaster stands on a pillar under each rail (not under the top one where they cross over)
			const bool bCo = (F.Flags & FTFlag::Coaster) != 0;
			const FFTRails Rl = RailsAt(i * Step);
			for (int32 pi = 0; pi < (bCo ? FFTRails::Num : 2); ++pi)
			{
				const float sgn = pi == 0 ? -1.f : 1.f;
				const float Xp = bCo ? Rl.X[pi] : sgn * FT::RoadHalf * 0.6f;
				const float Hp = bCo ? Rl.H[pi] : 0.f;
				// a rail passing over another one stands on nothing (the one below carries the pillar)
				bool bOver = false;
				if (bCo) for (int32 q = 0; q < FFTRails::Num; ++q) if (q != pi && FMath::Abs(Rl.X[q] - Xp) < 900.f && Hp > Rl.H[q] + 300.f) bOver = true;
				if (bOver) continue;
				const FVector Top = F.Pos + F.Right * Xp + F.Up * (Hp - DeckDepth);
				const float H = Top.Z - (F.Ground.Z - 400.f);
				if (H < 250.f) continue;
				// 2.7: not standing in (or through) some other part of the road below
				if (CVarFTClear.GetValueOnAnyThread() && RoadHitBox(FVector(Top.X, Top.Y, Top.Z - H * 0.5f), FQuat::Identity, FVector(120.f, 120.f, H * 0.5f), 100.f, Top.Z - H, Top.Z - DeckDepth - 10.f, i - 40, i + 40) != INDEX_NONE)
				{
					++PillarsDropped;
					continue;
				}
				Pillars->AddInstance(FTransform(FRotator::ZeroRotator, FVector(Top.X, Top.Y, Top.Z - H * 0.5f), FVector(2.4f, 2.4f, H / 100.f)), false);
			}
		}
	}
	for (UHierarchicalInstancedStaticMeshComponent* C : PropComps) C->BuildTreeIfOutdated(false, true);
	UE_LOG(LogFT, Display, TEXT("FTWPO level=%d life components=%d of %d stop moving past %d cm (velocity %d)"), LevelIndex, WpoCutComps, PropComps.Num(), CVarFTPropWPODist.GetValueOnGameThread(), CVarFTPropWPOVel.GetValueOnGameThread());
	if (Pillars) Pillars->BuildTreeIfOutdated(false, true);

	FDrop Tot;
	FString Lines;
	for (const TPair<FString, FDrop>& P : Report)
	{
		Tot.Kept += P.Value.Kept; Tot.Moved += P.Value.Moved; Tot.Dropped += P.Value.Dropped;
		if (P.Value.Moved || P.Value.Dropped) Lines += FString::Printf(TEXT(" %s:-%d/~%d/%d"), *P.Key, P.Value.Dropped, P.Value.Moved, P.Value.Kept);
	}
	UE_LOG(LogFT, Display, TEXT("FTCLEAR level=%d props kept=%d (moved off their own barrier %d) left out of the road=%d pillars left out=%d |%s"),
		LevelIndex, Tot.Kept, Tot.Moved, Tot.Dropped, PillarsDropped, *Lines);
}
