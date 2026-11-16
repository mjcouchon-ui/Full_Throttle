#include "FTTitleLogo.h"
#include "FTCore.h"
#include "FTGameMode.h"
#include "FTGameInstance.h"
#include "FTPlayerController.h"
#include "FTProgress.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"
#include "CanvasItem.h"
#include "CanvasTypes.h"
#if WITH_EDITOR
#include "TextureCompiler.h"   // an uncooked game shows 32 x 32 placeholders until a texture is built
#endif
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "HAL/IConsoleManager.h"
#include "Math/RotationMatrix.h"
#include "Math/ScaleMatrix.h"
#include "Math/TranslationMatrix.h"
#include "Misc/App.h"
#include "UObject/UObjectGlobals.h"

static TAutoConsoleVariable<int32> CVarFTTitleLogo(TEXT("ft.TitleLogo"), 1, TEXT("FULL THROTTLE: the title's animated logo. 0 = off (the flat logo, drawn exactly as before, no sound); 1 = on; 2 = test: the new draw path held at rest"));
static TAutoConsoleVariable<float> CVarFTTitleLogoVol(TEXT("ft.TitleLogoVol"), 1.f, TEXT("FULL THROTTLE: multiplies every sound of the title's logo, the hum included"));
static TAutoConsoleVariable<float> CVarFTTitleLogoHum(TEXT("ft.TitleLogoHum"), .035f, TEXT("FULL THROTTLE: the volume of the neon hum under the title (0 = none)"));
static TAutoConsoleVariable<int32> CVarFTTitleLogoIdleChime(TEXT("ft.TitleLogoIdleChime"), 1, TEXT("FULL THROTTLE: 0 = the title logo's idle twinkles are silent"));
static TAutoConsoleVariable<int32> CVarFTTitleLogoSweep(TEXT("ft.TitleLogoSweep"), 1, TEXT("FULL THROTTLE: 0 = no light sweep across the title logo's chrome"));
static TAutoConsoleVariable<int32> CVarFTTitleLogoRearm(TEXT("ft.TitleLogoRearm"), 1, TEXT("FULL THROTTLE: 1 = the title's logo comes alive again 0.6 s after a confirm that left the player on the title; 0 = it stays steady"));
static TAutoConsoleVariable<int32> CVarFTTitleLogoLog(TEXT("ft.TitleLogoLog"), 0, TEXT("FULL THROTTLE tests: 1 = log each idle flicker, twinkle and sweep of the title's logo"));

namespace
{
	// The layers (Tools/title/make_layers.py, DESIGN 2.2). Cover: the rectangle each is drawn over, in logo px
	// (pixels of the 1600 x 608 flat canvas).
	enum : int32 { LThrottle, LLines, LFull, LStar1, LStar2, LStar1Lit, LStar2Lit, LFullSoft, LFullGlow, LFullGlowIn, LThrottleLit, LCount };
	struct FLayerDef { const TCHAR* Name; int32 W, H; float Cx, Cy, Cw, Ch; };
	const FLayerDef GLayers[LCount] = {
		{ TEXT("Throttle"),    1600, 320,    0.f,  256.f, 1600.f, 320.f },
		{ TEXT("Lines"),       1600, 128,    0.f,  480.f, 1600.f, 128.f },
		{ TEXT("Full"),         768, 416,   48.f,    0.f,  768.f, 416.f },
		{ TEXT("Star1"),        128, 128,  370.f,  273.f,  128.f, 128.f },
		{ TEXT("Star2"),        128, 128, 1232.f,  342.f,  128.f, 128.f },
		{ TEXT("Star1Lit"),     128, 128,  370.f,  273.f,  128.f, 128.f },
		{ TEXT("Star2Lit"),     128, 128, 1232.f,  342.f,  128.f, 128.f },
		{ TEXT("FullSoft"),     240, 160,  -48.f, -112.f,  960.f, 640.f },
		{ TEXT("FullGlow"),     240, 160,  -48.f, -112.f,  960.f, 640.f },
		{ TEXT("FullGlowIn"),   228, 132,    0.f,    0.f,  912.f, 528.f },
		{ TEXT("ThrottleLit"),  800, 176,    0.f,  256.f, 1600.f, 352.f },
	};
	static_assert(LCount == UFTTitleLogo::NumLayers, "the layer table and the subsystem's array are one list");
	const TCHAR* const GSounds[] = { TEXT("TitleSlam"), TEXT("TitleWipe"), TEXT("TitleTwinkle"), TEXT("TitleTwinklePair"), TEXT("TitleNeonTick"), TEXT("TitleNeonHum") };

	constexpr float LogoW = 1600.f, LogoH = 608.f;
	constexpr float Slant = .2f, SlantY = 304.f;                 // the letters' lean: x_e(y) = E - 0.2 (y - 304)
	const FVector2D FullPivot(434.9f, 189.1f);                   // the centroid of "Full"'s lettering (measured)
	const FVector2D StarAt[2] = { FVector2D(434.f, 337.f), FVector2D(1296.f, 406.f) };

	// The intro, seconds from its zero (DESIGN 4.4).
	constexpr float Flight = .18f, Overshoot = .10f, FlareTime = .20f, ShakeTime = 3.f / 60.f, WipeTime = .35f;
	constexpr float StarStart[2] = { .54f, .64f }, StarGrow = .117f, StarSettle = .133f;
	constexpr float IntroEnd = .90f;

