// ENEMY VARIETY AND THE BOSSES' SECOND PHASE (stage 2, "variety").
//
// 1. LOOKS. Every enemy character has six looks: look 0 is the drawing as painted, looks 1..5 recolour it in the sprite
//    material (M_BrawlEnemyV, Content/Python/brawl_variety.py) from a colour table per character (FTBrawlLooks.inl,
//    written by Content/Python/brawl_variety_looks.py): clothes, trims, hair where the drawing lets it be told apart,
//    and natural skin tones (a step of value and saturation inside the skin's own hue range, nothing else).
//    A character's spawns take looks 0, 1, 2 ... in turn, so the same man never stands twice in a wave.
//
// 2. WAYS OF FIGHTING ("minds"). Each role has three; a role's spawns take them in turn, so a wave mixes them.
//
//      role      1st of the role        2nd                      3rd
//      rusher    brawler                flanker                  circler
//      grappler  brawler                flanker                  circler
//      thrower   brawler                coward                   skirmisher
//      heavy     brawler                blocker                  charger
//      guard     brawler                blocker                  flanker
//      dealer    brawler                coward                   flanker
//
//    brawler     as the fight always was: up to three step in, the rest hang back.
//    circler     walks a ring round Nova (about 138 px out) and does NOT step in until there is an opening (Nova busy
//                in a move, hurt, held, or with his back to him) or his patience (3.6 s) runs out; then 1.3 s of
//                closing at 1.45x speed and his ordinary wind-up.
//    charger     never hangs back (ignores the three-at-once rule), walks 1.25x, cools down 1.7x faster.
//    flanker     goes round Nova on another lane to the side Nova's back is turned to (re-decided every 2 s), then
//                steps in from there. A flanking dealer lines his cards up from behind.
//    blocker     his guard is up while he walks or stands facing Nova: a hit from the front that would not knock him
//                down is taken on the guard (no damage, "BLOCK", he skids 7 px). Three blocked hits break the guard
//                ("GUARD BROKEN", the third hit lands) for 3 s. Kicks, the uppercut, anything that knocks down, a
//                hit from behind, a grab, or a hit while he winds up or swings go through as always.
//    coward      (throwers) keeps 400 to 430 px off instead of 330 to 380, backs away 1.3x fast when Nova is within
//                230 px, and only slaps when cornered (56 px instead of 100).
//    skirmisher  (bottle thrower) works from 215 px, cools down between throws 0.65x as long.
//    No wind-up, tell or attack state is shorter for any of them: only where they walk, when they choose to start,
//    and how long they wait between attacks.
//
// 3. THE BOSSES' SECOND PHASE. At half health, the next step the boss is on his feet and not inside a move (walking,
//    standing, or the last 0.06 s of a stagger):
//      - a transition beat of 1.25 s: the fight holds (like his entrance), he throws his arms up (state "rage": a
//        stand-in, his taunt drawing), the screen shakes, the banner reads "<NAME> IS FURIOUS / HIS MOVES COME IN TWOS NOW";
//      - from then on the ink line round him burns in his colour (material glow, pulsing), he stands 5 % larger and
//        leans in 4 degrees while walking; he walks 1.18x, and waits 0.6x as long after a swing;
//      - new patterns, every one with the SAME tell at its FULL length:
//          one-two      after a swing, 60 % of the time (Nova still near) a second wind-up and the other swing;
//          double rush  after a rush across the room: the "!" again (0.55 s) and a rush back;
//          encore       his signature move a second time straight after the first, told in full again:
//                       Earl leaps again (slamwind 0.55 s; the second leap travels up to 300 px/s toward Nova),
//                       the Pit Boss draws and locks again (1.1 s) for a second volley, Frankie swigs again (0.85 s),
//                       the Kingpin lines up three new lanes (1.0 s), Sven drops the gloves again (0.75 s) after a
//                       lunge that missed, the Commodore winds the anchor again (0.9 s).
//                       Vinnie has no encore (his dizzy spell is the player's reward): his whirlwind chases at 185
//                       px/s instead of 150.
//      - his call for help (three more foes) comes 2.6 s after the beat, so its banner does not wipe the phase's.
//    Tell times are untouched: wind = the kind's Wind, aim 0.55, slamwind 0.55, whirltell 0.85, draw 1.1, swig 0.85,
//    hold 1.0, gloves 0.75, windup 0.9. No existing state changes length.
//
// Development (not in shipping builds):
//   ftb.Variety looks <role>            clears the room and stands six quiet foes of that role (tonight's own regular)
//                                       in a row, looks 0..5 left to right. Roles: rusher grappler thrower heavy guard dealer.
//   ftb.Variety mind <role> <mind> [n] [px] [cd]   clears the room and sends in n (default 1) foes of that role with that
//                                       mind (brawler circler charger flanker blocker coward; skirmisher = charger), starting
//                                       px (default 330) to Nova's right, the first attack not before cd (default 0.8) s.
//   ftb.Variety phase2                  the boss is put at half health (the second phase starts when he is next free).
//   ftb.Variety bosshp <0..1>           the boss's health as a fraction.
//   ftb.Variety list                    logs the table above.
//   ftb.VarietyTrace 1                  logs what the minds decide ("FTBRAWL mind ..."); 2 = also every foe's place twice a
//                                       second ("FTBRAWL mindpos t= id= mind= st= dx= dy= ...", dx / dy = from Nova, px).
// Log lines: "FTBRAWL variety spawn id= role= who= look= mind=", "FTBRAWL block id= guard=", "FTBRAWL guard broken id=",
// "FTBRAWL boss phase2 ...", "FTBRAWL boss phase2 beat over", "FTBRAWL boss encore tell=", "FTBRAWL boss onetwo".
#include "FTBrawlGameMode.h"
#include "FTBrawlSpriteBank.h"
#include "FTBrawlFighter.h"
#include "HAL/IConsoleManager.h"
#include "Engine/World.h"

