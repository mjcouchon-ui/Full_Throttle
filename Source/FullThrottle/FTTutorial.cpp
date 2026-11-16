#include "FTTutorial.h"

float FFTTutorialState::Progress() const
{
	if (Step == EFTTutorialStep::Complete) return 1.f;
	if (!IsActive()) return 0.f;
	return (StepIndex() + (bActionObserved ? 1.f : 0.f)) / (float)NumSteps;
}

void FFTTutorialState::Begin()
{
	*this = FFTTutorialState();
	Step = EFTTutorialStep::Steer;
}

void FFTTutorialState::Skip()
{
	if (!IsActive()) return;
	Step = EFTTutorialStep::None;
	bSkipped = true;
}

void FFTTutorialState::Finish()
{
	if (!IsActive()) return;
	Step = EFTTutorialStep::Complete;
	bActionObserved = false;
}

void FFTTutorialState::Signal(EFTTutorialStep Action)
{
	if (IsActive() && Action == Step) bActionObserved = true;
}

bool FFTTutorialState::Tick(float GameplaySeconds)
{
	if (!IsActive() || GameplaySeconds <= 0.f) return false;
	// A hitch cannot skip the acknowledgement or turn one event into several completed steps.
	const float Dt = FMath::Min(GameplaySeconds, 0.1f);
	StepElapsed += Dt;
	TotalElapsed += Dt;
	if (bActionObserved) SuccessElapsed += Dt;
	if (!bActionObserved || StepElapsed < MinimumStepSeconds || SuccessElapsed < ConfirmationSeconds) return false;
	Step = (EFTTutorialStep)((uint8)Step + 1);
	StepElapsed = SuccessElapsed = 0.f;
	bActionObserved = false;
	return true;
}

#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFTTutorialProgressTest, "FullThrottle.Onboarding.ActionProgress",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FFTTutorialProgressTest::RunTest(const FString& Parameters)
{
	FFTTutorialState S;
	S.Begin();
	// A slow player is not failed or silently advanced; unrelated actions do not satisfy a lesson.
	S.Signal(EFTTutorialStep::Missile);
	for (int32 I = 0; I < 1200; ++I) S.Tick(0.1f);
	TestTrue(TEXT("An unperformed action remains available after two minutes"), S.Step == EFTTutorialStep::Steer);
	S.Signal(EFTTutorialStep::Steer);
	TestFalse(TEXT("A zero-time/paused tick cannot advance"), S.Tick(0.f));
	TestFalse(TEXT("A single hitch cannot skip confirmation"), S.Tick(10.f));
	for (int32 I = 0; I < 9; ++I) S.Tick(0.1f);
	TestTrue(TEXT("Observed steering advances after confirmation"), S.Step == EFTTutorialStep::Drift);
	for (int32 Step = (int32)EFTTutorialStep::Drift; Step <= (int32)EFTTutorialStep::Missile; ++Step)
	{
		S.Signal((EFTTutorialStep)Step);
		S.Tick(0.1f);
		TestTrue(TEXT("Each new card gets reading time"), (int32)S.Step == Step);
		for (int32 I = 0; I < 40; ++I) S.Tick(0.1f);
	}
	TestTrue(TEXT("Completes only after all six performed actions"), S.Step == EFTTutorialStep::Complete);
	TestEqual(TEXT("Completed progress"), S.Progress(), 1.f);
	S.Begin(); S.Skip();
	S.Signal(EFTTutorialStep::Steer); S.Tick(20.f);
	TestTrue(TEXT("Explicit skip stops progress"), S.bSkipped && !S.IsActive());
	S.Begin();
	TestTrue(TEXT("Replay starts a clean first lesson"), S.IsActive() && S.Step == EFTTutorialStep::Steer && !S.bSkipped && S.TotalElapsed == 0.f);
	return true;
}
#endif
