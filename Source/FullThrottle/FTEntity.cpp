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
		P = Level == 0 ? TEXT("/Game/Characters/Bosses/SM_Juggernaut") : (Level == 1 ? TEXT("/Game/Characters/Bosses/SM_Tanker") : TEXT("/Game/Characters/Bosses/SM_Carrier"));
		break;
	case EFTKind::Bullet: P = TEXT("/Game/FX/SM_Tracer"); break;
	case EFTKind::Missile: P = TEXT("/Game/Characters/Kestrel/SM_Missile"); break;
	default: return nullptr;
	}
	if (UStaticMesh** F = Cache.Find(P)) return *F;
	UStaticMesh* M = Load<UStaticMesh>(P);
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
	Extra = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Extra"));
	Extra->SetupAttachment(Mesh);
	Extra->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	Kind = InKind; Variant = InVariant;
	Mesh->SetStaticMesh(MeshFor(Kind, Variant, Level));
	Mesh->SetRelativeTransform(FTransform::Identity);
	Mesh->SetCastShadow(Kind != EFTKind::Bullet && Kind != EFTKind::Missile);
	Extra->SetVisibility(false);
	Trail->Deactivate();
	Light->SetVisibility(false);
	SetActorHiddenInGame(false);
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
		Light->SetLightColor(Level == 1 ? FLinearColor(0.4f, 1.f, 0.6f) : FLinearColor(1.f, 0.15f, 0.1f));
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
}

// ------------------------------------------------------------------------------------------ wreck
AFTWreck::AFTWreck()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	Mesh->BodyInstance.bUseCCD = true;
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
