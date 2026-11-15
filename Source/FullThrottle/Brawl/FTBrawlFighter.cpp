#include "FTBrawlFighter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimSequence.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Engine/Texture.h"

// colour slots: 0 skin, 1 shirt, 2 pants, 3 hair
AFTBrawlFighter::AFTBrawlFighter()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Body = CreateDefaultSubobject<USceneComponent>(TEXT("Body"));
	Body->SetupAttachment(Root);
	auto Pivot = [this](const TCHAR* N, FVector L) {
		USceneComponent* P = CreateDefaultSubobject<USceneComponent>(N);
		P->SetupAttachment(Body); P->SetRelativeLocation(L); return P;
	};
	ArmF = Pivot(TEXT("ArmF"), FVector(0, -24, 142));
	ArmB = Pivot(TEXT("ArmB"), FVector(0, 24, 142));
	LegF = Pivot(TEXT("LegF"), FVector(0, -11, 88));
	LegB = Pivot(TEXT("LegB"), FVector(0, 11, 88));
}

UStaticMeshComponent* AFTBrawlFighter::Part(USceneComponent* Parent, const TCHAR* MeshPath, FVector Loc, FVector Scale, int32 Slot)
{
	UStaticMesh* M = LoadObject<UStaticMesh>(nullptr, MeshPath);
	UStaticMeshComponent* C = NewObject<UStaticMeshComponent>(this);
	C->SetStaticMesh(M);
	C->SetupAttachment(Parent);
	C->SetRelativeLocation(Loc);
	C->SetRelativeScale3D(Scale);
	C->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	C->SetCastShadow(true);
	C->RegisterComponent();
	UMaterialInterface* BaseMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	UMaterialInstanceDynamic* D = UMaterialInstanceDynamic::Create(BaseMat, this);
	C->SetMaterial(0, D);
	Parts.Add(C); Mids.Add(D); Base.Add(FLinearColor(float(Slot), 0, 0));
	return C;
}

void AFTBrawlFighter::SetLook(const FFTBrawlLook& L)
{
	Look = L;
	if (Mesh) return;
	if (Parts.Num() == 0)
	{
		const TCHAR* Cube = TEXT("/Engine/BasicShapes/Cube.Cube");
		const TCHAR* Sph = TEXT("/Engine/BasicShapes/Sphere.Sphere");
		const TCHAR* Cyl = TEXT("/Engine/BasicShapes/Cylinder.Cylinder");
		const float G = L.Girth;
		Part(Body, Cube, FVector(0, 0, 118), FVector(0.30f * G, 0.44f * G, 0.6f), 1);      // torso
		Part(Body, Cube, FVector(0, 0, 92), FVector(0.28f * G, 0.40f * G, 0.12f), 2);      // belt line
		Part(Body, Sph, FVector(0, 0, 166), FVector(0.3f, 0.28f, 0.32f), 0);                // head
		Part(Body, Cube, FVector(-2, 0, 181), FVector(0.3f, 0.29f, 0.08f), 3);               // hair
		Part(Body, Cube, FVector(13, 0, 168), FVector(0.06f, 0.16f, 0.05f), 3);              // brows/shades
		for (USceneComponent* A : { ArmF.Get(), ArmB.Get() })
		{
			Part(A, Cyl, FVector(0, 0, -26), FVector(0.15f, 0.15f, 0.5f), 1);              // sleeve
			Part(A, Sph, FVector(0, 0, -56), FVector(0.17f, 0.17f, 0.17f), 0);             // fist
		}
		for (USceneComponent* Lg : { LegF.Get(), LegB.Get() })
		{
			Part(Lg, Cube, FVector(0, 0, -42), FVector(0.2f, 0.18f, 0.84f), 2);            // leg
			Part(Lg, Cube, FVector(6, 0, -84), FVector(0.3f, 0.18f, 0.08f), 3);            // boot
		}
	}
	Pose(TEXT("idle"), 0, 0, 0, 0, 0, true);
}

