// NIGHT 1's CUTSCENES (run 49531; owner, 14 Nov 22:04: "make the cutscenes in the exact same way so we can see the whole
// aspect now, for the first level"). Played in the game itself, in the same painted 2.5D set with the same sprites:
//   1  THAT NIGHT   Nova walks into the Rusty Anchor; the guy with the beer; Big Earl. Then the fight.
//   2  LAST CALL    after Big Earl goes down.
//   3  2:40 AM      the in-between story, out back on the pier (owner, 11:03: sleazier, darker, something actually
//                   happens): Kip Lorenzo (the other double) paid the guy to start it, a photographer gets Nova bloody
//                   for Channel 9, and the guy who wanted more money goes off the end of the pier, punched by Kip's
//                   heavy. Sets up Kip sabotaging Nova from night 5; Voss is not in it (owner, 15 Nov: Voss isn't evil).
//                   Ends on the END OF NIGHT 1 card.
// The lines of 1 and 2 are the story flow's (~/Wisper/docs/full-throttle-flow, N1 THE RUSTY ANCHOR), word for word.
// A cutscene is a list of beats (FFTCutBeat): each one moves the cast and the camera, then types its line.
// Enter / J / Space hurries a line, Tab skips the cutscene, lines also run on by themselves (autopilot tests too).
// Test hooks: -brawlnocut (straight into the fight), -brawlcut=2|3 (start at LAST CALL / the pier, fight skipped).
// DAY 1's cutscene BEFORE THE DRIVE (run 73444; owner, 14 Nov 23:02: "We also need the before the race cutscene made"):
//   10 ON SET      Route 9, Bayshore, the film unit's morning call: Harlan Voss with his megaphone, Nova, and Kip Lorenzo
//                  (the other double) turning up by the KESTREL; Nova gets in and the car pulls away into THE DRIVE.
// Its own map L_Day1Set (the game's default map now: day 1 -> Enter -> night 1). The lines are the story flow's
// (DAY 1 · SCENE 1 · THE HIGHWAY ASSASSINATION, CUTSCENE · BEFORE THE DRIVE), word for word.
// THE DAYS (run 77694; owner, 15 Nov 01:17: ALL the cutscenes): every day's BEFORE THE DRIVE (cut 10; day 1 keeps its own
// staging above) and THE WRAP after the drive (cut 11), days 1-8, on the same Route 9 set (the film unit's base camp):
// morning light before, the golden hour for the wrap. Rosa Delgado, Gail Prentiss and Dee Dee Starr are cut from the
// career's cast sheets (ArtSource/sheets/rosa_a, gail_a, deedee_a). Lines: the story flow's, word for word (DayCuts).
// In the main game the career plays them around every race (FTNightFlow): BEFORE -> the race -> paycheck -> THE WRAP ->
// the night -> PRE-PRODUCTION -> the next day's BEFORE. On its own: day n BEFORE -> THE WRAP -> night n -> day n+1.
// Test hooks: -brawlday=N (L_Day1Set), -brawlwrap (start on THE WRAP), -brawlnodays (night to night, no days).
#include "FTBrawlGameMode.h"
#include "FTBrawlFighter.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/PointLight.h"
#include "Engine/SpotLight.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "EngineUtils.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/Texture.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Misc/CommandLine.h"
#include "Misc/PackageName.h"
#include "Misc/Parse.h"
#include "Kismet/GameplayStatics.h"

#if __has_include("FTNightFlow.h")   // the main game: day 1 opened from the title hands on to L1 BAYSHORE (FTNightFlow)
#include "FTNightFlow.h"
#define FTB_CAREER_DAY1 1
#else
#define FTB_CAREER_DAY1 0
#endif

DEFINE_LOG_CATEGORY_STATIC(LogFTCut, Log, All);

namespace
{
	const float CPX = 1.4f, CFB = 560.f, CCamY = 1450.f, CCamZ = 540.f;
	FLinearColor Hx(const TCHAR* H) { return FLinearColor(FColor::FromHex(H)); }
	// who speaks: name on the box and its colour
	struct FSpeaker { const TCHAR* Key; const TCHAR* Name; const TCHAR* Col; };
	const FSpeaker Speakers[] = {
		{ TEXT("nova"), TEXT("NOVA KANE"), TEXT("36f1ff") }, { TEXT("guy"), TEXT("A GUY"), TEXT("ff8a3d") },
		{ TEXT("earl"), TEXT("BIG EARL"), TEXT("ff4f6e") }, { TEXT("voss"), TEXT("HARLAN VOSS"), TEXT("ffd23f") },
		{ TEXT("photo"), TEXT("THE PHOTOGRAPHER"), TEXT("b38cff") }, { TEXT("kip"), TEXT("KIP LORENZO"), TEXT("9be564") },
		{ TEXT("vinnie"), TEXT("VINNIE VELOUR"), TEXT("ff4fb8") }, { TEXT("pitboss"), TEXT("THE PIT BOSS"), TEXT("ffd23f") },   // nights 2-7 (run 32365)
		{ TEXT("frankie"), TEXT("FLAMBÉ FRANKIE"), TEXT("ff8a3d") }, { TEXT("kingpin"), TEXT("THE KINGPIN"), TEXT("36f1ff") },
		{ TEXT("sven"), TEXT("BIG SVEN"), TEXT("ff4f6e") }, { TEXT("commodore"), TEXT("THE COMMODORE"), TEXT("f4f4f0") },
		{ TEXT("rosa"), TEXT("ROSA DELGADO"), TEXT("ff6a4d") }, { TEXT("gail"), TEXT("GAIL PRENTISS"), TEXT("c9b8e8") },   // the days (run 77694)
		{ TEXT("deedee"), TEXT("DEE DEE STARR"), TEXT("ff4fb8") },
		{ TEXT("vo"), TEXT(""), TEXT("f4e9d8") } };

	// NIGHTS 2-7's cutscenes (run 32365; owner, 15 Nov 01:17: "ALL the cutscenes for every night (before/after, same beats as
	// night 1)"): THAT NIGHT before the fight and LAST CALL after the boss, in the night's own painted room with its own cast,
	// then the END OF NIGHT card. The lines are the story's (prototypes/brawl/mode.js DAYS[n].night / .won), word for word.
	struct FCutLine { const TCHAR* Who; const TCHAR* Line; };
	struct FNightCut
	{
		const TCHAR* Title; const TCHAR* Sub;            // THAT NIGHT's title card
		const TCHAR* Boss; const TCHAR* BossSprite;      // speaker key, sprite folder
		float BossH;                                     // his sprite height (cm, as in the fight)
		const TCHAR* Guy; float GuyH;                    // who starts it (a wave's sprite; speaks as "guy" if the story has him)
		const TCHAR* Third; float ThirdH;                // night 7: Kip, who speaks for himself
		FCutLine Night[5]; FCutLine Won[4];
		const TCHAR* LastSub; const TCHAR* Next;         // LAST CALL's card, the END OF NIGHT card's NEXT line
	};
	const FNightCut NightCuts[6] = {
		{ TEXT("NIGHT 2 · THE VELVET ROOM"), TEXT("A DISCO ON SUNSET · NO SNEAKERS"), TEXT("vinnie"), TEXT("Vinnie"), 272.f, TEXT("Disco"), 228.f, nullptr, 0.f,
			{ { TEXT("vo"), TEXT("The Velvet Room on Sunset. Just one dance.") }, { TEXT("guy"), TEXT("That’s my girl you’re dancing with.") },
			  { TEXT("nova"), TEXT("Then she’s got great taste in partners. Better than in boyfriends.") }, { TEXT("vinnie"), TEXT("Nobody out-dances Vinnie Velour in Vinnie Velour’s room.") } },
			{ { TEXT("vo"), TEXT("Vinnie Velour went through his own light-up floor.") }, { TEXT("vinnie"), TEXT("My floor… my beautiful floor…") }, { TEXT("nova"), TEXT("I’m a lover. Mostly.") } },
			TEXT("THE VELVET ROOM · 2:20 AM"), TEXT("NEXT: DAY 3 · NEVADA  →  NIGHT 3 · THE LUCKY 7") },
		{ TEXT("NIGHT 3 · THE LUCKY 7"), TEXT("A CASINO ON FREMONT STREET · THE HOUSE ALWAYS WINS"), TEXT("pitboss"), TEXT("Pitboss"), 284.f, TEXT("Dealer"), 226.f, nullptr, 0.f,
			{ { TEXT("vo"), TEXT("The Lucky 7 on Fremont Street. One hand of cards. To celebrate.") }, { TEXT("pitboss"), TEXT("Sir. You can’t count cards on your fingers.") },
			  { TEXT("nova"), TEXT("Watch me count to five, then.") }, { TEXT("pitboss"), TEXT("Close the doors. And somebody bring me my piece.") } },
			{ { TEXT("vo"), TEXT("The Pit Boss folded.") }, { TEXT("nova"), TEXT("The house always wins? Not tonight.") }, { TEXT("vo"), TEXT("Hollywood After Dark had it by breakfast.") } },
			TEXT("THE LUCKY 7 · 3:05 AM"), TEXT("NEXT: DAY 4 · MOUNT KALDERA  →  NIGHT 4 · THE TIKI TORCH") },
		{ TEXT("NIGHT 4 · THE TIKI TORCH"), TEXT("A LUAU LOUNGE BELOW MOUNT KALDERA · FIRE SHOW AT TEN"), TEXT("frankie"), TEXT("Frankie"), 286.f, TEXT("Tourist"), 216.f, nullptr, 0.f,
			{ { TEXT("vo"), TEXT("The Tiki Torch, at the foot of the mountain. The crew luau.") }, { TEXT("frankie"), TEXT("And now, for the volcano boys: the great Flambé Frankie… breathes FIRE!") },
			  { TEXT("nova"), TEXT("Pal, I just drove through a volcano. You’re a cigarette lighter.") }, { TEXT("frankie"), TEXT("A LIGHTER?!") } },
			{ { TEXT("vo"), TEXT("Flambé Frankie put himself out in the punch bowl.") }, { TEXT("nova"), TEXT("Somebody get the man a mint.") } },
			TEXT("THE TIKI TORCH · 1:40 AM"), TEXT("NEXT: DAY 5 · COASTER COVE  →  NIGHT 5 · STRIKE CITY") },
		{ TEXT("NIGHT 5 · STRIKE CITY"), TEXT("A BOWLING ALLEY OFF THE BOARDWALK · LEAGUE NIGHT"), TEXT("kingpin"), TEXT("Kingpin"), 274.f, TEXT("Pinsetter"), 226.f, nullptr, 0.f,
			{ { TEXT("vo"), TEXT("Strike City. League night. Kip said the whole crew would be there. Nobody from the crew is there.") },
			  { TEXT("kingpin"), TEXT("You’re the stunt guy? Your pal Kip called. Says you said I bowl like a grandma.") }, { TEXT("nova"), TEXT("Never met you. But that shirt is a gutter ball.") } },
			{ { TEXT("vo"), TEXT("The Kingpin got picked up as a spare.") }, { TEXT("nova"), TEXT("Wait. Kip told him WHAT?") } },
			TEXT("STRIKE CITY · 1:15 AM"), TEXT("NEXT: DAY 6 · FROSTBITE PEAK  →  NIGHT 6 · THE FROSTBITE LODGE") },
		{ TEXT("NIGHT 6 · THE FROSTBITE LODGE"), TEXT("AN APRÈS-SKI BAR ON FROSTBITE PEAK · HOT COCOA $1"), TEXT("sven"), TEXT("Sven"), 300.f, TEXT("Linemate"), 240.f, nullptr, 0.f,
			{ { TEXT("vo"), TEXT("The Frostbite Lodge. Hot cocoa. A fireplace. Nobody knows him up here.") },
			  { TEXT("sven"), TEXT("You. Stunt boy. A little guy called. Says you tell people Sven skates like a baby deer.") }, { TEXT("nova"), TEXT("…Kip.") }, { TEXT("sven"), TEXT("Sven drops the gloves.") } },
			{ { TEXT("vo"), TEXT("Big Sven went past the fireplace, through the window and into a snowbank.") }, { TEXT("nova"), TEXT("Tell Kip I said hi.") } },
			TEXT("THE FROSTBITE LODGE · 12:50 AM"), TEXT("NEXT: DAY 7 · THE BIG TANK  →  NIGHT 7 · THE SEA BISCUIT") },
		{ TEXT("NIGHT 7 · THE SEA BISCUIT"), TEXT("THE COMMODORE’S YACHT · BLACK TIE · NO STUNTMEN"), TEXT("commodore"), TEXT("Commodore"), 274.f, TEXT("Firstmate"), 232.f, TEXT("Kip"), 212.f,
			{ { TEXT("vo"), TEXT("The Sea Biscuit, the Commodore’s yacht. Nova behaves. For forty minutes.") }, { TEXT("kip"), TEXT("Commodore, sir! He said your boat is a BATHTUB!") },
			  { TEXT("commodore"), TEXT("A BATHTUB?! Throw him to the fish!") }, { TEXT("nova"), TEXT("Kip!") } },
			{ { TEXT("vo"), TEXT("The Commodore went over the rail with his own anchor. Kip jumped in after him.") },
			  { TEXT("commodore"), TEXT("(dripping) …Nobody has hit me since 1951. I like this one. Gail! Put him on the poster.") }, { TEXT("nova"), TEXT("Man overboard. Two men.") } },
			TEXT("THE SEA BISCUIT · 2:30 AM"), TEXT("NEXT: DAY 8 · MIDTOWN · THE LAST SCENE  →  THE WRAP, THEN THE PREMIERE") } };

