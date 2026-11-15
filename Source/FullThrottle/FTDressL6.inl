// 3.2 dressing table, level 6: Under the Sea (the road to Nereida). The words (Set, Zone, Scatter, Site, Far, Afloat,
// SiteAfloat, Lit, Bank) are explained at the top of FTTrackDress.cpp; the meshes are made by
// Content/Python/ft_dress_abyss.py. Distances across are metres from the road's centre line; districts are in
// kilometres along the road.
//
// The lie of the land: pale sand to 90 m either side, then the trench walls climbing to 14 m at 300 m, where the land
// ends. The level's own props keep their places (kelp and coral by the road, a dome or a sea rock now and then); the
// swimming schools, the whale, the ray, the shafts of light and the bubbles are placed by FTTrack.cpp's own rules.
// The dressing gives the route six kinds of seabed, one after another, and something to see out in the blue.
// In layers: the verge (17-24 m: grass, clams, anemones, low reef: nothing over 2 m), the near bottom (26-60 m: coral
// stands, kelp, cargo, vents), the middle (60-130 m: reef terraces, the station, the salvage, the wrecks), the trench
// walls (to 300 m), and the horizon (sea mounts, reef walls, kelp walls, a broken tanker, Nereida).
// A Site with side 0 takes the two sides in turn; most sets here are given once for each side instead.

Set(TEXT("Pipe"), { { TEXT("SM_Pipeline"), 0, 0, 0, 0, 1 } });
Set(TEXT("Tube"), { { TEXT("SM_TubeRun"), 0, 0, 0, 0, 1 } });
Set(TEXT("ReefGarden"), {
	{ TEXT("SM_ReefShelf"),    0,   0, 0,   0, 1 },
	{ TEXT("SM_CoralGarden"), 22,  -9, 0,  40, 1 },
	{ TEXT("SM_Anemones"),   -18, -10, 0,   0, 1 },
	{ TEXT("/Game/Maps/Abyss/Props/SM_Coral"), 10, -14, 0, 0, 1.3f },
	{ TEXT("SM_ReefLow2"),   -27,   6, 0, 110, 1.2f },
	{ TEXT("SM_CoralGarden2"), -6, 17, 0, 200, 1.2f },
});
Set(TEXT("KelpStand"), {
	{ TEXT("SM_KelpForest2"),  0,   0, 0,   0, 1 },
	{ TEXT("SM_KelpForest"),  26, -10, 0,  30, 1 },
	{ TEXT("SM_KelpForest"), -24,  12, 0, -40, 1.1f },
	{ TEXT("SM_ReefLow"),      8, -22, 0,   0, 1 },
});
Set(TEXT("Outpost"), {                                    // a research station: a hull, its tower, a submarine on its cradle
	{ TEXT("SM_Station"),      0,   0, 0,   0, 1 },
	{ TEXT("SM_StationHub"),  32,  14, 0,   0, 1 },
	{ TEXT("SM_Submarine"),  -34,  -6, 0,  15, 1 },
	{ TEXT("SM_MarkerPost"),  14, -20, 0,   0, 1 },
	{ TEXT("SM_MarkerPost"), -12, -20, 0,   0, 1 },
});
Set(TEXT("Habitat"), {                                    // where the station's people live: two domes and a garden
	{ TEXT("SM_DomeHabitat"),  0,   0, 0,   0, 1 },
	{ TEXT("SM_DomeHabitat"), 24,  10, 0, 140, 0.7f },
	{ TEXT("SM_CoralGarden"), -16,  -4, 0,  0, 1 },
	{ TEXT("SM_MarkerPost"),   8, -17, 0,   0, 1 },
});
Set(TEXT("Salvage"), {                                    // a salvage frame over a find, and what has been brought to it
	{ TEXT("SM_SalvageRig"),   0,   0, 0,   0, 1 },
	{ TEXT("SM_Containers"),  24,  -8, 0,  20, 1 },
	{ TEXT("SM_Anchor"),     -22, -12, 0, -30, 1 },
	{ TEXT("SM_Crates"),      10, -22, 0,   0, 1 },
	{ TEXT("SM_Containers"), -31,  14, 0,  70, 1 },
});
Set(TEXT("Graveyard"), {                                  // where the ships and the plane lie
	{ TEXT("SM_WreckBow"),     0,   0, 0,   0, 1 },
	{ TEXT("SM_PlaneTail"),   46, -12, 0,  25, 1 },
	{ TEXT("SM_Anchor"),     -35, -16, 0,  60, 1 },
	{ TEXT("SM_Containers"),  20, -28, 0, -20, 1 },
});
Set(TEXT("OldWreck"), {                                   // an older loss
	{ TEXT("SM_Galleon"),      0,   0, 0,   0, 1 },
	{ TEXT("SM_Anchor"),      30,  -9, 0, 110, 0.8f },
	{ TEXT("SM_Crates"),     -24, -12, 0,   0, 1.2f },
	{ TEXT("SM_CoralGarden2"), 12, -16, 0,  0, 1 },
});
Set(TEXT("Vents"), {
	{ TEXT("SM_VentField"),    0,   0, 0,   0, 1 },
	{ TEXT("SM_VentField"),   17,   8, 0, 120, 0.7f },
	{ TEXT("SM_ReefLow2"),   -15,  -6, 0,   0, 1 },
});
Set(TEXT("Guard"), {                                      // Nereida's outworks: a watch tower, the marker lights at its foot
	{ TEXT("SM_WatchTower"),   0,   0, 0,   0, 1 },
	{ TEXT("SM_MarkerPost"),   9,  -9, 0,   0, 1 },
	{ TEXT("SM_MarkerPost"),  -9,  -9, 0,   0, 1 },
});

// ---- SECOND PASS (art pass 2, 15 Nov 2026): its sets. Its districts are at the end of the table, laid over the
// first pass's (nothing was put in between the first pass's rules: a rule's random numbers come from its place in
// the table, so the first pass's pieces stand where they stood).
Set(TEXT("Trench"), { { TEXT("SM_CableTrench"), 0, 0, 0, 0, 1 } });
Set(TEXT("Apron"), { { TEXT("SM_SubPad"), 0, 0, 0, 0, 1 } });
Set(TEXT("ApronSub"), {                                   // the station's apron with a submarine standing on it
	{ TEXT("SM_SubPad"),      0,  0, 0,   0, 1 },
	{ TEXT("SM_Submarine"),   0,  0, 0,  20, 1 },
});