void AFTBrawlFighter::Pose(FName St, float T, float Walk, int32 Combo, float LeanDeg, float Flash, bool bVisible)
{
	if (Sprite) { PoseSprite(St, T, Walk, Combo, LeanDeg, Flash, bVisible); return; }
	if (Mesh) { PoseCast(St, T, Combo, Flash, bVisible); return; }
	SetActorHiddenInGame(!bVisible);
	const FLinearColor Cols[4] = { Look.Skin, Look.Shirt, Look.Pants, Look.Hair };
	for (int32 i = 0; i < Mids.Num(); i++)
		Mids[i]->SetVectorParameterValue(TEXT("Color"), FMath::Lerp(Cols[int32(Base[i].R)], FLinearColor::White, FMath::Clamp(Flash, 0.f, 1.f)));

	// pitch about the feet: + leans back (away from where he faces)
	Body->SetRelativeRotation(FRotator(LeanDeg, 0, 0));
	Body->SetRelativeLocation(FVector(0, 0, FMath::Abs(LeanDeg) > 60 ? 16.f : 0.f));
	float aF = 0, aB = 0, lF = 0, lB = 0;
	const float Sw = FMath::Sin(Walk * PI * 2.f) * 28.f;
	if (St == TEXT("walk") || St == TEXT("charge")) { aF = Sw; aB = -Sw; lF = -Sw; lB = Sw; if (St == TEXT("charge")) { aF = aB = 60; } }
	else if (St == TEXT("punch")) { const float k = FMath::Clamp(T / 0.08f, 0.f, 1.f) * (T < 0.16f ? 1.f : 0.4f); if (Combo == 2) aB = 90 * k; else aF = 90 * k; if (Combo == 3) { aF = 100 * k; aB = 60 * k; } }
	else if (St == TEXT("kick")) { const float k = FMath::Clamp(T / 0.12f, 0.f, 1.f) * (T < 0.3f ? 1.f : 0.5f); lF = 95 * k; aF = -30 * k; aB = 40 * k; }
	else if (St == TEXT("jump")) { lF = 40; lB = -20; aF = 50; aB = 50; }
	else if (St == TEXT("jumpkick")) { lF = 90; lB = -30; aF = -40; aB = 70; }
	else if (St == TEXT("wind") || St == TEXT("aim")) { aF = -50; aB = 30; }
	else if (St == TEXT("swing")) { aF = 95; }
	else if (St == TEXT("hurt") || St == TEXT("fly")) { aF = 120; aB = 140; }
	else if (St == TEXT("down") || St == TEXT("dead")) { aF = 160; aB = 170; }
	ArmF->SetRelativeRotation(FRotator(aF, 0, 0));
	ArmB->SetRelativeRotation(FRotator(aB, 0, 0));
	LegF->SetRelativeRotation(FRotator(lF, 0, 0));
	LegB->SetRelativeRotation(FRotator(lB, 0, 0));
}

