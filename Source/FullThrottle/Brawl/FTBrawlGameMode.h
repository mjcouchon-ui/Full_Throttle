// THE NIGHT: the side-scrolling beat-em-up, ported from the web prototype (prototypes/brawl/brawl.js, the
// FT Beat-Em-Up Test app). Nova walks into a venue, it turns into a brawl: waves lock the screen until each
// is down, then GO ->; a boss at the end (at half health he calls two more). Same rules, numbers and fixed
// 1/60 s steps as brawl.js; the web game's pixels map to centimetres (FTBrawl::PX) on a 3D floor:
// x = along the bar (+X), y = depth (the floor band, back wall at +Y), z = jump height.
// The game mode builds the venue from code at BeginPlay (no level content), so the map is empty.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FTBrawlFighter.h"
#include "FTBrawlGameMode.generated.h"

class ACameraActor;
class UInstancedStaticMeshComponent;

USTRUCT()
struct FFTBrawlKind
{
	GENERATED_BODY()
	float Hp = 20, Sp = 70, Dmg = 6, Wind = 0.55f, Reach = 62, S = 1.9f;
	FName Role = TEXT("rusher");   // rusher | grappler | thrower | heavy | boss
	FString Who = TEXT("Punk");    // the Meshy model, /Game/Brawl/Characters/<Who>
	float Height = 175.f;          // cm
};

USTRUCT()
struct FFTBrawlBody
{
	GENERATED_BODY()
	int32 Id = 0;
	bool bBoss = false, bPlayer = false;
	FFTBrawlKind K;
	float X = 0, Y = 0, Z = 0, Vz = 0, Vx = 0, Hp = 0, Max = 0, Dir = 1, T = 0, Cd = 0, Walk = 0, S = 2, Flash = 0;
	FName St = TEXT("walk");
	// player only
	float Inv = 0, ComboT = 0, JVx = 0, JVy = 0; int32 Combo = 0; TSet<int32> HitIds;
	// boss only
	float Charge = 3.5f; bool bCalled = false; float SlamCd = 2.5f;   // SlamCd = the signature move's cooldown (Earl's slam, Vinnie's whirlwind)
	float SigTick = 0;                                                // Vinnie's whirlwind: time to the next hit
	float SigY = 0; int32 Shots = 0;                                  // the Pit Boss's revolver: the lane the red line locks on, shots fired
	float SigX = 0;                                                   // nights 4-7 (run 32365): where the flame / lunge starts
	// rusher / grappler / thrower
	bool bStruck = false; float Hold = 0; int32 Mash = 0;
	// juggling (FTBrawlMoves.cpp): hits taken in the air since he was launched, and when the last one was (fight time)
	int32 Jug = 0; float JugAt = -9.f; float StompAt = -9.f;   // StompAt: the last stamp that kept him down (its own marker, not the juggle count)
	int32 Stag = 0; float StagAt = -9.f;    // a boss in his second phase: staggers in a row; after four he shrugs the next blows off
	float Shrug = 0.f;                    // for this long blows hurt a boss but neither stagger nor floor him
	float NoFloor = 0.f;                  // a boss just back on his feet: for this long a blow hurts him but cannot floor him
	// stage 2 "variety" (FTBrawlVariety.cpp): which of the character's six looks he wears, how he fights (Mind: an
	// EFTBrawlMind), his own timers for it, a blocker's guard; a boss's phase (1, then 2 from half health) and the
	// flags of his second-phase patterns (a second rush, a second signature move, a second swing)
	int32 Look = 0; uint8 Mind = 0; float MindT = 0, MindWait = 0, MindSide = 0;
	int32 Guard = 3; float GuardT = 0;
	int32 Phase = 1; float PhaseAt = -9.f; bool bChain = false, bEncore = false, bOneTwo = false;
	// stage 2 "hitfeel" (FTBrawlFeel.cpp): Kb = what is left of a knock-back as a speed along the room (px/s, it dies
	// away), Rock = how far the struck figure shakes while the hit-stop holds (px)
	float Kb = 0, Rock = 0;
	bool bIn = false;                     // entered the gameplay boundary (KeepOnScreen)
	bool bSeen = false;                   // entered the actual camera view; retain in presentation framing (also without QA)
	UPROPERTY() TObjectPtr<AFTBrawlFighter> Actor = nullptr;
};

// night 1's cutscenes (run 49531): a sprite in a cutscene (it walks to Goal, then holds Pose; bFly = thrown, ballistic)
struct FFTCutActor
{
	TObjectPtr<AFTBrawlFighter> A = nullptr;
	FVector Pos = FVector::ZeroVector, Goal = FVector::ZeroVector, Vel = FVector::ZeroVector;
	float Speed = 190.f, T = 0, Walk = 0, Dir = 1, Lean = 0, Scale = 1;
	FName Pose = TEXT("idle");
	bool bFly = false, bHidden = false;
};
// one beat of a cutscene: Start sets the scene (who walks where, the camera, a pose); then the line is typed out.
// A beat lasts Hold seconds, or as long as its line takes to read; Enter / J / Space hurries it, Tab skips the cutscene.
struct FFTCutBeat
{
	FString Who, Line; float Hold = 0; bool bWait = false;
	TFunction<void()> Start;
};
struct FFTBrawlPop { FVector P; FString Text; FLinearColor Col; float T = 0; bool bBig = false; };
struct FFTBrawlDrop { FVector P, V; float S = 0.05f; bool bGlass = false, bWood = false; };          // blood (and glass), world cm
struct FFTBrawlBottle { float X = 0, Y = 0, Z = 0, Vx = 0, Vy = 0, Vz = 0, T = 0; };   // the thrower's bottle, brawl px
// run 52616: things that fly flat along a lane: the Pit Boss's bullets and the Dealer's playing cards, brawl px
struct FFTBrawlShot { float X = 0, Y = 0, Z = 0, Vx = 0, T = 0; float Dmg = 0; bool bCard = false, bBall = false; };   // bBall: the Kingpin's bowling balls (run 32365)
struct FFTBrawlFire { float X = 0, Y = 0, T = 0; };   // run 32365: Flambe Frankie's flame leaves the floor burning, brawl px
// run 21219: things to pick up. Kind = an FTBrawl::IT_* index (beer on the bar, weapons, food, power-ups), brawl px like a body.
// bBar = a beer standing on the bar counter (BarAt, world cm); bDrop = dropped by a foe (blinks out after a while);
// bThrown = flying out of Nova's hand.
struct FFTBrawlItem
{
	int32 Kind = 0, Uses = 0;
	float X = 0, Y = 0, Z = 0, Vx = 0, Vz = 0, T = 0, Spin = 0;
	bool bBar = false, bDrop = false, bThrown = false;
	FVector BarAt = FVector::ZeroVector;
	TWeakObjectPtr<class AStaticMeshActor> Actor;
};

