// The world's sound: every level has a sound of its own that changes as the car travels through its districts.
//
//   BEDS       looping ambiences (the sea, a boardwalk's murmur, plant hum, desert wind ...), three to five a level.
//   DISTRICTS  which beds play between which kilometres and how loud; the game cross-fades as the car travels. The
//              kilometres are the dressing tables' own (FTDressL<n>.inl), so that what is heard is what is seen.
//   PASSES     one sound at a kilometre mark and a side (a train going by, a big wheel's organ, a ship's horn): its level
//              follows the car's distance to the mark, it is panned to its side, and its pitch falls as the car goes by.
//
// Every sound is synthesised by Tools/sfx_ambience.py (nothing sampled) and imported by Content/Python/ft_ambience.py
// into /Game/Audio/Ambience. Each level's table is its own file, FTAmbienceL<n>.inl; the words it is written in are
// explained at the top of FTAmbience.cpp. Levels are written as "dB under the music": the generator writes every bed
// at one RMS (BedDb) and every pass with its loudest half second at one level (PassDb), and a table's first line is
// the level its music has as the game plays it, so the number in the table is the real distance between the two.
//
// The mix: the world stays UNDER the race. It follows the music's own ducking, squared (music at 0.4 for a big moment
// or the director's line: the world at 0.16), is trimmed again while a boss is out, plays only while a race is on
// (the slate, the race, a wreck, the wrap; at 0.4 under the title's demo, as the effects do), and uses at most five
// voices (four beds and one pass). A sound that is not in the project is simply not played.
// Two switches: ft.Ambience 0 turns the whole layer off; ft.AmbienceDb is its one master level (dB on top of the
// tables; the default, -3, is on the quiet side).
// Sound only, and its own sound only: nothing here reads input or changes anything in the game, and it changes no
// other sound: it READS the music's volume to keep itself under it, and sets nothing but its own voices.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FTAmbience.generated.h"

class UAudioComponent;
class USoundBase;
class AFTGameMode;
class FObjectPropertyBase;

/** A looping bed: the name the districts call it by, and its sound in /Game/Audio/Ambience. */
struct FFTAmbBed { FString Name, Asset; };

/** One bed in one district, and how far under the level's music it plays there (dB). */
struct FFTAmbMix { int32 Bed = 0; float Under = 12.f; };

/** A district runs from its kilometre to the next district's. */
struct FFTAmbDistrict { FString Name; float FromKm = 0.f, ToKm = 1.e9f; TArray<FFTAmbMix> Mix; };

struct FFTAmbPass
{
	FString Name, Asset;
	float Km = 0.f;          // the mark
	int32 Side = 0;          // -1 left of travel, +1 right, 0 on the road's own line (under it, over it): not panned
	float OutM = 60.f;       // metres from the centre line
	float UpM = 0.f;         // metres over the road
	float RangeM = 300.f;    // heard from this far before the mark to this far after it
	float Under = 6.f;       // at the mark: its loudest half second, dB under the music
	float Climax = 0.f;      // a one-shot: seconds into the sound where its climax is (it starts that long before the car reaches the mark)
	float Doppler = 1.f;     // 0..1 of the real fall in pitch of driving past it
	bool bSpot = false;      // a loop that plays for as long as the car is in range (a wheel's organ, a curtain of fire)
};

struct FFTAmbTable
{
	float MusicDb = -23.5f;  // the level's music as the game plays it: RMS, dBFS (DevLog: ambience/logs/music_levels.txt)
	TArray<FFTAmbBed> Beds;
	TArray<FFTAmbDistrict> Districts;
	TArray<FFTAmbPass> Passes;
};

namespace FTAmbience
{
	constexpr float BedDb = -23.f;       // every bed's RMS, dBFS (Tools/sfx_ambience.py: BED_RMS)
	constexpr float PassDb = -17.f;      // every pass's loudest half second (PASS_LOUD)
	constexpr float FadeKm = 0.3f;       // two districts cross-fade over this stretch, centred on their border
	constexpr float BossTrim = 0.7f;     // the whole layer while a boss is out (-3 dB)
	constexpr int32 MaxBedVoices = 4;    // and one pass: five voices at most

	/** The level's table (0 Bayshore ... 7 City Center). */
	void Build(int32 Level, FFTAmbTable& Out);
	/** The district the kilometre is in, or INDEX_NONE if the table has none. */
	int32 DistrictAt(const FFTAmbTable& T, float Km);
	/** The gain each bed's sound should play at when the car is at Km (one number a bed; 0 = not playing). */
	void BedGains(const FFTAmbTable& T, float Km, TArray<float>& Out);
	/** The gain a pass's sound should play at when the car is at Km. */
	float PassGain(const FFTAmbTable& T, const FFTAmbPass& P, float Km);
}

UCLASS()
class FULLTHROTTLE_API UFTAmbienceSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickableWhenPaused() const override { return true; }

protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

private:
	void Setup(AFTGameMode* GM);
	void StopAll();
	void StartPass(AFTGameMode* GM, int32 Index, float Volume);

	UPROPERTY(Transient) TArray<TObjectPtr<USoundBase>> BedSounds;
	UPROPERTY(Transient) TArray<TObjectPtr<USoundBase>> PassSounds;
	UPROPERTY(Transient) TArray<TObjectPtr<UAudioComponent>> BedVoices;
	UPROPERTY(Transient) TObjectPtr<UAudioComponent> PassVoice;

	TWeakObjectPtr<AFTGameMode> Game;
	const FObjectPropertyBase* MusicProp = nullptr;   // the game mode's "Music" component, looked up in Setup(): the duck is read off it
	FFTAmbTable Table;
	int32 Level = -1;
	TArray<float> Want, Cur;        // a bed's gain: where it is going, where it is
	TArray<uint8> PassDone;         // on this approach to its mark: a one-shot has been played, or a spot could not start
	int32 PassNow = INDEX_NONE;     // the pass the voice is playing
	int32 DistrictNow = -2;
	float Gate = 0.f;               // 0 silent .. 1: a race is on
	float Clock = 0.f, LogT = 0.f;
	bool bOverBudget = false;
};
