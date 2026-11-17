#include "FTBrawlGameMode.h"
#include "FTBrawlVenueDetails.h"

#include "Camera/CameraActor.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/PointLight.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/Texture.h"
#include "Materials/MaterialInstanceDynamic.h"

namespace
{
struct FVenueMotion
{
	const TCHAR* Name;
	FLinearColor Colour;
	FLinearColor BossColour;
	float Width;
	float Height;
	float CentreZ;
	float Drift;
	float Lift;
	float Rate;
	float Glow;
};

const FVenueMotion Venues[] = {
	{ TEXT("harbour"), FLinearColor(0.17f, 0.54f, 1.f), FLinearColor(1.f, 0.42f, 0.12f), 410, 92, 284, 42, 4, 0.85f, 0.32f },
	{ TEXT("disco"), FLinearColor(0.67f, 0.31f, 1.f), FLinearColor(1.f, 0.20f, 0.64f), 424, 204, 429, 47, 10, 1.8f, 0.48f },
	{ TEXT("casino"), FLinearColor(1.f, 0.58f, 0.18f), FLinearColor(0.67f, 1.f, 0.44f), 388, 130, 322, 22, 5, 2.8f, 0.38f },
	{ TEXT("embers"), FLinearColor(1.f, 0.35f, 0.07f), FLinearColor(1.f, 0.35f, 0.07f), 350, 192, 327, 24, 28, 1.2f, 0.56f },
	{ TEXT("lanes"), FLinearColor(0.35f, 0.40f, 1.f), FLinearColor(0.12f, 0.86f, 1.f), 430, 145, 302, 54, 4, 2.2f, 0.45f },
	{ TEXT("snow"), FLinearColor(0.55f, 0.77f, 1.f), FLinearColor(0.45f, 0.72f, 1.f), 344, 150, 330, 34, -16, 0.9f, 0.39f },
	{ TEXT("deck"), FLinearColor(0.22f, 0.86f, 0.77f), FLinearColor(0.96f, 0.75f, 0.36f), 414, 145, 313, 43, 7, 1.15f, 0.37f },
};

bool IsSignatureTell(FName State)
{
	static const FName Tells[] = { TEXT("slamwind"), TEXT("whirltell"), TEXT("draw"),
		TEXT("swig"), TEXT("hold"), TEXT("gloves"), TEXT("windup") };
	for (FName Tell : Tells)
		if (State == Tell) return true;
	return false;
}

bool IsSignatureRelease(FName State)
{
	static const FName Releases[] = { TEXT("slam"), TEXT("whirl"), TEXT("fire"),
		TEXT("breathe"), TEXT("bowl"), TEXT("lunge"), TEXT("sweep") };
	for (FName Release : Releases)
		if (State == Release) return true;
	return false;
}
}

