// The UI skin's drawing helpers (see FTHUDSkin.h). Every number about a texture here is KIT.md's
// (SourceArt/UI/Kit/kit_manifest.json, written by Tools/ui/ft_ui_kit.py): change the kit, change the table below.
#include "FTHUDSkin.h"
#include "FTCore.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Engine/Engine.h"
#include "CanvasItem.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "GlobalRenderResources.h"
#include "UObject/GCObject.h"
#include "HAL/IConsoleManager.h"

namespace FTSkin
{
namespace
{
	constexpr int32 NumTex = static_cast<int32>(ETex::Count);

	struct FTexInfo
	{
		const TCHAR* Name;
		float W, H;             // size, texture px
		float L, T, R, B;       // slice margins, texture px (0 = none)
		float CW, CH;           // atlas cell, texture px (0 = not an atlas)
		int32 Cols;             // atlas columns
		float PX, PY;           // pivot in texture (or cell) px; negative = the centre
	};

	// One row per ETex, in the enum's order (KIT.md's order).
	const FTexInfo GInfo[] =
	{
		//  name                          W     H     L   T   R   B    CW   CH  cols  pivot
		{ TEXT("T_UI_Cyc_Wall"),          16, 1024,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Cyc_Pool"),         256,  256,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Cyc_Side"),         224,  672,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Dots"),              64,   64,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Paper"),            256,  256,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_SlateDust"),        256,  256,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_ClapStripes"),       64,   64,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_HazardStripes"),     32,   32,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Hatch"),             32,   32,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_FilmPerf"),          16,   16,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Tape"),             128,   32,   0,  0,  0,  0,    0,   0,  1,  64,  16 },
		{ TEXT("T_UI_Paperclip"),         32,   64,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Pin"),               28,   28,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_ClipboardClip"),    208,   64,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_ClapStick"),        704,   48,   0,  0,  0,  0,    0,   0,  1,  24,  24 },
		{ TEXT("T_UI_FilmCan"),          208,  208,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Cheque"),           800,  232,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Signature"),        256,   72,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Highlighter"),       64,   24,  16,  0, 16,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Grease"),           384,  192,   0,  0,  0,  0,  192,  96,  2,  -1,  -1 },
		{ TEXT("T_UI_Bulb"),              64,   32,   0,  0,  0,  0,   32,  32,  2,  -1,  -1 },
		{ TEXT("T_UI_Stamp_Approved"),   192,   72,   0,  0,  0,  0,    0,   0,  1,  96,  36 },
		{ TEXT("T_UI_Stamp_Rating"),     448,  528,   0,  0,  0,  0,  448, 132,  1, 224,  66 },
		{ TEXT("T_UI_Icons"),            512,  384,   0,  0,  0,  0,   64,  64,  8,  -1,  -1 },
		{ TEXT("T_UI_IconsBig"),         720,  144,   0,  0,  0,  0,  144, 144,  5,  -1,  -1 },
		{ TEXT("T_UI_BranchBurst"),      320,  320,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Boards"),          1536,  432,   0,  0,  0,  0,  384, 216,  4,  -1,  -1 },
		{ TEXT("T_UI_BoardThumbs"),     1024,   72,   0,  0,  0,  0,  128,  72,  8,  -1,  -1 },
		{ TEXT("T_UI_Lockups"),          896,  512,   0,  0,  0,  0,  448, 128,  2,  -1,  -1 },
		{ TEXT("T_UI_Plaque"),           288,   72,  30,  0, 34,  0,   96,  72,  3,  -1,  -1 },
		{ TEXT("T_UI_Keycap"),            96,   48,  12, 10, 12, 14,   48,  48,  2,  -1,  -1 },
		{ TEXT("T_UI_Cursor"),            96,   96,  26, 26, 32, 32,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_SheetFrame"),       192,  192,  44, 44, 52, 52,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Memo"),             192,  160,  84, 46, 36, 34,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Dailies_Card"),     192,  160,  40, 44, 28, 28,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Dailies_Pip"),       96,   48,   0,  0,  0,  0,   48,  48,  2,  -1,  -1 },
		{ TEXT("T_UI_Sticker_Cash"),     288,  144,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Sticker_SP"),       128,  128,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Sticker_Level"),    256,   96,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Kestrel_Hero"),     768,  304,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Hero_Burst"),       352,  352,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Chase"),            640,  256,   0,  0,  0,  0,  320, 128,  2,  -1,  -1 },
		{ TEXT("T_UI_Chase_Skyline"),    512,  128,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Hud_Plate"),        160,  256,  48,  0, 96,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Hud_Bar"),          160,   96,  32,  0, 64,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Hud_Chip"),          96,   40,  20,  0, 20,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Hud_Tag"),          112,   44,  30,  0, 18,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Hud_Pips"),         256,   32,   0,  0,  0,  0,   32,  32,  8,  -1,  -1 },
		{ TEXT("T_UI_Hud_Bracket"),       64,   64,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Hud_Combo_Fill"),   160,  160,   0,  0,  0,  0,    0,   0,  1,  76,  76 },
		{ TEXT("T_UI_Hud_Combo_Ink"),    160,  160,   0,  0,  0,  0,    0,   0,  1,  76,  76 },
		{ TEXT("T_UI_Hud_ArmorLow"),     288,  160,   0,  0,  0,  0,    0,   0,  1, 144,  80 },
		{ TEXT("T_UI_Hud_Balloon"),       96,   64,  32, 38, 26, 22,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Hud_Megaphone"),     72,   72,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Digits_Fill"),      880,  304,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
		{ TEXT("T_UI_Digits_Ink"),       880,  304,   0,  0,  0,  0,    0,   0,  1,  -1,  -1 },
	};
	static_assert(static_cast<int32>(UE_ARRAY_COUNT(GInfo)) == NumTex, "GInfo needs one row per ETex, in the enum's order");

	const FTexInfo& Info(ETex T)
	{
		return GInfo[FMath::Clamp(static_cast<int32>(T), 0, NumTex - 1)];
	}

	// The loaded assets, kept alive for the garbage collector the way AFTHUD's UPROPERTY members keep its logo and
	// fonts. Made on first use and never freed (it must not be destroyed after the object system at exit).
	class FSkinCache final : public FGCObject
	{
	public:
		TObjectPtr<UTexture2D> Textures[NumTex];
		bool bTried[NumTex] = {};
		TObjectPtr<UFont> BungeeFont;
		bool bTriedBungee = false;

		virtual void AddReferencedObjects(FReferenceCollector& Collector) override
		{
			for (TObjectPtr<UTexture2D>& Each : Textures)
			{
				Collector.AddReferencedObject(Each);
			}
			Collector.AddReferencedObject(BungeeFont);
		}
		virtual FString GetReferencerName() const override { return TEXT("FTSkinCache"); }
	};

	FSkinCache& Cache()
	{
		static FSkinCache* Instance = new FSkinCache();
		return *Instance;
	}

	/** The render resource of a kit texture, or null. */
	const FTexture* Res(ETex T)
	{
		UTexture2D* Loaded = Texture(T);
		return Loaded ? Loaded->GetResource() : nullptr;
	}

	float Snap(float V) { return FMath::RoundToFloat(V); }

	/** A whole number of screen px for a size given in kit px, never under one pixel. */
	float Line(float Kit, float Kp) { return FMath::Max(1.f, FMath::RoundToFloat(Kit * Kp)); }

	/** Half a texel is kept back from a piece that is magnified, so its neighbour in the atlas does not bleed in. */
	float Bleed(float Dest, float Src) { return Dest > Src + 0.5f ? 0.5f : 0.f; }

	/** One textured tile from its screen edges. */
	void Quad(UCanvas* C, const FTexture* R, float X0, float Y0, float X1, float Y1, const FVector2D& U0, const FVector2D& U1, const FLinearColor& Color)
	{
		if (!C || !R || X1 <= X0 || Y1 <= Y0 || Color.A <= 0.f) return;
		FCanvasTileItem It(FVector2D(X0, Y0), R, FVector2D(X1 - X0, Y1 - Y0), U0, U1, Color);
		It.BlendMode = SE_BLEND_Translucent;
		C->DrawItem(It);
	}

	/** The source rectangle in texture px: the whole texture, or cell Cell of an atlas. */
	void Source(const FTexInfo& N, int32 Cell, float& SX, float& SY, float& SW, float& SH)
	{
		SX = 0.f; SY = 0.f; SW = N.W; SH = N.H;
		if (Cell >= 0 && N.CW > 0.f && N.Cols > 0)
		{
			SX = float(Cell % N.Cols) * N.CW;
			SY = float(Cell / N.Cols) * N.CH;
			SW = N.CW;
			SH = N.CH;
		}
	}

	/** A piece of T given in texture px, drawn to screen edges. InX, InY: texels kept back on each side of the piece. */
	void Sub(UCanvas* C, ETex T, float SX, float SY, float SW, float SH, float X0, float Y0, float X1, float Y1,
		const FLinearColor& Color, float InX = 0.f, float InY = 0.f, bool bFlipX = false, bool bFlipY = false)
	{
		const FTexInfo& N = Info(T);
		if (SW <= 2.f * InX) InX = 0.f;
		if (SH <= 2.f * InY) InY = 0.f;
		FVector2D U0((SX + InX) / N.W, (SY + InY) / N.H);
		FVector2D U1((SX + SW - InX) / N.W, (SY + SH - InY) / N.H);
		if (bFlipX) Swap(U0.X, U1.X);
		if (bFlipY) Swap(U0.Y, U1.Y);
		Quad(C, Res(T), X0, Y0, X1, Y1, U0, U1, Color);
	}

	/** A flat four-sided shape: two triangles of the white texture, a colour per corner (P0 P1 on top, P3 P2 below). */
	void Quad4(UCanvas* C, const FVector2D& P0, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3,
		const FLinearColor& C0, const FLinearColor& C1, const FLinearColor& C2, const FLinearColor& C3)
	{
		if (!C) return;
		FCanvasUVTri A; A.V0_Pos = P0; A.V1_Pos = P1; A.V2_Pos = P2; A.V0_Color = C0; A.V1_Color = C1; A.V2_Color = C2;
		FCanvasUVTri B; B.V0_Pos = P0; B.V1_Pos = P2; B.V2_Pos = P3; B.V0_Color = C0; B.V1_Color = C2; B.V2_Color = C3;
		// (the item is made from the first triangle and takes the second: its list is the same two, and the one
		// allocation is the item's own, where a list built here and copied by the item was two)
		FCanvasTriangleItem It(A, GWhiteTexture);
		It.TriangleList.Add(B);
		It.BlendMode = SE_BLEND_Translucent;
		C->DrawItem(It);
	}

	/** The two triangles Para draws, added to a list instead of drawn: the same corners, colour and order, and the
	 *  same reasons to draw nothing. (HudMeter draws a whole meter as one item.) */
	void ParaTris(TArray<FCanvasUVTri>& Tris, float X, float Y, float W, float H, float Lean, const FLinearColor& Color, float A)
	{
		const FLinearColor Col = WithA(Color, A);
		if (W <= 0.f || H <= 0.f || Col.A <= 0.f) return;
		const FVector2D P0(X + Lean, Y), P1(X + W, Y), P2(X + W - Lean, Y + H), P3(X, Y + H);
		FCanvasUVTri One; One.V0_Pos = P0; One.V1_Pos = P1; One.V2_Pos = P2; One.V0_Color = Col; One.V1_Color = Col; One.V2_Color = Col;
		FCanvasUVTri Two; Two.V0_Pos = P0; Two.V1_Pos = P2; Two.V2_Pos = P3; Two.V0_Color = Col; Two.V1_Color = Col; Two.V2_Color = Col;
		Tris.Add(One); Tris.Add(Two);
	}

	/** A filled circle (an n-gon of the white texture). */
	void Disc(UCanvas* C, float CX, float CY, float Radius, const FLinearColor& Color)
	{
		if (!C || Radius <= 0.f || Color.A <= 0.f) return;
		FCanvasNGonItem It(FVector2D(CX, CY), FVector2D(Radius), 16, Color);
		It.BlendMode = SE_BLEND_Translucent;
		C->DrawItem(It);
	}

	// ---- the digit atlas (KIT.md, "The digit atlas in numbers")
	struct FDigitSet
	{
		float AtX, AtY;         // first cell
		float CW, CH;           // cell
		float Cap;              // capital height in the atlas
		float PenX, PenY;       // pen origin (baseline left) inside a cell
		float Adv[20];          // advance per glyph at the atlas's own cap height
	};
	const FDigitSet GDigitsL = { 0.f, 0.f, 88.f, 96.f, 64.f, 3.f, 77.f,
		{ 60.9f, 60.9f, 60.9f, 60.9f, 60.9f, 60.9f, 60.9f, 60.9f, 60.9f, 60.9f, 55.6f, 30.4f, 30.4f, 36.1f, 67.f, 46.4f, 58.8f, 36.2f, 30.4f, 19.5f } };
	const FDigitSet GDigitsS = { 0.f, 192.f, 52.f, 56.f, 36.f, 2.5f, 44.5f,
		{ 34.4f, 34.4f, 34.4f, 34.4f, 34.4f, 34.4f, 34.4f, 34.4f, 34.4f, 34.4f, 31.4f, 17.2f, 17.2f, 20.5f, 37.9f, 26.2f, 33.2f, 20.5f, 17.2f, 11.f } };
	constexpr int32 DigitSpace = 19;

	const FDigitSet& DigitSet(float CapH) { return CapH > 44.f ? GDigitsL : GDigitsS; }

	/** The atlas cell of a character: 0-9 $ , . / % x + - : space; -1 for anything else. */
	int32 DigitIndex(TCHAR Ch)
	{
		if (Ch >= TEXT('0') && Ch <= TEXT('9')) return static_cast<int32>(Ch - TEXT('0'));
		switch (Ch)
		{
		case TEXT('$'): return 10;
		case TEXT(','): return 11;
		case TEXT('.'): return 12;
		case TEXT('/'): return 13;
		case TEXT('%'): return 14;
		case TEXT('x'): case TEXT('X'): return 15;
		case TEXT('+'): return 16;
		case TEXT('-'): return 17;
		case TEXT(':'): return 18;
		case TEXT(' '): return DigitSpace;
		default: return -1;
		}
	}
}

