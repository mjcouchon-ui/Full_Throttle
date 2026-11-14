// The race: a port of the 1.7.2 engine (engine.js + director.js) onto the 3D track and the Chaos car.
// Owns the phases (title, slate, play, wreck, retake, wrap, highlight reel, dailies), every foe and
// their attacks, scoring and stunts, the director's megaphone, slow motion and the chase camera.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Camera/CameraTypes.h"
#include "FTCore.h"
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
enum class EFTPhase : uint8 { Title, Card, Play, Wreck, Wrap, Reel, Rating };

UENUM()
enum class EFTOverlay : uint8 { None, Pause, Controls, Options };

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
	float Glow = 0.f;
	TWeakObjectPtr<AFTEntityActor> Proxy;
};

struct FFTHazardState
{
	int32 Type = 0;     // 0 lane arc, 1 lock-on, 2 shock wave, 3 landing mark
	float D = 0.f, X = 0.f, W = 0.f, Gap = 0.f;
	float Warn = 1.f, Live = 0.4f, T = 0.f;
	bool bHit = false, bDone = false;
	float Track = 0.f;  // lock-on: seconds it follows the car
	TWeakObjectPtr<AFTHazard> Actor;
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
	int32 OptionsRow = 0, ControlsRow = 0, ControlsCol = 0; bool bControlsWaiting = false;
	int32 PauseRow = 0;
	FString ReelLabel, ReelCamName; int32 ReelShot = 0, ReelShots = 0; bool bReelSlow = false; float ReelTitleT = 0.f;
	float Timecode = 0.f;
	int32 HenchOn[2] = { 0, 0 };

	AFTPlayerCar* GetCar() const { return Car; }
	AFTTrack* GetTrack() const { return Track; }
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
	void TickOverlay(float RDt);
	void TickPlay(float Dt, float RDt);
	void TickPlayer(float Dt);
	void TickEnts(float Dt);
	void TickBoss(FFTEnt& B, float Dt);
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
	void GoToLevel(int32 L, bool bStart);

	// entities
	FFTEnt& Spawn(EFTKind K, float Ahead, float Xc, int32 Variant = 0);
	void SpawnGroup(const TArray<EFTKind>& Kinds);
	void Kill(FFTEnt& E, int32 Cause); // 0 gun, 1 missile, 2 ram, 3 other
	void HurtEnt(FFTEnt& E, float Dmg, int32 Cause);
	void HurtPlayer(float Dmg, const TCHAR* Line = nullptr, float PushX = 0.f);
	void Explode(const FVector& At, float Size);
	void Release(FFTEnt& E);
	void ClearEnts();
	FTransform EntTransform(const FFTEnt& E) const;
	void FireBullet(int32 InOwner, float D, float X, float H, float VD, float VX);
	void FireMissile();
	void DropPickup(const FFTEnt& E, EFTKind K);
	FFTEnt* FindEnt(int32 Id);

	// scoring and voice
	void AddScore(int64 Pts, int32 Bucket);
	void Stunt(const FString& Name, int32 Pts, const TCHAR* Say = nullptr, float ReelScore = 3.f);
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

	TArray<FFTEnt> Ents;
	TArray<FFTEnt> PendingEnts; // spawned this frame; joins Ents at the next entity tick
	TArray<FFTHazardState> Hazards;
	int32 NextId = 1;
	TMap<FName, int32> PoolCursor;
	TMap<FName, float> SayCool;
	float AmbientT = 0.f;

	// player
	float Steer = 0.f, Vx = 0.f;
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

	// time
	float SlowT = 0.f, SlowScale = 1.f, SlowCool = 0.f, FreezeT = 0.f, TimeScale = 1.f;
	float DuckT = 0.f, DuckDepth = 1.f, MusicVol = 0.34f;

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
	FTransform ReelCar;
	int32 LastCam = -1;
	float SceneStartClock = 0.f;
};
