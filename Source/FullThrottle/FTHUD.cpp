#include "FTHUD.h"
#include "FTHUDStyle.h"
#include "FTGameMode.h"
#include "FTGameInstance.h"
#include "FTInput.h"
#include "FTInputGlyphs.h"
#include "FTPlayerCar.h"
#include "FTEntity.h"
#include "FTPlayerController.h"
#include "FTProgress.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Engine/Engine.h"
#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Math/RotationMatrix.h"
#include "Math/ScaleMatrix.h"
#include "Math/TranslationMatrix.h"
#include "Sound/SoundBase.h"

// THE STUDIO LOT, INKED (the style ruling of 15 Nov 2026). The palette lives here and not in FTHUDStyle.h: the
// brawl's HUD shares that header and this pass may not change it. The screens still ask for the names they always
// asked for (Ink, Steel, Active, Rule, White, Grey, the five signals); what each name IS has changed, and Box, Frame,
// Plate, Control, Print and Meter decide what it becomes on a slate panel, on paper, on the wall and in the race.
namespace FTLot
{
	// (the layout and easing tokens of FTHUDStyle.h, exactly as they are)
	using FTHUDStyle::CanvasHeight; using FTHUDStyle::Safe; using FTHUDStyle::Inset; using FTHUDStyle::Gap; using FTHUDStyle::Corner;
	using FTHUDStyle::IconSize; using FTHUDStyle::LabelSize; using FTHUDStyle::Body; using FTHUDStyle::Strong; using FTHUDStyle::Heading;
	using FTHUDStyle::Readout; using FTHUDStyle::Enter; using FTHUDStyle::Feedback; using FTHUDStyle::MenuEntrance; using FTHUDStyle::FocusSweep;
	using FTHUDStyle::RowStagger; using FTHUDStyle::ClapperClose; using FTHUDStyle::ClapperTravel; using FTHUDStyle::Alpha; using FTHUDStyle::EaseOut;

	inline FLinearColor LotHex(uint32 Rgb) { return FLinearColor(FColor(uint8(Rgb >> 16), uint8(Rgb >> 8), uint8(Rgb))); }
	const FLinearColor Ink = LotHex(0x16121E);          // outlines, hard shadows, the race's stickers, text on paper
	const FLinearColor Deep = LotHex(0x110E18);         // the dark behind a held picture, the letterbox
	const FLinearColor SlateFill = LotHex(0x1F1C30);    // a panel that holds a list or a board
	const FLinearColor Steel = LotHex(0x423D5E);        // a row, an idle button
	const FLinearColor Track = LotHex(0x2B2740);        // the empty part of a meter
	const FLinearColor Rule = LotHex(0x6F688C);         // a divider on slate (an ink line on paper, an ink outline as a frame)
	const FLinearColor Paper = LotHex(0xF4E8CE);        // a panel that is read at length
	const FLinearColor Active = LotHex(0xFFF6DC);       // "the chosen one" as the screens ask for it: lit paper
	const FLinearColor PaperRow = LotHex(0xE6D8B6);     // a row on paper: a shade of the stock
	const FLinearColor PaperTrack = LotHex(0xD3C5A2);
	const FLinearColor Aqua = LotHex(0xCBF0E9);         // the chosen row on paper
	const FLinearColor Sticky = LotHex(0xFFE078);
	const FLinearColor White = LotHex(0xF8F3E7);        // chalk
	const FLinearColor Grey = LotHex(0xB8B1C9);
	const FLinearColor InkSoft = LotHex(0x4A4460);      // second-rank text on paper
	// the signals keep their meaning
	const FLinearColor Cyan = LotHex(0x49E2D5);         // the player's systems
	const FLinearColor Yellow = LotHex(0xFFC44C);       // money and score (the ruling's amber)
	const FLinearColor Amber = LotHex(0xFF9340);        // caution: armour worn, a save that needs attention
	const FLinearColor Red = LotHex(0xEE3C3E);          // danger
	const FLinearColor Pink = LotHex(0xFF4FA3);         // focus, combo, finale
	const FLinearColor Green = LotHex(0x76EAA0);        // armour nominal
	const FLinearColor Lilac = LotHex(0xB7A6D9);        // the menus' wall, top
	const FLinearColor Peach = LotHex(0xF6CBA6);        // ... and bottom
	const FLinearColor Ledger = LotHex(0xE4EEDA);       // a ledger's stock
	const FLinearColor LedgerBand = LotHex(0xC9E2C6);   // ... and its green bands
}
using namespace FTLot;

// Motion (the overhaul of 15 Nov; the owner: "expressive entrances and responsive focus, with restrained motion in
// Options and the Skill Tree"). The layout, the wording and every hit box are where they were: a block that arrives is
// drawn through a canvas offset and a fade, its rectangle for the mouse is the final one from the first frame, and
// nothing here waits for an animation before a press is taken. REDUCED MOTION switches all of it off.
// (These would sit with the other easing tokens in FTHUDStyle.h; they are here because that header was not this
// pass's to change.)
namespace FTHUDMotion
{
	constexpr float DealTime = .26f;        // a major menu's panel arriving
	constexpr float DealStep = .03f;        // between rows or cards dealt one after another
	constexpr float QuietTime = .14f;       // OPTIONS, CONTROLS and the skill tree: a short fade, no travel
	constexpr float FocusRate = 18.f;       // the focused item's lean, per second (most of the way in a tenth of a second)
	constexpr float FocusRelease = 11.f;    // the item that lost the focus easing back
	constexpr float FocusFlashTime = .16f;  // the newly focused item's brightening
	constexpr float PunchTime = .20f;       // a value that just changed
	constexpr float ShakeTime = .28f;       // a panel that was hit
	constexpr float DimRate = 9.f;          // the dim behind a screen, eased from the last screen's
	constexpr float MatteRate = 11.f;       // the letterbox mattes
	constexpr float CountTime = .22f;       // a money line counting up
	constexpr float TotalTime = .55f;       // a total counting up
	constexpr float StampTime = .13f;       // the rating stamp coming down
	constexpr float GhostHold = .28f;       // armour just lost stays on the bar this long, then drains
}
using namespace FTHUDMotion;

namespace
{
	FString Num(int64 V)
	{
		FString R = FString::Printf(TEXT("%lld"), V);
		for (int32 i = R.Len() - 3; i > (V < 0 ? 1 : 0); i -= 3) R.InsertAt(i, TEXT(","));
		return R;
	}
	FString Money(int64 V) { return V < 0 ? TEXT("-$") + Num(V).RightChop(1) : TEXT("$") + Num(V); }
	FString InputHint(const UObject* Context, const TCHAR* Keyboard, const FKey& Pad)
	{
		const UFTInputSubsystem* Input = UFTInputSubsystem::Get(Context);
		return Input && Input->UsesGamepad() ? Input->KeyLabel(Pad) : FString(Keyboard);
	}
	FString ConfirmHint(const UObject* Context) { return InputHint(Context, TEXT("ENTER"), EKeys::Gamepad_FaceButton_Bottom); }
	FString BackHint(const UObject* Context) { return InputHint(Context, TEXT("ESC"), EKeys::Gamepad_FaceButton_Right); }

	FString Binding(const UFTGameInstance* GI, EFTAction Action)
	{
		if (!GI) return TEXT("UNBOUND");
		const FKey& Key = GI->Binds[static_cast<int32>(Action)][GI->Layout];
		return Key.IsValid() ? Key.GetDisplayName().ToString().ToUpper() : FString(TEXT("UNBOUND"));
	}

	int32 FocusKey(int32 View, int32 Page, int32 Item) { return View * 1000000 + (Page + 1) * 10000 + Item; }

	FLinearColor Mix(const FLinearColor& A, const FLinearColor& B, float Amount) { return FMath::Lerp(A, B, FMath::Clamp(Amount, 0.f, 1.f)); }

	// ---- the skin
	bool Same(const FLinearColor& A, const FLinearColor& B) { return FMath::Abs(A.R - B.R) < .002f && FMath::Abs(A.G - B.G) < .002f && FMath::Abs(A.B - B.B) < .002f; }
	float Lum(const FLinearColor& C) { return C.R * .2126f + C.G * .7152f + C.B * .0722f; }
	/** A colour that was chosen for a dark panel, as ink on paper: chalk is ink, grey is soft ink, and a signal keeps its
	 *  hue at an ink's weight (cyan a dark teal, money a brown ink, danger a dark red). */
	FLinearColor InkFor(const FLinearColor& C)
	{
		if (Same(C, White)) return FLinearColor(Ink.R, Ink.G, Ink.B, C.A);
		if (Same(C, Grey)) return FLinearColor(InkSoft.R, InkSoft.G, InkSoft.B, C.A);
		const float L = Lum(C);
		if (L < .08f) return C;
		const float K = .055f / L;
		return FLinearColor(C.R * K, C.G * K, C.B * K, C.A);
	}

	float EaseIn(float Age, float Duration)
	{
		const float U = FMath::Clamp(Age / Duration, 0.f, 1.f);
		return U * U * U;
	}
}

/** A rectangle exactly as asked for. (Box is the skinned one.) */
void AFTHUD::Flat(float X, float Y, float Wd, float Ht, const FLinearColor& C)
{
	FCanvasTileItem It(FVector2D(X, Y), FVector2D(Wd, Ht), Fade < 1.f ? Alpha(C, Fade) : C);
	It.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(It);
}

void AFTHUD::MarkTone(const FBox2D& R, ETone Tone)
{
	Areas.Add(FArea{ float(R.Min.X), float(R.Min.Y), float(R.Max.X), float(R.Max.Y), Tone });
}

/** What lies under a point: the last panel, row or wall drawn there this frame. Nothing (the live picture): dark. */
AFTHUD::ETone AFTHUD::ToneAt(float X, float Y) const
{
	for (int32 I = Areas.Num() - 1; I >= 0; --I)
	{
		const FArea& A = Areas[I];
		if (X >= A.X0 && X < A.X1 && Y >= A.Y0 && Y < A.Y1) return A.Tone;
	}
	return ETone::Dark;
}

void AFTHUD::Box(float X, float Y, float Wd, float Ht, const FLinearColor& C)
{
	// The screens ask for a row, the chosen row, a rule, a track, as they always did; what each one is depends on
	// what it lies on. The chosen row is lit paper on slate, aqua on paper and the player's cyan in the race; on paper
	// a row is a shade of the stock, a rule an ink line, and a flash of white a flash of ink.
	const bool bOnLight = ToneAt(X + Wd * .5f, Y + Ht * .5f) == ETone::Light;
	FLinearColor Use = C;
	if (Same(C, Active)) Use = bSticker ? Cyan : bOnLight ? Aqua : Active;
	else if (bOnLight)
	{
		if (Same(C, Steel)) Use = PaperRow;
		else if (Same(C, Track)) Use = PaperTrack;
		else if (Same(C, Rule)) Use = InkSoft;
		else if (Same(C, White)) Use = Ink;
	}
	Use.A = C.A;
	Flat(X, Y, Wd, Ht, Use);
	if (Use.A >= .5f && Wd >= 20.f * S && Ht >= 10.f * S) MarkTone(FBox2D(FVector2D(X, Y), FVector2D(X + Wd, Y + Ht)), Lum(Use) > .35f ? ETone::Light : ETone::Dark);
}

/** The focus cursor: a thick pink frame with a hard shadow (6 px at 1080), inside the item's own rectangle. */
void AFTHUD::Cursor(const FBox2D& R)
{
	const float X = R.Min.X, Y = R.Min.Y, Wd = R.GetSize().X, Ht = R.GetSize().Y;
	const float Th = FMath::Min(4.f * S, Ht * .2f), Off = 2.f * S;
	Flat(X + Wd, Y + Off, Off, Ht, Ink);
	Flat(X + Off, Y + Ht, Wd - Off, Off, Ink);
	Flat(X, Y, Wd, Th, Pink); Flat(X, Y + Ht - Th, Wd, Th, Pink);
	Flat(X, Y + Th, Th, Ht - 2 * Th, Pink); Flat(X + Wd - Th, Y + Th, Th, Ht - 2 * Th, Pink);
}

void AFTHUD::Frame(float X, float Y, float Wd, float Ht, const FLinearColor& C, float Th)
{
	// what was a thin cyan outline round the chosen row is the pink cursor now (not in the race, and not an icon's box)
	if (!bSticker && Same(C, Cyan) && C.A > .9f && Th <= 2.f && Wd >= 40.f * S && Ht >= 16.f * S)
	{
		Cursor(Rect(X, Y, Wd, Ht));
		return;
	}
	Th = FMath::Max(Th, 1.34f) * S;   // no line under 2 px at 1080
	const FLinearColor Use = Same(C, Rule) ? FLinearColor(Ink.R, Ink.G, Ink.B, C.A) : C;   // a rule round something is its ink outline
	Flat(X, Y, Wd, Th, Use); Flat(X, Y + Ht - Th, Wd, Th, Use);
	Flat(X, Y + Th, Th, Ht - 2 * Th, Use); Flat(X + Wd - Th, Y + Th, Th, Ht - 2 * Th, Use);
}

FBox2D AFTHUD::Rect(float X, float Y, float Wd, float Ht) const
{
	return FBox2D(FVector2D(X, Y), FVector2D(X + Wd, Y + Ht));
}

void AFTHUD::Stroke(FVector2D A, FVector2D B, const FLinearColor& Color, float Thickness)
{
	if (Fade < 1.f)
	{
		// The engine draws canvas lines without blending (BatchedElements draws them SE_BLEND_Opaque), so a line cannot
		// fade: icons, the speed ticks and a plate's cut corner stood at full strength while their panel came or went.
		// While a block is arriving or leaving, the stroke is two triangles, which do blend; at rest it is the line
		// it always was.
		if (Fade <= .004f) return;
		const FVector2D Along = (B - A).GetSafeNormal();
		const FVector2D Across = FVector2D(-Along.Y, Along.X) * (FMath::Max(Thickness * S, 1.f) * .5f);
		const FLinearColor Faded = Alpha(Color, Fade);
		FCanvasTriangleItem One(A + Across, A - Across, B + Across, GWhiteTexture);
		FCanvasTriangleItem Two(A - Across, B - Across, B + Across, GWhiteTexture);
		One.SetColor(Faded); Two.SetColor(Faded);
		One.BlendMode = SE_BLEND_Translucent; Two.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(One); Canvas->DrawItem(Two);
		return;
	}
	FCanvasLineItem Line(A, B);
	Line.SetColor(Color);
	Line.LineThickness = Thickness * S;
	Canvas->DrawItem(Line);
}

void AFTHUD::Plate(const FBox2D& R, const FLinearColor& Accent, const FLinearColor& Fill, bool bPaper)
{
	// The studio lot, inked: a hard offset shadow, an ink outline (3 px at 1080) and a flat fill; no glass, no cut
	// corner. A panel that holds a list or a board is slate; one that is read at length is paper (bPaper). In the race
	// a plate is an ink sticker with a cream keyline, and its shadow stays inside the footprint it always had, so the
	// instruments cover no more road than they did.
	const float Out = 2.f * S, Drop = (bSticker ? 3.f : 3.5f) * S;
	const float X = R.Min.X, Y = R.Min.Y;
	const float Wd = R.GetSize().X - (bSticker ? Drop : 0.f), Ht = R.GetSize().Y - (bSticker ? Drop : 0.f);
	Flat(X + Drop, Y + Drop, Wd, Ht, bSticker ? Alpha(Ink, .55f) : Ink);
	Flat(X, Y, Wd, Ht, Ink);
	const FLinearColor Stock = !Same(Fill, Ink) ? Fill : bSticker ? Ink : bPaper ? Paper : SlateFill;
	Flat(X + Out, Y + Out, Wd - 2 * Out, Ht - 2 * Out, Stock);
	MarkTone(Rect(X, Y, Wd, Ht), Lum(Stock) > .35f ? ETone::Light : ETone::Dark);
	if (bSticker)
	{
		const float Key = 1.34f * S, Kx = X + Out, Ky = Y + Out, Kw = Wd - 2 * Out, Kh = Ht - 2 * Out;
		Flat(Kx, Ky, Kw, Key, Paper); Flat(Kx, Ky + Kh - Key, Kw, Key, Paper);
		Flat(Kx, Ky + Key, Key, Kh - 2 * Key, Paper); Flat(Kx + Kw - Key, Ky + Key, Key, Kh - 2 * Key, Paper);
	}
	// the accent that says whose panel it is: a tab of its colour at the top left
	Flat(X + Out, Y + Out, FMath::Min(Wd * .34f, 72 * S), 4.f * S, Accent);
}

UFont* AFTHUD::DisplayFace() const
{
	return FBun ? FBun.Get() : FOrb ? FOrb.Get() : FD.Get();
}

UFont* AFTHUD::BodyFace() const
{
	return FRaj ? FRaj.Get() : FB.Get();
}

FVector2D AFTHUD::TextSize(const FString& Str, float Height, bool bDisplay)
{
	Height = FMath::Max(Height, bDisplay ? 15.9f : 15.4f);
	return MeasureF(bDisplay ? DisplayFace() : BodyFace(), Str, Height / (bDisplay ? BunHeight : RajHeight));
}

void AFTHUD::Print(const FString& Str, float X, float Y, const FLinearColor& Color, float Height, int32 Align, bool bDisplay, float MaxWidth)
{
	if (Str.IsEmpty()) return;
	// Bungee for titles, buttons, plaques and the big numerals (what the screens call "display"), Rajdhani Bold for
	// body and labels. The ruling's floor: no text that carries information under 18 px at 1080.
	UFont* Font = bDisplay ? DisplayFace() : BodyFace();
	Height = FMath::Max(Height, bDisplay ? 15.9f : 15.4f);
	float Scale = Height / (bDisplay ? BunHeight : RajHeight);
	float Measured = Align != 0 || MaxWidth > 0.f ? MeasureF(Font, Str, Scale).X : 0.f;
	if (MaxWidth > 0.f && Measured > MaxWidth)
	{
		Scale *= MaxWidth / Measured;
		Measured = MaxWidth;
	}
	if (Align != 0) X -= Align == 1 ? Measured * .5f : Measured;
	// a colour chosen for a dark panel is ink on paper and on the wall
	const bool bOnLight = ToneAt(X + (Measured > 0.f ? Measured * .5f : 6.f * S), Y + Height * S * .5f) == ETone::Light;
	const FLinearColor Use = bOnLight ? InkFor(Color) : Color;
	if (bDisplay && !bSticker && Height >= Heading - .5f && Use.A > .5f)
	{
		// a title is printed twice: the pink plate a little off register, under the ink or the chalk
		FCanvasTextItem Under(FVector2D(X + 2.f * S, Y + 2.f * S), FText::FromString(Str), Font, Alpha(Pink, Use.A * Fade));
		Under.Scale = FVector2D(Scale * S);
		Canvas->DrawItem(Under);
	}
	FCanvasTextItem Item(FVector2D(X, Y), FText::FromString(Str), Font, Fade < 1.f ? Alpha(Use, Fade) : Use);
	Item.Scale = FVector2D(Scale * S);
	Canvas->DrawItem(Item);
}

void AFTHUD::Meter(const FBox2D& R, float Fraction, const FLinearColor& Accent, int32 Segments, float Ghost)
{
	// chunky segments, each in its ink outline; a hairline meter (under 10 px at 1080) has no room for one and keeps
	// to a flat track
	const float Ht = R.GetSize().Y;
	const bool bChunky = Ht >= 6.5f * S;
	const float GapPx = Segments > 1 ? (bChunky ? 2.f : 3.f) * S : 0.f;
	const float Pitch = (R.GetSize().X + GapPx) / Segments;
	const float Wd = Pitch - GapPx;
	const float Edge = bChunky ? FMath::Max(1.f, 1.34f * S) : 0.f;
	const FLinearColor Empty = ToneAt(R.Min.X + 2.f * S, R.Min.Y + Ht * .5f) == ETone::Light ? PaperTrack : Track;
	const float Filled = FMath::Clamp(Fraction, 0.f, 1.f) * Segments;
	const float Trail = FMath::Clamp(Ghost, 0.f, 1.f) * Segments;
	for (int32 Index = 0; Index < Segments; ++Index)
	{
		const float X = R.Min.X + Index * Pitch;
		if (bChunky) Flat(X, R.Min.Y, Wd, Ht, Ink);
		const float Ix = X + Edge, Iy = R.Min.Y + Edge, Iw = Wd - 2 * Edge, Ih = Ht - 2 * Edge;
		Flat(Ix, Iy, Iw, Ih, Empty);
		const float Value = FMath::Clamp(Filled - Index, 0.f, 1.f);
		// what was there a moment ago, behind what is there now
		const float Was = FMath::Clamp(Trail - Index, 0.f, 1.f);
		if (Was > Value) Flat(Ix + Iw * Value, Iy, Iw * (Was - Value), Ih, Alpha(White, .85f));
		if (Value > 0.f) Flat(Ix, Iy, Iw * Value, Ih, Accent);
	}
}

void AFTHUD::Icon(EIcon Shape, float X, float Y, float Size, const FLinearColor& Color)
{
	const auto Dot = [&](float U, float V) { return FVector2D(X + U * Size, Y + V * Size); };
	const auto Line = [&](float A, float B, float C, float D) { Stroke(Dot(A, B), Dot(C, D), Color, 1.6f); };
	switch (Shape)
	{
	case EIcon::Repair:
		Frame(X + Size * 0.05f, Y + Size * 0.36f, Size * 0.9f, Size * 0.59f, Color, 1.5f);
		Line(0.32f, .36f, .32f, .13f); Line(.32f, .13f, .68f, .13f); Line(.68f, .13f, .68f, .36f);
		Line(.30f, .81f, .67f, .50f); Line(.58f, .50f, .73f, .50f); Line(.73f, .50f, .71f, .65f);
		break;
	case EIcon::Missile:
		Line(.08f, .87f, .32f, .63f); Line(.32f, .63f, .43f, .30f); Line(.43f, .30f, .93f, .07f);
		Line(.93f, .07f, .70f, .57f); Line(.70f, .57f, .37f, .68f); Line(.37f, .68f, .13f, .92f);
		break;
	case EIcon::Ammo:
		for (int32 I = 0; I < 3; ++I)
		{
			const float U = .14f + I * .29f;
			Line(U, .82f, U, .38f); Line(U, .38f, U + .09f, .15f); Line(U + .09f, .15f, U + .18f, .38f);
			Line(U + .18f, .38f, U + .18f, .82f); Line(U, .82f, U + .18f, .82f);
		}
		break;
	case EIcon::Turbo:
		Line(.52f, .08f, .16f, .56f); Line(.16f, .56f, .51f, .56f);
		Line(.51f, .56f, .41f, .93f); Line(.41f, .93f, .83f, .39f); Line(.83f, .39f, .49f, .39f);
		break;
	case EIcon::Record:
	{
		FCanvasNGonItem Light(Dot(.5f, .5f), FVector2D(Size * .28f), 12, Fade < 1.f ? Alpha(Color, Fade) : Color);
		Light.BlendMode = SE_BLEND_Translucent;   // a canvas item is opaque unless told: the light could neither fade nor blink
		Canvas->DrawItem(Light);
		break;
	}
	case EIcon::Warning:
		Line(.5f, .06f, .95f, .87f); Line(.95f, .87f, .05f, .87f); Line(.05f, .87f, .5f, .06f);
		Line(.5f, .38f, .5f, .59f); Line(.5f, .70f, .5f, .74f);
		break;
	case EIcon::Play:
		Line(.28f, .14f, .82f, .5f); Line(.82f, .5f, .28f, .86f); Line(.28f, .86f, .28f, .14f);
		break;
	}
}

AFTHUD::EControl AFTHUD::ControlState(const FBox2D& R, bool bFocus, bool bEnabled) const
{
	if (!bEnabled) return EControl::Disabled;
	const AFTPlayerController* PC = Cast<AFTPlayerController>(GetOwningPlayerController());
	if (PC && PC->bMenuMouse && !PC->UsesGamepadPrompts() && PC->MouseIn(R))
		return PC->IsInputKeyDown(EKeys::LeftMouseButton) ? EControl::Pressed : EControl::Hover;
	return bFocus ? EControl::Focus : EControl::Idle;
}

FBox2D AFTHUD::Control(const FBox2D& R, const FString& Label, const FString& Hint, EControl State, const FLinearColor& Accent, const FLinearColor* LabelColor)
{
	// The studio lot, inked. Idle: a slate plate with a clapper-stripe end. Hover or focus: lit paper with ink text, a
	// pink print shadow and an amber stripe. Pressed: it sinks onto its shadow (4 px at 1080). Disabled: dimmed.
	// The rectangle handed back (the hit box) is the one handed in.
	const bool bEnabled = State != EControl::Disabled;
	const bool bHot = State == EControl::Hover || State == EControl::Focus || State == EControl::Pressed;
	const float Out = 2.f * S, Drop = 3.5f * S, Sink = State == EControl::Pressed ? 2.7f * S : 0.f;
	const FVector2D Size = R.GetSize();
	const float X = R.Min.X + Sink, Y = R.Min.Y + Sink;
	Flat(R.Min.X + Drop, R.Min.Y + Drop, Size.X, Size.Y, bHot ? Pink : Ink);
	Flat(X, Y, Size.X, Size.Y, Ink);
	const FLinearColor Stock = !bEnabled ? Mix(Steel, SlateFill, .65f) : bHot ? Active : Steel;
	Flat(X + Out, Y + Out, Size.X - 2 * Out, Size.Y - 2 * Out, Stock);
	MarkTone(Rect(X, Y, Size.X, Size.Y), bEnabled && bHot ? ETone::Light : ETone::Dark);
	float StripeW = 0.f;
	if (Size.X >= 96.f * S)
	{
		// the clapper's stripes down the left end: chalk and ink, amber and ink when it is the chosen one
		StripeW = FMath::Min(11.f * S, float(Size.X) * .08f);
		const int32 Steps = 4;
		const float StepH = (Size.Y - 2 * Out) / Steps;
		const FLinearColor Light = !bEnabled ? Grey : bHot ? Yellow : White;
		for (int32 I = 0; I < Steps; ++I) Flat(X + Out, Y + Out + I * StepH, StripeW, StepH, I % 2 == 0 ? Light : Ink);
		Flat(X + Out + StripeW, Y + Out, 1.34f * S, Size.Y - 2 * Out, Ink);
		StripeW += 1.34f * S;
	}
	const float TextW = Size.X - 2 * Out - StripeW;
	const float Mid = X + Out + StripeW + TextW * .5f;
	Print(Label, Mid, Y + (Hint.IsEmpty() ? Size.Y * .24f : 5 * S), !bEnabled ? Alpha(Grey, .7f) : LabelColor ? *LabelColor : White,
		Strong, 1, true, TextW - 16 * S);
	Print(Hint, Mid, Y + Size.Y - 19 * S, bHot ? Accent : Grey, LabelSize, 1, false, TextW - 16 * S);
	return R;
}

