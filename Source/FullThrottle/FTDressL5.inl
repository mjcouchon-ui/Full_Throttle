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

// THIRD PASS (15 Nov 2026, the overhaul: "dense, distinctive and alive at racing speed"). What it added:
//  - The lifts MOVE: SM_LiftSpan is a moving prop now (ft_dress_ice.lift_span_live): chairs travel up one cable and
//    down the other, every span in step with the next. The rules that place it are unchanged.
//  - The gondola over the road (signature; the set GondolaCross): 140 m of cable from a station 70 m out on one side to
//    one on the other, over a pylon either side of the road (30 m out), eight cabins crossing 17 m over the road. At
//    0.226 km (the top), 3.529 km (leaving the super pipe) and 8.325 km (the valley, where the third jump lands). The
//    cable is a piece of its own (SM_CableCross: nothing of it is within 9 m of the deck, so the track's road test lets
//    it stand over the road; under 2.5 m tall, so it casts no shadow). Not in the finale: the fight has the road.
//  - The skyline is flat cards in receding tones now, not cones: SM_RidgeSaw (the near ridge, 340 to 400 m),
//    SM_RangeMid (the sierra, 660 to 800 m) and SM_RangeFar (the massif, 1.3 to 1.5 km, 1.2 km long), written once for
//    the whole road ("The far ranges"). Snow streams off the summits of the two far ones. The chute keeps its steep
//    peaks overhead; the glacier tongues, the far lift lines and the observatory stay. About 140 fewer horizon pieces
//    (none of them is ever culled).
//  - Spindrift nearer: off the tops of the ice walls (SM_RockIceWall, SM_RockIceWall2) and off the cornices
//    (SM_RockIceShelf), all three moving props under their old names; ice walls now also 34 m from the road in the
//    glacier districts. Snow lances blowing by the piste (SM_SnowLance, 27 m out, footed 12 m down).
//  - The near layer. FOUND: this hill falls 7 in 100, and the track's sight line (anything 2 m over a road keeps 24 m
//    from its centre line) counts a piece's top against the road further DOWN the hill as well. So a piece 18 m out
//    whose box tops about 1.4 m is pushed out to the 24 m line: every fence, bank, marker and snow gun of the first two
//    passes stands at 24.7 m, not where it was written, and the strip behind the barrier was empty. What can stand
//    there is low and short: crash padding in the resort's colours (SM_PadRun, 1.2 m, 8 m, three to a slot: the sets
//    PadsR / PadsL, each length at its own height down the grade), and in the glacier districts ice blocks at 0.85 of
//    their size (IceKerbR / IceKerbL). The wooden snow fence is on every slot of the ridge districts now.
//  - Wind-driven snow: gusts that lift the snow off the snowfield 58 m out and stream it 20 m down the wind
//    (SM_SnowGust, a moving prop: its box is the 20 m its snow travels, so it is written 58 m out), on the ridge
//    districts, the top and the whiteout.
//  - Lodges up to the road: SM_ChaletRow (three chalets gable-on, lit windows, 27 m out) with a lodge behind, where the
//    road runs straight through the resort (the sets StreetR / StreetL).

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

// Third pass. Crash padding right behind the barrier: three lengths to a slot, each at its own height down the grade
// (Up = -0.07 x along on the right, +0.07 x along on the left).
Set(TEXT("PadsR"), { { TEXT("SM_PadRun"), -15, 0, 1.05f, 0, 1 }, { TEXT("SM_PadRun"), 0, 0, 0, 0, 1 }, { TEXT("SM_PadRun"), 15, 0, -1.05f, 0, 1 } });
Set(TEXT("PadsL"), { { TEXT("SM_PadRun"), -15, 0, -1.05f, 0, 1 }, { TEXT("SM_PadRun"), 0, 0, 0, 0, 1 }, { TEXT("SM_PadRun"), 15, 0, 1.05f, 0, 1 } });
// Blocks of blue ice along the verge of the glacier districts: three clusters to a slot, under 1.3 m (see the head).
Set(TEXT("IceKerbR"), {
	{ TEXT("SM_SnowRockIce"),       -15,    0,  1.05f,    0, 0.85f },
	{ TEXT("SM_SnowRockIce"),         0, 0.6f,      0,  180, 0.8f },
	{ TEXT("SM_SnowRockIce"),        15,    0, -1.05f,    0, 0.85f },
});
Set(TEXT("IceKerbL"), {
	{ TEXT("SM_SnowRockIce"),       -15,    0, -1.05f,    0, 0.85f },
	{ TEXT("SM_SnowRockIce"),         0, 0.6f,      0,  180, 0.8f },
	{ TEXT("SM_SnowRockIce"),        15,    0,  1.05f,    0, 0.85f },
});
// The gondola over the road (signature). The anchor is the right-hand pylon, 30 m out; the cable's middle is over the
// road's middle, 16.8 m up, and its stations stand 70 m out on either side (the left one turned to face the road).
Set(TEXT("GondolaCross"), {
	{ TEXT("SM_CablePylon"),          0,    0,      0,    0, 1 },
	{ TEXT("SM_CablePylon"),          0,  -60,      0,    0, 1 },
	{ TEXT("SM_CableCross"),          0,  -30,  16.8f,    0, 1 },
	{ TEXT("SM_CableTower"),          0,   40,      0,    0, 1 },
	{ TEXT("SM_CableTower"),          0, -100,      0,  180, 1 },
});
// The resort's street front, with a lodge behind it and a groomer parked between.
Set(TEXT("StreetR"), {
	{ TEXT("SM_ChaletRow"),           0,    0,      0,    0, 1 },
	{ TEXT("SM_Lodge"),               5,   26,      0,    0, 1 },
	{ TEXT("SM_Snowcat"),           -15,   12,  1.05f,   30, 1 },
});
Set(TEXT("StreetL"), {
	{ TEXT("SM_ChaletRow"),           0,    0,      0,    0, 1 },
	{ TEXT("SM_Lodge"),               5,   26,      0,    0, 1 },
	{ TEXT("SM_Snowcat"),           -15,   12, -1.05f,   30, 1 },
});
// Wind-driven snow: gusts lifting the snow off the snowfield and streaming it down the world's wind (the same way on
// both sides of the road, whichever way the piece is turned).
Set(TEXT("GustR"), { { TEXT("SM_SnowGust"), 0, 0, 0, 0, 1 } });
Set(TEXT("GustL"), { { TEXT("SM_SnowGust"), 0, 0, 0, 180, 1 } });
Set(TEXT("Row"), { { TEXT("SM_ChaletRow"), 0, 0, 0, 0, 1 } });    // the street front alone, where the resort's ground (a rink, the service road) lies behind it

