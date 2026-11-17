// Per-character exposure charts for the brawl's drawings: one file per character in Charts/, all gathered here.
// A chart only chooses pictures. It cannot change how long a move lasts, when it hits, or how a fighter travels:
// those stay with the fight (FTBrawlGameMode.cpp).
//
// A chart file holds nothing but these two kinds of line (and comments):
//
//   FTB_STATE("<state>", <Clock>, <length ms>)     declares a state and the clock its rows are read against
//   FTB_ROW("<state>", <start ms>, "<frame>")      from <start ms> on that clock, show T_<who>_<frame>
//
// <state> is what the fight calls the state (idle walk run punch kick jump wind swing hurt fly down getup ...;
// a boss's own moves: slamwind slam whirltell whirl dizzy draw fire swig breathe hold bowl gloves lunge hug windup
// sweep, and intro intro2 defeat signaturefollow), optionally narrowed:
//   "punch:2"     only while the fighter's combo counter is 2 (Nova: punch:1 jab, punch:2 cross, punch:3 uppercut;
//                 a guard or a boss: swing:1 is the alternate swing). Without a ":n" chart the plain state is used.
//   "idle>walk"   a WINDOW: the first <length ms> of walk when the state before was idle (walk>idle = the stop,
//                 jump>idle = the landing). After the window the plain state's chart takes over.
//   "turn>walk"   a window that opens each time a walking fighter turns round.
// <Clock>:
//   Time   the state's own move-time, as the fight counts it. <length ms> is only a note (the fight ends the state).
//   Loop   move-time wrapped every <length ms>.
//   Walk   the walk phase, which only advances while the fighter moves; <length ms> is the time of one full cycle
//          at steady walking (Nova 666.667, Nova carrying 833.333, everyone else 909.091) and the row times are
//          positions in that cycle.
//   Age    windows only: time since the state began (or since the turn); the window lasts <length ms>.
// The row shown is the last one whose start is not later than the clock. Before a state's first row, and for any
// state or frame a chart does not have, the older rules decide (FTBrawlFighter.cpp PoseSprite and the two studio
// charts), so a character with an empty chart file is drawn exactly as before.
// <frame> is the texture name without its T_<who>_ prefix; every frame a chart names is loaded with the fighter.
#pragma once

