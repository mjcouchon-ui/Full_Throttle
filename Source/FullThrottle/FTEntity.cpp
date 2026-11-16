#include "FTEntity.h"
#include "FTFx.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Engine/StaticMesh.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "HAL/IConsoleManager.h"
#include "Materials/MaterialInterface.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "FTPlayerCar.h"
#include "FTGameMode.h"   // overhaul: the boss's proxy asks whether it was wrecked when it is let go (GetActiveEntities)
#include "EngineUtils.h"              // overhaul, round two: the boss's defeat finds the wreck of his own body (TActorIterator)
#include "TimerManager.h"             // ... and plays its later beats on the world's timers
#include "PhysicsEngine/BodySetup.h"  // ... and only throws a part that has a collision shape

namespace
{
	TAutoConsoleVariable<int32> CVarPickupMotion(TEXT("ft.PickupMotion"), 1, TEXT("Pickup visual bob and rocking; 0 for comparison."));
	TAutoConsoleVariable<int32> CVarPickupBurst(TEXT("ft.PickupBurst"), 1, TEXT("Pickup collection chips; always disabled at Low effects quality."));
	TAutoConsoleVariable<float> CVarPickupLight(TEXT("ft.PickupLight"), 2.f, TEXT("Existing unshadowed pickup light intensity in candelas; 0 disables it. Off at Low effects quality."));
	float PickupLightIntensity()
	{
		static const IConsoleVariable* Quality = IConsoleManager::Get().FindConsoleVariable(TEXT("sg.EffectsQuality"));   // found once (as FTFx::IsLow), read every call
		return Quality && Quality->GetInt() == 0 ? 0.f : FMath::Max(0.f, CVarPickupLight.GetValueOnGameThread());
	}
	template <typename T> T* Load(const FString& Path)
	{
		return Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *Path, nullptr, LOAD_NoWarn | LOAD_Quiet));
	}
	/** A mesh loaded once and kept (rooted: a plain static cache is invisible to the GC, see MeshFor). */
	UStaticMesh* Kept(const TCHAR* Path)
	{
		static TMap<FString, UStaticMesh*> Cache;
		if (UStaticMesh** F = Cache.Find(Path)) return *F;
		UStaticMesh* M = Load<UStaticMesh>(Path);
		if (M) M->AddToRoot();
		Cache.Add(Path, M);
		return M;
	}

	// 3.3 where the moving bits sit on their models (ft_v33.py reports them; model space, cm)
	const FVector HeliRotorHub(0.f, 0.f, 306.f);
	const FVector HeliTailHub(-641.f, 18.f, 280.f);
	const FVector HeliRope[2] = { FVector(8.f, -162.f, 203.f), FVector(8.f, 162.f, 203.f) };
	const FVector TeslaTop(440.f - 625.f, 0.f, 880.f);
	const FVector ClownProps[4] = { FVector(150.f - 625.f, -400.f, 620.f), FVector(150.f - 625.f, 400.f, 620.f), FVector(1090.f - 625.f, -400.f, 620.f), FVector(1090.f - 625.f, 400.f, 620.f) };
	constexpr int32 NumClownProps = 4;
	const FVector RappelGrip(0.f, 0.f, 203.f);   // the raised glove in ft_swat_overhaul.py

	UStaticMeshComponent* MakeBit(AActor* Owner, USceneComponent* Parent)
	{
		UStaticMeshComponent* C = NewObject<UStaticMeshComponent>(Owner);
		C->SetupAttachment(Parent);
		C->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		C->SetGenerateOverlapEvents(false);
		C->bAffectDistanceFieldLighting = false;   // 2.8 perf (see AFTEntityActor)
		C->bAffectDynamicIndirectLighting = false;
		C->SetMobility(EComponentMobility::Movable);
		C->RegisterComponent();
		Owner->AddInstanceComponent(C);
		return C;
	}

	/** Content/Data/wheels.json (ft_v33.py): body mesh path -> its wheels [x, y, z, r, w]. */
	const TMap<FString, TArray<AFTEntityActor::FWheelDef>>& WheelTable()
	{
		static TMap<FString, TArray<AFTEntityActor::FWheelDef>> T;
		static bool bLoaded = false;
		if (bLoaded) return T;
		bLoaded = true;
		FString Text;
		const FString Path = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Data"), TEXT("wheels.json"));
		TSharedPtr<FJsonObject> J;
		if (!FFileHelper::LoadFileToString(Text, *Path) || !FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Text), J) || !J.IsValid())
		{
			UE_LOG(LogFT, Warning, TEXT("FT LOAD data=wheels.json result=missing path=%s"), *Path);
			return T;
		}
		for (const TPair<FString, TSharedPtr<FJsonValue>>& P : J->Values)
		{
			const TArray<TSharedPtr<FJsonValue>>* Arr = nullptr;
			if (!P.Value.IsValid() || !P.Value->TryGetArray(Arr)) continue;
			TArray<AFTEntityActor::FWheelDef>& L = T.Add(P.Key);
			for (const TSharedPtr<FJsonValue>& V : *Arr)
			{
				const TArray<TSharedPtr<FJsonValue>>* N = nullptr;
				if (!V.IsValid() || !V->TryGetArray(N) || N->Num() < 5) continue;
				AFTEntityActor::FWheelDef W;
				W.C = FVector((*N)[0]->AsNumber(), (*N)[1]->AsNumber(), (*N)[2]->AsNumber());
				W.R = (*N)[3]->AsNumber(); W.W = (*N)[4]->AsNumber();
				L.Add(W);
			}
		}
		UE_LOG(LogFT, Display, TEXT("FT LOAD data=wheels.json result=ok meshes=%d"), T.Num());
		return T;
	}

	UNiagaraSystem* PickupSystem(EFTKind Kind)
	{
		static UNiagaraSystem* Systems[2] = {};
		const int32 Index = Kind == EFTKind::Wrench ? 1 : 0;
		if (!Systems[Index])
		{
			Systems[Index] = Load<UNiagaraSystem>(Index ? TEXT("/Game/FX/Pickups/NS_PickupRepair") : TEXT("/Game/FX/Pickups/NS_PickupMissile"));
			if (Systems[Index]) Systems[Index]->AddToRoot();
		}
		return Systems[Index];
	}
}

void AFTEntityActor::WarmPickups(AActor* Owner)
{
	if (!Owner || !Owner->GetWorld()) return;
	// Submit the exact mesh and sprite vertex-factory PSOs during level setup, before pickups spawn.
	for (EFTKind K : { EFTKind::Crate, EFTKind::Wrench })
	{
		UStaticMeshComponent* WarmMesh = NewObject<UStaticMeshComponent>(Owner);
		Owner->AddInstanceComponent(WarmMesh);
		WarmMesh->SetupAttachment(Owner->GetRootComponent());
		WarmMesh->SetMobility(EComponentMobility::Movable);
		WarmMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WarmMesh->SetHiddenInGame(true);
		WarmMesh->SetStaticMesh(MeshFor(K, 0, 0));
		WarmMesh->RegisterComponent();
		WarmMesh->PrecachePSOs();
		if (UNiagaraSystem* System = PickupSystem(K))
		{
			UNiagaraComponent* WarmFX = NewObject<UNiagaraComponent>(Owner);
			Owner->AddInstanceComponent(WarmFX);
			WarmFX->SetupAttachment(Owner->GetRootComponent());
			WarmFX->SetAutoActivate(false);
			WarmFX->SetHiddenInGame(true);
			WarmFX->SetAsset(System);
			WarmFX->RegisterComponent();
			WarmFX->PrecachePSOs();
			// System PostLoad primes its configured pool; the world manager's helper is not an exported API.
		}
	}
}

void AFTEntityActor::PickupBurst(UWorld* World, EFTKind Kind, const FTransform& At)
{
	if (!World || (Kind != EFTKind::Crate && Kind != EFTKind::Wrench) || CVarPickupBurst.GetValueOnGameThread() == 0) return;
	static const IConsoleVariable* Quality = IConsoleManager::Get().FindConsoleVariable(TEXT("sg.EffectsQuality"));
	if (Quality && Quality->GetInt() == 0) return;
	if (UNiagaraSystem* System = PickupSystem(Kind))
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, System, At.TransformPosition(FVector(0, 0, 70)),
			At.Rotator(), FVector::OneVector, true, true, ENCPoolMethod::AutoRelease);
}

