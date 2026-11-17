// THE STAGES: breakable scenery, onlookers, depth layers and lighting moods (stage 2, "stages").
//
// Everything here is built on top of BuildVenue's room and never changes the fight lane (floor band 360..560 px).
//
// 1. BREAKABLES. Two sources:
//      - "adopted": the painted stools and tables BuildVenue stands against the back wall. BuildVenue asks StageAdopt
//        for each; when this night's damaged and wrecked drawings are imported the stage owns the prop (same place,
//        same size, same drawing while it is whole), otherwise BuildVenue places it exactly as before.
//      - "own": crates, crates of bottles, floor signs and floor lamps (and two tables on a night that has none at the
//        wall), stood just behind the back edge of the lane (depth 356 px, 4 px behind the lane's back line).
//    Each has health and three drawings: whole, damaged (at 55 % health), wrecked (a heap). A hit shakes it, flashes it
//    and throws chips; the wreck throws a burst of pieces that bounce, settle on the floor and go after 4.5 to 7.5 s,
//    and the heap itself blinks out 9 s later. What can hit one:
//        Nova's attacks (every old and new attack, through the same reach numbers as against a foe; in depth he must
//          stand in the back third of the lane: within 68 px of wall furniture's line (339), 62 px of the others' (356)),
//        a thrown weapon passing it, a thrower's bottle landing by it,
//        a foe knocked flying or hurled past it in the back 40 % of the lane (within 86 / 80 px of its line; the foe
//          takes 5 more and loses speed), a boss's rush through it.
//    A wreck sometimes drops a pickup (table below) and its pieces stagger foes standing right behind it (6 damage,
//    within 110 px on the far side of the hit, back third of the lane only).
//
//      kind      health  drops (chance)                               chips
//      crate     26      45 %: chili dog / bottle / knuckles / jacket wood
//      bottles   14      70 %: a bottle (the night's own)             glass
//      sign      12      -                                            wood
//      lamp      16      -   (its light flickers when damaged, goes out when wrecked; the room's lights dip)  brass
//      stools    14      35 %: a bar stool to swing or throw (2 uses) wood / brass
//      tables    22      40 %: a bottle, else 15 %: a chili dog       wood / brass
//
// 2. ONLOOKERS. Six background people (new drawings: watching, cheering, flinching, ducking) stand in ones and twos in
//    the FOREGROUND band, 470 to 505 cm in front of the lane's front line (the lane is 280 cm deep), dimmed, never
//    part of the fight and never targeted. Their heads stay below the lane's front line on screen. They face Nova,
//    cheer a knock-out, a cleared wave, every 8th hit of a chain and the win; flinch when Nova is floored, when
//    scenery is wrecked near them, when a bottle or a body flies; duck for a boss's signature move and for gunfire,
//    and duck and step back 38 cm whenever a fighter comes within 50 px of the lane's front line near them.
//    Hidden in cutscenes (their cameras come close and low) and on the development pose stage; the stage's own
//    breakables are hidden on the pose stage only.
//
// 3. DEPTH. The room already had the wall, the wall props, the lane and a far foreground (posts, tables, 460..600 cm).
//    Added: the onlookers' band (470..505 cm) and a NEAR layer 900 cm toward the camera: black silhouettes of the
//    night's own furniture whose tops stay under the lane's front line; being 2.6 times nearer the camera than the
//    lane they slide past 2.6 times faster. Nothing is added between the camera and a fighter's body.
//
// 4. LIGHTING MOODS. Every light BuildVenue made (not the bosses' own effect lights) is on one dimmer, and two mood
//    lights ride with the camera plus one at Nova:
//      calm      walking between waves: the room as built
//      fight     a wave is locked in: room x0.90, a wash in the venue's neon colour
//      danger    Nova under 30 % health: room x0.74 and a slow red pulse on him
//      entrance  the boss walks in: the room drops to x0.42 in his colour
//      boss      room x0.82, his colour;   rage (his second phase): x0.68, stronger, pulsing
//      win       x1.12, gold;   ko: x0.5, cold blue
//    Each mood also grades the picture through the room's own post-process volume (gain, saturation, vignette;
//    table MoodGrade): the cel material is not touched. With reduced motion the pulses are steady and nothing flickers.
//
// Development (not in a shipping build):
//   ftb.Stage list                    logs every breakable: index, kind, x, state, health
//   ftb.Stage goto <i>                stands Nova in reach of breakable i, facing right at it
//   ftb.Stage hit <i|all> [dmg]       hits it (default 10) as Nova's punch would
//   ftb.Stage react <cheer|flinch|duck>   every onlooker does it
//   ftb.Stage mood <calm|fight|danger|entrance|boss|rage|win|ko|auto>
//   -brawlnostage on the command line: none of this is built (the room as it was).
// Log lines: "FTBRAWL stage built ...", "FTBRAWL stage hit i= kind= by= hp= state=", "FTBRAWL stage wreck i= kind= by= drop=",
// "FTBRAWL stage shrapnel ...", "FTBRAWL stage crash ...", "FTBRAWL stage mood=", "FTBRAWL stage react pose= n=".
#include "FTBrawlGameMode.h"
#include "FTBrawlFighter.h"
#include "Camera/CameraActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/LightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Light.h"
#include "Engine/PointLight.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/SpotLight.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/Texture.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "HAL/IConsoleManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"

namespace FTBrawlStage
{
#include "FTBrawlStageArt.inl"   // the opaque box of every stage drawing (written by the cutter's wrapper, cut_all.py)

	const float SFT = 360.f, SFB = 560.f, SW = 960.f, SPX = 1.4f, SStep = 1.f / 60.f;
	const float WallY = -((SFB - SFT) * SPX + 60.f);   // the back wall's face (world cm)
	const float OwnY = -(SFB - (SFT - 4.f)) * SPX;     // where the stage's own breakables stand
	const float HeapLife = 9.f, HeapBlink = 1.3f;
	const float StrikeDeep = 28.f, BodyDeep = 46.f;   // how much further into the room than its own depth a swing / a flying body still reaches it
	enum { MAT_WOOD, MAT_GLASS, MAT_BRASS };
	enum { MOOD_CALM, MOOD_FIGHT, MOOD_DANGER, MOOD_ENTRANCE, MOOD_BOSS, MOOD_RAGE, MOOD_WIN, MOOD_KO, MOOD_NUM };
	const TCHAR* MoodNames[MOOD_NUM] = { TEXT("calm"), TEXT("fight"), TEXT("danger"), TEXT("entrance"), TEXT("boss"), TEXT("rage"), TEXT("win"), TEXT("ko") };
	const float MoodDim[MOOD_NUM] = { 1.f, 0.90f, 0.74f, 0.42f, 0.82f, 0.68f, 1.12f, 0.5f };
	const float MoodAmt[MOOD_NUM] = { 0.f, 0.45f, 0.45f, 1.f, 0.8f, 1.25f, 0.6f, 0.4f };
	// the grade on the room's post-process volume: gain (r g b), how much of the mood's colour goes into the gain, saturation, vignette added
	struct FGrade { float R, G, B, Colour, Sat, Vig; };
	const FGrade MoodGrade[MOOD_NUM] = {
		{ 1.f, 1.f, 1.f, 0.f, 1.f, 0.f },             // calm
		{ 0.98f, 0.97f, 1.01f, 0.06f, 1.06f, 0.05f }, // fight
		{ 1.02f, 0.86f, 0.86f, 0.f, 0.95f, 0.22f },   // danger
		{ 0.58f, 0.56f, 0.62f, 0.22f, 0.9f, 0.3f },   // entrance
		{ 0.92f, 0.9f, 0.94f, 0.12f, 1.08f, 0.1f },   // boss
		{ 0.86f, 0.8f, 0.82f, 0.22f, 1.16f, 0.2f },   // rage
		{ 1.1f, 1.05f, 0.94f, 0.08f, 1.1f, -0.1f },   // win
		{ 0.6f, 0.66f, 0.82f, 0.f, 0.5f, 0.32f } };   // ko
	const TCHAR* PoseNames[4] = { TEXT("watch"), TEXT("cheer"), TEXT("flinch"), TEXT("duck") };

