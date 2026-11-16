#include "FTPlayerController.h"
#include "FTGameInstance.h"
#include "FTGameMode.h"
#include "FTInput.h"
#include "FTCursor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "InputKeyEventArgs.h"
#include "HAL/IConsoleManager.h"
#include "Containers/Ticker.h"
#include "EngineGlobals.h"

// test hooks that go through the player's real input path (WasInputKeyJustPressed sees them like a key):
//   ft.Press <Key>          press and release a key, mouse or pad button (Enter, SpaceBar, Gamepad_Special_Right, ...)
//   ft.Click <x> <y> [Right] move the cursor to viewport pixel x,y and click there
namespace
{
#if !UE_BUILD_SHIPPING
	TAutoConsoleVariable<int32> GFTControllerEvents(TEXT("ft.ControllerEvents"), 0,
		TEXT("Log controller button/stick transitions; simulated input is explicitly labelled."));
	FAutoConsoleCommandWithWorldAndArgs GFTAxis(TEXT("ft.Axis"), TEXT("QA only: simulated Gamepad_LeftX/Y axis value, -1 to 1; not physical controller evidence"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
		{
			APlayerController* PC = World ? World->GetFirstPlayerController() : nullptr;
			if (!PC || Args.Num() < 2) return;
			const FKey Key(*Args[0]);
			if (Key != EKeys::Gamepad_LeftX && Key != EKeys::Gamepad_LeftY) return;
			const float Value = FMath::Clamp(FCString::Atof(*Args[1]), -1.f, 1.f);
			PC->InputKey(FInputKeyEventArgs::CreateSimulated(Key, IE_Axis, Value, 1));
			UE_LOG(LogTemp, Display, TEXT("FTINPUT axis key=%s value=%.3f simulated=1"), *Key.ToString(), Value);
		}));
#endif
	void FTTap(UWorld* World, const FKey& K)
	{
		APlayerController* PC = World ? World->GetFirstPlayerController() : nullptr;
		if (!PC || !K.IsValid()) { UE_LOG(LogTemp, Warning, TEXT("ft.Press: no player or bad key")); return; }
		PC->InputKey(FInputKeyEventArgs::CreateSimulated(K, IE_Pressed, 1.f));
		TWeakObjectPtr<APlayerController> Weak(PC);
		FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([Weak, K](float)
		{
			if (Weak.IsValid()) Weak->InputKey(FInputKeyEventArgs::CreateSimulated(K, IE_Released, 0.f));
			return false;
		}), 0.12f);
		UE_LOG(LogTemp, Display, TEXT("FTINPUT tap %s"), *K.ToString());
	}
	FAutoConsoleCommandWithWorldAndArgs GFTPress(TEXT("ft.Press"), TEXT("FULL THROTTLE tests: press and release a key through the player's input"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World) { if (Args.Num() > 0) FTTap(World, FKey(*Args[0])); }));
	FAutoConsoleCommandWithWorldAndArgs GFTClick(TEXT("ft.Click"), TEXT("FULL THROTTLE tests: move the cursor to viewport x y and click (add Right for the right button)"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
		{
			APlayerController* PC = World ? World->GetFirstPlayerController() : nullptr;
			if (!PC || Args.Num() < 2) return;
			PC->SetMouseLocation(FCString::Atoi(*Args[0]), FCString::Atoi(*Args[1]));
			float X = 0, Y = 0; PC->GetMousePosition(X, Y);
			UE_LOG(LogTemp, Display, TEXT("FTINPUT cursor at %.0f,%.0f"), X, Y);
			FTTap(World, Args.Num() > 2 && Args[2].Equals(TEXT("Right"), ESearchCase::IgnoreCase) ? EKeys::RightMouseButton : EKeys::LeftMouseButton);
		}));
