#include "FTGameMode.h"
#include "FTTrack.h"
#include "FTPlayerCar.h"
#include "FTEntity.h"
#include "FTPlayerController.h"
#include "FTGameInstance.h"
#include "FTHUD.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformTime.h"
#include "GameFramework/GameUserSettings.h"
#include "RHI.h"
#include "DynamicRHI.h"
#include "RenderTimer.h"
#include "UnrealClient.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollectionObject.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "HAL/IConsoleManager.h"

// test hooks: ft.Autopilot 1 drives the car (and starts the race from the title); ft.Shots N takes a screenshot every N seconds
static TAutoConsoleVariable<int32> CVarFTAutopilot(TEXT("ft.Autopilot"), 0, TEXT("FULL THROTTLE: 1 = the autopilot drives and starts races (tests, benchmarks)"));
static TAutoConsoleVariable<float> CVarFTShots(TEXT("ft.Shots"), 0.f, TEXT("FULL THROTTLE: take a screenshot every N seconds while racing (0 = off)"));

using FT::XU; using FT::DU; using FT::RoadHalf; using FT::LaneX; using FT::BaseSpeed; using FT::WallH;

namespace
{
	template <typename T> T* Load(const FString& Path)
	{
		return Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *Path, nullptr, LOAD_NoWarn | LOAD_Quiet));
	}
	inline float R(float A, float B) { return FMath::FRandRange(A, B); }
	const FLinearColor CYellow(1.f, 0.82f, 0.25f), CPink(1.f, 0.31f, 0.65f), CCyan(0.21f, 0.95f, 1.f), CRed(1.f, 0.25f, 0.25f), CGreen(0.4f, 1.f, 0.5f), CWhite(1, 1, 1);
	constexpr float CarW = 256.f, CarL = 480.f;
	const int32 HenchSideLeft = 0;
}

AFTGameMode::AFTGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PlayerControllerClass = AFTPlayerController::StaticClass();
	HUDClass = AFTHUD::StaticClass();
	DefaultPawnClass = nullptr;
}

UFTGameInstance* AFTGameMode::GI() const { return GetGameInstance<UFTGameInstance>(); }
float AFTGameMode::PlayerD() const { return Car ? Car->D : 0.f; }

void AFTGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	Level = MapName.Contains(TEXT("Chemical")) ? 1 : (MapName.Contains(TEXT("Nevada")) ? 2 : 0);
}

void AFTGameMode::BeginPlay()
{
	Super::BeginPlay();
	UFTGameInstance* G = GI();
	if (G && G->PendingLevel >= 0 && G->PendingLevel != Level && G->bBench)
	{
		GoToLevel(G->PendingLevel, true);
		return;
	}
	SetupLevel();
	if (G) { G->ReapplyQuality(); G->ApplyMute(); TitleSel = Level; }
	if (G && G->bStartRace)
	{
		G->bStartRace = false;
		G->PendingLevel = -1;
		StartRace(false);
	}
	else
	{
		Phase = EFTPhase::Title;
		PhaseT = 0.f;
		if (Music) { Music->SetSound(Load<USoundBase>(TEXT("/Game/Audio/Music/M_TitleTheme"))); Music->SetVolumeMultiplier(0.55f); Music->Play(); }
	}
}

void AFTGameMode::EndPlay(const EEndPlayReason::Type Reason)
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.f);
	Super::EndPlay(Reason);
}

void AFTGameMode::SetupLevel()
{
	for (TActorIterator<AFTTrack> It(GetWorld()); It; ++It) { Track = *It; break; }
	if (!Track)
	{
		FActorSpawnParameters P; P.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Track = GetWorld()->SpawnActor<AFTTrack>(AFTTrack::StaticClass(), FTransform::Identity, P);
		Track->LevelIndex = Level;
	}
	if (!Track->IsBuilt()) Track->Rebuild();

	UClass* CarClass = Load<UClass>(TEXT("/Game/Characters/Kestrel/BP_Kestrel.BP_Kestrel_C"));
	if (!CarClass) CarClass = AFTPlayerCar::StaticClass();
	FActorSpawnParameters P; P.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Car = GetWorld()->SpawnActor<AFTPlayerCar>(CarClass, Track->TransformAt(600.f, 32.f * XU, 140.f), P);
	Car->SetTrack(Track);
	Car->SetLightsForLevel(Level);
	Car->PlaceOnTrack(600.f, 32.f * XU, 0.f);
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetViewTarget(Car);
	}

	// lights and audio helpers live on this actor
	USceneComponent* Root = NewObject<USceneComponent>(this, TEXT("FTRoot"));
	Root->RegisterComponent();
	SetRootComponent(Root);
	for (int32 i = 0; i < 6; ++i)
	{
		UPointLightComponent* L = NewObject<UPointLightComponent>(this);
		L->SetupAttachment(Root);
		L->SetMobility(EComponentMobility::Movable);
		L->SetCastShadows(false);
		L->SetIntensityUnits(ELightUnits::Candelas);
		L->SetIntensity(0.f);
		L->SetAttenuationRadius(4000.f);
		L->SetLightColor(FLinearColor(1.f, 0.6f, 0.25f));
		L->RegisterComponent();
		Flashes.Add(L);
	}
	const FLinearColor LampCol = Level == 1 ? FLinearColor(1.f, 0.62f, 0.25f) : (Level == 0 ? FLinearColor(1.f, 0.75f, 0.55f) : FLinearColor(1.f, 0.7f, 0.5f));
	for (int32 i = 0; i < 18; ++i)
	{
		UPointLightComponent* L = NewObject<UPointLightComponent>(this);
		L->SetupAttachment(Root);
		L->SetMobility(EComponentMobility::Movable);
		L->SetCastShadows(false);
		L->SetIntensityUnits(ELightUnits::Candelas);
		L->SetIntensity(0.f);
		L->SetAttenuationRadius(2400.f);
		L->SetLightColor(LampCol);
		L->RegisterComponent();
		RoadLights.Add(L);
	}
	Music = UGameplayStatics::CreateSound2D(this, Load<USoundBase>(TEXT("/Game/Audio/Music/M_PixelAdventure")), 0.34f, 1.f, 0.f, nullptr, false, false);
	if (Music) Music->bIsUISound = true;
	Rotor = UGameplayStatics::CreateSound2D(this, Load<USoundBase>(TEXT("/Game/Audio/SFX/S_Rotor")), 0.f, 1.f, 0.f, nullptr, false, false);
	if (Rotor && Rotor->Sound) Rotor->Play();
}

// --------------------------------------------------------------------------------------------- flow
void AFTGameMode::GoToLevel(int32 L, bool bStart)
{
	UFTGameInstance* G = GI();
	if (G) { G->PendingLevel = L; G->bStartRace = bStart; }
	UGameplayStatics::SetGlobalTimeDilation(this, 1.f);
	UGameplayStatics::SetGamePaused(this, false);
	UGameplayStatics::OpenLevel(this, FTData::Level(L).Map);
}

void AFTGameMode::StartRace(bool bRetake)
{
	ClearEnts();
	Hazards.Reset();
	const FFTLevelDef& L = Def();
	if (!bRetake)
	{
		Score = 0; Combo = 1; ComboT = 0; Kills = NearMisses = StuntCount = Collateral = 0;
		ScoreDestruction = ScoreNear = ScoreStunts = ScoreCollateral = ArmorBonus = 0;
		Retakes = 0; Take = 1; Scene = 0; Hp = 100.f; Missiles = 6; Turbo = 100.f;
		Clips.Reset(); Snaps.Reset(); Clock = 0.f;
		bBossSpawned = bBossDead = bEscaped = bOutOfFilm = bEscapeWarned = false; BossT = 0.f; BossLineStep = 0;
		if (UFTGameInstance* G = GI()) G->RunCounter++;
		AheadSaid.Reset();
	}
	else
	{
		Take++;
		Score = CkScore; ScoreDestruction = CkDest; ScoreNear = CkNear; ScoreStunts = CkStunts; ScoreCollateral = CkColl;
		Kills = CkKills; NearMisses = CkNearN; StuntCount = CkStuntN; Collateral = CkCollN;
		Hp = FMath::Max(CkHp, 75.f); Missiles = FMath::Max(CkMissiles, 3); Turbo = 100.f;
		Clips.RemoveAll([this](const FFTClip& C) { return C.Scene >= Scene && !C.bBoss; });
		bBossSpawned = false; BossT = 0.f;
	}
	// start at the top of the scene, on plain road
	float D0 = FMath::Max(600.f, L.Scenes[Scene].D * DU - 400.f);
	while (D0 > 600.f && (Track->FlagsAt(D0) & (FTFlag::Gap | FTFlag::Loop | FTFlag::Pipe | FTFlag::Twist | FTFlag::Kicker))) D0 -= 400.f;
	Car->PlaceOnTrack(D0, 32.f * XU, 0.f);
	Car->RestoreParts();
	Car->SetHenchmenVisible(false, false);
	HenchOn[0] = HenchOn[1] = 0;
	PartsShed = 0;
	for (int32 i = 0; i < 7; ++i) if (Hp < 88.f - 12.f * i) { Car->ShedPart(i); PartsShed = i + 1; }
	PrevHp = Hp;
	Car->SetDamageLook(Hp);
	SpawnScene = Scene; SpawnIdx = 0;
	bDrifting = false; DriftCharge = 0.f; DriftLevel = 0; BoostT = 0.f; bTurboOn = false; bTurboLock = false;
	Invuln = 1.2f; CivT = 1.f;
	Phase = EFTPhase::Card; PhaseT = 0.f;
	Overlay = EFTOverlay::None;
	Line.Reset(); LinePri = 0;
	TimeScale = 1.f; SlowT = 0.f; FreezeT = 0.f;
	UGameplayStatics::SetGlobalTimeDilation(this, 1.f);
	Sfx(TEXT("Sting"));
	if (!bRetake && Music) { Music->FadeOut(1.2f, 0.f); }
	CkScore = Score; CkDest = ScoreDestruction; CkNear = ScoreNear; CkStunts = ScoreStunts; CkColl = ScoreCollateral;
	CkKills = Kills; CkNearN = NearMisses; CkStuntN = StuntCount; CkCollN = Collateral; CkHp = Hp; CkMissiles = Missiles;
	bShowKeys = !bRetake;
	SceneStartClock = Clock;
}

void AFTGameMode::BeginPlayPhase()
{
	Phase = EFTPhase::Play; PhaseT = 0.f;
	// a standing start: the Chaos wheels and the governor launch the car (a teleport at speed would trip the tyres)
	Car->PlaceOnTrack(Car->D, Car->X, 0.f);
	SceneSlateT = RealClock; ActionT = RealClock + 0.45f;
	const FFTScene& Sc = Def().Scenes[Scene];
	if (Take <= 1) SayText(Sc.Intro, 3);
	else
	{
		const TArray<FString>& RL = FTData::Lines(TEXT("retake"));
		SayText(RL[(Take - 2) % RL.Num()].Replace(TEXT("{k}"), *FString::FromInt(Take)), 3);
		CaptionT = RealClock + 2.4f; Caption = Sc.Retake;
	}
	if (Music && (!Music->IsPlaying() || Take <= 1))
	{
		Music->SetSound(Load<USoundBase>(TEXT("/Game/Audio/Music/M_PixelAdventure")));
		Music->SetVolumeMultiplier(MusicVol);
		Music->Play();
	}
}

void AFTGameMode::Wreck()
{
	if (Phase != EFTPhase::Play) return;
	Phase = EFTPhase::Wreck; PhaseT = 0.f;
	Car->Wreck();
	Explode(Car->GetActorLocation(), 2.2f);
	Freeze(0.14f);
	Slowmo(1.8f, 0.3f);
	Sfx(TEXT("Wreck"));
	Retakes++;
	if (Retakes < 3)
	{
		Say(TEXT("cut"), 4);
		CutTitle = TEXT("CUT!"); CutSub = FString::Printf(TEXT("AGAIN! FROM THE TOP OF SCENE %d · TAKE %d COMING UP"), Scene + 1, Take + 1);
	}
	else
	{
		SayText(TEXT("CUT! That's it. We're out of film!"), 4);
		CutTitle = TEXT("CUT!"); CutSub = TEXT("OUT OF FILM. WE LOST THE STAR CAR!");
		bOutOfFilm = true;
	}
	CutT = RealClock + 0.6f;
	Clips.RemoveAll([this](const FFTClip& C) { return C.Scene >= Scene && !C.bBoss; });
}

void AFTGameMode::Wrap(bool bEscape)
{
	if (Phase != EFTPhase::Play) return;
	Phase = EFTPhase::Wrap; PhaseT = 0.f;
	bEscaped = bEscape;
	if (!bEscape)
	{
		Freeze(0.22f);
		Slowmo(2.4f, 0.22f);
		for (FFTEnt& E : Ents)
			if (!E.bDead && E.Kind != EFTKind::Bullet && E.Kind != EFTKind::Missile && E.Kind != EFTKind::Crate && E.Kind != EFTKind::Wrench && E.Kind != EFTKind::Civ) Kill(E, 3);
		ArmorBonus = (int64)(Hp * 20.f);
		Score += ArmorBonus;
		SayText(TEXT("CUT! PRINT IT! That's a WRAP, people!"), 5);
		CutTitle = TEXT("CUT!"); CutSub = TEXT("PRINT IT. THAT'S A WRAP!");
		Sfx(TEXT("Wrap")); Duck(0.35f, 1.2f);
	}
	else
	{
		SayText(TEXT("CUT... He got away. Unbelievable."), 5);
		CutTitle = TEXT("CUT!"); CutSub = TEXT("THE TARGET GOT AWAY!");
		Caption = FString::Printf(TEXT("%s GOT AWAY..."), *Def().BossName); CaptionT = RealClock;
	}
	CutT = RealClock + 0.8f;
}

void AFTGameMode::FinishRating()
{
	const FFTLevelDef& L = Def();
	RatingIdx = 0;
	for (int32 i = 0; i < 4; ++i) if (Score >= L.Rating[i]) RatingIdx = i;
	if (bOutOfFilm) RatingIdx = 0;
	if (bEscaped) RatingIdx = FMath::Min(RatingIdx, 1);
	static const TCHAR* Q[] = { TEXT("Straight to video. The stunt budget went on catering."), TEXT("Solid action. The trailer guy is warming up his voice."), TEXT("Lines around the block. Sequel greenlit."), TEXT("They'll teach this chase in film school.") };
	RatingQuip = Q[RatingIdx];
	static const TCHAR* T[] = { TEXT("B-MOVIE"), TEXT("ACTION HERO"), TEXT("BLOCKBUSTER"), TEXT("LEGENDARY DIRECTOR") };
	if (UFTGameInstance* G = GI())
	{
		if (!bOutOfFilm && Score > G->BestScore[Level]) { G->BestScore[Level] = (int32)Score; G->BestTitle[Level] = T[RatingIdx]; G->SaveSettings(); }
	}
	Phase = EFTPhase::Rating; PhaseT = 0.f;
	if (Music) Music->SetVolumeMultiplier(0.25f);
}

