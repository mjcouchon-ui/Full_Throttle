#include "FTHUD.h"
#include "FTGameMode.h"
#include "FTGameInstance.h"
#include "FTPlayerCar.h"
#include "FTPlayerController.h"
#include "FTProgress.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Engine/Engine.h"
#include "CanvasItem.h"
#include "Engine/Texture2D.h"

namespace
{
	const FLinearColor Ink(0.02f, 0.01f, 0.05f, 0.78f), Yellow(1.f, 0.82f, 0.25f), Pink(1.f, 0.24f, 0.65f), Cyan(0.21f, 0.95f, 1.f), Red(1.f, 0.2f, 0.25f), Green(0.35f, 1.f, 0.45f), White(1.f, 0.97f, 0.92f), Grey(0.65f, 0.62f, 0.75f), Amber(1.f, 0.62f, 0.18f);
	FString Money(int64 V);
	FString Num(int64 V)
	{
		FString R = FString::Printf(TEXT("%lld"), FMath::Abs(V));
		for (int32 i = R.Len() - 3; i > 0; i -= 3) R.InsertAt(i, TEXT(","));
		return (V < 0 ? TEXT("-") : TEXT("")) + R;
	}
	FString Money(int64 V) { return (V < 0 ? TEXT("-$") : TEXT("$")) + Num(FMath::Abs(V)); }
}

void AFTHUD::Txt(const FString& Str, float X, float Y, const FLinearColor& C, float Scale, int32 Align, bool bDisplay, bool bShadow)
{
	UFont* F = bDisplay ? FD : FB;
	FCanvasTextItem It(FVector2D(X, Y), FText::FromString(Str), F, C);
	It.Scale = FVector2D(Scale * S);
	if (bShadow) { It.EnableShadow(FLinearColor(0, 0, 0, 0.85f), FVector2D(2.f * S, 2.f * S)); }
	if (Align != 0)
	{
		const FVector2D M = Measure(Str, Scale, bDisplay);
		It.Position.X -= Align == 1 ? M.X * 0.5f : M.X;
	}
	Canvas->DrawItem(It);
}

FVector2D AFTHUD::Measure(const FString& Str, float Scale, bool bDisplay)
{
	float Xl = 0, Yl = 0;
	Canvas->StrLen(bDisplay ? FD : FB, Str, Xl, Yl);
	return FVector2D(Xl, Yl) * Scale * S;
}

void AFTHUD::Box(float X, float Y, float Wd, float Ht, const FLinearColor& C)
{
	FCanvasTileItem It(FVector2D(X, Y), FVector2D(Wd, Ht), C);
	It.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(It);
}

void AFTHUD::Frame(float X, float Y, float Wd, float Ht, const FLinearColor& C, float Th)
{
	Th *= S;
	Box(X, Y, Wd, Th, C); Box(X, Y + Ht - Th, Wd, Th, C); Box(X, Y, Th, Ht, C); Box(X + Wd - Th, Y, Th, Ht, C);
}

void AFTHUD::Panel(float X, float Y, float Wd, float Ht)
{
	Box(X, Y, Wd, Ht, Ink);
	Frame(X, Y, Wd, Ht, FLinearColor(1.f, 0.31f, 0.72f, 0.55f), 1.5f);
}

void AFTHUD::DrawHUD()
{
	Super::DrawHUD();
	AFTGameMode* G = GetWorld() ? GetWorld()->GetAuthGameMode<AFTGameMode>() : nullptr;
	if (!G || !Canvas) return;
	W = Canvas->SizeX; H = Canvas->SizeY; S = H / 720.f;
	T = GetWorld()->GetRealTimeSeconds();
	if (!FD) FD = DisplayFont ? DisplayFont.Get() : Cast<UFont>(StaticLoadObject(UFont::StaticClass(), nullptr, TEXT("/Game/UI/Fonts/F_Display"), nullptr, LOAD_NoWarn | LOAD_Quiet));
	if (!FB) FB = BodyFont ? BodyFont.Get() : Cast<UFont>(StaticLoadObject(UFont::StaticClass(), nullptr, TEXT("/Game/UI/Fonts/F_Body"), nullptr, LOAD_NoWarn | LOAD_Quiet));
	if (!FD) FD = GEngine->GetLargeFont();
	if (!FB) FB = GEngine->GetMediumFont();

	if (!bTitleAssets)
	{
		bTitleAssets = true;
		FOrb = Cast<UFont>(StaticLoadObject(UFont::StaticClass(), nullptr, TEXT("/Game/UI/Fonts/F_Orbitron"), nullptr, LOAD_NoWarn | LOAD_Quiet));
		FRaj = Cast<UFont>(StaticLoadObject(UFont::StaticClass(), nullptr, TEXT("/Game/UI/Fonts/F_Rajdhani"), nullptr, LOAD_NoWarn | LOAD_Quiet));
		TLogo = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/UI/Title/T_TitleLogo"), nullptr, LOAD_NoWarn | LOAD_Quiet));
		UE_LOG(LogFT, Display, TEXT("FT TITLE assets orbitron=%d rajdhani=%d logo=%d"), FOrb ? 1 : 0, FRaj ? 1 : 0, TLogo ? 1 : 0);
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
	case EFTPhase::Play: DrawPlay(G); break;
	case EFTPhase::Wreck: case EFTPhase::Wrap: DrawPlay(G); DrawCut(G); break;
	case EFTPhase::Reel: DrawReel(G); break;
	case EFTPhase::Rating: DrawRating(G); break;
	case EFTPhase::Paycheck: DrawPaycheck(G); break;
	case EFTPhase::PreProduction: DrawPreProduction(G); break;
	}
	if (!G->bAttract) DrawFx(G);
	switch (G->Overlay)
	{
	case EFTOverlay::Pause: DrawPause(G); break;
	case EFTOverlay::Controls: DrawControls(G); break;
	case EFTOverlay::Options: DrawOptions(G); break;
	default: break;
	}
	if (const UFTGameInstance* GI = G->GI())
	{
		if (GI->bShowFps) Txt(FString::Printf(TEXT("%.0f FPS  %.1f ms"), G->FpsSmoothed, 1000.f / FMath::Max(1.f, G->FpsSmoothed)), W - 12 * S, H - 26 * S, Green, 0.9f, 2);
		if (GI->bMuted) Txt(TEXT("MUTED (M)"), 12 * S, H - 26 * S, Grey, 0.8f);
	}
}

// ---------------------------------------------------------------------------------------------- title
// 3.0: text in a given font (the title's Orbitron / Rajdhani), with an optional neon glow around it
void AFTHUD::TxtF(UFont* F, const FString& Str, float X, float Y, const FLinearColor& C, float Scale, int32 Align, bool bShadow, const FLinearColor& Glow)
{
	if (!F) F = FB;
	if (Align != 0)
	{
		const FVector2D M = MeasureF(F, Str, Scale);
		X -= Align == 1 ? M.X * 0.5f : M.X;
	}
	if (Glow.A > 0.f)
	{
		// a cheap neon halo: the text eight times, offset, faint
		static const FVector2D Off[8] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {0.7f, 0.7f}, {-0.7f, 0.7f}, {0.7f, -0.7f}, {-0.7f, -0.7f} };
		for (const FVector2D& O : Off)
		{
			FCanvasTextItem Gl(FVector2D(X, Y) + O * 2.5f * S, FText::FromString(Str), F, Glow);
			Gl.Scale = FVector2D(Scale * S);
			Gl.BlendMode = SE_BLEND_Translucent;
			Canvas->DrawItem(Gl);
		}
	}
	FCanvasTextItem It(FVector2D(X, Y), FText::FromString(Str), F, C);
	It.Scale = FVector2D(Scale * S);
	if (bShadow) It.EnableShadow(FLinearColor(0.02f, 0.f, 0.06f, 0.9f), FVector2D(2.f * S, 2.f * S));
	Canvas->DrawItem(It);
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
	const FLinearColor C0 = A, C1 = bHorizontal ? B : A, C2 = bHorizontal ? A : B, C3 = B;
	TArray<FCanvasUVTri> Tris;
	FCanvasUVTri T0; T0.V0_Pos = P0; T0.V1_Pos = P1; T0.V2_Pos = P2; T0.V0_Color = C0; T0.V1_Color = C1; T0.V2_Color = C2;
	FCanvasUVTri T1; T1.V0_Pos = P1; T1.V1_Pos = P3; T1.V2_Pos = P2; T1.V0_Color = C1; T1.V1_Color = C3; T1.V2_Color = C2;
	Tris.Add(T0); Tris.Add(T1);
	FCanvasTriangleItem It(Tris, GWhiteTexture);
	It.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(It);
}

FBox2D AFTHUD::TitleButton(float X, float Y, float Wd, float Ht, const FString& Label, const FString& Sub, bool bHot)
{
	const float Pulse = 0.5f + 0.5f * FMath::Sin(T * 4.f);
	if (bHot) Grad(X, Y, Wd, Ht, FLinearColor(1.f, 0.18f, 0.62f, 0.9f), FLinearColor(0.45f, 0.06f, 0.55f, 0.9f), true);
	else Box(X, Y, Wd, Ht, FLinearColor(0.06f, 0.01f, 0.12f, 0.78f));
	Frame(X, Y, Wd, Ht, bHot ? Yellow : FLinearColor(0.21f, 0.95f, 1.f, 0.45f + 0.25f * Pulse), bHot ? 2.5f : 1.5f);
	TxtF(FOrb, Label, X + Wd * 0.5f, Y + Ht * 0.14f, bHot ? FLinearColor(1.f, 1.f, 0.9f) : White, 0.34f, 1);
	TxtF(FRaj, Sub, X + Wd * 0.5f, Y + Ht * 0.56f, bHot ? Yellow : FLinearColor(0.75f, 0.9f, 1.f, 0.85f), 0.38f, 1, false);
	return FBox2D(FVector2D(X, Y), FVector2D(X + Wd, Y + Ht));
}

