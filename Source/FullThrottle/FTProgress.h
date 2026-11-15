// FULL THROTTLE 2.6: the progression of design v6 (spec pack "full-throttle-spec", 14 Nov 2026).
// Driver levels and XP, the 30-node skill tree, the cash garage, AP/HE ammo ranks, the asset library, extras
// (greenlights, film, SEQUEL, respec), the paycheck and the career save. Data: Content/Data/skills.json
// (format full-throttle.skills/2, written by Tools/spec_to_skills.py from the spec's tables). The career is the
// JSON contract of spec 09, saved as career.json beside settings.json (WISPOS_APP_DATA); the test slot is
// career_pie.json. UFTCareerSubsystem is the only writer.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FTCore.h"
#include "FTProgress.generated.h"

// ------------------------------------------------------------------------------------------------ data
struct FFTFeatSpec
{
	FName Type;               // wrecks, trucks, snakeEyes, notes, carnageAny/All, ratingAny/All/Level, driverLevel, stuntTypes,
	int32 N = 0, K = -1;      // feat, featsAll, leftBehind, garage, runDownAll, wrapped, wraps, underParAny/All, cleanWrap,
	FName Line;               // armorWrap, bossesAll
	TArray<FString> Names;
	FString Text;
	bool IsSet() const { return !Type.IsNone(); }
};

struct FFTTierRow { int32 Tier = 1, SP = 1, Level = 1; bool bGreenlight = false; };
struct FFTDriverLevelRow { int32 Level = 1; int64 TotalXP = 0, XPToNext = 0; int32 SP = 0; FString Unlock; };
struct FFTNoteRow { FString Id; int32 Level = 1; FString Text; };
struct FFTBranchRow { FString Code, Name, About; };
struct FFTSkillNodeRow { FName Id; FString Branch, Name, Desc; int32 Tier = 1, SP = 1, Level = 1; bool bGreenlight = false; };
struct FFTRankRow { FString Name; int32 Price = 0, Level = 1; FFTFeatSpec Feat; };
struct FFTGarageLine
{
	FName Key; FString Name, Row;
	FFTRankRow Ranks[3];
	TMap<FName, TArray<float>> Values;   // field -> value at rank 0..3
	float Value(FName Field, int32 Rank, float Def = 0.f) const;
};
struct FFTCosmeticRow
{
	FName Id, Slot; FString Name, Shop, Group, How, Look;
	int32 Tier = 0, Price = 0, Level = 1, Wave = 1;
	FFTFeatSpec Feat;
	bool bSwatch = false; FLinearColor Swatch = FLinearColor::White;   // sRGB 0..1
	bool bReady = false;      // this build can show it (wave 1 and the code-made SFX); the rest says COMING SOON
	FString NotReadyWhy;
};
struct FFTLevelRow
{
	FString Id, Title;
	int32 Scenes = 0, Vehicles = 0, Henchmen = 0, Par = 300, Fee = 5000, KillFee = 2500, WrapXP = 250;
	int32 Bars[3] = { 0, 0, 0 };
	TArray<FString> Notes;
	bool bFromSpec = false;   // L1-L3 come from the spec; levels 4-8 (2.5) take the closest stated rule
};
struct FFTBossRow { FString Name; int32 Armor = 120; float Spot = 128.f, Escape = 70.f, Pace = 520.f, Flee = 720.f; bool bFromSpec = false; };
struct FFTKindRow { FString Name; int32 Armor = 1, Class = -1, Points = 0, XP = 0; float Pace = 0.f, HeightPx = 0.f; bool bKeepsUp = false; };

struct FFTProgressData
{
	bool bLoaded = false;
	FString Error;
	TMap<FName, float> Tuning;
	TArray<float> RatingMult;
	int32 GreenlightPrice = 75000, FilmPrice = 10000, FilmMax = 2, SequelPrice = 150000, RespecPrice = 25000;
	TArray<FFTTierRow> Tiers;
	TArray<FFTDriverLevelRow> DriverLevels;
	TArray<FFTNoteRow> Notes;
	TArray<FFTBranchRow> Branches;
	TArray<FFTSkillNodeRow> Nodes;
	TArray<FFTGarageLine> Garage;
	TArray<FFTCosmeticRow> Cosmetics;
	TArray<FFTLevelRow> Levels;   // one per game level (8)
	TArray<FFTBossRow> Bosses;    // one per game level
	TMap<EFTKind, FFTKindRow> Kinds;