	// Alive: fixed patterns, seconds from the alive state's zero (DESIGN 4.5). No random calls anywhere.
	constexpr double FlickerCycle = 61.0, DipTime = .034, DipGap = .06;   // (a dip: two frames at 60 fps, one at 30)
	constexpr double FlickerAt[] = { 6.3, 15.4, 20.1, 31.9, 39.3, 44.5, 57.5 };
	constexpr int32 FlickerKind[] = { 1, 2, 1, 1, 2, 1, 2 };
	constexpr double TwinkleCycle = 72.0, BigTime = .42, FaintTime = .32;
	struct FTwinkle { double At; bool bBig; };
	// (star 2's first big twinkle is at 8.9 s, not later: the attract demo's first cut loads a map at about 10.6 s alive)
	constexpr FTwinkle Twinkles[2][13] = {
		{ { 3.7, true }, { 9.6, false }, { 13.9, false }, { 21.0, true }, { 24.1, false }, { 30.5, false }, { 35.4, true }, { 39.5, false }, { 46.1, false }, { 49.5, true }, { 56.8, false }, { 62.0, true }, { 66.6, false } },
		{ { 7.2, false }, { 8.9, true }, { 17.3, false }, { 22.0, false }, { 29.0, true }, { 32.9, false }, { 38.4, false }, { 43.0, true }, { 50.4, false }, { 52.8, false }, { 59.8, false }, { 64.2, true }, { 69.4, false } } };
	constexpr double SweepFirst = 5.0, SweepEvery = 9.5, SweepTime = .7;
	constexpr float SweepGain = .13f;
	constexpr double ConfirmTwinkle = .45;

	/** How many of the starts First, First + Cycle, ... lie at or before Time. */
	double Starts(double First, double Cycle, double Time) { return Time < First ? 0.0 : FMath::FloorToDouble((Time - First) / Cycle) + 1.0; }

	/** Something that begins at First and again every Cycle: false before its first start; else Start is its latest
	 *  start at or before Now, and bFired says that start was crossed since Prev. Both times go through the same
	 *  count, so a start that falls exactly on a frame's time fires once and only once, whatever the rounding. */
	bool Latest(double First, double Cycle, double Prev, double Now, double& Start, bool& bFired)
	{
		const double N = Starts(First, Cycle, Now);
		if (N < 1.0) return false;
		Start = First + Cycle * (N - 1.0);
		bFired = N > Starts(First, Cycle, Prev);
		return true;
	}

	bool LogIdle() { return CVarFTTitleLogoLog.GetValueOnGameThread() > 0; }
}

// ------------------------------------------------------------------------------------------------ drawing
/** The logo's rectangle on the canvas and the few ways its layers are put down. Logo px to screen: (X + x k, Y + y k). */
struct UFTTitleLogo::FPaint
{
	UCanvas* Canvas;
	const UFTTitleLogo& Logo;
	UTexture2D* Flat;
	float X, Y, W, H, Alpha;

	float Kx() const { return W / LogoW; }
	float Ky() const { return H / LogoH; }
	FVector2D At(float Lx, float Ly) const { return FVector2D(X + Lx * Kx(), Y + Ly * Ky()); }
	const FTexture* Resource(int32 Layer) const { return Logo.Tex[Layer] ? Logo.Tex[Layer]->GetResource() : nullptr; }

	/** The flat logo: the HUD's own item, from the HUD's own floats, so its vertices and colour are what they always were. */
	void Base() const
	{
		FCanvasTileItem Item(FVector2D(X, Y), Flat->GetResource(), FVector2D(W, H), FLinearColor(1.f, 1.f, 1.f, Alpha));
		Item.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(Item);
	}

	/** A layer over its cover, tinted. */
	void Tile(int32 Layer, const FLinearColor& Colour, ESimpleElementBlendMode Blend) const
	{
		const FTexture* Res = Resource(Layer);
		if (!Res) return;
		const FLayerDef& D = GLayers[Layer];
		FCanvasTileItem Item(At(D.Cx, D.Cy), Res, FVector2D(D.Cw * Kx(), D.Ch * Ky()), Colour);
		Item.BlendMode = Blend;
		Canvas->DrawItem(Item);
	}
	void Tile(int32 Layer, float Opacity = 1.f) const { if (Opacity > .002f) Tile(Layer, FLinearColor(1.f, 1.f, 1.f, Opacity * Alpha), SE_BLEND_Translucent); }

	/** Light added from a "lit" layer (its colours already multiplied by its alpha, on black). A gain over 1 is two
	 *  passes, so nothing rests on vertex colours above 1. */
	void Light(int32 Layer, float Gain) const
	{
		for (float Left = Gain * Alpha; Left > .004f; Left -= 1.f)
		{
			const float G = FMath::Min(Left, 1.f);
			Tile(Layer, FLinearColor(G, G, G, 1.f), SE_BLEND_Additive);
		}
	}

	/** A star (Layer: its plain or its lit texture) about its own centre: scaled, turned clockwise by Degrees. */
	void Star(int32 Layer, int32 Which, float Scale, float Degrees, float Amount, bool bLight) const
	{
		const FTexture* Res = Resource(Layer);
		if (!Res || Scale < .02f) return;
		const FVector2D Size(128.f * Kx() * Scale, 128.f * Ky() * Scale);
		for (float Left = Amount * Alpha; Left > .004f; Left -= 1.f)
		{
			const float G = FMath::Min(Left, 1.f);
			FCanvasTileItem Item(At(StarAt[Which].X, StarAt[Which].Y) - Size * .5f, Res, Size, bLight ? FLinearColor(G, G, G, 1.f) : FLinearColor(1.f, 1.f, 1.f, G));
			Item.BlendMode = bLight ? SE_BLEND_Additive : SE_BLEND_Translucent;
			Item.Rotation = FRotator(0.f, Degrees, 0.f);
			Item.PivotPoint = FVector2D(.5f, .5f);
			Canvas->DrawItem(Item);
			if (!bLight) break;
		}
	}

