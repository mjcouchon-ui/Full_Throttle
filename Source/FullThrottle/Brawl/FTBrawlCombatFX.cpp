#include "FTBrawlGameMode.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/Texture.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Sound/SoundBase.h"

#if __has_include("FTGameInstance.h")
#include "FTGameInstance.h"
#endif

namespace
{
	constexpr int32 ImpactCapacity = 12;
	constexpr float ImpactLife = 0.22f;
	const TCHAR* CueNames[] = { TEXT("Jab"), TEXT("Finish"), TEXT("Weapon"), TEXT("Throw"), TEXT("Hurt"), TEXT("Clear") };
	const float CueSizes[] = { 72.f, 126.f, 106.f, 84.f, 101.f, 0.f };
	const float CueVolume[] = { 0.48f, 0.64f, 0.59f, 0.35f, 0.57f, 0.46f };
}

void AFTBrawlGameMode::InitCombatPresentation()
{
	static_assert(UE_ARRAY_COUNT(CueNames) == static_cast<int32>(ECombatCue::Count));
	static_assert(UE_ARRAY_COUNT(CueSizes) == UE_ARRAY_COUNT(CueNames));
	for (const TCHAR* Name : CueNames)
	{
		CombatSounds.Add(LoadObject<USoundBase>(nullptr,
			*FString::Printf(TEXT("/Game/Brawl/Audio/Polish/S_Brawl%s.S_Brawl%s"), Name, Name)));
		CombatTextures.Add(LoadObject<UTexture>(nullptr,
			*FString::Printf(TEXT("/Game/Brawl/Art/Combat/T_Brawl%s.T_Brawl%s"), Name, Name), nullptr, LOAD_Quiet | LOAD_NoWarn));
	}
	UStaticMesh* Plane = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"));
	UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr,
		TEXT("/Game/Brawl/Materials/M_BrawlNeon.M_BrawlNeon"));
	if (!Plane || !Material) return;
	CombatEffects.SetNum(ImpactCapacity);

	for (int32 Index = 0; Index < ImpactCapacity; ++Index)
	{
		AStaticMeshActor* Quad = GetWorld()->SpawnActor<AStaticMeshActor>();
		CombatQuads.Add(Quad);
		Quad->SetMobility(EComponentMobility::Movable);
		Quad->SetActorHiddenInGame(true);
		UStaticMeshComponent* Mesh = Quad->GetStaticMeshComponent();
		Mesh->SetStaticMesh(Plane);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetCastShadow(false);
		Mesh->bAffectDistanceFieldLighting = false;
		Mesh->SetTranslucentSortPriority(1);
		Mesh->CreateDynamicMaterialInstance(0, Material);
	}
}

void AFTBrawlGameMode::CombatCue(ECombatCue Cue, const FVector& At, float Direction)
{
	const int32 Index = static_cast<int32>(Cue);
	bool bMuted = false;
#if __has_include("FTGameInstance.h")
	if (const UFTGameInstance* Settings = GetGameInstance<UFTGameInstance>()) bMuted = Settings->bMuted;
#endif
	if (!bMuted && CombatSounds.IsValidIndex(Index) && CombatSounds[Index] && CueCooldowns[Index] <= 0.f)
	{
		CueCooldowns[Index] = 0.035f;
		UGameplayStatics::PlaySound2D(this, CombatSounds[Index], CueVolume[Index]);
	}
	if (CombatQuads.IsEmpty() || !CombatTextures.IsValidIndex(Index) || !CombatTextures[Index]) return;

	const int32 Slot = NextCombatQuad++ % CombatQuads.Num();
	NextCombatQuad %= CombatQuads.Num();
	FCombatEffect& Effect = CombatEffects[Slot];
	Effect = { At, 0.f, CueSizes[Index], Direction };
	AStaticMeshActor* Quad = CombatQuads[Slot];
	auto* Mid = CastChecked<UMaterialInstanceDynamic>(Quad->GetStaticMeshComponent()->GetMaterial(0));
	Mid->SetTextureParameterValue(TEXT("Tex"), CombatTextures[Index]);
	Mid->SetScalarParameterValue(TEXT("Glow"), 1.85f);
	Quad->SetActorLocation(At);
	Quad->SetActorRotation(FRotator(0.f, 0.f, 90.f));
	Quad->SetActorScale3D(FVector(Effect.Size / 100.f * Direction, Effect.Size / 100.f, 1.f));
	Quad->SetActorHiddenInGame(false);
}

void AFTBrawlGameMode::TickCombatPresentation(float Dt)
{
	for (float& Cooldown : CueCooldowns) Cooldown = FMath::Max(0.f, Cooldown - Dt);
	for (int32 Index = 0; Index < CombatQuads.Num(); ++Index)
	{
		FCombatEffect& Effect = CombatEffects[Index];
		if (Effect.Size <= 0.f) continue;
		AStaticMeshActor* Quad = CombatQuads[Index];
		Effect.Age += Dt;
		if (Effect.Age >= ImpactLife)
		{
			Effect.Size = 0.f;
			Quad->SetActorHiddenInGame(true);
			continue;
		}
		const float U = Effect.Age / ImpactLife;
		const float Scale = Effect.Size / 100.f * (0.86f + 0.43f * U);
		const float Opacity = FMath::Square(1.f - U);
		Quad->SetActorLocation(Effect.At + FVector(U * 29.f * Effect.Dir, 0, U * 7.f));
		Quad->SetActorScale3D(FVector(Scale * Effect.Dir, Scale, 1.f));
		auto* Mid = CastChecked<UMaterialInstanceDynamic>(Quad->GetStaticMeshComponent()->GetMaterial(0));
		Mid->SetScalarParameterValue(TEXT("Glow"), 2.2f * Opacity);
	}
}
