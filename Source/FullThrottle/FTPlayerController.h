// Reads the controls the 1.7.1 way: the left hand drives, the right hand shoots. Two keyboard layouts
// (WASD + Shift/Space + J/K, or arrows + Shift/Space + Z/X), each reading only its own keys; the layout
// follows the steering keys. Mouse (hold left = guns, right click = missile) and gamepad always work.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "FTPlayerController.generated.h"

struct FFTInput
{
	float Steer = 0.f;      // eased, -1..1
	float RawSteer = 0.f;
	bool bUp = false, bDown = false, bDrift = false, bTurbo = false, bFire = false;
	bool bMissile = false;  // this frame
	bool bAmmo = false;     // 2.6: AMMO cycles the load, this frame (L / C, pad Y, middle click)
	bool bDriftPressed = false;
	bool bPad = false;
};

UCLASS()
class FULLTHROTTLE_API AFTCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
};

UCLASS()
class FULLTHROTTLE_API AFTPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AFTPlayerController();
	virtual void PlayerTick(float Dt) override;
	virtual void BeginPlay() override;
	virtual bool InputKey(const FInputKeyEventArgs& Params) override;
	bool UsesGamepadPrompts() const { return bGamepadPrompts; }

	FFTInput In;
	bool bRaceInput = false;      // the race is running (layout switching only then)

	// menus
	bool Hit(const FKey& K) const { return !bReleaseMenuInput && WasInputKeyJustPressed(K); }
	void BlockMenuInputUntilRelease();
	bool MenuInputBlocked() const { return bReleaseMenuInput; }
	bool MenuLeft() const;
	bool MenuRight() const;
	bool MenuUp() const;
	bool MenuDown() const;
	bool MenuOk() const;
	bool MenuBack() const;

	// screens with buttons (the reel's CONTINUE, the garage): the cursor shows and clicks are read against rects
	void SetMenuMouse(bool bOn);
	bool bMenuMouse = false;
	/** Applies bMenuMouse's input mode and cursor (at BeginPlay, after a screen may already have asked for the cursor). */
	void ApplyMouseMode(const TCHAR* Why);
	FVector2D MousePos() const;
	bool MouseIn(const FBox2D& R) const { const FVector2D M = MousePos(); return R.bIsValid && M.X >= 0.f && R.IsInside(M); }
	bool Clicked(const FBox2D& R) const { return Hit(EKeys::LeftMouseButton) && MouseIn(R); }
	/** Set when a menu is closed by a button that is also FIRE: no shot until that button is released. */
	bool bSwallowFire = false;
	bool RightClicked(const FBox2D& R) const { return Hit(EKeys::RightMouseButton) && MouseIn(R); }

	// rebinding: the next key pressed is returned once
	bool bCapture = false;
	bool PollCapture(FKey& Out);

private:
	bool bReleaseMenuInput = false;
	double MenuInputReleasedAfter = 0.0;
	bool Down(const FKey& K) const;
	bool BindDown(int32 Action, int32 Layout) const;
	bool BindHit(int32 Action, int32 Layout) const;
	float PadSteer() const;
	bool bPrevPadMissile = false;
	bool bPrevDrift = false;
	bool bGamepadPrompts = false;
};