void AFTBrawlGameMode::BuildVenueAmbience(float StartX, float EndX, float BackWallY)
{
	UFTBrawlVenueDetails* Details = NewObject<UFTBrawlVenueDetails>(this);
	AddInstanceComponent(Details);
	Details->RegisterComponent();
	Details->Build(Night, StartX, EndX, BackWallY);
	const FVenueMotion& Art = Venues[FMath::Clamp(Night - 1, 0, 6)];
	UStaticMesh* Plane = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"));
	UMaterialInterface* Glow = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlGlow.M_BrawlGlow"));
	const FString Package = FString::Printf(TEXT("/Game/Brawl/Polish/T_venue_%s.T_venue_%s"), Art.Name, Art.Name);
	UTexture* Texture = LoadObject<UTexture>(nullptr, *Package, nullptr, LOAD_Quiet | LOAD_NoWarn);
	auto GlowCard = [&](const FVector& At, const FRotator& Rotation, const FVector& Scale, UTexture* Mask, const FLinearColor& Colour)
	{
		AStaticMeshActor* Card = GetWorld()->SpawnActor<AStaticMeshActor>(At, Rotation);
		if (!Card) return Card;
		Card->SetMobility(EComponentMobility::Movable);
		UStaticMeshComponent* Mesh = Card->GetStaticMeshComponent();
		Mesh->SetStaticMesh(Plane);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mesh->SetCastShadow(false);
		Mesh->SetWorldScale3D(Scale);
		UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(Glow, Card);
		Material->SetTextureParameterValue(TEXT("Tex"), Mask);
		Material->SetVectorParameterValue(TEXT("Color"), Colour);
		Material->SetScalarParameterValue(TEXT("Glow"), Art.Glow);
		Mesh->SetMaterial(0, Material);
		return Card;
	};

	if (Plane && Glow && Texture)
	{
		const float PanelWidth = 840.f;
		for (int32 Panel = 0; StartX + Panel * PanelWidth < EndX; ++Panel)
		{
			// Harbour reflections and lodge flurries are concentrated at their painted windows.
			if ((Night == 1 || Night == 6) && Panel % 3 != 2) continue;
			const float CentreX = StartX + (Panel + (Panel % 2 ? 0.42f : 0.57f)) * PanelWidth;
			const FVector At(CentreX, BackWallY + 13.f, Art.CentreZ + (Panel % 3 - 1) * 14.f);
			if (AStaticMeshActor* Card = GlowCard(At, FRotator(0.f, 0.f, 90.f),
				FVector(Art.Width / 100.f, Art.Height / 100.f, 1.f), Texture, Art.Colour))
			{
				VenueAmbience.Add(Card);
				VenueAmbienceOrigins.Add(At);
			}
		}
	}

	if (Night == 1 && Plane && Glow)
	{
		// Earl's slam uses the same visual vocabulary as the later boss specials.
		if (UTexture* Ring = LoadObject<UTexture>(nullptr, TEXT("/Game/Brawl/Velvet/T_ring.T_ring")))
		{
			const float Scale = World(1.f, 0.f, 0.f).X / 100.f;
			SlamAccent = GlowCard(FVector(0.f, 0.f, 3.f), FRotator::ZeroRotator,
				FVector(480.f * Scale, 180.f * Scale, 1.f), Ring, Art.BossColour);
			if (SlamAccent) SlamAccent->SetActorHiddenInGame(true);
		}
	}

	BossAccent = GetWorld()->SpawnActor<APointLight>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (BossAccent)
	{
		UPointLightComponent* Light = BossAccent->PointLightComponent;
		Light->SetMobility(EComponentMobility::Movable);
		Light->SetCastShadows(false);
		Light->SetLightColor(Art.BossColour);
		Light->SetAttenuationRadius(400.f);
		Light->SetVisibility(false);
	}
}

void AFTBrawlGameMode::TickVenueAmbience(float RealDt)
{
	if (VenueAmbience.IsEmpty() || ReducedPresentationMotion()) return;
	VenueAmbienceTime += FMath::Min(RealDt, 0.1f);
	const FVenueMotion& Art = Venues[FMath::Clamp(Night - 1, 0, 6)];
	const float CameraX = CamActor ? CamActor->GetActorLocation().X : 0.f;
	for (int32 i = 0; i < VenueAmbience.Num(); ++i)
	{
		AStaticMeshActor* Card = VenueAmbience[i];
		const FVector& Home = VenueAmbienceOrigins[i];
		if (!Card || FMath::Abs(Home.X - CameraX) > 2000.f) continue;
		const float Phase = 1.77f * i + VenueAmbienceTime * Art.Rate;
		const float Sway = FMath::Sin(Phase);
		const float Modulation = FMath::Sin(Phase * 1.61f + i * 0.7f);
		Card->SetActorLocation(Home + FVector(Art.Drift * Sway, 0.f, Art.Lift * Modulation));
		if (UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(Card->GetStaticMeshComponent()->GetMaterial(0)))
			Material->SetScalarParameterValue(TEXT("Glow"), Art.Glow * (0.67f + 0.21f * Sway + 0.12f * Modulation));
	}
}