void AFTHUD::DrawTitle(AFTGameMode* G)
{
	UFont* FO = FOrb ? FOrb.Get() : FD.Get();
	UFont* FR = FRaj ? FRaj.Get() : FB.Get();
	const UFTGameInstance* GI = G->GI();
	const bool bAtt = G->bAttract;
	const FLinearColor Night(0.02f, 0.f, 0.06f, 0.f);
	auto WithA = [](FLinearColor C, float A) { C.A = A; return C; };

	// shade where the title sits, so it reads over the racing (the static title, attract off, keeps its full veil)
	if (!bAtt) Box(0, 0, W, H, FLinearColor(0.02f, 0.f, 0.06f, 0.45f));
	Grad(0, 0, W, H * 0.34f, WithA(Night, 0.78f), WithA(Night, 0.f), false);
	Grad(0, 0, W * 0.47f, H, WithA(Night, 0.80f), WithA(Night, 0.f), true);
	Grad(0, H * 0.78f, W, H * 0.22f, WithA(Night, 0.f), WithA(Night, 0.88f), false);

	// the logo: a quick zoom in the first time the title opens (not on an attract cut)
	const float In = FMath::Clamp(G->TitleT / 0.7f, 0.f, 1.f);
	const float Ease = 1.f - FMath::Pow(1.f - In, 3.f);
	const float LogoH = 158.f * S * (1.f + 0.25f * (1.f - Ease)), LogoW = LogoH * 1600.f / 608.f;
	const float LogoY = 6.f * S;
	if (TLogo)
	{
		FCanvasTileItem Lg(FVector2D(W * 0.5f - LogoW * 0.5f, LogoY), TLogo->GetResource(), FVector2D(LogoW, LogoH), FLinearColor(1.f, 1.f, 1.f, Ease));
		Lg.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(Lg);
	}
	else
	{
		TxtF(FO, TEXT("FULL THROTTLE"), W * 0.5f, LogoY + 40 * S, Cyan, 1.6f, 1, true, FLinearColor(1.f, 0.2f, 0.65f, 0.25f));
	}
	const float TagY = LogoY + 158.f * S;
	TxtF(FR, TEXT("AN 80s SPY CAR CHASE  ·  STARRING YOU AS AGENT NOVA KANE"), W * 0.5f, TagY, Yellow, 0.50f, 1);
	if (const UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(G))
		TxtF(FR, FString::Printf(TEXT("DRIVER LV %d   ·   %s   ·   %d SKILL POINT%s TO SPEND"), CS->C.Level, *Money(CS->C.Cash), CS->SPLeft(), CS->SPLeft() == 1 ? TEXT("") : TEXT("S")),
			W * 0.5f, TagY + 19 * S, FLinearColor(1.f, 0.45f, 0.8f), 0.42f, 1);

	// the eight levels, one list down the left
	const int32 NL = FTData::NumLevels();
	const float Lx = 28 * S, Lw = 450 * S, Rh = 38 * S, Gap = 4 * S, Ly0 = H * 0.335f;
	TxtF(FO, TEXT("SELECT YOUR SHOOT"), Lx + 4 * S, Ly0 - 20 * S, Cyan, 0.32f, 0, true, FLinearColor(0.2f, 0.9f, 1.f, 0.12f));
	G->TitleRowRects.SetNum(NL);
	for (int32 i = 0; i < NL; ++i)
	{
		const FFTLevelDef& L = FTData::Level(i);
		const bool bSel = G->TitleSel == i;
		const float Y = Ly0 + i * (Rh + Gap);
		const float X = Lx + (bSel ? 10.f * S : 0.f);
		if (bSel) Grad(X, Y, Lw, Rh, FLinearColor(1.f, 0.18f, 0.62f, 0.88f), FLinearColor(0.3f, 0.04f, 0.45f, 0.35f), true);
		else Box(X, Y, Lw, Rh, FLinearColor(0.05f, 0.01f, 0.1f, 0.58f));
		Box(X, Y, 4 * S, Rh, bSel ? Yellow : FLinearColor(1.f, 0.31f, 0.72f, 0.55f));
		if (bSel) Frame(X, Y, Lw, Rh, FLinearColor(1.f, 0.85f, 0.3f, 0.9f), 1.5f);
		// the number in a neon square
		const float Nx = X + 12 * S, Ny = Y + 5 * S, Ns = Rh - 10 * S;
		Box(Nx, Ny, Ns, Ns, bSel ? FLinearColor(0.05f, 0.f, 0.1f, 0.9f) : FLinearColor(0.f, 0.f, 0.f, 0.4f));
		Frame(Nx, Ny, Ns, Ns, bSel ? Yellow : FLinearColor(0.21f, 0.95f, 1.f, 0.7f), 1.5f);
		TxtF(FO, FString::FromInt(i + 1), Nx + Ns * 0.5f, Ny + Ns * 0.16f, bSel ? Yellow : Cyan, 0.38f, 1, false);
		TxtF(FO, L.Zone, Nx + Ns + 12 * S, Y + 3 * S, bSel ? FLinearColor(1.f, 1.f, 0.95f) : FLinearColor(0.92f, 0.9f, 1.f, 0.9f), 0.37f, 0, true);
		FString Sub = L.Title;
		if (GI && i < UFTGameInstance::MaxLevels && GI->BestScore[i] > 0) Sub += FString::Printf(TEXT("   ·   BEST %s  %s"), *Num(GI->BestScore[i]), *GI->BestTitle[i]);
		TxtF(FR, Sub, Nx + Ns + 12 * S, Y + 20 * S, bSel ? Yellow : FLinearColor(1.f, 0.55f, 0.85f, 0.9f), 0.42f, 0, true);
		// the level the demo is showing right now
		if (bAtt && G->Level == i && G->AttractOutT < 0.f)
		{
			const bool bBlink = FMath::Fmod(T, 1.f) < 0.6f;
			Box(X + Lw - 78 * S, Y + Rh * 0.5f - 4 * S, 8 * S, 8 * S, FLinearColor(1.f, 0.15f, 0.2f, bBlink ? 1.f : 0.35f));
			TxtF(FR, TEXT("ON AIR"), X + Lw - 8 * S, Y + Rh * 0.5f - 7 * S, FLinearColor(1.f, 0.4f, 0.4f), 0.4f, 2, false);
		}
		G->TitleRowRects[i] = FBox2D(FVector2D(Lx, Y), FVector2D(Lx + Lw + 10 * S, Y + Rh));
	}

	// roll camera, then the buttons (each also has its key and pad button)
	const float PromptY = H * 0.818f;
	if (FMath::Fmod(T, 1.1f) < 0.75f)
		TxtF(FO, TEXT("PRESS ENTER (A) TO ROLL CAMERA"), W * 0.5f, PromptY, White, 0.48f, 1, true, FLinearColor(1.f, 0.2f, 0.65f, 0.22f));
	static const TCHAR* BL[5] = { TEXT("ROLL CAMERA"), TEXT("PRE-PRODUCTION"), TEXT("CONTROLS"), TEXT("OPTIONS"), TEXT("QUIT") };
	static const TCHAR* BS[5] = { TEXT("ENTER  ·  PAD A"), TEXT("G  ·  PAD BACK"), TEXT("C  ·  PAD Y"), TEXT("O"), TEXT("ESC") };
	const float Bw = 190 * S, Bh = 40 * S, Bg = 12 * S, Bx0 = W * 0.5f - (Bw * 5 + Bg * 4) * 0.5f, By = H * 0.874f;
	G->TitleBtnRects.SetNum(5);
	for (int32 i = 0; i < 5; ++i) G->TitleBtnRects[i] = TitleButton(Bx0 + i * (Bw + Bg), By, Bw, Bh, BL[i], BS[i], G->TitleHotBtn == i);

	// one help line at the bottom, taking turns
	if (GI)
	{
		const FString Lay = GI->Layout == 0 ? TEXT("WASD steer  ·  hold SHIFT + steer to drift  ·  SPACE turbo  ·  J guns  ·  K missile  ·  L ammo")
			: TEXT("ARROWS steer  ·  hold SHIFT + steer to drift  ·  SPACE turbo  ·  Z guns  ·  X missile  ·  C ammo");
		const int32 Turn = (int32)(T / 5.f) % 3;
		const FString Help = Turn == 0 ? Lay : (Turn == 1 ? FString(TEXT("Mouse: hold left = guns, right click = missile   ·   Gamepad: stick, A/RB drift, X guns, B missile, LB/RT turbo"))
			: FString(TEXT("UP / DOWN or 1-8 pick the level   ·   M mute   ·   F1 frame rate   ·   Unreal Engine 5.9")));
		TxtF(FR, Help, W * 0.5f, H * 0.955f, FLinearColor(0.7f, 0.95f, 1.f, 0.9f), 0.44f, 1, true);
	}
	if (GI && !GI->TitleBanner.IsEmpty() && G->TitleT < 8.f)
	{
		const float A = FMath::Clamp(8.f - G->TitleT, 0.f, 1.f);
		Box(W * 0.47f, H * 0.42f, W * 0.53f, 70 * S, FLinearColor(0, 0, 0, 0.6f * A));
		TxtF(FO, GI->TitleBanner, W * 0.735f, H * 0.43f, FLinearColor(1.f, 0.82f, 0.25f, A), 0.85f, 1, true, FLinearColor(1.f, 0.4f, 0.1f, 0.2f * A));
	}
	if (bAtt) DrawAttractOsd(G);
}

// 3.0: the VHS-style on-screen display in the top right of the attract mode
void AFTHUD::DrawAttractOsd(AFTGameMode* G)
{
	UFont* FO = FOrb ? FOrb.Get() : FD.Get();
	UFont* FR = FRaj ? FRaj.Get() : FB.Get();
	const bool bOut = G->AttractOutT >= 0.f;
	const int32 Lv = bOut ? G->AttractNextLevel() : G->Level;
	const FFTLevelDef& L = FTData::Level(Lv);
	const FLinearColor Osd(0.55f, 1.f, 0.7f, 0.95f);
	const float Rx = W - 26 * S, Y = 18 * S;
	const FString Ch = FString::Printf(TEXT("CH %02d"), Lv + 1);
	const FVector2D Cm = MeasureF(FO, Ch, 0.48f);
	TxtF(FO, Ch, Rx, Y, Osd, 0.48f, 2, true);
	const FString Pl = bOut ? TEXT("TUNING") : TEXT("PLAY");
	const FVector2D Pm = MeasureF(FO, Pl, 0.48f);
	const float Px = Rx - Cm.X - 22 * S - Pm.X;
	TxtF(FO, Pl, Px, Y, Osd, 0.48f, 0, true);
	if (!bOut && FMath::Fmod(T, 1.f) < 0.7f)
	{
		// the play triangle
		const float Tx = Px - 20 * S, Ty = Y + Pm.Y * 0.5f;
		FCanvasTriangleItem Tri(FVector2D(Tx, Ty - 9 * S), FVector2D(Tx + 15 * S, Ty), FVector2D(Tx, Ty + 9 * S), GWhiteTexture);
		Tri.SetColor(Osd);
		Canvas->DrawItem(Tri);
	}
	TxtF(FR, FString::Printf(TEXT("LEVEL %d  ·  %s  ·  %s"), Lv + 1, *L.Zone, *L.Title), Rx, Y + Cm.Y + 1 * S, FLinearColor(1.f, 1.f, 1.f, 0.92f), 0.44f, 2, true);
	TxtF(FR, TEXT("DEMO  ·  THE AUTOPILOT IS DRIVING"), Rx, Y + Cm.Y + 18 * S, FLinearColor(1.f, 0.6f, 0.85f, 0.85f), 0.38f, 2, true);
}