void AFTHUD::UpdateMenuMotion(const AFTGameMode& G)
{
	EMenuView View = EMenuView::None;
	int32 Page = INDEX_NONE, Selection = INDEX_NONE;
	if (!bReducedMotion)
	{
		switch (G.Overlay)
		{
		case EFTOverlay::Pause:
			View = EMenuView::Pause;
			Selection = G.PauseRow;
			break;
		case EFTOverlay::System:
			View = EMenuView::Pause; Page = int32(G.SystemDialog) + 1; Selection = G.SystemRow;
			break;
		case EFTOverlay::Controls:
			View = EMenuView::Controls;
			Selection = (G.ControlsRow * 2 + G.ControlsCol) * 2 + int32(G.bControlsWaiting);
			break;
		case EFTOverlay::Options:
			View = EMenuView::Options;
			Selection = G.OptionsRow;
			break;
		case EFTOverlay::None:
			if (G.bAttract || G.Phase == EFTPhase::Title)
			{
				View = EMenuView::Title;
				Page = int32(G.TitleActionFocus != INDEX_NONE);
				Selection = Page ? G.TitleActionFocus : G.TitleSel;
			}
			else if (G.Phase == EFTPhase::PreProduction)
			{
				// the skill tree is its own view: it moves less than the departments (see DrawPreProduction)
				View = G.Tab == EFTTab::Skills ? EMenuView::Skills : EMenuView::Department;
				Page = static_cast<int32>(G.Tab);
				Selection = G.SelIndex();
			}
			else if (G.Phase == EFTPhase::Reel)
			{
				View = EMenuView::Reel;
				Selection = G.bReelDone ? INDEX_NONE : G.ReelShot;
			}
			break;
		}
	}
	const bool bOpened = View != MotionView || Page != MotionPage;
	if (bOpened) ViewEnteredAt = T;
	if (bOpened || Selection != MotionSelection) SelectionChangedAt = T;
	// a view that has just opened shows its focus where it is: only a move made on the screen eases
	if (View != MotionView) FocusState.Reset();
	MotionView = View;
	MotionPage = Page;
	MotionSelection = Selection;
}

float AFTHUD::MenuAge(EMenuView View, bool bSelection) const
{
	return MotionView == View ? T - (bSelection ? SelectionChangedAt : ViewEnteredAt) : -1.f;
}

void AFTHUD::AccentSweep(const FBox2D& Lane, float Age, const FLinearColor& Accent, float Duration)
{
	if (bReducedMotion || Age < 0.f || Age >= Duration) return;

	// Move a short light along an edge. The clip stays inside the panel and never changes its hit box.
	const float Travel = Lane.GetSize().X;
	const float Length = FMath::Min(Travel * .42f, 115.f * S);
	const float Progress = FMath::InterpEaseOut(0.f, 1.f, Age / Duration, 1.6f);
	const float Head = Lane.Min.X + (Travel + Length) * Progress;
	const float Start = FMath::Max(Lane.Min.X, Head - Length);
	const float End = FMath::Min(Lane.Max.X, Head);
	if (End <= Start) return;

	const float Opacity = 1.f - Age / Duration;
	const float Height = Lane.GetSize().Y;
	Box(Start, Lane.Min.Y, End - Start, Height, Alpha(Accent, .6f * Opacity));
	const float Tip = FMath::Min(7.f * S, End - Start);
	Box(End - Tip, Lane.Min.Y, Tip, Height, Alpha(White, .72f * Opacity));
}

/** A marquee: an ink board ringed with bulbs that chase at three steps a second (and stand still with REDUCED MOTION).
 *  (TODO kit: FTSkin::Marquee draws the bulbs as textures.) */
void AFTHUD::Marquee(const FBox2D& R)
{
	const float X = R.Min.X, Y = R.Min.Y, Wd = R.GetSize().X, Ht = R.GetSize().Y;
	Flat(X, Y, Wd, Ht, Ink);
	MarkTone(R, ETone::Dark);
	const int32 Chase = bReducedMotion ? 0 : FMath::FloorToInt(T * 3.f);
	const float Lamp = 3.4f * S, Edge = 3.f * S;
	const int32 Across = FMath::Max(2, FMath::FloorToInt((Wd - 2 * Edge) / (12.f * S)));
	const int32 Down = FMath::Max(1, FMath::FloorToInt((Ht - 2 * Edge) / (12.f * S)));
	for (int32 I = 0; I <= Across; ++I)
	{
		const float Lx = X + Edge + (Wd - 2 * Edge - Lamp) * I / Across;
		Flat(Lx, Y + Edge, Lamp, Lamp, (I + Chase) % 3 == 0 ? White : Yellow);
		Flat(Lx, Y + Ht - Edge - Lamp, Lamp, Lamp, (I + Chase + 1) % 3 == 0 ? White : Yellow);
	}
	for (int32 I = 1; I < Down; ++I)
	{
		const float Ly = Y + Edge + (Ht - 2 * Edge - Lamp) * I / Down;
		Flat(X + Edge, Ly, Lamp, Lamp, (I + Chase + 2) % 3 == 0 ? White : Yellow);
		Flat(X + Wd - Edge - Lamp, Ly, Lamp, Lamp, (I + Chase) % 3 == 0 ? White : Yellow);
	}
}

void AFTHUD::FocusAccent(const FBox2D& Button, float Age, const FLinearColor& Accent)
{
	// (the skin) the chosen item wears the cursor for as long as it is the chosen one: a thick pink frame with a hard
	// shadow. The light that ran once along its edge went with the thin brackets. (Age below 0: not armed yet.)
	(void)Accent;
	if (Age >= 0.f || bReducedMotion) Cursor(Button);
}

// ---------------------------------------------------------------------------------------------- motion
/** A block that arrives. In 0: (Dx, Dy) away and clear; In 1: in place and solid. (Ox, Oy) moves it regardless (a jolt). */
void AFTHUD::BeginBlock(float In, float Dx, float Dy, float Ox, float Oy)
{
	In = FMath::Clamp(In, 0.f, 1.f);
	FBlockState State{ Fade, false };
	if (In < 1.f) Fade *= In;
	// whole pixels: text that slides on half pixels shimmers
	const float X = FMath::RoundToFloat(Dx * (1.f - In) + Ox), Y = FMath::RoundToFloat(Dy * (1.f - In) + Oy);
	if (X != 0.f || Y != 0.f)
	{
		Canvas->Canvas->PushRelativeTransform(FTranslationMatrix(FVector(X, Y, 0.f)));
		State.bPushed = true;
	}
	Blocks.Add(State);
}

void AFTHUD::EndBlock()
{
	if (Blocks.IsEmpty()) return;
	const FBlockState State = Blocks.Pop();
	if (State.bPushed) Canvas->Canvas->PopTransform();
	Fade = State.Fade;
}

struct AFTHUD::FBlock
{
	AFTHUD& Hud;
	FBlock(AFTHUD& InHud, float In, float Dx = 0.f, float Dy = 0.f, float Ox = 0.f, float Oy = 0.f) : Hud(InHud) { Hud.BeginBlock(In, Dx, Dy, Ox, Oy); }
	~FBlock() { Hud.EndBlock(); }
	FBlock(const FBlock&) = delete;
	FBlock& operator=(const FBlock&) = delete;
};

/** Everything drawn while one of these lives is scaled and turned about a point (a stamp coming down, the clapper's arm). */
struct AFTHUD::FPivot
{
	AFTHUD& Hud;
	bool bPushed = false;
	FPivot(AFTHUD& InHud, const FVector2D& Pivot, float Scale, float Degrees = 0.f) : Hud(InHud)
	{
		if (FMath::IsNearlyEqual(Scale, 1.f, .001f) && FMath::IsNearlyZero(Degrees, .01f)) return;
		const FVector P(Pivot.X, Pivot.Y, 0.f);
		Hud.Canvas->Canvas->PushRelativeTransform(FTranslationMatrix(-P) * FRotationMatrix(FRotator(0.f, Degrees, 0.f))
			* FScaleMatrix(FVector(Scale, Scale, 1.f)) * FTranslationMatrix(P));
		bPushed = true;
	}
	~FPivot() { if (bPushed) Hud.Canvas->Canvas->PopTransform(); }
	FPivot(const FPivot&) = delete;
	FPivot& operator=(const FPivot&) = delete;
};

/** 0 to 1 over Duration, starting Delay into Age. 1 when nothing should move (Age below 0, or REDUCED MOTION). */
float AFTHUD::In01(float Age, float Delay, float Duration) const
{
	if (bReducedMotion || Age < 0.f) return 1.f;
	return EaseOut(Age - Delay, Duration);
}

/** 1 falling to 0 over Duration from Age 0: something that just happened. */
float AFTHUD::Flash01(float Age, float Duration) const
{
	if (bReducedMotion || Age < 0.f || Age >= Duration) return 0.f;
	const float Rest = 1.f - Age / Duration;
	return Rest * Rest;
}

float AFTHUD::ShakeX(float Age, float Pixels, float Duration) const
{
	if (bReducedMotion || Age < 0.f || Age >= Duration) return 0.f;
	const float Rest = 1.f - Age / Duration;
	return FMath::Sin(Age * 2.f * PI * 17.f) * Pixels * Rest * Rest;
}

/** The focused item leans in at once and the one that lost the focus eases back. 0 to 1. */
float AFTHUD::FocusLevel(int32 Key, bool bFocused)
{
	const float Target = bFocused ? 1.f : 0.f;
	if (bReducedMotion) return 0.f;   // (nothing leans: the focus is shown by the bar and the frame alone)
	float* Level = FocusState.Find(Key);
	if (!Level) { FocusState.Add(Key, Target); return Target; }
	*Level = FMath::FInterpTo(*Level, Target, Dt, bFocused ? FocusRate : FocusRelease);
	return *Level;
}

/** The newly focused item brightens for a moment. Not when the screen has only just opened: its entrance is the motion then. */
float AFTHUD::FocusFlash(EMenuView View) const
{
	if (MotionView != View || SelectionChangedAt - ViewEnteredAt < .02f) return 0.f;
	return Flash01(T - SelectionChangedAt, FocusFlashTime);
}

float AFTHUD::ReelBar() const
{
	return FMath::Max((H - W / 2.39f) * .5f, 88 * S);
}

/** The dim behind a screen and the letterbox mattes: each one eased state shared by every screen, so a screen hands
 *  over to the next instead of cutting. Every screen calls this once, before it draws anything else. A menu that
 *  stands on its own (bWall: THE DAILIES, the PAYCHECK, PRE-PRODUCTION) stands on a lit cyclorama, dusty lilac at
 *  the top to peach at the bottom, eased in the same way; the title, the slate, the cut and the reel keep the picture. */
void AFTHUD::Backdrop(float Dim, float MattePx, bool bWall)
{
	const float Faded = Fade;   // (the instruments may be fading under CUT!; the scene behind them is not)
	Fade = 1.f;
	DimNow = DimNow < 0.f || bReducedMotion ? Dim : FMath::FInterpTo(DimNow, Dim, Dt, DimRate);
	if (DimNow > .004f) Box(0, 0, W, H, Alpha(Deep, DimNow));
	WallNow = bReducedMotion ? (bWall ? 1.f : 0.f) : FMath::FInterpTo(WallNow, bWall ? 1.f : 0.f, Dt, DimRate);
	if (WallNow > .004f)
	{
		Grad(0, 0, W, H, Alpha(Lilac, WallNow), Alpha(Peach, WallNow), false);
		if (WallNow >= .5f) MarkTone(Rect(0, 0, W, H), ETone::Light);
	}
	MatteNow = MatteNow < 0.f || bReducedMotion ? MattePx : FMath::FInterpTo(MatteNow, MattePx, Dt, MatteRate);
	if (MatteNow > .5f)
	{
		Box(0, 0, W, MatteNow, Deep);
		Box(0, H - MatteNow, W, MatteNow, Deep);
		Flat(0, MatteNow - S, W, S, Rule);
		Flat(0, H - MatteNow, W, S, Rule);
	}
	Fade = Faded;
}

/** What stands behind PAUSE, CONTROLS and OPTIONS. PAUSE holds the picture, dimmed; OPTIONS, CONTROLS and the dialogs
 *  stand on the wall. Either eases in; when the menu goes it fades out over a title or a results screen, and CUTS
 *  when the race is live again (DrawHUD): nothing dims live play. */
void AFTHUD::OverlayDim(float Dim)
{
	OverDimNow = bReducedMotion ? Dim : FMath::FInterpTo(OverDimNow, Dim, Dt, 14.f);
	if (Dim > .004f) bOverWall = Dim >= .8f;
	if (OverDimNow <= .004f) return;
	if (bOverWall)
	{
		const float Up = FMath::Clamp(OverDimNow / .8f, 0.f, 1.f);
		Grad(0, 0, W, H, Alpha(Lilac, Up), Alpha(Peach, Up), false);
		if (Up >= .5f) MarkTone(Rect(0, 0, W, H), ETone::Light);
	}
	else Box(0, 0, W, H, Alpha(Deep, OverDimNow));
}

/** The sounds of the HUD's own motion (a stamp landing, a total counting up) are played where the motion is drawn, so
 *  the two cannot drift apart. S_<Name> in /Game/Audio/SFX (Tools/sfx_synth.py); a sound that is not there is silent. */
void AFTHUD::PlayCue(const TCHAR* Name, float Volume, float Pitch)
{
	const FName Key(Name);
	TObjectPtr<USoundBase>& Sound = CueSounds.FindOrAdd(Key);
	if (!Sound && !CueMissing.Contains(Key))
	{
		Sound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, *(FString(TEXT("/Game/Audio/SFX/S_")) + Name), nullptr, LOAD_NoWarn | LOAD_Quiet));
		if (!Sound) CueMissing.Add(Key);
	}
	UE_LOG(LogFT, Display, TEXT("FT CUE hud=%s vol=%.2f pitch=%.2f found=%d"), Name, Volume, Pitch, Sound ? 1 : 0);
	if (Sound) UGameplayStatics::PlaySound2D(this, Sound, Volume, Pitch);
}

void AFTHUD::DrawHUD()
{
	Super::DrawHUD();
	AFTGameMode* G = GetWorld() ? GetWorld()->GetAuthGameMode<AFTGameMode>() : nullptr;
	if (!G || !Canvas) return;
	W = Canvas->SizeX; H = Canvas->SizeY;
	if (W <= 0.f || H <= 0.f) return;
	S = FMath::Min(W / FTHUDStyle::Width, H / CanvasHeight);
	Left = (W - FTHUDStyle::Width * S) * .5f;
	Right = W - Left;
	T = GetWorld()->GetRealTimeSeconds();
	Dt = LastT < 0.f ? 0.f : FMath::Clamp(T - LastT, 0.f, .1f);
	LastT = T;
	++DrawCount;
	Fade = 1.f;
	Blocks.Reset();
	Areas.Reset();
	bSticker = false;
	bReducedMotion = G->GI() && G->GI()->bReducedMotion;
	UpdateMenuMotion(*G);
	if (SeenPhase != static_cast<uint8>(G->Phase))
	{
		SeenPhase = static_cast<uint8>(G->Phase);
		LastPhaseT = -1.f;   // (an event at the very start of a phase is crossed on its first frame)
	}
	if (!bTitleAssets)
	{
		bTitleAssets = true;
		FD = DisplayFont ? DisplayFont.Get() : GEngine->GetLargeFont();
		FB = BodyFont ? BodyFont.Get() : GEngine->GetMediumFont();
		FOrb = Cast<UFont>(StaticLoadObject(UFont::StaticClass(), nullptr, TEXT("/Game/UI/Fonts/F_Orbitron"), nullptr, LOAD_NoWarn | LOAD_Quiet));
		FRaj = Cast<UFont>(StaticLoadObject(UFont::StaticClass(), nullptr, TEXT("/Game/UI/Fonts/F_Rajdhani"), nullptr, LOAD_NoWarn | LOAD_Quiet));
		// the skin's display face (Bungee, SIL Open Font License 1.1); Orbitron stands in if it is not there
		FBun = Cast<UFont>(StaticLoadObject(UFont::StaticClass(), nullptr, TEXT("/Game/UI/Fonts/F_Bungee"), nullptr, LOAD_NoWarn | LOAD_Quiet));
		TLogo = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/UI/Title/T_TitleLogo"), nullptr, LOAD_NoWarn | LOAD_Quiet));
		OrbitHeight = FMath::Max(1.f, float(MeasureF(FOrb ? FOrb.Get() : FD.Get(), TEXT("Hg"), 1.f).Y / S));
		RajHeight = FMath::Max(1.f, float(MeasureF(FRaj ? FRaj.Get() : FB.Get(), TEXT("Hg"), 1.f).Y / S));
		BunHeight = FMath::Max(1.f, float(MeasureF(DisplayFace(), TEXT("Hg"), 1.f).Y / S));
		UE_LOG(LogFT, Display, TEXT("FT TITLE assets orbitron=%d rajdhani=%d logo=%d bungee=%d"), FOrb ? 1 : 0, FRaj ? 1 : 0, TLogo ? 1 : 0, FBun ? 1 : 0);
	}
	// 3.0 attract mode: the game runs underneath (no race HUD), the wipe, then the title on top
	if (G->bAttract)
	{
		DrawAttractWipe(G);
		DrawTitle(G);
	}
	else switch (G->Phase)
	{
	case EFTPhase::Title: DrawTitle(G); break;
	case EFTPhase::Card: DrawCard(G); break;
	case EFTPhase::Play: if (G->Overlay == EFTOverlay::None) { DrawPickupMarkers(*G); DrawMissileLock(*G); } bSticker = true; DrawPlay(G); bSticker = false; break;   // braces: clang -Werror,-Wmisleading-indentation (Mac)
	case EFTPhase::Wreck: case EFTPhase::Wrap:
		// CUT!: the instruments drop away, the mattes close in and the card takes the frame
		Fade = bReducedMotion ? 1.f : 1.f - EaseOut(G->PhaseT - .05f, .4f);
		bSticker = true; DrawPlay(G); bSticker = false;
		Fade = 1.f;
		DrawCut(G);
		break;
	case EFTPhase::Reel: DrawReel(G); break;
	case EFTPhase::Rating: DrawRating(G); break;
	case EFTPhase::Paycheck: DrawPaycheck(G); break;
	case EFTPhase::PreProduction: DrawPreProduction(G); break;
	}
	if (!G->bAttract) DrawFx(G);
	G->DrawnOverlay = G->Overlay;
	G->OverlayRects.Reset();
	switch (G->Overlay)
	{
	case EFTOverlay::Pause: DrawPause(G); break;
	case EFTOverlay::Controls: DrawControls(G); break;
	case EFTOverlay::Options: DrawOptions(G); break;
	case EFTOverlay::System: DrawSystemDialog(G); break;
	default:
		// the race is live the moment RESUME lands: the dim cuts there, as it always did; it still fades out on the
		// title and the results screens, where nothing is live
		if (G->Phase == EFTPhase::Play && !G->bAttract) OverDimNow = 0.f;
		OverlayDim(0.f);
		break;
	}
	bSticker = true; DrawSaveStatus(G); bSticker = false;   // (the skin) a slip of the HUD's kind: an ink sticker with its keyline, no bigger than it was
	if (const UFTGameInstance* GI = G->GI())
	{
		if (GI->bShowFps) Print(FString::Printf(TEXT("%.0f FPS  %.1f ms"), G->FpsSmoothed, 1000.f / FMath::Max(1.f, G->FpsSmoothed)), W - 12 * S, H - 22 * S, Green, LabelSize, 2);
		if (GI->bMuted) Print(TEXT("MUTED"), 12 * S, H - 22 * S, Grey, LabelSize);
	}
	LastPhaseT = G->PhaseT;
}

FVector2D AFTHUD::MeasureF(UFont* F, const FString& Str, float Scale)
{
	if (!F) F = FB;
	float Xl = 0, Yl = 0;
	Canvas->StrLen(F, Str, Xl, Yl);
	return FVector2D(Xl, Yl) * Scale * S;
}

void AFTHUD::Grad(float X, float Y, float Wd, float Ht, const FLinearColor& A, const FLinearColor& B, bool bHorizontal)
{
	const FVector2D P0(X, Y), P1(X + Wd, Y), P2(X, Y + Ht), P3(X + Wd, Y + Ht);
	const FLinearColor Ca = Alpha(A, Fade), Cb = Alpha(B, Fade);
	const FLinearColor C0 = Ca, C1 = bHorizontal ? Cb : Ca, C2 = bHorizontal ? Ca : Cb, C3 = Cb;
	TArray<FCanvasUVTri> Tris;
	FCanvasUVTri T0; T0.V0_Pos = P0; T0.V1_Pos = P1; T0.V2_Pos = P2; T0.V0_Color = C0; T0.V1_Color = C1; T0.V2_Color = C2;
	FCanvasUVTri T1; T1.V0_Pos = P1; T1.V1_Pos = P3; T1.V2_Pos = P2; T1.V0_Color = C1; T1.V1_Color = C3; T1.V2_Color = C2;
	Tris.Add(T0); Tris.Add(T1);
	FCanvasTriangleItem It(Tris, GWhiteTexture);
	It.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(It);
}