// --------------------------------------------------------------------------------------------- tick
void AFTGameMode::Tick(float Dt)
{
	Super::Tick(Dt);
	if (!Track || !Car) return;
	const float RDt = FMath::Min(FApp::GetDeltaTime(), 0.1f);
	RealClock += RDt;
	Fps = RDt > 0.f ? 1.f / RDt : 0.f;
	FpsSmoothed = FMath::Lerp(FpsSmoothed, Fps, 0.05f);
	PhaseT += RDt;
	AFTPlayerController* PC = Cast<AFTPlayerController>(GetWorld()->GetFirstPlayerController());
	UFTGameInstance* G = GI();

	if (G && PC)
	{
		if (PC->Hit(EKeys::M)) { G->bMuted = !G->bMuted; G->ApplyMute(); G->SaveSettings(); }
		if (PC->Hit(EKeys::F1)) { G->bShowFps = !G->bShowFps; G->SaveSettings(); }
	}

	if (Overlay != EFTOverlay::None) { TickOverlay(RDt); return; }

	// music ducking
	if (Music && Phase != EFTPhase::Title)
	{
		DuckT -= RDt;
		const float Target = (DuckT > 0.f ? DuckDepth : 1.f) * MusicVol;
		Music->SetVolumeMultiplier(FMath::FInterpTo(Music->VolumeMultiplier, Target, RDt, DuckT > 0.f ? 12.f : 2.5f));
	}

	switch (Phase)
	{
	case EFTPhase::Title:
	{
		TickTitle(RDt);
		FFTDrive Dr; Dr.bFrozen = true; Car->SetDrive(Dr);
		break;
	}
	case EFTPhase::Card:
	{
		FFTDrive Dr; Dr.bFrozen = true; Car->SetDrive(Dr);
		if (PhaseT > 3.2f || (PhaseT > 1.2f && PC && PC->MenuOk()) || (G && G->bBench && PhaseT > 0.5f)) BeginPlayPhase();
		if (PhaseT > 0.45f && PhaseT - RDt <= 1.0f && PhaseT >= 1.0f) Sfx(TEXT("Clap"));
		break;
	}
	case EFTPhase::Play:
		TickPlay(Dt, RDt);
		break;
	case EFTPhase::Wreck:
	{
		Clock += Dt;
		TickEnts(Dt);
		TickSlowmo(RDt);
		TickDirector(RDt);
		if (PhaseT > 2.8f)
		{
			if (bOutOfFilm) { TickSlowmo(10.f); FinishRating(); }
			else StartRace(true);
		}
		break;
	}
	case EFTPhase::Wrap:
	{
		Clock += Dt;
		TickEnts(Dt);
		TickSlowmo(RDt);
		TickDirector(RDt);
		FFTDrive Dr; Dr.TargetSpeed = BaseSpeed * 0.8f; Dr.Steer = 0.f; Car->SetDrive(Dr);
		if (PhaseT > 4.2f)
		{
			UGameplayStatics::SetGlobalTimeDilation(this, 1.f); TimeScale = 1.f;
			if (!bEscaped && Clips.Num() > 0 && !(G && G->bBench)) StartReel();
			else FinishRating();
		}
		break;
	}
	case EFTPhase::Reel:
		TickReel(RDt);
		break;
	case EFTPhase::Rating:
	{
		FFTDrive Dr; Dr.TargetSpeed = BaseSpeed * 0.7f; Car->SetDrive(Dr);
		if (PC && PhaseT > 1.0f)
		{
			if (PC->Hit(EKeys::R)) GoToLevel(Level, true);
			else if (PC->MenuBack()) GoToLevel(Level, false);
			else if (PC->MenuOk())
			{
				if (bOutOfFilm || Level >= 2) GoToLevel(bOutOfFilm ? Level : 0, false);
				else GoToLevel(Level + 1, true);
			}
		}
		break;
	}
	}

	if (G && G->bBench) TickBench(RDt);
	if (G && G->QuitAfter > 0.f && RealClock > G->QuitAfter) { UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false); G->QuitAfter = 0.f; }
	if (const float Every = (G && G->ShotEvery > 0.f) ? G->ShotEvery : CVarFTShots.GetValueOnGameThread(); Every > 0.f)
	{
		static float ShotT = 0.f; static int32 ShotN = 0;
		ShotT += RDt;
		if (ShotT >= Every)
		{
			ShotT = 0.f;
			const FString Name = FString::Printf(TEXT("ft_L%d_%03d"), Level, ShotN++);
			if (G && G->ShotEvery > 0.f) FScreenshotRequest::RequestScreenshot(FPaths::Combine(G->SaveDir(), TEXT("shots"), Name + TEXT(".png")), true, false);
			else Snap(Name);
		}
	}
	if (CVarFTAutopilot.GetValueOnGameThread() > 0)
	{
		static float LogT = 0.f;
		LogT += RDt;
		if (LogT > 2.f)
		{
			LogT = 0.f;
			UE_LOG(LogFT, Display, TEXT("FTTEL phase=%d t=%.1f D=%.0f X=%.0f H=%.0f speed=%.0f lat=%.0f wheels=%d hp=%.0f ents=%d scene=%d score=%lld fps=%.0f"),
				(int32)Phase, PhaseT, Car->D, Car->X, Car->H, Car->Speed, Car->LatSpeed, Car->WheelsDown, Hp, Ents.Num(), Scene, Score, FpsSmoothed);
		}
	}

	// road lights follow the car: the nearest lamp heads ahead and behind get real lights
	{
		const TArray<FVector>& Spots = Track->LightSpots();
		const FVector P = Car->GetActorLocation();
		TArray<int32> Best;
		Best.Reserve(RoadLights.Num());
		static TArray<float> Dist; Dist.SetNum(Spots.Num());
		for (int32 i = 0; i < Spots.Num(); ++i) Dist[i] = FVector::DistSquared(Spots[i], P);
		TArray<int32> Idx; Idx.Reserve(64);
		for (int32 i = 0; i < Spots.Num(); ++i) if (Dist[i] < FMath::Square(26000.f)) Idx.Add(i);
		Idx.Sort([](int32 A, int32 B) { return Dist[A] < Dist[B]; });
		for (int32 i = 0; i < RoadLights.Num(); ++i)
		{
			UPointLightComponent* L = RoadLights[i];
			if (i < Idx.Num()) { L->SetWorldLocation(Spots[Idx[i]]); L->SetIntensity(Level == 2 ? 5000.f : 7000.f); L->SetAttenuationRadius(3600.f); }
			else L->SetIntensity(0.f);
		}
	}
	// flashes fade
	for (UPointLightComponent* L : Flashes) L->SetIntensity(FMath::Max(0.f, L->Intensity - RDt * 90000.f));
}

void AFTGameMode::TickTitle(float RDt)
{
	AFTPlayerController* PC = Cast<AFTPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PC) return;
	if (PC->MenuLeft()) TitleSel = (TitleSel + 2) % 3;
	if (PC->MenuRight()) TitleSel = (TitleSel + 1) % 3;
	if (PC->Hit(EKeys::One)) TitleSel = 0;
	if (PC->Hit(EKeys::Two)) TitleSel = 1;
	if (PC->Hit(EKeys::Three)) TitleSel = 2;
	if (PC->Hit(EKeys::C) || PC->Hit(EKeys::Gamepad_FaceButton_Top)) { Overlay = EFTOverlay::Controls; ControlsRow = 0; ControlsCol = GI() ? GI()->Layout : 0; return; }
	if (PC->Hit(EKeys::O)) { Overlay = EFTOverlay::Options; OptionsRow = 0; return; }
	if (PC->Hit(EKeys::Escape) && PhaseT > 1.f) { UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false); return; }
	const UFTGameInstance* TG = GI();
	if ((PC->MenuOk() && PhaseT > 0.4f) || (CVarFTAutopilot.GetValueOnGameThread() > 0 && PhaseT > 1.5f) || (TG && TG->bAutoDrive && PhaseT > 3.5f))
	{
		if (TitleSel != Level) GoToLevel(TitleSel, true);
		else StartRace(false);
	}
}

void AFTGameMode::TickOverlay(float RDt)
{
	AFTPlayerController* PC = Cast<AFTPlayerController>(GetWorld()->GetFirstPlayerController());
	UFTGameInstance* G = GI();
	if (!PC || !G) return;
	if (Overlay == EFTOverlay::Pause)
	{
		if (PC->Hit(EKeys::P) || PC->MenuBack() || PC->Hit(EKeys::Gamepad_Special_Right)) { Overlay = EFTOverlay::None; UGameplayStatics::SetGamePaused(this, false); if (Music) Music->SetPaused(false); return; }
		if (PC->Hit(EKeys::C) || PC->Hit(EKeys::Gamepad_FaceButton_Top)) { Overlay = EFTOverlay::Controls; ControlsRow = 0; ControlsCol = G->Layout; return; }
		if (PC->Hit(EKeys::O)) { Overlay = EFTOverlay::Options; OptionsRow = 0; return; }
		if (PC->Hit(EKeys::R)) { UGameplayStatics::SetGamePaused(this, false); Overlay = EFTOverlay::None; GoToLevel(Level, true); return; }
		if (PC->Hit(EKeys::T)) { UGameplayStatics::SetGamePaused(this, false); Overlay = EFTOverlay::None; GoToLevel(Level, false); return; }
		return;
	}
	const bool bFromPause = Phase == EFTPhase::Play || Phase == EFTPhase::Wreck || Phase == EFTPhase::Card;
	if (Overlay == EFTOverlay::Controls)
	{
		if (bControlsWaiting)
		{
			FKey K;
			if (PC->PollCapture(K))
			{
				bControlsWaiting = false; PC->bCapture = false;
				if (K == EKeys::Escape) return;
				if (K == EKeys::Delete || K == EKeys::BackSpace) G->SetBind(ControlsRow, ControlsCol, FKey());
				else if (!UFTGameInstance::IsReserved(K)) G->SetBind(ControlsRow, ControlsCol, K);
				G->SaveSettings();
			}
			return;
		}
		if (PC->MenuUp()) ControlsRow = (ControlsRow + UFTGameInstance::NumActions + 1) % (UFTGameInstance::NumActions + 1);
		if (PC->MenuDown()) ControlsRow = (ControlsRow + 1) % (UFTGameInstance::NumActions + 1);
		if (PC->MenuLeft() || PC->MenuRight()) ControlsCol = 1 - ControlsCol;
		if (PC->Hit(EKeys::Enter) || PC->Hit(EKeys::Gamepad_FaceButton_Bottom))
		{
			if (ControlsRow == UFTGameInstance::NumActions) { G->ResetBinds(); G->SaveSettings(); }
			else { bControlsWaiting = true; PC->bCapture = true; }
		}
		if (PC->MenuBack() || PC->Hit(EKeys::C)) Overlay = bFromPause ? EFTOverlay::Pause : EFTOverlay::None;
		return;
	}
	if (Overlay == EFTOverlay::Options)
	{
		const int32 Rows = 3;
		if (PC->MenuUp()) OptionsRow = (OptionsRow + Rows - 1) % Rows;
		if (PC->MenuDown()) OptionsRow = (OptionsRow + 1) % Rows;
		const int32 Dir = PC->MenuRight() ? 1 : (PC->MenuLeft() ? -1 : 0);
		if (Dir != 0)
		{
			if (OptionsRow == 0) { int32 Q = G->Quality + Dir; if (Q < -1) Q = 3; if (Q > 3) Q = -1; G->ApplyQuality(Q); }
			else if (OptionsRow == 1) { G->bShowFps = !G->bShowFps; G->SaveSettings(); }
			else if (OptionsRow == 2) { G->bVSync = !G->bVSync; G->ApplyQuality(G->Quality); }
		}
		if (PC->MenuBack() || PC->Hit(EKeys::O) || PC->Hit(EKeys::Enter)) Overlay = bFromPause ? EFTOverlay::Pause : EFTOverlay::None;
	}
}

void AFTGameMode::TickPlay(float Dt, float RDt)
{
	AFTPlayerController* PC = Cast<AFTPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->bRaceInput = true;
		if (PC->Hit(EKeys::P) || PC->Hit(EKeys::Escape) || PC->Hit(EKeys::Gamepad_Special_Right))
		{
			Overlay = EFTOverlay::Pause; PauseRow = 0;
			UGameplayStatics::SetGamePaused(this, true);
			if (Music) Music->SetPaused(true);
			return;
		}
	}
	Clock += Dt;
	Timecode += Dt;
	if (PhaseT > 10.f) bShowKeys = false;
	TickPlayer(Dt);
	TickSpawns();
	TickCivilians(Dt);
	TickEnts(Dt);
	TickHazards(Dt);
	TickStunts(Dt);
	TickSlowmo(RDt);
	TickDirector(RDt);
	RecordSnap(RDt);

	// scene changes
	const FFTLevelDef& L = Def();
	while (Scene + 1 < L.Scenes.Num() && Car->D >= L.Scenes[Scene + 1].D * DU)
	{
		Scene++;
		Take = 1;
		SceneSlateT = RealClock; ActionT = RealClock + 0.45f;
		SayText(L.Scenes[Scene].Intro, 3);
		CkScore = Score; CkDest = ScoreDestruction; CkNear = ScoreNear; CkStunts = ScoreStunts; CkColl = ScoreCollateral;
		CkKills = Kills; CkNearN = NearMisses; CkStuntN = StuntCount; CkCollN = Collateral; CkHp = Hp; CkMissiles = Missiles;
		SceneStartClock = Clock;
	}
	const float Fin = L.FinaleD * DU;
	if (!bBossSpawned) Progress = FMath::Clamp(Car->D / Fin, 0.f, 1.f) * 0.85f;
	else Progress = 0.85f + 0.15f * (1.f - BossHpFrac);
	SpeedMph = FMath::Max(0.f, Car->Speed) / DU * 0.31f;

	if (Hp <= 0.f) Wreck();
}

