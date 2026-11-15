#include "FTGameInstance.h"
#include "UObject/UObjectGlobals.h"
#include "FTCore.h"
#include "FTProgress.h"
#include "Containers/Ticker.h"
#include "UnrealClient.h"
#include "Misc/OutputDeviceRedirector.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/GameViewportClient.h"
#include "UnrealEngine.h"
#include "RHI.h"
#include "Engine/Engine.h"
#include "AudioDevice.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
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
#include "Engine/AssetManager.h"
#include "AssetRegistry/IAssetRegistry.h"

const TCHAR* UFTGameInstance::ActionName(int32 A)
{
	static const TCHAR* N[] = { TEXT("STEER LEFT"), TEXT("STEER RIGHT"), TEXT("THROTTLE UP"), TEXT("BRAKE"), TEXT("DRIFT"), TEXT("TURBO"), TEXT("FIRE GUNS"), TEXT("MISSILE"), TEXT("AMMO") };
	return N[FMath::Clamp(A, 0, NumActions - 1)];
}

bool UFTGameInstance::IsReserved(const FKey& K)
{
	return K == EKeys::AnyKey || K == EKeys::Escape || K == EKeys::P || K == EKeys::M || K == EKeys::Enter || K == EKeys::Tab || K == EKeys::BackSpace
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
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UFTGameInstance::OnPostLoadMap);
	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UFTGameInstance::TickHitch));   // loop cycle 6: mute survives map loads
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
#if !UE_BUILD_SHIPPING   // a test hook that pays out a full wrap after N seconds: not for a Shipping build
	FParse::Value(FCommandLine::Get(), TEXT("ftwrap="), WrapAfter);
#endif
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
	const FString SettingsPath = FPaths::Combine(SaveDir(), TEXT("settings.json"));
	int32 SettingsV = 0;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Text), J) || !J.IsValid() || !J->TryGetNumberField(TEXT("v"), SettingsV) || SettingsV != 2)
	{
		// unreadable or another version: the defaults are used, and the next save would write over the file (best
		// takes, key bindings). A copy is kept beside it first.
		IFileManager::Get().Copy(*(SettingsPath + TEXT(".unread")), *SettingsPath, true, true);
		UE_LOG(LogFT, Warning, TEXT("FT LOAD data=settings.json result=unreadable kept=settings.json.unread"));
		return;
	}
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
	const TSharedPtr<FJsonObject>* Au;
	if (J->TryGetObjectField(TEXT("auto"), Au))
	{
		(*Au)->TryGetStringField(TEXT("key"), AutoCacheKey);
		(*Au)->TryGetNumberField(TEXT("res"), AutoCacheRes);
		(*Au)->TryGetNumberField(TEXT("cpu"), AutoCacheCpu);
		(*Au)->TryGetNumberField(TEXT("gpu"), AutoCacheGpu);
		const TArray<TSharedPtr<FJsonValue>>* Lv;
		AutoCache.Reset();
		if ((*Au)->TryGetArrayField(TEXT("levels"), Lv)) for (const TSharedPtr<FJsonValue>& V : *Lv) AutoCache.Add(FMath::Clamp((int32)V->AsNumber(), 0, 4));
	}
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
	if (AutoCache.Num() == 11 && !AutoCacheKey.IsEmpty())
	{
		TSharedRef<FJsonObject> Au = MakeShared<FJsonObject>();
		Au->SetStringField(TEXT("key"), AutoCacheKey);
		TArray<TSharedPtr<FJsonValue>> Lv;
		for (int32 V : AutoCache) Lv.Add(MakeShared<FJsonValueNumber>(V));
		Au->SetArrayField(TEXT("levels"), Lv);
		Au->SetNumberField(TEXT("res"), AutoCacheRes);
		Au->SetNumberField(TEXT("cpu"), AutoCacheCpu);
		Au->SetNumberField(TEXT("gpu"), AutoCacheGpu);
		J->SetObjectField(TEXT("auto"), Au);
	}
	if (LegacySkills.IsValid()) J->SetObjectField(TEXT("skills"), LegacySkills);
	FString Out;
	FJsonSerializer::Serialize(J, TJsonWriterFactory<>::Create(&Out));
	// beside the file, then moved over it (as the career is saved): a crash mid-write cannot leave half a file
	IFileManager& FM = IFileManager::Get();
	FM.MakeDirectory(*SaveDir(), true);
	const FString Path = FPaths::Combine(SaveDir(), TEXT("settings.json")), Tmp = Path + TEXT(".tmp");
	if (FFileHelper::SaveStringToFile(Out, *Tmp)) FM.Move(*Path, *Tmp, true, true);
}