	/** A layer uncovered from its left edge up to the slanted edge E (logo px at the logo's mid height). The letters
	 *  do not move: the texture stays where it is and only the quad's right side travels. */
	void Wiped(int32 Layer, float E) const
	{
		const FTexture* Res = Resource(Layer);
		if (!Res) return;
		const FLayerDef& D = GLayers[Layer];
		const float Top = FMath::Clamp(E - Slant * (D.Cy - SlantY), D.Cx, D.Cx + D.Cw);
		const float Bottom = FMath::Clamp(E - Slant * (D.Cy + D.Ch - SlantY), D.Cx, D.Cx + D.Cw);
		if (Top <= D.Cx && Bottom <= D.Cx) return;
		if (Top >= D.Cx + D.Cw && Bottom >= D.Cx + D.Cw) { Tile(Layer); return; }
		const FLinearColor Colour(1.f, 1.f, 1.f, Alpha);   // (a triangle item takes its colour from its vertices)
		const auto Vertex = [&](float Lx, float Ly, FVector2D& Pos, FVector2D& UV, FLinearColor& Col)
		{
			Pos = At(Lx, Ly); UV = FVector2D((Lx - D.Cx) / D.Cw, (Ly - D.Cy) / D.Ch); Col = Colour;
		};
		TArray<FCanvasUVTri> Quad;
		FCanvasUVTri A, B;
		Vertex(D.Cx, D.Cy, A.V0_Pos, A.V0_UV, A.V0_Color); Vertex(Top, D.Cy, A.V1_Pos, A.V1_UV, A.V1_Color); Vertex(Bottom, D.Cy + D.Ch, A.V2_Pos, A.V2_UV, A.V2_Color);
		Vertex(D.Cx, D.Cy, B.V0_Pos, B.V0_UV, B.V0_Color); Vertex(Bottom, D.Cy + D.Ch, B.V1_Pos, B.V1_UV, B.V1_Color); Vertex(D.Cx, D.Cy + D.Ch, B.V2_Pos, B.V2_UV, B.V2_Color);
		Quad.Add(A); Quad.Add(B);
		FCanvasTriangleItem Item(Quad, Res);
		Item.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(Item);
	}

	/** White light on the chrome and the underlines only (the ThrottleLit mask), in a slanted band from edge E0 (gain G0)
	 *  to edge E1 (gain G1). Clamped to the logo's width, with the gain taken at the clamped place, so it never leaves
	 *  the logo's rectangle. */
	void Band(float E0, float G0, float E1, float G1) const
	{
		const FTexture* Res = Resource(LThrottleLit);
		if (!Res || E1 <= E0) return;
		const FLayerDef& D = GLayers[LThrottleLit];
		const auto Vertex = [&](float E, float Ly, FVector2D& Pos, FVector2D& UV, FLinearColor& Col)
		{
			const float Shift = -Slant * (Ly - SlantY);
			const float Lx = FMath::Clamp(E + Shift, D.Cx, D.Cx + D.Cw);
			const float G = FMath::Lerp(G0, G1, FMath::Clamp((Lx - (E0 + Shift)) / (E1 - E0), 0.f, 1.f)) * Alpha;
			Pos = At(Lx, Ly); UV = FVector2D((Lx - D.Cx) / D.Cw, (Ly - D.Cy) / D.Ch); Col = FLinearColor(G, G, G, 1.f);
		};
		if (E1 - Slant * (D.Cy + D.Ch - SlantY) <= D.Cx && E1 - Slant * (D.Cy - SlantY) <= D.Cx) return;            // all of it left of the logo
		if (E0 - Slant * (D.Cy + D.Ch - SlantY) >= D.Cx + D.Cw && E0 - Slant * (D.Cy - SlantY) >= D.Cx + D.Cw) return;   // ... or right of it
		TArray<FCanvasUVTri> Quad;
		FCanvasUVTri A, B;
		Vertex(E0, D.Cy, A.V0_Pos, A.V0_UV, A.V0_Color); Vertex(E1, D.Cy, A.V1_Pos, A.V1_UV, A.V1_Color); Vertex(E1, D.Cy + D.Ch, A.V2_Pos, A.V2_UV, A.V2_Color);
		Vertex(E0, D.Cy, B.V0_Pos, B.V0_UV, B.V0_Color); Vertex(E1, D.Cy + D.Ch, B.V1_Pos, B.V1_UV, B.V1_Color); Vertex(E0, D.Cy + D.Ch, B.V2_Pos, B.V2_UV, B.V2_Color);
		Quad.Add(A); Quad.Add(B);
		FCanvasTriangleItem Item(Quad, Res);
		Item.BlendMode = SE_BLEND_Additive;
		Canvas->DrawItem(Item);
	}
};