namespace FTBrawlVariety
{
	enum EMind : uint8 { M_Brawler, M_Circler, M_Charger, M_Flanker, M_Blocker, M_Coward, M_Num };
	const TCHAR* MindNames[M_Num] = { TEXT("brawler"), TEXT("circler"), TEXT("charger"), TEXT("flanker"), TEXT("blocker"), TEXT("coward") };
	struct FRoleMinds { const TCHAR* Role; uint8 Minds[3]; };
	const FRoleMinds RoleMinds[] = {
		{ TEXT("rusher"), { M_Brawler, M_Flanker, M_Circler } },
		{ TEXT("grappler"), { M_Brawler, M_Flanker, M_Circler } },
		{ TEXT("thrower"), { M_Brawler, M_Coward, M_Charger } },
		{ TEXT("heavy"), { M_Brawler, M_Blocker, M_Charger } },
		{ TEXT("guard"), { M_Brawler, M_Blocker, M_Flanker } },
		{ TEXT("dealer"), { M_Brawler, M_Coward, M_Flanker } },
	};
	const float VFT = 360.f, VFB = 560.f, VW = 960.f, VStep = 1.f / 60.f, VKeepIn = 70.f;
	const float BeatLen = 1.25f, CallDelay = 2.6f;

	TAutoConsoleVariable<int32> CVarTrace(TEXT("ftb.VarietyTrace"), 0, TEXT("Development: 1 = log what the foes' minds decide (FTBRAWL mind ...)"));
#if !UE_BUILD_SHIPPING
	FAutoConsoleCommandWithWorldAndArgs GFTBrawlVariety(TEXT("ftb.Variety"), TEXT("Development: ftb.Variety looks <role> | mind <role> <mind> [n] | phase2 | bosshp <0..1> | list (FTBrawlVariety.cpp)"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World) { if (AFTBrawlGameMode* G = World ? World->GetAuthGameMode<AFTBrawlGameMode>() : nullptr) G->VarietyCommand(Args); }));
#endif
	bool Free(const FFTBrawlBody& F) { return F.St == TEXT("walk") || F.St == TEXT("idle"); }
	FLinearColor RageColor(int32 Night)
	{
		static const TCHAR* Hex[7] = { TEXT("ff5a1e"), TEXT("ff3fb4"), TEXT("ff2a2a"), TEXT("ffb020"), TEXT("30e8ff"), TEXT("7fd0ff"), TEXT("ffd040") };
		return FLinearColor(FColor::FromHex(Hex[FMath::Clamp(Night, 1, 7) - 1]));
	}
}
using namespace FTBrawlVariety;

