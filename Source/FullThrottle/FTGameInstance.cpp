#include "FTGameInstance.h"
#include "FTCore.h"
#include "FTProgress.h"
#include "Containers/Ticker.h"
#include "UnrealClient.h"
#include "Misc/OutputDeviceRedirector.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/Engine.h"
#include "AudioDevice.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/CommandLine.h"
#include "HAL/PlatformFileManager.h"
#include "HAL/PlatformMisc.h"
#include "HAL/IConsoleManager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

const TCHAR* UFTGameInstance::ActionName(int32 A)
{
	static const TCHAR* N[] = { TEXT("STEER LEFT"), TEXT("STEER RIGHT"), TEXT("THROTTLE UP"), TEXT("BRAKE"), TEXT("DRIFT"), TEXT("TURBO"), TEXT("FIRE GUNS"), TEXT("MISSILE"), TEXT("AMMO") };
	return N[FMath::Clamp(A, 0, NumActions - 1)];
}

bool UFTGameInstance::IsReserved(const FKey& K)
{
	return K == EKeys::Escape || K == EKeys::P || K == EKeys::M || K == EKeys::Enter || K == EKeys::Tab || K == EKeys::BackSpace
		|| K == EKeys::Delete || K == EKeys::F1 || K == EKeys::O || K.IsMouseButton() || K.IsGamepadKey() || K.IsAxis1D() || K.IsAxis2D();
}

void UFTGameInstance::ResetBinds()
{
	// 2.6: AMMO is L on the WASD layout and C on the arrows layout (06 §5)
	const FKey W[NumActions] = { EKeys::A, EKeys::D, EKeys::W, EKeys::S, EKeys::LeftShift, EKeys::SpaceBar, EKeys::J, EKeys::K, EKeys::L };
	const FKey R[NumActions] = { EKeys::Left, EKeys::Right, EKeys::Up, EKeys::Down, EKeys::LeftShift, EKeys::SpaceBar, EKeys::Z, EKeys::X, EKeys::C };
	for (int32 a = 0; a < NumActions; ++a) { Binds[a][0] = W[a]; Binds[a][1] = R[a]; }
}

void UFTGameInstance::SetBind(int32 Action, int32 L, const FKey& Key)
{
	if (Key.IsValid())
	{
		for (int32 a = 0; a < NumActions; ++a) if (a != Action && Binds[a][L] == Key) Binds[a][L] = FKey();
	}
	Binds[Action][L] = Key;
}

FString UFTGameInstance::SaveDir() const
{
	const FString AppData = FPlatformMisc::GetEnvironmentVariable(TEXT("WISPOS_APP_DATA"));
	if (!AppData.IsEmpty()) return AppData;
	return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("FullThrottle"));
}

FString UFTGameInstance::QualityName(int32 Q)
{
	switch (Q)
	{
	case 0: return TEXT("LOW"); case 1: return TEXT("MEDIUM"); case 2: return TEXT("HIGH"); case 3: return TEXT("EPIC"); case 4: return TEXT("CINEMATIC");
	default: return TEXT("AUTO");
	}
}

