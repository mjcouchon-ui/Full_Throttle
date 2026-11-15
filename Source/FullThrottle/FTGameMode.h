// The race: a port of the 1.7.2 engine (engine.js + director.js) onto the 3D track and the Chaos car.
// Owns the phases (title, slate, play, wreck, retake, wrap, highlight reel, dailies), every foe and
// their attacks, scoring and stunts, the director's megaphone, slow motion and the chase camera.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Camera/CameraTypes.h"
#include "FTCore.h"
#include "FTProgress.h"
#include "FTGameMode.generated.h"

class AFTTrack;
class AFTPlayerCar;
class AFTEntityActor;
class AFTHazard;
class AFTPlayerController;
class UFTGameInstance;
class UAudioComponent;
class UPointLightComponent;
class USoundBase;

UENUM()
enum class EFTPhase : uint8 { Title, Card, Play, Wreck, Wrap, Reel, Rating, Paycheck, PreProduction };

UENUM()
enum class EFTOverlay : uint8 { None, Pause, Controls, Options };

// 2.6 PRE-PRODUCTION tabs (spec 10 §4)
enum class EFTTab : uint8 { Skills, Garage, Custom, Sfx, Extras, Count };

struct FFTEnt
{
	EFTKind Kind = EFTKind::None;
	int32 Id = 0, Variant = 0;
	float D = 0.f, X = 0.f, H = 0.f;    // track space (cm)
	float Sp = 0.f;                       // speed along the road (cm/s, absolute)
	float Vx = 0.f, Vh = 0.f;
	float W = 100.f, L = 100.f;          // footprint (cm across, cm along)
	float Hp = 1.f, MaxHp = 1.f;
	float T = 0.f;                        // age
	float Ta = 0.f;                       // target distance ahead of the player (cm)
	float Tx = 0.f;                       // target x
	float T1 = 0.f, T2 = 0.f, T3 = 0.f;   // timers
	int32 State = 0, Side = 1, Count = 0, Phase = 0, Step = 0;
	int32 Owner = 0;                      // bullets/missiles: 0 player, 1 foe
	float Heading = 0.f;                  // missiles: steering angle (rad, + right)
	float Dmg = 0.f;
	float PrevAhead = 0.f;
	bool bDead = false, bBig = false, bAir = false, bHit = false, bNear = false, bNoNear = false;
	// 2.6: script-spawned Syndicate (Carnage), a boss add (half pay and XP, outside Carnage), touched the player,
	// the slide down the screen (design px, 05 §2), the load a player round carries and the ids it went through
	bool bCarnage = false, bAdd = false, bTouched = false, bAutoSeen = false;
	float Slide = 0.f;
	int32 Load = 0;
	TArray<int32> Hits;
	float Glow = 0.f;
	TWeakObjectPtr<AFTEntityActor> Proxy;
};

struct FFTHazardState
{
	int32 Type = 0;     // 0 lane arc, 1 lock-on, 2 shock wave, 3 landing mark, 4 flame lane (2.5), 5 lobbed thing (2.5)
	float D = 0.f, X = 0.f, W = 0.f, Gap = 0.f;
	float Warn = 1.f, Live = 0.4f, T = 0.f;
	bool bHit = false, bDone = false;
	float Track = 0.f;  // lock-on: seconds it follows the car
	// 2.5: look (0 electric, 1 ice/snow, 2 fire, 3 water), a lane that sweeps (cm/s), and for a lobbed thing:
	// where it starts (track space, relative to the car) and how high it arcs, and which mesh flies
	int32 Style = 0;
	float Vx = 0.f;
	float FromD = 0.f, FromX = 0.f, FromH = 0.f, Arc = 600.f, Dmg = 14.f;
	int32 Thing = 0;    // 0 bomb, 1 snowball, 2 fish, 3 fireball, 4 lightning bolt
	float FxT = 0.f;
	int32 Rail = -1;    // 3.3 on the coaster: aimed at this rail (0 left, 1 centre, 2 right); hop off it to dodge
	bool bTesla = false; // 3.3 the Tesla truck's strike: an arc from the coil while it is live
	TWeakObjectPtr<AFTHazard> Actor;
};

