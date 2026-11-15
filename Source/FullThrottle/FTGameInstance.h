// Settings that live across levels and runs: key bindings (two layouts), mute, graphics preset, best takes.
// Saved as JSON in WISPOS_APP_DATA (WispOS gives each app its own folder), else in Saved/FullThrottle.
// In a Shipping build it also checks WispOS's launch ticket, so the game opens only from WispOS.
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InputCoreTypes.h"
#include "Scalability.h"
#include "Engine/StreamableManager.h"
#include "FTGameInstance.generated.h"

UENUM()
enum class EFTAction : uint8 { Left, Right, Up, Down, Drift, Turbo, Fire, Missile, Ammo, Count };

UCLASS()
class FULLTHROTTLE_API UFTGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	static constexpr int32 NumActions = 9;   // 2.6: AMMO (06 §5)
	FKey Binds[NumActions][2];
	int32 Layout = 0;            // 0 WASD, 1 arrows
	bool bMuted = false;
	int32 Quality = -1;          // -1 auto, 0 low .. 3 epic, 4 cinematic
	bool bShowFps = false;
	bool bVSync = true;
	static constexpr int32 MaxLevels = 8;
	int32 BestScore[MaxLevels] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	FString BestTitle[MaxLevels];
	int32 BestLv[MaxLevels] = { 0, 0, 0, 0, 0, 0, 0, 0 };   // 2.6 (09 §3): the driver level each best take was set at
	bool bReducedMotion = false;                             // 2.6 option: skips Film Burn and Supernova's flash (07 §1)
	/** 2.3's skill points and upgrade ranks, kept in the settings file untouched (the career replaces them). */
	TSharedPtr<class FJsonObject> LegacySkills;

	// 2.6 flow and test hooks
	int32 PendingScene = 0;
	bool bHarnessSequel = false;
	bool bPendingPreProd = false;
	FString TitleBanner;
	// -ftexec=<file>: run FT* harness commands from a file (lines: a console command, "wait <s>", "waitfor <text> [s]",
	// "shot <name>", "quit"), so the spec's checks run in a Development package
	bool bExecScript = false;
	TArray<FString> ExecLines;
	int32 ExecIdx = 0;
	double ExecWaitUntil = 0.0;
	FString ExecWaitFor; double ExecWaitForUntil = 0.0; int32 ExecWaitFrom = 0;
	bool TickExec(float Dt);
	virtual void Shutdown() override;

	// carried between maps
	int32 PendingLevel = -1;
	bool bStartRace = false;
	int32 RunCounter = 0;
	int32 TitleLevel = 0;

	// 3.0 attract mode: behind the title the game plays itself and cuts from level to level. A cut loads the next
	// level's map with its track built light around the shot (AttractD0, cm along the road; -1 = a full build), and
	// the title (selection, time on it) carries over. The title song lives here so it plays on across those loads.
	float AttractD0 = -1.f;
	int32 AttractScene = 0, AttractSpawn = 0;
	bool bAttractCut = false;
	int32 KeepTitleSel = 0;
	float KeepTitleT = 0.f;
	int32 AttractCuts = 0;
	UPROPERTY(Transient) TObjectPtr<class UAudioComponent> TitleMusic;
	bool bTitleMusicOn = false;
	/** Start the title song (Bit-Beast Dash) if it isn't playing, at this volume; it survives map loads. */
	void PlayTitleMusic(UObject* Ctx, float Volume);
	void SetTitleMusicVolume(float Volume);
	void StopTitleMusic(float FadeSeconds);

	// benchmark (-ftbench): fixed preset, autopilot, writes a JSON report and quits
	bool bBench = false;
	float BenchSeconds = 75.f;
	int32 BenchQuality = 2;
	FString BenchTag;
	// test flags: -ftauto (the autopilot drives), -ftshots=N (screenshot with the HUD every N s), -ftquit=N (quit after N s)
	bool bAutoDrive = false;
	float ShotEvery = 0.f;
	float QuitAfter = 0.f;
	// -ftwrap=N: the level wraps after N s of racing (tests of the reel and the garage)
	float WrapAfter = 0.f;

	static const TCHAR* ActionName(int32 A);
	static bool IsReserved(const FKey& K);
	void ResetBinds();
	void SetBind(int32 Action, int32 LayoutIdx, const FKey& Key); // clears the same key from other actions in that layout
	FString SaveDir() const;
	void LoadSettings();
	void SaveSettings();
	void ApplyQuality(int32 Q, bool bSave = true);
	void ReapplyQuality();
	bool bAutoMeasured = false;
	/** Loop cycle 4: AUTO's benchmark result, kept in settings.json for this graphics card (and these thresholds), so
	 *  only the first launch pays for the hardware benchmark (3.8 s of a 6.6 s launch here, longer on a slow GPU). */
	FString AutoCacheKey;
	TArray<int32> AutoCache;
	float AutoCacheRes = 100.f, AutoCacheCpu = 0.f, AutoCacheGpu = 0.f;
	static FString AutoKey();
	/** 2.9: what Unreal's hardware benchmark picked, kept so AUTO after a manual preset in the same session is AUTO again. */
	Scalability::FQualityLevels AutoLevels;
	/** 2.9 (the Mac audit): an Apple GPU, or -ftfakeapple (tests the Apple AUTO path on any GPU). */
	static bool IsAppleGpu();
	static bool bHarnessApple;   // FTQuality apple (Development harness)
	/** 2.9: on an Apple GPU, AUTO is capped (Unreal's benchmark rates Apple chips far above what they deliver). Returns the screen-percentage cap. */
	float CapAutoForApple(class UGameUserSettings* S) const;
	void ApplyMute();
	/** Loop cycle 6: every map load puts the engine's master volume back to 1 (UEngine::LoadMap), and only the race's maps
	 *  applied mute again; the nights (Brawl maps) played at full volume for a muted player. */
	void OnPostLoadMap(UWorld* World);
	/** Loop cycle 1: every gameplay sound, effect and vehicle mesh is loaded in the background once, while the title is up,
	 *  and held for the session. Before, the first explosion / chopper / fire / barrel of a race each loaded synchronously
	 *  mid-race (54 blocking loads in Level 1's first two minutes, some reloaded after each GC) - stalls on a cold disk. */
	void WarmGameplayAssets();
	TArray<TSharedPtr<FStreamableHandle>> WarmHandles;
	bool bWarmStarted = false;
	static FString QualityName(int32 Q);
};