// ---------------------------------------------------------------- the Meshy cast
bool AFTBrawlFighter::SetCast(const FString& Who, float HeightCm)
{
	const FString Dir = FString::Printf(TEXT("/Game/Brawl/Characters/%s/"), *Who);
	USkeletalMesh* SK = LoadObject<USkeletalMesh>(nullptr, *(Dir + TEXT("SK_") + Who + TEXT(".SK_") + Who));
	if (!SK) return false;
	static const TCHAR* All[] = { TEXT("idle"), TEXT("walk"), TEXT("run"), TEXT("jab_l"), TEXT("jab_r"), TEXT("uppercut"), TEXT("roundhouse"), TEXT("flykick"),
		TEXT("knee"), TEXT("hook"), TEXT("grab"), TEXT("throw"), TEXT("groundslam"), TEXT("hit_face"), TEXT("knockdown"), TEXT("dead"), TEXT("arise"),
		TEXT("taunt"), TEXT("cheer") };
	const FString Lo = Who.ToLower();
	for (const TCHAR* C : All)
	{
		const FString N = FString::Printf(TEXT("A_%s_%s%s"), *Who, *Lo, C);
		if (UAnimSequence* A = LoadObject<UAnimSequence>(nullptr, *(Dir + N + TEXT(".") + N), nullptr, LOAD_Quiet | LOAD_NoWarn)) Clips.Add(FName(C), A);
	}
	Mesh = NewObject<USkeletalMeshComponent>(this, TEXT("Cast"));
	Mesh->SetupAttachment(Body);
	Mesh->SetSkeletalMesh(SK);
	// imported in metres, facing +Y: scale to the fighter's height, turn to face +X (the actor's forward)
	const float H = FMath::Max(0.1f, SK->GetBounds().BoxExtent.Z * 2.f);
	Mesh->SetRelativeScale3D(FVector(HeightCm / H));
	Mesh->SetRelativeRotation(FRotator(0, -90, 0));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	Mesh->bEnableUpdateRateOptimizations = false;
	Mesh->SetCastShadow(true);
	Mesh->RegisterComponent();
	for (UStaticMeshComponent* P : Parts) if (P) P->DestroyComponent();
	Parts.Reset(); Mids.Reset(); Base.Reset();
	LastKey = NAME_None;
	return true;
}

UAnimSequence* AFTBrawlFighter::Clip(std::initializer_list<const TCHAR*> Names) const
{
	for (const TCHAR* N : Names) if (const TObjectPtr<UAnimSequence>* A = Clips.Find(FName(N))) return *A;
	if (const TObjectPtr<UAnimSequence>* A = Clips.Find(TEXT("idle"))) return *A;
	return nullptr;
}

