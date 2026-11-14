#include "FTTrack.h"
#include "ProceduralMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

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
}

void AFTTrack::Rebuild()
{
	Integrate();
	BuildMeshes();
	BuildProps();
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
	R.Right = FVector::CrossProduct(R.Up, R.Fwd).GetSafeNormal();
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
	OutX = FVector::DotProduct(Dv, F.Right);
	OutH = FVector::DotProduct(Dv, F.Up);
	return FMath::Clamp(Best * Step + Along, 0.f, Length());
}

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
	R.XL = -RailX0; R.XR = RailX0;
	float U = 0.f;
	const FFTSection* S = SectionAt(D, &U);
	if (!S || S->Shape != EFTSec::Coaster) return R;
	const int32 Mode = FMath::RoundToInt(FMath::Abs(S->A));
	const float E = FMath::Square(FMath::Sin(PI * U)); // 0 at both ends: every mode meets the next side by side
	switch (Mode)
	{
	case 1: // they split: one climbs high and wide, the other dives low and wide, both waving
	{
		const float Wv = 380.f * E * FMath::Sin(6.f * PI * U);
		R.XL = -(RailX0 + 2600.f * E); R.HL = 1700.f * E + Wv;
		R.XR = RailX0 + 2600.f * E;    R.HR = -900.f * E - Wv;
		break;
	}
	case 2: // the double helix: a little apart while the frame rolls them round each other
		R.XL = -(RailX0 + 520.f * E); R.XR = RailX0 + 520.f * E;
		break;
	case 3: // the cross-over: they swap sides, one over the top and one underneath
	{
		const float Sx = Smoother(U);
		R.XL = -RailX0 * FMath::Cos(PI * Sx); R.HL = 1900.f * FMath::Sin(PI * Sx);
		R.XR = RailX0 * FMath::Cos(PI * Sx);  R.HR = -1300.f * FMath::Sin(PI * Sx);
		break;
	}
	default: break;
	}
	R.bAdjacent = FMath::Abs(R.XR - R.XL) < 2.f * RailX0 + 200.f && FMath::Abs(R.HR - R.HL) < 120.f;
	return R;
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
		const float DL = FMath::Abs(X - R.XL), DR = FMath::Abs(X - R.XR);
		const bool bLeft = DL < DR;
		if (FMath::Min(DL, DR) > FFTRails::HalfW + 40.f && bSolid) *bSolid = false;
		return bLeft ? R.HL : R.HR;
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
	TArray<FVector2D> GP = GroundProfile;
	TArray<FLinearColor> GC = GroundColours;
	float WaterX = 0.f; // L1: sea on the left beyond this x
	if (GP.Num() < 2)
	{
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

	const int32 ChunkLen = 200;
	const FLinearColor Neon = L.Rail;
	for (int32 c0 = 0; c0 < Frames.Num() - 1; c0 += ChunkLen)
	{
		const int32 c1 = FMath::Min(c0 + ChunkLen, Frames.Num() - 1);
		FBuf Deck, Wall, NeonB, Fence, Glass, Rib, Ground, Water, Bldg, Lobby, Lava;
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
					for (int32 s = 0; s < 2; ++s)
					{
						const float Xc = s == 0 ? R.XL : R.XR, Hc = s == 0 ? R.HL : R.HR;
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
					const FVector P = F.Ground + F.GRight * GP[k].X + FVector::UpVector * GP[k].Y;
					Gr.Add(Ground.Add(P, FVector::UpVector, FVector2D(GP[k].X / 2000.f, V), GC[k]));
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
				if (WaterX != 0.f && MWater)
				{
					TArray<int32> Wa;
					const float Zs = GP[0].Y + 120.f;
					Wa.Add(Water.Add(F.Ground + F.GRight * -90000.f + FVector(0, 0, Zs), FVector::UpVector, FVector2D(-45, V)));
					Wa.Add(Water.Add(F.Ground + F.GRight * WaterX + FVector(0, 0, Zs), FVector::UpVector, FVector2D(WaterX / 2000.f, V)));
					if (i > c0) Water.Quad(Prev[6][0], Prev[6][1], Wa[0], Wa[1], FVector::UpVector);
					Prev[6] = Wa;
				}
			}
		}

		UProceduralMeshComponent* PM = NewObject<UProceduralMeshComponent>(this, NAME_None, RF_Transient);
		PM->SetupAttachment(RootComponent);
		PM->bUseAsyncCooking = false; // the road must be solid before the car drops onto it
		PM->bUseComplexAsSimpleCollision = true;
		PM->SetCollisionProfileName(TEXT("BlockAll"));
		PM->SetMobility(EComponentMobility::Static);
		PM->SetCanEverAffectNavigation(false); // no AI navigation here; the race runs in track space
		PM->RegisterComponent();
		const TArray<FProcMeshTangent> NoT;
		DeckFlags.SetNumZeroed(Deck.V.Num());
		const TArray<FVector2D> NoUV;
		PM->CreateMeshSection_LinearColor(0, Deck.V, Deck.I, Deck.N, Deck.UV, DeckFlags, NoUV, NoUV, Deck.C, NoT, true);
		PM->CreateMeshSection_LinearColor(1, Wall.V, Wall.I, Wall.N, Wall.UV, Wall.C, NoT, true);
		PM->CreateMeshSection_LinearColor(2, NeonB.V, NeonB.I, NeonB.N, NeonB.UV, NeonB.C, NoT, false);
		PM->CreateMeshSection_LinearColor(3, Fence.V, Fence.I, Fence.N, Fence.UV, Fence.C, NoT, true);
		PM->SetMeshSectionVisible(3, false);
		PM->CreateMeshSection_LinearColor(4, Glass.V, Glass.I, Glass.N, Glass.UV, Glass.C, NoT, false);
		PM->CreateMeshSection_LinearColor(5, Rib.V, Rib.I, Rib.N, Rib.UV, Rib.C, NoT, false);
		PM->CreateMeshSection_LinearColor(6, Ground.V, Ground.I, Ground.N, Ground.UV, Ground.C, NoT, true);
		PM->CreateMeshSection_LinearColor(7, Water.V, Water.I, Water.N, Water.UV, Water.C, NoT, false);
		PM->CreateMeshSection_LinearColor(8, Bldg.V, Bldg.I, Bldg.N, Bldg.UV, Bldg.C, NoT, false);
		PM->CreateMeshSection_LinearColor(9, Lobby.V, Lobby.I, Lobby.N, Lobby.UV, Lobby.C, NoT, false);
		PM->CreateMeshSection_LinearColor(10, Lava.V, Lava.I, Lava.N, Lava.UV, Lava.C, NoT, false);
		PM->SetMaterial(0, MRoad); PM->SetMaterial(1, MWall); PM->SetMaterial(2, MNeon);
		PM->SetMaterial(4, MGlass); PM->SetMaterial(5, MRib); PM->SetMaterial(6, MGround); PM->SetMaterial(7, MWater);
		PM->SetMaterial(8, MBldg); PM->SetMaterial(9, MWall); PM->SetMaterial(10, MLava);
		PM->SetCastShadow(false); // the road and land only receive shadows (keeps Virtual Shadow Map work low)
		Chunks.Add(PM);
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
	return R;
}

