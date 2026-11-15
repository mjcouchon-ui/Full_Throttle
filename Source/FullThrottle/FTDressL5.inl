// 3.2 dressing table, level 5: Ice Mountain (Frostbite Peak). The words (Set, Zone, Scatter, Site, Far) are explained
// at the top of FTTrackDress.cpp; the meshes are made by Content/Python/ft_dress_ice.py. Distances across are metres
// from the road's centre line; districts are in kilometres along the road and follow the level's own scenes.
//
// The lie of the land: the run goes downhill all the way, at 7 in 100, and the land beside the road with it. Snow lies
// level (across) to 80 m either side, then climbs to 26 m at 300 m, where the land ends: the middle distance is a slope
// that faces the camera. The level's own props keep their places: pines from the barrier out to 146 m on both sides, a
// lift pylon every so often on the right at 82 m, a chalet on the left, a ridge now and then at 256 to 439 m.
// The dressing: a verge of snow fences, ploughed banks, markers and snow guns (17 to 23 m, all low, nothing lit),
// the resort's works on the slopes above the pines (150 to 290 m), ice and rock that close in where the scenes say
// so, and peaks in two depths past the land's edge. The top of the mountain, the pipes, the chute, the glacier, the
// exposed ridge, the whiteout and the valley each have their own mix.
//
// Scatter and Site rules are left out of loops, corkscrews and jumps (not of the half pipes). Under a jump the land
// dips 18 m, steeply, and a piece stands level: so the solid pieces go 7 m into the ground, and the flat ones (drift,
// crevasse fields, avalanche fences) are written in zones of their own that stop at the jump.
// So what should stay through those (the ice, the drifts, the avalanche fences) is written as Far rules with a
// negative drop: the drop puts the piece on the land at that distance (the land is 0.45 m below the road's ground
// level at 80 m and rises 12 cm a metre from there; a piece stands 15 cm into it), and the band is kept narrow so
// the one drop fits it. A mesh casts shadows unless its first piece comes from a Far rule, so the first district
// places each of those by Scatter.
//
// The finale's road runs on for 16 km, so it is laid out as three districts that come round twice: the glacier, the
// white ridge, the resort.
//
// SECOND PASS (15 Nov 2026). What it added, and the rules it found:
//  - The ground plane (things a ground material cannot be): a rink, a car park, a terrain park, a ploughed service
//    road, and a frozen lake under each of the three jumps. A flat piece stands level, and this hill falls 7 in 100
//    ALONG the road: forty metres of level rink would stand 1.4 m in the air at one end. So the kit lays each of them
//    along the grade, and has a left-hand twin of each (SM_...L: on the left of the road a piece is turned round, so
//    its downhill end is the other one). Use the plain name on the right (side 1) and the L name on the left (-1).
//    A set's part that stands "along" from its anchor takes the anchor's height: give it Up = -0.07 x along on the
//    right, +0.07 x along on the left.
//  - Beside a banked bend the land is cut down to the road's low edge and banks back up over 25 m: whatever stands
//    17 to 40 m out there floats over the cutting. So the verge (fences, banks, markers, snow guns, the pipes' stands)
//    is written in zones of its own that leave the banked bends out, and flat pieces keep 42 m and more out.
//  - The race banners (SM_PoleFeather: 17 m masts, footed 12 m down, cloth that flies away from the road) are what
//    shows over a half pipe's wall and over the cutting of a banked bend: one zone per run of pipes and bends.
//  - The lakes: SM_SnowTarn is one level sheet, bigger than the lake; SiteAfloat stands it at one height above the
//    level's floor (6 m over the bottom of the jump's dip, 3 m under its downhill lip), and the land hides everything
//    outside the shore. Heights and places were read off the built track (ToolGround, ToolFloorZ); they hold as long
//    as the track is locked. Keep its decoration under 2.6 m: the road test counts the landing ramp 20 m further on.
//  - Zones written BEFORE the districts place the flat pieces and the banners by Far rules first, so those cast no
//    shadow.

Set(TEXT("FenceR"), { { TEXT("SM_SnowFence"), 0, 0, 0, 0, 1 } });
Set(TEXT("FenceL"), { { TEXT("SM_SnowFenceL"), 0, 0, 0, 0, 1 } });
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
// Second pass. R = for the right of the road, L = its twin for the left (see the head of this file: the grade).
Set(TEXT("DepotR"), {    // the piste crew's depot: the shed, the car park beside it, a groomer out front
	{ TEXT("SM_ShedGroomer"),         0,    0,     0,    0, 1 },
	{ TEXT("SM_SnowLot"),            40,   -2, -2.8f,    0, 1 },
	{ TEXT("SM_Snowcat"),            -8,  -12, 0.56f,   25, 1 },
});
Set(TEXT("DepotL"), {
	{ TEXT("SM_ShedGroomer"),         0,    0,     0,    0, 1 },
	{ TEXT("SM_SnowLotL"),           40,   -2,  2.8f,    0, 1 },
	{ TEXT("SM_Snowcat"),            -8,  -12, -0.56f,  25, 1 },
});
Set(TEXT("IceFieldR"), {    // the rink and the terrain park end to end
	{ TEXT("SM_SnowRink"),            0,    0,      0,   0, 1 },
	{ TEXT("SM_SnowPark"),           46,   -4, -3.22f,   0, 1 },
});
Set(TEXT("IceFieldL"), {
	{ TEXT("SM_SnowRinkL"),           0,    0,      0,   0, 1 },
	{ TEXT("SM_SnowParkL"),          46,   -4,  3.22f,   0, 1 },
});
Set(TEXT("ServiceRoad"), { { TEXT("SM_SnowRoad"), 0, 0, 0, 0, 1 } });    // right-hand side only: one length a slot
// The lakes under the jumps: metres along from the slot to the middle of the dip, metres above the level's floor.
Set(TEXT("Tarn1"), { { TEXT("SM_SnowTarn"),  -4.9f, 0, 1570.75f, 0, 1 } });
Set(TEXT("Tarn2"), { { TEXT("SM_SnowTarn"),  -4.5f, 0, 1383.35f, 0, 1 } });
Set(TEXT("Tarn3"), { { TEXT("SM_SnowTarn"), -16.5f, 0, 1166.14f, 0, 1 } });