void AFTBrawlFighter::PoseCast(FName St, float T, int32 Combo, float Flash, bool bVisible)
{
	SetActorHiddenInGame(!bVisible);
	Body->SetRelativeRotation(FRotator::ZeroRotator);
	Body->SetRelativeLocation(FVector::ZeroVector);
	// state -> (key, clip, loop, how long the state lasts: the clip is sped up to fit, Double Dragon snap)
	FName Key = St; UAnimSequence* A = nullptr; bool bLoop = false; float Fit = 0.f; float Rate = 1.f;
	const FString S = St.ToString();
	if (S == TEXT("idle")) { A = Clip({ TEXT("idle") }); bLoop = true; Rate = 1.2f; }
	else if (S == TEXT("walk")) { A = Clip({ TEXT("walk") }); bLoop = true; Rate = 1.6f; }
	else if (S == TEXT("run") || S == TEXT("dash") || S == TEXT("charge")) { Key = TEXT("run"); A = Clip({ TEXT("run") }); bLoop = true; Rate = 1.3f; }
	else if (S == TEXT("punch"))
	{
		Key = FName(*FString::Printf(TEXT("punch%d"), Combo));
		A = Combo == 3 ? Clip({ TEXT("uppercut"), TEXT("hook"), TEXT("jab_r") }) : Combo == 2 ? Clip({ TEXT("jab_r"), TEXT("jab_l") }) : Clip({ TEXT("jab_l"), TEXT("jab_r") });
		Fit = Combo == 3 ? 0.34f : 0.26f;
	}
	else if (S == TEXT("kick")) { A = Clip({ TEXT("roundhouse"), TEXT("knee") }); Fit = 0.45f; }
	else if (S == TEXT("jump") || S == TEXT("jumpkick")) { Key = TEXT("air"); A = Clip({ TEXT("flykick") }); Fit = 0.7f; }
	else if (S == TEXT("knee")) { A = Clip({ TEXT("knee") }); Fit = 0.42f; }
	else if (S == TEXT("wind") || S == TEXT("swing")) { Key = TEXT("atk"); A = Clip({ TEXT("hook"), TEXT("jab_r"), TEXT("jab_l") }); Fit = 0.8f; }
	else if (S == TEXT("grab") || S == TEXT("hold")) { Key = TEXT("grab"); A = Clip({ TEXT("grab"), TEXT("knee") }); bLoop = true; Rate = 1.4f; }
	else if (S == TEXT("throw")) { A = Clip({ TEXT("throw"), TEXT("jab_r") }); Fit = 0.9f; }
	else if (S == TEXT("aim") || S == TEXT("slamwind") || S == TEXT("slam")) { Key = TEXT("slam"); A = Clip({ TEXT("groundslam"), TEXT("hook") }); Fit = 1.3f; if (S == TEXT("aim")) { Key = TEXT("aim"); A = Clip({ TEXT("taunt"), TEXT("idle") }); Fit = 0; Rate = 2.f; } }
	else if (S == TEXT("hurt") || S == TEXT("held")) { Key = TEXT("hurt"); A = Clip({ TEXT("hit_face") }); Fit = 0.7f; }
	else if (S == TEXT("lift") || S == TEXT("carry")) { Key = TEXT("grab"); A = Clip({ TEXT("grab"), TEXT("idle") }); bLoop = true; }   // grab and throw (run 42137)
	else if (S == TEXT("hurl")) { A = Clip({ TEXT("throw"), TEXT("jab_r") }); Fit = 0.34f; }
	else if (S == TEXT("fly") || S == TEXT("down") || S == TEXT("dead") || S == TEXT("lifted") || S == TEXT("hurled")) { Key = TEXT("kd"); A = Clip({ TEXT("knockdown"), TEXT("dead") }); Fit = 1.3f; }
	else if (S == TEXT("getup")) { A = Clip({ TEXT("arise") }); Fit = 0.5f; }
	else if (S == TEXT("cheer")) { A = Clip({ TEXT("cheer"), TEXT("taunt") }); bLoop = true; }
	else { A = Clip({ TEXT("idle") }); bLoop = true; }
	if (!A) return;
	const bool bRestart = Key != LastKey || (St == LastSt && T + 0.001f < LastT && !bLoop);
	if (bRestart)
	{
		Mesh->PlayAnimation(A, bLoop);
		if (Fit > 0) Rate = FMath::Clamp(A->GetPlayLength() / Fit, 0.6f, 9.f);
		Mesh->SetPlayRate(Rate);
		LastKey = Key;
	}
	LastT = T; LastSt = St;
	// the hit flash: an overlay tint isn't in Meshy's material, so a quick shudder sells the impact instead
	Body->SetRelativeLocation(Flash > 0 ? FVector(FMath::Sin(T * 140.f) * 4.f, 0, 0) : FVector::ZeroVector);
}

// ---------------------------------------------------------------- 2.5D: the painted sprite
namespace
{
	// every frame the sheets give (ArtSource/frames): missing ones fall back down the list in Frame()
	const TCHAR* GFrameNames[] = { TEXT("idle"), TEXT("walk1"), TEXT("walk2"), TEXT("walk3"), TEXT("jab"), TEXT("cross"), TEXT("upper"),
		TEXT("kick"), TEXT("jump"), TEXT("jumpkick"), TEXT("hurt"), TEXT("down"), TEXT("getup"), TEXT("run"), TEXT("win"), TEXT("taunt"),
		TEXT("wind"), TEXT("punch"), TEXT("grab"), TEXT("throw"), TEXT("slam"),
		TEXT("drink"), TEXT("pickup"), TEXT("overhead"), TEXT("bash"),     // Nova's held-item poses (run 21219); bash = the Guard's nightstick
		TEXT("spin"), TEXT("spinb"), TEXT("dizzy"),                       // Vinnie's disco whirlwind (run 16961)
		TEXT("point"), TEXT("give"), TEXT("laugh"), TEXT("smoke"), TEXT("away"),   // Harlan Voss, night 1's cutscenes (run 49531)
		TEXT("mega"), TEXT("shout"), TEXT("frame"), TEXT("order"),           // Voss on set with his megaphone (day 1, run 73444)
		TEXT("wave"), TEXT("me"), TEXT("nervous"), TEXT("ask"), TEXT("smirk"),   // Kip Lorenzo (day 1, run 73444)
		TEXT("draw"), TEXT("aim"), TEXT("fire"),                           // the Pit Boss's revolver (run 52616)
		// nights 4-7 (run 32365): Flambe Frankie's fire breath, the Kingpin's bowling, Big Sven's bear hug, the Commodore's
		// anchor; sad = the boss beaten, in the LAST CALL cutscene
		TEXT("swig"), TEXT("breathe"), TEXT("hold"), TEXT("bowl"), TEXT("gloves"), TEXT("lunge"), TEXT("hug"), TEXT("windup"), TEXT("sweep"), TEXT("sad") };
	const float FramePx = 1024.f, FeetPx = 1000.f, StandPx = 600.f;
}