// --------------------------------------------------------------------------------------------- player
void AFTGameMode::TickPlayer(float Dt)
{
	AFTPlayerController* PC = Cast<AFTPlayerController>(GetWorld()->GetFirstPlayerController());
	UFTGameInstance* G = GI();
	FFTInput In = PC ? PC->In : FFTInput();
	if ((G && (G->bBench || G->bAutoDrive)) || CVarFTAutopilot.GetValueOnGameThread() > 0)
	{
		// autopilot: pick the lane with the most room ahead, shoot everything, use the turbo
		float BestRoom = -1.f; float BestX = Car->X;
		for (int32 l = 0; l < 4; ++l)
		{
			float Room = 20000.f;
			for (const FFTEnt& E : Ents)
			{
				if (E.bDead || E.Kind == EFTKind::Bullet || E.Kind == EFTKind::Missile || E.Kind == EFTKind::Crate || E.Kind == EFTKind::Wrench || E.bAir) continue;
				const float A = E.D - Car->D;
				if (A > -300.f && FMath::Abs(E.X - LaneX(l)) < (E.W + CarW) * 0.6f) Room = FMath::Min(Room, A);
			}
			Room -= FMath::Abs(LaneX(l) - Car->X) * 0.3f;
			if (Room > BestRoom) { BestRoom = Room; BestX = LaneX(l); }
		}
		AutoLane = BestX;
		In = FFTInput();
		// steer by heading: aim at the chosen lane at a shallow angle
		const FFTFrame AF = Track->Sample(Car->D);
		const FVector CF = Car->GetActorForwardVector();
		const float Head = FMath::Atan2(FVector::DotProduct(FVector::CrossProduct(AF.Fwd, CF), AF.Up), FVector::DotProduct(CF, AF.Fwd));
		const float WantHead = FMath::Clamp((AutoLane - Car->X) / 1800.f, -0.3f, 0.3f);
		In.RawSteer = In.Steer = FMath::Clamp((WantHead - Head) * 2.2f, -1.f, 1.f);
		In.bFire = true;
		In.bTurbo = Turbo > 95.f || bTurboOn;
		In.bMissile = FMath::Fmod(Clock, 1.7f) < Dt;
		if (G && G->bBench) Hp = 100.f;
	}

	Steer = In.Steer;
	NoFireT += Dt; NoMissileT += Dt; NoNearT += Dt; NoRailT += Dt; NoDriftT += Dt; LastFireT += Dt;
	Invuln -= Dt; MissileCd -= Dt; DriftPressBuf -= Dt;

	// turbo
	const bool bWant = In.bTurbo;
	if (!bWant && Turbo > 25.f) bTurboLock = false;
	if (Turbo <= 0.f) { bTurboLock = true; }
	const bool bWasOn = bTurboOn;
	bTurboOn = bWant && !bTurboLock && Turbo > 0.f;
	if (bTurboOn)
	{
		Turbo = FMath::Max(0.f, Turbo - 38.f * Dt);
		if (!bWasOn)
		{
			if (TurboOffT > 1.f && Car->Speed < 700.f * DU) Car->Shove(Car->GetActorForwardVector(), 90.f * DU * 0.6f);
			Sfx(TEXT("Turbo")); Say(TEXT("turbo"), 1, 12.f);
		}
		TurboOffT = 0.f; NoTurboT = 0.f;
	}
	else { Turbo = FMath::Min(100.f, Turbo + 13.f * Dt); TurboOffT += Dt; NoTurboT += Dt; }

	// drift (kart style): hold drift and steer, either order; let go to boost
	if (In.bDriftPressed) DriftPressBuf = 0.25f;
	const float SpeedD = Car->Speed / DU;
	if (!bDrifting)
	{
		if (In.bDrift && FMath::Abs(In.RawSteer) > 0.12f && SpeedD > 200.f && Car->bGrounded && (DriftPressBuf > 0.f || FMath::Abs(Steer) < 0.3f || DriftKickT > 0.f))
		{
			bDrifting = true; DriftDir = In.RawSteer > 0.f ? 1 : -1; DriftT = 0.f; DriftCharge = 0.f; DriftLevel = 0;
			Car->Shove(Track->Sample(Car->D).Right, DriftDir * 70.f * XU * 0.5f);
			Sfx(TEXT("DriftStart"));
			NoDriftT = 0.f;
		}
		DriftKickT = In.bDrift && !Car->bGrounded ? 0.3f : DriftKickT - Dt;
	}
	else
	{
		DriftT += Dt;
		if (DriftT < 0.18f && In.RawSteer * DriftDir < -0.5f) DriftDir = -DriftDir;
		if (Car->bGrounded)
		{
			DriftCharge += (0.65f + 0.55f * FMath::Max(0.f, In.Steer * DriftDir)) * Dt;
			const int32 NewLevel = DriftCharge >= 2.35f ? 3 : (DriftCharge >= 1.45f ? 2 : (DriftCharge >= 0.65f ? 1 : 0));
			if (NewLevel > DriftLevel) { DriftLevel = NewLevel; Sfx(TEXT("DriftLevel"), 0.8f, 1.f + 0.2f * NewLevel); }
		}
		if (!In.bDrift)
		{
			if (DriftLevel >= 1)
			{
				static const float BT[3] = { 0.55f, 0.95f, 1.45f };
				BoostT = BT[DriftLevel - 1];
				Turbo = FMath::Min(100.f, Turbo + 8.f * DriftLevel);
				if (Car->Speed < 640.f * DU) Car->Shove(Car->GetActorForwardVector(), (50.f + 30.f * DriftLevel) * DU * 0.6f);
				Sfx(TEXT("Boost"));
				if (DriftLevel >= 2) { Say(TEXT("boost"), 1, 6.f); Stunt(DriftLevel == 2 ? TEXT("POWER DRIFT") : TEXT("ULTRA DRIFT"), DriftLevel == 2 ? 300 : 600, DriftLevel == 2 ? TEXT("Look at that slide! Gorgeous!") : TEXT("Sideways at that speed?! Print it!"), DriftLevel == 2 ? 2.f : 4.f); Shout(TEXT("DRIFT!")); }
			}
			bDrifting = false; DriftDir = 0; DriftCharge = 0.f; DriftLevel = 0;
		}
	}
	BoostT = FMath::Max(0.f, BoostT - Dt);
	bHot = bTurboOn || BoostT > 0.f;

	// target speed (1.x numbers)
	float Tgt = 380.f + (In.bUp ? 90.f : 0.f) - (In.bDown ? 120.f : 0.f);
	if (bTurboOn) Tgt += 320.f;
	else if (BoostT > 0.f) Tgt += 200.f + 40.f * 2;
	if (bDrifting) Tgt -= 25.f;
	Tgt -= 18.f * (HenchOn[0] + HenchOn[1]);
	FFTDrive Dr;
	Dr.Steer = Steer;
	Dr.TargetSpeed = Tgt * DU;
	Dr.Accel = (bHot ? 900.f : 420.f) * DU;
	Dr.bHot = bHot;
	Dr.DriftDir = bDrifting && Car->bGrounded ? DriftDir : 0;
	Dr.DriftTargetLat = -DriftDir * FMath::Abs(Car->Speed) * 0.36f;
	// a henchman drags the car toward his side
	if (HenchOn[0]) Car->Shove(-Track->Sample(Car->D).Right, 18.f * XU * Dt);
	if (HenchOn[1]) Car->Shove(Track->Sample(Car->D).Right, 18.f * XU * Dt);
	Car->SetDrive(Dr);

	// guns
	FireT -= Dt;
	if (In.bFire)
	{
		NoFireT = 0.f; LastFireT = 0.f;
		while (FireT <= 0.f)
		{
			FireT += 0.085f;
			static int32 Alt = 0; Alt ^= 1;
			FireBullet(0, Car->D + 260.f, Car->X + (Alt ? 8.f : -8.f) * XU, 115.f, Car->Speed + 1150.f * DU, 0.f);
			if (UNiagaraSystem* M = Load<UNiagaraSystem>(TEXT("/Game/FX/NS_Muzzle")))
			{
				const FTransform T = Car->GunMuzzle(Alt ? 1 : -1);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, M, T.GetLocation(), T.Rotator(), FVector(1), true, true, ENCPoolMethod::AutoRelease);
			}
			Sfx(TEXT("Gun"), 0.35f, R(0.95f, 1.05f));
		}
	}
	else FireT = FMath::Max(FireT, 0.f);
	if (In.bMissile && MissileCd <= 0.f)
	{
		if (Missiles > 0) { FireMissile(); Missiles--; MissileCd = 0.3f; NoMissileT = 0.f; }
		else { Sfx(TEXT("Empty")); Say(TEXT("empty"), 1, 4.f); MissileCd = 0.3f; }
	}

	// damage stages, parts and fire
	if (Hp < PrevHp)
	{
		while (PartsShed < 7 && Hp < 88.f - 12.f * PartsShed)
		{
			Car->ShedPart(PartsShed); PartsShed++;
			Sfx(TEXT("PartOff")); Say(TEXT("partOff"), 1, 8.f);
		}
	}
	if (!FMath::IsNearlyEqual(Hp, PrevHp)) Car->SetDamageLook(Hp);
	PrevHp = Hp;

	// barrier sparks: the director loves them
	if ((Car->bWallLeft || Car->bWallRight) && Car->Speed > 2000.f)
	{
		if (NoRailT > 0.5f) { Say(TEXT("rail"), 1, 5.f); Sfx(TEXT("Rail"), 0.5f); }
		NoRailT = 0.f;
	}

	// henchmen on the car
	for (int32 s = 0; s < 2; ++s)
	{
		if (!HenchOn[s]) continue;
		HenchPunch[s] -= Dt;
		if (HenchPunch[s] <= 0.f) { HurtPlayer(4.f); HenchPunch[s] = R(1.4f, 2.0f); Sfx(TEXT("HenchHit")); }
		const bool bWall = s == 0 ? Car->bWallLeft : Car->bWallRight;
		if (bWall && (FMath::Abs(Car->LatSpeed) > 60.f * XU || Car->WallImpact > 250.f || Car->Speed > 4000.f))
		{
			HenchOn[s] = 0;
			Stunt(TEXT("WALL SMASH"), 500, TEXT("Into the wall! Ha! Print it!"), 8.f);
			AddScore(200 * Combo, 2);
			Slowmo(0.55f, 0.35f);
			Sfx(TEXT("HenchOff"));
		}
	}
	if ((HenchOn[0] || HenchOn[1]) && (Track->FlagsAt(Car->D) & FTFlag::Inverted))
	{
		const int32 N = HenchOn[0] + HenchOn[1];
		HenchOn[0] = HenchOn[1] = 0;
		Stunt(N > 1 ? TEXT("GRAVITY CHECK x2") : TEXT("GRAVITY CHECK"), 600 * N, N > 1 ? TEXT("BOTH of them! Off the ceiling! Legendary!") : TEXT("Upside down and he's GONE! Gravity wins!"), N > 1 ? 10.f : 8.f);
		AddScore(200 * Combo * N, 2);
		Sfx(TEXT("HenchOff"));
	}
	if (HenchOn[0] || HenchOn[1])
	{
		HenchHangT += Dt;
		if (HenchHangT > 3.2f) { HenchHangT = 0.f; Say(TEXT("henchHang"), 1); }
	}
	Car->SetHenchmenVisible(HenchOn[0] != 0, HenchOn[1] != 0);

	// fell off the world: put it back (costs armour)
	if (Car->H < -3000.f || FMath::Abs(Car->X) > FT::Edge + 2500.f)
	{
		float Dsafe = Car->D;
		while (Track->FlagsAt(Dsafe) & (FTFlag::Gap | FTFlag::Loop | FTFlag::Pipe | FTFlag::Twist)) Dsafe += 400.f;
		Car->PlaceOnTrack(Dsafe, 0.f, 0.f);
		HurtPlayer(10.f);
	}

	// combo timer
	ComboT -= Dt;
	if (ComboT <= 0.f) Combo = 1;
	for (FFTPopup& P : Popups) P.T += FApp::GetDeltaTime();
	Popups.RemoveAll([](const FFTPopup& P) { return P.T > 1.6f; });
}

void AFTGameMode::HurtPlayer(float Dmg, const TCHAR* LineKey, float PushX)
{
	if (Invuln > 0.f || Phase != EFTPhase::Play) return;
	if (!Car->bGrounded && Dmg < 12.f) return; // nothing hurts you in the air (except things that land on you)
	Hp = FMath::Max(0.f, Hp - Dmg);
	Invuln = 0.6f;
	Sfx(TEXT("PlayerHit")); Duck(0.7f, 0.2f);
	Shake(0.4f + Dmg * 0.03f);
	if (bDrifting) DriftCharge = FMath::Max(0.f, DriftCharge - (0.6f + Dmg * 0.02f));
	if (PushX != 0.f) Car->Shove(Track->Sample(Car->D).Right, PushX);
	if (Hp < 35.f) Say(TEXT("lowHp"), 2, 6.f);
	else Say(TEXT("hit"), 1, 3.5f);
	if (GI() && GI()->bBench) Hp = 100.f;
}

// --------------------------------------------------------------------------------------------- entities
FFTEnt* AFTGameMode::FindEnt(int32 Id)
{
	for (FFTEnt& E : Ents) if (E.Id == Id) return &E;
	return nullptr;
}

FFTEnt& AFTGameMode::Spawn(EFTKind K, float Ahead, float Xc, int32 Variant)
{
	FFTEnt& E = PendingEnts.AddDefaulted_GetRef();
	E.Kind = K; E.Id = NextId++; E.Variant = Variant;
	E.D = Car->D + Ahead; E.X = Xc; E.Sp = Car->Speed;
	E.PrevAhead = Ahead;
	switch (K)
	{
	case EFTKind::Moto: E.W = 14 * XU * 1.6f; E.L = 32 * DU * 0.6f; E.Hp = 2; break;
	case EFTKind::Rammer: E.W = 30 * XU * 0.85f; E.L = 54 * DU * 0.62f; E.Hp = 5; break;
	case EFTKind::Suv: E.W = 36 * XU * 0.75f; E.L = 64 * DU * 0.62f; E.Hp = 10; break;
	case EFTKind::Truck: E.W = 46 * XU * 0.65f; E.L = 104 * DU * 0.62f; E.Hp = 26; E.bBig = true; break;
	case EFTKind::Heli: E.W = 40 * XU; E.L = 60 * DU; E.Hp = 40; E.bAir = true; E.bBig = true; E.Count = 2; break;
	case EFTKind::Hog: E.W = 16 * XU * 1.4f; E.L = 38 * DU * 0.55f; E.Hp = 3; break;
	case EFTKind::Ramp: E.W = 42 * XU * 0.7f; E.L = 124 * DU * 0.62f; E.Hp = 999; E.bBig = true; break;
	case EFTKind::Crate: case EFTKind::Wrench: E.W = 200.f; E.L = 200.f; E.Hp = 999; break;
	case EFTKind::Barrel: E.W = 18 * XU; E.L = 18 * DU; E.Hp = 1; break;
	case EFTKind::Henchman: E.W = 12 * XU; E.L = 14 * DU; E.Hp = 1; E.bAir = true; break;
	case EFTKind::Civ:
	{
		static const float CW[8] = { 28, 28, 28, 26, 26, 32, 32, 36 };
		static const float CL[8] = { 52, 52, 52, 44, 44, 64, 64, 112 };
		static const float CH[8] = { 4, 4, 4, 3, 3, 6, 6, 14 };
		E.W = CW[Variant] * XU * 0.85f; E.L = CL[Variant] * DU * 0.62f; E.Hp = CH[Variant]; E.bBig = Variant == 7;
		break;
	}
	case EFTKind::Boss: E.W = 64 * XU * 0.7f; E.L = 150 * DU * 0.6f; E.Hp = Def().BossHp; E.bBig = true; break;
	case EFTKind::Bullet: E.W = 40.f; E.L = 80.f; E.Hp = 1; break;
	case EFTKind::Missile: E.W = 60.f; E.L = 120.f; E.Hp = 1; break;
	default: break;
	}
	E.MaxHp = E.Hp;

	// proxy from the pool
	AFTEntityActor* A = nullptr;
	for (AFTEntityActor* P : Pool) if (P && P->IsHidden() && P->Kind == EFTKind::None) { A = P; break; }
	if (!A)
	{
		FActorSpawnParameters SP; SP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		A = GetWorld()->SpawnActor<AFTEntityActor>(AFTEntityActor::StaticClass(), FTransform::Identity, SP);
		Pool.Add(A);
	}
	A->Setup(K, Variant, Level);
	E.Proxy = A;
	A->SetActorTransform(EntTransform(E));
	ProxyKind.Add(E.Id, (int32)K * 16 + Variant);
	return E;
}

void AFTGameMode::Release(FFTEnt& E)
{
	if (AFTEntityActor* A = E.Proxy.Get())
	{
		A->SetActorHiddenInGame(true);
		A->Trail->Deactivate();
		A->Light->SetVisibility(false);
		A->Kind = EFTKind::None;
	}
	E.Proxy = nullptr;
	E.bDead = true;
}

void AFTGameMode::ClearEnts()
{
	for (FFTEnt& E : Ents) Release(E);
	for (FFTEnt& E : PendingEnts) Release(E);
	Ents.Reset();
	PendingEnts.Reset();
	for (FFTHazardState& Hz : Hazards) if (Hz.Actor.IsValid()) Hz.Actor->SetActorHiddenInGame(true);
	Hazards.Reset();
}

FTransform AFTGameMode::EntTransform(const FFTEnt& E) const
{
	float Yaw = 0.f, Roll = 0.f, Pitch = 0.f;
	const float Rel = FMath::Max(800.f, FMath::Abs(E.Sp));
	Yaw = FMath::RadiansToDegrees(FMath::Atan2(E.Vx, Rel)) * 0.8f;
	if (E.Kind == EFTKind::Moto || E.Kind == EFTKind::Hog) Roll = FMath::Clamp(E.Vx / 40.f, -28.f, 28.f);
	if (E.Kind == EFTKind::Missile) Yaw = FMath::RadiansToDegrees(E.Heading);
	if (E.Kind == EFTKind::Heli) { Pitch = -8.f; Roll = FMath::Clamp(E.Vx / 60.f, -20.f, 20.f); }
	if (E.Kind == EFTKind::Bullet && E.Owner == 1) Yaw = 180.f + FMath::RadiansToDegrees(FMath::Atan2(E.Vx, FMath::Max(1.f, Car->Speed - E.Sp)));
	const FTransform Base = Track->TransformAt(E.D, E.X, E.H);
	return FTransform(FRotator(Pitch, Yaw, Roll)) * Base;
}

void AFTGameMode::SpawnGroup(const TArray<EFTKind>& Kinds)
{
	float Stagger = 0.f;
	for (EFTKind K : Kinds)
	{
		const float Ahead = 15000.f + Stagger;
		Stagger += 90.f * DU * 1.4f;
		switch (K)
		{
		case EFTKind::Moto: { FFTEnt& E = Spawn(K, Ahead, LaneX(FMath::RandRange(0, 3))); E.Ta = R(170, 300) * DU; E.T1 = R(1.f, 2.f); E.Tx = E.X; break; }
		case EFTKind::Rammer: { FFTEnt& E = Spawn(K, Ahead, LaneX(FMath::RandRange(0, 3))); E.Ta = R(120, 190) * DU; break; }
		case EFTKind::Suv: { FFTEnt& E = Spawn(K, Ahead, LaneX(FMath::RandRange(0, 3))); E.Ta = R(90, 160) * DU; E.Tx = (FMath::RandBool() ? 60.f : -60.f) * XU; E.T1 = 2.3f; break; }
		case EFTKind::Truck: { FFTEnt& E = Spawn(K, Ahead, LaneX(FMath::RandRange(1, 2))); E.Ta = 110.f * DU * 1.4f; E.T1 = 3.f; E.T2 = 2.1f; break; }
		case EFTKind::Heli:
		{
			const int32 Side = FMath::RandBool() ? 1 : -1;
			FFTEnt& E = Spawn(K, 9000.f, Car->X + Side * R(60, 100) * XU);
			E.H = 2600.f; E.Ta = 215.f * DU * 1.3f; E.Side = Side; E.T1 = 1.3f;
			Say(TEXT("heliIn"), 2); Shout(TEXT("CHOPPER!"));
			break;
		}
		case EFTKind::Hog: { FFTEnt& E = Spawn(K, Ahead, LaneX(FMath::RandRange(0, 3)), FMath::RandRange(0, 1)); E.Ta = R(150, 270) * DU; E.Side = FMath::RandBool() ? 1 : -1; E.T2 = R(1.f, 2.f); break; }
		case EFTKind::Ramp: { FFTEnt& E = Spawn(K, 18000.f, LaneX(FMath::RandRange(1, 2))); E.Sp = 235.f * DU; break; }
		case EFTKind::Crate: case EFTKind::Wrench: { FFTEnt& E = Spawn(K, 16000.f + Stagger, LaneX(FMath::RandRange(0, 3))); E.Sp = 0.f; break; }
		case EFTKind::Boss:
		{
			FFTEnt& E = Spawn(EFTKind::Boss, 16000.f, 0.f);
			E.Ta = (Level == 2 ? 300.f : 342.f) * DU * 1.25f; E.T1 = 1.6f; E.T2 = 1.0f; E.T3 = 6.f;
			bBossSpawned = true; BossT = 0.f; bBossOn = true; BossLabel = Def().BossName;
			Sfx(TEXT("BossIntro")); Duck(0.35f, 1.6f);
			break;
		}
		default: break;
		}
	}
}

void AFTGameMode::TickSpawns()
{
	const FFTLevelDef& L = Def();
	const FFTScene& Sc = L.Scenes[SpawnScene];
	while (SpawnIdx < Sc.Spawns.Num() && Car->D >= Sc.Spawns[SpawnIdx].D * DU - 2000.f)
	{
		SpawnGroup(Sc.Spawns[SpawnIdx].Kinds);
		SpawnIdx++;
	}
	if (SpawnIdx >= Sc.Spawns.Num() && SpawnScene + 1 < L.Scenes.Num() && Car->D >= L.Scenes[SpawnScene + 1].D * DU - 2000.f)
	{
		SpawnScene++; SpawnIdx = 0;
	}
}

