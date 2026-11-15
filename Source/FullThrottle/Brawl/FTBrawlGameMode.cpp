#include "FTBrawlGameMode.h"
#include "FTBrawlHUD.h"
#include "FTBrawlPlayerController.h"
#include "FTBrawlMusic.h"
#if __has_include("FTGameInstance.h")   // the main game only (the standalone beat-em-up has no settings of its own)
#include "FTGameInstance.h"
#define FT_BRAWL_MAIN_GAME 1
#endif
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/PointLight.h"
#include "Engine/SpotLight.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/Texture.h"
#include "Engine/TextRenderActor.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "HAL/IConsoleManager.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealClient.h"
#include "Misc/PackageName.h"
#include "GameFramework/PlayerController.h"
#include "InputKeyEventArgs.h"

#if __has_include("FTNightFlow.h")   // the main game (full-throttle-unreal): the nights between the races; the same file builds in both projects
#include "FTNightFlow.h"
#define FTB_CAREER 1
#else
#define FTB_CAREER 0
#endif

DEFINE_LOG_CATEGORY_STATIC(LogFTBrawl, Log, All);
#include "FTBrawlNightArt.inl"   // nights 4-7: the lamps on the painted panels and the props' sizes (written by ArtSource/tools/n47.py)

namespace FTBrawl
{
	static const float STEP = 1.f / 60.f;
	static const float PX = 1.4f;             // one brawl.js pixel in cm
	static const float W = 960.f;             // the web game's screen width (px)
	static const float FT = 360.f, FB = 560.f; // the floor band (feet y)
	static const float P_HP = 150.f;
	static const float CamZ = 540.f;
	static const float KeepIn = 70.f;         // a foe on the locked screen keeps his middle this far inside its edges (the camera shows ~Cam+30..Cam+930 at chest height on the near line)          // camera height (cm): side-on, a little above the floor band
	static TAutoConsoleVariable<int32> CVarAuto(TEXT("ftb.Auto"), 0, TEXT("Brawl autopilot: 1 = Nova fights by himself (tests)"));
	static TAutoConsoleVariable<int32> CVarWin(TEXT("ftb.Win"), 0, TEXT("1 = win the fight now (tests)"));
	static FLinearColor Hex(const TCHAR* H) { return FLinearColor(FColor::FromHex(H)); }
}
using namespace FTBrawl;

namespace FTBrawl
{
	enum { IT_BEER, IT_BOTTLE, IT_CUE, IT_STOOL, IT_PAN, IT_HOTDOG, IT_KNUCKS, IT_JACKET,
		IT_COCKTAIL, IT_MIC, IT_BUCKET, IT_CHAMP, IT_VSTOOL,   // night 2, the Velvet Room (run 16961; ArtSource/sheets/items_v, vprops)
		IT_WHISKEY, IT_RAKE, IT_WBOTTLE, IT_CASHBOX, IT_CSTOOL, // night 3, the Lucky 7 (run 52616; ArtSource/sheets/items_c, cprops)
		IT_NUM };
	enum { TY_BEER, TY_WEAPON, TY_FOOD, TY_POWER };
	// Px = the object's height in its 1024 px frame (bottom at px 1000), Cm = its height in the bar.
	// Weapons: damage, reach (brawl px: run 38222 measured it from the art, Tools/weapon_reach.py = where the far end is at the end
	// of the swing + 16, as a kick reaches past the boot; a pool cue hits from its tip), knocks down, swings before it breaks; the grip pixel, the image angle from the grip
	// to the far end (degrees, 0 = right, 90 = up) and the angle it is carried at (0 = forward, 90 = up).
	struct FItemDef { const TCHAR* Name; const TCHAR* Tex; float Px, Cm; int32 Type; float Dmg, Reach; bool bDown; int32 Uses; float Gu, Gv, Tip, Carry; };
	static const FItemDef GItems[IT_NUM] = {
		{ TEXT("BEER"), TEXT("/Game/Brawl/Sprites/Items/T_item_beer.T_item_beer"), 474, 40, TY_BEER, 0, 0, false, 0, 640, 760, 90, 90 },
		{ TEXT("BOTTLE"), TEXT("/Game/Brawl/Sprites/Items/T_item_bottle.T_item_bottle"), 608, 46, TY_WEAPON, 11, 89, false, 3, 511, 420, -90, -95 },
		{ TEXT("POOL CUE"), TEXT("/Game/Brawl/Sprites/Items/T_item_cue.T_item_cue"), 896, 165, TY_WEAPON, 10, 166, false, 6, 511, 930, 90, 70 },
		{ TEXT("BAR STOOL"), TEXT("/Game/Brawl/Sprites/Props/T_prop_stool.T_prop_stool"), 361, 92, TY_WEAPON, 18, 120, true, 4, 418, 985, 90, -80 },
		{ TEXT("FRYING PAN"), TEXT("/Game/Brawl/Sprites/Items/T_item_pan.T_item_pan"), 540, 58, TY_WEAPON, 15, 101, true, 5, 700, 480, -126.6f, -60 },
		{ TEXT("CHILI DOG"), TEXT("/Game/Brawl/Sprites/Items/T_item_hotdog.T_item_hotdog"), 381, 40, TY_FOOD, 0, 0, false, 0, 512, 1000, 90, 0 },
		{ TEXT("BRASS KNUCKLES"), TEXT("/Game/Brawl/Sprites/Items/T_item_knucks.T_item_knucks"), 280, 30, TY_POWER, 0, 0, false, 0, 512, 1000, 90, 0 },
		{ TEXT("LEATHER JACKET"), TEXT("/Game/Brawl/Sprites/Items/T_item_jacket.T_item_jacket"), 421, 48, TY_POWER, 0, 0, false, 0, 512, 1000, 90, 0 },
		{ TEXT("COCKTAIL"), TEXT("/Game/Brawl/Sprites/Items/T_item_cocktail.T_item_cocktail"), 651, 40, TY_BEER, 0, 0, false, 0, 512, 820, 90, 90 },
		{ TEXT("MIC STAND"), TEXT("/Game/Brawl/Sprites/Items/T_item_mic.T_item_mic"), 889, 165, TY_WEAPON, 10, 162, false, 6, 512, 900, 90, 70 },
		{ TEXT("ICE BUCKET"), TEXT("/Game/Brawl/Sprites/Items/T_item_bucket.T_item_bucket"), 728, 55, TY_WEAPON, 15, 84, true, 5, 296, 600, 0, 0 },
		{ TEXT("CHAMPAGNE"), TEXT("/Game/Brawl/Sprites/Items/T_item_champ.T_item_champ"), 747, 52, TY_WEAPON, 12, 92, false, 3, 512, 300, -90, -95 },
		{ TEXT("BAR STOOL"), TEXT("/Game/Brawl/Sprites/Props/T_prop_vstool.T_prop_vstool"), 328, 90, TY_WEAPON, 18, 120, true, 4, 440, 990, 90, -80 },
		{ TEXT("WHISKEY"), TEXT("/Game/Brawl/Sprites/Items/T_item_whiskey.T_item_whiskey"), 397, 30, TY_BEER, 0, 0, false, 0, 512, 820, 90, 90 },
		{ TEXT("CROUPIER'S RAKE"), TEXT("/Game/Brawl/Sprites/Items/T_item_rake.T_item_rake"), 905, 155, TY_WEAPON, 10, 160, false, 6, 512, 930, 90, 70 },
		{ TEXT("WHISKEY BOTTLE"), TEXT("/Game/Brawl/Sprites/Items/T_item_wbottle.T_item_wbottle"), 638, 46, TY_WEAPON, 11, 88, false, 3, 511, 410, -90, -95 },
		{ TEXT("CASH BOX"), TEXT("/Game/Brawl/Sprites/Items/T_item_cashbox.T_item_cashbox"), 341, 42, TY_WEAPON, 15, 89, true, 5, 512, 672, -90, -90 },
		{ TEXT("CASINO STOOL"), TEXT("/Game/Brawl/Sprites/Props/T_prop_cstool.T_prop_cstool"), 428, 100, TY_WEAPON, 18, 127, true, 4, 512, 990, 90, -80 } };
	static bool IsGlass(int32 K) { return K == IT_BOTTLE || K == IT_CHAMP || K == IT_WBOTTLE; }
	static const float NovaCmPerPx = 228.f / 600.f;   // Nova's sprite scale (SpawnFighter)
	static const float DropLife = 16.f, PowerTime = 12.f, BuzzTime = 10.f;
}

AFTBrawlGameMode::AFTBrawlGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;
	HUDClass = AFTBrawlHUD::StaticClass();
	PlayerControllerClass = AFTBrawlPlayerController::StaticClass();
	DefaultPawnClass = nullptr;
	// KINDS in brawl.js: hp, walk speed, punch damage, wind-up, reach, size
	// the four kinds of trouble at the Anchor (hp, walk speed, damage, wind-up, reach, size, role, model, height)
	Kinds.Add(TEXT("rusher"), { 18, 165, 6, 0.28f, 60, 2.0f, TEXT("rusher"), TEXT("Punk"), 172 });      // runs in, shoulder charge, quick jabs
	Kinds.Add(TEXT("grappler"), { 36, 120, 5, 0.32f, 56, 2.1f, TEXT("grappler"), TEXT("Biker"), 186 }); // grabs you and knees you till you mash free
	Kinds.Add(TEXT("thrower"), { 16, 130, 5, 0.3f, 58, 2.0f, TEXT("thrower"), TEXT("Sleaze"), 176 });  // hangs back and lobs beer bottles
	Kinds.Add(TEXT("heavy"), { 72, 82, 16, 0.55f, 80, 2.3f, TEXT("heavy"), TEXT("Bouncer"), 200 });    // shrugs off jabs, his hook puts you down
	Kinds.Add(TEXT("guard"), { 32, 140, 9, 0.42f, 66, 2.05f, TEXT("guard"), TEXT("Guard"), 182 });    // night 2's security: quick, every other swing the nightstick (knocks down)
	Kinds.Add(TEXT("dealer"), { 22, 125, 5, 0.3f, 58, 2.0f, TEXT("dealer"), TEXT("Dealer"), 178 });   // night 3: lines up on Nova's lane and flicks playing cards along it (jump them or step off)
	Waves = {
		{ TEXT("rusher"), TEXT("rusher"), TEXT("thrower"), TEXT("rusher") },
		{ TEXT("grappler"), TEXT("rusher"), TEXT("thrower"), TEXT("rusher"), TEXT("grappler") },
		{ TEXT("heavy"), TEXT("rusher"), TEXT("thrower"), TEXT("grappler"), TEXT("rusher"), TEXT("thrower"), TEXT("rusher") } };
	NumWaves = Waves.Num();
	BossK = { 210, 105, 15, 0.42f, 90, 2.7f, TEXT("boss"), TEXT("Earl"), 208 };
	BossRush = 18;
}

FVector AFTBrawlGameMode::World(float X, float Y, float Z) const
{
	return FVector(X * PX, -(FB - Y) * PX, Z * PX);   // back wall at -Y, camera at +Y looking -Y (so +X runs left to right)
}

int32 AFTBrawlGameMode::BossIdx() const
{
	for (int32 i = 0; i < Foes.Num(); i++) if (Foes[i].bBoss) return i;
	return INDEX_NONE;
}

// which night this map is: L_VelvetRoom (or -brawlnight=2) = night 2, THE VELVET ROOM, from the story flow
// (~/Wisper/docs/full-throttle-flow: 'A DISCO ON SUNSET · NO SNEAKERS', boss VINNIE VELOUR, DISCO WHIRLWIND, "SECURITY!")
const TCHAR* AFTBrawlGameMode::NightMap(int32 N)
{
	static const TCHAR* Maps[] = { TEXT("/Game/Brawl/Maps/L_RustyAnchor"), TEXT("/Game/Brawl/Maps/L_VelvetRoom"), TEXT("/Game/Brawl/Maps/L_Lucky7"),
		TEXT("/Game/Brawl/Maps/L_TikiTorch"), TEXT("/Game/Brawl/Maps/L_StrikeCity"), TEXT("/Game/Brawl/Maps/L_FrostbiteLodge"), TEXT("/Game/Brawl/Maps/L_SeaBiscuit") };
	return N >= 1 && N <= 7 ? Maps[N - 1] : nullptr;
}

void AFTBrawlGameMode::SetupNight()
{
	const FString Map = GetWorld() ? GetWorld()->GetMapName() : FString();
	int32 N = 1;
	for (int32 i = 7; i >= 1; i--) if (Map.Contains(FPackageName::GetShortName(NightMap(i)).Mid(2))) { N = i; break; }   // L_<Name>: the night of that map
	FParse::Value(FCommandLine::Get(), TEXT("brawlnight="), N);
	Night = FMath::Clamp(N, 1, 7);
	{
		static const TCHAR* Names[] = { TEXT("THE RUSTY ANCHOR"), TEXT("THE VELVET ROOM"), TEXT("THE LUCKY 7"), TEXT("THE TIKI TORCH"), TEXT("STRIKE CITY"), TEXT("THE FROSTBITE LODGE"), TEXT("THE SEA BISCUIT") };
		NextVenue = Night < 7 ? Names[Night] : TEXT("");
	}
	if (IsN47())
	{
		// nights 4-7 from the story flow (prototypes/brawl/mode.js VENUES): the story's waves as roles (tough = rusher,
		// drunk = thrower, bouncer = heavy, guard = guard), played by each night's own regulars (GNightFoes, run 97578), one grappler more in each
		// like nights 1 and 2; the boss's numbers a little over the web game's, like nights 2 and 3
		struct FN { const TCHAR *Name, *Tag, *Boss, *Title, *Call, *Who; float Hp, Sp, Dmg, Wind, Reach, Ht; uint32 Neon; int32 Seed; };
		static const FN Ns[4] = {
			{ TEXT("THE TIKI TORCH"), TEXT("A LUAU LOUNGE BELOW MOUNT KALDERA · FIRE SHOW AT TEN"), TEXT("FLAMBÉ FRANKIE"), TEXT("THE FIRE-EATER · TWENTY YEARS, NEVER BURNED"),
				TEXT("FRANKIE: \"VOLUNTEERS FOR THE FIRE SHOW!\""), TEXT("Frankie"), 240, 100, 14, 0.5f, 92, 204, 0xff8a3dff, 41 },
			{ TEXT("STRIKE CITY"), TEXT("A BOWLING ALLEY OFF THE BOARDWALK · LEAGUE NIGHT"), TEXT("THE KINGPIN"), TEXT("OWNS STRIKE CITY · LEAGUE CHAMP, 11 YEARS"),
				TEXT("KINGPIN: \"LEAGUE! LANE TWELVE! NOW!\""), TEXT("Kingpin"), 240, 110, 14, 0.45f, 88, 196, 0x36f1ffff, 53 },
			{ TEXT("THE FROSTBITE LODGE"), TEXT("AN APRÈS-SKI BAR ON FROSTBITE PEAK · HOT COCOA $1"), TEXT("BIG SVEN"), TEXT("RUNS THE LODGE · HOCKEY ENFORCER, 312 PENALTY MINUTES"),
				TEXT("SVEN: \"LINEMATES! ON THE ICE!\""), TEXT("Sven"), 260, 95, 15, 0.5f, 86, 214, 0xffd23fff, 67 },
			{ TEXT("THE SEA BISCUIT"), TEXT("THE COMMODORE’S YACHT · BLACK TIE · NO STUNTMEN"), TEXT("THE COMMODORE"), TEXT("OWNS THE YACHT · AND THE PICTURE"),
				TEXT("COMMODORE: \"CREW! ALL HANDS!\""), TEXT("Commodore"), 280, 95, 15, 0.5f, 112, 196, 0x36f1ffff, 79 } };
		const FN& V = Ns[Night - 4];
		VenueName = V.Name; VenueTag = V.Tag; BossName = V.Boss; BossTitle = V.Title; BossCall = V.Call;
		Neon = FLinearColor(FColor((V.Neon >> 24) & 255, (V.Neon >> 16) & 255, (V.Neon >> 8) & 255));
		const FName R = TEXT("rusher"), G = TEXT("grappler"), T = TEXT("thrower"), H = TEXT("heavy"), Gd = TEXT("guard");
		if (Night == 4) Waves = { { T, T, R, G }, { R, T, H, R, G }, { H, R, T, R, G, T } };          // drunk drunk tough / tough drunk bouncer / bouncer tough drunk tough
		else if (Night == 5) Waves = { { R, T, R, G }, { T, R, T, R, G }, { H, R, T, H, R } };         // tough drunk tough / drunk tough drunk tough / bouncer tough drunk bouncer
		else if (Night == 6) Waves = { { R, R, T, G }, { H, R, T, R, G }, { H, H, R, T, G } };         // tough tough drunk / bouncer tough drunk tough / bouncer bouncer tough drunk
		else Waves = { { Gd, T, Gd, R }, { Gd, H, T, Gd, R }, { H, Gd, H, R, Gd } };                   // guard drunk guard / guard bouncer drunk guard / bouncer guard bouncer tough
		NumWaves = Waves.Num();
		BossK = { V.Hp, V.Sp, V.Dmg, V.Wind, V.Reach, 2.6f, TEXT("boss"), V.Who, V.Ht };
		BossRush = 16;
		Rnd.Initialize(1986 + V.Seed);
		return;
	}
	int32 DayArg = 0;
	FParse::Value(FCommandLine::Get(), TEXT("brawlday="), DayArg);   // which day on L_Day1Set (the night maps stay nights)
	bDay = (GetWorld() && GetWorld()->GetMapName().Contains(TEXT("Day1"))) || FParse::Param(FCommandLine::Get(), TEXT("brawlday"));
	if (bDay)
	{
		// which day (run 77694): the career's (FTNightFlow), else the map's ?brawlday=N option (the brawl on its own goes
		// day -> night -> next day), else -brawlday=N; -brawlwrap / ?wrap = THE WRAP after the drive
		DayN = 1; bDayWrap = FParse::Param(FCommandLine::Get(), TEXT("brawlwrap"));
		if (DayArg > 0) DayN = DayArg;
		if (const FString Opt = UGameplayStatics::ParseOption(OptionsString, TEXT("brawlday")); !Opt.IsEmpty()) { DayN = FCString::Atoi(*Opt); bDayWrap = UGameplayStatics::HasOption(OptionsString, TEXT("wrap")); }
#if FTB_CAREER
		int32 CDay = 0; bool bCWrap = false;
		if (UFTNightFlow::DayCutNow(this, CDay, bCWrap)) { DayN = CDay; bDayWrap = bCWrap; }
#endif
		DayN = FMath::Clamp(DayN, 1, 8);
		Night = 1; VenueName = TEXT("ROUTE 9"); VenueTag = FString::Printf(TEXT("DAY %d · ON SET"), DayN);
	}
	if (IsLucky())
	{
		// night 3 from the story flow: THE LUCKY 7, 'A CASINO ON FREMONT STREET · THE HOUSE ALWAYS WINS', boss THE PIT BOSS,
		// THE REVOLVER ('WHEN THE RED LINE LOCKS ON, STEP OFF IT'), "CLOSE THE DOORS." The story's waves (guard, drunk, guard /
		// guard, tough, guard, tough / bouncer, guard, bouncer, guard) with the casino's own drunk: the Dealer (new, throws
		// cards along your lane); tough = the Punk, bouncer = the Bouncer; a Dealer more in waves 1 and 2 like nights 1 and 2;
		// wave 3 exactly as the story has it
		VenueName = TEXT("THE LUCKY 7"); VenueTag = TEXT("A CASINO ON FREMONT STREET · THE HOUSE ALWAYS WINS");
		BossName = TEXT("THE PIT BOSS"); BossTitle = TEXT("RUNS THE FLOOR · NEVER LOST A HAND"); BossCall = TEXT("PIT BOSS: \"CLOSE THE DOORS.\"");
		Neon = FLinearColor(1.f, 0.82f, 0.25f);
		Waves = {
			{ TEXT("guard"), TEXT("dealer"), TEXT("guard"), TEXT("dealer") },
			{ TEXT("guard"), TEXT("rusher"), TEXT("dealer"), TEXT("guard"), TEXT("rusher") },
			{ TEXT("heavy"), TEXT("guard"), TEXT("heavy"), TEXT("guard") } };
		NumWaves = Waves.Num();
		BossK = { 240, 100, 15, 0.45f, 88, 2.6f, TEXT("boss"), TEXT("Pitboss"), 200 };   // web: hp 220, sp 95, dmg 15, wind 0.45, reach 88
		BossRush = 16;
		Rnd.Initialize(1986 + 37);
		return;
	}
	if (!IsVelvet()) return;
	VenueName = TEXT("THE VELVET ROOM"); VenueTag = TEXT("A DISCO ON SUNSET · NO SNEAKERS");
	BossName = TEXT("VINNIE VELOUR"); BossTitle = TEXT("OWNS THE VELVET ROOM · KARATE, BROWN BELT"); BossCall = TEXT("VINNIE: \"SECURITY!\"");
	Neon = FLinearColor(1.f, 0.31f, 0.72f);
	// the story's waves (tough, drunk, tough / guard, tough, drunk, guard / bouncer, guard, tough, bouncer), as the
	// Anchor's roles (tough = the Punk, drunk = the Sleaze, bouncer = the Bouncer), one more each to match night 1's density
	Waves = {
		{ TEXT("rusher"), TEXT("thrower"), TEXT("rusher"), TEXT("grappler") },
		{ TEXT("guard"), TEXT("rusher"), TEXT("thrower"), TEXT("guard"), TEXT("rusher") },
		{ TEXT("heavy"), TEXT("guard"), TEXT("rusher"), TEXT("heavy"), TEXT("thrower"), TEXT("guard") } };
	NumWaves = Waves.Num();
	BossK = { 230, 125, 14, 0.42f, 84, 2.6f, TEXT("boss"), TEXT("Vinnie"), 196 };   // web: hp 200, sp 105, dmg 14, wind 0.42, reach 80
	BossRush = 16;
	Rnd.Initialize(1986 + 23);
}

void AFTBrawlGameMode::BeginPlay()
{
	Super::BeginPlay();
	Rnd.Initialize(1986 + 11);
	SetupNight();
	bAuto = FParse::Param(FCommandLine::Get(), TEXT("brawlauto"));
	bPadTest = FParse::Param(FCommandLine::Get(), TEXT("brawlpadtest"));
	bKeyTest = bPadTest || FParse::Param(FCommandLine::Get(), TEXT("brawlkeytest"));
	bAutoGrab = bKeyTest || FParse::Param(FCommandLine::Get(), TEXT("brawlgrab"));
	bCamCheck = FParse::Param(FCommandLine::Get(), TEXT("brawlcamcheck"));
	bNoKeepIn = FParse::Param(FCommandLine::Get(), TEXT("brawlnokeepin"));          // ... and grabs and throws
	if (bKeyTest) bAuto = false;
	BuildVenue();
	if (bDay)
	{
		// day 1: the film set and its cutscene only (no fight, no fight music); the bar is built but never seen
		ResetState(0, false);
		BuildSet();
		bCuts = true;
		UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL start DAY %d ON SET %s"), DayN, bDayWrap ? TEXT("wrap") : TEXT("before"));
		SetSetLight(bDayWrap);
		StartCut(bDayWrap ? 11 : 10);
		InitializeSession();
		return;
	}
	GetWorld()->SpawnActor<AFTBrawlMusic>();   // Matt's fight music, looping all night
	InitCombatPresentation();
	ResetState(0, false);
	// night 1 opens on its cutscene (run 49531, FTBrawlCutscene.cpp); -brawlnocut goes straight into the fight,
	// -brawlcut=2|3 starts at LAST CALL / the pier (tests)
	// every night opens on its THAT NIGHT and ends on its LAST CALL (nights 2-7: run 32365)
	bCuts = !FParse::Param(FCommandLine::Get(), TEXT("brawlnocut"));
	if (Night == 1) BuildPier();
	int32 FirstCut = 1; FParse::Value(FCommandLine::Get(), TEXT("brawlcut="), FirstCut);
#if FTB_CAREER
	bGoBoss = UFTNightFlow::TakeGoBoss(this);
#endif
	const bool bRestored = ApplySavedCheckpoint();
	if (bRestored) {}
	else if (bGoBoss)
	{
		// 3.9 test jump: no THAT NIGHT, straight to the boss's spot (as -brawlwave=<waves+1>); LAST CALL still follows the win
		Banner = VenueName; BannerSub = VenueTag; BannerT = 0;
		// in the boss's arena with him out at once (his entrance still plays), as when Nova walks in after the last wave
		Wave = NumWaves; Cam = LockX = ArenaEnd(); P.X = Cam + 200.f;
		UE_LOG(LogFTBrawl, Log, TEXT("FT GO night=%d map=%s waves=%d at=boss"), Night, *GetWorld()->GetMapName(), NumWaves);
		SpawnBoss();
	}
	else if (bCuts && FirstCut >= 2) { bWon = true; WinT = 3.f; bCutDone = true; StartCut(FMath::Min(FirstCut, Night == 1 ? 3 : 2)); }
	else if (bCuts) StartCut(1);
	else { Banner = VenueName; BannerSub = VenueTag; BannerT = 0; }
	// test hook: -brawlwave=N starts at wave N (NumWaves + 1 = straight to the boss).
	// FTGo has already placed and spawned its boss; do not relocate that locked arena.
	int32 StartWave = 0;
	if (!bRestored && !bGoBoss && FParse::Value(FCommandLine::Get(), TEXT("brawlwave="), StartWave) && StartWave > 1)
	{
		Wave = FMath::Min(StartWave - 1, NumWaves);
		Cam = FMath::Max(0.f, NextWaveAt(Wave) - 120.f); P.X = Cam + 200.f;
	}
	InitializeSession();
	UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL start %s night=%d waves=%d wave=%d"), *VenueName, Night, NumWaves, Wave + 1);
}

void AFTBrawlGameMode::ResetState(int32 WaveIdx, bool bKeep)
{
	const float OldX = P.X;
	for (FFTBrawlBody& F : Foes) if (F.Actor) F.Actor->Destroy();
	Foes.Reset();
	StT = 0; Shake = 0; LockX = -1; Wave = WaveIdx; bSpawned = false; Go = 0; KoT = 0; WinT = 0; BossIntro = 0; bWon = false; bBossOut = false;
	if (!bKeep) { Kos = 0; Hits = 0; Best = 0; Time = 0; Cam = 0; }
	Chain = 0;
	AFTBrawlFighter* A = P.Actor;
	P = FFTBrawlBody(); GrabId = -1;
	P.bPlayer = true; P.X = bKeep ? OldX : 160; P.Y = 470; P.Hp = P.Max = P_HP; P.St = TEXT("idle"); P.Inv = 1.2f; P.S = 2.05f;
	if (!A)
	{
		FFTBrawlLook L; L.Shirt = Hex(TEXT("1d1a2e")); L.Pants = Hex(TEXT("2b4a8a")); L.Hair = Hex(TEXT("2a160c")); L.Skin = Hex(TEXT("e8b08a"));
		FFTBrawlKind NK; NK.Who = TEXT("Nova"); NK.Height = 182.f;
		A = SpawnFighter(NK, L);
	}
	P.Actor = A;
	Buffered = {};
	ContactPause = 0.f;
	SustainedTravel = 0.f;
	for (FAccentEffect& Effect : AccentEffects) Effect.Life = 0.f;
	for (AStaticMeshActor* Quad : AccentQuads) if (Quad) Quad->SetActorHiddenInGame(true);
	LastPresentationState = NAME_None; LastPresentationT = -1.f;
	MotionAccentTime = BossAccentTime = AccentSoundCooldown = 0.f;
	for (FCombatEffect& Effect : CombatEffects) Effect.Size = 0.f;
	for (AStaticMeshActor* Quad : CombatQuads) if (Quad) Quad->SetActorHiddenInGame(true);
	Pops.Reset(); Bottles.Reset(); Drops.Reset(); Shots.Reset(); Fires.Reset();
	if (!bKeep && SplatIsm) { SplatIsm->ClearInstances(); SplatNext = 0; }
	if (!bKeep) SpawnPickups();   // the bar's weapons and beers, fresh each night
	else { Held = -1; HeldUses = 0; PowerT = ToughT = BuzzT = 0; }
}

