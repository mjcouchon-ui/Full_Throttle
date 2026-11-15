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
}