	// THE DAYS' cutscenes (run 77694): BEFORE THE DRIVE and THE WRAP of days 1-8, the story flow's lines word for word
	// (~/Wisper/docs/full-throttle-flow, DAY n · CUTSCENE · BEFORE THE DRIVE / THE WRAP). A line names its speaker and the
	// pose it is said in (a frame of that sprite).
	struct FDayLine { const TCHAR* Who; const TCHAR* Pose; const TCHAR* Line; };
	struct FDayCut
	{
		const TCHAR* Title; const TCHAR* Place;          // DAY n · SCENE n · ..., the location (the title card's second line)
		FDayLine Before[6]; FDayLine Wrap[5];
		const TCHAR* Drive; const TCHAR* NightName;      // THE DRIVE card's race; the night after the wrap ("" = none, day 8)
	};
	const FDayCut DayCuts[8] = {
		{ TEXT("DAY 1 · SCENE 1 · THE HIGHWAY ASSASSINATION"), TEXT("BAYSHORE · ROUTE 9"),
			{ },   // day 1's BEFORE THE DRIVE has its own staging (StartCut, Id 10)
			{ { TEXT("voss"), TEXT("order"), TEXT("That’s scene one. Go home, Kane. Sleep.") },
			  { TEXT("nova"), TEXT("taunt"), TEXT("Sleep. Sure. Right after one drink.") },
			  { TEXT("rosa"), TEXT("mad"), TEXT("One. I’m counting.") } },
			TEXT("L1 BAYSHORE"), TEXT("NIGHT 1 · THE RUSTY ANCHOR") },
		{ TEXT("DAY 2 · SCENE 2 · THE FORMULA"), TEXT("VOLTA CHEMICAL WORKS"),
			{ { TEXT("rosa"), TEXT("mad"), TEXT("Is that a black eye?") },
			  { TEXT("nova"), TEXT("taunt"), TEXT("Makeup. Method.") },
			  { TEXT("gail"), TEXT("mad"), TEXT("It was on Channel 9, Nova. “STUNTMAN WRECKS PIER BAR.” The insurance people called. Twice.") },
			  { TEXT("rosa"), TEXT("idle"), TEXT("The tubes, the helix, the reactor. Don’t touch the walls.") } },
			{ { TEXT("voss"), TEXT("frame"), TEXT("Kane. That was… fine. Fine work.") },
			  { TEXT("nova"), TEXT("taunt"), TEXT("Fine? I drove on a ceiling. I’m going dancing.") },
			  { TEXT("gail"), TEXT("talk"), TEXT("Dancing is fine. Dancing is not a fight. Promise me.") },
			  { TEXT("nova"), TEXT("win"), TEXT("Scout’s honour.") } },
			TEXT("L2 CHEMICAL WORKS"), TEXT("NIGHT 2 · THE VELVET ROOM") },
		{ TEXT("DAY 3 · SCENE 3 · ALL IN"), TEXT("NEVADA · HIGHWAY 93"),
			{ { TEXT("voss"), TEXT("point"), TEXT("Two nights, two fights, two headlines. You are on thin ice, Kane.") },
			  { TEXT("nova"), TEXT("taunt"), TEXT("Then I drive fast. Ice can’t crack if you never stop.") },
			  { TEXT("voss"), TEXT("order"), TEXT("Highway 93 to Las Vegas. Bring my car back in one piece.") },
			  { TEXT("nova"), TEXT("idle"), TEXT("Which piece?") } },
			{ { TEXT("voss"), TEXT("mega"), TEXT("…That’s the shot. Okay. OKAY! We’re in Vegas tonight. Everybody stays in the hotel.") },
			  { TEXT("nova"), TEXT("idle"), TEXT("Everybody?") },
			  { TEXT("voss"), TEXT("shout"), TEXT("ESPECIALLY you.") } },
			TEXT("L3 NEVADA"), TEXT("NIGHT 3 · THE LUCKY 7") },
		{ TEXT("DAY 4 · SCENE 4 · BAPTISM OF FIRE"), TEXT("MOUNT KALDERA"),
			{ { TEXT("deedee"), TEXT("wave"), TEXT("Dee Dee Starr, Hollywood After Dark! Stunt double Nova Kane: three nights, three brawls. Is this man a MENACE?") },
			  { TEXT("voss"), TEXT("frame"), TEXT("Gail wants you off the picture, Kane. I said one more chance.") },
			  { TEXT("nova"), TEXT("idle"), TEXT("Thanks, Harlan.") },
			  { TEXT("voss"), TEXT("point"), TEXT("Don’t thank me. Drive through that volcano.") } },
			{ { TEXT("rosa"), TEXT("mad"), TEXT("You drove through lava, Nova. Actual lava.") },
			  { TEXT("nova"), TEXT("taunt"), TEXT("Movie lava.") },
			  { TEXT("rosa"), TEXT("cheer"), TEXT("It was REAL. Crew luau at the Tiki Torch tonight. Please. Just eat a pineapple.") } },
			TEXT("L4 VOLCANO"), TEXT("NIGHT 4 · THE TIKI TORCH") },
		{ TEXT("DAY 5 · SCENE 5 · THE LAST RIDE"), TEXT("COASTER COVE"),
			{ { TEXT("kip"), TEXT("ask"), TEXT("Mr. Voss, I can do the coaster. I’m rested. I don’t get in fights.") },
			  { TEXT("voss"), TEXT("order"), TEXT("Kane does the coaster.") },
			  { TEXT("nova"), TEXT("taunt"), TEXT("Thanks for keeping my seat warm, Kip.") },
			  { TEXT("kip"), TEXT("smirk"), TEXT("…Any time.") } },
			{ { TEXT("voss"), TEXT("laugh"), TEXT("A car on a roller coaster. One take. Kane, I could kiss you.") },
			  { TEXT("nova"), TEXT("idle"), TEXT("Please don’t.") },
			  { TEXT("kip"), TEXT("wave"), TEXT("Crew bowling league is at Strike City tonight, Nova. Everybody’ll be there. You should come.") } },
			TEXT("L5 THEME PARK"), TEXT("NIGHT 5 · STRIKE CITY") },
		{ TEXT("DAY 6 · SCENE 6 · WHITE OUT"), TEXT("FROSTBITE PEAK"),
			{ { TEXT("nova"), TEXT("idle"), TEXT("Kip sent me into a fight last night, Rosa.") },
			  { TEXT("rosa"), TEXT("cheer"), TEXT("Kip? Little Kip? He cries at the dailies.") },
			  { TEXT("voss"), TEXT("mega"), TEXT("Frostbite Peak. The half pipe, the whiteout. Kane: if you fall, fall in a way I can use.") } },
			{ { TEXT("rosa"), TEXT("mad"), TEXT("I asked around. Kip’s been calling bars all week. Telling the owners you’re coming, and what you said about them.") },
			  { TEXT("nova"), TEXT("idle"), TEXT("Why?") },
			  { TEXT("rosa"), TEXT("mad"), TEXT("You get fired, he drives scene eight.") },
			  { TEXT("nova"), TEXT("taunt"), TEXT("Then tonight I stay in. Hot cocoa. A fireplace. Nobody knows me up here.") } },
			TEXT("L6 ICE MOUNTAIN"), TEXT("NIGHT 6 · THE FROSTBITE LODGE") },
		{ TEXT("DAY 7 · SCENE 7 · SINK OR SWIM"), TEXT("THE BIG TANK"),
			{ { TEXT("gail"), TEXT("talk"), TEXT("The Commodore is on set today. He’s the money, Nova. He is paying for this picture.") },
			  { TEXT("voss"), TEXT("frame"), TEXT("The big tank. A road on the sea floor and a whole sunken city. Don’t crack my glass.") },
			  { TEXT("nova"), TEXT("taunt"), TEXT("I’ll be gentle.") } },
			{ { TEXT("commodore"), TEXT("taunt"), TEXT("Splendid! Splendid driving, young man. Party on my yacht tonight. The whole crew!") },
			  { TEXT("gail"), TEXT("mad"), TEXT("Nova. He is the money. Do NOT hit the money.") },
			  { TEXT("nova"), TEXT("taunt"), TEXT("Not even a little?") },
			  { TEXT("gail"), TEXT("mad"), TEXT("NOVA.") } },
			TEXT("L7 UNDER THE SEA"), TEXT("NIGHT 7 · THE SEA BISCUIT") },
		{ TEXT("DAY 8 · SCENE 8 · HOSTILE TAKEOVER"), TEXT("MIDTOWN · FIFTH AND MAIN"),
			{ { TEXT("voss"), TEXT("order"), TEXT("Kip is fired. The Commodore wants you on the poster. The last scene, Kane: Midtown, midnight, the Chairman.") },
			  { TEXT("nova"), TEXT("idle"), TEXT("And after the wrap?") },
			  { TEXT("voss"), TEXT("shout"), TEXT("After the wrap you go HOME.") },
			  { TEXT("nova"), TEXT("taunt"), TEXT("One drink.") },
			  { TEXT("rosa"), TEXT("mad"), TEXT("NO.") } },
			{ { TEXT("voss"), TEXT("laugh"), TEXT("…That’s the movie. Kane. That is the movie.") },
			  { TEXT("nova"), TEXT("win"), TEXT("Drinks are on me, Harlan. Well. On somebody.") },
			  { TEXT("rosa"), TEXT("mad"), TEXT("One. I’m counting.") } },
			TEXT("L8 CITY CENTER"), TEXT("") } };
	// who is who on set: the sprite folder and the height in cm (Nova is 228)
	struct FDayActor { const TCHAR* Key; const TCHAR* Sprite; float H; };
	const FDayActor DayActors[] = { { TEXT("voss"), TEXT("Voss"), 222.f }, { TEXT("kip"), TEXT("Kip"), 212.f }, { TEXT("rosa"), TEXT("Rosa"), 205.f },
		{ TEXT("gail"), TEXT("Gail"), 212.f }, { TEXT("deedee"), TEXT("Deedee"), 220.f }, { TEXT("commodore"), TEXT("Commodore"), 250.f } };
	const TCHAR* const SceneWord[8] = { TEXT("ONE"), TEXT("TWO"), TEXT("THREE"), TEXT("FOUR"), TEXT("FIVE"), TEXT("SIX"), TEXT("SEVEN"), TEXT("EIGHT") };
}

