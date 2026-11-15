#include "FTBrawlHUD.h"
#include "FTBrawlGameMode.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "CanvasItem.h"
#if __has_include("FTGameInstance.h")   // the main game: M mutes in the nights too (loop cycle 6), pad prompts (loop cycle 8)
#define FT_BRAWL_MAIN_HUD 1
#define FT_BRAWL_MUTE_HINT "   /   M  MUTE"
#define FT_BRAWL_ENTER "ENTER / A"
#define FT_BRAWL_SKIP "TAB / BACK"
#else
#define FT_BRAWL_MUTE_HINT ""
#define FT_BRAWL_ENTER "ENTER"
#define FT_BRAWL_SKIP "TAB"
#endif

namespace
{
	FLinearColor Al(FLinearColor C, float A) { C.A *= A; return C; }
	// the races' palette, value for value (full-throttle-unreal FTHUDStyle.h; the standalone brawl project has no copy of it)
	const FLinearColor Ink(0.009f, 0.017f, 0.039f, 0.94f);
	const FLinearColor Deep(0.004f, 0.009f, 0.023f, 0.97f);
	const FLinearColor Rule(0.27f, 0.39f, 0.54f, 0.62f);
	const FLinearColor Track(0.095f, 0.135f, 0.20f, 0.95f);
	const FLinearColor White(1.f, 0.954f, 0.848f);
	const FLinearColor Grey(0.60f, 0.71f, 0.82f);
	const FLinearColor Cyan(0.19f, 0.92f, 1.f);
	const FLinearColor Pink(1.f, 0.27f, 0.62f);
	const FLinearColor Yellow(1.f, 0.83f, 0.42f);
	const FLinearColor Amber(1.f, 0.57f, 0.18f);
	const FLinearColor Red(1.f, 0.23f, 0.27f);
	const FLinearColor Green(0.38f, 1.f, 0.55f);
	constexpr float Safe = 32.f, Corner = 12.f, Label = 15.f, Body = 18.f, Strong = 21.f, Heading = 33.f, Readout = 46.f;
}

// the races' fonts have no arrow, and Orbitron no middle dot or accents: swap them for what they do draw
static FString Glyphs(const FString& In, bool bDisplay)
{
	FString O = In.Replace(TEXT("→"), TEXT(">"));
	if (bDisplay)
	{
		O.ReplaceInline(TEXT("·"), TEXT("/")); O.ReplaceInline(TEXT("É"), TEXT("E")); O.ReplaceInline(TEXT("È"), TEXT("E"));
		O.ReplaceInline(TEXT("’"), TEXT("'")); O.ReplaceInline(TEXT("…"), TEXT("..."));
	}
	return O;
}

UFont* AFTBrawlHUD::Face(bool bDisplay) const
{
	UFont* F = bDisplay ? FOrb.Get() : FRaj.Get();
	return F ? F : GEngine->GetLargeFont();
}

float AFTBrawlHUD::TextW(const FString& Str, float Height, bool bDisplay)
{
	float W = 0, H = 0;
	Canvas->StrLen(Face(bDisplay), Glyphs(Str, bDisplay), W, H);
	return W * Height / (bDisplay ? OrbH : RajH);
}

void AFTBrawlHUD::Txt(const FString& In, float X, float Y, const FLinearColor& C, float Height, int32 Align, bool bDisplay, float MaxW, bool bShadow)
{
	if (In.IsEmpty() || C.A <= 0.f) return;
	const FString Str = Glyphs(In, bDisplay);
	UFont* F = Face(bDisplay);
	float Sc = Height / (bDisplay ? OrbH : RajH);
	float W = TextW(Str, Height, bDisplay);
	if (MaxW > 0.f && W > MaxW) { Sc *= MaxW / W; W = MaxW; }
	if (Align == 1) X -= W / 2; else if (Align == 2) X -= W;
	if (bShadow)
	{
		FCanvasTextItem Sh(FVector2D((X + 2) * S, (Y + 2) * S), FText::FromString(Str), F, FLinearColor(Deep.R, Deep.G, Deep.B, C.A * 0.85f));
		Sh.Scale = FVector2D(Sc * S, Sc * S); Canvas->DrawItem(Sh);
	}
	FCanvasTextItem T(FVector2D(X * S, Y * S), FText::FromString(Str), F, C);
	T.Scale = FVector2D(Sc * S, Sc * S); Canvas->DrawItem(T);
}