// each night its own regulars (run 95562; owner, 15 Nov: "each of the nights needs to have its own enemies ... some cross
// over"). The roles (rusher / grappler / thrower / heavy / guard / dealer) keep their moves; this picks who plays them on
// that night: Sprite = /Game/Brawl/Sprites/<Sprite>/T_<sprite>_<frame> (ArtSource/tools/frames.py), H = sprite height (cm).
// A (night, role) not listed falls back to the Rusty Anchor's cast in SpawnFighter's Looks (night 1 = the bar's own).
struct FNightFoe { int32 Night; const TCHAR* Role; const TCHAR* Sprite; float H; };
static const FNightFoe GNightFoes[] = {
	// night 2, THE VELVET ROOM: the Disco Dude, the Hustler, the Barman (new); the Guard; the Bouncer works the rope here too
	{ 2, TEXT("rusher"), TEXT("Disco"), 228.f }, { 2, TEXT("grappler"), TEXT("Hustler"), 236.f }, { 2, TEXT("thrower"), TEXT("Barman"), 220.f },
	// night 3, THE LUCKY 7: the High Roller and the Muscle (new); the Dealer; the casino hires the Velvet Room's Guard
	{ 3, TEXT("rusher"), TEXT("Roller"), 230.f }, { 3, TEXT("heavy"), TEXT("Muscle"), 272.f },
	// nights 4-7 (run 97578): four new regulars a night, one per role the night's waves use, themed to the venue
	// night 4, THE TIKI TORCH: the Fire Dancer, the Surfer, the Tourist (throws his tiki mug), the Pit Cook
	{ 4, TEXT("rusher"), TEXT("Firedancer"), 228.f }, { 4, TEXT("grappler"), TEXT("Surfer"), 240.f }, { 4, TEXT("thrower"), TEXT("Tourist"), 216.f }, { 4, TEXT("heavy"), TEXT("Pitcook"), 268.f },
	// night 5, STRIKE CITY: the Pinsetter, the Leaguer, the Shoe Clerk (throws rental shoes), the Lane Hog
	{ 5, TEXT("rusher"), TEXT("Pinsetter"), 226.f }, { 5, TEXT("grappler"), TEXT("Leaguer"), 240.f }, { 5, TEXT("thrower"), TEXT("Shoeclerk"), 216.f }, { 5, TEXT("heavy"), TEXT("Lanehog"), 268.f },
	// night 6, THE FROSTBITE LODGE: the Ski Bum, the Linemate (Sven's hockey team), the Snowboarder (snowballs), the Goalie
	{ 6, TEXT("rusher"), TEXT("Skibum"), 228.f }, { 6, TEXT("grappler"), TEXT("Linemate"), 240.f }, { 6, TEXT("thrower"), TEXT("Boarder"), 218.f }, { 6, TEXT("heavy"), TEXT("Goalie"), 272.f },
	// night 7, THE SEA BISCUIT: the Deckhand, the First Mate (belaying pin, the guard's moves), the Waiter (champagne), the Tux Brute
	{ 7, TEXT("rusher"), TEXT("Deckhand"), 228.f }, { 7, TEXT("guard"), TEXT("Firstmate"), 232.f }, { 7, TEXT("thrower"), TEXT("Waiter"), 218.f }, { 7, TEXT("heavy"), TEXT("Tuxbrute"), 268.f },
};

AFTBrawlFighter* AFTBrawlGameMode::SpawnFighter(const FFTBrawlKind& K, const FFTBrawlLook& L)
{
	AFTBrawlFighter* A = GetWorld()->SpawnActor<AFTBrawlFighter>();
	// 2.5D: every fighter has his own painted frames (ArtSource/sheets, Codex to the cast concepts): Nova, and per role
	// the Punk (rusher), the Biker (grappler), the Sleaze (bottle thrower), the Bouncer (heavy) and Big Earl (boss).
	// Sprites are drawn 1.25x life size (arcade scale: the fight is 13.4 m of bar wide on screen), the room's painted
	// props and wall are scaled to match. The Dockhand's frames, then the Meshy cast, then the blocky body are the fallbacks.
	struct FRoleLook { const TCHAR* Role; const TCHAR* Sprite; float H; };
	static const FRoleLook Looks[] = {
		{ TEXT("rusher"), TEXT("Punk"), 228.f },
		{ TEXT("grappler"), TEXT("Biker"), 240.f },
		{ TEXT("thrower"), TEXT("Sleaze"), 218.f },
		{ TEXT("heavy"), TEXT("Bouncer"), 268.f },
		{ TEXT("guard"), TEXT("Guard"), 232.f },
		{ TEXT("dealer"), TEXT("Dealer"), 226.f },
		{ TEXT("boss"), TEXT("Earl"), 300.f } };
	bool bOk = false;
	if (K.Who == TEXT("Nova")) bOk = A->SetSprite(TEXT("Nova"), 228.f);
	else
	{
		for (const FNightFoe& NF : GNightFoes)   // tonight's own regular for this role (run 95562)
			if (NF.Night == Night && K.Role == NF.Role) { bOk = A->SetSprite(NF.Sprite, NF.H); break; }
		if (!bOk)
		for (const FRoleLook& R : Looks)
			if (K.Role == R.Role)
			{
				if (K.Role == TEXT("boss") && K.Who == TEXT("Vinnie")) { bOk = A->SetSprite(TEXT("Vinnie"), 272.f) || A->SetSprite(TEXT("Dock"), 272.f); break; }   // lean, taller than Nova
				if (K.Role == TEXT("boss") && K.Who == TEXT("Pitboss")) { bOk = A->SetSprite(TEXT("Pitboss"), 284.f) || A->SetSprite(TEXT("Dock"), 284.f); break; }   // tall, broad
				if (K.Role == TEXT("boss") && K.Who != TEXT("Earl") && K.Who != TEXT("Vinnie")) { bOk = A->SetSprite(K.Who, K.Height * 1.4f) || A->SetSprite(TEXT("Earl"), 300.f); break; }   // nights 4-7 (run 32365): Frankie, the Kingpin, Sven, the Commodore
				bOk = A->SetSprite(R.Sprite, R.H) || A->SetSprite(TEXT("Dock"), R.H); break;
			}
	}
	if (bOk && K.Who != TEXT("Nova")) A->SetEnemyPresentation(K.Who, K.Role);
	if (!bOk && !A->SetCast(K.Who, K.Height)) A->SetLook(L);
	return A;
}

FFTBrawlBody AFTBrawlGameMode::MakeFoe(int32 Id, FName Kind, float X, float Y)
{
	FFTBrawlBody F;
	F.Id = Id; F.K = Kinds.FindRef(Kind); F.X = X; F.Y = Y; F.Hp = F.Max = F.K.Hp; F.S = F.K.S; F.Cd = 0.6f + Rand() * 1.2f; F.Walk = Rand();
	FFTBrawlLook L;
	static const TCHAR* Shirts[] = { TEXT("c0392b"), TEXT("36a3ff"), TEXT("ffd23f"), TEXT("9fe870"), TEXT("ff8a3d"), TEXT("b38cff") };
	if (Kind == TEXT("drunk")) { L.Shirt = Hex(Shirts[Id % 6]); L.Pants = Hex(TEXT("4a4f5a")); L.Hair = Hex(Id % 2 ? TEXT("7a4a1e") : TEXT("d8c27a")); }
	else if (Kind == TEXT("tough")) { L.Shirt = Hex(TEXT("2e2a26")); L.Pants = Hex(TEXT("1f3c66")); L.Hair = Hex(TEXT("111111")); L.Girth = 1.08f; }
	else if (Kind == TEXT("bouncer")) { L.Shirt = Hex(TEXT("101014")); L.Pants = Hex(TEXT("101014")); L.Hair = Hex(TEXT("111111")); L.Girth = 1.3f; L.Skin = Hex(TEXT("a8714f")); }
	else { L.Shirt = Hex(TEXT("3b4a7a")); L.Pants = Hex(TEXT("20263a")); }
	F.Actor = SpawnFighter(F.K, L);
	return F;
}

void AFTBrawlGameMode::SpawnWave(int32 I)
{
	const TArray<FName>& List = Waves[I];
	for (int32 j = 0; j < List.Num(); j++)
	{
		const float Side = j % 2 ? -1.f : 1.f;
		FFTBrawlBody F = MakeFoe(I * 100 + j, List[j], LockX + (Side > 0 ? W + 40 + j * 30 : -40 - j * 30), FT + 20 + Rand() * (FB - FT - 40));
		F.Cd = 0.2f + Rand() * 0.6f;
		F.Dir = -Side;
		Foes.Add(F);
	}
	bSpawned = true;
	UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL wave %d/%d locked at x=%.0f: %d foes"), I + 1, NumWaves, LockX, List.Num());
}

void AFTBrawlGameMode::SpawnBoss()
{
	FFTBrawlBody B;
	B.Id = 999; B.bBoss = true; B.K = BossK; B.X = LockX + W + 80; B.Y = (FT + FB) / 2; B.Hp = B.Max = BossK.Hp; B.Dir = -1; B.Cd = 1.6f; B.S = BossK.S; B.Charge = 3.5f;
	FFTBrawlLook L; L.Shirt = Hex(TEXT("f4efe2")); L.Pants = Hex(TEXT("3a2a1e")); L.Hair = Hex(TEXT("8a8a8a")); L.Skin = Hex(TEXT("d99a74")); L.Girth = 1.45f;
	B.Actor = SpawnFighter(BossK, L); B.SlamCd = IsVelvet() || IsLucky() || IsN47() ? 2.0f : 3.0f;   // Vinnie / the Pit Boss / nights 4-7 show their move early
	Foes.Add(B);
	BossIntro = 2.2f; BossEntranceAt = StT; bBossOut = true; bSpawned = true;
	UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL boss %s"), *BossName);
	if (bGoBoss) UE_LOG(LogFTBrawl, Log, TEXT("FT GO landed night=%d boss=%s hp=%.0f"), Night, *BossName, B.Hp);
}

void AFTBrawlGameMode::Pop(float X, float Y, float Z, const FString& Text, const FLinearColor& C, bool bBig)
{
	Pops.Add({ World(X, Y, Z), Text, C, 0.f, bBig });
	if (Pops.Num() > 40) Pops.RemoveAt(0);
}

// ---------------------------------------------------------------- input
AFTBrawlGameMode::FIn AFTBrawlGameMode::ReadInput()
{
	FIn I;
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return I;
	auto D = [PC](const FKey& K) { return PC->IsInputKeyDown(K); };
	I.Steer = (D(EKeys::D) || D(EKeys::Right) || D(EKeys::Gamepad_DPad_Right) ? 1.f : 0.f) - (D(EKeys::A) || D(EKeys::Left) || D(EKeys::Gamepad_DPad_Left) ? 1.f : 0.f);
	I.Throttle = (D(EKeys::W) || D(EKeys::Up) || D(EKeys::Gamepad_DPad_Up) ? 1.f : 0.f) - (D(EKeys::S) || D(EKeys::Down) || D(EKeys::Gamepad_DPad_Down) ? 1.f : 0.f);
	const float SX = PC->GetInputAnalogKeyState(EKeys::Gamepad_LeftX), SY = PC->GetInputAnalogKeyState(EKeys::Gamepad_LeftY);
	if (FMath::Abs(SX) > 0.3f) I.Steer = FMath::Sign(SX);
	if (FMath::Abs(SY) > 0.3f) I.Throttle = FMath::Sign(SY);
	I.Fire = D(EKeys::J) || D(EKeys::Z) || D(EKeys::Gamepad_FaceButton_Left) || D(EKeys::LeftMouseButton);
	I.Kick = D(EKeys::K) || D(EKeys::X) || D(EKeys::Gamepad_FaceButton_Top) || D(EKeys::RightMouseButton);
	I.Jump = D(EKeys::SpaceBar) || D(EKeys::Gamepad_FaceButton_Bottom);
	I.Enter = D(EKeys::Enter);
	// loop cycle 2: with a pad, A (the jump button) also works the cards and the K.O. / knocked-out screens; only Start did
	if (!I.Enter && D(EKeys::Gamepad_FaceButton_Bottom) && (InCut() || P.Hp <= 0 || bWon)) I.Enter = true;
	// 3.9 (owner: "clicks not registering"): a left click works them too (the cards, KNOCKED OUT, the win); in the fight it punches
	if (!I.Enter && D(EKeys::LeftMouseButton) && (InCut() || P.Hp <= 0 || bWon)) I.Enter = true;
	I.Grab = D(EKeys::L) || D(EKeys::C) || D(EKeys::Gamepad_FaceButton_Right);
	if (FMath::Abs(SX) > 0.3f || FMath::Abs(SY) > 0.3f || D(EKeys::Gamepad_FaceButton_Bottom) || D(EKeys::Gamepad_FaceButton_Left) ||
		D(EKeys::Gamepad_FaceButton_Top) || D(EKeys::Gamepad_FaceButton_Right) || D(EKeys::Gamepad_DPad_Left) ||
		D(EKeys::Gamepad_DPad_Right) || D(EKeys::Gamepad_DPad_Up) || D(EKeys::Gamepad_DPad_Down) || D(EKeys::Gamepad_Special_Right))
		bPadLast = true;
	else if (I.Steer != 0 || I.Throttle != 0 || I.Fire || I.Kick || I.Jump || I.Grab || I.Enter)
		bPadLast = false;
	return I;
}

// the test autopilot: walk right, go for the nearest one standing, punch combos, kick now and then
AFTBrawlGameMode::FIn AFTBrawlGameMode::AutoInput()
{
	FIn I;
	const FFTBrawlBody* Tgt = nullptr; float Best2 = 1e9f;
	for (const FFTBrawlBody& F : Foes)
	{
		if (F.St == TEXT("dead") || F.St == TEXT("down") || F.St == TEXT("fly") || F.St == TEXT("lifted") || F.St == TEXT("hurled")) continue;
		const float D = FMath::Abs(F.X - P.X) + FMath::Abs(F.Y - P.Y);
		if (D < Best2) { Best2 = D; Tgt = &F; }
	}
	if (P.St == TEXT("held")) { I.Fire = int32(StT * 8) % 2 == 0; return I; }   // mash free of a grab
	// -brawlgrab: carrying one, turn to the nearest other foe standing (on his lane) and hurl him at it
	if (bAutoGrab && (P.St == TEXT("lift") || P.St == TEXT("carry")))
	{
		const FFTBrawlBody* Aim = nullptr; float Ad2 = 1e9f;
		for (const FFTBrawlBody& F : Foes) if (F.Id != GrabId && Standing(F) && FMath::Abs(F.X - P.X) < Ad2) { Ad2 = FMath::Abs(F.X - P.X); Aim = &F; }
		if (Aim)
		{
			const float Dy = Aim->Y - P.Y;
			if (FMath::Abs(Dy) > 10) I.Throttle = Dy > 0 ? -1.f : 1.f;
			I.Steer = Aim->X > P.X ? 1.f : -1.f;
		}
		I.Grab = P.St == TEXT("carry") && P.T > 0.15f && (!Aim || FMath::Abs(Aim->Y - P.Y) < 14 || P.T > 1.0f);
		return I;
	}
	{
		const int32 Bi = BossIdx();
		if (Bi != INDEX_NONE && (Foes[Bi].St == TEXT("whirltell") || Foes[Bi].St == TEXT("whirl")))   // out of his spotlight
		{
			const FFTBrawlBody& B = Foes[Bi];
			const float Dx = P.X - B.X;
			const float Room = Dx > 0 ? (LockX + W - 40) - P.X : P.X - (LockX + 40);
			I.Steer = Room > 40 ? (Dx > 0 ? 1.f : -1.f) : (Dx > 0 ? -1.f : 1.f);   // cornered: run past him
			I.Throttle = P.Y > B.Y ? -1.f : 1.f;
			if (Room <= 40 && FMath::Abs(Dx) < 160) I.Jump = int32(StT * 4) % 2 == 0;
			return I;
		}
	}
	// the Pit Boss's red line: step off the lane it locks on (and off any bullet coming down his lane)
	{
		float Danger = -1;
		const int32 Bi = BossIdx();
		if (Bi != INDEX_NONE && (Foes[Bi].St == TEXT("draw") || Foes[Bi].St == TEXT("fire"))) Danger = Foes[Bi].SigY;
		for (const FFTBrawlShot& S : Shots) if (!S.bCard && (S.X - P.X) * S.Vx < 0) Danger = S.Y;
		if (Danger >= 0 && FMath::Abs(P.Y - Danger) < 46 && !FParse::Param(FCommandLine::Get(), TEXT("brawlnododge")))   // test hook: -brawlnododge stays on the line
		{
			I.Throttle = (Danger - FT > FB - Danger) ? 1.f : -1.f;   // towards the wider side of the floor band
			return I;
		}
	}
	// nights 4-7 (run 32365), what a player does: off Frankie's / Sven's line, over the bowling balls, over the anchor
	{
		const int32 Bi = BossIdx();
		const FFTBrawlBody* B = Bi != INDEX_NONE ? &Foes[Bi] : nullptr;
		if (B && IsN47() && !FParse::Param(FCommandLine::Get(), TEXT("brawlnododge")))
		{
			const bool bLine = (Night == 4 && (B->St == TEXT("swig") || B->St == TEXT("breathe"))) || (Night == 6 && (B->St == TEXT("gloves") || B->St == TEXT("lunge")));
			if (bLine && FMath::Abs(P.Y - B->SigY) < 50) { I.Throttle = (B->SigY - FT > FB - B->SigY) ? 1.f : -1.f; return I; }
			if (Night == 7 && B->St == TEXT("windup") && B->T > 0.74f && P.Z <= 0) { I.Jump = true; return I; }
			for (const FFTBrawlShot& S : Shots)
				if (S.bBall && FMath::Abs(S.Y - P.Y) < 24 && (P.X - S.X) * S.Vx > 0 && FMath::Abs(P.X - S.X) < 130 && P.Z <= 0) { I.Jump = true; return I; }
		}
	}
	// a card flying at him down his lane: jump it (what a player does)
	for (const FFTBrawlShot& S : Shots)
		if (S.bCard && FMath::Abs(S.Y - P.Y) < 20 && (P.X - S.X) * S.Vx > 0 && FMath::Abs(P.X - S.X) < 150 && P.Z <= 0) { I.Jump = true; return I; }
	// pickups first (run 21219): a weapon when empty-handed, food when hurt, any power-up, a beer off the bar when hurt
	{
		const FFTBrawlItem* Pick = nullptr; float Gd = 1e9f;
		const float Lo = LockX >= 0 ? LockX + 30 : Cam + 30;
		for (const FFTBrawlItem& It : Items)
		{
			if (It.bThrown || It.Z > 1 || It.X < Lo || It.X > (LockX >= 0 ? LockX + W - 30 : 1e9f)) continue;
			const int32 Ty = GItems[It.Kind].Type;
			const bool bWant = (Ty == TY_WEAPON && Held < 0) || (Ty == TY_FOOD && P.Hp < P.Max - 15) || Ty == TY_POWER || (Ty == TY_BEER && P.Hp < P.Max - 20 && BuzzT <= 0);
			if (!bWant) continue;
			const float D = FMath::Abs(It.X - P.X) + FMath::Abs((It.bBar ? FT + 10 : It.Y) - P.Y);
			if (D < (Tgt ? 240.f : 700.f) && D < Gd) { Gd = D; Pick = &It; }
		}
		if (Pick)
		{
			const float Dx = Pick->X - P.X, Dy = (Pick->bBar ? FT + 10 : Pick->Y) - P.Y;
			if (FMath::Abs(Dy) > 10) I.Throttle = Dy > 0 ? -1.f : 1.f;
			if (FMath::Abs(Dx) > 18) I.Steer = Dx > 0 ? 1.f : -1.f;
			if (FMath::Abs(Dx) < 30 && FMath::Abs(Dy) < 16) I.Fire = int32(StT * 10) % 2 == 0;
			I.Enter = KoT > 1.2f;
			return I;
		}
	}
	// test hook: -brawlnext presses Enter after a win (night 1 -> night 2)
	if (bWon) { I.Enter = WinT > 3.f && FParse::Param(FCommandLine::Get(), TEXT("brawlnext")); return I; }
	if (!Tgt) { I.Steer = 1; I.Enter = KoT > 1.2f; return I; }
	const float Dx = Tgt->X - P.X, Dy = Tgt->Y - P.Y, Want = Tgt->bBoss ? 70.f : 55.f;
	if (FMath::Abs(Dy) > 14) I.Throttle = Dy > 0 ? -1.f : 1.f;
	if (FMath::Abs(Dx) > (Held >= 0 ? FMath::Max(Want, GItems[Held].Reach - 20) : Want)) I.Steer = Dx > 0 ? 1.f : -1.f;
	else if ((Dx > 0) != (P.Dir > 0)) I.Steer = Dx > 0 ? 0.3f : -0.3f;
	const int32 Ph = int32(StT * 10) % 4;
	const float WantW = Held >= 0 ? FMath::Max(Want, GItems[Held].Reach - 20) : Want;   // a cue keeps them at its length
	static const bool bTestAerial = FParse::Param(FCommandLine::Get(), TEXT("brawlairtest"));
	if (bTestAerial && P.Hp > 0.f && Held < 0 && FMath::Abs(Dx) < 115.f && FMath::Abs(Dy) < 30.f)
	{
		I.Jump = P.Z <= 0.f && P.St == TEXT("walk");
		I.Kick = P.St == TEXT("jump") && P.T >= 0.1f;
		return I;
	}
	// test hook (run 32365): -brawlpassive walks up to the boss but never hits him, so his own move shows (and lands)
	if (FMath::Abs(Dx) < WantW + 20 && FMath::Abs(Dy) < 22 && !(Tgt->bBoss && FParse::Param(FCommandLine::Get(), TEXT("brawlpassive"))))
	{
		static const bool bTestThrow = FParse::Param(FCommandLine::Get(), TEXT("brawlthrowtest"));
		const bool bKick = (bTestThrow && Held >= 0)
			|| (int32(StT * 3) % 5 == 4 && (Held < 0 || HeldUses <= 1));
		// -brawlgrab: with another foe standing to throw him at, grab this one (when he is in reach in front)
		int32 Up = 0; for (const FFTBrawlBody& F : Foes) if (Standing(F)) Up++;
		if (bAutoGrab && Held < 0 && !Tgt->bBoss && Standing(*Tgt) && Up >= 2 && FMath::Abs(Dy) < 20 && Dx * P.Dir > -6 && Dx * P.Dir < 66 && Ph < 2) { I.Grab = true; return I; }
		if (bKick) I.Kick = Ph < 2; else I.Fire = Ph % 2 == 0;
	}
	I.Enter = KoT > 1.2f;
	return I;
}

