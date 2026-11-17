// Nova's wider move set (stage 2 of the final pass, "moves"). Everything here is the player's; no enemy state is new.
// The older moves (punch, kick, jump, jumpkick, pickup, lift, carry, hurl, wpn, toss ...) are untouched in length and
// timing: they live in AFTBrawlGameMode::UpdatePlayer, which calls into this file at seven marked places.
//
// Units as in the fight: brawl px (1 px = 1.4 cm), seconds, the fight steps 60 times a second.
// "Active" = the ticks on which the hit box is tested. Hit box = a range test like the older moves':
// front / back = px ahead of / behind Nova's feet along the room, depth = px either side of his lane (a boss: +20 front,
// +8 depth). Hurt box: Nova's is unchanged (every enemy attack tests its own range against his feet) except where a
// move says "nothing hits him".
//
//  move       input                                   length  active        damage        box (front/back/depth)  result
//  ---------  --------------------------------------  ------  ------------  ------------  ----------------------  -------------------------------
//  sidestep   double tap up / down                    0.30    -             -             -                       78 px into the room; nothing hits him 0 to 0.20, open 0.20 to 0.30
//  dashfwd    double tap left / right                 0.26    -             -             -                       about 125 px; punch -> dashatk, kick -> runatk, jump -> long jump
//  dashatk    punch in a dash                         0.36    0.07 - 0.19   11            86 / 10 / 26            staggers (no knock-down)
//  runatk     kick in a dash or while running         0.46    0.08 - 0.30   12            96 / 10 / 26            knocks down; he slides about 65 px
//  airjab     punch in a jump (twice a jump)          0.20    0.04 - 0.12   6             84 / 10 / 26            staggers; lifts a foe already in the air; then the jump goes on
//  dive       down + kick in a jump (above 36 px)     to the  0.05 - land   14            78 / 10 / 26            knocks down; he bounces off a hit
//             floor
//  (jumpkick  kick in a jump: as before; a third punch in a jump is also the old jumpkick)
//  launch     kick out of the 2nd punch (after 0.12)  0.44    0.10 - 0.20   9             78 / 10 / 26            pops the foe straight up (560 up, 10 along); jump cancels after a hit
//  charging   hold punch 0.30 s (empty hands)         <=1.40  -             -             -                       he stands; let go from 0.25 = heavy; full at 0.80
//  heavy      let the charge go                       0.52    0.12 - 0.22   16 to 30      88 to 102 / 10 / 26     knocks down, thrown further the fuller the charge
//  special    punch + kick together (also when held)  0.62    0.10 - 0.38   14            112 / 112 / 44          both sides, knocks down; costs 12 health (never the last point);
//                                                                                                                 nothing hits him 0 to 0.46
//  backatk    punch with a foe behind, none in front  0.30    0.06 - 0.14   10            80 behind / 26          staggers the one behind; he does not turn
//  stomp      kick over a downed foe, none standing   0.46    0.16          12            74 / 24 / 28            hits foes lying down; the first one keeps a survivor down 0.75 s more
//  pummel     punch while carrying a foe (3 a grab)   0.30    0.12          6             the carried foe         back to the carry afterwards
//  throwback  back + grab / kick / punch, carrying    0.40    0.14 release  (16 landing,  the body: 50 x 30       the body flies behind him (640 along, 300 up)
//                                                                           14 to others)
//  slam       down + grab / kick / punch, carrying    0.50    0.20          20, shock 8   shock 84 x 34           he lies in front; the standing near the impact go down
//  (hurl      grab / kick while carrying, or 1.6 s: as before)
//  wpnair     punch in a jump, weapon in hand         0.30    0.08 - 0.22   weapon x 1.2  reach - 10 (>= 80)      knocks down; one use; then the jump goes on
//  wpndash    punch in a dash / run, weapon in hand   0.40    0.10 - 0.22   weapon x 1.1  reach + 20              knocks down; one use
//  airtoss    kick in a jump, weapon in hand          0.24    0.08 release  as a throw    the thrown item's       the weapon flies from his height
//  wpnbreak   (looks only) the 0.24 s after a weapon breaks in his hand: he is free to move and act
//
// Juggling: a foe knocked into the air (state fly, 20 px or more off the floor, not a boss) can be hit again by any
// attack that reaches his height: up to 3 times a flight, at 100 / 80 / 60 % damage; each hit pops him up again.
//
// Brass knuckles double fists and feet and add half to a weapon, a beer adds a quarter, as for the older moves.
//
// Development commands (not in a shipping build):
//   ftb.Move <name> [power 0..1]   starts that move now, as its input would (a move that needs a jump, a carried foe or a
//                                  weapon first jumps, grabs or is handed a frying pan); ftb.Move list prints the table.
//                                  ftb.Move give [uses] [kind] puts a weapon in his hand.
//   ftb.MoveFixture <front|back|crowd|down|far|attacker|none> [px]   clears the room and stands quiet foes (200 health, they do not
//                                  walk or attack for 60 s) round Nova at x 440, lane 470; px = how far away the one foe stands.
//   ftb.Seq <step> <step> ...      plays a timed run of button presses through the ordinary input path, in game time
//                                  (so it lines up with a "film" line). A step = controls joined by "+", then an optional
//                                  ":seconds" it is held (default 0.05): fire kick jump grab left right up down, dashl dashr
//                                  (a double tap along the room), sideu sided (a double tap into it), wait.
//                                  Example, the launcher into an air combo: ftb.Seq fire wait:0.13 fire wait:0.08 kick wait:0.12
//                                  jump wait:0.10 fire wait:0.15 fire wait:0.15 kick
//   ftb.MoveTrace 1                logs every change of Nova's state: "FTBRAWL novastate <state> t=<fight s> was=<state> for=<s>".
//   ftb.AutoMoves 1                the autopilot (ftb.Auto 1) also uses these moves.
// Log lines: "FTBRAWL move <name> ...", "FTBRAWL juggle ...", "FTBRAWL dodge ...", "FTBRAWL stomp ...", "FTBRAWL pummel ...",
// "FTBRAWL throwback ...", "FTBRAWL slam ...", "FTBRAWL special ...".
#include "FTBrawlGameMode.h"
#include "FTBrawlSpriteBank.h"
#include "FTBrawlFighter.h"
#include "GameFramework/PlayerController.h"
#include "HAL/IConsoleManager.h"
#include "Engine/World.h"

namespace FTBrawlMoves
{
	const float StepDt = 1.f / 60.f;
	const float FloorTop = 360.f, FloorBottom = 560.f;
	TAutoConsoleVariable<int32> CVarMoveTrace(TEXT("ftb.MoveTrace"), 0, TEXT("Development: 1 = log every change of Nova's state with the fight time (FTBRAWL novastate ...)"));
	TAutoConsoleVariable<int32> CVarAutoMoves(TEXT("ftb.AutoMoves"), 0, TEXT("Brawl autopilot: 1 = it also uses the wider move set (FTBrawlMoves.cpp)"));