#if !UE_BUILD_SHIPPING
	// A test run (-RenderOffScreen) has no real mouse: ft.MouseAt puts the pointer at a fraction of the viewport, for
	// every screen that reads it; ft.Press / ft.InputHold LeftMouseButton and ft.Press MouseScrollDown then click and scroll.
	FAutoConsoleCommandWithWorldAndArgs GFTMouseAt(TEXT("ft.MouseAt"), TEXT("FULL THROTTLE tests: pointer at <x 0..1> <y 0..1> of the viewport; 'off' gives it back to the real mouse"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
		{
			UFTInputSubsystem* Input = UFTInputSubsystem::Get(World);
			if (!Input || Args.IsEmpty()) return;
			FVector2D At = Args.Num() >= 2 ? FVector2D(FCString::Atof(*Args[0]), FCString::Atof(*Args[1])) : FVector2D(-1.f, -1.f);
			// ft.MouseAt row|btn|ov <n>: the middle of the title's row n, the title's button n or the open overlay's control n
			const AFTGameMode* GM = World ? World->GetAuthGameMode<AFTGameMode>() : nullptr;
			const APlayerController* PC = World ? World->GetFirstPlayerController() : nullptr;
			if (GM && PC && Args.Num() >= 2 && !FChar::IsDigit(Args[0][0]))
			{
				const TArray<FBox2D>& Rects = Args[0] == TEXT("row") ? GM->TitleRowRects : Args[0] == TEXT("btn") ? GM->TitleBtnRects : GM->OverlayRects;
				const int32 N = FCString::Atoi(*Args[1]);
				int32 Vw = 0, Vh = 0; PC->GetViewportSize(Vw, Vh);
				if (!Rects.IsValidIndex(N) || !Rects[N].bIsValid || Vw <= 0 || Vh <= 0) { UE_LOG(LogTemp, Warning, TEXT("FTINPUT pointer: no %s %d"), *Args[0], N); return; }
				At = Rects[N].GetCenter() / FVector2D(Vw, Vh);
			}
			Input->SetPointerOverride(At);
			UE_LOG(LogTemp, Display, TEXT("FTINPUT pointer override %.3f,%.3f"), At.X, At.Y);
		}));
#endif
}

void AFTCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	if (AFTGameMode* GM = GetWorld() ? GetWorld()->GetAuthGameMode<AFTGameMode>() : nullptr)
	{
		FMinimalViewInfo POV = OutVT.POV;
		if (GM->ComputeCamera(POV, DeltaTime))
		{
			OutVT.POV = POV;
			return;
		}
	}
	Super::UpdateViewTarget(OutVT, DeltaTime);
}

AFTPlayerController::AFTPlayerController()
{
	PrimaryActorTick.bTickEvenWhenPaused = true;
	bShouldPerformFullTickWhenPaused = true;
	PlayerCameraManagerClass = AFTCameraManager::StaticClass();
	bShowMouseCursor = false;
}

void AFTPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// 3.9 (owner: "clicks not registering"): the game mode's BeginPlay runs before this one, and a screen it opens there
	// (PRE-PRODUCTION after a night, or reopened on launch) has already asked for the cursor. Forcing game-only here took the
	// cursor's UI mode away again (mouse captured, clicks on READY / BUY / tabs not read) while bMenuMouse said it was on,
	// so every later SetMenuMouse(true) returned early. Apply whatever mode was asked for instead.
	ApplyMouseMode(TEXT("beginplay"));
	// A native controller can report a held Cross/A again after map travel.
	// Require release before the arriving screen accepts a fresh confirmation.
	BlockMenuInputUntilRelease();
}

void AFTPlayerController::ApplyMouseMode(const TCHAR* Why)
{
	SetShowMouseCursor(bMenuMouse);
	if (bMenuMouse) FTCursor::HideSystemCursor(this);
	if (UFTInputSubsystem* Input = UFTInputSubsystem::Get(this)) Input->ResyncPointer();
	if (bMenuMouse)
	{
		FInputModeGameAndUI Mode;
		Mode.SetHideCursorDuringCapture(false);
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(Mode);
	}
	else SetInputMode(FInputModeGameOnly());
	UE_LOG(LogTemp, Display, TEXT("FTINPUT mode=%s cursor=%d why=%s"), bMenuMouse ? TEXT("menu") : TEXT("game"), bMenuMouse ? 1 : 0, Why);
}

