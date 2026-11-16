// The nights between the races (story flow ~/Wisper/docs/full-throttle-flow: L1 -> N1 -> L2 -> N2 -> L3 -> N3 ...).
// A night is the beat-em-up brawl (Source/FullThrottle/Brawl, Content/Brawl, merged from beat-em-up-unreal).
// The races are not touched: after a level is wrapped, ENTER on the PAYCHECK opens that night's map instead of
// PRE-PRODUCTION; when the night is over (its last card / K.O. + ENTER) the finished level's map opens again on
// PRE-PRODUCTION for the next level, exactly where the paycheck would have gone, and its READY loads the next race
// as before. A night whose map is not in the project is skipped.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FTNightFlow.generated.h"

UCLASS()
class FULLTHROTTLE_API UFTNightFlow : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	/** The 0-based level the running night hands back to; -1 = no career night running (the night map was opened by itself). */
	int32 NextLevel = -1;
	/** The level whose wrap opened the night: its map hosts PRE-PRODUCTION again afterwards, as without the night. */
	int32 FromLevel = -1;

	static UFTNightFlow* Get(const UObject* Ctx);
	/** The night that follows level L (0-based), e.g. 0 -> /Game/Brawl/Maps/L_RustyAnchor; empty if none is in the project. */
	static FString NightMapAfter(int32 L);
	/** From the paycheck: L was wrapped, Next is the level PRE-PRODUCTION leads to. Opens the night and returns true, or false (no night). */
	static bool GoToNight(const UObject* Ctx, int32 L, int32 Next);
	/** True while a night runs inside the career (so it hands back to the next race instead of going on to the next night). */
	static bool InCareer(const UObject* Ctx);
	/** The next race's title for the night's cards, e.g. "DAY 2 · CHEMICAL WORKS". */
	static FString NextDay(const UObject* Ctx);
	/** From the brawl, when the night is over: PRE-PRODUCTION for the next level (on the finished level's map). False if not in the career. */
	static bool NightOver(const UObject* Ctx);

	// Day 1's cutscene BEFORE THE DRIVE (map L_Day1Set, FTBrawlCutscene.cpp cut 10) before L1 BAYSHORE when the career
	// starts from the title (owner's queue, 15 Nov). Off with -ftnoday1 / -ftnonights and in benches.
	/** True while day 1's cutscene runs as the career's opening (its ENTER then starts the race, not night 1). */
	bool bDay1 = false;
	/** From the title, level 1 picked: opens L_Day1Set and returns true, or false (no map, a bench, turned off). */
	static bool GoToDay1(const UObject* Ctx);
	static bool InDay1(const UObject* Ctx);

	// Every day (run 77694): BEFORE THE DRIVE before its race (PRE-PRODUCTION's READY) and THE WRAP after it (the paycheck,
	// before the night), on the same map; FTBrawlCutscene.cpp cuts 10 / 11. bDay1 = a day's cutscene runs for the career.
	// Off with -ftnodays / -ftnonights, in benches, and in autopilot / script runs unless -ftdays.
	int32 DayLevel = -1; bool bDayWrap = false;
	/** Opens the set for level L's (0-based) BEFORE THE DRIVE or THE WRAP; false if it doesn't play (then go on as before). */
	static bool GoToDayCut(const UObject* Ctx, int32 L, bool bWrap);
	/** For the brawl's game mode on the set: which day (1-8) and which cutscene the career asked for. */
	static bool DayCutNow(const UObject* Ctx, int32& Day, bool& bWrap);
	/** From the day's end card: BEFORE -> the race starts; THE WRAP -> the night (or PRE-PRODUCTION when there is none). */
	static bool DayCutOver(const UObject* Ctx);

	// 3.9 test jump (owner: "it keeps just testing the first level"): straight into the level or night a test needs, its boss
	// too, never through level 1. Spec: L<n> = race n from its start, L<n>boss = race n with its boss out, N<n> = the night
	// after race n inside the career (its K.O., LAST CALL, end card, then PRE-PRODUCTION for race n+1 as after a paycheck),
	// N<n>boss = that night on its boss. Launch option -ftgo=<spec> (once, from the title; uses the test career slot) or the
	// console FTGo <spec> (dev builds, any map). Landing is logged "FT GO landed ..." when the race starts / the boss is out.
	bool bGoBoss = false;
	static bool Go(const UObject* Ctx, const FString& Spec);
	/** The brawl asks once at BeginPlay: open this night on its boss? */
	static bool TakeGoBoss(const UObject* Ctx);

	/** Continue restores a checkpoint, never an arbitrary running simulation. */
	static bool ResumeSaved(const UObject* Ctx);
	/** Clear transient handoffs without changing the saved career. */
	static void ClearSession(const UObject* Ctx);
	/** Caller saves first or explicitly discards; title reloads the persisted career. */
	static bool ReturnToTitle(const UObject* Ctx);
	/** One-shot night entry: Wave=-1/Cut=1 is the intro, Cut=2/3 a completed boss's outro. */
	static bool TakeResumeCheckpoint(const UObject* Ctx, int32& OutWave, int32& OutCut);
	bool bResumeCheckpoint = false;
	int32 ResumeWave = -1, ResumeCut = 0;
};