// ---- SECOND PASS, before the districts: the lakes, the land under the set pieces, the jumps' aprons, the banners.
// A frozen lake under each jump: one slot each (51, 111, 181), the lake's sheet at its own height above the floor.
Zone(TEXT("Under the first jump: the lake"), 2.3f, 2.32f);
	SiteAfloat(TEXT("Tarn1"), 1, 0, 1, 0, 1.f);

Zone(TEXT("Under the second jump: the lake"), 5.01f, 5.03f);
	SiteAfloat(TEXT("Tarn2"), 1, 0, 1, 0, 1.f);

Zone(TEXT("Under the third jump: the lake"), 8.18f, 8.2f);
	SiteAfloat(TEXT("Tarn3"), 1, 0, 1, 0, 1.f);

// Under the corkscrews and the loops the camera looks at the land, and Scatter and Site are left out there: so the
// resort's ground (rink, terrain park, car park) is written as Far rules with the land's own drop, one piece a side
// every second slot (a Far piece is moved up to 11 m along its slot: closer than that they would overlap), and the
// banners line the set piece itself. The land runs straight and level-across under these.
Zone(TEXT("Under the chute's corkscrew"), 2.51f, 2.685f);
	Far(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.78f, 0.f);
	Far(TEXT("SM_SnowParkL"), 54, 58, 4, 0, 1.f, 1.f, 0.f, -1, 0.6f, 3.f);
	Far(TEXT("SM_SnowRinkL"), 58, 62, 4, 2, 1.f, 1.f, 0.f, -1, 0.57f, 3.f);
	Far(TEXT("SM_SnowLot"), 62, 66, 4, 3, 1.f, 1.f, 0.f, 1, 0.55f, 3.f);
	Far(TEXT("SM_SnowPark"), 54, 58, 4, 1, 1.f, 1.f, 0.f, 1, 0.6f, 3.f);

Zone(TEXT("Under Black diamond's loop"), 4.085f, 4.37f);
	Far(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.78f, 0.f);
	Far(TEXT("SM_SnowLotL"), 62, 66, 4, 0, 1.f, 1.f, 0.f, -1, 0.55f, 3.f);
	Far(TEXT("SM_SnowParkL"), 54, 58, 4, 2, 1.f, 1.f, 0.f, -1, 0.6f, 3.f);
	Far(TEXT("SM_SnowRink"), 58, 62, 4, 3, 1.f, 1.f, 0.f, 1, 0.57f, 3.f);
	Far(TEXT("SM_SnowPark"), 54, 58, 4, 1, 1.f, 1.f, 0.f, 1, 0.6f, 3.f);

Zone(TEXT("Under Black diamond's corkscrew"), 5.225f, 5.4f);
	Far(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.78f, 0.f);
	Far(TEXT("SM_SnowParkL"), 54, 58, 4, 0, 1.f, 1.f, 0.f, -1, 0.6f, 3.f);
	Far(TEXT("SM_SnowRinkL"), 58, 62, 4, 2, 1.f, 1.f, 0.f, -1, 0.57f, 3.f);
	Far(TEXT("SM_SnowLot"), 62, 66, 4, 3, 1.f, 1.f, 0.f, 1, 0.55f, 3.f);
	Far(TEXT("SM_SnowPark"), 54, 58, 4, 1, 1.f, 1.f, 0.f, 1, 0.6f, 3.f);

Zone(TEXT("Under the whiteout's loop"), 6.87f, 7.155f);
	Far(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.78f, 0.f);
	Far(TEXT("SM_SnowLotL"), 62, 66, 4, 0, 1.f, 1.f, 0.f, -1, 0.55f, 3.f);
	Far(TEXT("SM_SnowParkL"), 54, 58, 4, 2, 1.f, 1.f, 0.f, -1, 0.6f, 3.f);
	Far(TEXT("SM_SnowRink"), 58, 62, 4, 3, 1.f, 1.f, 0.f, 1, 0.57f, 3.f);
	Far(TEXT("SM_SnowPark"), 54, 58, 4, 1, 1.f, 1.f, 0.f, 1, 0.6f, 3.f);

Zone(TEXT("Under the valley's corkscrew"), 7.215f, 7.385f);
	Far(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.78f, 0.f);
	Far(TEXT("SM_SnowParkL"), 54, 58, 4, 0, 1.f, 1.f, 0.f, -1, 0.6f, 3.f);
	Far(TEXT("SM_SnowRinkL"), 58, 62, 4, 2, 1.f, 1.f, 0.f, -1, 0.57f, 3.f);
	Far(TEXT("SM_SnowLot"), 62, 66, 4, 3, 1.f, 1.f, 0.f, 1, 0.55f, 3.f);
	Far(TEXT("SM_SnowPark"), 54, 58, 4, 1, 1.f, 1.f, 0.f, 1, 0.6f, 3.f);

// The aprons of the three jumps, seen from the air: a rink and a terrain park on one side, the piste crew's depot on
// the other, each as one set on one chosen slot (a set is not moved along its slot), clear of the dip under the jump.
Zone(TEXT("The first jump, its run-up"), 2.1f, 2.19f);
	Site(TEXT("IceFieldL"), -1, 60, 2, 1, 1.f);
	Site(TEXT("DepotR"), 1, 68, 2, 1, 1.f);

