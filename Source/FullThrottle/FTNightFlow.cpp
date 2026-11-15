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
	// Preserve the night itself; PRE-PRODUCTION is saved only after its final card.
	CS->C.PendingLevel = Next; CS->C.bPendingFromTitle = false;
	CS->SetCheckpoint(TEXT("night"), L, -1, false, TEXT("Night"));
	NF->NextLevel = Next; NF->FromLevel = L;
	UE_LOG(LogFT, Display, TEXT("FT NIGHT start after=L%d map=%s next=L%d"), L + 1, *Map, Next + 1);
	G->StopTitleMusic(0.3f);   // 3.9.1: a night opened from the title (the FTGo / -ftgo jump) kept the title song playing under the fight
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
	if (UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(Ctx))
	{
		CS->C.PendingLevel = Next; CS->C.bPendingFromTitle = false;
		CS->SetCheckpoint(TEXT("preproduction"), Next, 0, false, TEXT("NightComplete"));
	}
	G->bPendingPreProd = true;
	UE_LOG(LogFT, Display, TEXT("FT NIGHT over next=L%d preproduction on=%s"), Next + 1, *FTData::Level(From).Map.ToString());
	UGameplayStatics::SetGlobalTimeDilation(Ctx, 1.f);
	UGameplayStatics::SetGamePaused(Ctx, false);
	UGameplayStatics::OpenLevel(Ctx, FTData::Level(From).Map);
	return true;
}

// ---- 3.9 test jump: FTGo / -ftgo=
bool UFTNightFlow::Go(const UObject* Ctx, const FString& SpecIn)
{
	UFTNightFlow* NF = Get(Ctx);
	UFTGameInstance* G = Ctx && Ctx->GetWorld() ? Ctx->GetWorld()->GetGameInstance<UFTGameInstance>() : nullptr;
	FString S = SpecIn.TrimStartAndEnd().ToUpper();
	const bool bBoss = S.EndsWith(TEXT("BOSS"));
	if (bBoss) S = S.LeftChop(4).TrimEnd();
	const int32 N = S.Len() >= 2 ? FCString::Atoi(*S.RightChop(1)) : 0;
	if (!NF || !G || N < 1 || N > FTData::NumLevels() || (S[0] != TEXT('L') && S[0] != TEXT('N') && S[0] != TEXT('D') && S[0] != TEXT('W')))
	{
		UE_LOG(LogFT, Warning, TEXT("FT GO bad spec=%s (L<n>[boss], N<n>[boss], D<n> or W<n>)"), *SpecIn);
		return false;
	}
	ClearSession(Ctx);
	if (S[0] == TEXT('D') || S[0] == TEXT('W'))
	{
		// run 2 cycle 5: day N's BEFORE THE DRIVE (D) or THE WRAP (W) on the day set (script runs also need -ftdays)
		UE_LOG(LogFT, Display, TEXT("FT GO day=%d cut=%s"), N, S[0] == TEXT('W') ? TEXT("wrap") : TEXT("before"));
		if (GoToDayCut(Ctx, N - 1, S[0] == TEXT('W'))) return true;
		UE_LOG(LogFT, Warning, TEXT("FT GO day=%d refused (no day set, or a script run without -ftdays)"), N);
		return false;
	}
	if (S[0] == TEXT('N'))
	{
		// the night after race N, as the paycheck opens it: afterwards PRE-PRODUCTION on race N's map for race N+1
		NF->bGoBoss = bBoss;
		UE_LOG(LogFT, Display, TEXT("FT GO night=%d boss=%d"), N, bBoss ? 1 : 0);
		if (GoToNight(Ctx, N - 1, N)) return true;
		NF->bGoBoss = false;
		UE_LOG(LogFT, Warning, TEXT("FT GO night=%d has no night map (or nights are off)"), N);
		return false;
	}
	NF->NextLevel = -1; NF->FromLevel = -1; NF->bGoBoss = false;
	const int32 L = N - 1;
	G->PendingLevel = L; G->bStartRace = true; G->GoLanding = bBoss ? 2 : 1;
	UE_LOG(LogFT, Display, TEXT("FT GO level=L%d boss=%d map=%s"), N, bBoss ? 1 : 0, *FTData::Level(L).Map.ToString());
	G->StopTitleMusic(0.3f);
	UGameplayStatics::SetGlobalTimeDilation(Ctx, 1.f);
	UGameplayStatics::SetGamePaused(Ctx, false);
	UGameplayStatics::OpenLevel(Ctx, FTData::Level(L).Map);
	return true;
}

bool UFTNightFlow::TakeGoBoss(const UObject* Ctx)
{
	UFTNightFlow* NF = Get(Ctx);
	if (!NF || !NF->bGoBoss) return false;
	NF->bGoBoss = false;
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
	if (UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(Ctx))
		CS->SetCheckpoint(TEXT("day"), L, 0, bWrap, TEXT("DayCut"));
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
		if (UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(Ctx))
			CS->SetCheckpoint(TEXT("race"), L, 0, false, TEXT("DayBeforeComplete"));
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
	if (UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(Ctx))
	{
		CS->C.PendingLevel = Next; CS->C.bPendingFromTitle = false;
		CS->SetCheckpoint(Next >= 0 ? TEXT("preproduction") : TEXT("complete"), Next >= 0 ? Next : L, 0, false, TEXT("Wrap"));
	}
	G->bPendingPreProd = true;
	UGameplayStatics::OpenLevel(Ctx, FTData::Level(L).Map);
	return true;
}

