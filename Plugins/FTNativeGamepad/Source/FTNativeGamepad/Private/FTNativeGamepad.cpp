#include "IInputDeviceModule.h"
#include "GameInputBaseModule.h"
#include "GameInputDeveloperSettings.h"
#include "GameInputDeviceContainer.h"
#include "GameInputUtils.h"
#include "IGameInputDeviceInterface.h"
#include "Processors/GameInputDeviceProcessor_Controller.h"
#include "Processors/GameInputDeviceProcessor_Gamepad.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "GenericPlatform/InputDeviceRegistry.h"
#include "Misc/ScopeLock.h"

DEFINE_LOG_CATEGORY_STATIC(LogFTNativeGamepad, Log, All);

#if GAME_INPUT_SUPPORT
namespace
{
bool IsConfiguredDualShock(const GameInputDeviceInfo* Info)
{
    // Do not also consume Xbox devices through their generic-controller capability.
    return Info && Info->vendorId == 0x054c && Info->productId == 0x09cc
        && (Info->supportedInput & GameInputKindController) != 0
        && GetDefault<UGameInputDeveloperSettings>()->FindDeviceConfiguration(Info);
}

class FFTNativeControllerProcessor : public FGameInputControllerDeviceProcessor
{
public:
    explicit FFTNativeControllerProcessor(const FGameInputDeviceConfiguration& InConfig) : Config(InConfig)
    {
        FMemory::Memzero(RepeatTime);
    }

protected:
    virtual void ClearState(const FGameInputEventParams& Params) override
    {
        // UE 5.9's generic controller reset reads uninitialized switch positions and
        // memsets the TArray object. Explicitly release inputs and retain its storage.
        for (GameInputSwitchPosition& Position : PreviousSwitchPositions)
        {
            EvaluateSwitchState(Params, GameInputSwitchCenter, Position, SwitchRepeatTimes);
        }
        EvaluateButtonStates(Params, 0, LastButtonHeldMask, RepeatTime, Config.ControllerButtonMappingData, MaxSupportedButtons);
        for (int32 Index = 0; Index < PreviousControllerAxisValues.Num(); ++Index)
        {
            if (const FGameInputControllerAxisData* Axis = Config.ControllerAxisMappingData.Find(Index))
            {
                OnControllerAnalog(Params, Axis->KeyName, 0.f, PreviousControllerAxisValues[Index], Axis->DeadZone);
            }
            PreviousControllerAxisValues[Index] = 0.f;
        }
        FMemory::Memzero(RepeatTime);
        for (double& Time : SwitchRepeatTimes) Time = 0.0;
        NumReadingsProcessedThisFrame = 0;
    }

private:
    // Remains available while releasing a device, including after a settings edit.
    FGameInputDeviceConfiguration Config;
};

class FFTNativeGamepadProcessor : public FGameInputGamepadDeviceProcessor
{
protected:
    virtual void ClearState(const FGameInputEventParams& Params) override
    {
        if (!Params.PlatformUserId.IsValid() || !Params.InputDeviceId.IsValid()) return;
        // Release synthetic digital stick/trigger keys before the base drops their mask.
        GameInputGamepadState Neutral = {};
        ProcessGamepadAnalogState(Params, Neutral);
        FGameInputGamepadDeviceProcessor::ClearState(Params);
        NumReadingsProcessedThisFrame = 0;
    }
};

class FFTNativeDeviceContainer final : public FGameInputDeviceContainer
{
public:
    using FGameInputDeviceContainer::FGameInputDeviceContainer;

protected:
    virtual void InitalizeDeviceProcessors_Impl() override
    {
        const GameInputDeviceInfo* Info = UE::GameInput::GetDeviceInfo(Device);
        if (!IsConfiguredDualShock(Info)) return;
        // Use exactly one processor for this device, even when it exposes both kinds.
        if ((AllowedGameInputKinds & Info->supportedInput & GameInputKindGamepad) != 0)
        {
            Processors.Add(MakeShared<FFTNativeGamepadProcessor>());
        }
        else if ((AllowedGameInputKinds & GameInputKindController) != 0)
        {
            const FGameInputDeviceConfiguration* Config = GetDefault<UGameInputDeveloperSettings>()->FindDeviceConfiguration(Info);
            Processors.Add(MakeShared<FFTNativeControllerProcessor>(*Config));
        }
    }
};

class FFTNativeInputDevice final : public IGameInputDeviceInterface
{
public:
    FFTNativeInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& Handler, IGameInput* Input)
        : IGameInputDeviceInterface(Handler, Input) {}

    void Attach(IGameInput* Input)
    {
        if (!Input || bWasinitialized) return;
        GameInput = Input;
        Initialize();
        UE_LOG(LogFTNativeGamepad, Log, TEXT("Native DS4 backend initialized; Xbox remains on XInput."));
    }

protected:
    virtual void Tick(float DeltaTime) override
    {
        // Also cover a delayed creation completing before our delegate was registered.
        if (!bWasinitialized) Attach(FGameInputBaseModule::GetGameInput());
        IGameInputDeviceInterface::Tick(DeltaTime);
    }