// ---------------------------------------------------------------------------------------------------- palette
FLinearColor ShootColor(int32 Shoot1To8)
{
	const FLinearColor* const All[8] = { &Shoot1, &Shoot2, &Shoot3, &Shoot4, &Shoot5, &Shoot6, &Shoot7, &Shoot8 };
	return *All[FMath::Clamp(Shoot1To8, 1, 8) - 1];
}

FLinearColor BranchColor(int32 Branch0To4)
{
	const FLinearColor* const All[5] = { &BranchDem, &BranchWhl, &BranchStn, &BranchCar, &BranchGet };
	return *All[FMath::Clamp(Branch0To4, 0, 4)];
}

FLinearColor RateColor(int32 Rating0To3)
{
	const FLinearColor* const All[4] = { &Rate0, &Rate1, &Rate2, &Rate3 };
	return *All[FMath::Clamp(Rating0To3, 0, 3)];
}

// ---------------------------------------------------------------------------------------------------- textures
UTexture2D* Texture(ETex T)
{
	const int32 I = static_cast<int32>(T);
	if (I < 0 || I >= NumTex) return nullptr;
	FSkinCache& Kept = Cache();
	if (!Kept.bTried[I])
	{
		Kept.bTried[I] = true;
		const FString Path = FString::Printf(TEXT("/Game/UI/Kit/%s.%s"), GInfo[I].Name, GInfo[I].Name);
		Kept.Textures[I] = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Path, nullptr, LOAD_NoWarn | LOAD_Quiet));
		if (!Kept.Textures[I])
		{
			UE_LOG(LogFT, Warning, TEXT("FT SKIN missing texture %s (it draws as nothing)"), *Path);
		}
	}
	return Kept.Textures[I].Get();
}

int32 Preload()
{
	int32 Found = 0;
	for (int32 I = 0; I < NumTex; ++I)
	{
		if (Texture(static_cast<ETex>(I))) ++Found;
	}
	const bool bFont = Bungee() != nullptr;
	UE_LOG(LogFT, Display, TEXT("FT SKIN kit textures=%d of %d bungee=%d"), Found, NumTex, bFont ? 1 : 0);
	return Found;
}

void Reload()
{
	FSkinCache& Kept = Cache();
	for (int32 I = 0; I < NumTex; ++I)
	{
		Kept.Textures[I] = nullptr;
		Kept.bTried[I] = false;
	}
	Kept.BungeeFont = nullptr;
	Kept.bTriedBungee = false;
}

FVector2D TexSize(ETex T)
{
	const FTexInfo& N = Info(T);
	return FVector2D(N.W, N.H);
}

FVector2D CellSize(ETex T)
{
	const FTexInfo& N = Info(T);
	return N.CW > 0.f ? FVector2D(N.CW, N.CH) : FVector2D(N.W, N.H);
}