void AFTGameMode::TickCivilians(float Dt)
{
	CivT -= Dt;
	if (CivT > 0.f) return;
	CivT = R(0.9f, 2.2f) * (bBossSpawned ? 1.6f : 1.f);
	const int32 Lane = FMath::RandRange(0, 3);
	for (const FFTEnt& E : Ents)
		if (!E.bDead && FMath::Abs(E.X - LaneX(Lane)) < 40.f * XU && E.D - Car->D > 13000.f) return;
	const float r = FMath::FRand();
	const int32 V = r < 0.5f ? FMath::RandRange(0, 2) : (r < 0.75f ? FMath::RandRange(3, 4) : (r < 0.93f ? FMath::RandRange(5, 6) : 7));
	if (Track->FlagsAt(Car->D + 17000.f) & (FTFlag::Gap | FTFlag::Loop | FTFlag::Pipe | FTFlag::Twist)) return;
	FFTEnt& E = Spawn(EFTKind::Civ, 17000.f, LaneX(Lane), V);
	E.Sp = (V == 7 ? 190.f : R(200.f, 270.f)) * DU;
}

void AFTGameMode::FireBullet(int32 InOwner, float D, float X, float H, float VD, float VX)
{
	FFTEnt& B = Spawn(EFTKind::Bullet, D - Car->D, X);
	B.Owner = InOwner; B.H = H; B.Sp = VD; B.Vx = VX; B.T1 = InOwner == 0 ? 1.0f : 2.6f;
}

void AFTGameMode::FireMissile()
{
	FFTEnt& M = Spawn(EFTKind::Missile, 250.f, Car->X);
	M.H = 120.f; M.Owner = 0; M.T1 = 2.6f; M.Dmg = 260.f * DU; M.Heading = 0.f;
	Sfx(TEXT("Missile")); Duck(0.7f, 0.3f);
}

void AFTGameMode::DropPickup(const FFTEnt& E, EFTKind K)
{
	FFTEnt& P = Spawn(K, E.D - Car->D, FMath::Clamp(E.X, -RoadHalf + 200.f, RoadHalf - 200.f));
	P.Sp = 0.f;
}

void AFTGameMode::AddScore(int64 Pts, int32 Bucket)
{
	Score += Pts;
	switch (Bucket) { case 0: ScoreDestruction += Pts; break; case 1: ScoreNear += Pts; break; case 2: ScoreStunts += Pts; break; case 3: ScoreCollateral += Pts; break; }
}

void AFTGameMode::HurtEnt(FFTEnt& E, float Dmg, int32 Cause)
{
	if (E.bDead) return;
	E.Hp -= Dmg;
	E.bHit = true;
	if (E.Hp <= 0.f) Kill(E, Cause);
	else Sfx(TEXT("Hit"), 0.4f);
}

void AFTGameMode::Kill(FFTEnt& E, int32 Cause)
{
	if (E.bDead) return;
	const FTransform T = EntTransform(E);
	const FVector At = T.GetLocation();
	int32 Pts = 0; float ExSize = 1.f; FString Label;
	switch (E.Kind)
	{
	case EFTKind::Moto: Pts = 150; Label = TEXT("BIKER DOWN"); if (FMath::FRand() < 0.15f) DropPickup(E, EFTKind::Wrench); break;
	case EFTKind::Rammer: Pts = 250; Label = TEXT("RAMMER WRECKED"); ExSize = 1.3f; if (FMath::FRand() < 0.25f) DropPickup(E, EFTKind::Crate); break;
	case EFTKind::Suv: Pts = 400; Label = TEXT("GUNSHIP DOWN"); ExSize = 1.5f; DropPickup(E, EFTKind::Crate); break;
	case EFTKind::Truck: Pts = 900; Label = TEXT("ARMOR CRACKED"); ExSize = 2.f; DropPickup(E, EFTKind::Wrench); DropPickup(E, EFTKind::Crate); Say(TEXT("bigKill"), 2); break;
	case EFTKind::Heli: Pts = 1000; Label = TEXT("CHOPPER DOWN"); ExSize = 2.4f; DropPickup(E, EFTKind::Crate); DropPickup(E, EFTKind::Wrench); Say(TEXT("heliDown"), 2); Shout(TEXT("CHOPPER DOWN!")); Sfx(TEXT("HeliDown")); break;
	case EFTKind::Hog: Pts = 300; Label = TEXT("SNAKE EYES DOWN"); if (FMath::FRand() < 0.1f) DropPickup(E, EFTKind::Wrench); else if (FMath::FRand() < 0.13f) DropPickup(E, EFTKind::Crate); break;
	case EFTKind::Henchman: Pts = 200; ExSize = 0.5f; break;
	case EFTKind::Barrel: Pts = 50; ExSize = 1.2f;
		for (FFTEnt& O : Ents) if (&O != &E && !O.bDead && O.Kind != EFTKind::Bullet && O.Kind != EFTKind::Missile && FMath::Abs(O.X - E.X) < 60 * XU && FMath::Abs(O.D - E.D) < 60 * DU) HurtEnt(O, 4.f, 3);
		if (FMath::Abs(Car->X - E.X) < 34 * XU + CarW / 2 && FMath::Abs(Car->D - E.D) < 40 * DU + CarL / 2) HurtPlayer(10.f);
		break;
	case EFTKind::Civ: Pts = -300; ExSize = E.bBig ? 1.6f : 1.2f; Collateral++; Say(TEXT("collateral"), 2, 3.f); Caption = TEXT("COLLATERAL!"); CaptionT = RealClock; Sfx(TEXT("Collateral")); break;
	case EFTKind::Boss: Pts = 3000; ExSize = 3.f; Label = Def().BossName + TEXT(" DOWN"); bBossDead = true; break;
	default: break;
	}
	E.bDead = true;
	if (E.Kind == EFTKind::Bullet || E.Kind == EFTKind::Missile) { Release(E); return; }

	// Chaos destruction: barrels, drums and crates burst into fractured geometry-collection debris,
	// and an armoured truck spills its cargo crates
	auto Shatter = [this](const TCHAR* Path, const FTransform& At, const FVector& Vel, float Strength)
	{
		UGeometryCollection* GC = Load<UGeometryCollection>(Path);
		if (!GC) return;
		AGeometryCollectionActor* A = GetWorld()->SpawnActorDeferred<AGeometryCollectionActor>(AGeometryCollectionActor::StaticClass(), At, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (!A) return;
		UGeometryCollectionComponent* C = A->GetGeometryCollectionComponent();
		C->SetRestCollection(GC);
		C->ObjectType = EObjectStateTypeEnum::Chaos_Object_Dynamic;
		C->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		UGameplayStatics::FinishSpawningActor(A, At);
		C->CrumbleActiveClusters();
		C->AddImpulse(Vel, NAME_None, true);
		C->AddRadialImpulse(At.GetLocation() - FVector(0, 0, 60.f), 400.f, Strength, ERadialImpulseFalloff::RIF_Linear, true);
		A->SetLifeSpan(7.f);
	};
	{
		const FFTFrame Fr = Track->Sample(E.D);
		const FVector Vel = Fr.Fwd * E.Sp;
		if (E.Kind == EFTKind::Barrel) Shatter(Level == 1 ? TEXT("/Game/FX/Destruction/GC_DrumGreen") : TEXT("/Game/FX/Destruction/GC_Barrel"), T, Vel, 1400.f);
		if (E.Kind == EFTKind::Truck)
			for (int32 k = 0; k < 3; ++k)
				Shatter(TEXT("/Game/FX/Destruction/GC_Crate"), FTransform(FRotator(0.f, k * 30.f, 0.f), T.GetLocation() + Fr.Up * (180.f + k * 120.f) - Fr.Fwd * (300.f + k * 90.f)), Vel * 0.8f, 1100.f);
	}

	Explode(At, ExSize);
	if (E.Kind == EFTKind::Heli)
	{
		FTimerHandle H; const FVector A2 = At + FVector(0, 0, -800.f);
		GetWorldTimerManager().SetTimer(H, FTimerDelegate::CreateWeakLambda(this, [this, A2]() { Explode(A2, 1.8f); }), 0.7f, false);
	}
	// the burning wreck: a real rigid body
	if (E.Kind != EFTKind::Barrel && E.Kind != EFTKind::Henchman)
	{
		if (UStaticMesh* M = AFTEntityActor::MeshFor(E.Kind, E.Variant, Level))
		{
			FActorSpawnParameters SP; SP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			if (AFTWreck* W = GetWorld()->SpawnActor<AFTWreck>(AFTWreck::StaticClass(), T, SP))
			{
				const FFTFrame F = Track->Sample(E.D);
				const FVector V = F.Fwd * E.Sp * 0.85f + F.Up * R(500.f, 1100.f) * (E.bBig ? 0.6f : 1.f) + F.Right * (E.Vx + R(-400.f, 400.f));
				W->Launch(M, T, V, FVector(R(-200, 200), R(-260, 260), R(-150, 150)) * (E.bBig ? 0.4f : 1.f));
			}
		}
	}
	if (Pts < 0) { AddScore(Pts, 3); }
	else if (Pts > 0 && E.Kind != EFTKind::Boss)
	{
		Kills++;
		AddScore((int64)Pts * Combo, E.Kind == EFTKind::Barrel ? 0 : 0);
		Popup(FString::Printf(TEXT("+%d"), Pts * Combo), CYellow);
		Combo = FMath::Min(5, Combo + 1); ComboT = 3.f;
		// multi kills
		if (Clock - LastKillT < 0.7f) MultiKills++; else MultiKills = 1;
		LastKillT = Clock;
		if (MultiKills == 2) Stunt(TEXT("DOUBLE KILL"), 300, TEXT("Two at once! Print it!"), 5.f);
		else if (MultiKills == 3) Stunt(TEXT("TRIPLE KILL"), 600, TEXT("Three! I'm crying!"), 8.f);
		else if (MultiKills >= 4) Stunt(TEXT("MASSACRE"), 300 * (MultiKills - 1), TEXT("MASSACRE! Somebody call the Academy!"), 10.f);
		const bool bInv = (Track->FlagsAt(Car->D) & FTFlag::Inverted) != 0;
		bool bSpectacular = E.bBig;
		if (!Car->bGrounded && E.Kind != EFTKind::Barrel) { Stunt(TEXT("AIRBORNE KILL"), 500, TEXT("From the AIR?! Genius!"), 6.f); bSpectacular = true; }
		if (bInv) { Stunt(TEXT("UPSIDE-DOWN KILL"), 600, TEXT("Upside down?! That's cinema!"), 6.f); bSpectacular = true; }
		if (Cause == 2 && bHot) { Stunt(TEXT("TURBO RAM"), 300, TEXT("Ram him! YES!"), 4.f); bSpectacular = true; }
		if (bSpectacular) { Slowmo(0.7f, 0.3f, 1.2f); Freeze(0.09f); }
		else Say(TEXT("kill"), 1, 2.2f);
		static const TMap<EFTKind, float> RS = { { EFTKind::Heli, 9.f }, { EFTKind::Truck, 7.f }, { EFTKind::Suv, 5.f }, { EFTKind::Rammer, 3.f }, { EFTKind::Hog, 3.f }, { EFTKind::Moto, 2.f } };
		if (const float* S = RS.Find(E.Kind)) ReelEvent(*S, Label, At);
	}
	if (E.Kind == EFTKind::Boss)
	{
		AddScore(3000, 0);
		ReelEvent(100.f, Label, At, true);
		bBossOn = false;
		Wrap(false);
	}
	Release(E);
}

void AFTGameMode::Explode(const FVector& At, float Size)
{
	if (UNiagaraSystem* S = Load<UNiagaraSystem>(TEXT("/Game/FX/NS_Explosion")))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, S, At, FRotator::ZeroRotator, FVector(Size), true, true, ENCPoolMethod::AutoRelease);
	}
	for (const TCHAR* Extra : { TEXT("/Game/FX/NS_ExplosionSparks"), TEXT("/Game/FX/NS_ExplosionSmoke") })
		if (UNiagaraSystem* S2 = Load<UNiagaraSystem>(Extra))
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, S2, At, FRotator::ZeroRotator, FVector(FMath::Max(0.6f, Size)), true, true, ENCPoolMethod::AutoRelease);
	UPointLightComponent* Best = Flashes[0];
	for (UPointLightComponent* L : Flashes) if (L->Intensity < Best->Intensity) Best = L;
	Best->SetWorldLocation(At + FVector(0, 0, 250.f));
	Best->SetAttenuationRadius(2500.f + 1500.f * Size);
	Best->SetIntensity(40000.f * Size);
	Sfx(TEXT("Explode"), FMath::Clamp(0.5f + Size * 0.25f, 0.5f, 1.2f), R(0.85f, 1.05f) / FMath::Sqrt(FMath::Max(Size, 0.5f)));
	Duck(0.55f, 0.25f);
	const float Dist = FVector::Dist(At, Car->GetActorLocation());
	Shake(FMath::Clamp(Size * 1.2f * (1.f - Dist / 9000.f), 0.f, 2.f));
}

