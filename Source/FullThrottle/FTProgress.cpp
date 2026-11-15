#include "FTProgress.h"
#include "FTGameInstance.h"
#include "FTSaveIO.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Misc/AutomationTest.h"
#include "HAL/FileManager.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

namespace
{
	FLinearColor HexSRGB(const FString& Hex)
	{
		const FColor C = FColor::FromHex(Hex);
		return FLinearColor(C.R / 255.f, C.G / 255.f, C.B / 255.f, 1.f);
	}

	FFTFeatSpec ReadFeat(const TSharedPtr<FJsonObject>& O)
	{
		FFTFeatSpec F;
		if (!O.IsValid()) return F;
		F.Type = FName(*O->GetStringField(TEXT("t")));
		O->TryGetNumberField(TEXT("n"), F.N);
		O->TryGetNumberField(TEXT("k"), F.K);
		FString L; if (O->TryGetStringField(TEXT("line"), L)) F.Line = FName(*L);
		const TArray<TSharedPtr<FJsonValue>>* A;
		if (O->TryGetArrayField(TEXT("names"), A)) for (const auto& V : *A) F.Names.Add(V->AsString());
		O->TryGetStringField(TEXT("text"), F.Text);
		return F;
	}

	EFTKind KindFromName(const FString& S)
	{
		if (S == TEXT("Moto")) return EFTKind::Moto; if (S == TEXT("Hog")) return EFTKind::Hog; if (S == TEXT("Rammer")) return EFTKind::Rammer;
		if (S == TEXT("Suv")) return EFTKind::Suv; if (S == TEXT("Truck")) return EFTKind::Truck; if (S == TEXT("Heli")) return EFTKind::Heli;
		if (S == TEXT("Henchman")) return EFTKind::Henchman;
		return EFTKind::None;
	}

	// SFX DEPT items this build cannot show yet: they need a new Niagara emitter, which the connector cannot author (12 §11)
	const TMap<FName, FString>& NeedsEmitter()
	{
		static const TMap<FName, FString> M = {
			{ TEXT("s-afterburner-rings"), TEXT("NEEDS A NEW NIAGARA EMITTER") }, { TEXT("s-rocket-plume"), TEXT("NEEDS A NEW NIAGARA EMITTER") },
			{ TEXT("s-tail-light-streaks"), TEXT("NEEDS A NEW NIAGARA EMITTER") }, { TEXT("s-neon-ribbon"), TEXT("NEEDS A NEW NIAGARA EMITTER") },
			{ TEXT("s-laser-grid"), TEXT("NEEDS A NEW NIAGARA EMITTER") }, { TEXT("s-lightning-trail"), TEXT("NEEDS A NEW NIAGARA EMITTER") },
			{ TEXT("s-time-warp"), TEXT("NEEDS A NEW NIAGARA EMITTER") }, { TEXT("s-neon-skids"), TEXT("NEEDS SKID-MARK DECALS") },
			{ TEXT("s-fire-tracks"), TEXT("NEEDS SKID-MARK DECALS") } };
		return M;
	}

	FFTProgressData LoadData()
	{
		FFTProgressData D;
		const FString Path = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Data"), TEXT("skills.json"));
		FString Text;
		if (!FFileHelper::LoadFileToString(Text, *Path)) { D.Error = TEXT("skills.json not found"); UE_LOG(LogFT, Error, TEXT("FT LOAD data=skills.json result=missing path=%s"), *Path); return D; }
		TSharedPtr<FJsonObject> J;
		if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Text), J) || !J.IsValid()) { D.Error = TEXT("skills.json is not valid JSON"); return D; }
		if (J->GetStringField(TEXT("format")) != TEXT("full-throttle.skills/2")) { D.Error = TEXT("skills.json is not format 2"); UE_LOG(LogFT, Error, TEXT("FT LOAD data=skills.json result=wrong_format")); return D; }

		const TSharedPtr<FJsonObject>* O;
		if (J->TryGetObjectField(TEXT("tuning"), O))
			for (const auto& KV : (*O)->Values)
			{
				if (KV.Value->Type == EJson::Array) { if (KV.Key == TEXT("RatingMult")) for (const auto& V : KV.Value->AsArray()) D.RatingMult.Add((float)V->AsNumber()); }
				else D.Tuning.Add(FName(*KV.Key), (float)KV.Value->AsNumber());
			}
		while (D.RatingMult.Num() < 4) D.RatingMult.Add(1.f);
		if (J->TryGetObjectField(TEXT("extras"), O))
		{
			(*O)->TryGetNumberField(TEXT("greenlight"), D.GreenlightPrice); (*O)->TryGetNumberField(TEXT("film"), D.FilmPrice);
			(*O)->TryGetNumberField(TEXT("filmMax"), D.FilmMax); (*O)->TryGetNumberField(TEXT("sequel"), D.SequelPrice); (*O)->TryGetNumberField(TEXT("respec"), D.RespecPrice);
		}
		if (J->TryGetObjectField(TEXT("xp"), O))
		{
			D.Tuning.Add(TEXT("XPSceneFirstTake"), (float)(*O)->GetNumberField(TEXT("sceneFirstTake")));
			D.Tuning.Add(TEXT("XPFirstStunt"), (float)(*O)->GetNumberField(TEXT("firstStunt")));
			D.Tuning.Add(TEXT("XPNote"), (float)(*O)->GetNumberField(TEXT("note")));
			D.Tuning.Add(TEXT("XPBossFirst"), (float)(*O)->GetNumberField(TEXT("bossFirst")));
			const TArray<TSharedPtr<FJsonValue>>& Rt = (*O)->GetArrayField(TEXT("rating"));
			for (int32 i = 0; i < Rt.Num(); ++i) D.Tuning.Add(FName(*FString::Printf(TEXT("XPRating%d"), i)), (float)Rt[i]->AsNumber());
		}
		for (const auto& V : J->GetArrayField(TEXT("tiers")))
		{
			const auto R = V->AsObject(); FFTTierRow T;
			T.Tier = R->GetIntegerField(TEXT("tier")); T.SP = R->GetIntegerField(TEXT("sp")); T.Level = R->GetIntegerField(TEXT("level")); T.bGreenlight = R->GetBoolField(TEXT("greenlight"));
			D.Tiers.Add(T);
		}
		for (const auto& V : J->GetArrayField(TEXT("driverLevels")))
		{
			const auto R = V->AsObject(); FFTDriverLevelRow L;
			L.Level = R->GetIntegerField(TEXT("level")); L.TotalXP = (int64)R->GetNumberField(TEXT("totalXP")); L.XPToNext = (int64)R->GetNumberField(TEXT("xpToNext"));
			L.SP = R->GetIntegerField(TEXT("sp")); L.Unlock = R->GetStringField(TEXT("unlock"));
			D.DriverLevels.Add(L);
		}
		for (const auto& V : J->GetArrayField(TEXT("notes")))
		{
			const auto R = V->AsObject(); FFTNoteRow N;
			N.Id = R->GetStringField(TEXT("id")); N.Level = R->GetIntegerField(TEXT("level")); N.Text = R->GetStringField(TEXT("text"));
			D.Notes.Add(N);
		}
		for (const auto& V : J->GetArrayField(TEXT("branches")))
		{
			const auto R = V->AsObject(); FFTBranchRow B;
			B.Code = R->GetStringField(TEXT("code")); B.Name = R->GetStringField(TEXT("name")); B.About = R->GetStringField(TEXT("about"));
			D.Branches.Add(B);
		}
		for (const auto& V : J->GetArrayField(TEXT("skillNodes")))
		{
			const auto R = V->AsObject(); FFTSkillNodeRow N;
			N.Id = FName(*R->GetStringField(TEXT("id"))); N.Branch = R->GetStringField(TEXT("branch")); N.Name = R->GetStringField(TEXT("name"));
			N.Desc = R->GetStringField(TEXT("desc")); N.Tier = R->GetIntegerField(TEXT("tier")); N.SP = R->GetIntegerField(TEXT("sp"));
			N.Level = R->GetIntegerField(TEXT("level")); N.bGreenlight = R->GetBoolField(TEXT("greenlight"));
			D.Nodes.Add(N);
		}
		for (const auto& V : J->GetArrayField(TEXT("garage")))
		{
			const auto R = V->AsObject(); FFTGarageLine G;
			G.Key = FName(*R->GetStringField(TEXT("key"))); G.Name = R->GetStringField(TEXT("name")); G.Row = R->GetStringField(TEXT("row"));
			const TArray<TSharedPtr<FJsonValue>>& Rk = R->GetArrayField(TEXT("ranks"));
			for (int32 i = 0; i < 3 && i < Rk.Num(); ++i)
			{
				const auto K = Rk[i]->AsObject();
				G.Ranks[i].Name = K->GetStringField(TEXT("name")); G.Ranks[i].Price = K->GetIntegerField(TEXT("price")); G.Ranks[i].Level = K->GetIntegerField(TEXT("level"));
				const TSharedPtr<FJsonObject>* F; if (K->TryGetObjectField(TEXT("feat"), F)) G.Ranks[i].Feat = ReadFeat(*F);
			}
			const TSharedPtr<FJsonObject>* Vals;
			if (R->TryGetObjectField(TEXT("values"), Vals))
				for (const auto& KV : (*Vals)->Values) { TArray<float>& A = G.Values.Add(FName(*KV.Key)); for (const auto& X : KV.Value->AsArray()) A.Add((float)X->AsNumber()); }
			D.Garage.Add(G);
		}
		for (const auto& V : J->GetArrayField(TEXT("cosmetics")))
		{
			const auto R = V->AsObject(); FFTCosmeticRow C;
			C.Id = FName(*R->GetStringField(TEXT("id"))); C.Slot = FName(*R->GetStringField(TEXT("slot"))); C.Name = R->GetStringField(TEXT("name"));
			C.Shop = R->GetStringField(TEXT("shop")); C.Group = R->GetStringField(TEXT("group")); C.How = R->GetStringField(TEXT("how")); C.Look = R->GetStringField(TEXT("look"));
			C.Tier = R->GetIntegerField(TEXT("tier")); C.Price = R->GetIntegerField(TEXT("price")); C.Level = R->GetIntegerField(TEXT("level")); C.Wave = R->GetIntegerField(TEXT("wave"));
			const TSharedPtr<FJsonObject>* F; if (R->TryGetObjectField(TEXT("feat"), F)) C.Feat = ReadFeat(*F);
			FString Sw; if (R->TryGetStringField(TEXT("swatch"), Sw) && Sw.StartsWith(TEXT("#"))) { C.bSwatch = true; C.Swatch = HexSRGB(Sw); }
			// 2.6 ships wave 1 (07 §7): recolour paints, rim finishes and the code-made SFX DEPT effects
			C.bReady = C.Wave == 1;
			if (C.Wave == 2) C.NotReadyWhy = TEXT("WAVE 2: NEW WHEEL ART COMING");
			if (C.Wave == 3) C.NotReadyWhy = TEXT("WAVE 3: NEW BODY ART COMING");
			if (const FString* Why = NeedsEmitter().Find(C.Id)) { C.bReady = false; C.NotReadyWhy = *Why; }
			if (C.Tier == 0) C.bReady = true;
			D.Cosmetics.Add(C);
		}
		for (const auto& V : J->GetArrayField(TEXT("kinds")))
		{
			const auto R = V->AsObject(); FFTKindRow K;
			K.Name = R->GetStringField(TEXT("name")); K.Armor = R->GetIntegerField(TEXT("armor")); K.Points = R->GetIntegerField(TEXT("points")); K.XP = R->GetIntegerField(TEXT("xp"));
			const FString Cl = R->GetStringField(TEXT("class")); K.Class = Cl == TEXT("LIGHT") ? 0 : (Cl == TEXT("MEDIUM") ? 1 : (Cl == TEXT("HEAVY") ? 2 : -1));
			R->TryGetNumberField(TEXT("pace"), K.Pace); R->TryGetNumberField(TEXT("heightPx"), K.HeightPx); R->TryGetBoolField(TEXT("keepsUp"), K.bKeepsUp);
			D.Kinds.Add(KindFromName(R->GetStringField(TEXT("kind"))), K);
		}
		// levels and bosses: the spec's three (L1-L3), then the 2.5 levels by the closest stated rule (08 §1: par = road
		// to the boss at 470 su plus the fight allowance, rounded to 5 s; fee, kill fee and wrap XP as level 3)
		TArray<FFTLevelRow> Spec;
		for (const auto& V : J->GetArrayField(TEXT("levels")))
		{
			const auto R = V->AsObject(); FFTLevelRow L;
			L.Id = R->GetStringField(TEXT("id")); L.Title = R->GetStringField(TEXT("title")); L.Scenes = R->GetIntegerField(TEXT("scenes"));
			L.Vehicles = R->GetIntegerField(TEXT("vehicles")); L.Henchmen = R->GetIntegerField(TEXT("henchmen")); L.Par = R->GetIntegerField(TEXT("par"));
			L.Fee = R->GetIntegerField(TEXT("fee")); L.KillFee = R->GetIntegerField(TEXT("killFee")); L.WrapXP = R->GetIntegerField(TEXT("wrapXP"));
			const TArray<TSharedPtr<FJsonValue>>& B = R->GetArrayField(TEXT("bars")); for (int32 i = 0; i < 3 && i < B.Num(); ++i) L.Bars[i] = (int32)B[i]->AsNumber();
			for (const auto& N : R->GetArrayField(TEXT("notes"))) L.Notes.Add(N->AsString());
			L.bFromSpec = true;
			Spec.Add(L);
		}
		TArray<FFTBossRow> SpecB;
		for (const auto& V : J->GetArrayField(TEXT("bosses")))
		{
			const auto R = V->AsObject(); FFTBossRow B;
			B.Name = R->GetStringField(TEXT("name")); B.Armor = R->GetIntegerField(TEXT("armor")); B.Spot = R->GetNumberField(TEXT("spot"));
			B.Escape = R->GetNumberField(TEXT("escape")); B.Pace = R->GetNumberField(TEXT("pace")); B.Flee = R->GetNumberField(TEXT("flee")); B.bFromSpec = true;
			SpecB.Add(B);
		}
		for (int32 i = 0; i < FTData::NumLevels(); ++i)
		{
			const FFTLevelDef& Def = FTData::Level(i);
			FFTLevelRow L;
			if (Spec.IsValidIndex(i)) L = Spec[i];
			else
			{
				const FFTLevelRow& L3 = Spec.Last();
				L.Id = FString::Printf(TEXT("L%d"), i + 1); L.Title = Def.Title; L.Scenes = Def.Scenes.Num();
				const float Allow = Def.BossEscape >= 90.f ? 55.f : 40.f;
				L.Par = FMath::RoundToInt((Def.FinaleD / 470.f + Allow) / 5.f) * 5;
				L.Fee = L3.Fee; L.KillFee = L3.KillFee; L.WrapXP = L3.WrapXP;
				for (int32 b = 0; b < 3; ++b) L.Bars[b] = Def.Rating[b + 1];
			}
			// the denominator as the level script spawns it (the count assertion compares it with the spec's number)
			int32 N = 0;
			for (const FFTScene& Sc : Def.Scenes) for (const FFTSpawn& Sp : Sc.Spawns) for (EFTKind K : Sp.Kinds)
				if (K == EFTKind::Moto || K == EFTKind::Hog || K == EFTKind::Rammer || K == EFTKind::Suv || K == EFTKind::Truck || K == EFTKind::Heli || K == EFTKind::Boss) ++N;
			if (L.bFromSpec && N != L.Vehicles) { UE_LOG(LogFT, Error, TEXT("FT LEVEL id=%s scriptVehicles=%d expected=%d"), *L.Id, N, L.Vehicles); }
			if (!L.bFromSpec) L.Vehicles = N;
			D.Levels.Add(L);
			FFTBossRow B;
			if (SpecB.IsValidIndex(i)) B = SpecB[i];
			else
			{
				B.Name = Def.BossName; B.Armor = FMath::RoundToInt(Def.BossHp); B.Escape = Def.BossEscape;
				const bool bLong = Def.BossEscape >= 90.f;
				B.Pace = bLong ? SpecB[2].Pace : SpecB[1].Pace; B.Flee = bLong ? SpecB[2].Flee : SpecB[1].Flee; B.Spot = 128.f;
			}
			D.Bosses.Add(B);
		}
		D.bLoaded = D.DriverLevels.Num() == 30 && D.Nodes.Num() == 30 && D.Garage.Num() == 10 && D.Cosmetics.Num() > 0;
		if (!D.bLoaded) D.Error = TEXT("skills.json is incomplete");
		int64 GSum = 0; for (const FFTGarageLine& G : D.Garage) for (const FFTRankRow& R : G.Ranks) GSum += R.Price;
		int64 CSum = 0; int32 CFor = 0; for (const FFTCosmeticRow& C : D.Cosmetics) { CSum += C.Price; CFor += C.Price > 0; }
		int32 SPSum = 0; for (const FFTSkillNodeRow& N : D.Nodes) SPSum += N.SP;
		UE_LOG(LogFT, Display, TEXT("FT LOAD data=skills.json result=%s levels=%d top=%lld nodes=%d sp=%d garage=%lld cosmetics=%d forSale=%d catalog=%lld"),
			D.bLoaded ? TEXT("ok") : TEXT("error"), D.DriverLevels.Num(), D.DriverLevels.Num() ? D.DriverLevels.Last().TotalXP : 0, D.Nodes.Num(), SPSum, GSum, D.Cosmetics.Num(), CFor, CSum);
		return D;
	}
}