void AFTBrawlHUD::Rect(float X, float Y, float W, float H, const FLinearColor& C)
{
	FCanvasTileItem T(FVector2D(X * S, Y * S), FVector2D(W * S, H * S), C);
	T.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(T);
}

// the races' panel (FTHUD::Plate): ink fill with the bottom-right corner clipped, a hairline rule round it, the accent tick top-left
void AFTBrawlHUD::Plate(float X, float Y, float W, float H, const FLinearColor& Accent, float A)
{
	const float Cl = FMath::Min(Corner, H * 0.3f), Px = 1.f / S;   // Px: one screen pixel
	const FLinearColor Fill = Al(Ink, A), Ln = Al(Rule, A);
	Rect(X, Y, W, H - Cl, Fill);
	Rect(X, Y + H - Cl, W - Cl, Cl, Fill);
	FCanvasTriangleItem Cut(FVector2D((X + W - Cl) * S, (Y + H - Cl) * S), FVector2D((X + W) * S, (Y + H - Cl) * S), FVector2D((X + W - Cl) * S, (Y + H) * S), FCanvasTileItem(FVector2D::ZeroVector, FVector2D(1, 1), Fill).Texture);   // the engine's white texture (GWhiteTexture is RenderCore's: the standalone brawl module doesn't link it)
	Cut.SetColor(Fill); Cut.BlendMode = SE_BLEND_Translucent; Canvas->DrawItem(Cut);
	Rect(X, Y, W, Px, Ln); Rect(X, Y + H - Px, W - Cl, Px, Ln); Rect(X, Y, Px, H, Ln); Rect(X + W - Px, Y, Px, H - Cl, Ln);
	FCanvasLineItem Diag(FVector2D((X + W - Cl) * S, (Y + H) * S), FVector2D((X + W) * S, (Y + H - Cl) * S));
	Diag.SetColor(Ln); Diag.LineThickness = 1.f; Canvas->DrawItem(Diag);
	Rect(X, Y, FMath::Min(W * 0.34f, 72.f), 2.f, Al(Accent, A));
}

// the races' segmented meter (FTHUD::Meter)
void AFTBrawlHUD::Meter(float X, float Y, float W, float H, float Frac, const FLinearColor& C, int32 Segments, float A)
{
	Segments = FMath::Max(1, Segments);
	const float Gap = Segments > 1 ? 3.f : 0.f, Pitch = (W + Gap) / Segments, Sw = Pitch - Gap;
	const float Filled = FMath::Clamp(Frac, 0.f, 1.f) * Segments;
	for (int32 i = 0; i < Segments; i++)
	{
		const float Sx = X + i * Pitch, V = FMath::Clamp(Filled - i, 0.f, 1.f);
		Rect(Sx, Y, Sw, H, Al(Track, A));
		if (V > 0) Rect(Sx, Y, Sw * V, H, Al(C, A));
	}
}

// a one-line plate (the races' ACTION! / shout slate): accent-coloured Orbitron on ink, centred on Cx
void AFTBrawlHUD::Callout(const FString& Str, float Cx, float Y, const FLinearColor& Accent, float Height, float A)
{
	const float Tw = TextW(Str, Height, true), Pw = Tw + 32.f, Ph = Height + 14.f;
	Plate(Cx - Pw / 2, Y, Pw, Ph, Accent, A);
	Txt(Str, Cx, Y + 6.f, Al(Accent, A), Height, 1);
}

