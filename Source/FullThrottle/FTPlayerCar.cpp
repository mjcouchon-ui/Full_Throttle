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
#include "FTFx.h"

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
	// 3.3 where a hanging trooper grips the car (left side; the right is mirrored): the door's window sill
	const FVector HangGrip(20.f, 100.f, 114.f);
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
		// 3.3: the wheels hang off the chassis, not the body, so the body can roll and pitch on its springs above them
		W->SetupAttachment(Chassis);
		W->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		W->SetRelativeLocation(WheelPos[i] - FVector(0.f, 0.f, ChassisZ));
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
	// FX overhaul: sparks off the rear tyres in the drift charge's colour; the hot core of the turbo flame
	DriftSparkFX = FX(TEXT("DriftSparkFX"), FVector(-133, 0, 12));
	BoostCoreFX = FX(TEXT("BoostCoreFX"), FVector(-232, 0, 30));

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
	// 3.4 perf: the car moves every frame, so while its meshes fed the distance fields the engine re-baked the global
	// distance field round it and re-captured its Lumen cards every frame (GlobalDistanceFieldUpdate "Movable" 0.21 ms
	// and ~37 card captures at EPIC). The enemies have been out of both since 2.8; the car looks the same without them
	// (its shadow is the virtual shadow map's, its reflections the screen's).
	TInlineComponentArray<UStaticMeshComponent*> CarMeshes(this);
	for (UStaticMeshComponent* C : CarMeshes)
	{
		C->bAffectDistanceFieldLighting = false;
		C->bAffectDynamicIndirectLighting = false;
		C->MarkRenderStateDirty();
	}
	// 3.3: the SWAT trooper hanging off the door (SM_HenchmanHang: origin at his grip, chest toward the car on +Y; the
	// right-hand one is mirrored); the old standing henchman if the model is missing
	UStaticMesh* HMHang = Load<UStaticMesh>(TEXT("/Game/Characters/Syndicate/SM_HenchmanHang"));
	UStaticMesh* HM = HMHang ? HMHang : Load<UStaticMesh>(TEXT("/Game/Characters/Syndicate/SM_Henchman"));
	bHangModel = HMHang != nullptr;
	for (int32 i = 0; i < Henchmen.Num(); ++i)
	{
		UStaticMeshComponent* Hm = Henchmen[i];
		if (!Hm->GetStaticMesh()) Hm->SetStaticMesh(HM);
		if (bHangModel)
		{
			// the right-hand one is the same model turned round (mirroring it would print SWAT backwards)
			Hm->SetRelativeLocation(FVector(HangGrip.X, i == 0 ? -HangGrip.Y : HangGrip.Y, HangGrip.Z));
			Hm->SetRelativeRotation(FRotator(0.f, i == 0 ? 0.f : 180.f, 0.f));
			Hm->SetRelativeScale3D(FVector(1.f));
		}
	}
	auto SetFX = [](UNiagaraComponent* C, const TCHAR* P) { if (!C->GetAsset()) C->SetAsset(Load<UNiagaraSystem>(P)); };
	SetFX(SmokeFX, TEXT("/Game/FX/NS_Smoke"));
	// 2.4: the car burns with its own flat cel fire (NS_CarFire, MI_Fire_Car); NS_Fire's white-hot puffs hid it
	SetFX(FireFX, TEXT("/Game/FX/NS_CarFire"));
	SetFX(SparksFX, TEXT("/Game/FX/NS_Sparks"));
	SetFX(TurboFX, TEXT("/Game/FX/NS_TurboFlame"));
	SetFX(DriftFX, TEXT("/Game/FX/NS_TyreSmoke"));
	SetFX(BoostCoreFX, TEXT("/Game/FX/NS_TurboCore"));   // FX overhaul (the drift sparks' system is picked by the charge, in TickFx)
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
	// 3.3: three rails; hop to the neighbour on that side where it sits beside yours
	const FFTRails R = Track->RailsAt(D);
	return R.Neighbour(R.Nearest(RailPt), Dir) >= 0;
}

int32 AFTPlayerCar::RailIndex() const
{
	if (!bRailInit || !Track) return -1;
	return Track->RailsAt(D).Nearest(RailPt);
}

