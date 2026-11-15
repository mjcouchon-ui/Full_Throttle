// The fight music: Matt's track (/Game/Brawl/Audio/S_FightMusic, from Docs/audio/matt-fight-music.mp3),
// looping through the waves, then the boss track (/Game/Brawl/Audio/S_BossMusic, from Docs/audio/boss-battle.mp3,
// "Bit-Reduced Boss Battle", owner 15 Nov) from the moment the boss comes out. The game mode spawns one at BeginPlay.
// The playing track ducks under the K.O.! screen and the cutscenes. Logs "FTBRAWL music ..." lines (start, every loop,
// "music boss" / "music waves" at a switch) for the tests.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FTBrawlMusic.generated.h"

class UAudioComponent;
class USoundWave;

UCLASS()
class AFTBrawlMusic : public AActor
{
	GENERATED_BODY()
public:
	AFTBrawlMusic();
	virtual void BeginPlay() override;
	virtual void Tick(float Dt) override;

	static constexpr const TCHAR* TrackPath = TEXT("/Game/Brawl/Audio/S_FightMusic.S_FightMusic");
	static constexpr const TCHAR* BossTrackPath = TEXT("/Game/Brawl/Audio/S_BossMusic.S_BossMusic");
	float Volume = 0.8f, WonVolume = 0.35f;

private:
	UPROPERTY() TObjectPtr<UAudioComponent> Audio;
	UPROPERTY() TObjectPtr<UAudioComponent> BossAudio;
	UAudioComponent* Playing() const { return bBossOn ? BossAudio : Audio; }
	void OnPercent(const UAudioComponent* Comp, const USoundWave* Wave, const float Percent);
	float LastPercent = 0; int32 Loops = 0; bool bDucked = false, bGotPercent = false, bBossOn = false; float BeatT = 0;
};
