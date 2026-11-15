#include "FTFx.h"
#include "FTGameInstance.h"
#include "FTCore.h"
#include "FTEntity.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "UObject/UObjectIterator.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/WorldSettings.h"
#include "HAL/IConsoleManager.h"
#include "Misc/Paths.h"
#include "UnrealClient.h"

namespace
{
	// Every attack effect, by its asset name under /Game/FX (ft_fx.py: definitions() and attack_definitions()).
	// Warm() loads them all; the ones marked true also get a hidden component whose PSOs are submitted at level
	// setup. One per material and renderer setup is enough: the sprites of all the others compile to the same PSOs.
	struct FFxName { const TCHAR* Name; bool bWarm; };
	const FFxName AllFx[] = {
		{ TEXT("NS_Explosion"), true }, { TEXT("NS_ExplosionFlash"), true }, { TEXT("NS_ExplosionSparks"), true }, { TEXT("NS_ExplosionSmoke"), true },
		{ TEXT("NS_Impact"), false }, { TEXT("NS_Muzzle"), false }, { TEXT("NS_Fire"), false }, { TEXT("NS_MissileTrail"), true },
		{ TEXT("NS_MuzzleStar"), true }, { TEXT("NS_MuzzleStarAP"), false }, { TEXT("NS_MuzzleStarHE"), true }, { TEXT("NS_MuzzleTongue"), true },
		{ TEXT("NS_ImpactStar"), false }, { TEXT("NS_ImpactStarAP"), false }, { TEXT("NS_ImpactStarHE"), false }, { TEXT("NS_ImpactPierce"), false },
		{ TEXT("NS_ImpactArmor"), true }, { TEXT("NS_ImpactRing"), true }, { TEXT("NS_Fizzle"), true },
		{ TEXT("NS_EnemyMuzzle"), false }, { TEXT("NS_EnemyTrail"), true }, { TEXT("NS_BigTracerTrail"), false }, { TEXT("NS_HitPlayer"), false }, { TEXT("NS_HitPlayerSparks"), false },
		{ TEXT("NS_FishTrail"), true }, { TEXT("NS_FireballTrail"), true }, { TEXT("NS_PieTrail"), true },
		{ TEXT("NS_MissileFlame"), false }, { TEXT("NS_MissileLaunch"), false }, { TEXT("NS_MissileBackblast"), false }, { TEXT("NS_MissileHit"), false },
		{ TEXT("NS_FuseTrail"), false }, { TEXT("NS_SnowTrail"), false }, { TEXT("NS_LandBomb"), false }, { TEXT("NS_FireSplash"), false },
		{ TEXT("NS_ExplosionIce"), false }, { TEXT("NS_SnowBurst"), false }, { TEXT("NS_IceShards"), false }, { TEXT("NS_ExplosionWater"), false }, { TEXT("NS_Splash"), false },
		{ TEXT("NS_ExplosionZap"), false }, { TEXT("NS_ZapArcs"), false }, { TEXT("NS_FlameBurst"), false }, { TEXT("NS_SnowWall"), false },
		{ TEXT("NS_BlastCore"), false }, { TEXT("NS_BlastCore_s_blue_fire"), false }, { TEXT("NS_BlastCore_s_toxic_blast"), false },
		{ TEXT("NS_ShockRing"), false }, { TEXT("NS_ExplosionEmbers"), false },
		{ TEXT("NS_BlastRing"), false }, { TEXT("NS_BlastRingGold"), false }, { TEXT("NS_HeBurst"), false }, { TEXT("NS_ChainBurst"), false },
		{ TEXT("NS_BarrelBurst"), false }, { TEXT("NS_BarrelFire"), false }, { TEXT("NS_ChainSwing"), false }, { TEXT("NS_RamBurst"), false }, { TEXT("NS_RamSparks"), false },
		{ TEXT("NS_AlertFlare"), false }, { TEXT("NS_ChargeTrail"), false }, { TEXT("NS_RevSmoke"), false }, { TEXT("NS_WreckSmoke"), false },
		// FX overhaul (ft_fx.py: overhaul_definitions()). The toon sprites are a material of their own (M_FX_Toon), and a sprite
		// that lies on the road is a renderer setup of its own: one of each is warmed
		{ TEXT("NS_FxFireball"), true }, { TEXT("NS_FxFireball_s_blue_fire"), false }, { TEXT("NS_FxFireball_s_toxic_blast"), false },
		{ TEXT("NS_FxFireRoll"), false }, { TEXT("NS_FxFireJet"), false }, { TEXT("NS_FxFlash"), false }, { TEXT("NS_FxStreaks"), false }, { TEXT("NS_FxDebris"), true },
		{ TEXT("NS_FxSmokeRise"), true }, { TEXT("NS_FxSmokeBall"), false }, { TEXT("NS_FxSmokeColumn"), false },
		{ TEXT("NS_FxGroundRing"), true }, { TEXT("NS_FxScorch"), true }, { TEXT("NS_FxEmbers"), false },
		{ TEXT("NS_MissileSmoke"), true }, { TEXT("NS_MissileSmokeNear"), false }, { TEXT("NS_MissileIgnite"), false }, { TEXT("NS_RocketTrail"), false },
		{ TEXT("NS_TracerTrail"), false }, { TEXT("NS_TracerTrailAP"), false }, { TEXT("NS_TracerTrailHE"), false }, { TEXT("NS_RamChips"), false },
		{ TEXT("NS_DriftSparks1"), false }, { TEXT("NS_DriftSparks2"), false }, { TEXT("NS_DriftSparks3"), false },
		{ TEXT("NS_DriftPop1"), false }, { TEXT("NS_DriftPop2"), false }, { TEXT("NS_DriftPop3"), false },
		{ TEXT("NS_BoostKick"), false }, { TEXT("NS_BoostRing"), false }, { TEXT("NS_TurboCore"), false },
		{ TEXT("NS_LandDust"), false }, { TEXT("NS_LandSparks"), false }, { TEXT("NS_WreckFire"), true },
	};