bool AFTPlayerCar::RailHop(int32 Dir)
{
	if (!CanRailHop(Dir)) return false;
	const FFTRails R = Track->RailsAt(D);
	const int32 To = R.Neighbour(R.Nearest(RailPt), Dir);
	// up and over on a set arc that always lands on the other rail (the first 2.5 build flew it with physics and a
	// spring, and one hop in four missed and fell between the rails)
	HopFrom = FVector2D(X, SurfH);
	RailPt = R.P(To);
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
		int32 Bi = 0;
		for (int32 i = 1; i < FFTRails::Num; ++i) if (FMath::Abs(InX - R.X[i]) < FMath::Abs(InX - R.X[Bi])) Bi = i;
		InX = R.X[Bi]; Hs = R.H[Bi];
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
	Half = EFTHalf::None;
	bPrevVel = false;
	bWrecked = false;
	// FX overhaul: Wreck() puts the underglow and the headlights out and nothing put them back, so every take after
	// the first wreck drove on without them. And the effects' counters start again with the car
	if (!Underglow->IsVisible()) { Underglow->SetVisibility(true); HeadL->SetVisibility(true); HeadR->SetVisibility(true); }
	FxAirT = 0.f;
	ShedQuietT = GetWorld() ? GetWorld()->GetTimeSeconds() + 0.5f : -1.f;
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
	// FX overhaul: the step itself shows as the armour drops into smoking and into burning: a cough of smoke off
	// the engine deck, then the fire catching with a pop and sparks. Not on a repair, and not when a take starts
	// with the car already in that state
	const int32 Stage = bFire ? 2 : (bSmoke ? 1 : 0);
	if (DamageStage >= 0 && Stage > DamageStage && !bWrecked)
	{
		if (Stage == 2)
		{
			FTFx::Attach(TEXT("NS_FxFireball"), Body, FVector(-115.f, 0.f, 96.f), FRotator::ZeroRotator, 0.3f, FTFx::Main, true);
			FTFx::Attach(TEXT("NS_RamSparks"), Body, FVector(-115.f, 0.f, 96.f), FRotator(60.f, 180.f, 0.f), 0.7f, FTFx::Extra, true);
		}
		else FTFx::Attach(TEXT("NS_MissileBackblast"), Body, FVector(-110.f, 0.f, 112.f), FRotator(-30.f, 0.f, 0.f), 0.6f, FTFx::Extra, true);
	}
	DamageStage = Stage;
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
	// FX overhaul: it comes off with sparks where it was bolted on (not the parts a damaged take starts without)
	if (GetWorld() && GetWorld()->GetTimeSeconds() > ShedQuietT)
		FTFx::Attach(TEXT("NS_RamSparks"), Body, P->GetRelativeLocation(), FRotator(25.f, 180.f, 0.f), 0.6f, FTFx::Extra, true);
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
	ShedQuietT = GetWorld() ? GetWorld()->GetTimeSeconds() + 0.5f : -1.f;   // FX overhaul: a repair puts them back and sheds again by the armour left: quietly
}

void AFTPlayerCar::SetHenchmenForLevel(int32 Level)
{
	if (Level <= 0 || !bHangModel) return;
	if (UStaticMesh* Themed = Load<UStaticMesh>(FString::Printf(TEXT("/Game/Characters/Themed/L%d/SM_HenchmanHang"), Level)))
	{
		for (UStaticMeshComponent* Hm : Henchmen) Hm->SetStaticMesh(Themed);
	}
}

