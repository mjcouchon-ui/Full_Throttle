// Settings that live across levels and runs: key bindings (two layouts), mute, graphics preset, best takes.
// Saved as JSON in WISPOS_APP_DATA (WispOS gives each app its own folder), else in Saved/FullThrottle.
// In a Shipping build it also checks WispOS's launch ticket, so the game opens only from WispOS.
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InputCoreTypes.h"
#include "Scalability.h"
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
	/** 2.9: what Unreal's hardware benchmark picked, kept so AUTO after a manual preset in the same session is AUTO again. */
	Scalability::FQualityLevels AutoLevels;
	/** 2.9 (the Mac audit): an Apple GPU, or -ftfakeapple (tests the Apple AUTO path on any GPU). */
	static bool IsAppleGpu();
	static bool bHarnessApple;   // FTQuality apple (Development harness)
	/** 2.9: on an Apple GPU, AUTO is capped (Unreal's benchmark rates Apple chips far above what they deliver). Returns the screen-percentage cap. */
	float CapAutoForApple(class UGameUserSettings* S) const;
	void ApplyMute();
	static FString QualityName(int32 Q);
};
