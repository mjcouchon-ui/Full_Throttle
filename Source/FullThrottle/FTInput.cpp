#include "FTInput.h"
#include "FTGameInstance.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "GenericPlatform/InputDeviceRegistry.h"
#include "InputKeyEventArgs.h"
#include "HAL/PlatformTime.h"

void UFTInputSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ConnectionHandle = IPlatformInputDeviceMapper::Get().GetOnInputDeviceConnectionChange().AddUObject(this, &UFTInputSubsystem::HandleConnectionChange);
}

void UFTInputSubsystem::Deinitialize()
{
	IPlatformInputDeviceMapper::Get().GetOnInputDeviceConnectionChange().Remove(ConnectionHandle);
	Super::Deinitialize();
}

UFTInputSubsystem* UFTInputSubsystem::Get(const UObject* Context)
{
	if (const UGameInstance* GI = Cast<UGameInstance>(Context)) return GI->GetSubsystem<UFTInputSubsystem>();
	const UWorld* World = Context ? Context->GetWorld() : nullptr;
	return World && World->GetGameInstance() ? World->GetGameInstance()->GetSubsystem<UFTInputSubsystem>() : nullptr;
}

EFTControllerFamily UFTInputSubsystem::ClassifyDevice(const FString& InterfaceName, const FString& HardwareName)
{
	const FString Name = (InterfaceName + TEXT(" ") + HardwareName).ToLower();
	// The exposed interface wins. Steam Input can translate a PlayStation pad to XInput;
	// the physical controller cannot then be inferred reliably, hence the manual override.
	if (Name.Contains(TEXT("xinput"))) return EFTControllerFamily::Xbox;
	if (Name.Contains(TEXT("dualsense")) || Name.Contains(TEXT("dualshock")) || Name.Contains(TEXT("playstation")) || Name.Contains(TEXT("ps4controller")) || Name.Contains(TEXT("ps5controller"))) return EFTControllerFamily::PlayStation;
	if (Name.Contains(TEXT("nintendo")) || Name.Contains(TEXT("joycon")) || Name.Contains(TEXT("joy-con")) || Name.Contains(TEXT("switchpro")) || Name.Contains(TEXT("switch pro"))) return EFTControllerFamily::Nintendo;
	if (Name.Contains(TEXT("xbox"))) return EFTControllerFamily::Xbox;
	return EFTControllerFamily::Generic;
}

void UFTInputSubsystem::ResolveDevice(FInputDeviceId Device)
{
	DetectedInterface.Reset();
	DetectedFamily = EFTControllerFamily::Generic;
	if (!Device.IsValid()) return;
	if (const TOptional<FInputDeviceDescriptor> Descriptor = FInputDeviceRegistry::FindDescriptor(Device); Descriptor.IsSet())
	{
		DetectedInterface = Descriptor->InputDeviceName.ToString();
		DetectedFamily = ClassifyDevice(DetectedInterface, Descriptor->HardwareDeviceIdentifier.ToString());
	}
	if (DetectedFamily == EFTControllerFamily::Generic)
	{
		if (const UInputDeviceSubsystem* Devices = UInputDeviceSubsystem::Get())
		{
			const FHardwareDeviceIdentifier Hardware = Devices->GetInputDeviceHardwareIdentifier(Device);
			DetectedFamily = ClassifyDevice(Hardware.InputClassName.ToString(), Hardware.HardwareDeviceIdentifier.ToString());
			if (DetectedInterface.IsEmpty()) DetectedInterface = Hardware.InputClassName.ToString();
		}
	}
}

bool UFTInputSubsystem::IsIntentionalInput(const FInputKeyEventArgs& Params)
{
	if (Params.Key.IsGamepadKey())
	{
		// Synthetic stick-direction buttons can chatter around their platform threshold.
		if (Params.Key == EKeys::Gamepad_LeftStick_Up || Params.Key == EKeys::Gamepad_LeftStick_Down || Params.Key == EKeys::Gamepad_LeftStick_Left || Params.Key == EKeys::Gamepad_LeftStick_Right
			|| Params.Key == EKeys::Gamepad_RightStick_Up || Params.Key == EKeys::Gamepad_RightStick_Down || Params.Key == EKeys::Gamepad_RightStick_Left || Params.Key == EKeys::Gamepad_RightStick_Right) return false;
		return (Params.Event == IE_Pressed && !Params.Key.IsAxis1D() && !Params.Key.IsAxis2D())
			|| (Params.Event == IE_Axis && FMath::Abs(Params.AmountDepressed) >= .55f);
	}
	// Mouse movement alone never replaces prompts; a click or deliberate keyboard press does.
	return Params.Event == IE_Pressed && !Params.Key.IsAxis1D() && !Params.Key.IsAxis2D() && Params.Key != EKeys::AnyKey;
}

