// The KESTREL-88: a Chaos Vehicles car (real suspension, tyres, weight transfer and collisions) with an
// arcade layer on top: speed governor, steering and grip assists, kart-style drift on the rear tyres,
// and "track gravity" that holds it to the road through loops, pipes and corkscrews.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ChaosVehicleWheel.h"
#include "FTPlayerCar.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UChaosWheeledVehicleMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UPointLightComponent;
class USpotLightComponent;
class UAudioComponent;
class AFTTrack;

UCLASS()
class FULLTHROTTLE_API UFTWheelRear : public UChaosVehicleWheel
{
	GENERATED_BODY()
public:
	UFTWheelRear();
};

UCLASS()
class FULLTHROTTLE_API UFTWheelFront : public UChaosVehicleWheel
{
	GENERATED_BODY()
public:
	UFTWheelFront();
};

/** What the race asks of the car this frame. */
struct FFTDrive
{
	float Steer = 0.f;        // -1..1
	float TargetSpeed = 0.f;  // cm/s along the road
	float Accel = 5760.f;     // cm/s^2 toward the target
	bool bHot = false;        // turbo or drift boost
	int32 DriftDir = 0;       // -1 left, +1 right, 0 not sliding
	float DriftTargetLat = 0.f; // cm/s sideways slide wanted while drifting
	bool bFrozen = false;     // cards, retake resets: hold still
	bool bFlip = false;       // 3.3 half pipe air: the drift button rolls the car (a barrel roll)
	int32 DriftLevel = 0;     // FX overhaul: the drift's charge (0 none, 1 blue, 2 orange, 3 pink), for its sparks: looks only
};

/** 3.3 half pipe: off the lip into the air (tricks), or grinding along the coping. */
enum class EFTHalf : uint8 { None, Air, Grind };

UCLASS()
class FULLTHROTTLE_API AFTPlayerCar : public APawn
{
	GENERATED_BODY()
public:
	AFTPlayerCar();

