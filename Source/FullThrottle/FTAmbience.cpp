#include "FTAmbience.h"
#include "FTGameMode.h"
#include "FTPlayerCar.h"
#include "FTTrack.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundAttenuation.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformTime.h"
#include "Misc/App.h"
#include "UObject/UnrealType.h"

// The world's sound (FTAmbience.h says what it is). A level's table, FTAmbienceL<n>.inl, is written with these words:
//
//   Music(-23.4f);
//       The level's music as the game plays it (RMS, dBFS: the track's own RMS less 6 dB, for the game plays music
//       at 0.5). Every "under" below is counted from it.
//   Bed(TEXT("Surf"), TEXT("A_L0_Surf"));
//       A looping bed: its name in this table, its sound in /Game/Audio/Ambience. Five at most.
//   District(TEXT("The Strip"), 0.f, 3.2f);
//       Starts a district (from km, to km: the dressing table's own). Each Mix after it belongs to it.
//   Mix(TEXT("Surf"), 13.f);
//       That bed plays in this district, 13 dB under the music. Two or three beds a district; with the district
//       before or after it no more than four different beds, because both play while the car crosses the border.
//   Pass(TEXT("A ship's horn"), TEXT("A_L0_ShipHorn"), km, side, out m, range m, under dB, climax s [, doppler]);
//       A one-shot at a mark: side -1 left of travel, +1 right, 0 on the road's line (not panned); "out" metres from
//       the centre line; heard from "range" metres before the mark to as far after it, loudest at the mark ("under"
//       dB under the music there); "climax" is where in the sound its climax is: the game starts it that long before
//       the car reaches the mark, whatever the car's speed. doppler 0..1: how much of the real fall in pitch.
//   Spot(TEXT("The big wheel's organ"), TEXT("A_L0_Organ"), km, side, out m, range m, under dB [, doppler]);
//       The same for a loop that plays as long as the car is in range.
//   Up(125.f);
//       The pass or spot just written is that many metres over the road (an airship, a gondola).
//
// Passes share ONE voice: keep two marks' ranges apart, or the later one waits for the earlier one's end.

static TAutoConsoleVariable<int32> CVarFTAmbience(TEXT("ft.Ambience"), 1, TEXT("FULL THROTTLE: 1 = the world's sound (ambience beds by district, passes at landmarks); 0 = off"));
static TAutoConsoleVariable<float> CVarFTAmbienceDb(TEXT("ft.AmbienceDb"), -9.f, TEXT("FULL THROTTLE: the world's sound louder or quieter, in dB on top of what the tables say. The default, -9 (15 Nov, the owner: with the ambience at -3 the music seemed too quiet), is on the quiet side; 0 = as the tables are written; -6 = half as loud again; +6 = double"));
static TAutoConsoleVariable<int32> CVarFTAmbienceLog(TEXT("ft.AmbienceLog"), 0, TEXT("FULL THROTTLE tests: 1 = log what the ambience plays every two seconds (FT AMB state); district changes and passes are logged always"));
static TAutoConsoleVariable<float> CVarFTAmbienceProbe(TEXT("ft.AmbienceProbe"), -1.f, TEXT("FULL THROTTLE tests: the ambience plays as if the car were at this kilometre (-1 = where the car is). Sound only: nothing in the game moves"));

namespace
{
	// AFTGameMode::MusicVol, which is protected there: the level the music plays at when nothing ducks it
	const float GameMusicVol = 0.63f;

	inline float DbToLin(float Db) { return FMath::Pow(10.f, Db / 20.f); }

