#include "FTBrawlGameMode.h"
#include "FTBrawlPlayerController.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraActor.h"
#include "Components/AudioComponent.h"
#include "UObject/UObjectIterator.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "HAL/IConsoleManager.h"
#if __has_include("FTProgress.h")
#include "FTProgress.h"
#include "FTGameInstance.h"
#include "FTNightFlow.h"
#define FTB_SESSION_CAREER 1
#else
#include "FTBrawlGameInstance.h"
#define FTB_SESSION_CAREER 0
#endif

// Screen: 0 gameplay, 1 title, 2 pause, 3 options, 4 replace campaign,
// 5 checkpoint-aware exit confirmation, 6 save failure. Menu input never enters combat.
void AFTBrawlGameMode::InitializeSession()
{
	bSessionReady=true;
#if !FTB_SESSION_CAREER
	if (UFTBrawlGameInstance* GI=Cast<UFTBrawlGameInstance>(GetGameInstance()))
	{
		GI->ApplyPreferences();
		if (GI->bShowTitle) { SetSystemScreen(1); return; }
		if (bDay) GI->bPendingResume=false;
	}
#endif
	SaveCheckpoint(TEXT("EncounterEntered"));
}

bool AFTBrawlGameMode::ApplySavedCheckpoint()
{
	int32 SavedWave=-1,SavedCut=0; bool bResume=false;
#if FTB_SESSION_CAREER
	bResume=UFTNightFlow::TakeResumeCheckpoint(this,SavedWave,SavedCut);
#else
	if (UFTBrawlGameInstance* GI=Cast<UFTBrawlGameInstance>(GetGameInstance())) if (GI->bPendingResume)
	{
		GI->bPendingResume=false; bResume=true;
		if (GI->Kind==TEXT("nightoutro")) SavedCut=GI->Stage;
		else if (GI->Stage<0) SavedCut=1;
		else SavedWave=GI->Stage;
	}
#endif
	if (!bResume) return false;
	if (SavedCut>=2)
	{
		ResetState(NumWaves,false); Cam=LockX=ArenaEnd(); P.X=Cam+200.f;
		bWon=true; WinT=3.f; bCutDone=true; bCuts=true; StartCut(FMath::Clamp(SavedCut,2,Night==1?3:2));
	}
	else if (SavedCut==1) { bCuts=true; StartCut(1); }
	else
	{
		ResetState(FMath::Clamp(SavedWave,0,NumWaves),false);
		Cam=FMath::Max(0.f,NextWaveAt(Wave)-120.f); P.X=Cam+200.f;
		Banner=TEXT("BACK IN THE FIGHT"); BannerSub=TEXT("Checkpoint restored / full health / wave restarted"); BannerT=0;
		SyncActors(0.f);
	}
	UE_LOG(LogTemp,Log,TEXT("FT BRAWL CONTINUE night=%d wave=%d cut=%d hp=%.0f"),Night,Wave,CutId,P.Hp);
	return true;
}

bool AFTBrawlGameMode::SaveCheckpoint(const TCHAR* Reason)
{
	if (!bSessionReady || SystemScreen==1) return true;
	const FString Kind=bDay?TEXT("day"):(bWon || CutId==2 || CutId==3)?TEXT("nightoutro"):TEXT("night");
	const int32 Level=bDay?DayN-1:Night-1;
	const int32 Stage=bDay?0:Kind==TEXT("nightoutro")?(CutId==3?3:2):CutId==1?-1:FMath::Clamp(Wave,0,NumWaves);
#if FTB_SESSION_CAREER
	if (UFTCareerSubsystem* CS=UFTCareerSubsystem::Get(this)) return CS->SetCheckpoint(Kind,Level,Stage,bDay&&bDayWrap,Reason);
#else
	if (UFTBrawlGameInstance* GI=Cast<UFTBrawlGameInstance>(GetGameInstance())) return GI->Checkpoint(Kind,Level,Stage,bDay&&bDayWrap,Reason);
#endif
	return false;
}