void AFTHUD::DrawTitle(AFTGameMode* G)
{
	const UFTGameInstance* GI = G->GI();
	const bool bInteractive = G->Overlay == EFTOverlay::None;
	const float X = Left + Safe * S, Y = (H - CanvasHeight * S) * .5f;
	const float BoardW = 442 * S, ActionY = H - 88 * S;
	const FLinearColor Night = Alpha(Deep, 0.f);

	Backdrop(G->bAttract ? 0.f : .46f, 0.f);
	Grad(0, 0, X + BoardW + 160 * S, H, Deep, Night, true);
	Grad(0, H - 135 * S, W, 135 * S, Night, Deep, false);

	// The title arrives: the board from the left with the logo on it, the shoots dealt down it one after another, the
	// actions up from the bottom edge (about half a second in all). The time is the game's time on the title, which is
	// kept across the attract mode's cuts, so a cut never deals it again.
	const float Entry = bInteractive ? G->TitleT : -1.f;
	const int32 TitleView = static_cast<int32>(EMenuView::Title);
	const int32 Count = FTData::NumLevels();
	G->TitleRowRects.SetNum(Count);
	const int32 Selection = FMath::Clamp(G->TitleSel, 0, Count - 1);
	// (main: lower rows, three lines stand under them; Career and bContinue are main's too)
	const float RowH = FMath::Min(34.f, 272.f / Count) * S;
	const float RowFlash = G->TitleActionFocus == INDEX_NONE ? FocusFlash(EMenuView::Title) : 0.f;
	const UFTCareerSubsystem* Career = UFTCareerSubsystem::Get(G);
	const bool bContinue = Career && Career->HasContinue();
	{
		FBlock Board(*this, In01(Entry, 0.f, DealTime), -48 * S, 0.f);
		Plate(Rect(X, Y + 25 * S, BoardW, ActionY - Y - 41 * S), Pink, Ink);
		AccentSweep(Rect(X + 1 * S, Y + 26 * S, BoardW - 2 * S, 4 * S), Entry, Pink, MenuEntrance);
		const float In = bReducedMotion || !bInteractive ? 1.f : EaseOut(Entry - .05f, MenuEntrance);
		const float LogoW = 395 * S;
		if (TLogo && TLogo->GetResource())
		{
			const float LogoH = LogoW * TLogo->GetSizeY() / FMath::Max(1, TLogo->GetSizeX());
			const float LogoX = X + (BoardW - LogoW) * .5f, LogoY = Y + (47.f + 12.f * (1.f - In)) * S;
			{
				// the logo settles into place a touch larger than it ends
				FPivot Settle(*this, FVector2D(LogoX + LogoW * .5f, LogoY + LogoH * .5f), 1.f + .07f * (1.f - In));
				FCanvasTileItem Logo(FVector2D(LogoX, LogoY), TLogo->GetResource(), FVector2D(LogoW, LogoH), Alpha(FLinearColor::White, (.74f + .26f * In) * Fade));
				Logo.BlendMode = SE_BLEND_Translucent;
				Canvas->DrawItem(Logo);
			}
			// (No glint across the chrome: an additive tile adds the texture's colour where its alpha is zero as well, so
			// a band of the logo "added to itself" lights a rectangle round the lettering. Seen in the frames of 15 Nov.)
		}
		else Print(TEXT("FULL THROTTLE"), X + BoardW * .5f, Y + 89 * S, White, Heading, 1, true, BoardW - 30 * S);

		Print(TEXT("AN 80s SPY CAR CHASE"), X + 16 * S, Y + 183 * S, Grey, Body);
		// the clapper's stripes across the board, between the masthead and the shoot list (TODO kit: the clapper arm)
		for (int32 I = 0; I < 21; ++I) Flat(X + (16 + I * 19.5f) * S, Y + 206 * S, 12 * S, 8 * S, White);
		Print(TEXT("SELECT YOUR SHOOT"), X + 16 * S, Y + 222 * S, White, Strong, 0, true);
		Print(TEXT("1–8  /  UP-DOWN  /  D-PAD"), X + BoardW - 16 * S, Y + 224 * S, Grey, LabelSize, 2);
		for (int32 I = 0; I < Count; ++I)
		{
			const FFTLevelDef& Level = FTData::Level(I);
			const FBox2D Row = Rect(X + 14 * S, Y + 252 * S + I * RowH, BoardW - 28 * S, RowH - 3 * S);
			const bool bFocus = Selection == I;
			const EControl State = bInteractive ? ControlState(Row, bFocus) : (bFocus ? EControl::Focus : EControl::Idle);
			const bool bHover = State == EControl::Hover || State == EControl::Pressed;
			const FLinearColor Accent = bFocus ? Cyan : bHover ? White : Rule;
			G->TitleRowRects[I] = Row;
			// dealt in one after another; the chosen shoot leans toward the reader and its bar widens
			const float RowEntry = Entry - .09f - I * DealStep;
			FBlock Dealt(*this, In01(Entry, .09f + I * DealStep, .2f), -26 * S, 0.f);
			const float Lean = FocusLevel(FocusKey(TitleView, 0, I), bFocus);
			const float Tx = Row.Min.X + 6 * S * Lean;
			Box(Row.Min.X, Row.Min.Y, Row.GetSize().X, Row.GetSize().Y, State == EControl::Pressed ? Deep : bFocus ? Active : Steel);
			Box(Row.Min.X, Row.Min.Y, (bReducedMotion ? (bFocus ? 4.f : 1.f) : 1.f + 3.f * Lean) * S, Row.GetSize().Y, Accent);
			if (bFocus || bHover) Frame(Row.Min.X, Row.Min.Y, Row.GetSize().X, Row.GetSize().Y, Accent, 1.f);
			if (bFocus && RowFlash > 0.f) Box(Row.Min.X, Row.Min.Y, Row.GetSize().X, Row.GetSize().Y, Alpha(White, .2f * RowFlash));
			if (bFocus && G->TitleActionFocus == INDEX_NONE && Entry >= MenuEntrance)
				FocusAccent(Row, MenuAge(EMenuView::Title, true), Cyan);
			else
				AccentSweep(Rect(Row.Min.X + 5 * S, Row.Min.Y + S, Row.GetSize().X - 10 * S, 2 * S),
					RowEntry, bFocus ? Cyan : Pink, FocusSweep);
			if (bFocus) Icon(EIcon::Play, Tx + 3 * S, Row.Min.Y + 9 * S, 12 * S, Pink);   // an arrow on the chosen shoot
			Print(FString::Printf(TEXT("%02d"), I + 1), Tx + 22 * S, Row.Min.Y + 9 * S, bFocus ? Cyan : Grey, LabelSize, 1, true);
			Print(Level.Zone, Tx + 45 * S, Row.Min.Y + 2 * S, White, Body, 0, true, 232 * S);
			Print(Level.Title, Tx + 45 * S, Row.Min.Y + 18 * S, Grey, LabelSize, 0, false, 232 * S);
			const bool bOnAir = G->bAttract && G->Level == I && G->AttractOutT < 0.f;
			const bool bHasBest = GI && I < UFTGameInstance::MaxLevels && GI->BestScore[I] > 0;
			if (bOnAir)
			{
				// the feed's light blinks slowly, as a camera's does
				const float Blink = bReducedMotion ? 1.f : .55f + .45f * FMath::Square(FMath::Sin(T * PI * .9f));
				Icon(EIcon::Record, Row.Max.X - (bHasBest ? 131 : 73) * S, Row.Min.Y + 9 * S, 16 * S, Alpha(Pink, Blink));
				if (!bHasBest) Print(TEXT("ON AIR"), Row.Max.X - 10 * S, Row.Min.Y + 9 * S, Pink, LabelSize, 2);
			}
			if (bHasBest)
			{
				Print(FString::Printf(TEXT("BEST %s"), *Num(GI->BestScore[I])), Row.Max.X - 10 * S, Row.Min.Y + 2 * S, Yellow, LabelSize, 2, false, 120 * S);
				Print(GI->BestTitle[I], Row.Max.X - 10 * S, Row.Min.Y + 17 * S, Grey, LabelSize, 2, false, 120 * S);
			}
			else if (!bOnAir) Print(TEXT("BEST ------"), Row.Max.X - 10 * S, Row.Min.Y + 9 * S, Alpha(Grey, .75f), LabelSize, 2);   // a shoot not played yet
		}
		if (Career)
		{
			// (main) the driver, what CONTINUE would resume, and the state of the save: main's lines, word for word and
			// where main puts them; they come up with the board
			FBlock Told(*this, In01(Entry, .34f, .2f));
			// (the skin) three stickers where the one line stood: the driver's level in a rating box, the cash on a price
			// tag, the skill points on a token. (TODO kit: FTSkin::Sticker draws the burst and the token.)
			const float ChipY = Y + 530 * S, ChipH = 23 * S;
			const auto Chip = [&](float Cx, float Cw, const FString& Words, const FLinearColor& Stock)
			{
				Flat(Cx, ChipY, Cw, ChipH, Ink);
				Flat(Cx + 1.5f * S, ChipY + 1.5f * S, Cw - 3 * S, ChipH - 3 * S, Stock);
				MarkTone(Rect(Cx, ChipY, Cw, ChipH), Lum(Stock) > .35f ? ETone::Light : ETone::Dark);
				Print(Words, Cx + Cw * .5f, ChipY + 3 * S, White, LabelSize, 1, true, Cw - 12 * S);
			};
			Chip(X + 16 * S, 122 * S, FString::Printf(TEXT("DRIVER LV %d"), Career->C.Level), Steel);
			Chip(X + 143 * S, 128 * S, Money(Career->C.Cash), Yellow);
			Chip(X + 276 * S, 150 * S, FString::Printf(TEXT("%d SKILL POINTS"), Career->SPLeft()), Cyan);
		}
	}

	{
		// The storyboard sheet (the ruling's item 4): a hollow paper frame taped over the live view, which is not touched;
		// the chosen shoot on its caption strip; and under it a memo, the home of what CONTINUE would resume and of the
		// state of the save (main's two lines, word for word; they stood under the shoot list).
		// (TODO kit: the sheet, its tape and the memo are FTSkin textures; the boss ringed in grease pencil is baked.)
		FBlock Sheet(*this, In01(Entry, .18f, DealTime), 30 * S, 0.f);
		const float Sx = X + BoardW + 44 * S, Sr = Right - (Safe - 4.f) * S, Sy = Y + 12 * S, Sb = Y + 466 * S;
		const float Rim = 12 * S, Strip = 46 * S, Sw = Sr - Sx, Sh = Sb - Sy;
		Flat(Sx + 3.5f * S, Sb, Sw, 3.5f * S, Ink);                       // the hard shadow, under and to the right
		Flat(Sr, Sy + 3.5f * S, 3.5f * S, Sh, Ink);
		Flat(Sx, Sy, Sw, Rim, Paper); Flat(Sx, Sb - Strip, Sw, Strip, Paper);
		Flat(Sx, Sy + Rim, Rim, Sh - Rim - Strip, Paper); Flat(Sr - Rim, Sy + Rim, Rim, Sh - Rim - Strip, Paper);
		Frame(Sx, Sy, Sw, Sh, Ink, 1.5f);
		Frame(Sx + Rim - 1.5f * S, Sy + Rim - 1.5f * S, Sw - 2 * Rim + 3 * S, Sh - Rim - Strip + 3 * S, Ink, 1.5f);
		MarkTone(Rect(Sx, Sb - Strip, Sw, Strip), ETone::Light);
		for (int32 Corner2 = 0; Corner2 < 4; ++Corner2)                    // four strips of tape
			Flat(Corner2 % 2 == 0 ? Sx - 12 * S : Sr - 34 * S, Corner2 < 2 ? Sy - 6 * S : Sb - 9 * S, 46 * S, 15 * S, Alpha(Sticky, .92f));
		const FFTLevelDef& Chosen = FTData::Level(Selection);
		Flat(Sx + 12 * S, Sb - Strip + 8 * S, 36 * S, 30 * S, Ink);
		MarkTone(Rect(Sx + 12 * S, Sb - Strip + 8 * S, 36 * S, 30 * S), ETone::Dark);
		Print(FString::Printf(TEXT("%02d"), Selection + 1), Sx + 30 * S, Sb - Strip + 13 * S, Yellow, Strong, 1, true);
		Print(Chosen.Zone, Sx + 58 * S, Sb - Strip + 4 * S, White, Strong, 0, true, 330 * S);
		Print(Chosen.Title, Sx + 58 * S, Sb - Strip + 26 * S, Grey, LabelSize, 0, false, 330 * S);
		if (GI && Selection < UFTGameInstance::MaxLevels && GI->BestScore[Selection] > 0)
			Print(FString::Printf(TEXT("BEST %s"), *Num(GI->BestScore[Selection])), Sr - 16 * S, Sb - Strip + 13 * S, Yellow, Strong, 2, true, 220 * S);
		if (Career)
		{
			const FBox2D Memo = Rect(Sx + 18 * S, Sb + 14 * S, 372 * S, 62 * S);
			Flat(Memo.Min.X + 3.5f * S, Memo.Min.Y + 3.5f * S, Memo.GetSize().X, Memo.GetSize().Y, Ink);
			Flat(Memo.Min.X, Memo.Min.Y, Memo.GetSize().X, Memo.GetSize().Y, Ink);
			Flat(Memo.Min.X + 1.5f * S, Memo.Min.Y + 1.5f * S, Memo.GetSize().X - 3 * S, Memo.GetSize().Y - 3 * S, Paper);
			MarkTone(Memo, ETone::Light);
			Flat(Memo.Min.X + 150 * S, Memo.Min.Y - 7 * S, 46 * S, 14 * S, Alpha(Sticky, .92f));
			Print(bContinue ? Career->ContinueLabel() : TEXT("NEW CAMPAIGN READY"), Memo.Min.X + 14 * S, Memo.Min.Y + 9 * S,
				bContinue ? Cyan : Grey, Body, 0, false, Memo.GetSize().X - 28 * S);
			FString SaveDetail = Career->C.Updated.IsEmpty() ? TEXT("Progress saves automatically at checkpoints.") : TEXT("LAST SAVE  ") + Career->C.Updated;
			if (Career->bRecoveredFromBackup) SaveDetail = TEXT("BACKUP RECOVERED  /  ") + Career->C.Updated;
			if (Career->bSaveError || Career->bLoadError) SaveDetail = TEXT("SAVE NEEDS ATTENTION  /  New Game can replace it.");
			Print(SaveDetail, Memo.Min.X + 14 * S, Memo.Min.Y + 35 * S, Career->bSaveError || Career->bLoadError ? Amber : Grey, LabelSize, 0, false, Memo.GetSize().X - 28 * S);
		}
	}

	static const TCHAR* Actions[] = { TEXT("CONTINUE"), TEXT("NEW GAME"), TEXT("OPTIONS"), TEXT("QUIT DESKTOP"), TEXT("PLAY SELECTED"), TEXT("CONTROLS") };
	const FString Hints[] = { TEXT("SAVED CHECKPOINT"), TEXT("FRESH CAMPAIGN"),
		InputHint(this, TEXT("O"), EKeys::Gamepad_FaceButton_Bottom), InputHint(this, TEXT("ESC"), EKeys::Gamepad_FaceButton_Bottom),
		TEXT("SELECTED SHOOT"), InputHint(this, TEXT("C"), EKeys::Gamepad_FaceButton_Top) };
	const AFTPlayerController* PC = Cast<AFTPlayerController>(GetOwningPlayerController());
	const float BarW = Right - X - Safe * S, LeadW = 220 * S;
	const float OtherW = (BarW - LeadW - 5 * Gap * S) / 5.f;
	G->TitleBtnRects.SetNum(UE_ARRAY_COUNT(Actions));
	const float ButtonFlash = G->TitleActionFocus != INDEX_NONE ? FocusFlash(EMenuView::Title) : 0.f;
	float Bx = X;
	for (int32 I = 0; I < UE_ARRAY_COUNT(Actions); ++I)
	{
		const float Bw = I == 0 ? LeadW : OtherW;
		const FBox2D ButtonRect = Rect(Bx, ActionY, Bw, 50 * S);
		const bool bEnabled = I != 0 || bContinue;
		const bool bFocus = I == G->TitleActionFocus;
		EControl State = bInteractive ? ControlState(ButtonRect, bFocus, bEnabled) : EControl::Idle;
		if (bEnabled && bFocus && bInteractive && PC && (PC->IsInputKeyDown(EKeys::Enter) || PC->IsInputKeyDown(EKeys::Gamepad_FaceButton_Bottom)))
			State = EControl::Pressed;
		{
			FBlock Dealt(*this, In01(Entry, .16f + I * .035f, .2f), 0.f, 24 * S);
			if (I == 0) Marquee(Rect(ButtonRect.Min.X - 6 * S, ButtonRect.Min.Y - 6 * S, Bw + 12 * S, 62 * S));   // the hero action stands in lights
			// loop run 2 cycle 4: a first launch has no save; the greyed CONTINUE said "SAVED CHECKPOINT" anyway
			G->TitleBtnRects[I] = Control(ButtonRect, Actions[I], I == 0 && !bContinue ? TEXT("NO SAVE YET") : Hints[I], State, I == 0 ? Cyan : Pink);
			if (bFocus && bEnabled && G->TitleActionFocus != INDEX_NONE)
			{
				FocusAccent(ButtonRect, MenuAge(EMenuView::Title, true), I == 0 ? Cyan : Pink);
				if (ButtonFlash > 0.f) Box(ButtonRect.Min.X, ButtonRect.Min.Y, Bw, 50 * S, Alpha(White, .2f * ButtonFlash));
			}
		}
		Bx += Bw + Gap * S;
	}
	{
		FBlock Help(*this, In01(Entry, .36f, .2f));
		Print(G->TitleActionFocus == INDEX_NONE ? InputHint(this, TEXT("TAB"), EKeys::Gamepad_RightShoulder) + TEXT("   ACTIONS")
			+ TEXT("     ") + InputHint(this, TEXT("G"), EKeys::Gamepad_Special_Left) + TEXT("   PRE-PRODUCTION")
			: TEXT("LEFT-RIGHT   ACTIONS     ") + ConfirmHint(this) + TEXT("   SELECT     ")
			+ InputHint(this, TEXT("TAB"), EKeys::Gamepad_RightShoulder) + TEXT("   SHOOTS     ")
			+ InputHint(this, TEXT("G"), EKeys::Gamepad_Special_Left) + TEXT("   PRE-PRODUCTION"),
			W * .5f, ActionY + 58 * S, Grey, LabelSize, 1);
	}

	if (!G->SystemMessage.IsEmpty() && G->Overlay == EFTOverlay::None)
	{
		const float Center = (X + BoardW + Right) * .5f;
		const FBox2D Notice = Rect(Center - 252 * S, Y + 327 * S, 504 * S, 88 * S);
		Plate(Notice, Amber, Ink);
		const TArray<FString> Lines = WrapText(G->SystemMessage, 464 * S, Body);
		for (int32 I = 0; I < Lines.Num() && I < 3; ++I) Print(Lines[I], Notice.Min.X + 20 * S, Notice.Min.Y + (15 + I * 21) * S, Amber, Body);
	}
	if (G->bAttract)
	{
		FBlock Feed(*this, In01(Entry, .2f, DealTime), 40 * S, 0.f);
		DrawAttractOsd(G);
	}
	if (GI && !GI->TitleBanner.IsEmpty() && G->TitleT < 8.f)
	{
		const float Center = (X + BoardW + Right) * .5f;
		const FBox2D Banner = Rect(Center - 230 * S, Y + 227 * S, 460 * S, 68 * S);
		FBlock News(*this, In01(Entry, .3f, DealTime), 0.f, -18 * S);
		Plate(Banner, Yellow, Ink);
		AccentSweep(Rect(Banner.Min.X + S, Banner.Min.Y + S, Banner.GetSize().X - 2 * S, 3 * S), Entry - .3f, Yellow, MenuEntrance);
		Print(GI->TitleBanner, Center, Banner.Min.Y + 13 * S, Yellow, Heading, 1, true, 420 * S);
	}
}

void AFTHUD::DrawAttractOsd(AFTGameMode* G)
{
	const bool bOut = G->AttractOutT >= 0.f;
	const int32 Index = bOut ? G->AttractNextLevel() : G->Level;
	const FFTLevelDef& Level = FTData::Level(Index);
	const float X = Right - (Safe + 306.f + 14.f) * S, Y = (H - CanvasHeight * S) * .5f + 30 * S;   // (inside the storyboard sheet's opening)
	const FBox2D Monitor = Rect(X, Y, 306 * S, 67 * S);
	Plate(Monitor, Pink, Ink);
	if (G->Overlay == EFTOverlay::None)
		AccentSweep(Rect(X + 7 * S, Monitor.Max.Y - 6 * S, 284 * S, 3 * S), G->TitleT - .085f, Pink, MenuEntrance);
	Icon(EIcon::Record, X + 12 * S, Y + 10 * S, 18 * S, Pink);
	Print(bOut ? TEXT("CHANGING FEED") : TEXT("ATTRACT FEED"), X + 36 * S, Y + 10 * S, Pink, LabelSize, 0, true);
	Print(FString::Printf(TEXT("CH %02d"), Index + 1), Monitor.Max.X - 14 * S, Y + 10 * S, Grey, LabelSize, 2, true);
	Print(Level.Zone + TEXT(" / ") + Level.Title, X + 15 * S, Y + 34 * S, White, Strong, 0, false, 276 * S);
}

void AFTHUD::DrawPickupMarkers(const AFTGameMode& Game)
{
	const AFTPlayerCar* Car = Game.GetCar();
	const APlayerController* View = GetOwningPlayerController();
	if (!Car || !View) return;

	constexpr float MaxAhead = 10500.f;
	constexpr int32 MaxMarkers = 4;
	TArray<const FFTEnt*, TInlineAllocator<8>> Pickups;
	for (const FFTEnt& Entity : Game.GetActiveEntities())
	{
		const float Ahead = Entity.D - Car->D;
		if (!Entity.bDead && (Entity.Kind == EFTKind::Crate || Entity.Kind == EFTKind::Wrench) && Ahead > 220.f && Ahead < MaxAhead)
			Pickups.Add(&Entity);
	}
	if (Pickups.IsEmpty()) return;
	Pickups.Sort([](const FFTEnt& Left, const FFTEnt& Right) { return Left.D < Right.D; });

	FVector Eye;
	FRotator EyeRotation;
	View->GetPlayerViewPoint(Eye, EyeRotation);
	FCollisionQueryParams Visibility(SCENE_QUERY_STAT(FTPickupMarker), true, Car);
	TArray<FBox2D, TInlineAllocator<MaxMarkers>> Drawn;
	const float LabelHeight = 16.f;
	const float Margin = 12.f * S;
	const float Stem = 8.f * S;
	static const FString Missiles(TEXT("MISSILES")), Repair(TEXT("REPAIR"));
	for (const FFTEnt* Pickup : Pickups)
	{
		const AFTEntityActor* Actor = Pickup->Proxy.Get();
		if (!Actor || Actor->IsHidden()) continue;
		const FVector Anchor = Actor->GetActorLocation() + Actor->GetActorUpVector() * 175.f;
		const FVector Screen = Canvas->Project(Anchor);
		if (Screen.Z <= 0.f || Screen.ContainsNaN()) continue;

		const bool bRepair = Pickup->Kind == EFTKind::Wrench;
		const FString& Label = bRepair ? Repair : Missiles;
		const FVector2D MarkerTextSize = TextSize(Label, LabelHeight);
		const FVector2D Size(MarkerTextSize.X + 18.f * S, MarkerTextSize.Y + 8.f * S);
		const FVector2D TopLeft(Screen.X - Size.X * 0.5, Screen.Y - Stem - Size.Y);
		const FBox2D Bounds(TopLeft, TopLeft + Size + FVector2D(0.f, Stem));
		if (Bounds.Min.X < Margin || Bounds.Max.X > W - Margin || Bounds.Min.Y < (Game.bBossOn ? 133 : 114) * S || Bounds.Max.Y > H - Margin) continue;
		if (Drawn.ContainsByPredicate([&](const FBox2D& Other) { return Bounds.Intersect(Other); })) continue;

		const FVector Target = Actor->GetActorLocation() + Actor->GetActorUpVector() * 78.f;
		if (GetWorld()->LineTraceTestByChannel(Eye, Target, ECC_Visibility, Visibility)) continue;

		const float Ahead = Pickup->D - Car->D;
		const float Alpha = FMath::SmoothStep(220.f, 630.f, Ahead) * (1.f - FMath::SmoothStep(8700.f, MaxAhead, Ahead));
		FLinearColor Accent = bRepair ? Green : Amber;
		Accent.A = Alpha;
		Box(TopLeft.X, TopLeft.Y, Size.X, Size.Y, FLinearColor(0.014f, 0.009f, 0.032f, 0.94f * Alpha));
		Frame(TopLeft.X, TopLeft.Y, Size.X, Size.Y, Accent, 1.5f);
		Box(Screen.X - S, Screen.Y - Stem, 2.f * S, Stem, Accent);
		Print(Label, Screen.X, TopLeft.Y + 3.5f * S, FLinearColor(White.R, White.G, White.B, Alpha), LabelHeight, 1);
		Drawn.Add(Bounds);
		if (Drawn.Num() == MaxMarkers) break;
	}
}

/** A missile in flight homes on the foe ahead with the lowest 2|dx| + dy (AFTGameMode::TickEnts, case Missile). The same
 *  sum is taken here, so the brackets sit on the car it will really hit. Four thin corners round that car and nothing
 *  else: they close in when the lock is taken and are gone with the missile. */
void AFTHUD::DrawMissileLock(const AFTGameMode& Game)
{
	const TConstArrayView<FFTEnt> Ents = Game.GetActiveEntities();
	const FFTEnt* Target = nullptr;
	for (const FFTEnt& Missile : Ents)
	{
		if (Missile.bDead || Missile.Kind != EFTKind::Missile || Missile.Owner != 0) continue;
		float Best = 1e12f;
		for (const FFTEnt& Other : Ents)
		{
			if (Other.bDead || Other.Kind == EFTKind::Bullet || Other.Kind == EFTKind::Missile || Other.Kind == EFTKind::Crate || Other.Kind == EFTKind::Wrench
				|| Other.Kind == EFTKind::Civ || Other.Kind == EFTKind::Henchman || Other.Kind == EFTKind::Ramp || Other.Kind == EFTKind::Barrel) continue;
			const float Ahead = Other.D - Missile.D;
			if (Ahead < -200.f) continue;
			const float Sum = 2.f * FMath::Abs(Other.X - Missile.X) / FT::XU + Ahead / FT::DU;
			if (Sum < Best) { Best = Sum; Target = &Other; }
		}
		break;   // the first one in flight: one mark is enough
	}
	const float Now = Game.RealClockNow();
	if (!Target) { LockId = 0; return; }
	if (Target->Id != LockId) { LockId = Target->Id; LockAt = Now; }
	const AFTEntityActor* Actor = Target->Proxy.Get();
	if (!Actor || Actor->IsHidden()) return;
	const FVector Middle = Actor->GetActorLocation() + Actor->GetActorUpVector() * 90.f;
	const FVector Screen = Canvas->Project(Middle);
	const FVector Above = Canvas->Project(Middle + Actor->GetActorUpVector() * 100.f);
	if (Screen.Z <= 0.f || Above.Z <= 0.f || Screen.ContainsNaN() || Above.ContainsNaN()) return;
	const float PerCm = FVector2D::Distance(FVector2D(Screen.X, Screen.Y), FVector2D(Above.X, Above.Y)) / 100.f;
	const float Closing = bReducedMotion ? 1.f : 1.f + .9f * (1.f - EaseOut(Now - LockAt, .16f));
	const float Half = FMath::Clamp(Target->W * .62f * PerCm, 15.f * S, 80.f * S) * Closing;
	if (Screen.X < Half || Screen.X > W - Half || Screen.Y < Half || Screen.Y > H - Half) return;
	const float Arm = Half * .34f;
	const FLinearColor Mark = Alpha(Mix(Amber, White, Flash01(Now - LockAt, .2f)), .92f);
	for (int32 Sx = -1; Sx <= 1; Sx += 2)
		for (int32 Sy = -1; Sy <= 1; Sy += 2)
		{
			const FVector2D Corner2(Screen.X + Sx * Half, Screen.Y + Sy * Half);
			Stroke(Corner2, Corner2 - FVector2D(Sx * Arm, 0.f), Mark, 2.f);
			Stroke(Corner2, Corner2 - FVector2D(0.f, Sy * Arm), Mark, 2.f);
		}
}

/** The race has just come on screen (after the slate, a retake, a reshoot): what the HUD remembers of the last frame
 *  is of another take, so nothing reads as a change. */
void AFTHUD::ResetPlayFeedback(const AFTGameMode& G)
{
	LastArmor = G.Hp;
	ArmorGhost = FMath::Clamp(G.HpPct() / 100.f, 0.f, 1.f);
	LastMissiles = G.Missiles;
	LastAmmoLoad = FMath::Clamp(G.AmmoLoad, 0, 2);
	LastAmmoRounds = LastAmmoLoad > 0 ? G.Run.Rounds[LastAmmoLoad - 1] : 0;
	LastDamageState = -1;
	LastCombo = G.Combo;
	LastKills = G.Kills; LastNear = G.NearMisses;
	LastScore = G.Score; ScoreShown = static_cast<double>(G.Score);
	LastDriftLevel = G.DriftLevel;
	bLastTurboFull = true;
	bLastBossOn = G.bBossOn; LastBossFrac = BossGhost = G.BossHpFrac;
	ArmorChanged = MissileChanged = AmmoChanged = LoadSwitchAt = ArmorHitAt = ArmorGainAt = DamageStateAt = MissileFiredAt = MissileGainAt = -10.f;
	ComboUpAt = CountsAt = ScoreUpAt = DriftUpAt = TurboReadyAt = BossOnAt = BossHitAt = BossPhaseAt = -10.f;
}