// ---- 0 to 1.7 km: THE REEF. Bright and shallow-looking: terraces and tall stands of coral, clams and anemones at
// the barrier, an arch, jellyfish in the water.
Zone(TEXT("The reef"), 0.0f, 1.7f);
	Scatter(TEXT("SM_SeaGrass"), 17.5f, 21, 1, 0, 1.f, 1.f, 0.2f, 0, 20.f);          // (round three: certain. 0.75 left runs of bare verge)
	Scatter(TEXT("SM_Anemones"), 18, 24, 2, 1, 1.f, 1.f, 0.2f, 0, 30.f);             // (round three: certain; they wave now)
	Scatter(TEXT("SM_GiantClam"), 18, 23, 3, 2, 0.8f, 1.f, 0.2f, 0, 60.f);
	Scatter(TEXT("SM_ReefLow"), 19, 42, 1, 0, 0.6f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_CoralGarden"), 28, 46, 1, 0, 0.75f, 1.f, 0.25f, 0, 180.f);
	Scatter(TEXT("SM_CoralGarden2"), 34, 60, 2, 1, 0.7f, 1.3f, 0.25f, 0, 180.f);
	Site(TEXT("ReefGarden"), -1, 68, 2, 0, 0.9f);
	Site(TEXT("ReefGarden"), 1, 72, 2, 1, 0.9f);
	Scatter(TEXT("SM_ReefShelf2"), 110, 230, 1, 0, 0.7f, 1.4f, 0.3f, 0, 40.f);
	Scatter(TEXT("SM_ReefArch"), 105, 170, 3, 1, 0.9f, 1.2f, 0.2f, 0, 30.f);
	Scatter(TEXT("SM_Jellyfish"), 30, 110, 2, 0, 0.7f, 1.f, 0.3f, 0, 180.f);
	Far(TEXT("SM_FarReef"), 330, 560, 3, 0, 0.9f, 1.f, 0.25f, 0, 8.f, 25.f);
	Far(TEXT("SM_SeaMount2"), 360, 700, 4, 1, 0.9f, 1.f, 0.3f, 0, 10.f, 180.f);
	Far(TEXT("SM_FarKelp"), 420, 800, 7, 3, 0.9f, 1.f, 0.2f, 0, 10.f, 30.f);

// ---- 1.7 to 3.4 km: THE KELP FOREST. The road runs between walls of weed that close in overhead.
Zone(TEXT("The kelp forest"), 1.7f, 3.4f);
	Scatter(TEXT("SM_SeaGrass2"), 17.5f, 21, 1, 0, 1.f, 1.f, 0.2f, 0, 20.f);         // (round three: certain)
	Scatter(TEXT("SM_ReefLow2"), 19, 30, 3, 1, 0.6f, 1.f, 0.3f, 0, 180.f);
	Site(TEXT("KelpStand"), -1, 56, 1, 0, 0.85f, 1.f, 0.2f);
	Site(TEXT("KelpStand"), 1, 58, 1, 0, 0.85f, 1.f, 0.2f);
	Scatter(TEXT("SM_KelpForest"), 27, 44, 1, 0, 0.7f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_KelpForest2"), 110, 270, 1, 0, 0.8f, 1.3f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_Jellyfish2"), 30, 100, 3, 1, 0.7f, 1.f, 0.3f, 0, 180.f);
	Far(TEXT("SM_FarKelp"), 320, 620, 2, 0, 0.9f, 1.f, 0.25f, 0, 12.f, 30.f);
	Far(TEXT("SM_SeaMount2"), 600, 1000, 9, 4, 1.f, 1.2f, 0.3f, 0, 10.f, 180.f);