    virtual GameInputKind GetCurrentGameInputKindSupport() const override
    {
        const UGameInputPlatformSettings* Settings = UGameInputPlatformSettings::Get();
        GameInputKind Kinds = Settings->bProcessController ? GameInputKindController : GameInputKindUnknown;
        if (Settings->bProcessGamepad) Kinds |= GameInputKindGamepad;
        return Kinds;
    }

    virtual bool IsSupportedDevice(IGameInputDevice* InputDevice) const override
    {
        return IGameInputDeviceInterface::IsSupportedDevice(InputDevice)
            && IsConfiguredDualShock(UE::GameInput::GetDeviceInfo(InputDevice));
    }

    virtual FGameInputDeviceContainer* CreateDeviceData(IGameInputDevice* InputDevice) override
    {
        TSharedPtr<FFTNativeDeviceContainer> Container = MakeShared<FFTNativeDeviceContainer>(MessageHandler, InputDevice, GetCurrentGameInputKindSupport());
        Container->InitalizeDeviceProcessors();
        if (Container->GetNumberOfProcessors() > 0)
        {
            Container->SetInputDeviceId(IPlatformInputDeviceMapper::Get().AllocateNewInputDeviceId());
        }
        DeviceData.Add(Container);
        return Container.Get();
    }

    virtual void HandleDeviceConnected(IGameInputDevice* InputDevice, uint64 Timestamp) override
    {
        FGameInputDeviceContainer* Container = GetOrCreateDeviceData(InputDevice);
        if (!Container || !Container->GetDeviceId().IsValid()) return;
        IPlatformInputDeviceMapper& Mapper = IPlatformInputDeviceMapper::Get();
        const FPlatformUserId User = Mapper.GetPrimaryPlatformUser();
        const FInputDeviceId Id = Container->GetDeviceId();
        Container->SetPlatformUserId(User);
        FInputDeviceDescriptor Descriptor;
        Descriptor.HardwareDeviceHandle = Id;
        Descriptor.InputDeviceName = UE::GameInput::GetDeviceScopeClassName();
        Descriptor.HardwareDeviceIdentifier = FName(UE::GameInput::GetHardwareDeviceIdentifierName(InputDevice));
        FInputDeviceRegistry::RegisterDevice(Descriptor);
        Mapper.Internal_MapInputDeviceToUser(Id, User, EInputDeviceConnectionState::Connected);
        EnumerateCurrentlyConnectedDeviceTypes();
        UE_LOG(LogFTNativeGamepad, Log, TEXT("Connected native DS4: user=%d device=%d %s"), User.GetInternalId(), Id.GetId(), *UE::GameInput::LexToString(InputDevice));
    }

    virtual void HandleDeviceDisconnected(IGameInputDevice* InputDevice, uint64 Timestamp) override
    {
        if (FGameInputDeviceContainer* Container = GetDeviceData(InputDevice))
        {
            // Release held keys and axes while the original device and user are valid.
            Container->ClearInputState(GameInput);
            Container->SetGameInputDevice(nullptr);
            IPlatformInputDeviceMapper& Mapper = IPlatformInputDeviceMapper::Get();
            const FPlatformUserId Unpaired = Mapper.GetUserForUnpairedInputDevices();
            Mapper.Internal_MapInputDeviceToUser(Container->GetDeviceId(), Unpaired, EInputDeviceConnectionState::Disconnected);
            Container->SetPlatformUserId(Unpaired);
            UE_LOG(LogFTNativeGamepad, Log, TEXT("Disconnected native DS4: device=%d; held inputs released."), Container->GetDeviceId().GetId());
        }
        EnumerateCurrentlyConnectedDeviceTypes();
    }
};
}
#endif

class FFTNativeGamepadModule final : public IInputDeviceModule
{
public:
    virtual TSharedPtr<IInputDevice> CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& Handler) override
    {
#if GAME_INPUT_SUPPORT
        FGameInputBaseModule& Base = FGameInputBaseModule::Get();
        TSharedPtr<FFTNativeInputDevice> Device = MakeShared<FFTNativeInputDevice>(Handler, nullptr);
        TWeakPtr<FFTNativeInputDevice> WeakDevice = Device;
        Base.OnGameInputCreation.AddLambda([WeakDevice](IGameInput* Input)
        {
            if (TSharedPtr<FFTNativeInputDevice> Pinned = WeakDevice.Pin()) Pinned->Attach(Input);
        });
        Device->Attach(FGameInputBaseModule::GetGameInput());
        return Device;
#else
        return nullptr;
#endif
    }

    virtual void ShutdownModule() override
    {
        IModularFeatures::Get().UnregisterModularFeature(GetModularFeatureName(), this);
    }
};

IMPLEMENT_MODULE(FFTNativeGamepadModule, FTNativeGamepad)

