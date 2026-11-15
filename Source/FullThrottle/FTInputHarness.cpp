// Development-only input checks travel through the real player-controller input path.
#if !UE_BUILD_SHIPPING
#include "FTInput.h"
#include "FTGameMode.h"
#include "FTGameInstance.h"
#include "FTPlayerController.h"
#include "FTProgress.h"
#include "Brawl/FTBrawlGameMode.h"
#include "Brawl/FTBrawlPlayerController.h"
#include "Engine/GameViewportClient.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SViewport.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GenericPlatform/InputDeviceRegistry.h"
#include "HAL/IConsoleManager.h"
#include "InputKeyEventArgs.h"
#include "TimerManager.h"
#include "Containers/Ticker.h"
#include "Kismet/GameplayStatics.h"

namespace
{
	FInputDeviceId HarnessDevice() { return FInputDeviceId::CreateFromInternalId(731); }
	struct FHeldAxis { TWeakObjectPtr<UWorld> World; FTimerHandle Timer; FTSTicker::FDelegateHandle NeutralTick; };
	TMap<FKey, FHeldAxis> HeldAxes;
	void ReleaseAxis(const FKey& Key)
	{
		if (FHeldAxis* Held = HeldAxes.Find(Key))
		{
			if (Held->World.IsValid()) Held->World->GetTimerManager().ClearTimer(Held->Timer);
			if (Held->NeutralTick.IsValid()) FTSTicker::RemoveTicker(Held->NeutralTick);
			HeldAxes.Remove(Key);
		}
	}
	void Simulate(APlayerController* PC, const FKey& Key, EInputEvent Event, float Value)
	{
		if (PC) PC->InputKey(FInputKeyEventArgs::CreateSimulated(Key, Event, Value, -1, Key.IsGamepadKey() ? HarnessDevice() : INPUTDEVICEID_NONE));
	}
	FAutoConsoleCommandWithWorldAndArgs Hold(TEXT("ft.InputHold"), TEXT("Development: hold a key or analog axis through real input: <Key> [value]"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		APlayerController* PC = World ? World->GetFirstPlayerController() : nullptr;
		if (!PC || Args.IsEmpty()) return;
		const FKey Key(*Args[0]); if (!Key.IsValid()) { UE_LOG(LogTemp, Error, TEXT("FTINPUT invalid key %s"), *Args[0]); return; }
		const float Value = Args.Num() > 1 ? FMath::Clamp(FCString::Atof(*Args[1]), -1.f, 1.f) : 1.f;
		ReleaseAxis(Key);
		const bool bAxis = Key.IsAxis1D();
		Simulate(PC, Key, bAxis ? IE_Axis : IE_Pressed, Value);
		if (bAxis)
		{
			// Real hardware produces nonzero analog samples continuously; preserve that
			// behavior so intentional-input hysteresis is exercised by the harness too.
			FHeldAxis& Held = HeldAxes.Add(Key); Held.World = World;
			TWeakObjectPtr<APlayerController> WeakPC(PC);
			World->GetTimerManager().SetTimer(Held.Timer, FTimerDelegate::CreateLambda([WeakPC, Key, Value]() { if (WeakPC.IsValid()) Simulate(WeakPC.Get(), Key, IE_Axis, Value); }), .016f, true);
		}
		UE_LOG(LogTemp, Display, TEXT("FTINPUT hold %s value=%.3f synthetic=%d"), *Key.ToString(), Value, HarnessDevice().GetId());
	}));
	FAutoConsoleCommandWithWorldAndArgs Release(TEXT("ft.InputRelease"), TEXT("Development: release a held key or axis: <Key>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		if (Args.IsEmpty()) return;
		const FKey Key(*Args[0]); ReleaseAxis(Key);
		APlayerController* PC = World ? World->GetFirstPlayerController() : nullptr;
		Simulate(PC, Key, Key.IsAxis1D() ? IE_Axis : IE_Released, 0.f);
		if (PC && Key.IsAxis1D())
		{
			// PlayerInput sums analog samples within a frame and retains the last raw
			// value when no new sample arrives. A world-timer hold and this core-ticker
			// release can therefore leave (+value + 0) in the same accumulator. Repeat
			// neutral across processing frames, as separate hardware polls would, rather
			// than clearing unrelated player input. A fresh hold cancels this ticker.
			FHeldAxis& Neutral = HeldAxes.Add(Key); Neutral.World = World;
			TWeakObjectPtr<APlayerController> WeakPC(PC);
			Neutral.NeutralTick = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([WeakPC, Key, Remaining = 3](float) mutable
			{
				if (!WeakPC.IsValid()) return false;
				Simulate(WeakPC.Get(), Key, IE_Axis, 0.f);
				return --Remaining > 0;
			}));
		}
		UE_LOG(LogTemp, Display, TEXT("FTINPUT release %s"), *Key.ToString());
	}));
	FAutoConsoleCommandWithWorldAndArgs Style(TEXT("ft.ControllerStyle"), TEXT("Development: prompt override 0 Auto, 1 Xbox, 2 PS, 3 Nintendo, 4 Generic"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		if (UFTInputSubsystem* Input = UFTInputSubsystem::Get(World); Input && !Args.IsEmpty()) { Input->SetPromptStyle(FCString::Atoi(*Args[0])); UE_LOG(LogTemp, Display, TEXT("FTINPUT style=%d"), Input->GetPromptStyle()); }
	}));
	FAutoConsoleCommandWithWorldAndArgs Focus(TEXT("ft.InputFocus"), TEXT("Development: exercise Slate viewport focus loss/return, 0 lost or 1 focused; does not synthesize key releases"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		UGameViewportClient* Viewport=World ? World->GetGameViewport() : nullptr;
		if (!Viewport || !FSlateApplication::IsInitialized() || Args.Num()!=1) { UE_LOG(LogTemp,Error,TEXT("FTINPUT focus requires a game viewport and 0/1 argument")); return; }
		FSlateApplication& Slate=FSlateApplication::Get();
		const TSharedPtr<SViewport> Widget=Viewport->GetGameViewportWidget();
		if (!Widget.IsValid()) { UE_LOG(LogTemp,Error,TEXT("FTINPUT focus requires a Slate viewport widget")); return; }
		if (FCString::Atoi(*Args[0])!=0) Slate.SetUserFocus(Slate.GetUserIndexForKeyboard(),Widget,EFocusCause::SetDirectly);
		else Slate.ClearKeyboardFocus(EFocusCause::SetDirectly);
		UE_LOG(LogTemp,Display,TEXT("FTINPUT viewport focus=%d"),Slate.GetKeyboardFocusedWidget().Get()==Widget.Get());
	}));
	FAutoConsoleCommandWithWorldAndArgs Device(TEXT("ft.InputDevice"), TEXT("Development: exposed synthetic device descriptor xbox/playstation/nintendo/generic; never changes real device metadata"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		if (Args.IsEmpty()) return;
		FInputDeviceDescriptor Descriptor; Descriptor.HardwareDeviceHandle = HarnessDevice(); Descriptor.InputDeviceName = TEXT("FTAutomation");
		const FString Kind = Args[0].ToLower();
		if (Kind == TEXT("xbox")) { Descriptor.InputDeviceName = TEXT("XInputInterface"); Descriptor.HardwareDeviceIdentifier = TEXT("XInputController"); }
		else if (Kind == TEXT("playstation")) Descriptor.HardwareDeviceIdentifier = TEXT("DualSense");
		else if (Kind == TEXT("nintendo")) Descriptor.HardwareDeviceIdentifier = TEXT("Nintendo Switch Pro Controller");
		else Descriptor.HardwareDeviceIdentifier = TEXT("Generic Gamepad");
		FInputDeviceRegistry::SetSimulatedDescriptor(HarnessDevice(), Descriptor);
		if (UFTInputSubsystem* Input = UFTInputSubsystem::Get(World); Input && Input->ActiveDevice() == HarnessDevice()) Input->HandleConnectionChange(EInputDeviceConnectionState::Connected, PLATFORMUSERID_NONE, HarnessDevice());
		UE_LOG(LogTemp, Display, TEXT("FTINPUT synthetic descriptor=%s"), *Kind);
	}));
	FAutoConsoleCommandWithWorldAndArgs Status(TEXT("ft.InputStatus"), TEXT("Development: print current prompt and device state"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		if (const UFTInputSubsystem* Input = UFTInputSubsystem::Get(World)) UE_LOG(LogTemp, Display, TEXT("FTINPUT status gamepad=%d style=%d family=%d active=%d disconnected=%d label=%s"), Input->UsesGamepad(), Input->GetPromptStyle(), int32(Input->Family()), Input->ActiveDevice().GetId(), Input->HasDisconnectedController(), *Input->DeviceLabel());
	}));
	TMap<FString, int32> ControlFields(UWorld* World)
	{
		TMap<FString, int32> Fields;
		if (!World) return Fields;
		Fields.Add(TEXT("worldpaused"), UGameplayStatics::IsGamePaused(World));
		if (const UFTCareerSubsystem* Career = UFTCareerSubsystem::Get(World))
		{
			Fields.Add(TEXT("careerpending"), Career->C.PendingLevel);
			Fields.Add(TEXT("hascontinue"), Career->HasContinue());
			Fields.Add(TEXT("saveerror"), Career->bSaveError);
			Fields.Add(TEXT("savepending"), Career->bSaving);
		}
		if (const AFTGameMode* GM = World->GetAuthGameMode<AFTGameMode>())
		{
			Fields.Add(TEXT("phase"), int32(GM->Phase)); Fields.Add(TEXT("overlay"), int32(GM->Overlay));
			Fields.Add(TEXT("title"), GM->Phase == EFTPhase::Title || GM->bAttract);
			Fields.Add(TEXT("systemdialog"), int32(GM->SystemDialog)); Fields.Add(TEXT("systemrow"), GM->SystemRow);
			Fields.Add(TEXT("titleactionfocus"), GM->TitleActionFocus); Fields.Add(TEXT("pauserow"), GM->PauseRow);
			Fields.Add(TEXT("titleselection"), GM->TitleSel);
			Fields.Add(TEXT("systemaction"), GM->SystemAction);
			Fields.Add(TEXT("pendingsystemsave"), GM->bPendingSystemSave);
			Fields.Add(TEXT("controlspage"), GM->ControlsPage); Fields.Add(TEXT("controlsrow"), GM->ControlsRow);
			Fields.Add(TEXT("tutorial"), int32(GM->Tutorial.Step)); Fields.Add(TEXT("actionobserved"), GM->Tutorial.bActionObserved);
			Fields.Add(TEXT("tutorialwrappending"), GM->bTutorialWrapPending); Fields.Add(TEXT("bosslastchance"), GM->bLastChance);
			Fields.Add(TEXT("missiles"), GM->Missiles); Fields.Add(TEXT("turbo"), FMath::RoundToInt(GM->Turbo));
			Fields.Add(TEXT("drift"), GM->bDrifting); Fields.Add(TEXT("ammoload"), GM->AmmoLoad);
		}
		if (const UFTGameInstance* GI = Cast<UFTGameInstance>(World->GetGameInstance()))
		{
			Fields.Add(TEXT("muted"), GI->bMuted);
			Fields.Add(TEXT("tutorialcompleted"), GI->bTutorialCompleted); Fields.Add(TEXT("tutorialskipped"), GI->bTutorialSkipped);
			Fields.Add(TEXT("brawlhelpseen"), GI->bBrawlHelpSeen);
		}
		if (const AFTBrawlGameMode* Brawl = World->GetAuthGameMode<AFTBrawlGameMode>())
		{
			Fields.Add(TEXT("brawlpage"), Brawl->PausePage); Fields.Add(TEXT("brawlselection"), Brawl->PauseSelection);
			Fields.Add(TEXT("brawlsystem"), Brawl->SystemScreen); Fields.Add(TEXT("brawlsystemselection"), Brawl->SystemSelection);
			Fields.Add(TEXT("brawlhelp"), Brawl->bShowCombatHelp); Fields.Add(TEXT("brawlcut"), Brawl->CutId);
		}
		if (const AFTBrawlPlayerController* PC=Cast<AFTBrawlPlayerController>(World->GetFirstPlayerController()))
			Fields.Add(TEXT("brawlheldaction"),PC->HasHeldActionInput());
		if (const UFTInputSubsystem* Input = UFTInputSubsystem::Get(World))
		{
			Fields.Add(TEXT("gamepad"), Input->UsesGamepad()); Fields.Add(TEXT("style"), Input->GetPromptStyle());
			Fields.Add(TEXT("family"), int32(Input->Family())); Fields.Add(TEXT("disconnected"), Input->HasDisconnectedController());
		}
		return Fields;
	}
	FAutoConsoleCommandWithWorldAndArgs ControlState(TEXT("ft.ControlState"), TEXT("Development: log actual control, tutorial and persistence state without changing it"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		const TMap<FString, int32> Fields = ControlFields(World);
		TArray<FString> Keys; Fields.GetKeys(Keys); Keys.Sort(); FString Values;
		for (const FString& Key : Keys) Values += FString::Printf(TEXT(" %s=%d"), *Key, Fields[Key]);
		if (const AFTGameMode* GM = World ? World->GetAuthGameMode<AFTGameMode>() : nullptr) Values += FString::Printf(TEXT(" tutorialElapsed=%.3f stepElapsed=%.3f turboExact=%.3f driftCharge=%.3f"), GM->Tutorial.TotalElapsed, GM->Tutorial.StepElapsed, GM->Turbo, GM->DriftCharge);
		if (const AFTPlayerController* PC = World ? Cast<AFTPlayerController>(World->GetFirstPlayerController()) : nullptr) Values += FString::Printf(TEXT(" inSteer=%.3f inFire=%d inTurbo=%d inMissile=%d inDrift=%d inAmmo=%d"), PC->In.RawSteer, PC->In.bFire, PC->In.bTurbo, PC->In.bMissile, PC->In.bDrift, PC->In.bAmmo);
		UE_LOG(LogTemp, Display, TEXT("FTCONTROL state%s"), *Values);
	}));
	FAutoConsoleCommandWithWorldAndArgs AssertControl(TEXT("ft.AssertControl"), TEXT("Development: assert a live control field equals an integer: <field> <expected>"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		if (Args.Num() != 2) { UE_LOG(LogTemp, Error, TEXT("FTCONTROL FAIL usage: ft.AssertControl <field> <int>")); return; }
		const TMap<FString, int32> Fields = ControlFields(World);
		const int32* Actual = Fields.Find(Args[0].ToLower()); const int32 Expected = FCString::Atoi(*Args[1]);
		if (!Actual) { UE_LOG(LogTemp, Error, TEXT("FTCONTROL FAIL unknown/unavailable field=%s"), *Args[0]); }
		else if (*Actual != Expected) { UE_LOG(LogTemp, Error, TEXT("FTCONTROL FAIL %s expected=%d actual=%d"), *Args[0], Expected, *Actual); }
		else { UE_LOG(LogTemp, Display, TEXT("FTCONTROL PASS %s=%d"), *Args[0], *Actual); }
	}));
	void Connection(UWorld* World, EInputDeviceConnectionState State)
	{
		if (UFTInputSubsystem* Input = UFTInputSubsystem::Get(World); Input && Input->ActiveDevice() == HarnessDevice())
		{
			if (State == EInputDeviceConnectionState::Disconnected)
			{
				TArray<FKey> Keys; HeldAxes.GetKeys(Keys); for (const FKey& Key : Keys) ReleaseAxis(Key);
			}
			Input->HandleConnectionChange(State, PLATFORMUSERID_NONE, HarnessDevice());
			UE_LOG(LogTemp, Display, TEXT("FTINPUT synthetic connection=%s"), State == EInputDeviceConnectionState::Connected ? TEXT("connected") : TEXT("disconnected"));
		}
		else { UE_LOG(LogTemp, Warning, TEXT("FTINPUT connection test requires an active ft.InputHold gamepad device; real devices untouched")); }
	}
	FAutoConsoleCommandWithWorldAndArgs Disconnect(TEXT("ft.InputDisconnect"), TEXT("Development: disconnect active synthetic controller only"), FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World) { Connection(World, EInputDeviceConnectionState::Disconnected); }));
	FAutoConsoleCommandWithWorldAndArgs Reconnect(TEXT("ft.InputReconnect"), TEXT("Development: reconnect active synthetic controller only"), FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World) { Connection(World, EInputDeviceConnectionState::Connected); }));
}
#endif