void AFTBrawlGameMode::VarietyReset()
{
	MindCount.Reset(); LookCount.Reset(); PhaseBeat = 0.f;
}

// MakeFoe: the n-th spawn of a character wears look n (mod 6), the n-th of a role fights the role's n-th way (mod 3)
void AFTBrawlGameMode::VarietySpawn(FFTBrawlBody& F, FName Kind)
{
	if (!F.Actor || !F.Actor->HasSprite()) return;
	const FString Who = F.Actor->SpriteName();
	int32& NLook = LookCount.FindOrAdd(Who);
	F.Look = NLook % AFTBrawlFighter::NumEnemyLooks(Who); NLook++;
	F.Actor->SetEnemyLook(F.Look);
	F.Mind = M_Brawler;
	for (const FRoleMinds& R : RoleMinds)
		if (F.K.Role == R.Role) { int32& N = MindCount.FindOrAdd(F.K.Role); F.Mind = R.Minds[N % 3]; N++; break; }
	F.MindT = 0; F.MindWait = 0; F.MindSide = 0; F.Guard = 3; F.GuardT = 0;
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL variety spawn id=%d role=%s who=%s look=%d mind=%s"), F.Id, *F.K.Role.ToString(), *Who, F.Look, MindNames[F.Mind]);
}

float AFTBrawlGameMode::VarietyCd(const FFTBrawlBody& F) const
{
	if (F.bBoss) return F.Phase == 2 ? 0.6f : 1.f;
	if (F.Mind == M_Charger) return F.K.Role == TEXT("thrower") ? 0.65f : 1.f;   // (a melee charger's cooldown runs faster in VarietyWalk)
	return 1.f;
}

float AFTBrawlGameMode::VarietyMelee(const FFTBrawlBody& F) const
{
	return F.Mind == M_Coward ? 56.f : 100.f;
}

// HitFoe: true = the blocker took it on his guard, nothing else happens to him
bool AFTBrawlGameMode::VarietyBlock(FFTBrawlBody& F, float Dmg, bool bDown)
{
	if (F.bBoss || F.Mind != M_Blocker || bDown || F.Guard <= 0 || F.Z > 0.f) return false;
	if (!(Free(F) || F.St == TEXT("block"))) return false;   // winding up, swinging, hurt: open
	if ((P.X - F.X) * F.Dir <= 0.f) return false;              // from behind
	if (F.Hp - Dmg <= 0.f && F.Guard <= 1) return false;
	F.Guard--; F.bIn = true;
	if (F.Guard <= 0)
	{
		F.GuardT = 3.f;
		Pop(F.X, F.Y, 190, TEXT("GUARD BROKEN"), FLinearColor(1.f, 0.82f, 0.25f), true);
		Sfx(TEXT("break"), 0.7f, 1.2f);
		UE_LOG(LogTemp, Display, TEXT("FTBRAWL guard broken id=%d t=%.2f"), F.Id, StT);
		return false;   // this hit lands
	}
	F.St = TEXT("block"); F.T = 0; F.Flash = 0.16f; F.X += P.Dir * 7.f;
	if (F.Actor) F.Actor->SetFlashColor(FLinearColor(0.55f, 0.8f, 1.f));   // a cold flash, not the red-white of a hit that lands
	CombatCue(ECombatCue::Jab, World(F.X - P.Dir * 26.f, F.Y + 9.f, 112.f * F.K.Height / 175.f), P.Dir, false);
	ContactPause = FMath::Max(ContactPause, 2.f * VStep);
	FeelImpact(World(F.X - P.Dir * 26.f, F.Y + 11.f, 112.f * F.K.Height / 175.f), -P.Dir, 0, FLinearColor(0.6f, 0.82f, 1.f));   // cold sparks back off his guard (FTBrawlFeel.cpp)
	Shake = FMath::Min(1.f, Shake + 0.12f);
	Pop(F.X, F.Y, 170, TEXT("BLOCK"), FLinearColor(0.62f, 0.8f, 1.f), false);
	Sfx(TEXT("wood"), 0.6f, 1.35f);
	Rumble(0.2f, 0.06f, TEXT("block"));
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL block id=%d guard=%d t=%.2f"), F.Id, F.Guard, StT);
	return true;
}

