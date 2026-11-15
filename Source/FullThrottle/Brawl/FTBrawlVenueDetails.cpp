#include "FTBrawlVenueDetails.h"
#include "FTBrawlGameMode.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Materials/MaterialInstanceDynamic.h"

namespace
{
	const FLinearColor Iron(0.025f, 0.041f, 0.07f), Brass(0.56f, 0.28f, 0.07f);
	const FLinearColor Chrome(0.36f, 0.50f, 0.57f), Cream(0.94f, 0.72f, 0.35f);
	const FLinearColor Coral(0.78f, 0.13f, 0.16f), Teal(0.06f, 0.62f, 0.61f);
	const FLinearColor Violet(0.45f, 0.13f, 0.63f), Ice(0.45f, 0.67f, 1.f);
}

UFTBrawlVenueDetails::UFTBrawlVenueDetails()
{
	PrimaryComponentTick.bCanEverTick = true;
	// Environmental motion is deliberately sampled less often than combat.
	PrimaryComponentTick.TickInterval = 1.f / 30.f;
}

int32 UFTBrawlVenueDetails::Batch(const TCHAR* Shape, const FLinearColor& Colour, bool bEmissive)
{
	const FString Key = FString::Printf(TEXT("%s_%08x_%d"), Shape, Colour.ToFColor(false).DWColor(), int32(bEmissive));
	const int32 Existing = BatchKeys.IndexOfByKey(Key);
	if (Existing != INDEX_NONE) return Existing;
	const FString Asset = FString::Printf(TEXT("/Engine/BasicShapes/%s.%s"), Shape, Shape);
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *Asset);
	UMaterialInterface* Base = LoadObject<UMaterialInterface>(nullptr, bEmissive
		? TEXT("/Game/Brawl/Materials/M_BrawlFlash.M_BrawlFlash")
		: TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (!Mesh || !Base) return INDEX_NONE;
	UInstancedStaticMeshComponent* Instances = NewObject<UInstancedStaticMeshComponent>(GetOwner());
	Instances->SetMobility(EComponentMobility::Movable);
	Instances->SetStaticMesh(Mesh);
	Instances->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Instances->SetCanEverAffectNavigation(false);
	Instances->SetCastShadow(false);
	Instances->SetCullDistances(2400, 3200);
	Instances->RegisterComponent();
	UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(Base, this);
	Material->SetVectorParameterValue(TEXT("Color"), Colour);
	if (bEmissive) Material->SetScalarParameterValue(TEXT("Amount"), 0.85f);
	Instances->SetMaterial(0, Material);
	BatchKeys.Add(Key);
	return Batches.Add(Instances);
}

void UFTBrawlVenueDetails::Piece(const TCHAR* Shape, const FVector& At, const FVector& Size,
	const FLinearColor& Colour, const FRotator& Rotation, EMotion Motion, float Phase, float Rate,
	float Range, const FVector& Pivot, bool bEmissive)
{
	const int32 B = Batch(Shape, Colour, bEmissive);
	if (B == INDEX_NONE) return;
	const FTransform Home(Rotation, At, Size / 100.f);
	const int32 Instance = Batches[B]->AddInstance(Home, true);
	if (Motion != EMotion::Still) Moving.Add({ B, Instance, Home, Pivot, Motion, Phase, Rate, Range });
	++PieceCount;
}

