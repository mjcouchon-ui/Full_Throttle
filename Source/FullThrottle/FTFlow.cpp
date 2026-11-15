// FULL THROTTLE 2.6: the progression of design v6 on the race (spec 01-10): car stats at ACTION!, AP/HE ammo,
// Chain Reaction, the Syndicate's road paces (slide, LEFT BEHIND), the boss escape clock and LAST CHANCE, the
// paycheck commit, PRE-PRODUCTION (skills, garage, custom, SFX DEPT, extras), the cosmetics in the race and the
// FT* test harness (spec 12 §9; development builds).
#include "FTGameMode.h"
#include "FTTrack.h"
#include "FTPlayerCar.h"
#include "FTEntity.h"
#include "FTFx.h"
#include "FTPlayerController.h"
#include "FTGameInstance.h"
#include "FTProgress.h"
#include "FTNightFlow.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/PointLightComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundWave.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "HAL/IConsoleManager.h"
#include "InputKeyEventArgs.h"
#include "Misc/PackageName.h"

using FT::XU; using FT::DU; using FT::RoadHalf; using FT::LaneX; using FT::BaseSpeed;

namespace
{
	template <typename T> T* Load(const FString& Path) { return Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *Path, nullptr, LOAD_NoWarn | LOAD_Quiet)); }
	inline float R(float A, float B) { return FMath::FRandRange(A, B); }
	const FLinearColor CYellow(1.f, 0.82f, 0.25f), CPink(1.f, 0.31f, 0.65f), CCyan(0.21f, 0.95f, 1.f), CRed(1.f, 0.25f, 0.25f), CGreen(0.4f, 1.f, 0.5f);
	constexpr float CarW = 256.f, CarL = 480.f;
	bool IsSyn(EFTKind K) { return K == EFTKind::Moto || K == EFTKind::Rammer || K == EFTKind::Suv || K == EFTKind::Truck || K == EFTKind::Heli || K == EFTKind::Hog || K == EFTKind::Boss; }
	FLinearColor Lin(const FLinearColor& Srgb) { return FLinearColor(FColor(FMath::RoundToInt(Srgb.R * 255.f), FMath::RoundToInt(Srgb.G * 255.f), FMath::RoundToInt(Srgb.B * 255.f))); }
	FLinearColor HexS(const TCHAR* H) { const FColor C = FColor::FromHex(H); return FLinearColor(C.R / 255.f, C.G / 255.f, C.B / 255.f); }
}

FString AFTGameMode::KindName(EFTKind K)
{
	switch (K)
	{
	case EFTKind::Moto: return TEXT("Biker"); case EFTKind::Hog: return TEXT("SnakeEyes"); case EFTKind::Rammer: return TEXT("Rammer");
	case EFTKind::Suv: return TEXT("SUV"); case EFTKind::Truck: return TEXT("Truck"); case EFTKind::Heli: return TEXT("Chopper");
	case EFTKind::Boss: return TEXT("Boss"); case EFTKind::Civ: return TEXT("Civilian"); case EFTKind::Barrel: return TEXT("Barrel");
	case EFTKind::Henchman: return TEXT("Henchman"); case EFTKind::Crate: return TEXT("Crate"); case EFTKind::Wrench: return TEXT("Wrench");
	default: return TEXT("Thing");
	}
}

// ============================================================================================== stats at ACTION!
void AFTGameMode::ApplyStats()
{
	const FFTProgressData& D = FFTProgressData::Get();
	UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(this);
	UFTGameInstance* G = GI();
	Stats = FFTCarStats();
	auto Rk = [CS](const TCHAR* K) { return CS ? CS->Rank(K) : 0; };
	auto V = [&](const TCHAR* K, const TCHAR* F, float Def) { const FFTGarageLine* L = D.Line(K); return L ? L->Value(F, Rk(K), Def) : Def; };
	Stats.MaxArmor = V(TEXT("armor"), TEXT("maxArmor"), 100.f);
	Stats.FireInterval = V(TEXT("guns"), TEXT("fireInterval"), 0.085f);
	Stats.MissileStart = FMath::RoundToInt(V(TEXT("missiles"), TEXT("missileStart"), 6.f));
	Stats.MissileCap = FMath::RoundToInt(V(TEXT("missiles"), TEXT("missileCap"), 9.f));
	Stats.TurboTank = V(TEXT("super"), TEXT("turboTank"), 100.f);
	Stats.TurboRefill = V(TEXT("super"), TEXT("turboRefill"), 13.f);
	Stats.FlameLen = V(TEXT("super"), TEXT("flameLength"), 10.f);
	Stats.RamDmg = V(TEXT("bullbar"), TEXT("ramDamage"), 6.f);
	Stats.RamDmgBig = V(TEXT("bullbar"), TEXT("ramDamageBig"), 4.f);
	Stats.BumpDmg = V(TEXT("bullbar"), TEXT("bumpDamage"), 1.f);
	Stats.DriftChargeMul = V(TEXT("tyres"), TEXT("driftChargeMult"), 1.f);
	Stats.BendPullMul = V(TEXT("tyres"), TEXT("bendPullMult"), 1.f);
	Stats.SpeedUpAdd = V(TEXT("engine"), TEXT("speedUpAdd"), 90.f);
	Stats.Accel = V(TEXT("gearbox"), TEXT("accel"), 420.f);
	Stats.AccelHot = V(TEXT("gearbox"), TEXT("accelHot"), 900.f);
	Stats.PunchAdd = V(TEXT("gearbox"), TEXT("punchAdd"), 90.f);
	Stats.PunchCap = V(TEXT("gearbox"), TEXT("punchCap"), 700.f);
	Stats.ApRank = Rk(TEXT("ap")); Stats.HeRank = Rk(TEXT("he"));
	Stats.ApRounds = FMath::RoundToInt(V(TEXT("ap"), TEXT("rounds"), 0.f));
	Stats.HeRounds = FMath::RoundToInt(V(TEXT("he"), TEXT("rounds"), 0.f));
	Stats.HeRadius = V(TEXT("he"), TEXT("blastRadius"), 0.f);
	Stats.HeDamage = V(TEXT("he"), TEXT("blastDamage"), 0.f);
	if (CS) for (const FName& S : CS->C.Skills) Stats.Skills.Add(S);
	Stats.PunchOff = Stats.Has(TEXT("GET1")) ? 0.6f : 1.f;
	Stats.Reels = 3 + (Stats.Has(TEXT("CAR4")) ? 1 : 0) + (CS ? CS->C.Film : 0);
	bSequelRun = (CS && CS->C.bSequel && CS->C.bSequelOn) || (G && G->bHarnessSequel);
	if (G) G->bHarnessSequel = false;
	// Race Tyres: the flat-bend pull is the share of a bend the car does not follow by itself
	if (Car) Car->RoadFollow = 1.f - (1.f - BaseRoadFollow) * Stats.BendPullMul;
	if (CS) for (const FFTGarageLine& L : D.Garage) { UE_LOG(LogFT, Display, TEXT("FT STATS line=%s rank=%d"), *L.Key.ToString(), CS->Rank(L.Key)); }
	FString Sk; for (const FName& S : Stats.Skills) Sk += S.ToString() + TEXT(" ");
	UE_LOG(LogFT, Display, TEXT("FT STATS maxArmor=%.0f fire=%.3f missiles=%d/%d tank=%.0f refill=%.0f speedUp=%.0f accel=%.0f/%.0f punch=%.0f/%.0f reels=%d ap=%d he=%d skills=[%s] sequel=%d"),
		Stats.MaxArmor, Stats.FireInterval, Stats.MissileStart, Stats.MissileCap, Stats.TurboTank, Stats.TurboRefill, Stats.SpeedUpAdd + (Stats.Has(TEXT("GET1")) ? 20.f : 0.f),
		Stats.Accel, Stats.AccelHot, Stats.PunchAdd, Stats.PunchCap, Stats.Reels, Stats.ApRounds, Stats.HeRounds, *Sk, bSequelRun ? 1 : 0);
}

void AFTGameMode::Checkpoint()
{
	CkScore = Score; CkDest = ScoreDestruction; CkNear = ScoreNear; CkStunts = ScoreStunts; CkColl = ScoreCollateral;
	CkKills = Kills; CkNearN = NearMisses; CkStuntN = StuntCount; CkCollN = Collateral; CkHp = Hp; CkMissiles = Missiles;
	RunCk = Run;
}

void AFTGameMode::Restore()
{
	Run = RunCk;
	// special rounds: max(rounds at the top of the scene, a quarter of the magazine) (06 §4)
	for (int32 i = 0; i < 2; ++i) { const int32 Mag = AmmoMag(i + 1); if (Mag > 0) Run.Rounds[i] = FMath::Max(RunCk.Rounds[i], Mag / 4); }
	if (AmmoLoad > 0 && Run.Rounds[AmmoLoad - 1] <= 0) AmmoLoad = 0;
	UE_LOG(LogFT, Display, TEXT("FT AMMO load=%s rounds=%d ap=%d he=%d reason=retake"), AmmoLoad == 1 ? TEXT("AP") : (AmmoLoad == 2 ? TEXT("HE") : TEXT("STD")), AmmoLoad > 0 ? Run.Rounds[AmmoLoad - 1] : 0, Run.Rounds[0], Run.Rounds[1]);
}

void AFTGameMode::AddXP(int32 N, const TCHAR* Source)
{
	if (N <= 0) return;
	Run.XP += N;
	UE_LOG(LogFT, Display, TEXT("FT XP delta=%d levelXP=%d source=%s"), N, Run.XP, Source);
}

void AFTGameMode::LandType(const FString& Name)
{
	const FString T = FFTProgressData::StuntType(Name);
	if (!Run.Types.Contains(T)) Run.Types.Add(T);
	const UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(this);
	if (CS && !CS->HasFeat(T) && !Run.NewFeats.Contains(T))
	{
		Run.NewFeats.Add(T);
		UE_LOG(LogFT, Display, TEXT("FT FEAT added=\"%s\""), *T);
		AddXP((int32)FFTProgressData::Get().T(TEXT("XPFirstStunt"), 100.f), TEXT("FirstStunt"));
	}
}

// ============================================================================================== ammo (06)
void AFTGameMode::CycleAmmo()
{
	const int32 Was = AmmoLoad;
	for (int32 Step = 1; Step <= 3; ++Step)
	{
		const int32 N = (AmmoLoad + Step) % 3;
		if (N == 0 || (N == 1 && Stats.ApRank > 0 && Run.Rounds[0] > 0) || (N == 2 && Stats.HeRank > 0 && Run.Rounds[1] > 0)) { AmmoLoad = N; break; }
	}
	static const TCHAR* Names[3] = { TEXT("STD"), TEXT("AP"), TEXT("HE") };
	UE_LOG(LogFT, Display, TEXT("FT AMMO load=%s rounds=%d from=%s"), Names[AmmoLoad], AmmoLoad > 0 ? Run.Rounds[AmmoLoad - 1] : 0, Names[Was]);
	Sfx(AmmoLoad != Was ? TEXT("DriftLevel") : TEXT("Empty"), 0.5f, AmmoLoad == 2 ? 0.8f : 1.2f);
}

void AFTGameMode::BulletHit(FFTEnt& B, FFTEnt& O)
{
	const FFTKindRow* KR = FFTProgressData::Get().Kind(O.Kind);
	const int32 Cls = O.Kind == EFTKind::Boss ? 2 : (KR ? KR->Class : -1);   // classes use base armor, never SEQUEL's (06 §2)
	float Dmg = 1.f;
	if (O.Kind == EFTKind::Civ || O.Kind == EFTKind::Barrel) Dmg = 1.f;
	else if (B.Load == 1) Dmg = Cls == 2 ? 2.f : 1.f;                        // AP: 2 to HEAVY
	else if (B.Load == 2) Dmg = Cls == 0 ? 2.f : 1.f;                        // HE: 2 to LIGHT
	else if (Stats.Has(TEXT("DEM2")) && (++HeavyCount % 4) == 0) Dmg = 2.f;  // Heavy Rounds: every 4th standard round
	const float Bd = O.D, Bx = O.X;
	const bool bLight = Cls == 0;
	// 3.1: what the round did, by its load and by what it hit. On the target (it is doing 100 mph too), at its tail,
	// in the round's own lane; +X is the round's way. Looks only, before the damage (a wreck lets its proxy go)
	if (USceneComponent* On = O.Proxy.IsValid() ? O.Proxy->GetRootComponent() : nullptr)
	{
		const FTransform At = Track->TransformAt(O.D - O.L * 0.5f, FMath::Clamp(B.X, O.X - O.W * 0.5f, O.X + O.W * 0.5f), O.bAir ? O.H : 110.f);
		if (Cls == 2 && B.Load != 1)
		{
			// armour shrugs a standard or HE round off: sparks come back off the plate, nothing goes in
			FTFx::Attach(TEXT("NS_ImpactArmor"), On, At.GetLocation(), At.Rotator());
			FTFx::Attach(TEXT("NS_ImpactRing"), On, At.GetLocation(), At.Rotator(), 1.f, FTFx::Extra);
		}
		else
		{
			static const TCHAR* Star[3] = { TEXT("NS_ImpactStar"), TEXT("NS_ImpactStarAP"), TEXT("NS_ImpactStarHE") };
			FTFx::Attach(Star[FMath::Clamp(B.Load, 0, 2)], On, At.GetLocation(), At.Rotator());
			// AP goes on through (further through armour, which is what it is for); the others throw sparks
			if (B.Load == 1) FTFx::Attach(TEXT("NS_ImpactPierce"), On, At.GetLocation(), At.Rotator(), Cls == 2 ? 1.3f : 1.f);
			else FTFx::Attach(TEXT("NS_Impact"), On, At.GetLocation(), At.Rotator(), 1.f, FTFx::Extra);
		}
	}
	O.Load = B.Load;
	HitLoad = B.Load;
	HurtEnt(O, Dmg, 0);
	HitLoad = 0;
	if (B.Load == 2) HeBlast(O, Bd, Bx);
	// Sabot Penetrator (AP rank 3): through LIGHT kinds to what is behind; each target once
	if (B.Load == 1 && Stats.ApRank >= 3 && bLight) B.Hits.Add(O.Id);
	else Release(B);
}