// UpdateFoes, after the old reaction states: the two new foe states. true = this foe is done for the step.
bool AFTBrawlGameMode::VarietyState(FFTBrawlBody& F, float Dt)
{
	if (CVarTrace.GetValueOnGameThread() >= 2 && FMath::Fmod(StT, 0.5f) < Dt)
		UE_LOG(LogTemp, Display, TEXT("FTBRAWL mindpos t=%.2f id=%d mind=%s st=%s dx=%.0f dy=%.0f novadir=%.0f guard=%d"), StT, F.Id, F.bBoss ? TEXT("boss") : MindNames[F.Mind], *F.St.ToString(), F.X - P.X, F.Y - P.Y, P.Dir, F.Guard);
	if (F.Mind == M_Blocker && F.Guard <= 0) { F.GuardT -= Dt; if (F.GuardT <= 0.f) { F.Guard = 3; if (CVarTrace.GetValueOnGameThread()) UE_LOG(LogTemp, Display, TEXT("FTBRAWL mind id=%d blocker guard back t=%.2f"), F.Id, StT); } }
	if (F.St == TEXT("block"))
	{
		if (F.T > 0.2f) { F.St = TEXT("walk"); F.T = 0; F.Cd = FMath::Min(F.Cd, 0.3f); }   // he answers soon after a block (his wind-up is the usual one)
		return true;
	}
	if (F.St == TEXT("rage"))
	{
		if (PhaseBeat <= 0.f) { F.St = TEXT("walk"); F.T = 0; F.Cd = 0.5f; }
		return true;
	}
	return false;
}

// the walking part of a melee foe's step: Tx, Ty = where he heads, bWant = may step in and swing, Sp = his speed
void AFTBrawlGameMode::VarietyWalk(FFTBrawlBody& F, float Dt, int32 Engaged, bool& bWant, float& Tx, float& Ty, float& Sp)
{
	if (F.bBoss) { if (F.Phase == 2) Sp *= 1.18f; return; }
	if (F.Mind == M_Brawler || F.K.Sp <= 0.f) return;
	const float Side = F.X >= P.X ? 1.f : -1.f;   // which side of Nova he stands on
	const bool bBack = (F.X - P.X) * P.Dir < 0.f;   // Nova's back is to him
	const bool bBusy = !(P.St == TEXT("idle") || P.St == TEXT("walk"));
	const bool bTrace = CVarTrace.GetValueOnGameThread() != 0;
	F.MindT -= Dt;
	if (F.Mind == M_Circler)
	{
		if (F.MindT <= 0.f)
		{
			F.MindWait += Dt;
			const bool bOpening = (bBack || bBusy) && F.MindWait > 0.5f;
			if ((bOpening || F.MindWait > 3.6f) && Engaged < 3)
			{
				if (bTrace) UE_LOG(LogTemp, Display, TEXT("FTBRAWL mind id=%d circler steps in (%s) waited=%.2f t=%.2f"), F.Id, bOpening ? (bBack ? TEXT("back turned") : TEXT("nova busy")) : TEXT("patience"), F.MindWait, StT);
				F.MindT = 1.3f; F.MindWait = 0.f;
			}
		}
		if (F.MindT > 0.f) { bWant = true; Sp *= 1.45f; Tx = P.X + Side * (F.K.Reach - 12.f); Ty = P.Y; }
		else
		{
			bWant = false;
			const float A = StT * 1.25f + F.Id * 1.7f;
			Tx = P.X + Side * (138.f + 22.f * FMath::Cos(A)); Ty = FMath::Clamp(P.Y + 58.f * FMath::Sin(A), VFT, VFB);
		}
	}
	else if (F.Mind == M_Charger)
	{
		bWant = true; Sp *= 1.25f; F.Cd -= Dt * 0.7f;
		Tx = P.X + Side * (F.K.Reach - 12.f); Ty = P.Y;
	}
	else if (F.Mind == M_Flanker)
	{
		if (F.MindT <= 0.f || F.MindSide == 0.f)
		{
			const float New = -P.Dir;
			if (bTrace && New != F.MindSide) UE_LOG(LogTemp, Display, TEXT("FTBRAWL mind id=%d flanker heads %s of nova t=%.2f"), F.Id, New > 0 ? TEXT("right") : TEXT("left"), StT);
			F.MindSide = New; F.MindT = 2.f;
		}
		if (Side == F.MindSide)
		{
			if (bBack) bWant = bWant || Engaged < 4;   // behind him: he may be a fourth
			Tx = P.X + Side * (bWant ? F.K.Reach - 12.f : 150.f); Ty = P.Y + (bWant ? 0.f : (F.Id % 2 ? -50.f : 50.f));
		}
		else
		{
			// round him on another lane, never through him
			bWant = false; Sp *= 1.35f;
			float Lane = F.Y >= P.Y ? 66.f : -66.f;
			if (P.Y + Lane > VFB - 4.f || P.Y + Lane < VFT + 4.f) Lane = -Lane;
			Ty = P.Y + Lane; Tx = P.X + F.MindSide * 130.f;
		}
	}
	else if (F.Mind == M_Blocker) Sp *= 0.92f;   // he comes on steadily behind his guard
	if (LockX >= 0.f) Tx = FMath::Clamp(Tx, LockX + 30.f, LockX + VW - 30.f);
}