void AFTGameMode::TickEnts(float Dt)
{
	Ents.Append(MoveTemp(PendingEnts));
	PendingEnts.Reset();
	const float Vp = Car->Speed;
	const float Px = Car->X;
	const float Pd = Car->D;
	const bool bGround = Car->bGrounded;
	FFTEnt* Boss = nullptr;
	int32 HogsOut = 0;
	for (FFTEnt& E : Ents) if (!E.bDead && E.Kind == EFTKind::Hog) HogsOut++;
	bool bRotor = false; float RotorDist = 1e9f;

	for (int32 i = 0; i < Ents.Num(); ++i)
	{
		FFTEnt& E = Ents[i];
		if (E.bDead) continue;
		E.T += Dt;
		const float PrevX = E.X;
		float Ahead = E.D - Pd;

		switch (E.Kind)
		{
		case EFTKind::Moto:
		{
			if (E.State == 0) { E.Sp = Vp - 210.f * DU; if (Ahead <= E.Ta + 300.f) E.State = 1; }
			else if (E.State == 1)
			{
				const float Want = E.Ta + 40.f * DU * FMath::Sin(1.3f * E.T);
				E.Sp = Vp + FMath::Clamp((Want - Ahead) * 2.f, -3000.f, 3000.f);
				E.Tx = FMath::FInterpConstantTo(E.Tx, Px, Dt, 55.f * XU);
				E.X = FMath::Clamp(E.Tx + 52.f * XU * FMath::Sin(2.7f * E.T), -RoadHalf, RoadHalf);
				E.T1 -= Dt;
				if (E.T1 <= 0.f && Ahead > 80.f * DU)
				{
					E.T1 = 1.9f;
					const FVector2D Dir = FVector2D(-Ahead, Px - E.X).GetSafeNormal();
					FireBullet(1, E.D - 200.f, E.X, 110.f, Vp + Dir.X * 330.f * DU, Dir.Y * 330.f * DU);
					Sfx(TEXT("EShot"), 0.5f);
				}
				if (E.T > 9.f) E.State = 2;
			}
			else E.Sp = Vp - 300.f * DU;
			break;
		}
		case EFTKind::Rammer:
		{
			if (E.State == 0) { E.Sp = Vp - 220.f * DU; if (Ahead <= E.Ta) { E.State = 1; E.T1 = 1.1f; } }
			else if (E.State == 1)
			{
				E.Sp = Vp + (E.Ta - Ahead) * 2.f;
				E.X = FMath::FInterpConstantTo(E.X, Px, Dt, 170.f * XU);
				E.T1 -= Dt;
				if (E.T1 <= 0.f) { E.State = 2; E.T1 = 0.5f; Say(TEXT("rev"), 2, 4.f); Sfx(TEXT("Rev")); }
			}
			else if (E.State == 2)
			{
				E.Sp = Vp + 40.f * DU; E.T1 -= Dt;
				E.Glow = FMath::Fmod(E.T, 0.16f) < 0.08f ? 1.f : 0.f;
				if (E.T1 <= 0.f) { E.State = 3; E.T2 = 0.f; E.Glow = 1.f; }
			}
			else
			{
				E.T2 = FMath::Min(560.f * DU, E.T2 + 1600.f * DU * Dt);
				E.Sp = Vp - E.T2;
				E.X = FMath::FInterpTo(E.X, Px, Dt, 3.f);
				// a charging rammer wrecks the traffic, and dies on a big vehicle
				for (FFTEnt& O : Ents)
				{
					if (&O == &E || O.bDead || (O.Kind != EFTKind::Civ && O.Kind != EFTKind::Truck && O.Kind != EFTKind::Ramp)) continue;
					if (FMath::Abs(O.X - E.X) < (O.W + E.W) / 2 && FMath::Abs(O.D - E.D) < (O.L + E.L) / 2)
					{
						if (O.bBig || O.Kind == EFTKind::Truck || O.Kind == EFTKind::Ramp) { Kill(E, 3); break; }
						Kill(O, 3);
					}
				}
			}
			break;
		}
		case EFTKind::Suv:
		{
			if (E.State == 0) { E.Sp = Vp - 210.f * DU; if (Ahead <= E.Ta) E.State = 1; }
			else if (E.State == 1)
			{
				E.Sp = Vp + (E.Ta - Ahead) * 2.f;
				E.T3 -= Dt;
				if (E.T3 <= 0.f) { E.T3 = 2.f; E.Tx = (FMath::RandBool() ? 60.f : -60.f) * XU; }
				E.X = FMath::Clamp(FMath::FInterpConstantTo(E.X, Px + E.Tx, Dt, 90.f * XU), -RoadHalf, RoadHalf);
				E.T1 -= Dt;
				if (E.T1 <= 0.f) { E.Count = 3; E.T1 = 2.3f; E.T2 = 0.f; }
				if (E.Count > 0)
				{
					E.T2 -= Dt;
					if (E.T2 <= 0.f)
					{
						E.T2 = 0.13f; E.Count--;
						FVector2D Dir = FVector2D(-Ahead, Px - E.X).GetSafeNormal();
						Dir = Dir.GetRotated(R(-3.4f, 3.4f));
						FireBullet(1, E.D - E.L / 2, E.X, 140.f, Vp + Dir.X * 360.f * DU, Dir.Y * 360.f * DU);
						Sfx(TEXT("EShot"), 0.6f);
					}
				}
				if (E.T > 22.f) E.State = 2;
			}
			else E.Sp = Vp + 260.f * DU;
			break;
		}
		case EFTKind::Truck:
		{
			if (E.State == 0) { E.Sp = Vp - 100.f * DU * 1.6f; if (Ahead <= E.Ta) E.State = 1; }
			else if (E.State == 1)
			{
				E.Sp = Vp + (E.Ta - Ahead) * 1.5f;
				E.T1 -= Dt;
				if (E.T1 <= 0.f) { E.T1 = 3.f; int32 Best = 0; for (int32 l = 1; l < 4; ++l) if (FMath::Abs(LaneX(l) - Px) < FMath::Abs(LaneX(Best) - Px)) Best = l; E.Tx = LaneX(Best); }
				E.X = FMath::FInterpConstantTo(E.X, E.Tx, Dt, 40.f * XU);
				E.T2 -= Dt;
				if (E.T2 <= 0.f)
				{
					E.T2 = 2.1f;
					FFTEnt& B = Spawn(EFTKind::Barrel, (E.D - E.L / 2 - 150.f) - Pd, E.X);
					B.Sp = 0.f;
				}
				if (E.T > 26.f) E.State = 2;
			}
			else E.Sp = Vp + 200.f * DU;
			break;
		}
		case EFTKind::Heli:
		{
			bRotor = true; RotorDist = FMath::Min(RotorDist, FMath::Abs(Ahead));
			if (E.State == 0)
			{
				E.Sp = Vp - 240.f * DU * 0.8f;
				E.H = FMath::FInterpTo(E.H, 1100.f, Dt, 1.2f);
				if (Ahead <= E.Ta) { E.State = 1; }
			}
			else if (E.State == 1)
			{
				E.Sp = Vp + (E.Ta - Ahead) * 1.6f;
				E.H = FMath::FInterpTo(E.H, 1000.f + 120.f * FMath::Sin(E.T * 1.3f), Dt, 1.5f);
				E.X = FMath::FInterpConstantTo(E.X, Px + E.Side * 30.f * XU, Dt, 120.f * XU);
				E.T1 -= Dt;
				if (E.T1 <= 0.f && E.Count > 0)
				{
					const int32 DoorSide = E.Step % 2 == 0 ? (E.Side > 0 ? 1 : 0) : (E.Side > 0 ? 0 : 1);
					bool bPending = false;
					for (const FFTEnt& O : Ents) if (!O.bDead && O.Kind == EFTKind::Henchman && O.Side == DoorSide) bPending = true;
					if (!HenchOn[DoorSide] && !bPending && bGround)
					{
						FFTEnt& Hn = Spawn(EFTKind::Henchman, Ahead - 200.f, E.X);
						Hn.H = E.H - 300.f; Hn.Side = DoorSide; Hn.T1 = 1.25f; Hn.Tx = Px; Hn.Ta = Ahead;
						E.Count--; E.Step++;
						Sfx(TEXT("HenchDrop"));
						E.T1 = 2.3f;
						if (E.Count == 0) E.T2 = 1.4f;
					}
					else E.T1 = 0.3f;
				}
				if ((E.Count == 0 && (E.T2 -= Dt) <= 0.f) || E.T > 26.f) E.State = 2;
			}
			else { E.Sp = Vp + 500.f * DU; E.H += 600.f * Dt; if (Ahead > 30000.f) Release(E); }
			break;
		}
		case EFTKind::Henchman:
		{
			// riding the rope down; the landing mark chases the car
			E.T1 -= Dt;
			E.Tx = FMath::FInterpConstantTo(E.Tx, Px, Dt, 95.f * XU);
			E.Ta = FMath::FInterpConstantTo(E.Ta, 0.f, Dt, 170.f * DU);
			E.Sp = Vp + (E.Ta - Ahead) * 4.f;
			E.X = E.Tx + (E.Side == 0 ? -1.f : 1.f) * 120.f;
			E.H = FMath::Max(100.f, E.H - 900.f * Dt);
			if (E.T1 <= 0.f)
			{
				if (FMath::Abs(E.X - Px) < 22.f * XU + 140.f && FMath::Abs(E.D - Pd) < 30.f * DU + 200.f && !HenchOn[E.Side] && Phase == EFTPhase::Play)
				{
					HenchOn[E.Side] = 1; HenchPunch[E.Side] = R(1.1f, 1.5f); HenchHangT = 0.f;
					Sfx(TEXT("Latch")); Duck(0.7f, 0.3f);
					if (HenchOn[0] && HenchOn[1]) Say(TEXT("latch2"), 2); else Say(TEXT("latch"), 2);
					Shout(TEXT("HE'S ON THE CAR!"));
					ReelEvent(3.f, TEXT("PASSENGER"), Car->GetActorLocation());
				}
				else { AddScore(150, 2); Popup(TEXT("DODGED THE DROP +150"), CCyan); Say(TEXT("dropDodged"), 1); }
				Release(E);
			}
			break;
		}
		case EFTKind::Hog:
		{
			if (E.State == 0) { E.Sp = Vp - 200.f * DU; if (Ahead <= E.Ta) E.State = 1; }
			else if (E.State == 9) // rolling off the carrier ramp
			{
				E.Sp = Vp - 140.f * DU; E.X += E.Vh * Dt; E.T1 -= Dt; if (E.T1 <= 0.f) { E.State = 1; }
			}
			else if (E.State == 8) // flying off the carrier's top deck at the car
			{
				E.T1 += Dt;
				const float U = FMath::Clamp(E.T1 / 1.15f, 0.f, 1.f);
				E.Tx = FMath::FInterpConstantTo(E.Tx, Px, Dt, 80.f * XU);
				E.Ta = FMath::FInterpConstantTo(E.Ta, 0.f, Dt, 120.f * DU * 1.4f);
				E.Sp = Vp + (E.Ta - Ahead) * 4.f;
				E.X = FMath::Lerp(E.X, E.Tx, FMath::Min(1.f, Dt * 4.f));
				E.H = FMath::Lerp(34.f * XU * 1.6f, 0.f, U) + 46.f * XU * 2.f * FMath::Sin(PI * U);
				if (U >= 1.f)
				{
					E.H = 0.f; E.State = 1; E.bAir = false;
					if (FMath::Abs(E.X - Px) < 20.f * XU + 150.f && FMath::Abs(E.D - Pd) < 34.f * DU + 200.f) { HurtPlayer(12.f); Sfx(TEXT("HogLand")); }
					else { AddScore(150, 2); Popup(TEXT("DODGED THE JUMP +150"), CCyan); }
				}
			}
			else if (E.State == 1 || E.State == 2 || E.State == 3)
			{
				// ride beside a door
				float Want = Px + E.Side * (CarW / 2 + E.W / 2 + 30.f * XU);
				if (FMath::Abs(Want) > RoadHalf) { E.Side = -E.Side; Want = Px + E.Side * (CarW / 2 + E.W / 2 + 30.f * XU); }
				E.X = FMath::FInterpConstantTo(E.X, FMath::Clamp(Want, -RoadHalf, RoadHalf), Dt, 140.f * XU);
				E.Sp = Vp + FMath::Clamp((0.f - Ahead) * 2.f, -2500.f, 2500.f);
				E.T2 -= Dt;
				if (E.State == 1 && E.T2 <= 0.f && FMath::Abs(Ahead) < 34.f * DU + 200.f && FMath::Abs(E.X - Px) < 64.f * XU)
				{
					E.State = 2; E.T1 = 0.6f; Say(TEXT("chainWarn"), 2); Shout(TEXT("CHAIN!")); Sfx(TEXT("ChainWarn"));
				}
				if (E.State == 2)
				{
					E.Glow = 1.f; E.T1 -= Dt;
					if (E.T1 <= 0.f)
					{
						E.Glow = 0.f; E.State = 1; E.T2 = R(1.8f, 2.6f);
						if (FMath::Abs(E.X - Px) < 56.f * XU && FMath::Abs(Ahead) < 36.f * DU + 200.f) { HurtPlayer(7.f); Say(TEXT("chainHit"), 2); Sfx(TEXT("Chain")); }
						else { AddScore(150, 2); Popup(TEXT("DUCKED THE CHAIN +150"), CCyan); Say(TEXT("chainDodged"), 1); }
					}
				}
				// slam it off the bike
				const float Toward = (E.X - Px) * Car->LatSpeed;
				if (FMath::Abs(E.X - Px) < (CarW + E.W) / 2 + 40.f && FMath::Abs(Ahead) < (CarL + E.L) / 2 && ((Toward > 0.f && FMath::Abs(Car->LatSpeed) > 40.f * XU) || bHot))
				{
					Car->Shove(Track->Sample(Pd).Right, -Car->LatSpeed * 0.4f);
					Stunt(TEXT("BIKER SLAM"), 300, TEXT("Off the bike! Ha! The stuntman earned his pay!"), 6.f);
					HurtEnt(E, 3.f, 2);
					Sfx(TEXT("HogOff"));
					break;
				}
				if (E.T > 16.f) E.State = 4;
			}
			else E.Sp = Vp - 260.f * DU;
			break;
		}
		case EFTKind::Ramp: case EFTKind::Civ: break;
		case EFTKind::Crate: case EFTKind::Wrench: case EFTKind::Barrel: E.Sp = 0.f; break;
		case EFTKind::Bullet:
		{
			E.T1 -= Dt;
			E.X += E.Vx * Dt;
			if (E.T1 <= 0.f) { Release(E); break; }
			if (E.Owner == 0)
			{
				for (FFTEnt& O : Ents)
				{
					if (O.bDead || &O == &E || O.Kind == EFTKind::Bullet || O.Kind == EFTKind::Missile || O.Kind == EFTKind::Crate || O.Kind == EFTKind::Wrench || O.Kind == EFTKind::Henchman || O.Kind == EFTKind::Ramp) continue;
					const float Sweep = FMath::Abs(E.Sp - O.Sp) * Dt;
					if (FMath::Abs(O.X - E.X) < O.W / 2 + 40.f && E.D > O.D - O.L / 2 - Sweep && E.D < O.D + O.L / 2 + 50.f)
					{
						if (UNiagaraSystem* S = Load<UNiagaraSystem>(TEXT("/Game/FX/NS_Impact")))
							UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, S, Track->ToWorld(E.D, E.X, O.bAir ? O.H : 110.f), FRotator::ZeroRotator, FVector(1), true, true, ENCPoolMethod::AutoRelease);
						HurtEnt(O, 1.f, 0);
						Release(E);
						break;
					}
				}
			}
			else if (FMath::Abs(E.X - Px) < CarW / 2 && FMath::Abs(E.D - Pd) < CarL / 2 + FMath::Abs(E.Sp - Vp) * Dt)
			{
				HurtPlayer(6.f);
				Release(E);
			}
			break;
		}
		case EFTKind::Missile:
		{
			E.T1 -= Dt;
			if (E.T1 <= 0.f) { Explode(EntTransform(E).GetLocation(), 0.6f); Release(E); break; }
			E.Dmg = FMath::Min(980.f * DU, E.Dmg + 1500.f * DU * Dt);
			// home on the foe ahead with the lowest 2|dx| + dy
			FFTEnt* Tgt = nullptr; float Best = 1e12f;
			for (FFTEnt& O : Ents)
			{
				if (O.bDead || O.Kind == EFTKind::Bullet || O.Kind == EFTKind::Missile || O.Kind == EFTKind::Crate || O.Kind == EFTKind::Wrench || O.Kind == EFTKind::Civ || O.Kind == EFTKind::Henchman || O.Kind == EFTKind::Ramp || O.Kind == EFTKind::Barrel) continue;
				const float Dy = O.D - E.D;
				if (Dy < -200.f) continue;
				const float Sc = 2.f * FMath::Abs(O.X - E.X) / XU + Dy / DU;
				if (Sc < Best) { Best = Sc; Tgt = &O; }
			}
			if (Tgt)
			{
				const float Want = FMath::Atan2(Tgt->X - E.X, FMath::Max(100.f, Tgt->D - E.D));
				E.Heading = FMath::Clamp(FMath::FInterpConstantTo(E.Heading, Want, Dt, 4.2f), -1.1f, 1.1f);
				if (Tgt->bAir) E.H = FMath::FInterpTo(E.H, Tgt->H, Dt, 3.f);
			}
			E.Sp = Vp + E.Dmg * FMath::Cos(E.Heading);
			E.X += E.Dmg * FMath::Sin(E.Heading) * Dt;
			for (FFTEnt& O : Ents)
			{
				if (O.bDead || &O == &E || O.Kind == EFTKind::Bullet || O.Kind == EFTKind::Missile || O.Kind == EFTKind::Crate || O.Kind == EFTKind::Wrench || O.Kind == EFTKind::Ramp) continue;
				if (FMath::Abs(O.X - E.X) < O.W / 2 + 60.f && FMath::Abs(O.D - E.D) < O.L / 2 + 120.f)
				{
					const float Mx = E.X, Md = E.D;
					Explode(EntTransform(E).GetLocation(), 0.9f);
					HurtEnt(O, 9.f, 1);
					for (FFTEnt& S : Ents)
						if (!S.bDead && &S != &O && S.Kind != EFTKind::Bullet && S.Kind != EFTKind::Missile && S.Kind != EFTKind::Crate && S.Kind != EFTKind::Wrench && S.Kind != EFTKind::Ramp
							&& FMath::Abs(S.X - Mx) < 46.f * XU + S.W / 2 && FMath::Abs(S.D - Md) < 46.f * DU + S.L / 2) HurtEnt(S, 3.f, 1);
					Release(E);
					break;
				}
			}
			break;
		}
		case EFTKind::Boss: Boss = &E; TickBoss(E, Dt); break;
		default: break;
		}
		if (E.bDead) continue;

		E.D += E.Sp * Dt;
		E.Vx = (E.X - PrevX) / FMath::Max(Dt, 1e-4f);
		Ahead = E.D - Pd;

		// player contact (ground vehicles and pickups)
		if (Phase == EFTPhase::Play && !E.bAir && E.Kind != EFTKind::Bullet && E.Kind != EFTKind::Missile && E.H < 300.f)
		{
			const float Dx = E.X - Px;
			const bool bTouch = FMath::Abs(Dx) < (CarW + E.W) / 2 && FMath::Abs(Ahead) < (CarL + E.L) / 2;
			if (bTouch)
			{
				if (E.Kind == EFTKind::Crate) { Missiles = FMath::Min(9, Missiles + 3); Popup(TEXT("+3 MISSILES"), FLinearColor(1.f, 0.6f, 0.2f)); Say(TEXT("pickupMissiles"), 1); Sfx(TEXT("Pickup")); Release(E); continue; }
				if (E.Kind == EFTKind::Wrench) { Hp = FMath::Min(100.f, Hp + 30.f); Car->RestoreParts(); PartsShed = 0; for (int32 p = 0; p < 7; ++p) if (Hp < 88.f - 12.f * p) { Car->ShedPart(p); PartsShed = p + 1; } Car->SetDamageLook(Hp); PrevHp = Hp; Popup(TEXT("+30 ARMOR"), CGreen); Say(TEXT("pickupRepair"), 1); Sfx(TEXT("Pickup")); Release(E); continue; }
				if (E.Kind == EFTKind::Barrel) { HurtPlayer(14.f); Kill(E, 2); continue; }
				if (E.Kind == EFTKind::Ramp)
				{
					if (Ahead > 0.f && bGround && RampAirT <= 0.f)
					{
						Car->Shove(Car->RoadUp, bHot ? 1350.f : 950.f);
						RampAirT = 0.3f;
						Stunt(bHot ? TEXT("TURBO JUMP") : TEXT("RAMP JUMP"), bHot ? 700 : 400, bHot ? TEXT("Look at that AIR! Fly!") : TEXT("FLY! Fly, Kane!"), bHot ? 5.f : 3.f);
						Shout(TEXT("FLY!"));
						Slowmo(0.6f, 0.4f);
						Sfx(TEXT("Jump"));
					}
					continue;
				}
				// roof landing: drop onto a foe from the air
				if (!bGround && Car->H > 120.f && E.Kind != EFTKind::Boss)
				{
					Stunt(TEXT("ROOF LANDING"), 1000, TEXT("On the ROOF! Legendary!"), 9.f);
					Kill(E, 2);
					continue;
				}
				if (Invuln <= 0.f && bGround)
				{
					const float Push = (Dx > 0.f ? -1.f : 1.f) * 650.f;
					// henchman sideswipe: hit something with the side he's riding on
					const int32 HitSide = Dx > 0.f ? 1 : 0;
					if (HenchOn[HitSide] && E.Kind != EFTKind::Boss)
					{
						HenchOn[HitSide] = 0;
						Stunt(TEXT("SIDESWIPE"), 400, TEXT("Wiped him off on that car! Genius!"), 6.f);
						AddScore(200 * Combo, 2);
						Slowmo(0.55f, 0.35f);
					}
					if (E.Kind == EFTKind::Boss)
					{
						HurtPlayer(10.f, nullptr, Push);
						Car->Shove(-Car->GetActorForwardVector(), 900.f);
					}
					else if (bHot)
					{
						HurtPlayer(3.f, nullptr, Push * 0.5f);
						HurtEnt(E, E.bBig ? 4.f : 6.f, 2);
						if (E.Kind == EFTKind::Civ && E.bDead) {}
					}
					else
					{
						const float Dmg = (E.Kind == EFTKind::Rammer && E.State == 3) ? 18.f : ((E.bBig || E.Kind == EFTKind::Truck) ? 14.f : 9.f);
						HurtPlayer(Dmg, nullptr, Push);
						Sfx(TEXT("Bump"));
						if (E.Kind == EFTKind::Civ) HurtEnt(E, 1.f, 2);
						if (E.Kind == EFTKind::Rammer && E.State == 3) { HurtEnt(E, 2.f, 2); E.State = 1; E.T1 = 1.5f; E.Sp = Vp; }
					}
					if (!E.bDead) { E.X -= Push * 0.25f; E.bNoNear = true; }
				}
			}
			else if (!E.bNoNear && (E.Kind == EFTKind::Civ || E.Kind == EFTKind::Moto || E.Kind == EFTKind::Rammer || E.Kind == EFTKind::Suv || E.Kind == EFTKind::Truck || E.Kind == EFTKind::Hog) && E.PrevAhead > 0.f && Ahead <= 0.f && bGround)
			{
				const float EdgeGap = (FMath::Abs(Dx) - (CarW + E.W) / 2) / XU;
				if (EdgeGap > -2.f && EdgeGap < 14.f)
				{
					NearMisses++;
					const int32 Pts = 100 * (bHot ? 2 : 1) * Combo;
					AddScore(Pts, 1);
					Popup(FString::Printf(TEXT("NEAR MISS +%d"), Pts), CCyan);
					Sfx(TEXT("Near"));
					Say(TEXT("near"), 1, 3.5f);
					ComboT = 3.f;
					const int32 Side = Dx > 0.f ? 1 : -1;
					if (Clock - LastNearT < 0.45f && Side != LastNearSide) Stunt(TEXT("THREAD THE NEEDLE"), 400, TEXT("Through the gap! Perfect!"), 5.f);
					LastNearT = Clock; LastNearSide = Side;
					NoNearT = 0.f;
					if (EdgeGap < 7.f || bHot || E.Kind != EFTKind::Civ) Slowmo(0.45f, 0.35f, 2.4f);
					ReelEvent(2.f, TEXT("NEAR MISS"), Car->GetActorLocation());
				}
			}
		}
		E.PrevAhead = Ahead;
		E.bHit = false;

		// gone
		if (Ahead < -12000.f || Ahead > 45000.f) { Release(E); continue; }

		if (AFTEntityActor* A = E.Proxy.Get())
		{
			A->SetActorTransform(EntTransform(E));
			A->Animate(Dt, E.Sp);
			if (E.Glow > 0.f || E.Kind == EFTKind::Rammer || E.Kind == EFTKind::Hog || E.Kind == EFTKind::Boss) A->SetGlow(E.Glow);
		}
	}
	Ents.RemoveAll([](const FFTEnt& E) { return E.bDead; });
	RampAirT -= Dt;

	if (Rotor)
	{
		Rotor->SetVolumeMultiplier(bRotor ? FMath::Clamp(1.f - RotorDist / 25000.f, 0.f, 1.f) * 0.8f : FMath::FInterpTo(Rotor->VolumeMultiplier, 0.f, Dt, 3.f));
	}
}

