#pragma once
#include "CoreMinimal.h"

// 3.2, the dressing: what stands in the world beside the locked track. A level's route is cut into DISTRICTS (zones,
// in kilometres along the road), and each district has its own rules, so every stretch has its own look at speed:
//   Scatter  a mesh strewn beside the road between two distances from its centre line      (the near roadside layer)
//   Site     a SET (a group of pieces with fixed offsets, composed round a landmark) put down as one  (middle distance)
//   Far      a big silhouette out past the land, on the horizon; it stays through loops and jumps    (the horizon)
// Looks only: instanced meshes without collision. AFTTrack::BuildProps places them after the level's own props (which
// keep their places) and runs every piece through the same road-clearance test as a prop: a piece that would touch
// its own barrier is pushed out, one that would touch any other part of the road is left out. Near pieces also keep
// out of loops, pipes, corkscrews, jump gaps, run-ups and landings.
// The tables are FTDressL0.inl ... FTDressL7.inl (one per level), read by FTTrackDress.cpp; the meshes are made by
// Content/Python/ft_dress_*.py and live under /Game/Maps/<Level>/Dress/.

/** One piece of a set, relative to the set's anchor: metres along the road (+ = the way the set faces along it),
 *  metres further out from the road (+ = away), metres up, yaw in degrees (0 = as built: front towards the road). */
struct FFTDressPart { FString Mesh; float Along = 0.f, Out = 0.f, Up = 0.f, Yaw = 0.f, Scale = 1.f; };
struct FFTDressSet { FString Name; TArray<FFTDressPart> Parts; };

enum class EFTDress : uint8 { Scatter, Site, Far };

struct FFTDressRule
{
	EFTDress Kind = EFTDress::Scatter;
	FString Mesh;                 // Scatter, Far: the mesh; Site: the set's name
	float XMin = 0.f, XMax = 0.f; // metres from the road's centre line (Site: where the set's anchor stands)
	int32 Side = 0;               // -1 left of travel, +1 right, 0 both (Site: alternates, slot by slot)
	int32 Every = 1, Phase = 0;   // every n-th 45 m slot of the district, offset by Phase
	float Chance = 1.f, Scale = 1.f, Jitter = 0.f, YawJitter = 0.f;
	float Drop = 25.f;            // Far only: metres below the road's own level that the piece's base stands at
	bool bFloor = false;          // Far and Site: stands on the level's floor instead (Bayshore's sea: boats, a marina)
	bool bLight = false;          // carries one of the road's pooled lamp lights
};

struct FFTDressZone { FString Name; float FromKm = 0.f, ToKm = 1.e9f; TArray<FFTDressRule> Rules; };
struct FFTDress { TArray<FFTDressSet> Sets; TArray<FFTDressZone> Zones; };

namespace FTDress
{
	/** The level's dressing tables (0 Bayshore ... 7 Metro). */
	void Build(int32 Level, FFTDress& Out);
}