enum class EFTBrawlItemCue : uint8 { Weapon, Drink, Food, Power };

struct FFTBrawlItemPresentation
{
	FVector Feet = FVector::ZeroVector, Top = FVector::ZeroVector;
	EFTBrawlItemCue Kind = EFTBrawlItemCue::Weapon;
	float Strength = 1.f;
	float Durability = 1.f;
	bool bLastUse = false;
};

UCLASS()
class AFTBrawlGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AFTBrawlGameMode();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	virtual void Tick(float Dt) override;

	// read by AFTBrawlHUD
	UPROPERTY() FFTBrawlBody P;
	UPROPERTY() TArray<FFTBrawlBody> Foes;
	TArray<FFTBrawlPop> Pops;
	int32 Wave = 0, NumWaves = 3, Kos = 0, Hits = 0, Best = 0, Chain = 0;
	float StT = 0, Time = 0, Go = 0, KoT = 0, WinT = 0, BossIntro = 0, BannerT = -1, Shake = 0;
	FString Banner, BannerSub;
	bool bWon = false, bBossOut = false;
	bool bPadLast = false;   // loop cycle 8: the last input came from a pad (the controls line names pad buttons)
	// Compatibility mirrors for diagnostics; SystemScreen/SystemSelection own all menu behavior.
	int32 PausePage = 0, PauseSelection = 0; // 0 closed, 1 pause, 2 controls, 3 confirmation
	bool bDisconnectedPause = false;
	float HelpT = 0.f;
	bool bShowCombatHelp = false;
	FBox2D SystemItemRect(int32 Index, float ScreenWidth) const; // HUD coordinates, 720 units high
	void SuppressActionInput();
	int32 BossIdx() const;
	FVector World(float X, float Y, float Z) const;
	// the venue: night 1 The Rusty Anchor (boss Big Earl), night 2 The Velvet Room (boss Vinnie Velour; run 16961),
	// night 3 The Lucky 7 (boss The Pit Boss; run 52616). Picked from the map name (L_VelvetRoom, L_Lucky7) or -brawlnight=N.
	int32 Night = 1;
	bool IsVelvet() const { return Night == 2; }
	bool IsLucky() const { return Night == 3; }
	// nights 4-7 (run 32365), one after each of races 4-7: 4 THE TIKI TORCH (Flambe Frankie, FIRE BREATH), 5 STRIKE CITY
	// (the Kingpin, STRIKE!), 6 THE FROSTBITE LODGE (Big Sven, BEAR HUG), 7 THE SEA BISCUIT (the Commodore, ANCHOR SWEEP).
	// Maps L_TikiTorch, L_StrikeCity, L_FrostbiteLodge, L_SeaBiscuit (or -brawlnight=4..7). Art: ArtSource/n47, brawl_n47.py.
	bool IsN47() const { return Night >= 4 && Night <= 7; }
	static const TCHAR* NightMap(int32 N);   // /Game/Brawl/Maps/L_... of night N (1..7), nullptr past 7
	FString NextVenue;                       // the next night's name (the K.O. and end cards)
	FString NextLabel() const;               // what Enter goes on to: the next night, or in the main game's career the next day
	void GoOn();                             // Enter after the night (the K.O. or the END OF NIGHT card)
	// day 1 (run 73444): L_Day1Set (or -brawlday) is no fight, only the cutscene BEFORE THE DRIVE on the Route 9 film set
	// (FTBrawlCutscene.cpp, cut 10); it ends on the THE DRIVE card and Enter goes on to night 1, the Rusty Anchor.
	bool bDay = false;
	// the days (run 77694): the same set plays every day's BEFORE THE DRIVE (cut 10) and THE WRAP after it (cut 11), days 1-8,
	// with the crew on screen (Voss, Kip, Rosa Delgado, Gail Prentiss, Dee Dee Starr, the Commodore). Which day: the main
	// game's career (FTNightFlow), else -brawlday=N / the map's ?brawlday=N option; -brawlwrap (or ?wrap) starts on THE WRAP.
	int32 DayN = 1; bool bDayWrap = false;
	void StartDayCut(int32 Id, TFunctionRef<void(const TCHAR*, const FString&, float, TFunction<void()>)> B);   // days 1-8
	void DayCutDone();                       // ENTER on a day's end card
	void SetSetLight(bool bDusk);            // morning for BEFORE THE DRIVE, the golden hour for THE WRAP
	UPROPERTY() TObjectPtr<class ADirectionalLight> SetSun;
	UPROPERTY() TObjectPtr<class ASpotLight> SetLamp;
	FString VenueName = TEXT("THE RUSTY ANCHOR"), VenueTag = TEXT("WATERED BEER · DIRTY MONEY · NO COPS AFTER 2 AM");
	FString BossName = TEXT("BIG EARL"), BossTitle = TEXT("OWNS THE PLACE · WATERS THE BEER · BREAKS THUMBS"), BossCall = TEXT("EARL: \"BOYS! PUT HIM IN THE BAY!\"");
	FLinearColor Neon = FLinearColor(0.02f, 0.88f, 1.f);

	void Retry();
	void Restart();

	// night 1's cutscenes (run 49531, FTBrawlCutscene.cpp): 1 = THAT NIGHT (before the fight), 2 = LAST CALL (after
	// Big Earl), 3 = 2:40 AM out on the pier (the in-between story), ending on the END OF NIGHT 1 card. Read by the HUD.
	int32 CutId = 0;
	bool InCut() const { return CutId != 0; }
	FString CutWho, CutLine, CutTitle, CutSub, CutCard, CutCardSub, CutCardKeys;
	FLinearColor CutWhoCol = FLinearColor::White;
	float CutLineT = 0, CutFade = 0, CutFlash = 0, CutBars = 0, CutTitleT = -1;
	bool bCutWait = false;
	// 3.9: ENTER / click on the K.O. and win screens acts on the press, not on a key still held from the fight
	bool bScreenEnterPrev = true;
	// 3.9 test jump (main game: FTGo N<n>boss / -ftgo=): this night opens on its boss; logs "FT GO landed" when he is out
	bool bGoBoss = false;

	// pickups (run 21219): what lies around, what Nova holds, the timed power-ups
	TArray<FFTBrawlItem> Items;
	int32 Held = -1, HeldUses = 0;
	float PowerT = 0, ToughT = 0, BuzzT = 0;   // brass knuckles, leather jacket, a beer from the bar
	static FString ItemName(int32 Kind);
	int32 ItemAt() const;   // the item J would grab right now (a weapon at his feet or a beer on the bar), INDEX_NONE if none
	int32 NearbyWalkPickup() const;
	bool DescribeItem(const FFTBrawlItem& Item, FFTBrawlItemPresentation& Out) const;
	bool ReducedPresentationMotion() const;

	// Controller-driven system UI remains live while the simulation/audio world is paused.
	int32 SystemScreen = 0, SystemSelection = 0;
	void ToggleSystemMenu();
	void RequestSystemQuit(bool bDesktop = true);
	void PollSystemSave();
	void SystemNavigate(int32 Delta);
	void SystemAdjust(int32 Delta);
	void OpenDisconnectedSystemMenu();
	void SyncPauseCompatibility();
	void SystemActivate();
	void SystemBack();
	TArray<FString> SystemRows() const;
	bool SystemRowEnabled(int32 Index) const;
	FString SystemHeading() const;
	FString SystemDetail() const;
	FString CheckpointLabel() const;
	FString SaveStatus(bool& bError) const;