void AFTGameMode::HeBlast(const FFTEnt& Direct, float D, float X)
{
	const float Rr = Stats.HeRadius, Dmg = Stats.HeDamage;
	if (Rr <= 0.f) return;
	// the burst: a small explosion whose ring is the blast's real radius
	{
		// 3.1: its own blast: an orange burst and a ring that ends at the blast's real reach, then the sparks as before
		const FVector At = Track->ToWorld(D, X, 110.f);
		FTFx::Burst(this, TEXT("NS_HeBurst"), At, FRotator::ZeroRotator, FMath::Max(0.8f, Rr / 24.f));
		FTFx::Burst(this, TEXT("NS_BlastRing"), At, FRotator::ZeroRotator, Rr * XU / 100.f);
		FTFx::Burst(this, TEXT("NS_ExplosionSparks"), At, FRotator::ZeroRotator, 0.35f * Rr / 24.f, FTFx::Extra);
	}
	Sfx(TEXT("Explode"), 0.35f, 1.4f);
	const int32 DirectId = Direct.Id;
	for (FFTEnt& E : Ents)
	{
		if (E.bDead || E.Id == DirectId) continue;
		// LIGHT vehicles, SUVs, civilians and barrels; never HEAVY kinds, pickups, henchmen or the player
		const bool bHits = E.Kind == EFTKind::Moto || E.Kind == EFTKind::Hog || E.Kind == EFTKind::Rammer || E.Kind == EFTKind::Suv || E.Kind == EFTKind::Civ || E.Kind == EFTKind::Barrel;
		if (!bHits) continue;
		if (FMath::Abs(E.X - X) < Rr * XU + E.W / 2 && FMath::Abs(E.D - D) < Rr * DU + E.L / 2) { E.Load = 2; HurtEnt(E, Dmg, 4); }
	}
}

void AFTGameMode::ChainBlast(const FFTEnt& E)
{
	// Chain Reaction (DEM4): 4 to everything within 60 px (box), the barrel's player rule
	const float Ex = E.X, Ed = E.D; const int32 Id = E.Id;
	{
		// 3.1: Chain Reaction had no look of its own: a gold star and a gold ring out to its real reach (60 px)
		const FVector At = Track->ToWorld(Ed, Ex, 110.f);
		FTFx::Burst(this, TEXT("NS_ChainBurst"), At, FRotator::ZeroRotator, 1.f, FTFx::Flash);
		FTFx::Burst(this, TEXT("NS_BlastRingGold"), At, FRotator::ZeroRotator, 60.f * XU / 100.f);
	}
	for (FFTEnt& O : Ents)
	{
		if (O.bDead || O.Id == Id || O.Kind == EFTKind::Bullet || O.Kind == EFTKind::Missile || O.Kind == EFTKind::Crate || O.Kind == EFTKind::Wrench || O.Kind == EFTKind::Ramp || O.Kind == EFTKind::Henchman) continue;
		if (FMath::Abs(O.X - Ex) < 60 * XU && FMath::Abs(O.D - Ed) < 60 * DU) HurtEnt(O, 4.f, 5);
	}
	if (FMath::Abs(Car->X - Ex) < 34 * XU + CarW / 2 && FMath::Abs(Car->D - Ed) < 40 * DU + CarL / 2) HurtPlayer(10.f);
}

// ============================================================================================== speed track (05)
void AFTGameMode::TickSlide(FFTEnt& E, float Dt)
{
	const FFTKindRow* K = FFTProgressData::Get().Kind(E.Kind);
	if (!K || K->bKeepsUp || K->Pace <= 0.f) return;
	const FFTProgressData& D = FFTProgressData::Get();
	const float Vp = FMath::Max(0.f, Car->Speed) / DU;
	const float Was = E.Slide;
	if (Vp > K->Pace) E.Slide += D.T(TEXT("SlideFactor"), 0.5f) * (Vp - K->Pace) * Dt;
	else E.Slide = FMath::Max(0.f, E.Slide - FMath::Max(D.T(TEXT("SlideReturnMin"), 60.f), K->Pace - Vp) * Dt);
	if (Was <= 0.f && E.Slide > 0.f) { UE_LOG(LogFT, Display, TEXT("FT SLIDE kind=%s g=%.0f start=1 player=%.0f pace=%.0f"), *KindName(E.Kind), E.Slide, Vp, K->Pace); }
	else if (Was > 0.f && E.Slide <= 0.f) { UE_LOG(LogFT, Display, TEXT("FT SLIDE kind=%s g=0 back=1"), *KindName(E.Kind)); }
	else if (E.Slide > 0.f && FMath::FloorToInt(Was / 60.f) != FMath::FloorToInt(E.Slide / 60.f)) { UE_LOG(LogFT, Display, TEXT("FT SLIDE kind=%s g=%.0f"), *KindName(E.Kind), E.Slide); }
}

// 05 §2 in the spec's screen space: a car leaves when its slide g carries it from its home spot past the bottom of
// the 600 px screen (centre y > H + h). The port keeps each kind's home spot as its distance ahead (Ta = px x DU, the
// truck x1.4), so the same g is mapped onto the road from that spot to just behind the camera. Snake Eyes ride
// beside the door; their spot is taken as 275 px, which reproduces the spec's 2.7 s table time (assumption).
float AFTGameMode::SlideLeavePx(const FFTEnt& E) const
{
	const FFTKindRow* K = FFTProgressData::Get().Kind(E.Kind);
	const float H = K ? K->HeightPx : 40.f;
	const float HomeY = E.Kind == EFTKind::Hog ? 275.f : E.Ta / DU / (E.Kind == EFTKind::Truck ? 1.4f : 1.f);
	return FMath::Max(60.f, 600.f + H - HomeY);
}

float AFTGameMode::SlideCm(const FFTEnt& E) const
{
	if (E.Slide <= 0.f) return 0.f;
	const float F = FMath::Clamp(E.Slide / SlideLeavePx(E), 0.f, 1.f);
	return F * ((E.Kind == EFTKind::Hog ? 0.f : E.Ta) + CarL + E.L + 600.f);
}

void AFTGameMode::LeftBehindEnt(FFTEnt& E)
{
	Run.LeftBehind++;
	UE_LOG(LogFT, Display, TEXT("FT LEFTBEHIND kind=%s count=%d"), *KindName(E.Kind), Run.LeftBehind);
	if (Stats.Has(TEXT("GET3")) && !E.bTouched)
	{
		Turbo = FMath::Min(Stats.TurboTank, Turbo + 8.f);
		UE_LOG(LogFT, Display, TEXT("FT TURBO delta=+8 source=CleanPass"));
	}
	Popup(TEXT("LEFT BEHIND"), CCyan);
	Release(E);
}

void AFTGameMode::TickBossClock(FFTEnt& B, float Dt, float& Want)
{
	// The opening lesson has no failure deadline; the encounter resumes on completion or skip.
	if (IsTutorialActive()) { BossRate = 0.f; return; }
	const FFTProgressData& D = FFTProgressData::Get();
	const FFTBossRow& BR = D.Bosses[FMath::Clamp(Level, 0, D.Bosses.Num() - 1)];
	const float Vp = FMath::Max(0.f, Car->Speed) / DU;
	BossFightT += Dt;
	static float LogT = 0.f;
	if (!bLastChance)
	{
		BossRate = FMath::Clamp(1.f - (Vp - BR.Pace) / D.T(TEXT("BossClockDivisor"), 250.f), D.T(TEXT("BossClockMinRate"), 0.5f), 1.f);
		BossClock += Dt * BossRate;
		BossT = BossClock;
		LogT += Dt;
		if (LogT >= 0.5f) { LogT = 0.f; UE_LOG(LogFT, Display, TEXT("FT BOSS clock=%.1f rate=%.2f state=Fight fight=%.1f"), BossClock, BossRate, BossFightT); }
		if (BossRate < 1.f)
		{
			float& Cd = SayCool.FindOrAdd(TEXT("stayOnHim"), -100.f);
			if (RealClock - Cd > 9.f) { Cd = RealClock; SayText(TEXT("Stay on him! Don't let him get away!"), 2); UE_LOG(LogFT, Display, TEXT("FT DIRECTOR line=\"Stay on him! Don't let him get away!\"")); }
		}
		if (!bEscapeWarned && BossClock > BossEscapeT - (Level >= 2 ? 18.f : 15.f)) { bEscapeWarned = true; Say(TEXT("escape"), 3); }
		if (BossClock >= BossEscapeT)
		{
			// LAST CHANCE (05 §4): he floors it
			bLastChance = true; LastChanceT = 0.f; BossOffset = 0.f;
			SayText(TEXT("Don't lose him! FLOOR IT!"), 5); Shout(TEXT("FLOOR IT!"));
			Caption = TEXT("LAST CHANCE!"); CaptionT = RealClock;
			UE_LOG(LogFT, Display, TEXT("FT DIRECTOR line=\"Don't lose him! FLOOR IT!\""));
			UE_LOG(LogFT, Display, TEXT("FT BOSS state=LastChance clock=%.1f fight=%.1f"), BossClock, BossFightT);
		}
	}
	else
	{
		LastChanceT += Dt;
		BossOffset = FMath::Min(0.f, BossOffset + (Vp - BR.Flee) * Dt);
		if (BossOffset < -(BR.Spot + D.T(TEXT("LastChanceClearPx"), 75.f)) || LastChanceT >= D.T(TEXT("LastChanceSeconds"), 8.f))
		{
			UE_LOG(LogFT, Display, TEXT("FT BOSS state=Gone lastChance=%.1f offset=%.0f"), LastChanceT, BossOffset);
			bBossOn = false; Release(B); Wrap(true);
			return;
		}
	}
	Want -= BossOffset * DU * 1.25f;   // negative offset = up the screen = further ahead
}

// ============================================================================================== the paycheck
void AFTGameMode::CommitLevel()
{
	bPayValid = false;
	UFTGameInstance* G = GI();
	UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(this);
	if (!CS || (G && G->bBench)) return;
	FFTLevelCommit In;
	FFTLevelResult& R = In.R;
	R.Level = Level;
	R.Outcome = bOutOfFilm ? EFTOutcome::OutOfFilm : (bEscaped ? EFTOutcome::GotAway : EFTOutcome::Wrap);
	R.D = (double)ScoreDestruction; R.S = (double)ScoreStunts; R.N = (double)ScoreNear; R.BossAddPoints = (double)Run.BossAddPts;
	R.Wrecked = Run.Wrecked; R.ScriptTotal = ScriptTotal; R.StuntTypes = Run.Types.Num(); R.Rating = RatingIdx;
	R.Retakes = bOutOfFilm ? Stats.Reels : Retakes; R.ArmorPct = FMath::Clamp(FMath::RoundToInt(HpPct()), 0, 100); R.Civilians = Run.Civilians;
	R.MultiKills = Run.MultiKills; R.Barrels = Run.Barrels; R.LevelTime = R.Outcome == EFTOutcome::Wrap ? LevelClock : 0.0;
	R.bBossWrecked = bBossDead && !bEscaped && !bOutOfFilm;
	// Director's Notes (01 §6), from the level counters
	const bool bWrap = R.Outcome == EFTOutcome::Wrap;
	TArray<FString> Done;
	if (Level == 0) { if (R.bBossWrecked && BossFightT < 40.f) Done.Add(TEXT("L1-boss40")); if (Run.bRoof) Done.Add(TEXT("L1-roof")); if (bWrap && Run.Civilians == 0) Done.Add(TEXT("L1-clean")); }
	if (Level == 1) { if (Run.bPipe3) Done.Add(TEXT("L2-pipe")); if (Run.Shock >= 5) Done.Add(TEXT("L2-shock5")); if (Run.bGrav2) Done.Add(TEXT("L2-grav2")); }
	if (Level == 2) { if (Run.Slams >= 10) Done.Add(TEXT("L3-slam10")); if ((Run.AirWash & 3) == 3) Done.Add(TEXT("L3-air")); if (R.bBossWrecked && BossFightT < 60.f) Done.Add(TEXT("L3-boss60")); }
	for (const FString& N : Done) if (!CS->C.Notes.FindRef(N)) { In.NotesDone.Add(N); UE_LOG(LogFT, Display, TEXT("FT NOTE done=%s"), *N); }
	R.NotesCompleted = In.NotesDone.Num();
	In.XP = Run.XP; In.NewFeats = Run.NewFeats; In.LeftBehind = Run.LeftBehind; In.Trucks = Run.Trucks; In.SnakeEyes = Run.SnakeEyes;
	In.bRunDown = bRunDown && R.bBossWrecked; In.ExactCarnage = (double)Run.Wrecked / FMath::Max(1, ScriptTotal); In.BestScore = (int32)Score;
	UE_LOG(LogFT, Display, TEXT("FT CARNAGE wrecked=%d total=%d shown=%d exact=%.4f leftBehind=%d final=1"), Run.Wrecked, ScriptTotal, (int32)FMath::RoundToInt(In.ExactCarnage * 100.0), In.ExactCarnage, Run.LeftBehind);
	// Rewards and the next resumable step are one durable transaction. Presentation
	// screens never become a checkpoint that can pay this take twice.
	const int32 NextLevel = Level + 1 < FTData::NumLevels() ? Level + 1 : -2;
	const TCHAR* Args = FCommandLine::Get();
	const bool bDays = !FParse::Param(Args, TEXT("ftnoday1")) && !FParse::Param(Args, TEXT("ftnodays")) &&
		!FParse::Param(Args, TEXT("ftnonights")) && FPackageName::DoesPackageExist(TEXT("/Game/Brawl/Maps/L_Day1Set")) &&
		(!(G && (G->bAutoDrive || G->bExecScript)) || FParse::Param(Args, TEXT("ftdays")));
	if (bWrap && bDays) { In.NextKind = TEXT("day"); In.NextLevel = Level; In.bNextWrap = true; }
	else if (bWrap && !FParse::Param(Args, TEXT("ftnonights")) && !UFTNightFlow::NightMapAfter(Level).IsEmpty())
	{
		In.NextKind = TEXT("night"); In.NextLevel = Level; In.NextStage = -1;
	}
	else
	{
		In.NextLevel = bWrap ? NextLevel : Level;
		In.NextKind = In.NextLevel < 0 ? TEXT("complete") : TEXT("preproduction");
	}
	Pay = CS->Commit(In);
	bPayValid = true;
	const bool bLast = Level >= FTData::NumLevels() - 1;
	PayNext = R.Outcome == EFTOutcome::Wrap ? (bLast ? 2 : 0) : 1;
}