UStaticMesh* AFTEntityActor::MeshFor(EFTKind Kind, int32 Variant, int32 Level)
{
	static TMap<FString, UStaticMesh*> Cache;
	FString P;
	switch (Kind)
	{
	case EFTKind::Moto: P = TEXT("/Game/Characters/Syndicate/SM_Moto"); break;
	case EFTKind::Rammer: P = TEXT("/Game/Characters/Syndicate/SM_Rammer"); break;
	case EFTKind::Suv: P = TEXT("/Game/Characters/Syndicate/SM_Suv"); break;
	case EFTKind::Truck: P = TEXT("/Game/Characters/Syndicate/SM_Truck"); break;
	case EFTKind::Heli: P = TEXT("/Game/Characters/Choppers/SM_Heli"); break;
	case EFTKind::Hog: P = Variant ? TEXT("/Game/Characters/SnakeEyes/SM_Hog2") : TEXT("/Game/Characters/SnakeEyes/SM_Hog"); break;
	case EFTKind::Ramp: P = TEXT("/Game/Characters/Traffic/SM_CarCarrier"); break;
	case EFTKind::Civ:
	{
		static const TCHAR* C[] = { TEXT("SM_Sedan0"), TEXT("SM_Sedan1"), TEXT("SM_Sedan2"), TEXT("SM_Hatch"), TEXT("SM_Hatch1"), TEXT("SM_Van"), TEXT("SM_Van1"), TEXT("SM_Bus") };
		P = FString(TEXT("/Game/Characters/Traffic/")) + C[FMath::Clamp(Variant, 0, 7)];
		break;
	}
	case EFTKind::Crate: P = TEXT("/Game/Characters/Pickups/SM_Crate"); break;
	case EFTKind::Wrench: P = TEXT("/Game/Characters/Pickups/SM_Wrench"); break;
	case EFTKind::Barrel: P = Level == 1 ? TEXT("/Game/Characters/Pickups/SM_DrumGreen") : TEXT("/Game/Characters/Pickups/SM_Barrel"); break;
	case EFTKind::Henchman: P = TEXT("/Game/Characters/Syndicate/SM_Henchman"); break;
	case EFTKind::Boss:
	{
		static const TCHAR* B[8] = { TEXT("SM_Juggernaut"), TEXT("SM_Tanker"), TEXT("SM_Carrier"), TEXT("SM_FireTruck"), TEXT("SM_ClownCar"), TEXT("SM_Yeti"), TEXT("SM_SharkCar"), TEXT("SM_Limo") };
		P = FString(TEXT("/Game/Characters/Bosses/")) + B[FMath::Clamp(Level, 0, 7)];
		break;
	}
	// 2.5: the boss's bullets look like the boss: fish from the shark, fireballs from the fire truck, the limo's heavy tracers
	case EFTKind::Bullet:
		P = Variant == 1 ? TEXT("/Game/Characters/Projectiles/SM_Swordfish") : (Variant == 2 ? TEXT("/Game/Characters/Projectiles/SM_Fireball") : (Variant == 3 ? TEXT("/Game/Characters/Projectiles/SM_BigTracer") : TEXT("/Game/FX/SM_Tracer")));
		if (Variant == 4) P = TEXT("/Game/FX/SM_Pie");   // 3.1: MR. CHUCKLES throws pies (ft_attackfx.py); no pie, a tracer as before
		break;
	case EFTKind::Missile: P = TEXT("/Game/Characters/Kestrel/SM_Missile"); break;
	default: return nullptr;
	}
	// 2.8: every level dresses its Syndicate its own way (Themed/L<n>/SM_<kind>, built by ft_v28.py); Bayshore
	// keeps the classic purple Syndicate, and a level without a themed model falls back to the classic one
	if (Level >= 1 && Level <= 7 && (Kind == EFTKind::Moto || Kind == EFTKind::Rammer || Kind == EFTKind::Suv || Kind == EFTKind::Truck || Kind == EFTKind::Heli || Kind == EFTKind::Henchman))
	{
		FString Leaf; P.Split(TEXT("/"), nullptr, &Leaf, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		const FString Th = FString::Printf(TEXT("/Game/Characters/Themed/L%d/%s"), Level, *Leaf);
		UStaticMesh** TF = Cache.Find(Th);
		UStaticMesh* TM = TF ? *TF : Load<UStaticMesh>(Th);
		if (!TF) { if (TM) TM->AddToRoot(); Cache.Add(Th, TM); }
		if (TM) return TM;
	}
	if (UStaticMesh** F = Cache.Find(P)) return *F;
	UStaticMesh* M = Load<UStaticMesh>(P);
	// a missing 2.5 model falls back to the 1.x one rather than an invisible foe
	if (!M && Kind == EFTKind::Bullet) M = Load<UStaticMesh>(TEXT("/Game/FX/SM_Tracer"));
	if (!M && Kind == EFTKind::Boss) M = Load<UStaticMesh>(TEXT("/Game/Characters/Bosses/SM_Juggernaut"));
	// the cache is invisible to the garbage collector: root the mesh, or the 61 s GC pass frees a mesh no
	// proxy is showing at that moment and the next Setup/wreck uses a dangling pointer
	if (M) M->AddToRoot();
	Cache.Add(P, M);
	return M;
}

// ------------------------------------------------------------------------------------------ the bosses' rigs
// Overhaul (15 Nov 2026): every boss is its body plus moving parts and effects, posed from the fight's own state.
// The boss's function in FTGameMode.cpp says what is happening (BossState, BossWind, BossFire, SetTesla); the code
// here turns that into motion. Nothing feeds back: no timer, speed, box or damage is ever read from a pose.
// Parts are Bits slots (attached to the body mesh, so they ride its springs); their pivots are the generator's
// (ft_enemy_vehicles.py, BOSS_PARTS), repeated here in the body's own space (cm). A ghost in the highlight reel is
// never animated: Setup must leave every part in a pose that stands by itself.
namespace
{
	inline UStaticMeshComponent* BitOf(AFTEntityActor& A, int32 I) { return A.Bits.IsValidIndex(I) ? A.Bits[I].Get() : nullptr; }
	inline void Show(UStaticMeshComponent* C, bool bOn) { if (C && C->IsVisible() != bOn) C->SetVisibility(bOn); }
	/** 1 at T = 0, gone after about 3 / Rate seconds. */
	inline float Pulse(float T, float Rate) { return FMath::Exp(-FMath::Max(T, 0.f) * Rate); }
	inline float Smooth(float U) { U = FMath::Clamp(U, 0.f, 1.f); return U * U * (3.f - 2.f * U); }
	/** The rigs' own random numbers: looks only, never the generator the fights draw from (FMath's). */
	FRandomStream RigRng(0xB055);
	/** A push on the body's springs (degrees a second, cm a second): it rocks and settles by itself. */
	void Kick(AFTEntityActor& A, float Pitch, float Roll, float Heave) { A.SPitchV += Pitch; A.SRollV += Roll; A.SHeaveV += Heave; }
	/** A lightning bolt (SM_TeslaArc: 1000 cm down its own -Z) between two points on the body, crackling. */
	void LocalArc(UStaticMeshComponent* Arc, const FVector& From, const FVector& To, float Thick)
	{
		if (!Arc) return;
		const FVector Dv = From - To;
		const float L = Dv.Size();
		if (L < 20.f) { Show(Arc, false); return; }
		Arc->SetRelativeLocationAndRotation(From, FQuat(FRotationMatrix::MakeFromZ(Dv / L).Rotator()) * FQuat(FVector::UpVector, RigRng.FRandRange(0.f, 2.f * PI)));
		Arc->SetRelativeScale3D(FVector(Thick, Thick, L / 1000.f));
		Show(Arc, true);
	}
	/** A piece that flies off when the boss dies: a wreck of its own (a rigid body, as the body's). Kick is in the
	 *  boss's own axes (cm/s) on top of the speed it was doing. */
	/** Round two: a wreck that is not a burnt thing (the Dealer's dice, the clown's head, the yeti and his ice): its
	 *  own colours, no fire, no smoke. Only this one wreck's look: how it flies and how long it lasts are the game's. */
	void Unburnt(AFTWreck* Wr)
	{
		if (!Wr) return;
		Wr->Mesh->SetScalarParameterValueOnMaterials(TEXT("Soot"), 0.f);
		Wr->Fire->Deactivate();
		Wr->Smoke->Deactivate();
	}
	AFTWreck* ThrowPart(AFTEntityActor& A, UStaticMeshComponent* C, const FVector& Away, const FVector& Spin, bool bBurn = true)
	{
		UWorld* W = A.GetWorld();
		if (!W || !C || !C->GetStaticMesh() || !C->IsVisible()) return nullptr;
		// (a mesh without a collision shape cannot be a rigid body: it would hang in the air where it was let go)
		const UBodySetup* Shape = C->GetStaticMesh()->GetBodySetup();
		if (!Shape || Shape->AggGeom.GetElementCount() == 0) return nullptr;
		const FTransform T = C->GetComponentTransform();
		const FTransform At = A.GetActorTransform();
		FActorSpawnParameters SP; SP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AFTWreck* Wr = W->SpawnActor<AFTWreck>(AFTWreck::StaticClass(), T, SP);
		if (!Wr) return nullptr;
		Wr->Launch(C->GetStaticMesh(), FTransform(T.GetRotation(), T.GetLocation()), At.GetUnitAxis(EAxis::X) * A.LastSpeed * 0.85f + At.TransformVectorNoScale(Away), Spin, FMath::Abs(T.GetScale3D().Z));
		if (!bBurn) Unburnt(Wr);
		return Wr;
	}

	// ---- round two: how a boss dies. Kill (the game mode's) has already blown him up and launched the wreck of his
	// body when the proxy is let go; from here the rig gives that wreck what is his (parts left hanging on it, his own
	// fire), throws his parts, and plays the rest of his end beat by beat. Every beat is a look: sprites and wrecks
	// that nothing in the race reads. The wrap freezes the game for 0.22 s and then runs it at 0.22 speed, and its
	// CUT! card covers the road where he was from 0.8 s on, so the first beat is the big one and what follows goes
	// up, over the card, or stays on the wreck as it tumbles on.
	/** The game's own wreck of this boss's body (the newest wreck wearing his body mesh, where he was). */
	AFTWreck* BodyWreck(AFTEntityActor& A)
	{
		UWorld* W = A.GetWorld();
		UStaticMesh* Body = A.Mesh->GetStaticMesh();
		if (!W || !Body) return nullptr;
		const FVector Here = A.GetActorLocation();
		AFTWreck* Best = nullptr;
		for (TActorIterator<AFTWreck> It(W); It; ++It)
		{
			AFTWreck* Wr = *It;
			if (!Wr || !Wr->Mesh || Wr->Mesh->GetStaticMesh() != Body || Wr->Age > 0.05f) continue;
			if (!Best || FVector::DistSquared(Wr->GetActorLocation(), Here) < FVector::DistSquared(Best->GetActorLocation(), Here)) Best = Wr;
		}
		return Best;
	}
	/** A piece left hanging on a wreck: it rides the wreck's body, without a shape of its own, and goes with it. */
	UStaticMeshComponent* WreckBit(AFTWreck* Wr, UStaticMesh* M, const FVector& At, const FRotator& Rot, float Soot = 0.9f, float Scale = 1.f)
	{
		if (!Wr || !M) return nullptr;
		UStaticMeshComponent* C = MakeBit(Wr, Wr->Mesh);
		C->SetStaticMesh(M);
		C->SetRelativeLocationAndRotation(At, Rot);
		C->SetRelativeScale3D(FVector(Scale));
		C->SetScalarParameterValueOnMaterials(TEXT("Soot"), Soot);
		return C;
	}
	/** A looping effect that burns on a wreck until the wreck goes (the wreck's own component, as AFTEntityActor::Loop's
	 *  are the proxy's: a pooled one would be left looping in the air). The Extra layer: not at Low effects quality. */
	UNiagaraComponent* WreckLoop(AFTWreck* Wr, const TCHAR* Name, const FVector& At, const FRotator& Rot = FRotator::ZeroRotator, float Scale = 1.f)
	{
		UNiagaraSystem* Sys = (Wr && FTFx::Shows(Wr, FTFx::Extra)) ? FTFx::System(Name) : nullptr;
		if (!Sys) return nullptr;
		UNiagaraComponent* N = NewObject<UNiagaraComponent>(Wr);
		N->SetupAttachment(Wr->Mesh);
		N->SetAutoActivate(false);
		N->RegisterComponent();
		Wr->AddInstanceComponent(N);
		N->SetAsset(Sys);
		N->SetRelativeLocationAndRotation(At, Rot);
		N->SetRelativeScale3D(FVector(Scale));
		N->Activate(true);
		return N;
	}
	/** What the later beats of a defeat have to go on (the proxy is hidden and back in its pool by then): the wreck
	 *  of his body, and which way he was facing. */
	struct FDeath { TWeakObjectPtr<AFTWreck> Body; FRotator Face = FRotator::ZeroRotator; };
	/** A burst where a point of the body's wreck is now (Local: the body's own space), upright whatever way the wreck
	 *  has tumbled, Lift cm higher: left in the world, as every explosion is. Nothing if the wreck has gone. */
	void Pop(const FDeath& D, const TCHAR* Name, const FVector& Local, float Scale, int32 Layer = FTFx::Main, float Lift = 0.f)
	{
		if (const AFTWreck* Wr = D.Body.Get())
			FTFx::Burst(Wr, Name, Wr->GetActorTransform().TransformPosition(Local) + FVector(0.f, 0.f, Lift), D.Face, Scale, Layer);
	}
	/** A later beat of a defeat: Delay seconds of the game's own clock after the kill, so it rides the wrap's slow motion. */
	void Later(AFTEntityActor& A, float Delay, TFunction<void()> Beat)
	{
		UWorld* W = A.GetWorld();
		if (!W) return;
		FTimerHandle H;
		W->GetTimerManager().SetTimer(H, FTimerDelegate::CreateWeakLambda(&A, [Beat]() { Beat(); }), Delay, false);
	}
	/** True on the one frame the boss's entrance clock passes T (seconds since he arrived): an entrance's one-off beat. */
	inline bool Crossed(const AFTEntityActor& A, float Dt, float T) { return A.bBossLive && A.IntroT > T && A.IntroT - Dt <= T; }

	// What every boss shares: where a hit shows and with what, and where it comes apart
	struct FBossLook { const TCHAR* HurtFx; FVector HurtAt; const TCHAR* DefeatFx; FVector Heart; };
	const FBossLook BossLooks[8] = {
		{ TEXT("NS_BossHurtSparks"), FVector(-560.f, 0.f, 230.f), TEXT("NS_BossDefeat"), FVector(-200.f, 0.f, 260.f) },    // THE JUGGERNAUT
		{ TEXT("NS_BossHurtZap"), FVector(-185.f, 0.f, 430.f), TEXT("NS_BossDefeatZap"), FVector(-185.f, 0.f, 700.f) },    // THE ALCHEMIST
		{ TEXT("NS_BossHurtChips"), FVector(-520.f, 0.f, 300.f), TEXT("NS_BossDefeatChips"), FVector(-100.f, 0.f, 330.f) },   // THE BIG DEALER
		{ TEXT("NS_BossHurtSparks"), FVector(-600.f, 0.f, 240.f), TEXT("NS_BossDefeatFire"), FVector(-205.f, 0.f, 360.f) },   // ENGINE 13
		{ TEXT("NS_BossHurtConfetti"), FVector(-560.f, 0.f, 330.f), TEXT("NS_BossDefeatConfetti"), FVector(-100.f, 0.f, 460.f) },   // MR. CHUCKLES
		{ TEXT("NS_BossHurtSnow"), FVector(-220.f, 0.f, 640.f), TEXT("NS_BossDefeatSnow"), FVector(0.f, 0.f, 620.f) },       // THE ABOMINABLE
		{ TEXT("NS_BossHurtSpray"), FVector(-560.f, 0.f, 280.f), TEXT("NS_BossDefeatSplash"), FVector(-100.f, 0.f, 300.f) }, // THE GREAT WHITE
		{ TEXT("NS_BossHurtBills"), FVector(-600.f, 0.f, 220.f), TEXT("NS_BossDefeatCash"), FVector(-150.f, 0.f, 240.f) },   // THE CHAIRMAN
	};
	inline const FBossLook& LookOf(const AFTEntityActor& A) { return BossLooks[FMath::Clamp(A.Lvl, 0, 7)]; }

	// ---------------------------------------------------------------- THE JUGGERNAUT (level 1): the armoured hauler
	// Bits: 0 the turret, 1-2 its rotary guns, 3-4 the roof shields (left, right), 5-6 the barrel bays' trays, 7-8
	// the barrels on them (the game's own barrel mesh). BossFx: 0 smoke (damage 1), 1 fire (damage 2). RigA the guns'
	// turn, RigB the turret's scan clock, RigE how far the trays are out, RigF its list, RigH the guns' heat.
	// Wind kind 1: the fan of rounds. Cue[0]: the barrel drop's wind-up (it runs on its own timer). Fire kinds: 1 the
	// fan, 2 the barrels.
	const FVector JuggTurret(-200.f, 0.f, 300.f), JuggGun(-176.f, 64.f, 70.f), JuggShield(-600.f, 196.f, 321.f), JuggTray(-625.f, 180.f, 84.f);
	const FVector JuggStack(425.f, 266.f, 360.f);   // the top of an exhaust stack behind the cab (ft_enemy_vehicles.juggernaut_body)
	constexpr float JuggTrayRun = 210.f;            // how far a tray runs out (round two: 120 was lost under the tail at 58 m)
	void SetupJuggernaut(AFTEntityActor& A)
	{
		A.Part(0, TEXT("/Game/Characters/Bosses/SM_JuggTurret"), JuggTurret);
		for (int32 s = 0; s < 2; ++s)
		{
			const float Y = s ? 1.f : -1.f;
			A.Part(1 + s, TEXT("/Game/Characters/Bosses/SM_JuggGun"), JuggTurret + FVector(JuggGun.X, Y * JuggGun.Y, JuggGun.Z), false);
			A.Part(3 + s, TEXT("/Game/Characters/Bosses/SM_JuggShield"), FVector(JuggShield.X, Y * JuggShield.Y, JuggShield.Z), false);
			A.Part(5 + s, TEXT("/Game/Characters/Bosses/SM_JuggTray"), FVector(JuggTray.X, Y * JuggTray.Y, JuggTray.Z), false);
			Show(A.Part(7 + s, TEXT("/Game/Characters/Pickups/SM_Barrel"), FVector(JuggTray.X + 50.f, Y * JuggTray.Y, JuggTray.Z), false), false);
		}
	}
	void AnimJuggernaut(AFTEntityActor& A, float Dt)
	{
		const float W1 = A.WindKind == 1 ? A.Wind : 0.f, W2 = FMath::Clamp(A.Cue[0], 0.f, 1.f);
		const float Recoil = A.FireKind == 1 ? Pulse(A.FireT, 7.f) : 0.f;
		// the entrance: the turret comes up out of its ring and the shields swing up while he drops back onto his spot
		// (done before his first fan winds up, a second after he appears)
		const float In = A.bBossLive ? Smooth(A.IntroT / 1.0f) : 1.f;
		// (round two: and both exhaust stacks throw a pillar of fire as he arrives: two tall bright strokes that read
		// from 160 m; fire is nothing he attacks with)
		if (Crossed(A, Dt, 0.05f))
			for (int32 s = 0; s < 2; ++s) FTFx::Attach(TEXT("NS_BossPillar"), A.Mesh, FVector(JuggStack.X, (s ? 1.f : -1.f) * JuggStack.Y, JuggStack.Z), FRotator::ZeroRotator, 1.1f, FTFx::Main, true);
		if (W2 > 0.f && A.RigG <= 0.f) { UE_LOG(LogFT, Display, TEXT("FT BOSSRIG wind level=%d kind=2"), A.Lvl); }
		A.RigG = W2;
		// the turret scans the road while nothing is due, squares up on the car as its guns spin, and is thrown back by the fan
		A.RigB += Dt;
		const float Scan = 11.f * FMath::Sin(A.RigB * 0.9f) * (1.f - Smooth(W1 * 2.f)) * (1.f - Recoil);
		const FQuat TQ(FRotator(3.f * W1 - 8.f * Recoil, Scan, 0.f));
		const FVector TP = JuggTurret + FVector(24.f * Recoil, 0.f, -70.f * (1.f - In));
		if (UStaticMeshComponent* T = BitOf(A, 0)) T->SetRelativeLocationAndRotation(TP, TQ);
		// the guns turn only when a fan is coming: faster through its last 0.6 s, flat out as it goes, then they run down
		A.RigA += 2100.f * FMath::Max(W1 * W1, A.FireKind == 1 ? Pulse(A.FireT, 2.2f) : 0.f) * Dt;
		const float Heat = FMath::RoundToFloat((2.5f * W1 + 4.f * Recoil) * 4.f) * 0.25f;
		for (int32 s = 0; s < 2; ++s)
		{
			const float Y = s ? 1.f : -1.f;
			if (UStaticMeshComponent* G = BitOf(A, 1 + s))
			{
				G->SetRelativeLocationAndRotation(TP + TQ.RotateVector(FVector(JuggGun.X + 30.f * Recoil, Y * JuggGun.Y, JuggGun.Z)), TQ * FQuat(FRotator(0.f, 0.f, Y * A.RigA)));
				if (Heat != A.RigH) G->SetScalarParameterValueOnMaterials(TEXT("Alert"), Heat);
				if (Heat != A.RigH && s == 0) if (UStaticMeshComponent* Tur = BitOf(A, 0)) Tur->SetScalarParameterValueOnMaterials(TEXT("Alert"), Heat);
			}
			// a roof shield: up with the entrance, rattled by every hit and by the guns
			if (UStaticMeshComponent* Sh = BitOf(A, 3 + s))
				Sh->SetRelativeRotation(FRotator(-88.f * (1.f - In) + 5.f * Pulse(A.HurtT, 6.f) * A.HurtAmt * FMath::Sin(A.AnimT * 61.f + s) - 4.f * Recoil, 0.f, 0.f));
		}
		A.RigH = Heat;
		// the barrel bays: the trays run out through the drop's wind-up with a barrel on each, let it go, and run back in
		A.RigE = FMath::FInterpTo(A.RigE, Smooth(W2), Dt, 14.f);
		for (int32 s = 0; s < 2; ++s)
		{
			const float Y = s ? 1.f : -1.f;
			if (UStaticMeshComponent* Tr = BitOf(A, 5 + s)) Tr->SetRelativeLocation(FVector(JuggTray.X - JuggTrayRun * A.RigE, Y * JuggTray.Y, JuggTray.Z));
			if (UStaticMeshComponent* Br = BitOf(A, 7 + s))
			{
				Show(Br, W2 > 0.06f && Br->GetStaticMesh() != nullptr);
				Br->SetRelativeLocation(FVector(JuggTray.X - JuggTrayRun * A.RigE + 50.f, Y * JuggTray.Y, JuggTray.Z));
			}
		}
		A.BossAlert = FMath::Max(1.1f * W2, 0.9f * W1);   // the bays' lamps, and every other light on the hull, as the barrels come out or the guns spin up
		// worn down, it lists to the side that has lost its shield
		A.RigF = FMath::FInterpTo(A.RigF, A.DmgState >= 2 ? 0.f : (A.DmgState == 1 ? -2.5f : 0.f), Dt, 2.f);
		// (only on wheels: the body's springs write its rotation afresh every frame then, so this cannot add up)
		if (A.WheelSpec.Num() && FMath::Abs(A.RigF) > 0.05f) A.Mesh->SetRelativeRotation(A.Mesh->GetRelativeRotation() + FRotator(0.f, 0.f, A.RigF));
	}
	void FireJuggernaut(AFTEntityActor& A)
	{
		if (A.FireKind == 1)
		{
			for (int32 s = 0; s < 2; ++s)
				if (UStaticMeshComponent* G = BitOf(A, 1 + s)) FTFx::Attach(TEXT("NS_BossGunFlash"), G, FVector(-300.f, 0.f, 0.f), FRotator::ZeroRotator, 1.f, FTFx::Flash, true);
			Kick(A, 60.f, 0.f, -220.f);
		}
		else
		{
			FTFx::Attach(TEXT("NS_BossDust"), A.Mesh, FVector(-800.f, 0.f, 20.f), FRotator::ZeroRotator, 1.f, FTFx::Extra, true);
			Kick(A, -25.f, 0.f, 140.f);
		}
	}
	void BreakJuggernaut(AFTEntityActor& A, int32 State)
	{
		// a roof shield is shot away: the left at damage 1, the right at 2
		const int32 s = State == 1 ? 0 : 1;
		if (UStaticMeshComponent* Sh = BitOf(A, 3 + s))
		{
			FTFx::Attach(TEXT("NS_BossBreak"), A.Mesh, FVector(JuggShield.X, (s ? 1.f : -1.f) * JuggShield.Y, JuggShield.Z + 90.f), FRotator::ZeroRotator, 0.7f, FTFx::Main, true);
			Show(Sh, false);
		}
		if (State == 1) A.Loop(0, TEXT("NS_BossSmoke"), FVector(JuggShield.X + 60.f, -JuggShield.Y, JuggShield.Z + 20.f), true);
		else A.Loop(1, TEXT("NS_BossFlame"), FVector(JuggShield.X + 60.f, JuggShield.Y, JuggShield.Z + 20.f), true);
	}
	void DefeatJuggernaut(AFTEntityActor& A, const FDeath& D)
	{
		// the magazine under the turret goes first: the turret is blown straight up out of its ring on a pillar of fire
		// (high over the road, where the camera still sees it), the guns are torn off it to either side and whatever
		// armour is left goes with them
		ThrowPart(A, BitOf(A, 0), FVector(-150.f, RigRng.FRandRange(-200.f, 200.f), 2500.f), FVector(160.f, 300.f, 90.f));
		for (int32 s = 0; s < 2; ++s)
		{
			const float Y = s ? 1.f : -1.f;
			ThrowPart(A, BitOf(A, 1 + s), FVector(-500.f, Y * 1500.f, 1300.f), FVector(300.f, 700.f, 200.f));
			ThrowPart(A, BitOf(A, 3 + s), FVector(-300.f, Y * 1100.f, 1700.f), FVector(500.f, 100.f, 100.f));
		}
		FTFx::Burst(&A, TEXT("NS_BossPillar"), A.Mesh->GetComponentTransform().TransformPosition(JuggTurret), FRotator::ZeroRotator, 1.5f, FTFx::Main);
		// what is left is his: the turret's ring burns like a torch, and both barrel bays hang open with their load
		AFTWreck* Wr = D.Body.Get();
		WreckLoop(Wr, TEXT("NS_BossWreckFire"), JuggTurret + FVector(0.f, 0.f, 20.f), FRotator::ZeroRotator, 2.2f);
		for (int32 s = 0; s < 2; ++s)
		{
			const float Y = s ? 1.f : -1.f;
			WreckBit(Wr, Kept(TEXT("/Game/Characters/Bosses/SM_JuggTray")), FVector(JuggTray.X - JuggTrayRun, Y * JuggTray.Y, JuggTray.Z), FRotator(-14.f, 0.f, 0.f));
			WreckBit(Wr, Kept(TEXT("/Game/Characters/Pickups/SM_Barrel")), FVector(JuggTray.X - JuggTrayRun + 50.f, Y * JuggTray.Y, JuggTray.Z - 12.f), FRotator(-14.f, 0.f, 0.f));
		}
		// then the bays cook off, the left and then the right, and last of all his fuel
		for (int32 s = 0; s < 2; ++s)
			Later(A, 0.1f + 0.12f * s, [D, s]()
			{
				const FVector Bay(JuggTray.X - 150.f, (s ? 1.f : -1.f) * JuggTray.Y, 120.f);
				Pop(D, TEXT("NS_BossBreak"), Bay, 1.5f);
				Pop(D, TEXT("NS_BossBreakShards"), Bay, 1.2f, FTFx::Extra);
			});
		Later(A, 0.75f, [D]() { Pop(D, TEXT("NS_BossPillar"), FVector(200.f, 0.f, 200.f), 1.9f); });
	}

	// ---------------------------------------------------------------- THE ALCHEMIST (level 2): the Tesla truck
	// Bits: 0 the toroid (SM_TeslaTop, or SM_TeslaTopB once it has melted down), 1-3 the strikes (SetTesla),
	// 4-6 the charge collars, 7-8 the arcs to its own electrodes. BossFx: 0 the crackle round the toroid,
	// 1 smoke (damage 1), 2 fire (damage 2). RigA the toroid's spin, RigB its wobble, RigC since the last strike
	// ended, RigD how much power the lights have (0..1: the entrance, the moment after a strike).
	const FVector TeslaBall[2] = { FVector(-185.f, -336.f, 600.f), FVector(-185.f, 336.f, 600.f) };   // the spark gaps' electrodes (ft_v33.tesla_truck)
	constexpr float CollarLow = 335.f, CollarHigh = 800.f;                                              // the winding the collars climb
	void SetupAlchemist(AFTEntityActor& A)
	{
		for (int32 c = 0; c < 3; ++c) Show(A.Part(4 + c, TEXT("/Game/Characters/Bosses/SM_TeslaCollar"), FVector(TeslaTop.X, 0.f, CollarLow), false), false);
		for (int32 s = 0; s < 2; ++s) Show(A.Part(7 + s, TEXT("/Game/Characters/Bosses/SM_TeslaArc"), TeslaTop, false), false);
		Kept(TEXT("/Game/Characters/Bosses/SM_TeslaTopB"));   // (the meltdown's toroid: loaded now, not in the middle of the fight)
		A.RigD = 1.f;
	}
	void AnimAlchemist(AFTEntityActor& A, float Dt)
	{
		const float Ch = A.TeslaCharge;
		// the entrance: dead, then the lights stutter on; after a strike the coil is spent for a moment
		const float Boot = A.bBossLive ? (A.IntroT < 0.35f ? 0.f : (A.IntroT < 1.15f ? (FMath::Sin(A.IntroT * 47.f) > -0.2f ? Smooth((A.IntroT - 0.35f) / 0.8f) : 0.f) : 1.f)) : 1.f;
		const float Spent = (A.NumCharges == 0 && A.RigC < 0.5f) ? 1.f - Smooth(A.RigC / 0.5f) : 0.f;   // (never while another strike is charging)
		A.RigD = Boot * (1.f - 0.75f * Spent);
		// (round two, to read from 160 m: the toroid winds up out of the coil as the power comes on, and the reactor
		// vents two big clouds of steam to either side as it starts: white, and nothing his strikes look like)
		const float Rise = A.bBossLive ? Smooth((A.IntroT - 0.35f) / 0.8f) : 1.f;
		if (Crossed(A, Dt, 0.35f))
			for (int32 s = 0; s < 2; ++s) FTFx::Attach(TEXT("NS_BossSteam"), A.Mesh, FVector(TeslaTop.X, s ? 260.f : -260.f, 330.f), FRotator::ZeroRotator, 3.f, FTFx::Extra, true);
		// the collars: one for each strike still charging, climbing the winding at the pace of its own warning
		for (int32 c = 0; c < 3; ++c)
		{
			UStaticMeshComponent* C = BitOf(A, 4 + c);
			const bool bOn = C && c < A.NumCharges && A.Charges[c] > 0.02f;
			Show(C, bOn);
			if (!bOn) continue;
			const float U = A.Charges[c];
			C->SetRelativeLocation(FVector(TeslaTop.X, 0.f, FMath::Lerp(CollarLow, CollarHigh, U)));
			C->SetRelativeScale3D(FVector(1.f + 0.35f * U, 1.f + 0.35f * U, 1.f + 0.8f * U));
			C->SetScalarParameterValueOnMaterials(TEXT("Glow"), (1.f + 4.5f * U) * (U > 0.75f ? RigRng.FRandRange(0.75f, 1.25f) : 1.f));
		}
		// the toroid: spins up with the charge, rises a little, kicks flat as it lets go, and wobbles once it is damaged
		if (UStaticMeshComponent* Top = BitOf(A, 0))
		{
			A.RigA = FMath::Fmod(A.RigA + (25.f + 400.f * Ch) * Boot * Dt, 360.f);
			A.RigB = FMath::Fmod(A.RigB + (70.f + 110.f * A.DmgState) * Dt, 360.f);
			const float Tilt = (A.DmgState == 1 ? 4.f : (A.DmgState >= 2 ? 9.f : 0.f)) + 8.f * Pulse(A.HurtT, 7.f) * A.HurtAmt;
			const float Squash = Pulse(A.FireT, 8.f);
			const FQuat Q = FQuat(FVector::UpVector, FMath::DegreesToRadians(A.RigB)) * FQuat(FVector::ForwardVector, FMath::DegreesToRadians(Tilt))
				* FQuat(FVector::UpVector, FMath::DegreesToRadians(A.RigA - A.RigB));
			Top->SetRelativeLocationAndRotation(TeslaTop + FVector(0.f, 0.f, 12.f * Ch - 30.f * Squash - 190.f * (1.f - Rise)), Q);
			Top->SetRelativeScale3D(FVector(1.f + 0.05f * Ch + 0.16f * Squash, 1.f + 0.05f * Ch + 0.16f * Squash, 1.f - 0.32f * Squash));
		}
		// arcs from the toroid to its own two electrodes for the first instant of a strike
		const bool bSide = A.FireT < 0.14f && A.NumArcs > 0;
		for (int32 s = 0; s < 2; ++s)
		{
			if (bSide) LocalArc(BitOf(A, 7 + s), TeslaTop, TeslaBall[s], RigRng.FRandRange(1.1f, 1.9f));
			else Show(BitOf(A, 7 + s), false);
		}
		// every lit part of the truck follows the charge (the winding, the capacitor towers, the eyes): hot as it
		// peaks, dark for a moment after it has let go, and never quite steady once it is melting down
		A.BossAlert = FMath::Lerp(-0.4f, 1.5f * Ch * Ch + (A.DmgState >= 2 ? 0.5f : 0.f), A.RigD);
		// the crackle is the charge's (full size from 30 %); once it is melting down a small one never stops
		A.Loop(0, TEXT("NS_BossZapCrackle"), TeslaTop - FVector(0.f, 0.f, 190.f * (1.f - Rise)), A.bBossLive && (Ch > 0.3f || A.DmgState >= 2), FRotator::ZeroRotator, Ch > 0.3f ? 1.f : 0.5f);
	}
	void FireAlchemist(AFTEntityActor& A)
	{
		// the strike: a flash and a spray of arcs at the toroid, and the truck sits down under it
		FTFx::Attach(TEXT("NS_BossZapFlash"), A.Mesh, TeslaTop, FRotator::ZeroRotator, 1.f, FTFx::Flash, true);
		FTFx::Attach(TEXT("NS_BossZapSpray"), A.Mesh, TeslaTop, FRotator::ZeroRotator, 1.f, FTFx::Extra, true);
		Kick(A, 20.f, 0.f, -500.f);
	}
	void BreakAlchemist(AFTEntityActor& A, int32 State)
	{
		if (State == 1) A.Loop(1, TEXT("NS_BossSmoke"), FVector(-555.f, 186.f, 520.f), true);
		else
		{
			// the meltdown: a bite out of the toroid, the hot core showing; a capacitor tower burns
			// (SetTesla has left a dynamic instance of the whole toroid's material in every slot; the broken one has other slots)
			if (UStaticMeshComponent* Top = BitOf(A, 0)) if (UStaticMesh* Broken = Kept(TEXT("/Game/Characters/Bosses/SM_TeslaTopB"))) { Top->EmptyOverrideMaterials(); Top->SetStaticMesh(Broken); }
			A.Loop(2, TEXT("NS_BossFlame"), FVector(-555.f, -186.f, 500.f), true);
		}
		FTFx::Attach(TEXT("NS_BossZapSpray"), A.Mesh, TeslaTop, FRotator::ZeroRotator, 1.3f, FTFx::Main, true);
	}
	void DefeatAlchemist(AFTEntityActor& A, const FDeath& D)
	{
		// the coil lets go of everything it holds: the toroid is shot straight up off the winding, still crackling
		// (it climbs over the road where the camera sees it), in a spray of arcs three times a strike's
		if (AFTWreck* Top = ThrowPart(A, BitOf(A, 0), FVector(-200.f, RigRng.FRandRange(-250.f, 250.f), 2700.f), FVector(RigRng.FRandRange(-200.f, 200.f), 340.f, RigRng.FRandRange(-160.f, 160.f))))
			WreckLoop(Top, TEXT("NS_BossZapCrackle"), FVector::ZeroVector, FRotator::ZeroRotator, 1.4f);
		FTFx::Burst(&A, TEXT("NS_BossZapSpray"), A.Mesh->GetComponentTransform().TransformPosition(TeslaTop), FRotator::ZeroRotator, 3.f, FTFx::Main);
		// the hulk keeps arcing: the winding's stump crackles all the way down the road, both capacitor towers burn
		AFTWreck* Wr = D.Body.Get();
		WreckLoop(Wr, TEXT("NS_BossZapCrackle"), FVector(TeslaTop.X, 0.f, 620.f), FRotator::ZeroRotator, 1.3f);
		WreckLoop(Wr, TEXT("NS_BossWreckFire"), FVector(-555.f, 186.f, 500.f), FRotator::ZeroRotator, 1.5f);
		WreckLoop(Wr, TEXT("NS_BossWreckFire"), FVector(-555.f, -186.f, 500.f), FRotator::ZeroRotator, 1.5f);
		// three discharges up the winding, each higher and bigger than the last (about a third of a second apart as
		// the player sees them, in the wrap's slow motion: a count, not a strobe), then the capacitor bank
		for (int32 k = 0; k < 3; ++k)
			Later(A, 0.07f + 0.08f * k, [D, k]()
			{
				const FVector Coil(TeslaTop.X, 0.f, 500.f);
				Pop(D, TEXT("NS_BossZapFlash"), Coil, 1.2f + 0.5f * k, FTFx::Flash, 200.f * k);
				Pop(D, TEXT("NS_BossZapSpray"), Coil, 2.f + 0.6f * k, FTFx::Extra, 200.f * k);
			});
		Later(A, 0.7f, [D]()
		{
			Pop(D, TEXT("NS_BossDefeatZap"), FVector(-555.f, 0.f, 420.f), 1.5f);
			Pop(D, TEXT("NS_BossBreakShards"), FVector(-555.f, 0.f, 420.f), 1.6f, FTFx::Extra);
		});
	}

	// ---------------------------------------------------------------- THE BIG DEALER (level 3): the bike carrier
	// A launcher: the middle lane of each deck sends the riders. Bits: 0 the top deck's lift, 1-2 the air brakes
	// (left, right), 3-4 the dice, 5 the bike at the lower gate, 6 the bike on the lift, 7-9 the gate's launch
	// lights (amber, amber, green), 10-12 the lift's. BossFx: 0-1 tyre smoke while the brake check comes back at the
	// car, 2 smoke (damage 1), 3 fire (damage 2). RigA / RigB seconds since the gate / the lift sent a rider, RigD
	// how far the air brakes are out, RigE the dice's tumble. Wind kinds: 1 the gate, 2 the lift, 3 both.
	const FVector CarrierLift(30.f, 0.f, 314.f), CarrierGate(-230.f, 0.f, 116.f);
	const FVector CarrierDie[2] = { FVector(540.f, -120.f, 700.f), FVector(540.f, 120.f, 700.f) };
	constexpr float CarrierLiftUp = 183.f;   // a jumper leaves from 497 cm over the road (TickBoss); the top deck is at 314
	// the launch lights (round two: each lamp is a 72 cm square in an 80 cm hood now, so a count reads from 58 m): the
	// gate's three on the rear posts beside the lower deck, the lift's three above them on the masts
	// (ft_enemy_vehicles.carrier draws the posts and masts, carrier_lamp the lamps)
	constexpr float CarrierLampX = -477.f;
	const float CarrierLampZ[6] = { 284.f, 204.f, 124.f, 556.f, 476.f, 396.f };
	constexpr float CarrierBike = 1.25f;     // the bike waiting at the gate is drawn a quarter over life size: it is the next attack
	void SetupDealer(AFTEntityActor& A)
	{
		A.Part(0, TEXT("/Game/Characters/Bosses/SM_CarrierLift"), CarrierLift);
		for (int32 s = 0; s < 2; ++s)
		{
			A.Part(1 + s, TEXT("/Game/Characters/Bosses/SM_CarrierFlap"), FVector(-164.f, (s ? 1.f : -1.f) * 286.5f, 400.f));
			A.Part(3 + s, TEXT("/Game/Characters/Bosses/SM_CarrierDie"), CarrierDie[s])->SetRelativeRotation(FRotator(s ? 20.f : -20.f, 30.f, 15.f));
		}
		A.Part(5, TEXT("/Game/Characters/SnakeEyes/SM_Hog"), CarrierGate, false);
		A.Part(6, TEXT("/Game/Characters/SnakeEyes/SM_Hog2"), CarrierLift, false);
		for (int32 l = 0; l < 6; ++l)
		{
			// (the count is a column of burning squares that grows downward: amber, amber, green)
			UStaticMeshComponent* Lamp = A.Part(7 + l, l % 3 == 2 ? TEXT("/Game/Characters/Bosses/SM_CarrierLampG") : TEXT("/Game/Characters/Bosses/SM_CarrierLampA"), FVector(CarrierLampX, 0.f, CarrierLampZ[l]), false);
			Lamp->SetRelativeScale3D(FVector(1.f));
			Lamp->SetScalarParameterValueOnMaterials(TEXT("Glow"), 7.f);
			Show(Lamp, false);
		}
		A.RigA = A.RigB = 9.f;
		A.RigH = -1.f;   // (what the waiting bike's lights were last set to: none yet)
	}
	void AnimDealer(AFTEntityActor& A, float Dt)
	{
		A.RigA += Dt; A.RigB += Dt;
		const float W1 = (A.WindKind == 1 || A.WindKind == 3) ? A.Wind : 0.f;
		const float W2 = (A.WindKind == 2 || A.WindKind == 3) ? A.Wind : 0.f;
		const float Rev = FMath::Sin(A.AnimT * 52.f);
		// the gate: the next rider rolls back to the ramp's lip while his lights count down, tips onto the ramp, and is
		// gone (the real rider appears at the ramp's foot that instant); the slot stands empty a moment, then the next
		if (UStaticMeshComponent* G = BitOf(A, 5))
		{
			Show(G, A.RigA > 0.55f && G->GetStaticMesh() != nullptr);
			// (he stops at the lip, front wheel lifting: where on the road the real rider comes out, up to 365 cm to either
			// side, is only known as he goes, so nothing is shown riding down the middle of the ramp)
			const float Lip = Smooth(W1 / 0.8f), Tip = Smooth((W1 - 0.8f) / 0.2f);
			G->SetRelativeScale3D(FVector(CarrierBike * FMath::Max(Smooth((A.RigA - 0.55f) / 0.25f), 0.01f)));
			G->SetRelativeLocationAndRotation(FVector(CarrierGate.X - 240.f * Lip, 0.f, CarrierGate.Z + 2.f * Rev * W1 + 10.f * Tip),
				FRotator(9.f * Tip + 2.5f * Rev * W1, 0.f, 0.f));
			// (round two: his own lights burn, brighter as his count runs down, so he is a lit shape in the dark of the
			// gate and not a shadow in it; written in half steps, a write per step)
			const float Lit = FMath::RoundToFloat((1.f + 2.5f * W1) * 2.f) * 0.5f;
			if (Lit != A.RigH) { A.RigH = Lit; G->SetScalarParameterValueOnMaterials(TEXT("Alert"), Lit); }
		}
		// the lift: it carries the next jumper up through his wind-up (he leaves from 183 cm over the deck), holds a
		// moment after he has gone, and drops for the next
		const float Up = FMath::Max(Smooth(W2), A.RigB < 0.3f ? 1.f : 1.f - Smooth((A.RigB - 0.3f) / 0.45f));
		if (UStaticMeshComponent* L = BitOf(A, 0)) L->SetRelativeLocation(CarrierLift + FVector(0.f, 0.f, CarrierLiftUp * Up));
		if (UStaticMeshComponent* J = BitOf(A, 6))
		{
			Show(J, A.RigB > 0.95f && J->GetStaticMesh() != nullptr);
			J->SetRelativeScale3D(FVector(FMath::Max(Smooth((A.RigB - 0.95f) / 0.25f), 0.01f)));
			J->SetRelativeLocationAndRotation(CarrierLift + FVector(-30.f * Up, 0.f, CarrierLiftUp * Up + 2.f * Rev * W2), FRotator(14.f * Smooth(W2) + 2.5f * Rev * W2, 0.f, 0.f));
		}
		// the launch lights: two ambers count a rider down, green as he leaves
		const float Wd[2] = { W1, W2 }, Since[2] = { A.RigA, A.RigB };
		for (int32 d = 0; d < 2; ++d)
		{
			Show(BitOf(A, 7 + d * 3), Wd[d] > 0.3f);
			Show(BitOf(A, 8 + d * 3), Wd[d] > 0.65f);
			Show(BitOf(A, 9 + d * 3), Since[d] < 0.45f);
		}
		// the brake check: the air brakes swing out with its warning and stay out while he comes back at the car, tyres smoking
		const bool bBrake = A.Lunge > 0.02f;
		A.RigD = FMath::FInterpTo(A.RigD, bBrake ? 1.f : 0.f, Dt, bBrake ? 9.f : 4.f);
		for (int32 s = 0; s < 2; ++s)
			if (UStaticMeshComponent* F = BitOf(A, 1 + s))
				F->SetRelativeRotation(FRotator(0.f, (s ? -1.f : 1.f) * (72.f * A.RigD + (A.bLunge ? 3.f * FMath::Sin(A.AnimT * 37.f + s) : 0.f)), 0.f));
		A.Loop(0, TEXT("NS_BossTyreSmoke"), FVector(-335.f, -235.f, 30.f), A.bLunge);
		A.Loop(1, TEXT("NS_BossTyreSmoke"), FVector(-335.f, 235.f, 30.f), A.bLunge);
		// the dice tumble: lazily, faster as a rider winds up, a spin as he goes and when the carrier is hit
		// (the entrance: he rolls them as he drops back onto his spot)
		const float Roll = A.bBossLive ? 1.f - Smooth(A.IntroT / 2.6f) : 0.f;
		A.RigE += (40.f + 1500.f * Roll + 420.f * FMath::Max(W1, W2) + 900.f * Pulse(FMath::Min(A.RigA, A.RigB), 2.5f) + 500.f * Pulse(A.HurtT, 5.f) * A.HurtAmt) * Dt;
		// (round two: he throws them as he arrives: both dice go up six metres off their mast, half as big again at
		// the top, and come down onto it as he reaches his spot: two white cubes in the air read from 160 m)
		const float Toss = A.bBossLive ? FMath::Sin(PI * FMath::Clamp(A.IntroT / 1.5f, 0.f, 1.f)) : 0.f;
		for (int32 s = 0; s < 2; ++s)
			if (UStaticMeshComponent* D = BitOf(A, 3 + s))
			{
				D->SetRelativeLocationAndRotation(CarrierDie[s] + FVector(0.f, 0.f, (560.f + 110.f * s) * Toss),
					FQuat(FVector(s ? 0.3f : 0.5f, s ? 1.f : -0.6f, 0.7f).GetSafeNormal(), FMath::DegreesToRadians(A.RigE * (s ? 1.f : -1.3f) + (s ? 40.f : 0.f))));
				D->SetRelativeScale3D(FVector(1.f + 0.5f * Toss));
			}
	}
	void FireDealer(AFTEntityActor& A)
	{
		if (A.FireKind == 2)
		{
			A.RigB = 0.f;   // off the lift
			FTFx::Attach(TEXT("NS_BossLaunch"), A.Mesh, CarrierLift + FVector(0.f, 0.f, CarrierLiftUp), FRotator::ZeroRotator, 1.f, FTFx::Flash, true);
			Kick(A, 40.f, 0.f, -300.f);
		}
		else
		{
			A.RigA = 0.f;   // down the ramp: dust where the real rider is (Cue 3: how far to the side of the carrier he came out)
			FTFx::Attach(TEXT("NS_BossDust"), A.Mesh, FVector(-800.f, FMath::Clamp(A.Cue[3], -420.f, 420.f), 20.f), FRotator::ZeroRotator, 1.5f, FTFx::Extra, true);
			Kick(A, -30.f, 0.f, 160.f);
		}
	}
	void BreakDealer(AFTEntityActor& A, int32 State)
	{
		if (State == 1) A.Loop(2, TEXT("NS_BossSmoke"), FVector(-460.f, 266.f, 480.f), true);
		else
		{
			// he is losing: one of his dice is shot off its mast, and the lift's ram burns
			if (UStaticMeshComponent* D = BitOf(A, 4)) { FTFx::Attach(TEXT("NS_BossBreak"), A.Mesh, CarrierDie[1], FRotator::ZeroRotator, 0.6f, FTFx::Main, true); Show(D, false); }
			A.Loop(3, TEXT("NS_BossFlame"), FVector(30.f, 0.f, 330.f), true);
		}
	}
	void DefeatDealer(AFTEntityActor& A, const FDeath& D)
	{
		// the house loses: both dice go up high and clean (white, tumbling over the road where the camera sees them),
		// the bikes he never sent are thrown clear, the air brakes and the lift go their own ways, and his chips
		// fountain out of him
		for (int32 s = 0; s < 2; ++s)
		{
			const float Y = s ? 1.f : -1.f;
			ThrowPart(A, BitOf(A, 3 + s), FVector(-200.f, Y * 500.f, 2400.f + 300.f * s), FVector(420.f, 260.f * Y, 300.f), false);
			ThrowPart(A, BitOf(A, 1 + s), FVector(-600.f, Y * 1300.f, 900.f), FVector(120.f, 80.f, Y * 500.f));
		}
		ThrowPart(A, BitOf(A, 0), FVector(-300.f, 0.f, 1700.f), FVector(200.f, 320.f, 60.f));
		ThrowPart(A, BitOf(A, 5), FVector(-1500.f, -300.f, 900.f), FVector(100.f, 500.f, 200.f));
		ThrowPart(A, BitOf(A, 6), FVector(-400.f, 400.f, 2000.f), FVector(300.f, 400.f, 100.f));
		FTFx::Burst(&A, TEXT("NS_BossHurtChips"), A.Mesh->GetComponentTransform().TransformPosition(FVector(-300.f, 0.f, 420.f)), A.GetActorRotation(), 3.f, FTFx::Main);
		// the wreck is the carrier with every launch light burning at once, and the lift's ram alight
		AFTWreck* Wr = D.Body.Get();
		for (int32 l = 0; l < 6; ++l)
			if (UStaticMeshComponent* Lamp = WreckBit(Wr, Kept(l % 3 == 2 ? TEXT("/Game/Characters/Bosses/SM_CarrierLampG") : TEXT("/Game/Characters/Bosses/SM_CarrierLampA")), FVector(CarrierLampX, 0.f, CarrierLampZ[l]), FRotator::ZeroRotator, 0.f))
				Lamp->SetScalarParameterValueOnMaterials(TEXT("Glow"), 7.f);
		WreckLoop(Wr, TEXT("NS_BossWreckFire"), FVector(30.f, 0.f, 330.f), FRotator::ZeroRotator, 1.8f);
		// the chips keep coming, a bigger and higher fountain each time; a ring where the lift was; its ram blows last
		for (int32 k = 0; k < 3; ++k)
			Later(A, 0.1f + 0.14f * k, [D, k]() { Pop(D, TEXT("NS_BossHurtChips"), FVector(-300.f + 250.f * k, 0.f, 420.f), 3.f + 0.8f * k, FTFx::Main, 150.f * k); });
		Later(A, 0.3f, [D]() { Pop(D, TEXT("NS_BossLaunch"), FVector(30.f, 0.f, 500.f), 3.f, FTFx::Flash, 200.f); });
		Later(A, 0.8f, [D]() { Pop(D, TEXT("NS_BossBreak"), FVector(30.f, 0.f, 330.f), 1.6f); });
	}

	// ---------------------------------------------------------------- ENGINE 13 (level 4): the fire engine
	// Bits: 0 the fire cannon on its turntable, 1 the ladder (carried raised over the tail; its tip holds the second
	// nozzle). BossFx: 0 the cannon's jet, 1 the ladder's jet, 2 smoke (damage 1), 3 fire (damage 2). RigA / RigB the
	// cannon's turn and tip, RigD / RigE the ladder's swing and raise, RigG whether the hose was burning last frame,
	// RigH what the throats' glow was set to, RigN whether this wind-up has blown its steam.
	// Wind kinds: 1 the hose filling (the lane's own warning), 2 the hose burning (the lane is live), 3 fireballs
	// about to be lobbed; AimX = where the lane is across the road from him. Cue[1], Cue[2]: the second lane (five
	// alarms): where, and how far along (0 none, up to 1 warning, 2 live). Fire kinds: 3 the fireballs, 4 a barrel.
	const FVector FireCannonAt(-495.f, 0.f, 352.f), FireLadderAt(275.f, 0.f, 372.f), FireMuzzle(-232.f, 0.f, 96.f), FireTip(-858.f, 0.f, -68.f);
	const FVector EngineHatch(-205.f, 0.f, 440.f);   // the roof tank's hatch: the fireballs are lobbed from 420 cm up, over the truck's middle
	void SetupEngine(AFTEntityActor& A)
	{
		A.Part(0, TEXT("/Game/Characters/Bosses/SM_FireCannon"), FireCannonAt);
		A.Part(1, TEXT("/Game/Characters/Bosses/SM_FireLadder"), FireLadderAt)->SetRelativeRotation(FRotator(-25.f, 0.f, 0.f));
		A.RigE = -25.f;
	}
	void AnimEngine(AFTEntityActor& A, float Dt)
	{
		const bool bHose = (A.WindKind == 1 || A.WindKind == 2) && A.Wind > 0.f, bLive = A.WindKind == 2 && A.Wind > 0.5f;
		// (Lob: the fireballs' wind-up, whatever the hose is doing; Mortar: what of it the cannon is free to show)
		const float Lob = FMath::Clamp(A.Cue[3], 0.f, 1.f);
		const float Fill = A.WindKind == 1 ? A.Wind : (bLive ? 1.f : 0.f), Mortar = bHose ? 0.f : Lob;
		const float Thump = (A.FireKind == 3 && !bHose) ? Pulse(A.FireT, 6.f) : 0.f;   // (not while it is hosing: the jet stays on its lane)
		const float Rattle = Pulse(A.HurtT, 5.f) * A.HurtAmt * FMath::Sin(A.AnimT * 50.f);
		// the cannon turns at the lane it is about to burn and follows it when the hose sweeps; it tips up to lob
		// fireballs and is kicked back as they go; with nothing due it looks the road over
		A.RigA = FMath::FInterpTo(A.RigA, bHose ? -FMath::RadiansToDegrees(FMath::Atan2(A.AimX, 3500.f)) : 7.f * FMath::Sin(A.AnimT * 0.8f), Dt, bHose ? 8.f : 2.f);
		A.RigB = FMath::FInterpTo(A.RigB, -34.f * FMath::Max(Smooth(Mortar), Thump) + 5.f * Fill, Dt, 10.f);
		// (the entrance: it comes round from facing up the road to face the car, and the ladder goes up)
		const float In = A.bBossLive ? Smooth(A.IntroT / 1.4f) : 1.f;
		const FQuat CQ(FRotator(A.RigB + (bLive ? 1.5f * FMath::Sin(A.AnimT * 43.f) : 0.f) + 2.f * Rattle, A.RigA + 180.f * (1.f - In), 0.f));
		const FVector CP = FireCannonAt + CQ.RotateVector(FVector(26.f * Thump + (bLive ? 7.f : 0.f), 0.f, 0.f));
		if (UStaticMeshComponent* C = BitOf(A, 0)) C->SetRelativeLocationAndRotation(CP, CQ);
		A.Loop(0, TEXT("NS_BossFlameJet"), CP + CQ.RotateVector(FireMuzzle), bLive, CQ.Rotator());
		// the ladder rides raised over the tail and sways; at five alarms its nozzle burns the second lane
		const bool bSecond = A.Cue[2] > 0.f, bLive2 = A.Cue[2] > 1.5f;
		A.RigD = FMath::FInterpTo(A.RigD, bSecond ? -FMath::RadiansToDegrees(FMath::Atan2(A.Cue[1], 3500.f)) : 0.f, Dt, bSecond ? 8.f : 2.f);
		A.RigE = FMath::FInterpTo(A.RigE, -25.f - 9.f * FMath::Max(Smooth(Mortar), Thump) + 2.5f * FMath::Sin(A.AnimT * 1.7f), Dt, 6.f);
		// (round two: on the way in the ladder stands right up before it settles over the tail: nine metres of ladder
		// going up is a shape that reads from 160 m; it is down to its carry by the end of the entrance, as before)
		const FQuat LQ(FRotator(A.RigE * In - 40.f * FMath::Sin(PI * In) + 2.f * Rattle, A.RigD, 5.f * A.DmgState));   // (knocked askew as he is worn down)
		if (UStaticMeshComponent* L = BitOf(A, 1)) L->SetRelativeLocationAndRotation(FireLadderAt, LQ);
		A.Loop(1, TEXT("NS_BossFlameJet"), FireLadderAt + LQ.RotateVector(FireTip), bLive2, (LQ * FQuat(FRotator(45.f, 0.f, 0.f))).Rotator(), 0.8f);
		// pressure: the two throats and the tank's sight glass burn brighter as the hose fills or the mortar loads
		const float Second = bLive2 ? 1.f : FMath::Clamp(A.Cue[2], 0.f, 1.f);
		const float Heat = FMath::RoundToFloat((2.6f * FMath::Max3(Fill, Lob, Second) + (bLive || bLive2 ? 2.f : 0.f)) * 4.f) * 0.25f;
		if (Heat != A.RigH)
		{
			A.RigH = Heat;
			for (int32 b = 0; b < 2; ++b) if (UStaticMeshComponent* C = BitOf(A, b)) C->SetScalarParameterValueOnMaterials(TEXT("Alert"), Heat);
		}
		A.BossAlert = 1.2f * FMath::Max3(Fill, Lob, Second);
		// the fireballs' own sign: a pilot flame stands on the roof tank through their wind-up, taller as it runs out
		A.Loop(4, TEXT("NS_BossFlameJet"), EngineHatch, Lob > 0.04f, FRotator(-90.f, 0.f, 0.f), 0.22f + 0.3f * Lob);
		// a wind-up starts with the safety valve blowing off; the hose catching kicks the truck
		const float W = FMath::Max3(Fill, Lob, Second);
		if (W > 0.05f && A.RigN == 0) { A.RigN = 1; FTFx::Attach(TEXT("NS_BossSteam"), A.Mesh, FVector(-205.f, 0.f, 430.f), FRotator::ZeroRotator, 1.f, FTFx::Extra, true); }
		else if (W < 0.02f) A.RigN = 0;
		if (bLive && A.RigG < 0.5f) Kick(A, 45.f, 0.f, -200.f);
		A.RigG = bLive ? 1.f : 0.f;
	}
	void FireEngine(AFTEntityActor& A)
	{
		if (A.FireKind == 3)
		{
			// the fireballs go: a ring where they leave from (the cannon's mouth if it was free to tip up, else the tank's hatch)
			const bool bHose = (A.WindKind == 1 || A.WindKind == 2) && A.Wind > 0.f;
			UStaticMeshComponent* C = bHose ? nullptr : BitOf(A, 0);
			if (C) FTFx::Attach(TEXT("NS_BossMortar"), C, FireMuzzle, FRotator::ZeroRotator, 1.f, FTFx::Flash, true);
			else FTFx::Attach(TEXT("NS_BossMortar"), A.Mesh, EngineHatch, FRotator::ZeroRotator, 1.f, FTFx::Flash, true);
			Kick(A, 70.f, 0.f, -260.f);
		}
		else FTFx::Attach(TEXT("NS_BossDust"), A.Mesh, FVector(-800.f, 0.f, 20.f), FRotator::ZeroRotator, 0.8f, FTFx::Extra, true);
	}
	void BreakEngine(AFTEntityActor& A, int32 State)
	{
		// the fuel tank on the roof is holed, then alight
		if (State == 1) A.Loop(2, TEXT("NS_BossSmoke"), FVector(-205.f, 60.f, 420.f), true);
		else A.Loop(3, TEXT("NS_BossFlame"), FVector(-205.f, -40.f, 420.f), true);
	}
	void DefeatEngine(AFTEntityActor& A, const FDeath& D)
	{
		// the tank goes up: the cannon is blown off its turntable on a pillar of fire and the ladder goes over the
		// side, the boiler lets go its steam, and what is left rolls on down the road with the tank's hatch a jet of flame
		ThrowPart(A, BitOf(A, 0), FVector(-600.f, RigRng.FRandRange(-300.f, 300.f), 2300.f), FVector(200.f, 380.f, 120.f));
		ThrowPart(A, BitOf(A, 1), FVector(200.f, 1100.f, 1500.f), FVector(160.f, 60.f, 320.f));
		const FTransform Body = A.Mesh->GetComponentTransform();
		FTFx::Burst(&A, TEXT("NS_BossPillar"), Body.TransformPosition(EngineHatch), FRotator::ZeroRotator, 1.8f, FTFx::Main);
		FTFx::Burst(&A, TEXT("NS_BossSteam"), Body.TransformPosition(FVector(300.f, 0.f, 330.f)), FRotator::ZeroRotator, 3.f, FTFx::Extra);
		AFTWreck* Wr = D.Body.Get();
		WreckLoop(Wr, TEXT("NS_BossFlameJet"), EngineHatch, FRotator(-90.f, 0.f, 0.f), 0.8f);
		WreckLoop(Wr, TEXT("NS_BossWreckFire"), FireCannonAt, FRotator::ZeroRotator, 1.8f);
		// then the tank itself, in a ring and a fireball over the wreck; more steam; and the last of the fuel
		Later(A, 0.12f, [D]()
		{
			Pop(D, TEXT("NS_BossMortar"), EngineHatch, 3.f, FTFx::Flash, 150.f);
			Pop(D, TEXT("NS_BossDefeatFire"), EngineHatch, 1.2f, FTFx::Main, 300.f);
		});
		Later(A, 0.3f, [D]() { Pop(D, TEXT("NS_BossSteam"), FVector(300.f, 0.f, 300.f), 3.5f, FTFx::Extra); });
		Later(A, 0.8f, [D]() { Pop(D, TEXT("NS_BossPillar"), FVector(-205.f, 0.f, 300.f), 2.2f); });
	}

	// ---------------------------------------------------------------- MR. CHUCKLES (level 5): the flying clown car
	// He flies: the car hangs off the road under the rig, nose down, its wheels dangling. Bits: 0 the rig, 1-4 the
	// propellers (as 3.3), 5 the bomb balloons (SM_ClownBalloons; B at damage 1, C at 2), 6 the head on its spring,
	// 7 the jaw, 8 the pie oven's lid, 9 the pies in it. BossFx: 0 smoke off the coughing motor (damage 1), 1 fire on
	// it (damage 2). RigA / RigF / RigG the propellers' turn (the good ones, rear left, front right), RigB the laugh
	// he has left, RigD the oven's lid, RigE how far he hangs off the road, RigH the nose's glow.
	// Wind kinds: 1 bombs, 2 pies, 3 the bomb carpet. The same model is the coaster's flyer (TickFlyer): it gets no
	// cues, only the flight.
	const FVector ClownNeck(-62.5f, 0.f, 330.f), ClownWinch(-437.5f, 0.f, 424.f), ClownHinge(-608.f, 0.f, 334.f), ClownOven(-618.f, 0.f, 286.f);
	const FVector ClownChute(-668.f, 0.f, 150.f);   // where the pies leave from: behind the tail, 150 cm over the road (Z is over the ROAD here)
	/** The wheels of a boss that has left the road hang from its body (they are the actor's, not the body's). */
	void HangWheels(AFTEntityActor& A, float Droop, float Turn)
	{
		const FTransform Body = A.Mesh->GetRelativeTransform();
		for (int32 i = 0; i < A.WheelSpec.Num() && i < A.Wheels.Num(); ++i)
			if (UStaticMeshComponent* W = A.Wheels[i])
				W->SetRelativeLocationAndRotation(Body.TransformPosition(A.WheelSpec[i].C - FVector(0.f, 0.f, Droop)), Body.GetRotation() * FQuat(FRotator(-Turn, 0.f, 0.f)));
	}
	void SetupChuckles(AFTEntityActor& A)
	{
		A.Part(5, TEXT("/Game/Characters/Bosses/SM_ClownBalloons"), ClownWinch)->SetRelativeRotation(FRotator(12.f, 0.f, 0.f));
		A.Part(6, TEXT("/Game/Characters/Bosses/SM_ClownHead"), ClownNeck);
		A.Part(7, TEXT("/Game/Characters/Bosses/SM_ClownJaw"), ClownNeck, false);
		A.Part(8, TEXT("/Game/Characters/Bosses/SM_ClownLid"), ClownHinge, false);
		Kept(TEXT("/Game/Characters/Bosses/SM_ClownBalloonsB")); Kept(TEXT("/Game/Characters/Bosses/SM_ClownBalloonsC"));   // (loaded now, not as they burst)
		Show(A.Part(9, TEXT("/Game/Characters/Bosses/SM_ClownPies"), ClownOven, false), false);
		// a pose that stands by itself (the reel's ghost is never animated): in the air, the discs leaning into their work
		for (int32 p = 0; p < NumClownProps; ++p) if (UStaticMeshComponent* Pc = BitOf(A, 1 + p)) Pc->SetRelativeRotation(FRotator(-24.f, 40.f * p, 0.f));
		A.RigE = 95.f;
		A.Mesh->SetRelativeTransform(FTransform(FRotator(-5.f, 0.f, 0.f), FVector(0.f, 0.f, A.RigE)));
		HangWheels(A, 22.f, 0.f);
	}
	void AnimChuckles(AFTEntityActor& A, float Dt, float Vx)
	{
		const float T = A.AnimT;
		const float W1 = (A.WindKind == 1 || A.WindKind == 3) ? A.Wind : 0.f, W2 = A.WindKind == 2 ? A.Wind : 0.f, W = FMath::Max(W1, W2);
		const float Hit = Pulse(A.HurtT, 5.f) * A.HurtAmt;
		// flight: lower and listing as the motors go, knocked down by a hit; in the fight he comes down out of the sky
		const float Target = A.DmgState == 0 ? 95.f : (A.DmgState == 1 ? 78.f : 48.f);
		A.RigE = FMath::FInterpTo(A.RigE, Target - 45.f * Hit, Dt, 3.f);
		const float Drop = A.bBossLive ? 900.f * FMath::Square(1.f - Smooth(A.IntroT / 2.2f)) : 0.f;
		const float List = A.DmgState >= 2 ? 9.f : (A.DmgState == 1 ? 3.5f : 0.f);
		const float Cough = A.DmgState >= 1 ? FMath::Max(0.f, FMath::Sin(T * 5.3f)) * FMath::Max(0.f, FMath::Sin(T * 1.7f)) : 0.f;   // the bad motor
		const FRotator Rot(-5.f + 3.f * FMath::Sin(T * 2.7f) - 5.f * W1 + 7.f * Pulse(A.FireT, 5.f), 2.5f * FMath::Sin(T * 1.3f),
			6.f * FMath::Sin(T * 1.9f) + FMath::Clamp(Vx / 70.f, -10.f, 10.f) + List + 5.f * Cough);
		const float Height = A.RigE + Drop + 20.f * FMath::Sin(T * 2.3f) + 8.f * FMath::Sin(T * 5.1f) - 14.f * Cough;
		A.Mesh->SetRelativeTransform(FTransform(Rot, FVector(0.f, 0.f, Height)));
		HangWheels(A, 22.f, T * 140.f);
		// how hard the motors are working: a hit, the bad motor's cough, arming the bombs, and climbing back to his height
		const float Strain = FMath::Clamp(1.2f * Hit + 0.6f * Cough + 0.5f * W1 + FMath::Max(0.f, Target - A.RigE) / 40.f, 0.f, 1.5f);
		A.RigC += 2.f * Strain * Dt;   // (the discs' wobble has its own clock: it runs up to three times as fast under strain)
		// the rig flexes over the car: it stays nearer level while the car swings under it, and lifts as the motors strain
		const FVector RigPivot(0.f, 0.f, 470.f);
		const FQuat RigQ(FRotator(-0.35f * (Rot.Pitch + 5.f), 0.f, -0.45f * (Rot.Roll - List)));
		const FVector RigAt = RigPivot - RigQ.RotateVector(RigPivot) + FVector(0.f, 0.f, 5.f + 9.f * Strain);
		if (UStaticMeshComponent* Rg = BitOf(A, 0)) Rg->SetRelativeLocationAndRotation(RigAt, RigQ);
		// his downwash blows the road clear under him (the fight's boss only: the coaster's flyer has no road beneath)
		A.Loop(2, TEXT("NS_BossTyreSmoke"), FVector(0.f, 0.f, 12.f - Height), A.bBossLive && Height < 420.f, FRotator::ZeroRotator, 1.7f);
		// the propellers: each disc leans into its work and they never quite agree (the rig is fighting to hold him
		// up); the rear left coughs at damage 1 and stops at 2, when the front right starts to cough
		const float Rpm = 2200.f * (1.f + 0.5f * Strain);   // (the good motors race when he strains)
		A.RigA += Rpm * Dt;
		A.RigF += Rpm * Dt * (A.DmgState >= 2 ? 0.03f : (A.DmgState == 1 ? 0.3f + 0.6f * (1.f - Cough) : 1.f));
		A.RigG += Rpm * Dt * (A.DmgState >= 2 ? 0.3f + 0.6f * (1.f - Cough) : 1.f);
		for (int32 p = 0; p < NumClownProps; ++p)
			if (UStaticMeshComponent* Pc = BitOf(A, 1 + p))
			{
				const float Turn = (p == 0 ? A.RigF : (p == 3 ? A.RigG : A.RigA)) * (p % 2 ? 1.f : -1.f);
				const float Lean = -24.f - (7.f + 4.f * Strain) * FMath::Sin(A.RigC * 3.1f + p * 1.7f) - 8.f * W1 + 10.f * Hit;
				const float Side = (p % 2 ? -1.f : 1.f) * (4.f + (3.f + 3.f * Strain) * FMath::Sin(A.RigC * 2.3f + p));
				Pc->SetRelativeLocationAndRotation(RigAt + RigQ.RotateVector(ClownProps[p]), RigQ * FQuat(FRotator(Lean, 0.f, Side)) * FQuat(FVector::UpVector, FMath::DegreesToRadians(Turn)));
			}
		A.Loop(0, TEXT("NS_BossSmoke"), RigAt + RigQ.RotateVector(ClownProps[0] + FVector(0.f, 0.f, -30.f)), A.DmgState >= 1);
		A.Loop(1, TEXT("NS_BossFlame"), RigAt + RigQ.RotateVector(ClownProps[0] + FVector(0.f, 0.f, -50.f)), A.DmgState >= 2);
		// the bomb balloons stream back off their winch and stay upright while the car swings; the bunch strains up
		// as the bombs arm and jumps as they go
		if (UStaticMeshComponent* Bl = BitOf(A, 5))
		{
			const float Jump = (A.FireKind == 1 || A.FireKind == 3) ? Pulse(A.FireT, 6.f) : 0.f;
			Bl->SetRelativeRotation(FRotator(12.f - 9.f * W1 + 4.f * FMath::Sin(T * 1.6f), 0.f, 6.f * FMath::Sin(T * 1.1f) - Rot.Roll));
			Bl->SetRelativeScale3D(FVector(1.f, 1.f, 1.f + 0.07f * W1 + 0.12f * Jump));
		}
		// the head rides its spring: it wags against the car's swing, glares down at the car through a wind-up with
		// its nose burning brighter, and rears back to laugh as the attack leaves (the jaw drops for the laugh, and
		// when he is hit)
		A.RigB = FMath::Max(0.f, A.RigB - Dt);
		const float Laugh = FMath::Min(1.f, A.RigB / 0.25f);
		const FRotator HeadRot(4.f * FMath::Sin(T * 3.3f) + 9.f * W - 16.f * Laugh + 5.f * Laugh * FMath::Sin(T * 38.f) + 14.f * Hit, 6.f * FMath::Sin(T * 0.9f), -1.4f * Rot.Roll);
		const float Open = FMath::Max(Laugh * (0.6f + 0.4f * FMath::Abs(FMath::Sin(T * 19.f))), FMath::Max(0.55f * Hit, 0.35f * W));
		// (round two, the entrance: a jack-in-the-box. As he comes down out of the sky his head is thrown two and a half
		// metres up its spring and bounces down onto it, smaller each time; it has settled before he is on his spot)
		const FVector Neck = ClownNeck + FVector(0.f, 0.f, A.bBossLive ? 250.f * FMath::Exp(-2.2f * A.IntroT) * FMath::Abs(FMath::Cos(A.IntroT * 7.f)) : 0.f);
		if (UStaticMeshComponent* H = BitOf(A, 6))
		{
			H->SetRelativeLocationAndRotation(Neck, HeadRot);
			const float Nose = FMath::RoundToFloat(4.f * W) * 0.55f;
			if (Nose != A.RigH) { A.RigH = Nose; H->SetScalarParameterValueOnMaterials(TEXT("Alert"), Nose); }
		}
		if (UStaticMeshComponent* J = BitOf(A, 7)) J->SetRelativeLocationAndRotation(Neck + HeadRot.RotateVector(FVector(0.f, 0.f, -62.f * Open)), HeadRot);
		// the pie oven: its lid swings up through the wind-up of a fan of pies and shows them racked; shut once they have gone
		A.RigD = FMath::FInterpTo(A.RigD, W2 > 0.05f ? 1.f : 0.f, Dt, W2 > 0.05f ? 7.f : 5.f);
		if (UStaticMeshComponent* Ld = BitOf(A, 8)) Ld->SetRelativeRotation(FRotator(-112.f * A.RigD, 0.f, 0.f));
		if (UStaticMeshComponent* Pies = BitOf(A, 9))
		{
			// (the real pies leave from 150 cm over the road: the rack comes down out of the oven to that height as the
			// wind-up runs out, so they go from where it is)
			Show(Pies, W2 > 0.12f && A.RigD > 0.3f && Pies->GetStaticMesh() != nullptr);
			const float Out = Smooth((W2 - 0.3f) / 0.6f);
			Pies->SetRelativeLocation(FVector(FMath::Lerp(float(ClownOven.X), float(ClownChute.X), Out), 0.f, FMath::Lerp(float(ClownOven.Z), float(ClownChute.Z) - A.RigE, Out)));
		}
	}
	void FireChuckles(AFTEntityActor& A)
	{
		A.RigB = 0.7f;   // he laughs
		if (A.FireKind == 2) FTFx::Attach(TEXT("NS_BossPieBlast"), A.Mesh, FVector(ClownChute.X - 30.f, 0.f, ClownChute.Z - A.RigE), FRotator::ZeroRotator, 1.f, FTFx::Flash, true);
		else FTFx::Attach(TEXT("NS_BossConfetti"), A.Mesh, ClownWinch + FVector(-150.f, 0.f, 900.f), FRotator::ZeroRotator, A.FireKind == 3 ? 1.5f : 1.f, FTFx::Extra, true);
	}
	void BreakChuckles(AFTEntityActor& A, int32 State)
	{
		// balloons burst: eight, then five, then three
		if (UStaticMeshComponent* Bl = BitOf(A, 5))
			if (UStaticMesh* Fewer = Kept(State == 1 ? TEXT("/Game/Characters/Bosses/SM_ClownBalloonsB") : TEXT("/Game/Characters/Bosses/SM_ClownBalloonsC"))) Bl->SetStaticMesh(Fewer);
		for (int32 k = 0; k < 3; ++k)
			FTFx::Attach(TEXT("NS_BossPop"), A.Mesh, ClownWinch + FVector(-150.f + 90.f * k, -200.f + 200.f * k, 880.f + 60.f * k), FRotator::ZeroRotator, 1.f, FTFx::Main, true);
	}
	void DefeatChuckles(AFTEntityActor& A, const FDeath& D)
	{
		// the party is over: the four propellers spin off their motors and the rig goes after them, his head shoots
		// up off its spring (clean, not burnt: it is the last of him anyone sees, high over the road), and every
		// balloon he has left bursts in turn, nine metres up
		for (int32 p = 0; p < NumClownProps; ++p)
			ThrowPart(A, BitOf(A, 1 + p), FVector(p < 2 ? -900.f : 700.f, (p % 2 ? 1.f : -1.f) * 1400.f, 1300.f), FVector(100.f, 80.f, (p % 2 ? 1.f : -1.f) * 1200.f));
		ThrowPart(A, BitOf(A, 0), FVector(300.f, RigRng.FRandRange(-300.f, 300.f), 1500.f), FVector(60.f, 240.f, 120.f));
		ThrowPart(A, BitOf(A, 6), FVector(-300.f, 0.f, 2800.f), FVector(520.f, 60.f, 90.f), false);
		const FTransform Body = A.Mesh->GetComponentTransform();
		const FVector Bunch = ClownWinch + FVector(-150.f, 0.f, 900.f);
		FTFx::Burst(&A, TEXT("NS_BossConfetti"), Body.TransformPosition(Bunch - FVector(0.f, 0.f, 300.f)), FRotator::ZeroRotator, 3.f, FTFx::Main);
		const int32 Left = A.DmgState >= 2 ? 3 : (A.DmgState == 1 ? 5 : 8);
		for (int32 k = 0; k < Left; ++k)
		{
			// (0.075 s of the game's clock apart: a third of a second as the player sees it in the wrap's slow motion)
			const FVector At = Bunch + FVector(-90.f + 60.f * (k % 4), -210.f + 140.f * (k % 4), 60.f * (k / 4) + 40.f * (k % 3));
			if (k == 0) FTFx::Burst(&A, TEXT("NS_BossPop"), Body.TransformPosition(At), FRotator::ZeroRotator, 1.6f, FTFx::Main);
			else Later(A, 0.075f * k, [D, At]() { Pop(D, TEXT("NS_BossPop"), At, 1.6f); });
		}
		// the car goes on down the road spilling what was in it: the pies all at once, and confetti in clouds
		Later(A, 0.2f, [D]() { Pop(D, TEXT("NS_BossPieBlast"), ClownOven, 3.f, FTFx::Main, 100.f); });
		for (int32 k = 0; k < 4; ++k)
			Later(A, 0.15f + 0.22f * k, [D, k]() { Pop(D, TEXT("NS_BossHurtConfetti"), FVector(-300.f, 0.f, 330.f), 3.f, FTFx::Main, 120.f * k); });
	}

	// ---------------------------------------------------------------- THE ABOMINABLE (level 6): the yeti
	// His two arms move on their own now. Bits: 0 the left arm (-Y), 1 the right (their shoulders are 285 cm either
	// side of the old pivot), 2-4 the ice he wears (left shoulder, right shoulder, the back of his neck), 5 the
	// snowball he packs. BossFx: 0 the snow he kicks up when he runs at the car. RigH what his eyes' glow was set to.
	// Wind kinds: 1 snowballs, 2 the ground pound, 3 the avalanche (his roar). He faces the car (-X) through the fight.
	const FVector YetiShoulder[2] = { FVector(0.f, -285.f, AFTEntityActor::YetiShoulderZ), FVector(0.f, 285.f, AFTEntityActor::YetiShoulderZ) };
	const FVector YetiIceAt[3] = { FVector(20.f, -270.f, 960.f), FVector(20.f, 270.f, 960.f), FVector(110.f, 0.f, 1090.f) };
	void SetupYeti(AFTEntityActor& A)
	{
		UStaticMeshComponent* L = A.Part(0, TEXT("/Game/Characters/Bosses/SM_YetiArmL"), YetiShoulder[0]);
		UStaticMeshComponent* R = A.Part(1, TEXT("/Game/Characters/Bosses/SM_YetiArmR"), YetiShoulder[1]);
		// (without the two new meshes he keeps the 2.8 arms: one mesh, one pivot)
		const bool bTwo = L->GetStaticMesh() && R->GetStaticMesh();
		if (bTwo) A.Extra->SetVisibility(false);
		Show(L, bTwo); Show(R, bTwo);
		for (int32 i = 0; i < 3; ++i)
		{
			UStaticMeshComponent* Ice = A.Part(2 + i, TEXT("/Game/Characters/Bosses/SM_YetiIce"), YetiIceAt[i], false);
			Ice->SetRelativeRotation(i == 2 ? FRotator(-30.f, 0.f, 0.f) : FRotator(0.f, 0.f, i == 0 ? -18.f : 18.f));
			if (i == 2) Ice->SetRelativeScale3D(FVector(1.35f));
		}
		Show(A.Part(5, TEXT("/Game/Characters/Projectiles/SM_Snowball"), FVector(0.f, 0.f, 1500.f), false), false);
	}
	void AnimYeti(AFTEntityActor& A, float Dt)
	{
		A.YetiPhase += Dt * (1.f + 0.7f * A.Charge);
		const float P = A.YetiPhase;
		const float W1 = A.WindKind == 1 ? Smooth(A.Wind) : 0.f, W2 = A.WindKind == 2 ? Smooth(A.Wind) : 0.f, W3 = A.WindKind == 3 ? Smooth(A.Wind) : 0.f;
		const float Hit = Pulse(A.HurtT, 5.f) * A.HurtAmt;
		const float Throw = A.FireKind == 1 ? Pulse(A.FireT, 4.f) : 0.f;
		// the pound: his fists go down as the ice starts along the lane and stay down while it runs (about its warning)
		// (down for as long as the ice he made is on the road, Cue 0; arms wide for as long as his avalanche is, Cue 1)
		A.RigD = FMath::FInterpTo(A.RigD, A.Cue[0] > 0.f ? 1.f : 0.f, Dt, A.Cue[0] > 0.f ? 16.f : 5.f);
		A.RigE = FMath::FInterpTo(A.RigE, A.Cue[1] > 0.f ? 1.f : 0.f, Dt, 6.f);
		const float Slam = Smooth(A.RigD);
		const float Roar = FMath::Max(W3, Smooth(A.RigE));
		const float Plant = FMath::Max3(FMath::Max(W1, W2), Slam, Roar);              // he plants his feet for all of these
		const bool bWayIn = A.Cue[2] > 0.5f;                                            // the chase in from behind (he faces down the run): the fight's own state
		// the body: the lope (as 2.8) unless his feet are planted; reared back to wind up, bent double by the slam,
		// leaning at the car when he charges, flinching from a hit
		const FRotator Body(-5.f * FMath::Sin(P * 6.f) * (1.f - Plant) + A.Charge * (bWayIn ? -9.f : 11.f) - 9.f * FMath::Max(W1, W2) + 17.f * Slam + 12.f * Throw - 7.f * Roar - 8.f * Hit,
			0.f, 5.f * FMath::Sin(P * 3.f) * (1.f - Plant));
		// (round two, the entrance: he comes in with one great bound, three and a half metres clear of the snow, and is
		// down again 1.2 s after he appears: ten metres of yeti in the air reads from 160 m. No attack of his leaves the ground)
		const float Bound = A.bBossLive ? 350.f * FMath::Sin(PI * FMath::Clamp(A.IntroT / 1.2f, 0.f, 1.f)) : 0.f;
		A.Mesh->SetRelativeTransform(FTransform(Body, FVector(0.f, 0.f, 90.f * FMath::Abs(FMath::Sin(P * 3.f)) * (1.f - 0.8f * Plant) - 50.f * Slam + Bound)));
		if (A.Extra->IsVisible())
			A.Extra->SetRelativeRotation(FRotator(14.f * FMath::Sin(P * 6.f + 0.8f) + 12.f * A.Charge, 0.f, 16.f * FMath::Sin(P * 3.f + 0.4f)));   // (the 2.8 arms)
		// the arms: they pump as he runs; both go back to pack a snowball and over to throw it; right back and then
		// all the way over and down for the pound; wide for the roar; out at the car when he charges
		const float Both = -40.f * W1 + 88.f * Throw - 30.f * W2 + 148.f * Slam - 10.f * Roar + 40.f * A.Charge * (bWayIn ? 0.f : 1.f) - 16.f * Hit;
		for (int32 s = 0; s < 2; ++s)
			if (UStaticMeshComponent* Arm = BitOf(A, s))
			{
				const float Out = (s ? 1.f : -1.f);
				const float Pump = 16.f * FMath::Sin(P * 6.f + 0.8f + (s ? PI : 0.f)) * (1.f - Plant);
				Arm->SetRelativeRotation(FRotator(Pump + Both, 0.f, Out * (8.f + 6.f * FMath::Sin(P * 3.f + s) * (1.f - Plant) + 52.f * Roar - 12.f * W1 * (1.f - Throw))));
			}
		// the snowball: it grows between his hands through the wind-up and is gone as he throws (the real ones fly then)
		if (UStaticMeshComponent* Ball = BitOf(A, 5))
		{
			const bool bOn = W1 > 0.08f && Ball->GetStaticMesh() != nullptr;
			Show(Ball, bOn);
			if (bOn)
			{
				Ball->SetRelativeLocation(FVector(0.f, 0.f, AFTEntityActor::YetiShoulderZ) + FRotator(-40.f * W1, 0.f, 0.f).RotateVector(FVector(-70.f, 0.f, 800.f)));
				Ball->SetRelativeScale3D(FVector(0.5f + 1.0f * W1));
			}
		}
		// his eyes: they burn when he charges, as he winds up, and all the time once the ice is off him
		const float Eyes = FMath::RoundToFloat((2.2f * FMath::Max(A.Charge, FMath::Max3(W1, W2, W3)) + (A.DmgState >= 2 ? 2.2f : 0.f)) * 4.f) * 0.25f;
		A.BossAlert = Eyes;
		A.Loop(0, TEXT("NS_BossSnowKick"), FVector(120.f, 0.f, 40.f), A.Charge > 0.4f);
	}
	void FireYeti(AFTEntityActor& A)
	{
		if (A.FireKind == 2) FTFx::Attach(TEXT("NS_BossSnowSlam"), A.Mesh, FVector(-420.f, 0.f, 60.f), FRotator::ZeroRotator, 1.f, FTFx::Main, true);
		else if (A.FireKind == 3) FTFx::Attach(TEXT("NS_BossSnowSlam"), A.Mesh, FVector(0.f, 0.f, 700.f), FRotator::ZeroRotator, 1.6f, FTFx::Extra, true);
	}
	void BreakYeti(AFTEntityActor& A, int32 State)
	{
		// the ice is shot off him: the left shoulder at damage 1, the right and the neck at 2. After that he is berserk
		for (int32 i = (State == 1 ? 0 : 1); i < (State == 1 ? 1 : 3); ++i)
			if (UStaticMeshComponent* Ice = BitOf(A, 2 + i))
			{
				FTFx::Attach(TEXT("NS_BossIceBreak"), A.Mesh, YetiIceAt[i] + FVector(0.f, 0.f, 80.f), FRotator::ZeroRotator, 1.f, FTFx::Main, true);
				Show(Ice, false);
			}
	}
	void DefeatYeti(AFTEntityActor& A, const FDeath& D)
	{
		// he does not burn. What ice is left on him bursts off and goes up clean, in a cloud of snow and a fountain of
		// ice from his head (ten metres up: over the road where the camera sees it)
		for (int32 i = 0; i < 3; ++i)
			ThrowPart(A, BitOf(A, 2 + i), FVector(-300.f, (i == 0 ? -900.f : (i == 1 ? 900.f : 0.f)), 2000.f + 300.f * i), FVector(300.f, 200.f, 260.f), false);
		const FTransform Body = A.Mesh->GetComponentTransform();
		FTFx::Burst(&A, TEXT("NS_BossSnowSlam"), Body.TransformPosition(FVector(0.f, 0.f, 700.f)), FRotator::ZeroRotator, 2.4f, FTFx::Main);
		FTFx::Burst(&A, TEXT("NS_BossIceBreak"), Body.TransformPosition(FVector(0.f, 0.f, 1000.f)), FRotator::ZeroRotator, 2.5f, FTFx::Main);
		// his wreck is himself: white, not sooted, no fire on him, both arms flung up and back as he goes over, and
		// snow streaming off him where a truck would trail flame
		if (AFTWreck* Wr = D.Body.Get())
		{
			Unburnt(Wr);
			for (int32 s = 0; s < 2; ++s)
				WreckBit(Wr, Kept(s ? TEXT("/Game/Characters/Bosses/SM_YetiArmR") : TEXT("/Game/Characters/Bosses/SM_YetiArmL")), YetiShoulder[s], FRotator(-38.f, 0.f, (s ? 1.f : -1.f) * 34.f), 0.f);
			WreckLoop(Wr, TEXT("NS_BossSnowKick"), FVector(0.f, 0.f, 500.f), FRotator::ZeroRotator, 1.6f);
		}
		// the ice goes off one shoulder and then the other, and a last cloud as he comes down
		for (int32 s = 0; s < 2; ++s)
			Later(A, 0.12f + 0.12f * s, [D, s]() { Pop(D, TEXT("NS_BossIceBreak"), FVector(0.f, s ? 270.f : -270.f, 960.f), 2.5f); });
		Later(A, 0.5f, [D]() { Pop(D, TEXT("NS_BossSnowSlam"), FVector(0.f, 0.f, 500.f), 3.f); });
		Later(A, 0.9f, [D]() { Pop(D, TEXT("NS_BossDefeatSnow"), FVector(0.f, 0.f, 600.f), 1.5f); });
	}

	// ---------------------------------------------------------------- THE GREAT WHITE (level 7): the shark car
	// It swims. Bits: 0 the tail, 1 the dorsal fin (SM_SharkFin; SM_SharkFinB, torn, from damage 1), 2 the lower jaw.
	// BossFx: 0 smoke (damage 1), 1 fire (damage 2). RigA the swim's phase, RigB how far it has turned on the car
	// (the lunge: 0..1 = 0..180 degrees), RigD how far the jaw is down.
	// Wind kinds: 1 the fan of swordfish, 2 the lobbed flying fish. BossLunge: the lunge.
	const FVector SharkTailAt(-565.f, 0.f, 255.f), SharkFinAt(25.f, 0.f, 392.f), SharkJawAt(462.f, 0.f, 178.f);
	void SetupShark(AFTEntityActor& A)
	{
		A.Part(0, TEXT("/Game/Characters/Bosses/SM_SharkTail"), SharkTailAt)->SetRelativeRotation(FRotator(0.f, 18.f, 0.f));
		A.Part(1, TEXT("/Game/Characters/Bosses/SM_SharkFin"), SharkFinAt);
		A.Part(2, TEXT("/Game/Characters/Bosses/SM_SharkJaw"), SharkJawAt, false)->SetRelativeRotation(FRotator(-6.f, 0.f, 0.f));
		Kept(TEXT("/Game/Characters/Bosses/SM_SharkFinB"));   // (the bitten fin: loaded now)
	}
	void AnimShark(AFTEntityActor& A, float Dt)
	{
		const float W1 = A.WindKind == 1 ? Smooth(A.Wind) : 0.f, W2 = A.WindKind == 2 ? Smooth(A.Wind) : 0.f;
		const float Hit = Pulse(A.HurtT, 5.f) * A.HurtAmt;
		const bool bJaws = A.Lunge > 0.02f;
		// it swims: the body snakes and rolls and the tail beats behind it; harder when it lunges, a thrash when it is hit
		A.RigA += Dt * (5.f + 4.f * A.Lunge + 7.f * Hit);
		// the lunge: it whips round on the car as the warning starts, jaws wide, and comes back at it snapping
		A.RigB = FMath::FInterpTo(A.RigB, bJaws ? 1.f : 0.f, Dt, bJaws ? 7.f : 3.5f);
		const float Whip = A.FireKind == 1 ? Pulse(A.FireT, 5.f) : 0.f, Buck = A.FireKind == 2 ? Pulse(A.FireT, 5.f) : 0.f;
		// (the entrance: one leap clear of the road, nose up and then down, as it drops back onto its spot)
		// (round two: the leap is twice as high, four and a half metres clear, out of a spout of water where it leaves
		// the road; the same 1.1 s)
		const float Air = A.bBossLive ? FMath::Clamp(A.IntroT / 1.1f, 0.f, 1.f) : 1.f;
		const float Leap = 460.f * FMath::Sin(PI * Air), Dive = 26.f * FMath::Sin(2.f * PI * Air);
		if (Crossed(A, Dt, 0.05f)) FTFx::Attach(TEXT("NS_BossSpray"), A.GetRootComponent(), FVector(0.f, 0.f, 60.f), FRotator::ZeroRotator, 3.f, FTFx::Extra, true);
		// (round two: it swims in a wake. Two low sheets of spray run back off its flanks all the time it is on the
		// road and facing away: a white V on the road that marks it out from behind. Knee high, and it dies away
		// (0.4 s) as soon as either load starts to wind up, stays off until the fish have gone, and is off while it
		// has turned on the car: nothing it fires is ever behind spray)
		const bool bWake = A.bBossLive && Air >= 1.f && A.RigB < 0.08f && A.Wind < 0.05f && A.FireT > 0.7f;
		A.Loop(2, TEXT("NS_BossWake"), FVector(-300.f, -300.f, 30.f), bWake, FRotator(0.f, -24.f, 0.f));
		A.Loop(3, TEXT("NS_BossWake"), FVector(-300.f, 300.f, 30.f), bWake, FRotator(0.f, 24.f, 0.f));
		A.Mesh->SetRelativeTransform(FTransform(FRotator(-5.f * W2 + 8.f * Buck + Dive, 180.f * Smooth(A.RigB) + 5.f * FMath::Sin(A.RigA) * (1.f - 0.6f * W1) + 8.f * Hit * FMath::Sin(A.AnimT * 31.f),
			4.f * FMath::Sin(A.RigA * 0.5f + 1.f) + (A.DmgState >= 2 ? 12.f : 0.f)), FVector(0.f, 0.f, 6.f * FMath::Sin(A.RigA * 0.5f) + Leap)));
		if (Air < 1.f) HangWheels(A, 0.f, A.AnimT * 600.f);   // (its wheels leave the road with it)
		// the tail: it beats with the swim, is held hard over while a fan of swordfish winds up, and whips across as they go
		if (UStaticMeshComponent* T = BitOf(A, 0)) T->SetRelativeRotation(FRotator(0.f, FMath::Lerp(26.f * FMath::Sin(A.RigA - 0.9f), -36.f, W1) + 72.f * Whip, 0.f));
		// the jaw: wide through the warning, snapping as it comes
		const float Chomp = A.bLunge ? 0.3f + 0.7f * FMath::Abs(FMath::Sin(A.AnimT * 9.f)) : 1.f;
		A.RigD = FMath::FInterpTo(A.RigD, bJaws ? Chomp : 0.f, Dt, 14.f);
		if (UStaticMeshComponent* J = BitOf(A, 2)) J->SetRelativeRotation(FRotator(-6.f - 30.f * A.RigD, 0.f, 0.f));
		A.BossAlert = 1.6f * FMath::Max(W1, W2);   // the launchers' mouths burn brighter as either load winds up
	}
	void FireShark(AFTEntityActor& A)
	{
		if (A.FireKind == 1)
		{
			// out of the two launchers on its back, or, when it has turned on the car, out of its mouth
			if (A.RigB > 0.5f) FTFx::Attach(TEXT("NS_BossFishFlash"), A.Mesh, FVector(640.f, 0.f, 230.f), FRotator::ZeroRotator, 1.3f, FTFx::Flash, true);
			else for (int32 s = 0; s < 2; ++s) FTFx::Attach(TEXT("NS_BossFishFlash"), A.Mesh, FVector(-640.f, s ? 120.f : -120.f, 400.f), FRotator::ZeroRotator, 1.f, FTFx::Flash, true);
		}
		else FTFx::Attach(TEXT("NS_BossSpray"), A.Mesh, FVector(25.f, 0.f, 430.f), FRotator::ZeroRotator, 1.f, FTFx::Extra, true);
	}
	void BreakShark(AFTEntityActor& A, int32 State)
	{
		if (State == 1)
		{
			// a bite out of its own fin
			if (UStaticMeshComponent* Fin = BitOf(A, 1)) if (UStaticMesh* Torn = Kept(TEXT("/Game/Characters/Bosses/SM_SharkFinB"))) Fin->SetStaticMesh(Torn);
			A.Loop(0, TEXT("NS_BossSmoke"), FVector(-420.f, 120.f, 450.f), true);
		}
		else A.Loop(1, TEXT("NS_BossFlame"), FVector(-420.f, -120.f, 440.f), true);   // a launcher burns, and it lists
		FTFx::Attach(TEXT("NS_BossSpray"), A.Mesh, SharkFinAt + FVector(0.f, 0.f, 150.f), FRotator::ZeroRotator, 1.4f, FTFx::Main, true);
	}
	void DefeatShark(AFTEntityActor& A, const FDeath& D)
	{
		// gutted: the dorsal fin goes straight up on a spout of water (high over the road, where the camera sees it),
		// the tail whips off to one side and the jaw is torn away
		ThrowPart(A, BitOf(A, 1), FVector(-200.f, RigRng.FRandRange(-250.f, 250.f), 2600.f), FVector(500.f, 200.f, 100.f));
		ThrowPart(A, BitOf(A, 0), FVector(-1100.f, RigRng.FRand() < 0.5f ? -1300.f : 1300.f, 1000.f), FVector(200.f, 100.f, 700.f));
		ThrowPart(A, BitOf(A, 2), FVector(700.f, 0.f, 1300.f), FVector(100.f, 600.f, 100.f));
		FTFx::Burst(&A, TEXT("NS_BossSpray"), A.Mesh->GetComponentTransform().TransformPosition(SharkFinAt), FRotator::ZeroRotator, 3.f, FTFx::Main);
		// the wreck rolls on with both launchers burning
		AFTWreck* Wr = D.Body.Get();
		WreckLoop(Wr, TEXT("NS_BossWreckFire"), FVector(-420.f, 120.f, 440.f), FRotator::ZeroRotator, 1.6f);
		WreckLoop(Wr, TEXT("NS_BossWreckFire"), FVector(-420.f, -120.f, 440.f), FRotator::ZeroRotator, 1.6f);
		// its magazines go, one launcher and then the other, each in a flash of the sea's colour and a spout; and a
		// last, biggest spout out of its belly
		for (int32 s = 0; s < 2; ++s)
			Later(A, 0.1f + 0.14f * s, [D, s]()
			{
				const FVector Mouth(-640.f, s ? 120.f : -120.f, 400.f);
				Pop(D, TEXT("NS_BossFishFlash"), Mouth, 2.6f, FTFx::Flash, 100.f);
				Pop(D, TEXT("NS_BossSpray"), Mouth, 2.6f, FTFx::Main, 100.f);
			});
		Later(A, 0.75f, [D]() { Pop(D, TEXT("NS_BossSpray"), FVector(0.f, 0.f, 250.f), 3.6f); });
	}

	// ---------------------------------------------------------------- THE CHAIRMAN (level 8): the limo gunship
	// Bits: 0 the boot's turret, 1 the sunroof's turret (it comes up in phase 1: "he's opened the sunroof guns"),
	// 2-3 the rocket bays in the rear wings (left, right: the right one is the same mesh mirrored). BossFx: 0-1 the
	// rear jets, 2 the sunroof guns' fire, 3 smoke (damage 1), 4 fire (damage 2). RigA / RigB the two turrets' turn,
	// RigD how far the sunroof's turret is up, RigE how far the bays are open, RigH / RigG what the turrets' heat was set to.
	// Wind kinds: 1 the wall of shots, 2 the aimed bursts (AimX: the car), 3 the rockets. Cue[0]: where the stream is
	// aimed across the road from him; Cue[1]: 1 while the stream runs.
	const FVector LimoBoot(-550.f, 0.f, 185.f), LimoRoof(0.f, 0.f, 292.f), LimoBay(-550.f, 250.f, 187.f), LimoMuzzle(-266.f, 30.f, 44.f);
	void SetupChairman(AFTEntityActor& A)
	{
		A.Part(0, TEXT("/Game/Characters/Bosses/SM_LimoTurret"), LimoBoot);
		Show(A.Part(1, TEXT("/Game/Characters/Bosses/SM_LimoTurret"), LimoRoof - FVector(0.f, 0.f, 90.f)), false);
		for (int32 s = 0; s < 2; ++s)
		{
			UStaticMeshComponent* Bay = A.Part(2 + s, TEXT("/Game/Characters/Bosses/SM_LimoPod"), FVector(LimoBay.X, (s ? 1.f : -1.f) * LimoBay.Y, LimoBay.Z), false);
			if (s) Bay->SetRelativeScale3D(FVector(1.f, -1.f, 1.f));
		}
	}
	void AnimChairman(AFTEntityActor& A, float Dt, float Vx)
	{
		const float T = A.AnimT;
		const float W1 = A.WindKind == 1 ? Smooth(A.Wind) : 0.f, W2 = A.WindKind == 2 ? Smooth(A.Wind) : 0.f, W3 = A.WindKind == 3 ? Smooth(A.Wind) : 0.f;
		const float Hit = Pulse(A.HurtT, 5.f) * A.HurtAmt;
		const bool bStream = A.Cue[1] >= 1.f && A.DmgState >= 1;                          // the stream's shots are leaving now
		const float StreamUp = A.DmgState >= 1 ? FMath::Clamp(A.Cue[1], 0.f, 1.f) : 0.f;   // its first shot is 0.6 s off or less
		if (A.Cue[2] != A.RigF) { A.RigF = A.Cue[2]; A.RigC = 0.f; }                       // a volley of the aimed bursts has just left
		const bool bVolley = A.RigC <= 0.f;
		// it hovers on its jets (as 2.5), banks into its weave, drops when it is hit, and lists once a jet is failing
		const float List = A.DmgState >= 2 ? 6.f + 2.f * FMath::Sin(T * 7.f) : 0.f;
		A.Mesh->SetRelativeTransform(FTransform(FRotator(2.f * FMath::Sin(T * 1.3f) - 3.f * Hit, 0.f, 3.f * FMath::Sin(T * 1.7f) + FMath::Clamp(Vx / 80.f, -9.f, 9.f) + List),
			FVector(0.f, 0.f, 40.f * FMath::Sin(T * 2.f) - 55.f * Hit)));
		// the boot's turret: it looks the road over; squares up and heats as a wall of shots winds up and rakes across
		// as it fires; turns on the car for the aimed bursts and kicks with each of the three
		const float In = A.bBossLive ? Smooth((A.IntroT - 0.3f) / 0.9f) : 1.f;   // the entrance: a limousine, until the turret comes up out of the boot
		const float Volley = A.RigF > 0.f ? Pulse(A.RigC, 9.f) : 0.f;
		const float Rake = A.FireKind == 1 ? Pulse(A.FireT, 3.5f) : 0.f;
		const bool bOnCar = A.WindKind == 2 && (A.Wind > 0.f || (A.FireKind == 2 && A.FireT < 0.9f));
		const float Want0 = Rake > 0.05f ? 40.f * FMath::Sin(A.FireT * 21.f) * Rake : (bOnCar || (A.FireKind == 2 && A.FireT < 0.9f) ? -FMath::RadiansToDegrees(FMath::Atan2(A.AimX, 5200.f)) : 14.f * FMath::Sin(T * 0.8f) * (1.f - W1));
		A.RigA = FMath::FInterpTo(A.RigA, Want0, Dt, Rake > 0.05f ? 30.f : 9.f);
		if (UStaticMeshComponent* Tu = BitOf(A, 0))
		{
			const FQuat Q(FRotator(0.f, A.RigA, 0.f));
			Show(Tu, In > 0.01f && Tu->GetStaticMesh() != nullptr);   // (a limousine and nothing else until it starts to come up)
			Tu->SetRelativeLocationAndRotation(LimoBoot + FVector(0.f, 0.f, -70.f * (1.f - In)) + Q.RotateVector(FVector(14.f * FMath::Max(Rake, Volley), 0.f, 0.f)), Q);
			const float Heat = FMath::RoundToFloat((2.4f * FMath::Max(W1, W2) + 3.f * FMath::Max(Rake, Volley)) * 4.f) * 0.25f;
			if (Heat != A.RigH) { A.RigH = Heat; Tu->SetScalarParameterValueOnMaterials(TEXT("Alert"), Heat); }
			// each volley of an aimed burst flashes at the guns as it leaves (TickBossLimo's own timers say when: Cue 2)
			if (bVolley)
				for (int32 g = 0; g < 2; ++g) FTFx::Attach(TEXT("NS_BossGunFlash"), Tu, FVector(LimoMuzzle.X, g ? LimoMuzzle.Y : -LimoMuzzle.Y, LimoMuzzle.Z), FRotator::ZeroRotator, 0.7f, FTFx::Flash, true);
		}
		// the sunroof's turret: up through the roof when the fight reaches phase 1, and on the stream from then on: it
		// follows where the stream is aimed (which trails the car), shaking as it fires
		A.RigD = FMath::FInterpTo(A.RigD, A.DmgState >= 1 ? 1.f : 0.f, Dt, 2.5f);
		if (UStaticMeshComponent* Tu = BitOf(A, 1))
		{
			Show(Tu, A.RigD > 0.02f && Tu->GetStaticMesh() != nullptr);
			A.RigB = FMath::FInterpTo(A.RigB, bStream ? -FMath::RadiansToDegrees(FMath::Atan2(A.Cue[0], 5700.f)) : 0.f, Dt, 9.f);
			const FQuat Q(FRotator(0.f, A.RigB, 0.f));
			Tu->SetRelativeLocationAndRotation(LimoRoof + FVector(0.f, 0.f, -90.f * (1.f - Smooth(A.RigD))) + Q.RotateVector(FVector(bStream ? 5.f * FMath::Sin(T * 60.f) : 0.f, 0.f, 0.f)), Q);
			const float Heat = bStream ? 2.5f : FMath::RoundToFloat(2.f * StreamUp * 4.f) * 0.25f;   // (they heat as the stream's first shot comes due)
			if (Heat != A.RigG) { A.RigG = Heat; Tu->SetScalarParameterValueOnMaterials(TEXT("Alert"), Heat); }
			A.Loop(2, TEXT("NS_BossGunStream"), LimoRoof + Q.RotateVector(FVector(LimoMuzzle.X, 0.f, LimoMuzzle.Z)), bStream && A.RigD > 0.9f, Q.Rotator());
		}
		// the rocket bays: they tip up out of the rear wings through the rockets' wind-up, fire, and shut
		A.RigE = FMath::FInterpTo(A.RigE, (W3 > 0.02f || (A.FireKind == 3 && A.FireT < 0.55f)) ? 1.f : 0.f, Dt, 8.f);
		for (int32 s = 0; s < 2; ++s)
			if (UStaticMeshComponent* Bay = BitOf(A, 2 + s)) Bay->SetRelativeRotation(FRotator(0.f, 0.f, (s ? 1.f : -1.f) * 62.f * A.RigE));
		// the jets: two flames at the rear; one coughs once he is nearly done
		// (round two: half as big again, and he arrives on full burn: three times that as he appears, throttled back
		// over the 1.6 s he takes to reach his spot. Two long blue-white flames low at his tail read from 160 m, and
		// they are his hover, nothing he fires)
		const float Burn = 1.5f * (A.bBossLive ? 1.f + 2.f * (1.f - Smooth(A.IntroT / 1.6f)) : 1.f);
		A.Loop(0, TEXT("NS_BossJet"), FVector(-602.f, -299.f, 60.f), true, FRotator::ZeroRotator, Burn);
		A.Loop(1, TEXT("NS_BossJet"), FVector(-602.f, 299.f, 60.f), A.DmgState < 2 || FMath::Sin(T * 7.f) > -0.2f, FRotator::ZeroRotator, Burn);
		A.BossAlert = 1.2f * FMath::Max3(W1, W2, W3);   // the fins' lights and the tail flare with every wind-up
	}
	void FireChairman(AFTEntityActor& A)
	{
		if (A.FireKind == 1)
		{
			if (UStaticMeshComponent* Tu = BitOf(A, 0))
				for (int32 g = 0; g < 2; ++g) FTFx::Attach(TEXT("NS_BossGunFlash"), Tu, FVector(LimoMuzzle.X, g ? LimoMuzzle.Y : -LimoMuzzle.Y, LimoMuzzle.Z), FRotator::ZeroRotator, 1.f, FTFx::Flash, true);
		}
		else if (A.FireKind == 3)
			for (int32 s = 0; s < 2; ++s) FTFx::Attach(TEXT("NS_BossRocketUp"), A.Mesh, FVector(LimoBay.X, (s ? 1.f : -1.f) * 200.f, LimoBay.Z + 40.f), FRotator::ZeroRotator, 1.f, FTFx::Main, true);
	}
	void BreakChairman(AFTEntityActor& A, int32 State)
	{
		// the money starts to go; then a jet
		if (State == 1) A.Loop(3, TEXT("NS_BossSmoke"), FVector(-560.f, 0.f, 230.f), true);
		else A.Loop(4, TEXT("NS_BossFlame"), FVector(-600.f, 299.f, 110.f), true);
		FTFx::Attach(TEXT("NS_BossHurtBills"), A.Mesh, FVector(-560.f, 0.f, 260.f), FRotator::ZeroRotator, 2.f, FTFx::Main, true);
	}
	void DefeatChairman(AFTEntityActor& A, const FDeath& D)
	{
		// the vault bursts: both turrets are blown out of him (the boot's highest, over the road where the camera
		// sees it), the rocket bays are torn off the wings to either side, and his money goes up in a fountain
		ThrowPart(A, BitOf(A, 0), FVector(-500.f, RigRng.FRandRange(-250.f, 250.f), 2500.f), FVector(200.f, 400.f, 160.f));
		ThrowPart(A, BitOf(A, 1), FVector(100.f, RigRng.FRandRange(-300.f, 300.f), 1900.f), FVector(300.f, 200.f, 300.f));
		for (int32 s = 0; s < 2; ++s) ThrowPart(A, BitOf(A, 2 + s), FVector(-300.f, (s ? 1.f : -1.f) * 1400.f, 1100.f), FVector(600.f, 100.f, 100.f));
		FTFx::Burst(&A, TEXT("NS_BossHurtBills"), A.Mesh->GetComponentTransform().TransformPosition(FVector(-300.f, 0.f, 300.f)), A.GetActorRotation(), 4.f, FTFx::Main);
		// the wreck goes on down the road on the one jet that still burns, the other alight
		AFTWreck* Wr = D.Body.Get();
		WreckLoop(Wr, TEXT("NS_BossJet"), FVector(-602.f, -299.f, 60.f), FRotator::ZeroRotator, 2.4f);
		WreckLoop(Wr, TEXT("NS_BossWreckFire"), FVector(-600.f, 299.f, 110.f), FRotator::ZeroRotator, 1.6f);
		// the rockets he never fired cook off straight up out of both wings, and the money keeps coming, higher each time
		for (int32 s = 0; s < 2; ++s)
			Later(A, 0.12f + 0.12f * s, [D, s]() { Pop(D, TEXT("NS_BossRocketUp"), FVector(LimoBay.X, s ? 200.f : -200.f, LimoBay.Z + 40.f), 2.2f); });
		for (int32 k = 0; k < 4; ++k)
			Later(A, 0.2f + 0.2f * k, [D, k]() { Pop(D, TEXT("NS_BossHurtBills"), FVector(-300.f, 0.f, 300.f), 3.5f, FTFx::Main, 120.f * k); });
	}

	// ---------------------------------------------------------------- every boss
	/** The effects a boss's rig uses, loaded as he appears (FTFx::System keeps them): never from disk in the middle of his fight. */
	void RigPreload(int32 Lvl)
	{
		static const TCHAR* Shared[] = { TEXT("NS_BossBreak"), TEXT("NS_BossBreakShards"), TEXT("NS_BossSmoke"), TEXT("NS_BossFlame"), TEXT("NS_BossWreckFire") };
		// (round two: with what his entrance and his end use. A row is nullptr-padded to its eight places)
		static const TCHAR* Own[8][8] = {
			{ TEXT("NS_BossHurtSparks"), TEXT("NS_BossDefeat"), TEXT("NS_BossGunFlash"), TEXT("NS_BossDust"), TEXT("NS_BossPillar"), nullptr, nullptr, nullptr },
			{ TEXT("NS_BossHurtZap"), TEXT("NS_BossDefeatZap"), TEXT("NS_BossZapCrackle"), TEXT("NS_BossZapFlash"), TEXT("NS_BossZapSpray"), TEXT("NS_BossSteam"), nullptr, nullptr },
			{ TEXT("NS_BossHurtChips"), TEXT("NS_BossDefeatChips"), TEXT("NS_BossLaunch"), TEXT("NS_BossDust"), TEXT("NS_BossTyreSmoke"), nullptr, nullptr, nullptr },
			{ TEXT("NS_BossHurtSparks"), TEXT("NS_BossDefeatFire"), TEXT("NS_BossFlameJet"), TEXT("NS_BossSteam"), TEXT("NS_BossMortar"), TEXT("NS_BossDust"), TEXT("NS_BossPillar"), nullptr },
			{ TEXT("NS_BossHurtConfetti"), TEXT("NS_BossDefeatConfetti"), TEXT("NS_BossConfetti"), TEXT("NS_BossPop"), TEXT("NS_BossPieBlast"), TEXT("NS_BossTyreSmoke"), nullptr, nullptr },
			{ TEXT("NS_BossHurtSnow"), TEXT("NS_BossDefeatSnow"), TEXT("NS_BossSnowKick"), TEXT("NS_BossSnowSlam"), TEXT("NS_BossIceBreak"), nullptr, nullptr, nullptr },
			{ TEXT("NS_BossHurtSpray"), TEXT("NS_BossDefeatSplash"), TEXT("NS_BossFishFlash"), TEXT("NS_BossSpray"), TEXT("NS_BossWake"), nullptr, nullptr, nullptr },
			{ TEXT("NS_BossHurtBills"), TEXT("NS_BossDefeatCash"), TEXT("NS_BossGunFlash"), TEXT("NS_BossGunStream"), TEXT("NS_BossRocketUp"), TEXT("NS_BossJet"), nullptr, nullptr },
		};
		for (const TCHAR* N : Shared) FTFx::System(N);
		for (const TCHAR* N : Own[FMath::Clamp(Lvl, 0, 7)]) if (N) FTFx::System(N);
	}
	void RigSetup(AFTEntityActor& A)
	{
		RigPreload(A.Lvl);
		switch (A.Lvl)
		{
		case 0: SetupJuggernaut(A); break;
		case 1: SetupAlchemist(A); break;
		case 2: SetupDealer(A); break;
		case 3: SetupEngine(A); break;
		case 4: SetupChuckles(A); break;
		case 5: SetupYeti(A); break;
		case 6: SetupShark(A); break;
		case 7: SetupChairman(A); break;
		default: break;
		}
	}
	void RigFire(AFTEntityActor& A)
	{
		switch (A.Lvl)
		{
		case 0: FireJuggernaut(A); break;
		case 1: FireAlchemist(A); break;
		case 2: FireDealer(A); break;
		case 3: FireEngine(A); break;
		case 4: FireChuckles(A); break;
		case 5: FireYeti(A); break;
		case 6: FireShark(A); break;
		case 7: FireChairman(A); break;
		default: break;
		}
	}
	/** A new damage state (1 at two thirds of its armour, 2 at one third): it shows, and it stays. */
	void RigBreak(AFTEntityActor& A, int32 State)
	{
		const FBossLook& L = LookOf(A);
		A.Mesh->SetScalarParameterValueOnMaterials(TEXT("Soot"), State == 1 ? 0.14f : 0.3f);
		FTFx::Attach(TEXT("NS_BossBreak"), A.Mesh, L.Heart, FRotator::ZeroRotator, 1.f, FTFx::Main, true);
		FTFx::Attach(TEXT("NS_BossBreakShards"), A.Mesh, L.Heart, FRotator::ZeroRotator, 1.f, FTFx::Extra, true);
		Kick(A, 110.f, (RigRng.FRand() < 0.5f) ? 140.f : -140.f, 420.f);
		switch (A.Lvl)
		{
		case 0: BreakJuggernaut(A, State); break;
		case 1: BreakAlchemist(A, State); break;
		case 2: BreakDealer(A, State); break;
		case 3: BreakEngine(A, State); break;
		case 4: BreakChuckles(A, State); break;
		case 5: BreakYeti(A, State); break;
		case 6: BreakShark(A, State); break;
		case 7: BreakChairman(A, State); break;
		default: break;
		}
	}
	void RigAnimate(AFTEntityActor& A, float Dt, float Speed, float Vx)
	{
		A.LastSpeed = Speed;
		A.IntroT += Dt; A.HurtT += Dt; A.HurtFxT += Dt; A.DmgT += Dt; A.FireT += Dt; A.RigC += Dt;
		if (!A.bWindSet) { A.Wind = FMath::FInterpTo(A.Wind, 0.f, Dt, 9.f); if (A.Wind < 0.01f) A.Wind = 0.f; }
		if (!A.bLungeSet) { A.Lunge = FMath::FInterpTo(A.Lunge, 0.f, Dt, 6.f); if (A.Lunge < 0.01f) A.Lunge = 0.f; A.bLunge = false; }
		A.bWindSet = A.bLungeSet = false;
		switch (A.Lvl)
		{
		case 0: AnimJuggernaut(A, Dt); break;
		case 1: AnimAlchemist(A, Dt); break;
		case 2: AnimDealer(A, Dt); break;
		case 3: AnimEngine(A, Dt); break;
		case 4: AnimChuckles(A, Dt, Vx); break;
		case 5: AnimYeti(A, Dt); break;
		case 6: AnimShark(A, Dt); break;
		case 7: AnimChairman(A, Dt, Vx); break;
		default: break;
		}
		// the lit parts of the body: the game's own warning glow (SetGlow) plus what the rig adds and the flinch of
		// a hit, written to the materials only when it changes
		const float Hit = Pulse(A.HurtT, 14.f) * A.HurtAmt;
		const float BodyHit = A.HurtAmt > 0.35f ? Hit : 0.f;   // a missile or a damage state flashes the body's materials; a single round flashes the light, the sparks and the kick
		// quarter steps: a write per step, not per frame (3.3 perf: each write walks every material slot)
		const float Want = FMath::Max(-0.4f, FMath::RoundToFloat((FMath::Max(A.LastGlow, 0.f) + A.BossAlert + 0.9f * BodyHit) * 4.f) * 0.25f);
		if (Want != A.AlertSet) { A.AlertSet = Want; A.Mesh->SetScalarParameterValueOnMaterials(TEXT("Alert"), Want); }
		if (A.Lvl != 1) A.Light->SetIntensity(1500.f + 1800.f * FMath::Clamp(A.LastGlow, 0.f, 1.f) + 2600.f * Hit);   // (the Tesla truck's light is SetTesla's)
	}
	/** The boss's proxy is being let go. Only a wreck (its armour gone) comes apart; an escape or a restart is quiet. */
	void RigRelease(AFTEntityActor& A)
	{
		bool bWrecked = false;
		if (const UWorld* W = A.GetWorld())
			if (const AFTGameMode* GM = W->GetAuthGameMode<AFTGameMode>())
				for (const FFTEnt& E : GM->GetActiveEntities()) if (E.Proxy.Get() == &A) { bWrecked = E.Hp <= 0.f; break; }
		if (bWrecked)
		{
			// round two: his end is his own, and it is played in beats (see "how a boss dies" above). First, on the
			// frame of the kill (the frame the wrap freezes on): his burst, bigger than it was, and his shards
			const FBossLook& L = LookOf(A);
			const FVector At = A.Mesh->GetComponentTransform().TransformPosition(L.Heart);
			FTFx::Burst(&A, L.DefeatFx, At, A.GetActorRotation(), 1.3f, FTFx::Main);
			FTFx::Burst(&A, TEXT("NS_BossBreakShards"), At, A.GetActorRotation(), 1.6f, FTFx::Extra);
			FDeath D;
			D.Body = BodyWreck(A);
			D.Face = A.GetActorRotation();
			switch (A.Lvl)
			{
			case 0: DefeatJuggernaut(A, D); break;
			case 1: DefeatAlchemist(A, D); break;
			case 2: DefeatDealer(A, D); break;
			case 3: DefeatEngine(A, D); break;
			case 4: DefeatChuckles(A, D); break;
			case 5: DefeatYeti(A, D); break;
			case 6: DefeatShark(A, D); break;
			case 7: DefeatChairman(A, D); break;
			default: break;
			}
			// every boss: his wreck goes up once more as the card comes on, a blast that stands two and a half metres
			// over it and bigger than the first, so it shows over the card's top edge
			const TCHAR* Again = L.DefeatFx;
			const FVector Heart = L.Heart;
			Later(A, 0.42f, [D, Again, Heart]()
			{
				Pop(D, Again, Heart, 1.7f, FTFx::Main, 250.f);
				Pop(D, TEXT("NS_BossBreakShards"), Heart, 1.8f, FTFx::Extra, 250.f);
			});
			UE_LOG(LogFT, Display, TEXT("FT BOSSRIG defeat level=%d speed=%.0f wreck=%d"), A.Lvl, A.LastSpeed, D.Body.IsValid() ? 1 : 0);
		}
		for (UNiagaraComponent* N : A.BossFx) if (N && N->IsActive()) N->Deactivate();
	}
}

void AFTEntityActor::BossState(float HpFrac, int32 Phase)
{
	if (Kind != EFTKind::Boss) return;
	if (!bBossLive) { bBossLive = true; BossHp = HpFrac; UE_LOG(LogFT, Display, TEXT("FT BOSSRIG live level=%d bits=%d"), Lvl, Bits.Num()); }
	if (HpFrac < BossHp - 1e-5f)
	{
		// a hit: how hard by how much armour it took (one round is well under a percent, a missile about six). A
		// stream of rounds is a shiver: the pushes do not add up
		const float Hard = FMath::Clamp((BossHp - HpFrac) * 22.f, 0.3f, 1.f);
		const float Fresh = FMath::Clamp(HurtT / 0.12f, 0.f, 1.f);
		HurtAmt = FMath::Max(HurtAmt * Pulse(HurtT, 6.f), Hard);
		Kick(*this, 70.f * Hard * Fresh, RigRng.FRandRange(-80.f, 80.f) * Hard * Fresh, 300.f * Hard * Fresh);
		HurtT = 0.f;
		if (HurtFxT > 0.16f)
		{
			HurtFxT = 0.f;
			const FBossLook& L = LookOf(*this);
			FTFx::Attach(L.HurtFx, Mesh, L.HurtAt + FVector(0.f, RigRng.FRandRange(-120.f, 120.f), RigRng.FRandRange(-40.f, 60.f)), FRotator::ZeroRotator, 0.7f + 0.6f * Hard, FTFx::Extra, true);
		}
	}
	BossHp = HpFrac; BossPhase = Phase;
	while (DmgState < FMath::Clamp(Phase, 0, 2))
	{
		++DmgState; DmgT = 0.f; HurtAmt = 1.f; HurtT = 0.f;   // (a damage state is also the hardest flinch he has)
		RigBreak(*this, DmgState);
		UE_LOG(LogFT, Display, TEXT("FT BOSSRIG damage level=%d state=%d hp=%.2f"), Lvl, DmgState, HpFrac);
	}
}

void AFTEntityActor::BossWind(int32 InKind, float Amount, float InAimX)
{
	if (Kind != EFTKind::Boss) return;
	// (one line as each wind-up starts: the film scripts wait for it)
	if (Amount > 0.f && (InKind != WindKind || Wind <= 0.f)) { UE_LOG(LogFT, Display, TEXT("FT BOSSRIG wind level=%d kind=%d"), Lvl, InKind); }
	WindKind = InKind; Wind = FMath::Clamp(Amount, 0.f, 1.f); AimX = InAimX; bWindSet = true;
}

void AFTEntityActor::BossLunge(float Amount, bool bGoing)
{
	if (Kind != EFTKind::Boss) return;
	if (Amount > 0.f && Lunge <= 0.f) { UE_LOG(LogFT, Display, TEXT("FT BOSSRIG lunge level=%d warn=1"), Lvl); }
	if (bGoing && !bLunge) { UE_LOG(LogFT, Display, TEXT("FT BOSSRIG lunge level=%d go=1"), Lvl); }
	Lunge = FMath::Clamp(Amount, 0.f, 1.f); bLunge = bGoing; bLungeSet = true;
}

void AFTEntityActor::BossFire(int32 InKind, int32 Count)
{
	if (Kind != EFTKind::Boss) return;
	FireKind = InKind; FireCount = Count; FireT = 0.f;
	RigFire(*this);
	UE_LOG(LogFT, Display, TEXT("FT BOSSRIG fire level=%d kind=%d n=%d"), Lvl, InKind, Count);
}

UStaticMeshComponent* AFTEntityActor::Part(int32 I, const TCHAR* MeshPath, const FVector& At, bool bShadow)
{
	while (Bits.Num() <= I) Bits.Add(MakeBit(this, Mesh));
	UStaticMeshComponent* C = Bits[I];
	UStaticMesh* M = Kept(MeshPath);
	if (C->GetStaticMesh() != M) C->SetStaticMesh(M);
	C->SetRelativeLocation(At);
	C->SetCastShadow(bShadow);
	C->SetVisibility(M != nullptr);
	return C;
}

UNiagaraComponent* AFTEntityActor::Loop(int32 I, const TCHAR* Name, const FVector& At, bool bOn, const FRotator& Rot, float Scale)
{
	while (BossFx.Num() <= I)
	{
		UNiagaraComponent* N = NewObject<UNiagaraComponent>(this);
		N->SetupAttachment(Mesh);
		N->SetAutoActivate(false);
		N->RegisterComponent();
		AddInstanceComponent(N);
		BossFx.Add(N);
	}
	UNiagaraComponent* N = BossFx[I];
	if (!bOn) { if (N->IsActive()) N->Deactivate(); return N; }
	// (the Extra layer: a rig's smoke, crackle and jets are not shown at Low effects quality)
	UNiagaraSystem* Sys = FTFx::Shows(this, FTFx::Extra) ? FTFx::System(Name) : nullptr;
	if (N->GetAsset() != Sys) N->SetAsset(Sys);
	N->SetRelativeLocationAndRotation(At, Rot);
	N->SetRelativeScale3D(FVector(Scale));
	if (Sys && !N->IsActive()) N->Activate(true);
	return N;
}

AFTEntityActor::AFTEntityActor()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetGenerateOverlapEvents(false);
	// 2.8 perf: foes, bullets and pickups move every frame. Kept out of the distance fields and the Lumen scene
	// they no longer force a global distance field update and Lumen card captures every frame (0.3-0.6 ms at
	// EPIC on this PC); the cel pass bands their light the same either way
	Mesh->bAffectDistanceFieldLighting = false;
	Mesh->bAffectDynamicIndirectLighting = false;
	Extra = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Extra"));
	Extra->SetupAttachment(Mesh);
	Extra->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Extra->bAffectDistanceFieldLighting = false;
	Extra->bAffectDynamicIndirectLighting = false;
	Extra->SetVisibility(false);
	Trail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));
	Trail->SetupAttachment(Mesh);
	Trail->SetAutoActivate(false);
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetupAttachment(Mesh);
	Light->SetCastShadows(false);
	Light->SetVisibility(false);
	Light->SetIntensityUnits(ELightUnits::Candelas);
	// 3.1: a second trail layer, and the road mark under something about to land (it stays on the road: absolute)
	Trail2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail2"));
	Trail2->SetupAttachment(Mesh);
	Trail2->SetAutoActivate(false);
	Mark = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mark"));
	Mark->SetupAttachment(RootComponent);
	Mark->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mark->SetGenerateOverlapEvents(false);
	Mark->SetCastShadow(false);
	Mark->SetUsingAbsoluteLocation(true); Mark->SetUsingAbsoluteRotation(true); Mark->SetUsingAbsoluteScale(true);
	Mark->bAffectDistanceFieldLighting = false;
	Mark->bAffectDynamicIndirectLighting = false;
	Mark->SetVisibility(false);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MarkMesh(TEXT("/Engine/BasicShapes/Plane.Plane"));
	Mark->SetStaticMesh(MarkMesh.Object);
}