void AFTPlayerController::SetMenuMouse(bool bOn)
{
	if (bOn == bMenuMouse) return;
	bMenuMouse = bOn;
	SetShowMouseCursor(bOn);
	if (bOn) FTCursor::HideSystemCursor(this);
	if (UFTInputSubsystem* Input = UFTInputSubsystem::Get(this)) Input->ResyncPointer();
	if (bOn)
	{
		FInputModeGameAndUI Mode;
		Mode.SetHideCursorDuringCapture(false);
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(Mode);
	}
	else SetInputMode(FInputModeGameOnly());
	UE_LOG(LogTemp, Display, TEXT("FTINPUT mode=%s cursor=%d why=screen"), bOn ? TEXT("menu") : TEXT("game"), bOn ? 1 : 0);
}

FVector2D AFTPlayerController::MousePos() const
{
	FVector2D At;
	const UFTInputSubsystem* Input = UFTInputSubsystem::Get(this);
	if (!Input || !Input->PointerPosition(this, At)) return FVector2D(-1.f, -1.f);
	return At;
}

bool AFTPlayerController::PointerShown() const
{
	const UFTInputSubsystem* Input = UFTInputSubsystem::Get(this);
	return bMenuMouse && Input && Input->PointerActive() && !Input->UsesGamepad();
}

bool AFTPlayerController::InputKey(const FInputKeyEventArgs& Params)
{
	if (UFTInputSubsystem* Input = UFTInputSubsystem::Get(this)) Input->ObserveInput(Params);
#if !UE_BUILD_SHIPPING
	if (GFTControllerEvents.GetValueOnGameThread() && Params.Key.IsGamepadKey())
	{
		bool bLog = Params.Event == IE_Pressed || Params.Event == IE_Released;
		if (Params.Event == IE_Axis && (Params.Key == EKeys::Gamepad_LeftX || Params.Key == EKeys::Gamepad_LeftY))
		{
			int8& Previous = Params.Key == EKeys::Gamepad_LeftX ? LoggedStickX : LoggedStickY;
			const int8 Direction = Params.AmountDepressed > .5f ? 1 : Params.AmountDepressed < -.5f ? -1 : 0;
			bLog = Direction != Previous; Previous = Direction;
		}
		if (bLog) UE_LOG(LogTemp, Display, TEXT("FT PAD event key=%s event=%d value=%.3f simulated=%d"),
			*Params.Key.ToString(), int32(Params.Event), Params.AmountDepressed, Params.IsSimulatedInput() ? 1 : 0);
	}
#endif
	return Super::InputKey(Params);
}

bool AFTPlayerController::UsesGamepadPrompts() const
{
	const UFTInputSubsystem* Input = UFTInputSubsystem::Get(this);
	return Input && Input->UsesGamepad();
}

void AFTPlayerController::SuppressGameplayUntilRelease()
{
	bSuppressGameplay = true;
	In = FFTInput();
}

void AFTPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PostProcessInput(DeltaTime, bGamePaused);
	// Some native GameInput devices expose axes without the virtual stick buttons.
	// Sample every input tick, including racing and pause, so a held stick does not
	// create a fresh menu press when a screen opens.
	UpdateMenuStickDirections();
}

void AFTPlayerController::UpdateMenuStickDirections() const
{
	if (MenuStickFrame == GFrameCounter) return;
	MenuStickFrame = GFrameCounter;
	MenuStickPressed = 0;
	const float X = GetInputAnalogKeyState(EKeys::Gamepad_LeftX);
	const float Y = GetInputAnalogKeyState(EKeys::Gamepad_LeftY);
	const float Amounts[] = { -X, X, Y, -Y };
	const FKey Keys[] = { EKeys::Gamepad_LeftStick_Left, EKeys::Gamepad_LeftStick_Right,
		EKeys::Gamepad_LeftStick_Up, EKeys::Gamepad_LeftStick_Down };
	uint8 Held = 0;
	for (int32 I = 0; I < UE_ARRAY_COUNT(Keys); ++I)
	{
		const uint8 Bit = uint8(1 << I);
		const bool bWasHeld = (MenuStickHeld & Bit) != 0;
		const bool bDigital = IsInputKeyDown(Keys[I]) || WasInputKeyJustPressed(Keys[I]);
		const bool bHeld = bDigital || Amounts[I] >= (bWasHeld ? .35f : .55f);
		if (bHeld) Held |= Bit;
		// Merge the physical episode, not two event streams: when a driver emits
		// both digital and axis input, crossing the other threshold cannot step twice.
		if (bHeld && !bWasHeld) MenuStickPressed |= Bit;
	}
	MenuStickHeld = Held;
}