void AFTHUD::DrawPlay(AFTGameMode* G)
{
	const FFTLevelDef& Level = G->Def();
	const UFTGameInstance* GI = G->GI();
	const FFTProgressData& ProgressData = FFTProgressData::Get();
	const UFTInputSubsystem* Input = UFTInputSubsystem::Get(this);
	const auto Hint = [&](EFTAction Action, const TCHAR*) { return Input ? Input->RacePrompt(Action) : Binding(GI, Action); };
	const float Now = G->RealClockNow();
	const float X = Left + Safe * S, R = Right - Safe * S, Center = W * .5f;
	const float Top = 25 * S, Base = H - 47 * S;
	const float Hp = G->HpPct(), TurboFrac = G->Turbo / FMath::Max(1.f, G->Stats.TurboTank);
	const float HpFrac = FMath::Clamp(Hp / 100.f, 0.f, 1.f);
	const int32 Load = FMath::Clamp(G->AmmoLoad, 0, 2), Rounds = Load > 0 ? G->Run.Rounds[Load - 1] : 0;
	const bool bCut = G->Phase == EFTPhase::Wreck || G->Phase == EFTPhase::Wrap;

	// Live play is never dimmed or masked: the slate's dim and its slim mattes CUT when the race starts, as the slate
	// always did. Only at CUT! and THAT'S A WRAP, when the take is over, do the letterbox mattes ease in (they are
	// the highlight reel's, so the reel takes over without a cut).
	if (!bCut) { DimNow = 0.f; MatteNow = 0.f; }
	Backdrop(0.f, bCut && !bReducedMotion ? ReelBar() : 0.f);

	// ---- what changed since the last frame (the feedback below is timed from these)
	if (PlayDrawn != DrawCount - 1)
	{
		ResetPlayFeedback(*G);
		PlaySince = T;
	}
	PlayDrawn = DrawCount;
	if (LastArmor >= 0.f && !FMath::IsNearlyEqual(LastArmor, G->Hp))
	{
		ArmorChanged = Now;
		const float Was = FMath::Clamp(LastArmor / FMath::Max(1.f, G->MaxHp()), 0.f, 1.f);
		if (G->Hp < LastArmor) { ArmorHitAt = Now; ArmorHitSize = (Was - HpFrac) * 100.f; ArmorGhost = FMath::Max(ArmorGhost, Was); }
		else ArmorGainAt = Now;
	}
	// the armour just lost stays on the bar for a beat, then drains
	if (bReducedMotion || HpFrac >= ArmorGhost) ArmorGhost = HpFrac;
	else if (Now - ArmorHitAt > GhostHold) ArmorGhost = FMath::Max(HpFrac, ArmorGhost - Dt * .9f);
	const int32 DamageState = Hp > 79.f ? 0 : Hp > 59.f ? 1 : Hp > 39.f ? 2 : Hp > 24.f ? 3 : 4;
	if (LastDamageState >= 0 && DamageState > LastDamageState) DamageStateAt = Now;
	LastDamageState = DamageState;
	if (LastMissiles >= 0 && LastMissiles != G->Missiles)
	{
		MissileChanged = Now;
		(G->Missiles < LastMissiles ? MissileFiredAt : MissileGainAt) = Now;
	}
	if (LastAmmoLoad >= 0 && (LastAmmoLoad != Load || LastAmmoRounds != Rounds)) AmmoChanged = Now;
	if (LastAmmoLoad >= 0 && LastAmmoLoad != Load) LoadSwitchAt = Now;
	if (G->Combo > LastCombo) ComboUpAt = Now;
	if (G->Kills != LastKills || G->NearMisses != LastNear) CountsAt = Now;
	if (G->Score > LastScore) ScoreUpAt = Now;
	// the box office rolls up to its new figure instead of jumping (a retake's lower figure is simply shown)
	if (bReducedMotion || G->Score < ScoreShown || G->Score - ScoreShown < 1.0) ScoreShown = static_cast<double>(G->Score);
	else ScoreShown += (G->Score - ScoreShown) * FMath::Min(1.f, Dt * 11.f);
	if (G->DriftLevel > LastDriftLevel) DriftUpAt = Now;
	const bool bTurboFull = TurboFrac >= .99f;
	if (bTurboFull && !bLastTurboFull) TurboReadyAt = Now;
	if (G->bBossOn)
	{
		if (!bLastBossOn) { BossOnAt = Now; BossGhost = 0.f; LastBossFrac = G->BossHpFrac; }
		if (G->BossHpFrac < LastBossFrac - 1e-4f)
		{
			BossHitAt = Now;
			BossGhost = FMath::Max(BossGhost, LastBossFrac);
			// a third of his armour gone is the next phase of the fight (the two lines TickBoss reads)
			for (const float Line : { .66f, .33f })
				if (LastBossFrac > Line && G->BossHpFrac <= Line) BossPhaseAt = Now;
		}
		LastBossFrac = G->BossHpFrac;
		if (bReducedMotion || G->BossHpFrac >= BossGhost) BossGhost = G->BossHpFrac;
		else if (Now - BossHitAt > GhostHold) BossGhost = FMath::Max(G->BossHpFrac, BossGhost - Dt * .7f);
	}
	LastArmor = G->Hp;
	LastMissiles = G->Missiles;
	LastAmmoLoad = Load;
	LastAmmoRounds = Rounds;
	LastCombo = G->Combo;
	LastKills = G->Kills; LastNear = G->NearMisses;
	LastScore = G->Score;
	LastDriftLevel = G->DriftLevel;
	bLastTurboFull = bTurboFull;
	bLastBossOn = G->bBossOn;
	const auto Emphasis = [&](float Changed, const FLinearColor& Normal)
	{
		const float Amount = bReducedMotion ? 0.f : 1.f - EaseOut(Now - Changed, Feedback);
		return FMath::Lerp(Normal, White, Amount * .65f);
	};
	// the instruments come on from their own edges when the slate gives way to the race
	const float PlayAge = bReducedMotion ? -1.f : T - PlaySince;

	// Primary instruments sit on either side of the car and stay within the center 16:9 area.
	const float DriverW = 322 * S, DriverY = Base - 171 * S;
	{
		// a hit jolts the panel, flashes it red and leaves the lost armour on the bar for a beat
		const float HitAge = Now - ArmorHitAt;
		const float Hit = Flash01(HitAge, .3f);
		FBlock Arrive(*this, In01(PlayAge, 0.f, DealTime), -48 * S, 0.f, ShakeX(HitAge, FMath::Clamp(ArmorHitSize * .4f, 2.5f, 8.f) * S, ShakeTime), 0.f);
		Plate(Rect(X, DriverY, DriverW, 171 * S), Cyan, Ink);
		if (Hit > 0.f)
		{
			Box(X, DriverY, DriverW, 171 * S, Alpha(Red, .16f * Hit));
			Frame(X, DriverY, DriverW, 171 * S, Alpha(Red, Hit), 2.f);
		}
		Print(TEXT("KESTREL-88 / AGENT N. KANE"), X + 14 * S, DriverY + 9 * S, Grey, LabelSize, 0, true);
		Print(FString::FromInt(FMath::RoundToInt(G->SpeedMph)), X + 14 * S, DriverY + 28 * S, White, Readout, 0, true, 109 * S);
		Print(TEXT("MPH"), X + 132 * S, DriverY + 50 * S, Grey, LabelSize, 0, true);
		const float SpeedFrac = FMath::Clamp(G->SpeedMph / (900.f * .31f), 0.f, 1.f);
		for (int32 I = 0; I < 12; ++I)
		{
			const float Px = X + (184 + I * 10) * S, He = (5.f + .7f * I) * S;
			Stroke(FVector2D(Px, DriverY + 64 * S), FVector2D(Px + 4 * S, DriverY + 64 * S - He),
				(I + 1) / 12.f <= SpeedFrac ? (I > 9 ? Pink : Cyan) : Track, 3.f);
		}
		Box(X + 14 * S, DriverY + 76 * S, 294 * S, S, Rule);

		const FLinearColor ArmorColor = Hp > 60.f ? Green : Hp > 30.f ? Amber : Red;
		const TCHAR* Damage = Hp > 79.f ? TEXT("NOMINAL") : Hp > 59.f ? TEXT("BODYWORK DAMAGED") : Hp > 39.f ? TEXT("WINDSHIELD CRACKED") : Hp > 24.f ? TEXT("ENGINE SMOKING") : TEXT("CRITICAL / ON FIRE");
		// critical: the warning beats (slowly: it is a state, not an attack)
		// (a danger read-out beats BRIGHTER, toward white, never dimmer: it is at full strength all the time)
		const float Beat = Hp <= 25.f && !bReducedMotion ? FMath::Square(FMath::Sin(Now * PI * 1.6f)) : 0.f;
		// (the skin) armour at 25 or less: the whole armour row is solid red with white text
		const bool bArmorLow = Hp <= 25.f;
		if (bArmorLow) Flat(X + 7 * S, DriverY + 80 * S, DriverW - 17 * S, 52 * S, Red);
		const FLinearColor ArmorInk = bArmorLow ? White : ArmorColor;
		Icon(Hp <= 25.f ? EIcon::Warning : EIcon::Repair, X + 14 * S, DriverY + 84 * S, IconSize * S, Mix(ArmorInk, White, .45f * Beat));
		Print(TEXT("ARMOR"), X + 39 * S, DriverY + 84 * S, White, Body, 0, true);
		Print(FString::Printf(TEXT("%d%%   %.0f / %.0f"), FMath::RoundToInt(Hp), G->Hp, G->MaxHp()),
			X + 307 * S, DriverY + 84 * S, Emphasis(ArmorChanged, ArmorInk), Strong, 2, false, 190 * S);
		const FBox2D ArmorBar = Rect(X + 14 * S, DriverY + 107 * S, 294 * S, 8 * S);
		Meter(ArmorBar, Hp / 100.f, ArmorInk, 12, ArmorGhost);
		const float Gain = Flash01(Now - ArmorGainAt, .35f);
		if (Gain > 0.f) Box(ArmorBar.Min.X, ArmorBar.Min.Y, ArmorBar.GetSize().X * HpFrac, ArmorBar.GetSize().Y, Alpha(White, .6f * Gain));
		Print(Damage, X + 14 * S, DriverY + 117 * S, Mix(ArmorInk, White, FMath::Max(Flash01(Now - DamageStateAt, .5f), .45f * Beat)), LabelSize);

		const TCHAR* Turbo = G->bTurboOn ? TEXT("BURN") : G->BoostT > 0.f ? TEXT("BOOST") : G->bTurboLock ? TEXT("RECHARGING") : TurboFrac >= .99f ? TEXT("READY") : TEXT("CHARGING");
		const FLinearColor TurboColor = G->bTurboOn ? Pink : Cyan;
		const float Ready = Flash01(Now - TurboReadyAt, .45f);
		Icon(EIcon::Turbo, X + 14 * S, DriverY + 135 * S, 16 * S, Mix(TurboColor, White, Ready));
		Print(FString::Printf(TEXT("TURBO  [%s]"), *Hint(EFTAction::Turbo, TEXT("LB/RT"))), X + 37 * S, DriverY + 134 * S, White, LabelSize, 0, false, 154 * S);
		Print(G->bDraft ? FString(Turbo) + TEXT(" / DRAFT") : FString(Turbo), X + 307 * S, DriverY + 134 * S, Mix(TurboColor, White, Ready), LabelSize, 2);
		const FBox2D TurboBar = Rect(X + 14 * S, DriverY + 153 * S, 294 * S, 7 * S);
		Meter(TurboBar, TurboFrac, TurboColor, 8);
		if (Ready > 0.f) Box(TurboBar.Min.X, TurboBar.Min.Y, TurboBar.GetSize().X, TurboBar.GetSize().Y, Alpha(White, .5f * Ready));
	}

	const float WeaponW = 300 * S, WX = R - WeaponW, WY = Base - 163 * S;
	{
		FBlock Arrive(*this, In01(PlayAge, .04f, DealTime), 48 * S, 0.f);
		Plate(Rect(WX, WY, WeaponW, 163 * S), Amber, Ink);
		Print(TEXT("ORDNANCE"), WX + 14 * S, WY + 8 * S, Grey, LabelSize, 0, true);
		// a launch kicks the icon and the count, and the round that left the rack flashes out of the bar
		const float Fired = Flash01(Now - MissileFiredAt, .22f), Loaded = Flash01(Now - MissileGainAt, .4f);
		Icon(EIcon::Missile, WX + (14 + 5 * Fired) * S, WY + (29 - 4 * Fired) * S, IconSize * S, Mix(Amber, White, FMath::Max(Fired, Loaded)));
		Print(FString::Printf(TEXT("MISSILES  [%s]"), *Hint(EFTAction::Missile, TEXT("B"))), WX + 39 * S, WY + 29 * S, White, Body, 0, false, 160 * S);
		const int32 Capacity = G->MissileCap();
		{
			FPivot Kick(*this, FVector2D(R - 14 * S, WY + 38 * S), 1.f + .22f * Flash01(Now - MissileChanged, PunchTime));
			Print(FString::Printf(TEXT("%d / %d"), G->Missiles, Capacity), R - 14 * S, WY + 26 * S,
				Emphasis(MissileChanged, G->Missiles > 0 ? Amber : Grey), Strong, 2, true, 82 * S);
		}
		const int32 Racks = FMath::Clamp(Capacity, 1, 16);
		const FBox2D MissileBar = Rect(WX + 14 * S, WY + 54 * S, 272 * S, 6 * S);
		Meter(MissileBar, float(G->Missiles) / FMath::Max(1, Capacity), Amber, Racks);
		if (Fired > 0.f && G->Missiles < Racks)
		{
			const float Pitch = (MissileBar.GetSize().X + 3 * S) / Racks;
			Box(MissileBar.Min.X + G->Missiles * Pitch, MissileBar.Min.Y - 2 * S * Fired, Pitch - 3 * S, 6 * S + 4 * S * Fired, Alpha(White, Fired));
		}
		if (Loaded > 0.f) Box(MissileBar.Min.X, MissileBar.Min.Y, MissileBar.GetSize().X * FMath::Clamp(float(G->Missiles) / FMath::Max(1, Capacity), 0.f, 1.f), 6 * S, Alpha(White, .6f * Loaded));

		Icon(EIcon::Ammo, WX + 14 * S, WY + 68 * S, IconSize * S, Cyan);
		Print(FString::Printf(TEXT("AMMO  [%s]"), *Hint(EFTAction::Ammo, TEXT("Y"))), WX + 39 * S, WY + 67 * S, White, Body, 0, false, 150 * S);
		if (Load > 0) Print(Num(Rounds), R - 14 * S, WY + 65 * S, Emphasis(AmmoChanged, Cyan), Strong, 2, true, 94 * S);
		static const TCHAR* LoadNames[] = { TEXT("STD"), TEXT("AP"), TEXT("HE") };
		static const FLinearColor LoadColors[] = { Yellow, Cyan, Amber };
		const float Switched = Flash01(Now - LoadSwitchAt, .25f);
		for (int32 I = 0; I < UE_ARRAY_COUNT(LoadNames); ++I)
		{
			const float Px = WX + (14 + I * 92) * S;
			const bool bOwned = I == 0 || (I == 1 ? G->Stats.ApRank > 0 : G->Stats.HeRank > 0);
			Box(Px, WY + 92 * S, 86 * S, 23 * S, I == Load ? Active : bOwned ? Steel : Deep);
			if (I == Load) Box(Px, WY + 113 * S, 86 * S, 2 * S, LoadColors[I]);
			if (I == Load && Switched > 0.f) Box(Px, WY + 92 * S, 86 * S, 23 * S, Alpha(LoadColors[I], .45f * Switched));
			Print(LoadNames[I], Px + 43 * S, WY + 94 * S, I == Load ? LoadColors[I] : bOwned ? White : Grey,
				LabelSize, 1, true);
		}
		const TCHAR* Drift = G->BoostT > 0.f ? TEXT("BOOST!") : !G->bDrifting ? TEXT("HOLD + STEER")
			: G->DriftLevel == 0 ? TEXT("SLIDING") : G->DriftLevel == 1 ? TEXT("BLUE") : G->DriftLevel == 2 ? TEXT("ORANGE") : TEXT("PINK!");
		const FLinearColor DriftColor = G->DriftLevel >= 3 ? Pink : G->DriftLevel == 2 ? Amber : Cyan;
		const float Charged = Flash01(Now - DriftUpAt, .3f);
		Print(FString::Printf(TEXT("DRIFT  [%s]"), *Hint(EFTAction::Drift, TEXT("A/RB"))), WX + 14 * S, WY + 120 * S, Grey, LabelSize, 0, false, 136 * S);
		Print(Drift, R - 14 * S, WY + 120 * S, G->bDrifting || G->BoostT > 0.f ? Mix(DriftColor, White, Charged) : Grey, LabelSize, 2);
		const FBox2D DriftBar = Rect(WX + 14 * S, WY + 141 * S, 272 * S, 5 * S);
		Meter(DriftBar, G->DriftLevel / 3.f, DriftColor, 3);
		if (Charged > 0.f) Box(DriftBar.Min.X, DriftBar.Min.Y, DriftBar.GetSize().X * FMath::Clamp(G->DriftLevel / 3.f, 0.f, 1.f), 5 * S, Alpha(White, .7f * Charged));
	}

	// Secondary information stays near the top edge. Temporary cards share the same lane.
	const float ObjectiveW = 362 * S, OX = Center - ObjectiveW * .5f;
	const float ProgressW = ObjectiveW - 24 * S;
	float NoticeY = Top + 58 * S;
	{
		FBlock Arrive(*this, In01(PlayAge, .02f, DealTime), 0.f, -40 * S);
		Plate(Rect(OX, Top, ObjectiveW, 51 * S), Cyan, Ink);
		Print(FString::Printf(TEXT("SCENE %d/%d  /  TAKE %d"), G->Scene + 1, G->NumScenes(), G->Take), OX + 12 * S, Top + 7 * S, White, LabelSize, 0, true);
		Print(TEXT("FINALE"), OX + ObjectiveW - 12 * S, Top + 7 * S, Pink, LabelSize, 2, true);
		Meter(Rect(OX + 12 * S, Top + 31 * S, ProgressW, 7 * S), G->Progress, Cyan);
		for (const FFTScene& Scene : Level.Scenes)
			Box(OX + 12 * S + ProgressW * FMath::Clamp(Scene.D / FMath::Max(1.f, Level.FinaleD), 0.f, 1.f) * .85f, Top + 28 * S, S, 12 * S, Grey);
		Box(OX + 12 * S + ProgressW * .85f, Top + 27 * S, 2 * S, 14 * S, Pink);
	}
	if (G->bBossOn)
	{
		// The boss's plate drops in and its bar fills; every hit leaves what he just lost on the bar for a beat; a third
		// of his armour gone (the next phase of the fight) jolts the plate and flashes it white.
		const float BossAge = Now - BossOnAt, PhaseAge = Now - BossPhaseAt;
		const float Phased = Flash01(PhaseAge, .45f);
		FBlock Arrive(*this, In01(BossAge, 0.f, DealTime), 0.f, -18 * S, ShakeX(PhaseAge, 7 * S, .42f), 0.f);
		const FBox2D BossPlate = Rect(OX, NoticeY, ObjectiveW, 66 * S);
		Plate(BossPlate, Red, Ink);
		AccentSweep(Rect(OX + S, NoticeY + S, ObjectiveW - 2 * S, 3 * S), BossAge, Red, MenuEntrance);
		if (Phased > 0.f)
		{
			Box(OX, NoticeY, ObjectiveW, 66 * S, Alpha(White, .22f * Phased));
			Frame(OX, NoticeY, ObjectiveW, 66 * S, Alpha(White, Phased), 2.f);
		}
		Icon(EIcon::Warning, OX + 12 * S, NoticeY + 8 * S, 16 * S, Mix(Red, White, Phased));
		Print(G->BossLabel, OX + 36 * S, NoticeY + 7 * S, White, Body, 0, true, 206 * S);
		const FString GapLabel = G->bLastChance ? TEXT("LAST CHANCE!") : G->BossRate < 1.f
			? FString::Printf(TEXT("GAP ×%.2f"), G->BossRate) : FString(TEXT("GAP"));
		const float Urgent = G->bLastChance && !bReducedMotion ? FMath::Square(FMath::Sin(Now * PI * 1.5f)) : 0.f;   // brighter, never dimmer; slow: no strobe
		Print(GapLabel, OX + ObjectiveW - 12 * S, NoticeY + 8 * S, G->bLastChance ? Mix(Red, White, .5f * Urgent) : Yellow, Body, 2, false, 120 * S);
		const FBox2D BossBar = Rect(OX + 12 * S, NoticeY + 33 * S, ProgressW, 8 * S);
		const float Filling = bReducedMotion ? 1.f : EaseOut(BossAge - .1f, .5f);
		Meter(BossBar, G->BossHpFrac * Filling, Red, 8, BossGhost * Filling);
		// where the fight changes: a notch at two thirds and at one third (white while it is still to come)
		for (const float Line : { .66f, .33f })
			Box(BossBar.Min.X + ProgressW * Line - S, BossBar.Min.Y - 3 * S, 2 * S, 14 * S, G->BossHpFrac > Line ? White : Alpha(Grey, .45f));
		const float Struck = Flash01(Now - BossHitAt, .14f);
		if (Struck > 0.f) Box(BossBar.Min.X + ProgressW * G->BossHpFrac - 3 * S, BossBar.Min.Y - 2 * S, 4 * S, 12 * S, Alpha(White, Struck));
		Meter(Rect(OX + 12 * S, NoticeY + 48 * S, ProgressW, 5 * S), G->BossClock / FMath::Max(1.f, G->BossEscapeT), G->bLastChance ? Red : Yellow);
		NoticeY += 73 * S;
	}
	if (Now >= G->CaptionT && Now - G->CaptionT < 3.f)
	{
		// a warning is at full strength from its first frame: it lands a touch large with a flash, it never fades in
		const float Age = Now - G->CaptionT, Landed = Flash01(Age, .22f);
		FPivot Land(*this, FVector2D(Center, NoticeY + 15 * S), 1.f + .14f * Flash01(Age, .16f));
		Plate(Rect(OX, NoticeY, ObjectiveW, 31 * S), Red, Ink);
		if (Landed > 0.f) Frame(OX, NoticeY, ObjectiveW, 31 * S, Alpha(Red, Landed), 2.f);
		Print(G->Caption, Center, NoticeY + 5 * S, Mix(Red, White, Landed), Strong, 1, true, ObjectiveW - 24 * S);
		NoticeY += 37 * S;
	}
	if (!G->Line.IsEmpty() && Now - G->LineT < G->LineHold + 1.2f)
	{
		const FLinearColor Accent = G->LinePri >= 3 ? Yellow : G->LinePri >= 2 ? Pink : Cyan;
		const float LineW = 480 * S, LineX = Center - LineW * .5f;
		const float Offset = bReducedMotion ? 0.f : (1.f - EaseOut(Now - G->LineT)) * 6 * S;
		const float Y = NoticeY - Offset;
		// the director's line fades out over its last third of a second instead of blinking off
		const float Left01 = bReducedMotion ? 1.f : FMath::Clamp((G->LineHold + 1.2f - (Now - G->LineT)) / .3f, 0.f, 1.f);
		// (a line of priority 2 or more is a warning of an attack: solid from its first frame; a remark fades in)
		FBlock Speak(*this, FMath::Min(G->LinePri >= 2 ? 1.f : In01(Now - G->LineT, 0.f, .14f), Left01));
		Plate(Rect(LineX, Y, LineW, 36 * S), Accent, Ink);
		Print(TEXT("DIRECTOR"), LineX + 12 * S, Y + 10 * S, Accent, LabelSize, 0, true);
		Print(G->Line, LineX + 108 * S, Y + 7 * S, White, Body, 0, false, LineW - 122 * S);
		NoticeY += 43 * S;
	}
	const bool bAction = Now >= G->ActionT && Now - G->ActionT < 1.f;
	if (bAction || Now - G->ShoutT < 1.f || Now - G->SceneSlateT < 2.8f)
	{
		const bool bShout = !bAction && Now - G->ShoutT < 1.f;
		const FString Text = bAction ? TEXT("ACTION!") : bShout ? G->ShoutWord
			: Level.Scenes.IsValidIndex(G->Scene) ? Level.Scenes[G->Scene].Name : FString();
		// ACTION! and a shout (HOP!, ROCKETS!, CHAIN!) are called out: they land large and flash; a scene's name slides in
		const float Age = bAction ? Now - G->ActionT : bShout ? Now - G->ShoutT : Now - G->SceneSlateT;
		const bool bCalled = bAction || bShout;
		const float Landed = bCalled ? Flash01(Age, .2f) : 0.f;
		// (ACTION! and an attack's shout are solid from their first frame; only a scene's name fades and slides in)
		FBlock Arrive(*this, bCalled ? 1.f : In01(Age, 0.f, .2f), 0.f, bCalled ? 0.f : -10 * S);
		FPivot Land(*this, FVector2D(Center, NoticeY + 17 * S), bCalled ? 1.f + .3f * Flash01(Age, .14f) : 1.f);
		Plate(Rect(Center - 148 * S, NoticeY, 296 * S, 34 * S), Yellow, Ink);
		if (Landed > 0.f) Frame(Center - 148 * S, NoticeY, 296 * S, 34 * S, Alpha(Yellow, Landed), 2.f);
		Print(Text, Center, NoticeY + 5 * S, Mix(Yellow, White, Landed), Strong, 1, true, 272 * S);
	}

	// The recording ledger and the latest three rewards stay out of the driving corridor.
	const float LedgerW = 255 * S, LX = R - LedgerW;
	{
		FBlock Arrive(*this, In01(PlayAge, .06f, DealTime), 48 * S, 0.f);
		Plate(Rect(LX, Top, LedgerW, 173 * S), Pink, Ink);
		// the camera's light: a slow blink while it films
		const float Rec = bReducedMotion || bCut ? 1.f : .5f + .5f * FMath::Square(FMath::Cos(G->Timecode * PI * .5f));
		Icon(EIcon::Record, LX + 10 * S, Top + 7 * S, 18 * S, Alpha(Pink, Rec));
		Print(TEXT("NOW FILMING"), LX + 33 * S, Top + 8 * S, White, LabelSize, 0, true);
		const int32 Tc = FMath::FloorToInt(G->Timecode);
		const int32 FrameNo = FMath::FloorToInt(FMath::Fmod(G->Timecode, 1.f) * 24.f);
		Print(FString::Printf(TEXT("%02d:%02d:%02d"), Tc / 60, Tc % 60, FrameNo), R - 12 * S, Top + 8 * S, Grey, LabelSize, 2);
		Print(TEXT("BOX OFFICE"), LX + 12 * S, Top + 30 * S, Grey, LabelSize);
		Print(Money(FMath::RoundToInt64(ScoreShown)), LX + 12 * S, Top + 46 * S, Mix(Yellow, White, .7f * Flash01(Now - ScoreUpAt, .3f)), Heading, 0, true, 228 * S);
		const FString Counts = G->Combo > 1 ? FString::Printf(TEXT("COMBO ×%d"), G->Combo)
			: FString::Printf(TEXT("KILLS %d    NEAR MISSES %d"), G->Kills, G->NearMisses);
		{
			// a combo that grows kicks; a kill or a near miss brightens the count
			const float Up = G->Combo > 1 ? Flash01(Now - ComboUpAt, PunchTime) : 0.f;
			FPivot Kick(*this, FVector2D(LX + 12 * S, Top + 93 * S), 1.f + .28f * Up);
			Print(Counts, LX + 12 * S, Top + 83 * S, G->Combo > 1 ? Mix(Pink, White, Up) : Mix(White, Cyan, Flash01(Now - CountsAt, .35f)), Body, 0, false, 227 * S);
		}
		if (G->Combo > 1)
		{
			// the time left to keep it: the bar blinks through its last second
			const float Ending = G->ComboT < 1.f && !bReducedMotion ? FMath::Square(FMath::Sin(Now * PI * 2.5f)) : 0.f;   // toward white, never dimmer
			Meter(Rect(LX + 135 * S, Top + 91 * S, 107 * S, 4 * S), G->ComboT / 3.f, Mix(Pink, White, .6f * Ending));
		}
		Print(FString::Printf(TEXT("STUNTS %d   /   COLLATERAL %d"), G->StuntCount, G->Collateral), LX + 12 * S, Top + 104 * S, Grey, LabelSize, 0, false, 227 * S);
		const int32 Time = FMath::FloorToInt(G->LevelClock), Par = ProgressData.Levels.IsValidIndex(G->Level) ? ProgressData.Levels[G->Level].Par : 0;
		Print(FString::Printf(TEXT("CARNAGE %d%%  /  %d:%02d  PAR %d:%02d"),
			FMath::RoundToInt(100.f * G->Run.Wrecked / FMath::Max(1, G->ScriptTotal)), Time / 60, Time % 60, Par / 60, Par % 60),
			LX + 12 * S, Top + 123 * S, Grey, LabelSize, 0, false, 227 * S);
		Print(FString::Printf(TEXT("FILM  %d/%d"), FMath::Max(0, G->Stats.Reels - G->Retakes), G->Stats.Reels), LX + 12 * S, Top + 145 * S, White, LabelSize);
		Meter(Rect(LX + 112 * S, Top + 152 * S, 130 * S, 6 * S), float(G->Stats.Reels - G->Retakes) / FMath::Max(1, G->Stats.Reels), Yellow, FMath::Clamp(G->Stats.Reels, 1, 10));
	}

	for (int32 I = FMath::Max(0, G->Popups.Num() - 3); I < G->Popups.Num(); ++I)
	{
		const FFTPopup& Popup = G->Popups[I];
		const float Opacity = bReducedMotion ? 1.f : FMath::Clamp((1.6f - Popup.T) * 3.f, 0.f, 1.f);
		const float Py = Top + (182 + (G->Popups.Num() - 1 - I) * 32) * S;
		const bool bRepair = (Popup.Text.StartsWith(TEXT("+")) && Popup.Text.EndsWith(TEXT(" ARMOR"))) || Popup.Text == TEXT("ARMOR FULL");
		const bool bMissile = (Popup.Text.StartsWith(TEXT("+")) && (Popup.Text.EndsWith(TEXT(" MISSILES")) || Popup.Text.EndsWith(TEXT(" MISSILE")))) || Popup.Text == TEXT("MISSILES FULL");
		const FLinearColor Color = Alpha(bRepair ? Green : bMissile ? Amber : Popup.Col, Opacity);
		// a reward slides in from the edge with a flash; a stunt (the big ones) with a wider bar and a longer flash
		const bool bBig = Popup.Big > 1.f;
		const float Landed = Flash01(Popup.T, bBig ? .3f : .18f);
		FBlock Arrive(*this, In01(Popup.T, 0.f, .13f), 26 * S, 0.f);
		Box(LX, Py, LedgerW, 27 * S, Alpha(Ink, Opacity));
		Box(LX, Py, (bBig ? 5 : 3) * S, 27 * S, Color);
		if (Landed > 0.f) Box(LX, Py, LedgerW, 27 * S, Alpha(bBig ? Color : White, (bBig ? .45f : .28f) * Landed));
		if (bRepair || bMissile) Icon(bRepair ? EIcon::Repair : EIcon::Missile, LX + 10 * S, Py + 4 * S, 17 * S, Color);
		Print(Popup.Text, LX + (bRepair || bMissile ? 35 : 12) * S, Py + 4 * S, Mix(Color, Alpha(White, Opacity), .6f * Landed), Body, 0, false, 207 * S);
	}
	if (G->bShowKeys && GI)
	{
		const FString Keys = FString::Printf(TEXT("DRIFT %s   /   TURBO %s   /   FIRE %s   /   MISSILE %s   /   AMMO %s   /   %s PAUSE"),
			*Hint(EFTAction::Drift, TEXT("A/RB")), *Hint(EFTAction::Turbo, TEXT("LB/RT")), *Hint(EFTAction::Fire, TEXT("X")),
			*Hint(EFTAction::Missile, TEXT("B")), *Hint(EFTAction::Ammo, TEXT("Y")), *InputHint(this, TEXT("P / ESC"), EKeys::Gamepad_Special_Right));
		// (the keys go after ten seconds of the race: they fade out over the last half second rather than blink off)
		FBlock Keyline(*this, bReducedMotion || G->Phase != EFTPhase::Play ? 1.f : FMath::Min(In01(PlayAge, .1f, DealTime), FMath::Clamp((10.f - G->PhaseT) / .5f, 0.f, 1.f)));
		Box(Center - 306 * S, H - 32 * S, 612 * S, 22 * S, Ink);
		Print(Keys, Center, H - 31 * S, Grey, LabelSize, 1, false, 588 * S);
	}
	if (G->Overlay == EFTOverlay::None) DrawTutorial(G);
}