	// the table at the top of this file, for ftb.Move list and for anyone checking the numbers against the code
	struct FMoveDef { const TCHAR* Name; float Len, From, To, Dmg, Front, Back, Depth; bool bDown; const TCHAR* Note; };
	const FMoveDef GMoveDefs[] = {
		{ TEXT("sidestep"), 0.30f, 0, 0, 0, 0, 0, 0, false, TEXT("no hit on him 0-0.20; moves 78 px into the room") },
		{ TEXT("dashfwd"), 0.26f, 0, 0, 0, 0, 0, 0, false, TEXT("about 125 px along the room") },
		{ TEXT("dashatk"), 0.36f, 0.07f, 0.19f, 11, 86, 10, 26, false, TEXT("") },
		{ TEXT("runatk"), 0.46f, 0.08f, 0.30f, 12, 96, 10, 26, true, TEXT("slides about 65 px") },
		{ TEXT("airjab"), 0.20f, 0.04f, 0.12f, 6, 84, 10, 26, false, TEXT("twice a jump; then the jump goes on") },
		{ TEXT("dive"), 0.20f, 0.05f, 9.f, 14, 78, 10, 26, true, TEXT("until he lands; bounces off a hit") },
		{ TEXT("launch"), 0.44f, 0.10f, 0.20f, 9, 78, 10, 26, true, TEXT("pops the foe 560 up, 10 along") },
		{ TEXT("charging"), 1.40f, 0, 0, 0, 0, 0, 0, false, TEXT("let go from 0.25 = heavy; full at 0.80") },
		{ TEXT("heavy"), 0.52f, 0.12f, 0.22f, 16, 88, 10, 26, true, TEXT("damage 16-30, front 88-102 with the charge") },
		{ TEXT("special"), 0.62f, 0.10f, 0.38f, 14, 112, 112, 44, true, TEXT("costs 12 health; no hit on him 0-0.46") },
		{ TEXT("backatk"), 0.30f, 0.06f, 0.14f, 10, 10, 80, 26, false, TEXT("hits behind") },
		{ TEXT("stomp"), 0.46f, 0.16f, 0.16f, 12, 74, 24, 28, false, TEXT("hits foes lying down") },
		{ TEXT("pummel"), 0.30f, 0.12f, 0.12f, 6, 0, 0, 0, false, TEXT("the carried foe; 3 a grab") },
		{ TEXT("throwback"), 0.40f, 0.14f, 0.14f, 0, 0, 0, 0, true, TEXT("release: the body flies behind him") },
		{ TEXT("slam"), 0.50f, 0.20f, 0.20f, 20, 84, 84, 34, true, TEXT("shock 8 to the standing near the impact") },
		{ TEXT("wpnair"), 0.30f, 0.08f, 0.22f, 0, 0, 10, 26, true, TEXT("weapon damage x 1.2, reach - 10") },
		{ TEXT("wpndash"), 0.40f, 0.10f, 0.22f, 0, 0, 10, 26, true, TEXT("weapon damage x 1.1, reach + 20") },
		{ TEXT("airtoss"), 0.24f, 0.08f, 0.08f, 0, 0, 0, 0, true, TEXT("release") },
	};
	const FMoveDef* MoveDef(FName N) { for (const FMoveDef& D : GMoveDefs) if (N == D.Name) return &D; return nullptr; }

#if !UE_BUILD_SHIPPING
	AFTBrawlGameMode* MoveMode(UWorld* World) { return World ? World->GetAuthGameMode<AFTBrawlGameMode>() : nullptr; }
	FAutoConsoleCommandWithWorldAndArgs GFTBrawlMove(TEXT("ftb.Move"), TEXT("Development: ftb.Move <name> [power] starts one of Nova's newer moves now; ftb.Move list prints them (FTBrawlMoves.cpp)"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World) { if (AFTBrawlGameMode* G = MoveMode(World)) G->MoveCommand(Args); }));
	FAutoConsoleCommandWithWorldAndArgs GFTBrawlSeq(TEXT("ftb.Seq"), TEXT("Development: ftb.Seq <step> ... plays button presses in fight time; a step is controls joined by + (fire kick jump grab left right up down dashl dashr sideu sided wait) with an optional :seconds (default 0.05)"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World) { if (AFTBrawlGameMode* G = MoveMode(World)) G->SeqCommand(Args); }));
	FAutoConsoleCommandWithWorldAndArgs GFTBrawlMoveFixture(TEXT("ftb.MoveFixture"), TEXT("Development: ftb.MoveFixture <front|back|crowd|down|far|attacker|none> [px] clears the room and stands quiet foes round Nova"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World) { if (AFTBrawlGameMode* G = MoveMode(World)) G->MoveFixture(Args); }));
#endif
}
using namespace FTBrawlMoves;

bool AFTBrawlGameMode::IsNewMove(FName St)
{
	return MoveDef(St) != nullptr;
}

void AFTBrawlGameMode::MovesReset()
{
	EvadeT = BreakLookT = FireHeldT = AirT = MovePower = CarryT = 0.f;
	AirJabs = Pummels = 0;
	bMoveHit = bMoveDone = bChargeFull = bEvadeSaid = false;
	PendingMove = NAME_None; PendingPower = 0.f; PendingT = 0.f;
	AutoHoldFire = 0.f;
	SeqSteps.Reset(); SeqAt = 0; SeqT = 0.f;
	TraceSt = NAME_None; TraceT = 0.f;
}

// ---------------------------------------------------------------- input
void AFTBrawlGameMode::MovesReadTaps(FIn& In)
{
	const double Now = FPlatformTime::Seconds();
	const int32 Sd = In.Steer > 0.6f ? 1 : In.Steer < -0.6f ? -1 : 0;
	if (Sd != 0 && !bTapSteerDown)
	{
		if (Sd == TapSteerDir && Now - TapSteerAt < 0.26) { In.Dash = Sd; TapSteerAt = -9; }
		else { TapSteerDir = Sd; TapSteerAt = Now; }
	}
	bTapSteerDown = Sd != 0;
	const int32 Td = In.Throttle > 0.6f ? 1 : In.Throttle < -0.6f ? -1 : 0;
	if (Td != 0 && !bTapThrottleDown)
	{
		if (Td == TapThrottleDir && Now - TapThrottleAt < 0.26) { In.Side = Td; TapThrottleAt = -9; }
		else { TapThrottleDir = Td; TapThrottleAt = Now; }
	}
	bTapThrottleDown = Td != 0;
	if (bBlockCombatInput) In.Dash = In.Side = 0;
}

float AFTBrawlGameMode::MoveMult(bool bFists) const
{
	return (PowerT > 0 ? (bFists ? 2.f : 1.5f) : 1.f) * (BuzzT > 0 ? 1.25f : 1.f);
}

// ---------------------------------------------------------------- hits
// the range test of the older moves, for the newer ones. HitSfx null = the held weapon's own impact sound and one use of it.
int32 AFTBrawlGameMode::MoveStrike(float Dmg, float Front, float Back, float Depth, bool bDown, bool bFists, const TCHAR* HitSfx, float SfxPitch)
{
	Dmg *= MoveMult(bFists);
	StageStrike(Dmg, Front, Back, Depth, bDown);   // the same box can break scenery at the back of the lane (FTBrawlStage.cpp)
	int32 N = 0;
	for (FFTBrawlBody& F : Foes)
	{
		if (!Standing(F) || P.HitIds.Contains(F.Id)) continue;
		const float Dx = (F.X - P.X) * P.Dir;
		if (Dx > -Back && Dx < Front + (F.bBoss ? 20.f : 0.f) && FMath::Abs(F.Y - P.Y) < Depth + (F.bBoss ? 8.f : 0.f))
		{
			if (!bMoveHit) { if (HitSfx) Sfx(HitSfx, 0.9f, SfxPitch); else if (Held >= 0) ImpactSfx(Held); }
			P.HitIds.Add(F.Id); HitFoe(F, Dmg, bDown, false);
			if (!HitSfx && Held >= 0)
			{
				if (HeldGlass()) Bleed(F.X, F.Y, 140, P.Dir, 6, 0.6f, true);
				if (!bMoveHit) HeldUsed(F);
			}
			bMoveHit = true; N++;
		}
	}
	return N;
}

// one more hit on a foe who is in the air: he pops up again. Up to three a flight, each worth less.
bool AFTBrawlGameMode::JuggleOne(FFTBrawlBody& F, float Dmg, float PopUp, float Push)
{
	if (F.St != TEXT("fly") || F.bBoss || F.Hp <= 0.f || F.Z < 20.f || P.HitIds.Contains(F.Id)) return false;   // a beaten man in the air is left alone
	if (Time - F.JugAt > 1.2f) F.Jug = 0;
	if (F.Jug >= 3) return false;
	P.HitIds.Add(F.Id);
	F.Jug++; F.JugAt = Time;
	const float Scale = FMath::Max(0.4f, 1.f - 0.2f * (F.Jug - 1));
	F.Hp -= Dmg * Scale; F.Flash = 0.12f;
	F.T = 0.06f; F.Vz = PopUp - 40.f * (F.Jug - 1); F.Vx = P.Dir * Push; F.Dir = -P.Dir;
	ContactPause = FMath::Max(ContactPause, 2.f * StepDt);
	CombatCue(ECombatCue::Jab, World(F.X - P.Dir * 12.f, F.Y + 9.f, F.Z + 90.f), P.Dir, false);
	Shake = FMath::Min(1.f, Shake + 0.25f);
	Rumble(0.3f, 0.09f, TEXT("juggle"));
	Chain++; if (Chain > Best) Best = Chain;
	Bleed(F.X + P.Dir * 6, F.Y, F.Z + 100.f, P.Dir, 10, 0.9f);
	Pops.RemoveAll([](const FFTBrawlPop& Q) { return Q.Text.StartsWith(TEXT("AIR x")); });   // one counter, not a pile of them
	if (F.Jug >= 2) Pop(F.X, F.Y, F.Z + 150.f, FString::Printf(TEXT("AIR x%d"), F.Jug), FLinearColor(1.f, 0.82f, 0.25f));
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL juggle %s by=%s n=%d dmg=%.1f hp=%.0f z=%.0f"), *F.K.Role.ToString(), *P.St.ToString(), F.Jug, Dmg * Scale, F.Hp, F.Z);
	return true;
}

