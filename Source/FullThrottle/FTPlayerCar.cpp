#include "FTPlayerCar.h"
#include "FTTrack.h"
#include "FTCore.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/StaticMesh.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "HAL/IConsoleManager.h"

// which arcade assists run on top of Chaos (bits): 1 speed governor, 2 grip, 4 yaw, 8 anti-flip, 16 track gravity, 32 air control,
// 64 road glue (2.4 downforce)
static TAutoConsoleVariable<int32> CVarFTArcade(TEXT("ft.Arcade"), 127, TEXT("FULL THROTTLE: arcade assist bitmask on the Chaos car"));

namespace
{
	template <typename T> T* Load(const FString& Path)
	{
		return Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *Path, nullptr, LOAD_NoWarn | LOAD_Quiet));
	}
	const TCHAR* KDir = TEXT("/Game/Characters/Kestrel/");
	// Wheel hubs on the KESTREL model (origin mid-car on the ground, X forward, Y right)
	const FVector WheelPos[4] = { FVector(147, -99, 31), FVector(147, 99, 31), FVector(-133, -112, 40), FVector(-133, 112, 40) };
	// the chassis box's centre sits this high above the model's ground origin
	constexpr float ChassisZ = 88.f; // box from 56 to 120 cm: clear of the wheels' suspension traces
	// Parts in the order they come off (1.x: below 88, 76, 64, 52, 40, 28 and 16 armour)
	const TCHAR* PartNames[7] = { TEXT("MirrorR"), TEXT("Antenna"), TEXT("PodL"), TEXT("LightBar"), TEXT("Wing"), TEXT("PodR"), TEXT("Blower") };
}

UFTWheelRear::UFTWheelRear()
{
	AxleType = EAxleType::Rear;
	WheelRadius = 40.f; WheelWidth = 56.f; WheelMass = 30.f;
	CorneringStiffness = 1200.f; FrictionForceMultiplier = 3.2f; SideSlipModifier = 1.f;
	SlipThreshold = 30.f; SkidThreshold = 30.f;
	MaxSteerAngle = 0.f; bAffectedBySteering = false; bAffectedByEngine = true; bAffectedByBrake = true; bAffectedByHandbrake = true;
	bTractionControlEnabled = true; bABSEnabled = true;
	// suspension sized for 1450 kg: ~3.6 kN per wheel sits mid-travel
	SuspensionMaxRaise = 14.f; SuspensionMaxDrop = 14.f; SuspensionDampingRatio = 0.5f; SpringRate = 700.f; SpringPreload = 400.f; RollbarScaling = 0.4f;
	MaxBrakeTorque = 3500.f; MaxHandBrakeTorque = 4000.f;
}

UFTWheelFront::UFTWheelFront()
{
	AxleType = EAxleType::Front;
	WheelRadius = 31.f; WheelWidth = 26.f; WheelMass = 22.f;
	CorneringStiffness = 1200.f; FrictionForceMultiplier = 3.0f; SideSlipModifier = 1.f;
	SlipThreshold = 30.f; SkidThreshold = 30.f;
	MaxSteerAngle = 32.f; bAffectedBySteering = true; bAffectedByEngine = false; bAffectedByBrake = true; bAffectedByHandbrake = false;
	bABSEnabled = true;
	SuspensionMaxRaise = 14.f; SuspensionMaxDrop = 14.f; SuspensionDampingRatio = 0.5f; SpringRate = 650.f; SpringPreload = 350.f; RollbarScaling = 0.4f;
	MaxBrakeTorque = 3000.f;
}

AFTPlayerCar::AFTPlayerCar()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	Chassis = CreateDefaultSubobject<UBoxComponent>(TEXT("Chassis"));
	RootComponent = Chassis;
	Chassis->SetBoxExtent(FVector(220.f, 100.f, 32.f));
	Chassis->SetSimulatePhysics(true);
	Chassis->SetCollisionProfileName(TEXT("Vehicle"));
	Chassis->BodyInstance.bUseCCD = true;
	Chassis->SetNotifyRigidBodyCollision(true);
	Chassis->SetLinearDamping(0.05f);
	Chassis->SetAngularDamping(0.6f);
	Chassis->SetHiddenInGame(true);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Chassis);
	Body->SetRelativeLocation(FVector(0.f, 0.f, -ChassisZ));
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Vehicle = CreateDefaultSubobject<UChaosWheeledVehicleMovementComponent>(TEXT("Vehicle"));
	Vehicle->SetUpdatedComponent(Chassis);
	Vehicle->Mass = 1450.f;
	Vehicle->bEnableCenterOfMassOverride = false;
	Vehicle->ChassisWidth = 220.f; Vehicle->ChassisHeight = 110.f;
	Vehicle->DragCoefficient = 0.28f; Vehicle->DownforceCoefficient = 0.9f;
	Vehicle->SetRequiresControllerForInputs(false);
	Vehicle->WheelSetups.SetNum(4);
	for (int32 i = 0; i < 4; ++i)
	{
		Vehicle->WheelSetups[i].WheelClass = i < 2 ? UFTWheelFront::StaticClass() : UFTWheelRear::StaticClass();
		// Chaos insists on a bone name; with a static-mesh body there is no skeleton, so it's only a label
		// and the wheel sits at AdditionalOffset
		static const FName Bones[4] = { TEXT("Wheel_FL"), TEXT("Wheel_FR"), TEXT("Wheel_RL"), TEXT("Wheel_RR") };
		Vehicle->WheelSetups[i].BoneName = Bones[i];
		Vehicle->WheelSetups[i].AdditionalOffset = WheelPos[i] - FVector(0.f, 0.f, ChassisZ);
	}
	FVehicleEngineConfig& E = Vehicle->EngineSetup;
	E.MaxTorque = 820.f; E.MaxRPM = 7800.f; E.EngineIdleRPM = 900.f; E.EngineBrakeEffect = 0.02f; E.EngineRevUpMOI = 3.f; E.EngineRevDownRate = 900.f;
	E.TorqueCurve.GetRichCurve()->Reset();
	E.TorqueCurve.GetRichCurve()->AddKey(0.f, 0.65f);
	E.TorqueCurve.GetRichCurve()->AddKey(3000.f, 0.92f);
	E.TorqueCurve.GetRichCurve()->AddKey(5600.f, 1.f);
	E.TorqueCurve.GetRichCurve()->AddKey(7800.f, 0.82f);
	Vehicle->DifferentialSetup.DifferentialType = EVehicleDifferential::RearWheelDrive;
	FVehicleTransmissionConfig& T = Vehicle->TransmissionSetup;
	T.bUseAutomaticGears = true; T.bUseAutoReverse = true; T.FinalRatio = 3.1f;
	T.ForwardGearRatios = { 3.6f, 2.4f, 1.75f, 1.35f, 1.08f, 0.88f };
	T.ReverseGearRatios = { 3.2f };
	T.ChangeUpRPM = 7000.f; T.ChangeDownRPM = 3600.f; T.GearChangeTime = 0.18f; T.TransmissionEfficiency = 0.92f;
	Vehicle->SteeringSetup.SteeringType = ESteeringType::AngleRatio;
	Vehicle->SteeringSetup.AngleRatio = 0.8f;
	FRichCurve* SC = Vehicle->SteeringSetup.SteeringCurve.GetRichCurve();
	SC->Reset(); SC->AddKey(0.f, 1.f); SC->AddKey(60.f, 0.7f); SC->AddKey(120.f, 0.42f); SC->AddKey(220.f, 0.28f);

	for (int32 i = 0; i < 4; ++i)
	{
		UStaticMeshComponent* W = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("Wheel%d"), i));
		W->SetupAttachment(Body);
		W->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		W->SetRelativeLocation(WheelPos[i]);
		if (WheelPos[i].Y > 0.f) W->SetRelativeScale3D(FVector(1.f, -1.f, 1.f));
		WheelMeshes.Add(W);
	}
	for (int32 i = 0; i < 7; ++i)
	{
		UStaticMeshComponent* P = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("Part_%s"), PartNames[i]));
		P->SetupAttachment(Body);
		P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Parts.Add(P);
	}
	for (int32 i = 0; i < 2; ++i)
	{
		UStaticMeshComponent* Hm = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("Henchman%d"), i));
		Hm->SetupAttachment(Body);
		Hm->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Hm->SetRelativeLocation(FVector(10.f, i == 0 ? -118.f : 118.f, 70.f));
		Hm->SetRelativeRotation(FRotator(0.f, i == 0 ? 90.f : -90.f, 0.f));
		Hm->SetVisibility(false);
		Henchmen.Add(Hm);
	}

	Underglow = CreateDefaultSubobject<UPointLightComponent>(TEXT("Underglow"));
	Underglow->SetupAttachment(Body);
	Underglow->SetRelativeLocation(FVector(0, 0, 14));
	Underglow->SetIntensityUnits(ELightUnits::Candelas);
	Underglow->SetIntensity(60.f);
	Underglow->SetAttenuationRadius(520.f);
	Underglow->SetLightColor(FLinearColor(1.f, 0.31f, 0.72f));
	Underglow->SetCastShadows(false);

	auto Head = [this](const TCHAR* Name, float Y) {
		USpotLightComponent* S = CreateDefaultSubobject<USpotLightComponent>(Name);
		S->SetupAttachment(Body);
		S->SetRelativeLocation(FVector(228.f, Y, 58.f));
		S->SetRelativeRotation(FRotator(-4.f, 0.f, 0.f));
		S->SetIntensityUnits(ELightUnits::Candelas);
		S->SetIntensity(2600.f);
		S->SetAttenuationRadius(5200.f);
		S->SetInnerConeAngle(12.f); S->SetOuterConeAngle(26.f);
		S->SetLightColor(FLinearColor(1.f, 0.93f, 0.8f));
		S->SetCastShadows(false);
		return S;
	};
	HeadL = Head(TEXT("HeadL"), -72.f);
	HeadR = Head(TEXT("HeadR"), 72.f);

	auto FX = [this](const TCHAR* Name, const FVector& At) {
		UNiagaraComponent* N = CreateDefaultSubobject<UNiagaraComponent>(Name);
		N->SetupAttachment(Body);
		N->SetRelativeLocation(At);
		N->SetAutoActivate(false);
		return N;
	};
	SmokeFX = FX(TEXT("SmokeFX"), FVector(-110, 0, 120));
	FireFX = FX(TEXT("FireFX"), FVector(-115, 0, 82)); // 2.4: on the engine deck, not floating over the roof
	SparksFX = FX(TEXT("SparksFX"), FVector(-210, 0, 8));
	TurboFX = FX(TEXT("TurboFX"), FVector(-232, 0, 30));
	DriftFX = FX(TEXT("DriftFX"), FVector(-133, 0, 10));

	EngineAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineAudio"));
	EngineAudio->SetupAttachment(Body);
	EngineAudio->bAutoActivate = false;
	SkidAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("SkidAudio"));
	SkidAudio->SetupAttachment(Body);
	SkidAudio->bAutoActivate = false;

	AutoPossessPlayer = EAutoReceiveInput::Disabled;
}

