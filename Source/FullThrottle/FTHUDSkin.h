// FULL THROTTLE - the UI skin, "THE STUDIO LOT, INKED" (the ruling: STYLE_RULING.md; the manifest: KIT.md).
// The kit's palette, its 56 textures (/Game/UI/Kit/T_UI_*: made by Tools/ui/ft_ui_kit.py, imported by
// Content/Python/ft_ui_kit_import.py) and the drawing helpers every restyled screen calls. Free functions on a
// UCanvas*, so any Draw function of AFTHUD can call them:
//     FTSkin::PaperPanel(Canvas, S, X, Y, Wd, Ht, FTSkin::Paper, Fade);
//
// UNITS
//   X, Y, W, H and every position are SCREEN pixels, as everywhere in AFTHUD (design px * S).
//   Kit px = texture px = 1080p px. One kit px is S / 1.5 screen px (KitPx(S)). A parameter whose name ends in "Kit"
//   is in kit px and is multiplied by S / 1.5 inside; a helper that needs that factor takes S.
//   A = opacity, the last of the common parameters: PASS THE HUD'S Fade (nothing in here can see it).
//   Degrees turn CLOCKWISE on screen (the engine's yaw); the "angle" of ft_ui_kit.py turns the other way.
//   Rectangles are snapped to whole pixels (the way a flat box rasterises), so ink lines stay crisp at 1080p.
//   The canvas's transform applies as usual: FBlock and FPivot wrap these calls exactly as they wrap Box and Print.
// TINT
//   A "mask" texture is white art: Tint is its colour; its baked ink and shadow stay dark under any tint.
//   A "colour" texture is drawn with White. The colours here are linear (made from the kit's sRGB hex values),
//   which is what a canvas item wants.
// NAMES
//   Never write "using namespace FTSkin" in a file that has "using namespace FTHUDStyle": both have Ink, Steel,
//   Cyan, Pink, Amber, Red, Green and White. Qualify: FTSkin::Ink.
// TEXT
//   Nothing here draws live text: Print stays AFTHUD's. Where a helper leaves room for text its comment says where.
// A missing texture draws nothing and is logged once (LogFT, "FT SKIN missing texture"); nothing here crashes on one.
// The paper and slate panels, the memo, the card, the balloon, the ink plate, the key cap and the wall fall back
// to flat boxes, so a screen stays readable before the kit is imported.
#pragma once

#include "CoreMinimal.h"
#include "Math/Box2D.h"

class UCanvas;
class UFont;
class UTexture2D;

namespace FTSkin
{
	// ------------------------------------------------------------------------------------------------ units
	/** Screen px per kit px (texture px, 1080p px): KIT.md's K = S / 1.5. */
	inline float KitPx(float S) { return S / 1.5f; }