void AFTHUD::DrawAttractWipe(AFTGameMode* G)
{
	const float Cover = G->AttractCover();
	if (Cover <= 0.f) return;
	if (bReducedMotion)
	{
		Box(0, 0, W, H, Alpha(Deep, Cover));
		return;
	}
	const bool bOut = G->AttractOutT >= 0.f;
	for (int32 I = 0; I < 5; ++I)
	{
		const float U = EaseOut(Cover * 1.08f - .02f * I, 1.f);
		if (U <= 0.f) continue;
		const bool bLeft = (I % 2 == 0) == bOut;
		const float X = bLeft ? 0.f : W * (1.f - U), Y = I * H * .2f;
		Box(X, Y, W * U, H * .2f + 1.f, Deep);
		Box(bLeft ? X + W * U - 3 * S : X, Y, 3 * S, H * .2f, I % 2 ? Cyan : Pink);
	}
}

void AFTHUD::DrawCard(AFTGameMode* G)
{
	const float Time = G->PhaseT;
	const float Mid = W * .5f;
	const float X = Mid - 320 * S, Y = H * .5f - 144 * S;
	// the slate is on film: a slim letterbox while it is up; the dim behind it eases in from the last screen's. Both
	// cut when the race starts (DrawPlay): nothing dims live play. (Backdrop)
	Backdrop(FMath::Lerp(.68f, .96f, FMath::Clamp((Time - 2.9f) / .3f, 0.f, 1.f)), bReducedMotion ? 0.f : ReelBar() * .5f);
	// The clapper: its arm is hinged at the left end, held open while the card drops in, lifts a little more, then
	// snaps shut (it meets the card as the clap sounds, 1.0 s in). The clap knocks the card and lights the stick.
	const float Closed = bReducedMotion ? 1.f : EaseIn(Time - ClapperClose, ClapperTravel);
	const float Shut = Time - ClapperClose - ClapperTravel;
	// the card lifts away over the slate's last quarter second (a skipped slate simply cuts)
	const float Leaving = bReducedMotion ? 0.f : EaseIn(Time - 2.95f, .25f);
	FBlock Arrive(*this, In01(bReducedMotion ? -1.f : Time, 0.f, DealTime), 0.f, -30 * S, 0.f, 5 * S * Flash01(Shut, .16f));
	FBlock Leave(*this, 1.f - Leaving, 0.f, -20 * S);
	Plate(Rect(X, Y, 640 * S, 310 * S), Cyan, Ink);
	{
		const float Lift = bReducedMotion ? 0.f : FMath::SmoothStep(ClapperClose - .22f, ClapperClose, Time);
		const float StickY = Y - 13 * S;
		FPivot Hinge(*this, FVector2D(X + 5 * S, StickY + 25 * S), 1.f, -(11.f + 3.f * Lift) * (1.f - Closed));
		Box(X, StickY, 640 * S, 28 * S, Steel);
		for (int32 I = 0; I < 10; ++I)
		{
			const float Px = X + (I * 64 + 7) * S;
			Stroke(FVector2D(Px, StickY + 23 * S), FVector2D(Px + 27 * S, StickY + 4 * S), I < 2 ? Cyan : White, 6.f);
		}
		const float Snap = Flash01(Shut, .14f);
		if (Snap > 0.f) Box(X, StickY, 640 * S, 28 * S, Alpha(White, .8f * Snap));
	}
	AccentSweep(Rect(X + 1 * S, Y + 17 * S, 638 * S, 4 * S), Shut, Cyan, FocusSweep);
	const FFTLevelDef& Level = G->Def();
	const int32 Roll = 40 + (G->GI() ? G->GI()->RunCounter : 1);
	Print(FString::Printf(TEXT("FULL THROTTLE  /  ROLL A-%d"), Roll), X + 22 * S, Y + 33 * S, Grey, LabelSize, 0, true);
	Print(G->Take > 1 ? TEXT("RESET THE CARS") : Level.Zone, X + 22 * S, Y + 65 * S, White, Readout, 0, true, 594 * S);
	Print(G->Take > 1 ? TEXT("FRESH PAINT ON THE KESTREL. ROLLING…") : Level.Title + TEXT("  /  ") + Level.Place,
		X + 22 * S, Y + 120 * S, Grey, Strong, 0, false, 594 * S);
	Box(X + 22 * S, Y + 160 * S, 596 * S, S, Rule);
	Print(TEXT("SCENE"), X + 24 * S, Y + 177 * S, Grey, LabelSize, 0, true);
	Print(TEXT("TAKE"), X + 232 * S, Y + 177 * S, Grey, LabelSize, 0, true);
	Print(TEXT("LEVEL"), X + 439 * S, Y + 177 * S, Grey, LabelSize, 0, true);
	Print(FString::Printf(TEXT("%02d / %02d"), G->Scene + 1, G->NumScenes()), X + 24 * S, Y + 197 * S, Cyan, Heading, 0, true);
	Print(FString::Printf(TEXT("%02d"), G->Take), X + 232 * S, Y + 197 * S, G->Take > 1 ? Red : White, Heading, 0, true);
	Print(FString::Printf(TEXT("%02d"), Level.Index + 1), X + 439 * S, Y + 197 * S, White, Heading, 0, true);
	if (Time > 1.2f)
	{
		FBlock Prompt(*this, bReducedMotion ? 1.f : EaseOut(Time - 1.2f, .18f));
		Print(ConfirmHint(this) + TEXT("   SKIP SLATE"), X + 615 * S, Y + 266 * S, Grey, Body, 2);
	}
}

void AFTHUD::DrawCut(AFTGameMode* G)
{
	const float Age = G->RealClockNow() - G->CutT;
	// the card lands like a slate shut hard (the sound is played here, on the frame the card appears)
	if (LastCutAge < 0.f && Age >= 0.f) PlayCue(TEXT("CutSlam"), .6f);
	LastCutAge = Age;
	if (Age < 0.f) return;
	const float Mid = W * .5f, Y = H * .32f;
	const FBox2D Card = Rect(Mid - 330 * S, Y, 660 * S, 112 * S);
	// CUT! is shouted: the card comes down large and fast, lands in a tenth of a second, flashes, and jolts
	const float Down = bReducedMotion ? 1.f : EaseIn(Age, .1f);
	const float Since = Age - .1f;
	const float Landed = Flash01(Since, .22f);
	FBlock Arrive(*this, .25f + .75f * Down, 0.f, 0.f, ShakeX(Since, 6 * S, .26f), 0.f);
	FPivot Slam(*this, Card.GetCenter(), 1.f + .34f * (1.f - Down));
	Plate(Card, Red, Ink);
	if (Landed > 0.f)
	{
		Box(Card.Min.X, Card.Min.Y, Card.GetSize().X, Card.GetSize().Y, Alpha(White, .3f * Landed));
		Frame(Card.Min.X, Card.Min.Y, Card.GetSize().X, Card.GetSize().Y, Alpha(White, Landed), 2.f);
	}
	const float Wipe = bReducedMotion ? 1.f : EaseOut(Since);
	Box(Card.Min.X, Card.Max.Y - 4 * S, (Card.GetSize().X - Corner * S) * Wipe, 3 * S, Red);
	AccentSweep(Rect(Card.Min.X + 12 * S, Y + 3 * S, Card.GetSize().X - 24 * S, 4 * S), Since, Red, MenuEntrance);
	Print(G->CutTitle, Mid, Y + (9.f + 6.f * (1.f - Wipe)) * S, Mix(Red, White, Landed), Readout, 1, true);
	Print(G->CutSub, Mid, Y + 64 * S, White, Strong, 1, false, 616 * S);
}

void AFTHUD::DrawReel(AFTGameMode* G)
{
	const float Mid = W * .5f, X = Left + Safe * S, R = Right - Safe * S;
	const float Bar = ReelBar();
	const float Entry = MenuAge(EMenuView::Reel);
	// The highlights' title card holds the picture dark behind it; card and dark clear together over its last fifth of
	// a second, so the first shot comes up from under them. The mattes are the ones CUT! closed (Backdrop): no cut.
	const float Holding = G->ReelTitleT > 0.f ? (bReducedMotion ? 1.f : FMath::Clamp(G->ReelTitleT / .2f, 0.f, 1.f)) : 0.f;
	Backdrop(.88f * Holding, Bar);
	const float ShotAge = MenuAge(EMenuView::Reel, true);
	if (G->ReelShot != LastReelShot)
	{
		// a splice between shots (not for the first frame of a reel: its title card has its own sound)
		if (LastReelShot >= 0 && !G->bReelDone && G->ReelTitleT <= 0.f && G->ReelShot > 1) PlayCue(TEXT("ReelClick"), .3f);
		LastReelShot = G->ReelShot;
	}
	if (!G->bReelDone)
		AccentSweep(Rect(R - 318 * S, Bar - 6 * S, 318 * S, 4 * S), ShotAge, Pink, FocusSweep);
	const FFTLevelDef& Level = G->Def();
	if (G->ReelTitleT > 0.f)
	{
		const FBox2D Card = Rect(Mid - 335 * S, H * .34f, 670 * S, 158 * S);
		FBlock Arrive(*this, In01(Entry, .04f, DealTime), 0.f, -28 * S);
		FBlock Leave(*this, Holding, 0.f, -16 * S);
		Plate(Card, Pink, Ink);
		AccentSweep(Rect(Card.Min.X + 12 * S, Card.Max.Y - 8 * S, Card.GetSize().X - 24 * S, 4 * S),
			Entry, Pink, MenuEntrance);
		Print(TEXT("ROADBURN PICTURES"), Mid, Card.Min.Y + 16 * S, Pink, LabelSize, 1, true);
		Print(TEXT("THE HIGHLIGHTS"), Mid, Card.Min.Y + 43 * S, White, Readout, 1, true, 630 * S);
		Print(Level.Zone + TEXT("  /  ") + Level.Title, Mid, Card.Min.Y + 104 * S, Grey, Strong, 1, false, 630 * S);
	}
	{
		FBlock Slug(*this, In01(Entry, 0.f, DealTime), -34 * S, 0.f);
		Icon(EIcon::Play, X, Bar * .5f - 10 * S, 22 * S, Pink);
		Print(TEXT("DAILIES / HIGHLIGHTS"), X + 35 * S, Bar * .5f - 11 * S, White, Strong, 0, true);
	}
	// each shot is slugged as it cuts in: the counter flashes, the label slides in from the left
	const float Spliced = G->bReelDone || G->ReelTitleT > 0.f ? 0.f : Flash01(ShotAge, .3f);
	Print(FString::Printf(TEXT("SHOT %02d / %02d  %s"), G->ReelShot, G->ReelShots, *G->ReelCamName),
		R, Bar * .5f - 10 * S, Mix(Grey, White, Spliced), Body, 2, false, 318 * S);
	{
		FBlock Label(*this, G->bReelDone ? 1.f : In01(ShotAge, 0.f, .18f), -18 * S, 0.f);
		Print(G->ReelLabel, X, H - Bar * .5f - 10 * S, Yellow, Strong, 0, true, 332 * S);
	}
	if (G->bReelSlow != bLastReelSlow) { bLastReelSlow = G->bReelSlow; ReelSlowAt = T; }
	if (G->bReelSlow)
	{
		FBlock Slow(*this, In01(T - ReelSlowAt, 0.f, .15f), 14 * S, 0.f);
		Print(TEXT("SLOW MOTION"), R, H - Bar * .5f - 8 * S, Pink, Body, 2, true);
	}
	if (G->RealClockNow() - G->ReelLoopT < 2.f) Print(TEXT("AGAIN, FROM THE TOP"), Mid, Bar + 18 * S, White, Strong, 1, true);
	DrawReelContinue(G);
}

void AFTHUD::DrawReelContinue(AFTGameMode* G)
{
	const float Bar = ReelBar();
	const FBox2D Action = Rect(W * .5f - 182 * S, H - Bar * .5f - 27 * S, 364 * S, 54 * S);
	const bool bArmed = G->PhaseT > AFTGameMode::ReelArmT;
	const EControl State = G->ReelHoldT > 0.f ? EControl::Pressed : ControlState(Action, true, bArmed || G->bReelDone);
	G->ReelContinueRect = Control(Action, G->bReelDone ? TEXT("CONTINUE") : TEXT("HOLD TO SKIP"),
		G->bReelDone ? ConfirmHint(this) + TEXT(" / CLICK") : TEXT("HOLD ") + ConfirmHint(this) + TEXT(" / CLICK"), State, Cyan);
	if (G->bReelDone)
	{
		// the reel has played: the button says so (its light runs once and it brightens)
		const float Ready = MenuAge(EMenuView::Reel, true);
		FocusAccent(Action, Ready, Cyan);
		const float Lit = Flash01(Ready, .3f);
		if (Lit > 0.f) Box(Action.Min.X, Action.Min.Y, Action.GetSize().X, Action.GetSize().Y, Alpha(White, .22f * Lit));
	}
	if (!G->bReelDone)
		Meter(Rect(Action.Min.X, Action.Max.Y + 5 * S, Action.GetSize().X, 4 * S),
			G->ReelHoldT / FMath::Max(.01f, FFTProgressData::Get().T(TEXT("HoldToSkip"), 1.f)), Cyan);
}

void AFTHUD::DrawRating(AFTGameMode* G)
{
	// the reel's mattes slide away and its picture dims as the panels come in from either side (Backdrop)
	Backdrop(.83f, 0.f, true);
	const float X = Left + 184 * S, Y = (H - CanvasHeight * S) * .5f + 97 * S, R = Right - 184 * S;
	const FFTLevelDef& Level = G->Def();
	const float Time = G->PhaseT, Age = bReducedMotion ? -1.f : Time;
	// the panels come in on a sweep (with REDUCED MOTION everything is simply there: the stamp is the screen's one sound)
	if (!bReducedMotion && Crossed(LastPhaseT, Time, 0.f)) PlayCue(TEXT("UiOpen"), .5f);
	{
		FBlock Head(*this, In01(Age, 0.f, DealTime), 0.f, -22 * S);
		Print(TEXT("THE DAILIES"), X, Y - 57 * S, White, Heading, 0, true);
		Print(Level.Zone + TEXT("  /  ") + Level.Title, R, Y - 47 * S, Grey, Strong, 2, false, 500 * S);
		for (int32 I = 0; I < 12; ++I) Flat(X + I * 16 * S, Y - 16 * S, 10 * S, 5 * S, I % 2 == 0 ? Ink : White);   // the clapper's stripes under the title
	}
	{
		FBlock Arrive(*this, In01(Age, .04f, DealTime), -36 * S, 0.f);
		Plate(Rect(X, Y, 442 * S, 464 * S), Cyan, Ink, true);   // (the skin) read at length: paper
		AccentSweep(Rect(X + 1 * S, Y + S, 440 * S, 4 * S), G->PhaseT, Cyan, MenuEntrance);
		Print(TEXT("BOX OFFICE BREAKDOWN"), X + 21 * S, Y + 18 * S, White, Strong, 0, true, 400 * S);
		Flat(X + 20 * S, Y + 50 * S, 402 * S, 1.34f * S, Ink);
		struct FRow { const TCHAR* Name; int64 Value; };
		const FRow Rows[] = { { TEXT("DESTRUCTION"), G->ScoreDestruction }, { TEXT("NEAR MISSES"), G->ScoreNear },
			{ TEXT("STUNTS"), G->ScoreStunts }, { TEXT("ARMOR BONUS"), G->ArmorBonus }, { TEXT("COLLATERAL"), G->ScoreCollateral } };
		for (int32 I = 0; I < UE_ARRAY_COUNT(Rows); ++I)
		{
			const float Ry = Y + (67 + I * 51) * S;
			Box(X + 20 * S, Ry, 402 * S, 40 * S, Steel);
			Print(Rows[I].Name, X + 31 * S, Ry + 8 * S, White, Body);
			const float RowAge = G->PhaseT - (.3f + .3f * I);
			AccentSweep(Rect(X + 25 * S, Ry + 35 * S, 392 * S, 3 * S), RowAge, Cyan, FocusSweep);
			if (!bReducedMotion && Crossed(LastPhaseT, Time, .3f + .3f * I)) PlayCue(TEXT("CashTick"), .32f, 1.f + .07f * I);
			if (bReducedMotion || RowAge >= 0.f)
			{
				// each figure counts up and lands bright
				const float Counted = bReducedMotion ? 1.f : EaseOut(RowAge, CountTime);
				const int64 Figure = Counted >= 1.f ? Rows[I].Value : FMath::RoundToInt64(static_cast<double>(Rows[I].Value) * Counted / 10.0) * 10;
				// (ink on the card; it flashes cyan as it lands)
				Print(Money(Figure), X + 408 * S, Ry + 6 * S, Mix(Rows[I].Value < 0 ? Red : White, Cyan, Flash01(RowAge - CountTime, .25f)), Strong, 2, true, 179 * S);
			}
		}
		// the take's box office stands on an ink plaque at the foot of the card
		Flat(X + 14 * S, Y + 338 * S, 414 * S, 92 * S, Ink);
		MarkTone(Rect(X + 14 * S, Y + 338 * S, 414 * S, 92 * S), ETone::Dark);
		Print(TEXT("BOX OFFICE"), X + 22 * S, Y + 347 * S, Yellow, LabelSize, 0, true);
		if (!bReducedMotion && Crossed(LastPhaseT, Time, 1.9f)) PlayCue(TEXT("CashRoll"), .4f);
		if (!bReducedMotion && Crossed(LastPhaseT, Time, 1.9f + TotalTime)) PlayCue(TEXT("CashTotal"), .5f);
		if (G->PhaseT > 1.9f || bReducedMotion)
		{
			// the take's box office counts up for half a second and lands with weight
			const float TotalAge = Time - 1.9f - TotalTime;
			const float Counted = bReducedMotion ? 1.f : EaseOut(Time - 1.9f, TotalTime);
			const int64 Figure = Counted >= 1.f ? G->Score : FMath::RoundToInt64(static_cast<double>(G->Score) * Counted / 10.0) * 10;
			FPivot Land(*this, FVector2D(X + 22 * S, Y + 398 * S), 1.f + .1f * Flash01(TotalAge, PunchTime));
			Print(Money(Figure), X + 22 * S, Y + 373 * S, Mix(Yellow, White, Flash01(TotalAge, .3f)), Readout, 0, true, 399 * S);
		}
	}

	{
		// The rating is stamped: it comes down large and fast (0.13 s), lands, and the panel takes the knock; the bar
		// fills and the director's line follows only once it has landed.
		const float StampAge = Time - 2.7f, Hit = StampAge - StampTime;
		FBlock Arrive(*this, In01(Age, .1f, DealTime), 36 * S, 0.f, ShakeX(Hit, 6 * S, .3f), 0.f);
		Plate(Rect(R - 442 * S, Y, 442 * S, 464 * S), Pink, Ink, true);   // (the skin) read at length: paper
		AccentSweep(Rect(R - 441 * S, Y + S, 440 * S, 4 * S), G->PhaseT - .075f, Pink, MenuEntrance);
		const float RatingX = R - 420 * S;
		Print(TEXT("DIRECTOR'S RATING"), RatingX, Y + 18 * S, White, Strong, 0, true, 398 * S);
		Flat(RatingX, Y + 50 * S, 398 * S, 1.34f * S, Ink);
		const int32 Rating = FMath::Clamp(G->RatingIdx, 0, 3);
		if (Crossed(LastPhaseT, Time, bReducedMotion ? 0.f : 2.7f + StampTime)) PlayCue(Rating >= 2 ? TEXT("StampGold") : TEXT("Stamp"), .65f);
		if (G->PhaseT >= 2.7f || bReducedMotion)
		{
			static const TCHAR* Names[] = { TEXT("B-MOVIE"), TEXT("ACTION HERO"), TEXT("BLOCKBUSTER"), TEXT("LEGENDARY DIRECTOR") };
			static const FLinearColor Colors[] = { White, Cyan, Pink, Yellow };
			const FLinearColor Color = Colors[Rating];
			const float Down = bReducedMotion ? 1.f : EaseIn(StampAge, StampTime);
			const float Landed = Flash01(Hit, .32f);
			const float After = bReducedMotion ? 1.f : EaseOut(Hit, .3f);
			const float StampY = Y + 118 * S;
			const FVector2D NameSize = TextSize(Names[Rating], Heading, true);
			{
				FPivot Stamp(*this, FVector2D(RatingX + FMath::Min<float>(NameSize.X, 398 * S) * .5f, StampY + NameSize.Y * .5f),
					1.f + .9f * (1.f - Down), -5.f * (1.f - Down));
				Print(Names[Rating], RatingX, StampY, Alpha(Mix(Color, White, Landed), .25f + .75f * Down), Heading, 0, true, 398 * S);
			}
			// The rating is a rubber stamp: once it has landed, a double rule of its own ink stands round the name
			// (B-MOVIE in plain ink, the others in the ink of their colour). (TODO kit: FTSkin::Stamp, the film can.)
			const FLinearColor StampInk = InkFor(Color);
			if (Down >= 1.f)
			{
				Frame(RatingX - 10 * S, StampY - 8 * S, 418 * S, NameSize.Y + 16 * S, Alpha(StampInk, .9f), 3.f);
				Frame(RatingX - 5 * S, StampY - 3 * S, 408 * S, NameSize.Y + 6 * S, Alpha(StampInk, .7f), 1.f);
			}
			if (Landed > 0.f) Flat(RatingX - 10 * S, StampY - 8 * S, 418 * S, NameSize.Y + 16 * S, Alpha(Color, .3f * Landed));
			Meter(Rect(RatingX, Y + 182 * S, 397 * S, 12 * S), (Rating + 1) / 4.f * After, StampInk, 4);   // the four pips
			Flat(RatingX, Y + 209 * S, 396 * S * After, 1.34f * S, StampInk);
			FBlock Quip(*this, bReducedMotion ? 1.f : EaseOut(Hit - .12f, .25f));
			Print(G->RatingQuip, RatingX, Y + 233 * S, White, Strong, 0, false, 398 * S);
		}
		if (G->PhaseT > 3.f && (G->bOutOfFilm || G->bEscaped))
		{
			const bool bFilm = G->bOutOfFilm;
			FBlock Verdict(*this, bReducedMotion ? 1.f : EaseOut(Time - 3.f, .25f), 0.f, 8 * S);
			Print(bFilm ? TEXT("OUT OF FILM") : TEXT("TARGET ESCAPED"), RatingX, Y + 309 * S, bFilm ? Red : Amber, Strong, 0, true);
			Print(bFilm ? TEXT("Kill fee only. The level is reshot.") : TEXT("Rated at most Action Hero. The level is reshot."),
				RatingX, Y + 340 * S, Grey, Body, 0, false, 398 * S);
		}
	}
	const FBox2D Continue = Rect(R - 442 * S, Y + 481 * S, 442 * S, 55 * S);
	{
		FBlock Arrive(*this, In01(Age, .16f, .2f), 0.f, 18 * S);
		Marquee(Rect(Continue.Min.X - 7 * S, Continue.Min.Y - 7 * S, Continue.GetSize().X + 14 * S, Continue.GetSize().Y + 14 * S));   // the hero action stands in lights
		G->ResultsContinueRect = Control(Continue, TEXT("CONTINUE TO PAYCHECK"), ConfirmHint(this),
			ControlState(Continue, true, G->PhaseT > 2.9f), Cyan);
		FocusAccent(Continue, bReducedMotion ? -1.f : Time - 2.9f, Cyan);   // armed: its light runs once
		Print(InputHint(this, TEXT("R"), EKeys::Gamepad_FaceButton_Top) + TEXT("   RESHOOT     ") + InputHint(this, TEXT("B"), EKeys::Gamepad_FaceButton_Right) + TEXT("   TITLE     ")
			+ InputHint(this, TEXT("ESC"), EKeys::Gamepad_Special_Right) + TEXT("   PAUSE"), X + 2 * S, Y + 499 * S, Grey, Body);
	}
}