void UFTBrawlVenueDetails::Fan(const FVector& At, const FLinearColor& Metal, float Phase)
{
	// The cage and mount stay still while three individually modelled blades rotate inside.
	Piece(TEXT("Cylinder"), At + FVector(0, -2, 0), FVector(88, 88, 8), Iron, FRotator(0, 0, 90));
	Piece(TEXT("Cylinder"), At, FVector(77, 77, 7), Metal, FRotator(0, 0, 90));
	Piece(TEXT("Cylinder"), At + FVector(0, 5, 0), FVector(68, 68, 7), Iron, FRotator(0, 0, 90));
	for (int32 Blade = 0; Blade < 3; ++Blade)
	{
		const float Angle = Blade * 120.f;
		const FQuat Q(FVector::YAxisVector, FMath::DegreesToRadians(Angle));
		Piece(TEXT("Cube"), At + FVector(0, 10, 0) + Q.RotateVector(FVector(18, 0, 0)), FVector(32, 3, 11), Metal,
			FRotator(Angle, 0, 0), EMotion::Rotor, Phase, 1.2f, 0.f, At + FVector(0, 10, 0));
	}
	for (int32 Bar = -2; Bar <= 2; ++Bar)
	{
		const float X = Bar * 12.f, Height = 2.f * FMath::Sqrt(FMath::Max(0.f, 32.f * 32.f - X * X));
		Piece(TEXT("Cube"), At + FVector(X, 15, 0), FVector(1.8f, 2.f, Height), Metal);
	}
	Piece(TEXT("Sphere"), At + FVector(0, 17, 0), FVector(12, 5, 12), Brass);
}

void UFTBrawlVenueDetails::Pennants(const FVector& At, const FLinearColor& Accent, float Phase)
{
	// Each pennant pivots at its tie point. Two tapering pieces make the swallowtail silhouette.
	for (int32 Flag = 0; Flag < 5; ++Flag)
	{
		const float X = Flag * 55.f, Z = -FMath::Sin(Flag * PI / 4.f) * 18.f;
		const FVector Tie = At + FVector(X, 0, Z);
		Piece(TEXT("Cube"), Tie + FVector(0, -2, 1), FVector(57, 1.7f, 1.7f), Brass,
			FRotator(Flag < 2 ? 8.f : -8.f, 0, 0));
		const FLinearColor C = Flag % 2 ? Cream : Accent;
		Piece(TEXT("Cube"), Tie + FVector(0, 1, -9), FVector(25, 2, 17), C,
			FRotator::ZeroRotator, EMotion::Sway, Phase + Flag * 0.7f, 1.5f, 5.f, Tie);
		Piece(TEXT("Cone"), Tie + FVector(0, 1, -22), FVector(25, 2, 16), C,
			FRotator(180, 0, 0), EMotion::Sway, Phase + Flag * 0.7f, 1.5f, 5.f, Tie);
	}
}