FVector AFTBrawlGameMode::RigLoc() const
{
	return FVector(World(Cam + 480.f, 460.f, 0).X, CCamY, CCamZ);   // where SyncActors puts the fight camera
}

FFTCutActor& AFTBrawlGameMode::AddCast(FName Key, const FString& Who, float HeightCm, float Scale, const FVector& At, float Dir)
{
	if (FFTCutActor* Old = CutCast.Find(Key)) { if (Old->A) Old->A->Destroy(); }
	FFTCutActor C;
	C.A = GetWorld()->SpawnActor<AFTBrawlFighter>();
	if (!C.A->SetSprite(Who, HeightCm)) C.A->SetSprite(TEXT("Dock"), HeightCm);
	C.Pos = C.Goal = At; C.Dir = Dir; C.Scale = Scale;
	return CutCast.Add(Key, C);
}

void AFTBrawlGameMode::Walk(FName Key, const FVector& To, float Speed)
{
	if (FFTCutActor* C = CutCast.Find(Key)) { C->Goal = To; C->Speed = Speed; C->bHidden = false; }
}

void AFTBrawlGameMode::PoseAs(FName Key, FName Pose, float Dir)
{
	if (FFTCutActor* C = CutCast.Find(Key)) { C->Pose = Pose; C->T = 0; if (Dir != 0) C->Dir = Dir; }
}

void AFTBrawlGameMode::CamTo(const FVector& Loc, float Pitch, float Fov, float Rate, bool bSnap)
{
	CutCamGoal = Loc; CutPitchGoal = Pitch; CutFovGoal = Fov; CutCamRate = Rate;
	if (bSnap) { CutCamLoc = Loc; CutPitch = Pitch; CutFov = Fov; }
}