bool AFTPlayerController::Hit(const FKey& K) const
{
	if (bReleaseMenuInput) return false;
	const int32 Direction = K == EKeys::Gamepad_LeftStick_Left ? 0 : K == EKeys::Gamepad_LeftStick_Right ? 1 :
		K == EKeys::Gamepad_LeftStick_Up ? 2 : K == EKeys::Gamepad_LeftStick_Down ? 3 : INDEX_NONE;
	if (Direction == INDEX_NONE) return WasInputKeyJustPressed(K);
	UpdateMenuStickDirections();
	return (MenuStickPressed & uint8(1 << Direction)) != 0;
}

bool AFTPlayerController::Down(const FKey& K) const
{
	if (!K.IsValid()) return false;
	if (IsInputKeyDown(K)) return true;
	// the right-hand modifiers count as the left ones
	if (K == EKeys::LeftShift) return IsInputKeyDown(EKeys::RightShift);
	if (K == EKeys::LeftControl) return IsInputKeyDown(EKeys::RightControl);
	if (K == EKeys::LeftAlt) return IsInputKeyDown(EKeys::RightAlt);
	return false;
}

bool AFTPlayerController::BindDown(int32 A, int32 L) const
{
	const UFTGameInstance* GI = GetGameInstance<UFTGameInstance>();
	return GI && Down(GI->Binds[A][L]);
}

bool AFTPlayerController::BindHit(int32 A, int32 L) const
{
	if (bReleaseMenuInput) return false;
	const UFTGameInstance* GI = GetGameInstance<UFTGameInstance>();
	if (!GI) return false;
	const FKey& K = GI->Binds[A][L];
	if (!K.IsValid()) return false;
	if (WasInputKeyJustPressed(K)) return true;
	if (K == EKeys::LeftShift) return WasInputKeyJustPressed(EKeys::RightShift);
	if (K == EKeys::LeftControl) return WasInputKeyJustPressed(EKeys::RightControl);
	return false;
}

float AFTPlayerController::PadSteer() const
{
	const float Xv = GetInputAnalogKeyState(EKeys::Gamepad_LeftX);
	const float A = FMath::Abs(Xv);
	float S = A < 0.2f ? 0.f : FMath::Sign(Xv) * (A - 0.2f) / 0.8f;
	if (IsInputKeyDown(EKeys::Gamepad_DPad_Left)) S = -1.f;
	if (IsInputKeyDown(EKeys::Gamepad_DPad_Right)) S = 1.f;
	return S;
}

void AFTPlayerController::BlockMenuInputUntilRelease()
{
	bReleaseMenuInput = true;
	MenuInputReleasedAfter = FPlatformTime::Seconds() + 0.12;
	SuppressGameplayUntilRelease();
	bSwallowFire = true;
	bPrevPadMissile = bPrevDrift = false;
}

