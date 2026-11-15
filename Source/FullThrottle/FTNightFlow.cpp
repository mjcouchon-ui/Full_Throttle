// The nights between the races: see FTNightFlow.h. Log lines start "FT NIGHT".
#include "FTNightFlow.h"
#include "FTCore.h"
#include "FTGameInstance.h"
#include "FTProgress.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/PackageName.h"

namespace
{
	// story flow order: after level 1 (Bayshore) night 1, after 2 (Chemical Works) night 2, after 3 (Nevada) night 3, ...
	const TCHAR* const NightMaps[] = {
		TEXT("/Game/Brawl/Maps/L_RustyAnchor"),   // N1 THE RUSTY ANCHOR (boss BIG EARL, its cutscenes)
		TEXT("/Game/Brawl/Maps/L_VelvetRoom"),    // N2 THE VELVET ROOM (boss VINNIE VELOUR)
		TEXT("/Game/Brawl/Maps/L_Lucky7"),        // N3 THE LUCKY 7 (boss THE PIT BOSS), when it lands
		TEXT("/Game/Brawl/Maps/L_TikiTorch"),     // N4 .. N7: when they land
		TEXT("/Game/Brawl/Maps/L_StrikeCity"),
		TEXT("/Game/Brawl/Maps/L_FrostbiteLodge"),
		TEXT("/Game/Brawl/Maps/L_SeaBiscuit"),
	};
}

UFTNightFlow* UFTNightFlow::Get(const UObject* Ctx)
{
	const UWorld* W = Ctx ? Ctx->GetWorld() : nullptr;
	UGameInstance* G = W ? W->GetGameInstance() : nullptr;
	return G ? G->GetSubsystem<UFTNightFlow>() : nullptr;
}

FString UFTNightFlow::NightMapAfter(int32 L)
{
	if (L < 0 || L >= (int32)UE_ARRAY_COUNT(NightMaps)) return FString();
	return FPackageName::DoesPackageExist(NightMaps[L]) ? FString(NightMaps[L]) : FString();
}

bool UFTNightFlow::GoToNight(const UObject* Ctx, int32 L, int32 Next)
{
	UFTNightFlow* NF = Get(Ctx);
	UFTGameInstance* G = Ctx && Ctx->GetWorld() ? Ctx->GetWorld()->GetGameInstance<UFTGameInstance>() : nullptr;
	UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(Ctx);
	const FString Map = NightMapAfter(L);
	if (!NF || !G || !CS || G->bBench || Next < 0 || Next >= FTData::NumLevels() || Map.IsEmpty() || FParse::Param(FCommandLine::Get(), TEXT("ftnonights"))) return false;
	// what PRE-PRODUCTION would have saved: quitting during the night reopens PRE-PRODUCTION for the next level
	CS->C.PendingLevel = Next; CS->C.bPendingFromTitle = false; CS->Save(TEXT("Night"));
	NF->NextLevel = Next; NF->FromLevel = L;
	UE_LOG(LogFT, Display, TEXT("FT NIGHT start after=L%d map=%s next=L%d"), L + 1, *Map, Next + 1);
	UGameplayStatics::SetGlobalTimeDilation(Ctx, 1.f);
	UGameplayStatics::SetGamePaused(Ctx, false);
	UGameplayStatics::OpenLevel(Ctx, FName(*Map));
	return true;
}

bool UFTNightFlow::InCareer(const UObject* Ctx)
{
	const UFTNightFlow* NF = Get(Ctx);
	return NF && NF->NextLevel >= 0;
}

FString UFTNightFlow::NextDay(const UObject* Ctx)
{
	const UFTNightFlow* NF = Get(Ctx);
	if (!NF || NF->NextLevel < 0) return FString();
	return FString::Printf(TEXT("DAY %d · %s"), NF->NextLevel + 1, *FTData::Level(NF->NextLevel).Zone);
}

bool UFTNightFlow::NightOver(const UObject* Ctx)
{
	UFTNightFlow* NF = Get(Ctx);
	UFTGameInstance* G = Ctx && Ctx->GetWorld() ? Ctx->GetWorld()->GetGameInstance<UFTGameInstance>() : nullptr;
	if (!NF || !G || NF->NextLevel < 0) return false;
	const int32 Next = NF->NextLevel, From = FMath::Clamp(NF->FromLevel, 0, FTData::NumLevels() - 1);
	NF->NextLevel = -1; NF->FromLevel = -1;
	// the race's own flow from here: the finished level's map opens PRE-PRODUCTION for the career's pending level
	// (= Next, saved by GoToNight), the same screen the paycheck leads to; its READY loads the next race
	G->bPendingPreProd = true;
	UE_LOG(LogFT, Display, TEXT("FT NIGHT over next=L%d preproduction on=%s"), Next + 1, *FTData::Level(From).Map.ToString());
	UGameplayStatics::SetGlobalTimeDilation(Ctx, 1.f);
	UGameplayStatics::SetGamePaused(Ctx, false);
	UGameplayStatics::OpenLevel(Ctx, FTData::Level(From).Map);
	return true;
}