Zone(TEXT("The first jump, its landing"), 2.38f, 2.5f);
	Site(TEXT("IceFieldL"), -1, 60, 3, 0, 1.f);
	Site(TEXT("DepotR"), 1, 68, 3, 1, 1.f);

Zone(TEXT("The second jump, its run-up"), 4.83f, 4.9f);
	Site(TEXT("IceFieldR"), 1, 60, 2, 1, 1.f);
	Site(TEXT("DepotL"), -1, 68, 2, 0, 1.f);

Zone(TEXT("The second jump, its landing"), 5.1f, 5.21f);
	Site(TEXT("IceFieldR"), 1, 60, 3, 1, 1.f);
	Site(TEXT("DepotL"), -1, 68, 3, 0, 1.f);

Zone(TEXT("The third jump, its run-up"), 7.99f, 8.06f);
	Site(TEXT("IceFieldL"), -1, 60, 2, 0, 1.f);
	Site(TEXT("DepotR"), 1, 68, 2, 1, 1.f);

Zone(TEXT("The third jump, its landing"), 8.27f, 8.375f);
	Site(TEXT("IceFieldR"), 1, 60, 3, 0, 1.f);

// The race banners: one zone for each run of half pipes and banked bends (the track's own flags, 15 Nov), a mast
// group a slot on both sides. (The avalanche chute has its own, opposite each gallery.)
Zone(TEXT("Banners: the top's bends and the first pipe"), 0.26f, 2.095f);
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("Banners: the super pipe and its bends"), 3.045f, 4.03f);
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("Banners: Black diamond's walls"), 4.47f, 4.825f);
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("Banners: the whiteout's pipe and bends"), 5.44f, 6.815f);
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("Banners: the valley's bends"), 7.525f, 7.975f);
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("Banners: the valley's pipe"), 8.38f, 8.785f);
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("Banners: finale, 1"), 14.625f, 15.435f);
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("Banners: finale, 2"), 15.76f, 16.57f);
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("Banners: finale, 3"), 16.9f, 17.71f);
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("Banners: finale, 4"), 18.035f, 18.85f);
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("Banners: finale, 5"), 19.175f, 19.985f);
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("Banners: finale, 6"), 20.31f, 21.125f);
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("Banners: finale, 7"), 21.45f, 22.26f);
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("Banners: finale, 8"), 22.585f, 23.4f);
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("Banners: finale, 9"), 23.725f, 24.54f);
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 1, 0, 1.f, 1.f, 0.f, 0, 0.f);

// ---- 0 to 1.19 km: FIRST TRACKS, the top of the mountain. A lift arrives from below on the left, the rescue post is
// on the right, the piste is marked and fenced, cornices hang under the edge, and the summit's observatory and the
// great peaks stand close. Second pass: the summit hotel on the left slope, the crew's depot on the right, a near
// ridge in front of the peaks, and two more shapes of peak.
Zone(TEXT("First tracks"), 0.f, 1.19f);
	Site(TEXT("LiftLine"), -1, 85, 8, 3, 1.f, 1.f, 0.f);
	Site(TEXT("RescuePost"), 1, 165, 9, 4, 1.f, 1.f, 0.f);
	Site(TEXT("DepotR"), 1, 68, 9, 1, 1.f);
	Scatter(TEXT("SM_Hotel"), 204, 210, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("SM_SnowRockDrift"), 150, 262, 2, 0, 0.6f, 1.f, 0.3f, 0, 8.f);
	Scatter(TEXT("SM_SnowRockCrevasse"), 160, 250, 5, 2, 0.7f, 1.f, 0.2f, 0, 8.f);
	Scatter(TEXT("SM_RockSeracB"), 184, 260, 6, 3, 0.8f, 1.f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_RockSeracH"), 190, 260, 7, 5, 0.8f, 1.f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_RockIceShelf"), 230, 272, 4, 1, 0.8f, 1.f, 0.2f, 0, 8.f);
	Scatter(TEXT("SM_RidgeFences"), 236, 262, 9, 6, 1.f, 1.f, 0.f, 0, 6.f);
	Scatter(TEXT("SM_PoleSignal"), 200, 282, 7, 5, 1.f, 1.2f, 0.f, 0, 0.f);
	Far(TEXT("SM_RidgeSaw"), 345, 400, 10, 4, 0.7f, 1.f, 0.15f, 0, 8.f, 10.f);
	Far(TEXT("SM_PeakObservatory"), 430, 700, 9, 3, 1.f, 1.3f, 0.1f, 1, 25.f, 30.f);
	Far(TEXT("SM_PeakGreat"), 800, 1200, 12, 0, 1.f, 1.3f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_PeakHorn"), 760, 1100, 12, 6, 1.f, 1.1f, 0.2f, 0, 25.f, 40.f);
	Far(TEXT("SM_PeakTwin"), 700, 1000, 14, 9, 1.f, 1.2f, 0.2f, 0, 25.f, 40.f);
	Far(TEXT("SM_RidgeRange"), 820, 1200, 9, 2, 1.f, 1.7f, 0.2f, 0, 0.f, 8.f);

Zone(TEXT("First tracks, the verge"), 0.f, 0.25f);
	Site(TEXT("FenceL"), -1, 19, 4, 0, 0.9f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 4, 2, 0.9f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank"), 20.2f, 22, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 2, 0, 0.8f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_PoleMarker2"), 18, 19.5f, 3, 1, 0.7f, 1.f, 0.f, 0, 0.f);