void AFTGameMode::EnterPaycheck()
{
	Phase = EFTPhase::Paycheck; PhaseT = 0.f;
	ResultsContinueRect = FBox2D(ForceInit);
	UE_LOG(LogFT, Display, TEXT("FT SCREEN name=Paycheck lines=%d"), bPayValid ? Pay.Pay.Lines.Num() : 0);
	if (bBossMusic) SetBossMusic(false);   // 3.8
	Sfx(TEXT("Sting"), 0.5f);
}

void AFTGameMode::TickPaycheck(float RDt)
{
	AFTPlayerController* PC = Cast<AFTPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PC || PhaseT < 0.6f) return;
	static int32 LastLine = -1;
	const int32 Shown = FMath::FloorToInt((PhaseT - 0.3f) / 0.32f);
	if (Shown != LastLine && bPayValid && Shown >= 0 && Shown <= Pay.Pay.Lines.Num()) { LastLine = Shown; Sfx(TEXT("Pickup"), 0.25f, 1.4f); }
	auto Next = [this](int32 Kind)
	{
		if (RequireProgressSaved([this, Kind]() { ContinuePaycheck(Kind); })) ContinuePaycheck(Kind);
	};
	if (PC->Hit(EKeys::R) || PC->Hit(EKeys::Gamepad_FaceButton_Top)) { PayNext = 1; Next(1); return; }
	if (PC->MenuBack() || PC->Hit(EKeys::B)) { RequestSystemQuit(false); return; }
	if (PC->MenuOk() || PC->Clicked(ResultsContinueRect)) Next(PayNext);
}

void AFTGameMode::ContinuePaycheck(int32 Kind)
{
	const int32 To = Kind == 2 ? -2 : (Kind == 1 ? Level : Level + 1);
	const bool bLastWrap = Kind == 2 && Level >= FTData::NumLevels() - 1 && !bEscaped && !bOutOfFilm && bPayValid;
	if ((Kind == 0 || bLastWrap) && UFTNightFlow::GoToDayCut(this, Level, true)) return;
	if (Kind == 0 && UFTNightFlow::GoToNight(this, Level, To)) return;
	EnterPreProduction(To, false);
}

// ============================================================================================== PRE-PRODUCTION (10 §4)
FString AFTGameMode::PPHeader() const
{
	if (bPPFromTitle) return TEXT("PRE-PRODUCTION · FROM THE TITLE");
	if (PPNextLevel < 0) return TEXT("PRE-PRODUCTION · NEXT: BACK TO THE TITLE");
	const FFTLevelDef& L = FTData::Level(PPNextLevel);
	return FString::Printf(TEXT("PRE-PRODUCTION · NEXT: LEVEL %d · %s%s"), PPNextLevel + 1, *L.Title, PPNextLevel == Level && !bPPFromTitle ? TEXT(" (RESHOOT)") : TEXT(""));
}

void AFTGameMode::EnterPreProduction(int32 NextLevel, bool bFromTitle)
{
	UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(this);
	Phase = EFTPhase::PreProduction; PhaseT = 0.f;
	PPNextLevel = NextLevel; bPPFromTitle = bFromTitle;
	ClearEnts();
	UGameplayStatics::SetGlobalTimeDilation(this, 1.f); TimeScale = 1.f;
	// 3.8 (owner: the store plays the level's music): after a level (straight from the paycheck, or back from the night
	// on the finished level's map, where nothing played) the level's own track; from the title the title song plays on
	if (!bFromTitle) SetBossMusic(false);
	if (Music) Music->SetVolumeMultiplier(0.12f);
	if (UFTGameInstance* GT = GI()) GT->SetTitleMusicVolume(0.12f);   // 3.0: from the title the title song plays on, low
	BuyHoldT = 0.f; bBuyHoldDone = false;
	TabRects.Reset(); TileRects.Reset();
	ReadyRect = BuyRect = UndoRect = FBox2D(ForceInit); bReadyRectLogged = false;
	if (CS)
	{
		if (bFromTitle) CS->Save(TEXT("TitleShop")); // Preserve an active race/night Continue checkpoint.
		else
		{
			CS->C.PendingLevel = NextLevel; CS->C.bPendingFromTitle = false;
			CS->SetCheckpoint(NextLevel < 0 ? TEXT("complete") : TEXT("preproduction"), NextLevel < 0 ? Level : NextLevel, 0, false, TEXT("PreProduction"));
		}
	}
	if (AFTPlayerController* PC = Cast<AFTPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PC->SetMenuMouse(true);
		PC->BlockMenuInputUntilRelease();
	}
	if (CS && CS->SPLeft() > 0) Tab = EFTTab::Skills;
	BuildTiles();
	if (bFromTitle) { UE_LOG(LogFT, Display, TEXT("FT SCREEN name=PreProduction from=Title")); }
	else if (NextLevel < 0) { UE_LOG(LogFT, Display, TEXT("FT SCREEN name=PreProduction next=Title")); }
	else { UE_LOG(LogFT, Display, TEXT("FT SCREEN name=PreProduction next=L%d"), NextLevel + 1); }
	Sfx(TEXT("Sting"), 0.6f);
}

void AFTGameMode::LeavePreProduction()
{
	if (!RequireProgressSaved([this]() { LeavePreProduction(); })) return;
	UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(this);
	const bool bAccepted = CS && CS->Ready();
	if (!bAccepted || CS->bSaving || CS->bSaveError)
	{
		WaitForProgressSave([this]() { FinishPreProduction(); }, [this]()
		{
			UFTCareerSubsystem* Career = UFTCareerSubsystem::Get(this);
			return Career && Career->Ready();
		});
		return;
	}
	FinishPreProduction();
}

void AFTGameMode::FinishPreProduction()
{
	UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(this);
	UFTGameInstance* G = GI();
	if (AFTPlayerController* PC = Cast<AFTPlayerController>(GetWorld()->GetFirstPlayerController())) PC->SetMenuMouse(false);
	Sfx(TEXT("Clap"));
	if (bPPFromTitle)
	{
		// 10 §4 / 00 assumption 11: from the title, READY returns to the title
		UE_LOG(LogFT, Display, TEXT("FT SCREEN name=Title"));
		EnterTitle(false);
		return;
	}
	if (PPNextLevel < 0)
	{
		// the run is over: back to the title (after the last level, "THAT'S A WRAP")
		const bool bWrapBanner = PayNext == 2 && !bEscaped && !bOutOfFilm && Level >= FTData::NumLevels() - 1;
		if (CS && CS->C.Film != 0)
		{
			CS->C.Film = 0;
			const bool bAccepted = CS->Save(TEXT("RunEnd"));
			if (!bAccepted || CS->bSaving || CS->bSaveError)
			{
				WaitForProgressSave([this]() { FinishPreProduction(); });
				return;
			}
		}
		if (G) { G->TitleBanner = bWrapBanner ? TEXT("THAT'S A WRAP") : FString(); }
		UE_LOG(LogFT, Display, TEXT("FT SCREEN name=Title banner=\"%s\""), bWrapBanner ? TEXT("THAT'S A WRAP") : TEXT(""));
		if (bWrapBanner) GoToLevel(0, false);
		else EnterTitle(false);
		return;
	}
	UE_LOG(LogFT, Display, TEXT("FT SCREEN name=Slate next=L%d"), PPNextLevel + 1);
	// the next day's cutscene BEFORE THE DRIVE first (run 77694; not for a reshoot of the same level)
	if (PPNextLevel != Level && UFTNightFlow::GoToDayCut(this, PPNextLevel, false)) return;
	if (PPNextLevel == Level) StartRace(false);
	else GoToLevel(PPNextLevel, true);
}

void AFTGameMode::PPSay(const FString& M, bool bBad)
{
	const UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(this);
	if (!bBad && CS && CS->bSaving)
	{
		PPPendingMessage = M;
		PPMsg = TEXT("SAVING..."); PPMsgT = GetWorld()->GetRealTimeSeconds(); bPPMsgBad = false;
		UE_LOG(LogFT, Display, TEXT("FT SHOP save=pending result=%s"), *M);
		return;
	}
	PPMsg = M; PPMsgT = GetWorld()->GetRealTimeSeconds(); bPPMsgBad = bBad;
	UE_LOG(LogFT, Display, TEXT("FT SHOP save=%s message=%s"), bBad ? TEXT("failed") : TEXT("complete"), *M);
	Sfx(bBad ? TEXT("Empty") : TEXT("Pickup"), bBad ? 0.7f : 0.8f);
}

namespace
{
	FString RankText(const FFTGarageLine& G, int32 Rk)
	{
		auto V = [&](const TCHAR* F) { return G.Value(F, Rk); };
		auto V0 = [&](const TCHAR* F) { return G.Value(F, 0); };
		const FName K = G.Key;
		if (K == TEXT("armor")) return FString::Printf(TEXT("Max armor %.0f (stock %.0f). The bar shows a share of max."), V(TEXT("maxArmor")), V0(TEXT("maxArmor")));
		if (K == TEXT("guns")) return FString::Printf(TEXT("Fire interval %.3f s (stock %.3f), every ammo load."), V(TEXT("fireInterval")), V0(TEXT("fireInterval")));
		if (K == TEXT("missiles")) return FString::Printf(TEXT("%.0f missiles at the start, carry %.0f (stock %.0f / %.0f)."), V(TEXT("missileStart")), V(TEXT("missileCap")), V0(TEXT("missileStart")), V0(TEXT("missileCap")));
		if (K == TEXT("super")) return FString::Printf(TEXT("Turbo tank %.0f, refill %.0f a second: a full burn lasts %.1f s (stock 100 / 13 / 2.6 s)."), V(TEXT("turboTank")), V(TEXT("turboRefill")), V(TEXT("burnSeconds")));
		if (K == TEXT("bullbar")) return FString::Printf(TEXT("Turbo ram %.0f to the Syndicate, %.0f to trucks and bosses; a plain bump hits for %.0f (stock 6 / 4 / 1)."), V(TEXT("ramDamage")), V(TEXT("ramDamageBig")), V(TEXT("bumpDamage")));
		if (K == TEXT("tyres")) return FString::Printf(TEXT("Drift charges %+.0f%% faster; the pull on flat bends %.0f%%."), (V(TEXT("driftChargeMult")) - 1.f) * 100.f, (V(TEXT("bendPullMult")) - 1.f) * 100.f);
		if (K == TEXT("engine")) return FString::Printf(TEXT("SPEED UP adds %.0f: top %.0f su (%.0f MPH), turbo top %.0f su (%.0f MPH). Cruise stays 118 MPH."), V(TEXT("speedUpAdd")), V(TEXT("speedUpTop")), V(TEXT("speedUpMph")), V(TEXT("turboTop")), V(TEXT("turboMph")));
		if (K == TEXT("gearbox")) return FString::Printf(TEXT("Acceleration %.0f su/s (%.0f hot); turbo punch +%.0f, cap %.0f."), V(TEXT("accel")), V(TEXT("accelHot")), V(TEXT("punchAdd")), V(TEXT("punchCap")));
		if (K == TEXT("ap")) return FString::Printf(TEXT("%.0f rounds a level. 2 damage a round to trucks, choppers and bosses, 1 to anything else.%s"), V(TEXT("rounds")), Rk >= 3 ? TEXT(" Goes through bikers, Snake Eyes and rammers.") : TEXT(""));
		if (K == TEXT("he")) return FString::Printf(TEXT("%.0f rounds a level. 2 damage to bikers, Snake Eyes and rammers; a blast of %.0f within %.0f px to light cars, SUVs, civilians and barrels."), V(TEXT("rounds")), V(TEXT("blastDamage")), V(TEXT("blastRadius")));
		return FString();
	}
	const FLinearColor BranchCol[5] = { FLinearColor(1.f, 0.48f, 0.1f), FLinearColor(0.21f, 0.95f, 1.f), FLinearColor(1.f, 0.31f, 0.72f), FLinearColor(0.36f, 1.f, 0.45f), FLinearColor(1.f, 0.82f, 0.25f) };
}