// ---------------------------------------------------------------- the three cutscenes
void AFTBrawlGameMode::StartCut(int32 Id)
{
	for (auto& Kv : CutCast) if (Kv.Value.A) Kv.Value.A->Destroy();
	CutCast.Reset(); Beats.Reset();
	CutId = Id; CutBeat = -1; CutT = 0; CutLine.Reset(); CutWho.Reset(); bCutWait = false; CutTitleT = -1;
	bCutPrev = bCutEnterPrev = bCutSkipPrev = true;   // 3.9: a key or click still held from the fight is not a press here
	CutCard.Reset(); CutCardSub.Reset(); CutCardKeys.Reset();
	CutBarsGoal = 1;
	// the fight's own bodies stand aside while a cutscene plays
	if (P.Actor) P.Actor->SetActorHiddenInGame(true);
	for (FFTBrawlBody& F : Foes) if (F.Actor) F.Actor->SetActorHiddenInGame(true);
	UE_LOG(LogFTCut, Log, TEXT("FTBRAWL cut start %d"), Id);
	auto B = [this](const TCHAR* Who, const FString& Line, float Hold, TFunction<void()> Fn = nullptr) { FFTCutBeat Bt; Bt.Who = Who; Bt.Line = Line; Bt.Hold = Hold; Bt.Start = MoveTemp(Fn); Beats.Add(MoveTemp(Bt)); };
	const float Nova = 228.f, Sc = 1.f;
	if ((Id == 1 || Id == 2) && Night >= 2 && Night <= 7)
	{
		StartNightCut(Id, B);
	}
	else if ((Id == 10 && DayN >= 2) || Id == 11)
	{
		StartDayCut(Id, B);
	}
	else if (Id == 1)
	{
		CutRndSave = Rnd;
		// THAT NIGHT. The bar panel is at x -560..280 cm (stools at -350, -140, 70), the floor band is y -280..0.
		CutFade = 1; CutFadeGoal = 0;
		AddCast(TEXT("nova"), TEXT("Nova"), Nova, Sc, FVector(-520, -150, 0), 1);
		AddCast(TEXT("guy"), TEXT("Punk"), 228.f, 2.0f / 2.05f, FVector(250, -250, 0), -1);
		AddCast(TEXT("earl"), TEXT("Earl"), 300.f, 2.7f / 2.05f, FVector(1700, -170, 0), -1);
		CamTo(FVector(560, 1450, 560), -17, 50, 1.f, true);
		B(TEXT(""), TEXT(""), 2.6f, [this]() {
			CutTitle = TEXT("NIGHT 1 · THE RUSTY ANCHOR"); CutSub = TEXT("A DIVE BAR ON THE PIER · LAST CALL 2 AM"); CutTitleT = 0;
			Walk(TEXT("nova"), FVector(-60, -200, 0), 170); CamTo(FVector(260, 1150, 470), -15, 48, 0.8f); });
		B(TEXT("vo"), TEXT("The Rusty Anchor, a dive bar at the end of the pier. One drink, he said."), 2.2f, [this]() {
			Walk(TEXT("nova"), FVector(30, -262, 0), 150); });
		B(TEXT(""), TEXT(""), 1.3f, [this]() { PoseAs(TEXT("nova"), TEXT("drink"), 1); CamTo(FVector(140, 760, 330), -9, 42, 1.4f); });
		B(TEXT("guy"), TEXT("Hey. Stunt boy. You spilled my beer."), 0.f, [this]() { PoseAs(TEXT("guy"), TEXT("wind"), -1); PoseAs(TEXT("nova"), TEXT("idle"), 1); });
		B(TEXT("nova"), TEXT("Pal, I fall off buildings for a living. You want to try me?"), 0.f, [this]() { PoseAs(TEXT("nova"), TEXT("taunt"), 1); CamTo(FVector(110, 700, 320), -9, 40, 1.2f); });
		B(TEXT(""), TEXT(""), 2.4f, [this]() { Walk(TEXT("earl"), FVector(760, -200, 0), 400); CamTo(FVector(400, 1050, 400), -11, 46, 1.2f); });
		B(TEXT("earl"), TEXT("Nobody fights in my bar. Nobody but ME."), 0.f, [this]() { PoseAs(TEXT("earl"), TEXT("taunt"), -1); PoseAs(TEXT("nova"), TEXT("idle"), 1); });
		B(TEXT(""), TEXT(""), 0.9f, [this]() { PoseAs(TEXT("guy"), TEXT("punch"), -1); PoseAs(TEXT("nova"), TEXT("hurt"), 1); Shake = 0.8f;
			Bleed(30 / CPX, CFB - 262 / CPX, 120, -1, 14, 0.9f); Walk(TEXT("earl"), FVector(1800, -170, 0), 170); });
		B(TEXT(""), TEXT(""), 1.3f, [this]() { PoseAs(TEXT("nova"), TEXT("idle"), 1); Walk(TEXT("nova"), World(160, 470, 0), 260); Walk(TEXT("guy"), FVector(1500, -200, 0), 420); CamTo(RigLoc(), -16, 46, 2.2f); });
	}
	else if (Id == 2)
	{
		// LAST CALL: where the fight ended
		const int32 Bi = BossIdx();
		const FVector E = Bi != INDEX_NONE ? World(Foes[Bi].X, Foes[Bi].Y, 0) : World(P.X + 200, 470, 0);
		const float Ed = Bi != INDEX_NONE ? Foes[Bi].Dir : -1.f;
		FVector N = World(P.X, P.Y, 0);
		if (FMath::Abs(N.X - E.X) < 260) N.X = E.X + (N.X < E.X ? -260 : 260);
		const float Nd = N.X < E.X ? 1 : -1;
		AddCast(TEXT("nova"), TEXT("Nova"), Nova, Sc, N, Nd).Pose = TEXT("win");
		AddCast(TEXT("earl"), TEXT("Earl"), 300.f, 2.7f / 2.05f, E, Ed).Pose = TEXT("down");
		const float Mx = (N.X + E.X) / 2;
		CutCamLoc = RigLoc(); CutPitch = -16; CutFov = 46;
		CamTo(FVector(Mx, 900, 330), -10, 44, 1.0f);
		B(TEXT("vo"), TEXT("Big Earl went down on his own pool table."), 2.4f, [this]() { CutTitle = TEXT("LAST CALL"); CutSub = TEXT("THE RUSTY ANCHOR · 2:10 AM"); CutTitleT = 0; });
		B(TEXT("earl"), TEXT("Kid’s got a drop kick. Drinks are on the house. Forever."), 0.f, [this, E]() { PoseAs(TEXT("earl"), TEXT("getup")); CamTo(FVector(E.X, 760, 280), -8, 42, 1.0f); });
		B(TEXT("nova"), TEXT("Put it on my tab."), 0.f, [this, N]() { PoseAs(TEXT("nova"), TEXT("taunt")); CamTo(FVector(N.X, 760, 300), -9, 42, 1.0f); });
		B(TEXT("earl"), TEXT("Back door, kid. Cops come at three. And whatever you see out there, you didn't."), 0.f, [this, E, Mx]() { PoseAs(TEXT("earl"), TEXT("down")); CamTo(FVector(Mx, 900, 330), -10, 44, 1.0f); });
		B(TEXT(""), TEXT(""), 2.0f, [this, N]() { PoseAs(TEXT("nova"), TEXT("idle"), 1); Walk(TEXT("nova"), N + FVector(900, -120, 0), 200); CutFadeGoal = 1; });
		B(TEXT(""), TEXT(""), 0.3f, [this]() { StartCut(3); });
	}
	else if (Id == 3)
	{
		// 2:40 AM, out back: the bar's back door at PierX, the pier and the harbour to the right, the limousine Kip came in
		const float X = PierX;
		CutFade = 1; CutFadeGoal = 0;
		AddCast(TEXT("nova"), TEXT("Nova"), Nova, Sc, FVector(X - 20, -318, 0), 1).bHidden = true;
		AddCast(TEXT("photo"), TEXT("Sleaze"), 218.f, 2.0f / 2.05f, FVector(X + 560, -90, 0), -1).Pose = TEXT("idle");
		AddCast(TEXT("guy"), TEXT("Punk"), 228.f, 2.0f / 2.05f, FVector(X + 1000, -215, 0), 1);
		AddCast(TEXT("kip"), TEXT("Kip"), 212.f, 1.f, FVector(X + 1290, -235, 0), -1).Pose = TEXT("smirk");
		AddCast(TEXT("heavy"), TEXT("Bouncer"), 268.f, 2.3f / 2.05f, FVector(X + 1560, -150, 0), -1);
		if (Limo) { LimoAt = FVector(X + 1720, -318, 0); LimoV = 0; Limo->SetActorHiddenInGame(false); }
		CamTo(FVector(X + 330, 1350, 520), -16, 50, 1.f, true);
		B(TEXT(""), TEXT(""), 2.4f, [this, X]() { CutTitle = TEXT("2:40 AM"); CutSub = TEXT("OUT BACK OF THE RUSTY ANCHOR"); CutTitleT = 0;
			FFTCutActor& N = CutCast[TEXT("nova")]; N.bHidden = false; Walk(TEXT("nova"), FVector(X + 120, -200, 0), 120); });
		B(TEXT("vo"), TEXT("Out back, the pier was dark. Somebody’s limousine wasn’t."), 1.6f, [this, X]() { CamTo(FVector(X + 1130, 1300, 500), -15, 48, 0.7f); });
		B(TEXT("guy"), TEXT("He hit back harder than you said, Kip. That’s extra."), 0.f, [this, X]() { PoseAs(TEXT("guy"), TEXT("idle"), 1); CamTo(FVector(X + 1150, 820, 330), -9, 42, 1.0f); });
		B(TEXT("kip"), TEXT("Extra. Sure. STUNTMAN WRECKS PIER BAR, Channel 9 by six. Let’s see Voss keep him after that."), 0.f, [this]() { PoseAs(TEXT("kip"), TEXT("ask"), -1); });
		B(TEXT(""), TEXT(""), 0.7f, [this, X]() { PoseAs(TEXT("photo"), TEXT("throw"), -1); CamTo(FVector(X + 380, 900, 360), -10, 44, 2.0f); });
		B(TEXT("photo"), TEXT("Got him coming out the back, Kip. Blood and all."), 0.f, [this]() { CutFlash = 1.f; PoseAs(TEXT("nova"), TEXT("hurt"), 1); });
		B(TEXT("kip"), TEXT("Perfect. Kane bleeds, and the other double gets the part."), 0.f, [this, X]() { PoseAs(TEXT("kip"), TEXT("me"), -1); PoseAs(TEXT("nova"), TEXT("idle"), 1); PoseAs(TEXT("photo"), TEXT("idle"), -1);
			CamTo(FVector(X + 1150, 820, 330), -9, 42, 1.2f); });
		B(TEXT("guy"), TEXT("And my extra?"), 0.f, [this, X]() { Walk(TEXT("guy"), FVector(X + 1110, -225, 0), 90); });
		B(TEXT("kip"), TEXT("Your extra is you were never here."), 0.f, [this]() { PoseAs(TEXT("kip"), TEXT("smirk"), -1); PoseAs(TEXT("heavy"), TEXT("wind"), -1); });
		B(TEXT(""), TEXT(""), 0.35f, [this, X]() { Walk(TEXT("heavy"), FVector(X + 1230, -215, 0), 520); });
		B(TEXT(""), TEXT(""), 1.9f, [this, X]() {
			PoseAs(TEXT("heavy"), TEXT("punch"), -1); Shake = 1.f;
			FFTCutActor& G = CutCast[TEXT("guy")]; G.bFly = true; G.T = 0; G.Vel = FVector(-140, -420, 900); G.Pose = TEXT("fly");
			Bleed((X + 1110) / CPX, CFB - 225 / CPX, 130, -1, 22, 1.2f);
			CamTo(FVector(X + 1000, 1250, 470), -15, 48, 1.4f); });
		B(TEXT("nova"), TEXT("…Kip?"), 0.f, [this, X]() { PoseAs(TEXT("heavy"), TEXT("idle"), -1); Walk(TEXT("heavy"), FVector(X + 1560, -150, 0), 160); Walk(TEXT("nova"), FVector(X + 330, -200, 0), 90); CamTo(FVector(X + 300, 820, 320), -9, 42, 1.2f); });
		B(TEXT("kip"), TEXT("Tide’s out. He’ll keep. See you on set, Kane."), 0.f, [this, X]() { PoseAs(TEXT("kip"), TEXT("wave"), -1); CamTo(FVector(X + 1300, 860, 340), -9, 42, 1.2f); });
		// Kip and his heavy get in the limousine
		B(TEXT(""), TEXT(""), 1.4f, [this, X]() { Walk(TEXT("kip"), FVector(X + 1700, -300, 0), 300); Walk(TEXT("heavy"), FVector(X + 1760, -290, 0), 300); });
		B(TEXT(""), TEXT(""), 3.2f, [this, X]() {
			CutCast[TEXT("kip")].bHidden = true; CutCast[TEXT("heavy")].bHidden = true; LimoV = 30.f;
			Walk(TEXT("photo"), FVector(X + 2600, -60, 0), 260);
			CamTo(FVector(X + 1100, 1350, 500), -16, 50, 0.8f); });
		B(TEXT("vo"), TEXT("Nobody ever found the guy. Channel 9 had the picture by six."), 2.0f, [this, X]() { Walk(TEXT("nova"), FVector(X + 1080, -265, 0), 110); CamTo(FVector(X + 900, 1200, 480), -15, 48, 0.6f); });
		B(TEXT(""), TEXT(""), 1.6f, [this]() { CutFadeGoal = 0.85f; });
		{
			FFTCutBeat End; End.bWait = true;
			End.Start = [this]() {
				CutCard = TEXT("END OF NIGHT 1");
				CutCardSub = TEXT("NEXT: DAY 2 · VOLTA CHEMICAL WORKS  →  NIGHT 2 · THE VELVET ROOM");
				CutCardKeys = FString::Printf(TEXT("ENTER/A: ON TO %s   ·   R: FIGHT NIGHT 1 AGAIN"), *NextLabel());
				UE_LOG(LogFTCut, Log, TEXT("FTBRAWL cut end card"));
			};
			Beats.Add(MoveTemp(End));
		}
	}
	else if (Id == 10)
	{
		// DAY 1 · ON SET. The set's back panels stand at y -340; the band is y -280..0. Left to right: the camera dolly and
		// Voss's chair, Voss, the KESTREL parked at the back of the road, the apple boxes; Kip comes in from the right.
		const float X = SetX;
		CutFade = 1; CutFadeGoal = 0;
		AddCast(TEXT("voss"), TEXT("Voss"), 222.f, 1.f, FVector(X - 330, -190, 0), 1).Pose = TEXT("mega");
		AddCast(TEXT("nova"), TEXT("Nova"), Nova, Sc, FVector(X + 1250, -150, 0), -1);   // from the right, past his car
		AddCast(TEXT("kip"), TEXT("Kip"), 212.f, 1.f, FVector(X + 1700, -170, 0), -1).bHidden = true;
		if (Limo) { LimoAt = FVector(X + 520, -300, 0); LimoV = 0; Limo->SetActorHiddenInGame(false); }
		CamTo(FVector(X - 200, 1450, 540), -16, 50, 1.f, true);
		B(TEXT(""), TEXT(""), 2.8f, [this, X]() {
			CutTitle = TEXT("DAY 1 · SCENE 1 · THE HIGHWAY ASSASSINATION"); CutSub = TEXT("BAYSHORE · ROUTE 9 · ON SET, 7 AM"); CutTitleT = 0; CutFadeGoal = 0.35f;   // the title over a dimmed set (pink on the pastel sky doesn't read)
			Walk(TEXT("nova"), FVector(X + 40, -170, 0), 300); CamTo(FVector(X + 100, 1250, 480), -15, 48, 0.7f); });
		B(TEXT("voss"), TEXT("Kane. Scene one: the getaway on Route 9. Eight scenes, eight days, and the car is the star."), 0.f, [this, X]() {
			CutFadeGoal = 0; PoseAs(TEXT("voss"), TEXT("order"), 1); PoseAs(TEXT("nova"), TEXT("idle"), -1); CamTo(FVector(X - 150, 860, 330), -9, 42, 1.1f); });
		B(TEXT("nova"), TEXT("And I make the star look good. Roll it, Harlan."), 0.f, [this, X]() {
			PoseAs(TEXT("voss"), TEXT("mega"), 1); PoseAs(TEXT("nova"), TEXT("taunt"), -1); CamTo(FVector(X + 40, 760, 320), -9, 40, 1.2f); });
		B(TEXT(""), TEXT(""), 2.2f, [this, X]() {
			PoseAs(TEXT("nova"), TEXT("idle"), 1); CutCast[TEXT("kip")].bHidden = false; Walk(TEXT("kip"), FVector(X + 330, -150, 0), 230);
			CamTo(FVector(X + 60, 1150, 440), -14, 46, 1.0f); });
		B(TEXT("kip"), TEXT("If he breaks a leg, Mr. Voss, I’m right here. Kip Lorenzo. The other double."), 0.f, [this, X]() {
			PoseAs(TEXT("kip"), TEXT("wave"), -1); CamTo(FVector(X + 120, 820, 330), -9, 42, 1.1f); });
		B(TEXT(""), TEXT(""), 0.9f, [this]() { PoseAs(TEXT("kip"), TEXT("me"), -1); });
		B(TEXT("voss"), TEXT("Noted, Kip. Nobody breaks anything."), 0.f, [this, X]() {
			PoseAs(TEXT("voss"), TEXT("frame"), 1); PoseAs(TEXT("kip"), TEXT("nervous"), -1); CamTo(FVector(X - 120, 900, 340), -10, 44, 1.1f); });
		// nobody's looking at Kip: he isn't as nice as he sounds
		B(TEXT(""), TEXT(""), 1.6f, [this, X]() { PoseAs(TEXT("kip"), TEXT("smirk"), -1); CamTo(FVector(X + 330, 640, 300), -8, 34, 1.6f); });
		// Nova gets in the KESTREL; Voss through the megaphone; the car pulls away to the right
		B(TEXT(""), TEXT(""), 1.5f, [this, X]() {
			PoseAs(TEXT("kip"), TEXT("idle"), -1); Walk(TEXT("nova"), FVector(X + 380, -235, 0), 200); PoseAs(TEXT("voss"), TEXT("shout"), 1);
			CamTo(FVector(X + 100, 1250, 470), -15, 48, 1.0f); });
		B(TEXT(""), TEXT(""), 3.4f, [this, X]() {
			CutCast[TEXT("nova")].bHidden = true; LimoV = 40.f; Shake = 0.35f;
			CutTitle = TEXT("ROLLING"); CutSub = TEXT("SCENE ONE · TAKE ONE"); CutTitleT = 0;
			CamTo(FVector(X + 900, 1350, 500), -16, 50, 0.9f); UE_LOG(LogFTCut, Log, TEXT("FTBRAWL cut kestrel away")); });
		B(TEXT(""), TEXT(""), 1.2f, [this]() { CutFadeGoal = 0.85f; });
		{
			FFTCutBeat End; End.bWait = true;
			End.Start = [this]() {
				CutCard = TEXT("▶ THE DRIVE");
				CutCardSub = TEXT("L1 BAYSHORE · ROUTE 9, IN FULL THROTTLE  →  THEN THE WRAP AND NIGHT 1 · THE RUSTY ANCHOR");
				CutCardKeys = TEXT("ENTER/A: ON TO DAY 1 · THE WRAP   ·   R: WATCH AGAIN");   // the brawl on its own (run 77694)
#if FTB_CAREER_DAY1
				// the main game's career, started from the title: the drive comes next
				if (UFTNightFlow::InDay1(this)) CutCardKeys = TEXT("ENTER/A: ON TO THE DRIVE · L1 BAYSHORE   ·   R: WATCH AGAIN");
#endif
				UE_LOG(LogFTCut, Log, TEXT("FTBRAWL cut end card"));
			};
			Beats.Add(MoveTemp(End));
		}
	}
	NextBeat();
}

