#pragma once
#include "CoreMinimal.h"

// 3.2, the dressing: what stands in the world beside the locked track. A level's route is cut into DISTRICTS (zones,
// in kilometres along the road), and each district has its own rules, so every stretch has its own look at speed:
//   Scatter  a mesh strewn beside the road between two distances from its centre line      (the near roadside layer)
//   Site     a SET (a group of pieces with fixed offsets, composed round a landmark) put down as one  (middle distance)
//   Far      a big silhouette out past the land, on the horizon; it stays through loops and jumps    (the horizon)
//   Bank     a flat panel laid ON the cutting inside a banked bend, fitted to it by the track         (the cut bank)
// A banked bend rolls the road about its centre line, so its inner edge sinks (11.5 m at 57 degrees) and the track
// cuts the land down to it: one straight slope from under the low barrier up to the land's inner edge, 4 to 13 m
// long. The chase camera, rolled with the road, looks straight at it: a third of the frame. No Scatter or Site piece
// can stand there (they stand level, at the uncut land's height). A Bank rule gives the track a panel drawn FLAT and
// the track lays it on its own cutting, panel after panel along the bend: turned to the slope (whatever the bank's
// angle), its long side along the land's edge, pitched to the grade, stretched along to its share of the bend and up
// the slope to the cutting's length. Through the bend's lead-in and lead-out the cutting deepens under a panel (its
// foot sinks as the bank comes on, its top stays at the land's edge): there the panel is kept inside the cutting at
// both of its ends, so its top edge makes one level line under the land's edge and its foot steps down from panel to
// panel, and where the cutting deepens fast the panel is laid as two halves, a half as two quarters, a quarter as two
// eighths: the panel's own design, squeezed along the road (FTTrack.cpp, LayBank).
// The cutting is the track's own mesh (the bed), so a panel on it hides nothing the bed does not hide; it is tested
// only against OTHER parts of the road.
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

enum class EFTDress : uint8 { Scatter, Site, Far, Bank };

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
	// Bank only. Every and Phase count PANELS there, not slots (so two rules can take alternate panels).
	int32 Per = 4;                // panels to a 45 m slot: 4 = 11.3 m of road each (on the tightest bend a longer one would not lie flat)
	float RunMin = 0.f, RunMax = 1.e9f; // laid only where the panel's own run up the cutting (at the panel's middle) is this long (metres): a tall design and a low one
	float Foot = 0.6f, Top = 0.5f; // metres of the cutting left bare under the panel at the least (behind the low barrier; in a lead-in up to 0.8 m more at a piece's deeper end, more under an eighth) and above it (under the land's edge)
};

struct FFTDressZone { FString Name; float FromKm = 0.f, ToKm = 1.e9f; TArray<FFTDressRule> Rules; };
struct FFTDress { TArray<FFTDressSet> Sets; TArray<FFTDressZone> Zones; };

namespace FTDress
{
	/** The level's dressing tables (0 Bayshore ... 7 Metro). */
	void Build(int32 Level, FFTDress& Out);
}
