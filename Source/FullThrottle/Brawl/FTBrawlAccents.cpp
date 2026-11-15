#include "FTBrawlGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Sound/SoundAttenuation.h"
#include "Sound/SoundConcurrency.h"
#include "Sound/SoundBase.h"
#if __has_include("FTGameInstance.h")
#include "FTGameInstance.h"
#endif

namespace
{
	TAutoConsoleVariable<int32> CVarBrawlReducedMotion(TEXT("ftb.ReducedMotion"), 0,
		TEXT("Reduce Brawl camera motion and presentation effects; the career accessibility setting also applies."));
	constexpr int32 AccentCapacity = 32;
	const TCHAR* AccentNames[] = { TEXT("Dust"), TEXT("Land"), TEXT("Slash"), TEXT("Pickup"),
		TEXT("Glass"), TEXT("Wood"), TEXT("Smoke"), TEXT("Flame") };
	const float AccentLife[] = { 0.28f, 0.35f, 0.105f, 0.32f, 0.24f, 0.28f, 0.48f, 0.15f };
	const float AccentVolumes[] = { 0.f, 0.24f, 0.16f, 0.29f, 0.44f, 0.38f, 0.f, 0.f };
}

bool AFTBrawlGameMode::ReducedPresentationMotion() const
{
	if (CVarBrawlReducedMotion.GetValueOnGameThread() != 0) return true;
#if __has_include("FTGameInstance.h")
	if (const UFTGameInstance* Settings = GetGameInstance<UFTGameInstance>()) return Settings->bReducedMotion;
#endif
	return false;
}

void AFTBrawlGameMode::InitAccents()
{
	static_assert(UE_ARRAY_COUNT(AccentNames) == static_cast<int32>(EAccent::Count));
	CombatAttenuation = NewObject<USoundAttenuation>(this);
	CombatAttenuation->Attenuation.bAttenuate = true;
	CombatAttenuation->Attenuation.bSpatialize = true;
	CombatAttenuation->Attenuation.AttenuationShapeExtents = FVector(2100.f);
	CombatAttenuation->Attenuation.FalloffDistance = 1800.f;
	CombatConcurrency = NewObject<USoundConcurrency>(this);
	CombatConcurrency->Concurrency.MaxCount = 8;
	CombatConcurrency->Concurrency.ResolutionRule = EMaxConcurrentResolutionRule::StopQuietest;
	for (int32 I = 0; I < UE_ARRAY_COUNT(AccentNames); ++I)
	{
		const TCHAR* Name = AccentNames[I];
		AccentTextures.Add(LoadObject<UTexture>(nullptr,
			*FString::Printf(TEXT("/Game/Brawl/Art/Studio/T_BrawlV2%s.T_BrawlV2%s"), Name, Name)));
		AccentSounds.Add(AccentVolumes[I] > 0.f ? LoadObject<USoundBase>(nullptr,
			*FString::Printf(TEXT("/Game/Brawl/Audio/Studio/S_BrawlV2%s.S_BrawlV2%s"), Name, Name)) : nullptr);
	}
	UStaticMesh* Plane = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"));
	UMaterialInterface* Mat = LoadObject<UMaterialInterface>(nullptr,
		TEXT("/Game/Brawl/Materials/M_BrawlV2Accent.M_BrawlV2Accent"));
	if (!Plane || !Mat) return;
	AccentEffects.SetNum(AccentCapacity);
	for (int32 I = 0; I < AccentCapacity; ++I)
	{
		AStaticMeshActor* Quad = GetWorld()->SpawnActor<AStaticMeshActor>();
		Quad->SetMobility(EComponentMobility::Movable);
		Quad->SetActorHiddenInGame(true);
		auto* Mesh = Quad->GetStaticMeshComponent();
		Mesh->SetStaticMesh(Plane);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetCastShadow(false);
		Mesh->bAffectDistanceFieldLighting = false;
		Mesh->SetTranslucentSortPriority(2);
		Mesh->CreateDynamicMaterialInstance(0, Mat);
		AccentQuads.Add(Quad);
	}
}