bool AFTBrawlFighter::SetSprite(const FString& Who, float HeightCm, const FLinearColor& Tint)
{
	UMaterialInterface* M = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlSprite.M_BrawlSprite"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UStaticMesh* Plane = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (!M || !Plane) return false;
	const FString Lo = Who.ToLower();
	for (const TCHAR* N : GFrameNames)
	{
		const FString A = FString::Printf(TEXT("/Game/Brawl/Sprites/%s/T_%s_%s.T_%s_%s"), *Who, *Lo, N, *Lo, N);
		if (UTexture* Tx = LoadObject<UTexture>(nullptr, *A, nullptr, LOAD_Quiet | LOAD_NoWarn)) Frames.Add(FName(N), Tx);
	}
	if (!Frames.Contains(TEXT("idle"))) { Frames.Reset(); return false; }
	Sprite = NewObject<UStaticMeshComponent>(this, TEXT("Sprite"));
	Sprite->SetupAttachment(Body);
	Sprite->SetStaticMesh(Plane);
	// the engine plane lies in XY (normal +Z, 100 cm): stand it up facing the camera (+Y), size it so 600 px = HeightCm,
	// and lift it so the feet (px 1000 of 1024) sit on the actor's origin
	const float CmPerPx = HeightCm / StandPx;
	Sprite->SetRelativeRotation(FRotator(0, 0, 90));
	Sprite->SetRelativeScale3D(FVector(FramePx * CmPerPx / 100.f, FramePx * CmPerPx / 100.f, 1));
	Sprite->SetRelativeLocation(FVector(0, 0, (FeetPx - FramePx / 2) * CmPerPx));
	Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sprite->SetCastShadow(true);
	Sprite->bCastShadowAsTwoSided = true;
	SpriteMid = UMaterialInstanceDynamic::Create(M, this);
	SpriteMid->SetVectorParameterValue(TEXT("Tint"), Tint);
	Sprite->SetMaterial(0, SpriteMid);
	Sprite->RegisterComponent();
	for (UStaticMeshComponent* P : Parts) if (P) P->DestroyComponent();
	Parts.Reset(); Mids.Reset(); Base.Reset();
	CurFrame = nullptr;
	return true;
}

void AFTBrawlFighter::SetTint(const FLinearColor& C)
{
	if (SpriteMid) SpriteMid->SetVectorParameterValue(TEXT("Tint"), C);
}

UTexture* AFTBrawlFighter::Frame(std::initializer_list<const TCHAR*> Names) const
{
	for (const TCHAR* N : Names) if (const TObjectPtr<UTexture>* T = Frames.Find(FName(N))) return *T;
	return Frames.FindRef(TEXT("idle"));
}

