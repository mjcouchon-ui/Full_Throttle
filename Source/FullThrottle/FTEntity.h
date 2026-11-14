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

	EFTKind Kind = EFTKind::None;
	int32 Variant = 0;
	int32 Lvl = 0;
	float AnimT = 0.f;
	void Setup(EFTKind InKind, int32 InVariant, int32 Level);
	void SetGlow(float Amount);   // rev lights, chain wind-up, brake flare
	void Animate(float Dt, float Speed);
	static UStaticMesh* MeshFor(EFTKind Kind, int32 Variant, int32 Level);
	/** 2.6 ammo: an AP (blue-white, thinner) or HE (orange) tracer; standard rounds keep the stock look. */
	void SetTracer(const FLinearColor& Colour, float Width, bool bSpecial);
	bool bTracerColoured = false;
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