void AFTGameMode::TickBoss(FFTEnt& B, float Dt)
{
	const float Vp = Car->Speed, Px = Car->X, Pd = Car->D;
	const float Ahead = B.D - Pd;
	BossT += Dt;
	BossHpFrac = FMath::Clamp(B.Hp / B.MaxHp, 0.f, 1.f);
	const int32 Ph = BossHpFrac > 0.66f ? 0 : (BossHpFrac > 0.33f ? 1 : 2);
	float Want = B.Ta;
	if (Level == 0)
	{
		Want += 26.f * DU * FMath::Sin(0.7f * B.T);
		B.X = FMath::FInterpTo(B.X, 74.f * XU * FMath::Sin(0.55f * B.T), Dt, 2.f);
		B.T1 -= Dt;
		if (B.T1 <= 0.f && Ahead < B.Ta + 2000.f)
		{
			const bool bHard = BossHpFrac < 0.5f;
			B.T1 = bHard ? 1.5f : 2.1f;
			const int32 N = bHard ? 7 : 5;
			for (int32 k = 0; k < N; ++k)
			{
				const float A = (k - (N - 1) * 0.5f) * 0.17f;
				FireBullet(1, B.D - B.L / 2, B.X, 200.f, Vp - FMath::Cos(A) * 300.f * DU, FMath::Sin(A) * 300.f * DU);
			}
			Sfx(TEXT("EShotBig"));
		}
		B.T2 -= Dt;
		if (B.T2 <= 0.f)
		{
			B.T2 = BossHpFrac < 0.5f ? 2.2f : 3.f;
			for (int32 s = -1; s <= 1; s += 2) { FFTEnt& Bb = Spawn(EFTKind::Barrel, B.D - B.L / 2 - 200.f - Pd, B.X + s * 22.f * XU); Bb.Sp = 0.f; }
		}
		if (BossHpFrac < 0.66f && B.Step == 0) { B.Step = 1; SpawnGroup({ EFTKind::Moto, EFTKind::Moto }); SayText(TEXT("He's calling in the bikers! Hold your line!"), 3); Caption = TEXT("HE'S CALLING IN THE BIKERS!"); CaptionT = RealClock; }
		if (BossHpFrac < 0.45f && BossLineStep == 0) { BossLineStep = 1; SayText(TEXT("He's cracking! Pour it on!"), 3); }
		if (BossHpFrac < 0.33f && B.Step == 1) { B.Step = 2; SpawnGroup({ EFTKind::Heli, EFTKind::Moto }); Caption = TEXT("HE'S CALLED IN A CHOPPER! ONE MORE PUSH!"); CaptionT = RealClock; }
		if (BossHpFrac < 0.2f && BossLineStep == 1) { BossLineStep = 2; SayText(TEXT("Finish him! FINISH HIM!"), 3); }
	}
	else if (Level == 1)
	{
		// THE ALCHEMIST: lane arcs, lock-on strikes and shock waves, each warned on the road first
		Want += 22.f * DU * FMath::Sin(0.8f * B.T);
		B.X = FMath::FInterpTo(B.X, 60.f * XU * FMath::Sin(0.45f * B.T), Dt, 2.f);
		if (Ph != B.Phase)
		{
			B.Phase = Ph; B.Step = 0;
			SayText(Ph == 1 ? TEXT("He's overcharging! Double arcs! Watch both lanes!") : TEXT("MELTDOWN! Everything's live! Dodge and SHOOT!"), 3);
			Caption = Ph == 1 ? TEXT("HE'S OVERCHARGING THE COILS!") : TEXT("MELTDOWN! THE WHOLE TANKER IS LIVE!"); CaptionT = RealClock;
			Sfx(TEXT("BossPhase")); Duck(0.45f, 0.8f);
		}
		B.T1 -= Dt;
		if (B.T1 <= 0.f && Ahead < B.Ta + 3000.f)
		{
			static const TCHAR* Seq[3][5] = { { TEXT("lane"), TEXT("zone"), TEXT("lane"), TEXT("ring"), nullptr }, { TEXT("lane2"), TEXT("ring"), TEXT("zone"), TEXT("lane2"), TEXT("zone") }, { TEXT("ring2"), TEXT("lane2"), TEXT("zone3"), TEXT("ring"), TEXT("lane2") } };
			const int32 Len = Ph == 0 ? 4 : 5;
			const FString A = Seq[Ph][B.Step % Len];
			B.Step++;
			const float Gap = Ph == 0 ? 2.6f : (Ph == 1 ? 2.1f : 1.75f);
			const float WarnK = Ph == 0 ? 1.f : 0.85f;
			if (A.StartsWith(TEXT("lane")))
			{
				int32 Lane = 0; for (int32 l = 1; l < 4; ++l) if (FMath::Abs(LaneX(l) - Px) < FMath::Abs(LaneX(Lane) - Px)) Lane = l;
				TArray<int32> Lanes = { Lane };
				if (A == TEXT("lane2")) Lanes.Add(FMath::Clamp(Lane + ((FMath::FRand() < 0.35f) ? 2 : 1) * (Lane < 2 ? 1 : -1), 0, 3));
				for (int32 Ln : Lanes) { FFTHazardState& Hz = Hazards.AddDefaulted_GetRef(); Hz.Type = 0; Hz.X = LaneX(Ln); Hz.Warn = 1.0f * WarnK; Hz.Live = 0.4f; }
				Say(TEXT("zapWarn"), 2); Sfx(TEXT("ZapWarn"));
				B.T1 = Gap;
			}
			else if (A.StartsWith(TEXT("zone")))
			{
				const int32 N = A == TEXT("zone3") ? 3 : (Ph >= 1 ? 2 : 1);
				for (int32 k = 0; k < N; ++k)
				{
					FFTHazardState& Hz = Hazards.AddDefaulted_GetRef(); Hz.Type = 1;
					const float Off = k == 0 ? 0.f : (k == 1 ? 1.f : -1.f) * R(75.f, 105.f) * XU * (k == 2 ? 1.8f : 1.f);
					Hz.X = FMath::Clamp(Px + Off, -RoadHalf, RoadHalf); Hz.D = 0.f; Hz.Track = k == 0 ? 0.55f : 0.f;
					Hz.Warn = (Ph == 0 ? 1.15f : 0.95f) + 0.18f * k; Hz.Live = 0.32f; Hz.W = 36.f * XU * 1.4f;
				}
				Say(TEXT("zoneWarn"), 2); Sfx(TEXT("ZoneWarn"));
				B.T1 = Gap + 0.2f;
			}
			else
			{
				const int32 N = A == TEXT("ring2") ? 2 : 1;
				for (int32 k = 0; k < N; ++k)
				{
					FFTHazardState& Hz = Hazards.AddDefaulted_GetRef(); Hz.Type = 2;
					int32 GapLane = 0; do { GapLane = FMath::RandRange(0, 3); } while (FMath::Abs(LaneX(GapLane) - Px) < 50.f * XU && FMath::RandBool());
					if (k == 1) GapLane = (GapLane + 2) % 4;
					Hz.X = LaneX(GapLane); Hz.Gap = (Ph == 2 ? 66.f : 78.f) * XU * 1.3f;
					Hz.Warn = (Ph == 0 ? 0.8f : 0.65f) + 0.9f * k; Hz.D = Ahead; Hz.W = (Ph == 0 ? 300.f : 340.f) * DU;
				}
				Say(TEXT("ringWarn"), 2); Shout(TEXT("SHOCK WAVE!")); Sfx(TEXT("RingWarn"));
				B.T1 = Gap + (N > 1 ? 0.9f : 0.4f);
			}
		}
	}
	else
	{
		// THE BIG DEALER: a two-deck carrier that never stops sending Snake Eyes bikers
		Want = B.Ta + 18.f * DU * FMath::Sin(0.6f * B.T);
		B.X = FMath::FInterpTo(B.X, 70.f * XU * FMath::Sin(0.4f * B.T), Dt, 2.f);
		if (Ph != B.Phase)
		{
			B.Phase = Ph; B.T3 = FMath::Min(B.T3, 2.5f);
			SayText(Ph == 1 ? TEXT("They're jumping off the top deck! Watch the road!") : TEXT("He's emptying the whole truck! Shoot the carrier!"), 3);
			Caption = Ph == 1 ? TEXT("THEY'RE JUMPING OFF THE TOP DECK!") : TEXT("HE'S EMPTYING THE WHOLE TRUCK!"); CaptionT = RealClock;
			Sfx(TEXT("BossPhase"));
		}
		int32 Hogs = 0; for (const FFTEnt& E : Ents) if (!E.bDead && E.Kind == EFTKind::Hog) Hogs++;
		B.T2 -= Dt;
		if (B.T2 <= 0.f && Hogs < 4 + Ph && Ahead < B.Ta + 2500.f)
		{
			B.T2 = Ph == 0 ? 2.6f : (Ph == 1 ? 2.1f : 1.6f);
			B.Count++;
			const bool bJump = (Ph == 1 && B.Count % 3 == 0) || (Ph == 2 && B.Count % 2 == 0);
			auto RampRider = [&]()
			{
				FFTEnt& H = Spawn(EFTKind::Hog, B.D - B.L / 2 - 200.f - Pd, B.X + R(-40.f, 40.f) * XU, FMath::RandRange(0, 1));
				H.State = 9; H.T1 = 0.55f; H.Vh = R(-40.f, 40.f) * XU; H.Side = FMath::RandBool() ? 1 : -1; H.T2 = R(1.f, 2.f);
			};
			if (bJump)
			{
				FFTEnt& H = Spawn(EFTKind::Hog, B.D - Pd, B.X, FMath::RandRange(0, 1));
				H.State = 8; H.T1 = 0.f; H.bAir = true; H.Tx = Px; H.Ta = B.D - Pd; H.H = 400.f; H.Side = FMath::RandBool() ? 1 : -1; H.T2 = 2.f;
				Say(TEXT("hogJump"), 2); Shout(TEXT("INCOMING!")); Sfx(TEXT("HogJump"));
			}
			else RampRider();
			if (Ph == 2) RampRider();
		}
		// brake check
		if (Ph >= 1)
		{
			B.T3 -= Dt;
			if (B.State == 0 && B.T3 <= 0.f) { B.State = 1; B.T1 = 0.f; Say(TEXT("brakeWarn"), 2); Shout(TEXT("BRAKES!")); Sfx(TEXT("BrakeWarn")); }
			if (B.State == 1)
			{
				B.T1 += Dt; B.Glow = 1.f;
				if (B.T1 > 0.8f) Want = B.Ta - 150.f * DU * 1.4f;
				if (B.T1 > 2.3f) { B.State = 0; B.Glow = 0.f; B.T3 = Ph == 2 ? 5.f : 7.5f; }
			}
		}
	}
	B.Sp = Vp + FMath::Clamp((Want - Ahead) * 1.4f, -6000.f, 6000.f);
	if (!bEscapeWarned && BossT > Def().BossEscape - (Level == 2 ? 18.f : 15.f)) { bEscapeWarned = true; Say(TEXT("escape"), 3); }
	if (BossT > Def().BossEscape) { bBossOn = false; Release(B); Wrap(true); }
}

void AFTGameMode::TickHazards(float Dt)
{
	const float Px = Car->X, Pd = Car->D;
	for (FFTHazardState& Hz : Hazards)
	{
		Hz.T += Dt;
		AFTHazard* A = Hz.Actor.Get();
		if (!A)
		{
			for (AFTHazard* P : HazardPool) if (P && P->IsHidden()) { A = P; break; }
			if (!A) { A = GetWorld()->SpawnActor<AFTHazard>(AFTHazard::StaticClass(), FTransform::Identity); HazardPool.Add(A); }
			A->SetActorHiddenInGame(false);
			Hz.Actor = A;
		}
		const bool bLive = Hz.T >= Hz.Warn;
		const float Blink = bLive ? 6.f : (FMath::Fmod(Hz.T, 0.2f) < 0.1f ? 1.6f : 0.6f);
		if (Hz.Type == 0)
		{
			// a lane goes live down the road from the tanker to behind the car
			const float L = 9000.f;
			A->SetActorTransform(FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector(L / 100.f, 64.f * XU / 100.f, 1.f)) * Track->TransformAt(Pd + L * 0.35f, Hz.X, 6.f));
			A->SetLook(bLive ? FLinearColor(0.7f, 0.9f, 1.f) : FLinearColor(1.f, 0.2f, 0.2f), Blink);
			if (bLive && !Hz.bHit) { Hz.bHit = true; Sfx(TEXT("Zap")); if (FMath::Abs(Px - Hz.X) < 39.f * XU) { HurtPlayer(16.f); Say(TEXT("shock"), 2); } else { Say(TEXT("dodge"), 1, 2.f); } }
			if (Hz.T > Hz.Warn + Hz.Live) Hz.bDone = true;
		}
		else if (Hz.Type == 1)
		{
			if (Hz.T < Hz.Track) Hz.X = FMath::FInterpConstantTo(Hz.X, Px, Dt, 70.f * XU);
			A->SetActorTransform(FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector(Hz.W / 50.f, Hz.W / 50.f, 1.f)) * Track->TransformAt(Pd + Hz.D, Hz.X, 6.f));
			A->SetLook(bLive ? FLinearColor(0.7f, 0.9f, 1.f) : FLinearColor(1.f, 0.15f, 0.2f), Blink);
			if (bLive && !Hz.bHit) { Hz.bHit = true; Sfx(TEXT("Strike")); Explode(Track->ToWorld(Pd + Hz.D, Hz.X, 50.f), 0.5f); if (FMath::Abs(Px - Hz.X) < Hz.W && FMath::Abs(Hz.D) < Hz.W) { HurtPlayer(18.f); Say(TEXT("shock"), 2); } }
			if (Hz.T > Hz.Warn + Hz.Live) Hz.bDone = true;
		}
		else if (Hz.Type == 2)
		{
			// the shock wave rolls down the road toward the car; a green gap lane lets you through
			if (bLive) Hz.D -= Hz.W * Dt;
			A->SetActorTransform(FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector(3.f, 2.f * FT::Edge / 100.f, 1.f)) * Track->TransformAt(Pd + Hz.D, 0.f, 8.f));
			A->SetLook(bLive ? FLinearColor(0.6f, 0.85f, 1.f) : FLinearColor(1.f, 0.25f, 0.2f), Blink);
			if (bLive && !Hz.bHit && Hz.D <= CarL * 0.5f)
			{
				Hz.bHit = true;
				const bool bInGap = FMath::Abs(Px - Hz.X) < Hz.Gap * 0.5f;
				if (!bInGap && Car->bGrounded) { HurtPlayer(14.f); Say(TEXT("shock"), 2); Sfx(TEXT("Shock")); }
				else { AddScore(200, 2); Popup(TEXT("DODGED! +200"), CGreen); Say(TEXT("dodge"), 1); Sfx(TEXT("Dodge")); }
			}
			if (Hz.D < -3000.f) Hz.bDone = true;
		}
	}
	for (FFTHazardState& Hz : Hazards) if (Hz.bDone && Hz.Actor.IsValid()) Hz.Actor->SetActorHiddenInGame(true);
	Hazards.RemoveAll([](const FFTHazardState& Hz) { return Hz.bDone; });
	// the shock wave's gap shows as its own green strip
}

