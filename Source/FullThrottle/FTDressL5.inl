// 3.2 dressing table, level 5: Ice Mountain (Frostbite Peak). The words (Set, Zone, Scatter, Site, Far) are explained
// at the top of FTTrackDress.cpp; the meshes are made by Content/Python/ft_dress_ice.py. Distances across are metres
// from the road's centre line; districts are in kilometres along the road and follow the level's own scenes.
//
// The lie of the land: the run goes downhill all the way. Snow lies level to 80 m either side, then climbs to 26 m at
// 300 m, where the land ends: the middle distance is a slope that faces the camera. The level's own props keep their
// places: pines from the barrier out to 146 m on both sides, a lift pylon every so often on the right at 82 m, a
// chalet on the left, a ridge now and then at 256 to 439 m.
// The dressing: a verge of snow fences, ploughed banks, markers and snow guns (17 to 23 m, all low, nothing lit),
// the resort's works on the slopes above the pines (150 to 290 m), ice and rock that close in where the scenes say
// so, and peaks in two depths past the land's edge. The top of the mountain, the pipes, the chute, the glacier, the
// exposed ridge, the whiteout and the valley each have their own mix.
//
// Scatter and Site rules are left out of the half pipes, loops, corkscrews and jumps, which is a third of this road.
// Under a jump the land dips 18 m, steeply, and a piece stands level: so the solid pieces go 7 m into the ground, and
// the flat ones (drift, crevasse fields, avalanche fences) are written in zones of their own that stop at the jump.
// So what should stay through those (the ice, the drifts, the avalanche fences) is written as Far rules with a
// negative drop: the drop puts the piece on the land at that distance (the land is 0.45 m below the road's ground
// level at 80 m and rises 12 cm a metre from there; a piece stands 15 cm into it), and the band is kept narrow so
// the one drop fits it. A mesh casts shadows unless its first piece comes from a Far rule, so the first district
// places each of those by Scatter.
//
// The finale's road runs on for 16 km, so it is laid out as three districts that come round twice: the glacier, the
// white ridge, the resort.

Set(TEXT("Fence"), { { TEXT("SM_SnowFence"), 0, 0, 0, 0, 1 } });
Set(TEXT("RescuePost"), {    // the mountain rescue: hut, landing pad, groomer, mast
	{ TEXT("SM_HutRescue"),           0,    0, 0,    0, 1 },
	{ TEXT("SM_Helipad"),            19,    4, 0,    0, 1 },
	{ TEXT("SM_Snowcat"),           -13,   -9, 0,   20, 1 },
	{ TEXT("SM_PoleSignal"),        -10,    9, 0,    0, 1 },
	{ TEXT("SM_Sled"),                7,   -9, 0,    0, 1 },
});
Set(TEXT("LiftLine"), {    // a chair lift up the slope: the anchor is where the slope begins (85 m)
	{ TEXT("SM_LiftStation"),         0,  -15, 0,   90, 1 },
	{ TEXT("SM_LiftSpan"),            0,    0, 0,    0, 1 },
	{ TEXT("SM_LiftSpan"),            0,   60, 0,    0, 1 },
	{ TEXT("SM_LiftSpan"),            0,  120, 0,    0, 1 },
	{ TEXT("SM_LiftTop"),             0,  185, 0,    0, 1 },
	{ TEXT("SM_SnowCannon"),         12,  -22, 0,   40, 1 },
});
Set(TEXT("Cutting"), {    // a gallery beside the road under a netted rock cut
	{ TEXT("SM_SnowShed"),            0,    0, 0,    0, 1 },
	{ TEXT("SM_RockCutting"),         0,    9, 0,    0, 1 },
	{ TEXT("SM_RockCutting"),        38, 9.5f, 0,    0, 1 },
});
Set(TEXT("IceFall"), {    // where the glacier breaks: walls, seracs, a frozen fall
	{ TEXT("SM_RockIceWall"),         0,    0, 0,    0, 1 },
	{ TEXT("SM_RockSeracB"),         31,   -8, 0,    0, 1 },
	{ TEXT("SM_RockIceFalls"),      -35,    6, 0,    0, 1 },
	{ TEXT("SM_RockIceWall2"),        8,   24, 0,    0, 1.2f },
	{ TEXT("SM_SnowRockIce"),        14,  -22, 0,    0, 1.5f },
});
Set(TEXT("SeracField"), {    // seracs over a crevasse field
	{ TEXT("SM_RockSeracB"),          0,    0, 0,    0, 1 },
	{ TEXT("SM_RockSeracH"),         23,   10, 0,    0, 1 },
	{ TEXT("SM_SnowRockCrevasse"),   -6,  -27, 0,    0, 1 },
	{ TEXT("SM_RockSeracB"),        -27,   14, 0,   60, 1.2f },
	{ TEXT("SM_SnowRockIce"),        10,  -14, 0,    0, 1.5f },
});
Set(TEXT("WeatherPost"), {    // the weather station on the ridge
	{ TEXT("SM_WeatherStation"),      0,    0, 0,    0, 1 },
	{ TEXT("SM_PoleSignal"),         15,    8, 0,    0, 1 },
	{ TEXT("SM_SnowRockDrift"),     -18,   -6, 0,    0, 1 },
	{ TEXT("SM_Sled"),                6,   -8, 0,    0, 1 },
});
Set(TEXT("PipeSide"), {    // by a half pipe: the judges' tower and benches, snow guns
	{ TEXT("SM_PipeStands"),          0,    0, 0,    0, 1 },
	{ TEXT("SM_SnowCannon"),         15,   -6, 0,    0, 1 },
	{ TEXT("SM_SnowCannon"),        -17,   -6, 0,    0, 1 },
});
Set(TEXT("Village"), {    // the valley: a lodge and chalets round it
	{ TEXT("SM_Lodge"),               0,    0, 0,    0, 1 },
	{ TEXT("/Game/Maps/Glacier/Props/SM_Chalet"),   25,    6, 0,   90, 1 },
	{ TEXT("/Game/Maps/Glacier/Props/SM_Chalet"),  -24,   10, 0,   90, 1 },
	{ TEXT("SM_Snowcat"),            13,  -15, 0,  -20, 1 },
	{ TEXT("SM_Sled"),               -9,  -13, 0,    0, 1 },
});

