#include "FTGameInstance.h"
#include "FTCore.h"
#include "FTSkills.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/Engine.h"
#include "AudioDevice.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/CommandLine.h"
#include "HAL/PlatformFileManager.h"
#include "HAL/PlatformMisc.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

const TCHAR* UFTGameInstance::ActionName(int32 A)
{
	static const TCHAR* N[] = { TEXT("STEER LEFT"), TEXT("STEER RIGHT"), TEXT("THROTTLE UP"), TEXT("BRAKE"), TEXT("DRIFT"), TEXT("TURBO"), TEXT("FIRE GUNS"), TEXT("MISSILE") };
	return N[FMath::Clamp(A, 0, NumActions - 1)];
}

bool UFTGameInstance::IsReserved(const FKey& K)
{
	return K == EKeys::Escape || K == EKeys::P || K == EKeys::M || K == EKeys::Enter || K == EKeys::Tab || K == EKeys::BackSpace
		|| K == EKeys::Delete || K == EKeys::F1 || K == EKeys::O || K.IsMouseButton() || K.IsGamepadKey() || K.IsAxis1D() || K.IsAxis2D();
}

void UFTGameInstance::ResetBinds()
{
	const FKey W[NumActions] = { EKeys::A, EKeys::D, EKeys::W, EKeys::S, EKeys::LeftShift, EKeys::SpaceBar, EKeys::J, EKeys::K };
	const FKey R[NumActions] = { EKeys::Left, EKeys::Right, EKeys::Up, EKeys::Down, EKeys::LeftShift, EKeys::SpaceBar, EKeys::Z, EKeys::X };
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

	FString Bench;
	bAutoDrive = FParse::Param(FCommandLine::Get(), TEXT("ftauto"));
	FParse::Value(FCommandLine::Get(), TEXT("ftshots="), ShotEvery);
	FParse::Value(FCommandLine::Get(), TEXT("ftquit="), QuitAfter);
	FParse::Value(FCommandLine::Get(), TEXT("ftwrap="), WrapAfter);
	if (FParse::Value(FCommandLine::Get(), TEXT("ftbench="), Bench) || FParse::Param(FCommandLine::Get(), TEXT("ftbench")))
	{
		bBench = true;
		int32 L = 0; FParse::Value(FCommandLine::Get(), TEXT("ftlevel="), L);
		PendingLevel = FMath::Clamp(L, 0, 2);
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
		for (int32 i = 0; i < 3 && i < Best->Num(); ++i)
		{
			const TSharedPtr<FJsonObject> O = (*Best)[i]->AsObject();
			if (!O) continue;
			BestScore[i] = O->GetIntegerField(TEXT("score"));
			BestTitle[i] = O->GetStringField(TEXT("title"));
		}
	}
	const TSharedPtr<FJsonObject>* Sk;
	if (J->TryGetObjectField(TEXT("skills"), Sk))
	{
		(*Sk)->TryGetNumberField(TEXT("points"), SkillPoints);
		(*Sk)->TryGetNumberField(TEXT("earned"), SkillEarned);
		SkillPoints = FMath::Max(0, SkillPoints);
		const TSharedPtr<FJsonObject>* Ranks;
		if ((*Sk)->TryGetObjectField(TEXT("ranks"), Ranks))
		{
			const FFTSkillData& D = FFTSkillData::Get();
			for (const auto& KV : (*Ranks)->Values)
			{
				const FFTUpgradeDef* U = D.Find(FName(*KV.Key));
				// an upgrade that left the data keeps its rank in the save (it may come back in the tree)
				SkillRanks.Add(FName(*KV.Key), FMath::Clamp((int32)KV.Value->AsNumber(), 0, U ? U->MaxRank : 99));
			}
		}
	}
}

// --------------------------------------------------------------------------------------------- skill points
float UFTGameInstance::Stat(FName StatId) const
{
	const FFTSkillData& D = FFTSkillData::Get();
	float Sum = 0.f;
	for (const FFTUpgradeDef& U : D.Upgrades)
	{
		const int32 R = Rank(U.Id);
		if (R <= 0) continue;
		for (const FFTSkillEffect& E : U.Effects) if (E.Stat == StatId) Sum += E.PerRank * R;
	}
	const FFTStatDef* S = D.Stats.Find(StatId);
	if (S && S->bAdd) return Sum;
	return FMath::Max(0.2f, 1.f + Sum);
}

