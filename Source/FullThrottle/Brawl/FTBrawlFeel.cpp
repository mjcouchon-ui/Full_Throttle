// Stage 2 "hitfeel": what a hit feels like. Looks, timing of the freeze, and how a knock-back travels; no new
// move, no new damage number, no new sound file, no new texture or material (the sparks and rings are quads with
// the existing additive material M_BrawlGlow: a spark is a plain bright sliver, the engine's white square as its
// mask; a ring uses the existing mask T_ring; the dust is the existing accent cards).
//
// THE NUMBERS (fight steps are 1/60 s; "old" = what the fight did before this file, and still does with ftb.Feel 0)
//
//  Hit-stop (the whole fight freezes, so both fighters do; effects and the camera go on)
//    Nova hits a foe     light, damage under 9.5                    2 steps   (old 2)
//                        light, damage 9.5 or more                  3         (old 2)
//                        knock-down                                 6         (old 3)   +1 from damage 16, +1 from 24
//                        the blow that kills                        +1, at most 8        (old 3)
//                        the last foe of a wave, a boss's defeat    9         (old 3)
//    stamp on a man down (stomp)                                    6         (old 3)
//    slam of a carried foe                                          8         (old 4)
//    A foe hits Nova     light, damage under 10                     2         (old 1)
//                        light, damage 10 or more                   3         (old 1)
//                        floored                                    7         (old 3)
//                        knocked out                                9         (old 3, or 1)
//                        a knee or a squeeze while held             3         (old 0)
//    Juggle hits, pummel knees, a blocked hit and scenery keep their 2 steps.
//
//  The struck figure: white flash (Nova too, he had none), 0.12 s, 0.16 s on a heavy hit, and it now runs out DURING
//  the freeze instead of waiting for it; while the freeze holds he shakes sideways 2.5 px (light), 6 (heavy), 8 (last)
//  at 22 a second, dying away over the last 4 steps.
//
//  Sparks: 5 (light), 9 (heavy), 12 (kill), 16 (last blow) bright slivers thrown in a fan the way the blow goes (one in
//  three back toward the striker), alive 0.16 to 0.30 s, warm white for
//  fists and feet, cold white for a weapon, red for a hit on Nova, pale blue for a block. Heavy hits add a shock ring
//  that opens to 110 / 150 / 190 cm in 0.16 to 0.20 s. A body that lands raises landing dust and (a boss or a beaten
//  foe) a flat ring on the floor; a body that slides raises dust puffs.
//
//  Camera kick: the camera is knocked the way the blow goes (and up a little on a launch) by 2.2 cm (light), 3.5,
//  8 (knock-down), 10 (kill), 15 (last), 5 / 12 for hits on Nova, straight down 4 (boss 9) when a body lands, then
//  swings back as a spring at 9 swings a second that has died to a tenth in about 0.2 s. It adds to the old shake.
//
//  Knock-back: the 12 px shove of a light hit (4 px on armour) is no longer a jump: he slides it, fast at first,
//  nine tenths of the way in 0.18 s, and the distance grows with the damage (x 0.85 to x 1.7; a 7-damage jab is the
//  old 12 px). A knock-down flies further along the room the harder the hit (x 1.0 to x 1.5, a kill x 1.15 more),
//  higher only from damage 20, and after landing skids on about 5% of its speed in px. Nova's 40 px (floored) and
//  14 px (hurt) are slid the same way.
//
//  The slow beat: the blow that beats the last foe of a wave runs the fight at 0.30 speed for 0.36 real seconds
//  (0.22 speed for 0.85 s on a boss's defeat), easing back over the last third; it begins when the hit-stop ends.
//  It adds about 0.21 s (0.56 s) of real time; the fight clock ("time=" in "FTBRAWL won") does not count it.
//
//  Camera lead and rise: the view drifts up to 95 cm toward where Nova and the nearest foe are, plus 26 cm the way
//  he faces (eased at 2.2 a second), inside the limits that keep every fighter framed; it rises up to 90 cm when
//  someone is high in the air (above 110 px).
//
//  Rumble (the existing path, AFTBrawlGameMode::Rumble): 0.7 for 0.2 s on a knock-down, 1.0 for 0.45 s on the last blow.
//
// The reduced-motion setting (ftb.ReducedMotion 1 or the career setting) switches off the kick, the lead and rise,
// the struck figure's shake and the slow beat; hit-stop, sparks (fewer and dimmer) and the slides stay.
//
// Development:
//   ftb.Feel 0|1                 0 = everything in this file off: the fight as it was (for comparing times)
//   ftb.FeelTest slow [boss]     the slow beat now
//   ftb.FeelTest spark <0|1|2>   sparks, ring and kick in front of Nova: light, heavy, finishing
//   ftb.FeelTest stats           logs the "FTBRAWL feel stats" line (also logged once when the night is won)
// Log lines: "FTBRAWL feel beat ..." (a slow beat began), "FTBRAWL feel stats ...".
#include "FTBrawlGameMode.h"
#include "FTBrawlFighter.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/Texture.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