UFont* Bungee()
{
	FSkinCache& Kept = Cache();
	if (!Kept.bTriedBungee)
	{
		Kept.bTriedBungee = true;
		Kept.BungeeFont = Cast<UFont>(StaticLoadObject(UFont::StaticClass(), nullptr, TEXT("/Game/UI/Fonts/F_Bungee"), nullptr, LOAD_NoWarn | LOAD_Quiet));
		if (!Kept.BungeeFont)
		{
			UE_LOG(LogFT, Warning, TEXT("FT SKIN missing font /Game/UI/Fonts/F_Bungee"));
		}
	}
	return Kept.BungeeFont.Get();
}

// ---------------------------------------------------------------------------------------------------- primitives
void Box(UCanvas* C, float X, float Y, float W, float H, const FLinearColor& Color, float A)
{
	const float X0 = Snap(X), Y0 = Snap(Y), X1 = Snap(X + W), Y1 = Snap(Y + H);
	const FLinearColor Col = WithA(Color, A);
	if (!C || X1 <= X0 || Y1 <= Y0 || Col.A <= 0.f) return;
	FCanvasTileItem It(FVector2D(X0, Y0), FVector2D(X1 - X0, Y1 - Y0), Col);
	It.BlendMode = SE_BLEND_Translucent;
	C->DrawItem(It);
}

void Frame(UCanvas* C, float X, float Y, float W, float H, const FLinearColor& Color, float Th, float A)
{
	const float Wide = FMath::Max(1.f, FMath::RoundToFloat(Th));
	Box(C, X, Y, W, Wide, Color, A);
	Box(C, X, Y + H - Wide, W, Wide, Color, A);
	Box(C, X, Y + Wide, Wide, H - 2.f * Wide, Color, A);
	Box(C, X + W - Wide, Y + Wide, Wide, H - 2.f * Wide, Color, A);
}

void Para(UCanvas* C, float X, float Y, float W, float H, float Lean, const FLinearColor& Color, float A)
{
	const FLinearColor Col = WithA(Color, A);
	if (!C || W <= 0.f || H <= 0.f || Col.A <= 0.f) return;
	Quad4(C, FVector2D(X + Lean, Y), FVector2D(X + W, Y), FVector2D(X + W - Lean, Y + H), FVector2D(X, Y + H), Col, Col, Col, Col);
}

void Tex(UCanvas* C, ETex T, float X, float Y, float W, float H, const FLinearColor& Tint, float A, bool bFlipX, bool bFlipY)
{
	const FTexInfo& N = Info(T);
	Sub(C, T, 0.f, 0.f, N.W, N.H, Snap(X), Snap(Y), Snap(X + W), Snap(Y + H), WithA(Tint, A), 0.f, 0.f, bFlipX, bFlipY);
}

FVector2D TexNat(UCanvas* C, float S, ETex T, float X, float Y, const FLinearColor& Tint, float A, float Scale, bool bFlipX, bool bFlipY)
{
	const FTexInfo& N = Info(T);
	const float Kp = KitPx(S) * Scale;
	Tex(C, T, X, Y, N.W * Kp, N.H * Kp, Tint, A, bFlipX, bFlipY);
	return FVector2D(N.W * Kp, N.H * Kp);
}

void TexCell(UCanvas* C, ETex T, int32 Cell, float X, float Y, float W, float H, const FLinearColor& Tint, float A, bool bFlipX)
{
	float SX = 0.f, SY = 0.f, SW = 0.f, SH = 0.f;
	Source(Info(T), Cell, SX, SY, SW, SH);
	const float X0 = Snap(X), Y0 = Snap(Y), X1 = Snap(X + W), Y1 = Snap(Y + H);
	Sub(C, T, SX, SY, SW, SH, X0, Y0, X1, Y1, WithA(Tint, A), Bleed(X1 - X0, SW), Bleed(Y1 - Y0, SH), bFlipX, false);
}

FVector2D TexCellNat(UCanvas* C, float S, ETex T, int32 Cell, float X, float Y, const FLinearColor& Tint, float A, float Scale)
{
	const FVector2D Size = CellSize(T) * static_cast<double>(KitPx(S) * Scale);
	TexCell(C, T, Cell, X, Y, static_cast<float>(Size.X), static_cast<float>(Size.Y), Tint, A, false);
	return Size;
}

void TexTiled(UCanvas* C, float S, ETex T, float X, float Y, float W, float H, const FLinearColor& Tint, float A, float Scale, bool bTileX, bool bTileY, float OffX, float OffY)
{
	const FTexInfo& N = Info(T);
	const float X0 = Snap(X), Y0 = Snap(Y), X1 = Snap(X + W), Y1 = Snap(Y + H);
	const float TileW = N.W * KitPx(S) * Scale, TileH = N.H * KitPx(S) * Scale;     // one tile on screen
	float UA = 0.f, VA = 0.f, UB = 1.f, VB = 1.f;
	if (bTileX && TileW > 0.f)
	{
		UA = FMath::Fmod(OffX / TileW, 1.f);
		UB = UA + (X1 - X0) / TileW;
	}
	if (bTileY && TileH > 0.f)
	{
		VA = FMath::Fmod(OffY / TileH, 1.f);
		VB = VA + (Y1 - Y0) / TileH;
	}
	Quad(C, Res(T), X0, Y0, X1, Y1, FVector2D(UA, VA), FVector2D(UB, VB), WithA(Tint, A));
}

void TexRot(UCanvas* C, ETex T, float X, float Y, float W, float H, float Degrees, const FVector2D& Pivot01, const FLinearColor& Tint, float A, int32 Cell)
{
	const FTexture* R = Res(T);
	const FLinearColor Col = WithA(Tint, A);
	if (!C || !R || W <= 0.f || H <= 0.f || Col.A <= 0.f) return;
	const FTexInfo& N = Info(T);
	float SX = 0.f, SY = 0.f, SW = 0.f, SH = 0.f;
	Source(N, Cell, SX, SY, SW, SH);
	const float InX = Cell >= 0 ? Bleed(W, SW) : 0.f, InY = Cell >= 0 ? Bleed(H, SH) : 0.f;
	FCanvasTileItem It(FVector2D(X, Y), R, FVector2D(W, H),
		FVector2D((SX + InX) / N.W, (SY + InY) / N.H), FVector2D((SX + SW - InX) / N.W, (SY + SH - InY) / N.H), Col);
	It.BlendMode = SE_BLEND_Translucent;
	It.Rotation = FRotator(0.f, Degrees, 0.f);      // yaw: clockwise on screen
	It.PivotPoint = Pivot01;                        // a fraction of the tile
	C->DrawItem(It);
}

FVector2D TexPivot(UCanvas* C, float S, ETex T, float PX, float PY, float Degrees, float Scale, const FLinearColor& Tint, float A, int32 Cell)
{
	const FTexInfo& N = Info(T);
	float SX = 0.f, SY = 0.f, SW = 0.f, SH = 0.f;
	Source(N, Cell, SX, SY, SW, SH);
	const float Kp = KitPx(S) * Scale;
	const float PivX = N.PX >= 0.f ? N.PX : SW * 0.5f, PivY = N.PY >= 0.f ? N.PY : SH * 0.5f;
	const float W = SW * Kp, H = SH * Kp;
	if (SW > 0.f && SH > 0.f)
	{
		TexRot(C, T, PX - PivX * Kp, PY - PivY * Kp, W, H, Degrees, FVector2D(PivX / SW, PivY / SH), Tint, A, Cell);
	}
	return FVector2D(W, H);
}

void NineSlice(UCanvas* C, float S, ETex T, float X, float Y, float W, float H, const FLinearColor& Tint, float A, bool bCentre, int32 Cell)
{
	const FTexInfo& N = Info(T);
	float SX = 0.f, SY = 0.f, SW = 0.f, SH = 0.f;
	Source(N, Cell, SX, SY, SW, SH);
	const float Kp = KitPx(S);
	const float X0 = Snap(X), Y0 = Snap(Y), X3 = Snap(X + W), Y3 = Snap(Y + H);
	if (X3 <= X0 || Y3 <= Y0) return;
	float ML = N.L * Kp, MR = N.R * Kp, MT = N.T * Kp, MB = N.B * Kp;      // the margins on screen: never stretched ...
	if (ML + MR > X3 - X0)                                                  // ... only squeezed when the rectangle is too small for them
	{
		const float Fit = (X3 - X0) / (ML + MR);
		ML *= Fit; MR *= Fit;
	}
	if (MT + MB > Y3 - Y0)
	{
		const float Fit = (Y3 - Y0) / (MT + MB);
		MT *= Fit; MB *= Fit;
	}
	const float DX[4] = { X0, Snap(X0 + ML), Snap(X3 - MR), X3 };
	const float DY[4] = { Y0, Snap(Y0 + MT), Snap(Y3 - MB), Y3 };
	const float UX[4] = { SX, SX + N.L, SX + SW - N.R, SX + SW };
	const float UY[4] = { SY, SY + N.T, SY + SH - N.B, SY + SH };
	const FLinearColor Col = WithA(Tint, A);
	for (int32 Row = 0; Row < 3; ++Row)
	{
		for (int32 Column = 0; Column < 3; ++Column)
		{
			if (Row == 1 && Column == 1 && !bCentre) continue;
			// a stretched piece keeps half a texel back along its stretch, so the caps do not smear into it
			Sub(C, T, UX[Column], UY[Row], UX[Column + 1] - UX[Column], UY[Row + 1] - UY[Row],
				DX[Column], DY[Row], DX[Column + 1], DY[Row + 1], Col, Column == 1 ? 0.5f : 0.f, Row == 1 ? 0.5f : 0.f);
		}
	}
}

