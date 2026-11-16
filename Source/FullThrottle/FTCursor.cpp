#include "FTCursor.h"
#include "FTHUDSkin.h"
#include "Engine/Canvas.h"
#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "Engine/Texture.h"
#include "GameFramework/PlayerController.h"
#include "GenericPlatform/ICursor.h"

namespace
{
	// The arrow in its own units (34 high), tip at the origin, leaning a little: head, then the tail.
	const FVector2D Arrow[] = {
		FVector2D(0.f, 0.f),      // 0 tip
		FVector2D(1.5f, 27.f),    // 1 the head's lower left corner
		FVector2D(8.f, 20.5f),    // 2 notch, left
		FVector2D(14.f, 33.f),    // 3 tail, lower left
		FVector2D(20.f, 30.f),    // 4 tail, lower right
		FVector2D(14.f, 18.f),    // 5 notch, right
		FVector2D(23.f, 17.f),    // 6 the head's right corner
	};
	const int32 ArrowTris[][3] = { { 0, 1, 2 }, { 0, 2, 5 }, { 0, 5, 6 }, { 2, 3, 4 }, { 2, 4, 5 } };
	// the cel shade: the right-hand side of the head and of the tail
	const FVector2D Shade[] = { FVector2D(0.f, 0.f), FVector2D(11.f, 19.f), FVector2D(14.f, 18.f), FVector2D(23.f, 17.f),
		FVector2D(11.f, 19.f), FVector2D(17.f, 31.5f), FVector2D(20.f, 30.f), FVector2D(14.f, 18.f) };
	const int32 ShadeTris[][3] = { { 0, 1, 2 }, { 0, 2, 3 }, { 4, 5, 6 }, { 4, 6, 7 } };

	struct FPen
	{
		UCanvas* Canvas; FVector2D Origin; float Scale;
		FVector2D P(const FVector2D& V) const { return Origin + V * Scale; }
		void Tri(const FVector2D& A, const FVector2D& B, const FVector2D& C, const FLinearColor& Color) const
		{
			FCanvasTriangleItem It(A, B, C, GWhiteTexture);
			It.SetColor(FLinearColor(Color.R, Color.G, Color.B, 1.f));
			It.BlendMode = SE_BLEND_Translucent;
			Canvas->DrawItem(It);
		}
		void Shape(const FVector2D* Pts, const int32 (*Tris)[3], int32 Num, const FVector2D& Off, const FLinearColor& Color) const
		{
			for (int32 I = 0; I < Num; ++I) Tri(P(Pts[Tris[I][0]]) + Off, P(Pts[Tris[I][1]]) + Off, P(Pts[Tris[I][2]]) + Off, Color);
		}
		/** The shape grown by Width on every side: stamped round a circle (flat colour, so the stamps do not show). */
		void Fat(const FVector2D* Pts, const int32 (*Tris)[3], int32 Num, const FVector2D& Off, float Width, const FLinearColor& Color) const
		{
			for (int32 K = 0; K < 12; ++K)
			{
				const float A = K * (2.f * PI / 12.f);
				Shape(Pts, Tris, Num, Off + FVector2D(FMath::Cos(A), FMath::Sin(A)) * Width, Color);
			}
			Shape(Pts, Tris, Num, Off, Color);
		}
		void Bar(const FVector2D& A, const FVector2D& B, float Width, const FLinearColor& Color) const
		{
			const FVector2D Along = (B - A).GetSafeNormal();
			const FVector2D Across = FVector2D(-Along.Y, Along.X) * (Width * .5f);
			Tri(A + Across, A - Across, B + Across, Color); Tri(A - Across, B - Across, B + Across, Color);
		}
		void Star(const FVector2D& C, float Outer, float Inner, int32 Points, float Turn, const FLinearColor& Color) const
		{
			FVector2D Prev = C + FVector2D(FMath::Cos(Turn), FMath::Sin(Turn)) * Outer;
			for (int32 K = 1; K <= Points * 2; ++K)
			{
				const float A = Turn + K * PI / Points;
				const FVector2D Next = C + FVector2D(FMath::Cos(A), FMath::Sin(A)) * (K % 2 ? Inner : Outer);
				Tri(C, Prev, Next, Color);
				Prev = Next;
			}
		}
	};
}