namespace FTBrawlFeel
{
	const float StepDt = 1.f / 60.f;
	const float PxCm = 1.4f;            // one brawl px in cm (PX in FTBrawlGameMode.cpp)
	const float ScreenW = 960.f;
	const int32 NumSparks = 56, NumRings = 8;
	const float KbRate = 13.f;          // a slide loses this share of its speed a second (e-folds): 90% of the way in 0.18 s
	const float KickHz = 9.f, KickDamp = 0.34f;
	TAutoConsoleVariable<int32> CVarFeel(TEXT("ftb.Feel"), 1, TEXT("Brawl hit feel (FTBrawlFeel.cpp): 0 = off, the fight as it was before it"));

#if !UE_BUILD_SHIPPING
	FAutoConsoleCommandWithWorldAndArgs GFTBrawlFeelTest(TEXT("ftb.FeelTest"), TEXT("Development: ftb.FeelTest slow [boss] | spark <0|1|2> | stats (FTBrawlFeel.cpp)"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
		{
			if (AFTBrawlGameMode* GM = World ? World->GetAuthGameMode<AFTBrawlGameMode>() : nullptr) GM->FeelCommand(Args);
		}));
#endif
}
using namespace FTBrawlFeel;

bool AFTBrawlGameMode::FeelOn() const { return CVarFeel.GetValueOnGameThread() != 0; }

void AFTBrawlGameMode::FeelCommand(const TArray<FString>& Args)
{
	const FString Cmd = Args.Num() ? Args[0].ToLower() : FString();
	if (Cmd == TEXT("slow")) { const bool bBoss = Args.Num() > 1; FeelSlow(bBoss ? 0.85f : 0.36f, bBoss ? 0.22f : 0.30f); UE_LOG(LogTemp, Display, TEXT("FTBRAWL feel beat forced boss=%d"), bBoss ? 1 : 0); }
	else if (Cmd == TEXT("spark"))
	{
		const int32 Grade = Args.Num() > 1 ? FMath::Clamp(FCString::Atoi(*Args[1]), 0, 2) : 1;
		FeelImpact(World(P.X + P.Dir * 62.f, P.Y + 11.f, P.Z + 108.f), P.Dir, Grade, FLinearColor(1.f, 0.8f, 0.42f));
		UE_LOG(LogTemp, Display, TEXT("FTBRAWL feel spark grade=%d"), Grade);
	}
	else FeelStats(TEXT("asked"));
}

void AFTBrawlGameMode::FeelStats(const TCHAR* Why)
{
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL feel stats %s on=%d light=%d heavy=%d novahits=%d novadown=%d beats=%d stop=%.2f stopadded=%.2f slowadded=%.2f fight=%.2f real=%.2f kos=%d hits=%d"),
		Why, FeelOn() ? 1 : 0, FeelLight, FeelHeavy, FeelNova, FeelNovaDown, FeelBeats, FeelStopT, FeelStopAdded, FeelSlowAdded, Time, FeelRealT, Kos, Hits);
}

