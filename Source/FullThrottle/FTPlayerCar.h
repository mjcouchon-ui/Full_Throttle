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
};

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
	FVector RoadUp = FVector::UpVector;

	/** Put the car on the road at d, x, facing along it, at a given speed. */
	void PlaceOnTrack(float InD, float InX, float InSpeed);
	void Shove(const FVector& WorldDir, float CmPerSec, float SpinRad = 0.f);
	void SetDamageLook(float Hp);           // smoke, fire, sparks, soot
	void ShedPart(int32 Index);
	void RestoreParts();
	int32 NumPartsOn() const;
	void SetHenchmenVisible(bool bLeft, bool bRight);
	void Wreck();
	bool IsWrecked() const { return bWrecked; }
	void SetLightsForLevel(int32 Level);
	FTransform GunMuzzle(int32 Side) const;

	virtual void Tick(float Dt) override;
	virtual void BeginPlay() override;

protected:
	void ApplyArcade(float Dt);
	UFUNCTION() void OnBodyHit(UPrimitiveComponent* HitComp, AActor* Other, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(Transient) TObjectPtr<AFTTrack> Track;
	FFTDrive Drive;
	bool bWrecked = false;
	float WallTimer = 0.f;
	float DriftBlend = 0.f;
	float CapsizeT = 0.f;
	TArray<FTransform> PartRest;
	UPROPERTY(Transient) TArray<TObjectPtr<UStaticMeshComponent>> LooseParts;
};