	USoundBase* LoadAmb(const FString& Asset)
	{
		const FString Path = FString::Printf(TEXT("/Game/Audio/Ambience/%s.%s"), *Asset, *Asset);
		return Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, *Path, nullptr, LOAD_NoWarn | LOAD_Quiet));
	}

	// The mix's duck state is not public in AFTGameMode (DuckT, DuckDepth, Music are protected), so it is read off the
	// music's own audio component, found by its property name: its volume against the level it rests at. The
	// property is looked up in Setup(), for the game mode at hand, and kept in the subsystem (a function-static
	// pointer would go stale if the class were reinstanced in the editor).
	UAudioComponent* MusicOf(const FObjectPropertyBase* Prop, AFTGameMode* GM)
	{
		return (Prop && GM) ? Cast<UAudioComponent>(Prop->GetObjectPropertyValue_InContainer(GM)) : nullptr;
	}

	struct FAmbWriter
	{
		FFTAmbTable& T;
		explicit FAmbWriter(FFTAmbTable& In) : T(In) {}

		void Music(float Db) { T.MusicDb = Db; }
		void Bed(const TCHAR* Name, const TCHAR* Asset)
		{
			FFTAmbBed& B = T.Beds.AddDefaulted_GetRef();
			B.Name = Name; B.Asset = Asset;
		}
		void District(const TCHAR* Name, float FromKm, float ToKm)
		{
			FFTAmbDistrict& D = T.Districts.AddDefaulted_GetRef();
			D.Name = Name; D.FromKm = FromKm; D.ToKm = ToKm;
		}
		void Mix(const TCHAR* BedName, float Under)
		{
			if (!T.Districts.Num()) return;
			for (int32 i = 0; i < T.Beds.Num(); ++i)
			{
				if (T.Beds[i].Name == BedName)
				{
					FFTAmbMix& M = T.Districts.Last().Mix.AddDefaulted_GetRef();
					M.Bed = i; M.Under = Under;
					return;
				}
			}
			UE_LOG(LogFT, Warning, TEXT("FT AMB table: district %s names a bed that is not in the table: %s"), *T.Districts.Last().Name, BedName);
		}
		FFTAmbPass& Mark(const TCHAR* Name, const TCHAR* Asset, float Km, int32 Side, float OutM, float RangeM, float Under)
		{
			FFTAmbPass& P = T.Passes.AddDefaulted_GetRef();
			P.Name = Name; P.Asset = Asset; P.Km = Km; P.Side = Side; P.OutM = OutM; P.RangeM = RangeM; P.Under = Under;
			return P;
		}
		void Pass(const TCHAR* Name, const TCHAR* Asset, float Km, int32 Side, float OutM, float RangeM, float Under, float Climax, float Doppler = 1.f)
		{
			FFTAmbPass& P = Mark(Name, Asset, Km, Side, OutM, RangeM, Under);
			P.Climax = Climax; P.Doppler = Doppler;
		}
		void Spot(const TCHAR* Name, const TCHAR* Asset, float Km, int32 Side, float OutM, float RangeM, float Under, float Doppler = 0.5f)
		{
			FFTAmbPass& P = Mark(Name, Asset, Km, Side, OutM, RangeM, Under);
			P.bSpot = true; P.Doppler = Doppler;
		}
		void Up(float Metres) { if (T.Passes.Num()) T.Passes.Last().UpM = Metres; }

		void Level0(); void Level1(); void Level2(); void Level3(); void Level4(); void Level5(); void Level6(); void Level7();
	};

	void FAmbWriter::Level0()
	{
#include "FTAmbienceL0.inl"
	}
	void FAmbWriter::Level1()
	{
#include "FTAmbienceL1.inl"
	}
	void FAmbWriter::Level2()
	{
#include "FTAmbienceL2.inl"
	}
	void FAmbWriter::Level3()
	{
#include "FTAmbienceL3.inl"
	}
	void FAmbWriter::Level4()
	{
#include "FTAmbienceL4.inl"
	}
	void FAmbWriter::Level5()
	{
#include "FTAmbienceL5.inl"
	}
	void FAmbWriter::Level6()
	{
#include "FTAmbienceL6.inl"
	}
	void FAmbWriter::Level7()
	{
#include "FTAmbienceL7.inl"
	}
}

void FTAmbience::Build(int32 Level, FFTAmbTable& Out)
{
	Out = FFTAmbTable();
	FAmbWriter W(Out);
	switch (Level)
	{
	case 0: W.Level0(); break;
	case 1: W.Level1(); break;
	case 2: W.Level2(); break;
	case 3: W.Level3(); break;
	case 4: W.Level4(); break;
	case 5: W.Level5(); break;
	case 6: W.Level6(); break;
	case 7: W.Level7(); break;
	default: break;
	}
	Out.Districts.Sort([](const FFTAmbDistrict& A, const FFTAmbDistrict& B) { return A.FromKm < B.FromKm; });
}

int32 FTAmbience::DistrictAt(const FFTAmbTable& T, float Km)
{
	// the last district that has begun (they are in order, and each runs on to the next one's start)
	int32 Found = T.Districts.Num() ? 0 : INDEX_NONE;
	for (int32 i = 1; i < T.Districts.Num(); ++i)
	{
		if (Km >= T.Districts[i].FromKm) Found = i;
	}
	return Found;
}