const FFTProgressData& FFTProgressData::Get()
{
	static const FFTProgressData Data = LoadData(); // plain data, no UObjects
	return Data;
}

float FFTGarageLine::Value(FName Field, int32 Rank, float Def) const
{
	const TArray<float>* A = Values.Find(Field);
	return (A && A->Num()) ? (*A)[FMath::Clamp(Rank, 0, A->Num() - 1)] : Def;
}
const FFTSkillNodeRow* FFTProgressData::Node(FName Id) const { for (const FFTSkillNodeRow& N : Nodes) if (N.Id == Id) return &N; return nullptr; }
const FFTGarageLine* FFTProgressData::Line(FName Key) const { for (const FFTGarageLine& G : Garage) if (G.Key == Key) return &G; return nullptr; }
const FFTCosmeticRow* FFTProgressData::Cosmetic(FName Id) const { for (const FFTCosmeticRow& C : Cosmetics) if (C.Id == Id) return &C; return nullptr; }
int32 FFTProgressData::LevelForXP(int64 XP) const
{
	int32 L = 1;
	for (const FFTDriverLevelRow& R : DriverLevels) if (R.TotalXP <= XP) L = R.Level;
	return L;
}
int64 FFTProgressData::TotalXPFor(int32 Level) const
{
	for (const FFTDriverLevelRow& R : DriverLevels) if (R.Level == Level) return R.TotalXP;
	return DriverLevels.Num() ? DriverLevels.Last().TotalXP : 0;
}
int32 FFTProgressData::SPFor(int32 Level) const { for (const FFTDriverLevelRow& R : DriverLevels) if (R.Level == Level) return R.SP; return 0; }

FString FFTProgressData::StuntType(const FString& Name)
{
	FString S = Name;
	S.RemoveFromEnd(TEXT(" x2"));
	return S;
}

// ================================================================================================ paycheck
namespace
{
	double Round10(double V) { return FMath::RoundToDouble(V / 10.0) * 10.0; }
}

FFTPaycheck FTPay::Compute(const FFTLevelResult& R, const FFTCareerSnapshot& C, const FFTProgressData& D)
{
	FFTPaycheck P;
	const FFTLevelRow& L = D.Levels.IsValidIndex(R.Level) ? D.Levels[R.Level] : D.Levels[0];
	auto Line = [&P](const TCHAR* Key, const FString& Label, double Amount, bool bMult = false, double Mult = 0, const FString& Note = FString())
	{
		FFTPayLine& Ln = P.Lines.AddDefaulted_GetRef();
		Ln.Key = Key; Ln.Label = Label; Ln.Amount = Amount; Ln.bMult = bMult; Ln.Mult = Mult; Ln.Note = Note;
	};
	const double SeqPay = C.bSequel ? D.T(TEXT("SequelPay"), 1.5f) : 1.0;
	if (R.Outcome == EFTOutcome::OutOfFilm)
	{
		Line(TEXT("Fee"), TEXT("KILL FEE (OUT OF FILM)"), L.KillFee);
		double Sum = L.KillFee;
		if (C.bSequel) { Sum *= SeqPay; Line(TEXT("SEQUEL"), TEXT("SEQUEL"), Sum, true, SeqPay); }
		P.Total = (int64)Round10(Sum);
		return P;
	}
	const bool bWrap = R.Outcome == EFTOutcome::Wrap;
	const double Fee = L.Fee;
	Line(TEXT("Fee"), TEXT("FEE"), Fee);
	const double Dest = (R.D - 0.5 * R.BossAddPoints) / 10.0;
	const double Stn = (R.S + R.N) / 20.0;
	Line(TEXT("Destruction"), TEXT("DESTRUCTION"), Dest, false, 0, R.BossAddPoints > 0 ? TEXT("BOSS ADDS AT HALF") : TEXT(""));
	Line(TEXT("Stunts"), TEXT("STUNTS & NEAR MISSES"), Stn);
	double Run = Dest + Stn;

	P.Exact = R.ScriptTotal > 0 ? (double)R.Wrecked / R.ScriptTotal : 0.0;
	P.Shown = (int32)FMath::RoundToInt(P.Exact * 100.0);
	const double W = P.Shown / 100.0;
	const double F = C.bScorched ? D.T(TEXT("CarnageFactorScorched"), 2.5f) : D.T(TEXT("CarnageFactor"), 2.f);
	const double Cap = C.bScorched ? D.T(TEXT("CarnageCapScorched"), 3.5f) : D.T(TEXT("CarnageCap"), 3.f);
	P.Carnage = FMath::Min(Cap, 1.0 + F * W * W);
	Run *= P.Carnage;
	Line(TEXT("Carnage"), FString::Printf(TEXT("CARNAGE %d%% (%d / %d)"), P.Shown, R.Wrecked, R.ScriptTotal), Run, true, P.Carnage);

	const double Step = C.bCrowd ? D.T(TEXT("StyleStepCrowd"), 0.06f) : D.T(TEXT("StyleStep"), 0.05f);
	const double SCap = C.bCrowd ? D.T(TEXT("StyleCapCrowd"), 1.9f) : D.T(TEXT("StyleCap"), 1.75f);
	P.Style = FMath::Min(SCap, 1.0 + Step * R.StuntTypes);
	Run *= P.Style;
	Line(TEXT("Style"), FString::Printf(TEXT("STYLE (%d STUNT TYPES)"), R.StuntTypes), Run, true, P.Style);

	const double RM = D.RatingMult.IsValidIndex(R.Rating) ? D.RatingMult[R.Rating] : 1.0;
	P.Skill = RM * FMath::Max(0.0, 1.0 - D.T(TEXT("RetakePenalty"), 0.1f) * R.Retakes) * (bWrap ? 1.0 : D.T(TEXT("GotAwaySkill"), 0.75f));
	if (C.bBigFinish) P.Skill += FMath::Min((double)D.T(TEXT("BigFinishMax"), 0.5f), D.T(TEXT("BigFinishStep"), 0.05f) * (double)R.MultiKills);
	Run *= P.Skill;
	Line(TEXT("Skill"), FString::Printf(TEXT("SKILL (RATING, %d RETAKE%s%s)"), R.Retakes, R.Retakes == 1 ? TEXT("") : TEXT("S"), bWrap ? TEXT("") : TEXT(", HE GOT AWAY")), Run, true, P.Skill);

	double Sum = Fee + Run;
	const double Armor = D.T(TEXT("ArmorPayPerPct"), 10.f) * R.ArmorPct;
	Line(TEXT("armor"), FString::Printf(TEXT("ARMOR LEFT %d%%"), R.ArmorPct), Armor); Sum += Armor;
	const double Civ = -D.T(TEXT("CivilianPenalty"), 500.f) * R.Civilians;
	Line(TEXT("civilians"), FString::Printf(TEXT("CIVILIANS HIT (%d)"), R.Civilians), Civ); Sum += Civ;
	if (bWrap && P.Exact >= D.T(TEXT("TotalCarnageShare"), 0.95f))
	{
		P.bTotalCarnage = true;
		const double TC = D.T(TEXT("TotalCarnageBonus"), 25000.f);
		Line(TEXT("TOTAL CARNAGE"), TEXT("TOTAL CARNAGE"), TC); Sum += TC;
	}
	if (bWrap && R.LevelTime > 0.0 && R.LevelTime < L.Par)
	{
		const double Rate = C.bAhead ? D.T(TEXT("ScheduleRateAhead"), 300.f) : D.T(TEXT("ScheduleRate"), 200.f);
		const double SCp = C.bAhead ? D.T(TEXT("ScheduleCapAhead"), 30000.f) : D.T(TEXT("ScheduleCap"), 20000.f);
		const double Sch = FMath::Min(SCp, Rate * (L.Par - R.LevelTime) * P.Skill);
		const int32 Tm = FMath::FloorToInt(R.LevelTime);
		Line(TEXT("SCHEDULE"), FString::Printf(TEXT("SCHEDULE %d:%02d (PAR %d:%02d)"), Tm / 60, Tm % 60, L.Par / 60, L.Par % 60), Sch); Sum += Sch;
	}
	if (R.NotesCompleted > 0) { const double N = D.T(TEXT("NoteCash"), 2000.f) * R.NotesCompleted; Line(TEXT("Notes"), FString::Printf(TEXT("DIRECTOR'S NOTES (%d)"), R.NotesCompleted), N); Sum += N; }
	if (C.bBigFinish && bWrap && R.bBossWrecked) { const double B = D.T(TEXT("BigFinishCash"), 10000.f); Line(TEXT("BIG FINISH"), TEXT("BIG FINISH: BOSS WRECKED"), B); Sum += B; }
	if (C.bScorched && R.Barrels > 0) { const double B = D.T(TEXT("BarrelCash"), 100.f) * R.Barrels; Line(TEXT("Barrels"), FString::Printf(TEXT("SCORCHED EARTH: %d BARRELS"), R.Barrels), B); Sum += B; }
	Sum = FMath::Max(Sum, Fee);   // the Fee is the floor on a wrap or a getaway
	if (C.bSequel) { Sum *= SeqPay; Line(TEXT("SEQUEL"), TEXT("SEQUEL"), Sum, true, SeqPay); }
	P.Total = (int64)Round10(Sum);
	return P;
}

FFTLevelResult FTPay::Example(TCHAR Which, FFTCareerSnapshot& OutC)
{
	// 03 §7: A regular stock build, B speed build (Ahead of Schedule owned), C Brad's best take
	FFTLevelResult R; OutC = FFTCareerSnapshot();
	R.Level = 0; R.Outcome = EFTOutcome::Wrap; R.ScriptTotal = 114; R.bBossWrecked = true;
	if (Which == 'A') { R.D = 61800; R.BossAddPoints = 900; R.S = 89500; R.Wrecked = 80; R.StuntTypes = 10; R.Rating = 1; R.Retakes = 1; R.ArmorPct = 50; R.Civilians = 1; R.LevelTime = 315; }
	else if (Which == 'B') { R.D = 53500; R.BossAddPoints = 900; R.S = 89800; R.Wrecked = 68; R.StuntTypes = 10; R.Rating = 1; R.Retakes = 1; R.ArmorPct = 50; R.Civilians = 1; R.LevelTime = 260.3; OutC.bAhead = true; }
	else { R.D = 100000; R.S = 101140; R.Wrecked = 97; R.StuntTypes = 12; R.Rating = 2; R.Retakes = 0; R.ArmorPct = 60; R.Civilians = 0; R.LevelTime = 0; }
	return R;
}

#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFTPaycheckExamplesTest, "FT.Paycheck.Examples", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)
bool FFTPaycheckExamplesTest::RunTest(const FString& Parameters)
{
	const FFTProgressData& D = FFTProgressData::Get();
	if (!D.bLoaded) { AddError(TEXT("skills.json not loaded")); return false; }
	const TCHAR W[3] = { 'A', 'B', 'C' };
	const int64 Want[3] = { 40450, 46830, 93950 };
	for (int32 i = 0; i < 3; ++i)
	{
		FFTCareerSnapshot C; const FFTLevelResult R = FTPay::Example(W[i], C);
		const FFTPaycheck P = FTPay::Compute(R, C, D);
		TestEqual(FString::Printf(TEXT("example %c"), W[i]), P.Total, Want[i]);
	}
	return true;
}
#endif

