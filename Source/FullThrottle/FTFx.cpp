#include "FTFx.h"
#include "FTGameInstance.h"
#include "FTCore.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "HAL/IConsoleManager.h"

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
	};

	TMap<FName, UNiagaraSystem*>& Cache()
	{
		static TMap<FName, UNiagaraSystem*> Map;
		return Map;
	}
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
	UNiagaraSystem* Sys = System(Name);
	if (!Sys) return nullptr;
	return UNiagaraFunctionLibrary::SpawnSystemAtLocation(Ctx, Sys, At, Rot, FVector(Scale), true, true, ENCPoolMethod::AutoRelease);
}

UNiagaraComponent* FTFx::Attach(const TCHAR* Name, USceneComponent* To, const FVector& At, const FRotator& Rot, float Scale, int32 Layer, bool bRelative)
{
	if (!To || !Shows(To, Layer)) return nullptr;
	UNiagaraSystem* Sys = System(Name);
	if (!Sys) return nullptr;
	return UNiagaraFunctionLibrary::SpawnSystemAttached(Sys, To, NAME_None, At, Rot, FVector(Scale),
		bRelative ? EAttachLocation::KeepRelativeOffset : EAttachLocation::KeepWorldPosition, true, ENCPoolMethod::AutoRelease, true);
}

void FTFx::Warm(AActor* Owner)
{
	if (!Owner || !Owner->GetWorld()) return;
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
