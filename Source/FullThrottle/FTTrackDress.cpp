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
}

void FTDress::Build(int32 Level, FFTDress& Out)
{
	static const TCHAR* Dirs[] = {
		TEXT("/Game/Maps/Bayshore/Dress/"), TEXT("/Game/Maps/ChemicalWorks/Dress/"), TEXT("/Game/Maps/Nevada/Dress/"), TEXT("/Game/Maps/Volcano/Dress/"),
		TEXT("/Game/Maps/ThemePark/Dress/"), TEXT("/Game/Maps/Glacier/Dress/"), TEXT("/Game/Maps/Abyss/Dress/"), TEXT("/Game/Maps/Metro/Dress/") };
	const FString Dir = Dirs[FMath::Clamp(Level, 0, 7)];
	auto Path = [&Dir](const TCHAR* M) { return (M && M[0] == TEXT('/')) ? FString(M) : Dir + M; };
	auto Rule = [&Out]() -> FFTDressRule&
	{
		if (!Out.Zones.Num()) { FFTDressZone Z; Z.Name = TEXT("Everywhere"); Out.Zones.Add(Z); }
		return Out.Zones.Last().Rules.AddDefaulted_GetRef();
	};
	auto Set = [&Out, &Path](const TCHAR* Name, std::initializer_list<FDP> Parts)
	{
		FFTDressSet& S = Out.Sets.AddDefaulted_GetRef();
		S.Name = Name;
		for (const FDP& P : Parts) { FFTDressPart Q; Q.Mesh = Path(P.Mesh); Q.Along = P.Along; Q.Out = P.Out; Q.Up = P.Up; Q.Yaw = P.Yaw; Q.Scale = P.Scale; S.Parts.Add(Q); }
	};
	auto Zone = [&Out](const TCHAR* Name, float FromKm, float ToKm)
	{
		FFTDressZone& Z = Out.Zones.AddDefaulted_GetRef();
		Z.Name = Name; Z.FromKm = FromKm; Z.ToKm = ToKm;
	};
	auto Scatter = [&Rule, &Path](const TCHAR* Mesh, float X0, float X1, int32 Every, int32 Phase, float Chance, float Scale = 1.f, float Jitter = 0.f, int32 Side = 0, float YawJitter = 0.f)
	{
		FFTDressRule& Q = Rule();
		Q.Kind = EFTDress::Scatter; Q.Mesh = Path(Mesh); Q.XMin = X0; Q.XMax = X1; Q.Every = Every; Q.Phase = Phase; Q.Chance = Chance;
		Q.Scale = Scale; Q.Jitter = Jitter; Q.Side = Side; Q.YawJitter = YawJitter;
	};
	auto Site = [&Rule](const TCHAR* SetName, int32 Side, float X, int32 Every, int32 Phase, float Chance, float Scale = 1.f, float Jitter = 0.f)
	{
		FFTDressRule& Q = Rule();
		Q.Kind = EFTDress::Site; Q.Mesh = SetName; Q.XMin = Q.XMax = X; Q.Side = Side; Q.Every = Every; Q.Phase = Phase; Q.Chance = Chance; Q.Scale = Scale; Q.Jitter = Jitter;
	};
	auto Far = [&Rule, &Path](const TCHAR* Mesh, float D0, float D1, int32 Every, int32 Phase, float Chance, float Scale = 1.f, float Jitter = 0.f, int32 Side = 0, float Drop = 25.f, float YawJitter = 30.f)
	{
		FFTDressRule& Q = Rule();
		Q.Kind = EFTDress::Far; Q.Mesh = Path(Mesh); Q.XMin = D0; Q.XMax = D1; Q.Every = Every; Q.Phase = Phase; Q.Chance = Chance;
		Q.Scale = Scale; Q.Jitter = Jitter; Q.Side = Side; Q.Drop = Drop; Q.YawJitter = YawJitter;
	};
	// Afloat / SiteAfloat: like Far and Site, but the piece (or the set) stands on the level's FLOOR, the sheet under
	// the whole level. On Bayshore that is the sea, so these are boats, a marina, islands: build them with the
	// waterline at z = 0. They are horizon things: no shadow, and they stay through loops and jumps.
	auto Afloat = [&Rule, &Path](const TCHAR* Mesh, float D0, float D1, int32 Every, int32 Phase, float Chance, float Scale = 1.f, float Jitter = 0.f, int32 Side = 0, float YawJitter = 180.f)
	{
		FFTDressRule& Q = Rule();
		Q.Kind = EFTDress::Far; Q.Mesh = Path(Mesh); Q.XMin = D0; Q.XMax = D1; Q.Every = Every; Q.Phase = Phase; Q.Chance = Chance;
		Q.Scale = Scale; Q.Jitter = Jitter; Q.Side = Side; Q.Drop = 0.f; Q.YawJitter = YawJitter; Q.bFloor = true;
	};
	auto SiteAfloat = [&Rule](const TCHAR* SetName, int32 Side, float X, int32 Every, int32 Phase, float Chance, float Scale = 1.f)
	{
		FFTDressRule& Q = Rule();
		Q.Kind = EFTDress::Site; Q.Mesh = SetName; Q.XMin = Q.XMax = X; Q.Side = Side; Q.Every = Every; Q.Phase = Phase; Q.Chance = Chance; Q.Scale = Scale; Q.bFloor = true;
	};
	auto Lit = [&Out]() { if (Out.Zones.Num() && Out.Zones.Last().Rules.Num()) Out.Zones.Last().Rules.Last().bLight = true; };
	(void)Set; (void)Zone; (void)Scatter; (void)Site; (void)Far; (void)Afloat; (void)SiteAfloat; (void)Lit;

	switch (Level)
	{
	case 0: {
#include "FTDressL0.inl"
	} break;
	case 1: {
#include "FTDressL1.inl"
	} break;
	case 2: {
#include "FTDressL2.inl"
	} break;
	case 3: {
#include "FTDressL3.inl"
	} break;
	case 4: {
#include "FTDressL4.inl"
	} break;
	case 5: {
#include "FTDressL5.inl"
	} break;
	case 6: {
#include "FTDressL6.inl"
	} break;
	default: {
#include "FTDressL7.inl"
	} break;
	}
}