TArray<FString> AFTHUD::WrapText(const FString& Text, float MaxWidth, float FontHeight, bool bDisplay)
{
	TArray<FString> Words, Lines;
	Text.ParseIntoArrayWS(Words);
	FString Line;
	for (const FString& Word : Words)
	{
		const FString Candidate = Line.IsEmpty() ? Word : Line + TEXT(" ") + Word;
		if (!Line.IsEmpty() && TextSize(Candidate, FontHeight, bDisplay).X > MaxWidth)
		{
			Lines.Add(MoveTemp(Line));
			Line = Word;
		}
		else Line = Candidate;
	}
	if (!Line.IsEmpty()) Lines.Add(MoveTemp(Line));
	return Lines;
}

void AFTHUD::DrawPreProduction(AFTGameMode* G)
{
	const UFTCareerSubsystem* Career = UFTCareerSubsystem::Get(G);
	if (!Career) return;
	const FFTProgressData& Data = FFTProgressData::Get();
	G->BuildTiles();
	const float X = Left + Safe * S, R = Right - Safe * S, Y = (H - CanvasHeight * S) * .5f;
	const float TabY = Y + 106 * S, BoardY = Y + 192 * S, Bottom = Y + 618 * S;
	const float BoardW = 806 * S, DetailX = R - 389 * S;
	const bool bSkills = G->Tab == EFTTab::Skills, bGarage = G->Tab == EFTTab::Garage;
	const bool bShop = G->Tab == EFTTab::Custom || G->Tab == EFTTab::Sfx;
	const bool bExtras = G->Tab == EFTTab::Extras;
	const int32 Selection = G->SelIndex();
	// The departments are dealt: their cards come up one after another when a tab opens and the detail slides in with
	// each choice. The skill tree is restrained (the owner's word): a short fade, no travel, and the chosen node only
	// brightens.
	const EMenuView View = bSkills ? EMenuView::Skills : EMenuView::Department;
	const float DepartmentAge = MenuAge(View);
	const float SelectionAge = MenuAge(View, true);
	const float Sweeps = bSkills ? -1.f : DepartmentAge;
	const float Age = bReducedMotion ? -1.f : G->PhaseT;
	const float Chosen = FocusFlash(View);

	Backdrop(.88f, 0.f, true);
	{
		FBlock Head(*this, In01(Age, 0.f, DealTime), 0.f, -24 * S);
		Plate(Rect(X, Y + 24 * S, R - X, 67 * S), Pink, Ink);
		Print(TEXT("PRE-PRODUCTION"), X + 16 * S, Y + 32 * S, White, Heading, 0, true);
		FString Header, Destination;
		G->PPHeader().Split(TEXT(" · "), &Header, &Destination);
		Print(Destination, X + 18 * S, Y + 66 * S, Grey, Body, 0, false, 533 * S);

		const int32 Driver = Career->C.Level;
		const int64 Lo = Data.TotalXPFor(Driver), Hi = Driver >= 30 ? Lo : Data.TotalXPFor(Driver + 1);
		const float Rx = R - 631 * S;
		Print(FString::Printf(TEXT("DRIVER LV %d"), Driver), Rx, Y + 34 * S, Cyan, Strong, 0, true, 155 * S);
		Print(FString::Printf(TEXT("%s / %s XP"), *Num(Career->C.XP), *Num(Hi)), Rx, Y + 61 * S, Grey, LabelSize, 0, false, 147 * S);
		Meter(Rect(Rx, Y + 81 * S, 151 * S, 3 * S), Hi > Lo ? float(Career->C.XP - Lo) / float(Hi - Lo) : 1.f, Cyan);
		Print(TEXT("SKILL POINTS"), Rx + 181 * S, Y + 34 * S, Grey, LabelSize, 0, true);
		// (the skin) the skill points stand on a cyan token and the cash on an amber price tag (TODO kit: FTSkin::Sticker)
		Flat(Rx + 177 * S, Y + 54 * S, 100 * S, 27 * S, Ink);
		Flat(Rx + 178.5f * S, Y + 55.5f * S, 97 * S, 24 * S, Cyan);
		MarkTone(Rect(Rx + 177 * S, Y + 54 * S, 100 * S, 27 * S), ETone::Light);
		Print(FString::Printf(TEXT("%d / %d"), Career->SPLeft(), Career->SPEarned()), Rx + 185 * S, Y + 57 * S, White, Strong, 0, true, 86 * S);
		Print(TEXT("CASH"), R - 16 * S, Y + 34 * S, Grey, LabelSize, 2, true);
		Flat(R - 196 * S, Y + 54 * S, 184 * S, 27 * S, Ink);
		Flat(R - 194.5f * S, Y + 55.5f * S, 181 * S, 24 * S, Yellow);
		MarkTone(Rect(R - 196 * S, Y + 54 * S, 184 * S, 27 * S), ETone::Light);
		Print(Money(Career->C.Cash), R - 20 * S, Y + 57 * S, White, Strong, 2, true, 168 * S);
	}

	static const TCHAR* Tabs[] = { TEXT("SKILLS"), TEXT("GARAGE"), TEXT("CUSTOM"), TEXT("SFX DEPT"), TEXT("EXTRAS") };
	G->TabRects.SetNum(UE_ARRAY_COUNT(Tabs));
	for (int32 I = 0; I < UE_ARRAY_COUNT(Tabs); ++I)
	{
		const FBox2D TabRect = Rect(X + I * 178 * S, TabY, 170 * S, 44 * S);
		const bool bCurrent = I == static_cast<int32>(G->Tab);
		FBlock Dealt(*this, In01(Age, .05f + I * .025f, .2f), 0.f, -14 * S);
		G->TabRects[I] = Control(TabRect, Tabs[I], FString(), ControlState(TabRect, bCurrent), Cyan);
		if (bCurrent) FocusAccent(TabRect, DepartmentAge, Cyan);
	}
	{
		FBlock Help(*this, In01(Age, .2f, .2f));
		Print(InputHint(this, TEXT("Q / E"), EKeys::Gamepad_LeftShoulder) + TEXT(" / ") + InputHint(this, TEXT("TAB"), EKeys::Gamepad_RightShoulder) + TEXT("   DEPARTMENT"), R, TabY + 13 * S, Grey, LabelSize, 2, false, 307 * S);
	}

	const int32 Cols = bSkills ? 5 : bGarage ? 3 : bExtras ? 4 : 6;
	const int32 Rows = bSkills ? 6 : bGarage ? 10 : bExtras ? 2 : 7;
	const float RowLabel = bGarage ? 157 * S : 0.f;
	const float CellW = (BoardW - RowLabel) / Cols;
	const float CellH = FMath::Min((Bottom - BoardY) / Rows, (bExtras ? 104.f : 96.f) * S);
	int32& Scroll = G->TabScroll[static_cast<int32>(G->Tab)];
	if (G->Tiles.IsValidIndex(Selection))
	{
		const int32 Row = G->Tiles[Selection].Row;
		Scroll = bShop ? FMath::Clamp(Scroll, FMath::Max(0, Row - Rows + 1), Row) : 0;
	}
	static const TCHAR* States[] = { TEXT("LOCKED"), TEXT("AVAILABLE"), TEXT("PENDING"), TEXT("OWNED"), TEXT("EQUIPPED"), TEXT("COMING SOON") };
	const auto StateColor = [](EFTTile State)
	{
		switch (State)
		{
		case EFTTile::Available: return Cyan;
		case EFTTile::Pending: return Yellow;
		case EFTTile::Owned: case EFTTile::Equipped: return Green;
		default: return Grey;
		}
	};
	// what the last purchase or equip said: the chosen card flashes green, a refusal shakes the button and flashes it red
	const bool bSaid = T - G->PPMsgT < 2.2f && !G->PPMsg.IsEmpty();
	const float Agreed = bSaid && !G->bPPMsgBad ? Flash01(T - G->PPMsgT, .4f) : 0.f;
	const float Refused = bSaid && G->bPPMsgBad ? T - G->PPMsgT : -1.f;
	{
		FBlock Board(*this, In01(Age, .08f, DealTime), bSkills ? 0.f : -30 * S, 0.f);   // (the skill tree's board does not travel)
		Plate(Rect(X, BoardY - 32 * S, BoardW, Bottom - BoardY + 32 * S), Cyan, Ink);
		AccentSweep(Rect(X + 2 * S, BoardY - 30 * S, BoardW - 4 * S, 4 * S), Sweeps, Cyan, MenuEntrance);
		if (bSkills)
			for (int32 I = 0; I < Data.Branches.Num() && I < Cols; ++I)
				Print(Data.Branches[I].Name, X + (I + .5f) * CellW, BoardY - 22 * S, Grey, LabelSize, 1, true, CellW - 8 * S);
		if (bGarage)
		{
			Print(TEXT("KESTREL-88"), X + 10 * S, BoardY - 22 * S, Grey, LabelSize, 0, true);
			for (int32 Rank = 1; Rank <= Cols; ++Rank)
				Print(FString::Printf(TEXT("RANK %d"), Rank), X + RowLabel + (Rank - .5f) * CellW, BoardY - 22 * S, Grey, LabelSize, 1, true);
			static const TCHAR* Lines[] = { TEXT("ARMOR PLATING"), TEXT("BULL BAR"), TEXT("GUN BARRELS"), TEXT("MISSILE RACKS"),
				TEXT("BIG-BLOCK V8"), TEXT("CLOSE-RATIO GEARBOX"), TEXT("SUPERCHARGER"), TEXT("RACE TYRES"), TEXT("AP ROUNDS"), TEXT("HE ROUNDS") };
			for (int32 I = 0; I < UE_ARRAY_COUNT(Lines); ++I)
				Print(Lines[I], X + 10 * S, BoardY + I * CellH + 12 * S, White, LabelSize, 0, false, RowLabel - 15 * S);
		}
		if (bShop || bExtras)
		{
			Print(Tabs[static_cast<int32>(G->Tab)], X + 12 * S, BoardY - 22 * S, Grey, LabelSize, 0, true);
			const int32 Count = G->Tiles.IsEmpty() ? 0 : G->Tiles.Last().Row + 1;
			if (Count > Rows)
				Print(FString::Printf(TEXT("ROWS %d–%d / %d  UP-DOWN"), Scroll + 1, FMath::Min(Count, Scroll + Rows), Count),
					X + BoardW - 12 * S, BoardY - 22 * S, Grey, LabelSize, 2);
		}

		G->TileRects.Init(FBox2D(ForceInit), G->Tiles.Num());
		for (int32 I = 0; I < G->Tiles.Num(); ++I)
		{
			const FFTTileView& Tile = G->Tiles[I];
			if (Tile.Row < Scroll || Tile.Row >= Scroll + Rows || Tile.Col < 0 || Tile.Col >= Cols) continue;
			const float Tx = X + RowLabel + Tile.Col * CellW + 4 * S, Ty = BoardY + (Tile.Row - Scroll) * CellH + 3 * S;
			const FBox2D TileRect = Rect(Tx, Ty, CellW - 8 * S, CellH - 6 * S);
			const bool bSelected = I == Selection;
			const EControl Interaction = ControlState(TileRect, bSelected);
			const bool bMouse = Interaction == EControl::Hover || Interaction == EControl::Pressed;
			const FLinearColor State = StateColor(Tile.State);
			const FVector2D Size = TileRect.GetSize();
			G->TileRects[I] = TileRect;
			// a department's cards are dealt from the top left corner; the skill tree's simply come up together
			FBlock Dealt(*this, bSkills ? In01(DepartmentAge, 0.f, QuietTime) : In01(DepartmentAge, .02f + (Tile.Row - Scroll + Tile.Col) * .016f, .18f),
				0.f, bSkills ? 0.f : 14 * S);
			// An index card per tile, by its state: owned is cream stock with a check badge, available is aqua, pending a
			// yellow sticky note, locked (and coming soon) a slate blank. The chosen card wears the cursor.
			// (TODO kit: the card stock, the APPROVED stamp and a pending card's paper clip are FTSkin textures.)
			const bool bOwnedCard = Tile.State == EFTTile::Owned || Tile.State == EFTTile::Equipped;
			const FLinearColor Card = bOwnedCard ? Paper : Tile.State == EFTTile::Available ? Aqua : Tile.State == EFTTile::Pending ? Sticky : Steel;
			Flat(Tx, Ty, Size.X, Size.Y, Ink);
			Flat(Tx + 1.34f * S, Ty + 1.34f * S, Size.X - 2.68f * S, Size.Y - 2.68f * S, Interaction == EControl::Pressed ? Mix(Card, Ink, .25f) : Card);
			MarkTone(TileRect, Lum(Card) > .35f ? ETone::Light : ETone::Dark);
			Flat(Tx, Ty, (bSelected ? 4 : 2) * S, Size.Y, bSelected ? Pink : State);
			if (bSelected) Cursor(TileRect);
			else if (bMouse) Frame(Tx, Ty, Size.X, Size.Y, Pink, 1.5f);
			if (bOwnedCard)
			{
				const float BadgeX = TileRect.Max.X - (Tile.bSwatch ? 31 : 17) * S, BadgeY = Ty + 5 * S;
				Flat(BadgeX, BadgeY, 12 * S, 12 * S, Ink);
				Stroke(FVector2D(BadgeX + 2.5f * S, BadgeY + 6 * S), FVector2D(BadgeX + 5 * S, BadgeY + 9 * S), Green, 2.f);
				Stroke(FVector2D(BadgeX + 5 * S, BadgeY + 9 * S), FVector2D(BadgeX + 9.5f * S, BadgeY + 3 * S), Green, 2.f);
			}
			if (bSelected)
			{
				if (!bSkills) FocusAccent(TileRect, SelectionAge, Cyan);
				if (Chosen > 0.f) Box(Tx, Ty, Size.X, Size.Y, Alpha(White, (bSkills ? .13f : .2f) * Chosen));
				if (Agreed > 0.f) Box(Tx, Ty, Size.X, Size.Y, Alpha(Green, .3f * Agreed));
			}
			if (Tile.bSwatch)
			{
				// (a swatch is its true colour: never through Box, which would turn a white or a steel into paper tones)
				Flat(TileRect.Max.X - 16.5f * S, Ty + 6.5f * S, 11 * S, 11 * S, Ink);
				Flat(TileRect.Max.X - 15 * S, Ty + 8 * S, 8 * S, 8 * S, Tile.Color);
			}
			const float AvailableWidth = Size.X - ((Tile.bSwatch ? 27 : 17) + (bOwnedCard ? 15 : 0)) * S;
			Print(Tile.Title, Tx + 9 * S, Ty + 3 * S, Tile.State == EFTTile::Locked || Tile.State == EFTTile::Soon ? Grey : White,
				bGarage ? LabelSize : Body, 0, false, AvailableWidth);
			if (!bGarage) Print(Tile.Sub, Tx + 9 * S, Ty + 23 * S, Grey, LabelSize, 0, false, Size.X - 18 * S);
			const bool bMaxed = bGarage && Tile.Col == Cols - 1 && Tile.State == EFTTile::Owned;
			Print(bMaxed ? TEXT("MAX RANK") : States[static_cast<int32>(Tile.State)], Tx + 9 * S, TileRect.Max.Y - 17 * S,
				State, 13.f, 0, true, Size.X - 18 * S);
		}
	}

	G->BuyRect = FBox2D(ForceInit);
	G->UndoRect = FBox2D(ForceInit);
	{
		FBlock Panel(*this, In01(Age, .12f, DealTime), bSkills ? 0.f : 30 * S, 0.f);
		Plate(Rect(DetailX, BoardY - 32 * S, R - DetailX, Bottom - BoardY + 32 * S), Pink, Ink, true);   // (the skin) read at length: paper
		// ... a sheet on a clipboard: its clip (TODO kit: the board and the clip are FTSkin textures)
		Flat(DetailX + (R - DetailX) * .5f - 36 * S, BoardY - 40 * S, 72 * S, 16 * S, Ink);
		Flat(DetailX + (R - DetailX) * .5f - 33 * S, BoardY - 37 * S, 66 * S, 10 * S, Steel);
		AccentSweep(Rect(DetailX + 2 * S, BoardY - 30 * S, R - DetailX - 4 * S, 4 * S),
			bSkills ? -1.f : DepartmentAge - .06f, Pink, MenuEntrance);
		if (G->Tiles.IsValidIndex(Selection))
		{
			const FFTTileView& Tile = G->Tiles[Selection];
			const float Dx = DetailX + 17 * S, Dw = R - Dx - 17 * S;
			const float ActionY = Bottom - (bSkills ? 100 : 67) * S;
			const bool bCanAfford = Tile.State != EFTTile::Available || !Tile.bCash || Tile.Cost <= Career->C.Cash;
			{
				// the detail follows the choice at once: it is never clear, it only settles (and in the skill tree it does not travel)
				FBlock Read(*this, .45f + .55f * In01(SelectionAge, 0.f, .12f), bSkills ? 0.f : 18 * S, 0.f);
				float Dy = BoardY - 17 * S;
				Print(Tile.Group.IsEmpty() ? Tabs[static_cast<int32>(G->Tab)] : Tile.Group.Replace(TEXT("·"), TEXT("/")),
					Dx, Dy, Grey, LabelSize, 0, true, Dw);
				Dy += 25 * S;
				for (const FString& Line : WrapText(Tile.Title, Dw, Strong + 3.f, true))
				{
					Print(Line, Dx, Dy, White, Strong + 3.f, 0, true, Dw);
					Dy += 27 * S;
				}
				if (Tile.bSwatch)
				{
					Flat(Dx - 1.5f * S, Dy + .5f * S, Dw + 3 * S, 17 * S, Ink);
					Flat(Dx, Dy + 2 * S, Dw, 14 * S, Tile.Color);   // (the true colour: see the tile's swatch)
					Dy += 24 * S;
				}
				Print(States[static_cast<int32>(Tile.State)], Dx, Dy + 4 * S, StateColor(Tile.State), LabelSize, 0, true, Dw * .52f);
				Print(Tile.Price, R - 17 * S, Dy, bCanAfford ? Yellow : Red, Strong, 2, false, Dw * .47f);
				Dy += 31 * S;
				Box(Dx, Dy, Dw, S, Rule);
				if (!bSkills) AccentSweep(Rect(Dx, Dy, Dw, 3 * S), SelectionAge - .035f, Pink, FocusSweep);
				Dy += 13 * S;
				if (bGarage)
				{
					TArray<FString> Parts;
					Tile.Id.ParseIntoArray(Parts, TEXT(":"));
					if (Parts.IsValidIndex(1))
					{
						const int32 Rank = Career->Rank(FName(*Parts[1]));
						Print(Rank >= 3 ? TEXT("FULLY UPGRADED  /  RANK 3") : FString::Printf(TEXT("INSTALLED RANK %d  /  %d"), Rank, Cols),
							Dx, Dy, Cyan, LabelSize, 0, true);
						Dy += 23 * S;
					}
				}
				const TArray<FString> Desc = WrapText(Tile.Desc, Dw, Body);
				const TArray<FString> Needs = WrapText(Tile.Needs, Dw, LabelSize);
				const float NeedsSpace = Needs.IsEmpty() ? 0.f : (21 + Needs.Num() * 16) * S;
				for (const FString& Line : Desc)
				{
					if (Dy + 20 * S > ActionY - NeedsSpace - 9 * S) break;
					Print(Line, Dx, Dy, White, Body, 0, false, Dw);
					Dy += 19 * S;
				}
				if (!Needs.IsEmpty())
				{
					Print(TEXT("REQUIREMENTS"), Dx, Dy + 4 * S, Red, LabelSize, 0, true);
					Dy += 23 * S;
					for (const FString& Line : Needs)
					{
						if (Dy + 16 * S > ActionY - 4 * S) break;
						Print(Line, Dx, Dy, Grey, LabelSize, 0, false, Dw);
						Dy += 16 * S;
					}
				}
				else if (Tile.State == EFTTile::Available && !bCanAfford)
					Print(FString::Printf(TEXT("AVAILABLE CASH  %s"), *Money(Career->C.Cash)), Dx, Dy + 5 * S, Red, LabelSize, 0, false, Dw);
			}

			FString Action = TEXT("BUY"), Hint = Tile.bCash ? TEXT("HOLD ") + ConfirmHint(this) : ConfirmHint(this);
			const bool bEquipped = bShop && Tile.State == EFTTile::Equipped;
			const bool bRequiredSlot = Tile.Slot == TEXT("paint") || Tile.Slot == TEXT("rims") || Tile.Slot == TEXT("neon") || Tile.Slot == TEXT("nitro");
			if (bShop && Tile.State == EFTTile::Owned) { Action = TEXT("EQUIP"); Hint = ConfirmHint(this); }
			else if (bEquipped) { Action = bRequiredSlot ? TEXT("EQUIPPED") : TEXT("UNEQUIP"); Hint = bRequiredSlot ? TEXT("REQUIRED SLOT") : ConfirmHint(this); }
			else if (Tile.Id == TEXT("sequel") && Tile.State == EFTTile::Owned) { Action = TEXT("SWITCH"); Hint = ConfirmHint(this); }
			else if (Tile.State != EFTTile::Available) { Action = States[static_cast<int32>(Tile.State)]; Hint = Tile.State == EFTTile::Pending ? TEXT("KEPT ON READY") : FString(); }
			const bool bEnabled = !Career->bSaving && ((Tile.State == EFTTile::Available && bCanAfford)
				|| (Tile.State == EFTTile::Owned && (bShop || Tile.Id == TEXT("sequel"))) || (bEquipped && !bRequiredSlot));
			if (Career->bSaving) { Action = TEXT("SAVING..."); Hint = TEXT("PLEASE WAIT"); }
			const FBox2D Buy = Rect(Dx, ActionY, Dw, 48 * S);
			{
				// a refusal: the button shakes its head (the rectangle the mouse is read against does not move)
				FBlock No(*this, 1.f, 0.f, 0.f, ShakeX(bSkills ? -1.f : Refused, 6 * S, ShakeTime), 0.f);   // (not in the skill tree: its red frame says no)
				G->BuyRect = Control(Buy, Action, Hint, G->BuyHoldT > 0.f && bEnabled ? EControl::Pressed : ControlState(Buy, bEnabled, bEnabled), Cyan);
				const float Denied = Flash01(Refused, .35f);
				if (Denied > 0.f) Frame(Buy.Min.X, Buy.Min.Y, Dw, 48 * S, Alpha(Red, Denied), 2.f);
				if (Agreed > 0.f) Box(Buy.Min.X, Buy.Min.Y, Dw, 48 * S, Alpha(Green, .25f * Agreed));
			}
			if (Tile.bCash && Tile.State == EFTTile::Available)
				Meter(Rect(Dx, Buy.Max.Y + 3 * S, Dw, 3 * S), G->BuyHoldT / FMath::Max(.01f, Data.T(TEXT("HoldToBuy"), .5f)), Yellow);
			if (bSkills)
			{
				const FBox2D Undo = Rect(Dx, ActionY + 59 * S, Dw, 31 * S);
				G->UndoRect = Control(Undo, TEXT("UNDO"), InputHint(this, TEXT("BACKSPACE"), EKeys::Gamepad_FaceButton_Right), ControlState(Undo, false, !Career->bSaving && !Career->C.PendingSkills.IsEmpty()), Yellow);
			}
		}
		else Print(TEXT("NO ITEMS AVAILABLE"), DetailX + 18 * S, BoardY + 20 * S, Grey, Strong, 0, true);
	}

	FBlock Foot(*this, In01(Age, .16f, .2f), 0.f, 16 * S);
	const FBox2D Ready = Rect(R - 344 * S, Bottom + 13 * S, 344 * S, 54 * S);
	const FString ReadyLabel = G->bPPFromTitle || G->PPNextLevel < 0 ? TEXT("READY / TITLE") : FString::Printf(TEXT("READY / LEVEL %d"), G->PPNextLevel + 1);
	Marquee(Rect(Ready.Min.X - 6 * S, Ready.Min.Y - 6 * S, Ready.GetSize().X + 12 * S, Ready.GetSize().Y + 12 * S));   // the hero action stands in lights
	G->ReadyRect = Control(Ready, ReadyLabel, InputHint(this, TEXT("SPACE / CLICK"), EKeys::Gamepad_FaceButton_Top), ControlState(Ready, false, !Career->bSaving && G->PhaseT >= .3f), Cyan);
	// (main's test, word for word; bSaid above is this stream's, for the flash of an answer)
	const bool bFeedback = (Career->bSaving || T - G->PPMsgT < 2.2f) && !G->PPMsg.IsEmpty();
	if (bFeedback)
	{
		if (!bSkills)
			AccentSweep(Rect(X + 2 * S, Bottom + 39 * S, 336 * S, 3 * S),
				T - G->PPMsgT, G->bPPMsgBad ? Red : Green, Feedback);
		// what was said comes up into its line (it is not there to be missed)
		FBlock Said(*this, .5f + .5f * In01(T - G->PPMsgT, 0.f, .12f), 0.f, bSkills ? 0.f : 8 * S);
		Print(G->PPMsg.Replace(TEXT("·"), TEXT("/")), X + 2 * S, Bottom + 14 * S, G->bPPMsgBad ? Red : Green, Strong, 0, true, 836 * S);
	}
	else if (bSkills && !Career->C.bTipSeen && Career->SPLeft() > 0)
		Print(TEXT("Pick an available skill to spend a point. READY keeps pending changes."), X + 2 * S, Bottom + 16 * S, Yellow, Body, 0, false, 836 * S);
	Print(TEXT("ARROWS / D-PAD   MOVE     ") + ConfirmHint(this) + TEXT("   CONFIRM     ") + InputHint(this, TEXT("Q / E"), EKeys::Gamepad_LeftShoulder) + TEXT(" / ") + InputHint(this, TEXT("TAB"), EKeys::Gamepad_RightShoulder) + TEXT("   CHANGE TAB"),
		X + 2 * S, Bottom + 48 * S, Grey, Body, 0, false, 836 * S);
}