#if !UE_BUILD_SHIPPING
	void PrepareGrabFixture(bool bEnable);
#endif
	// development pose stage (FTBrawlPose.cpp): ftb.Pose / ftb.PoseFilm / ftb.PoseSweep / ftb.PoseClear show any character in
	// any state at any move-time through the fight's own SyncBody -> AFTBrawlFighter::Pose. Inert unless one is called:
	// PoseBodies stays empty and Tick never leaves the ordinary path. While it is in use the fight stands still.
	void PoseCommand(const TArray<FString>& Args, bool bFilm);
	void PoseSweep(const TArray<FString>& Args);
	void PoseClear();
	void PoseBackdrop(const TArray<FString>& Args);
	UPROPERTY() TObjectPtr<class AStaticMeshActor> PoseCard;
	bool PoseActive() const { return PoseBodies.Num() > 0; }
	void TickPose(float Dt);
	UPROPERTY() TArray<FFTBrawlBody> PoseBodies;
	struct FPoseRun { FString Who, Token; FName State, Prev; float BaseX = 0, BaseY = 0, Dir = 1, T = 0, Speed = 0, HeightCm = 228; int32 Combo = 0, Lead = 0, Ticks = 0; bool bBossKind = false, bStarted = false; double ArmedAt = 0; FName LastFrame; };
	TArray<FPoseRun> PoseRuns;   // one per PoseBodies entry
	void PoseApply(int32 Index, bool bPrime);
	void SyncBody(FFTBrawlBody& F);
	static void StyleBossSignature(int32 BossNight, FName& Pose, float PoseTime);

