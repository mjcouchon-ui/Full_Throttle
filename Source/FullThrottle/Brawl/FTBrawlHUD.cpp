#include "FTBrawlHUD.h"
#include "FTBrawlGameMode.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#if __has_include("FTGameInstance.h")   // the main game: M mutes in the nights too (loop cycle 6)
#define FT_BRAWL_MUTE_HINT " · M MUTE"
#else
#define FT_BRAWL_MUTE_HINT ""
#endif

namespace { FLinearColor Hx(const TCHAR* H, float A = 1.f) { FLinearColor C(FColor::FromHex(H)); C.A = A; return C; } }

void AFTBrawlHUD::Txt(const FString& Str, float X, float Y, const FLinearColor& C, float Scale, int32 Align)
{
	UFont* F = GEngine->GetLargeFont();
	float W = 0, H = 0;
	Canvas->StrLen(F, Str, W, H);
	const float Sc = Scale * S;
	float Px = X * S;
	if (Align == 1) Px -= W * Sc / 2; else if (Align == 2) Px -= W * Sc;
	FCanvasTextItem Sh(FVector2D(Px + 2 * S, Y * S + 2 * S), FText::FromString(Str), F, FLinearColor(0.02f, 0.01f, 0.05f, C.A * 0.8f));
	Sh.Scale = FVector2D(Sc, Sc); Canvas->DrawItem(Sh);
	FCanvasTextItem T(FVector2D(Px, Y * S), FText::FromString(Str), F, C);
	T.Scale = FVector2D(Sc, Sc); Canvas->DrawItem(T);
}

void AFTBrawlHUD::Rect(float X, float Y, float W, float H, const FLinearColor& C)
{
	FCanvasTileItem T(FVector2D(X * S, Y * S), FVector2D(W * S, H * S), C);
	T.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(T);
}

void AFTBrawlHUD::Centre(const FString& A, const FString& B, const FLinearColor& C)
{
	const float W = Canvas->ClipX / S, H = Canvas->ClipY / S;
	Rect(0, H / 2 - 70, W, 140, FLinearColor(0.02f, 0.02f, 0.05f, 0.55f));
	Txt(A, W / 2, H / 2 - 40, C, 2.6f, 1);
	if (!B.IsEmpty()) Txt(B, W / 2, H / 2 + 22, FLinearColor::White, 1.0f, 1);
}

