// 3.1: how the attacks look. One place loads, roots and warms the attack effects (Content/Python/ft_fx.py builds
// them under /Game/FX) and decides which layers a quality setting shows. Looks only: nothing here deals damage,
// moves anything the race simulates, or changes a timer.
#pragma once

#include "CoreMinimal.h"

class UNiagaraSystem;
class UNiagaraComponent;
class USceneComponent;
class AActor;

namespace FTFx
{
	/** What a layer is for. A layered effect is several systems spawned together; each says how much it matters. */
	enum : int32
	{
		Main = 0,    // the effect's readable shape: always shown
		Extra = 1,   // secondary detail (sparks, embers, smoke, rings): not at Low effects quality
		Flash = 2,   // a short full-bright flash: not with the Reduced Motion option
	};

	/** The system /Game/FX/<Name>, loaded once and kept (rooted); null if there is no such asset. */
	UNiagaraSystem* System(const TCHAR* Name);
	/** Low effects quality (sg.EffectsQuality 0): only the Main layers. */
	bool IsLow();
	/** Whether a layer shows now, for this world's options. */
	bool Shows(const UObject* Ctx, int32 Layer);

	/** A burst left in the world where it happened (explosions, landings, anything the car drives away from). */
	UNiagaraComponent* Burst(const UObject* Ctx, const TCHAR* Name, const FVector& At, const FRotator& Rot = FRotator::ZeroRotator, float Scale = 1.f, int32 Layer = Main);
	/** A burst that rides with what it is on (a muzzle flash on a car doing 120 mph, sparks on the car that was hit).
	 *  At/Rot are in the world unless bRelative. It lets go by itself when it has finished. */
	UNiagaraComponent* Attach(const TCHAR* Name, USceneComponent* To, const FVector& At, const FRotator& Rot = FRotator::ZeroRotator, float Scale = 1.f, int32 Layer = Main, bool bRelative = false);

	/** Level setup: load every attack effect and submit its PSOs, so the first shot of a race does not hitch. */
	void Warm(AActor* Owner);

	// ---- the combat-effects overhaul. Still looks only.

	/** What blew up. Each has its own recipe of layers (FTFx.cpp, Explosion): a fuel drum goes straight up, a car's
	 *  fire rolls on along the hit, a chopper bursts in the air, a boss goes in a chain. */
	enum class EBlast : uint8 { Generic, Barrel, Bike, Car, Truck, Heli, HeliCrash, Boss, Missile, MissileFizz, Player };

	struct FBlast
	{
		FVector At = FVector::ZeroVector;
		float Size = 1.f;                       // as AFTGameMode::Explode's Size
		EBlast Kind = EBlast::Generic;
		FVector Dir = FVector::ZeroVector;      // world: the way the hit was travelling (debris and fire go on along it); zero = none
		FVector Vel = FVector::ZeroVector;      // world velocity of what blew up: its fire rolls on with it, slowing
		FQuat Road = FQuat::Identity;           // the road's frame at the blast (+Z off the road)
		FVector Cam = FVector::ZeroVector;      // the camera: nothing flat is drawn right beside it
		const TCHAR* Tint = nullptr;            // the SFX DEPT's wreck colour, as an asset suffix ("_s_blue_fire"), or null
		float Reach = 0.f;                      // cm: the blast's real reach across the road, for the ring on the road (0 = by Size)
		bool bGround = true;                    // on the road: it gets the ring and the scorch
		FVector Ground = FVector::ZeroVector;   // the point of the road under it (where the ring and the scorch lie)
		bool bLight = false;                    // what it hit blows up in the same place this frame: the fire and smoke are left to that
	};
	/** The blast's layers, by what blew up. Low shows the fire only (Main); Reduced Motion drops the flash. */
	void Explosion(const UObject* Ctx, const FBlast& B);

	/** Give a burst the speed of what it came off: it rolls on with it and slows (Drag, 1/s), for Seconds. */
	void Carry(UNiagaraComponent* C, const FVector& Velocity, float Drag, float Seconds);

	/** Smoke laid in the world behind something that flies (a missile): call every frame with where it is now, under
	 *  a key of its own (its entity id). The smoke stays where it was put; lengths the viewer (ViewFrom, moving at
	 *  ViewSpeed cm/s) is about to reach get a short life, so the road it is driving into is clear. */
	void Trail(const UObject* Ctx, int32 Key, const FVector& Now, const FVector& Up, const FVector& ViewFrom, float ViewSpeed);
	/** That thing is gone: forget its trail (what it laid stays and fades by itself). */
	void TrailEnd(int32 Key);

	/** Once a frame (the player's car calls it): moves what is being carried and fires the delayed layers. */
	void Tick(const UObject* Ctx, float Dt);
	/** A new scene (a retake's slate, the highlight reel, the title): nothing of the last one is left burning or
	 *  smoking. Stops every burst this file spawned, removes the wrecks, and logs what it did (FTFX clear). */
	void ClearWorld(const UObject* Ctx, const TCHAR* Why);
	/** Log every effect still running in the world, by system (FTFX census): the proof of the above in a test run.
	 *  Also the console command ft.FxCensus. */
	void Census(const UObject* Ctx, const TCHAR* Tag);
}