// the older attacks (punch, kick, jump kick, weapon swing), inside their own active window, also reach a foe in the air
void AFTBrawlGameMode::MovesJuggle(float Dmg, float Reach)
{
	const bool bKick = P.St == TEXT("kick") || P.St == TEXT("jumpkick");
	for (FFTBrawlBody& F : Foes)
	{
		if (F.St != TEXT("fly")) continue;
		const float Dx = (F.X - P.X) * P.Dir;
		if (Dx > -10 && Dx < Reach && FMath::Abs(F.Y - P.Y) < 26 && FMath::Abs(F.Z - P.Z) < 150.f)
			if (JuggleOne(F, Dmg, bKick ? 260.f : 300.f, bKick ? 240.f : 60.f))
				Sfx(bKick ? TEXT("kick") : TEXT("punch"), 0.8f, 1.1f);
	}
}

bool AFTBrawlGameMode::MovesEvade(const FFTBrawlBody& From)
{
	if (EvadeT <= 0.f) return false;
	if (From.St == TEXT("slam") || From.St == TEXT("sweep")) return false;   // the floor moves: only a jump clears these
	if (!bEvadeSaid)
	{
		bEvadeSaid = true;
		Pop(P.X, P.Y, 175, TEXT("DODGE!"), FLinearColor(0.21f, 0.95f, 1.f));
		Sfx(TEXT("whoosh"), 0.5f, 1.6f);
		UE_LOG(LogTemp, Display, TEXT("FTBRAWL dodge evaded %s in=%s t=%.3f"), From.K.Role.IsNone() ? TEXT("shot") : *From.K.Role.ToString(), *P.St.ToString(), P.T);
	}
	return true;
}

// ---------------------------------------------------------------- starting a move
bool AFTBrawlGameMode::MoveStart(FName Move, const FIn& In)
{
	static const FName NSide(TEXT("sidestep")), NDash(TEXT("dashfwd")), NDashAtk(TEXT("dashatk")), NRunAtk(TEXT("runatk")), NAirJab(TEXT("airjab")),
		NDive(TEXT("dive")), NLaunch(TEXT("launch")), NCharging(TEXT("charging")), NHeavy(TEXT("heavy")), NSpecial(TEXT("special")), NBack(TEXT("backatk")),
		NStomp(TEXT("stomp")), NPummel(TEXT("pummel")), NThrowBack(TEXT("throwback")), NSlam(TEXT("slam")), NWpnAir(TEXT("wpnair")), NWpnDash(TEXT("wpndash")), NAirToss(TEXT("airtoss"));
	if (!MoveDef(Move)) return false;
	const FName Was = P.St;
	P.St = Move; P.T = 0; P.HitIds.Reset(); bMoveHit = bMoveDone = false;
	if (Move == NSide)
	{
		const int32 Dir = Buffered.SideDir != 0 ? Buffered.SideDir : (In.Throttle < 0 ? -1 : 1);
		P.JVy = -Dir * 433.f; P.JVx = 0; EvadeT = 0.20f; bEvadeSaid = false; P.Combo = 0;
		Accent(EAccent::Dust, World(P.X, P.Y + 7.f, 9.f), 52.f, P.Dir, false);
		Sfx(TEXT("whoosh"), 0.4f, 1.5f);
	}
	else if (Move == NDash)
	{
		if (Buffered.DashDir != 0) P.Dir = Buffered.DashDir > 0 ? 1.f : -1.f;
		P.JVx = P.Dir * 620.f; P.JVy = 0; P.Combo = 0;
		Accent(EAccent::Dust, World(P.X - P.Dir * 16.f, P.Y + 7.f, 9.f), 60.f, P.Dir, false);
		Sfx(TEXT("whoosh"), 0.45f, 1.3f);
	}
	else if (Move == NDashAtk) Sfx(TEXT("whoosh"), 0.55f, 0.95f);
	else if (Move == NRunAtk) Sfx(TEXT("whooshbig"), 0.6f, 1.1f);
	else if (Move == NAirJab) { AirJabs++; Sfx(TEXT("whoosh"), 0.45f, 1.2f); }
	else if (Move == NDive) { P.Vz = -420.f; P.JVx = P.Dir * 440.f; P.JVy = 0; Sfx(TEXT("whooshbig"), 0.6f, 1.25f); }
	else if (Move == NLaunch) { P.Combo = 0; Sfx(TEXT("whoosh"), 0.6f, 0.7f); }
	else if (Move == NCharging) { bChargeFull = false; P.Combo = 0; }
	else if (Move == NHeavy) Sfx(TEXT("whooshbig"), 0.8f, 0.8f);
	else if (Move == NSpecial)
	{
		const float Cost = FMath::Min(12.f, P.Hp - 1.f);
		P.Hp -= Cost; EvadeT = 0.46f; bEvadeSaid = true; P.Combo = 0;
		if (Was == TEXT("held")) P.Mash = 0;
		Pop(P.X, P.Y, 190, FString::Printf(TEXT("-%d"), FMath::RoundToInt32(Cost)), FLinearColor(1.f, 0.3f, 0.3f));
		Sfx(TEXT("whooshbig"), 0.9f, 0.9f);
		UE_LOG(LogTemp, Display, TEXT("FTBRAWL special cost=%.0f hp=%.0f from=%s"), Cost, P.Hp, *Was.ToString());
	}
	else if (Move == NBack) { P.Combo = 0; Sfx(TEXT("whoosh"), 0.5f, 1.15f); }
	else if (Move == NStomp) Sfx(TEXT("whoosh"), 0.5f, 0.7f);
	else if (Move == NPummel) Sfx(TEXT("whoosh"), 0.4f, 1.1f);
	else if (Move == NThrowBack || Move == NSlam) Sfx(TEXT("whooshbig"), 0.8f, Move == NSlam ? 0.6f : 0.75f);
	else if (Move == NWpnAir || Move == NWpnDash) Sfx(TEXT("whooshbig"), 0.7f, HeldReach() > 140 ? 0.85f : 1.05f);
	else if (Move == NAirToss) Sfx(TEXT("whooshbig"), 0.6f, 1.2f);
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL move %s from=%s x=%.0f y=%.0f z=%.0f hp=%.0f"), *Move.ToString(), *Was.ToString(), P.X, P.Y, P.Z, P.Hp);
	return true;
}

// ---------------------------------------------------------------- every step
void AFTBrawlGameMode::MovesTick(float Dt, const FIn& In)
{
	if (EvadeT > 0.f) EvadeT -= Dt;
	if (BreakLookT > 0.f) BreakLookT -= Dt;
	Buffered.Dash = FMath::Max(0.f, Buffered.Dash - Dt);
	Buffered.Side = FMath::Max(0.f, Buffered.Side - Dt);
	FireHeldT = In.Fire ? FireHeldT + Dt : 0.f;
	if (P.Z <= 0 && P.Vz <= 0) { AirT = 0.f; AirJabs = 0; }
	if (P.St == TEXT("lift")) Pummels = 0;
#if !UE_BUILD_SHIPPING
	if (CVarMoveTrace.GetValueOnGameThread() && P.St != TraceSt)
	{
		UE_LOG(LogTemp, Display, TEXT("FTBRAWL novastate %s t=%.4f was=%s for=%.4f x=%.1f y=%.1f z=%.1f hp=%.1f combo=%d"), *P.St.ToString(), StT, *TraceSt.ToString(), StT - TraceT, P.X, P.Y, P.Z, P.Hp, P.Combo);
		TraceSt = P.St; TraceT = StT;
	}
	// ftb.Move: a move that had to jump, grab or wait first
	if (!PendingMove.IsNone())
	{
		PendingT += Dt;
		const bool bAir = PendingMove == TEXT("airjab") || PendingMove == TEXT("dive") || PendingMove == TEXT("wpnair") || PendingMove == TEXT("airtoss");
		const bool bCarry = PendingMove == TEXT("pummel") || PendingMove == TEXT("throwback") || PendingMove == TEXT("slam");
		bool bGo = false;
		if (bAir) bGo = P.St == TEXT("jump") && AirT >= (PendingMove == TEXT("dive") ? 0.30f : 0.16f);
		else if (bCarry) bGo = P.St == TEXT("carry") && P.T >= 0.20f && GrabId != -1;
		else bGo = P.St == TEXT("idle") || P.St == TEXT("walk");
		if (bGo)
		{
			const FName M = PendingMove; PendingMove = NAME_None;
			if (bCarry) CarryT = P.T;
			MovePower = PendingPower;
			MoveStart(M, In);
		}
		else if (PendingT > 2.f) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL move FAIL %s could not start (state %s)"), *PendingMove.ToString(), *P.St.ToString()); PendingMove = NAME_None; }
	}
#endif
}