void AFTGameMode::BuildTiles()
{
	Tiles.Reset();
	const FFTProgressData& D = FFTProgressData::Get();
	const UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(this);
	if (!CS || !D.bLoaded) return;
	auto Money = [](int64 V) { FString R = FString::Printf(TEXT("%lld"), V); for (int32 i = R.Len() - 3; i > 0; i -= 3) R.InsertAt(i, TEXT(",")); return TEXT("$") + R; };
	switch (Tab)
	{
	case EFTTab::Skills:
		for (const FFTSkillNodeRow& N : D.Nodes)
		{
			FFTTileView& T = Tiles.AddDefaulted_GetRef();
			int32 B = 0; for (int32 i = 0; i < D.Branches.Num(); ++i) if (D.Branches[i].Code == N.Branch) B = i;
			T.Id = N.Id.ToString(); T.Col = B; T.Row = N.Tier - 1; T.Title = N.Name.ToUpper(); T.Desc = N.Desc;
			T.Group = FString::Printf(TEXT("%s · TIER %d%s"), D.Branches.IsValidIndex(B) ? *D.Branches[B].Name : TEXT(""), N.Tier, N.bGreenlight ? TEXT(" · CAPSTONE") : TEXT(""));
			T.Price = FString::Printf(TEXT("%d SP%s"), N.SP, N.bGreenlight ? TEXT(" + GREENLIGHT") : TEXT(""));
			T.Sub = T.Id + TEXT(" · ") + T.Price; T.State = CS->SkillState(N.Id, &T.Needs); T.Color = BranchCol[B % 5];
		}
		break;
	case EFTTab::Garage:
	{
		static const TCHAR* Order[10] = { TEXT("armor"), TEXT("bullbar"), TEXT("guns"), TEXT("missiles"), TEXT("engine"), TEXT("gearbox"), TEXT("super"), TEXT("tyres"), TEXT("ap"), TEXT("he") };
		for (int32 r = 0; r < 10; ++r)
		{
			const FFTGarageLine* G = D.Line(Order[r]);
			if (!G) continue;
			for (int32 k = 1; k <= 3; ++k)
			{
				FFTTileView& T = Tiles.AddDefaulted_GetRef();
				T.Id = FString::Printf(TEXT("garage:%s:%d"), *G->Key.ToString(), k); T.Col = k - 1; T.Row = r;
				T.Title = G->Ranks[k - 1].Name.ToUpper(); T.Group = FString::Printf(TEXT("%s · %s · RANK %d"), *G->Row, *G->Name.ToUpper(), k);
				T.Cost = G->Ranks[k - 1].Price; T.Price = Money(T.Cost); T.bCash = true;
				T.Sub = T.Price;
				if (G->Key == TEXT("engine")) T.Sub += FString::Printf(TEXT(" · %.0f MPH"), G->Value(TEXT("speedUpMph"), k));
				if (G->Row == TEXT("AMMO")) T.Sub += FString::Printf(TEXT(" · %.0f RDS · %s"), G->Value(TEXT("rounds"), k), G->Key == TEXT("ap") ? TEXT("2x HEAVY") : TEXT("2x LIGHT + BLAST"));
				T.Desc = RankText(*G, k); T.State = CS->GarageState(G->Key, k, &T.Needs);
				T.Color = G->Row == TEXT("ARMOR") ? FLinearColor(0.36f, 1.f, 0.45f) : (G->Row == TEXT("WEAPONS") ? FLinearColor(1.f, 0.82f, 0.25f) : (G->Row == TEXT("ENGINE") ? FLinearColor(1.f, 0.31f, 0.72f) : (G->Row == TEXT("TYRES") ? FLinearColor(0.21f, 0.95f, 1.f) : FLinearColor(1.f, 0.48f, 0.1f))));
			}
		}
		break;
	}
	case EFTTab::Custom: case EFTTab::Sfx:
	{
		int32 i = 0;
		for (const FFTCosmeticRow& C : D.Cosmetics)
		{
			const bool bCustom = C.Shop != TEXT("SFX");
			if (bCustom != (Tab == EFTTab::Custom)) continue;
			FFTTileView& T = Tiles.AddDefaulted_GetRef();
			T.Id = C.Id.ToString(); T.Col = i % 6; T.Row = i / 6; ++i;
			static const TCHAR* TierN[5] = { TEXT("STOCK"), TEXT("B-MOVIE"), TEXT("ACTION HERO"), TEXT("BLOCKBUSTER"), TEXT("LEGENDARY") };
			T.Title = C.Name.ToUpper(); T.Group = FString::Printf(TEXT("%s · %s · %s"), C.Shop == TEXT("SFX") ? TEXT("SFX DEPT") : (C.Shop == TEXT("PAINT") ? TEXT("PAINT SHOP") : TEXT("RIMS")), *C.Group.ToUpper(), TierN[FMath::Clamp(C.Tier, 0, 4)]);
			T.Cost = C.Price; T.Price = C.Price > 0 ? Money(C.Price) : FString(TEXT("STOCK")); T.bCash = C.Price > 0; T.Sub = T.Price;
			T.Desc = C.Look + (C.Feat.IsSet() ? FString::Printf(TEXT(" · Needs: %s"), *C.Feat.Text) : FString()) + FString::Printf(TEXT(" · Driver level %d."), C.Level);
			T.State = CS->CosmeticState(C.Id, &T.Needs); T.bSwatch = C.bSwatch; T.Color = C.bSwatch ? C.Swatch : FLinearColor(0.8f, 0.8f, 0.85f); T.Slot = C.Slot;
		}
		break;
	}
	case EFTTab::Extras:
	{
		int32 i = 0;
		auto Add = [&](const FString& Id, const FString& Title, const FString& Desc, int64 Cost, EFTTile St, const FString& Needs, const FLinearColor& Col)
		{
			FFTTileView& T = Tiles.AddDefaulted_GetRef();
			T.Id = Id; T.Col = i % 4; T.Row = i / 4; ++i; T.Title = Title; T.Desc = Desc; T.Cost = Cost; T.bCash = Cost > 0;
			T.Price = Cost > 0 ? Money(Cost) : FString(TEXT("FREE")); T.Sub = T.Price; T.State = St; T.Needs = Needs; T.Color = Col; T.Group = TEXT("EXTRAS");
		};
		FString N;
		const EFTTile Fs = CS->FilmState(&N);
		Add(TEXT("film"), FString::Printf(TEXT("EXTRA REEL OF FILM (%d/%d)"), CS->C.Film, D.FilmMax), TEXT("One more retake this run (up to 2 a run; this run only)."), D.FilmPrice, Fs == EFTTile::Owned ? EFTTile::Owned : EFTTile::Available, N, CYellow);
		for (int32 b = 0; b < D.Branches.Num(); ++b)
		{
			const FFTBranchRow& Br = D.Branches[b];
			Add(TEXT("greenlight-") + Br.Code, FString::Printf(TEXT("GREENLIGHT: %s"), *Br.Name), FString::Printf(TEXT("The studio greenlights the %s capstone (%s6). Needed with the 4 SP; stays paid through respecs."), *Br.Name, *Br.Code), D.GreenlightPrice, CS->GreenlightState(Br.Code), FString(), BranchCol[b % 5]);
		}
		Add(TEXT("sequel"), CS->C.bSequel ? FString::Printf(TEXT("SEQUEL MODE: %s"), CS->C.bSequelOn ? TEXT("ON") : TEXT("OFF")) : FString(TEXT("SEQUEL MODE")), TEXT("Replay with Syndicate armor x1.3, rating bars x1.35 and paychecks x1.5. Bought once; ENTER switches it on or off."), CS->C.bSequel ? 0 : D.SequelPrice, CS->C.bSequel ? EFTTile::Owned : EFTTile::Available, FString(), CPink);
		Add(TEXT("respec"), TEXT("REWRITE THE SCRIPT (RESPEC)"), FString::Printf(TEXT("Refunds every skill point; greenlights stay paid. Used %d time%s."), CS->C.Respecs, CS->C.Respecs == 1 ? TEXT("") : TEXT("s")), CS->RespecPrice(), EFTTile::Available, FString(), CCyan);
		if (CS->RespecPrice() == 0) { Tiles.Last().bCash = true; Tiles.Last().Price = TEXT("FIRST ONE FREE"); Tiles.Last().Sub = Tiles.Last().Price; }
		break;
	}
	default: break;
	}
	int32& Sel = TabSel[(int32)Tab];
	Sel = FMath::Clamp(Sel, 0, FMath::Max(0, Tiles.Num() - 1));
}

bool AFTGameMode::PPActivate(int32 Idx, bool bHoldDone)
{
	UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(this);
	if (!CS || !Tiles.IsValidIndex(Idx)) return false;
	if (CS->bSaving) { BuyHoldT = 0.f; return false; }
	const FFTTileView T = Tiles[Idx];
	const auto Failed = [&](const TCHAR* Fallback)
	{
		PPSay(CS->bSaveError && !CS->SaveError.IsEmpty() ? CS->SaveError : FString(Fallback), true);
	};
	if (T.State == EFTTile::Soon) { PPSay(FString::Printf(TEXT("COMING SOON: %s"), *T.Needs), true); return false; }
	if (Tab == EFTTab::Skills)
	{
		if (T.State == EFTTile::Pending) { PPSay(TEXT("PENDING UNTIL READY · BACKSPACE / B UNDOES IT"), false); return false; }
		if (T.State == EFTTile::Owned) { PPSay(TEXT("OWNED"), false); return false; }
		if (T.State != EFTTile::Available) { PPSay(FString::Printf(TEXT("NEEDS: %s"), *T.Needs), true); return false; }
		const bool bOk = CS->BuySkill(FName(*T.Id));
		if (bOk) { CS->C.bTipSeen = true; PPSay(FString::Printf(TEXT("%s PENDING · READY TO KEEP IT"), *T.Title), false); }
		else Failed(TEXT("SKILL NOT ADDED"));
		return bOk;
	}
	// cosmetics: owned -> equip (free, instant); an equipped optional slot comes off
	if ((Tab == EFTTab::Custom || Tab == EFTTab::Sfx) && (T.State == EFTTile::Owned || T.State == EFTTile::Equipped))
	{
		const bool bFixed = T.Slot == TEXT("paint") || T.Slot == TEXT("rims") || T.Slot == TEXT("neon") || T.Slot == TEXT("nitro");
		if (T.State == EFTTile::Equipped)
		{
			if (bFixed) { PPSay(TEXT("EQUIPPED"), false); return false; }
			if (!CS->Equip(T.Slot, NAME_None)) { Failed(TEXT("COULD NOT TAKE OFF")); return false; }
			PPSay(TEXT("TAKEN OFF"), false); return true;
		}
		if (!CS->Equip(T.Slot, FName(*T.Id))) { Failed(TEXT("COULD NOT EQUIP")); return false; }
		PPSay(FString::Printf(TEXT("%s EQUIPPED"), *T.Title), false); return true;
	}
	if (T.Id == TEXT("sequel") && T.State == EFTTile::Owned)
	{
		if (!CS->ToggleSequel()) { Failed(TEXT("SEQUEL MODE NOT CHANGED")); return false; }
		PPSay(CS->C.bSequelOn ? TEXT("SEQUEL MODE ON") : TEXT("SEQUEL MODE OFF"), false); return true;
	}
	if (T.State == EFTTile::Owned) { PPSay(TEXT("OWNED"), false); return false; }
	if (T.State == EFTTile::Locked) { PPSay(FString::Printf(TEXT("NEEDS: %s"), *T.Needs), true); return false; }
	if (T.bCash && !bHoldDone) { PPSay(TEXT("HOLD ENTER / A (OR THE BUY BUTTON) 0.5 s TO BUY"), false); return false; }
	if (T.Cost > CS->C.Cash) { PPSay(FString::Printf(TEXT("NEEDS %s · YOU HAVE $%lld"), *T.Price, CS->C.Cash), true); return false; }
	bool bOk = false;
	if (T.Id.StartsWith(TEXT("garage:"))) { TArray<FString> P; T.Id.ParseIntoArray(P, TEXT(":")); bOk = CS->BuyGarage(FName(*P[1]), FCString::Atoi(*P[2])); }
	else if (T.Id == TEXT("film")) bOk = CS->BuyFilm();
	else if (T.Id.StartsWith(TEXT("greenlight-"))) bOk = CS->BuyGreenlight(T.Id.RightChop(11));
	else if (T.Id == TEXT("sequel")) bOk = CS->BuySequel();
	else if (T.Id == TEXT("respec")) bOk = CS->Respec();
	else bOk = CS->BuyCosmetic(FName(*T.Id), true); // Purchase and equip share one transaction.
	if (bOk) PPSay(FString::Printf(TEXT("%s · DONE"), *T.Title), false);
	else Failed(TEXT("NOT BOUGHT"));
	return bOk;
}