FString AFTBrawlGameMode::CheckpointLabel() const
{
#if FTB_SESSION_CAREER
	if (UFTCareerSubsystem* CS=UFTCareerSubsystem::Get(this)) return CS->ContinueLabel();
#else
	if (UFTBrawlGameInstance* GI=Cast<UFTBrawlGameInstance>(GetGameInstance())) return GI->ContinueLabel();
#endif
	return TEXT("Checkpoint unavailable");
}
FString AFTBrawlGameMode::SaveStatus(bool& bError) const
{
	bError=false;
#if FTB_SESSION_CAREER
	if (UFTCareerSubsystem* CS=UFTCareerSubsystem::Get(this))
	{
		bError=CS->bSaveError;
		if (bError) return TEXT("SAVE FAILED / Open Pause to retry");
		if (CS->bSaving) return TEXT("SAVING...");
		if (CS->LastSaveTime>0 && FPlatformTime::Seconds()-CS->LastSaveTime<3.) return TEXT("CHECKPOINT SAVED");
		if (CS->bRecoveredFromBackup) return TEXT("BACKUP RECOVERED");
	}
#else
	if (UFTBrawlGameInstance* GI=Cast<UFTBrawlGameInstance>(GetGameInstance()))
	{
		bError=GI->bSaveError || !GI->PreferenceError.IsEmpty();
		if (bError) return TEXT("SAVE FAILED / Open Pause to retry");
		if (GI->bSaving) return TEXT("SAVING...");
		if (GI->LastSaveTime>0 && FPlatformTime::Seconds()-GI->LastSaveTime<3.) return TEXT("CHECKPOINT SAVED");
		if (GI->bRecovered) return TEXT("BACKUP RECOVERED");
	}
#endif
	return FString();
}

void AFTBrawlGameMode::SetSystemScreen(int32 Screen)
{
	const bool bOpening=SystemScreen==0 && Screen!=0;
	SystemScreen=Screen; SystemSelection=0;
	if (Screen==1 && !SystemRowEnabled(0)) SystemSelection=1;
	if (APlayerController* PC=GetWorld()->GetFirstPlayerController())
	{
		if (bOpening)
		{
			PressKeys(FIn()); Latch=FIn(); Buffered=FBufferedInput();
			PC->ClientStopForceFeedback(nullptr,NAME_None);
			FLatentActionInfo RumbleStop; RumbleStop.CallbackTarget=this; RumbleStop.UUID=38222; RumbleStop.Linkage=INDEX_NONE;
			PC->PlayDynamicForceFeedback(0.f,0.f,true,true,true,true,EDynamicForceFeedbackAction::Stop,RumbleStop);
			SystemPausedAudio.Reset();
			for (TObjectIterator<UAudioComponent> It;It;++It) if (It->GetWorld()==GetWorld() && It->IsPlaying() && !It->bIsPaused)
			{ SystemPausedAudio.Add(*It); It->SetPaused(true); }
		}
		PC->bShowMouseCursor=Screen!=0;
		if (Screen)
		{ FInputModeGameAndUI Mode; Mode.SetHideCursorDuringCapture(false); Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); PC->SetInputMode(Mode); }
		else PC->SetInputMode(FInputModeGameOnly());
		if (PC->PlayerInput) PC->PlayerInput->FlushPressedKeys();
		if (AFTBrawlPlayerController* BrawlPC=Cast<AFTBrawlPlayerController>(PC)) BrawlPC->ResetSystemPointer();
	}
	if (Screen==0)
	{
		for (const auto& Audio:SystemPausedAudio) if (Audio.IsValid()) Audio->SetPaused(false);
		SystemPausedAudio.Reset();
		// Require release of menu controls before accepting gameplay/dialogue input again.
		bScreenEnterPrev=bCutPrev=bCutEnterPrev=bCutSkipPrev=true;
		InputResumeAt=FPlatformTime::Seconds()+0.18;
		Acc=0; Latch=FIn(); Buffered=FBufferedInput();
	}
	UGameplayStatics::SetGamePaused(this,Screen!=0);
	UE_LOG(LogTemp,Log,TEXT("FT BRAWL SYSTEM screen=%d paused=%d wave=%d cut=%d"),Screen,Screen!=0,Wave,CutId);
}
void AFTBrawlGameMode::ToggleSystemMenu()
{
	if (!bSessionReady) return;
	if (SystemScreen==0)
	{
		MenuHome=2; bool Error=false; SaveStatus(Error); RequestedExit=0; SetSystemScreen(Error?6:2);
	}
	else SystemBack();
}
void AFTBrawlGameMode::SystemBack()
{
	if (SystemScreen==1) return;
	if (SystemScreen==2) SetSystemScreen(0);
	else SetSystemScreen(MenuHome);
}
void AFTBrawlGameMode::SystemNavigate(int32 Delta)
{
	if (!SystemScreen) return;
	const int32 N=SystemRows().Num(); SystemSelection=(SystemSelection+Delta+N)%N;
	if (!SystemRowEnabled(SystemSelection)) SystemSelection=(SystemSelection+Delta+N)%N;
	UE_LOG(LogTemp,Log,TEXT("FT BRAWL SYSTEM selection=%d label=%s"),SystemSelection,*SystemRows()[SystemSelection]);
}
bool AFTBrawlGameMode::SystemRowEnabled(int32 Index) const
{
#if !FTB_SESSION_CAREER
	if (SystemScreen==1 && Index==0)
	{
		const UFTBrawlGameInstance* GI=Cast<UFTBrawlGameInstance>(GetGameInstance()); return GI&&GI->bValid;
	}
#endif
	return true;
}