// ================================================================================================ career
void UFTCareerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Fresh();   // UFTGameInstance::Init loads the slot once the settings (best takes) are read
}

UFTCareerSubsystem* UFTCareerSubsystem::Get(const UObject* Ctx)
{
	const UWorld* W = Ctx ? Ctx->GetWorld() : nullptr;
	const UGameInstance* GI = W ? W->GetGameInstance() : nullptr;
	return GI ? GI->GetSubsystem<UFTCareerSubsystem>() : nullptr;
}

FString UFTCareerSubsystem::FilePath() const
{
#if WITH_DEV_AUTOMATION_TESTS
	if (!TestSavePath.IsEmpty()) return TestSavePath;
#endif
	const UFTGameInstance* GI = Cast<UFTGameInstance>(GetGameInstance());
	const FString Dir = GI ? GI->SaveDir() : FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("FullThrottle"));
	return FPaths::Combine(Dir, Slot + TEXT(".json"));
}

void UFTCareerSubsystem::UseTestSlot()
{
	Slot = TEXT("career_pie");
	UE_LOG(LogFT, Display, TEXT("FT SAVE slot=%s reason=UseTestSlot"), *Slot);
	Load();
}

void UFTCareerSubsystem::SizeArrays()
{
	const int32 N = FTData::NumLevels();
	C.Legendary.SetNum(N); C.Bosses.SetNum(N);
	C.Stats.BestCarnage.SetNum(N); C.Stats.BestTime.SetNum(N); C.Stats.Wrapped.SetNum(N); C.Stats.RunDown.SetNum(N);
	C.Stats.BestRating.SetNum(N); C.Stats.BossWrecks.SetNum(N);
}

void UFTCareerSubsystem::Fresh()
{
	C = FFTCareer();
	SizeArrays();
	C.Own = { TEXT("p-kestrel-red"), TEXT("r-gold-five-hole"), TEXT("s-kestrel-pink"), TEXT("s-hot-pink-nitro") };
	C.Wear.Add(TEXT("paint"), TEXT("p-kestrel-red")); C.Wear.Add(TEXT("rims"), TEXT("r-gold-five-hole"));
	C.Wear.Add(TEXT("neon"), TEXT("s-kestrel-pink")); C.Wear.Add(TEXT("nitro"), TEXT("s-hot-pink-nitro"));
	for (const TCHAR* S : { TEXT("exhaust"), TEXT("trail"), TEXT("smoke"), TEXT("wreck"), TEXT("finisher") }) C.Wear.Add(S, NAME_None);
	for (const FFTGarageLine& G : FFTProgressData::Get().Garage) C.Garage.Add(G.Key, 0);
}

bool UFTCareerSubsystem::Reset()
{
	const FFTCareer Before = C;
	const bool WasLoadError = bLoadError, WasPreserveBackup = bPreserveBackup;
	const FString PreviousLoadError = LoadError;
	Fresh();
	bPreserveBackup = bPreserveBackup || bLoadError;
	bLoadError = false; LoadError.Reset();
	// A fresh campaign starts at the first day's supported story checkpoint.
	C.ResumeKind = TEXT("day"); C.ResumeLevel = 0;
	UE_LOG(LogFT, Display, TEXT("FT CAREER reset slot=%s"), *Slot);
	if (!Save(TEXT("Reset")))
	{
		C = Before; bLoadError = WasLoadError; LoadError = PreviousLoadError; bPreserveBackup = WasPreserveBackup;
		return false;
	}
	LogCareer();
	bRecoveredFromBackup = false;
	return true;
}

void UFTCareerSubsystem::Drop()
{
	// the backup and any half-written file go with it: a dropped career must not come back from its .bak
	for (const TCHAR* Ext : { TEXT(""), TEXT(".bak"), TEXT(".tmp") }) IFileManager::Get().Delete(*(FilePath() + Ext));
	bHasPersistedCareer = false; LastSavedText.Reset();
	UE_LOG(LogFT, Display, TEXT("FT SAVE slot=%s reason=Drop"), *Slot);
}

namespace
{
	TArray<TSharedPtr<FJsonValue>> JArr(const TArray<FName>& A) { TArray<TSharedPtr<FJsonValue>> O; for (const FName& N : A) O.Add(MakeShared<FJsonValueString>(N.ToString())); return O; }
	TArray<TSharedPtr<FJsonValue>> JArr(const TArray<FString>& A) { TArray<TSharedPtr<FJsonValue>> O; for (const FString& N : A) O.Add(MakeShared<FJsonValueString>(N)); return O; }
	TArray<TSharedPtr<FJsonValue>> JArr(const TArray<bool>& A) { TArray<TSharedPtr<FJsonValue>> O; for (bool B : A) O.Add(MakeShared<FJsonValueBoolean>(B)); return O; }
	TArray<TSharedPtr<FJsonValue>> JArr(const TArray<int32>& A) { TArray<TSharedPtr<FJsonValue>> O; for (int32 V : A) O.Add(MakeShared<FJsonValueNumber>(V)); return O; }
	TArray<TSharedPtr<FJsonValue>> JArr(const TArray<float>& A) { TArray<TSharedPtr<FJsonValue>> O; for (float V : A) O.Add(MakeShared<FJsonValueNumber>(FMath::RoundToDouble(V * 10000.0) / 10000.0)); return O; }
	template <typename T, typename F> void RArr(const TSharedPtr<FJsonObject>& J, const TCHAR* K, TArray<T>& Out, F Conv)
	{
		const TArray<TSharedPtr<FJsonValue>>* A;
		if (!J->TryGetArrayField(K, A)) return;
		for (int32 i = 0; i < A->Num() && i < Out.Num(); ++i) Out[i] = Conv((*A)[i]);
	}

	bool ValidCheckpoint(const FString& Kind, int32 Level, int32 Stage)
	{
		if (Kind == TEXT("none")) return Level == 0 && Stage == 0;
		if (Level < 0 || Level >= FTData::NumLevels()) return false;
		if (Kind == TEXT("night")) return Level < 7 && Stage >= -1 && Stage <= 3;   // three waves, then the boss
		if (Kind == TEXT("nightoutro")) return Level < 7 && (Stage == 2 || (Level == 0 && Stage == 3));
		return Stage == 0 && (Kind == TEXT("race") || Kind == TEXT("day") || Kind == TEXT("preproduction") || Kind == TEXT("complete"));
	}

	bool ValidateCareer(const TSharedPtr<FJsonObject>& J, FString& Error)
	{
		if (!J.IsValid()) { Error = TEXT("The career save is not valid JSON."); return false; }
		double Version = 0;
		if (!J->TryGetNumberField(TEXT("v"), Version) || (Version != 1 && Version != 2))
		{ Error = FString::Printf(TEXT("Unsupported career save version %.0f. The file has been preserved."), Version); return false; }
		const TSharedPtr<FJsonObject>* Driver = nullptr;
		double Value = 0;
		if (!J->TryGetObjectField(TEXT("driver"), Driver) || !J->HasTypedField<EJson::Object>(TEXT("stats"))
			|| !J->TryGetNumberField(TEXT("cash"), Value) || !FMath::IsFinite(Value)
			|| !J->TryGetNumberField(TEXT("cashEarned"), Value) || !FMath::IsFinite(Value))
		{ Error = TEXT("The career save is missing required progress data."); return false; }
		for (const TCHAR* Field : { TEXT("level"), TEXT("xp"), TEXT("sp"), TEXT("spSpent") })
			if (!(*Driver)->TryGetNumberField(Field, Value) || !FMath::IsFinite(Value) || Value < 0 || Value > (FCString::Strcmp(Field, TEXT("xp")) == 0 ? 9.0e15 : 2.0e9))
			{ Error = TEXT("The career save contains invalid driver progress."); return false; }
		// Reject wrong container/element types before the older field readers run.
		auto Container = [&](const TCHAR* Field, EJson Type, EJson Element = EJson::None)
		{
			const TSharedPtr<FJsonValue> V = J->TryGetField(Field);
			if (!V.IsValid()) return true;
			if (V->Type != Type) return false;
			if (Element != EJson::None && Type == EJson::Array)
				for (const auto& X : V->AsArray()) if (!X.IsValid() || X->Type != Element) return false;
			if (Element != EJson::None && Type == EJson::Object)
				for (const auto& X : V->AsObject()->Values) if (!X.Value.IsValid() || X.Value->Type != Element) return false;
			return true;
		};
		if (!Container(TEXT("garage"), EJson::Object, EJson::Number) || !Container(TEXT("notes"), EJson::Object, EJson::Boolean)
			|| !Container(TEXT("greenlit"), EJson::Array, EJson::String) || !Container(TEXT("feats"), EJson::Array, EJson::String)
			|| !Container(TEXT("legendary"), EJson::Array, EJson::Boolean) || !Container(TEXT("bosses"), EJson::Array, EJson::Boolean)
			|| !Container(TEXT("pending"), EJson::Object) || !Container(TEXT("extras"), EJson::Object) || !Container(TEXT("custom"), EJson::Object))
		{ Error = TEXT("The career save contains invalid progress fields."); return false; }
		if (J->HasField(TEXT("skills")) && !Container(TEXT("skills"), EJson::Object, EJson::Boolean) && !Container(TEXT("skills"), EJson::Array, EJson::String))
		{ Error = TEXT("The career save contains invalid skills."); return false; }
		auto Typed = [](const TSharedPtr<FJsonObject>& Object, const TCHAR* Field, EJson Type, EJson Element = EJson::None, bool bAllowNull = false)
		{
			const auto V = Object->TryGetField(Field);
			if (!V.IsValid()) return true;
			if (V->IsNull()) return bAllowNull;
			if (V->Type != Type) return false;
			if (Type == EJson::Number) return FMath::IsFinite(V->AsNumber()) && V->AsNumber() >= 0 && V->AsNumber() <= 2.0e9;
			if (Type == EJson::Array)
				for (const auto& X : V->AsArray())
					if (!X.IsValid() || X->Type != Element || (Element == EJson::Number && (!FMath::IsFinite(X->AsNumber()) || X->AsNumber() < 0 || X->AsNumber() > 2.0e9))) return false;
			return true;
		};
		bool FieldsValid = Typed(J, TEXT("respecs"), EJson::Number) && Typed(J, TEXT("tipSeen"), EJson::Boolean) && Typed(J, TEXT("updated"), EJson::String);
		const TSharedPtr<FJsonObject>* Part = nullptr;
		if (J->TryGetObjectField(TEXT("pending"), Part)) FieldsValid &= Typed(*Part, TEXT("skills"), EJson::Array, EJson::String) && Typed(*Part, TEXT("level"), EJson::Number, EJson::None, true) && Typed(*Part, TEXT("fromTitle"), EJson::Boolean);
		if (J->TryGetObjectField(TEXT("extras"), Part)) FieldsValid &= Typed(*Part, TEXT("film"), EJson::Number) && Typed(*Part, TEXT("sequel"), EJson::Boolean) && Typed(*Part, TEXT("sequelOn"), EJson::Boolean);
		if (J->TryGetObjectField(TEXT("garage"), Part)) for (const auto& X : (*Part)->Values) FieldsValid &= FMath::IsFinite(X.Value->AsNumber()) && X.Value->AsNumber() >= 0 && X.Value->AsNumber() <= 2.0e9;
		if (J->TryGetObjectField(TEXT("custom"), Part))
		{
			FieldsValid &= Typed(*Part, TEXT("own"), EJson::Array, EJson::String);
			for (const TCHAR* Key : { TEXT("paint"), TEXT("rims"), TEXT("neon"), TEXT("nitro"), TEXT("exhaust"), TEXT("trail"), TEXT("smoke"), TEXT("wreck"), TEXT("finisher") }) FieldsValid &= Typed(*Part, Key, EJson::String, EJson::None, true);
		}
		if (J->TryGetObjectField(TEXT("stats"), Part))
		{
			for (const TCHAR* Key : { TEXT("runs"), TEXT("wraps"), TEXT("wrecks"), TEXT("leftBehind"), TEXT("civilians"), TEXT("trucks"), TEXT("snakeEyes") }) FieldsValid &= Typed(*Part, Key, EJson::Number);
			for (const TCHAR* Key : { TEXT("bestCarnage"), TEXT("bestTime"), TEXT("bestRating"), TEXT("bossWrecks") }) FieldsValid &= Typed(*Part, Key, EJson::Array, EJson::Number);
			for (const TCHAR* Key : { TEXT("wrapped"), TEXT("runDown") }) FieldsValid &= Typed(*Part, Key, EJson::Array, EJson::Boolean);
			FieldsValid &= Typed(*Part, TEXT("cleanWrap"), EJson::Boolean) && Typed(*Part, TEXT("armorWrap"), EJson::Boolean);
		}
		if (!FieldsValid) { Error = TEXT("The career save contains damaged progress values."); return false; }
		if (Version == 2)
		{
			const TSharedPtr<FJsonObject>* Resume = nullptr;
			FString Kind, RunId, CommitId; double Level = -1, Stage = -99; bool Wrap = false;
			if (!J->TryGetObjectField(TEXT("resume"), Resume) || !(*Resume)->TryGetStringField(TEXT("kind"), Kind)
				|| !(*Resume)->TryGetNumberField(TEXT("level"), Level) || !(*Resume)->TryGetNumberField(TEXT("stage"), Stage)
				|| !FMath::IsFinite(Level) || !FMath::IsFinite(Stage) || Level < 0 || Level >= FTData::NumLevels() || Stage < -1 || Stage > 99
				|| Level != FMath::FloorToDouble(Level) || Stage != FMath::FloorToDouble(Stage)
				|| !(*Resume)->TryGetBoolField(TEXT("wrap"), Wrap) || !ValidCheckpoint(Kind, int32(Level), int32(Stage))
				|| !J->TryGetStringField(TEXT("runId"), RunId) || !J->TryGetStringField(TEXT("lastCommitId"), CommitId)
				|| ((Kind == TEXT("race") || !CommitId.IsEmpty()) && RunId.IsEmpty()))
			{ Error = TEXT("The career checkpoint is invalid or unsupported."); return false; }
		}
		return true;
	}

