// 3.1: how the attacks look. One place loads, roots and warms the attack effects (Content/Python/ft_fx.py builds
// them under /Game/FX) and decides which layers a quality setting shows. Looks only: nothing here deals damage,
// moves anything the race simulates, or changes a timer.
#pragma once

#include "CoreMinimal.h"

class UNiagaraSystem;
class UNiagaraComponent;
class USceneComponent;
class AActor;
class AFTEntityActor;

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
		bool bImpact = false;                   // round 3: a missile kill or a turbo ram: the impact card, the blast held two frames behind it
	};
	/** The blast's layers, by what blew up. Low shows the fire only (Main); Reduced Motion drops the flash. */
	void Explosion(const UObject* Ctx, const FBlast& B);

	/** Give a burst the speed of what it came off: it rolls on with it and slows (Drag, 1/s), for Seconds. */
	void Carry(UNiagaraComponent* C, const FVector& Velocity, float Drag, float Seconds);

	/** Smoke laid in the world behind something that flies (a missile): call every frame with where it is now, under
	 *  a key of its own (its entity id). The smoke stays where it was put; lengths the viewer (ViewFrom, moving at
	 *  ViewSpeed cm/s) is about to reach get a short life, so the road it is driving into is clear. */
	void Trail(const UObject* Ctx, int32 Key, const FVector& Now, const FVector& Up, const FVector& ViewFrom, float ViewSpeed);

	/** Once a frame (the player's car calls it): moves what is being carried and fires the delayed layers. */
	void Tick(const UObject* Ctx, float Dt);
	/** A new scene (a retake's slate, the highlight reel, the title): nothing of the last one is left burning or
	 *  smoking. Stops every burst this file spawned, removes the wrecks, and logs what it did (FTFX clear). */
	void ClearWorld(const UObject* Ctx, const TCHAR* Why);
	/** Log every effect still running in the world, by system (FTFX census): the proof of the above in a test run.
	 *  Also the console command ft.FxCensus. */
	void Census(const UObject* Ctx, const TCHAR* Tag);

	// ---- round 3: weight. Still looks only.

	/** A foe wears what it has left: smoke once it has lost a third of its armour, flames under a third, and a burst
	 *  of sparks at each step, so a kill is read before it lands. Call every frame for each live foe, after its actor
	 *  has been placed, with what it has left (0..1). A foe not heard of for three frames is forgotten. At most 8
	 *  foes wear smoke or fire at a time (4 at Low; the smoke is the Extra layer, the fire shows at Low). */
	void FoeLook(const UObject* Ctx, int32 Key, AFTEntityActor* Foe, float Left);

	// ---- round 3: the car in the world. Still looks only.

	/** The level's surface (0..7, FTData's order of levels): the colour of the marks the tyres leave on it. The car
	 *  says it at level setup. */
	void Surface(const UObject* Ctx, int32 Level);
	/** A length of tyre mark on the road from From to To (world), Width cm wide, lying against Up. It stays for the
	 *  scene: the newest 480 lengths are kept, and FTFx::ClearWorld removes them all. Not at Low. */
	void TyreMark(const UObject* Ctx, const FVector& From, const FVector& To, const FVector& Up, float Width);
	/** The soot a blast leaves on the road, kept for the scene (the newest 64; FTFx::ClearWorld removes them). False
	 *  when it could not be laid (Low, or a build without the mark material): the caller draws the short-lived
	 *  sprite instead. Road is the road's frame there (+Z off the road). */
	bool SootMark(const UObject* Ctx, const FVector& At, const FQuat& Road, float RadiusCm);

	// ---- round 4. Still looks only.

	/** A sliding wreck leaves a burning trail: a length of scorched road from From to To (world), Width cm wide,
	 *  lying against Up, kept for the scene with the other marks (the newest 160 lengths), and on every other length
	 *  a lick of flame that burns out in under a second (at most 8 alight at once). Not at Low. */
	void WreckTrail(const UObject* Ctx, const FVector& From, const FVector& To, const FVector& Up, float Width);
}