	// ------------------------------------------------------------------------------------------------ palette (KIT.md)
	/** A linear colour from an sRGB hex value 0xRRGGBB (the exact sRGB curve, as the GPU decodes the textures). */
	inline FLinearColor Hex(uint32 Value, float Opacity = 1.f)
	{
		auto Lin = [](uint32 V) -> float { const float C = float(V & 0xFF) / 255.f; return C <= 0.04045f ? C / 12.92f : FMath::Pow((C + 0.055f) / 1.055f, 2.4f); };
		return FLinearColor(Lin(Value >> 16), Lin(Value >> 8), Lin(Value), Opacity);
	}
	inline const FLinearColor Ink = Hex(0x16121E);          // every outline, every hard shadow, text on paper
	inline const FLinearColor Slate = Hex(0x1B1824);        // slate panels, the HUD sticker's body
	inline const FLinearColor Slate2 = Hex(0x2C2842);       // idle buttons and rows, idle tab, raised dark surface
	inline const FLinearColor Steel = Hex(0x423D5E);        // locked, meter track, disabled, dark key cap
	inline const FLinearColor Mute = Hex(0xA09CBA);         // secondary text on dark
	inline const FLinearColor Chalk = Hex(0xF8F3E7);        // text on dark, the HUD keyline, balloon and memo stock
	inline const FLinearColor Paper = Hex(0xF4E8CE);        // call sheet stock (owned tile, panels read at length)
	inline const FLinearColor PaperShade = Hex(0xDFCEAA);   // rules, folds and screws on paper
	inline const FLinearColor Manila = Hex(0xE9C682);       // clipboard, memo head
	inline const FLinearColor Aqua = Hex(0xCBF0E9);         // "available" stock, the options' focus row
	inline const FLinearColor Sticky = Hex(0xFFE078);       // "pending" stock
	inline const FLinearColor Ledger = Hex(0xE8F2DE);       // ledger paper
	inline const FLinearColor LedgerBar = Hex(0xCDE4C4);    // the ledger's green bar rows
	inline const FLinearColor Red = Hex(0xEE3C3E);          // danger, the boss, armour low, CUT!, margin rules
	inline const FLinearColor Pink = Hex(0xFF4FA3);         // focus, the cursor, combo, finale, grease pencil
	inline const FLinearColor Cyan = Hex(0x49E2D5);         // the player's systems
	inline const FLinearColor Teal = Hex(0x067876);         // cyan printed on paper
	inline const FLinearColor Amber = Hex(0xFFC44C);        // money, score, bulbs, ordnance
	inline const FLinearColor AmberInk = Hex(0xA05C00);     // amber printed on paper
	inline const FLinearColor Green = Hex(0x76EAA0);        // armour nominal, owned
	inline const FLinearColor GreenInk = Hex(0x148450);     // APPROVED, green printed on paper
	inline const FLinearColor White = Hex(0xFFFFFF);        // untinted
	inline const FLinearColor CycTop = Hex(0xB8A8DE);       // cyclorama, top (dusty lilac)
	inline const FLinearColor CycMid = Hex(0xE2B8D6);       // cyclorama, middle
	inline const FLinearColor CycLow = Hex(0xFFD0B0);       // cyclorama, foot of the wall (peach)
	inline const FLinearColor CycFloor = Hex(0xF7BEA0);     // cyclorama floor
	inline const FLinearColor CycShade = Hex(0x8E7CC0);     // the side dressing's flats and stands
	inline const FLinearColor Rate0 = Hex(0xEE3C3E);        // stamp ink, B-MOVIE
	inline const FLinearColor Rate1 = Hex(0x2454C4);        // stamp ink, ACTION HERO
	inline const FLinearColor Rate2 = Hex(0xD45C00);        // stamp ink, BLOCKBUSTER
	inline const FLinearColor Rate3 = Hex(0xC01C78);        // stamp ink, LEGENDARY DIRECTOR
	inline const FLinearColor BranchDem = Hex(0xFF6B4A);    // skill branch DEMOLITION
	inline const FLinearColor BranchWhl = Hex(0x49E2D5);    // skill branch WHEELMAN
	inline const FLinearColor BranchStn = Hex(0xFF4FA3);    // skill branch STUNT DOUBLE
	inline const FLinearColor BranchCar = Hex(0xFFC44C);    // skill branch STAR CAR
	inline const FLinearColor BranchGet = Hex(0x76EAA0);    // skill branch GETAWAY
	inline const FLinearColor Shoot1 = Hex(0xFF808E);       // one accent per shoot: the lockup's second ink, the row's tab
	inline const FLinearColor Shoot2 = Hex(0x96E260);
	inline const FLinearColor Shoot3 = Hex(0xFFA854);
	inline const FLinearColor Shoot4 = Hex(0xF2543C);
	inline const FLinearColor Shoot5 = Hex(0xFF6EC4);
	inline const FLinearColor Shoot6 = Hex(0x8CD0FF);
	inline const FLinearColor Shoot7 = Hex(0x40CCCC);
	inline const FLinearColor Shoot8 = Hex(0xA88AFF);
	// not in KIT.md's table, used by its recipes:
	inline const FLinearColor LitPaper = Hex(0xFFF6E0);     // the focused button's lit paper
	inline const FLinearColor MarqueeBand = Hex(0x601828);  // the painted band under the marquee's bulbs
	inline const FLinearColor FilmBase = Hex(0x0C0A11);     // film base (film meter, film strip)
	/** Opacity of a hard shadow that lies on the road (the HUD's stickers); in the menus a shadow is Ink at 1. */
	constexpr float HudShadow = 0.62f;

	/** The accent of shoot 1..8 (clamped). */
	FLinearColor ShootColor(int32 Shoot1To8);
	/** The colour of skill branch 0..4: DEMOLITION, WHEELMAN, STUNT DOUBLE, STAR CAR, GETAWAY (clamped). */
	FLinearColor BranchColor(int32 Branch0To4);
	/** The stamp ink of rating 0..3: B-MOVIE, ACTION HERO, BLOCKBUSTER, LEGENDARY DIRECTOR (clamped). */
	FLinearColor RateColor(int32 Rating0To3);
	/** A blend of two colours (T = 0 gives A, 1 gives B). */
	inline FLinearColor Mix(const FLinearColor& A, const FLinearColor& B, float T) { return A + (B - A) * T; }
	/** The colour with its opacity multiplied. */
	inline FLinearColor WithA(FLinearColor Color, float Opacity) { Color.A *= Opacity; return Color; }

	// ------------------------------------------------------------------------------------------------ type (KIT.md)
	// Heights for AFTHUD::Print, in DESIGN px (Print's own unit). Floor for game information: Label (16).
	namespace Txt
	{
		constexpr float Title = 44.f;       // Bungee: screen titles
		constexpr float Hero = 30.f;        // Bungee: ROLL CAMERA, CONTINUE, READY
		constexpr float Button = 24.f;      // Bungee: actions, shoot names, a tile's title in the detail
		constexpr float Plaque = 19.f;      // Bungee: tabs, column heads, HUD labels
		constexpr float Chip = 16.f;        // Bungee: chips, small HUD labels
		constexpr float BodyLarge = 22.f;   // Rajdhani: the director's line, detail text, the verdict
		constexpr float Body = 19.f;        // Rajdhani: rows, tile names, descriptions, values
		constexpr float Label = 16.f;       // Rajdhani: sublabels, hints, key caps (THE FLOOR for game information)
		constexpr float SmallPrint = 11.f;  // Rajdhani: decoration only, never game information
		// capital heights of the atlas numerals in DESIGN px: pass (value * S) to Digits as CapH
		constexpr float NumeralXL = 43.f;   // speed, totals
		constexpr float NumeralL = 31.f;    // box office
		constexpr float NumeralM = 24.f;    // missile count, level
		constexpr float NumeralS = 18.f;    // percent, small counts
		// to place a line by its capitals: cap height = Cap * height, capitals start CapTop * height under the line box's top
		constexpr float BungeeCap = 0.545f, BungeeCapTop = 0.227f;
		constexpr float RajdhaniCap = 0.504f, RajdhaniCapTop = 0.225f;
	}