// ---------------------------------------------------------------- the quads
void AFTBrawlGameMode::FeelInit()
{
	UStaticMesh* Plane = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"));
	UMaterialInterface* Glow = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlGlow.M_BrawlGlow"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UTexture* Streak = LoadObject<UTexture>(nullptr, TEXT("/Engine/EngineResources/WhiteSquareTexture.WhiteSquareTexture"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UTexture* Ring = LoadObject<UTexture>(nullptr, TEXT("/Game/Brawl/Velvet/T_ring.T_ring"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL feel init material=%d streak=%d ring=%d"), Glow ? 1 : 0, Streak ? 1 : 0, Ring ? 1 : 0);
	if (!Plane || !Glow || !GetWorld()) return;
	auto Make = [&](UTexture* Tex, int32 Sort) -> AStaticMeshActor*
	{
		AStaticMeshActor* Quad = GetWorld()->SpawnActor<AStaticMeshActor>();
		if (!Quad) return nullptr;
		Quad->SetMobility(EComponentMobility::Movable);
		Quad->SetActorHiddenInGame(true);
		UStaticMeshComponent* Mesh = Quad->GetStaticMeshComponent();
		Mesh->SetStaticMesh(Plane);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetCastShadow(false);
		Mesh->bAffectDistanceFieldLighting = false;
		Mesh->SetTranslucentSortPriority(Sort);
		UMaterialInstanceDynamic* Mid = Mesh->CreateDynamicMaterialInstance(0, Glow);
		if (Mid && Tex) Mid->SetTextureParameterValue(TEXT("Tex"), Tex);
		return Quad;
	};
	if (Streak) { FeelSparks.SetNum(NumSparks); for (int32 I = 0; I < NumSparks; ++I) FeelSparkQuads.Add(Make(Streak, 4)); }
	if (Ring) { FeelRings.SetNum(NumRings); for (int32 I = 0; I < NumRings; ++I) FeelRingQuads.Add(Make(Ring, 3)); }
}

void AFTBrawlGameMode::FeelReset(bool bKeep)
{
	for (FFeelSpark& S : FeelSparks) S.Life = 0.f;
	for (FFeelRing& R : FeelRings) R.Life = 0.f;
	for (AStaticMeshActor* Q : FeelSparkQuads) if (Q) Q->SetActorHiddenInGame(true);
	for (AStaticMeshActor* Q : FeelRingQuads) if (Q) Q->SetActorHiddenInGame(true);
	FeelKickOff = FeelKickVel = FVector2D::ZeroVector;
	FeelSlowT = FeelSlowLen = 0.f; FeelSlowTo = 1.f; FeelDustT = 0.f;
	if (!bKeep)
	{
		FeelLight = FeelHeavy = FeelNova = FeelNovaDown = FeelBeats = 0; bFeelLogged = false;
		FeelStopT = FeelStopAdded = FeelSlowAdded = FeelRealT = 0.f; FeelLead = FeelRise = 0.f;
	}
}

// ---------------------------------------------------------------- sparks, rings, the kick
void AFTBrawlGameMode::FeelSparkBurst(const FVector& At, float Dir, int32 N, float Speed, const FLinearColor& Color)
{
	if (FeelSparkQuads.IsEmpty()) return;
	const bool bCalm = ReducedPresentationMotion();
	if (bCalm) N = FMath::Max(2, N / 2);
	for (int32 I = 0; I < N; ++I)
	{
		const int32 Slot = FeelNextSpark; FeelNextSpark = (FeelNextSpark + 1) % FeelSparkQuads.Num();
		FFeelSpark& S = FeelSparks[Slot];
		// a fan the way the blow goes, tipped up a little; one streak in four goes back toward the striker
		// (looks only: the engine's dice, never the fight's seeded stream)
		const bool bBack = I % 3 == 2;
		const float Ang = FMath::DegreesToRadians((bBack ? 180.f : 0.f) + 16.f + FMath::FRandRange(-70.f, 70.f));
		const float Sp = Speed * FMath::FRandRange(0.5f, 1.f) * (bBack ? 0.6f : 1.f);
		S.V = FVector(FMath::Cos(Ang) * Dir, 0.f, FMath::Sin(Ang)) * Sp;
		S.P = At + S.V * 0.012f + FVector(0.f, I * 0.15f, 0.f);
		S.Age = 0.f; S.Life = FMath::FRandRange(0.16f, 0.30f);
		S.Len = Sp * FMath::FRandRange(0.05f, 0.085f); S.Wide = FMath::FRandRange(3.5f, 7.f);   // a plain bright sliver (the laser mask blurred to a haze at this size: seen)
		S.Glow = (bCalm ? 2.f : 3.4f) * FMath::FRandRange(0.8f, 1.2f);
		if (AStaticMeshActor* Q = FeelSparkQuads[Slot])
		{
			if (UMaterialInstanceDynamic* M = Cast<UMaterialInstanceDynamic>(Q->GetStaticMeshComponent()->GetMaterial(0)))
			{
				// most are the hit's colour, every third is white-hot
				M->SetVectorParameterValue(TEXT("Color"), I % 3 == 0 ? FLinearColor(1.f, 0.97f, 0.9f) : Color);
				M->SetScalarParameterValue(TEXT("Glow"), S.Glow);
			}
			Q->SetActorHiddenInGame(false);
		}
	}
}

