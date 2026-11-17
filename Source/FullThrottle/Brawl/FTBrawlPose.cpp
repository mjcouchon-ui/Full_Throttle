// Development pose stage for the brawl: show any character in any state at any move-time, or let one move's own
// clock run at a chosen speed, so that drawings can be judged and filmed before and after an art pass.
//
//   ftb.Pose <who> <state> <timeSeconds> [x] [lane] [dir=1|-1]
//       spawns (or reuses) one quiet fighter of that kind and holds it in <state> at that move-time.
//   ftb.PoseFilm <who> <state> <speed> [x] [lane] [dir=1|-1] [t0=<seconds>] [lead=<ticks>]
//       the same, but the move's clock runs from t0 (default 0) at <speed> (1 = full, 0.25 = quarter). The clock
//       starts with the test runner's next "film" line (the engine's fixed time step coming on), or after 1 s
//       without one, so "ftb.PoseFilm Nova kick 0.25" + "film nova_kick 97 60" records the whole kick, picture 0001
//       at t0 and each later picture <speed>/60 s further on. (lead = how many film ticks the clock waits at t0; the
//       default 2 is what puts t0 on picture 0001 with the runner's "film" as it is: measured, 16 Nov 2026.)
//   ftb.PoseSweep <who> <state> <t0> <t1> [stepMs=1]
//       steps the move-time from t0 to t1 and logs every change of drawing (to check a timing chart in one call).
//   ftb.PoseBackdrop <0|1> [grey 0..1]
//       1 stands a plain card (no shadow of its own, lit by the room) behind the posed fighters, in front of the
//       painted wall: films crop small and the outline reads. The fighter is drawn and lit exactly as without it.
//   ftb.PoseClear
//       removes the posed fighters and the card; the fight carries on from where it stood.
//
// <who>   = a sprite folder under /Game/Brawl/Sprites (Nova, Punk, Earl, Disco, Dock ...), any letter case.
// <state> = [previous>]state[:combo]. state is the fight's own state name (idle walk run punch kick jump jumpkick
//           pickup lift carry hurl drink wpn toss hurt held down getup cheer / wind swing dash grab knee throw fly
//           dead lifted hurled / aim charge slamwind slam whirltell whirl dizzy draw fire swig breathe hold bowl
//           gloves lunge hug windup sweep / intro intro2 defeat signaturefollow) or any frame name (taunt, win ...).
//           Nova's wider move set (FTBrawlMoves.cpp): sidestep dashfwd dashatk runatk airjab dive launch charging heavy
//           special backatk stomp pummel throwback slam wpnair wpndash airtoss wpnbreak, and dive>idle (its landing).
//           combo: punch:1|2|3 = jab, cross, uppercut; swing:1 = the alternate swing (nightstick, boss kick or bash).
//           previous: the state the fighter was in just before (walk>idle = the stop frame, idle>walk = the first
//           step, jump>idle = the landing); turn>walk = a walk that has just turned round.
//
// The frame is picked by the fight's own code: AFTBrawlGameMode::SyncBody -> AFTBrawlFighter::Pose -> PoseSprite and
// the two studio charts. Nothing here chooses a texture. While a fighter is posed the fight stands still (no steps:
// no AI, no damage), Nova and the foes are hidden and the HUD is off. Inert unless one of the commands is called.
// One line per call: "FTBRAWL posed who= state= t= frame=<texture name> ...".
// A posed fighter stays where he is put, except the three ballistic states, which follow the fight's own numbers
// from where he stands: jump / jumpkick (up 600, gravity 1700; the kick taken 0.15 s into the jump), fly (back 300
// and up 340, a boss 180 and 260, gravity 1500), hurled (from 108 up, 230, gravity 1500). Walk phase: time x the
// fight's rate (Nova 1.5, carrying 1.2, everyone else 1.1 a second).
#include "FTBrawlGameMode.h"
#include "FTBrawlSpriteBank.h"
#include "FTBrawlFighter.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/HUD.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/PlayerController.h"
#include "HAL/IConsoleManager.h"
#include "Engine/World.h"
#include "Misc/App.h"