/** The intro at Tau seconds from its zero: the layers, in the flat logo's own order (chrome, stars, "Full", underlines). */
void UFTTitleLogo::DrawIntro(FPaint& P, float Tau, float S, float W, float FrameTime) const
{
	FCanvas* C = P.Canvas->Canvas;
	const float Hit = Tau - Flight;   // seconds since "Full" landed

	// the hit shakes the logo, and only the logo: three steps of 1/60 s (the trailer's own), 0.05 s at any frame rate,
	// and never fewer than two drawn frames (the second frame after the landing always shakes): three frames at
	// 60 fps, two at 30
	const bool bShake = Hit >= 0.f && (Hit < ShakeTime || Hit < 2.f * FMath::Min(FrameTime, .1f));
	if (bShake)
	{
		static const FVector2D Steps[3] = { FVector2D(2.94f, -2.29f), FVector2D(-1.96f, 1.63f), FVector2D(.98f, -.65f) };
		const FVector2D Step = Steps[FMath::Clamp(FMath::FloorToInt(Hit * 60.f), 0, 2)] * S;
		C->PushRelativeTransform(FTranslationMatrix(FVector(FMath::RoundToFloat(Step.X), FMath::RoundToFloat(Step.Y), 0.f)));
	}

	// THROTTLE and the underlines are uncovered left to right from the instant "Full" lands
	const bool bWiping = Hit >= 0.f && Hit < WipeTime;
	const float Edge = -70.f + 1740.f * (1.f - FMath::Pow(1.f - FMath::Clamp(Hit / WipeTime, 0.f, 1.f), 1.6f));
	if (bWiping)
	{
		P.Wiped(LThrottle, Edge);
		P.Band(Edge - 90.f, 0.f, Edge, .9f);   // the hard bright leading edge
	}
	else if (Hit >= WipeTime) P.Tile(LThrottle);

	// the stars twinkle on, the left one first: up from nothing with a quarter turn, a flare, and settle
	for (int32 I = 0; I < 2; ++I)
	{
		const float T = Tau - StarStart[I];
		if (T < 0.f) continue;
		float Scale = 1.f, Turn = 0.f, Flare = 0.f;
		if (T < StarGrow) { const float U = T / StarGrow; Scale = 1.55f * (1.f - FMath::Square(1.f - U)); Turn = 90.f * (1.f - U); Flare = 1.4f * U; }
		else if (T < StarGrow + StarSettle) { const float U = (T - StarGrow) / StarSettle; Scale = 1.55f - .55f * U; Flare = 1.4f * (1.f - U); }
		P.Star(LStar1 + I, I, Scale, Turn, 1.f, false);
		P.Star(LStar1Lit + I, I, Scale, Turn, Flare, true);
	}

	// "Full"
	const FVector2D Pivot = P.At(FullPivot.X, FullPivot.Y);
	if (Hit < 0.f)
	{
		// in flight, thrown from behind the camera: far too large and soft, shrinking fast with a spin that snaps
		// straight. Three shutter samples 1/180 s apart in real time, so the smear is the same at any frame rate.
		for (int32 J = 2; J >= 0; --J)
		{
			const float U = FMath::Clamp((Tau - J / 180.f) / Flight, 0.f, 1.f);
			const float K = FMath::Pow(1.f - U, 2.2f);
			// (never fainter than 0.35 in all: on film a throw that began from nothing read as a haze, not as lettering)
			const float Each = 1.f - FMath::Pow(FMath::Min(K, .65f), 1.f / 3.f);   // three of these laid over each other give 1 - K
			if (Each <= .002f) continue;
			const float Soft = FMath::SmoothStep(.02f, .20f, K);
			const float Scale = 1.f + 10.5f * K;
			C->PushRelativeTransform(FTranslationMatrix(FVector(-Pivot.X, -Pivot.Y, 0.f)) * FRotationMatrix(FRotator(0.f, 24.06f * K, 0.f)) * FScaleMatrix(FVector(Scale, Scale, 1.f))
				* FTranslationMatrix(FVector(Pivot.X + .40625f * W * K, Pivot.Y + .296875f * W * K, 0.f)));
			P.Tile(LFullSoft, Each * Soft);
			P.Tile(LFull, Each * (1.f - Soft));
			C->PopTransform();
		}
	}
	else if (Hit < Overshoot)
	{
		// stops hard, one small overshoot
		const float Scale = 1.f - .07f * FMath::Exp(-Hit / .01833f) * FMath::Cos(114.f * Hit);
		C->PushRelativeTransform(FTranslationMatrix(FVector(-Pivot.X, -Pivot.Y, 0.f)) * FScaleMatrix(FVector(Scale, Scale, 1.f)) * FTranslationMatrix(FVector(Pivot.X, Pivot.Y, 0.f)));
		P.Tile(LFull);
		C->PopTransform();
	}
	else P.Tile(LFull);

	// a brief pink flare on the hit
	if (Hit >= 0.f && Hit < FlareTime) P.Light(LFullGlow, 1.6f * FMath::Exp(-Hit / .05f));

	if (bWiping) P.Wiped(LLines, Edge);
	else if (Hit >= WipeTime) P.Tile(LLines);

	if (bShake) C->PopTransform();
}

/** Alive: on top of the flat logo, and never outside its rectangle. Events fire on the frame their time is crossed, and
 *  a flicker's dip is drawn on that frame whatever the frame rate (with its tick), then for as long as it lasts. */
