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

UENUM()
enum class EFTSec : uint8 { Flat, Hill, Rise, Fall, Loop, Twist, Pipe, Jump };

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

enum class EFTBoss : uint8 { Juggernaut, Alchemist, Dealer };

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
	TArray<FString> Gantries;
};

namespace FTData
{
	const FFTLevelDef& Level(int32 Index);
	int32 NumLevels();
	// director line pools by key; cycled in order
	const TArray<FString>& Lines(const FName& Key);
	FLinearColor Hex(const TCHAR* H);
}