void AFTBrawlHUD::DrawHUD()
{
	Super::DrawHUD();
	AFTBrawlGameMode* G = GetWorld() ? GetWorld()->GetAuthGameMode<AFTBrawlGameMode>() : nullptr;
	if (!G || !Canvas) return;
	S = Canvas->ClipY / 600.f;   // brawl.js's 600-high canvas
	if (G->InCut()) { DrawCut(G); return; }
	const float W = Canvas->ClipX / S, H = 600.f;
	const FFTBrawlBody& P = G->P;
	// the "!" tells and the hurt bars over heads, the hit words where they landed
	for (const FFTBrawlBody& F : G->Foes)
	{
		if (F.St == TEXT("dead")) continue;
		const FVector Top = G->World(F.X, F.Y, F.Z + 140 * F.S / 2);
		const FVector Sp = Canvas->Project(Top);
		if (Sp.Z <= 0) continue;
		if (F.St == TEXT("wind") || F.St == TEXT("aim") || F.St == TEXT("slamwind") || F.St == TEXT("whirltell") || (F.St == TEXT("dash") && F.T < 0.18f) || (F.St == TEXT("throw") && F.T < 0.3f)) Txt(TEXT("!"), Sp.X / S, Sp.Y / S - 40, Hx(TEXT("ff4f6e")), 2.4f, 1);
		if (!F.bBoss && F.Hp < F.Max && F.Hp > 0)
		{
			Rect(Sp.X / S - 25, Sp.Y / S - 6, 50, 6, FLinearColor(0, 0, 0, 0.6f));
			Rect(Sp.X / S - 25, Sp.Y / S - 6, 50 * F.Hp / F.Max, 6, Hx(TEXT("ff4f6e")));
		}
	}
	for (const FFTBrawlPop& E : G->Pops)
	{
		const FVector Sp = Canvas->Project(E.P + FVector(0, 0, E.T * 40 * 1.4f));
		if (Sp.Z <= 0) continue;
		FLinearColor C = E.Col; C.A = FMath::Max(0.f, 1 - E.T);
		Txt(E.Text, Sp.X / S, Sp.Y / S, C, E.bBig ? 2.0f : 1.3f, 1);
	}
	// loop cycle 2: dark plates behind the two top corners, so K.O. / the held weapon / the timers / the venue read over the bright
	// neon walls (the jukebox sits right behind the K.O. count)
	{
		const int32 Rows = (G->Held >= 0 ? 1 : 0) + (G->PowerT > 0 ? 1 : 0) + (G->ToughT > 0 ? 1 : 0) + (G->BuzzT > 0 ? 1 : 0);
		Rect(70, 22, 282, 56 + Rows * 18, FLinearColor(0.02f, 0.01f, 0.05f, 0.55f));
		Rect(W - 300, 20, 288, 58, FLinearColor(0.02f, 0.01f, 0.05f, 0.55f));
	}
	// Nova's bar
	Rect(80, 30, 260, 18, FLinearColor(0, 0, 0, 0.6f));
	Rect(80, 30, 260 * FMath::Max(0.f, P.Hp) / FMath::Max(1.f, P.Max), 18, P.Hp > 40 ? Hx(TEXT("9fe870")) : Hx(TEXT("ff4f6e")));
	Txt(TEXT("NOVA"), 82, 52, FLinearColor::White, 0.9f);
	Txt(FString::Printf(TEXT("K.O. %d"), G->Kos), 340, 52, Hx(TEXT("ffd23f")), 0.9f, 2);
	Txt(G->VenueName, W - 24, 26, G->Neon, 1.2f, 2);
	const int32 At = FMath::Min(G->Wave + 1, G->NumWaves);
	Txt(G->bBossOut ? FString(TEXT("THE BOSS")) : FString::Printf(TEXT("WAVE %d / %d"), At, G->NumWaves), W - 24, 52, FLinearColor::White, 0.9f, 2);
	// what he holds and the power-ups running (run 21219)
	{
		float Ry = 78;
		if (G->Held >= 0) { Txt(FString::Printf(TEXT("%s x%d"), *AFTBrawlGameMode::ItemName(G->Held), FMath::Max(0, G->HeldUses)), 340, Ry, Hx(TEXT("f4e9d8")), 0.8f, 2); Ry += 18; }
		auto Timer = [&](const TCHAR* N, float T, const TCHAR* C) { if (T > 0) { Txt(FString::Printf(TEXT("%s %d"), N, FMath::CeilToInt(T)), 340, Ry, Hx(C, T < 3 && int32(G->StT * 6) % 2 ? 0.4f : 1.f), 0.8f, 2); Ry += 18; } };
		Timer(TEXT("POWER"), G->PowerT, TEXT("ffd23f"));
		Timer(TEXT("TOUGH"), G->ToughT, TEXT("36f1ff"));
		Timer(TEXT("BUZZED"), G->BuzzT, TEXT("ff8a3d"));
		const int32 Ii = G->ItemAt();
		if (Ii != INDEX_NONE && (P.St == TEXT("idle") || P.St == TEXT("walk")))
		{
			const FVector Sp = Canvas->Project(G->World(P.X, P.Y, 250));
			if (Sp.Z > 0) Txt(G->Items[Ii].bBar ? FString(TEXT("J: DRINK")) : FString::Printf(TEXT("J: GRAB %s"), *AFTBrawlGameMode::ItemName(G->Items[Ii].Kind)), Sp.X / S, Sp.Y / S, Hx(TEXT("ffd23f")), 0.9f, 1);
		}
	}
	if (G->Chain >= 2) Txt(FString::Printf(TEXT("%d HITS"), G->Chain), 82, 80, Hx(TEXT("ff4fb8")), 1.7f);
	const int32 Bi = G->BossIdx();
	if (Bi != INDEX_NONE && G->Foes[Bi].St != TEXT("dead"))
	{
		const FFTBrawlBody& B = G->Foes[Bi];
		Rect(W / 2 - 260, H - 70, 520, 16, FLinearColor(0, 0, 0, 0.7f));
		Rect(W / 2 - 260, H - 70, 520 * FMath::Max(0.f, B.Hp) / B.Max, 16, Hx(TEXT("ff4f6e")));
		Txt(G->BossName, W / 2, H - 94, FLinearColor::White, 1.0f, 1);
	}
	if (P.St == TEXT("held") && int32(G->StT * 6) % 2) Txt(TEXT("MASH J / K TO BREAK FREE"), W / 2, 120, Hx(TEXT("ff4f6e")), 1.4f, 1);
	if (G->Go > 0 && int32(G->StT * 4) % 2) Txt(TEXT("GO →"), W - 80, H / 2 - 30, Hx(TEXT("ffd23f")), 3.0f, 2);
	if ((P.St == TEXT("lift") || P.St == TEXT("carry")) && int32(G->StT * 6) % 2) Txt(TEXT("L / J / K: THROW HIM (STEER TO AIM)"), W / 2, 120, Hx(TEXT("ffd23f")), 1.4f, 1);   // grab and throw (run 42137)
	Txt(TEXT("WASD MOVE · J PUNCH / PICK UP / SWING · K KICK / THROW WEAPON · L GRAB & THROW · SPACE JUMP · J AT THE BAR: A DRINK" FT_BRAWL_MUTE_HINT " · ESC QUIT"), W / 2, H - 26, FLinearColor(1, 1, 1, 0.75f), 0.8f, 1);
	if (G->BannerT >= 0)
	{
		const float Bt = G->BannerT, A = FMath::Clamp(FMath::Min3(1.f, Bt * 4, (2.4f - Bt) * 3), 0.f, 1.f);
		FLinearColor C = G->Neon; C.A = A;
		Centre(G->Banner, G->BannerSub, C);
	}
	if (G->BossIntro > 0) Centre(G->BossName, G->BossTitle, Hx(TEXT("ff4f6e")));
	// the boss's last call, from the story (after the K.O.)
	if (G->bWon && G->IsVelvet() && G->WinT > 1.2f) Txt(TEXT("VINNIE: \"MY FLOOR... MY BEAUTIFUL FLOOR...\""), W / 2, H / 2 + 90, Hx(TEXT("ff4fb8")), 1.1f, 1);
	if (G->bWon && G->IsLucky() && G->WinT > 1.2f) Txt(TEXT("THE PIT BOSS FOLDED.   NOVA: \"THE HOUSE ALWAYS WINS? NOT TONIGHT.\""), W / 2, H / 2 + 90, Hx(TEXT("ffd23f")), 1.1f, 1);   // the story's last call
	if (G->Foes.Num() && G->BossIdx() != INDEX_NONE && G->Foes[G->BossIdx()].St == TEXT("dizzy") && int32(G->StT * 6) % 2) Txt(TEXT("DIZZY! HIT HIM!"), W / 2, 150, Hx(TEXT("ffd23f")), 1.6f, 1);
	if (P.Hp <= 0 && G->KoT > 1) Centre(TEXT("KNOCKED OUT"), TEXT("ENTER/A: GET BACK UP (THIS WAVE AGAIN)"), Hx(TEXT("ff4f6e")));
	if (G->bWon) Centre(TEXT("K.O.!"), FString::Printf(TEXT("%s IS DOWN · %d KNOCKED OUT · BEST COMBO %d%s"), *G->BossName, G->Kos, G->Best, G->WinT > 2.6f ? *(G->NextLabel().IsEmpty() ? FString(TEXT(" · ENTER/A: AGAIN")) : TEXT(" · ENTER/A: ON TO ") + G->NextLabel()) : TEXT("")), Hx(TEXT("ffd23f")));
}