// the same for the bottle thrower and the card dealer (they keep a range instead of closing)
void AFTBrawlGameMode::VarietyThrower(FFTBrawlBody& F, float Dt, bool bDealer, float& Tx, float& Ty, float& Sp)
{
	if (F.Mind == M_Brawler || F.K.Sp <= 0.f) return;
	const float Ad = FMath::Abs(P.X - F.X);
	const float Side = F.X >= P.X ? 1.f : -1.f;
	F.MindT -= Dt;
	if (F.Mind == M_Coward)
	{
		Tx = P.X + Side * (bDealer ? 430.f : 400.f);
		if (Ad < 230.f) Sp *= 1.3f;
	}
	else if (F.Mind == M_Charger) Tx = P.X + Side * 215.f;
	else if (F.Mind == M_Flanker)
	{
		if (F.MindT <= 0.f || F.MindSide == 0.f) { F.MindSide = -P.Dir; F.MindT = 2.5f; }
		Tx = P.X + F.MindSide * 380.f;
		if (LockX >= 0.f && (Tx < LockX + VKeepIn + 40.f || Tx > LockX + VW - VKeepIn - 40.f) && Side != F.MindSide) { F.MindSide = Side; Tx = P.X + Side * 380.f; }   // no room behind Nova: stay
		if (Side != F.MindSide) { float Lane = F.Y >= P.Y ? 70.f : -70.f; if (P.Y + Lane > VFB - 4.f || P.Y + Lane < VFT + 4.f) Lane = -Lane; Ty = P.Y + Lane; Sp *= 1.3f; }
	}
}

// ---------------------------------------------------------------- the bosses' second phase
bool AFTBrawlGameMode::VarietyBossPhase(FFTBrawlBody& F)
{
	if (!F.bBoss || F.Phase != 1 || F.Hp <= 0.f || F.Hp > F.Max * 0.5f || P.Hp <= 0.f) return false;
	// on his feet and not inside a move: walking, standing, or at the end of a stagger (he shakes the hit off).
	// Not out of a dizzy spell (that window is the player's) and not while he holds Nova.
	if (!Free(F) || F.Z > 0.f || P.St == TEXT("held")) return false;   // a stagger plays to its end first (its length is the contract's)
	F.Phase = 2; F.PhaseAt = StT; F.St = TEXT("rage"); F.T = 0; F.Dir = P.X > F.X ? 1.f : -1.f;
	F.S *= 1.05f; F.bChain = F.bEncore = F.bOneTwo = false;
	PhaseBeat = BeatLen;
	Banner = FString::Printf(TEXT("%s IS FURIOUS"), *BossName); BannerSub = TEXT("HIS MOVES COME IN TWOS NOW · THE WARNINGS ARE THE SAME"); BannerT = 0;
	Shake = 1.f;
	Sfx(TEXT("whooshbig"), 0.9f, 0.7f); Sfx(TEXT("powerup"), 0.7f, 0.6f);
	Rumble(0.8f, 0.5f, TEXT("phase2"));
	Pop(F.X, F.Y, 250, TEXT("!!"), RageColor(Night), true);
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL boss phase2 boss=%s hp=%.0f/%.0f t=%.2f beat=%.2f"), *BossName, F.Hp, F.Max, StT, BeatLen);
	return true;
}

