#include "FTBrawlMusic.h"
#include "FTBrawlGameMode.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogFTBrawlMusic, Log, All);

static UAudioComponent* MakeTrack(AActor* Owner, const TCHAR* Name)
{
	UAudioComponent* A = Owner->CreateDefaultSubobject<UAudioComponent>(Name);
	A->bAutoActivate = false;
	A->bAllowSpatialization = false;   // 2D: music, not a sound in the bar
	A->bIsUISound = false;
	A->bIsMusic = true;
	return A;
}

AFTBrawlMusic::AFTBrawlMusic()
{
	PrimaryActorTick.bCanEverTick = true;
	Audio = MakeTrack(this, TEXT("Music"));
	RootComponent = Audio;
	BossAudio = MakeTrack(this, TEXT("BossMusic"));
	BossAudio->SetupAttachment(Audio);
}

void AFTBrawlMusic::BeginPlay()
{
	Super::BeginPlay();
	// the day set: the day track only (no fight, no boss)
	const AFTBrawlGameMode* GM = GetWorld() ? GetWorld()->GetAuthGameMode<AFTBrawlGameMode>() : nullptr;
	if (GM && GM->bDay)
	{
		bDayTrack = true;
		USoundWave* Day = LoadObject<USoundWave>(nullptr, DayTrackPath);
		if (!Day)
		{
			UE_LOG(LogFTBrawlMusic, Warning, TEXT("FTBRAWL music missing %s (import Docs/audio/descending-staircase.mp3: Content/Python/brawl_audio.py)"), DayTrackPath);
			return;
		}
		Day->bLooping = true;
		Audio->SetSound(Day);
		Audio->SetVolumeMultiplier(Volume);
		Audio->OnAudioPlaybackPercentNative.AddUObject(this, &AFTBrawlMusic::OnPercent);
		Audio->Play();
		UE_LOG(LogFTBrawlMusic, Log, TEXT("FTBRAWL music day start %s %.1fs loop=%d day=%d %s"), *Day->GetName(), Day->Duration, Day->bLooping ? 1 : 0,
			GM->DayN, GM->bDayWrap ? TEXT("wrap") : TEXT("before"));
		return;
	}
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
	if (USoundWave* Boss = LoadObject<USoundWave>(nullptr, BossTrackPath))
	{
		Boss->bLooping = true;
		BossAudio->SetSound(Boss);
		BossAudio->SetVolumeMultiplier(Volume);
		BossAudio->OnAudioPlaybackPercentNative.AddUObject(this, &AFTBrawlMusic::OnPercent);
	}
	else UE_LOG(LogFTBrawlMusic, Warning, TEXT("FTBRAWL music missing %s (import Docs/audio/boss-battle.mp3: Content/Python/brawl_audio.py); the fight track plays on"), BossTrackPath);
}

void AFTBrawlMusic::OnPercent(const UAudioComponent* Comp, const USoundWave*, const float Percent)
{
	if (Comp != Playing()) return;
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
	if (bDayTrack)   // the day set: keep the day track going, nothing to switch or duck
	{
		if (!GetWorld()->GetAudioDeviceRaw()) return;   // -nosound: nothing can play (it would "restart" every frame)
		if (!Audio->IsPlaying()) { Audio->Play(); UE_LOG(LogFTBrawlMusic, Log, TEXT("FTBRAWL music restarted %s"), *Audio->Sound->GetName()); }
		BeatT += Dt;
		if (BeatT >= 10.f)   // heartbeat for the tests (the scenes are short)
		{
			BeatT = 0;
			UE_LOG(LogFTBrawlMusic, Log, TEXT("FTBRAWL music day playing=%d track=%s percent=%.3f loops=%d"),
				Audio->IsPlaying() ? 1 : 0, *Audio->Sound->GetName(), LastPercent, Loops);
		}
		return;
	}
	// the boss track from the moment the boss comes out (after the waves) to the end of the night; back to Matt's
	// track only if the night goes back before the boss (a retry in the boss fight stays on the boss track)
	const bool bBossWant = BossAudio->Sound && (GM->bBossOut || (bBossOn && GM->Wave >= GM->NumWaves));
	if (bBossWant != bBossOn)
	{
		UAudioComponent* From = Playing();
		bBossOn = bBossWant;
		UAudioComponent* To = Playing();
		From->FadeOut(1.0f, 0.f);
		To->SetVolumeMultiplier(Volume);
		To->FadeIn(0.4f, bDucked ? WonVolume / Volume : 1.f);
		LastPercent = 0; Loops = 0;
		UE_LOG(LogFTBrawlMusic, Log, TEXT("FTBRAWL music %s %s wave=%d"), bBossOn ? TEXT("boss") : TEXT("waves"), *To->Sound->GetName(), GM->Wave + 1);
	}
	UAudioComponent* Cur = Playing();
	const bool bDuck = GM->bWon || GM->InCut();   // under the K.O.! screen and the cutscenes
	if (bDuck != bDucked)
	{
		bDucked = bDuck;
		Cur->AdjustVolume(bDuck ? 2.5f : 1.f, bDuck ? WonVolume / Volume : 1.f);
	}
	if (!Cur->IsPlaying()) { Cur->Play(); UE_LOG(LogFTBrawlMusic, Log, TEXT("FTBRAWL music restarted %s"), *Cur->Sound->GetName()); }
	// heartbeat for the tests: every 30 s of play, is it playing, where in the track, how loud
	BeatT += Dt;
	if (BeatT >= 30.f)
	{
		BeatT = 0;
		UE_LOG(LogFTBrawlMusic, Log, TEXT("FTBRAWL music playing=%d track=%s percent=%.3f loops=%d percent_events=%d ducked=%d"),
			Cur->IsPlaying() ? 1 : 0, *Cur->Sound->GetName(), LastPercent, Loops, bGotPercent ? 1 : 0, bDucked ? 1 : 0);
	}
}