namespace
{
	// who plays what, and how tall his sprite stands (cm): the same numbers as SpawnFighter's Looks, GNightFoes and the
	// boss lines in SetupNight (FTBrawlGameMode.cpp). Only the size and the enemy rim colour come from here.
	struct FFTPoseCast { const TCHAR* Who; const TCHAR* Role; float H; };
	const FFTPoseCast GFTPoseCast[] = {
		{ TEXT("Nova"), TEXT("player"), 228.f },
		{ TEXT("Punk"), TEXT("rusher"), 228.f }, { TEXT("Biker"), TEXT("grappler"), 240.f }, { TEXT("Sleaze"), TEXT("thrower"), 218.f },
		{ TEXT("Bouncer"), TEXT("heavy"), 268.f }, { TEXT("Guard"), TEXT("guard"), 232.f }, { TEXT("Dealer"), TEXT("dealer"), 226.f },
		{ TEXT("Earl"), TEXT("boss"), 300.f }, { TEXT("Vinnie"), TEXT("boss"), 272.f }, { TEXT("Pitboss"), TEXT("boss"), 284.f },
		{ TEXT("Frankie"), TEXT("boss"), 204.f * 1.4f }, { TEXT("Kingpin"), TEXT("boss"), 196.f * 1.4f },
		{ TEXT("Sven"), TEXT("boss"), 214.f * 1.4f }, { TEXT("Commodore"), TEXT("boss"), 196.f * 1.4f },
		{ TEXT("Disco"), TEXT("rusher"), 228.f }, { TEXT("Hustler"), TEXT("grappler"), 236.f }, { TEXT("Barman"), TEXT("thrower"), 220.f },
		{ TEXT("Roller"), TEXT("rusher"), 230.f }, { TEXT("Muscle"), TEXT("heavy"), 272.f },
		{ TEXT("Firedancer"), TEXT("rusher"), 228.f }, { TEXT("Surfer"), TEXT("grappler"), 240.f }, { TEXT("Tourist"), TEXT("thrower"), 216.f }, { TEXT("Pitcook"), TEXT("heavy"), 268.f },
		{ TEXT("Pinsetter"), TEXT("rusher"), 226.f }, { TEXT("Leaguer"), TEXT("grappler"), 240.f }, { TEXT("Shoeclerk"), TEXT("thrower"), 216.f }, { TEXT("Lanehog"), TEXT("heavy"), 268.f },
		{ TEXT("Skibum"), TEXT("rusher"), 228.f }, { TEXT("Linemate"), TEXT("grappler"), 240.f }, { TEXT("Boarder"), TEXT("thrower"), 218.f }, { TEXT("Goalie"), TEXT("heavy"), 272.f },
		{ TEXT("Deckhand"), TEXT("rusher"), 228.f }, { TEXT("Firstmate"), TEXT("guard"), 232.f }, { TEXT("Waiter"), TEXT("thrower"), 218.f }, { TEXT("Tuxbrute"), TEXT("heavy"), 268.f },
		{ TEXT("Dock"), TEXT("rusher"), 228.f },   // the fallback sheet: SpawnFighter gives it whatever role lacks frames
	};

#if !UE_BUILD_SHIPPING
	AFTBrawlGameMode* FTPoseMode(UWorld* World) { return World ? World->GetAuthGameMode<AFTBrawlGameMode>() : nullptr; }
	FAutoConsoleCommandWithWorldAndArgs GFTBrawlPose(TEXT("ftb.Pose"), TEXT("Development: ftb.Pose <who> <state> <timeSeconds> [x] [lane] [dir=1|-1] holds a quiet fighter in a state at a move-time (see FTBrawlPose.cpp)"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World) { if (AFTBrawlGameMode* G = FTPoseMode(World)) G->PoseCommand(Args, false); }));
	FAutoConsoleCommandWithWorldAndArgs GFTBrawlPoseFilm(TEXT("ftb.PoseFilm"), TEXT("Development: ftb.PoseFilm <who> <state> <speed> [x] [lane] [dir=] [t0=] [lead=] runs the move's clock at a speed, from the next film line"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World) { if (AFTBrawlGameMode* G = FTPoseMode(World)) G->PoseCommand(Args, true); }));
	FAutoConsoleCommandWithWorldAndArgs GFTBrawlPoseSweep(TEXT("ftb.PoseSweep"), TEXT("Development: ftb.PoseSweep <who> <state> <t0> <t1> [stepMs] logs every change of drawing between two move-times"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World) { if (AFTBrawlGameMode* G = FTPoseMode(World)) G->PoseSweep(Args); }));
	FAutoConsoleCommandWithWorldAndArgs GFTBrawlPoseBackdrop(TEXT("ftb.PoseBackdrop"), TEXT("Development: ftb.PoseBackdrop <0|1> [grey] a plain card behind the posed fighters"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World) { if (AFTBrawlGameMode* G = FTPoseMode(World)) G->PoseBackdrop(Args); }));
	FAutoConsoleCommandWithWorld GFTBrawlPoseClear(TEXT("ftb.PoseClear"), TEXT("Development: remove the posed fighters and let the fight carry on"),
		FConsoleCommandWithWorldDelegate::CreateLambda([](UWorld* World) { if (AFTBrawlGameMode* G = FTPoseMode(World)) G->PoseClear(); }));