// Step: while the beat plays, nothing in the fight moves (Nova cannot be hit, the boss cannot be hit)
bool AFTBrawlGameMode::VarietyBeat(float Dt)
{
	if (PhaseBeat <= 0.f) return false;
	PhaseBeat -= Dt;
	const int32 Bi = BossIdx();
	if (Bi == INDEX_NONE) { PhaseBeat = 0.f; return false; }
	FFTBrawlBody& B = Foes[Bi];
	B.T += Dt; if (B.Flash > 0) B.Flash -= Dt;
	if (B.T < 0.85f && !ReducedPresentationMotion()) Shake = FMath::Max(Shake, 0.4f);
	// he roars Nova off him: Nova slides back to 180 px, so he is never under the boss while the beat holds the picture,
	// and the boss has the room his doubled move needs
	{
		const float Dx = P.X - B.X, Ad = FMath::Abs(Dx);
		if (Ad < 180.f && P.Hp > 0.f)
		{
			float X = P.X + (Dx >= 0.f ? 1.f : -1.f) * FMath::Min(180.f - Ad, 560.f * Dt);
			if (LockX >= 0.f) X = FMath::Clamp(X, LockX + 30.f, LockX + VW - 30.f);
			P.X = X;
		}
	}
	if (PhaseBeat <= 0.f)
	{
		PhaseBeat = 0.f;
		// his second phase opens with his own move, told as always: no waiting on the cooldown, and for a moment blows
		// hurt him without stopping him (then the warning itself carries him)
		if (B.St == TEXT("rage")) { B.St = TEXT("walk"); B.T = 0; B.Cd = 0.5f; B.SlamCd = 0.f; B.Shrug = 1.2f; }
		P.Inv = FMath::Max(P.Inv, 0.35f);
		UE_LOG(LogTemp, Display, TEXT("FTBRAWL boss phase2 beat over t=%.2f"), StT);
	}
	return true;
}

bool AFTBrawlGameMode::VarietyCallOk(const FFTBrawlBody& F) const
{
	return F.Phase == 2 && StT - F.PhaseAt > CallDelay;
}

// the move that just ended is told and done a second time (Tell = the state its warning starts in). A flag per kind
// (the rush, the signature move) makes it exactly twice.
bool AFTBrawlGameMode::VarietyEncore(FFTBrawlBody& F, FName Tell)
{
	const bool bRush = Tell == TEXT("aim");
	bool& Flag = bRush ? F.bChain : F.bEncore;
	if (!F.bBoss || F.Phase != 2 || P.Hp <= 0.f || F.Hp <= 0.f) { Flag = false; return false; }
	if (Flag) { Flag = false; return false; }
	// only straight out of the strike itself (not after a hug's throw, a broken hug or a dizzy spell)
	if (!bRush && !(F.St == TEXT("slam") || F.St == TEXT("fire") || F.St == TEXT("breathe") || F.St == TEXT("bowl") || F.St == TEXT("lunge") || F.St == TEXT("sweep"))) return false;
	Flag = true;
	F.St = Tell; F.T = 0; F.Dir = P.X > F.X ? 1.f : -1.f;
	F.SigY = F.Y; F.SigX = F.X; F.Shots = 0; F.bStruck = false; F.Hold = 0;
	if (bRush) F.Y += (P.Y - F.Y) * 0.6f;
	if (Tell == TEXT("hold"))
	{
		// three NEW lanes: half a step off the first three (towards Nova; the other way if the floor ends there)
		const float Gap = 68.f, Old = Lanes[1];
		float C = FMath::Clamp(Old + (P.Y >= Old ? 0.5f : -0.5f) * Gap, VFT + Gap, VFB - Gap);
		if (FMath::Abs(C - Old) < 20.f) C = FMath::Clamp(Old + (P.Y >= Old ? -0.5f : 0.5f) * Gap, VFT + Gap, VFB - Gap);
		Lanes[0] = C - Gap; Lanes[1] = C; Lanes[2] = C + Gap;
	}
	Pop(F.X, F.Y, 240, TEXT("AGAIN!"), FLinearColor::White, true);   // white: the rage colour vanished on a wall of the same colour
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL boss encore tell=%s t=%.2f"), *Tell.ToString(), StT);
	return true;
}

