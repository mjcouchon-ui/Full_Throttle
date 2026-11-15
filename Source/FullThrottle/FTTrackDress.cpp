#include "FTDress.h"
#include <initializer_list>

// 3.2, the dressing tables. See FTDress.h for what the three kinds of rule do; each level's table is its own file,
// FTDressL<n>.inl, written with the few words defined here:
//
//   Set(TEXT("Name"), { { TEXT("SM_Piece"), along m, out m, up m, yaw deg, scale }, ... });
//       A group of pieces composed round a landmark. Offsets are from the set's anchor: "along" runs with the road,
//       "out" away from it. A piece's front (its -Y side as built) faces the road at yaw 0.
//   Zone(TEXT("Name"), from km, to km);
//       Starts a district: every rule after it belongs to it, until the next Zone.
//   Scatter(TEXT("SM_Piece"), x min m, x max m, every, phase, chance [, scale, jitter, side, yaw jitter deg]);
//   Site(TEXT("SetName"), side, x m, every, phase, chance [, scale, jitter]);
//   Far(TEXT("SM_Piece"), dist min m, dist max m, every, phase, chance [, scale, jitter, side, drop m, yaw jitter deg]);
//       The horizon, past the land's edge (the land ends 300 m out, 640 m on Bayshore's town side). The piece's
//       z = 0 stands "drop" metres below the road's own level: build it from z = 0 up, 60 to 250 m tall.
//   Afloat(TEXT("SM_Piece"), dist min m, dist max m, every, phase, chance [, scale, jitter, side, yaw jitter deg]);
//   SiteAfloat(TEXT("SetName"), side, x m, every, phase, chance [, scale]);
//       The same, standing on the level's floor instead (Bayshore: the sea's surface. Boats, a marina, islands).
//   Lit();   the rule just written carries a lamp light (a kiosk, a hut, a sign)
//   Bank(TEXT("SM_Piece"), per slot, every, phase, chance [, run min m, run max m, foot m, top m]);
//       Panels on the cut bank inside every banked bend of the district (FTDress.h). "per slot" panels to a 45 m
//       slot; every/phase count PANELS here. The panel is laid where its run up the cutting at the panel's middle, after
//       "foot" and "top" metres are left bare, is between "run min" and "run max": a tall design for the deep cuttings of the wall
//       bends (banked 32 degrees and more: 5.5 m and up) and a low one for the rest. Under a gentle bank the bed's
//       foot is hidden behind the low barrier: the run is counted from 30 cm under the barrier's top, so a bend
//       banked under about 19 degrees shows too little cutting (under 2.4 m) and is left alone. Draw the piece FLAT, as if the
//       bank were the floor: x along the road, y up the slope (the road is on the -y side), z out of the bank with
//       z = 0 the bank's own surface. Give it a body from z = -40 to about +14 (the track fits a flat panel to a
//       curved bank to within 12 cm) and keep what stands on the body under 30 cm. It is stretched in x and y to fit,
//       so draw bands and blocks, not circles. Through a bend's lead-in and lead-out, where the cutting deepens under a
//       panel, the piece is kept inside the cutting at both ends (shorter up the slope than the run, down to 1 m) and is
//       laid as two halves, a half as two quarters, a quarter as two eighths: the whole design squeezed into 5.6, 2.8 or
//       1.4 m of road. No lights and no arrows: it lies 9 to 16 m from the centre line.
//
// Scatter and Site pieces stand ON the land at their distance (it rises and falls with the level's ground profile),
// so keep them inside the land: nearer than 295 m (Bayshore's town side: 630 m). Use Far beyond that.
//
// side: -1 = left of travel, +1 = right, 0 = both (a Site alternates sides). every/phase count 45 m slots.
// x is measured from the road's centre line: the barrier's outer face is at 14.1 m, so nothing nearer than 17.
// A mesh name without a leading slash is looked up in the level's own /Game/Maps/<Level>/Dress/ folder; a full path
// (TEXT("/Game/Maps/Bayshore/Props/SM_Palm")) uses that asset. A mesh that does not exist is skipped silently.