void UFTGameInstance::Init()
{
	Super::Init();
#if UE_BUILD_SHIPPING
	{
		// WispOS launch ticket: WispOS writes $WISPOS_APP_DATA/launch-ticket and passes the same value in
		// WISPOS_LAUNCH_TICKET. Compare, delete the file, run. Anything else: say so and quit.
		const FString AppData = FPlatformMisc::GetEnvironmentVariable(TEXT("WISPOS_APP_DATA"));
		const FString Ticket = FPlatformMisc::GetEnvironmentVariable(TEXT("WISPOS_LAUNCH_TICKET"));
		FString OnDisk;
		const FString File = FPaths::Combine(AppData, TEXT("launch-ticket"));
		const bool bOk = !AppData.IsEmpty() && !Ticket.IsEmpty() && FFileHelper::LoadFileToString(OnDisk, *File) && OnDisk.TrimStartAndEnd() == Ticket.TrimStartAndEnd();
		if (bOk) IFileManager::Get().Delete(*File);
		else
		{
			FPlatformMisc::MessageBoxExt(EAppMsgType::Ok, TEXT("Open FULL THROTTLE from WispOS."), TEXT("FULL THROTTLE"));
			FPlatformMisc::RequestExit(true);
			return;
		}
	}
#endif
	ResetBinds();
	LoadSettings();
	// the career (spec 09): loaded after the settings, so a save with no career gets the back-catalogue bonus
	if (UFTCareerSubsystem* CS = GetSubsystem<UFTCareerSubsystem>())
	{
		FString Slot;
		if (FParse::Value(FCommandLine::Get(), TEXT("ftslot="), Slot) && Slot == TEXT("pie")) CS->Slot = TEXT("career_pie");
		CS->Load();
	}
	{
		FString ExecFile;
		if (FParse::Value(FCommandLine::Get(), TEXT("ftexec="), ExecFile))
		{
			FString Text;
			if (FFileHelper::LoadFileToString(Text, *ExecFile))
			{
				Text.ParseIntoArrayLines(ExecLines);
				bExecScript = true; ExecIdx = 0; ExecWaitUntil = FPlatformTime::Seconds() + 4.0;
				FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UFTGameInstance::TickExec));
				UE_LOG(LogFT, Display, TEXT("FT EXEC file=%s lines=%d"), *ExecFile, ExecLines.Num());
			}
			else { UE_LOG(LogFT, Error, TEXT("FT EXEC cannot read %s"), *ExecFile); }
		}
	}

	FString Bench;
	bAutoDrive = FParse::Param(FCommandLine::Get(), TEXT("ftauto"));
	FParse::Value(FCommandLine::Get(), TEXT("ftshots="), ShotEvery);
	FParse::Value(FCommandLine::Get(), TEXT("ftquit="), QuitAfter);
	FParse::Value(FCommandLine::Get(), TEXT("ftwrap="), WrapAfter);
	if (FParse::Value(FCommandLine::Get(), TEXT("ftbench="), Bench) || FParse::Param(FCommandLine::Get(), TEXT("ftbench")))
	{
		bBench = true;
		int32 L = 0; FParse::Value(FCommandLine::Get(), TEXT("ftlevel="), L);
		PendingLevel = FMath::Clamp(L, 0, FTData::NumLevels() - 1);
		bStartRace = true;
		FParse::Value(FCommandLine::Get(), TEXT("ftquality="), BenchQuality);
		FParse::Value(FCommandLine::Get(), TEXT("ftseconds="), BenchSeconds);
		FParse::Value(FCommandLine::Get(), TEXT("fttag="), BenchTag);
		ApplyQuality(BenchQuality, false);
		if (UGameUserSettings* S = GEngine ? GEngine->GetGameUserSettings() : nullptr)
		{
			S->SetVSyncEnabled(false);
			S->SetFrameRateLimit(0.f);
			S->ApplySettings(false);
		}
	}
	else
	{
		ApplyQuality(Quality, false);
	}
}

void UFTGameInstance::LoadSettings()
{
	FString Text;
	if (!FFileHelper::LoadFileToString(Text, *FPaths::Combine(SaveDir(), TEXT("settings.json")))) return;
	TSharedPtr<FJsonObject> J;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Text), J) || !J.IsValid()) return;
	if (J->GetIntegerField(TEXT("v")) != 2) return;
	const TSharedPtr<FJsonObject>* B;
	if (J->TryGetObjectField(TEXT("binds"), B))
	{
		for (int32 a = 0; a < NumActions; ++a)
		{
			const TArray<TSharedPtr<FJsonValue>>* Arr;
			if ((*B)->TryGetArrayField(ActionName(a), Arr) && Arr->Num() == 2)
			{
				for (int32 l = 0; l < 2; ++l)
				{
					const FKey K((*Arr)[l]->AsString().IsEmpty() ? NAME_None : FName(*(*Arr)[l]->AsString()));
					Binds[a][l] = (K.IsValid() && !IsReserved(K)) ? K : FKey();
				}
			}
		}
		for (int32 l = 0; l < 2; ++l) // duplicates within a layout are dropped
			for (int32 a = 0; a < NumActions; ++a)
				for (int32 b = a + 1; b < NumActions; ++b)
					if (Binds[a][l].IsValid() && Binds[a][l] == Binds[b][l]) Binds[b][l] = FKey();
	}
	Layout = FMath::Clamp((int32)J->GetIntegerField(TEXT("layout")), 0, 1);
	J->TryGetBoolField(TEXT("muted"), bMuted);
	J->TryGetNumberField(TEXT("quality"), Quality);
	J->TryGetBoolField(TEXT("fps"), bShowFps);
	J->TryGetBoolField(TEXT("vsync"), bVSync);
	const TArray<TSharedPtr<FJsonValue>>* Best;
	if (J->TryGetArrayField(TEXT("best"), Best))
	{
		for (int32 i = 0; i < MaxLevels && i < Best->Num(); ++i)
		{
			const TSharedPtr<FJsonObject> O = (*Best)[i]->AsObject();
			if (!O) continue;
			BestScore[i] = O->GetIntegerField(TEXT("score"));
			BestTitle[i] = O->GetStringField(TEXT("title"));
			O->TryGetNumberField(TEXT("lv"), BestLv[i]);
		}
	}
	J->TryGetBoolField(TEXT("reducedMotion"), bReducedMotion);
	const TSharedPtr<FJsonObject>* Sk;
	if (J->TryGetObjectField(TEXT("skills"), Sk)) LegacySkills = *Sk;   // 2.3's points and ranks: kept as they are
}

