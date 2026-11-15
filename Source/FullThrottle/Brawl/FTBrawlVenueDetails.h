// Decorative, instanced venue machinery. It never participates in Brawl collision or targeting.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FTBrawlVenueDetails.generated.h"

class UInstancedStaticMeshComponent;

UCLASS()
class UFTBrawlVenueDetails : public UActorComponent
{
	GENERATED_BODY()
public:
	UFTBrawlVenueDetails();
	void Build(int32 Night, float StartX, float EndX, float BackWallY);
	virtual void TickComponent(float Dt, ELevelTick TickType, FActorComponentTickFunction* ThisTick) override;

private:
	enum class EMotion : uint8 { Still, Rotor, Sway, Chase, Ember, Snow, ReturnBall, Glint };
	struct FPiece
	{
		int32 Batch = 0, Instance = 0;
		FTransform Home;
		FVector Pivot = FVector::ZeroVector;
		EMotion Motion = EMotion::Still;
		float Phase = 0.f, Rate = 1.f, Range = 0.f;
	};
	UPROPERTY() TArray<TObjectPtr<UInstancedStaticMeshComponent>> Batches;
	TArray<FString> BatchKeys;
	TArray<FPiece> Moving;
	float Clock = 0.f;
	int32 PieceCount = 0;
	int32 Batch(const TCHAR* Shape, const FLinearColor& Colour, bool bEmissive = false);
	void Piece(const TCHAR* Shape, const FVector& At, const FVector& Size, const FLinearColor& Colour,
		const FRotator& Rotation = FRotator::ZeroRotator, EMotion Motion = EMotion::Still,
		float Phase = 0.f, float Rate = 1.f, float Range = 0.f, const FVector& Pivot = FVector::ZeroVector, bool bEmissive = false);
	void Fan(const FVector& At, const FLinearColor& Metal, float Phase);
	void Pennants(const FVector& At, const FLinearColor& Accent, float Phase);
};