// nights 2-7: THAT NIGHT (Id 1) and LAST CALL (Id 2), night 1's beats with the night's own cast and lines (NightCuts)
void AFTBrawlGameMode::StartNightCut(int32 Id, TFunctionRef<void(const TCHAR*, const FString&, float, TFunction<void()>)> B)
{
	const FNightCut& N = NightCuts[Night - 2];
	const float Nova = 228.f, BossSc = 2.6f / 2.05f, GuySc = 2.0f / 2.05f;
	const FName BossKey = N.Boss;
	if (Id == 1)
	{
		CutRndSave = Rnd;
		CutFade = 1; CutFadeGoal = 0;
		// the bar panel is at x -560..280 cm (the counter's stools at -350, -140, 70), the floor band is y -280..0
		AddCast(TEXT("nova"), TEXT("Nova"), Nova, 1.f, FVector(-520, -150, 0), 1);
		AddCast(TEXT("guy"), N.Guy, N.GuyH, GuySc, FVector(250, -250, 0), -1);
		if (N.Third) AddCast(TEXT("kip"), N.Third, N.ThirdH, 1.f, FVector(470, -215, 0), -1);
		AddCast(BossKey, N.BossSprite, N.BossH, BossSc, FVector(1700, -170, 0), -1);
		CamTo(FVector(560, 1450, 560), -17, 50, 1.f, true);
		const FString Title = N.Title, Sub = N.Sub;
		B(TEXT(""), TEXT(""), 2.6f, [this, Title, Sub]() {
			CutTitle = Title; CutSub = Sub; CutTitleT = 0;
			Walk(TEXT("nova"), FVector(-60, -200, 0), 170); CamTo(FVector(260, 1150, 470), -15, 48, 0.8f); });
		bool bBossIn = false;
		for (const FCutLine& L : N.Night)
		{
			if (!L.Who) break;
			const FString Who = L.Who;
			if (Who == TEXT("vo"))
			{
				B(TEXT("vo"), L.Line, 2.2f, [this]() { Walk(TEXT("nova"), FVector(30, -262, 0), 150); });
				B(TEXT(""), TEXT(""), 1.3f, [this]() { PoseAs(TEXT("nova"), TEXT("drink"), 1); CamTo(FVector(140, 760, 330), -9, 42, 1.4f); });
				continue;
			}
			if (Who == N.Boss && !bBossIn)
			{
				bBossIn = true;
				B(TEXT(""), TEXT(""), 2.4f, [this, BossKey]() { PoseAs(TEXT("nova"), TEXT("idle"), 1); Walk(BossKey, FVector(760, -200, 0), 400); CamTo(FVector(400, 1050, 400), -11, 46, 1.2f); });
			}
			if (Who == N.Boss) B(L.Who, L.Line, 0.f, [this, BossKey]() { PoseAs(BossKey, TEXT("taunt"), -1); PoseAs(TEXT("nova"), TEXT("idle"), 1); CamTo(FVector(560, 900, 360), -10, 44, 1.2f); });
			else if (Who == TEXT("guy")) B(L.Who, L.Line, 0.f, [this]() { PoseAs(TEXT("guy"), TEXT("wind"), -1); PoseAs(TEXT("nova"), TEXT("idle"), 1); CamTo(FVector(140, 760, 330), -9, 42, 1.2f); });
			else if (Who == TEXT("kip")) B(L.Who, L.Line, 0.f, [this]() { PoseAs(TEXT("kip"), TEXT("me"), -1); PoseAs(TEXT("nova"), TEXT("idle"), 1); CamTo(FVector(300, 780, 330), -9, 42, 1.2f); });
			else B(L.Who, L.Line, 0.f, [this]() { PoseAs(TEXT("nova"), TEXT("taunt"), 1); CamTo(FVector(110, 700, 320), -9, 40, 1.2f); });
		}
		// it starts: the one by the bar swings at Nova (night 1's beat), the boss steps back to wait for the end of the night
		B(TEXT(""), TEXT(""), 0.9f, [this, BossKey]() { PoseAs(TEXT("guy"), TEXT("punch"), -1); PoseAs(TEXT("nova"), TEXT("hurt"), 1); Shake = 0.8f;
			if (CutCast.Contains(TEXT("kip"))) PoseAs(TEXT("kip"), TEXT("smirk"), -1);
			Bleed(30 / CPX, CFB - 262 / CPX, 120, -1, 14, 0.9f); Walk(BossKey, FVector(1800, -170, 0), 170); });
		B(TEXT(""), TEXT(""), 1.3f, [this]() { PoseAs(TEXT("nova"), TEXT("idle"), 1); Walk(TEXT("nova"), World(160, 470, 0), 260); Walk(TEXT("guy"), FVector(1500, -200, 0), 420);
			if (CutCast.Contains(TEXT("kip"))) Walk(TEXT("kip"), FVector(1600, -230, 0), 380);
			CamTo(RigLoc(), -16, 46, 2.2f); });
		return;
	}
	// LAST CALL: where the fight ended; the boss beaten (his own 'sad' frame, else dizzy / down), Nova standing
	const int32 Bi = BossIdx();
	const FVector E = Bi != INDEX_NONE ? World(Foes[Bi].X, Foes[Bi].Y, 0) : World(P.X + 200, 470, 0);
	const float Ed = Bi != INDEX_NONE ? Foes[Bi].Dir : -1.f;
	FVector Nv = World(P.X, P.Y, 0);
	if (FMath::Abs(Nv.X - E.X) < 260) Nv.X = E.X + (Nv.X < E.X ? -260 : 260);
	const float Nd = Nv.X < E.X ? 1 : -1;
	AddCast(TEXT("nova"), TEXT("Nova"), Nova, 1.f, Nv, Nd).Pose = TEXT("win");
	FFTCutActor& Bc = AddCast(BossKey, N.BossSprite, N.BossH, BossSc, E, Ed);
	const FName Sad = Bc.A && Bc.A->HasFrame(TEXT("sad")) ? FName(TEXT("sad")) : Bc.A && Bc.A->HasFrame(TEXT("dizzy")) ? FName(TEXT("dizzy")) : FName(TEXT("down"));
	Bc.Pose = TEXT("down");
	const float Mx = (Nv.X + E.X) / 2;
	CutCamLoc = RigLoc(); CutPitch = -16; CutFov = 46;
	CamTo(FVector(Mx, 900, 330), -10, 44, 1.0f);
	const FString LastSub = N.LastSub;
	bool bFirst = true;
	for (const FCutLine& L : N.Won)
	{
		if (!L.Who) break;
		const FString Who = L.Who;
		TFunction<void()> Fn;
		if (Who == TEXT("vo")) Fn = [this, Mx]() { CamTo(FVector(Mx, 900, 330), -10, 44, 1.0f); };
		else if (Who == N.Boss) Fn = [this, BossKey, Sad, E]() { PoseAs(BossKey, Sad); CamTo(FVector(E.X, 760, 280), -8, 42, 1.0f); };
		else Fn = [this, Nv]() { PoseAs(TEXT("nova"), TEXT("taunt")); CamTo(FVector(Nv.X, 760, 300), -9, 42, 1.0f); };
		if (bFirst)
		{
			TFunction<void()> Inner = MoveTemp(Fn);
			Fn = [this, LastSub, Inner, BossKey, Sad]() { CutTitle = TEXT("LAST CALL"); CutSub = LastSub; CutTitleT = 0; PoseAs(BossKey, Sad); Inner(); };
			bFirst = false;
		}
		B(L.Who, L.Line, Who == TEXT("vo") ? 2.4f : 0.f, MoveTemp(Fn));
	}
	B(TEXT(""), TEXT(""), 2.0f, [this, Nv]() { PoseAs(TEXT("nova"), TEXT("idle"), 1); Walk(TEXT("nova"), Nv + FVector(900, -120, 0), 200); CutFadeGoal = 1; });
	B(TEXT(""), TEXT(""), 0.6f, [this]() { CutFadeGoal = 0.85f; });
	FFTCutBeat End; End.bWait = true;
	const FString Next = N.Next;
	End.Start = [this, Next]() {
		CutCard = FString::Printf(TEXT("END OF NIGHT %d"), Night);
		CutCardSub = Next;
		const FString On = NextLabel();
		CutCardKeys = On.IsEmpty() ? FString::Printf(TEXT("ENTER/A: FIGHT NIGHT %d AGAIN"), Night) : FString::Printf(TEXT("ENTER/A: ON TO %s   ·   R: FIGHT NIGHT %d AGAIN"), *On, Night);
		UE_LOG(LogFTCut, Log, TEXT("FTBRAWL cut end card night %d"), Night);
	};
	Beats.Add(MoveTemp(End));
}

void AFTBrawlGameMode::NextBeat()
{
	CutBeat++; CutT = 0; CutLineT = 0;
	if (!Beats.IsValidIndex(CutBeat)) { EndCut(); return; }
	const int32 Id = CutId;
	FFTCutBeat& Bt = Beats[CutBeat];
	CutLine = Bt.Line;
	CutWho.Reset(); CutWhoCol = Hx(TEXT("f4e9d8"));
	for (const FSpeaker& S : Speakers) if (Bt.Who == S.Key) { CutWho = S.Name; CutWhoCol = Hx(S.Col); }
	bCutWait = Bt.bWait;
	if (!Bt.Line.IsEmpty()) UE_LOG(LogFTCut, Log, TEXT("FTBRAWL cut %d line %s: %s"), Id, *Bt.Who, *Bt.Line);
	if (Bt.Start) { TFunction<void()> Fn = Bt.Start; Fn(); }   // may start another cutscene (and reset Beats)
}

void AFTBrawlGameMode::EndCut()
{
	const int32 Was = CutId;
	CutId = 0; CutBeat = -1; CutLine.Reset(); CutWho.Reset(); CutTitleT = -1; CutFade = 0; CutFadeGoal = 0; CutFlash = 0; CutBars = 0; CutBarsGoal = 0;
	CutCard.Reset(); CutCardSub.Reset(); CutCardKeys.Reset(); bCutWait = false;
	FVector NovaAt = FVector::ZeroVector; bool bNova = false;
	if (FFTCutActor* N = CutCast.Find(TEXT("nova"))) { NovaAt = N->Pos; bNova = true; }
	for (auto& Kv : CutCast) if (Kv.Value.A) Kv.Value.A->Destroy();
	CutCast.Reset();
	if (Limo) Limo->SetActorHiddenInGame(true);
	if (CamActor) CamActor->GetCameraComponent()->SetFieldOfView(46.f);
	if (P.Actor) P.Actor->SetActorHiddenInGame(false);
	UE_LOG(LogFTCut, Log, TEXT("FTBRAWL cut end %d"), Was);
	if (Was == 1)
	{
		// into the fight: Nova where the cutscene left him
		// into the fight exactly as it starts without the cutscene: Nova on his mark (he walks there in the last beat),
		// the same random stream (the cutscene's blood used some)
		(void)bNova; (void)NovaAt;
		P.X = 160; P.Y = 470; Rnd = CutRndSave;
		Banner = TEXT("FIGHT!"); BannerSub = VenueName + TEXT(" · ") + VenueTag; BannerT = 0;
		UE_LOG(LogFTCut, Log, TEXT("FTBRAWL fight begins x=%.0f y=%.0f"), P.X, P.Y);
	}
}