void FTAmbience::BedGains(const FFTAmbTable& T, float Km, TArray<float>& Out)
{
	Out.Init(0.f, T.Beds.Num());
	const int32 N = T.Districts.Num();
	// Each district has a weight: 1 inside it, easing to 0 across FadeKm centred on each of its borders, so that two
	// neighbours' weights add up to 1. The weights mix POWER, not gain: two unrelated beds cross over at equal
	// loudness, and a bed that both districts play at one level does not move at all.
	auto Ease = [](float X) { X = FMath::Clamp(X, 0.f, 1.f); return X * X * (3.f - 2.f * X); };
	for (int32 i = 0; i < N; ++i)
	{
		const float In = i == 0 ? 1.f : Ease((Km - T.Districts[i].FromKm) / FadeKm + 0.5f);
		const float Left = i + 1 < N ? 1.f - Ease((Km - T.Districts[i + 1].FromKm) / FadeKm + 0.5f) : 1.f;
		const float Weight = In * Left;
		if (Weight <= 0.f) continue;
		for (const FFTAmbMix& M : T.Districts[i].Mix)
		{
			if (Out.IsValidIndex(M.Bed)) Out[M.Bed] += Weight * FMath::Square(DbToLin(T.MusicDb - M.Under - BedDb));
		}
	}
	for (float& G : Out) G = FMath::Sqrt(G);
}

float FTAmbience::PassGain(const FFTAmbTable& T, const FFTAmbPass& P, float Km)
{
	const float X = FMath::Abs(Km - P.Km) * 1000.f / FMath::Max(1.f, P.RangeM);
	if (X >= 1.f) return 0.f;
	return DbToLin(T.MusicDb - P.Under - PassDb) * FMath::Square(1.f - X * X);
}

// ------------------------------------------------------------------------------------------------ the subsystem
bool UFTAmbienceSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

TStatId UFTAmbienceSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFTAmbienceSubsystem, STATGROUP_Tickables);
}

void UFTAmbienceSubsystem::Deinitialize()
{
	StopAll();
	Super::Deinitialize();
}

void UFTAmbienceSubsystem::StopAll()
{
	for (UAudioComponent* V : BedVoices)
	{
		if (IsValid(V) && V->IsPlaying()) V->Stop();
	}
	if (IsValid(PassVoice) && PassVoice->IsPlaying()) PassVoice->Stop();
	PassNow = INDEX_NONE;
}

void UFTAmbienceSubsystem::Setup(AFTGameMode* GM)
{
	StopAll();
	BedVoices.Reset();
	PassVoice = nullptr;
	BedSounds.Reset();
	PassSounds.Reset();
	Game = GM;
	Level = GM->Level;
	MusicProp = FindFProperty<FObjectPropertyBase>(GM->GetClass(), TEXT("Music"));
	FTAmbience::Build(Level, Table);
	int32 BedsFound = 0, PassesFound = 0;
	for (const FFTAmbBed& B : Table.Beds)
	{
		USoundBase* S = LoadAmb(B.Asset);
		BedSounds.Add(S);
		if (S) ++BedsFound;
	}
	for (const FFTAmbPass& P : Table.Passes)
	{
		USoundBase* S = LoadAmb(P.Asset);
		PassSounds.Add(S);
		if (S) ++PassesFound;
	}
	BedVoices.SetNum(Table.Beds.Num());
	Want.Init(0.f, Table.Beds.Num());
	Cur.Init(0.f, Table.Beds.Num());
	PassDone.Init(0, Table.Passes.Num());
	DistrictNow = -2;
	Gate = 0.f;
	bOverBudget = false;
	UE_LOG(LogFT, Display, TEXT("FT AMB setup level=L%d music=%.1f beds=%d found=%d districts=%d passes=%d found=%d"),
		Level + 1, Table.MusicDb, Table.Beds.Num(), BedsFound, Table.Districts.Num(), Table.Passes.Num(), PassesFound);
}

