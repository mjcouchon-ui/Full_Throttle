// The brawl's screen: Nova's bar and K.O. count, the venue and WAVE n / 3, the combo counter, the boss's bar,
// GO ->, the "!" tells, hit words, the banners (venue, boss intro, KNOCKED OUT, K.O.!) and the key strip,
// all on the canvas like brawl.js's hud(). Since run 52897 (owner, 15 Nov: "make it match the driving ui a bit
// better, we want the game to feel cohesive") it is drawn in the races' HUD style (FTHUD.cpp / FTHUDStyle.h):
// Orbitron labels and readouts, Rajdhani body text, ink plates with a clipped corner, a hairline rule and an accent
// tick, segmented meters, the same palette. Units: the races' 1280 x 720 screen (S = height / 720).
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FTBrawlHUD.generated.h"

class UFont;

UCLASS()
class AFTBrawlHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;
private:
	float S = 1.f;
	UPROPERTY() TObjectPtr<UFont> FOrb;   // /Game/UI/Fonts/F_Orbitron (the races' display face); engine font if missing
	UPROPERTY() TObjectPtr<UFont> FRaj;   // /Game/UI/Fonts/F_Rajdhani (the races' body face)
	bool bFonts = false;
	bool bReducedMotion = false;
	float OrbH = 1.f, RajH = 1.f;          // each font's line height at scale 1 (px)
	float LastGameTime = -1.f, LastHealth = -1.f, HealthChip = 1.f, HealthHold = 0.f;
	float LastBossHealth = -1.f, BossChip = 1.f, BossHold = 0.f, ComboPulse = 0.f;
	int32 LastBossId = INDEX_NONE, LastChain = 0;
	UFont* Face(bool bDisplay) const;
	float TextW(const FString& Str, float Height, bool bDisplay);
	// Height in the races' units (FTHUDStyle: Label 15, Body 18, Strong 21, Heading 33, Readout 46); Align 0 left, 1 centre, 2 right;
	// MaxW > 0 shrinks a line that would not fit; bShadow = an ink drop shadow for text straight on the picture
	void Txt(const FString& Str, float X, float Y, const FLinearColor& C, float Height, int32 Align = 0, bool bDisplay = true, float MaxW = 0.f, bool bShadow = false);
	void Rect(float X, float Y, float W, float H, const FLinearColor& C);
	void Plate(float X, float Y, float W, float H, const FLinearColor& Accent, float A = 1.f);
	void Meter(float X, float Y, float W, float H, float Frac, const FLinearColor& C, int32 Segments = 1, float A = 1.f);
	void DamageMeter(float X, float Y, float W, float H, float Frac, float Trail, const FLinearColor& C, int32 Segments);
	void Callout(const FString& Str, float Cx, float Y, const FLinearColor& Accent, float Height, float A = 1.f);
	void Card(const FString& Title, const FString& Sub, const FString& Keys, const FLinearColor& Accent, float A = 1.f);
	void DrawSystemMenu(class AFTBrawlGameMode* G);
	void DrawSaveIndicator(class AFTBrawlGameMode* G);
	void DrawCut(class AFTBrawlGameMode* G);   // night 1's cutscenes: letterbox, dialogue box, title cards, flash, the end card
	void DrawControls(class AFTBrawlGameMode* G);
	void DrawCombatHelp(class AFTBrawlGameMode* G);
	void DrawItemCues(const class AFTBrawlGameMode& Game);
	TArray<FString> Wrap(const FString& Str, float MaxW, float Height, bool bDisplay);
};