void UFTBrawlVenueDetails::Build(int32 Night, float StartX, float EndX, float WallY)
{
	constexpr float PW = 840.f;
	for (int32 Panel = 0; StartX + Panel * PW < EndX; ++Panel)
	{
		const float X = StartX + Panel * PW, Y = WallY + 24.f, Phase = Panel * 1.619f;
		const int32 Kind = Panel % 3;
		if (Night == 1)
		{
			if (Kind == 1) Fan(FVector(X + 715, Y, 470), Chrome, Phase);
			if (Kind == 2)
			{
				// Small moving harbour lights stay within the existing window panes.
				for (int32 Light = 0; Light < 6; ++Light)
					Piece(TEXT("Cube"), FVector(X + 285 + Light * 42, Y - 8, 291), FVector(8, 1, 2.2f), Light % 2 ? Teal : Cream,
						FRotator::ZeroRotator, EMotion::Glint, Phase + Light, 0.65f, 0.f, FVector::ZeroVector, true);
			}
		}
		else if (Night == 2)
		{
			if (Kind == 1)
			{
				Fan(FVector(X + 755, Y, 450), Violet, Phase);
				// A slow marquee chase frames the booth, above head height.
				for (int32 Bulb = 0; Bulb < 12; ++Bulb)
					Piece(TEXT("Sphere"), FVector(X + 155 + Bulb * 46, Y + 3, 472), FVector(6.5, 3, 6.5), Bulb % 2 ? Teal : Coral,
						FRotator::ZeroRotator, EMotion::Chase, Bulb * 0.12f + Phase, 0.4f, 0.f, FVector::ZeroVector, true);
			}
		}
		else if (Night == 3)
		{
			if (Kind == 1)
			{
				for (int32 Slot = 0; Slot < 4; ++Slot)
				{
					const float SX = X + (0.17f + Slot * 0.20f) * PW;
					for (int32 Bulb = 0; Bulb < 7; ++Bulb)
						Piece(TEXT("Sphere"), FVector(SX - 32.f + Bulb * 10.6f, Y + 2, 405), FVector(4, 2, 4), Slot % 2 ? Cream : Coral,
							FRotator::ZeroRotator, EMotion::Chase, Bulb * 0.13f + Slot * 0.2f, 0.36f, 0.f, FVector::ZeroVector, true);
				}
			}
			if (Kind == 2) Fan(FVector(X + 748, Y, 430), Brass, Phase);
		}
		else if (Night == 4)
		{
			if (Kind == 1)
			{
				Pennants(FVector(X + 300, Y + 16, 495), Teal, Phase);
				for (float U : { 0.08f, 0.92f })
				{
					const FVector Fire(X + U * PW, Y + 12.f, 281.f);
					for (int32 Ember = 0; Ember < 8; ++Ember)
						Piece(TEXT("Cube"), Fire, FVector(1.7f, 1.f, 3.f), Ember % 2 ? Cream : Coral,
							FRotator(35, 0, 0), EMotion::Ember, Ember / 8.f + Phase, 0.32f, 57.f, FVector::ZeroVector, true);
				}
			}
		}
		else if (Night == 5)
		{
			if (Kind == 1)
			{
				// A restrained mechanical return beneath the painted lane display.
				const FVector Return(X + 580, Y + 14, 39);
				Piece(TEXT("Cube"), Return, FVector(150, 15, 12), Iron);
				Piece(TEXT("Cube"), Return + FVector(0, 8, 5), FVector(148, 2, 2), Chrome);
				Piece(TEXT("Sphere"), Return + FVector(0, 1, 15), FVector(20, 20, 20), Teal,
					FRotator::ZeroRotator, EMotion::ReturnBall, Phase, 0.12f, 59.f);
				for (int32 Bulb = 0; Bulb < 10; ++Bulb)
					Piece(TEXT("Cube"), FVector(X + 165 + Bulb * 56, Y + 2, 465), FVector(10, 2, 3), Bulb % 2 ? Teal : Coral,
						FRotator::ZeroRotator, EMotion::Chase, Bulb * 0.1f, 0.5f, 0.f, FVector::ZeroVector, true);
			}
		}
		else if (Night == 6)
		{
			if (Kind == 2)
			{
				// Three separate pane regions prevent snow drifting over the painted mullions.
				for (int32 Pane = 0; Pane < 3; ++Pane)
					for (int32 Flake = 0; Flake < 12; ++Flake)
					{
						const float Fx = X + 146.f + Pane * 210.f + FMath::Fmod(Flake * 59.f, 140.f);
						Piece(TEXT("Cube"), FVector(Fx, Y - 8, 405), FVector(2.1, 1, 3.1), Ice,
							FRotator(35, 0, 0), EMotion::Snow, Flake / 12.f + Pane * 0.17f, 0.10f + Pane * 0.018f, 140.f, FVector::ZeroVector, true);
					}
			}
			if (Kind == 1)
				for (int32 Ember = 0; Ember < 10; ++Ember)
					Piece(TEXT("Cube"), FVector(X + 420, Y, 137), FVector(1.5, 1, 2.5), Cream,
						FRotator(35, 0, 0), EMotion::Ember, Ember / 10.f, 0.23f, 45.f, FVector::ZeroVector, true);
		}
		else if (Night == 7)
		{
			if (Kind == 1)
			{
				Pennants(FVector(X + 280, Y + 35, 458), Coral, Phase);
				for (int32 Reflection = 0; Reflection < 9; ++Reflection)
					Piece(TEXT("Cube"), FVector(X + 200 + Reflection * 56, Y - 8, 246 - Reflection % 3 * 13), FVector(17, 1, 2), Reflection % 2 ? Teal : Cream,
						FRotator::ZeroRotator, EMotion::Glint, Phase + Reflection * 0.9f, 0.6f, 0.f, FVector::ZeroVector, true);
			}
		}
	}
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL venue details night=%d pieces=%d moving=%d batches=%d"), Night, PieceCount, Moving.Num(), Batches.Num());
}