namespace
{
	struct FDP { const TCHAR* Mesh; float Along, Out, Up, Yaw, Scale; };

	// A level's table is written by one member function of this (Level0 ... Level7, below), and the words a table
	// is written in (Set, Zone, Scatter, Site, Far, Afloat, SiteAfloat, Lit, Bank) are its other members. The eight
	// tables used to be included one after another in a single function, FTDress::Build: with the third dressing pass
	// that function outgrew what the compiler will optimise (C4883 "function size suppresses optimizations", an
	// error in this build). A table's own lambdas capture by [&], which takes this writer with it.
	struct FTableWriter
	{
		FFTDress& Out;
		const FString Dir;
		FTableWriter(FFTDress& InOut, const FString& InDir) : Out(InOut), Dir(InDir) {}

		FString Path(const TCHAR* M) const { return (M && M[0] == TEXT('/')) ? FString(M) : Dir + M; }
		FFTDressRule& Rule()
		{
			if (!Out.Zones.Num()) { FFTDressZone Z; Z.Name = TEXT("Everywhere"); Out.Zones.Add(Z); }
			return Out.Zones.Last().Rules.AddDefaulted_GetRef();
		}
		void Set(const TCHAR* Name, std::initializer_list<FDP> Parts)
		{
			FFTDressSet& S = Out.Sets.AddDefaulted_GetRef();
			S.Name = Name;
			for (const FDP& P : Parts) { FFTDressPart Q; Q.Mesh = Path(P.Mesh); Q.Along = P.Along; Q.Out = P.Out; Q.Up = P.Up; Q.Yaw = P.Yaw; Q.Scale = P.Scale; S.Parts.Add(Q); }
		}
		void Zone(const TCHAR* Name, float FromKm, float ToKm)
		{
			FFTDressZone& Z = Out.Zones.AddDefaulted_GetRef();
			Z.Name = Name; Z.FromKm = FromKm; Z.ToKm = ToKm;
		}
		void Scatter(const TCHAR* Mesh, float X0, float X1, int32 Every, int32 Phase, float Chance, float Scale = 1.f, float Jitter = 0.f, int32 Side = 0, float YawJitter = 0.f)
		{
			FFTDressRule& Q = Rule();
			Q.Kind = EFTDress::Scatter; Q.Mesh = Path(Mesh); Q.XMin = X0; Q.XMax = X1; Q.Every = Every; Q.Phase = Phase; Q.Chance = Chance;
			Q.Scale = Scale; Q.Jitter = Jitter; Q.Side = Side; Q.YawJitter = YawJitter;
		}
		void Site(const TCHAR* SetName, int32 Side, float X, int32 Every, int32 Phase, float Chance, float Scale = 1.f, float Jitter = 0.f)
		{
			FFTDressRule& Q = Rule();
			Q.Kind = EFTDress::Site; Q.Mesh = SetName; Q.XMin = Q.XMax = X; Q.Side = Side; Q.Every = Every; Q.Phase = Phase; Q.Chance = Chance; Q.Scale = Scale; Q.Jitter = Jitter;
		}
		void Far(const TCHAR* Mesh, float D0, float D1, int32 Every, int32 Phase, float Chance, float Scale = 1.f, float Jitter = 0.f, int32 Side = 0, float Drop = 25.f, float YawJitter = 30.f)
		{
			FFTDressRule& Q = Rule();
			Q.Kind = EFTDress::Far; Q.Mesh = Path(Mesh); Q.XMin = D0; Q.XMax = D1; Q.Every = Every; Q.Phase = Phase; Q.Chance = Chance;
			Q.Scale = Scale; Q.Jitter = Jitter; Q.Side = Side; Q.Drop = Drop; Q.YawJitter = YawJitter;
		}
		// Afloat / SiteAfloat: like Far and Site, but the piece (or the set) stands on the level's FLOOR, the sheet under
		// the whole level. On Bayshore that is the sea, so these are boats, a marina, islands: build them with the
		// waterline at z = 0. They are horizon things: no shadow, and they stay through loops and jumps.
		void Afloat(const TCHAR* Mesh, float D0, float D1, int32 Every, int32 Phase, float Chance, float Scale = 1.f, float Jitter = 0.f, int32 Side = 0, float YawJitter = 180.f)
		{
			FFTDressRule& Q = Rule();
			Q.Kind = EFTDress::Far; Q.Mesh = Path(Mesh); Q.XMin = D0; Q.XMax = D1; Q.Every = Every; Q.Phase = Phase; Q.Chance = Chance;
			Q.Scale = Scale; Q.Jitter = Jitter; Q.Side = Side; Q.Drop = 0.f; Q.YawJitter = YawJitter; Q.bFloor = true;
		}
		void SiteAfloat(const TCHAR* SetName, int32 Side, float X, int32 Every, int32 Phase, float Chance, float Scale = 1.f)
		{
			FFTDressRule& Q = Rule();
			Q.Kind = EFTDress::Site; Q.Mesh = SetName; Q.XMin = Q.XMax = X; Q.Side = Side; Q.Every = Every; Q.Phase = Phase; Q.Chance = Chance; Q.Scale = Scale; Q.bFloor = true;
		}
		void Lit() { if (Out.Zones.Num() && Out.Zones.Last().Rules.Num()) Out.Zones.Last().Rules.Last().bLight = true; }
		void Bank(const TCHAR* Mesh, int32 Per, int32 Every, int32 Phase, float Chance, float RunMin = 0.f, float RunMax = 1.e9f, float Foot = 0.6f, float Top = 0.5f)
		{
			FFTDressRule& Q = Rule();
			Q.Kind = EFTDress::Bank; Q.Mesh = Path(Mesh); Q.Per = Per; Q.Every = Every; Q.Phase = Phase; Q.Chance = Chance;
			Q.RunMin = RunMin; Q.RunMax = RunMax; Q.Foot = Foot; Q.Top = Top;
		}