void AFTPlayerCar::BeginPlay()
{
	// meshes and effects come from fixed asset paths (made in the editor from the 1.x models)
	if (!Body->GetStaticMesh()) Body->SetStaticMesh(Load<UStaticMesh>(FString(KDir) + TEXT("SM_Kestrel_Body")));
	UStaticMesh* WF = Load<UStaticMesh>(FString(KDir) + TEXT("SM_Kestrel_WheelFront"));
	UStaticMesh* WR = Load<UStaticMesh>(FString(KDir) + TEXT("SM_Kestrel_WheelRear"));
	for (int32 i = 0; i < 4; ++i) if (!WheelMeshes[i]->GetStaticMesh()) WheelMeshes[i]->SetStaticMesh(i < 2 ? WF : WR);
	for (int32 i = 0; i < Parts.Num(); ++i)
	{
		if (!Parts[i]->GetStaticMesh()) Parts[i]->SetStaticMesh(Load<UStaticMesh>(FString(KDir) + TEXT("SM_Kestrel_") + PartNames[i]));
		PartRest.Add(Parts[i]->GetRelativeTransform());
	}
	UStaticMesh* HM = Load<UStaticMesh>(TEXT("/Game/Characters/Syndicate/SM_Henchman"));
	for (UStaticMeshComponent* Hm : Henchmen) if (!Hm->GetStaticMesh()) Hm->SetStaticMesh(HM);
	auto SetFX = [](UNiagaraComponent* C, const TCHAR* P) { if (!C->GetAsset()) C->SetAsset(Load<UNiagaraSystem>(P)); };
	SetFX(SmokeFX, TEXT("/Game/FX/NS_Smoke"));
	// 2.4: the car burns with its own flat cel fire (NS_CarFire, MI_Fire_Car); NS_Fire's white-hot puffs hid it
	SetFX(FireFX, TEXT("/Game/FX/NS_CarFire"));
	SetFX(SparksFX, TEXT("/Game/FX/NS_Sparks"));
	SetFX(TurboFX, TEXT("/Game/FX/NS_TurboFlame"));
	SetFX(DriftFX, TEXT("/Game/FX/NS_TyreSmoke"));
	if (!EngineAudio->Sound) EngineAudio->SetSound(Load<USoundBase>(TEXT("/Game/Audio/SFX/S_EngineLoop")));
	if (!SkidAudio->Sound) SkidAudio->SetSound(Load<USoundBase>(TEXT("/Game/Audio/SFX/S_Skid")));
	if (UPhysicalMaterial* PM = Load<UPhysicalMaterial>(TEXT("/Game/Core/Physics/PM_CarBody"))) Chassis->SetPhysMaterialOverride(PM);
	// the suspension traces must never find the car's own chassis (a Vehicle-channel body)
	Vehicle->SetWheelTraceResponseToChannel(ECC_Vehicle, ECR_Ignore);
	Vehicle->SetWheelTraceResponseToChannel(ECC_Pawn, ECR_Ignore);
	Chassis->OnComponentHit.AddDynamic(this, &AFTPlayerCar::OnBodyHit);
	Super::BeginPlay();
	if (EngineAudio->Sound) EngineAudio->Play();
	if (SkidAudio->Sound) { SkidAudio->SetVolumeMultiplier(0.f); SkidAudio->Play(); }
}

void AFTPlayerCar::SetLightsForLevel(int32 Level)
{
	Underglow->SetLightColor(FTData::Level(Level).Glow);
}

bool AFTPlayerCar::CanRailHop(int32 Dir) const
{
	if (!bRailInit || !Track || HopCd > 0.f || HopT >= 0.f || Dir == 0) return false;
	const FFTRails R = Track->RailsAt(D);
	if (!R.bAdjacent) return false;
	// hop toward the other rail only
	const bool bOnLeft = FVector2D::DistSquared(RailPt, FVector2D(R.XL, R.HL)) < FVector2D::DistSquared(RailPt, FVector2D(R.XR, R.HR));
	return bOnLeft ? Dir > 0 : Dir < 0;
}

