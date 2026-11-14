#include "FTHUD.h"
#include "FTGameMode.h"
#include "FTGameInstance.h"
#include "FTPlayerCar.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Engine/Engine.h"
#include "CanvasItem.h"

namespace
{
	const FLinearColor Ink(0.02f, 0.01f, 0.05f, 0.78f), Yellow(1.f, 0.82f, 0.25f), Pink(1.f, 0.24f, 0.65f), Cyan(0.21f, 0.95f, 1.f), Red(1.f, 0.2f, 0.25f), Green(0.35f, 1.f, 0.45f), White(1.f, 0.97f, 0.92f), Grey(0.65f, 0.62f, 0.75f), Amber(1.f, 0.62f, 0.18f);
	FString Num(int64 V)
	{
		FString R = FString::Printf(TEXT("%lld"), FMath::Abs(V));
		for (int32 i = R.Len() - 3; i > 0; i -= 3) R.InsertAt(i, TEXT(","));
		return (V < 0 ? TEXT("-") : TEXT("")) + R;
	}
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

	switch (G->Phase)
	{
	case EFTPhase::Title: DrawTitle(G); break;
	case EFTPhase::Card: DrawCard(G); break;
	case EFTPhase::Play: DrawPlay(G); break;
	case EFTPhase::Wreck: case EFTPhase::Wrap: DrawPlay(G); DrawCut(G); break;
	case EFTPhase::Reel: DrawReel(G); break;
	case EFTPhase::Rating: DrawRating(G); break;
	}
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
void AFTHUD::DrawTitle(AFTGameMode* G)
{
	Box(0, 0, W, H, FLinearColor(0.02f, 0.f, 0.06f, 0.35f));
	const float Pulse = 0.85f + 0.15f * FMath::Sin(T * 3.f);
	Txt(TEXT("FULL"), W * 0.5f, H * 0.1f, FLinearColor(Cyan.R, Cyan.G, Cyan.B, 1.f) * Pulse, 3.4f, 1, true);
	Txt(TEXT("THROTTLE"), W * 0.5f, H * 0.1f + 74 * S, Pink * Pulse, 3.4f, 1, true);
	Txt(TEXT("AN 80s SPY CAR CHASE  ·  STARRING YOU AS AGENT NOVA KANE"), W * 0.5f, H * 0.36f, Yellow, 1.f, 1);
	const UFTGameInstance* GI = G->GI();
	const float Cw = 300 * S, Gap = 24 * S, X0 = W * 0.5f - (Cw * 3 + Gap * 2) * 0.5f, Y0 = H * 0.45f;
	for (int32 i = 0; i < 3; ++i)
	{
		const FFTLevelDef& L = FTData::Level(i);
		const float X = X0 + i * (Cw + Gap);
		const bool bSel = G->TitleSel == i;
		Box(X, Y0, Cw, 150 * S, bSel ? FLinearColor(0.25f, 0.05f, 0.25f, 0.85f) : Ink);
		Frame(X, Y0, Cw, 150 * S, bSel ? Yellow : FLinearColor(1, 1, 1, 0.25f), bSel ? 3.f : 1.5f);
		Txt(FString::Printf(TEXT("LEVEL %d"), i + 1), X + 14 * S, Y0 + 10 * S, bSel ? Yellow : Grey, 0.9f);
		Txt(L.Zone, X + 14 * S, Y0 + 34 * S, White, 1.15f, 0, true);
		Txt(L.Title, X + 14 * S, Y0 + 72 * S, Pink, 0.95f);
		if (GI && GI->BestScore[i] > 0) Txt(FString::Printf(TEXT("BEST TAKE  %s  ·  %s"), *Num(GI->BestScore[i]), *GI->BestTitle[i]), X + 14 * S, Y0 + 116 * S, Cyan, 0.75f);
	}
	if (FMath::Fmod(T, 1.f) < 0.65f) Txt(TEXT("PRESS ENTER (A) TO ROLL CAMERA"), W * 0.5f, H * 0.71f, White, 1.3f, 1, true);
	Txt(TEXT("◀ ▶  PICK THE LEVEL   ·   C  CONTROLS   ·   O  OPTIONS   ·   M  MUTE   ·   ESC  QUIT"), W * 0.5f, H * 0.79f, Grey, 0.85f, 1);
	if (GI)
	{
		const FString Lay = GI->Layout == 0 ? TEXT("WASD steer  ·  hold SHIFT + steer to drift  ·  SPACE turbo  ·  J guns  ·  K missile")
			: TEXT("ARROWS steer  ·  hold SHIFT + steer to drift  ·  SPACE turbo  ·  Z guns  ·  X missile");
		Txt(Lay, W * 0.5f, H * 0.84f, Cyan, 0.85f, 1);
		Txt(TEXT("Mouse: hold left = guns, right click = missile   ·   Gamepad: stick, A/RB drift, X guns, B missile, LB/RT turbo"), W * 0.5f, H * 0.88f, Grey, 0.75f, 1);
	}
	Txt(TEXT("UNREAL ENGINE 5.9  ·  CHAOS VEHICLES  ·  LUMEN  ·  NANITE  ·  NIAGARA"), W * 0.5f, H * 0.95f, FLinearColor(1, 1, 1, 0.45f), 0.7f, 1);
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
	}
	// director's megaphone bar
	if (!G->Line.IsEmpty() && Now - G->LineT < G->LineHold + 1.2f)
	{
		const int32 Chars = FMath::Min(G->Line.Len(), FMath::FloorToInt((Now - G->LineT) * 70.f));
		const FLinearColor Acc = G->LinePri >= 3 ? Yellow : (G->LinePri >= 2 ? Pink : Cyan);
		const float By = (G->bBossOn ? 104 : 72) * S;
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
	const float Lx = 30 * S, Ly = H - 290 * S, Lw = 250 * S;
	Panel(Lx, Ly, Lw, 262 * S);
	Txt(TEXT("◉ KESTREL-88"), Lx + 12 * S, Ly + 8 * S, Pink, 0.95f, 0, true);
	Txt(TEXT("AGENT N. KANE"), Lx + 12 * S, Ly + 34 * S, Grey, 0.7f);
	const float Hp = G->Hp;
	Txt(FString::Printf(TEXT("ARMOR %d%%"), FMath::RoundToInt(Hp)), Lx + 12 * S, Ly + 54 * S, White, 0.8f);
	const FLinearColor HC = Hp > 60 ? Green : (Hp > 30 ? Amber : Red);
	const bool bBlink = Hp <= 25 && FMath::Fmod(Now, 0.5f) < 0.25f;
	for (int32 i = 0; i < 10; ++i) Box(Lx + 12 * S + i * 22 * S, Ly + 74 * S, 18 * S, 12 * S, (i * 10 < Hp && !bBlink) ? HC : FLinearColor(1, 1, 1, 0.12f));
	const TCHAR* DmgTxt = Hp > 79 ? TEXT("ALL SYSTEMS NOMINAL") : (Hp > 59 ? TEXT("BODYWORK DAMAGED") : (Hp > 39 ? TEXT("WINDSHIELD CRACKED") : (Hp > 24 ? TEXT("ENGINE SMOKING") : TEXT("CRITICAL — ON FIRE"))));
	Txt(DmgTxt, Lx + 12 * S, Ly + 90 * S, HC, 0.65f);
	const UFTGameInstance* GI = G->GI();
	auto KeyName = [GI](EFTAction A) { return GI && GI->Binds[(int32)A][GI->Layout].IsValid() ? GI->Binds[(int32)A][GI->Layout].GetDisplayName().ToString().ToUpper() : FString(TEXT("-")); };
	Txt(FString::Printf(TEXT("MISSILES [%s]"), *KeyName(EFTAction::Missile)), Lx + 12 * S, Ly + 110 * S, White, 0.75f);
	for (int32 i = 0; i < 9; ++i) Box(Lx + 12 * S + i * 18 * S, Ly + 130 * S, 12 * S, 8 * S, i < G->Missiles ? Amber : FLinearColor(1, 1, 1, 0.12f));
	const TCHAR* TurboSt = G->bTurboOn ? TEXT("BURN") : (G->BoostT > 0 ? TEXT("BOOST") : (G->bTurboLock ? TEXT("RECHARGING") : (G->Turbo >= 99.f ? TEXT("READY") : TEXT("CHARGING"))));
	Txt(FString::Printf(TEXT("TURBO [%s]  %s"), *KeyName(EFTAction::Turbo), TurboSt), Lx + 12 * S, Ly + 144 * S, White, 0.75f);
	Box(Lx + 12 * S, Ly + 164 * S, 220 * S, 9 * S, FLinearColor(1, 1, 1, 0.12f));
	Box(Lx + 12 * S, Ly + 164 * S, 220 * S * G->Turbo / 100.f, 9 * S, G->bTurboOn ? Pink : Cyan);
	static const FLinearColor DC[3] = { FLinearColor(0.37f, 0.82f, 1.f), FLinearColor(1.f, 0.65f, 0.24f), FLinearColor(1.f, 0.31f, 0.72f) };
	const TCHAR* DriftSt = G->BoostT > 0 ? TEXT("BOOST!") : (!G->bDrifting ? TEXT("HOLD + STEER") : (G->DriftLevel == 0 ? TEXT("SLIDING") : (G->DriftLevel == 1 ? TEXT("BLUE") : (G->DriftLevel == 2 ? TEXT("ORANGE") : TEXT("PINK!")))));
	Txt(FString::Printf(TEXT("DRIFT [%s]  %s"), *KeyName(EFTAction::Drift), DriftSt), Lx + 12 * S, Ly + 178 * S, White, 0.75f);
	for (int32 i = 0; i < 3; ++i) Box(Lx + 12 * S + i * 26 * S, Ly + 198 * S, 20 * S, 9 * S, i < G->DriftLevel ? DC[i] : FLinearColor(1, 1, 1, 0.12f));
	Txt(FString::Printf(TEXT("%d"), FMath::RoundToInt(G->SpeedMph)), Lx + 12 * S, Ly + 210 * S, White, 1.6f, 0, true);
	Txt(TEXT("MPH"), Lx + 92 * S, Ly + 232 * S, Grey, 0.7f);
	{
		// tacho arc
		const FVector2D C(Lx + 190 * S, Ly + 236 * S);
		const float R = 34 * S, Frac = FMath::Clamp(G->SpeedMph / (760.f * 0.31f), 0.f, 1.f);
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
		Txt(GI->Layout == 0 ? TEXT("WASD steer · SHIFT drift · SPACE turbo · J guns · K missile · P pause")
			: TEXT("ARROWS steer · SHIFT drift · SPACE turbo · Z guns · X missile · P pause"), W * 0.5f, H - 40 * S, FLinearColor(1, 1, 1, 0.8f), 0.8f, 1);
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
	for (int32 i = 0; i < 3; ++i)
	{
		const bool bLeft = i < 3 - G->Retakes;
		const FVector2D C(Rx + 110 * S + i * 30 * S, Ry + 108 * S);
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
		return;
	}
	Txt(TEXT("● DAILIES · HIGHLIGHTS"), 24 * S, 10 * S, Red, 0.85f, 0, true);
	Txt(FString::Printf(TEXT("SHOT %d / %d   %s"), G->ReelShot, G->ReelShots, *G->ReelCamName), W - 24 * S, 12 * S, White, 0.8f, 2);
	Txt(G->ReelLabel, 40 * S, H - 80 * S, Yellow, 1.3f, 0, true);
	if (G->bReelSlow) Txt(TEXT("▶▶ SLOW MOTION"), W - 40 * S, H - 74 * S, Pink, 0.9f, 2);
	Txt(TEXT("ENTER TO SKIP"), W * 0.5f, H - 30 * S, Grey, 0.6f, 1);
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
	const bool bLast = G->Level >= 2 || G->bOutOfFilm;
	Txt(bLast ? TEXT("ENTER  TITLE   ·   R  RESHOOT   ·   ESC  TITLE") : TEXT("ENTER  NEXT LEVEL   ·   R  RESHOOT   ·   ESC  TITLE"), W * 0.5f, H * 0.93f, Grey, 0.85f, 1);
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
	Txt(TEXT("Always on: mouse (hold left = guns, right click = missile). Gamepad: stick steers, A or RB drift, X guns, B missile, LB or RT turbo, START pause, Y controls."), W * 0.5f, H * 0.91f, Cyan, 0.75f, 1);
	Txt(TEXT("P, ESC, M, ENTER, TAB, O and F1 are reserved."), W * 0.5f, H * 0.95f, Grey, 0.7f, 1);
}

void AFTHUD::DrawOptions(AFTGameMode* G)
{
	const UFTGameInstance* GI = G->GI();
	if (!GI) return;
	Box(0, 0, W, H, FLinearColor(0.01f, 0.f, 0.04f, 0.85f));
	Txt(TEXT("OPTIONS"), W * 0.5f, H * 0.12f, Cyan, 2.f, 1, true);
	const FString Rows[3] = {
		FString::Printf(TEXT("GRAPHICS PRESET   ◀ %s ▶"), *UFTGameInstance::QualityName(GI->Quality)),
		FString::Printf(TEXT("SHOW FRAME RATE   ◀ %s ▶"), GI->bShowFps ? TEXT("ON") : TEXT("OFF")),
		FString::Printf(TEXT("VSYNC   ◀ %s ▶"), GI->bVSync ? TEXT("ON") : TEXT("OFF")) };
	for (int32 i = 0; i < 3; ++i) Txt(Rows[i], W * 0.5f, H * 0.32f + i * 56 * S, G->OptionsRow == i ? Yellow : White, 1.3f, 1, G->OptionsRow == i);
	Txt(TEXT("LOW and MEDIUM turn Lumen off and render below native resolution with TSR; HIGH uses Lumen GI and reflections; EPIC adds the full Lumen,"), W * 0.5f, H * 0.62f, Grey, 0.75f, 1);
	Txt(TEXT("shadow and post quality. AUTO measures this PC and picks. Nanite and Virtual Shadow Maps are on at every preset."), W * 0.5f, H * 0.655f, Grey, 0.75f, 1);
	Txt(TEXT("↑↓ PICK  ·  ←→ CHANGE  ·  ESC  BACK"), W * 0.5f, H * 0.8f, Grey, 0.85f, 1);
}