void AFTBrawlGameMode::FeelRing(const FVector& At, float R0, float R1, float Life, bool bFloor, const FLinearColor& Color, float Glow)
{
	if (FeelRingQuads.IsEmpty()) return;
	const int32 Slot = FeelNextRing; FeelNextRing = (FeelNextRing + 1) % FeelRingQuads.Num();
	FFeelRing& R = FeelRings[Slot];
	R.P = At; R.Age = 0.f; R.Life = Life; R.R0 = R0; R.R1 = R1; R.bFloor = bFloor; R.Glow = Glow * (ReducedPresentationMotion() ? 0.5f : 1.f);
	if (AStaticMeshActor* Q = FeelRingQuads[Slot])
	{
		if (UMaterialInstanceDynamic* M = Cast<UMaterialInstanceDynamic>(Q->GetStaticMeshComponent()->GetMaterial(0)))
		{ M->SetVectorParameterValue(TEXT("Color"), Color); M->SetScalarParameterValue(TEXT("Glow"), R.Glow); }
		Q->SetActorHiddenInGame(false);
	}
}

void AFTBrawlGameMode::FeelKick(float DirX, float DirZ, float Cm)
{
	if (ReducedPresentationMotion()) return;
	const float Omega = 2.f * PI * KickHz;
	FeelKickVel += FVector2D(DirX, DirZ).GetSafeNormal() * Cm * Omega;
	const float Most = 22.f * Omega;   // many hits in one step do not add up to a lurch
	if (FeelKickVel.SizeSquared() > Most * Most) FeelKickVel = FeelKickVel.GetSafeNormal() * Most;
}

void AFTBrawlGameMode::FeelImpact(const FVector& At, float Dir, int32 Grade, const FLinearColor& Color)
{
	if (!FeelOn()) return;
	static const int32 Count[] = { 5, 9, 14 };
	static const float Speed[] = { 760.f, 1250.f, 1500.f };
	static const float Kick[] = { 2.2f, 8.f, 12.f };
	Grade = FMath::Clamp(Grade, 0, 2);
	FeelSparkBurst(At, Dir, Count[Grade], Speed[Grade], Color);
	if (Grade >= 1) FeelRing(At, 24.f, Grade == 2 ? 170.f : 110.f, Grade == 2 ? 0.22f : 0.16f, false, FMath::Lerp(Color, FLinearColor::White, 0.5f), 1.6f);
	FeelKick(Dir, Grade >= 1 ? 0.3f : 0.f, Kick[Grade]);
}

void AFTBrawlGameMode::FeelSlow(float RealSeconds, float Speed)
{
	if (!FeelOn() || ReducedPresentationMotion()) return;
	FeelSlowT = FeelSlowLen = RealSeconds; FeelSlowTo = Speed;
}

void AFTBrawlGameMode::FeelStop(float Steps, float OldSteps)
{
	const float Old = FMath::Max(ContactPause, OldSteps * StepDt);
	if (!FeelOn()) { ContactPause = Old; return; }
	const float New = FMath::Max(ContactPause, Steps * StepDt);
	FeelStopAdded += New - Old;
	ContactPause = New;
}

