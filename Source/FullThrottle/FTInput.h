// Shared input presentation for races and nights. Device identity follows input, never an unrelated HID scan.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InputCoreTypes.h"
#include "FTInput.generated.h"

struct FInputKeyEventArgs;
enum class EFTAction : uint8;

enum class EFTControllerFamily : uint8 { Generic, Xbox, PlayStation, Nintendo };
enum class EFTBrawlAction : uint8 { Move, Punch, Kick, Jump, GrabThrow, Pause, Confirm, Back };

UCLASS()
class FULLTHROTTLE_API UFTInputSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	static UFTInputSubsystem* Get(const UObject* Context);
	void ObserveInput(const FInputKeyEventArgs& Params);
	bool UsesGamepad() const { return bGamepad; }
	EFTControllerFamily Family() const;
	FString DeviceLabel() const;
	FString KeyLabel(const FKey& Key) const;
	FString RacePrompt(EFTAction Action) const;
	FString BrawlPrompt(EFTBrawlAction Action) const;
	FKey BrawlKeyboardKey(EFTBrawlAction Action) const;
	static FKey RacePadKey(EFTAction Action);
	static FKey BrawlPadKey(EFTBrawlAction Action);
	static const TCHAR* PromptStyleName(int32 Style);
	int32 GetPromptStyle() const { return PromptStyle; }
	void SetPromptStyle(int32 Style);
	bool HasDisconnectedController() const { return bDisconnected; }
	bool ConsumeDisconnect();
	void DismissDisconnect() { bDisconnected = false; bDisconnectPending = false; }
	FInputDeviceId ActiveDevice() const { return ActivePad; }
	// ---- the mouse pointer (drawn by the HUDs, FTCursor): it shows once the mouse moves, clicks or scrolls on a
	// screen that takes the mouse, and goes away again at the next deliberate key or controller input
	bool PointerActive() const { return bPointer; }
	/** Where the pointer is, in viewport pixels; false when there is none. The one place the menus read it. */
	bool PointerPosition(const class APlayerController* PC, FVector2D& Out) const;
	/** Once a frame, from the player controller: the pointer moving wakes it. */
	void ObservePointer(const class APlayerController* PC);
	/** A screen opened or closed, or the input mode changed: the next position is not a movement. */
	void ResyncPointer() { bPointerSeen = false; }
	/** Development: put the pointer at a fraction of the viewport (a test run has no real mouse); negative clears it. */
	void SetPointerOverride(const FVector2D& Fraction) { PointerOverride = Fraction; }
	/** Pure helpers shared with automation tests. */
	static EFTControllerFamily ClassifyDevice(const FString& InterfaceName, const FString& HardwareName);
	static FString FamilyKeyLabel(EFTControllerFamily ControllerFamily, const FKey& Key);
	static bool IsIntentionalInput(const FInputKeyEventArgs& Params);
	/** Development validation uses the same disconnect transition as the platform callback. */
	void HandleConnectionChange(EInputDeviceConnectionState State, FPlatformUserId User, FInputDeviceId Device);

private:
	void ResolveDevice(FInputDeviceId Device);
	bool bPointer = false, bPointerSeen = false;
	FVector2D PointerLast = FVector2D::ZeroVector;
	float PointerTravel = 0.f; double PointerMovedAt = -100.0;   // how far the mouse has gone without a pause (it takes over from a controller)
	FVector2D PointerOverride = FVector2D(-1.f, -1.f);
	bool bGamepad = false;
	bool bDisconnected = false;
	bool bDisconnectPending = false;
	int32 PromptStyle = 0;
	EFTControllerFamily DetectedFamily = EFTControllerFamily::Generic;
	FInputDeviceId ActivePad = INPUTDEVICEID_NONE;
	FString DetectedInterface;
	FDelegateHandle ConnectionHandle;
	double LastDigitalInputTime = -100.0;
	FInputDeviceId AxisCandidate = INPUTDEVICEID_NONE;
	TMap<FKey, double> AxisIntentSince;
};