private:
	void ReplayCombatHelp();
	void TickCombatHelp(float Dt);
	void FinishCombatHelp();
	bool bBlockCombatInput = false, bHelpInitialized = false;
	bool bSessionReady = false;
	int32 MenuHome = 2, MenuHomeSelection = 0, RequestedExit = 0;
	bool bSystemSaveWait = false, bSystemExitNeedsConfirm = false, bSystemDiscardAfterWait = false;
	double InputResumeAt = 0;
	TArray<TWeakObjectPtr<class UAudioComponent>> SystemPausedAudio;
	void InitializeSession();
	bool ApplySavedCheckpoint();
	bool SaveCheckpoint(const TCHAR* Reason);
	void SetSystemScreen(int32 Screen);
	void FinishSystemAction(bool bDiscard);
	void CompleteSystemAction(bool bDiscard);
	bool IsSystemSaving() const;
	bool HasSystemSaveError() const;
	// Dash / Side (FTBrawlMoves.cpp): a double tap along the room (-1 / +1 = the way to dash) or into it (+1 = up the
	// floor band, -1 = down it), true for the one frame it is read; the autopilot sets them directly
	struct FIn { float Steer = 0, Throttle = 0; bool Fire = false, Kick = false, Jump = false, Enter = false, Grab = false; int32 Dash = 0, Side = 0; };
	FIn ReadInput();
	FIn AutoInput();
	FIn Latch;
	struct FBufferedInput { float Fire = 0, Kick = 0, Jump = 0, Grab = 0, Dash = 0, Side = 0; int32 DashDir = 0, SideDir = 0; };
	FBufferedInput Buffered;
	void BufferInput(const FIn& In);
	float ContactPause = 0.f;
	float SustainedTravel = 0.f;
	enum class ECombatCue : uint8 { Jab, Finish, Weapon, Throw, Hurt, Clear, Count };
	struct FCombatEffect { FVector At; float Age = 0.f, Size = 0.f, Dir = 1.f; };
	void InitCombatPresentation();
	void CombatCue(ECombatCue Cue, const FVector& At, float Direction, bool bSound = true);
	void TickCombatPresentation(float Dt);
	UPROPERTY() TArray<TObjectPtr<class USoundBase>> CombatSounds;
	UPROPERTY() TArray<TObjectPtr<class UTexture>> CombatTextures;
	UPROPERTY() TArray<TObjectPtr<class AStaticMeshActor>> CombatQuads;
	TArray<FCombatEffect> CombatEffects;
	float CueCooldowns[static_cast<int32>(ECombatCue::Count)] = {};
	int32 NextCombatQuad = 0;
	// Visual-only authored cards; this pool never consumes the combat random stream.
	enum class EAccent : uint8 { Dust, Land, Slash, Pickup, Glass, Wood, Smoke, Flame, Count };
	struct FAccentEffect { FVector At, Velocity; float Age = 0, Life = 0, Size = 0, Dir = 1; EAccent Kind = EAccent::Dust; };
	void Accent(EAccent Kind, const FVector& At, float Size, float Direction = 1.f, bool bSound = true);
	void TickAccents(float Dt);
	void InitAccents();
	UPROPERTY() TArray<TObjectPtr<class UTexture>> AccentTextures;
	UPROPERTY() TArray<TObjectPtr<class USoundBase>> AccentSounds;
	UPROPERTY() TArray<TObjectPtr<class AStaticMeshActor>> AccentQuads;
	UPROPERTY() TObjectPtr<class USoundAttenuation> CombatAttenuation;
	UPROPERTY() TObjectPtr<class USoundConcurrency> CombatConcurrency;
	TArray<FAccentEffect> AccentEffects;
	int32 NextAccentQuad = 0, SoundVariation = 0;
	float AccentSoundCooldown = 0.f, MotionAccentTime = 0.f, BossAccentTime = 0.f;
	float LastPresentationT = -1.f;
	FName LastPresentationState;
	int32 LastPresentationCombo = 0;
	void Step(float Dt, const FIn& In);
	void UpdatePlayer(float Dt, const FIn& In);
	void UpdateFoes(float Dt);
	void HitFoe(FFTBrawlBody& F, float Dmg, bool bDown, bool bSound = true);
	void HurtPlayer(float Dmg, bool bDown, const FFTBrawlBody& From);
	void SpawnWave(int32 I);
	void SpawnBoss();
	FFTBrawlBody MakeFoe(int32 Id, FName Kind, float X, float Y);
	void Pop(float X, float Y, float Z, const FString& Text, const FLinearColor& C, bool bBig = false);
	// blood: a spray from a hit (Dir = which way it flies), drops fall and leave splats on the floor that stay all night
	void Bleed(float X, float Y, float Z, float Dir, int32 N, float Force, bool bGlass = false, bool bWood = false);
	void Splat(const FVector& At, float Size);
	void StepFx(float Dt);
	TArray<FFTBrawlDrop> Drops;
	TArray<FFTBrawlBottle> Bottles;
	TArray<FFTBrawlShot> Shots;
	void StepShots(float Dt);
	TArray<FFTBrawlFire> Fires;
	float Lanes[3] = { 0, 0, 0 };   // the Kingpin's three lit lanes
	void BossSig47(FFTBrawlBody& F, float Dt, bool& bDone);   // nights 4-7: the boss's own move (FTBrawlGameMode.cpp)
	UPROPERTY() TArray<TObjectPtr<class AStaticMeshActor>> LaneA;   // the lit lanes / Frankie's cone / Sven's line on the floor
	UPROPERTY() TArray<TObjectPtr<class AStaticMeshActor>> BallA;   // the bowling balls (painted)
	UPROPERTY() TArray<TObjectPtr<class AStaticMeshActor>> FireA;   // burning patches
	UPROPERTY() TObjectPtr<class APointLight> FireL;
	UPROPERTY() TObjectPtr<UInstancedStaticMeshComponent> CardIsm;
	UPROPERTY() TObjectPtr<UInstancedStaticMeshComponent> SlugIsm;
	// the Lucky 7's moving parts: the Pit Boss's red line (on the floor along the lane, and the beam at gun height), the muzzle flash
	UPROPERTY() TObjectPtr<class AStaticMeshActor> LaserFloor;
	UPROPERTY() TObjectPtr<class AStaticMeshActor> LaserBeam;
	UPROPERTY() TObjectPtr<class APointLight> Muzzle;
	float MuzzleT = 0;
	int32 SplatNext = 0;
	UPROPERTY() TObjectPtr<UInstancedStaticMeshComponent> DropIsm;
	UPROPERTY() TObjectPtr<UInstancedStaticMeshComponent> GlassIsm;
	UPROPERTY() TObjectPtr<UInstancedStaticMeshComponent> SplatIsm;
	UPROPERTY() TObjectPtr<UInstancedStaticMeshComponent> BottleIsm;
	UPROPERTY() TObjectPtr<UInstancedStaticMeshComponent> WoodIsm;
	void SpawnPickups();
	void DropItem(int32 Kind, float X, float Y, int32 Uses, bool bHop, bool bDrop);
	void DropFrom(const FFTBrawlBody& F);
	void StepItems(float Dt);
	void SyncItems();
	bool CanWalkPickup() const;
	bool UseGroundItem();
	void BreakHeld();
	void BreakItem(int32 Kind, float X, float Y, float Z, float Dir, bool bSound = true);
	void ClearItems();
	class AStaticMeshActor* ItemActor(int32 Kind);
	void PlaceItem(AActor* A, int32 Kind, const FVector& Anchor, float Px, float Py, float RotDeg, bool bFlip, float Glow);
	UPROPERTY() TObjectPtr<class AStaticMeshActor> HeldA;
	UPROPERTY() TObjectPtr<class AStaticMeshActor> MugA;
	bool bWpnHit = false;
	bool bHeldPickupPresentation = false;
	FTransform HeldPickupTransform;
	AFTBrawlFighter* SpawnFighter(const FFTBrawlKind& K, const FFTBrawlLook& L);
	// the night's drawings, loaded before the night (FTBrawlSpriteBank): one list for the spawn and for the warm-up
	struct FSpriteChoice { FString Sprite; float H; };
	void SpriteChoices(const FFTBrawlKind& K, TArray<FSpriteChoice>& Out) const;
	FName BossHelpKind(int32 c) const;
	void NightSprites(TArray<FString>& Out) const;
	void CutSprites(TArray<FString>& Out) const;   // FTBrawlCutscene.cpp
	void WarmNight();
	void BuildVenue();
	void BuildVenueAmbience(float StartX, float EndX, float BackWallY);
	void TickVenueAmbience(float RealDt);
	void StyleBossPose(FName& Pose, float& PoseTime) const;
	float BossEntranceCameraOffset() const;
	void SyncBossAccent();
	UPROPERTY() TArray<TObjectPtr<class AStaticMeshActor>> VenueAmbience;
	TArray<FVector> VenueAmbienceOrigins;
	UPROPERTY() TObjectPtr<class APointLight> BossAccent;
	UPROPERTY() TObjectPtr<class AStaticMeshActor> SlamAccent;
	float VenueAmbienceTime = 0.f;
	float BossEntranceAt = 0.f;
	void SetupNight();
	int32 DrinkKind() const;
	bool bSigSeen = false;   // the boss's signature move gets its banner the first time
	// the Velvet Room's moving parts: sweeping coloured spots, Vinnie's spotlight and the pink ring on the floor round him
	UPROPERTY() TArray<TObjectPtr<class ASpotLight>> Sweeps;
	UPROPERTY() TObjectPtr<class ASpotLight> BossSpot;
	UPROPERTY() TObjectPtr<class AStaticMeshActor> BossRing;
	void SyncActors(float RealDt);
	// cutscenes (FTBrawlCutscene.cpp)
	TArray<FFTCutBeat> Beats;
	TMap<FName, FFTCutActor> CutCast;
	int32 CutBeat = -1; float CutT = 0, CutFadeGoal = 0, CutBarsGoal = 0; bool bCutPrev = false, bCutSkipPrev = false, bCutEnterPrev = false, bCutDone = false;
	FVector CutCamLoc = FVector::ZeroVector, CutCamGoal = FVector::ZeroVector; float CutPitch = -16, CutPitchGoal = -16, CutFov = 46, CutFovGoal = 46, CutCamRate = 1.5f;
	bool bCuts = true;
	FRandomStream CutRndSave;   // the fight's random stream, put back after the intro so the fight plays exactly as without it
	void StartCut(int32 Id);
	void StartNightCut(int32 Id, TFunctionRef<void(const TCHAR*, const FString&, float, TFunction<void()>)> B);   // nights 2-7
	void StepCut(float Dt, const FIn& In);
	void SyncCut(float Dt);
	void EndCut();
	void NextBeat();
	FFTCutActor& AddCast(FName Key, const FString& Who, float HeightCm, float Scale, const FVector& At, float Dir);
	void CamTo(const FVector& Loc, float Pitch, float Fov, float Rate = 1.5f, bool bSnap = false);
	void Walk(FName Key, const FVector& To, float Speed = 190.f);
	void PoseAs(FName Key, FName Pose, float Dir = 0);
	FVector RigLoc() const;
	// the pier outside the Rusty Anchor (built at BeginPlay on night 1, far along +X from the bar)
	void BuildPier();
	static constexpr float PierX = 30000.f;
	// day 1's film set on Route 9 (run 73444), built far along +X; the KESTREL uses Limo / LimoAt / LimoV to drive off
	void BuildSet();
	static constexpr float SetX = 80000.f;
	UPROPERTY() TObjectPtr<class AStaticMeshActor> Limo;
	UPROPERTY() TObjectPtr<class APointLight> FlashL;
	UPROPERTY() TObjectPtr<class APointLight> TailL;
	FVector LimoAt = FVector::ZeroVector; float LimoV = 0;
	float NextWaveAt(int32 I) const { return 260.f + I * 800.f; }
	float ArenaEnd() const { return NextWaveAt(NumWaves); }
	void ResetState(int32 WaveIdx, bool bKeepStats);

	TArray<TArray<FName>> Waves;
	TMap<FName, FFTBrawlKind> Kinds;
	FFTBrawlKind BossK; float BossRush = 18;
	float Cam = 0, LockX = -1, Acc = 0; bool bSpawned = false;
	FRandomStream Rnd;
	float Rand() { return Rnd.FRand(); }
	UPROPERTY() TObjectPtr<ACameraActor> CamActor;
	// the moon (night 1): in the fight it rides the camera's scroll (X only); in a cutscene it stays put in the sky,
	// and out on the pier it hangs over the harbour (owner, 15 Nov: "the moon travels with the camera")
	UPROPERTY() TObjectPtr<AActor> MoonA;
	FVector MoonOff = FVector::ZeroVector;   // from the fight camera's home (X, 1450, CamZ)
	void PlaceMoon();
	UPROPERTY() TArray<TObjectPtr<AFTBrawlFighter>> Pool;
	bool bAuto = false;
	// grab and throw (run 42137): L / C / pad B lifts the foe in front over his head (state lift -> carry, the foe "lifted"),
	// L / J / K hurls him (steer = which way): the body ("hurled") flies down the lane and knocks down every foe it meets.
	int32 GrabId = -1;   // the lifted foe's Id, -1 = none
	// keep every foe on screen (owner, 15 Nov): test hooks -brawlcamcheck (log any foe the camera can't see) and
	// -brawlnokeepin (the old rules, to show the bug); CamFrames/CamOff/CamWorst = the check's tally for the night
	bool bCamCheck = false, bNoKeepIn = false; int32 CamFrames = 0, CamOff = 0; float CamWorst = 0;
	void KeepOnScreen();
	void CheckOnCamera();
	bool bAutoGrab = false, bKeyTest = false;   // -brawlgrab: the autopilot grabs and throws; -brawlkeytest: it presses real keys
	bool bPadTest = false;
	TSet<FName> TestKeysDown;
	bool TryGrab();
	void Hurl();
	void DropGrab();
	void StepHurled(FFTBrawlBody& F, float Dt);
	void PressKeys(const FIn& In);
	// fight-night polish (run 38222): the fight's sounds (Docs/audio/sfx from Tools/brawl_sfx_synth.py, imported by
	// Content/Python/brawl_audio.py to /Game/Brawl/Audio/SFX/S_brawl_<Name>) and the pad's rumble (hits landed: a short light
	// buzz; hits taken and knockdowns: a hard, longer one). Logs FTBRAWL sfx / FTBRAWL rumble (LogFTBrawl Verbose).
	void Sfx(const TCHAR* Name, float Vol = 1.f, float Pitch = 1.f);
	void ImpactSfx(int32 Kind);   // a weapon landing: wood, metal or glass
	void Rumble(float Intensity, float Dur, const TCHAR* Why);
	UPROPERTY() TMap<FName, TObjectPtr<class USoundBase>> Sounds;
	double RumbleAt = -1; float RumbleI = 0; int32 Voice = 0, NSfx = 0, NRumble = 0;
	static bool Standing(const FFTBrawlBody& F);   // on his feet and hittable (not flying, down, getting up, lifted or hurled)

	// ---- Nova's wider move set (FTBrawlMoves.cpp; the table of every move is at the top of that file) ----
	// New states: sidestep dashfwd dashatk runatk airjab dive launch charging heavy special backatk stomp pummel
	// throwback slam wpnair wpndash airtoss, and the look "wpnbreak". None of them changes an older state's length.