// ---- 1.19 to 2.43 km: THE HALF PIPE (then a jump). A competition pipe: judges' towers and benches at its ends, snow
// guns down the verge, groomers parked on the slopes, avalanche fences above; the great peaks show over its walls.
Zone(TEXT("The half pipe"), 1.19f, 2.43f);
	Scatter(TEXT("SM_Snowcat"), 150, 225, 5, 2, 0.9f, 1.f, 0.f, 0, 180.f);
	Site(TEXT("RescuePost"), -1, 165, 11, 5, 1.f, 1.f, 0.f);
	Far(TEXT("SM_RidgeSaw"), 345, 400, 9, 1, 0.7f, 1.f, 0.15f, 0, 8.f, 10.f);
	Far(TEXT("SM_PeakLesser"), 520, 820, 9, 2, 1.f, 1.2f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_PeakGreat"), 800, 1200, 11, 4, 1.f, 1.4f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_PeakTwin"), 620, 900, 12, 7, 1.f, 1.1f, 0.2f, 0, 25.f, 40.f);
	Far(TEXT("SM_RidgeRange"), 820, 1200, 8, 0, 1.f, 1.7f, 0.2f, 0, 0.f, 8.f);

Zone(TEXT("The half pipe, the verge (1)"), 1.21f, 1.77f);
	Site(TEXT("PipeSide"), 0, 33, 2, 0, 0.9f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 2, 1, 0.7f, 1.f, 0.f, 0, 15.f);
	Site(TEXT("FenceL"), -1, 19, 6, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 6, 3, 0.8f, 1.f, 0.f);

Zone(TEXT("The half pipe, the verge (2)"), 2.1f, 2.43f);
	Site(TEXT("PipeSide"), 0, 33, 2, 0, 0.9f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 2, 1, 0.7f, 1.f, 0.f, 0, 15.f);
	Site(TEXT("FenceL"), -1, 19, 6, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 6, 3, 0.8f, 1.f, 0.f);

// The same district, for what lies flat on the slopes (the land dips 18 m under a jump, and a piece stands level):
// avalanche fences and drift, through the pipe and as far as the jump's lip.
Zone(TEXT("The half pipe, above the jump"), 1.19f, 2.19f);
	Far(TEXT("SM_RidgeFences"), 160, 168, 3, 1, 0.7f, 1.f, 0.f, 0, -9.59f, 6.f);
	Far(TEXT("SM_RidgeFences"), 236, 244, 4, 2, 0.7f, 1.f, 0.f, 0, -18.68f, 6.f);
	Far(TEXT("SM_SnowRockDrift"), 150, 158, 3, 0, 0.6f, 1.f, 0.3f, 0, -8.4f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 250, 258, 3, 1, 0.6f, 1.f, 0.3f, 0, -20.35f, 8.f);