// 3.0: the channel-change wipe between the attract mode's levels. Neon-edged bands slide in from alternate sides,
// the covered screen shows a test card (a striped synthwave sun, the channel and the level), then the bands slide
// away. The title stays drawn on top of all of it; the next map loads while the card is up.
void AFTHUD::DrawAttractWipe(AFTGameMode* G)
{
	const float Cv = G->AttractCover();
	if (Cv <= 0.f) return;
	const bool bOut = G->AttractOutT >= 0.f;
	UFont* FO = FOrb ? FOrb.Get() : FD.Get();
	UFont* FR = FRaj ? FRaj.Get() : FB.Get();
	const int32 N = 10;
	const float Bh = H / N;
	static const FLinearColor Edge[4] = { FLinearColor(1.f, 0.18f, 0.66f), FLinearColor(0.21f, 0.95f, 1.f), FLinearColor(1.f, 0.55f, 0.2f), FLinearColor(1.f, 0.85f, 0.3f) };
	for (int32 i = 0; i < N; ++i)
	{
		const float P = FMath::Clamp(Cv * 1.6f - i * 0.06f, 0.f, 1.f);
		const float E = P * P * (3.f - 2.f * P);
		if (E <= 0.f) continue;
		const float Wd = W * E;
		const bool bFromLeft = ((i % 2) == 0) == bOut;   // going out they come in from one side; coming in they leave the other way
		const float X = bFromLeft ? 0.f : W - Wd;
		const float Y = i * Bh;
		Box(X, Y, Wd, Bh + 1.f, FLinearColor(0.07f, 0.01f, 0.15f, 1.f));
		Box(X, Y + Bh * 0.42f, Wd, Bh * 0.08f, FLinearColor(0.25f, 0.04f, 0.35f, 1.f));
		const float Ex = bFromLeft ? X + Wd - 5 * S : X;
		if (E < 0.999f) { Box(Ex, Y, 5 * S, Bh, Edge[i % 4]); Box(bFromLeft ? Ex - 18 * S : Ex + 5 * S, Y, 18 * S, Bh, FLinearColor(Edge[i % 4].R, Edge[i % 4].G, Edge[i % 4].B, 0.25f)); }
	}
	if (Cv < 0.985f) return;
	// the test card
	const int32 Lv = bOut ? G->AttractNextLevel() : G->Level;
	const FFTLevelDef& L = FTData::Level(Lv);
	const float Cx = W * 0.72f, Cy = H * 0.60f, R = 120.f * S;
	for (float Dy = -R; Dy < 0.f; Dy += 3.f * S)
	{
		const float U = (Dy + R) / R;   // 0 at the top .. 1 at the horizon
		const float Half = FMath::Sqrt(FMath::Max(0.f, R * R - Dy * Dy));
		// the sunset stripes: gaps that widen toward the horizon
		if (U > 0.45f && FMath::Fmod((Dy + R) / S, 14.f) < 2.f + 8.f * (U - 0.45f)) continue;
		Box(Cx - Half, Cy + Dy, Half * 2.f, 3.f * S + 0.5f, FLinearColor(1.f, FMath::Lerp(0.85f, 0.2f, U), FMath::Lerp(0.3f, 0.6f, U), 1.f));
	}
	// the grid floor
	for (int32 k = 0; k < 7; ++k)
	{
		const float Gy = Cy + 4 * S + FMath::Pow(k / 6.f, 1.8f) * H * 0.3f;
		Box(W * 0.47f, Gy, W * 0.53f, 2 * S, FLinearColor(1.f, 0.2f, 0.7f, 0.75f));
	}
	for (int32 k = -6; k <= 6; ++k)
	{
		FCanvasLineItem Ln(FVector2D(Cx + k * 30 * S, Cy + 4 * S), FVector2D(Cx + k * 200 * S, H));
		Ln.SetColor(FLinearColor(1.f, 0.2f, 0.7f, 0.6f)); Ln.LineThickness = 2.f * S;
		Canvas->DrawItem(Ln);
	}
	TxtF(FO, FString::Printf(TEXT("CH %02d"), Lv + 1), Cx, Cy - R - 70 * S, FLinearColor(0.55f, 1.f, 0.7f), 0.9f, 1, true, FLinearColor(0.3f, 1.f, 0.5f, 0.18f));
	TxtF(FO, L.Zone, Cx, Cy + 22 * S, White, 0.62f, 1, true, FLinearColor(1.f, 0.2f, 0.65f, 0.25f));
	TxtF(FR, FString::Printf(TEXT("LEVEL %d  ·  %s"), Lv + 1, *L.Title), Cx, Cy + 52 * S, Yellow, 0.52f, 1, true);
	// scan lines and a little snow
	for (float Y = 0.f; Y < H; Y += 4.f * S) Box(0, Y, W, 1.2f * S, FLinearColor(0.f, 0.f, 0.f, 0.22f));
	FRandomStream Rs((int32)(T * 30.f));
	for (int32 k = 0; k < 90; ++k) Box(Rs.FRand() * W, Rs.FRand() * H, (2.f + Rs.FRand() * 10.f) * S, 2.f * S, FLinearColor(1.f, 1.f, 1.f, 0.25f + 0.4f * Rs.FRand()));
}

// ---------------------------------------------------------------------------------------------- slate
void AFTHUD::DrawSlate(float Cx, float Cy, float Sc, float Clap, const TArray<FString>& Lines)
{
	const float Wd = 420 * S * Sc, Ht = 250 * S * Sc;
	const float X = Cx - Wd / 2, Y = Cy - Ht / 2;
	Box(X, Y, Wd, Ht, FLinearColor(0.05f, 0.05f, 0.07f, 0.95f));
	Frame(X, Y, Wd, Ht, White, 2.f);
	// the clapper sticks, striped, hinged at the left
	const float Sh = 34 * S * Sc;
	for (int32 k = 0; k < 2; ++k)
	{
		// two striped sticks sit on top of the board; the top one lifts while the clapper is open
		const float Yy = Y - Sh * (k == 0 ? 2.f : 1.f) - (k == 0 ? Clap * 40 * S * Sc : 0.f);
		Box(X, Yy, Wd, Sh * 0.95f, FLinearColor(0.08f, 0.08f, 0.1f, 1.f));
		for (int32 i = 0; i < 7; ++i)
		{
			FCanvasTriangleItem Tri(FVector2D(X + (i * 2) * Wd / 14, Yy + 0.f), FVector2D(X + (i * 2 + 1) * Wd / 14, Yy + 0.f), FVector2D(X + (i * 2 + 1.6f) * Wd / 14, Yy + 0.f + Sh * 0.95f), GWhiteTexture);
			Tri.SetColor(White);
			Canvas->DrawItem(Tri);
		}
	}
	for (int32 i = 0; i < Lines.Num(); ++i) Txt(Lines[i], X + 18 * S * Sc, Y + (18 + i * 44) * S * Sc, i == 0 ? Yellow : White, (i == 0 ? 0.9f : 1.25f) * Sc, 0, i > 0);
}

void AFTHUD::DrawCard(AFTGameMode* G)
{
	const float P = G->PhaseT;
	Box(0, 0, W, H, FLinearColor(0, 0, 0, P > 2.9f ? FMath::Clamp((P - 2.9f) / 0.3f, 0.f, 1.f) : 0.55f));
	const float In = FMath::Clamp(P / 0.45f, 0.f, 1.f);
	const float Clap = P < 1.0f ? 1.f : 0.f;
	const int32 Run = G->GI() ? G->GI()->RunCounter : 1;
	DrawSlate(W * 0.5f, H * 0.42f - (1.f - In) * H * 0.6f, 1.f, Clap,
		{ TEXT("PROD. FULL THROTTLE"), FString::Printf(TEXT("SCENE %d   TAKE %d"), G->Scene + 1, G->Take), TEXT("STARRING YOU"), FString::Printf(TEXT("ROLL A-%d"), 40 + Run) });
	const FFTLevelDef& L = G->Def();
	if (G->Take > 1)
	{
		Txt(FString::Printf(TEXT("CUT! AGAIN. SCENE %d, TAKE %d"), G->Scene + 1, G->Take), W * 0.5f, H * 0.74f, Red, 1.3f, 1, true);
		Txt(TEXT("RESET THE CARS. FRESH PAINT ON THE KESTREL. ROLLING..."), W * 0.5f, H * 0.82f, White, 0.95f, 1);
	}
	else
	{
		Txt(FString::Printf(TEXT("SCENE %d OF %d  ·  %s"), G->Scene + 1, G->NumScenes(), *L.Scenes[G->Scene].Name), W * 0.5f, H * 0.74f, Yellow, 1.25f, 1, true);
		Txt(FString::Printf(TEXT("LEVEL %d: %s  ·  %s"), L.Index + 1, *L.Title, *L.Place), W * 0.5f, H * 0.82f, White, 0.95f, 1);
	}
	if (P > 1.2f) Txt(TEXT("ENTER TO SKIP"), W - 16 * S, H - 30 * S, Grey, 0.7f, 2);
}