public:
	void MoveCommand(const TArray<FString>& Args);   // development: ftb.Move <name> starts a move as its input would
	void MoveFixture(const TArray<FString>& Args);   // development: ftb.MoveFixture <layout> stands quiet foes round Nova
	void SeqCommand(const TArray<FString>& Args);    // development: ftb.Seq <step> ... plays a timed run of button presses
	static bool IsNewMove(FName St);                 // one of the new player states
private:
	void MovesReset();
	void MovesReadTaps(FIn& In);                     // ReadInput: double taps -> In.Dash / In.Side
	void MovesTick(float Dt, const FIn& In);         // every fight step, before the player's states
	bool MovesAir(float Dt, const FIn& In);          // in the air: air jab, dive, air weapon swing and throw; true = it took the press
	bool MovesState(float Dt, const FIn& In);        // one of the new ground states is running (true = handled)
	void MovesNeutral(const FIn& In);                // starts a new move from idle / walk (or cancels a punch into one)
	bool MovesCarry(const FIn& In);                  // carrying a foe: pummel, slam, throw back; true = it took the press
	bool MovesHoldGrab(FFTBrawlBody& G);             // places the grabbed foe during pummel / slam / throwback; true = still held
	void MovesJuggle(float Dmg, float Reach);        // the older attacks can also hit a foe who is in the air
	bool MovesEvade(const FFTBrawlBody& From);       // HurtPlayer: inside a sidestep's or the special's window = no hit
	void MovesStylePose(const FFTBrawlBody& F, FName& St, float& PoseTime) const;   // looks only (the weapon just broke)
	void MovesTint(FLinearColor& Tint) const;        // charging glow, the dodge window
	bool MovesWeaponAngle(float& Ang) const;         // the held weapon's angle in the new weapon moves
	bool MovesAuto(FIn& I, const FFTBrawlBody* Tgt); // the autopilot uses the new moves (ftb.AutoMoves 1)
	bool MoveStart(FName Move, const FIn& In);       // begin one move now (shared by input, autopilot and ftb.Move)
	int32 MoveStrike(float Dmg, float Front, float Back, float Depth, bool bDown, bool bFists, const TCHAR* HitSfx, float SfxPitch = 1.f);
	bool JuggleOne(FFTBrawlBody& F, float Dmg, float Pop, float Push);
	float MoveMult(bool bFists) const;
	// the held weapon's numbers (the item table lives in FTBrawlGameMode.cpp)
	float HeldDmg() const; float HeldReach() const; bool HeldDown() const; bool HeldGlass() const;
	void HeldUsed(const FFTBrawlBody& On);          // one use of the held weapon spent on a hit
	void ThrowHeld(float FromZ);                     // the held weapon leaves his hand (toss, airtoss)
	void HeldGive(int32 Kind);                       // development: put a weapon in his hand (-1 = a frying pan)
	FName PendingMove; float PendingPower = 0.f, PendingT = 0.f;   // ftb.Move: a move waiting for its jump or grab
	struct FSeqStep { FIn In; float Dur = 0.05f; FString Text; };
	TArray<FSeqStep> SeqSteps; int32 SeqAt = 0; float SeqT = 0.f;   // ftb.Seq: the presses still to play
	void MovesSeqInput(FIn& In, float Dt);           // Tick: the sequence's buttons go in as if pressed
	FName TraceSt; float TraceT = 0.f;               // ftb.MoveTrace: the player's last logged state
	float EvadeT = 0.f;           // > 0: nothing hits him (sidestep, special)
	bool bEvadeSaid = false;      // one "DODGE!" a window
	float BreakLookT = 0.f;       // the weapon broke a moment ago: he looks at his empty hand (looks only)
	float FireHeldT = 0.f;        // how long the punch button has been down
	float AirT = 0.f;             // time since take-off (the jump drawing picks up from here after an air move)
	bool bBreakArmour = false;    // set round a thrown body's hit: it floors a boss whatever he is doing
	float MovePower = 0.f;        // the charged heavy: 0..1
	float CarryT = 0.f;           // the carry clock, kept across a pummel
	int32 AirJabs = 0, Pummels = 0, HeldBoth = 0;
	bool bMoveHit = false, bMoveDone = false, bChargeFull = false;
	double TapSteerAt = -9, TapThrottleAt = -9; int32 TapSteerDir = 0, TapThrottleDir = 0; bool bTapSteerDown = false, bTapThrottleDown = false;
	float AutoHoldFire = 0.f; int32 AutoPlan = 0; float AutoPlanT = 0.f;

	// ---- enemy variety and the bosses' second phase (FTBrawlVariety.cpp; the tables are at the top of that file) ----