	// ------------------------------------------------------------------------------------------------ textures
	/** Every texture of the kit, in KIT.md's order: ETex::Paper is /Game/UI/Kit/T_UI_Paper. */
	enum class ETex : uint8
	{
		CycWall, CycPool, CycSide, Dots,                                                    // the wall
		Paper, SlateDust, ClapStripes, HazardStripes, Hatch, FilmPerf,                      // stock and pattern (they tile)
		Tape, Paperclip, Pin, ClipboardClip, ClapStick, FilmCan, Cheque, Signature, Highlighter, Grease,   // paper props
		Bulb, StampApproved, StampRating, Icons, IconsBig, BranchBurst,
		Boards, BoardThumbs, Lockups,                                                       // shoots
		Plaque, Keycap, Cursor, SheetFrame, Memo, DailiesCard, DailiesPip,
		StickerCash, StickerSP, StickerLevel, KestrelHero, HeroBurst, Chase, ChaseSkyline,
		HudPlate, HudBar, HudChip, HudTag, HudPips, HudBracket, HudComboFill, HudComboInk, HudArmorLow, HudBalloon, HudMegaphone,
		DigitsFill, DigitsInk,
		Count
	};

	/** The icons of T_UI_Icons in atlas order (the value is the cell). BadgeCheck and BadgeLock are full colour. */
	enum class EIco : uint8
	{
		Clapper, Megaphone, Camera, Reel, Rec, Ticket, Cash, Star,
		Shield, Bolt, Missile, Bullets, Wrench, Spray, Fire, Burst,
		People, Wheel, Helmet, Flag, Padlock, Warning, Skull, Check,
		Gear, Pad, Exit, Arrow, Clock, Undo, Disk, Crosshair,
		Shoot1, Shoot2, Shoot3, Shoot4, Shoot5, Shoot6, Shoot7, Shoot8,   // palm, flask, cactus, volcano, ferris wheel, mountain, fish, tower
		BadgeCheck, BadgeLock, Speaker, Trophy, Car,
		Count
	};

	/** Cells of T_UI_Hud_Pips. */
	enum class EPip : uint8 { ArrowFull, ArrowEmpty, ReelFull, ReelEmpty, SegmentFull, SegmentFlat, FinaleMark, Tick };
	/** Cells of T_UI_Grease (grease pencil marks). */
	enum class EGrease : uint8 { Ring, Underline, Cross, Arrow };
	/** Cells of T_UI_Chase (side view, nose to the right). */
	enum class EChase : uint8 { Kestrel, Truck, Sedan, Gunship };
	/** Cells of T_UI_Plaque (a tab as a door plaque). */
	enum class EPlaque : uint8 { Idle, Focus, Active };
	/** The live-text zones of T_UI_Cheque. */
	enum class EChequeZone : uint8 { Stub, Payee, Amount, Words, Memo, Signature };
	/** Stamp's "Which" for the APPROVED stamp; 0..3 are the four rating stamps. */
	constexpr int32 StampApproved = -1;

	/** The texture, loaded once and kept alive (null when the asset is missing: logged once, never retried). */
	UTexture2D* Texture(ETex T);
	/** Loads all 56 textures and the Bungee font now (call once, e.g. where the HUD loads its fonts), so nothing loads mid-race. Returns how many textures were found. */
	int32 Preload();
	/** Forgets what was loaded or found missing, so the next draw looks again (after an import in a running editor). */
	void Reload();
	/** Size of a texture in texture px (from KIT.md, not from the asset). */
	FVector2D TexSize(ETex T);
	/** Size of one atlas cell in texture px (the whole texture when it is not an atlas). */
	FVector2D CellSize(ETex T);
	/** /Game/UI/Fonts/F_Bungee, loaded once and kept alive (null until the kit's importer has run). */
	UFont* Bungee();

	// ------------------------------------------------------------------------------------------------ primitives
	/** A flat rectangle (the white texture), edges snapped to whole pixels. */
	void Box(UCanvas* C, float X, float Y, float W, float H, const FLinearColor& Color, float A = 1.f);
	/** An outline INSIDE the rectangle, Th screen px thick: four flat boxes. */
	void Frame(UCanvas* C, float X, float Y, float W, float H, const FLinearColor& Color, float Th, float A = 1.f);
	/** A flat parallelogram inscribed in the rectangle, leaning right by Lean screen px at the top: two triangles (the white texture). */
	void Para(UCanvas* C, float X, float Y, float W, float H, float Lean, const FLinearColor& Color, float A = 1.f);