void AFTEntityActor::Setup(EFTKind InKind, int32 InVariant, int32 Level)
{
	Kind = InKind; Variant = InVariant; Lvl = Level; AnimT = 0.f;
	BikeHit = 0.f;
	// 3.3 bug fix: SetGlow/SetTracer leave dynamic material instances on the slots; a pooled actor reused for another
	// kind kept them, so a car could come back wearing the last one's paint. Any override goes on reuse.
	if (bTracerColoured || Mesh->GetNumOverrideMaterials() > 0 || Kind == EFTKind::Crate || Kind == EFTKind::Wrench) { Mesh->EmptyOverrideMaterials(); bTracerColoured = false; }
	Mesh->SetStaticMesh(MeshFor(Kind, Variant, Level));
	Mesh->SetRelativeTransform(FTransform::Identity);
	Mesh->SetCastShadow(Kind != EFTKind::Bullet && Kind != EFTKind::Missile);
	Extra->SetVisibility(false);
	Extra->SetRelativeTransform(FTransform::Identity);
	Charge = 0.f; YetiPhase = 0.f;
	Trail->Deactivate();
	Trail->SetRelativeLocation(FVector::ZeroVector);
	Trail2->Deactivate();
	Trail2->SetRelativeLocation(FVector::ZeroVector);
	Mark->SetVisibility(false);
	bHostile = false; GlowOffT = 1.f; GlowOnT = 0.f;
	Light->SetVisibility(false);
	Light->SetRelativeLocation(FVector::ZeroVector);
	SetActorHiddenInGame(false);
	// 3.3: whatever this pooled actor carried last time goes away
	for (UStaticMeshComponent* C : Bits) if (C) { C->SetVisibility(false); C->EmptyOverrideMaterials(); C->SetRelativeTransform(FTransform::Identity); C->SetUsingAbsoluteLocation(false); C->SetUsingAbsoluteRotation(false); C->SetUsingAbsoluteScale(false); }
	Extra->SetUsingAbsoluteLocation(false); Extra->SetUsingAbsoluteRotation(false); Extra->SetUsingAbsoluteScale(false);
	Extra->EmptyOverrideMaterials();
	WheelSpec.Reset();
	for (UStaticMeshComponent* C : Wheels) if (C) C->SetVisibility(false);
	for (UStaticMeshComponent* C : Struts) if (C) C->SetVisibility(false);
	LastGlow = -1.f;
	// overhaul: a boss's rig starts from rest, and whatever this pooled actor's last life left running is stopped
	for (UNiagaraComponent* N : BossFx) if (N) N->Deactivate();
	bBossLive = false; BossHp = 1.f; BossPhase = 0; HurtT = HurtFxT = DmgT = FireT = RigC = 9.f; HurtAmt = 0.f; DmgState = 0;
	WindKind = FireKind = FireCount = 0; Wind = AimX = IntroT = BossAlert = RigA = RigB = RigD = RigE = 0.f; AlertSet = -9.f; bWindSet = false;
	NumCharges = NumArcs = 0; LastSpeed = 0.f; RigF = RigG = RigH = Lunge = 0.f; RigN = 0; bLunge = bLungeSet = false;
	Cue[0] = Cue[1] = Cue[2] = Cue[3] = 0.f;
	SRoll = SRollV = SPitch = SPitchV = SHeave = SHeaveV = 0.f; bSusInit = false; WheelSpin = 0.f; TeslaCharge = 0.f; PropSpin = 0.f; bGhostCar = false;
	BumpPhase = FMath::FRandRange(0.f, 100.f);
	// separate wheels on every car, truck and boss whose body was exported without them
	if (Kind == EFTKind::Rammer || Kind == EFTKind::Suv || Kind == EFTKind::Truck || Kind == EFTKind::Civ || Kind == EFTKind::Ramp || Kind == EFTKind::Boss)
		SetupWheels(Mesh->GetStaticMesh(), Kind == EFTKind::Civ);
	auto Bit = [this](int32 I) -> UStaticMeshComponent*
	{
		while (Bits.Num() <= I) Bits.Add(MakeBit(this, Mesh));
		Bits[I]->SetVisibility(true);
		return Bits[I];
	};
	// 2.8: THE ABOMINABLE's arms are their own model, pivoting at the shoulders, so he can wave them over his head
	if (Kind == EFTKind::Boss && Level == 5)
	{
		if (UStaticMesh* Arms = Load<UStaticMesh>(TEXT("/Game/Characters/Bosses/SM_YetiArms")))
		{
			Extra->SetStaticMesh(Arms);
			Extra->SetRelativeLocation(FVector(0.f, 0.f, YetiShoulderZ));
			Extra->SetVisibility(true);
		}
	}
	// 3.3 the Tesla truck (Chemical Works boss): the coil's top spins and glows; three arcs strike from it
	if (Kind == EFTKind::Boss && Level == 1)
	{
		if (UStaticMesh* Top = Kept(TEXT("/Game/Characters/Bosses/SM_TeslaTop")))
		{
			UStaticMeshComponent* T = Bit(0);
			T->SetStaticMesh(Top); T->SetRelativeLocation(TeslaTop); T->SetCastShadow(true);
		}
		for (int32 a = 0; a < 3; ++a)
		{
			UStaticMeshComponent* A = Bit(1 + a);
			A->SetStaticMesh(Kept(TEXT("/Game/Characters/Bosses/SM_TeslaArc")));
			A->SetCastShadow(false);
			A->SetUsingAbsoluteLocation(true); A->SetUsingAbsoluteRotation(true); A->SetUsingAbsoluteScale(true);
			A->SetVisibility(false);
		}
	}
	// 3.3 MR. CHUCKLES flies: a propeller rig bolted on the clown car
	if (Kind == EFTKind::Boss && Level == 4)
	{
		if (UStaticMesh* Rig = Kept(TEXT("/Game/Characters/Bosses/SM_ClownRig"))) { UStaticMeshComponent* R = Bit(0); R->SetStaticMesh(Rig); R->SetCastShadow(true); }
		UStaticMesh* Prop = Kept(TEXT("/Game/Characters/Bosses/SM_ClownProp"));
		for (int32 p = 0; p < NumClownProps && Prop; ++p) { UStaticMeshComponent* Pc = Bit(1 + p); Pc->SetStaticMesh(Prop); Pc->SetRelativeLocation(ClownProps[p]); Pc->SetCastShadow(false); }
	}
	if (Kind == EFTKind::Heli)
	{
		Extra->SetStaticMesh(Load<UStaticMesh>(TEXT("/Game/Characters/Choppers/SM_HeliRotor")));
		Extra->SetRelativeLocation(HeliRotorHub);
		Extra->SetVisibility(true);
		// 3.3: the tail rotor spins too
		if (UStaticMesh* Tail = Kept(TEXT("/Game/Characters/Choppers/SM_HeliTailRotor"))) { UStaticMeshComponent* Tr = Bit(0); Tr->SetStaticMesh(Tail); Tr->SetRelativeLocation(HeliTailHub); Tr->SetCastShadow(false); }
		Light->SetRelativeLocation(FVector(260.f, 0.f, 60.f));
		Light->SetLightColor(FLinearColor(1.f, 0.95f, 0.85f));
		Light->SetIntensity(900.f); Light->SetAttenuationRadius(2600.f);
		Light->SetVisibility(true);
	}
	else if (Kind == EFTKind::Henchman)
	{
		// SetRope shows the cable only when both endpoints are present.
		Extra->SetStaticMesh(Kept(TEXT("/Engine/BasicShapes/Cylinder.Cylinder")));
		Extra->SetUsingAbsoluteLocation(true);
		Extra->SetUsingAbsoluteRotation(true);
		Extra->SetUsingAbsoluteScale(true);
		if (UMaterialInterface* RopeMaterial = Load<UMaterialInterface>(TEXT("/Game/Core/VisualFinish/MI_e8dcb0_g0_m0_r60")))
		{
			Extra->SetMaterial(0, RopeMaterial);
		}
	}
	else if (Kind == EFTKind::Missile)
	{
		// 3.1: smoke straight back from the tail and a flame in it (the flame is the Extra layer: not at Low)
		Trail->SetAsset(FTFx::System(TEXT("NS_MissileTrail")));
		Trail->SetRelativeLocation(FVector(-70.f, 0.f, 0.f));
		if (Trail->GetAsset()) Trail->Activate(true);
		Trail2->SetAsset(FTFx::Shows(this, FTFx::Extra) ? FTFx::System(TEXT("NS_MissileFlame")) : nullptr);
		Trail2->SetRelativeLocation(FVector(-62.f, 0.f, 0.f));
		if (Trail2->GetAsset()) Trail2->Activate(true);
		Light->SetLightColor(FLinearColor(1.f, 0.55f, 0.2f));
		Light->SetIntensity(400.f); Light->SetAttenuationRadius(900.f);
		Light->SetVisibility(true);
	}
	else if (Kind == EFTKind::Bullet)
	{
		Mesh->SetRelativeScale3D(FVector(1.f));
		if (Variant == 2)
		{
			Light->SetLightColor(FLinearColor(1.f, 0.5f, 0.15f));
			Light->SetIntensity(300.f); Light->SetAttenuationRadius(700.f);
			Light->SetRelativeLocation(FVector::ZeroVector);
			Light->SetVisibility(true);
		}
	}
	else if (Kind == EFTKind::Barrel)
	{
		// 3.1: a barrel had nothing that said "do not touch". Every barrel carries a red warning light now (pulsed in
		// Animate), and Mount Kaldera's are alight, as the fire engine's "burning barrels" always claimed to be.
		// The barrel itself, its box and its blast are what they were
		if (Level == 3)
		{
			Trail->SetAsset(FTFx::System(TEXT("NS_BarrelFire")));
			Trail->SetRelativeLocation(FVector(0.f, 0.f, 95.f));
			if (Trail->GetAsset()) Trail->Activate(true);
		}
		Light->SetLightColor(Level == 3 ? FLinearColor(1.f, 0.5f, 0.15f) : FLinearColor(1.f, 0.16f, 0.1f));
		Light->SetIntensity(240.f); Light->SetAttenuationRadius(650.f);
		Light->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
		Light->SetVisibility(!FTFx::IsLow());
	}
	else if (Kind == EFTKind::Crate || Kind == EFTKind::Wrench)
	{
		Light->SetLightColor(Kind == EFTKind::Crate ? FLinearColor(1.f, 0.55f, 0.15f) : FLinearColor(0.3f, 1.f, 0.5f));
		Light->SetIntensity(PickupLightIntensity()); Light->SetAttenuationRadius(700.f);
		// The former 250 cd source at Z120 intersected the new handle/noses and blew out their paint.
		Light->SetRelativeLocation(FVector(0, 0, 220.f));
		Light->SetVisibility(PickupLightIntensity() > 0.f);
	}
	else if (Kind == EFTKind::Boss)
	{
		static const FLinearColor BC[8] = { FLinearColor(1.f, 0.15f, 0.1f), FLinearColor(0.4f, 1.f, 0.6f), FLinearColor(1.f, 0.15f, 0.1f), FLinearColor(1.f, 0.35f, 0.08f),
			FLinearColor(1.f, 0.3f, 0.8f), FLinearColor(0.6f, 0.85f, 1.f), FLinearColor(0.3f, 0.9f, 1.f), FLinearColor(1.f, 0.8f, 0.3f) };
		Light->SetLightColor(BC[FMath::Clamp(Level, 0, 7)]);
		Light->SetIntensity(1500.f); Light->SetAttenuationRadius(2500.f);
		Light->SetRelativeLocation(FVector(-600.f, 0, 300.f));
		Light->SetVisibility(true);
		RigSetup(*this);   // overhaul: this boss's own moving parts
	}
}