bool UFTInputSubsystem::PointerPosition(const APlayerController* PC, FVector2D& Out) const
{
	if (!PC) return false;
	if (PointerOverride.X >= 0.f)
	{
		int32 Vw = 0, Vh = 0; PC->GetViewportSize(Vw, Vh);
		Out = FVector2D(PointerOverride.X * Vw, PointerOverride.Y * Vh);
		return Vw > 0 && Vh > 0;
	}
	float X = -1.f, Y = -1.f;
	if (!PC->GetMousePosition(X, Y)) return false;
	Out = FVector2D(X, Y);
	return true;
}

void UFTInputSubsystem::ObservePointer(const APlayerController* PC)
{
	FVector2D At;
	if (!PointerPosition(PC, At)) { bPointerSeen = false; PointerTravel = 0.f; return; }
	const double Now = FPlatformTime::Seconds();
	const float Moved = bPointerSeen ? FVector2D::Distance(At, PointerLast) : 0.f;
	if (Moved > 2.f)
	{
		bPointer = true;
		// 16 Nov 2026 (the owner: "keyboard and mouse don't work when a gamepad is connected"): on a screen that takes
		// the mouse, MOVING it takes over from the controller. Until now only a click or a key did, so after any
		// controller input the pointer stayed hidden and nothing lit under it however far the mouse went. A real move,
		// not a nudge: 30 pixels of travel without a pause, and not in the moment after a controller button.
		if (Now - PointerMovedAt > .4) PointerTravel = 0.f;
		PointerTravel += Moved; PointerMovedAt = Now;
		if (bGamepad && PC->bShowMouseCursor && PointerTravel > 30.f && Now - LastDigitalInputTime > .35)
		{
			bGamepad = false;
			LastDigitalInputTime = Now;
			AxisIntentSince.Reset();
			UE_LOG(LogTemp, Display, TEXT("FTINPUT device=mouse why=moved travel=%.0f"), PointerTravel);
		}
	}
	PointerLast = At; bPointerSeen = true;
}

void UFTInputSubsystem::ObserveInput(const FInputKeyEventArgs& Params)
{
	// the pointer: a click or the wheel wakes it, a deliberate key or controller input puts it away
	if (Params.Key.IsMouseButton()) { if (Params.Event == IE_Pressed) bPointer = true; }
	else if (IsIntentionalInput(Params)) bPointer = false;
	if (!IsIntentionalInput(Params))
	{
		if (Params.InputDevice == AxisCandidate && FMath::Abs(Params.AmountDepressed) < .35f) AxisIntentSince.Remove(Params.Key);
		return;
	}
	const double Now = FPlatformTime::Seconds();
	if (Params.Key.IsGamepadKey())
	{
		if (Params.Event == IE_Axis)
		{
			// Require a sustained deliberate axis when changing input sources; an idle second
			// device or one anomalous analog event must not steal the active player's prompts.
			if (!bGamepad || ActivePad != Params.InputDevice)
			{
				if (Now - LastDigitalInputTime < .35) return;
				if (AxisCandidate != Params.InputDevice) { AxisCandidate = Params.InputDevice; AxisIntentSince.Reset(); }
				double* Since = AxisIntentSince.Find(Params.Key);
				if (!Since) { AxisIntentSince.Add(Params.Key, Now); return; }
				if (Now - *Since < .10) return;
			}
		}
		else LastDigitalInputTime = Now;
		const bool bChangedDevice = ActivePad != Params.InputDevice;
		ActivePad = Params.InputDevice;
		bGamepad = true;
		DismissDisconnect();
		if (bChangedDevice || DetectedFamily == EFTControllerFamily::Generic) ResolveDevice(ActivePad);
	}
	else
	{
		bGamepad = false;
		LastDigitalInputTime = Now;
		AxisIntentSince.Reset();
		DismissDisconnect();
	}
}

