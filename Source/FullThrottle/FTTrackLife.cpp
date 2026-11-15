#include "FTLife.h"
#include "FTTrack.h"

// 3.1, the living world: extra roadside rules per level, for the animated props made by Content/Python/ft_life.py
// (ft_life.props()). Everything here is looks only. A rule whose mesh has not been built is skipped silently.
//
// Same slot rules as FTTrack.cpp's DefaultProps: P(Mesh, XMin, XMax, Every, Phase, Chance, bOnRoad, Light, Scale,
// Jitter, bBoth). X is in 1.x across units (9.14 cm), negative = left of travel; the barrier stands at 150. A slot is
// 45 m of road. These rules are appended after the level's own, so every existing prop keeps its place (a rule's
// random stream is seeded by its index); for the same reason, add new rules at the END of a level's list.
//
// Where things go, and why:
//   - nothing tall or bright nearer than 260 units (24 m from the centre line, 10 m past the barrier); the two
//     near things are low or dull on purpose (a tumbleweed, a pavement steam grate), and stand past the barrier
//   - moving lights (traffic, fireworks, the searchlights, the flare) stay out in the distance: the lane the player
//     reads has none of them
//   - the builder drops anything whose bounds (grown by how far its parts travel) would touch the road, a jump's
//     landing or another part of the circuit, exactly as it does for the levels' own props
void FTLife::ExtraProps(int32 Level, TArray<FFTPropRule>& R)
{
	auto P = [&R](const FString& Mesh, float X0, float X1, int32 Every, int32 Phase, float Chance, bool bOnRoad = false, int32 Light = 0, float Scale = 1.f, float Jit = 0.f, bool bBoth = false)
	{
		FFTPropRule Q; Q.Mesh = Mesh; Q.XMin = X0; Q.XMax = X1; Q.Every = Every; Q.Phase = Phase; Q.Chance = Chance;
		Q.bOnRoad = bOnRoad; Q.Light = Light; Q.Scale = Scale; Q.ScaleJitter = Jit; Q.bBothSides = bBoth;
		R.Add(Q);
	};
	if (Level == 1) // the chemical works: pumps nodding, fans turning, vents letting off steam, a flare on the skyline
	{
		const FString D = TEXT("/Game/Maps/ChemicalWorks/Life/");
		P(D + TEXT("SM_VentStack"), 300, 380, 4, 1, 0.7f, false, 0, 1.f, 0.15f, true);
		P(D + TEXT("SM_FanUnit"), 950, 1300, 4, 0, 0.7f);
		P(D + TEXT("SM_FanUnit"), -1950, -1550, 5, 2, 0.7f);
		P(D + TEXT("SM_Pumpjack"), 1500, 2000, 4, 2, 0.8f, false, 0, 1.25f, 0.15f);
		P(D + TEXT("SM_Pumpjack"), -2300, -1750, 5, 4, 0.8f, false, 0, 1.25f, 0.15f);
		P(D + TEXT("SM_Flare"), 4000, 5400, 6, 4, 0.9f, false, 0, 1.f, 0.2f, true);
	}
	else if (Level == 2) // Nevada: wind pumps, a pump jack or two, tumbleweed, dust devils on the flat, buzzards overhead
	{
		const FString D = TEXT("/Game/Maps/Nevada/Life/");
		P(D + TEXT("SM_Windpump"), -1300, -820, 9, 2, 0.9f, false, 0, 1.f, 0.12f);
		P(D + TEXT("SM_Windpump"), 1100, 1500, 11, 7, 0.9f, false, 0, 1.f, 0.12f);
		P(D + TEXT("SM_Pumpjack"), 1700, 2300, 7, 3, 0.85f, false, 0, 1.3f, 0.15f);
		P(D + TEXT("SM_Pumpjack"), -2300, -1700, 8, 5, 0.85f, false, 0, 1.3f, 0.15f);
		P(D + TEXT("SM_Tumbleweed"), 300, 650, 3, 1, 0.5f, false, 0, 1.f, 0.3f, true);
		P(D + TEXT("SM_DustDevil"), 1900, 3200, 7, 2, 0.6f, false, 0, 1.f, 0.3f, true);
		P(D + TEXT("SM_Buzzards"), 1500, 3000, 9, 5, 0.7f, false, 0, 1.f, 0.2f, true);
	}
	else if (Level == 4) // the theme park: bunting, searchlights sweeping, fireworks over the far side
	{
		const FString D = TEXT("/Game/Maps/ThemePark/Life/");
		P(D + TEXT("SM_Bunting"), 520, 900, 3, 2, 0.7f, false, 0, 1.f, 0.1f, true);
		P(D + TEXT("SM_Searchlight"), 2200, 3400, 11, 3, 0.8f, false, 0, 1.f, 0.f, true); // few and far: a dozen in view read as a fence of poles
		P(D + TEXT("SM_Firework"), 3000, 5200, 3, 1, 0.8f, false, 0, 1.f, 0.25f, true);
	}
	else if (Level == 5) // the glacier: a gondola lift, snow blowing off the crests, flags by the piste
	{
		const FString D = TEXT("/Game/Maps/Glacier/Life/");
		P(D + TEXT("SM_Gondola"), -2400, -1750, 4, 2, 0.9f);
		P(D + TEXT("SM_Spindrift"), 2900, 4600, 3, 1, 0.8f, false, 0, 1.f, 0.3f, true);
		P(D + TEXT("SM_Bunting"), 480, 900, 5, 3, 0.7f, false, 0, 1.f, 0.1f, true);
	}
	else if (Level == 7) // the city: steam off the pavement, an expressway of moving lights behind the towers
	{
		const FString D = TEXT("/Game/Maps/Metro/Life/");
		P(D + TEXT("SM_SteamGrate"), 200, 240, 4, 1, 0.7f, false, 0, 1.f, 0.f, true);
		P(D + TEXT("SM_FarTraffic"), 1150, 1650, 2, 0, 0.9f, false, 0, 1.f, 0.f, true);
	}
}