#endif
}

// sets the posed body's simulation fields for its move-time; bPrime also restarts the sprite's own state memory
// (the state before, the time the state began, a turn) so that the picture depends on nothing shown earlier
void AFTBrawlGameMode::PoseApply(int32 Index, bool bPrime)
{
	if (!PoseBodies.IsValidIndex(Index)) return;
	FFTBrawlBody& B = PoseBodies[Index];
	const FPoseRun& R = PoseRuns[Index];
	const float T = R.T;
	FName St = R.State;
	if (R.bBossKind) StyleBossSignature(R.Who == TEXT("Vinnie") ? 2 : R.Who == TEXT("Pitboss") ? 3 : 0, St, T);
	const bool bNova = R.Who == TEXT("Nova");
	B.St = St; B.T = T; B.Combo = R.Combo; B.Dir = R.Dir;
	B.Walk = St == TEXT("carry") ? T * 1.2f : St == TEXT("walk") ? T * (bNova ? 1.5f : 1.1f) : 0.f;
	float Z = 0.f, Dx = 0.f;
	if (St == TEXT("jump")) Z = FMath::Max(0.f, 600.f * T - 850.f * T * T);
	else if (St == TEXT("jumpkick")) { const float J = T + 0.15f; Z = FMath::Max(0.f, 600.f * J - 850.f * J * J); }
	else if (St == TEXT("fly"))
	{
		const float Vz = R.bBossKind ? 260.f : 340.f, Vx = R.bBossKind ? 180.f : 300.f;
		Z = FMath::Max(0.f, Vz * T - 750.f * T * T);
		Dx = -R.Dir * Vx * FMath::Min(T, Vz / 750.f);
	}
	// Nova's air moves (FTBrawlMoves.cpp): the air jab, air weapon swing and air throw taken 0.16 s into the jump; the dive
	// from the top of the jump (down 420, along 440, the fight's numbers)
	else if (St == TEXT("airjab") || St == TEXT("wpnair") || St == TEXT("airtoss")) { const float J = T + 0.16f; Z = FMath::Max(0.f, 600.f * J - 850.f * J * J); }
	else if (St == TEXT("dive"))
	{
		Z = FMath::Max(0.f, 105.9f - 420.f * T - 850.f * T * T);
		Dx = R.Dir * 440.f * FMath::Min(T, 0.187f);
	}
	else if (St == TEXT("lifted")) Z = 112.f;
	else if (St == TEXT("hurled")) Z = FMath::Max(0.f, 108.f + 230.f * T - 750.f * T * T);
	B.X = R.BaseX + Dx; B.Y = R.BaseY; B.Z = Z;
	if (bPrime && B.Actor)
	{
		AFTBrawlFighter* A = B.Actor;
		const FRotator Facing(0, B.Dir > 0 ? 0 : 180, 0), Away(0, B.Dir > 0 ? 180 : 0, 0);
		A->SetActorLocationAndRotation(World(B.X, B.Y, B.Z), R.Prev == TEXT("turn") ? Away : Facing);
		A->Pose(TEXT("posed"), 0.f, 0.f, 0, 0.f, 0.f, true);                       // no state the charts know: forgets what was shown
		if (R.Prev == TEXT("turn"))
		{
			A->Pose(St, 0.f, 0.f, B.Combo, 0.f, 0.f, true);                       // walking the other way ...
			A->SetActorRotation(Facing);
			A->Pose(St, 0.f, 0.f, B.Combo, 0.f, 0.f, true);                       // ... and round he comes, at move-time 0
		}
		else
		{
			if (!R.Prev.IsNone()) A->Pose(R.Prev, 0.f, 0.f, 0, 0.f, 0.f, true);   // the state he was in just before
			A->Pose(St, 0.f, 0.f, B.Combo, 0.f, 0.f, true);                       // the state begins at move-time 0
		}
		SyncBody(B);
	}
}