TArray<FString> AFTBrawlGameMode::SystemRows() const
{
	if (SystemScreen==1) return {TEXT("CONTINUE"),TEXT("NEW GAME"),TEXT("OPTIONS"),TEXT("QUIT TO DESKTOP")};
	if (SystemScreen==2) return {TEXT("RESUME"),TEXT("OPTIONS"),TEXT("QUIT TO TITLE"),TEXT("QUIT TO DESKTOP")};
	if (SystemScreen==3)
	{
		bool Muted=false,Reduced=ReducedPresentationMotion();
#if FTB_SESSION_CAREER
		if (const UFTGameInstance* GI=Cast<UFTGameInstance>(GetGameInstance())) Muted=GI->bMuted;
#else
		if (const UFTBrawlGameInstance* GI=Cast<UFTBrawlGameInstance>(GetGameInstance())) Muted=GI->bMuted;
#endif
		return {FString::Printf(TEXT("AUDIO / %s"),Muted?TEXT("MUTED"):TEXT("ON")),FString::Printf(TEXT("REDUCED MOTION / %s"),Reduced?TEXT("ON"):TEXT("OFF")),TEXT("BACK")};
	}
	if (SystemScreen==4) return {TEXT("CANCEL"),TEXT("REPLACE CAMPAIGN")};
	if (SystemScreen==5) return {TEXT("CANCEL"),RequestedExit==2?TEXT("SAVE AND QUIT TO DESKTOP"):TEXT("SAVE AND QUIT TO TITLE")};
	if (SystemScreen==6) return {TEXT("RETRY SAVE"),RequestedExit>0&&RequestedExit<3?TEXT("QUIT WITHOUT SAVING"):TEXT("BACK")};
	return {};
}
FString AFTBrawlGameMode::SystemHeading() const
{
	switch(SystemScreen)
	{
	case 1:return TEXT("FULL THROTTLE / BRAWL"); case 2:return TEXT("PAUSED"); case 3:return TEXT("OPTIONS");
	case 4:return TEXT("START A NEW CAMPAIGN?"); case 5:return TEXT("LEAVE THIS SESSION?"); case 6:return TEXT("PROGRESS WAS NOT SAVED"); default:return {};
	}
}
FString AFTBrawlGameMode::SystemDetail() const
{
	if (SystemScreen==3) return TEXT("Audio and accessibility preferences are kept when starting a New Game.");
	if (SystemScreen==4) return TEXT("This replaces campaign progress with Day 1. Audio and accessibility preferences stay unchanged. Cancel leaves the save untouched.");
	if (SystemScreen==6)
	{
#if FTB_SESSION_CAREER
		if (UFTCareerSubsystem* CS=UFTCareerSubsystem::Get(this)) return CS->SaveError+TEXT("\nRetry, or explicitly leave without saving. The previous valid file is retained.");
#else
		if (UFTBrawlGameInstance* GI=Cast<UFTBrawlGameInstance>(GetGameInstance())) return (GI->PreferenceError.IsEmpty()?GI->SaveError:GI->PreferenceError)+TEXT("\nThe previous valid save is retained.");
#endif
	}
	FString Detail=CheckpointLabel();
	if (!bDay && SystemScreen!=1) Detail+=TEXT("\nWaves restart with full health and fresh pickups; held items and active power-ups reset.");
	if (SystemScreen==5) Detail=TEXT("Continue resumes at this checkpoint; current combat or scene timing is not stored.\n")+Detail;
#if !FTB_SESSION_CAREER
	if (UFTBrawlGameInstance* GI=Cast<UFTBrawlGameInstance>(GetGameInstance())) if (!GI->LoadNotice.IsEmpty()) Detail+=TEXT("\n")+GI->LoadNotice;
#endif
	return Detail;
}