void AFTEntityActor::SetGlow(float Amount)
{
	// 3.3 perf: the game mode asks every frame for rammers, hogs and bosses; only touch the materials on a change
	// (each call walks every slot and pushes new parameter data to the render thread)
	if (FMath::IsNearlyEqual(Amount, LastGlow, 0.01f)) return;
	// (overhaul: a boss's lit parts are written by its rig, which adds its own to this glow: RigAnimate)
	if (Kind != EFTKind::Boss) Mesh->SetScalarParameterValueOnMaterials(TEXT("Alert"), Amount);
	// 3.1: the wind-up also shows as a flare when it starts and as a red light while it lasts. Both follow the same
	// Glow the attack's own timers drive, so neither can start before the warning or outstay it
	const bool bOn = Amount > 0.5f, bWas = LastGlow > 0.5f;
	LastGlow = Amount;
	if (Kind != EFTKind::Rammer && Kind != EFTKind::Hog && Kind != EFTKind::Boss) return;
	if (bOn && !bWas && GlowOffT > 0.35f)
	{
		// where it comes from: a rammer and a boss drop back at you tail first, a Snake Eyes swings from the saddle
		const FVector At = Kind == EFTKind::Hog ? FVector(0.f, 0.f, 140.f) : (Kind == EFTKind::Boss ? FVector(-620.f, 0.f, 220.f) : FVector(-230.f, 0.f, 110.f));
		FTFx::Attach(TEXT("NS_AlertFlare"), Mesh, At, FRotator::ZeroRotator, Kind == EFTKind::Boss ? 2.2f : 1.f, FTFx::Main, true);
		if (Kind == EFTKind::Rammer) FTFx::Attach(TEXT("NS_RevSmoke"), Mesh, FVector(-200.f, 0.f, 20.f), FRotator::ZeroRotator, 1.f, FTFx::Extra, true);
	}
	if (Kind == EFTKind::Boss) Light->SetIntensity(1500.f + 1800.f * FMath::Clamp(Amount, 0.f, 1.f));
	else
	{
		Light->SetLightColor(FLinearColor(1.f, 0.12f, 0.1f));
		Light->SetIntensity(520.f * FMath::Clamp(Amount, 0.f, 1.f)); Light->SetAttenuationRadius(900.f);
		Light->SetRelativeLocation(FVector(Kind == EFTKind::Hog ? 0.f : -230.f, 0.f, 130.f));
		Light->SetVisibility(bOn && !FTFx::IsLow());
	}
}