void AFTBrawlGameMode::PoseCommand(const TArray<FString>& Args, bool bFilm)
{
	const TCHAR* Cmd = bFilm ? TEXT("ftb.PoseFilm") : TEXT("ftb.Pose");
	if (Args.Num() < 3) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL pose FAIL usage: %s <who> <state> <%s> [x] [lane] [dir=1|-1]%s"), Cmd, bFilm ? TEXT("speed") : TEXT("timeSeconds"), bFilm ? TEXT(" [t0=s] [lead=ticks]") : TEXT("")); return; }
	if (bDay || InCut() || SystemScreen) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL pose FAIL needs a fight on screen (no cutscene, no day set, no menu): start the night with -brawlnocut")); return; }
	const FFTPoseCast* Cast = nullptr;
	for (const FFTPoseCast& C : GFTPoseCast) if (Args[0].Equals(C.Who, ESearchCase::IgnoreCase)) { Cast = &C; break; }
	if (!Cast) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL pose FAIL unknown who=%s"), *Args[0]); return; }
	// [previous>]state[:combo]
	FString Token = Args[1], Prev, Combo;
	FString StateText = Token;
	if (Token.Split(TEXT(">"), &Prev, &StateText)) {}
	FString Left = StateText;
	if (StateText.Split(TEXT(":"), &Left, &Combo)) StateText = Left;
	if (StateText.IsEmpty()) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL pose FAIL empty state in %s"), *Token); return; }
	const float Number = FCString::Atof(*Args[2]);
	float X = Cam + 480.f, Lane = 470.f, Dir = 1.f, T0 = 0.f; int32 Lead = 2, Positional = 0;
	for (int32 i = 3; i < Args.Num(); i++)
	{
		FString Key, Val;
		if (Args[i].Split(TEXT("="), &Key, &Val))
		{
			if (Key == TEXT("x")) X = FCString::Atof(*Val);
			else if (Key == TEXT("lane")) Lane = FCString::Atof(*Val);
			else if (Key == TEXT("dir")) Dir = FCString::Atof(*Val) < 0 ? -1.f : 1.f;
			else if (Key == TEXT("t0")) T0 = FCString::Atof(*Val);
			else if (Key == TEXT("lead")) Lead = FMath::Clamp(FCString::Atoi(*Val), 0, 30);
		}
		else if (Positional == 0) { X = FCString::Atof(*Args[i]); Positional++; }
		else if (Positional == 1) { Lane = FCString::Atof(*Args[i]); Positional++; }
	}
	Lane = FMath::Clamp(Lane, 360.f, 560.f);   // the floor band (FTBrawl::FT, FB)
	int32 Index = PoseRuns.IndexOfByPredicate([&](const FPoseRun& R) { return R.Who == Cast->Who; });
	if (Index == INDEX_NONE)
	{
		UFTBrawlSpriteBank::TestStage(this);   // anybody of the cast, in whatever room: his folder is loaded here, as expected
		AFTBrawlFighter* A = GetWorld()->SpawnActor<AFTBrawlFighter>();
		if (!A || !A->SetSprite(Cast->Who, Cast->H))
		{
			if (A) A->Destroy();
			UE_LOG(LogTemp, Error, TEXT("FTBRAWL pose FAIL no sprite frames for who=%s"), Cast->Who);
			return;
		}
		const bool bPlayerKind = FCString::Strcmp(Cast->Role, TEXT("player")) == 0;
		if (!bPlayerKind) A->SetEnemyPresentation(Cast->Who, FName(Cast->Role));
		FFTBrawlBody B;
		B.Id = 9100 + PoseBodies.Num(); B.K.Who = Cast->Who; B.K.Role = FName(Cast->Role); B.K.Height = Cast->H; B.Hp = B.Max = 1.f;
		B.Actor = A;
		if (PoseBodies.Num() == 0)
			if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
				if (AHUD* Hud = PC->GetHUD()) Hud->bShowHUD = false;
		Index = PoseBodies.Add(B);
		FPoseRun R; R.Who = Cast->Who; R.HeightCm = Cast->H; R.bBossKind = FCString::Strcmp(Cast->Role, TEXT("boss")) == 0;
		PoseRuns.Add(R);
	}
	FPoseRun& R = PoseRuns[Index];
	R.Token = Token; R.State = FName(*StateText); R.Prev = Prev.IsEmpty() ? NAME_None : FName(*Prev); R.Combo = Combo.IsEmpty() ? 0 : FCString::Atoi(*Combo);
	R.BaseX = X; R.BaseY = Lane; R.Dir = Dir;
	R.T = bFilm ? T0 : Number; R.Speed = bFilm ? FMath::Max(0.f, Number) : 0.f;
	R.Lead = Lead; R.Ticks = 0; R.bStarted = false; R.ArmedAt = FPlatformTime::Seconds();
	PoseApply(Index, true);
	SyncActors(0.f);
	if (P.Actor) P.Actor->SetActorHiddenInGame(true);
	for (FFTBrawlBody& F : Foes) if (F.Actor) F.Actor->SetActorHiddenInGame(true);
	const FFTBrawlBody& B = PoseBodies[Index];
	R.LastFrame = B.Actor->CurrentSpriteFrame();
	// where the fighter's whole 1024 px canvas is on screen (for cropping films): left, top, right, bottom in pixels
	FString Rect = TEXT("none");
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		const float Cm = R.HeightCm / 600.f;
		const FVector At = World(B.X, B.Y, B.Z);
		FVector2D A0, A1; int32 Vx = 0, Vy = 0;
		PC->GetViewportSize(Vx, Vy);
		if (PC->ProjectWorldLocationToScreen(At + FVector(-512.f * Cm, 0, 1000.f * Cm), A0) && PC->ProjectWorldLocationToScreen(At + FVector(512.f * Cm, 0, -24.f * Cm), A1))
			Rect = FString::Printf(TEXT("%d,%d,%d,%d view=%dx%d"), FMath::RoundToInt32(A0.X), FMath::RoundToInt32(A0.Y), FMath::RoundToInt32(A1.X), FMath::RoundToInt32(A1.Y), Vx, Vy);
	}
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL posed who=%s state=%s t=%.3f frame=%s speed=%.3f x=%.0f lane=%.0f z=%.0f dir=%.0f rect=%s"),
		*R.Who, *R.Token, R.T, *R.LastFrame.ToString(), R.Speed, B.X, B.Y, B.Z, R.Dir, *Rect);
}