void AFTHUD::DrawPaycheck(AFTGameMode* G)
{
	Backdrop(.86f, 0.f, true);
	const float X = Left + 71 * S, R = Right - 71 * S, Y = (H - CanvasHeight * S) * .5f;
	const FFTLevelDef& Level = G->Def();
	const float Time = G->PhaseT, Age = bReducedMotion ? -1.f : Time;
	if (Crossed(LastPhaseT, Time, 0.f)) { PayLeveled = 0; LevelUpAt = -10.f; }
	{
		FBlock Head(*this, In01(Age, 0.f, DealTime), 0.f, -22 * S);
		Print(TEXT("PAYCHECK"), X, Y + 32 * S, White, Heading, 0, true);
		Print(Level.Zone + TEXT("  /  ") + Level.Title, R, Y + 42 * S, Grey, Strong, 2, false, 526 * S);
		for (int32 I = 0; I < 12; ++I) Flat(X + I * 16 * S, Y + 73 * S, 10 * S, 5 * S, I % 2 == 0 ? Ink : White);   // the clapper's stripes under the title
	}
	const FBox2D Cash = Rect(X, Y + 91 * S, 543 * S, 512 * S);
	const FBox2D Career = Rect(R - 573 * S, Y + 91 * S, 573 * S, 512 * S);
	const int32 Lines = G->bPayValid ? G->Pay.Pay.Lines.Num() : 0;
	const int32 Shown = bReducedMotion ? Lines : FMath::Clamp(FMath::FloorToInt((G->PhaseT - .3f) / .32f), 0, Lines);
	{
		// the statement is read out a line at a time: each slides in, its figure counts up and lands; then the total
		// counts up for half a second and lands with weight
		FBlock Arrive(*this, In01(Age, .04f, DealTime), -36 * S, 0.f);
		// the statement is a ledger: green-ruled stock, a red margin, three punch holes (TODO kit: FTSkin's ledger panel)
		Plate(Cash, Yellow, Ledger, true);
		Flat(Cash.Min.X + 20 * S, Cash.Min.Y + 2 * S, 1.34f * S, Cash.GetSize().Y - 4 * S, Alpha(Red, .6f));
		for (int32 I = 0; I < 3; ++I) Icon(EIcon::Record, Cash.Min.X + 4 * S, Cash.Min.Y + (90 + I * 140) * S, 14 * S, Ink);
		AccentSweep(Rect(Cash.Min.X + S, Cash.Min.Y + S, Cash.GetSize().X - 2 * S, 4 * S), G->PhaseT, Yellow, MenuEntrance);
		Print(TEXT("CASH STATEMENT"), Cash.Min.X + 28 * S, Cash.Min.Y + 14 * S, White, Strong, 0, true);
		Print(TEXT("MULT"), Cash.Max.X - 144 * S, Cash.Min.Y + 31 * S, Grey, LabelSize, 2);
		Print(TEXT("AMOUNT"), Cash.Max.X - 23 * S, Cash.Min.Y + 31 * S, Grey, LabelSize, 2);
		if (G->bPayValid)
		{
			const FFTCommitOut& Pay = G->Pay;
			const float RowH = FMath::Min(30.f, 343.f / FMath::Max(1, Lines)) * S;
			for (int32 I = 0; I < Shown; ++I)
			{
				const FFTPayLine& Line = Pay.Pay.Lines[I];
				const int64 Value = static_cast<int64>(FMath::RoundToDouble(Line.Amount / 10.0) * 10.0);
				const float Ry = Cash.Min.Y + 50 * S + I * RowH;
				const float ReadAt = .3f + (I + 1) * .32f, LineAge = Time - ReadAt;
				if (!bReducedMotion && Crossed(LastPhaseT, Time, ReadAt)) PlayCue(TEXT("CashTick"), .3f, 1.f + .045f * I);
				FBlock Read(*this, bReducedMotion ? 1.f : EaseOut(LineAge, .14f), -16 * S, 0.f);
				if (I % 2 == 0) Flat(Cash.Min.X + 22 * S, Ry, Cash.GetSize().X - 26 * S, RowH - 2 * S, LedgerBand);   // a green band of the ledger
				AccentSweep(Rect(Cash.Min.X + 16 * S, Ry + RowH - 3 * S, Cash.GetSize().X - 32 * S, 2 * S),
					LineAge, Yellow, FocusSweep);
				Print(Line.Label, Cash.Min.X + 30 * S, Ry + 3 * S, Line.bMult ? Cyan : White, Body, 0, false, (Line.bMult ? 255 : 305) * S);
				if (Line.bMult)
					Print(FString::Printf(TEXT("× %.3g"), Line.Mult), Cash.Max.X - 144 * S, Ry + 4 * S, Pink, LabelSize, 2, false, 85 * S);
				const float Counted = bReducedMotion ? 1.f : EaseOut(LineAge, CountTime);
				const int64 Figure = Counted >= 1.f ? Value : FMath::RoundToInt64(static_cast<double>(Value) * Counted / 10.0) * 10;
				// (ink on the ledger, in the display face; it flashes amber as it lands)
				Print(Money(Figure), Cash.Max.X - 23 * S, Ry + 3 * S, Mix(Value < 0 ? Red : White, Yellow, Flash01(LineAge - CountTime, .25f)), Body, 2, true, 119 * S);
			}
			// the ledger's two columns
			Flat(Cash.Max.X - 236 * S, Cash.Min.Y + 50 * S, 1.34f * S, Shown * RowH, Alpha(InkSoft, .45f));
			Flat(Cash.Max.X - 138 * S, Cash.Min.Y + 50 * S, 1.34f * S, Shown * RowH, Alpha(InkSoft, .45f));
			// The cheque, laid over the foot of the ledger: a stub, the payee, and the total in its own box.
			// (TODO kit: the cheque's stock, the paper clip and the director's signature are FTSkin textures.)
			const FBox2D Cheque = Rect(Cash.Min.X + 34 * S, Cash.Max.Y - 104 * S, Cash.GetSize().X - 24 * S, 108 * S);
			const FBox2D Sum = Rect(Cash.Max.X - 214 * S, Cash.Max.Y - 100 * S, 216 * S, 54 * S);
			Flat(Cheque.Min.X + 3.5f * S, Cheque.Min.Y + 3.5f * S, Cheque.GetSize().X, Cheque.GetSize().Y, Ink);
			Flat(Cheque.Min.X, Cheque.Min.Y, Cheque.GetSize().X, Cheque.GetSize().Y, Ink);
			Flat(Cheque.Min.X + 2 * S, Cheque.Min.Y + 2 * S, Cheque.GetSize().X - 4 * S, Cheque.GetSize().Y - 4 * S, Aqua);
			MarkTone(Cheque, ETone::Light);
			for (int32 I = 0; I < 13; ++I) Flat(Cheque.Min.X + 78 * S, Cheque.Min.Y + (6 + I * 8) * S, 2 * S, 3 * S, Ink);   // the stub's perforation
			Print(TEXT("STUB"), Cheque.Min.X + 40 * S, Cheque.Min.Y + 8 * S, Grey, LabelSize, 1);
			Icon(EIcon::Record, Cheque.Min.X + 22 * S, Cheque.Min.Y + 36 * S, 36 * S, Alpha(InkSoft, .7f));
			Print(TEXT("FULL THROTTLE PICTURES"), Cheque.Min.X + 90 * S, Cheque.Min.Y + 8 * S, White, LabelSize, 0, true, 190 * S);
			Print(TEXT("PAY TO THE ORDER OF"), Cheque.Min.X + 90 * S, Cheque.Min.Y + 34 * S, Grey, LabelSize, 0, false, 190 * S);
			Print(TEXT("AGENT N. KANE"), Cheque.Min.X + 90 * S, Cheque.Min.Y + 51 * S, White, Strong, 0, true, 190 * S);
			Flat(Cheque.Min.X + 90 * S, Cheque.Min.Y + 77 * S, 190 * S, 1.34f * S, Ink);
			Flat(Sum.Min.X, Sum.Min.Y, Sum.GetSize().X, Sum.GetSize().Y, Ink);
			MarkTone(Sum, ETone::Dark);
			Print(TEXT("TOTAL"), Sum.Min.X + 9 * S, Sum.Min.Y + 3 * S, Yellow, LabelSize, 0, true);
			const float TotalAt = .3f + Lines * .32f;
			if (!bReducedMotion && Crossed(LastPhaseT, Time, TotalAt)) PlayCue(TEXT("CashRoll"), .4f);
			if (!bReducedMotion && Crossed(LastPhaseT, Time, TotalAt + TotalTime)) PlayCue(TEXT("CashTotal"), .55f);
			if (Shown >= Lines)
			{
				const float Since = Time - TotalAt - TotalTime;
				const float Counted = bReducedMotion ? 1.f : EaseOut(Time - TotalAt, TotalTime);
				const int64 Figure = Counted >= 1.f ? Pay.Pay.Total : FMath::RoundToInt64(static_cast<double>(Pay.Pay.Total) * Counted / 10.0) * 10;
				AccentSweep(Rect(Sum.Min.X + 4 * S, Sum.Max.Y - 6 * S, Sum.GetSize().X - 8 * S, 3 * S),
					Time - TotalAt, Yellow, MenuEntrance);
				{
					FPivot Land(*this, FVector2D(Cash.Max.X - 22 * S, Cash.Max.Y - 66 * S), 1.f + .14f * Flash01(Since, PunchTime));
					Print(Money(Figure), Cash.Max.X - 8 * S, Cash.Max.Y - 83 * S, Mix(Yellow, White, Flash01(Since, .35f)), Heading, 2, true, 196 * S);
				}
				if (const UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(G))
				{
					FBlock Banked(*this, bReducedMotion ? 1.f : EaseOut(Since, .25f), 0.f, 8 * S);
					Print(FString::Printf(TEXT("CASH NOW  %s"), *Money(CS->C.Cash)), Cash.Max.X - 8 * S, Cash.Max.Y - 38 * S, Grey, Body, 2, false, 300 * S);
				}
			}
		}
		else Print(TEXT("NO PAYCHECK FOR A BENCHMARK RUN"), Cash.Min.X + 20 * S, Cash.Min.Y + 67 * S, Grey, Body);
	}
	{
		FBlock Arrive(*this, In01(Age, .1f, DealTime), 36 * S, 0.f);
		Plate(Career, Cyan, Ink, true);   // (the skin) read at length: paper
		Flat(Career.Min.X + 18 * S, Career.Min.Y + 43 * S, Career.GetSize().X - 36 * S, 1.34f * S, Ink);
		AccentSweep(Rect(Career.Min.X + S, Career.Min.Y + S, Career.GetSize().X - 2 * S, 4 * S),
			G->PhaseT - .075f, Cyan, MenuEntrance);
		Print(TEXT("CAREER PROGRESSION"), Career.Min.X + 20 * S, Career.Min.Y + 14 * S, White, Strong, 0, true);
		const float XPStart = .6f + Lines * .32f;
		if (G->bPayValid && (G->PhaseT > XPStart || bReducedMotion))
		{
			const FFTCommitOut& Pay = G->Pay;
			FBlock Told(*this, bReducedMotion ? 1.f : EaseOut(Time - XPStart, .22f), 0.f, 10 * S);
			const float XpRow = FMath::Min(27.f, 183.f / FMath::Max(1, Pay.XPLines.Num())) * S;
			for (int32 I = 0; I < Pay.XPLines.Num(); ++I)
			{
				const float Ry = Career.Min.Y + 51 * S + I * XpRow;
				Print(Pay.XPLines[I].Label, Career.Min.X + 20 * S, Ry, White, Body, 0, false, 386 * S);
				Print(FString::Printf(TEXT("+%s XP"), *Num(static_cast<int64>(Pay.XPLines[I].Amount))),
					Career.Max.X - 21 * S, Ry, Green, Body, 2, false, 127 * S);
			}
			const FFTProgressData& Data = FFTProgressData::Get();
			const float U = bReducedMotion ? 1.f : FMath::Clamp((G->PhaseT - XPStart - .4f) / 1.4f, 0.f, 1.f);
			const int64 Xp = Pay.XPBefore + (U >= 1.f ? Pay.XPGained : static_cast<int64>(double(Pay.XPGained) * U));
			const int32 Driver = Data.LevelForXP(Xp);
			const int64 Lo = Data.TotalXPFor(Driver), Hi = Driver >= 30 ? Lo : Data.TotalXPFor(Driver + 1);
			const float ProgressY = Career.Min.Y + 75 * S + Pay.XPLines.Num() * XpRow;
			Print(FString::Printf(TEXT("DRIVER LEVEL %d"), Driver), Career.Min.X + 20 * S, ProgressY, Cyan, Strong, 0, true);
			Print(FString::Printf(TEXT("%s / %s XP"), *Num(Xp), *Num(Hi)), Career.Max.X - 21 * S, ProgressY + 4 * S, Grey, Body, 2, false, 252 * S);
			Meter(Rect(Career.Min.X + 20 * S, ProgressY + 33 * S, Career.GetSize().X - 41 * S, 8 * S),
				Hi > Lo ? float(Xp - Lo) / float(Hi - Lo) : 1.f, Cyan, 12);

			int32 SkillPoints = 0, Leveled = 0;
			for (const TPair<int32, int32>& Up : Pay.LevelUps)
			{
				if (Up.Key > Driver) break;
				SkillPoints += Up.Value;
				++Leveled;
			}
			float RewardY = ProgressY + 57 * S;
			if (Leveled > PayLeveled)
			{
				// a level gained as the bar fills: the line lands with its own sting
				PayLeveled = Leveled;
				LevelUpAt = T;
				if (!bReducedMotion) PlayCue(TEXT("LevelUp"), .55f);
			}
			if (Leveled)
			{
				const float Up = Flash01(T - LevelUpAt, .4f);
				FPivot Land(*this, FVector2D(Career.GetCenter().X, RewardY + 17 * S), 1.f + .06f * Flash01(T - LevelUpAt, PunchTime));
				Marquee(Rect(Career.Min.X + 18 * S, RewardY, Career.GetSize().X - 36 * S, 35 * S));   // a level gained goes up in lights
				if (Up > 0.f) Flat(Career.Min.X + 18 * S, RewardY, Career.GetSize().X - 36 * S, 35 * S, Alpha(Yellow, .4f * Up));
				Print(FString::Printf(TEXT("LEVEL UP   /   +%d SKILL POINT%s"), SkillPoints, SkillPoints == 1 ? TEXT("") : TEXT("S")),
					Career.Min.X + 27 * S, RewardY + 6 * S, Mix(Yellow, White, Up), Strong, 0, true, 520 * S);
				RewardY += 43 * S;
			}
			if (U >= 1.f)
			{
				const float Done = bReducedMotion ? -1.f : Time - (XPStart + 1.8f);
				if (Pay.LegendarySP > 0)
				{
					FBlock Extra(*this, In01(Done, 0.f, .2f), -12 * S, 0.f);
					Print(FString::Printf(TEXT("FIRST LEGENDARY DIRECTOR HERE  /  +%d SP"), Pay.LegendarySP),
						Career.Min.X + 20 * S, RewardY, Yellow, LabelSize, 0, true, 530 * S);
					RewardY += 23 * S;
				}
				for (int32 I = 0; I < Pay.Unlocks.Num() && RewardY < Career.Max.Y - 24 * S; ++I)
				{
					const bool bLastRow = RewardY + 42 * S >= Career.Max.Y && I + 1 < Pay.Unlocks.Num();
					FBlock Unlocked(*this, In01(Done, .05f + I * .05f, .2f), -12 * S, 0.f);
					if (I % 2 == 0) Flat(Career.Min.X + 16 * S, RewardY - 1 * S, Career.GetSize().X - 32 * S, 20 * S, PaperRow);
					if (!bLastRow)
					{
						// a ticked box per unlock (TODO kit: the department's icon; two columns of chips is a layout ask)
						Flat(Career.Min.X + 20 * S, RewardY + 3 * S, 13 * S, 13 * S, Ink);
						Flat(Career.Min.X + 21.5f * S, RewardY + 4.5f * S, 10 * S, 10 * S, Paper);
						Stroke(FVector2D(Career.Min.X + 23 * S, RewardY + 10 * S), FVector2D(Career.Min.X + 26 * S, RewardY + 13 * S), InkFor(Green), 2.f);
						Stroke(FVector2D(Career.Min.X + 26 * S, RewardY + 13 * S), FVector2D(Career.Min.X + 31 * S, RewardY + 6 * S), InkFor(Green), 2.f);
					}
					Print(bLastRow ? FString::Printf(TEXT("+%d MORE UNLOCKS IN PRE-PRODUCTION"), Pay.Unlocks.Num() - I) : Pay.Unlocks[I],
						Career.Min.X + (bLastRow ? 20 : 40) * S, RewardY, Green, Body, 0, false, (bLastRow ? 530 : 510) * S);
					RewardY += 21 * S;
				}
			}
		}
	}

	const FBox2D Continue = Rect(R - 447 * S, Y + 622 * S, 447 * S, 54 * S);
	FBlock Foot(*this, In01(Age, .16f, .2f), 0.f, 18 * S);
	Marquee(Rect(Continue.Min.X - 7 * S, Continue.Min.Y - 7 * S, Continue.GetSize().X + 14 * S, Continue.GetSize().Y + 14 * S));   // the hero action stands in lights
	G->ResultsContinueRect = Control(Continue, TEXT("CONTINUE"), ConfirmHint(this),
		ControlState(Continue, true, G->PhaseT >= .6f), Cyan);
	FocusAccent(Continue, bReducedMotion ? -1.f : Time - .6f, Cyan);   // armed: its light runs once
	Print(InputHint(this, TEXT("R"), EKeys::Gamepad_FaceButton_Top) + TEXT("   RESHOOT     ") + InputHint(this, TEXT("B"), EKeys::Gamepad_FaceButton_Right) + TEXT("   TITLE     ")
		+ InputHint(this, TEXT("ESC"), EKeys::Gamepad_Special_Right) + TEXT("   PAUSE"), X, Y + 638 * S, Grey, Body);
}

// ---------------------------------------------------------------------------------------------- cosmetic overlays (07): wreck FX and boss finishers
void AFTHUD::DrawFx(AFTGameMode* G)
{
	const float Now = G->RealClockNow();
	if (Now - G->KaBoomT < .9f && G->Phase == EFTPhase::Play)
	{
		const float X = Left + Safe * S, Y = 25 * S;
		Plate(Rect(X, Y, 210 * S, 38 * S), Yellow, Ink);
		Print(G->KaBoom, X + Inset * S, Y + 8 * S, Yellow, Strong, 0, true, 182 * S);
	}
	if (!bReducedMotion && Now - G->FilmBurnT < .45f)
	{
		// Film Burn: the frame burns through for a moment
		const float A = 1.f - (Now - G->FilmBurnT) / 0.45f;
		Grad(0, 0, W * .17f, H, Alpha(Amber, .34f * A), Alpha(Amber, 0.f), true);
		Grad(W * .84f, 0, W * .16f, H, Alpha(Amber, 0.f), Alpha(Amber, .34f * A), true);
	}
	if (!G->Finisher.IsNone() && Now - G->FinisherT < 4.f && Now >= G->FinisherT)
	{
		const float U = Now - G->FinisherT;
		if (G->Finisher == TEXT("s-the-end-card") && U > 0.6f)
		{
			const float A = bReducedMotion ? 1.f : EaseOut(U - .6f, .65f);
			Print(TEXT("THE END"), W * .5f, H * .39f, Alpha(White, A), 72.f, 1, true);
			if (U >= 2.f) Print(TEXT("...?"), W * .5f + 185 * S, H * .39f + 78 * S, Red, Heading, 1, true);
		}
		if (G->Finisher == TEXT("s-supernova") && U < .6f && !bReducedMotion)
			Box(0, 0, W, H, Alpha(White, .35f * (1.f - U / .6f)));
	}
}

// ---------------------------------------------------------------------------------------------- menus
void AFTHUD::DrawPause(AFTGameMode* G)
{
	// the race is held: the picture dims quickly and the board drops in with its rows dealt down it (a quarter second)
	const float Entry = MenuAge(EMenuView::Pause);
	const float In = Entry < 0.f ? 1.f : EaseOut(Entry, MenuEntrance);
	OverlayDim(.68f);
	const UFTCareerSubsystem* Career = UFTCareerSubsystem::Get(G);
	const bool bRetrySave = Career && Career->bSaveError;
	const bool bRestartRace = G->CanRestartRace();
	const float ExtraHeight = (int32(bRestartRace) + int32(bRetrySave)) * 60.f;
	const float X = W * .5f - 251 * S, Y = (H - CanvasHeight * S) * .5f + 121 * S, WidthPx = 502 * S;
	const int32 PauseView = static_cast<int32>(EMenuView::Pause);
	const float Chosen = FocusFlash(EMenuView::Pause);
	FBlock Board(*this, In01(Entry, 0.f, .18f), 0.f, -20 * S);
	Plate(Rect(X, Y, WidthPx, (468 + ExtraHeight) * S), Yellow, Ink);
	AccentSweep(Rect(X + 2 * S, Y + 2 * S, WidthPx - 4 * S, 4 * S), Entry, Yellow, MenuEntrance);
	Print(TEXT("CAMERAS HOLDING"), X + (24.f + 9.f * (1.f - In)) * S, Y + 23 * S, White, Heading, 0, true);
	for (int32 I = 0; I < 10; ++I) Flat(X + (24 + I * 16) * S, Y + 59 * S, 10 * S, 4 * S, I % 2 == 0 ? White : Pink);   // the clapper's stripes under the heading
	const UFTInputSubsystem* Input = UFTInputSubsystem::Get(this);
	const bool bDisconnected = Input && Input->HasDisconnectedController();
	Print(bDisconnected ? TEXT("CONTROLLER DISCONNECTED") : G->ResumeDescription(), X + 24 * S, Y + 67 * S,
		bDisconnected ? Yellow : Grey, Body, 0, false, WidthPx - 48 * S);
	Print(TEXT("PAUSED"), X + WidthPx - 24 * S, Y + 30 * S, Yellow, LabelSize, 2, true);
	TArray<FString> Names = { TEXT("RESUME"), TEXT("OPTIONS"), TEXT("CONTROLS"), TEXT("QUIT TO TITLE"), TEXT("QUIT TO DESKTOP") };
	TArray<FString> Hints = { InputHint(this, TEXT("P / ESC"), EKeys::Gamepad_Special_Right),
		InputHint(this, TEXT("O"), EKeys::Gamepad_FaceButton_Bottom), InputHint(this, TEXT("C"), EKeys::Gamepad_FaceButton_Top),
		TEXT("SAVE AND RETURN"), TEXT("SAVE AND EXIT") };
	if (bRestartRace) { Names.Add(TEXT("RESTART RACE")); Hints.Add(TEXT("RETAKE THIS RACE")); }
	if (bRetrySave) { Names.Add(TEXT("RETRY SAVE")); Hints.Add(InputHint(this, TEXT("R"), EKeys::Gamepad_FaceButton_Bottom)); }
	const int32 Rows = Names.Num();
	for (int32 I = 0; I < Rows; ++I)
	{
		const FBox2D Row = Rect(X + 24 * S, Y + (108 + 60 * I) * S, WidthPx - 48 * S, 51 * S);
		const FLinearColor& Accent = I == 0 ? Cyan : Pink;
		const bool bSelected = G->bPendingSystemSave ? I == 0 : G->PauseRow == I;
		// the chosen row leans out a little and brightens as the choice lands on it
		const float Lean = FocusLevel(FocusKey(PauseView, 0, I), bSelected);
		FBlock Dealt(*this, In01(Entry, .03f + I * .022f, .16f), -18 * S, 0.f, 5 * S * Lean, 0.f);
		G->OverlayRects.Add(Row);
		Control(Row, G->bPendingSystemSave && I == 0 ? TEXT("STAY HERE") : Names[I], Hints[I], ControlState(Row, bSelected, !G->bPendingSystemSave || I == 0), Accent);   // (main: the row's label and when it can be chosen)
		if (bSelected)
		{
			FocusAccent(Row, MenuAge(EMenuView::Pause, true), Accent);
			if (Chosen > 0.f) Box(Row.Min.X, Row.Min.Y, Row.GetSize().X, Row.GetSize().Y, Alpha(White, .2f * Chosen));
		}
	}
	Print(G->bPendingSystemSave ? G->SystemMessage : bDisconnected ? TEXT("Reconnect your controller, or resume with the keyboard.")
		: TEXT("UP-DOWN   NAVIGATE     ") + ConfirmHint(this) + TEXT("   SELECT"),
		X + 24 * S, Y + (420 + ExtraHeight) * S, Grey, Body, 0, false, WidthPx - 48 * S);
}