void UFTGameInstance::SaveSettings()
{
	if (bBench) return;
	TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
	J->SetNumberField(TEXT("v"), 2);
	TSharedRef<FJsonObject> B = MakeShared<FJsonObject>();
	for (int32 a = 0; a < NumActions; ++a)
	{
		TArray<TSharedPtr<FJsonValue>> Arr;
		for (int32 l = 0; l < 2; ++l) Arr.Add(MakeShared<FJsonValueString>(Binds[a][l].IsValid() ? Binds[a][l].GetFName().ToString() : FString()));
		B->SetArrayField(ActionName(a), Arr);
	}
	J->SetObjectField(TEXT("binds"), B);
	J->SetNumberField(TEXT("layout"), Layout);
	J->SetBoolField(TEXT("muted"), bMuted);
	J->SetNumberField(TEXT("quality"), Quality);
	J->SetBoolField(TEXT("fps"), bShowFps);
	J->SetBoolField(TEXT("vsync"), bVSync);
	TArray<TSharedPtr<FJsonValue>> Best;
	for (int32 i = 0; i < MaxLevels; ++i)
	{
		TSharedRef<FJsonObject> O = MakeShared<FJsonObject>();
		O->SetNumberField(TEXT("score"), BestScore[i]);
		O->SetStringField(TEXT("title"), BestTitle[i]);
		O->SetNumberField(TEXT("lv"), BestLv[i]);
		Best.Add(MakeShared<FJsonValueObject>(O));
	}
	J->SetArrayField(TEXT("best"), Best);
	J->SetBoolField(TEXT("reducedMotion"), bReducedMotion);
	if (LegacySkills.IsValid()) J->SetObjectField(TEXT("skills"), LegacySkills);
	FString Out;
	FJsonSerializer::Serialize(J, TJsonWriterFactory<>::Create(&Out));
	IFileManager::Get().MakeDirectory(*SaveDir(), true);
	FFileHelper::SaveStringToFile(Out, *FPaths::Combine(SaveDir(), TEXT("settings.json")));
}