// ---- SECOND PASS, before the districts: the lakes, the land under the set pieces, the jumps' aprons, the banners.
// A frozen lake under each jump: one slot each (51, 111, 181), the lake's sheet at its own height above the floor.
Zone(TEXT("Under the first jump: the lake"), 2.3f, 2.32f);
	SiteAfloat(TEXT("Tarn1"), 1, 0, 1, 0, 1.f);

Zone(TEXT("Under the second jump: the lake"), 5.01f, 5.03f);
	SiteAfloat(TEXT("Tarn2"), 1, 0, 1, 0, 1.f);

Zone(TEXT("Under the third jump: the lake"), 8.18f, 8.2f);
	SiteAfloat(TEXT("Tarn3"), 1, 0, 1, 0, 1.f);

// ---- THIRD PASS, before the districts.
// The far ranges, for the whole road: three rows in receding tones, each a different outline, left and right out of
// step. The near ridge has gaps (three slots in five get one), so the two behind it show through.
Zone(TEXT("The far ranges"), 0.f, 99.f);
	Far(TEXT("SM_RangeFar"), 1300, 1500, 20, 3, 1.f, 1.f, 0.1f, -1, 30.f, 6.f);
	Far(TEXT("SM_RangeFar"), 1300, 1500, 20, 13, 1.f, 1.f, 0.1f, 1, 30.f, 6.f);
	Far(TEXT("SM_RangeMid"), 660, 800, 14, 5, 1.f, 1.f, 0.12f, -1, 20.f, 8.f);
	Far(TEXT("SM_RangeMid"), 660, 800, 14, 12, 1.f, 1.f, 0.12f, 1, 20.f, 8.f);
	Far(TEXT("SM_RidgeSaw"), 340, 400, 8, 2, 0.6f, 0.8f, 0.12f, -1, 8.f, 8.f);
	Far(TEXT("SM_RidgeSaw"), 340, 400, 8, 6, 0.6f, 0.8f, 0.12f, 1, 8.f, 8.f);

// The gondola over the road, three times (one slot each: 5, 78 and 184; a slot is 45.24 m).
Zone(TEXT("The gondola over the top"), 0.22f, 0.23f);
	Site(TEXT("GondolaCross"), 1, 30, 1, 0, 1.f);

Zone(TEXT("The gondola over the glacier"), 3.52f, 3.535f);
	Site(TEXT("GondolaCross"), 1, 30, 1, 0, 1.f);

Zone(TEXT("The gondola over the valley"), 8.32f, 8.33f);
	Site(TEXT("GondolaCross"), 1, 30, 1, 0, 1.f);

// Crash padding right behind the barrier (third pass): three lengths a slot a side for the first 9 km, alternate slots a
// side after that; the banked bends and the pipes too (the land beside a bend is not cut: the padding stands on its lip, over the cutting).
Zone(TEXT("Crash padding"), 0.f, 9.f);
	Site(TEXT("PadsL"), -1, 19.5f, 1, 0, 0.95f);
	Site(TEXT("PadsR"), 1, 19.5f, 1, 0, 0.95f);

Zone(TEXT("Crash padding, the finale"), 9.f, 99.f);     // alternate slots a side
	Site(TEXT("PadsL"), -1, 19.5f, 2, 0, 0.95f);
	Site(TEXT("PadsR"), 1, 19.5f, 2, 1, 0.95f);

// Under the corkscrews and the loops the camera looks at the land, and Scatter and Site are left out there: so the
// resort's ground (rink, terrain park, car park) is written as Far rules with the land's own drop, one piece a side
// every second slot (a Far piece is moved up to 11 m along its slot: closer than that they would overlap), and the
// banners line the set piece itself. The land runs straight and level-across under these.
Zone(TEXT("Under the chute's corkscrew"), 2.51f, 2.685f);
	Far(TEXT("SM_SnowLance"), 28, 28.6f, 2, 1, 1.f, 1.f, 0.f, 0, 0.78f, 0.f);     // third pass: the lance's first piece (so it casts no shadow)
	Far(TEXT("SM_SnowGust"), 60, 62, 2, 0, 1.f, 1.f, 0.f, 0, 0.58f, 0.f);        // and the gust's
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
	Site(TEXT("LiftLine"), -1, 85, 8, 5, 1.f, 1.f, 0.f);     // third pass: phase 5 (was 3: slot 5 is the gondola's now)
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
	Far(TEXT("SM_PeakObservatory"), 430, 700, 9, 3, 1.f, 1.3f, 0.1f, 1, 25.f, 30.f);

Zone(TEXT("The summit street"), 0.1f, 0.15f);     // third pass: one row a side by the gondola's top station (0.136 km)
	Site(TEXT("StreetL"), -1, 27.6f, 1, 0, 1.f);
	Site(TEXT("StreetR"), 1, 27.6f, 1, 0, 1.f);

Zone(TEXT("First tracks, the verge"), 0.f, 0.25f);
	Site(TEXT("GustL"), -1, 58, 2, 0, 0.9f);     // third pass: wind-driven snow (left only: the right has the street's lodge)
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

