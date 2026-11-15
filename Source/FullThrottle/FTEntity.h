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
	void Animate(float Dt, float Speed, float Vx = 0.f);
	/** 3.3: give this actor the wheels listed for its mesh (Content/Data/wheels.json); false if it has none. */
	bool SetupWheels(UStaticMesh* BodyMesh, bool bChrome);
	/** 3.3 the Tesla truck: Charge 0..1 (glow, hum, spin), and up to 3 arcs from the coil's top to world points. */
	void SetTesla(float InCharge, const TArray<FVector>& ArcTo);
	/** 3.3: a henchman's rope runs from his raised hand up to this world point (the chopper's door). */
	void SetRope(const FVector& WorldTop);
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
	UPROPERTY(VisibleAnywhere) TObjectPtr<UPointLightComponent> Light;
	/** 2.5: what is flying in to land on the mark (a bomb, a snowball, a fish, a fireball, a lightning bolt). */
	UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> Thing;
	void SetLook(const FLinearColor& Colour, float Intensity);
	void SetThing(UStaticMesh* M, const FTransform& WorldT);
	void HideThing();
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
};