// ---- the days: day 1's cutscene BEFORE THE DRIVE between the title and L1 BAYSHORE (run 34723), and (run 77694) every
// day's BEFORE THE DRIVE before its race and THE WRAP after it, on the brawl's Route 9 set. Log lines "FT DAY1" / "FT DAY".
namespace { const TCHAR* const Day1Map = TEXT("/Game/Brawl/Maps/L_Day1Set"); }

bool UFTNightFlow::GoToDay1(const UObject* Ctx)
{
	return GoToDayCut(Ctx, 0, false);
}

bool UFTNightFlow::GoToDayCut(const UObject* Ctx, int32 L, bool bWrap)
{
	UFTNightFlow* NF = Get(Ctx);
	UFTGameInstance* G = Ctx && Ctx->GetWorld() ? Ctx->GetWorld()->GetGameInstance<UFTGameInstance>() : nullptr;
	const TCHAR* Cl = FCommandLine::Get();
	if (!NF || !G || G->bBench || L < 0 || L >= FTData::NumLevels() || !FPackageName::DoesPackageExist(Day1Map)
		|| FParse::Param(Cl, TEXT("ftnoday1")) || FParse::Param(Cl, TEXT("ftnodays")) || FParse::Param(Cl, TEXT("ftnonights"))) return false;
	// autopilot / script runs (benches, the night tests) keep the old flow unless they ask for the days (-ftdays)
	if (L > 0 || bWrap) { if ((G->bAutoDrive || G->bExecScript) && !FParse::Param(Cl, TEXT("ftdays"))) return false; }
	NF->bDay1 = true; NF->DayLevel = L; NF->bDayWrap = bWrap;
	if (!bWrap) { NF->NextLevel = -1; NF->FromLevel = -1; }
	if (L == 0 && !bWrap) UE_LOG(LogFT, Display, TEXT("FT DAY1 start map=%s then=L1"), Day1Map);
	UE_LOG(LogFT, Display, TEXT("FT DAY start day=%d cut=%s map=%s"), L + 1, bWrap ? TEXT("wrap") : TEXT("before"), Day1Map);
	G->StopTitleMusic(0.8f);
	UGameplayStatics::SetGlobalTimeDilation(Ctx, 1.f);
	UGameplayStatics::SetGamePaused(Ctx, false);
	UGameplayStatics::OpenLevel(Ctx, FName(Day1Map));
	return true;
}

bool UFTNightFlow::InDay1(const UObject* Ctx)
{
	const UFTNightFlow* NF = Get(Ctx);
	return NF && NF->bDay1;
}

bool UFTNightFlow::DayCutNow(const UObject* Ctx, int32& Day, bool& bWrap)
{
	const UFTNightFlow* NF = Get(Ctx);
	if (!NF || !NF->bDay1) return false;
	Day = NF->DayLevel + 1; bWrap = NF->bDayWrap;
	return true;
}

bool UFTNightFlow::Day1Over(const UObject* Ctx)
{
	return DayCutOver(Ctx);
}

bool UFTNightFlow::DayCutOver(const UObject* Ctx)
{
	UFTNightFlow* NF = Get(Ctx);
	UFTGameInstance* G = Ctx && Ctx->GetWorld() ? Ctx->GetWorld()->GetGameInstance<UFTGameInstance>() : nullptr;
	if (!NF || !G || !NF->bDay1) return false;
	NF->bDay1 = false;
	const int32 L = FMath::Clamp(NF->DayLevel, 0, FTData::NumLevels() - 1);
	const bool bWrap = NF->bDayWrap;
	NF->DayLevel = -1; NF->bDayWrap = false;
	UGameplayStatics::SetGlobalTimeDilation(Ctx, 1.f);
	UGameplayStatics::SetGamePaused(Ctx, false);
	if (!bWrap)
	{
		// BEFORE THE DRIVE: what PRE-PRODUCTION's READY (or the title's start for level 1) did: the map opens and the race starts
		G->PendingLevel = L; G->bStartRace = true;
		if (L == 0) UE_LOG(LogFT, Display, TEXT("FT DAY1 over race=L1 map=%s"), *FTData::Level(0).Map.ToString());
		UE_LOG(LogFT, Display, TEXT("FT DAY over day=%d cut=before race=L%d"), L + 1, L + 1);
		UGameplayStatics::OpenLevel(Ctx, FTData::Level(L).Map);
		return true;
	}
	// THE WRAP: on to the night, as the paycheck did; with no night (the last level, a missing map) back to the race's flow:
	// PRE-PRODUCTION on the finished level's map (for the next level, or after the last one "BACK TO THE TITLE")
	const int32 Next = L + 1 < FTData::NumLevels() ? L + 1 : -2;
	UE_LOG(LogFT, Display, TEXT("FT DAY over day=%d cut=wrap next=%s"), L + 1, Next >= 0 ? *FString::Printf(TEXT("night %d"), L + 1) : TEXT("title"));
	if (Next >= 0 && GoToNight(Ctx, L, Next)) return true;
	if (UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(Ctx)) { CS->C.PendingLevel = Next; CS->C.bPendingFromTitle = false; CS->Save(TEXT("Wrap")); }
	G->bPendingPreProd = true;
	UGameplayStatics::OpenLevel(Ctx, FTData::Level(L).Map);
	return true;
}