void UFTAmbienceSubsystem::StartPass(AFTGameMode* GM, int32 Index, float Volume)
{
	PassNow = INDEX_NONE;
	if (PassVoice && PassVoice->IsPlaying()) PassVoice->Stop();
	if (!Table.Passes.IsValidIndex(Index) || !PassSounds.IsValidIndex(Index) || !PassSounds[Index]) return;
	const FFTAmbPass& P = Table.Passes[Index];
	USoundBase* S = PassSounds[Index];
	if (!PassVoice)
	{
		PassVoice = UGameplayStatics::CreateSound2D(GM, S, Volume, 1.f, 0.f, nullptr, false, false);
		if (!PassVoice) return;
		// a place in the world, panned by where it is; its level is set here from the car's distance, not by the engine
		PassVoice->bIsUISound = false;
		PassVoice->bOverrideAttenuation = true;
		PassVoice->AttenuationOverrides.bAttenuate = false;
		PassVoice->AttenuationOverrides.bSpatialize = true;
	}
	else
	{
		PassVoice->SetSound(S);
	}
	PassVoice->bAllowSpatialization = P.Side != 0;
	// (nearer than this it eases towards the middle: abreast of the mark it is left or right, but not in one ear only)
	PassVoice->AttenuationOverrides.NonSpatializedRadiusStart = FMath::Max(1500.f, P.OutM * 100.f * 2.2f);
	PassVoice->AttenuationOverrides.NonSpatializedRadiusEnd = 0.f;
	if (const AFTTrack* Track = GM->GetTrack())
	{
		PassVoice->SetWorldLocation(Track->ToWorld(P.Km * 100000.f, P.Side * P.OutM * 100.f, P.UpM * 100.f));
	}
	PassVoice->SetVolumeMultiplier(Volume);
	PassVoice->SetPitchMultiplier(1.f);
	PassVoice->Play(0.f);
	PassNow = Index;
}

void UFTAmbienceSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UWorld* World = GetWorld();
	AFTGameMode* GM = World ? World->GetAuthGameMode<AFTGameMode>() : nullptr;
	if (!GM || !GM->GetCar() || !GM->GetTrack() || !GM->GetTrack()->IsBuilt()) return;
	// a world without audio (-nosound, a server): nothing is attempted, made or logged
	if (!World->GetAudioDeviceRaw() || !World->bAllowAudioPlayback) return;
	if (Game.Get() != GM || Level != GM->Level) Setup(GM);
	if (!Table.Beds.Num() && !Table.Passes.Num()) return;

	const float RDt = FMath::Min((float)FApp::GetDeltaTime(), 0.1f);
	Clock += RDt;
	const AFTPlayerCar* Car = GM->GetCar();

	// ---- when: only while a race is on; under the title's demo as loud as the game plays its effects there
	const bool bOn = CVarFTAmbience.GetValueOnGameThread() != 0;
	const bool bRace = GM->Overlay == EFTOverlay::None
		&& (GM->Phase == EFTPhase::Card || GM->Phase == EFTPhase::Play || GM->Phase == EFTPhase::Wreck || GM->Phase == EFTPhase::Wrap);
	const float GateWant = (bOn && bRace) ? (GM->bAttract ? 0.4f : 1.f) : 0.f;
	Gate = FMath::FInterpConstantTo(Gate, GateWant, RDt, GateWant > Gate ? 1.2f : 3.f);

	// ---- how far under: the music's own ducking (the director's line, a big moment, a hit), squared; a boss;
	// the master level. The music's volume is only READ here: this layer changes nothing of the music or of any
	// other sound, it only turns itself down.
	float Duck = 1.f;
	if (const UAudioComponent* MusicComp = MusicOf(MusicProp, GM)) Duck = FMath::Clamp(MusicComp->VolumeMultiplier / GameMusicVol, 0.f, 1.f);
	const float Master = DbToLin(FMath::Clamp(CVarFTAmbienceDb.GetValueOnGameThread(), -40.f, 12.f));
	const float Under = Duck * Duck * (GM->bBossOn ? FTAmbience::BossTrim : 1.f) * Master;

	// ---- where
	float Km = Car->D / 100000.f;
	const float Probe = CVarFTAmbienceProbe.GetValueOnGameThread();
	if (Probe >= 0.f) Km = Probe;

	// ---- the beds
	FTAmbience::BedGains(Table, Km, Want);
	int32 Playing = 0;
	for (int32 b = 0; b < Table.Beds.Num(); ++b)
	{
		Cur[b] = FMath::FInterpTo(Cur[b], Want[b], RDt, 2.f);
		if (Want[b] <= 0.f && Cur[b] < 1.e-4f) Cur[b] = 0.f;
		// each bed drifts a decibel up and down on a slow clock of its own, so that no two minutes are mixed alike
		const float Drift = 1.f + 0.11f * FMath::Sin(Clock * (2.f * PI / (23.f + 7.f * b)) + 1.7f * b);
		const float Vol = Cur[b] * Gate * Under * Drift;
		UAudioComponent* V = BedVoices[b];
		if (Vol > 1.e-4f && BedSounds[b])
		{
			if (!V)
			{
				V = UGameplayStatics::CreateSound2D(GM, BedSounds[b], Vol, 1.f, 0.f, nullptr, false, false);
				BedVoices[b] = V;
			}
			if (V)
			{
				if (!V->IsPlaying())
				{
					if (Playing >= FTAmbience::MaxBedVoices)
					{
						if (!bOverBudget) { bOverBudget = true; UE_LOG(LogFT, Warning, TEXT("FT AMB level=L%d km=%.2f more than %d beds at once: %s waits (the table's fault)"), Level + 1, Km, FTAmbience::MaxBedVoices, *Table.Beds[b].Name); }
						continue;
					}
					// (a loop has no beginning: each bed comes in somewhere else in its loop every time)
					V->Play((float)FMath::Fmod(FPlatformTime::Seconds() * 0.37 + 2.3 * b, 7.0));
				}
				V->SetVolumeMultiplier(Vol);
				++Playing;
			}
		}
		else if (V && V->IsPlaying())
		{
			V->Stop();
		}
	}

	// ---- the passes: one voice. A one-shot whose moment has come takes it; otherwise the nearest spot in range has it.
	const float SpeedMs = FMath::Max(Car->Speed / 100.f, 20.f);
	int32 Due = INDEX_NONE, Spot = INDEX_NONE;
	float SpotGain = 0.f;
	for (int32 p = 0; p < Table.Passes.Num(); ++p)
	{
		if (!PassSounds.IsValidIndex(p) || !PassSounds[p]) continue;               // its sound is not in the project: it does not exist
		const FFTAmbPass& P = Table.Passes[p];
		const float AheadM = (P.Km - Km) * 1000.f;
		if (FMath::Abs(AheadM) > P.RangeM + 60.f) { PassDone[p] = 0; continue; }   // out of range: ready for the next approach
		if (P.bSpot)
		{
			if (PassDone[p]) continue;                                             // it could not start on this approach: not tried again until the car has left its range
			const float G = FTAmbience::PassGain(Table, P, Km);
			if (G > SpotGain) { SpotGain = G; Spot = p; }
		}
		else if (!PassDone[p] && Due == INDEX_NONE && AheadM > -20.f && AheadM <= P.Climax * SpeedMs && FMath::Abs(AheadM) < P.RangeM)
		{
			Due = p;
		}
	}
	const bool bVoiceBusy = PassVoice && PassVoice->IsPlaying() && PassNow != INDEX_NONE;
	const bool bOneShotPlaying = bVoiceBusy && !Table.Passes[PassNow].bSpot;
	if (Gate > 0.01f)
	{
		if (Due != INDEX_NONE)
		{
			PassDone[Due] = 1;
			const float G = FTAmbience::PassGain(Table, Table.Passes[Due], Km) * Gate * Under;
			StartPass(GM, Due, G);
			UE_LOG(LogFT, Display, TEXT("FT AMB pass level=L%d km=%.2f name=\"%s\" asset=%s side=%d vol=%.3f started=%d"),
				Level + 1, Km, *Table.Passes[Due].Name, *Table.Passes[Due].Asset, Table.Passes[Due].Side, G, PassNow == Due ? 1 : 0);
		}
		else if (Spot != INDEX_NONE && !bOneShotPlaying && (!bVoiceBusy || PassNow != Spot) && SpotGain * Gate * Under > 1.e-4f)
		{
			const bool bSameSound = bVoiceBusy && Table.Passes[PassNow].Asset == Table.Passes[Spot].Asset;
			if (bSameSound)
			{
				// the next wheel's organ: the same loop, from its new place
				PassNow = Spot;
				if (const AFTTrack* Track = GM->GetTrack())
				{
					const FFTAmbPass& P = Table.Passes[Spot];
					PassVoice->SetWorldLocation(Track->ToWorld(P.Km * 100000.f, P.Side * P.OutM * 100.f, P.UpM * 100.f));
				}
			}
			else
			{
				StartPass(GM, Spot, SpotGain * Gate * Under);
				// a spot that could not start (no voice could be made) is remembered, like a one-shot that has played:
				// it is not tried, and nothing is logged, again until the car has left its range and come back
				if (PassNow != Spot) PassDone[Spot] = 1;
			}
			if (PassNow == Spot)
			{
				UE_LOG(LogFT, Display, TEXT("FT AMB spot level=L%d km=%.2f name=\"%s\" asset=%s side=%d started=%d"),
					Level + 1, Km, *Table.Passes[Spot].Name, *Table.Passes[Spot].Asset, Table.Passes[Spot].Side, 1);
			}
		}
	}
	float PassVol = 0.f, PassPitch = 1.f;
	if (PassNow != INDEX_NONE)
	{
		const FFTAmbPass& P = Table.Passes[PassNow];
		PassVol = FTAmbience::PassGain(Table, P, Km) * Gate * Under;
		if (!PassVoice || !PassVoice->IsPlaying() || (P.bSpot && PassVol <= 1.e-4f))
		{
			if (PassVoice && PassVoice->IsPlaying()) PassVoice->Stop();
			UE_LOG(LogFT, Display, TEXT("FT AMB pass level=L%d km=%.2f name=\"%s\" over"), Level + 1, Km, *P.Name);
			PassNow = INDEX_NONE;
		}
		else
		{
			// the fall in pitch of driving past: the car's speed towards the mark against the speed of sound
			const float AheadM = (P.Km - Km) * 1000.f;
			const float Aside = FMath::Max(15.f, FMath::Sqrt(P.OutM * P.OutM + P.UpM * P.UpM));
			const float Towards = (Car->Speed / 100.f) * AheadM / FMath::Sqrt(AheadM * AheadM + Aside * Aside);
			PassPitch = FMath::Clamp(1.f / (1.f - P.Doppler * Towards / 343.f), 0.8f, 1.25f);
			PassVoice->SetVolumeMultiplier(PassVol);
			PassVoice->SetPitchMultiplier(PassPitch);
		}
	}

	// ---- the log: a line when the district changes, and with ft.AmbienceLog 1 a line every two seconds
	const int32 District = FTAmbience::DistrictAt(Table, Km);
	if (District != DistrictNow && Gate > 0.f)
	{
		DistrictNow = District;
		FString Beds;
		if (Table.Districts.IsValidIndex(District))
		{
			for (const FFTAmbMix& M : Table.Districts[District].Mix)
			{
				if (Table.Beds.IsValidIndex(M.Bed)) Beds += FString::Printf(TEXT("%s%s:-%.0f"), Beds.IsEmpty() ? TEXT("") : TEXT(","), *Table.Beds[M.Bed].Name, M.Under);
			}
		}
		UE_LOG(LogFT, Display, TEXT("FT AMB district level=L%d km=%.2f name=\"%s\" beds=%s (dB under the music)"),
			Level + 1, Km, Table.Districts.IsValidIndex(District) ? *Table.Districts[District].Name : TEXT("none"), *Beds);
	}
	LogT += RDt;
	if (LogT >= 2.f && CVarFTAmbienceLog.GetValueOnGameThread() != 0)
	{
		LogT = 0.f;
		FString Beds;
		for (int32 b = 0; b < Table.Beds.Num(); ++b)
		{
			const UAudioComponent* V = BedVoices[b];
			if (V && V->IsPlaying()) Beds += FString::Printf(TEXT(" %s=%.3f"), *Table.Beds[b].Name, V->VolumeMultiplier);
		}
		UE_LOG(LogFT, Display, TEXT("FT AMB state level=L%d km=%.2f district=\"%s\" gate=%.2f duck=%.2f boss=%d voices=%d beds:%s pass=%s vol=%.3f pitch=%.3f"),
			Level + 1, Km, Table.Districts.IsValidIndex(District) ? *Table.Districts[District].Name : TEXT("none"), Gate, Duck, GM->bBossOn ? 1 : 0,
			Playing + (PassNow != INDEX_NONE ? 1 : 0), *Beds, PassNow != INDEX_NONE ? *Table.Passes[PassNow].Asset : TEXT("-"), PassVol, PassPitch);
	}
}