void FTCursor::Draw(UCanvas* Canvas, const FVector2D& At, float S, EState State, float Time, bool bStill)
{
	if (!Canvas || S <= 0.f) return;
	const bool bHover = State == EState::Hover, bDown = State == EState::Pressed;
	const float Outline = 2.6f * S;
	const FVector2D Drop = FVector2D(3.4f, 4.2f) * S;
	// pressed: it sinks onto its shadow (the buttons do the same) and is a touch smaller
	FPen Pen{ Canvas, At + (bDown ? Drop * .8f : FVector2D::ZeroVector), S * (bDown ? .94f : bHover ? 1.06f : 1.f) };
	const FLinearColor Fill = bDown ? FTSkin::Pink : bHover ? FTSkin::Amber : FTSkin::LitPaper;
	const FLinearColor Cel = bDown ? FLinearColor(.78f, .10f, .42f) : bHover ? FTSkin::AmberInk : FTSkin::Cyan;
	const FLinearColor Shadow = bHover ? FTSkin::Cyan : FTSkin::Pink;

	if (bDown)
	{
		// the impact star behind the tip: ink, amber, a lit centre
		const FVector2D C = At + FVector2D(1.f, 1.f) * S;
		Pen.Star(C, 17.f * S, 8.5f * S, 8, .2f, FTSkin::Ink);
		Pen.Star(C, 13.5f * S, 6.5f * S, 8, .2f, FTSkin::Amber);
		Pen.Star(C, 7.f * S, 3.4f * S, 8, .2f, FTSkin::LitPaper);
	}
	else
	{
		// the print shadow, hard and offset, with the outline's weight
		Pen.Fat(Arrow, ArrowTris, UE_ARRAY_COUNT(ArrowTris), Drop, Outline, Shadow);
	}
	Pen.Fat(Arrow, ArrowTris, UE_ARRAY_COUNT(ArrowTris), FVector2D::ZeroVector, Outline, FTSkin::Ink);
	Pen.Shape(Arrow, ArrowTris, UE_ARRAY_COUNT(ArrowTris), FVector2D::ZeroVector, Fill);
	Pen.Shape(Shade, ShadeTris, UE_ARRAY_COUNT(ShadeTris), FVector2D::ZeroVector, Cel);
	// the glint down the lit edge
	Pen.Bar(Pen.P(FVector2D(2.6f, 7.f)), Pen.P(FVector2D(3.3f, 19.f)), 1.7f * S, FLinearColor::White);

	if (bHover)
	{
		// three sparks off the tip, ink under amber; they breathe unless motion is reduced
		const float Beat = bStill ? .5f : .5f + .5f * FMath::Sin(Time * 9.f);
		const float Near = (7.5f + 1.5f * Beat) * S, Far = (13.5f + 3.f * Beat) * S;
		for (const float Deg : { 180.f, 225.f, 270.f })
		{
			const float A = FMath::DegreesToRadians(Deg);
			const FVector2D Dir(FMath::Cos(A), FMath::Sin(A));
			const FVector2D Tip = At + FVector2D(.5f, .5f) * S;
			Pen.Bar(Tip + Dir * (Near - 1.4f * S), Tip + Dir * (Far + 1.4f * S), 5.6f * S, FTSkin::Ink);
			Pen.Bar(Tip + Dir * Near, Tip + Dir * Far, 2.8f * S, FTSkin::Amber);
		}
	}
}

void FTCursor::HideSystemCursor(APlayerController* PC)
{
	if (!PC) return;
	// The system's pointer is switched OFF (EMouseCursor::None), not swapped for an empty shape. Until 16 Nov 2026
	// this made a 16 x 16 picture with every pixel clear and set it as a custom shape; on the owner's PC Windows drew
	// that as a small black square beside the drawn pointer ("two cursors almost"; it is the system's own pointer, so
	// it is in no screenshot). bShowMouseCursor stays as it is, so clicks and positions arrive as before: the viewport
	// only asks the player controller which shape to show, and None shows nothing.
	static bool bSaid = false;
	if (!bSaid) { bSaid = true; UE_LOG(LogTemp, Display, TEXT("FTINPUT system cursor=none")); }
	PC->CurrentMouseCursor = EMouseCursor::None;
}
