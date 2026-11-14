// Settings that live across levels and runs: key bindings (two layouts), mute, graphics preset, best takes.
// Saved as JSON in WISPOS_APP_DATA (WispOS gives each app its own folder), else in Saved/FullThrottle.
// In a Shipping build it also checks WispOS's launch ticket, so the game opens only from WispOS.
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InputCoreTypes.h"
#include "FTGameInstance.generated.h"

UENUM()
enum class EFTAction : uint8 { Left, Right, Up, Down, Drift, Turbo, Fire, Missile, Count };

UCLASS()
class FULLTHROTTLE_API UFTGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	static constexpr int32 NumActions = 8;
	FKey Binds[NumActions][2];
	int32 Layout = 0;            // 0 WASD, 1 arrows
	bool bMuted = false;
	int32 Quality = -1;          // -1 auto, 0 low .. 3 epic, 4 cinematic
	bool bShowFps = false;
	bool bVSync = true;
	int32 BestScore[3] = { 0, 0, 0 };
	FString BestTitle[3];

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
	void ApplyMute();
	static FString QualityName(int32 Q);
};