Zone(TEXT("The half pipe, the verge (1)"), 1.21f, 1.77f);
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 3, 0, 0.85f, 1.f, 0.f, -1, 0.f);     // third pass: snow lances, blowing
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 3, 1, 0.85f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PipeSide"), 0, 33, 2, 0, 0.9f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 2, 1, 0.7f, 1.f, 0.f, 0, 15.f);
	Site(TEXT("FenceL"), -1, 19, 6, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 6, 3, 0.8f, 1.f, 0.f);

Zone(TEXT("The half pipe, the verge (2)"), 2.1f, 2.43f);
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 3, 0, 0.85f, 1.f, 0.f, -1, 0.f);     // third pass: snow lances, blowing
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 3, 1, 0.85f, 1.f, 0.f, 1, 0.f);
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
	Far(TEXT("SM_RockIceWall"), 33.5f, 35.5f, 3, 1, 0.9f, 1.f, 0.1f, -1, 0.74f, 2.f);     // third pass: ice walls right by the road, a gap between
	Far(TEXT("SM_RockIceWall"), 33.5f, 35.5f, 3, 2, 0.9f, 1.f, 0.1f, 1, 0.74f, 2.f);
	Far(TEXT("SM_RockIceWall"), 38, 46, 1, 0, 0.7f, 1.f, 0.15f, 0, 0.69f, 6.f);
	Site(TEXT("IceFall"), 0, 118, 1, 0, 0.85f, 1.f, 0.1f);
	Far(TEXT("SM_RockIceWall2"), 100, 110, 2, 0, 0.8f, 1.2f, 0.2f, 0, -2.54f, 6.f);
	Far(TEXT("SM_RockSeracB"), 150, 160, 1, 0, 0.6f, 1.3f, 0.3f, 0, -8.52f, 180.f);
	Far(TEXT("SM_RockIceFalls"), 196, 206, 4, 2, 0.9f, 1.2f, 0.f, 0, -14.02f, 10.f);
	Far(TEXT("SM_RockIceWall2"), 240, 250, 2, 1, 0.8f, 1.4f, 0.2f, 0, -19.27f, 6.f);
	Far(TEXT("SM_RidgeGlacier"), 380, 600, 5, 1, 1.f, 1.f, 0.15f, 0, 20.f, 14.f);

Zone(TEXT("The super pipe, the verge"), 2.96f, 3.59f);
	Site(TEXT("IceKerbL"), -1, 20.5f, 1, 0, 0.9f);     // third pass: ice blocks, three clusters a slot a side
	Site(TEXT("IceKerbR"), 1, 20.5f, 1, 0, 0.9f);

// ---- 4.02 to 5.65 km: BLACK DIAMOND, the exposed ridge (a loop, a jump, a corkscrew). Wind country: fences down the
// whole verge, cornices and carved drift on the slopes, crevasse fields, a weather post and signal masts, and the far
// ranges open on both sides. The slopes' dressing stays through the loop and the jump. Second pass: the far range
// stood too near and too big here (a lavender flank filled a corner of the view from the jump): further and smaller.
Zone(TEXT("Black diamond"), 4.02f, 5.65f);
	Far(TEXT("SM_RockIceShelf"), 138, 146, 2, 0, 0.9f, 1.f, 0.15f, 0, -6.96f, 8.f);
	Site(TEXT("WeatherPost"), 1, 172, 9, 3, 1.f, 1.f, 0.f);
	Far(TEXT("SM_PoleSignal"), 176, 184, 4, 1, 0.7f, 1.2f, 0.f, 0, -11.5f, 0.f);
	Far(TEXT("SM_PeakObservatory"), 500, 800, 11, 4, 1.f, 1.3f, 0.1f, 0, 25.f, 30.f);
	Far(TEXT("SM_RidgeLiftLine"), 420, 650, 9, 6, 1.f, 1.f, 0.1f, 0, 20.f, 8.f);

Zone(TEXT("Black diamond, the verge (1)"), 4.03f, 4.46f);
	Site(TEXT("GustL"), -1, 58, 2, 0, 0.9f);     // third pass: wind-driven snow, a gust every other slot a side (58 m: its snow travels 20 m)
	Site(TEXT("GustR"), 1, 58, 2, 1, 0.9f);
	Site(TEXT("FenceL"), -1, 19, 1, 0, 0.9f, 1.f, 0.f);     // third pass: every slot (was every other)
	Site(TEXT("FenceR"), 1, 19, 1, 0, 0.9f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 0, 0.6f, 1.f, 0.2f, 0, 2.f);

Zone(TEXT("Black diamond, the verge (2)"), 5.12f, 5.43f);     // (review) starts past the second jump: fences and banks hung beside its pylons
	Site(TEXT("GustL"), -1, 58, 2, 0, 0.9f);     // third pass: wind-driven snow, a gust every other slot a side (58 m: its snow travels 20 m)
	Site(TEXT("GustR"), 1, 58, 2, 1, 0.9f);
	Site(TEXT("FenceL"), -1, 19, 1, 0, 0.9f, 1.f, 0.f);     // third pass: every slot (was every other)
	Site(TEXT("FenceR"), 1, 19, 1, 0, 0.9f, 1.f, 0.f);
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

Zone(TEXT("The whiteout, the verge (1)"), 5.66f, 6.33f);
	Site(TEXT("GustL"), -1, 58, 2, 0, 0.9f);     // third pass: wind-driven snow, a gust every other slot a side (58 m: its snow travels 20 m)
	Site(TEXT("GustR"), 1, 58, 2, 1, 0.9f);
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 3, 0, 0.85f, 1.f, 0.f, -1, 0.f);     // third pass: snow lances, blowing
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 3, 1, 0.85f, 1.f, 0.f, 1, 0.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 1, 0, 0.8f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_PoleMarker2"), 18, 19.5f, 1, 0, 0.5f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("FenceL"), -1, 20.5f, 2, 0, 0.7f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 20.5f, 2, 1, 0.7f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Site(TEXT("PipeSide"), 0, 33, 3, 1, 0.9f, 1.f, 0.f);