void AFTBrawlGameMode::SystemActivate()
{
	if (!SystemScreen || !SystemRowEnabled(SystemSelection)) return;
	const int32 Row=SystemSelection;
	UE_LOG(LogTemp,Log,TEXT("FT BRAWL SYSTEM activate screen=%d row=%d"),SystemScreen,Row);
	if (SystemScreen==1)
	{
#if !FTB_SESSION_CAREER
		UFTBrawlGameInstance* GI=Cast<UFTBrawlGameInstance>(GetGameInstance()); if (!GI) return;
		MenuHome=1;
		if (Row==0) { GI->Continue(); return; }
		if (Row==1)
		{
			RequestedExit=3;
			if (GI->bValid || !GI->LoadNotice.IsEmpty()) SetSystemScreen(4); else FinishSystemAction(false);
		}
		if (Row==2) SetSystemScreen(3);
		if (Row==3) { RequestedExit=2; FinishSystemAction(false); }
#endif
		return;
	}
	if (SystemScreen==2)
	{
		MenuHome=2;
		if (Row==0) SetSystemScreen(0);
		else if (Row==1) SetSystemScreen(3);
		else RequestSystemQuit(Row==3);
		return;
	}
	if (SystemScreen==3)
	{
		if (Row==2) { SetSystemScreen(MenuHome); return; }
#if FTB_SESSION_CAREER
		if (UFTGameInstance* GI=Cast<UFTGameInstance>(GetGameInstance()))
		{ if (Row==0) GI->bMuted=!GI->bMuted; else GI->bReducedMotion=!GI->bReducedMotion; GI->ApplyMute(); GI->SaveSettings(); }
#else
		if (UFTBrawlGameInstance* GI=Cast<UFTBrawlGameInstance>(GetGameInstance()))
		{
			if (Row==0) GI->bMuted=!GI->bMuted; else GI->bReducedMotion=!GI->bReducedMotion;
			if (!GI->SavePreferences()) { RequestedExit=0; SetSystemScreen(6); }
		}
#endif
		return;
	}
	if (SystemScreen==4 || SystemScreen==5) { if (Row==0) SetSystemScreen(MenuHome); else FinishSystemAction(false); return; }
	if (SystemScreen==6) { if (Row==0) FinishSystemAction(false); else if (RequestedExit>0&&RequestedExit<3) FinishSystemAction(true); else SetSystemScreen(MenuHome); }
}
void AFTBrawlGameMode::RequestSystemQuit(bool bDesktop)
{
	RequestedExit=bDesktop?2:1;
	// A close request inside title Options/confirmation still belongs to the title.
	MenuHome=SystemScreen==0?2:SystemScreen==1?1:MenuHome;
	if (MenuHome==1 || (StT<0.5f && CutT<0.5f)) FinishSystemAction(false);
	else SetSystemScreen(5);
}
void AFTBrawlGameMode::FinishSystemAction(bool bDiscard)
{
#if !FTB_SESSION_CAREER
	UFTBrawlGameInstance* GI=Cast<UFTBrawlGameInstance>(GetGameInstance());
	// Preferences can fail before a campaign exists. Retry them for every orderly action,
	// including title exits; only the explicit discard choice may bypass this write.
	if (!bDiscard && GI && !GI->PreferenceError.IsEmpty() && !GI->SavePreferences())
	{ SetSystemScreen(6); return; }
	if (RequestedExit==3)
	{
		if (GI && GI->NewGame()) GI->Continue(); else SetSystemScreen(6);
		return;
	}
#endif
	bool Ok=true;
	if (!bDiscard && MenuHome!=1)
	{
		Ok=SaveCheckpoint(TEXT("Exit"));
	}
	if (!Ok) { SetSystemScreen(6); return; }
	if (RequestedExit==0) { SetSystemScreen(MenuHome==1?3:2); return; }
	UE_LOG(LogTemp,Log,TEXT("FT BRAWL SYSTEM exit=%s discard=%d"),RequestedExit==2?TEXT("desktop"):TEXT("title"),bDiscard);
	SetSystemScreen(0);
	if (RequestedExit==2)
	{
#if FTB_SESSION_CAREER
		if (UFTGameInstance* MainGI=Cast<UFTGameInstance>(GetGameInstance())) MainGI->AllowWindowClose();
#else
		if (GI) GI->AllowWindowClose();
#endif
		UKismetSystemLibrary::QuitGame(this,GetWorld()->GetFirstPlayerController(),EQuitPreference::Quit,false);
	}
	else
	{
#if FTB_SESSION_CAREER
		UFTNightFlow::ReturnToTitle(this);
#else
		if (GI) GI->ReturnToTitle();
#endif
	}
}