// ---------------------------------------------------------------- the contacts
// HitFoe's last line. By now the old rules have run: his health is down, he is in "fly" (knocked down or beaten),
// in "hurt" and shoved 12 px, or shoved 4 px (armour, a boss in his move). X0 is where he stood before the shove.
void AFTBrawlGameMode::FeelFoeHit(FFTBrawlBody& F, float Dmg, bool bDown, float X0)
{
	const bool bKill = F.Hp <= 0.f, bHeavy = bDown || bKill;
	if (bHeavy) FeelHeavy++; else FeelLight++;
	if (!FeelOn()) return;
	const bool bFly = F.St == TEXT("fly") && F.T == 0.f;
	// the last one standing of a wave, or the boss: the blow the fight was for
	bool bLast = bKill && F.bBoss;
	if (bKill && !bLast && bSpawned && !bBossOut)
	{
		bLast = true;
		for (const FFTBrawlBody& O : Foes) if (O.Id != F.Id && O.Hp > 0.f && O.St != TEXT("dead")) { bLast = false; break; }
	}
	float Steps = Dmg >= 9.5f ? 3.f : 2.f;
	if (bHeavy) { Steps = 6.f + (Dmg >= 16.f ? 1.f : 0.f) + (Dmg >= 24.f ? 1.f : 0.f) + (bKill ? 1.f : 0.f); Steps = FMath::Min(Steps, 8.f); if (bLast) Steps = 9.f; }
	FeelStop(Steps, bHeavy ? 3.f : 2.f);
	// the knock-back
	if (bFly)
	{
		F.Vx *= FMath::Clamp(0.85f + Dmg / 45.f, 1.f, 1.5f) * (bKill ? 1.15f : 1.f);
		if (Dmg >= 20.f) F.Vz *= FMath::Min(1.15f, 1.f + (Dmg - 20.f) / 66.f);
		F.Kb = 0.f;
		Accent(EAccent::Dust, World(F.X, F.Y + 7.f, 9.f), F.bBoss ? 70.f : 50.f, -P.Dir, false);   // his feet leave the floor
	}
	else if (F.X != X0)
	{
		const float D = (F.X - X0) * FMath::Clamp(Dmg / 7.f, 0.85f, 1.7f);
		F.X = X0;
		F.Kb += D * (1.f - FMath::Exp(-KbRate * StepDt)) / StepDt;   // the slide's steps add up to exactly D
	}
	F.Flash = bHeavy ? 0.16f : 0.12f;
	F.Rock = bLast ? 8.f : bHeavy ? 6.f : 2.5f;
	// sparks, ring, kick, rumble
	const bool bWeapon = P.St == TEXT("wpn") || P.St == TEXT("wpnair") || P.St == TEXT("wpndash");
	const FLinearColor Col = bWeapon ? FLinearColor(0.7f, 0.88f, 1.f) : FLinearColor(1.f, 0.8f, 0.42f);
	const FVector At = World(F.X - P.Dir * 12.f, F.Y + 11.f, F.Z + 105.f * F.K.Height / 175.f);
	FeelSparkBurst(At, P.Dir, bLast ? 16 : bKill ? 12 : bHeavy ? 9 : 5, bLast ? 1550.f : bHeavy ? 1250.f : 760.f, Col);
	if (bHeavy) FeelRing(At, 24.f, bLast ? 190.f : bKill ? 150.f : 110.f, bLast ? 0.2f : bKill ? 0.18f : 0.16f, false, FMath::Lerp(Col, FLinearColor::White, 0.5f), bLast ? 2.2f : bKill ? 1.8f : 1.5f);   // dim: the mask's inside is a quarter as bright as its rim, and at 4.5 to 6 it hid the fighters (seen)
	FeelKick(P.Dir, bFly ? 0.35f : 0.f, bLast ? 15.f : bKill ? 10.f : bHeavy ? 8.f : Dmg >= 9.5f ? 3.5f : 2.2f);
	if (bLast) Rumble(1.f, 0.45f, TEXT("finish"));
	else if (bHeavy) Rumble(0.7f, 0.2f, TEXT("heavy"));
	if (bLast)
	{
		FeelBeats++;
		FeelSlow(F.bBoss ? 0.85f : 0.36f, F.bBoss ? 0.22f : 0.30f);
		Sfx(TEXT("punchbig"), 1.f, 0.7f);   // the low thump under the last blow (an existing sound, pitched down)
		UE_LOG(LogTemp, Display, TEXT("FTBRAWL feel beat %s wave=%d dmg=%.0f steps=%.0f t=%.2f"), F.bBoss ? TEXT("boss") : TEXT("lastfoe"), Wave + 1, Dmg, Steps, Time);
	}
}

