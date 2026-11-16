// Everything on screen: the title, the clapper slate, the spy dashboard and the NOW FILMING panel,
// the director's megaphone bar, shouts and popups, CUT! cards, the highlight reel's letterbox, the dailies,
// and the pause, controls (rebinding) and options screens. Drawn on the canvas, scaled to the window.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Math/Box2D.h"
#include "FTCursor.h"
#include "FTHUD.generated.h"

class AFTGameMode;
class UFont;
class USoundBase;

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
	float RajHeight = 1.f, BunHeight = 1.f;
	bool bReducedMotion = false;
	enum class EMenuView : uint8 { None, Title, Department, Pause, Controls, Reel, Options, Skills };
	EMenuView MotionView = EMenuView::None;
	int32 MotionPage = INDEX_NONE, MotionSelection = INDEX_NONE;
	float ViewEnteredAt = 0.f, SelectionChangedAt = 0.f;
	void UpdateMenuMotion(const AFTGameMode& G);
	float MenuAge(EMenuView View, bool bSelection = false) const;
	void AccentSweep(const FBox2D& Lane, float Age, const FLinearColor& Accent, float Duration);
	void FocusAccent(const FBox2D& Button, float Age, const FLinearColor& Accent);

	// ---- motion (the overhaul of 15 Nov). The layout and every hit box stay where they were: a block that arrives is
	// drawn through a canvas offset and a fade, and the rectangle the mouse is read against is the final one from the
	// first frame. REDUCED MOTION switches all of it off. (FTHUD.cpp, "motion".)
	float Dt = 0.f, LastT = -1.f;                 // real seconds since the last draw
	float Fade = 1.f;                             // multiplies the opacity of everything the primitives draw
	struct FBlockState { float Fade; bool bPushed; };
	TArray<FBlockState, TInlineAllocator<8>> Blocks;
	void BeginBlock(float In, float Dx, float Dy, float Ox, float Oy);
	void EndBlock();
	struct FBlock;                                // everything drawn while one lives arrives together
	struct FPivot;                                // ... or is scaled and turned about a point
	float In01(float Age, float Delay, float Duration) const;
	float Flash01(float Age, float Duration) const;
	float ShakeX(float Age, float Pixels, float Duration) const;
	TMap<int32, float> FocusState;                // (view, page, item) -> 0..1: the focused item leans in, the last one eases back
	float FocusLevel(int32 Key, bool bFocused);
	float FocusFlash(EMenuView View) const;
	// the dim behind a screen and the letterbox mattes are one eased state each, shared by every screen, so a screen
	// hands over to the next instead of cutting
	float DimNow = -1.f, OverDimNow = 0.f, MatteNow = -1.f;
	void Backdrop(float Dim, float MattePx, bool bWall = false);
	void OverlayDim(float Dim);
	float ReelBar() const;
	// the sounds of the HUD's own motion (a stamp landing, a total counting up) are played where the motion is drawn
	UPROPERTY(Transient) TMap<FName, TObjectPtr<USoundBase>> CueSounds;
	TSet<FName> CueMissing;
	void PlayCue(const TCHAR* Name, float Volume = .5f, float Pitch = 1.f);
	bool Crossed(float Before, float Now, float At) const { return Before < At && Now >= At; }
	float LastPhaseT = -1.f;                      // the game's PhaseT at the last draw (an event fires on the frame it is crossed)
	uint8 SeenPhase = 255;
	int32 DrawCount = 0, PlayDrawn = -10;
	float PlaySince = -100.f;

	// ---- the play HUD's feedback: what changed, and when (the game's real clock)
	float LastArmor = -1.f, ArmorChanged = -10.f, MissileChanged = -10.f, AmmoChanged = -10.f, LoadSwitchAt = -10.f;
	int32 LastMissiles = -1, LastAmmoLoad = -1, LastAmmoRounds = -1;
	float ArmorGhost = 1.f, ArmorHitAt = -10.f, ArmorHitSize = 0.f, ArmorGainAt = -10.f, DamageStateAt = -10.f;
	int32 LastDamageState = -1;
	float MissileFiredAt = -10.f, MissileGainAt = -10.f;
	int32 LastCombo = 1; float ComboUpAt = -10.f;
	int32 LastKills = 0, LastNear = 0; float CountsAt = -10.f;
	double ScoreShown = 0.0; int64 LastScore = 0; float ScoreUpAt = -10.f;
	int32 LastDriftLevel = 0; float DriftUpAt = -10.f;
	bool bLastTurboFull = true; float TurboReadyAt = -10.f;
	bool bLastBossOn = false; float BossOnAt = -10.f, BossGhost = 1.f, LastBossFrac = 1.f, BossHitAt = -10.f, BossPhaseAt = -10.f;
	int32 LockId = 0; float LockAt = -10.f;
	void ResetPlayFeedback(const AFTGameMode& G);
	void DrawMissileLock(const AFTGameMode& Game);

	// ---- results and menus
	float LastCutAge = 1.f;
	int32 LastReelShot = -1;
	bool bLastReelSlow = false; float ReelSlowAt = -10.f;
	int32 PayLeveled = 0; float LevelUpAt = -10.f;
	FString OptionValues[5]; float OptionChangedAt[5] = { -10.f, -10.f, -10.f, -10.f, -10.f };

	UPROPERTY(Transient) TObjectPtr<UFont> FD = nullptr;
	UPROPERTY(Transient) TObjectPtr<UFont> FB = nullptr;
	// 3.0 title: Orbitron Black and Rajdhani Bold (both SIL Open Font License 1.1, see Content/UI/Fonts/OFL-*.txt)
	// and the lettered logo (T_TitleLogo: chrome THROTTLE in Orbitron, neon "Full" in Mr Dafoe, also OFL)
	UPROPERTY(Transient) TObjectPtr<UFont> FOrb = nullptr;
	UPROPERTY(Transient) TObjectPtr<UFont> FRaj = nullptr;
	// the skin's display face: Bungee (SIL Open Font License 1.1, Content/Data/Licenses/OFL-Bungee.txt)
	UPROPERTY(Transient) TObjectPtr<UFont> FBun = nullptr;
	UFont* DisplayFace() const;
	UFont* BodyFace() const;
	UPROPERTY(Transient) TObjectPtr<class UTexture2D> TLogo = nullptr;
	bool bTitleAssets = false;
	FVector2D MeasureF(UFont* F, FStringView Str, float Scale);
	/** A gradient rectangle (A at the top/left, B at the bottom/right). */
	void Grad(float X, float Y, float Wd, float Ht, const FLinearColor& A, const FLinearColor& B, bool bHorizontal);
	void DrawAttractWipe(AFTGameMode* G);
	void DrawAttractOsd(AFTGameMode* G);
	enum class EIcon : uint8 { Repair, Missile, Ammo, Turbo, Record, Warning, Play };
	enum class EControl : uint8 { Idle, Hover, Focus, Pressed, Disabled };
	FBox2D Rect(float X, float Y, float Wd, float Ht) const;
	void Stroke(FVector2D A, FVector2D B, const FLinearColor& Color, float Thickness = 1.f);
	void Icon(EIcon Shape, float X, float Y, float Size, const FLinearColor& Color);
	/** bPaper: a panel that is read at length (cream stock, ink text); otherwise slate. In the race: an ink sticker. */
	void Plate(const FBox2D& R, const FLinearColor& Accent, const FLinearColor& Fill, bool bPaper = false);
	/** Ghost: where the value was a moment ago (drawn behind the value while it drains). */
	void Meter(const FBox2D& R, float Fraction, const FLinearColor& Accent, int32 Segments = 1, float Ghost = -1.f);
	void Print(const FString& Str, float X, float Y, const FLinearColor& Color, float Height, int32 Align = 0, bool bDisplay = false, float MaxWidth = 0.f);
	/** Print's body, on a view of the string (a label written in the code costs no FString on its way here).
	 *  Premeasured: TextSize(Str, Height, bDisplay).X when the caller has just measured it; negative: measured here. */
	void PrintText(FStringView Str, float X, float Y, const FLinearColor& Color, float Height, int32 Align, bool bDisplay, float MaxWidth, float Premeasured = -1.f);
	/** A row of key hints: Print's own arguments; a key cap of the kit stands under each key. */
	void HintRow(const FString& Str, float X, float Y, const FLinearColor& Color, float Height, int32 Align = 0, bool bDisplay = false, float MaxWidth = 0.f);
	FVector2D TextSize(FStringView Str, float Height, bool bDisplay = false);
	FBox2D Control(const FBox2D& R, const FString& Label, const FString& Hint, EControl State, const FLinearColor& Accent, const FLinearColor* LabelColor = nullptr);
	EControl ControlState(const FBox2D& R, bool bFocus = false, bool bEnabled = true) const;
	void Box(float X, float Y, float Wd, float Ht, const FLinearColor& C);
	// ---- the skin ("the studio lot, inked", 15 Nov): what lies under a point this frame, so that text and rows asked
	// for in the colours of a dark panel come out right on paper and on the wall (FTHUD.cpp, Box and Print)
	enum class ETone : uint8 { Dark, Light };
	struct FArea { float X0, Y0, X1, Y1; ETone Tone; };
	TArray<FArea, TInlineAllocator<256>> Areas;
	void MarkTone(const FBox2D& R, ETone Tone);
	ETone ToneAt(float X, float Y) const;
	bool bSticker = false;                        // the race's instruments: ink stickers with a cream keyline
	float WallNow = 0.f; bool bOverWall = false;  // the menus' cyclorama, eased like the dim
	/** A rectangle exactly as asked for (Box is the skinned one). */
	void Flat(float X, float Y, float Wd, float Ht, const FLinearColor& C);
	void Cursor(const FBox2D& R);
	void Marquee(const FBox2D& R);
	void Stripes(float X, float Y, float Wd, float Ht, const FLinearColor& Light, const FLinearColor& Dark);
	void Frame(float X, float Y, float Wd, float Ht, const FLinearColor& C, float Th = 2.f);
	void DrawTitle(AFTGameMode* G);
	bool bLogoSaid = false;   // the title's logo line is logged once
	void DrawCard(AFTGameMode* G);
	void DrawPlay(AFTGameMode* G);
	// ---- the race's stickers and the flow screens (FTHUD.cpp, above DrawPlay): what DrawPlay, DrawCard, DrawCut and
	// DrawReel share on top of FTHUDSkin, and what stands in while a kit texture is missing. Nothing else calls these.
	bool bLastArmorLow = false;
	float ArmorLowAt = -10.f;
	// (15 Nov, the frame-cost pass) The text of these is a view: a label written in the code, a word picked from a
	// table and a string all go in as they are, and no FString is made for a label every frame. Premeasured is
	// RaceTextW(Str, Height, bDisplay) when the caller already has it (negative: not measured yet), so a string
	// whose width the layout needs is measured once a frame, not once more in RacePrint and again in Print.
	float RaceTextW(FStringView Str, float Height, bool bDisplay = false);
	void RacePrint(FStringView Str, float X, float MidY, const FLinearColor& Color, float Height, int32 Align = 0, bool bDisplay = false, float MaxWidth = 0.f, float Premeasured = -1.f);
	float RaceDigits(const FString& Str, float X, float CapTop, float CapH, const FLinearColor& Fill, int32 Align = 0, float MaxWidth = 0.f);
	void RaceChip(float X, float Y, float Wd, float Ht, const FLinearColor& Tint);
	float RaceTab(FStringView Label, float X, float Y, const FLinearColor& Tint, const FLinearColor& TextColor);
	void RaceIcon(int32 Cell, float X, float Y, float Size, const FLinearColor& Tint, int32 Fallback = -1);
	float RaceKey(FStringView Label, float X, float MidY, float Room = 0.f, float HeightKit = 28.f);
	/** A figure the race HUD writes out (the speed, SCENE 2/5, the box office): formatted again only when a value it
	 *  is written from changes, so a frame in which nothing changed formats nothing. A, B, C: those values (a float
	 *  goes in by its bits, FMath::AsUInt). The text is the one the same Printf gave every frame before. */
	struct FKept
	{
		uint64 Key[3] = { 0, 0, 0 };
		bool bMade = false;
		FString Text;
		template <typename MakeType>
		const FString& Get(uint64 A, uint64 B, uint64 C, MakeType&& Make)
		{
			if (!bMade || Key[0] != A || Key[1] != B || Key[2] != C)
			{
				bMade = true; Key[0] = A; Key[1] = B; Key[2] = C;
				Text = Make();
			}
			return Text;
		}
	};
	FKept KeptSpeed, KeptPool, KeptArmorPct, KeptMissileCap, KeptMissiles, KeptRounds, KeptScene, KeptTake, KeptClock, KeptScore, KeptCounts, KeptStunts, KeptCarnage, KeptFilm;
	float RacePips(int32 FullCell, float X, float Y, int32 Have, int32 Total, float Room, float PitchKit, const FLinearColor& Tint, int32 FlashAt = -1, float Flash = 0.f);
	void RaceTile(class UTexture2D* Tex, float X, float Y, float Wd, float Ht, float U0, float V0, float U1, float V1, const FLinearColor& Tint);
	void RaceStick(float X, float Y, const FLinearColor& Tint, bool bFlipY);
	void RaceMattes();
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
	void DrawSystemDialog(AFTGameMode* G);
	void DrawSaveStatus(AFTGameMode* G);
	void DrawControls(AFTGameMode* G);
	void DrawControlGuide(AFTGameMode* G);
	void DrawTutorial(AFTGameMode* G);
	void DrawOptions(AFTGameMode* G);
	// the mouse pointer (FTCursor), drawn last; a Control under it this frame makes it the hover pointer
	void DrawPointer(AFTGameMode* G);
	mutable bool bPointerOverControl = false;
	FTCursor::EState PointerState = FTCursor::EState::Normal;
};