void UFTTitleLogo::DrawAlive(FPaint& P, const AFTGameMode* G, bool bReducedMotion)
{
	AliveNow += ClockStep;
	// (never more than a quarter second looked back over, whatever happened to the clock: a late chime is worse than none)
	const double Now = AliveNow, Prev = FMath::Clamp(PrevAlive, Now - .25, Now);
	PrevAlive = Now;
	const float Ramp = float(FMath::Clamp((Now - RampFrom) / .6, 0.0, 1.0));
	// no idle sound into the attract demo's wipe-out: the map load that follows would cut it off
	const bool bQuiet = G->bAttract && G->AttractOutT >= 0.f;
	double Start = 0.0;
	bool bFired = false;

	// the neon tube: a rare short flicker (the glow goes and the tube dips for a frame or two, never dark)
	bool bDip = false;
	if (!bReducedMotion)
	{
		for (int32 I = 0; I < UE_ARRAY_COUNT(FlickerAt); ++I)
		{
			for (int32 D = 0; D < FlickerKind[I]; ++D)
			{
				if (!Latest(FlickerAt[I] + D * (DipTime + DipGap), FlickerCycle, Prev, Now, Start, bFired)) continue;
				if (Start < DrawFrom) continue;
				if (bFired)
				{
					if (!bQuiet) Cue(G, TEXT("TitleNeonTick"), .12f, 1.f, EVoice::OneShot);
					if (D == 0 && LogIdle()) { UE_LOG(LogFT, Display, TEXT("FT TITLE logo flicker n=%d kind=%d a=%.2f"), I, FlickerKind[I], Now); }
				}
				if (bFired || Now < Start + DipTime) bDip = true;   // (always the frame its tick sounds on, then while it lasts)
			}
		}
	}
	if (bDip) P.Tile(LFull, FLinearColor(.45f, .45f, .45f, .7f * P.Alpha), SE_BLEND_Translucent);
	else
	{
		// ... and its slow breathing: light added, so the lettering never dims from it
		const float A = float(Now);
		P.Light(LFullGlowIn, Ramp * (.16f + .10f * FMath::Sin(2.f * PI * A / 3.1f - PI * .5f) + .03f * FMath::Sin(2.f * PI * A / 7.3f)));
	}
	if (bReducedMotion) return;

	// the stars glint again, each on its own fixed list of times: sometimes a big twinkle, sometimes a faint one
	for (int32 I = 0; I < 2; ++I)
	{
		for (const FTwinkle& T : Twinkles[I])
		{
			if (!Latest(T.At, TwinkleCycle, Prev, Now, Start, bFired)) continue;
			if (Start < DrawFrom) continue;
			if (bFired)
			{
				if (T.bBig && !bQuiet && CVarFTTitleLogoIdleChime.GetValueOnGameThread() > 0) Cue(G, TEXT("TitleTwinkle"), .09f, I == 0 ? 1.f : 1.122f, EVoice::OneShot);
				if (LogIdle()) { UE_LOG(LogFT, Display, TEXT("FT TITLE logo twinkle star=%d big=%d a=%.2f"), I + 1, T.bBig ? 1 : 0, Now); }
			}
			const double Time = T.bBig ? BigTime : FaintTime;
			if (Now - Start >= Time) continue;
			const float U = float((Now - Start) / Time), Glint = FMath::Sin(PI * U);
			if (T.bBig) P.Star(LStar1Lit + I, I, 1.f + .55f * Glint, 45.f * (1.f - U), Glint, true);
			else P.Star(LStar1Lit + I, I, 1.f + .15f * Glint, 0.f, .35f * Glint, true);
		}
	}

	// a faint light across the chrome now and then
	if (CVarFTTitleLogoSweep.GetValueOnGameThread() > 0 && Latest(SweepFirst, SweepEvery, Prev, Now, Start, bFired))
	{
		if (bFired && LogIdle()) { UE_LOG(LogFT, Display, TEXT("FT TITLE logo sweep a=%.2f"), Now); }
		if (Start >= DrawFrom && Now - Start < SweepTime)
		{
			const float Centre = -100.f + 1800.f * float((Now - Start) / SweepTime);
			P.Band(Centre - 70.f, 0.f, Centre, SweepGain);   // (faint: .30 on film turned a whole letter white)
			P.Band(Centre, SweepGain, Centre + 70.f, 0.f);
		}
	}
}

// ------------------------------------------------------------------------------------------------ the subsystem
UFTTitleLogo* UFTTitleLogo::Get(const AFTGameMode* G)
{
	const UGameInstance* GI = G ? G->GetGameInstance() : nullptr;
	return GI ? GI->GetSubsystem<UFTTitleLogo>() : nullptr;
}

void UFTTitleLogo::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	EnsureLoaded();   // everything now, so no load lands inside the one-second intro
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UFTTitleLogo::OnPreLoadMap);
}

void UFTTitleLogo::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	if (Hum) Hum->Stop();
	Hum = nullptr;
	bHumWanted = false;
	Super::Deinitialize();
}

TStatId UFTTitleLogo::GetStatId() const { RETURN_QUICK_DECLARE_CYCLE_STAT(UFTTitleLogo, STATGROUP_Tickables); }

/** All eleven layers and the six sounds. Tried when the game instance starts, and once more at the first draw if that
 *  did not find every layer. A missing sound is silent, not an error; a missing layer leaves the flat logo. */
bool UFTTitleLogo::EnsureLoaded()
{
	if (bLoadTried && (bLayersOk || LoadPasses >= 2)) return bLayersOk;
	bLoadTried = true;
	++LoadPasses;
	int32 Found = 0, Sounds = 0;
	FString Missing;
	for (int32 I = 0; I < LCount; ++I)
	{
		if (!Tex[I]) Tex[I] = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *(FString(TEXT("/Game/UI/Title/T_TitleLogo_")) + GLayers[I].Name), nullptr, LOAD_NoWarn | LOAD_Quiet));
	}
#if WITH_EDITOR
	{
		TArray<UTexture*> Wait;
		for (int32 I = 0; I < LCount; ++I) if (Tex[I]) Wait.Add(Tex[I].Get());
		if (Wait.Num() > 0) FTextureCompilingManager::Get().FinishCompilation(Wait);
	}