	bool ReadCareer(const FString& Path, TSharedPtr<FJsonObject>& J, FString& Text, FString& Error)
	{
		Text.Reset(); J.Reset();
		if (!FFileHelper::LoadFileToString(Text, *Path)) { Error = TEXT("The career save could not be read."); return false; }
		if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Text), J)) { Error = TEXT("The career save is damaged or incomplete."); return false; }
		return ValidateCareer(J, Error);
	}
}

bool UFTCareerSubsystem::Save(const TCHAR* Reason)
{
	if (bLoadError) { bSaveError = true; SaveError = LoadError; UE_LOG(LogFT, Warning, TEXT("FT SAVE slot=%s reason=%s result=refused (%s)"), *Slot, Reason, *LoadError); return false; }
	if (const UFTGameInstance* GI = Cast<UFTGameInstance>(GetGameInstance()); GI && GI->bBench) return true;
	if (!bHasPersistedCareer && C.ResumeKind == TEXT("none")) { bSaveError = false; SaveError.Reset(); return true; }   // first-launch exit has no campaign to save
	if (bSaving) { bSaveError = true; SaveError = TEXT("A save is already in progress. Please retry."); return false; }
	bSaving = true;
	C.V = 2;
	TSharedRef<FJsonObject> J = MakeShared<FJsonObject>();
	J->SetNumberField(TEXT("v"), C.V);
	TSharedRef<FJsonObject> Dr = MakeShared<FJsonObject>();
	Dr->SetNumberField(TEXT("level"), C.Level); Dr->SetNumberField(TEXT("xp"), (double)C.XP); Dr->SetNumberField(TEXT("sp"), C.SP); Dr->SetNumberField(TEXT("spSpent"), C.SPSpent);
	J->SetObjectField(TEXT("driver"), Dr);
	J->SetNumberField(TEXT("cash"), (double)C.Cash); J->SetNumberField(TEXT("cashEarned"), (double)C.CashEarned);
	TSharedRef<FJsonObject> Sk = MakeShared<FJsonObject>(); for (const FName& S : C.Skills) Sk->SetBoolField(S.ToString(), true);
	J->SetObjectField(TEXT("skills"), Sk);
	TSharedRef<FJsonObject> Pe = MakeShared<FJsonObject>();
	Pe->SetArrayField(TEXT("skills"), JArr(C.PendingSkills));
	if (C.PendingLevel == -1) Pe->SetField(TEXT("level"), MakeShared<FJsonValueNull>()); else Pe->SetNumberField(TEXT("level"), C.PendingLevel >= 0 ? C.PendingLevel + 1 : 0);
	Pe->SetBoolField(TEXT("fromTitle"), C.bPendingFromTitle);
	J->SetObjectField(TEXT("pending"), Pe);
	TSharedRef<FJsonObject> Ga = MakeShared<FJsonObject>(); for (const auto& KV : C.Garage) Ga->SetNumberField(KV.Key.ToString().ToLower(), KV.Value);
	J->SetObjectField(TEXT("garage"), Ga);
	J->SetArrayField(TEXT("greenlit"), JArr(C.Greenlit));
	TSharedRef<FJsonObject> Ex = MakeShared<FJsonObject>(); Ex->SetNumberField(TEXT("film"), C.Film); Ex->SetBoolField(TEXT("sequel"), C.bSequel); Ex->SetBoolField(TEXT("sequelOn"), C.bSequelOn);
	J->SetObjectField(TEXT("extras"), Ex);
	TSharedRef<FJsonObject> Cu = MakeShared<FJsonObject>(); Cu->SetArrayField(TEXT("own"), JArr(C.Own));
	for (const auto& KV : C.Wear) { if (KV.Value.IsNone()) Cu->SetField(KV.Key.ToString().ToLower(), MakeShared<FJsonValueNull>()); else Cu->SetStringField(KV.Key.ToString().ToLower(), KV.Value.ToString()); }
	J->SetObjectField(TEXT("custom"), Cu);
	J->SetNumberField(TEXT("respecs"), C.Respecs);
	J->SetArrayField(TEXT("feats"), JArr(C.Feats));
	TSharedRef<FJsonObject> No = MakeShared<FJsonObject>(); for (const auto& KV : C.Notes) No->SetBoolField(KV.Key, KV.Value);
	J->SetObjectField(TEXT("notes"), No);
	J->SetArrayField(TEXT("legendary"), JArr(C.Legendary)); J->SetArrayField(TEXT("bosses"), JArr(C.Bosses));
	TSharedRef<FJsonObject> St = MakeShared<FJsonObject>();
	const FFTCareerStats& S = C.Stats;
	St->SetNumberField(TEXT("runs"), S.Runs); St->SetNumberField(TEXT("wraps"), S.Wraps); St->SetNumberField(TEXT("wrecks"), S.Wrecks);
	St->SetNumberField(TEXT("leftBehind"), S.LeftBehind); St->SetNumberField(TEXT("civilians"), S.Civilians);
	St->SetArrayField(TEXT("bestCarnage"), JArr(S.BestCarnage)); St->SetArrayField(TEXT("bestTime"), JArr(S.BestTime));
	St->SetArrayField(TEXT("wrapped"), JArr(S.Wrapped)); St->SetArrayField(TEXT("bestRating"), JArr(S.BestRating));
	St->SetNumberField(TEXT("trucks"), S.Trucks); St->SetNumberField(TEXT("snakeEyes"), S.SnakeEyes);
	St->SetArrayField(TEXT("bossWrecks"), JArr(S.BossWrecks)); St->SetArrayField(TEXT("runDown"), JArr(S.RunDown));
	St->SetBoolField(TEXT("cleanWrap"), S.bCleanWrap); St->SetBoolField(TEXT("armorWrap"), S.bArmorWrap);
	J->SetObjectField(TEXT("stats"), St);
	J->SetBoolField(TEXT("tipSeen"), C.bTipSeen);
	J->SetStringField(TEXT("updated"), C.Updated);
	TSharedRef<FJsonObject> Resume = MakeShared<FJsonObject>();
	Resume->SetStringField(TEXT("kind"), C.ResumeKind); Resume->SetNumberField(TEXT("level"), C.ResumeLevel);
	Resume->SetNumberField(TEXT("stage"), C.ResumeStage); Resume->SetBoolField(TEXT("wrap"), C.bResumeWrap);
	J->SetObjectField(TEXT("resume"), Resume); J->SetStringField(TEXT("runId"), C.RunId); J->SetStringField(TEXT("lastCommitId"), C.LastCommitId);
	FString Out;
	FJsonSerializer::Serialize(J, TJsonWriterFactory<>::Create(&Out));
	bool bFaultInjected = false;
#if !UE_BUILD_SHIPPING
	if (const IConsoleVariable* Fault = IConsoleManager::Get().FindConsoleVariable(TEXT("ft.SaveFail"))) bFaultInjected = Fault->GetInt() != 0;
#endif
	if (!bFaultInjected && !bPreserveBackup && bHasPersistedCareer && Out == LastSavedText)
	{ bSaving = false; bSaveError = false; SaveError.Reset(); return true; }
	const FString Updated = FDateTime::UtcNow().ToIso8601(); J->SetStringField(TEXT("updated"), Updated);
	Out.Reset(); FJsonSerializer::Serialize(J, TJsonWriterFactory<>::Create(&Out));
	bool bOk = ValidateCareer(J, SaveError) && FTSaveIO::Write(FilePath(), Out, SaveError, bPreserveBackup);
	bSaving = false; bSaveError = !bOk;
	if (bOk)
	{
		C.Updated = Updated; PersistedCareer = C; bHasPersistedCareer = true; LastSavedText = Out;
		LastSaveTime = FPlatformTime::Seconds(); bPreserveBackup = false; SaveError.Reset();
	}
	UE_LOG(LogFT, Display, TEXT("FT SAVE slot=%s reason=%s result=%s"), *Slot, Reason, bOk ? TEXT("ok") : TEXT("failed"));
	return bOk;
}

void UFTCareerSubsystem::Load()
{
	bLoadError = false; LoadError.Reset(); bSaveError = false; SaveError.Reset();
	bRecoveredFromBackup = false; bPreserveBackup = false; bHasPersistedCareer = false;
	LastSavedText.Reset(); LastSaveTime = 0; LastCommitOut = FFTCommitOut();
	Fresh();
	FString Text, PrimaryError, BackupError;
	const FString Path = FilePath(), Bak = Path + TEXT(".bak");
	IFileManager& FM = IFileManager::Get();
	UFTGameInstance* Banner = Cast<UFTGameInstance>(GetGameInstance());
	TSharedPtr<FJsonObject> J;
	const bool bHave = FM.FileExists(*Path), bHaveBackup = FM.FileExists(*Bak);
	bool bRead = bHave && ReadCareer(Path, J, Text, PrimaryError);
	if (!bRead && bHaveBackup)
	{
		bRead = ReadCareer(Bak, J, Text, BackupError);
		bRecoveredFromBackup = bRead;
		bPreserveBackup = bRead;
		if (bRead && Banner) Banner->TitleBanner = TEXT("SAVE RESTORED FROM BACKUP · ORIGINAL FILE PRESERVED");
	}
	if (!bRead)
	{
		if (bHave || bHaveBackup)
		{
			bLoadError = true;
			LoadError = PrimaryError.IsEmpty() ? BackupError : PrimaryError;
			if (LoadError.IsEmpty()) LoadError = TEXT("The save could not be recovered. Start New Game to replace it.");
			if (Banner) Banner->TitleBanner = TEXT("SAVE UNAVAILABLE · ORIGINAL FILES PRESERVED");
			UE_LOG(LogFT, Warning, TEXT("FT LOAD slot=%s result=refused error=%s"), *Slot, *LoadError);
		}
		else
		{
			// First launch is strictly read-only. The optional legacy bonus stays in
			// memory until an explicit campaign start; no empty Continue is created.
			int64 Best = 0;
			if (Banner) for (int32 i = 0; i < 3; ++i) Best += Banner->BestScore[i];
			if (Best > 0) GiveXP(Best / 30, TEXT("BackCatalogue"));
			UE_LOG(LogFT, Display, TEXT("FT LOAD slot=%s result=missing"), *Slot);
		}
		return;
	}
	int32 V = 1; J->TryGetNumberField(TEXT("v"), V);
	const TSharedPtr<FJsonObject>* O;
	if (J->TryGetObjectField(TEXT("driver"), O))
	{
		// (numbers are clamped before the cast: a huge or negative value in a hand-edited file is not a valid int64)
		(*O)->TryGetNumberField(TEXT("level"), C.Level); double X = 0; (*O)->TryGetNumberField(TEXT("xp"), X); C.XP = (int64)FMath::Clamp(X, 0.0, 9.0e15);
		(*O)->TryGetNumberField(TEXT("sp"), C.SP); (*O)->TryGetNumberField(TEXT("spSpent"), C.SPSpent);
	}
	double Cash = 0, Earned = 0; J->TryGetNumberField(TEXT("cash"), Cash); J->TryGetNumberField(TEXT("cashEarned"), Earned); C.Cash = (int64)FMath::Clamp(Cash, 0.0, 9.0e15); C.CashEarned = (int64)FMath::Clamp(Earned, 0.0, 9.0e15);
	if (J->TryGetObjectField(TEXT("skills"), O)) for (const auto& KV : (*O)->Values) if (KV.Value->AsBool()) C.Skills.AddUnique(FName(*KV.Key));
	const TArray<TSharedPtr<FJsonValue>>* A;
	if (J->TryGetArrayField(TEXT("skills"), A)) for (const auto& Sv : *A) C.Skills.AddUnique(FName(*Sv->AsString()));
	if (J->TryGetObjectField(TEXT("pending"), O))
	{
		if ((*O)->TryGetArrayField(TEXT("skills"), A)) for (const auto& Sv : *A) C.PendingSkills.AddUnique(FName(*Sv->AsString()));
		double Lv = 0;
		if ((*O)->TryGetNumberField(TEXT("level"), Lv)) C.PendingLevel = Lv >= 1 ? (int32)Lv - 1 : -2; else C.PendingLevel = -1;
		(*O)->TryGetBoolField(TEXT("fromTitle"), C.bPendingFromTitle);
	}
	if (J->TryGetObjectField(TEXT("garage"), O)) for (const auto& KV : (*O)->Values) C.Garage.Add(FName(*KV.Key), (int32)KV.Value->AsNumber());
	if (J->TryGetArrayField(TEXT("greenlit"), A)) for (const auto& Sv : *A) C.Greenlit.AddUnique(Sv->AsString());
	if (J->TryGetObjectField(TEXT("extras"), O)) { (*O)->TryGetNumberField(TEXT("film"), C.Film); (*O)->TryGetBoolField(TEXT("sequel"), C.bSequel); (*O)->TryGetBoolField(TEXT("sequelOn"), C.bSequelOn); }
	if (J->TryGetObjectField(TEXT("custom"), O))
	{
		if ((*O)->TryGetArrayField(TEXT("own"), A)) for (const auto& Sv : *A) C.Own.AddUnique(FName(*Sv->AsString()));
		for (auto& KV : C.Wear)
		{
			const TSharedPtr<FJsonValue> Vv = (*O)->TryGetField(KV.Key.ToString());
			if (Vv.IsValid()) KV.Value = Vv->IsNull() ? NAME_None : FName(*Vv->AsString());
		}
	}
	J->TryGetNumberField(TEXT("respecs"), C.Respecs);
	if (J->TryGetArrayField(TEXT("feats"), A)) for (const auto& Sv : *A) C.Feats.AddUnique(FFTProgressData::StuntType(Sv->AsString()));
	if (J->TryGetObjectField(TEXT("notes"), O)) for (const auto& KV : (*O)->Values) C.Notes.Add(FString(KV.Key), KV.Value->AsBool());
	auto B = [](const TSharedPtr<FJsonValue>& X) { return X->AsBool(); };
	auto I = [](const TSharedPtr<FJsonValue>& X) { return (int32)X->AsNumber(); };
	auto Fl = [](const TSharedPtr<FJsonValue>& X) { return (float)X->AsNumber(); };
	RArr(J, TEXT("legendary"), C.Legendary, B); RArr(J, TEXT("bosses"), C.Bosses, B);
	if (J->TryGetObjectField(TEXT("stats"), O))
	{
		FFTCareerStats& S = C.Stats; const TSharedPtr<FJsonObject>& So = *O;
		So->TryGetNumberField(TEXT("runs"), S.Runs); So->TryGetNumberField(TEXT("wraps"), S.Wraps); So->TryGetNumberField(TEXT("wrecks"), S.Wrecks);
		So->TryGetNumberField(TEXT("leftBehind"), S.LeftBehind); So->TryGetNumberField(TEXT("civilians"), S.Civilians);
		So->TryGetNumberField(TEXT("trucks"), S.Trucks); So->TryGetNumberField(TEXT("snakeEyes"), S.SnakeEyes);
		So->TryGetBoolField(TEXT("cleanWrap"), S.bCleanWrap); So->TryGetBoolField(TEXT("armorWrap"), S.bArmorWrap);
		RArr(So, TEXT("bestCarnage"), S.BestCarnage, Fl); RArr(So, TEXT("bestTime"), S.BestTime, Fl); RArr(So, TEXT("wrapped"), S.Wrapped, B);
		RArr(So, TEXT("bestRating"), S.BestRating, I); RArr(So, TEXT("bossWrecks"), S.BossWrecks, I); RArr(So, TEXT("runDown"), S.RunDown, B);
	}
	J->TryGetBoolField(TEXT("tipSeen"), C.bTipSeen);
	J->TryGetStringField(TEXT("updated"), C.Updated);
	Fix();
	if (V == 2 && J->TryGetObjectField(TEXT("resume"), O))
	{
		(*O)->TryGetStringField(TEXT("kind"), C.ResumeKind); (*O)->TryGetNumberField(TEXT("level"), C.ResumeLevel);
		(*O)->TryGetNumberField(TEXT("stage"), C.ResumeStage); (*O)->TryGetBoolField(TEXT("wrap"), C.bResumeWrap);
		J->TryGetStringField(TEXT("runId"), C.RunId); J->TryGetStringField(TEXT("lastCommitId"), C.LastCommitId);
	}
	else if (V == 1)
	{
		// Old saves have no active encounter. Resume at their saved preparation
		// screen, or at preparation for the first unfinished race; never repay it.
		if (C.PendingLevel == -2) { C.ResumeKind = TEXT("complete"); C.ResumeLevel = FTData::NumLevels() - 1; }
		else if (C.PendingLevel >= 0) { C.ResumeKind = TEXT("preproduction"); C.ResumeLevel = C.PendingLevel; }
		else if (C.Stats.Runs > 0 || C.XP > 0 || C.CashEarned > 0 || C.Skills.Num() > 0)
		{
			int32 Next = 0; while (Next < C.Stats.Wrapped.Num() && C.Stats.Wrapped[Next]) ++Next;
			C.ResumeKind = Next >= FTData::NumLevels() ? TEXT("complete") : TEXT("preproduction");
			C.ResumeLevel = FMath::Min(Next, FTData::NumLevels() - 1);
		}
	}
	C.V = 2;
	UE_LOG(LogFT, Display, TEXT("FT LOAD slot=%s result=ok"), *Slot);
	PersistedCareer = C; bHasPersistedCareer = true;
	if (V == 2 && !bRecoveredFromBackup) LastSavedText = Text;
	LogCareer();
}