	/** One tile: the whole texture T stretched to the rectangle, tinted; mirrored by flipping its UVs. */
	void Tex(UCanvas* C, ETex T, float X, float Y, float W, float H, const FLinearColor& Tint = White, float A = 1.f, bool bFlipX = false, bool bFlipY = false);
	/** One tile at the texture's own size (texture px * S / 1.5 * Scale) with its top left at X, Y. Returns the size drawn. */
	FVector2D TexNat(UCanvas* C, float S, ETex T, float X, float Y, const FLinearColor& Tint = White, float A = 1.f, float Scale = 1.f, bool bFlipX = false, bool bFlipY = false);
	/** One cell of an atlas texture T (cell = column + row * columns) stretched to the rectangle. */
	void TexCell(UCanvas* C, ETex T, int32 Cell, float X, float Y, float W, float H, const FLinearColor& Tint = White, float A = 1.f, bool bFlipX = false);
	/** One cell of an atlas at its own size (cell px * S / 1.5 * Scale), top left at X, Y. Returns the size drawn. */
	FVector2D TexCellNat(UCanvas* C, float S, ETex T, int32 Cell, float X, float Y, const FLinearColor& Tint = White, float A = 1.f, float Scale = 1.f);
	/** T repeated by UV over the rectangle, never stretched: one tile is texture px * S / 1.5 * Scale. An axis that is not tiled runs 0..1 over the rectangle. OffX, OffY (screen px) shift the pattern: pass the rectangle's own X, Y to anchor the pattern to the screen. For the WRAP textures: Dots, Paper, SlateDust, ClapStripes, HazardStripes, Hatch, FilmPerf (x and y), ChaseSkyline (x only). */
	void TexTiled(UCanvas* C, float S, ETex T, float X, float Y, float W, float H, const FLinearColor& Tint = White, float A = 1.f, float Scale = 1.f, bool bTileX = true, bool bTileY = true, float OffX = 0.f, float OffY = 0.f);
	/** The rectangle of T (or of its cell Cell) turned Degrees clockwise about the point Pivot01 of the rectangle (0..1; 0.5, 0.5 is its centre). Not snapped. */
	void TexRot(UCanvas* C, ETex T, float X, float Y, float W, float H, float Degrees, const FVector2D& Pivot01 = FVector2D(0.5f, 0.5f), const FLinearColor& Tint = White, float A = 1.f, int32 Cell = -1);
	/** T (or its cell) at its own size * Scale, placed so that its pivot of KIT.md (its centre when it has none) lands on PX, PY, and turned Degrees clockwise about it. Tape, ClapStick, the stamps, the combo burst, ARMOR LOW. Returns the size drawn. */
	FVector2D TexPivot(UCanvas* C, float S, ETex T, float PX, float PY, float Degrees = 0.f, float Scale = 1.f, const FLinearColor& Tint = White, float A = 1.f, int32 Cell = -1);
	/** Nine tiles of T (or of its cell) with KIT.md's margins: corners drawn at margin * S / 1.5 and never stretched, edges stretched along their length, the centre stretched or (bCentre false) left open. The rectangle is the WHOLE texture, baked shadow included. Cursor, SheetFrame, Memo, DailiesCard, Keycap, HudBalloon. */
	void NineSlice(UCanvas* C, float S, ETex T, float X, float Y, float W, float H, const FLinearColor& Tint = White, float A = 1.f, bool bCentre = true, int32 Cell = -1);
	/** Three tiles of T (or of its cell) side by side: the texture is scaled UNIFORMLY to the height H, the two caps keep that scale, the middle stretches. Highlighter, Plaque, HudPlate, HudBar, HudChip, HudTag. */
	void ThreeSliceH(UCanvas* C, ETex T, float X, float Y, float W, float H, const FLinearColor& Tint = White, float A = 1.f, int32 Cell = -1);

	// ------------------------------------------------------------------------------------------------ the wall
	/** The whole viewport as lit cyclorama: T_UI_Cyc_Wall stretched over the true screen (a gradient of CycTop, CycMid, CycLow, CycFloor when the texture is missing), two halftone light pools (T_UI_Cyc_Pool) in the centre 16:9 block, and on a wider screen the wall runs on and T_UI_Cyc_Side stands outside the block, mirrored on the right. */
	void Wall(UCanvas* C, float S, float A = 1.f, bool bSideDressing = true, bool bPools = true);
	/** The wall inside a rectangle, its bands where they are on the full wall (the title's left two fifths and action bar over the live view): T_UI_Cyc_Wall, V following the screen. */
	void WallRect(UCanvas* C, float X, float Y, float W, float H, float A = 1.f);
	/** One round halftone light pool of radius R (screen px) centred on CX, CY: T_UI_Cyc_Pool four times, mirrored. */
	void WallPool(UCanvas* C, float CX, float CY, float R, const FLinearColor& Tint = Chalk, float A = 0.5f);

