// Everything on screen: the title, the clapper slate, the spy dashboard and the NOW FILMING panel,
// the director's megaphone bar, shouts and popups, CUT! cards, the highlight reel's letterbox, the dailies,
// and the pause, controls (rebinding) and options screens. Drawn on the canvas, scaled to the window.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Math/Box2D.h"
#include "FTHUD.generated.h"

class AFTGameMode;
class UFont;

UCLASS()
class FULLTHROTTLE_API AFTHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "HUD") TObjectPtr<UFont> DisplayFont;
	UPROPERTY(EditAnywhere, Category = "HUD") TObjectPtr<UFont> BodyFont;

private:
	float S = 1.f, W = 1280.f, H = 720.f, T = 0.f;
	UPROPERTY(Transient) TObjectPtr<UFont> FD = nullptr;
	UPROPERTY(Transient) TObjectPtr<UFont> FB = nullptr;
	// 3.0 title: Orbitron Black and Rajdhani Bold (both SIL Open Font License 1.1, see Content/UI/Fonts/OFL-*.txt)
	// and the lettered logo (T_TitleLogo: chrome THROTTLE in Orbitron, neon "Full" in Mr Dafoe, also OFL)
	UPROPERTY(Transient) TObjectPtr<UFont> FOrb = nullptr;
	UPROPERTY(Transient) TObjectPtr<UFont> FRaj = nullptr;
	UPROPERTY(Transient) TObjectPtr<class UTexture2D> TLogo = nullptr;
	bool bTitleAssets = false;
	void TxtF(UFont* F, const FString& Str, float X, float Y, const FLinearColor& C, float Scale, int32 Align = 0, bool bShadow = true, const FLinearColor& Glow = FLinearColor::Transparent);
	FVector2D MeasureF(UFont* F, const FString& Str, float Scale);
	/** A gradient rectangle (A at the top/left, B at the bottom/right). */
	void Grad(float X, float Y, float Wd, float Ht, const FLinearColor& A, const FLinearColor& B, bool bHorizontal);
	void DrawAttractWipe(AFTGameMode* G);
	void DrawAttractOsd(AFTGameMode* G);
	FBox2D TitleButton(float X, float Y, float Wd, float Ht, const FString& Label, const FString& Sub, bool bHot);
	void Txt(const FString& Str, float X, float Y, const FLinearColor& C, float Scale, int32 Align = 0, bool bDisplay = false, bool bShadow = true);
	FVector2D Measure(const FString& Str, float Scale, bool bDisplay);
	void Box(float X, float Y, float Wd, float Ht, const FLinearColor& C);
	void Frame(float X, float Y, float Wd, float Ht, const FLinearColor& C, float Th = 2.f);
	void Panel(float X, float Y, float Wd, float Ht);
	void DrawTitle(AFTGameMode* G);
	void DrawCard(AFTGameMode* G);
	void DrawPlay(AFTGameMode* G);
	void DrawCut(AFTGameMode* G);
	void DrawReel(AFTGameMode* G);
	void DrawRating(AFTGameMode* G);
	void DrawPaycheck(AFTGameMode* G);
	void DrawPreProduction(AFTGameMode* G);
	void DrawFx(AFTGameMode* G);
	void Ring(float Cx, float Cy, float Rad, float Frac, const FLinearColor& C, float Th);
	void DrawReelContinue(AFTGameMode* G);
	/** A neon button: returns its rect (for the mouse). bArmed false = drawn dim; bHot = selected or hovered. */
	FBox2D Button(float Cx, float Cy, float Wd, float Ht, const FString& Label, const FString& Sub, bool bArmed, bool bHot);
	TArray<FString> WrapText(const FString& Str, float MaxW, float Scale);
	void DrawPause(AFTGameMode* G);
	void DrawControls(AFTGameMode* G);
	void DrawOptions(AFTGameMode* G);
	void DrawSlate(float Cx, float Cy, float Sc, float Clap, const TArray<FString>& Lines);
};