	TMap<FName, UNiagaraSystem*>& Cache()
	{
		static TMap<FName, UNiagaraSystem*> Map;
		return Map;
	}

	// ---- FX overhaul: what this file has put in the world, so it can be moved, delayed and cleared
	struct FCarried { TWeakObjectPtr<UNiagaraComponent> Comp; FVector Vel = FVector::ZeroVector; float Drag = 1.8f; float Until = 0.f; };
	struct FPendingLayer { float When = 0.f; FString Name; FVector At = FVector::ZeroVector; FQuat Rot = FQuat::Identity; float Scale = 1.f; FVector Vel = FVector::ZeroVector; float Drag = 1.8f; float Hold = 0.7f; int32 Sort = 0; };
	struct FTrailState { FVector Pen = FVector::ZeroVector; float Touch = 0.f; int32 Step = 0; };
	struct FFxState
	{
		TWeakObjectPtr<UWorld> World;
		TArray<FCarried> Carried;
		TArray<FPendingLayer> Pending;
		TArray<TWeakObjectPtr<UNiagaraComponent>> Live;
		TMap<int32, FTrailState> Trails;
		TArray<float> BlastT;       // world seconds of the last blasts: a stack of them is thinned (Explosion)
		TArray<float> SmokeUntil;   // when each laid length of missile smoke ends: there is a cap on them (Trail)
		float PruneT = 0.f;
	};
	FFxState& FxRaw()
	{
		static FFxState S;
		return S;
	}
	FFxState& FxState(UWorld* W)
	{
		FFxState& S = FxRaw();
		if (S.World.Get() != W)
		{
			// a new map: everything of the last one went with its world
			S.Carried.Reset(); S.Pending.Reset(); S.Live.Reset(); S.Trails.Reset(); S.BlastT.Reset(); S.SmokeUntil.Reset(); S.PruneT = 0.f;
			S.World = W;
		}
		return S;
	}

	// Sort priorities. Translucent things draw in this order, whatever their distance: the scorch on the road, smoke,
	// fire, debris, then everything that was left at 0, which is every danger cue (the road markers and landing
	// marks, hostile streaks and muzzle stars) and the pickups' own effects. A warning is never under a blast's smoke.
	constexpr int32 SortScorch = -40, SortSmoke = -30, SortFire = -20, SortCore = -15, SortDebris = -10, SortSpark = -5;

	UNiagaraComponent* FxKeep(UNiagaraComponent* C, int32 Sort)
	{
		if (!C) return nullptr;
		// a pooled component keeps what its last use set; only touch it on a change (it re-makes the render proxy)
		if (C->TranslucencySortPriority != Sort) C->SetTranslucentSortPriority(Sort);
		if (UWorld* W = C->GetWorld())
		{
			FFxState& S = FxState(W);
			// it came out of the pool: whatever was carrying its last use has nothing to do with this one
			S.Carried.RemoveAllSwap([C](const FCarried& K) { return K.Comp.Get() == C; });
			S.Live.Add(C);
		}
		return C;
	}