		void Level0(); void Level1(); void Level2(); void Level3(); void Level4(); void Level5(); void Level6(); void Level7();
	};

	void FTableWriter::Level0()
	{
#include "FTDressL0.inl"
	}
	void FTableWriter::Level1()
	{
#include "FTDressL1.inl"
	}
	void FTableWriter::Level2()
	{
#include "FTDressL2.inl"
	}
	void FTableWriter::Level3()
	{
#include "FTDressL3.inl"
	}
	void FTableWriter::Level4()
	{
#include "FTDressL4.inl"
	}
	void FTableWriter::Level5()
	{
#include "FTDressL5.inl"
	}
	void FTableWriter::Level6()
	{
#include "FTDressL6.inl"
	}
	void FTableWriter::Level7()
	{
#include "FTDressL7.inl"
	}
}

void FTDress::Build(int32 Level, FFTDress& Out)
{
	static const TCHAR* Dirs[] = {
		TEXT("/Game/Maps/Bayshore/Dress/"), TEXT("/Game/Maps/ChemicalWorks/Dress/"), TEXT("/Game/Maps/Nevada/Dress/"), TEXT("/Game/Maps/Volcano/Dress/"),
		TEXT("/Game/Maps/ThemePark/Dress/"), TEXT("/Game/Maps/Glacier/Dress/"), TEXT("/Game/Maps/Abyss/Dress/"), TEXT("/Game/Maps/Metro/Dress/") };
	FTableWriter W(Out, Dirs[FMath::Clamp(Level, 0, 7)]);
	switch (Level)
	{
	case 0: W.Level0(); break;
	case 1: W.Level1(); break;
	case 2: W.Level2(); break;
	case 3: W.Level3(); break;
	case 4: W.Level4(); break;
	case 5: W.Level5(); break;
	case 6: W.Level6(); break;
	default: W.Level7(); break;
	}
}