// HurtPlayer's last line: he is "down" and was moved back 40 px, or "hurt" and moved back 14 px. X0 = before that.
void AFTBrawlGameMode::FeelNovaHurt(float Dmg, bool bDown, float X0)
{
	const bool bFloor = P.St == TEXT("down");
	FeelNova++; if (bFloor) FeelNovaDown++;
	if (!FeelOn()) return;
	FeelStop(bFloor ? (P.Hp <= 0.f ? 9.f : 7.f) : (Dmg >= 10.f ? 3.f : 2.f), bDown ? 3.f : 1.f);
	if (P.X != X0)
	{
		const float D = P.X - X0;
		P.X = X0;
		P.Kb = D * (1.f - FMath::Exp(-KbRate * StepDt)) / StepDt;
	}
	P.Flash = bFloor ? 0.16f : 0.12f;
	P.Rock = bFloor ? 6.f : 3.f;
	const FVector At = World(P.X + P.Dir * 13.f, P.Y + 11.f, 116.f);
	const FLinearColor Col(1.f, 0.42f, 0.32f);
	FeelSparkBurst(At, -P.Dir, bFloor ? 10 : 5, bFloor ? 1250.f : 760.f, Col);
	if (bFloor)
	{
		FeelRing(At, 24.f, 130.f, 0.18f, false, FLinearColor(1.f, 0.7f, 0.62f), 1.6f);
		Accent(EAccent::Land, World(P.X, P.Y + 7.f, 10.f), 112.f, -P.Dir, false);   // he hits the floor (the novadown Sfx is the sound)
	}
	FeelKick(-P.Dir, 0.f, bFloor ? 12.f : 5.f);
}

void AFTBrawlGameMode::FeelNovaSqueeze(const FFTBrawlBody& By)
{
	FeelNova++;
	if (!FeelOn()) return;
	FeelStop(3.f, 0.f);
	P.Flash = 0.12f; P.Rock = 3.f;
	const FVector At = World(P.X + P.Dir * 10.f, P.Y + 11.f, 100.f);
	CombatCue(ECombatCue::Hurt, At, -P.Dir, false);   // the novahurt Sfx beside the call is the sound
	FeelSparkBurst(At, By.Dir, 5, 760.f, FLinearColor(1.f, 0.42f, 0.32f));
	FeelKick(By.Dir, 0.3f, 5.f);
}

// a knocked-down or thrown body meets the floor (its "down" Sfx and rumble are beside the call)
void AFTBrawlGameMode::FeelLand(FFTBrawlBody& F)
{
	if (!FeelOn()) return;
	const float Way = F.Vx >= 0.f ? 1.f : -1.f;
	Accent(EAccent::Land, World(F.X, F.Y + 7.f, 10.f), F.bBoss ? 175.f : 108.f, Way, false);
	if (F.bBoss || F.Hp <= 0.f) FeelRing(World(F.X, F.Y, 3.f), 30.f, F.bBoss ? 190.f : 120.f, 0.26f, true, FLinearColor(0.85f, 0.78f, 0.66f), F.bBoss ? 3.f : 2.f);
	FeelKick(0.f, -1.f, F.bBoss ? 9.f : 4.f);
	F.Kb = F.Vx * 0.6f;   // the skid: about 5% of his speed in px, dying away
}