void AFTBrawlGameMode::StyleBossPose(FName& Pose, float& PoseTime) const
{
	const float EntranceAge = StT - BossEntranceAt;
	if (Pose == TEXT("dead") && bWon)
	{
		Pose = WinT < 0.42f ? FName(TEXT("defeat")) : FName(TEXT("down"));
		PoseTime = WinT;
	}
	else if (EntranceAge < 2.82f && (Pose == TEXT("idle") || Pose == TEXT("walk")))
	{
		Pose = EntranceAge < 2.46f ? FName(TEXT("intro")) : FName(TEXT("intro2"));
		PoseTime = EntranceAge;
	}
	else StyleBossSignature(Night, Pose, PoseTime);
}

// (split out so that the development pose stage, FTBrawlPose.cpp, renames a posed boss's state by the same rule)
void AFTBrawlGameMode::StyleBossSignature(int32 BossNight, FName& Pose, float PoseTime)
{
	if ((BossNight == 2 && Pose == TEXT("dizzy") && PoseTime < 0.35f)
		|| (BossNight == 3 && Pose == TEXT("fire") && PoseTime >= 0.6f))
	{
		// Vinnie's first stunned breath and the Pit Boss's settled aim after the final shot.
		Pose = TEXT("signaturefollow");
	}
}

float AFTBrawlGameMode::BossEntranceCameraOffset() const
{
	if (!bBossOut || bWon || ReducedPresentationMotion()) return 0.f;
	const float Age = StT - BossEntranceAt;
	const int32 Index = BossIdx();
	if (Index == INDEX_NONE) return 0.f;
	// The introduction freezes the boss for 2.2 s. Returning solely by elapsed
	// time could pan away before his walk-in reached the arena's safe boundary.
	const float WalkIn = FMath::SmoothStep(LockX + 890.f, LockX + 1040.f, Foes[Index].X);
	const float Intro = 1.f - FMath::SmoothStep(2.07f, 2.82f, Age);
	return 145.f * FMath::SmoothStep(0.f, 0.42f, Age) * FMath::Max(Intro, WalkIn);
}

void AFTBrawlGameMode::SyncBossAccent()
{
	const int32 BossIndex = BossIdx();
	const FFTBrawlBody* Boss = BossIndex == INDEX_NONE ? nullptr : &Foes[BossIndex];
	const bool bTell = Boss && IsSignatureTell(Boss->St);
	const bool bRelease = Boss && IsSignatureRelease(Boss->St);
	const bool bEntrance = Boss && StT - BossEntranceAt < 2.82f;
	const bool bDefeat = Boss && Boss->St == TEXT("dead") && WinT < 0.8f;
	if (BossAccent)
	{
		UPointLightComponent* Light = BossAccent->PointLightComponent;
		const bool bLit = bTell || bRelease || bEntrance || bDefeat;
		Light->SetVisibility(bLit);
		if (bLit)
		{
			const float Stage = bRelease ? 0.25f + 0.75f * FMath::Exp(-Boss->T * 7.f)
				: bDefeat ? FMath::Exp(-WinT * 4.f) : bEntrance ? 0.60f : 0.24f + FMath::Min(0.44f, Boss->T);
			BossAccent->SetActorLocation(World(Boss->X, Boss->Y, Boss->Z) + FVector(Boss->Dir * 44.f, 95.f, 170.f));
			Light->SetIntensity(16000.f * (ReducedPresentationMotion() ? 0.45f : Stage));
		}
	}
	if (SlamAccent)
	{
		const bool bSlamming = Boss && (Boss->St == TEXT("slamwind") || (Boss->St == TEXT("slam") && Boss->T < 0.38f));
		SlamAccent->SetActorHiddenInGame(!bSlamming);
		if (bSlamming)
		{
			SlamAccent->SetActorLocation(World(Boss->X, Boss->Y, 0.f) + FVector(0.f, 0.f, 3.f));
			if (UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(SlamAccent->GetStaticMeshComponent()->GetMaterial(0)))
				Material->SetScalarParameterValue(TEXT("Glow"), ReducedPresentationMotion() ? 1.4f
					: bTell ? 0.8f + 2.1f * Boss->T : 3.0f * FMath::Exp(-Boss->T * 8.f));
		}
	}
}