// ---------------------------------------------------------------------------------------------- play
void AFTHUD::DrawPlay(AFTGameMode* G)
{
	const FFTLevelDef& L = G->Def();
	const float Now = T;
	// viewfinder brackets
	const float Bk = 40 * S, Th = 2.f;
	const FLinearColor BC(1, 1, 1, 0.35f);
	for (int32 c = 0; c < 4; ++c)
	{
		const float X = (c % 2 == 0) ? 20 * S : W - 20 * S - Bk;
		const float Y = (c < 2) ? 20 * S : H - 20 * S - Bk;
		Box(c % 2 == 0 ? X : X + Bk - Th * S, Y, Th * S, Bk, BC);
		Box(X, c < 2 ? Y : Y + Bk - Th * S, Bk, Th * S, BC);
	}
	// film-strip progress
	const float Pw = 300 * S, Px = W * 0.5f - Pw * 0.5f, Py = 22 * S;
	Box(Px - 6 * S, Py - 6 * S, Pw + 12 * S, 30 * S, Ink);
	for (int32 i = 0; i < 30; ++i) { Box(Px + i * Pw / 30 + 2 * S, Py - 4 * S, 5 * S, 3 * S, FLinearColor(1, 1, 1, 0.3f)); Box(Px + i * Pw / 30 + 2 * S, Py + 19 * S, 5 * S, 3 * S, FLinearColor(1, 1, 1, 0.3f)); }
	Box(Px, Py + 2 * S, Pw * G->Progress, 14 * S, Yellow);
	for (const FFTScene& Sc : L.Scenes) Box(Px + Pw * FMath::Clamp(Sc.D / L.FinaleD, 0.f, 1.f) * 0.85f, Py, 1.5f * S, 18 * S, FLinearColor(1, 1, 1, 0.5f));
	Box(Px + Pw * 0.85f, Py - 2 * S, 3 * S, 22 * S, Pink);
	Txt(FString::Printf(TEXT("SCENE %d/%d"), G->Scene + 1, G->NumScenes()), Px - 12 * S, Py, White, 0.75f, 2);
	Txt(TEXT("★ FINALE"), Px + Pw + 12 * S, Py, Pink, 0.75f);
	if (G->bBossOn)
	{
		Txt(G->BossLabel, W * 0.5f, Py + 30 * S, Red, 1.f, 1, true);
		Box(Px, Py + 58 * S, Pw, 9 * S, Ink);
		Box(Px, Py + 58 * S, Pw * G->BossHpFrac, 9 * S, Red);
		// the GAP bar (05 §3): fills as his escape clock runs, pulses while the clock runs slow
		const float Gf = FMath::Clamp(G->BossClock / FMath::Max(1.f, G->BossEscapeT), 0.f, 1.f);
		const float Pulse = G->BossRate < 1.f ? 0.55f + 0.45f * FMath::Sin(T * 9.f) : 1.f;
		Box(Px, Py + 70 * S, Pw, 6 * S, Ink);
		Box(Px, Py + 70 * S, Pw * Gf, 6 * S, G->bLastChance ? FLinearColor(1.f, 0.2f, 0.2f, Pulse) : FLinearColor(1.f, 0.82f, 0.25f, Pulse));
		Txt(G->bLastChance ? TEXT("LAST CHANCE!") : (G->BossRate < 1.f ? FString::Printf(TEXT("GAP  x%.2f"), G->BossRate) : FString(TEXT("GAP"))), Px + Pw + 10 * S, Py + 64 * S, G->bLastChance ? Red : Yellow, 0.6f);
	}
	// director's megaphone bar
	if (!G->Line.IsEmpty() && Now - G->LineT < G->LineHold + 1.2f)
	{
		const int32 Chars = FMath::Min(G->Line.Len(), FMath::FloorToInt((Now - G->LineT) * 70.f));
		const FLinearColor Acc = G->LinePri >= 3 ? Yellow : (G->LinePri >= 2 ? Pink : Cyan);
		const float By = (G->bBossOn ? 114 : 72) * S;
		const FVector2D M = Measure(G->Line, 1.05f, false);
		const float Bw = FMath::Max(360 * S, M.X + 120 * S);
		Box(W * 0.5f - Bw / 2, By, Bw, 40 * S, Ink);
		Box(W * 0.5f - Bw / 2, By, 5 * S, 40 * S, Acc);
		Txt(TEXT("DIRECTOR"), W * 0.5f - Bw / 2 + 14 * S, By + 11 * S, Acc, 0.62f);
		Txt(G->Line.Left(Chars), W * 0.5f - Bw / 2 + 92 * S, By + 8 * S, White, 1.05f);
	}
	// mini slate + ACTION!
	if (Now - G->SceneSlateT < 2.8f)
	{
		const float A = FMath::Clamp((2.8f - (Now - G->SceneSlateT)) * 2.f, 0.f, 1.f);
		Box(W * 0.5f - 210 * S, H * 0.2f, 420 * S, 56 * S, FLinearColor(0, 0, 0, 0.7f * A));
		Txt(FString::Printf(TEXT("SCENE %d · TAKE %d"), G->Scene + 1, G->Take), W * 0.5f, H * 0.2f + 4 * S, FLinearColor(Yellow.R, Yellow.G, Yellow.B, A), 0.8f, 1);
		Txt(L.Scenes[G->Scene].Name, W * 0.5f, H * 0.2f + 22 * S, FLinearColor(1, 1, 1, A), 1.2f, 1, true);
	}
	if (Now >= G->ActionT && Now - G->ActionT < 1.0f) Txt(TEXT("ACTION!"), W * 0.5f, H * 0.32f, Yellow, 3.f, 1, true);
	else if (Now - G->ShoutT < 1.0f) Txt(G->ShoutWord, W * 0.5f, H * 0.3f, FLinearColor(1.f, 0.9f, 0.3f, 1.f - (Now - G->ShoutT)), 2.2f, 1, true);
	if (Now - G->CaptionT < 3.f && Now >= G->CaptionT) Txt(G->Caption, W * 0.5f, H * 0.62f, Red, 1.1f, 1, true);
	// popups
	for (int32 i = 0; i < G->Popups.Num(); ++i)
	{
		const FFTPopup& P = G->Popups[i];
		const float A = FMath::Clamp(1.6f - P.T, 0.f, 1.f);
		Txt(P.Text, W * 0.5f, H * 0.56f - (G->Popups.Num() - 1 - i) * 30 * S - P.T * 30 * S, FLinearColor(P.Col.R, P.Col.G, P.Col.B, A), 0.95f * P.Big, 1, true);
	}

	// ---- spy dashboard (left)
	const float Lx = 30 * S, Ly0 = H - 312 * S, Lw = 250 * S;
	Panel(Lx, Ly0, Lw, 284 * S);
	const float Ly = Ly0;
	Txt(TEXT("◉ KESTREL-88"), Lx + 12 * S, Ly + 8 * S, Pink, 0.95f, 0, true);
	Txt(TEXT("AGENT N. KANE"), Lx + 12 * S, Ly + 34 * S, Grey, 0.7f);
	const float Hp = G->HpPct();   // the bar shows a share of max (Armor Plating raises the max)
	Txt(FString::Printf(TEXT("ARMOR %d%%  (%.0f/%.0f)"), FMath::RoundToInt(Hp), G->Hp, G->MaxHp()), Lx + 12 * S, Ly + 54 * S, White, 0.8f);
	const FLinearColor HC = Hp > 60 ? Green : (Hp > 30 ? Amber : Red);
	const bool bBlink = Hp <= 25 && FMath::Fmod(Now, 0.5f) < 0.25f;
	for (int32 i = 0; i < 10; ++i) Box(Lx + 12 * S + i * 22 * S, Ly + 74 * S, 18 * S, 12 * S, (i * 10 < Hp && !bBlink) ? HC : FLinearColor(1, 1, 1, 0.12f));
	const TCHAR* DmgTxt = Hp > 79 ? TEXT("ALL SYSTEMS NOMINAL") : (Hp > 59 ? TEXT("BODYWORK DAMAGED") : (Hp > 39 ? TEXT("WINDSHIELD CRACKED") : (Hp > 24 ? TEXT("ENGINE SMOKING") : TEXT("CRITICAL — ON FIRE"))));
	Txt(DmgTxt, Lx + 12 * S, Ly + 90 * S, HC, 0.65f);
	const UFTGameInstance* GI = G->GI();
	auto KeyName = [GI](EFTAction A) { return GI && GI->Binds[(int32)A][GI->Layout].IsValid() ? GI->Binds[(int32)A][GI->Layout].GetDisplayName().ToString().ToUpper() : FString(TEXT("-")); };
	Txt(FString::Printf(TEXT("MISSILES [%s]"), *KeyName(EFTAction::Missile)), Lx + 12 * S, Ly + 110 * S, White, 0.75f);
	for (int32 i = 0, Cap = G->MissileCap(); i < Cap; ++i) Box(Lx + 12 * S + i * 18 * S, Ly + 130 * S, 12 * S, 8 * S, i < G->Missiles ? Amber : FLinearColor(1, 1, 1, 0.12f));
	{
		// 06 §5: AMMO [L]  AP 37, with a three-pip STD / AP / HE selector
		static const TCHAR* LN[3] = { TEXT("STD"), TEXT("AP"), TEXT("HE") };
		static const FLinearColor LC[3] = { FLinearColor(1.f, 0.82f, 0.25f), FLinearColor(0.62f, 0.91f, 1.f), FLinearColor(1.f, 0.48f, 0.1f) };
		const int32 Ld = G->AmmoLoad;
		const FString Cnt = Ld > 0 ? FString::Printf(TEXT("  %d"), G->Run.Rounds[Ld - 1]) : FString();
		Txt(FString::Printf(TEXT("AMMO [%s]  %s%s"), *KeyName(EFTAction::Ammo), LN[Ld], *Cnt), Lx + 12 * S, Ly + 141 * S, LC[Ld], 0.72f);
		for (int32 i = 0; i < 3; ++i)
		{
			const bool bOwned = i == 0 || (i == 1 ? G->Stats.ApRank > 0 : G->Stats.HeRank > 0);
			Box(Lx + 170 * S + i * 22 * S, Ly + 146 * S, 16 * S, 8 * S, i == Ld ? LC[i] : FLinearColor(1, 1, 1, bOwned ? 0.25f : 0.07f));
		}
	}
	const TCHAR* TurboSt = G->bTurboOn ? TEXT("BURN") : (G->BoostT > 0 ? TEXT("BOOST") : (G->bTurboLock ? TEXT("RECHARGING") : (G->Turbo >= 99.f ? TEXT("READY") : TEXT("CHARGING"))));
	Txt(FString::Printf(TEXT("TURBO [%s]  %s%s"), *KeyName(EFTAction::Turbo), TurboSt, G->bDraft ? TEXT("  DRAFT") : TEXT("")), Lx + 12 * S, Ly + 166 * S, G->bDraft ? Cyan : White, 0.75f);
	Box(Lx + 12 * S, Ly + 186 * S, 220 * S, 9 * S, FLinearColor(1, 1, 1, 0.12f));
	Box(Lx + 12 * S, Ly + 186 * S, 220 * S * FMath::Clamp(G->Turbo / FMath::Max(1.f, G->Stats.TurboTank), 0.f, 1.f), 9 * S, G->bTurboOn ? Pink : Cyan);
	static const FLinearColor DC[3] = { FLinearColor(0.37f, 0.82f, 1.f), FLinearColor(1.f, 0.65f, 0.24f), FLinearColor(1.f, 0.31f, 0.72f) };
	const TCHAR* DriftSt = G->BoostT > 0 ? TEXT("BOOST!") : (!G->bDrifting ? TEXT("HOLD + STEER") : (G->DriftLevel == 0 ? TEXT("SLIDING") : (G->DriftLevel == 1 ? TEXT("BLUE") : (G->DriftLevel == 2 ? TEXT("ORANGE") : TEXT("PINK!")))));
	Txt(FString::Printf(TEXT("DRIFT [%s]  %s"), *KeyName(EFTAction::Drift), DriftSt), Lx + 12 * S, Ly + 200 * S, White, 0.75f);
	for (int32 i = 0; i < 3; ++i) Box(Lx + 12 * S + i * 26 * S, Ly + 220 * S, 20 * S, 9 * S, i < G->DriftLevel ? DC[i] : FLinearColor(1, 1, 1, 0.12f));
	Txt(FString::Printf(TEXT("%d"), FMath::RoundToInt(G->SpeedMph)), Lx + 12 * S, Ly + 232 * S, White, 1.6f, 0, true);
	Txt(TEXT("MPH"), Lx + 92 * S, Ly + 254 * S, Grey, 0.7f);
	{
		// tacho arc: full scale 900 su (05 §1) so the Maxed build fits
		const FVector2D C(Lx + 190 * S, Ly + 258 * S);
		const float R = 34 * S, Frac = FMath::Clamp(G->SpeedMph / (900.f * 0.31f), 0.f, 1.f);
		for (int32 i = 0; i < 20; ++i)
		{
			const float A0 = PI * (1.f + i / 20.f), A1 = PI * (1.f + (i + 1) / 20.f);
			FCanvasLineItem Ln(C + FVector2D(FMath::Cos(A0), FMath::Sin(A0)) * R, C + FVector2D(FMath::Cos(A1), FMath::Sin(A1)) * R);
			Ln.SetColor(i / 20.f < Frac ? (i > 15 ? Pink : Cyan) : FLinearColor(1, 1, 1, 0.15f));
			Ln.LineThickness = 4.f * S;
			Canvas->DrawItem(Ln);
		}
	}
	if (G->bShowKeys && GI)
	{
		Txt(GI->Layout == 0 ? TEXT("WASD steer · SHIFT drift · SPACE turbo · J guns · K missile · L ammo · P pause")
			: TEXT("ARROWS steer · SHIFT drift · SPACE turbo · Z guns · X missile · C ammo · P pause"), W * 0.5f, H - 40 * S, FLinearColor(1, 1, 1, 0.8f), 0.8f, 1);
	}

	// ---- NOW FILMING (right)
	const float Rw = 250 * S, Rx = W - 30 * S - Rw, Ry = H - 290 * S;
	Panel(Rx, Ry, Rw, 262 * S);
	if (FMath::Fmod(Now, 1.f) < 0.6f) { Box(Rx + 12 * S, Ry + 12 * S, 12 * S, 12 * S, Red); }
	Txt(TEXT("REC  NOW FILMING"), Rx + 30 * S, Ry + 8 * S, Red, 0.8f, 0, true);
	const int32 Tc = FMath::FloorToInt(G->Timecode);
	Txt(FString::Printf(TEXT("TC 00:%02d:%02d:%02d"), Tc / 60, Tc % 60, FMath::FloorToInt(FMath::Fmod(G->Timecode, 1.f) * 24.f)), Rx + 12 * S, Ry + 34 * S, Grey, 0.7f);
	Txt(FString::Printf(TEXT("SCENE %d · TAKE %d"), G->Scene + 1, G->Take), Rx + 12 * S, Ry + 54 * S, White, 0.8f);
	Txt(L.Scenes[G->Scene].Name, Rx + 12 * S, Ry + 72 * S, Pink, 0.85f, 0, true);
	Txt(TEXT("FILM LEFT"), Rx + 12 * S, Ry + 100 * S, Grey, 0.7f);
	for (int32 i = 0, NR = FMath::Min(G->Stats.Reels, 6); i < NR; ++i)
	{
		const bool bLeft = i < G->Stats.Reels - G->Retakes;
		const FVector2D C(Rx + 100 * S + i * 24 * S, Ry + 108 * S);
		FCanvasNGonItem Reel(C, FVector2D(10 * S, 10 * S), 10, bLeft ? Yellow : FLinearColor(1, 1, 1, 0.15f));
		Canvas->DrawItem(Reel);
	}
	Txt(TEXT("BOX OFFICE"), Rx + 12 * S, Ry + 128 * S, Grey, 0.7f);
	Txt(FString::Printf(TEXT("$%s"), *Num(G->Score)), Rx + 12 * S, Ry + 144 * S, Yellow, 1.35f, 0, true);
	if (G->Combo > 1)
	{
		Txt(FString::Printf(TEXT("COMBO x%d"), G->Combo), Rx + 12 * S, Ry + 184 * S, Pink, 1.f, 0, true);
		Box(Rx + 12 * S, Ry + 210 * S, 220 * S * FMath::Clamp(G->ComboT / 3.f, 0.f, 1.f), 6 * S, Pink);
	}
	else Txt(FString::Printf(TEXT("KILLS %d   NEAR MISSES %d"), G->Kills, G->NearMisses), Rx + 12 * S, Ry + 188 * S, White, 0.75f);
	Txt(FString::Printf(TEXT("CARNAGE %d%%  ·  %d:%02d / PAR %d:%02d"), FMath::RoundToInt(100.f * G->Run.Wrecked / FMath::Max(1, G->ScriptTotal)), FMath::FloorToInt(G->LevelClock) / 60, FMath::FloorToInt(G->LevelClock) % 60,
		FFTProgressData::Get().Levels.IsValidIndex(G->Level) ? FFTProgressData::Get().Levels[G->Level].Par / 60 : 0, FFTProgressData::Get().Levels.IsValidIndex(G->Level) ? FFTProgressData::Get().Levels[G->Level].Par % 60 : 0), Rx + 12 * S, Ry + 244 * S, Grey, 0.62f);
	Txt(FString::Printf(TEXT("STUNTS %d   COLLATERAL %d"), G->StuntCount, G->Collateral), Rx + 12 * S, Ry + 226 * S, White, 0.75f);
}