void AFTTrack::BuildProps()
{
	for (UHierarchicalInstancedStaticMeshComponent* C : PropComps) if (C) C->DestroyComponent();
	PropComps.Reset();
	if (Pillars) { Pillars->DestroyComponent(); Pillars = nullptr; }
	LampSpots.Reset();
	if (Frames.Num() < 2) return;

	const TArray<FFTPropRule> Rules = Props.Num() ? Props : DefaultProps(LevelIndex);
	TMap<FString, UHierarchicalInstancedStaticMeshComponent*> ByMesh;
	auto Comp = [&](const FString& Path) -> UHierarchicalInstancedStaticMeshComponent*
	{
		if (UHierarchicalInstancedStaticMeshComponent** F = ByMesh.Find(Path)) return *F;
		UStaticMesh* M = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *Path, nullptr, LOAD_NoWarn | LOAD_Quiet));
		if (!M) { ByMesh.Add(Path, nullptr); return nullptr; }
		UHierarchicalInstancedStaticMeshComponent* C = NewObject<UHierarchicalInstancedStaticMeshComponent>(this, NAME_None, RF_Transient);
		C->SetupAttachment(RootComponent);
		C->SetStaticMesh(M);
		C->SetMobility(EComponentMobility::Static);
		C->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		C->SetCullDistances(0, 0);
		C->RegisterComponent();
		PropComps.Add(C);
		ByMesh.Add(Path, C);
		return C;
	};

	const float SlotLen = 330.f * FT::DU;
	const int32 Slots = FMath::FloorToInt(Length() / SlotLen);
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
			UHierarchicalInstancedStaticMeshComponent* C = Comp(Q.Mesh);
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
				FVector Pos; FRotator Rot;
				if (Q.bOnRoad)
				{
					// mounted on the barrier, arm over the road (the model's left/right variant knows which way)
					Pos = G.Pos + G.Right * X + G.Up * (FT::WallH + SurfaceH(Dj, FMath::Sign(X) * FT::Edge));
					Rot = FRotationMatrix::MakeFromXZ(G.Fwd, G.Up).Rotator();
				}
				else
				{
					Pos = G.Ground + G.GRight * X + FVector(0, 0, -60.f);
					Rot = FVector(G.Fwd.X, G.Fwd.Y, 0.f).GetSafeNormal().Rotation();
					Rot.Yaw += Q.bFaceRoad ? 0.f : Rs.FRandRange(0.f, 360.f);
				}
				const float Sc = Q.Scale * (1.f + (Rs.FRand() - 0.5f) * 2.f * Q.ScaleJitter);
				if (C) C->AddInstance(FTransform(Rot, Pos, FVector(Sc)), false);
				if (Q.Light) LampSpots.Add(Q.bOnRoad ? (G.Pos + G.Right * (X * 0.8f) + G.Up * 780.f) : (Pos + FVector(0, 0, 600.f)));
			}
		}
	}

	// Las Vegas, lit up ahead of the Nevada highway, past the end of the road (2.5: every level can have its
	// destination on the horizon: the volcano, the mountain, the sunken city, the downtown towers)
	const FFTLevelDef& LD = FTData::Level(LevelIndex);
	if (!LD.Backdrop.IsEmpty())
	{
		if (UHierarchicalInstancedStaticMeshComponent* V = Comp(LD.Backdrop))
		{
			const FFTFrame E = FrameAt(Frames.Num() - 1);
			const FVector Fw = FVector(E.Fwd.X, E.Fwd.Y, 0.f).GetSafeNormal();
			V->AddInstance(FTransform((-Fw).Rotation(), E.Ground + Fw * 60000.f + FVector(0, 0, -200.f), FVector(LD.BackdropScale)), false);
			const FFTFrame Fin = Sample(LD.FinaleD * FT::DU);
			const FVector Fw2 = FVector(Fin.Fwd.X, Fin.Fwd.Y, 0.f).GetSafeNormal();
			V->AddInstance(FTransform((-Fw2).Rotation(), Fin.Ground + Fw2 * 90000.f + FVector(0, 0, -200.f), FVector(LD.BackdropScale * 1.5f)), false);
			// and one a third of the way in, so it is on the horizon early (the new levels)
			if (LevelIndex >= 3)
			{
			const FFTFrame Mid = Sample(LD.FinaleD * FT::DU * 0.35f);
			const FVector Fw3 = FVector(Mid.Fwd.X, Mid.Fwd.Y, 0.f).GetSafeNormal();
			V->AddInstance(FTransform((-Fw3).Rotation(), Mid.Ground + Fw3 * 110000.f + FVector(0, 0, -400.f), FVector(LD.BackdropScale * 1.8f)), false);
			}
		}
	}

	// pillars under the deck where it stands above the ground
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
			// 2.5: a coaster stands on a pillar under each rail (not under the top one where they cross over)
			const bool bCo = (F.Flags & FTFlag::Coaster) != 0;
			const FFTRails Rl = RailsAt(i * Step);
			for (int32 sgn = -1; sgn <= 1; sgn += 2)
			{
				const float Xp = bCo ? (sgn < 0 ? Rl.XL : Rl.XR) : sgn * FT::RoadHalf * 0.6f;
				const float Hp = bCo ? (sgn < 0 ? Rl.HL : Rl.HR) : 0.f;
				if (bCo && FMath::Abs(Rl.XL - Rl.XR) < 900.f && Hp > FMath::Min(Rl.HL, Rl.HR) + 300.f) continue;
				const FVector Top = F.Pos + F.Right * Xp + F.Up * (Hp - DeckDepth);
				const float H = Top.Z - (F.Ground.Z - 400.f);
				if (H < 250.f) continue;
				Pillars->AddInstance(FTransform(FRotator::ZeroRotator, FVector(Top.X, Top.Y, Top.Z - H * 0.5f), FVector(2.4f, 2.4f, H / 100.f)), false);
			}
		}
	}
	for (UHierarchicalInstancedStaticMeshComponent* C : PropComps) C->BuildTreeIfOutdated(false, true);
	if (Pillars) Pillars->BuildTreeIfOutdated(false, true);
}