#endif
	bool bOk = true;
	for (int32 I = 0; I < LCount; ++I)
	{
		const UTexture2D* T = Tex[I];
		if (T) ++Found;
		if (T && T->GetResource() && T->GetSizeX() == GLayers[I].W && T->GetSizeY() == GLayers[I].H) continue;
		bOk = false;
		Missing += (Missing.IsEmpty() ? TEXT("") : TEXT(",")) + (T ? FString::Printf(TEXT("%s(%dx%d res=%d)"), GLayers[I].Name, T->GetSizeX(), T->GetSizeY(), T->GetResource() ? 1 : 0) : FString(GLayers[I].Name));
	}
	bLayersOk = bOk;
	for (const TCHAR* Name : GSounds)
	{
		const FName Key(Name);
		TObjectPtr<USoundBase>& Sound = Cues.FindOrAdd(Key);
		if (!Sound) Sound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, *(FString(TEXT("/Game/Audio/SFX/S_")) + Name), nullptr, LOAD_NoWarn | LOAD_Quiet));
		if (Sound) ++Sounds;
		else Missing += (Missing.IsEmpty() ? TEXT("") : TEXT(",")) + (FString(TEXT("S_")) + Name);
	}
	UE_LOG(LogFT, Display, TEXT("FT TITLE logo layers=%d ok=%d sounds=%d missing=%s"), Found, bLayersOk ? 1 : 0, Sounds, Missing.IsEmpty() ? TEXT("-") : *Missing);
	return bLayersOk;
}

bool UFTTitleLogo::Draw(UCanvas* Canvas, const AFTGameMode* G, UTexture2D* Flat, float X, float Y, float W, float H, float Alpha, float S, bool bInteractive, bool bReducedMotion)
{
	UFTTitleLogo* Logo = Get(G);
	return Logo && Canvas && Canvas->Canvas && Logo->DrawNow(Canvas, G, Flat, X, Y, W, H, Alpha, S, bInteractive, bReducedMotion);
}

void UFTTitleLogo::Confirm(const AFTGameMode* G, int32 Activate)
{
	if (UFTTitleLogo* Logo = Get(G)) Logo->ConfirmNow(G, Activate);
}

/** Since: how long ago the alive state really began (the intro's exact end lies inside the frame that saw it). */
void UFTTitleLogo::EnterAlive(float TitleT, float Since, const TCHAR* Why)
{
	State = EState::Alive;
	AliveNow = PrevAlive = FMath::Max(0.0, double(Since)) - ClockStep;   // (DrawAlive adds this frame's step back)
	RampFrom = DrawFrom = 0.0;
	UE_LOG(LogFT, Display, TEXT("FT TITLE logo alive=1 why=%s t=%.3f"), Why, TitleT);
}

/** Any key, mouse button or pad button pressed this frame, or a push of the stick (the same merged, per-frame press
 *  the menu itself moves on). Not while the controller is waiting for a held button's release after a map load: the
 *  menu ignores such a press, so it must not skip either. It only reads: nothing is consumed, and the game mode has
 *  already acted on the same press in this frame's tick. A mouse move alone is not a press. */
bool UFTTitleLogo::SkipPressed(const AFTGameMode* G) const
{
	const UWorld* World = G->GetWorld();
	const APlayerController* PC = World ? World->GetFirstPlayerController() : nullptr;
	if (!PC) return false;
	const AFTPlayerController* Player = Cast<AFTPlayerController>(PC);
	if (Player && Player->MenuInputBlocked()) return false;
	if (PC->WasInputKeyJustPressed(EKeys::AnyKey)) return true;
	return Player && (Player->Hit(EKeys::Gamepad_LeftStick_Up) || Player->Hit(EKeys::Gamepad_LeftStick_Down)
		|| Player->Hit(EKeys::Gamepad_LeftStick_Left) || Player->Hit(EKeys::Gamepad_LeftStick_Right));
}