Zone(TEXT("The whiteout, the verge (2)"), 6.82f, 7.3f);
	Site(TEXT("GustL"), -1, 58, 2, 0, 0.9f);     // third pass: wind-driven snow, a gust every other slot a side (58 m: its snow travels 20 m)
	Site(TEXT("GustR"), 1, 58, 2, 1, 0.9f);
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 3, 0, 0.85f, 1.f, 0.f, -1, 0.f);     // third pass: snow lances, blowing
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 3, 1, 0.85f, 1.f, 0.f, 1, 0.f);
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
	Site(TEXT("LiftLine"), -1, 85, 5, 2, 1.f, 1.f, 0.f);     // third pass: phase 2 (was 1: slot 184 is the gondola's now)
	Site(TEXT("Village"), 1, 165, 4, 0, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_Hotel"), 204, 210, 8, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("DepotR"), 1, 68, 6, 0, 1.f);
	Scatter(TEXT("SM_Lodge"), 152, 160, 5, 2, 0.9f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/Maps/Glacier/Props/SM_Chalet"), 190, 262, 2, 0, 0.7f, 1.f, 0.15f, -1, 40.f);
	Scatter(TEXT("/Game/Maps/Glacier/Props/SM_Chalet"), 228, 262, 2, 0, 0.7f, 1.f, 0.15f, 1, 40.f);
	Scatter(TEXT("SM_Snowcat"), 150, 232, 4, 1, 0.8f, 1.f, 0.f, 0, 180.f);
	Far(TEXT("SM_RidgeLiftLine"), 400, 640, 5, 0, 1.f, 1.f, 0.1f, 0, 20.f, 8.f);
	Far(TEXT("SM_PeakObservatory"), 520, 820, 13, 7, 1.f, 1.3f, 0.1f, 0, 25.f, 30.f);

// Third pass: THE VILLAGE STREET. Chalets gable-on to the road, both sides, a lodge behind each row: after the corkscrew
// (7.42 and 7.47 km), before the jump (8.01 km), and where the jump lands (8.23 and 8.28 km), with the gondola crossing
// over the road right after (8.325 km: signature, see "The gondola over the valley").
Zone(TEXT("The village street (7.4 km)"), 7.4f, 7.5f);
	Site(TEXT("StreetL"), -1, 27.6f, 1, 0, 1.f);
	Site(TEXT("StreetR"), 1, 27.6f, 1, 0, 1.f);

Zone(TEXT("The village street (8.0 km)"), 8.f, 8.02f);
	Site(TEXT("StreetL"), -1, 27.6f, 1, 0, 1.f);
	Site(TEXT("StreetR"), 1, 27.6f, 1, 0, 1.f);

Zone(TEXT("The village street (8.25 km)"), 8.22f, 8.3f);
	Site(TEXT("StreetL"), -1, 27.6f, 1, 0, 1.f);
	Site(TEXT("Row"), 1, 27.6f, 1, 0, 1.f);     // the landing's rink lies behind these

Zone(TEXT("The valley run, its galleries (7.3 km)"), 7.3f, 7.52f);
	Site(TEXT("Cutting"), 1, 30, 5, 3, 1.f, 1.f, 0.f);

Zone(TEXT("The valley run, its galleries (7.98 km)"), 7.98f, 8.37f);
	Site(TEXT("Cutting"), 1, 30, 5, 3, 0.f, 1.f, 0.f);     // (review) off: the street front stands on its only two slots

Zone(TEXT("The valley's pipe, its lances"), 8.39f, 8.94f);     // third pass
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 3, 0, 0.85f, 1.f, 0.f, -1, 0.f);     // third pass: snow lances, blowing
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 3, 1, 0.85f, 1.f, 0.f, 1, 0.f);

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
	Far(TEXT("SM_RockIceWall"), 33.5f, 35.5f, 3, 0, 0.9f, 1.f, 0.1f, -1, 0.74f, 2.f);     // third pass: ice walls right by the road, a gap between
	Far(TEXT("SM_RockIceWall"), 33.5f, 35.5f, 3, 1, 0.9f, 1.f, 0.1f, 1, 0.74f, 2.f);
	Far(TEXT("SM_RockIceWall"), 38, 46, 2, 0, 0.7f, 1.f, 0.15f, 0, 0.69f, 6.f);
	Far(TEXT("SM_RockIceWall2"), 62, 72, 3, 1, 0.7f, 1.f, 0.2f, 0, 0.53f, 6.f);
	Site(TEXT("IceKerbL"), -1, 20.5f, 1, 0, 0.9f);     // third pass: ice blocks, three clusters a slot a side
	Site(TEXT("IceKerbR"), 1, 20.5f, 1, 0, 0.9f);
	Scatter(TEXT("SM_SnowRockBank"), 20.2f, 22, 3, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_PoleMarker2"), 18, 19.5f, 4, 2, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("IceFall"), 0, 120, 3, 0, 0.9f, 1.f, 0.1f);
	Site(TEXT("SeracField"), 0, 205, 3, 1, 1.f, 1.f, 0.f);
	Far(TEXT("SM_SnowRockCrevasse"), 170, 176, 3, 2, 0.7f, 1.f, 0.2f, 0, -10.67f, 8.f);
	Far(TEXT("SM_RockIceFalls"), 250, 258, 4, 1, 0.9f, 1.2f, 0.f, 0, -20.35f, 10.f);
	Far(TEXT("SM_RidgeGlacier"), 380, 600, 6, 0, 1.f, 1.f, 0.15f, 0, 20.f, 14.f);

// 11.6 to 14.3 km, THE WHITE RIDGE: fences, cornices, carved drift and the weather post; the far ranges open.
Zone(TEXT("The white ridge"), 11.6f, 14.3f);
	Site(TEXT("GustL"), -1, 58, 2, 0, 0.9f);     // third pass: wind-driven snow, a gust every other slot a side (58 m: its snow travels 20 m)
	Site(TEXT("GustR"), 1, 58, 2, 1, 0.9f);
	Site(TEXT("FenceL"), -1, 19, 1, 0, 0.9f, 1.f, 0.f);     // third pass: every slot (was every other)
	Site(TEXT("FenceR"), 1, 19, 1, 0, 0.9f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 0, 0.6f, 1.f, 0.2f, 0, 2.f);
	Far(TEXT("SM_RockIceShelf"), 138, 146, 2, 0, 0.9f, 1.f, 0.15f, 0, -6.96f, 8.f);
	Site(TEXT("WeatherPost"), 0, 172, 9, 3, 1.f, 1.f, 0.f);
	Far(TEXT("SM_PoleSignal"), 176, 184, 5, 1, 0.7f, 1.2f, 0.f, 0, -11.5f, 0.f);
	Far(TEXT("SM_SnowRockDrift"), 150, 158, 1, 0, 0.7f, 1.2f, 0.2f, 0, -8.4f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 204, 212, 2, 1, 0.7f, 1.2f, 0.2f, 0, -14.85f, 8.f);
	Far(TEXT("SM_SnowRockDrift"), 250, 256, 2, 0, 0.7f, 1.2f, 0.2f, 0, -20.23f, 8.f);
	Far(TEXT("SM_SnowRockCrevasse"), 226, 232, 2, 0, 0.7f, 1.f, 0.2f, 0, -17.36f, 8.f);
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
	Far(TEXT("SM_RidgeLiftLine"), 400, 640, 5, 0, 1.f, 1.f, 0.1f, 0, 20.f, 8.f);
	Far(TEXT("SM_PeakObservatory"), 520, 820, 13, 7, 1.f, 1.3f, 0.1f, 0, 25.f, 30.f);

// Third pass: the village street on the resort's straights (between each pipe and the next bends).
// (review, 15 Nov) On the Abominable's road the street fronts stand 33 m out, not 27.6 (their windows, lit and
// switching, were the brightest things beside his fight), and the snow lances are off (chance 0: white balls of
// snow moving 28 m from the road where he hurls snowballs). The village street at 7.4 to 8.3 km keeps both.
Zone(TEXT("The resort's street (1)"), 14.3f, 14.6f);
	Site(TEXT("Row"), -1, 33.f, 2, 0, 1.f);
	Site(TEXT("Row"), 1, 33.f, 2, 1, 1.f);

Zone(TEXT("The resort's street (2)"), 15.46f, 15.73f);
	Site(TEXT("Row"), -1, 33.f, 2, 0, 1.f);
	Site(TEXT("Row"), 1, 33.f, 2, 1, 1.f);

Zone(TEXT("The resort's street (3)"), 16.6f, 16.87f);
	Site(TEXT("Row"), -1, 33.f, 2, 0, 1.f);
	Site(TEXT("Row"), 1, 33.f, 2, 1, 1.f);

Zone(TEXT("The resort, the verge (1)"), 14.3f, 14.62f);
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 4, 1, 0.f, 1.f, 0.f, -1, 0.f);     // third pass: snow lances, blowing
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 4, 0, 0.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("FenceL"), -1, 19, 4, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 4, 2, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 2, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("PipeSide"), 0, 33, 9, 2, 0.9f, 1.f, 0.f);