bool AFTBrawlGameMode::VarietyOneTwo(FFTBrawlBody& F)
{
	if (!F.bBoss || F.Phase != 2 || P.Hp <= 0.f) return false;
	if (F.bOneTwo) { F.bOneTwo = false; return false; }
	const float Dx = P.X - F.X, Dy = P.Y - F.Y;
	if (FMath::Abs(Dx) > F.K.Reach + 60.f || FMath::Abs(Dy) > 40.f || Rand() > 0.6f) return false;
	F.bOneTwo = true;
	F.St = TEXT("wind"); F.T = 0; F.Dir = Dx > 0 ? 1.f : -1.f; F.Combo = (F.Combo + 1) % 2;
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL boss onetwo t=%.2f wind=%.2f"), StT, F.K.Wind);
	return true;
}

void AFTBrawlGameMode::VarietyLeap(FFTBrawlBody& F, float Dt)
{
	if (!F.bEncore || F.Phase != 2 || F.T < 0.12f) return;
	const float Dx = P.X - F.X, Ad = FMath::Abs(Dx);
	if (Ad > 70.f) F.X += FMath::Sign(Dx) * FMath::Min(Ad - 70.f, 300.f * Dt);
	if (LockX >= 0.f) F.X = FMath::Clamp(F.X, LockX + 40.f, LockX + VW - 40.f);
}

float AFTBrawlGameMode::VarietyWhirlSpeed(const FFTBrawlBody& F) const
{
	return F.Phase == 2 ? 185.f : 150.f;
}

// SyncBody: looks only
void AFTBrawlGameMode::VarietyStyle(FFTBrawlBody& F, FName St, float& Lean)
{
	if (!F.Actor) return;
	if (F.bBoss)
	{
		float Rage = 0.f;
		if (F.Phase == 2 && F.Hp > 0.f)
		{
			const float In = FMath::Clamp((StT - F.PhaseAt) / 0.8f, 0.f, 1.f);
			Rage = In * (ReducedPresentationMotion() ? 0.8f : 0.74f + 0.26f * FMath::Sin(StT * 7.f));
			F.Actor->SetAccent(0.25f, 1.f, -4.f);
		}
		else F.Actor->SetAccent(0.f, 1.f, 0.f);
		F.Actor->SetRage(Rage, RageColor(Night));
		return;
	}
	if (F.Mind == M_Blocker)
	{
		const bool bUp = F.Guard > 0 && (St == TEXT("walk") || St == TEXT("idle") || St == TEXT("block"));
		F.Actor->SetAccent(bUp ? 0.45f : 0.f, bUp ? 0.965f : 1.f, bUp ? 3.f : 0.f);
		F.Actor->SetFlashColor(St == TEXT("block") ? FLinearColor(0.55f, 0.8f, 1.f) : FLinearColor(1.f, 0.85f, 0.85f));
	}
}