// ---------------------------------------------------------------- the fight's steps
void AFTBrawlGameMode::FeelTickFx(float Dt)
{
	if (bWon && !bFeelLogged) { bFeelLogged = true; FeelStats(TEXT("won")); }
	for (int32 I = 0; I < FeelSparks.Num(); ++I)
	{
		FFeelSpark& S = FeelSparks[I];
		if (S.Life <= 0.f) continue;
		AStaticMeshActor* Q = FeelSparkQuads[I];
		S.Age += Dt;
		if (S.Age >= S.Life) { S.Life = 0.f; if (Q) Q->SetActorHiddenInGame(true); continue; }
		S.V *= FMath::Exp(-5.f * Dt); S.V.Z -= 520.f * Dt; S.P += S.V * Dt;
		if (!Q) continue;
		const float U = S.Age / S.Life;
		const FVector Along = S.V.GetSafeNormal(1e-3f, FVector(1, 0, 0));
		// the quad's X lies along the flight, its face (Z) toward the camera at +Y
		Q->SetActorLocationAndRotation(S.P, FRotationMatrix::MakeFromXZ(Along, FVector(0, 1, 0)).ToQuat());
		Q->SetActorScale3D(FVector(S.Len * (1.f - 0.55f * U) / 100.f, S.Wide * (1.f - 0.6f * U) / 100.f, 1.f));
		if (UMaterialInstanceDynamic* M = Cast<UMaterialInstanceDynamic>(Q->GetStaticMeshComponent()->GetMaterial(0)))
			M->SetScalarParameterValue(TEXT("Glow"), S.Glow * (1.f - U * U));   // bright until late: for its first steps it lies over the struck figure's white flash
	}
	for (int32 I = 0; I < FeelRings.Num(); ++I)
	{
		FFeelRing& R = FeelRings[I];
		if (R.Life <= 0.f) continue;
		AStaticMeshActor* Q = FeelRingQuads[I];
		R.Age += Dt;
		if (R.Age >= R.Life) { R.Life = 0.f; if (Q) Q->SetActorHiddenInGame(true); continue; }
		if (!Q) continue;
		const float U = R.Age / R.Life;
		const float Rad = FMath::Lerp(R.R0, R.R1, 1.f - FMath::Square(1.f - U));   // opens fast, then slows
		Q->SetActorLocationAndRotation(R.P, R.bFloor ? FQuat::Identity : FRotationMatrix::MakeFromXZ(FVector(1, 0, 0), FVector(0, 1, 0)).ToQuat());
		Q->SetActorScale3D(FVector(Rad * 2.f / 100.f, Rad * 2.f * (R.bFloor ? 0.5f : 1.f) / 100.f, 1.f));
		if (UMaterialInstanceDynamic* M = Cast<UMaterialInstanceDynamic>(Q->GetStaticMeshComponent()->GetMaterial(0)))
			M->SetScalarParameterValue(TEXT("Glow"), R.Glow * (R.bFloor ? 1.f - U : FMath::Square(1.f - U)));
	}
}

void AFTBrawlGameMode::FeelPaused(float Dt)
{
	FeelStopT += Dt;
	if (!FeelOn()) return;
	// the flash is short and does not wait for the freeze: white for its first steps, then the frozen drawing shows
	if (P.Flash > 0.f) P.Flash -= Dt;
	for (FFTBrawlBody& F : Foes) if (F.Flash > 0.f) F.Flash -= Dt;
}

void AFTBrawlGameMode::FeelStep(float Dt)
{
	if (P.Flash > 0.f) P.Flash -= Dt;
	const float Keep = FMath::Exp(-KbRate * Dt);
	FeelDustT -= Dt;
	bool bDust = false;
	auto Slide = [&](FFTBrawlBody& B)
	{
		if (B.Kb == 0.f) return;
		if (B.St == TEXT("lifted") || B.St == TEXT("hurled") || B.St == TEXT("held") || B.St == TEXT("dead")) { B.Kb = 0.f; return; }
		B.X += B.Kb * Dt; B.Kb *= Keep;
		if (FMath::Abs(B.Kb) < 3.f) { B.Kb = 0.f; return; }
		if (FMath::Abs(B.Kb) > 110.f && B.Z <= 0.f && FeelDustT <= 0.f)
		{ bDust = true; Accent(EAccent::Dust, World(B.X - FMath::Sign(B.Kb) * 10.f, B.Y + 7.f, 8.f), B.St == TEXT("down") ? 46.f : 34.f, FMath::Sign(B.Kb), false); }
	};
	Slide(P);
	if (P.Kb != 0.f)
	{
		const float Lo = LockX >= 0 ? LockX + 30 : Cam + 30, Hi = LockX >= 0 ? LockX + ScreenW - 30 : ArenaEnd() + ScreenW - 30;   // UpdatePlayer's own limits
		if (P.X < Lo || P.X > Hi) { P.X = FMath::Clamp(P.X, Lo, Hi); P.Kb = 0.f; }
	}
	for (FFTBrawlBody& F : Foes) Slide(F);
	if (bDust) FeelDustT = 0.07f;
}

