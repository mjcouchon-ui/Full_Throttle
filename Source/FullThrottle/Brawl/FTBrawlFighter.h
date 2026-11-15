// One body in the brawl (Nova, a drunk, a tough, a bouncer, Big Earl): a blocky 3D figure built from the
// engine's basic shapes, coloured per look, posed in code each frame from the fight's state (walk swing,
// punch, kick, jump kick, hurt, knocked flying, down, get up). The cel pass bands and inks it.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FTBrawlFighter.generated.h"

class UStaticMeshComponent;
class USkeletalMeshComponent;
class UAnimSequence;
class UMaterialInstanceDynamic;
class UTexture;

USTRUCT()
struct FFTBrawlLook
{
	GENERATED_BODY()
	FLinearColor Skin = FLinearColor(0.9f, 0.62f, 0.45f);
	FLinearColor Shirt = FLinearColor::White;
	FLinearColor Pants = FLinearColor(0.1f, 0.12f, 0.3f);
	FLinearColor Hair = FLinearColor(0.08f, 0.05f, 0.04f);
	float Girth = 1.f;   // torso width (Big Earl, bouncers)
};

UCLASS()
class AFTBrawlFighter : public AActor
{
	GENERATED_BODY()
public:
	AFTBrawlFighter();
	void SetLook(const FFTBrawlLook& L);
	// the Meshy cast: /Game/Brawl/Characters/<Who>/SK_<Who> + A_<Who>_<who><clip>. False = not imported (blocky body stays).
	bool SetCast(const FString& Who, float HeightCm);
	bool HasCast() const { return Mesh != nullptr || Sprite != nullptr; }
	// 2.5D: painted frames /Game/Brawl/Sprites/<Who>/T_<who>_<frame> on a lit, shadow-casting quad facing the camera.
	// HeightCm = the character's standing height (600 px in the frame), Tint multiplies the paint. False = no frames.
	bool SetSprite(const FString& Who, float HeightCm, const FLinearColor& Tint = FLinearColor::White);
	bool HasSprite() const { return Sprite != nullptr; }
	bool HasFrame(FName N) const { return Frames.Contains(N); }   // cutscenes: pick a pose the sheet has (run 32365)
	void SetTint(const FLinearColor& C);   // multiplies the paint (power-up glow)
	// Pose: state name as in brawl.js, t = time in state, walk phase, lean in degrees (+ = backwards), flash 0..1
	void Pose(FName St, float T, float Walk, int32 Combo, float LeanDeg, float Flash, bool bVisible);

private:
	UPROPERTY() TObjectPtr<USceneComponent> Root;
	UPROPERTY() TObjectPtr<USceneComponent> Body;
	UPROPERTY() TObjectPtr<USceneComponent> ArmF;
	UPROPERTY() TObjectPtr<USceneComponent> ArmB;
	UPROPERTY() TObjectPtr<USceneComponent> LegF;
	UPROPERTY() TObjectPtr<USceneComponent> LegB;
	UPROPERTY() TArray<TObjectPtr<UStaticMeshComponent>> Parts;
	UPROPERTY() TArray<TObjectPtr<UMaterialInstanceDynamic>> Mids;
	TArray<FLinearColor> Base;
	UStaticMeshComponent* Part(USceneComponent* Parent, const TCHAR* MeshPath, FVector Loc, FVector Scale, int32 Slot);
	FFTBrawlLook Look;
	// skeletal cast
	UPROPERTY() TObjectPtr<USkeletalMeshComponent> Mesh;
	UPROPERTY() TMap<FName, TObjectPtr<UAnimSequence>> Clips;
	UAnimSequence* Clip(std::initializer_list<const TCHAR*> Names) const;
	FName LastKey; float LastT = 0; FName LastSt;
	void PoseCast(FName St, float T, int32 Combo, float Flash, bool bVisible);
	// sprite
	UPROPERTY() TObjectPtr<UStaticMeshComponent> Sprite;
	UPROPERTY() TObjectPtr<UMaterialInstanceDynamic> SpriteMid;
	UPROPERTY() TMap<FName, TObjectPtr<UTexture>> Frames;
	UPROPERTY() TObjectPtr<UTexture> CurFrame;
	UTexture* Frame(std::initializer_list<const TCHAR*> Names) const;
	void PoseSprite(FName St, float T, float Walk, int32 Combo, float LeanDeg, float Flash, bool bVisible);
};