void AFTGameMode::TickPreProduction(float RDt)
{
	for (FFTPopup& P : Popups) P.T += RDt;
	Popups.RemoveAll([](const FFTPopup& P) { return P.T > 1.6f; });
	AFTPlayerController* PC = Cast<AFTPlayerController>(GetWorld()->GetFirstPlayerController());
	UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(this);
	BuildTiles();
	if (!PC || !CS || PhaseT < 0.3f) return;
	// Navigation may still open pause; a second shop mutation cannot overtake this save.
	if (CS->bSaving) { BuyHoldT = 0.f; return; }
	int32& Sel = TabSel[(int32)Tab];
	// tabs: Tab / Q / E, pad LB / RB, or a click
	const int32 NT = (int32)EFTTab::Count;
	int32 TabDir = 0;
	if (PC->Hit(EKeys::Tab) || PC->Hit(EKeys::E) || PC->Hit(EKeys::Gamepad_RightShoulder)) TabDir = 1;
	if (PC->Hit(EKeys::Q) || PC->Hit(EKeys::Gamepad_LeftShoulder)) TabDir = -1;
	for (int32 i = 0; i < TabRects.Num(); ++i) if (PC->Clicked(TabRects[i])) { Tab = (EFTTab)i; BuyHoldT = 0.f; BuildTiles(); UE_LOG(LogFT, Display, TEXT("FT SCREEN name=PreProduction tab=%d"), i); return; }
	if (TabDir != 0) { Tab = (EFTTab)((((int32)Tab + TabDir) % NT + NT) % NT); BuyHoldT = 0.f; BuildTiles(); UE_LOG(LogFT, Display, TEXT("FT SCREEN name=PreProduction tab=%d"), (int32)Tab); return; }
	// move between tiles (grid: column / row)
	if (Tiles.Num())
	{
		const FFTTileView& Cur = Tiles[FMath::Clamp(Sel, 0, Tiles.Num() - 1)];
		auto Find = [&](int32 Dc, int32 Dr)
		{
			int32 Best = -1; float BestScore = 1e9f;
			for (int32 i = 0; i < Tiles.Num(); ++i)
			{
				const int32 dc = Tiles[i].Col - Cur.Col, dr = Tiles[i].Row - Cur.Row;
				if ((Dc != 0 && dc * Dc <= 0) || (Dr != 0 && dr * Dr <= 0)) continue;
				const float Sc = Dc != 0 ? FMath::Abs(dc) + FMath::Abs(dr) * 3.f : FMath::Abs(dr) + FMath::Abs(dc) * 3.f;
				if (Sc < BestScore) { BestScore = Sc; Best = i; }
			}
			if (Best >= 0) { Sel = Best; BuyHoldT = 0.f; }
		};
		const bool bL = PC->Hit(EKeys::Left) || PC->Hit(EKeys::A) || PC->Hit(EKeys::Gamepad_DPad_Left) || PC->Hit(EKeys::Gamepad_LeftStick_Left);
		const bool bR = PC->Hit(EKeys::Right) || PC->Hit(EKeys::D) || PC->Hit(EKeys::Gamepad_DPad_Right) || PC->Hit(EKeys::Gamepad_LeftStick_Right);
		if (bL) Find(-1, 0); if (bR) Find(1, 0);
		if (PC->MenuUp()) Find(0, -1); if (PC->MenuDown()) Find(0, 1);
		const FVector2D M = PC->MousePos();
		if (!PC->UsesGamepadPrompts() && M != PPMouse) { PPMouse = M; for (int32 i = 0; i < TileRects.Num() && i < Tiles.Num(); ++i) if (PC->MouseIn(TileRects[i])) { if (Sel != i) BuyHoldT = 0.f; Sel = i; } }
		for (int32 i = 0; i < TileRects.Num() && i < Tiles.Num(); ++i)
		{
			if (!PC->Clicked(TileRects[i])) continue;
			if (Sel != i) BuyHoldT = 0.f;
			Sel = i;
			if (!Tiles[i].bCash || Tiles[i].State == EFTTile::Owned || Tiles[i].State == EFTTile::Equipped)
			{
				PPActivate(i, false);
				return;
			}
			break;
		}
	}
	// READY: Space, pad Y, or the button. Start / Escape always open the system pause.
	// (3.9: where the button is, once, and what pressed it, so a real-input test can click it and see it register)
	if (ReadyRect.bIsValid && !bReadyRectLogged) { bReadyRectLogged = true; UE_LOG(LogFT, Display, TEXT("FT SCREEN name=PreProduction ready=%.0f,%.0f,%.0f,%.0f cursor=%d"), ReadyRect.Min.X, ReadyRect.Min.Y, ReadyRect.Max.X, ReadyRect.Max.Y, PC->bMenuMouse ? 1 : 0); }
	if (PC->Hit(EKeys::SpaceBar) || PC->Hit(EKeys::Gamepad_FaceButton_Top) || PC->Clicked(ReadyRect))
	{
		UE_LOG(LogFT, Display, TEXT("FT INPUT screen=PreProduction ready by=%s"), PC->Clicked(ReadyRect) ? TEXT("click") : PC->Hit(EKeys::SpaceBar) ? TEXT("Space") : TEXT("padY"));
		LeavePreProduction(); return;
	}
	// UNDO: Backspace / pad B / the button
	if (PC->Hit(EKeys::BackSpace) || PC->Hit(EKeys::Delete) || PC->Hit(EKeys::Gamepad_FaceButton_Right) || PC->Clicked(UndoRect))
	{
		const FName Id = (Tab == EFTTab::Skills && Tiles.IsValidIndex(Sel) && Tiles[Sel].State == EFTTile::Pending) ? FName(*Tiles[Sel].Id) : NAME_None;
		if (CS->UndoSkill(Id)) PPSay(TEXT("UNDONE · SKILL POINTS BACK"), false);
		else PPSay(CS->bSaveError && !CS->SaveError.IsEmpty() ? CS->SaveError : FString(TEXT("NOTHING PENDING TO UNDO")), true);
		return;
	}
	// BUY: Enter / pad A (cash: hold 0.5 s), or hold the mouse on the BUY button
	if (Tiles.IsValidIndex(Sel))
	{
		const bool bCashTile = Tiles[Sel].bCash && Tiles[Sel].State == EFTTile::Available;
		const bool bDownKey = !PC->MenuInputBlocked() && (PC->IsInputKeyDown(EKeys::Enter) || PC->IsInputKeyDown(EKeys::Gamepad_FaceButton_Bottom));
		const bool bMouseHeld = !PC->MenuInputBlocked() && PC->IsInputKeyDown(EKeys::LeftMouseButton);
		const bool bDownMouse = bMouseHeld && PC->MouseIn(BuyRect);
		const bool bPress = PC->Hit(EKeys::Enter) || PC->Hit(EKeys::Gamepad_FaceButton_Bottom) || PC->Clicked(BuyRect);
		if (!bDownKey && !bMouseHeld) bBuyHoldDone = false;
		if (bBuyHoldDone) return;
		if (bCashTile)
		{
			if (bDownKey || bDownMouse)
			{
				BuyHoldT += RDt;
				if (!bBuyHoldDone && BuyHoldT >= FFTProgressData::Get().T(TEXT("HoldToBuy"), 0.5f)) { bBuyHoldDone = true; PPActivate(Sel, true); }
			}
			else BuyHoldT = 0.f;
		}
		else { BuyHoldT = 0.f; if (bPress) PPActivate(Sel, false); }
	}
}

// ============================================================================================== the start of a level (harness and flow)
void AFTGameMode::StartLevelAt(int32 L, int32 SceneIdx, bool bSequel)
{
	if (bSystemPaused) ResumeSystemPause();
	UFTGameInstance* G = GI();
	L = FMath::Clamp(L, 0, FTData::NumLevels() - 1);
	if (G) { G->PendingScene = FMath::Max(0, SceneIdx); G->bHarnessSequel = bSequel; }
	if (AFTPlayerController* PC = Cast<AFTPlayerController>(GetWorld()->GetFirstPlayerController())) PC->SetMenuMouse(false);
	Overlay = EFTOverlay::None;
	StopAttract();
	UGameplayStatics::SetGamePaused(this, false);
	if (L != Level || (Track && Track->IsLight())) GoToLevel(L, true);
	else StartRace(false);
}

// ============================================================================================== cosmetics in the race (07): looks only
void AFTGameMode::ApplyCosmetics(float RDt)
{
	const UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(this);
	if (!CS || !Car) return;
	const FFTProgressData& D = FFTProgressData::Get();
	CosT += RDt;
	const UFTGameInstance* G = GI();
	const bool bReduced = G && G->bReducedMotion;
	// paint and rims: a recolour of the car's paint and rim materials (the 3D car has its own material slots, so
	// the tail lights keep their red with no mask); all damage stages keep the paint
	static FName LastPaint, LastRims; static TWeakObjectPtr<AFTPlayerCar> LastCar;
	const FName Paint = CS->Worn(TEXT("paint")), Rims = CS->Worn(TEXT("rims"));
	const bool bChameleon = Paint == TEXT("p-chameleon");
	if (LastCar.Get() != Car || Paint != LastPaint || Rims != LastRims || bChameleon)
	{
		LastCar = Car; LastPaint = Paint; LastRims = Rims;
		const FFTCosmeticRow* P = D.Cosmetic(Paint);
		const FFTCosmeticRow* Rm = D.Cosmetic(Rims);
		FLinearColor PC = FLinearColor(FColor::FromHex(TEXT("ff2d3c"))), PCd = FLinearColor(FColor::FromHex(TEXT("c81a35")));
		float Metal = -1.f, Rough = -1.f;
		if (P && P->Tier > 0)
		{
			FLinearColor Sw = P->bSwatch ? P->Swatch : FLinearColor(0.95f, 0.95f, 0.97f);
			if (Paint == TEXT("p-sunburst-fade")) Sw = HexS(TEXT("ff7a1a"));
			if (bChameleon)
			{
				// cyan to violet as the car turns (follows the frame angle)
				const float A = 0.5f + 0.5f * FMath::Sin(FMath::DegreesToRadians(Car->GetActorRotation().Yaw * 4.f) + CosT * 0.6f);
				Sw = FMath::Lerp(FLinearColor(0.176f, 0.886f, 0.902f), FLinearColor(0.541f, 0.169f, 0.886f), A);
			}
			PC = Lin(Sw); PCd = PC * 0.7f;
			if (P->Group.Contains(TEXT("Metallic"))) { Metal = Paint == TEXT("p-matte-stealth") ? 0.f : 1.f; Rough = Paint == TEXT("p-matte-stealth") ? 0.95f : (Paint == TEXT("p-pearl-white") ? 0.3f : 0.22f); }
		}
		FLinearColor RC = FLinearColor(FColor::FromHex(TEXT("ffc93c"))); float RMetal = -1.f, RRough = -1.f;
		if (Rm && Rm->Tier > 0)
		{
			RC = Rm->bSwatch ? Lin(Rm->Swatch) : FLinearColor(0.8f, 0.85f, 0.9f);
			if (Rims == TEXT("r-chrome-five-hole")) { RMetal = 1.f; RRough = 0.12f; }
			if (Rims == TEXT("r-matte-black-five-hole")) { RMetal = 0.f; RRough = 0.9f; }
		}
		auto Recolour = [&](UStaticMeshComponent* M, const TCHAR* Hex, const FLinearColor& Col, float Mt, float Rg)
		{
			if (!M) return;
			for (int32 i = 0; i < M->GetNumMaterials(); ++i)
			{
				UMaterialInterface* Mat = M->GetMaterial(i);
				if (!Mat) continue;
				UMaterialInstanceDynamic* Dyn = Cast<UMaterialInstanceDynamic>(Mat);
				const FString Base = Dyn && Dyn->Parent ? Dyn->Parent->GetName() : Mat->GetName();
				if (!Base.Contains(Hex, ESearchCase::IgnoreCase) || !Base.Contains(TEXT("_g0_"))) continue;
				// reuse the instance the damage look already made (its soot stays), else make one
				UMaterialInstanceDynamic* MID = Dyn ? Dyn : M->CreateDynamicMaterialInstance(i, Mat);
				if (!MID) continue;
				MID->SetVectorParameterValue(TEXT("Color"), Col);
				if (Mt >= 0.f) MID->SetScalarParameterValue(TEXT("Metal"), Mt);
				if (Rg >= 0.f) MID->SetScalarParameterValue(TEXT("Rough"), Rg);
			}
		};
		TArray<UStaticMeshComponent*> Painted = { Car->Body.Get() };
		for (UStaticMeshComponent* Pt : Car->Parts) Painted.Add(Pt);
		for (UStaticMeshComponent* M : Painted)
		{
			Recolour(M, TEXT("ff2d3c"), Paint == TEXT("p-sunburst-fade") && M != Car->Body ? Lin(HexS(TEXT("ff2e88"))) : PC, Metal, Rough);
			Recolour(M, TEXT("c81a35"), PCd, Metal, Rough);
		}
		for (UStaticMeshComponent* W : Car->WheelMeshes) Recolour(W, TEXT("ffc93c"), RC, RMetal, RRough);
		if (!bChameleon) { UE_LOG(LogFT, Display, TEXT("FT SHOP look paint=%s rims=%s"), *Paint.ToString(), *Rims.ToString()); }
	}
	// neon underglow
	const FName Neon = CS->Worn(TEXT("neon"));
	if (Car->Underglow && Car->Underglow->IsVisible())
	{
		FLinearColor Col = Def().Glow; float Amp = 1.f;
		const FFTCosmeticRow* N = D.Cosmetic(Neon);
		if (N && N->Tier > 0 && N->bSwatch) Col = Lin(N->Swatch);
		if (Neon == TEXT("s-pulse")) Amp = 0.55f + 0.45f * FMath::Sin(CosT * 2.f * PI * (bTurboOn ? 2.f : 1.f));
		else if (Neon == TEXT("s-police-strobe")) Col = FMath::Fmod(CosT, 0.5f) < 0.25f ? FLinearColor(1.f, 0.05f, 0.05f) : FLinearColor(0.05f, 0.2f, 1.f);
		else if (Neon == TEXT("s-colour-cycle"))
		{
			TArray<FLinearColor> Owned;
			for (const FFTCosmeticRow& C : D.Cosmetics) if (C.Slot == TEXT("neon") && C.bSwatch && CS->Owns(C.Id)) Owned.Add(Lin(C.Swatch));
			if (Owned.Num())
			{
				const float U = CosT / 1.5f; const int32 A = FMath::FloorToInt(U) % Owned.Num();
				Col = FMath::Lerp(Owned[A], Owned[(A + 1) % Owned.Num()], FMath::Frac(U));
			}
		}
		else if (Neon == TEXT("s-carnage-meter"))
		{
			const float W = FMath::Clamp((float)Run.Wrecked / FMath::Max(1, ScriptTotal) / 0.95f, 0.f, 1.f);
			const FLinearColor Cy = Lin(HexS(TEXT("2de2e6"))), Pk = Lin(HexS(TEXT("ff4fb8"))), Gd = Lin(HexS(TEXT("ffc93c")));
			Col = W < 0.5f ? FMath::Lerp(Cy, Pk, W * 2.f) : FMath::Lerp(Pk, Gd, (W - 0.5f) * 2.f);
		}
		Car->Underglow->SetLightColor(Col);
		Car->Underglow->SetIntensity(60.f * Amp);
	}
	// nitro colour (turbo only; a drift boost keeps the drift-charge colours) and the flame length
	auto Swap = [](UNiagaraComponent* C, const FString& Path)
	{
		UNiagaraSystem* Want = Load<UNiagaraSystem>(Path);
		if (!C || !Want || C->GetAsset() == Want) return;
		const bool bOn = C->IsActive();
		C->SetAsset(Want);
		if (bOn) C->Activate(true);
	};
	if (Car->TurboFX)
	{
		// the nitro colour is a recoloured copy of the stock flame (ft_v26); a drift boost keeps the stock flame
		const FName Ni = CS->Worn(TEXT("nitro"));
		const FFTCosmeticRow* N = D.Cosmetic(Ni);
		const bool bNitro = (bTurboOn || bOutrun) && N && N->Tier > 0;
		Swap(Car->TurboFX, bNitro ? TEXT("/Game/FX/NS_TurboFlame_") + Ni.ToString().Replace(TEXT("-"), TEXT("_")) : FString(TEXT("/Game/FX/NS_TurboFlame")));
		const bool bLong = CS->Worn(TEXT("exhaust")) == TEXT("s-long-flames");
		Car->TurboFX->SetRelativeScale3D(FVector(Stats.FlameLen / 10.f * (bLong ? 1.5f : 1.f), 1.f, 1.f));
	}
	// tyre smoke colour (the drift sparks keep blue, orange and pink)
	if (Car->DriftFX)
	{
		const FName Sm = CS->Worn(TEXT("smoke"));
		const FFTCosmeticRow* SmR = D.Cosmetic(Sm);
		Swap(Car->DriftFX, (SmR && SmR->bSwatch) ? TEXT("/Game/FX/NS_TyreSmoke_") + Sm.ToString().Replace(TEXT("-"), TEXT("_")) : FString(TEXT("/Game/FX/NS_TyreSmoke")));
	}
	(void)bReduced;
}

