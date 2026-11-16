// FULL THROTTLE: the title menu's logo, animated. "Full" is thrown in, THROTTLE wipes on, the two stars twinkle
// (the intro, once per arrival at the title, inside one second), then the neon breathes and flickers and the stars
// glint until a selection is confirmed. The HUD and the game mode are rebuilt on every attract cut, so the state
// lives here, in the game instance. At rest the flat T_TitleLogo itself is drawn, by the same tile item as before,
// so the masthead is pixel for pixel what it was; the layers (Tools/title/make_layers.py) are used only by the intro.
// Switch it off: ft.TitleLogo 0 (the HUD then draws the flat logo exactly as it always did).
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "FTTitleLogo.generated.h"

class AFTGameMode;
class UAudioComponent;
class UCanvas;
class USoundBase;
class UTexture2D;

UCLASS()
class FULLTHROTTLE_API UFTTitleLogo : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
public:
	/** Draws the title's logo. false: nothing was drawn and the caller draws the flat logo as it always did
	 *  (ft.TitleLogo 0, a layer missing, no subsystem). X, Y, W, H: the flat logo's rectangle, the caller's own floats.
	 *  Alpha: the flat tile's opacity this frame. */
	static bool Draw(UCanvas* Canvas, const AFTGameMode* G, UTexture2D* Flat, float X, float Y, float W, float H, float Alpha, float S, bool bInteractive, bool bReducedMotion);
	/** A title selection was confirmed (AFTGameMode::TickTitle, Activate 0..6). */
	static void Confirm(const AFTGameMode* G, int32 Activate);

	// UGameInstanceSubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// FTickableGameObject (the not-drawn timer and the hum's wanted state)
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override { return !IsTemplate(); }
	virtual bool IsTickableWhenPaused() const override { return true; }
	virtual ETickableTickType GetTickableTickType() const override { return IsTemplate() ? ETickableTickType::Never : ETickableTickType::Conditional; }

	static constexpr int32 NumLayers = 11;

private:
	enum class EState : uint8 { Idle, Intro, Alive, Confirmed };
	enum class EVoice : uint8 { OneShot, Intro, Persist };
	struct FPaint;

	UPROPERTY(Transient) TObjectPtr<UTexture2D> Tex[11];   // (NumLayers; a plain number for the header tool)
	UPROPERTY(Transient) TMap<FName, TObjectPtr<USoundBase>> Cues;
	UPROPERTY(Transient) TObjectPtr<UAudioComponent> Hum;
	TArray<TWeakObjectPtr<UAudioComponent>> IntroVoices;

	EState State = EState::Idle;
	float LastTitleT = -1.f;          // the game's TitleT at the last draw (-1: never drawn)
	uint64 LastDrawFrame = 0, ArrivalFrame = 0;
	float IntroT0 = -1.f;             // the TitleT the intro counts from (-1: not set yet, the masthead is empty)
	float PrevTau = -1.f;             // the intro's time at the last draw (its events fire on the frame they are crossed)
	double Clock = 0.0;               // this subsystem's own clock, one step per drawn frame (the confirm twinkle)
	double ClockStep = 0.0;           // ... and this frame's step of it (0 on a second draw in the same frame)
	float SinceDraw = 0.f;            // real seconds since the title was last drawn (at most 0.1 a tick)
	// Alive time: seconds the alive state has been drawn, summed in a double from the frames' own steps. Not the
	// game's TitleT: that is a float carried across every attract cut, and after a day on the title it no longer
	// steps evenly. It stands still whenever the alive logo is not drawn, so nothing is ever owed for a gap.
	double AliveNow = 0.0, PrevAlive = 0.0;
	double RampFrom = 0.0;            // the breathing glow fades in from here (alive time)
	double DrawFrom = 0.0;            // a twinkle, sweep or dip that began before this is not shown part-way (alive time)
	double ConfirmHeld = 0.0;         // seconds the title has stood open to input since the confirm
	uint64 ConfirmFrame = 0;
	double ConfirmClock = -1.0;
	bool bHumWanted = false;
	float HumVolume = 0.f;
	bool bLoadTried = false, bLayersOk = false;
	int32 LoadPasses = 0;

	static UFTTitleLogo* Get(const AFTGameMode* G);
	bool EnsureLoaded();
	bool DrawNow(UCanvas* Canvas, const AFTGameMode* G, UTexture2D* Flat, float X, float Y, float W, float H, float Alpha, float S, bool bInteractive, bool bReducedMotion);
	void DrawIntro(FPaint& P, float Tau, float S, float W, float FrameTime) const;
	void DrawAlive(FPaint& P, const AFTGameMode* G, bool bReducedMotion);
	void ConfirmNow(const AFTGameMode* G, int32 Activate);
	void EnterAlive(float TitleT, float Since, const TCHAR* Why);
	bool SkipPressed(const AFTGameMode* G) const;
	void Cue(const AFTGameMode* G, const TCHAR* Name, float Volume, float Pitch, EVoice Kind);
	void FadeIntroVoices();
	void SetHum(const UObject* Ctx, bool bWant);
	void OnPreLoadMap(const FString& MapName);
};
