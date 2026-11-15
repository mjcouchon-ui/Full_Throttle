#if WITH_DEV_AUTOMATION_TESTS
#include "FTInput.h"
#include "FTGameInstance.h"
#include "InputKeyEventArgs.h"
#include "Misc/AutomationTest.h"
#include "HAL/PlatformProcess.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFTInputFamilyTest, "FullThrottle.Input.DeviceFamiliesAndBindings", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FFTInputFamilyTest::RunTest(const FString& Parameters)
{
	TestTrue(TEXT("XInput describes its exposed family, even if hardware text mentions Sony"), UFTInputSubsystem::ClassifyDevice(TEXT("XInputInterface"), TEXT("DualSense")) == EFTControllerFamily::Xbox);
	TestTrue(TEXT("Native DualSense descriptor"), UFTInputSubsystem::ClassifyDevice(TEXT("GameInput"), TEXT("DualSense Wireless Controller")) == EFTControllerFamily::PlayStation);
	TestTrue(TEXT("Nintendo descriptor"), UFTInputSubsystem::ClassifyDevice(TEXT("SDL"), TEXT("Nintendo Switch Pro Controller")) == EFTControllerFamily::Nintendo);
	TestTrue(TEXT("Unknown remains generic"), UFTInputSubsystem::ClassifyDevice(TEXT("Generic"), TEXT("USB Gamepad")) == EFTControllerFamily::Generic);
	TestEqual(TEXT("Nintendo south is B, not A"), UFTInputSubsystem::FamilyKeyLabel(EFTControllerFamily::Nintendo, EKeys::Gamepad_FaceButton_Bottom), FString(TEXT("B")));
	TestEqual(TEXT("Nintendo east is A"), UFTInputSubsystem::FamilyKeyLabel(EFTControllerFamily::Nintendo, EKeys::Gamepad_FaceButton_Right), FString(TEXT("A")));
	TestEqual(TEXT("PS secondary button"), UFTInputSubsystem::FamilyKeyLabel(EFTControllerFamily::PlayStation, EKeys::Gamepad_FaceButton_Right), FString(TEXT("Circle")));
	TestEqual(TEXT("Unknown button has positional fallback"), UFTInputSubsystem::FamilyKeyLabel(EFTControllerFamily::Generic, EKeys::Gamepad_FaceButton_Left), FString(TEXT("West")));
	TestTrue(TEXT("Primary attack consistent across modes"), UFTInputSubsystem::RacePadKey(EFTAction::Fire) == UFTInputSubsystem::BrawlPadKey(EFTBrawlAction::Punch));
	TestTrue(TEXT("Secondary attack consistent across modes"), UFTInputSubsystem::RacePadKey(EFTAction::Missile) == UFTInputSubsystem::BrawlPadKey(EFTBrawlAction::Kick));
	TestTrue(TEXT("Utility consistent across modes"), UFTInputSubsystem::RacePadKey(EFTAction::Ammo) == UFTInputSubsystem::BrawlPadKey(EFTBrawlAction::GrabThrow));
	UFTGameInstance* GI = NewObject<UFTGameInstance>();
	GI->ResetBinds();
	UFTInputSubsystem* Input = NewObject<UFTInputSubsystem>(GI);
	TestTrue(TEXT("Jump defaults to Space"), Input->BrawlKeyboardKey(EFTBrawlAction::Jump) == EKeys::SpaceBar);
	GI->Binds[int32(EFTAction::Fire)][0] = EKeys::SpaceBar;
	TestTrue(TEXT("Jump avoids rebound attack on Space"), Input->BrawlKeyboardKey(EFTBrawlAction::Jump) != EKeys::SpaceBar);
	TestTrue(TEXT("Punch follows rebound key"), Input->BrawlKeyboardKey(EFTBrawlAction::Punch) == EKeys::SpaceBar);
	TestEqual(TEXT("Prompt follows rebound key"), Input->BrawlPrompt(EFTBrawlAction::Punch), FString(TEXT("Space")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFTInputSwitchTest, "FullThrottle.Input.IntentionalSwitchAndDisconnect", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FFTInputSwitchTest::RunTest(const FString& Parameters)
{
	UFTGameInstance* GI = NewObject<UFTGameInstance>();
	UFTInputSubsystem* Input = NewObject<UFTInputSubsystem>(GI);
	const FInputDeviceId Device = FInputDeviceId::CreateFromInternalId(731);
	Input->ObserveInput(FInputKeyEventArgs::CreateSimulated(EKeys::Gamepad_FaceButton_Bottom, IE_Pressed, 1.f, -1, Device));
	TestTrue(TEXT("Deliberate gamepad button switches prompts"), Input->UsesGamepad());
	Input->ObserveInput(FInputKeyEventArgs::CreateSimulated(EKeys::MouseX, IE_Axis, 100.f));
	TestTrue(TEXT("Incidental mouse movement does not switch prompts"), Input->UsesGamepad());
	Input->ObserveInput(FInputKeyEventArgs::CreateSimulated(EKeys::J, IE_Pressed, 1.f));
	TestFalse(TEXT("Keyboard press switches prompts"), Input->UsesGamepad());
	Input->ObserveInput(FInputKeyEventArgs::CreateSimulated(EKeys::Gamepad_LeftX, IE_Axis, .2f, -1, Device));
	TestFalse(TEXT("Stick drift does not steal prompts"), Input->UsesGamepad());
	TestFalse(TEXT("Synthetic stick direction does not bypass noise filter"), UFTInputSubsystem::IsIntentionalInput(FInputKeyEventArgs::CreateSimulated(EKeys::Gamepad_LeftStick_Right, IE_Pressed, 1.f)));
	Input->ObserveInput(FInputKeyEventArgs::CreateSimulated(EKeys::Gamepad_FaceButton_Left, IE_Pressed, 1.f, -1, Device));
	Input->HandleConnectionChange(EInputDeviceConnectionState::Disconnected, PLATFORMUSERID_NONE, FInputDeviceId::CreateFromInternalId(732));
	TestFalse(TEXT("Disconnecting idle pad does not pause active player"), Input->ConsumeDisconnect());
	Input->HandleConnectionChange(EInputDeviceConnectionState::Disconnected, PLATFORMUSERID_NONE, Device);
	TestTrue(TEXT("Active pad disconnect latches notification"), Input->HasDisconnectedController());
	TestTrue(TEXT("Active pad disconnect requests pause"), Input->ConsumeDisconnect());
	TestFalse(TEXT("Pause request consumed once"), Input->ConsumeDisconnect());
	Input->ObserveInput(FInputKeyEventArgs::CreateSimulated(EKeys::Escape, IE_Pressed, 1.f));
	TestFalse(TEXT("Keyboard can recover after disconnect"), Input->HasDisconnectedController());
	UFTInputSubsystem* Diagonal = NewObject<UFTInputSubsystem>(GI);
	Diagonal->ObserveInput(FInputKeyEventArgs::CreateSimulated(EKeys::Gamepad_LeftX, IE_Axis, .8f, -1, Device));
	Diagonal->ObserveInput(FInputKeyEventArgs::CreateSimulated(EKeys::Gamepad_LeftY, IE_Axis, .8f, -1, Device));
	TestFalse(TEXT("One diagonal sample does not bypass sustained intent"), Diagonal->UsesGamepad());
	FPlatformProcess::Sleep(.11f);
	Diagonal->ObserveInput(FInputKeyEventArgs::CreateSimulated(EKeys::Gamepad_LeftX, IE_Axis, .8f, -1, Device));
	TestTrue(TEXT("Sustained diagonal movement switches prompts"), Diagonal->UsesGamepad());
	return true;
}
#endif
