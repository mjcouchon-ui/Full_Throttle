// The fight music: Matt's track (/Game/Brawl/Audio/S_FightMusic, from Docs/audio/matt-fight-music.mp3),
// looping for the whole night. The game mode spawns one at BeginPlay. It ducks under the K.O.! screen and comes
// back up when the fight starts again. Logs "FTBRAWL music ..." lines (start, every loop) for the tests.
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
	float Volume = 0.8f, WonVolume = 0.35f;

private:
	UPROPERTY() TObjectPtr<UAudioComponent> Audio;
	void OnPercent(const UAudioComponent* Comp, const USoundWave* Wave, const float Percent);
	float LastPercent = 0; int32 Loops = 0; bool bDucked = false, bGotPercent = false; float BeatT = 0;
};