void AFTPlayerCar::SetHenchmenVisible(bool bLeft, bool bRight, FVector2D Age, FVector2D UntilPunch)
{
	Henchmen[0]->SetVisibility(bLeft);
	Henchmen[1]->SetVisibility(bRight);
	// 3.3: they swing on the door, legs trailing in the wind
	if (bHangModel && (bLeft || bRight))
	{
		const float T = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
		for (int32 i = 0; i < 2; ++i)
		{
			if ((i == 0 && !bLeft) || (i == 1 && !bRight)) continue;
			// the body swings out from the door (about 40 degrees, so his boots clear the road) and trails back in the
			// wind. In his own frame he hangs on -Y; the right-hand one is turned round (yaw 180), so "back" flips.
			const float HangTime = Age[i];
			const float Catch = FMath::Exp(-8.f * HangTime) * FMath::Cos(15.f * HangTime);
			const float Windup = 1.f - FMath::SmoothStep(0.f, 0.24f, static_cast<float>(UntilPunch[i]));
			const float Out = 40.f + 6.f * FMath::Sin(T * 5.3f + 1.7f * i) + 12.f * Catch - 7.f * Windup;
			const float Back = (22.f + 5.f * FMath::Sin(T * 7.f + i) - 9.f * Catch + 5.f * Windup) * (i == 0 ? 1.f : -1.f);
			const FQuat Local = FQuat(FVector::ForwardVector, FMath::DegreesToRadians(-Out)) * FQuat(FVector::RightVector, FMath::DegreesToRadians(Back));
			Henchmen[i]->SetRelativeTransform(FTransform(FQuat(FVector::UpVector, i == 0 ? 0.f : PI) * Local,
				FVector(HangGrip.X, i == 0 ? -HangGrip.Y : HangGrip.Y, HangGrip.Z), FVector(1.f, 1.f, 1.f - 0.055f * FMath::Abs(Catch))));
		}
	}
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
	{
		// FX overhaul: the effects' own frame (blasts that roll on, their delayed layers), and a clean start for every
		// new scene. Parked for a slate (a retake, the title, PRE-PRODUCTION) or hidden for the highlight reel, nothing
		// of the scene before is left burning or smoking: FTFx::ClearWorld stops it and logs what it stopped
		const bool bStage = Drive.bFrozen || IsHidden();
		if (bStage && !bFxStage) FTFx::ClearWorld(this, IsHidden() ? TEXT("reel") : TEXT("slate"));
		bFxStage = bStage;
		FTFx::Tick(this, Dt);
	}
	if (!Track || !Track->IsBuilt() || !Chassis->IsSimulatingPhysics()) return;

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
				if (AirT0 > 0.25f) { UE_LOG(LogFT, Display, TEXT("FTJUMP kind=%s from D=%.0f air=%.2fs rise=%.0f dist=%.0f lift=%.0f..%.0f"), Kind, D0, AirT0, ZMax - Z0, D - D0, LiftMin, LiftMax); }
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

#if !UE_BUILD_SHIPPING
	// 3.6 (owner: "a part of the track in level 2 where my car hit and slowed it down a lot"): every sudden loss of
	// speed, with where it happened and what the car last touched (test runs read FTBUMP)
	if (!bWrecked && !Drive.bFrozen && GetWorld())
	{
		static TArray<FVector2f> Hist; static float LastBumpT = -10.f;
		const float Now = GetWorld()->GetTimeSeconds();
		Hist.Add(FVector2f(Now, Speed));
		while (Hist.Num() && Hist[0].X < Now - 0.5f) Hist.RemoveAt(0, 1, EAllowShrinking::No);
		float Peak = Speed; float PeakT = Now;
		for (const FVector2f& Hs : Hist) if (Hs.Y > Peak) { Peak = Hs.Y; PeakT = Hs.X; }
		if (Peak - Speed > 1800.f && Speed < Peak * 0.75f && Now - LastBumpT > 1.5f)
		{
			LastBumpT = Now;
			float U = 0.f; const FFTSection* Sc = Track->SectionAt(D, &U);
			UE_LOG(LogFT, Display, TEXT("FTBUMP D=%.0f X=%.0f H=%.0f v=%.0f from %.0f in %.2fs target=%.0f section=%d shape=%d u=%.2f flags=%d wheels=%d hit(%.2fs ago)=%s"),
				D, X, H, Speed, Peak, Now - PeakT, Drive.TargetSpeed, F.Section, Sc ? (int32)Sc->Shape : -1, U, (int32)F.Flags, WheelsDown,
				Now - LastHitT, Now - LastHitT < 0.7f ? *LastHit : TEXT("-"));
		}
	}
#endif
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
		// (in the air the suspension hangs all the way down: the wheels drop away from the body, then the body
		// lands on them; 3.3 eases the reported offset so a landing reads as a compression, not a pop)
		const FVector Base = WheelPos[i] + FVector(0, 0, -W->GetSuspensionOffset() - ChassisZ);
		WheelMeshes[i]->SetRelativeLocation(FMath::VInterpTo(WheelMeshes[i]->GetRelativeLocation(), Base, Dt, 30.f));
		const float Spin = W->GetRotationAngle();
		const float Steer = W->GetSteerAngle();
		WheelMeshes[i]->SetRelativeRotation(FRotator(-Spin, Steer, 0.f));
	}
	TickBody(Dt);

	// effects and sound
	const bool bSpark = (bWallLeft || bWallRight) && FMath::Abs(Speed) > 1500.f && !bWrecked;
	if (SparksFX->GetAsset() && Half != EFTHalf::Grind)
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
	TickFx(Dt, V);
	if (EngineAudio->IsPlaying())
	{
		const float S = FMath::Clamp(FMath::Abs(Speed) / 9600.f, 0.f, 1.f);
		EngineAudio->SetFloatParameter(TEXT("Speed"), S);
		EngineAudio->SetFloatParameter(TEXT("Turbo"), Drive.bHot ? 1.f : 0.f);
		EngineAudio->SetPitchMultiplier(0.75f + S * 0.9f + (Drive.bHot ? 0.12f : 0.f));
	}
	if (SkidAudio->IsPlaying()) SkidAudio->SetVolumeMultiplier(bSlide ? 0.7f : 0.f);
}