bool UFTTitleLogo::DrawNow(UCanvas* Canvas, const AFTGameMode* G, UTexture2D* Flat, float X, float Y, float W, float H, float Alpha, float S, bool bInteractive, bool bReducedMotion)
{
	// 1. off, or something is missing: the caller draws the flat logo as it always did
	const int32 Mode = CVarFTTitleLogo.GetValueOnGameThread();
	if (Mode <= 0 || !EnsureLoaded() || !Flat || !Flat->GetResource())
	{
		SetHum(G, false);
		return false;
	}
	const float TitleT = G->TitleT;
	const float FrameTime = float(FApp::GetDeltaTime());
	ClockStep = LastTitleT < 0.f || GFrameCounter != LastDrawFrame ? FMath::Min(FApp::GetDeltaTime(), 0.1) : 0.0;
	Clock += ClockStep;

	// 2. arrival at the title. An attract cut is not one (TitleT is carried over), nor is closing an overlay (it resumes).
	const bool bGap = SinceDraw > .3f || GFrameCounter - LastDrawFrame > 1;
	if (LastTitleT < 0.f || TitleT < LastTitleT - 1e-4f || (bGap && TitleT < .2f))
	{
		IntroT0 = -1.f; PrevTau = -1.f;
		ConfirmClock = -1.0;
		ArrivalFrame = GFrameCounter;
		IntroVoices.Reset();
		if (bReducedMotion)
		{
			UE_LOG(LogFT, Display, TEXT("FT TITLE logo intro=reduced t=%.3f"), TitleT);
			EnterAlive(TitleT, TitleT, TEXT("reduced"));
		}
		else
		{
			State = EState::Intro;
			UE_LOG(LogFT, Display, TEXT("FT TITLE logo intro=start t=%.3f"), TitleT);
		}
	}

	FPaint P{ Canvas, *this, Flat, X, Y, W, H, Alpha };
	const auto Seen = [&]()
	{
		LastTitleT = TitleT; LastDrawFrame = GFrameCounter; SinceDraw = 0.f;
	};

	// 3. the test mode: this draw path, held at rest
	if (Mode >= 2)
	{
		P.Base();
		SetHum(G, false);
		Seen();
		return true;
	}

	if (State == EState::Intro)
	{
		if (bReducedMotion)   // (switched on in OPTIONS while the intro stood frozen behind it)
		{
			FadeIntroVoices();
			EnterAlive(TitleT, 0.f, TEXT("reduced"));
		}
		else
		{
			// 4. the intro's zero: not before the frames have settled (a cold start's first frames are long), so the
			// flight is not eaten and the thud does not run ahead of the landing
			if (IntroT0 < 0.f && bInteractive && ((TitleT >= .10f && FrameTime <= .07f) || TitleT >= .40f))
			{
				IntroT0 = TitleT; PrevTau = -1.f;
				UE_LOG(LogFT, Display, TEXT("FT TITLE logo intro=zero t=%.3f dt=%.3f"), TitleT, FrameTime);
			}
			// 5. skip: straight to rest, in this same frame. Not on the arrival frame: the press that brought the
			// player here without a map load (READY in PRE-PRODUCTION) is still "just pressed" then.
			if (bInteractive && GFrameCounter > ArrivalFrame && SkipPressed(G))
			{
				UE_LOG(LogFT, Display, TEXT("FT TITLE logo intro=skip t=%.3f"), TitleT);
				FadeIntroVoices();
				EnterAlive(TitleT, 0.f, TEXT("skip"));
			}
			else if (IntroT0 >= 0.f)
			{
				const float Tau = TitleT - IntroT0;
				if (Tau >= IntroEnd)
				{
					// 6. done: the cut to the flat logo, counted from the exact time and not from this frame's
					UE_LOG(LogFT, Display, TEXT("FT TITLE logo intro=done t=%.3f"), TitleT);
					EnterAlive(TitleT, Tau - IntroEnd, TEXT("intro"));
				}
				else if (bInteractive)
				{
					const auto Crossed = [&](float At) { return PrevTau < At && Tau >= At; };
					if (Crossed(0.f)) Cue(G, TEXT("TitleSlam"), .32f, 1.f, EVoice::Intro);   // (the thud is 0.18 s into the file)
					if (Crossed(Flight))
					{
						UE_LOG(LogFT, Display, TEXT("FT TITLE logo intro=land t=%.3f"), TitleT);
						Cue(G, TEXT("TitleWipe"), .28f, 1.f, EVoice::Intro);
					}
					if (Crossed(StarStart[0])) Cue(G, TEXT("TitleTwinkle"), .32f, 1.f, EVoice::Intro);
					if (Crossed(StarStart[1])) Cue(G, TEXT("TitleTwinkle"), .32f, 1.122f, EVoice::Intro);
					PrevTau = Tau;
				}
			}
		}
	}
	else if (State == EState::Confirmed && bInteractive)
	{
		// 7. the confirm left the player on the title (back from OPTIONS, CONTROLS or a dialog): alive again, 0.6 s
		// after the title is open to input again. The alive patterns go on from where they stood; whatever was part
		// way through then is not shown (it would appear half done, without its chime).
		if (GFrameCounter != ConfirmFrame) ConfirmHeld += ClockStep;
		if (CVarFTTitleLogoRearm.GetValueOnGameThread() > 0 && ConfirmHeld >= .6 - 1e-6)
		{
			State = EState::Alive;
			RampFrom = DrawFrom = PrevAlive = AliveNow;
			AliveNow -= ClockStep;   // (DrawAlive adds this frame's step back, so this first frame stands at DrawFrom)
			UE_LOG(LogFT, Display, TEXT("FT TITLE logo alive=1 why=resume t=%.3f"), TitleT);
		}
	}

	// What is drawn is chosen by the state, never by the clock alone: the intro draws layers, everything else the
	// flat logo with light on top. Under an overlay (step 8): the intro as it stands, otherwise the flat logo only.
	switch (State)
	{
	case EState::Intro:
		if (IntroT0 >= 0.f) DrawIntro(P, TitleT - IntroT0, S, W, FrameTime);   // (before the zero the masthead is empty)
		break;
	case EState::Alive:
		P.Base();
		if (bInteractive) DrawAlive(P, G, bReducedMotion);
		break;
	default:
		P.Base();
		if (State == EState::Confirmed && ConfirmClock >= 0.0 && Clock - ConfirmClock < ConfirmTwinkle)
		{
			// one last bright twinkle from both stars together, then steady
			const float U = float((Clock - ConfirmClock) / ConfirmTwinkle), Glint = FMath::Sin(PI * U);
			for (int32 I = 0; I < 2; ++I) P.Star(LStar1Lit + I, I, 1.f + .7f * Glint, 45.f * (1.f - U), 1.3f * Glint, true);
		}
		break;
	}

	SetHum(G, bInteractive && (State == EState::Alive || (State == EState::Intro && IntroT0 >= 0.f && TitleT - IntroT0 >= Flight)));
	Seen();   // 9.
	return true;
}

void UFTTitleLogo::ConfirmNow(const AFTGameMode* G, int32 Activate)
{
	if (CVarFTTitleLogo.GetValueOnGameThread() != 1) return;
	// A confirm while the logo already stands confirmed (a selection made straight after OPTIONS, CONTROLS or a
	// dialog closed, before the logo is alive again) twinkles and chimes like any other; once a frame at most.
	if (State == EState::Idle || (State == EState::Confirmed && ConfirmFrame == GFrameCounter)) return;
	if (Activate == 0)
	{
		// CONTINUE with nothing to continue: the press does nothing in the game, so it is not a selection
		const UFTCareerSubsystem* Career = UFTCareerSubsystem::Get(G);
		if (!Career || !Career->HasContinue()) return;
	}
	const UFTGameInstance* GI = G->GI();
	const bool bReducedMotion = GI && GI->bReducedMotion;
	if (State == EState::Intro) { AliveNow = PrevAlive = 0.0; RampFrom = DrawFrom = 0.0; }   // (the alive patterns count from here if the logo is re-armed)
	State = EState::Confirmed;
	ConfirmHeld = 0.0;
	ConfirmFrame = GFrameCounter;
	ConfirmClock = bReducedMotion ? -1.0 : Clock;      // reduced motion: only the chime, no twinkle
	FadeIntroVoices();
	SetHum(G, false);                                  // at once: a blocking map load may start in this same tick
	UE_LOG(LogFT, Display, TEXT("FT TITLE logo confirm=%d t=%.3f"), Activate, G->TitleT);
	Cue(G, TEXT("TitleTwinklePair"), .22f, 1.f, EVoice::Persist);
}

