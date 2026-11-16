#include "FTHUD.h"
#include "FTHUDStyle.h"
#include "FTHUDSkin.h"
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
#if WITH_EDITOR
#include "TextureCompiler.h"   // the title logo must not stand as a placeholder while its texture is still being built
#endif
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Math/RotationMatrix.h"
#include "Math/ScaleMatrix.h"
#include "Math/TranslationMatrix.h"
#include "Sound/SoundBase.h"

// THE STUDIO LOT, INKED (the style ruling of 15 Nov 2026). The palette lives here and not in FTHUDStyle.h: that
// header is not this pass's to change, and the brawl's HUD keeps its own copy of its values (it does not include
// it: Brawl/FTBrawlHUD.cpp, "the races' palette, value for value"). The screens still ask for the names they always
// asked for (Ink, Steel, Active, Rule, White, Grey, the five signals); what each name IS has changed, and Box, Frame,
// Plate, Control, Print and Meter decide what it becomes on a slate panel, on paper, on the wall and in the race.
namespace FTLot
{
	// (the layout and easing tokens of FTHUDStyle.h, exactly as they are)
	using FTHUDStyle::CanvasHeight; using FTHUDStyle::Safe; using FTHUDStyle::Inset; using FTHUDStyle::Gap;
	using FTHUDStyle::LabelSize; using FTHUDStyle::Body; using FTHUDStyle::Strong; using FTHUDStyle::Heading;
	using FTHUDStyle::Readout; using FTHUDStyle::Enter; using FTHUDStyle::Feedback; using FTHUDStyle::MenuEntrance; using FTHUDStyle::FocusSweep;
	using FTHUDStyle::ClapperClose; using FTHUDStyle::ClapperTravel; using FTHUDStyle::Alpha; using FTHUDStyle::EaseOut;

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

/** The focus cursor: a pink band round the item, OUTSIDE its own rectangle, so that it never lies on the item's words
 *  (the owner, 15 Nov: "the pink border on the selected button is cutting off the words"). Rows stand 3 px apart: the
 *  band fits the gap between them. */
void AFTHUD::Cursor(const FBox2D& R)
{
	const float X = R.Min.X, Y = R.Min.Y, Wd = R.GetSize().X, Ht = R.GetSize().Y;
	const float Th = 3.f * S;
	Flat(X - Th, Y - Th, Wd + 2 * Th, Th, Pink); Flat(X - Th, Y + Ht, Wd + 2 * Th, Th, Pink);
	Flat(X - Th, Y, Th, Ht, Pink); Flat(X + Wd, Y, Th, Ht, Pink);
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
	const FLinearColor Stock = !Same(Fill, Ink) ? Fill : bSticker ? Ink : bPaper ? Paper : SlateFill;
	if (bSticker)
	{
		Flat(X + Drop, Y + Drop, Wd, Ht, Alpha(Ink, .55f));
		Flat(X, Y, Wd, Ht, Ink);
		Flat(X + Out, Y + Out, Wd - 2 * Out, Ht - 2 * Out, Stock);
	}
	// In the menus the stock is the kit's: paper with its grain, or clapper-slate black with chalk dust and a chalk
	// hairline, each in its ink outline on its hard shadow (FTSkin draws flat boxes while its textures are missing).
	else if (Lum(Stock) > .35f) FTSkin::PaperPanel(Canvas, S, X, Y, Wd, Ht, Stock, Fade);
	else FTSkin::SlatePanel(Canvas, S, X, Y, Wd, Ht, Fade, 7.f, Stock);
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

FVector2D AFTHUD::TextSize(FStringView Str, float Height, bool bDisplay)
{
	Height = FMath::Max(Height, bDisplay ? 15.9f : 15.4f);
	return MeasureF(bDisplay ? DisplayFace() : BodyFace(), Str, Height / (bDisplay ? BunHeight : RajHeight));
}

void AFTHUD::Print(const FString& Str, float X, float Y, const FLinearColor& Color, float Height, int32 Align, bool bDisplay, float MaxWidth)
{
	PrintText(Str, X, Y, Color, Height, Align, bDisplay, MaxWidth);
}

void AFTHUD::PrintText(FStringView Str, float X, float Y, const FLinearColor& Color, float Height, int32 Align, bool bDisplay, float MaxWidth, float Premeasured)
{
	if (Str.IsEmpty()) return;
	// Bungee for titles, buttons, plaques and the big numerals (what the screens call "display"), Rajdhani Bold for
	// body and labels. The ruling's floor: no text that carries information under 18 px at 1080.
	UFont* Font = bDisplay ? DisplayFace() : BodyFace();
	Height = FMath::Max(Height, bDisplay ? 15.9f : 15.4f);
	float Scale = Height / (bDisplay ? BunHeight : RajHeight);
	// (Premeasured is this very product, taken by the caller through TextSize: the same face, string and scale)
	float Measured = Align != 0 || MaxWidth > 0.f ? (Premeasured >= 0.f ? Premeasured : float(MeasureF(Font, Str, Scale).X)) : 0.f;
	if (MaxWidth > 0.f && Measured > MaxWidth)
	{
		Scale *= MaxWidth / Measured;
		Measured = MaxWidth;
	}
	if (Align != 0) X -= Align == 1 ? Measured * .5f : Measured;
	// a colour chosen for a dark panel is ink on paper and on the wall
	const bool bOnLight = ToneAt(X + (Measured > 0.f ? Measured * .5f : 6.f * S), Y + Height * S * .5f) == ETone::Light;
	const FLinearColor Use = bOnLight ? InkFor(Color) : Color;
	// (15 Nov, the owner: "some text is blurry".) A runtime font's glyphs are drawn at the font asset's own size and a
	// scale on the item only stretches that bitmap: big titles and numerals came out soft. Text larger than the asset's
	// size is therefore drawn at its real size (a ladder of 4-point steps, so that an animated size does not fill the
	// glyph atlas), with only the small remainder, always a reduction, left to the item's scale.
	// (15 Nov, the frame-cost pass.) The items are FCanvasTextStringViewItem: the engine's FCanvasTextItem only hands
	// Text.ToString() to the very string drawer this one calls with Str, so an FText made from every string every
	// frame bought nothing. Str outlives both DrawItem calls. The font, SlateFontInfo, scale, colour and blend mode
	// are FCanvasSimpleTextItem's, set as they were.
	auto Sharp = [&](FCanvasSimpleTextItem& It)
	{
		const float Total = Scale * S;
		if (Font && Font->FontCacheType == EFontCacheType::Runtime && Total > 1.02f)
		{
			FSlateFontInfo Info = Font->GetLegacySlateFontInfo();
			const float Want = Info.Size * Total, Drawn = FMath::CeilToFloat(Want / 4.f) * 4.f;
			Info.Size = Drawn;
			It.SlateFontInfo = Info;
			It.Scale = FVector2D(Want / Drawn);
		}
		else It.Scale = FVector2D(Total);
	};
	if (bDisplay && !bSticker && Height >= Heading - .5f && Use.A > .5f)
	{
		// a title is printed twice: the pink plate a little off register, under the ink or the chalk
		FCanvasTextStringViewItem Under(FVector2D(X + 2.f * S, Y + 2.f * S), Str, Font, Alpha(Pink, Use.A * Fade));
		Sharp(Under);
		Canvas->DrawItem(Under);
	}
	FCanvasTextStringViewItem Item(FVector2D(X, Y), Str, Font, Fade < 1.f ? Alpha(Use, Fade) : Use);
	Sharp(Item);
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
	{
		bPointerOverControl = true;
		return PC->IsInputKeyDown(EKeys::LeftMouseButton) ? EControl::Pressed : EControl::Hover;
	}
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
	const FLinearColor Stock = !bEnabled ? Mix(Steel, SlateFill, .65f) : bHot ? Active : Steel;
	// the body is the kit's stock, its own shadow off: the button's shadow above stays put while the body sinks
	if (bEnabled && bHot) FTSkin::PaperPanel(Canvas, S, X, Y, Size.X, Size.Y, Stock, Fade, 0.f);
	else FTSkin::SlatePanel(Canvas, S, X, Y, Size.X, Size.Y, Fade, 0.f, Stock, false);
	if (!bEnabled && FTSkin::Texture(FTSkin::ETex::Hatch)) FTSkin::Hatch(Canvas, S, X + Out, Y + Out, Size.X - 2 * Out, Size.Y - 2 * Out, Fade * .5f);
	MarkTone(Rect(X, Y, Size.X, Size.Y), bEnabled && bHot ? ETone::Light : ETone::Dark);
	float StripeW = 0.f;
	if (Size.X >= 96.f * S)
	{
		// the clapper's stripes down the left end: chalk and ink, amber and ink when it is the chosen one
		StripeW = FMath::Min(11.f * S, float(Size.X) * .08f);
		const int32 Steps = 4;
		const float StepH = (Size.Y - 2 * Out) / Steps;
		const FLinearColor Light = !bEnabled ? Grey : bHot ? Yellow : White;
		if (FTSkin::Texture(FTSkin::ETex::HazardStripes)) FTSkin::HazardStripes(Canvas, S, X + Out, Y + Out, StripeW, Size.Y - 2 * Out, Light, Fade);
		else for (int32 I = 0; I < Steps; ++I) Flat(X + Out, Y + Out + I * StepH, StripeW, StepH, I % 2 == 0 ? Light : Ink);
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

/** A band of the clapper's stripes: the kit's narrow stripes tinted Light over their ink; blocks of Light and Dark while
 *  the kit is missing. */
void AFTHUD::Stripes(float X, float Y, float Wd, float Ht, const FLinearColor& Light, const FLinearColor& Dark)
{
	if (FTSkin::Texture(FTSkin::ETex::HazardStripes))
	{
		FTSkin::HazardStripes(Canvas, S, X, Y, Wd, Ht, Light, Fade, .75f);
		return;
	}
	int32 Index = 0;
	for (float Bx = X; Bx + 10.f * S <= X + Wd; Bx += 16.f * S) Flat(Bx, Y, 10.f * S, Ht, Index++ % 2 == 0 ? Dark : Light);
}

/** A row of key hints ("KEYS   ACTION     KEYS   ACTION"): the same string, in the same place, with a key cap of the
 *  kit under each key, sized to it (light caps on a dark panel, dark caps on paper and on the wall). Anything that is
 *  not such a row (a message, a string that had to be squeezed into its width) is printed as it always was. */
void AFTHUD::HintRow(const FString& Str, float X, float Y, const FLinearColor& Color, float Height, int32 Align, bool bDisplay, float MaxWidth)
{
	struct FSpan { int32 At, Len, ActionAt; };
	TArray<FSpan, TInlineAllocator<8>> Spans;
	const float Whole = TextSize(Str, Height, bDisplay).X;
	bool bRow = Str.Contains(TEXT("  ")) && !(MaxWidth > 0.f && Whole > MaxWidth);
	for (int32 Start = 0; bRow && Start < Str.Len(); )
	{
		int32 End = Str.Find(TEXT("     "), ESearchCase::CaseSensitive, ESearchDir::FromStart, Start);
		if (End == INDEX_NONE) End = Str.Len();
		const FString One = Str.Mid(Start, End - Start);
		const int32 Split = One.Find(TEXT("  "));
		int32 Action = Split;
		while (Action != INDEX_NONE && Action < One.Len() && One[Action] == TEXT(' ')) ++Action;
		if (Split <= 0 || Split > 26 || Action >= One.Len() || One.Mid(Action).TrimEnd().Contains(TEXT("  "))) bRow = false;
		else Spans.Add(FSpan{ Start, Split, Start + Action });
		Start = End + 5;
	}
	if (!bRow || Spans.IsEmpty())
	{
		Print(Str, X, Y, Color, Height, Align, bDisplay, MaxWidth);
		return;
	}
	const float RowX = X - (Align == 1 ? Whole * .5f : Align == 2 ? Whole : 0.f);
	const float CapH = FMath::Clamp((FMath::Max(Height, 15.4f) + 4.f) * S, 28.f * FTSkin::KitPx(S), 44.f * FTSkin::KitPx(S));
	for (int32 I = 0; I < Spans.Num(); ++I)
	{
		const FSpan& Span = Spans[I];
		const FString Name = Str.Mid(Span.At, Span.Len);
		// (a width is always measured up to a letter, never up to a space: where each piece stands is where it stood)
		const float NameW = TextSize(Name, Height, bDisplay).X;
		const float NameX = RowX + TextSize(Str.Left(Span.At + Span.Len), Height, bDisplay).X - NameW;
		const float CapW = FTSkin::KeyCapWidth(NameW, CapH), CapX = NameX - (CapW - NameW) * .5f, CapY = Y - 1.f * S;
		const bool bOnDark = ToneAt(NameX + NameW * .5f, Y + Height * S * .5f) == ETone::Dark;
		FTSkin::KeyCap(Canvas, S, CapX, CapY, NameW, CapH, bOnDark, Fade);
		MarkTone(Rect(CapX, CapY, CapW, CapH), bOnDark ? ETone::Light : ETone::Dark);
		Print(Name, NameX, Y, Color, Height, 0, bDisplay);
		const int32 ActionEnd = I + 1 < Spans.Num() ? Spans[I + 1].At : Str.Len();
		const float ActionX = RowX + TextSize(Str.Left(Span.ActionAt + 1), Height, bDisplay).X - TextSize(Str.Mid(Span.ActionAt, 1), Height, bDisplay).X;
		Print(Str.Mid(Span.ActionAt, ActionEnd - Span.ActionAt).TrimEnd(), ActionX, Y, Color, Height, 0, bDisplay);
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
		FTSkin::Wall(Canvas, S, WallNow);   // the kit's lit cyclorama (its own gradient while the textures are missing)
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
		FTSkin::Wall(Canvas, S, Up);
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
	bPointerOverControl = false;
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
#if WITH_EDITOR
		// (an uncooked game builds a texture in the background and shows a 32 x 32 placeholder until it is done: seen on
		// 15 Nov, the logo stood blank for the whole title. One texture, once: wait for it here.)
		if (TLogo)
		{
			const bool bWas = FTextureCompilingManager::Get().IsCompilingTexture(TLogo);
			UTexture* LogoWait[] = { TLogo.Get() };
			FTextureCompilingManager::Get().FinishCompilation(LogoWait);
			UE_LOG(LogFT, Display, TEXT("FT TITLE logo compiling=%d size after=%dx%d"), bWas ? 1 : 0, TLogo->GetSizeX(), TLogo->GetSizeY());
		}
#endif
		RajHeight = FMath::Max(1.f, float(MeasureF(FRaj ? FRaj.Get() : FB.Get(), TEXT("Hg"), 1.f).Y / S));
		BunHeight = FMath::Max(1.f, float(MeasureF(DisplayFace(), TEXT("Hg"), 1.f).Y / S));
		UE_LOG(LogFT, Display, TEXT("FT SKIN kit textures=%d"), FTSkin::Preload());   // all of the kit now, so nothing loads mid-race
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
	DrawPointer(G);
}

/** The mouse pointer, last of all (FTCursor): only on a screen that takes the mouse, and only while the mouse is what
 *  the player is using. Amber over anything that can be clicked, sunk and pink while the button is down. */
void AFTHUD::DrawPointer(AFTGameMode* G)
{
	const AFTPlayerController* PC = Cast<AFTPlayerController>(GetOwningPlayerController());
	if (!PC || !PC->PointerShown()) return;
	const FVector2D At = PC->MousePos();
	if (At.X < 0.f) return;
	bool bOver = bPointerOverControl;
	const auto Any = [&](const TArray<FBox2D>& Rects) { for (const FBox2D& R : Rects) if (R.bIsValid && R.IsInside(At)) bOver = true; };
	const auto One = [&](const FBox2D& R) { if (R.bIsValid && R.IsInside(At)) bOver = true; };
	if (G->Overlay != EFTOverlay::None) Any(G->OverlayRects);
	else if (G->bAttract || G->Phase == EFTPhase::Title) { Any(G->TitleRowRects); Any(G->TitleBtnRects); }
	else if (G->Phase == EFTPhase::Reel) One(G->ReelContinueRect);
	else if (G->Phase == EFTPhase::Rating || G->Phase == EFTPhase::Paycheck) One(G->ResultsContinueRect);
	else if (G->Phase == EFTPhase::PreProduction) { Any(G->TileRects); Any(G->TabRects); One(G->ReadyRect); One(G->BuyRect); One(G->UndoRect); }
	const FTCursor::EState State = PC->IsInputKeyDown(EKeys::LeftMouseButton) ? FTCursor::EState::Pressed : bOver ? FTCursor::EState::Hover : FTCursor::EState::Normal;
	if (State != PointerState) { PointerState = State; UE_LOG(LogFT, Display, TEXT("FT POINTER state=%d at=%.0f,%.0f"), int32(State), At.X, At.Y); }
	FTCursor::Draw(Canvas, At, S, State, T, bReducedMotion);
}

FVector2D AFTHUD::MeasureF(UFont* F, FStringView Str, float Scale)
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

	Backdrop(G->bAttract ? 0.f : .46f, 0.f);
	// The wall (the ruling's item 4): the lit cyclorama stands only behind the shoot list, the left two fifths, and
	// behind the action bar; the rest of the screen is the live view. (Text printed on it comes out in ink.)
	const float WallW = X + BoardW + 22 * S, BarY = ActionY - 16 * S;
	FTSkin::WallRect(Canvas, 0.f, 0.f, WallW, H, Fade);
	FTSkin::WallRect(Canvas, WallW, BarY, W - WallW, H - BarY, Fade);
	Flat(WallW, 0.f, 2.f * S, BarY, Ink);
	Flat(WallW, BarY, W - WallW, 2.f * S, Ink);
	MarkTone(Rect(0.f, 0.f, WallW, H), ETone::Light);
	MarkTone(Rect(WallW, BarY, W - WallW, H - BarY), ETone::Light);

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
		const float LogoW = 418 * S;
		if (TLogo && TLogo->GetResource())
		{
			// the logo fills the masthead, down to the clapper's stripes (the tagline that stood under it is gone)
			const float Asp = TLogo->GetSizeX() > 0 && TLogo->GetSizeY() > 0 ? float(TLogo->GetSizeY()) / float(TLogo->GetSizeX()) : .42f;
			const float LogoH = FMath::Min(LogoW * Asp, 162.f * S), LogoDrawW = LogoH / Asp;
			const float LogoX = X + (BoardW - LogoDrawW) * .5f, LogoY = Y + 36.f * S + (166.f * S - LogoH) * .5f;
			FCanvasTileItem Logo(FVector2D(LogoX, LogoY), TLogo->GetResource(), FVector2D(LogoDrawW, LogoH), FLinearColor(1.f, 1.f, 1.f, (.74f + .26f * In) * Fade));
			Logo.BlendMode = SE_BLEND_Translucent;
			Canvas->DrawItem(Logo);
			if (!bLogoSaid) { bLogoSaid = true; UE_LOG(LogFT, Display, TEXT("FT TITLE logo size=%dx%d drawn=%.0fx%.0f at %.0f,%.0f fade=%.2f"), TLogo->GetSizeX(), TLogo->GetSizeY(), LogoDrawW, LogoH, LogoX, LogoY, Fade); }
		}
		else Print(TEXT("FULL THROTTLE"), X + BoardW * .5f, Y + 89 * S, White, Heading, 1, true, BoardW - 30 * S);

		// the clapper's stripes across the board, between the masthead and the shoot list (TODO kit: the clapper arm)
		Stripes(X + 16 * S, Y + 205 * S, BoardW - 32 * S, 9 * S, White, Steel);
		Print(TEXT("SELECT YOUR SHOOT"), X + 16 * S, Y + 222 * S, White, Strong, 0, true);
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

	if (Career)
	{
		// (main) what CONTINUE would resume, and the state of the save: main's two lines, word for word, under the shoot
		// list and the driver's chips, where main puts them. (The taped storyboard sheet that framed the live view, with
		// its caption strip and its memo, was taken out on the owner's word of 15 Nov: the view stays clear.)
		FBlock Told2(*this, In01(Entry, .34f, .2f));
		const float TellY = Y + 560 * S;
		Print(bContinue ? Career->ContinueLabel() : TEXT("NEW CAMPAIGN READY"), X + 16 * S, TellY, bContinue ? Cyan : Grey, Body, 0, false, BoardW - 32 * S);
		FString SaveDetail = Career->C.Updated.IsEmpty() ? TEXT("Progress saves automatically at checkpoints.") : TEXT("LAST SAVE  ") + Career->C.Updated;
		if (Career->bRecoveredFromBackup) SaveDetail = TEXT("BACKUP RECOVERED  /  ") + Career->C.Updated;
		if (Career->bSaveError || Career->bLoadError) SaveDetail = TEXT("SAVE NEEDS ATTENTION  /  New Game can replace it.");
		Print(SaveDetail, X + 16 * S, TellY + 24 * S, Career->bSaveError || Career->bLoadError ? Amber : Grey, LabelSize, 0, false, BoardW - 32 * S);
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
			// (15 Nov: with no save the greyed CONTINUE stood in lights and read as the selected button; the lights wait for a save)
			if (I == 0 && bContinue) FTSkin::Marquee(Canvas, S, ButtonRect.Min.X, ButtonRect.Min.Y, Bw, 50 * S, T, bReducedMotion, Fade);   // the hero action stands in lights
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
		HintRow(G->TitleActionFocus == INDEX_NONE ? InputHint(this, TEXT("TAB"), EKeys::Gamepad_RightShoulder) + TEXT("   ACTIONS")
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
		PrintText(Label, Screen.X, TopLeft.Y + 3.5f * S, FLinearColor(White.R, White.G, White.B, Alpha), LabelHeight, 1, false, 0.f, float(MarkerTextSize.X));   // (the width measured above: not measured again)
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

// ============================================================================================================
// THE RACE'S STICKERS and the flow screens (DrawPlay, DrawCard, DrawCut, DrawReel), restyled to "the studio lot,
// inked". The targets are ui_style/kit/mock/*.png and their recipe is mock_hud.py, whose numbers are kit px
// (1080p px): K = S / 1.5 turns one into screen px. The kit's textures and their drawing are FTHUDSkin's; what
// follows is what those four functions share on top of it, and what stands in while a texture is missing, so that
// no figure the player needs ever goes unshown. Layout, wording, order and timing are the ones the HUD had.
namespace
{
	/** A HUD sticker in its layout rectangle (FTSkin::InkPlate: T_UI_Hud_Plate from 160 kit px high, T_UI_Hud_Bar
	 *  below; the hard shadow is inside the rectangle) and where its slanted body is: Lx and Rx are the body's left
	 *  and right edge at a screen row, so every row of an instrument can follow the lean. (mock_hud.py, Sticker.) */
	struct FRaceSticker
	{
		float X, Y, Wd, Ht, Sc, Bh, Lean;
		FRaceSticker(float InX, float InY, float InWd, float InHt, float Kit) : X(InX), Y(InY), Wd(InWd), Ht(InHt)
		{
			Sc = InHt / (InHt >= 160.f * Kit ? 256.f : 96.f);   // screen px per texel of the sticker
			Bh = InHt - 5.f * Sc;                              // the body without the shadow under it
			Lean = .12f * Bh;                                  // how far the top stands to the right of the foot
		}
		float Lx(float Row) const { return X + Lean * (1.f - FMath::Clamp((Row - Y) / Bh, 0.f, 1.f)) + 6.f * Sc; }
		float Rx(float Row) const { return X + Wd - 10.f * Sc - Lean * FMath::Clamp((Row - Y) / Bh, 0.f, 1.f); }
	};

	// cells of the kit's icon atlas, as RaceIcon takes them
	constexpr int32 IcoClapper = static_cast<int32>(FTSkin::EIco::Clapper), IcoRec = static_cast<int32>(FTSkin::EIco::Rec);
	constexpr int32 IcoShield = static_cast<int32>(FTSkin::EIco::Shield), IcoBolt = static_cast<int32>(FTSkin::EIco::Bolt);
	constexpr int32 IcoMissile = static_cast<int32>(FTSkin::EIco::Missile), IcoBullets = static_cast<int32>(FTSkin::EIco::Bullets);
	constexpr int32 IcoWrench = static_cast<int32>(FTSkin::EIco::Wrench), IcoWarning = static_cast<int32>(FTSkin::EIco::Warning);
	constexpr int32 IcoSkull = static_cast<int32>(FTSkin::EIco::Skull), IcoArrow = static_cast<int32>(FTSkin::EIco::Arrow);
}

float AFTHUD::RaceTextW(FStringView Str, float Height, bool bDisplay)
{
	return float(TextSize(Str, Height, bDisplay).X);
}

/** Live text set by the MIDDLE OF ITS CAPITALS: a row's label, its figures and its key cap share one line whatever
 *  their sizes. A string that MaxWidth makes smaller stays on that line. */
void AFTHUD::RacePrint(FStringView Str, float X, float MidY, const FLinearColor& Color, float Height, int32 Align, bool bDisplay, float MaxWidth, float Premeasured)
{
	if (Str.IsEmpty()) return;
	float LineH = FMath::Max(Height, bDisplay ? 15.9f : 15.4f);
	// The string's width at this height is measured once: by the caller (Premeasured), or here when the fit needs it,
	// and Print is handed that number instead of measuring the same string at the same scale again.
	float Measured = Premeasured;
	if (MaxWidth > 0.f)
	{
		if (Measured < 0.f) Measured = RaceTextW(Str, LineH, bDisplay);
		if (Measured > MaxWidth) LineH *= MaxWidth / Measured;   // (Print scales the whole line down to fit)
	}
	// the capitals start .227 of the line's height under its top and are .545 of it high (Bungee; Rajdhani .225, .504)
	const float MidOfCaps = bDisplay ? .227f + .545f * .5f : .225f + .504f * .5f;
	PrintText(Str, X, MidY - MidOfCaps * LineH * S, Color, Height, Align, bDisplay, MaxWidth, Measured);
}

/** Big numerals from the baked digit atlas (0-9 $ , . / % x + - : and space; never live text). X, CapTop: the top
 *  left of the capitals (their right end with Align 2, their middle with 1); CapH: their height in screen px. A
 *  figure wider than MaxWidth is drawn smaller on the same baseline. Returns the width drawn. Before the kit is
 *  imported the figure is still shown: in Bungee, upright. */
float AFTHUD::RaceDigits(const FString& Str, float X, float CapTop, float CapH, const FLinearColor& Fill, int32 Align, float MaxWidth)
{
	if (Str.IsEmpty() || CapH <= 0.f) return 0.f;
	const float Baseline = CapTop + CapH;
	if (FTSkin::Texture(FTSkin::ETex::DigitsFill))
	{
		const float Natural = FTSkin::MeasureDigits(Str, CapH);
		if (MaxWidth > 0.f && Natural > MaxWidth) CapH *= MaxWidth / Natural;
		return FTSkin::Digits(Canvas, Str, X, Baseline, CapH, Fill, Fade, Align);
	}
	const float LineH = CapH / (.545f * S);
	const float Natural = RaceTextW(Str, LineH, true);
	const float Fit = MaxWidth > 0.f && Natural > MaxWidth ? MaxWidth / Natural : 1.f;
	Print(Str, X, Baseline - (.227f + .545f) * LineH * Fit * S, Fill, LineH, Align, true, MaxWidth);
	return Natural * Fit;
}

/** A small slanted chip (a plate's tab, TAKE 1, a load, a caption, a shout, the keys strip): T_UI_Hud_Chip, white
 *  with an ink outline, tinted. Without the texture: the same shape from two flat parallelograms. */
void AFTHUD::RaceChip(float X, float Y, float Wd, float Ht, const FLinearColor& Tint)
{
	if (Wd <= 0.f || Ht <= 0.f) return;
	if (FTSkin::Texture(FTSkin::ETex::HudChip))
	{
		FTSkin::HudChip(Canvas, X, Y, Wd, Ht, Tint, Fade);
		return;
	}
	const float Slant = .25f * Ht, Edge = Ht * 2.5f / 40.f;
	FTSkin::Para(Canvas, X, Y, Wd, Ht, Slant, Ink, Tint.A * Fade);
	FTSkin::Para(Canvas, X + 1.25f * Edge, Y + Edge, Wd - 2.5f * Edge, Ht - 2.f * Edge, Slant * (Ht - 2.f * Edge) / Ht, Tint, Fade);
}

/** A plate's tab: a chip 30 kit px high with live text on it (KESTREL-88 / AGENT N. KANE, ORDNANCE). Returns its width. */
float AFTHUD::RaceTab(FStringView Label, float X, float Y, const FLinearColor& Tint, const FLinearColor& TextColor)
{
	const float Ht = 30.f * S / 1.5f;
	const float Wd = RaceTextW(Label, 16.f, true) + Ht * .9f;
	RaceChip(X, Y, Wd, Ht, Tint);
	RacePrint(Label, X + Ht * .5f, Y + Ht * .5f, TextColor, 16.f, 0, true);
	return Wd;
}

/** An icon of the kit's atlas (Cell: FTSkin::EIco), Size screen px square. Without the atlas: the line icon the HUD
 *  always had (Fallback: an EIcon), or nothing. */
void AFTHUD::RaceIcon(int32 Cell, float X, float Y, float Size, const FLinearColor& Tint, int32 Fallback)
{
	if (FTSkin::Texture(FTSkin::ETex::Icons)) FTSkin::IconTex(Canvas, static_cast<FTSkin::EIco>(Cell), X, Y, Size, Tint, Fade);
	else if (Fallback >= 0) Icon(static_cast<EIcon>(Fallback), X + Size * .12f, Y + Size * .12f, Size * .76f, Tint);
}

/** A key cap on a dark sticker with its key's name (Rajdhani at the floor size), the cap's face centred on the row's
 *  middle line. A name too long for its room (a rebound key can be RIGHT MOUSE BUTTON) is set smaller, as Print fits
 *  any string. Returns the cap's width. */
float AFTHUD::RaceKey(FStringView Label, float X, float MidY, float Room, float HeightKit)
{
	const float Kit = S / 1.5f, Ht = HeightKit * Kit, Side = Ht * .62f;
	const float Natural = RaceTextW(Label, 16.f);   // (measured once; RacePrint and Print below take this number)
	float TextW = Natural;
	float MaxText = 0.f;
	if (Room > 0.f && FMath::Max(Ht, TextW + Side) > Room)
	{
		MaxText = FMath::Max(Room - Side, 10.f * S);
		TextW = FMath::Min(TextW, MaxText);
	}
	const float Wd = FTSkin::KeyCap(Canvas, S, X, MidY - (Ht - 4.f * Kit) * .5f, TextW, Ht, true, Fade);
	RacePrint(Label, X + Wd * .5f, MidY, Ink, 16.f, 1, false, MaxText, Natural);
	return Wd;
}

/** A row of T_UI_Hud_Pips (FullCell and the empty cell after it: 0 the missiles' arrows, 2 the film's reels), left
 *  aligned at X. The pitch is the kit's while the row fits its room; a long row is drawn smaller, cells and pitch
 *  together. FlashAt: a pip to light white (the rack a missile has just left). Returns the row's width. */
float AFTHUD::RacePips(int32 FullCell, float X, float Y, int32 Have, int32 Total, float Room, float PitchKit, const FLinearColor& Tint, int32 FlashAt, float Flash)
{
	if (Total <= 0) return 0.f;
	const float Kit = S / 1.5f;
	const float Fit = FMath::Clamp(Room / (Total * PitchKit * Kit), .3f, 1.f);
	const float Pitch = PitchKit * Kit * Fit, Size = 32.f * Kit * Fit, RowY = Y + 16.f * Kit * (1.f - Fit);
	const bool bAtlas = FTSkin::Texture(FTSkin::ETex::HudPips) != nullptr;
	for (int32 I = 0; I < Total; ++I)
	{
		const bool bFull = I < Have;
		const float Px = X + I * Pitch;
		if (bAtlas)
		{
			FTSkin::Pip(Canvas, static_cast<FTSkin::EPip>(FullCell + (bFull ? 0 : 1)), Px, RowY, Size, Size, bFull ? Tint : Steel, Fade);
			if (I == FlashAt && Flash > 0.f) FTSkin::Pip(Canvas, static_cast<FTSkin::EPip>(FullCell), Px, RowY, Size, Size, FLinearColor::White, Flash * Fade);
		}
		else
		{
			// no atlas yet: a slanted block a pip, so the count can still be read off the row
			FTSkin::Para(Canvas, Px + Size * .1f, RowY + Size * .22f, Size * .72f, Size * .56f, Size * .14f, bFull ? Tint : Steel, Fade);
		}
	}
	return Total * Pitch;
}

/** One tile of a texture through a UV rectangle (a mirrored tile has its UVs the other way round). */
void AFTHUD::RaceTile(UTexture2D* Tex, float X, float Y, float Wd, float Ht, float U0, float V0, float U1, float V1, const FLinearColor& Tint)
{
	if (!Tex || !Tex->GetResource() || Wd <= 0.f || Ht <= 0.f) return;
	FCanvasTileItem Tile(FVector2D(X, Y), Tex->GetResource(), FVector2D(Wd, Ht), FVector2D(U0, V0), FVector2D(U1, V1), Alpha(Tint, Fade));
	Tile.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(Tile);
}

/** The clapper's stick, 956 kit px long, from T_UI_ClapStick (704 x 48, a stripe every 84 px): two tiles of the one
 *  texture side by side, its first 500 px and its last 456 (which start three stripes further along, so the stripes
 *  run on). X, Y: the texture's top left; the stick's body is 40 px high inside it and its hinge 24, 24 px in.
 *  bFlipY: the fixed jaw on a slate's top edge. Without the texture: an ink bar and its stripes. */
void AFTHUD::RaceStick(float X, float Y, const FLinearColor& Tint, bool bFlipY)
{
	const float Kit = S / 1.5f;
	if (UTexture2D* Stick = FTSkin::Texture(FTSkin::ETex::ClapStick))
	{
		const float V0 = bFlipY ? 1.f : 0.f, V1 = bFlipY ? 0.f : 1.f;
		const float Seam = FMath::RoundToFloat(X + 500.f * Kit);
		RaceTile(Stick, X, Y, Seam - X, 48.f * Kit, 0.f, V0, 500.f / 704.f, V1, Tint);
		RaceTile(Stick, Seam, Y, X + 956.f * Kit - Seam, 48.f * Kit, 248.f / 704.f, V0, 1.f, V1, Tint);
		return;
	}
	Flat(X + 4.f * Kit, Y + 4.f * Kit, 948.f * Kit, 40.f * Kit, Ink);
	for (int32 I = 0; I < 11; ++I)
		FTSkin::Para(Canvas, X + (14.f + I * 84.f) * Kit, Y + 8.f * Kit, 58.f * Kit, 32.f * Kit, (bFlipY ? -1.f : 1.f) * 18.f * Kit, Tint, Fade);
}

/** The letterbox as film: a row of perforations along the picture's side of each matte. (The mattes themselves are
 *  Backdrop's, eased from screen to screen; like them, this does not fade with the instruments.) */
void AFTHUD::RaceMattes()
{
	const float Row = 16.f * S / 1.5f;
	if (MatteNow <= Row) return;
	FTSkin::TexTiled(Canvas, S, FTSkin::ETex::FilmPerf, 0.f, MatteNow - Row, W, Row, FTSkin::White, 1.f, 1.f, true, false);
	FTSkin::TexTiled(Canvas, S, FTSkin::ETex::FilmPerf, 0.f, H - MatteNow, W, Row, FTSkin::White, 1.f, 1.f, true, false);
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
	const float K = S / 1.5f;   // screen px per kit px: the numbers below that are times K are mock_hud.py's
	const float Hp = G->HpPct(), TurboFrac = G->Turbo / FMath::Max(1.f, G->Stats.TurboTank);
	const float HpFrac = FMath::Clamp(Hp / 100.f, 0.f, 1.f);
	const int32 Load = FMath::Clamp(G->AmmoLoad, 0, 2), Rounds = Load > 0 ? G->Run.Rounds[Load - 1] : 0;
	const bool bCut = G->Phase == EFTPhase::Wreck || G->Phase == EFTPhase::Wrap;

	// Live play is never dimmed or masked: the slate's dim and its slim mattes CUT when the race starts, as the slate
	// always did. Only at CUT! and THAT'S A WRAP, when the take is over, do the letterbox mattes ease in (they are
	// the highlight reel's, so the reel takes over without a cut).
	if (!bCut) { DimNow = 0.f; MatteNow = 0.f; }
	Backdrop(0.f, bCut && !bReducedMotion ? ReelBar() : 0.f);
	RaceMattes();

	// ---- what changed since the last frame (the feedback below is timed from these)
	const bool bArmorLow = Hp <= 25.f;
	if (PlayDrawn != DrawCount - 1)
	{
		ResetPlayFeedback(*G);
		PlaySince = T;
		bLastArmorLow = bArmorLow;   // (a take that starts on a burning car does not shout it again)
		ArmorLowAt = -10.f;
	}
	PlayDrawn = DrawCount;
	if (bArmorLow && !bLastArmorLow) ArmorLowAt = Now;
	bLastArmorLow = bArmorLow;
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
	// (15 Nov, the frame-cost pass.) The figures below are written through an FKept: the same Printf as before, run
	// only when a value it prints has changed. A float is keyed by its bits, so every value that prints differently
	// is a different key.
	const auto FloatKey = [](float Value) { return static_cast<uint64>(FMath::AsUInt(Value)); };
	const auto IntKey = [](int64 Value) { return static_cast<uint64>(Value); };

	// The camera's view: the viewfinder's corner brackets, at the true corners of the screen (red while the car burns).
	{
		FBlock Arrive(*this, In01(PlayAge, 0.f, DealTime));
		FTSkin::Brackets(Canvas, S, bArmorLow ? Red : White, Fade);
	}

	// Primary instruments sit on either side of the car and stay within the center 16:9 area.
	// ---- the driver's sticker: speed, armour, turbo (mock_hud.py, hud_driver)
	const float DriverW = 322 * S, DriverY = Base - 171 * S;
	{
		// a hit jolts the sticker, flashes it red and leaves the lost armour on the bar for a beat
		const float HitAge = Now - ArmorHitAt;
		const float Hit = Flash01(HitAge, .3f);
		FBlock Arrive(*this, In01(PlayAge, 0.f, DealTime), -48 * S, 0.f, ShakeX(HitAge, FMath::Clamp(ArmorHitSize * .4f, 2.5f, 8.f) * S, ShakeTime), 0.f);
		const float Y0 = DriverY;
		const FRaceSticker P(X, Y0 + 12 * K, DriverW, 171 * S - 12 * K, K);
		FTSkin::InkPlate(Canvas, S, P.X, P.Y, P.Wd, P.Ht, bArmorLow ? Red : Mix(White, Red, Hit), Fade);
		if (Hit > 0.f) FTSkin::Para(Canvas, P.X, P.Y, P.Wd - 5.f * P.Sc, P.Bh, P.Lean, Red, .2f * Hit * Fade);
		RaceTab(TEXT("KESTREL-88 / AGENT N. KANE"), P.Lx(P.Y) + 2 * K, Y0, bArmorLow ? Red : Cyan, bArmorLow ? FLinearColor::White : Ink);

		// speed: the big numerals (capitals 60 kit px); MPH stands on their baseline
		const float SpeedX = P.Lx(Y0 + 102 * K) + 12 * K;
		const int32 SpeedShown = FMath::RoundToInt(G->SpeedMph);
		const float SpeedW = RaceDigits(KeptSpeed.Get(IntKey(SpeedShown), 0, 0, [&] { return FString::FromInt(SpeedShown); }), SpeedX, Y0 + 42 * K, 60 * K, White);
		RacePrint(TEXT("MPH"), SpeedX + SpeedW + 10 * K, Y0 + 95.5f * K, Cyan, 16.f, 0, true);
		if (bArmorLow)
		{
			// ARMOR LOW!: the baked shout, inside the sticker's footprint, where the rev ticks are (they are not drawn
			// while the car burns). It is popped in by scale and is solid from its first frame.
			const float LowAge = Now - ArmorLowAt;
			const float Pop = bReducedMotion ? 1.f : FMath::Lerp(.4f, 1.f, EaseOut(LowAge, .12f)) + .16f * Flash01(LowAge - .12f, .2f);
			const float ShoutX = X + DriverW - 104 * K, ShoutY = Y0 + 66 * K;
			if (FTSkin::Texture(FTSkin::ETex::HudArmorLow)) FTSkin::ArmorLow(Canvas, S, ShoutX, ShoutY, Fade, .68f * Pop);
			else
			{
				FPivot PopIn(*this, FVector2D(ShoutX, ShoutY), Pop);
				RaceChip(ShoutX - 96 * K, ShoutY - 21 * K, 192 * K, 42 * K, Sticky);
				RacePrint(TEXT("ARMOR LOW!"), ShoutX, ShoutY, Ink, 16.f, 1, true);
			}
		}
		else
		{
			// twelve rev ticks, growing; the last two are the red line (pink)
			const float SpeedFrac = FMath::Clamp(G->SpeedMph / (900.f * .31f), 0.f, 1.f);
			const float TickR = P.Rx(Y0 + 102 * K) - 12 * K;
			const bool bAtlas = FTSkin::Texture(FTSkin::ETex::HudPips) != nullptr;
			for (int32 I = 0; I < 12; ++I)
			{
				const float He = (16.f + 2.6f * I) * K;
				const bool bLit = (I + 1) / 12.f <= SpeedFrac;
				const FLinearColor TickColor = bLit ? (I > 9 ? Pink : Cyan) : Steel;
				const float TickX = TickR - (12 - I) * 15.f * K, TickY = Y0 + 102 * K - He;
				if (bAtlas) FTSkin::Pip(Canvas, bLit ? FTSkin::EPip::SegmentFull : FTSkin::EPip::SegmentFlat, TickX, TickY, 12 * K, He, TickColor, Fade);
				else FTSkin::Para(Canvas, TickX, TickY, 12 * K, He, 3 * K, TickColor, Fade);
			}
		}
		{
			const float RuleL = P.Lx(Y0 + 111 * K) + 8 * K, RuleR = P.Rx(Y0 + 111 * K) - 8 * K;
			Flat(RuleL, Y0 + 110 * K, RuleR - RuleL, 2 * K, Alpha(White, .24f));
		}

		// armour: the icon, ARMOR, the percentage (big numerals) and now / max; the bar; the damage line
		const FLinearColor ArmorColor = Hp > 60.f ? Green : Hp > 30.f ? Amber : Red;
		const TCHAR* Damage = Hp > 79.f ? TEXT("NOMINAL") : Hp > 59.f ? TEXT("BODYWORK DAMAGED") : Hp > 39.f ? TEXT("WINDSHIELD CRACKED") : Hp > 24.f ? TEXT("ENGINE SMOKING") : TEXT("CRITICAL / ON FIRE");
		// critical: the warning beats (slowly: it is a state, not an attack)
		// (a danger read-out beats BRIGHTER, toward white, never dimmer: it is at full strength all the time)
		const float Beat = bArmorLow && !bReducedMotion ? FMath::Square(FMath::Sin(Now * PI * 1.6f)) : 0.f;
		// armour at 25 or less: the whole armour row is solid red with white text, and the damage line is a red bar
		const FLinearColor Danger = Mix(Red, FLinearColor::White, .22f * Beat);
		const FLinearColor ArmorInk = bArmorLow ? FLinearColor::White : ArmorColor;
		float RowL = P.Lx(Y0 + 114 * K) + 4 * K, RowR = P.Rx(Y0 + 168 * K) - 4 * K;
		if (bArmorLow) RaceChip(RowL - 6 * K, Y0 + 114 * K, RowR - RowL + 12 * K, 54 * K, Danger);
		RaceIcon(bArmorLow ? IcoWarning : IcoShield, RowL + 8 * K, Y0 + 117 * K, 30 * K, ArmorInk, static_cast<int32>(bArmorLow ? EIcon::Warning : EIcon::Repair));
		RacePrint(TEXT("ARMOR"), RowL + 46 * K, Y0 + 132 * K, bArmorLow ? FLinearColor::White : White, 19.f, 0, true);
		const float ArmorNow = G->Hp, ArmorMax = G->MaxHp();
		const FString& Pool = KeptPool.Get(FloatKey(ArmorNow), FloatKey(ArmorMax), 0, [&] { return FString::Printf(TEXT("%.0f / %.0f"), ArmorNow, ArmorMax); });
		const float PoolNatural = RaceTextW(Pool, 16.f);
		const float PoolW = FMath::Min(PoolNatural, 110 * K);
		RacePrint(Pool, RowR - 12 * K, Y0 + 132 * K, bArmorLow ? FLinearColor::White : Grey, 16.f, 2, false, 110 * K, PoolNatural);
		const int32 ArmorShown = FMath::RoundToInt(Hp);
		RaceDigits(KeptArmorPct.Get(IntKey(ArmorShown), 0, 0, [&] { return FString::Printf(TEXT("%d%%"), ArmorShown); }), RowR - 24 * K - PoolW, Y0 + 118.5f * K, 27 * K, Emphasis(ArmorChanged, ArmorInk), 2);
		{
			const float Gain = Flash01(Now - ArmorGainAt, .35f);
			const float BarH = 13 * K;
			FTSkin::HudMeter(Canvas, S, RowL + 8 * K, Y0 + 150 * K, RowR - RowL - 20 * K - BarH * .35f, BarH, Hp / 100.f, 12,
				Mix(ArmorInk, FLinearColor::White, .6f * Gain), bArmorLow ? Mix(Red, Ink, .62f) : Steel, Fade, ArmorGhost);
		}
		RowL = P.Lx(Y0 + 170 * K) + 4 * K; RowR = P.Rx(Y0 + 196 * K) - 4 * K;
		if (bArmorLow)
		{
			RaceChip(RowL - 6 * K, Y0 + 170 * K, RowR - RowL + 12 * K, 26 * K, Danger);
			RacePrint(Damage, (RowL + RowR) * .5f, Y0 + 183 * K, Ink, 16.f, 1, true);
		}
		else RacePrint(Damage, RowL + 8 * K, Y0 + 183 * K, Mix(ArmorColor, White, Flash01(Now - DamageStateAt, .5f)), 16.f);

		// turbo
		const TCHAR* Turbo = G->bTurboOn ? TEXT("BURN") : G->BoostT > 0.f ? TEXT("BOOST") : G->bTurboLock ? TEXT("RECHARGING") : TurboFrac >= .99f ? TEXT("READY") : TEXT("CHARGING");
		const FLinearColor TurboColor = G->bTurboOn ? Pink : Cyan;
		const float Ready = Flash01(Now - TurboReadyAt, .45f);
		RowL = P.Lx(Y0 + 210 * K) + 12 * K; RowR = P.Rx(Y0 + 238 * K) - 12 * K;
		RaceIcon(IcoBolt, RowL, Y0 + 197 * K, 26 * K, Mix(TurboColor, White, Ready), static_cast<int32>(EIcon::Turbo));
		RacePrint(TEXT("TURBO"), RowL + 32 * K, Y0 + 210 * K, White, 16.f, 0, true);
		const float TurboKeyX = RowL + 32 * K + RaceTextW(TEXT("TURBO"), 16.f, true) + 10 * K;
		// (the state is one of the words above as it stands; a string is put together only while drafting)
		const FString TurboDraft = G->bDraft ? FString(Turbo) + TEXT(" / DRAFT") : FString();
		const FStringView TurboState = G->bDraft ? FStringView(TurboDraft) : FStringView(Turbo);
		const float TurboStateW = RaceTextW(TurboState, 16.f);
		RacePrint(TurboState, RowR, Y0 + 210 * K, Mix(TurboColor, White, Ready), 16.f, 2, false, 0.f, TurboStateW);
		RaceKey(Hint(EFTAction::Turbo, TEXT("LB/RT")), TurboKeyX, Y0 + 210 * K, RowR - TurboStateW - 12 * K - TurboKeyX);
		RowL = P.Lx(Y0 + 238 * K) + 12 * K;
		FTSkin::HudMeter(Canvas, S, RowL, Y0 + 227 * K, RowR - RowL - 9 * K * .35f, 9 * K, TurboFrac, 8, Mix(TurboColor, FLinearColor::White, .5f * Ready), Steel, Fade, 0.f);
	}

	// ---- the ordnance sticker: missiles, ammo and its three loads, drift (mock_hud.py, hud_ordnance)
	const float WeaponW = 300 * S, WX = R - WeaponW, WY = Base - 163 * S;
	{
		FBlock Arrive(*this, In01(PlayAge, .04f, DealTime), 48 * S, 0.f);
		const float Y0 = WY;
		const FRaceSticker P(WX, Y0 + 12 * K, WeaponW, 163 * S - 12 * K, K);
		FTSkin::InkPlate(Canvas, S, P.X, P.Y, P.Wd, P.Ht, White, Fade);
		RaceTab(TEXT("ORDNANCE"), P.Lx(P.Y) + 2 * K, Y0, Yellow, Ink);

		// missiles: the count large (capitals 46), the capacity small (27) on its baseline; one arrow pip a missile.
		// A launch kicks the icon and the count, and the rack that emptied flashes; a pickup lights the row.
		const float Fired = Flash01(Now - MissileFiredAt, .22f), Loaded = Flash01(Now - MissileGainAt, .4f);
		float RowL = P.Lx(Y0 + 73 * K) + 12 * K, RowR = P.Rx(Y0 + 82 * K) - 12 * K;
		RaceIcon(IcoMissile, RowL + 5 * Fired * S, Y0 + 43 * K - 4 * Fired * S, 30 * K, Mix(Yellow, White, FMath::Max(Fired, Loaded)), static_cast<int32>(EIcon::Missile));
		RacePrint(TEXT("MISSILES"), RowL + 38 * K, Y0 + 58 * K, White, 19.f, 0, true);
		float KeyX = RowL + 38 * K + RaceTextW(TEXT("MISSILES"), 19.f, true) + 10 * K;
		const int32 Capacity = G->MissileCap();
		float CountW = 0.f;
		{
			FPivot Kick(*this, FVector2D(RowR, Y0 + 60 * K), 1.f + .22f * Flash01(Now - MissileChanged, PunchTime));
			CountW = RaceDigits(KeptMissileCap.Get(IntKey(Capacity), 0, 0, [&] { return FString::Printf(TEXT("/%d"), Capacity); }), RowR, Y0 + 55 * K, 27 * K, Grey, 2);
			CountW += RaceDigits(KeptMissiles.Get(IntKey(G->Missiles), 0, 0, [&] { return FString::FromInt(G->Missiles); }), RowR - CountW + K, Y0 + 36 * K, 46 * K, Emphasis(MissileChanged, G->Missiles > 0 ? Yellow : Grey), 2);
		}
		RaceKey(Hint(EFTAction::Missile, TEXT("B")), KeyX, Y0 + 58 * K, RowR - CountW - 12 * K - KeyX);
		const int32 Racks = FMath::Clamp(Capacity, 1, 16);
		RowL = P.Lx(Y0 + 118 * K) + 8 * K;
		RacePips(0, RowL, Y0 + 86 * K, G->Missiles, Racks, P.Rx(Y0 + 118 * K) - 8 * K - RowL, 32.f, Mix(Yellow, FLinearColor::White, .6f * Loaded), G->Missiles, Fired);

		// ammo: the load's rounds on the right (none for STD), the three loads as chips
		RowL = P.Lx(Y0 + 148 * K) + 12 * K; RowR = P.Rx(Y0 + 148 * K) - 12 * K;
		RaceIcon(IcoBullets, RowL, Y0 + 120 * K, 28 * K, Cyan, static_cast<int32>(EIcon::Ammo));
		RacePrint(TEXT("AMMO"), RowL + 38 * K, Y0 + 134 * K, White, 19.f, 0, true);
		KeyX = RowL + 38 * K + RaceTextW(TEXT("AMMO"), 19.f, true) + 10 * K;
		CountW = Load > 0 ? RaceDigits(KeptRounds.Get(IntKey(Rounds), 0, 0, [&] { return Num(Rounds); }), RowR, Y0 + 120.5f * K, 27 * K, Emphasis(AmmoChanged, Cyan), 2) : 0.f;
		RaceKey(Hint(EFTAction::Ammo, TEXT("Y")), KeyX, Y0 + 134 * K, RowR - CountW - 12 * K - KeyX);
		static const TCHAR* LoadNames[] = { TEXT("STD"), TEXT("AP"), TEXT("HE") };
		const float Switched = Flash01(Now - LoadSwitchAt, .25f);
		RowL = P.Lx(Y0 + 182 * K) + 6 * K; RowR = P.Rx(Y0 + 182 * K) - 2 * K;
		const float LoadW = (RowR - RowL - 8 * K) / 3.f;
		for (int32 I = 0; I < UE_ARRAY_COUNT(LoadNames); ++I)
		{
			const float Px = RowL + I * (LoadW + 4 * K);
			const bool bOwned = I == 0 || (I == 1 ? G->Stats.ApRank > 0 : G->Stats.HeRank > 0);
			const bool bLoaded = I == Load;
			RaceChip(Px, Y0 + 152 * K, LoadW, 30 * K, bLoaded ? Mix(Cyan, FLinearColor::White, .6f * Switched) : bOwned ? Steel : Track);
			RacePrint(LoadNames[I], Px + LoadW * .5f, Y0 + 167 * K, bLoaded ? Ink : bOwned ? White : Grey, 16.f, 1, true);
		}

		// drift
		const TCHAR* Drift = G->BoostT > 0.f ? TEXT("BOOST!") : !G->bDrifting ? TEXT("HOLD + STEER")
			: G->DriftLevel == 0 ? TEXT("SLIDING") : G->DriftLevel == 1 ? TEXT("BLUE") : G->DriftLevel == 2 ? TEXT("ORANGE") : TEXT("PINK!");
		const FLinearColor DriftColor = G->DriftLevel >= 3 ? Pink : G->DriftLevel == 2 ? Amber : Cyan;
		const float Charged = Flash01(Now - DriftUpAt, .3f);
		RowL = P.Lx(Y0 + 226 * K) + 12 * K; RowR = P.Rx(Y0 + 226 * K) - 12 * K;
		RacePrint(TEXT("DRIFT"), RowL, Y0 + 198 * K, White, 16.f, 0, true);
		KeyX = RowL + RaceTextW(TEXT("DRIFT"), 16.f, true) + 10 * K;
		const FStringView DriftState(Drift);
		const float DriftStateW = RaceTextW(DriftState, 16.f);
		RacePrint(DriftState, RowR, Y0 + 198 * K, G->bDrifting || G->BoostT > 0.f ? Mix(DriftColor, White, Charged) : Grey, 16.f, 2, false, 0.f, DriftStateW);
		RaceKey(Hint(EFTAction::Drift, TEXT("A/RB")), KeyX, Y0 + 198 * K, RowR - DriftStateW - 12 * K - KeyX);
		FTSkin::HudMeter(Canvas, S, RowL, Y0 + 215 * K, RowR - RowL - 9 * K * .35f, 9 * K, G->DriftLevel / 3.f, 3, Mix(DriftColor, FLinearColor::White, .7f * Charged), Steel, Fade, 0.f);
	}

	// Secondary information stays near the top edge. Temporary cards share the same lane.
	// ---- the scene slate: a clapper stripe along its top edge, the clapper, SCENE n/N, the take as a chip, FINALE;
	// under them the film strip with a tick at every scene and the finale's flag at 85 % (mock_hud.py, hud_scene)
	const float ObjectiveW = 362 * S, OX = Center - ObjectiveW * .5f;
	float NoticeY = Top + 58 * S;
	{
		FBlock Arrive(*this, In01(PlayAge, .02f, DealTime), 0.f, -40 * S);
		const FRaceSticker P(OX, Top, ObjectiveW, 51 * S, K);
		FTSkin::InkPlate(Canvas, S, P.X, P.Y, P.Wd, P.Ht, White, Fade);
		{
			const float StripeL = P.Lx(Top + 5 * K) - K, StripeR = P.Rx(Top + 13 * K) + 5 * K;
			FTSkin::HazardStripes(Canvas, S, StripeL, Top + 5 * K, StripeR - StripeL, 8 * K, White, Fade);
		}
		const float RowL = P.Lx(Top + 14 * K) + 10 * K, RowR = P.Rx(Top + 66 * K) - 10 * K;
		RaceIcon(IcoClapper, RowL, Top + 14 * K, 28 * K, White);
		const int32 SceneNo = G->Scene + 1, SceneCount = G->NumScenes();
		const FString& SceneText = KeptScene.Get(IntKey(SceneNo), IntKey(SceneCount), 0, [&] { return FString::Printf(TEXT("SCENE %d/%d"), SceneNo, SceneCount); });
		RacePrint(SceneText, RowL + 36 * K, Top + 29 * K, White, 19.f, 0, true);
		const float TakeX = RowL + 36 * K + RaceTextW(SceneText, 19.f, true) + 14 * K;
		const FString& TakeText = KeptTake.Get(IntKey(G->Take), 0, 0, [&] { return FString::Printf(TEXT("TAKE %d"), G->Take); });
		const float TakeTextW = RaceTextW(TakeText, 16.f, true);
		const float TakeW = TakeTextW + 28 * K;
		RaceChip(TakeX, Top + 16 * K, TakeW, 26 * K, White);
		RacePrint(TakeText, TakeX + TakeW * .5f, Top + 29 * K, Ink, 16.f, 1, true, 0.f, TakeTextW);
		RacePrint(TEXT("FINALE"), RowR, Top + 29 * K, Pink, 16.f, 2, true);
		const float StripX = P.Lx(Top + 66 * K) + 8 * K, StripY = Top + 46 * K, StripH = 18 * K, StripW = RowR - StripX;
		FTSkin::FilmMeter(Canvas, S, StripX, StripY, StripW, StripH, G->Progress, 1, Cyan, Steel, Fade);
		const bool bAtlas = FTSkin::Texture(FTSkin::ETex::HudPips) != nullptr;
		for (const FFTScene& Scene : Level.Scenes)
		{
			const float At = FMath::Clamp(Scene.D / FMath::Max(1.f, Level.FinaleD), 0.f, 1.f);
			if (At <= .001f || At >= .999f) continue;   // (the first scene starts at the strip's end; the finale has the flag)
			if (bAtlas) FTSkin::Pip(Canvas, FTSkin::EPip::Tick, StripX + 3 * K + (StripW - 6 * K) * At * .85f - 8 * K, StripY + K, 16 * K, StripH - 2 * K, White, Fade);
			else FTSkin::FilmMeterMark(Canvas, S, StripX, StripY + 3 * K, StripW, StripH - 6 * K, At * .85f, White, .7f * Fade);
		}
		if (bAtlas) FTSkin::Pip(Canvas, FTSkin::EPip::FinaleMark, StripX + 3 * K + (StripW - 6 * K) * .85f - 14 * K, StripY - 9 * K, 27 * K, 27 * K, Pink, Fade);
		else FTSkin::FilmMeterMark(Canvas, S, StripX, StripY, StripW, StripH, .85f, Pink, Fade);
	}
	if (G->bBossOn)
	{
		// The boss's sticker drops in and its bar fills; every hit leaves what he just lost on the bar for a beat; a
		// third of his armour gone (the next phase of the fight) jolts the sticker and flashes it white.
		// (mock_hud.py, hud_boss)
		const float BossAge = Now - BossOnAt, PhaseAge = Now - BossPhaseAt;
		const float Phased = Flash01(PhaseAge, .45f);
		FBlock Arrive(*this, In01(BossAge, 0.f, DealTime), 0.f, -18 * S, ShakeX(PhaseAge, 7 * S, .42f), 0.f);
		const float Y0 = NoticeY;
		const FRaceSticker P(OX, Y0, ObjectiveW, 66 * S, K);
		FTSkin::InkPlate(Canvas, S, P.X, P.Y, P.Wd, P.Ht, Mix(Red, FLinearColor::White, Phased), Fade);
		if (Phased > 0.f) FTSkin::Para(Canvas, P.X, P.Y, P.Wd - 5.f * P.Sc, P.Bh, P.Lean, FLinearColor::White, .22f * Phased * Fade);
		AccentSweep(Rect(P.Lx(P.Y) + 4 * K, Y0 + 6 * K, P.Rx(P.Y) - P.Lx(P.Y) - 8 * K, 3 * S), BossAge, Red, MenuEntrance);
		const float RowL = P.Lx(Y0 + 12 * K) + 10 * K, RowR = P.Rx(Y0 + 86 * K) - 10 * K;
		RaceIcon(IcoSkull, RowL, Y0 + 11 * K, 32 * K, Mix(White, FLinearColor::White, Phased), static_cast<int32>(EIcon::Warning));
		RacePrint(G->BossLabel, RowL + 42 * K, Y0 + 27 * K, White, 24.f, 0, true, 300 * K);
		const FString GapLabel = G->bLastChance ? TEXT("LAST CHANCE!") : G->BossRate < 1.f
			? FString::Printf(TEXT("GAP ×%.2f"), G->BossRate) : FString(TEXT("GAP"));
		const float Urgent = G->bLastChance && !bReducedMotion ? FMath::Square(FMath::Sin(Now * PI * 1.5f)) : 0.f;   // brighter, never dimmer; slow: no strobe
		if (G->bLastChance)
		{
			// LAST CHANCE!: white on a red chip, the same words of danger as the armour row
			const float GapTextW = RaceTextW(GapLabel, 16.f, true);
			const float GapW = GapTextW + 30 * K;
			RaceChip(RowR + 8 * K - GapW, Y0 + 13 * K, GapW, 28 * K, Mix(Red, FLinearColor::White, .22f * Urgent));
			RacePrint(GapLabel, RowR + 8 * K - GapW * .5f, Y0 + 27 * K, FLinearColor::White, 16.f, 1, true, 0.f, GapTextW);
		}
		else RacePrint(GapLabel, RowR, Y0 + 27 * K, Yellow, 16.f, 2, true, 180 * K);
		const float BarX = P.Lx(Y0 + 86 * K) + 10 * K, BarW = RowR - BarX, BarH = 17 * K;
		const float Filling = bReducedMotion ? 1.f : EaseOut(BossAge - .1f, .5f);
		FTSkin::HudMeter(Canvas, S, BarX, Y0 + 50 * K, BarW - BarH * .35f, BarH, G->BossHpFrac * Filling, 8, Red, Mix(Red, Ink, .72f), Fade, BossGhost * Filling);
		// where the fight changes: a notch at two thirds and at one third (chalk while it is still to come)
		const bool bAtlas = FTSkin::Texture(FTSkin::ETex::HudPips) != nullptr;
		for (const float Line : { .66f, .33f })
		{
			const FLinearColor Notch = G->BossHpFrac > Line ? White : Alpha(Grey, .7f);
			if (bAtlas) FTSkin::Pip(Canvas, FTSkin::EPip::Tick, BarX + BarW * Line - 8 * K, Y0 + 45 * K, 16 * K, 27 * K, Notch, Fade);
			else Flat(BarX + BarW * Line - K, Y0 + 45 * K, 2 * K, 27 * K, Notch);
		}
		const float Struck = Flash01(Now - BossHitAt, .14f);
		if (Struck > 0.f) Flat(BarX + BarW * G->BossHpFrac - 2 * K, Y0 + 46 * K, 5 * K, 25 * K, Alpha(FLinearColor::White, Struck));
		// the gap: how long before he gets away
		Flat(BarX, Y0 + 77 * K, BarW, 7 * K, Steel);
		Flat(BarX, Y0 + 77 * K, BarW * FMath::Clamp(G->BossClock / FMath::Max(1.f, G->BossEscapeT), 0.f, 1.f), 7 * K, G->bLastChance ? Red : Yellow);
		NoticeY += 73 * S;
	}
	if (Now >= G->CaptionT && Now - G->CaptionT < 3.f)
	{
		// a warning is at full strength from its first frame: a solid red chip the width of the slate, white text; it
		// lands a touch large with a flash, it never fades in. (Rajdhani, not Bungee: a caption can be a whole sentence.)
		const float Age = Now - G->CaptionT, Landed = Flash01(Age, .22f);
		FPivot Land(*this, FVector2D(Center, NoticeY + 15 * S), 1.f + .14f * Flash01(Age, .16f));
		RaceChip(OX, NoticeY, ObjectiveW, 46 * K, Mix(Red, FLinearColor::White, .3f * Landed));
		RacePrint(G->Caption, Center, NoticeY + 23 * K, FLinearColor::White, 22.f, 1, false, ObjectiveW - 52 * K);
		NoticeY += 37 * S;
	}
	if (!G->Line.IsEmpty() && Now - G->LineT < G->LineHold + 1.2f)
	{
		// the director's line: the megaphone badge, the DIRECTOR chip in the line's priority colour (cyan a remark,
		// pink a warning of an attack, amber the loud ones) and the speech balloon hugging the line; the group is
		// centred in the lane the line always had (480 x 36). (mock_hud.py, hud_director)
		const FLinearColor Accent = G->LinePri >= 3 ? Yellow : G->LinePri >= 2 ? Pink : Cyan;
		const float Offset = bReducedMotion ? 0.f : (1.f - EaseOut(Now - G->LineT)) * 6 * S;
		const float Y0 = NoticeY - Offset;
		// the director's line fades out over its last third of a second instead of blinking off
		const float Left01 = bReducedMotion ? 1.f : FMath::Clamp((G->LineHold + 1.2f - (Now - G->LineT)) / .3f, 0.f, 1.f);
		// (a line of priority 2 or more is a warning of an attack: solid from its first frame; a remark fades in)
		FBlock Speak(*this, FMath::Min(G->LinePri >= 2 ? 1.f : In01(Now - G->LineT, 0.f, .14f), Left01));
		const float LineMax = 496 * K;
		const float LineNatural = RaceTextW(G->Line, 19.f);
		const float LineW = FMath::Min(LineNatural, LineMax);
		const float ChipW = 40 * K + RaceTextW(TEXT("DIRECTOR"), 16.f, true);
		const float GroupX = Center - (34 * K + ChipW + LineW + 64 * K) * .5f;
		RaceChip(GroupX + 34 * K, Y0 + 11 * K, ChipW, 32 * K, Accent);
		RacePrint(TEXT("DIRECTOR"), GroupX + 60 * K, Y0 + 27 * K, Ink, 16.f, 0, true);
		FTSkin::TexNat(Canvas, S, FTSkin::ETex::HudMegaphone, GroupX, Y0, FTSkin::White, Fade, .75f);
		// (the balloon's rectangle starts 6 kit px under the Y it is given; today's lane is 54 kit px high)
		const FBox2D Says = FTSkin::Balloon(Canvas, S, GroupX + 34 * K + ChipW - 6 * K, Y0 - 6 * K, LineW, Fade, false, 54 * K);
		RacePrint(G->Line, float(Says.Min.X), float(Says.Min.Y + Says.Max.Y) * .5f, Ink, 19.f, 0, false, LineMax, LineNatural);
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
		RaceChip(Center - 148 * S, NoticeY, 296 * S, 48 * K, Mix(Yellow, FLinearColor::White, .5f * Landed));
		RacePrint(Text, Center, NoticeY + 24 * K, Ink, 24.f, 1, true, 390 * K);
	}

	// The recording ledger and the latest three rewards stay out of the driving corridor.
	// ---- the ledger: NOW FILMING, the box office, the combo, the counts, the film left (mock_hud.py, hud_ledger)
	const float LedgerW = 255 * S, LX = R - LedgerW;
	{
		FBlock Arrive(*this, In01(PlayAge, .06f, DealTime), 48 * S, 0.f);
		const FRaceSticker P(LX, Top + 12 * K, LedgerW, 173 * S - 12 * K, K);
		FTSkin::InkPlate(Canvas, S, P.X, P.Y, P.Wd, P.Ht, White, Fade);
		{
			const float TabX = P.Lx(P.Y) + 2 * K;
			RaceChip(TabX, Top, 54 * K + RaceTextW(TEXT("NOW FILMING"), 16.f, true), 30 * K, Pink);
			// the camera's light: a slow blink while it films
			const float Rec = bReducedMotion || bCut ? 1.f : .5f + .5f * FMath::Square(FMath::Cos(G->Timecode * PI * .5f));
			RaceIcon(IcoRec, TabX + 14 * K, Top + 4 * K, 22 * K, Alpha(FLinearColor::White, Rec), static_cast<int32>(EIcon::Record));
			RacePrint(TEXT("NOW FILMING"), TabX + 42 * K, Top + 15 * K, Ink, 16.f, 0, true);
		}
		const int32 Tc = FMath::FloorToInt(G->Timecode);
		const int32 FrameNo = FMath::FloorToInt(FMath::Fmod(G->Timecode, 1.f) * 24.f);
		RacePrint(KeptClock.Get(IntKey(Tc), IntKey(FrameNo), 0, [&] { return FString::Printf(TEXT("%02d:%02d:%02d"), Tc / 60, Tc % 60, FrameNo); }), P.Rx(Top + 34 * K) - 12 * K, Top + 34 * K, Grey, 16.f, 2);
		RacePrint(TEXT("BOX OFFICE"), P.Lx(Top + 60 * K) + 12 * K, Top + 56 * K, Yellow, 16.f, 0, true);
		float RowL = P.Lx(Top + 108 * K) + 12 * K;
		// the box office in big numerals; a long figure is drawn smaller to fit, and gives the combo's burst its room
		const int64 ScoreFigure = FMath::RoundToInt64(ScoreShown);
		RaceDigits(KeptScore.Get(IntKey(ScoreFigure), 0, 0, [&] { return Money(ScoreFigure); }), RowL, Top + 72 * K, 36 * K, Mix(Yellow, FLinearColor::White, .7f * Flash01(Now - ScoreUpAt, .3f)), 0,
			P.Rx(Top + 108 * K) - 12 * K - RowL - (G->Combo > 1 ? 112 * K : 0.f));
		RowL = P.Lx(Top + 148 * K) + 12 * K;
		if (G->Combo > 1)
		{
			// the combo: the pink starburst with its multiplier (it kicks when the combo grows), COMBO and the time
			// left to keep it (the bar blinks toward white through its last second, never dimmer)
			const float Up = Flash01(Now - ComboUpAt, PunchTime);
			const float BurstX = P.Rx(Top + 98 * K) - 58 * K, BurstY = Top + 96 * K;
			if (FTSkin::Texture(FTSkin::ETex::HudComboFill)) FTSkin::Combo(Canvas, S, BurstX, BurstY, G->Combo, Mix(Pink, FLinearColor::White, .45f * Up), Fade, .76f * (1.f + .28f * Up), FTSkin::White);
			else RacePrint(FString::Printf(TEXT("x%d"), G->Combo), BurstX, BurstY, Mix(Pink, White, Up), 30.f, 1, true);
			RacePrint(TEXT("COMBO"), RowL, Top + 136 * K, Pink, 16.f, 0, true);
			const float BarX = RowL + RaceTextW(TEXT("COMBO"), 16.f, true) + 12 * K, BarW = BurstX - 58 * K - BarX;
			const float Ending = G->ComboT < 1.f && !bReducedMotion ? FMath::Square(FMath::Sin(Now * PI * 2.5f)) : 0.f;   // toward white, never dimmer
			Flat(BarX, Top + 132 * K, BarW, 8 * K, Steel);
			Flat(BarX, Top + 132 * K, BarW * FMath::Clamp(G->ComboT / 3.f, 0.f, 1.f), 8 * K, Mix(Pink, FLinearColor::White, .6f * Ending));
		}
		else
		{
			// a kill or a near miss brightens the count
			RacePrint(KeptCounts.Get(IntKey(G->Kills), IntKey(G->NearMisses), 0, [&] { return FString::Printf(TEXT("KILLS %d    NEAR MISSES %d"), G->Kills, G->NearMisses); }), RowL, Top + 136 * K,
				Mix(White, Cyan, Flash01(Now - CountsAt, .35f)), 19.f, 0, false, 320 * K);
		}
		RacePrint(KeptStunts.Get(IntKey(G->StuntCount), IntKey(G->Collateral), 0, [&] { return FString::Printf(TEXT("STUNTS %d   /   COLLATERAL %d"), G->StuntCount, G->Collateral); }), P.Lx(Top + 166 * K) + 12 * K, Top + 166 * K, Grey, 16.f, 0, false, 320 * K);
		const int32 Time = FMath::FloorToInt(G->LevelClock), Par = ProgressData.Levels.IsValidIndex(G->Level) ? ProgressData.Levels[G->Level].Par : 0;
		const int32 CarnagePct = FMath::RoundToInt(100.f * G->Run.Wrecked / FMath::Max(1, G->ScriptTotal));
		RacePrint(KeptCarnage.Get(IntKey(CarnagePct), IntKey(Time), IntKey(Par), [&] { return FString::Printf(TEXT("CARNAGE %d%%  /  %d:%02d  PAR %d:%02d"),
			CarnagePct, Time / 60, Time % 60, Par / 60, Par % 60); }),
			P.Lx(Top + 192 * K) + 12 * K, Top + 192 * K, Grey, 16.f, 0, false, 320 * K);
		// the film left: FILM n/N and one reel pip a reel (up to ten), right aligned
		RowL = P.Lx(Top + 224 * K) + 12 * K;
		const int32 FilmLeft = FMath::Max(0, G->Stats.Reels - G->Retakes), FilmReels = G->Stats.Reels;
		const FString& Film = KeptFilm.Get(IntKey(FilmLeft), IntKey(FilmReels), 0, [&] { return FString::Printf(TEXT("FILM  %d/%d"), FilmLeft, FilmReels); });
		RacePrint(Film, RowL, Top + 224 * K, White, 16.f, 0, true);
		const int32 Reels = FMath::Clamp(G->Stats.Reels, 1, 10);
		const int32 ReelsLeft = FMath::Clamp(FMath::RoundToInt(float(G->Stats.Reels - G->Retakes) / FMath::Max(1, G->Stats.Reels) * Reels), 0, Reels);
		const float PipR = P.Rx(Top + 240 * K) - 8 * K;
		const float PipRoom = FMath::Min(PipR - RowL - RaceTextW(Film, 16.f, true) - 14 * K, 224 * K);
		RacePips(2, PipR - Reels * 32 * K * FMath::Clamp(PipRoom / (Reels * 32 * K), .3f, 1.f), Top + 208 * K, ReelsLeft, Reels, PipRoom, 32.f, Yellow);
	}

	// the reward tags under the ledger: the latest three, the newest on top, their right edge on the ledger's
	for (int32 I = FMath::Max(0, G->Popups.Num() - 3); I < G->Popups.Num(); ++I)
	{
		const FFTPopup& Popup = G->Popups[I];
		const float Opacity = bReducedMotion ? 1.f : FMath::Clamp((1.6f - Popup.T) * 3.f, 0.f, 1.f);
		const float Py = Top + (182 + (G->Popups.Num() - 1 - I) * 32) * S;
		const bool bRepair = (Popup.Text.StartsWith(TEXT("+")) && Popup.Text.EndsWith(TEXT(" ARMOR"))) || Popup.Text == TEXT("ARMOR FULL");
		const bool bMissile = (Popup.Text.StartsWith(TEXT("+")) && (Popup.Text.EndsWith(TEXT(" MISSILES")) || Popup.Text.EndsWith(TEXT(" MISSILE")))) || Popup.Text == TEXT("MISSILES FULL");
		const FLinearColor TagColor = bRepair ? Green : bMissile ? Yellow : FLinearColor(Popup.Col.R, Popup.Col.G, Popup.Col.B, 1.f);
		// a reward slides in from the edge with a flash; a stunt (the big ones) lands a touch large with a longer flash
		const bool bBig = Popup.Big > 1.f;
		const float Landed = Flash01(Popup.T, bBig ? .3f : .18f);
		FBlock Arrive(*this, In01(Popup.T, 0.f, .13f), 26 * S, 0.f);
		FBlock Leave(*this, Opacity);
		const bool bIcon = bRepair || bMissile;
		const float TextRoom = (bIcon ? 270 : 300) * K;
		const float TextX = (bIcon ? 58 : 28) * K;
		const float PopupNatural = RaceTextW(Popup.Text, 18.f, true);
		const float TagW = TextX + FMath::Min(PopupNatural, TextRoom) + 20 * K;
		const float TagX = R - TagW;
		FPivot Land(*this, FVector2D(R, Py + 20 * K), bBig ? 1.f + .12f * Flash01(Popup.T, .16f) : 1.f);
		const FLinearColor TagTint = Mix(TagColor, FLinearColor::White, (bBig ? .6f : .4f) * Landed);
		if (FTSkin::Texture(FTSkin::ETex::HudTag)) FTSkin::HudTag(Canvas, TagX, Py, TagW, 40 * K, TagTint, Fade);
		else
		{
			Flat(TagX, Py, TagW, 38 * K, Ink);
			Flat(TagX + 3 * K, Py + 3 * K, TagW - 6 * K, 32 * K, TagTint);
		}
		if (bIcon) RaceIcon(bRepair ? IcoWrench : IcoMissile, TagX + 26 * K, Py + 5 * K, 26 * K, Ink, static_cast<int32>(bRepair ? EIcon::Repair : EIcon::Missile));
		RacePrint(Popup.Text, TagX + TextX, Py + 18 * K, Ink, 18.f, 0, true, TextRoom, PopupNatural);
	}
	if (G->bShowKeys && GI)
	{
		// the keys line: a slanted slate strip, each key in its cap with what it does beside it (mock_hud.py, hud_keys)
		// (the keys go after ten seconds of the race: they fade out over the last half second rather than blink off)
		FBlock Keyline(*this, bReducedMotion || G->Phase != EFTPhase::Play ? 1.f : FMath::Min(In01(PlayAge, .1f, DealTime), FMath::Clamp((10.f - G->PhaseT) / .5f, 0.f, 1.f)));
		const FString KeyNames[] = { Hint(EFTAction::Drift, TEXT("A/RB")), Hint(EFTAction::Turbo, TEXT("LB/RT")), Hint(EFTAction::Fire, TEXT("X")),
			Hint(EFTAction::Missile, TEXT("B")), Hint(EFTAction::Ammo, TEXT("Y")), InputHint(this, TEXT("P / ESC"), EKeys::Gamepad_Special_Right) };
		static const TCHAR* KeyDoes[] = { TEXT("DRIFT"), TEXT("TURBO"), TEXT("FIRE"), TEXT("MISSILE"), TEXT("AMMO"), TEXT("PAUSE") };
		const float CapH = 30 * K, Between = 24 * K;
		float CapW[UE_ARRAY_COUNT(KeyDoes)], DoesW[UE_ARRAY_COUNT(KeyDoes)];
		float Whole = -Between;
		for (int32 I = 0; I < UE_ARRAY_COUNT(KeyDoes); ++I)
		{
			CapW[I] = FMath::Max(CapH, RaceTextW(KeyNames[I], 16.f) + CapH * .62f);
			DoesW[I] = RaceTextW(KeyDoes[I], 16.f);
			Whole += CapW[I] + 8 * K + DoesW[I] + Between;
		}
		// (rebound keys with long names: the row is set narrower, never wider than the line always was)
		const float Fit = FMath::Min(1.f, (612 * S - 56 * K) / Whole);
		const float StripW = Whole * Fit + 56 * K, StripY = H - 32 * S, MidY = StripY + 16 * K;
		RaceChip(Center - StripW * .5f, StripY, StripW, 33 * K, FTSkin::Slate);
		float Px = Center - Whole * Fit * .5f;
		for (int32 I = 0; I < UE_ARRAY_COUNT(KeyDoes); ++I)
		{
			Px += RaceKey(KeyNames[I], Px, MidY, Fit < 1.f ? CapW[I] * Fit : 0.f, 30.f) + 8 * K * Fit;
			RacePrint(KeyDoes[I], Px, MidY, White, 16.f, 0, false, Fit < 1.f ? DoesW[I] * Fit : 0.f, DoesW[I]);
			Px += (DoesW[I] + Between) * Fit;
		}
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
	const float K = S / 1.5f;   // screen px per kit px: the numbers below that are times K are mock_hud.py's (screen_card)
	// the slate is on film: a slim letterbox while it is up (a row of perforations along each matte); the dim behind
	// it eases in from the last screen's. Both cut when the race starts (DrawPlay): nothing dims live play. (Backdrop)
	Backdrop(FMath::Lerp(.68f, .96f, FMath::Clamp((Time - 2.9f) / .3f, 0.f, 1.f)), bReducedMotion ? 0.f : ReelBar() * .5f);
	RaceMattes();
	// The clapper: its arm is hinged at the left end, held open while the card drops in, lifts a little more, then
	// snaps shut (it meets the card as the clap sounds, 1.0 s in). The clap knocks the card and lights the stick.
	const float Closed = bReducedMotion ? 1.f : EaseIn(Time - ClapperClose, ClapperTravel);
	const float Shut = Time - ClapperClose - ClapperTravel;
	// the card lifts away over the slate's last quarter second (a skipped slate simply cuts)
	const float Leaving = bReducedMotion ? 0.f : EaseIn(Time - 2.95f, .25f);
	FBlock Arrive(*this, In01(bReducedMotion ? -1.f : Time, 0.f, DealTime), 0.f, -30 * S, 0.f, 5 * S * Flash01(Shut, .16f));
	FBlock Leave(*this, 1.f - Leaving, 0.f, -20 * S);
	// the slate itself: clapper-slate black with chalk dust and a chalk hairline, in its ink outline on its hard shadow
	FTSkin::SlatePanel(Canvas, S, X, Y, 640 * S, 310 * S, Fade);
	MarkTone(Rect(X, Y, 640 * S, 310 * S), ETone::Dark);   // (chalk stays chalk on it, even while the last screen's wall is still clearing behind)
	// the fixed jaw on the slate's top edge (the stick, flipped), and the arm on it, hinged at its left end
	RaceStick(X + 2 * K, Y + 11 * K, FLinearColor::White, true);
	{
		const float Lift = bReducedMotion ? 0.f : FMath::SmoothStep(ClapperClose - .22f, ClapperClose, Time);
		FPivot Hinge(*this, FVector2D(X + 26 * K, Y - 5 * K), 1.f, -(11.f + 3.f * Lift) * (1.f - Closed));
		RaceStick(X + 2 * K, Y - 29 * K, FLinearColor::White, false);
		const float Snap = Flash01(Shut, .14f);
		if (Snap > 0.f) Flat(X + 6 * K, Y - 25 * K, 948 * K, 40 * K, Alpha(FLinearColor::White, .8f * Snap));
	}
	AccentSweep(Rect(X + 6 * K, Y + 60 * K, 640 * S - 12 * K, 3 * S), Shut, Cyan, FocusSweep);
	const FFTLevelDef& Level = G->Def();
	const int32 Roll = 40 + (G->GI() ? G->GI()->RunCounter : 1);
	RacePrint(FString::Printf(TEXT("FULL THROTTLE  /  ROLL A-%d"), Roll), X + 33 * K, Y + 82 * K, Grey, 16.f);
	{
		// (the slate's own lettering is chalk: no second, pink printing under it)
		const bool bWasSticker = bSticker;
		bSticker = true;
		RacePrint(G->Take > 1 ? TEXT("RESET THE CARS") : Level.Zone, X + 31 * K, Y + 142 * K, White, 64.f, 0, true, 894 * K);
		bSticker = bWasSticker;
	}
	RacePrint(G->Take > 1 ? TEXT("FRESH PAINT ON THE KESTREL. ROLLING…") : Level.Title + TEXT("  /  ") + Level.Place,
		X + 33 * K, Y + 202 * K, Grey, 22.f, 0, false, 894 * K);
	Flat(X + 33 * K, Y + 240 * K, 894 * K, 2 * K, Alpha(White, .35f));
	// SCENE, TAKE and LEVEL: a label over big numerals, a chalk rule between one and the next
	const auto Reading = [&](const TCHAR* Label, const FString& Value, const FLinearColor& Tint, float At)
	{
		if (At > 40.f) Flat(X + (At - 24.f) * K, Y + 258 * K, 2 * K, 96 * K, Alpha(White, .24f));
		RacePrint(Label, X + At * K, Y + 277 * K, Grey, 16.f);
		RaceDigits(Value, X + (At - 2.f) * K, Y + 298 * K, 46 * K, Tint);
	};
	Reading(TEXT("SCENE"), FString::Printf(TEXT("%02d/%02d"), G->Scene + 1, G->NumScenes()), Cyan, 36.f);
	Reading(TEXT("TAKE"), FString::Printf(TEXT("%02d"), G->Take), G->Take > 1 ? Red : White, 348.f);
	Reading(TEXT("LEVEL"), FString::Printf(TEXT("%02d"), Level.Index + 1), White, 658.5f);
	FTSkin::Signature(Canvas, S, X + 30 * K, Y + 372 * K, FTSkin::Mute, .55f * Fade);   // the director's hand, in chalk
	if (Time > 1.2f)
	{
		// the key in its cap, SKIP SLATE beside it, right aligned where the prompt always was
		FBlock Prompt(*this, bReducedMotion ? 1.f : EaseOut(Time - 1.2f, .18f));
		const FString SkipKey = ConfirmHint(this);
		const float SkipW = RaceTextW(TEXT("SKIP SLATE"), 16.f);
		const float SkipCapW = FMath::Max(30 * K, RaceTextW(SkipKey, 16.f) + 30 * K * .62f);
		RaceKey(SkipKey, X + 925 * K - SkipW - 8 * K - SkipCapW, Y + 409 * K, 0.f, 30.f);
		RacePrint(TEXT("SKIP SLATE"), X + 925 * K, Y + 409 * K, Grey, 16.f, 2);
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
	const float K = S / 1.5f;   // screen px per kit px: the numbers below that are times K are mock_hud.py's (screen_cut)
	const float CardX = Mid - 330 * S, CardW = 660 * S, CardH = 112 * S;
	// CUT! is shouted: the card comes down large and fast, lands in a tenth of a second, flashes, and jolts
	const float Down = bReducedMotion ? 1.f : EaseIn(Age, .1f);
	const float Since = Age - .1f;
	const float Landed = Flash01(Since, .22f);
	FBlock Arrive(*this, .25f + .75f * Down, 0.f, 0.f, ShakeX(Since, 6 * S, .26f), 0.f);
	FPivot Slam(*this, FVector2D(Mid, Y + CardH * .5f), 1.f + .34f * (1.f - Down));
	// a slate shut on the take: slate black in its ink outline on its hard shadow, the clapper's stick down on its top
	// edge in the cut's red, a red rule along its foot
	FTSkin::SlatePanel(Canvas, S, CardX, Y, CardW, CardH, Fade);
	MarkTone(Rect(CardX, Y, CardW, CardH), ETone::Dark);
	RaceStick(CardX + 17 * K, Y - K, Mix(Red, FLinearColor::White, .6f * Landed), false);
	if (Landed > 0.f) Flat(CardX, Y, CardW, CardH, Alpha(FLinearColor::White, .3f * Landed));
	const float Wipe = bReducedMotion ? 1.f : EaseOut(Since);
	Flat(CardX + 3 * K, Y + CardH - 10 * K, (CardW - 6 * K) * Wipe, 7 * K, Red);
	AccentSweep(Rect(CardX + 6 * K, Y + 48 * K, CardW - 12 * K, 3 * S), Since, Red, MenuEntrance);
	{
		// the title: red over its ink print (the slate's own lettering: no second, pink printing under it)
		const bool bWasSticker = bSticker;
		bSticker = true;
		const float TitleY = Y + 83 * K + 6.f * (1.f - Wipe) * S;
		RacePrint(G->CutTitle, Mid + 4 * K, TitleY + 4 * K, Ink, 66.f, 1, true, 924 * K);
		RacePrint(G->CutTitle, Mid, TitleY, Mix(Red, FLinearColor::White, Landed), 66.f, 1, true, 924 * K);
		bSticker = bWasSticker;
	}
	// the caption under it: why the camera stopped, or what comes next
	RacePrint(G->CutSub, Mid, Y + 134 * K, White, 24.f, 1, false, 924 * K);
}

void AFTHUD::DrawReel(AFTGameMode* G)
{
	const float Mid = W * .5f, X = Left + Safe * S, R = Right - Safe * S;
	const float Bar = ReelBar();
	const float K = S / 1.5f;   // screen px per kit px: the numbers below that are times K are mock_hud.py's (screen_reel)
	const float Entry = MenuAge(EMenuView::Reel);
	// The highlights' title card holds the picture dark behind it; card and dark clear together over its last fifth of
	// a second, so the first shot comes up from under them. The mattes are the ones CUT! closed (Backdrop): no cut.
	// They are film: a row of perforations runs along the picture's side of each.
	const float Holding = G->ReelTitleT > 0.f ? (bReducedMotion ? 1.f : FMath::Clamp(G->ReelTitleT / .2f, 0.f, 1.f)) : 0.f;
	Backdrop(.88f * Holding, Bar);
	RaceMattes();
	const float ShotAge = MenuAge(EMenuView::Reel, true);
	if (G->ReelShot != LastReelShot)
	{
		// a splice between shots (not for the first frame of a reel: its title card has its own sound)
		if (LastReelShot >= 0 && !G->bReelDone && G->ReelTitleT <= 0.f && G->ReelShot > 1) PlayCue(TEXT("ReelClick"), .3f);
		LastReelShot = G->ReelShot;
	}
	if (!G->bReelDone)
		AccentSweep(Rect(R - 318 * S, Bar - 16 * K - 5 * S, 318 * S, 4 * S), ShotAge, Pink, FocusSweep);
	const FFTLevelDef& Level = G->Def();
	if (G->ReelTitleT > 0.f)
	{
		// the title card: call-sheet paper in its ink outline on its hard shadow, a clapper stripe along its head, the
		// title in ink over its pink print (Print sets a title that way), a rule, the shoot
		const float CardX = Mid - 335 * S, CardY = H * .34f, CardW = 670 * S, CardH = 158 * S;
		FBlock Arrive(*this, In01(Entry, .04f, DealTime), 0.f, -28 * S);
		FBlock Leave(*this, Holding, 0.f, -16 * S);
		FTSkin::PaperPanel(Canvas, S, CardX, CardY, CardW, CardH, FTSkin::Paper, Fade);
		MarkTone(Rect(CardX, CardY, CardW, CardH), ETone::Dark);   // (the colours below are given as inks: Print is to set them as they are)
		FTSkin::ClapStripes(Canvas, S, CardX + 3 * K, CardY + 3 * K, CardW - 6 * K, 16 * K, FTSkin::Chalk, Fade, .5f);
		Flat(CardX + 3 * K, CardY + 19 * K, CardW - 6 * K, 3 * K, Ink);
		AccentSweep(Rect(CardX + 12 * S, CardY + CardH - 8 * S, CardW - 24 * S, 4 * S), Entry, Pink, MenuEntrance);
		RacePrint(TEXT("ROADBURN PICTURES"), Mid, CardY + 50 * K, Mix(Pink, Ink, .25f), 16.f, 1, true);
		RacePrint(TEXT("THE HIGHLIGHTS"), Mid, CardY + 112 * K, Ink, 61.f, 1, true, 945 * K);
		Flat(CardX + 180 * K, CardY + 162 * K, CardW - 360 * K, 3 * K, Ink);
		RacePrint(Level.Zone + TEXT("  /  ") + Level.Title, Mid, CardY + 196 * K, Ink, 22.f, 1, false, 945 * K);
	}
	{
		// the reel's name on the top bar
		FBlock Slug(*this, In01(Entry, 0.f, DealTime), -34 * S, 0.f);
		RaceIcon(IcoArrow, X - 8 * K, Bar * .5f - 20 * K, 40 * K, Pink, static_cast<int32>(EIcon::Play));
		RacePrint(TEXT("DAILIES / HIGHLIGHTS"), X + 40 * K, Bar * .5f, White, 24.f, 0, true);
	}
	// each shot is slugged as it cuts in: the counter flashes, the label slides in from the left
	const float Spliced = G->bReelDone || G->ReelTitleT > 0.f ? 0.f : Flash01(ShotAge, .3f);
	RacePrint(FString::Printf(TEXT("SHOT %02d / %02d  %s"), G->ReelShot, G->ReelShots, *G->ReelCamName),
		R, Bar * .5f, Mix(Grey, White, Spliced), 19.f, 2, false, 477 * K);
	{
		FBlock Label(*this, G->bReelDone ? 1.f : In01(ShotAge, 0.f, .18f), -18 * S, 0.f);
		RacePrint(G->ReelLabel, X, H - Bar * .5f, Yellow, 24.f, 0, true, 498 * K);
	}
	if (G->bReelSlow != bLastReelSlow) { bLastReelSlow = G->bReelSlow; ReelSlowAt = T; }
	if (G->bReelSlow)
	{
		FBlock Slow(*this, In01(T - ReelSlowAt, 0.f, .15f), 14 * S, 0.f);
		RacePrint(TEXT("SLOW MOTION"), R, H - Bar * .5f, Pink, 19.f, 2, true);
	}
	if (G->RealClockNow() - G->ReelLoopT < 2.f)
	{
		// the reel starts over: said on a chalk chip under the top bar
		const float AgainW = RaceTextW(TEXT("AGAIN, FROM THE TOP"), 19.f, true) + 44 * K;
		RaceChip(Mid - AgainW * .5f, Bar + 12 * S, AgainW, 40 * K, White);
		RacePrint(TEXT("AGAIN, FROM THE TOP"), Mid, Bar + 12 * S + 20 * K, Ink, 19.f, 1, true);
	}
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
		Stripes(X, Y - 17 * S, 190 * S, 6 * S, White, Ink);   // the clapper's stripes under the title
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
			// The rating is a rubber stamp. With the kit it is the baked stamp of that rating (its name, its stars, its worn
			// edge), a little askew, coming down as it always did. Without it: the name in the display face, upright, and
			// once it has landed a double rule of its own ink round it. (TODO kit: the film can beside it.)
			const FLinearColor StampInk = InkFor(Color);
			if (FTSkin::Texture(FTSkin::ETex::StampRating))
				FTSkin::Stamp(Canvas, S, RatingX + 199 * S, StampY + NameSize.Y * .5f, Rating, -4.f - 6.f * (1.f - Down), .8f * (1.f + .9f * (1.f - Down)), Fade * (.25f + .75f * Down));
			else
			{
				{
					FPivot Stamp(*this, FVector2D(RatingX + FMath::Min<float>(NameSize.X, 398 * S) * .5f, StampY + NameSize.Y * .5f),
						1.f + .9f * (1.f - Down), -5.f * (1.f - Down));
					Print(Names[Rating], RatingX, StampY, Alpha(Mix(Color, White, Landed), .25f + .75f * Down), Heading, 0, true, 398 * S);
				}
				if (Down >= 1.f)
				{
					Frame(RatingX - 10 * S, StampY - 8 * S, 418 * S, NameSize.Y + 16 * S, Alpha(StampInk, .9f), 3.f);
					Frame(RatingX - 5 * S, StampY - 3 * S, 408 * S, NameSize.Y + 6 * S, Alpha(StampInk, .7f), 1.f);
				}
			}
			if (Landed > 0.f) Flat(RatingX - 10 * S, StampY - 8 * S, 418 * S, NameSize.Y + 16 * S, Alpha(Color, .3f * Landed));
			if (FTSkin::Texture(FTSkin::ETex::DailiesPip)) FTSkin::DailiesPips(Canvas, S, RatingX, Y + 175 * S, FMath::RoundToInt((Rating + 1) * After), 4, Fade, .8f);   // the four pips, as stars
			else Meter(Rect(RatingX, Y + 182 * S, 397 * S, 12 * S), (Rating + 1) / 4.f * After, StampInk, 4);   // the four pips
			Flat(RatingX, Y + 209 * S, 396 * S * After, 1.34f * S, StampInk);
			FBlock Quip(*this, bReducedMotion ? 1.f : EaseOut(Hit - .12f, .25f));
			Print(G->RatingQuip, RatingX, Y + 233 * S, White, Strong, 0, false, 398 * S);
			// (the kit) the take's can in the card's free corner: under the lowest line the card can carry, at every rating
			if (FTSkin::Texture(FTSkin::ETex::FilmCan)) FTSkin::TexNat(Canvas, S, FTSkin::ETex::FilmCan, R - 118 * S, Y + 362 * S, FTSkin::White, Fade, .68f);
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
		FTSkin::Marquee(Canvas, S, Continue.Min.X, Continue.Min.Y, Continue.GetSize().X, Continue.GetSize().Y, T, bReducedMotion, Fade);   // the hero action stands in lights
		G->ResultsContinueRect = Control(Continue, TEXT("CONTINUE TO PAYCHECK"), ConfirmHint(this),
			ControlState(Continue, true, G->PhaseT > 2.9f), Cyan);
		FocusAccent(Continue, bReducedMotion ? -1.f : Time - 2.9f, Cyan);   // armed: its light runs once
		HintRow(InputHint(this, TEXT("R"), EKeys::Gamepad_FaceButton_Top) + TEXT("   RESHOOT     ") + InputHint(this, TEXT("B"), EKeys::Gamepad_FaceButton_Right) + TEXT("   TITLE     ")
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
		const EControl TabState = ControlState(TabRect, bCurrent);
		if (FTSkin::Texture(FTSkin::ETex::Plaque))
		{
			// (the kit) a department's tab is a door plaque in the tab's own rectangle: slate when idle, lit under the mouse,
			// paper with its hazard stripe for the department that is open; the name stands where the button put it
			const bool bUnderMouse = TabState == EControl::Hover || TabState == EControl::Pressed;
			FTSkin::Plaque(Canvas, TabRect.Min.X, TabRect.Min.Y, TabRect.GetSize().X, TabRect.GetSize().Y,
				bCurrent ? FTSkin::EPlaque::Active : bUnderMouse ? FTSkin::EPlaque::Focus : FTSkin::EPlaque::Idle, Fade);
			MarkTone(TabRect, bCurrent ? ETone::Light : ETone::Dark);
			Print(Tabs[I], TabRect.GetCenter().X, TabRect.Min.Y + TabRect.GetSize().Y * .24f, White, Strong, 1, true, TabRect.GetSize().X - 24 * S);
			G->TabRects[I] = TabRect;
		}
		else
		{
			G->TabRects[I] = Control(TabRect, Tabs[I], FString(), TabState, Cyan);
			if (bCurrent) FocusAccent(TabRect, DepartmentAge, Cyan);
		}
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
			// (the kit's stock: paper with its grain for a card, clapper-slate under a hatch for a blank; flat boxes without it)
			const FLinearColor CardNow = Interaction == EControl::Pressed ? Mix(Card, Ink, .25f) : Card;
			if (Lum(Card) > .35f) FTSkin::PaperPanel(Canvas, S, Tx, Ty, Size.X, Size.Y, CardNow, Fade, 0.f, FTSkin::Ink, 2.f);
			else
			{
				FTSkin::SlatePanel(Canvas, S, Tx, Ty, Size.X, Size.Y, Fade, 0.f, CardNow, false, FTSkin::Ink, 2.f);
				if (FTSkin::Texture(FTSkin::ETex::Hatch)) FTSkin::Hatch(Canvas, S, Tx + 2 * S, Ty + 2 * S, Size.X - 4 * S, Size.Y - 4 * S, Fade * .3f);
			}
			MarkTone(TileRect, Lum(Card) > .35f ? ETone::Light : ETone::Dark);
			Flat(Tx, Ty, (bSelected ? 4 : 2) * S, Size.Y, bSelected ? Pink : State);
			if (bSelected) Cursor(TileRect);
			else if (bMouse) Frame(Tx, Ty, Size.X, Size.Y, Pink, 1.5f);
			if (bOwnedCard)
			{
				const float BadgeX = TileRect.Max.X - (Tile.bSwatch ? 31 : 17) * S, BadgeY = Ty + 5 * S;
				if (FTSkin::Texture(FTSkin::ETex::Icons))
				{
					FTSkin::IconTex(Canvas, FTSkin::EIco::BadgeCheck, BadgeX - 3 * S, BadgeY - 3 * S, 18 * S, FTSkin::White, Fade);
					// the APPROVED stamp, small and askew in the card's lower right, off the word OWNED
					FTSkin::Stamp(Canvas, S, TileRect.Max.X - 34 * S, TileRect.Max.Y - 14 * S, FTSkin::StampApproved, -7.f, .42f, Fade * .9f);
				}
				else
				{
					Flat(BadgeX, BadgeY, 12 * S, 12 * S, Ink);
					Stroke(FVector2D(BadgeX + 2.5f * S, BadgeY + 6 * S), FVector2D(BadgeX + 5 * S, BadgeY + 9 * S), Green, 2.f);
					Stroke(FVector2D(BadgeX + 5 * S, BadgeY + 9 * S), FVector2D(BadgeX + 9.5f * S, BadgeY + 3 * S), Green, 2.f);
				}
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
		if (FTSkin::Texture(FTSkin::ETex::ClipboardClip))
		{
			const FVector2D ClipSize = FTSkin::TexSize(FTSkin::ETex::ClipboardClip) * FTSkin::KitPx(S) * .7f;
			FTSkin::TexNat(Canvas, S, FTSkin::ETex::ClipboardClip, DetailX + (R - DetailX) * .5f - ClipSize.X * .5f, BoardY - 32 * S - ClipSize.Y * .6f, FTSkin::White, Fade, .7f);
		}
		else
		{
			Flat(DetailX + (R - DetailX) * .5f - 36 * S, BoardY - 40 * S, 72 * S, 16 * S, Ink);
			Flat(DetailX + (R - DetailX) * .5f - 33 * S, BoardY - 37 * S, 66 * S, 10 * S, Steel);
		}
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
				// (the kit) the branch's picture in the skill detail, in the room the text leaves above the button: never over a line
				if (bSkills && ActionY - Dy > 150 * S && FTSkin::Texture(FTSkin::ETex::BranchBurst))
					FTSkin::BranchArt(Canvas, S, Dx + Dw * .5f - 53.3f * S, Dy + 28 * S + (ActionY - Dy - 135 * S) * .5f, FMath::Clamp(Tile.Col, 0, 4), Fade, .5f);
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
	FTSkin::Marquee(Canvas, S, Ready.Min.X, Ready.Min.Y, Ready.GetSize().X, Ready.GetSize().Y, T, bReducedMotion, Fade);   // the hero action stands in lights
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
	HintRow(TEXT("ARROWS / D-PAD   MOVE     ") + ConfirmHint(this) + TEXT("   CONFIRM     ") + InputHint(this, TEXT("Q / E"), EKeys::Gamepad_LeftShoulder) + TEXT(" / ") + InputHint(this, TEXT("TAB"), EKeys::Gamepad_RightShoulder) + TEXT("   CHANGE TAB"),
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
		Stripes(X, Y + 72 * S, 190 * S, 6 * S, White, Ink);   // the clapper's stripes under the title
		// (the kit) the chase runs along the header band, in the empty stretch between the title and the shoot's name
		if (FTSkin::Texture(FTSkin::ETex::Chase))
			FTSkin::ChaseStrip(Canvas, S, X + 250 * S, Y + 30 * S, 520 * S, 44 * S, bReducedMotion ? .62f : .15f + .7f * FMath::Fmod(T * .03f, 1.f), Fade);
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
			if (FTSkin::Texture(FTSkin::ETex::Signature)) FTSkin::Signature(Canvas, S, Cheque.Min.X + 196 * S, Cheque.Min.Y + 83 * S, FTSkin::Ink, Fade, .42f);   // the director signs it
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
	FTSkin::Marquee(Canvas, S, Continue.Min.X, Continue.Min.Y, Continue.GetSize().X, Continue.GetSize().Y, T, bReducedMotion, Fade);   // the hero action stands in lights
	G->ResultsContinueRect = Control(Continue, TEXT("CONTINUE"), ConfirmHint(this),
		ControlState(Continue, true, G->PhaseT >= .6f), Cyan);
	FocusAccent(Continue, bReducedMotion ? -1.f : Time - .6f, Cyan);   // armed: its light runs once
	HintRow(InputHint(this, TEXT("R"), EKeys::Gamepad_FaceButton_Top) + TEXT("   RESHOOT     ") + InputHint(this, TEXT("B"), EKeys::Gamepad_FaceButton_Right) + TEXT("   TITLE     ")
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
	Stripes(X + 24 * S, Y + 58 * S, 158 * S, 5 * S, White, Pink);   // the clapper's stripes under the heading
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
	HintRow(G->bPendingSystemSave ? G->SystemMessage : bDisconnected ? TEXT("Reconnect your controller, or resume with the keyboard.")
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
	HintRow(TEXT("LEFT-RIGHT   CHOOSE     ") + ConfirmHint(this) + TEXT("   SELECT     ") + BackHint(this) + TEXT("   CANCEL"),
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
	HintRow(TEXT("UP-DOWN   ACTION     LEFT-RIGHT   LAYOUT     ") + ConfirmHint(this) + TEXT("   REBIND"), X + 25 * S, Y + 522 * S, White, Body);
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
	Stripes(X + 24 * S, Y + 56 * S, 158 * S, 5 * S, White, Pink);   // the clapper's stripes under the heading
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
	HintRow(FString::Printf(TEXT("%s  SELECT     %s  BACK     UP / DOWN  NAVIGATE     LEFT / RIGHT  BUTTON STYLE"), *ConfirmHint(this), *BackHint(this)),
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
	Stripes(X + 25 * S, Y + 59 * S, 158 * S, 5 * S, White, Pink);   // the clapper's stripes under the heading
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
	HintRow(TEXT("UP-DOWN / D-PAD   PICK     LEFT-RIGHT   CHANGE"), X + 24 * S, Y + 561 * S, White, Body);
}