void AFTPlayerCar::BoostKick(float Scale)
{
	// FX overhaul (looks only): long flames out of the pipes and a ring round them, riding on the car
	FTFx::Attach(TEXT("NS_BoostKick"), Body, FVector(-232.f, 0.f, 30.f), FRotator::ZeroRotator, Scale, FTFx::Main, true);
	FTFx::Attach(TEXT("NS_BoostRing"), Body, FVector(-240.f, 0.f, 36.f), FRotator::ZeroRotator, Scale, FTFx::Extra | FTFx::Flash, true);
}

void AFTPlayerCar::HitFlash(float Amount)
{
	AlertAmt = FMath::Max(AlertAmt, FMath::Clamp(Amount, 0.f, 1.f));
}

void AFTPlayerCar::TickFx(float Dt, const FVector& Velocity)
{
	// FX overhaul. Looks only: nothing here moves the car or is read by the race.
	// The barrier: a hard hit throws a burst of sparks back from where it struck, over the scrape's stream
	WallHitCd -= Dt;
	if ((bWallLeft || bWallRight) && WallImpact > 300.f && WallHitCd <= 0.f && !bWrecked && FMath::Abs(Speed) > 1500.f)
	{
		WallHitCd = 0.5f;
		FTFx::Attach(TEXT("NS_RamSparks"), Body, FVector(60.f, bWallLeft ? -128.f : 128.f, 45.f), FRotator(10.f, 180.f, 0.f), 0.9f, FTFx::Extra, true);
	}
	// the turbo flame's hot core (not at Low)
	if (BoostCoreFX->GetAsset())
	{
		const bool bCore = Drive.bHot && !bWrecked && FTFx::Shows(this, FTFx::Extra);
		if (bCore && !BoostCoreFX->IsActive()) BoostCoreFX->Activate(true);
		if (!bCore && BoostCoreFX->IsActive()) BoostCoreFX->Deactivate();
	}
	// Tyre smoke and drift sparks stream back along the way the car is really going: a drifting car points 24 degrees
	// off its line, and both emitters throw along their own -X
	const bool bSlide = Drive.DriftDir != 0 && bGrounded;
	const FVector Flat = Velocity - RoadUp * FVector::DotProduct(Velocity, RoadUp);
	if (bSlide && Flat.SizeSquared() > FMath::Square(500.f))
	{
		const FRotator Way = FRotationMatrix::MakeFromXZ(Flat.GetSafeNormal(), RoadUp).Rotator();
		DriftFX->SetWorldRotation(Way);
		DriftSparkFX->SetWorldRotation(Way);
	}
	// The drift's charge had no look in the world (only the HUD said BLUE, ORANGE, PINK): sparks off the rear tyres
	// in its colour, and a ring as each charge is reached
	const int32 Charge = bSlide && !bWrecked ? FMath::Clamp(Drive.DriftLevel, 0, 3) : 0;
	if (Charge != SparkLevel)
	{
		static const TCHAR* Sparks[3] = { TEXT("NS_DriftSparks1"), TEXT("NS_DriftSparks2"), TEXT("NS_DriftSparks3") };
		static const TCHAR* Pop[3] = { TEXT("NS_DriftPop1"), TEXT("NS_DriftPop2"), TEXT("NS_DriftPop3") };
		if (Charge > 0)
		{
			UNiagaraSystem* Sys = FTFx::System(Sparks[Charge - 1]);
			if (DriftSparkFX->GetAsset() != Sys) DriftSparkFX->SetAsset(Sys);
			if (Sys) DriftSparkFX->Activate(true);
			if (Charge > SparkLevel) FTFx::Attach(Pop[Charge - 1], Body, FVector(-133.f, 0.f, 40.f), FRotator::ZeroRotator, 1.f, FTFx::Extra, true);
		}
		else if (DriftSparkFX->IsActive()) DriftSparkFX->Deactivate();
		SparkLevel = Charge;
	}
	// A landing: sparks off the belly and dust thrown out flat from under the car, by how long it flew. (On the
	// coaster's rails the wheels never touch anything: that is not a flight.)
	if (!bGrounded && !bRailInit && Half != EFTHalf::Grind) FxAirT += Dt;
	else if (FxAirT > 0.f)
	{
		if (FxAirT > 0.3f && bGrounded && !bWrecked && !Drive.bFrozen)
		{
			const float K = FMath::Clamp(0.65f + FxAirT * 0.45f, 0.75f, 1.5f);
			FTFx::Attach(TEXT("NS_LandSparks"), Body, FVector(-40.f, 0.f, 12.f), FRotator::ZeroRotator, K, FTFx::Main, true);
			FTFx::Attach(TEXT("NS_LandDust"), Body, FVector(0.f, 0.f, 14.f), FRotator::ZeroRotator, K, FTFx::Extra, true);
		}
		FxAirT = 0.f;
	}
	// A hit: the car's lights surge and fall back (the paint's Alert, which only its glowing parts answer to)
	if (AlertAmt > 0.f || AlertShown > 0.f)
	{
		AlertAmt = FMath::Max(0.f, AlertAmt - Dt * 5.f);
		if (FMath::Abs(AlertAmt - AlertShown) > 0.03f || AlertAmt <= 0.f)
		{
			AlertShown = AlertAmt;
			Body->SetScalarParameterValueOnMaterials(TEXT("Alert"), AlertShown);
			for (UStaticMeshComponent* P : Parts) if (P->GetAttachParent() == Body) P->SetScalarParameterValueOnMaterials(TEXT("Alert"), AlertShown);
		}
	}
}
void AFTPlayerCar::TickBody(float Dt)
{
	// 3.3 "the cars are so stiff": the body rides its springs over the wheels. Sideways acceleration rolls it to the
	// outside of a bend, speeding up squats it, braking dives it, and a landing bounces it. Looks only: the physics
	// chassis (and the gameplay) is exactly as before.
	if (Dt <= 0.f) return;
	const FVector Vn = Chassis->GetPhysicsLinearVelocity();
	const bool bKinematic = bRailInit || Half != EFTHalf::None || Drive.bFrozen || bWrecked;
	FVector ALocal = FVector::ZeroVector;
	if (bPrevVel && !bKinematic)
	{
		const FVector A = (Vn - PrevVel) / Dt;
		ALocal = Chassis->GetComponentTransform().InverseTransformVectorNoScale(A);
		// a teleport (put back, rescue, a retake) is not an acceleration
		if (ALocal.Size() > 30000.f) ALocal = FVector::ZeroVector;
	}
	PrevVel = Vn; bPrevVel = true;
	AccF = FMath::VInterpTo(AccF, ALocal, Dt, 9.f);
	const float G = 980.f;
	const float RollT = FMath::Clamp(-AccF.Y / G * LeanRollPerG, -5.5f, 5.5f);
	const float PitchT = FMath::Clamp(AccF.X / G * LeanPitchPerG, -3.5f, 3.5f);
	// heave: the springs compress under a hard push down (a landing, the bottom of a dip) and the body sinks
	const float HeaveT = FMath::Clamp(-(AccF.Z) / G * 5.f, -10.f, 6.f);
	const float W = BodyFreq, Z = BodyDamp;
	auto Spring = [&](float& X, float& V, float Target)
	{
		const float Acc = W * W * (Target - X) - 2.f * Z * W * V;
		V += Acc * Dt; X += V * Dt;
		if (!FMath::IsFinite(X)) { X = 0.f; V = 0.f; }
	};
	Spring(BodyRoll, BodyRollV, RollT);
	Spring(BodyPitch, BodyPitchV, PitchT);
	Spring(BodyHeave, BodyHeaveV, HeaveT);
	// the body pivots about its middle at axle height, so a roll doesn't swing it off the wheels
	const FVector Pivot(0.f, 0.f, 40.f);
	const FRotator Rot(BodyPitch, 0.f, BodyRoll);
	const FVector Off = Pivot - Rot.RotateVector(Pivot);
	Body->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -ChassisZ + BodyHeave) + Off, Rot);
}