// ---------------------------------------------------------------- in the air
bool AFTBrawlGameMode::MovesAir(float Dt, const FIn& In)
{
	AirT += Dt;
	const FName S = P.St;
	auto AirHits = [this](float Dmg, float Front, float PopUp, float Push, bool bDown, const TCHAR* Sound, float Pitch) -> int32
	{
		int32 N = 0;
		for (FFTBrawlBody& F : Foes)
		{
			if (F.St != TEXT("fly")) continue;
			const float Dx = (F.X - P.X) * P.Dir;
			if (Dx > -10 && Dx < Front && FMath::Abs(F.Y - P.Y) < 26 && FMath::Abs(F.Z - P.Z) < 110.f && JuggleOne(F, Dmg * MoveMult(true), PopUp, Push))
			{
				if (!bMoveHit) Sfx(Sound, 0.85f, Pitch);
				bMoveHit = true; N++;
			}
		}
		if (P.Z < 120.f) N += MoveStrike(Dmg, Front, 10, 26, bDown, true, Sound, Pitch);   // and the ones on their feet, when he is low enough
		return N;
	};
	if (S == TEXT("airjab"))
	{
		if (P.T >= 0.04f && P.T < 0.12f)
		{
			if (P.T - Dt < 0.04f) Accent(EAccent::Slash, World(P.X + P.Dir * 36.f, P.Y + 9.f, P.Z + 120.f), 62.f, P.Dir, false);
			if (AirHits(6, 84, 320, 30, false, TEXT("punch"), 1.15f) > 0) P.Vz = FMath::Max(P.Vz, 150.f);   // a hit holds him up a moment: room for the next
		}
		if (P.T >= 0.20f) { P.St = TEXT("jump"); P.T = AirT; }
		return true;
	}
	if (S == TEXT("wpnair"))
	{
		if (Held >= 0 && P.T >= 0.08f && P.T < 0.22f)
		{
			if (P.T - Dt < 0.08f) Accent(EAccent::Slash, World(P.X + P.Dir * 44.f, P.Y + 9.f, P.Z + 95.f), 120.f, P.Dir, false);
			const float Reach = FMath::Max(80.f, HeldReach() - 10.f), Dmg = HeldDmg() * 1.2f;
			for (FFTBrawlBody& F : Foes)
			{
				if (F.St != TEXT("fly")) continue;
				const float Dx = (F.X - P.X) * P.Dir;
				if (Dx > -10 && Dx < Reach && FMath::Abs(F.Y - P.Y) < 26 && FMath::Abs(F.Z - P.Z) < 120.f && JuggleOne(F, Dmg * MoveMult(false), 180.f, 260.f))
				{
					if (!bMoveHit) { ImpactSfx(Held); HeldUsed(F); }
					bMoveHit = true;
				}
			}
			if (Held >= 0 && P.Z < 130.f) MoveStrike(Dmg, Reach, 10, 26, true, false, nullptr);
		}
		if (P.T >= 0.30f) { P.St = TEXT("jump"); P.T = AirT; }
		return true;
	}
	if (S == TEXT("airtoss"))
	{
		if (P.T >= 0.08f && Held >= 0) ThrowHeld(FMath::Max(40.f, P.Z + 70.f));
		if (P.T >= 0.24f) { P.St = TEXT("jump"); P.T = AirT; }
		return true;
	}
	if (S == TEXT("dive"))
	{
		if (P.T >= 0.05f && AirHits(14, 78, 200, 280, true, TEXT("kick"), 0.85f) > 0)
		{
			P.Vz = 300.f; P.JVx = -P.Dir * 40.f; P.St = TEXT("jump"); P.T = 0.43f;   // he bounces off the hit and falls
		}
		return true;
	}
	if (S == TEXT("jump"))
	{
		const bool F = Buffered.Fire > 0.f, K = Buffered.Kick > 0.f;
		if (F && K) return false;
		if (K && In.Throttle < -0.5f && P.Z > 36.f) { Buffered.Kick = 0; return MoveStart(TEXT("dive"), In); }
		if (K && Held >= 0) { Buffered.Kick = 0; return MoveStart(TEXT("airtoss"), In); }
		if (F && Held >= 0) { Buffered.Fire = 0; return MoveStart(TEXT("wpnair"), In); }
		if (F && AirJabs < 2) { Buffered.Fire = 0; return MoveStart(TEXT("airjab"), In); }
	}
	return false;
}