	// ------------------------------------------------------------------------------------------------ panels (cel finish: ink outline, hard offset shadow)
	/** Call-sheet stock: a hard shadow box, an ink box, then T_UI_Paper tiled and tinted with Stock (Paper, Aqua, Sticky, Ledger, Manila, Chalk, LitPaper). The rectangle is the panel; the shadow lies outside it, right and below. ShadowKit 0 = none. */
	void PaperPanel(UCanvas* C, float S, float X, float Y, float W, float H, const FLinearColor& Stock = Paper, float A = 1.f, float ShadowKit = 7.f, const FLinearColor& ShadowColor = Ink, float OutlineKit = 3.f);
	/** Clapper-slate black: a hard shadow, an ink outline, a flat Fill, chalk dust (T_UI_SlateDust tiled, Chalk) and an inner chalk hairline. */
	void SlatePanel(UCanvas* C, float S, float X, float Y, float W, float H, float A = 1.f, float ShadowKit = 7.f, const FLinearColor& Fill = Slate, bool bRim = true, const FLinearColor& ShadowColor = Ink, float OutlineKit = 3.f);
	/** Green-bar ledger: a PaperPanel in Ledger stock (T_UI_Paper), a LedgerBar box on every other row (first row at RowY, pitch RowH, both screen px), two red margin rules and three punch holes. */
	void LedgerPanel(UCanvas* C, float S, float X, float Y, float W, float H, float RowY, float RowH, int32 Rows, float A = 1.f, float MarginKit = 60.f, float ShadowKit = 7.f);
	/** The upright paper memo (T_UI_Memo, nine-slice; MEMO is baked in its head band). The rectangle is the memo's body (at least 124 x 84 kit px); its shadow lies outside. Returns where text starts (20 kit px in, 52 down). */
	FVector2D Memo(UCanvas* C, float S, float X, float Y, float W, float H, float A = 1.f);
	/** THE DAILIES card stock (T_UI_Dailies_Card, nine-slice): paper with a slate head band 30 kit px high that takes a Chalk title. The rectangle is the card's body; its shadow lies outside. Returns where text starts (20 kit px in, 50 down). */
	FVector2D DailiesCard(UCanvas* C, float S, float X, float Y, float W, float H, float A = 1.f);
	/** The storyboard sheet as a HOLLOW taped frame over the live view (T_UI_SheetFrame, nine-slice, centre open; bTape adds T_UI_Tape on the two top corners). The rectangle is the sheet's outer edge; returns the window (30 kit px inside). */
	FBox2D SheetFrame(UCanvas* C, float S, float X, float Y, float W, float H, float A = 1.f, bool bTape = true);
	/** The skill detail's clipboard: a Manila board (PaperPanel), a Paper sheet on it and T_UI_ClipboardClip on its top edge. Returns the sheet's rectangle. */
	FBox2D ClipboardPanel(UCanvas* C, float S, float X, float Y, float W, float H, float A = 1.f);
	/** The HUD's ink sticker in a layout rectangle: Slate body, a keyline in Keyline (Chalk; Red for armour low and the boss), ink edge, hard shadow at 62 %. The whole sticker, shadow included, fits INSIDE the rectangle. bSlantedEnds: T_UI_Hud_Plate (160 kit px high or more) or T_UI_Hud_Bar, three-sliced, with their slanted ends and halftone; false: the same sticker from flat boxes, square ends. Returns the rectangle that is safely inside the slate body. */
	FBox2D InkPlate(UCanvas* C, float S, float X, float Y, float W, float H, const FLinearColor& Keyline = Chalk, float A = 1.f, bool bSlantedEnds = true);