	/** The physics body: a box chassis the Chaos vehicle drives (the KESTREL mesh rides on it). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Car") TObjectPtr<UBoxComponent> Chassis;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Car") TObjectPtr<UStaticMeshComponent> Body;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Car") TObjectPtr<UChaosWheeledVehicleMovementComponent> Vehicle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Car") TArray<TObjectPtr<UStaticMeshComponent>> WheelMeshes;
	/** Body panels that come off as the armour drops (wing, pods, mirrors, blower, light bar, bumper, diffuser). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Car") TArray<TObjectPtr<UStaticMeshComponent>> Parts;
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<UPointLightComponent> Underglow;
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<USpotLightComponent> HeadL;
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<USpotLightComponent> HeadR;
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<UNiagaraComponent> SmokeFX;
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<UNiagaraComponent> FireFX;
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<UNiagaraComponent> SparksFX;
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<UNiagaraComponent> TurboFX;
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<UNiagaraComponent> DriftFX;
	// FX overhaul: the drift's charge as sparks off the rear tyres, and the hot core of the turbo flame
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<UNiagaraComponent> DriftSparkFX;
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<UNiagaraComponent> BoostCoreFX;
	// FX round 3: what the rear wheels throw off the road (by the level's surface: the run of it, and the rooster tail of
	// a slide), the banded turbo flame that stands in for TurboFX's additive tuft, the exhaust at top speed, the speed lines
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<UNiagaraComponent> WheelFX;
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<UNiagaraComponent> WheelFX2;
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<UNiagaraComponent> TurboCelFX;
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<UNiagaraComponent> ExhaustFX;
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<UNiagaraComponent> SpeedFXL;
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<UNiagaraComponent> SpeedFXR;
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<UAudioComponent> EngineAudio;
	UPROPERTY(VisibleAnywhere, Category = "Car") TObjectPtr<UAudioComponent> SkidAudio;
	UPROPERTY(VisibleAnywhere, Category = "Car") TArray<TObjectPtr<UStaticMeshComponent>> Henchmen;

	UPROPERTY(EditAnywhere, Category = "Car|Tuning") float SteerYawRateLow = 1.5f;   // rad/s at low speed
	UPROPERTY(EditAnywhere, Category = "Car|Tuning") float SteerYawRateHigh = 0.7f; // rad/s at top speed
	UPROPERTY(EditAnywhere, Category = "Car|Tuning") float RoadFollow = 0.55f;       // share of the bend the car follows by itself
	UPROPERTY(EditAnywhere, Category = "Car|Tuning") float Grip = 7.f;               // sideways slip removed per second
	UPROPERTY(EditAnywhere, Category = "Car|Tuning") float AirGravityScale = 1.7f;
	UPROPERTY(EditAnywhere, Category = "Car|Tuning") float RearFriction = 3.2f;
	UPROPERTY(EditAnywhere, Category = "Car|Tuning") float DriftRearFriction = 1.25f;
	// 2.4 "glued to the road": extra downforce (in g) on the wheels, and a critically damped spring that pulls a
	// lifted car back onto the road (GlueK: cm/s^2 per cm of lift; GlueDamp: 1/s on the speed away from the road).
	// Off on real jumps (the kicker and the gap of a jump section, and a ramp launch).
	UPROPERTY(EditAnywhere, Category = "Car|Tuning") float Downforce = 0.8f;
	UPROPERTY(EditAnywhere, Category = "Car|Tuning") float GlueK = 150.f;
	UPROPERTY(EditAnywhere, Category = "Car|Tuning") float GlueDamp = 24.5f;
	bool IsLaunched() const { return LaunchT > 0.f || HopT >= 0.f; }
	// 2.5 "stick to the road": anything over this much lift off the road (not on a real jump) is put back on it
	UPROPERTY(EditAnywhere, Category = "Car|Tuning") float StickMaxLift = 40.f;
	// 4.5 bump stop: the chassis never sinks deeper than this under ride height (cm, negative). The springs bottom out
	// at about -13 under the glue and track gravity at speed; past them the box sat on its belly (lift -55), the wheel
	// traces started under the road and read "in the air", and the car lost grip, the governor and its speed
	UPROPERTY(EditAnywhere, Category = "Car|Tuning") float BumpStopLift = -18.f;
	int32 StopFrames = 0;   // frames the bump stop held the car (FTGLUE telemetry)
	// 2.5 coaster: the spring that keeps the car on its rail (1/s^2) and its damping (1/s)
	UPROPERTY(EditAnywhere, Category = "Car|Tuning") float RailK = 36.f;
	UPROPERTY(EditAnywhere, Category = "Car|Tuning") float RailDamp = 12.f;
	bool IsOnRails() const { return bRailInit; }
	/** 4.7: the road surface's direction under the car (in a pipe, not the tube's axis; elsewhere the road's forward). */
	FVector RoadPathFwd() const { return PathF; }
	/** 3.3: which of the three coaster rails the car rides (0 left, 1 centre, 2 right), -1 off the rails. */
	int32 RailIndex() const;
	// 3.3 half pipe air and grind (the game mode scores them; it reads these and clears the events)
	EFTHalf Half = EFTHalf::None;
	int32 HalfSide = 0;          // which wall (-1 left, +1 right)
	float HalfT = 0.f;           // time in the air / on the grind
	float HalfDur = 0.f;         // the air's flight time
	float TrickYaw = 0.f, TrickRoll = 0.f;   // degrees turned in the air (spins, barrel rolls)
	bool bHalfLaunched = false, bHalfLanded = false, bHalfBailed = false, bGrindStart = false, bGrindEnd = false;
	float HalfPeak = 0.f;        // how high over the lip (cm)
	// 3.3 the body on its springs (looks only): roll into turns, squat and dive, a bounce on landings
	UPROPERTY(EditAnywhere, Category = "Car|Look") float LeanRollPerG = 3.2f;    // degrees of body roll per g sideways
	UPROPERTY(EditAnywhere, Category = "Car|Look") float LeanPitchPerG = 2.4f;   // degrees of squat/dive per g
	UPROPERTY(EditAnywhere, Category = "Car|Look") float BodyFreq = 11.f;        // spring (rad/s)
	UPROPERTY(EditAnywhere, Category = "Car|Look") float BodyDamp = 0.32f;       // damping ratio (under 1: it rocks)
	/** On a coaster where the rails sit side by side: hop to the other rail. Dir = -1 left, +1 right. */
	bool RailHop(int32 Dir);
	bool CanRailHop(int32 Dir) const;
	/** Speed held through the set piece the car is in (0 outside one). */
	float SetPieceFloor() const { return bInSet ? SetFloor : 0.f; }
	float SurfH = 0.f;   // height of the surface under the car over the road's frame (half pipe, coaster rail)

	void SetTrack(AFTTrack* InTrack) { Track = InTrack; }
	void SetDrive(const FFTDrive& In) { Drive = In; }

