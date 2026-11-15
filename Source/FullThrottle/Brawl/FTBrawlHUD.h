// The brawl's screen: Nova's bar and K.O. count, the venue and WAVE n / 3, the combo counter, the boss's bar,
// GO ->, the "!" tells, hit words, the banners (venue, boss intro, KNOCKED OUT, K.O.!) and the key strip,
// all on the canvas like brawl.js's hud().
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FTBrawlHUD.generated.h"

UCLASS()
class AFTBrawlHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;
private:
	float S = 1.f;
	void Txt(const FString& Str, float X, float Y, const FLinearColor& C, float Scale, int32 Align = 0);
	void Rect(float X, float Y, float W, float H, const FLinearColor& C);
	void Centre(const FString& A, const FString& B, const FLinearColor& C);
	void DrawCut(class AFTBrawlGameMode* G);   // night 1's cutscenes: letterbox, dialogue box, title cards, flash, the end card
	TArray<FString> Wrap(const FString& Str, float MaxW, float Scale);
};
