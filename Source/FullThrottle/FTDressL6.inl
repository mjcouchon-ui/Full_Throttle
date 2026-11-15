// 3.2 dressing table, level 6: Under the Sea (the road to Nereida). The words (Set, Zone, Scatter, Site, Far, Afloat,
// SiteAfloat, Lit) are explained at the top of FTTrackDress.cpp; the meshes are made by
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
	Scatter(TEXT("SM_SeaGrass"), 17.5f, 21, 1, 0, 0.75f, 1.f, 0.2f, 0, 20.f);
	Scatter(TEXT("SM_Anemones"), 18, 24, 2, 1, 0.8f, 1.f, 0.2f, 0, 30.f);
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
	Scatter(TEXT("SM_SeaGrass2"), 17.5f, 21, 1, 0, 0.85f, 1.f, 0.2f, 0, 20.f);
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