void AFTGameMode::WreckExtras(FName Fx, const FVector& At, float Size)
{
	const UFTGameInstance* G = GI();
	const bool bReduced = G && G->bReducedMotion;
	auto Burst = [this, At](const TCHAR* Asset, float Sc)
	{
		if (UNiagaraSystem* S = Load<UNiagaraSystem>(FString(TEXT("/Game/FX/")) + Asset))
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, S, At + FVector(0, 0, 120.f), FRotator::ZeroRotator, FVector(Sc), true, true, ENCPoolMethod::AutoRelease);
	};
	if (Fx == TEXT("s-ka-boom-lettering") && Size >= 1.5f)
	{
		static const TCHAR* W[5] = { TEXT("KA-BOOM!"), TEXT("BLAM!"), TEXT("KRAKKA-THOOM!"), TEXT("WHAM!"), TEXT("KA-POW!") };
		KaBoom = W[FMath::RandRange(0, 4)]; KaBoomT = RealClock;
	}
	else if (Fx == TEXT("s-confetti-pop")) { Burst(TEXT("NS_ExplosionSparks_s_confetti_pop"), Size); Burst(TEXT("NS_ExplosionSparks_s_confetti_pop"), Size * 0.8f); }
	else if (Fx == TEXT("s-gold-rush")) { Burst(TEXT("NS_ExplosionSparks_s_gold_rush"), Size * 1.2f); Burst(TEXT("NS_ExplosionSparks_s_gold_rush"), Size); }
	else if (Fx == TEXT("s-film-burn") && MultiKills >= 4 && !bReduced) { FilmBurnT = RealClock; UE_LOG(LogFT, Display, TEXT("FT SHOP fx=film-burn")); }
	else if (Fx == TEXT("s-director-s-pyro") && Combo >= 5)
	{
		// a second, harmless blast 0.4 s later (07): same stock look, no damage
		FTimerHandle H;
		GetWorldTimerManager().SetTimer(H, FTimerDelegate::CreateWeakLambda(this, [this, At, Size]()
		{
			if (UNiagaraSystem* S = Load<UNiagaraSystem>(TEXT("/Game/FX/NS_Explosion")))
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, S, At + FVector(0, 0, 80.f), FRotator::ZeroRotator, FVector(Size * 0.8f), true, true, ENCPoolMethod::AutoRelease);
		}), 0.4f, false);
	}
}

// ============================================================================================== test harness (12 §9)
namespace
{
	FString Arg(const TArray<FString>& A, int32 i, const FString& Def = FString()) { return A.IsValidIndex(i) ? A[i] : Def; }
	FString KeyVal(const TArray<FString>& A, const TCHAR* K, const FString& Def = FString())
	{
		const FString P = FString(K) + TEXT("=");
		for (const FString& S : A) if (S.StartsWith(P, ESearchCase::IgnoreCase)) return S.RightChop(P.Len());
		return Def;
	}
	EFTKind KindArg(const FString& S)
	{
		if (S.Equals(TEXT("Biker"), ESearchCase::IgnoreCase)) return EFTKind::Moto;
		if (S.Equals(TEXT("SnakeEyes"), ESearchCase::IgnoreCase) || S.Equals(TEXT("Hog"), ESearchCase::IgnoreCase)) return EFTKind::Hog;
		if (S.Equals(TEXT("Rammer"), ESearchCase::IgnoreCase)) return EFTKind::Rammer;
		if (S.Equals(TEXT("SUV"), ESearchCase::IgnoreCase)) return EFTKind::Suv;
		if (S.Equals(TEXT("Truck"), ESearchCase::IgnoreCase)) return EFTKind::Truck;
		if (S.Equals(TEXT("Chopper"), ESearchCase::IgnoreCase)) return EFTKind::Heli;
		if (S.Equals(TEXT("Boss"), ESearchCase::IgnoreCase)) return EFTKind::Boss;
		if (S.Equals(TEXT("Civilian"), ESearchCase::IgnoreCase)) return EFTKind::Civ;
		if (S.Equals(TEXT("Barrel"), ESearchCase::IgnoreCase)) return EFTKind::Barrel;
		return EFTKind::None;
	}
	int32 StuntBase(const FString& N)
	{
		static const TMap<FString, int32> M = { { TEXT("AIRBORNE KILL"), 500 }, { TEXT("TURBO RAM"), 300 }, { TEXT("ROOF LANDING"), 1000 }, { TEXT("BIKER SLAM"), 300 },
			{ TEXT("UPSIDE-DOWN KILL"), 600 }, { TEXT("DOUBLE KILL"), 300 }, { TEXT("TRIPLE KILL"), 600 }, { TEXT("MASSACRE"), 900 }, { TEXT("POWER DRIFT"), 300 },
			{ TEXT("ULTRA DRIFT"), 600 }, { TEXT("THREAD THE NEEDLE"), 400 }, { TEXT("WALL SMASH"), 500 }, { TEXT("SIDESWIPE"), 400 }, { TEXT("GRAVITY CHECK"), 600 },
			{ TEXT("GRAVITY CHECK x2"), 1200 }, { TEXT("BIG AIR"), 700 }, { TEXT("TURBO AIR"), 1200 }, { TEXT("LOOP THE LOOP"), 700 }, { TEXT("TURBO LOOP"), 1200 },
			{ TEXT("PIPE RUN"), 1000 }, { TEXT("CORKSCREW"), 800 }, { TEXT("WALL RIDE"), 600 }, { TEXT("RAMP JUMP"), 400 }, { TEXT("TURBO JUMP"), 700 }, { TEXT("RUN DOWN"), 1500 } };
		const int32* V = M.Find(N);
		return V ? *V : 300;
	}
}