bool UFTGameInstance::IsLocked(int32 Idx, FString* Why) const
{
	const FFTSkillData& D = FFTSkillData::Get();
	if (!D.Upgrades.IsValidIndex(Idx)) return true;
	for (const FFTSkillReq& Q : D.Upgrades[Idx].Requires)
	{
		if (Rank(Q.Id) >= Q.Rank) continue;
		if (Why) { const FFTUpgradeDef* U = D.Find(Q.Id); *Why = FString::Printf(TEXT("NEEDS %s RANK %d"), U ? *U->Name : *Q.Id.ToString().ToUpper(), Q.Rank); }
		return true;
	}
	return false;
}

int32 UFTGameInstance::NextCost(int32 Idx) const
{
	const FFTSkillData& D = FFTSkillData::Get();
	if (!D.Upgrades.IsValidIndex(Idx)) return -1;
	const FFTUpgradeDef& U = D.Upgrades[Idx];
	const int32 R = Rank(U.Id);
	return R >= U.MaxRank ? -1 : U.CostOf(R + 1);
}

bool UFTGameInstance::CanBuy(int32 Idx) const
{
	const int32 C = NextCost(Idx);
	return C >= 0 && C <= SkillPoints && !IsLocked(Idx);
}

bool UFTGameInstance::Buy(int32 Idx)
{
	if (!CanBuy(Idx)) return false;
	const FFTUpgradeDef& U = FFTSkillData::Get().Upgrades[Idx];
	SkillPoints -= NextCost(Idx);
	SkillRanks.FindOrAdd(U.Id)++;
	SaveSettings();
	return true;
}

bool UFTGameInstance::CanRefund(int32 Idx) const
{
	const FFTSkillData& D = FFTSkillData::Get();
	if (!D.Upgrades.IsValidIndex(Idx)) return false;
	const FFTUpgradeDef& U = D.Upgrades[Idx];
	const int32 R = Rank(U.Id);
	if (R <= 0) return false;
	// not below a rank another bought upgrade stands on
	for (const FFTUpgradeDef& O : D.Upgrades)
		if (Rank(O.Id) > 0)
			for (const FFTSkillReq& Q : O.Requires) if (Q.Id == U.Id && Q.Rank >= R) return false;
	return true;
}

bool UFTGameInstance::Refund(int32 Idx)
{
	if (!CanRefund(Idx)) return false;
	const FFTUpgradeDef& U = FFTSkillData::Get().Upgrades[Idx];
	const int32 R = Rank(U.Id);
	SkillPoints += U.CostOf(R);
	SkillRanks.Add(U.Id, R - 1);
	SaveSettings();
	return true;
}

void UFTGameInstance::AddSkillPoints(int32 N)
{
	if (N <= 0) return;
	SkillPoints += N; SkillEarned += N;
	SaveSettings();
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
	for (int32 i = 0; i < 3; ++i)
	{
		TSharedRef<FJsonObject> O = MakeShared<FJsonObject>();
		O->SetNumberField(TEXT("score"), BestScore[i]);
		O->SetStringField(TEXT("title"), BestTitle[i]);
		Best.Add(MakeShared<FJsonValueObject>(O));
	}
	J->SetArrayField(TEXT("best"), Best);
	TSharedRef<FJsonObject> Sk = MakeShared<FJsonObject>();
	Sk->SetNumberField(TEXT("points"), SkillPoints);
	Sk->SetNumberField(TEXT("earned"), SkillEarned);
	TSharedRef<FJsonObject> Ranks = MakeShared<FJsonObject>();
	for (const TPair<FName, int32>& R : SkillRanks) if (R.Value > 0) Ranks->SetNumberField(R.Key.ToString(), R.Value);
	Sk->SetObjectField(TEXT("ranks"), Ranks);
	J->SetObjectField(TEXT("skills"), Sk);
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
		S->ApplySettings(false);
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