void UFTInputSubsystem::HandleConnectionChange(EInputDeviceConnectionState State, FPlatformUserId User, FInputDeviceId Device)
{
	if (Device != ActivePad || !Device.IsValid()) return;
	if (State == EInputDeviceConnectionState::Disconnected && bGamepad)
	{
		bDisconnected = true; bDisconnectPending = true;
		if (UGameInstance* GI = GetGameInstance()) if (APlayerController* PC = GI->GetFirstLocalPlayerController()) PC->FlushPressedKeys();
		UE_LOG(LogTemp, Display, TEXT("FTINPUT active controller disconnected id=%d"), Device.GetId());
	}
	else if (State == EInputDeviceConnectionState::Connected)
	{
		ResolveDevice(Device);
		// The game stays paused. Resuming requires an explicit player action.
		DismissDisconnect();
	}
}

bool UFTInputSubsystem::ConsumeDisconnect()
{
	const bool Result = bDisconnectPending;
	bDisconnectPending = false;
	return Result;
}

EFTControllerFamily UFTInputSubsystem::Family() const
{
	switch (PromptStyle)
	{
	case 1: return EFTControllerFamily::Xbox;
	case 2: return EFTControllerFamily::PlayStation;
	case 3: return EFTControllerFamily::Nintendo;
	case 4: return EFTControllerFamily::Generic;
	default: return DetectedFamily;
	}
}

const TCHAR* UFTInputSubsystem::PromptStyleName(int32 Style)
{
	static const TCHAR* Names[] = { TEXT("AUTO"), TEXT("XBOX"), TEXT("PLAYSTATION"), TEXT("NINTENDO"), TEXT("GENERIC") };
	return Names[FMath::Clamp(Style, 0, 4)];
}

void UFTInputSubsystem::SetPromptStyle(int32 Style)
{
	PromptStyle = FMath::Clamp(Style, 0, 4);
	if (UFTGameInstance* GI = Cast<UFTGameInstance>(GetGameInstance()))
	{
		if (GI->ControllerPromptStyle != PromptStyle) { GI->ControllerPromptStyle = PromptStyle; GI->SaveSettings(); }
	}
}

FString UFTInputSubsystem::DeviceLabel() const
{
	if (!ActivePad.IsValid() && !bGamepad && PromptStyle == 0) return TEXT("Keyboard / mouse - press a controller button to use it");
	switch (Family())
	{
	case EFTControllerFamily::Xbox: return DetectedInterface.Contains(TEXT("XInput"), ESearchCase::IgnoreCase) && PromptStyle == 0 ? TEXT("Xbox-style (XInput)") : TEXT("Xbox-style controller");
	case EFTControllerFamily::PlayStation: return TEXT("PlayStation-style controller");
	case EFTControllerFamily::Nintendo: return TEXT("Nintendo-style controller");
	default: return TEXT("Generic gamepad - buttons shown by position");
	}
}

