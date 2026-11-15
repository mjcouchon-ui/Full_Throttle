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