void AFTBrawlGameMode::Accent(EAccent Kind, const FVector& At, float Size, float Direction, bool bSound)
{
	const int32 I = static_cast<int32>(Kind);
	bool bMuted = false;
#if __has_include("FTGameInstance.h")
	if (const UFTGameInstance* Settings = GetGameInstance<UFTGameInstance>()) bMuted = Settings->bMuted;
#endif
	if (bSound && !bMuted && AccentSoundCooldown <= 0.f && AccentSounds.IsValidIndex(I) && AccentSounds[I])
	{
		AccentSoundCooldown = 0.045f;
		const float Pitch = 0.96f + float((SoundVariation++ * 3) % 7) * 0.014f;
		UGameplayStatics::PlaySoundAtLocation(this, AccentSounds[I], At, AccentVolumes[I], Pitch, 0.f,
			CombatAttenuation, CombatConcurrency);
	}
	if (AccentQuads.IsEmpty() || !AccentTextures.IsValidIndex(I) || !AccentTextures[I]) return;
	const int32 Slot = NextAccentQuad++ % AccentQuads.Num();
	NextAccentQuad %= AccentQuads.Num();
	FAccentEffect& Effect = AccentEffects[Slot];
	Effect = {};
	Effect.At = At;
	Effect.Dir = Direction < 0.f ? -1.f : 1.f;
	Effect.Size = Size;
	Effect.Life = AccentLife[I];
	Effect.Kind = Kind;
	Effect.Velocity = Kind == EAccent::Smoke ? FVector(-Effect.Dir * 28.f, 0, 72.f)
		: Kind == EAccent::Dust || Kind == EAccent::Land ? FVector(-Effect.Dir * 32.f, 0, 9.f)
		: Kind == EAccent::Pickup ? FVector(0, 0, 24.f) : FVector(Effect.Dir * 22.f, 0, 0);
	auto* Quad = AccentQuads[Slot].Get();
	auto* Mat = CastChecked<UMaterialInstanceDynamic>(Quad->GetStaticMeshComponent()->GetMaterial(0));
	Mat->SetTextureParameterValue(TEXT("Tex"), AccentTextures[I]);
	Mat->SetScalarParameterValue(TEXT("Opacity"), ReducedPresentationMotion() ? 0.68f : 0.92f);
	Mat->SetScalarParameterValue(TEXT("Glow"), Kind == EAccent::Flame ? 1.65f : 1.f);
	Quad->SetActorLocation(At);
	Quad->SetActorRotation(FRotator(0, 0, 90));
	Quad->SetActorScale3D(FVector(Size / 100.f * Effect.Dir, Size / 100.f, 1.f));
	Quad->SetActorHiddenInGame(false);
}