void ThreeSliceH(UCanvas* C, ETex T, float X, float Y, float W, float H, const FLinearColor& Tint, float A, int32 Cell)
{
	const FTexInfo& N = Info(T);
	float SX = 0.f, SY = 0.f, SW = 0.f, SH = 0.f;
	Source(N, Cell, SX, SY, SW, SH);
	const float X0 = Snap(X), Y0 = Snap(Y), X3 = Snap(X + W), Y1 = Snap(Y + H);
	if (SH <= 0.f || X3 <= X0 || Y1 <= Y0) return;
	const float Kt = (Y1 - Y0) / SH;                // uniform: screen px per texel
	float CapL = N.L * Kt, CapR = N.R * Kt;
	if (CapL + CapR > X3 - X0)
	{
		const float Fit = (X3 - X0) / (CapL + CapR);
		CapL *= Fit; CapR *= Fit;
	}
	const float X1 = Snap(X0 + CapL), X2 = Snap(X3 - CapR);
	const float Keep = (Cell >= 0 && Kt > 1.01f) ? 0.5f : 0.f;     // a magnified atlas cell: keep its neighbours out
	const FLinearColor Col = WithA(Tint, A);
	Sub(C, T, SX, SY, N.L, SH, X0, Y0, X1, Y1, Col, Keep, Keep);
	Sub(C, T, SX + N.L, SY, SW - N.L - N.R, SH, X1, Y0, X2, Y1, Col, 0.5f, Keep);
	Sub(C, T, SX + SW - N.R, SY, N.R, SH, X2, Y0, X3, Y1, Col, Keep, Keep);
}

// ---------------------------------------------------------------------------------------------------- the wall
void WallRect(UCanvas* C, float X, float Y, float W, float H, float A)
{
	if (!C || C->SizeY <= 0 || A <= 0.f) return;
	const float ScreenH = static_cast<float>(C->SizeY);
	const float X0 = Snap(X), X1 = Snap(X + W), Y0 = FMath::Max(0.f, Snap(Y)), Y1 = FMath::Min(ScreenH, Snap(Y + H));
	if (X1 <= X0 || Y1 <= Y0) return;
	if (const FTexture* R = Res(ETex::CycWall))
	{
		// V follows the screen, so a piece of wall has its bands where the whole wall has them
		Quad(C, R, X0, Y0, X1, Y1, FVector2D(0.5f, Y0 / ScreenH), FVector2D(0.5f, Y1 / ScreenH), WithA(White, A));
		return;
	}
	// the kit is not imported yet: the same wall as a gradient (ft_ui_kit.py, t_cyc_wall)
	struct FStop { float V; FLinearColor Color; };
	const FStop Stops[6] =
	{
		{ 0.f, CycTop }, { 0.5f, CycMid }, { 0.788f, CycLow }, { 0.802f, Hex(0xFFE8D4) }, { 0.85f, CycFloor }, { 1.f, Mix(CycFloor, Hex(0xE2968C), 0.38f) }
	};
	for (int32 I = 0; I + 1 < 6; ++I)
	{
		const float V0 = Stops[I].V, V1 = Stops[I + 1].V;
		const float Top = FMath::Max(Y0, V0 * ScreenH), Foot = FMath::Min(Y1, V1 * ScreenH);
		if (Foot <= Top) continue;
		const FLinearColor Ca = WithA(Mix(Stops[I].Color, Stops[I + 1].Color, (Top / ScreenH - V0) / (V1 - V0)), A);
		const FLinearColor Cb = WithA(Mix(Stops[I].Color, Stops[I + 1].Color, (Foot / ScreenH - V0) / (V1 - V0)), A);
		Quad4(C, FVector2D(X0, Top), FVector2D(X1, Top), FVector2D(X1, Foot), FVector2D(X0, Foot), Ca, Ca, Cb, Cb);
	}
}

void WallPool(UCanvas* C, float CX, float CY, float R, const FLinearColor& Tint, float A)
{
	Tex(C, ETex::CycPool, CX - R, CY - R, R, R, Tint, A, false, false);
	Tex(C, ETex::CycPool, CX, CY - R, R, R, Tint, A, true, false);
	Tex(C, ETex::CycPool, CX - R, CY, R, R, Tint, A, false, true);
	Tex(C, ETex::CycPool, CX, CY, R, R, Tint, A, true, true);
}

void Wall(UCanvas* C, float S, float A, bool bSideDressing, bool bPools)
{
	if (!C) return;
	const float ScreenW = static_cast<float>(C->SizeX), ScreenH = static_cast<float>(C->SizeY);
	WallRect(C, 0.f, 0.f, ScreenW, ScreenH, A);
	const float Kp = KitPx(S);
	const float BlockX = (ScreenW - 1280.f * S) * 0.5f;       // the left edge of the centre 16:9 block (AFTHUD's Left)
	if (bPools)
	{
		WallPool(C, BlockX + 520.f * Kp, 400.f * Kp, 420.f * Kp, Chalk, A * 0.42f);
		WallPool(C, BlockX + 1420.f * Kp, 520.f * Kp, 400.f * Kp, Chalk, A * 0.36f);
	}
	if (bSideDressing && BlockX > 8.f)
	{
		const float SideW = ScreenH / 3.f;
		Tex(C, ETex::CycSide, BlockX - SideW, 0.f, SideW, ScreenH, White, A, false, false);
		Tex(C, ETex::CycSide, ScreenW - BlockX, 0.f, SideW, ScreenH, White, A, true, false);
	}
}

// ---------------------------------------------------------------------------------------------------- panels
void PaperPanel(UCanvas* C, float S, float X, float Y, float W, float H, const FLinearColor& Stock, float A, float ShadowKit, const FLinearColor& ShadowColor, float OutlineKit)
{
	const float Kp = KitPx(S);
	const float Out = OutlineKit > 0.f ? Line(OutlineKit, Kp) : 0.f;
	const float Drop = ShadowKit > 0.f ? Line(ShadowKit, Kp) : 0.f;
	const float PX = Snap(X), PY = Snap(Y), PW = Snap(W), PH = Snap(H);
	if (Drop > 0.f) Box(C, PX + Drop, PY + Drop, PW, PH, ShadowColor, A);
	if (Out > 0.f) Box(C, PX, PY, PW, PH, Ink, A);
	if (Texture(ETex::Paper))
	{
		TexTiled(C, S, ETex::Paper, PX + Out, PY + Out, PW - 2.f * Out, PH - 2.f * Out, Stock, A, 1.f, true, true, PX + Out, PY + Out);
	}
	else
	{
		Box(C, PX + Out, PY + Out, PW - 2.f * Out, PH - 2.f * Out, Stock, A);
	}
}

void SlatePanel(UCanvas* C, float S, float X, float Y, float W, float H, float A, float ShadowKit, const FLinearColor& Fill, bool bRim, const FLinearColor& ShadowColor, float OutlineKit)
{
	const float Kp = KitPx(S);
	const float Out = OutlineKit > 0.f ? Line(OutlineKit, Kp) : 0.f;
	const float Drop = ShadowKit > 0.f ? Line(ShadowKit, Kp) : 0.f;
	const float PX = Snap(X), PY = Snap(Y), PW = Snap(W), PH = Snap(H);
	if (Drop > 0.f) Box(C, PX + Drop, PY + Drop, PW, PH, ShadowColor, A);
	if (Out > 0.f) Box(C, PX, PY, PW, PH, Ink, A);
	Box(C, PX + Out, PY + Out, PW - 2.f * Out, PH - 2.f * Out, Fill, A);
	TexTiled(C, S, ETex::SlateDust, PX + Out, PY + Out, PW - 2.f * Out, PH - 2.f * Out, Chalk, A, 1.f, true, true, PX + Out, PY + Out);
	if (bRim)
	{
		const float In = Line(7.f, Kp);
		Frame(C, PX + In, PY + In, PW - 2.f * In, PH - 2.f * In, Chalk, 1.5f * Kp, A * 0.22f);
	}
}