bool UFTGameInstance::bHarnessApple = false;

bool UFTGameInstance::IsAppleGpu()
{
	return IsRHIDeviceApple() || bHarnessApple || FParse::Param(FCommandLine::Get(), TEXT("ftfakeapple"));
}

// 2.9, from the Mac performance audit (Apple M1 Max, 14 Nov 2026). Unreal's hardware benchmark rates Apple GPUs far above
// what they deliver, so AUTO picked EPIC and every level ran 19-27 fps there; the game thread was ~3 ms, so it was all GPU.
// On an Apple GPU, AUTO now:
//  - caps view distance, anti-aliasing, shadows, post, foliage, shading and landscape at HIGH;
//  - keeps effects and textures as measured (cheap, and the fire and smoke carry the look);
//  - keeps GI and reflections as measured (EPIC, so Lumen stays on): below EPIC this project turns Lumen off (2.7), and the
//    audit showed Metro's tower lobby losing its warm bounce light without it. 2.8 already runs EPIC Lumen at HIGH-gather cost;
//  - caps the render target at 1.65 megapixels (67% at 2560x1440, 89% at 1920x1080) and lets TSR upscale it to the screen;
//    dynamic resolution then goes from 50% up to that cap to hold 60 fps.
// Every manual preset (LOW..EPIC) is unchanged on every GPU.
float UFTGameInstance::CapAutoForApple(UGameUserSettings* S) const
{
	auto High = [](int32 V) { return FMath::Min(V, 2); };
	auto Epic = [](int32 V) { return FMath::Min(V, 3); };
	S->SetViewDistanceQuality(High(S->GetViewDistanceQuality()));
	S->SetAntiAliasingQuality(High(S->GetAntiAliasingQuality()));
	S->SetShadowQuality(High(S->GetShadowQuality()));
	S->SetPostProcessingQuality(High(S->GetPostProcessingQuality()));
	S->SetFoliageQuality(High(S->GetFoliageQuality()));
	S->SetShadingQuality(High(S->GetShadingQuality()));
	S->SetLandscapeQuality(High(S->GetLandscapeQuality()));
	S->SetGlobalIlluminationQuality(Epic(S->GetGlobalIlluminationQuality()));
	S->SetReflectionQuality(Epic(S->GetReflectionQuality()));
	S->SetVisualEffectQuality(Epic(S->GetVisualEffectQuality()));
	S->SetTextureQuality(Epic(S->GetTextureQuality()));
	// the screen the game draws to: the game viewport once it exists (each map start re-applies), else the chosen resolution
	FIntPoint Px(0, 0);
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->Viewport) Px = GEngine->GameViewport->Viewport->GetSizeXY();
	if (Px.X <= 0 || Px.Y <= 0) Px = S->GetScreenResolution();
	if (Px.X <= 0 || Px.Y <= 0) Px = FIntPoint(GSystemResolution.ResX, GSystemResolution.ResY);
	if (Px.X <= 0 || Px.Y <= 0) Px = FIntPoint(2560, 1440);
	constexpr double MaxPixels = 1.65e6;
	const float Cap = FMath::Clamp((float)(100.0 * FMath::Sqrt(MaxPixels / ((double)Px.X * Px.Y))), 50.f, 100.f);
	float Cur = 0.f, CurN = 0.f, Mn = 0.f, Mx = 0.f;
	S->GetResolutionScaleInformationEx(CurN, Cur, Mn, Mx);
	S->SetResolutionScaleValueEx(Cur > 0.f ? FMath::Min(Cur, Cap) : Cap);
	return Cur > 0.f ? FMath::Min(Cur, Cap) : Cap;
}

