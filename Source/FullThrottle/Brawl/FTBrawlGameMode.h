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
	void SystemActivate();
	void SystemBack();
	TArray<FString> SystemRows() const;
	bool SystemRowEnabled(int32 Index) const;
	FString SystemHeading() const;
	FString SystemDetail() const;
	FString CheckpointLabel() const;
	FString SaveStatus(bool& bError) const;

private:
	bool bSessionReady = false;
	int32 MenuHome = 2, RequestedExit = 0;
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
	struct FIn { float Steer = 0, Throttle = 0; bool Fire = false, Kick = false, Jump = false, Enter = false, Grab = false; };
	FIn ReadInput();
	FIn AutoInput();
	FIn Latch;
	struct FBufferedInput { float Fire = 0, Kick = 0, Jump = 0, Grab = 0; };
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
};