/** Per-level counters the scene checkpoint copies (a retake restores them in one assignment). */
struct FFTLevelRun
{
	int32 XP = 0;
	int32 Wrecked = 0, LeftBehind = 0, Civilians = 0, Trucks = 0, SnakeEyes = 0, Barrels = 0, MultiKills = 0;
	int64 BossAddPts = 0;
	TArray<FString> Types;       // distinct stunt types landed this level (Style)
	TArray<FString> NewFeats;    // types landed for the first time ever (career.feats gets them at the paycheck)
	int32 Rounds[2] = { 0, 0 };  // AP, HE
	int32 Shock = 0, Slams = 0;
	bool bRoof = false, bPipe3 = false, bGrav2 = false;
	uint8 AirWash = 0;           // L3-air: bit 0 The Dry Wash, bit 1 Last Chance Gas
	int32 FirstTakeScenes = 0;
	bool bHeMulti = false;
};

/** The KESTREL's numbers for this level: garage ranks and skills, filled at ACTION! (04, 02, 05). */
struct FFTCarStats
{
	float MaxArmor = 100.f, FireInterval = 0.085f;
	int32 MissileStart = 6, MissileCap = 9, Reels = 3;
	float TurboTank = 100.f, TurboRefill = 13.f, FlameLen = 10.f;
	float RamDmg = 6.f, RamDmgBig = 4.f, BumpDmg = 1.f;
	float DriftChargeMul = 1.f, BendPullMul = 1.f;
	float SpeedUpAdd = 90.f, Accel = 420.f, AccelHot = 900.f, PunchAdd = 90.f, PunchCap = 700.f, PunchOff = 1.f;
	int32 ApRank = 0, HeRank = 0, ApRounds = 0, HeRounds = 0;
	float HeRadius = 0.f, HeDamage = 0.f;
	TSet<FName> Skills;
	bool Has(const TCHAR* Id) const { return Skills.Contains(FName(Id)); }
};

/** A PRE-PRODUCTION tile (built each frame from the data and the career; the screen stores no state). */
struct FFTTileView
{
	FString Id;          // node id, "garage:<key>:<rank>", cosmetic id, "film", "greenlight-DEM", "sequel", "respec"
	int32 Col = 0, Row = 0;
	FString Title, Sub, Desc, Group, Price;
	EFTTile State = EFTTile::Locked;
	FString Needs;
	bool bCash = false, bSwatch = false;
	FLinearColor Color = FLinearColor::White;
	int64 Cost = 0;
	FName Slot;
};

struct FFTPopup { FString Text; FLinearColor Col; float T = 0.f; float Big = 1.f; };

/** One recorded frame for the highlight reel. */
struct FFTSnap
{
	float Time = 0.f;
	FTransform Car;
	float CarSteer = 0.f;
	TArray<TPair<int32, FTransform>> Ents;  // proxy id -> transform
	TArray<TPair<int32, int32>> EntMesh;    // proxy id -> kind/variant code (first appearance)
};

struct FFTClip
{
	float T0 = 0.f, T1 = 0.f, Focus = 0.f;
	float Score = 0.f;
	FString Label;
	int32 Scene = 0;
	int32 Camera = 0;
	bool bBoss = false;
	FVector FocusPos = FVector::ZeroVector;
};

