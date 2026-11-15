#pragma once

#include "CoreMinimal.h"

namespace FTHUDStyle
{
	inline const FLinearColor Ink(0.009f, 0.017f, 0.039f, 0.94f);
	inline const FLinearColor Deep(0.004f, 0.009f, 0.023f, 0.97f);
	inline const FLinearColor Steel(0.041f, 0.069f, 0.112f, 0.97f);
	inline const FLinearColor Active(0.025f, 0.145f, 0.190f, 0.99f);
	inline const FLinearColor Rule(0.27f, 0.39f, 0.54f, 0.62f);
	inline const FLinearColor Track(0.095f, 0.135f, 0.20f, 0.95f);
	inline const FLinearColor White(1.f, 0.954f, 0.848f);
	inline const FLinearColor Grey(0.60f, 0.71f, 0.82f);
	inline const FLinearColor Cyan(0.19f, 0.92f, 1.f);
	inline const FLinearColor Pink(1.f, 0.27f, 0.62f);
	inline const FLinearColor Yellow(1.f, 0.83f, 0.42f);
	inline const FLinearColor Amber(1.f, 0.57f, 0.18f);
	inline const FLinearColor Red(1.f, 0.23f, 0.27f);
	inline const FLinearColor Green(0.38f, 1.f, 0.55f);

	constexpr float Width = 1280.f;
	constexpr float CanvasHeight = 720.f;
	constexpr float Safe = 32.f;
	constexpr float Inset = 14.f;
	constexpr float Gap = 8.f;
	constexpr float Corner = 12.f;
	constexpr float IconSize = 18.f;
	constexpr float LabelSize = 15.f;
	constexpr float Body = 18.f;
	constexpr float Strong = 21.f;
	constexpr float Heading = 33.f;
	constexpr float Readout = 46.f;
	constexpr float Enter = 0.28f;
	constexpr float Feedback = 0.42f;
	constexpr float MenuEntrance = .32f;
	constexpr float FocusSweep = .22f;
	constexpr float RowStagger = .017f;
	constexpr float ClapperClose = .88f;
	constexpr float ClapperTravel = .12f;

	inline FLinearColor Alpha(FLinearColor Color, float Opacity)
	{
		Color.A *= Opacity;
		return Color;
	}

	inline float EaseOut(float Age, float Duration = Enter)
	{
		const float Rest = 1.f - FMath::Clamp(Age / Duration, 0.f, 1.f);
		return 1.f - Rest * Rest * Rest;
	}
}
