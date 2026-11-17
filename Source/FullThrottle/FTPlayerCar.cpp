#include "FTPlayerCar.h"
#include "FTTrack.h"
#include "FTCore.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "UObject/UObjectIterator.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Engine/StaticMesh.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "HAL/IConsoleManager.h"
#include "FTFx.h"
#include "NiagaraSystem.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "EngineUtils.h"

// which arcade assists run on top of Chaos (bits): 1 speed governor, 2 grip, 4 yaw, 8 anti-flip, 16 track gravity, 32 air control,
// 64 road glue (2.4 downforce)
static TAutoConsoleVariable<int32> CVarFTArcade(TEXT("ft.Arcade"), 127, TEXT("FULL THROTTLE: arcade assist bitmask on the Chaos car"));
static TAutoConsoleVariable<int32> CVarFTPipeSweep(TEXT("ft.PipeSweep"), 1,
	TEXT("4.5: 1 = in a pipe (as in a twist since 3.6) the car rides with the deck's sweep along its normal near the surface, and a put-back there leaves with the deck's own motion. 0 = 4.4 (Level 7's fast pipe put the car back 32 times in 1.5 s)."));
static TAutoConsoleVariable<int32> CVarFTBumpStop(TEXT("ft.BumpStop"), 1,
	TEXT("4.5: 1 = a bump stop under the springs: the chassis is never let sink more than BumpStopLift under ride height (its speed into the road is taken out, and it is eased back up), so a dip, a crest or a landing at speed can't put the car on its belly with its wheels reading 'in the air'. 0 = 4.4."));
static TAutoConsoleVariable<int32> CVarFTWallGlance(TEXT("ft.WallGlance"), 1,
	TEXT("4.6: 1 = a glancing barrier hit (nose not pointed into it) keeps the car's speed along the road: 2% at the hit and a light drag while it scrapes, eased to the governor's speed. 0 = 4.5 (25-35% lost)."));
static TAutoConsoleVariable<int32> CVarFTWallUnstick(TEXT("ft.WallUnstick"), 1,
	TEXT("4.6: 1 = a car pinned nose-first into a barrier at low speed is turned back along the road and pushed off it within ~0.3 s (throttle off meanwhile). 0 = 4.5 (stuck until the 1.2 s crew put-back)."));
static TAutoConsoleVariable<int32> CVarFTPipeHold(TEXT("ft.PipeHold"), 1,
	TEXT("4.6: 1 = in a pipe the car is carried round with the road's roll (the sideways pull toward the middle a rolling road needs, fed forward), so it holds its lane instead of being slung into the barrier. 0 = 4.5."));
#if !UE_BUILD_SHIPPING
// 4.6 tests: park the car at the barrier with its nose turned into it, crawling, and time how long it takes to get away
static FAutoConsoleCommandWithWorldAndArgs GFTNosePin(TEXT("ft.NosePin"),
	TEXT("FULL THROTTLE 4.6 tests: ft.NosePin <deg> [side -1|1]: the car parked at the barrier 4 m on, nose turned <deg> into it, crawling; FTNOSEPIN logs when it is away (over 2,500 cm/s)"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& A, UWorld* W)
	{
		for (TActorIterator<AFTPlayerCar> It(W); It; ++It) It->NosePin(A.Num() ? FCString::Atof(*A[0]) : 60.f, A.Num() > 1 ? FCString::Atoi(*A[1]) : 1);
	}));
static TAutoConsoleVariable<int32> CVarFTPipeTrace(TEXT("ft.PipeTrace"), 0, TEXT("FULL THROTTLE 4.8 tests: 1 = log FTPIPE every frame the car is in a pipe"));
static FAutoConsoleCommandWithWorldAndArgs GFTWarp(TEXT("ft.Warp"),
	TEXT("FULL THROTTLE 4.8 tests: ft.Warp <D cm> [X cm] [speed cm/s]: put the car on the track there (FTWARP log)"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& A, UWorld* W)
	{
		if (A.Num() < 1) return;
		const float Dw = FCString::Atof(*A[0]), Xw = A.Num() > 1 ? FCString::Atof(*A[1]) : 0.f, Sw = A.Num() > 2 ? FCString::Atof(*A[2]) : 0.f;
		for (TActorIterator<AFTPlayerCar> It(W); It; ++It) { It->PlaceOnTrack(Dw, Xw, Sw); UE_LOG(LogFT, Display, TEXT("FTWARP D=%.0f X=%.0f v=%.0f"), Dw, Xw, Sw); }
	}));
#endif
static TAutoConsoleVariable<int32> CVarFTLandKeep(TEXT("ft.LandKeep"), 1,
	TEXT("4.6: 1 = a jump's touchdown keeps its flight speed through the impact also when it lands on its belly (wheels reading in the air) and when the turbo is on (flight speed under the governor's target: held 0.3 s, no gain). 0 = 4.5."));
static TAutoConsoleVariable<int32> CVarFTLandCushion(TEXT("ft.LandCushion"), 1,
	TEXT("4.7: 1 = in the last ~2 frames before a jump's touchdown the speed into the deck is cut to 600 cm/s and the car is squared to the road and pointed along it (if within 35 degrees), so a long landing doesn't slam the chassis into the deck and spin. 0 = 4.6."));
static TAutoConsoleVariable<int32> CVarFTPipeHead(TEXT("ft.PipeHead"), 1,
	TEXT("4.7: 1 = in a pipe the car's nose (yaw assist, air control, autopilot) follows the road surface's own direction under it, not the tube's axis, and the sideways feed-forward is capped at 60,000 instead of 20,000 cm/s^2. 0 = 4.6 (crabbed 20-30 degrees at turbo speed in Level 7's fast pipe, slid to the barrier, spun)."));
static TAutoConsoleVariable<int32> CVarFTBodySlide(TEXT("ft.BodySlide"), 1,
	TEXT("4.8: 1 = the chassis box gets its own low-friction, no-bounce physical material (friction ft.BodyFriction, combine Min; restitution 0, combine Min), so a barrier scrape or a belly touchdown slides instead of dragging. 0 = 4.7 (the PM_CarBody asset never existed: engine default friction 0.7, restitution 0.3)."));
static TAutoConsoleVariable<float> CVarFTBodyFriction(TEXT("ft.BodyFriction"), 0.1f,
	TEXT("4.8: the chassis friction ft.BodySlide gives it (combine Min, so the lower of this and what it touches)."));
static TAutoConsoleVariable<int32> CVarFTPhysSubsteps(TEXT("ft.PhysSubsteps"), 16,
	TEXT("4.8: most physics substeps (8.3 ms each) in one frame. With 8 (4.7, the project setting) a frame slower than 15 fps ran the car's physics slower than the game clock (72% of real time at 11 fps: the car lost ground to the world and its assists). 16 keeps it real time down to 7.5 fps. 0 = leave the project setting."));
static TAutoConsoleVariable<int32> CVarFTLowFpsGlue(TEXT("ft.LowFpsGlue"), 63,
	TEXT("4.9: frame-rate-aware road hold (bits). The assists are applied once a frame and held for all its physics substeps, so at 12 fps (8 m a frame at turbo speed) a pull worked out for where the car was went on acting 8 m later. 1 = the crest pull follows the surface's curve over the distance the frame covers, not at its start; 2 = the glue spring never pulls harder than what brings the car to ride height by the frame's end; 4 = the bump stop never pushes it back past the stop in one frame; 8 = the snap back down is 35 cm per 60th of a second, not per frame; 16 = the crest pull works up to 100 cm per 60th of a second of lift; 32 = the glue judges closing speed against the road under the stretch the frame covers. 0 = 4.8."));
static TAutoConsoleVariable<int32> CVarFTSetCeil(TEXT("ft.SetCeil"), 1,
	TEXT("4.9: 1 = in a set piece (loop, corkscrew, pipe, coaster) the car is never faster along the road than 1.1 x the most of its way-in speed, the governor's target and the set floor, + 300 cm/s. Level 7's fast pipe fed the deck-sweep assist's speed back into forward speed: 31,592 cm/s. 0 = 4.8."));
static TAutoConsoleVariable<float> CVarFTPipeEdge(TEXT("ft.PipeEdge"), 850.f,
	TEXT("4.9: in a pipe (not a loop or a corkscrew) the lane ends this far from the middle (cm, at most 900): past it the car's outward speed against the surface is taken out and a stiff spring holds it; 2 m further it is put back. Level 7's fast pipe swept a car near its barrier into the deck at turbo speed and spun it. 0 = 4.8."));
#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable<int32> CVarFTHitTrace(TEXT("ft.HitTrace"), 0, TEXT("FULL THROTTLE 5.2 tests: 1 = log FTHIT for every hit on the track (normal and impulse in the road's frame, the car's speed and yaw rate) and FTTW every frame in a corkscrew (each sideways push of the frame); 2 = FTTW in pipes too"));
static TAutoConsoleVariable<int32> CVarFTGlueTrace(TEXT("ft.GlueTrace"), 0, TEXT("FULL THROTTLE 4.9 tests: 1 = log FTGT every frame on ordinary road (lift, speed off the road, wheels, suspension, the glue/crest/bump-stop terms)"));
#endif
static TAutoConsoleVariable<int32> CVarFTPipeKick(TEXT("ft.PipeKick"), 1,
	TEXT("5.0: 1 = in a pipe, for 0.15 s after the chassis strikes the deck (not drifting), the car's yaw rate is held within 1 rad/s of turning it back onto the surface's path (Level 7's fast pipe at 24 fps: the swept deck struck the chassis and kicked it 6-8 rad/s round: herr 38-55 deg, a spin). Only below 40 fps. 0 = 4.9."));
static TAutoConsoleVariable<int32> CVarFTWheelBarrier(TEXT("ft.WheelBarrier"), 1,
	TEXT("5.0: 1 = on ordinary road beside the barrier (|x| > 11.5 m), while the chassis is more than 20 cm over ride height, the wheels' suspension traces ignore static geometry (the road is out of their reach then: they reach 14 cm down), their contacts don't count as wheels down, and a body over the barrier line is eased back in at 3 m/s. The rear wheels sit 12 cm wider than the body box, so their traces landed on the 160 cm barrier top and the springs held the car up there (the 12 fps road flights in the outer lanes, lift 146-711 cm with wheels 'down'). 0 = 4.9."));
static TAutoConsoleVariable<int32> CVarFTLandAim(TEXT("ft.LandAim"), 7,
	TEXT("5.1 (bits): 1 = over a gap jump the car's arc is bent down (never up, at most ft.LandAimMax) so it meets the road by the landing zone's end instead of flying past it 3-17 m up; 2 = a car still in the air past the landing zone stays in its jump (no glue yank, no snap-down) until it touches down. 4 = late aim (5.2): the car climbs on air gravity alone and the pull starts only once the full pull is just enough to meet the zone (rise ~5.1-5.6 m at 60 fps instead of ~4.2-4.7, same landing). 3 = 5.1. 0 = 5.0 (past the zone the glue pulled it down at 12 g: the 'road drops' on Levels 1, 7 and 8)."));
static TAutoConsoleVariable<float> CVarFTLandAimMax(TEXT("ft.LandAimMax"), 3000.f,
	TEXT("5.1: the most extra downward pull ft.LandAim adds over air gravity (cm/s^2). 1,500 left turbo-speed jumps (~9,500 cm/s) 7-10 m up past the zone; 3,000 lands them in it (jumps rise ~4 m instead of ~6.5 m)."));