void AFTBrawlGameMode::StepCut(float Dt, const FIn& In)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	const bool bNext = In.Enter || In.Fire || In.Jump || In.Kick;
	const bool bSkip = PC && (PC->IsInputKeyDown(EKeys::Tab) || PC->IsInputKeyDown(EKeys::Gamepad_Special_Left));   // Back/View on a pad
	const bool bNextPress = bNext && !bCutPrev, bSkipPress = bSkip && !bCutSkipPrev, bEnterPress = In.Enter && !bCutEnterPrev;
	bCutPrev = bNext; bCutSkipPrev = bSkip; bCutEnterPrev = In.Enter;
	CutT += Dt; CutLineT += Dt;
	if (CutTitleT >= 0) { CutTitleT += Dt; if (CutTitleT > 3.2f) CutTitleT = -1; }
	CutFade = FMath::FInterpConstantTo(CutFade, CutFadeGoal, Dt, 1.4f);
	CutBars = FMath::FInterpConstantTo(CutBars, CutBarsGoal, Dt, 2.5f);
	CutFlash = FMath::Max(0.f, CutFlash - Dt * 2.2f);
	if (Shake > 0) Shake = FMath::Max(0.f, Shake - Dt * 1.8f);
	StT += Dt;
	StepFx(Dt);
	// the cast
	for (auto& Kv : CutCast)
	{
		FFTCutActor& C = Kv.Value;
		C.T += Dt;
		if (C.bFly)
		{
			C.Vel.Z -= 2100.f * Dt; C.Pos += C.Vel * Dt; C.Lean = FMath::Min(80.f, C.T * 160.f);
			if (C.Pos.Z < -160.f && !C.bHidden)
			{
				C.bHidden = true;   // into the water: a splash off the end of the pier
				for (int32 i = 0; i < 26; i++)
				{
					FFTBrawlDrop D; D.P = FVector(C.Pos.X + (Rand() - 0.5f) * 60, -470.f, -20.f);
					D.V = FVector((Rand() - 0.5f) * 260, (Rand() - 0.5f) * 120, 520 + Rand() * 520); D.S = 0.05f + Rand() * 0.05f; D.bGlass = true;
					Drops.Add(D);
				}
				UE_LOG(LogFTCut, Log, TEXT("FTBRAWL cut splash"));
			}
			continue;
		}
		const FVector To = C.Goal - C.Pos;
		const float D = To.Size2D();
		if (D > 3.f)
		{
			const float Stepd = FMath::Min(D, C.Speed * Dt);
			C.Pos += To.GetSafeNormal2D() * Stepd;
			if (FMath::Abs(To.X) > 4.f) C.Dir = To.X > 0 ? 1 : -1;
			C.Walk += Dt * C.Speed / 220.f;
		}
	}
	// the limousine pulls away
	if (Limo && LimoV > 0) { LimoV = FMath::Min(900.f, LimoV + 420.f * Dt); LimoAt.X += LimoV * Dt; }
	// the beat: run on when its time is up (or a key hurries it); Tab skips to the end of this cutscene
	if (bSkipPress && !bCutWait)
	{
		UE_LOG(LogFTCut, Log, TEXT("FTBRAWL cut %d skipped"), CutId);
		UE_LOG(LogFTCut, Log, TEXT("FT BRAWL screen=cut%d key=skip"), CutId);
		if (CutId == 1) EndCut();
		else if (CutId == 2 && Night == 1) StartCut(3);
		else if (CutId == 10 || CutId == 11) { if (FFTCutActor* N = CutCast.Find(TEXT("nova"))) N->bHidden = true; if (Limo) Limo->SetActorHiddenInGame(true);
			CutBeat = Beats.Num() - 2; CutFade = CutFadeGoal = 0.85f; NextBeat(); }
		else { CutBeat = Beats.Num() - 2; CutFade = CutFadeGoal = 0.85f; NextBeat(); }
		return;
	}
	if (!Beats.IsValidIndex(CutBeat)) return;
	const FFTCutBeat& Bt = Beats[CutBeat];
	const float Typed = Bt.Line.Len() / 45.f;
	const float Dur = Bt.Line.IsEmpty() ? Bt.Hold : FMath::Max(Bt.Hold, Typed + 1.5f + Bt.Line.Len() * 0.03f);
	if (Bt.bWait)
	{
		const bool bAutoNext = bAuto && FParse::Param(FCommandLine::Get(), TEXT("brawlnext")) && CutT > 3.f;
		if (CutId == 10 || CutId == 11)   // a day: on to the drive / the night (DayCutDone), R watches it again
		{
			if ((bEnterPress && CutT > 0.6f) || bAutoNext) DayCutDone();
			else if (PC && PC->IsInputKeyDown(EKeys::R)) StartCut(CutId);
			return;
		}
		if ((bEnterPress && CutT > 0.6f) || bAutoNext)
		{
			UE_LOG(LogFTCut, Log, TEXT("FT BRAWL screen=endcard cut=%d result=on"), CutId);
			GoOn();   // the next night (the next race in the main game's career)
		}
		else if (PC && PC->IsInputKeyDown(EKeys::R)) { EndCut(); Restart(); }
		return;
	}
	if (bNextPress && !Bt.Line.IsEmpty() && CutLineT < Typed) { CutLineT = Typed; return; }   // first press: the whole line at once
	if (CutT >= Dur || (bNextPress && CutT > 0.25f)) NextBeat();
}

void AFTBrawlGameMode::SyncCut(float Dt)
{
	for (auto& Kv : CutCast)
	{
		FFTCutActor& C = Kv.Value;
		if (!C.A) continue;
		const bool bMoving = !C.bFly && (C.Goal - C.Pos).Size2D() > 3.f;
		C.A->SetActorLocationAndRotation(C.Pos, FRotator(0, C.Dir > 0 ? 0 : 180, 0));
		C.A->SetActorScale3D(FVector(C.Scale));
		const FName St = bMoving ? FName(TEXT("walk")) : C.Pose;
		const float Lean = C.Pose == TEXT("fly") ? C.Lean : C.Pose == TEXT("hurt") ? 14.f : 0.f;
		C.A->Pose(St, C.T, C.Walk, 1, Lean, 0.f, !C.bHidden);
	}
	SyncItems();
	if (Limo)
	{
		Limo->SetActorLocation(LimoAt + FVector(0, 0, Limo->GetActorLocation().Z - LimoAt.Z));
		if (TailL) { TailL->SetActorLocation(LimoAt + FVector(-380, 30, 90)); }
	}
	if (FlashL) FlashL->PointLightComponent->SetIntensity(CutFlash * 90000.f);
	if (FFTCutActor* Ph = CutCast.Find(TEXT("photo"))) if (FlashL) FlashL->SetActorLocation(Ph->Pos + FVector(-40, 60, 170));
	// blood / spray in flight
	if (DropIsm && GlassIsm)
	{
		DropIsm->ClearInstances(); GlassIsm->ClearInstances();
		TArray<FTransform> D0, G0;
		for (const FFTBrawlDrop& D : Drops) (D.bGlass ? G0 : D0).Add(FTransform(D.V.Rotation(), D.P, FVector(D.S)));
		if (D0.Num()) DropIsm->AddInstances(D0, false);
		if (G0.Num()) GlassIsm->AddInstances(G0, false);
	}
	// the camera eases to its mark
	const float A = FMath::Min(1.f, Dt * CutCamRate);
	CutCamLoc += (CutCamGoal - CutCamLoc) * A; CutPitch += (CutPitchGoal - CutPitch) * A; CutFov += (CutFovGoal - CutFov) * A;
	if (CamActor)
	{
		float Sx = 0, Sz = 0;
		if (Shake > 0) { const float Tr = Shake * Shake * 14.f; Sx = FMath::Sin(StT * 61) * Tr; Sz = FMath::Cos(StT * 53) * Tr; }
		CamActor->SetActorLocation(CutCamLoc + FVector(Sx, 0, Sz));
		CamActor->SetActorRotation(FRotator(CutPitch, -90.f, 0));
		CamActor->GetCameraComponent()->SetFieldOfView(CutFov);
	}
}

// ---------------------------------------------------------------- the set: the pier out back of the Rusty Anchor
// The bar's outside wall (Codex, ArtSource/night1, glow mask by tools/night1.py) around the back door at PierX, the
// plank deck, then open water to the right: the pier rail and lamps (T_out_near) just past the deck's end, the shore
// (T_out_far) at 3.2 x the depth; the sky and sea are the bar's own far layer, which runs all the way out here.
// Light: a cold moon spot, the caged bulb over the door, the pink anchor neon, a sodium lamp by the limousine.
void AFTBrawlGameMode::BuildPier()
{
	UWorld* Wd = GetWorld();
	auto Tex = [](const TCHAR* Path) { return LoadObject<UTexture>(nullptr, Path, nullptr, LOAD_Quiet | LOAD_NoWarn); };
	UMaterialInterface* PaintM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlPaint.M_BrawlPaint"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UMaterialInterface* SpriteM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlSprite.M_BrawlSprite"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UMaterialInterface* Wall2DM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlWall2D.M_BrawlWall2D"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UMaterialInterface* OutM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlOutside.M_BrawlOutside"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UStaticMesh* PlaneM = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"));
	UTexture* Floor = Tex(TEXT("/Game/Brawl/Backdrop/T_floor.T_floor"));
	UTexture* WallT = Tex(TEXT("/Game/Brawl/Night1/T_bg_anchor_out.T_bg_anchor_out"));
	UTexture* WallG = Tex(TEXT("/Game/Brawl/Night1/T_glow_anchor_out.T_glow_anchor_out"));
	UTexture* OutFar = Tex(TEXT("/Game/Brawl/Night/T_out_far.T_out_far")), *OutNear = Tex(TEXT("/Game/Brawl/Night/T_out_near.T_out_near"));
	UTexture* LimoT = Tex(TEXT("/Game/Brawl/Sprites/Props/T_car_limo.T_car_limo"));
	if (!PaintM || !SpriteM || !Wall2DM || !OutM || !PlaneM || !Floor || !WallT || !WallG || !OutFar || !OutNear)
	{
		UE_LOG(LogFTCut, Warning, TEXT("FTBRAWL no pier art (brawl_night1.py): the pier scene plays on a dark set"));
	}
	auto Quad = [&](FVector C, FVector2D Size, FRotator Rot, UMaterialInterface* M, UTexture* T, bool bShadow, TFunctionRef<void(UMaterialInstanceDynamic*)> Params) -> AStaticMeshActor*
	{
		if (!M || !T || !PlaneM) return nullptr;
		AStaticMeshActor* A = Wd->SpawnActor<AStaticMeshActor>(C, Rot);
		A->SetMobility(EComponentMobility::Movable);
		UStaticMeshComponent* SM = A->GetStaticMeshComponent();
		SM->SetStaticMesh(PlaneM); SM->SetWorldScale3D(FVector(Size.X / 100.f, Size.Y / 100.f, 1)); SM->SetCastShadow(bShadow);
		SM->bCastShadowAsTwoSided = true;
		SM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UMaterialInstanceDynamic* D = UMaterialInstanceDynamic::Create(M, A);
		D->SetTextureParameterValue(TEXT("Tex"), T);
		Params(D);
		SM->SetMaterial(0, D);
		return A;
	};
	auto Point = [&](FVector At, const TCHAR* C, float I, float R, bool bShadow) -> APointLight*
	{
		APointLight* L = Wd->SpawnActor<APointLight>(At, FRotator::ZeroRotator);
		L->PointLightComponent->SetMobility(EComponentMobility::Movable);
		L->PointLightComponent->SetIntensity(I); L->PointLightComponent->SetAttenuationRadius(R); L->PointLightComponent->SetCastShadows(bShadow);
		L->PointLightComponent->SetLightColor(Hx(C));
		return L;
	};
	auto Spot = [&](FVector At, FRotator Rot, const TCHAR* C, float I, float R, float In, float Out, bool bShadow)
	{
		ASpotLight* Sp = Wd->SpawnActor<ASpotLight>(At, Rot);
		USpotLightComponent* SC = Sp->SpotLightComponent;
		SC->SetMobility(EComponentMobility::Movable);
		Sp->SetActorRotation(Rot);
		SC->SetIntensity(I); SC->SetAttenuationRadius(R); SC->SetInnerConeAngle(In); SC->SetOuterConeAngle(Out);
		SC->SetLightColor(Hx(C)); SC->SetCastShadows(bShadow);
	};
	const FRotator Upright(0, 0, 90);
	const float X = PierX, Back = (560.f - 360.f) * CPX + 60.f, WallY = -Back, NearY = 700.f;
	const float X0 = X - 2600.f, X1 = X + 6000.f, Len = X1 - X0;
	// the deck
	Quad(FVector((X0 + X1) / 2, (WallY + NearY) / 2, 0), FVector2D(Len, NearY - WallY), FRotator::ZeroRotator, PaintM, Floor, false,
		[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("TileU"), Len / 380.f); D->SetScalarParameterValue(TEXT("TileV"), (NearY - WallY) / 380.f);
			D->SetScalarParameterValue(TEXT("Glow"), 0.03f); D->SetScalarParameterValue(TEXT("Rough"), 0.5f);
			D->SetVectorParameterValue(TEXT("Tint"), FLinearColor(0.78f, 0.82f, 0.95f)); });
	// the bar's outside wall: the back door at PierX (u 0.48 on the panel), a second panel to the left, mirrored
	const float PH = 560.f, PW = PH * 1.5f, Px0 = X - 0.48f * PW;
	for (int32 i = 0; i < 3; i++)
		Quad(FVector(Px0 + PW / 2 - i * PW, WallY, PH / 2), FVector2D(i % 2 ? -PW : PW, PH), Upright, Wall2DM, WallT, false,
			[&](UMaterialInstanceDynamic* D) { D->SetTextureParameterValue(TEXT("GlowTex"), WallG); });
	// past the wall's end: the pier rail and lamps right behind the deck, the shore farther out (same projection as the bar's windows)
	const float Dw = CCamY - WallY;
	auto Layer = [&](float K, UTexture* T, float Zw0, float Zw1, float TileW, float XA, float XB, float Bright)
	{
		const float Y = CCamY - K * Dw, Z0 = CCamZ + (Zw0 - CCamZ) * K, Z1 = CCamZ + (Zw1 - CCamZ) * K;
		Quad(FVector((XA + XB) / 2, Y, (Z0 + Z1) / 2), FVector2D(XB - XA, Z1 - Z0), Upright, OutM, T, false,
			[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("TileU"), (XB - XA) / (TileW * K));
				D->SetScalarParameterValue(TEXT("PanU"), 0.f); D->SetScalarParameterValue(TEXT("Bright"), Bright); });
	};
	const float Wend = Px0 + PW;   // the wall stops here
	Layer(3.2f, OutFar, 224.f, 424.f, 300.f, X - 2000.f, X + 14000.f, 0.6f);
	Layer(1.06f, OutNear, -5.f, 325.f, 420.f, Wend - 10.f, X1, 0.75f);
	// the limousine, at the deck's back edge, nose to the right (frame 1024 px, the car 248 px tall, wheels at px 1000)
	if (LimoT)
	{
		const float K = 190.f / 248.f, Sz = 1024.f * K;
		LimoAt = FVector(X + 1720, -318, 0);
		Limo = Quad(FVector(LimoAt.X, LimoAt.Y, (1000.f - 512.f) * K), FVector2D(Sz, Sz), Upright, SpriteM, LimoT, true,
			[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("SelfLit"), 0.06f); });
		if (Limo) Limo->SetActorHiddenInGame(true);
	}
	// light
	Spot(FVector(X + 700, 900, 1500), FRotator(-58, -90, 0), TEXT("8fa8ff"), 38000.f, 4200.f, 30.f, 60.f, true);   // the moon
	Point(FVector(X - 10, WallY + 70, 470), TEXT("ffb066"), 5200.f, 760.f, true);                                   // the caged bulb over the door
	Point(FVector(Px0 + 0.2f * PW, WallY + 110, PH * 0.66f), TEXT("ff4fb8"), 3600.f, 700.f, false);                // the anchor neon
	Point(FVector(Px0 + 0.78f * PW, WallY + 100, PH * 0.6f), TEXT("36f1ff"), 1600.f, 520.f, false);                // the beer sign in the window
	Spot(FVector(X + 1350, -120, 560), FRotator(-90, 0, 0), TEXT("ffc27a"), 52000.f, 1100.f, 18.f, 52.f, true);    // a pier lamp over the deal
	TailL = Point(FVector(X + 1340, -290, 90), TEXT("ff2a2a"), 1400.f, 300.f, false);
	FlashL = Point(FVector(X + 520, -30, 170), TEXT("ffffff"), 0.f, 1600.f, false);
	UE_LOG(LogFTCut, Log, TEXT("FTBRAWL pier built at x=%.0f (wall %d, limo %d)"), X, WallT ? 1 : 0, LimoT ? 1 : 0);
}