bool AFTPlayerCar::TickHalfPipe(float Dt, const FFTFrame& F, float Slope)
{
	// 3.3 half pipe. The owner: "the half pipe should allow the player car to do jumps and tricks and stuff, you can
	// grind if you just gently drive up to the edge". Up a wall at speed, the car leaves the lip and flies straight up
	// (and keeps rolling down the pipe), you spin it with steer and barrel-roll it with drift, and it comes back down
	// onto the same wall: land square (within 50 degrees) or bail. Drive up gently instead and it locks onto the
	// coping and grinds along it until you steer back down. Both are set on a path, like the coaster rails: physics
	// takes the car back the moment it lands or drops in.
	const bool bHalf = (F.Flags & FTFlag::HalfPipe) != 0;
	const float EdgeX = FT::Edge - 150.f;   // the car's centre on the coping
	if (Half == EFTHalf::None)
	{
		if (!bHalf || bWrecked || LaunchT > 0.f || !bGrounded) return false;
		const float Side = X > 0.f ? 1.f : -1.f;
		const float Out = LatSpeed * Side;   // up the wall
		if (FMath::Abs(X) < FT::Edge - 300.f || Track->SurfaceH(D, Side * EdgeX) < 300.f) return false;
		HalfSide = (int32)Side; HalfT = 0.f; TrickYaw = TrickRoll = 0.f; HalfPeak = 0.f;
		HalfSpeed = FMath::Max(FVector::DotProduct(Chassis->GetPhysicsLinearVelocity(), F.Fwd), FT::BaseSpeed * 0.8f);
		float Sl = 0.f; const float Hs = Track->SurfaceH(D, X, &Sl);
		HalfX0 = X; HalfH0 = Hs;
		HalfTilt0 = FMath::RadiansToDegrees(FMath::Atan(FMath::Abs(Sl)));
		if (Out > 1250.f)
		{
			// launch: the faster up the wall, the bigger the air (1.5 .. 2.6 s)
			Half = EFTHalf::Air; bHalfLaunched = true;
			HalfVUp = FMath::Clamp(Out * 0.9f + 700.f, 1500.f, 2550.f);
			HalfDur = 2.f * HalfVUp / 1960.f;
			return true;
		}
		if (Out > -200.f && Drive.Steer * Side > -0.1f)
		{
			Half = EFTHalf::Grind; bGrindStart = true;
			return true;
		}
		return false;
	}
	if (bWrecked || Drive.bFrozen) { Half = EFTHalf::None; return false; }
	const float Side = (float)HalfSide;
	HalfT += Dt;
	HalfSpeed = FMath::FInterpTo(HalfSpeed, FMath::Max(Drive.TargetSpeed, FT::BaseSpeed * 0.8f), Dt, 0.4f);
	const float Dn = D + HalfSpeed * Dt;
	const FFTFrame Fn = Track->Sample(Dn);
	if (Half == EFTHalf::Air)
	{
		// up and down over the lip; drifts in a little so it lands on the curve, not the coping
		const float Tn = FMath::Min(HalfT, HalfDur);
		const float Lift = HalfVUp * Tn - 0.5f * 1960.f * Tn * Tn;
		HalfPeak = FMath::Max(HalfPeak, Lift);
		const float Xn = FMath::Lerp(HalfX0, Side * (FT::Edge - 420.f), FMath::Clamp(Tn / HalfDur, 0.f, 1.f));
		const float Hn = Track->SurfaceH(Dn, Xn) + ChassisZ + FMath::Max(Lift, 0.f);
		// tricks: steer spins it, drift rolls it (keeps turning a little after you let go, then holds)
		TrickYaw += Drive.Steer * 540.f * Dt;
		if (Drive.bFlip) TrickRoll += -Side * 600.f * Dt;
		// the wall's tilt eases to level at the top and back for the landing
		const float U = FMath::Clamp(Tn / HalfDur, 0.f, 1.f);
		const float Tilt = HalfTilt0 * FMath::Abs(FMath::Cos(PI * U)) * Side;
		const FTransform Base = Track->TransformAt(Dn, Xn, Hn);
		const FQuat Q = Base.GetRotation() * FQuat(FVector::ForwardVector, FMath::DegreesToRadians(Tilt + TrickRoll)) * FQuat(FVector::UpVector, FMath::DegreesToRadians(TrickYaw));
		Chassis->SetWorldTransform(FTransform(Q, Base.GetLocation()), false, nullptr, ETeleportType::TeleportPhysics);
		Chassis->SetPhysicsLinearVelocity(Fn.Fwd * HalfSpeed);
		Chassis->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);
		LaunchT = FMath::Max(LaunchT, 0.3f);
		if (HalfT >= HalfDur || !(Fn.Flags & FTFlag::HalfPipe))
		{
			// down: square within 50 degrees of a whole turn on both axes, or a bail
			auto Off = [](float Deg) { const float M = FMath::Fmod(FMath::Abs(Deg), 360.f); return FMath::Min(M, 360.f - M); };
			bHalfBailed = Off(TrickYaw) > 50.f || Off(TrickRoll) > 50.f;
			bHalfLanded = true;
			Half = EFTHalf::None;
			const FTransform L = Track->TransformAt(Dn, Xn, Track->SurfaceH(Dn, Xn) + ChassisZ + 4.f);
			Chassis->SetWorldTransform(FTransform(L.GetRotation() * FQuat(FVector::ForwardVector, FMath::DegreesToRadians(HalfTilt0 * Side)), L.GetLocation()), false, nullptr, ETeleportType::TeleportPhysics);
			Chassis->SetPhysicsLinearVelocity(Fn.Fwd * HalfSpeed - Fn.Right * Side * 700.f);
			LaunchT = 0.f;
		}
		return true;
	}
	// the grind: on the coping, leaning on it, sparks; steer back down (or the pipe runs out) to drop in
	const float Hc = Track->SurfaceH(Dn, Side * EdgeX);
	const bool bOff = Drive.Steer * Side < -0.35f || !(Fn.Flags & FTFlag::HalfPipe) || Hc < 250.f;
	float Sl = 0.f; Track->SurfaceH(Dn, Side * EdgeX, &Sl);
	const float TiltG = FMath::RadiansToDegrees(FMath::Atan(FMath::Abs(Sl))) * 0.85f * Side;
	const FTransform Base = Track->TransformAt(Dn, Side * EdgeX, Hc + ChassisZ - 6.f);
	Chassis->SetWorldTransform(FTransform(Base.GetRotation() * FQuat(FVector::ForwardVector, FMath::DegreesToRadians(TiltG)), Base.GetLocation()), false, nullptr, ETeleportType::TeleportPhysics);
	Chassis->SetPhysicsLinearVelocity(Fn.Fwd * HalfSpeed);
	Chassis->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);
	if (SparksFX->GetAsset())
	{
		SparksFX->SetRelativeLocation(FVector(-60.f, Side * 130.f, 20.f));
		if (!SparksFX->IsActive()) SparksFX->Activate(true);
	}
	if (bOff)
	{
		Half = EFTHalf::None; bGrindEnd = true;
		Chassis->SetPhysicsLinearVelocity(Fn.Fwd * HalfSpeed - Fn.Right * Side * 900.f);
		if (SparksFX->IsActive()) SparksFX->Deactivate();
	}
	(void)Slope;
	return true;
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
		if (!bRailInit)
		{
			int32 Bi = 0;
			for (int32 i = 1; i < FFTRails::Num; ++i) if (FMath::Abs(X - R.X[i]) < FMath::Abs(X - R.X[Bi])) Bi = i;
			RailPt = R.P(Bi); bRailInit = true;
		}
		RailPt = R.P(R.Nearest(RailPt));
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

	// 3.6 (owner: "a part of the track in level 2 where my car hit and slowed it down a lot"): every gap jump's landing
	// took 35-45% of the car's speed in one hit (Chemical Works' pond and Mega Vat jumps: ~7,200 -> ~4,100 cm/s, under
	// cruising speed, FTBUMP). Touching down after a jump or a ramp launch it now keeps the speed along the road it flew
	// with, easing back to the governor's speed over about a second (no engine braking meanwhile, like a set piece).
	{
		const float VRoadL = FVector::DotProduct(Chassis->GetPhysicsLinearVelocity(), F.Fwd);
		const bool bJumpAir = !bGrounded && ((F.Flags & (FTFlag::Kicker | FTFlag::Gap | FTFlag::Landing)) || LaunchT > 0.f);
		if (bJumpAir) { bLandArmed = true; LandAirV = VRoadL; }
		else if (bGrounded && bLandArmed)
		{
			bLandArmed = false;
			LandFloor = FMath::Min(LandAirV, 11000.f) * 0.97f;
			LandHoldT = LandFloor > Drive.TargetSpeed + 100.f ? 1.5f : 0.f;
			if (LandHoldT > 0.f) UE_LOG(LogFT, Display, TEXT("FTLAND D=%.0f air=%.0f hit=%.0f floor=%.0f target=%.0f"), D, LandAirV, VRoadL, LandFloor, Drive.TargetSpeed);
		}
		else if (bGrounded) bLandArmed = false;
		if (LandHoldT > 0.f)
		{
			LandHoldT -= Dt;
			LandFloor = FMath::FInterpTo(LandFloor, Drive.TargetSpeed, Dt, 2.f);
			if (bWrecked || LandFloor <= Drive.TargetSpeed + 50.f) LandHoldT = 0.f;
			else if (VRoadL < LandFloor) Chassis->SetPhysicsLinearVelocity(Chassis->GetPhysicsLinearVelocity() + F.Fwd * (LandFloor - VRoadL));
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
		// (3.3: 7,000 cm/s across, so the corkscrewing centre rail can't leave its car behind)
		const float MaxStep = 7000.f * Dt;
		const float NX = X + FMath::Clamp(TX - X, -MaxStep, MaxStep);
		const float NH = Hr + SurfH; // current height over the frame
		const float WantH = TH + ChassisZ - 2.f;
		const float NewH = NH + FMath::Clamp(WantH - NH, -MaxStep, MaxStep);
		// how the rail itself moves across and up as the car runs along it
		const FFTRails R2 = Track->RailsAt(D + 200.f);
		const FVector2D P2 = R2.P(R2.Nearest(RailPt));
		const float VRoadC = FMath::Max(FVector::DotProduct(Chassis->GetPhysicsLinearVelocity(), F.Fwd), bInSet ? SetFloor : 0.f);
		const float VX = HopT >= 0.f ? 0.f : (P2.X - RailX) / 200.f * VRoadC;
		const float VH = HopT >= 0.f ? 0.f : (P2.Y - SurfH) / 200.f * VRoadC;
		Chassis->SetWorldTransform(Track->TransformAt(D, NX, NewH), false, nullptr, ETeleportType::TeleportPhysics);
		Chassis->SetPhysicsLinearVelocity(F.Fwd * VRoadC + F.Right * VX + F.Up * VH);
		Chassis->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);
		X = NX;
	}
	else HopT = -1.f;

	// 3.3 half pipe: air off the lip with tricks, or a grind along the coping (the car rides a set path meanwhile)
	if (!bCoaster && TickHalfPipe(Dt, F, Slope))
	{
		Vehicle->SetThrottleInput(0.3f);
		Vehicle->SetBrakeInput(0.f);
		Vehicle->SetSteeringInput(0.f);
		return;
	}

	// 2.5 corkscrews: the road rolls round its centreline, so off-centre the deck sweeps through a car that isn't
	// rolling with it (3.3 rad/s at turbo speed) and the depenetration threw it off, or under the deck. Held near
	// the centreline, and its roll turned with the road's.
	// (The roll is matched on all road, not only corkscrews: where a 57-degree banked wall unwinds to flat in 25 m,
	// the Chemical Works road rolls 2 rad/s under a car at its edge, and its nose planted into the deck.)
	if (!bCoaster && !bWrecked && !bRealJump && (bGrounded || (F.Flags & FTFlag::SetPiece)))
	{
		const FVector Vc = Chassis->GetPhysicsLinearVelocity();
		const FFTFrame F2 = Track->Sample(D + 100.f);
		const float DRoll = FMath::Atan2(FVector::DotProduct(FVector::CrossProduct(F.Up, F2.Up), F.Fwd), FVector::DotProduct(F.Up, F2.Up));
		const float RollRate = DRoll / 100.f * FVector::DotProduct(Vc, F.Fwd);
		if (F.Flags & FTFlag::Twist)
		{
			const float VLatR = FVector::DotProduct(Vc, F.Right);
			Chassis->AddForce(F.Right * FMath::Clamp(-X * 22.f - VLatR * 9.f, -12000.f, 12000.f), NAME_None, true);
			// 3.6: and the car rides with the deck's sweep. Off the centreline a rolling deck moves along its own normal
			// (roll rate x distance out), so a car driving straight was struck by it (Level 2's second corkscrew, entered at
			// turbo speed: ~8,200 -> ~2,700 cm/s for a frame in every run) or left hanging. Near the surface, its speed along
			// the normal is the deck's own there. (Tried in the fast pipes too: no clear gain, left out.)
			if (Lift < 80.f)
			{
				const FVector Sweep = (F2.Pos + F2.Right * X) - (F.Pos + F.Right * X);
				const float VUp = FVector::DotProduct(Sweep, F.Up) / 100.f * FMath::Max(FVector::DotProduct(Vc, F.Fwd), 0.f);
				if (FMath::Abs(VUp) > 300.f)
					Chassis->SetPhysicsLinearVelocity(Chassis->GetPhysicsLinearVelocity() + F.Up * (VUp - FVector::DotProduct(Vc, F.Up)));
			}
		}
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
	Vehicle->SetBrakeInput(Gap < -1500.f && VAlong > 800.f && !bInSet && LandHoldT <= 0.f ? 0.4f : 0.f);
	Vehicle->SetSteeringInput(Drive.Steer * (Drive.DriftDir != 0 ? 0.25f : 0.55f));

	if (bGrounded)
	{
		// 2.6 (spec 05 §1): speed moves toward the target at the stated rate (420 / 900 su/s, by the Gearbox), both
		// ways; a stiff gain so it reaches the target instead of settling under it
		const float A = FMath::Clamp(Gap * 8.f, (bInSet || LandHoldT > 0.f) ? 0.f : -Drive.Accel, Drive.Accel);
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