void AFTEntityActor::SetHostile(int32 InVariant)
{
	bHostile = true;
	static const FLinearColor Theirs(1.f, 0.07f, 0.26f);
	const TCHAR* TrailName = TEXT("NS_EnemyTrail");
	int32 Layer = FTFx::Main;
	float Back = 40.f;
	if (InVariant == 0)
	{
		// the stock tracer is the KESTREL's own yellow, so a foe's shot and yours looked the same: theirs is theirs
		Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(Theirs.R, Theirs.G, Theirs.B));
		Mesh->SetVectorParameterValueOnMaterials(TEXT("Tint"), FVector(Theirs.R, Theirs.G, Theirs.B));
		bTracerColoured = true;
	}
	else if (InVariant == 1) { TrailName = TEXT("NS_FishTrail"); Layer = FTFx::Extra; Back = 110.f; }
	else if (InVariant == 2) { TrailName = TEXT("NS_FireballTrail"); Back = 70.f; }
	else if (InVariant == 3) { TrailName = TEXT("NS_BigTracerTrail"); Back = 110.f; }
	else if (InVariant == 4) { TrailName = TEXT("NS_PieTrail"); Layer = FTFx::Extra; Back = 30.f; }
	Trail->SetAsset(FTFx::Shows(this, Layer) ? FTFx::System(TrailName) : nullptr);
	Trail->SetRelativeLocation(FVector(-Back, 0.f, 0.f));
	if (Trail->GetAsset()) Trail->Activate(true);
	// no light on a hostile round: on this road a light is a wide banded pool, and a magenta pool sliding along the
	// tarmac reads as a place that hurts (first captures). The streak, the muzzle star and the hit burst carry the colour
}

