#include "FTBrawlMusic.h"
#include "FTBrawlGameMode.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogFTBrawlMusic, Log, All);

AFTBrawlMusic::AFTBrawlMusic()
{
	PrimaryActorTick.bCanEverTick = true;
	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Music"));
	RootComponent = Audio;
	Audio->bAutoActivate = false;
	Audio->bAllowSpatialization = false;   // 2D: music, not a sound in the bar
	Audio->bIsUISound = false;
	Audio->bIsMusic = true;
}

void AFTBrawlMusic::BeginPlay()
{
	Super::BeginPlay();
	USoundWave* Track = LoadObject<USoundWave>(nullptr, TrackPath);
	if (!Track)
	{
		UE_LOG(LogFTBrawlMusic, Warning, TEXT("FTBRAWL music missing %s (import Docs/audio/matt-fight-music.mp3: Content/Python/brawl_audio.py)"), TrackPath);
		return;
	}
	Track->bLooping = true;   // the asset is saved looping too; this keeps it looping if someone reimports
	Audio->SetSound(Track);
	Audio->SetVolumeMultiplier(Volume);
	Audio->OnAudioPlaybackPercentNative.AddUObject(this, &AFTBrawlMusic::OnPercent);
	Audio->Play();
	UE_LOG(LogFTBrawlMusic, Log, TEXT("FTBRAWL music start %s %.1fs loop=%d"), *Track->GetName(), Track->Duration, Track->bLooping ? 1 : 0);
}

void AFTBrawlMusic::OnPercent(const UAudioComponent*, const USoundWave*, const float Percent)
{
	// a new pass of the looping track: the percent either wraps (drops back toward 0) or runs past the next whole number
	const bool bWrapped = Percent + 0.5f < LastPercent;
	const bool bPastWhole = FMath::FloorToInt(Percent) > FMath::FloorToInt(LastPercent);
	if (bWrapped || bPastWhole)
	{
		Loops++;
		UE_LOG(LogFTBrawlMusic, Log, TEXT("FTBRAWL music loop %d (percent %.3f -> %.3f)"), Loops, LastPercent, Percent);
	}
	LastPercent = Percent;
	bGotPercent = true;
}

void AFTBrawlMusic::Tick(float Dt)
{
	Super::Tick(Dt);
	const AFTBrawlGameMode* GM = GetWorld() ? GetWorld()->GetAuthGameMode<AFTBrawlGameMode>() : nullptr;
	if (!GM || !Audio->Sound) return;
	const bool bDuck = GM->bWon || GM->InCut();   // under the K.O.! screen and the cutscenes
	if (bDuck != bDucked)
	{
		bDucked = bDuck;
		Audio->AdjustVolume(bDuck ? 2.5f : 1.f, bDuck ? WonVolume / Volume : 1.f);
	}
	if (!Audio->IsPlaying()) { Audio->Play(); UE_LOG(LogFTBrawlMusic, Log, TEXT("FTBRAWL music restarted")); }
	// heartbeat for the tests: every 30 s of play, is it playing, where in the track, how loud
	BeatT += Dt;
	if (BeatT >= 30.f)
	{
		BeatT = 0;
		UE_LOG(LogFTBrawlMusic, Log, TEXT("FTBRAWL music playing=%d percent=%.3f loops=%d percent_events=%d ducked=%d"),
			Audio->IsPlaying() ? 1 : 0, LastPercent, Loops, bGotPercent ? 1 : 0, bDucked ? 1 : 0);
	}
}