// the races' title card (FTHUD::DrawCut): a wide plate, the title in Orbitron in the accent, the line under it in Rajdhani, keys in yellow
void AFTBrawlHUD::Card(const FString& Title, const FString& Sub, const FString& Keys, const FLinearColor& Accent, float A)
{
	const float W = Canvas->ClipX / S, Cw = 680.f, Cx = W / 2, Y = 720.f * 0.32f;
	const float Ch = 112.f + (Keys.IsEmpty() ? 0.f : 30.f);
	Plate(Cx - Cw / 2, Y, Cw, Ch, Accent, A);
	Rect(Cx - Cw / 2, Y + Ch - 4.f, Cw - Corner, 3.f, Al(Accent, A));
	Txt(Title, Cx, Y + 9.f, Al(Accent, A), Readout, 1, true, Cw - 40.f);
	Txt(Sub, Cx, Y + 66.f, Al(White, A), Strong, 1, false, Cw - 44.f);
	if (!Keys.IsEmpty()) Txt(Keys, Cx, Y + 96.f, Al(Yellow, A), Label, 1, true, Cw - 44.f);
}

void AFTBrawlHUD::DrawHUD()
{
	Super::DrawHUD();
	AFTBrawlGameMode* G = GetWorld() ? GetWorld()->GetAuthGameMode<AFTBrawlGameMode>() : nullptr;
	if (!G || !Canvas) return;
	S = Canvas->ClipY / 720.f;   // the races' 720-high screen
	if (!bFonts)
	{
		bFonts = true;
		FOrb = Cast<UFont>(StaticLoadObject(UFont::StaticClass(), nullptr, TEXT("/Game/UI/Fonts/F_Orbitron"), nullptr, LOAD_NoWarn | LOAD_Quiet));
		FRaj = Cast<UFont>(StaticLoadObject(UFont::StaticClass(), nullptr, TEXT("/Game/UI/Fonts/F_Rajdhani"), nullptr, LOAD_NoWarn | LOAD_Quiet));
		float Xl = 0, Yl = 0;
		Canvas->StrLen(Face(true), TEXT("Hg"), Xl, Yl); OrbH = FMath::Max(1.f, Yl);
		Canvas->StrLen(Face(false), TEXT("Hg"), Xl, Yl); RajH = FMath::Max(1.f, Yl);
		UE_LOG(LogTemp, Log, TEXT("FTBRAWL HUD fonts orbitron=%d rajdhani=%d"), FOrb ? 1 : 0, FRaj ? 1 : 0);
	}
	if (G->InCut()) { DrawCut(G); return; }
	const float W = Canvas->ClipX / S, H = 720.f;
	const float L = FMath::Max(0.f, (W - 1280.f) / 2) + Safe, R = W - L, Mid = W / 2, Top = 25.f;
	const FFTBrawlBody& P = G->P;
	const bool bBlink = int32(G->StT * 6) % 2 != 0;
	// the "!" tells and the hurt bars over heads, the hit words where they landed
	for (const FFTBrawlBody& F : G->Foes)
	{
		if (F.St == TEXT("dead")) continue;
		const FVector Top3 = G->World(F.X, F.Y, F.Z + 140 * F.S / 2);
		const FVector Sp = Canvas->Project(Top3);
		if (Sp.Z <= 0) continue;
		if (F.St == TEXT("wind") || F.St == TEXT("aim") || F.St == TEXT("slamwind") || F.St == TEXT("whirltell") || (F.St == TEXT("dash") && F.T < 0.18f) || (F.St == TEXT("throw") && F.T < 0.3f)) Txt(TEXT("!"), Sp.X / S, Sp.Y / S - 54, Red, Readout, 1, true, 0, true);
		if (!F.bBoss && F.Hp < F.Max && F.Hp > 0)
		{
			Rect(Sp.X / S - 32, Sp.Y / S - 9, 64, 9, Al(Deep, 0.8f));
			Meter(Sp.X / S - 30, Sp.Y / S - 7, 60, 5, F.Hp / F.Max, Red, 4);
		}
	}
	for (const FFTBrawlPop& E : G->Pops)
	{
		const FVector Sp = Canvas->Project(E.P + FVector(0, 0, E.T * 40 * 1.4f));
		if (Sp.Z <= 0) continue;
		FLinearColor C = E.Col; C.A = FMath::Max(0.f, 1 - E.T);
		Txt(E.Text, Sp.X / S, Sp.Y / S, C, E.bBig ? Heading : Strong, 1, true, 0, true);
	}
	// NOVA: the races' driver plate (top left): who, HEALTH and its segmented bar, K.O. count, what he holds, the power-ups running
	{
		struct FRow { FString N, V; FLinearColor C; };
		TArray<FRow> Rows;
		if (G->Held >= 0) Rows.Add({ AFTBrawlGameMode::ItemName(G->Held), FString::Printf(TEXT("×%d"), FMath::Max(0, G->HeldUses)), White });
		auto Timer = [&](const TCHAR* N, float T, const FLinearColor& C) { if (T > 0) Rows.Add({ N, FString::Printf(TEXT("%d"), FMath::CeilToInt(T)), Al(C, T < 3 && bBlink ? 0.4f : 1.f) }); };
		Timer(TEXT("POWER"), G->PowerT, Yellow);
		Timer(TEXT("TOUGH"), G->ToughT, Cyan);
		Timer(TEXT("BUZZED"), G->BuzzT, Amber);
		const float Pw = 322.f, Ph = 108.f + Rows.Num() * 21.f, X = L;
		const float Frac = FMath::Clamp(P.Hp / FMath::Max(1.f, P.Max), 0.f, 1.f);
		const FLinearColor Hc = Frac > 0.6f ? Green : Frac > 0.3f ? Amber : Red;
		Plate(X, Top, Pw, Ph, Cyan);
		Txt(TEXT("NOVA KANE / STUNT DOUBLE"), X + 14, Top + 9, Grey, Label);
		Txt(TEXT("HEALTH"), X + 14, Top + 30, White, Body);
		Txt(FString::Printf(TEXT("%d / %d"), FMath::Max(0, FMath::RoundToInt(P.Hp)), FMath::RoundToInt(P.Max)), X + Pw - 14, Top + 28, Hc, Strong, 2, false);
		Meter(X + 14, Top + 54, Pw - 28, 8, Frac, Hc, 12);
		Rect(X + 14, Top + 72, Pw - 28, 1, Rule);
		Txt(TEXT("K.O."), X + 14, Top + 81, Grey, Label);
		Txt(FString::Printf(TEXT("%d"), G->Kos), X + Pw - 14, Top + 76, Yellow, Strong, 2);
		float Ry = Top + 104;
		for (const FRow& Rw : Rows)
		{
			Txt(Rw.N, X + 14, Ry, Rw.C, Body, 0, false);
			Txt(Rw.V, X + Pw - 14, Ry, Rw.C, Body, 2, false);
			Ry += 21;
		}
		// the combo: the races' COMBO plate, under Nova's
		if (G->Chain >= 2) Callout(FString::Printf(TEXT("%d HITS"), G->Chain), X + 90, Top + Ph + 8, Pink, Heading);
	}
	// THE VENUE: the races' ledger plate (top right) in the night's neon, WAVE n / 3 and a meter of the waves and the boss
	{
		const float Pw = 300.f, Ph = 96.f, X = R - Pw;
		const int32 At = FMath::Min(G->Wave + 1, G->NumWaves);
		Plate(X, Top, Pw, Ph, G->Neon);
		Txt(TEXT("FIGHT NIGHT"), X + 14, Top + 9, Grey, Label);
		Txt(G->VenueName, X + 14, Top + 28, G->Neon, Strong, 0, true, Pw - 28);
		Txt(G->bBossOut ? FString(TEXT("THE BOSS")) : FString::Printf(TEXT("WAVE %d / %d"), At, G->NumWaves), X + 14, Top + 58, G->bBossOut ? Red : White, Body);
		Meter(X + 14, Top + 82, Pw - 28, 5, G->bBossOut ? 1.f : float(At) / (G->NumWaves + 1), G->bBossOut ? Red : G->Neon, G->NumWaves + 1);
	}
	// J: GRAB / J: DRINK over his head
	{
		const int32 Ii = G->ItemAt();
		if (Ii != INDEX_NONE && (P.St == TEXT("idle") || P.St == TEXT("walk")))
		{
			const FVector Sp = Canvas->Project(G->World(P.X, P.Y, 250));
			if (Sp.Z > 0) Callout(G->Items[Ii].bBar ? FString(TEXT("J: DRINK")) : FString::Printf(TEXT("J: GRAB %s"), *AFTBrawlGameMode::ItemName(G->Items[Ii].Kind)), Sp.X / S, Sp.Y / S - 12, Yellow, Label);
		}
	}
	// the boss: the races' boss notice (red, a warning plate, his segmented bar), over the key strip
	const int32 Bi = G->BossIdx();
	if (Bi != INDEX_NONE && G->Foes[Bi].St != TEXT("dead"))
	{
		const FFTBrawlBody& B = G->Foes[Bi];
		const float Pw = 540.f, Ph = 56.f, X = Mid - Pw / 2, Y = H - 44 - Ph;
		Plate(X, Y, Pw, Ph, Red);
		Txt(G->BossName, X + 14, Y + 8, White, Body, 0, true, Pw - 120);
		Txt(TEXT("BOSS"), X + Pw - 14, Y + 10, Red, Label, 2);
		Meter(X + 14, Y + 35, Pw - 28, 9, B.Hp / FMath::Max(1.f, B.Max), Red, 8);
	}
	if (P.St == TEXT("held")) Callout(TEXT("MASH J / K TO BREAK FREE"), Mid, 150, Red, Strong, bBlink ? 1.f : 0.55f);
	if (G->Go > 0 && int32(G->StT * 4) % 2) Callout(TEXT("GO →"), R - 70, H / 2 - 40, Yellow, Heading);
	if (P.St == TEXT("lift") || P.St == TEXT("carry")) Callout(TEXT("L / J / K: THROW HIM (STEER TO AIM)"), Mid, 150, Yellow, Strong, bBlink ? 1.f : 0.55f);   // grab and throw (run 42137)
	if (G->Foes.Num() && Bi != INDEX_NONE && G->Foes[Bi].St == TEXT("dizzy")) Callout(TEXT("DIZZY! HIT HIM!"), Mid, 196, Yellow, Strong, bBlink ? 1.f : 0.55f);
	// the key strip: the races' (ink bar, grey Rajdhani, " / " between)
	{
#ifdef FT_BRAWL_MAIN_HUD
		const FString Keys = G->bPadLast
			? TEXT("STICK  MOVE   /   X  PUNCH, PICK UP, SWING   /   Y  KICK, THROW WEAPON   /   B  GRAB & THROW   /   A  JUMP   /   X AT THE BAR  A DRINK")
			: TEXT("WASD  MOVE   /   J  PUNCH, PICK UP, SWING   /   K  KICK, THROW WEAPON   /   L  GRAB & THROW   /   SPACE  JUMP   /   J AT THE BAR  A DRINK" FT_BRAWL_MUTE_HINT "   /   ESC  QUIT");
#else
		const FString Keys = TEXT("WASD  MOVE   /   J  PUNCH, PICK UP, SWING   /   K  KICK, THROW WEAPON   /   L  GRAB & THROW   /   SPACE  JUMP   /   J AT THE BAR  A DRINK   /   ESC  QUIT");
#endif
		const float Kw = FMath::Min(R - L, TextW(Keys, Label, false) + 28.f);
		Rect(Mid - Kw / 2, H - 32, Kw, 22, Ink);
		Txt(Keys, Mid, H - 31, Grey, Label, 1, false, Kw - 24);
	}
	if (G->BannerT >= 0)
	{
		const float Bt = G->BannerT, A = FMath::Clamp(FMath::Min3(1.f, Bt * 4, (2.4f - Bt) * 3), 0.f, 1.f);
		Card(G->Banner, G->BannerSub, FString(), G->Neon, A);
	}
	if (G->BossIntro > 0) Card(G->BossName, G->BossTitle, FString(), Red);
	// the boss's last call, from the story (after the K.O.): the races' DIRECTOR line plate, under the card
	auto LastCall = [&](const TCHAR* Who, const TCHAR* Line, const FLinearColor& C)
	{
		const float Lw = 680.f, X = Mid - Lw / 2, Y = 720.f * 0.32f + 154.f;
		Plate(X, Y, Lw, 38, C);
		Txt(Who, X + 14, Y + 11, C, Label);
		const float Lx = X + 30 + TextW(Who, Label, true);
		Txt(Line, Lx, Y + 8, White, Body, 0, false, X + Lw - 14 - Lx);
	};
	if (G->bWon && G->IsVelvet() && G->WinT > 1.2f) LastCall(TEXT("VINNIE"), TEXT("\"MY FLOOR... MY BEAUTIFUL FLOOR...\""), Pink);
	if (G->bWon && G->IsLucky() && G->WinT > 1.2f) LastCall(TEXT("THE PIT BOSS FOLDED"), TEXT("NOVA: \"THE HOUSE ALWAYS WINS? NOT TONIGHT.\""), Yellow);   // the story's last call
	if (P.Hp <= 0 && G->KoT > 1) Card(TEXT("KNOCKED OUT"), TEXT("NOVA HIT THE FLOOR. THIS WAVE AGAIN."), TEXT(FT_BRAWL_ENTER "   GET BACK UP"), Red);
	if (G->bWon)
	{
		const FString Keys = G->WinT > 2.6f ? (G->NextLabel().IsEmpty() ? FString(TEXT(FT_BRAWL_ENTER "   AGAIN")) : FString(TEXT(FT_BRAWL_ENTER "   ON TO ")) + G->NextLabel()) : FString();
		Card(TEXT("K.O.!"), FString::Printf(TEXT("%s IS DOWN  /  %d KNOCKED OUT  /  BEST COMBO %d"), *G->BossName, G->Kos, G->Best), Keys, Yellow);
	}
}