bool AFTPlayerCar::RailHop(int32 Dir)
{
	if (!CanRailHop(Dir)) return false;
	const FFTRails R = Track->RailsAt(D);
	const bool bOnLeft = FVector2D::DistSquared(RailPt, FVector2D(R.XL, R.HL)) < FVector2D::DistSquared(RailPt, FVector2D(R.XR, R.HR));
	// up and over on a set arc that always lands on the other rail (the first 2.5 build flew it with physics and a
	// spring, and one hop in four missed and fell between the rails)
	HopFrom = FVector2D(X, SurfH);
	RailPt = bOnLeft ? FVector2D(R.XR, R.HR) : FVector2D(R.XL, R.HL);
	HopT = 0.f;
	LaunchT = HopDur + 0.1f;
	HopCd = 1.1f;
	return true;
}

FTransform AFTPlayerCar::GunMuzzle(int32 Side) const
{
	return FTransform(FVector(40.f, Side < 0 ? -34.f : 34.f, 124.f)) * Chassis->GetComponentTransform();
}

void AFTPlayerCar::PlaceOnTrack(float InD, float InX, float InSpeed)
{
	if (!Track) return;
	// 2.5: on a coaster, onto the nearest rail; in a half pipe, onto its curve
	bRailInit = false;
	float Hs = 0.f;
	if (Track->FlagsAt(InD) & FTFlag::Coaster)
	{
		const FFTRails R = Track->RailsAt(InD);
		const bool bLeft = FMath::Abs(InX - R.XL) < FMath::Abs(InX - R.XR);
		InX = bLeft ? R.XL : R.XR; Hs = bLeft ? R.HL : R.HR;
		RailPt = FVector2D(InX, Hs); bRailInit = true;
	}
	else Hs = Track->SurfaceH(InD, InX);
	const FTransform T = Track->TransformAt(InD, InX, Hs + ChassisZ + 45.f);
	Chassis->SetWorldTransform(T, false, nullptr, ETeleportType::ResetPhysics);
	Chassis->SetPhysicsLinearVelocity(T.GetRotation().GetForwardVector() * InSpeed);
	Chassis->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);
	// (no ResetVehicle: it rebuilds the physics vehicle and asserts if called mid-spawn; a teleport is enough)
	D = InD; X = InX; H = 0.f;
	LaunchT = 0.f;
	bWrecked = false;
	Chassis->SetSimulatePhysics(true);
}

void AFTPlayerCar::Shove(const FVector& WorldDir, float CmPerSec, float SpinRad)
{
	Chassis->AddImpulse(WorldDir * CmPerSec, NAME_None, true);
	// an upward kick (the car-carrier ramp) is a real jump: let it fly
	if (FVector::DotProduct(WorldDir, RoadUp) * CmPerSec > 400.f) LaunchT = 1.6f;
	if (SpinRad != 0.f) Chassis->AddAngularImpulseInRadians(RoadUp * SpinRad, NAME_None, true);
}

void AFTPlayerCar::SetDamageLook(float Hp)
{
	const bool bSmoke = Hp < 82.f && !bWrecked;
	if (SmokeFX->GetAsset())
	{
		if (bSmoke && !SmokeFX->IsActive()) SmokeFX->Activate(true);
		if (!bSmoke && SmokeFX->IsActive()) SmokeFX->Deactivate();
		SmokeFX->SetVariableFloat(TEXT("User.Rate"), Hp < 45.f ? 3.f : (Hp < 65.f ? 2.f : 1.f));
		SmokeFX->SetVariableFloat(TEXT("User.Dark"), Hp < 50.f ? 1.f : 0.f);
	}
	const bool bFire = Hp < 35.f || bWrecked;
	if (FireFX->GetAsset())
	{
		if (bFire && !FireFX->IsActive()) FireFX->Activate(true);
		if (!bFire && FireFX->IsActive()) FireFX->Deactivate();
		// the stateless emitter has no User.Size; it scales with the component (ApplyOwnerScaleToAttributes)
		FireFX->SetRelativeScale3D(FVector(Hp < 18.f ? 1.3f : 1.f));
	}
	Body->SetScalarParameterValueOnMaterials(TEXT("Soot"), FMath::Clamp((85.f - Hp) / 85.f * 0.8f, 0.f, 0.8f));
	for (UStaticMeshComponent* P : Parts) P->SetScalarParameterValueOnMaterials(TEXT("Soot"), FMath::Clamp((85.f - Hp) / 85.f * 0.8f, 0.f, 0.8f));
}

int32 AFTPlayerCar::NumPartsOn() const
{
	int32 N = 0;
	for (UStaticMeshComponent* P : Parts) if (P->GetAttachParent() == Body) ++N;
	return N;
}

void AFTPlayerCar::ShedPart(int32 Index)
{
	if (!Parts.IsValidIndex(Index)) return;
	UStaticMeshComponent* P = Parts[Index];
	if (P->GetAttachParent() != Body || !P->GetStaticMesh()) return;
	const FVector V = Chassis->GetPhysicsLinearVelocity();
	P->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	P->SetCollisionProfileName(TEXT("PhysicsActor"));
	P->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	P->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	P->SetSimulatePhysics(true);
	P->SetMassOverrideInKg(NAME_None, 25.f, true);
	P->SetPhysicsLinearVelocity(V * 0.82f + RoadUp * 420.f + Chassis->GetRightVector() * FMath::FRandRange(-500.f, 500.f));
	P->SetPhysicsAngularVelocityInDegrees(FVector(FMath::FRandRange(-720.f, 720.f), FMath::FRandRange(-720.f, 720.f), FMath::FRandRange(-720.f, 720.f)));
	LooseParts.Add(P);
}

void AFTPlayerCar::RestoreParts()
{
	for (int32 i = 0; i < Parts.Num(); ++i)
	{
		UStaticMeshComponent* P = Parts[i];
		if (P->GetAttachParent() == Body) continue;
		P->SetSimulatePhysics(false);
		P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		P->AttachToComponent(Body, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		if (PartRest.IsValidIndex(i)) P->SetRelativeTransform(PartRest[i]);
	}
	LooseParts.Reset();
}

void AFTPlayerCar::SetHenchmenVisible(bool bLeft, bool bRight)
{
	Henchmen[0]->SetVisibility(bLeft);
	Henchmen[1]->SetVisibility(bRight);
}

void AFTPlayerCar::Wreck()
{
	if (bWrecked) return;
	bWrecked = true;
	for (int32 i = 0; i < Parts.Num(); ++i) ShedPart(i);
	Chassis->AddImpulse(RoadUp * 900.f + Chassis->GetRightVector() * FMath::FRandRange(-300.f, 300.f), NAME_None, true);
	Chassis->AddAngularImpulseInRadians(FVector(FMath::FRandRange(-4.f, 4.f), FMath::FRandRange(-3.f, 3.f), FMath::FRandRange(-2.f, 2.f)), NAME_None, true);
	Vehicle->SetThrottleInput(0.f);
	Vehicle->SetBrakeInput(1.f);
	if (FireFX->GetAsset()) { FireFX->SetRelativeScale3D(FVector(1.6f)); FireFX->Activate(true); }
	if (SmokeFX->GetAsset()) { SmokeFX->SetVariableFloat(TEXT("User.Dark"), 1.f); SmokeFX->Activate(true); }
	Underglow->SetVisibility(false);
	HeadL->SetVisibility(false); HeadR->SetVisibility(false);
}

void AFTPlayerCar::OnBodyHit(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Other && Track)
	{
		// test reports: what the car last ran into (FTSTALL prints it)
		float Hx = 0.f, Hh = 0.f;
		const float Hd = Track->Locate(Hit.ImpactPoint, D, Hx, Hh);
		LastHit = FString::Printf(TEXT("%s/%s n.up=%.2f n.fwd=%.2f at D=%.0f X=%.0f H=%.0f"), *Other->GetName(), OtherComp ? *OtherComp->GetName() : TEXT("-"),
			FVector::DotProduct(Hit.ImpactNormal, RoadUp), FVector::DotProduct(Hit.ImpactNormal, Chassis->GetForwardVector()), Hd, Hx, Hh);
		LastHitT = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
	}
	if (Other && Other == Track)
	{
		const float Side = FVector::DotProduct(Hit.ImpactNormal, RoadUp);
		if (FMath::Abs(Side) < 0.5f) // a barrier, not the deck
		{
			WallTimer = 0.12f;
			WallImpact = FMath::Max(WallImpact, NormalImpulse.Size() / FMath::Max(1.f, Chassis->GetMass()));
		}
	}
}