// ---------------------------------------------------------------- on the ground: the new states
bool AFTBrawlGameMode::MovesState(float Dt, const FIn& In)
{
	const FName S = P.St;
	if (S == TEXT("held"))
	{
		// the special breaks a hold: both buttons held down together for two steps. Mashing one after the other (the way
		// out the HUD teaches) no longer spends 12 health by accident.
		HeldBoth = (In.Fire && In.Kick) ? HeldBoth + 1 : 0;
		if (HeldBoth >= 2 && P.Hp > 1.f) { HeldBoth = 0; Buffered = {}; return MoveStart(TEXT("special"), In); }
		return false;
	}
	if (!IsNewMove(S)) return false;
	auto End = [this](const TCHAR* To = TEXT("idle")) { P.St = To; P.T = 0; };
	auto Crossed = [this, Dt](float At) { return P.T >= At && P.T - Dt < At; };
	auto StartJump = [this](float Vx) { Buffered.Jump = 0; P.St = TEXT("jump"); P.T = 0; P.Vz = 600; P.Z = 0.01f; P.JVx = Vx; P.JVy = 0; P.HitIds.Reset(); };
	if (S == TEXT("airjab") || S == TEXT("dive") || S == TEXT("wpnair") || S == TEXT("airtoss")) { End(); return true; }   // an air move on the floor: over
	if (S == TEXT("sidestep"))
	{
		if (P.T < 0.18f) P.Y += P.JVy * Dt;
		if (P.T > 0.30f || (P.T >= 0.22f && (Buffered.Fire > 0 || Buffered.Kick > 0 || Buffered.Jump > 0 || Buffered.Grab > 0))) End();
	}
	else if (S == TEXT("dashfwd"))
	{
		const float V = P.T < 0.18f ? 620.f : 620.f * FMath::Max(0.f, 1.f - (P.T - 0.18f) / 0.08f);
		P.X += P.Dir * V * Dt;
		if (Crossed(0.12f)) Accent(EAccent::Dust, World(P.X - P.Dir * 16.f, P.Y + 7.f, 9.f), 46.f, P.Dir, false);
		if (P.T >= 0.04f && Buffered.Fire > 0.f) { Buffered.Fire = 0; MoveStart(Held >= 0 ? TEXT("wpndash") : TEXT("dashatk"), In); }
		else if (P.T >= 0.04f && Buffered.Kick > 0.f && Held < 0) { Buffered.Kick = 0; MoveStart(TEXT("runatk"), In); }
		else if (P.T >= 0.04f && Buffered.Jump > 0.f) StartJump(P.Dir * 420.f);
		else if (P.T > 0.26f)
		{
			const bool bOn = In.Steer * P.Dir > 0.5f;
			End(bOn ? TEXT("walk") : TEXT("idle"));
			if (bOn) SustainedTravel = 0.24f;   // still holding the way he dashed: he runs on
		}
	}
	else if (S == TEXT("dashatk"))
	{
		if (P.T < 0.19f) P.X += P.Dir * 420.f * (1.f - P.T / 0.19f) * Dt;
		if (Crossed(0.07f)) Accent(EAccent::Slash, World(P.X + P.Dir * 41.f, P.Y + 9.f, 100.f), 96.f, P.Dir, false);
		if (P.T >= 0.07f && P.T < 0.19f) MoveStrike(11, 86, 10, 26, false, true, TEXT("punch2"), 0.9f);
		if (P.T > 0.36f) End();
	}
	else if (S == TEXT("runatk"))
	{
		if (P.T < 0.34f) P.X += P.Dir * 380.f * (1.f - P.T / 0.34f) * Dt;
		if (Crossed(0.08f)) Accent(EAccent::Slash, World(P.X + P.Dir * 50.f, P.Y + 9.f, 40.f), 110.f, P.Dir, false);
		if (Crossed(0.10f) || Crossed(0.20f)) Accent(EAccent::Dust, World(P.X - P.Dir * 10.f, P.Y + 7.f, 9.f), 50.f, P.Dir, false);
		if (P.T >= 0.08f && P.T < 0.30f) MoveStrike(12, 96, 10, 26, true, true, TEXT("kick"), 0.95f);
		if (P.T > 0.46f) End();
	}
	else if (S == TEXT("launch"))
	{
		if (Crossed(0.10f)) Accent(EAccent::Slash, World(P.X + P.Dir * 38.f, P.Y + 9.f, 110.f), 100.f, P.Dir, false);
		if (P.T >= 0.10f && P.T < 0.20f && MoveStrike(9, 78, 10, 26, true, true, TEXT("punchbig"), 1.15f) > 0)
			for (FFTBrawlBody& F : Foes)
				if (F.St == TEXT("fly") && F.T == 0.f && !F.bBoss && P.HitIds.Contains(F.Id))
				{
					F.Vx = P.Dir * 10.f; F.Vz = 560.f; F.Jug = 0; F.JugAt = Time;   // straight up: there to be followed
					UE_LOG(LogTemp, Display, TEXT("FTBRAWL launch %s hp=%.0f"), *F.K.Role.ToString(), F.Hp);
				}
		if (bMoveHit && P.T >= 0.16f && Buffered.Jump > 0.f) StartJump(In.Steer != 0 ? In.Steer * 320.f : P.Dir * 70.f);   // no direction held: he drifts in under the foe
		else if (P.T > 0.44f) End();
	}
	else if (S == TEXT("charging"))
	{
		if (In.Steer != 0) P.Dir = In.Steer > 0 ? 1.f : -1.f;
		if (!bChargeFull && P.T >= 0.80f)
		{
			bChargeFull = true;
			Sfx(TEXT("powerup"), 0.5f, 1.35f);
			Accent(EAccent::Pickup, World(P.X - P.Dir * 10.f, P.Y + 8.f, 95.f), 70.f, P.Dir, false);
			UE_LOG(LogTemp, Display, TEXT("FTBRAWL charge full"));
		}
		if (Buffered.Kick > 0 || Buffered.Jump > 0 || Buffered.Grab > 0 || Buffered.Side > 0 || Buffered.Dash > 0) End();   // any other button: he drops the charge
		else if (!In.Fire || P.T > 1.40f)
		{
			if (P.T >= 0.25f) { MovePower = FMath::Clamp((P.T - 0.25f) / 0.55f, 0.f, 1.f); MoveStart(TEXT("heavy"), In); }
			else End();
		}
	}
	else if (S == TEXT("heavy"))
	{
		if (P.T < 0.12f) P.X += P.Dir * 420.f * Dt;
		if (Crossed(0.12f)) Accent(EAccent::Slash, World(P.X + P.Dir * 46.f, P.Y + 9.f, 105.f), 120.f + 40.f * MovePower, P.Dir, false);
		if (P.T >= 0.12f && P.T < 0.22f && MoveStrike(16.f + 14.f * MovePower, 88.f + 14.f * MovePower, 10, 26, true, true, TEXT("punchbig"), 0.8f) > 0)
		{
			for (FFTBrawlBody& F : Foes)
				if (F.St == TEXT("fly") && F.T == 0.f && !F.bBoss && P.HitIds.Contains(F.Id)) { F.Vx = P.Dir * (300.f + 180.f * MovePower); F.Vz = 340.f + 60.f * MovePower; }
			ContactPause = FMath::Max(ContactPause, (3.f + 2.f * MovePower) * StepDt);
			Shake = FMath::Min(1.f, Shake + 0.25f * MovePower);
			UE_LOG(LogTemp, Display, TEXT("FTBRAWL heavy hit power=%.2f dmg=%.1f"), MovePower, (16.f + 14.f * MovePower) * MoveMult(true));
		}
		if (P.T > 0.52f) End();
	}
	else if (S == TEXT("special"))
	{
		if (Crossed(0.10f)) Accent(EAccent::Slash, World(P.X + P.Dir * 50.f, P.Y + 9.f, 95.f), 150.f, P.Dir, false);
		if (Crossed(0.24f)) Accent(EAccent::Slash, World(P.X - P.Dir * 50.f, P.Y + 9.f, 60.f), 150.f, -P.Dir, false);
		if (P.T >= 0.10f && P.T < 0.38f)
		{
			const float Face = P.Dir;
			for (FFTBrawlBody& F : Foes)
			{
				if (!Standing(F) || P.HitIds.Contains(F.Id)) continue;
				const float Dx = F.X - P.X;
				if (FMath::Abs(Dx) < 112.f + (F.bBoss ? 20.f : 0.f) && FMath::Abs(F.Y - P.Y) < 44.f + (F.bBoss ? 8.f : 0.f))
				{
					if (!bMoveHit) Sfx(TEXT("punchbig"), 0.95f, 0.9f);
					bMoveHit = true;
					P.HitIds.Add(F.Id);
					P.Dir = Dx >= 0 ? 1.f : -1.f;   // each goes flying away from him
					HitFoe(F, 14.f * MoveMult(true), true, false);
					P.Dir = Face;
				}
			}
		}
		if (P.T > 0.62f) { UE_LOG(LogTemp, Display, TEXT("FTBRAWL special done hits=%d"), P.HitIds.Num()); End(); }
	}
	else if (S == TEXT("backatk"))
	{
		if (Crossed(0.06f)) Accent(EAccent::Slash, World(P.X - P.Dir * 38.f, P.Y + 9.f, 105.f), 80.f, -P.Dir, false);
		if (P.T >= 0.06f && P.T < 0.14f)
		{
			const float Face = P.Dir;
			P.Dir = -Face;
			MoveStrike(10, 80, 6, 26, false, true, TEXT("punch2"), 1.05f);
			P.Dir = Face;
		}
		if (P.T > 0.30f) End();
	}
	else if (S == TEXT("stomp"))
	{
		if (P.T < 0.16f)   // the lift of the boot: a short step to the man on the floor, so the stamp lands on him
		{
			float Near = 1e9f;
			for (const FFTBrawlBody& F : Foes)
			{
				const float Dx = (F.X - P.X) * P.Dir;
				if (F.St == TEXT("down") && F.Hp > 0 && Dx > -30 && Dx < 150 && FMath::Abs(F.Y - P.Y) < 28 && Dx < Near) Near = Dx;
			}
			if (Near < 1e8f && Near > 34.f) P.X += P.Dir * FMath::Min(Near - 34.f, 560.f * Dt);
		}
		if (P.T >= 0.16f && !bMoveDone)
		{
			bMoveDone = true;
			int32 N = 0;
			for (FFTBrawlBody& F : Foes)
			{
				if (F.St != TEXT("down") || F.Hp <= 0) continue;
				const float Dx = (F.X - P.X) * P.Dir;
				if (Dx > -30 && Dx < 90 && FMath::Abs(F.Y - P.Y) < 28)
				{
					const float Dmg = 12.f * MoveMult(true);
					// the first stamp of a knock-down keeps him on the floor from the start again (0.75 s); later ones only hurt,
					// so he cannot be held down for ever. (Jug / JugAt double as that marker: they clear 1.2 s after the last.)
					const bool bFirst = Time - F.StompAt > 1.2f;
					F.Hp -= Dmg; F.Flash = 0.12f; F.StompAt = Time;
					if (bFirst || F.Hp <= 0) F.T = 0;
					CombatCue(ECombatCue::Finish, World(F.X, F.Y + 9.f, 24.f), P.Dir, false);
					Bleed(F.X, F.Y, 20, P.Dir, 16, 1.0f);
					Chain++; if (Chain > Best) Best = Chain;
					N++;
					UE_LOG(LogTemp, Display, TEXT("FTBRAWL stomp %s dmg=%.0f hp=%.0f"), *F.K.Role.ToString(), Dmg, F.Hp);
				}
			}
			Accent(EAccent::Land, World(P.X + P.Dir * 30.f, P.Y + 7.f, 10.f), N ? 110.f : 70.f, P.Dir, false);
			if (N)
			{
				Sfx(TEXT("kick"), 0.95f, 0.7f);
				FeelStop(6.f, 3.f);   // hit-stop by strength (FTBrawlFeel.cpp): a stamp is a heavy hit
				FeelImpact(World(P.X + P.Dir * 30.f, P.Y + 11.f, 22.f), P.Dir, 1, FLinearColor(1.f, 0.8f, 0.42f));
				Shake = FMath::Min(1.f, Shake + 0.4f);
				Rumble(0.5f, 0.14f, TEXT("stomp"));
			}
		}
		if (P.T > 0.46f) End();
	}
	else if (S == TEXT("pummel"))
	{
		FFTBrawlBody* G = Foes.FindByPredicate([this](const FFTBrawlBody& F) { return F.Id == GrabId; });
		if (G && P.T >= 0.12f && !bMoveDone)
		{
			bMoveDone = true; Pummels++;
			const float Dmg = 6.f * MoveMult(true);
			G->Hp -= Dmg; G->Flash = 0.12f;
			CombatCue(ECombatCue::Jab, World(G->X, G->Y + 9.f, G->Z + 30.f), P.Dir, false);
			Sfx(Pummels == 3 ? TEXT("punchbig") : TEXT("punch"), 0.85f, 0.9f + 0.06f * Pummels);
			ContactPause = FMath::Max(ContactPause, 2.f * StepDt);
			Shake = FMath::Min(1.f, Shake + 0.2f);
			Rumble(0.3f, 0.09f, TEXT("pummel"));
			Chain++; if (Chain > Best) Best = Chain;
			Bleed(G->X, G->Y, G->Z + 30.f, P.Dir, 10, 0.8f);
			UE_LOG(LogTemp, Display, TEXT("FTBRAWL pummel %s n=%d dmg=%.0f hp=%.0f"), *G->K.Role.ToString(), Pummels, Dmg, G->Hp);
			if (G->Hp <= 0) { G->St = TEXT("fly"); G->T = 0; G->Vx = P.Dir * 120.f; G->Vz = 200.f; G->Dir = -P.Dir; GrabId = -1; }   // he is done: he drops out of Nova's hands
		}
		if (P.T > 0.30f)
		{
			if (GrabId != -1) { P.St = TEXT("carry"); P.T = CarryT; }
			else End();
		}
	}
	else if (S == TEXT("throwback"))
	{
		FFTBrawlBody* G = Foes.FindByPredicate([this](const FFTBrawlBody& F) { return F.Id == GrabId; });
		if (G && P.T >= 0.14f)
		{
			GrabId = -1;
			G->St = TEXT("hurled"); G->T = 0; G->HitIds.Reset();
			G->X = P.X - P.Dir * 30.f; G->Y = P.Y; G->Z = 120.f; G->Vx = -P.Dir * 640.f; G->Vz = 300.f; G->Dir = -P.Dir;
			CombatCue(ECombatCue::Throw, World(G->X, G->Y + 8.f, G->Z), -P.Dir);
			Shake = FMath::Min(1.f, Shake + 0.3f);
			UE_LOG(LogTemp, Display, TEXT("FTBRAWL throwback %s dir=%.0f"), *G->K.Role.ToString(), -P.Dir);
		}
		if (P.T > 0.40f) End();
	}
	else if (S == TEXT("slam"))
	{
		FFTBrawlBody* G = Foes.FindByPredicate([this](const FFTBrawlBody& F) { return F.Id == GrabId; });
		if (G && P.T >= 0.20f)
		{
			GrabId = -1;
			const float Ix = P.X + P.Dir * 48.f;
			G->St = TEXT("down"); G->T = 0; G->Z = 0; G->Vx = G->Vz = 0; G->X = Ix; G->Y = P.Y + 2.f; G->Dir = -P.Dir;
			G->Hp -= 20.f * (BuzzT > 0 ? 1.25f : 1.f); G->Flash = 0.12f;
			int32 Shock = 0;
			const float Face = P.Dir;
			for (FFTBrawlBody& F : Foes)
			{
				if (F.Id == G->Id || !Standing(F)) continue;
				if (FMath::Abs(F.X - Ix) < 84.f + (F.bBoss ? 12.f : 0.f) && FMath::Abs(F.Y - P.Y) < 34.f)
				{
					P.Dir = F.X >= Ix ? 1.f : -1.f;
					HitFoe(F, 8.f * (BuzzT > 0 ? 1.25f : 1.f), true, false);
					P.Dir = Face; Shock++;
				}
			}
			CombatCue(ECombatCue::Finish, World(Ix, P.Y + 8.f, 20.f), P.Dir, false);
			Accent(EAccent::Land, World(Ix, P.Y + 8.f, 10.f), 150.f, P.Dir, false);
			Sfx(TEXT("down"), 1.f, 0.8f); Sfx(TEXT("punchbig"), 0.8f, 0.7f);
			FeelStop(8.f, 4.f);   // hit-stop by strength (FTBrawlFeel.cpp): the slam is the heaviest thing he does
			FeelImpact(World(Ix, P.Y + 11.f, 24.f), P.Dir, 2, FLinearColor(1.f, 0.8f, 0.42f));
			Shake = FMath::Min(1.f, Shake + 0.6f);
			Rumble(0.7f, 0.25f, TEXT("slam"));
			Bleed(Ix, P.Y, 30, P.Dir, 26, 1.2f);
			Chain++; if (Chain > Best) Best = Chain;
			UE_LOG(LogTemp, Display, TEXT("FTBRAWL slam %s hp=%.0f shock=%d"), *G->K.Role.ToString(), G->Hp, Shock);
		}
		if (P.T > 0.50f) End();
	}
	else if (S == TEXT("wpndash"))
	{
		if (P.T < 0.22f) P.X += P.Dir * 300.f * (1.f - P.T / 0.22f) * Dt;
		if (Crossed(0.10f)) Accent(EAccent::Slash, World(P.X + P.Dir * 48.f, P.Y + 9.f, 100.f), 120.f, P.Dir, false);
		if (Held >= 0 && P.T >= 0.10f && P.T < 0.22f) MoveStrike(HeldDmg() * 1.1f, HeldReach() + 20.f, 10, 26, true, false, nullptr);
		if (P.T > 0.40f) End();
	}
	else End();
	return true;
}

