#include "FTBrawlGameMode.h"
#include "FTGameInstance.h"
#include "FTInput.h"
#include "GameFramework/PlayerController.h"
#include "HAL/IConsoleManager.h"
#include "Engine/World.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"

#if !UE_BUILD_SHIPPING
namespace
{
	FAutoConsoleCommandWithWorldAndArgs GFTBrawlGrabFixture(TEXT("ftb.GrabFixture"), TEXT("Development: 1 isolates a normal grab target; 0 restores the command-line gameplay scenario"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
		{
			if (AFTBrawlGameMode* G = World ? World->GetAuthGameMode<AFTBrawlGameMode>() : nullptr)
				if (Args.Num() == 1) G->PrepareGrabFixture(FCString::Atoi(*Args[0]) != 0);
		}));
	FAutoConsoleCommandWithWorldAndArgs GFTBrawlAssertAction(TEXT("ftb.AssertAction"), TEXT("Development: assert the actual player action state without changing it"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
		{
			const AFTBrawlGameMode* G = World ? World->GetAuthGameMode<AFTBrawlGameMode>() : nullptr;
			if (!G || Args.Num() != 1 || G->P.St != FName(*Args[0]))
			{
				UE_LOG(LogTemp, Error, TEXT("FTCONTROL FAIL brawlAction expected=%s actual=%s"), Args.Num() ? *Args[0] : TEXT("missing"), G ? *G->P.St.ToString() : TEXT("unavailable"));
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("FTCONTROL PASS brawlAction=%s"), *G->P.St.ToString());
			}
		}));
	FAutoConsoleCommandWithWorld GFTBrawlControlState(TEXT("ftb.ControlState"), TEXT("Log brawl menu, simulation and action state for input smoke checks"),
		FConsoleCommandWithWorldDelegate::CreateLambda([](UWorld* World)
		{
			if (const AFTBrawlGameMode* G = World ? World->GetAuthGameMode<AFTBrawlGameMode>() : nullptr)
				UE_LOG(LogTemp, Display, TEXT("FTBRAWLCONTROLS page=%d selected=%d sim=%.3f action=%s hp=%.1f x=%.1f y=%.1f z=%.1f pad=%d help=%d"),
					G->PausePage, G->PauseSelection, G->StT, *G->P.St.ToString(), G->P.Hp, G->P.X, G->P.Y, G->P.Z, G->bPadLast ? 1 : 0, G->bShowCombatHelp ? 1 : 0);
		}));
}

void AFTBrawlGameMode::PrepareGrabFixture(bool bEnable)
{
	if (bDay || InCut() || SystemScreen)
	{
		UE_LOG(LogTemp, Error, TEXT("FTCONTROL FAIL grab fixture requires active fight"));
		return;
	}
	// Only arrange the encounter. Grab, lift, carry, input buffering, hurl and landing
	// are still performed by ordinary gameplay in response to the test's pad presses.
	PressKeys(FIn());
	bAuto = bKeyTest = bAutoGrab = false;
	Rnd.Initialize(1986 + 11);
	ResetState(0, false);
	if (bEnable)
	{
		Cam = LockX = 0.f; P.X = 320.f; P.Y = 470.f; P.Dir = 1.f;
		Foes.Add(MakeFoe(9001, TEXT("drunk"), P.X + 40.f, P.Y));
		// Keep the single normal target from attacking during the explicit input sequence.
		// No invulnerability, health, eligibility or action result is forced.
		Foes.Last().Cd = 10.f;
		bSpawned = true; BannerT = -1.f;
	}
	else
	{
		bPadTest = FParse::Param(FCommandLine::Get(), TEXT("brawlpadtest"));
		bKeyTest = bPadTest || FParse::Param(FCommandLine::Get(), TEXT("brawlkeytest"));
		bAutoGrab = bKeyTest || FParse::Param(FCommandLine::Get(), TEXT("brawlgrab"));
		bAuto = !bKeyTest && FParse::Param(FCommandLine::Get(), TEXT("brawlauto"));
		Banner = VenueName; BannerSub = VenueTag; BannerT = 0.f;
		ReplayCombatHelp();
	}
	SuppressActionInput();
	SyncActors(0.f);
	UE_LOG(LogTemp, Display, TEXT("FT BRAWL grab fixture enabled=%d foes=%d x=%.0f"), bEnable, Foes.Num(), P.X);
}
#endif

FBox2D AFTBrawlGameMode::SystemItemRect(int32 Index, float ScreenWidth) const
{
	const float Width = SystemScreen == 7 ? 788.f : 580.f;
	const float Top = SystemScreen == 7 ? 508.f : 320.f;
	return FBox2D(FVector2D((ScreenWidth - Width) * .5f, Top + Index * 44.f),
		FVector2D((ScreenWidth + Width) * .5f, Top + Index * 44.f + (SystemScreen == 7 ? 36.f : 38.f)));
}

void AFTBrawlGameMode::SuppressActionInput()
{
	bBlockCombatInput = true;
	Latch = FIn();
	Buffered = {};
	Acc = 0.f;
	bCutPrev = bCutEnterPrev = true;
	// View is both menu Back and scene Skip: a held Back must not skip the scene after resuming.
	if (const APlayerController* PC = GetWorld()->GetFirstPlayerController())
		bCutSkipPrev = PC->IsInputKeyDown(EKeys::Tab) || PC->IsInputKeyDown(EKeys::Gamepad_Special_Left);
}

void AFTBrawlGameMode::ReplayCombatHelp()
{
	HelpT = 0.f;
	bShowCombatHelp = true;
	bHelpInitialized = true;
	if (UFTGameInstance* GI = GetGameInstance<UFTGameInstance>())
	{
		GI->bBrawlHelpSeen = false;
		GI->SaveSettings();
	}
}

void AFTBrawlGameMode::FinishCombatHelp()
{
	bShowCombatHelp = false;
	if (UFTGameInstance* GI = GetGameInstance<UFTGameInstance>())
	{
		GI->bBrawlHelpSeen = true;
		GI->SaveSettings();
	}
}

void AFTBrawlGameMode::TickCombatHelp(float Dt)
{
	if (InCut() || bDay || P.Hp <= 0 || bWon) return;
	if (!bHelpInitialized)
	{
		bHelpInitialized = true;
		if (const UFTGameInstance* GI = GetGameInstance<UFTGameInstance>()) bShowCombatHelp = !GI->bBrawlHelpSeen;
	}
	if (bShowCombatHelp && (HelpT += FMath::Min(Dt, .1f)) >= 30.f) FinishCombatHelp();
}