TArray<FString> AFTBrawlHUD::Wrap(const FString& Str, float MaxW, float Height, bool bDisplay)
{
	TArray<FString> Words, Lines;
	Str.ParseIntoArray(Words, TEXT(" "));
	FString Cur;
	for (const FString& Wd : Words)
	{
		const FString Try = Cur.IsEmpty() ? Wd : Cur + TEXT(" ") + Wd;
		if (TextW(Try, Height, bDisplay) > MaxW && !Cur.IsEmpty()) { Lines.Add(Cur); Cur = Wd; } else Cur = Try;
	}
	if (!Cur.IsEmpty()) Lines.Add(Cur);
	return Lines;
}

// the nights' cutscenes (run 49531; restyled run 52897 to the races' look): the letterbox in the races' reel matte (deep ink, a
// hairline rule), the title in the races' title card, the line in a plate with the speaker's colour as its accent tick and his
// name in Orbitron over the Rajdhani line, the END OF NIGHT card as the races' slate
void AFTBrawlHUD::DrawCut(AFTBrawlGameMode* G)
{
	const float W = Canvas->ClipX / S, H = 720.f, Mid = W / 2;
	const float L = FMath::Max(0.f, (W - 1280.f) / 2) + Safe, R = W - L;
	const float Bar = 74.f * G->CutBars;
	if (G->CutFlash > 0) Rect(0, 0, W, H, FLinearColor(1, 1, 1, FMath::Min(1.f, G->CutFlash * 0.9f)));
	if (G->CutFade > 0) Rect(0, 0, W, H, FLinearColor(0, 0, 0, FMath::Clamp(G->CutFade, 0.f, 1.f)));
	Rect(0, 0, W, Bar, FLinearColor(Deep.R, Deep.G, Deep.B, 1)); Rect(0, H - Bar, W, Bar, FLinearColor(Deep.R, Deep.G, Deep.B, 1));
	if (Bar > 2) { Rect(0, Bar - 1, W, 1, Rule); Rect(0, H - Bar, W, 1, Rule); }
	if (!G->CutCard.IsEmpty())
	{
		// the races' slate (FTHUD::DrawCard): plate, the title in Orbitron, a rule, the line, the keys
		const float Cw = 720.f, X = Mid - Cw / 2, Y = H / 2 - 112.f, Ch = 214.f;
		Plate(X, Y, Cw, Ch, Pink);
		Txt(TEXT("FULL THROTTLE  /  FIGHT NIGHTS"), X + 22, Y + 18, Grey, Label);
		Txt(G->CutCard, X + 22, Y + 46, Pink, Readout, 0, true, Cw - 44);
		Rect(X + 22, Y + 112, Cw - 44, 1, Rule);
		Txt(G->CutCardSub, X + 22, Y + 126, White, Strong, 0, false, Cw - 44);
		if (int32(G->StT * 2) % 2 == 0) Txt(G->CutCardKeys, X + 22, Y + 170, Yellow, Label, 0, true, Cw - 44);
		return;
	}
	if (G->CutTitleT >= 0)
	{
		const float T = G->CutTitleT, A = FMath::Clamp(FMath::Min3(1.f, T * 2.5f, (3.2f - T) * 2.f), 0.f, 1.f);
		const float Cw = 680.f, X = Mid - Cw / 2, Y = Bar + 40.f, Ch = 104.f;
		Plate(X, Y, Cw, Ch, Pink, A);
		Rect(X, Y + Ch - 4.f, (Cw - Corner) * FMath::Min(1.f, T * 3.f), 3.f, Al(Pink, A));
		Txt(G->CutTitle, Mid, Y + 12, Al(Pink, A), Heading + 4.f, 1, true, Cw - 40);
		Txt(G->CutSub, Mid, Y + 62, Al(White, A), Strong, 1, false, Cw - 44);
	}
	if (!G->CutLine.IsEmpty())
	{
		const int32 N = FMath::Min(G->CutLine.Len(), FMath::FloorToInt(G->CutLineT * 45.f));
		const bool bVo = G->CutWho.IsEmpty();
		const float Lh = 34.f, Fs = 26.f, Bw = FMath::Min(R - L, 900.f), Bx = Mid - Bw / 2;
		const TArray<FString> Lines = Wrap(G->CutLine, Bw - 48.f, Fs, false);
		const float Bh = 26.f + Lines.Num() * Lh + (bVo ? 0.f : 26.f), By = H - Bar - Bh - 14.f;
		const FLinearColor Ac = bVo ? Grey : G->CutWhoCol;
		Plate(Bx, By, Bw, Bh, Ac);
		Rect(Bx, By + 2, 3, Bh - 2 - FMath::Min(Corner, Bh * 0.3f), Ac);   // the speaker's colour down the side, like the races' popups
		float Y = By + 12;
		if (!bVo) { Txt(G->CutWho, Bx + 22, Y + 2, Ac, Label); Y += 26; }
		int32 Left = N;
		for (const FString& Ln : Lines)
		{
			if (Left <= 0) break;
			Txt(Ln.Left(Left), Bx + 22, Y, bVo ? Al(White, 0.88f) : White, Fs, 0, false);
			Left -= Ln.Len() + 1; Y += Lh;
		}
	}
	if (Bar > 20) Txt(TEXT(FT_BRAWL_ENTER "   NEXT   /   " FT_BRAWL_SKIP "   SKIP"), R, H - Bar + (Bar - Label) / 2, Grey, Label, 2);
}