	UNiagaraComponent* FxSpawn(const UObject* Ctx, const TCHAR* Name, const FVector& At, const FRotator& Rot, float Scale, int32 Sort)
	{
		UNiagaraSystem* Sys = FTFx::System(Name);
		if (!Sys) return nullptr;
		return FxKeep(UNiagaraFunctionLibrary::SpawnSystemAtLocation(Ctx, Sys, At, Rot, FVector(Scale), true, true, ENCPoolMethod::AutoRelease), Sort);
	}

#if !UE_BUILD_SHIPPING
	// ft.FxFilm <name> <frames> [step]: a frame sequence for a clip, taken by the game itself. One screenshot (with the
	// HUD) each <step> seconds of GAME time, 1/30 unless given, as <name>_00.png, _01 ... in the test runner's shots
	// folder. The frame a screenshot is taken on is a long one, so while it films the world's longest step is held to
	// <step>: no frame of the sequence jumps further than that, whatever the game's own slow motion is doing.
	struct FFxFilm { FString Name; int32 Left = 0, Index = 0; float Step = 1.f / 30.f, Acc = 0.f, WasMax = 0.4f; };
	FFxFilm GFilm;

	void TickFilm(UWorld* W, float Dt)
	{
		if (GFilm.Left <= 0) return;
		AWorldSettings* WS = W->GetWorldSettings();
		if (!WS) { GFilm.Left = 0; return; }
		WS->MaxUndilatedFrameTime = FMath::Clamp(GFilm.Step / FMath::Max(WS->GetEffectiveTimeDilation(), 0.01f), 0.004f, 0.4f);
		GFilm.Acc += Dt;
		if (GFilm.Acc < GFilm.Step - 0.0005f) return;
		GFilm.Acc = FMath::Min(GFilm.Acc - GFilm.Step, GFilm.Step * 0.5f);
		const UFTGameInstance* G = Cast<UFTGameInstance>(W->GetGameInstance());
		const FString Dir = FPaths::Combine(G ? G->SaveDir() : FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("FullThrottle")), TEXT("shots"));
		FScreenshotRequest::RequestScreenshot(FPaths::Combine(Dir, FString::Printf(TEXT("%s_%02d.png"), *GFilm.Name, GFilm.Index++)), true, false);
		if (--GFilm.Left > 0) return;
		WS->MaxUndilatedFrameTime = GFilm.WasMax;
		UE_LOG(LogFT, Display, TEXT("FT FXFILM done name=%s frames=%d"), *GFilm.Name, GFilm.Index);
	}
#endif
}

UNiagaraSystem* FTFx::System(const TCHAR* Name)
{
	const FName Key(Name);
	if (UNiagaraSystem** Found = Cache().Find(Key)) return *Found;
	const FString Path = FString::Printf(TEXT("/Game/FX/%s.%s"), Name, Name);
	UNiagaraSystem* Sys = Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), nullptr, *Path, nullptr, LOAD_NoWarn | LOAD_Quiet));
	// the map is invisible to the garbage collector: root what it holds (as the pickups' and the meshes' caches do).
	// A missing asset is remembered too, so a build without it does not hit the disk on every shot
	if (Sys) Sys->AddToRoot();
	Cache().Add(Key, Sys);
	return Sys;
}

bool FTFx::IsLow()
{
	static const IConsoleVariable* Quality = IConsoleManager::Get().FindConsoleVariable(TEXT("sg.EffectsQuality"));
	return Quality && Quality->GetInt() == 0;
}

bool FTFx::Shows(const UObject* Ctx, int32 Layer)
{
	if ((Layer & Extra) && IsLow()) return false;
	if (Layer & Flash)
	{
		const UWorld* W = Ctx ? Ctx->GetWorld() : nullptr;
		const UFTGameInstance* G = W ? Cast<UFTGameInstance>(W->GetGameInstance()) : nullptr;
		if (G && G->bReducedMotion) return false;
	}
	return true;
}

UNiagaraComponent* FTFx::Burst(const UObject* Ctx, const TCHAR* Name, const FVector& At, const FRotator& Rot, float Scale, int32 Layer)
{
	if (!Ctx || !Shows(Ctx, Layer)) return nullptr;
	return FxSpawn(Ctx, Name, At, Rot, Scale, 0);
}

UNiagaraComponent* FTFx::Attach(const TCHAR* Name, USceneComponent* To, const FVector& At, const FRotator& Rot, float Scale, int32 Layer, bool bRelative)
{
	if (!To || !Shows(To, Layer)) return nullptr;
	UNiagaraSystem* Sys = System(Name);
	if (!Sys) return nullptr;
	return FxKeep(UNiagaraFunctionLibrary::SpawnSystemAttached(Sys, To, NAME_None, At, Rot, FVector(Scale),
		bRelative ? EAttachLocation::KeepRelativeOffset : EAttachLocation::KeepWorldPosition, true, ENCPoolMethod::AutoRelease, true), 0);
}