bool UFTCareerSubsystem::HasContinue() const
{
	return bHasPersistedCareer && !bLoadError && PersistedCareer.ResumeKind != TEXT("none")
		&& ValidCheckpoint(PersistedCareer.ResumeKind, PersistedCareer.ResumeLevel, PersistedCareer.ResumeStage);
}

FString UFTCareerSubsystem::ContinueLabel() const
{
	if (!HasContinue()) return bLoadError ? LoadError : TEXT("NO SAVED CAMPAIGN");
	const FFTCareer& Saved = PersistedCareer;
	const int32 Day = Saved.ResumeLevel + 1;
	if (Saved.ResumeKind == TEXT("race")) return FString::Printf(TEXT("DAY %d · %s · RESTART RACE"), Day, *FTData::Level(Saved.ResumeLevel).Zone);
	if (Saved.ResumeKind == TEXT("day")) return FString::Printf(TEXT("DAY %d · %s"), Day, Saved.bResumeWrap ? TEXT("THE WRAP") : TEXT("BEFORE THE DRIVE"));
	if (Saved.ResumeKind == TEXT("night") || Saved.ResumeKind == TEXT("nightoutro"))
	{
		static const TCHAR* Venues[] = { TEXT("THE RUSTY ANCHOR"), TEXT("THE VELVET ROOM"), TEXT("THE LUCKY 7"), TEXT("THE TIKI TORCH"), TEXT("STRIKE CITY"), TEXT("FROSTBITE LODGE"), TEXT("THE SEA BISCUIT") };
		const FString Point = Saved.ResumeKind == TEXT("nightoutro") ? TEXT("LAST CALL") : Saved.ResumeStage < 0 ? TEXT("INTRODUCTION") : FString::Printf(TEXT("RESTART ENCOUNTER %d"), Saved.ResumeStage + 1);
		return FString::Printf(TEXT("NIGHT %d · %s · %s"), Day, Venues[Saved.ResumeLevel], *Point);
	}
	if (Saved.ResumeKind == TEXT("complete")) return TEXT("CAMPAIGN COMPLETE · PRE-PRODUCTION");
	return FString::Printf(TEXT("DAY %d · PRE-PRODUCTION"), Day);
}

bool UFTCareerSubsystem::SetCheckpoint(const FString& Kind, int32 Level, int32 Stage, bool bWrap, const TCHAR* Reason)
{
	if (!ValidCheckpoint(Kind, Level, Stage))
	{ bSaveError = true; SaveError = TEXT("The requested checkpoint is not supported."); return false; }
	if (Kind == TEXT("race") && (C.RunId.IsEmpty() || C.RunId == C.LastCommitId || C.ResumeKind != Kind || C.ResumeLevel != Level))
		C.RunId = FGuid::NewGuid().ToString(EGuidFormats::Digits);
	C.ResumeKind = Kind; C.ResumeLevel = Level; C.ResumeStage = Stage; C.bResumeWrap = bWrap;
	return Save(Reason);
}

bool UFTCareerSubsystem::SaveMutation(const FFTCareer& Before, const TCHAR* Reason)
{
	if (Save(Reason)) return true;
	C = Before;
	return false;
}

void UFTCareerSubsystem::Fix()
{
	const FFTProgressData& D = FFTProgressData::Get();
	SizeArrays();
	// a hand-edited or damaged file: nothing below zero, and no pending level past the last one (99 showed "LEVEL 99")
	C.Cash = FMath::Max<int64>(0, C.Cash); C.CashEarned = FMath::Max<int64>(0, C.CashEarned); C.XP = FMath::Max<int64>(0, C.XP);
	C.SP = FMath::Max(0, C.SP); C.SPSpent = FMath::Max(0, C.SPSpent); C.Respecs = FMath::Max(0, C.Respecs);
	if (C.PendingLevel >= FTData::NumLevels()) C.PendingLevel = -1;
	const int32 Lv = D.LevelForXP(C.XP);
	if (Lv != C.Level) { UE_LOG(LogFT, Display, TEXT("FT LOAD fix=driver.level stored=%d xp=%lld level=%d"), C.Level, C.XP, Lv); C.Level = Lv; }
	for (const TCHAR* S : { TEXT("p-kestrel-red"), TEXT("r-gold-five-hole"), TEXT("s-kestrel-pink"), TEXT("s-hot-pink-nitro") }) C.Own.AddUnique(S);
	const TMap<FName, FName> Defaults = { { TEXT("paint"), TEXT("p-kestrel-red") }, { TEXT("rims"), TEXT("r-gold-five-hole") }, { TEXT("neon"), TEXT("s-kestrel-pink") }, { TEXT("nitro"), TEXT("s-hot-pink-nitro") } };
	for (auto& KV : C.Wear)
	{
		const FFTCosmeticRow* Row = KV.Value.IsNone() ? nullptr : D.Cosmetic(KV.Value);
		const bool bBad = !KV.Value.IsNone() && (!Owns(KV.Value) || !Row || Row->Slot != KV.Key);
		if (bBad || (KV.Value.IsNone() && Defaults.Contains(KV.Key))) KV.Value = Defaults.Contains(KV.Key) ? Defaults[KV.Key] : NAME_None;
	}
	for (auto& KV : C.Garage) KV.Value = FMath::Clamp(KV.Value, 0, 3);
	for (const FFTGarageLine& G : D.Garage) if (!C.Garage.Contains(G.Key)) C.Garage.Add(G.Key, 0);
	// skills not in the data are dropped and their SP refunded: spent is recomputed from the nodes that remain
	const int32 Earned = C.SP + C.SPSpent;
	C.Skills.RemoveAll([&D](const FName& Id) { return D.Node(Id) == nullptr; });
	C.PendingSkills.RemoveAll([&D, this](const FName& Id) { return D.Node(Id) == nullptr || C.Skills.Contains(Id); });
	int32 Spent = 0; for (const FName& Id : C.Skills) Spent += D.Node(Id)->SP;
	C.SPSpent = Spent; C.SP = FMath::Max(0, Earned - Spent);
	C.Film = FMath::Clamp(C.Film, 0, D.FilmMax);
}

FString UFTCareerSubsystem::Dump() const
{
	FString Sk; for (const FName& S : C.Skills) Sk += (Sk.IsEmpty() ? TEXT("") : TEXT(",")) + S.ToString();
	FString Pe; for (const FName& S : C.PendingSkills) Pe += (Pe.IsEmpty() ? TEXT("") : TEXT(",")) + S.ToString();
	FString Ga; for (const auto& KV : C.Garage) Ga += FString::Printf(TEXT(" garage.%s=%d"), *KV.Key.ToString().ToLower(), KV.Value);
	FString Gl = FString::Join(C.Greenlit, TEXT(","));
	FString Own; for (const FName& S : C.Own) Own += (Own.IsEmpty() ? TEXT("") : TEXT(",")) + S.ToString();
	FString Wr; for (const auto& KV : C.Wear) Wr += FString::Printf(TEXT(" custom.%s=%s"), *KV.Key.ToString().ToLower(), KV.Value.IsNone() ? TEXT("null") : *KV.Value.ToString());
	const FFTCareerStats& S = C.Stats;
	return FString::Printf(TEXT("slot=%s level=%d xp=%lld sp=%d spSpent=%d cash=%lld cashEarned=%lld skills=[%s] pending.skills=[%s] pending.level=%d%s greenlit=%s film=%d sequel=%d respecs=%d custom.own=[%s]%s feats=%d notes=%d stats.runs=%d stats.wraps=%d stats.wrecks=%d stats.leftBehind=%d stats.civilians=%d stats.trucks=%d stats.snakeEyes=%d"),
		*Slot, C.Level, C.XP, C.SP, C.SPSpent, C.Cash, C.CashEarned, *Sk, *Pe, C.PendingLevel >= 0 ? C.PendingLevel + 1 : C.PendingLevel, *Ga, Gl.IsEmpty() ? TEXT("-") : *Gl,
		C.Film, C.bSequel ? 1 : 0, C.Respecs, *Own, *Wr, C.Feats.Num(), C.Notes.Num(), S.Runs, S.Wraps, S.Wrecks, S.LeftBehind, S.Civilians, S.Trucks, S.SnakeEyes);
}

void UFTCareerSubsystem::LogCareer() const
{
	UE_LOG(LogFT, Display, TEXT("FT CAREER level=%d xp=%lld sp=%d cash=%lld"), C.Level, C.XP, C.SP, C.Cash);
}

void UFTCareerSubsystem::SetRaw(const FString& Field, const FString& Value)
{
	if (Field == TEXT("driver.level")) C.Level = FCString::Atoi(*Value);
	else if (Field == TEXT("driver.xp")) C.XP = FCString::Atoi64(*Value);
	else if (Field == TEXT("driver.sp")) C.SP = FCString::Atoi(*Value);
	else if (Field == TEXT("cash")) C.Cash = FCString::Atoi64(*Value);
	else if (Field == TEXT("v")) C.V = FCString::Atoi(*Value);
	UE_LOG(LogFT, Display, TEXT("FT CAREER raw %s=%s"), *Field, *Value);
}

int32 UFTCareerSubsystem::PendingCost() const
{
	int32 N = 0;
	for (const FName& Id : C.PendingSkills) if (const FFTSkillNodeRow* R = FFTProgressData::Get().Node(Id)) N += R->SP;
	return N;
}

TArray<TPair<int32, int32>> UFTCareerSubsystem::GiveXP(int64 N, const TCHAR* Source)
{
	TArray<TPair<int32, int32>> Ups;
	if (N <= 0) return Ups;
	const FFTProgressData& D = FFTProgressData::Get();
	C.XP += N;
	const int32 NewLevel = D.LevelForXP(C.XP);
	while (C.Level < NewLevel)
	{
		C.Level++;
		const int32 Sp = D.SPFor(C.Level);
		C.SP += Sp;
		Ups.Add(TPair<int32, int32>(C.Level, Sp));
		UE_LOG(LogFT, Display, TEXT("FT LEVELUP level=%d sp=+%d"), C.Level, Sp);
	}
	UE_LOG(LogFT, Display, TEXT("FT XP career delta=%lld total=%lld source=%s"), N, C.XP, Source);
	LogCareer();
	return Ups;
}

void UFTCareerSubsystem::SetXP(int64 XP)
{
	// setup for checks: the level and the SP it pays, as if earned from a fresh career
	const FFTProgressData& D = FFTProgressData::Get();
	C.XP = FMath::Max<int64>(0, XP);
	C.Level = D.LevelForXP(C.XP);
	int32 Sp = 0; for (int32 L = 2; L <= C.Level; ++L) Sp += D.SPFor(L);
	C.SP = FMath::Max(0, Sp - C.SPSpent);
	LogCareer();
}

// ------------------------------------------------------------------------------------------------ skills
EFTTile UFTCareerSubsystem::SkillState(FName Id, FString* Needs) const
{
	const FFTProgressData& D = FFTProgressData::Get();
	const FFTSkillNodeRow* N = D.Node(Id);
	if (!N) return EFTTile::Locked;
	if (C.Skills.Contains(Id)) return EFTTile::Owned;
	if (C.PendingSkills.Contains(Id)) return EFTTile::Pending;
	TArray<FString> Why;
	if (N->Tier > 1)
	{
		const FName Above(*FString::Printf(TEXT("%s%d"), *N->Branch, N->Tier - 1));
		if (!C.Skills.Contains(Above) && !C.PendingSkills.Contains(Above)) Why.Add(FString::Printf(TEXT("%s FIRST"), *Above.ToString()));
	}
	if (C.Level < N->Level) Why.Insert(FString::Printf(TEXT("LV %d"), N->Level), 0);
	if (N->bGreenlight && !IsGreenlit(N->Branch)) Why.Add(TEXT("greenlight"));
	if (SPLeft() < N->SP) Why.Add(FString::Printf(TEXT("%d SP"), N->SP));
	if (Needs) *Needs = FString::Join(Why, TEXT(" · "));
	return Why.Num() ? EFTTile::Locked : EFTTile::Available;
}

