#include "FTPlayerController.h"
#include "FTGameInstance.h"
#include "FTGameMode.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "InputKeyEventArgs.h"
#include "HAL/IConsoleManager.h"

// test hooks that go through the player's real input path (WasInputKeyJustPressed sees them like a key):
//   ft.Press <Key>          press and release a key, mouse or pad button (Enter, SpaceBar, Gamepad_Special_Right, ...)
//   ft.Click <x> <y> [Right] move the cursor to viewport pixel x,y and click there
namespace
{
	void FTTap(UWorld* World, const FKey& K)
	{
		APlayerController* PC = World ? World->GetFirstPlayerController() : nullptr;
		if (!PC || !K.IsValid()) { UE_LOG(LogTemp, Warning, TEXT("ft.Press: no player or bad key")); return; }
		PC->InputKey(FInputKeyEventArgs::CreateSimulated(K, IE_Pressed, 1.f));
		TWeakObjectPtr<APlayerController> Weak(PC);
		FTimerHandle H;
		World->GetTimerManager().SetTimer(H, FTimerDelegate::CreateLambda([Weak, K]() { if (Weak.IsValid()) Weak->InputKey(FInputKeyEventArgs::CreateSimulated(K, IE_Released, 0.f)); }), 0.12f, false);
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
	PlayerCameraManagerClass = AFTCameraManager::StaticClass();
	bShowMouseCursor = false;
}

void AFTPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FInputModeGameOnly Mode;
	SetInputMode(Mode);
}

void AFTPlayerController::SetMenuMouse(bool bOn)
{
	if (bOn == bMenuMouse) return;
	bMenuMouse = bOn;
	SetShowMouseCursor(bOn);
	if (bOn)
	{
		FInputModeGameAndUI Mode;
		Mode.SetHideCursorDuringCapture(false);
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(Mode);
	}
	else SetInputMode(FInputModeGameOnly());
}

FVector2D AFTPlayerController::MousePos() const
{
	float X = -1.f, Y = -1.f;
	if (!GetMousePosition(X, Y)) return FVector2D(-1.f, -1.f);
	return FVector2D(X, Y);
}

bool AFTPlayerController::InputKey(const FInputKeyEventArgs& Params)
{
	if (Params.Event == IE_Pressed || (Params.Key.IsGamepadKey() && Params.Event == IE_Axis && FMath::Abs(Params.AmountDepressed) > .5f))
		bGamepadPrompts = Params.Key.IsGamepadKey();
	return Super::InputKey(Params);
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

void AFTPlayerController::PlayerTick(float Dt)
{
	Super::PlayerTick(Dt);
	UFTGameInstance* GI = GetGameInstance<UFTGameInstance>();
	if (!GI) return;
	const float RealDt = FApp::GetDeltaTime();

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
	In.bDown = Act(EFTAction::Down) || StickY < -0.55f || IsInputKeyDown(EKeys::Gamepad_DPad_Down);
	In.bDrift = Act(EFTAction::Drift) || IsInputKeyDown(EKeys::Gamepad_FaceButton_Bottom) || IsInputKeyDown(EKeys::Gamepad_RightShoulder);
	In.bDriftPressed = In.bDrift && !bPrevDrift;
	bPrevDrift = In.bDrift;
	In.bTurbo = Act(EFTAction::Turbo) || IsInputKeyDown(EKeys::Gamepad_LeftShoulder) || GetInputAnalogKeyState(EKeys::Gamepad_RightTriggerAxis) > 0.4f;
	In.bFire = Act(EFTAction::Fire) || IsInputKeyDown(EKeys::LeftMouseButton) || IsInputKeyDown(EKeys::Gamepad_FaceButton_Left);
	const bool bPadM = IsInputKeyDown(EKeys::Gamepad_FaceButton_Right);
	In.bMissile = BindHit((int32)EFTAction::Missile, L) || WasInputKeyJustPressed(EKeys::RightMouseButton) || (bPadM && !bPrevPadMissile);
	bPrevPadMissile = bPadM;
	In.bAmmo = bRaceInput && (BindHit((int32)EFTAction::Ammo, L) || WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Top) || WasInputKeyJustPressed(EKeys::MiddleMouseButton));
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
bool AFTPlayerController::MenuUp() const { return Hit(EKeys::Up) || Hit(EKeys::W) || Hit(EKeys::Gamepad_DPad_Up) || Hit(EKeys::Gamepad_LeftStick_Up); }
bool AFTPlayerController::MenuDown() const { return Hit(EKeys::Down) || Hit(EKeys::S) || Hit(EKeys::Gamepad_DPad_Down) || Hit(EKeys::Gamepad_LeftStick_Down); }
bool AFTPlayerController::MenuOk() const { return Hit(EKeys::Enter) || Hit(EKeys::SpaceBar) || Hit(EKeys::Gamepad_FaceButton_Bottom) || Hit(EKeys::Gamepad_Special_Right); }
bool AFTPlayerController::MenuBack() const { return Hit(EKeys::Escape) || Hit(EKeys::Gamepad_FaceButton_Right) || Hit(EKeys::Gamepad_Special_Left); }

bool AFTPlayerController::PollCapture(FKey& Out)
{
	TArray<FKey> Keys;
	EKeys::GetAllKeys(Keys);
	for (const FKey& K : Keys)
	{
		if (K == EKeys::AnyKey || K.IsMouseButton() || K.IsGamepadKey() || K.IsAxis1D() || K.IsAxis2D() || K.IsTouch()) continue;
		if (WasInputKeyJustPressed(K)) { Out = K; return true; }
	}
	return false;
}