float AFTBrawlGameMode::FeelTimeScale(float RealDt)
{
	const float Dt = FMath::Min(0.05f, RealDt);
	if (!bWon) FeelRealT += Dt;
	if (FeelSlowT <= 0.f) return 1.f;
	if (ContactPause > 0.f) return 1.f;   // the freeze first, at full speed; the slow beat is what comes out of it
	FeelSlowT -= Dt;
	const float U = 1.f - FMath::Max(0.f, FeelSlowT) / FMath::Max(0.01f, FeelSlowLen);
	const float Scale = U < 0.67f ? FeelSlowTo : FMath::Lerp(FeelSlowTo, 1.f, FMath::SmoothStep(0.67f, 1.f, U));
	FeelSlowAdded += Dt * (1.f - Scale);
	return Scale;
}

// ---------------------------------------------------------------- the camera and the struck figure
void AFTBrawlGameMode::FeelCamera(float RealDt, float MidX, float& Sx, float& Sz, float& LeadX, float& RiseZ)
{
	const bool bOff = !FeelOn() || ReducedPresentationMotion();
	// the kick: a spring knocked off its rest, stepped finely so a long frame cannot blow it up
	const float Omega = 2.f * PI * KickHz;
	const int32 N = FMath::Clamp(FMath::CeilToInt32(RealDt / 0.004f), 1, 16);
	const float H = FMath::Min(RealDt, 0.064f) / N;
	for (int32 I = 0; I < N; ++I)
	{
		FeelKickVel += (-Omega * Omega * FeelKickOff - 2.f * KickDamp * Omega * FeelKickVel) * H;
		FeelKickOff += FeelKickVel * H;
	}
	if (bOff) { FeelKickOff = FeelKickVel = FVector2D::ZeroVector; }
	Sx += float(FeelKickOff.X); Sz += float(FeelKickOff.Y);
	// the lead: toward the middle of the action, and a little the way he faces
	float WantLead = 0.f, WantRise = 0.f;
	if (!bOff && !bWon && P.Hp > 0.f && BossIntro <= 0.f && !InCut())
	{
		float Focus = P.X, Near = 380.f, Top = P.Z;
		for (const FFTBrawlBody& F : Foes)
		{
			if (F.St == TEXT("dead") || !(F.bIn || F.bSeen)) continue;
			const float D = FMath::Abs(F.X - P.X);
			if (D < Near) { Near = D; Focus = (P.X + F.X) * 0.5f; }
			if (F.St == TEXT("fly") || F.St == TEXT("hurled") || F.St == TEXT("lifted")) Top = FMath::Max(Top, F.Z);
		}
		WantLead = FMath::Clamp((Focus * PxCm - MidX) * 0.22f, -95.f, 95.f) + P.Dir * 26.f;
		WantRise = FMath::Clamp((Top - 110.f) * PxCm * 0.55f, 0.f, 90.f);
	}
	FeelLead = FMath::FInterpTo(FeelLead, WantLead, RealDt, 2.2f);
	FeelRise = FMath::FInterpTo(FeelRise, WantRise, RealDt, WantRise > FeelRise ? 6.f : 3.f);
	LeadX = FeelLead; RiseZ = FeelRise;
}

void AFTBrawlGameMode::FeelBody(FFTBrawlBody& F)
{
	if (F.Rock <= 0.f) return;
	if (ContactPause <= 0.f || !FeelOn() || ReducedPresentationMotion() || !F.Actor) { F.Rock = 0.f; return; }
	const float A = F.Rock * PxCm * FMath::Min(1.f, ContactPause / (4.f * StepDt));
	const float Sign = FMath::Fmod(float(GetWorld()->GetTimeSeconds()) * 22.f, 1.f) < 0.5f ? 1.f : -1.f;
	F.Actor->AddActorWorldOffset(FVector(Sign * A, 0.f, 0.f));
}