	static const FFTProgressData& Get();
	float T(const TCHAR* Key, float Def = 0.f) const { const float* V = Tuning.Find(Key); return V ? *V : Def; }
	const FFTSkillNodeRow* Node(FName Id) const;
	const FFTGarageLine* Line(FName Key) const;
	const FFTCosmeticRow* Cosmetic(FName Id) const;
	int32 LevelForXP(int64 XP) const;
	int64 TotalXPFor(int32 Level) const;
	int32 SPFor(int32 Level) const;      // SP paid on reaching this level
	const FFTKindRow* Kind(EFTKind K) const { return Kinds.Find(K); }
	/** The 24 stunt types (01 §5) plus the 2.5 set-piece stunts; "x2" stripped. */
	static FString StuntType(const FString& Name);
};

// ------------------------------------------------------------------------------------------------ career (09)
struct FFTCareerStats
{
	int32 Runs = 0, Wraps = 0, Wrecks = 0, LeftBehind = 0, Civilians = 0, Trucks = 0, SnakeEyes = 0;
	TArray<float> BestCarnage, BestTime;
	TArray<bool> Wrapped, RunDown;
	TArray<int32> BestRating, BossWrecks;
	bool bCleanWrap = false, bArmorWrap = false;
};

struct FFTCareer
{
	int32 V = 2;
	int32 Level = 1;
	int64 XP = 0;
	int32 SP = 0, SPSpent = 0;
	int64 Cash = 0, CashEarned = 0;
	TArray<FName> Skills;
	TArray<FName> PendingSkills;
	int32 PendingLevel = -1;     // 0-based level PRE-PRODUCTION leads to; -1 = none. -2 = the title.
	bool bPendingFromTitle = false;
	TMap<FName, int32> Garage;
	TArray<FString> Greenlit;
	int32 Film = 0;
	bool bSequel = false, bSequelOn = false;
	TArray<FName> Own;
	TMap<FName, FName> Wear;     // slot -> id (None = nothing)
	int32 Respecs = 0;
	TArray<FString> Feats;
	TMap<FString, bool> Notes;
	TArray<bool> Legendary, Bosses;
	FFTCareerStats Stats;
	bool bTipSeen = false;
	FString Updated;
	FString ResumeKind = TEXT("none");
	int32 ResumeLevel = 0, ResumeStage = 0;
	bool bResumeWrap = false;
	FString RunId, LastCommitId;
};

// ------------------------------------------------------------------------------------------------ paycheck (03)
UENUM()
enum class EFTOutcome : uint8 { Wrap, GotAway, OutOfFilm };

struct FFTLevelResult
{
	int32 Level = 0;             // 0-based
	EFTOutcome Outcome = EFTOutcome::Wrap;
	double D = 0, S = 0, N = 0, BossAddPoints = 0;
	int32 Wrecked = 0, ScriptTotal = 1, StuntTypes = 0, Rating = 0, Retakes = 0, ArmorPct = 0, Civilians = 0, MultiKills = 0, Barrels = 0, NotesCompleted = 0;
	double LevelTime = 0;        // game seconds; 0 = not recorded
	bool bBossWrecked = false;
};
struct FFTCareerSnapshot { bool bScorched = false, bCrowd = false, bBigFinish = false, bAhead = false, bSequel = false; };
struct FFTPayLine { FString Key, Label; double Amount = 0; double Mult = 0; bool bMult = false; FString Note; };
struct FFTPaycheck
{
	TArray<FFTPayLine> Lines;
	int64 Total = 0;
	double Carnage = 1, Style = 1, Skill = 1, Exact = 0;
	int32 Shown = 0;
	bool bTotalCarnage = false;
};

namespace FTPay
{
	/** One pure function over a level result (03 §1-§9). Fees and par from the level row, constants from the tuning. */
	FFTPaycheck Compute(const FFTLevelResult& R, const FFTCareerSnapshot& C, const FFTProgressData& D);
	/** The worked examples of 03 §7 (A, B, C) as level results. */
	FFTLevelResult Example(TCHAR Which, FFTCareerSnapshot& OutC);
}

/** What a level hands the career at the paycheck (built by the GameMode from its level run). */
struct FFTLevelCommit
{
	FFTLevelResult R;
	int32 XP = 0;                      // level XP counted in the race (events, first stunt types, first-take scenes)
	TArray<FString> NewFeats;          // stunt types landed for the first time ever
	TArray<FString> NotesDone;         // Director's Notes completed this level (ids)
	int32 LeftBehind = 0, Trucks = 0, SnakeEyes = 0;
	bool bRunDown = false;
	double ExactCarnage = 0;
	int32 BestScore = 0;
	FString NextKind;
	int32 NextLevel = -1, NextStage = 0;
	bool bNextWrap = false;
};

