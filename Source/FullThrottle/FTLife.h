#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FTLife.generated.h"

struct FFTPropRule;
class AFTTrack;
class UNiagaraComponent;
class UStaticMeshComponent;
class UMaterialInstanceDynamic;

/** 3.1, the living world: scenery that moves. Looks only: nothing here has collision, deals damage or marks a hazard. */
namespace FTLife
{
	/** More roadside rules for a level, appended after its own (so every existing prop keeps its place: a rule's
	 *  random stream is seeded by its index). A rule whose mesh does not exist is skipped silently. FTTrackLife.cpp. */
	void ExtraProps(int32 Level, TArray<FFTPropRule>& R);
}

/** The air round the camera: one ambient effect per level (dust, snow, embers, bubbles...) that the car drives
 *  through, and any timed set-piece cues. AFTTrack spawns one when a race level begins. FTAmbient.cpp. */
UCLASS()
class AFTAmbient : public AActor
{
	GENERATED_BODY()
public:
	AFTAmbient();
	virtual void Tick(float Dt) override;
	/** Called once by the track that spawned it. */
	void Setup(AFTTrack* InTrack, int32 InLevel);

protected:
	/** Low effects quality thins the air (it never empties it). Read at Setup and now and then after: the pause
	 *  menu can change it mid-race. */
	void ApplyQuality();

	UPROPERTY(Transient) TObjectPtr<AFTTrack> Track;
	/** The air itself: 600 cards in one mesh (SM_AirField), placed by its material (ft_life.py, M_Air). */
	UPROPERTY(Transient) TObjectPtr<UStaticMeshComponent> Field;
	UPROPERTY(Transient) TObjectPtr<UMaterialInstanceDynamic> FieldMat;
	/** Optional extra: /Game/FX/Ambient/NS_Ambient_L<level>, if a level ever needs a real emitter as well. */
	UPROPERTY(Transient) TObjectPtr<UNiagaraComponent> Air;
	int32 Level = -1;
	float FullDensity = 1.f;
	float QualityClock = 0.f;
	int32 QualitySeen = -1;
};