void AFTPlayerController::PlayerTick(float Dt)
{
	Super::PlayerTick(Dt);
	if (UFTInputSubsystem* Input = UFTInputSubsystem::Get(this)) Input->ObservePointer(this);
	UFTGameInstance* GI = GetGameInstance<UFTGameInstance>();
	if (!GI) return;
	const float RealDt = FApp::GetDeltaTime();
	if (bReleaseMenuInput)
	{
		bool bHeld = false;
		for (const FKey& K : { EKeys::Escape, EKeys::Enter, EKeys::SpaceBar, EKeys::P,
			EKeys::LeftMouseButton, EKeys::RightMouseButton, EKeys::Gamepad_Special_Right, EKeys::Gamepad_Special_Left,
			EKeys::Gamepad_FaceButton_Bottom, EKeys::Gamepad_FaceButton_Right,
			EKeys::Gamepad_FaceButton_Left, EKeys::Gamepad_FaceButton_Top })
			bHeld |= IsInputKeyDown(K);
		if (!bHeld && FPlatformTime::Seconds() >= MenuInputReleasedAfter) bReleaseMenuInput = false;
		In = FFTInput();
		return;
	}
	if (bMenuMouse || IsPaused()) { In = FFTInput(); return; }

	// the layout follows the steering keys
	if (bRaceInput && !bCapture)
	{
		const int32 Other = 1 - GI->Layout;
		for (int32 A : { (int32)EFTAction::Left, (int32)EFTAction::Right })
		{
			const FKey K = GI->Binds[A][Other];
			bool bInMine = false;
			for (int32 b = 0; b < UFTGameInstance::NumActions; ++b) if (GI->Binds[b][GI->Layout] == K) bInMine = true;
			if (K.IsValid() && !bInMine && WasInputKeyJustPressed(K)) { GI->Layout = Other; GI->SaveSettings(); break; }
		}
	}
	const int32 L = GI->Layout;
	auto Act = [&](EFTAction A) { return BindDown((int32)A, L); };

	float Raw = 0.f;
	if (Act(EFTAction::Left)) Raw -= 1.f;
	if (Act(EFTAction::Right)) Raw += 1.f;
	const float Pad = PadSteer();
	In.bPad = FMath::Abs(Pad) > 0.f;
	if (FMath::Abs(Pad) > FMath::Abs(Raw)) Raw = Pad;
	In.RawSteer = Raw;
	// ease: 9/s growing, 16/s coming back
	const float Rate = (FMath::Abs(Raw) > FMath::Abs(In.Steer) && FMath::Sign(Raw) == FMath::Sign(In.Steer)) || In.Steer == 0.f ? 9.f : 16.f;
	// Quick Hands (WHL1): steering rate 9 -> 12 a second
	const AFTGameMode* GM = GetWorld() ? GetWorld()->GetAuthGameMode<AFTGameMode>() : nullptr;
	const float SteerMul = (GM && GM->Stats.Has(TEXT("WHL1"))) ? 12.f / 9.f : 1.f;
	In.Steer = FMath::FInterpConstantTo(In.Steer, Raw, RealDt, Rate * (In.bPad ? 1.f : 0.6f) * SteerMul);
	if (In.bPad && FMath::Abs(Raw) > 0.f) In.Steer = FMath::FInterpTo(In.Steer, Raw, RealDt, 14.f * SteerMul);

	const float StickY = GetInputAnalogKeyState(EKeys::Gamepad_LeftY);
	In.bUp = Act(EFTAction::Up) || StickY > 0.55f || IsInputKeyDown(EKeys::Gamepad_DPad_Up);
	In.bDown = Act(EFTAction::Down) || StickY < -0.55f || IsInputKeyDown(EKeys::Gamepad_DPad_Down) || GetInputAnalogKeyState(UFTInputSubsystem::RacePadKey(EFTAction::Down)) > .4f;
	In.bDrift = Act(EFTAction::Drift) || IsInputKeyDown(UFTInputSubsystem::RacePadKey(EFTAction::Drift)) || IsInputKeyDown(EKeys::Gamepad_RightShoulder);
	In.bDriftPressed = In.bDrift && !bPrevDrift;
	bPrevDrift = In.bDrift;
	In.bTurbo = Act(EFTAction::Turbo) || IsInputKeyDown(EKeys::Gamepad_LeftShoulder) || GetInputAnalogKeyState(UFTInputSubsystem::RacePadKey(EFTAction::Turbo)) > 0.4f;
	In.bFire = Act(EFTAction::Fire) || IsInputKeyDown(EKeys::LeftMouseButton) || IsInputKeyDown(UFTInputSubsystem::RacePadKey(EFTAction::Fire));
	// RESUME acts on the button going down, and that button, still held, was FIRE on the next frame (it spent a
	// round, and AP/HE rounds are bought): nothing fires until it has been let go
	if (bSwallowFire) { if (In.bFire) In.bFire = false; else bSwallowFire = false; }
	const bool bPadM = IsInputKeyDown(UFTInputSubsystem::RacePadKey(EFTAction::Missile));
	In.bMissile = BindHit((int32)EFTAction::Missile, L) || WasInputKeyJustPressed(EKeys::RightMouseButton) || (bPadM && !bPrevPadMissile);
	bPrevPadMissile = bPadM;
	In.bAmmo = bRaceInput && (BindHit((int32)EFTAction::Ammo, L) || WasInputKeyJustPressed(UFTInputSubsystem::RacePadKey(EFTAction::Ammo)) || WasInputKeyJustPressed(EKeys::MiddleMouseButton)
		|| WasInputKeyJustPressed(EKeys::MouseScrollUp) || WasInputKeyJustPressed(EKeys::MouseScrollDown));
	if (bSuppressGameplay)
	{
		// Check held buttons, not one-frame events. Resetting a pressed event alone would
		// allow the same held confirm button to drift or fire on the following frame.
		const bool bHeldAction = Act(EFTAction::Drift) || Act(EFTAction::Turbo) || Act(EFTAction::Fire) || Act(EFTAction::Missile) || Act(EFTAction::Ammo)
			|| IsInputKeyDown(EKeys::LeftMouseButton) || IsInputKeyDown(EKeys::RightMouseButton) || IsInputKeyDown(EKeys::MiddleMouseButton)
			|| IsInputKeyDown(EKeys::Gamepad_FaceButton_Bottom) || IsInputKeyDown(EKeys::Gamepad_FaceButton_Right) || IsInputKeyDown(EKeys::Gamepad_FaceButton_Left) || IsInputKeyDown(EKeys::Gamepad_FaceButton_Top)
			|| IsInputKeyDown(EKeys::Gamepad_LeftShoulder) || IsInputKeyDown(EKeys::Gamepad_RightShoulder)
			|| GetInputAnalogKeyState(EKeys::Gamepad_RightTriggerAxis) > .2f || GetInputAnalogKeyState(EKeys::Gamepad_LeftTriggerAxis) > .2f;
		if (!bHeldAction) bSuppressGameplay = false;
		In = FFTInput();
		bPrevPadMissile = bPadM;
		bPrevDrift = false;
	}
}