	// Whole: the drawing while it is whole (bWholeNew 0 = the existing painted prop T_prop_<Whole>)
	struct FKind { const TCHAR* Key; const TCHAR* Whole; int32 bWholeNew; const TCHAR* Hurt; const TCHAR* Wreck; float Cm, Hp; int32 Mat; float Depth; bool bLamp; };
	const FKind GKinds[] = {
		{ TEXT("crate"), TEXT("crate_0"), 1, TEXT("crate_1"), TEXT("crate_2"), 72.f, 26.f, MAT_WOOD, 34.f, false },
		{ TEXT("bottles"), TEXT("bottles_0"), 1, TEXT("bottles_1"), TEXT("bottles_2"), 66.f, 14.f, MAT_GLASS, 34.f, false },
		{ TEXT("sign"), TEXT("sign_0"), 1, TEXT("sign_1"), TEXT("sign_2"), 98.f, 12.f, MAT_WOOD, 34.f, false },
		{ TEXT("lamp"), TEXT("lamp_0"), 1, TEXT("lamp_1"), TEXT("lamp_2"), 152.f, 16.f, MAT_BRASS, 34.f, true },
		{ TEXT("stool"), TEXT("stool"), 0, TEXT("stool_1"), TEXT("stool_2"), 98.f, 14.f, MAT_WOOD, 40.f, false },
		{ TEXT("table"), TEXT("table"), 0, TEXT("table_1"), TEXT("table_2"), 125.f, 22.f, MAT_WOOD, 40.f, false },
		{ TEXT("vstool"), TEXT("vstool"), 0, TEXT("vstool_1"), TEXT("vstool_2"), 92.f, 14.f, MAT_BRASS, 40.f, false },
		{ TEXT("vtable"), TEXT("vtable"), 0, TEXT("vtable_1"), TEXT("vtable_2"), 112.f, 22.f, MAT_BRASS, 40.f, false },
		{ TEXT("cstool"), TEXT("cstool"), 0, TEXT("cstool_1"), TEXT("cstool_2"), 100.f, 14.f, MAT_BRASS, 40.f, false },
		{ TEXT("ctable"), TEXT("ctable"), 0, TEXT("ctable_1"), TEXT("ctable_2"), 115.f, 22.f, MAT_WOOD, 40.f, false },
		{ TEXT("tstool"), TEXT("tstool"), 0, TEXT("tstool_1"), TEXT("tstool_2"), 95.f, 14.f, MAT_WOOD, 40.f, false },
		{ TEXT("ttable"), TEXT("ttable"), 0, TEXT("ttable_1"), TEXT("ttable_2"), 90.f, 22.f, MAT_WOOD, 40.f, false },
		{ TEXT("bstool"), TEXT("bstool"), 0, TEXT("bstool_1"), TEXT("bstool_2"), 95.f, 14.f, MAT_BRASS, 40.f, false },
		{ TEXT("btable"), TEXT("btable"), 0, TEXT("btable_1"), TEXT("btable_2"), 115.f, 22.f, MAT_BRASS, 40.f, false },
		{ TEXT("lstool"), TEXT("lstool"), 0, TEXT("lstool_1"), TEXT("lstool_2"), 95.f, 14.f, MAT_WOOD, 40.f, false },
		{ TEXT("ltable"), TEXT("ltable"), 0, TEXT("ltable_1"), TEXT("ltable_2"), 115.f, 22.f, MAT_WOOD, 40.f, false },
		{ TEXT("ystool"), TEXT("ystool"), 0, TEXT("ystool_1"), TEXT("ystool_2"), 100.f, 14.f, MAT_BRASS, 40.f, false },
		{ TEXT("ytable"), TEXT("ytable"), 0, TEXT("ytable_1"), TEXT("ytable_2"), 115.f, 22.f, MAT_BRASS, 40.f, false },
	};
	const int32 NumKinds = UE_ARRAY_COUNT(GKinds);
	const TCHAR* NightStool[7] = { TEXT("stool"), TEXT("vstool"), TEXT("cstool"), TEXT("tstool"), TEXT("bstool"), TEXT("lstool"), TEXT("ystool") };
	const TCHAR* NightTable[7] = { TEXT("table"), TEXT("vtable"), TEXT("ctable"), TEXT("ttable"), TEXT("btable"), TEXT("ltable"), TEXT("ytable") };
	// the bosses' colours (as the venue's drifting glow cards use them, FTBrawlVenueMotion.cpp)
	const FLinearColor BossColour[7] = { FLinearColor(1.f, 0.42f, 0.12f), FLinearColor(1.f, 0.20f, 0.64f), FLinearColor(0.67f, 1.f, 0.44f),
		FLinearColor(1.f, 0.35f, 0.07f), FLinearColor(0.12f, 0.86f, 1.f), FLinearColor(0.45f, 0.72f, 1.f), FLinearColor(0.96f, 0.75f, 0.36f) };
	const float PersonCm[6] = { 164.f, 160.f, 166.f, 158.f, 163.f, 157.f };