void UFTGameInstance::ApplyQuality(int32 Q, bool bSave)
{
	Quality = Q;
	UGameUserSettings* S = GEngine ? GEngine->GetGameUserSettings() : nullptr;
	if (S)
	{
		float AppleCap = 0.f;
		if (Q < 0)
		{
			// once a session; the result is kept, so AUTO after picking another preset is AUTO again
			// (until 2.8, ApplyHardwareBenchmarkResults re-applied whatever preset was picked last)
			if (!bAutoMeasured && AutoCacheKey == AutoKey() && AutoCache.Num() == 11 && !FParse::Param(FCommandLine::Get(), TEXT("ftrebench")))
			{
				Scalability::FQualityLevels& A = AutoLevels;
				A = S->ScalabilityQuality;
				A.ResolutionQuality = AutoCacheRes;
				int32* const Groups[11] = { &A.ViewDistanceQuality, &A.AntiAliasingQuality, &A.ShadowQuality, &A.GlobalIlluminationQuality, &A.ReflectionQuality,
					&A.PostProcessQuality, &A.TextureQuality, &A.EffectsQuality, &A.FoliageQuality, &A.ShadingQuality, &A.LandscapeQuality };
				for (int32 i = 0; i < 11; ++i) *Groups[i] = AutoCache[i];
				A.CPUBenchmarkResults = AutoCacheCpu; A.GPUBenchmarkResults = AutoCacheGpu;
				bAutoMeasured = true;
				UE_LOG(LogFT, Display, TEXT("FT AUTO cached cpuIndex=%.0f gpuIndex=%.0f overall=%d shadow=%d gi=%d res=%.0f"), AutoCacheCpu, AutoCacheGpu, A.GetMinQualityLevel(), A.ShadowQuality, A.GlobalIlluminationQuality, A.ResolutionQuality);
			}
			if (!bAutoMeasured)
			{
				// half Unreal's default work (10): the first launch is a judge's first impression
				S->RunHardwareBenchmark(5); AutoLevels = S->ScalabilityQuality; bAutoMeasured = true;
				{
					const Scalability::FQualityLevels& A = AutoLevels;
					AutoCacheKey = AutoKey();
					AutoCache = { A.ViewDistanceQuality, A.AntiAliasingQuality, A.ShadowQuality, A.GlobalIlluminationQuality, A.ReflectionQuality,
						A.PostProcessQuality, A.TextureQuality, A.EffectsQuality, A.FoliageQuality, A.ShadingQuality, A.LandscapeQuality };
					AutoCacheRes = A.ResolutionQuality; AutoCacheCpu = A.CPUBenchmarkResults; AutoCacheGpu = A.GPUBenchmarkResults;
					bSave = true;
				}
				UE_LOG(LogFT, Display, TEXT("FT AUTO cpuIndex=%.0f gpuIndex=%.0f overall=%d shadow=%d gi=%d res=%.0f"), S->GetLastCPUBenchmarkResult(), S->GetLastGPUBenchmarkResult(),
					AutoLevels.GetMinQualityLevel(), AutoLevels.ShadowQuality, AutoLevels.GlobalIlluminationQuality, AutoLevels.ResolutionQuality);
			}
			S->ScalabilityQuality = AutoLevels;
			if (IsAppleGpu()) AppleCap = CapAutoForApple(S);
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
		// 2.8: anti-aliasing per preset. TSR was a third of the GPU frame at MEDIUM and LOW (0.4 of 1.4 ms at LOW on
		// this PC, far more on a small card); TAA does the same job (and the same dynamic-resolution upscale) for a
		// fraction of it. HIGH and EPIC keep TSR. -fttsr keeps TSR everywhere (before/after checks).
		int32 Aa = 4;
		if (IConsoleVariable* AQ = IConsoleManager::Get().FindConsoleVariable(TEXT("sg.AntiAliasingQuality")))
			if (AQ->GetInt() <= 1 && !FParse::Param(FCommandLine::Get(), TEXT("fttsr"))) Aa = 2;
		if (IConsoleVariable* V = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AntiAliasingMethod"))) V->Set(Aa, ECVF_SetByCode);
		// 3.4: Lumen reflections (EPIC only) trace only glossy surfaces (glass, chrome, wet paint); rougher ones take the
		// radiance cache. Metro's EPIC frame -0.5 ms, same look side by side (full-throttle-tests/v340/look). Not a
		// scalability variable (the .ini can't set it), so it is set here; below EPIC Lumen reflections are off anyway.
		if (IConsoleVariable* V = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Lumen.Reflections.MaxRoughnessToTrace"))) V->Set(0.15f, ECVF_SetByCode);
		UE_LOG(LogFT, Display, TEXT("FTQUALITY applied=%d overall=%d dynres=%d aa=%s"), Q, S->GetOverallScalabilityLevel(), bDyn ? 1 : 0, Aa == 4 ? TEXT("TSR") : TEXT("TAA"));
		if (AppleCap > 0.f)
		{
			UE_LOG(LogFT, Display, TEXT("FT QUALITY auto=apple cap=HIGH view=%d aa=%d shadow=%d gi=%d refl=%d pp=%d fx=%d tex=%d screenPercentage=%.0f dynres=%s"),
				S->GetViewDistanceQuality(), S->GetAntiAliasingQuality(), S->GetShadowQuality(), S->GetGlobalIlluminationQuality(), S->GetReflectionQuality(),
				S->GetPostProcessingQuality(), S->GetVisualEffectQuality(), S->GetTextureQuality(), AppleCap,
				bDyn ? *FString::Printf(TEXT("%.0f-%.0f"), FMath::Min(AppleCap, 50.f), AppleCap) : TEXT("off"));
		}
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

void UFTGameInstance::PlayTitleMusic(UObject* Ctx, float Volume)
{
	if (!TitleMusic)
	{
		USoundBase* Song = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("/Game/Audio/Music/M_TitleTheme"), nullptr, LOAD_NoWarn | LOAD_Quiet));
		// bPersistAcrossLevelTransition: the attract mode's map loads don't cut the song
		TitleMusic = UGameplayStatics::CreateSound2D(Ctx, Song, Volume, 1.f, 0.f, nullptr, true, false);
		if (TitleMusic) TitleMusic->bIsUISound = true;
		bTitleMusicOn = false;
	}
	if (!TitleMusic) return;
	TitleMusic->SetVolumeMultiplier(Volume);
	if (!bTitleMusicOn || !TitleMusic->IsPlaying())
	{
		TitleMusic->Play();
		bTitleMusicOn = true;
		UE_LOG(LogFT, Display, TEXT("FTMUSIC title=%s playing=1"), TitleMusic->Sound ? *TitleMusic->Sound->GetName() : TEXT("none"));
	}
}

void UFTGameInstance::SetTitleMusicVolume(float Volume)
{
	if (TitleMusic && bTitleMusicOn) TitleMusic->SetVolumeMultiplier(Volume);
}

void UFTGameInstance::StopTitleMusic(float FadeSeconds)
{
	if (!TitleMusic || !bTitleMusicOn) return;
	bTitleMusicOn = false;
	if (FadeSeconds > 0.f) TitleMusic->FadeOut(FadeSeconds, 0.f);
	else TitleMusic->Stop();
	UE_LOG(LogFT, Display, TEXT("FTMUSIC title stop fade=%.1f"), FadeSeconds);
}

void UFTGameInstance::OnPostLoadMap(UWorld* World)
{
	ApplyMute();
	if (!bBrawlWarmStarted && World && World->GetOutermost()->GetName().StartsWith(TEXT("/Game/Brawl/")) && UAssetManager::IsInitialized())
	{
		bBrawlWarmStarted = true;
		TArray<FAssetData> Found;
		UAssetManager::Get().GetAssetRegistry().GetAssetsByPath(FName(TEXT("/Game/Brawl/Sprites")), Found, true, true);
		TArray<FSoftObjectPath> Paths;
		for (const FAssetData& A : Found) Paths.Add(A.GetSoftObjectPath());
		const double T0 = FPlatformTime::Seconds(); const int32 Count = Paths.Num();
		if (Count > 0)
		{
			TSharedPtr<FStreamableHandle> H = UAssetManager::GetStreamableManager().RequestAsyncLoad(MoveTemp(Paths),
				FStreamableDelegate::CreateWeakLambda(this, [T0, Count]() { UE_LOG(LogFT, Display, TEXT("FT WARM brawl done assets=%d seconds=%.2f"), Count, FPlatformTime::Seconds() - T0); }),
				FStreamableManager::AsyncLoadHighPriority, true);
			if (H.IsValid()) WarmHandles.Add(H);
		}
		UE_LOG(LogFT, Display, TEXT("FT WARM brawl start assets=%d"), Count);
	}
	UE_LOG(LogFT, Display, TEXT("FT MUTE reapplied muted=%d map=%s"), bMuted ? 1 : 0, World ? *World->GetMapName() : TEXT("-"));
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
			// (3.9: the brawl's FTBRAWL and the input's FTINPUT lines too, so a script can wait on the nights' screens)
			if (!V || (FCString::Strncmp(V, TEXT("FT "), 3) != 0 && FCString::Strncmp(V, TEXT("FTBRAWL "), 8) != 0 && FCString::Strncmp(V, TEXT("FTINPUT "), 8) != 0)) return;
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

// --------------------------------------------------------------------------------------------- loop cycle 1: warm the gameplay assets
void UFTGameInstance::WarmGameplayAssets()
{
	if (bWarmStarted || !UAssetManager::IsInitialized()) return;
	bWarmStarted = true;
	IAssetRegistry& AR = UAssetManager::Get().GetAssetRegistry();
	TArray<FSoftObjectPath> Paths;
	const auto AddDir = [&](const TCHAR* Dir, bool bRecursive)
	{
		TArray<FAssetData> Found;
		AR.GetAssetsByPath(FName(Dir), Found, bRecursive, true);
		for (const FAssetData& A : Found) Paths.Add(A.GetSoftObjectPath());
	};
	AddDir(TEXT("/Game/Audio/SFX"), true);
	AddDir(TEXT("/Game/FX"), true);
	AddDir(TEXT("/Game/Characters"), true);
	const double T0 = FPlatformTime::Seconds();
	const int32 Count = Paths.Num();
	if (Count == 0) { UE_LOG(LogFT, Warning, TEXT("FT WARM no assets found (asset registry empty?)")); return; }
	TSharedPtr<FStreamableHandle> H = UAssetManager::GetStreamableManager().RequestAsyncLoad(MoveTemp(Paths),
		FStreamableDelegate::CreateWeakLambda(this, [T0, Count]()
		{
			UE_LOG(LogFT, Display, TEXT("FT WARM done assets=%d seconds=%.2f"), Count, FPlatformTime::Seconds() - T0);
		}), FStreamableManager::AsyncLoadHighPriority, true);
	if (H.IsValid()) WarmHandles.Add(H);
	UE_LOG(LogFT, Display, TEXT("FT WARM start assets=%d"), Count);
}

FString UFTGameInstance::AutoKey()
{
	// the card, and the thresholds in DefaultScalability.ini (bump the tag when they change)
	return GRHIAdapterName + TEXT("|epic230|v1");
}

bool UFTGameInstance::TickHitch(float Dt)
{
	const double Now = FPlatformTime::Seconds();
	const double Ms = HitchLastT > 0.0 ? (Now - HitchLastT) * 1000.0 : 0.0;
	HitchLastT = Now;
	// map loads are expected stalls: only frames of a running world count (the race's own FTHITCH says more there)
	UWorld* W = GetWorld();
	if (Ms > 40.0 && W && W->HasBegunPlay() && W->GetTimeSeconds() > 1.f)
	{
		UE_LOG(LogFT, Display, TEXT("FTHITCH2 ms=%.0f map=%s t=%.1f"), Ms, *W->GetMapName(), W->GetTimeSeconds());
	}
	return true;
}
