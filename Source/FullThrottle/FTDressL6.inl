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