static TAutoConsoleVariable<int32> CVarFTCrestFollow(TEXT("ft.CrestFollow"), 1,
	TEXT("4.6: 1 = where the surface curves away from the car (crests, a pipe's inner wall) the car is pulled with the curve (v^2 x curvature), so it doesn't float over it on the glue spring. 0 = 4.5."));

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
	// FX round 4: the marks the body wears as its armour drops, in the order they come on (Below: the armour under which
	// each shows). Dents, a cracked lamp, soot: what ft_car.py models on the damaged car, here as flat planes on the same
	// panels, 1 to 4 cm proud of them. Places are in the body's own space: the model's (ft_car.py), 215 cm further back
	// (ft_export.py), so the origin is under the middle of the car at road level. Kind: 0 dent, 1 cracked glass, 2 soot.
	// Size is the plane's: across and up for a mark on the tail panel, along and across the car for one that lies flat
	struct FCarMark { float Below; int32 Kind; FVector At; FVector Normal; FVector2D Size; float Seed; };
	const FCarMark CarMarks[] = {
		{ 82.f, 0, FVector(-220.8f, 0.f, 66.f), FVector(-1.f, 0.f, 0.f), FVector2D(44.f, 20.f), 1.f },             // a dent in the tail panel, over the plate
		{ 70.f, 1, FVector(-221.f, -84.f, 62.f), FVector(-1.f, 0.f, 0.f), FVector2D(30.f, 26.f), 2.f },            // the left outer tail lamp cracked
		{ 58.f, 2, FVector(-179.f, 62.f, 88.8f), FVector(0.f, 0.f, 1.f), FVector2D(46.f, 40.f), 3.f },             // soot on the rear deck, right
		{ 46.f, 0, FVector(-189.f, -70.f, 88.5f), FVector(0.f, 0.f, 1.f), FVector2D(30.f, 40.f), 4.f },            // the deck's left corner crumpled
		{ 35.f, 1, FVector(-221.f, 63.f, 62.f), FVector(-1.f, 0.f, 0.f), FVector2D(28.f, 24.f), 5.f },             // a right-hand tail lamp cracked
		{ 35.f, 0, FVector(-220.8f, -62.f, 42.f), FVector(-1.f, 0.f, 0.f), FVector2D(46.f, 14.f), 6.f },           // a dent low in the tail panel, left
		{ 24.f, 2, FVector(-69.f, -18.f, 123.3f), FVector(0.f, 0.f, 1.f), FVector2D(40.f, 44.f), 7.f },            // soot on the roof
		{ 24.f, 1, FVector(-128.6f, -58.f, 107.3f), FVector(-0.365f, 0.f, 0.931f), FVector2D(28.f, 28.f), 8.f },   // the rear screen cracked, beside the louvres
	};
	const int32 NumCarMarks = UE_ARRAY_COUNT(CarMarks);
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
	// FX round 3: what the rear wheels throw off the road (the systems are the level's: SetLightsForLevel), the banded
	// turbo flame and the exhaust at top speed, and the speed lines (those two are put by the camera each frame, in
	// TickFx). They sort with the blasts' smoke and fire, under every danger cue (which are left at 0)
	WheelFX = FX(TEXT("WheelFX"), FVector(-133, 0, 8));
	WheelFX2 = FX(TEXT("WheelFX2"), FVector(-133, 0, 8));
	TurboCelFX = FX(TEXT("TurboCelFX"), FVector(-232, 0, 30));
	ExhaustFX = FX(TEXT("ExhaustFX"), FVector(-232, 0, 30));
	SpeedFXL = FX(TEXT("SpeedFXL"), FVector(0, 0, 0));
	SpeedFXR = FX(TEXT("SpeedFXR"), FVector(0, 0, 0));
	WheelFX->SetTranslucentSortPriority(-30);
	WheelFX2->SetTranslucentSortPriority(-30);
	TurboCelFX->SetTranslucentSortPriority(-20);
	ExhaustFX->SetTranslucentSortPriority(-20);
	SpeedFXL->SetUsingAbsoluteLocation(true); SpeedFXL->SetUsingAbsoluteRotation(true); SpeedFXL->SetUsingAbsoluteScale(true);
	SpeedFXR->SetUsingAbsoluteLocation(true); SpeedFXR->SetUsingAbsoluteRotation(true); SpeedFXR->SetUsingAbsoluteScale(true);
	SpeedFXL->SetTranslucentSortPriority(-5);
	SpeedFXR->SetTranslucentSortPriority(-5);

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
	// FX round 3 (the wheels' systems are the level's, picked in SetLightsForLevel; the banded turbo flame by the tuft, in TickFx)
	SetFX(ExhaustFX, TEXT("/Game/FX/NS_ExhaustFlame"));
	SetFX(SpeedFXL, TEXT("/Game/FX/NS_SpeedLines"));
	SetFX(SpeedFXR, TEXT("/Game/FX/NS_SpeedLines"));
	if (!EngineAudio->Sound) EngineAudio->SetSound(Load<USoundBase>(TEXT("/Game/Audio/SFX/S_EngineLoop")));
	if (!SkidAudio->Sound) SkidAudio->SetSound(Load<USoundBase>(TEXT("/Game/Audio/SFX/S_Skid")));
	// 4.8: /Game/Core/Physics/PM_CarBody was loaded here but never existed (the body ran on the engine default, friction
	// 0.7 and restitution 0.3): the body's material is now made in code (ApplyBodySlide, also re-run when ft.BodySlide changes)
	ApplyBodySlide();
	// the suspension traces must never find the car's own chassis (a Vehicle-channel body)
	Vehicle->SetWheelTraceResponseToChannel(ECC_Vehicle, ECR_Ignore);
	Vehicle->SetWheelTraceResponseToChannel(ECC_Pawn, ECR_Ignore);
	Chassis->OnComponentHit.AddDynamic(this, &AFTPlayerCar::OnBodyHit);
	// FX round 4: the marks the body wears as its armour drops (SetDamageLook shows them): planes on the body's own
	// panels, drawn with the generator's M_FX_CarMark. A build without that material has none. They sort under the
	// smoke and the fire (-30, -20), so what burns on the car is drawn over them
	if (DamageMarks.Num() == 0)
	{
		UStaticMesh* PlaneMesh = Load<UStaticMesh>(TEXT("/Engine/BasicShapes/Plane"));
		static const TCHAR* MarkMats[3] = { TEXT("/Game/FX/MI_FX_CarDent"), TEXT("/Game/FX/MI_FX_CarCrack"), TEXT("/Game/FX/MI_FX_CarSoot") };
		for (int32 i = 0; i < NumCarMarks; ++i)
		{
			const FCarMark& M = CarMarks[i];
			UMaterialInterface* Mat = Load<UMaterialInterface>(MarkMats[FMath::Clamp(M.Kind, 0, 2)]);
			if (!PlaneMesh || !Mat) break;
			UStaticMeshComponent* C = NewObject<UStaticMeshComponent>(this);
			C->SetupAttachment(Body);
			C->SetStaticMesh(PlaneMesh);
			UMaterialInstanceDynamic* Mid = UMaterialInstanceDynamic::Create(Mat, C);
			if (Mid) Mid->SetScalarParameterValue(TEXT("Seed"), M.Seed);
			C->SetMaterial(0, Mid ? static_cast<UMaterialInterface*>(Mid) : Mat);
			C->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			C->SetGenerateOverlapEvents(false);
			C->SetCastShadow(false);
			C->bAffectDistanceFieldLighting = false;
			C->bAffectDynamicIndirectLighting = false;
			C->bReceivesDecals = false;
			C->SetTranslucentSortPriority(-35);
			// the plane's face (+Z) along the panel's normal; its X across the car for an upright mark, along it for a flat one
			C->SetRelativeLocationAndRotation(M.At, FRotationMatrix::MakeFromZX(M.Normal, FMath::Abs(M.Normal.Z) > 0.99 ? FVector(1., 0., 0.) : FVector(0., 1., 0.)).Rotator());
			C->SetRelativeScale3D(FVector(M.Size.X * 0.01, M.Size.Y * 0.01, 1.));
			C->SetVisibility(false);
			C->RegisterComponent();
			AddInstanceComponent(C);
			DamageMarks.Add(C);
		}
	}
	Super::BeginPlay();
	if (EngineAudio->Sound) EngineAudio->Play();
	if (SkidAudio->Sound) { SkidAudio->SetVolumeMultiplier(0.f); SkidAudio->Play(); }
}

static TAutoConsoleVariable<float> CVarFTHeadDay(TEXT("ft.HeadDay"), 0.f,
	TEXT("4.1: the headlights' strength on a level lit by a sun (the brightest directional light >= ft.HeadDayLux), as a share of the night strength. Their pool lands 1-2 m ahead of the nose, so from the chase camera it sat behind the spoiler as a pale smudge in broad daylight. 4.2: 0 = off in daylight (at 4.1's 0.1 a faint smudge stayed); at 0 the two spot lights are hidden, so they cost nothing. 1 = 4.0."));
static TAutoConsoleVariable<float> CVarFTHeadDayLux(TEXT("ft.HeadDayLux"), 6.5f,
	TEXT("4.1: a directional light this bright (lux) or brighter makes a level a day level for the headlights (the suns are 7-10 lux, Bayshore's low sun 7; the moons 4-5.5)."));

void AFTPlayerCar::ApplyHeadlights()
{
	UWorld* W = GetWorld();
	if (!W || !HeadL || !HeadR) return;
	float Sun = 0.f; int32 NumSun = 0;
	for (TObjectIterator<UDirectionalLightComponent> It; It; ++It)
		if (It->GetWorld() == W && It->IsRegistered() && It->IsVisible()) { Sun = FMath::Max(Sun, It->Intensity); ++NumSun; }
	const bool bDay = Sun >= CVarFTHeadDayLux.GetValueOnGameThread();
	const float I = 2600.f * (bDay ? FMath::Clamp(CVarFTHeadDay.GetValueOnGameThread(), 0.f, 1.f) : 1.f);
	// 4.2: off = hidden (out of the light pass), unless the car is wrecked (Wreck() hides them, PlaceOnTrack shows them)
	const bool bShow = I > 0.f && (!Underglow || Underglow->IsVisible());
	if (HeadL->IsVisible() != bShow) { HeadL->SetVisibility(bShow); HeadR->SetVisibility(bShow); }
	if (!FMath::IsNearlyEqual(HeadL->Intensity, I))
	{
		HeadL->SetIntensity(I); HeadR->SetIntensity(I);
		UE_LOG(LogFT, Display, TEXT("FTHEAD suns=%d sun=%.1f lux day=%d headlights=%.0f cd"), NumSun, Sun, bDay ? 1 : 0, I);
	}
}