bool UFTCareerSubsystem::BuySkill(FName Id)
{
	const FFTCareer Before = C;
	if (SkillState(Id) != EFTTile::Available) { UE_LOG(LogFT, Display, TEXT("FT SKILL id=%s state=refused"), *Id.ToString()); return false; }
	C.PendingSkills.Add(Id);
	UE_LOG(LogFT, Display, TEXT("FT SKILL id=%s state=PENDING"), *Id.ToString());
	UE_LOG(LogFT, Display, TEXT("FT SP left=%d spent=%d"), SPLeft(), C.SPSpent + PendingCost());
	return SaveMutation(Before, TEXT("Purchase"));
}

bool UFTCareerSubsystem::UndoSkill(FName Id)
{
	const FFTCareer Before = C;
	if (Id.IsNone()) { if (!C.PendingSkills.Num()) return false; Id = C.PendingSkills.Last(); }
	if (!C.PendingSkills.Contains(Id)) return false;
	const FFTSkillNodeRow* N = FFTProgressData::Get().Node(Id);
	// Remove dependent pending nodes in this same transaction, so a failed write
	// cannot leave only half of a branch undone.
	if (N) C.PendingSkills.RemoveAll([N](FName Candidate) { const FFTSkillNodeRow* Other = FFTProgressData::Get().Node(Candidate); return Other && Other->Branch == N->Branch && Other->Tier > N->Tier; });
	C.PendingSkills.Remove(Id);
	UE_LOG(LogFT, Display, TEXT("FT SKILL id=%s state=UNDONE"), *Id.ToString());
	UE_LOG(LogFT, Display, TEXT("FT SP left=%d spent=%d"), SPLeft(), C.SPSpent + PendingCost());
	return SaveMutation(Before, TEXT("Undo"));
}

bool UFTCareerSubsystem::Ready()
{
	const FFTCareer Before = C;
	const FFTProgressData& D = FFTProgressData::Get();
	for (const FName& Id : C.PendingSkills)
	{
		const FFTSkillNodeRow* N = D.Node(Id);
		if (!N) continue;
		C.Skills.AddUnique(Id); C.SP -= N->SP; C.SPSpent += N->SP;
		UE_LOG(LogFT, Display, TEXT("FT SKILL id=%s state=OWNED"), *Id.ToString());
	}
	C.PendingSkills.Reset();
	C.PendingLevel = -1; C.bPendingFromTitle = false;
	UE_LOG(LogFT, Display, TEXT("FT SP left=%d spent=%d"), C.SP, C.SPSpent);
	return SaveMutation(Before, TEXT("Ready"));
}

EFTTile UFTCareerSubsystem::GreenlightState(const FString& Code, FString* Needs) const
{
	if (IsGreenlit(Code)) return EFTTile::Owned;
	if (Needs) *Needs = FString();
	return EFTTile::Available;
}

bool UFTCareerSubsystem::Spend(int64 Price, const TCHAR* What)
{
	if (C.Cash < Price) { UE_LOG(LogFT, Display, TEXT("FT CASH refused what=%s price=%lld cash=%lld"), What, Price, C.Cash); return false; }
	C.Cash -= Price;
	UE_LOG(LogFT, Display, TEXT("FT CASH delta=%lld what=%s cash=%lld"), -Price, What, C.Cash);
	return true;
}

bool UFTCareerSubsystem::BuyGreenlight(const FString& Code)
{
	const FFTCareer Before = C;
	if (IsGreenlit(Code)) return false;
	if (!Spend(FFTProgressData::Get().GreenlightPrice, *(TEXT("greenlight-") + Code))) return false;
	C.Greenlit.Add(Code);
	UE_LOG(LogFT, Display, TEXT("FT SKILL greenlight=%s"), *Code);
	return SaveMutation(Before, TEXT("Purchase"));
}

int32 UFTCareerSubsystem::RespecPrice() const { return C.Respecs == 0 ? 0 : FFTProgressData::Get().RespecPrice; }

bool UFTCareerSubsystem::Respec()
{
	const FFTCareer Before = C;
	const int32 Price = RespecPrice();
	if (C.Cash < Price) { UE_LOG(LogFT, Display, TEXT("FT CASH refused what=respec price=%d cash=%lld"), Price, C.Cash); return false; }
	C.Cash -= Price;
	UE_LOG(LogFT, Display, TEXT("FT CASH delta=%d what=respec cash=%lld"), -Price, C.Cash);
	C.SP += C.SPSpent; C.SPSpent = 0;
	C.Skills.Reset(); C.PendingSkills.Reset();
	C.Respecs++;
	UE_LOG(LogFT, Display, TEXT("FT SKILL respec=%d"), C.Respecs);
	UE_LOG(LogFT, Display, TEXT("FT SP left=%d spent=%d"), C.SP, C.SPSpent);
	return SaveMutation(Before, TEXT("Purchase"));
}

// ------------------------------------------------------------------------------------------------ feats
bool UFTCareerSubsystem::IsFeatMet(const FFTFeatSpec& F, FString* Needs) const
{
	if (!F.IsSet()) return true;
	const FFTCareerStats& S = C.Stats;
	const FFTProgressData& D = FFTProgressData::Get();
	auto Count = [&](int32 Have, int32 Want, const TCHAR* Unit) { if (Needs) *Needs = FString::Printf(TEXT("%d / %d %s"), FMath::Min(Have, Want), Want, Unit); return Have >= Want; };
	auto Say = [&](bool bOk) { if (Needs) *Needs = F.Text.ToUpper(); return bOk; };
	const int32 N3 = FMath::Min(3, FTData::NumLevels());
	const FName T = F.Type;
	if (T == TEXT("wrecks")) return Count(S.Wrecks, F.N, TEXT("WRECKS"));
	if (T == TEXT("trucks")) return F.N <= 1 ? Say(S.Trucks >= 1) : Count(S.Trucks, F.N, TEXT("TRUCKS"));
	if (T == TEXT("snakeEyes")) return Count(S.SnakeEyes, F.N, TEXT("SNAKE EYES"));
	if (T == TEXT("leftBehind")) return Count(S.LeftBehind, F.N, TEXT("LEFT BEHIND"));
	if (T == TEXT("notes")) { int32 N = 0; for (const auto& KV : C.Notes) N += KV.Value; return Count(N, F.N, TEXT("NOTES")); }
	if (T == TEXT("stuntTypes")) return Count(C.Feats.Num(), F.N, TEXT("STUNT TYPES"));
	if (T == TEXT("wraps")) return Count(S.Wraps, F.N, TEXT("WRAPS"));
	if (T == TEXT("driverLevel")) return Say(C.Level >= F.N);
	if (T == TEXT("carnageAny")) { bool b = false; for (int32 i = 0; i < N3; ++i) b |= S.BestCarnage[i] >= F.N / 100.f - 1e-4f; return Say(b); }
	if (T == TEXT("carnageAll")) { bool b = true; for (int32 i = 0; i < N3; ++i) b &= S.BestCarnage[i] >= F.N / 100.f - 1e-4f; return Say(b); }
	if (T == TEXT("ratingAny")) { bool b = false; for (int32 i = 0; i < N3; ++i) b |= S.BestRating[i] >= F.N; return Say(b); }
	if (T == TEXT("ratingAll")) { bool b = true; for (int32 i = 0; i < N3; ++i) b &= S.BestRating[i] >= F.N; return Say(b); }
	if (T == TEXT("ratingLevel")) return Say(S.BestRating.IsValidIndex(F.K) && S.BestRating[F.K] >= F.N);
	if (T == TEXT("feat")) { bool b = true; for (const FString& Nm : F.Names) b &= HasFeat(Nm); return Say(b); }
	if (T == TEXT("featsAll")) { bool b = true; for (const FString& Nm : F.Names) b &= HasFeat(Nm); return Say(b); }
	if (T == TEXT("garage")) return Say(Rank(F.Line) >= F.N);
	if (T == TEXT("runDownAll")) { bool b = true; for (int32 i = 0; i < N3; ++i) b &= S.RunDown[i]; return Say(b); }
	if (T == TEXT("wrapped")) return Say(S.Wrapped.IsValidIndex(F.K) && S.Wrapped[F.K]);
	if (T == TEXT("underParAny") || T == TEXT("underParAll"))
	{
		const bool bAll = T == TEXT("underParAll"); bool b = bAll;
		for (int32 i = 0; i < N3; ++i) { const bool u = S.BestTime[i] > 0.f && S.BestTime[i] < D.Levels[i].Par; b = bAll ? (b && u) : (b || u); }
		return Say(b);
	}
	if (T == TEXT("cleanWrap")) return Say(S.bCleanWrap);
	if (T == TEXT("armorWrap")) return Say(S.bArmorWrap);
	if (T == TEXT("bossesAll")) { int32 Mn = 1 << 30; for (int32 i = 0; i < N3; ++i) Mn = FMath::Min(Mn, S.BossWrecks[i]); return F.N <= 1 ? Say(Mn >= 1) : Count(Mn, F.N, TEXT("EACH BOSS")); }
	return Say(false);
}

// ------------------------------------------------------------------------------------------------ garage
EFTTile UFTCareerSubsystem::GarageState(FName Key, int32 R, FString* Needs) const
{
	const FFTGarageLine* G = FFTProgressData::Get().Line(Key);
	if (!G || R < 1 || R > 3) return EFTTile::Locked;
	const int32 Have = Rank(Key);
	if (Have >= R) return EFTTile::Owned;
	const FFTRankRow& Rr = G->Ranks[R - 1];
	const bool bAmmo = G->Row == TEXT("AMMO");
	TArray<FString> Why;
	if (C.Level < Rr.Level) Why.Add(bAmmo ? FString::Printf(TEXT("DRIVER LV %d"), Rr.Level) : FString::Printf(TEXT("LV %d"), Rr.Level));
	FString FN;
	if (Rr.Feat.IsSet() && !IsFeatMet(Rr.Feat, &FN)) Why.Add(FN);
	if (Have < R - 1) Why.Add(FString::Printf(TEXT("RANK %d FIRST"), R - 1));
	if (Needs) *Needs = FString::Join(Why, TEXT(" · "));
	return Why.Num() ? EFTTile::Locked : EFTTile::Available;
}

bool UFTCareerSubsystem::BuyGarage(FName Key, int32 R)
{
	const FFTCareer Before = C;
	FString Why;
	if (GarageState(Key, R, &Why) != EFTTile::Available) { UE_LOG(LogFT, Display, TEXT("FT GARAGE key=%s rank=%d refused needs=\"%s\""), *Key.ToString(), R, *Why); return false; }
	const FFTGarageLine* G = FFTProgressData::Get().Line(Key);
	const int32 Price = G->Ranks[R - 1].Price;
	if (!Spend(Price, *Key.ToString())) return false;
	C.Garage.Add(Key, R);
	UE_LOG(LogFT, Display, TEXT("FT GARAGE key=%s rank=%d price=%d"), *Key.ToString(), R, Price);
	return SaveMutation(Before, TEXT("Purchase"));
}

// ------------------------------------------------------------------------------------------------ cosmetics
EFTTile UFTCareerSubsystem::CosmeticState(FName Id, FString* Needs) const
{
	const FFTCosmeticRow* R = FFTProgressData::Get().Cosmetic(Id);
	if (!R) return EFTTile::Locked;
	if (Owns(Id)) return Worn(R->Slot) == Id ? EFTTile::Equipped : EFTTile::Owned;
	if (!R->bReady) { if (Needs) *Needs = R->NotReadyWhy; return EFTTile::Soon; }
	TArray<FString> Why;
	if (C.Level < R->Level) Why.Add(FString::Printf(TEXT("LV %d"), R->Level));
	FString FN;
	if (R->Feat.IsSet() && !IsFeatMet(R->Feat, &FN)) Why.Add(FN);
	if (Needs) *Needs = FString::Join(Why, TEXT(" · "));
	return Why.Num() ? EFTTile::Locked : EFTTile::Available;
}

bool UFTCareerSubsystem::BuyCosmetic(FName Id)
{
	const FFTCareer Before = C;
	FString Why;
	if (CosmeticState(Id, &Why) != EFTTile::Available) { UE_LOG(LogFT, Display, TEXT("FT SHOP id=%s refused needs=\"%s\""), *Id.ToString(), *Why); return false; }
	const FFTCosmeticRow* R = FFTProgressData::Get().Cosmetic(Id);
	if (!Spend(R->Price, *Id.ToString())) return false;
	C.Own.AddUnique(Id);
	UE_LOG(LogFT, Display, TEXT("FT SHOP id=%s price=%d"), *Id.ToString(), R->Price);
	return SaveMutation(Before, TEXT("Purchase"));
}

bool UFTCareerSubsystem::Equip(FName SlotName, FName Id)
{
	const FFTCareer Before = C;
	const bool bFixed = SlotName == TEXT("paint") || SlotName == TEXT("rims") || SlotName == TEXT("neon") || SlotName == TEXT("nitro");
	if (Id.IsNone() || Id == TEXT("none"))
	{
		if (bFixed) { UE_LOG(LogFT, Display, TEXT("FT SHOP equip refused slot=%s"), *SlotName.ToString()); return false; }
		C.Wear.Add(SlotName, NAME_None);
		UE_LOG(LogFT, Display, TEXT("FT SHOP equip slot=%s id=null"), *SlotName.ToString());
		return SaveMutation(Before, TEXT("Equip"));
	}
	const FFTCosmeticRow* R = FFTProgressData::Get().Cosmetic(Id);
	if (!R || R->Slot != SlotName || !Owns(Id)) { UE_LOG(LogFT, Display, TEXT("FT SHOP equip refused slot=%s id=%s"), *SlotName.ToString(), *Id.ToString()); return false; }
	C.Wear.Add(SlotName, Id);
	UE_LOG(LogFT, Display, TEXT("FT SHOP equip slot=%s id=%s"), *SlotName.ToString(), *Id.ToString());
	return SaveMutation(Before, TEXT("Equip"));
}