#if WITH_DEV_AUTOMATION_TESTS && GAME_INPUT_SUPPORT
#include "Misc/AutomationTest.h"

namespace
{
class FFTNativeRecordingHandler final : public FGenericApplicationMessageHandler
{
public:
    TArray<FName> Released;
    TMap<FName, float> Axes;
    virtual bool OnControllerButtonReleased(FGamepadKeyNames::Type Key, FPlatformUserId User, FInputDeviceId Device, bool Repeat) override
    {
        Released.Add(Key);
        return true;
    }
    virtual bool OnControllerAnalog(FGamepadKeyNames::Type Key, FPlatformUserId User, FInputDeviceId Device, float Value) override
    {
        Axes.Add(Key, Value);
        return true;
    }
};

class FFTNativeProcessorFixture final : public FFTNativeControllerProcessor
{
public:
    using FFTNativeControllerProcessor::FFTNativeControllerProcessor;
    using FFTNativeControllerProcessor::ClearState;
    void Hold()
    {
        LastButtonHeldMask = 2;
        PreviousSwitchPositions = {GameInputSwitchUpRight};
        PreviousControllerAxisValues = {1.f, -1.f};
        NumReadingsProcessedThisFrame = 5;
    }
    const float* AxisStorage() const { return PreviousControllerAxisValues.GetData(); }
    bool IsClear() const
    {
        return LastButtonHeldMask == 0 && PreviousSwitchPositions.Num() == 1
            && PreviousSwitchPositions[0] == GameInputSwitchCenter && PreviousControllerAxisValues.Num() == 2
            && PreviousControllerAxisValues[0] == 0.f && PreviousControllerAxisValues[1] == 0.f
            && NumReadingsProcessedThisFrame == 0;
    }
};

class FFTNativeCanonicalFixture final : public FFTNativeGamepadProcessor
{
public:
    using FFTNativeGamepadProcessor::ClearState;
    void Hold(const FGameInputEventParams& Params)
    {
        GameInputGamepadState State = {};
        State.leftThumbstickY = 1.f;
        State.rightTrigger = 1.f;
        ProcessGamepadAnalogState(Params, State);
    }
};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFTNativeDisconnectTest, "FT.Input.NativeDualShockDisconnect", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FFTNativeDisconnectTest::RunTest(const FString& Parameters)
{
    FGameInputDeviceConfiguration Config;
    Config.ControllerButtonMappingData.Add(2, FName(TEXT("Gamepad_FaceButton_Bottom")));
    FGameInputControllerAxisData X;
    X.KeyName = TEXT("Gamepad_LeftX");
    Config.ControllerAxisMappingData.Add(0, X);
    X.KeyName = TEXT("Gamepad_LeftY");
    Config.ControllerAxisMappingData.Add(1, X);
    FFTNativeProcessorFixture Processor(Config);
    const TSharedRef<FFTNativeRecordingHandler> Handler = MakeShared<FFTNativeRecordingHandler>();
    IGameInputDeviceProcessor::FGameInputEventParams Params;
    Params.MessageHandler = Handler;
    Params.PlatformUserId = IPlatformInputDeviceMapper::Get().GetPrimaryPlatformUser();
    Params.InputDeviceId = IPlatformInputDeviceMapper::Get().GetDefaultInputDevice();
    Processor.Hold();
    const float* Storage = Processor.AxisStorage();
    Processor.ClearState(Params);
    TestTrue(TEXT("Disconnect releases every cached control"), Processor.IsClear());
    TestTrue(TEXT("Axis allocation is retained, not overwritten"), Storage == Processor.AxisStorage());
    TestTrue(TEXT("Cross released"), Handler->Released.Contains(FName(TEXT("Gamepad_FaceButton_Bottom"))));
    TestTrue(TEXT("Diagonal releases both directions"), Handler->Released.Contains(FName(TEXT("Gamepad_DPad_Up"))) && Handler->Released.Contains(FName(TEXT("Gamepad_DPad_Right"))));
    TestEqual(TEXT("Both held axes reset"), Handler->Axes.Num(), 2);
    for (const auto& Pair : Handler->Axes) TestEqual(TEXT("Neutral axis event"), Pair.Value, 0.f);
    const int32 ReleaseCount = Handler->Released.Num();
    Processor.ClearState(Params);
    TestEqual(TEXT("Repeated disconnect does not repeat releases"), Handler->Released.Num(), ReleaseCount);
    Processor.Hold();
    Processor.ClearState(Params);
    TestTrue(TEXT("State can be reused after reconnection"), Processor.IsClear());
    FFTNativeCanonicalFixture Canonical;
    Canonical.Hold(Params);
    Canonical.ClearState(Params);
    TestTrue(TEXT("Canonical digital stick released"), Handler->Released.Contains(FName(TEXT("Gamepad_LeftStick_Up"))));
    TestTrue(TEXT("Canonical digital trigger released"), Handler->Released.Contains(FName(TEXT("Gamepad_RightTrigger"))));
    return true;
}
#endif