void FTFx::Warm(AActor* Owner)
{
	if (!Owner || !Owner->GetWorld()) return;
	FxState(Owner->GetWorld());   // FX overhaul: a new map starts with nothing carried over
	int32 Loaded = 0, Warmed = 0;
	for (const FFxName& F : AllFx)
	{
		UNiagaraSystem* Sys = System(F.Name);
		if (!Sys) continue;
		++Loaded;
		if (!F.bWarm) continue;
		// as AFTEntityActor::WarmPickups: a hidden, idle component per system, so its sprite PSOs exist before a shot
		UNiagaraComponent* WarmFX = NewObject<UNiagaraComponent>(Owner);
		Owner->AddInstanceComponent(WarmFX);
		WarmFX->SetupAttachment(Owner->GetRootComponent());
		WarmFX->SetAutoActivate(false);
		WarmFX->SetHiddenInGame(true);
		WarmFX->SetAsset(Sys);
		WarmFX->RegisterComponent();
		WarmFX->PrecachePSOs();
		++Warmed;
	}
	UE_LOG(LogFT, Display, TEXT("FTFX warm loaded=%d of %d precached=%d low=%d"), Loaded, (int32)UE_ARRAY_COUNT(AllFx), Warmed, IsLow() ? 1 : 0);
}

// ------------------------------------------------------------------------------------------ FX overhaul: blasts
void FTFx::Carry(UNiagaraComponent* C, const FVector& Velocity, float Drag, float Seconds)
{
	UWorld* W = C ? C->GetWorld() : nullptr;
	if (!W || Velocity.SizeSquared() < 2500.f) return;
	FCarried& K = FxState(W).Carried.AddDefaulted_GetRef();
	K.Comp = C; K.Vel = Velocity; K.Drag = Drag; K.Until = W->GetTimeSeconds() + Seconds;
}