	// ------------------------------------------------------------------------------------------------ stamps, stickers, the balloon
	/** A rubber stamp centred on CX, CY and turned Degrees clockwise: Which = StampApproved (T_UI_Stamp_Approved) or a rating 0..3 (a cell of T_UI_Stamp_Rating). Scale is the pop: pass 1 + 0.3 * Flash01(Age, .14f), or 1. */
	void Stamp(UCanvas* C, float S, float CX, float CY, int32 Which = StampApproved, float Degrees = 8.f, float Scale = 1.f, float A = 1.f);
	/** The yellow price burst (T_UI_Sticker_Cash, 288 x 144 kit px, top left at X, Y) with Amount on it in atlas numerals (fill only, Ink), shrunk to fit. The label is yours: Print "CASH" centred on X + 140 kit px at Y + 26 kit px (Rajdhani, Txt::Label, Ink); bLabelRoom false centres the amount in the burst instead. Returns the size drawn. */
	FVector2D StickerCash(UCanvas* C, float S, float X, float Y, const FString& Amount, float A = 1.f, float Scale = 1.f, bool bLabelRoom = true);
	/** The cyan skill point token (T_UI_Sticker_SP, 128 x 128 kit px, top left at X, Y) with Points in its middle in atlas numerals (Ink). Returns the size drawn. */
	FVector2D StickerSP(UCanvas* C, float S, float X, float Y, int32 Points, float A = 1.f, float Scale = 1.f);
	/** The driver level rating box (T_UI_Sticker_Level, 256 x 96 kit px, top left at X, Y) with Level in its chalk cell in atlas numerals (Ink). The two lines in its slate cell are yours: Print at X + 104 kit px, Y + 14 kit px (Rajdhani, Txt::Label, Chalk) and Y + 40 kit px (Bungee, 22, Chalk, max width 138 kit px). Returns the size drawn. */
	FVector2D StickerLevel(UCanvas* C, float S, float X, float Y, int32 Level, float A = 1.f);
	/** The director's megaphone speech balloon (T_UI_Hud_Balloon nine-sliced; bMegaphone puts the T_UI_Hud_Megaphone badge left of its tail). X, Y is the top left of the whole thing, TextW the measured width of your line; H (screen px, 0 = one line, 64 kit px) is the balloon's height with its baked shadow. Returns the box your text goes in (Ink; centre the line in its height). */
	FBox2D Balloon(UCanvas* C, float S, float X, float Y, float TextW, float A = 1.f, bool bMegaphone = true, float H = 0.f);
	/** The combo starburst centred on CX, CY: T_UI_Hud_Combo_Ink, T_UI_Hud_Combo_Fill tinted Tint, and "x" Mult in atlas numerals. Scale is its size (0.6 to 1) times your pop. */
	void Combo(UCanvas* C, float S, float CX, float CY, int32 Mult, const FLinearColor& Tint = Pink, float A = 1.f, float Scale = 0.8f, const FLinearColor& Numeral = White);
	/** The baked shout ARMOR LOW! (T_UI_Hud_ArmorLow) centred on CX, CY; Scale is its size (0.8) times your pop. Pin it to the driver plate's top right corner. */
	void ArmorLow(UCanvas* C, float S, float CX, float CY, float A = 1.f, float Scale = 0.8f);

	// ------------------------------------------------------------------------------------------------ controls and small parts
	/** The chasing bulb border of the hero action, OUTSIDE the rectangle (draw it before the button): an ink box, a painted band (MarqueeBand) and T_UI_Bulb round it. Two bulbs in three are lit and the pattern steps along by Time (real seconds), StepsPerSecond clamped to 3; bReducedMotion: it stands still. */
	void Marquee(UCanvas* C, float S, float X, float Y, float W, float H, float Time, bool bReducedMotion, float A = 1.f, float StepsPerSecond = 3.f, float BandKit = 14.f, float PitchKit = 23.f);
	/** The viewfinder's four corner brackets at the TRUE corners of the screen (T_UI_Hud_Bracket, mirrored), InsetKit in from the edges. */
	void Brackets(UCanvas* C, float S, const FLinearColor& Tint = Chalk, float A = 1.f, float InsetKit = 18.f);
	/** A band of clapper stripes: T_UI_ClapStripes tiled (bands of 32 kit px * Scale; use Scale 1 or 0.5), white bands tinted, ink bands stay dark. */
	void ClapStripes(UCanvas* C, float S, float X, float Y, float W, float H, const FLinearColor& Tint = Chalk, float A = 1.f, float Scale = 1.f);
	/** A band of narrow stripes: T_UI_HazardStripes tiled (bands of 16 kit px * Scale). Hazard bars (Red, Amber) and the small clapper stripe of button ends, row ends and title rules (Chalk, Amber, Steel). */
	void HazardStripes(UCanvas* C, float S, float X, float Y, float W, float H, const FLinearColor& Tint = Red, float A = 1.f, float Scale = 1.f);
	/** Thin diagonal ink hatch over a disabled button or a locked tile: T_UI_Hatch tiled. */
	void Hatch(UCanvas* C, float S, float X, float Y, float W, float H, float A = 1.f);
	/** The halftone dot screen over a rectangle: T_UI_Dots tiled, tinted (Chalk, Ink, Pink). */
	void Dots(UCanvas* C, float S, float X, float Y, float W, float H, const FLinearColor& Tint = Chalk, float A = 1.f, float Scale = 1.f);
	/** Width of a key cap H high for a label TextW wide (both screen px). */
	float KeyCapWidth(float TextW, float H);
	/** A key cap chip sized to a measured string: T_UI_Keycap nine-sliced, H high (28 to 44 kit px, its 4 kit px lip included), top left at X, Y. bOnDark: the light cap (Print the label in Ink); false, on paper and on the wall: the dark cap (label in Chalk). The label is yours: centre it on X + width / 2, in the height H minus the lip. Returns the cap's width. */
	float KeyCap(UCanvas* C, float S, float X, float Y, float TextW, float H, bool bOnDark = true, float A = 1.f);
	/** The selection cursor round a rectangle, GapKit outside it: T_UI_Cursor nine-sliced, centre open, tinted (Pink). Its hard shadow runs right and below. */
	void Cursor(UCanvas* C, float S, float X, float Y, float W, float H, const FLinearColor& Tint = Pink, float A = 1.f, float GapKit = 8.f);
	/** A tab as a door plaque: a cell of T_UI_Plaque three-sliced. The rectangle is the plaque's body (66 kit px high in the texture: 44 design); its 6 px shadow lies outside. The Active plaque is open at its foot: overlap the board below by 3 kit px. */
	void Plaque(UCanvas* C, float X, float Y, float W, float H, EPlaque State, float A = 1.f);
	/** A highlighter stroke behind a value on paper: T_UI_Highlighter three-sliced (24 kit px high in the texture), tinted (Amber, Cyan, Pink, Green). */
	void Highlighter(UCanvas* C, float X, float Y, float W, float H, const FLinearColor& Tint = Amber, float A = 1.f);
	/** A grease pencil mark stretched to the rectangle: a cell of T_UI_Grease (192 x 96 kit px), tinted (Pink, Red, Ink). */
	void Grease(UCanvas* C, EGrease Mark, float X, float Y, float W, float H, const FLinearColor& Tint = Pink, float A = 1.f);
	/** A strip of masking tape centred on CX, CY and turned: T_UI_Tape. */
	void Tape(UCanvas* C, float S, float CX, float CY, float Degrees = 0.f, float A = 1.f);
	/** The clapperboard's stick turned about its hinge at HingeX, HingeY (negative Degrees open it upward): T_UI_ClapStick, 704 x 48 kit px * Scale. */
	void ClapStick(UCanvas* C, float S, float HingeX, float HingeY, float Degrees = 0.f, float A = 1.f, float Scale = 1.f);
	/** An icon of T_UI_Icons, its 64 px cell drawn Size x Size (20 to 64 screen px) from X, Y, tinted; BadgeCheck and BadgeLock ignore the tint. */
	void IconTex(UCanvas* C, EIco Icon, float X, float Y, float Size, const FLinearColor& Tint = Ink, float A = 1.f, bool bFlipX = false);
	/** The skill detail's picture, 320 x 320 kit px * Scale from X, Y: T_UI_BranchBurst tinted with the branch's colour and the branch's big icon (a cell of T_UI_IconsBig) on it in Ink over a Chalk offset. Branch 0..4. */
	void BranchArt(UCanvas* C, float S, float X, float Y, int32 Branch, float A = 1.f, float Scale = 1.f);
	/** The rating as star pips, Lit of Total in a row from X, Y: cells of T_UI_Dailies_Pip (48 kit px * Scale, pitch 46 kit px * Scale). Returns the row's width. */
	float DailiesPips(UCanvas* C, float S, float X, float Y, int32 Lit, int32 Total = 4, float A = 1.f, float Scale = 0.8f);