	const FStageArt* FindArt(const TCHAR* Name, int32 bNew)
	{
		for (const FStageArt& A : GStageArt) if (A.bNew == bNew && FCString::Stricmp(A.Name, Name) == 0) return &A;
		return nullptr;
	}
	UTexture* ArtTex(const TCHAR* Name, int32 bNew)
	{
		if (!FindArt(Name, bNew)) return nullptr;
		const FString Path = bNew ? FString::Printf(TEXT("/Game/Brawl/Stage/T_stage_%s.T_stage_%s"), Name, Name)
			: FString::Printf(TEXT("/Game/Brawl/Sprites/Props/T_prop_%s.T_prop_%s"), Name, Name);
		return LoadObject<UTexture>(nullptr, *Path, nullptr, LOAD_Quiet | LOAD_NoWarn);
	}
	const TCHAR* StateArt(const FKind& D, int32 State, int32& bNew)
	{
		bNew = State == 0 ? D.bWholeNew : 1;
		return State == 0 ? D.Whole : State == 1 ? D.Hurt : D.Wreck;
	}
	int32 FindKind(const TCHAR* Key)
	{
		for (int32 i = 0; i < NumKinds; i++) if (FCString::Stricmp(GKinds[i].Key, Key) == 0) return i;
		return INDEX_NONE;
	}
	FString PersonArt(int32 Who, int32 Pose) { return FString::Printf(TEXT("by%d%c_%s"), Who / 2 + 1, Who % 2 ? TEXT('b') : TEXT('a'), PoseNames[Pose]); }
	bool StageOff() { static const bool bOff = FParse::Param(FCommandLine::Get(), TEXT("brawlnostage")); return bOff; }
	bool IsBossMove(FName St)
	{
		static const FName Moves[] = { TEXT("slamwind"), TEXT("whirltell"), TEXT("draw"), TEXT("swig"), TEXT("hold"), TEXT("gloves"), TEXT("windup"),
			TEXT("slam"), TEXT("whirl"), TEXT("fire"), TEXT("breathe"), TEXT("bowl"), TEXT("lunge"), TEXT("sweep") };
		for (const FName& M : Moves) if (St == M) return true;
		return false;
	}
	UMaterialInterface* SpriteMat() { return LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlSprite.M_BrawlSprite"), nullptr, LOAD_Quiet | LOAD_NoWarn); }
	UStaticMesh* PlaneMesh() { return LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane")); }
	UMaterialInstanceDynamic* CardMat(AStaticMeshActor* A) { return A ? Cast<UMaterialInstanceDynamic>(A->GetStaticMeshComponent()->GetMaterial(0)) : nullptr; }

#if !UE_BUILD_SHIPPING
	FAutoConsoleCommandWithWorldAndArgs GFTBrawlStage(TEXT("ftb.Stage"), TEXT("Development: ftb.Stage list | goto <i> | hit <i|all> [dmg] | react <cheer|flinch|duck> | mood <name|auto> (FTBrawlStage.cpp)"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World) { if (AFTBrawlGameMode* G = World ? World->GetAuthGameMode<AFTBrawlGameMode>() : nullptr) G->StageCommand(Args); }));
#endif
}
using namespace FTBrawlStage;

// ---------------------------------------------------------------- building
// one upright painted card (the engine plane stood up to face the camera), its feet (px 1000 of the 1024 frame) at At
static AStaticMeshActor* StageCard(UWorld* Wd, UTexture* T, const FVector& At, float K, bool bFlip, bool bShadow, float SelfLit, const FLinearColor& Tint)
{
	UMaterialInterface* M = SpriteMat(); UStaticMesh* Plane = PlaneMesh();
	if (!Wd || !M || !Plane || !T) return nullptr;
	const float Sz = 1024.f * K;
	AStaticMeshActor* A = Wd->SpawnActor<AStaticMeshActor>(At + FVector(0, 0, 488.f * K), FRotator(0, 0, 90));
	if (!A) return nullptr;
	A->SetMobility(EComponentMobility::Movable);
	UStaticMeshComponent* SM = A->GetStaticMeshComponent();
	SM->SetStaticMesh(Plane); SM->SetWorldScale3D(FVector((bFlip ? -Sz : Sz) / 100.f, Sz / 100.f, 1.f));
	SM->SetCastShadow(bShadow); SM->bCastShadowAsTwoSided = true;
	SM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UMaterialInstanceDynamic* D = UMaterialInstanceDynamic::Create(M, A);
	D->SetTextureParameterValue(TEXT("Tex"), T);
	D->SetScalarParameterValue(TEXT("SelfLit"), SelfLit);
	D->SetVectorParameterValue(TEXT("Tint"), Tint);
	SM->SetMaterial(0, D);
	return A;
}

int32 AFTBrawlGameMode::StageAddBreak(int32 Kind, float CmPerPx, float WorldX, float WorldY, bool bFlip, bool bOwn)
{
	if (Kind < 0 || Kind >= NumKinds) return INDEX_NONE;
	const FKind& D = GKinds[Kind];
	UTexture* T0 = ArtTex(D.Whole, D.bWholeNew); UTexture* T1 = ArtTex(D.Hurt, 1); UTexture* T2 = ArtTex(D.Wreck, 1);
	if (!T0 || !T1 || !T2) return INDEX_NONE;
	FStageBreak B;
	B.Kind = Kind; B.Id = StageBreaks.Num(); B.K = CmPerPx; B.bFlip = bFlip; B.bOwn = bOwn;
	B.At = FVector(WorldX, WorldY, 0.f);
	B.X = WorldX / SPX; B.Y = SFB + WorldY / SPX; B.Depth = D.Depth;
	B.Hp = B.Max = D.Hp;
	AStaticMeshActor* A = StageCard(GetWorld(), T0, B.At, B.K, bFlip, true, Night == 1 ? 0.07f : 0.09f, FLinearColor::White);
	if (!A) return INDEX_NONE;
	B.Actor = A;
	for (UTexture* T : { T0, T1, T2 }) StageTex.AddUnique(T);
	if (D.bLamp)
	{
		const FStageArt* Art = FindArt(D.Whole, D.bWholeNew);
		const float Top = Art ? (1000.f - Art->T) * B.K : D.Cm;
		APointLight* L = GetWorld()->SpawnActor<APointLight>(B.At + FVector(0, 30.f, Top * 0.86f), FRotator::ZeroRotator);
		if (L)
		{
			L->PointLightComponent->SetMobility(EComponentMobility::Movable);
			L->PointLightComponent->SetIntensity(2400.f); L->PointLightComponent->SetAttenuationRadius(430.f); L->PointLightComponent->SetCastShadows(false);
			L->PointLightComponent->SetLightColor(FLinearColor(1.f, 0.82f, 0.56f));
		}
		B.Light = L;
	}
	return StageBreaks.Add(B);
}

// (the fixture's or a locked wave's screen edge holds Nova: goto only reaches what is on the locked screen)
bool AFTBrawlGameMode::StageAdopt(const TCHAR* Prop, float CmPerPx, float WorldX, float WorldY, bool bFlip)
{
	if (bDay || StageOff() || WorldY >= 0.f || !Prop) return false;
	const float Xpx = WorldX / SPX;
	if (Xpx < 130.f || Xpx > ArenaEnd() + SW - 70.f) return false;   // only where the fight can reach it
	const int32 Kind = FindKind(Prop);
	if (Kind == INDEX_NONE || GKinds[Kind].bWholeNew) return false;
	return StageAddBreak(Kind, CmPerPx, WorldX, WorldY, bFlip, false) != INDEX_NONE;
}

void AFTBrawlGameMode::StageBuild()
{
	if (bDay || StageOff()) return;
	UWorld* Wd = GetWorld();
	StageRnd.Initialize(7001 + Night); StageLookRnd.Initialize(9001 + Night);
	const int32 Ni = FMath::Clamp(Night, 1, 7) - 1;
	const int32 Adopted = StageBreaks.Num();
	// ---- every light of the room goes on the dimmer (not the bosses' effect lights, not the far sets)
	for (TActorIterator<ALight> It(Wd); It; ++It)
	{
		ALight* L = *It;
		if (!L || L == FireL.Get() || L == Muzzle.Get() || L == BossSpot.Get() || L == BossAccent.Get() || L == FlashL.Get() || L == TailL.Get() || L == SetLamp.Get()) continue;
		if (FMath::Abs(L->GetActorLocation().X) > 20000.f) continue;   // the pier and the film set
		ULightComponent* C = L->GetLightComponent();
		if (!C || !C->IsA<UPointLightComponent>()) continue;   // point and spot lights (a spot is a point light component)
		FStageLight S; S.L = C; S.Base = C->Intensity;
		StageLights.Add(S);
	}
	// ---- the stage's own breakables, just behind the lane's back line; none on top of an adopted one
	auto Free = [this](float Xpx) { for (const FStageBreak& B : StageBreaks) if (FMath::Abs(B.X - Xpx) < 84.f) return false; return true; };
	auto Own = [&](const TCHAR* Key, float Xpx)
	{
		const int32 Kind = FindKind(Key);
		if (Kind == INDEX_NONE) return;
		const FKind& D = GKinds[Kind];
		const FStageArt* Art = FindArt(D.Whole, D.bWholeNew);
		if (!Art) return;
		for (int32 Try = 0; Try < 4 && !Free(Xpx); Try++) Xpx += 96.f;
		if (!Free(Xpx) || Xpx > ArenaEnd() + SW - 70.f) return;
		StageAddBreak(Kind, D.Cm / float(Art->B - Art->T), Xpx * SPX, OwnY, StageRnd.FRand() < 0.5f, true);
	};
	static const TCHAR* Cycle[] = { TEXT("crate"), TEXT("bottles"), TEXT("lamp"), TEXT("sign"), TEXT("crate"), TEXT("bottles"), TEXT("sign"), TEXT("lamp"), TEXT("bottles"), TEXT("crate"), TEXT("sign") };
	int32 Ci = 0;
	for (float X = 400.f; X < ArenaEnd() + SW - 120.f && Ci < 11; X += 300.f, Ci++) Own(Cycle[Ci], X);
	int32 Tables = 0; const int32 TableKind = FindKind(NightTable[Ni]);
	for (const FStageBreak& B : StageBreaks) if (B.Kind == TableKind) Tables++;
	if (Tables < 2) { Own(NightTable[Ni], 760.f); Own(NightTable[Ni], 2260.f); }
	// ---- the pieces: three instanced batches (wood, glass, brass)
	{
		UMaterialInterface* BaseMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
		UStaticMesh* Cube = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
		AActor* Fx = Wd->SpawnActor<AActor>();
		USceneComponent* Root = NewObject<USceneComponent>(Fx, TEXT("StageFxRoot"));
		Fx->SetRootComponent(Root); Root->RegisterComponent();
		static const FLinearColor Col[3] = { FLinearColor(0.30f, 0.15f, 0.06f), FLinearColor(0.30f, 0.62f, 0.52f), FLinearColor(0.72f, 0.52f, 0.16f) };
		static const TCHAR* Names[3] = { TEXT("StageWood"), TEXT("StageGlass"), TEXT("StageBrass") };
		for (int32 i = 0; i < 3; i++)
		{
			UInstancedStaticMeshComponent* I = NewObject<UInstancedStaticMeshComponent>(Fx, Names[i]);
			I->SetStaticMesh(Cube); I->SetupAttachment(Root); I->SetMobility(EComponentMobility::Movable);
			I->SetCollisionEnabled(ECollisionEnabled::NoCollision); I->SetCastShadow(false);
			UMaterialInstanceDynamic* D = UMaterialInstanceDynamic::Create(BaseMat, Fx);
			D->SetVectorParameterValue(TEXT("Color"), Col[i]);
			I->SetMaterial(0, D);
			I->RegisterComponent();
			StageChipIsm.Add(I);
		}
	}
	// ---- the onlookers: ones and twos in the foreground band, clear of the foreground posts
	TArray<int32> People;
	for (int32 W6 = 0; W6 < 6; W6++)
	{
		bool bAll = true;
		for (int32 Ps = 0; Ps < 4; Ps++) { UTexture* T = ArtTex(*PersonArt(W6, Ps), 1); if (T) StageTex.AddUnique(T); else bAll = false; }
		if (bAll) People.Add(W6);
	}
	if (People.Num())
	{
		const float RoomX0 = -400.f * SPX, End = (ArenaEnd() + SW - 40.f) * SPX;
		auto NearPost = [&](float X) { for (float Px = RoomX0 + 2600.f; Px < End + 2000.f; Px += 2900.f) if (FMath::Abs(X - Px) < 150.f) return true; return false; };
		int32 Pi = Night;   // each night starts its cast on a different person
		for (float X = 300.f; X < End; X += 300.f + StageRnd.FRand() * 220.f)
		{
			const int32 N = StageRnd.FRand() < 0.6f ? 2 : 1;
			for (int32 j = 0; j < N; j++)
			{
				const float Xb = X + j * (78.f + StageRnd.FRand() * 30.f);
				if (NearPost(Xb)) continue;
				FStageBy By;
				By.Who = People[Pi++ % People.Num()];
				const FStageArt* Art = FindArt(*PersonArt(By.Who, 0), 1);
				By.K = PersonCm[By.Who] / float(Art->B - Art->T);
				By.X = Xb; By.Y = 470.f + StageRnd.FRand() * 35.f; By.Phase = StageRnd.FRand() * 6.28f;
				AStaticMeshActor* A = StageCard(Wd, ArtTex(*PersonArt(By.Who, 0), 1), FVector(By.X, By.Y, 0), By.K, false, false, 0.22f, FLinearColor(0.5f, 0.48f, 0.6f));
				if (!A) continue;
				By.Actor = A;
				StageBys.Add(By);
			}
		}
	}
	// ---- the near layer: black silhouettes of the night's own furniture, tops under the lane's front line
	{
		const float NearYcm = 900.f, TopZ = 300.f;
		int32 Fi = 0;
		for (float X = 520.f; X < (ArenaEnd() + SW) * SPX; X += 760.f, Fi++)
		{
			const TCHAR* Name = Fi % 2 ? NightStool[Ni] : NightTable[Ni];
			const FStageArt* Art = FindArt(Name, 0); UTexture* T = ArtTex(Name, 0);
			if (!Art || !T) continue;
			const float Tall = Fi % 2 ? 160.f : 185.f, K = Tall / float(Art->B - Art->T);
			if (AStaticMeshActor* A = StageCard(Wd, T, FVector(X, NearYcm, TopZ - Tall), K, Fi % 4 >= 2, false, 0.55f, FLinearColor(0.085f, 0.075f, 0.14f)))
				StageFg.Add(A);
		}
	}
	// ---- the room's own post-process volume takes the mood's grade
	for (TActorIterator<APostProcessVolume> It(Wd); It; ++It)
		if (It->bUnbound) { StageGrade = *It; StageVig0 = It->Settings.VignetteIntensity; break; }
	// ---- the mood lights: two ride with the camera, one stays on Nova
	for (int32 i = 0; i < 3; i++)
	{
		APointLight* L = Wd->SpawnActor<APointLight>(FVector(0, 140.f, 300.f), FRotator::ZeroRotator);
		if (!L) continue;   // (the mood lights are used only when all three exist)
		L->PointLightComponent->SetMobility(EComponentMobility::Movable);
		L->PointLightComponent->SetIntensity(0.f); L->PointLightComponent->SetAttenuationRadius(i == 2 ? 520.f : 1000.f); L->PointLightComponent->SetCastShadows(false);
		L->PointLightComponent->SetVisibility(false);
		StageMoodL.Add(L);
	}
	int32 Own0 = 0; for (const FStageBreak& B : StageBreaks) if (B.bOwn) Own0++;
	UE_LOG(LogTemp, Log, TEXT("FTBRAWL stage built night=%d breakables=%d adopted=%d own=%d onlookers=%d people=%d near=%d lights=%d"),
		Night, StageBreaks.Num(), Adopted, Own0, StageBys.Num(), People.Num(), StageFg.Num(), StageLights.Num());
}

void AFTBrawlGameMode::StageReset(bool bKeep)
{
	for (int32 i = 0; i < StageBreaks.Num(); i++)
	{
		FStageBreak& B = StageBreaks[i];
		B.Hp = B.Max; B.State = 0; B.Flash = B.Wob = B.T = 0.f; B.LastBody = -1; B.LastBodyT = -9.f;
		StageShow(i);
	}
	StageChips.Reset();
	for (UInstancedStaticMeshComponent* I : StageChipIsm) if (I) I->ClearInstances();
	for (FStageBy& By : StageBys) { By.Next = 0; By.Delay = 0.01f; By.Hold = 0; By.Back = 0; }
	StageSwung.Reset(); StageSwingSt = NAME_None;
	StageKos = Kos; StageWave = Wave; StageShots = 0; StageBottles = 0; StageChain = 0; StagePSt = P.St; bStageWon = false;
	StageFlicker = 0.f; StageWrecked = 0;
}

// the card shows the drawing of its state, standing straight
void AFTBrawlGameMode::StageShow(int32 I)
{
	if (!StageBreaks.IsValidIndex(I)) return;
	FStageBreak& B = StageBreaks[I];
	AStaticMeshActor* A = B.Actor.Get();
	if (!A) return;
	const FKind& D = GKinds[B.Kind];
	const bool bHide = B.State >= 3 || (B.bOwn && PoseActive());   // the pose stage shows the room as the 'before' films have it
	A->SetActorHiddenInGame(bHide);
	if (B.State < 3)
	{
		int32 bNew = 0; const TCHAR* Name = StateArt(D, B.State, bNew);
		if (UMaterialInstanceDynamic* M = CardMat(A)) { if (UTexture* T = ArtTex(Name, bNew)) M->SetTextureParameterValue(TEXT("Tex"), T); M->SetScalarParameterValue(TEXT("Flash"), 0.f); }
		A->SetActorLocationAndRotation(B.At + FVector(0, 0, 488.f * B.K), FRotator(0, 0, 90));
		A->GetStaticMeshComponent()->SetCastShadow(B.State < 2);
	}
	if (APointLight* L = B.Light.Get()) { L->PointLightComponent->SetVisibility(B.State < 2 && !bHide); L->PointLightComponent->SetIntensity(2400.f); }
}

// ---------------------------------------------------------------- hits
void AFTBrawlGameMode::StageBurst(const FVector& At, float Dir, int32 N, int32 Mat, float Force)
{
	for (int32 i = 0; i < N; i++)
	{
		FStageChip C;
		C.Mat = uint8(Mat);
		C.P = At + FVector((StageLookRnd.FRand() - 0.5f) * 30.f, (StageLookRnd.FRand() - 0.5f) * 10.f, (StageLookRnd.FRand() - 0.5f) * 36.f);
		C.V = FVector(Dir * (60.f + StageLookRnd.FRand() * 380.f) * Force, 30.f + StageLookRnd.FRand() * 170.f, 120.f + StageLookRnd.FRand() * 430.f * Force);
		C.R = FRotator(StageLookRnd.FRand() * 360.f, StageLookRnd.FRand() * 360.f, StageLookRnd.FRand() * 360.f);
		C.Spin = FRotator((StageLookRnd.FRand() - 0.5f) * 900.f, (StageLookRnd.FRand() - 0.5f) * 900.f, (StageLookRnd.FRand() - 0.5f) * 900.f);
		if (Mat == MAT_WOOD) C.S = FVector(0.10f + StageLookRnd.FRand() * 0.14f, 0.025f + StageLookRnd.FRand() * 0.03f, 0.015f + StageLookRnd.FRand() * 0.02f);
		else if (Mat == MAT_GLASS) C.S = FVector(0.03f + StageLookRnd.FRand() * 0.05f, 0.02f + StageLookRnd.FRand() * 0.03f, 0.01f);
		else C.S = FVector(0.05f + StageLookRnd.FRand() * 0.08f, 0.025f + StageLookRnd.FRand() * 0.02f, 0.02f);
		C.Life = 4.5f + StageLookRnd.FRand() * 3.f;
		StageChips.Add(C);
	}
	if (StageChips.Num() > 260) StageChips.RemoveAt(0, StageChips.Num() - 260);
}

void AFTBrawlGameMode::StageHit(int32 I, float Dmg, float Dir, bool bHeavy, const TCHAR* By)
{
	if (!StageBreaks.IsValidIndex(I)) return;
	FStageBreak& B = StageBreaks[I];
	if (B.State >= 2 || Dmg <= 0.f) return;
	const FKind& D = GKinds[B.Kind];
	const int32 Old = B.State;
	Dir = Dir < 0.f ? -1.f : 1.f;
	B.Hp -= Dmg * (bHeavy ? 1.25f : 1.f); B.Flash = 0.12f; B.Wob = 1.f; B.WobDir = Dir; B.T = 0.f;
	B.State = B.Hp <= 0.f ? 2 : B.Hp <= B.Max * 0.55f ? 1 : 0;
	int32 bNew = 0; const FStageArt* Art = FindArt(StateArt(D, Old, bNew), Old == 0 ? D.bWholeNew : 1);
	const float Tall = Art ? (1000.f - Art->T) * B.K : D.Cm;
	const FVector Mid = B.At + FVector(0, 14.f, Tall * 0.6f);
	const TCHAR* Snd = D.Mat == MAT_GLASS ? TEXT("glass") : D.Mat == MAT_BRASS ? TEXT("metal") : TEXT("wood");
	if (B.State < 2)
	{
		Sfx(Snd, 0.6f, D.Mat == MAT_BRASS ? 1.2f : 1.05f);
		StageBurst(Mid, Dir, B.State != Old ? 8 : 3, D.Mat, 0.7f);
		if (D.Mat != MAT_GLASS && B.Kind == FindKind(TEXT("bottles"))) StageBurst(Mid, Dir, 3, MAT_GLASS, 0.6f);
		Shake = FMath::Min(1.f, Shake + 0.08f);
		if (B.State != Old) StageShow(I);
		UE_LOG(LogTemp, Log, TEXT("FTBRAWL stage hit i=%d kind=%s by=%s hp=%.0f/%.0f state=%d"), I, D.Key, By, B.Hp, B.Max, B.State);
		return;
	}
	// ---- wrecked: the heap, a burst of pieces, sometimes a pickup, and the pieces catch a foe standing right behind it
	B.T = 0.f;
	StageShow(I);
	B.Wob = 0.6f;
	Sfx(Snd, 0.95f, D.Mat == MAT_BRASS ? 0.9f : 0.85f); Sfx(TEXT("break"), 0.7f, D.Mat == MAT_GLASS ? 1.25f : 1.f);
	StageBurst(Mid, Dir, 16, D.Mat, 1.15f);
	if (B.Kind == FindKind(TEXT("bottles")) || B.Kind == FindKind(TEXT("lamp")) || D.Mat == MAT_BRASS) StageBurst(Mid, Dir, 8, MAT_GLASS, 1.f);
	if (FCString::Stristr(D.Key, TEXT("table"))) StageBurst(Mid + FVector(0, 0, Tall * 0.3f), Dir, 5, MAT_GLASS, 0.9f);
	Accent(D.Mat == MAT_WOOD ? EAccent::Wood : EAccent::Glass, Mid, 118.f, Dir, false);
	Accent(EAccent::Dust, B.At + FVector(0, 16.f, 14.f), 70.f, Dir, false);
	Shake = FMath::Min(1.f, Shake + 0.26f);
	ContactPause = FMath::Max(ContactPause, 2.f * SStep);
	Rumble(0.35f, 0.12f, TEXT("scenery"));
	StageWrecked++;
	StageReact(2, B.X, 330.f, 0.7f);
	if (D.bLamp) StageFlicker = 0.5f;
	// the drop
	int32 Drop = -1;
	const float R = StageRnd.FRand(), R2 = StageRnd.FRand();
	if (B.Kind == FindKind(TEXT("crate"))) { if (R < 0.45f) Drop = R2 < 0.5f ? 1 : R2 < 0.7f ? 0 : R2 < 0.85f ? 2 : 3; }
	else if (B.Kind == FindKind(TEXT("bottles"))) { if (R < 0.7f) Drop = 0; }
	else if (FCString::Stristr(D.Key, TEXT("stool"))) { if (R < 0.35f) Drop = 4; }
	else if (FCString::Stristr(D.Key, TEXT("table"))) { if (R < 0.4f) Drop = 0; else if (R < 0.55f) Drop = 1; }
	if (Drop >= 0) StageDropItem(Drop, B.X + Dir * 26.f, SFT + 14.f);
	// the pieces fly on
	int32 Caught = 0;
	const float SaveDir = P.Dir; P.Dir = Dir;
	for (FFTBrawlBody& F : Foes)
	{
		if (!Standing(F)) continue;
		const float Dx = (F.X - B.X) * Dir;
		if (Dx > 6.f && Dx < 110.f && F.Y < SFT + 70.f) { HitFoe(F, 6.f, false, true); Caught++; UE_LOG(LogTemp, Log, TEXT("FTBRAWL stage shrapnel i=%d kind=%s on %s hp=%.0f"), I, D.Key, *F.K.Role.ToString(), F.Hp); }
	}
	P.Dir = SaveDir;
	UE_LOG(LogTemp, Log, TEXT("FTBRAWL stage wreck i=%d kind=%s by=%s drop=%d caught=%d wrecked=%d/%d"), I, D.Key, By, Drop, Caught, StageWrecked, StageBreaks.Num());
}

// one of Nova's attacks is live this step: the same box as against a foe
void AFTBrawlGameMode::StageStrike(float Dmg, float Front, float Back, float Depth, bool bDown)
{
	if (StageBreaks.IsEmpty() || Dmg <= 0.f || P.Z > 90.f) return;
	if (P.St != StageSwingSt || P.T < StageSwingT) StageSwung.Reset();   // a new swing
	StageSwingSt = P.St; StageSwingT = P.T;
	for (int32 i = 0; i < StageBreaks.Num(); i++)
	{
		const FStageBreak& B = StageBreaks[i];
		if (B.State >= 2 || StageSwung.Contains(i)) continue;
		const float Dx = (B.X - P.X) * P.Dir;
		if (Dx > -Back && Dx < Front && FMath::Abs(B.Y - P.Y) < B.Depth + StrikeDeep)
		{
			StageSwung.Add(i);
			StageHit(i, Dmg, P.Dir, bDown, TEXT("nova"));
		}
	}
}

bool AFTBrawlGameMode::StageThrown(float X, float Y, float Z, float Dir, float Dmg)
{
	if (Z > 150.f) return false;
	for (int32 i = 0; i < StageBreaks.Num(); i++)
	{
		const FStageBreak& B = StageBreaks[i];
		if (B.State >= 2) continue;
		if (FMath::Abs(B.X - X) < 34.f && FMath::Abs(B.Y - Y) < B.Depth) { StageHit(i, Dmg, Dir, true, TEXT("thrown")); return true; }
	}
	return false;
}

// ---------------------------------------------------------------- the fight step
void AFTBrawlGameMode::StageReact(int32 Pose, float X, float Range, float Hold)
{
	int32 N = 0;
	for (FStageBy& By : StageBys)
	{
		if (Range > 0.f && FMath::Abs(By.X / SPX - X) > Range) continue;
		const int32 Now = By.Delay > 0.f ? By.Next : By.Pose;
		if (Now == Pose) { By.Hold = FMath::Max(By.Hold, Hold); continue; }
		if (Now > Pose && Now != 0) continue;   // ducking beats flinching beats cheering
		By.Next = Pose; By.Delay = 0.02f + StageLookRnd.FRand() * (Pose == 3 ? 0.08f : 0.26f); By.Hold = Hold * (0.8f + StageLookRnd.FRand() * 0.5f);
		N++;
	}
	if (N) UE_LOG(LogTemp, Verbose, TEXT("FTBRAWL stage react pose=%s n=%d x=%.0f"), PoseNames[Pose], N, X);
}

void AFTBrawlGameMode::StageStep(float Dt)
{
	if (StageBreaks.IsEmpty() && StageBys.IsEmpty()) return;
	// ---- bodies against the scenery: a foe knocked flying or hurled, a boss's rush
	for (FFTBrawlBody& F : Foes)
	{
		const bool bFly = F.St == TEXT("fly") || F.St == TEXT("hurled");
		const bool bRush = F.St == TEXT("charge");
		if (!bFly && !bRush) continue;
		if (bFly && (F.Z > 130.f || FMath::Abs(F.Vx) < 60.f)) continue;
		for (int32 i = 0; i < StageBreaks.Num(); i++)
		{
			FStageBreak& B = StageBreaks[i];
			if (B.State >= 2 || FMath::Abs(B.X - F.X) > (bRush ? 46.f : 36.f) || FMath::Abs(B.Y - F.Y) > B.Depth + BodyDeep) continue;
			if (B.LastBody == F.Id && Time - B.LastBodyT < 1.f) continue;
			B.LastBody = F.Id; B.LastBodyT = Time;
			const float Dir = bRush ? F.Dir : (F.Vx < 0 ? -1.f : 1.f);
			if (bFly) { F.Hp -= 5.f; F.Vx *= 0.55f; F.Flash = 0.12f; }
			UE_LOG(LogTemp, Log, TEXT("FTBRAWL stage crash i=%d %s %s into it"), i, *F.K.Role.ToString(), *F.St.ToString());
			StageHit(i, bRush ? 40.f : 16.f, Dir, true, bRush ? TEXT("rush") : TEXT("body"));
		}
	}
	// ---- a thrower's bottle landing by the scenery
	for (const FFTBrawlBottle& Bt : Bottles)
	{
		if (Bt.Z > 60.f || Bt.Vz > 0.f) continue;
		for (int32 i = 0; i < StageBreaks.Num(); i++)
		{
			const FStageBreak& B = StageBreaks[i];
			if (B.State < 2 && FMath::Abs(B.X - Bt.X) < 30.f && FMath::Abs(B.Y - Bt.Y) < B.Depth && !(B.LastBody == -2 && Time - B.LastBodyT < 0.5f))
			{
				StageBreaks[i].LastBody = -2; StageBreaks[i].LastBodyT = Time;
				StageHit(i, 6.f, Bt.Vx < 0 ? -1.f : 1.f, false, TEXT("bottle"));
			}
		}
	}
	// ---- what the onlookers saw
	if (StageBys.Num())
	{
		if (Kos != StageKos) { StageKos = Kos; StageReact(1, P.X, 700.f, 1.0f); }
		if (Wave != StageWave) { StageWave = Wave; StageReact(1, 0.f, 0.f, 1.5f); }
		if (Chain < StageChain) StageChain = 0;
		if (Chain >= StageChain + 8) { StageChain = Chain - Chain % 8; StageReact(1, P.X, 600.f, 0.8f); }
		if (P.St != StagePSt) { if (P.St == TEXT("down")) StageReact(2, P.X, 520.f, 0.8f); StagePSt = P.St; }
		if (Shots.Num() > StageShots && Shots.Num()) StageReact(3, Shots.Last().X, 900.f, 0.9f);
		StageShots = Shots.Num();
		if (Bottles.Num() > StageBottles && Bottles.Num()) StageReact(2, Bottles.Last().X, 420.f, 0.6f);
		StageBottles = Bottles.Num();
		for (const FFTBrawlBody& F : Foes)
		{
			if (F.bBoss && IsBossMove(F.St)) StageReact(3, F.X, 620.f, 0.35f);
			else if (F.St == TEXT("hurled")) StageReact(2, F.X, 300.f, 0.4f);
		}
	}
}

// ---------------------------------------------------------------- every frame
int32 AFTBrawlGameMode::StageMood() const
{
	if (StageMoodForce >= 0) return StageMoodForce;
	if (InCut() || PoseActive()) return MOOD_CALM;
	if (bWon) return MOOD_WIN;
	if (P.Hp <= 0.f) return MOOD_KO;
	const int32 Bi = BossIdx();
	if (Bi != INDEX_NONE && Foes[Bi].St != TEXT("dead"))
	{
		if (BossIntro > 0.f) return MOOD_ENTRANCE;
		return Foes[Bi].Phase >= 2 ? MOOD_RAGE : MOOD_BOSS;
	}
	if (P.Hp < P.Max * 0.3f) return MOOD_DANGER;
	if (LockX >= 0.f && bSpawned) return MOOD_FIGHT;
	return MOOD_CALM;
}

void AFTBrawlGameMode::StageSync(float Dt)
{
	if (StageBreaks.IsEmpty() && StageBys.IsEmpty() && StageLights.IsEmpty()) return;
	Dt = FMath::Min(Dt, 0.05f);
	StageT += Dt;
	const bool bCalm = ReducedPresentationMotion();
	// ---- shown at all? (no onlookers and no near layer in a cutscene; on the pose stage not the stage's own objects either)
	const bool bShow = !InCut() && !PoseActive();
	if (bShow != bStageShown)
	{
		bStageShown = bShow;
		for (AActor* A : StageFg) if (A) A->SetActorHiddenInGame(!bShow);
		for (FStageBy& By : StageBys) if (AActor* A = By.Actor.Get()) A->SetActorHiddenInGame(!bShow);
		for (int32 i = 0; i < StageBreaks.Num(); i++) if (StageBreaks[i].bOwn) StageShow(i);
	}
	// ---- the breakables: the shake after a hit, the heap going
	const FQuat Upright = FRotator(0, 0, 90).Quaternion();
	for (int32 i = 0; i < StageBreaks.Num(); i++)
	{
		FStageBreak& B = StageBreaks[i];
		AStaticMeshActor* A = B.Actor.Get();
		if (!A || B.State >= 3) continue;
		B.T += Dt;
		if (B.Flash > 0.f) { B.Flash -= Dt; if (UMaterialInstanceDynamic* M = CardMat(A)) M->SetScalarParameterValue(TEXT("Flash"), B.Flash > 0.f ? (bCalm ? 0.16f : 0.6f) : 0.f); }
		if (B.Wob > 0.f)
		{
			B.Wob = FMath::Max(0.f, B.Wob - Dt * 3.4f);
			const float Ang = bCalm ? 0.f : FMath::DegreesToRadians(-B.WobDir * 6.5f * B.Wob * B.Wob * FMath::Cos(B.T * 34.f));
			const FQuat Q(FVector(0, 1, 0), Ang);
			A->SetActorLocationAndRotation(B.At + Q.RotateVector(FVector(0, 0, 488.f * B.K)), Q * Upright);
		}
		if (B.State == 1) if (APointLight* L = B.Light.Get())   // a damaged lamp flickers
			L->PointLightComponent->SetIntensity(2400.f * (bCalm ? 0.5f : (FMath::Sin(StageT * 23.f + i) + FMath::Sin(StageT * 61.f) > 0.3f ? 0.9f : 0.18f)));
		if (B.State == 2 && B.T > HeapLife)
		{
			if (B.T > HeapLife + HeapBlink) { B.State = 3; A->SetActorHiddenInGame(true); }
			else if (!bCalm) A->SetActorHiddenInGame((B.bOwn && PoseActive()) || int32(B.T * 14.f) % 2 == 1);
		}
	}
	// ---- the pieces fly, bounce once or twice, settle and go
	if (StageChips.Num() || (StageChipIsm.Num() && StageChipIsm[0] && StageChipIsm[0]->GetInstanceCount()))
	{
		TArray<FTransform> Tr[3];
		for (int32 i = StageChips.Num() - 1; i >= 0; i--)
		{
			FStageChip& C = StageChips[i];
			C.T += Dt;
			if (!C.bRest)
			{
				C.V.Z -= 1700.f * Dt; C.P += C.V * Dt; C.R += C.Spin * Dt;
				if (C.P.Y < WallY + 10.f) { C.P.Y = WallY + 10.f; C.V.Y = FMath::Abs(C.V.Y) * 0.4f; }
				if (C.P.Z <= 2.f)
				{
					C.P.Z = 2.f;
					if (C.V.Z < -150.f) { C.V.Z = -C.V.Z * 0.32f; C.V.X *= 0.55f; C.V.Y *= 0.55f; C.Spin = C.Spin * 0.5f; }
					else { C.bRest = true; C.P.Z = 1.f + C.S.Z * 50.f; C.R = FRotator(0.f, C.R.Yaw, 0.f); }
				}
			}
			if (C.T > C.Life + 0.5f) { StageChips.RemoveAtSwap(i); continue; }
			const float Fade = C.T > C.Life ? 1.f - (C.T - C.Life) / 0.5f : 1.f;
			if (C.Mat < 3) Tr[C.Mat].Add(FTransform(C.R, C.P, C.S * Fade));
		}
		for (int32 m = 0; m < 3 && m < StageChipIsm.Num(); m++)
			if (UInstancedStaticMeshComponent* I = StageChipIsm[m]) { I->ClearInstances(); if (Tr[m].Num()) I->AddInstances(Tr[m], false); }
	}
	// ---- the onlookers
	if (bShow)
	{
		if (bWon && !bStageWon) { bStageWon = true; StageReact(1, 0.f, 0.f, 2.2f); }
		if (bWon && FMath::Fmod(StageT, 1.6f) < Dt) StageReact(1, 0.f, 0.f, 1.2f);
		for (FStageBy& By : StageBys)
		{
			AStaticMeshActor* A = By.Actor.Get();
			if (!A) continue;
			// a fighter at the front of the lane, close by: get down and back
			bool bNear = P.Hp > 0.f && SFB - P.Y < 50.f && FMath::Abs(P.X * SPX - By.X) < 170.f;
			for (const FFTBrawlBody& F : Foes) if (!bNear && F.St != TEXT("dead") && SFB - F.Y < 50.f && FMath::Abs(F.X * SPX - By.X) < 170.f) bNear = true;
			if (bNear && !InCut())
			{
				const int32 Now = By.Delay > 0.f ? By.Next : By.Pose;
				if (Now != 3) { By.Next = 3; By.Delay = 0.01f + StageLookRnd.FRand() * 0.06f; By.Hold = 0.5f; }
				else By.Hold = FMath::Max(By.Hold, 0.45f);
			}
			if (By.Delay > 0.f)
			{
				By.Delay -= Dt;
				if (By.Delay <= 0.f && By.Pose != By.Next)
				{
					By.Pose = By.Next; By.Pop = 1.f;
					if (UMaterialInstanceDynamic* M = CardMat(A)) if (UTexture* T = ArtTex(*PersonArt(By.Who, By.Pose), 1)) M->SetTextureParameterValue(TEXT("Tex"), T);
				}
			}
			else if (By.Pose != 0)
			{
				By.Hold -= Dt;
				if (By.Hold <= 0.f) { By.Next = 0; By.Delay = 0.01f + StageLookRnd.FRand() * 0.12f; }
			}
			By.Back = FMath::FInterpTo(By.Back, By.Pose >= 2 ? 1.f : 0.f, Dt, 7.f);
			By.Pop = FMath::Max(0.f, By.Pop - Dt * 7.f);
			if (FMath::Abs(P.X * SPX - By.X) > 70.f) By.Dir = P.X * SPX > By.X ? 1.f : -1.f;
			float Vs = 1.f, Hs = 1.f, Up = 0.f;
			if (!bCalm)
			{
				if (By.Pose == 0) Vs += 0.011f * FMath::Sin(StageT * 2.1f + By.Phase);
				if (By.Pose == 1) Up = FMath::Abs(FMath::Sin(StageT * 9.5f + By.Phase)) * 7.f;
				Vs *= 1.f - 0.07f * By.Pop; Hs *= 1.f + 0.05f * By.Pop;
			}
			const float Sz = 1024.f * By.K / 100.f;
			A->SetActorLocation(FVector(By.X - By.Dir * By.Back * 10.f, By.Y + By.Back * 38.f, 488.f * By.K * Vs + Up));
			A->SetActorScale3D(FVector(By.Dir * Sz * Hs, Sz * Vs, 1.f));
		}
	}
	// ---- the lights
	const int32 Mood = StageMood();
	if (Mood != StageMoodNow)
	{
		StageMoodNow = Mood;
		UE_LOG(LogTemp, Log, TEXT("FTBRAWL stage mood=%s t=%.2f dim=%.2f"), MoodNames[Mood], StT, MoodDim[Mood]);
	}
	const int32 Ni = FMath::Clamp(Night, 1, 7) - 1;
	const FLinearColor WantC = Mood == MOOD_WIN ? FLinearColor(1.f, 0.78f, 0.42f) : Mood == MOOD_KO ? FLinearColor(0.3f, 0.45f, 1.f)
		: Mood == MOOD_FIGHT || Mood == MOOD_DANGER ? Neon : BossColour[Ni];
	StageDim = FMath::FInterpTo(StageDim, MoodDim[Mood], Dt, Mood == MOOD_ENTRANCE ? 6.f : 2.2f);
	StageMoodI = FMath::FInterpTo(StageMoodI, MoodAmt[Mood], Dt, 3.f);
	StageMoodC = FMath::CInterpTo(StageMoodC, WantC, Dt, 4.f);
	if (StageFlicker > 0.f) StageFlicker -= Dt;
	const float Eff = StageDim * (StageFlicker > 0.f && !bCalm ? (int32(StageT * 28.f) % 2 ? 0.55f : 0.95f) : 1.f);
	if (FMath::Abs(Eff - StageDimSet) > 0.004f)
	{
		StageDimSet = Eff;
		for (const FStageLight& S : StageLights) if (ULightComponent* C = S.L.Get()) C->SetIntensity(S.Base * Eff);
	}
	if (APostProcessVolume* G = StageGrade.Get())
	{
		const FGrade& W = MoodGrade[Mood];
		const float Mx = FMath::Max3(WantC.R, WantC.G, WantC.B);
		const FVector Hue = Mx > 0.01f ? FVector(WantC.R, WantC.G, WantC.B) / Mx : FVector(1, 1, 1);
		const FVector Want = FVector(W.R, W.G, W.B) * FMath::Lerp(FVector(1, 1, 1), Hue, W.Colour);
		const float Rate = Mood == MOOD_ENTRANCE ? 6.f : 2.4f;
		StageGain = FMath::VInterpTo(StageGain, Want, Dt, Rate);
		StageSat = FMath::FInterpTo(StageSat, W.Sat, Dt, Rate);
		StageVig = FMath::FInterpTo(StageVig, StageVig0 + W.Vig, Dt, Rate);
		const float Fl = StageFlicker > 0.f && !bCalm ? (int32(StageT * 28.f) % 2 ? 0.8f : 1.f) : 1.f;
		FPostProcessSettings& S = G->Settings;
		S.bOverride_ColorGain = true; S.ColorGain = FVector4(StageGain.X * Fl, StageGain.Y * Fl, StageGain.Z * Fl, 1.f);
		S.bOverride_ColorSaturation = true; S.ColorSaturation = FVector4(StageSat, StageSat, StageSat, 1.f);
		S.bOverride_VignetteIntensity = true; S.VignetteIntensity = StageVig;
	}
	if (StageMoodL.Num() == 3 && CamActor)
	{
		const float CamX = CamActor->GetActorLocation().X;
		const float Pulse = Mood == MOOD_RAGE && !bCalm ? 0.78f + 0.22f * FMath::Sin(StageT * 5.f) : 1.f;
		for (int32 i = 0; i < 2; i++)
		{
			UPointLightComponent* C = StageMoodL[i]->PointLightComponent;
			const bool bOn = StageMoodI > 0.02f;
			if (C->IsVisible() != bOn) C->SetVisibility(bOn);
			if (!bOn) continue;
			StageMoodL[i]->SetActorLocation(FVector(CamX + (i ? 520.f : -520.f), 140.f, 300.f));
			C->SetIntensity(16000.f * StageMoodI * Pulse * (i ? 0.7f : 1.f));
			C->SetLightColor(i ? FMath::Lerp(StageMoodC, FLinearColor(1.f, 0.9f, 0.8f), 0.45f) : StageMoodC);
		}
		UPointLightComponent* D = StageMoodL[2]->PointLightComponent;
		const bool bDanger = StageMoodForce == MOOD_DANGER || (StageMoodForce < 0 && bShow && !bWon && P.Hp > 0.f && P.Hp < P.Max * 0.3f);
		if (D->IsVisible() != bDanger) D->SetVisibility(bDanger);
		if (bDanger)
		{
			StageMoodL[2]->SetActorLocation(World(P.X, P.Y, 0.f) + FVector(0, 90.f, 150.f));
			D->SetLightColor(FLinearColor(1.f, 0.1f, 0.08f));
			D->SetIntensity(9000.f * (bCalm ? 0.8f : 0.6f + 0.4f * FMath::Sin(StageT * 8.8f)));
		}
	}
}

// ---------------------------------------------------------------- development
void AFTBrawlGameMode::StageCommand(const TArray<FString>& Args)
{
	const FString Cmd = Args.Num() ? Args[0].ToLower() : TEXT("list");
	if (Cmd == TEXT("list"))
	{
		UE_LOG(LogTemp, Log, TEXT("FTBRAWL stage list breakables=%d onlookers=%d near=%d lights=%d mood=%s"), StageBreaks.Num(), StageBys.Num(), StageFg.Num(), StageLights.Num(), MoodNames[FMath::Clamp(StageMoodNow, 0, MOOD_NUM - 1)]);
		for (int32 i = 0; i < StageBreaks.Num(); i++)
		{
			const FStageBreak& B = StageBreaks[i];
			UE_LOG(LogTemp, Log, TEXT("FTBRAWL stage item i=%d kind=%s x=%.0f y=%.0f own=%d state=%d hp=%.0f/%.0f"), i, GKinds[B.Kind].Key, B.X, B.Y, B.bOwn ? 1 : 0, B.State, B.Hp, B.Max);
		}
	}
	else if (Cmd == TEXT("goto") && Args.Num() > 1)
	{
		const int32 I = FCString::Atoi(*Args[1]);
		if (!StageBreaks.IsValidIndex(I)) return;
		P.X = StageBreaks[I].X - 48.f; P.Y = SFT + 2.f; P.Dir = 1.f; P.St = TEXT("idle"); P.T = 0.f; P.Z = 0.f;
		if (LockX < 0.f) Cam = FMath::Max(0.f, P.X - SW * 0.4f);
		UE_LOG(LogTemp, Log, TEXT("FTBRAWL stage goto i=%d kind=%s nova x=%.0f y=%.0f lock=%.0f"), I, GKinds[StageBreaks[I].Kind].Key, P.X, P.Y, LockX);
	}
	else if (Cmd == TEXT("hit") && Args.Num() > 1)
	{
		const float Dmg = Args.Num() > 2 ? FCString::Atof(*Args[2]) : 10.f;
		if (Args[1].ToLower() == TEXT("all")) { for (int32 i = 0; i < StageBreaks.Num(); i++) StageHit(i, Dmg, 1.f, false, TEXT("command")); }
		else StageHit(FCString::Atoi(*Args[1]), Dmg, 1.f, false, TEXT("command"));
	}
	else if (Cmd == TEXT("react") && Args.Num() > 1)
	{
		const FString W = Args[1].ToLower();
		const int32 Pose = W == TEXT("cheer") ? 1 : W == TEXT("flinch") ? 2 : W == TEXT("duck") ? 3 : 0;
		if (Pose) StageReact(Pose, 0.f, 0.f, 1.6f);
		UE_LOG(LogTemp, Log, TEXT("FTBRAWL stage react %s onlookers=%d"), PoseNames[Pose], StageBys.Num());
	}
	else if (Cmd == TEXT("mood") && Args.Num() > 1)
	{
		StageMoodForce = -1;
		for (int32 m = 0; m < MOOD_NUM; m++) if (Args[1].ToLower() == MoodNames[m]) StageMoodForce = m;
		UE_LOG(LogTemp, Log, TEXT("FTBRAWL stage mood forced=%d (%s)"), StageMoodForce, StageMoodForce >= 0 ? MoodNames[StageMoodForce] : TEXT("auto"));
	}
}