// ---------------------------------------------------------------- from idle / walk (or out of a punch)
void AFTBrawlGameMode::MovesNeutral(const FIn& In)
{
	if (P.Z > 0 || P.Vz > 0) return;
	const FName S = P.St;
	const bool bNeutral = S == TEXT("idle") || S == TEXT("walk");
	const bool F = Buffered.Fire > 0.f, K = Buffered.Kick > 0.f;
	// both attack buttons together = the special; a punch or kick that began up to 3 steps earlier turns into it
	if (P.Hp > 1.f && ((bNeutral && F && K) || (S == TEXT("punch") && P.T < 0.05f && K) || (S == TEXT("kick") && P.T < 0.05f && F)))
	{ Buffered.Fire = Buffered.Kick = 0; MoveStart(TEXT("special"), In); return; }
	// the launcher: kick out of the second punch, in the window the third punch would use
	if (S == TEXT("punch") && P.Combo == 2 && P.T > 0.12f && K && !F) { Buffered.Kick = 0; MoveStart(TEXT("launch"), In); return; }
	if (!bNeutral) return;
	if (Buffered.Side > 0.f) { Buffered.Side = 0; MoveStart(TEXT("sidestep"), In); return; }
	if (Buffered.Dash > 0.f) { Buffered.Dash = 0; MoveStart(TEXT("dashfwd"), In); return; }
	auto Ahead = [this](float Reach) { for (const FFTBrawlBody& B : Foes) { const float Dx = (B.X - P.X) * P.Dir; if (Standing(B) && Dx > -10 && Dx < Reach && FMath::Abs(B.Y - P.Y) < 26) return true; } return false; };
	auto Behind = [this](float Reach) { for (const FFTBrawlBody& B : Foes) { const float Dx = (B.X - P.X) * P.Dir; if (Standing(B) && Dx < -10 && Dx > -Reach && FMath::Abs(B.Y - P.Y) < 26) return true; } return false; };
	auto Downed = [this]() { for (const FFTBrawlBody& B : Foes) { const float Dx = (B.X - P.X) * P.Dir; if (B.St == TEXT("down") && B.Hp > 0 && Dx > -30 && Dx < 140 && FMath::Abs(B.Y - P.Y) < 28) return true; } return false; };
	const bool bRunning = SustainedTravel >= 0.24f;
	if (K && Held < 0)
	{
		if (Downed() && !Ahead(100.f)) { Buffered.Kick = 0; MoveStart(TEXT("stomp"), In); return; }   // before the slide: a slide passes over a man lying down
		if (bRunning) { Buffered.Kick = 0; MoveStart(TEXT("runatk"), In); return; }
	}
	if (F && ItemAt() == INDEX_NONE)
	{
		if (Held >= 0 && bRunning) { Buffered.Fire = 0; MoveStart(TEXT("wpndash"), In); return; }
		if (Held < 0 && Behind(80.f) && !Ahead(96.f)) { Buffered.Fire = 0; MoveStart(TEXT("backatk"), In); return; }
	}
	// the punch button held down with empty hands: he winds up the heavy
	if (In.Fire && !F && FireHeldT >= 0.30f && Held < 0 && ItemAt() == INDEX_NONE) MoveStart(TEXT("charging"), In);
}

// ---------------------------------------------------------------- carrying a foe
bool AFTBrawlGameMode::MovesCarry(const FIn& In)
{
	const bool F = Buffered.Fire > 0.f, K = Buffered.Kick > 0.f, G = Buffered.Grab > 0.f;
	if (!(F || K || G) || GrabId == -1) return false;
	if (In.Throttle < -0.5f) { CarryT = P.T; Buffered = {}; return MoveStart(TEXT("slam"), In); }
	if (In.Steer * P.Dir < -0.5f) { CarryT = P.T; Buffered = {}; return MoveStart(TEXT("throwback"), In); }
	if (F && !G && !K && Pummels < 3) { CarryT = P.T; Buffered = {}; return MoveStart(TEXT("pummel"), In); }
	return false;
}