// ---------------------------------------------------------------- day 1's set: Route 9 on the bay, the film unit's morning call
// (run 73444) Two Codex panels (ArtSource/day1: the crew's base camp with the crane and trailer, the beach with the stunt
// ramp and the lifeguard tower) alternate along the back at y -340, the road in front (T_road, tools/day1.py: asphalt,
// a kerb at the back, the dashed centre line), the set's props on sprite quads (the camera dolly, Voss's chair, film lamps,
// apple boxes) and the KESTREL (on Limo, so the cutscene can drive it off). Light: a morning sun from the left, a bright
// sky fill, a warm film lamp on the actors' mark.
void AFTBrawlGameMode::BuildSet()
{
	UWorld* Wd = GetWorld();
	auto Tex = [](const TCHAR* Path) { return LoadObject<UTexture>(nullptr, Path, nullptr, LOAD_Quiet | LOAD_NoWarn); };
	UMaterialInterface* PaintM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlPaint.M_BrawlPaint"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UMaterialInterface* SpriteM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlSprite.M_BrawlSprite"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UStaticMesh* PlaneM = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"));
	UTexture* Pa = Tex(TEXT("/Game/Brawl/Day1/T_bg_set_a.T_bg_set_a")), *Pb = Tex(TEXT("/Game/Brawl/Day1/T_bg_set_b.T_bg_set_b"));
	UTexture* Road = Tex(TEXT("/Game/Brawl/Day1/T_road.T_road"));
	UTexture* CarT = Tex(TEXT("/Game/Brawl/Sprites/Props/T_car_kestrel.T_car_kestrel"));
	if (!PaintM || !SpriteM || !PlaneM || !Pa || !Pb || !Road || !CarT)
		UE_LOG(LogFTCut, Warning, TEXT("FTBRAWL no set art (brawl_day1.py): day 1 plays on a bare set"));
	auto Quad = [&](FVector C, FVector2D Size, FRotator Rot, UMaterialInterface* M, UTexture* T, bool bShadow, TFunctionRef<void(UMaterialInstanceDynamic*)> Params) -> AStaticMeshActor*
	{
		if (!M || !T || !PlaneM) return nullptr;
		AStaticMeshActor* A = Wd->SpawnActor<AStaticMeshActor>(C, Rot);
		A->SetMobility(EComponentMobility::Movable);
		UStaticMeshComponent* SM = A->GetStaticMeshComponent();
		SM->SetStaticMesh(PlaneM); SM->SetWorldScale3D(FVector(Size.X / 100.f, Size.Y / 100.f, 1)); SM->SetCastShadow(bShadow);
		SM->bCastShadowAsTwoSided = true;
		SM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UMaterialInstanceDynamic* D = UMaterialInstanceDynamic::Create(M, A);
		D->SetTextureParameterValue(TEXT("Tex"), T);
		Params(D);
		SM->SetMaterial(0, D);
		return A;
	};
	const FRotator Upright(0, 0, 90);
	const float X = SetX, WallY = -((560.f - 360.f) * CPX + 60.f), NearY = 900.f;
	const float X0 = X - 4000.f, X1 = X + 8000.f, Len = X1 - X0;
	// the road
	Quad(FVector((X0 + X1) / 2, (WallY + NearY) / 2, 0), FVector2D(Len, NearY - WallY), FRotator::ZeroRotator, PaintM, Road, false,
		[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("TileU"), Len / (NearY - WallY)); D->SetScalarParameterValue(TEXT("TileV"), -1.f);   // -1: the kerb (top of the image) at the back
			D->SetScalarParameterValue(TEXT("Glow"), 0.12f); D->SetScalarParameterValue(TEXT("Rough"), 0.8f); });
	// the backdrop: panel A (base camp) behind the actors' mark, B (the beach) either side, alternating
	const float PH = 620.f, PW = PH * 1.5f;
	for (int32 i = -5; i <= 9; i++)
		Quad(FVector(X - 0.45f * PW + PW / 2 + i * PW, WallY, PH / 2 - 8.f), FVector2D(PW, PH), Upright, PaintM, (i % 2 == 0) ? Pa : Pb, false,
			[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("Glow"), 0.35f); });
	// props on the set (frames 1024 px, standing on px 1000; Px = the object's height in the frame)
	auto Prop = [&](const TCHAR* Name, float Px, float HeightCm, FVector At, bool bFlip)
	{
		UTexture* T = Tex(*FString::Printf(TEXT("/Game/Brawl/Sprites/Props/T_prop_%s.T_prop_%s"), Name, Name));
		const float K = HeightCm / Px, Sz = 1024.f * K;
		Quad(FVector(At.X, At.Y, (1000.f - 512.f) * K), FVector2D(bFlip ? -Sz : Sz, Sz), Upright, SpriteM, T, true,
			[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("SelfLit"), 0.25f); });
	};
	Prop(TEXT("camera"), 557.f, 200.f, FVector(X - 780, -250, 0), false);
	Prop(TEXT("chair"), 405.f, 105.f, FVector(X - 560, -300, 0), false);
	Prop(TEXT("lamp"), 750.f, 300.f, FVector(X - 1150, -300, 0), false);
	Prop(TEXT("lamp"), 750.f, 300.f, FVector(X + 1150, -300, 0), true);
	Prop(TEXT("boxes"), 399.f, 110.f, FVector(X + 900, -290, 0), false);
	// the KESTREL (car 289 px tall in its frame, wheels on px 1000): 125 cm tall, nose to the right
	if (CarT)
	{
		const float K = 125.f / 289.f, Sz = 1024.f * K;
		LimoAt = FVector(X + 520, -300, 0);
		Limo = Quad(FVector(LimoAt.X, LimoAt.Y, (1000.f - 512.f) * K), FVector2D(Sz, Sz), Upright, SpriteM, CarT, true,
			[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("SelfLit"), 0.2f); });
	}
	// light: the morning sun from the left over the camera, the sky bright, a film lamp on the mark
	ADirectionalLight* Sun = Wd->SpawnActor<ADirectionalLight>(FVector(X, 0, 800), FRotator::ZeroRotator);
	SetSun = Sun;   // the days (run 77694): SetSetLight turns it to the golden hour for THE WRAP
	UDirectionalLightComponent* DL = CastChecked<UDirectionalLightComponent>(Sun->GetLightComponent());
	DL->SetMobility(EComponentMobility::Movable);
	Sun->SetActorRotation(FRotator(-34, -55, 0));
	DL->SetIntensity(5.f); DL->SetLightColor(Hx(TEXT("ffe6c8")));
	DL->SetDynamicShadowDistanceMovableLight(5000.f); DL->SetDynamicShadowCascades(1);
	for (TActorIterator<ASkyLight> It(Wd); It; ++It)
	{
		USkyLightComponent* SL = It->GetLightComponent();
		SL->SetIntensity(1.2f); SL->SetLightColor(Hx(TEXT("c9b8e8"))); SL->RecaptureSky();
	}
	ASpotLight* Sp = Wd->SpawnActor<ASpotLight>(FVector(X - 1100, 300, 420), FRotator::ZeroRotator);
	SetLamp = Sp;
	Sp->SpotLightComponent->SetMobility(EComponentMobility::Movable);
	Sp->SetActorRotation((FVector(X, -170, 120) - FVector(X - 1100, 300, 420)).Rotation());
	Sp->SpotLightComponent->SetIntensity(30000.f); Sp->SpotLightComponent->SetAttenuationRadius(2600.f);
	Sp->SpotLightComponent->SetInnerConeAngle(16.f); Sp->SpotLightComponent->SetOuterConeAngle(34.f);
	Sp->SpotLightComponent->SetLightColor(Hx(TEXT("fff2d6"))); Sp->SpotLightComponent->SetCastShadows(false);
	UE_LOG(LogFTCut, Log, TEXT("FTBRAWL set built at x=%.0f (panels %d, road %d, kestrel %d)"), X, Pa && Pb ? 1 : 0, Road ? 1 : 0, CarT ? 1 : 0);
}