void FTFx::Explosion(const UObject* Ctx, const FBlast& B)
{
	UWorld* W = Ctx ? Ctx->GetWorld() : nullptr;
	if (!W) return;
	FFxState& S = FxState(W);
	const float Now = W->GetTimeSeconds();
	const float Sz = FMath::Max(0.6f, B.Size);
	// A blast right beside the camera gets nothing flat and wide (the banded core, the star): shapes that close fill
	// the screen. Its fire is smaller there too, and the toon sprites break up by themselves as the lens nears them
	const bool bNearCam = FVector::DistSquared(B.At, B.Cam) < FMath::Square(1400.f);
	// A stack: the blasts of the last 0.35 s. From the third the fire is smaller, from the fourth there is no roll and
	// no smoke, so several kills at once do not wall off the road and what is behind them. Never a boss's or the
	// KESTREL's own blast; a missile's punch through its victim (bLight) is not counted. A real reach's ring is
	// never thinned
	const bool bThin = B.Kind != EBlast::Boss && B.Kind != EBlast::Player;
	S.BlastT.RemoveAllSwap([Now](float T) { return Now - T > 0.35f || T > Now; });
	const int32 Stack = S.BlastT.Num();
	if (!(B.Kind == EBlast::Missile && B.bLight)) S.BlastT.Add(Now);
	const float NearK = (bNearCam ? 0.7f : 1.f) * (bThin && Stack >= 2 ? 0.7f : 1.f);
	const FQuat Road = B.Road.IsNormalized() ? B.Road : FQuat::Identity;
	const FVector Up = Road.GetUpVector(), Fwd = Road.GetForwardVector();
	const bool bDir = !B.Dir.IsNearlyZero();
	// the hit's frame: +X the way the hit was going, +Z as near the road's up as that allows. With no hit behind it,
	// "along the hit" is straight up off the road
	const FQuat UpQ = FRotationMatrix::MakeFromXZ(Up, Fwd).ToQuat();
	const FQuat Hit = bDir ? FRotationMatrix::MakeFromXZ(B.Dir.GetSafeNormal(), Up).ToQuat() : UpQ;
	const FString Tint = B.Tint ? FString(B.Tint) : FString();

	// one layer: which system, where (Off is in the road's frame), how much of the victim's speed it rolls on with
	auto Put = [&](const FString& Name, int32 Layer, float Scale, const FQuat& Rot, const FVector& Off, float Inherit, float Drag, int32 Sort, float Delay = 0.f, bool bOnRoad = false)
	{
		if (!Shows(Ctx, Layer)) return;
		const FVector V = B.Vel * Inherit;
		const float Hold = Drag > 2.f ? 2.2f : 0.8f;
		const FVector From = (bOnRoad ? B.Ground : B.At) + Road.RotateVector(Off);
		if (Delay <= 0.f)
		{
			if (UNiagaraComponent* C = FxSpawn(Ctx, *Name, From, Rot.Rotator(), Scale, Sort)) Carry(C, V, Drag, Hold);
			return;
		}
		// later: where the blast has rolled to by then, with the speed it has left
		FPendingLayer& P = S.Pending.AddDefaulted_GetRef();
		P.When = Now + Delay; P.Name = Name; P.Rot = Rot; P.Scale = Scale; P.Drag = Drag; P.Hold = Hold; P.Sort = Sort;
		P.At = From + V * ((1.f - FMath::Exp(-Drag * Delay)) / FMath::Max(Drag, 0.01f));
		P.Vel = V * FMath::Exp(-Drag * Delay);
	};
	const FVector FireAt(0.f, 0.f, 70.f * Sz), SmokeAt(0.f, 0.f, 110.f * Sz), OnRoad(0.f, 0.f, 7.f);
	auto FlashStar = [&](float K, const FVector& Off, float Delay = 0.f) { if (!bNearCam) Put(TEXT("NS_FxFlash"), Flash, Sz * K, Road, Off, 0.85f, 1.8f, SortSpark, Delay); };
	auto Core = [&](float K) { if (!bNearCam) Put(TEXT("NS_BlastCore") + Tint, Main, Sz * K, Road, FireAt, 0.85f, 1.8f, SortCore); };
	// (a build without the overhaul's fireball gets the stock one, standing: a blast is never without its fire)
	auto Fire = [&](float K, const FVector& Off, float Delay = 0.f)
	{
		const FString Ball = TEXT("NS_FxFireball") + Tint;
		if (System(*Ball)) Put(Ball, Main, Sz * K * NearK, Road, Off, 0.85f, 1.8f, SortFire, Delay);
		else Put(TEXT("NS_Explosion") + Tint, Main, B.Size * K, Road, Off, 0.f, 1.8f, 0, Delay);
	};
	auto Roll = [&](float K) { if (bThin && Stack >= 3) return; Put(TEXT("NS_FxFireRoll"), Main, Sz * K * NearK, Hit, FireAt, 0.9f, 1.8f, SortFire); };
	auto Jet = [&](float K, float Delay = 0.f) { Put(TEXT("NS_FxFireJet"), Main, Sz * K * NearK, Road, FVector(0.f, 0.f, 40.f), 0.85f, 1.8f, SortFire, Delay); };
	auto Streaks = [&](float K, const FVector& Off, float Delay = 0.f) { Put(TEXT("NS_FxStreaks"), Extra, Sz * K, Road, Off, 0.6f, 1.8f, SortSpark, Delay); };
	auto Debris = [&](float K, const FQuat& Way, const FVector& Off, float Delay = 0.f) { Put(TEXT("NS_FxDebris"), Extra, Sz * K, Way, Off, 0.9f, 1.8f, SortDebris, Delay); };
	auto Smoke = [&](const TCHAR* Which, float K, float Delay = 0.f) { if (bThin && Stack >= 3) return; Put(Which, Extra, Sz * K, Road, SmokeAt, 0.55f, 2.4f, SortSmoke, Delay); };
	auto Embers = [&](float K, float Delay = 0.f) { Put(TEXT("NS_FxEmbers"), Extra, Sz * K, Road, SmokeAt, 0.55f, 2.4f, 0, Delay); };
	// on the road: the ring out to the blast's reach (it ends 100 cm from the middle at scale 1) and the scorch it
	// leaves. Both lie flat on the tarmac under the blast. (A build without the flat ring gets 3.1's, facing the camera.)
	const bool bFlat = System(TEXT("NS_FxGroundRing")) != nullptr;
	auto Ring = [&](float RadiusCm, float Delay = 0.f)
	{
		if (!B.bGround || RadiusCm <= 0.f) return;
		if (bFlat) Put(TEXT("NS_FxGroundRing"), Extra, RadiusCm / 100.f, Road, OnRoad, 0.f, 1.8f, SortSmoke, Delay, true);
		else if (!bNearCam) Put(TEXT("NS_BlastRing"), Extra, RadiusCm / 100.f, Road, FireAt, 0.f, 1.8f, SortSmoke, Delay);
	};
	auto Scorch = [&](float K) { if (B.bGround) Put(TEXT("NS_FxScorch"), Extra, Sz * K, Road, FVector(0.f, 0.f, 4.f), 0.f, 1.8f, SortScorch, 0.f, true); };
	// (the ring is the blast's REAL reach, a barrel's or a missile's splash: a blast with none gets no ring. A ring on
	// the road of a made-up size under a plain wreck, or where a hazard lands, would say "this far hurts" and lie)
	const float Reach = B.Reach;

	switch (B.Kind)
	{
	case EBlast::Barrel:
		// a fuel drum goes straight up: a column of flame over a small fireball, a ring out to its real reach
		FlashStar(0.8f, FireAt); Core(0.9f); Jet(1.f); Fire(0.6f, FireAt);
		Debris(0.8f, UpQ, FireAt); Smoke(TEXT("NS_FxSmokeRise"), 0.9f); Embers(1.f); Ring(Reach); Scorch(1.f);
		break;
	case EBlast::Car: case EBlast::Truck:
		// a car's fire rolls on along the hit, and what comes off it goes the same way
		FlashStar(1.f, FireAt); Core(1.f); Fire(1.f, FireAt); Roll(1.f);
		Streaks(1.f, FireAt); Debris(1.f, Hit, FireAt); Smoke(TEXT("NS_FxSmokeRise"), 1.f); Embers(1.f); Ring(Reach); Scorch(1.f);
		if (B.Kind == EBlast::Truck)
		{
			// and a truck goes twice: the cab, then the load behind it
			const FVector Load(-320.f, 0.f, 150.f);
			Fire(0.75f, Load, 0.2f); Streaks(0.8f, Load, 0.2f); Debris(1.f, UpQ, Load, 0.2f); Smoke(TEXT("NS_FxSmokeRise"), 0.9f, 0.24f);
		}
		break;
	case EBlast::Heli:
	{
		// an airburst: a ball of fire and smoke with a ring where the rotor was, and the pieces rain down ahead
		const FQuat Down = FRotationMatrix::MakeFromXZ((Fwd * 0.8f - Up * 0.6f).GetSafeNormal(), Up).ToQuat();
		FlashStar(1.f, FVector::ZeroVector); Core(1.f); Fire(1.f, FVector::ZeroVector);
		Put(TEXT("NS_FxGroundRing"), Extra, 3.2f * Sz, Road, FVector(0.f, 0.f, 120.f), 0.85f, 1.8f, SortCore);
		Streaks(1.f, FVector::ZeroVector); Debris(1.f, Down, FVector::ZeroVector); Debris(0.8f, Hit, FVector::ZeroVector);
		Smoke(TEXT("NS_FxSmokeBall"), 1.f); Embers(1.f);
		break;
	}
	case EBlast::Boss:
		// a boss goes in a chain: the hit, three more along the body, then the column
		FlashStar(1.1f, FireAt); Core(1.f); Fire(1.f, FireAt); Roll(1.f);
		Streaks(1.f, FireAt); Debris(1.f, Hit, FireAt); Debris(1.f, UpQ, FireAt); Scorch(1.4f);
		Smoke(TEXT("NS_FxSmokeRise"), 1.f, 0.1f);
		Fire(0.5f, FVector(380.f, -160.f, 120.f), 0.12f); Streaks(0.5f, FVector(380.f, -160.f, 120.f), 0.12f);
		Fire(0.55f, FVector(-420.f, 180.f, 160.f), 0.26f); Streaks(0.5f, FVector(-420.f, 180.f, 160.f), 0.26f); Debris(0.8f, UpQ, FVector(-420.f, 180.f, 160.f), 0.26f);
		Fire(0.6f, FVector(120.f, 60.f, 240.f), 0.42f); FlashStar(0.6f, FVector(120.f, 60.f, 240.f), 0.42f);
		Jet(1.1f, 0.58f); Smoke(TEXT("NS_FxSmokeColumn"), 0.9f, 0.6f); Streaks(0.8f, FireAt, 0.58f); Embers(1.f, 0.6f);
		break;
	case EBlast::Missile:
		// a missile's blast goes on the way it was flying, and its ring is the splash's real reach. When what it hit
		// blows up too, that is the fireball: this one is only the punch through it
		FlashStar(0.7f, FVector::ZeroVector); Roll(1.1f); Ring(Reach);
		if (!B.bLight) { Fire(0.8f, FVector::ZeroVector); Streaks(0.7f, FVector::ZeroVector); Smoke(TEXT("NS_FxSmokeRise"), 0.7f); Scorch(0.8f); }
		break;
	case EBlast::MissileFizz:
		// it ran out of fuel in the air: a small burst, nothing on the road
		FlashStar(0.6f, FVector::ZeroVector); Fire(1.f, FVector::ZeroVector); Streaks(0.6f, FVector::ZeroVector); Smoke(TEXT("NS_FxSmokeBall"), 0.7f);
		break;
	case EBlast::Bike: case EBlast::HeliCrash: case EBlast::Player: case EBlast::Generic: default:
		FlashStar(1.f, FireAt); Core(1.f); Fire(1.f, FireAt);
		Streaks(1.f, FireAt); Debris(B.Kind == EBlast::Bike ? 0.8f : 1.f, Hit, FireAt); Smoke(TEXT("NS_FxSmokeRise"), 1.f); Embers(1.f); Ring(Reach); Scorch(1.f);
		break;
	}
}