UCLASS()
class FULLTHROTTLE_API AFTGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AFTGameMode();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	virtual void Tick(float Dt) override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	bool ComputeCamera(FMinimalViewInfo& POV, float Dt);
	/** Render the game camera to Saved/Shots/<Name>.png (tests; works with the window in the background). */
	void Snap(const FString& Name);

	// ---- state the HUD reads
	EFTPhase Phase = EFTPhase::Title;
	EFTOverlay Overlay = EFTOverlay::None;
	int32 Level = 0;
	int32 TitleSel = 0;
	float PhaseT = 0.f;
	float Hp = 100.f, Turbo = 100.f, DriftCharge = 0.f;
	int32 Missiles = 6, DriftLevel = 0;
	bool bTurboOn = false, bTurboLock = false, bDrifting = false, bHot = false;
	float BoostT = 0.f;
	int64 Score = 0;
	int32 Combo = 1; float ComboT = 0.f;
	int32 Kills = 0, NearMisses = 0, StuntCount = 0, Collateral = 0;
	int64 ScoreDestruction = 0, ScoreNear = 0, ScoreStunts = 0, ScoreCollateral = 0, ArmorBonus = 0;
	int32 Retakes = 0, Take = 1, Scene = 0;
	float SceneSlateT = -10.f, ActionT = -10.f;
	FString Line; int32 LinePri = 0; float LineT = 0.f, LineHold = 0.f;
	FString ShoutWord; float ShoutT = -10.f;
	FString Caption; float CaptionT = -10.f;
	TArray<FFTPopup> Popups;
	bool bBossOn = false; float BossHpFrac = 1.f; FString BossLabel;
	FString CutTitle, CutSub; float CutT = -10.f;
	int32 RatingIdx = 0; FString RatingQuip; bool bEscaped = false, bOutOfFilm = false;
	float Fps = 0.f, FpsSmoothed = 60.f;
	float Progress = 0.f;
	float SpeedMph = 0.f;
	bool bShowKeys = true;
	int32 HitchCount = 0;
	float LastRumbleT = -10.f;
	int32 OptionsRow = 0, ControlsRow = 0, ControlsCol = 0; bool bControlsWaiting = false;
	int32 PauseRow = 0;
	EFTOverlay DrawnOverlay = EFTOverlay::None;
	TArray<FBox2D> OverlayRects;
	FString OverlayFeedback;
	float OverlayFeedbackT = -10.f;
	bool bOverlayFeedbackError = false;
	FBox2D ResultsContinueRect = FBox2D(ForceInit);
	FString ReelLabel, ReelCamName; int32 ReelShot = 0, ReelShots = 0; bool bReelSlow = false; float ReelTitleT = 0.f;
	// 2.3: the reel plays (and loops) until the dedicated CONTINUE button; the HUD writes its rect here for the mouse
	int32 ReelLoop = 0; float ReelLoopT = -10.f;
	static constexpr float ReelArmT = 0.8f;
	FBox2D ReelContinueRect = FBox2D(ForceInit);
	// 2.3: skill points earned at the end of the level (paid when the dailies open) and the garage between levels
	int32 PointsEarned = 0;
	TArray<TPair<FString, int32>> PointsLines;
	int32 GarageSel = 0;              // 0..N-1 upgrades, N = the START button
	float GarageFlashT = -10.f; int32 GarageFlashIdx = -1; bool bGarageDenied = false;
	TArray<FBox2D> GarageRects;       // the HUD's card rects, then the START button
	FVector2D GarageMouse = FVector2D(-1, -1);
	int32 MissileCap() const { return Stats.MissileCap; }
	// ---- 2.6 progression (spec v6)
	FFTCarStats Stats;
	FFTLevelRun Run, RunCk;
	float LevelClock = 0.f, ClockLogT = 0.f;
	int32 ScriptTotal = 1;
	float MaxHp() const { return Stats.MaxArmor; }
	float HpPct() const { return Stats.MaxArmor > 0.f ? Hp / Stats.MaxArmor * 100.f : 0.f; }
	int32 ReelsLeft() const { return FMath::Max(0, Stats.Reels - Retakes); }
	int32 AmmoLoad = 0;          // 0 STANDARD, 1 AP, 2 HE
	int32 AmmoMag(int32 L) const { return L == 1 ? Stats.ApRounds : (L == 2 ? Stats.HeRounds : 0); }
	bool bDraft = false;
	float BossClock = 0.f, BossRate = 1.f, BossEscapeT = 70.f, BossFightT = 0.f, LastChanceT = 0.f, BossOffset = 0.f;
	bool bLastChance = false, bRunDown = false;
	bool bSequelRun = false;
	FString TitleBanner; float TitleBannerT = -10.f;
	// 3.0 the title's attract mode: the game plays itself behind the title (Phase stays Play, the HUD draws the title),
	// cutting to the next level every AttractLen seconds behind a channel-change wipe
	bool bAttract = false;
	float TitleT = 0.f;              // time on the title (carried across attract cuts)
	float AttractT = 0.f;            // time in this shot
	float AttractOutT = -1.f;        // >= 0: the wipe out to the next level is running
	bool bAttractCutIn = false;      // this shot came in from a cut (the wipe opens)
	int32 AttractHeld = 0;           // frames drawn fully covered before the map load starts
	static constexpr float AttractLen = 11.f;
	static constexpr float AttractWipeOut = 0.6f;
	static constexpr float AttractWipeIn = 0.75f;
	int32 AttractNextLevel() const { return (Level + 1) % FTData::NumLevels(); }
	/** 0 = clear, 1 = the screen is covered by the channel-change wipe. */
	float AttractCover() const;
	float LastMenuInputT = -10.f;
	TArray<FBox2D> TitleRowRects;    // the HUD writes the level rows and the buttons here for the mouse
	TArray<FBox2D> TitleBtnRects;    // ROLL CAMERA, PRE-PRODUCTION, CONTROLS, OPTIONS, QUIT
	int32 TitleActionFocus = INDEX_NONE;
	float LastMenuSoundT = -10.f;
	FVector2D TitleMouse = FVector2D(-1, -1);
	// highlight reel (10 §2): hold to skip mid-reel, CONTINUE once it has played
	float ReelHoldT = 0.f; bool bReelDone = false;
	// THE DAILIES + PAYCHECK
	FFTCommitOut Pay; bool bPayValid = false; int32 PayNext = 0; // 0 next level, 1 same level, 2 title
	// PRE-PRODUCTION
	EFTTab Tab = EFTTab::Skills;
	int32 TabSel[(int32)EFTTab::Count] = { 0, 0, 0, 0, 0 };
	int32 TabScroll[(int32)EFTTab::Count] = { 0, 0, 0, 0, 0 };
	TArray<FFTTileView> Tiles;
	TArray<FBox2D> TileRects; FBox2D ReadyRect = FBox2D(ForceInit), BuyRect = FBox2D(ForceInit), UndoRect = FBox2D(ForceInit); TArray<FBox2D> TabRects;
	float BuyHoldT = 0.f; bool bBuyHoldDone = false;
	int32 PPNextLevel = 0; bool bPPFromTitle = false;
	FString PPMsg; float PPMsgT = -10.f; bool bPPMsgBad = false;
	FVector2D PPMouse = FVector2D(-1, -1);
	void BuildTiles();
	FString PPHeader() const;
	int32 SelIndex() const { return TabSel[(int32)Tab]; }
	// cosmetics in the race (07): looks only
	float CosT = 0.f;
	// test harness (spec 12 §9; development builds)
	struct FFTHold { bool bUp = false, bTurbo = false, bDown = false, bDrift = false, bFire = false; } Held;
	bool bHarnessInvuln = false;
	bool bHarnessFreeze = false;   // 2.9 FTFreeze: holds the world still (time dilation 0.0001) for same-frame A/B screenshots
	int32 AutoWreck = -1, AutoLeave = 0; float AutoWrapAt = -1.f; int32 AutoTurbo = 0; bool bAutoRun = false;
	void HarnessCmd(const FString& Cmd, const TArray<FString>& Args);
	void StartLevelAt(int32 L, int32 SceneIdx, bool bSequel);
	static FString KindName(EFTKind K);
	float RealClockNow() const { return RealClock; }
	FVector FinisherAt = FVector::ZeroVector; float FireworkT = 0.f;
	// wreck FX and boss finishers (07): HUD overlays
	FString KaBoom; float KaBoomT = -10.f; float FilmBurnT = -10.f; float FinisherT = -10.f; FName Finisher; float FlashT = -10.f;
	float Timecode = 0.f;
	int32 HenchOn[2] = { 0, 0 };

	AFTPlayerCar* GetCar() const { return Car; }
	AFTTrack* GetTrack() const { return Track; }
	TConstArrayView<FFTEnt> GetActiveEntities() const { return Ents; }
	UFTGameInstance* GI() const;
	const FFTLevelDef& Def() const { return FTData::Level(Level); }
	float PlayerD() const;
	int32 NumScenes() const { return Def().Scenes.Num(); }

