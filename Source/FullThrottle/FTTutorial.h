// First-play onboarding is driven by successful gameplay events, not by a timer expiring.
#pragma once

#include "CoreMinimal.h"

enum class EFTTutorialStep : uint8 { None, Steer, Drift, Turbo, Fire, Pickup, Missile, Complete };

struct FULLTHROTTLE_API FFTTutorialState
{
	static constexpr int32 NumSteps = 6;
	static constexpr float MinimumStepSeconds = 4.f;
	static constexpr float ConfirmationSeconds = 0.9f;
	EFTTutorialStep Step = EFTTutorialStep::None;
	float StepElapsed = 0.f, TotalElapsed = 0.f, SuccessElapsed = 0.f;
	bool bActionObserved = false, bSkipped = false;

	bool IsActive() const { return Step > EFTTutorialStep::None && Step < EFTTutorialStep::Complete; }
	int32 StepIndex() const { return FMath::Clamp((int32)Step - 1, 0, NumSteps - 1); }
	float Progress() const;
	void Begin();
	void Skip();
	void Signal(EFTTutorialStep Action);
	/** Call only while gameplay is running. Returns true when a step changes. */
	bool Tick(float GameplaySeconds);
};
