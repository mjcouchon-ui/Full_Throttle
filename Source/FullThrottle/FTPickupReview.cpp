#include "FTPickupReview.h"
#include "FTEntity.h"
#include "Camera/CameraTypes.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Components/DirectionalLightComponent.h"
#include "HAL/IConsoleManager.h"

#if !UE_BUILD_SHIPPING
namespace
{
	TAutoConsoleVariable<int32> Mode(TEXT("ft.PickupReview"), 0, TEXT("Development visual stage: 1 = turntable, 2 = 100 m, 3 = 121 mph approach and burst. Never use for normal-play GPU timing."));
	TAutoConsoleVariable<float> Yaw(TEXT("ft.PickupReviewYaw"), 0.f, TEXT("Rotate the inspection camera in degrees; 180 compares the opposite sunlight direction."));
	TAutoConsoleVariable<int32> SunView(TEXT("ft.PickupReviewSun"), 0, TEXT("Development inspection: 1 faces the brightest visible directional light; 2 faces away. Overrides orbit/yaw."));
	TWeakObjectPtr<UWorld> ReviewWorld;
	TWeakObjectPtr<AFTEntityActor> Props[2];
	bool bBurst = false;
	float StartTime = 0.f;
	int32 PreviousMode = 0;
}
#endif

bool FTPickupReview::Camera(UWorld* World, const FTransform& Anchor, int32 Level, float Time, float Dt, FMinimalViewInfo& POV)
{
#if !UE_BUILD_SHIPPING
	const int32 M = Mode.GetValueOnGameThread();
	if (M == 0)
	{
		for (auto& Prop : Props) { if (Prop.IsValid()) Prop->Destroy(); Prop.Reset(); }
		ReviewWorld.Reset();
		PreviousMode = 0;
		return false;
	}
	if (!World) return false;
	if (ReviewWorld.Get() != World || PreviousMode != M || !Props[0].IsValid() || !Props[1].IsValid())
	{
		for (int32 I = 0; I < 2; ++I)
		{
			if (Props[I].IsValid()) Props[I]->Destroy();
			Props[I] = World->SpawnActor<AFTEntityActor>();
			if (Props[I].IsValid()) Props[I]->Setup(I ? EFTKind::Wrench : EFTKind::Crate, 0, Level);
		}
		ReviewWorld = World; StartTime = Time; bBurst = false; PreviousMode = M;
		UE_LOG(LogTemp, Display, TEXT("POWERUPS REVIEW staged visual inspection mode=%d level=%d; excluded from normal-play timing"), M, Level);
	}
	const float Age = Time - StartTime;
	const float Distance = M == 2 ? 10000.f : (M == 3 ? FMath::Max(450.f, 10000.f - FMath::Max(0.f, Age - 2.f) * 5400.f) : 450.f);
	float AngleDegrees = Yaw.GetValueOnGameThread() + (M == 1 ? Age * 30.f : 0.f);
	if (SunView.GetValueOnGameThread() != 0)
	{
		UDirectionalLightComponent* Key = nullptr;
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			TInlineComponentArray<UDirectionalLightComponent*> Lights;
			It->GetComponents(Lights);
			for (UDirectionalLightComponent* Light : Lights)
				if (Light->ShouldRender() && Light->bAffectsWorld && Light->Intensity > 0.f && (!Key || Light->Intensity > Key->Intensity)) Key = Light;
		}
		if (Key)
		{
			const FVector ToSun = Anchor.InverseTransformVectorNoScale(-Key->GetForwardVector());
			AngleDegrees = FMath::RadiansToDegrees(FMath::Atan2(ToSun.Y, ToSun.X)) + (SunView.GetValueOnGameThread() == 2 ? 180.f : 0.f);
		}
	}
	const float Angle = FMath::DegreesToRadians(AngleDegrees);
	for (int32 I = 0; I < 2; ++I)
	{
		if (!Props[I].IsValid()) continue;
		FTransform At = Anchor;
		At.SetLocation(Anchor.TransformPosition(FVector(0, I ? 130.f : -130.f, 0)));
		Props[I]->SetActorTransform(At);
		Props[I]->Animate(Dt, 0.f);
		if (M == 3 && Age >= 5.f)
		{
			if (!bBurst) AFTEntityActor::PickupBurst(World, I ? EFTKind::Wrench : EFTKind::Crate, At);
			Props[I]->SetActorHiddenInGame(true);
		}
	}
	if (M == 3 && Age >= 5.f) bBurst = true;
	const FVector Look = Anchor.TransformPosition(FVector(0, 0, 70.f));
	POV.Location = Anchor.TransformPosition(FVector(-Distance * FMath::Cos(Angle), -Distance * FMath::Sin(Angle), M == 1 ? 235.f : 290.f));
	POV.Rotation = FRotationMatrix::MakeFromXZ((Look - POV.Location).GetSafeNormal(), Anchor.GetUnitAxis(EAxis::Z)).Rotator();
	POV.FOV = M == 1 ? 58.f : 86.f;
	return true;
#else
	return false;
#endif
}