void AFTPlayerCar::SetLightsForLevel(int32 Level)
{
	Underglow->SetLightColor(FTData::Level(Level).Glow);
	ApplyHeadlights();
	// a level instance's sun (Bayshore's) registers a little after the map: look again shortly (and at every take)
	if (UWorld* W = GetWorld())
		for (float Dly : { 1.f, 3.f })
		{
			FTimerHandle Th;
			W->GetTimerManager().SetTimer(Th, FTimerDelegate::CreateWeakLambda(this, [this]() { ApplyHeadlights(); }), Dly, false);
		}
	// FX round 3: the level's surface: what the rear wheels throw off it (NS_WheelA<n> the run of it at speed, NS_WheelB<n>
	// the rooster tail of a slide or a boost; n in FTData's order of levels), what a landing kicks up, and the colour of
	// the marks the tyres leave. Looks only
	SurfaceLevel = FMath::Clamp(Level, 0, 7);
	auto Give = [](UNiagaraComponent* C, const FString& Name)
	{
		UNiagaraSystem* Sys = FTFx::System(*Name);
		if (C->GetAsset() != Sys) { C->Deactivate(); C->SetAsset(Sys); }
	};
	Give(WheelFX, FString::Printf(TEXT("NS_WheelA%d"), SurfaceLevel));
	Give(WheelFX2, FString::Printf(TEXT("NS_WheelB%d"), SurfaceLevel));
	FTFx::System(*FString::Printf(TEXT("NS_LandKick%d"), SurfaceLevel));   // loaded now, not at the first landing
	FTFx::Surface(this, SurfaceLevel);
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

void AFTPlayerCar::NosePin(float Deg, int32 Side)
{
	if (!Track || !Track->IsBuilt() || bWrecked) return;
	Side = Side < 0 ? -1 : 1;
	float Dn = D + 400.f;
	while (Track->FlagsAt(Dn) & (FTFlag::Gap | FTFlag::Kicker | FTFlag::Loop | FTFlag::Pipe | FTFlag::Twist | FTFlag::Coaster | FTFlag::HalfPipe)) Dn += 400.f;
	const float Xn = Side * (FT::Edge - 330.f);   // (its nose corner ~0.8 m short of the barrier at 60-80 degrees)
	PlaceOnTrack(Dn, Xn, 0.f);
	const FTransform T = Chassis->GetComponentTransform();
	const FQuat Q = T.GetRotation() * FQuat(FVector::UpVector, FMath::DegreesToRadians(Deg * Side));
	Chassis->SetWorldTransform(FTransform(Q, T.GetLocation()), false, nullptr, ETeleportType::ResetPhysics);
	Chassis->SetPhysicsLinearVelocity(Q.GetForwardVector() * 300.f);
	NoseProbeT = 0.f; NoseProbeD = Dn;
	UE_LOG(LogFT, Display, TEXT("FTNOSEPIN start D=%.0f X=%.0f deg=%.0f unstick=%d"), Dn, Xn, Deg * Side, CVarFTWallUnstick.GetValueOnGameThread());
}

void AFTPlayerCar::PlaceOnTrack(float InD, float InX, float InSpeed)
{
	if (!Track) return;
	if (bWheelsOffStatic) { bWheelsOffStatic = false; Vehicle->SetWheelTraceResponseToChannel(ECC_WorldStatic, ECR_Block); }   // (5.0 ft.WheelBarrier)
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
	ApplyHeadlights(); // 4.1: a level instance's sun may stream in after the car was set up for the level (4.2: after the show above, so day levels hide them again)
	FxAirT = 0.f;
	ShedQuietT = GetWorld() ? GetWorld()->GetTimeSeconds() + 0.5f : -1.f;
	bMarkOn[0] = bMarkOn[1] = false;   // FX round 3: no tyre mark is drawn from where the car was to where it has been put
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
	// FX round 4: the body wears its damage: dents, cracked lamps, soot, coming on one by one as the armour drops and
	// going again on a repair (CarMarks, in order). One that comes on in play does it with a few chips of paint, or of
	// glass, thrown off the panel; not when a take starts with the car already hurt
	{
		int32 Want = 0;
		for (int32 i = 0; i < DamageMarks.Num() && i < NumCarMarks; ++i) if (bWrecked || Hp < CarMarks[i].Below) Want = i + 1;
		if (Want != MarksShown)
		{
			const bool bLoud = DamageStage >= 0 && !bWrecked && GetWorld() && GetWorld()->GetTimeSeconds() > ShedQuietT;
			for (int32 i = 0; i < DamageMarks.Num() && i < NumCarMarks; ++i)
			{
				UStaticMeshComponent* Mark = DamageMarks[i];
				const bool bOn = i < Want;
				if (!Mark || Mark->IsVisible() == bOn) continue;
				Mark->SetVisibility(bOn);
				if (bOn && bLoud)
					FTFx::Attach(CarMarks[i].Kind == 1 ? TEXT("NS_HitGlass") : TEXT("NS_RamChips"), Body, CarMarks[i].At, CarMarks[i].Normal.Rotation(), 0.8f, FTFx::Extra, true);
			}
			MarksShown = Want;
		}
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

void AFTPlayerCar::SetHenchmenVisible(bool bLeft, bool bRight, FVector2D Age, FVector2D UntilPunch, FVector2D SinceWallHit)
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
			// a wall hit on his side crushes him in against the door and he bounces back out (decays in ~0.4 s); never
			// closer than 14 degrees, so his braced knees and boots stay off the door
			const float WallT = SinceWallHit[i];
			const float Jolt = FMath::Exp(-9.f * WallT) * FMath::Cos(16.f * WallT);
			const float Out = FMath::Max(14.f, 40.f + 6.f * FMath::Sin(T * 5.3f + 1.7f * i) + 12.f * Catch - 7.f * Windup - 12.f * Jolt);
			const float Back = (22.f + 5.f * FMath::Sin(T * 7.f + i) - 9.f * Catch + 5.f * Windup + 6.f * Jolt) * (i == 0 ? 1.f : -1.f);
			const FQuat Local = FQuat(FVector::ForwardVector, FMath::DegreesToRadians(-Out)) * FQuat(FVector::RightVector, FMath::DegreesToRadians(Back));
			Henchmen[i]->SetRelativeTransform(FTransform(FQuat(FVector::UpVector, i == 0 ? 0.f : PI) * Local,
				FVector(HangGrip.X, i == 0 ? -HangGrip.Y : HangGrip.Y, HangGrip.Z), FVector(1.f, 1.f, 1.f - 0.055f * FMath::Abs(Catch) - 0.07f * FMath::Abs(Jolt))));
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
		LastHitUp = FVector::DotProduct(Hit.ImpactNormal, RoadUp);
		LastHitTrack = Other == Track;
#if !UE_BUILD_SHIPPING
		if (CVarFTHitTrace.GetValueOnGameThread() > 0 && Other == Track)
		{
			// 5.2 probe: the hit in the road's own frame (r = across, u = up, f = along), the impulse as a speed change, where on the
			// body it landed (across / ahead of the middle), the car's speed and yaw rate as the hit is reported
			const FFTFrame Fh = Track->Sample(D);
			const FVector N = Hit.ImpactNormal;
			const FVector Vh = Chassis->GetPhysicsLinearVelocity();
			const float Imp = NormalImpulse.Size() / FMath::Max(1.f, Chassis->GetMass());
			const FVector Rel = Hit.ImpactPoint - Chassis->GetComponentLocation();
			const FVector Ni = NormalImpulse.GetSafeNormal();
			UE_LOG(LogFT, Display, TEXT("FTHIT t=%.3f D=%.0f X=%.0f hx=%.0f hh=%.0f n(r,u,f)=%.2f,%.2f,%.2f imp=%.0f impdir(r,u,f)=%.2f,%.2f,%.2f across=%.0f ahead=%.0f pen=%.1f v(f,r,u)=%.0f,%.0f,%.0f yawrate=%.2f w=%.2f flags=%d comp=%s"),
				GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f, D, X, Hx, Hh, FVector::DotProduct(N, Fh.Right), FVector::DotProduct(N, Fh.Up), FVector::DotProduct(N, Fh.Fwd), Imp,
				FVector::DotProduct(Ni, Fh.Right), FVector::DotProduct(Ni, Fh.Up), FVector::DotProduct(Ni, Fh.Fwd),
				FVector::DotProduct(Rel, Chassis->GetRightVector()), FVector::DotProduct(Rel, Chassis->GetForwardVector()), Hit.PenetrationDepth,
				FVector::DotProduct(Vh, Fh.Fwd), FVector::DotProduct(Vh, Fh.Right), FVector::DotProduct(Vh, Fh.Up),
				FVector::DotProduct(Chassis->GetPhysicsAngularVelocityInRadians(), Fh.Up), Chassis->GetPhysicsAngularVelocityInRadians().Size(), (int32)Fh.Flags,
				OtherComp ? *OtherComp->GetName() : TEXT("-"));
			++TwHits; TwHitImp = FMath::Max(TwHitImp, Imp);
			if (FMath::Abs(FVector::DotProduct(N, Fh.Up)) < 0.5f) TwHitNR = FVector::DotProduct(NormalImpulse, Fh.Right) / FMath::Max(1.f, Chassis->GetMass());
		}
#endif
	}
	if (Other && Other == Track)
	{
		const float Side = FVector::DotProduct(Hit.ImpactNormal, RoadUp);
		if (FMath::Abs(Side) < 0.5f) // a barrier, not the deck
		{
			WallTimer = 0.12f;
			const float Imp = NormalImpulse.Size() / FMath::Max(1.f, Chassis->GetMass());
			WallImpact = FMath::Max(WallImpact, Imp);
			// which door took it: where the contact landed across the car body (the contact normal's sign convention
			// differs between the two bodies of a physics hit; the point does not)
			const float Across = FVector::DotProduct(Hit.ImpactPoint - Chassis->GetComponentLocation(), Chassis->GetRightVector());
			if (FMath::Abs(Across) > 30.f)
			{
				const int32 S = Across > 0.f ? 1 : 0;
				if (WallSideT[S] <= 0.f) { ++WallSideHits[S]; WallSideImpact[S] = 0.f; }
				WallSideT[S] = 0.12f;
				WallSideImpact[S] = FMath::Max(WallSideImpact[S], Imp);
			}
			// 4.6: glancing or nose-first. Nose-first = the contact normal runs along the car (within ~35 degrees) and the
			// contact is ahead of its middle (sign-proof: the normal's sign differs between the two bodies of a hit)
			const float NFwd = FVector::DotProduct(Hit.ImpactNormal, Chassis->GetForwardVector());
			const float Ahead = FVector::DotProduct(Hit.ImpactPoint - Chassis->GetComponentLocation(), Chassis->GetForwardVector());
			if (FMath::Abs(NFwd) > 0.57f && Ahead > 80.f) bNoseHit = true; else bGlanceHit = true;
		}
	}
}

void AFTPlayerCar::ApplyBodySlide()
{
	BodySlideOn = CVarFTBodySlide.GetValueOnGameThread();
	BodySlideMu = CVarFTBodyFriction.GetValueOnGameThread();
	if (!Chassis) return;
	if (BodySlideOn > 0)
	{
		// a new object for each friction value: a physical material's solver copy is made on first use
		UPhysicalMaterial* PM = NewObject<UPhysicalMaterial>(this, NAME_None, RF_Transient);
		PM->Friction = PM->StaticFriction = FMath::Max(0.f, BodySlideMu);
		PM->bOverrideFrictionCombineMode = true;
		PM->FrictionCombineMode = EFrictionCombineMode::Min;
		PM->Restitution = 0.f;
		PM->bOverrideRestitutionCombineMode = true;
		PM->RestitutionCombineMode = EFrictionCombineMode::Min;
		BodySlidePM = PM;
		Chassis->SetPhysMaterialOverride(PM);
	}
	else
	{
		BodySlidePM = nullptr;
		Chassis->SetPhysMaterialOverride(nullptr);
	}
	UE_LOG(LogFT, Display, TEXT("FTBODYPM slide=%d friction=%.2f restitution=%.2f"), BodySlideOn,
		BodySlideOn > 0 ? BodySlideMu : 0.7f, BodySlideOn > 0 ? 0.f : 0.3f);
}

void AFTPlayerCar::Tick(float Dt)
{
	Super::Tick(Dt);
	{
		// FX overhaul: the effects' own frame (blasts that roll on, their delayed layers), and a clean start for every
		// new scene. Parked for a slate (a retake, the title, PRE-PRODUCTION) or hidden for the highlight reel, nothing
		// of the scene before is left burning or smoking: FTFx::ClearWorld stops it and logs what it stopped
		const bool bStage = Drive.bFrozen || IsHidden();
		if (bStage && !bFxStage)
		{
			FTFx::ClearWorld(this, IsHidden() ? TEXT("reel") : TEXT("slate"));
			// (FX round 3: and the car's own running effects of the world stop with it: the wheels' spray, the exhaust, the speed lines)
			UNiagaraComponent* Mine[] = { WheelFX, WheelFX2, ExhaustFX, SpeedFXL, SpeedFXR };
			for (UNiagaraComponent* C : Mine) if (C && C->IsActive()) C->DeactivateImmediate();
		}
		bFxStage = bStage;
		FTFx::Tick(this, Dt);
	}
	{
		// 4.8 (ft.PhysSubsteps): physics keeps the game's clock in slow frames
		const int32 Sub = CVarFTPhysSubsteps.GetValueOnGameThread();
		UPhysicsSettings* PS = UPhysicsSettings::Get();
		if (Sub > 0 && PS && PS->MaxSubsteps != Sub)
		{
			UE_LOG(LogFT, Display, TEXT("FTSUBSTEPS %d -> %d (substep %.4f s: real time down to %.1f fps)"), PS->MaxSubsteps, Sub, PS->MaxSubstepDeltaTime, 1.f / FMath::Max(PS->MaxSubstepDeltaTime * Sub, 1e-3f));
			PS->MaxSubsteps = Sub;
		}
		else if (Sub == 0 && PS && PS->MaxSubsteps != 8) { UE_LOG(LogFT, Display, TEXT("FTSUBSTEPS %d -> 8 (project setting)"), PS->MaxSubsteps); PS->MaxSubsteps = 8; }
	}
	if (CVarFTBodySlide.GetValueOnGameThread() != BodySlideOn || (BodySlideOn > 0 && CVarFTBodyFriction.GetValueOnGameThread() != BodySlideMu)) ApplyBodySlide();
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
	for (int32 S = 0; S < 2; ++S)
	{
		WallSideT[S] -= Dt;
		bWallSide[S] = WallSideT[S] > 0.f;
		if (!bWallSide[S]) WallSideImpact[S] = 0.f;
	}
	// no physics hit recorded but pressed against the road's edge (the arcade clamp can hold it there without one):
	// the door facing that edge, if the car points roughly along the road
	if (!bWallSide[0] && !bWallSide[1] && FMath::Abs(X) > FT::Edge - 150.f)
	{
		const float Facing = FVector::DotProduct(Chassis->GetForwardVector(), F.Fwd);
		if (FMath::Abs(Facing) > 0.5f) bWallSide[(X > 0.f) == (Facing > 0.f) ? 1 : 0] = true;
	}

	// 4.6 barrier contact (physics run 2). The v450 survey: a glancing touch of the side barrier at turbo speed cost
	// 25-35% of the car's speed in under half a second (Chaos friction and the bounce turning the car), and a car
	// spun nose-first into it sat pinned there (the throttle held the nose in, the yaw assist couldn't turn it)
	// until the crew put it back 1.2 s later.
	{
		const float NowW = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
		static IConsoleVariable* AutoW = IConsoleManager::Get().FindConsoleVariable(TEXT("ft.Autopilot"));
		const bool bLogW = AutoW && AutoW->GetInt() > 0;
		if (bNoseHit) NoseHitT = NowW;
		if (bGlanceHit && !bNoseHit && CVarFTWallGlance.GetValueOnGameThread() && !bWrecked && !Drive.bFrozen && LastSpeed > 2000.f)
		{
			// the speed it had before this touch (less what the governor would have taken anyway), 2% off the first touch
			const float Pre = FMath::Min(LastSpeed, FMath::Max(Drive.TargetSpeed, LastSpeed - Drive.Accel * Dt));
			WallFloor = FMath::Max(WallHoldT > 0.f ? WallFloor : 0.f, Pre * 0.98f);
			WallHoldT = 0.25f;
		}
		if (bNoseHit || bWrecked || Drive.bFrozen) WallHoldT = 0.f;
		if (WallHoldT > 0.f)
		{
			WallHoldT -= Dt;
			WallFloor -= WallFloor * 0.05f * Dt;   // a light drag while it scrapes (5% a second)
			if (WallFloor > Drive.TargetSpeed) WallFloor = FMath::Max(Drive.TargetSpeed, WallFloor - Drive.Accel * Dt);   // the governor's own easing
		}
		// nose pinned into the barrier, throttle open, crawling
		NoseT = (CVarFTWallUnstick.GetValueOnGameThread() && NowW - NoseHitT < 0.25f && Drive.TargetSpeed > 2000.f && FMath::Abs(Speed) < 1500.f
			&& !bWrecked && !Drive.bFrozen && !(Track->FlagsAt(D) & FTFlag::Gap)) ? NoseT + Dt : 0.f;
		if (NoseT <= 0.f) bUnstickLogged = false;
		// telemetry: one FTWALL line per contact (from the first touch to 0.3 s after the last)
		if (bLogW && (bGlanceHit || bNoseHit) && WallEpT < 0.f && !bWrecked && !Drive.bFrozen)
		{
			WallEpT = 0.f; WallEpV0 = LastSpeed; WallEpMin = Speed; WallEpTgt = Drive.TargetSpeed; WallEpD = D; WallEpX = X; WallEpHits = 0;
			WallEpFwd = bNoseHit ? 1.f : 0.f;
		}
		if (WallEpT >= 0.f)
		{
			WallEpT += Dt; WallEpMin = FMath::Min(WallEpMin, Speed); WallEpTgt = FMath::Min(WallEpTgt, Drive.TargetSpeed);
			if (bGlanceHit || bNoseHit) ++WallEpHits;
			if (bNoseHit) WallEpFwd = 1.f;
			if (WallTimer < -0.3f || bWrecked || Drive.bFrozen || WallEpT > 4.f)
			{
				// gov = what the governor alone would have taken over the contact (turbo ending)
				const float Gov = FMath::Clamp(WallEpV0 - WallEpTgt, 0.f, Drive.Accel * WallEpT);
				UE_LOG(LogFT, Display, TEXT("FTWALL kind=%s D=%.0f X=%.0f v0=%.0f min=%.0f end=%.0f target=%.0f gov=%.0f lost=%.0f%% dur=%.2fs hits=%d"),
					WallEpFwd > 0.f ? TEXT("nose") : TEXT("glance"), WallEpD, WallEpX, WallEpV0, WallEpMin, Speed, WallEpTgt, Gov,
					100.f * FMath::Max(0.f, WallEpV0 - Gov - WallEpMin) / FMath::Max(WallEpV0, 1.f), WallEpT, WallEpHits);
				WallEpT = -1.f;
			}
		}
		bGlanceHit = bNoseHit = false;
	}

	ApplyArcade(Dt);
#if !UE_BUILD_SHIPPING
	{
		// 5.2 ft.HitTrace: one FTTW line a frame in a corkscrew (2: and in pipes). v0 = the car's speed (along, across, up the
		// road) as the frame began, vset = across after this frame's direct speed sets; the pushes held over the frame
		// (cm/s^2): cp = corkscrew centre pull, lat = the lane spring/damper, grip, gov (governor's across part); back/out =
		// put in from the edge (cm / cm/s); hits = track hits reported since the last line (max impulse, across part of a barrier's)
		const int32 HT = CVarFTHitTrace.GetValueOnGameThread();
		const uint16 Fl = Track->FlagsAt(D);
		if (HT > 0 && ((Fl & FTFlag::Twist) || (HT > 1 && (Fl & FTFlag::Pipe))) && !bWrecked && !Drive.bFrozen)
		{
			const FFTFrame Ft = Track->Sample(D);
			const FVector Vs = Chassis->GetPhysicsLinearVelocity();
			const FVector CarF = Chassis->GetForwardVector();
			const FVector Cf = (CarF - Ft.Up * FVector::DotProduct(CarF, Ft.Up)).GetSafeNormal();
			const float Herr = FMath::RadiansToDegrees(FMath::Atan2(FVector::DotProduct(FVector::CrossProduct(PathF, Cf), Ft.Up), FVector::DotProduct(Cf, PathF)));
			UE_LOG(LogFT, Display, TEXT("FTTW t=%.3f dt=%.3f D=%.0f X=%.0f lift=%.0f wh=%d herr=%.1f yaw0=%.2f yawset=%.2f v0(f,r,u)=%.0f,%.0f,%.0f vset_r=%.0f cp=%.0f lat=%.0f grip=%.0f gov=%.0f back=%.0f out=%.0f sweep=%.0f want=%.2f yawA=%.1f hits=%d imp=%.0f impR=%.0f put=%d fl=%d"),
				GetWorld()->GetTimeSeconds(), Dt, D, X, TwLift, WheelsDown, Herr, TwW0, FVector::DotProduct(Chassis->GetPhysicsAngularVelocityInRadians(), Ft.Up),
				FVector::DotProduct(TwV0, Ft.Fwd), FVector::DotProduct(TwV0, Ft.Right), FVector::DotProduct(TwV0, Ft.Up), FVector::DotProduct(Vs, Ft.Right),
				TwCp, TwLat, TwGrip, TwGov, TwBack, TwOut, TwSweep, TwWant, TwYawA, TwHits, TwHitImp, TwHitNR, TwPut ? 1 : 0, (int32)Fl);
		}
		TwHits = 0; TwHitImp = TwHitNR = 0.f;
	}
#endif
	LastSpeed = Speed;
	if (NoseProbeT >= 0.f)
	{
		NoseProbeT += Dt;
		if (Speed > 2500.f || NoseProbeT > 6.f)
		{
			UE_LOG(LogFT, Display, TEXT("FTNOSEPIN %s after %.2fs D+%.0f X=%.0f v=%.0f"), Speed > 2500.f ? TEXT("away") : TEXT("STUCK"), NoseProbeT, D - NoseProbeD, X, Speed);
			NoseProbeT = -1.f;
		}
	}
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
				Kind = LaunchT > 0.f ? TEXT("ramp") : ((F.Flags & (FTFlag::Kicker | FTFlag::Gap)) ? TEXT("gap") : ((HopT >= 0.f || bRailInit) ? TEXT("hop") : TEXT("road")));
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
				UE_LOG(LogFT, Display, TEXT("FTGLUE D=%.0f straysAir=%.2fs strayMaxH=%.0f jumpAir=%.2fs jumpMaxH=%.0f v=%.0f fps=%.0f stop=%d"), D, StrayAir, StrayMaxH, JumpAir, JumpMaxH, Speed, 1.f / FMath::Max(Dt, 1e-4f), StopFrames);
				StrayAir = StrayMaxH = JumpAir = JumpMaxH = GlueT = 0.f; StopFrames = 0;
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
			// 4.5: what took the speed. gov = the governor easing a turbo or a boost back to its target (no contact, wheels down)
			const bool bRecent = Now - LastHitT < 0.3f;
			// 4.6: the governor easing a turbo's end back to its speed takes up to Accel x time by itself: a touch that
			// happened meanwhile was blamed for all of it ("wall" with target 5,210 from 9,300)
			const float GovLoss = FMath::Clamp(Peak - Drive.TargetSpeed, 0.f, Drive.Accel * (Now - PeakT));
			const TCHAR* Cause = (Peak - Speed - GovLoss < 900.f && GovLoss > 0.f) ? TEXT("gov") : !bRecent ?(WheelsDown >= 3 && Peak > Drive.TargetSpeed + 500.f ? TEXT("gov") : TEXT("other"))
				: (FMath::Abs(LastHitUp) < 0.5f ? TEXT("wall") : (LastHitTrack ? TEXT("deck") : TEXT("prop")));
			UE_LOG(LogFT, Display, TEXT("FTBUMP cause=%s D=%.0f X=%.0f H=%.0f v=%.0f from %.0f in %.2fs target=%.0f section=%d shape=%d u=%.2f flags=%d wheels=%d hit(%.2fs ago)=%s"),
				Cause, D, X, H, Speed, Peak, Now - PeakT, Drive.TargetSpeed, F.Section, Sc ? (int32)Sc->Shape : -1, U, (int32)F.Flags, WheelsDown,
				Now - LastHitT, Now - LastHitT < 0.7f ? *LastHit : TEXT("-"));
		}
	}
	// 4.7 tests (autopilot runs): FTSPIN, one line per spin-out (heading more than 35 degrees off the road, not drifting,
	// not in a half pipe): where, what came before (a landing, a barrier hit, the steering) and the ~0.5 s leading up to it
	if (!bWrecked && !Drive.bFrozen && GetWorld())
	{
		static IConsoleVariable* AutoP = IConsoleManager::Get().FindConsoleVariable(TEXT("ft.Autopilot"));
		const float Now = GetWorld()->GetTimeSeconds();
		const FVector CarF = Chassis->GetForwardVector();
		const FVector Cf = (CarF - F.Up * FVector::DotProduct(CarF, F.Up)).GetSafeNormal();
		const float Herr = FMath::RadiansToDegrees(FMath::Atan2(FVector::DotProduct(FVector::CrossProduct(PathF, Cf), F.Up), FVector::DotProduct(Cf, PathF)));   // (against the surface's path: a pipe's isn't its axis)
		const float Yr = FVector::DotProduct(Chassis->GetPhysicsAngularVelocityInRadians(), F.Up);
		SpinHist.Add(FVector4f(Now, Herr, Yr, LatSpeed)); SpinHist2.Add(FVector4f(Drive.Steer, (float)WheelsDown, X, Speed));
		while (SpinHist.Num() && SpinHist[0].X < Now - 0.6f) { SpinHist.RemoveAt(0, 1, EAllowShrinking::No); SpinHist2.RemoveAt(0, 1, EAllowShrinking::No); }
		const bool bHalf = (F.Flags & FTFlag::HalfPipe) || Half != EFTHalf::None || IsOnRails();
		if (AutoP && AutoP->GetInt() > 0 && !bHalf)
		{
			if (SpinT < 0.f && FMath::Abs(Herr) > 35.f && Drive.DriftDir == 0)
			{
				SpinT = 0.f; SpinMax = Herr; SpinV0 = SpinHist.Num() ? SpinHist2[0].W : Speed; SpinVMin = Speed; SpinD = D; SpinOkT = 0.f;
				float U = 0.f; const FFTSection* Sc = Track->SectionAt(D, &U);
				FString Trail;
				for (int32 i = 0; i < SpinHist.Num(); i += FMath::Max(1, SpinHist.Num() / 6))
					Trail += FString::Printf(TEXT(" [%.2f h%.0f w%.1f vl%.0f s%.1f wh%.0f x%.0f v%.0f]"), SpinHist[i].X - Now, SpinHist[i].Y, SpinHist[i].Z, SpinHist[i].W, SpinHist2[i].X, SpinHist2[i].Y, SpinHist2[i].Z, SpinHist2[i].W);
				UE_LOG(LogFT, Display, TEXT("FTSPIN start D=%.0f X=%.0f herr=%.0f yawrate=%.2f vlat=%.0f v=%.0f steer=%.2f wheels=%d air=%.2fs land=%.2fs ago wall=%.2fs ago(up %.2f) turbo=%d tgt=%.0f section=%d shape=%d flags=%d trail:%s"),
					D, X, Herr, Yr, LatSpeed, Speed, Drive.Steer, WheelsDown, AirTime, Now - LastLandT, Now - LastHitT, LastHitUp, Drive.TargetSpeed > 6500.f ? 1 : 0, Drive.TargetSpeed,
					F.Section, Sc ? (int32)Sc->Shape : -1, (int32)F.Flags, *Trail);
			}
			else if (SpinT >= 0.f)
			{
				SpinT += Dt;
				if (FMath::Abs(Herr) > FMath::Abs(SpinMax)) SpinMax = Herr;
				SpinVMin = FMath::Min(SpinVMin, Speed);
				SpinOkT = FMath::Abs(Herr) < 15.f ? SpinOkT + Dt : 0.f;
				if (SpinOkT > 0.2f || SpinT > 5.f)
				{
					UE_LOG(LogFT, Display, TEXT("FTSPIN end D=%.0f dur=%.2fs maxHerr=%.0f v0=%.0f min=%.0f end=%.0f lost=%.0f%%"), SpinD, SpinT, SpinMax, SpinV0, SpinVMin, Speed,
						100.f * FMath::Max(0.f, SpinV0 - SpinVMin) / FMath::Max(SpinV0, 1.f));
					SpinT = -1.f;
				}
			}
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
			UE_LOG(LogFT, Display, TEXT("FTCAPSIZE D=%.0f X=%.0f lift=%.0f upright=%.2f wheels=%d v=%.0f belly=%d lastHit(%.1fs ago)=%s"), D, X, H - SurfH - ChassisZ, Upright, WheelsDown, Speed, bBelly ? 1 : 0,
				GetWorld() ? GetWorld()->GetTimeSeconds() - LastHitT : -1.f, *LastHit);
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
		if (Drive.bHot && !bTurboCel && !TurboFX->IsActive()) TurboFX->Activate(true);   // (FX round 3: not while the banded flame stands in for it)
		if ((!Drive.bHot || bTurboCel) && TurboFX->IsActive()) TurboFX->Deactivate();
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
			// (FX round 3: what it kicks up is the level's own: sand, snow, silt, ash; the plain dust on a build without them)
			const FString Kick = FString::Printf(TEXT("NS_LandKick%d"), SurfaceLevel);
			FTFx::Attach(FTFx::System(*Kick) ? *Kick : TEXT("NS_LandDust"), Body, FVector(0.f, 0.f, 14.f), FRotator::ZeroRotator, K, FTFx::Extra, true);
		}
		FxAirT = 0.f;
	}
	// ---- FX round 3: the car in the world, and speed. Still looks only.
	auto Lit = [](UNiagaraComponent* C, bool bOn)
	{
		if (!C->GetAsset()) return;
		if (bOn && !C->IsActive()) C->Activate(true);
		if (!bOn && C->IsActive()) C->Deactivate();
	};
	// What the rear wheels throw off the road is the level's own (SetLightsForLevel picked the systems): sand, slick,
	// dust, ash, confetti, powder snow, silt, rain. A thin run of it whenever the car is at speed on its wheels (the
	// Main layer: a few small sprites), and the rooster tail of a drift, a boost or the turbo (Extra). Both stream
	// back along the car's real line, as the tyre smoke does
	const bool bRoll = bGrounded && !bWrecked && !Drive.bFrozen && !bRailInit && Half == EFTHalf::None;
	const bool bRun = bRoll && FMath::Abs(Speed) > 2400.f;
	if (bRun && Flat.SizeSquared() > FMath::Square(500.f))
	{
		const FRotator Back = FRotationMatrix::MakeFromXZ(Flat.GetSafeNormal(), RoadUp).Rotator();
		WheelFX->SetWorldRotation(Back);
		WheelFX2->SetWorldRotation(Back);
	}
	Lit(WheelFX, bRun);
	Lit(WheelFX2, bRun && (Drive.DriftDir != 0 || Drive.bHot) && FTFx::Shows(this, FTFx::Extra));
	// Tyre marks: laid on the road under the rear tyres while they slide (a drift, or the tail stepping out), a length
	// every 2.5 m. FTFx::TyreMark keeps the newest 480 lengths for the scene (none at Low)
	const bool bMark = bRoll && (Drive.DriftDir != 0 || FMath::Abs(LatSpeed) > 1100.f) && Flat.SizeSquared() > FMath::Square(1500.f);
	for (int32 i = 0; i < 2; ++i)
	{
		const int32 Wi = 2 + i;   // the rear wheels
		const bool bDown = bMark && WheelMeshes.IsValidIndex(Wi) && WheelMeshes[Wi] && Vehicle->Wheels.IsValidIndex(Wi) && Vehicle->Wheels[Wi] && !Vehicle->Wheels[Wi]->IsInAir();
		if (!bDown) { bMarkOn[i] = false; continue; }
		// the tyre's patch on the road: under the hub by the wheel's radius, along the surface's own normal
		const FVector Patch = WheelMeshes[Wi]->GetComponentLocation() - SurfN * Vehicle->Wheels[Wi]->WheelRadius;
		if (!bMarkOn[i]) { bMarkOn[i] = true; MarkFrom[i] = Patch; continue; }
		const float Gone = (float)FVector::Dist(Patch, MarkFrom[i]);
		if (Gone > 1200.f) { MarkFrom[i] = Patch; continue; }   // the car was put somewhere else: start again from here
		if (Gone < 250.f) continue;
		FTFx::TyreMark(this, MarkFrom[i], Patch, SurfN, Vehicle->Wheels[Wi]->WheelWidth * 0.8f);
		MarkFrom[i] = Patch;
	}
	// The turbo's flame, banded. TurboFX holds the additive tuft FTFlow gives it (the stock one, or a nitro colour's):
	// where the generator has a banded flame of that name (NS_TurboFlameCel, NS_TurboFlameCel_<colour>) it burns in
	// the tuft's place, at the tuft's own length, and Tick leaves the tuft unlit. A build without it keeps the tuft
	const FName Tuft = TurboFX->GetAsset() ? TurboFX->GetAsset()->GetFName() : FName();
	if (Tuft != TurboTuftSeen)
	{
		TurboTuftSeen = Tuft;
		const FString TuftName = Tuft.ToString();
		UNiagaraSystem* Cel = TuftName.StartsWith(TEXT("NS_TurboFlame")) ? FTFx::System(*(FString(TEXT("NS_TurboFlameCel")) + TuftName.RightChop(13))) : nullptr;
		if (TurboCelFX->GetAsset() != Cel) { TurboCelFX->Deactivate(); TurboCelFX->SetAsset(Cel); }
		bTurboCel = Cel != nullptr;
	}
	if (bTurboCel) TurboCelFX->SetRelativeScale3D(TurboFX->GetRelativeScale3D());
	Lit(TurboCelFX, bTurboCel && Drive.bHot);
	// The exhaust at top speed, when neither the turbo nor a boost is burning: short banded licks out of the pipes (Extra)
	const bool bTop = !bWrecked && !Drive.bFrozen && Speed > FT::BaseSpeed * 1.17f;
	Lit(ExhaustFX, bTop && !Drive.bHot && FTFx::Shows(this, FTFx::Extra));
	// Speed: thin streaks past the edges of the picture while the turbo or a boost burns, and at top speed. The two
	// emitters are put either side of where the camera looks, 14 m on and 9.5 m out, well off the road, and turned to
	// it: their streaks fly back past the lens. The Flash layer (off with Reduced Motion) and not at Low. The camera
	// is only read (last frame's: a metre late at this speed, which a streak does not show)
	bool bLines = (bTop || (Drive.bHot && Speed > 3000.f)) && !bWrecked && !Drive.bFrozen && !IsHidden() && FTFx::Shows(this, FTFx::Extra | FTFx::Flash);
	if (bLines)
	{
		const APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
		if (PC && PC->PlayerCameraManager)
		{
			const FVector Eye = PC->PlayerCameraManager->GetCameraLocation();
			const FRotator Look = PC->PlayerCameraManager->GetCameraRotation();
			SpeedFXL->SetWorldLocationAndRotation(Eye + Look.RotateVector(FVector(1400.f, -950.f, 150.f)), Look);
			SpeedFXR->SetWorldLocationAndRotation(Eye + Look.RotateVector(FVector(1400.f, 950.f, 150.f)), Look);
		}
		else bLines = false;
	}
	Lit(SpeedFXL, bLines);
	Lit(SpeedFXR, bLines);
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
	// 5.2: stepped at 1/120 s at most. One step a frame is unstable past W x Dt ~2 (frames over ~0.17 s): a run of slow
	// frames (3 fps, or a stall streak) grew the swing every frame until the body sat 27-100 km under the road (v520
	// ft.FarProbe: the low-fps 'precision loss' ensure; the chassis and the game were fine, the car drawn was gone)
	const int32 NSub = FMath::Clamp(FMath::CeilToInt(Dt * 120.f), 1, 64);
	const float Hs = Dt / NSub;
	auto Spring = [&](float& X, float& V, float Target, float Lim)
	{
		for (int32 i = 0; i < NSub; ++i)
		{
			const float Acc = W * W * (Target - X) - 2.f * Z * W * V;
			V += Acc * Hs; X += V * Hs;
		}
		if (!FMath::IsFinite(X) || !FMath::IsFinite(V)) { X = 0.f; V = 0.f; }
		if (FMath::Abs(X) > Lim) { X = FMath::Sign(X) * Lim; V = 0.f; }
	};
	Spring(BodyRoll, BodyRollV, RollT, 12.f);
	Spring(BodyPitch, BodyPitchV, PitchT, 8.f);
	Spring(BodyHeave, BodyHeaveV, HeaveT, 25.f);
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
	// (5.0 ft.WheelBarrier: never left off on a wreck or a car parked for a slate)
	if (bWheelsOffStatic && (bWrecked || Drive.bFrozen)) { bWheelsOffStatic = false; Vehicle->SetWheelTraceResponseToChannel(ECC_WorldStatic, ECR_Block); }
	if (bWrecked)
	{
		Chassis->AddForce(FVector(0, 0, -400.f), NAME_None, true);
		return;
	}
	const FFTFrame F = Track->Sample(D);
	PathF = F.Fwd; PathYawRate = 0.f;
	HopCd = FMath::Max(0.f, HopCd - Dt);
	TwCp = TwLat = TwGrip = TwGov = TwBack = TwOut = TwSweep = TwYawA = TwWant = 0.f; TwPut = false;
	TwV0 = Chassis->GetPhysicsLinearVelocity(); TwW0 = FVector::DotProduct(Chassis->GetPhysicsAngularVelocityInRadians(), F.Up);

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
	const bool bFlagJump = (F.Flags & (FTFlag::Kicker | FTFlag::Gap)) != 0 || ((F.Flags & FTFlag::Landing) && !bGrounded) || LaunchT > 0.f || !bSolid;
	// height over the surface measured along its normal (on a half pipe wall the frame's up is tilted off it)
	const float Lift = Hr / FMath::Sqrt(1.f + Slope * Slope) - ChassisZ;
	TwLift = Lift;

	// 5.1 landing aim (ft.LandAim; loop target "road drops with no jump flag"). A gap jump's landing zone is the first 8%
	// of the ramp down (u 0.6-0.68), but the road under the gap pitches down from the kicker's +A to the ramp's -B, and at
	// turbo speed it falls away faster than the car's air gravity (1.7 g): every gap on Levels 1, 7 and 8 was flown past
	// its landing zone, the car still 3-17 m up (Level 8 D 280,700: 17 m, Level 1 D 357,800: 10-14 m) at every frame
	// rate. Past the zone the jump was over, so the glue took the car's speed off the road away in one frame and pulled it
	// down at 12 g with the snap-down teleporting it 35 cm a frame: a drop. Bit 1: in the air over the gap and the landing
	// zone the car's arc is bent down (only ever down, at most ft.LandAimMax = 3,000 cm/s^2 more than air gravity) so it meets the
	// road at the zone's end; a car that would come down sooner is left alone. Bit 2: a car still up past the zone stays
	// in its jump (no glue, no snap) until it touches down, so it comes down on gravity and the aim, not on the glue.
	const int32 LandAim = CVarFTLandAim.GetValueOnGameThread();
	AimA = 0.f;
	if (bGrounded || bWrecked) bLateAimOn = false;
	if ((F.Flags & (FTFlag::Kicker | FTFlag::Gap)) && GapLandEnd < D)
	{
		GapLandEnd = -1.f;
		bool bSeen = false;
		for (float s = D; s < D + 60000.f; s += 200.f)
		{
			const bool bL = (Track->FlagsAt(s) & FTFlag::Landing) != 0;
			if (bL) bSeen = true;
			else if (bSeen) { GapLandEnd = s; break; }
		}
	}
	if (bGrounded || bWrecked || bCoaster || (F.Flags & FTFlag::SetPiece) || LaunchT > 0.f || (bSolid && Lift < 20.f && !(F.Flags & (FTFlag::Gap | FTFlag::Kicker))))
		bGapFlight = false;
	else if ((LandAim & 2) && GapLandEnd > 0.f && (F.Flags & (FTFlag::Gap | FTFlag::Landing)))
		bGapFlight = true;
	if (bGapFlight && GapLandEnd > 0.f && D > GapLandEnd + 15000.f) bGapFlight = false; // never more than 150 m past the zone
	const bool bRealJump = bFlagJump || bGapFlight;
	if ((LandAim & 1) && !bGrounded && !bWrecked && !bCoaster && LaunchT <= 0.f && GapLandEnd > 0.f
		&& ((F.Flags & (FTFlag::Gap | FTFlag::Landing)) || bGapFlight))
	{
		const FVector Vc = Chassis->GetPhysicsLinearVelocity();
		const float Vf = FMath::Max(FVector::DotProduct(Vc, F.Fwd), 1500.f);
		// aim at the zone's end (4 m short of it); once past it, at where the car will be in 0.35 s
		const float Daim = D < GapLandEnd - 800.f ? GapLandEnd - 400.f : D + Vf * 0.35f;
		const float T = (Daim - D) / Vf;
		if (T > 0.12f)
		{
			const FVector Pt = Track->TransformAt(Daim, X, Track->SurfaceH(Daim, X) + ChassisZ).GetLocation();
			const FVector Pc = Chassis->GetComponentLocation();
			const float GAir = 980.f * AirGravityScale;
			// the extra downward pull that puts the car on the road there at that time (z + vz t - (g + a) t^2 / 2 = zt)
			const float Need = 2.f * (Pc.Z + Vc.Z * T - Pt.Z) / (T * T) - GAir;
			const float AimMax = CVarFTLandAimMax.GetValueOnGameThread();
			AimA = FMath::Clamp(Need, 0.f, AimMax);
			// 5.2 (bit 4, late aim): spread over the whole flight the pull also flattened the climb (rise ~4 m, was ~6.5). Now the
			// car climbs on air gravity alone and the pull only starts once the full pull is just enough to meet the zone's end
			// (the most height the cap allows for the same landing point); once started it stays on until touchdown.
			if (LandAim & 4)
			{
				if (!bLateAimOn && Need < AimMax * 0.9f) AimA = 0.f;
				else bLateAimOn = true;
			}
			if (AimA > 0.f) Chassis->AddForce(FVector(0, 0, -AimA), NAME_None, true);
		}
	}

	// 5.0 wheels on the barrier (ft.WheelBarrier). The rear wheels' centres are 112 cm out, the body box 100: with the body
	// against the barrier (at 1,371 cm) they already hang 12 cm over its top. Lifted ~0.7 m by a crest, their suspension rays
	// start above the 160 cm barrier and land on it; the springs then held the car up there (lift ~150 cm, up to 7 m on Level
	// 8's hills) with its wheels reading 'down', so the snap-down (all wheels off only) never came: the outer-lane "road
	// flights" at low frame rates. More than 20 cm over ride height no wheel can reach the road (14 cm of droop), so beside the
	// barrier the traces then ignore static geometry and the glue brings the car down.
	{
		const bool bZone = CVarFTWheelBarrier.GetValueOnGameThread() && !bWrecked && !bRealJump && !bCoaster && bSolid
			&& !(F.Flags & (FTFlag::SetPiece | FTFlag::Coaster | FTFlag::HalfPipe)) && FMath::Abs(X) > FT::RoadHalf - 20.f && FMath::Abs(X) < FT::Edge + 400.f;
		const bool bIgnore = bZone && Lift > 20.f;
		if (bIgnore != bWheelsOffStatic)
		{
			bWheelsOffStatic = bIgnore;
			Vehicle->SetWheelTraceResponseToChannel(ECC_WorldStatic, bIgnore ? ECR_Ignore : ECR_Block);
			static IConsoleVariable* AutoB = IConsoleManager::Get().FindConsoleVariable(TEXT("ft.Autopilot"));
			if (bIgnore && AutoB && AutoB->GetInt() > 0)
				UE_LOG(LogFT, Display, TEXT("FTWHEELBAR D=%.0f X=%.0f lift=%.0f wheels=%d v=%.0f"), D, X, Lift, WheelsDown, Speed);
		}
		if (bIgnore)
		{
			WheelsDown = 0; bGrounded = false;   // (what they touch up there is not the road)
			if (FMath::Abs(X) > FT::Edge - 130.f)
			{
				const float In = FVector::DotProduct(Chassis->GetPhysicsLinearVelocity(), F.Right) * -FMath::Sign(X);
				if (In < 300.f) Chassis->SetPhysicsLinearVelocity(Chassis->GetPhysicsLinearVelocity() - F.Right * FMath::Sign(X) * (300.f - In));
			}
		}
	}

	// 2.5 set pieces (loops, corkscrews, pipes, coaster rails): the car keeps the speed it came in with, all the
	// way round. The owner: "some of the corkscrews really slow the car down ... it needs to maintain speed".
	// The floor is the way-in speed (at least cruising speed); nothing here ever slows the car down.
	{
		const uint16 Kind = F.Flags & (FTFlag::SetPiece | FTFlag::Coaster);
		const bool bSetNow = Kind != 0 && !(F.Flags & (FTFlag::Kicker | FTFlag::Gap));
		const float VRoad = FVector::DotProduct(Chassis->GetPhysicsLinearVelocity(), F.Fwd);
		if (bSetNow && !bInSet)
		{
			bInSet = true; SetKind = Kind; SetEntry = VRoad; SetMin = VRoad; SetMaxLift = -1e6f; SetAir = 0.f; SetD0 = D; SetSnaps = 0; SetCeilFrames = 0;
			SetFloor = FMath::Max(VRoad, FT::BaseSpeed) * 0.98f;
		}
		else if (!bSetNow && bInSet)
		{
			bInSet = false;
			static IConsoleVariable* AutoS = IConsoleManager::Get().FindConsoleVariable(TEXT("ft.Autopilot"));
			if (AutoS && AutoS->GetInt() > 0)
				UE_LOG(LogFT, Display, TEXT("FTSET kind=%s D=%.0f entry=%.0f min=%.0f exit=%.0f kept=%.0f%% maxLift=%.0f air=%.2fs snaps=%d ceil=%d"),
					(SetKind & FTFlag::Coaster) ? TEXT("coaster") : ((SetKind & FTFlag::Loop) ? TEXT("loop") : ((SetKind & FTFlag::Pipe) ? TEXT("pipe") : TEXT("twist"))),
					SetD0, SetEntry, SetMin, VRoad, 100.f * SetMin / FMath::Max(SetEntry, 1.f), SetMaxLift, SetAir, SetSnaps, SetCeilFrames);
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
			// 4.9 (ft.SetCeil): and nothing in there speeds it up past what it came in with or is driving for. Off the
			// centreline of Level 7's fast pipe the deck-sweep assist sets the car's speed along the normal in proportion
			// to its forward speed (x0.87 at X ~1,000); striking the static deck turned that back into forward speed, which
			// fed the next frame's sweep: 10,935 -> 31,352 cm/s in 0.4 s (v490 d_24_l), then a put-back at 316 m/s.
			if (CVarFTSetCeil.GetValueOnGameThread() && !bWrecked)
			{
				const float Ceil = FMath::Max3(SetEntry, Drive.TargetSpeed, SetFloor) * 1.1f + 300.f;
				const float VNow = FVector::DotProduct(Chassis->GetPhysicsLinearVelocity(), F.Fwd);
				if (VNow > Ceil)
				{
					Chassis->SetPhysicsLinearVelocity(Chassis->GetPhysicsLinearVelocity() - F.Fwd * (VNow - Ceil));
					++SetCeilFrames;
				}
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
		if (bJumpAir) { bLandArmed = true; LandAirV = VRoadL; CushionLogged = false; }
		// 4.7 landing cushion (ft.LandCushion; LOOP-REPORT 37 target 1). A long gap jump landing past its ramp comes down
		// from up to 18 m (Level 3's BIG AIR) at ~2,500 cm/s into the deck. The chassis box met the deck first, its friction
		// at 8,000 cm/s took the speed (8,126 -> 1,744 cm/s) and twisted the car round (17 rad/s yaw: FTSPIN, a spin-out,
		// 51% lost) - worse at the barrier, worse at low frame rates (a frame then drops ~2 m). In the last ~2 frames before
		// the deck the speed into it is cut to what the springs take (600 cm/s) and the car is squared to the road and
		// pointed along it (within 35 degrees: a car already turned further is left alone), so it lands on its wheels.
		if (CVarFTLandCushion.GetValueOnGameThread() && bLandArmed && !bGrounded && bSolid && !bWrecked && !(F.Flags & (FTFlag::Gap | FTFlag::Kicker)))
		{
			const FVector Vc = Chassis->GetPhysicsLinearVelocity();
			const float Vn = FVector::DotProduct(Vc, Up);
			const float Reach = FMath::Max(60.f, -Vn * Dt * 2.f);
			if (Lift < Reach && Lift > -60.f)
			{
				if (Vn < -600.f) Chassis->SetPhysicsLinearVelocity(Vc + Up * (-600.f - Vn));
				const FVector CarF = Chassis->GetForwardVector();
				const FVector CfL = (CarF - Up * FVector::DotProduct(CarF, Up)).GetSafeNormal();
				const float He = FMath::Atan2(FVector::DotProduct(FVector::CrossProduct(F.Fwd, CfL), Up), FVector::DotProduct(CfL, F.Fwd));
				if (FMath::Abs(He) < 0.6f)
				{
					const FVector Tilt = FVector::CrossProduct(Chassis->GetUpVector(), Up);
					Chassis->SetPhysicsAngularVelocityInRadians(Tilt * 8.f + Up * FMath::Clamp(-He * 6.f, -3.f, 3.f));
				}
				static IConsoleVariable* AutoC = IConsoleManager::Get().FindConsoleVariable(TEXT("ft.Autopilot"));
				if (!CushionLogged && AutoC && AutoC->GetInt() > 0)
				{
					CushionLogged = true;
					UE_LOG(LogFT, Display, TEXT("FTCUSHION D=%.0f X=%.0f vn=%.0f lift=%.0f herr=%.0f v=%.0f fps=%.0f"), D, X, Vn, Lift, FMath::RadiansToDegrees(He), VRoadL, 1.f / FMath::Max(Dt, 1e-4f));
				}
			}
		}
		// 4.6: a touchdown is also the chassis reaching the deck with every wheel still reading "in the air" (landing
		// long, past the landing ramp, on its belly: the wheel traces start under the road), and with the turbo on (flight
		// speed under the governor's target) the touchdown speed is held too, for the impact only. The v460 survey: 4 of 8
		// levels lost 50-80% of their speed on a gap landing (Level 7 at turbo: 8,293 -> 3,192 cm/s, clawed back over
		// ~0.5 s; the 3.6 hold only ran above the target and only once the wheels read down).
		const bool bLandKeep = CVarFTLandKeep.GetValueOnGameThread() != 0;
		const bool bTouch = bGrounded || (bLandKeep && Lift < 10.f && bSolid && !(F.Flags & (FTFlag::Gap | FTFlag::Kicker)) && LaunchT <= 0.f);   // (not leaving a kicker or a ramp)
		// telemetry (both settings alike): FTLANDED 0.4 s after the first touch of the chassis or the wheels
		const bool bTouchAny = bGrounded || (Lift < 10.f && bSolid && !(F.Flags & (FTFlag::Gap | FTFlag::Kicker)) && LaunchT <= 0.f);
		if (bTouchAny && bLandArmed && GetWorld()) LastLandT = GetWorld()->GetTimeSeconds();
		if (bTouchAny && bLandArmed && LandProbeT <= 0.f) { LandProbeT = 0.4f; LandProbeV = LandAirV; LandProbeMin = 1e9f; LandProbeLowT = 0.f; }
		if (bTouch && bLandArmed)
		{
			bLandArmed = false;
			LandFloor = FMath::Min(LandAirV, 11000.f) * 0.97f;
			bLandAbove = LandFloor > Drive.TargetSpeed + 100.f;
			LandHoldT = bLandAbove ? 1.5f : (bLandKeep && LandFloor > 2000.f ? 0.3f : 0.f);
			if (LandHoldT > 0.f) UE_LOG(LogFT, Display, TEXT("FTLAND D=%.0f air=%.0f hit=%.0f floor=%.0f target=%.0f wheels=%d"), D, LandAirV, VRoadL, LandFloor, Drive.TargetSpeed, WheelsDown);
		}
		else if (bGrounded) bLandArmed = false;
		if (LandHoldT > 0.f)
		{
			LandHoldT -= Dt;
			if (bLandAbove)
			{
				LandFloor = FMath::FInterpTo(LandFloor, Drive.TargetSpeed, Dt, 2.f);
				if (LandFloor <= Drive.TargetSpeed + 50.f) LandHoldT = 0.f;
			}
			if (bWrecked) LandHoldT = 0.f;
			else if (VRoadL < LandFloor) Chassis->SetPhysicsLinearVelocity(Chassis->GetPhysicsLinearVelocity() + F.Fwd * (LandFloor - VRoadL));
		}
		if (LandProbeT > 0.f)
		{
			LandProbeT -= Dt;
			// the speed the car is left with this frame (after the hold above, if it holds)
			const float VPost = LandHoldT > 0.f ? FMath::Max(VRoadL, LandFloor) : VRoadL;
			LandProbeMin = FMath::Min(LandProbeMin, VPost);
			if (VPost < 0.85f * LandProbeV) LandProbeLowT += Dt;   // time spent more than 15% under the flight speed
			static IConsoleVariable* AutoL = IConsoleManager::Get().FindConsoleVariable(TEXT("ft.Autopilot"));
			if (LandProbeT <= 0.f && AutoL && AutoL->GetInt() > 0)
				UE_LOG(LogFT, Display, TEXT("FTLANDED D=%.0f air=%.0f now=%.0f kept=%.0f%% min=%.0f dip=%.0f%% low=%.2fs target=%.0f wheels=%d"), D, LandProbeV, VRoadL,
					100.f * VRoadL / FMath::Max(LandProbeV, 1.f), LandProbeMin, 100.f * FMath::Max(0.f, LandProbeV - LandProbeMin) / FMath::Max(LandProbeV, 1.f), LandProbeLowT, Drive.TargetSpeed, WheelsDown);
		}
		// 4.6: a glancing barrier touch keeps the speed along the road (Tick sets WallFloor; only ever adds speed back)
		if (WallHoldT > 0.f && !bWrecked)
		{
			const float VRoadW = FVector::DotProduct(Chassis->GetPhysicsLinearVelocity(), F.Fwd);
			if (VRoadW < WallFloor) Chassis->SetPhysicsLinearVelocity(Chassis->GetPhysicsLinearVelocity() + F.Fwd * (WallFloor - VRoadW));
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
		const bool bSweep = (F.Flags & FTFlag::Twist) || ((F.Flags & FTFlag::Pipe) && CVarFTPipeSweep.GetValueOnGameThread());
		if (bSweep)
		{
			const float VLatR = FVector::DotProduct(Vc, F.Right);
			if (F.Flags & FTFlag::Twist)
			{
				TwCp = FMath::Clamp(-X * 22.f - VLatR * 9.f, -12000.f, 12000.f);
				Chassis->AddForce(F.Right * TwCp, NAME_None, true);
			}
			// 3.6: and the car rides with the deck's sweep. Off the centreline a rolling deck moves along its own normal
			// (roll rate x distance out), so a car driving straight was struck by it (Level 2's second corkscrew, entered at
			// turbo speed: ~8,200 -> ~2,700 cm/s for a frame in every run) or left hanging. Near the surface, its speed along
			// the normal is the deck's own there. (Tried in the fast pipes too: no clear gain, left out. 4.5: in again for
			// pipes, ft.PipeSweep: off the centreline at turbo speed Level 7's pipe dropped away from the car every 2-3 frames.)
			if (Lift < 80.f)
			{
				const FVector Sweep = (F2.Pos + F2.Right * X) - (F.Pos + F.Right * X);
				const float VUp = FVector::DotProduct(Sweep, F.Up) / 100.f * FMath::Max(FVector::DotProduct(Vc, F.Fwd), 0.f);
				if (FMath::Abs(VUp) > 300.f)
				{
					TwSweep = VUp - FVector::DotProduct(Vc, F.Up);
					Chassis->SetPhysicsLinearVelocity(Chassis->GetPhysicsLinearVelocity() + F.Up * TwSweep);
				}
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
			TwPut = true; TwLift = Lift;
			Chassis->SetWorldTransform(Track->TransformAt(D, Xc, Track->SurfaceH(D, Xc) + ChassisZ + 6.f), false, nullptr, ETeleportType::TeleportPhysics);
			FVector Vput = F.Fwd * Vr;
			// 4.5: in a twist or a pipe the deck off the centreline sweeps along its own normal (roll rate x distance out:
			// ~4,700 cm/s at X 1,200 in Level 7's fast pipe). Put back with only the road's forward speed, the deck fell
			// away from it again in 2-3 frames: 32 put-backs in 1.5 s there (a stutter). It leaves with the deck's own motion
			if (bSetP && (F.Flags & (FTFlag::Twist | FTFlag::Pipe)) && CVarFTPipeSweep.GetValueOnGameThread())
			{
				const FFTFrame F2 = Track->Sample(D + 100.f);
				const FVector Sweep = (F2.Pos + F2.Right * Xc) - (F.Pos + F.Right * Xc);
				Vput += F.Up * (FVector::DotProduct(Sweep, F.Up) / 100.f * Vr);
			}
			Chassis->SetPhysicsLinearVelocity(Vput);
			Chassis->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);
			if (bInSet) ++SetSnaps;
			return;
		}
	}

	// 4.6 crest follow (physics run 2). Over a crest (or on a pipe's inner wall) the surface curves away from the car, and
	// following it takes v^2 x curvature toward it: ~8 g at turbo speed over Level 8's hilltops. The glue only pulled in
	// proportion to the lift already gained, so the car settled ~20-60 cm up with every wheel off for 0.3-0.7 s (FTJUMP
	// kind=road, rise 0: no grip, no steering). That pull is now fed forward from the surface's own curve under the car,
	// measured 2.5 m either side along its path; the glue and the springs only handle what is left.
	CrestA = 0.f;
	const int32 LowFps = CVarFTLowFpsGlue.GetValueOnGameThread();
	float TrBump = 0.f, TrPull = 0.f, TrPullCap = -1.f;
	if (CVarFTCrestFollow.GetValueOnGameThread() && (AM & 64) && !bWrecked && !bRealJump && !bCoaster && bSolid
		&& Lift < ((LowFps & 16) ? 100.f * FMath::Max(1.f, Dt * 60.f) : 100.f) && Lift > -60.f && FMath::Abs(X) < FT::Edge)
	{
		constexpr float L = 250.f;
		auto SurfP = [this](float Dd) { const FFTFrame Fs = Track->Sample(Dd); return Fs.Pos + Fs.Right * X + Fs.Up * Track->SurfaceH(Dd, X); };
		const FVector P0 = F.Pos + F.Right * X + F.Up * SurfH;
		float Kn = FVector::DotProduct(SurfP(D + L) + SurfP(D - L) - 2.f * P0, Up) / (L * L);
		const float Vr = FMath::Max(FVector::DotProduct(Chassis->GetPhysicsLinearVelocity(), F.Fwd), 0.f);
		// 4.9 (ft.LowFpsGlue 1): the pull is held for the whole frame, so it follows the curve over the stretch the car
		// covers in it (the change of the surface's slope from here to there), not the curve under it now: at 12 fps
		// and turbo speed that stretch is 8 m, over a crest only 5-10 m long
		const float Span = Vr * Dt;
		if ((LowFps & 1) && Span > L)
		{
			auto SlopeAt = [&](float Dd) { return FVector::DotProduct(SurfP(Dd + L) - SurfP(Dd - L), Up) / (2.f * L); };
			Kn = (SlopeAt(D + Span) - SlopeAt(D)) / Span;
		}
		if (Kn < 0.f)
		{
			CrestA = FMath::Min(-Kn * Vr * Vr, 15000.f);
			Chassis->AddForce(-Up * CrestA, NAME_None, true);
		}
	}
	// 4.6 pipe hold. In a pipe the road rolls round the tube's axis 7.3 m over it, so the road itself (its centreline too)
	// is carried round that axis: at turbo speed in Level 7's fast pipe its surface slides sideways at roll rate x 7.3 m,
	// ~3,000-5,000 cm/s. The grip assist only ever drove the car's sideways speed in the WORLD to zero, so it held the car
	// still while the road slid out from under it: the car ended up at X ~1,200 against the barrier, bounced off the deck
	// (FTPUTBACK lift 300+, ~6 a run) and was snapped back ~140 times per pass. In a pipe the grip, the off-wheels damping
	// and the barrier's put-back now work against the surface's own sideways motion (PipeLatV), and the surface path's
	// sideways acceleration under the car is fed forward (PipeA).
	PipeA = 0.f;
	float PipeLatV = 0.f;
	// 4.7 pipe heading (ft.PipeHead, Level 7's pipe along the barrier): the frame's forward in a pipe is the TUBE's axis, but
	// the road surface at the car's x runs at an angle to it (atan(7.3 m x roll per metre): 20-30 degrees at Level 7's fast
	// pipe). The yaw assist, the autopilot and the air control all aimed the nose down the axis, so a car holding its lane
	// drove crabwise (the 4.6 grip pushed it along with the surface) and the Chaos tyres, which know nothing of that, kept
	// dragging it toward where its nose pointed: off the lane into the barrier, and in the v470 survey two 177-degree spins
	// in that pipe at turbo speed (45-60% of the speed lost). PathF is the surface's own direction under the car; with
	// ft.PipeHead the nose follows it (plus the rate it turns at), the grip no longer needs the surface's slide, and the
	// sideways feed-forward is no longer cut at 20,000 cm/s^2 (at the barrier the turn takes ~50,000).
	PathF = F.Fwd; PathYawRate = 0.f;
	const bool bPipeHead = CVarFTPipeHead.GetValueOnGameThread() != 0;
	if ((F.Flags & FTFlag::Pipe) && !(F.Flags & FTFlag::Twist) && !bWrecked && !bCoaster && FMath::Abs(X) < FT::Edge + 500.f)
	{
		constexpr float L = 250.f;
		auto SurfP = [this](float Dd) { const FFTFrame Fs = Track->Sample(Dd); return Fs.Pos + Fs.Right * X + Fs.Up * Track->SurfaceH(Dd, X); };
		const FVector P0 = F.Pos + F.Right * X + F.Up * SurfH;
		const FVector Pa = SurfP(D + L), Pb = SurfP(D - L);
		const float Vr = FMath::Max(FVector::DotProduct(Chassis->GetPhysicsLinearVelocity(), F.Fwd), 0.f);
		const FVector Pf = (Pa - Pb) - Up * FVector::DotProduct(Pa - Pb, Up);
		if (Pf.SizeSquared() > 1.f) PathF = Pf.GetSafeNormal();
		if (CVarFTPipeHold.GetValueOnGameThread())
		{
			PipeLatV = FVector::DotProduct(Pa - Pb, F.Right) / (2.f * L) * Vr;
			if ((AM & 64) && !bRealJump && Lift < 150.f)
			{
				const float Cap = bPipeHead ? 60000.f : 20000.f;
				PipeA = FMath::Clamp(FVector::DotProduct(Pa + Pb - 2.f * P0, F.Right) / (L * L) * Vr * Vr, -Cap, Cap);
				Chassis->AddForce(F.Right * PipeA, NAME_None, true);
			}
		}
		if (bPipeHead)
		{
			// how fast the surface's direction turns about the road's normal as the car runs along it
			auto Ang = [&](const FVector& Dv) { return FMath::Atan2(FVector::DotProduct(Dv, F.Right), FVector::DotProduct(Dv, F.Fwd)); };
			PathYawRate = (Ang(Pa - P0) - Ang(P0 - Pb)) / L * Vr;
		}
#if !UE_BUILD_SHIPPING
		// 4.8 tests: ft.PipeTrace 1 = one line a frame in a pipe (Level 7's fast pipe at low fps)
		if (CVarFTPipeTrace.GetValueOnGameThread() > 0)
		{
			const FVector Vt = Chassis->GetPhysicsLinearVelocity();
			UE_LOG(LogFT, Display, TEXT("FTPIPE dt=%.3f D=%.0f X=%.0f lift=%.0f wh=%d vr=%.0f vlatR=%.0f vup=%.0f surfV=%.0f pipeA=%.0f yawR=%.2f"),
				Dt, D, X, Lift, WheelsDown, Vr, FVector::DotProduct(Vt, F.Right) - PipeLatV, FVector::DotProduct(Vt, Up), PipeLatV, PipeA, PathYawRate);
		}
#endif
	}
	const FVector HeadRef = bPipeHead ? PathF : F.Fwd;

	// 4.5 bump stop (physics run, owner 2026-11-16). At speed the glue, the track gravity's speed term and the aero push
	// past what the springs hold (~2 g at full compression), so a dip or a landing pressed the car through their 14 cm onto
	// its belly box (lift -55; v450 survey: 11 such "flights" on Level 1 and 17 on Level 2 in 90 s, 0.3-1.4 s each). Down
	// there the wheel traces start under the road and read "in the air": no grip, no governor, no steering, the belly
	// scraping the deck (FTBUMP deck hits, -25 to -35% speed with the turbo on) and landing sparks on flat road. Now the
	// chassis stops at BumpStopLift: its speed into the road is taken out there and it eases back up, so the wheels keep
	// the road. Only the speed along the road's normal is touched, never the speed along it.
	if (CVarFTBumpStop.GetValueOnGameThread() && !bWrecked && !bCoaster && bSolid && !(F.Flags & FTFlag::Gap) && FMath::Abs(X) < FT::Edge
		&& Lift < BumpStopLift && Lift > -120.f)
	{
		FVector Vb = Chassis->GetPhysicsLinearVelocity();
		const float Into = FVector::DotProduct(Vb, Up);   // < 0: still sinking
		float WantOut = FMath::Min((BumpStopLift - Lift) * 12.f, 500.f);
		// (4.9, ft.LowFpsGlue 4: at most back to the stop by the frame's end, the springs do the rest; at 12 fps the x12 and a jump to ride height threw it back up 50 cm)
		if (LowFps & 4) WantOut = FMath::Min(WantOut, (BumpStopLift - Lift) / FMath::Max(Dt, 1e-3f));
		if (Into < WantOut)
		{
			TrBump = WantOut - Into;
			Vb += Up * (WantOut - Into);
			Chassis->SetPhysicsLinearVelocity(Vb);
			++StopFrames;
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
		// (4.9, ft.LowFpsGlue 8: 35 cm per 60th of a second, not per frame: at 12 fps a frame's 35 cm was 420 cm/s, and a
		// car off a crest whose road fell away faster than that was never caught: v490 rf0_L8, 9.5 m up, on its roof)
		if (Lift > StickMaxLift && WheelsDown == 0 && !bWrecked)
		{
			const float SnapMax = (LowFps & 8) ? 35.f * FMath::Max(1.f, Dt * 60.f) : 35.f;
			const FVector P = Chassis->GetComponentLocation() - Up * FMath::Min(Lift - 8.f, SnapMax);
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
			const bool bSet = (F.Flags & (FTFlag::Pipe | FTFlag::Loop | FTFlag::Twist)) != 0;
			// 4.9 (ft.LowFpsGlue 32): "coming toward the road" is judged against the road under the stretch this frame
			// covers. Over a crest at 12 fps the road ahead falls away 1-2 m in one frame; a car sinking at 1,800 cm/s
			// still lost ground to it, but the damping read it as closing fast and pulled nothing (v490 rf0_L8 D 98,969)
			float AwayC = Away;
			if ((LowFps & 32) && !bSet && Dt > 0.025f && bSolid)
			{
				const float Vr = FMath::Max(FVector::DotProduct(Vg, F.Fwd), 0.f);
				const FFTFrame Fa = Track->Sample(D + Vr * Dt);
				const FVector Pa = Fa.Pos + Fa.Right * X + Fa.Up * Track->SurfaceH(D + Vr * Dt, X);
				const FVector P0 = F.Pos + F.Right * X + F.Up * SurfH;
				// (how far the road there lies under this point's tangent plane: 0 on a straight slope)
				const float Drop = -FVector::DotProduct(Pa - P0, Up);
				AwayC = Away + FMath::Max(Drop, 0.f) / Dt;
			}
			float DampA = AwayC > 0.f ? FMath::Min(AwayC * GlueDamp, AwayC / FMath::Max(Dt, 1e-3f)) : AwayC * GlueDamp;
			// in pipes, loops and twists the surface itself turns away from the car, so "coming toward the road"
			// (Away < 0) says nothing about closing in on it: there the spring always pulls in full, up to 20 g
			if (bSet) DampA = FMath::Max(DampA, 0.f);
			float Spring = FMath::Clamp(GlueK * FMath::Max(Lift, 0.f) + DampA, 0.f, bSet ? 20000.f : 12000.f);
			// 4.9 (ft.LowFpsGlue 2): held for the whole frame, the spring is explicit: at 12 fps GlueDamp x Dt = 2, and a pull
			// sized for where the car is now drove it through its springs by the frame's end (the next frame's bump stop
			// threw it back up: wheels chattering +-14 cm, "flights" with lift ~0). Never more than what brings it to ride
			// height by the frame's end (x + v t - a t^2 / 2 = 0); binds only below ~40 fps.
			if ((LowFps & 2) && !bSet && Dt > 0.025f)
			{
				TrPullCap = FMath::Max(0.f, 2.f * (Lift + AwayC * Dt) / (Dt * Dt));
				Spring = FMath::Min(Spring, TrPullCap);
			}
			Pull += Spring;
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
				const float Out = (FVector::DotProduct(Vg, F.Right) - PipeLatV) * FMath::Sign(X);   // (4.6: against the surface, which slides in a pipe)
				if (Out > 0.f) { Vg -= F.Right * FMath::Sign(X) * Out; Chassis->SetPhysicsLinearVelocity(Vg); }
				TwBack = -FMath::Sign(X) * Back; TwOut = Out > 0.f ? -FMath::Sign(X) * Out : 0.f;
				if (bInSet) ++SetSnaps;
			}
			float VLatR = FVector::DotProduct(Vg, F.Right) - PipeLatV;
			// 4.9 pipe edge (ft.PipeEdge, LOOP-REPORT 41 target 2). Level 7's fast pipe at turbo speed: past X ~900 the deck
			// sweeps along its own normal at up to 100 m/s and the pull that keeps the car on the curving surface there passes
			// 60,000 cm/s^2; the car swung across the pipe at 2,000-3,000 cm/s (vlatR), was struck by the deck, thrown 3-4 m
			// up, put back (speed climbing to 17,000) and spun (v480 pw24: 11 spins, 21 put-backs in 24 passes). In a pipe the
			// lane now ends at ft.PipeEdge: past it the outward speed (against the surface) is taken out, a stiff spring
			// holds it, and 2 m further on it is put back.
			const float PE = CVarFTPipeEdge.GetValueOnGameThread();
			const bool bPipeEdge = PE > 0.f && (F.Flags & FTFlag::Pipe) && !(F.Flags & (FTFlag::Twist | FTFlag::Loop)) && !bWrecked;
			const float LaneEdge = bPipeEdge ? FMath::Min(PE, 900.f) : 900.f;
			if (bPipeEdge && FMath::Abs(X) > LaneEdge)
			{
				if (FMath::Abs(X) > LaneEdge + 200.f)
				{
					const float Back = FMath::Abs(X) - (LaneEdge + 150.f);
					Chassis->SetWorldLocation(Chassis->GetComponentLocation() - F.Right * FMath::Sign(X) * Back, false, nullptr, ETeleportType::TeleportPhysics);
					if (bInSet) ++SetSnaps;
				}
				const float Out = VLatR * FMath::Sign(X);
				if (Out > 0.f) { Vg -= F.Right * FMath::Sign(X) * Out; Chassis->SetPhysicsLinearVelocity(Vg); VLatR -= FMath::Sign(X) * Out; }
			}
			const float Over = FMath::Sign(X) * FMath::Max(FMath::Abs(X) - LaneEdge, 0.f);
			// damp the sideways speed while off the wheels, and always when it is heading out past 9 m
			const bool bOutward = Over * VLatR > 0.f;
			const float LatA = (bPipeEdge ? 4.f : 1.f) * GlueK * Over + ((!bGrounded || bOutward) ? GlueDamp * VLatR : 0.f);
			TwLat = -FMath::Clamp(LatA, bPipeEdge ? -60000.f : -20000.f, bPipeEdge ? 60000.f : 20000.f);
			if (TwLat != 0.f) Chassis->AddForce(F.Right * TwLat, NAME_None, true);
		}
		Chassis->AddForce(-Up * Pull, NAME_None, true);
		TrPull = Pull;
	}
#if !UE_BUILD_SHIPPING
	// 4.9 tests: ft.GlueTrace 1 = one line a frame on ordinary road (road flights at low fps)
	if (CVarFTGlueTrace.GetValueOnGameThread() > 0 && !(F.Flags & (FTFlag::SetPiece | FTFlag::Coaster)) && !bCoaster)
	{
		const FVector Vt = Chassis->GetPhysicsLinearVelocity();
		auto Su = [this](int32 i) { return Vehicle->Wheels.IsValidIndex(i) && Vehicle->Wheels[i] ? Vehicle->Wheels[i]->GetSuspensionOffset() : 0.f; };
		auto Ia = [this](int32 i) { return Vehicle->Wheels.IsValidIndex(i) && Vehicle->Wheels[i] ? (int32)Vehicle->Wheels[i]->IsInAir() : 9; };
		UE_LOG(LogFT, Display, TEXT("FTGT dt=%.3f D=%.0f X=%.0f lift=%.1f vn=%.0f v=%.0f wh=%d air=%d%d%d%d susp=%.0f,%.0f,%.0f,%.0f pull=%.0f cap=%.0f crest=%.0f bump=%.0f jump=%d fl=%d up=%.3f fz=%.3f aim=%.0f gf=%d"),
			Dt, D, X, Lift, FVector::DotProduct(Vt, Up), FVector::DotProduct(Vt, F.Fwd), WheelsDown, Ia(0), Ia(1), Ia(2), Ia(3), Su(0), Su(1), Su(2), Su(3),
			TrPull, TrPullCap, CrestA, TrBump, bFlagJump ? 1 : 0, (int32)F.Flags, FVector::DotProduct(Chassis->GetUpVector(), Up), FVector::DotProduct(Chassis->GetForwardVector(), Up), AimA, bGapFlight ? 1 : 0);
	}
#endif

	const FVector CarF = Chassis->GetForwardVector();
	const FVector Cf = (CarF - Up * FVector::DotProduct(CarF, Up)).GetSafeNormal();
	const FVector Cr = FVector::CrossProduct(Up, Cf);
	const FVector V = Chassis->GetPhysicsLinearVelocity();
	const float VAlong = FVector::DotProduct(V, Cf);
	const float VLat = FVector::DotProduct(V, Cr);
	FVector W = Chassis->GetPhysicsAngularVelocityInRadians();
	float YawRate = FVector::DotProduct(W, Up);
	const float HeadErr = FMath::Atan2(FVector::DotProduct(FVector::CrossProduct(HeadRef, Cf), Up), FVector::DotProduct(Cf, HeadRef));   // (4.7: the surface's path in a pipe)
	// 5.0 pipe kick (ft.PipeKick, LOOP-REPORT 43 target 2). The last Level 7 fast-pipe spins at 24 fps start 0.08-0.12 s after
	// the chassis meets the deck (its normal sweeps through a car off the centreline): one frame the yaw rate goes -0.9 -> +6.2
	// rad/s against a wanted -2.1, the nose is 38-55 degrees off the path a frame later and the car spins. For 0.15 s after
	// such a strike (in a pipe, not drifting) the yaw rate is held within 1 rad/s of turning back onto the surface's path.
	// (only below 40 fps: at 60 fps the hold fought the yaw assist and spun the car in 12 of 16 passes; there the strike never spun it)
	if (CVarFTPipeKick.GetValueOnGameThread() && Dt > 0.025f && (F.Flags & FTFlag::Pipe) && !(F.Flags & (FTFlag::Twist | FTFlag::Loop)) && !bWrecked && !bCoaster
		&& Drive.DriftDir == 0 && LastHitTrack && FMath::Abs(LastHitUp) > 0.5f && GetWorld() && GetWorld()->GetTimeSeconds() - LastHitT < 0.15f)
	{
		const float Tgt = PathYawRate - FMath::Clamp(HeadErr * 3.f, -3.f, 3.f);
		const float Held = FMath::Clamp(YawRate, Tgt - 1.f, Tgt + 1.f);
		if (Held != YawRate)
		{
			static IConsoleVariable* AutoK = IConsoleManager::Get().FindConsoleVariable(TEXT("ft.Autopilot"));
			if (AutoK && AutoK->GetInt() > 0)
				UE_LOG(LogFT, Display, TEXT("FTPIPEKICK D=%.0f X=%.0f yawrate=%.2f -> %.2f herr=%.0f hitAgo=%.2f wheels=%d"), D, X, YawRate, Held, FMath::RadiansToDegrees(HeadErr), GetWorld()->GetTimeSeconds() - LastHitT, WheelsDown);
			W += Up * (Held - YawRate);
			YawRate = Held;
			Chassis->SetPhysicsAngularVelocityInRadians(W);
		}
	}

	DriftBlend = FMath::FInterpTo(DriftBlend, Drive.DriftDir != 0 ? 1.f : 0.f, Dt, 6.f);
	const float RearFr = FMath::Lerp(RearFriction, DriftRearFriction, DriftBlend);
	Vehicle->SetWheelFrictionMultiplier(2, RearFr);
	Vehicle->SetWheelFrictionMultiplier(3, RearFr);

	// Chaos inputs: the engine drives the rear wheels; the governor below keeps the arcade speed
	const float Gap = Drive.TargetSpeed - VAlong;
	// 4.6 unstick: pinned nose-first into the barrier (Tick's NoseT), the throttle only held the nose in and the yaw assist's
	// torque lost to the barrier's friction (FTSTALL: 1.2 s stuck, then a put-back 4 m on). Throttle off, the car is turned
	// back along the road and eased 3 m/s off the barrier, then drives away by itself.
	const bool bUnstick = NoseT > 0.15f && FMath::Abs(HeadErr) > 0.3f;
	if (bUnstick)
	{
		FVector Wu = W + Up * (FMath::Clamp(-HeadErr * 5.f, -4.f, 4.f) - YawRate);
		Chassis->SetPhysicsAngularVelocityInRadians(Wu);
		FVector Vu = V;
		const float Out = -FMath::Sign(X);
		const float VLatRoad = FVector::DotProduct(V, F.Right);
		if (VLatRoad * Out < 300.f) Vu += F.Right * (Out * 300.f - VLatRoad);
		Chassis->SetPhysicsLinearVelocity(Vu);
		static IConsoleVariable* AutoU = IConsoleManager::Get().FindConsoleVariable(TEXT("ft.Autopilot"));
		if (!bUnstickLogged && AutoU && AutoU->GetInt() > 0)
		{
			bUnstickLogged = true;
			UE_LOG(LogFT, Display, TEXT("FTUNSTICK D=%.0f X=%.0f herr=%.0f v=%.0f after=%.2fs"), D, X, FMath::RadiansToDegrees(HeadErr), VAlong, NoseT);
		}
	}
	Vehicle->SetThrottleInput(bUnstick ? 0.f : FMath::Clamp(Gap / 900.f + 0.35f, 0.f, 1.f));
	Vehicle->SetBrakeInput(!bUnstick && Gap < -1500.f && VAlong > 800.f && !bInSet && LandHoldT <= 0.f ? 0.4f : 0.f);
	Vehicle->SetSteeringInput(Drive.Steer * (Drive.DriftDir != 0 ? 0.25f : 0.55f));

	if (bGrounded)
	{
		// 2.6 (spec 05 §1): speed moves toward the target at the stated rate (420 / 900 su/s, by the Gearbox), both
		// ways; a stiff gain so it reaches the target instead of settling under it
		const float A = FMath::Clamp(Gap * 8.f, (bInSet || LandHoldT > 0.f) ? 0.f : -Drive.Accel, Drive.Accel);
		if ((AM & 1) && !bUnstick) { Chassis->AddForce(Cf * A, NAME_None, true); TwGov = A * FVector::DotProduct(Cf, F.Right); }

		// grip: remove sideways slip unless sliding on purpose
		const float LatTarget = Drive.DriftDir != 0 ? Drive.DriftTargetLat : 0.f;
		const float GripNow = FMath::Lerp(Grip, 2.2f, DriftBlend);
		// (4.6: in a pipe the slip is against the surface, which slides sideways as the road rolls round the tube)
		const float SurfLat = bPipeHead ? 0.f : PipeLatV * FVector::DotProduct(F.Right, Cr);   // (4.7: none needed once the nose follows the surface)
		if (AM & 2) { const float GA = FMath::Clamp((LatTarget + SurfLat - VLat) * GripNow, -9000.f, 9000.f); Chassis->AddForce(Cr * GA, NAME_None, true); TwGrip = GA * FVector::DotProduct(Cr, F.Right); }

		// yaw: steer rate, plus following the bend, plus drift angle
		const float SK = FMath::Clamp(FMath::Abs(VAlong) / 9600.f, 0.f, 1.f);
		const float RoadRate = F.Curve * 1e-3f / 1.5f / FT::DU * VAlong;
		float Want = Drive.Steer * FMath::Lerp(SteerYawRateLow, SteerYawRateHigh, SK) + RoadRate * RoadFollow + PathYawRate;
		if (Drive.DriftDir != 0)
		{
			const float Angle = Drive.DriftDir * FMath::DegreesToRadians(24.f) + Drive.Steer * FMath::DegreesToRadians(12.f);
			Want = RoadRate + PathYawRate + (Angle - HeadErr) * 2.4f + Drive.Steer * 0.4f;
		}
		else
		{
			Want += -HeadErr * 2.4f * (1.f - FMath::Abs(Drive.Steer)) - HeadErr * 0.6f;
			// never let it swing more than ~40 degrees off the road unless drifting
			if (FMath::Abs(HeadErr) > 0.7f) Want = RoadRate + PathYawRate - FMath::Sign(HeadErr) * 2.5f;
		}
		TwWant = Want; TwYawA = FMath::Clamp((Want - YawRate) * 11.f, -40.f, 40.f);
		if (AM & 4) Chassis->AddTorqueInRadians(Up * TwYawA, NAME_None, true);
#if !UE_BUILD_SHIPPING
		if (CVarFTPipeTrace.GetValueOnGameThread() > 0 && (F.Flags & FTFlag::Pipe))
			UE_LOG(LogFT, Display, TEXT("FTPIPE2 D=%.0f herr=%.1f steer=%.2f want=%.2f yawrate=%.2f pathYawR=%.2f vlat=%.0f surfLat=%.0f valong=%.0f"),
				D, FMath::RadiansToDegrees(HeadErr), Drive.Steer, Want, YawRate, PathYawRate, VLat, SurfLat, VAlong);
#endif
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
		const float YawErr = FMath::Atan2(FVector::DotProduct(FVector::CrossProduct(Chassis->GetForwardVector(), HeadRef), CarUp), FVector::DotProduct(Chassis->GetForwardVector(), HeadRef));
		const FVector Tq = Axis * 9.f + CarUp * (YawErr * 2.5f + Drive.Steer * 1.2f) - W * 2.2f;
		if (AM & 32) Chassis->AddTorqueInRadians(Tq, NAME_None, true);
		// you always clear a gap: keep enough speed to make the far side
		if (bGap && VAlong < 6000.f) Chassis->AddForce(F.Fwd * 2500.f, NAME_None, true);
		// 2.5: off its wheels inside a set piece the engine still drives it on (the floor above holds the rest)
		if (bInSet && (AM & 1)) Chassis->AddForce(F.Fwd * FMath::Clamp(Gap * 2.6f, 0.f, Drive.Accel), NAME_None, true);
	}
}