void AFTGameMode::HarnessCmd(const FString& Cmd, const TArray<FString>& A)
{
	UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(this);
	UFTGameInstance* G = GI();
	AFTPlayerController* PC = Cast<AFTPlayerController>(GetWorld()->GetFirstPlayerController());
	const FFTProgressData& D = FFTProgressData::Get();
	UE_LOG(LogFT, Display, TEXT("FT HARNESS %s %s"), *Cmd, *FString::Join(A, TEXT(" ")));
	auto SpawnAt = [&](EFTKind K, const FString& Beside, bool bAddFlag) -> FFTEnt*
	{
		float Ahead = 1800.f, X = LaneX(FMath::RandRange(0, 3));
		FFTEnt* Ref = nullptr;
		if (!Beside.IsEmpty())
		{
			const EFTKind BK = KindArg(Beside);
			for (int32 i = PendingEnts.Num() - 1; i >= 0 && !Ref; --i) if (!PendingEnts[i].bDead && PendingEnts[i].Kind == BK) Ref = &PendingEnts[i];
			for (int32 i = Ents.Num() - 1; i >= 0 && !Ref; --i) if (!Ents[i].bDead && Ents[i].Kind == BK) Ref = &Ents[i];
		}
		const FFTEnt RefCopy = Ref ? *Ref : FFTEnt();
		bSpawningAdds = bAddFlag;
		FFTEnt& E = Spawn(K, Ahead, X);
		bSpawningAdds = false;
		E.bCarnage = !bAddFlag && IsSyn(K);
		E.State = 1; E.Sp = Car->Speed;
		switch (K)
		{
		case EFTKind::Moto: E.Ta = R(170, 300) * DU; E.T1 = 1.5f; break;
		case EFTKind::Rammer: E.Ta = R(120, 190) * DU; E.T1 = 2.5f; break;
		case EFTKind::Suv: E.Ta = R(90, 160) * DU; E.T1 = 2.3f; E.Tx = 0.f; break;
		case EFTKind::Truck: E.Ta = 110.f * DU * 1.4f; E.T1 = 3.f; E.T2 = 2.1f; E.Tx = X; break;
		case EFTKind::Heli: E.H = 1000.f; E.Ta = 215.f * DU * 1.3f; E.T1 = 1.3f; E.Side = 1; break;
		case EFTKind::Hog: E.Ta = 0.f; E.Side = 1; E.T2 = 2.f; break;
		case EFTKind::Civ: E.Sp = Car->Speed; break;
		case EFTKind::Barrel: E.Sp = Car->Speed; break;
		default: break;
		}
		if (K != EFTKind::Civ && K != EFTKind::Barrel && K != EFTKind::Heli) Ahead = E.Ta;
		E.D = Car->D + (K == EFTKind::Hog ? 0.f : Ahead); E.PrevAhead = E.D - Car->D; E.Tx = E.X;
		if (Ref)
		{
			// beside: next to that car, inside the smallest HE blast reach (R = 24 px)
			const float Side = RefCopy.X > 0.f ? -1.f : 1.f;
			E.D = RefCopy.D; E.X = RefCopy.X + Side * (RefCopy.W / 2 + E.W / 2 + 0.f); E.Tx = E.X; E.Ta = RefCopy.Ta; E.Sp = RefCopy.Sp;
			E.PrevAhead = E.D - Car->D;
		}
		if (AFTEntityActor* Pa = E.Proxy.Get()) Pa->SetActorTransform(EntTransform(E));
		UE_LOG(LogFT, Display, TEXT("FT SPAWN kind=%s ahead=%.0f x=%.0f carnage=%d add=%d hp=%.0f"), *KindName(K), E.D - Car->D, E.X, E.bCarnage ? 1 : 0, E.bAdd ? 1 : 0, E.Hp);
		return &E;
	};
	auto Live = [&](EFTKind K) { TArray<FFTEnt*> L; for (FFTEnt& E : Ents) if (!E.bDead && (K == EFTKind::None ? IsSyn(E.Kind) : E.Kind == K)) L.Add(&E); for (FFTEnt& E : PendingEnts) if (!E.bDead && (K == EFTKind::None ? IsSyn(E.Kind) : E.Kind == K)) L.Add(&E); L.Sort([this](const FFTEnt& X, const FFTEnt& Y) { return X.D < Y.D; }); return L; };
	auto Flush = [&]() { Ents.Append(MoveTemp(PendingEnts)); PendingEnts.Reset(); };

	if (Cmd == TEXT("FTUseTestSlot")) { if (CS) CS->UseTestSlot(); }
	else if (Cmd == TEXT("FTCareerReset")) { if (CS) CS->Reset(); }
	else if (Cmd == TEXT("FTDropCareer")) { if (CS) CS->Drop(); }
	else if (Cmd == TEXT("FTLoadCareer")) { if (CS) CS->Load(); if (CS && CS->C.PendingLevel != -1) EnterPreProduction(CS->C.PendingLevel, CS->C.bPendingFromTitle); }
	else if (Cmd == TEXT("FTSaveCareer")) { if (CS) CS->Save(TEXT("Harness")); }
	else if (Cmd == TEXT("FTDumpCareer")) { if (CS) UE_LOG(LogFT, Display, TEXT("FT CAREER dump %s"), *CS->Dump()); }
	else if (Cmd == TEXT("FTSetXP")) { if (CS) CS->SetXP(FCString::Atoi64(*Arg(A, 0, TEXT("0")))); }
	else if (Cmd == TEXT("FTGiveXP")) { if (CS) { CS->GiveXP(FCString::Atoi64(*Arg(A, 0, TEXT("0"))), TEXT("Harness")); CS->Save(TEXT("Harness")); } }
	else if (Cmd == TEXT("FTSetCash")) { if (CS) { CS->C.Cash = FCString::Atoi64(*Arg(A, 0, TEXT("0"))); UE_LOG(LogFT, Display, TEXT("FT CAREER cash=%lld"), CS->C.Cash); } }
	else if (Cmd == TEXT("FTSetSP")) { if (CS) { CS->C.SP = FCString::Atoi(*Arg(A, 0, TEXT("0"))); UE_LOG(LogFT, Display, TEXT("FT SP left=%d spent=%d"), CS->SPLeft(), CS->C.SPSpent); } }
	else if (Cmd == TEXT("FTSetStat") && CS)
	{
		const FString F = Arg(A, 0); const int32 V = FCString::Atoi(*Arg(A, 1, TEXT("0")));
		FFTCareerStats& S = CS->C.Stats;
		if (F == TEXT("wrecks")) S.Wrecks = V; else if (F == TEXT("trucks")) S.Trucks = V; else if (F == TEXT("leftBehind")) S.LeftBehind = V; else if (F == TEXT("snakeEyes")) S.SnakeEyes = V; else if (F == TEXT("wraps")) S.Wraps = V;
		UE_LOG(LogFT, Display, TEXT("FT CAREER stats.%s=%d"), *F, V);
	}
	else if (Cmd == TEXT("FTAddFeat") && CS) { CS->C.Feats.AddUnique(Arg(A, 0).Replace(TEXT("_"), TEXT(" "))); }
	else if (Cmd == TEXT("FTSetBest") && G) { const int32 L = FCString::Atoi(*Arg(A, 0, TEXT("1"))) - 1; if (L >= 0 && L < UFTGameInstance::MaxLevels) { G->BestScore[L] = FCString::Atoi(*Arg(A, 1, TEXT("0"))); G->SaveSettings(); UE_LOG(LogFT, Display, TEXT("FT CAREER best%d=%d"), L + 1, G->BestScore[L]); } }
	else if (Cmd == TEXT("FTSetRaw") && CS) CS->SetRaw(Arg(A, 0), Arg(A, 1));
	else if (Cmd == TEXT("FTGarage") && CS) { CS->C.Garage.Add(FName(*Arg(A, 0)), FMath::Clamp(FCString::Atoi(*Arg(A, 1, TEXT("1"))), 0, 3)); CS->Save(TEXT("Harness")); UE_LOG(LogFT, Display, TEXT("FT GARAGE key=%s rank=%s harness=1"), *Arg(A, 0), *Arg(A, 1)); }
	else if (Cmd == TEXT("FTOwnSkill") && CS) { CS->C.Skills.AddUnique(FName(*Arg(A, 0))); if (const FFTSkillNodeRow* N = D.Node(FName(*Arg(A, 0)))) CS->C.SPSpent += N->SP; UE_LOG(LogFT, Display, TEXT("FT SKILL id=%s state=OWNED harness=1"), *Arg(A, 0)); CS->Save(TEXT("Harness")); }
	else if (Cmd == TEXT("FTGreenlight") && CS) { CS->C.Greenlit.AddUnique(Arg(A, 0)); CS->Save(TEXT("Harness")); }
	else if (Cmd == TEXT("FTSequel") && CS) { CS->C.bSequel = true; CS->C.bSequelOn = true; CS->Save(TEXT("Harness")); }
	else if (Cmd == TEXT("FTEquip") && CS)
	{
		const FName Slot(*Arg(A, 0)); FName Id(*Arg(A, 1, TEXT("none")));
		if (Id != TEXT("none") && !CS->Owns(Id)) CS->C.Own.AddUnique(Id);
		CS->Equip(Slot, Id == TEXT("none") ? NAME_None : Id);
	}
	else if (Cmd == TEXT("FTGoto"))
	{
		const FString W = Arg(A, 0);
		if (W == TEXT("Title")) { StopAttract(); ClearEnts(); UE_LOG(LogFT, Display, TEXT("FT SCREEN name=Title")); EnterTitle(false); }
		else if (W == TEXT("PreProduction")) EnterPreProduction(Level, true);
		else if (W == TEXT("Paycheck")) EnterPaycheck();
	}
	else if (Cmd == TEXT("FTSelect"))
	{
		const FString Id = Arg(A, 0);
		for (int32 t = 0; t < (int32)EFTTab::Count; ++t)
		{
			Tab = (EFTTab)t; BuildTiles();
			const int32 i = Tiles.IndexOfByPredicate([&Id](const FFTTileView& T) { return T.Id == Id; });
			if (i >= 0) { TabSel[t] = i; UE_LOG(LogFT, Display, TEXT("FT SCREEN name=PreProduction tab=%d select=%s state=%d"), t, *Id, (int32)Tiles[i].State); break; }
		}
	}
	else if (Cmd == TEXT("FTPress"))
	{
		const FString W = Arg(A, 0); const float Secs = FCString::Atof(*Arg(A, 1, TEXT("0")));
		if (W == TEXT("Ammo")) CycleAmmo();
		else if (W == TEXT("Ready") && Phase == EFTPhase::PreProduction) LeavePreProduction();
		else if (W == TEXT("Undo") && CS) CS->UndoSkill(NAME_None);
		else if (W == TEXT("Buy") && Phase == EFTPhase::PreProduction) { BuildTiles(); PPActivate(TabSel[(int32)Tab], Secs >= FFTProgressData::Get().T(TEXT("HoldToBuy"), 0.5f)); }
		else if (W == TEXT("Continue"))
		{
			if (Phase == EFTPhase::Reel) { if (bReelDone) EndReel(); else UE_LOG(LogFT, Display, TEXT("FT INPUT screen=Reel key=Continue result=ignored")); }
			else if (Phase == EFTPhase::Rating) EnterPaycheck();
			else if (Phase == EFTPhase::Paycheck) EnterPreProduction(PayNext == 2 ? -2 : (PayNext == 1 ? Level : Level + 1), false);
		}
	}
	else if (Cmd == TEXT("FTStartLevel")) StartLevelAt(FCString::Atoi(*Arg(A, 0, TEXT("1"))) - 1, FCString::Atoi(*Arg(A, 1, TEXT("1"))) - 1, Arg(A, 2) == TEXT("sequel"));
	else if (Cmd == TEXT("FTNextScene") && Phase == EFTPhase::Play && Scene + 1 < NumScenes())
	{
		ClearEnts();
		SpawnScene = Scene + 1; SpawnIdx = 0;
		Car->PlaceOnTrack(Def().Scenes[Scene + 1].D * DU + 50.f, Car->X, Car->Speed);
	}
	else if (Cmd == TEXT("FTBossNow") && Phase == EFTPhase::Play)
	{
		ClearEnts();
		const int32 Last = NumScenes() - 1;
		SpawnScene = Last; SpawnIdx = Def().Scenes[Last].Spawns.Num();
		Car->PlaceOnTrack(Def().FinaleD * DU + 100.f, Car->X, Car->Speed);
		SpawnGroup({ EFTKind::Boss });
		// 2.8: the yeti keeps his real entrance (chasing in from behind); the others start on their spot
		if (PendingEnts.Num() && Level != 5) { FFTEnt& B = PendingEnts.Last(); B.D = Car->D + B.Ta; B.State = 0; B.Heading = 0.f; }
		Flush();
	}
	else if (Cmd == TEXT("FTSpawn") || Cmd == TEXT("FTSpawnAdd") || Cmd == TEXT("FTSpawnCivilian") || Cmd == TEXT("FTSpawnBarrel"))
	{
		const EFTKind K = Cmd == TEXT("FTSpawnCivilian") ? EFTKind::Civ : (Cmd == TEXT("FTSpawnBarrel") ? EFTKind::Barrel : KindArg(Arg(A, 0)));
		int32 N = 1; FString Beside;
		for (const FString& S : A) { if (S.StartsWith(TEXT("beside="))) Beside = S.RightChop(7); else if (S.IsNumeric()) N = FCString::Atoi(*S); }
		for (int32 i = 0; i < N; ++i) SpawnAt(K, Beside, Cmd == TEXT("FTSpawnAdd"));
		Flush();
	}
	else if (Cmd == TEXT("FTWreck"))
	{
		const FString W = Arg(A, 0);
		int32 N = 1; int32 Cause = 0;
		for (int32 i = 1; i < A.Num(); ++i) { if (A[i].IsNumeric()) N = FCString::Atoi(*A[i]); else if (A[i] == TEXT("missile")) Cause = 1; else if (A[i] == TEXT("ram")) Cause = 2; }
		Flush();
		TArray<FFTEnt*> L = Live(W == TEXT("all") ? EFTKind::None : KindArg(W));
		for (int32 i = 0; i < L.Num() && (W == TEXT("all") || i < N); ++i) { FFTEnt* E = L[i]; int32 Guard = 0; while (!E->bDead && Guard++ < 400) HurtEnt(*E, FMath::Max(1.f, E->Hp), Cause); }
	}
	else if (Cmd == TEXT("FTFire"))
	{
		const int32 N = FCString::Atoi(*Arg(A, 0, TEXT("1")));
		const EFTKind K = KindArg(Arg(A, 1, TEXT("Biker")));
		Flush();
		for (int32 r = 0; r < N; ++r)
		{
			TArray<FFTEnt*> L = Live(K);
			if (!L.Num()) { UE_LOG(LogFT, Display, TEXT("FT FIRE miss=1 kind=%s"), *KindName(K)); break; }
			// one round of the current load through the gun's hit path, along its line (Sabot rounds keep going)
			int32 Ld = AmmoLoad;
			if (Ld > 0) { int32& Rr = Run.Rounds[Ld - 1]; if (Rr <= 0) Ld = 0; else if (--Rr == 0) { AmmoLoad = 0; Sfx(TEXT("Empty")); SayText(TEXT("You're out of the good stuff, Kane!"), 3); UE_LOG(LogFT, Display, TEXT("FT DIRECTOR line=\"You're out of the good stuff, Kane!\"")); UE_LOG(LogFT, Display, TEXT("FT AMMO load=STD rounds=0 reason=dry")); } }
			FFTEnt B; B.Kind = EFTKind::Bullet; B.Owner = 0; B.Load = Ld; B.X = L[0]->X; B.D = Car->D;
			FFTEnt* Tgt = L[0];
			int32 Guard = 0;
			while (Tgt && !B.bDead && Guard++ < 8)
			{
				BulletHit(B, *Tgt);
				if (B.bDead) break;
				// what is next along the line
				FFTEnt* Next = nullptr; float Best = 1e9f;
				for (FFTEnt& O : Ents)
				{
					if (O.bDead || B.Hits.Contains(O.Id) || O.Kind == EFTKind::Bullet || O.Kind == EFTKind::Missile || O.Kind == EFTKind::Crate || O.Kind == EFTKind::Wrench || O.Kind == EFTKind::Henchman || O.Kind == EFTKind::Ramp) continue;
					if (FMath::Abs(O.X - B.X) < O.W / 2 + 40.f && O.D > Tgt->D - 1.f && O.D - Tgt->D < Best && &O != Tgt) { Best = O.D - Tgt->D; Next = &O; }
				}
				Tgt = Next;
			}
			if (Ld > 0) { UE_LOG(LogFT, Display, TEXT("FT AMMO load=%s rounds=%d fired=1"), Ld == 1 ? TEXT("AP") : TEXT("HE"), Run.Rounds[Ld - 1]); }
		}
	}
	else if (Cmd == TEXT("FTCombo")) { Combo = FCString::Atoi(*Arg(A, 0, TEXT("1"))); ComboT = 3.f; UE_LOG(LogFT, Display, TEXT("FT COMBO value=%d harness=1"), Combo); }
	else if (Cmd == TEXT("FTLandStunt")) { const FString N = Arg(A, 0).Replace(TEXT("_"), TEXT(" ")).Replace(TEXT(" X2"), TEXT(" x2")); Stunt(N, StuntBase(N), nullptr, 3.f); }
	else if (Cmd == TEXT("FTAddScore")) AddScore(FCString::Atoi64(*Arg(A, 0, TEXT("0"))), 2);
	else if (Cmd == TEXT("FTSlowMo")) Slowmo(FCString::Atof(*Arg(A, 0, TEXT("2"))), 0.3f);
	else if (Cmd == TEXT("FTCrash") && Phase == EFTPhase::Play) { Invuln = 0.f; Hp = 0.f; }
	else if (Cmd == TEXT("FTDamagePlayer")) { Hp = FMath::Max(0.f, Hp - FCString::Atof(*Arg(A, 0, TEXT("10")))); }
	else if (Cmd == TEXT("FTPickup"))
	{
		const EFTKind Kind = Arg(A, 0) == TEXT("Wrench") ? EFTKind::Wrench : EFTKind::Crate;
		const float Ahead = FCString::Atof(*KeyVal(A, TEXT("ahead"), TEXT("0")));
		const float X = Car->X + FCString::Atof(*KeyVal(A, TEXT("offset"), TEXT("0")));
		FFTEnt& E = Spawn(Kind, Ahead, X);
		E.Sp = Ahead > 0.f ? 0.f : Car->Speed;
		Flush();
	}
	else if (Cmd == TEXT("FTRam"))
	{
		Flush();
		TArray<FFTEnt*> L = Live(KindArg(Arg(A, 0, TEXT("Boss"))));
		if (L.Num()) { FFTEnt* E = L[0]; Invuln = 0.f; E->D = Car->D + (CarL + E->L) / 2 - 300.f; E->X = Car->X; E->Sp = Car->Speed; }
	}
	else if (Cmd == TEXT("FTHold"))
	{
		const FString W = Arg(A, 0); const bool bOn = Arg(A, 1, TEXT("1")) != TEXT("0");
		if (W == TEXT("SpeedUp")) Held.bUp = bOn; else if (W == TEXT("Turbo")) Held.bTurbo = bOn; else if (W == TEXT("Brake")) Held.bDown = bOn;
		else if (W == TEXT("Drift")) Held.bDrift = bOn; else if (W == TEXT("Fire")) Held.bFire = bOn;
	}
	else if (Cmd == TEXT("FTInvuln")) bHarnessInvuln = Arg(A, 0, TEXT("1")) != TEXT("0");
	// 2.9: same-frame A/B of the graphics presets. FTFreeze [1|0] holds the world still; FTQuality <-1..4|apple> applies a preset
	// ("apple" = AUTO on the Apple path, as on a Mac)
	else if (Cmd == TEXT("FTFreeze")) { bHarnessFreeze = Arg(A, 0, TEXT("1")) != TEXT("0"); UE_LOG(LogFT, Display, TEXT("FT FREEZE on=%d"), bHarnessFreeze ? 1 : 0); }
	else if (Cmd == TEXT("FTQuality") && G)
	{
		const FString Q = Arg(A, 0, TEXT("-1"));
		UFTGameInstance::bHarnessApple = Q == TEXT("apple");
		G->ApplyQuality(UFTGameInstance::bHarnessApple ? -1 : FCString::Atoi(*Q), false);
	}
	else if (Cmd == TEXT("FTBossEscape")) { BossClock = BossEscapeT; UE_LOG(LogFT, Display, TEXT("FT BOSS clock=%.1f harness=escape"), BossClock); }
	else if (Cmd == TEXT("FTWrapNow") && Phase == EFTPhase::Play)
	{
		Flush();
		TArray<FFTEnt*> L = Live(EFTKind::Boss);
		if (!L.Num()) { SpawnGroup({ EFTKind::Boss }); Flush(); L = Live(EFTKind::Boss); }
		if (L.Num()) { FFTEnt* B = L[0]; int32 Guard = 0; while (!B->bDead && Guard++ < 500) HurtEnt(*B, FMath::Max(1.f, B->Hp), 0); }
	}
	else if (Cmd == TEXT("FTPayExample"))
	{
		FFTCareerSnapshot C; const FFTLevelResult Rr = FTPay::Example(Arg(A, 0, TEXT("A"))[0], C);
		const FFTPaycheck P = FTPay::Compute(Rr, C, D);
		for (const FFTPayLine& L : P.Lines) UE_LOG(LogFT, Display, TEXT("FT PAY example=%s line=%s amount=%.2f mult=%.4g"), *Arg(A, 0), *L.Key, L.Amount, L.Mult);
		UE_LOG(LogFT, Display, TEXT("FT PAY example=%s total=%lld"), *Arg(A, 0), P.Total);
	}
	else if (Cmd == TEXT("FTPayCase"))
	{
		FFTLevelResult Rr; Rr.Level = FCString::Atoi(*KeyVal(A, TEXT("level"), TEXT("1"))) - 1; Rr.ScriptTotal = D.Levels[FMath::Clamp(Rr.Level, 0, D.Levels.Num() - 1)].Vehicles;
		const FString Rt = KeyVal(A, TEXT("rating"), TEXT("BMovie"));
		Rr.Rating = Rt.StartsWith(TEXT("Legend")) ? 3 : (Rt.StartsWith(TEXT("Block")) ? 2 : (Rt.StartsWith(TEXT("Action")) ? 1 : 0));
		Rr.Retakes = FCString::Atoi(*KeyVal(A, TEXT("retakes"), TEXT("0"))); Rr.LevelTime = FCString::Atof(*KeyVal(A, TEXT("time"), TEXT("0")));
		Rr.Wrecked = FCString::Atoi(*KeyVal(A, TEXT("wrecked"), TEXT("0"))); Rr.D = FCString::Atof(*KeyVal(A, TEXT("d"), TEXT("0"))); Rr.S = FCString::Atof(*KeyVal(A, TEXT("s"), TEXT("0")));
		Rr.ArmorPct = FCString::Atoi(*KeyVal(A, TEXT("armor"), TEXT("100"))); Rr.bBossWrecked = true;
		const FFTPaycheck P = FTPay::Compute(Rr, CS ? CS->Snapshot() : FFTCareerSnapshot(), D);
		for (const FFTPayLine& L : P.Lines) UE_LOG(LogFT, Display, TEXT("FT PAY case=1 line=%s amount=%.0f"), *L.Key, FMath::RoundToDouble(L.Amount / 10.0) * 10.0);
		UE_LOG(LogFT, Display, TEXT("FT PAY case=1 total=%lld"), P.Total);
	}
	else if (Cmd == TEXT("FTDumpLevel"))
	{
		FString Ty = FString::Join(Run.Types, TEXT(","));
		UE_LOG(LogFT, Display, TEXT("FT LEVEL dump level=L%d scene=%d take=%d retakes=%d xp=%d wrecked=%d total=%d leftBehind=%d score=%lld clock=%.1f types=%d [%s] ap=%d he=%d load=%d hp=%.0f/%.0f missiles=%d"),
			Level + 1, Scene + 1, Take, Retakes, Run.XP, Run.Wrecked, ScriptTotal, Run.LeftBehind, Score, LevelClock, Run.Types.Num(), *Ty, Run.Rounds[0], Run.Rounds[1], AmmoLoad, Hp, Stats.MaxArmor, Missiles);
		// 2.6.1: which track the level's music component is really playing
		const USoundWave* MW = Music ? Cast<USoundWave>(Music->Sound) : nullptr;
		UE_LOG(LogFT, Display, TEXT("FTMUSIC dump level=L%d track=%s playing=%d looping=%d vol=%.2f"),
			Level + 1, Music && Music->Sound ? *Music->Sound->GetName() : TEXT("none"), Music && Music->IsPlaying() ? 1 : 0, MW && MW->bLooping ? 1 : 0, Music ? Music->VolumeMultiplier : 0.f);
		// 3.0: the title song (kept by the game instance across the attract mode's map loads) and the attract state
		const UFTGameInstance* GT = GI();
		const UAudioComponent* TM = GT ? GT->TitleMusic.Get() : nullptr;
		const USoundWave* TW = TM ? Cast<USoundWave>(TM->Sound) : nullptr;
		UE_LOG(LogFT, Display, TEXT("FTMUSIC dump title=%s playing=%d looping=%d vol=%.2f attract=%d phase=%d titleT=%.1f cuts=%d light=%d"),
			TM && TM->Sound ? *TM->Sound->GetName() : TEXT("none"), TM && TM->IsPlaying() ? 1 : 0, TW && TW->bLooping ? 1 : 0, TM ? TM->VolumeMultiplier : 0.f,
			bAttract ? 1 : 0, (int32)Phase, TitleT, GT ? GT->AttractCuts : 0, Track && Track->IsLight() ? 1 : 0);
	}
	else if (Cmd == TEXT("FTDumpSkills") || Cmd == TEXT("FTDumpGarage") || Cmd == TEXT("FTDumpShop"))
	{
		const EFTTab Was = Tab;
		const TArray<EFTTab> Which = Cmd == TEXT("FTDumpSkills") ? TArray<EFTTab>{ EFTTab::Skills } : (Cmd == TEXT("FTDumpGarage") ? TArray<EFTTab>{ EFTTab::Garage } : TArray<EFTTab>{ EFTTab::Custom, EFTTab::Sfx });
		static const TCHAR* St[6] = { TEXT("LOCKED"), TEXT("AVAILABLE"), TEXT("PENDING"), TEXT("OWNED"), TEXT("EQUIPPED"), TEXT("SOON") };
		for (EFTTab T : Which)
		{
			Tab = T; BuildTiles();
			for (const FFTTileView& Tl : Tiles)
				UE_LOG(LogFT, Display, TEXT("FT %s id=%s state=%s needs=\"%s\" price=\"%s\""), T == EFTTab::Skills ? TEXT("SKILL") : (T == EFTTab::Garage ? TEXT("GARAGE") : TEXT("SHOP")), *Tl.Id, St[(int32)Tl.State], *Tl.Needs, *Tl.Price);
		}
		Tab = Was; BuildTiles();
	}
	else if (Cmd == TEXT("FTKey") && PC)
	{
		const FKey K(*Arg(A, 0)); const float Secs = FMath::Max(0.08f, FCString::Atof(*Arg(A, 1, TEXT("0.1"))));
		PC->InputKey(FInputKeyEventArgs::CreateSimulated(K, IE_Pressed, 1.f));
		TWeakObjectPtr<APlayerController> Weak(PC);
		FTimerHandle H;
		GetWorldTimerManager().SetTimer(H, FTimerDelegate::CreateLambda([Weak, K]() { if (Weak.IsValid()) Weak->InputKey(FInputKeyEventArgs::CreateSimulated(K, IE_Released, 0.f)); }), Secs, false);
	}
	else if (Cmd == TEXT("FTAutoRun"))
	{
		bAutoRun = true; bHarnessInvuln = true;
		const FString W = KeyVal(A, TEXT("wreck"), TEXT("0"));
		AutoLeave = FCString::Atoi(*KeyVal(A, TEXT("leaveBehind"), TEXT("0")));
		AutoWreck = W == TEXT("all") ? 100000 : (W == TEXT("rest") ? 100000 : FCString::Atoi(*W));
		const FString At = KeyVal(A, TEXT("wrapAt"));
		AutoWrapAt = -1.f;
		if (!At.IsEmpty()) { FString M, S2; if (At.Split(TEXT(":"), &M, &S2)) AutoWrapAt = FCString::Atoi(*M) * 60.f + FCString::Atof(*S2); else AutoWrapAt = FCString::Atof(*At); }
		const FString Tb = KeyVal(A, TEXT("turbo"), TEXT("off"));
		AutoTurbo = Tb == TEXT("hold") ? 2 : (Tb == TEXT("fullTanks") ? 1 : 0);
		if (G) G->bAutoDrive = true;
		Held.bUp = true;
		const FString Lv = KeyVal(A, TEXT("level"));
		if (!Lv.IsEmpty()) StartLevelAt(FCString::Atoi(*Lv) - 1, 0, false);
	}
	else { UE_LOG(LogFT, Warning, TEXT("FT HARNESS unknown %s"), *Cmd); }
}