void LedgerPanel(UCanvas* C, float S, float X, float Y, float W, float H, float RowY, float RowH, int32 Rows, float A, float MarginKit, float ShadowKit)
{
	const float Kp = KitPx(S);
	const float Out = Line(3.f, Kp);
	PaperPanel(C, S, X, Y, W, H, Ledger, A, ShadowKit);
	for (int32 Row = 0; Row < Rows; Row += 2)
	{
		Box(C, X + Out, RowY + static_cast<float>(Row) * RowH, W - 2.f * Out, RowH, LedgerBar, A);
	}
	const float RuleW = Line(1.5f, Kp);
	Box(C, X + MarginKit * Kp, Y + Out, RuleW, H - 2.f * Out, Red, A * 0.65f);
	Box(C, X + (MarginKit + 5.f) * Kp, Y + Out, RuleW, H - 2.f * Out, Red, A * 0.65f);
	const float HoleAt[3] = { 0.14f, 0.5f, 0.86f };
	for (int32 I = 0; I < 3; ++I)
	{
		Disc(C, X + MarginKit * Kp * 0.5f, Y + H * HoleAt[I], 10.f * Kp, WithA(Ink, A));
		Disc(C, X + MarginKit * Kp * 0.5f, Y + H * HoleAt[I], 7.5f * Kp, WithA(Slate, A));
	}
}

FVector2D Memo(UCanvas* C, float S, float X, float Y, float W, float H, float A)
{
	const float Kp = KitPx(S);
	if (Texture(ETex::Memo))
	{
		NineSlice(C, S, ETex::Memo, X, Y, W + 6.f * Kp, H + 6.f * Kp, White, A);
	}
	else
	{
		PaperPanel(C, S, X, Y, W, H, Chalk, A, 6.f);
		Box(C, X + Line(3.f, Kp), Y + Line(3.f, Kp), W - 2.f * Line(3.f, Kp), 38.f * Kp, Manila, A);
	}
	return FVector2D(X + 20.f * Kp, Y + 52.f * Kp);
}

FVector2D DailiesCard(UCanvas* C, float S, float X, float Y, float W, float H, float A)
{
	const float Kp = KitPx(S);
	if (Texture(ETex::DailiesCard))
	{
		NineSlice(C, S, ETex::DailiesCard, X, Y, W + 6.f * Kp, H + 6.f * Kp, White, A);
	}
	else
	{
		PaperPanel(C, S, X, Y, W, H, Paper, A, 6.f);
		Box(C, X + Line(3.f, Kp), Y + Line(3.f, Kp), W - 2.f * Line(3.f, Kp), 30.f * Kp, Slate, A);
	}
	return FVector2D(X + 20.f * Kp, Y + 50.f * Kp);
}

FBox2D SheetFrame(UCanvas* C, float S, float X, float Y, float W, float H, float A, bool bTape)
{
	const float Kp = KitPx(S);
	NineSlice(C, S, ETex::SheetFrame, X, Y, W + 8.f * Kp, H + 8.f * Kp, White, A, false);
	if (bTape)
	{
		TexPivot(C, S, ETex::Tape, X - 34.f * Kp, Y + 2.f * Kp, -38.f, 1.f, White, A);
		TexPivot(C, S, ETex::Tape, X + W + 34.f * Kp, Y + 2.f * Kp, 38.f, 1.f, White, A);
	}
	return FBox2D(FVector2D(X + 30.f * Kp, Y + 30.f * Kp), FVector2D(X + W - 30.f * Kp, Y + H - 30.f * Kp));
}

FBox2D ClipboardPanel(UCanvas* C, float S, float X, float Y, float W, float H, float A)
{
	const float Kp = KitPx(S), Pad = 14.f * Kp;
	PaperPanel(C, S, X, Y, W, H, Mix(Manila, AmberInk, 0.35f), A);
	const float SheetX = X + Pad, SheetY = Y + Pad + 10.f * Kp, SheetW = W - 2.f * Pad, SheetH = H - 2.f * Pad - 10.f * Kp;
	PaperPanel(C, S, SheetX, SheetY, SheetW, SheetH, Paper, A, 4.f, Mix(Manila, Ink, 0.55f));
	TexNat(C, S, ETex::ClipboardClip, X + W * 0.5f - 104.f * Kp, Y - 28.f * Kp, White, A);
	return FBox2D(FVector2D(SheetX, SheetY), FVector2D(SheetX + SheetW, SheetY + SheetH));
}

FBox2D InkPlate(UCanvas* C, float S, float X, float Y, float W, float H, const FLinearColor& Keyline, float A, bool bSlantedEnds)
{
	const float Kp = KitPx(S);
	const ETex T = H >= 160.f * Kp ? ETex::HudPlate : ETex::HudBar;
	if (bSlantedEnds && Texture(T))
	{
		ThreeSliceH(C, T, X, Y, W, H, Keyline, A);
		const float Kt = H / Info(T).H;                 // screen px per texel of the sticker
		const float BodyH = H - 5.f * Kt;               // the sticker without its shadow
		const float Slant = 0.12f * BodyH, Rim = 5.f * Kt;
		return FBox2D(FVector2D(X + Slant + Rim, Y + Rim), FVector2D(X + W - 5.f * Kt - Slant - Rim, Y + BodyH - Rim));
	}
	// the same sticker from flat boxes: square ends
	const float Drop = Line(5.f, Kp), Rim = Line(2.f, Kp), Key = Line(3.f, Kp);
	const float BodyW = W - Drop, BodyH = H - Drop;
	Box(C, X + Drop, Y + Drop, BodyW, BodyH, Ink, A * HudShadow);
	Box(C, X, Y, BodyW, BodyH, Ink, A);
	Box(C, X + Rim, Y + Rim, BodyW - 2.f * Rim, BodyH - 2.f * Rim, Keyline, A);
	Box(C, X + Rim + Key, Y + Rim + Key, BodyW - 2.f * (Rim + Key), BodyH - 2.f * (Rim + Key), Slate, A);
	return FBox2D(FVector2D(X + Rim + Key, Y + Rim + Key), FVector2D(X + BodyW - Rim - Key, Y + BodyH - Rim - Key));
}

// ---------------------------------------------------------------------------------------------------- stamps, stickers, the balloon
void Stamp(UCanvas* C, float S, float CX, float CY, int32 Which, float Degrees, float Scale, float A)
{
	if (Which < 0)
	{
		TexPivot(C, S, ETex::StampApproved, CX, CY, Degrees, Scale, White, A);
	}
	else
	{
		TexPivot(C, S, ETex::StampRating, CX, CY, Degrees, Scale, White, A, FMath::Clamp(Which, 0, 3));
	}
}

FVector2D StickerCash(UCanvas* C, float S, float X, float Y, const FString& Amount, float A, float Scale, bool bLabelRoom)
{
	const float Kp = KitPx(S) * Scale;
	Tex(C, ETex::StickerCash, X, Y, 288.f * Kp, 144.f * Kp, White, A, false, false);
	float Cap = 30.f * Kp;
	const float Step = FMath::Max(0.5f, Kp);
	while (Cap > 14.f * Kp && MeasureDigits(Amount, Cap) > 186.f * Kp)
	{
		Cap -= Step;
	}
	Digits(C, Amount, X + 138.f * Kp, Y + (bLabelRoom ? 60.f : 52.f) * Kp + Cap, Cap, Ink, A, 1, false);
	return FVector2D(288.f * Kp, 144.f * Kp);
}

FVector2D StickerSP(UCanvas* C, float S, float X, float Y, int32 Points, float A, float Scale)
{
	const float Kp = KitPx(S) * Scale;
	Tex(C, ETex::StickerSP, X, Y, 128.f * Kp, 128.f * Kp, White, A, false, false);
	const FString Str = FString::FromInt(Points);
	const float Cap = (Str.Len() < 3 ? 36.f : 24.f) * Kp;
	Digits(C, Str, X + 58.f * Kp, Y + 60.f * Kp + Cap * 0.5f, Cap, Ink, A, 1, false);
	return FVector2D(128.f * Kp, 128.f * Kp);
}

FVector2D StickerLevel(UCanvas* C, float S, float X, float Y, int32 Level, float A)
{
	const float Kp = KitPx(S);
	Tex(C, ETex::StickerLevel, X, Y, 256.f * Kp, 96.f * Kp, White, A, false, false);
	const FString Str = FString::FromInt(Level);
	const float Cap = (Str.Len() < 2 ? 48.f : 36.f) * Kp;
	Digits(C, Str, X + 46.f * Kp, Y + 45.f * Kp + Cap * 0.5f, Cap, Ink, A, 1, false);
	return FVector2D(256.f * Kp, 96.f * Kp);
}