	// track-space state, refreshed every frame
	float D = 0.f, X = 0.f, H = 0.f;
	float Speed = 0.f;      // cm/s along the car
	float LatSpeed = 0.f;   // cm/s across the road (+ right)
	bool bGrounded = true;
	int32 WheelsDown = 4;
	float AirTime = 0.f;
	bool bWallLeft = false, bWallRight = false; // scraping a barrier this frame
	float WallImpact = 0.f;
	// Which side OF THE CAR is touching a barrier (0 = left door, 1 = right door), from where the hit landed on the body
	// rather than which half of the road the car is on (bWallLeft/Right). WallSideImpact is that side's peak impulse per
	// mass (cm/s) while the contact lasts; WallSideHits counts fresh contacts (a new one after the side went quiet).
	bool bWallSide[2] = { false, false };
	float WallSideImpact[2] = { 0.f, 0.f };
	int32 WallSideHits[2] = { 0, 0 };
	FVector RoadUp = FVector::UpVector;

	/** Put the car on the road at d, x, facing along it, at a given speed. */
	void PlaceOnTrack(float InD, float InX, float InSpeed);
	/** 4.6 tests (ft.NosePin): parked at the barrier 4 m on, nose turned Deg into it, crawling. */
	void NosePin(float Deg, int32 Side);
	void Shove(const FVector& WorldDir, float CmPerSec, float SpinRad = 0.f);
	void SetDamageLook(float Hp);           // smoke, fire, sparks, soot
	void ShedPart(int32 Index);
	void RestoreParts();
	int32 NumPartsOn() const;
	void SetHenchmenForLevel(int32 Level);
	void SetHenchmenVisible(bool bLeft, bool bRight, FVector2D Age = FVector2D::ZeroVector,
		FVector2D UntilPunch = FVector2D(2.f, 2.f), FVector2D SinceWallHit = FVector2D(10.f, 10.f));
	void Wreck();
	bool IsWrecked() const { return bWrecked; }
	void SetLightsForLevel(int32 Level);
	// 4.1: headlights by daylight: dimmed where the level is lit by a sun (ft.HeadDay), full under the moon
	void ApplyHeadlights();
	FTransform GunMuzzle(int32 Side) const;
	/** FX overhaul (looks only): the turbo lighting, or a drift boost letting go: long flames out of the pipes and a ring. */
	void BoostKick(float Scale);
	/** FX overhaul (looks only): the car was hurt: its lights surge for a moment (0..1). */
	void HitFlash(float Amount);