void AFTPlayerCar::Tick(float Dt)
{
	Super::Tick(Dt);
	if (!Track || !Track->IsBuilt()) return;

	float Xl, Hl;
	D = Track->Locate(Chassis->GetComponentLocation(), D, Xl, Hl);
	X = Xl; H = Hl;
	const FFTFrame F = Track->Sample(D);
	RoadUp = F.Up;
	const FVector V = Chassis->GetPhysicsLinearVelocity();
	Speed = FVector::DotProduct(V, F.Fwd);
	LatSpeed = FVector::DotProduct(V, F.Right);

	WheelsDown = 0;
	const int32 NumW = FMath::Min(Vehicle->WheelSetups.Num(), Vehicle->Wheels.Num());
	for (int32 i = 0; i < NumW; ++i)
	{
		if (Vehicle->Wheels[i] && !Vehicle->Wheels[i]->IsInAir()) ++WheelsDown;
	}
	bGrounded = WheelsDown >= 2;
	AirTime = bGrounded ? 0.f : AirTime + Dt;

	WallTimer -= Dt;
	const bool bScrape = WallTimer > 0.f || FMath::Abs(X) > FT::Edge - 150.f;
	bWallLeft = bScrape && X < 0.f;
	bWallRight = bScrape && X > 0.f;
	if (!bScrape) WallImpact = 0.f;

	ApplyArcade(Dt);
	{
		// 2.4 glue telemetry (autopilot runs): time off the road on ordinary road vs on real jumps, and how high
		static float StrayAir = 0.f, StrayMaxH = 0.f, JumpAir = 0.f, JumpMaxH = 0.f, GlueT = 0.f;
		static IConsoleVariable* AutoG = IConsoleManager::Get().FindConsoleVariable(TEXT("ft.Autopilot"));
		if (AutoG && AutoG->GetInt() > 0 && !bWrecked && !Drive.bFrozen)
		{
			const bool bJumpNow = (F.Flags & (FTFlag::Kicker | FTFlag::Gap | FTFlag::Landing)) != 0 || LaunchT > 0.f;
			if (!bGrounded) { if (bJumpNow) { JumpAir += Dt; JumpMaxH = FMath::Max(JumpMaxH, H); } else { StrayAir += Dt; StrayMaxH = FMath::Max(StrayMaxH, H); } }
			// one line per flight longer than 0.25 s: what launched it, air time, rise over the take-off point, distance
			static bool bWasAir = false; static float AirT0 = 0.f, Z0 = 0.f, ZMax = 0.f, D0 = 0.f, LiftMax = 0.f, LiftMin = 0.f; static const TCHAR* Kind = TEXT("");
			const float Z = Chassis->GetComponentLocation().Z;
			if (!bGrounded && !bWasAir)
			{
				bWasAir = true; AirT0 = 0.f; Z0 = ZMax = Z; D0 = D; LiftMax = LiftMin = H - SurfH - ChassisZ;
				Kind = LaunchT > 0.f ? TEXT("ramp") : ((F.Flags & (FTFlag::Kicker | FTFlag::Gap)) ? TEXT("gap") : TEXT("road"));
			}
			if (bWasAir) { AirT0 += Dt; ZMax = FMath::Max(ZMax, Z); LiftMax = FMath::Max(LiftMax, H - SurfH - ChassisZ); LiftMin = FMath::Min(LiftMin, H - SurfH - ChassisZ); }
			if (bGrounded && bWasAir)
			{
				bWasAir = false;
				// lift = chassis height over the road minus ride height (negative: pressed down past the springs)
				if (AirT0 > 0.25f) UE_LOG(LogFT, Display, TEXT("FTJUMP kind=%s from D=%.0f air=%.2fs rise=%.0f dist=%.0f lift=%.0f..%.0f"), Kind, D0, AirT0, ZMax - Z0, D - D0, LiftMin, LiftMax);
			}
			GlueT += Dt;
			if (GlueT > 5.f)
			{
				UE_LOG(LogFT, Display, TEXT("FTGLUE D=%.0f straysAir=%.2fs strayMaxH=%.0f jumpAir=%.2fs jumpMaxH=%.0f v=%.0f fps=%.0f"), D, StrayAir, StrayMaxH, JumpAir, JumpMaxH, Speed, 1.f / FMath::Max(Dt, 1e-4f));
				StrayAir = StrayMaxH = JumpAir = JumpMaxH = GlueT = 0.f;
			}
		}
	}
	{
		static int32 Burst = 0;
		static IConsoleVariable* Auto = IConsoleManager::Get().FindConsoleVariable(TEXT("ft.Autopilot"));
		if (Auto && Auto->GetInt() > 0 && GFrameCounter % 10 == 0 && Burst++ < 60)
			UE_LOG(LogFT, Display, TEXT("FTSPAWN D=%.0f X=%.0f H=%.0f up=%.2f fwdz=%.2f v=%.0f w=%.2f susp=%.1f,%.1f,%.1f,%.1f inair=%d%d%d%d"), D, X, H,
				FVector::DotProduct(Chassis->GetUpVector(), RoadUp), FVector::DotProduct(Chassis->GetForwardVector(), RoadUp), Chassis->GetPhysicsLinearVelocity().Size(), Chassis->GetPhysicsAngularVelocityInRadians().Size(),
				Vehicle->Wheels.Num() > 3 ? Vehicle->Wheels[0]->GetSuspensionOffset() : -1.f, Vehicle->Wheels.Num() > 3 ? Vehicle->Wheels[1]->GetSuspensionOffset() : -1.f,
				Vehicle->Wheels.Num() > 3 ? Vehicle->Wheels[2]->GetSuspensionOffset() : -1.f, Vehicle->Wheels.Num() > 3 ? Vehicle->Wheels[3]->GetSuspensionOffset() : -1.f,
				Vehicle->Wheels.Num() > 3 ? (int32)Vehicle->Wheels[0]->IsInAir() : -1, Vehicle->Wheels.Num() > 3 ? (int32)Vehicle->Wheels[1]->IsInAir() : -1, Vehicle->Wheels.Num() > 3 ? (int32)Vehicle->Wheels[2]->IsInAir() : -1, Vehicle->Wheels.Num() > 3 ? (int32)Vehicle->Wheels[3]->IsInAir() : -1);
	}

	// capsized for a while: the crew rights the car where it is
	if (!bWrecked && !Drive.bFrozen)
	{
		const float Upright = FVector::DotProduct(Chassis->GetUpVector(), SurfN);
		// capsized, or down on its belly with the wheels below the road (they can't find it again)
		const bool bBelly = WheelsDown == 0 && H - SurfH < ChassisZ - 30.f && H - SurfH > -200.f;
		CapsizeT = (Upright < 0.35f || bBelly) ? CapsizeT + Dt : 0.f;
		if (CapsizeT > 1.5f)
		{
			CapsizeT = 0.f;
			PlaceOnTrack(D, FMath::Clamp(X, -FT::RoadHalf, FT::RoadHalf), FMath::Max(0.f, Speed) * 0.5f);
		}
		// 2.5: wedged (nose up on a banked wall, hung on a barrier) with the throttle open: after 1.2 s the crew puts
		// it back on the road a little ahead, rolling, instead of leaving the player stuck
		StallT = (Drive.TargetSpeed > 2000.f && Speed < 600.f && !(Track->FlagsAt(D) & FTFlag::Gap)) ? StallT + Dt : 0.f;
		if (StallT > 1.2f)
		{
			StallT = 0.f;
			float Dn = D + 400.f;
			while (Track->FlagsAt(Dn) & (FTFlag::Gap | FTFlag::Kicker)) Dn += 400.f;
			UE_LOG(LogFT, Display, TEXT("FTSTALL D=%.0f X=%.0f H=%.0f surf=%.0f -> D=%.0f lastHit(%.1fs ago)=%s"), D, X, H, SurfH, Dn,
				GetWorld() ? GetWorld()->GetTimeSeconds() - LastHitT : -1.f, *LastHit);
			PlaceOnTrack(Dn, FMath::Clamp(X, -FT::RoadHalf + 250.f, FT::RoadHalf - 250.f), FT::BaseSpeed * 0.6f);
		}
	}

	// wheels: spin, steer and ride the springs
	for (int32 i = 0; i < 4 && i < Vehicle->Wheels.Num(); ++i)
	{
		UChaosVehicleWheel* W = Vehicle->Wheels[i];
		if (!W) continue;
		const FVector Base = WheelPos[i] + FVector(0, 0, -W->GetSuspensionOffset());
		WheelMeshes[i]->SetRelativeLocation(Base);
		const float Spin = W->GetRotationAngle();
		const float Steer = W->GetSteerAngle();
		WheelMeshes[i]->SetRelativeRotation(FRotator(-Spin, Steer, 0.f));
	}

	// effects and sound
	const bool bSpark = (bWallLeft || bWallRight) && FMath::Abs(Speed) > 1500.f && !bWrecked;
	if (SparksFX->GetAsset())
	{
		SparksFX->SetRelativeLocation(FVector(-60.f, bWallLeft ? -130.f : 130.f, 30.f));
		if (bSpark && !SparksFX->IsActive()) SparksFX->Activate(true);
		if (!bSpark && SparksFX->IsActive()) SparksFX->Deactivate();
	}
	if (TurboFX->GetAsset())
	{
		if (Drive.bHot && !TurboFX->IsActive()) TurboFX->Activate(true);
		if (!Drive.bHot && TurboFX->IsActive()) TurboFX->Deactivate();
	}
	const bool bSlide = Drive.DriftDir != 0 && bGrounded;
	if (DriftFX->GetAsset())
	{
		if (bSlide && !DriftFX->IsActive()) DriftFX->Activate(true);
		if (!bSlide && DriftFX->IsActive()) DriftFX->Deactivate();
	}
	if (EngineAudio->IsPlaying())
	{
		const float S = FMath::Clamp(FMath::Abs(Speed) / 9600.f, 0.f, 1.f);
		EngineAudio->SetFloatParameter(TEXT("Speed"), S);
		EngineAudio->SetFloatParameter(TEXT("Turbo"), Drive.bHot ? 1.f : 0.f);
		EngineAudio->SetPitchMultiplier(0.75f + S * 0.9f + (Drive.bHot ? 0.12f : 0.f));
	}
	if (SkidAudio->IsPlaying()) SkidAudio->SetVolumeMultiplier(bSlide ? 0.7f : 0.f);
}