void AFTHUD::DrawCut(AFTGameMode* G)
{
	if (T < G->CutT) return;
	const float A = FMath::Clamp((T - G->CutT) * 3.f, 0.f, 1.f);
	Box(0, H * 0.36f, W, H * 0.24f, FLinearColor(0, 0, 0, 0.6f * A));
	Txt(G->CutTitle, W * 0.5f, H * 0.37f, FLinearColor(1.f, 0.25f, 0.25f, A), 3.4f, 1, true);
	Txt(G->CutSub, W * 0.5f, H * 0.37f + 92 * S, FLinearColor(1, 1, 1, A), 1.15f, 1);
}

// ---------------------------------------------------------------------------------------------- reel
void AFTHUD::DrawReel(AFTGameMode* G)
{
	const float Bar = (H - W / 2.39f) * 0.5f;
	Box(0, 0, W, FMath::Max(Bar, 40 * S), FLinearColor(0, 0, 0, 1));
	Box(0, H - FMath::Max(Bar, 40 * S), W, FMath::Max(Bar, 40 * S), FLinearColor(0, 0, 0, 1));
	if (FMath::FRand() < 0.08f) Box(FMath::FRand() * W, 0, 1.5f * S, H, FLinearColor(1, 1, 1, 0.12f));
	Box(0, 0, W, H, FLinearColor(1.f, 0.9f, 0.7f, 0.03f + 0.02f * FMath::FRand()));
	const FFTLevelDef& L = G->Def();
	if (G->ReelTitleT > 0.f)
	{
		Box(0, 0, W, H, FLinearColor(0, 0, 0, 0.92f));
		Txt(TEXT("ROADBURN PICTURES PRESENTS"), W * 0.5f, H * 0.36f, Grey, 0.9f, 1);
		Txt(TEXT("THE HIGHLIGHTS"), W * 0.5f, H * 0.42f, Yellow, 2.4f, 1, true);
		Txt(FString::Printf(TEXT("LEVEL %d: %s · %s"), L.Index + 1, *L.Zone, *L.Title), W * 0.5f, H * 0.56f, White, 1.f, 1);
		DrawReelContinue(G);
		return;
	}
	Txt(TEXT("● DAILIES · HIGHLIGHTS"), 24 * S, 10 * S, Red, 0.85f, 0, true);
	Txt(FString::Printf(TEXT("SHOT %d / %d   %s"), G->ReelShot, G->ReelShots, *G->ReelCamName), W - 24 * S, 12 * S, White, 0.8f, 2);
	Txt(G->ReelLabel, 40 * S, H - 80 * S, Yellow, 1.3f, 0, true);
	if (G->bReelSlow) Txt(TEXT("▶▶ SLOW MOTION"), W - 40 * S, H - 74 * S, Pink, 0.9f, 2);
	if (T - G->ReelLoopT < 2.f) Txt(TEXT("AGAIN, FROM THE TOP"), W * 0.5f, H * 0.2f, FLinearColor(1, 1, 1, FMath::Clamp(2.f - (T - G->ReelLoopT), 0.f, 1.f)), 1.2f, 1, true);
	DrawReelContinue(G);
}

void AFTHUD::DrawReelContinue(AFTGameMode* G)
{
	// the only way on: a clearly drawn CONTINUE button in the bottom letterbox bar
	const float Bar = FMath::Max((H - W / 2.39f) * 0.5f, 70 * S);
	const float Bh = FMath::Min(58 * S, Bar - 12 * S);
	const bool bArmed = G->PhaseT > AFTGameMode::ReelArmT;
	const AFTPlayerController* PC = Cast<AFTPlayerController>(GetOwningPlayerController());
	const bool bHover = PC && bArmed && PC->MouseIn(G->ReelContinueRect);
	// 10 §2: mid-reel the button reads HOLD TO SKIP (a 1 s hold fills the ring); once played, CONTINUE
	if (G->bReelDone) G->ReelContinueRect = Button(W * 0.5f, H - Bar * 0.5f, 360 * S, Bh, TEXT("CONTINUE  ▶"), TEXT("ENTER  ·  PAD A  ·  CLICK HERE"), bArmed, true);
	else
	{
		G->ReelContinueRect = Button(W * 0.5f, H - Bar * 0.5f, 360 * S, Bh, TEXT("HOLD TO SKIP"), TEXT("HOLD ENTER  ·  PAD A  ·  OR THIS BUTTON"), bArmed, bHover || G->ReelHoldT > 0.f);
		const float Hold = FFTProgressData::Get().T(TEXT("HoldToSkip"), 1.f);
		Ring(W * 0.5f + 200 * S, H - Bar * 0.5f, Bh * 0.38f, FMath::Clamp(G->ReelHoldT / Hold, 0.f, 1.f), Yellow, 4.f);
	}
}

// ---------------------------------------------------------------------------------------------- dailies
void AFTHUD::DrawRating(AFTGameMode* G)
{
	Box(0, 0, W, H, FLinearColor(0.01f, 0.f, 0.03f, 0.8f));
	Txt(TEXT("THE DAILIES"), W * 0.5f, H * 0.08f, Yellow, 2.4f, 1, true);
	const FFTLevelDef& L = G->Def();
	Txt(FString::Printf(TEXT("LEVEL %d: %s · %s"), L.Index + 1, *L.Zone, *L.Title), W * 0.5f, H * 0.19f, White, 0.95f, 1);
	struct FRow { const TCHAR* N; int64 V; };
	const FRow Rows[] = { { TEXT("DESTRUCTION"), G->ScoreDestruction }, { TEXT("NEAR MISSES"), G->ScoreNear }, { TEXT("STUNTS"), G->ScoreStunts }, { TEXT("ARMOR BONUS"), G->ArmorBonus }, { TEXT("COLLATERAL"), G->ScoreCollateral } };
	const float X0 = W * 0.5f - 260 * S, X1 = W * 0.5f + 260 * S;
	for (int32 i = 0; i < 5; ++i)
	{
		if (G->PhaseT < 0.3f + i * 0.3f) break;
		Txt(Rows[i].N, X0, H * 0.28f + i * 36 * S, White, 1.f);
		Txt(FString::Printf(TEXT("$%s"), *Num(Rows[i].V)), X1, H * 0.28f + i * 36 * S, Rows[i].V < 0 ? Red : Cyan, 1.f, 2);
	}
	if (G->PhaseT > 1.9f)
	{
		Box(X0, H * 0.28f + 5 * 36 * S, X1 - X0, 2 * S, White);
		Txt(TEXT("BOX OFFICE"), X0, H * 0.28f + 5.3f * 36 * S, Yellow, 1.3f, 0, true);
		Txt(FString::Printf(TEXT("$%s"), *Num(G->Score)), X1, H * 0.28f + 5.3f * 36 * S, Yellow, 1.3f, 2, true);
	}
	if (G->PhaseT > 2.7f)
	{
		static const TCHAR* Names[] = { TEXT("B-MOVIE"), TEXT("ACTION HERO"), TEXT("BLOCKBUSTER"), TEXT("LEGENDARY DIRECTOR") };
		static const FLinearColor Cols[] = { FLinearColor(0.79f, 0.71f, 0.54f), FLinearColor(0.21f, 0.95f, 1.f), FLinearColor(1.f, 0.24f, 0.65f), FLinearColor(1.f, 0.82f, 0.25f) };
		const float Sc = FMath::Lerp(2.4f, 1.6f, FMath::Clamp((G->PhaseT - 2.7f) * 5.f, 0.f, 1.f));
		Txt(TEXT("DIRECTOR'S RATING"), W * 0.5f, H * 0.66f, Grey, 0.85f, 1);
		Txt(Names[G->RatingIdx], W * 0.5f, H * 0.7f, Cols[G->RatingIdx], Sc, 1, true);
		FString Stars; for (int32 i = 0; i < 4; ++i) Stars += i <= G->RatingIdx ? TEXT("★ ") : TEXT("☆ ");
		Txt(Stars, W * 0.5f, H * 0.8f, Cols[G->RatingIdx], 1.2f, 1);
		Txt(G->RatingQuip, W * 0.5f, H * 0.86f, White, 0.9f, 1);
	}
	if (G->PhaseT > 3.0f)
	{
		if (G->bOutOfFilm) Txt(TEXT("OUT OF FILM: THE KILL FEE ONLY. THE LEVEL IS RESHOT."), W * 0.5f, H * 0.895f, Red, 0.85f, 1);
		else if (G->bEscaped) Txt(TEXT("HE GOT AWAY: RATED AT MOST ACTION HERO. THE LEVEL IS RESHOT."), W * 0.5f, H * 0.895f, Amber, 0.85f, 1);
	}
	if (G->PhaseT > 2.9f && FMath::Fmod(T, 1.f) < 0.7f) Txt(TEXT("ENTER / A  CONTINUE TO THE PAYCHECK"), W * 0.5f, H * 0.925f, White, 0.95f, 1, true);
	Txt(TEXT("R  RESHOOT   ·   ESC / B  TITLE   (BOTH THROUGH PRE-PRODUCTION)"), W * 0.5f, H * 0.96f, Grey, 0.8f, 1);
}

// ---------------------------------------------------------------------------------------------- garage
TArray<FString> AFTHUD::WrapText(const FString& Str, float MaxW, float Scale)
{
	TArray<FString> Words, Lines;
	Str.ParseIntoArrayWS(Words);
	FString Cur;
	for (const FString& Wd : Words)
	{
		const FString Try = Cur.IsEmpty() ? Wd : Cur + TEXT(" ") + Wd;
		if (!Cur.IsEmpty() && Measure(Try, Scale, false).X > MaxW) { Lines.Add(Cur); Cur = Wd; }
		else Cur = Try;
	}
	if (!Cur.IsEmpty()) Lines.Add(Cur);
	return Lines;
}