	// ------------------------------------------------------------------------------------------------ meters and pips
	/** The missile count as arrow pips in a row from X, Y: cells 0 and 1 of T_UI_Hud_Pips (32 kit px, pitch PitchKit), full ones tinted Tint, empties Empty. Returns the row's width. */
	float ArrowPips(UCanvas* C, float S, float X, float Y, int32 Have, int32 Total, const FLinearColor& Tint = Amber, const FLinearColor& Empty = Steel, float A = 1.f, float PitchKit = 30.f);
	/** The film left as reel pips: cells 2 and 3 of T_UI_Hud_Pips (32 kit px, pitch PitchKit). Returns the row's width. */
	float ReelPips(UCanvas* C, float S, float X, float Y, int32 Have, int32 Total, const FLinearColor& Tint = Amber, const FLinearColor& Empty = Steel, float A = 1.f, float PitchKit = 32.f);
	/** One cell of T_UI_Hud_Pips stretched to the rectangle (the finale mark, a tick, a segment). */
	void Pip(UCanvas* C, EPip Cell, float X, float Y, float W, float H, const FLinearColor& Tint = Amber, float A = 1.f);
	/** A 35 mm strip meter (XP, scene progress): FilmBase, a row of T_UI_FilmPerf perforations along the top and the foot, and one flat frame per segment between them (Empty, filled ones Tint). */
	void FilmMeter(UCanvas* C, float S, float X, float Y, float W, float H, float Fraction, int32 Segments, const FLinearColor& Tint = Cyan, const FLinearColor& Empty = Steel, float A = 1.f);
	/** A mark on a FilmMeter drawn with the same rectangle (a scene tick, the finale): a flat bar 3 kit px wide at At01 of its length, standing 3 kit px proud. */
	void FilmMeterMark(UCanvas* C, float S, float X, float Y, float W, float H, float At01, const FLinearColor& Color, float A = 1.f);
	/** The HUD's chunky meter: slanted flat segments (lean 0.35 * H) on an ink bed, the top third of a filled segment a lighter tone; Ghost is the white trail of what was just lost (a fraction). No texture. The bed reaches 3 kit px outside the rectangle and 0.35 * H further right. */
	void HudMeter(UCanvas* C, float S, float X, float Y, float W, float H, float Fraction, int32 Segments, const FLinearColor& Tint = Green, const FLinearColor& TrackColor = Steel, float A = 1.f, float Ghost = 0.f);
	/** The plain segmented bar of the menus (today's Meter): flat boxes, gaps of GapKit. No texture. */
	void BlockMeter(UCanvas* C, float S, float X, float Y, float W, float H, float Fraction, int32 Segments, const FLinearColor& Tint = Cyan, const FLinearColor& TrackColor = Steel, float A = 1.f, float GapKit = 4.f);