// ------------------------------------------------------------------------------------------ FX overhaul: smoke laid in the world
void FTFx::Trail(const UObject* Ctx, int32 Key, const FVector& Now, const FVector& Up, const FVector& ViewFrom, float ViewSpeed)
{
	UWorld* W = Ctx ? Ctx->GetWorld() : nullptr;
	if (!W) return;
	FFxState& S = FxState(W);
	const float T = W->GetTimeSeconds();
	FTrailState* St = S.Trails.Find(Key);
	if (!St || FVector::DistSquared(St->Pen, Now) > FMath::Square(6000.f))
	{
		// new, or it jumped (a pooled id, a teleport): start from here
		FTrailState& N = S.Trails.Add(Key);
		N.Pen = Now; N.Touch = T; N.Step = 0;
		return;
	}
	St->Touch = T;
	// NS_MissileSmoke lays exactly this much (its box, ft_fx.py): one system per length, turned along the flight.
	// Laid by distance, not by time, so the smoke is as dense at 20 fps as at 200
	const float Len = 800.f;
	const bool bLow = IsLow();
	int32 Guard = 0;
	while (FVector::DistSquared(St->Pen, Now) >= Len * Len && Guard++ < 12)
	{
		const FVector Dir = (Now - St->Pen).GetSafeNormal();
		const FVector Mid = St->Pen + Dir * (Len * 0.5f);
		St->Pen += Dir * Len;
		const int32 Step = St->Step++;
		// Low: every other length (the sheet's line of separate puffs), still the Main layer: the trail reads
		if (bLow && (Step & 1)) continue;
		// seconds until the viewer is where this length lies: under that, the short-lived smoke
		const bool bNear = FVector::Dist(ViewFrom, Mid) / FMath::Max(ViewSpeed, 1000.f) < 0.55f;
		// a cap on the lengths alive at once (each is a component of its own): a salvo, or a run of misses that fly
		// their whole 2.6 s, cannot flood the pool. A missile flies at 186 m/s: 23 lengths a second
		S.SmokeUntil.RemoveAllSwap([T](float U) { return U <= T; });
		if (S.SmokeUntil.Num() >= (bLow ? 32 : 64)) continue;
		S.SmokeUntil.Add(T + (bNear ? 0.77f : 1.65f));
		FxSpawn(Ctx, bNear ? TEXT("NS_MissileSmokeNear") : TEXT("NS_MissileSmoke"), Mid, FRotationMatrix::MakeFromXZ(Dir, Up).Rotator(), 1.f, SortSmoke);
	}
}

