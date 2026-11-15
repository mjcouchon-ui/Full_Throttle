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
	float Left = 0.f, Right = 1280.f;
	float OrbitHeight = 1.f, RajHeight = 1.f;
	bool bReducedMotion = false;
	enum class EMenuView : uint8 { None, Title, Department, Pause, Controls, Reel };
	EMenuView MotionView = EMenuView::None;
	int32 MotionPage = INDEX_NONE, MotionSelection = INDEX_NONE;
	float ViewEnteredAt = 0.f, SelectionChangedAt = 0.f;
	void UpdateMenuMotion(const AFTGameMode& G);
	float MenuAge(EMenuView View, bool bSelection = false) const;
	void AccentSweep(const FBox2D& Lane, float Age, const FLinearColor& Accent, float Duration);
	void FocusAccent(const FBox2D& Button, float Age, const FLinearColor& Accent);
	float LastArmor = -1.f, ArmorChanged = -10.f, MissileChanged = -10.f, AmmoChanged = -10.f;
	int32 LastMissiles = -1, LastAmmoLoad = -1, LastAmmoRounds = -1;
	UPROPERTY(Transient) TObjectPtr<UFont> FD = nullptr;
	UPROPERTY(Transient) TObjectPtr<UFont> FB = nullptr;
	// 3.0 title: Orbitron Black and Rajdhani Bold (both SIL Open Font License 1.1, see Content/UI/Fonts/OFL-*.txt)
	// and the lettered logo (T_TitleLogo: chrome THROTTLE in Orbitron, neon "Full" in Mr Dafoe, also OFL)
	UPROPERTY(Transient) TObjectPtr<UFont> FOrb = nullptr;
	UPROPERTY(Transient) TObjectPtr<UFont> FRaj = nullptr;
	UPROPERTY(Transient) TObjectPtr<class UTexture2D> TLogo = nullptr;
	bool bTitleAssets = false;
	FVector2D MeasureF(UFont* F, const FString& Str, float Scale);
	/** A gradient rectangle (A at the top/left, B at the bottom/right). */
	void Grad(float X, float Y, float Wd, float Ht, const FLinearColor& A, const FLinearColor& B, bool bHorizontal);
	void DrawAttractWipe(AFTGameMode* G);
	void DrawAttractOsd(AFTGameMode* G);
	enum class EIcon : uint8 { Repair, Missile, Ammo, Turbo, Record, Warning, Play };
	enum class EControl : uint8 { Idle, Hover, Focus, Pressed, Disabled };
	FBox2D Rect(float X, float Y, float Wd, float Ht) const;
	void Stroke(FVector2D A, FVector2D B, const FLinearColor& Color, float Thickness = 1.f);
	void Icon(EIcon Shape, float X, float Y, float Size, const FLinearColor& Color);
	void Plate(const FBox2D& R, const FLinearColor& Accent, const FLinearColor& Fill);
	void Meter(const FBox2D& R, float Fraction, const FLinearColor& Accent, int32 Segments = 1);
	void Print(const FString& Str, float X, float Y, const FLinearColor& Color, float Height, int32 Align = 0, bool bDisplay = false, float MaxWidth = 0.f);
	FVector2D TextSize(const FString& Str, float Height, bool bDisplay = false);
	FBox2D Control(const FBox2D& R, const FString& Label, const FString& Hint, EControl State, const FLinearColor& Accent);
	EControl ControlState(const FBox2D& R, bool bFocus = false, bool bEnabled = true) const;
	void Box(float X, float Y, float Wd, float Ht, const FLinearColor& C);
	void Frame(float X, float Y, float Wd, float Ht, const FLinearColor& C, float Th = 2.f);
	void DrawTitle(AFTGameMode* G);
	void DrawCard(AFTGameMode* G);
	void DrawPlay(AFTGameMode* G);
	void DrawPickupMarkers(const AFTGameMode& Game);
	void DrawCut(AFTGameMode* G);
	void DrawReel(AFTGameMode* G);
	void DrawRating(AFTGameMode* G);
	void DrawPaycheck(AFTGameMode* G);
	void DrawPreProduction(AFTGameMode* G);
	void DrawFx(AFTGameMode* G);
	void DrawReelContinue(AFTGameMode* G);
	TArray<FString> WrapText(const FString& Text, float MaxWidth, float FontHeight, bool bDisplay = false);
	void DrawPause(AFTGameMode* G);
	void DrawControls(AFTGameMode* G);
	void DrawOptions(AFTGameMode* G);
};