bool AFTPlayerController::MenuLeft() const
{
	const UFTGameInstance* GI = GetGameInstance<UFTGameInstance>();
	return Hit(EKeys::Left) || Hit(EKeys::A) || Hit(EKeys::Gamepad_DPad_Left) || Hit(EKeys::Gamepad_LeftStick_Left) || (GI && BindHit((int32)EFTAction::Left, GI->Layout));
}
bool AFTPlayerController::MenuRight() const
{
	const UFTGameInstance* GI = GetGameInstance<UFTGameInstance>();
	return Hit(EKeys::Right) || Hit(EKeys::D) || Hit(EKeys::Gamepad_DPad_Right) || Hit(EKeys::Gamepad_LeftStick_Right) || (GI && BindHit((int32)EFTAction::Right, GI->Layout));
}
// (the mouse wheel steps through a list like Up / Down, on the screens that take the mouse)
bool AFTPlayerController::MenuUp() const { return Hit(EKeys::Up) || Hit(EKeys::W) || Hit(EKeys::Gamepad_DPad_Up) || Hit(EKeys::Gamepad_LeftStick_Up) || (bMenuMouse && Hit(EKeys::MouseScrollUp)); }
bool AFTPlayerController::MenuDown() const { return Hit(EKeys::Down) || Hit(EKeys::S) || Hit(EKeys::Gamepad_DPad_Down) || Hit(EKeys::Gamepad_LeftStick_Down) || (bMenuMouse && Hit(EKeys::MouseScrollDown)); }
bool AFTPlayerController::MenuOk() const { return Hit(EKeys::Enter) || Hit(EKeys::SpaceBar) || Hit(EKeys::Gamepad_FaceButton_Bottom); }
bool AFTPlayerController::MenuBack() const { return Hit(EKeys::Escape) || Hit(EKeys::Gamepad_FaceButton_Right) || Hit(EKeys::Gamepad_Special_Left); }

bool AFTPlayerController::PollCapture(FKey& Out)
{
	if (bReleaseMenuInput) return false;
	TArray<FKey> Keys;
	EKeys::GetAllKeys(Keys);
	for (const FKey& K : Keys)
	{
		if (K == EKeys::AnyKey || K.IsMouseButton() || K.IsGamepadKey() || K.IsAxis1D() || K.IsAxis2D() || K.IsTouch()) continue;
		if (WasInputKeyJustPressed(K)) { Out = K; return true; }
	}
	return false;
}