void FTFx::TrailEnd(int32 Key)
{
	FxRaw().Trails.Remove(Key);
}

void FTFx::Tick(const UObject* Ctx, float Dt)
{
	UWorld* W = Ctx ? Ctx->GetWorld() : nullptr;
	if (!W || Dt <= 0.f) return;
#if !UE_BUILD_SHIPPING
	TickFilm(W, Dt);
#endif
	FFxState& S = FxState(W);
	const float T = W->GetTimeSeconds();
	for (int32 i = S.Carried.Num() - 1; i >= 0; --i)
	{
		FCarried& C = S.Carried[i];
		UNiagaraComponent* N = C.Comp.Get();
		if (!N || !N->IsActive() || T > C.Until || C.Vel.SizeSquared() < 400.f) { S.Carried.RemoveAtSwap(i); continue; }
		N->AddWorldOffset(C.Vel * Dt);
		C.Vel *= FMath::Exp(-C.Drag * Dt);
	}
	for (int32 i = S.Pending.Num() - 1; i >= 0; --i)
	{
		if (S.Pending[i].When > T) continue;
		const FPendingLayer P = S.Pending[i];
		S.Pending.RemoveAtSwap(i);
		if (UNiagaraComponent* N = FxSpawn(Ctx, *P.Name, P.At, P.Rot.Rotator(), P.Scale, P.Sort)) Carry(N, P.Vel, P.Drag, P.Hold);
	}
	// A wreck the car catches up with (the turbo, a corkscrew) must not fill the screen: a body that close to the lens
	// is taken out of the picture, its fire and smoke with it. (The white fireball used to hide this; round 1's first
	// frames had a black one.) Looks only: the body is still there for whatever it touches
	if (const APlayerController* PC = W->GetFirstPlayerController())
	{
		if (PC->PlayerCameraManager)
		{
			const FVector Lens = PC->PlayerCameraManager->GetCameraLocation();
			for (TActorIterator<AFTWreck> It(W); It; ++It)
				if (!It->IsHidden() && FVector::DistSquared(It->GetActorLocation(), Lens) < FMath::Square(520.f)) It->SetActorHiddenInGame(true);
		}
	}
	S.PruneT -= Dt;
	if (S.PruneT <= 0.f)
	{
		S.PruneT = 0.5f;
		S.Live.RemoveAllSwap([](const TWeakObjectPtr<UNiagaraComponent>& C) { return !C.IsValid() || !C->IsActive(); });
		for (auto It = S.Trails.CreateIterator(); It; ++It) if (T - It->Value.Touch > 0.4f) It.RemoveCurrent();
	}
}