	// ------------------------------------------------------------------------------------------------ HUD chips
	/** A small slanted chip (plate tabs, TAKE 1, ammo boxes; tinted Slate: the keys strip): T_UI_Hud_Chip three-sliced, white with an ink outline, no shadow, 26 to 48 kit px high. Text starts half the height in. */
	void HudChip(UCanvas* C, float X, float Y, float W, float H, const FLinearColor& Tint = Cyan, float A = 1.f);
	/** A reward tag (+3 MISSILES, NEAR MISS +$250): T_UI_Hud_Tag three-sliced, pointed end with a punched hole on the left, 30 to 48 kit px high; its 4 px shadow is inside the rectangle. Returns where text starts in X (30/44 of the height in); centre the line in the top 40/44 of the height. */
	float HudTag(UCanvas* C, float X, float Y, float W, float H, const FLinearColor& Tint = Amber, float A = 1.f);

	// ------------------------------------------------------------------------------------------------ shoots and hero art
	/** The storyboard frame of shoot 0..7 with its boss ringed and named: a cell of T_UI_Boards (384 x 216) drawn W wide from X, Y inside an ink frame of OutlineKit. Returns the height drawn. */
	float Board(UCanvas* C, float S, int32 Shoot0To7, float X, float Y, float W, float A = 1.f, float OutlineKit = 3.f);
	/** The small clean frame of shoot 0..7 for the shoot list: a cell of T_UI_BoardThumbs (128 x 72) drawn W wide inside an ink frame. Returns the height drawn. */
	float BoardThumb(UCanvas* C, float S, int32 Shoot0To7, float X, float Y, float W, float A = 1.f, float OutlineKit = 2.5f);
	/** The two-ink lockup of shoot 0..7 at its own size (448 x 128 kit px * Scale) from X, Y: a cell of T_UI_Lockups. Returns the size drawn. */
	FVector2D Lockup(UCanvas* C, float S, int32 Shoot0To7, float X, float Y, float A = 1.f, float Scale = 1.f);
	/** The KESTREL die-cut sticker (T_UI_Kestrel_Hero, 768 x 304 kit px * Scale, top left at X, Y), with the explosion sticker (T_UI_Hero_Burst) behind its tail when bBurst. Returns the car's size drawn. */
	FVector2D KestrelHero(UCanvas* C, float S, float X, float Y, float A = 1.f, float Scale = 1.f, bool bBurst = true);
	/** One sprite of the chase (a cell of T_UI_Chase, 320 x 128 kit px * Scale) centred on CX with its wheels on GroundY (the gunship: lift it yourself). */
	void ChaseSprite(UCanvas* C, float S, EChase Who, float CX, float GroundY, float A = 1.f, float Scale = 0.62f);
	/** The masthead band of PRE-PRODUCTION and PAYCHECK in the rectangle: T_UI_Chase_Skyline tiled along x, an ink bar with T_UI_ClapStripes as the rule at its foot, and the chase on the rule: the sedan and the truck behind, the gunship above, the KESTREL at Car01 (0..1 along the band; animate it). Returns the Y of the rule's top. */
	float ChaseStrip(UCanvas* C, float S, float X, float Y, float W, float H, float Car01 = 0.62f, float A = 1.f);
	/** The cheque blank (T_UI_Cheque, 800 x 232 kit px, never rotated), top left at X, Y. Returns the size drawn. */
	FVector2D Cheque(UCanvas* C, float S, float X, float Y, float A = 1.f);
	/** A live-text zone of a cheque drawn at X, Y, in screen px. The signature zone takes T_UI_Signature (see Signature). */
	FBox2D ChequeZone(float S, float X, float Y, EChequeZone Zone);
	/** The director's signature (T_UI_Signature, 256 x 72 kit px * Scale) from X, Y, tinted (Ink, Teal, Pink). */
	void Signature(UCanvas* C, float S, float X, float Y, const FLinearColor& Tint = Ink, float A = 1.f, float Scale = 1.f);

	// ------------------------------------------------------------------------------------------------ big numerals (never live text)
	/** A string of 0-9 $ , . / % x + - : and space from the baked digit atlas (Bungee, leaning, tabular digits): the ink cells of the whole string first (T_UI_Digits_Ink, tinted InkTint), then the fill cells (T_UI_Digits_Fill, tinted Fill). X, Baseline is the pen position (Align 0 left, 1 centre, 2 right), CapH the capitals' height in screen px (above 44 the L atlas, else the S). bInk false draws the fill alone: printed numerals on paper (Fill = Ink). Other characters are skipped. Returns the width. */
	float Digits(UCanvas* C, const FString& Str, float X, float Baseline, float CapH, const FLinearColor& Fill = Chalk, float A = 1.f, int32 Align = 0, bool bInk = true, const FLinearColor& InkTint = White);
	/** The width Digits would draw. */
	float MeasureDigits(const FString& Str, float CapH);
}
