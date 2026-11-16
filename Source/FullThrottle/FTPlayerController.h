// Reads the controls the 1.7.1 way: the left hand drives, the right hand shoots. Two keyboard layouts
// (WASD + Shift/Space + J/K, or arrows + Shift/Space + Z/X), each reading only its own keys; the layout
// follows the steering keys. Mouse (hold left = guns, right click = missile, middle click or wheel = next ammo) and
// gamepad always work. In the menus the mouse points (hover, click, wheel through a list, right click = back).
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
	bool bAmmo = false;     // 2.6: AMMO cycles the load, this frame (L / C, pad Y, middle click, mouse wheel)
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
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void BeginPlay() override;
	virtual bool InputKey(const FInputKeyEventArgs& Params) override;
	bool UsesGamepadPrompts() const;
	/** A menu's confirm/back must be released before it can become a race action. */
	void SuppressGameplayUntilRelease();

	FFTInput In;
	bool bRaceInput = false;      // the race is running (layout switching only then)

	// menus
	bool Hit(const FKey& K) const;
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
	/** A right click anywhere on a pause / options / controls / dialog screen is BACK. */
	bool RightBack() const { return bMenuMouse && Hit(EKeys::RightMouseButton); }
	/** The drawn pointer (FTCursor) is on screen: a screen takes the mouse and the mouse was the last thing used. */
	bool PointerShown() const;

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
	bool bSuppressGameplay = false;
	void UpdateMenuStickDirections() const;
	mutable uint64 MenuStickFrame = ~uint64(0);
	mutable uint8 MenuStickHeld = 0, MenuStickPressed = 0;
#if !UE_BUILD_SHIPPING
	int8 LoggedStickX = 0, LoggedStickY = 0;
#endif
};