protected:
	// flow
	void SetupLevel();
	void StartRace(bool bRetake);
	void BeginPlayPhase();
	void TickTitle(float RDt);
	void TitleStart();
	// 3.0 attract mode
	void EnterTitle(bool bKeepTitleT);
	bool AttractAllowed() const;
	void StartAttract();
	void TickAttract(float RDt);
	void CutAttract();
	void StopAttract();
	static void PickAttractShot(int32 L, int32& OutScene, int32& OutSpawn, float& OutD0);
	int32 AttractScene = 0, AttractSpawn = 0;
	bool bAttractPaused = false;
	void TickOverlay(float RDt);
	void MenuCue(const TCHAR* Name, float Pitch = 1.f);
	void TickPlay(float Dt, float RDt);
	void TickPlayer(float Dt);
	void TickEnts(float Dt);
	void TickBoss(FFTEnt& B, float Dt);
	// 2.5 bosses
	void TickBossFire(FFTEnt& B, float Dt, int32 Ph, float& Want);
	void TickBossClown(FFTEnt& B, float Dt, int32 Ph, float& Want);
	void TickBossYeti(FFTEnt& B, float Dt, int32 Ph, float& Want);
	void TickBossShark(FFTEnt& B, float Dt, int32 Ph, float& Want);
	void TickBossLimo(FFTEnt& B, float Dt, int32 Ph, float& Want);
	void BossPhaseLine(FFTEnt& B, int32 Ph, const TCHAR* L1, const TCHAR* C1, const TCHAR* L2, const TCHAR* C2);
	/** Something lobbed from (fromAhead, fromX, fromH) onto a warned mark on the road (car-relative, like the lock-ons). */
	FFTHazardState& Lob(int32 Thing, float FromAhead, float FromX, float FromH, float ToAhead, float ToX, float Flight, float Radius, float Arc, float Dmg);
	void FlameLane(float X, float Warn, float Live, float SweepV);
	void BulletFan(const FFTEnt& B, int32 N, float Step, int32 GapAt, float Spd, float AimX);
	void TickLightning(float Dt);
	int32 LaneOf(float X) const;
	void TickHazards(float Dt);
	void TickDirector(float RDt);
	void TickSlowmo(float RDt);
	void TickSpawns();
	void TickStunts(float Dt);
	void TickCivilians(float Dt);
	void TickBench(float RDt);
	void Wreck();
	void Wrap(bool bEscape);
	void FinishRating();
	/** The rating, the best take and the paycheck, worked out and SAVED, once per run. Called when the level is
	 *  won, before the highlight reel (quitting during the reel used to lose the whole level's pay). */
	void RateAndCommit();
	bool bRunCommitted = false;
	void EnterPaycheck();
	void TickPaycheck(float RDt);
	void EnterPreProduction(int32 NextLevel, bool bFromTitle);
	void TickPreProduction(float RDt);
	void LeavePreProduction();
	bool PPActivate(int32 Idx, bool bHoldDone);
	void PPSay(const FString& M, bool bBad);
	void CommitLevel();
	void ApplyStats();
	void ApplyCosmetics(float RDt);
	void TickBossClock(FFTEnt& B, float Dt, float& Want);
	void TickSlide(FFTEnt& E, float Dt);
	float SlideLeavePx(const FFTEnt& E) const;
	float SlideCm(const FFTEnt& E) const;
	void LeftBehindEnt(FFTEnt& E);
	void AddXP(int32 N, const TCHAR* Source);
	void LandType(const FString& Name);
	void CycleAmmo();
	void BulletHit(FFTEnt& B, FFTEnt& O);
	void HeBlast(const FFTEnt& Direct, float D, float X);
	void ChainBlast(const FFTEnt& E);
	void Checkpoint();
	void Restore();
	void GoToLevel(int32 L, bool bStart);

	// entities
	FFTEnt& Spawn(EFTKind K, float Ahead, float Xc, int32 Variant = 0);
	void SpawnGroup(const TArray<EFTKind>& Kinds);
	void Kill(FFTEnt& E, int32 Cause); // 0 gun, 1 missile, 2 ram, 3 other (not the player), 4 splash (missile splash, HE blast), 5 chain/barrel blast, 6 the wrap
	void HurtEnt(FFTEnt& E, float Dmg, int32 Cause);
	void HurtPlayer(float Dmg, const TCHAR* Line = nullptr, float PushX = 0.f);
	/** Look (3.1): what the blast is made of. 0 fire, 1 ice and snow, 2 water, 3 electricity. Looks only. */
	void Explode(const FVector& At, float Size, bool bPlayerWreck = false, int32 Look = 0);
	void WreckExtras(FName Fx, const FVector& At, float Size);
	void Release(FFTEnt& E);
	void ClearEnts();
	FTransform EntTransform(const FFTEnt& E) const;
	FTransform EntTransformRaw(const FFTEnt& E) const;
	/** 3.3: where a rail-aimed or road mark sits: (x, h over the frame) on the coaster's rail, else the road/half pipe. */
	float MarkH(float D, float X, int32 Rail, float* OutX = nullptr) const;
	void FireBullet(int32 InOwner, float D, float X, float H, float VD, float VX, int32 Variant = 0, int32 LoadIdx = 0);
	void FireMissile();
	void DropPickup(const FFTEnt& E, EFTKind K);
	FFTEnt* FindEnt(int32 Id);

	// scoring and voice
	/** Score; BasePts >= 0 also counts XP = BasePts / 10 (the points before the combo, 01 §1); bAdd halves it (boss adds). */
	void AddScore(int64 Pts, int32 Bucket, int64 BasePts = -1, const TCHAR* Source = nullptr, bool bAdd = false);
	void Stunt(const FString& Name, int32 Pts, const TCHAR* Say = nullptr, float ReelScore = 3.f, bool bType = true);
	void Popup(const FString& T, const FLinearColor& C, float Big = 1.f);
	void Say(const FName& Key, int32 Pri, float Cooldown = 0.f);
	void SayText(const FString& T, int32 Pri);
	void Shout(const FString& W);
	void Slowmo(float Dur, float Scale, float Cool = 0.f);
	void Freeze(float Dur);
	void Sfx(const TCHAR* Name, float Vol = 1.f, float Pitch = 1.f);
	void Duck(float Depth, float Hold);
	void Shake(float Amount);

	// reel
	void RecordSnap(float RDt);
	void ReelEvent(float Score, const FString& Label, const FVector& Where, bool bBoss = false);
	void StartReel();
	void TickReel(float RDt);
	void EndReel();

	UPROPERTY(Transient) TObjectPtr<AFTTrack> Track;
	UPROPERTY(Transient) TObjectPtr<AFTPlayerCar> Car;
	UPROPERTY(Transient) TArray<TObjectPtr<AFTEntityActor>> Pool;
	UPROPERTY(Transient) TArray<TObjectPtr<AFTHazard>> HazardPool;
	UPROPERTY(Transient) TObjectPtr<UAudioComponent> Music;
	UPROPERTY(Transient) TObjectPtr<UAudioComponent> Rotor;
	UPROPERTY(Transient) TArray<TObjectPtr<UPointLightComponent>> Flashes;
	UPROPERTY(Transient) TArray<TObjectPtr<UPointLightComponent>> RoadLights;
	UPROPERTY(Transient) TMap<FName, TObjectPtr<USoundBase>> Sounds;
	// 2.5: the lightning bolt in the volcano's sky, and the meshes things are lobbed as
	UPROPERTY(Transient) TObjectPtr<class UStaticMeshComponent> BoltComp;
	UPROPERTY(Transient) TArray<TObjectPtr<class UStaticMesh>> LobMeshes;
	float BoltT = 0.f, BoltNext = 3.f; int32 BoltFlash = -1;
	// 2.5 set-piece bookkeeping for the new stunts
	int32 HalfSection = -1, HalfSide = 0; int32 BldgSection = -1; bool bOnCoaster = false; int32 CoasterMode = -1; bool bHopAir = false;
	float CoasterT = 0.f;
	// 3.3 the coaster's flying boss: MR. CHUCKLES on propellers, bombing the rail you ride (an actor of its own,
	// not a foe in Ents: he can't be shot down up there, only dodged)
	UPROPERTY(Transient) TObjectPtr<AFTEntityActor> Flyer;
	UPROPERTY(Transient) TObjectPtr<UAudioComponent> PropAudio;
	int32 FlyState = 0;               // 0 none, 1 flying in, 2 bombing, 3 leaving
	float FlyT = 0.f, FlyAhead = 9000.f, FlyX = 0.f, FlyH = 3000.f, FlyBombT = 2.f, FlyClear = 0.f;
	int32 FlyDrops = 0;
	bool bFlyDone = false;
	void TickFlyer(float Dt);
	void HideFlyer();
	float CoasterChopT = 0.f;
	// 3.3 half pipe tricks and grinds
	float GrindT = 0.f, GrindPayT = 0.f;
	// 3.3 the Tesla truck's charge (0..1), for its glow and hum
	float TeslaWarnT = 0.f, TeslaWarnLen = 1.f;
	// 3.3 the replay camera: smoothed, and kept out of the scenery
	FVector ReelCamLoc = FVector::ZeroVector, ReelCamTgt = FVector::ZeroVector; int32 ReelCamClip = -1;
	FVector ReelSafeCam(const FVector& Focus, const FVector& Want) const;

	TArray<FFTEnt> Ents;
	TArray<FFTEnt> PendingEnts; // spawned this frame; joins Ents at the next entity tick
	TArray<FFTHazardState> Hazards;
	int32 NextId = 1;
	TMap<FName, int32> PoolCursor;
	TMap<FName, float> SayCool;
	float AmbientT = 0.f;

	// player
	float Steer = 0.f, Vx = 0.f;
	float DraftOutT = 10.f, StarHoldT = 0.f, StarT = 0.f, SpeedLogT = 0.f, HenchOnT[2] = { 0, 0 };
	bool bStarUsed = false, bLastTakeUsed = false, bOutrun = false, bWasDraft = false, bApNagged = false; int32 BoostLvl = 0, HeavyCount = 0; float BaseRoadFollow = 0.55f;
	int32 SpeedLogState = -1; float LastSpeedLog = -1.f;
	float TurboOffT = 10.f, DriftT = 0.f, DriftPressBuf = 0.f, Invuln = 0.f, FireT = 0.f, MissileCd = 0.f;
	float NoFireT = 0.f, NoMissileT = 10.f, NoTurboT = 0.f, NoNearT = 0.f, NoRailT = 0.f, NoDriftT = 0.f, LastFireT = 10.f;
	int32 DriftDir = 0; float DriftKickT = 0.f;
	float PrevHp = 100.f;
	int32 PartsShed = 0;
	float LastKillT = -10.f; int32 MultiKills = 0;
	float LastNearT = -10.f; int32 LastNearSide = 0;
	float HenchPunch[2] = { 0, 0 };
	float HenchHangT = 0.f;
	float WallRideT = 0.f; int32 WallRideSection = -1;
	int32 SetPieceRolls = 0;
	int32 SetPieceSection = -1; bool bSetPieceHot = false; float SetPieceTurns = 0.f; uint16 SetPieceFlag = 0;
	bool bWasAir = false, bGapAir = false;
	float RampAirT = 0.f;
	float CivT = 1.f;
	float Clock = 0.f, RealClock = 0.f;
	int32 SpawnScene = 0, SpawnIdx = 0;
	float BossT = 0.f; bool bBossSpawned = false, bBossDead = false, bEscapeWarned = false;
	int32 BossLineStep = 0;
	TArray<FString> AheadSaid;

	// checkpoint for retakes
	int64 CkScore = 0, CkDest = 0, CkNear = 0, CkStunts = 0, CkColl = 0; int32 CkKills = 0, CkNearN = 0, CkStuntN = 0, CkCollN = 0;
	float CkHp = 100.f; int32 CkMissiles = 6;
	bool bSpawningAdds = false;
	int32 HitLoad = 0;

	// time
	float SlowT = 0.f, SlowScale = 1.f, SlowCool = 0.f, FreezeT = 0.f, TimeScale = 1.f;
	float DuckT = 0.f, DuckDepth = 1.f, MusicVol = 0.5f; // 3.6 (owner: "the music is too quiet" on the driving levels): 0.34 -> 0.5, +3.4 dB

	// camera
	FVector CamLoc = FVector::ZeroVector, CamUp = FVector::UpVector, CamFwd = FVector::ForwardVector;
	float CamFov = 82.f, ShakeAmt = 0.f, CamRoll = 0.f;
	bool bCamInit = false;

	// bench
	TArray<float> BenchFrames;
	float BenchT = 0.f;
	// the real work per frame (the window is composed at the display's refresh, so fps alone hides the headroom)
	double BenchGameMs = 0.0, BenchRenderMs = 0.0, BenchGpuMs = 0.0;
	TArray<float> BenchGpuFrames;
	float AutoLane = 0.f;

	// reel
	TArray<FFTSnap> Snaps;
	TArray<FFTClip> Clips;
	float SnapT = 0.f;
	int32 ReelClip = 0; float ReelT = 0.f; float ReelPlayT = 0.f;
	TArray<FFTClip> ReelList;
	UPROPERTY(Transient) TArray<TObjectPtr<AFTEntityActor>> Ghosts;
	TMap<int32, int32> GhostOf;
	TMap<int32, int32> ProxyKind;
	int32 LastCam = -1;
	float SceneStartClock = 0.f;
};