FBox2D AFTHUD::Button(float Cx, float Cy, float Wd, float Ht, const FString& Label, const FString& Sub, bool bArmed, bool bHot)
{
	const float X = Cx - Wd * 0.5f, Y = Cy - Ht * 0.5f;
	const float Pulse = 0.5f + 0.5f * FMath::Sin(T * 5.f);
	const FLinearColor Edge = !bArmed ? FLinearColor(1, 1, 1, 0.25f) : (bHot ? Yellow : FLinearColor(Pink.R, Pink.G, Pink.B, 0.65f + 0.35f * Pulse));
	Box(X, Y, Wd, Ht, !bArmed ? FLinearColor(0.05f, 0.04f, 0.08f, 0.85f) : (bHot ? FLinearColor(0.42f, 0.08f, 0.32f, 0.95f) : FLinearColor(0.2f, 0.03f, 0.18f, 0.92f)));
	Frame(X, Y, Wd, Ht, Edge, bHot ? 3.f : 2.f);
	if (bArmed) Frame(X - 4 * S, Y - 4 * S, Wd + 8 * S, Ht + 8 * S, FLinearColor(Edge.R, Edge.G, Edge.B, 0.25f + 0.2f * Pulse), 1.f);
	const FLinearColor Tc = bArmed ? (bHot ? Yellow : White) : FLinearColor(1, 1, 1, 0.35f);
	Txt(Label, Cx, Y + Ht * 0.12f, Tc, 1.15f, 1, true);
	if (!Sub.IsEmpty()) Txt(Sub, Cx, Y + Ht * 0.6f, bArmed ? FLinearColor(1.f, 0.97f, 0.92f, 0.95f) : FLinearColor(1, 1, 1, 0.3f), 0.8f, 1);
	return FBox2D(FVector2D(X, Y), FVector2D(X + Wd, Y + Ht));
}

void AFTHUD::Ring(float Cx, float Cy, float Rad, float Frac, const FLinearColor& C, float Th)
{
	const int32 N = 32;
	for (int32 i = 0; i < N; ++i)
	{
		const float A0 = -PI * 0.5f + 2.f * PI * i / N, A1 = -PI * 0.5f + 2.f * PI * (i + 1) / N;
		FCanvasLineItem Ln(FVector2D(Cx, Cy) + FVector2D(FMath::Cos(A0), FMath::Sin(A0)) * Rad, FVector2D(Cx, Cy) + FVector2D(FMath::Cos(A1), FMath::Sin(A1)) * Rad);
		Ln.SetColor((float)i / N < Frac ? C : FLinearColor(1, 1, 1, 0.18f));
		Ln.LineThickness = Th * S;
		Canvas->DrawItem(Ln);
	}
}

// ---------------------------------------------------------------------------------------------- THE DAILIES + PAYCHECK (10 §3)
void AFTHUD::DrawPaycheck(AFTGameMode* G)
{
	Box(0, 0, W, H, FLinearColor(0.01f, 0.f, 0.03f, 0.86f));
	Txt(TEXT("PAYCHECK"), W * 0.5f, H * 0.04f, Yellow, 2.2f, 1, true);
	const FFTLevelDef& L = G->Def();
	Txt(FString::Printf(TEXT("LEVEL %d: %s · %s"), L.Index + 1, *L.Zone, *L.Title), W * 0.5f, H * 0.135f, White, 0.9f, 1);
	if (!G->bPayValid) { Txt(TEXT("NO PAYCHECK FOR A BENCHMARK RUN"), W * 0.5f, H * 0.4f, Grey, 1.f, 1); return; }
	const FFTCommitOut& P = G->Pay;
	const float X0 = W * 0.08f, X1 = W * 0.5f, Y0 = H * 0.2f, Lh = 30 * S;
	const int32 Shown = FMath::Clamp(FMath::FloorToInt((G->PhaseT - 0.3f) / 0.32f), 0, P.Pay.Lines.Num());
	Txt(TEXT("CASH"), X0, Y0 - 26 * S, Grey, 0.75f);
	for (int32 i = 0; i < Shown; ++i)
	{
		const FFTPayLine& Ln = P.Pay.Lines[i];
		const float Y = Y0 + i * Lh;
		Txt(Ln.Label, X0, Y, Ln.bMult ? Cyan : White, 0.82f);
		if (Ln.bMult) Txt(FString::Printf(TEXT("x %.3g"), Ln.Mult), X1 - 150 * S, Y, Pink, 0.82f, 2);
		const int64 Amt = (int64)(FMath::RoundToDouble(Ln.Amount / 10.0) * 10.0);
		Txt(Money(Amt), X1, Y, Amt < 0 ? Red : (Ln.bMult ? Pink : Cyan), 0.82f, 2);
	}
	const float Yt = Y0 + P.Pay.Lines.Num() * Lh + 8 * S;
	if (Shown >= P.Pay.Lines.Num())
	{
		Box(X0, Yt, X1 - X0, 2 * S, White);
		Txt(TEXT("TOTAL"), X0, Yt + 8 * S, Yellow, 1.4f, 0, true);
		Txt(Money(P.Pay.Total), X1, Yt + 8 * S, Yellow, 1.4f, 2, true);
		if (const UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(G)) Txt(FString::Printf(TEXT("CASH NOW %s"), *Money(CS->C.Cash)), X1, Yt + 52 * S, Grey, 0.75f, 2);
	}
	// XP, then the bar fills and each LEVEL UP stamps its skill points
	const float Tx = W * 0.56f, Tw = W * 0.36f;
	const float TXp = 0.6f + P.Pay.Lines.Num() * 0.32f;
	if (G->PhaseT > TXp)
	{
		Txt(TEXT("XP"), Tx, Y0 - 26 * S, Grey, 0.75f);
		for (int32 i = 0; i < P.XPLines.Num(); ++i) { Txt(P.XPLines[i].Label, Tx, Y0 + i * 26 * S, White, 0.72f); Txt(FString::Printf(TEXT("+%s XP"), *Num((int64)P.XPLines[i].Amount)), Tx + Tw, Y0 + i * 26 * S, Green, 0.72f, 2); }
		const float Yb = Y0 + FMath::Max(1, P.XPLines.Num()) * 26 * S + 14 * S;
		const FFTProgressData& D = FFTProgressData::Get();
		const float U = FMath::Clamp((G->PhaseT - TXp - 0.4f) / 1.4f, 0.f, 1.f);
		const int64 Xp = P.XPBefore + (int64)(P.XPGained * U);
		const int32 Lv = D.LevelForXP(Xp);
		const int64 Lo = D.TotalXPFor(Lv), Hi = Lv >= 30 ? Lo : D.TotalXPFor(Lv + 1);
		Txt(FString::Printf(TEXT("DRIVER LEVEL %d"), Lv), Tx, Yb, Yellow, 1.f, 0, true);
		Txt(FString::Printf(TEXT("%s / %s XP"), *Num(Xp), *Num(Hi)), Tx + Tw, Yb + 6 * S, Grey, 0.7f, 2);
		Box(Tx, Yb + 34 * S, Tw, 12 * S, FLinearColor(1, 1, 1, 0.12f));
		Box(Tx, Yb + 34 * S, Tw * (Hi > Lo ? (float)(Xp - Lo) / (float)(Hi - Lo) : 1.f), 12 * S, Green);
		float Ys = Yb + 60 * S;
		for (const TPair<int32, int32>& Up : P.LevelUps)
		{
			if (Lv < Up.Key) break;
			const float Pop = FMath::Clamp((G->PhaseT - TXp - 0.4f - 1.4f * (float)(D.TotalXPFor(Up.Key) - P.XPBefore) / FMath::Max<int64>(1, P.XPGained)) * 4.f, 0.f, 1.f);
			Box(Tx, Ys, Tw, 34 * S, FLinearColor(0.3f, 0.05f, 0.25f, 0.9f * Pop));
			Frame(Tx, Ys, Tw, 34 * S, FLinearColor(Yellow.R, Yellow.G, Yellow.B, Pop), 2.f);
			Txt(FString::Printf(TEXT("LEVEL UP!  LV %d   +%d SKILL POINT%s"), Up.Key, Up.Value, Up.Value == 1 ? TEXT("") : TEXT("S")), Tx + Tw * 0.5f, Ys + 6 * S, FLinearColor(1.f, 0.9f, 0.4f, Pop), 0.9f, 1, true);
			Ys += 40 * S;
		}
		if (U >= 1.f && P.LegendarySP > 0) { Txt(TEXT("FIRST LEGENDARY DIRECTOR HERE: +1 SKILL POINT"), Tx, Ys, Yellow, 0.8f, 0, true); Ys += 30 * S; }
		if (U >= 1.f) for (const FString& Un : P.Unlocks) { Txt(Un, Tx, Ys, Cyan, 0.7f, 0, true); Ys += 24 * S; if (Ys > H * 0.86f) break; }
	}
	if (G->PhaseT > 1.f && FMath::Fmod(T, 1.f) < 0.7f) Txt(TEXT("ENTER / A  CONTINUE TO PRE-PRODUCTION"), W * 0.5f, H * 0.915f, White, 0.95f, 1, true);
	Txt(TEXT("R  RESHOOT   ·   ESC / B  TITLE   (BOTH THROUGH PRE-PRODUCTION)"), W * 0.5f, H * 0.955f, Grey, 0.8f, 1);
}