void UFTGameInstance::ApplyQuality(int32 Q, bool bSave)
{
	Quality = Q;
	UGameUserSettings* S = GEngine ? GEngine->GetGameUserSettings() : nullptr;
	if (S)
	{
		if (Q < 0)
		{
			if (!bAutoMeasured) { S->RunHardwareBenchmark(); bAutoMeasured = true; } // once a session
			S->ApplyHardwareBenchmarkResults();
		}
		else
		{
			S->SetOverallScalabilityLevel(FMath::Clamp(Q, 0, 4));
		}
		if (!bBench) S->SetVSyncEnabled(bVSync);
		// 2.7: on a graphics card that can't hold 60 fps, TSR renders fewer pixels instead of dropping frames.
		// The preset's own resolution is the most it draws (so a fast PC sees no change); the least is half.
		// -ftnodynres turns it off (a fixed-resolution benchmark).
		const bool bDyn = !FParse::Param(FCommandLine::Get(), TEXT("ftnodynres"));
		S->SetDynamicResolutionEnabled(bDyn);
		S->ApplySettings(false);
		if (IConsoleVariable* RQ = IConsoleManager::Get().FindConsoleVariable(TEXT("sg.ResolutionQuality")))
		{
			const float Top = FMath::Clamp(RQ->GetFloat() > 0.f ? RQ->GetFloat() : 100.f, 50.f, 100.f);
			if (IConsoleVariable* V = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DynamicRes.MaxScreenPercentage"))) V->Set(Top, ECVF_SetByGameSetting);
			if (IConsoleVariable* V = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DynamicRes.MinScreenPercentage"))) V->Set(FMath::Min(Top, 50.f), ECVF_SetByGameSetting);
		}
		UE_LOG(LogFT, Display, TEXT("FTQUALITY applied=%d overall=%d dynres=%d"), Q, S->GetOverallScalabilityLevel(), bDyn ? 1 : 0);
	}
	if (bSave) SaveSettings();
}

void UFTGameInstance::ReapplyQuality()
{
	// the engine applies its own saved GameUserSettings after Init, which put every scalability group back
	// to Epic; so the preset is applied again each time a map starts (not in the editor: PIE shares its scalability)
	if (GIsEditor) return;
	ApplyQuality(bBench ? BenchQuality : Quality, false);
	if (bBench)
	{
		if (UGameUserSettings* S = GEngine ? GEngine->GetGameUserSettings() : nullptr)
		{
			S->SetVSyncEnabled(false);
			S->SetFrameRateLimit(0.f);
			S->ApplySettings(false);
		}
	}
}

void UFTGameInstance::ApplyMute()
{
	if (GEngine)
	{
		if (FAudioDeviceHandle Dev = GEngine->GetMainAudioDevice()) Dev->SetTransientPrimaryVolume(bMuted ? 0.f : 1.f);
	}
}

// --------------------------------------------------------------------------------------------- -ftexec test runner
namespace
{
	/** Keeps the FT log lines for "waitfor" (the runner's only view of the game). */
	class FFTLogTap : public FOutputDevice
	{
	public:
		FCriticalSection Lock;
		TArray<FString> Lines;
		virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category) override
		{
			if (!V || FCString::Strncmp(V, TEXT("FT "), 3) != 0) return;
			FScopeLock L(&Lock);
			Lines.Add(V);
		}
	};
	FFTLogTap* GTap = nullptr;
}

void UFTGameInstance::Shutdown()
{
	if (GTap) { GLog->RemoveOutputDevice(GTap); delete GTap; GTap = nullptr; }
	Super::Shutdown();
}

bool UFTGameInstance::TickExec(float Dt)
{
	if (!bExecScript) return false;
	if (!GTap) { GTap = new FFTLogTap(); GLog->AddOutputDevice(GTap); }
	const double Now = FPlatformTime::Seconds();
	if (Now < ExecWaitUntil) return true;
	if (!ExecWaitFor.IsEmpty())
	{
		bool bFound = false;
		{
			FScopeLock L(&GTap->Lock);
			for (int32 i = ExecWaitFrom; i < GTap->Lines.Num() && !bFound; ++i) bFound = !GTap->Lines[i].StartsWith(TEXT("FT EXEC")) && !GTap->Lines[i].StartsWith(TEXT("FT HARNESS")) && GTap->Lines[i].Contains(ExecWaitFor);
		}
		if (!bFound && Now < ExecWaitForUntil) return true;
		UE_LOG(LogFT, Display, TEXT("FT EXEC waitfor=\"%s\" result=%s"), *ExecWaitFor, bFound ? TEXT("found") : TEXT("timeout"));
		ExecWaitFor.Reset();
	}
	UWorld* W = GetWorld();
	if (!W || !GEngine) return true;
	while (ExecIdx < ExecLines.Num())
	{
		FString Line = ExecLines[ExecIdx++].TrimStartAndEnd();
		if (Line.IsEmpty() || Line.StartsWith(TEXT("#"))) continue;
		UE_LOG(LogFT, Display, TEXT("FT EXEC line=%d cmd=%s"), ExecIdx, *Line);
		if (Line.StartsWith(TEXT("wait ")))
		{
			ExecWaitUntil = Now + FCString::Atof(*Line.RightChop(5));
			return true;
		}
		if (Line.StartsWith(TEXT("waitfor ")))
		{
			FString Rest = Line.RightChop(8), Text = Rest; float Timeout = 30.f;
			int32 Bar = INDEX_NONE;
			if (Rest.FindLastChar(TEXT('|'), Bar)) { Text = Rest.Left(Bar).TrimEnd(); Timeout = FCString::Atof(*Rest.RightChop(Bar + 1)); }
			// lines since the last command count, so a fast event is never missed
			ExecWaitFor = Text; ExecWaitForUntil = Now + Timeout;
			return true;
		}
		if (Line.StartsWith(TEXT("shot ")))
		{
			FScreenshotRequest::RequestScreenshot(FPaths::Combine(SaveDir(), TEXT("shots"), Line.RightChop(5) + TEXT(".png")), true, false);
			ExecWaitUntil = Now + 0.5;
			return true;
		}
		if (Line == TEXT("quit")) { FPlatformMisc::RequestExit(false); return false; }
		{ FScopeLock L(&GTap->Lock); ExecWaitFrom = GTap->Lines.Num(); }
		GEngine->Exec(W, *Line);
		ExecWaitUntil = Now + 0.15;   // one command, then a few frames
		return true;
	}
	UE_LOG(LogFT, Display, TEXT("FT EXEC done"));
	bExecScript = false;
	return false;
}
