#include "FTInputGlyphs.h"
#include "FTInput.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "CanvasItem.h"

void FTInputGlyphs::DrawFace(UCanvas* Canvas, const UFTInputSubsystem* Input, const FKey& Key, FVector2D Center, float Size, UFont* Font)
{
	if (!Canvas || !Input || !Input->UsesGamepad()) return;
	const int32 Face = Key == EKeys::Gamepad_FaceButton_Bottom ? 0 : Key == EKeys::Gamepad_FaceButton_Right ? 1
		: Key == EKeys::Gamepad_FaceButton_Left ? 2 : Key == EKeys::Gamepad_FaceButton_Top ? 3 : INDEX_NONE;
	if (Face == INDEX_NONE) return;
	const float R = Size * .46f;
	const FLinearColor Colors[] = { FLinearColor(.28f, .95f, .65f), FLinearColor(1.f, .38f, .40f), FLinearColor(.36f, .76f, 1.f), FLinearColor(1.f, .84f, .28f) };
	const FLinearColor Color = Input->Family() == EFTControllerFamily::Nintendo ? FLinearColor::White : Colors[Face];
	const auto Line = [&](FVector2D A, FVector2D B, FLinearColor C)
	{
		FCanvasLineItem Item(A, B); Item.SetColor(C); Item.LineThickness = FMath::Max(1.f, Size * .075f); Canvas->DrawItem(Item);
	};
	const auto Circle = [&](FVector2D At, float Radius, FLinearColor C)
	{
		for (int32 I = 0; I < 24; ++I)
		{
			const float A = I * 2.f * PI / 24.f, B = (I + 1) * 2.f * PI / 24.f;
			Line(At + FVector2D(FMath::Cos(A), FMath::Sin(A)) * Radius, At + FVector2D(FMath::Cos(B), FMath::Sin(B)) * Radius, C);
		}
	};
	if (Input->Family() == EFTControllerFamily::PlayStation)
	{
		if (Face == 0)
		{
			Line(Center + FVector2D(-R, -R), Center + FVector2D(R, R), Color);
			Line(Center + FVector2D(-R, R), Center + FVector2D(R, -R), Color);
		}
		else if (Face == 1) Circle(Center, R, Color);
		else if (Face == 2)
		{
			const FVector2D P[] = { Center + FVector2D(-R,-R), Center + FVector2D(R,-R), Center + FVector2D(R,R), Center + FVector2D(-R,R) };
			for (int32 I = 0; I < 4; ++I) Line(P[I], P[(I + 1) % 4], Color);
		}
		else
		{
			Line(Center + FVector2D(0,-R), Center + FVector2D(R,R), Color);
			Line(Center + FVector2D(R,R), Center + FVector2D(-R,R), Color);
			Line(Center + FVector2D(-R,R), Center + FVector2D(0,-R), Color);
		}
		return;
	}
	if (Input->Family() == EFTControllerFamily::Generic)
	{
		const FVector2D Offset[] = { FVector2D(0, R), FVector2D(R, 0), FVector2D(-R, 0), FVector2D(0, -R) };
		for (int32 I = 0; I < 4; ++I) Circle(Center + Offset[I] * .72f, Size * .15f, I == Face ? FLinearColor::White : FLinearColor(.20f,.26f,.34f));
		return;
	}
	Circle(Center, R, Color);
	if (Font)
	{
		const FString Label = Input->KeyLabel(Key);
		float Width = 0.f, Height = 0.f; Canvas->StrLen(Font, Label, Width, Height);
		const float Scale = Size * .64f / FMath::Max(1.f, Height);
		FCanvasTextItem Text(Center - FVector2D(Width, Height) * Scale * .5f, FText::FromString(Label), Font, Color);
		Text.Scale = FVector2D(Scale, Scale); Canvas->DrawItem(Text);
	}
}
