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

namespace
{
	TAutoConsoleVariable<int32> CVarPickupMotion(TEXT("ft.PickupMotion"), 1, TEXT("Pickup visual bob and rocking; 0 for comparison."));
	TAutoConsoleVariable<int32> CVarPickupBurst(TEXT("ft.PickupBurst"), 1, TEXT("Pickup collection chips; always disabled at Low effects quality."));
	TAutoConsoleVariable<float> CVarPickupLight(TEXT("ft.PickupLight"), 2.f, TEXT("Existing unshadowed pickup light intensity in candelas; 0 disables it. Off at Low effects quality."));
	float PickupLightIntensity()
	{
		const IConsoleVariable* Quality = IConsoleManager::Get().FindConsoleVariable(TEXT("sg.EffectsQuality"));
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
	const IConsoleVariable* Quality = IConsoleManager::Get().FindConsoleVariable(TEXT("sg.EffectsQuality"));
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
	}
}

void AFTEntityActor::SetGlow(float Amount)
{
	// 3.3 perf: the game mode asks every frame for rammers, hogs and bosses; only touch the materials on a change
	// (each call walks every slot and pushes new parameter data to the render thread)
	if (FMath::IsNearlyEqual(Amount, LastGlow, 0.01f)) return;
	Mesh->SetScalarParameterValueOnMaterials(TEXT("Alert"), Amount);
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

void AFTEntityActor::SetTesla(float InCharge, const TArray<FVector>& ArcTo)
{
	if (Kind != EFTKind::Boss || Lvl != 1 || Bits.Num() < 4) return;
	TeslaCharge = FMath::Clamp(InCharge, 0.f, 1.f);
	UStaticMeshComponent* Top = Bits[0];
	if (Top && Top->GetStaticMesh())
	{
		// the top glows up as it charges, and crackles (flicker) near full
		const float Flick = TeslaCharge > 0.7f ? FMath::FRandRange(0.7f, 1.3f) : 1.f;
		Top->SetScalarParameterValueOnMaterials(TEXT("Glow"), (0.25f + 3.2f * TeslaCharge * TeslaCharge) * Flick);
	}
	Light->SetLightColor(FLinearColor(0.45f, 0.8f, 1.f));
	Light->SetRelativeLocation(TeslaTop);
	Light->SetIntensity(400.f + 5200.f * TeslaCharge * TeslaCharge + (ArcTo.Num() ? 9000.f : 0.f));
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
	if (Kind == EFTKind::Boss && Lvl == 4) for (int32 p = 1; p <= NumClownProps && p < Bits.Num(); ++p) if (Bits[p]) Bits[p]->AddLocalRotation(FRotator(0.f, (p % 2 ? 1.f : -1.f) * 2200.f * Dt, 0.f));
	if (Kind == EFTKind::Boss && Lvl == 1 && Bits.Num() > 0 && Bits[0]) Bits[0]->AddLocalRotation(FRotator(0.f, (25.f + 400.f * TeslaCharge) * Dt, 0.f));
	AnimT += Dt;
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
		auto Spring = [Dt](float& X, float& V, float T, float W, float Z)
		{
			V += (W * W * (T - X) - 2.f * Z * W * V) * Dt; X += V * Dt;
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
		if (Lvl == 5)
		{
			// 2.8: a big loping bound with a sway, both arms up over his head and flailing; faster and leaning in
			// when he charges at you
			YetiPhase += Dt * (1.f + 0.7f * Charge);
			const float A = YetiPhase;
			Mesh->SetRelativeTransform(FTransform(FRotator(-5.f * FMath::Sin(A * 6.f) - 9.f * Charge, 0.f, 5.f * FMath::Sin(A * 3.f)), FVector(0, 0, 90.f * FMath::Abs(FMath::Sin(A * 3.f)))));
			if (Extra->IsVisible())
				Extra->SetRelativeRotation(FRotator(14.f * FMath::Sin(A * 6.f + 0.8f) + 12.f * Charge, 0.f, 16.f * FMath::Sin(A * 3.f + 0.4f)));
		}
		else if (Lvl == 4) Mesh->SetRelativeTransform(FTransform(FRotator(0.f, 0.f, 7.f * FMath::Sin(AnimT * 4.f)), FVector(0, 0, 35.f * FMath::Abs(FMath::Sin(AnimT * 8.f)))));
		else if (Lvl == 7) Mesh->SetRelativeTransform(FTransform(FRotator(2.f * FMath::Sin(AnimT * 1.3f), 0.f, 3.f * FMath::Sin(AnimT * 1.7f)), FVector(0, 0, 40.f * FMath::Sin(AnimT * 2.f))));
		else if (Lvl == 6) Mesh->SetRelativeTransform(FTransform(FRotator(0.f, 3.f * FMath::Sin(AnimT * 5.f), 0.f), FVector::ZeroVector));
	}
	// fish wiggle on their way in
	if (Kind == EFTKind::Bullet && Variant == 1) Mesh->SetRelativeRotation(FRotator(0.f, 18.f * FMath::Sin(AnimT * 18.f), 0.f));
	// 3.1: and a pie spins face first
	if (Kind == EFTKind::Bullet && Variant == 4) Mesh->SetRelativeRotation(FRotator(0.f, 0.f, 540.f * AnimT));
}

// ------------------------------------------------------------------------------------------ wreck
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
	if (const TArray<AFTEntityActor::FWheelDef>* WL = AFTEntityActor::WheelsFor(InMesh))
	{
		UStaticMesh* WM = Kept(TEXT("/Game/Characters/Wheels/SM_FoeWheel"));
		const int32 Lose = FMath::RandRange(0, WL->Num() - 1);
		for (int32 i = 0; i < WL->Num() && WM; ++i)
		{
			if (i == Lose && WL->Num() > 2) continue;
			const AFTEntityActor::FWheelDef& D = (*WL)[i];
			UStaticMeshComponent* W = MakeBit(this, Mesh);
			W->SetStaticMesh(WM);
			W->SetRelativeLocationAndRotation(D.C, FRotator(FMath::FRandRange(0.f, 360.f), 0.f, 0.f));
			W->SetRelativeScale3D(FVector(D.R / 50.f, (D.C.Y > 0.f ? -1.f : 1.f) * D.W / 40.f, D.R / 50.f));
			W->SetScalarParameterValueOnMaterials(TEXT("Soot"), 0.9f);
			Wheels.Add(W);
		}
	}
	Fire->SetAsset(FTFx::System(TEXT("NS_Fire")));
	if (Fire->GetAsset()) { Fire->SetVariableFloat(TEXT("User.Size"), 1.4f); Fire->Activate(true); }
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
		const TCHAR* TrailName = N.Contains(TEXT("Bomb")) ? TEXT("NS_FuseTrail") : (N.Contains(TEXT("Snow")) ? TEXT("NS_SnowTrail") : (N.Contains(TEXT("Fireball")) ? TEXT("NS_FireballTrail")
			: (N.Contains(TEXT("Fish")) ? TEXT("NS_FishTrail") : (N.Contains(TEXT("Missile")) ? TEXT("NS_MissileTrail") : nullptr))));
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
	if (!bSpecial) return;
	const FLinearColor Lin = Colour;
	Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(Lin.R, Lin.G, Lin.B));
	Mesh->SetVectorParameterValueOnMaterials(TEXT("Tint"), FVector(Lin.R, Lin.G, Lin.B));
	bTracerColoured = true;
	Mesh->SetRelativeScale3D(FVector(1.f, Width, Width));
	Light->SetLightColor(Colour);
	Light->SetIntensity(260.f); Light->SetAttenuationRadius(600.f);
	Light->SetRelativeLocation(FVector::ZeroVector);
	Light->SetVisibility(true);
}