void AFTEntityActor::SetLandingMark(const FTransform& WorldT, float HalfAlong, float HalfAcross, float Progress)
{
	UMaterialInterface* M0 = Mark->GetMaterial(0);
	if (!M0 || !M0->GetBaseMaterial() || !M0->GetBaseMaterial()->GetName().Contains(TEXT("Hazard")))
	{
		if (UMaterialInterface* M = Load<UMaterialInterface>(TEXT("/Game/FX/MI_Hazard"))) Mark->SetMaterial(0, M);
	}
	// the engine's plane is 100 cm square: scaled to the real box the landing hits
	Mark->SetWorldTransform(FTransform(WorldT.GetRotation(), WorldT.GetLocation(), FVector(HalfAlong / 50.f, HalfAcross / 50.f, 1.f)));
	const float P = FMath::Clamp(Progress, 0.f, 1.f);
	Mark->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.f, 0.3f, 0.2f));
	Mark->SetScalarParameterValueOnMaterials(TEXT("Intensity"), FMath::Lerp(0.85f, 1.9f, P) + 0.32f * FMath::Sin(AnimT * 23.f));
	Mark->SetScalarParameterValueOnMaterials(TEXT("Style"), 2.f);
	Mark->SetScalarParameterValueOnMaterials(TEXT("Progress"), P);
	Mark->SetScalarParameterValueOnMaterials(TEXT("Live"), 0.f);
	Mark->SetScalarParameterValueOnMaterials(TEXT("Mark"), 1.f);
	Mark->SetVectorParameterValueOnMaterials(TEXT("Size"), FVector(HalfAlong * 2.f, HalfAcross * 2.f, 0.f));
	Mark->SetVisibility(true);
}

