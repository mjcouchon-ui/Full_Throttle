// Visual proxies for the race's track-space entities, and the physics wrecks they leave when they die.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FTCore.h"
#include "FTEntity.generated.h"

class UStaticMeshComponent;
class UNiagaraComponent;
class UPointLightComponent;
class UStaticMesh;

UCLASS()
class FULLTHROTTLE_API AFTEntityActor : public AActor
{
	GENERATED_BODY()
public:
	AFTEntityActor();
	UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> Extra;   // rotor, rope, chain
	UPROPERTY(VisibleAnywhere) TObjectPtr<UNiagaraComponent> Trail;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UPointLightComponent> Light;

	/** 3.3: separate wheels (spinning on the road), the struts up to the body, and extra moving bits (tail rotor,
	 *  the Tesla coil's top and arcs, the clown car's propellers). Made on demand, reused with the pooled actor. */
	UPROPERTY(Transient) TArray<TObjectPtr<UStaticMeshComponent>> Wheels;
	UPROPERTY(Transient) TArray<TObjectPtr<UStaticMeshComponent>> Struts;
	UPROPERTY(Transient) TArray<TObjectPtr<UStaticMeshComponent>> Bits;

	EFTKind Kind = EFTKind::None;
	int32 Variant = 0;
	int32 Lvl = 0;
	float AnimT = 0.f;
	void Setup(EFTKind InKind, int32 InVariant, int32 Level);
	void SetGlow(float Amount);   // rev lights, chain wind-up, brake flare
	/** Speed = along the road (cm/s), Vx = across it (cm/s): the wheels spin and the body rides its springs. */
	void Animate(float Dt, float Speed, float Vx = 0.f, float BikeWindup = 0.f);
	void KickBike();
	/** 3.3: give this actor the wheels listed for its mesh (Content/Data/wheels.json); false if it has none. */
	bool SetupWheels(UStaticMesh* BodyMesh, bool bChrome);
	/** 3.3 the Tesla truck: Charge 0..1 (glow, hum, spin), and up to 3 arcs from the coil's top to world points.
	 *  Overhaul: Rising = each strike still charging (0..1 through its own warning): a collar climbs the coil for each. */
	void SetTesla(float InCharge, const TArray<FVector>& ArcTo, const TArray<float>* Rising = nullptr);
	/** 3.3: a henchman's rope runs from his raised hand up to this world point (the chopper's door). */
	void SetRope(const FVector& WorldTop, float Progress, int32 Side);
	/** 3.3 a chopper's rappel arm at its door (Side 0 left, 1 right), in the world. */
	FVector RopeAnchor(int32 Side) const;
	/** The KESTREL ghost in the highlight reel: its body and its four wheels. */
	void SetupGhostCar(class AFTPlayerCar* From);
	struct FWheelDef { FVector C = FVector::ZeroVector; float R = 40.f, W = 30.f; };
	static const TArray<FWheelDef>* WheelsFor(const UStaticMesh* M);
	TArray<FWheelDef> WheelSpec;
	float WheelSpin = 0.f;
	// body springs (degrees, cm) and what drives them
	float SRoll = 0.f, SRollV = 0.f, SPitch = 0.f, SPitchV = 0.f, SHeave = 0.f, SHeaveV = 0.f;
	float PrevSp = 0.f, PrevVx = 0.f, AccLat = 0.f, AccLong = 0.f; bool bSusInit = false; float BumpPhase = 0.f;
	float TeslaCharge = 0.f, PropSpin = 0.f;
	bool bGhostCar = false;
	bool bFar = false;        // set by the game mode: far down the road, fewer updates
	int32 FarTick = 0;
	float LastGlow = -1.f;
	static UStaticMesh* MeshFor(EFTKind Kind, int32 Variant, int32 Level);
	static void WarmPickups(AActor* Owner);
	static void PickupBurst(UWorld* World, EFTKind Kind, const FTransform& At);
	/** 2.6 ammo: an AP (blue-white, thinner) or HE (orange) tracer; standard rounds keep the stock look. */
	void SetTracer(const FLinearColor& Colour, float Width, bool bSpecial);
	bool bTracerColoured = false;
	/** 2.8: the yeti's charge (0..1): he leans in and flails faster. Set by the game mode each frame. */
	float Charge = 0.f;
	float YetiPhase = 0.f;
	static constexpr float YetiShoulderZ = 820.f;   // where SM_YetiArms pivots (ft_v28.py SHOULDER_Z)

	// ---- 3.1: how the attacks look (nothing here is read by the race)
	UPROPERTY(VisibleAnywhere) TObjectPtr<UNiagaraComponent> Trail2;   // a second trail layer (a missile's flame)
	UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> Mark;  // where a jumper or a henchman is about to land
	/** A foe's round: their colour, their streak, a glow (Variant as the bullet's: 0 tracer, 1 swordfish,
	 *  2 fireball, 3 big tracer, 4 pie). Looks only: the round's speed and box are the game mode's. */
	void SetHostile(int32 InVariant);
	/** The road mark under something about to land on the car: WorldT on the road under it, the real half-extents
	 *  of what it hits (cm along and across), and how far through its fall it is (0..1). */
	void SetLandingMark(const FTransform& WorldT, float HalfAlong, float HalfAcross, float Progress);
	void HideLandingMark();
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	bool bHostile = false;
	float GlowOffT = 1.f, GlowOnT = 0.f;