// ------------------------------------------------------------------------------------------------ extras
EFTTile UFTCareerSubsystem::FilmState(FString* Needs) const
{
	if (C.Film >= FFTProgressData::Get().FilmMax) { if (Needs) *Needs = TEXT("2 PER RUN"); return EFTTile::Owned; }
	return EFTTile::Available;
}
bool UFTCareerSubsystem::BuyFilm()
{
	const FFTCareer Before = C;
	if (FilmState() != EFTTile::Available || !Spend(FFTProgressData::Get().FilmPrice, TEXT("film"))) return false;
	C.Film++;
	return SaveMutation(Before, TEXT("Purchase"));
}
EFTTile UFTCareerSubsystem::SequelState(FString* Needs) const { return C.bSequel ? EFTTile::Owned : EFTTile::Available; }
bool UFTCareerSubsystem::BuySequel()
{
	const FFTCareer Before = C;
	if (C.bSequel || !Spend(FFTProgressData::Get().SequelPrice, TEXT("sequel"))) return false;
	C.bSequel = true; C.bSequelOn = true;
	return SaveMutation(Before, TEXT("Purchase"));
}
bool UFTCareerSubsystem::ToggleSequel() { if (!C.bSequel) return false; const FFTCareer Before = C; C.bSequelOn = !C.bSequelOn; return SaveMutation(Before, TEXT("Equip")); }

FFTCareerSnapshot UFTCareerSubsystem::Snapshot() const
{
	FFTCareerSnapshot S;
	S.bScorched = HasSkill(TEXT("DEM5")); S.bCrowd = HasSkill(TEXT("STN3")); S.bBigFinish = HasSkill(TEXT("DEM6")); S.bAhead = HasSkill(TEXT("GET5"));
	S.bSequel = C.bSequel && C.bSequelOn;
	return S;
}

TSet<FString> UFTCareerSubsystem::LockedSet() const
{
	TSet<FString> L;
	const FFTProgressData& D = FFTProgressData::Get();
	for (const FFTGarageLine& G : D.Garage) for (int32 r = 1; r <= 3; ++r)
	{
		if (Rank(G.Key) >= r) continue;
		// "unlocked" = everything but the previous rank is met
		const FFTRankRow& Rr = G.Ranks[r - 1];
		if (C.Level < Rr.Level || (Rr.Feat.IsSet() && !IsFeatMet(Rr.Feat))) L.Add(FString::Printf(TEXT("g:%s:%d"), *G.Key.ToString(), r));
	}
	for (const FFTCosmeticRow& Cr : D.Cosmetics)
		if (!Owns(Cr.Id) && Cr.bReady && CosmeticState(Cr.Id) == EFTTile::Locked) L.Add(TEXT("c:") + Cr.Id.ToString());
	return L;
}

// ------------------------------------------------------------------------------------------------ the paycheck commit
FFTCommitOut UFTCareerSubsystem::Commit(const FFTLevelCommit& In)
{
	const FFTProgressData& D = FFTProgressData::Get();
	FFTCommitOut Out;
	const FFTLevelResult& R = In.R;
	const int32 Lv = FMath::Clamp(R.Level, 0, FTData::NumLevels() - 1);
	if (C.RunId.IsEmpty()) C.RunId = FGuid::NewGuid().ToString(EGuidFormats::Digits);
	if (C.LastCommitId == C.RunId)
	{
		Out = LastCommitOut;
		if (!Out.Pay.Lines.Num()) Out.Pay = FTPay::Compute(R, Snapshot(), D);
		Out.bDuplicate = true;
		Out.bSaved = Save(TEXT("PaycheckRetry"));
		UE_LOG(LogFT, Display, TEXT("FT PAY duplicate run=%s saved=%d"), *C.RunId, Out.bSaved ? 1 : 0);
		return Out;
	}
	const FString NextKind = In.NextKind.IsEmpty() ? (R.Outcome == EFTOutcome::Wrap ? TEXT("day") : TEXT("preproduction")) : In.NextKind;
	const int32 NextLevel = In.NextLevel < 0 ? Lv : In.NextLevel;
	if (!ValidCheckpoint(NextKind, NextLevel, In.NextStage) || NextKind == TEXT("none"))
	{ bSaveError = true; SaveError = TEXT("The reward's next checkpoint is invalid. No rewards were applied."); return Out; }
	const bool bWrap = R.Outcome == EFTOutcome::Wrap, bAway = R.Outcome == EFTOutcome::GotAway;
	const TSet<FString> Before = LockedSet();
	Out.LevelBefore = C.Level; Out.XPBefore = C.XP;

	// cash
	Out.Pay = FTPay::Compute(R, Snapshot(), D);
	for (const FFTPayLine& L : Out.Pay.Lines)
	{
		if (L.bMult) { UE_LOG(LogFT, Display, TEXT("FT PAY line=%s mult=%.4g amount=%.0f"), L.Key.Contains(TEXT(" ")) ? *(TEXT("\"") + L.Key + TEXT("\"")) : *L.Key, L.Mult, FMath::RoundToDouble(L.Amount / 10.0) * 10.0); }
		else { UE_LOG(LogFT, Display, TEXT("FT PAY line=%s amount=%.0f"), L.Key.Contains(TEXT(" ")) ? *(TEXT("\"") + L.Key + TEXT("\"")) : *L.Key, FMath::RoundToDouble(L.Amount / 10.0) * 10.0); }
	}
	UE_LOG(LogFT, Display, TEXT("FT PAY total=%lld"), Out.Pay.Total);
	C.Cash += Out.Pay.Total; C.CashEarned += Out.Pay.Total;
	UE_LOG(LogFT, Display, TEXT("FT CASH delta=%lld what=paycheck cash=%lld"), Out.Pay.Total, C.Cash);

	// XP (01 §2-§3)
	auto XL = [&Out](const TCHAR* Key, const FString& Label, int64 N) { if (N <= 0) return; FFTPayLine& L = Out.XPLines.AddDefaulted_GetRef(); L.Key = Key; L.Label = Label; L.Amount = (double)N; Out.XPGained += N; UE_LOG(LogFT, Display, TEXT("FT XP delta=%lld source=%s"), N, Key); };
	XL(TEXT("Level"), TEXT("XP FROM THE TAKES"), In.XP);
	if (bWrap) XL(TEXT("LevelWrapped"), TEXT("LEVEL WRAPPED"), D.Levels[Lv].WrapXP);
	if (R.Outcome != EFTOutcome::OutOfFilm) XL(TEXT("Rating"), TEXT("DIRECTOR'S RATING"), (int64)D.T(*FString::Printf(TEXT("XPRating%d"), FMath::Clamp(R.Rating, 0, 3))));
	if (R.bBossWrecked && !C.Bosses[Lv]) { C.Bosses[Lv] = true; XL(TEXT("BossFirst"), TEXT("FIRST WRECK OF THIS BOSS"), (int64)D.T(TEXT("XPBossFirst"), 500.f)); }
	for (const FString& Id : In.NotesDone)
	{
		if (C.Notes.FindRef(Id)) continue;
		C.Notes.Add(Id, true);
		const FFTNoteRow* NR = D.Notes.FindByPredicate([&Id](const FFTNoteRow& N) { return N.Id == Id; });
		XL(TEXT("Note"), FString::Printf(TEXT("DIRECTOR'S NOTE: %s"), NR ? *NR->Text.ToUpper() : *Id), (int64)D.T(TEXT("XPNote"), 300.f));
	}
	for (const FString& F : In.NewFeats) { if (!C.Feats.Contains(F)) { C.Feats.Add(F); UE_LOG(LogFT, Display, TEXT("FT FEAT committed=\"%s\""), *F); } }

	// stats (09 §3): from the final, post-retake counters
	FFTCareerStats& S = C.Stats;
	S.Wrecks += R.Wrecked; S.LeftBehind += In.LeftBehind; S.Civilians += R.Civilians; S.Trucks += In.Trucks; S.SnakeEyes += In.SnakeEyes;
	if (R.bBossWrecked) S.BossWrecks[Lv]++;
	if (In.bRunDown) S.RunDown[Lv] = true;
	if (bWrap || bAway)
	{
		S.BestCarnage[Lv] = FMath::Max(S.BestCarnage[Lv], (float)In.ExactCarnage);
		S.BestRating[Lv] = FMath::Max(S.BestRating[Lv], R.Rating);
	}
	if (bWrap)
	{
		S.Wraps++; S.Wrapped[Lv] = true;
		if (R.LevelTime > 0 && (S.BestTime[Lv] <= 0.f || R.LevelTime < S.BestTime[Lv])) S.BestTime[Lv] = (float)R.LevelTime;
		if (R.Civilians == 0) S.bCleanWrap = true;
		if (R.ArmorPct >= 50) S.bArmorWrap = true;
	}
	// first Legendary Director per level: +1 SP (01 §4)
	if (R.Rating == 3 && R.Outcome != EFTOutcome::OutOfFilm && !C.Legendary[Lv])
	{
		C.Legendary[Lv] = true; C.SP += 1; Out.LegendarySP = 1;
		UE_LOG(LogFT, Display, TEXT("FT SP delta=1 source=Legendary level=%d"), Lv + 1);
	}
	Out.LevelUps = GiveXP(Out.XPGained, TEXT("Paycheck"));

	// unlocks earned this level, one line each (03 §6, 06 §6, 07 §1)
	const TSet<FString> After = LockedSet();
	for (const FString& K : Before)
	{
		if (After.Contains(K)) continue;
		if (K.StartsWith(TEXT("g:")))
		{
			TArray<FString> P; K.ParseIntoArray(P, TEXT(":"));
			const FFTGarageLine* G = D.Line(FName(*P[1]));
			if (!G) continue;
			const int32 Rk = FCString::Atoi(*P[2]);
			if (G->Row == TEXT("AMMO") && Rk == 1) Out.Unlocks.Add(FString::Printf(TEXT("%s UNLOCKED · IN THE GARAGE"), *G->Name.ToUpper()));
			else Out.Unlocks.Add(FString::Printf(TEXT("%s RANK %d (%s) UNLOCKED · IN THE GARAGE"), *G->Name.ToUpper(), Rk, *G->Ranks[Rk - 1].Name.ToUpper()));
		}
		else if (const FFTCosmeticRow* Cr = D.Cosmetic(FName(*K.RightChop(2))))
			Out.Unlocks.Add(FString::Printf(TEXT("%s UNLOCKED · %s"), *Cr->Name.ToUpper(), Cr->Shop == TEXT("SFX") ? TEXT("IN THE SFX DEPT") : TEXT("IN CUSTOM")));
	}
	for (const FString& U : Out.Unlocks) UE_LOG(LogFT, Display, TEXT("FT PAY line=\"%s\""), *U);
	// Reward ledger and the next playable checkpoint are one disk transaction.
	// On failure retain this staged result for Retry Save; never credit it twice.
	C.LastCommitId = C.RunId;
	C.ResumeKind = NextKind; C.ResumeLevel = NextLevel; C.ResumeStage = In.NextStage;
	C.bResumeWrap = In.NextKind.IsEmpty() ? R.Outcome == EFTOutcome::Wrap : In.bNextWrap;
	Out.bSaved = Save(TEXT("Paycheck"));
	LastCommitOut = Out;
	return Out;
}

#if WITH_DEV_AUTOMATION_TESTS
namespace
{
struct FFTSaveTestScope
{
	FString Directory, Path;
	IConsoleVariable* Fault = nullptr;
	int32 PreviousFault = 0;
	FFTSaveTestScope()
	{
		Directory = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Automation"), TEXT("Career-") + FGuid::NewGuid().ToString(EGuidFormats::Digits));
		Path = FPaths::Combine(Directory, TEXT("career.json"));
		IFileManager::Get().MakeDirectory(*Directory, true);
		Fault = IConsoleManager::Get().FindConsoleVariable(TEXT("ft.SaveFail"));
		if (Fault) { PreviousFault = Fault->GetInt(); Fault->Set(0, ECVF_SetByConsole); }
	}
	~FFTSaveTestScope()
	{
		if (Fault) Fault->Set(PreviousFault, ECVF_SetByConsole);
		FPlatformFileManager::Get().GetPlatformFile().SetReadOnly(*Path, false);
		IFileManager::Get().DeleteDirectory(*Directory, false, true);
	}
	UFTCareerSubsystem* Career()
	{
		UGameInstance* Owner = NewObject<UGameInstance>();
		UFTCareerSubsystem* Career = NewObject<UFTCareerSubsystem>(Owner);
		Career->TestSavePath = Path; Career->Load();
		return Career;
	}
	FString Read(const FString& File = FString()) const
	{
		FString Text; FFileHelper::LoadFileToString(Text, *(File.IsEmpty() ? Path : File)); return Text;
	}
};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFTCareerFirstLaunchTest, "FT.Save.FirstLaunchAndMigration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)
bool FFTCareerFirstLaunchTest::RunTest(const FString&)
{
	FFTSaveTestScope Files;
	UFTCareerSubsystem* Career = Files.Career();
	TestFalse(TEXT("No Continue before a campaign exists"), Career->HasContinue());
	TestTrue(TEXT("First-launch window close has nothing to save"), Career->Save(TEXT("WindowClose")));
	TestFalse(TEXT("Load and first-launch exit create no file"), IFileManager::Get().FileExists(*Files.Path));
	TestTrue(TEXT("New Game writes its first checkpoint"), Career->Reset());
	TestTrue(TEXT("New Game can Continue immediately"), Files.Career()->HasContinue());
	TestEqual(TEXT("First checkpoint is the before-drive story"), Files.Career()->C.ResumeKind, FString(TEXT("day")));
	TSharedPtr<FJsonObject> Legacy;
	FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Files.Read()), Legacy);
	Legacy->SetNumberField(TEXT("v"), 1); Legacy->RemoveField(TEXT("resume")); Legacy->RemoveField(TEXT("runId")); Legacy->RemoveField(TEXT("lastCommitId"));
	Legacy->GetObjectField(TEXT("pending"))->SetNumberField(TEXT("level"), 2);
	FString Text; FJsonSerializer::Serialize(Legacy.ToSharedRef(), TJsonWriterFactory<>::Create(&Text));
	FFileHelper::SaveStringToFile(Text, *Files.Path);
	UFTCareerSubsystem* Migrated = Files.Career();
	TestTrue(TEXT("Valid old save migrates in memory"), Migrated->HasContinue());
	TestEqual(TEXT("Legacy pending level becomes preparation checkpoint"), Migrated->C.ResumeLevel, 1);
	TestEqual(TEXT("Migration does not write during Load"), Files.Read(), Text);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFTCareerRecoveryTest, "FT.Save.BackupAndUnsupportedVersion", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)