FBox2D Balloon(UCanvas* C, float S, float X, float Y, float TextW, float A, bool bMegaphone, float H)
{
	const float Kp = KitPx(S);
	const float BalloonX = X + (bMegaphone ? 60.f * Kp : 0.f), BalloonY = Y + 6.f * Kp;
	const float BalloonW = TextW + 64.f * Kp, BalloonH = H > 0.f ? H : 64.f * Kp;
	if (Texture(ETex::HudBalloon))
	{
		NineSlice(C, S, ETex::HudBalloon, BalloonX, BalloonY, BalloonW, BalloonH, White, A);
	}
	else
	{
		PaperPanel(C, S, BalloonX + 16.f * Kp, BalloonY, BalloonW - 22.f * Kp, BalloonH - 6.f * Kp, Chalk, A, 6.f, WithA(Ink, HudShadow), 4.f);
	}
	if (bMegaphone)
	{
		Tex(C, ETex::HudMegaphone, X, Y + 2.f * Kp, 72.f * Kp, 72.f * Kp, White, A, false, false);
	}
	return FBox2D(FVector2D(BalloonX + 36.f * Kp, BalloonY), FVector2D(BalloonX + 36.f * Kp + TextW, BalloonY + BalloonH - 6.f * Kp));
}

void Combo(UCanvas* C, float S, float CX, float CY, int32 Mult, const FLinearColor& Tint, float A, float Scale, const FLinearColor& Numeral)
{
	TexPivot(C, S, ETex::HudComboInk, CX, CY, 0.f, Scale, White, A);
	TexPivot(C, S, ETex::HudComboFill, CX, CY, 0.f, Scale, Tint, A);
	const float Kp = KitPx(S) * Scale, Cap = 36.f * Kp;
	Digits(C, FString::Printf(TEXT("x%d"), Mult), CX - 2.f * Kp, CY + Cap * 0.5f - Kp, Cap, Numeral, A, 1);
}

void ArmorLow(UCanvas* C, float S, float CX, float CY, float A, float Scale)
{
	TexPivot(C, S, ETex::HudArmorLow, CX, CY, 0.f, Scale, White, A);
}

// ---------------------------------------------------------------------------------------------------- controls and small parts
void Marquee(UCanvas* C, float S, float X, float Y, float W, float H, float Time, bool bReducedMotion, float A, float StepsPerSecond, float BandKit, float PitchKit)
{
	const float Kp = KitPx(S), Band = BandKit * Kp, Lip = 3.f * Kp, Pitch = FMath::Max(1.f, PitchKit * Kp);
	Box(C, X - Band - Lip, Y - Band - Lip, W + 2.f * (Band + Lip), H + 2.f * (Band + Lip), Ink, A);
	Box(C, X - Band, Y - Band, W + 2.f * Band, H + 2.f * Band, MarqueeBand, A);
	const int32 NX = FMath::Max(2, FMath::RoundToInt32((W + Band) / Pitch));
	const int32 NY = FMath::Max(1, FMath::RoundToInt32((H + Band) / Pitch));
	// the chase: which bulbs are lit moves one bulb a step, three steps a second at most; reduced motion: it stands
	const int32 Step = bReducedMotion ? 0 : FMath::FloorToInt32(FMath::Fmod(FMath::Max(0.f, Time) * FMath::Clamp(StepsPerSecond, 0.f, 3.f), 3.f));
	const float Bulb = 32.f * Kp, HalfBulb = Bulb * 0.5f, HalfBand = Band * 0.5f;
	const int32 Total = 2 * (NX + NY);
	for (int32 I = 0; I < Total; ++I)
	{
		float BX = 0.f, BY = 0.f;
		if (I < NX)
		{
			BX = X - HalfBand + (W + Band) * static_cast<float>(I) / static_cast<float>(NX);
			BY = Y - HalfBand;
		}
		else if (I < NX + NY)
		{
			BX = X + W + HalfBand;
			BY = Y - HalfBand + (H + Band) * static_cast<float>(I - NX) / static_cast<float>(NY);
		}
		else if (I < 2 * NX + NY)
		{
			BX = X + W + HalfBand - (W + Band) * static_cast<float>(I - NX - NY) / static_cast<float>(NX);
			BY = Y + H + HalfBand;
		}
		else
		{
			BX = X - HalfBand;
			BY = Y + H + HalfBand - (H + Band) * static_cast<float>(I - 2 * NX - NY) / static_cast<float>(NY);
		}
		const bool bLit = ((I - Step) % 3 + 3) % 3 != 2;
		TexCell(C, ETex::Bulb, bLit ? 0 : 1, BX - HalfBulb, BY - HalfBulb, Bulb, Bulb, White, A, false);
	}
}

void Brackets(UCanvas* C, float S, const FLinearColor& Tint, float A, float InsetKit)
{
	if (!C) return;
	const float Kp = KitPx(S), In = InsetKit * Kp, Size = 64.f * Kp;
	const float ScreenW = static_cast<float>(C->SizeX), ScreenH = static_cast<float>(C->SizeY);
	Tex(C, ETex::HudBracket, In, In, Size, Size, Tint, A, false, false);
	Tex(C, ETex::HudBracket, ScreenW - In - Size, In, Size, Size, Tint, A, true, false);
	Tex(C, ETex::HudBracket, In, ScreenH - In - Size, Size, Size, Tint, A, false, true);
	Tex(C, ETex::HudBracket, ScreenW - In - Size, ScreenH - In - Size, Size, Size, Tint, A, true, true);
}

void ClapStripes(UCanvas* C, float S, float X, float Y, float W, float H, const FLinearColor& Tint, float A, float Scale)
{
	TexTiled(C, S, ETex::ClapStripes, X, Y, W, H, Tint, A, Scale);
}

void HazardStripes(UCanvas* C, float S, float X, float Y, float W, float H, const FLinearColor& Tint, float A, float Scale)
{
	TexTiled(C, S, ETex::HazardStripes, X, Y, W, H, Tint, A, Scale);
}

void Hatch(UCanvas* C, float S, float X, float Y, float W, float H, float A)
{
	TexTiled(C, S, ETex::Hatch, X, Y, W, H, White, A);
}

void Dots(UCanvas* C, float S, float X, float Y, float W, float H, const FLinearColor& Tint, float A, float Scale)
{
	TexTiled(C, S, ETex::Dots, X, Y, W, H, Tint, A, Scale);
}

float KeyCapWidth(float TextW, float H)
{
	return FMath::Max(H, TextW + H * 0.62f);
}

float KeyCap(UCanvas* C, float S, float X, float Y, float TextW, float H, bool bOnDark, float A)
{
	const float W = KeyCapWidth(TextW, H);
	if (Texture(ETex::Keycap))
	{
		NineSlice(C, S, ETex::Keycap, X, Y, W, H, White, A, true, bOnDark ? 1 : 0);
	}
	else
	{
		const float Kp = KitPx(S);
		Box(C, X, Y, W, H, Ink, A);
		Box(C, X + Line(2.f, Kp), Y + Line(2.f, Kp), W - 2.f * Line(2.f, Kp), H - Line(2.f, Kp) - Line(4.f, Kp), bOnDark ? Chalk : Steel, A);
	}
	return W;
}

void Cursor(UCanvas* C, float S, float X, float Y, float W, float H, const FLinearColor& Tint, float A, float GapKit)
{
	const float Kp = KitPx(S), Out = (GapKit + 10.f) * Kp;     // the band is 10 kit px; its shadow adds 6 right and below
	NineSlice(C, S, ETex::Cursor, X - Out, Y - Out, W + 2.f * Out + 6.f * Kp, H + 2.f * Out + 6.f * Kp, Tint, A, false);
}

void Plaque(UCanvas* C, float X, float Y, float W, float H, EPlaque State, float A)
{
	const float Kt = H / 66.f;      // the plaque is 66 of its cell's 72 texels; the rest is its shadow
	ThreeSliceH(C, ETex::Plaque, X, Y, W + 6.f * Kt, 72.f * Kt, White, A, static_cast<int32>(State));
}

void Highlighter(UCanvas* C, float X, float Y, float W, float H, const FLinearColor& Tint, float A)
{
	ThreeSliceH(C, ETex::Highlighter, X, Y, W, H, Tint, A);
}

void Grease(UCanvas* C, EGrease Mark, float X, float Y, float W, float H, const FLinearColor& Tint, float A)
{
	TexCell(C, ETex::Grease, static_cast<int32>(Mark), X, Y, W, H, Tint, A, false);
}

void Tape(UCanvas* C, float S, float CX, float CY, float Degrees, float A)
{
	TexPivot(C, S, ETex::Tape, CX, CY, Degrees, 1.f, White, A);
}

void ClapStick(UCanvas* C, float S, float HingeX, float HingeY, float Degrees, float A, float Scale)
{
	TexPivot(C, S, ETex::ClapStick, HingeX, HingeY, Degrees, Scale, White, A);
}