namespace FTBrawlChart
{
enum EKind : uint8 { Time, Loop, Walk, Age, RowLine };
struct FSource { const TCHAR* Who; const TCHAR* State; uint8 Kind; double Ms; const TCHAR* Frame; };

#define FTB_STATE(State, Clock, LengthMs) { FTB_WHO, TEXT(State), FTBrawlChart::Clock, double(LengthMs), nullptr },
#define FTB_ROW(State, StartMs, Frame) { FTB_WHO, TEXT(State), FTBrawlChart::RowLine, double(StartMs), TEXT(Frame) },
static const FSource GSource[] = {
#define FTB_WHO TEXT("Nova")
#include "Charts/FTBrawlChart_Nova.inl"
#include "Charts/FTBrawlChart_NovaMoves.inl"   // Nova's wider move set (FTBrawlMoves.cpp): a second file, states the first does not have
#undef FTB_WHO
#define FTB_WHO TEXT("Punk")
#include "Charts/FTBrawlChart_Punk.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Biker")
#include "Charts/FTBrawlChart_Biker.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Sleaze")
#include "Charts/FTBrawlChart_Sleaze.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Bouncer")
#include "Charts/FTBrawlChart_Bouncer.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Guard")
#include "Charts/FTBrawlChart_Guard.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Dealer")
#include "Charts/FTBrawlChart_Dealer.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Earl")
#include "Charts/FTBrawlChart_Earl.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Vinnie")
#include "Charts/FTBrawlChart_Vinnie.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Pitboss")
#include "Charts/FTBrawlChart_Pitboss.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Frankie")
#include "Charts/FTBrawlChart_Frankie.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Kingpin")
#include "Charts/FTBrawlChart_Kingpin.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Sven")
#include "Charts/FTBrawlChart_Sven.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Commodore")
#include "Charts/FTBrawlChart_Commodore.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Disco")
#include "Charts/FTBrawlChart_Disco.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Hustler")
#include "Charts/FTBrawlChart_Hustler.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Barman")
#include "Charts/FTBrawlChart_Barman.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Roller")
#include "Charts/FTBrawlChart_Roller.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Muscle")
#include "Charts/FTBrawlChart_Muscle.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Firedancer")
#include "Charts/FTBrawlChart_Firedancer.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Surfer")
#include "Charts/FTBrawlChart_Surfer.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Tourist")
#include "Charts/FTBrawlChart_Tourist.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Pitcook")
#include "Charts/FTBrawlChart_Pitcook.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Pinsetter")
#include "Charts/FTBrawlChart_Pinsetter.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Leaguer")
#include "Charts/FTBrawlChart_Leaguer.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Shoeclerk")
#include "Charts/FTBrawlChart_Shoeclerk.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Lanehog")
#include "Charts/FTBrawlChart_Lanehog.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Skibum")
#include "Charts/FTBrawlChart_Skibum.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Linemate")
#include "Charts/FTBrawlChart_Linemate.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Boarder")
#include "Charts/FTBrawlChart_Boarder.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Goalie")
#include "Charts/FTBrawlChart_Goalie.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Deckhand")
#include "Charts/FTBrawlChart_Deckhand.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Firstmate")
#include "Charts/FTBrawlChart_Firstmate.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Waiter")
#include "Charts/FTBrawlChart_Waiter.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Tuxbrute")
#include "Charts/FTBrawlChart_Tuxbrute.inl"
#undef FTB_WHO
#define FTB_WHO TEXT("Dock")
#include "Charts/FTBrawlChart_Dock.inl"
#undef FTB_WHO
	{ nullptr, nullptr, 0, 0.0, nullptr }
};
#undef FTB_STATE
#undef FTB_ROW

struct FRow { float Start; FName Frame; };   // Start: seconds (Time, Age) or the fraction of the cycle (Loop, Walk)
struct FState { uint8 Clock = Time; float Length = 0.f; TArray<FRow> Rows; bool bDeclared = false; };
struct FBase
{
	int32 Plain = INDEX_NONE;
	TArray<TPair<int32, int32>> Combos;    // combo counter -> state
	TArray<TPair<FName, int32>> Windows;   // the state before (or "turn") -> state
};
struct FChart
{
	TArray<FState> States;
	TMap<FName, FBase> Bases;
	TArray<FName> FrameNames;
	int32 NumRows = 0;
};

static TMap<FString, FChart> Build()
{
	TMap<FString, FChart> All;
	TMap<FString, int32> Index;   // "<who>|<state>" -> state number + 1
	for (const FSource* Src = GSource; Src->Who; ++Src)
	{
		FChart& Chart = All.FindOrAdd(Src->Who);
		const FString Key(Src->State);
		const FString Full = FString(Src->Who) + TEXT("|") + Key;
		int32 At = Index.FindRef(Full) - 1;
		if (At < 0)
		{
			At = Chart.States.AddDefaulted();
			Index.Add(Full, At + 1);
			FString Before, Rest = Key, Left, ComboText;
			const bool bWindow = Key.Split(TEXT(">"), &Before, &Rest);
			const bool bCombo = Rest.Split(TEXT(":"), &Left, &ComboText);
			FBase& Base = Chart.Bases.FindOrAdd(FName(bCombo ? *Left : *Rest));
			if (bWindow) Base.Windows.Emplace(FName(*Before), At);
			else if (bCombo) Base.Combos.Emplace(FCString::Atoi(*ComboText), At);
			else Base.Plain = At;
		}
		FState& State = Chart.States[At];
		if (Src->Kind != RowLine)
		{
			State.Clock = Src->Kind; State.Length = float(Src->Ms / 1000.0); State.bDeclared = true;
#if !UE_BUILD_SHIPPING
			if (State.Rows.Num()) UE_LOG(LogTemp, Error, TEXT("FTBRAWL chart FAIL who=%s state=%s: FTB_STATE must come before the state's rows"), Src->Who, Src->State);
#endif
			continue;
		}
#if !UE_BUILD_SHIPPING
		if (!State.bDeclared) UE_LOG(LogTemp, Error, TEXT("FTBRAWL chart FAIL who=%s state=%s: row without FTB_STATE (read as Time)"), Src->Who, Src->State);
#endif
		const bool bCycle = State.Clock == Loop || State.Clock == Walk;
		const float Start = bCycle ? (State.Length > 0.f ? float(Src->Ms / (double(State.Length) * 1000.0)) : 0.f) : float(Src->Ms / 1000.0);
		State.Rows.Add({ Start, FName(Src->Frame) });
		Chart.FrameNames.AddUnique(FName(Src->Frame));
		Chart.NumRows++;
	}
	for (TPair<FString, FChart>& Pair : All)
		for (FState& State : Pair.Value.States)
			State.Rows.StableSort([](const FRow& A, const FRow& B) { return A.Start < B.Start; });
	return All;
}

// the chart of one character, or null when his file has no rows
static const FChart* Find(const FString& Who)
{
	static const TMap<FString, FChart> All = Build();
	const FChart* Chart = All.Find(Who);
	return Chart && Chart->NumRows > 0 ? Chart : nullptr;
}

// the chart's drawing for this moment, or null: then the older rules' choice stands
static UTexture* Select(const FChart& Chart, FName St, float T, float WalkPhase, int32 Combo, float StateAge,
	FName Previous, float TurnAge, const TMap<FName, TObjectPtr<UTexture>>& Frames)
{
	const FBase* Base = Chart.Bases.Find(St);
	if (!Base) return nullptr;
	const FState* State = nullptr;
	float Clock = 0.f;
	static const FName TurnName(TEXT("turn"));
	for (const TPair<FName, int32>& Window : Base->Windows)
	{
		const FState& W = Chart.States[Window.Value];
		const float Age = Window.Key == TurnName ? TurnAge : Window.Key == Previous ? StateAge : -1.f;
		if (Age >= 0.f && Age < W.Length && W.Rows.Num()) { State = &W; Clock = Age; break; }
	}
	if (!State)
	{
		for (const TPair<int32, int32>& C : Base->Combos)
			if (C.Key == Combo && Chart.States[C.Value].Rows.Num()) { State = &Chart.States[C.Value]; break; }
		if (!State && Base->Plain != INDEX_NONE) State = &Chart.States[Base->Plain];
		if (!State || !State->Rows.Num()) return nullptr;
		if (State->Clock == Walk) Clock = FMath::Frac(FMath::Max(WalkPhase, 0.f));
		else if (State->Clock == Loop) Clock = State->Length > 0.f ? FMath::Frac(FMath::Max(T, 0.f) / State->Length) : 0.f;
		else Clock = T;
	}
	const FRow* Shown = nullptr;
	for (const FRow& Row : State->Rows)
	{
		if (Row.Start > Clock) break;
		Shown = &Row;
	}
	return Shown ? Frames.FindRef(Shown->Frame).Get() : nullptr;
}
}