/** A logo sound: the same assets, 2D playback and attract rule as AFTGameMode::Sfx (x 0.4 under the attract demo,
 *  which is the title's normal state). The log line is the evidence of timing in a run without sound. */
void UFTTitleLogo::Cue(const AFTGameMode* G, const TCHAR* Name, float Volume, float Pitch, EVoice Kind)
{
	const TObjectPtr<USoundBase>* Found = Cues.Find(FName(Name));
	USoundBase* Sound = Found ? Found->Get() : nullptr;
	const float V = Volume * (G->bAttract ? .4f : 1.f) * FMath::Max(0.f, CVarFTTitleLogoVol.GetValueOnGameThread());
	UE_LOG(LogFT, Display, TEXT("FT CUE logo=%s vol=%.3f pitch=%.2f found=%d"), Name, V, Pitch, Sound ? 1 : 0);
	if (!Sound || V <= 0.f) return;
	if (Kind == EVoice::OneShot) { UGameplayStatics::PlaySound2D(G, Sound, V, Pitch); return; }
	// an intro voice is kept so a skip or a confirm can cut it short; the confirm's chime outlives the map load that may follow
	UAudioComponent* Voice = UGameplayStatics::CreateSound2D(G, Sound, V, Pitch, 0.f, nullptr, Kind == EVoice::Persist, true);
	if (!Voice) return;   // (no audio device)
	Voice->bIsUISound = true;
	Voice->Play();
	if (Kind == EVoice::Intro) IntroVoices.Add(Voice);
}

void UFTTitleLogo::FadeIntroVoices()
{
	// the thud must not land after the logo has already jumped to rest
	for (const TWeakObjectPtr<UAudioComponent>& Voice : IntroVoices) if (UAudioComponent* V = Voice.Get()) V->FadeOut(.04f, 0.f);
	IntroVoices.Reset();
}

/** The neon hum under the menu. The audio device does the fades (as it does for the title music), so they run through
 *  a blocking load. Not scaled by the attract rule: it is set against the music, which is not scaled either. */
void UFTTitleLogo::SetHum(const UObject* Ctx, bool bWant)
{
	const float Volume = FMath::Max(0.f, CVarFTTitleLogoHum.GetValueOnGameThread()) * FMath::Max(0.f, CVarFTTitleLogoVol.GetValueOnGameThread());
	bWant = bWant && Volume > 0.f && CVarFTTitleLogo.GetValueOnGameThread() == 1;
	if (bWant == bHumWanted)
	{
		if (bWant && Hum && Volume != HumVolume) { HumVolume = Volume; Hum->SetVolumeMultiplier(Volume); }
		return;
	}
	bHumWanted = bWant;
	if (bWant)
	{
		if (!Hum && Ctx)
		{
			const TObjectPtr<USoundBase>* Found = Cues.Find(FName(TEXT("TitleNeonHum")));
			if (USoundBase* Sound = Found ? Found->Get() : nullptr)
			{
				// bPersistAcrossLevelTransition: the attract mode's map loads do not cut it
				Hum = UGameplayStatics::CreateSound2D(Ctx, Sound, Volume, 1.f, 0.f, nullptr, true, false);
				if (Hum) Hum->bIsUISound = true;
			}
		}
		if (Hum)
		{
			HumVolume = Volume;
			Hum->SetVolumeMultiplier(Volume);
			Hum->FadeIn(.4f, 1.f);   // (the fade's level multiplies the component's volume: 1, not the volume again)
		}
	}
	else if (Hum) Hum->FadeOut(.15f, 0.f);
	UE_LOG(LogFT, Display, TEXT("FT TITLE logo hum=%d dev=%d"), bWant ? 1 : 0, Hum ? 1 : 0);
}

void UFTTitleLogo::Tick(float DeltaTime)
{
	SinceDraw = FMath::Min(SinceDraw + FMath::Min(DeltaTime, .1f), 1000.f);
	// the title is no longer drawn (0.3 s of grace: a blocking attract-cut load adds at most 0.1), or a switch went off
	if (bHumWanted) SetHum(nullptr, SinceDraw <= .3f);
}

void UFTTitleLogo::OnPreLoadMap(const FString& MapName)
{
	// A map load that is not an attract cut leaves the title (the harness jumps never pass through Confirm). The
	// cut sets bAttractCut before its load and the next game mode clears it: the hum plays on across it.
	const UFTGameInstance* GI = Cast<UFTGameInstance>(GetGameInstance());
	if (GI && GI->bAttractCut) return;
	SetHum(nullptr, false);
}

#if !UE_BUILD_SHIPPING
namespace
{
	FAutoConsoleCommandWithWorldAndArgs GFTTitleLogoConfirm(TEXT("ft.TitleLogoConfirm"), TEXT("FULL THROTTLE tests: the title logo's confirm twinkle without touching the menu"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
		{
			if (const AFTGameMode* G = World ? World->GetAuthGameMode<AFTGameMode>() : nullptr) UFTTitleLogo::Confirm(G, -2);
		}));
}
#endif