public:
	void VarietyCommand(const TArray<FString>& Args);   // development: ftb.Variety looks|mind|phase2|bosshp|list
private:
	void VarietyReset();
	void VarietySpawn(FFTBrawlBody& F, FName Kind);                // MakeFoe: picks his look and his way of fighting
	bool VarietyBlock(FFTBrawlBody& F, float Dmg, bool bDown);    // HitFoe: a blocker takes it on his guard (true = no hit)
	bool VarietyState(FFTBrawlBody& F, float Dt);                  // UpdateFoes: the new foe states (block, rage); true = handled
	void VarietyWalk(FFTBrawlBody& F, float Dt, int32 Engaged, bool& bWant, float& Tx, float& Ty, float& Sp);   // where he walks, whether he may swing
	void VarietyThrower(FFTBrawlBody& F, float Dt, bool bDealer, float& Tx, float& Ty, float& Sp);              // the same for bottle and card throwers
	float VarietyCd(const FFTBrawlBody& F) const;                  // how his cooldown after an attack is scaled
	float VarietyMelee(const FFTBrawlBody& F) const;               // inside this distance a thrower slaps instead
	bool VarietyBossPhase(FFTBrawlBody& F);                        // half health: the second phase begins (true = this step is spent)
	bool VarietyBeat(float Dt);                                    // Step: the short transition beat holds the fight
	bool VarietyCallOk(const FFTBrawlBody& F) const;               // the boss calls his help after the beat's banner, not over it
	bool VarietyEncore(FFTBrawlBody& F, FName Tell);               // phase 2: the move just ended is told and done once more
	bool VarietyOneTwo(FFTBrawlBody& F);                           // phase 2: a second swing, with its own full wind-up
	void VarietyLeap(FFTBrawlBody& F, float Dt);                   // phase 2, Earl: the second slam's leap travels toward Nova
	float VarietyWhirlSpeed(const FFTBrawlBody& F) const;          // phase 2, Vinnie: the whirlwind chases faster
	void VarietyStyle(FFTBrawlBody& F, FName St, float& Lean);     // SyncBody: looks only (glow, guard stance, lean)
	TMap<FName, int32> MindCount; TMap<FString, int32> LookCount;  // how many of a role / of a character have spawned this night
	float PhaseBeat = 0.f;                                         // > 0: the transition beat is playing

	// ---- the stages: breakable scenery, onlookers, depth layers, lighting moods (FTBrawlStage.cpp; the tables and the
	// development command ftb.Stage are described at the top of that file). Looks and scenery only, except that a
	// breakable can be hit, can drop a pickup, and its pieces can stagger a foe standing right behind it.
