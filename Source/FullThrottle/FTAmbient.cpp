#include "FTLife.h"
#include "FTTrack.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "HAL/IConsoleManager.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"

// 3.1, the living world: the air round the camera (dust, snow, embers, bubbles...), one look per level.
//
// The air is one static mesh of 600 tiny cards (/Game/FX/Ambient/SM_AirField) and a material instance per level
// (MI_Air_L0..7), both made by Content/Python/ft_life.py. The material gives every card a place in the WORLD (its own
// spot, its drift, its wander) and shows the box of them that lies round the view, wrapping the rest in: the car drives
// through the air, it does not carry it along. That is why this is not a Niagara emitter: the game's emitters are the
// engine's lightweight (stateless) kind, which keep no particle history: a particle's place is worked out each frame
// from its age, in the emitter's own space (ft_fx.py's templates offer no world-space switch; 15 Nov probe), so on
// anything that follows the camera the particles would follow it too. A mesh also costs the same every frame (one
// draw, 2400 vertices, no simulation), which is what Low quality wants; Low shows 45% of the cards.
// This actor only keeps the mesh at the camera (so it is never culled) and tells the material how dense the air is.
// No asset, no air: a level without an instance simply has none.

AFTAmbient::AFTAmbient()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork; // after the camera has moved
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Field = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Field"));
	Field->SetupAttachment(RootComponent);
	Field->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Field->SetGenerateOverlapEvents(false);
	Field->SetCastShadow(false);
	Field->bAffectDistanceFieldLighting = false;
	Field->bAffectDynamicIndirectLighting = false;
	Field->SetCanEverAffectNavigation(false);
	Field->SetVisibility(false);
	Air = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Air"));
	Air->SetupAttachment(RootComponent);
	Air->SetAutoActivate(false);
	Air->SetCastShadow(false);
}

void AFTAmbient::Setup(AFTTrack* InTrack, int32 InLevel)
{
	Track = InTrack;
	Level = InLevel;
	UStaticMesh* Mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/FX/Ambient/SM_AirField.SM_AirField"), nullptr, LOAD_NoWarn | LOAD_Quiet));
	const FString MatPath = FString::Printf(TEXT("/Game/FX/Ambient/MI_Air_L%d.MI_Air_L%d"), Level, Level);
	UMaterialInterface* Mat = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *MatPath, nullptr, LOAD_NoWarn | LOAD_Quiet));
	if (Mesh && Mat)
	{
		Field->SetStaticMesh(Mesh);
		FieldMat = UMaterialInstanceDynamic::Create(Mat, this);
		Field->SetMaterial(0, FieldMat);
		FullDensity = FieldMat->K2_GetScalarParameterValue(TEXT("Density"));
		Field->SetVisibility(true);
		ApplyQuality();
	}
	// No level has an emitter of its own (Air stays empty, emitter=0 below): the load of /Game/FX/Ambient/NS_Ambient_L<level>
	// that stood here asked the packaged game for a package that has never existed, once a level.
	UE_LOG(LogFT, Display, TEXT("FTLIFE air level=%d field=%d density=%.2f emitter=%d"), Level, FieldMat ? 1 : 0, FullDensity, Air->GetAsset() ? 1 : 0);
}

void AFTAmbient::ApplyQuality()
{
	if (!FieldMat) return;
	int32 Q = 3;
	if (const IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("sg.EffectsQuality"))) Q = CVar->GetInt();
	if (Q == QualitySeen) return;
	QualitySeen = Q;
	FieldMat->SetScalarParameterValue(TEXT("Density"), FullDensity * (Q <= 0 ? 0.45f : 1.f));
}

void AFTAmbient::Tick(float Dt)
{
	Super::Tick(Dt);
	if (const APlayerCameraManager* Cam = UGameplayStatics::GetPlayerCameraManager(this, 0))
	{
		SetActorLocation(Cam->GetCameraLocation());
	}
	QualityClock += Dt;
	if (QualityClock > 1.f) { QualityClock = 0.f; ApplyQuality(); }
}