void IconTex(UCanvas* C, EIco Icon, float X, float Y, float Size, const FLinearColor& Tint, float A, bool bFlipX)
{
	const bool bColour = Icon == EIco::BadgeCheck || Icon == EIco::BadgeLock;
	TexCell(C, ETex::Icons, static_cast<int32>(Icon), X, Y, Size, Size, bColour ? White : Tint, A, bFlipX);
}

void BranchArt(UCanvas* C, float S, float X, float Y, int32 Branch, float A, float Scale)
{
	const float Kp = KitPx(S) * Scale;
	const int32 Cell = FMath::Clamp(Branch, 0, 4);
	Tex(C, ETex::BranchBurst, X, Y, 320.f * Kp, 320.f * Kp, BranchColor(Cell), A, false, false);
	const float At = (154.f - 72.f) * Kp, Size = 144.f * Kp;
	TexCell(C, ETex::IconsBig, Cell, X + At + 4.f * Kp, Y + At + 2.f * Kp, Size, Size, Chalk, A, false);
	TexCell(C, ETex::IconsBig, Cell, X + At, Y + At - 2.f * Kp, Size, Size, Ink, A, false);
}

float DailiesPips(UCanvas* C, float S, float X, float Y, int32 Lit, int32 Total, float A, float Scale)
{
	const float Kp = KitPx(S) * Scale, Size = 48.f * Kp, Pitch = 46.f * Kp;
	for (int32 I = 0; I < Total; ++I)
	{
		TexCell(C, ETex::DailiesPip, I < Lit ? 0 : 1, X + static_cast<float>(I) * Pitch, Y, Size, Size, White, A, false);
	}
	return Total > 0 ? static_cast<float>(Total - 1) * Pitch + Size : 0.f;
}

// ---------------------------------------------------------------------------------------------------- meters and pips
float ArrowPips(UCanvas* C, float S, float X, float Y, int32 Have, int32 Total, const FLinearColor& Tint, const FLinearColor& Empty, float A, float PitchKit)
{
	const float Kp = KitPx(S), Size = 32.f * Kp, Pitch = PitchKit * Kp;
	for (int32 I = 0; I < Total; ++I)
	{
		TexCell(C, ETex::HudPips, I < Have ? 0 : 1, X + static_cast<float>(I) * Pitch, Y, Size, Size, I < Have ? Tint : Empty, A, false);
	}
	return Total > 0 ? static_cast<float>(Total - 1) * Pitch + Size : 0.f;
}

float ReelPips(UCanvas* C, float S, float X, float Y, int32 Have, int32 Total, const FLinearColor& Tint, const FLinearColor& Empty, float A, float PitchKit)
{
	const float Kp = KitPx(S), Size = 32.f * Kp, Pitch = PitchKit * Kp;
	for (int32 I = 0; I < Total; ++I)
	{
		TexCell(C, ETex::HudPips, I < Have ? 2 : 3, X + static_cast<float>(I) * Pitch, Y, Size, Size, I < Have ? Tint : Empty, A, false);
	}
	return Total > 0 ? static_cast<float>(Total - 1) * Pitch + Size : 0.f;
}

void Pip(UCanvas* C, EPip Cell, float X, float Y, float W, float H, const FLinearColor& Tint, float A)
{
	TexCell(C, ETex::HudPips, static_cast<int32>(Cell), X, Y, W, H, Tint, A, false);
}

void FilmMeter(UCanvas* C, float S, float X, float Y, float W, float H, float Fraction, int32 Segments, const FLinearColor& Tint, const FLinearColor& Empty, float A)
{
	const float Kp = KitPx(S);
	Box(C, X, Y, W, H, FilmBase, A);
	const float Perf = FMath::RoundToFloat(FMath::Max(6.f * Kp, H * 0.24f));
	const float PerfScale = Kp > 0.f ? Perf / (16.f * Kp) : 1.f;      // one perforation is 16 kit px: scaled to the row's height
	TexTiled(C, S, ETex::FilmPerf, X + 2.f * Kp, Y, W - 4.f * Kp, Perf, White, A, PerfScale, true, false);
	TexTiled(C, S, ETex::FilmPerf, X + 2.f * Kp, Y + H - Perf, W - 4.f * Kp, Perf, White, A, PerfScale, true, false);
	if (Segments <= 0) return;
	const float Between = 3.f * Kp, Side = 3.f * Kp;
	const float FrameW = (W - 2.f * Side - Between * static_cast<float>(Segments - 1)) / static_cast<float>(Segments);
	const float Filled = FMath::Clamp(Fraction, 0.f, 1.f) * static_cast<float>(Segments);
	for (int32 I = 0; I < Segments; ++I)
	{
		const float FrameX = X + Side + static_cast<float>(I) * (FrameW + Between);
		Box(C, FrameX, Y + Perf, FrameW, H - 2.f * Perf, Empty, A);
		const float Part = FMath::Clamp(Filled - static_cast<float>(I), 0.f, 1.f);
		if (Part > 0.f) Box(C, FrameX, Y + Perf, FrameW * Part, H - 2.f * Perf, Tint, A);
	}
}

void FilmMeterMark(UCanvas* C, float S, float X, float Y, float W, float H, float At01, const FLinearColor& Color, float A)
{
	const float Kp = KitPx(S), Side = 3.f * Kp, Wide = Line(3.f, Kp);
	Box(C, X + Side + (W - 2.f * Side) * FMath::Clamp(At01, 0.f, 1.f) - Wide * 0.5f, Y - 3.f * Kp, Wide, H + 6.f * Kp, Color, A);
}

void HudMeter(UCanvas* C, float S, float X, float Y, float W, float H, float Fraction, int32 Segments, const FLinearColor& Tint, const FLinearColor& TrackColor, float A, float Ghost)
{
	if (Segments <= 0 || !C) return;
	const float Kp = KitPx(S), Between = 4.f * Kp, Pad = 3.f * Kp, Lean = H * 0.35f;
	// (15 Nov, the frame-cost pass.) The whole meter is ONE canvas item: the bed's and every segment's triangles, in
	// the order they were drawn one parallelogram at a time. They always shared the white texture and the blend
	// mode, so they were one batch of the canvas already and the triangles reach it unchanged; what goes is an
	// item and two allocations a parallelogram (the armour bar alone, when full, drew 37 of them every frame).
	FCanvasTriangleItem It(FVector2D::ZeroVector, FVector2D::ZeroVector, FVector2D::ZeroVector, GWhiteTexture);
	TArray<FCanvasUVTri>& Tris = It.TriangleList;
	Tris.Reset(2 + 8 * FMath::Min(Segments, 64));       // (the constructor's own triangle goes; room for the usual meter at once)
	ParaTris(Tris, X - Pad, Y - Pad, W + 2.f * Pad + Lean, H + 2.f * Pad, (H + 2.f * Pad) * 0.35f, Ink, A);
	const float Pitch = (W + Between) / static_cast<float>(Segments), SegW = Pitch - Between;
	const float Filled = FMath::Clamp(Fraction, 0.f, 1.f) * static_cast<float>(Segments);
	const float Trail = FMath::Clamp(Ghost, 0.f, 1.f) * static_cast<float>(Segments);
	const FLinearColor Light = Mix(Tint, White, 0.45f);
	for (int32 I = 0; I < Segments; ++I)
	{
		const float SegX = X + static_cast<float>(I) * Pitch;
		ParaTris(Tris, SegX, Y, SegW + Lean, H, Lean, TrackColor, A);
		const float Part = FMath::Clamp(Filled - static_cast<float>(I), 0.f, 1.f);
		const float Was = FMath::Clamp(Trail - static_cast<float>(I), 0.f, 1.f);
		if (Was > Part) ParaTris(Tris, SegX, Y, SegW * Was + Lean, H, Lean, White, A * 0.85f);
		if (Part > 0.f)
		{
			ParaTris(Tris, SegX, Y, SegW * Part + Lean, H, Lean, Tint, A);
			ParaTris(Tris, SegX + Lean * 0.7f, Y, SegW * Part + Lean * 0.3f, H * 0.3f, Lean * 0.3f, Light, A);    // the cel highlight
		}
	}
	if (Tris.Num() == 0) return;        // (nothing was visible: nothing was drawn before either)
	It.BlendMode = SE_BLEND_Translucent;
	C->DrawItem(It);
}