// ---------------------------------------------------------------- development
void AFTBrawlGameMode::VarietyCommand(const TArray<FString>& Args)
{
#if !UE_BUILD_SHIPPING
	const FString Cmd = Args.Num() ? Args[0].ToLower() : TEXT("list");
	if (Cmd == TEXT("list"))
	{
		for (const FRoleMinds& R : RoleMinds) UE_LOG(LogTemp, Display, TEXT("FTBRAWL variety role=%s minds=%s,%s,%s"), R.Role, MindNames[R.Minds[0]], MindNames[R.Minds[1]], MindNames[R.Minds[2]]);
		return;
	}
	if (Cmd == TEXT("phase2") || Cmd == TEXT("bosshp"))
	{
		const int32 Bi = BossIdx();
		if (Bi == INDEX_NONE) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL variety FAIL no boss out (FTGo N<n>boss)")); return; }
		const float Frac = Cmd == TEXT("phase2") ? 0.5f : FMath::Clamp(Args.Num() > 1 ? FCString::Atof(*Args[1]) : 0.5f, 0.01f, 1.f);
		Foes[Bi].Hp = Foes[Bi].Max * Frac;
		UE_LOG(LogTemp, Display, TEXT("FTBRAWL variety boss hp=%.0f/%.0f phase=%d"), Foes[Bi].Hp, Foes[Bi].Max, Foes[Bi].Phase);
		return;
	}
	if (bDay || InCut() || SystemScreen) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL variety FAIL needs a fight on screen")); return; }
	if (Args.Num() < 2) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL variety FAIL usage: ftb.Variety looks <role> | mind <role> <mind> [n] | phase2 | bosshp <f> | list")); return; }
	const FName RoleName(*Args[1].ToLower());
	if (!Kinds.Contains(RoleName)) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL variety FAIL unknown role %s"), *Args[1]); return; }
	UFTBrawlSpriteBank::TestStage(this);   // a role tonight's waves do not have may be stood here
	PressKeys(FIn());
	bAuto = bKeyTest = bAutoGrab = false;
	Rnd.Initialize(1986 + 23);
	ResetState(0, false);
	Cam = LockX = 0.f; P.Y = 470.f; P.Dir = 1.f; P.Inv = 0.f;
	bSpawned = true; BannerT = -1.f;
	if (Cmd == TEXT("looks"))
	{
		P.X = 60.f; P.Y = 548.f;
		for (int32 i = 0; i < 6; i++)
		{
			Foes.Add(MakeFoe(9100 + i, RoleName, 170.f + i * 132.f, 452.f + (i % 2) * 4.f));
			FFTBrawlBody& F = Foes.Last();
			F.Cd = 600.f; F.K.Sp = 0.f; F.bIn = true; F.Dir = -1.f; F.St = TEXT("idle"); F.T = 0; F.Hp = F.Max = 200.f; F.Mind = M_Brawler;
			F.Look = i; if (F.Actor) F.Actor->SetEnemyLook(i);
		}
		UE_LOG(LogTemp, Display, TEXT("FTBRAWL variety looks role=%s who=%s looks=%d"), *RoleName.ToString(), Foes.Num() && Foes[0].Actor ? *Foes[0].Actor->SpriteName() : TEXT("?"),
			Foes.Num() && Foes[0].Actor ? AFTBrawlFighter::NumEnemyLooks(Foes[0].Actor->SpriteName()) : 0);
		return;
	}
	if (Cmd == TEXT("mind"))
	{
		int32 Mind = INDEX_NONE;
		const FString Want = Args.Num() > 2 ? Args[2].ToLower() : TEXT("brawler");
		for (int32 m = 0; m < M_Num; m++) if (Want == MindNames[m]) Mind = m;
		if (Want == TEXT("skirmisher")) Mind = M_Charger;
		if (Mind == INDEX_NONE) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL variety FAIL unknown mind %s"), *Want); return; }
		const int32 N = FMath::Clamp(Args.Num() > 3 ? FCString::Atoi(*Args[3]) : 1, 1, 6);
		const float Dist = FMath::Clamp(Args.Num() > 4 ? FCString::Atof(*Args[4]) : 330.f, 60.f, 480.f);   // how far to Nova's right he starts
		const float FirstCd = Args.Num() > 5 ? FCString::Atof(*Args[5]) : 0.8f;                           // seconds before his first attack
		P.X = 420.f;
		for (int32 i = 0; i < N; i++)
		{
			Foes.Add(MakeFoe(9200 + i, RoleName, P.X + Dist + i * 40.f, 440.f + i * 30.f));
			FFTBrawlBody& F = Foes.Last();
			F.Mind = uint8(Mind); F.bIn = true; F.Dir = -1.f; F.Cd = FirstCd; F.Hp = F.Max = FMath::Max(F.Max, 120.f);
		}
		UE_LOG(LogTemp, Display, TEXT("FTBRAWL variety mind role=%s mind=%s n=%d"), *RoleName.ToString(), MindNames[Mind], N);
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("FTBRAWL variety FAIL unknown command %s"), *Cmd);
#endif
}