// ---------------------------------------------------------------- the player's moves
void AFTBrawlGameMode::HitFoe(FFTBrawlBody& F, float Dmg, bool bDown, bool bSound)
{
	if (F.St == TEXT("dizzy")) Dmg *= 1.5f;   // Vinnie, dizzy after his whirlwind: the time to hit him
	if (F.St == TEXT("draw") || F.St == TEXT("fire")) { F.SlamCd = 2.5f; UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL boss revolver interrupted")); }   // hit him out of his draw
	F.Hp -= Dmg; F.Flash = 0.12f; F.bIn = true;   // hit = in the fight: the screen edge holds him from now on (KeepOnScreen)
	const bool bFinisher = bDown || F.Hp <= 0.f;
	const bool bWeapon = P.St == TEXT("wpn");
	ContactPause = FMath::Max(ContactPause, bFinisher ? 3.f * STEP : 2.f * STEP);
	CombatCue(bWeapon ? ECombatCue::Weapon : bFinisher ? ECombatCue::Finish : ECombatCue::Jab,
		World(F.X - P.Dir * 12.f, F.Y + 9.f, F.Z + 105.f * F.K.Height / 175.f), P.Dir, bSound);
	Shake = FMath::Min(1.f, Shake + (bDown ? 0.55f : 0.3f));
	Rumble(bDown || F.Hp <= 0 ? 0.55f : 0.3f, bDown || F.Hp <= 0 ? 0.16f : 0.09f, bDown || F.Hp <= 0 ? TEXT("knock") : TEXT("hit"));
	if (F.Hp > 0 && !bDown) { static const TCHAR* Grunts[] = { TEXT("hurt1"), TEXT("hurt2"), TEXT("hurt3") }; Sfx(Grunts[Voice++ % 3], 0.5f, (F.bBoss ? 0.78f : 1.f) * (0.92f + 0.05f * (F.Id % 4))); }
	Chain++; if (Chain > Best) Best = Chain;
	// no comic words: a spray of blood off the face, more on the big ones, and it stays on the floor
	Bleed(F.X + P.Dir * 6, F.Y, 120 * F.K.Height / 175.f, P.Dir, bDown ? 34 : 14, bDown ? 1.4f : 1.f);
	if ((F.St == TEXT("grab") || F.St == TEXT("knee") || F.St == TEXT("hug")) && P.St == TEXT("held")) { P.St = TEXT("idle"); P.T = 0; P.Inv = 0.4f; }
	const bool bArmour = F.K.Role == TEXT("heavy") && !bDown && F.Hp > 0;   // the bouncer shrugs off jabs
	const bool bSpin = F.St == TEXT("whirl") || F.St == TEXT("whirltell");   // the whirlwind can't be stopped: step out of the light
	if (F.Hp <= 0 || (bDown && !bSpin)) { F.St = TEXT("fly"); F.T = 0; F.Vx = P.Dir * (F.bBoss ? 180 : 300); F.Vz = F.bBoss ? 260 : 340; F.Dir = -P.Dir; }
	else if (!bArmour && !bSpin && F.St != TEXT("dizzy") && F.St != TEXT("slamwind") && F.St != TEXT("slam") && F.St != TEXT("charge")
		&& F.St != TEXT("swig") && F.St != TEXT("breathe") && F.St != TEXT("hold") && F.St != TEXT("bowl") && F.St != TEXT("gloves") && F.St != TEXT("lunge")
		&& F.St != TEXT("hug") && F.St != TEXT("windup") && F.St != TEXT("sweep")) { F.St = TEXT("hurt"); F.T = 0; F.X += P.Dir * 12; }   // nights 4-7: a jab doesn't stop the boss's move
	else F.X += P.Dir * 4;
}

void AFTBrawlGameMode::BufferInput(const FIn& In)
{
	if (InCut() || bWon || P.Hp <= 0.f || BossIntro > 0.f) Buffered = {};
	else
	{
		if (In.Fire && !Latch.Fire) Buffered.Fire = 0.12f;
		if (In.Kick && !Latch.Kick) Buffered.Kick = 0.10f;
		if (In.Jump && !Latch.Jump) Buffered.Jump = 0.10f;
		if (In.Grab && !Latch.Grab) Buffered.Grab = 0.10f;
	}
	Latch = In;
}

void AFTBrawlGameMode::UpdatePlayer(float Dt, const FIn& In)
{
	const bool FirePress = Buffered.Fire > 0.f, JumpPress = Buffered.Jump > 0.f;
	const bool KickPress = Buffered.Kick > 0.f, GrabPress = Buffered.Grab > 0.f;
	P.T += Dt; if (P.Inv > 0) P.Inv -= Dt; if (P.ComboT > 0) P.ComboT -= Dt; else P.Combo = 0;
	if (P.Z > 0 || P.Vz > 0)
	{
		P.Vz -= 1700 * Dt; P.Z += P.Vz * Dt; P.X += P.JVx * Dt; P.Y += P.JVy * Dt;
		if ((FirePress || KickPress) && P.St == TEXT("jump")) { Buffered.Fire = Buffered.Kick = 0; P.St = TEXT("jumpkick"); P.T = 0; P.HitIds.Reset(); Sfx(TEXT("whoosh"), 0.55f, 0.75f); }
		if (P.Z <= 0) {
			Accent(EAccent::Land, World(P.X, P.Y + 7.f, 10.f), 102.f, P.Dir);
			P.Z = 0; P.Vz = 0; P.St = TEXT("idle"); P.T = 0;
		}
	}
	else if (P.St == TEXT("held"))
	{
		if (FirePress || KickPress || JumpPress) { P.Mash++; Buffered = {}; }
		if (P.T > 3.5f) { P.St = TEXT("idle"); P.T = 0; }
	}
	else if (P.St == TEXT("hurt")) { if (P.T > 0.3f) P.St = TEXT("idle"); }
	else if (P.St == TEXT("down")) { if (P.T > 0.9f && P.Hp > 0) { P.St = TEXT("getup"); P.T = 0; } }
	else if (P.St == TEXT("getup")) { if (P.T > 0.35f) { P.St = TEXT("idle"); P.Inv = 1.0f; } }
	else if (P.St == TEXT("punch")) { if (P.T > (P.Combo == 3 ? 0.3f : 0.2f)) P.St = TEXT("idle"); }
	else if (P.St == TEXT("kick")) { if (P.T > 0.4f) P.St = TEXT("idle"); }
	else if (P.St == TEXT("pickup")) { if (P.T > 0.22f) P.St = TEXT("idle"); }
	// grab and throw (run 42137): lift him (0.22 s), carry him (walk slower; he is hurled by himself after 1.6 s), hurl
	else if (P.St == TEXT("lift")) { if (P.T > 0.22f) { P.St = TEXT("carry"); P.T = 0; } }
	else if (P.St == TEXT("carry"))
	{
		if (GrabPress || FirePress || KickPress || P.T > 1.6f) { Buffered = {}; if (In.Steer != 0) P.Dir = In.Steer > 0 ? 1 : -1; P.St = TEXT("hurl"); P.T = 0; }
		else
		{
			P.X += In.Steer * 200 * Dt; P.Y -= In.Throttle * 140 * Dt;
			if (In.Steer != 0) { P.Dir = In.Steer > 0 ? 1 : -1; P.Walk += Dt * 1.2f; }
		}
	}
	else if (P.St == TEXT("hurl")) { if (P.T >= 0.1f && GrabId != -1) { Hurl(); Sfx(TEXT("whooshbig"), 0.8f, 0.7f); } if (P.T > 0.34f) P.St = TEXT("idle"); }
	else if (P.St == TEXT("drink"))
	{
		if (P.T > 0.95f)
		{
			const float H0 = P.Hp; P.Hp = FMath::Min(P.Max, P.Hp + 25); BuzzT = BuzzTime; P.St = TEXT("idle"); P.T = 0;
			Pop(P.X, P.Y, 170, TEXT("BUZZED!"), FLinearColor(1.f, 0.82f, 0.25f), true);
			UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL drink beer hp=%.0f->%.0f buzzed %.0fs"), H0, P.Hp, BuzzTime);
		}
	}
	else if (P.St == TEXT("wpn")) { if (P.T > 0.34f) { P.St = TEXT("idle"); if (HeldUses <= 0) BreakHeld(); } }
	else if (P.St == TEXT("toss"))
	{
		if (P.T >= 0.14f && Held >= 0)
		{
			FFTBrawlItem It; It.Kind = Held; It.Uses = HeldUses; It.bThrown = true; It.X = P.X + P.Dir * 30; It.Y = P.Y; It.Z = 110; It.Vx = P.Dir * 950;
			Items.Add(It);
			CombatCue(ECombatCue::Throw, World(It.X, It.Y + 6.f, It.Z), P.Dir);
			UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL throw %s"), GItems[Held].Name);
			Held = -1; HeldUses = 0;
		}
		if (P.T > 0.32f) P.St = TEXT("idle");
	}
	const bool Can = P.St == TEXT("idle") || P.St == TEXT("walk");
	bool bUsed = false;
	if (Can && Buffered.Fire > 0.f)
	{
		bUsed = UseGroundItem();
		if (!bUsed && Held >= 0) { P.St = TEXT("wpn"); P.T = 0; P.HitIds.Reset(); bWpnHit = false; bUsed = true; Sfx(TEXT("whooshbig"), 0.7f, GItems[Held].Reach > 140 ? 0.85f : 1.05f); }
	}
	if (bUsed) Buffered.Fire = 0.f;
	if (Can && !bUsed && Buffered.Kick > 0.f && Held >= 0) { Buffered.Kick = 0; P.St = TEXT("toss"); P.T = 0; bUsed = true; Sfx(TEXT("whooshbig"), 0.6f, 1.2f); }
	if (Can && !bUsed && Buffered.Grab > 0.f && TryGrab()) { Buffered.Grab = 0; bUsed = true; }
	if (!bUsed && (Can || (P.St == TEXT("punch") && P.T > 0.12f)))
	{
		if (Buffered.Fire > 0.f) { Buffered.Fire = 0; P.Combo = P.ComboT > 0 && P.Combo < 3 ? P.Combo + 1 : 1; P.ComboT = 0.5f; P.St = TEXT("punch"); P.T = 0; P.HitIds.Reset(); Sfx(TEXT("whoosh"), 0.45f, 1.f + 0.08f * P.Combo); }
		else if (Can && Buffered.Kick > 0.f) { Buffered.Kick = 0; P.St = TEXT("kick"); P.T = 0; P.HitIds.Reset(); Sfx(TEXT("whoosh"), 0.55f, 0.8f); }
		else if (Can && Buffered.Jump > 0.f) { Buffered.Jump = 0; P.St = TEXT("jump"); P.T = 0; P.Vz = 600; P.Z = 0.01f; P.JVx = In.Steer * 320; P.JVy = -In.Throttle * 120; P.HitIds.Reset(); }
	}
	auto AgePress = [Dt](float& Remaining) { Remaining = FMath::Max(0.f, Remaining - Dt); };
	AgePress(Buffered.Fire);
	AgePress(Buffered.Kick);
	AgePress(Buffered.Jump);
	AgePress(Buffered.Grab);
	if (P.St == TEXT("idle") || P.St == TEXT("walk"))
	{
		const float Mx = In.Steer, My = -In.Throttle;
		SustainedTravel = FMath::Abs(Mx) > 0.5f && My == 0.f && FMath::Sign(Mx) == P.Dir
			? FMath::Min(0.3f, SustainedTravel + Dt) : 0.f;
		P.X += Mx * 330 * Dt; P.Y += My * 210 * Dt;
		if (BuzzT > 0 && (Mx != 0 || My != 0)) { P.Y += FMath::Sin(StT * 2.3f) * 40 * Dt; P.X += FMath::Cos(StT * 1.7f) * 25 * Dt; }   // a little unsteady
		if (Mx != 0) P.Dir = Mx > 0 ? 1 : -1;
		P.St = (Mx != 0 || My != 0) ? TEXT("walk") : TEXT("idle"); if (Mx != 0 || My != 0) P.Walk += Dt * 1.5f;
	}
	else SustainedTravel = 0.f;
	P.Y = FMath::Clamp(P.Y, FT, FB);
	const float Lo = LockX >= 0 ? LockX + 30 : Cam + 30, Hi = LockX >= 0 ? LockX + W - 30 : ArenaEnd() + W - 30;
	P.X = FMath::Clamp(P.X, Lo, Hi);
	// the lifted foe rides over his head (rises with the lift, bobs a little as he carries him)
	if (GrabId != -1)
	{
		FFTBrawlBody* G = Foes.FindByPredicate([this](const FFTBrawlBody& F) { return F.Id == GrabId; });
		if (!G || G->St != TEXT("lifted")) { GrabId = -1; if (P.St == TEXT("lift") || P.St == TEXT("carry")) { P.St = TEXT("idle"); P.T = 0; } }
		else if (P.St != TEXT("lift") && P.St != TEXT("carry") && P.St != TEXT("hurl")) DropGrab();   // caught some other way (Big Sven's bear hug): he lets go
		else
		{
			const float Up = P.St == TEXT("lift") ? FMath::Lerp(30.f, 112.f, FMath::Clamp(P.T / 0.22f, 0.f, 1.f)) : 112.f + FMath::Sin(P.Walk * 12.f) * 3.f;
			G->X = P.X + P.Dir * 4; G->Y = P.Y + 2; G->Z = Up; G->Dir = P.Dir;
		}
	}
	// the swing lands (attackBox in brawl.js)
	float Dmg = 0, Reach = 0; bool bDown = false;
	if (P.St == TEXT("punch") && P.T >= 0.06f && P.T < 0.14f) { Dmg = P.Combo == 3 ? 12 : 7; Reach = 74; bDown = P.Combo == 3; }
	else if (P.St == TEXT("kick") && P.T >= 0.12f && P.T < 0.24f) { Dmg = 13; Reach = 92; bDown = true; }
	else if (P.St == TEXT("jumpkick") && P.Z > 4) { Dmg = 15; Reach = 84; bDown = true; }
	else if (P.St == TEXT("wpn") && Held >= 0 && P.T >= 0.13f && P.T < 0.21f) { Dmg = GItems[Held].Dmg; Reach = GItems[Held].Reach; bDown = GItems[Held].bDown; }
	// brass knuckles: double on the fists and feet (half again on a weapon); a beer: a quarter more
	Dmg *= (PowerT > 0 ? (P.St == TEXT("wpn") ? 1.5f : 2.f) : 1.f) * (BuzzT > 0 ? 1.25f : 1.f);
	if (Dmg > 0)
		for (FFTBrawlBody& F : Foes)
		{
			if (!Standing(F) || P.HitIds.Contains(F.Id)) continue;
			const float Dx = (F.X - P.X) * P.Dir;
			if (Dx > -10 && Dx < Reach + (F.bBoss ? 20 : 0) && FMath::Abs(F.Y - P.Y) < (F.bBoss ? 34 : 26))
			{
				if (P.HitIds.Num() == 0)   // one impact sound a swing, however many it hits
				{
					if (P.St == TEXT("wpn") && Held >= 0) ImpactSfx(Held);
					else if (P.St == TEXT("punch")) Sfx(P.Combo == 3 ? TEXT("punchbig") : (P.Combo == 2 ? TEXT("punch2") : TEXT("punch")), P.Combo == 3 ? 0.95f : 0.8f, 0.95f + 0.04f * P.Combo);
					else Sfx(TEXT("kick"), 0.95f, P.St == TEXT("jumpkick") ? 0.9f : 1.f);
					UE_LOG(LogFTBrawl, Verbose, TEXT("FTBRAWL swing hit %s dx=%.0f reach=%.0f on %s"), P.St == TEXT("wpn") && Held >= 0 ? GItems[Held].Name : *P.St.ToString(), Dx, Reach, *F.K.Role.ToString());
				}
				P.HitIds.Add(F.Id); HitFoe(F, Dmg, bDown, false);   // the swing's material/contact Sfx above owns its impact sound
				if (P.St == TEXT("wpn") && Held >= 0)
				{
					if (IsGlass(Held)) Bleed(F.X, F.Y, 140, P.Dir, 6, 0.6f, true);
					if (!bWpnHit) { bWpnHit = true; HeldUses--; UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL weapon hit %s on %s uses=%d"), GItems[Held].Name, *F.K.Role.ToString(), HeldUses); }
				}
			}
		}
}

void AFTBrawlGameMode::HurtPlayer(float Dmg, bool bDown, const FFTBrawlBody& From)
{
	if (P.Inv > 0 || P.St == TEXT("down")) return;
	if (GrabId != -1) DropGrab();   // hit while he holds one: he drops him
	if (ToughT > 0) { Dmg *= 0.5f; if (P.Hp - Dmg > 0) bDown = false; }   // the leather jacket: half damage, stays on his feet
	if (BuzzT > 0) Dmg *= 0.8f;                                           // a beer in him: feels it less
	if (P.St == TEXT("drink")) Bleed(P.X, P.Y, 150, -P.Dir, 10, 0.6f, true);   // the beer goes everywhere
	P.Hp -= Dmg; Hits++; Chain = 0; Shake = FMath::Min(1.f, Shake + 0.6f);
	{ const bool bFloor = bDown || P.Hp <= 0; Sfx(bFloor ? TEXT("novadown") : TEXT("novahurt"), bFloor ? 1.f : 0.85f); Rumble(bFloor ? 0.95f : 0.65f, bFloor ? 0.38f : 0.2f, bFloor ? TEXT("floored") : TEXT("hurt")); }
	UE_LOG(LogFTBrawl, Verbose, TEXT("FTBRAWL hurt by %s dmg=%.0f down=%d hp=%.0f"), From.K.Role.IsNone() ? TEXT("shot") : *From.K.Role.ToString(), Dmg, bDown ? 1 : 0, P.Hp);
	P.Dir = From.X > P.X ? 1 : -1;
	ContactPause = FMath::Max(ContactPause, bDown ? 3.f * STEP : STEP);
	CombatCue(ECombatCue::Hurt, World(P.X + P.Dir * 13.f, P.Y + 9.f, P.Z + 116.f), -P.Dir);
	Bleed(P.X, P.Y, 125, -P.Dir, bDown ? 26 : 12, bDown ? 1.3f : 0.9f);
	if (P.Z > 0) { P.Z = 0; P.Vz = 0; }
	if (bDown || P.Hp <= 0)
	{
		P.St = TEXT("down"); P.T = 0; P.Inv = 2; P.X -= P.Dir * 40;
		if (Held >= 0) { DropItem(Held, P.X + P.Dir * 30, P.Y, HeldUses, true, false); UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL knocked down: dropped %s"), GItems[Held].Name); Held = -1; HeldUses = 0; }
	}
	else { P.St = TEXT("hurt"); P.T = 0; P.Inv = 0.7f; P.X -= P.Dir * 14; }
}

// ---------------------------------------------------------------- them
void AFTBrawlGameMode::UpdateFoes(float Dt)
{
	int32 Engaged = 0;
	for (const FFTBrawlBody& F : Foes) if (F.St == TEXT("wind") || F.St == TEXT("swing") || F.St == TEXT("charge") || F.St == TEXT("aim") || F.St == TEXT("grab") || F.St == TEXT("knee")) Engaged++;
	for (int32 i = 0; i < Foes.Num(); i++)
	{
		FFTBrawlBody& F = Foes[i];
		F.T += Dt; if (F.Flash > 0) F.Flash -= Dt;
		if (F.St == TEXT("dead")) continue;
		if (F.St == TEXT("lifted")) { if (GrabId != F.Id) { F.St = TEXT("fly"); F.T = 0; F.Vx = 0; F.Vz = 0; } continue; }   // Nova carries him (UpdatePlayer)
		if (F.St == TEXT("hurled")) { StepHurled(F, Dt); continue; }
		if (F.St == TEXT("fly"))
		{
			F.Vz -= 1500 * Dt; F.Z += F.Vz * Dt; F.X += F.Vx * Dt;
			if (F.Z <= 0 && F.T > 0.05f) { F.Z = 0; F.St = TEXT("down"); F.T = 0; Shake = FMath::Min(1.f, Shake + 0.3f); Sfx(TEXT("down"), F.bBoss ? 1.f : 0.85f, F.bBoss ? 0.8f : 1.f); Rumble(F.bBoss ? 0.7f : 0.45f, F.bBoss ? 0.3f : 0.16f, TEXT("down")); }
			continue;
		}
		if (F.St == TEXT("down"))
		{
			if (F.Hp <= 0) { if (F.T > 0.9f) { F.St = TEXT("dead"); F.T = 0; Kos++; DropFrom(F); Splat(World(F.X - F.Dir * 50, F.Y, 0), 2.6f); if (F.bBoss) { bWon = true; WinT = 0; SaveCheckpoint(TEXT("BossDefeated")); UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL won kos=%d best=%d hits=%d time=%.0f"), Kos, Best, Hits, Time); UE_LOG(LogFTBrawl, Log, TEXT("FT BRAWL won night=%d boss=%s"), Night, *BossName); if (bCamCheck) UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL camcheck night=%d frames=%d offcam=%d worst=%.2f keepin=%d"), Night, CamFrames, CamOff, CamWorst, bNoKeepIn ? 0 : 1); } } }
			else if (F.T > (F.bBoss ? 0.7f : 0.75f)) { F.St = TEXT("getup"); F.T = 0; }
			continue;
		}
		if (F.St == TEXT("getup")) { if (F.T > 0.45f) { F.St = TEXT("walk"); F.T = 0; F.Cd = 0.4f; } continue; }
		if (F.St == TEXT("hurt")) { if (F.T > 0.26f) { F.St = TEXT("walk"); F.T = 0; F.Cd = FMath::Max(F.Cd, 0.3f); } continue; }
		if (P.Hp <= 0) { F.St = TEXT("walk"); continue; }
		const FFTBrawlKind& K = F.K;
		const float Dx = P.X - F.X, Dy = P.Y - F.Y, Ad = FMath::Abs(Dx);
		const FName FighterRole = K.Role;
		const bool bFree = F.St == TEXT("walk") || F.St == TEXT("idle");
		// RUSHER: from across the room, a running shoulder charge (a 0.18 s crouch is the tell)
		if (FighterRole == TEXT("rusher"))
		{
			if (F.St == TEXT("dash"))
			{
				if (F.T > 0.18f) F.X += F.Vx * Dt;
				if (!F.bStruck && F.T > 0.18f && FMath::Abs(P.X - F.X) < 46 && FMath::Abs(P.Y - F.Y) < 26 && P.Z < 30) { F.bStruck = true; HurtPlayer(K.Dmg + 3, false, F); }
				if (F.T > 0.75f || F.X < LockX - 80 || F.X > LockX + W + 80) { F.St = TEXT("walk"); F.T = 0; F.Cd = 0.5f + Rand() * 0.7f; }
				continue;
			}
			if (bFree && F.Cd <= 0 && Ad > 170 && Ad < 460 && FMath::Abs(Dy) < 30) { F.St = TEXT("dash"); F.T = 0; F.bStruck = false; F.Dir = Dx > 0 ? 1 : -1; F.Vx = F.Dir * 640; F.Y += Dy * 0.5f; continue; }
		}
		// GRAPPLER: a grab instead of a punch; holds you and knees you three times, then throws you down. Mash to break free.
		if (FighterRole == TEXT("grappler") && (F.St == TEXT("grab") || F.St == TEXT("knee")))
		{
			if (P.St != TEXT("held")) { F.St = TEXT("walk"); F.T = 0; F.Cd = 1.0f; continue; }
			P.X = F.X + F.Dir * 44; P.Y = F.Y; P.Dir = -F.Dir; P.Z = 0;
			if (P.Mash >= 5) { P.St = TEXT("idle"); P.T = 0; P.Inv = 0.6f; F.St = TEXT("hurt"); F.T = 0; F.X -= F.Dir * 30; continue; }
			if (F.St == TEXT("grab") && F.T > 0.45f) { F.St = TEXT("knee"); F.T = 0; }
			else if (F.St == TEXT("knee") && F.T > 0.12f && !F.bStruck)
			{
				F.bStruck = true; P.Hp -= K.Dmg; Hits++; Chain = 0; Shake = FMath::Min(1.f, Shake + 0.45f); Sfx(TEXT("novahurt"), 0.85f, 1.05f); Rumble(0.6f, 0.18f, TEXT("knee"));
				Bleed(P.X, P.Y, 115, -F.Dir, 14, 0.9f);
				if (P.Hp <= 0) { P.St = TEXT("down"); P.T = 0; P.Inv = 2; F.St = TEXT("walk"); F.T = 0; F.Cd = 1.5f; continue; }
			}
			else if (F.St == TEXT("knee") && F.T > 0.42f)
			{
				F.bStruck = false; F.Hold += 1;
				if (F.Hold >= 3) { P.St = TEXT("idle"); P.Inv = 0; HurtPlayer(K.Dmg + 3, true, F); F.St = TEXT("walk"); F.T = 0; F.Cd = 1.6f; }
				else { F.St = TEXT("grab"); F.T = 0.2f; }
			}
			continue;
		}
		// THROWER: keeps his distance and lobs bottles in an arc; up close he slaps like anyone
		// DEALER (night 3): the same, but he lines up on Nova's lane and flicks two playing cards flat along it
		const bool bDealer = FighterRole == TEXT("dealer");
		if ((FighterRole == TEXT("thrower") || bDealer) && Ad > 100)
		{
			if (F.St == TEXT("throw") && bDealer)
			{
				if ((F.T > 0.22f && F.Hold < 1) || (F.T > 0.42f && F.Hold < 2))
				{
					F.Hold += 1;
					FFTBrawlShot C; C.bCard = true; C.X = F.X + F.Dir * 40; C.Y = F.Y; C.Z = 105; C.Vx = F.Dir * 720; C.Dmg = 5;
					Shots.Add(C);
					if (F.Hold == 1) UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL dealer cards"));
				}
				if (F.T > 0.7f) { F.St = TEXT("walk"); F.T = 0; F.Cd = 2.2f + Rand() * 1.4f; }
				continue;
			}
			if (F.St == TEXT("throw"))
			{
				if (F.T > 0.32f && !F.bStruck)
				{
					F.bStruck = true;
					FFTBrawlBottle B; B.X = F.X + F.Dir * 20; B.Y = F.Y; B.Z = 85; const float Ft = 0.7f;
					B.Vx = FMath::Clamp(Dx / Ft, -700.f, 700.f); B.Vy = Dy / Ft; B.Vz = (0.5f * 1500.f * Ft * Ft - B.Z) / Ft;
					Bottles.Add(B);
				}
				if (F.T > 0.75f) { F.St = TEXT("walk"); F.T = 0; F.Cd = 1.2f + Rand() * 1.2f; }
				continue;
			}
			if (F.St == TEXT("wind") || F.St == TEXT("swing")) { F.St = TEXT("walk"); F.T = 0; }
			F.Cd -= Dt; F.Dir = Dx > 0 ? 1 : -1;
			if (F.Cd <= 0 && Ad < 640 && FMath::Abs(Dy) < (bDealer ? 14.f : 70.f) && P.Hp > 0) { F.St = TEXT("throw"); F.T = 0; F.bStruck = false; F.Hold = 0; continue; }
			float Tx = P.X - F.Dir * (bDealer ? 380 : 330); const float Ty = bDealer ? P.Y : P.Y + (F.Id % 2 ? -30 : 30);
			if (!bNoKeepIn && LockX >= 0) Tx = FMath::Clamp(Tx, LockX + KeepIn, LockX + W - KeepIn);   // backs off, but only to the screen's edge
			const float X0 = F.X;
			const float Mx = Tx - F.X, My = Ty - F.Y, D = FMath::Sqrt(Mx * Mx + My * My);
			if (D > 10) { F.X += Mx / D * K.Sp * Dt; F.Y += My / D * K.Sp * 0.7f * Dt; F.Walk += Dt * 1.1f; F.St = TEXT("walk"); } else F.St = TEXT("idle");
			if (bNoKeepIn) F.X = FMath::Clamp(F.X, LockX - 40, LockX + W + 40);
			else if (LockX >= 0) F.X = FMath::Clamp(F.X, FMath::Min(X0, LockX + KeepIn), FMath::Max(X0, LockX + W - KeepIn));   // walking in from off screen is fine; out is not
			F.Y = FMath::Clamp(F.Y, FT, FB);
			continue;
		}
		// BIG EARL's special: the GROUND SLAM. He winds up (0.55 s), leaps and drops; the shockwave floors anyone near who isn't in the air
		// VINNIE VELOUR's special (brawl.js 'whirl'): he points at the sky, the spotlight and a pink ring find him (0.85 s),
		// then he spins through it after Nova for 1.4 s (7 a hit, knocks down late in the spin), then he is DIZZY (1.6 s, hits x1.5)
		if (F.bBoss && IsVelvet())
		{
			if (F.St == TEXT("whirltell")) { F.Dir = Dx > 0 ? 1 : -1; if (F.T > 0.85f) { F.St = TEXT("whirl"); F.T = 0; F.SigTick = 0; UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL boss whirlwind")); } continue; }
			if (F.St == TEXT("whirl"))
			{
				F.X += FMath::Sign(Dx) * FMath::Min(Ad, 150.f * Dt); F.Y += FMath::Sign(Dy) * FMath::Min(FMath::Abs(Dy), 60.f * Dt);
				F.X = FMath::Clamp(F.X, LockX + 40, LockX + W - 40); F.Y = FMath::Clamp(F.Y, FT, FB);
				F.SigTick -= Dt;
				if (F.SigTick <= 0 && FMath::Sqrt(Dx * Dx + 4 * Dy * Dy) < 120 && P.Z < 40) { F.SigTick = 0.38f; HurtPlayer(7, F.T > 1.0f, F); }
				if (F.T > 1.4f) { F.St = TEXT("dizzy"); F.T = 0; UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL boss dizzy")); }
				continue;
			}
			if (F.St == TEXT("dizzy")) { if (F.T > 1.6f) { F.St = TEXT("walk"); F.T = 0; F.Cd = 0.5f; F.SlamCd = (F.Hp < F.Max / 2 ? 3.2f : 5.f) + Rand() * 1.6f; } continue; }
			F.SlamCd -= Dt;
			if (F.SlamCd <= 0 && bFree && Ad < 420)
			{
				F.St = TEXT("whirltell"); F.T = 0; F.Dir = Dx > 0 ? 1 : -1;
				if (!bSigSeen) { bSigSeen = true; Banner = TEXT("DISCO WHIRLWIND"); BannerSub = TEXT("STAY OUT OF HIS SPOTLIGHT · HIT HIM WHEN HE IS DIZZY"); BannerT = 0; }
				continue;
			}
		}
		// THE PIT BOSS's special (brawl.js 'gun'): he draws (the red line along his lane follows Nova's lane for 0.75 s and
		// pulls him onto it), the line LOCKS and blinks (0.35 s), then three shots 0.22 s apart down that lane. Step off it.
		// A hit while he draws or fires knocks him out of the move.
		if (F.bBoss && IsLucky())
		{
			if (F.St == TEXT("draw"))
			{
				F.Dir = Dx > 0 ? 1 : -1;
				if (F.T < 0.75f) { F.SigY += (P.Y - F.SigY) * FMath::Min(1.f, Dt * 4); F.Y += (F.SigY - F.Y) * FMath::Min(1.f, Dt * 6); }
				if (F.T > 1.1f) { F.St = TEXT("fire"); F.T = 0; F.Shots = 0; UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL boss revolver lane=%.0f nova=%.0f"), F.SigY, P.Y); }
				continue;
			}
			if (F.St == TEXT("fire"))
			{
				if (F.Shots < 3 && F.T > F.Shots * 0.22f)
				{
					F.Shots++;
					FFTBrawlShot B; B.X = F.X + F.Dir * 70; B.Y = F.SigY; B.Z = 120; B.Vx = F.Dir * 980; B.Dmg = 11;
					Shots.Add(B); Shake = FMath::Min(1.f, Shake + 0.2f); MuzzleT = 0.07f;
				}
				if (F.T > 0.9f) { F.St = TEXT("walk"); F.T = 0; F.Cd = 0.7f; F.SlamCd = (F.Hp < F.Max / 2 ? 3.2f : 5.f) + Rand() * 1.6f; }
				continue;
			}
			F.SlamCd -= Dt;
			if (F.SlamCd <= 0 && bFree && Ad > 60 && Ad < 760 && F.X > LockX + 90 && F.X < LockX + W - 90)   // on screen (brawl.js f.inside)
			{
				F.St = TEXT("draw"); F.T = 0; F.Dir = Dx > 0 ? 1 : -1; F.SigY = F.Y;
				if (!bSigSeen) { bSigSeen = true; Banner = TEXT("THE REVOLVER"); BannerSub = TEXT("WHEN THE RED LINE LOCKS ON, STEP OFF IT"); BannerT = 0; }
				continue;
			}
		}
		if (F.bBoss && IsN47())
		{
			bool bDone = false;
			BossSig47(F, Dt, bDone);
			if (bDone) continue;
		}
		if (F.bBoss && !IsVelvet() && !IsLucky() && !IsN47())
		{
			if (F.St == TEXT("slamwind")) { if (F.T > 0.55f) { F.St = TEXT("slam"); F.T = 0; Shake = 1.f;
				for (int32 d = 0; d < 3; d++) Splat(World(F.X + (Rand() - 0.5f) * 200, F.Y + (Rand() - 0.5f) * 40, 0), 0.4f);
				if (FMath::Abs(P.X - F.X) < 240 && FMath::Abs(P.Y - F.Y) < 90 && P.Z < 12) { P.Inv = 0; HurtPlayer(16, true, F); }
				UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL boss slam")); } continue; }
			if (F.St == TEXT("slam")) { if (F.T > 0.75f) { F.St = TEXT("walk"); F.T = 0; F.Cd = 0.5f; F.SlamCd = F.Hp < F.Max / 2 ? 3.2f : 5.0f; } continue; }
			F.SlamCd -= Dt;
			if (F.SlamCd <= 0 && bFree && Ad < 250 && FMath::Abs(Dy) < 80) { F.St = TEXT("slamwind"); F.T = 0; F.Dir = Dx > 0 ? 1 : -1; continue; }
		}
		// the boss's rush: "!" for 0.7 s, then he charges straight across
		if (F.bBoss)
		{
			if (F.St == TEXT("aim")) { if (F.T > 0.55f) { F.St = TEXT("charge"); F.T = 0; F.Vx = F.Dir * 620; } continue; }
			if (F.St == TEXT("charge"))
			{
				F.X += F.Vx * Dt;
				if (FMath::Abs(P.X - F.X) < 50 && FMath::Abs(P.Y - F.Y) < 34 && P.Z < 30) HurtPlayer(BossRush, true, F);
				const float Lo = LockX + 40, Hi = LockX + W - 40;
				if (F.T > 1.4f || F.X < Lo || F.X > Hi) { F.X = FMath::Clamp(F.X, Lo, Hi); F.St = TEXT("walk"); F.T = 0; F.Cd = 0.9f; F.Charge = F.Hp < F.Max / 2 ? 2.6f : 4.2f; }
				continue;
			}
			F.Charge -= Dt;
			if (!F.bCalled && F.Hp < F.Max / 2)
			{
				F.bCalled = true; Banner = BossCall; BannerSub = TEXT(""); BannerT = 0;
				const float Save = LockX;
				static const TCHAR* Backup[] = { TEXT("rusher"), TEXT("grappler"), TEXT("thrower") };
				static const TCHAR* Security[] = { TEXT("guard"), TEXT("guard"), TEXT("rusher") };   // Vinnie: "SECURITY!"
				static const TCHAR* Doors[] = { TEXT("guard"), TEXT("dealer"), TEXT("guard") };      // the Pit Boss: "CLOSE THE DOORS."
				for (int32 c = 0; c < 3; c++)
				{
					static const TCHAR* Help47[4][3] = { { TEXT("thrower"), TEXT("rusher"), TEXT("thrower") },   // Frankie's volunteers (drunks)
						{ TEXT("rusher"), TEXT("grappler"), TEXT("rusher") }, { TEXT("rusher"), TEXT("grappler"), TEXT("heavy") },   // the league, Sven's linemates
						{ TEXT("guard"), TEXT("guard"), TEXT("heavy") } };                                                // the Commodore's crew
					FFTBrawlBody D = MakeFoe(900 + c, IsN47() ? Help47[Night - 4][c] : IsLucky() ? Doors[c] : IsVelvet() ? Security[c] : Backup[c], Save + (c % 2 ? -60 : W + 60), FT + 30 + c * 70);
					D.Dir = c % 2 ? 1 : -1; D.Cd = 0.8f;
					Foes.Add(D);
				}
				UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL boss calls 3 more"));
				continue; // the array may have moved; the boss carries on next step
			}
			if (F.Charge <= 0 && (F.St == TEXT("walk") || F.St == TEXT("idle")) && Ad > 140)
			{
				F.St = TEXT("aim"); F.T = 0; F.Dir = Dx > 0 ? 1 : -1; F.Y += (P.Y - F.Y) * 0.6f;
				continue;
			}
		}
		if (F.St == TEXT("wind"))
		{
			if (F.T > K.Wind)
			{
				F.St = TEXT("swing"); F.T = 0;
				const bool bIn = Ad < K.Reach + 10 && FMath::Abs(Dy) < 28 && (Dx > 0) == (F.Dir > 0);
				if (FighterRole == TEXT("grappler") && bIn && P.Z < 10 && P.Inv <= 0 && (P.St == TEXT("idle") || P.St == TEXT("walk") || P.St == TEXT("punch") || P.St == TEXT("hurt")))
				{ F.St = TEXT("grab"); F.T = 0; F.Hold = 0; F.bStruck = false; P.St = TEXT("held"); P.T = 0; P.Mash = 0; P.Combo = 0; UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL grabbed")); continue; }
				const bool bAlt = F.Combo == 1 && (FighterRole == TEXT("guard") || F.bBoss);   // the nightstick / Vinnie's high kick
				if (bIn && P.Z < 40) HurtPlayer(bAlt ? K.Dmg + 2 : K.Dmg, FighterRole == TEXT("heavy") || (bAlt && FighterRole == TEXT("guard")) || (F.bBoss && Rand() < (bAlt ? 0.6f : 0.35f)), F);
			}
			continue;
		}
		if (F.St == TEXT("swing")) { if (F.T > 0.22f) { F.St = TEXT("walk"); F.T = 0; F.Cd = (F.bBoss ? 0.5f : 0.6f) + Rand() * 0.6f; } continue; }
		// walking: two at most step up to swing; the rest hang back and circle
		F.Cd -= Dt;
		const bool Want = Engaged < 3 || F.bBoss;
		const float Tx = P.X - (Dx > 0 ? 1 : -1) * (Want ? K.Reach - 12 : 150), Ty = P.Y + (Want ? 0 : (F.Id % 2 ? -50 : 50));
		const float Mx = Tx - F.X, My = Ty - F.Y, D = FMath::Sqrt(Mx * Mx + My * My);
		F.Dir = Dx > 0 ? 1 : -1;
		if (D > 8) { F.X += Mx / D * K.Sp * Dt; F.Y += My / D * K.Sp * 0.7f * Dt; F.Walk += Dt * 1.1f; F.St = TEXT("walk"); } else F.St = TEXT("idle");
		F.Y = FMath::Clamp(F.Y, FT, FB);
		if (Want && F.Cd <= 0 && Ad < K.Reach + 6 && FMath::Abs(Dy) < 22) { F.St = TEXT("wind"); F.T = 0; Engaged++; if (FighterRole == TEXT("guard") || F.bBoss) F.Combo = (F.Combo + 1) % 2; }
	}
	KeepOnScreen();
}

// keep every foe on screen (owner, 15 Nov 2026: a bottle thrower backed off camera and you punched blind): once a foe has come
// onto the locked screen, nothing (his own walk, a charge, a knockback, a throw) takes his middle past KeepIn from its edges
void AFTBrawlGameMode::KeepOnScreen()
{
	if (bNoKeepIn || LockX < 0) return;
	const float Lo = LockX + KeepIn, Hi = LockX + W - KeepIn;
	for (FFTBrawlBody& F : Foes)
	{
		if (F.St == TEXT("dead") || F.St == TEXT("lifted")) continue;   // Nova carries a lifted one (and Nova stays on screen)
		if (!F.bIn) { F.bIn = F.X >= Lo && F.X <= Hi; continue; }
		if (F.X < Lo || F.X > Hi) { F.X = FMath::Clamp(F.X, Lo, Hi); if (F.St == TEXT("fly") || F.St == TEXT("hurled")) F.Vx = 0; }
	}
}

// Track actual camera entry in normal play too: an incoming foe can become visible
// before reaching KeepIn. -brawlcamcheck additionally reports any subsequent loss
// using the real camera position, rotation and current horizontal field of view.
void AFTBrawlGameMode::CheckOnCamera()
{
	if (!CamActor || LockX < 0 || bWon) return;
	const FVector C = CamActor->GetActorLocation(); const FRotator R = CamActor->GetActorRotation();
	const float Tan = FMath::Tan(FMath::DegreesToRadians(CamActor->GetCameraComponent()->FieldOfView * 0.5f));
	if (bCamCheck) CamFrames++;
	for (FFTBrawlBody& F : Foes)
	{
		if (F.St == TEXT("dead") || F.St == TEXT("lifted")) continue;
		const FVector V = R.UnrotateVector(World(F.X, F.Y, F.Z + 60) - C);   // x ahead, y right
		const float Ndc = V.X > 1 ? V.Y / (V.X * Tan) : 9.f;               // -1..1 = on screen
		if (FMath::Abs(Ndc) <= 0.97f) { F.bSeen = true; continue; }
		if (!F.bSeen || !bCamCheck) continue;                                 // still walking on, or diagnostics disabled
		CamOff++; CamWorst = FMath::Max(CamWorst, FMath::Abs(Ndc));
		UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL OFFCAM %s%s st=%s x=%.0f lock=%.0f ndc=%.2f"), F.bBoss ? TEXT("boss ") : TEXT(""), *F.K.Role.ToString(), *F.St.ToString(), F.X - LockX, LockX, Ndc);
	}
}

// ---------------------------------------------------------------- the flow: walk -> lock -> wave -> GO -> ... -> boss -> won
void AFTBrawlGameMode::Step(float Dt, const FIn& In)
{
	StT += Dt; Time += Dt;
	if (BannerT >= 0) { BannerT += Dt; if (BannerT > 2.4f) BannerT = -1; }
	StepFx(Dt);
	TickCombatPresentation(Dt);
	if (ContactPause > 0.f) { ContactPause = FMath::Max(0.f, ContactPause - Dt); return; }
	if (Shake > 0) Shake = FMath::Max(0.f, Shake - Dt * 1.8f);
	for (FFTBrawlPop& E : Pops) E.T += Dt;
	while (Pops.Num() && Pops[0].T > 1) Pops.RemoveAt(0);
	if (bWon) { WinT += Dt; return; }
	if (P.Hp <= 0) { KoT += Dt; UpdateFoes(Dt); return; }
	if (BossIntro > 0) { BossIntro -= Dt; return; }
	UpdatePlayer(Dt, In);
	UpdateFoes(Dt);
	for (int32 b = Bottles.Num() - 1; b >= 0; b--)
	{
		FFTBrawlBottle& B = Bottles[b];
		B.T += Dt; B.Vz -= 1500 * Dt; B.X += B.Vx * Dt; B.Y += B.Vy * Dt; B.Z += B.Vz * Dt;
		const bool bHit = FMath::Abs(P.X - B.X) < 28 && FMath::Abs(P.Y - B.Y) < 24 && B.Z < 120 && B.Z > P.Z - 10 && P.Inv <= 0 && P.St != TEXT("down");
		if (bHit || B.Z <= 0)
		{
			if (bHit) { FFTBrawlBody From; From.X = B.X - B.Vx; HurtPlayer(9, false, From); }
			Bleed(B.X, B.Y, FMath::Max(B.Z, 2.f), B.Vx > 0 ? 1.f : -1.f, 16, 0.8f, true);   // the bottle shatters
			Bottles.RemoveAt(b);
		}
	}
	StepItems(Dt);
	StepShots(Dt);
	for (int32 i = Foes.Num() - 1; i >= 0; i--)
		if (Foes[i].St == TEXT("dead") && Foes[i].T > 1.4f && !Foes[i].bBoss) { if (Foes[i].Actor) Foes[i].Actor->Destroy(); Foes.RemoveAt(i); }
	const float Target = P.X - W * 0.4f;
	if (LockX < 0)
	{
		const float Stop = Wave < NumWaves ? NextWaveAt(Wave) : ArenaEnd();
		Cam += (FMath::Min(Target, Stop) - Cam) * FMath::Min(1.f, Dt * 8);
		Cam = FMath::Max(0.f, Cam);
		if (Cam >= Stop - 2) { Cam = Stop; LockX = Stop; if (Wave < NumWaves) SpawnWave(Wave); else SpawnBoss(); }
	}
	else if (bSpawned && !bBossOut && !Foes.ContainsByPredicate([](const FFTBrawlBody& F) { return F.St != TEXT("dead"); }))
	{
		Wave++; LockX = -1; bSpawned = false; Go = 2.0f; SaveCheckpoint(TEXT("WaveCompleted"));
		CombatCue(ECombatCue::Clear, World(P.X, P.Y, 70.f), P.Dir);
		UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL wave cleared -> %d (kos=%d)"), Wave, Kos);
	}
	if (Go > 0) Go -= Dt;
}

// ---------------------------------------------------------------- nights 4-7 (run 32365): the bosses' own moves
// As brawl.js has them ('fire', 'bowl', 'grab', 'anchor'): a tell the floor shows first, then the move. A light jab doesn't
// stop a tell (HitFoe); a kick or the uppercut does. bDone = the boss is busy with it this step.
void AFTBrawlGameMode::BossSig47(FFTBrawlBody& F, float Dt, bool& bDone)
{
	const float Dx = P.X - F.X, Dy = P.Y - F.Y, Ad = FMath::Abs(Dx);
	const bool bFree = F.St == TEXT("walk") || F.St == TEXT("idle");
	auto After = [&]() { F.St = TEXT("walk"); F.T = 0; F.Cd = 0.6f; F.SlamCd = (F.Hp < F.Max / 2 ? 3.2f : 5.f) + Rand() * 1.6f; };
	auto Banner1 = [&](const TCHAR* N, const TCHAR* Hint) { if (!bSigSeen) { bSigSeen = true; Banner = N; BannerSub = Hint; BannerT = 0; } };
	bDone = true;
	if (Night == 4)
	{
		// FLAMBE FRANKIE, FIRE BREATH: a swig (0.85 s; he steps onto Nova's lane for 0.45 s, then the cone on the floor
		// locks and blinks), then the flame down his line for 0.9 s, 330 px long; it leaves the floor burning for 3 s
		if (F.St == TEXT("swig"))
		{
			F.Dir = Dx > 0 ? 1 : -1;
			if (F.T < 0.45f) { F.SigY += (P.Y - F.SigY) * FMath::Min(1.f, Dt * 4); F.Y += (F.SigY - F.Y) * FMath::Min(1.f, Dt * 6); }
			if (F.T > 0.85f) { F.St = TEXT("breathe"); F.T = 0; F.bStruck = false; F.Hold = 0; UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL boss fire breath lane=%.0f nova=%.0f"), F.SigY, P.Y); }
			return;
		}
		if (F.St == TEXT("breathe"))
		{
			const float Reach = FMath::Min(330.f, 60.f + F.T * 900.f);
			const float Ahead = (P.X - F.X) * F.Dir;
			if (!F.bStruck && Ahead > 10 && Ahead < Reach && FMath::Abs(P.Y - F.SigY) < 30 && P.Z < 70) { F.bStruck = true; HurtPlayer(13, true, F); UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL fire hit nova hp=%.0f"), P.Hp); }
			while (F.Hold * 40.f + 70.f < Reach && F.Hold < 8) { Fires.Add({ F.X + F.Dir * (70.f + F.Hold * 40.f), F.SigY, 0.f }); F.Hold += 1; }
			Shake = FMath::Max(Shake, 0.15f);
			if (F.T > 0.9f) After();
			return;
		}
		F.SlamCd -= Dt;
		if (F.SlamCd <= 0 && bFree && Ad > 60 && Ad < 360 && FMath::Abs(Dy) < 80 && F.X > LockX + 60 && F.X < LockX + W - 60)
		{ F.St = TEXT("swig"); F.T = 0; F.Dir = Dx > 0 ? 1 : -1; F.SigY = F.Y; Banner1(TEXT("FIRE BREATH"), TEXT("HE TAKES A SWIG, THEN BREATHES DOWN HIS LINE: STEP OFF IT")); return; }
	}
	else if (Night == 5)
	{
		// THE KINGPIN, STRIKE!: he lines up (1.0 s): three lanes light up across the floor, one of them Nova's; then three
		// balls roll down them, 0.12 s apart. Jump them or get off the lanes.
		if (F.St == TEXT("hold")) { F.Dir = Dx > 0 ? 1 : -1; if (F.T > 1.0f) { F.St = TEXT("bowl"); F.T = 0; F.Shots = 0; UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL boss strike lanes=%.0f,%.0f,%.0f nova=%.0f"), Lanes[0], Lanes[1], Lanes[2], P.Y); } return; }
		if (F.St == TEXT("bowl"))
		{
			if (F.Shots < 3 && F.T > F.Shots * 0.12f)
			{
				FFTBrawlShot B; B.bBall = true; B.X = F.X + F.Dir * 50; B.Y = Lanes[F.Shots]; B.Z = 0; B.Vx = F.Dir * 700; B.Dmg = 12;
				Shots.Add(B); F.Shots++;
			}
			if (F.T > 0.7f) After();
			return;
		}
		F.SlamCd -= Dt;
		if (F.SlamCd <= 0 && bFree && Ad > 160 && Ad < 760 && F.X > LockX + 60 && F.X < LockX + W - 60)
		{
			F.St = TEXT("hold"); F.T = 0; F.Dir = Dx > 0 ? 1 : -1;
			const float Gap = 68.f, C = FMath::Clamp(P.Y + (Rand() < 0.5f ? -Gap : Gap) * (Rand() < 0.5f ? 1 : 0), FT + Gap, FB - Gap);
			Lanes[0] = C - Gap; Lanes[1] = C; Lanes[2] = C + Gap;
			Banner1(TEXT("STRIKE!"), TEXT("THREE LANES LIGHT UP: JUMP THE BALLS OR GET OFF THE LANES"));
			return;
		}
	}
	else if (Night == 6)
	{
		// BIG SVEN, BEAR HUG: he drops the gloves (0.75 s; his line on the floor finds Nova's lane, then locks), lunges
		// down it (0.75 s); caught = held in the hug: a squeeze every 0.6 s, mash J to break out (6 presses), then he's
		// wide open (dizzy, hits x1.5). Three squeezes and he throws Nova down.
		if (F.St == TEXT("gloves"))
		{
			F.Dir = Dx > 0 ? 1 : -1;
			if (F.T < 0.45f) { F.SigY += (P.Y - F.SigY) * FMath::Min(1.f, Dt * 4); F.Y += (F.SigY - F.Y) * FMath::Min(1.f, Dt * 6); }
			if (F.T > 0.75f) { F.St = TEXT("lunge"); F.T = 0; F.SigX = F.X; UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL boss bear hug lunge lane=%.0f nova=%.0f"), F.SigY, P.Y); }
			return;
		}
		if (F.St == TEXT("lunge"))
		{
			F.X += F.Dir * 620 * Dt; F.Y = F.SigY;
			if (FMath::Abs(P.X - F.X) < 54 && FMath::Abs(P.Y - F.Y) < 26 && P.Z < 20 && P.Inv <= 0 && P.Hp > 0 && P.St != TEXT("down"))
			{
				F.St = TEXT("hug"); F.T = 0; F.Hold = 0; F.bStruck = false; P.St = TEXT("held"); P.T = 0; P.Mash = 0; P.Combo = 0;
				if (Held >= 0) { DropItem(Held, P.X, P.Y, HeldUses, true, false); Held = -1; HeldUses = 0; }
				UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL boss bear hug caught"));
				return;
			}
			if (F.T > 0.75f || F.X < LockX + 40 || F.X > LockX + W - 40) { F.X = FMath::Clamp(F.X, LockX + 40, LockX + W - 40); After(); }
			return;
		}
		if (F.St == TEXT("hug"))
		{
			if (P.St != TEXT("held")) { After(); return; }
			P.X = F.X + F.Dir * 46; P.Y = F.Y + 2; P.Dir = -F.Dir; P.Z = 0; P.T = 0;
			if (P.Mash >= 6)
			{
				P.St = TEXT("idle"); P.Inv = 0.6f; F.St = TEXT("dizzy"); F.T = 0; F.X -= F.Dir * 30;
				Pop(F.X, F.Y, 200, TEXT("BROKE FREE!"), FLinearColor(0.2f, 0.95f, 1.f), true);
				UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL boss bear hug broken mash=%d"), P.Mash);
				return;
			}
			if (F.T > 0.6f)
			{
				F.T = 0; F.Hold += 1; P.Hp -= 7; Hits++; Chain = 0; Shake = FMath::Min(1.f, Shake + 0.5f); Sfx(TEXT("novahurt"), 0.85f, 0.95f); Rumble(0.65f, 0.2f, TEXT("hug"));
				Bleed(P.X, P.Y, 140, -F.Dir, 10, 0.7f);
				if (F.Hold >= 3 || P.Hp <= 0) { P.St = TEXT("idle"); P.Inv = 0; HurtPlayer(8, true, F); After(); UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL boss bear hug throw hp=%.0f"), P.Hp); }
			}
			return;
		}
		if (F.St == TEXT("dizzy")) { if (F.T > 1.6f) After(); return; }
		F.SlamCd -= Dt;
		if (F.SlamCd <= 0 && bFree && Ad > 120 && Ad < 440 && F.X > LockX + 60 && F.X < LockX + W - 60)
		{ F.St = TEXT("gloves"); F.T = 0; F.Dir = Dx > 0 ? 1 : -1; F.SigY = F.Y; Banner1(TEXT("BEAR HUG"), TEXT("GET OFF HIS LANE WHEN HE DROPS THE GLOVES · CAUGHT? MASH PUNCH")); return; }
	}
	else if (Night == 7)
	{
		// THE COMMODORE, ANCHOR SWEEP: he winds the anchor overhead (0.9 s), a wide ring shows round him on the deck; then
		// it sweeps low through the ring (0.4 s): anyone in it and not in the air goes down. Jump it.
		if (F.St == TEXT("windup")) { F.Dir = Dx > 0 ? 1 : -1; if (F.T > 0.9f) { F.St = TEXT("sweep"); F.T = 0; F.bStruck = false; UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL boss anchor sweep nova z=%.0f"), P.Z); } return; }
		if (F.St == TEXT("sweep"))
		{
			const float Ex = Dx / 200.f, Ey = Dy / 100.f;
			if (!F.bStruck && F.T > 0.08f) { F.bStruck = true; Shake = FMath::Min(1.f, Shake + 0.4f); if (Ex * Ex + Ey * Ey < 1.f && P.Z < 30) { P.Inv = 0; HurtPlayer(15, true, F); UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL anchor hit nova hp=%.0f"), P.Hp); } }
			if (F.T > 0.45f) After();
			return;
		}
		F.SlamCd -= Dt;
		if (F.SlamCd <= 0 && bFree && Ad < 230 && FMath::Abs(Dy) < 90)
		{ F.St = TEXT("windup"); F.T = 0; F.Dir = Dx > 0 ? 1 : -1; Banner1(TEXT("ANCHOR SWEEP"), TEXT("HE WINDS THE ANCHOR OVERHEAD: JUMP THE SWEEP")); return; }
	}
	bDone = false;
}

// bullets and cards fly flat down a lane; they hit Nova if he is on it (|dy| < 18) and not jumped clear
void AFTBrawlGameMode::StepShots(float Dt)
{
	if (MuzzleT > 0) MuzzleT -= Dt;
	for (int32 i = Shots.Num() - 1; i >= 0; i--)
	{
		FFTBrawlShot& S = Shots[i];
		S.T += Dt; S.X += S.Vx * Dt;
		const bool bHit = FMath::Abs(S.X - P.X) < (S.bBall ? 26.f : 22.f) && FMath::Abs(S.Y - P.Y) < (S.bBall ? 20.f : 18.f) && P.Z < (S.bBall ? 34.f : S.bCard ? 60.f : 90.f) && P.Inv <= 0 && P.St != TEXT("down") && P.Hp > 0;
		if (bHit)
		{
			FFTBrawlBody From; From.X = S.X - S.Vx;
			HurtPlayer(S.Dmg, S.bBall, From);   // a bowling ball takes his legs out
			UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL %s hit nova hp=%.0f"), S.bBall ? TEXT("ball") : S.bCard ? TEXT("card") : TEXT("bullet"), P.Hp);
		}
		const float Lo = (LockX >= 0 ? LockX : Cam) - 80, Hi = (LockX >= 0 ? LockX : Cam) + W + 80;
		if (bHit || S.T > (S.bBall ? 2.4f : 1.6f) || S.X < Lo || S.X > Hi) Shots.RemoveAt(i);
	}
	// Frankie's burning floor: a patch under Nova's feet singes him (no knockdown)
	for (int32 i = Fires.Num() - 1; i >= 0; i--)
	{
		FFTBrawlFire& Fr = Fires[i];
		Fr.T += Dt;
		if (Fr.T > 3.f) { Fires.RemoveAt(i); continue; }
		if (Fr.T > 0.2f && FMath::Abs(Fr.X - P.X) < 24 && FMath::Abs(Fr.Y - P.Y) < 16 && P.Z < 6 && P.Inv <= 0 && P.Hp > 0 && P.St != TEXT("down") && P.St != TEXT("held"))
		{ FFTBrawlBody From; From.X = Fr.X - P.Dir; HurtPlayer(4, false, From); UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL burned hp=%.0f"), P.Hp); }
	}
}

void AFTBrawlGameMode::Retry()
{
	// back on his feet, full health, the wave he was on again (a boss fight starts the boss again)
	const int32 Wv = Wave; const bool bBoss = bBossOut; const float C = LockX >= 0 ? LockX : Cam;
	ResetState(Wv, true);
	Cam = C; LockX = -1; P.X = C + 200;
	Banner = TEXT("ROUND TWO"); BannerSub = TEXT(""); BannerT = 0;
	if (bBoss) Wave = NumWaves;
	UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL retry wave %d"), Wave);
}

// after a night: the main game's career goes back to the races (FTNightFlow, full-throttle-unreal only); the brawl on its own
// goes on to the next night's map, after night 7 it fights night 7 again
void AFTBrawlGameMode::GoOn()
{
#if FTB_CAREER
	if (UFTNightFlow::NightOver(this)) { UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL on to the next race")); return; }
#endif
	const TCHAR* Next = NightMap(Night + 1);
	// run 77694: the next day's BEFORE THE DRIVE and THE WRAP on the Route 9 set come first (-brawlnodays: night to night)
	if (!bDay && Next && !FParse::Param(FCommandLine::Get(), TEXT("brawlnodays")) && FPackageName::DoesPackageExist(TEXT("/Game/Brawl/Maps/L_Day1Set")))
	{
		UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL on to day %d"), Night + 1);
		UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/Brawl/Maps/L_Day1Set")), true, FString::Printf(TEXT("brawlday=%d"), Night + 1));
	}
	else if (!bDay && Next && FPackageName::DoesPackageExist(Next)) { UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL on to night %d"), Night + 1); UGameplayStatics::OpenLevel(this, FName(Next)); }
	else { if (InCut()) EndCut(); Restart(); }
}

FString AFTBrawlGameMode::NextLabel() const
{
#if FTB_CAREER
	if (UFTNightFlow::InCareer(this)) return UFTNightFlow::NextDay(this);
#endif
	const TCHAR* Next = NightMap(Night + 1);
	if (Next && !FParse::Param(FCommandLine::Get(), TEXT("brawlnodays")) && FPackageName::DoesPackageExist(TEXT("/Game/Brawl/Maps/L_Day1Set"))) return FString::Printf(TEXT("DAY %d"), Night + 1);
	return Next && FPackageName::DoesPackageExist(Next) ? NextVenue : FString();
}

void AFTBrawlGameMode::Restart()
{
	bCutDone = false;
	ResetState(0, false);
	SaveCheckpoint(TEXT("RestartNight"));
	Banner = VenueName; BannerSub = VenueTag; BannerT = 0;
}

void AFTBrawlGameMode::Tick(float Dt)
{
	Super::Tick(Dt);
	if (SystemScreen || FPlatformTime::Seconds() < InputResumeAt) return;
#ifdef FT_BRAWL_MAIN_GAME
	// loop cycle 6: M mutes here too, as in the races (the same setting, saved)
	if (APlayerController* MPC = GetWorld()->GetFirstPlayerController())
		if (MPC->WasInputKeyJustPressed(EKeys::M))
			if (UFTGameInstance* GI = GetGameInstance<UFTGameInstance>()) { GI->bMuted = !GI->bMuted; GI->ApplyMute(); GI->SaveSettings(); UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL mute=%d"), GI->bMuted ? 1 : 0); }
#endif
	// (3.9: cleared at console priority: a SetByCode 0 never overrode the console's 1, so the night re-won every frame
	// and the win screen's clock never ran)
	if (CVarWin.GetValueOnGameThread()) { CVarWin->Set(0, ECVF_SetByConsole); bWon = true; WinT = 0; SaveCheckpoint(TEXT("BossDefeatedTest")); UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL won (ftb.Win)")); }
	if (bKeyTest)   // -brawlkeytest: the autopilot's moves go in as key presses (in a cutscene: ENTER on each beat after 3 s with -brawlnext)
	{
		FIn K = InCut() ? FIn() : AutoInput();
		if (InCut()) K.Enter = CutT > 3.f && FMath::Fmod(CutT, 0.5f) < 0.25f && FParse::Param(FCommandLine::Get(), TEXT("brawlnext"));
		PressKeys(K);
	}
	const bool bBot = bAuto || CVarAuto.GetValueOnGameThread();
	FIn In = bBot ? AutoInput() : ReadInput();
	// 3.9 (the boss music runs got stuck here): with the autopilot fighting, the cutscenes were handed no input at all and
	// the win / K.O. screens only the autopilot's, so a real ENTER, click, pad A or skip never reached them. The player's
	// own keys (and the harness's ft.Press / ft.Click, which go through the same input) now always work the screens.
	const FIn Real = bBot ? ReadInput() : In;
	const bool bBotScreenEnter = bBot && In.Enter;   // only the autopilot's own confirmation may be held
	In.Enter = In.Enter || Real.Enter;
	BufferInput(In);
	const bool bScreenEnter = Real.Enter && !bScreenEnterPrev;
	bScreenEnterPrev = Real.Enter;
	if (InCut())   // night 1's cutscenes play instead of the fight (FTBrawlCutscene.cpp)
	{
		StepCut(FMath::Min(Dt, 0.05f), Real);   // the autopilot's punches don't hurry the lines
		if (InCut()) SyncCut(FMath::Min(Dt, 0.05f)); else SyncActors(Dt);
	}
	else if (P.Hp <= 0 && KoT > 1 && (bScreenEnter || bBotScreenEnter)) Retry();
	else if (bWon && bCuts && !bCutDone && WinT > 2.2f) { bCutDone = true; StartCut(2); }   // LAST CALL, then the pier
	else if (bWon && !bCuts && WinT > 2.6f && (bScreenEnter || bBotScreenEnter))
	{
		UE_LOG(LogFTBrawl, Log, TEXT("FT BRAWL screen=win key=enter result=on"));
		GoOn();
	}
	if (!InCut())
	{
		Acc += FMath::Min(0.05f, Dt);
		int32 N = 0;
		while (Acc >= STEP && N < 4) { Step(STEP, In); Acc -= STEP; N++; }
		SyncActors(Dt);
	}
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		if (CamActor && PC->GetViewTarget() != CamActor) PC->SetViewTarget(CamActor);
	PlaceMoon();
	TickVenueAmbience(Dt);
	// test hook: -brawlshots=<abs dir> saves the game view every 4 s as <dir>/shot_NN.png (look checks without an editor)
	static FString ShotDir; static bool bShotInit = false; static float ShotT = 0; static int32 ShotN = 0;
	static float ShotEvery = 4.f;
	if (!bShotInit) { bShotInit = true; FParse::Value(FCommandLine::Get(), TEXT("brawlshots="), ShotDir); FParse::Value(FCommandLine::Get(), TEXT("brawlshotdt="), ShotEvery); }
	if (!ShotDir.IsEmpty() && (ShotT += Dt) >= ShotEvery)
	{
		ShotT = 0;
		const FString Path = FString::Printf(TEXT("%s/shot_%03d.png"), *ShotDir, ++ShotN);
		FScreenshotRequest::RequestScreenshot(Path, false, false);
		UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL shot %s"), *Path);
	}
}

// ---------------------------------------------------------------- 3D: bodies and the camera follow the simulation
void AFTBrawlGameMode::SyncActors(float RealDt)
{
	auto Sync = [this](FFTBrawlBody& F)
	{
		if (!F.Actor) return;
		float Lean = 0;
		const bool bDown = F.St == TEXT("down") || F.St == TEXT("dead");
		if (bDown) Lean = 90;
		else if (F.St == TEXT("fly")) Lean = FMath::Min(75.f, F.T * 230.f);
		else if (F.St == TEXT("getup")) Lean = 90 * (1 - F.T / 0.45f);
		else if (F.St == TEXT("hurt")) Lean = 14;
		else if (F.St == TEXT("wind")) Lean = 7;
		else if (F.St == TEXT("swing") || F.St == TEXT("punch") || F.St == TEXT("kick")) Lean = -10;
		else if (F.St == TEXT("charge")) Lean = -20;
		bool bVis = true;
		if (F.St == TEXT("dead")) bVis = F.Actor->HasSprite() ? (F.T < 0.5f || (int32(F.T * 14) % 2 == 0 && F.T < 1.3f))   // lies there, blinks out
			: F.Actor->HasCast() ? F.T < 1.3f : (int32(F.T * 14) % 2 == 0 && F.T < 0.8f);
		if (F.St == TEXT("dead") && ReducedPresentationMotion()) bVis = F.T < 1.3f;
		FName St = F.St;
		if (F.bPlayer && St == TEXT("walk") && SustainedTravel >= 0.24f) St = TEXT("run");
		float PoseTime = F.T;
		if (F.bBoss) StyleBossPose(St, PoseTime);
		if (F.bPlayer && bWon) St = TEXT("cheer");
		if (F.bPlayer && F.Inv > 0 && F.St != TEXT("down")) bVis = ReducedPresentationMotion() || int32(StT * 16) % 2 == 0;
		F.Actor->SetActorLocationAndRotation(World(F.X, F.Y, F.Z), FRotator(0, F.Dir > 0 ? 0 : 180, 0));
		F.Actor->SetActorScale3D(FVector(F.Actor->HasCast() ? 1.f : F.S / 2.05f));
		F.Actor->Pose(St, PoseTime, F.Walk, F.Combo, Lean, F.Flash > 0 ? (ReducedPresentationMotion() ? 0.16f : 0.7f) : 0.f, bVis);
	};
	Sync(P);
	for (FFTBrawlBody& F : Foes) Sync(F);
	SyncBossAccent();
	SyncItems();
	// blood drops, glass and bottles in flight
	if (DropIsm && GlassIsm)
	{
		DropIsm->ClearInstances(); GlassIsm->ClearInstances(); if (WoodIsm) WoodIsm->ClearInstances();
		TArray<FTransform> Dt0, Gt0, Wt0;
		for (const FFTBrawlDrop& D : Drops)
		{
			const FVector Sc = FVector(D.S * (1.f + FMath::Min(1.5f, D.V.Size() / 600.f)), D.S, D.S);
			(D.bWood ? Wt0 : D.bGlass ? Gt0 : Dt0).Add(FTransform(D.V.Rotation(), D.P, Sc));
		}
		if (Dt0.Num()) DropIsm->AddInstances(Dt0, false);
		if (Gt0.Num()) GlassIsm->AddInstances(Gt0, false);
		if (Wt0.Num() && WoodIsm) WoodIsm->AddInstances(Wt0, false);
	}
	// the Dealer's cards (white, spinning in the screen plane) and the Pit Boss's bullets (a short bright streak)
	if (CardIsm && SlugIsm)
	{
		CardIsm->ClearInstances(); SlugIsm->ClearInstances();
		for (const FFTBrawlShot& S : Shots)
			if (S.bCard) CardIsm->AddInstance(FTransform(FRotator(S.T * 1400.f, 0, 0), World(S.X, S.Y, S.Z), FVector(0.13f, 0.01f, 0.09f)));
			else SlugIsm->AddInstance(FTransform(FRotator::ZeroRotator, World(S.X, S.Y, S.Z), FVector(0.55f, 0.03f, 0.03f)));
	}
	if (BottleIsm)
	{
		BottleIsm->ClearInstances();
		for (const FFTBrawlBottle& B : Bottles) BottleIsm->AddInstance(FTransform(FRotator(B.T * 900.f, 0, 0), World(B.X, B.Y, B.Z), FVector(0.08f, 0.08f, 0.26f)));
	}
	// the Velvet Room: the disco spots swing over the floor; Vinnie's spotlight and ring while he whirls
	for (int32 i = 0; i < Sweeps.Num(); i++)
		if (Sweeps[i]) Sweeps[i]->SetActorRotation(FRotator(-62.f + 14.f * FMath::Sin(StT * 0.9f + i * 1.7f), -90.f + 38.f * FMath::Sin(StT * 0.7f + i * 2.3f), 0));
	if (BossSpot && BossRing)
	{
		const int32 Bi = BossIdx();
		const FFTBrawlBody* B = Bi != INDEX_NONE ? &Foes[Bi] : nullptr;
		const bool bOn = B && (B->St == TEXT("whirltell") || B->St == TEXT("whirl"));
		BossSpot->SpotLightComponent->SetVisibility(bOn);
		BossRing->SetActorHiddenInGame(!bOn);
		if (bOn)
		{
			const FVector At = World(B->X, B->Y, 0);
			BossSpot->SetActorLocation(At + FVector(0, 120.f, 640.f));
			BossSpot->SetActorRotation(FRotator(-80.f, -90.f, 0));
			const bool bTell = B->St == TEXT("whirltell");
			const float Pulse = bTell ? (int32(StT * 10) % 2 ? 1.f : 0.45f) : 0.7f;
			BossRing->SetActorLocation(FVector(At.X, At.Y, 2.f));
			if (UMaterialInstanceDynamic* M = Cast<UMaterialInstanceDynamic>(BossRing->GetStaticMeshComponent()->GetMaterial(0))) M->SetScalarParameterValue(TEXT("Glow"), 3.f * Pulse);
		}
	}
	// the Lucky 7: the Pit Boss's red line while he draws (follows Nova's lane, then locks and blinks), the muzzle flash
	if (LaserFloor && LaserBeam)
	{
		const int32 Bi = BossIdx();
		const FFTBrawlBody* B = Bi != INDEX_NONE ? &Foes[Bi] : nullptr;
		const bool bOn = B && B->St == TEXT("draw") && B->T > 0.15f;
		LaserFloor->SetActorHiddenInGame(!bOn); LaserBeam->SetActorHiddenInGame(!bOn);
		if (bOn)
		{
			const bool bLock = B->T > 0.75f;
			const float X0 = B->X + B->Dir * 70, X1 = B->Dir > 0 ? (LockX >= 0 ? LockX : Cam) + W + 40 : (LockX >= 0 ? LockX : Cam) - 40;
			const float Len = FMath::Abs(X1 - X0) * PX, Xm = (X0 + X1) / 2;
			const float Blink = ReducedPresentationMotion() ? (bLock ? 0.85f : 0.55f) : bLock ? (int32(StT * 16) % 2 ? 1.f : 0.35f) : 0.55f;
			LaserFloor->SetActorLocation(World(Xm, B->SigY, 0) + FVector(0, 0, 2.f));
			LaserFloor->SetActorScale3D(FVector(Len / 100.f, 32.f * PX / 100.f, 1));
			LaserBeam->SetActorLocation(World(Xm, B->SigY, 127));
			LaserBeam->SetActorScale3D(FVector(Len / 100.f, (bLock ? 16.f : 10.f) / 100.f, 1));
			if (UMaterialInstanceDynamic* M = Cast<UMaterialInstanceDynamic>(LaserFloor->GetStaticMeshComponent()->GetMaterial(0))) M->SetScalarParameterValue(TEXT("Glow"), 6.f * Blink);
			if (UMaterialInstanceDynamic* M = Cast<UMaterialInstanceDynamic>(LaserBeam->GetStaticMeshComponent()->GetMaterial(0))) M->SetScalarParameterValue(TEXT("Glow"), 30.f * Blink);
		}
		if (Muzzle && IsLucky())
		{
			Muzzle->PointLightComponent->SetVisibility(MuzzleT > 0 && B);
			if (MuzzleT > 0 && B) Muzzle->SetActorLocation(World(B->X + B->Dir * 75, B->Y, 125) + FVector(0, 40.f, 0));
		}
	}
	// nights 4-7 (run 32365): the boss's tell on the floor, the balls, the burning patches
	if (LaneA.Num() == 3)
	{
		const int32 Bi = BossIdx();
		const FFTBrawlBody* B = Bi != INDEX_NONE ? &Foes[Bi] : nullptr;
		const float L0 = (LockX >= 0 ? LockX : Cam) - 40, L1 = (LockX >= 0 ? LockX : Cam) + W + 40;
		auto Strip = [&](int32 i, float Xa, float Xb, float Y, float Wpx, float Glow)
		{
			AStaticMeshActor* A = LaneA[i];
			A->SetActorHiddenInGame(false);
			A->SetActorLocation(World((Xa + Xb) / 2, Y, 0) + FVector(0, 0, 2.f + i * 0.2f));
			A->SetActorScale3D(FVector(FMath::Abs(Xb - Xa) * PX / 100.f, Wpx * PX / 100.f, 1));
			if (UMaterialInstanceDynamic* M = Cast<UMaterialInstanceDynamic>(A->GetStaticMeshComponent()->GetMaterial(0))) M->SetScalarParameterValue(TEXT("Glow"), Glow);
		};
		for (AStaticMeshActor* A : LaneA) A->SetActorHiddenInGame(true);
		const float Blink = ReducedPresentationMotion() ? 4.5f : int32(StT * 14) % 2 ? 7.f : 2.5f;
		if (B && Night == 4 && (B->St == TEXT("swig") || B->St == TEXT("breathe")))   // Frankie's cone: his line ahead of him
			Strip(0, B->X + B->Dir * 50, B->X + B->Dir * 340, B->SigY, B->St == TEXT("swig") ? 46.f : 60.f, B->St == TEXT("swig") ? (B->T > 0.45f ? Blink : 2.5f) : 9.f);
		if (B && Night == 5 && (B->St == TEXT("hold") || (B->St == TEXT("bowl") && B->T < 0.6f)))   // the Kingpin's three lanes
			for (int32 i = 0; i < 3; i++) Strip(i, L0, L1, Lanes[i], 30.f, B->St == TEXT("hold") ? Blink : 6.f);
		if (B && Night == 6 && (B->St == TEXT("gloves") || B->St == TEXT("lunge")))   // Sven's lane
			Strip(0, B->X, B->Dir > 0 ? L1 : L0, B->SigY, 34.f, B->St == TEXT("gloves") ? (B->T > 0.45f ? Blink : 2.5f) : 7.f);
		if (BossRing && Night == 7)   // the Commodore's ring round him
		{
			const bool bOn = B && (B->St == TEXT("windup") || (B->St == TEXT("sweep") && B->T < 0.25f));
			BossRing->SetActorHiddenInGame(!bOn);
			if (bOn)
			{
				BossRing->SetActorLocation(World(B->X, B->Y, 0) + FVector(0, 0, 2.f));
				if (UMaterialInstanceDynamic* M = Cast<UMaterialInstanceDynamic>(BossRing->GetStaticMeshComponent()->GetMaterial(0)))
					M->SetScalarParameterValue(TEXT("Glow"), B->St == TEXT("sweep") ? 8.f : (B->T > 0.5f ? (int32(StT * 14) % 2 ? 5.f : 1.5f) : 2.f));
			}
		}
		int32 Bn = 0;
		for (const FFTBrawlShot& S : Shots)
			if (S.bBall && Bn < BallA.Num())
			{
				AStaticMeshActor* A = BallA[Bn++];
				A->SetActorHiddenInGame(false);
				const float K = 34.f / 588.f;
				A->SetActorLocation(World(S.X, S.Y, 0) + FVector(0, 0, (1000.f - 512.f) * K));
				A->SetActorRotation(FRotator(-S.T * 900.f * FMath::Sign(S.Vx), 0, 90));
			}
		for (; Bn < BallA.Num(); Bn++) BallA[Bn]->SetActorHiddenInGame(true);
		int32 Fn = 0; FVector FireAt = FVector::ZeroVector;
		for (const FFTBrawlFire& Fr : Fires)
			if (Fn < FireA.Num())
			{
				AStaticMeshActor* A = FireA[Fn++];
				A->SetActorHiddenInGame(false);
				A->SetActorLocation(World(Fr.X, Fr.Y, 0) + FVector(0, 0, 3.f));
				const float Life = FMath::Clamp(3.f - Fr.T, 0.f, 1.f), Fl = ReducedPresentationMotion() ? 1.f : 0.75f + 0.25f * FMath::Sin(StT * 23.f + Fr.X);
				A->SetActorScale3D(FVector(46.f * PX / 100.f * (0.8f + 0.2f * Fl), 26.f * PX / 100.f, 1));
				if (UMaterialInstanceDynamic* M = Cast<UMaterialInstanceDynamic>(A->GetStaticMeshComponent()->GetMaterial(0))) M->SetScalarParameterValue(TEXT("Glow"), 9.f * Life * Fl);
				FireAt += A->GetActorLocation();
			}
		for (int32 i = Fn; i < FireA.Num(); i++) FireA[i]->SetActorHiddenInGame(true);
		if (FireL)
		{
			const bool bBreath = B && Night == 4 && B->St == TEXT("breathe");
			FireL->PointLightComponent->SetVisibility(bBreath || Fn > 0);
			if (bBreath) FireL->SetActorLocation(World(B->X + B->Dir * 180, B->SigY, 120));
			else if (Fn > 0) FireL->SetActorLocation(FireAt / Fn + FVector(0, 0, 60.f));
			FireL->PointLightComponent->SetIntensity((bBreath ? 60000.f : 14000.f) * (ReducedPresentationMotion() ? 1.f : 0.8f + 0.2f * FMath::Sin(StT * 31.f)));
		}
	}
	if (CamActor)
	{
		float Sx = 0, Sz = 0;
		if (Shake > 0 && !ReducedPresentationMotion()) { const float Tr = Shake * Shake * 10 * PX; Sx = FMath::Sin(StT * 61) * Tr; Sz = FMath::Cos(StT * 53) * Tr; }
		const FVector Mid = World(Cam + W / 2, (FT + FB) / 2, 0);
		// Frame the visible silhouettes, not only their feet. Nova may reach 30 px from
		// an arena edge; the old 46-degree view clipped her arms there. Incoming foes
		// still walk on naturally, then remain inside the presentation's safe area.
		float Left = P.X * PX - 155.f, Right = P.X * PX + 155.f;
		float NearDepth = 2000.f;
		const FRotator FlatView(-16.f, -90.f, 0.f);
		auto FrameBody = [&](const FFTBrawlBody& F, float HalfWidth, float Reveal)
		{
			const float BodyLeft = FMath::Lerp(Right, F.X * PX - HalfWidth, Reveal);
			const float BodyRight = FMath::Lerp(Right, F.X * PX + HalfWidth, Reveal);
			Left = FMath::Min(Left, BodyLeft);
			Right = FMath::Max(Right, BodyRight);
			const FVector V = FlatView.UnrotateVector(World(F.X, F.Y, 90.f) - FVector(Mid.X, 1450.f, CamZ));
			NearDepth = FMath::Min(NearDepth, FMath::Max(1000.f, float(V.X)));
		};
		FrameBody(P, 155.f, 1.f);
		for (const FFTBrawlBody& F : Foes)
			if ((F.bIn || F.bSeen || F.bBoss) && (F.St != TEXT("dead") || F.T < 1.3f))
			{
				// Blend the off-stage boss into the framing during the reveal, then
				// keep him framed even if he is knocked down before walking fully in.
				const float Reveal = F.bBoss && !F.bIn && !ReducedPresentationMotion() ? FMath::SmoothStep(0.f, 0.42f, StT - BossEntranceAt) : 1.f;
				FrameBody(F, F.bBoss ? 220.f : 155.f, Reveal);
			}
		const float SafeDepth = NearDepth * 0.94f;
		const float WantFov = FMath::Clamp(FMath::RadiansToDegrees(2.f * FMath::Atan((Right - Left) / (2.f * SafeDepth))), 50.f, 60.f);
		UCameraComponent* Camera = CamActor->GetCameraComponent();
		// Open immediately when necessary; ease back without zooming on every pose.
		const float Fov = FMath::Max(WantFov, FMath::FInterpTo(Camera->FieldOfView, WantFov, RealDt, 3.f));
		Camera->SetFieldOfView(Fov);
		const float HalfView = SafeDepth * FMath::Tan(FMath::DegreesToRadians(Fov * 0.5f));
		const float Lo = Right - HalfView, Hi = Left + HalfView;
		const float WantX = Mid.X + BossEntranceCameraOffset();
		const float FollowX = FMath::FInterpTo(float(CamActor->GetActorLocation().X), WantX, RealDt, 5.f);
		const float ViewX = Lo <= Hi ? FMath::Clamp(FollowX, Lo, Hi) : (Left + Right) * 0.5f;
		CamActor->SetActorLocation(FVector(ViewX + Sx, 1450.f, CamZ + Sz));
		// buzzed: the room sways a little
		const float Sway = BuzzT > 0 && !ReducedPresentationMotion() ? FMath::Sin(StT * 1.6f) * 1.8f * FMath::Min(1.f, BuzzT) : 0.f;
		CamActor->SetActorRotation(FRotator(-16.f + Sway * 0.3f, -90.f, Sway));
	}
	CheckOnCamera();
}

// ---------------------------------------------------------------- blood
void AFTBrawlGameMode::Bleed(float X, float Y, float Z, float Dir, int32 N, float Force, bool bGlass, bool bWood)
{
	const FVector O = World(X, Y, Z);
	for (int32 i = 0; i < N; i++)
	{
		FFTBrawlDrop D;
		D.P = O + FVector(0, (Rand() - 0.5f) * 20, (Rand() - 0.5f) * 20);
		const float Sp = (120 + Rand() * 520) * Force;
		D.V = FVector(Dir * Sp, (Rand() - 0.5f) * 300, 60 + Rand() * 420 * Force);
		D.S = bGlass ? 0.03f + Rand() * 0.03f : 0.035f + Rand() * 0.08f;
		D.bGlass = bGlass || bWood; D.bWood = bWood;
		if (bWood) D.S *= 1.6f;
		Drops.Add(D);
	}
	if (Drops.Num() > 600) Drops.RemoveAt(0, Drops.Num() - 600);
}

void AFTBrawlGameMode::Splat(const FVector& At, float Size)
{
	if (!SplatIsm) return;
	Size *= 0.5f;   // 2.5D: the pools read at sprite scale
	const FTransform T(FRotator(0, Rand() * 360.f, 0), FVector(At.X, At.Y, 1.2f + Rand() * 0.4f), FVector(Size * (0.7f + Rand() * 0.6f), Size * (0.4f + Rand() * 0.4f), 0.004f));
	static const int32 MaxSplats = 500;
	if (SplatIsm->GetInstanceCount() < MaxSplats) SplatIsm->AddInstance(T, true);
	else SplatIsm->UpdateInstanceTransform(SplatNext, T, true, true);
	SplatNext = (SplatNext + 1) % MaxSplats;
}

void AFTBrawlGameMode::StepFx(float Dt)
{
	for (int32 i = Drops.Num() - 1; i >= 0; i--)
	{
		FFTBrawlDrop& D = Drops[i];
		D.V.Z -= 1900 * Dt; D.P += D.V * Dt;
		if (D.P.Z <= 1.f)
		{
			if (!D.bGlass) Splat(D.P, D.S * (2.5f + Rand() * 3.f));
			Drops.RemoveAt(i);
		}
	}
}

// ---------------------------------------------------------------- the venue: The Rusty Anchor, built from basic shapes
void AFTBrawlGameMode::BuildVenue()
{
	UWorld* Wd = GetWorld();
	UMaterialInterface* BaseMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	UStaticMesh* Cube = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	UStaticMesh* Cyl = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	UStaticMesh* Sph = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	auto Box = [&](UStaticMesh* M, FVector C, FVector Size, const FLinearColor& Col, bool bShadow = true)
	{
		AStaticMeshActor* A = Wd->SpawnActor<AStaticMeshActor>(FVector(C.X, -C.Y, C.Z), FRotator::ZeroRotator);   // venue Y is written depth-positive
		A->SetMobility(EComponentMobility::Movable);
		UStaticMeshComponent* SM = A->GetStaticMeshComponent();
		SM->SetStaticMesh(M); SM->SetWorldScale3D(Size / 100.f); SM->SetCastShadow(bShadow);
		SM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UMaterialInstanceDynamic* D = UMaterialInstanceDynamic::Create(BaseMat, A);
		D->SetVectorParameterValue(TEXT("Color"), Col);
		SM->SetMaterial(0, D);
		return A;
	};
	const float X0 = -400 * PX, X1 = (ArenaEnd() + W + 400) * PX, Len = X1 - X0, Xm = (X0 + X1) / 2;
	const float Back = (FB - FT) * PX + 60;   // the back wall's front face (Y)
	// ---- 2.5D: the painted Rusty Anchor. Back wall = Codex-painted panels in a row, the plank floor, painted props
	// in front of the wall and in the foreground (the perspective camera gives the parallax), coloured neon lights
	// placed on the painted signs so the sprites pick up their spill. Falls back to the blocky bar if the art is missing.
	UMaterialInterface* PaintM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlPaint.M_BrawlPaint"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UMaterialInterface* SpriteM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlSprite.M_BrawlSprite"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UStaticMesh* PlaneM = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"));
	auto Tex = [](const TCHAR* Path) { return LoadObject<UTexture>(nullptr, Path, nullptr, LOAD_Quiet | LOAD_NoWarn); };
	// a plane: C = world centre, size in cm, Rot = how it stands (floor: flat; wall/prop: facing the camera)
	auto Quad = [&](FVector C, FVector2D Size, FRotator Rot, UMaterialInterface* M, UTexture* T, bool bShadow, TFunctionRef<void(UMaterialInstanceDynamic*)> Params)
	{
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
	auto Lamp = [&](FVector At, const TCHAR* C, float I, float R, bool bShadow = false)
	{
		APointLight* L = Wd->SpawnActor<APointLight>(At, FRotator::ZeroRotator);
		L->PointLightComponent->SetMobility(EComponentMobility::Movable);
		L->PointLightComponent->SetIntensity(I); L->PointLightComponent->SetAttenuationRadius(R); L->PointLightComponent->SetCastShadows(bShadow);
		L->PointLightComponent->SetLightColor(Hex(C));
	};
	UTexture* Floor = Tex(TEXT("/Game/Brawl/Backdrop/T_floor.T_floor"));
	UTexture* Panels[3] = { Tex(TEXT("/Game/Brawl/Backdrop/T_bg_bar.T_bg_bar")), Tex(TEXT("/Game/Brawl/Backdrop/T_bg_games.T_bg_games")), Tex(TEXT("/Game/Brawl/Backdrop/T_bg_booths.T_bg_booths")) };
	const bool bPainted = PaintM && SpriteM && PlaneM && Floor && Panels[0] && Panels[1] && Panels[2];
	if (bPainted && !bDay) BuildVenueAmbience(X0, X1, -Back);
	// the night pass (brawl_night.py): a dark dive bar lit by its neon and bar lamps, real windows onto the harbour
	UMaterialInterface* Wall2DM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlWall2D.M_BrawlWall2D"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UMaterialInterface* OutM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlOutside.M_BrawlOutside"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UTexture* Glows[3] = { Tex(TEXT("/Game/Brawl/Night/T_glow_bar.T_glow_bar")), Tex(TEXT("/Game/Brawl/Night/T_glow_games.T_glow_games")), Tex(TEXT("/Game/Brawl/Night/T_glow_booths.T_glow_booths")) };
	UTexture* BoothsWin = Tex(TEXT("/Game/Brawl/Night/T_bg_booths_win.T_bg_booths_win"));
	UTexture* OutSky = Tex(TEXT("/Game/Brawl/Night/T_out_sky.T_out_sky")), *OutFar = Tex(TEXT("/Game/Brawl/Night/T_out_far.T_out_far"));
	UTexture* OutNear = Tex(TEXT("/Game/Brawl/Night/T_out_near.T_out_near")), *OutMoon = Tex(TEXT("/Game/Brawl/Night/T_out_moon.T_out_moon"));
	UTexture* LampT = Tex(TEXT("/Game/Brawl/Night/T_lamp.T_lamp"));
	const bool bNight = bPainted && Wall2DM && OutM && Glows[0] && Glows[1] && Glows[2] && BoothsWin && OutSky && OutFar && OutNear && OutMoon && LampT
		&& !FParse::Param(FCommandLine::Get(), TEXT("brawlbright"));
	if (bPainted && !bNight) UE_LOG(LogFTBrawl, Warning, TEXT("FTBRAWL no night art (run brawl_night.build() in the editor): bright room"));
	// night 2: THE VELVET ROOM (run 16961; art ArtSource/velvet + sheets, importer Content/Python/brawl_velvet.py)
	UMaterialInterface* DiscoM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlDisco.M_BrawlDisco"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UMaterialInterface* GlowM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlGlow.M_BrawlGlow"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UTexture* VPanels[3] = { Tex(TEXT("/Game/Brawl/Velvet/T_vbg_bar.T_vbg_bar")), Tex(TEXT("/Game/Brawl/Velvet/T_vbg_dj.T_vbg_dj")), Tex(TEXT("/Game/Brawl/Velvet/T_vbg_vip.T_vbg_vip")) };
	UTexture* VGlows[3] = { Tex(TEXT("/Game/Brawl/Velvet/T_vglow_bar.T_vglow_bar")), Tex(TEXT("/Game/Brawl/Velvet/T_vglow_dj.T_vglow_dj")), Tex(TEXT("/Game/Brawl/Velvet/T_vglow_vip.T_vglow_vip")) };
	UTexture* VFloor = Tex(TEXT("/Game/Brawl/Velvet/T_vfloor.T_vfloor")), *VFloorGlow = Tex(TEXT("/Game/Brawl/Velvet/T_vfloor_glow.T_vfloor_glow"));
	UTexture* VBall = Tex(TEXT("/Game/Brawl/Velvet/T_vball.T_vball")), *VRing = Tex(TEXT("/Game/Brawl/Velvet/T_ring.T_ring"));
	const bool bVelvet = IsVelvet() && bPainted && Wall2DM && OutM && DiscoM && GlowM && VPanels[0] && VPanels[1] && VPanels[2] && VGlows[0] && VGlows[1] && VGlows[2]
		&& VFloor && VFloorGlow && VBall && VRing;
	if (IsVelvet() && !bVelvet) UE_LOG(LogFTBrawl, Warning, TEXT("FTBRAWL no Velvet Room art (brawl_velvet.py): the Rusty Anchor's room stands in"));
	// night 3: THE LUCKY 7 (run 52616; art ArtSource/lucky + sheets, importer Content/Python/brawl_lucky.py)
	UTexture* CPanels[3] = { Tex(TEXT("/Game/Brawl/Lucky/T_cbg_bar.T_cbg_bar")), Tex(TEXT("/Game/Brawl/Lucky/T_cbg_slots.T_cbg_slots")), Tex(TEXT("/Game/Brawl/Lucky/T_cbg_cage.T_cbg_cage")) };
	UTexture* CGlows[3] = { Tex(TEXT("/Game/Brawl/Lucky/T_cglow_bar.T_cglow_bar")), Tex(TEXT("/Game/Brawl/Lucky/T_cglow_slots.T_cglow_slots")), Tex(TEXT("/Game/Brawl/Lucky/T_cglow_cage.T_cglow_cage")) };
	UTexture* CFloor = Tex(TEXT("/Game/Brawl/Lucky/T_cfloor.T_cfloor")), *CChand = Tex(TEXT("/Game/Brawl/Lucky/T_chand.T_chand")), *CLaser = Tex(TEXT("/Game/Brawl/Lucky/T_laser.T_laser"));
	const bool bLucky = IsLucky() && bPainted && Wall2DM && OutM && GlowM && CPanels[0] && CPanels[1] && CPanels[2] && CGlows[0] && CGlows[1] && CGlows[2] && CFloor && CChand && CLaser;
	if (IsLucky() && !bLucky) UE_LOG(LogFTBrawl, Warning, TEXT("FTBRAWL no Lucky 7 art (brawl_lucky.py): the Rusty Anchor's room stands in"));
	// nights 4-7 (run 32365; art ArtSource/n47 by tools/n47.py, importer Content/Python/brawl_n47.py): the same painted
	// room as nights 2 and 3 from one table: three back-wall panels with their glow masks and the lamps tools/n47.py found
	// on them (FTBrawlNightArt.inl), the floor tile, four painted props, warm key spots overhead
	static const TCHAR* Key47[4] = { TEXT("tiki"), TEXT("bowl"), TEXT("lodge"), TEXT("yacht") };
	static const TCHAR* Pan47[4][3] = { { TEXT("bar"), TEXT("masks"), TEXT("window") }, { TEXT("bar"), TEXT("lanes"), TEXT("shoes") },
		{ TEXT("bar"), TEXT("fire"), TEXT("window") }, { TEXT("bar"), TEXT("rail"), TEXT("cabin") } };
	const int32 N47 = IsN47() ? Night - 4 : 0;
	UTexture* NPanels[3] = { nullptr, nullptr, nullptr }; UTexture* NGlows[3] = { nullptr, nullptr, nullptr }; UTexture* NFloor = nullptr;
	if (IsN47())
	{
		for (int32 k = 0; k < 3; k++)
		{
			NPanels[k] = Tex(*FString::Printf(TEXT("/Game/Brawl/N47/T_%s_bg_%s.T_%s_bg_%s"), Key47[N47], Pan47[N47][k], Key47[N47], Pan47[N47][k]));
			NGlows[k] = Tex(*FString::Printf(TEXT("/Game/Brawl/N47/T_%s_glow_%s.T_%s_glow_%s"), Key47[N47], Pan47[N47][k], Key47[N47], Pan47[N47][k]));
		}
		NFloor = Tex(*FString::Printf(TEXT("/Game/Brawl/N47/T_%s_floor.T_%s_floor"), Key47[N47], Key47[N47]));
	}
	UTexture* LaserT = Tex(TEXT("/Game/Brawl/Lucky/T_laser.T_laser")), *RingT = Tex(TEXT("/Game/Brawl/Velvet/T_ring.T_ring"));
	const bool b47 = IsN47() && bPainted && Wall2DM && GlowM && NPanels[0] && NPanels[1] && NPanels[2] && NGlows[0] && NGlows[1] && NGlows[2] && NFloor && LaserT && RingT;
	if (IsN47() && !b47) UE_LOG(LogFTBrawl, Warning, TEXT("FTBRAWL no night %d art (brawl_n47.py): the Rusty Anchor's room stands in"), Night);
	if (b47)
	{
		const FRotator Upright(0, 0, 90);
		const float WallY = -Back, NearY = 700.f;
		static const float TileCm[4] = { 380.f, 300.f, 380.f, 300.f };
		static const TCHAR* SkyKey[4] = { TEXT("ffa060"), TEXT("e6d2ff"), TEXT("ffc27a"), TEXT("ffe8c8") };
		Quad(FVector(Xm, (WallY + NearY) / 2, 0), FVector2D(Len, NearY - WallY), FRotator::ZeroRotator, PaintM, NFloor, false,
			[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("TileU"), Len / TileCm[N47]); D->SetScalarParameterValue(TEXT("TileV"), (NearY - WallY) / TileCm[N47]);
				D->SetScalarParameterValue(TEXT("Glow"), N47 == 1 ? 0.12f : 0.05f); D->SetScalarParameterValue(TEXT("Rough"), N47 == 1 ? 0.85f : 0.55f);
				D->SetVectorParameterValue(TEXT("Tint"), FLinearColor(0.92f, 0.88f, 0.9f)); });
		const float PH = 560.f, PW = PH * 1.5f;
		int32 Pi = 0;
		for (float X = X0; X < X1; X += PW, Pi++)
		{
			const int32 K = Pi % 3;
			Quad(FVector(X + PW / 2, WallY, PH / 2), FVector2D(PW, PH), Upright, Wall2DM, NPanels[K], false,
				[&](UMaterialInstanceDynamic* D) { D->SetTextureParameterValue(TEXT("GlowTex"), NGlows[K]); D->SetScalarParameterValue(TEXT("Base"), 0.07f); });
			for (const FN47Lamp& L : GN47Lamps[N47][K]) if (L.I > 0) Lamp(FVector(X + L.U * PW, WallY + 120.f, PH * (1 - L.V)), L.C, L.I, 650.f);
		}
		static const float PropCm[4][4] = { { 95, 90, 300, 290 }, { 95, 115, 62, 30 }, { 95, 115, 230, 60 }, { 100, 115, 270, 75 } };
		auto NProp = [&](int32 I, float X, float Y, bool bFlip, bool bShadow)
		{
			const TCHAR* N = GN47Props[N47][I];
			UTexture* T = Tex(*FString::Printf(TEXT("/Game/Brawl/Sprites/Props/T_prop_%s.T_prop_%s"), N, N));
			if (!T) return;
			const float K = PropCm[N47][I] / GN47PropPx[N47][I], Sz = 1024.f * K;
			Quad(FVector(X, Y, (1000.f - 512.f) * K), FVector2D(bFlip ? -Sz : Sz, Sz), Upright, SpriteM, T, bShadow,
				[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("SelfLit"), I == 2 && (N47 == 0 || N47 == 3) ? 0.25f : 0.09f); });
		};
		Pi = 0;
		for (float X = X0; X < X1; X += PW, Pi++)
		{
			const float Yb = WallY + 30.f;
			if (Pi % 3 == 0) { NProp(0, X + PW * 0.25f, Yb, false, true); NProp(0, X + PW * 0.5f, Yb, false, true); NProp(0, X + PW * 0.75f, Yb, true, true); }
			else if (Pi % 3 == 1) { NProp(3, X + PW * 0.08f, Yb, false, true); NProp(3, X + PW * 0.92f, Yb, true, true); }
			else { NProp(1, X + PW * 0.2f, Yb, false, true); NProp(1, X + PW * 0.8f, Yb, true, true); }
		}
		int32 Fi = 0;
		for (float X = X0 + 2600.f; X < X1; X += 1450.f, Fi++)
		{
			if (Fi % 2 == 0) NProp(2, X, 600.f, Fi % 4 == 2, false);
			if (Fi % 2 == 0) NProp(1, X + 520.f, 470.f, false, true); else NProp(3, X, 470.f, true, true);
		}
		for (float X = X0 + 500.f; X < X1; X += 820.f)
		{
			ASpotLight* Sp = Wd->SpawnActor<ASpotLight>(FVector(X, -150.f, 470.f), FRotator(-90, 0, 0));
			USpotLightComponent* SC = Sp->SpotLightComponent;
			SC->SetMobility(EComponentMobility::Movable);
			Sp->SetActorRotation(FRotator(-90, 0, 0));
			SC->SetIntensity(62000.f); SC->SetAttenuationRadius(1150.f); SC->SetInnerConeAngle(20.f); SC->SetOuterConeAngle(52.f);
			SC->SetLightColor(Hex(SkyKey[N47])); SC->SetCastShadows(true);
		}
		// the bosses' tells on the floor (SyncActors): three lane strips (the Kingpin's lanes; Frankie's cone and Sven's line
		// use the first), the Commodore's ring, the bowling balls, Frankie's burning patches and the fire's light
		static const FLinearColor TellC[4] = { FLinearColor(1.f, 0.45f, 0.08f), FLinearColor(0.2f, 0.95f, 1.f), FLinearColor(1.f, 0.15f, 0.2f), FLinearColor(0.9f, 0.95f, 1.f) };
		for (int32 i = 0; i < 3; i++)
		{
			AStaticMeshActor* A = Quad(FVector(0, 0, 2.f), FVector2D(100.f, 45.f), FRotator::ZeroRotator, GlowM, LaserT, false,
				[&](UMaterialInstanceDynamic* D) { D->SetVectorParameterValue(TEXT("Color"), TellC[N47]); D->SetScalarParameterValue(TEXT("Glow"), 6.f); });
			A->SetActorHiddenInGame(true); LaneA.Add(A);
		}
		BossRing = Quad(FVector(0, 0, 2.f), FVector2D(400.f * PX, 200.f * PX), FRotator::ZeroRotator, GlowM, RingT, false,
			[&](UMaterialInstanceDynamic* D) { D->SetVectorParameterValue(TEXT("Color"), TellC[N47]); D->SetScalarParameterValue(TEXT("Glow"), 3.f); });
		BossRing->SetActorHiddenInGame(true);
		if (UTexture* BallT = Tex(TEXT("/Game/Brawl/Sprites/Props/T_prop_ball.T_prop_ball")))
			for (int32 i = 0; i < 3; i++)
			{
				const float K = 34.f / 588.f, Sz = 1024.f * K;
				AStaticMeshActor* A = Quad(FVector(0, 0, 0), FVector2D(Sz, Sz), Upright, SpriteM, BallT, true, [&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("SelfLit"), 0.2f); });
				A->SetActorHiddenInGame(true); BallA.Add(A);
			}
		for (int32 i = 0; i < 12; i++)
		{
			AStaticMeshActor* A = Quad(FVector(0, 0, 3.f), FVector2D(46.f * PX, 26.f * PX), FRotator::ZeroRotator, GlowM, LaserT, false,
				[&](UMaterialInstanceDynamic* D) { D->SetVectorParameterValue(TEXT("Color"), FLinearColor(1.f, 0.42f, 0.05f)); D->SetScalarParameterValue(TEXT("Glow"), 8.f); });
			A->SetActorHiddenInGame(true); FireA.Add(A);
		}
		FireL = Wd->SpawnActor<APointLight>(FVector(0, 0, 120.f), FRotator::ZeroRotator);
		FireL->PointLightComponent->SetMobility(EComponentMobility::Movable);
		FireL->PointLightComponent->SetIntensity(30000.f); FireL->PointLightComponent->SetAttenuationRadius(700.f); FireL->PointLightComponent->SetCastShadows(false);
		FireL->PointLightComponent->SetLightColor(Hex(TEXT("ff8a3d"))); FireL->PointLightComponent->SetVisibility(false);
	}
	else if (bLucky)
	{
		const FRotator Upright(0, 0, 90);
		const float WallY = -Back, NearY = 700.f;
		// the carpet: crimson and gold medallions, two to a 3 m tile
		const float TileCm = 300.f;
		Quad(FVector(Xm, (WallY + NearY) / 2, 0), FVector2D(Len, NearY - WallY), FRotator::ZeroRotator, PaintM, CFloor, false,
			[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("TileU"), Len / TileCm); D->SetScalarParameterValue(TEXT("TileV"), (NearY - WallY) / TileCm);
				D->SetScalarParameterValue(TEXT("Glow"), 0.06f); D->SetScalarParameterValue(TEXT("Rough"), 0.85f); D->SetVectorParameterValue(TEXT("Tint"), FLinearColor(0.9f, 0.85f, 0.85f)); });
		// the back wall: the bar (K 0: the drinks stand on its counter, SpawnPickups), the slots, the cashier's cage, again and again
		const float PH = 560.f, PW = PH * 1.5f;
		struct FLamp { float U, V; const TCHAR* C; float I; };
		static const FLamp Lamps[3][4] = {
			{ { 0.50f, 0.17f, TEXT("ff3040"), 3200 }, { 0.20f, 0.13f, TEXT("ffc070"), 1800 }, { 0.80f, 0.13f, TEXT("ffc070"), 1800 }, { 0.50f, 0.60f, TEXT("ffb050"), 1200 } },   // horseshoe, chandeliers, the champagne
			{ { 0.17f, 0.33f, TEXT("36f1ff"), 2000 }, { 0.37f, 0.33f, TEXT("ff4fb8"), 2000 }, { 0.57f, 0.33f, TEXT("ffd23f"), 2000 }, { 0.77f, 0.33f, TEXT("36f1ff"), 2000 } },   // slot toppers
			{ { 0.50f, 0.12f, TEXT("ffd23f"), 2600 }, { 0.20f, 0.25f, TEXT("36f1ff"), 1800 }, { 0.80f, 0.25f, TEXT("36f1ff"), 1800 }, { 0.50f, 0.55f, TEXT("9fe870"), 1100 } } }; // the wheel, neon, banker's lamps
		int32 Pi = 0;
		for (float X = X0; X < X1; X += PW, Pi++)
		{
			const int32 K = Pi % 3;
			Quad(FVector(X + PW / 2, WallY, PH / 2), FVector2D(PW, PH), Upright, Wall2DM, CPanels[K], false,
				[&](UMaterialInstanceDynamic* D) { D->SetTextureParameterValue(TEXT("GlowTex"), CGlows[K]); D->SetScalarParameterValue(TEXT("Base"), 0.07f); });
			for (const FLamp& L : Lamps[K]) Lamp(FVector(X + L.U * PW, WallY + 120.f, PH * (1 - L.V)), L.C, L.I, 650.f);
		}
		auto CProp = [&](const TCHAR* N, float Px, float Cm, float X, float Y, bool bFlip, bool bShadow)
		{
			UTexture* T = Tex(*FString::Printf(TEXT("/Game/Brawl/Sprites/Props/T_prop_%s.T_prop_%s"), N, N));
			if (!T) return;
			const float K = Cm / Px, Sz = 1024.f * K;
			Quad(FVector(X, Y, (1000.f - 512.f) * K), FVector2D(bFlip ? -Sz : Sz, Sz), Upright, SpriteM, T, bShadow,
				[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("SelfLit"), 0.1f); });
		};
		// against the wall: stools at the bar and the slots, gold ash urns either side of the cage
		Pi = 0;
		for (float X = X0; X < X1; X += PW, Pi++)
		{
			const float Yb = WallY + 30.f;
			if (Pi % 3 == 0) { CProp(TEXT("cstool"), 428, 100, X + PW * 0.25f, Yb, false, true); CProp(TEXT("cstool"), 428, 100, X + PW * 0.5f, Yb, false, true); CProp(TEXT("cstool"), 428, 100, X + PW * 0.75f, Yb, true, true); }
			else if (Pi % 3 == 1) for (const float U : { 0.17f, 0.37f, 0.57f, 0.77f }) CProp(TEXT("cstool"), 428, 100, X + PW * U, Yb, false, true);
			else { CProp(TEXT("urn"), 409, 110, X + PW * 0.06f, Yb, false, true); CProp(TEXT("urn"), 409, 110, X + PW * 0.94f, Yb, false, true); }
		}
		// the foreground: a palm or a slot machine now and then, blackjack tables and urns along the bottom edge
		int32 Fi = 0;
		for (float X = X0 + 2600.f; X < X1; X += 1450.f, Fi++)
		{
			if (Fi % 4 == 0) CProp(TEXT("palm"), 860, 280, X, 600.f, false, false);
			else if (Fi % 4 == 2) CProp(TEXT("slot"), 818, 220, X, 610.f, true, false);
			if (Fi % 2 == 0) CProp(TEXT("ctable"), 396, 115, X + 540.f, 470.f, false, true); else CProp(TEXT("urn"), 409, 110, X, 470.f, true, true);
		}
		// overhead: a gold chandelier every 8.2 m over the fight, each a warm spot pooling light on the carpet (the fighters'
		// shadows) and a soft glow of its own
		const float Ck = 135.f / 962.f;   // T_chand: 1024 x 1536, the chandelier ~962 px wide, its bottom at px 1508
		for (float X = X0 + 500.f; X < X1; X += 820.f)
		{
			const float Yl = -150.f, Zb = 425.f;
			Quad(FVector(X, Yl, Zb + (1508.f - 768.f) * Ck), FVector2D(1024.f * Ck, 1536.f * Ck), Upright, OutM, CChand, false,
				[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("Bright"), 1.0f); });
			ASpotLight* Sp = Wd->SpawnActor<ASpotLight>(FVector(X, Yl, Zb - 5.f), FRotator(-90, 0, 0));
			USpotLightComponent* SC = Sp->SpotLightComponent;
			SC->SetMobility(EComponentMobility::Movable);
			Sp->SetActorRotation(FRotator(-90, 0, 0));
			SC->SetIntensity(70000.f); SC->SetAttenuationRadius(1150.f); SC->SetInnerConeAngle(20.f); SC->SetOuterConeAngle(52.f);
			SC->SetLightColor(Hex(TEXT("ffd49a"))); SC->SetCastShadows(true);
			Lamp(FVector(X, Yl + 60.f, Zb - 30.f), TEXT("ffc070"), 1800, 560.f);
		}
		// the Pit Boss's red line (floor strip + beam at gun height) and the muzzle flash, shown in SyncActors
		LaserFloor = Quad(FVector(0, 0, 2.f), FVector2D(100.f, 45.f), FRotator::ZeroRotator, GlowM, CLaser, false,
			[&](UMaterialInstanceDynamic* D) { D->SetVectorParameterValue(TEXT("Color"), FLinearColor(1.f, 0.12f, 0.1f)); D->SetScalarParameterValue(TEXT("Glow"), 6.f); });
		LaserFloor->SetActorHiddenInGame(true);
		LaserBeam = Quad(FVector(0, 0, 160.f), FVector2D(100.f, 5.f), Upright, GlowM, CLaser, false,
			[&](UMaterialInstanceDynamic* D) { D->SetVectorParameterValue(TEXT("Color"), FLinearColor(1.f, 0.1f, 0.08f)); D->SetScalarParameterValue(TEXT("Glow"), 30.f); });
		LaserBeam->SetActorHiddenInGame(true);
		Muzzle = Wd->SpawnActor<APointLight>(FVector(0, 0, 160.f), FRotator::ZeroRotator);
		Muzzle->PointLightComponent->SetMobility(EComponentMobility::Movable);
		Muzzle->PointLightComponent->SetIntensity(40000.f); Muzzle->PointLightComponent->SetAttenuationRadius(600.f); Muzzle->PointLightComponent->SetCastShadows(false);
		Muzzle->PointLightComponent->SetLightColor(Hex(TEXT("ffe08a")));
		Muzzle->PointLightComponent->SetVisibility(false);
	}
	else if (bVelvet)
	{
		const FRotator Upright(0, 0, 90);
		const float WallY = -Back, NearY = 700.f;
		// the light-up dance floor: frosted glass tiles (4 x 4 a texture, 1.1 m a tile); M_BrawlDisco lights a random
		// set of tiles in pink / cyan / gold / violet on every beat (120 bpm), the rest stay dark glass
		const float TileCm = 340.f;   // 85 cm glass tiles
		Quad(FVector(Xm, (WallY + NearY) / 2, 0), FVector2D(Len, NearY - WallY), FRotator::ZeroRotator, DiscoM, VFloor, false,
			[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("TileU"), Len / TileCm); D->SetScalarParameterValue(TEXT("TileV"), (NearY - WallY) / TileCm);
				D->SetTextureParameterValue(TEXT("MaskTex"), VFloorGlow); D->SetScalarParameterValue(TEXT("Glow"), 0.3f); });
		// the back wall: bar, DJ booth, VIP corner, again and again (quilted velvet, the painted neon glows via the masks)
		const float PH = 560.f, PW = PH * 1.5f;
		struct FLamp { float U, V; const TCHAR* C; float I; };
		static const FLamp Lamps[3][3] = {
			{ { 0.81f, 0.13f, TEXT("ff4fb8"), 3000 }, { 0.40f, 0.06f, TEXT("36f1ff"), 2400 }, { 0.50f, 0.78f, TEXT("ff4fb8"), 2200 } },   // martini, wave, under the counter
			{ { 0.50f, 0.26f, TEXT("ff4fb8"), 3600 }, { 0.50f, 0.52f, TEXT("36f1ff"), 2600 }, { 0.25f, 0.15f, TEXT("ffd23f"), 1600 } },  // heart, sunburst, stars
			{ { 0.31f, 0.43f, TEXT("ff4fb8"), 2800 }, { 0.70f, 0.43f, TEXT("36f1ff"), 2800 }, { 0.50f, 0.10f, TEXT("ffc070"), 1500 } } }; // VIP neon, the lamps
		int32 Pi = 0;
		for (float X = X0; X < X1; X += PW, Pi++)
		{
			const int32 K = Pi % 3;
			Quad(FVector(X + PW / 2, WallY, PH / 2), FVector2D(PW, PH), Upright, Wall2DM, VPanels[K], false,
				[&](UMaterialInstanceDynamic* D) { D->SetTextureParameterValue(TEXT("GlowTex"), VGlows[K]); D->SetScalarParameterValue(TEXT("Base"), 0.05f); });
			for (const FLamp& L : Lamps[K]) Lamp(FVector(X + L.U * PW, WallY + 120.f, PH * (1 - L.V)), L.C, L.I, 650.f);
		}
		// painted props (1024 px frames, bottom at px 1000): chrome stools at the bar, velvet ropes by the DJ, cocktail
		// tables in the VIP corner; palms and tables in the foreground
		auto VProp = [&](const TCHAR* N, float Px, float Cm, float X, float Y, bool bFlip, bool bShadow)
		{
			UTexture* T = Tex(*FString::Printf(TEXT("/Game/Brawl/Sprites/Props/T_prop_%s.T_prop_%s"), N, N));
			if (!T) return;
			const float K = Cm / Px, Sz = 1024.f * K;
			Quad(FVector(X, Y, (1000.f - 512.f) * K), FVector2D(bFlip ? -Sz : Sz, Sz), Upright, SpriteM, T, bShadow,
				[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("SelfLit"), 0.08f); });
		};
		Pi = 0;
		for (float X = X0; X < X1; X += PW, Pi++)
		{
			const float Yb = WallY + 30.f;
			if (Pi % 3 == 0) { VProp(TEXT("vstool"), 328, 92, X + PW * 0.25f, Yb, false, true); VProp(TEXT("vstool"), 328, 92, X + PW * 0.5f, Yb, false, true); VProp(TEXT("vstool"), 328, 92, X + PW * 0.75f, Yb, true, true); }
			else if (Pi % 3 == 1) { VProp(TEXT("rope"), 353, 100, X + PW * 0.12f, Yb, false, true); VProp(TEXT("rope"), 353, 100, X + PW * 0.88f, Yb, true, true); }
			else { VProp(TEXT("vtable"), 409, 112, X + PW * 0.2f, Yb, false, true); VProp(TEXT("vtable"), 409, 112, X + PW * 0.8f, Yb, true, true); }
		}
		int32 Fi = 0;
		for (float X = X0 + 2600.f; X < X1; X += 1450.f, Fi++)
		{
			if (Fi % 2 == 0) VProp(TEXT("palm"), 860, 280, X, 600.f, Fi % 4 == 2, false);
			if (Fi % 2 == 0) VProp(TEXT("vtable"), 409, 112, X + 520.f, 470.f, false, true); else VProp(TEXT("rope"), 353, 100, X, 470.f, true, true);
		}
		// the ceiling rig: a mirror ball every 16 m with a little light of its own; a soft lavender key spot every 8 m
		// pools light on the dance floor and gives the fighters their shadows
		const float Ck = 78.f / 640.f;   // T_vball: 1024 x 1536, the ball ~640 px across, its centre at px 1190
		for (float X = X0 + 900.f; X < X1; X += 1640.f)
		{
			Quad(FVector(X, -170.f, 450.f + (1190.f - 768.f) * Ck), FVector2D(1024.f * Ck, 1536.f * Ck), Upright, OutM, VBall, false,
				[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("Bright"), 0.95f); });
			Lamp(FVector(X, -110.f, 420.f), TEXT("ffd6f4"), 1800, 600.f, false);
		}
		for (float X = X0 + 500.f; X < X1; X += 820.f)
		{
			ASpotLight* Sp = Wd->SpawnActor<ASpotLight>(FVector(X, -150.f, 520.f), FRotator(-90, 0, 0));
			USpotLightComponent* SC = Sp->SpotLightComponent;
			SC->SetMobility(EComponentMobility::Movable);
			Sp->SetActorRotation(FRotator(-90, 0, 0));
			SC->SetIntensity(42000.f); SC->SetAttenuationRadius(1200.f); SC->SetInnerConeAngle(20.f); SC->SetOuterConeAngle(52.f);
			SC->SetLightColor(Hex(TEXT("e6d2ff"))); SC->SetCastShadows(true);
		}
		// sweeping disco spots (pink, cyan, gold): no shadows, swung about in SyncActors
		static const TCHAR* SweepC[] = { TEXT("ff4fb8"), TEXT("36f1ff"), TEXT("ffd23f"), TEXT("b38cff") };
		int32 Si = 0;
		for (float X = X0 + 700.f; X < X1; X += 1100.f, Si++)
		{
			ASpotLight* Sp = Wd->SpawnActor<ASpotLight>(FVector(X, 150.f, 620.f), FRotator(-70, -90, 0));
			USpotLightComponent* SC = Sp->SpotLightComponent;
			SC->SetMobility(EComponentMobility::Movable);
			SC->SetIntensity(70000.f); SC->SetAttenuationRadius(1500.f); SC->SetInnerConeAngle(6.f); SC->SetOuterConeAngle(11.f);
			SC->SetLightColor(Hex(SweepC[Si % 4])); SC->SetCastShadows(false);
			Sweeps.Add(Sp);
		}
		// Vinnie's spotlight and the pink ring on the floor round him (shown during his DISCO WHIRLWIND)
		BossSpot = Wd->SpawnActor<ASpotLight>(FVector(0, 0, 700.f), FRotator(-90, 0, 0));
		BossSpot->SpotLightComponent->SetMobility(EComponentMobility::Movable);
		BossSpot->SetActorRotation(FRotator(-90, 0, 0));
		BossSpot->SpotLightComponent->SetIntensity(160000.f); BossSpot->SpotLightComponent->SetAttenuationRadius(1200.f);
		BossSpot->SpotLightComponent->SetInnerConeAngle(10.f); BossSpot->SpotLightComponent->SetOuterConeAngle(17.f);
		BossSpot->SpotLightComponent->SetLightColor(Hex(TEXT("fff2fb"))); BossSpot->SpotLightComponent->SetCastShadows(true);
		BossSpot->SpotLightComponent->SetVisibility(false);
		BossRing = Quad(FVector(0, 0, 2.f), FVector2D(240.f * PX, 120.f * PX), FRotator::ZeroRotator, GlowM, VRing, false,
			[&](UMaterialInstanceDynamic* D) { D->SetVectorParameterValue(TEXT("Color"), FLinearColor(1.f, 0.31f, 0.72f)); D->SetScalarParameterValue(TEXT("Glow"), 3.f); });
		BossRing->SetActorHiddenInGame(true);
	}
	else if (bPainted)
	{
		const FRotator Upright(0, 0, 90);   // the engine plane (XY, normal +Z) stood up to face the camera (+Y)
		// the floor: from the wall to past the bottom of the screen, planks ~2.6 m a tile
		const float WallY = -Back, NearY = 700.f, TileCm = 380.f;
		Quad(FVector(Xm, (WallY + NearY) / 2, 0), FVector2D(Len, NearY - WallY), FRotator::ZeroRotator, PaintM, Floor, false,
			[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("TileU"), Len / TileCm); D->SetScalarParameterValue(TEXT("TileV"), (NearY - WallY) / TileCm);
				D->SetScalarParameterValue(TEXT("Glow"), bNight ? 0.05f : 0.1f); D->SetScalarParameterValue(TEXT("Rough"), 0.6f);
				D->SetVectorParameterValue(TEXT("Tint"), FLinearColor(0.95f, 0.9f, 1.f)); });
		// the back wall: bar, game corner, booths, again and again; 5.6 m tall panels (3:2), the top runs out of frame
		const float PH = 560.f, PW = PH * 1.5f;
		// neon on each panel, in panel fractions (u from the left, v from the top): colour, intensity
		struct FLamp { float U, V; const TCHAR* C; float I; };
		static const FLamp Lamps[3][3] = {
			{ { 0.12f, 0.26f, TEXT("ff4fb8"), 3200 }, { 0.74f, 0.30f, TEXT("36f1ff"), 2500 }, { 0.45f, 0.14f, TEXT("ffc070"), 1800 } },
			{ { 0.27f, 0.62f, TEXT("ff4fb8"), 3200 }, { 0.36f, 0.22f, TEXT("ffd23f"), 2100 }, { 0.70f, 0.55f, TEXT("36f1ff"), 1400 } },
			{ { 0.12f, 0.22f, TEXT("36f1ff"), 2800 }, { 0.86f, 0.18f, TEXT("ff8a3d"), 2100 }, { 0.50f, 0.42f, TEXT("8fb4ff"), 1400 } } };
		int32 Pi = 0;
		for (float X = X0; X < X1; X += PW, Pi++)
		{
			const int32 K = Pi % 3;
			if (bNight)   // K 2 = the booths: its window panes are cut out, the harbour shows through
				Quad(FVector(X + PW / 2, WallY, PH / 2), FVector2D(PW, PH), Upright, Wall2DM, K == 2 ? BoothsWin : Panels[K], false,
					[&](UMaterialInstanceDynamic* D) { D->SetTextureParameterValue(TEXT("GlowTex"), Glows[K]); });
			else
				Quad(FVector(X + PW / 2, WallY, PH / 2), FVector2D(PW, PH), Upright, PaintM, Panels[K], false, [&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("Glow"), 0.5f); });
			for (const FLamp& L : Lamps[K]) Lamp(FVector(X + L.U * PW, WallY + 120.f, PH * (1 - L.V)), L.C, L.I, 650.f);
			if (bNight && K == 2) Lamp(FVector(X + 0.5f * PW, WallY + 90.f, 290.f), TEXT("7f9cff"), 900, 520.f);   // a little cold moonlight in at the window
		}
		// the view out of the windows: painted layers at real depths behind the wall, so the perspective camera moves
		// them in parallax as the fight scrolls (sky + sea farthest, then the lighthouse / boat / town shore, then the
		// pier rail just outside). Each layer is placed by where it should project on the wall (Zw0..Zw1, a tile TileW
		// wide there) from the camera's height, then pushed back K times the camera-to-wall distance.
		if (bNight)
		{
			const float CamY = 1450.f, Dw = CamY - WallY;
			auto Layer = [&](float K, UTexture* T, float Zw0, float Zw1, float TileW, float PanU, float Bright)
			{
				const float Y = CamY - K * Dw, Z0 = CamZ + (Zw0 - CamZ) * K, Z1 = CamZ + (Zw1 - CamZ) * K;
				const float XA = X1 - Len * K - 2000.f, XB = X0 + Len * K + 2000.f;
				Quad(FVector((XA + XB) / 2, Y, (Z0 + Z1) / 2), FVector2D(XB - XA, Z1 - Z0), Upright, OutM, T, false,
					[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("TileU"), (XB - XA) / (TileW * K));
						D->SetScalarParameterValue(TEXT("PanU"), PanU); D->SetScalarParameterValue(TEXT("Bright"), Bright); });
			};
			Layer(10.f, OutSky, 15.f, 615.f, 1800.f, 0.004f, 0.55f);    // the horizon (60% down the image) at wall height 255
			Layer(3.2f, OutFar, 224.f, 424.f, 300.f, 0.f, 0.65f);       // the shore's waterline (86%) just under the horizon
			Layer(1.3f, OutNear, 110.f, 377.f, 400.f, 0.f, 0.8f);       // the rail top (53%) at 235, its lamps up to the window top
		}
		// painted props (ArtSource/frames/prop_*: 1024 px frames, bottom at px 1000): px tall in the frame -> cm tall in the bar
		struct FProp { const TCHAR* N; float Px, Cm; };
		static const FProp Stool = { TEXT("stool"), 361, 98 }, Table = { TEXT("table"), 478, 125 }, Post = { TEXT("post"), 902, 590 }, Barrel = { TEXT("barrel"), 499, 155 };
		auto Prop = [&](const FProp& P, float X, float Y, bool bFlip = false, bool bShadow = true)
		{
			UTexture* T = Tex(*FString::Printf(TEXT("/Game/Brawl/Sprites/Props/T_prop_%s.T_prop_%s"), P.N, P.N));
			if (!T) return;
			const float K = P.Cm / P.Px, Sz = 1024.f * K;
			Quad(FVector(X, Y, (1000.f - 512.f) * K), FVector2D(bFlip ? -Sz : Sz, Sz), Upright, SpriteM, T, bShadow,
				[&](UMaterialInstanceDynamic* D) { if (bNight) D->SetScalarParameterValue(TEXT("SelfLit"), 0.07f); });
		};
		// against the wall (behind the fight's back edge): stools at the bar, tables by the booths, barrels in the game corner
		Pi = 0;
		for (float X = X0; X < X1; X += PW, Pi++)
		{
			const float Yb = WallY + 30.f;
			if (Pi % 3 == 0) { Prop(Stool, X + PW * 0.25f, Yb); Prop(Stool, X + PW * 0.5f, Yb); Prop(Stool, X + PW * 0.75f, Yb, true); }
			else if (Pi % 3 == 1) Prop(Barrel, X + PW * 0.88f, Yb);
			else { Prop(Table, X + PW * 0.2f, Yb); Prop(Table, X + PW * 0.8f, Yb, true); }
		}
		// the foreground: a pier post now and then between the camera and the fight (no shadow: it would cut across the
		// whole floor), barrels and tables along the bottom edge
		const float EarlEntryX = (ArenaEnd() + W + 80.f) * PX;
		int32 Fi = 0;
		for (float X = X0 + 2600.f; X < X1; X += 1450.f, Fi++)
		{
			// Leave the entrance sightline open at Earl's arena. The nearer post otherwise covers his entrance pose.
			if (Fi % 2 == 0 && (Night != 1 || FMath::Abs(X - EarlEntryX) >= 360.f))
				Prop(Post, X, 600.f, false, false);
			if (Fi % 2 == 0) Prop(Barrel, X + 520.f, 470.f); else Prop(Table, X, 460.f, true);
		}
		// overhead: warm bar lamps every few metres, these cast the sprites' shadows on the planks
		if (!bNight) for (float X = X0 + 400.f; X < X1; X += 1300.f) Lamp(FVector(X, -120.f, 430.f), TEXT("ffcf8a"), 8000, 1000.f, true);
		// at night they are the room's light: painted pendant lamps hung over the fight, each a spot pooling warm light
		// down on the planks (the fighters' shadows), plus a soft bulb glow so faces under the shade read
		else for (float X = X0 + 500.f; X < X1; X += 820.f)
		{
			const float Yl = -150.f, Zb = 415.f;   // over the middle of the fight band; the shade's rim at Zb
			const float Ck = 75.f / 650.f;         // T_lamp: 1024x1536, the shade ~650 px wide, its rim at px 1490
			Quad(FVector(X, Yl, Zb + (1490.f - 768.f) * Ck), FVector2D(1024.f * Ck, 1536.f * Ck), Upright, OutM, LampT, false,
				[&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("Bright"), 0.7f); });
			ASpotLight* Sp = Wd->SpawnActor<ASpotLight>(FVector(X, Yl, Zb - 5.f), FRotator(-90, 0, 0));
			USpotLightComponent* SC = Sp->SpotLightComponent;
			SC->SetMobility(EComponentMobility::Movable);
			Sp->SetActorRotation(FRotator(-90, 0, 0));
			SC->SetIntensity(60000.f); SC->SetAttenuationRadius(1100.f); SC->SetInnerConeAngle(18.f); SC->SetOuterConeAngle(50.f);
			SC->SetLightColor(Hex(TEXT("ffc27a"))); SC->SetCastShadows(true);
			Lamp(FVector(X, Yl + 60.f, Zb - 40.f), TEXT("ffb066"), 1500, 520.f);
		}
	}
	else
	{
		UE_LOG(LogFTBrawl, Warning, TEXT("FTBRAWL no 2.5D art (run brawl_2d.build() in the editor): blocky bar"));
	}
	if (!bPainted) {
	// floor: planks (alternating tones) of the venue's floor colour
	const FLinearColor F0 = Hex(TEXT("4a2a18")), F1 = Hex(TEXT("3e2314"));
	UMaterialInterface* FloorM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlFloor.M_BrawlFloor"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UMaterialInterface* WallM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlWall.M_BrawlWall"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	auto Skin = [&](AStaticMeshActor* A, UMaterialInterface* M, const FLinearColor& Tint)
	{
		if (!M) return;
		UMaterialInstanceDynamic* D = UMaterialInstanceDynamic::Create(M, A);
		D->SetVectorParameterValue(TEXT("Tint"), Tint);
		A->GetStaticMeshComponent()->SetMaterial(0, D);
	};
	Skin(Box(Cube, FVector(Xm, Back / 2 - 600, -10), FVector(Len, Back + 2200, 20), F0, false), FloorM, FLinearColor(0.75f, 0.6f, 0.6f));
	if (!FloorM) for (int32 i = 0; i < 24; i++) Box(Cube, FVector(Xm, -1300 + i * 70, 0.5f), FVector(Len, 30, 1), i % 2 ? F1 : F0, false);
	// back wall, wainscot and the trim line
	Skin(Box(Cube, FVector(Xm, Back + 30, 300), FVector(Len, 60, 620), Hex(TEXT("4a2a30"))), WallM, FLinearColor(0.7f, 0.5f, 0.6f));
	Box(Cube, FVector(Xm, Back - 4, 60), FVector(Len, 8, 120), Hex(TEXT("5a3420")));
	Box(Cube, FVector(Xm, Back - 8, 122), FVector(Len, 6, 6), Hex(TEXT("ffd23f")));
	// props from VENUES.anchor (x in brawl px)
	auto Bar = [&](float X, float Wb)
	{
		const float Cx = (X + Wb / 2) * PX, Y = Back - 70;
		Box(Cube, FVector(Cx, Y, 55), FVector(Wb * PX, 70, 110), Hex(TEXT("3a2214")));
		Box(Cube, FVector(Cx, Y - 4, 114), FVector(Wb * PX + 24, 90, 10), Hex(TEXT("6b3d1f")));
		static const TCHAR* Bt[] = { TEXT("36f1ff"), TEXT("9fe870"), TEXT("ffd23f"), TEXT("ff8a3d") };
		for (int32 b = 0; b < int32(Wb / 34); b++) Box(Cyl, FVector((X + 12 + b * 34) * PX, Back - 10, 160), FVector(10, 10, 30), Hex(Bt[b % 4]));
		Box(Cube, FVector(Cx, Back - 12, 140), FVector(Wb * PX, 20, 4), Hex(TEXT("6b3d1f")));
	};
	auto Stool = [&](float X) { Box(Cyl, FVector(X * PX, Back - 150, 72), FVector(40, 40, 8), Hex(TEXT("c0392b"))); Box(Cyl, FVector(X * PX, Back - 150, 36), FVector(6, 6, 70), Hex(TEXT("9aa0aa"))); };
	auto Table = [&](float X, const TCHAR* C)
	{
		Box(Cyl, FVector(X * PX, Back - 110, 80), FVector(110, 110, 6), Hex(TEXT("2a1a2e")));
		Box(Cyl, FVector(X * PX, Back - 110, 40), FVector(10, 10, 80), Hex(TEXT("555555")));
		Box(Cyl, FVector(X * PX - 20, Back - 110, 96), FVector(9, 9, 24), Hex(C));
		Box(Cyl, FVector(X * PX + 18, Back - 100, 94), FVector(9, 9, 20), Hex(C));
	};
	auto Juke = [&](float X)
	{
		Box(Cube, FVector(X * PX, Back - 45, 75), FVector(90, 60, 150), Hex(TEXT("ff8a3d")));
		Box(Sph, FVector(X * PX, Back - 45, 150), FVector(90, 60, 60), Hex(TEXT("ff8a3d")));
		AStaticMeshActor* G = Box(Cube, FVector(X * PX, Back - 78, 95), FVector(60, 6, 44), Hex(TEXT("36f1ff")));
		APointLight* L = Wd->SpawnActor<APointLight>(FVector(X * PX, -(Back - 160), 120), FRotator::ZeroRotator);
		L->PointLightComponent->SetMobility(EComponentMobility::Movable);
		L->PointLightComponent->SetIntensity(2500); L->PointLightComponent->SetAttenuationRadius(450); L->PointLightComponent->SetCastShadows(false);
		L->PointLightComponent->SetLightColor(Hex(TEXT("ff8a3d")));
		(void)G;
	};
	auto Sign = [&](float X, const TCHAR* Text, const TCHAR* C)
	{
		ATextRenderActor* T = Wd->SpawnActor<ATextRenderActor>(FVector(X * PX, -(Back - 2), 330), FRotator(0, 90, 0));
		UTextRenderComponent* TR = T->GetTextRender();
		TR->SetText(FText::FromString(Text)); TR->SetTextRenderColor(FColor::FromHex(C)); TR->SetWorldSize(70);
		TR->SetHorizontalAlignment(EHTA_Center); TR->SetVerticalAlignment(EVRTA_TextCenter);
		APointLight* L = Wd->SpawnActor<APointLight>(FVector(X * PX, -(Back - 120), 330), FRotator::ZeroRotator);
		L->PointLightComponent->SetMobility(EComponentMobility::Movable);
		L->PointLightComponent->SetIntensity(4000); L->PointLightComponent->SetAttenuationRadius(600); L->PointLightComponent->SetCastShadows(false);
		L->PointLightComponent->SetLightColor(Hex(C));
	};
	// the 80s art pass: Codex neon tubes (additive, on black) and torn posters on the brick, a pink/cyan light under each
	UMaterialInterface* NeonM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlNeon.M_BrawlNeon"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UMaterialInterface* PosterM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlPoster.M_BrawlPoster"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	auto Art = [&](float X, float Z, float Wd2, float Ht, UMaterialInterface* M, const TCHAR* Tex, const TCHAR* LightC, float Out) -> bool
	{
		UTexture* T = LoadObject<UTexture>(nullptr, *FString::Printf(TEXT("/Game/Brawl/Art/%s.%s"), Tex, Tex), nullptr, LOAD_Quiet | LOAD_NoWarn);
		if (!M || !T || !PlaneM) return false;
		AStaticMeshActor* A = Wd->SpawnActor<AStaticMeshActor>(FVector(X * PX, -(Back - Out), Z), FRotator(0, 0, 90));
		A->SetMobility(EComponentMobility::Movable);
		UStaticMeshComponent* SM = A->GetStaticMeshComponent();
		SM->SetStaticMesh(PlaneM); SM->SetWorldScale3D(FVector(Wd2 / 100.f, Ht / 100.f, 1)); SM->SetCastShadow(false);
		SM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UMaterialInstanceDynamic* D = UMaterialInstanceDynamic::Create(M, A);
		D->SetTextureParameterValue(TEXT("Tex"), T);
		SM->SetMaterial(0, D);
		if (LightC)
		{
			APointLight* L = Wd->SpawnActor<APointLight>(FVector(X * PX, -(Back - 140), Z), FRotator::ZeroRotator);
			L->PointLightComponent->SetMobility(EComponentMobility::Movable);
			L->PointLightComponent->SetIntensity(5000); L->PointLightComponent->SetAttenuationRadius(650); L->PointLightComponent->SetCastShadows(false);
			L->PointLightComponent->SetLightColor(Hex(LightC));
		}
		return true;
	};
	auto NeonSign = [&](float X, const TCHAR* Tex, const TCHAR* C, const TCHAR* Fallback) { if (!Art(X, 360, 330, 220, NeonM, Tex, C, 3)) Sign(X, Fallback, C); };
	NeonSign(300, TEXT("T_n_anchor"), TEXT("36f1ff"), TEXT("THE ANCHOR"));
	Bar(520, 520); Stool(560); Stool(660); Stool(760); Stool(860);
	NeonSign(900, TEXT("T_n_beer"), TEXT("ffd23f"), TEXT("BEER"));
	Art(1150, 230, 120, 180, PosterM, TEXT("T_p_ladies"), nullptr, 4);
	Juke(1300); NeonSign(1700, TEXT("T_n_girls"), TEXT("ff4fb8"), TEXT("GIRLS GIRLS GIRLS")); Table(1500, TEXT("ff4fb8")); Table(1900, TEXT("9fe870"));
	Art(2050, 230, 120, 180, PosterM, TEXT("T_p_rats"), nullptr, 4);
	Bar(2200, 400); Stool(2250); Stool(2380);
	NeonSign(2550, TEXT("T_n_cocktails"), TEXT("b38cff"), TEXT("COCKTAILS"));
	Sign(2900, TEXT("NO FIGHTING"), TEXT("ff4f6e")); Table(3100, TEXT("ff4fb8")); Juke(3400);
	Art(3250, 240, 130, 195, PosterM, TEXT("T_p_earl"), nullptr, 4);
	}
	// blood, glass and bottles: instanced, one draw each
	{
		AActor* Fx = Wd->SpawnActor<AActor>();
		USceneComponent* FxRoot = NewObject<USceneComponent>(Fx, TEXT("FxRoot"));
		Fx->SetRootComponent(FxRoot); FxRoot->RegisterComponent();
		UMaterialInterface* BloodM = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlBlood.M_BrawlBlood"), nullptr, LOAD_Quiet | LOAD_NoWarn);
		auto Ism = [&](const TCHAR* N, UStaticMesh* M, UMaterialInterface* Mat, const FLinearColor& Fallback)
		{
			UInstancedStaticMeshComponent* I = NewObject<UInstancedStaticMeshComponent>(Fx, N);
			I->SetStaticMesh(M); I->SetupAttachment(FxRoot); I->SetMobility(EComponentMobility::Movable);
			I->SetCollisionEnabled(ECollisionEnabled::NoCollision); I->SetCastShadow(false);
			if (Mat) I->SetMaterial(0, Mat);
			else { UMaterialInstanceDynamic* D = UMaterialInstanceDynamic::Create(BaseMat, Fx); D->SetVectorParameterValue(TEXT("Color"), Fallback); I->SetMaterial(0, D); }
			I->RegisterComponent();
			return I;
		};
		UMaterialInstanceDynamic* BloodMid = BloodM ? UMaterialInstanceDynamic::Create(BloodM, Fx) : nullptr;
		if (BloodMid) { BloodMid->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.11f, 0.f, 0.008f)); BloodM = BloodMid; }   // dark, wet, under neon
		DropIsm = Ism(TEXT("BloodDrops"), Sph, BloodM, FLinearColor(0.3f, 0, 0.01f));
		SplatIsm = Ism(TEXT("BloodSplats"), Cyl, BloodM, FLinearColor(0.3f, 0, 0.01f));
		GlassIsm = Ism(TEXT("Glass"), Cube, nullptr, FLinearColor(0.35f, 0.8f, 0.5f));
		BottleIsm = Ism(TEXT("Bottles"), Cyl, nullptr, FLinearColor(0.1f, 0.35f, 0.12f));
		WoodIsm = Ism(TEXT("Splinters"), Cube, nullptr, FLinearColor(0.28f, 0.14f, 0.06f));
		CardIsm = Ism(TEXT("Cards"), Cube, nullptr, FLinearColor(0.95f, 0.93f, 0.88f));
		SlugIsm = Ism(TEXT("Bullets"), Cube, nullptr, FLinearColor(1.f, 0.85f, 0.4f));
	}
	// light: one cheap sun-like key from behind-left over the camera, a sky light fill (no Lumen). At night there is
	// no sun: the neon and the bar lamps light the room, the sky light is only a dim blue so the shadows aren't black.
	if (!bNight && !bVelvet && !bLucky && !b47) {
	ADirectionalLight* Sun = Wd->SpawnActor<ADirectionalLight>(FVector(0, 0, 800), FRotator(-40, -70, 0));
	UDirectionalLightComponent* DL = CastChecked<UDirectionalLightComponent>(Sun->GetLightComponent());
	DL->SetMobility(EComponentMobility::Movable);
	Sun->SetActorRotation(FRotator(-30, -60, 0));   // from over the camera's left shoulder: the sprites' shadows fall back and right (the spawn rotation doesn't stick)
	DL->SetIntensity(bPainted ? 10.f : 8.f); DL->SetLightColor(Hex(TEXT("fff0dc")));
	DL->SetDynamicShadowDistanceMovableLight(4000.f); DL->SetDynamicShadowCascades(1);
	}
	ASkyLight* Sky = Wd->SpawnActor<ASkyLight>(FVector(0, 0, 600), FRotator::ZeroRotator);
	USkyLightComponent* SL = Sky->GetLightComponent();
	SL->SetMobility(EComponentMobility::Movable);
	static const TCHAR* Sky47[4] = { TEXT("8a5236"), TEXT("5a3a9c"), TEXT("5a6a9c"), TEXT("4a5a8c") };   // tiki fire, bowling violet, lodge snow-blue, harbour night
SL->SetIntensity(b47 ? 0.55f : bLucky ? 0.6f : bVelvet ? 0.5f : bNight ? 0.45f : 2.0f); SL->SetLightColor(Hex(b47 ? Sky47[N47] : bLucky ? TEXT("8a4a52") : bVelvet ? TEXT("6a3a9c") : bNight ? TEXT("4a4a8c") : TEXT("8c7cb8")));
	SL->bLowerHemisphereIsBlack = false;
	SL->RecaptureSky();
	// the cel pass: the main game's M_PP_Cel (copied into /Game/Brawl/Materials), its instance for this venue
	APostProcessVolume* PPV = Wd->SpawnActor<APostProcessVolume>();
	PPV->bUnbound = true;
	FPostProcessSettings& S = PPV->Settings;
	S.bOverride_AutoExposureMethod = true; S.AutoExposureMethod = AEM_Manual;
	S.bOverride_AutoExposureBias = true; S.AutoExposureBias = 0.8f;
	S.bOverride_AutoExposureApplyPhysicalCameraExposure = true; S.AutoExposureApplyPhysicalCameraExposure = false;
	S.bOverride_BloomIntensity = true; S.BloomIntensity = b47 ? 0.7f : bLucky ? 0.7f : bVelvet ? 0.75f : bNight ? 0.6f : 0.25f;
	S.bOverride_VignetteIntensity = true; S.VignetteIntensity = 0.45f;
	S.bOverride_MotionBlurAmount = true; S.MotionBlurAmount = 0.f;
	// the painted art carries its own ink and bands, and the CRT / VHS smear hides the paint: both passes are for the blocky fallback only
	if (!bPainted)
	{
		if (UMaterialInterface* Cel = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/MI_PP_Cel_RustyAnchor.MI_PP_Cel_RustyAnchor")))
			S.WeightedBlendables.Array.Add(FWeightedBlendable(1.f, Cel));
		// then the 1986 screen: CRT curve, RGB split, VHS wobble and scanlines (after the tonemapper)
		if (UMaterialInterface* Retro = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/MI_PP_Retro.MI_PP_Retro"), nullptr, LOAD_Quiet | LOAD_NoWarn))
			S.WeightedBlendables.Array.Add(FWeightedBlendable(1.f, Retro));
	}
	// the camera: side-on, a little above, following the fight like the web game's scrolling screen
	CamActor = Wd->SpawnActor<ACameraActor>(FVector(W / 2 * PX, 1450, CamZ), FRotator(-16, -90, 0));
	CamActor->GetCameraComponent()->SetFieldOfView(46.f);
	CamActor->GetCameraComponent()->SetConstraintAspectRatio(false);
	if (APlayerController* PC = Wd->GetFirstPlayerController()) PC->SetViewTarget(CamActor);
	// the moon follows the fight's scroll (it is as good as infinitely far): it stays put on screen and shows through
	// whichever window scrolls past it. Not attached to the camera: a cutscene's camera moves, tilts and zooms, and the
	// moon must stay in the sky then (PlaceMoon)
	if (bNight && !bVelvet && !bLucky)
	{
		const float CamY = 1450.f, Dw = CamY + Back, K = 9.f, Zw = 325.f, Dm = 75.f, Xw = 260.f;   // the window is wall height 220..364
		MoonOff = FVector(Xw * K, -K * Dw, (Zw - CamZ) * K);
		MoonA = Quad(FVector(CamActor->GetActorLocation().X, CamY, CamZ) + MoonOff, FVector2D(Dm * K, Dm * K),
			FRotator(0, 0, 90), OutM, OutMoon, false, [&](UMaterialInstanceDynamic* D) { D->SetScalarParameterValue(TEXT("Bright"), 0.9f); });
	}
}

void AFTBrawlGameMode::PlaceMoon()
{
	if (!MoonA || !CamActor) return;
	FVector At;
	if (CutId == 3) At = FVector(PierX - 100.f, 1450.f, CamZ + 600.f) + MoonOff;   // the pier: high over the harbour, clear of the cast
	else if (!InCut()) At = FVector(CamActor->GetActorLocation().X, 1450.f, CamZ) + MoonOff;      // the fight: rides the scroll (not the shake)
	else return;                                                                                   // THAT NIGHT / LAST CALL: stays where it is
	if (!MoonA->GetActorLocation().Equals(At, 1.f))
	{
		if (CutId == 3) UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL moon fixed over the pier at x=%.0f"), At.X);
		MoonA->SetActorLocation(At);
	}
}

// ---------------------------------------------------------------- pickups (run 21219): drops, bar weapons, the beer, power-ups
// Painted in the cast's style (ArtSource/sheets/items_a, items_b; Nova's held poses nova_e), imported by brawl_items.py.
// J on a weapon picks it up (on a beer at the bar: drinks it), J with a weapon swings it, K with a weapon throws it.
// Food and power-ups are taken by walking over them. Foes drop something now and then (DropFrom); the venue has a few
// weapons lying about and beers standing on the bar counter (SpawnPickups).

int32 AFTBrawlGameMode::DrinkKind() const
{
	if (IsLucky() && LoadObject<UTexture>(nullptr, GItems[IT_WHISKEY].Tex, nullptr, LOAD_Quiet | LOAD_NoWarn)) return IT_WHISKEY;   // a whiskey off the Lucky 7's bar
	return (IsVelvet() || Night == 4 || Night == 7) && LoadObject<UTexture>(nullptr, GItems[IT_COCKTAIL].Tex, nullptr, LOAD_Quiet | LOAD_NoWarn) ? IT_COCKTAIL : IT_BEER;   // a cocktail off the Velvet Room's bar
}

FString AFTBrawlGameMode::ItemName(int32 Kind) { return Kind >= 0 && Kind < IT_NUM ? GItems[Kind].Name : TEXT(""); }

void AFTBrawlGameMode::ClearItems()
{
	for (FFTBrawlItem& It : Items) if (It.Actor.IsValid()) It.Actor->Destroy();
	Items.Reset();
	Held = -1; HeldUses = 0; PowerT = ToughT = BuzzT = 0;
	bHeldPickupPresentation = false;
}

void AFTBrawlGameMode::SpawnPickups()
{
	ClearItems();
	if (!LoadObject<UTexture>(nullptr, GItems[IT_BEER].Tex, nullptr, LOAD_Quiet | LOAD_NoWarn)) { UE_LOG(LogFTBrawl, Warning, TEXT("FTBRAWL no item art (brawl_items.py): no pickups")); return; }
	// weapons lying about the Rusty Anchor: a stool in every fight, a bottle, the pool cue by the games, a pan from the kitchen
	struct FAt { int32 K; float X, Y; };
	// the Velvet Room: chrome bar stools, champagne, the DJ's mic stand, an ice bucket off a VIP table
	static const FAt Velvet[] = { { IT_VSTOOL, 560, 410 }, { IT_CHAMP, 940, 520 }, { IT_MIC, 1380, 420 }, { IT_VSTOOL, 1720, 520 },
		{ IT_BUCKET, 2240, 430 }, { IT_CHAMP, 2500, 380 }, { IT_VSTOOL, 2950, 470 }, { IT_MIC, 3330, 530 } };
	static const FAt Floor[] = { { IT_STOOL, 560, 410 }, { IT_BOTTLE, 940, 520 }, { IT_CUE, 1380, 420 }, { IT_STOOL, 1720, 520 },
		{ IT_PAN, 2240, 430 }, { IT_BOTTLE, 2500, 380 }, { IT_STOOL, 2950, 470 }, { IT_CUE, 3330, 530 } };
	// the Lucky 7: casino stools, whiskey bottles, a croupier's rake off a table, a cash box from the cage
	static const FAt Lucky[] = { { IT_CSTOOL, 560, 410 }, { IT_WBOTTLE, 940, 520 }, { IT_RAKE, 1380, 420 }, { IT_CSTOOL, 1720, 520 },
		{ IT_CASHBOX, 2240, 430 }, { IT_WBOTTLE, 2500, 380 }, { IT_CSTOOL, 2950, 470 }, { IT_RAKE, 3330, 530 } };
	const bool bLuckyArt = IsLucky() && LoadObject<UTexture>(nullptr, GItems[IT_RAKE].Tex, nullptr, LOAD_Quiet | LOAD_NoWarn);
	if (bLuckyArt) for (const FAt& A : Lucky) DropItem(A.K, A.X, A.Y, GItems[A.K].Uses, false, false);
	else if (IsVelvet() || Night == 7) for (const FAt& A : Velvet) DropItem(A.K, A.X, A.Y, GItems[A.K].Uses, false, false);
	else for (const FAt& A : Floor) DropItem(A.K, A.X, A.Y, GItems[A.K].Uses, false, false);
	// beers on the bar counter of every bar panel (the painting's counter top is 72% down the 5.6 m panel = 155 cm up)
	const float X0 = -400 * PX, PW = 560.f * 1.5f, WallY = -((FB - FT) * PX + 60);
	for (int32 Pi = 0; X0 + Pi * PW < (ArenaEnd() + W + 400) * PX; Pi += 3)
		for (const float U : { 0.3f, 0.82f })
		{
			const float Xc = X0 + (Pi + U) * PW, Xb = Xc / PX;
			if (Xb < 60 || Xb > ArenaEnd() + W - 60) continue;
			FFTBrawlItem It; It.Kind = DrinkKind(); It.bBar = true; It.X = Xb; It.Y = FT; It.BarAt = FVector(Xc, WallY + 8.f, 155.f);
			Items.Add(It);
		}
	UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL pickups: %d (beers on the bar: %d)"), Items.Num(), Items.FilterByPredicate([](const FFTBrawlItem& I) { return I.bBar; }).Num());
}

void AFTBrawlGameMode::DropItem(int32 Kind, float X, float Y, int32 Uses, bool bHop, bool bDrop)
{
	if (LockX >= 0) X = FMath::Clamp(X, LockX + 40, LockX + W - 40);   // never out of reach past the locked screen's edge (run 52616)
	if (GItems[Kind].Type == TY_WEAPON && Uses <= 0)
	{
		BreakItem(Kind, X, Y, 42.f, P.Dir);
		return;
	}
	FFTBrawlItem It; It.Kind = Kind; It.X = X; It.Y = FMath::Clamp(Y, FT + 6, FB); It.Uses = Uses; It.bDrop = bDrop;
	if (bHop) { It.Z = 30; It.Vz = 420; It.Vx = (Rand() - 0.5f) * 120; }
	Items.Add(It);
}

// a foe going down for good sometimes leaves something: rare, mostly food
void AFTBrawlGameMode::DropFrom(const FFTBrawlBody& F)
{
	if (F.bBoss || Items.FilterByPredicate([](const FFTBrawlItem& I) { return I.bDrop; }).Num() >= 2) return;
	const float R = Rand();
	int32 K = INDEX_NONE;
	const bool bPowered = PowerT > 0 || ToughT > 0;   // no second power-up while one is running
	if (F.K.Role == TEXT("heavy")) K = R < 0.4f ? IT_HOTDOG : (R < 0.55f && !bPowered) ? IT_JACKET : INDEX_NONE;   // the bouncer had his dinner on him
	else if (F.K.Role == TEXT("thrower")) K = R < 0.2f ? (IsVelvet() ? IT_CHAMP : IT_BOTTLE) : INDEX_NONE;     // the sleaze's last bottle
	else K = R < 0.09f ? IT_HOTDOG : bPowered ? INDEX_NONE : R < 0.125f ? IT_KNUCKS : R < 0.16f ? IT_JACKET : INDEX_NONE;
	if (K == INDEX_NONE) return;
	DropItem(K, F.X, F.Y, GItems[K].Uses, true, true);
	UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL drop %s from %s x=%.0f"), GItems[K].Name, *F.K.Role.ToString(), F.X);
}

int32 AFTBrawlGameMode::ItemAt() const
{
	if (bWon || bDay || InCut() || BossIntro > 0 || P.Hp <= 0 || P.Z > 0 ||
		(P.St != TEXT("idle") && P.St != TEXT("walk"))) return INDEX_NONE;
	int32 Nearest = INDEX_NONE;
	float BestDistance = MAX_flt;
	for (int32 Index = 0; Index < Items.Num(); ++Index)
	{
		const FFTBrawlItem& Item = Items[Index];
		if (Item.bThrown || Item.Z > 1.f) continue;
		const float Dx = FMath::Abs(P.X - Item.X), Dy = FMath::Abs(P.Y - Item.Y);
		const bool bReach = Item.bBar ? P.Y < FT + 40.f && Dx < 46.f
			: GItems[Item.Kind].Type == TY_WEAPON && Item.Uses > 0 && Dx < 44.f && Dy < 26.f;
		const float Distance = Item.bBar ? Dx : Dx + Dy;
		if (bReach && Distance < BestDistance)
		{
			Nearest = Index;
			BestDistance = Distance;
		}
	}
	return Nearest;
}

bool AFTBrawlGameMode::CanWalkPickup() const
{
	return P.Hp > 0.f && P.Z < 20.f && P.St != TEXT("down") && P.St != TEXT("held");
}

int32 AFTBrawlGameMode::NearbyWalkPickup() const
{
	if (!CanWalkPickup()) return INDEX_NONE;
	int32 Nearest = INDEX_NONE;
	float BestDistance = MAX_flt;
	for (int32 Index = 0; Index < Items.Num(); ++Index)
	{
		const FFTBrawlItem& Item = Items[Index];
		const int32 Type = GItems[Item.Kind].Type;
		if (Item.bThrown || Item.Z > 0.f || Item.Vz > 0.f || (Type != TY_FOOD && Type != TY_POWER)) continue;
		const float Dx = FMath::Abs(P.X - Item.X), Dy = FMath::Abs(P.Y - Item.Y);
		if (Dx < 98.f && Dy < 48.f && Dx + Dy < BestDistance)
		{
			Nearest = Index;
			BestDistance = Dx + Dy;
		}
	}
	return Nearest;
}

bool AFTBrawlGameMode::DescribeItem(const FFTBrawlItem& Item, FFTBrawlItemPresentation& Out) const
{
	if (Item.bThrown || Item.Z > 1.f || !Item.Actor.IsValid()) return false;
	const FItemDef& Def = GItems[Item.Kind];
	Out.Kind = Def.Type == TY_BEER ? EFTBrawlItemCue::Drink : Def.Type == TY_FOOD ? EFTBrawlItemCue::Food
		: Def.Type == TY_POWER ? EFTBrawlItemCue::Power : EFTBrawlItemCue::Weapon;
	Out.Feet = Item.bBar ? Item.BarAt : World(Item.X, Item.Y, 1.f);
	Out.Top = Out.Feet + FVector(0, 0, Def.Cm + 10.f);
	Out.Durability = Def.Type == TY_WEAPON ? FMath::Clamp(float(Item.Uses) / FMath::Max(1, Def.Uses), 0.f, 1.f) : 1.f;
	Out.bLastUse = Def.Type == TY_WEAPON && Item.Uses == 1;
	const float Distance = FMath::Abs(Item.X - P.X) + FMath::Abs(Item.Y - P.Y);
	Out.Strength = FMath::Clamp(1.f - (Distance - 160.f) / 560.f, 0.24f, 1.f);
	if (Item.bDrop && Item.T > DropLife - 3.f)
		Out.Strength *= 0.6f + 0.4f * FMath::Clamp((DropLife - Item.T) / 3.f, 0.f, 1.f);
	return true;
}

bool AFTBrawlGameMode::UseGroundItem()
{
	const int32 Index = ItemAt();
	if (Index == INDEX_NONE) return false;
	const FFTBrawlItem Item = Items[Index];
	bHeldPickupPresentation = !Item.bBar && Item.Actor.IsValid();
	if (bHeldPickupPresentation) HeldPickupTransform = Item.Actor->GetActorTransform();
	if (Item.Actor.IsValid()) Item.Actor->Destroy();
	Items.RemoveAt(Index);
	Accent(EAccent::Pickup, Item.bBar ? Item.BarAt + FVector(0, 8, 28) : World(Item.X, Item.Y + 7.f, 40.f), 65.f, P.Dir, false);
	Sfx(TEXT("pickup"), 0.6f, Item.bBar ? 1.25f : 1.f);
	if (Item.bBar)
	{
		P.St = TEXT("drink"); P.T = 0; P.Dir = 1;
		UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL grab beer off the bar x=%.0f (%s)"), Item.X, GItems[Item.Kind].Name);
	}
	else
	{
		if (Held >= 0) DropItem(Held, P.X - P.Dir * 24, P.Y, HeldUses, true, false);
		Held = Item.Kind; HeldUses = Item.Uses; P.St = TEXT("pickup"); P.T = 0;
		UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL pickup %s uses=%d"), GItems[Held].Name, HeldUses);
	}
	return true;
}

void AFTBrawlGameMode::BreakItem(int32 Kind, float X, float Y, float Z, float Dir, bool bSound)
{
	const bool bGlass = IsGlass(Kind);
	Accent(bGlass ? EAccent::Glass : EAccent::Wood, World(X, Y + 8.f, FMath::Max(Z, 20.f)), bGlass ? 94.f : 110.f, Dir, bSound);
	Bleed(X, Y, FMath::Max(Z, 4.f), Dir, bGlass ? 22 : 18, bGlass ? 0.9f : 0.8f, bGlass, !bGlass);
	Pop(X, Y, Z + 28.f, bGlass ? TEXT("SHATTERED") : TEXT("BROKEN"), FLinearColor(1.f, 0.78f, 0.4f));
}

void AFTBrawlGameMode::BreakHeld()
{
	if (Held < 0) return;
	BreakItem(Held, P.X + P.Dir * 50.f, P.Y, 110.f, P.Dir, false);   // the break Sfx below supplies the sound
	Shake = FMath::Min(1.f, Shake + 0.3f);
	Sfx(IsGlass(Held) ? TEXT("glass") : TEXT("break"), 0.9f, IsGlass(Held) ? 1.15f : 1.f);
	UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL weapon broke %s"), GItems[Held].Name);
	Held = -1; HeldUses = 0;
}

void AFTBrawlGameMode::StepItems(float Dt)
{
	if (Held >= 0 && HeldUses <= 0 && P.St != TEXT("wpn")) BreakHeld();
	if (PowerT > 0) PowerT -= Dt;
	if (ToughT > 0) ToughT -= Dt;
	if (BuzzT > 0) BuzzT -= Dt;
	const float Lo = LockX >= 0 ? LockX + 20 : -1e9f, Hi = LockX >= 0 ? LockX + W - 20 : 1e9f;
	for (int32 i = Items.Num() - 1; i >= 0; i--)
	{
		FFTBrawlItem& It = Items[i];
		const FItemDef& D = GItems[It.Kind];
		It.T += Dt;
		bool bGone = false;
		if (It.bThrown)
		{
			const float Sg = It.Vx > 0 ? 1.f : -1.f;
			It.X += It.Vx * Dt; It.Spin += 1080.f * Dt; It.Z = FMath::Max(30.f, It.Z - 50 * Dt);
			bool bHit = false;
			for (FFTBrawlBody& F : Foes)
			{
				if (!Standing(F)) continue;
				if (FMath::Abs(F.X - It.X) < 44 && FMath::Abs(F.Y - It.Y) < 28)
				{
					ImpactSfx(It.Kind);
					const float SaveDir = P.Dir; P.Dir = Sg; HitFoe(F, D.Dmg * 1.4f * (PowerT > 0 ? 1.5f : 1.f), true, false); P.Dir = SaveDir;
					bHit = true; UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL thrown %s hit %s"), D.Name, *F.K.Role.ToString()); break;
				}
			}
			if (bHit || It.T > 0.7f || It.X < Lo - 60 || It.X > Hi + 60)
			{
				if (IsGlass(It.Kind) || It.Uses <= 1) { BreakItem(It.Kind, It.X, It.Y, It.Z, Sg, !bHit); bGone = true; }   // a missed throw still needs its break sound
				else { It.bThrown = false; It.Uses--; It.Vx = -Sg * 90; It.Vz = 260; It.Spin = 0; It.X = FMath::Clamp(It.X, Lo, Hi); }
			}
		}
		else if (It.Z > 0 || It.Vz > 0)
		{
			It.Vz -= 1500 * Dt; It.Z += It.Vz * Dt; It.X += It.Vx * Dt;
			if (It.Z <= 0) {
				Accent(EAccent::Dust, World(It.X, It.Y + 6.f, 12.f), 45.f, It.Vx < 0 ? -1.f : 1.f, false);
				It.Z = 0; It.Vz = 0; It.Vx = 0;
			}
		}
		else if ((D.Type == TY_FOOD || D.Type == TY_POWER) && CanWalkPickup()
			&& FMath::Abs(P.X - It.X) < 36 && FMath::Abs(P.Y - It.Y) < 24)
		{
			Accent(EAccent::Pickup, World(It.X, It.Y + 7.f, 45.f), 78.f, P.Dir, false);
			Sfx(TEXT("powerup"), 0.6f, It.Kind == IT_HOTDOG ? 0.9f : 1.f);
			if (It.Kind == IT_HOTDOG) { const float H0 = P.Hp; P.Hp = FMath::Min(P.Max, P.Hp + 40); Pop(P.X, P.Y, 170, TEXT("+HP"), FLinearColor(0.62f, 0.91f, 0.44f)); UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL eat %s hp=%.0f->%.0f"), D.Name, H0, P.Hp); }
			else if (It.Kind == IT_KNUCKS) { PowerT = PowerTime; Pop(P.X, P.Y, 170, TEXT("POWER!"), FLinearColor(1.f, 0.82f, 0.25f), true); UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL power %s %.0fs"), D.Name, PowerTime); }
			else if (It.Kind == IT_JACKET) { ToughT = PowerTime; Pop(P.X, P.Y, 170, TEXT("TOUGH!"), FLinearColor(0.21f, 0.95f, 1.f), true); UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL power %s %.0fs"), D.Name, PowerTime); }
			bGone = true;
		}
		if (It.bDrop && !It.bThrown && It.T > DropLife) bGone = true;
		if (bGone) { if (It.Actor.IsValid()) It.Actor->Destroy(); Items.RemoveAt(i); }
	}
}

AStaticMeshActor* AFTBrawlGameMode::ItemActor(int32 Kind)
{
	UMaterialInterface* M = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Brawl/Materials/M_BrawlSprite.M_BrawlSprite"), nullptr, LOAD_Quiet | LOAD_NoWarn);
	UStaticMesh* Plane = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"));
	UTexture* T = LoadObject<UTexture>(nullptr, GItems[Kind].Tex, nullptr, LOAD_Quiet | LOAD_NoWarn);
	if (!M || !Plane || !T) return nullptr;
	AStaticMeshActor* A = GetWorld()->SpawnActor<AStaticMeshActor>();
	A->SetMobility(EComponentMobility::Movable);
	UStaticMeshComponent* SM = A->GetStaticMeshComponent();
	SM->SetStaticMesh(Plane); SM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SM->SetCastShadow(true); SM->bCastShadowAsTwoSided = true;
	UMaterialInstanceDynamic* D = UMaterialInstanceDynamic::Create(M, A);
	D->SetTextureParameterValue(TEXT("Tex"), T);
	SM->SetMaterial(0, D);
	A->Tags.Add(FName(*FString::FromInt(Kind)));
	return A;
}

// put pixel (Px, Py) of the item's frame at Anchor (world cm), turned RotDeg counter-clockwise on screen; bFlip mirrors it
void AFTBrawlGameMode::PlaceItem(AActor* A, int32 Kind, const FVector& Anchor, float Px, float Py, float RotDeg, bool bFlip, float Glow)
{
	if (!A) return;
	const FItemDef& D = GItems[Kind];
	const float K = D.Cm / D.Px, R = FMath::DegreesToRadians(RotDeg);
	const float Dx = (512.f - Px) * K, Dz = (Py - 512.f) * K;   // the frame's centre from the anchor (x right, z up)
	float Rx = Dx * FMath::Cos(R) - Dz * FMath::Sin(R); const float Rz = Dx * FMath::Sin(R) + Dz * FMath::Cos(R);
	if (bFlip) Rx = -Rx;
	const FQuat Q = FQuat(FVector(0, -1, 0), bFlip ? -R : R) * FQuat(FRotator(0, bFlip ? 180.f : 0.f, 0)) * FQuat(FRotator(0, 0, 90));
	A->SetActorLocationAndRotation(Anchor + FVector(Rx, 0, Rz), Q);
	A->SetActorScale3D(FVector(1024.f * K / 100.f, 1024.f * K / 100.f, 1));
	if (AStaticMeshActor* S = Cast<AStaticMeshActor>(A))
		if (UMaterialInstanceDynamic* M = Cast<UMaterialInstanceDynamic>(S->GetStaticMeshComponent()->GetMaterial(0)))
			M->SetScalarParameterValue(TEXT("SelfLit"), Glow);
}

void AFTBrawlGameMode::SyncItems()
{
	const int32 PressFocus = ItemAt();
	const int32 Focus = PressFocus != INDEX_NONE ? PressFocus : NearbyWalkPickup();
	for (int32 Index = 0; Index < Items.Num(); ++Index)
	{
		FFTBrawlItem& It = Items[Index];
		if (!It.Actor.IsValid()) It.Actor = ItemActor(It.Kind);
		AStaticMeshActor* A = It.Actor.Get();
		if (!A) continue;
		const FItemDef& D = GItems[It.Kind];
		const float Pulse = 0.04f * FMath::Sin(StT * 4.f + It.X * 0.013f);
		const float Glow = (Index == Focus ? 0.68f : (D.Type == TY_WEAPON ? 0.33f : 0.44f)) + Pulse;
		if (It.bBar) PlaceItem(A, It.Kind, It.BarAt, 512, 1000, 0, false, Glow);
		else if (It.bThrown) PlaceItem(A, It.Kind, World(It.X, It.Y, It.Z), D.Gu, D.Gv, (It.Vx > 0 ? -1.f : 1.f) * It.Spin, It.Vx < 0, 0.42f);
		else PlaceItem(A, It.Kind, World(It.X, It.Y, It.Z), 512, 1000, (It.Kind == IT_CUE || It.Kind == IT_MIC || It.Kind == IT_RAKE) ? -8.f : 0.f, false, Glow);
	}
	// in his hand: where the fist is in the frame he shows (ArtSource/frames/nova_*), the angle the state swings it to
	auto Hand = [this](float& Hx, float& Hy)
	{
		static const TMap<FName, FVector2D> GripByFrame = {
			{ TEXT("T_nova_walkdown"), { 636, 642 } }, { TEXT("T_nova_walkpass"), { 622, 640 } },
			{ TEXT("T_nova_walklift"), { 638, 637 } }, { TEXT("T_nova_walkpush"), { 620, 632 } },
			{ TEXT("T_nova_stepstart"), { 630, 618 } }, { TEXT("T_nova_stepstop"), { 635, 641 } },
			{ TEXT("T_nova_turn"), { 569, 614 } }, { TEXT("T_nova_takeoff"), { 580, 624 } },
			{ TEXT("T_nova_jumpfall"), { 632, 649 } }, { TEXT("T_nova_jumpkickout"), { 691, 720 } },
			{ TEXT("T_nova_liftload"), { 630, 853 } }, { TEXT("T_nova_overheadaim"), { 432, 394 } },
			{ TEXT("T_nova_bashrecover"), { 628, 681 } }, { TEXT("T_nova_hurtrecover"), { 612, 560 } },
			{ TEXT("T_nova_landing"), { 632, 637 } },
		};
		if (P.Actor)
		{
			FVector2D AuthoredGrip;
			if (P.Actor->CurrentSpriteGrip(AuthoredGrip)) { Hx = AuthoredGrip.X; Hy = AuthoredGrip.Y; return; }
			if (const FVector2D* Grip = GripByFrame.Find(P.Actor->CurrentSpriteFrame()))
			{
				Hx = Grip->X;
				Hy = Grip->Y;
				return;
			}
		}
		const FName S = P.St;
		if (S == TEXT("walk")) { Hx = 622; Hy = 630; }
		else if (S == TEXT("jump") || S == TEXT("jumpkick")) { Hx = 614; Hy = 665; }
		else if (S == TEXT("hurt") || S == TEXT("held")) { Hx = 635; Hy = 570; }
		else if (S == TEXT("pickup")) { Hx = 665; Hy = 945; }
		else if (S == TEXT("drink")) { Hx = 590; Hy = 465; }
		else if ((S == TEXT("wpn") || S == TEXT("toss")) && P.T < 0.12f) { Hx = 452; Hy = 405; }
		else if (S == TEXT("wpn") || S == TEXT("toss")) { Hx = 665; Hy = 650; }
		else { Hx = 620; Hy = 605; }
	};
	const bool bFlip = P.Dir < 0;
	const bool bShow = P.Actor && !P.Actor->IsHidden() && P.St != TEXT("down") && P.Hp > 0;
	auto HandAt = [&](float Hx, float Hy)
	{
		FVector Position;
		if (P.Actor && P.Actor->SpritePixelToWorld(FVector2D(Hx, Hy), Position)) return Position + FVector(0, 4, 0);
		return World(P.X, P.Y, P.Z) + FVector(P.Dir * (Hx - 512.f) * NovaCmPerPx, 4.f, (1000.f - Hy) * NovaCmPerPx);
	};
	if (Held >= 0 && bShow && P.St != TEXT("drink") && !(P.St == TEXT("toss") && P.T >= 0.14f))
	{
		if (!HeldA || HeldA->Tags.Num() == 0 || HeldA->Tags[0] != FName(*FString::FromInt(Held))) { if (HeldA) HeldA->Destroy(); HeldA = ItemActor(Held); }
		const FItemDef& D = GItems[Held];
		float Hx, Hy; Hand(Hx, Hy);
		float Ang = D.Carry;
		if (P.St == TEXT("wpn") || P.St == TEXT("toss")) Ang = P.T < 0.12f ? 110.f : FMath::Lerp(110.f, -15.f, FMath::Clamp((P.T - 0.12f) / 0.08f, 0.f, 1.f));
		if (HeldA)
		{
			HeldA->SetActorHiddenInGame(false);
			PlaceItem(HeldA, Held, HandAt(Hx, Hy), D.Gu, D.Gv, Ang - D.Tip, bFlip, PowerT > 0 ? 0.5f : 0.15f);
			if (P.St == TEXT("pickup") && bHeldPickupPresentation)
			{
				FTransform Lift;
				Lift.Blend(HeldPickupTransform, HeldA->GetActorTransform(), FMath::SmoothStep(0.035f, 0.17f, P.T));
				HeldA->SetActorTransform(Lift);
			}
			else bHeldPickupPresentation = false;
		}
	}
	else if (HeldA) HeldA->SetActorHiddenInGame(true);
	// the beer: up to his mouth and tipped back
	if (P.St == TEXT("drink") && bShow)
	{
		const int32 Dk = DrinkKind();
		if (MugA && (MugA->Tags.Num() == 0 || MugA->Tags[0] != FName(*FString::FromInt(Dk)))) { MugA->Destroy(); MugA = nullptr; }
		if (!MugA) MugA = ItemActor(Dk);
		float Hx, Hy; Hand(Hx, Hy);
		if (MugA) { MugA->SetActorHiddenInGame(false); PlaceItem(MugA, Dk, HandAt(Hx, Hy), GItems[Dk].Gu, GItems[Dk].Gv, FMath::Lerp(0.f, 60.f, FMath::Clamp(P.T / 0.4f, 0.f, 1.f)), bFlip, 0.2f); }
	}
	else if (MugA) MugA->SetActorHiddenInGame(true);
	// power-ups show on him: brass knuckles = a gold glow, the jacket = a cool steel one, buzzed = a warm flush
	if (P.Actor)
	{
		FLinearColor Tint = FLinearColor::White;
		if (PowerT > 0) Tint = FLinearColor::LerpUsingHSV(Tint, FLinearColor(1.45f, 1.2f, 0.55f), 0.55f + 0.25f * FMath::Sin(StT * 8.f));
		else if (ToughT > 0) Tint = FLinearColor(0.85f, 1.0f, 1.3f);
		else if (BuzzT > 0) Tint = FLinearColor(1.18f, 0.95f, 0.88f);
		P.Actor->SetTint(Tint);
	}
}

// ---------------------------------------------------------------- grab and throw (run 42137; owner, 15 Nov: "a grab and throw.
// Enemies thrown should hit other enemies"). L / C / pad B on a foe in front (any but the boss) lifts him over Nova's head;
// L / J / K hurls him the way you steer (or the way he faces). The body flies down the lane: every foe on his feet it
// meets takes 14 and is knocked down (the boss too), and the thrown one takes 16 when he lands. Hit while carrying = he drops him.
bool AFTBrawlGameMode::Standing(const FFTBrawlBody& F)
{
	return F.St != TEXT("fly") && F.St != TEXT("down") && F.St != TEXT("dead") && F.St != TEXT("getup") && F.St != TEXT("lifted") && F.St != TEXT("hurled");
}

bool AFTBrawlGameMode::TryGrab()
{
	FFTBrawlBody* Pick = nullptr; float Bd = 1e9f;
	for (FFTBrawlBody& F : Foes)
	{
		if (F.bBoss || !Standing(F) || F.Z > 10) continue;
		const float Dx = (F.X - P.X) * P.Dir, Dy = FMath::Abs(F.Y - P.Y);
		if (Dx > -12 && Dx < 72 && Dy < 26 && Dx + Dy < Bd) { Bd = Dx + Dy; Pick = &F; }
	}
	if (!Pick) return false;
	if (Held >= 0) { DropItem(Held, P.X - P.Dir * 24, P.Y, HeldUses, true, false); Held = -1; HeldUses = 0; }   // hands free for him
	Pick->St = TEXT("lifted"); Pick->T = 0; Pick->Vx = Pick->Vz = 0; Pick->Flash = 0.12f;
	GrabId = Pick->Id; P.St = TEXT("lift"); P.T = 0; P.Combo = 0;
	UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL grab %s hp=%.0f"), *Pick->K.Role.ToString(), Pick->Hp);
	return true;
}

void AFTBrawlGameMode::Hurl()
{
	FFTBrawlBody* G = Foes.FindByPredicate([this](const FFTBrawlBody& F) { return F.Id == GrabId; });
	GrabId = -1;
	if (!G) return;
	G->St = TEXT("hurled"); G->T = 0; G->HitIds.Reset();
	G->X = P.X + P.Dir * 30; G->Y = P.Y; G->Z = 108; G->Vx = P.Dir * 720; G->Vz = 230; G->Dir = P.Dir;
	CombatCue(ECombatCue::Throw, World(G->X, G->Y + 8.f, G->Z), P.Dir);
	Shake = FMath::Min(1.f, Shake + 0.3f);
	UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL hurl %s dir=%.0f"), *G->K.Role.ToString(), P.Dir);
}

void AFTBrawlGameMode::DropGrab()
{
	FFTBrawlBody* G = Foes.FindByPredicate([this](const FFTBrawlBody& F) { return F.Id == GrabId; });
	GrabId = -1;
	if (G && G->St == TEXT("lifted")) { G->St = TEXT("fly"); G->T = 0; G->Vx = P.Dir * 60; G->Vz = 0; UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL grab dropped %s"), *G->K.Role.ToString()); }
}

void AFTBrawlGameMode::StepHurled(FFTBrawlBody& F, float Dt)
{
	F.Vz -= 1500 * Dt; F.Z += F.Vz * Dt; F.X += F.Vx * Dt;
	const float Sg = F.Dir;
	for (FFTBrawlBody& O : Foes)
	{
		if (O.Id == F.Id || !Standing(O) || F.HitIds.Contains(O.Id)) continue;
		if (FMath::Abs(O.X - F.X) < (O.bBoss ? 62.f : 50.f) && FMath::Abs(O.Y - F.Y) < (O.bBoss ? 36.f : 30.f) && F.Z < 160)
		{
			F.HitIds.Add(O.Id);
			Sfx(TEXT("punchbig"), 0.9f, 0.8f);
			const float SaveDir = P.Dir; P.Dir = Sg; HitFoe(O, 14 * (BuzzT > 0 ? 1.25f : 1.f), true, false); P.Dir = SaveDir;   // punchbig above supplies the impact sound
			F.Vx *= 0.75f;
			UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL hurl hit %s (thrown %s) hp=%.0f"), *O.K.Role.ToString(), *F.K.Role.ToString(), O.Hp);
		}
	}
	if (LockX >= 0)   // a locked screen's edges stop him
	{
		const float Lo = LockX + 20, Hi = LockX + W - 20;
		if (F.X < Lo || F.X > Hi) { F.X = FMath::Clamp(F.X, Lo, Hi); F.Vx = 0; }
	}
	if (F.Z <= 0 && F.T > 0.05f)
	{
		F.Z = 0; F.St = TEXT("down"); F.T = 0; F.Hp -= 16; F.Flash = 0.12f;
		CombatCue(ECombatCue::Finish, World(F.X, F.Y + 8.f, 20.f), F.Dir, false);   // the down Sfx below supplies the landing sound
		Shake = FMath::Min(1.f, Shake + 0.5f); Sfx(TEXT("down"), 1.f, 0.9f); Rumble(0.6f, 0.22f, TEXT("down"));
		Bleed(F.X, F.Y, 40, Sg, 22, 1.2f);
		UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL hurl land %s hp=%.0f hits=%d"), *F.K.Role.ToString(), F.Hp, F.HitIds.Num());
	}
}

// test hook -brawlkeytest: the autopilot's input goes in as key events on the player controller (the same key state
// ReadInput reads when you play), so a headless run plays the keyboard path: arrows, J, K, L, Space, Enter
void AFTBrawlGameMode::PressKeys(const FIn& In)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	auto Key = [this, PC](const FKey& K, bool bOn)
	{
		if (TestKeysDown.Contains(K.GetFName()) == bOn) return;
		if (bOn) TestKeysDown.Add(K.GetFName()); else TestKeysDown.Remove(K.GetFName());
		PC->InputKey(FInputKeyEventArgs::CreateSimulated(K, bOn ? IE_Pressed : IE_Released, bOn ? 1.f : 0.f));
		if (K == EKeys::L && bOn) UE_LOG(LogFTBrawl, Log, TEXT("FTBRAWL key L"));
	};
	Key(bPadTest ? EKeys::Gamepad_DPad_Right : EKeys::Right, In.Steer > 0.1f);
	Key(bPadTest ? EKeys::Gamepad_DPad_Left : EKeys::Left, In.Steer < -0.1f);
	Key(bPadTest ? EKeys::Gamepad_DPad_Up : EKeys::Up, In.Throttle > 0.1f);
	Key(bPadTest ? EKeys::Gamepad_DPad_Down : EKeys::Down, In.Throttle < -0.1f);
	Key(bPadTest ? EKeys::Gamepad_FaceButton_Left : EKeys::J, In.Fire);
	Key(bPadTest ? EKeys::Gamepad_FaceButton_Top : EKeys::K, In.Kick);
	Key(bPadTest ? EKeys::Gamepad_FaceButton_Right : EKeys::L, In.Grab);
	Key(bPadTest ? EKeys::Gamepad_FaceButton_Bottom : EKeys::SpaceBar, In.Jump || (bPadTest && In.Enter));
	if (!bPadTest) Key(EKeys::Enter, In.Enter);
}

// ---------------------------------------------------------------- fight-night polish (run 38222; owner, 15 Nov: "in the fight scenes we
// need to add gamepad rumble. We need soundfx as well"). Sounds: original, synthesized (beat-em-up-unreal/Tools/brawl_sfx_synth.py),
// played 2D with a little pitch spread so a combo doesn't machine-gun; the M mute (master volume) covers them. Rumble: the pad's
// motors through the player controller (a no-op without a pad); a bigger one cuts in on a smaller one, smaller ones within 80 ms
// of the last are dropped. The races' own rumble is AFTGameMode::Shake (loop cycle 9); the nights had none.
void AFTBrawlGameMode::Sfx(const TCHAR* Name, float Vol, float Pitch)
{
	if (InCut()) return;
	const FName Key(Name);
	TObjectPtr<USoundBase>* Found = Sounds.Find(Key);
	USoundBase* S = Found ? Found->Get() : nullptr;
	if (!Found)
	{
		const FString Path = FString::Printf(TEXT("/Game/Brawl/Audio/SFX/S_brawl_%s.S_brawl_%s"), Name, Name);
		const double LT0 = FPlatformTime::Seconds();
		S = LoadObject<USoundBase>(nullptr, *Path, nullptr, LOAD_Quiet | LOAD_NoWarn);
		Sounds.Add(Key, S);
		UE_LOG(LogFTBrawl, Log, TEXT("FT BRAWL sfx first=%s ms=%.2f"), Name, (FPlatformTime::Seconds() - LT0) * 1000.0);   // loop: a load here is mid-fight
		if (!S) UE_LOG(LogFTBrawl, Warning, TEXT("FTBRAWL sfx missing %s (import beat-em-up-unreal/Docs/audio/sfx: Content/Python/brawl_audio.py)"), *Path);
	}
	if (!S) return;
	UGameplayStatics::PlaySound2D(this, S, Vol, Pitch * FMath::FRandRange(0.95f, 1.05f), 0.f, nullptr, this, false);   // looks only: not the fight's seeded stream
	NSfx++;
	UE_LOG(LogFTBrawl, Verbose, TEXT("FTBRAWL sfx %s vol=%.2f n=%d"), Name, Vol, NSfx);
}

void AFTBrawlGameMode::ImpactSfx(int32 Kind)
{
	if (IsGlass(Kind)) Sfx(TEXT("glass"), 0.7f, 1.3f);
	else if (Kind == IT_PAN || Kind == IT_BUCKET || Kind == IT_CASHBOX || Kind == IT_MIC) Sfx(TEXT("metal"), 0.75f, Kind == IT_MIC ? 1.3f : (Kind == IT_PAN ? 1.f : 0.85f));
	else Sfx(TEXT("wood"), 0.95f, Kind == IT_CUE || Kind == IT_RAKE ? 1.25f : 0.9f);   // a cue cracks higher than a stool
}

void AFTBrawlGameMode::Rumble(float Intensity, float Dur, const TCHAR* Why)
{
	if (InCut() || Intensity <= 0) return;
	const double Now = FPlatformTime::Seconds();
	if (Now - RumbleAt < 0.08 && Intensity <= RumbleI) return;
	APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (!PC) return;
	FLatentActionInfo Li; Li.CallbackTarget = this; Li.UUID = 38222; Li.Linkage = INDEX_NONE;   // one channel: a new rumble replaces the last
	PC->PlayDynamicForceFeedback(FMath::Clamp(Intensity, 0.f, 1.f), Dur, true, true, true, true, EDynamicForceFeedbackAction::Start, Li);
	RumbleAt = Now; RumbleI = Intensity; NRumble++;
	UE_LOG(LogFTBrawl, Verbose, TEXT("FTBRAWL rumble %s intensity=%.2f dur=%.2f n=%d out=%.2f"), Why, Intensity, Dur, NRumble, PC->ForceFeedbackValues.LeftLarge);
}
