#pragma once
#include "CoreMinimal.h"
struct FMinimalViewInfo;
class UWorld;

// Opt-in visual inspection only. No entities, rewards, hitboxes or spawn schedules are modified.
namespace FTPickupReview
{
	bool Camera(UWorld* World, const FTransform& Anchor, int32 Level, float Time, float Dt, FMinimalViewInfo& POV);
}