// ---------------------------------------------------------------- the days (run 77694): BEFORE THE DRIVE (10) and THE WRAP (11)
// On the Route 9 set: the crew who speak stand on their marks (Voss by his chair, then left to right), Nova's mark is in
// the middle (SetX + 60), the KESTREL parked behind him. BEFORE: Nova walks in, the lines, he gets in the car and it pulls
// away (ROLLING), then the THE DRIVE card. THE WRAP: the golden hour, Nova gets out of the car, the lines, he walks off
// to the night, then the END OF DAY card.
void AFTBrawlGameMode::StartDayCut(int32 Id, TFunctionRef<void(const TCHAR*, const FString&, float, TFunction<void()>)> B)
{
	const int32 D = FMath::Clamp(DayN, 1, 8) - 1;
	const FDayCut& C = DayCuts[D];
	const bool bWrap = Id == 11;
	const FDayLine* Lines = bWrap ? C.Wrap : C.Before;
	const int32 NL = bWrap ? 5 : 6;
	const float X = SetX, NX = SetX + 60.f;
	CutFade = 1; CutFadeGoal = 0;
	// the crew: everybody who speaks (Voss first, by his chair)
	TArray<FName> Crew;
	for (int32 i = 0; i < NL && Lines[i].Who; i++)
		if (FCString::Strcmp(Lines[i].Who, TEXT("nova")) != 0) Crew.AddUnique(FName(Lines[i].Who));
	if (Crew.Remove(TEXT("voss"))) Crew.Insert(TEXT("voss"), 0);
	const float MarkX[] = { X - 330.f, X - 110.f, X + 300.f }, MarkY[] = { -190.f, -160.f, -175.f };
	TMap<FName, float> At;
	At.Add(TEXT("nova"), NX);
	for (int32 i = 0; i < Crew.Num() && i < 3; i++)
	{
		const FDayActor* A = nullptr;
		for (const FDayActor& Da : DayActors) if (Crew[i] == Da.Key) A = &Da;
		if (!A) continue;
		AddCast(Crew[i], A->Sprite, A->H, 1.f, FVector(MarkX[i], MarkY[i], 0), MarkX[i] < NX ? 1 : -1);
		At.Add(Crew[i], MarkX[i]);
	}
	if (bWrap) AddCast(TEXT("nova"), TEXT("Nova"), 228.f, 1.f, FVector(X + 380, -235, 0), -1).bHidden = true;   // in the car
	else AddCast(TEXT("nova"), TEXT("Nova"), 228.f, 1.f, FVector(X + 1250, -150, 0), -1);                    // from the right, past his car
	if (Limo) { LimoAt = FVector(X + 520, -300, 0); LimoV = 0; Limo->SetActorHiddenInGame(false); }
	CamTo(FVector(X - 200, 1450, 540), -16, 50, 1.f, true);
	const FString Title = bWrap ? FString::Printf(TEXT("DAY %d · THE WRAP"), D + 1) : FString(C.Title);
	const FString Sub = bWrap ? FString::Printf(TEXT("%s · SCENE %s IS IN THE CAN"), C.Place, SceneWord[D]) : FString::Printf(TEXT("%s · ON SET"), C.Place);
	B(TEXT(""), TEXT(""), 2.8f, [this, Title, Sub, NX, X, bWrap]() {
		CutTitle = Title; CutSub = Sub; CutTitleT = 0; CutFadeGoal = 0.35f;   // the title over a dimmed set, as day 1
		FFTCutActor& N = CutCast[TEXT("nova")]; N.bHidden = false;
		Walk(TEXT("nova"), FVector(NX, -170, 0), bWrap ? 150 : 300);
		CamTo(FVector(X + 100, 1250, 480), -15, 48, 0.7f); });
	// the lines: the speaker in the line's pose, facing Nova; Nova faces whoever spoke last; a two-shot of the pair
	FName Prev = NAME_None, Other = Crew.Num() ? Crew[0] : FName(NAME_None);
	for (int32 i = 0; i < NL && Lines[i].Who; i++)
	{
		const FName Key = Lines[i].Who;
		const FName Pose = Lines[i].Pose ? FName(Lines[i].Pose) : FName(TEXT("idle"));
		const bool bNova = Key == TEXT("nova");
		if (!bNova) Other = Key;
		const float OX = At.Contains(Other) ? At[Other] : X - 330.f;
		const float Mx = (OX + NX) / 2, Far = FMath::Abs(OX - NX) > 300.f ? 900.f : 800.f;
		const FName Was = Prev;
		B(Lines[i].Who, Lines[i].Line, 0.f, [this, Key, Pose, bNova, Was, OX, NX, Mx, Far]() {
			CutFadeGoal = 0;
			if (Was != NAME_None && Was != Key) PoseAs(Was, TEXT("idle"));
			const float ToNova = OX < NX ? 1 : -1;
			if (bNova) { PoseAs(TEXT("nova"), Pose, -ToNova); CamTo(FVector(NX - 20, 760, 320), -9, 40, 1.2f); }
			else { PoseAs(Key, Pose, ToNova); PoseAs(TEXT("nova"), TEXT("idle"), -ToNova); CamTo(FVector(Mx, Far, 330), -9, 42, 1.1f); } });
		Prev = Key;
	}
	if (!bWrap)
	{
		// Nova gets in the KESTREL, the car pulls away to the right: ROLLING
		const bool bVoss = Crew.Contains(TEXT("voss"));
		B(TEXT(""), TEXT(""), 1.5f, [this, X, Prev, bVoss]() {
			if (Prev == TEXT("nova")) PoseAs(TEXT("nova"), TEXT("idle"));
			Walk(TEXT("nova"), FVector(X + 380, -235, 0), 200);
			if (bVoss) PoseAs(TEXT("voss"), TEXT("shout"), 1);
			CamTo(FVector(X + 100, 1250, 470), -15, 48, 1.0f); });
		const FString Roll = FString::Printf(TEXT("SCENE %s · TAKE ONE"), SceneWord[D]);
		B(TEXT(""), TEXT(""), 3.4f, [this, X, Roll]() {
			CutCast[TEXT("nova")].bHidden = true; LimoV = 40.f; Shake = 0.35f;
			CutTitle = TEXT("ROLLING"); CutSub = Roll; CutTitleT = 0;
			CamTo(FVector(X + 900, 1350, 500), -16, 50, 0.9f); UE_LOG(LogFTCut, Log, TEXT("FTBRAWL cut kestrel away")); });
		B(TEXT(""), TEXT(""), 1.2f, [this]() { CutFadeGoal = 0.85f; });
		FFTCutBeat End; End.bWait = true;
		const FString Drive = C.Drive;
		End.Start = [this, Drive, D]() {
			CutCard = TEXT("▶ THE DRIVE");
			CutCardSub = FString::Printf(TEXT("%s, IN FULL THROTTLE  →  THEN DAY %d · THE WRAP"), *Drive, D + 1);
			CutCardKeys = FString::Printf(TEXT("ENTER/A: ON TO DAY %d · THE WRAP   ·   R: WATCH AGAIN"), D + 1);
#if FTB_CAREER_DAY1
			if (UFTNightFlow::InDay1(this)) CutCardKeys = FString::Printf(TEXT("ENTER/A: ON TO THE DRIVE · %s   ·   R: WATCH AGAIN"), *Drive);
#endif
			UE_LOG(LogFTCut, Log, TEXT("FTBRAWL cut end card day %d before"), D + 1);
		};
		Beats.Add(MoveTemp(End));
		return;
	}
	// THE WRAP: Nova walks off to the right, into the night
	B(TEXT(""), TEXT(""), 2.2f, [this, X, Prev]() {
		if (Prev == TEXT("nova")) PoseAs(TEXT("nova"), TEXT("idle"));
		Walk(TEXT("nova"), FVector(X + 1500, -150, 0), 200); CamTo(FVector(X + 300, 1250, 470), -15, 48, 0.8f); CutFadeGoal = 1; });
	B(TEXT(""), TEXT(""), 0.6f, [this]() { CutFadeGoal = 0.85f; });
	FFTCutBeat End; End.bWait = true;
	const FString NightName = C.NightName;
	End.Start = [this, NightName, D]() {
		CutCard = FString::Printf(TEXT("END OF DAY %d"), D + 1);
		if (NightName.IsEmpty())
		{
			CutCardSub = TEXT("THAT’S A WRAP ON THE PICTURE");
			CutCardKeys = TEXT("ENTER/A: BACK TO DAY 1   ·   R: WATCH AGAIN");
#if FTB_CAREER_DAY1
			if (UFTNightFlow::InDay1(this)) CutCardKeys = TEXT("ENTER/A: ON   ·   R: WATCH AGAIN");
#endif
		}
		else
		{
			CutCardSub = FString::Printf(TEXT("NEXT: %s"), *NightName);
			CutCardKeys = FString::Printf(TEXT("ENTER/A: ON TO %s   ·   R: WATCH AGAIN"), *NightName);
		}
		UE_LOG(LogFTCut, Log, TEXT("FTBRAWL cut end card day %d wrap"), D + 1);
	};
	Beats.Add(MoveTemp(End));
}

// ENTER on a day's end card: in the main game's career FTNightFlow takes it from here (the race after BEFORE THE DRIVE,
// the night after THE WRAP); on its own the brawl plays the day's WRAP after its BEFORE, then that day's night
void AFTBrawlGameMode::DayCutDone()
{
#if FTB_CAREER_DAY1
	if (UFTNightFlow::DayCutOver(this)) { UE_LOG(LogFTCut, Log, TEXT("FTBRAWL day %d %s over: back to the career"), DayN, CutId == 11 ? TEXT("wrap") : TEXT("before")); return; }
#endif
	if (CutId == 10)
	{
		UE_LOG(LogFTCut, Log, TEXT("FTBRAWL on to day %d wrap"), DayN);
		bDayWrap = true; SetSetLight(true); StartCut(11);
		return;
	}
	const TCHAR* Next = NightMap(DayN);
	if (DayN <= 7 && Next && FPackageName::DoesPackageExist(Next))
	{
		UE_LOG(LogFTCut, Log, TEXT("FTBRAWL on to night %d"), DayN);
		UGameplayStatics::OpenLevel(this, FName(Next));
	}
	else
	{
		UE_LOG(LogFTCut, Log, TEXT("FTBRAWL back to day 1"));
		UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/Brawl/Maps/L_Day1Set")), true, TEXT("brawlday=1"));
	}
}

// the set's light: the morning call (BEFORE THE DRIVE) or the golden hour (THE WRAP)
void AFTBrawlGameMode::SetSetLight(bool bDusk)
{
	if (SetSun)
	{
		SetSun->SetActorRotation(bDusk ? FRotator(-14, -35, 0) : FRotator(-34, -55, 0));
		UDirectionalLightComponent* DL = CastChecked<UDirectionalLightComponent>(SetSun->GetLightComponent());
		DL->SetIntensity(bDusk ? 4.2f : 5.f); DL->SetLightColor(Hx(bDusk ? TEXT("ffa860") : TEXT("ffe6c8")));
	}
	if (SetLamp) SetLamp->SpotLightComponent->SetLightColor(Hx(bDusk ? TEXT("ffd2a0") : TEXT("fff2d6")));
	for (TActorIterator<ASkyLight> It(GetWorld()); It; ++It)
	{
		USkyLightComponent* SL = It->GetLightComponent();
		SL->SetIntensity(bDusk ? 1.0f : 1.2f); SL->SetLightColor(Hx(bDusk ? TEXT("e8a8b8") : TEXT("c9b8e8"))); SL->RecaptureSky();
	}
	UE_LOG(LogFTCut, Log, TEXT("FTBRAWL set light %s"), bDusk ? TEXT("golden hour") : TEXT("morning"));
}
