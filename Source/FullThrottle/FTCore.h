// FULL THROTTLE (Unreal) - shared types, constants and level data.
// Gameplay runs in "track space" like the 1.x web game: d = distance along the road (cm),
// x = across (cm, + right), h = height above the road surface (cm). The track turns that into world space.
#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFT, Log, All);

// 1.x units -> centimetres. Across the road 1 unit = 9.14 cm; along the road 1 unit = 13.71 cm.
namespace FT
{
	constexpr float XU = 9.14f;
	constexpr float DU = 13.71f;
	constexpr float RoadHalf = 128.f * XU;   // 1170 cm: the painted road
	constexpr float Edge = 150.f * XU;       // 1371 cm: the barrier
	constexpr float WallH = 160.f;           // visible barrier height
	inline float LaneX(int32 i) { static const float L[4] = { -96.f, -32.f, 32.f, 96.f }; return L[FMath::Clamp(i, 0, 3)] * XU; }
	constexpr float BaseSpeed = 380.f * DU;  // 5210 cm/s (118 mph)
}

// 2.5: HalfPipe (a snowboard U: the road's sides curve up into walls), Coaster (two roller-coaster rails instead
// of a road: side by side you can hop between them, then they split and go wild) and Building (the road runs
// through the ground floor of a tower block).
UENUM()
enum class EFTSec : uint8 { Flat, Hill, Rise, Fall, Loop, Twist, Pipe, Jump, HalfPipe, Coaster, Building };

struct FFTSection
{
	float Len = 1000.f;  // 1.x road units
	float Curve = 0.f;   // + right
	float Bank = 0.f;    // radians, + leans into a right turn
	EFTSec Shape = EFTSec::Flat;
	float A = 0.f, B = 0.f;
};

UENUM()
enum class EFTKind : uint8
{
	None, Moto, Rammer, Suv, Truck, Heli, Hog, Ramp, Crate, Wrench, Barrel, Boss, Civ, Henchman, Bullet, Missile
};

struct FFTSpawn { float D; TArray<EFTKind> Kinds; };

struct FFTScene
{
	FString Name;
	float D = 0.f;          // 1.x road units
	FString Intro, Retake;
	TArray<FFTSpawn> Spawns;
};

enum class EFTBoss : uint8 { Juggernaut, Alchemist, Dealer, FireTruck, Clown, Yeti, Shark, Limo };

struct FFTLevelDef
{
	int32 Index = 0;
	FString Zone, Title, Place;
	FName Map;
	TArray<FFTSection> Opening, Arena, Circuit;
	float FinaleD = 0.f;
	TArray<FFTScene> Scenes;
	int32 Rating[4] = { 0, 0, 0, 0 };
	EFTBoss Boss = EFTBoss::Juggernaut;
	FString BossName;
	float BossHp = 120.f, BossEscape = 70.f;
	// look
	FLinearColor RoadA, RoadB, Line, KerbA, KerbB, Rail, Neon2, Sky0, Sky1, Sky2, Fog;
	FLinearColor Glow = FLinearColor(1.f, 0.31f, 0.72f);   // the car's underglow
	FLinearColor Lamp = FLinearColor(1.f, 0.75f, 0.55f);   // road lamps
	float LampCd = 7000.f;
	TArray<FString> Gantries;
	// 2.5 level features
	float Slope = 0.f;            // radians added to the road's pitch everywhere (the ski level runs downhill)
	bool bLava = false;           // lava under every jump gap and lava rivers beside the road
	TArray<FVector2D> LavaBands;  // lava rivers: x from..to (cm) beside the road
	bool bLightning = false;      // lightning in the sky (and the odd strike)
	int32 BossBullet = 0;         // what the boss's bullets look like (0 tracer, 1 fish, 2 fireball, 3 big tracer)
	FString Hint;                 // one line on the title card
	FString Backdrop;             // a landmark mesh on the horizon (Las Vegas, the volcano, the sunken city)
	FString Music = TEXT("/Game/Audio/Music/M_PixelAdventure"); // 2.6.1: the level's own looping track (Bayshore keeps Pixel Adventure)
	float BackdropScale = 4.f;
};

namespace FTData
{
	const FFTLevelDef& Level(int32 Index);
	int32 NumLevels();
	// director line pools by key; cycled in order
	const TArray<FString>& Lines(const FName& Key);
	FLinearColor Hex(const TCHAR* H);
}
