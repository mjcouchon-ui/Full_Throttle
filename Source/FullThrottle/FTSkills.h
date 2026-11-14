// Skill points and upgrades, read from Content/Data/skills.json (the web app's app/skills.js is the same data).
// The upgrades, their costs, ranks and effects, and how many points a level pays all live in that file; the
// code only knows the stat names it applies (steer, driftCharge, turboBurn, turboRegen, damageTaken, gunDamage,
// missiles). The coming skill tree adds entries with a branch, a tier and requirements; nothing here changes.
#pragma once

#include "CoreMinimal.h"

struct FFTSkillEffect { FName Stat; float PerRank = 0.f; };
struct FFTSkillReq { FName Id; int32 Rank = 1; };

struct FFTUpgradeDef
{
	FName Id;
	FString Name, Branch, Desc;
	int32 Tier = 0;
	FLinearColor Color = FLinearColor::White;
	int32 MaxRank = 1;
	TArray<int32> Cost;            // points for rank 1, 2, ...
	TArray<FFTSkillReq> Requires;  // other upgrades and the rank they need
	TArray<FFTSkillEffect> Effects;
	int32 CostOf(int32 Rank) const { return Cost.Num() ? Cost[FMath::Clamp(Rank - 1, 0, Cost.Num() - 1)] : 1; }
};

struct FFTStatDef { FString Label; bool bAdd = false; };

struct FFTSkillData
{
	bool bLoaded = false;
	FString Error;
	TMap<FName, int32> Points;           // wrap, escape, perRating, noRetakes
	TMap<FName, FString> PointLabels;
	TMap<FName, FFTStatDef> Stats;
	TArray<FFTUpgradeDef> Upgrades;

	static const FFTSkillData& Get();
	int32 Pts(const TCHAR* Key) const { const int32* P = Points.Find(Key); return P ? *P : 0; }
	FString PointLabel(const TCHAR* Key) const { const FString* L = PointLabels.Find(Key); return L ? *L : FString(Key).ToUpper(); }
	const FFTUpgradeDef* Find(FName Id) const;
	/** "+24% drift charge" or "+2 missiles" for an effect at a rank. */
	FString EffectText(const FFTSkillEffect& E, int32 Rank) const;
};