FString UFTInputSubsystem::FamilyKeyLabel(EFTControllerFamily F, const FKey& Key)
{
	const TCHAR* Face[4] = { TEXT("South"), TEXT("East"), TEXT("West"), TEXT("North") };
	if (F == EFTControllerFamily::Xbox) { Face[0] = TEXT("A"); Face[1] = TEXT("B"); Face[2] = TEXT("X"); Face[3] = TEXT("Y"); }
	else if (F == EFTControllerFamily::PlayStation) { Face[0] = TEXT("Cross"); Face[1] = TEXT("Circle"); Face[2] = TEXT("Square"); Face[3] = TEXT("Triangle"); }
	else if (F == EFTControllerFamily::Nintendo) { Face[0] = TEXT("B"); Face[1] = TEXT("A"); Face[2] = TEXT("Y"); Face[3] = TEXT("X"); }
	if (Key == EKeys::Gamepad_FaceButton_Bottom) return Face[0];
	if (Key == EKeys::Gamepad_FaceButton_Right) return Face[1];
	if (Key == EKeys::Gamepad_FaceButton_Left) return Face[2];
	if (Key == EKeys::Gamepad_FaceButton_Top) return Face[3];
	if (Key == EKeys::Gamepad_LeftShoulder) return F == EFTControllerFamily::PlayStation ? TEXT("L1") : F == EFTControllerFamily::Nintendo ? TEXT("L") : TEXT("LB");
	if (Key == EKeys::Gamepad_RightShoulder) return F == EFTControllerFamily::PlayStation ? TEXT("R1") : F == EFTControllerFamily::Nintendo ? TEXT("R") : TEXT("RB");
	if (Key == EKeys::Gamepad_LeftTrigger || Key == EKeys::Gamepad_LeftTriggerAxis) return F == EFTControllerFamily::PlayStation ? TEXT("L2") : F == EFTControllerFamily::Nintendo ? TEXT("ZL") : TEXT("LT");
	if (Key == EKeys::Gamepad_RightTrigger || Key == EKeys::Gamepad_RightTriggerAxis) return F == EFTControllerFamily::PlayStation ? TEXT("R2") : F == EFTControllerFamily::Nintendo ? TEXT("ZR") : TEXT("RT");
	if (Key == EKeys::Gamepad_LeftThumbstick) return F == EFTControllerFamily::PlayStation ? TEXT("L3") : TEXT("LS click");
	if (Key == EKeys::Gamepad_RightThumbstick) return F == EFTControllerFamily::PlayStation ? TEXT("R3") : TEXT("RS click");
	if (Key == EKeys::Gamepad_LeftX || Key == EKeys::Gamepad_LeftY) return TEXT("LS");
	if (Key == EKeys::Gamepad_RightX || Key == EKeys::Gamepad_RightY) return TEXT("RS");
	if (Key == EKeys::Gamepad_Special_Right) return F == EFTControllerFamily::PlayStation ? TEXT("Options") : F == EFTControllerFamily::Nintendo ? TEXT("+") : TEXT("Menu");
	if (Key == EKeys::Gamepad_Special_Left) return F == EFTControllerFamily::PlayStation ? TEXT("Share") : F == EFTControllerFamily::Nintendo ? TEXT("-") : TEXT("View");
	if (Key == EKeys::Gamepad_DPad_Left || Key == EKeys::Gamepad_LeftStick_Left) return TEXT("D-pad Left");
	if (Key == EKeys::Gamepad_DPad_Right || Key == EKeys::Gamepad_LeftStick_Right) return TEXT("D-pad Right");
	if (Key == EKeys::Gamepad_DPad_Up || Key == EKeys::Gamepad_LeftStick_Up) return TEXT("D-pad Up");
	if (Key == EKeys::Gamepad_DPad_Down || Key == EKeys::Gamepad_LeftStick_Down) return TEXT("D-pad Down");
	if (Key == EKeys::LeftShift || Key == EKeys::RightShift) return TEXT("Shift");
	if (Key == EKeys::LeftControl || Key == EKeys::RightControl) return TEXT("Ctrl");
	if (Key == EKeys::SpaceBar) return TEXT("Space");
	if (Key == EKeys::LeftMouseButton) return TEXT("LMB");
	if (Key == EKeys::RightMouseButton) return TEXT("RMB");
	if (Key == EKeys::MiddleMouseButton) return TEXT("MMB");
	if (Key == EKeys::Escape) return TEXT("Esc");
	return Key.IsValid() ? Key.GetDisplayName().ToString() : TEXT("Unbound");
}

FString UFTInputSubsystem::KeyLabel(const FKey& Key) const { return FamilyKeyLabel(Family(), Key); }

FKey UFTInputSubsystem::RacePadKey(EFTAction Action)
{
	switch (Action)
	{
	case EFTAction::Left: case EFTAction::Right: return EKeys::Gamepad_LeftX;
	case EFTAction::Up: return EKeys::Gamepad_DPad_Up;
	case EFTAction::Down: return EKeys::Gamepad_LeftTriggerAxis;
	case EFTAction::Drift: return EKeys::Gamepad_FaceButton_Bottom;
	case EFTAction::Turbo: return EKeys::Gamepad_RightTriggerAxis;
	case EFTAction::Fire: return EKeys::Gamepad_FaceButton_Left;
	case EFTAction::Missile: return EKeys::Gamepad_FaceButton_Right;
	case EFTAction::Ammo: return EKeys::Gamepad_FaceButton_Top;
	default: return FKey();
	}
}