// ---------------------------------------------------------------------------------------------- PRE-PRODUCTION (10 §4)
void AFTHUD::DrawPreProduction(AFTGameMode* G)
{
	const UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(G);
	if (!CS) return;
	const FFTProgressData& D = FFTProgressData::Get();
	const AFTPlayerController* PC = Cast<AFTPlayerController>(GetOwningPlayerController());
	G->BuildTiles();
	Box(0, 0, W, H, FLinearColor(0.01f, 0.f, 0.03f, 0.88f));
	// header: next level, driver level, XP bar, skill points, cash
	Txt(G->PPHeader(), 40 * S, 18 * S, Yellow, 1.25f, 0, true);
	const int32 Lv = CS->C.Level;
	const int64 Lo = D.TotalXPFor(Lv), Hi = Lv >= 30 ? Lo : D.TotalXPFor(Lv + 1);
	const float Hx = W * 0.56f, Hw = W * 0.4f;
	Txt(FString::Printf(TEXT("DRIVER LV %d"), Lv), Hx, 14 * S, White, 1.f, 0, true);
	Txt(FString::Printf(TEXT("XP %s / %s"), *Num(CS->C.XP), *Num(Hi)), Hx + 170 * S, 20 * S, Grey, 0.7f);
	Box(Hx, 46 * S, Hw * 0.5f, 8 * S, FLinearColor(1, 1, 1, 0.12f));
	Box(Hx, 46 * S, Hw * 0.5f * (Hi > Lo ? FMath::Clamp((float)(CS->C.XP - Lo) / (float)(Hi - Lo), 0.f, 1.f) : 1.f), 8 * S, Green);
	Txt(FString::Printf(TEXT("SKILL POINTS %d / %d"), CS->SPLeft(), CS->SPEarned()), Hx + Hw * 0.55f, 14 * S, Cyan, 0.9f, 0, true);
	Txt(FString::Printf(TEXT("CASH %s"), *Money(CS->C.Cash)), Hx + Hw * 0.55f, 40 * S, Yellow, 0.9f, 0, true);
	// tabs
	static const TCHAR* TabN[5] = { TEXT("SKILLS"), TEXT("GARAGE"), TEXT("CUSTOM"), TEXT("SFX DEPT"), TEXT("EXTRAS") };
	G->TabRects.SetNum(5);
	const float TabY = H * 0.105f, TabW = 170 * S, TabH = 36 * S;
	for (int32 i = 0; i < 5; ++i)
	{
		const float X = 40 * S + i * (TabW + 10 * S);
		const bool bOn = (int32)G->Tab == i;
		Box(X, TabY, TabW, TabH, bOn ? FLinearColor(0.42f, 0.08f, 0.32f, 0.95f) : Ink);
		Frame(X, TabY, TabW, TabH, bOn ? Yellow : FLinearColor(1, 1, 1, 0.3f), bOn ? 2.5f : 1.f);
		Txt(TabN[i], X + TabW * 0.5f, TabY + 7 * S, bOn ? Yellow : White, 0.85f, 1, true);
		G->TabRects[i] = FBox2D(FVector2D(X, TabY), FVector2D(X + TabW, TabY + TabH));
	}
	Txt(TEXT("TAB / Q E / LB RB"), 40 * S + 5 * (TabW + 10 * S), TabY + 10 * S, Grey, 0.65f);

	// the board
	const float Bx = 40 * S, By = TabY + TabH + 34 * S, Bw = W * 0.64f, Bh = H * 0.86f - By;
	const int32 Sel = G->SelIndex();
	G->TileRects.SetNum(G->Tiles.Num());
	int32 Cols = 6, Rows = 7; float LabelW = 0.f;
	switch (G->Tab)
	{
	case EFTTab::Skills: Cols = 5; Rows = 6; break;
	case EFTTab::Garage: Cols = 3; Rows = 10; LabelW = Bw * 0.2f; break;
	case EFTTab::Extras: Cols = 4; Rows = 2; break;
	default: Cols = 6; Rows = 7; break;
	}
	// scroll the long shop grids so the selected tile shows
	int32& Scroll = G->TabScroll[(int32)G->Tab];
	if (G->Tiles.IsValidIndex(Sel)) { const int32 Rr = G->Tiles[Sel].Row; if (Rr < Scroll) Scroll = Rr; if (Rr >= Scroll + Rows) Scroll = Rr - Rows + 1; }
	if (G->Tab == EFTTab::Skills || G->Tab == EFTTab::Garage || G->Tab == EFTTab::Extras) Scroll = 0;
	const float Cw = (Bw - LabelW) / Cols, Ch = FMath::Min(Bh / Rows, G->Tab == EFTTab::Extras ? 150 * S : 96 * S);
	if (G->Tab == EFTTab::Skills)
		for (int32 b = 0; b < D.Branches.Num() && b < 5; ++b) Txt(D.Branches[b].Name, Bx + b * Cw + 6 * S, By - 24 * S, White, 0.75f, 0, true);
	static const FLinearColor RowCols[5] = { FLinearColor(0.36f, 1.f, 0.45f), FLinearColor(1.f, 0.82f, 0.25f), FLinearColor(1.f, 0.31f, 0.72f), FLinearColor(0.21f, 0.95f, 1.f), FLinearColor(1.f, 0.48f, 0.1f) };
	if (G->Tab == EFTTab::Garage)
	{
		static const TCHAR* LineN[10] = { TEXT("ARMOR PLATING"), TEXT("BULL BAR"), TEXT("GUN BARRELS"), TEXT("MISSILE RACKS"), TEXT("BIG-BLOCK V8"), TEXT("CLOSE-RATIO GEARBOX"), TEXT("SUPERCHARGER"), TEXT("RACE TYRES"), TEXT("AP ROUNDS"), TEXT("HE ROUNDS") };
		static const int32 RowG[10] = { 0, 0, 1, 1, 2, 2, 2, 3, 4, 4 };
		static const TCHAR* GrpN[5] = { TEXT("ARMOR"), TEXT("WEAPONS"), TEXT("ENGINE"), TEXT("TYRES"), TEXT("AMMO") };
		for (int32 r = 0; r < 10; ++r)
		{
			Txt(LineN[r], Bx, By + r * Ch + Ch * 0.18f, White, 0.66f);
			if (r == 0 || RowG[r] != RowG[r - 1]) Txt(GrpN[RowG[r]], Bx, By + r * Ch + Ch * 0.55f, RowCols[RowG[r]], 0.58f, 0, true);
		}
	}
	static const TCHAR* StN[6] = { TEXT("LOCKED"), TEXT(""), TEXT("PENDING"), TEXT("OWNED"), TEXT("EQUIPPED"), TEXT("COMING SOON") };
	int32 FirstAvail = -1;
	for (int32 i = 0; i < G->Tiles.Num(); ++i)
	{
		const FFTTileView& Tl = G->Tiles[i];
		G->TileRects[i] = FBox2D(ForceInit);
		if (Tl.Row < Scroll || Tl.Row >= Scroll + Rows) continue;
		const float X = Bx + LabelW + Tl.Col * Cw + 3 * S, Y = By + (Tl.Row - Scroll) * Ch + 3 * S, Tw = Cw - 6 * S, Th = Ch - 6 * S;
		const bool bSel = i == Sel;
		const bool bDim = Tl.State == EFTTile::Locked || Tl.State == EFTTile::Soon;
		FLinearColor Bg = Ink;
		if (Tl.State == EFTTile::Owned || Tl.State == EFTTile::Equipped) Bg = FLinearColor(Tl.Color.R * 0.35f, Tl.Color.G * 0.35f, Tl.Color.B * 0.35f, 0.92f);
		if (bSel) Bg = FLinearColor(0.32f, 0.06f, 0.28f, 0.95f);
		Box(X, Y, Tw, Th, Bg);
		if (Tl.State == EFTTile::Pending)
			for (int32 k = 0; k < 8; ++k) Box(X + k * Tw / 8.f, Y, Tw / 16.f, Th, FLinearColor(Tl.Color.R, Tl.Color.G, Tl.Color.B, 0.18f));   // striped
		const float Glow = bSel ? 0.7f + 0.3f * FMath::Sin(T * 6.f) : 1.f;
		Frame(X, Y, Tw, Th, bSel ? FLinearColor(1.f, 0.82f, 0.25f, Glow) : FLinearColor(Tl.Color.R, Tl.Color.G, Tl.Color.B, bDim ? 0.25f : 0.7f), bSel ? 3.f : 1.2f);
		if (Tl.bSwatch) Box(X + Tw - 22 * S, Y + 6 * S, 16 * S, 16 * S, Tl.Color);
		const bool bTight = Th < 90 * S;
		const float Fs = bTight ? 0.55f : 0.6f;
		const TArray<FString> TL = WrapText(Tl.Title, Tw - 30 * S, Fs);
		for (int32 l = 0; l < TL.Num() && l < (bTight ? 1 : 2); ++l) Txt(TL[l], X + 6 * S, Y + 4 * S + l * 17 * S, bDim ? Grey : White, Fs);
		Txt(Tl.Sub, X + 6 * S, bTight ? Y + Th * 0.5f - 9 * S : Y + Th - 38 * S, bDim ? FLinearColor(1, 1, 1, 0.35f) : Tl.Color, 0.52f);
		const FString StT = StN[(int32)Tl.State];
		if (!StT.IsEmpty()) Txt(StT, X + 6 * S, Y + Th - 20 * S, Tl.State == EFTTile::Locked || Tl.State == EFTTile::Soon ? Grey : (Tl.State == EFTTile::Pending ? Pink : Green), 0.52f, 0, true);
		else if (Tl.Needs.IsEmpty()) Txt(Tl.bCash ? TEXT("HOLD TO BUY") : TEXT("AVAILABLE"), X + 6 * S, Y + Th - 20 * S, Yellow, 0.52f, 0, true);
		if (Tl.State == EFTTile::Locked && !Tl.Needs.IsEmpty()) Txt(Tl.Needs, X + Tw - 6 * S, Y + Th - 20 * S, Grey, 0.48f, 2);
		G->TileRects[i] = FBox2D(FVector2D(X, Y), FVector2D(X + Tw, Y + Th));
		if (Tl.State == EFTTile::Available && FirstAvail < 0) FirstAvail = i;
	}
	if (G->Tab == EFTTab::Custom || G->Tab == EFTTab::Sfx)
	{
		const int32 MaxRow = G->Tiles.Num() ? G->Tiles.Last().Row : 0;
		if (MaxRow >= Rows) Txt(FString::Printf(TEXT("ROWS %d-%d OF %d  ·  ▲▼ SCROLL"), Scroll + 1, FMath::Min(MaxRow + 1, Scroll + Rows), MaxRow + 1), Bx + Bw, By - 24 * S, Grey, 0.62f, 2);
	}
	// first visit: point at the glowing node
	if (G->Tab == EFTTab::Skills && !CS->C.bTipSeen && CS->SPLeft() > 0 && G->TileRects.IsValidIndex(FirstAvail))
	{
		const FBox2D& R = G->TileRects[FirstAvail];
		Txt(TEXT("You've got points. Spend one here.  ▼"), R.Min.X, By - 50 * S, Yellow, 0.75f, 0, true);
		Frame(R.Min.X - 3 * S, R.Min.Y - 3 * S, R.GetSize().X + 6 * S, R.GetSize().Y + 6 * S, FLinearColor(1.f, 0.82f, 0.25f, 0.5f + 0.5f * FMath::Sin(T * 6.f)), 2.f);
	}

	// the detail panel
	const float Dx = W * 0.7f, Dy = By - 30 * S, Dw = W - Dx - 40 * S, Dh = H * 0.86f - Dy;
	Panel(Dx, Dy, Dw, Dh);
	G->BuyRect = FBox2D(ForceInit); G->UndoRect = FBox2D(ForceInit);
	if (G->Tiles.IsValidIndex(Sel))
	{
		const FFTTileView& Tl = G->Tiles[Sel];
		float Y = Dy + 14 * S;
		const TArray<FString> TL = WrapText(Tl.Title, Dw - 28 * S, 1.0f);
		for (const FString& L : TL) { Txt(L, Dx + 14 * S, Y, Tl.Color, 1.0f, 0, true); Y += 30 * S; }
		Txt(Tl.Group, Dx + 14 * S, Y, Grey, 0.62f); Y += 22 * S;
		Txt(Tl.Price, Dx + 14 * S, Y, Yellow, 0.9f, 0, true); Y += 32 * S;
		for (const FString& L : WrapText(Tl.Desc, Dw - 28 * S, 0.66f)) { Txt(L, Dx + 14 * S, Y, White, 0.66f); Y += 19 * S; if (Y > Dy + Dh - 150 * S) break; }
		Y += 10 * S;
		// Needs: with a check or a cross per condition
		if (Tl.State == EFTTile::Locked || Tl.State == EFTTile::Soon)
		{
			TArray<FString> Parts; Tl.Needs.ParseIntoArray(Parts, TEXT(" · "));
			Txt(TEXT("NEEDS"), Dx + 14 * S, Y, Grey, 0.62f); Y += 20 * S;
			for (const FString& Pn : Parts) { Txt(TEXT("✗  ") + Pn, Dx + 14 * S, Y, Red, 0.68f); Y += 20 * S; }
		}
		else if (Tl.State == EFTTile::Available) { Txt(Tl.bCash && Tl.Cost > CS->C.Cash ? FString::Printf(TEXT("✗  %s (YOU HAVE %s)"), *Tl.Price, *Money(CS->C.Cash)) : FString(TEXT("✓  EVERYTHING IT NEEDS")), Dx + 14 * S, Y, Tl.bCash && Tl.Cost > CS->C.Cash ? Red : Green, 0.68f); Y += 22 * S; }
		// BUY (hold for cash) / EQUIP / UNDO
		const float Bw2 = Dw - 28 * S, Bh2 = 52 * S, By2 = Dy + Dh - 2 * Bh2 - 26 * S;
		FString Lbl = TEXT("BUY"), Sub = Tl.bCash ? TEXT("HOLD ENTER / A  ·  OR HOLD HERE") : TEXT("ENTER / A  ·  CLICK");
		const bool bCos = G->Tab == EFTTab::Custom || G->Tab == EFTTab::Sfx;
		if (bCos && Tl.State == EFTTile::Owned) { Lbl = TEXT("EQUIP"); Sub = TEXT("ENTER / A  ·  FREE"); }
		else if (bCos && Tl.State == EFTTile::Equipped) { Lbl = TEXT("EQUIPPED"); Sub = Tl.Slot == TEXT("paint") || Tl.Slot == TEXT("rims") || Tl.Slot == TEXT("neon") || Tl.Slot == TEXT("nitro") ? TEXT("THIS SLOT IS NEVER EMPTY") : TEXT("ENTER / A  TAKES IT OFF"); }
		else if (Tl.Id == TEXT("sequel") && Tl.State == EFTTile::Owned) { Lbl = TEXT("SWITCH"); Sub = TEXT("ENTER / A"); }
		else if (Tl.State == EFTTile::Owned) { Lbl = TEXT("OWNED"); Sub = FString(); }
		else if (Tl.State == EFTTile::Pending) { Lbl = TEXT("PENDING"); Sub = TEXT("KEPT ON READY"); }
		const bool bCanPress = Tl.State == EFTTile::Available || (bCos && Tl.State == EFTTile::Owned) || (Tl.Id == TEXT("sequel") && Tl.State == EFTTile::Owned);
		G->BuyRect = Button(Dx + Dw * 0.5f, By2 + Bh2 * 0.5f, Bw2, Bh2, Lbl, Sub, bCanPress, PC && PC->MouseIn(G->BuyRect));
		if (Tl.bCash && Tl.State == EFTTile::Available) Ring(Dx + Dw - 40 * S, By2 + Bh2 * 0.5f, 16 * S, FMath::Clamp(G->BuyHoldT / FFTProgressData::Get().T(TEXT("HoldToBuy"), 0.5f), 0.f, 1.f), Yellow, 4.f);
		if (G->Tab == EFTTab::Skills) G->UndoRect = Button(Dx + Dw * 0.5f, By2 + Bh2 * 1.5f + 10 * S, Bw2, Bh2 * 0.8f, TEXT("UNDO"), TEXT("BACKSPACE / B"), CS->C.PendingSkills.Num() > 0, PC && PC->MouseIn(G->UndoRect));
	}
	// READY
	const bool bReadyHot = PC && PC->MouseIn(G->ReadyRect);
	G->ReadyRect = Button(W - 40 * S - 210 * S, H * 0.925f, 420 * S, 56 * S, G->bPPFromTitle || G->PPNextLevel < 0 ? TEXT("READY  ▶  TITLE") : FString::Printf(TEXT("READY  ▶  LEVEL %d"), G->PPNextLevel + 1), TEXT("SPACE  ·  PAD START  ·  CLICK"), G->PhaseT > 0.3f, bReadyHot);
	Txt(TEXT("ARROWS / D-PAD MOVE   ·   ENTER / A BUY (CASH: HOLD 0.5 s)   ·   BACKSPACE / B UNDO   ·   SKILL POINTS STAY PENDING UNTIL READY"), 40 * S, H * 0.905f, Grey, 0.66f);
	Txt(TEXT("Nothing runs here: no race, no timers. Cosmetics only change looks; the garage changes numbers; skills change how you play."), 40 * S, H * 0.94f, FLinearColor(1, 1, 1, 0.45f), 0.62f);
	if (T - G->PPMsgT < 2.2f) Txt(G->PPMsg, Bx + Bw * 0.5f, H * 0.865f, G->bPPMsgBad ? Red : Green, 0.85f, 1, true);
}

