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
};