// --------------------------------------------------------------------------------------------- stunts
void AFTGameMode::TickStunts(float Dt)
{
	const FFTFrame F = Track->Sample(Car->D);
	const uint16 SetFlags = F.Flags & (FTFlag::Loop | FTFlag::Twist | FTFlag::Pipe);
	if (SetFlags && SetPieceSection != F.Section)
	{
		SetPieceSection = F.Section; SetPieceFlag = SetFlags; bSetPieceHot = bHot;
		AheadSaid.Add(FString::Printf(TEXT("top%d"), F.Section));
	}
	if (SetPieceSection >= 0)
	{
		bSetPieceHot |= bHot;
		if ((F.Flags & FTFlag::Inverted) && !AheadSaid.Contains(FString::Printf(TEXT("inv%d"), SetPieceSection)))
		{
			AheadSaid.Add(FString::Printf(TEXT("inv%d"), SetPieceSection));
			Say(SetPieceFlag & FTFlag::Loop ? TEXT("loopTop") : (SetPieceFlag & FTFlag::Pipe ? TEXT("pipeTop") : TEXT("twistTop")), 2);
			Shout(TEXT("UPSIDE DOWN!"));
		}
		if (F.Section != SetPieceSection)
		{
			if (SetPieceFlag & FTFlag::Loop) { Stunt(bSetPieceHot ? TEXT("TURBO LOOP") : TEXT("LOOP THE LOOP"), bSetPieceHot ? 1200 : 700, bSetPieceHot ? TEXT("Turbo through the loop! Print it twice!") : TEXT("A full LOOP! The crew is screaming!"), bSetPieceHot ? 6.f : 4.f); Shout(TEXT("LOOP!")); }
			else if (SetPieceFlag & FTFlag::Twist) { Stunt(TEXT("CORKSCREW"), 800, TEXT("Through the corkscrew! Legendary!"), 5.f); Shout(TEXT("CORKSCREW!")); }
			else { const FFTFrame Prev = Track->Sample(Car->D - 400.f); (void)Prev; Stunt(TEXT("PIPE RUN"), 1000, TEXT("All the way round the pipe! Print it!"), 5.f); Shout(TEXT("PIPE!")); }
			SetPieceSection = -1;
		}
	}
	// gap jumps: you always clear the gap
	if ((F.Flags & FTFlag::Gap) && !Car->bGrounded && !bGapAir)
	{
		bGapAir = true; bSetPieceHot = bHot;
		Shout(TEXT("FLY!")); Slowmo(0.9f, 0.4f); Sfx(TEXT("Jump"));
	}
	if (bGapAir && Car->bGrounded && !(F.Flags & FTFlag::Gap))
	{
		bGapAir = false;
		Stunt(bSetPieceHot ? TEXT("TURBO AIR") : TEXT("BIG AIR"), bSetPieceHot ? 1200 : 700, bSetPieceHot ? TEXT("LOOK at that air! Over the whole gap!") : TEXT("FLY! Clear that gap!"), bSetPieceHot ? 7.f : 5.f);
		Say(TEXT("gapLand"), 2); Shout(TEXT("LANDED!")); Sfx(TEXT("Land"));
		Shake(0.8f);
	}
	if (bWasAir && Car->bGrounded && !bGapAir) { Sfx(TEXT("Land"), 0.6f); if (Car->AirTime > 0.6f) Shake(0.4f); }
	bWasAir = !Car->bGrounded;
	// wall ride: up on the high side of a banked wall
	if ((F.Flags & FTFlag::Wall) && Car->bGrounded && Car->X * FMath::Sign(F.Curve) < -50.f * XU)
	{
		if (WallRideSection != F.Section) { WallRideSection = F.Section; WallRideT = 0.f; Say(TEXT("wallIn"), 1); }
		WallRideT += Dt;
		if (WallRideT >= 1.2f && WallRideT - Dt < 1.2f) Stunt(TEXT("WALL RIDE"), 600, TEXT("Riding the wall! Gorgeous!"), 2.f);
	}
	// set pieces ahead
	struct FAheadKey { uint16 Flag; const TCHAR* Pool; };
	static const FAheadKey Keys[] = { { FTFlag::Loop, TEXT("aheadLoop") }, { FTFlag::Gap, TEXT("aheadJump") }, { FTFlag::Twist, TEXT("aheadTwist") }, { FTFlag::Pipe, TEXT("aheadPipe") }, { FTFlag::Wall, TEXT("aheadWall") } };
	for (const FAheadKey& K : Keys)
	{
		const float At = Track->NextFlagStart(Car->D, K.Flag, 1150.f * DU);
		if (At > 0.f)
		{
			const FString Key = FString::Printf(TEXT("%s%d"), K.Pool, FMath::RoundToInt(At / 1000.f));
			if (!AheadSaid.Contains(Key)) { AheadSaid.Add(Key); Say(K.Pool, 2); break; }
		}
	}
	// the boss shows up at the finale
	if (!bBossSpawned && Car->D >= Def().FinaleD * DU) { SpawnGroup({ EFTKind::Boss }); }
}

void AFTGameMode::Stunt(const FString& Name, int32 Pts, const TCHAR* SayLine, float ReelScore)
{
	const int64 P = (int64)Pts * Combo;
	AddScore(P, 2);
	StuntCount++;
	ComboT = 3.f;
	Popup(FString::Printf(TEXT("%s +%lld"), *Name, P), CPink, 1.3f);
	if (SayLine) SayText(SayLine, 2);
	Sfx(TEXT("Stunt"));
	Duck(0.5f, 0.5f);
	ReelEvent(ReelScore, Name, Car->GetActorLocation());
}

void AFTGameMode::Popup(const FString& T, const FLinearColor& C, float Big)
{
	FFTPopup& P = Popups.AddDefaulted_GetRef();
	P.Text = T; P.Col = C; P.Big = Big;
	if (Popups.Num() > 6) Popups.RemoveAt(0);
}

// --------------------------------------------------------------------------------------------- director
void AFTGameMode::SayText(const FString& T, int32 Pri)
{
	const float Age = RealClock - LineT;
	if (!Line.IsEmpty() && Pri < LinePri && Age < LineHold) return;
	if (!Line.IsEmpty() && Pri <= LinePri && Pri < 3 && Age < 0.7f) return;
	Line = T; LinePri = Pri; LineT = RealClock;
	LineHold = FMath::Clamp(0.9f + 0.045f * T.Len(), 1.8f, 3.6f);
	Duck(Pri >= 3 ? 0.4f : 0.6f, Pri >= 3 ? 1.4f : 0.7f);
}

void AFTGameMode::Say(const FName& Key, int32 Pri, float Cooldown)
{
	if (Cooldown > 0.f)
	{
		float& C = SayCool.FindOrAdd(Key, -100.f);
		if (RealClock - C < Cooldown) return;
		C = RealClock;
	}
	const TArray<FString>& L = FTData::Lines(Key);
	if (L.Num() == 0) return;
	int32& Cur = PoolCursor.FindOrAdd(Key, 0);
	SayText(L[Cur % L.Num()], Pri);
	Cur++;
}

void AFTGameMode::Shout(const FString& W) { ShoutWord = W; ShoutT = RealClock; }

void AFTGameMode::TickDirector(float RDt)
{
	if (Phase != EFTPhase::Play) return;
	if (RealClock - LineT < LineHold + 0.4f) return;
	AmbientT -= RDt;
	if (AmbientT > 0.f) return;
	AmbientT = 0.8f;
	bool bFoes = false, bBigAhead = false;
	for (const FFTEnt& E : Ents)
	{
		if (E.bDead) continue;
		const float A = E.D - Car->D;
		if (A > 0.f && A < 9000.f && (E.Kind == EFTKind::Moto || E.Kind == EFTKind::Rammer || E.Kind == EFTKind::Suv || E.Kind == EFTKind::Truck || E.Kind == EFTKind::Heli || E.Kind == EFTKind::Hog || E.Kind == EFTKind::Boss))
		{
			bFoes = true;
			if (E.bBig) bBigAhead = true;
		}
	}
	const bool bInv = (Track->FlagsAt(Car->D) & FTFlag::Inverted) != 0;
	auto Amb = [this](const TCHAR* K, float Cd) { const float* C = SayCool.Find(K); if (C && RealClock - *C < Cd) return false; Say(K, 0, Cd); return true; };
	if (bInv && Amb(TEXT("inverted"), 4.f)) return;
	if (bBigAhead && Missiles > 0 && NoMissileT > 4.f && Amb(TEXT("missiles"), 5.f)) return;
	if (bFoes && NoFireT > 1.2f && Amb(TEXT("takeOut"), 2.5f)) return;
	if (Hp < 40.f && Amb(TEXT("lowHpAmb"), 8.f)) return;
	if (Car->Speed < 360.f * DU && NoTurboT > 2.f && Amb(TEXT("faster"), 6.f)) return;
	if (bFoes && LastFireT < 0.5f && Amb(TEXT("keepFiring"), 6.f)) return;
	if (Turbo > 95.f && NoTurboT > 7.f && Amb(TEXT("useTurbo"), 8.f)) return;
	if (!bFoes && NoRailT > 10.f && Amb(TEXT("sparks"), 10.f)) return;
	if (!bFoes && NoNearT > 6.f && Amb(TEXT("weave"), 8.f)) return;
	if (FMath::Abs(Track->Sample(Car->D + 3000.f).Curve) > 0.9f && NoDriftT > 14.f && Amb(TEXT("drift"), 14.f)) return;
	if (bFoes && Amb(TEXT("takeOut"), 2.5f)) return;
	Amb(TEXT("calm"), 6.f);
}

// --------------------------------------------------------------------------------------------- time, sound, shake
void AFTGameMode::Slowmo(float Dur, float Scale, float Cool)
{
	if (Cool > 0.f && RealClock < SlowCool) return;
	if (Cool > 0.f) SlowCool = RealClock + Cool;
	SlowT = FMath::Max(SlowT, Dur); SlowScale = FMath::Min(Scale, SlowT > Dur ? SlowScale : 1.f);
	SlowScale = Scale;
	Sfx(TEXT("Slowmo"), 0.6f); Duck(0.45f, 0.8f);
}

void AFTGameMode::Freeze(float Dur) { FreezeT = FMath::Max(FreezeT, Dur); }

void AFTGameMode::TickSlowmo(float RDt)
{
	FreezeT -= RDt; SlowT -= RDt;
	float Target = 1.f;
	if (SlowT > 0.f) Target = SlowScale;
	TimeScale = Target < TimeScale ? FMath::FInterpTo(TimeScale, Target, RDt, 12.f) : FMath::FInterpTo(TimeScale, Target, RDt, 3.f);
	const float Final = FreezeT > 0.f ? 0.02f : TimeScale;
	UGameplayStatics::SetGlobalTimeDilation(this, Final);
}

void AFTGameMode::Sfx(const TCHAR* Name, float Vol, float Pitch)
{
	const FName Key(Name);
	TObjectPtr<USoundBase>* F = Sounds.Find(Key);
	USoundBase* S = F ? F->Get() : nullptr;
	if (!F) { S = Load<USoundBase>(FString(TEXT("/Game/Audio/SFX/S_")) + Name); Sounds.Add(Key, S); }
	if (S) UGameplayStatics::PlaySound2D(this, S, Vol, Pitch);
}

void AFTGameMode::Duck(float Depth, float Hold) { DuckDepth = FMath::Min(DuckT > 0.f ? DuckDepth : 1.f, Depth); DuckT = FMath::Max(DuckT, Hold); }
void AFTGameMode::Shake(float Amount) { ShakeAmt = FMath::Max(ShakeAmt, Amount); }

// --------------------------------------------------------------------------------------------- camera
bool AFTGameMode::ComputeCamera(FMinimalViewInfo& POV, float Dt)
{
	if (!Car || !Track || !Track->IsBuilt()) return false;
	const float RDt = FMath::Min(FApp::GetDeltaTime(), 0.05f);
	if (Phase == EFTPhase::Reel)
	{
		POV.Location = CamLoc; POV.Rotation = FRotationMatrix::MakeFromXZ(CamFwd, CamUp).Rotator(); POV.FOV = CamFov;
		return true;
	}
	const FVector CarLoc = Car->GetActorLocation();
	const FFTFrame F = Track->Sample(Car->D);
	if (Phase == EFTPhase::Title || Phase == EFTPhase::Rating)
	{
		const float A = RealClock * 0.18f;
		const FVector Off = (F.Fwd * FMath::Cos(A) + F.Right * FMath::Sin(A)) * 900.f + F.Up * 260.f;
		POV.Location = CarLoc + Off;
		POV.Rotation = FRotationMatrix::MakeFromXZ((CarLoc + F.Up * 60.f - POV.Location).GetSafeNormal(), F.Up).Rotator();
		POV.FOV = 60.f;
		bCamInit = false;
		return true;
	}
	// chase camera: rides the road's frame so loops and pipes turn the world, not the car
	const FVector Vel = Car->Chassis->GetPhysicsLinearVelocity();
	const float Spd = FMath::Max(0.f, Car->Speed);
	const FVector Heading = (F.Fwd * 0.6f + Car->GetActorForwardVector() * 0.4f).GetSafeNormal();
	if (!bCamInit) { CamUp = F.Up; CamFwd = Heading; bCamInit = true; }
	CamUp = FMath::VInterpTo(CamUp, Car->bGrounded || Car->H < 600.f ? F.Up : FVector::UpVector, RDt, 6.f).GetSafeNormal();
	CamFwd = FMath::VInterpTo(CamFwd, Heading, RDt, 7.f).GetSafeNormal();
	const bool bInLoop = (F.Flags & (FTFlag::Loop | FTFlag::Pipe)) != 0;
	const float Back = 780.f + Spd * 0.018f;
	const float Up = bInLoop ? 210.f : 290.f;
	FVector Desired = CarLoc - CamFwd * Back + CamUp * Up;
	FVector Look = CarLoc + CamFwd * 700.f + CamUp * 90.f;
	if (Phase == EFTPhase::Wreck) { Desired = CarLoc - CamFwd * 1400.f + CamUp * 700.f + F.Right * 600.f; Look = CarLoc; }
	const float RollTarget = FMath::Clamp(-Car->LatSpeed / 2500.f * 0.0f + Steer * -0.06f, -0.08f, 0.08f);
	CamRoll = FMath::FInterpTo(CamRoll, RollTarget, RDt, 3.f);
	ShakeAmt = FMath::FInterpTo(ShakeAmt, 0.f, RDt, 4.f);
	const FVector ShakeV = FVector(FMath::FRandRange(-1.f, 1.f), FMath::FRandRange(-1.f, 1.f), FMath::FRandRange(-1.f, 1.f)) * ShakeAmt * 18.f;
	CamLoc = Desired + ShakeV;
	const FVector Fwd = (Look - CamLoc).GetSafeNormal();
	const FVector UpRolled = CamUp.RotateAngleAxis(FMath::RadiansToDegrees(CamRoll), Fwd);
	POV.Location = CamLoc;
	POV.Rotation = FRotationMatrix::MakeFromXZ(Fwd, UpRolled).Rotator();
	const float FovTarget = 78.f + FMath::Clamp(Spd / (700.f * DU), 0.f, 1.f) * 14.f + (bTurboOn ? 6.f : 0.f);
	CamFov = FMath::FInterpTo(CamFov, FovTarget, RDt, 3.f);
	POV.FOV = CamFov;
	(void)Vel;
	return true;
}