// ---------------------------------------------------------------------------------------------- cosmetic overlays (07): wreck FX and boss finishers
void AFTHUD::DrawFx(AFTGameMode* G)
{
	const float Now = G->RealClockNow();
	if (Now - G->KaBoomT < 0.9f)
	{
		const float A = FMath::Clamp(0.9f - (Now - G->KaBoomT), 0.f, 1.f);
		const float Sc = 2.6f + (Now - G->KaBoomT) * 1.2f;
		Txt(G->KaBoom, W * 0.5f + 4 * S, H * 0.38f + 4 * S, FLinearColor(0.f, 0.f, 0.f, A), Sc, 1, true, false);
		Txt(G->KaBoom, W * 0.5f, H * 0.38f, FLinearColor(1.f, 0.85f, 0.1f, A), Sc, 1, true, false);
	}
	if (Now - G->FilmBurnT < 0.45f)
	{
		// Film Burn: the frame burns through for a moment
		const float A = 1.f - (Now - G->FilmBurnT) / 0.45f;
		Box(0, 0, W, H, FLinearColor(1.f, 0.55f, 0.15f, 0.45f * A));
		Box(W * 0.62f, 0, W * 0.38f, H, FLinearColor(1.f, 0.95f, 0.8f, 0.35f * A));
	}
	if (!G->Finisher.IsNone() && Now - G->FinisherT < 4.f && Now >= G->FinisherT)
	{
		const float U = Now - G->FinisherT;
		if (G->Finisher == TEXT("s-the-end-card") && U > 0.6f)
		{
			const float A = FMath::Clamp((U - 0.6f) * 1.5f, 0.f, 1.f);
			Txt(TEXT("THE END"), W * 0.5f, H * 0.4f, FLinearColor(1.f, 0.95f, 0.85f, A), 3.6f, 1, true);
			Txt(TEXT("...?"), W * 0.5f + 240 * S, H * 0.4f + 70 * S, FLinearColor(1.f, 0.3f, 0.3f, A * FMath::Clamp(U - 2.f, 0.f, 1.f)), 1.4f, 1, true);
		}
		const UFTGameInstance* GI = G->GI();
		if (G->Finisher == TEXT("s-supernova") && U < 0.6f && !(GI && GI->bReducedMotion)) Box(0, 0, W, H, FLinearColor(1.f, 1.f, 1.f, 0.85f * (1.f - U / 0.6f)));
	}
}

// ---------------------------------------------------------------------------------------------- menus
void AFTHUD::DrawPause(AFTGameMode* G)
{
	Box(0, 0, W, H, FLinearColor(0, 0, 0, 0.6f));
	Txt(TEXT("CAMERAS HOLDING"), W * 0.5f, H * 0.3f, Yellow, 2.2f, 1, true);
	Txt(TEXT("P / ESC  RESUME      C  CONTROLS      O  OPTIONS      R  RESHOOT LEVEL      T  TITLE"), W * 0.5f, H * 0.52f, White, 0.95f, 1);
	Txt(TEXT("M  MUTE      F1  FRAME RATE"), W * 0.5f, H * 0.58f, Grey, 0.85f, 1);
}

void AFTHUD::DrawControls(AFTGameMode* G)
{
	const UFTGameInstance* GI = G->GI();
	if (!GI) return;
	Box(0, 0, W, H, FLinearColor(0.01f, 0.f, 0.04f, 0.88f));
	Txt(TEXT("CONTROLS"), W * 0.5f, H * 0.06f, Pink, 2.f, 1, true);
	Txt(TEXT("The left hand drives, the right hand shoots. Each layout reads only its own keys; steering with the other layout switches to it."), W * 0.5f, H * 0.16f, Grey, 0.8f, 1);
	const float Cx0 = W * 0.5f - 330 * S, Cx1 = W * 0.5f - 20 * S, Cx2 = W * 0.5f + 190 * S, Y0 = H * 0.23f;
	Txt(TEXT("ACTION"), Cx0, Y0, Grey, 0.8f);
	Txt(GI->Layout == 0 ? TEXT("WASD LAYOUT (IN USE)") : TEXT("WASD LAYOUT"), Cx1, Y0, GI->Layout == 0 ? Yellow : Grey, 0.8f);
	Txt(GI->Layout == 1 ? TEXT("ARROWS LAYOUT (IN USE)") : TEXT("ARROWS LAYOUT"), Cx2, Y0, GI->Layout == 1 ? Yellow : Grey, 0.8f);
	for (int32 a = 0; a <= UFTGameInstance::NumActions; ++a)
	{
		const float Y = Y0 + (a + 1) * 38 * S;
		const bool bRow = G->ControlsRow == a;
		if (a == UFTGameInstance::NumActions) { Txt(TEXT("RESET TO DEFAULTS"), Cx0, Y + 8 * S, bRow ? Yellow : White, 0.95f); if (bRow) Frame(Cx0 - 10 * S, Y + 2 * S, 260 * S, 34 * S, Yellow); continue; }
		Txt(UFTGameInstance::ActionName(a), Cx0, Y + 6 * S, bRow ? Yellow : White, 0.95f);
		for (int32 l = 0; l < 2; ++l)
		{
			const float X = l == 0 ? Cx1 : Cx2;
			const bool bSel = bRow && G->ControlsCol == l;
			FString K = GI->Binds[a][l].IsValid() ? GI->Binds[a][l].GetDisplayName().ToString().ToUpper() : TEXT("—");
			if (bSel && G->bControlsWaiting) K = FMath::Fmod(T, 0.6f) < 0.3f ? TEXT("PRESS A KEY") : TEXT("");
			Box(X - 6 * S, Y, 180 * S, 32 * S, bSel ? FLinearColor(0.3f, 0.05f, 0.25f, 0.9f) : Ink);
			if (bSel) Frame(X - 6 * S, Y, 180 * S, 32 * S, Yellow);
			Txt(K, X + 6 * S, Y + 6 * S, White, 0.9f);
		}
	}
	Txt(TEXT("↑↓ PICK  ·  ←→ LAYOUT  ·  ENTER  CHANGE (DELETE CLEARS)  ·  ESC  BACK"), W * 0.5f, H * 0.86f, Grey, 0.85f, 1);
	Txt(TEXT("Always on: mouse (hold left = guns, right click = missile, middle click = AMMO). Gamepad: stick steers, A or RB drift, X guns, B missile, LB or RT turbo, Y ammo in a race, START pause."), W * 0.5f, H * 0.91f, Cyan, 0.75f, 1);
	Txt(TEXT("P, ESC, M, ENTER, TAB, O and F1 are reserved."), W * 0.5f, H * 0.95f, Grey, 0.7f, 1);
}

void AFTHUD::DrawOptions(AFTGameMode* G)
{
	const UFTGameInstance* GI = G->GI();
	if (!GI) return;
	Box(0, 0, W, H, FLinearColor(0.01f, 0.f, 0.04f, 0.85f));
	Txt(TEXT("OPTIONS"), W * 0.5f, H * 0.12f, Cyan, 2.f, 1, true);
	const FString Rows[4] = {
		FString::Printf(TEXT("GRAPHICS PRESET   ◀ %s ▶"), *UFTGameInstance::QualityName(GI->Quality)),
		FString::Printf(TEXT("SHOW FRAME RATE   ◀ %s ▶"), GI->bShowFps ? TEXT("ON") : TEXT("OFF")),
		FString::Printf(TEXT("VSYNC   ◀ %s ▶"), GI->bVSync ? TEXT("ON") : TEXT("OFF")),
		FString::Printf(TEXT("REDUCED MOTION   ◀ %s ▶"), GI->bReducedMotion ? TEXT("ON") : TEXT("OFF")) };
	for (int32 i = 0; i < 4; ++i) Txt(Rows[i], W * 0.5f, H * 0.32f + i * 56 * S, G->OptionsRow == i ? Yellow : White, 1.3f, 1, G->OptionsRow == i);
	Txt(TEXT("LOW and MEDIUM turn Lumen off and render below native resolution with TSR; HIGH uses Lumen GI and reflections; EPIC adds the full Lumen,"), W * 0.5f, H * 0.62f, Grey, 0.75f, 1);
	// 2.9 (the Mac audit): the old line said Nanite and VSM are on at every preset, which is false on a Mac's Metal SM5 path
	Txt(TEXT("shadow and post quality. AUTO measures this computer and picks; on a Mac it picks HIGH with Lumen on, drawn at a lower"), W * 0.5f, H * 0.655f, Grey, 0.75f, 1);
	Txt(TEXT("resolution that TSR upscales to the screen. Nanite and Virtual Shadow Maps run where the graphics card supports them."), W * 0.5f, H * 0.69f, Grey, 0.75f, 1);
	Txt(TEXT("↑↓ PICK  ·  ←→ CHANGE  ·  ESC  BACK"), W * 0.5f, H * 0.8f, Grey, 0.85f, 1);
}