bool AFTBrawlGameMode::MovesHoldGrab(FFTBrawlBody& G)
{
	const FName S = P.St;
	if (S == TEXT("pummel"))
	{
		// down onto the knee and back up over his head
		const float A = P.T < 0.10f ? P.T / 0.10f : P.T < 0.20f ? 1.f : FMath::Max(0.f, 1.f - (P.T - 0.20f) / 0.10f);
		G.X = P.X + P.Dir * FMath::Lerp(4.f, 30.f, A); G.Y = P.Y + 2; G.Z = FMath::Lerp(112.f, 52.f, A); G.Dir = P.Dir;
		return true;
	}
	if (S == TEXT("throwback"))
	{
		const float A = FMath::Clamp(P.T / 0.14f, 0.f, 1.f);
		G.X = P.X + P.Dir * FMath::Lerp(4.f, -26.f, A); G.Y = P.Y + 2; G.Z = 112.f + 10.f * A; G.Dir = P.Dir;
		return true;
	}
	if (S == TEXT("slam"))
	{
		const float A = FMath::Clamp(P.T / 0.20f, 0.f, 1.f);
		G.X = P.X + P.Dir * FMath::Lerp(4.f, 48.f, A); G.Y = P.Y + 2; G.Z = FMath::Lerp(112.f, 0.f, A * A); G.Dir = P.Dir;
		return true;
	}
	return false;
}

// ---------------------------------------------------------------- looks
void AFTBrawlGameMode::MovesStylePose(const FFTBrawlBody& F, FName& St, float& PoseTime) const
{
	if (BreakLookT > 0.f && (St == TEXT("idle") || St == TEXT("walk") || St == TEXT("run")))
	{
		St = TEXT("wpnbreak"); PoseTime = 0.24f - BreakLookT;
	}
}

void AFTBrawlGameMode::MovesTint(FLinearColor& Tint) const
{
	if (P.St == TEXT("charging") && P.T >= 0.25f)
	{
		const float Pw = FMath::Clamp((P.T - 0.25f) / 0.55f, 0.f, 1.f);
		const float Pulse = 0.5f + 0.5f * FMath::Sin(StT * (Pw >= 1.f ? 26.f : 12.f));
		Tint = FLinearColor::LerpUsingHSV(Tint, FLinearColor(1.5f, 1.25f, 0.6f), Pw * (0.45f + 0.35f * Pulse));
	}
	else if (P.St == TEXT("sidestep") && EvadeT > 0.f) Tint = FLinearColor(0.72f, 1.05f, 1.5f);   // the window in which nothing hits him
	else if (P.St == TEXT("special") && EvadeT > 0.f) Tint = FLinearColor(1.45f, 1.15f, 0.8f);
}

bool AFTBrawlGameMode::MovesWeaponAngle(float& Ang) const
{
	if (P.St == TEXT("wpnair")) { Ang = P.T < 0.08f ? 110.f : FMath::Lerp(110.f, -40.f, FMath::Clamp((P.T - 0.08f) / 0.08f, 0.f, 1.f)); return true; }
	if (P.St == TEXT("wpndash")) { Ang = P.T < 0.10f ? 110.f : FMath::Lerp(110.f, -5.f, FMath::Clamp((P.T - 0.10f) / 0.06f, 0.f, 1.f)); return true; }
	if (P.St == TEXT("airtoss")) { Ang = 110.f; return true; }
	return false;
}

// ---------------------------------------------------------------- the autopilot (ftb.AutoMoves 1)
bool AFTBrawlGameMode::MovesAuto(FIn& I, const FFTBrawlBody* Tgt)
{
	if (!CVarAutoMoves.GetValueOnGameThread() || bWon || P.Hp <= 0 || bAutoGrab) return false;
	const bool bPulse = int32(StT * 10) % 2 == 0;   // a button pressed and let go five times a second
	const int32 Plan = int32(StT / 1.5f) % 8;
	const FName S = P.St;
	if (S == TEXT("lift")) return true;
	if (S == TEXT("carry"))
	{
		if (P.T < 0.25f) return true;
		const int32 How = Plan % 3;
		if (How == 0) { I.Throttle = -1.f; I.Grab = bPulse; }                                   // slam
		else if (How == 1) { if (Pummels < 3) I.Fire = bPulse; else { I.Steer = -P.Dir; I.Grab = bPulse; } }   // three knees, then over the back
		else I.Grab = bPulse;                                                                   // the old throw
		return true;
	}
	if (S == TEXT("pummel") || S == TEXT("slam") || S == TEXT("throwback") || S == TEXT("special") || S == TEXT("stomp") || S == TEXT("heavy") || S == TEXT("backatk")) return true;
	if (S == TEXT("charging")) { I.Fire = P.T < 0.86f; return true; }
	if (S == TEXT("launch")) { I.Jump = bMoveHit && P.T >= 0.16f && bPulse; return true; }
	if (S == TEXT("jump") || S == TEXT("airjab"))
	{
		for (const FFTBrawlBody& F : Foes)
			if (F.St == TEXT("fly") && FMath::Abs(F.X - P.X) < 110 && FMath::Abs(F.Y - P.Y) < 26)
			{
				if (AirJabs < 2) I.Fire = bPulse; else I.Kick = bPulse;
				return true;
			}
		return false;
	}
	if (!Tgt || !(S == TEXT("idle") || S == TEXT("walk") || S == TEXT("punch") || S == TEXT("dashfwd"))) return false;
	const float Dx = Tgt->X - P.X, Dy = Tgt->Y - P.Y;
	if (S == TEXT("dashfwd")) { I.Steer = P.Dir; if (FMath::Abs(Dx) < 120) { if (Plan % 2) I.Kick = bPulse; else I.Fire = bPulse; } return true; }
	if (S == TEXT("punch")) { if (P.Combo == 2 && P.T > 0.12f && (Plan == 1 || Plan == 5)) { I.Kick = true; return true; } return false; }
	// a swing about to land on him: step out of it
	if (Plan % 2 == 0)
		for (const FFTBrawlBody& F : Foes)
			if (F.St == TEXT("wind") && F.T > F.K.Wind - 0.12f && FMath::Abs(F.X - P.X) < F.K.Reach + 14 && FMath::Abs(F.Y - P.Y) < 28)
			{ I.Side = P.Y > 0.5f * (FloorTop + FloorBottom) ? 1 : -1; return true; }
	// three or more round him: the special
	{
		int32 Near = 0;
		for (const FFTBrawlBody& F : Foes) if (Standing(F) && FMath::Abs(F.X - P.X) < 105 && FMath::Abs(F.Y - P.Y) < 40) Near++;
		if (Near >= 3 && P.Hp > 45) { I.Fire = I.Kick = bPulse; return true; }
	}
	// one lying near with nobody on his feet close by: go and stamp on him
	if (Plan == 3 || Plan == 7)
		for (const FFTBrawlBody& F : Foes)
		{
			if (F.St != TEXT("down") || F.Hp <= 0 || F.T > 0.4f || FMath::Abs(F.X - P.X) > 150 || FMath::Abs(F.Y - P.Y) > 40 || Held >= 0) continue;
			if (FMath::Abs(Dx) < 110 && FMath::Abs(Dy) < 30) break;   // someone standing is too close for that
			const float Fx = F.X - P.X, Fy = F.Y - P.Y;
			if (FMath::Abs(Fy) > 12) I.Throttle = Fy > 0 ? -1.f : 1.f;
			if (FMath::Abs(Fx) > 50) I.Steer = Fx > 0 ? 1.f : -1.f; else if ((Fx > 0) != (P.Dir > 0)) I.Steer = Fx > 0 ? 0.3f : -0.3f;
			if (FMath::Abs(Fx) <= 60 && FMath::Abs(Fy) <= 20) I.Kick = bPulse;
			return true;
		}
	const bool bIn = FMath::Abs(Dx) < 72 && FMath::Abs(Dy) < 20 && (Dx > 0) == (P.Dir > 0);
	if (Plan == 4 && bIn && Held < 0 && !Tgt->bBoss && Standing(*Tgt)) { I.Grab = bPulse; return true; }
	if (Plan == 6 && Held < 0 && FMath::Abs(Dx) < 96 && FMath::Abs(Dy) < 20 && (Dx > 0) == (P.Dir > 0)) { I.Fire = true; return true; }   // held down: a jab, then the charge
	if ((Plan == 0 || Plan == 2) && S == TEXT("walk") && FMath::Abs(Dx) > 230 && FMath::Abs(Dy) < 30) { I.Dash = bPulse ? (Dx > 0 ? 1 : -1) : 0; I.Steer = Dx > 0 ? 1.f : -1.f; return true; }
	if (Plan % 2 == 1 && SustainedTravel >= 0.24f && Held < 0 && FMath::Abs(Dx) < 125 && FMath::Abs(Dy) < 20 && (Dx > 0) == (P.Dir > 0)) { I.Kick = bPulse; I.Steer = P.Dir; return true; }
	return false;
}