void AFTBrawlGameMode::PoseSweep(const TArray<FString>& Args)
{
	if (Args.Num() < 4) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL pose FAIL usage: ftb.PoseSweep <who> <state> <t0> <t1> [stepMs]")); return; }
	const double T0 = FCString::Atod(*Args[2]), T1 = FCString::Atod(*Args[3]);
	const double StepMs = Args.Num() > 4 ? FMath::Clamp(FCString::Atod(*Args[4]), 0.1, 1000.0) : 1.0;
	const int32 Before = PoseRuns.Num();
	PoseCommand({ Args[0], Args[1], FString::SanitizeFloat(T0) }, false);
	const int32 Index = PoseRuns.IndexOfByPredicate([&](const FPoseRun& R) { return R.Who.Equals(Args[0], ESearchCase::IgnoreCase); });
	if (Index == INDEX_NONE || T1 < T0) { UE_LOG(LogTemp, Error, TEXT("FTBRAWL pose FAIL sweep %s %s"), *Args[0], *Args[1]); return; }
	FPoseRun& R = PoseRuns[Index];
	FFTBrawlBody& B = PoseBodies[Index];
	FName Last = B.Actor->CurrentSpriteFrame();
	int32 Changes = 1;
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL sweep who=%s state=%s t=%.4f frame=%s"), *R.Who, *R.Token, float(T0), *Last.ToString());
	const int32 Steps = FMath::Min(200000, FMath::FloorToInt32((T1 - T0) * 1000.0 / StepMs + 1e-6));
	for (int32 i = 1; i <= Steps; i++)
	{
		R.T = float(T0 + i * StepMs / 1000.0);
		PoseApply(Index, false);
		SyncBody(B);
		const FName Now = B.Actor->CurrentSpriteFrame();
		if (Now != Last)
		{
			Last = Now; Changes++;
			UE_LOG(LogTemp, Display, TEXT("FTBRAWL sweep who=%s state=%s t=%.4f frame=%s"), *R.Who, *R.Token, R.T, *Now.ToString());
		}
	}
	R.LastFrame = Last;
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL sweepdone who=%s state=%s from=%.4f to=%.4f stepms=%.2f drawings_shown=%d new=%d"), *R.Who, *R.Token, float(T0), float(T1), float(StepMs), Changes, PoseRuns.Num() - Before);
}