void UFTBrawlVenueDetails::TickComponent(float Dt, ELevelTick TickType, FActorComponentTickFunction* ThisTick)
{
	Super::TickComponent(Dt, TickType, ThisTick);
	if (const AFTBrawlGameMode* Game = Cast<AFTBrawlGameMode>(GetOwner()))
		if (Game->SystemScreen != 0 || Game->ReducedPresentationMotion()) return;
	Clock += FMath::Min(Dt, 0.1f);
	float CameraX = 0.f;
	if (const APlayerController* PC = GetWorld()->GetFirstPlayerController())
		if (const AActor* View = PC->GetViewTarget()) CameraX = View->GetActorLocation().X;
	TSet<int32> Dirty;
	for (const FPiece& P : Moving)
	{
		const FVector Home = P.Home.GetLocation();
		if (FMath::Abs(Home.X - CameraX) > 2300.f) continue;
		const float T = Clock * P.Rate + P.Phase, Wave = FMath::Sin(T * 2.f * PI);
		FTransform Transform = P.Home;
		if (P.Motion == EMotion::Rotor || P.Motion == EMotion::Sway)
		{
			const float Angle = P.Motion == EMotion::Rotor ? T * 2.f * PI : FMath::DegreesToRadians(Wave * P.Range);
			const FQuat Q(FVector::YAxisVector, Angle);
			Transform.SetLocation(P.Pivot + Q.RotateVector(Home - P.Pivot));
			Transform.SetRotation(Q * P.Home.GetRotation());
		}
		else if (P.Motion == EMotion::Chase || P.Motion == EMotion::Glint)
		{
			const float Gain = P.Motion == EMotion::Chase ? 0.60f + 0.40f * FMath::Pow(FMath::Max(0.f, Wave), 2.f) : 0.78f + 0.22f * Wave;
			Transform.SetScale3D(P.Home.GetScale3D() * FVector(Gain, 1.f, Gain));
		}
		else if (P.Motion == EMotion::Snow || P.Motion == EMotion::Ember)
		{
			const float Life = FMath::Frac(T), Fade = FMath::Min3(1.f, Life * 8.f, (1.f - Life) * 8.f);
			const bool bSnow = P.Motion == EMotion::Snow;
			Transform.SetLocation(Home + FVector(Wave * (bSnow ? 7.f : 12.f), 0, (bSnow ? -1.f : 1.f) * Life * P.Range));
			Transform.SetScale3D(P.Home.GetScale3D() * FMath::Max(0.02f, Fade));
		}
		else if (P.Motion == EMotion::ReturnBall)
		{
			const float Cycle = FMath::Frac(T), Run = FMath::Clamp(Cycle / 0.65f, 0.f, 1.f);
			Transform.SetLocation(Home + FVector(FMath::Lerp(-P.Range, P.Range, Run), 0, 0));
			Transform.SetRotation(FQuat(FVector::YAxisVector, -Run * 5.f));
			const float Fade = FMath::Min(FMath::Clamp(Cycle * 14.f, 0.f, 1.f), FMath::Clamp((0.80f - Cycle) * 14.f, 0.f, 1.f));
			Transform.SetScale3D(P.Home.GetScale3D() * FMath::Max(0.02f, Fade));
		}
		Batches[P.Batch]->UpdateInstanceTransform(P.Instance, Transform, true, false, true);
		Dirty.Add(P.Batch);
	}
	for (int32 B : Dirty) Batches[B]->MarkRenderStateDirty();
}