void AFTBrawlFighter::PoseSprite(FName St, float T, float Walk, int32 Combo, float LeanDeg, float Flash, bool bVisible)
{
	SetActorHiddenInGame(!bVisible);
	const FString S = St.ToString();
	auto Cycle = [&](float Phase) -> UTexture*
	{
		const float F = FMath::Frac(Phase);
		if (Frames.Contains(TEXT("walk3")))
		{
			static const TCHAR* Four[] = { TEXT("walk1"), TEXT("walk2"), TEXT("walk3"), TEXT("walk2") };
			return Frame({ Four[FMath::Clamp(int32(F * 4), 0, 3)] });
		}
		return F < 0.5f ? Frame({ TEXT("walk1") }) : Frame({ TEXT("walk2"), TEXT("idle") });
	};
	UTexture* Tx = nullptr;
	float Lean = 0.f, Squash = 1.f;
	if (S == TEXT("walk")) Tx = Cycle(Walk);
	else if (S == TEXT("run") || S == TEXT("dash") || S == TEXT("charge")) Tx = Frames.Contains(TEXT("run")) ? Frame({ TEXT("run") }) : Frames.Contains(TEXT("kick")) ? Frame({ TEXT("kick") }) : Cycle(T * 3.f);
	else if (S == TEXT("whirltell")) Tx = Frame({ TEXT("taunt"), TEXT("wind") });                           // Vinnie points at the sky: the spotlight finds him
	else if (S == TEXT("whirl")) Tx = int32(T / 0.09f) % 2 ? Frame({ TEXT("spinb"), TEXT("kick") }) : Frame({ TEXT("spin"), TEXT("kick") });
	else if (S == TEXT("draw")) Tx = T < 0.3f ? Frame({ TEXT("draw"), TEXT("wind") }) : Frame({ TEXT("aim"), TEXT("wind") });   // the Pit Boss draws, then holds the aim
	else if (S == TEXT("fire")) Tx = (T < 0.6f && FMath::Fmod(T, 0.22f) < 0.08f) ? Frame({ TEXT("fire"), TEXT("punch") }) : Frame({ TEXT("aim"), TEXT("punch") });   // a flash on each shot
	else if (S == TEXT("swig")) Tx = Frame({ TEXT("swig"), TEXT("wind") });                         // nights 4-7 (run 32365): the bosses' own moves
	else if (S == TEXT("breathe")) Tx = Frame({ TEXT("breathe"), TEXT("punch") });
	else if (S == TEXT("hold")) Tx = Frame({ TEXT("hold"), TEXT("wind") });
	else if (S == TEXT("bowl")) Tx = Frame({ TEXT("bowl"), TEXT("punch") });
	else if (S == TEXT("gloves")) Tx = Frame({ TEXT("gloves"), TEXT("taunt"), TEXT("wind") });
	else if (S == TEXT("lunge")) Tx = Frame({ TEXT("lunge"), TEXT("run"), TEXT("punch") });
	else if (S == TEXT("hug")) Tx = Frame({ TEXT("hug"), TEXT("grab"), TEXT("punch") });
	else if (S == TEXT("windup")) Tx = Frame({ TEXT("windup"), TEXT("wind") });
	else if (S == TEXT("sweep")) Tx = Frame({ TEXT("sweep"), TEXT("bash"), TEXT("punch") });
	else if (S == TEXT("dizzy")) { Tx = Frame({ TEXT("dizzy"), TEXT("hurt") }); Lean = FMath::Sin(T * 9.f) * 7.f; }
	else if (S == TEXT("punch")) Tx = Combo == 3 ? Frame({ TEXT("upper"), TEXT("punch") }) : Combo == 2 ? Frame({ TEXT("cross"), TEXT("punch") }) : Frame({ TEXT("jab"), TEXT("punch") });
	else if (S == TEXT("kick")) Tx = Frame({ TEXT("kick"), TEXT("punch") });
	else if (S == TEXT("jump")) Tx = Frame({ TEXT("jump"), TEXT("idle") });
	else if (S == TEXT("jumpkick")) Tx = Frame({ TEXT("jumpkick"), TEXT("kick"), TEXT("punch") });
	else if (S == TEXT("wind") || S == TEXT("aim")) Tx = Frame({ TEXT("wind"), TEXT("idle") });
	else if (S == TEXT("slamwind")) Tx = Frame({ TEXT("jump"), TEXT("wind"), TEXT("idle") });   // Earl's leap
	else if (S == TEXT("slam")) Tx = Frame({ TEXT("slam"), TEXT("punch"), TEXT("cross") });
	else if (S == TEXT("throw")) Tx = Frame({ TEXT("throw"), TEXT("punch"), TEXT("cross") });
	else if (S == TEXT("grab") || S == TEXT("knee")) Tx = Frame({ TEXT("grab"), TEXT("punch"), TEXT("cross") });
	else if (S == TEXT("swing")) Tx = Combo == 1 ? Frame({ TEXT("bash"), TEXT("kick"), TEXT("punch"), TEXT("cross") }) : Frame({ TEXT("punch"), TEXT("cross") });   // every other swing: the nightstick / Vinnie's kick
	else if (S == TEXT("drink")) Tx = Frame({ TEXT("drink"), TEXT("taunt"), TEXT("idle") });
	else if (S == TEXT("pickup")) Tx = Frame({ TEXT("pickup"), TEXT("getup"), TEXT("idle") });
	else if (S == TEXT("wpn") || S == TEXT("toss")) Tx = T < 0.12f ? Frame({ TEXT("overhead"), TEXT("jump") }) : Frame({ TEXT("bash"), TEXT("cross") });
	else if (S == TEXT("hurt") || S == TEXT("held")) Tx = Frame({ TEXT("hurt") });
	// grab and throw (run 42137): Nova lifts him (pickup), holds him overhead, hurls him (bash); the foe lies flat up there and in flight
	else if (S == TEXT("lift")) Tx = Frame({ TEXT("pickup"), TEXT("getup"), TEXT("idle") });
	else if (S == TEXT("carry")) Tx = Frame({ TEXT("overhead"), TEXT("jump"), TEXT("idle") });
	else if (S == TEXT("hurl")) Tx = T < 0.1f ? Frame({ TEXT("overhead"), TEXT("jump") }) : Frame({ TEXT("bash"), TEXT("cross"), TEXT("punch") });
	else if (S == TEXT("lifted")) Tx = Frame({ TEXT("down"), TEXT("hurt") });
	else if (S == TEXT("hurled")) { Tx = Frame({ TEXT("down"), TEXT("hurt") }); Lean = FMath::Sin(T * 18.f) * 10.f; }
	else if (S == TEXT("fly")) { Tx = Frame({ TEXT("hurt") }); Lean = FMath::Min(70.f, LeanDeg); }
	else if (S == TEXT("down") || S == TEXT("dead")) Tx = Frame({ TEXT("down") });
	else if (S == TEXT("getup")) Tx = Frame({ TEXT("getup"), TEXT("down") });
	else if (S == TEXT("cheer")) Tx = Frame({ TEXT("win"), TEXT("taunt"), TEXT("idle") });
	else if (const TObjectPtr<UTexture>* Named = Frames.Find(St)) Tx = *Named;   // cutscenes: any frame by its name (taunt, give, smoke...)
	else { Tx = Frame({ TEXT("idle") }); Squash = 1.f + FMath::Sin(T * 5.f) * 0.012f; }   // idle: a breath
	if (Tx && Tx != CurFrame) { SpriteMid->SetTextureParameterValue(TEXT("Tex"), Tx); CurFrame = Tx; }
	SpriteMid->SetScalarParameterValue(TEXT("Flash"), FMath::Clamp(Flash, 0.f, 1.f) * 1.6f);
	// knocked flying: the hurt frame tips back about the feet; a hit shudders the body
	Body->SetRelativeRotation(FRotator(Lean, 0, 0));
	Body->SetRelativeScale3D(FVector(1, 1, Squash));
	Body->SetRelativeLocation(Flash > 0 ? FVector(FMath::Sin(T * 140.f) * 4.f, 0, 0) : FVector::ZeroVector);
}