#if !UE_BUILD_SHIPPING
namespace
{
	void RunHarness(const FString& Name, const TArray<FString>& Args, UWorld* World)
	{
		AFTGameMode* GM = World ? World->GetAuthGameMode<AFTGameMode>() : nullptr;
		if (GM) GM->HarnessCmd(Name, Args);
		else { UE_LOG(LogFT, Warning, TEXT("FT HARNESS %s: no race GameMode in this world"), *Name); }
	}
	struct FFTHarnessCommands
	{
		TArray<TUniquePtr<FAutoConsoleCommandWithWorldAndArgs>> Cmds;
		FFTHarnessCommands()
		{
			static const TCHAR* Names[] = { TEXT("FTUseTestSlot"), TEXT("FTCareerReset"), TEXT("FTDropCareer"), TEXT("FTLoadCareer"), TEXT("FTSaveCareer"), TEXT("FTDumpCareer"),
				TEXT("FTSetXP"), TEXT("FTGiveXP"), TEXT("FTSetCash"), TEXT("FTSetSP"), TEXT("FTSetStat"), TEXT("FTAddFeat"), TEXT("FTSetBest"), TEXT("FTSetRaw"),
				TEXT("FTGarage"), TEXT("FTOwnSkill"), TEXT("FTGreenlight"), TEXT("FTSequel"), TEXT("FTEquip"), TEXT("FTGoto"), TEXT("FTSelect"), TEXT("FTPress"),
				TEXT("FTStartLevel"), TEXT("FTNextScene"), TEXT("FTBossNow"), TEXT("FTSpawn"), TEXT("FTSpawnAdd"), TEXT("FTSpawnCivilian"), TEXT("FTSpawnBarrel"),
				TEXT("FTWreck"), TEXT("FTFire"), TEXT("FTCombo"), TEXT("FTLandStunt"), TEXT("FTAddScore"), TEXT("FTSlowMo"), TEXT("FTCrash"), TEXT("FTDamagePlayer"),
				TEXT("FTPickup"), TEXT("FTRam"), TEXT("FTHold"), TEXT("FTInvuln"), TEXT("FTBossEscape"), TEXT("FTWrapNow"), TEXT("FTPayExample"), TEXT("FTPayCase"),
				TEXT("FTDumpLevel"), TEXT("FTDumpSkills"), TEXT("FTDumpGarage"), TEXT("FTDumpShop"), TEXT("FTKey"), TEXT("FTAutoRun"), TEXT("FTFreeze"), TEXT("FTQuality") };
			for (const TCHAR* N : Names)
			{
				const FString Name(N);
				Cmds.Add(MakeUnique<FAutoConsoleCommandWithWorldAndArgs>(N, TEXT("FULL THROTTLE test harness (spec 12 §9)"),
					FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([Name](const TArray<FString>& Args, UWorld* World) { RunHarness(Name, Args, World); })));
			}
		}
	};
	FFTHarnessCommands GFTHarness;
	// 3.9: FTGo <spec> works from any map (the race's harness commands need the race's game mode)
	FAutoConsoleCommandWithWorldAndArgs GFTGo(TEXT("FTGo"), TEXT("FULL THROTTLE tests: FTGo L<n>[boss] | N<n>[boss] - straight into a race / night, its boss too"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
		{
			if (World && Args.Num()) UFTNightFlow::Go(World, FString::Join(Args, TEXT("")));
			else { UE_LOG(LogFT, Warning, TEXT("FT GO usage: FTGo L<n>[boss] | N<n>[boss]")); }
		}));
}
#endif