// --------------------------------------------------------------------------------------------- highlight reel
void AFTGameMode::RecordSnap(float RDt)
{
	SnapT -= RDt;
	if (SnapT > 0.f) return;
	SnapT = 1.f / 20.f;
	FFTSnap& S = Snaps.AddDefaulted_GetRef();
	S.Time = Clock;
	S.Car = Car->GetActorTransform();
	S.CarSteer = Steer;
	for (const FFTEnt& E : Ents)
	{
		if (E.bDead || !E.Proxy.IsValid() || E.Kind == EFTKind::Bullet) continue;
		S.Ents.Add(TPair<int32, FTransform>(E.Id, E.Proxy->GetActorTransform()));
	}
	// keep only what a clip can still use: the last 4.2 s, plus everything inside a clip
	const float Keep = Clock - 4.2f;
	int32 Drop = 0;
	while (Drop < Snaps.Num() && Snaps[Drop].Time < Keep)
	{
		bool bUsed = false;
		for (const FFTClip& C : Clips) if (Snaps[Drop].Time >= C.T0 && Snaps[Drop].Time <= C.T1 + 2.f) { bUsed = true; break; }
		if (bUsed) break;
		++Drop;
	}
	if (Drop > 0) Snaps.RemoveAt(0, Drop, EAllowShrinking::No);
}

void AFTGameMode::ReelEvent(float Sc, const FString& Label, const FVector& Where, bool bBoss)
{
	if (Phase != EFTPhase::Play && !bBoss) return;
	for (FFTClip& C : Clips)
	{
		if (!C.bBoss && Clock >= C.T0 && Clock <= C.T1 && C.T1 - C.T0 < 7.f)
		{
			C.T1 = FMath::Min(C.T0 + 7.f, Clock + 1.9f);
			C.Score += Sc;
			if (Sc >= C.Score - Sc) { C.Focus = Clock; C.Label = Label; C.FocusPos = Where; }
			return;
		}
	}
	FFTClip& C = Clips.AddDefaulted_GetRef();
	C.T0 = Clock - 1.7f; C.T1 = Clock + (bBoss ? 3.6f : 1.9f); C.Focus = Clock; C.Score = Sc; C.Label = Label; C.Scene = Scene; C.bBoss = bBoss; C.FocusPos = Where;
	if (Clips.Num() > 12)
	{
		int32 Worst = -1;
		for (int32 i = 0; i < Clips.Num(); ++i) if (!Clips[i].bBoss && (Worst < 0 || Clips[i].Score < Clips[Worst].Score)) Worst = i;
		if (Worst >= 0) Clips.RemoveAt(Worst);
	}
}

void AFTGameMode::StartReel()
{
	TArray<FFTClip> Pick = Clips.FilterByPredicate([](const FFTClip& C) { return !C.bBoss; });
	Pick.Sort([](const FFTClip& A, const FFTClip& B) { return A.Score > B.Score; });
	if (Pick.Num() > 5) Pick.SetNum(5);
	Pick.Sort([](const FFTClip& A, const FFTClip& B) { return A.T0 < B.T0; });
	ReelList = Pick;
	for (const FFTClip& C : Clips) if (C.bBoss)
	{
		FFTClip A = C; A.Camera = 3; A.T0 = C.Focus - 2.6f; A.T1 = C.Focus + 2.f; ReelList.Add(A);
		FFTClip B = C; B.Camera = 6; B.T0 = C.Focus - 0.9f; B.T1 = C.Focus + 2.f; B.Label = TEXT("ANOTHER ANGLE"); ReelList.Add(B);
	}
	if (ReelList.Num() == 0) { FinishRating(); return; }
	int32 Cam = 0; LastCam = -1;
	static const int32 Pref[] = { 2, 7, 1, 4, 5, 6, 0, 3 };
	for (FFTClip& C : ReelList)
	{
		if (C.bBoss) continue;
		do { Cam = Pref[FMath::RandRange(0, 7)]; } while (Cam == LastCam);
		C.Camera = Cam; LastCam = Cam;
	}
	Phase = EFTPhase::Reel; PhaseT = 0.f;
	ReelClip = 0; ReelT = ReelList[0].T0; ReelShots = ReelList.Num(); ReelTitleT = 2.2f;
	// hide the live scene; ghosts play the recording
	Car->SetActorHiddenInGame(true);
	Car->Chassis->SetSimulatePhysics(false);
	for (TObjectPtr<AFTEntityActor>& A : Pool) if (A) A->SetActorHiddenInGame(true);
	UGameplayStatics::SetGlobalTimeDilation(this, 1.f);
}

void AFTGameMode::TickReel(float RDt)
{
	AFTPlayerController* PC = Cast<AFTPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC && PhaseT > 0.8f && (PC->MenuOk() || PC->MenuBack())) { EndReel(); return; }
	if (ReelTitleT > 0.f) { ReelTitleT -= RDt; return; }
	if (ReelClip >= ReelList.Num()) { EndReel(); return; }
	FFTClip& C = ReelList[ReelClip];
	ReelShot = ReelClip + 1;
	ReelLabel = C.Label;
	static const TCHAR* CamNames[] = { TEXT("A CHASE"), TEXT("B BUMPER"), TEXT("C TRACKING"), TEXT("D CRANE"), TEXT("E AERIAL"), TEXT("F ROADSIDE"), TEXT("G KILL CAM"), TEXT("H DOLLY") };
	ReelCamName = CamNames[C.Camera];
	float Rate = 1.f;
	if (C.Score >= 5.f) { const float Df = FMath::Abs(ReelT - C.Focus); Rate = Df < 0.3f ? 0.35f : (Df < 0.6f ? 0.6f : 1.f); }
	bReelSlow = Rate < 1.f;
	ReelT += RDt * Rate;
	if (ReelT > C.T1) { ReelClip++; if (ReelClip < ReelList.Num()) ReelT = ReelList[ReelClip].T0; return; }

	// find the two snapshots around ReelT
	int32 i1 = 0;
	while (i1 < Snaps.Num() - 1 && Snaps[i1].Time < ReelT) ++i1;
	const int32 i0 = FMath::Max(0, i1 - 1);
	if (Snaps.Num() == 0) { EndReel(); return; }
	const FFTSnap& S0 = Snaps[i0];
	const FFTSnap& S1 = Snaps[i1];
	const float A = (S1.Time > S0.Time) ? FMath::Clamp((ReelT - S0.Time) / (S1.Time - S0.Time), 0.f, 1.f) : 0.f;
	FTransform CarT; CarT.Blend(S0.Car, S1.Car, A);

	// ghost car and ghost entities
	auto Ghost = [this](int32 Key, EFTKind K, int32 V) -> AFTEntityActor*
	{
		if (int32* Gi = GhostOf.Find(Key)) return Ghosts[*Gi];
		AFTEntityActor* G = GetWorld()->SpawnActor<AFTEntityActor>(AFTEntityActor::StaticClass(), FTransform::Identity);
		if (K == EFTKind::None) { G->Setup(EFTKind::None, 0, Level); G->Mesh->SetStaticMesh(Car->Body->GetStaticMesh()); }
		else G->Setup(K, V, Level);
		GhostOf.Add(Key, Ghosts.Add(G));
		return G;
	};
	for (AFTEntityActor* G : Ghosts) if (G) G->SetActorHiddenInGame(true);
	AFTEntityActor* GC = Ghost(-1, EFTKind::None, 0);
	GC->SetActorHiddenInGame(false);
	GC->SetActorTransform(CarT);
	for (const TPair<int32, FTransform>& P : S0.Ents)
	{
		FTransform T = P.Value;
		for (const TPair<int32, FTransform>& Q : S1.Ents) if (Q.Key == P.Key) { T.Blend(P.Value, Q.Value, A); break; }
		const int32* KV = ProxyKind.Find(P.Key);
		if (!KV) continue;
		AFTEntityActor* G = Ghost(P.Key, (EFTKind)(*KV / 16), *KV % 16);
		G->SetActorHiddenInGame(false);
		G->SetActorTransform(T);
	}
	if (FMath::Abs(ReelT - C.Focus) < RDt * Rate * 0.6f && C.Score >= 2.f) Explode(C.FocusPos, C.bBoss ? 3.f : 1.2f);

	// cameras (road terms, relative to the car)
	const FVector P = CarT.GetLocation();
	const FVector Fw = CarT.GetRotation().GetForwardVector(), Rt = CarT.GetRotation().GetRightVector(), Up = CarT.GetRotation().GetUpVector();
	const float U = FMath::Clamp((ReelT - C.T0) / FMath::Max(0.1f, C.T1 - C.T0), 0.f, 1.f);
	FVector Loc, Tgt = P + Up * 80.f; float Fov = 70.f;
	switch (C.Camera)
	{
	case 1: Loc = P - Fw * 40.f * DU + Rt * 16.f * XU + Up * 60.f; Tgt = P + Fw * 2000.f; Fov = 82.f; break;
	case 2: Loc = P - Fw * 88.f * DU * 0.5f + Rt * 100.f * XU * 0.6f + Up * 160.f; Fov = 55.f; break;
	case 3: Loc = P - Fw * 600.f + Up * FMath::Lerp(1600.f, 1000.f, U) + Rt * 300.f; Tgt = C.bBoss ? FMath::Lerp(P, C.FocusPos, 0.5f) : P; Fov = 65.f; break;
	case 4: Loc = P - Fw * 240.f * DU * 0.5f + Up * 2100.f; Fov = 60.f; break;
	case 5: Loc = Track->ToWorld(Track->Locate(P, Car->D, Fov, Fov) + 1500.f, (C.FocusPos.Y > 0 ? 1.f : -1.f) * 172.f * XU, 120.f); Fov = FMath::Lerp(70.f, 30.f, U); break;
	case 6: Loc = FMath::Lerp(P - Fw * 900.f + Up * 300.f, C.FocusPos - Fw * 1200.f + Up * 400.f + Rt * 500.f, FMath::Clamp(U * 1.5f, 0.f, 1.f)); Tgt = C.FocusPos; Fov = 55.f; break;
	case 7: Loc = P - Fw * 105.f * DU * 0.5f + Rt * FMath::Lerp(-95.f, 95.f, U) * XU + Up * 140.f; Fov = 62.f; break;
	default: Loc = P - Fw * 800.f + Up * 290.f; Tgt = P + Fw * 700.f; Fov = 80.f; break;
	}
	CamLoc = Loc; CamFwd = (Tgt - Loc).GetSafeNormal(); CamUp = Up; CamFov = Fov;
}

void AFTGameMode::EndReel()
{
	for (AFTEntityActor* G : Ghosts) if (G) G->Destroy();
	Ghosts.Reset(); GhostOf.Reset();
	Car->SetActorHiddenInGame(false);
	Car->Chassis->SetSimulatePhysics(true);
	Car->PlaceOnTrack(Car->D, 0.f, 0.f);
	FinishRating();
}

// --------------------------------------------------------------------------------------------- benchmark
void AFTGameMode::TickBench(float RDt)
{
	UFTGameInstance* G = GI();
	if (Phase == EFTPhase::Rating || Phase == EFTPhase::Wrap) { G->BenchSeconds = BenchT; }
	if (Phase != EFTPhase::Play && Phase != EFTPhase::Wreck && Phase != EFTPhase::Wrap && Phase != EFTPhase::Rating) return;
	BenchT += RDt;
	if (BenchT > 4.f)
	{
		BenchFrames.Add(FApp::GetDeltaTime() * 1000.f);
		BenchGameMs += FPlatformTime::ToMilliseconds(GGameThreadTime);
		BenchRenderMs += FPlatformTime::ToMilliseconds(GRenderThreadTime);
		const float Gpu = FPlatformTime::ToMilliseconds(RHIGetGPUFrameCycles(0));
		BenchGpuMs += Gpu;
		BenchGpuFrames.Add(Gpu);
	}
	if (BenchT < G->BenchSeconds + 4.f && Phase == EFTPhase::Play) return;
	if (BenchFrames.Num() < 10) return;

	TArray<float> S = BenchFrames;
	S.Sort();
	double Sum = 0; for (float v : S) Sum += v;
	const float Avg = Sum / S.Num();
	auto Pct = [&S](float P) { return S[FMath::Clamp(FMath::FloorToInt(P * (S.Num() - 1)), 0, S.Num() - 1)]; };
	// 1% low = average fps over the slowest 1% of frames
	const int32 N1 = FMath::Max(1, S.Num() / 100);
	double Slow = 0; for (int32 i = S.Num() - N1; i < S.Num(); ++i) Slow += S[i];
	const float Low1 = 1000.f / (Slow / N1);
	int32 Over = 0; for (float v : S) if (v > 1000.f / 60.f * 1.05f) Over++;
	TArray<float> Gs = BenchGpuFrames; Gs.Sort();
	const float GpuP99 = Gs.Num() ? Gs[FMath::Clamp(FMath::FloorToInt(0.99f * (Gs.Num() - 1)), 0, Gs.Num() - 1)] : 0.f;
	const float N = (float)S.Num();
	// what the frames were actually made with: the live scalability groups and present settings
	auto CV = [](const TCHAR* Name) { IConsoleVariable* V = IConsoleManager::Get().FindConsoleVariable(Name); return V ? V->GetString() : FString(TEXT("?")); };
	const FString Live = FString::Printf(TEXT("view%s shadow%s gi%s refl%s pp%s fx%s tex%s foliage%s shading%s vsync%s maxfps%s sp%s"),
		*CV(TEXT("sg.ViewDistanceQuality")), *CV(TEXT("sg.ShadowQuality")), *CV(TEXT("sg.GlobalIlluminationQuality")), *CV(TEXT("sg.ReflectionQuality")),
		*CV(TEXT("sg.PostProcessQuality")), *CV(TEXT("sg.EffectsQuality")), *CV(TEXT("sg.TextureQuality")), *CV(TEXT("sg.FoliageQuality")), *CV(TEXT("sg.ShadingQuality")),
		*CV(TEXT("r.VSync")), *CV(TEXT("t.MaxFPS")), *CV(TEXT("r.ScreenPercentage")));
	const FString Json = FString::Printf(TEXT("{\"level\":%d,\"quality\":\"%s\",\"tag\":\"%s\",\"seconds\":%.1f,\"frames\":%d,\"avg_fps\":%.1f,\"p1_low_fps\":%.1f,\"median_ms\":%.2f,\"p99_ms\":%.2f,\"worst_ms\":%.2f,\"frames_over_17_5ms\":%d,\"game_thread_ms\":%.2f,\"render_thread_ms\":%.2f,\"gpu_ms\":%.2f,\"gpu_p99_ms\":%.2f,\"live\":\"%s\",\"resolution\":\"%dx%d\",\"rhi\":\"%s\",\"gpu\":\"%s\",\"distance_m\":%.0f}"),
		Level, *UFTGameInstance::QualityName(G->BenchQuality), *G->BenchTag, BenchT - 4.f, S.Num(), 1000.f / Avg, Low1, Pct(0.5f), Pct(0.99f), S.Last(), Over,
		BenchGameMs / N, BenchRenderMs / N, BenchGpuMs / N, GpuP99, *Live,
		GEngine && GEngine->GameViewport ? (int32)GEngine->GameViewport->Viewport->GetSizeXY().X : 0, GEngine && GEngine->GameViewport ? (int32)GEngine->GameViewport->Viewport->GetSizeXY().Y : 0,
		GDynamicRHI ? GDynamicRHI->GetName() : TEXT("?"), *GRHIAdapterName, Car->D / 100.f);
	const FString Dir = FPaths::Combine(G->SaveDir(), TEXT("bench"));
	IFileManager::Get().MakeDirectory(*Dir, true);
	FFileHelper::SaveStringToFile(Json, *FPaths::Combine(Dir, FString::Printf(TEXT("bench_L%d_%s%s.json"), Level, *UFTGameInstance::QualityName(G->BenchQuality), *G->BenchTag)));
	UE_LOG(LogFT, Display, TEXT("FTBENCH %s"), *Json);
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
	BenchFrames.Reset();
}

void AFTGameMode::Snap(const FString& Name)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !PC->PlayerCameraManager) return;
	const FMinimalViewInfo POV = PC->PlayerCameraManager->GetCameraCacheView();
	UTextureRenderTarget2D* RT = UKismetRenderingLibrary::CreateRenderTarget2D(this, 1280, 720, RTF_RGBA8_SRGB);
	USceneCaptureComponent2D* C = NewObject<USceneCaptureComponent2D>(this);
	C->bCaptureEveryFrame = false;
	C->bCaptureOnMovement = false;
	C->CaptureSource = SCS_FinalToneCurveHDR;
	C->TextureTarget = RT;
	C->FOVAngle = POV.FOV;
	C->RegisterComponentWithWorld(GetWorld());
	C->SetWorldLocationAndRotation(POV.Location, POV.Rotation);
	C->CaptureScene();
	UKismetRenderingLibrary::ExportRenderTarget(this, RT, FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Shots")), Name + TEXT(".png"));
	C->DestroyComponent();
}
