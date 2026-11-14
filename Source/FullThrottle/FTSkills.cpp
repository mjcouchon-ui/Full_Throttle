#include "FTSkills.h"
#include "FTCore.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

namespace
{
	FLinearColor HexColor(const FString& Hex)
	{
		const FColor C = FColor::FromHex(Hex);
		// the HUD's colours are plain 0..1 sRGB values, so no gamma conversion here
		return FLinearColor(C.R / 255.f, C.G / 255.f, C.B / 255.f, 1.f);
	}

	FFTSkillData LoadSkills()
	{
		FFTSkillData D;
		const FString Path = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Data"), TEXT("skills.json"));
		FString Text;
		if (!FFileHelper::LoadFileToString(Text, *Path)) { D.Error = TEXT("skills.json not found"); UE_LOG(LogFT, Error, TEXT("FTSKILLS cannot read %s"), *Path); return D; }
		TSharedPtr<FJsonObject> J;
		if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Text), J) || !J.IsValid()) { D.Error = TEXT("skills.json is not valid JSON"); UE_LOG(LogFT, Error, TEXT("FTSKILLS bad JSON in %s"), *Path); return D; }

		const TSharedPtr<FJsonObject>* O;
		if (J->TryGetObjectField(TEXT("points"), O))
			for (const auto& KV : (*O)->Values) D.Points.Add(FName(*KV.Key), (int32)KV.Value->AsNumber());
		if (J->TryGetObjectField(TEXT("pointLabels"), O))
			for (const auto& KV : (*O)->Values) D.PointLabels.Add(FName(*KV.Key), KV.Value->AsString());
		if (J->TryGetObjectField(TEXT("stats"), O))
			for (const auto& KV : (*O)->Values)
			{
				const TSharedPtr<FJsonObject> S = KV.Value->AsObject();
				if (!S) continue;
				FFTStatDef Def; Def.Label = S->GetStringField(TEXT("label")); Def.bAdd = S->GetStringField(TEXT("kind")) == TEXT("add");
				D.Stats.Add(FName(*KV.Key), Def);
			}
		const TArray<TSharedPtr<FJsonValue>>* Ups;
		if (J->TryGetArrayField(TEXT("upgrades"), Ups))
			for (const TSharedPtr<FJsonValue>& V : *Ups)
			{
				const TSharedPtr<FJsonObject> U = V->AsObject();
				if (!U) continue;
				FFTUpgradeDef Def;
				Def.Id = FName(*U->GetStringField(TEXT("id")));
				Def.Name = U->GetStringField(TEXT("name"));
				U->TryGetStringField(TEXT("branch"), Def.Branch);
				U->TryGetStringField(TEXT("desc"), Def.Desc);
				U->TryGetNumberField(TEXT("tier"), Def.Tier);
				FString Col; if (U->TryGetStringField(TEXT("color"), Col)) Def.Color = HexColor(Col);
				U->TryGetNumberField(TEXT("maxRank"), Def.MaxRank);
				Def.MaxRank = FMath::Max(1, Def.MaxRank);
				const TArray<TSharedPtr<FJsonValue>>* A;
				if (U->TryGetArrayField(TEXT("cost"), A)) for (const TSharedPtr<FJsonValue>& C : *A) Def.Cost.Add(FMath::Max(0, (int32)C->AsNumber()));
				if (U->TryGetArrayField(TEXT("requires"), A))
					for (const TSharedPtr<FJsonValue>& R : *A)
						if (const TSharedPtr<FJsonObject> RO = R->AsObject()) { FFTSkillReq Q; Q.Id = FName(*RO->GetStringField(TEXT("id"))); RO->TryGetNumberField(TEXT("rank"), Q.Rank); Def.Requires.Add(Q); }
				if (U->TryGetArrayField(TEXT("effects"), A))
					for (const TSharedPtr<FJsonValue>& E : *A)
						if (const TSharedPtr<FJsonObject> EO = E->AsObject()) { FFTSkillEffect F; F.Stat = FName(*EO->GetStringField(TEXT("stat"))); F.PerRank = (float)EO->GetNumberField(TEXT("perRank")); Def.Effects.Add(F); }
				D.Upgrades.Add(Def);
			}
		D.bLoaded = D.Upgrades.Num() > 0;
		if (!D.bLoaded) D.Error = TEXT("skills.json has no upgrades");
		UE_LOG(LogFT, Display, TEXT("FTSKILLS loaded %d upgrades, %d stats from %s"), D.Upgrades.Num(), D.Stats.Num(), *Path);
		return D;
	}
}

const FFTSkillData& FFTSkillData::Get()
{
	static const FFTSkillData Data = LoadSkills(); // plain data, no UObjects: safe as a static
	return Data;
}

const FFTUpgradeDef* FFTSkillData::Find(FName Id) const
{
	for (const FFTUpgradeDef& U : Upgrades) if (U.Id == Id) return &U;
	return nullptr;
}

FString FFTSkillData::EffectText(const FFTSkillEffect& E, int32 Rank) const
{
	const FFTStatDef* S = Stats.Find(E.Stat);
	const FString Label = S ? S->Label : E.Stat.ToString();
	const float V = E.PerRank * Rank;
	if (S && S->bAdd) return FString::Printf(TEXT("%s%d %s"), V >= 0.f ? TEXT("+") : TEXT(""), FMath::RoundToInt(V), *Label);
	return FString::Printf(TEXT("%s%d%% %s"), V >= 0.f ? TEXT("+") : TEXT(""), FMath::RoundToInt(V * 100.f), *Label);
}