FKey UFTInputSubsystem::BrawlPadKey(EFTBrawlAction Action)
{
	switch (Action)
	{
	case EFTBrawlAction::Move: return EKeys::Gamepad_LeftX;
	case EFTBrawlAction::Punch: return EKeys::Gamepad_FaceButton_Left;
	case EFTBrawlAction::Kick: return EKeys::Gamepad_FaceButton_Right;
	case EFTBrawlAction::Jump: case EFTBrawlAction::Confirm: return EKeys::Gamepad_FaceButton_Bottom;
	case EFTBrawlAction::GrabThrow: return EKeys::Gamepad_FaceButton_Top;
	case EFTBrawlAction::Pause: return EKeys::Gamepad_Special_Right;
	case EFTBrawlAction::Back: return EKeys::Gamepad_FaceButton_Right;
	default: return FKey();
	}
}

FString UFTInputSubsystem::RacePrompt(EFTAction Action) const
{
	if (bGamepad)
	{
		if (Action == EFTAction::Left || Action == EFTAction::Right) return TEXT("LS / D-pad");
		if (Action == EFTAction::Drift) return KeyLabel(RacePadKey(Action)) + TEXT(" / ") + KeyLabel(EKeys::Gamepad_RightShoulder);
		if (Action == EFTAction::Turbo) return KeyLabel(RacePadKey(Action)) + TEXT(" / ") + KeyLabel(EKeys::Gamepad_LeftShoulder);
		return KeyLabel(RacePadKey(Action));
	}
	const UFTGameInstance* GI = Cast<UFTGameInstance>(GetGameInstance());
	return GI && int32(Action) < UFTGameInstance::NumActions ? KeyLabel(GI->Binds[int32(Action)][FMath::Clamp(GI->Layout, 0, 1)]) : TEXT("Unbound");
}

FString UFTInputSubsystem::BrawlPrompt(EFTBrawlAction Action) const
{
	if (bGamepad) return Action == EFTBrawlAction::Move ? TEXT("LS / D-pad") : KeyLabel(BrawlPadKey(Action));
	switch (Action)
	{
	case EFTBrawlAction::Move: return RacePrompt(EFTAction::Left) + TEXT(" / ") + RacePrompt(EFTAction::Right) + TEXT(" / ") + RacePrompt(EFTAction::Up) + TEXT(" / ") + RacePrompt(EFTAction::Down);
	case EFTBrawlAction::Punch: return RacePrompt(EFTAction::Fire);
	case EFTBrawlAction::Kick: return RacePrompt(EFTAction::Missile);
	case EFTBrawlAction::GrabThrow: return RacePrompt(EFTAction::Ammo);
	case EFTBrawlAction::Jump: return KeyLabel(BrawlKeyboardKey(Action));
	case EFTBrawlAction::Pause: case EFTBrawlAction::Back: return TEXT("Esc");
	case EFTBrawlAction::Confirm: return TEXT("Enter");
	default: return FString();
	}
}

FKey UFTInputSubsystem::BrawlKeyboardKey(EFTBrawlAction Action) const
{
	const UFTGameInstance* GI = Cast<UFTGameInstance>(GetGameInstance());
	auto Bound = [&](EFTAction A, const FKey& Fallback)
	{
		return GI ? GI->Binds[int32(A)][FMath::Clamp(GI->Layout, 0, 1)] : Fallback;
	};
	switch (Action)
	{
	case EFTBrawlAction::Punch: return Bound(EFTAction::Fire, EKeys::J);
	case EFTBrawlAction::Kick: return Bound(EFTAction::Missile, EKeys::K);
	case EFTBrawlAction::GrabThrow: return Bound(EFTAction::Ammo, EKeys::L);
	case EFTBrawlAction::Pause: case EFTBrawlAction::Back: return EKeys::Escape;
	case EFTBrawlAction::Confirm: return EKeys::Enter;
	case EFTBrawlAction::Jump:
		// Space remains the familiar default. Rebinding a combat or movement action
		// onto Space must not simultaneously jump; both prompts and gameplay use this resolver.
		for (const FKey Candidate : { EKeys::SpaceBar, EKeys::LeftShift, EKeys::E, EKeys::U, EKeys::V, EKeys::N, EKeys::B, EKeys::H })
		{
			bool bUsed = false;
			if (GI) for (EFTAction A : { EFTAction::Left, EFTAction::Right, EFTAction::Up, EFTAction::Down, EFTAction::Fire, EFTAction::Missile, EFTAction::Ammo })
				if (Bound(A, FKey()) == Candidate) bUsed = true;
			if (!bUsed) return Candidate;
		}
		return EKeys::SpaceBar; // Eight candidates, at most seven assigned brawl actions.
	default: return FKey();
	}
}