Zone(TEXT("The resort, the verge (2)"), 14.97f, 15.75f);
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 4, 1, 0.f, 1.f, 0.f, -1, 0.f);     // third pass: snow lances, blowing
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 4, 0, 0.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("FenceL"), -1, 19, 4, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 4, 2, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 2, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("PipeSide"), 0, 33, 9, 2, 0.9f, 1.f, 0.f);

Zone(TEXT("The resort, the verge (3)"), 16.11f, 16.89f);
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 4, 1, 0.f, 1.f, 0.f, -1, 0.f);     // third pass: snow lances, blowing
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 4, 0, 0.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("FenceL"), -1, 19, 4, 0, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 4, 2, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 2, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("PipeSide"), 0, 33, 9, 2, 0.9f, 1.f, 0.f);

// 17.0 to 19.7 km: the glacier again, its tongues nearer.
Zone(TEXT("The ice again"), 17.f, 19.7f);
	Far(TEXT("SM_RockIceWall"), 33.5f, 35.5f, 3, 1, 0.9f, 1.f, 0.1f, -1, 0.74f, 2.f);     // third pass: ice walls right by the road, a gap between
	Far(TEXT("SM_RockIceWall"), 33.5f, 35.5f, 3, 2, 0.9f, 1.f, 0.1f, 1, 0.74f, 2.f);
	Far(TEXT("SM_RockIceWall"), 38, 46, 2, 1, 0.7f, 1.f, 0.15f, 0, 0.69f, 6.f);
	Far(TEXT("SM_RockIceWall2"), 62, 72, 3, 1, 0.7f, 1.f, 0.2f, 0, 0.53f, 6.f);
	Site(TEXT("IceFall"), 0, 120, 3, 1, 0.9f, 1.f, 0.1f);
	Site(TEXT("SeracField"), 0, 205, 3, 2, 1.f, 1.f, 0.f);
	Far(TEXT("SM_SnowRockCrevasse"), 170, 176, 3, 2, 0.7f, 1.f, 0.2f, 0, -10.67f, 8.f);
	Far(TEXT("SM_RockIceFalls"), 250, 258, 4, 1, 0.9f, 1.2f, 0.f, 0, -20.35f, 10.f);
	Far(TEXT("SM_RidgeGlacier"), 420, 640, 4, 2, 1.f, 1.2f, 0.15f, 0, 20.f, 14.f);