// ---- 2.43 to 2.96 km: AVALANCHE CHUTE (a corkscrew, then hard banked bends). Engineered against the mountain: tiers
// of avalanche fence up both slopes (they stay through the corkscrew), galleries and rock cuts hard by the road, steep
// peaks close overhead. Second pass: the fences are panels now (they inked to black scribble as slats); the second
// tier is every other slot with a deflecting dam between, the top tier every other slot; leaning horns overhead.
Zone(TEXT("Avalanche chute"), 2.43f, 2.96f);
	Far(TEXT("SM_RidgeFences"), 148, 156, 1, 0, 0.9f, 1.f, 0.f, 0, -8.16f, 5.f);
	Far(TEXT("SM_RidgeFences"), 174, 182, 2, 1, 0.9f, 1.f, 0.f, 0, -11.27f, 5.f);
	Far(TEXT("SM_AvalancheDam"), 174, 182, 2, 0, 0.9f, 1.f, 0.f, 0, -11.27f, 12.f);
	Far(TEXT("SM_RidgeFences"), 200, 208, 2, 1, 0.8f, 1.f, 0.f, 0, -14.37f, 5.f);
	Far(TEXT("SM_RidgeFences"), 236, 244, 2, 0, 0.8f, 1.2f, 0.f, 0, -18.68f, 5.f);
	Site(TEXT("Cutting"), -1, 30, 2, 0, 1.f, 1.f, 0.f);
	Site(TEXT("Cutting"), 1, 30, 2, 1, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 2, 0, 1.f, 1.f, 0.f, 1, 0.f);     // banners opposite each gallery: right on even slots,
	Scatter(TEXT("SM_PoleFeather"), 26, 28, 2, 1, 1.f, 1.f, 0.f, -1, 0.f);    // left on odd ones
	Far(TEXT("SM_PeakGreat"), 420, 600, 5, 0, 1.f, 1.f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_PeakHorn"), 460, 640, 5, 2, 1.f, 0.7f, 0.2f, 0, 25.f, 40.f);

// ---- 2.96 to 4.02 km: THE SUPER PIPE, cut through the glacier. Blue ice walls right outside the pipe's lip and along
// the road after it, ice falls, seracs and frozen falls up the slopes, and a glacier tongue coming down between the
// peaks.
Zone(TEXT("The super pipe"), 2.96f, 4.02f);
	Far(TEXT("SM_RockIceWall"), 38, 46, 1, 0, 0.7f, 1.f, 0.15f, 0, 0.69f, 6.f);
	Site(TEXT("IceFall"), 0, 118, 1, 0, 0.85f, 1.f, 0.1f);
	Far(TEXT("SM_RockIceWall2"), 100, 110, 2, 0, 0.8f, 1.2f, 0.2f, 0, -2.54f, 6.f);
	Far(TEXT("SM_RockSeracB"), 150, 160, 1, 0, 0.6f, 1.3f, 0.3f, 0, -8.52f, 180.f);
	Far(TEXT("SM_RockIceFalls"), 196, 206, 4, 2, 0.9f, 1.2f, 0.f, 0, -14.02f, 10.f);
	Far(TEXT("SM_RockIceWall2"), 240, 250, 2, 1, 0.8f, 1.4f, 0.2f, 0, -19.27f, 6.f);
	Far(TEXT("SM_RidgeGlacier"), 380, 600, 5, 1, 1.f, 1.f, 0.15f, 0, 20.f, 14.f);
	Far(TEXT("SM_PeakGreat"), 800, 1200, 9, 3, 1.f, 1.4f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_PeakTwin"), 700, 1000, 11, 6, 1.f, 1.2f, 0.2f, 0, 25.f, 40.f);

Zone(TEXT("The super pipe, the verge"), 2.96f, 3.59f);
	Scatter(TEXT("SM_SnowRockIce"), 20.6f, 22.5f, 1, 0, 0.6f, 1.f, 0.2f, 0, 8.f);

// ---- 4.02 to 5.65 km: BLACK DIAMOND, the exposed ridge (a loop, a jump, a corkscrew). Wind country: fences down the
// whole verge, cornices and carved drift on the slopes, crevasse fields, a weather post and signal masts, and the far
// ranges open on both sides. The slopes' dressing stays through the loop and the jump. Second pass: the far range
// stood too near and too big here (a lavender flank filled a corner of the view from the jump): further and smaller.
Zone(TEXT("Black diamond"), 4.02f, 5.65f);
	Far(TEXT("SM_RockIceShelf"), 138, 146, 2, 0, 0.9f, 1.f, 0.15f, 0, -6.96f, 8.f);
	Site(TEXT("WeatherPost"), 1, 172, 9, 3, 1.f, 1.f, 0.f);
	Far(TEXT("SM_PoleSignal"), 176, 184, 4, 1, 0.7f, 1.2f, 0.f, 0, -11.5f, 0.f);
	Far(TEXT("SM_RidgeSaw"), 345, 400, 8, 2, 0.7f, 1.f, 0.15f, 0, 8.f, 10.f);
	Far(TEXT("SM_RidgeRange"), 850, 1200, 6, 0, 1.f, 1.4f, 0.2f, 0, 0.f, 8.f);
	Far(TEXT("SM_PeakHorn"), 800, 1150, 9, 4, 1.f, 1.2f, 0.2f, 0, 25.f, 40.f);
	Far(TEXT("SM_PeakObservatory"), 500, 800, 11, 4, 1.f, 1.3f, 0.1f, 0, 25.f, 30.f);
	Far(TEXT("SM_RidgeLiftLine"), 420, 650, 9, 6, 1.f, 1.f, 0.1f, 0, 20.f, 8.f);

Zone(TEXT("Black diamond, the verge (1)"), 4.03f, 4.46f);
	Site(TEXT("FenceL"), -1, 19, 2, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 2, 1, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 0, 0.6f, 1.f, 0.2f, 0, 2.f);

Zone(TEXT("Black diamond, the verge (2)"), 4.83f, 5.43f);
	Site(TEXT("FenceL"), -1, 19, 2, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 2, 1, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 0, 0.6f, 1.f, 0.2f, 0, 2.f);

// The same district, for what lies flat on the slopes: carved drift and crevasse fields, through the loop and the
// corkscrew but not in the dip under the jump at 5 km.
Zone(TEXT("Black diamond, above the jump"), 4.02f, 4.91f);
	Far(TEXT("SM_SnowRockDrift"), 150, 158, 1, 0, 0.7f, 1.2f, 0.2f, 0, -8.4f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 204, 212, 2, 1, 0.7f, 1.2f, 0.2f, 0, -14.85f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 250, 256, 2, 0, 0.7f, 1.2f, 0.2f, 0, -20.23f, 8.f);
	Far(TEXT("SM_SnowRockCrevasse"), 170, 176, 2, 1, 0.7f, 1.f, 0.2f, 0, -10.67f, 8.f);
	Far(TEXT("SM_SnowRockCrevasse"), 226, 232, 3, 0, 0.7f, 1.f, 0.2f, 0, -17.36f, 8.f);

Zone(TEXT("Black diamond, below the jump"), 5.12f, 5.65f);
	Far(TEXT("SM_SnowRockDrift"), 150, 158, 1, 0, 0.7f, 1.2f, 0.2f, 0, -8.4f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 204, 212, 2, 1, 0.7f, 1.2f, 0.2f, 0, -14.85f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 250, 256, 2, 0, 0.7f, 1.2f, 0.2f, 0, -20.23f, 8.f);
	Far(TEXT("SM_SnowRockCrevasse"), 170, 176, 2, 1, 0.7f, 1.f, 0.2f, 0, -10.67f, 8.f);
	Far(TEXT("SM_SnowRockCrevasse"), 226, 232, 3, 0, 0.7f, 1.f, 0.2f, 0, -17.36f, 8.f);

// ---- 5.65 to 7.3 km: THE WHITEOUT (a pipe, a loop, a corkscrew). Every gate on the mountain: markers thick along the
// verge, fences, a pipe with its stands, avalanche works and seracs on the slopes.
Zone(TEXT("The whiteout"), 5.65f, 7.3f);
	Far(TEXT("SM_RidgeFences"), 152, 160, 3, 0, 0.9f, 1.f, 0.f, 0, -8.64f, 5.f);
	Far(TEXT("SM_RidgeFences"), 178, 186, 3, 0, 0.8f, 1.f, 0.f, 0, -11.74f, 5.f);
	Far(TEXT("SM_AvalancheDam"), 204, 212, 4, 2, 0.9f, 1.f, 0.f, 0, -14.85f, 12.f);
	Site(TEXT("SeracField"), 0, 200, 4, 2, 1.f, 1.f, 0.f);
	Far(TEXT("SM_RockSeracB"), 226, 234, 2, 1, 0.7f, 1.2f, 0.3f, 0, -17.48f, 180.f);
	Far(TEXT("SM_RockSeracH"), 252, 258, 3, 0, 0.7f, 1.2f, 0.2f, 0, -20.47f, 180.f);
	Far(TEXT("SM_SnowRockDrift"), 150, 158, 2, 1, 0.6f, 1.2f, 0.2f, 0, -8.4f, 8.f);
	Far(TEXT("SM_RidgeSaw"), 345, 400, 10, 6, 0.7f, 1.f, 0.15f, 0, 8.f, 10.f);
	Far(TEXT("SM_PeakLesser"), 520, 820, 8, 0, 1.f, 1.2f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_PeakTwin"), 640, 900, 11, 5, 1.f, 1.f, 0.2f, 0, 25.f, 40.f);
	Far(TEXT("SM_RidgeRange"), 850, 1200, 8, 2, 1.f, 1.7f, 0.2f, 0, 0.f, 8.f);

Zone(TEXT("The whiteout, the verge (1)"), 5.66f, 6.33f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 1, 0, 0.8f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_PoleMarker2"), 18, 19.5f, 1, 0, 0.5f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("FenceL"), -1, 20.5f, 2, 0, 0.7f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 20.5f, 2, 1, 0.7f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Site(TEXT("PipeSide"), 0, 33, 3, 1, 0.9f, 1.f, 0.f);

Zone(TEXT("The whiteout, the verge (2)"), 6.82f, 7.3f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 1, 0, 0.8f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_PoleMarker2"), 18, 19.5f, 1, 0, 0.5f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("FenceL"), -1, 20.5f, 2, 0, 0.7f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 20.5f, 2, 1, 0.7f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Site(TEXT("PipeSide"), 0, 33, 3, 1, 0.9f, 1.f, 0.f);

// ---- 7.3 to 8.94 km: THE VALLEY RUN, the descent (banked bends, a jump, the last pipe). Down among the resort: lodges
// and chalets on the slopes, lifts going up out of base stations on the left, groomers, a gallery now and then, a lift
// line on the far ridge. Second pass: the grand hotel above every other village, the crew's depot by the road.
Zone(TEXT("The valley run"), 7.3f, 8.94f);
	Site(TEXT("LiftLine"), -1, 85, 5, 1, 1.f, 1.f, 0.f);
	Site(TEXT("Village"), 1, 165, 4, 0, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_Hotel"), 204, 210, 8, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("DepotR"), 1, 68, 6, 0, 1.f);
	Scatter(TEXT("SM_Lodge"), 152, 160, 5, 2, 0.9f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/Maps/Glacier/Props/SM_Chalet"), 190, 262, 2, 0, 0.7f, 1.f, 0.15f, -1, 40.f);
	Scatter(TEXT("/Game/Maps/Glacier/Props/SM_Chalet"), 228, 262, 2, 0, 0.7f, 1.f, 0.15f, 1, 40.f);
	Scatter(TEXT("SM_Snowcat"), 150, 232, 4, 1, 0.8f, 1.f, 0.f, 0, 180.f);
	Far(TEXT("SM_RidgeLiftLine"), 400, 640, 5, 0, 1.f, 1.f, 0.1f, 0, 20.f, 8.f);
	Far(TEXT("SM_PeakGreat"), 800, 1200, 9, 2, 1.f, 1.4f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_PeakHorn"), 760, 1100, 12, 7, 1.f, 1.1f, 0.2f, 0, 25.f, 40.f);
	Far(TEXT("SM_PeakObservatory"), 520, 820, 13, 7, 1.f, 1.3f, 0.1f, 0, 25.f, 30.f);

Zone(TEXT("The valley run, its galleries (7.3 km)"), 7.3f, 7.52f);
	Site(TEXT("Cutting"), 1, 30, 5, 3, 1.f, 1.f, 0.f);

Zone(TEXT("The valley run, its galleries (7.98 km)"), 7.98f, 8.37f);
	Site(TEXT("Cutting"), 1, 30, 5, 3, 1.f, 1.f, 0.f);

Zone(TEXT("The valley run, the verge (1)"), 7.3f, 7.52f);
	Site(TEXT("FenceL"), -1, 19, 4, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 4, 2, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank"), 20.2f, 22, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);

Zone(TEXT("The valley run, the verge (2)"), 7.98f, 8.94f);
	Site(TEXT("FenceL"), -1, 19, 4, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 4, 2, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank"), 20.2f, 22, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);

// ---- 8.94 km to the end: THE FINALE, the Abominable's road, in three districts that come round twice.
// 8.94 to 11.6 km, THE GLACIER: ice walls by the road, ice falls and serac fields on the slopes, crevasses, glacier
// tongues between the peaks. (No banked bends here: the verge stays in the district.)
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
	Far(TEXT("SM_PeakGreat"), 800, 1200, 10, 2, 1.f, 1.4f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_PeakHorn"), 800, 1150, 11, 5, 1.f, 1.2f, 0.2f, 0, 25.f, 40.f);
	Far(TEXT("SM_PeakLesser"), 520, 820, 11, 3, 1.f, 1.2f, 0.3f, 0, 25.f, 60.f);

// 11.6 to 14.3 km, THE WHITE RIDGE: fences, cornices, carved drift and the weather post; the far ranges open.
Zone(TEXT("The white ridge"), 11.6f, 14.3f);
	Site(TEXT("FenceL"), -1, 19, 2, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 2, 1, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 0, 0.6f, 1.f, 0.2f, 0, 2.f);
	Far(TEXT("SM_RockIceShelf"), 138, 146, 2, 0, 0.9f, 1.f, 0.15f, 0, -6.96f, 8.f);
	Site(TEXT("WeatherPost"), 0, 172, 9, 3, 1.f, 1.f, 0.f);
	Far(TEXT("SM_PoleSignal"), 176, 184, 5, 1, 0.7f, 1.2f, 0.f, 0, -11.5f, 0.f);
	Far(TEXT("SM_SnowRockDrift"), 150, 158, 1, 0, 0.7f, 1.2f, 0.2f, 0, -8.4f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 204, 212, 2, 1, 0.7f, 1.2f, 0.2f, 0, -14.85f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 250, 256, 2, 0, 0.7f, 1.2f, 0.2f, 0, -20.23f, 8.f);
	Far(TEXT("SM_SnowRockCrevasse"), 226, 232, 2, 0, 0.7f, 1.f, 0.2f, 0, -17.36f, 8.f);
	Far(TEXT("SM_RidgeSaw"), 345, 400, 9, 5, 0.7f, 1.f, 0.15f, 0, 8.f, 10.f);
	Far(TEXT("SM_RidgeRange"), 850, 1200, 6, 0, 1.f, 1.4f, 0.2f, 0, 0.f, 8.f);
	Far(TEXT("SM_PeakTwin"), 700, 1000, 11, 3, 1.f, 1.2f, 0.2f, 0, 25.f, 40.f);
	Far(TEXT("SM_PeakObservatory"), 500, 800, 11, 4, 1.f, 1.3f, 0.1f, 0, 25.f, 30.f);

// 14.3 to 17.0 km, THE RESORT: lifts, lodges and chalets, groomers, the rescue post; a lift line on the far ridge.
// Second pass: a ploughed service road down the right (50 m), the crew's depot on it, the rink and terrain park on the left, the hotel above every other village.
Zone(TEXT("The resort"), 14.3f, 17.f);
	Site(TEXT("ServiceRoad"), 1, 50, 1, 0, 1.f);
	Site(TEXT("DepotR"), 1, 70, 7, 2, 1.f);
	Site(TEXT("IceFieldL"), -1, 60, 6, 4, 1.f);
	Site(TEXT("LiftLine"), -1, 85, 6, 1, 1.f, 1.f, 0.f);
	Site(TEXT("Village"), 1, 165, 5, 0, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_Hotel"), 204, 210, 10, 5, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("RescuePost"), 1, 232, 11, 6, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_Lodge"), 152, 160, 6, 3, 0.9f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/Maps/Glacier/Props/SM_Chalet"), 190, 262, 3, 0, 0.7f, 1.f, 0.15f, -1, 40.f);
	Scatter(TEXT("/Game/Maps/Glacier/Props/SM_Chalet"), 228, 262, 3, 0, 0.7f, 1.f, 0.15f, 1, 40.f);
	Scatter(TEXT("SM_Snowcat"), 150, 232, 5, 1, 0.8f, 1.f, 0.f, 0, 180.f);
	Far(TEXT("SM_RidgeSaw"), 345, 400, 9, 7, 0.7f, 1.f, 0.15f, 0, 8.f, 10.f);
	Far(TEXT("SM_RidgeLiftLine"), 400, 640, 5, 0, 1.f, 1.f, 0.1f, 0, 20.f, 8.f);
	Far(TEXT("SM_PeakLesser"), 600, 900, 9, 2, 1.f, 1.3f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_PeakHorn"), 760, 1100, 13, 4, 1.f, 1.1f, 0.2f, 0, 25.f, 40.f);
	Far(TEXT("SM_PeakObservatory"), 520, 820, 13, 7, 1.f, 1.3f, 0.1f, 0, 25.f, 30.f);

Zone(TEXT("The resort, the verge (1)"), 14.3f, 14.62f);
	Site(TEXT("FenceL"), -1, 19, 4, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 4, 2, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 2, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("PipeSide"), 0, 33, 9, 2, 0.9f, 1.f, 0.f);

Zone(TEXT("The resort, the verge (2)"), 14.97f, 15.75f);
	Site(TEXT("FenceL"), -1, 19, 4, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 4, 2, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 2, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("PipeSide"), 0, 33, 9, 2, 0.9f, 1.f, 0.f);

Zone(TEXT("The resort, the verge (3)"), 16.11f, 16.89f);
	Site(TEXT("FenceL"), -1, 19, 4, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 4, 2, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 2, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("PipeSide"), 0, 33, 9, 2, 0.9f, 1.f, 0.f);

// 17.0 to 19.7 km: the glacier again, its tongues nearer.
Zone(TEXT("The ice again"), 17.f, 19.7f);
	Far(TEXT("SM_RockIceWall"), 38, 46, 2, 1, 0.7f, 1.f, 0.15f, 0, 0.69f, 6.f);
	Far(TEXT("SM_RockIceWall2"), 62, 72, 3, 1, 0.7f, 1.f, 0.2f, 0, 0.53f, 6.f);
	Site(TEXT("IceFall"), 0, 120, 3, 1, 0.9f, 1.f, 0.1f);
	Site(TEXT("SeracField"), 0, 205, 3, 2, 1.f, 1.f, 0.f);
	Far(TEXT("SM_SnowRockCrevasse"), 170, 176, 3, 2, 0.7f, 1.f, 0.2f, 0, -10.67f, 8.f);
	Far(TEXT("SM_RockIceFalls"), 250, 258, 4, 1, 0.9f, 1.2f, 0.f, 0, -20.35f, 10.f);
	Far(TEXT("SM_RidgeGlacier"), 420, 640, 4, 2, 1.f, 1.2f, 0.15f, 0, 20.f, 14.f);
	Far(TEXT("SM_PeakLesser"), 480, 760, 8, 0, 1.f, 1.3f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_PeakTwin"), 700, 1000, 12, 6, 1.f, 1.2f, 0.2f, 0, 25.f, 40.f);
	Far(TEXT("SM_RidgeRange"), 850, 1250, 9, 4, 1.f, 1.7f, 0.2f, 0, 0.f, 8.f);

Zone(TEXT("The ice again, the verge (1)"), 17.24f, 18.03f);
	Scatter(TEXT("SM_SnowRockIce"), 20.6f, 22.5f, 2, 0, 0.6f, 1.f, 0.2f, 0, 8.f);
	Scatter(TEXT("SM_SnowRockBank"), 20.2f, 22, 3, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_PoleMarker2"), 18, 19.5f, 4, 2, 0.8f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("The ice again, the verge (2)"), 18.38f, 19.17f);
	Scatter(TEXT("SM_SnowRockIce"), 20.6f, 22.5f, 2, 0, 0.6f, 1.f, 0.2f, 0, 8.f);
	Scatter(TEXT("SM_SnowRockBank"), 20.2f, 22, 3, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_PoleMarker2"), 18, 19.5f, 4, 2, 0.8f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("The ice again, the verge (3)"), 19.52f, 19.7f);
	Scatter(TEXT("SM_SnowRockIce"), 20.6f, 22.5f, 2, 0, 0.6f, 1.f, 0.2f, 0, 8.f);
	Scatter(TEXT("SM_SnowRockBank"), 20.2f, 22, 3, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_PoleMarker2"), 18, 19.5f, 4, 2, 0.8f, 1.f, 0.f, 0, 0.f);

// 19.7 to 22.4 km: the white ridge again.
Zone(TEXT("The high ridge"), 19.7f, 22.4f);
	Far(TEXT("SM_RockIceShelf"), 138, 146, 2, 1, 0.9f, 1.f, 0.15f, 0, -6.96f, 8.f);
	Site(TEXT("WeatherPost"), 0, 172, 9, 4, 1.f, 1.f, 0.f);
	Far(TEXT("SM_PoleSignal"), 176, 184, 5, 1, 0.7f, 1.2f, 0.f, 0, -11.5f, 0.f);
	Far(TEXT("SM_SnowRockDrift"), 150, 158, 1, 0, 0.7f, 1.2f, 0.2f, 0, -8.4f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 204, 212, 2, 1, 0.7f, 1.2f, 0.2f, 0, -14.85f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 250, 256, 2, 0, 0.7f, 1.2f, 0.2f, 0, -20.23f, 8.f);
	Far(TEXT("SM_SnowRockCrevasse"), 226, 232, 2, 1, 0.7f, 1.f, 0.2f, 0, -17.36f, 8.f);
	Far(TEXT("SM_RidgeSaw"), 345, 400, 9, 2, 0.7f, 1.f, 0.15f, 0, 8.f, 10.f);
	Far(TEXT("SM_RidgeRange"), 880, 1250, 6, 1, 1.f, 1.4f, 0.2f, 0, 0.f, 8.f);
	Far(TEXT("SM_PeakHorn"), 800, 1150, 10, 3, 1.f, 1.2f, 0.2f, 0, 25.f, 40.f);
	Far(TEXT("SM_RidgeLiftLine"), 420, 650, 7, 2, 1.f, 1.f, 0.1f, 0, 20.f, 8.f);
	Far(TEXT("SM_PeakGreat"), 900, 1300, 14, 5, 1.f, 1.4f, 0.3f, 0, 25.f, 60.f);

Zone(TEXT("The high ridge, the verge (1)"), 19.7f, 20.3f);
	Site(TEXT("FenceL"), -1, 19, 2, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 2, 1, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);

Zone(TEXT("The high ridge, the verge (2)"), 20.66f, 21.44f);
	Site(TEXT("FenceL"), -1, 19, 2, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 2, 1, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);

Zone(TEXT("The high ridge, the verge (3)"), 21.79f, 22.4f);
	Site(TEXT("FenceL"), -1, 19, 2, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 2, 1, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);

// 22.4 km to the end of the road: the resort again.
Zone(TEXT("The last lifts"), 22.4f, 99.f);
	Site(TEXT("ServiceRoad"), 1, 50, 1, 0, 1.f);
	Site(TEXT("DepotR"), 1, 70, 7, 4, 1.f);
	Site(TEXT("IceFieldL"), -1, 60, 6, 4, 1.f);
	Site(TEXT("LiftLine"), -1, 85, 6, 2, 1.f, 1.f, 0.f);
	Site(TEXT("Village"), 1, 165, 5, 0, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_Hotel"), 204, 210, 10, 5, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("RescuePost"), 1, 232, 11, 6, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_Lodge"), 152, 160, 6, 4, 0.9f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/Maps/Glacier/Props/SM_Chalet"), 190, 262, 3, 1, 0.7f, 1.f, 0.15f, -1, 40.f);
	Scatter(TEXT("/Game/Maps/Glacier/Props/SM_Chalet"), 228, 262, 3, 1, 0.7f, 1.f, 0.15f, 1, 40.f);
	Scatter(TEXT("SM_Snowcat"), 150, 232, 5, 1, 0.8f, 1.f, 0.f, 0, 180.f);
	Far(TEXT("SM_RidgeSaw"), 345, 400, 9, 4, 0.7f, 1.f, 0.15f, 0, 8.f, 10.f);
	Far(TEXT("SM_RidgeLiftLine"), 440, 680, 6, 3, 1.f, 1.2f, 0.1f, 0, 20.f, 8.f);
	Far(TEXT("SM_PeakGreat"), 800, 1200, 8, 1, 1.f, 1.4f, 0.3f, 0, 25.f, 60.f);
	Far(TEXT("SM_PeakTwin"), 700, 1000, 12, 2, 1.f, 1.2f, 0.2f, 0, 25.f, 40.f);

Zone(TEXT("The last lifts, the verge (1)"), 22.4f, 22.58f);
	Site(TEXT("FenceL"), -1, 19, 4, 3, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 4, 1, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 2, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("PipeSide"), 0, 33, 9, 2, 0.9f, 1.f, 0.f);

Zone(TEXT("The last lifts, the verge (2)"), 22.93f, 23.72f);
	Site(TEXT("FenceL"), -1, 19, 4, 3, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 4, 1, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 2, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("PipeSide"), 0, 33, 9, 2, 0.9f, 1.f, 0.f);

Zone(TEXT("The last lifts, the verge (3)"), 24.07f, 99.f);
	Site(TEXT("FenceL"), -1, 19, 4, 3, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 4, 1, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 2, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("PipeSide"), 0, 33, 9, 2, 0.9f, 1.f, 0.f);