void FTFx::ClearWorld(const UObject* Ctx, const TCHAR* Why)
{
	UWorld* W = Ctx ? Ctx->GetWorld() : nullptr;
	if (!W) return;
	FFxState& S = FxState(W);
	int32 Bursts = 0, Wrecks = 0, Flames = 0;
	for (const TWeakObjectPtr<UNiagaraComponent>& C : S.Live)
		if (UNiagaraComponent* N = C.Get()) if (N->IsActive()) { N->DeactivateImmediate(); ++Bursts; }
	const int32 Pending = S.Pending.Num(), Carried = S.Carried.Num(), Trails = S.Trails.Num();
	S.Live.Reset(); S.Pending.Reset(); S.Carried.Reset(); S.Trails.Reset(); S.BlastT.Reset(); S.SmokeUntil.Reset();
	// the wrecks of the last take, still burning where they stopped
	for (TActorIterator<AFTWreck> It(W); It; ++It) { It->Destroy(); ++Wrecks; }
	// a pooled missile's flame keeps running on its hidden actor until the actor is used again; and a missile or a
	// round still in flight when the reel starts is hidden with everything else, its trail running on unseen
	for (TActorIterator<AFTEntityActor> It(W); It; ++It)
	{
		if (!It->IsHidden()) continue;
		if (It->Trail && It->Trail->IsActive()) { It->Trail->Deactivate(); ++Flames; }
		if (It->Trail2 && It->Trail2->IsActive()) { It->Trail2->Deactivate(); ++Flames; }
	}
	UE_LOG(LogFT, Display, TEXT("FTFX clear why=%s bursts=%d pending=%d carried=%d trails=%d wrecks=%d strayTrails=%d"), Why ? Why : TEXT("-"), Bursts, Pending, Carried, Trails, Wrecks, Flames);
}

void FTFx::Census(const UObject* Ctx, const TCHAR* Tag)
{
	UWorld* W = Ctx ? Ctx->GetWorld() : nullptr;
	if (!W) return;
	FFxState& S = FxState(W);
	TMap<FName, int32> By;
	int32 All = 0;
	for (TObjectIterator<UNiagaraComponent> It; It; ++It)
	{
		UNiagaraComponent* C = *It;
		if (!IsValid(C) || C->GetWorld() != W || !C->IsActive() || !C->GetAsset()) continue;
		++All;
		By.FindOrAdd(C->GetAsset()->GetFName())++;
	}
	int32 Wrecks = 0;
	for (TActorIterator<AFTWreck> It(W); It; ++It) ++Wrecks;
	By.KeySort([](const FName& A, const FName& B) { return A.LexicalLess(B); });
	FString List;
	for (const TPair<FName, int32>& P : By) List += FString::Printf(TEXT(" %s:%d"), *P.Key.ToString(), P.Value);
	UE_LOG(LogFT, Display, TEXT("FTFX census tag=%s systems=%d wrecks=%d carried=%d pending=%d trails=%d low=%d |%s"),
		Tag ? Tag : TEXT("-"), All, Wrecks, S.Carried.Num(), S.Pending.Num(), S.Trails.Num(), IsLow() ? 1 : 0, *List);
}

#if !UE_BUILD_SHIPPING
namespace
{
	// ft.FxCensus [tag]: what is still running, in the log (a staged run's proof that a new scene starts clean)
	FAutoConsoleCommandWithWorldAndArgs GFTFxCensus(TEXT("ft.FxCensus"), TEXT("FULL THROTTLE FX overhaul tests: log every effect still running in the world, by system"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World) { FTFx::Census(World, Args.Num() ? *Args[0] : TEXT("cmd")); }));

	// ft.FxFilm <name> <frames> [step]: see TickFilm
	FAutoConsoleCommandWithWorldAndArgs GFTFxFilm(TEXT("ft.FxFilm"), TEXT("FULL THROTTLE FX overhaul tests: ft.FxFilm <name> <frames> [step]: a screenshot each <step> s of game time (1/30)"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
		{
			if (Args.Num() < 2 || !World) return;
			GFilm = FFxFilm();
			GFilm.Name = Args[0];
			GFilm.Left = FMath::Clamp(FCString::Atoi(*Args[1]), 1, 240);
			if (Args.Num() > 2) GFilm.Step = FMath::Clamp(FCString::Atof(*Args[2]), 0.005f, 0.5f);
			GFilm.Acc = GFilm.Step;   // the first frame is the next one drawn
			if (const AWorldSettings* WS = World->GetWorldSettings()) GFilm.WasMax = WS->MaxUndilatedFrameTime;
			UE_LOG(LogFT, Display, TEXT("FT FXFILM start name=%s frames=%d step=%.4f"), *GFilm.Name, GFilm.Left, GFilm.Step);
		}));
}
#endif