Zone(TEXT("The ice again, the verge (1)"), 17.24f, 18.03f);
	Site(TEXT("IceKerbL"), -1, 20.5f, 1, 0, 0.9f);     // third pass: ice blocks, three clusters a slot a side
	Site(TEXT("IceKerbR"), 1, 20.5f, 1, 0, 0.9f);
	Scatter(TEXT("SM_SnowRockBank"), 20.2f, 22, 3, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_PoleMarker2"), 18, 19.5f, 4, 2, 0.8f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("The ice again, the verge (2)"), 18.38f, 19.17f);
	Site(TEXT("IceKerbL"), -1, 20.5f, 1, 0, 0.9f);     // third pass: ice blocks, three clusters a slot a side
	Site(TEXT("IceKerbR"), 1, 20.5f, 1, 0, 0.9f);
	Scatter(TEXT("SM_SnowRockBank"), 20.2f, 22, 3, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_PoleMarker2"), 18, 19.5f, 4, 2, 0.8f, 1.f, 0.f, 0, 0.f);

Zone(TEXT("The ice again, the verge (3)"), 19.52f, 19.7f);
	Site(TEXT("IceKerbL"), -1, 20.5f, 1, 0, 0.9f);     // third pass: ice blocks, three clusters a slot a side
	Site(TEXT("IceKerbR"), 1, 20.5f, 1, 0, 0.9f);
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
	Far(TEXT("SM_RidgeLiftLine"), 420, 650, 7, 2, 1.f, 1.f, 0.1f, 0, 20.f, 8.f);

Zone(TEXT("The high ridge, the verge (1)"), 19.7f, 20.3f);
	Site(TEXT("GustL"), -1, 58, 2, 0, 0.9f);     // third pass: wind-driven snow, a gust every other slot a side (58 m: its snow travels 20 m)
	Site(TEXT("GustR"), 1, 58, 2, 1, 0.9f);
	Site(TEXT("FenceL"), -1, 19, 1, 0, 0.9f, 1.f, 0.f);     // third pass: every slot (was every other)
	Site(TEXT("FenceR"), 1, 19, 1, 0, 0.9f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);

Zone(TEXT("The high ridge, the verge (2)"), 20.66f, 21.44f);
	Site(TEXT("GustL"), -1, 58, 2, 0, 0.9f);     // third pass: wind-driven snow, a gust every other slot a side (58 m: its snow travels 20 m)
	Site(TEXT("GustR"), 1, 58, 2, 1, 0.9f);
	Site(TEXT("FenceL"), -1, 19, 1, 0, 0.9f, 1.f, 0.f);     // third pass: every slot (was every other)
	Site(TEXT("FenceR"), 1, 19, 1, 0, 0.9f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowRockBank2"), 20.2f, 22, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);

Zone(TEXT("The high ridge, the verge (3)"), 21.79f, 22.4f);
	Site(TEXT("GustL"), -1, 58, 2, 0, 0.9f);     // third pass: wind-driven snow, a gust every other slot a side (58 m: its snow travels 20 m)
	Site(TEXT("GustR"), 1, 58, 2, 1, 0.9f);
	Site(TEXT("FenceL"), -1, 19, 1, 0, 0.9f, 1.f, 0.f);     // third pass: every slot (was every other)
	Site(TEXT("FenceR"), 1, 19, 1, 0, 0.9f, 1.f, 0.f);
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
	Far(TEXT("SM_RidgeLiftLine"), 440, 680, 6, 3, 1.f, 1.2f, 0.1f, 0, 20.f, 8.f);

Zone(TEXT("The last street (1)"), 22.4f, 22.56f);     // third pass
	Site(TEXT("Row"), -1, 33.f, 2, 0, 1.f);
	Site(TEXT("Row"), 1, 33.f, 2, 1, 1.f);

Zone(TEXT("The last street (2)"), 23.42f, 23.7f);
	Site(TEXT("Row"), -1, 33.f, 2, 0, 1.f);
	Site(TEXT("Row"), 1, 33.f, 2, 1, 1.f);

Zone(TEXT("The last lifts, the verge (1)"), 22.4f, 22.58f);
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 4, 1, 0.f, 1.f, 0.f, -1, 0.f);     // third pass: snow lances, blowing
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 4, 0, 0.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("FenceL"), -1, 19, 4, 3, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 4, 1, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 2, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("PipeSide"), 0, 33, 9, 2, 0.9f, 1.f, 0.f);

Zone(TEXT("The last lifts, the verge (2)"), 22.93f, 23.72f);
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 4, 1, 0.f, 1.f, 0.f, -1, 0.f);     // third pass: snow lances, blowing
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 4, 0, 0.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("FenceL"), -1, 19, 4, 3, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 4, 1, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 2, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("PipeSide"), 0, 33, 9, 2, 0.9f, 1.f, 0.f);