void AFTBrawlGameMode::TickAccents(float Dt)
{
	AccentSoundCooldown = FMath::Max(0.f, AccentSoundCooldown - Dt);
	for (int32 I = 0; I < AccentQuads.Num(); ++I)
	{
		FAccentEffect& E = AccentEffects[I];
		if (E.Life <= 0.f) continue;
		E.Age += Dt;
		auto* Quad = AccentQuads[I].Get();
		if (E.Age >= E.Life) { E.Life = 0.f; Quad->SetActorHiddenInGame(true); continue; }
		const float U = E.Age / E.Life;
		const bool bDebris = E.Kind == EAccent::Glass || E.Kind == EAccent::Wood;
		const float Expansion = E.Kind == EAccent::Slash ? 0.9f + U * 0.2f : 0.72f + 0.64f * FMath::Sqrt(U);
		const float Scale = E.Size / 100.f * Expansion;
		Quad->SetActorScale3D(FVector(Scale * E.Dir, Scale, 1.f));
		Quad->SetActorLocation(E.At + E.Velocity * E.Age + FVector(0, 0, bDebris ? -260.f * E.Age * E.Age : 0.f));
		auto* Mat = CastChecked<UMaterialInstanceDynamic>(Quad->GetStaticMeshComponent()->GetMaterial(0));
		const float Fade = 1.f - FMath::SmoothStep(0.22f, 1.f, U);
		Mat->SetScalarParameterValue(TEXT("Opacity"), Fade * (ReducedPresentationMotion() ? 0.68f : 0.92f));
	}
	if (InCut() || bDay || bWon || P.Hp <= 0 || BossIntro > 0.f) return;
	const bool bRestart = LastPresentationState != P.St || P.T < LastPresentationT || LastPresentationCombo != P.Combo;
	const float PreviousT = bRestart ? -1.f : LastPresentationT;
	const float ContactAt = P.St == TEXT("punch") ? 0.06f : P.St == TEXT("wpn") ? 0.13f
		: P.St == TEXT("kick") ? 0.12f : P.St == TEXT("jumpkick") ? 0.025f : -1.f;
	if (ContactAt >= 0.f && P.T >= ContactAt && PreviousT < ContactAt)
	{
		const float Size = P.St == TEXT("punch") ? (P.Combo == 3 ? 104.f : 68.f) : 120.f;
		Accent(EAccent::Slash, World(P.X + P.Dir * 41.f, P.Y + 9.f, P.Z + 100.f), Size, P.Dir, false);   // attack-start Sfx already supplies the whoosh
	}
	LastPresentationState = P.St; LastPresentationT = P.T; LastPresentationCombo = P.Combo;
	// Contact accents start during hit-stop; periodic movement accents wait for the simulation.
	if (ContactPause > 0.f) return;
	MotionAccentTime += Dt;
	if (P.St == TEXT("walk") && P.Z <= 0.f && MotionAccentTime >= (SustainedTravel >= 0.24f ? 0.18f : 0.34f))
	{
		MotionAccentTime = 0.f;
		Accent(EAccent::Dust, World(P.X - P.Dir * 14.f, P.Y + 7.f, 9.f), SustainedTravel >= 0.24f ? 46.f : 30.f, P.Dir, false);
	}
	BossAccentTime += Dt;
	if (BossAccentTime >= 0.12f)
	{
		BossAccentTime = 0.f;
		const int32 Bi = BossIdx();
		if (Foes.IsValidIndex(Bi))
		{
			const auto& B = Foes[Bi];
			if (B.St == TEXT("breathe"))
			{
				for (int32 J = 0; J < 3; ++J)
					Accent(EAccent::Flame, World(B.X + B.Dir * (72.f + J * 85.f), B.SigY + 7.f, 100.f - J * 18.f), 116.f - J * 10.f, B.Dir, false);
				Accent(EAccent::Smoke, World(B.X + B.Dir * 245.f, B.SigY + 6.f, 48.f), 50.f, B.Dir, false);
			}
			else if (B.St == TEXT("whirl") || (B.St == TEXT("sweep") && B.T < 0.25f))
				Accent(EAccent::Slash, World(B.X + B.Dir * 35.f, B.Y + 9.f, 83.f), 135.f, B.Dir, false);
			else if (B.St == TEXT("lunge") || B.St == TEXT("charge"))
				Accent(EAccent::Dust, World(B.X - B.Dir * 28.f, B.Y + 7.f, 11.f), 63.f, B.Dir, false);
			else if (B.St == TEXT("slam") && B.T < 0.12f)
				Accent(EAccent::Land, World(B.X, B.Y + 9.f, 11.f), 165.f, B.Dir);
			else if (B.St == TEXT("fire") && MuzzleT > 0.f)
				Accent(EAccent::Smoke, World(B.X + B.Dir * 78.f, B.Y + 8.f, 126.f), 42.f, B.Dir, false);
		}
		for (const auto& Shot : Shots)
			if (Shot.bBall) Accent(EAccent::Dust, World(Shot.X - FMath::Sign(Shot.Vx) * 20.f, Shot.Y + 5.f, 7.f), 28.f, FMath::Sign(Shot.Vx), false);
	}
}
