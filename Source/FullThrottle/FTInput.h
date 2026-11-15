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
	/** Pure helpers shared with automation tests. */
	static EFTControllerFamily ClassifyDevice(const FString& InterfaceName, const FString& HardwareName);
	static FString FamilyKeyLabel(EFTControllerFamily ControllerFamily, const FKey& Key);
	static bool IsIntentionalInput(const FInputKeyEventArgs& Params);
	/** Development validation uses the same disconnect transition as the platform callback. */
	void HandleConnectionChange(EInputDeviceConnectionState State, FPlatformUserId User, FInputDeviceId Device);

private:
	void ResolveDevice(FInputDeviceId Device);
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
