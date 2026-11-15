#include "FTHUD.h"
#include "FTHUDStyle.h"
#include "FTGameMode.h"
#include "FTGameInstance.h"
#include "FTPlayerCar.h"
#include "FTEntity.h"
#include "FTPlayerController.h"
#include "FTProgress.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Engine/Engine.h"
#include "CanvasItem.h"
#include "Engine/Texture2D.h"

using namespace FTHUDStyle;

namespace
{
	FString Num(int64 V)
	{
		FString R = FString::Printf(TEXT("%lld"), V);
		for (int32 i = R.Len() - 3; i > (V < 0 ? 1 : 0); i -= 3) R.InsertAt(i, TEXT(","));
		return R;
	}
	FString Money(int64 V) { return V < 0 ? TEXT("-$") + Num(V).RightChop(1) : TEXT("$") + Num(V); }

	FString Binding(const UFTGameInstance* GI, EFTAction Action)
	{
		if (!GI) return TEXT("UNBOUND");
		const FKey& Key = GI->Binds[static_cast<int32>(Action)][GI->Layout];
		return Key.IsValid() ? Key.GetDisplayName().ToString().ToUpper() : FString(TEXT("UNBOUND"));
	}
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

FBox2D AFTHUD::Rect(float X, float Y, float Wd, float Ht) const
{
	return FBox2D(FVector2D(X, Y), FVector2D(X + Wd, Y + Ht));
}

void AFTHUD::Stroke(FVector2D A, FVector2D B, const FLinearColor& Color, float Thickness)
{
	FCanvasLineItem Line(A, B);
	Line.SetColor(Color);
	Line.LineThickness = Thickness * S;
	Canvas->DrawItem(Line);
}

void AFTHUD::Plate(const FBox2D& R, const FLinearColor& Accent, const FLinearColor& Fill)
{
	const FVector2D Size = R.GetSize();
	const float X = R.Min.X, Y = R.Min.Y, Wd = Size.X, Ht = Size.Y;
	const float Clipped = FMath::Min(Corner * S, Ht * 0.3f);
	Box(X, Y, Wd, Ht - Clipped, Fill);
	Box(X, Y + Ht - Clipped, Wd - Clipped, Clipped, Fill);
	FCanvasTriangleItem Cut(FVector2D(R.Max.X - Clipped, R.Max.Y - Clipped),
		FVector2D(R.Max.X, R.Max.Y - Clipped), FVector2D(R.Max.X - Clipped, R.Max.Y), GWhiteTexture);
	Cut.SetColor(Fill);
	Cut.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(Cut);
	Box(X, Y, Wd, S, Rule);
	Box(X, R.Max.Y - S, Wd - Clipped, S, Rule);
	Box(X, Y, S, Ht, Rule);
	Box(R.Max.X - S, Y, S, Ht - Clipped, Rule);
	Stroke(FVector2D(R.Max.X - Clipped, R.Max.Y), FVector2D(R.Max.X, R.Max.Y - Clipped), Rule);
	Box(X, Y, FMath::Min(Wd * 0.34f, 72 * S), 2 * S, Accent);
}

FVector2D AFTHUD::TextSize(const FString& Str, float Height, bool bDisplay)
{
	return MeasureF(bDisplay ? (FOrb ? FOrb.Get() : FD.Get()) : (FRaj ? FRaj.Get() : FB.Get()),
		Str, Height / (bDisplay ? OrbitHeight : RajHeight));
}

void AFTHUD::Print(const FString& Str, float X, float Y, const FLinearColor& Color, float Height, int32 Align, bool bDisplay, float MaxWidth)
{
	if (Str.IsEmpty()) return;
	UFont* Font = bDisplay ? (FOrb ? FOrb.Get() : FD.Get()) : (FRaj ? FRaj.Get() : FB.Get());
	float Scale = Height / (bDisplay ? OrbitHeight : RajHeight);
	float Measured = Align != 0 || MaxWidth > 0.f ? MeasureF(Font, Str, Scale).X : 0.f;
	if (MaxWidth > 0.f && Measured > MaxWidth)
	{
		Scale *= MaxWidth / Measured;
		Measured = MaxWidth;
	}
	if (Align != 0) X -= Align == 1 ? Measured * .5f : Measured;
	FCanvasTextItem Item(FVector2D(X, Y), FText::FromString(Str), Font, Color);
	Item.Scale = FVector2D(Scale * S);
	Canvas->DrawItem(Item);
}

void AFTHUD::Meter(const FBox2D& R, float Fraction, const FLinearColor& Accent, int32 Segments)
{
	const float GapPx = Segments > 1 ? 3.f * S : 0.f;
	const float Pitch = (R.GetSize().X + GapPx) / Segments;
	const float Wd = Pitch - GapPx;
	const float Filled = FMath::Clamp(Fraction, 0.f, 1.f) * Segments;
	for (int32 Index = 0; Index < Segments; ++Index)
	{
		const float X = R.Min.X + Index * Pitch;
		const float Ht = R.GetSize().Y;
		Box(X, R.Min.Y, Wd, Ht, Track);
		const float Value = FMath::Clamp(Filled - Index, 0.f, 1.f);
		if (Value > 0.f) Box(X, R.Min.Y, Wd * Value, Ht, Accent);
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
		FCanvasNGonItem Light(Dot(.5f, .5f), FVector2D(Size * .28f), 12, Color);
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

FBox2D AFTHUD::Control(const FBox2D& R, const FString& Label, const FString& Hint, EControl State, const FLinearColor& Accent)
{
	const bool bEnabled = State != EControl::Disabled;
	const bool bHot = State == EControl::Hover || State == EControl::Focus || State == EControl::Pressed;
	const FLinearColor Fill = State == EControl::Pressed ? Deep : bHot ? Active : Steel;
	const FLinearColor Edge = State == EControl::Hover ? White : bHot ? Accent : Rule;
	const FVector2D Size = R.GetSize();
	Box(R.Min.X, R.Min.Y, Size.X, Size.Y, bEnabled ? Fill : Ink);
	Frame(R.Min.X, R.Min.Y, Size.X, Size.Y, Edge, bHot ? 1.5f : 1.f);
	if (bHot)
	{
		Box(R.Min.X + 3 * S, R.Min.Y + 4 * S, 3 * S, Size.Y - 8 * S, Edge);
		Stroke(FVector2D(R.Max.X - 14 * S, R.Min.Y + S), FVector2D(R.Max.X - S, R.Min.Y + 14 * S), Edge);
	}
	const float Mid = R.GetCenter().X;
	const float Shift = State == EControl::Pressed ? S : 0.f;
	Print(Label, Mid, R.Min.Y + (Hint.IsEmpty() ? Size.Y * .24f : 5 * S) + Shift, bEnabled ? White : Grey,
		Strong, 1, true, Size.X - 20 * S);
	Print(Hint, Mid, R.Min.Y + Size.Y - 19 * S + Shift, bHot ? Accent : Grey, LabelSize, 1, false, Size.X - 20 * S);
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
		case EFTOverlay::Controls:
			View = EMenuView::Controls;
			Selection = (G.ControlsRow * 2 + G.ControlsCol) * 2 + int32(G.bControlsWaiting);
			break;
		case EFTOverlay::Options:
			break;
		case EFTOverlay::None:
			if (G.bAttract || G.Phase == EFTPhase::Title)
			{
				View = EMenuView::Title;
				Page = int32(G.TitleActionFocus != INDEX_NONE);
				Selection = Page ? G.TitleActionFocus : G.TitleSel;
			}
			else if (G.Phase == EFTPhase::PreProduction && G.Tab != EFTTab::Skills)
			{
				View = EMenuView::Department;
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

void AFTHUD::FocusAccent(const FBox2D& Button, float Age, const FLinearColor& Accent)
{
	AccentSweep(Rect(Button.Min.X + 5 * S, Button.Max.Y - 4 * S, Button.GetSize().X - 10 * S, 3 * S),
		Age, Accent, FocusSweep);
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
	bReducedMotion = G->GI() && G->GI()->bReducedMotion;
	UpdateMenuMotion(*G);
	if (!bTitleAssets)
	{
		bTitleAssets = true;
		FD = DisplayFont ? DisplayFont.Get() : GEngine->GetLargeFont();
		FB = BodyFont ? BodyFont.Get() : GEngine->GetMediumFont();
		FOrb = Cast<UFont>(StaticLoadObject(UFont::StaticClass(), nullptr, TEXT("/Game/UI/Fonts/F_Orbitron"), nullptr, LOAD_NoWarn | LOAD_Quiet));
		FRaj = Cast<UFont>(StaticLoadObject(UFont::StaticClass(), nullptr, TEXT("/Game/UI/Fonts/F_Rajdhani"), nullptr, LOAD_NoWarn | LOAD_Quiet));
		TLogo = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/UI/Title/T_TitleLogo"), nullptr, LOAD_NoWarn | LOAD_Quiet));
		OrbitHeight = FMath::Max(1.f, float(MeasureF(FOrb ? FOrb.Get() : FD.Get(), TEXT("Hg"), 1.f).Y / S));
		RajHeight = FMath::Max(1.f, float(MeasureF(FRaj ? FRaj.Get() : FB.Get(), TEXT("Hg"), 1.f).Y / S));
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
	case EFTPhase::Play: if (G->Overlay == EFTOverlay::None) { DrawPickupMarkers(*G); } DrawPlay(G); break;   // braces: clang -Werror,-Wmisleading-indentation (Mac)
	case EFTPhase::Wreck: case EFTPhase::Wrap: DrawPlay(G); DrawCut(G); break;
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
	default: break;
	}
	if (const UFTGameInstance* GI = G->GI())
	{
		if (GI->bShowFps) Print(FString::Printf(TEXT("%.0f FPS  %.1f ms"), G->FpsSmoothed, 1000.f / FMath::Max(1.f, G->FpsSmoothed)), W - 12 * S, H - 22 * S, Green, LabelSize, 2);
		if (GI->bMuted) Print(TEXT("MUTED / M"), 12 * S, H - 22 * S, Grey, LabelSize);
	}
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

void AFTHUD::DrawTitle(AFTGameMode* G)
{
	const UFTGameInstance* GI = G->GI();
	const bool bInteractive = G->Overlay == EFTOverlay::None;
	const float X = Left + Safe * S, Y = (H - CanvasHeight * S) * .5f;
	const float BoardW = 442 * S, ActionY = H - 88 * S;
	const FLinearColor Night = Alpha(Deep, 0.f);

	if (!G->bAttract) Box(0, 0, W, H, Alpha(Deep, .46f));
	Grad(0, 0, X + BoardW + 160 * S, H, Deep, Night, true);
	Grad(0, H - 135 * S, W, 135 * S, Night, Deep, false);
	Plate(Rect(X, Y + 25 * S, BoardW, ActionY - Y - 41 * S), Pink, Ink);

	const float Entry = bInteractive ? G->TitleT : -1.f;
	AccentSweep(Rect(X + 1 * S, Y + 26 * S, BoardW - 2 * S, 4 * S), Entry, Pink, MenuEntrance);
	const float In = bReducedMotion || !bInteractive ? 1.f : EaseOut(Entry, MenuEntrance);
	const float LogoW = 395 * S;
	if (TLogo && TLogo->GetResource())
	{
		const float LogoH = LogoW * TLogo->GetSizeY() / FMath::Max(1, TLogo->GetSizeX());
		FCanvasTileItem Logo(FVector2D(X + (BoardW - LogoW) * .5f, Y + (47.f + 12.f * (1.f - In)) * S),
			TLogo->GetResource(), FVector2D(LogoW, LogoH), Alpha(FLinearColor::White, .74f + .26f * In));
		Logo.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(Logo);
	}
	else Print(TEXT("FULL THROTTLE"), X + BoardW * .5f, Y + 89 * S, White, Heading, 1, true, BoardW - 30 * S);

	Print(TEXT("AN 80s SPY CAR CHASE"), X + 16 * S, Y + 183 * S, Grey, Body);
	Box(X + 16 * S, Y + 211 * S, BoardW - 32 * S, S, Rule);
	Print(TEXT("SELECT YOUR SHOOT"), X + 16 * S, Y + 222 * S, White, Strong, 0, true);
	Print(TEXT("1–8  /  UP-DOWN  /  D-PAD"), X + BoardW - 16 * S, Y + 224 * S, Grey, LabelSize, 2);
	const int32 Count = FTData::NumLevels();
	G->TitleRowRects.SetNum(Count);
	const int32 Selection = FMath::Clamp(G->TitleSel, 0, Count - 1);
	const float RowH = FMath::Min(39.f, 312.f / Count) * S;
	for (int32 I = 0; I < Count; ++I)
	{
		const FFTLevelDef& Level = FTData::Level(I);
		const FBox2D Row = Rect(X + 14 * S, Y + 252 * S + I * RowH, BoardW - 28 * S, RowH - 3 * S);
		const bool bFocus = Selection == I;
		const EControl State = bInteractive ? ControlState(Row, bFocus) : (bFocus ? EControl::Focus : EControl::Idle);
		const bool bHover = State == EControl::Hover || State == EControl::Pressed;
		const FLinearColor Accent = bFocus ? Cyan : bHover ? White : Rule;
		Box(Row.Min.X, Row.Min.Y, Row.GetSize().X, Row.GetSize().Y, State == EControl::Pressed ? Deep : bFocus ? Active : Steel);
		Box(Row.Min.X, Row.Min.Y, (bFocus ? 4 : 1) * S, Row.GetSize().Y, Accent);
		if (bFocus || bHover) Frame(Row.Min.X, Row.Min.Y, Row.GetSize().X, Row.GetSize().Y, Accent, 1.f);
		const float RowEntry = Entry - .075f - I * RowStagger;
		if (bFocus && G->TitleActionFocus == INDEX_NONE && Entry >= MenuEntrance)
			FocusAccent(Row, MenuAge(EMenuView::Title, true), Cyan);
		else
			AccentSweep(Rect(Row.Min.X + 5 * S, Row.Min.Y + S, Row.GetSize().X - 10 * S, 2 * S),
				RowEntry, bFocus ? Cyan : Pink, FocusSweep);
		Print(FString::Printf(TEXT("%02d"), I + 1), Row.Min.X + 22 * S, Row.Min.Y + 9 * S, bFocus ? Cyan : Grey, LabelSize, 1, true);
		Print(Level.Zone, Row.Min.X + 45 * S, Row.Min.Y + 2 * S, White, Body, 0, true, 232 * S);
		Print(Level.Title, Row.Min.X + 45 * S, Row.Min.Y + 18 * S, Grey, LabelSize, 0, false, 232 * S);
		const bool bOnAir = G->bAttract && G->Level == I && G->AttractOutT < 0.f;
		const bool bHasBest = GI && I < UFTGameInstance::MaxLevels && GI->BestScore[I] > 0;
		if (bOnAir)
		{
			Icon(EIcon::Record, Row.Max.X - (bHasBest ? 131 : 73) * S, Row.Min.Y + 9 * S, 16 * S, Pink);
			if (!bHasBest) Print(TEXT("ON AIR"), Row.Max.X - 10 * S, Row.Min.Y + 9 * S, Pink, LabelSize, 2);
		}
		if (bHasBest)
		{
			Print(FString::Printf(TEXT("BEST %s"), *Num(GI->BestScore[I])), Row.Max.X - 10 * S, Row.Min.Y + 2 * S, Yellow, LabelSize, 2, false, 120 * S);
			Print(GI->BestTitle[I], Row.Max.X - 10 * S, Row.Min.Y + 17 * S, Grey, LabelSize, 2, false, 120 * S);
		}
		G->TitleRowRects[I] = Row;
	}
	if (const UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(G))
		Print(FString::Printf(TEXT("DRIVER LV %d   /   %s   /   %d SKILL POINTS"), CS->C.Level, *Money(CS->C.Cash), CS->SPLeft()),
			X + 16 * S, Y + 573 * S, Yellow, Body, 0, false, BoardW - 32 * S);

	static const TCHAR* Actions[] = { TEXT("ROLL CAMERA"), TEXT("PRE-PRODUCTION"), TEXT("CONTROLS"), TEXT("OPTIONS"), TEXT("QUIT") };
	static const TCHAR* Hints[] = { TEXT("ENTER / A"), TEXT("G / PAD BACK"), TEXT("C / Y"), TEXT("O"), TEXT("ESC") };
	const AFTPlayerController* PC = Cast<AFTPlayerController>(GetOwningPlayerController());
	const float BarW = Right - X - Safe * S, LeadW = 272 * S;
	const float OtherW = (BarW - LeadW - 4 * Gap * S) * .25f;
	G->TitleBtnRects.SetNum(UE_ARRAY_COUNT(Actions));
	float Bx = X;
	for (int32 I = 0; I < UE_ARRAY_COUNT(Actions); ++I)
	{
		const float Bw = I == 0 ? LeadW : OtherW;
		const FBox2D ButtonRect = Rect(Bx, ActionY, Bw, 50 * S);
		const bool bEnabled = I != 4 || G->TitleT > 1.f;
		const bool bFocus = I == (G->TitleActionFocus == INDEX_NONE ? 0 : G->TitleActionFocus);
		EControl State = bInteractive ? ControlState(ButtonRect, bFocus, bEnabled) : EControl::Idle;
		if (bFocus && bInteractive && PC && (PC->IsInputKeyDown(EKeys::Enter) || PC->IsInputKeyDown(EKeys::Gamepad_FaceButton_Bottom)))
			State = EControl::Pressed;
		G->TitleBtnRects[I] = Control(ButtonRect, Actions[I], Hints[I], State, I == 0 ? Cyan : Pink);
		if (bFocus && bEnabled && G->TitleActionFocus != INDEX_NONE)
			FocusAccent(ButtonRect, MenuAge(EMenuView::Title, true), I == 0 ? Cyan : Pink);
		Bx += Bw + Gap * S;
	}
	Print(G->TitleActionFocus == INDEX_NONE ? TEXT("TAB / RB   ACTIONS") : TEXT("LEFT-RIGHT / D-PAD   ACTIONS     TAB / RB   SHOOTS"),
		W * .5f, ActionY + 58 * S, Grey, LabelSize, 1);

	if (G->bAttract) DrawAttractOsd(G);
	if (GI && !GI->TitleBanner.IsEmpty() && G->TitleT < 8.f)
	{
		const float Center = (X + BoardW + Right) * .5f;
		const FBox2D Banner = Rect(Center - 230 * S, Y + 227 * S, 460 * S, 68 * S);
		Plate(Banner, Yellow, Ink);
		Print(GI->TitleBanner, Center, Banner.Min.Y + 13 * S, Yellow, Heading, 1, true, 420 * S);
	}
}

void AFTHUD::DrawAttractOsd(AFTGameMode* G)
{
	const bool bOut = G->AttractOutT >= 0.f;
	const int32 Index = bOut ? G->AttractNextLevel() : G->Level;
	const FFTLevelDef& Level = FTData::Level(Index);
	const float X = Right - (Safe + 306.f) * S, Y = 28 * S;
	const FBox2D Monitor = Rect(X, Y, 306 * S, 67 * S);
	Plate(Monitor, Pink, Ink);
	if (G->Overlay == EFTOverlay::None)
		AccentSweep(Rect(X + 7 * S, Monitor.Max.Y - 6 * S, 284 * S, 3 * S), G->TitleT - .085f, Pink, MenuEntrance);
	Icon(EIcon::Record, X + 12 * S, Y + 10 * S, 18 * S, Pink);
	Print(bOut ? TEXT("CHANGING FEED") : TEXT("ATTRACT FEED"), X + 36 * S, Y + 10 * S, Pink, LabelSize, 0, true);
	Print(FString::Printf(TEXT("CH %02d"), Index + 1), Monitor.Max.X - 14 * S, Y + 10 * S, Grey, LabelSize, 2, true);
	Print(Level.Zone + TEXT(" / ") + Level.Title, X + 15 * S, Y + 34 * S, White, Strong, 0, false, 276 * S);

	const float R = Right - Safe * S, Bottom = H - 121 * S;
	Stroke(FVector2D(R - 40 * S, Bottom), FVector2D(R, Bottom), Rule, 2.f);
	Stroke(FVector2D(R, Bottom), FVector2D(R, Bottom - 28 * S), Rule, 2.f);
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

void AFTHUD::DrawPlay(AFTGameMode* G)
{
	const FFTLevelDef& Level = G->Def();
	const UFTGameInstance* GI = G->GI();
	const FFTProgressData& ProgressData = FFTProgressData::Get();
	const AFTPlayerController* PC = Cast<AFTPlayerController>(GetOwningPlayerController());
	const bool bPad = PC && PC->UsesGamepadPrompts();
	const auto Hint = [&](EFTAction Action, const TCHAR* Pad) { return bPad ? FString(Pad) : Binding(GI, Action); };
	const float Now = G->RealClockNow();
	const float X = Left + Safe * S, R = Right - Safe * S, Center = W * .5f;
	const float Top = 25 * S, Base = H - 47 * S;
	const float Hp = G->HpPct(), TurboFrac = G->Turbo / FMath::Max(1.f, G->Stats.TurboTank);
	const int32 Load = FMath::Clamp(G->AmmoLoad, 0, 2), Rounds = Load > 0 ? G->Run.Rounds[Load - 1] : 0;
	if (LastArmor >= 0.f && !FMath::IsNearlyEqual(LastArmor, G->Hp)) ArmorChanged = Now;
	if (LastMissiles >= 0 && LastMissiles != G->Missiles) MissileChanged = Now;
	if (LastAmmoLoad >= 0 && (LastAmmoLoad != Load || LastAmmoRounds != Rounds)) AmmoChanged = Now;
	LastArmor = G->Hp;
	LastMissiles = G->Missiles;
	LastAmmoLoad = Load;
	LastAmmoRounds = Rounds;
	const auto Emphasis = [&](float Changed, const FLinearColor& Normal)
	{
		const float Amount = bReducedMotion ? 0.f : 1.f - EaseOut(Now - Changed, Feedback);
		return FMath::Lerp(Normal, White, Amount * .65f);
	};

	// Primary instruments sit on either side of the car and stay within the center 16:9 area.
	const float DriverW = 322 * S, DriverY = Base - 171 * S;
	Plate(Rect(X, DriverY, DriverW, 171 * S), Cyan, Ink);
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
	Icon(Hp <= 25.f ? EIcon::Warning : EIcon::Repair, X + 14 * S, DriverY + 84 * S, IconSize * S, ArmorColor);
	Print(TEXT("ARMOR"), X + 39 * S, DriverY + 84 * S, White, Body, 0, true);
	Print(FString::Printf(TEXT("%d%%   %.0f / %.0f"), FMath::RoundToInt(Hp), G->Hp, G->MaxHp()),
		X + 307 * S, DriverY + 84 * S, Emphasis(ArmorChanged, ArmorColor), Strong, 2, false, 190 * S);
	Meter(Rect(X + 14 * S, DriverY + 107 * S, 294 * S, 8 * S), Hp / 100.f, ArmorColor, 12);
	Print(Damage, X + 14 * S, DriverY + 117 * S, ArmorColor, LabelSize);

	const TCHAR* Turbo = G->bTurboOn ? TEXT("BURN") : G->BoostT > 0.f ? TEXT("BOOST") : G->bTurboLock ? TEXT("RECHARGING") : TurboFrac >= .99f ? TEXT("READY") : TEXT("CHARGING");
	const FLinearColor TurboColor = G->bTurboOn ? Pink : Cyan;
	Icon(EIcon::Turbo, X + 14 * S, DriverY + 135 * S, 16 * S, TurboColor);
	Print(FString::Printf(TEXT("TURBO  [%s]"), *Hint(EFTAction::Turbo, TEXT("LB/RT"))), X + 37 * S, DriverY + 134 * S, White, LabelSize, 0, false, 154 * S);
	Print(G->bDraft ? FString(Turbo) + TEXT(" / DRAFT") : FString(Turbo), X + 307 * S, DriverY + 134 * S, TurboColor, LabelSize, 2);
	Meter(Rect(X + 14 * S, DriverY + 153 * S, 294 * S, 7 * S), TurboFrac, TurboColor, 8);

	const float WeaponW = 300 * S, WX = R - WeaponW, WY = Base - 163 * S;
	Plate(Rect(WX, WY, WeaponW, 163 * S), Amber, Ink);
	Print(TEXT("ORDNANCE"), WX + 14 * S, WY + 8 * S, Grey, LabelSize, 0, true);
	Icon(EIcon::Missile, WX + 14 * S, WY + 29 * S, IconSize * S, Amber);
	Print(FString::Printf(TEXT("MISSILES  [%s]"), *Hint(EFTAction::Missile, TEXT("B"))), WX + 39 * S, WY + 29 * S, White, Body, 0, false, 160 * S);
	const int32 Capacity = G->MissileCap();
	Print(FString::Printf(TEXT("%d / %d"), G->Missiles, Capacity), R - 14 * S, WY + 26 * S,
		Emphasis(MissileChanged, G->Missiles > 0 ? Amber : Grey), Strong, 2, true, 82 * S);
	Meter(Rect(WX + 14 * S, WY + 54 * S, 272 * S, 6 * S), float(G->Missiles) / FMath::Max(1, Capacity), Amber, FMath::Clamp(Capacity, 1, 16));

	Icon(EIcon::Ammo, WX + 14 * S, WY + 68 * S, IconSize * S, Cyan);
	Print(FString::Printf(TEXT("AMMO  [%s]"), *Hint(EFTAction::Ammo, TEXT("Y"))), WX + 39 * S, WY + 67 * S, White, Body, 0, false, 150 * S);
	if (Load > 0) Print(Num(Rounds), R - 14 * S, WY + 65 * S, Emphasis(AmmoChanged, Cyan), Strong, 2, true, 94 * S);
	static const TCHAR* LoadNames[] = { TEXT("STD"), TEXT("AP"), TEXT("HE") };
	static const FLinearColor LoadColors[] = { Yellow, Cyan, Amber };
	for (int32 I = 0; I < UE_ARRAY_COUNT(LoadNames); ++I)
	{
		const float Px = WX + (14 + I * 92) * S;
		const bool bOwned = I == 0 || (I == 1 ? G->Stats.ApRank > 0 : G->Stats.HeRank > 0);
		Box(Px, WY + 92 * S, 86 * S, 23 * S, I == Load ? Active : bOwned ? Steel : Deep);
		if (I == Load) Box(Px, WY + 113 * S, 86 * S, 2 * S, LoadColors[I]);
		Print(LoadNames[I], Px + 43 * S, WY + 94 * S, I == Load ? LoadColors[I] : bOwned ? White : Grey,
			LabelSize, 1, true);
	}
	const TCHAR* Drift = G->BoostT > 0.f ? TEXT("BOOST!") : !G->bDrifting ? TEXT("HOLD + STEER")
		: G->DriftLevel == 0 ? TEXT("SLIDING") : G->DriftLevel == 1 ? TEXT("BLUE") : G->DriftLevel == 2 ? TEXT("ORANGE") : TEXT("PINK!");
	const FLinearColor DriftColor = G->DriftLevel >= 3 ? Pink : G->DriftLevel == 2 ? Amber : Cyan;
	Print(FString::Printf(TEXT("DRIFT  [%s]"), *Hint(EFTAction::Drift, TEXT("A/RB"))), WX + 14 * S, WY + 120 * S, Grey, LabelSize, 0, false, 136 * S);
	Print(Drift, R - 14 * S, WY + 120 * S, G->bDrifting || G->BoostT > 0.f ? DriftColor : Grey, LabelSize, 2);
	Meter(Rect(WX + 14 * S, WY + 141 * S, 272 * S, 5 * S), G->DriftLevel / 3.f, DriftColor, 3);

	// Secondary information stays near the top edge. Temporary cards share the same lane.
	const float ObjectiveW = 362 * S, OX = Center - ObjectiveW * .5f;
	Plate(Rect(OX, Top, ObjectiveW, 51 * S), Cyan, Ink);
	Print(FString::Printf(TEXT("SCENE %d/%d  /  TAKE %d"), G->Scene + 1, G->NumScenes(), G->Take), OX + 12 * S, Top + 7 * S, White, LabelSize, 0, true);
	Print(TEXT("FINALE"), OX + ObjectiveW - 12 * S, Top + 7 * S, Pink, LabelSize, 2, true);
	const float ProgressW = ObjectiveW - 24 * S;
	Meter(Rect(OX + 12 * S, Top + 31 * S, ProgressW, 7 * S), G->Progress, Cyan);
	for (const FFTScene& Scene : Level.Scenes)
		Box(OX + 12 * S + ProgressW * FMath::Clamp(Scene.D / FMath::Max(1.f, Level.FinaleD), 0.f, 1.f) * .85f, Top + 28 * S, S, 12 * S, Grey);
	Box(OX + 12 * S + ProgressW * .85f, Top + 27 * S, 2 * S, 14 * S, Pink);
	float NoticeY = Top + 58 * S;
	if (G->bBossOn)
	{
		Plate(Rect(OX, NoticeY, ObjectiveW, 66 * S), Red, Ink);
		Icon(EIcon::Warning, OX + 12 * S, NoticeY + 8 * S, 16 * S, Red);
		Print(G->BossLabel, OX + 36 * S, NoticeY + 7 * S, White, Body, 0, true, 206 * S);
		const FString GapLabel = G->bLastChance ? TEXT("LAST CHANCE!") : G->BossRate < 1.f
			? FString::Printf(TEXT("GAP ×%.2f"), G->BossRate) : FString(TEXT("GAP"));
		Print(GapLabel, OX + ObjectiveW - 12 * S, NoticeY + 8 * S, G->bLastChance ? Red : Yellow, Body, 2, false, 120 * S);
		Meter(Rect(OX + 12 * S, NoticeY + 33 * S, ProgressW, 8 * S), G->BossHpFrac, Red, 8);
		Meter(Rect(OX + 12 * S, NoticeY + 48 * S, ProgressW, 5 * S), G->BossClock / FMath::Max(1.f, G->BossEscapeT), G->bLastChance ? Red : Yellow);
		NoticeY += 73 * S;
	}
	if (Now >= G->CaptionT && Now - G->CaptionT < 3.f)
	{
		Plate(Rect(OX, NoticeY, ObjectiveW, 31 * S), Red, Ink);
		Print(G->Caption, Center, NoticeY + 5 * S, Red, Strong, 1, true, ObjectiveW - 24 * S);
		NoticeY += 37 * S;
	}
	if (!G->Line.IsEmpty() && Now - G->LineT < G->LineHold + 1.2f)
	{
		const FLinearColor Accent = G->LinePri >= 3 ? Yellow : G->LinePri >= 2 ? Pink : Cyan;
		const float LineW = 480 * S, LineX = Center - LineW * .5f;
		const float Offset = bReducedMotion ? 0.f : (1.f - EaseOut(Now - G->LineT)) * 6 * S;
		const float Y = NoticeY - Offset;
		Plate(Rect(LineX, Y, LineW, 36 * S), Accent, Ink);
		Print(TEXT("DIRECTOR"), LineX + 12 * S, Y + 10 * S, Accent, LabelSize, 0, true);
		Print(G->Line, LineX + 108 * S, Y + 7 * S, White, Body, 0, false, LineW - 122 * S);
		NoticeY += 43 * S;
	}
	const bool bAction = Now >= G->ActionT && Now - G->ActionT < 1.f;
	if (bAction || Now - G->ShoutT < 1.f || Now - G->SceneSlateT < 2.8f)
	{
		const FString Text = bAction ? TEXT("ACTION!") : Now - G->ShoutT < 1.f ? G->ShoutWord
			: Level.Scenes.IsValidIndex(G->Scene) ? Level.Scenes[G->Scene].Name : FString();
		Plate(Rect(Center - 148 * S, NoticeY, 296 * S, 34 * S), Yellow, Ink);
		Print(Text, Center, NoticeY + 5 * S, Yellow, Strong, 1, true, 272 * S);
	}

	// The recording ledger and the latest three rewards stay out of the driving corridor.
	const float LedgerW = 255 * S, LX = R - LedgerW;
	Plate(Rect(LX, Top, LedgerW, 173 * S), Pink, Ink);
	Icon(EIcon::Record, LX + 10 * S, Top + 7 * S, 18 * S, Pink);
	Print(TEXT("NOW FILMING"), LX + 33 * S, Top + 8 * S, White, LabelSize, 0, true);
	const int32 Tc = FMath::FloorToInt(G->Timecode);
	const int32 FrameNo = FMath::FloorToInt(FMath::Fmod(G->Timecode, 1.f) * 24.f);
	Print(FString::Printf(TEXT("%02d:%02d:%02d"), Tc / 60, Tc % 60, FrameNo), R - 12 * S, Top + 8 * S, Grey, LabelSize, 2);
	Print(TEXT("BOX OFFICE"), LX + 12 * S, Top + 30 * S, Grey, LabelSize);
	Print(Money(G->Score), LX + 12 * S, Top + 46 * S, Yellow, Heading, 0, true, 228 * S);
	const FString Counts = G->Combo > 1 ? FString::Printf(TEXT("COMBO ×%d"), G->Combo)
		: FString::Printf(TEXT("KILLS %d    NEAR MISSES %d"), G->Kills, G->NearMisses);
	Print(Counts, LX + 12 * S, Top + 83 * S, G->Combo > 1 ? Pink : White, Body, 0, false, 227 * S);
	if (G->Combo > 1) Meter(Rect(LX + 135 * S, Top + 91 * S, 107 * S, 4 * S), G->ComboT / 3.f, Pink);
	Print(FString::Printf(TEXT("STUNTS %d   /   COLLATERAL %d"), G->StuntCount, G->Collateral), LX + 12 * S, Top + 104 * S, Grey, LabelSize, 0, false, 227 * S);
	const int32 Time = FMath::FloorToInt(G->LevelClock), Par = ProgressData.Levels.IsValidIndex(G->Level) ? ProgressData.Levels[G->Level].Par : 0;
	Print(FString::Printf(TEXT("CARNAGE %d%%  /  %d:%02d  PAR %d:%02d"),
		FMath::RoundToInt(100.f * G->Run.Wrecked / FMath::Max(1, G->ScriptTotal)), Time / 60, Time % 60, Par / 60, Par % 60),
		LX + 12 * S, Top + 123 * S, Grey, LabelSize, 0, false, 227 * S);
	Print(FString::Printf(TEXT("FILM  %d/%d"), FMath::Max(0, G->Stats.Reels - G->Retakes), G->Stats.Reels), LX + 12 * S, Top + 145 * S, White, LabelSize);
	Meter(Rect(LX + 112 * S, Top + 152 * S, 130 * S, 6 * S), float(G->Stats.Reels - G->Retakes) / FMath::Max(1, G->Stats.Reels), Yellow, FMath::Clamp(G->Stats.Reels, 1, 10));

	for (int32 I = FMath::Max(0, G->Popups.Num() - 3); I < G->Popups.Num(); ++I)
	{
		const FFTPopup& Popup = G->Popups[I];
		const float Opacity = bReducedMotion ? 1.f : FMath::Clamp((1.6f - Popup.T) * 3.f, 0.f, 1.f);
		const float Py = Top + (182 + (G->Popups.Num() - 1 - I) * 32) * S;
		const bool bRepair = Popup.Text.StartsWith(TEXT("+")) && Popup.Text.EndsWith(TEXT(" ARMOR"));
		const bool bMissile = Popup.Text.StartsWith(TEXT("+")) && Popup.Text.EndsWith(TEXT(" MISSILES"));
		const FLinearColor Color = Alpha(bRepair ? Green : bMissile ? Amber : Popup.Col, Opacity);
		Box(LX, Py, LedgerW, 27 * S, Alpha(Ink, Opacity));
		Box(LX, Py, 3 * S, 27 * S, Color);
		if (bRepair || bMissile) Icon(bRepair ? EIcon::Repair : EIcon::Missile, LX + 10 * S, Py + 4 * S, 17 * S, Color);
		Print(Popup.Text, LX + (bRepair || bMissile ? 35 : 12) * S, Py + 4 * S, Color, Body, 0, false, 207 * S);
	}
	if (G->bShowKeys && GI)
	{
		const FString Keys = FString::Printf(TEXT("DRIFT %s   /   TURBO %s   /   FIRE %s   /   MISSILE %s   /   AMMO %s   /   %s PAUSE"),
			*Hint(EFTAction::Drift, TEXT("A/RB")), *Hint(EFTAction::Turbo, TEXT("LB/RT")), *Hint(EFTAction::Fire, TEXT("X")),
			*Hint(EFTAction::Missile, TEXT("B")), *Hint(EFTAction::Ammo, TEXT("Y")), bPad ? TEXT("START") : TEXT("P"));
		Box(Center - 306 * S, H - 32 * S, 612 * S, 22 * S, Ink);
		Print(Keys, Center, H - 31 * S, Grey, LabelSize, 1, false, 588 * S);
	}
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
	Box(0, 0, W, H, Alpha(Deep, FMath::Lerp(.68f, .96f, FMath::Clamp((Time - 2.9f) / .3f, 0.f, 1.f))));
	Plate(Rect(X, Y, 640 * S, 310 * S), Cyan, Ink);
	const float Closed = bReducedMotion ? 1.f : EaseOut(Time - ClapperClose, ClapperTravel);
	const float StickY = Y - (13.f + (1.f - Closed) * 22.f) * S;
	Box(X, StickY, 640 * S, 28 * S, Steel);
	for (int32 I = 0; I < 10; ++I)
	{
		const float Px = X + (I * 64 + 7) * S;
		Stroke(FVector2D(Px, StickY + 23 * S), FVector2D(Px + 27 * S, StickY + 4 * S), I < 2 ? Cyan : White, 6.f);
	}
	AccentSweep(Rect(X + 1 * S, Y + 17 * S, 638 * S, 4 * S), Time - ClapperClose - ClapperTravel, Cyan, FocusSweep);
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
	if (Time > 1.2f) Print(TEXT("ENTER / A   SKIP SLATE"), X + 615 * S, Y + 266 * S, Grey, Body, 2);
}

void AFTHUD::DrawCut(AFTGameMode* G)
{
	const float Age = G->RealClockNow() - G->CutT;
	if (Age < 0.f) return;
	const float Mid = W * .5f, Y = H * .32f;
	const FBox2D Card = Rect(Mid - 330 * S, Y, 660 * S, 112 * S);
	Plate(Card, Red, Ink);
	const float Wipe = bReducedMotion ? 1.f : EaseOut(Age);
	Box(Card.Min.X, Card.Max.Y - 4 * S, (Card.GetSize().X - Corner * S) * Wipe, 3 * S, Red);
	AccentSweep(Rect(Card.Min.X + 12 * S, Y + 3 * S, Card.GetSize().X - 24 * S, 4 * S), Age, Red, MenuEntrance);
	Print(G->CutTitle, Mid, Y + (9.f + 6.f * (1.f - Wipe)) * S, Red, Readout, 1, true);
	Print(G->CutSub, Mid, Y + 64 * S, White, Strong, 1, false, 616 * S);
}

void AFTHUD::DrawReel(AFTGameMode* G)
{
	const float Mid = W * .5f, X = Left + Safe * S, R = Right - Safe * S;
	const float Bar = FMath::Max((H - W / 2.39f) * .5f, 88 * S);
	const float Entry = MenuAge(EMenuView::Reel);
	const float Matte = Entry < 0.f ? Bar : Bar - 14 * S * (1.f - EaseOut(Entry, MenuEntrance));
	Box(0, 0, W, Matte, Deep);
	Box(0, H - Matte, W, Matte, Deep);
	Box(0, Matte - S, W, S, Rule);
	Box(0, H - Matte, W, S, Rule);
	const float ShotAge = MenuAge(EMenuView::Reel, true);
	if (!G->bReelDone)
		AccentSweep(Rect(R - 318 * S, Bar - 6 * S, 318 * S, 4 * S), ShotAge, Pink, FocusSweep);
	const FFTLevelDef& Level = G->Def();
	if (G->ReelTitleT > 0.f)
	{
		Box(0, Bar, W, H - 2 * Bar, Alpha(Deep, .88f));
		const FBox2D Card = Rect(Mid - 335 * S, H * .34f, 670 * S, 158 * S);
		Plate(Card, Pink, Ink);
		AccentSweep(Rect(Card.Min.X + 12 * S, Card.Max.Y - 8 * S, Card.GetSize().X - 24 * S, 4 * S),
			Entry, Pink, MenuEntrance);
		Print(TEXT("ROADBURN PICTURES"), Mid, Card.Min.Y + 16 * S, Pink, LabelSize, 1, true);
		Print(TEXT("THE HIGHLIGHTS"), Mid, Card.Min.Y + 43 * S, White, Readout, 1, true, 630 * S);
		Print(Level.Zone + TEXT("  /  ") + Level.Title, Mid, Card.Min.Y + 104 * S, Grey, Strong, 1, false, 630 * S);
	}
	Icon(EIcon::Play, X, Bar * .5f - 10 * S, 22 * S, Pink);
	Print(TEXT("DAILIES / HIGHLIGHTS"), X + 35 * S, Bar * .5f - 11 * S, White, Strong, 0, true);
	Print(FString::Printf(TEXT("SHOT %02d / %02d  %s"), G->ReelShot, G->ReelShots, *G->ReelCamName),
		R, Bar * .5f - 10 * S, Grey, Body, 2, false, 318 * S);
	Print(G->ReelLabel, X, H - Bar * .5f - 10 * S, Yellow, Strong, 0, true, 332 * S);
	if (G->bReelSlow) Print(TEXT("SLOW MOTION"), R, H - Bar * .5f - 8 * S, Pink, Body, 2, true);
	if (G->RealClockNow() - G->ReelLoopT < 2.f) Print(TEXT("AGAIN, FROM THE TOP"), Mid, Bar + 18 * S, White, Strong, 1, true);
	DrawReelContinue(G);
}

void AFTHUD::DrawReelContinue(AFTGameMode* G)
{
	const float Bar = FMath::Max((H - W / 2.39f) * .5f, 88 * S);
	const FBox2D Action = Rect(W * .5f - 182 * S, H - Bar * .5f - 27 * S, 364 * S, 54 * S);
	const bool bArmed = G->PhaseT > AFTGameMode::ReelArmT;
	const EControl State = G->ReelHoldT > 0.f ? EControl::Pressed : ControlState(Action, true, bArmed || G->bReelDone);
	G->ReelContinueRect = Control(Action, G->bReelDone ? TEXT("CONTINUE") : TEXT("HOLD TO SKIP"),
		G->bReelDone ? TEXT("ENTER / A / CLICK") : TEXT("HOLD ENTER / A / CLICK"), State, Cyan);
	if (G->bReelDone) FocusAccent(Action, MenuAge(EMenuView::Reel, true), Cyan);
	if (!G->bReelDone)
		Meter(Rect(Action.Min.X, Action.Max.Y + 5 * S, Action.GetSize().X, 4 * S),
			G->ReelHoldT / FMath::Max(.01f, FFTProgressData::Get().T(TEXT("HoldToSkip"), 1.f)), Cyan);
}

void AFTHUD::DrawRating(AFTGameMode* G)
{
	Box(0, 0, W, H, Alpha(Deep, .83f));
	const float X = Left + 184 * S, Y = (H - CanvasHeight * S) * .5f + 97 * S, R = Right - 184 * S;
	const FFTLevelDef& Level = G->Def();
	Print(TEXT("THE DAILIES"), X, Y - 57 * S, White, Heading, 0, true);
	Print(Level.Zone + TEXT("  /  ") + Level.Title, R, Y - 47 * S, Grey, Strong, 2, false, 500 * S);
	Plate(Rect(X, Y, 442 * S, 464 * S), Cyan, Ink);
	Plate(Rect(R - 442 * S, Y, 442 * S, 464 * S), Pink, Ink);
	AccentSweep(Rect(X + 1 * S, Y + S, 440 * S, 4 * S), G->PhaseT, Cyan, MenuEntrance);
	AccentSweep(Rect(R - 441 * S, Y + S, 440 * S, 4 * S), G->PhaseT - .075f, Pink, MenuEntrance);
	Print(TEXT("BOX OFFICE BREAKDOWN"), X + 21 * S, Y + 21 * S, Grey, LabelSize, 0, true);
	struct FRow { const TCHAR* Name; int64 Value; };
	const FRow Rows[] = { { TEXT("DESTRUCTION"), G->ScoreDestruction }, { TEXT("NEAR MISSES"), G->ScoreNear },
		{ TEXT("STUNTS"), G->ScoreStunts }, { TEXT("ARMOR BONUS"), G->ArmorBonus }, { TEXT("COLLATERAL"), G->ScoreCollateral } };
	for (int32 I = 0; I < UE_ARRAY_COUNT(Rows); ++I)
	{
		const float Ry = Y + (67 + I * 51) * S;
		Box(X + 20 * S, Ry, 402 * S, 40 * S, Steel);
		Print(Rows[I].Name, X + 31 * S, Ry + 8 * S, White, Body);
		const float Age = G->PhaseT - (.3f + .3f * I);
		AccentSweep(Rect(X + 25 * S, Ry + 35 * S, 392 * S, 3 * S), Age, Cyan, FocusSweep);
		if (bReducedMotion || Age >= 0.f) Print(Money(Rows[I].Value), X + 408 * S, Ry + 6 * S,
			Rows[I].Value < 0 ? Red : Cyan, Strong, 2, true, 179 * S);
	}
	Print(TEXT("BOX OFFICE"), X + 22 * S, Y + 347 * S, Yellow, LabelSize, 0, true);
	if (G->PhaseT > 1.9f || bReducedMotion) Print(Money(G->Score), X + 22 * S, Y + 373 * S, Yellow, Readout, 0, true, 399 * S);

	const float RatingX = R - 420 * S;
	Print(TEXT("DIRECTOR'S RATING"), RatingX, Y + 21 * S, Grey, LabelSize, 0, true);
	const int32 Rating = FMath::Clamp(G->RatingIdx, 0, 3);
	if (G->PhaseT >= 2.7f || bReducedMotion)
	{
		static const TCHAR* Names[] = { TEXT("B-MOVIE"), TEXT("ACTION HERO"), TEXT("BLOCKBUSTER"), TEXT("LEGENDARY DIRECTOR") };
		static const FLinearColor Colors[] = { White, Cyan, Pink, Yellow };
		const FLinearColor Color = Colors[Rating];
		const float In = bReducedMotion ? 1.f : EaseOut(G->PhaseT - 2.7f);
		const float StampY = Y + (118.f - 9.f * (1.f - In)) * S;
		Print(Names[Rating], RatingX, StampY, Alpha(Color, .72f + .28f * In), Heading, 0, true, 398 * S);
		Meter(Rect(RatingX, Y + 182 * S, 397 * S, 12 * S), (Rating + 1) / 4.f, Color, 4);
		Box(RatingX, Y + 209 * S, 396 * S * In, S, Color);
		Print(G->RatingQuip, RatingX, Y + 233 * S, White, Strong, 0, false, 398 * S);
	}
	if (G->PhaseT > 3.f && (G->bOutOfFilm || G->bEscaped))
	{
		const bool bFilm = G->bOutOfFilm;
		Print(bFilm ? TEXT("OUT OF FILM") : TEXT("TARGET ESCAPED"), RatingX, Y + 309 * S, bFilm ? Red : Amber, Strong, 0, true);
		Print(bFilm ? TEXT("Kill fee only. The level is reshot.") : TEXT("Rated at most Action Hero. The level is reshot."),
			RatingX, Y + 340 * S, Grey, Body, 0, false, 398 * S);
	}
	const FBox2D Continue = Rect(R - 442 * S, Y + 481 * S, 442 * S, 55 * S);
	G->ResultsContinueRect = Control(Continue, TEXT("CONTINUE TO PAYCHECK"), TEXT("ENTER / A"),
		ControlState(Continue, true, G->PhaseT > 2.9f), Cyan);
	Print(TEXT("R   RESHOOT     ESC / B   TITLE"), X + 2 * S, Y + 499 * S, Grey, Body);
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
	const float DepartmentAge = MenuAge(EMenuView::Department);
	const float SelectionAge = MenuAge(EMenuView::Department, true);

	Box(0, 0, W, H, Alpha(Deep, .88f));
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
	Print(FString::Printf(TEXT("%d / %d"), Career->SPLeft(), Career->SPEarned()), Rx + 181 * S, Y + 57 * S, Cyan, Strong, 0, true, 140 * S);
	Print(TEXT("CASH"), R - 16 * S, Y + 34 * S, Grey, LabelSize, 2, true);
	Print(Money(Career->C.Cash), R - 16 * S, Y + 55 * S, Yellow, Strong, 2, true, 244 * S);

	static const TCHAR* Tabs[] = { TEXT("SKILLS"), TEXT("GARAGE"), TEXT("CUSTOM"), TEXT("SFX DEPT"), TEXT("EXTRAS") };
	G->TabRects.SetNum(UE_ARRAY_COUNT(Tabs));
	for (int32 I = 0; I < UE_ARRAY_COUNT(Tabs); ++I)
	{
		const FBox2D TabRect = Rect(X + I * 178 * S, TabY, 170 * S, 44 * S);
		const bool bCurrent = I == static_cast<int32>(G->Tab);
		G->TabRects[I] = Control(TabRect, Tabs[I], FString(), ControlState(TabRect, bCurrent), Cyan);
		if (bCurrent) FocusAccent(TabRect, DepartmentAge, Cyan);
	}
	Print(TEXT("TAB / Q E / LB RB   SWITCH DEPARTMENT"), R, TabY + 13 * S, Grey, LabelSize, 2, false, 307 * S);

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
	Plate(Rect(X, BoardY - 32 * S, BoardW, Bottom - BoardY + 32 * S), Cyan, Ink);
	Plate(Rect(DetailX, BoardY - 32 * S, R - DetailX, Bottom - BoardY + 32 * S), Pink, Ink);
	AccentSweep(Rect(X + 2 * S, BoardY - 30 * S, BoardW - 4 * S, 4 * S), DepartmentAge, Cyan, MenuEntrance);
	AccentSweep(Rect(DetailX + 2 * S, BoardY - 30 * S, R - DetailX - 4 * S, 4 * S),
		DepartmentAge - .06f, Pink, MenuEntrance);
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
		Box(Tx, Ty, Size.X, Size.Y, Interaction == EControl::Pressed ? Deep : bSelected ? Active : Steel);
		Box(Tx, Ty, (bSelected ? 4 : 2) * S, Size.Y, bSelected ? Cyan : State);
		if (bSelected || bMouse) Frame(Tx, Ty, Size.X, Size.Y, bMouse ? White : Cyan, 1.f);
		if (bSelected) FocusAccent(TileRect, SelectionAge, Cyan);
		if (Tile.bSwatch) Box(TileRect.Max.X - 15 * S, Ty + 8 * S, 8 * S, 8 * S, Tile.Color);
		const float AvailableWidth = Size.X - (Tile.bSwatch ? 27 : 17) * S;
		Print(Tile.Title, Tx + 9 * S, Ty + 3 * S, Tile.State == EFTTile::Locked || Tile.State == EFTTile::Soon ? Grey : White,
			bGarage ? LabelSize : Body, 0, false, AvailableWidth);
		if (!bGarage) Print(Tile.Sub, Tx + 9 * S, Ty + 23 * S, Grey, LabelSize, 0, false, Size.X - 18 * S);
		const bool bMaxed = bGarage && Tile.Col == Cols - 1 && Tile.State == EFTTile::Owned;
		Print(bMaxed ? TEXT("MAX RANK") : States[static_cast<int32>(Tile.State)], Tx + 9 * S, TileRect.Max.Y - 17 * S,
			State, 13.f, 0, true, Size.X - 18 * S);
		G->TileRects[I] = TileRect;
	}

	G->BuyRect = FBox2D(ForceInit);
	G->UndoRect = FBox2D(ForceInit);
	if (G->Tiles.IsValidIndex(Selection))
	{
		const FFTTileView& Tile = G->Tiles[Selection];
		const float Dx = DetailX + 17 * S, Dw = R - Dx - 17 * S;
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
			Box(Dx, Dy + 2 * S, Dw, 14 * S, Tile.Color);
			Dy += 24 * S;
		}
		const bool bCanAfford = Tile.State != EFTTile::Available || !Tile.bCash || Tile.Cost <= Career->C.Cash;
		Print(States[static_cast<int32>(Tile.State)], Dx, Dy + 4 * S, StateColor(Tile.State), LabelSize, 0, true, Dw * .52f);
		Print(Tile.Price, R - 17 * S, Dy, bCanAfford ? Yellow : Red, Strong, 2, false, Dw * .47f);
		Dy += 31 * S;
		Box(Dx, Dy, Dw, S, Rule);
		AccentSweep(Rect(Dx, Dy, Dw, 3 * S), SelectionAge - .035f, Pink, FocusSweep);
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
		const float ActionY = Bottom - (bSkills ? 100 : 67) * S;
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

		FString Action = TEXT("BUY"), Hint = Tile.bCash ? TEXT("HOLD ENTER / A / MOUSE") : TEXT("ENTER / A / CLICK");
		const bool bEquipped = bShop && Tile.State == EFTTile::Equipped;
		const bool bRequiredSlot = Tile.Slot == TEXT("paint") || Tile.Slot == TEXT("rims") || Tile.Slot == TEXT("neon") || Tile.Slot == TEXT("nitro");
		if (bShop && Tile.State == EFTTile::Owned) { Action = TEXT("EQUIP"); Hint = TEXT("ENTER / A / CLICK"); }
		else if (bEquipped) { Action = bRequiredSlot ? TEXT("EQUIPPED") : TEXT("UNEQUIP"); Hint = bRequiredSlot ? TEXT("REQUIRED SLOT") : TEXT("ENTER / A / CLICK"); }
		else if (Tile.Id == TEXT("sequel") && Tile.State == EFTTile::Owned) { Action = TEXT("SWITCH"); Hint = TEXT("ENTER / A / CLICK"); }
		else if (Tile.State != EFTTile::Available) { Action = States[static_cast<int32>(Tile.State)]; Hint = Tile.State == EFTTile::Pending ? TEXT("KEPT ON READY") : FString(); }
		const bool bEnabled = (Tile.State == EFTTile::Available && bCanAfford)
			|| (Tile.State == EFTTile::Owned && (bShop || Tile.Id == TEXT("sequel"))) || (bEquipped && !bRequiredSlot);
		const FBox2D Buy = Rect(Dx, ActionY, Dw, 48 * S);
		G->BuyRect = Control(Buy, Action, Hint, G->BuyHoldT > 0.f && bEnabled ? EControl::Pressed : ControlState(Buy, bEnabled, bEnabled), Cyan);
		if (Tile.bCash && Tile.State == EFTTile::Available)
			Meter(Rect(Dx, Buy.Max.Y + 3 * S, Dw, 3 * S), G->BuyHoldT / FMath::Max(.01f, Data.T(TEXT("HoldToBuy"), .5f)), Yellow);
		if (bSkills)
		{
			const FBox2D Undo = Rect(Dx, ActionY + 59 * S, Dw, 31 * S);
			G->UndoRect = Control(Undo, TEXT("UNDO  /  BACKSPACE OR B"), FString(), ControlState(Undo, false, !Career->C.PendingSkills.IsEmpty()), Yellow);
		}
	}
	else Print(TEXT("NO ITEMS AVAILABLE"), DetailX + 18 * S, BoardY + 20 * S, Grey, Strong, 0, true);

	const FBox2D Ready = Rect(R - 344 * S, Bottom + 13 * S, 344 * S, 54 * S);
	const FString ReadyLabel = G->bPPFromTitle || G->PPNextLevel < 0 ? TEXT("READY / TITLE") : FString::Printf(TEXT("READY / LEVEL %d"), G->PPNextLevel + 1);
	G->ReadyRect = Control(Ready, ReadyLabel, TEXT("SPACE / START / CLICK"), ControlState(Ready, false, G->PhaseT >= .3f), Cyan);
	const bool bFeedback = T - G->PPMsgT < 2.2f && !G->PPMsg.IsEmpty();
	if (bFeedback)
	{
		if (!bSkills)
			AccentSweep(Rect(X + 2 * S, Bottom + 39 * S, 336 * S, 3 * S),
				T - G->PPMsgT, G->bPPMsgBad ? Red : Green, Feedback);
		Print(G->PPMsg.Replace(TEXT("·"), TEXT("/")), X + 2 * S, Bottom + 14 * S, G->bPPMsgBad ? Red : Green, Strong, 0, true, 836 * S);
	}
	else if (bSkills && !Career->C.bTipSeen && Career->SPLeft() > 0)
		Print(TEXT("Pick an available skill to spend a point. READY keeps pending changes."), X + 2 * S, Bottom + 16 * S, Yellow, Body, 0, false, 836 * S);
	Print(TEXT("ARROWS / D-PAD   MOVE     ENTER / A   CONFIRM     Q E / LB RB   CHANGE TAB"),
		X + 2 * S, Bottom + 48 * S, Grey, Body, 0, false, 836 * S);
}

void AFTHUD::DrawPaycheck(AFTGameMode* G)
{
	Box(0, 0, W, H, Alpha(Deep, .86f));
	const float X = Left + 71 * S, R = Right - 71 * S, Y = (H - CanvasHeight * S) * .5f;
	const FFTLevelDef& Level = G->Def();
	Print(TEXT("PAYCHECK"), X, Y + 32 * S, White, Heading, 0, true);
	Print(Level.Zone + TEXT("  /  ") + Level.Title, R, Y + 42 * S, Grey, Strong, 2, false, 526 * S);
	const FBox2D Cash = Rect(X, Y + 91 * S, 543 * S, 512 * S);
	const FBox2D Career = Rect(R - 573 * S, Y + 91 * S, 573 * S, 512 * S);
	Plate(Cash, Yellow, Ink);
	Plate(Career, Cyan, Ink);
	AccentSweep(Rect(Cash.Min.X + S, Cash.Min.Y + S, Cash.GetSize().X - 2 * S, 4 * S), G->PhaseT, Yellow, MenuEntrance);
	AccentSweep(Rect(Career.Min.X + S, Career.Min.Y + S, Career.GetSize().X - 2 * S, 4 * S),
		G->PhaseT - .075f, Cyan, MenuEntrance);
	Print(TEXT("CASH STATEMENT"), Cash.Min.X + 18 * S, Cash.Min.Y + 17 * S, Yellow, LabelSize, 0, true);
	Print(TEXT("CAREER PROGRESSION"), Career.Min.X + 18 * S, Career.Min.Y + 17 * S, Cyan, LabelSize, 0, true);
	if (G->bPayValid)
	{
		const FFTCommitOut& Pay = G->Pay;
		const int32 Shown = bReducedMotion ? Pay.Pay.Lines.Num()
			: FMath::Clamp(FMath::FloorToInt((G->PhaseT - .3f) / .32f), 0, Pay.Pay.Lines.Num());
		const float RowH = FMath::Min(30.f, 343.f / FMath::Max(1, Pay.Pay.Lines.Num())) * S;
		for (int32 I = 0; I < Shown; ++I)
		{
			const FFTPayLine& Line = Pay.Pay.Lines[I];
			const int64 Value = static_cast<int64>(FMath::RoundToDouble(Line.Amount / 10.0) * 10.0);
			const float Ry = Cash.Min.Y + 50 * S + I * RowH;
			if (I % 2 == 0) Box(Cash.Min.X + 16 * S, Ry, Cash.GetSize().X - 32 * S, RowH - 2 * S, Steel);
			AccentSweep(Rect(Cash.Min.X + 16 * S, Ry + RowH - 3 * S, Cash.GetSize().X - 32 * S, 2 * S),
				G->PhaseT - (.3f + (I + 1) * .32f), Yellow, FocusSweep);
			Print(Line.Label, Cash.Min.X + 25 * S, Ry + 3 * S, Line.bMult ? Cyan : White, Body, 0, false, (Line.bMult ? 260 : 310) * S);
			if (Line.bMult)
				Print(FString::Printf(TEXT("× %.3g"), Line.Mult), Cash.Max.X - 144 * S, Ry + 4 * S, Pink, LabelSize, 2, false, 85 * S);
			Print(Money(Value), Cash.Max.X - 23 * S, Ry + 3 * S, Value < 0 ? Red : Yellow, Body, 2, false, 119 * S);
		}
		Box(Cash.Min.X + 18 * S, Cash.Max.Y - 98 * S, Cash.GetSize().X - 36 * S, S, Rule);
		Print(TEXT("TOTAL"), Cash.Min.X + 21 * S, Cash.Max.Y - 79 * S, Yellow, LabelSize, 0, true);
		if (Shown >= Pay.Pay.Lines.Num())
		{
			AccentSweep(Rect(Cash.Min.X + 18 * S, Cash.Max.Y - 99 * S, Cash.GetSize().X - 36 * S, 4 * S),
				G->PhaseT - (.3f + Pay.Pay.Lines.Num() * .32f), Yellow, MenuEntrance);
			Print(Money(Pay.Pay.Total), Cash.Max.X - 22 * S, Cash.Max.Y - 86 * S, Yellow, Heading, 2, true, 375 * S);
			if (const UFTCareerSubsystem* CS = UFTCareerSubsystem::Get(G))
				Print(FString::Printf(TEXT("CASH NOW  %s"), *Money(CS->C.Cash)), Cash.Max.X - 22 * S, Cash.Max.Y - 38 * S, Grey, Body, 2, false, 490 * S);
		}
		const float XPStart = .6f + Pay.Pay.Lines.Num() * .32f;
		if (G->PhaseT > XPStart || bReducedMotion)
		{
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
			if (Leveled)
			{
				Box(Career.Min.X + 18 * S, RewardY, Career.GetSize().X - 36 * S, 35 * S, Active);
				Print(FString::Printf(TEXT("LEVEL UP   /   +%d SKILL POINT%s"), SkillPoints, SkillPoints == 1 ? TEXT("") : TEXT("S")),
					Career.Min.X + 27 * S, RewardY + 6 * S, Yellow, Strong, 0, true, 520 * S);
				RewardY += 43 * S;
			}
			if (U >= 1.f)
			{
				if (Pay.LegendarySP > 0)
				{
					Print(FString::Printf(TEXT("FIRST LEGENDARY DIRECTOR HERE  /  +%d SP"), Pay.LegendarySP),
						Career.Min.X + 20 * S, RewardY, Yellow, LabelSize, 0, true, 530 * S);
					RewardY += 23 * S;
				}
				for (int32 I = 0; I < Pay.Unlocks.Num() && RewardY < Career.Max.Y - 24 * S; ++I)
				{
					const bool bLastRow = RewardY + 42 * S >= Career.Max.Y && I + 1 < Pay.Unlocks.Num();
					Print(bLastRow ? FString::Printf(TEXT("+%d MORE UNLOCKS IN PRE-PRODUCTION"), Pay.Unlocks.Num() - I) : Pay.Unlocks[I],
						Career.Min.X + 20 * S, RewardY, Green, Body, 0, false, 530 * S);
					RewardY += 21 * S;
				}
			}
		}
	}
	else Print(TEXT("NO PAYCHECK FOR A BENCHMARK RUN"), Cash.Min.X + 20 * S, Cash.Min.Y + 67 * S, Grey, Body);

	const FBox2D Continue = Rect(R - 447 * S, Y + 622 * S, 447 * S, 54 * S);
	G->ResultsContinueRect = Control(Continue, TEXT("CONTINUE"), TEXT("ENTER / A"),
		ControlState(Continue, true, G->PhaseT >= .6f), Cyan);
	Print(TEXT("R   RESHOOT     ESC / B   TITLE"), X, Y + 638 * S, Grey, Body);
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
	const float Entry = MenuAge(EMenuView::Pause);
	const float In = Entry < 0.f ? 1.f : EaseOut(Entry, MenuEntrance);
	Box(0, 0, W, H, Alpha(Deep, FMath::Lerp(.48f, .68f, In)));
	const float X = W * .5f - 251 * S, Y = (H - CanvasHeight * S) * .5f + 121 * S, WidthPx = 502 * S;
	Plate(Rect(X, Y, WidthPx, 468 * S), Yellow, Ink);
	AccentSweep(Rect(X + 2 * S, Y + 2 * S, WidthPx - 4 * S, 4 * S), Entry, Yellow, MenuEntrance);
	Print(TEXT("CAMERAS HOLDING"), X + (24.f + 9.f * (1.f - In)) * S, Y + 23 * S, White, Heading, 0, true);
	Print(FString::Printf(TEXT("SCENE %02d   /   TAKE %02d"), G->Scene + 1, G->Take), X + 24 * S, Y + 67 * S, Grey, Body);
	Print(TEXT("PAUSED"), X + WidthPx - 24 * S, Y + 30 * S, Yellow, LabelSize, 2, true);
	static const TCHAR* Names[] = { TEXT("RESUME"), TEXT("CONTROLS"), TEXT("OPTIONS"), TEXT("RESHOOT LEVEL"), TEXT("TITLE") };
	static const TCHAR* Hints[] = { TEXT("P / ESC / START"), TEXT("C / Y"), TEXT("O"), TEXT("R"), TEXT("T") };
	for (int32 I = 0; I < UE_ARRAY_COUNT(Names); ++I)
	{
		const FBox2D Row = Rect(X + 24 * S, Y + (108 + 60 * I) * S, WidthPx - 48 * S, 51 * S);
		const FLinearColor& Accent = I == 0 ? Cyan : Pink;
		const bool bSelected = G->PauseRow == I;
		G->OverlayRects.Add(Control(Row, Names[I], Hints[I], ControlState(Row, bSelected), Accent));
		if (bSelected) FocusAccent(Row, MenuAge(EMenuView::Pause, true), Accent);
	}
	Print(TEXT("UP-DOWN / D-PAD   NAVIGATE     ENTER / A   SELECT     M   MUTE     F1   FPS"),
		X + 24 * S, Y + 420 * S, Grey, Body, 0, false, WidthPx - 48 * S);
}

void AFTHUD::DrawControls(AFTGameMode* G)
{
	const UFTGameInstance* GI = G->GI();
	if (!GI) return;
	Box(0, 0, W, H, Alpha(Deep, .86f));
	const float X = W * .5f - 450 * S, Y = (H - CanvasHeight * S) * .5f + 30 * S, WidthPx = 900 * S;
	Plate(Rect(X, Y, WidthPx, 660 * S), Cyan, Ink);
	const float Entry = MenuAge(EMenuView::Controls);
	const float In = Entry < 0.f ? 1.f : EaseOut(Entry, MenuEntrance);
	AccentSweep(Rect(X + 2 * S, Y + 2 * S, WidthPx - 4 * S, 4 * S), Entry, Cyan, MenuEntrance);
	Print(TEXT("CONTROLS"), X + (24.f + 9.f * (1.f - In)) * S, Y + 18 * S, White, Heading, 0, true);
	Print(TEXT("Active layout switches when you steer with the other set of bindings."),
		X + 25 * S, Y + 62 * S, Grey, Body, 0, false, 690 * S);
	const FBox2D Back = Rect(X + 715 * S, Y + 18 * S, 161 * S, 46 * S);
	G->OverlayRects.Add(Control(Back, G->bControlsWaiting ? TEXT("CANCEL") : TEXT("BACK"), TEXT("ESC / B"),
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
		for (int32 Layout = 0; Layout < 2; ++Layout)
		{
			const FBox2D Key = Rect(KeysX + Layout * KeyPitch, RowY, 248 * S, 32 * S);
			const bool bSelected = G->ControlsRow == Action && G->ControlsCol == Layout;
			const bool bCapture = bSelected && G->bControlsWaiting;
			const FKey& Bind = GI->Binds[Action][Layout];
			FString Text = Bind.IsValid() ? Bind.GetDisplayName().ToString().ToUpper() : FString(TEXT("UNBOUND"));
			if (bCapture) Text = TEXT("PRESS A KEY");
			G->OverlayRects.Add(Control(Key, Text, FString(),
				ControlState(Key, bSelected, !G->bControlsWaiting || bCapture), bCapture ? Yellow : Cyan));
			if (bSelected) FocusAccent(Key, MenuAge(EMenuView::Controls, true), bCapture ? Yellow : Cyan);
		}
	}
	const FBox2D Reset = Rect(X + 25 * S, Y + 454 * S, 279 * S, 44 * S);
	G->OverlayRects.Add(Control(Reset, TEXT("RESET TO DEFAULTS"), FString(),
		ControlState(Reset, G->ControlsRow == UFTGameInstance::NumActions, !G->bControlsWaiting), Yellow));
	if (G->ControlsRow == UFTGameInstance::NumActions)
		FocusAccent(Reset, MenuAge(EMenuView::Controls, true), Yellow);
	if (G->bControlsWaiting)
		Print(TEXT("ESC CANCELS   /   DELETE OR BACKSPACE CLEARS"), X + 326 * S, Y + 462 * S, Yellow, Body);
	else if (G->RealClockNow() - G->OverlayFeedbackT < 2.7f)
		Print(G->OverlayFeedback, X + 326 * S, Y + 461 * S, G->bOverlayFeedbackError ? Red : Cyan, Body, 0, false, 542 * S);
	Print(TEXT("UP-DOWN / D-PAD   ACTION     LEFT-RIGHT   LAYOUT     ENTER / A   REBIND"), X + 25 * S, Y + 522 * S, White, Body);
	Print(TEXT("MOUSE   LEFT: GUNS   /   RIGHT: MISSILE   /   MIDDLE: AMMO"), X + 25 * S, Y + 555 * S, Grey, Body);
	Print(TEXT("GAMEPAD   STICK: STEER   /   A OR RB: DRIFT   /   LB OR RT: TURBO   /   X: GUNS   /   B: MISSILE   /   Y: AMMO"),
		X + 25 * S, Y + 581 * S, Grey, Body, 0, false, 850 * S);
	Print(TEXT("Reserved keys and fixed mouse and gamepad actions cannot be rebound."), X + 25 * S, Y + 617 * S, Grey, LabelSize);
}

void AFTHUD::DrawOptions(AFTGameMode* G)
{
	const UFTGameInstance* GI = G->GI();
	if (!GI) return;
	Box(0, 0, W, H, Alpha(Deep, .82f));
	const float X = W * .5f - 365 * S, Y = (H - CanvasHeight * S) * .5f + 83 * S, WidthPx = 730 * S;
	Plate(Rect(X, Y, WidthPx, 554 * S), Cyan, Ink);
	Print(TEXT("OPTIONS"), X + 25 * S, Y + 21 * S, White, Heading, 0, true);
	const FBox2D Back = Rect(X + 545 * S, Y + 20 * S, 161 * S, 48 * S);
	G->OverlayRects.Add(Control(Back, TEXT("BACK"), TEXT("ESC / B"), ControlState(Back), Cyan));

	static const TCHAR* Labels[] = { TEXT("GRAPHICS PRESET"), TEXT("SHOW FRAME RATE"), TEXT("VSYNC"), TEXT("REDUCED MOTION") };
	static const TCHAR* Groups[] = { TEXT("DISPLAY"), TEXT("ACCESSIBILITY") };
	const FString Values[] = { UFTGameInstance::QualityName(GI->Quality), GI->bShowFps ? TEXT("ON") : TEXT("OFF"),
		GI->bVSync ? TEXT("ON") : TEXT("OFF"), GI->bReducedMotion ? TEXT("ON") : TEXT("OFF") };
	for (int32 I = 0; I < UE_ARRAY_COUNT(Labels); ++I)
	{
		const float RowY = Y + (112 + I * 53 + (I == 3 ? 38 : 0)) * S;
		const bool bSelected = G->OptionsRow == I;
		if (I == 0 || I == 3)
			Print(Groups[I == 0 ? 0 : 1], X + 25 * S, RowY - 24 * S, Grey, LabelSize, 0, true);
		const FBox2D Row = Rect(X + 24 * S, RowY, WidthPx - 48 * S, 45 * S);
		Box(Row.Min.X, Row.Min.Y, Row.GetSize().X, Row.GetSize().Y, bSelected ? Active : Steel);
		Frame(Row.Min.X, Row.Min.Y, Row.GetSize().X, Row.GetSize().Y, bSelected ? Cyan : Rule, bSelected ? 1.5f : 1.f);
		Print(Labels[I], Row.Min.X + 14 * S, RowY + 10 * S, White, Strong, 0, true);
		Print(Values[I], Row.Max.X - 117 * S, RowY + 9 * S, bSelected ? Cyan : White, Strong, 1, true, 122 * S);
		const FBox2D Dec = Rect(Row.Max.X - 215 * S, RowY + 5 * S, 31 * S, 35 * S);
		const FBox2D Inc = Rect(Row.Max.X - 50 * S, RowY + 5 * S, 31 * S, 35 * S);
		G->OverlayRects.Add(Control(Dec, TEXT("<"), FString(), ControlState(Dec), Cyan));
		G->OverlayRects.Add(Control(Inc, TEXT(">"), FString(), ControlState(Inc), Cyan));
	}
	Box(X + 24 * S, Y + 381 * S, WidthPx - 48 * S, S, Rule);
	static const TCHAR* Details[] = {
		TEXT("Low and Medium use TSR with Lumen off. High enables Lumen GI and reflections. Epic increases shadow and post quality. Auto chooses for this computer."),
		TEXT("Display the current frame rate and frame time in the corner."),
		TEXT("Synchronize frame presentation to the display refresh rate."),
		TEXT("Reduce decorative movement and flashes in the interface and game.")
	};
	float DetailY = Y + 398 * S;
	for (const FString& Line : WrapText(Details[FMath::Clamp(G->OptionsRow, 0, 3)], WidthPx - 48 * S, Body))
	{
		Print(Line, X + 24 * S, DetailY, Grey, Body, 0, false, WidthPx - 48 * S);
		DetailY += 22 * S;
	}
	Print(TEXT("CHANGES SAVE AUTOMATICALLY"), X + 24 * S, Y + 473 * S, Grey, LabelSize, 0, true);
	Print(TEXT("UP-DOWN / D-PAD   PICK     LEFT-RIGHT   CHANGE"), X + 24 * S, Y + 508 * S, White, Body);
}