// ---- 0 to 1.19 km: FIRST TRACKS, the top of the mountain. A lift arrives from below on the left, the rescue post is
// on the right, the piste is marked and fenced, cornices hang under the edge, and the summit's observatory and the
// great peaks stand close.
Zone(TEXT("First tracks"), 0.f, 1.19f);
	Site(TEXT("Fence"), 0, 19, 2, 0, 0.9f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank"), 20.2f, 22, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 2, 0, 0.8f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_PoleMarker2"), 18, 19.5f, 3, 1, 0.7f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("LiftLine"), -1, 85, 8, 3, 1.f, 1.f, 0.f);
	Site(TEXT("RescuePost"), 1, 165, 9, 4, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockDrift"), 150, 262, 2, 0, 0.6f, 1.f, 0.3f, 0, 8.f);
	Scatter(TEXT("SM_SnowRockCrevasse"), 160, 250, 5, 2, 0.7f, 1.f, 0.2f, 0, 8.f);
	Scatter(TEXT("SM_RockSeracB"), 184, 260, 6, 3, 0.8f, 1.f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_RockSeracH"), 190, 260, 7, 5, 0.8f, 1.f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_RockIceShelf"), 230, 272, 4, 1, 0.8f, 1.f, 0.2f, 0, 8.f);
	Scatter(TEXT("SM_RidgeFences"), 236, 262, 9, 6, 1.f, 1.f, 0.f, 0, 6.f);
	Scatter(TEXT("SM_PoleSignal"), 200, 282, 7, 5, 1.f, 1.2f, 0.f, 0, 0.f);
	Far(TEXT("SM_PeakObservatory"), 430, 700, 9, 3, 1.f, 1.3f, 0.1f, 1, 25.f, 30.f);
	Far(TEXT("SM_PeakGreat"), 800, 1200, 8, 0, 1.f, 1.3f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_RidgeRange"), 820, 1200, 9, 2, 1.f, 1.7f, 0.2f, 0, 0.f, 8.f);

// ---- 1.19 to 2.43 km: THE HALF PIPE (then a jump). A competition pipe: judges' towers and benches at its ends, snow
// guns down the verge, groomers parked on the slopes, avalanche fences above; the great peaks show over its walls.
Zone(TEXT("The half pipe"), 1.19f, 2.43f);
	Site(TEXT("PipeSide"), 0, 33, 2, 0, 0.9f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 2, 1, 0.7f, 1.f, 0.f, 0, 15.f);
	Site(TEXT("Fence"), 0, 19, 3, 0, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_Snowcat"), 150, 225, 5, 2, 0.9f, 1.f, 0.f, 0, 180.f);
	Site(TEXT("RescuePost"), -1, 165, 11, 5, 1.f, 1.f, 0.f);
	Far(TEXT("SM_PeakLesser"), 520, 820, 9, 2, 1.f, 1.2f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_PeakGreat"), 800, 1200, 11, 4, 1.f, 1.4f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_RidgeRange"), 820, 1200, 8, 0, 1.f, 1.7f, 0.2f, 0, 0.f, 8.f);

// The same district, for what lies flat on the slopes (the land dips 18 m under a jump, and a piece stands level):
// avalanche fences and drift, through the pipe and as far as the jump's lip.
Zone(TEXT("The half pipe, above the jump"), 1.19f, 2.19f);
	Far(TEXT("SM_RidgeFences"), 160, 168, 3, 1, 0.7f, 1.f, 0.f, 0, -9.59f, 6.f);
	Far(TEXT("SM_RidgeFences"), 236, 244, 4, 2, 0.7f, 1.f, 0.f, 0, -18.68f, 6.f);
	Far(TEXT("SM_SnowRockDrift"), 150, 158, 3, 0, 0.6f, 1.f, 0.3f, 0, -8.4f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 250, 258, 3, 1, 0.6f, 1.f, 0.3f, 0, -20.35f, 8.f);

// ---- 2.43 to 2.96 km: AVALANCHE CHUTE (a corkscrew, then hard banked bends). Engineered against the mountain: four
// tiers of avalanche fence up both slopes (they stay through the corkscrew), galleries and netted rock cuts hard by
// the road, steep peaks close overhead.
Zone(TEXT("Avalanche chute"), 2.43f, 2.96f);
	Far(TEXT("SM_RidgeFences"), 148, 156, 1, 0, 0.9f, 1.f, 0.f, 0, -8.16f, 5.f);
	Far(TEXT("SM_RidgeFences"), 174, 182, 1, 0, 0.8f, 1.f, 0.f, 0, -11.27f, 5.f);
	Far(TEXT("SM_RidgeFences"), 200, 208, 1, 0, 0.8f, 1.f, 0.f, 0, -14.37f, 5.f);
	Far(TEXT("SM_RidgeFences"), 236, 244, 1, 0, 0.7f, 1.2f, 0.f, 0, -18.68f, 5.f);
	Site(TEXT("Cutting"), -1, 30, 2, 0, 1.f, 1.f, 0.f);
	Site(TEXT("Cutting"), 1, 30, 2, 1, 1.f, 1.f, 0.f);
	Far(TEXT("SM_PeakGreat"), 420, 600, 5, 0, 1.f, 1.f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_PeakLesser"), 400, 600, 5, 1, 1.f, 1.3f, 0.3f, 0, 25.f, 60.f);

// ---- 2.96 to 4.02 km: THE SUPER PIPE, cut through the glacier. Blue ice walls right outside the pipe's lip and along
// the road after it, ice falls, seracs and frozen falls up the slopes, and a glacier tongue coming down between the
// peaks.
Zone(TEXT("The super pipe"), 2.96f, 4.02f);
	Far(TEXT("SM_RockIceWall"), 38, 46, 1, 0, 0.7f, 1.f, 0.15f, 0, 0.69f, 6.f);
	Scatter(TEXT("SM_SnowRockIce"), 20.6f, 22.5f, 1, 0, 0.6f, 1.f, 0.2f, 0, 8.f);
	Site(TEXT("IceFall"), 0, 118, 1, 0, 0.85f, 1.f, 0.1f);
	Far(TEXT("SM_RockIceWall2"), 100, 110, 2, 0, 0.8f, 1.2f, 0.2f, 0, -2.54f, 6.f);
	Far(TEXT("SM_RockSeracB"), 150, 160, 1, 0, 0.6f, 1.3f, 0.3f, 0, -8.52f, 180.f);
	Far(TEXT("SM_RockIceFalls"), 196, 206, 4, 2, 0.9f, 1.2f, 0.f, 0, -14.02f, 10.f);
	Far(TEXT("SM_RockIceWall2"), 240, 250, 2, 1, 0.8f, 1.4f, 0.2f, 0, -19.27f, 6.f);
	Far(TEXT("SM_RidgeGlacier"), 380, 600, 5, 1, 1.f, 1.f, 0.15f, 0, 20.f, 14.f);
	Far(TEXT("SM_PeakGreat"), 800, 1200, 9, 3, 1.f, 1.4f, 0.3f, 0, 25.f, 60.f);

// ---- 4.02 to 5.65 km: BLACK DIAMOND, the exposed ridge (a loop, a jump, a corkscrew). Wind country: fences down the
// whole verge, cornices and carved drift on the slopes, crevasse fields, a weather post and signal masts, and the far
// ranges open on both sides. The slopes' dressing stays through the loop and the jump.
Zone(TEXT("Black diamond"), 4.02f, 5.65f);
	Site(TEXT("Fence"), 0, 19, 1, 0, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 0, 0.6f, 1.f, 0.2f, 0, 2.f);
	Far(TEXT("SM_RockIceShelf"), 138, 146, 2, 0, 0.9f, 1.f, 0.15f, 0, -6.96f, 8.f);
	Site(TEXT("WeatherPost"), 1, 172, 9, 3, 1.f, 1.f, 0.f);
	Far(TEXT("SM_PoleSignal"), 176, 184, 4, 1, 0.7f, 1.2f, 0.f, 0, -11.5f, 0.f);
	Far(TEXT("SM_RidgeRange"), 700, 1100, 5, 0, 1.f, 1.8f, 0.2f, 0, 0.f, 8.f);
	Far(TEXT("SM_PeakObservatory"), 500, 800, 11, 4, 1.f, 1.3f, 0.1f, 0, 25.f, 30.f);
	Far(TEXT("SM_RidgeLiftLine"), 420, 650, 9, 6, 1.f, 1.f, 0.1f, 0, 20.f, 8.f);

// The same district, for what lies flat on the slopes: carved drift and crevasse fields, through the loop and the
// corkscrew but not in the dip under the jump at 5 km.
Zone(TEXT("Black diamond, above the jump"), 4.02f, 4.91f);
	Far(TEXT("SM_SnowRockDrift"), 150, 158, 1, 0, 0.7f, 1.2f, 0.2f, 0, -8.4f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 204, 212, 2, 1, 0.7f, 1.2f, 0.2f, 0, -14.85f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 250, 256, 2, 0, 0.7f, 1.2f, 0.2f, 0, -20.23f, 8.f);
	Far(TEXT("SM_SnowRockCrevasse"), 170, 176, 2, 1, 0.7f, 1.f, 0.2f, 0, -10.67f, 8.f);
	Far(TEXT("SM_SnowRockCrevasse"), 226, 232, 3, 0, 0.7f, 1.f, 0.2f, 0, -17.36f, 8.f);

// The same district, for what lies flat on the slopes: carved drift and crevasse fields, through the loop and the
// corkscrew but not in the dip under the jump at 5 km.
Zone(TEXT("Black diamond, below the jump"), 5.12f, 5.65f);
	Far(TEXT("SM_SnowRockDrift"), 150, 158, 1, 0, 0.7f, 1.2f, 0.2f, 0, -8.4f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 204, 212, 2, 1, 0.7f, 1.2f, 0.2f, 0, -14.85f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 250, 256, 2, 0, 0.7f, 1.2f, 0.2f, 0, -20.23f, 8.f);
	Far(TEXT("SM_SnowRockCrevasse"), 170, 176, 2, 1, 0.7f, 1.f, 0.2f, 0, -10.67f, 8.f);
	Far(TEXT("SM_SnowRockCrevasse"), 226, 232, 3, 0, 0.7f, 1.f, 0.2f, 0, -17.36f, 8.f);

// ---- 5.65 to 7.3 km: THE WHITEOUT (a pipe, a loop, a corkscrew). Every gate on the mountain: markers thick along the
// verge, fences, a pipe with its stands, avalanche works and seracs on the slopes.
Zone(TEXT("The whiteout"), 5.65f, 7.3f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 1, 0, 0.8f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_PoleMarker2"), 18, 19.5f, 1, 0, 0.5f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("Fence"), 0, 20.5f, 1, 0, 0.7f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Site(TEXT("PipeSide"), 0, 33, 3, 1, 0.9f, 1.f, 0.f);
	Far(TEXT("SM_RidgeFences"), 152, 160, 3, 0, 0.9f, 1.f, 0.f, 0, -8.64f, 5.f);
	Far(TEXT("SM_RidgeFences"), 178, 186, 3, 0, 0.8f, 1.f, 0.f, 0, -11.74f, 5.f);
	Site(TEXT("SeracField"), 0, 200, 4, 2, 1.f, 1.f, 0.f);
	Far(TEXT("SM_RockSeracB"), 226, 234, 2, 1, 0.7f, 1.2f, 0.3f, 0, -17.48f, 180.f);
	Far(TEXT("SM_RockSeracH"), 252, 258, 3, 0, 0.7f, 1.2f, 0.2f, 0, -20.47f, 180.f);
	Far(TEXT("SM_SnowRockDrift"), 150, 158, 2, 1, 0.6f, 1.2f, 0.2f, 0, -8.4f, 8.f);
	Far(TEXT("SM_PeakLesser"), 520, 820, 6, 0, 1.f, 1.2f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_RidgeRange"), 850, 1200, 8, 2, 1.f, 1.7f, 0.2f, 0, 0.f, 8.f);

// ---- 7.3 to 8.94 km: THE VALLEY RUN, the descent (banked bends, a jump, the last pipe). Down among the resort: lodges
// and chalets on the slopes, lifts going up out of base stations on the left, groomers, a gallery now and then, a lift
// line on the far ridge.
Zone(TEXT("The valley run"), 7.3f, 8.94f);
	Site(TEXT("Fence"), 0, 19, 2, 0, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank"), 20.2f, 22, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);
	Site(TEXT("LiftLine"), -1, 85, 5, 1, 1.f, 1.f, 0.f);
	Site(TEXT("Village"), 1, 165, 4, 0, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_Lodge"), 152, 160, 5, 2, 0.9f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/Maps/Glacier/Props/SM_Chalet"), 190, 262, 2, 0, 0.7f, 1.f, 0.15f, 0, 40.f);
	Site(TEXT("Cutting"), 1, 30, 5, 3, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_Snowcat"), 150, 232, 4, 1, 0.8f, 1.f, 0.f, 0, 180.f);
	Far(TEXT("SM_RidgeLiftLine"), 400, 640, 5, 0, 1.f, 1.f, 0.1f, 0, 20.f, 8.f);
	Far(TEXT("SM_PeakGreat"), 800, 1200, 9, 2, 1.f, 1.4f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_PeakObservatory"), 520, 820, 13, 7, 1.f, 1.3f, 0.1f, 0, 25.f, 30.f);

// ---- 8.94 km to the end: THE FINALE, the Abominable's road, in three districts that come round twice.
// 8.94 to 11.6 km, THE GLACIER: ice walls by the road, ice falls and serac fields on the slopes, crevasses, glacier
// tongues between the peaks.
Zone(TEXT("The Abominable's glacier"), 8.94f, 11.6f);
	Far(TEXT("SM_RockIceWall"), 38, 46, 2, 0, 0.7f, 1.f, 0.15f, 0, 0.69f, 6.f);
	Far(TEXT("SM_RockIceWall2"), 62, 72, 3, 1, 0.7f, 1.f, 0.2f, 0, 0.53f, 6.f);
	Scatter(TEXT("SM_SnowRockIce"), 20.6f, 22.5f, 2, 0, 0.6f, 1.f, 0.2f, 0, 8.f);
	Scatter(TEXT("SM_SnowRockBank"), 20.2f, 22, 3, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_PoleMarker2"), 18, 19.5f, 4, 2, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("IceFall"), 0, 120, 3, 0, 0.9f, 1.f, 0.1f);
	Site(TEXT("SeracField"), 0, 205, 3, 1, 1.f, 1.f, 0.f);
	Far(TEXT("SM_SnowRockCrevasse"), 170, 176, 3, 2, 0.7f, 1.f, 0.2f, 0, -10.67f, 8.f);
	Far(TEXT("SM_RockIceFalls"), 250, 258, 4, 1, 0.9f, 1.2f, 0.f, 0, -20.35f, 10.f);
	Far(TEXT("SM_RidgeGlacier"), 380, 600, 6, 0, 1.f, 1.f, 0.15f, 0, 20.f, 14.f);
	Far(TEXT("SM_PeakGreat"), 800, 1200, 8, 2, 1.f, 1.4f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_PeakLesser"), 520, 820, 11, 3, 1.f, 1.2f, 0.3f, 0, 25.f, 60.f);

// 11.6 to 14.3 km, THE WHITE RIDGE: fences, cornices, carved drift and the weather post; the far ranges open.
Zone(TEXT("The white ridge"), 11.6f, 14.3f);
	Site(TEXT("Fence"), 0, 19, 1, 0, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 0, 0.6f, 1.f, 0.2f, 0, 2.f);
	Far(TEXT("SM_RockIceShelf"), 138, 146, 2, 0, 0.9f, 1.f, 0.15f, 0, -6.96f, 8.f);
	Site(TEXT("WeatherPost"), 0, 172, 9, 3, 1.f, 1.f, 0.f);
	Far(TEXT("SM_PoleSignal"), 176, 184, 5, 1, 0.7f, 1.2f, 0.f, 0, -11.5f, 0.f);
	Far(TEXT("SM_SnowRockDrift"), 150, 158, 1, 0, 0.7f, 1.2f, 0.2f, 0, -8.4f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 204, 212, 2, 1, 0.7f, 1.2f, 0.2f, 0, -14.85f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 250, 256, 2, 0, 0.7f, 1.2f, 0.2f, 0, -20.23f, 8.f);
	Far(TEXT("SM_SnowRockCrevasse"), 226, 232, 2, 0, 0.7f, 1.f, 0.2f, 0, -17.36f, 8.f);
	Far(TEXT("SM_RidgeRange"), 700, 1100, 5, 0, 1.f, 1.8f, 0.2f, 0, 0.f, 8.f);
	Far(TEXT("SM_PeakObservatory"), 500, 800, 11, 4, 1.f, 1.3f, 0.1f, 0, 25.f, 30.f);

// 14.3 to 17.0 km, THE RESORT: lifts, lodges and chalets, groomers, the rescue post; a lift line on the far ridge.
Zone(TEXT("The resort"), 14.3f, 17.f);
	Site(TEXT("Fence"), 0, 19, 2, 0, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 2, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("LiftLine"), -1, 85, 6, 1, 1.f, 1.f, 0.f);
	Site(TEXT("Village"), 1, 165, 5, 0, 1.f, 1.f, 0.f);
	Site(TEXT("RescuePost"), 1, 232, 11, 6, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_Lodge"), 152, 160, 6, 3, 0.9f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/Maps/Glacier/Props/SM_Chalet"), 190, 262, 3, 0, 0.7f, 1.f, 0.15f, 0, 40.f);
	Scatter(TEXT("SM_Snowcat"), 150, 232, 5, 1, 0.8f, 1.f, 0.f, 0, 180.f);
	Site(TEXT("PipeSide"), 0, 33, 9, 2, 0.9f, 1.f, 0.f);
	Far(TEXT("SM_RidgeLiftLine"), 400, 640, 5, 0, 1.f, 1.f, 0.1f, 0, 20.f, 8.f);
	Far(TEXT("SM_PeakLesser"), 600, 900, 9, 2, 1.f, 1.3f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_PeakObservatory"), 520, 820, 13, 7, 1.f, 1.3f, 0.1f, 0, 25.f, 30.f);

// 17.0 to 19.7 km: the glacier again, its tongues nearer.
Zone(TEXT("The ice again"), 17.f, 19.7f);
	Far(TEXT("SM_RockIceWall"), 38, 46, 2, 1, 0.7f, 1.f, 0.15f, 0, 0.69f, 6.f);
	Far(TEXT("SM_RockIceWall2"), 62, 72, 3, 1, 0.7f, 1.f, 0.2f, 0, 0.53f, 6.f);
	Scatter(TEXT("SM_SnowRockIce"), 20.6f, 22.5f, 2, 0, 0.6f, 1.f, 0.2f, 0, 8.f);
	Scatter(TEXT("SM_SnowRockBank"), 20.2f, 22, 3, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_PoleMarker2"), 18, 19.5f, 4, 2, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("IceFall"), 0, 120, 3, 1, 0.9f, 1.f, 0.1f);
	Site(TEXT("SeracField"), 0, 205, 3, 2, 1.f, 1.f, 0.f);
	Far(TEXT("SM_SnowRockCrevasse"), 170, 176, 3, 2, 0.7f, 1.f, 0.2f, 0, -10.67f, 8.f);
	Far(TEXT("SM_RockIceFalls"), 250, 258, 4, 1, 0.9f, 1.2f, 0.f, 0, -20.35f, 10.f);
	Far(TEXT("SM_RidgeGlacier"), 420, 640, 4, 2, 1.f, 1.2f, 0.15f, 0, 20.f, 14.f);
	Far(TEXT("SM_PeakLesser"), 480, 760, 8, 0, 1.f, 1.3f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_RidgeRange"), 850, 1250, 9, 4, 1.f, 1.7f, 0.2f, 0, 0.f, 8.f);

// 19.7 to 22.4 km: the white ridge again.
Zone(TEXT("The high ridge"), 19.7f, 22.4f);
	Site(TEXT("Fence"), 0, 19, 1, 0, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Far(TEXT("SM_RockIceShelf"), 138, 146, 2, 1, 0.9f, 1.f, 0.15f, 0, -6.96f, 8.f);
	Site(TEXT("WeatherPost"), 0, 172, 9, 4, 1.f, 1.f, 0.f);
	Far(TEXT("SM_PoleSignal"), 176, 184, 5, 1, 0.7f, 1.2f, 0.f, 0, -11.5f, 0.f);
	Far(TEXT("SM_SnowRockDrift"), 150, 158, 1, 0, 0.7f, 1.2f, 0.2f, 0, -8.4f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 204, 212, 2, 1, 0.7f, 1.2f, 0.2f, 0, -14.85f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 250, 256, 2, 0, 0.7f, 1.2f, 0.2f, 0, -20.23f, 8.f);
	Far(TEXT("SM_SnowRockCrevasse"), 226, 232, 2, 1, 0.7f, 1.f, 0.2f, 0, -17.36f, 8.f);
	Far(TEXT("SM_RidgeRange"), 760, 1150, 5, 1, 1.f, 1.8f, 0.2f, 0, 0.f, 8.f);
	Far(TEXT("SM_RidgeLiftLine"), 420, 650, 7, 2, 1.f, 1.f, 0.1f, 0, 20.f, 8.f);
	Far(TEXT("SM_PeakGreat"), 900, 1300, 14, 5, 1.f, 1.4f, 0.3f, 0, 25.f, 60.f);

// 22.4 km to the end of the road: the resort again.
Zone(TEXT("The last lifts"), 22.4f, 99.f);
	Site(TEXT("Fence"), 0, 19, 2, 1, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 2, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("LiftLine"), -1, 85, 6, 2, 1.f, 1.f, 0.f);
	Site(TEXT("Village"), 1, 165, 5, 0, 1.f, 1.f, 0.f);
	Site(TEXT("RescuePost"), 1, 232, 11, 6, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_Lodge"), 152, 160, 6, 4, 0.9f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/Maps/Glacier/Props/SM_Chalet"), 190, 262, 3, 1, 0.7f, 1.f, 0.15f, 0, 40.f);
	Scatter(TEXT("SM_Snowcat"), 150, 232, 5, 1, 0.8f, 1.f, 0.f, 0, 180.f);
	Site(TEXT("PipeSide"), 0, 33, 9, 2, 0.9f, 1.f, 0.f);
	Far(TEXT("SM_RidgeLiftLine"), 440, 680, 6, 3, 1.f, 1.2f, 0.1f, 0, 20.f, 8.f);
	Far(TEXT("SM_PeakGreat"), 800, 1200, 8, 1, 1.f, 1.4f, 0.3f, 0, 25.f, 60.f);