public:
	void StageCommand(const TArray<FString>& Args);   // development: ftb.Stage list | goto | hit | react | mood | show
	bool StageAdopt(const TCHAR* Prop, float CmPerPx, float WorldX, float WorldY, bool bFlip);   // BuildVenue: a painted prop at the wall that can break (true = the stage owns it)
private:
	struct FStageBreak
	{
		int32 Kind = 0, State = 0, Id = 0;     // State: 0 whole, 1 damaged, 2 wrecked (the heap), 3 gone
		float X = 0, Y = 0;                     // brawl px: along the room, and the depth it is hit at
		FVector At = FVector::ZeroVector;       // world cm: where its base stands
		float K = 0.1f;                         // cm per px of its drawings
		float Hp = 0, Max = 0, Flash = 0, Wob = 0, WobDir = 1, T = 0, Depth = 34;
		bool bFlip = false, bOwn = false;
		int32 LastBody = -1; float LastBodyT = -9.f;
		TWeakObjectPtr<class AStaticMeshActor> Actor;
		TWeakObjectPtr<class APointLight> Light;
	};
	struct FStageChip { FVector P = FVector::ZeroVector, V = FVector::ZeroVector; FRotator R = FRotator::ZeroRotator, Spin = FRotator::ZeroRotator; FVector S = FVector::OneVector; float T = 0, Life = 6; uint8 Mat = 0; bool bRest = false; };
	struct FStageBy
	{
		int32 Who = 0, Pose = 0;                // Pose: 0 watching, 1 cheering, 2 flinching, 3 ducking
		float X = 0, Y = 0, Back = 0, K = 0.1f, Hold = 0, Delay = 0, Phase = 0, Dir = 1, Pop = 0;
		int32 Next = 0;
		TWeakObjectPtr<class AStaticMeshActor> Actor;
	};
	struct FStageLight { TWeakObjectPtr<class ULightComponent> L; float Base = 0; };
	TArray<FStageBreak> StageBreaks;
	TArray<FStageChip> StageChips;
	TArray<FStageBy> StageBys;
	TArray<FStageLight> StageLights;
	UPROPERTY() TArray<TObjectPtr<class UTexture>> StageTex;          // every drawing the stage shows (kept loaded)
	UPROPERTY() TArray<TObjectPtr<class AActor>> StageFg;             // the near layer between the camera and the room
	UPROPERTY() TArray<TObjectPtr<UInstancedStaticMeshComponent>> StageChipIsm;   // wood, glass, brass
	UPROPERTY() TArray<TObjectPtr<class APointLight>> StageMoodL;     // two mood lights that ride with the camera, one at Nova
	UPROPERTY() TObjectPtr<class APostProcessVolume> StageGrade;      // the room's own post-process volume: the mood's grade (gain, saturation, vignette; never the cel material)
	FVector StageGain = FVector(1, 1, 1); float StageSat = 1.f, StageVig = 0.45f, StageVig0 = 0.45f;
	void StageBuild();                               // BeginPlay, after BuildVenue
	void StageReset(bool bKeep);                     // ResetState: everything whole again
	void StageStep(float Dt);                        // every fight step: bodies and bottles against the scenery, what the onlookers saw
	void StageSync(float Dt);                        // every frame: cards, pieces, onlookers, lights
	void StageStrike(float Dmg, float Front, float Back, float Depth, bool bDown);   // one of Nova's attacks is live
	bool StageThrown(float X, float Y, float Z, float Dir, float Dmg);              // a thrown weapon passes (true = it hit scenery)
	void StageDropItem(int32 Slot, float X, float Y);   // FTBrawlGameMode.cpp (the item table lives there): 0 bottle, 1 food, 2 knuckles, 3 jacket, 4 stool
	int32 StageAddBreak(int32 Kind, float CmPerPx, float WorldX, float WorldY, bool bFlip, bool bOwn);
	void StageHit(int32 I, float Dmg, float Dir, bool bHeavy, const TCHAR* By);
	void StageShow(int32 I);
	void StageBurst(const FVector& At, float Dir, int32 N, int32 Mat, float Force);
	void StageReact(int32 Pose, float X, float Range, float Hold);
	int32 StageMood() const;
	TSet<int32> StageSwung; FName StageSwingSt; float StageSwingT = 0.f;   // which breakables this swing has already hit
	FRandomStream StageLookRnd;                      // onlookers and chips: looks only, may be drawn every picture
	FRandomStream StageRnd;                          // the stage's own dice: never the fight's
	int32 StageKos = 0, StageWave = 0, StageShots = 0, StageBottles = 0, StageChain = 0, StageMoodNow = -1, StageMoodForce = -1, StageWrecked = 0;
	FName StagePSt; bool bStageWon = false, bStageShown = true;
	float StageDim = 1.f, StageDimSet = -1.f, StageFlicker = 0.f, StageMoodI = 0.f, StageT = 0.f;
	FLinearColor StageMoodC = FLinearColor::Black;

	// ---- hit feel (FTBrawlFeel.cpp; the numbers are in the table at the top of that file). Hit-stop by strength, sparks
	// and shock rings (M_BrawlGlow; the rings use the existing ring mask), dust, a directed camera kick that dies away,
	// knock-back that slides instead of jumping, a flash on Nova too, the slow beat, the camera's lead and rise.
	// ftb.Feel 0 switches all of it off (the fight is then as it was before this file).