void BlockMeter(UCanvas* C, float S, float X, float Y, float W, float H, float Fraction, int32 Segments, const FLinearColor& Tint, const FLinearColor& TrackColor, float A, float GapKit)
{
	if (Segments <= 0) return;
	const float Between = Segments > 1 ? GapKit * KitPx(S) : 0.f;
	const float BlockW = (W - Between * static_cast<float>(Segments - 1)) / static_cast<float>(Segments);
	const float Filled = FMath::Clamp(Fraction, 0.f, 1.f) * static_cast<float>(Segments);
	for (int32 I = 0; I < Segments; ++I)
	{
		const float BlockX = X + static_cast<float>(I) * (BlockW + Between);
		Box(C, BlockX, Y, BlockW, H, TrackColor, A);
		const float Part = FMath::Clamp(Filled - static_cast<float>(I), 0.f, 1.f);
		if (Part > 0.f) Box(C, BlockX, Y, BlockW * Part, H, Tint, A);
	}
}

// ---------------------------------------------------------------------------------------------------- HUD chips
void HudChip(UCanvas* C, float X, float Y, float W, float H, const FLinearColor& Tint, float A)
{
	ThreeSliceH(C, ETex::HudChip, X, Y, W, H, Tint, A);
}

float HudTag(UCanvas* C, float X, float Y, float W, float H, const FLinearColor& Tint, float A)
{
	ThreeSliceH(C, ETex::HudTag, X, Y, W, H, Tint, A);
	return X + 30.f * H / 44.f;
}

// ---------------------------------------------------------------------------------------------------- shoots and hero art
float Board(UCanvas* C, float S, int32 Shoot0To7, float X, float Y, float W, float A, float OutlineKit)
{
	const float Out = Line(OutlineKit, KitPx(S));
	const float H = W * 216.f / 384.f;
	Box(C, X - Out, Y - Out, W + 2.f * Out, H + 2.f * Out, Ink, A);
	TexCell(C, ETex::Boards, FMath::Clamp(Shoot0To7, 0, 7), X, Y, W, H, White, A, false);
	return H;
}

float BoardThumb(UCanvas* C, float S, int32 Shoot0To7, float X, float Y, float W, float A, float OutlineKit)
{
	const float Out = Line(OutlineKit, KitPx(S));
	const float H = W * 72.f / 128.f;
	Box(C, X - Out, Y - Out, W + 2.f * Out, H + 2.f * Out, Ink, A);
	TexCell(C, ETex::BoardThumbs, FMath::Clamp(Shoot0To7, 0, 7), X, Y, W, H, White, A, false);
	return H;
}

FVector2D Lockup(UCanvas* C, float S, int32 Shoot0To7, float X, float Y, float A, float Scale)
{
	return TexCellNat(C, S, ETex::Lockups, FMath::Clamp(Shoot0To7, 0, 7), X, Y, White, A, Scale);
}

FVector2D KestrelHero(UCanvas* C, float S, float X, float Y, float A, float Scale, bool bBurst)
{
	const float Kp = KitPx(S) * Scale;
	if (bBurst)
	{
		Tex(C, ETex::HeroBurst, X + (768.f - 300.f) * Kp, Y - 150.f * Kp, 352.f * Kp, 352.f * Kp, White, A, false, false);
	}
	Tex(C, ETex::KestrelHero, X, Y, 768.f * Kp, 304.f * Kp, White, A, false, false);
	return FVector2D(768.f * Kp, 304.f * Kp);
}

void ChaseSprite(UCanvas* C, float S, EChase Who, float CX, float GroundY, float A, float Scale)
{
	const float Kp = KitPx(S) * Scale;      // the cell is 320 x 128; the wheels stand on its row 120
	TexCell(C, ETex::Chase, static_cast<int32>(Who), CX - 160.f * Kp, GroundY - 120.f * Kp, 320.f * Kp, 128.f * Kp, White, A, false);
}

float ChaseStrip(UCanvas* C, float S, float X, float Y, float W, float H, float Car01, float A)
{
	const float Kp = KitPx(S);
	const float RuleH = Line(16.f, Kp), RuleY = Y + H - RuleH, Lip = Line(2.f, Kp);
	const float SkyH = FMath::RoundToFloat(128.f * 0.75f * Kp);
	TexTiled(C, S, ETex::ChaseSkyline, X, RuleY - SkyH, W, SkyH, White, A, 0.75f, true, false);
	Box(C, X, RuleY - Lip, W, RuleH + Lip, Ink, A);
	TexTiled(C, S, ETex::ClapStripes, X, RuleY, W, RuleH - Lip, Chalk, A, 0.5f);
	ChaseSprite(C, S, EChase::Gunship, X + W * 0.82f, RuleY - 26.f * Kp, A);
	ChaseSprite(C, S, EChase::Sedan, X + W * 0.12f, RuleY, A);
	ChaseSprite(C, S, EChase::Truck, X + W * 0.30f, RuleY, A);
	ChaseSprite(C, S, EChase::Kestrel, X + W * FMath::Clamp(Car01, 0.f, 1.f), RuleY, A);
	return RuleY;
}

FVector2D Cheque(UCanvas* C, float S, float X, float Y, float A)
{
	return TexNat(C, S, ETex::Cheque, X, Y, White, A);
}

FBox2D ChequeZone(float S, float X, float Y, EChequeZone Zone)
{
	// x, y, w, h in texture px (KIT.md, T_UI_Cheque), in the enum's order
	static const float Zones[6][4] =
	{
		{ 16.f, 16.f, 100.f, 200.f },       // Stub
		{ 150.f, 62.f, 440.f, 34.f },       // Payee
		{ 612.f, 52.f, 168.f, 46.f },       // Amount
		{ 150.f, 112.f, 630.f, 30.f },      // Words
		{ 150.f, 164.f, 250.f, 30.f },      // Memo
		{ 520.f, 150.f, 256.f, 60.f },      // Signature
	};
	const float* Z = Zones[FMath::Clamp(static_cast<int32>(Zone), 0, 5)];
	const float Kp = KitPx(S);
	return FBox2D(FVector2D(X + Z[0] * Kp, Y + Z[1] * Kp), FVector2D(X + (Z[0] + Z[2]) * Kp, Y + (Z[1] + Z[3]) * Kp));
}

void Signature(UCanvas* C, float S, float X, float Y, const FLinearColor& Tint, float A, float Scale)
{
	TexNat(C, S, ETex::Signature, X, Y, Tint, A, Scale);
}

// ---------------------------------------------------------------------------------------------------- big numerals
float MeasureDigits(const FString& Str, float CapH)
{
	const FDigitSet& D = DigitSet(CapH);
	const float Kd = CapH / D.Cap;
	float Total = 0.f;
	for (int32 I = 0; I < Str.Len(); ++I)
	{
		const int32 N = DigitIndex(Str[I]);
		if (N >= 0) Total += D.Adv[N] * Kd;
	}
	return Total;
}

float Digits(UCanvas* C, const FString& Str, float X, float Baseline, float CapH, const FLinearColor& Fill, float A, int32 Align, bool bInk, const FLinearColor& InkTint)
{
	const float Total = MeasureDigits(Str, CapH);
	if (!C || CapH <= 0.f || Str.IsEmpty()) return Total;
	const FDigitSet& D = DigitSet(CapH);
	const float Kd = CapH / D.Cap;
	const float Start = X - (Align == 1 ? Total * 0.5f : Align == 2 ? Total : 0.f);
	const float Keep = Kd > 1.01f ? 0.5f : 0.f;
	const float CellY = Snap(Baseline - D.PenY * Kd);
	// the ink cells of the whole string first, then the fills: neighbours' outlines merge under the fills
	for (int32 Pass = bInk ? 0 : 1; Pass < 2; ++Pass)
	{
		const ETex T = Pass == 0 ? ETex::DigitsInk : ETex::DigitsFill;
		const FLinearColor Col = WithA(Pass == 0 ? InkTint : Fill, A);
		float Pen = Start;
		for (int32 I = 0; I < Str.Len(); ++I)
		{
			const int32 N = DigitIndex(Str[I]);
			if (N < 0) continue;
			if (N != DigitSpace)
			{
				const float CellX = Snap(Pen - D.PenX * Kd);
				Sub(C, T, D.AtX + static_cast<float>(N % 10) * D.CW, D.AtY + static_cast<float>(N / 10) * D.CH, D.CW, D.CH,
					CellX, CellY, CellX + D.CW * Kd, CellY + D.CH * Kd, Col, Keep, Keep);
			}
			Pen += D.Adv[N] * Kd;
		}
	}
	return Total;
}
}

#if !UE_BUILD_SHIPPING
namespace
{
	// "FTSkinPreload" in the console: looks for the kit again and logs "FT SKIN kit textures=N of 56 bungee=0/1".
	// The import job (SourceArt/UI/kit_import_job.py) ends with it; in a running editor it picks up a fresh import.
	FAutoConsoleCommandWithWorldAndArgs GFTSkinPreload(TEXT("FTSkinPreload"), TEXT("FULL THROTTLE UI kit: load its textures again and log how many of the 56 were found"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>&, UWorld*)
		{
			FTSkin::Reload();
			FTSkin::Preload();
		}));
}
#endif