void AFTHUD::DrawSystemDialog(AFTGameMode* G)
{
	OverlayDim(.86f);   // (overhaul) the dim eases in, and hands over from the pause menu's, instead of cutting
	const bool bFailure = G->SystemDialog == EFTSystemDialog::SaveFailed;
	const bool bNew = G->SystemDialog == EFTSystemDialog::NewGame;
	const bool bRestart = G->SystemDialog == EFTSystemDialog::RestartRace;
	const bool bExit = G->SystemAction == 1 || G->SystemAction == 2;
	const float X = W * .5f - 328 * S, Y = (H - CanvasHeight * S) * .5f + 157 * S;
	const FLinearColor Accent = bFailure ? Red : Amber;
	// (overhaul) a confirmation is a working screen, like OPTIONS: a short fade, nothing travels, and the answer the
	// choice lands on brightens. (UpdateMenuMotion counts this dialog as a page of the pause view.)
	const float Chosen = FocusFlash(EMenuView::Pause);
	FBlock Panel(*this, In01(MenuAge(EMenuView::Pause), 0.f, QuietTime));
	Plate(Rect(X, Y, 656 * S, 382 * S), Accent, Ink, true);   // (the skin) a question from the production office: a paper memo, ink text
	Print(G->bPendingSystemSave ? TEXT("SAVING PROGRESS...") : bFailure ? TEXT("SAVE COULD NOT COMPLETE") : bNew ? TEXT("START A NEW CAMPAIGN?") : bRestart ? TEXT("RESTART THIS RACE?") : TEXT("LEAVE THIS TAKE?"),
		X + 25 * S, Y + 25 * S, White, Heading, 0, true, 606 * S);
	FString Message = G->bPendingSystemSave ? G->SystemMessage : bNew ? TEXT("This replaces your campaign, including cash, XP, skills, gear and story progress. Your controls and display preferences are kept.") : G->SystemMessage;
	if (bFailure && Message.IsEmpty()) Message = TEXT("Your last successful save is still available. Retry the save, or cancel and stay here.");
	const TArray<FString> Lines = WrapText(Message, 596 * S, Strong);
	for (int32 I = 0; I < Lines.Num() && I < 5; ++I)
		Print(Lines[I], X + 29 * S, Y + (88 + 26 * I) * S, bFailure ? Amber : Grey, Strong);
	if (!bNew && !bFailure && !bRestart)
		Print(G->ResumeDescription(), X + 29 * S, Y + 221 * S, Cyan, Body, 0, false, 596 * S);
	const int32 Rows = G->bPendingSystemSave ? 1 : bFailure && bExit ? 3 : 2;
	const float Bw = (598.f - 12.f * (Rows - 1)) / Rows;
	for (int32 I = 0; I < Rows; ++I)
	{
		const TCHAR* Label = G->bPendingSystemSave ? TEXT("STAY HERE") : bFailure ? (I == 0 ? TEXT("RETRY") : I == 1 ? TEXT("CANCEL") : TEXT("QUIT WITHOUT SAVING"))
			: (I == 0 ? TEXT("CANCEL") : bNew ? TEXT("START NEW GAME") : bRestart ? TEXT("RESTART RACE") : G->SystemAction == 1 ? TEXT("SAVE AND QUIT") : TEXT("SAVE AND RETURN"));
		const FBox2D Row = Rect(X + (29 + I * (Bw + 12)) * S, Y + 273 * S, Bw * S, 57 * S);
		G->OverlayRects.Add(Control(Row, Label, FString(), ControlState(Row, G->bPendingSystemSave || G->SystemRow == I, true), I == 2 ? Red : Cyan));
		if (G->SystemRow == I && Chosen > 0.f) Box(Row.Min.X, Row.Min.Y, Row.GetSize().X, Row.GetSize().Y, Alpha(White, .16f * Chosen));
	}
	Print(TEXT("LEFT-RIGHT   CHOOSE     ") + ConfirmHint(this) + TEXT("   SELECT     ") + BackHint(this) + TEXT("   CANCEL"),
		X + 29 * S, Y + 349 * S, Grey, LabelSize, 0, false, 598 * S);
}

void AFTHUD::DrawSaveStatus(AFTGameMode* G)
{
	// Title and dialogs show their own save details; keep the transient status clear of their controls.
	if (G->Overlay != EFTOverlay::None || G->bAttract || G->Phase == EFTPhase::Title) return;
	const UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(G);
	if (!CS) return;
	FString Status;
	FLinearColor Color = Cyan;
	if (CS->bSaving) Status = TEXT("SAVING...");
	else if (CS->bSaveError) { Status = TEXT("SAVE FAILED  /  OPEN PAUSE TO RETRY"); Color = Amber; }
	else if (CS->LastSaveTime > 0 && FPlatformTime::Seconds() - CS->LastSaveTime < 3.) Status = TEXT("CHECKPOINT SAVED");
	if (!Status.IsEmpty())
	{
		const FBox2D R = Rect(W * .5f - 154 * S, H - 27 * S, 308 * S, 23 * S);
		// (overhaul) CHECKPOINT SAVED rises into its place at the bottom edge and fades out over its last third of a
		// second; SAVING and SAVE FAILED stand for as long as they are true and are simply there
		const float SavedFor = static_cast<float>(FPlatformTime::Seconds() - CS->LastSaveTime);
		const bool bPassing = !CS->bSaving && !CS->bSaveError && !bReducedMotion;
		FBlock Note(*this, bPassing ? FMath::Min(EaseOut(SavedFor, .2f), FMath::Clamp((3.f - SavedFor) / .3f, 0.f, 1.f)) : 1.f, 0.f, 10 * S);
		Plate(R, Color, Ink);
		Print(Status, R.Max.X - 11 * S, R.Min.Y + 5 * S, Color, LabelSize, 2, true, 284 * S);
	}
}

void AFTHUD::DrawControls(AFTGameMode* G)
{
	if (G->ControlsPage == 0) { DrawControlGuide(G); return; }
	const UFTGameInstance* GI = G->GI();
	if (!GI) return;
	// a working screen: it comes up in a short fade, nothing travels, and the chosen binding only brightens
	OverlayDim(.86f);
	const float X = W * .5f - 450 * S, Y = (H - CanvasHeight * S) * .5f + 30 * S, WidthPx = 900 * S;
	const float Entry = MenuAge(EMenuView::Controls);
	const float Chosen = FocusFlash(EMenuView::Controls);
	FBlock Panel(*this, In01(Entry, 0.f, QuietTime));
	Plate(Rect(X, Y, WidthPx, 660 * S), Cyan, Ink);
	AccentSweep(Rect(X + 2 * S, Y + 2 * S, WidthPx - 4 * S, 4 * S), Entry, Cyan, MenuEntrance);
	Print(TEXT("CONTROLS"), X + 24 * S, Y + 18 * S, White, Heading, 0, true);   // (its heading used to slide in 9 px: nothing travels here now)
	Print(TEXT("Active layout switches when you steer with the other set of bindings."),
		X + 25 * S, Y + 62 * S, Grey, Body, 0, false, 690 * S);
	const FBox2D Back = Rect(X + 715 * S, Y + 18 * S, 161 * S, 46 * S);
	G->OverlayRects.Add(Control(Back, G->bControlsWaiting ? TEXT("CANCEL") : TEXT("BACK"), BackHint(this),
		ControlState(Back), Cyan));
	const float KeysX = X + 308 * S, KeyPitch = 270 * S, RowH = 36 * S;
	for (int32 Layout = 0; Layout < 2; ++Layout)
	{
		const bool bActive = GI->Layout == Layout;
		const FString Name = Layout == 0 ? TEXT("WASD LAYOUT") : TEXT("ARROWS LAYOUT");
		Print(bActive ? Name + TEXT("  /  ACTIVE") : Name, KeysX + (Layout * KeyPitch + 124 * S), Y + 98 * S,
			bActive ? Cyan : Grey, LabelSize, 1, true);
	}
	Print(TEXT("ACTION"), X + 25 * S, Y + 98 * S, Grey, LabelSize, 0, true);
	for (int32 Action = 0; Action < UFTGameInstance::NumActions; ++Action)
	{
		const float RowY = Y + 122 * S + Action * RowH;
		Box(X + 25 * S, RowY, 851 * S, 32 * S, G->ControlsRow == Action ? Active : Steel);
		Print(UFTGameInstance::ActionName(Action), X + 37 * S, RowY + 5 * S, White, Body);
		// an action with no key in the layout that is driving is marked: it cannot be used until it is bound again
		// (the audit of 15 Nov: "an action can be left unbound with no warning")
		if (!GI->Binds[Action][GI->Layout].IsValid())
			Icon(EIcon::Warning, X + 279 * S, RowY + 8 * S, 16 * S, Amber);
		for (int32 Layout = 0; Layout < 2; ++Layout)
		{
			const FBox2D Key = Rect(KeysX + Layout * KeyPitch, RowY, 248 * S, 32 * S);
			const bool bSelected = G->ControlsRow == Action && G->ControlsCol == Layout;
			const bool bCapture = bSelected && G->bControlsWaiting;
			const FKey& Bind = GI->Binds[Action][Layout];
			FString Text = Bind.IsValid() ? Bind.GetDisplayName().ToString().ToUpper() : FString(TEXT("UNBOUND"));
			if (bCapture) Text = TEXT("PRESS A KEY");
			G->OverlayRects.Add(Control(Key, Text, FString(),
				ControlState(Key, bSelected, !G->bControlsWaiting || bCapture), bCapture ? Yellow : Cyan,
				Bind.IsValid() || bCapture ? nullptr : &Amber));
			if (bSelected)
			{
				FocusAccent(Key, MenuAge(EMenuView::Controls, true), bCapture ? Yellow : Cyan);
				if (Chosen > 0.f) Box(Key.Min.X, Key.Min.Y, Key.GetSize().X, Key.GetSize().Y, Alpha(White, .16f * Chosen));
				// waiting for a key: the cell breathes so it is plain which one is listening
				if (bCapture && !bReducedMotion)
					Frame(Key.Min.X, Key.Min.Y, Key.GetSize().X, Key.GetSize().Y, Alpha(Yellow, .35f + .65f * FMath::Square(FMath::Sin(T * PI * 1.4f))), 2.f);
			}
		}
	}
	const FBox2D Reset = Rect(X + 25 * S, Y + 454 * S, 279 * S, 44 * S);
	G->OverlayRects.Add(Control(Reset, TEXT("RESET TO DEFAULTS"), FString(),
		ControlState(Reset, G->ControlsRow == UFTGameInstance::NumActions, !G->bControlsWaiting), Yellow));
	if (G->ControlsRow == UFTGameInstance::NumActions)
	{
		FocusAccent(Reset, MenuAge(EMenuView::Controls, true), Yellow);
		if (Chosen > 0.f) Box(Reset.Min.X, Reset.Min.Y, Reset.GetSize().X, Reset.GetSize().Y, Alpha(White, .16f * Chosen));
	}
	if (G->bControlsWaiting)
		Print(TEXT("ESC CANCELS   /   DELETE OR BACKSPACE CLEARS"), X + 326 * S, Y + 462 * S, Yellow, Body);
	else if (G->RealClockNow() - G->OverlayFeedbackT < 2.7f)
	{
		// what the last rebind did: it lands in its line, and a refusal lands white-hot
		const float Said = G->RealClockNow() - G->OverlayFeedbackT;
		const FLinearColor Tone = G->bOverlayFeedbackError ? Red : Cyan;
		Print(G->OverlayFeedback, X + 326 * S, Y + 461 * S, Mix(Tone, White, Flash01(Said, .3f)), Body, 0, false, 542 * S);
		AccentSweep(Rect(X + 326 * S, Y + 487 * S, 300 * S, 2 * S), Said, Tone, FocusSweep);
	}
	Print(TEXT("UP-DOWN   ACTION     LEFT-RIGHT   LAYOUT     ") + ConfirmHint(this) + TEXT("   REBIND"), X + 25 * S, Y + 522 * S, White, Body);
	Print(TEXT("MOUSE   LEFT: GUNS   /   RIGHT: MISSILE   /   MIDDLE: AMMO"), X + 25 * S, Y + 555 * S, Grey, Body);
	Print(TEXT("Primary, secondary and utility bindings also carry over to the fighting controls."),
		X + 25 * S, Y + 581 * S, Grey, Body, 0, false, 850 * S);
	Print(TEXT("Reserved keys and fixed mouse and gamepad actions cannot be rebound."), X + 25 * S, Y + 617 * S, Grey, LabelSize);
}

void AFTHUD::DrawControlGuide(AFTGameMode* G)
{
	const UFTInputSubsystem* Input = UFTInputSubsystem::Get(this);
	if (!Input) return;
	OverlayDim(.88f);   // (overhaul) the overlays' one eased dim, so the guide and the bindings page hand over without a cut
	const float X = W * .5f - 492 * S, Y = (H - CanvasHeight * S) * .5f + 24 * S, WidthPx = 984 * S;
	Plate(Rect(X, Y, WidthPx, 672 * S), Cyan, Ink);
	Print(TEXT("KNOW YOUR CONTROLS"), X + 24 * S, Y + 20 * S, White, Heading, 0, true);
	for (int32 I = 0; I < 10; ++I) Flat(X + (24 + I * 16) * S, Y + 57 * S, 10 * S, 4 * S, I % 2 == 0 ? White : Pink);   // the clapper's stripes under the heading
	const FBox2D Back = Rect(X + 800 * S, Y + 18 * S, 160 * S, 45 * S);
	G->OverlayRects.Add(Control(Back, TEXT("BACK"), BackHint(this), ControlState(Back), Cyan));
	Print(Input->UsesGamepad() ? Input->DeviceLabel() : TEXT("KEYBOARD + MOUSE"), X + 25 * S, Y + 68 * S, Cyan, Body);
	Print(TEXT("Same attack positions. A different way to move."), X + 25 * S, Y + 95 * S, Grey, Body);
	const float ColW = 448 * S, L = X + 24 * S, R = X + 512 * S;
	Print(TEXT("ON THE ROAD"), L + 12 * S, Y + 135 * S, Cyan, Strong, 0, true);
	Print(TEXT("ON FOOT"), R + 12 * S, Y + 135 * S, Pink, Strong, 0, true);
	const TCHAR* RaceNames[] = { TEXT("STEER"), TEXT("MOVE UP / BRAKE"), TEXT("DRIFT"), TEXT("TURBO"), TEXT("FIRE GUNS"), TEXT("MISSILE"), TEXT("CYCLE AMMO"), TEXT("PAUSE") };
	const FString RaceKeys[] = {
		Input->UsesGamepad() ? Input->RacePrompt(EFTAction::Left) : Input->RacePrompt(EFTAction::Left) + TEXT(" / ") + Input->RacePrompt(EFTAction::Right),
		Input->RacePrompt(EFTAction::Up) + TEXT(" / ") + Input->RacePrompt(EFTAction::Down),
		Input->RacePrompt(EFTAction::Drift), Input->RacePrompt(EFTAction::Turbo), Input->RacePrompt(EFTAction::Fire),
		Input->RacePrompt(EFTAction::Missile), Input->RacePrompt(EFTAction::Ammo), InputHint(this, TEXT("P / ESC"), EKeys::Gamepad_Special_Right)
	};
	const TCHAR* FightNames[] = { TEXT("MOVE"), TEXT("PUNCH / SWING"), TEXT("KICK"), TEXT("JUMP"), TEXT("GRAB / THROW FOE"), TEXT("PICK UP / DRINK"), TEXT("THROW WEAPON"), TEXT("PAUSE") };
	const EFTBrawlAction FightActions[] = { EFTBrawlAction::Move, EFTBrawlAction::Punch, EFTBrawlAction::Kick, EFTBrawlAction::Jump,
		EFTBrawlAction::GrabThrow, EFTBrawlAction::Punch, EFTBrawlAction::Kick, EFTBrawlAction::Pause };
	const EFTAction RaceActions[] = { EFTAction::Left, EFTAction::Down, EFTAction::Drift, EFTAction::Turbo, EFTAction::Fire, EFTAction::Missile, EFTAction::Ammo, EFTAction::Count };
	for (int32 I = 0; I < 8; ++I)
	{
		const float RowY = Y + (168 + I * 31) * S;
		for (int32 Col = 0; Col < 2; ++Col)
		{
			const float CX = Col == 0 ? L : R;
			Box(CX, RowY, ColW, 28 * S, I % 2 ? Steel : Deep);
			Print(Col == 0 ? RaceNames[I] : FightNames[I], CX + 12 * S, RowY + 4 * S, White, Body, 0, false, 215 * S);
			Print(Col == 0 ? RaceKeys[I] : Input->BrawlPrompt(FightActions[I]), CX + ColW - 12 * S, RowY + 4 * S,
				Col == 0 ? Cyan : Pink, Body, 2, false, 180 * S);
			FTInputGlyphs::DrawFace(Canvas, Input, Col == 0 ? UFTInputSubsystem::RacePadKey(RaceActions[I]) : UFTInputSubsystem::BrawlPadKey(FightActions[I]),
				FVector2D(CX + ColW - 204 * S, RowY + 14 * S), 17 * S, FRaj ? FRaj.Get() : FB.Get());
		}
	}
	Print(TEXT("Auto-accelerates. Collect supplies by driving through them."), L + 8 * S, Y + 424 * S, Grey, LabelSize, 0, false, ColW - 16 * S);
	Print(TEXT("Get close to a foe or an item before grabbing or picking up."), R + 8 * S, Y + 424 * S, Grey, LabelSize, 0, false, ColW - 16 * S);
	const FString Labels[] = {
		FString(TEXT("BUTTON STYLE: ")) + UFTInputSubsystem::PromptStyleName(Input->GetPromptStyle()),
		TEXT("REPLAY TUTORIAL / LEVEL 1"), TEXT("EDIT KEYBOARD BINDINGS"), TEXT("SKIP CURRENT TUTORIAL")
	};
	const FString Hints[] = { TEXT("< / >"), TEXT("RESTARTS LEVEL 1"), InputHint(this, TEXT("TAB"), EKeys::Gamepad_RightShoulder), InputHint(this, TEXT("F2"), EKeys::Gamepad_Special_Left) };
	const int32 Rows = G->IsTutorialActive() ? 4 : 3;
	for (int32 I = 0; I < Rows; ++I)
	{
		const float BY = Y + (464 + I * 44) * S;
		const FBox2D Button = Rect(L, BY, WidthPx - 48 * S, 42 * S);
		G->OverlayRects.Add(Control(Button, Labels[I], Hints[I], ControlState(Button, G->ControlsRow == I), I == 1 ? Yellow : Cyan));
	}
	Print(TEXT("Button style changes labels, keeping the same physical button positions."), L + 8 * S, Y + 445 * S, Grey, LabelSize, 0, false, WidthPx - 64 * S);
	Print(FString::Printf(TEXT("%s  SELECT     %s  BACK     UP / DOWN  NAVIGATE     LEFT / RIGHT  BUTTON STYLE"), *ConfirmHint(this), *BackHint(this)),
		L + 8 * S, Y + 644 * S, White, Body, 0, false, WidthPx - 64 * S);
}

void AFTHUD::DrawTutorial(AFTGameMode* G)
{
	if (!G->IsTutorialActive()) return;
	const UFTInputSubsystem* Input = UFTInputSubsystem::Get(this);
	if (!Input) return;
	FString Title, Instruction, Cue;
	switch (G->Tutorial.Step)
	{
	case EFTTutorialStep::Steer:
		Title = TEXT("FIND YOUR LINE"); Instruction = TEXT("The car drives forward automatically. Steer left and right.");
		Cue = Input->UsesGamepad() ? Input->RacePrompt(EFTAction::Left) : Input->RacePrompt(EFTAction::Left) + TEXT(" / ") + Input->RacePrompt(EFTAction::Right); break;
	case EFTTutorialStep::Drift:
		Title = TEXT("SLIDE INTO THE TURN"); Instruction = TEXT("Hold drift while steering. Charge the meter, then release."); Cue = Input->RacePrompt(EFTAction::Drift); break;
	case EFTTutorialStep::Turbo:
		Title = TEXT("GIVE IT SOME HEAT"); Instruction = TEXT("Hold turbo for a burst of speed. Release to recharge."); Cue = Input->RacePrompt(EFTAction::Turbo); break;
	case EFTTutorialStep::Fire:
		Title = TEXT("OPEN FIRE"); Instruction = TEXT("Hold your primary attack to fire the guns."); Cue = Input->RacePrompt(EFTAction::Fire); break;
	case EFTTutorialStep::Pickup:
		Title = TEXT("PICK UP SUPPLIES"); Instruction = TEXT("Drive through the missile pickup ahead. Collection is automatic."); Cue = TEXT("DRIVE THROUGH"); break;
	case EFTTutorialStep::Missile:
		Title = TEXT("MAKE AN ENTRANCE"); Instruction = TEXT("Press your secondary attack to launch a missile."); Cue = Input->RacePrompt(EFTAction::Missile); break;
	default: return;
	}
	const float X = Left + Safe * S, Y = 25 * S, WidthPx = 312 * S;
	const bool bDone = G->Tutorial.bActionObserved;
	const FLinearColor Accent = bDone ? Green : Cyan;
	Plate(Rect(X, Y, WidthPx, 202 * S), Accent, Ink);
	Print(FString::Printf(TEXT("FIRST TAKE  /  %d OF %d"), G->Tutorial.StepIndex() + 1, FFTTutorialState::NumSteps), X + 14 * S, Y + 10 * S, Accent, LabelSize, 0, true);
	Meter(Rect(X + 14 * S, Y + 34 * S, WidthPx - 28 * S, 5 * S), G->Tutorial.Progress(), Accent, FFTTutorialState::NumSteps);
	Print(bDone ? TEXT("NAILED IT!") : Title, X + 14 * S, Y + 49 * S, White, Strong, 0, true, WidthPx - 28 * S);
	float LineY = Y + 81 * S;
	for (const FString& Line : WrapText(Instruction, WidthPx - 28 * S, Body))
	{
		Print(Line, X + 14 * S, LineY, White, Body, 0, false, WidthPx - 28 * S);
		LineY += 20 * S;
	}
	Box(X + 14 * S, Y + 133 * S, WidthPx - 28 * S, 28 * S, Active);
	Print(Cue.ToUpper(), X + WidthPx * .5f, Y + 137 * S, Accent, Strong, 1, true, WidthPx - 44 * S);
	Print(InputHint(this, TEXT("F2"), EKeys::Gamepad_Special_Left) + TEXT("  SKIP    /    TAKE YOUR TIME"), X + 14 * S, Y + 174 * S, Grey, LabelSize, 0, false, WidthPx - 28 * S);
}

void AFTHUD::DrawOptions(AFTGameMode* G)
{
	const UFTGameInstance* GI = G->GI();
	if (!GI) return;
	// restrained (the owner's word): a short fade, nothing travels; the chosen row brightens as the choice lands and
	// a value that was just changed flashes once
	OverlayDim(.82f);
	const float X = W * .5f - 365 * S, Y = (H - CanvasHeight * S) * .5f + 83 * S, WidthPx = 730 * S;
	const float Chosen = FocusFlash(EMenuView::Options);
	FBlock Panel(*this, In01(MenuAge(EMenuView::Options), 0.f, QuietTime));
	Plate(Rect(X, Y, WidthPx, 607 * S), Cyan, Ink, true);   // (the skin) read at length: paper
	Print(TEXT("OPTIONS"), X + 25 * S, Y + 21 * S, White, Heading, 0, true);
	for (int32 I = 0; I < 10; ++I) Flat(X + (25 + I * 16) * S, Y + 60 * S, 10 * S, 4 * S, I % 2 == 0 ? Ink : Pink);   // the clapper's stripes under the heading
	const FBox2D Back = Rect(X + 545 * S, Y + 20 * S, 161 * S, 48 * S);
	G->OverlayRects.Add(Control(Back, TEXT("BACK"), BackHint(this), ControlState(Back), Cyan));

	static const TCHAR* Labels[] = { TEXT("GRAPHICS PRESET"), TEXT("SHOW FRAME RATE"), TEXT("VSYNC"), TEXT("REDUCED MOTION"), TEXT("AUDIO") };
	static const TCHAR* Groups[] = { TEXT("DISPLAY"), TEXT("AUDIO & ACCESSIBILITY") };
	const FString Values[] = { UFTGameInstance::QualityName(GI->Quality), GI->bShowFps ? TEXT("ON") : TEXT("OFF"),
		GI->bVSync ? TEXT("ON") : TEXT("OFF"), GI->bReducedMotion ? TEXT("ON") : TEXT("OFF"), GI->bMuted ? TEXT("MUTED") : TEXT("ON") };
	for (int32 I = 0; I < UE_ARRAY_COUNT(Labels); ++I)
	{
		const float RowY = Y + (112 + I * 53 + (I >= 3 ? 38 : 0)) * S;
		const bool bSelected = G->OptionsRow == I;
		if (!OptionValues[I].IsEmpty() && OptionValues[I] != Values[I]) OptionChangedAt[I] = T;
		OptionValues[I] = Values[I];
		const float Changed = Flash01(T - OptionChangedAt[I], .22f);
		if (I == 0 || I == 3)
			Print(Groups[I == 0 ? 0 : 1], X + 25 * S, RowY - 24 * S, Grey, LabelSize, 0, true);
		const FBox2D Row = Rect(X + 24 * S, RowY, WidthPx - 48 * S, 45 * S);
		Box(Row.Min.X, Row.Min.Y, Row.GetSize().X, Row.GetSize().Y, bSelected ? Active : Steel);
		Frame(Row.Min.X, Row.Min.Y, Row.GetSize().X, Row.GetSize().Y, bSelected ? Cyan : Rule, bSelected ? 1.5f : 1.f);
		if (bSelected && Chosen > 0.f) Box(Row.Min.X, Row.Min.Y, Row.GetSize().X, Row.GetSize().Y, Alpha(White, .13f * Chosen));
		Print(Labels[I], Row.Min.X + 14 * S, RowY + 10 * S, White, Strong, 0, true);
		Print(Values[I], Row.Max.X - 117 * S, RowY + 9 * S, Mix(bSelected ? Cyan : White, White, Changed), Strong, 1, true, 122 * S);
		if (Changed > 0.f) Box(Row.Max.X - 178 * S, RowY + 38 * S, 122 * S, 2 * S, Alpha(Cyan, Changed));
		const FBox2D Dec = Rect(Row.Max.X - 215 * S, RowY + 5 * S, 31 * S, 35 * S);
		const FBox2D Inc = Rect(Row.Max.X - 50 * S, RowY + 5 * S, 31 * S, 35 * S);
		G->OverlayRects.Add(Control(Dec, TEXT("<"), FString(), ControlState(Dec), Cyan));
		G->OverlayRects.Add(Control(Inc, TEXT(">"), FString(), ControlState(Inc), Cyan));
	}
	Box(X + 24 * S, Y + 434 * S, WidthPx - 48 * S, S, Rule);
	static const TCHAR* Details[] = {
		// (2.7 left Lumen to EPIC only and 2.8 gave LOW and MEDIUM TAA: this line still described 2.6)
		TEXT("Low and Medium use TAA with Lumen off. High uses TSR. Epic adds Lumen lighting and reflections. Auto chooses for this computer."),
		TEXT("Display the current frame rate and frame time in the corner."),
		TEXT("Synchronize frame presentation to the display refresh rate."),
		TEXT("Reduce decorative movement and flashes in the interface and game."),
		TEXT("Turn game audio on or mute it. This setting carries between driving and fighting.")
	};
	float DetailY = Y + 451 * S;
	{
		// the line that explains the chosen row follows the choice (a short fade; it is never clear)
		FBlock Told(*this, .5f + .5f * In01(MenuAge(EMenuView::Options, true), 0.f, .12f));
		for (const FString& Line : WrapText(Details[FMath::Clamp(G->OptionsRow, 0, 4)], WidthPx - 48 * S, Body))
		{
			Print(Line, X + 24 * S, DetailY, Grey, Body, 0, false, WidthPx - 48 * S);
			DetailY += 22 * S;
		}
	}
	Print(TEXT("CHANGES SAVE AUTOMATICALLY"), X + 24 * S, Y + 526 * S, Grey, LabelSize, 0, true);
	Print(TEXT("UP-DOWN / D-PAD   PICK     LEFT-RIGHT   CHANGE"), X + 24 * S, Y + 561 * S, White, Body);
}