void AFTPlayerCar::ApplyArcade(float Dt)
{
	if (bWrecked)
	{
		Chassis->AddForce(FVector(0, 0, -400.f), NAME_None, true);
		return;
	}
	const FFTFrame F = Track->Sample(D);
	HopCd = FMath::Max(0.f, HopCd - Dt);

	// 2.5: the surface under the car. Flat road: the frame itself. Half pipe: the U's curve (height and slope
	// across). Coaster: the car's own rail, followed by continuity so a cross-over keeps it on the same rail.
	bool bSolid = true;
	float Slope = 0.f;
	const bool bCoaster = (F.Flags & FTFlag::Coaster) != 0;
	float RailX = 0.f;
	if (bCoaster)
	{
		const FFTRails R = Track->RailsAt(D);
		if (!bRailInit) { RailPt = FMath::Abs(X - R.XL) < FMath::Abs(X - R.XR) ? FVector2D(R.XL, R.HL) : FVector2D(R.XR, R.HR); bRailInit = true; }
		const FVector2D PL(R.XL, R.HL), PR(R.XR, R.HR);
		RailPt = FVector2D::DistSquared(RailPt, PL) < FVector2D::DistSquared(RailPt, PR) ? PL : PR;
		RailX = RailPt.X; SurfH = RailPt.Y;
		bSolid = FMath::Abs(X - RailX) < FFTRails::HalfW + 60.f;
	}
	else
	{
		bRailInit = false;
		SurfH = Track->SurfaceH(D, X, &Slope, &bSolid);
	}
	SurfN = (F.Up - F.Right * Slope).GetSafeNormal();
	const FVector Up = SurfN;
	if (Drive.bFrozen)
	{
		// parked for the slate: no throttle, no brake (a brake at a standstill engages reverse) and no
		// handbrake (a handbraked, stopped Chaos car parks and stops simulating its suspension)
		Vehicle->SetThrottleInput(0.f);
		Vehicle->SetBrakeInput(0.f);
		Vehicle->SetHandbrakeInput(false);
		Vehicle->SetSteeringInput(0.f);
		return;
	}
	Vehicle->SetHandbrakeInput(false);

	// track gravity: the road pulls the car onto itself (loops, pipes, walls); far above it, plain gravity.
	// (Along the frame's up, not the half pipe's wall: up a wall, it pulls you back down into the U.)
	const bool bGap = (F.Flags & FTFlag::Gap) != 0 || !bSolid;
	const float Hr = H - SurfH; // height over the surface under the car
	const float RoadW = bGap ? 0.f : FMath::Clamp(1.f - (Hr - 500.f) / 1200.f, 0.f, 1.f);
	const FVector GDir = ((-F.Up) * RoadW + FVector(0, 0, -1.f) * (1.f - RoadW)).GetSafeNormal();
	const float GMag = 980.f * (bGrounded ? 1.f : AirGravityScale) + (bGrounded ? FMath::Clamp(FMath::Abs(Speed) - 5000.f, 0.f, 6000.f) * 0.12f : 0.f);
	const int32 AM = CVarFTArcade.GetValueOnGameThread();
	if (AM & 16) Chassis->AddForce(GDir * GMag + FVector(0, 0, 980.f), NAME_None, true);

	// 2.4 glue: everywhere but a real jump the car is pressed onto the road like a slot car. Crests, bumps,
	// kerbs, hits and landings can't lift it: speed away from the road is bled off within a few frames, and a
	// lifted car is pulled back down hard. A jump section's kicker and gap, and a ramp launch, fly as before.
	LaunchT = FMath::Max(0.f, LaunchT - Dt);
	if (LaunchT < 1.3f && bGrounded) LaunchT = 0.f; // landed after a launch
	const bool bRealJump = (F.Flags & (FTFlag::Kicker | FTFlag::Gap)) != 0 || ((F.Flags & FTFlag::Landing) && !bGrounded) || LaunchT > 0.f || !bSolid;
	// height over the surface measured along its normal (on a half pipe wall the frame's up is tilted off it)
	const float Lift = Hr / FMath::Sqrt(1.f + Slope * Slope) - ChassisZ;

	// 2.5 set pieces (loops, corkscrews, pipes, coaster rails): the car keeps the speed it came in with, all the
	// way round. The owner: "some of the corkscrews really slow the car down ... it needs to maintain speed".
	// The floor is the way-in speed (at least cruising speed); nothing here ever slows the car down.
	{
		const uint16 Kind = F.Flags & (FTFlag::SetPiece | FTFlag::Coaster);
		const bool bSetNow = Kind != 0 && !(F.Flags & (FTFlag::Kicker | FTFlag::Gap));
		const float VRoad = FVector::DotProduct(Chassis->GetPhysicsLinearVelocity(), F.Fwd);
		if (bSetNow && !bInSet)
		{
			bInSet = true; SetKind = Kind; SetEntry = VRoad; SetMin = VRoad; SetMaxLift = -1e6f; SetAir = 0.f; SetD0 = D; SetSnaps = 0;
			SetFloor = FMath::Max(VRoad, FT::BaseSpeed) * 0.98f;
		}
		else if (!bSetNow && bInSet)
		{
			bInSet = false;
			static IConsoleVariable* AutoS = IConsoleManager::Get().FindConsoleVariable(TEXT("ft.Autopilot"));
			if (AutoS && AutoS->GetInt() > 0)
				UE_LOG(LogFT, Display, TEXT("FTSET kind=%s D=%.0f entry=%.0f min=%.0f exit=%.0f kept=%.0f%% maxLift=%.0f air=%.2fs snaps=%d"),
					(SetKind & FTFlag::Coaster) ? TEXT("coaster") : ((SetKind & FTFlag::Loop) ? TEXT("loop") : ((SetKind & FTFlag::Pipe) ? TEXT("pipe") : TEXT("twist"))),
					SetD0, SetEntry, SetMin, VRoad, 100.f * SetMin / FMath::Max(SetEntry, 1.f), SetMaxLift, SetAir, SetSnaps);
		}
		if (bInSet)
		{
			// a turbo or a drift boost inside raises the floor: what you gain in there you keep
			SetFloor = FMath::Max(SetFloor, FMath::Min(VRoad, Drive.TargetSpeed) * 0.98f);
			if (VRoad < SetFloor && !bWrecked)
			{
				FVector Vn = Chassis->GetPhysicsLinearVelocity() + F.Fwd * (SetFloor - VRoad);
				Chassis->SetPhysicsLinearVelocity(Vn);
			}
			SetMin = FMath::Min(SetMin, FVector::DotProduct(Chassis->GetPhysicsLinearVelocity(), F.Fwd));
			SetMaxLift = FMath::Max(SetMaxLift, Lift);
			if (!bGrounded) SetAir += Dt;
		}
	}

	// 2.5: you always clear a gap (the 1.x rule). The 2.4 runs fell short of the jumps whenever the car reached the
	// lip under ~5,000 cm/s (a 30 m drop and a reset); off the kicker it now leaves at 6,800 or more.
	if ((F.Flags & FTFlag::Kicker) && !bWrecked)
	{
		const float VRoadK = FVector::DotProduct(Chassis->GetPhysicsLinearVelocity(), F.Fwd);
		if (VRoadK < 6800.f) Chassis->SetPhysicsLinearVelocity(Chassis->GetPhysicsLinearVelocity() + F.Fwd * (6800.f - VRoadK));
	}

	// 2.5 coaster: the car rides its rail like a coaster car; a strong damped spring holds it across. During a hop
	// it carries the car over to the other rail.
	// The owner: "this part you don't really need to do much except accelerate". So on the rails the car rides like
	// a coaster car: its place across and its height are its rail's (or its hop's arc), it faces down the rail, and
	// only its speed is physics (throttle, turbo and the set-piece floor still work). Coming onto the rails it slides
	// over to its rail at up to 30 m/s rather than jumping there.
	if (bCoaster && !bWrecked)
	{
		float TX = RailX, TH = SurfH;
		if (HopT >= 0.f)
		{
			HopT += Dt;
			const float U = FMath::Clamp(HopT / HopDur, 0.f, 1.f);
			TX = FMath::Lerp(HopFrom.X, RailX, U * U * (3.f - 2.f * U));
			TH = FMath::Lerp(HopFrom.Y, SurfH, U) + 320.f * 4.f * U * (1.f - U);
			if (U >= 1.f) HopT = -1.f;
		}
		const float MaxStep = 3000.f * Dt;
		const float NX = X + FMath::Clamp(TX - X, -MaxStep, MaxStep);
		const float NH = Hr + SurfH; // current height over the frame
		const float WantH = TH + ChassisZ - 2.f;
		const float NewH = NH + FMath::Clamp(WantH - NH, -MaxStep, MaxStep);
		// how the rail itself moves across and up as the car runs along it
		const FFTRails R2 = Track->RailsAt(D + 200.f);
		const FVector2D P2a(R2.XL, R2.HL), P2b(R2.XR, R2.HR);
		const FVector2D P2 = FVector2D::DistSquared(RailPt, P2a) < FVector2D::DistSquared(RailPt, P2b) ? P2a : P2b;
		const float VRoadC = FMath::Max(FVector::DotProduct(Chassis->GetPhysicsLinearVelocity(), F.Fwd), bInSet ? SetFloor : 0.f);
		const float VX = HopT >= 0.f ? 0.f : (P2.X - RailX) / 200.f * VRoadC;
		const float VH = HopT >= 0.f ? 0.f : (P2.Y - SurfH) / 200.f * VRoadC;
		Chassis->SetWorldTransform(Track->TransformAt(D, NX, NewH), false, nullptr, ETeleportType::TeleportPhysics);
		Chassis->SetPhysicsLinearVelocity(F.Fwd * VRoadC + F.Right * VX + F.Up * VH);
		Chassis->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);
		X = NX;
	}
	else HopT = -1.f;

	// 2.5 corkscrews: the road rolls round its centreline, so off-centre the deck sweeps through a car that isn't
	// rolling with it (3.3 rad/s at turbo speed) and the depenetration threw it off, or under the deck. Held near
	// the centreline, and its roll turned with the road's.
	// (The roll is matched on all road, not only corkscrews: where a 57-degree banked wall unwinds to flat in 25 m,
	// the Chemical Works road rolls 2 rad/s under a car at its edge, and its nose planted into the deck.)
	if (!bCoaster && !bWrecked && !bRealJump && (bGrounded || (F.Flags & FTFlag::SetPiece)))
	{
		const FVector Vc = Chassis->GetPhysicsLinearVelocity();
		if (F.Flags & FTFlag::Twist)
		{
			const float VLatR = FVector::DotProduct(Vc, F.Right);
			Chassis->AddForce(F.Right * FMath::Clamp(-X * 22.f - VLatR * 9.f, -12000.f, 12000.f), NAME_None, true);
		}
		const FFTFrame F2 = Track->Sample(D + 100.f);
		const float DRoll = FMath::Atan2(FVector::DotProduct(FVector::CrossProduct(F.Up, F2.Up), F.Fwd), FVector::DotProduct(F.Up, F2.Up));
		const float RollRate = DRoll / 100.f * FVector::DotProduct(Vc, F.Fwd);
		// plus a pull back square to the surface (the car's own lean off it, about the road's forward axis)
		const FVector CarUpR = Chassis->GetUpVector();
		const float RollErr = FMath::Atan2(FVector::DotProduct(FVector::CrossProduct(CarUpR, Up), F.Fwd), FVector::DotProduct(CarUpR, Up));
		FVector Wc = Chassis->GetPhysicsAngularVelocityInRadians();
		Wc += F.Fwd * (RollRate + RollErr * 6.f - FVector::DotProduct(Wc, F.Fwd));
		Chassis->SetPhysicsAngularVelocityInRadians(Wc);
	}

	// 2.5 put back: inside a loop, corkscrew or pipe a car more than 3 m off the surface, or anywhere a car found under
	// the deck (its box can't sink more than ~56 cm into its springs, so past 60 cm it went through), goes straight
	// back on top of the road where it is, at its speed along the road.
	if (!bWrecked && !bRealJump && !bCoaster && FMath::Abs(X) < FT::Edge + 2500.f)
	{
		const bool bSetP = (F.Flags & FTFlag::SetPiece) != 0;
		if ((bSetP && Lift > 300.f) || (Lift < -60.f && WheelsDown == 0))
		{
			const float Xc = FMath::Clamp(X, -FT::RoadHalf, FT::RoadHalf);
			const float Vr = FMath::Max(FVector::DotProduct(Chassis->GetPhysicsLinearVelocity(), F.Fwd), bInSet ? SetFloor : 0.f);
			UE_LOG(LogFT, Display, TEXT("FTPUTBACK D=%.0f X=%.0f lift=%.0f v=%.0f %s"), D, X, Lift, Vr, bSetP ? TEXT("setpiece") : TEXT("road"));
			Chassis->SetWorldTransform(Track->TransformAt(D, Xc, Track->SurfaceH(D, Xc) + ChassisZ + 6.f), false, nullptr, ETeleportType::TeleportPhysics);
			Chassis->SetPhysicsLinearVelocity(F.Fwd * Vr);
			Chassis->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);
			if (bInSet) ++SetSnaps;
			return;
		}
	}

	if ((AM & 64) && !bRealJump && Hr < 4000.f)
	{
		FVector Vg = Chassis->GetPhysicsLinearVelocity();
		float Away = FVector::DotProduct(Vg, Up);
		// 2.5 stick: the owner's "just have the car stick to the road". Once the car is lifting off (over ride
		// height + 4 cm) any speed away from the road is taken out, so a crest, a kerb, a hit or a landing bounce
		// can't throw it. (Only removed, never turned along the road: the first 2.5 build kept the speed's size,
		// and a bounce off a pipe's barrier came back as 14,000 cm/s sideways and threw the car out of the pipe.)
		// Lifted further than StickMaxLift anyway, it is put straight back down onto its springs.
		if (Lift > 4.f && Away > 0.f)
		{
			Vg -= Up * Away;
			Chassis->SetPhysicsLinearVelocity(Vg);
			Away = 0.f;
		}
		// (only with every wheel off, and at most 35 cm a frame: a bad height reading must never push it through the deck)
		if (Lift > StickMaxLift && WheelsDown == 0 && !bWrecked)
		{
			const FVector P = Chassis->GetComponentLocation() - Up * FMath::Min(Lift - 8.f, 35.f);
			Chassis->SetWorldLocation(P, false, nullptr, ETeleportType::TeleportPhysics);
			if (bInSet) ++SetSnaps;
		}
		// lifted is judged by the chassis's height over the road, not by the wheels: pressed past the suspension's
		// travel, the wheel traces start under the road and read "in the air", and a pull keyed to that kept the
		// car pinned on its belly (first 2.4 build). Ride height is ~85 cm; nothing extra once it is down there.
		float Pull = bGrounded && Lift > -12.f ? Downforce * 980.f : 0.f;
		// a critically damped spring on the lift (GlueK, GlueDamp = 2 sqrt(GlueK)): hard down when high, easing off
		// as it closes, and nothing extra while it is already coming down fast, so it lands on the springs instead of
		// slamming through their 14 cm onto its belly (a flat 7 g pull did that, second 2.4 build). Never pushes up.
		// Only above ride height: below it the springs are lifting the car back up, and that must not be fought.
		if (Lift > -4.f)
		{
			// never take off more than the away speed itself in one step (stays stable at low frame rates)
			float DampA = Away > 0.f ? FMath::Min(Away * GlueDamp, Away / FMath::Max(Dt, 1e-3f)) : Away * GlueDamp;
			// in pipes, loops and twists the surface itself turns away from the car, so "coming toward the road"
			// (Away < 0) says nothing about closing in on it: there the spring always pulls in full, up to 20 g
			const bool bSet = (F.Flags & (FTFlag::Pipe | FTFlag::Loop | FTFlag::Twist)) != 0;
			if (bSet) DampA = FMath::Max(DampA, 0.f);
			Pull += FMath::Clamp(GlueK * FMath::Max(Lift, 0.f) + DampA, 0.f, bSet ? 20000.f : 12000.f);
		}
		// down on its belly (wheel traces start under the road and find nothing): lift it back onto its wheels
		// instead of sitting there until the crew resets it 1.5 s later
		if (Lift < -35.f && WheelsDown == 0) Pull = -2600.f;
		// pipes, loops and twists: the road rolls under the car, and a car that loses its wheels there slid off the
		// ribbon sideways (X past the barrier; the pull above is only along the road's normal). Hold it across too:
		// a spring back from 2 m inside the paint's edge, and the sideways speed damped while it is off its wheels.
		if ((F.Flags & (FTFlag::Pipe | FTFlag::Loop | FTFlag::Twist)) && !bCoaster)
		{
			// 2.5: never off the side of the ribbon in there: past 1.5 m inside the barrier it is put back, and its
			// outward speed is taken away
			const float XMax = FT::Edge - 150.f;
			if (FMath::Abs(X) > XMax && !bWrecked)
			{
				const float Back = FMath::Abs(X) - (XMax - 50.f);
				Chassis->SetWorldLocation(Chassis->GetComponentLocation() - F.Right * FMath::Sign(X) * Back, false, nullptr, ETeleportType::TeleportPhysics);
				const float Out = FVector::DotProduct(Vg, F.Right) * FMath::Sign(X);
				if (Out > 0.f) { Vg -= F.Right * FMath::Sign(X) * Out; Chassis->SetPhysicsLinearVelocity(Vg); }
				if (bInSet) ++SetSnaps;
			}
			const float VLatR = FVector::DotProduct(Vg, F.Right);
			const float Over = FMath::Sign(X) * FMath::Max(FMath::Abs(X) - 900.f, 0.f);
			// damp the sideways speed while off the wheels, and always when it is heading out past 9 m
			const bool bOutward = Over * VLatR > 0.f;
			const float LatA = GlueK * Over + ((!bGrounded || bOutward) ? GlueDamp * VLatR : 0.f);
			if (LatA != 0.f) Chassis->AddForce(-F.Right * FMath::Clamp(LatA, -20000.f, 20000.f), NAME_None, true);
		}
		Chassis->AddForce(-Up * Pull, NAME_None, true);
	}

	const FVector CarF = Chassis->GetForwardVector();
	const FVector Cf = (CarF - Up * FVector::DotProduct(CarF, Up)).GetSafeNormal();
	const FVector Cr = FVector::CrossProduct(Up, Cf);
	const FVector V = Chassis->GetPhysicsLinearVelocity();
	const float VAlong = FVector::DotProduct(V, Cf);
	const float VLat = FVector::DotProduct(V, Cr);
	const FVector W = Chassis->GetPhysicsAngularVelocityInRadians();
	const float YawRate = FVector::DotProduct(W, Up);
	const float HeadErr = FMath::Atan2(FVector::DotProduct(FVector::CrossProduct(F.Fwd, Cf), Up), FVector::DotProduct(Cf, F.Fwd));

	DriftBlend = FMath::FInterpTo(DriftBlend, Drive.DriftDir != 0 ? 1.f : 0.f, Dt, 6.f);
	const float RearFr = FMath::Lerp(RearFriction, DriftRearFriction, DriftBlend);
	Vehicle->SetWheelFrictionMultiplier(2, RearFr);
	Vehicle->SetWheelFrictionMultiplier(3, RearFr);

	// Chaos inputs: the engine drives the rear wheels; the governor below keeps the arcade speed
	const float Gap = Drive.TargetSpeed - VAlong;
	Vehicle->SetThrottleInput(FMath::Clamp(Gap / 900.f + 0.35f, 0.f, 1.f));
	Vehicle->SetBrakeInput(Gap < -1500.f && VAlong > 800.f && !bInSet ? 0.4f : 0.f);
	Vehicle->SetSteeringInput(Drive.Steer * (Drive.DriftDir != 0 ? 0.25f : 0.55f));

	if (bGrounded)
	{
		const float A = FMath::Clamp(Gap * 2.6f, bInSet ? 0.f : -3800.f, Drive.Accel);
		if (AM & 1) Chassis->AddForce(Cf * A, NAME_None, true);

		// grip: remove sideways slip unless sliding on purpose
		const float LatTarget = Drive.DriftDir != 0 ? Drive.DriftTargetLat : 0.f;
		const float GripNow = FMath::Lerp(Grip, 2.2f, DriftBlend);
		if (AM & 2) Chassis->AddForce(Cr * FMath::Clamp((LatTarget - VLat) * GripNow, -9000.f, 9000.f), NAME_None, true);

		// yaw: steer rate, plus following the bend, plus drift angle
		const float SK = FMath::Clamp(FMath::Abs(VAlong) / 9600.f, 0.f, 1.f);
		const float RoadRate = F.Curve * 1e-3f / 1.5f / FT::DU * VAlong;
		float Want = Drive.Steer * FMath::Lerp(SteerYawRateLow, SteerYawRateHigh, SK) + RoadRate * RoadFollow;
		if (Drive.DriftDir != 0)
		{
			const float Angle = Drive.DriftDir * FMath::DegreesToRadians(24.f) + Drive.Steer * FMath::DegreesToRadians(12.f);
			Want = RoadRate + (Angle - HeadErr) * 2.4f + Drive.Steer * 0.4f;
		}
		else
		{
			Want += -HeadErr * 2.4f * (1.f - FMath::Abs(Drive.Steer)) - HeadErr * 0.6f;
			// never let it swing more than ~40 degrees off the road unless drifting
			if (FMath::Abs(HeadErr) > 0.7f) Want = RoadRate - FMath::Sign(HeadErr) * 2.5f;
		}
		if (AM & 4) Chassis->AddTorqueInRadians(Up * FMath::Clamp((Want - YawRate) * 11.f, -40.f, 40.f), NAME_None, true);
		// keep it square to the road (anti-flip): roll/pitch toward the road's normal, damped
		{
			const FVector CarUp = Chassis->GetUpVector();
			const FVector Tilt = FVector::CrossProduct(CarUp, Up);
			const FVector WNoYaw = W - Up * FVector::DotProduct(W, Up);
			if (AM & 8) Chassis->AddTorqueInRadians(Tilt * 7.f - WNoYaw * 2.5f, NAME_None, true);
		}
		static float DbgT = 0.f; DbgT += Dt;
		static IConsoleVariable* AutoV = IConsoleManager::Get().FindConsoleVariable(TEXT("ft.Autopilot"));
		if (DbgT > 1.f && AutoV && AutoV->GetInt() > 0) { DbgT = 0.f; UE_LOG(LogFT, Display, TEXT("FTCAR herr=%.1f yawrate=%.2f want=%.2f valong=%.0f vlat=%.0f tgt=%.0f steer=%.2f up=%.2f fwdz=%.2f H=%.0f susp=%.1f,%.1f,%.1f,%.1f thr=%.2f"), FMath::RadiansToDegrees(HeadErr), YawRate, Want, VAlong, VLat, Drive.TargetSpeed, Drive.Steer,
			FVector::DotProduct(Chassis->GetUpVector(), Up), FVector::DotProduct(Chassis->GetForwardVector(), Up), H,
			Vehicle->Wheels.Num() > 3 ? Vehicle->Wheels[0]->GetSuspensionOffset() : 0.f, Vehicle->Wheels.Num() > 3 ? Vehicle->Wheels[1]->GetSuspensionOffset() : 0.f,
			Vehicle->Wheels.Num() > 3 ? Vehicle->Wheels[2]->GetSuspensionOffset() : 0.f, Vehicle->Wheels.Num() > 3 ? Vehicle->Wheels[3]->GetSuspensionOffset() : 0.f, FMath::Clamp(Gap / 900.f + 0.35f, 0.f, 1.f)); }
	}
	else
	{
		// in the air: level out to land on the wheels, and point roughly down the road
		const FVector TargetUp = RoadW > 0.2f ? Up : FVector::UpVector;
		const FVector CarUp = Chassis->GetUpVector();
		const FVector Axis = FVector::CrossProduct(CarUp, TargetUp);
		const float YawErr = FMath::Atan2(FVector::DotProduct(FVector::CrossProduct(Chassis->GetForwardVector(), F.Fwd), CarUp), FVector::DotProduct(Chassis->GetForwardVector(), F.Fwd));
		const FVector Tq = Axis * 9.f + CarUp * (YawErr * 2.5f + Drive.Steer * 1.2f) - W * 2.2f;
		if (AM & 32) Chassis->AddTorqueInRadians(Tq, NAME_None, true);
		// you always clear a gap: keep enough speed to make the far side
		if (bGap && VAlong < 6000.f) Chassis->AddForce(F.Fwd * 2500.f, NAME_None, true);
		// 2.5: off its wheels inside a set piece the engine still drives it on (the floor above holds the rest)
		if (bInSet && (AM & 1)) Chassis->AddForce(F.Fwd * FMath::Clamp(Gap * 2.6f, 0.f, Drive.Accel), NAME_None, true);
	}
}
