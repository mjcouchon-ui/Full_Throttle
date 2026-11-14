#include "FTEntity.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Engine/StaticMesh.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	template <typename T> T* Load(const FString& Path)
	{
		return Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *Path, nullptr, LOAD_NoWarn | LOAD_Quiet));
	}
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
}

void AFTEntityActor::Setup(EFTKind InKind, int32 InVariant, int32 Level)
{
	Kind = InKind; Variant = InVariant; Lvl = Level; AnimT = 0.f;
	if (bTracerColoured) { Mesh->EmptyOverrideMaterials(); bTracerColoured = false; }
	Mesh->SetStaticMesh(MeshFor(Kind, Variant, Level));
	Mesh->SetRelativeTransform(FTransform::Identity);
	Mesh->SetCastShadow(Kind != EFTKind::Bullet && Kind != EFTKind::Missile);
	Extra->SetVisibility(false);
	Extra->SetRelativeTransform(FTransform::Identity);
	Charge = 0.f; YetiPhase = 0.f;
	Trail->Deactivate();
	Light->SetVisibility(false);
	SetActorHiddenInGame(false);
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
	if (Kind == EFTKind::Heli)
	{
		Extra->SetStaticMesh(Load<UStaticMesh>(TEXT("/Game/Characters/Choppers/SM_HeliRotor")));
		Extra->SetRelativeLocation(FVector(40.f, 0.f, 330.f));
		Extra->SetVisibility(true);
		Light->SetRelativeLocation(FVector(260.f, 0.f, 60.f));
		Light->SetLightColor(FLinearColor(1.f, 0.95f, 0.85f));
		Light->SetIntensity(900.f); Light->SetAttenuationRadius(2600.f);
		Light->SetVisibility(true);
	}
	else if (Kind == EFTKind::Henchman)
	{
		Extra->SetStaticMesh(Load<UStaticMesh>(TEXT("/Engine/BasicShapes/Cylinder.Cylinder")));
		Extra->SetRelativeLocation(FVector(0, 0, 800.f));
		Extra->SetRelativeScale3D(FVector(0.03f, 0.03f, 14.f));
		Extra->SetVisibility(true);
	}
	else if (Kind == EFTKind::Missile)
	{
		Trail->SetAsset(Load<UNiagaraSystem>(TEXT("/Game/FX/NS_MissileTrail")));
		if (Trail->GetAsset()) Trail->Activate(true);
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
	else if (Kind == EFTKind::Crate || Kind == EFTKind::Wrench)
	{
		Light->SetLightColor(Kind == EFTKind::Crate ? FLinearColor(1.f, 0.55f, 0.15f) : FLinearColor(0.3f, 1.f, 0.5f));
		Light->SetIntensity(250.f); Light->SetAttenuationRadius(700.f);
		Light->SetRelativeLocation(FVector(0, 0, 120.f));
		Light->SetVisibility(true);
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
	Mesh->SetScalarParameterValueOnMaterials(TEXT("Alert"), Amount);
}

void AFTEntityActor::Animate(float Dt, float Speed)
{
	if (Kind == EFTKind::Heli && Extra) Extra->AddLocalRotation(FRotator(0.f, 1900.f * Dt, 0.f));
	AnimT += Dt;
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
	Fire->SetAsset(Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), nullptr, TEXT("/Game/FX/NS_Fire"), nullptr, LOAD_NoWarn | LOAD_Quiet)));
	if (Fire->GetAsset()) { Fire->SetVariableFloat(TEXT("User.Size"), 1.4f); Fire->Activate(true); }
}

void AFTWreck::Tick(float Dt)
{
	Super::Tick(Dt);
	Age += Dt;
	if (Age > 6.f && Fire->IsActive()) Fire->Deactivate();
}

// ------------------------------------------------------------------------------------------ hazard
AFTHazard::AFTHazard()
{
	PrimaryActorTick.bCanEverTick = false;
	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	RootComponent = Plane;
	Plane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Plane->SetCastShadow(false);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane.Plane"));
	Plane->SetStaticMesh(PlaneMesh.Object);
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetupAttachment(Plane);
	Light->SetCastShadows(false);
	Light->SetIntensityUnits(ELightUnits::Candelas);
	Light->SetAttenuationRadius(1600.f);
	Light->SetRelativeLocation(FVector(0, 0, 150.f));
	Thing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Thing"));
	Thing->SetupAttachment(Plane);
	Thing->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Thing->SetUsingAbsoluteLocation(true); Thing->SetUsingAbsoluteRotation(true); Thing->SetUsingAbsoluteScale(true);
	Thing->SetVisibility(false);
	Thing->bAffectDistanceFieldLighting = false;   // 2.8 perf
	Thing->bAffectDynamicIndirectLighting = false;
	Plane->bAffectDistanceFieldLighting = false;
	Plane->bAffectDynamicIndirectLighting = false;
}

void AFTHazard::SetThing(UStaticMesh* M, const FTransform& WorldT)
{
	if (!M) { HideThing(); return; }
	if (Thing->GetStaticMesh() != M) Thing->SetStaticMesh(M);
	Thing->SetWorldTransform(WorldT);
	Thing->SetVisibility(true);
}

void AFTHazard::HideThing() { Thing->SetVisibility(false); }

void AFTHazard::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	if (bNewHidden && Thing) Thing->SetVisibility(false);
}

void AFTHazard::SetLook(const FLinearColor& Colour, float Intensity)
{
	if (Plane->GetMaterial(0) == nullptr || !Plane->GetMaterial(0)->GetName().Contains(TEXT("Hazard")))
	{
		if (UMaterialInterface* M = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/FX/MI_Hazard"), nullptr, LOAD_NoWarn | LOAD_Quiet))) Plane->SetMaterial(0, M);
	}
	Plane->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(Colour.R, Colour.G, Colour.B));
	Plane->SetScalarParameterValueOnMaterials(TEXT("Intensity"), Intensity);
	Light->SetLightColor(Colour);
	Light->SetIntensity(Intensity * 300.f);
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