	virtual void Tick(float Dt) override;
	virtual void BeginPlay() override;

protected:
	void ApplyArcade(float Dt);
	UFUNCTION() void OnBodyHit(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(Transient) TObjectPtr<AFTTrack> Track;
	FFTDrive Drive;
	bool bWrecked = false;
	float WallTimer = 0.f;
	float WallSideT[2] = { 0.f, 0.f };   // per car side: a barrier touched it within the last moment
	float DriftBlend = 0.f;
	float CapsizeT = 0.f;
	float LaunchT = 0.f;   // a ramp launch is in the air: no glue until it lands
	// 2.5 set pieces: speed at the way in, held as a floor all the way through; telemetry
	float LandProbeT = 0.f, LandProbeV = 0.f, LandProbeMin = 0.f, LandProbeLowT = 0.f;   // 4.6 FTLANDED telemetry
	bool bLandArmed = false, bLandAbove = false, CushionLogged = false; float LandAirV = 0.f, LandFloor = 0.f, LandHoldT = 0.f; // 3.6: a jump's landing keeps its speed
	bool bInSet = false; uint16 SetKind = 0; float SetEntry = 0.f, SetFloor = 0.f, SetMin = 0.f, SetMaxLift = 0.f, SetAir = 0.f, SetD0 = 0.f; int32 SetSnaps = 0, SetCeilFrames = 0;
	// 2.5 coaster: the rail the car rides (followed by continuity: the rail point nearest the last one)
	bool bRailInit = false; FVector2D RailPt = FVector2D::ZeroVector; float HopCd = 0.f;
	FVector SurfN = FVector::UpVector;
	bool bWheelsOffStatic = false;   // 5.0 ft.WheelBarrier: the wheel traces ignore static geometry (lifted beside the barrier)
	float StallT = 0.f;
	// 4.6 barrier contact: a glancing hit keeps the speed the car had (WallFloor, held while it scrapes, eased to the
	// governor's speed); a nose pinned into the barrier is turned back along the road (NoseT). LastSpeed = last tick's.
	float LastSpeed = 0.f, WallFloor = 0.f, WallHoldT = 0.f, NoseT = 0.f;
	bool bGlanceHit = false, bNoseHit = false, bUnstickLogged = false; float NoseHitT = -10.f;
	float WallEpT = -1.f, WallEpV0 = 0.f, WallEpMin = 0.f, WallEpTgt = 0.f, WallEpD = 0.f, WallEpX = 0.f, WallEpFwd = 0.f; int32 WallEpHits = 0;
	float NoseProbeT = -1.f, NoseProbeD = 0.f;   // 4.6 ft.NosePin probe
	float CrestA = 0.f, PipeA = 0.f;   // 4.6: the crest and pipe feed-forwards this tick (cm/s^2, telemetry)
	// 4.7 FTSPIN telemetry: the last ~0.6 s (time, heading error, yaw rate, slip / steer, wheels, x, speed) and the spin under way
	TArray<FVector4f> SpinHist, SpinHist2;
	FVector PathF = FVector::ForwardVector; float PathYawRate = 0.f;   // 4.7: the road surface's own direction under the car (a pipe's isn't its axis), and how fast it turns
	float SpinT = -1.f, SpinMax = 0.f, SpinV0 = 0.f, SpinVMin = 0.f, SpinD = 0.f, SpinOkT = 0.f, LastLandT = -10.f;
	// 2.5 coaster: a hop's arc (HopT < 0: not hopping) from where it left
	float HopT = -1.f; FVector2D HopFrom = FVector2D::ZeroVector;
	static constexpr float HopDur = 0.85f;
	// 2.5 tests: what the car last hit (stall reports)
	FString LastHit; float LastHitT = 0.f; float LastHitUp = 1.f; bool LastHitTrack = false;
	TArray<FTransform> PartRest;
	bool bHangModel = false;
	// 3.3 half pipe air: where it left (x, height over the frame), its speed up and along
	float HalfX0 = 0.f, HalfH0 = 0.f, HalfVUp = 0.f, HalfSpeed = 0.f, HalfTilt0 = 0.f;
	bool TickHalfPipe(float Dt, const struct FFTFrame& F, float Slope);
	// 3.3 body springs: roll, pitch (degrees) and heave (cm), each with its rate
	float BodyRoll = 0.f, BodyRollV = 0.f, BodyPitch = 0.f, BodyPitchV = 0.f, BodyHeave = 0.f, BodyHeaveV = 0.f;
	FVector PrevVel = FVector::ZeroVector; bool bPrevVel = false;
	FVector AccF = FVector::ZeroVector;   // filtered acceleration in the car's frame
	void TickBody(float Dt);
	UPROPERTY(Transient) TArray<TObjectPtr<UStaticMeshComponent>> LooseParts;
	// FX overhaul: the effects' own bookkeeping (nothing here is read by the race)
	void TickFx(float Dt, const FVector& Velocity);
	int32 SparkLevel = 0;                      // which drift sparks are on
	float FxAirT = 0.f;                        // time off the wheels, for the landing
	float AlertAmt = 0.f, AlertShown = 0.f;    // the hit flash on the car's lights
	float WallHitCd = 0.f;
	float ShedQuietT = -1.f;                   // parts put back on a car that starts damaged come off without sparks
	int32 DamageStage = -1;                    // 0 clean, 1 smoking, 2 burning; -1 not looked at yet
	bool bFxStage = false;                     // parked for a slate or hidden for the reel: a new scene
	// FX round 3: the car in the world, and speed
	int32 SurfaceLevel = 0;                    // the level's surface (0..7): what the wheels throw up, a landing's kick
	bool bMarkOn[2] = { false, false };        // a rear tyre is laying a mark on the road...
	FVector MarkFrom[2] = { FVector::ZeroVector, FVector::ZeroVector };   // ...and where its next length starts
	FName TurboTuftSeen;                       // the tuft FTFlow last gave TurboFX (a nitro colour's, or the stock one)
	bool bTurboCel = false;                    // a banded flame stands in for that tuft (TurboCelFX)
	// FX round 4: what the body wears as its armour drops (dents, a cracked lamp, soot): flat marks on its own panels,
	// made in BeginPlay, shown by SetDamageLook in the order of FTPlayerCar.cpp's CarMarks
	UPROPERTY(Transient) TArray<TObjectPtr<UStaticMeshComponent>> DamageMarks;
	int32 MarksShown = 0;
	// 4.8: the chassis's own physical material (ft.BodySlide; the PM_CarBody asset it used to look for never existed)
	UPROPERTY(Transient) TObjectPtr<class UPhysicalMaterial> BodySlidePM;
	int32 BodySlideOn = -1;                    // the ft.BodySlide value last applied (-1 = not yet)
	float BodySlideMu = -1.f;                  // ...and its friction
	void ApplyBodySlide();
};