struct FFTCommitOut
{
	FFTPaycheck Pay;
	TArray<FFTPayLine> XPLines;        // level XP, wrap, rating, first boss, notes
	int64 XPGained = 0, XPBefore = 0;
	int32 LevelBefore = 1;
	TArray<TPair<int32, int32>> LevelUps; // level, sp
	TArray<FString> Unlocks;
	int32 LegendarySP = 0;
	bool bSaved = false, bDuplicate = false;
};

// ------------------------------------------------------------------------------------------------ tile states (PRE-PRODUCTION)
enum class EFTTile : uint8 { Locked, Available, Pending, Owned, Equipped, Soon };

UCLASS()
class FULLTHROTTLE_API UFTCareerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	static UFTCareerSubsystem* Get(const UObject* Ctx);

	FFTCareer C;
	FString Slot = TEXT("career");
	bool bLoadError = false; FString LoadError;
	bool bSaveError = false, bSaving = false, bRecoveredFromBackup = false;
	FString SaveError;
	double LastSaveTime = 0;
#if WITH_DEV_AUTOMATION_TESTS
	FString TestSavePath;   // automation never touches a player's real slot
#endif

	FString FilePath() const;
	void UseTestSlot();
	void Load();
	bool Save(const TCHAR* Reason);
	bool Reset();          // explicit New Game; a failed replacement restores the previous career
	bool HasContinue() const;
	FString ContinueLabel() const;
	bool SetCheckpoint(const FString& Kind, int32 Level, int32 Stage = 0, bool bWrap = false, const TCHAR* Reason = TEXT("Checkpoint"));
	void Drop();           // remove the career file of this slot
	FString Dump() const;
	void LogCareer() const;
	void SetRaw(const FString& Field, const FString& Value);

	// XP, levels, SP
	int32 PendingCost() const;
	int32 SPLeft() const { return C.SP - PendingCost(); }
	int32 SPEarned() const { return C.SP + C.SPSpent; }
	/** Credit XP through the level-up path (one function); logs FT LEVELUP per level and FT CAREER. */
	TArray<TPair<int32, int32>> GiveXP(int64 N, const TCHAR* Source);
	void SetXP(int64 XP);

	// skills (02)
	bool HasSkill(FName Id) const { return C.Skills.Contains(Id); }
	EFTTile SkillState(FName Id, FString* Needs = nullptr) const;
	bool BuySkill(FName Id);
	bool UndoSkill(FName Id);    // NAME_None: the last pending one
	bool Ready();                // pending -> owned, save; false leaves allocations unchanged
	bool IsGreenlit(const FString& Code) const { return C.Greenlit.Contains(Code); }
	EFTTile GreenlightState(const FString& Code, FString* Needs = nullptr) const;
	bool BuyGreenlight(const FString& Code);
	int32 RespecPrice() const;
	bool Respec();

	// garage (04)
	int32 Rank(FName Key) const { const int32* R = C.Garage.Find(Key); return R ? *R : 0; }
	EFTTile GarageState(FName Key, int32 Rank, FString* Needs = nullptr) const;
	bool BuyGarage(FName Key, int32 Rank);

	// asset library (07)
	bool Owns(FName Id) const { return C.Own.Contains(Id); }
	FName Worn(FName SlotName) const { const FName* W = C.Wear.Find(SlotName); return W ? *W : NAME_None; }
	EFTTile CosmeticState(FName Id, FString* Needs = nullptr) const;
	bool BuyCosmetic(FName Id);
	bool Equip(FName SlotName, FName Id);

	// extras
	EFTTile FilmState(FString* Needs = nullptr) const;
	bool BuyFilm();
	EFTTile SequelState(FString* Needs = nullptr) const;
	bool BuySequel();
	bool ToggleSequel();

	// feats (04 §3, 07 §6) shared by the garage and the shop
	bool IsFeatMet(const FFTFeatSpec& F, FString* Needs = nullptr) const;
	bool HasFeat(const FString& Name) const { return C.Feats.Contains(Name); }

	/** The paycheck commit: pay, XP, stats, feats, notes, legendary SP, unlock lines; saves (reason Paycheck). */
	FFTCommitOut Commit(const FFTLevelCommit& In);
	FFTCareerSnapshot Snapshot() const;

	/** Everything locked now that a commit may open (for the one-line unlock announcements). */
	TSet<FString> LockedSet() const;

private:
	FFTCareer PersistedCareer;
	bool bHasPersistedCareer = false, bPreserveBackup = false;
	FString LastSavedText;
	FFTCommitOut LastCommitOut;
	bool SaveMutation(const FFTCareer& Before, const TCHAR* Reason);
	void Fix();            // the load integrity rules (09 §4)
	void Fresh();
	bool Spend(int64 Price, const TCHAR* What);
	void SizeArrays();
};