// Checkpoint handoffs live in the persistent game instance; map actors remain disposable.
void UFTNightFlow::ClearSession(const UObject* Ctx)
{
	if (UFTNightFlow* NF = Get(Ctx))
	{
		NF->NextLevel = NF->FromLevel = NF->DayLevel = -1;
		NF->bDay1 = NF->bDayWrap = NF->bGoBoss = NF->bResumeCheckpoint = false;
		NF->ResumeWave = -1; NF->ResumeCut = 0;
	}
	if (UFTGameInstance* G = Ctx && Ctx->GetWorld() ? Ctx->GetWorld()->GetGameInstance<UFTGameInstance>() : nullptr)
	{
		G->PendingLevel = -1; G->PendingScene = 0; G->bStartRace = G->bPendingPreProd = false;
		G->GoLanding = 0; G->bAttractCut = false; G->AttractD0 = -1.f;
		G->KeepTitleSel = 0; G->KeepTitleActionFocus = INDEX_NONE; G->KeepTitleT = 0.f;
	}
}

bool UFTNightFlow::TakeResumeCheckpoint(const UObject* Ctx, int32& OutWave, int32& OutCut)
{
	UFTNightFlow* NF = Get(Ctx);
	if (!NF || !NF->bResumeCheckpoint) return false;
	OutWave = NF->ResumeWave; OutCut = NF->ResumeCut;
	NF->bResumeCheckpoint = false;
	return true;
}

bool UFTNightFlow::ReturnToTitle(const UObject* Ctx)
{
	UFTGameInstance* G = Ctx && Ctx->GetWorld() ? Ctx->GetWorld()->GetGameInstance<UFTGameInstance>() : nullptr;
	if (!G) return false;
	ClearSession(Ctx);
	if (UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(Ctx)) CS->Load();
	G->StopTitleMusic(0.f);
	UGameplayStatics::SetGlobalTimeDilation(Ctx, 1.f);
	UGameplayStatics::SetGamePaused(Ctx, false);
	UE_LOG(LogFT, Display, TEXT("FT SESSION return=title"));
	UGameplayStatics::OpenLevel(Ctx, FTData::Level(0).Map);
	return true;
}

bool UFTNightFlow::ResumeSaved(const UObject* Ctx)
{
	UFTNightFlow* NF = Get(Ctx);
	UFTGameInstance* G = Ctx && Ctx->GetWorld() ? Ctx->GetWorld()->GetGameInstance<UFTGameInstance>() : nullptr;
	UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(Ctx);
	if (!NF || !G || !CS || !CS->HasContinue()) return false;
	const FString Kind = CS->C.ResumeKind;
	const int32 L = CS->C.ResumeLevel, Stage = CS->C.ResumeStage;
	if (L < 0 || L >= FTData::NumLevels()) return false;
	FName Map;
	ClearSession(Ctx);
	if (Kind == TEXT("race"))
	{
		Map = FTData::Level(L).Map;
		G->PendingLevel = L; G->bStartRace = true;
	}
	else if (Kind == TEXT("day"))
	{
		if (!FPackageName::DoesPackageExist(Day1Map)) return false;
		Map = FName(Day1Map); NF->bDay1 = true; NF->DayLevel = L; NF->bDayWrap = CS->C.bResumeWrap;
	}
	else if (Kind == TEXT("night") || Kind == TEXT("nightoutro"))
	{
		const FString NightMap = NightMapAfter(L);
		if (NightMap.IsEmpty() || L + 1 >= FTData::NumLevels()) return false;
		Map = FName(*NightMap); NF->FromLevel = L; NF->NextLevel = L + 1;
		NF->bResumeCheckpoint = true;
		NF->ResumeWave = Kind == TEXT("night") ? Stage : -1;
		NF->ResumeCut = Kind == TEXT("nightoutro") ? FMath::Clamp(Stage, 2, L == 0 ? 3 : 2) : Stage < 0 ? 1 : 0;
		CS->C.PendingLevel = L + 1; CS->C.bPendingFromTitle = false;
	}
	else if (Kind == TEXT("preproduction") || Kind == TEXT("complete"))
	{
		CS->C.PendingLevel = Kind == TEXT("complete") ? -2 : L;
		CS->C.bPendingFromTitle = false; G->bPendingPreProd = true;
		Map = FTData::Level(Kind == TEXT("complete") ? L : FMath::Max(0, L - 1)).Map;
	}
	else return false;
	G->StopTitleMusic(0.3f);
	UGameplayStatics::SetGlobalTimeDilation(Ctx, 1.f);
	UGameplayStatics::SetGamePaused(Ctx, false);
	UE_LOG(LogFT, Display, TEXT("FT SESSION continue kind=%s level=%d stage=%d wrap=%d"), *Kind, L + 1, Stage, CS->C.bResumeWrap ? 1 : 0);
	UGameplayStatics::OpenLevel(Ctx, Map);
	return true;
}