void AFTEntityActor::HideLandingMark()
{
	Mark->SetVisibility(false);
}

void AFTEntityActor::SetActorHiddenInGame(bool bNewHidden)
{
	// overhaul: the fight's boss is being let go (Release): if it was wrecked it comes apart its own way, before it goes
	if (bNewHidden && Kind == EFTKind::Boss && bBossLive) { bBossLive = false; RigRelease(*this); }
	Super::SetActorHiddenInGame(bNewHidden);
	// back to the pool: a landing mark of the last life may not show on the next. The second trail is left alone,
	// as the first is (Setup stops both when the actor is used again): the highlight reel hides every ghost each
	// frame and shows the ones in shot, and a ghost missile whose flame was stopped here lost it after one frame (review)
	if (bNewHidden && Mark) Mark->SetVisibility(false);
}

const TArray<AFTEntityActor::FWheelDef>* AFTEntityActor::WheelsFor(const UStaticMesh* M)
{
	if (!M) return nullptr;
	return WheelTable().Find(M->GetPackage()->GetName());
}

bool AFTEntityActor::SetupWheels(UStaticMesh* BodyMesh, bool bChrome)
{
	const TArray<FWheelDef>* L = WheelsFor(BodyMesh);
	if (!L || L->Num() == 0) return false;
	WheelSpec = *L;
	UStaticMesh* WM = Kept(bChrome ? TEXT("/Game/Characters/Wheels/SM_FoeWheelChrome") : TEXT("/Game/Characters/Wheels/SM_FoeWheel"));
	if (!WM) WM = Kept(TEXT("/Game/Characters/Wheels/SM_FoeWheel"));
	UStaticMesh* SM = Kept(TEXT("/Game/Characters/Wheels/SM_FoeStrut"));
	if (!WM) { WheelSpec.Reset(); return false; }
	// struts only on cars (4 wheels): a truck's axles would double its draw calls for something you can't see
	const bool bStruts = SM && WheelSpec.Num() <= 4;
	for (int32 i = 0; i < WheelSpec.Num(); ++i)
	{
		while (Wheels.Num() <= i) Wheels.Add(MakeBit(this, RootComponent));
		UStaticMeshComponent* W = Wheels[i];
		// 3.3 perf (round 2): the wheels sit inside the body's own shadow; only a boss's big wheels cast one (every
		// extra non-Nanite caster costs Virtual Shadow Map marking, and Bayshore already overflows that queue)
		W->SetCastShadow(Kind == EFTKind::Boss);
		const FWheelDef& D = WheelSpec[i];
		if (W->GetStaticMesh() != WM) W->SetStaticMesh(WM);
		// the wheel model is a left wheel, 50 cm radius and 40 wide: scaled to size, mirrored on the right
		W->SetRelativeLocationAndRotation(D.C, FRotator::ZeroRotator);
		W->SetRelativeScale3D(FVector(D.R / 50.f, (D.C.Y > 0.f ? -1.f : 1.f) * D.W / 40.f, D.R / 50.f));
		W->SetVisibility(true);
		if (bStruts)
		{
			while (Struts.Num() <= i) { UStaticMeshComponent* C = MakeBit(this, RootComponent); C->SetCastShadow(false); Struts.Add(C); }
			if (Struts[i]->GetStaticMesh() != SM) Struts[i]->SetStaticMesh(SM);
			Struts[i]->SetVisibility(true);
		}
	}
	return true;
}

void AFTEntityActor::SetupGhostCar(AFTPlayerCar* From)
{
	if (!From) return;
	Setup(EFTKind::None, 0, Lvl);
	Mesh->SetStaticMesh(From->Body->GetStaticMesh());
	for (int32 m = 0; m < From->Body->GetNumMaterials(); ++m) Mesh->SetMaterial(m, From->Body->GetMaterial(m));
	// the reel records the chassis (the actor); the body hangs below it, and the wheels where they sit on the car
	// (3.2 and before showed the body alone, without its wheels, 88 cm up in the air)
	const FTransform ActInv = From->GetActorTransform().Inverse();
	const FTransform BodyRel = From->Body->GetComponentTransform() * ActInv;
	Mesh->SetRelativeTransform(FTransform(FRotator::ZeroRotator, BodyRel.GetLocation()));
	for (int32 i = 0; i < From->WheelMeshes.Num(); ++i)
	{
		while (Wheels.Num() <= i) Wheels.Add(MakeBit(this, RootComponent));
		UStaticMeshComponent* W = Wheels[i];
		W->SetStaticMesh(From->WheelMeshes[i]->GetStaticMesh());
		const FTransform Rel = From->WheelMeshes[i]->GetComponentTransform() * ActInv;
		W->SetRelativeTransform(FTransform(FRotator::ZeroRotator, Rel.GetLocation(), From->WheelMeshes[i]->GetRelativeScale3D()));
		W->SetVisibility(true);
		FWheelDef D; D.C = Rel.GetLocation(); D.R = i < 2 ? 31.f : 40.f; D.W = 0.f;
		WheelSpec.Add(D);
	}
	bGhostCar = true;
}

FVector AFTEntityActor::RopeAnchor(int32 Side) const
{
	return Mesh->GetComponentTransform().TransformPosition(HeliRope[Side ? 1 : 0]);
}

void AFTEntityActor::SetRope(const FVector& WorldTop, float Progress, int32 Side)
{
	const FVector Anchor = GetActorTransform().TransformPosition(RappelGrip);
	const FVector Cable = WorldTop - Anchor;
	const float Length = Cable.Size();
	if (Length < 8.f) { Extra->SetVisibility(false); return; }

	const float Sway = FMath::Sin(PI * Progress);
	const float Brace = FMath::SmoothStep(0.72f, 1.f, Progress);
	const float Outside = Side == 0 ? -1.f : 1.f;
	const FQuat Pose = FRotator(5.f * Sway * FMath::Sin(AnimT * 7.f) - 9.f * Brace,
		Outside * (14.f * Brace + 5.f * Sway), Outside * (6.f * Brace + 8.f * Sway * FMath::Sin(AnimT * 5.f))).Quaternion();
	const FVector Scale(1.f, 1.f, 1.f - 0.075f * Brace);
	// Rotate and compress around the rope joint so the hands and cable never separate.
	Mesh->SetRelativeTransform(FTransform(Pose, RappelGrip - Pose.RotateVector(Scale * RappelGrip), Scale));
	Extra->SetWorldLocationAndRotation(Anchor + Cable * 0.5f, FRotationMatrix::MakeFromZ(Cable / Length).Rotator());
	Extra->SetWorldScale3D(FVector(0.06f, 0.06f, Length / 100.f));
	Extra->SetVisibility(true);
}

void AFTEntityActor::KickBike()
{
	if (Kind == EFTKind::Moto || Kind == EFTKind::Hog) BikeHit = 1.f;
}

void AFTEntityActor::SetTesla(float InCharge, const TArray<FVector>& ArcTo, const TArray<float>* Rising)
{
	if (Kind != EFTKind::Boss || Lvl != 1 || Bits.Num() < 4) return;
	TeslaCharge = FMath::Clamp(InCharge, 0.f, 1.f);
	// overhaul: each strike still charging is a collar on the coil; a strike going live is the release, and the coil
	// is spent for a moment when the last one has ended (AnimAlchemist)
	const int32 WasCharging = NumCharges;
	NumCharges = 0;
	if (Rising) for (float R : *Rising) if (NumCharges < 3) Charges[NumCharges++] = FMath::Clamp(R, 0.f, 1.f);
	if (NumCharges > 0 && WasCharging == 0) { UE_LOG(LogFT, Display, TEXT("FT BOSSRIG wind level=%d kind=1"), Lvl); }
	if (ArcTo.Num() > NumArcs) BossFire(1, ArcTo.Num());
	else if (ArcTo.Num() == 0 && NumArcs > 0) RigC = 0.f;
	NumArcs = ArcTo.Num();
	UStaticMeshComponent* Top = Bits[0];
	if (Top && Top->GetStaticMesh())
	{
		// the top glows up as it charges, and crackles (flicker) near full
		const float Flick = TeslaCharge > 0.7f ? FMath::FRandRange(0.7f, 1.3f) : 1.f;
		Top->SetScalarParameterValueOnMaterials(TEXT("Glow"), (0.25f + 3.2f * TeslaCharge * TeslaCharge) * Flick * FMath::Max(RigD, 0.04f));
	}
	Light->SetLightColor(FLinearColor(0.45f, 0.8f, 1.f));
	Light->SetRelativeLocation(TeslaTop);
	Light->SetIntensity((400.f + 5200.f * TeslaCharge * TeslaCharge) * RigD + (ArcTo.Num() ? 9000.f : 0.f) + 2600.f * Pulse(HurtT, 14.f) * HurtAmt);
	Light->SetAttenuationRadius(3200.f);
	Light->SetVisibility(true);
	const FVector From = Top ? Top->GetComponentLocation() : GetActorTransform().TransformPosition(TeslaTop);
	for (int32 a = 0; a < 3; ++a)
	{
		UStaticMeshComponent* A = Bits[1 + a];
		if (!A || !A->GetStaticMesh() || !ArcTo.IsValidIndex(a)) { if (A) A->SetVisibility(false); continue; }
		const FVector Dv = From - ArcTo[a];
		const float L = Dv.Size();
		if (L < 50.f) { A->SetVisibility(false); continue; }
		// the bolt model runs from 0 down to -1000 along -Z: point its +Z from the target back up to the coil, and a
		// new random twist and thickness every frame so it crackles
		const FRotator R = FRotationMatrix::MakeFromZ(Dv / L).Rotator();
		A->SetWorldLocationAndRotation(From, FQuat(R) * FQuat(FVector::UpVector, FMath::FRandRange(0.f, 2.f * PI)));
		const float Th = FMath::FRandRange(1.6f, 3.2f);   // thick enough to read from 50 m back
		A->SetWorldScale3D(FVector(Th, Th, L / 1000.f));
		A->SetVisibility(true);
	}
}

void AFTEntityActor::Animate(float Dt, float Speed, float Vx, float BikeWindup)
{
	if (Kind == EFTKind::Heli && Extra) Extra->AddLocalRotation(FRotator(0.f, 1900.f * Dt, 0.f));
	if (Kind == EFTKind::Heli && Bits.Num() > 0 && Bits[0] && Bits[0]->IsVisible()) Bits[0]->AddLocalRotation(FRotator(2600.f * Dt, 0.f, 0.f));
	// 3.3 propellers on the clown car, the Tesla coil's top turning faster as it charges
	// (overhaul: the clown car's propellers are turned by MR. CHUCKLES' rig, below: AnimChuckles)
	AnimT += Dt;   // (overhaul: the Tesla coil's top is posed by the Alchemist's rig, below)
	if (Kind == EFTKind::Moto || Kind == EFTKind::Hog)
	{
		if (!bSusInit) { PrevSp = Speed; bSusInit = true; }
		const float SpeedChange = FMath::Clamp((Speed - PrevSp) / FMath::Max(Dt, 0.001f), -4000.f, 4000.f);
		PrevSp = Speed;
		AccLong = FMath::FInterpTo(AccLong, SpeedChange, Dt, 8.f);
		BumpPhase += Dt * FMath::Clamp(FMath::Abs(Speed) / 500.f, 0.f, 13.f);
		const float Recoil = BikeHit * FMath::Sin(BikeHit * PI * 1.5f);
		BikeHit = FMath::Max(0.f, BikeHit - 3.5f * Dt);
		const float Pitch = FMath::Clamp(AccLong * 0.00035f + 0.35f * FMath::Sin(BumpPhase)
			- 1.8f * BikeWindup + 1.7f * Recoil, -2.5f, 2.5f);
		const float Bob = 1.f + FMath::Sin(BumpPhase * 1.6f);
		const float AxleSpan = Kind == EFTKind::Hog ? 202.f : 170.f;
		const float WheelClearance = AxleSpan * FMath::Abs(FMath::Sin(FMath::DegreesToRadians(Pitch)));
		Mesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, Bob + WheelClearance),
			FRotator(Pitch, 2.4f * Recoil, 0.f));
	}
	// 3.3 the wheels roll on the road and the body rides its springs over them (roll out of a turn, squat and dive,
	// a little road bounce): the owner's "the cars are so stiff, like they don't really attach to the road"
	if (WheelSpec.Num() && !bGhostCar)
	{
		if (!bSusInit) { PrevSp = Speed; PrevVx = Vx; bSusInit = true; }
		const float Idt = 1.f / FMath::Max(Dt, 1e-3f);
		AccLat = FMath::FInterpTo(AccLat, FMath::Clamp((Vx - PrevVx) * Idt, -4000.f, 4000.f), Dt, 8.f);
		AccLong = FMath::FInterpTo(AccLong, FMath::Clamp((Speed - PrevSp) * Idt, -4000.f, 4000.f), Dt, 8.f);
		PrevSp = Speed; PrevVx = Vx;
		const bool bBig = WheelSpec.Num() > 4 || Kind == EFTKind::Boss;
		const float K = bBig ? 0.6f : 1.f;   // heavy trucks lean less
		BumpPhase += Dt * FMath::Clamp(FMath::Abs(Speed) / 900.f, 0.f, 9.f);
		const float Bump = 1.6f * FMath::Sin(BumpPhase * 1.7f) + 0.9f * FMath::Sin(BumpPhase * 4.3f + 1.f);
		const float RollT = FMath::Clamp(-AccLat / 980.f * 4.5f * K, -6.f, 6.f) + 0.25f * FMath::Sin(BumpPhase * 2.9f);
		const float PitchT = FMath::Clamp(AccLong / 980.f * 3.f * K, -4.f, 4.f) + 0.2f * FMath::Sin(BumpPhase * 2.3f + 0.5f);
		// overhaul: in steps of a sixtieth at most. One long frame (a loading stall, a screenshot) used to throw the
		// body metres off its wheels: this explicit step is only stable under about a tenth of a second
		auto Spring = [Dt](float& X, float& V, float T, float W, float Z)
		{
			const int32 Steps = FMath::Clamp(FMath::CeilToInt(Dt * 60.f - 0.05f), 1, 12);
			const float H = FMath::Min(Dt, 0.2f) / Steps;
			for (int32 k = 0; k < Steps; ++k) { V += (W * W * (T - X) - 2.f * Z * W * V) * H; X += V * H; }
			if (!FMath::IsFinite(X)) { X = 0.f; V = 0.f; }
		};
		Spring(SRoll, SRollV, RollT, 9.f, 0.3f);
		Spring(SPitch, SPitchV, PitchT, 9.f, 0.3f);
		Spring(SHeave, SHeaveV, Bump * K, 14.f, 0.4f);
		// the yeti, the clown car, the shark and the limo animate their own bodies
		const bool bOwnAnim = Kind == EFTKind::Boss && (Lvl == 4 || Lvl == 5 || Lvl == 6 || Lvl == 7);
		if (!bOwnAnim)
		{
			const FRotator Rot(SPitch, 0.f, SRoll);
			const FVector Pivot(0.f, 0.f, 50.f);
			Mesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, SHeave) + Pivot - Rot.RotateVector(Pivot), Rot);
		}
		WheelSpin += Speed * Dt;   // cm rolled
		// 3.3 perf: far down the road (a few pixels tall) the wheels update every third frame and the struts not at all
		const bool bSkipWheels = bFar && (++FarTick % 3) != 0;
		const FTransform BodyRel = Mesh->GetRelativeTransform();
		for (int32 i = 0; !bSkipWheels && i < WheelSpec.Num() && i < Wheels.Num(); ++i)
		{
			const FWheelDef& D = WheelSpec[i];
			// each wheel follows the road under it with its own little bump (out of phase with the body's)
			const float Wb = 0.8f * FMath::Sin(BumpPhase * 3.1f + i * 1.9f);
			const float Steer = (D.C.X > 0.f && !bBig) ? FMath::Clamp(Vx / 60.f, -14.f, 14.f) : 0.f;
			Wheels[i]->SetRelativeLocationAndRotation(D.C + FVector(0, 0, Wb), FRotator(-FMath::RadiansToDegrees(WheelSpin / FMath::Max(D.R, 1.f)), Steer, 0.f));
			if (!bFar && Struts.IsValidIndex(i) && Struts[i] && Struts[i]->IsVisible())
			{
				// from the hub (inboard) up to the body's wheel arch, wherever the body has rocked to
				const float In = D.C.Y > 0.f ? -1.f : 1.f;
				const FVector Hub = D.C + FVector(0.f, In * D.W * 0.55f, Wb);
				const FVector Top = BodyRel.TransformPosition(FVector(D.C.X, D.C.Y + In * D.W * 0.75f, D.C.Z + D.R * 1.05f));
				const FVector Dv = Top - Hub;
				const float L = FMath::Max(Dv.Size(), 1.f);
				Struts[i]->SetRelativeLocationAndRotation(Hub, FRotationMatrix::MakeFromZ(Dv / L).Rotator());
				Struts[i]->SetRelativeScale3D(FVector(1.f, 1.f, L / 100.f));
			}
		}
	}
	else if (bGhostCar)
	{
		WheelSpin += Speed * Dt;
		for (int32 i = 0; i < WheelSpec.Num() && i < Wheels.Num(); ++i)
			Wheels[i]->SetRelativeRotation(FRotator(-FMath::RadiansToDegrees(WheelSpin / FMath::Max(WheelSpec[i].R, 1.f)), 0.f, 0.f));
	}
	// 3.1: how long the wind-up glow has been on or off (SetGlow reads these)
	if (LastGlow > 0.5f) { GlowOnT += Dt; GlowOffT = 0.f; } else { GlowOffT += Dt; GlowOnT = 0.f; }
	if (Kind == EFTKind::Rammer)
	{
		// the charge is a steady glow (the rev before it blinks): the rammer throws dust for as long as it lasts
		const bool bCharge = GlowOnT > 0.2f;
		if (bCharge && !Trail->IsActive())
		{
			Trail->SetAsset(FTFx::Shows(this, FTFx::Extra) ? FTFx::System(TEXT("NS_ChargeTrail")) : nullptr);
			Trail->SetRelativeLocation(FVector(-200.f, 0.f, 15.f));
			if (Trail->GetAsset()) Trail->Activate(true);
		}
		else if (!bCharge && Trail->IsActive()) Trail->Deactivate();
	}
	if (Kind == EFTKind::Barrel && Light->IsVisible()) Light->SetIntensity(Lvl == 3 ? 260.f + 90.f * FMath::Sin(AnimT * 11.f) : 150.f + 130.f * FMath::Max(0.f, FMath::Sin(AnimT * 7.f)));
	if (Kind == EFTKind::Crate || Kind == EFTKind::Wrench)
	{
		// Visual component only: logical track position, hitbox, spawn schedule and RNG stay untouched.
		const float Phase = (Kind == EFTKind::Wrench ? 1.7f : 0.f);
		const bool bMotion = CVarPickupMotion.GetValueOnGameThread() != 0;
		const float Intensity = PickupLightIntensity();
		if (!FMath::IsNearlyEqual(Light->Intensity, Intensity)) Light->SetIntensity(Intensity);
		if (Light->IsVisible() != (Intensity > 0.f)) Light->SetVisibility(Intensity > 0.f);
		Mesh->SetRelativeLocationAndRotation(bMotion ? FVector(0, 0, 10.f + 6.f * FMath::Sin(AnimT * 2.1f + Phase)) : FVector::ZeroVector,
			bMotion ? FRotator(0, 18.f * FMath::Sin(AnimT * 0.7f + Phase), 0) : FRotator::ZeroRotator);
	}
	// 2.5 bosses with a bit of life: the yeti lopes, the clown car bounces and wobbles, the limo hovers
	if (Kind == EFTKind::Boss)
	{
		// (overhaul: the yeti's lope, the clown car's flight, the shark's swim and the limo's hover are in their rigs now:
		// AnimYeti, AnimChuckles, AnimShark, AnimChairman, where the same motion also answers the fight)
		RigAnimate(*this, Dt, Speed, Vx);   // overhaul: the boss's moving parts, its damage and its lights
	}
	// fish wiggle on their way in
	if (Kind == EFTKind::Bullet && Variant == 1) Mesh->SetRelativeRotation(FRotator(0.f, 18.f * FMath::Sin(AnimT * 18.f), 0.f));
	// 3.1: and a pie spins face first
	if (Kind == EFTKind::Bullet && Variant == 4) Mesh->SetRelativeRotation(FRotator(0.f, 0.f, 540.f * AnimT));
}

// ------------------------------------------------------------------------------------------ wreck
// (round 3's wreck pieces need these; they are here, with the only code that uses them)
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"

namespace
{
	// FX round 3: a wreck comes apart. A wheel and a couple of panels are thrown off it and bounce down the road on
	// their own, and where the body drags on the road it throws sparks. The pieces are the wreck's own components,
	// moved here by hand: no collision, no physics body, nothing the race reads. They go with the wreck (nine
	// seconds, or at once when FTFx::ClearWorld removes the wrecks at a slate or the reel). The state is kept here,
	// by wreck, so the header is as it was.
	struct FWreckPiece
	{
		TWeakObjectPtr<UStaticMeshComponent> C;
		FVector P = FVector::ZeroVector, V = FVector::ZeroVector, Scale = FVector::OneVector;
		FRotator Spin = FRotator::ZeroRotator;
		float Rest = 20.f, Age = 0.f, Life = 2.f;
		bool bWheel = false;
	};
	struct FWreckLook
	{
		TArray<FWreckPiece> Pieces;
		TWeakObjectPtr<UNiagaraComponent> Scrape;
		FVector GroundP = FVector::ZeroVector, GroundN = FVector::UpVector;
		bool bGround = false;
		float HalfHeight = 60.f;
		// FX round 4: the burning trail it leaves while it slides (FTFx::WreckTrail): where its next length starts
		float HalfWidth = 80.f;
		FVector TrailFrom = FVector::ZeroVector;
		bool bTrail = false;
	};
	TMap<TWeakObjectPtr<AFTWreck>, FWreckLook>& WreckLooks()
	{
		static TMap<TWeakObjectPtr<AFTWreck>, FWreckLook> M;
		return M;
	}
	// looks draw from their own stream, never from the generator the race uses
	FRandomStream& WreckRand()
	{
		static FRandomStream R(7919);
		return R;
	}