void AFTBrawlGameMode::PoseBackdrop(const TArray<FString>& Args)
{
	const bool bOn = Args.Num() > 0 && FCString::Atoi(*Args[0]) != 0;
	const float Grey = Args.Num() > 1 ? FMath::Clamp(FCString::Atof(*Args[1]), 0.f, 1.f) : 0.2f;
	if (PoseCard) { PoseCard->Destroy(); PoseCard = nullptr; }
	if (bOn)
	{
		UStaticMesh* Plane = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));   // (a slab, not a plane: seen from either side)
		UMaterialInterface* M = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
		AStaticMeshActor* A = Plane ? GetWorld()->SpawnActor<AStaticMeshActor>() : nullptr;
		if (A)
		{
			UStaticMeshComponent* C = A->GetStaticMeshComponent();
			C->SetMobility(EComponentMobility::Movable);
			C->SetStaticMesh(Plane);
			C->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			C->SetCastShadow(false);
			if (UMaterialInstanceDynamic* Mid = M ? UMaterialInstanceDynamic::Create(M, A) : nullptr)
			{
				Mid->SetVectorParameterValue(TEXT("Color"), FLinearColor(Grey, Grey, Grey));
				C->SetMaterial(0, Mid);
			}
			// a thin slab 60 m by 9 m standing on the floor at lane 440, behind the default lane (470) and in front of the wall and its props
			A->SetActorLocation(World(Cam + 480.f, 440.f, 0.f) + FVector(0, 0, 450.f));
			A->SetActorScale3D(FVector(60.f, 0.02f, 9.f));
			PoseCard = A;
		}
	}
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL posebackdrop on=%d grey=%.2f"), PoseCard ? 1 : 0, Grey);
}

void AFTBrawlGameMode::PoseClear()
{
	const int32 N = PoseBodies.Num();
	if (PoseCard) { PoseCard->Destroy(); PoseCard = nullptr; }
	for (FFTBrawlBody& B : PoseBodies) if (B.Actor) B.Actor->Destroy();
	PoseBodies.Reset(); PoseRuns.Reset();
	if (N > 0)
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
			if (AHUD* Hud = PC->GetHUD()) Hud->bShowHUD = true;
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL poseclear n=%d"), N);
}

// instead of the fight's steps while anything is posed: the running clocks move on, every actor is placed as usual
void AFTBrawlGameMode::TickPose(float Dt)
{
	const double Now = FPlatformTime::Seconds();
	for (int32 i = 0; i < PoseRuns.Num(); i++)
	{
		FPoseRun& R = PoseRuns[i];
		if (R.Speed <= 0.f) continue;
		if (!R.bStarted && (FApp::UseFixedTimeStep() || Now - R.ArmedAt > 1.0)) R.bStarted = true;
		if (!R.bStarted) continue;
		if (R.Ticks >= R.Lead) R.T += Dt * R.Speed;   // the first Lead ticks hold t0: the film's first picture is taken a frame or two after its line
		R.Ticks++;
		PoseApply(i, false);
	}
	SyncActors(Dt);
	if (P.Actor) P.Actor->SetActorHiddenInGame(true);
	for (FFTBrawlBody& F : Foes) if (F.Actor) F.Actor->SetActorHiddenInGame(true);
	for (int32 i = 0; i < PoseRuns.Num(); i++)
	{
		FPoseRun& R = PoseRuns[i];
		if (R.Speed <= 0.f || !R.bStarted || !PoseBodies[i].Actor) continue;
		const FName Frame = PoseBodies[i].Actor->CurrentSpriteFrame();
		if (Frame != R.LastFrame)
		{
			R.LastFrame = Frame;
			UE_LOG(LogTemp, Display, TEXT("FTBRAWL poseframe who=%s state=%s t=%.4f tick=%d frame=%s"), *R.Who, *R.Token, R.T, R.Ticks, *Frame.ToString());
		}
	}
}