// ---------------------------------------------------------------- development commands
void AFTBrawlGameMode::MoveCommand(const TArray<FString>& Args)
{
#if !UE_BUILD_SHIPPING
	if (Args.Num() < 1) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL move FAIL usage: ftb.Move <name> [power 0..1] | ftb.Move list")); return; }
	if (Args[0] == TEXT("list"))
	{
		for (const FMoveDef& D : GMoveDefs)
			UE_LOG(LogTemp, Display, TEXT("FTBRAWL movedef name=%s len=%.2f active=%.2f-%.2f dmg=%.0f front=%.0f back=%.0f depth=%.0f down=%d note=%s"), D.Name, D.Len, D.From, D.To, D.Dmg, D.Front, D.Back, D.Depth, D.bDown ? 1 : 0, D.Note);
		return;
	}
	if (bDay || InCut() || SystemScreen || P.Hp <= 0) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL move FAIL needs a fight on screen")); return; }
	if (Args[0] == TEXT("give"))   // ftb.Move give [uses] [kind]: a weapon in his hand (a frying pan unless a kind number is given)
	{
		HeldGive(Args.Num() > 2 ? FCString::Atoi(*Args[2]) : -1);
		if (Args.Num() > 1 && FCString::Atoi(*Args[1]) > 0) HeldUses = FCString::Atoi(*Args[1]);
		return;
	}
	const FName M(*Args[0].ToLower());
	if (!MoveDef(M)) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL move FAIL unknown move %s (ftb.Move list)"), *Args[0]); return; }
	PendingPower = Args.Num() > 1 ? FMath::Clamp(FCString::Atof(*Args[1]), 0.f, 1.f) : 1.f;
	const bool bAir = M == TEXT("airjab") || M == TEXT("dive") || M == TEXT("wpnair") || M == TEXT("airtoss");
	const bool bCarry = M == TEXT("pummel") || M == TEXT("throwback") || M == TEXT("slam");
	const bool bWeapon = M == TEXT("wpnair") || M == TEXT("wpndash") || M == TEXT("airtoss");
	if (bWeapon && Held < 0) HeldGive(-1);
	if (!bWeapon && !bCarry && Held >= 0 && (M == TEXT("airjab"))) { Held = -1; HeldUses = 0; }
	const bool bGround = P.Z <= 0 && P.Vz <= 0 && (P.St == TEXT("idle") || P.St == TEXT("walk"));
	if (bAir && bGround) { P.St = TEXT("jump"); P.T = 0; P.Vz = 600; P.Z = 0.01f; P.JVx = 0; P.JVy = 0; P.HitIds.Reset(); }
	if (bCarry && GrabId == -1 && bGround && !TryGrab()) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL move FAIL %s needs a foe in reach to grab (ftb.MoveFixture front)"), *M.ToString()); return; }
	PendingMove = M; PendingT = 0.f;
#endif
}

void AFTBrawlGameMode::MoveFixture(const TArray<FString>& Args)
{
#if !UE_BUILD_SHIPPING
	if (bDay || InCut() || SystemScreen) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL movefixture FAIL needs a fight on screen")); return; }
	const FString Layout = Args.Num() ? Args[0].ToLower() : TEXT("front");
	UFTBrawlSpriteBank::TestStage(this);   // a role tonight's waves do not have may be stood here
	PressKeys(FIn());
	bAuto = bKeyTest = bAutoGrab = false;
	Rnd.Initialize(1986 + 23);
	ResetState(0, false);
	Cam = LockX = 0.f; P.X = 440.f; P.Y = 470.f; P.Dir = 1.f; P.Inv = 0.f;
	auto Stand = [this](int32 Id, const TCHAR* Kind, float Dx, float Dy) -> FFTBrawlBody&
	{
		Foes.Add(MakeFoe(Id, Kind, P.X + Dx, P.Y + Dy - 2.f));   // 2 px behind his lane: two sprites on exactly one lane flicker through each other
		FFTBrawlBody& F = Foes.Last();
		F.Cd = 60.f; F.K.Sp = 0.f; F.bIn = true; F.Dir = Dx > 0 ? -1.f : 1.f; F.St = TEXT("idle"); F.T = 0;
		F.Hp = F.Max = 200.f;   // he outlasts the move being looked at
		return F;
	};
	const float Dist = Args.Num() > 1 ? FMath::Clamp(FCString::Atof(*Args[1]), 10.f, 450.f) : 0.f;   // optional: how far away he stands
	if (Layout == TEXT("front")) Stand(9001, TEXT("rusher"), Dist > 0 ? Dist : 46.f, 0);
	else if (Layout == TEXT("back")) Stand(9001, TEXT("rusher"), -(Dist > 0 ? Dist : 50.f), 0);
	else if (Layout == TEXT("far")) Stand(9001, TEXT("rusher"), Dist > 0 ? Dist : 300.f, 0);
	else if (Layout == TEXT("down")) { FFTBrawlBody& F = Stand(9001, TEXT("grappler"), Dist > 0 ? Dist : 40.f, 0); F.St = TEXT("down"); F.T = 0; }   // he gets up after 0.75 s like anyone knocked down
	else if (Layout == TEXT("attacker")) { FFTBrawlBody& F = Stand(9001, TEXT("heavy"), Dist > 0 ? Dist : 60.f, 0); F.Cd = 0.5f; }   // he swings once his 0.5 s are up (wind-up 0.55 s)
	else if (Layout == TEXT("crowd")) { Stand(9001, TEXT("rusher"), 52, 0); Stand(9002, TEXT("thrower"), 92, -22); Stand(9003, TEXT("rusher"), -54, 6); Stand(9004, TEXT("grappler"), -90, 26); }
	bSpawned = true; BannerT = -1.f;
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL movefixture %s foes=%d nova x=%.0f y=%.0f"), *Layout, Foes.Num(), P.X, P.Y);
#endif
}

void AFTBrawlGameMode::SeqCommand(const TArray<FString>& Args)
{
#if !UE_BUILD_SHIPPING
	SeqSteps.Reset(); SeqAt = 0; SeqT = 0.f;
	for (const FString& Arg : Args)
	{
		FSeqStep Step; Step.Text = Arg;
		FString Controls = Arg, Seconds;
		if (Arg.Split(TEXT(":"), &Controls, &Seconds)) Step.Dur = FMath::Clamp(FCString::Atof(*Seconds), 0.f, 30.f);
		TArray<FString> Parts; Controls.ToLower().ParseIntoArray(Parts, TEXT("+"));
		for (const FString& C : Parts)
		{
			if (C == TEXT("fire") || C == TEXT("punch")) Step.In.Fire = true;
			else if (C == TEXT("kick")) Step.In.Kick = true;
			else if (C == TEXT("jump")) Step.In.Jump = true;
			else if (C == TEXT("grab")) Step.In.Grab = true;
			else if (C == TEXT("left")) Step.In.Steer = -1.f;
			else if (C == TEXT("right")) Step.In.Steer = 1.f;
			else if (C == TEXT("up")) Step.In.Throttle = 1.f;
			else if (C == TEXT("down")) Step.In.Throttle = -1.f;
			else if (C == TEXT("dashl")) Step.In.Dash = -1;
			else if (C == TEXT("dashr")) Step.In.Dash = 1;
			else if (C == TEXT("sideu")) Step.In.Side = 1;
			else if (C == TEXT("sided")) Step.In.Side = -1;
			else if (C != TEXT("wait") && C != TEXT("none")) UE_LOG(LogTemp, Error, TEXT("FTBRAWL seq FAIL unknown control %s in %s"), *C, *Arg);
		}
		SeqSteps.Add(Step);
	}
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL seq armed steps=%d t=%.4f"), SeqSteps.Num(), StT);
#endif
}

void AFTBrawlGameMode::MovesSeqInput(FIn& In, float Dt)
{
#if !UE_BUILD_SHIPPING
	if (!SeqSteps.IsValidIndex(SeqAt)) return;
	if (ContactPause > 0.f) { const FIn Hold = SeqSteps[SeqAt].In; In.Steer = Hold.Steer; In.Throttle = Hold.Throttle; In.Fire = Hold.Fire; In.Kick = Hold.Kick; In.Jump = Hold.Jump; In.Grab = Hold.Grab; return; }   // a hit-stop does not use up the step
	const FSeqStep& Step = SeqSteps[SeqAt];
	if (SeqT == 0.f) UE_LOG(LogTemp, Display, TEXT("FTBRAWL seq step=%d %s t=%.4f state=%s"), SeqAt, *Step.Text, StT, *P.St.ToString());
	In.Steer = Step.In.Steer; In.Throttle = Step.In.Throttle; In.Fire = Step.In.Fire; In.Kick = Step.In.Kick; In.Jump = Step.In.Jump; In.Grab = Step.In.Grab;
	In.Dash = Step.In.Dash; In.Side = Step.In.Side;
	SeqT += FMath::Min(Dt, 0.05f);
	if (SeqT >= Step.Dur - 1e-4f)
	{
		SeqAt++; SeqT = 0.f;
		if (!SeqSteps.IsValidIndex(SeqAt)) { SeqSteps.Reset(); SeqAt = 0; UE_LOG(LogTemp, Display, TEXT("FTBRAWL seq done t=%.4f state=%s"), StT, *P.St.ToString()); }
	}
#endif
}