// ---- 3.4 to 5.0 km: THE RESEARCH STATION. A pipeline on one verge and a lit travel tube on the other run from
// module to module; domes and hulls stand back from them, towers ringed with light behind, survey posts on the verge.
Zone(TEXT("The research station"), 3.4f, 5.0f);
	Site(TEXT("Pipe"), -1, 19.5f, 1, 0, 1.f);
	Site(TEXT("Tube"), 1, 33, 1, 0, 1.f);
	Site(TEXT("Outpost"), 1, 80, 4, 0, 1.f);
	Site(TEXT("Outpost"), -1, 84, 4, 2, 1.f);
	Site(TEXT("Habitat"), -1, 52, 4, 0, 1.f);
	Site(TEXT("Habitat"), 1, 58, 4, 2, 1.f);
	Scatter(TEXT("SM_MarkerPost"), 18, 21, 3, 1, 0.9f, 1.f, 0.f, 1); Lit();
	Scatter(TEXT("SM_SeaGrass"), 17.5f, 21, 2, 0, 0.7f, 1.f, 0.2f, 1, 20.f);
	Scatter(TEXT("SM_StationHub"), 150, 255, 2, 1, 0.8f, 1.3f, 0.2f, 0);
	Scatter(TEXT("SM_DomeHabitat"), 110, 200, 3, 0, 0.8f, 1.5f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_Submarine"), 44, 60, 9, 5, 1.f, 1.f, 0.f, 0, 25.f);
	Scatter(TEXT("SM_Anemones"), 18, 24, 4, 3, 0.6f, 1.f, 0.2f, 1, 30.f);
	Scatter(TEXT("SM_Jellyfish"), 40, 120, 4, 2, 0.7f, 1.f, 0.3f, 0, 180.f);
	Far(TEXT("SM_SeaMount"), 420, 800, 5, 2, 1.f, 1.f, 0.3f, 0, 10.f, 180.f);
	Far(TEXT("SM_FarReef"), 340, 600, 6, 3, 0.9f, 1.f, 0.25f, 0, 8.f, 25.f);

// ---- 5.0 to 6.6 km: THE SALVAGE FIELD. Cargo strewn to the verge, frames over the finds, the dead ships beyond: a
// freighter's bow, a plane, an old sailing ship.
Zone(TEXT("The salvage field"), 5.0f, 6.6f);
	Site(TEXT("Salvage"), -1, 70, 3, 0, 0.95f);
	Site(TEXT("Salvage"), 1, 74, 3, 1, 0.95f);
	Site(TEXT("Graveyard"), -1, 125, 5, 2, 1.f);
	Site(TEXT("OldWreck"), 1, 118, 5, 4, 1.f);
	Site(TEXT("OldWreck"), -1, 60, 7, 5, 1.f);
	Scatter(TEXT("SM_Crates"), 19, 40, 1, 0, 0.7f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_SeaGrass2"), 17.5f, 21, 2, 1, 0.7f, 1.f, 0.2f, 0, 20.f);
	Scatter(TEXT("SM_Containers"), 42, 110, 1, 0, 0.6f, 1.f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_Anchor"), 26, 60, 3, 2, 0.8f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_WreckBow"), 170, 270, 3, 1, 0.9f, 1.3f, 0.3f, 0, 90.f);
	Scatter(TEXT("SM_Galleon"), 150, 250, 4, 3, 0.9f, 1.4f, 0.3f, 0, 90.f);
	Far(TEXT("SM_FarWreck"), 360, 620, 9, 3, 1.f, 1.f, 0.2f, 0, 14.f, 50.f);
	Far(TEXT("SM_SeaMount2"), 500, 900, 6, 1, 1.f, 1.f, 0.3f, 0, 10.f, 180.f);

// ---- 6.6 to 7.94 km: THE APPROACH. The bottom smokes: black vents on both sides, arches and reef between them,
// the first of Nereida's watch towers, her domes ahead.
Zone(TEXT("The approach to Nereida"), 6.6f, 7.94f);
	Site(TEXT("Vents"), -1, 40, 2, 0, 0.95f);
	Site(TEXT("Vents"), 1, 44, 2, 1, 0.95f);
	Site(TEXT("Guard"), -1, 66, 4, 1, 1.f); Lit();
	Site(TEXT("Guard"), 1, 70, 4, 3, 1.f); Lit();
	Site(TEXT("ReefGarden"), -1, 96, 3, 0, 0.9f);
	Site(TEXT("ReefGarden"), 1, 100, 3, 1, 0.9f);
	Scatter(TEXT("SM_SeaGrass"), 17.5f, 21, 2, 0, 0.7f, 1.f, 0.2f, 0, 20.f);
	Scatter(TEXT("SM_GiantClam"), 18, 23, 3, 1, 0.8f, 1.f, 0.2f, 0, 60.f);
	Scatter(TEXT("SM_ReefArch"), 60, 140, 3, 1, 0.9f, 1.2f, 0.2f, 0, 30.f);
	Scatter(TEXT("SM_StationHub"), 120, 260, 2, 0, 0.8f, 1.4f, 0.25f, 0);
	Scatter(TEXT("/Game/Maps/Abyss/Props/SM_Dome"), 150, 280, 2, 1, 0.8f, 1.f, 0.2f, 0);
	Scatter(TEXT("SM_KelpForest"), 27, 44, 3, 0, 0.6f, 1.f, 0.3f, 0, 180.f);
	Far(TEXT("/Game/Maps/Abyss/Props/SM_City"), 600, 1100, 9, 2, 1.f, 2.2f, 0.2f, 0, 30.f, 40.f);
	Far(TEXT("SM_SeaMount"), 420, 800, 4, 0, 1.f, 1.f, 0.3f, 0, 10.f, 180.f);

// ---- 7.94 km to the end: NEREIDA, the finale. The city's outworks on both sides: watch towers every 90 m, the
// pipeline and the tube, habitats, hulls, weed and reef between them, vents now and then.
Zone(TEXT("Nereida"), 7.94f, 99.f);
	Site(TEXT("Pipe"), 1, 19.5f, 1, 0, 1.f);
	Site(TEXT("Tube"), -1, 33, 1, 0, 1.f);
	Site(TEXT("Guard"), -1, 52, 4, 0, 1.f); Lit();
	Site(TEXT("Guard"), 1, 56, 4, 2, 1.f); Lit();
	Site(TEXT("Habitat"), 1, 60, 6, 1, 1.f);
	Site(TEXT("Habitat"), -1, 64, 6, 4, 1.f);
	Site(TEXT("Outpost"), -1, 96, 6, 1, 1.f);
	Site(TEXT("Outpost"), 1, 100, 6, 4, 1.f);
	Site(TEXT("KelpStand"), 0, 62, 3, 0, 0.8f, 1.f, 0.2f);
	Site(TEXT("ReefGarden"), 0, 78, 4, 3, 0.9f);
	Site(TEXT("Vents"), 0, 44, 7, 5, 0.9f);
	Site(TEXT("Salvage"), 0, 76, 9, 5, 0.9f);
	Scatter(TEXT("SM_StationHub"), 120, 260, 2, 0, 0.8f, 1.4f, 0.25f, 0);
	Scatter(TEXT("/Game/Maps/Abyss/Props/SM_Dome"), 150, 285, 2, 1, 0.85f, 1.1f, 0.2f, 0);
	Scatter(TEXT("SM_WatchTower"), 130, 240, 3, 2, 0.8f, 1.5f, 0.2f, 0);
	Scatter(TEXT("SM_MarkerPost"), 18, 21, 3, 1, 0.9f, 1.f, 0.f, -1); Lit();
	Scatter(TEXT("SM_SeaGrass2"), 17.5f, 21, 2, 0, 0.7f, 1.f, 0.2f, 0, 20.f);
	Scatter(TEXT("SM_Anemones"), 18, 24, 3, 2, 0.6f, 1.f, 0.2f, 0, 30.f);
	Scatter(TEXT("SM_CoralGarden"), 28, 46, 3, 1, 0.7f, 1.f, 0.25f, 0, 180.f);
	Scatter(TEXT("SM_Jellyfish"), 30, 110, 3, 0, 0.7f, 1.f, 0.3f, 0, 180.f);
	Far(TEXT("/Game/Maps/Abyss/Props/SM_City"), 600, 1100, 6, 0, 1.f, 2.2f, 0.2f, 0, 30.f, 40.f);
	Far(TEXT("SM_SeaMount"), 420, 800, 5, 3, 1.f, 1.f, 0.3f, 0, 10.f, 180.f);
	Far(TEXT("SM_FarKelp"), 340, 640, 6, 4, 0.9f, 1.f, 0.25f, 0, 12.f, 30.f);
	Far(TEXT("SM_FarReef"), 330, 560, 5, 2, 0.9f, 1.f, 0.25f, 0, 8.f, 25.f);
	Far(TEXT("SM_FarWreck"), 380, 640, 23, 11, 1.f, 1.f, 0.2f, 0, 14.f, 50.f);
// ================================================================================================= SECOND PASS
// Two things. First the seabed itself: from a tunnel's roof, a loop or a banked bend the camera looks down on the
// sand, and the sand between the reefs was bare. Cables in trenches run out from the road to the modules and the
// salvage frames (5 m wide along the road: they may lie on a slope).
Zone(TEXT("The research station, second pass"), 3.4f, 5.0f);
	Site(TEXT("Trench"), 1, 52, 4, 1, 1.f);                // out from the tube
	Site(TEXT("Trench"), -1, 52, 4, 3, 1.f);               // out from the pipeline
Zone(TEXT("The salvage field, second pass"), 5.0f, 6.6f);
	Site(TEXT("Trench"), -1, 48, 3, 0, 0.9f);              // an umbilical out to each salvage frame
	Site(TEXT("Trench"), 1, 50, 3, 1, 0.9f);
Zone(TEXT("Nereida, second pass"), 7.94f, 99.f);
	Site(TEXT("Trench"), 1, 50, 6, 1, 1.f);
	Site(TEXT("Trench"), -1, 54, 6, 4, 1.f);

// The flat pieces: reef pavement on the sand (SM_ReefFloor: plates of rock, plate and brain corals, starfish), landing
// aprons for the submarines, a whale's bones (ft_dress_abyss.py, "the third round"). The track stands a piece level
// at its middle and the seabed rises and falls with the road, so they go on LEVEL sand only: each district's rules
// are written once, as a function, and given the stretches of 45 m slots where the sand rises or falls no more than
// 20 cm within 25 m of the slot's middle (make_inl_L6.py in SkyScratch/DevLog/jam/art_pass2/w1 works them out from
// the level's sections; from the approach on, that is nearly all of them).
auto SlotKm = [](float S) { return S * 0.045243f; };                // a slot is 330 road units: 45.243 m
auto ReefLevel = [&](float A, float B)
{
	Zone(TEXT("The reef, level sand"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Scatter(TEXT("SM_ReefFloor"), 24, 58, 2, 0, 0.55f, 1.5f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_ReefFloor2"), 44, 88, 2, 1, 0.55f, 1.9f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_WhaleBones"), 34, 70, 9, 4, 1.f, 1.f, 0.1f, 0, 180.f);
};
ReefLevel(1, 4); ReefLevel(10, 14); ReefLevel(21, 29); ReefLevel(35, 36);
auto KelpLevel = [&](float A, float B)
{
	Zone(TEXT("The kelp forest, level sand"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Scatter(TEXT("SM_ReefFloor"), 24, 58, 2, 0, 0.55f, 1.5f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_ReefFloor2"), 44, 88, 2, 1, 0.55f, 1.9f, 0.3f, 0, 180.f);
};
KelpLevel(44, 51); KelpLevel(61, 61); KelpLevel(69, 70);
auto StationLevel = [&](float A, float B)
{
	Zone(TEXT("The research station, level sand"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Scatter(TEXT("SM_ReefFloor"), 24, 58, 2, 0, 0.55f, 1.5f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_ReefFloor2"), 44, 88, 2, 1, 0.55f, 1.9f, 0.3f, 0, 180.f);
	Site(TEXT("ApronSub"), 1, 52, 4, 3, 1.f);              // the slots the modules left free
	Site(TEXT("ApronSub"), -1, 50, 4, 1, 1.f);
};
StationLevel(77, 78); StationLevel(84, 93); StationLevel(99, 100); StationLevel(109, 110);
auto SalvageLevel = [&](float A, float B)
{
	Zone(TEXT("The salvage field, level sand"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Scatter(TEXT("SM_ReefFloor"), 24, 58, 2, 0, 0.55f, 1.5f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_ReefFloor2"), 44, 88, 2, 1, 0.55f, 1.9f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_WhaleBones"), 30, 60, 7, 3, 1.f, 1.1f, 0.1f, 0, 180.f);
};
SalvageLevel(111, 117); SalvageLevel(126, 127); SalvageLevel(132, 139);
auto ApproachLevel = [&](float A, float B)
{
	Zone(TEXT("The approach, level sand"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Scatter(TEXT("SM_ReefFloor"), 24, 58, 2, 0, 0.55f, 1.5f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_ReefFloor2"), 44, 88, 2, 1, 0.55f, 1.9f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_WhaleBones"), 34, 64, 9, 2, 1.f, 1.f, 0.1f, 0, 180.f);
};
ApproachLevel(147, 147); ApproachLevel(155, 155); ApproachLevel(161, 166); ApproachLevel(173, 175);
auto NereidaLevel = [&](float A, float B)
{
	Zone(TEXT("Nereida, level sand"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Scatter(TEXT("SM_ReefFloor"), 24, 58, 2, 0, 0.55f, 1.5f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_ReefFloor2"), 44, 88, 2, 1, 0.55f, 1.9f, 0.3f, 0, 180.f);
	Site(TEXT("Apron"), 1, 50, 12, 11, 1.f);
	Site(TEXT("Apron"), -1, 50, 12, 9, 1.f);
};
NereidaLevel(176, 309); NereidaLevel(315, 330); NereidaLevel(336, 351); NereidaLevel(357, 372); NereidaLevel(378, 393); NereidaLevel(399, 413); NereidaLevel(420, 434); NereidaLevel(441, 455); NereidaLevel(462, 476);
NereidaLevel(483, 497); NereidaLevel(504, 518); NereidaLevel(524, 539); NereidaLevel(545, 550);
// On the slopes of the first 8 km: the same pavement, smaller (12 m: on a 1 in 12 slope its ends are half a metre off
// the sand, which a ledge of rock can be).
auto ReefSlope = [&](float A, float B)
{
	Zone(TEXT("Sand on a slope"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Scatter(TEXT("SM_ReefFloor"), 24, 70, 2, 0, 0.6f, 0.9f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_ReefFloor2"), 40, 88, 2, 1, 0.6f, 1.f, 0.2f, 0, 180.f);
};
ReefSlope(5, 9); ReefSlope(30, 34); ReefSlope(52, 54); ReefSlope(59, 60); ReefSlope(66, 68); ReefSlope(94, 98); ReefSlope(118, 120); ReefSlope(124, 125); ReefSlope(156, 160);
// ---- Second, the shot: glass tunnels running past reef shelves, wreckage, service rigs and marine life. Scatter and
// Site rules (and the level's own kelp and coral) take no slot of a glass tunnel, a loop, a corkscrew or a jump's
// gap: 128 of the level's 550 slots, 104 of them tunnels, which ran through bare sand. Far rules stay through them.
// So each has a district of its own, of Far rules used as land rules: the distance is the place across, the drop
// stands the piece on the sand there (0.9 m under the road's level by the road, 1.0 at 60 m; the land climbs again
// past 90 m), the last number is the yaw jitter. What stands by a tunnel is its district's own: reef, kelp, the
// station, the wrecks, the vents, Nereida's towers. FTTrack's road test keeps anything tall 27 m and more from a
// tunnel's middle (the glass is 16 m out), so its mouths and what comes out of them stay clear. The sand under a
// tunnel, a loop and a corkscrew is level.
// The numbers are the 45 m slots flagged at their middles, first and last.
auto PipeSite = [&](float A, float B, const TCHAR* Low, const TCHAR* Mid, const TCHAR* Big, const TCHAR* Swim)
{
	Zone(TEXT("Glass tunnel"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Far(TEXT("SM_ReefFloor"), 23, 27, 1, 0, 0.7f, 1.2f, 0.2f, 0, 0.87f, 180.f);
	Far(Low, 34, 46, 1, 0, 0.8f, 1.f, 0.25f, 0, 0.94f, 180.f);
	Far(Mid, 50, 72, 1, 0, 0.75f, 1.f, 0.2f, 0, 1.02f, 40.f);
	Far(Big, 76, 104, 2, 0, 0.9f, 1.2f, 0.2f, 0, 0.6f, 40.f);
	Far(Swim, 30, 100, 2, 1, 0.7f, 1.f, 0.3f, 0, 0.9f, 180.f);
};
// a loop or a corkscrew: the reef goes on under it
auto SetPiece = [&](float A, float B)
{
	Zone(TEXT("Under a set piece"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Far(TEXT("SM_ReefFloor"), 30, 80, 1, 0, 0.9f, 1.6f, 0.3f, 0, 1.f, 180.f);
	Far(TEXT("SM_CoralGarden"), 34, 60, 1, 0, 0.8f, 1.f, 0.25f, 0, 0.98f, 180.f);
	Far(TEXT("SM_ReefLow2"), 28, 90, 1, 0, 0.8f, 1.2f, 0.3f, 0, 1.f, 180.f);
	Far(TEXT("SM_KelpForest"), 60, 100, 2, 0, 0.8f, 1.f, 0.3f, 0, 0.8f, 180.f);
};
// under a jump the sand dips 18 m, with sides as steep as 2 in 5: only single plants can stand there
auto JumpSite = [&](float A, float B)
{
	Zone(TEXT("Under the jump"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Far(TEXT("/Game/Maps/Abyss/Props/SM_Kelp"), 26, 70, 1, 0, 1.f, 1.f, 0.3f, 0, 1.3f, 180.f);
	Far(TEXT("/Game/Maps/Abyss/Props/SM_Kelp"), 60, 120, 1, 0, 1.f, 1.2f, 0.3f, 0, 1.2f, 180.f);
	Far(TEXT("/Game/Maps/Abyss/Props/SM_Coral"), 24, 80, 1, 0, 1.f, 1.2f, 0.3f, 0, 1.2f, 180.f);
	Far(TEXT("/Game/Maps/Abyss/Props/SM_Coral"), 40, 110, 1, 0, 1.f, 1.5f, 0.3f, 0, 1.2f, 180.f);
};
PipeSite(15, 20, TEXT("SM_CoralGarden"), TEXT("SM_ReefShelf"), TEXT("SM_ReefArch"), TEXT("SM_Jellyfish"));
PipeSite(37, 43, TEXT("SM_CoralGarden2"), TEXT("SM_KelpForest"), TEXT("SM_KelpForest2"), TEXT("SM_Jellyfish2"));
PipeSite(79, 83, TEXT("SM_Crates"), TEXT("SM_DomeHabitat"), TEXT("SM_StationHub"), TEXT("SM_Jellyfish"));
PipeSite(101, 108, TEXT("SM_Submarine"), TEXT("SM_DomeHabitat"), TEXT("SM_StationHub"), TEXT("SM_Jellyfish"));
PipeSite(140, 146, TEXT("SM_Crates"), TEXT("SM_Containers"), TEXT("SM_WreckBow"), TEXT("SM_Jellyfish2"));
PipeSite(167, 172, TEXT("SM_CoralGarden2"), TEXT("SM_VentField"), TEXT("SM_ReefArch"), TEXT("SM_Jellyfish"));
PipeSite(310, 314, TEXT("SM_CoralGarden"), TEXT("SM_DomeHabitat"), TEXT("SM_WatchTower"), TEXT("SM_Jellyfish"));
PipeSite(331, 335, TEXT("SM_Crates"), TEXT("SM_KelpForest"), TEXT("SM_StationHub"), TEXT("SM_Jellyfish2"));
PipeSite(352, 356, TEXT("SM_CoralGarden2"), TEXT("SM_VentField"), TEXT("SM_ReefShelf2"), TEXT("SM_Jellyfish"));
PipeSite(373, 377, TEXT("SM_CoralGarden"), TEXT("SM_DomeHabitat"), TEXT("SM_WatchTower"), TEXT("SM_Jellyfish2"));
PipeSite(394, 398, TEXT("SM_Submarine"), TEXT("SM_Containers"), TEXT("SM_Galleon"), TEXT("SM_Jellyfish"));
PipeSite(414, 419, TEXT("SM_CoralGarden2"), TEXT("SM_KelpForest"), TEXT("SM_StationHub"), TEXT("SM_Jellyfish2"));
PipeSite(435, 440, TEXT("SM_CoralGarden"), TEXT("SM_ReefShelf"), TEXT("SM_ReefArch"), TEXT("SM_Jellyfish"));
PipeSite(456, 461, TEXT("SM_Crates"), TEXT("SM_DomeHabitat"), TEXT("SM_WatchTower"), TEXT("SM_Jellyfish2"));
PipeSite(477, 482, TEXT("SM_CoralGarden2"), TEXT("SM_VentField"), TEXT("SM_WreckBow"), TEXT("SM_Jellyfish"));
PipeSite(498, 503, TEXT("SM_CoralGarden"), TEXT("SM_KelpForest"), TEXT("SM_StationHub"), TEXT("SM_Jellyfish2"));
PipeSite(519, 523, TEXT("SM_Submarine"), TEXT("SM_DomeHabitat"), TEXT("SM_WatchTower"), TEXT("SM_Jellyfish"));
PipeSite(540, 544, TEXT("SM_CoralGarden2"), TEXT("SM_ReefShelf"), TEXT("SM_ReefArch"), TEXT("SM_Jellyfish2"));
SetPiece(62, 65); SetPiece(71, 76); SetPiece(128, 131); SetPiece(148, 154);
JumpSite(57, 57); JumpSite(122, 123);

// ---- THE CUT BANKS (15 Nov 2026, after the second pass). Inside a banked bend the track cuts the seabed down to the
// road's low edge; that cutting was one pale wall of sand, a third of the frame from the rolled camera ("the big pale
// wall at 20 s"), and no Scatter or Site piece can stand on it. Bank rules (FTDress.h) have the track lay panels on it
// itself, fitted to the bank's own angle and grade: four to a 45 m slot (11.3 m of road each), tall designs where a
// panel's run up the cutting is 5.5 m or more (bends banked 32 degrees and up), a low one on the shallower cuttings
// (down to 2.4 m: about 20 degrees of bank). Every and phase count panels here. ft.DressBank 0 leaves them out.
// Under the Sea: the road cut through the reef. SM_CutReef and SM_CutReef2 (the same six beds of reef rock and
// packed sand, so they meet from panel to panel; the second has coral and weed on its ledges) take alternate panels
// of the deep cuttings; SM_CutShoal on the shallow ones. The glass tunnels have no cutting: nothing is laid there.
Zone(TEXT("Cut banks"), 0.f, 99.f);
	Bank(TEXT("SM_CutReef"), 4, 2, 0, 1.f, 5.5f);
	Bank(TEXT("SM_CutReef2"), 4, 2, 1, 1.f, 5.5f);
	Bank(TEXT("SM_CutShoal"), 4, 1, 0, 1.f, 2.4f, 5.5f);

// ================================================================================================= THIRD PASS
// The zone pass (the overhaul, 15 Nov 2026): "nearby pressure structures, marine silhouettes, and suspended scenery
// with schools of fish, rising bubbles, drifting vegetation, and moving caustics". Written by make_l6.py
// (SkyScratch/DevLog/jam/overhaul/zones_w1); ft_dress_abyss.py, "the fourth round", has the pieces.
// The bottom was dressed and the water over it was empty: from the chase camera the top half of the frame is the
// bright net of the surface, and nothing stood against it but shafts of light. This pass fills the water, in layers:
//   the verge (18-21 m)   SM_PodLow: pressure vessels on cradles, dark and unlit, under 2.6 m
//   24 to 50 m            SM_PressureHab (a sphere habitat and its tank); schools that turn (SM_FishRing) and stream
//                         along the road (SM_FishStream), never nearer than 30 m and never blue (the boss's torpedo
//                         fish are); rays in formation 13 m up (SM_MantaFlight)
//   30 to 200 m           what hangs in the water: buoyancy spheres on chains (SM_Tethered), rafts of weed with fronds
//                         trailing (SM_KelpRaft), columns of bubbles over vents (SM_BubbleVent), sharks on their
//                         rounds 12 to 21 m up (SM_SharkRing), and on level sand the net of light (SM_CausticNet)
//   the horizon           whales in three receding tones, 30 to 55 m up over the trench walls (SM_WhalePod), and the
//                         hulls of ships on the surface with their anchor cables (SM_HullAbove)
// This level's rule stands: nothing near the road may read as a pickup, and nothing blinks beside it.
Set(TEXT("Tornado"), {                                    // the fish tornado: three rings of the school, wider and larger as they go up
	{ TEXT("SM_FishRing"),     0, 0,  0,    0, 1.1f },
	{ TEXT("SM_FishRing"),     0, 0,  5.2f, 0, 1.6f },
	{ TEXT("SM_FishRing"),     0, 0, 12.5f, 0, 2.2f },
	{ TEXT("SM_CoralGarden2"), 0, 0,  0,    0, 1.6f },
});
Set(TEXT("GatePost"), {                                   // one side of Nereida's gate: a watch tower twice the size, its habitat, spheres on chains
	{ TEXT("SM_WatchTower"),    0,  0, 0,  0, 1.9f },
	{ TEXT("SM_PressureHab"), -24, -4, 0, 30, 1.25f },
	{ TEXT("SM_Tethered"),     22, 14, 0,  0, 1.2f },
});

// No rule of this pass has a chance under 1. A chance is not a sprinkle in this game: the first random number of a
// slot and a rule steps by 0.039 from slot to slot and by 0.052 from rule to rule, so a chance of 0.6 is "15 slots
// on, 10 off", and rules written one after another are off at nearly the same slots (round one left 450 m of
// water empty at a time). Each rule has a longer step instead and its pieces come evenly.
Zone(TEXT("The reef, third pass"), 0.0f, 1.7f);
	Scatter(TEXT("SM_KelpRaft"), 34, 130, 3, 1, 1.f, 1.f, 0.25f, 0, 180.f);
	Scatter(TEXT("SM_FishRing"), 42, 70, 2, 0, 1.f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_FishStream"), 38, 50, 4, 2, 1.f, 1.f, 0.f, 0, 6.f);
	Scatter(TEXT("SM_SharkRing"), 90, 190, 9, 4, 1.f, 1.f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_BubbleVent"), 30, 100, 5, 3, 1.f, 1.f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_MantaFlight"), 36, 60, 11, 6, 1.f, 1.f, 0.15f, 0, 5.f);
Zone(TEXT("The kelp forest, third pass"), 1.7f, 3.4f);
	Scatter(TEXT("SM_KelpRaft"), 30, 140, 2, 0, 1.f, 1.1f, 0.3f, 0, 180.f);          // weed adrift over the forest
	Scatter(TEXT("SM_FishRing"), 42, 70, 3, 1, 1.f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_FishStream"), 38, 50, 4, 0, 1.f, 1.f, 0.f, 0, 6.f);
	Scatter(TEXT("SM_MantaFlight"), 36, 60, 9, 4, 1.f, 1.f, 0.15f, 0, 5.f);
Zone(TEXT("The research station, third pass"), 3.4f, 5.0f);
	Scatter(TEXT("SM_PodLow"), 18.3f, 21, 2, 0, 1.f, 1.f, 0.f, 1, 4.f);              // the right verge (the pipeline has the left)
	Scatter(TEXT("SM_PressureHab"), 33, 38, 2, 1, 1.f, 1.f, 0.1f, -1, 180.f);     // left, behind the pipeline
	Scatter(TEXT("SM_PressureHab"), 44, 50, 3, 2, 1.f, 1.f, 0.1f, 1, 180.f);         // right, behind the travel tube
	Scatter(TEXT("SM_Tethered"), 40, 110, 4, 0, 1.f, 1.f, 0.25f, 0, 180.f);
	Scatter(TEXT("SM_BubbleVent"), 36, 100, 5, 1, 1.f, 1.f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_FishRing"), 42, 80, 7, 3, 1.f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_FishStream"), 38, 50, 9, 2, 1.f, 1.f, 0.f, 0, 6.f);
Zone(TEXT("The salvage field, third pass"), 5.0f, 6.6f);
	Scatter(TEXT("SM_PodLow"), 18.3f, 21, 4, 1, 1.f, 1.f, 0.f, 0, 4.f);
	Scatter(TEXT("SM_PressureHab"), 33, 38, 6, 2, 1.f, 1.f, 0.1f, 0, 180.f);
	Scatter(TEXT("SM_Tethered"), 36, 110, 4, 0, 1.f, 1.f, 0.25f, 0, 180.f);          // floats over the finds
	Scatter(TEXT("SM_KelpRaft"), 34, 130, 6, 2, 1.f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_SharkRing"), 90, 190, 7, 3, 1.f, 1.f, 0.2f, 0, 180.f);          // sharks over the dead ships
	Scatter(TEXT("SM_FishRing"), 42, 70, 7, 1, 1.f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_FishStream"), 38, 50, 7, 3, 1.f, 1.f, 0.f, 0, 6.f);
	Scatter(TEXT("SM_BubbleVent"), 30, 100, 6, 4, 1.f, 1.f, 0.2f, 0, 180.f);
Zone(TEXT("The approach, third pass"), 6.6f, 7.94f);
	Scatter(TEXT("SM_PodLow"), 18.3f, 21, 3, 0, 1.f, 1.f, 0.f, 0, 4.f);
	Scatter(TEXT("SM_PressureHab"), 33, 38, 3, 1, 1.f, 1.f, 0.1f, 0, 180.f);
	Scatter(TEXT("SM_Tethered"), 36, 110, 4, 2, 1.f, 1.f, 0.25f, 0, 180.f);
	Scatter(TEXT("SM_BubbleVent"), 30, 110, 3, 0, 1.f, 1.f, 0.2f, 0, 180.f);         // the bottom smokes here: bubbles on both sides
	Scatter(TEXT("SM_SharkRing"), 90, 190, 8, 5, 1.f, 1.f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_FishRing"), 42, 70, 7, 2, 1.f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_KelpRaft"), 34, 130, 8, 3, 1.f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_MantaFlight"), 36, 54, 10, 7, 1.f, 1.f, 0.15f, 0, 5.f);
Zone(TEXT("Nereida, third pass"), 7.94f, 99.f);
	Scatter(TEXT("SM_PodLow"), 18.3f, 21, 4, 0, 1.f, 1.f, 0.f, -1, 4.f);             // the left verge (the pipeline has the right)
	Scatter(TEXT("SM_PressureHab"), 33, 38, 6, 3, 1.f, 1.f, 0.1f, 1, 180.f);      // right (slot 178 stays free: the habitat of the gate stands there)
	Scatter(TEXT("SM_PressureHab"), 44, 50, 6, 5, 1.f, 1.f, 0.1f, -1, 180.f);        // left, behind the travel tube
	Scatter(TEXT("SM_Tethered"), 40, 120, 8, 1, 1.f, 1.f, 0.25f, 0, 180.f);
	Scatter(TEXT("SM_KelpRaft"), 36, 140, 10, 3, 1.f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_FishRing"), 42, 80, 8, 4, 1.f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_FishStream"), 38, 50, 10, 2, 1.f, 1.f, 0.f, 0, 6.f);             // between the tube and the watch towers
	Scatter(TEXT("SM_BubbleVent"), 40, 110, 10, 6, 1.f, 1.f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_SharkRing"), 100, 200, 11, 5, 1.f, 1.f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_MantaFlight"), 68, 92, 12, 7, 1.f, 1.f, 0.15f, 0, 5.f);         // beyond the watch towers (40 m tall at 52-56 m)
// The far water. The trench walls stand 14 m over the road at 300 m: the whales swim 28 to 52 m over the road's level,
// clear of that crest from the chase camera; the hulls float on the surface (a Far piece's base is its drop under the road).
Zone(TEXT("In the water, far out"), 0.f, 99.f);
	Far(TEXT("SM_WhalePod"), 170, 380, 8, 3, 1.f, 1.f, 0.3f, 0, 5.f, 40.f);            // (round one: 300 to 600 m, every 17th slot: in the real game the haze had them, and few frames held one)
	Far(TEXT("SM_HullAbove"), 260, 480, 16, 8, 1.f, 1.f, 0.2f, 0, 25.f, 180.f);          // (round one: 420 to 900 m: past the haze, or over the top of the frame)
// Moving caustics. The net of light lies flat, 8 cm over the sand, so it goes on level sand only (the land rises or
// falls no more than 20 cm within 17 m of the slot's middle).
// Round three: DROPPED (every call below has a chance of 0.f; the rules keep their places in the table). Judged from
// the road: a net lies flat on level sand 57 m and more out and the camera rides 3.5 m up, so it was a pale sliver
// 1 degree high that no frame showed as a net. Its pieces went to things that are seen moving.
auto LightNet = [&](float A, float B, int32 Every, float Chance)
{
	Zone(TEXT("Light on the sand"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Scatter(TEXT("SM_CausticNet"), 57, 92, Every, 0, Chance, 1.2f, 0.25f, 0, 180.f);   // its middle from 57 m: a net is 30 x 20 m, up to half as large again and turned any way, and no corner of it comes nearer than 30 m
};
LightNet(1, 4, 3, 0.f); LightNet(10, 14, 3, 0.f); LightNet(21, 29, 3, 0.f); LightNet(35, 36, 3, 0.f); LightNet(44, 52, 3, 0.f); LightNet(61, 61, 3, 0.f);
LightNet(69, 70, 3, 0.f); LightNet(77, 78, 3, 0.f); LightNet(84, 93, 3, 0.f); LightNet(99, 100, 3, 0.f); LightNet(109, 117, 3, 0.f); LightNet(126, 127, 3, 0.f);
LightNet(132, 139, 3, 0.f); LightNet(147, 147, 3, 0.f); LightNet(155, 155, 3, 0.f); LightNet(161, 166, 3, 0.f); LightNet(173, 175, 3, 0.f); LightNet(176, 309, 7, 0.f);
LightNet(315, 330, 7, 0.f); LightNet(336, 351, 7, 0.f); LightNet(357, 372, 7, 0.f); LightNet(378, 393, 7, 0.f); LightNet(399, 413, 7, 0.f); LightNet(420, 434, 7, 0.f);
LightNet(441, 455, 7, 0.f); LightNet(462, 476, 7, 0.f); LightNet(483, 497, 7, 0.f); LightNet(504, 518, 7, 0.f); LightNet(524, 539, 7, 0.f); LightNet(545, 550, 7, 0.f);
// ---- The signature moments, each in a district of one slot.
// The fish tornado (slot 6, 0.27 km, the left): the school in three rings, 5 to 32 m up, turning; the first big
// thing of the level, on the outside of its first curve.
Zone(TEXT("The fish tornado"), 0.2488f, 0.2941f);
	Site(TEXT("Tornado"), -1, 51, 1, 0, 1.f);
// The salvage ship (slot 115, 5.20 km): a hull on the surface right over the field, her anchor cable coming down among the wrecks.
Zone(TEXT("The salvage ship"), 5.1803f, 5.2256f);
	Far(TEXT("SM_SalvageShip"), 115, 125, 1, 0, 1.f, 1.f, 0.f, -1, 25.f, 20.f);     // (round three: the hull with her diving bell and hoist at work under her)
// Nereida's gate (slot 179, 8.10 km, dead ahead of the last tunnel's mouth): a watch tower twice the size on each side,
// and at slot 181 the whales: a cow, her calf and a second cow cross over the road 38 m up, all the time (SM_WhalePass:
// its base is 38 m OVER the road's level, a negative drop, and nothing of it comes lower than 31 m).
Zone(TEXT("The gate of Nereida"), 8.0759f, 8.1211f);
	Site(TEXT("GatePost"), -1, 52, 1, 0, 1.f);               // (where the level's own watch towers stand: their lamps blink)
	Site(TEXT("GatePost"), 1, 50, 1, 0, 1.f);
Zone(TEXT("The whales over the gate"), 8.1664f, 8.2116f);
	Far(TEXT("SM_WhalePass"), 0, 0, 1, 0, 1.f, 1.f, 0.f, 1, -38.f, 0.f);
// ---- Round three (15 Nov): the built world in motion. Props that move by their own materials (ft_dress_abyss.py,
// "the fifth round"), written after the table's last rule so that no rule before them changes its place. No divers
// and no new creatures (another machine's); the kit's schools, sharks, rays and whales stay as they were.
//   SM_TubeRun      (the same rules as before) is rebuilt with capsules travelling the tube, one to a length
//   SM_SalvageShip  (in the salvage ship's own rule, above) the hull with her diving bell and her hoist at work
//   SM_SubLane      mini-subs flying between two docking masts 136 m apart, 30 and 38 m up: through Nereida, 90 m out,
//                   every sixth slot, left and right in turn (corridor.py in the notes folder: at 90 m and that
//                   height nothing of the kit stands in their way but three spheres on chains and a wreck's bow)
//   SM_SupplyRun    supply subs with cargo in tow crossing 40 m over the road: over the kelp bend (the empty water of
//                   the 35 s frame) and over the research station's straight. None in Nereida: that is the boss's
//                   arena (nothing crosses over the fight), and its watch towers stand 42 m and taller
//   SM_ShaftSway    shafts of light whose feet wander: every second slot on the first 3.4 km, both sides, every ninth
//                   after; their middles 44 m and more out (a shaft's foot reaches 17 m from its middle)
//   SM_SeaWhips     beds of sea whips and young kelp on the verge of the first 3.4 km, under 1.9 m and unlit, between
//                   the sea grass and the anemones (which are certain now, and wave)
Set(TEXT("SubLane"), { { TEXT("SM_SubLane"), 0, 0, 0, 0, 1 } });
Zone(TEXT("Nereida, mini-subs"), 7.94f, 99.f);
	Site(TEXT("SubLane"), 0, 90, 6, 2, 1.f);
Zone(TEXT("The first 3.4 km, light and weed"), 0.0f, 3.4f);
	Scatter(TEXT("SM_ShaftSway"), 44, 120, 2, 0, 1.f, 1.f, 0.25f, 0, 180.f);
	Scatter(TEXT("SM_SeaWhips"), 18.5f, 21.5f, 2, 1, 1.f, 1.f, 0.15f, 0, 12.f);
Zone(TEXT("Past 3.4 km, light that sways"), 3.4f, 99.f);
	Scatter(TEXT("SM_ShaftSway"), 50, 130, 9, 4, 1.f, 1.f, 0.25f, 0, 180.f);
auto Supply = [&](float S)
{
	Zone(TEXT("A supply run over the road"), SlotKm(S - 0.5f), SlotKm(S + 0.5f));
	Far(TEXT("SM_SupplyRun"), 0, 0, 1, 0, 1.f, 1.f, 0.f, 1, -40.f, 0.f);
};
Supply(48); Supply(100);