TArray<FString> AFTBrawlHUD::Wrap(const FString& Str, float MaxW, float Scale)
{
	UFont* F = GEngine->GetLargeFont();
	TArray<FString> Words, Lines;
	Str.ParseIntoArray(Words, TEXT(" "));
	FString Cur;
	for (const FString& Wd : Words)
	{
		const FString Try = Cur.IsEmpty() ? Wd : Cur + TEXT(" ") + Wd;
		float Tw = 0, Th = 0; Canvas->StrLen(F, Try, Tw, Th);
		if (Tw * Scale > MaxW && !Cur.IsEmpty()) { Lines.Add(Cur); Cur = Wd; } else Cur = Try;
	}
	if (!Cur.IsEmpty()) Lines.Add(Cur);
	return Lines;
}

// night 1's cutscenes (run 49531): cinema bars, the speaker's name in his colour over a dark box, the line typed out,
// title cards ("NIGHT 1 · THE RUSTY ANCHOR", "2:40 AM"), the photographer's flash, fades, and the END OF NIGHT 1 card
void AFTBrawlHUD::DrawCut(AFTBrawlGameMode* G)
{
	const float W = Canvas->ClipX / S, H = 600.f;
	const float Bar = 62.f * G->CutBars;
	if (G->CutFlash > 0) Rect(0, 0, W, H, FLinearColor(1, 1, 1, FMath::Min(1.f, G->CutFlash * 0.9f)));
	if (G->CutFade > 0) Rect(0, 0, W, H, FLinearColor(0, 0, 0, FMath::Clamp(G->CutFade, 0.f, 1.f)));
	Rect(0, 0, W, Bar, FLinearColor(0, 0, 0, 1)); Rect(0, H - Bar, W, Bar, FLinearColor(0, 0, 0, 1));
	if (!G->CutCard.IsEmpty())
	{
		Txt(G->CutCard, W / 2, H / 2 - 70, FLinearColor(1.f, 0.31f, 0.72f), 2.6f, 1);
		Txt(G->CutCardSub, W / 2, H / 2 + 6, FLinearColor(0.96f, 0.91f, 0.85f), 1.3f, 1);
		if (int32(G->StT * 2) % 2 == 0) Txt(G->CutCardKeys, W / 2, H / 2 + 64, FLinearColor(1.f, 0.82f, 0.25f), 1.2f, 1);
		return;
	}
	if (G->CutTitleT >= 0)
	{
		const float T = G->CutTitleT, A = FMath::Clamp(FMath::Min3(1.f, T * 2.5f, (3.2f - T) * 2.f), 0.f, 1.f);
		Txt(G->CutTitle, W / 2, 110, FLinearColor(1.f, 0.31f, 0.72f, A), 2.6f, 1);
		Txt(G->CutSub, W / 2, 178, FLinearColor(0.96f, 0.91f, 0.85f, A), 1.25f, 1);
	}
	if (!G->CutLine.IsEmpty())
	{
		const int32 N = FMath::Min(G->CutLine.Len(), FMath::FloorToInt(G->CutLineT * 45.f));
		const bool bVo = G->CutWho.IsEmpty();
		const float Sc = 1.45f, Lh = 38.f, Bw = FMath::Min(W - 100.f, 900.f), Bx = (W - Bw) / 2;
		const TArray<FString> Lines = Wrap(G->CutLine, Bw - 52.f, Sc);
		const float Bh = 30.f + Lines.Num() * Lh + (bVo ? 0.f : 30.f), By = H - Bar - Bh - 12.f;
		Rect(Bx, By, Bw, Bh, FLinearColor(0.03f, 0.02f, 0.07f, 0.82f));
		Rect(Bx, By, 5, Bh, G->CutWhoCol);
		float Y = By + 14;
		if (!bVo) { Txt(G->CutWho, Bx + 24, Y, G->CutWhoCol, 1.2f); Y += 30; }
		int32 Left = N;
		for (const FString& L : Lines)
		{
			if (Left <= 0) break;
			const FString Part = L.Left(Left);
			Txt(Part, Bx + 24, Y, bVo ? FLinearColor(0.96f, 0.91f, 0.85f, 0.95f) : FLinearColor::White, Sc);
			Left -= L.Len() + 1; Y += Lh;
		}
	}
	Txt(TEXT("ENTER/A: NEXT  ·  TAB/BACK: SKIP"), W - 20, H - Bar + 18, FLinearColor(1, 1, 1, 0.5f), 0.95f, 2);
}