bool FFTCareerRecoveryTest::RunTest(const FString&)
{
	FFTSaveTestScope Files;
	UFTCareerSubsystem* Career = Files.Career();
	TestTrue(TEXT("Initial checkpoint"), Career->Reset());
	Career->C.Cash = 100; TestTrue(TEXT("Updated progress"), Career->Save(TEXT("Fixture")));
	const FString Backup = Files.Read(Files.Path + TEXT(".bak"));
	FFileHelper::SaveStringToFile(TEXT("{interrupted"), *Files.Path);
	UFTCareerSubsystem* Recovered = Files.Career();
	TestTrue(TEXT("Valid backup is offered"), Recovered->bRecoveredFromBackup && Recovered->HasContinue());
	TestEqual(TEXT("Recovery returns prior durable currency"), Recovered->C.Cash, int64(0));
	TestEqual(TEXT("Load never renames or overwrites damaged primary"), Files.Read(), FString(TEXT("{interrupted")));
	TestTrue(TEXT("Explicit later save repairs primary"), Recovered->Save(TEXT("Recovery")));
	TestEqual(TEXT("Known-good backup survives recovery save"), Files.Read(Files.Path + TEXT(".bak")), Backup);
	TSharedPtr<FJsonObject> Unsupported;
	FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Files.Read()), Unsupported);
	Unsupported->SetNumberField(TEXT("v"), 999);
	FString Text; FJsonSerializer::Serialize(Unsupported.ToSharedRef(), TJsonWriterFactory<>::Create(&Text));
	FFileHelper::SaveStringToFile(Text, *Files.Path);
	IFileManager::Get().Delete(*(Files.Path + TEXT(".bak")));
	UFTCareerSubsystem* Refused = Files.Career();
	TestTrue(TEXT("Unsupported version is visibly refused"), Refused->bLoadError);
	TestFalse(TEXT("Unsupported version has no Continue"), Refused->HasContinue());
	TestFalse(TEXT("Autosave cannot overwrite incompatible data"), Refused->Save(TEXT("WindowClose")));
	TestEqual(TEXT("Incompatible primary preserved byte-for-byte"), Files.Read(), Text);
	FFileHelper::SaveStringToFile(TEXT("{}"), *Files.Path);
	UFTCareerSubsystem* Damaged = Files.Career();
	TestTrue(TEXT("Parseable but incomplete JSON is damaged, not fresh progress"), Damaged->bLoadError);
	TestTrue(TEXT("Explicit New Game replaces unrecoverable progress"), Damaged->Reset());
	FFileHelper::SaveStringToFile(TEXT("{second interruption"), *Files.Path);
	TestTrue(TEXT("Replacement without old backup creates a valid new backup"), Files.Career()->HasContinue());
	TestTrue(TEXT("Night introduction is supported"), ValidCheckpoint(TEXT("night"), 0, -1));
	TestFalse(TEXT("Damaged out-of-range night stage is rejected"), ValidCheckpoint(TEXT("night"), 0, 99));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFTCareerAtomicFailureTest, "FT.Save.AtomicFailureAndInterruptedWrite", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)
bool FFTCareerAtomicFailureTest::RunTest(const FString&)
{
	FFTSaveTestScope Files;
	UFTCareerSubsystem* Career = Files.Career();
	TestTrue(TEXT("Create durable save"), Career->Reset());
	const FString Before = Files.Read();
	IPlatformFile& Platform = FPlatformFileManager::Get().GetPlatformFile();
	TestTrue(TEXT("Set actual filesystem write protection"), Platform.SetReadOnly(*Files.Path, true));
	Career->C.Cash = 111;
	TestFalse(TEXT("Read-only primary replacement fails"), Career->Save(TEXT("ReadonlyTest")));
	TestTrue(TEXT("Failure is exposed to UI"), Career->bSaveError && !Career->SaveError.IsEmpty());
	TestEqual(TEXT("Failed replacement preserves valid primary"), Files.Read(), Before);
	Platform.SetReadOnly(*Files.Path, false);
	TestNotNull(TEXT("Non-shipping fault hook is registered"), Files.Fault);
	if (!Files.Fault) return false;
	Files.Fault->Set(2, ECVF_SetByConsole);
	TestFalse(TEXT("Simulated interruption leaves a verified temp only"), Career->Save(TEXT("InterruptTest")));
	TestEqual(TEXT("Interrupted write keeps primary"), Files.Read(), Before);
	UFTCareerSubsystem* Reloaded = Files.Career();
	TestEqual(TEXT("Reload ignores orphan temporary file"), Reloaded->C.Cash, int64(0));
	Files.Fault->Set(0, ECVF_SetByConsole);
	TestTrue(TEXT("Retry stores staged progress"), Career->Save(TEXT("Retry")));
	TestEqual(TEXT("Retry is durable after reload"), Files.Career()->C.Cash, int64(111));
	const FString Durable = Files.Read();
	Files.Fault->Set(1, ECVF_SetByConsole);
	TestFalse(TEXT("Failed New Game does not replace career"), Career->Reset());
	TestEqual(TEXT("Failed New Game restores in-memory progress"), Career->C.Cash, int64(111));
	TestEqual(TEXT("Failed New Game preserves file"), Files.Read(), Durable);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFTCareerRewardTransactionTest, "FT.Save.IdempotentRewardsAndPurchases", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)
bool FFTCareerRewardTransactionTest::RunTest(const FString&)
{
	FFTSaveTestScope Files;
	UFTCareerSubsystem* Career = Files.Career();
	TestTrue(TEXT("New campaign"), Career->Reset());
	TestTrue(TEXT("Race checkpoint creates an attempt ID"), Career->SetCheckpoint(TEXT("race"), 0));
	const FString RunId = Career->C.RunId;
	TestTrue(TEXT("Continuing same race checkpoint"), Career->SetCheckpoint(TEXT("race"), 0));
	TestEqual(TEXT("Continuing keeps the reward ledger identity"), Career->C.RunId, RunId);
	FFTLevelCommit Result;
	Result.R.Level = 0; Result.R.Wrecked = 1; Result.R.ScriptTotal = 1; Result.R.Outcome = EFTOutcome::Wrap;
	Result.XP = 100; Result.NextKind = TEXT("day"); Result.NextLevel = 0; Result.bNextWrap = true;
	TestTrue(TEXT("Rewards and following story checkpoint save together"), Career->Commit(Result).bSaved);
	const int64 Cash = Career->C.Cash, XP = Career->C.XP;
	TestTrue(TEXT("Duplicate commit identified"), Career->Commit(Result).bDuplicate);
	TestEqual(TEXT("Duplicate does not pay again"), Career->C.Cash, Cash);
	TestEqual(TEXT("Duplicate does not grant XP again"), Career->C.XP, XP);
	UFTCareerSubsystem* Reloaded = Files.Career();
	TestEqual(TEXT("Reload resumes post-reward story"), Reloaded->C.ResumeKind, FString(TEXT("day")));
	TestTrue(TEXT("Duplicate is blocked after full reload"), Reloaded->Commit(Result).bDuplicate);
	TestEqual(TEXT("Reloaded ledger prevents a second payment"), Reloaded->C.Cash, Cash);
	TestTrue(TEXT("New race attempt"), Reloaded->SetCheckpoint(TEXT("race"), 0));
	TestNotEqual(TEXT("Committed attempt gets a new ID on replay"), Reloaded->C.RunId, RunId);
	TestNotNull(TEXT("Fault hook"), Files.Fault); if (!Files.Fault) return false;
	Files.Fault->Set(1, ECVF_SetByConsole);
	TestFalse(TEXT("Failed reward save is not reported as stored"), Reloaded->Commit(Result).bSaved);
	const int64 StagedCash = Reloaded->C.Cash;
	TestTrue(TEXT("Retrying commit cannot repeat staged rewards"), Reloaded->Commit(Result).bDuplicate);
	TestEqual(TEXT("Failed retry does not duplicate reward"), Reloaded->C.Cash, StagedCash);
	Files.Fault->Set(0, ECVF_SetByConsole);
	TestTrue(TEXT("Retry Save commits staged ledger and checkpoint"), Reloaded->Save(TEXT("Retry")));
	TestEqual(TEXT("Retried reward survives restart"), Files.Career()->C.Cash, StagedCash);
	Reloaded->C.Cash = 100000; Reloaded->C.Film = 0; TestTrue(TEXT("Purchase fixture"), Reloaded->Save(TEXT("Fixture")));
	Files.Fault->Set(1, ECVF_SetByConsole);
	TestFalse(TEXT("Purchase reports write failure"), Reloaded->BuyFilm());
	TestEqual(TEXT("Unconfirmed purchase does not charge cash"), Reloaded->C.Cash, int64(100000));
	TestEqual(TEXT("Unconfirmed purchase does not duplicate inventory"), Reloaded->C.Film, 0);
	Files.Fault->Set(0, ECVF_SetByConsole);
	const FFTProgressData& Data = FFTProgressData::Get();
	const FName Blue(TEXT("p-bayshore-blue")), Skill(TEXT("WHL1"));
	const FFTCosmeticRow* Paint = Data.Cosmetic(Blue);
	const FFTSkillNodeRow* Node = Data.Node(Skill);
	TestNotNull(TEXT("Purchase fixture uses a shipped paint"), Paint);
	TestNotNull(TEXT("Skill fixture uses a shipped node"), Node);
	if (!Paint || !Node) return false;
	Reloaded->C.Cash = 1000000;
	Reloaded->GiveXP(10000, TEXT("PurchaseFixture")); // Earn real levels/SP, so reload has a consistent driver state.
	TestTrue(TEXT("Fund the purchase fixture"), Reloaded->Save(TEXT("Fixture")));
	const int64 PurchaseCash = Reloaded->C.Cash;
	const int32 SkillPoints = Reloaded->C.SP, SpentPoints = Reloaded->C.SPSpent;
	TestTrue(TEXT("Film purchase saves successfully"), Reloaded->BuyFilm());
	TestTrue(TEXT("Paint purchase saves successfully"), Reloaded->BuyCosmetic(Blue));
	TestTrue(TEXT("Equipping purchased paint saves successfully"), Reloaded->Equip(TEXT("paint"), Blue));
	TestTrue(TEXT("Sequel purchase saves successfully"), Reloaded->BuySequel());
	TestTrue(TEXT("Sequel equipment toggle saves successfully"), Reloaded->ToggleSequel());
	TestTrue(TEXT("Skill allocation saves pending choice"), Reloaded->BuySkill(Skill));
	const int64 ExpectedCash = PurchaseCash - Data.FilmPrice - Paint->Price - Data.SequelPrice;
	UFTCareerSubsystem* Purchased = Files.Career();
	TestEqual(TEXT("Confirmed purchases retain their exact total charge after reload"), Purchased->C.Cash, ExpectedCash);
	TestEqual(TEXT("Purchased film survives reload exactly once"), Purchased->C.Film, 1);
	TestTrue(TEXT("Purchased cosmetic ownership survives reload"), Purchased->Owns(Blue));
	TestEqual(TEXT("Equipped cosmetic survives reload"), Purchased->Worn(TEXT("paint")), Blue);
	TestTrue(TEXT("Purchased sequel ownership survives reload"), Purchased->C.bSequel);
	TestFalse(TEXT("Confirmed sequel toggle survives reload"), Purchased->C.bSequelOn);
	TestTrue(TEXT("Pending skill survives reload"), Purchased->C.PendingSkills.Contains(Skill));
	TestFalse(TEXT("Pending skill is not applied before Ready"), Purchased->C.Skills.Contains(Skill));
	TestEqual(TEXT("Pending allocation reserves its skill point once"), Purchased->SPLeft(), SkillPoints - Node->SP);
	TestTrue(TEXT("Ready durably confirms the skill allocation"), Purchased->Ready());
	UFTCareerSubsystem* Equipped = Files.Career();
	TestTrue(TEXT("Confirmed skill survives reload"), Equipped->C.Skills.Contains(Skill));
	TestFalse(TEXT("Confirmed skill is no longer pending"), Equipped->C.PendingSkills.Contains(Skill));
	TestEqual(TEXT("Ready charges skill points once"), Equipped->C.SP, SkillPoints - Node->SP);
	TestEqual(TEXT("Ready records the exact spent points"), Equipped->C.SPSpent, SpentPoints + Node->SP);
	TestFalse(TEXT("Already-owned paint cannot be purchased twice"), Equipped->BuyCosmetic(Blue));
	TestEqual(TEXT("Refused duplicate purchase does not charge again"), Equipped->C.Cash, ExpectedCash);
	const FString BeforeEquipFailure = Files.Read();
	Files.Fault->Set(1, ECVF_SetByConsole);
	TestFalse(TEXT("Equipment change reports write failure"), Equipped->Equip(TEXT("paint"), TEXT("p-kestrel-red")));
	TestEqual(TEXT("Failed equipment change restores the previously equipped paint"), Equipped->Worn(TEXT("paint")), Blue);
	TestFalse(TEXT("Sequel toggle reports write failure"), Equipped->ToggleSequel());
	TestFalse(TEXT("Failed sequel toggle restores the previous mode"), Equipped->C.bSequelOn);
	TestEqual(TEXT("Failed equipment writes preserve the durable file"), Files.Read(), BeforeEquipFailure);
	Files.Fault->Set(0, ECVF_SetByConsole);
	TestTrue(TEXT("Retry after rolled-back equipment failure clears the error"), Equipped->Save(TEXT("Retry")));
	UFTCareerSubsystem* Stable = Files.Career();
	TestEqual(TEXT("Equipment failure and retry do not lose confirmed purchases"), Stable->C.Cash, ExpectedCash);
	TestEqual(TEXT("Original equipment remains durable after failure and retry"), Stable->Worn(TEXT("paint")), Blue);
	TestTrue(TEXT("Confirmed skill remains durable after failure and retry"), Stable->C.Skills.Contains(Skill));
	return true;
}
#endif