	// ---- overhaul (15 Nov): the bosses' rigs (FTEntity.cpp, "the bosses' rigs"). Looks only: nothing here is read by
	// the race, and every pose is driven by the fight's own timers, never the other way round.
	/** Every frame, from the boss's own function: armour left (0..1) and the fight's phase (0, 1, 2). A drop in
	 *  armour is a hit (flash, lurch, sparks); a new phase is the next damage state. */
	void BossState(float HpFrac, int32 Phase);
	/** Every frame while an attack winds up: which one (per boss, from 1) and how far (0 as its timer starts, 1 as it
	 *  is released), and where across the road it is aimed, relative to the boss (cm). Not called: no wind-up. */
	void BossWind(int32 InKind, float Amount, float InAimX = 0.f);
	/** The frame an attack leaves the boss (Kind as BossWind's): recoil, and the flash at the part that fired. */
	void BossFire(int32 InKind, int32 Count = 1);
	/** Every frame while the boss's own move at the car lasts (the Dealer's brake check, the yeti's charge, the
	 *  shark's lunge): Amount 0..1 through its warning, bGoing once it is really coming back at the car. */
	void BossLunge(float Amount, bool bGoing);
	/** A value the boss's function keeps up to date for its model (per boss: a second wind-up on its own timer, where
	 *  a gun is tracking, ...). It holds until it is set again. */
	void BossCue(int32 Slot, float Value) { if (Slot >= 0 && Slot < 4) Cue[Slot] = Value; }
	float Cue[4] = { 0.f, 0.f, 0.f, 0.f };
	/** A moving part: the Bits slot I with this mesh at this place on the body (made on demand, as the Tesla top). */
	UStaticMeshComponent* Part(int32 I, const TCHAR* MeshPath, const FVector& At, bool bShadow = true);
	/** A looping effect of the rig (slot I of BossFx) on the body at At, switched on or off. */
	UNiagaraComponent* Loop(int32 I, const TCHAR* Name, const FVector& At, bool bOn, const FRotator& Rot = FRotator::ZeroRotator, float Scale = 1.f);
	UPROPERTY(Transient) TArray<TObjectPtr<UNiagaraComponent>> BossFx;
	bool bBossLive = false;                                // the fight's own boss (not a ghost in the reel, not the coaster's flyer)
	float BossHp = 1.f; int32 BossPhase = 0;
	float HurtT = 9.f, HurtAmt = 0.f, HurtFxT = 9.f;       // the last hit: seconds since, how hard (0..1); seconds since its sparks
	int32 DmgState = 0; float DmgT = 9.f;                  // the damage state shown (0, 1, 2), seconds since it changed
	int32 WindKind = 0; float Wind = 0.f, AimX = 0.f; bool bWindSet = false;
	int32 FireKind = 0, FireCount = 0; float FireT = 9.f;  // the last release
	float IntroT = 0.f;                                    // seconds since the boss arrived
	float BossAlert = 0.f, AlertSet = -9.f;                // the rig's own brightness of the lit parts; what the materials hold
	float RigA = 0.f, RigB = 0.f, RigC = 9.f, RigD = 0.f, RigE = 0.f;  // per-boss angles, phases and clocks
	float RigF = 0.f, RigG = 0.f, RigH = 0.f; int32 RigN = 0;
	float Lunge = 0.f; bool bLunge = false, bLungeSet = false;
	float Charges[4] = { 0.f, 0.f, 0.f, 0.f }; int32 NumCharges = 0, NumArcs = 0;
	float LastSpeed = 0.f;                                 // along the road (cm/s): what the pieces keep when it comes apart

private:
	float BikeHit = 0.f;
};

/** A dead vehicle: a real Chaos rigid body that tumbles down the road burning, then goes. */
UCLASS()
class FULLTHROTTLE_API AFTWreck : public AActor
{
	GENERATED_BODY()
public:
	AFTWreck();
	UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UNiagaraComponent> Fire;
	UPROPERTY(Transient) TArray<TObjectPtr<UStaticMeshComponent>> Wheels;   // 3.3: the wreck keeps its wheels
	UPROPERTY(VisibleAnywhere) TObjectPtr<UNiagaraComponent> Smoke;   // 3.1: it smokes as it tumbles (not at Low)
	void Launch(UStaticMesh* InMesh, const FTransform& At, const FVector& Velocity, const FVector& Spin, float Scale = 1.f);
	virtual void Tick(float Dt) override;
	float Age = 0.f;
};

/** A road hazard marker (lightning lanes, lock-on rings, shock wave, landing marks). */
UCLASS()
class FULLTHROTTLE_API AFTHazard : public AActor
{
	GENERATED_BODY()
public:
	AFTHazard();
	UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> Plane;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> RightPlane;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UPointLightComponent> Light;
	/** 2.5: what is flying in to land on the mark (a bomb, a snowball, a fish, a fireball, a lightning bolt). */
	UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> Thing;
	void SetLook(const FLinearColor& Colour, float Intensity);
	void SetSafeGap(float Center, float HalfWidth);
	void SetThing(UStaticMesh* M, const FTransform& WorldT);
	void HideThing();
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	/** 3.1: what the marker draws inside its real area. Style 0 electric, 1 ice, 2 fire, 3 water; Type as the
	 *  hazard's (1 and 5 are square marks, the rest lanes and waves); Progress is the warning's own clock (0 at its
	 *  start, 1 as it goes live), which the marker shows closing in. Call after the actor's transform is set. */
	void SetStyle(int32 Style, int32 Type, float Progress, bool bLive);
	/** 3.1: the trail of whatever is flying in (picked by its mesh), turned to lie back along its flight. */
	UPROPERTY(VisibleAnywhere) TObjectPtr<UNiagaraComponent> ThingTrail;
	FVector LastThingAt = FVector::ZeroVector;
};