public:
	void FeelCommand(const TArray<FString>& Args);   // development: ftb.FeelTest slow [boss] | spark <0|1|2> | stats
private:
	struct FFeelSpark { FVector P = FVector::ZeroVector, V = FVector::ZeroVector; float Age = 0, Life = 0, Len = 0, Wide = 0, Glow = 0; };
	struct FFeelRing { FVector P = FVector::ZeroVector; float Age = 0, Life = 0, R0 = 0, R1 = 0, Glow = 0; bool bFloor = false; };
	UPROPERTY() TArray<TObjectPtr<class AStaticMeshActor>> FeelSparkQuads;
	UPROPERTY() TArray<TObjectPtr<class AStaticMeshActor>> FeelRingQuads;
	TArray<FFeelSpark> FeelSparks; TArray<FFeelRing> FeelRings;
	int32 FeelNextSpark = 0, FeelNextRing = 0;
	FVector2D FeelKickOff = FVector2D::ZeroVector, FeelKickVel = FVector2D::ZeroVector;   // the camera's kick: cm along the room, cm up
	float FeelLead = 0.f, FeelRise = 0.f;                    // the camera's lead along the room and its rise (cm), eased
	float FeelSlowT = 0.f, FeelSlowLen = 0.f, FeelSlowTo = 1.f;   // the slow beat: real seconds left, its length, the slowest speed
	float FeelDustT = 0.f;
	// what the night added up to (logged once at the win and by ftb.FeelTest stats)
	int32 FeelLight = 0, FeelHeavy = 0, FeelNova = 0, FeelNovaDown = 0, FeelBeats = 0; bool bFeelLogged = false;
	float FeelStopT = 0.f, FeelStopAdded = 0.f, FeelSlowAdded = 0.f, FeelRealT = 0.f;
	bool FeelOn() const;
	void FeelInit();                                  // BeginPlay: the spark and ring quads
	void FeelReset(bool bKeep);                       // ResetState
	void FeelStop(float Steps, float OldSteps);       // a hit-stop of Steps fight steps where the older rule gave OldSteps
	void FeelFoeHit(FFTBrawlBody& F, float Dmg, bool bDown, float X0);   // HitFoe, last line: X0 = where he stood before the shove
	void FeelNovaHurt(float Dmg, bool bDown, float X0);                  // HurtPlayer, last line
	void FeelNovaSqueeze(const FFTBrawlBody& By);     // a knee or a squeeze while he is held (no HurtPlayer there)
	void FeelLand(FFTBrawlBody& F);                   // a knocked or thrown body meets the floor
	void FeelImpact(const FVector& At, float Dir, int32 Grade, const FLinearColor& Color);   // sparks, ring and kick at a contact: 0 light, 1 heavy, 2 finishing
	void FeelSparkBurst(const FVector& At, float Dir, int32 N, float Speed, const FLinearColor& Color);
	void FeelRing(const FVector& At, float R0, float R1, float Life, bool bFloor, const FLinearColor& Color, float Glow);
	void FeelKick(float DirX, float DirZ, float Cm);  // the camera is knocked that way and swings back
	void FeelSlow(float RealSeconds, float Speed);
	void FeelTickFx(float Dt);                        // Step, first: sparks and rings (they run on through a hit-stop)
	void FeelPaused(float Dt);                        // Step, while a hit-stop holds
	void FeelStep(float Dt);                          // Step, when it does not: the knock-backs slide
	float FeelTimeScale(float RealDt);                // Tick: how fast the fight runs this frame (1 = as ever)
	void FeelCamera(float RealDt, float MidX, float& Sx, float& Sz, float& LeadX, float& RiseZ);   // SyncActors: kick, lead, rise
	void FeelBody(FFTBrawlBody& F);                   // SyncBody: the struck figure shakes while the hit-stop holds
	void FeelStats(const TCHAR* Why);
};