	/** At launch: what comes off this wreck, under the caps (24 pieces in the air over every wreck, 4 a wreck;
	 *  10 and 2 at Low; sparks on 6 wrecks at once, none at Low). */
	void StartWreckLook(AFTWreck* W, UStaticMesh* InMesh, const FTransform& At, const FVector& Velocity, float Scale, int32 LostWheel)
	{
		int32 InAir = 0, Dragging = 0;
		for (auto It = WreckLooks().CreateIterator(); It; ++It)
		{
			if (!It->Key.IsValid()) { It.RemoveCurrent(); continue; }   // a wreck that has gone
			InAir += It->Value.Pieces.Num();
			if (It->Value.Scrape.IsValid()) ++Dragging;
		}
		const bool bLow = FTFx::IsLow();
		FWreckLook& Look = WreckLooks().Add(TWeakObjectPtr<AFTWreck>(W));
		const FBoxSphereBounds Bd = InMesh ? InMesh->GetBounds() : FBoxSphereBounds(ForceInit);
		const float Size = FMath::Clamp((float)Bd.BoxExtent.X / 230.f, 0.6f, 2.f) * Scale;
		Look.HalfHeight = FMath::Max(30.f, (float)Bd.BoxExtent.Z * Scale);
		Look.HalfWidth = FMath::Max(40.f, (float)Bd.BoxExtent.Y * Scale);
		int32 Room = FMath::Clamp((bLow ? 10 : 24) - InAir, 0, bLow ? 2 : 4);
		FRandomStream& Rnd = WreckRand();
		auto Throw = [&](UStaticMeshComponent* C, const FVector& LocalAt, const FVector& S3, float Rest, bool bWheel)
		{
			C->SetUsingAbsoluteLocation(true); C->SetUsingAbsoluteRotation(true); C->SetUsingAbsoluteScale(true);
			FWreckPiece& Pc = Look.Pieces.AddDefaulted_GetRef();
			Pc.C = C; Pc.bWheel = bWheel; Pc.Rest = Rest; Pc.Scale = S3;
			Pc.P = At.TransformPosition(LocalAt);
			// off the wreck with most of its speed, out to its own side and up
			const FVector Side = At.GetUnitAxis(EAxis::Y) * (LocalAt.Y >= 0. ? 1.f : -1.f);
			Pc.V = Velocity * Rnd.FRandRange(0.75f, 1.05f) + Side * Rnd.FRandRange(250.f, 700.f) + FVector(0., 0., Rnd.FRandRange(350.f, 800.f));
			Pc.Spin = FRotator(Rnd.FRandRange(-500.f, 500.f), Rnd.FRandRange(-300.f, 300.f), Rnd.FRandRange(-500.f, 500.f));
			Pc.Life = Rnd.FRandRange(1.5f, 2.4f);
			C->SetWorldLocationAndRotation(Pc.P, At.GetRotation());
			C->SetWorldScale3D(S3);
			--Room;
		};
		// the wheel it has lost is thrown clear (it was just missing)
		const TArray<AFTEntityActor::FWheelDef>* WL = AFTEntityActor::WheelsFor(InMesh);
		UStaticMesh* WM = WL ? Kept(TEXT("/Game/Characters/Wheels/SM_FoeWheel")) : nullptr;
		if (WM && Room > 0 && WL->IsValidIndex(LostWheel))
		{
			const AFTEntityActor::FWheelDef& D = (*WL)[LostWheel];
			UStaticMeshComponent* C = MakeBit(W, W->Mesh);
			C->SetStaticMesh(WM);
			C->SetScalarParameterValueOnMaterials(TEXT("Soot"), 0.9f);
			Throw(C, D.C, FVector(D.R / 50.f, (D.C.Y > 0.f ? -1.f : 1.f) * D.W / 40.f, D.R / 50.f) * Scale, D.R * Scale, true);
		}
		// panels: flat plates in the wreck's own paint, sooted (a truck sheds three, a car or a bike two)
		UStaticMesh* Plate = Kept(TEXT("/Engine/BasicShapes/Cube.Cube"));
		UMaterialInterface* Paint = W->Mesh->GetMaterial(0);
		const int32 Panels = Size > 1.4f ? 3 : 2;
		for (int32 i = 0; i < Panels && Room > 0 && Plate && Paint; ++i)
		{
			UStaticMeshComponent* C = MakeBit(W, W->Mesh);
			C->SetStaticMesh(Plate);
			C->SetMaterial(0, Paint);
			C->SetScalarParameterValueOnMaterials(TEXT("Soot"), 0.9f);
			C->SetCastShadow(false);
			const FVector LocalAt(Rnd.FRandRange(-0.5f, 0.5f) * Bd.BoxExtent.X, ((i & 1) ? 1.f : -1.f) * Bd.BoxExtent.Y * 0.8f, Bd.Origin.Z + Bd.BoxExtent.Z * 0.3f);
			Throw(C, LocalAt, FVector(Rnd.FRandRange(0.55f, 0.95f) * Size, Rnd.FRandRange(0.35f, 0.6f) * Size, 0.05f), 8.f, false);
		}
		// sparks where the body drags on the road (TickWreckLook puts them under it)
		if (!bLow && Dragging < 6)
		{
			if (UNiagaraSystem* Sparks = FTFx::System(TEXT("NS_WreckScrape")))
			{
				UNiagaraComponent* Sc = NewObject<UNiagaraComponent>(W);
				Sc->SetupAttachment(W->Mesh);
				Sc->SetAutoActivate(false);
				Sc->SetUsingAbsoluteLocation(true); Sc->SetUsingAbsoluteRotation(true); Sc->SetUsingAbsoluteScale(true);
				Sc->SetAsset(Sparks);
				Sc->SetTranslucentSortPriority(-5);
				Sc->RegisterComponent();
				W->AddInstanceComponent(Sc);
				Look.Scrape = Sc;
			}
		}
	}

	void TickWreckLook(AFTWreck* W, FWreckLook& Look, float Dt)
	{
		UWorld* World = W->GetWorld();
		if (!World || Dt <= 0.f) return;
		const bool bEarly = W->Age < 3.5f;
		UNiagaraComponent* Sc = Look.Scrape.Get();
		if (Look.Pieces.Num() == 0 && !bEarly && !(Sc && Sc->IsActive())) return;
		const FVector At = W->GetActorLocation();
		// the road under the wreck: one line a frame, shared by its pieces (they are never far from it)
		if (W->Age < 4.f)
		{
			FHitResult Hit;
			FCollisionObjectQueryParams What;
			What.AddObjectTypesToQuery(ECC_WorldStatic);
			What.AddObjectTypesToQuery(ECC_WorldDynamic);
			const FCollisionQueryParams Q(FName(TEXT("FTWreckRoad")), false, W);
			if (World->LineTraceSingleByObjectType(Hit, At + FVector(0., 0., 200.), At - FVector(0., 0., 3000.), What, Q))
			{
				Look.GroundP = Hit.ImpactPoint; Look.GroundN = Hit.ImpactNormal; Look.bGround = true;
			}
		}
		const FVector N = Look.bGround ? Look.GroundN : FVector::UpVector;
		FVector Lens = At; bool bLens = false;
		if (const APlayerController* PC = World->GetFirstPlayerController())
			if (PC->PlayerCameraManager) { Lens = PC->PlayerCameraManager->GetCameraLocation(); bLens = true; }
		for (int32 i = Look.Pieces.Num() - 1; i >= 0; --i)
		{
			FWreckPiece& Pc = Look.Pieces[i];
			UStaticMeshComponent* C = Pc.C.Get();
			Pc.Age += Dt;
			if (!C || Pc.Age >= Pc.Life)
			{
				if (C) C->SetVisibility(false);
				Look.Pieces.RemoveAtSwap(i);
				continue;
			}
			Pc.V -= N * (2200.f * Dt);
			Pc.P += Pc.V * Dt;
			if (Look.bGround)
			{
				const float H = (float)FVector::DotProduct(Pc.P - Look.GroundP, N) - Pc.Rest;
				const float Vn = (float)FVector::DotProduct(Pc.V, N);
				if (H < 0.f)
				{
					Pc.P -= N * H;
					if (Vn < 0.f)
					{
						// a bounce: under half the height back, the slide slowed. A wheel keeps rolling, a panel soon lies flat
						Pc.V -= N * (Vn * 1.45f);
						Pc.V -= (Pc.V - N * FVector::DotProduct(Pc.V, N)) * (Pc.bWheel ? 0.06f : 0.22f);
						Pc.Spin *= Pc.bWheel ? 0.9f : 0.5f;
					}
				}
			}
			C->SetWorldLocation(Pc.P);
			C->AddWorldRotation(Pc.Spin * Dt);
			// it shrinks away at the end, and nothing tumbles into the lens
			C->SetWorldScale3D(Pc.Scale * FMath::Clamp((Pc.Life - Pc.Age) / 0.3f, 0.f, 1.f));
			if (bLens && FVector::DistSquared(Pc.P, Lens) < FMath::Square(420.f)) C->SetVisibility(false);
		}
		// where the body drags on the road it throws sparks back along its slide
		const FVector Vel = W->Mesh->GetPhysicsLinearVelocity();
		const float Up = (float)FVector::DotProduct(At - Look.GroundP, N);
		const bool bDrag = bEarly && Look.bGround && Vel.SizeSquared() > FMath::Square(900.f) && Up < Look.HalfHeight + 70.f;
		if (Sc)
		{
			if (bDrag)
			{
				const FVector Flat = (Vel - N * FVector::DotProduct(Vel, N)).GetSafeNormal();
				Sc->SetWorldLocationAndRotation(At - N * (Up - 6.f), FRotationMatrix::MakeFromXZ(Flat.IsNearlyZero() ? FVector::ForwardVector : Flat, N).Rotator());
				if (!Sc->IsActive()) Sc->Activate(true);
			}
			else if (Sc->IsActive()) Sc->Deactivate();
		}
		// FX round 4: and it leaves a burning trail: a length of scorched road every 4 m of its slide, a lick of flame on
		// every other one (FTFx::WreckTrail has the caps: 160 lengths kept, 8 flames alight; none at Low)
		if (bDrag)
		{
			const FVector Under = At - N * Up;
			if (!Look.bTrail) { Look.bTrail = true; Look.TrailFrom = Under; }
			else if (FVector::DistSquared(Under, Look.TrailFrom) > FMath::Square(400.f))
			{
				FTFx::WreckTrail(W, Look.TrailFrom, Under, N, Look.HalfWidth * 1.3f);
				Look.TrailFrom = Under;
			}
		}
		else Look.bTrail = false;
	}
}
AFTWreck::AFTWreck()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	// 2.5: the wreck tumbles on the road but never stops the KESTREL dead (in a half pipe they all roll down to
	// the bottom of the U, right where the car drives, and the player stalled on them)
	Mesh->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Mesh->BodyInstance.bUseCCD = true;
	Mesh->bAffectDistanceFieldLighting = false;   // 2.8 perf (see AFTEntityActor)
	Mesh->bAffectDynamicIndirectLighting = false;
	Fire = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire"));
	Fire->SetupAttachment(Mesh);
	Fire->SetAutoActivate(false);
	Smoke = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Smoke"));
	Smoke->SetupAttachment(Mesh);
	Smoke->SetAutoActivate(false);
	InitialLifeSpan = 9.f;
	// FX overhaul: the fire and the smoke keep the world's own up while the wreck tumbles under them. Attached with
	// the wreck's rotation, every flame and puff swung round with the body like a flag on a stick
	Fire->SetUsingAbsoluteRotation(true);
	Smoke->SetUsingAbsoluteRotation(true);
	// FX overhaul: and they sort with the blasts' fire and smoke (FTFx.cpp: -20 and -30), under everything left at 0: a
	// foe's flare, muzzle star, streak or landing mark is never under a wreck's column. (Ten wrecks can burn at once,
	// for six seconds each: these are the longest-lived effects of a fight.)
	Fire->SetTranslucentSortPriority(-20);
	Smoke->SetTranslucentSortPriority(-30);
}

void AFTWreck::Launch(UStaticMesh* InMesh, const FTransform& At, const FVector& Velocity, const FVector& Spin, float Scale)
{
	Mesh->SetStaticMesh(InMesh);
	SetActorTransform(At);
	SetActorScale3D(FVector(Scale));
	Mesh->SetSimulatePhysics(true);
	Mesh->SetMassOverrideInKg(NAME_None, 1200.f, true);
	Mesh->SetPhysicsLinearVelocity(Velocity);
	Mesh->SetPhysicsAngularVelocityInDegrees(Spin);
	Mesh->SetScalarParameterValueOnMaterials(TEXT("Soot"), 0.9f);
	// 3.3: the bodies are exported without wheels now; the wreck keeps its wheels (one of them comes off)
	int32 LostWheel = INDEX_NONE;   // FX round 3: the wheel that comes off is thrown clear (StartWreckLook), not just missing
	if (const TArray<AFTEntityActor::FWheelDef>* WL = AFTEntityActor::WheelsFor(InMesh))
	{
		UStaticMesh* WM = Kept(TEXT("/Game/Characters/Wheels/SM_FoeWheel"));
		const int32 Lose = FMath::RandRange(0, WL->Num() - 1);
		for (int32 i = 0; i < WL->Num() && WM; ++i)
		{
			if (i == Lose && WL->Num() > 2) { LostWheel = i; continue; }
			const AFTEntityActor::FWheelDef& D = (*WL)[i];
			UStaticMeshComponent* W = MakeBit(this, Mesh);
			W->SetStaticMesh(WM);
			W->SetRelativeLocationAndRotation(D.C, FRotator(FMath::FRandRange(0.f, 360.f), 0.f, 0.f));
			W->SetRelativeScale3D(FVector(D.R / 50.f, (D.C.Y > 0.f ? -1.f : 1.f) * D.W / 40.f, D.R / 50.f));
			W->SetScalarParameterValueOnMaterials(TEXT("Soot"), 0.9f);
			Wheels.Add(W);
		}
	}
	// FX overhaul: a wreck burns as the effects sheet draws it, in banded flames with pointed tongues (NS_WreckFire;
	// NS_Fire's white-hot puffs on a build without it), sized to what is burning: a truck's fire is not a bike's
	UNiagaraSystem* FireSys = FTFx::System(TEXT("NS_WreckFire"));
	const float Burn = InMesh ? FMath::Clamp((float)InMesh->GetBounds().BoxExtent.X / 230.f, 0.75f, 2.2f) : 1.f;
	Fire->SetAsset(FireSys ? FireSys : FTFx::System(TEXT("NS_Fire")));
	Fire->SetRelativeScale3D(FVector(FireSys ? Burn : 1.f));
	if (Fire->GetAsset()) { Fire->SetVariableFloat(TEXT("User.Size"), 1.4f); Fire->Activate(true); }
	StartWreckLook(this, InMesh, At, Velocity, Scale, LostWheel);   // FX round 3: it comes apart (looks only)
	// 3.1: and it smokes (the Extra layer: not at Low)
	if (FTFx::Shows(this, FTFx::Extra))
	{
		Smoke->SetAsset(FTFx::System(TEXT("NS_WreckSmoke")));
		Smoke->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
		if (Smoke->GetAsset()) Smoke->Activate(true);
	}
}

void AFTWreck::Tick(float Dt)
{
	Super::Tick(Dt);
	Age += Dt;
	if (Age > 6.f && Fire->IsActive()) Fire->Deactivate();
	if (Age > 7.5f && Smoke->IsActive()) Smoke->Deactivate();
	if (FWreckLook* Look = WreckLooks().Find(TWeakObjectPtr<AFTWreck>(this))) TickWreckLook(this, *Look, Dt);   // FX round 3: its pieces and its sparks
}

// ------------------------------------------------------------------------------------------ hazard
AFTHazard::AFTHazard()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HazardRoot"));
	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	Plane->SetupAttachment(RootComponent);
	Plane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Plane->SetCastShadow(false);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane.Plane"));
	Plane->SetStaticMesh(PlaneMesh.Object);
	RightPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightPlane"));
	RightPlane->SetupAttachment(RootComponent);
	RightPlane->SetStaticMesh(PlaneMesh.Object);
	RightPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightPlane->SetCastShadow(false);
	RightPlane->SetVisibility(false);
	RightPlane->bAffectDistanceFieldLighting = false;
	RightPlane->bAffectDynamicIndirectLighting = false;
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetupAttachment(RootComponent);
	Light->SetCastShadows(false);
	Light->SetIntensityUnits(ELightUnits::Candelas);
	Light->SetAttenuationRadius(1600.f);
	Light->SetRelativeLocation(FVector(0, 0, 150.f));
	Thing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Thing"));
	Thing->SetupAttachment(RootComponent);
	Thing->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Thing->SetUsingAbsoluteLocation(true); Thing->SetUsingAbsoluteRotation(true); Thing->SetUsingAbsoluteScale(true);
	Thing->SetVisibility(false);
	Thing->bAffectDistanceFieldLighting = false;   // 2.8 perf
	Thing->bAffectDynamicIndirectLighting = false;
	Plane->bAffectDistanceFieldLighting = false;
	Plane->bAffectDynamicIndirectLighting = false;
	// 3.1: the trail of whatever is flying in; it is placed in the world each frame (SetThing)
	ThingTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ThingTrail"));
	ThingTrail->SetupAttachment(RootComponent);
	ThingTrail->SetUsingAbsoluteLocation(true); ThingTrail->SetUsingAbsoluteRotation(true); ThingTrail->SetUsingAbsoluteScale(true);
	ThingTrail->SetAutoActivate(false);
	// FX overhaul: a road marker draws after every effect, whatever is nearer the camera. The overhaul's smoke,
	// fire and scorch sort below zero (FTFx.cpp); this puts the marker above anything left at zero as well. A
	// warning is never under smoke
	Plane->SetTranslucentSortPriority(20);
	RightPlane->SetTranslucentSortPriority(20);
}

void AFTHazard::SetThing(UStaticMesh* M, const FTransform& WorldT)
{
	if (!M) { HideThing(); return; }
	const bool bNew = Thing->GetStaticMesh() != M || !Thing->IsVisible();
	if (Thing->GetStaticMesh() != M) Thing->SetStaticMesh(M);
	Thing->SetWorldTransform(WorldT);
	Thing->SetVisibility(true);
	// 3.1: its trail. The mark rides with the car, so the thing's flight as the player sees it is its movement
	// relative to the mark: the emitter (which throws along its own -X) is turned to face along that
	const FVector Rel = WorldT.GetLocation() - GetActorLocation();
	if (bNew)
	{
		const FString N = M->GetName();
		const bool bMain = N.Contains(TEXT("Fireball")) || N.Contains(TEXT("Missile"));
		// FX overhaul: a rocket coming down on its mark keeps the line of smoke it has had since 3.1, as a system of its own
		// (NS_RocketTrail). NS_MissileTrail, which drew it, is now the short hot core at the tail of the KESTREL's
		// missiles; those lay their smoke in the world (FTFx::Trail), which a thing whose mark rides with the car cannot do
		const TCHAR* TrailName = N.Contains(TEXT("Bomb")) ? TEXT("NS_FuseTrail") : (N.Contains(TEXT("Snow")) ? TEXT("NS_SnowTrail") : (N.Contains(TEXT("Fireball")) ? TEXT("NS_FireballTrail")
			: (N.Contains(TEXT("Fish")) ? TEXT("NS_FishTrail") : (N.Contains(TEXT("Missile")) ? TEXT("NS_RocketTrail") : nullptr))));
		ThingTrail->SetAsset(TrailName && FTFx::Shows(this, bMain ? FTFx::Main : FTFx::Extra) ? FTFx::System(TrailName) : nullptr);
		if (ThingTrail->GetAsset()) ThingTrail->Activate(true); else ThingTrail->Deactivate();
		LastThingAt = Rel;
	}
	ThingTrail->SetWorldLocation(WorldT.GetLocation());
	const FVector Move = Rel - LastThingAt;
	if (Move.SizeSquared() > 1.f) ThingTrail->SetWorldRotation(Move.Rotation());
	LastThingAt = Rel;
}

void AFTHazard::HideThing()
{
	Thing->SetVisibility(false);
	if (ThingTrail) ThingTrail->Deactivate();
}

void AFTHazard::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	if (bNewHidden && Thing) Thing->SetVisibility(false);
	if (bNewHidden && ThingTrail) ThingTrail->Deactivate();
}

void AFTHazard::SetStyle(int32 Style, int32 Type, float Progress, bool bLive)
{
	// the marker's material (ft_mats.py, M_Hazard) draws in real centimetres: tell it how big each plane is now
	const FVector S = GetActorScale3D() * 100.f;
	for (UStaticMeshComponent* Marker : { Plane.Get(), RightPlane.Get() })
	{
		if (!Marker->IsVisible()) continue;
		Marker->SetScalarParameterValueOnMaterials(TEXT("Style"), (float)FMath::Clamp(Style, 0, 3));
		Marker->SetScalarParameterValueOnMaterials(TEXT("Progress"), FMath::Clamp(Progress, 0.f, 1.f));
		Marker->SetScalarParameterValueOnMaterials(TEXT("Live"), bLive ? 1.f : 0.f);
		Marker->SetScalarParameterValueOnMaterials(TEXT("Mark"), (Type == 1 || Type == 5) ? 1.f : 0.f);
		const FVector R = Marker->GetRelativeScale3D();
		Marker->SetVectorParameterValueOnMaterials(TEXT("Size"), FVector(S.X * R.X, S.Y * R.Y, 0.f));
	}
}

void AFTHazard::SetSafeGap(float Center, float HalfWidth)
{
	// Local Y runs across the road. Each source plane is 100 cm wide.
	const auto Span = [](UStaticMeshComponent* Marker, float Lo, float Hi)
	{
		Marker->SetVisibility(Hi - Lo > KINDA_SMALL_NUMBER);
		Marker->SetRelativeLocation(FVector(0.f, 25.f * (Lo + Hi), 0.f));
		Marker->SetRelativeScale3D(FVector(1.f, FMath::Max((Hi - Lo) * 0.5f, 0.001f), 1.f));
	};
	Span(Plane, -1.f, HalfWidth > 0.f ? FMath::Clamp(Center - HalfWidth, -1.f, 1.f) : 1.f);
	Span(RightPlane, HalfWidth > 0.f ? FMath::Clamp(Center + HalfWidth, -1.f, 1.f) : 1.f, 1.f);
}

void AFTHazard::SetLook(const FLinearColor& Colour, float Intensity)
{
	for (UStaticMeshComponent* Marker : { Plane.Get(), RightPlane.Get() })
	{
		if (!Marker->IsVisible()) continue;
		// 3.1: by its base material. Setting a parameter swaps the slot to a dynamic instance, whose own name never
		// said "Hazard": the marker material was reloaded, and a new instance made, on every frame of every hazard
		UMaterialInterface* M0 = Marker->GetMaterial(0);
		if (M0 == nullptr || !M0->GetBaseMaterial() || !M0->GetBaseMaterial()->GetName().Contains(TEXT("Hazard")))
		{
			if (UMaterialInterface* M = Load<UMaterialInterface>(TEXT("/Game/FX/MI_Hazard"))) Marker->SetMaterial(0, M);
		}
		Marker->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(Colour.R, Colour.G, Colour.B));
		Marker->SetScalarParameterValueOnMaterials(TEXT("Intensity"), Intensity);
	}
	Light->SetLightColor(Colour);
	Light->SetIntensity(Intensity * 230.f);
}

void AFTEntityActor::SetTracer(const FLinearColor& Colour, float Width, bool bSpecial)
{
	// FX overhaul: every round of the KESTREL's is a longer streak (the mesh is stretched along its flight: looks
	// only, the round's box is the game mode's). Setup puts a pooled actor's scale back
	Mesh->SetRelativeScale3D(FVector(bSpecial ? 1.5f : 1.35f, Width, Width));
	if (!bSpecial) return;
	const FLinearColor Lin = Colour;
	Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(Lin.R, Lin.G, Lin.B));
	Mesh->SetVectorParameterValueOnMaterials(TEXT("Tint"), FVector(Lin.R, Lin.G, Lin.B));
	bTracerColoured = true;
	Light->SetLightColor(Colour);
	Light->SetIntensity(260.f); Light->SetAttenuationRadius(600.f);
	Light->SetRelativeLocation(FVector::ZeroVector);
	Light->SetVisibility(true);
}