Zone(TEXT("The last lifts, the verge (3)"), 24.07f, 99.f);
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 4, 1, 0.f, 1.f, 0.f, -1, 0.f);     // third pass: snow lances, blowing
	Scatter(TEXT("SM_SnowLance"), 27.5f, 29, 4, 0, 0.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("FenceL"), -1, 19, 4, 3, 0.8f, 1.f, 0.f);
	Site(TEXT("FenceR"), 1, 19, 4, 1, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_SnowCannon"), 20, 21.5f, 3, 2, 0.7f, 1.f, 0.f, 0, 15.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 2, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("PipeSide"), 0, 33, 9, 2, 0.9f, 1.f, 0.f);

// ---- THE CUT BANKS (15 Nov 2026, after the second pass). Inside a banked bend the track cuts the land down to the
// road's low edge; that cutting was the land's one plain colour, a third of the frame from the rolled camera, and no
// Scatter or Site piece can stand on it. Bank rules (FTDress.h) have the track lay panels on it itself, fitted to the
// bank's own angle and grade: four to a 45 m slot (11.3 m of road each), tall designs where a panel's run up the
// cutting is 5.5 m or more (bends banked 32 degrees and up), a low one on the shallower cuttings (down to 2.4 m: about
// 20 degrees of bank). Every and phase count panels here. ft.DressBank 0 leaves them out, for a before and an after.
// Ice Mountain: the road cut through the glacier. SM_CutIce (layers of blue ice, splinters standing proud, a cornice
// of snow) on two panels in three of the deep cuttings (2.7 to 3.0, 4.5 to 4.8, 7.5 to 8.0 km and the bends of the
// top), SM_CutIce2 (the mountain's own rock in place of the splinters) on the third. SM_CutSnow (a ploughed bank, a
// seam of ice at its foot) on the shallow ones: most of this level's bends, and all of the finale's. The road falls
// 7 in 100 all the way: the track pitches each panel.
Zone(TEXT("Cut banks"), 0.f, 99.f);
	Bank(TEXT("SM_CutIce"), 4, 3, 0, 1.f, 5.5f);
	Bank(TEXT("SM_CutIce"), 4, 3, 1, 1.f, 5.5f);
	Bank(TEXT("SM_CutIce2"), 4, 3, 2, 1.f, 5.5f);
	Bank(TEXT("SM_CutSnow"), 4, 1, 0, 1.f, 2.4f, 5.5f);

// ---- ROUND THREE (15 Nov 2026): the built world in motion. Everything of this round is APPENDED here, after the cut
// banks, so that no older rule's place in the table (which seeds its random stream) moves; in place, numbers only:
// the crash pads' four rules from 18 m to 19.5 m (in bends they came as near as 15.8 m).
//  - A rack railway climbing the slope (SM_RackTrain: red trains up and down 170 m of track, its foot 96 m out).
//  - Groomers at work on the pistes (SM_GroomerRun: a groomed strip 120 m up the slope, two groomers, 84 m out).
//  - A snow-plough convoy on a far road (SM_PloughConvoy: 335 m out and more, 30 m up, along the near ridge's foot).
//  - Cloud drifting past the summits (SM_CloudDrift: 900 m out and more, 260 m up).
//  - No rule is needed for the rest of the round: the far lift line's cabins travel (SM_RidgeLiftLine is a moving
//    prop now), the middle range avalanches on a clock, the ranges are taller with bigger banners.
// None of these casts a shadow: they stand beyond 70 m or are Far.
Zone(TEXT("Round three: the rack railway, the top"), 0.3f, 1.19f);
	Scatter(TEXT("SM_RackTrain"), 96, 102, 9, 3, 0.f, 1.f, 0.f, 1, 6.f);     // second part: off (chance 0): laid across the slope now (the sets RackR and RackL at the end of this file)

Zone(TEXT("Round three: the rack railway, the half pipe"), 1.19f, 2.43f);
	Scatter(TEXT("SM_RackTrain"), 96, 102, 9, 5, 0.f, 1.f, 0.f, -1, 6.f);     // second part: off (chance 0): laid across the slope now (the sets RackR and RackL at the end of this file)

Zone(TEXT("Round three: the rack railway, the whiteout"), 5.65f, 7.3f);
	Scatter(TEXT("SM_RackTrain"), 96, 102, 9, 0, 0.f, 1.f, 0.f, 1, 6.f);     // second part: off (chance 0): laid across the slope now (the sets RackR and RackL at the end of this file)

Zone(TEXT("Round three: the rack railway, the valley run"), 7.3f, 8.94f);
	Scatter(TEXT("SM_RackTrain"), 96, 102, 9, 7, 0.f, 1.f, 0.f, 1, 6.f);     // second part: off (chance 0): laid across the slope now (the sets RackR and RackL at the end of this file)

Zone(TEXT("Round three: the rack railway, the white ridge"), 11.6f, 14.3f);
	Scatter(TEXT("SM_RackTrain"), 96, 102, 10, 0, 0.f, 1.f, 0.f, -1, 6.f);     // second part: off (chance 0): laid across the slope now (the sets RackR and RackL at the end of this file)
	Scatter(TEXT("SM_RackTrain"), 96, 102, 10, 5, 0.f, 1.f, 0.f, 1, 6.f);     // second part: off (chance 0): laid across the slope now (the sets RackR and RackL at the end of this file)

Zone(TEXT("Round three: the rack railway, the high ridge"), 19.7f, 22.4f);
	Scatter(TEXT("SM_RackTrain"), 96, 102, 10, 3, 0.f, 1.f, 0.f, -1, 6.f);     // second part: off (chance 0): laid across the slope now (the sets RackR and RackL at the end of this file)
	Scatter(TEXT("SM_RackTrain"), 96, 102, 10, 8, 0.f, 1.f, 0.f, 1, 6.f);     // second part: off (chance 0): laid across the slope now (the sets RackR and RackL at the end of this file)

Zone(TEXT("Round three: groomers, the top"), 0.26f, 1.19f);
	Scatter(TEXT("SM_GroomerRun"), 84, 90, 7, 1, 0.f, 1.f, 0.f, 1, 10.f);     // second part: off (chance 0): laid across the slope now (the sets GroomR and GroomL at the end of this file)

Zone(TEXT("Round three: groomers, the half pipe"), 1.19f, 2.43f);
	Scatter(TEXT("SM_GroomerRun"), 84, 90, 7, 4, 0.f, 1.f, 0.f, 1, 10.f);     // second part: off (chance 0): laid across the slope now (the sets GroomR and GroomL at the end of this file)

Zone(TEXT("Round three: groomers, Black diamond"), 4.02f, 5.65f);
	Scatter(TEXT("SM_GroomerRun"), 84, 90, 7, 2, 0.f, 1.f, 0.f, -1, 10.f);     // second part: off (chance 0): laid across the slope now (the sets GroomR and GroomL at the end of this file)
	Scatter(TEXT("SM_GroomerRun"), 84, 90, 7, 5, 0.f, 1.f, 0.f, 1, 10.f);     // second part: off (chance 0): laid across the slope now (the sets GroomR and GroomL at the end of this file)

Zone(TEXT("Round three: groomers, the whiteout"), 5.65f, 7.3f);
	Scatter(TEXT("SM_GroomerRun"), 84, 90, 7, 3, 0.f, 1.f, 0.f, -1, 10.f);     // second part: off (chance 0): laid across the slope now (the sets GroomR and GroomL at the end of this file)

Zone(TEXT("Round three: groomers, the valley run"), 7.3f, 8.94f);
	Scatter(TEXT("SM_GroomerRun"), 84, 90, 7, 6, 0.f, 1.f, 0.f, 1, 10.f);     // second part: off (chance 0): laid across the slope now (the sets GroomR and GroomL at the end of this file)

Zone(TEXT("Round three: groomers, the resort"), 14.3f, 17.f);
	Scatter(TEXT("SM_GroomerRun"), 84, 90, 8, 3, 0.f, 1.f, 0.f, 1, 10.f);     // second part: off (chance 0): laid across the slope now (the sets GroomR and GroomL at the end of this file)

Zone(TEXT("Round three: groomers, the last lifts"), 22.4f, 99.f);
	Scatter(TEXT("SM_GroomerRun"), 84, 90, 8, 5, 0.f, 1.f, 0.f, 1, 10.f);     // second part: off (chance 0): laid across the slope now (the sets GroomR and GroomL at the end of this file)

Zone(TEXT("Round three: the far road and the cloud"), 0.f, 99.f);
	Far(TEXT("SM_PloughConvoy"), 335, 360, 33, 4, 1.f, 1.f, 0.f, -1, -30.f, 3.f);
	Far(TEXT("SM_PloughConvoy"), 335, 360, 33, 20, 1.f, 1.f, 0.f, 1, -30.f, 3.f);
	Far(TEXT("SM_CloudDrift"), 900, 1100, 30, 7, 1.f, 1.f, 0.2f, -1, -260.f, 0.f);
	Far(TEXT("SM_CloudDrift"), 900, 1100, 30, 22, 1.f, 1.f, 0.2f, 1, -260.f, 0.f);

// ---- ROUND THREE, SECOND PART (15 Nov 2026; NOT COMPILED and not seen in the game when it was committed).
//  - The rack railway and the groomers' strips ran straight up the slope, away from the road: from the car their
//    trains and groomers only grew and shrank. Their sixteen Scatter rules are off in place (chance 0) and the same
//    pieces are laid here by sets, turned 50 degrees ACROSS the slope with the far end towards the car coming (yaw +50
//    on the right of the road, -50 on the left), on the same slots; the meshes are rebuilt with the slope's rise along
//    that line. Everything of round three also runs faster (the kit).
//  - The cable car beyond the gondola (SM_CableCar: a 600 m span up to a station on its own mountain, four big cabins),
//    its middle 640 m out on the RIGHT (the mesh is turned for that side), 26 m up (the snowfield's height out there).
//  - An aurora over the horizon ahead (SM_Aurora: two pale curtains 1.4 km long, 1.6 km out and more, 650 m up). The
//    level is in daylight: if it looks wrong in the game, its two rules' chance is the switch.
Set(TEXT("RackR"), { { TEXT("SM_RackTrain"), 0, 0, 0, 50, 1 } });
Set(TEXT("RackL"), { { TEXT("SM_RackTrain"), 0, 0, 0, -50, 1 } });
Set(TEXT("GroomR"), { { TEXT("SM_GroomerRun"), 0, 0, 0, 50, 1 } });
Set(TEXT("GroomL"), { { TEXT("SM_GroomerRun"), 0, 0, 0, -50, 1 } });

Zone(TEXT("Second part: the rack railway, the top"), 0.3f, 1.19f);
	Site(TEXT("RackR"), 1, 98, 9, 3, 1.f);
Zone(TEXT("Second part: the rack railway, the half pipe"), 1.19f, 2.43f);
	Site(TEXT("RackL"), -1, 98, 9, 5, 1.f);
Zone(TEXT("Second part: the rack railway, the whiteout"), 5.65f, 7.3f);
	Site(TEXT("RackR"), 1, 98, 9, 0, 1.f);
Zone(TEXT("Second part: the rack railway, the valley run"), 7.3f, 8.94f);
	Site(TEXT("RackR"), 1, 98, 9, 7, 1.f);
Zone(TEXT("Second part: the rack railway, the white ridge"), 11.6f, 14.3f);
	Site(TEXT("RackL"), -1, 98, 10, 0, 1.f);
	Site(TEXT("RackR"), 1, 98, 10, 5, 1.f);
Zone(TEXT("Second part: the rack railway, the high ridge"), 19.7f, 22.4f);
	Site(TEXT("RackL"), -1, 98, 10, 3, 1.f);
	Site(TEXT("RackR"), 1, 98, 10, 8, 1.f);

Zone(TEXT("Second part: groomers, the top"), 0.26f, 1.19f);
	Site(TEXT("GroomR"), 1, 86, 7, 1, 1.f);
Zone(TEXT("Second part: groomers, the half pipe"), 1.19f, 2.43f);
	Site(TEXT("GroomR"), 1, 86, 7, 4, 1.f);
Zone(TEXT("Second part: groomers, Black diamond"), 4.02f, 5.65f);
	Site(TEXT("GroomL"), -1, 86, 7, 2, 1.f);
	Site(TEXT("GroomR"), 1, 86, 7, 5, 1.f);
Zone(TEXT("Second part: groomers, the whiteout"), 5.65f, 7.3f);
	Site(TEXT("GroomL"), -1, 86, 7, 3, 1.f);
Zone(TEXT("Second part: groomers, the valley run"), 7.3f, 8.94f);
	Site(TEXT("GroomR"), 1, 86, 7, 6, 1.f);
Zone(TEXT("Second part: groomers, the resort"), 14.3f, 17.f);
	Site(TEXT("GroomR"), 1, 86, 8, 3, 1.f);
Zone(TEXT("Second part: groomers, the last lifts"), 22.4f, 99.f);
	Site(TEXT("GroomR"), 1, 86, 8, 5, 1.f);

Zone(TEXT("Second part: the cable car and the aurora"), 0.f, 99.f);
	Far(TEXT("SM_CableCar"), 630, 650, 60, 32, 1.f, 1.f, 0.f, 1, -26.f, 0.f);
	Far(TEXT("SM_Aurora"), 1600, 2100, 90, 20, 1.f, 1.f, 0.15f, -1, -650.f, 8.f);
	Far(TEXT("SM_Aurora"), 1600, 2100, 90, 65, 1.f, 1.f, 0.15f, 1, -650.f, 8.f);
