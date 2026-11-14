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
		default: break;
		}
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
		i0 = i1 + 1;
	}
	// warning stripes before set pieces
	for (int32 s = 1; s < All.Num(); ++s)
	{
		const EFTSec Sh = All[s].Shape;
		if (Sh == EFTSec::Loop || Sh == EFTSec::Jump || Sh == EFTSec::Twist || Sh == EFTSec::Pipe)
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
		else
		{
			P(-30000, 200, TEXT("a2507a")); P(-12000, -120, TEXT("c9805a")); P(-1600, -70, TEXT("d98a5e"));
			P(1600, -70, TEXT("d98a5e")); P(12000, -120, TEXT("c9805a")); P(30000, 260, TEXT("a2507a"));
		}
	}

	const int32 ChunkLen = 200;
	const FLinearColor Neon = L.Rail;
	for (int32 c0 = 0; c0 < Frames.Num() - 1; c0 += ChunkLen)
	{
		const int32 c1 = FMath::Min(c0 + ChunkLen, Frames.Num() - 1);
		FBuf Deck, Wall, NeonB, Fence, Glass, Rib, Ground, Water;
		TArray<FVector2D> DeckFlags; // UV1 = (warning stripes, kicker paint)
		TArray<int32> Prev[8];
		for (int32 i = c0; i <= c1; ++i)
		{
			const FFTFrame& F = Frames[i];
			const float V = i * Step / 1000.f;
			const bool bRoad = !(F.Flags & FTFlag::Gap);
			const bool bLink = i > c0 && bRoad && !(Frames[i - 1].Flags & FTFlag::Gap);
			const FLinearColor VC(F.Flags & FTFlag::Warn ? 1.f : 0.f, (F.Flags & (FTFlag::Kicker | FTFlag::Landing)) ? 1.f : 0.f, 0.f, 1.f);
			auto At = [&F](float X, float H) { return F.Pos + F.Right * X + F.Up * H; };
			const float E = FT::Edge;

			// deck: top (5 columns), bottom, two sides
			TArray<int32> R;
			const float Xs[5] = { -E, -FT::RoadHalf, 0.f, FT::RoadHalf, E };
			for (float X : Xs) R.Add(Deck.Add(At(X, 0.f), F.Up, FVector2D((X + E) / (2.f * E), V), VC));
			while (DeckFlags.Num() < Deck.V.Num() - 5) DeckFlags.Add(FVector2D::ZeroVector);
			for (int32 k = DeckFlags.Num(); k < Deck.V.Num(); ++k) DeckFlags.Add(FVector2D(VC.R, VC.G));
			R.Add(Deck.Add(At(-E, -DeckDepth), -F.Up, FVector2D(0, V))); R.Add(Deck.Add(At(E, -DeckDepth), -F.Up, FVector2D(1, V)));
			R.Add(Deck.Add(At(-E - WallW, 0.f), -F.Right, FVector2D(0, V))); R.Add(Deck.Add(At(-E - WallW, -DeckDepth), -F.Right, FVector2D(0.1f, V)));
			R.Add(Deck.Add(At(E + WallW, 0.f), F.Right, FVector2D(0, V))); R.Add(Deck.Add(At(E + WallW, -DeckDepth), F.Right, FVector2D(0.1f, V)));
			if (bLink)
			{
				const TArray<int32>& P = Prev[0];
				for (int32 k = 0; k < 4; ++k) Deck.Quad(P[k], P[k + 1], R[k], R[k + 1], F.Up);
				Deck.Quad(P[5], P[6], R[5], R[6], -F.Up);
				Deck.Quad(P[7], P[8], R[7], R[8], -F.Right);
				Deck.Quad(P[9], P[10], R[9], R[10], F.Right);
			}
			Prev[0] = R;

			// barriers: inner face, top, outer face on each side
			TArray<int32> W;
			for (int32 sgn = -1; sgn <= 1; sgn += 2)
			{
				const float Xi = sgn * E, Xo = sgn * (E + WallW);
				const FVector In = -F.Right * float(sgn);
				W.Add(Wall.Add(At(Xi, 0.f), In, FVector2D(0, V))); W.Add(Wall.Add(At(Xi, FT::WallH), In, FVector2D(1, V)));
				W.Add(Wall.Add(At(Xi, FT::WallH), F.Up, FVector2D(0, V))); W.Add(Wall.Add(At(Xo, FT::WallH), F.Up, FVector2D(1, V)));
				W.Add(Wall.Add(At(Xo, FT::WallH), -In, FVector2D(0, V))); W.Add(Wall.Add(At(Xo, -DeckDepth), -In, FVector2D(1, V)));
			}
			if (bLink)
			{
				const TArray<int32>& P = Prev[1];
				for (int32 sd = 0; sd < 2; ++sd)
				{
					const int32 o = sd * 6;
					const float sgn = sd == 0 ? -1.f : 1.f;
					Wall.Quad(P[o + 0], P[o + 1], W[o + 0], W[o + 1], -F.Right * sgn);
					Wall.Quad(P[o + 2], P[o + 3], W[o + 2], W[o + 3], F.Up);
					Wall.Quad(P[o + 4], P[o + 5], W[o + 4], W[o + 5], F.Right * sgn);
				}
			}
			Prev[1] = W;

			// neon strip along the top of each barrier, facing the road
			TArray<int32> Nn;
			for (int32 sgn = -1; sgn <= 1; sgn += 2)
			{
				const float X = sgn * (E - 2.f);
				const FVector In = -F.Right * float(sgn);
				Nn.Add(NeonB.Add(At(X, FT::WallH - 32.f), In, FVector2D(0, V), Neon));
				Nn.Add(NeonB.Add(At(X, FT::WallH - 10.f), In, FVector2D(1, V), Neon));
			}
			if (bLink)
			{
				const TArray<int32>& P = Prev[2];
				NeonB.Quad(P[0], P[1], Nn[0], Nn[1], F.Right);
				NeonB.Quad(P[2], P[3], Nn[2], Nn[3], -F.Right);
			}
			Prev[2] = Nn;

			// invisible fence above the barrier (collision only) so a car can't sail off a bend
			TArray<int32> Fe;
			for (int32 sgn = -1; sgn <= 1; sgn += 2)
			{
				const FVector In = -F.Right * float(sgn);
				Fe.Add(Fence.Add(At(sgn * E, FT::WallH), In, FVector2D(0, V))); Fe.Add(Fence.Add(At(sgn * E, 900.f), In, FVector2D(1, V)));
			}
			if (bLink && !(F.Flags & (FTFlag::Loop | FTFlag::Pipe | FTFlag::Twist)))
			{
				const TArray<int32>& P = Prev[3];
				Fence.Quad(P[0], P[1], Fe[0], Fe[1], F.Right);
				Fence.Quad(P[2], P[3], Fe[2], Fe[3], -F.Right);
			}
			Prev[3] = Fe;

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
		PM->SetMaterial(0, MRoad); PM->SetMaterial(1, MWall); PM->SetMaterial(2, MNeon);
		PM->SetMaterial(4, MGlass); PM->SetMaterial(5, MRib); PM->SetMaterial(6, MGround); PM->SetMaterial(7, MWater);
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
				FVector Pos; FRotator Rot;
				if (Q.bOnRoad)
				{
					// mounted on the barrier, arm over the road (the model's left/right variant knows which way)
					Pos = G.Pos + G.Right * X + G.Up * FT::WallH;
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

	// Las Vegas, lit up ahead of the Nevada highway, past the end of the road
	if (LevelIndex == 2)
	{
		if (UHierarchicalInstancedStaticMeshComponent* V = Comp(TEXT("/Game/Maps/Nevada/Props/SM_Vegas")))
		{
			const FFTFrame E = FrameAt(Frames.Num() - 1);
			const FVector Fw = FVector(E.Fwd.X, E.Fwd.Y, 0.f).GetSafeNormal();
			V->AddInstance(FTransform((-Fw).Rotation(), E.Ground + Fw * 60000.f + FVector(0, 0, -200.f), FVector(4.f)), false);
			const FFTFrame Fin = Sample(FTData::Level(2).FinaleD * FT::DU);
			const FVector Fw2 = FVector(Fin.Fwd.X, Fin.Fwd.Y, 0.f).GetSafeNormal();
			V->AddInstance(FTransform((-Fw2).Rotation(), Fin.Ground + Fw2 * 90000.f + FVector(0, 0, -200.f), FVector(6.f)), false);
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
			for (int32 sgn = -1; sgn <= 1; sgn += 2)
			{
				const FVector Top = F.Pos + F.Right * (sgn * FT::RoadHalf * 0.6f) - F.Up * DeckDepth;
				const float H = Top.Z - (F.Ground.Z - 400.f);
				if (H < 250.f) continue;
				Pillars->AddInstance(FTransform(FRotator::ZeroRotator, FVector(Top.X, Top.Y, Top.Z - H * 0.5f), FVector(2.4f, 2.4f, H / 100.f)), false);
			}
		}
	}
	for (UHierarchicalInstancedStaticMeshComponent* C : PropComps) C->BuildTreeIfOutdated(false, true);
	if (Pillars) Pillars->BuildTreeIfOutdated(false, true);
}
