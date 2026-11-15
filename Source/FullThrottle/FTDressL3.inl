// 3.2 dressing table, level 3: the Volcano (Mount Kaldera). The words (Set, Zone, Scatter, Site, Far) are explained at
// the top of FTTrackDress.cpp; the meshes are made by Content/Python/ft_dress_volcano.py. Distances across are metres
// from the road's centre line; districts are in kilometres along the road and follow the level's own scenes.
//
// The lie of the land: ash and basalt, level to 90 m either side, then climbing to 9 m (left) and 12 m (right) at
// 300 m, where the land ends. Two lava channels run the whole way: 38 to 52 m out on the left, 46 to 61 m on the
// right (their middles: 45 and 53.5). The level's own props keep their places: rock spires and boulders from the
// barrier out to 137 m, vents, dead trees, a far crag every so often at 240 to 420 m.
// The dressing: a verge of ash banks, fences and pipe (17 to 23 m, all of it low and dull, nothing lit), works over
// the lava channels, the survey's camp and the drill sites on the slopes past the spires (150 to 290 m), cliffs and
// columnar basalt that close in where the scenes say so, and a horizon that changes district by district.
//
// The road spends a third of this level in the air or upside down (eleven lava jumps, three loops, three corkscrews),
// and Scatter and Site rules are left out of those stretches. So the rock and the big works on the slopes are written
// as Far rules with a small or negative drop instead: those stay through everything, and the drop puts the piece on
// the land at that distance (the land is 0.75 m below the road's ground level out to 90 m, then rises 4.6 cm a metre
// on the left and 6 cm a metre on the right; a piece stands 15 cm into it). Such a rule comes as a pair, left (-1)
// and right (1), and keeps to a narrow band so the one drop fits the whole of it.
//
// The finale's road runs on for 16 km, so it is laid out as three districts that come round twice: the works, the
// burning slopes, the ash fields.
//
// SECOND PASS (15 Nov 2026). What it added, and the rules it found:
//  - The mine (the level's shot): a crusher, its conveyor and a spoil tip as one set on the flat beyond the lava (112 and
//    118 m), the mine's railway with slag trains down the right-hand side (66 m), pump houses on the channels' banks.
//  - The ground: ash blown into pale crescents between the road and the lava (the ground is black: this is what gives
//    the verge a floor); rafts of crust and bright chevrons of flow on both lava channels, which light runs along
//    (ft_dress_volcano.animate), so the channels read as flowing; basalt stepping stones in the lava lake under every
//    jump (one zone per jump, from the track's own flags: a Far rule with drop -0.6 stands a piece's z = 0 at the lava).
//  - Beside a banked bend the land is cut down to the road's low edge and banks back up over 25 m: whatever stands 17 to
//    40 m out there floats over the cutting. So the verge of the six districts that have banked bends is written in
//    zones of its own that leave the bends out, and the bends get vent stacks footed 12 m down instead (they show
//    against the sky from down in the bend).
//  - The eruption (ft_world.py, the lead's): its cone stands 485 m to the RIGHT of the road at 3.85 km and is watched
//    from 2.4 km to 3.9 km. Nothing tall or bright of this pass stands on the right between those: no vent stacks, no
//    mine, no trains, no rafts on the right-hand channel; and the first pass's survey camp on the right (a 34 m lit
//    tower at 3.8 km) now starts at 3.95 km.
//  - Zones written BEFORE the districts place the flat pieces and the stacks by Far rules first, so those cast no shadow.
//  - The left-hand channel takes SM_LavaRaftsL (the table turns a left-hand piece round; the twin is drawn turned round,
//    so both channels flow the way the road goes).

Set(TEXT("Fence"), { { TEXT("SM_GuardFence"), 0, 0, 0, 0, 1 } });
Set(TEXT("Pipe"), { { TEXT("SM_PipeRun"), 0, 0, 0, 0, 1 } });
Set(TEXT("SurveyCamp"), {    // the survey's camp round its watch tower
	{ TEXT("SM_TowerSurvey"),         0,    0, 0,    0, 1 },
	{ TEXT("SM_HutSurvey"),          15,   -7, 0,    0, 1 },
	{ TEXT("SM_HutSurvey2"),        -16,   -5, 0,    0, 1 },
	{ TEXT("SM_PoleMast"),            9,   12, 0,    0, 1 },
	{ TEXT("SM_Helipad"),           -26,   14, 0,    0, 1 },
	{ TEXT("SM_CableReels"),         24,  -15, 0,   20, 1 },
	{ TEXT("SM_HutSurvey"),          30,    8, 0,   90, 1 },
});
Set(TEXT("ChannelWorks"), {    // works on a lava channel: the anchor is the channel's middle
	{ TEXT("SM_Gantry"),              0,    0, 0,    0, 1 },
	{ TEXT("SM_SluiceGate"),         27,    0, 0,    0, 1 },
	{ TEXT("SM_PolePipeBridge"),    -31,    0, 0,    0, 1 },
	{ TEXT("SM_CoolingFins"),         6,   24, 0,    0, 1 },
	{ TEXT("SM_HutSurvey"),         -13,   25, 0,    0, 1 },
});
Set(TEXT("DrillSite"), {    // an abandoned geothermal drill and what was left round it
	{ TEXT("SM_DrillRig"),            0,    0, 0,    0, 1 },
	{ TEXT("SM_Hauler"),             17,   -9, 0,   25, 1 },
	{ TEXT("SM_CableReels"),        -13,   -9, 0,    0, 1 },
	{ TEXT("SM_AshDune"),             5,   17, 0,    0, 1 },
	{ TEXT("SM_HutSurvey2"),        -20,    6, 0,    0, 1 },
	{ TEXT("SM_CoolingFins"),        26,   10, 0,   90, 1 },
});
Set(TEXT("Ruin"), {    // the old observatory
	{ TEXT("SM_Observatory"),         0,    0, 0,    0, 1 },
	{ TEXT("SM_PoleMast"),           15,    4, 0,    0, 1 },
	{ TEXT("SM_AshDune2"),          -22,   -6, 0,   15, 0.8f },
	{ TEXT("SM_RockRubble"),         11,  -13, 0,    0, 1.4f },
	{ TEXT("SM_HutSurvey2"),         24,   -4, 0,   20, 1 },
});
Set(TEXT("AshField"), {    // dunes of ash with a hauler left in them
	{ TEXT("SM_AshDune"),             0,    0, 0,    0, 1 },
	{ TEXT("SM_Hauler"),             15,    7, 0,  -30, 1 },
	{ TEXT("SM_AshDune2"),          -28,   10, 0,   15, 0.8f },
	{ TEXT("SM_CableReels"),          6,  -12, 0,    0, 1 },
});
Set(TEXT("Stacks"), {    // heat-stained stacks
	{ TEXT("SM_RockStack"),           0,    0, 0,    0, 1 },
	{ TEXT("SM_RockStack2Z"),        13,    9, 0,    0, 1 },
	{ TEXT("SM_RockStack"),         -11,   13, 0,   70, 0.8f },
	{ TEXT("SM_RockRubble"),          5,  -11, 0,    0, 1.3f },
});
Set(TEXT("Rail"), { { TEXT("SM_RailSpur"), 0, 0, 0, 0, 1 } });    // the mine's railway: one length a slot
Set(TEXT("Mine"), {    // the crusher, the conveyor climbing away from it, the tip under the conveyor's head, a hauler
	{ TEXT("SM_Crusher"),             0,    0, 0,    0, 1 },
	{ TEXT("SM_Conveyor"),           36,    4, 0,    0, 1 },
	{ TEXT("SM_AshTip"),             76,    2, 0,    0, 1 },
	{ TEXT("SM_Hauler"),            -20,   -8, 0,   30, 1 },
});

// ---- SECOND PASS, before the districts: the lava lakes, the land under the set pieces, the banked bends, the channels.
// Stepping stones in the lava lake under each of the 21 jumps (the lake is the track's own; it lies 0.6 m over the
// dipped land out to 90 m either side). Two bands a side.
Zone(TEXT("The lava lake at 0.68 km"), 0.672f, 0.738f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 1.40 km"), 1.387f, 1.488f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 2.38 km"), 2.372f, 2.438f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 3.40 km"), 3.397f, 3.468f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 3.64 km"), 3.632f, 3.703f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 4.39 km"), 4.387f, 4.503f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 5.63 km"), 5.622f, 5.688f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 6.55 km"), 6.542f, 6.643f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 7.46 km"), 7.457f, 7.528f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 7.70 km"), 7.692f, 7.758f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 14.30 km"), 14.297f, 14.368f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 15.31 km"), 15.302f, 15.368f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 16.31 km"), 16.302f, 16.368f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 17.31 km"), 17.302f, 17.368f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 18.31 km"), 18.302f, 18.368f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 19.31 km"), 19.302f, 19.373f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 20.32 km"), 20.307f, 20.373f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 21.32 km"), 21.307f, 21.373f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 22.32 km"), 22.307f, 22.373f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 23.32 km"), 23.307f, 23.378f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

Zone(TEXT("The lava lake at 24.32 km"), 24.307f, 24.378f);
	Far(TEXT("SM_LavaStones"), 34, 40, 1, 0, 1.f, 1.f, 0.2f, 0, -0.6f, 180.f);
	Far(TEXT("SM_LavaStones"), 66, 74, 1, 0, 0.9f, 1.f, 0.2f, 0, -0.6f, 180.f);

// Under the corkscrews and the loops the camera looks at the land, and Scatter and Site are left out there: vent
// stacks along the set piece, ash crescents on the flat, rafts on the channels, all as Far rules with the land's own
// drop. The caldera's loop looks at the eruption's cone (ahead and to the right): nothing of these on its right.
Zone(TEXT("Under the first corkscrew"), 1.95f, 2.12f);
	Far(TEXT("SM_VentStacks"), 30, 33, 2, 0, 1.f, 1.f, 0.f, -1, 0.83f, 0.f);
	Far(TEXT("SM_AshRipples"), 26, 30, 1, 0, 0.9f, 1.f, 0.f, 0, 0.84f, 3.f);
	Far(TEXT("SM_LavaRaftsL"), 45, 45, 2, 0, 1.f, 1.f, 0.f, -1, 0.81f, 0.f);
	Far(TEXT("SM_VentStacks"), 30, 33, 2, 1, 1.f, 1.f, 0.f, 1, 0.83f, 0.f);
	Far(TEXT("SM_LavaRafts"), 53.5f, 53.5f, 2, 1, 1.f, 1.f, 0.f, 1, 0.8f, 0.f);

Zone(TEXT("Under the caldera's loop"), 2.565f, 2.85f);
	Far(TEXT("SM_VentStacks"), 30, 33, 2, 0, 1.f, 1.f, 0.f, -1, 0.83f, 0.f);
	Far(TEXT("SM_AshRipples"), 26, 30, 1, 0, 0.9f, 1.f, 0.f, -1, 0.84f, 3.f);
	Far(TEXT("SM_LavaRaftsL"), 45, 45, 2, 0, 1.f, 1.f, 0.f, -1, 0.81f, 0.f);

Zone(TEXT("Under the second corkscrew"), 4.895f, 5.07f);
	Far(TEXT("SM_VentStacks"), 30, 33, 2, 0, 1.f, 1.f, 0.f, -1, 0.83f, 0.f);
	Far(TEXT("SM_AshRipples"), 26, 30, 1, 0, 0.9f, 1.f, 0.f, 0, 0.84f, 3.f);
	Far(TEXT("SM_LavaRaftsL"), 45, 45, 2, 0, 1.f, 1.f, 0.f, -1, 0.81f, 0.f);
	Far(TEXT("SM_VentStacks"), 30, 33, 2, 1, 1.f, 1.f, 0.f, 1, 0.83f, 0.f);
	Far(TEXT("SM_LavaRafts"), 53.5f, 53.5f, 2, 1, 1.f, 1.f, 0.f, 1, 0.8f, 0.f);

Zone(TEXT("Under the magma run's loop"), 5.815f, 6.1f);
	Far(TEXT("SM_VentStacks"), 30, 33, 2, 0, 1.f, 1.f, 0.f, -1, 0.83f, 0.f);
	Far(TEXT("SM_AshRipples"), 26, 30, 1, 0, 0.9f, 1.f, 0.f, 0, 0.84f, 3.f);
	Far(TEXT("SM_LavaRaftsL"), 45, 45, 2, 0, 1.f, 1.f, 0.f, -1, 0.81f, 0.f);
	Far(TEXT("SM_VentStacks"), 30, 33, 2, 1, 1.f, 1.f, 0.f, 1, 0.83f, 0.f);
	Far(TEXT("SM_LavaRafts"), 53.5f, 53.5f, 2, 1, 1.f, 1.f, 0.f, 1, 0.8f, 0.f);

Zone(TEXT("Under the third corkscrew"), 7.035f, 7.21f);
	Far(TEXT("SM_VentStacks"), 30, 33, 2, 0, 1.f, 1.f, 0.f, -1, 0.83f, 0.f);
	Far(TEXT("SM_AshRipples"), 26, 30, 1, 0, 0.9f, 1.f, 0.f, 0, 0.84f, 3.f);
	Far(TEXT("SM_LavaRaftsL"), 45, 45, 2, 0, 1.f, 1.f, 0.f, -1, 0.81f, 0.f);
	Far(TEXT("SM_VentStacks"), 30, 33, 2, 1, 1.f, 1.f, 0.f, 1, 0.83f, 0.f);
	Far(TEXT("SM_LavaRafts"), 53.5f, 53.5f, 2, 1, 1.f, 1.f, 0.f, 1, 0.8f, 0.f);

Zone(TEXT("Under the pass's loop"), 7.885f, 8.17f);
	Far(TEXT("SM_VentStacks"), 30, 33, 2, 0, 1.f, 1.f, 0.f, -1, 0.83f, 0.f);
	Far(TEXT("SM_AshRipples"), 26, 30, 1, 0, 0.9f, 1.f, 0.f, 0, 0.84f, 3.f);
	Far(TEXT("SM_LavaRaftsL"), 45, 45, 2, 0, 1.f, 1.f, 0.f, -1, 0.81f, 0.f);
	Far(TEXT("SM_VentStacks"), 30, 33, 2, 1, 1.f, 1.f, 0.f, 1, 0.83f, 0.f);
	Far(TEXT("SM_LavaRafts"), 53.5f, 53.5f, 2, 1, 1.f, 1.f, 0.f, 1, 0.8f, 0.f);

// Vent stacks down the banked bends (the track's own bank, 15 Nov): a group every other slot a side, left on even
// slots and right on odd ones (a group a slot on both sides read as a picket of posts). Left only where the eruption's
// cone is watched over the right-hand side.
Zone(TEXT("Vent stacks: the bends at 0.90 km"), 0.9f, 1.24f);
	Scatter(TEXT("SM_VentStacks"), 30, 33, 2, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("SM_VentStacks"), 30, 33, 2, 1, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("Vent stacks: the bends at 1.67 km"), 1.67f, 1.885f);
	Scatter(TEXT("SM_VentStacks"), 30, 33, 2, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("SM_VentStacks"), 30, 33, 2, 1, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("Vent stacks: the bends at 2.95 km"), 2.95f, 3.3f);
	Scatter(TEXT("SM_VentStacks"), 30, 33, 2, 0, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("Vent stacks: the bends at 3.85 km"), 3.85f, 4.08f);
	Scatter(TEXT("SM_VentStacks"), 30, 33, 2, 0, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("Vent stacks: the bends at 5.22 km"), 5.22f, 5.525f);
	Scatter(TEXT("SM_VentStacks"), 30, 33, 2, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("SM_VentStacks"), 30, 33, 2, 1, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("Vent stacks: the bends at 6.84 km"), 6.84f, 7.07f);
	Scatter(TEXT("SM_VentStacks"), 30, 33, 2, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("SM_VentStacks"), 30, 33, 2, 1, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("Vent stacks: the bends at 7.25 km"), 7.255f, 7.36f);
	Scatter(TEXT("SM_VentStacks"), 30, 33, 2, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("SM_VentStacks"), 30, 33, 2, 1, 1.f, 1.f, 0.f, 1, 0.f);

// Rafts of crust and chevrons of flow on the lava channels, every other slot: the left-hand channel the whole way,
// the right-hand one except where the eruption is watched over it.
Zone(TEXT("The left-hand channel, flowing"), 0.f, 99.f);
	Scatter(TEXT("SM_LavaRaftsL"), 45, 45, 2, 1, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("The right-hand channel, flowing (from 0 km)"), 0.f, 2.4f);
	Scatter(TEXT("SM_LavaRafts"), 53.5f, 53.5f, 2, 0, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("The right-hand channel, flowing (from 3.9 km)"), 3.9f, 99.f);
	Scatter(TEXT("SM_LavaRafts"), 53.5f, 53.5f, 2, 0, 1.f, 1.f, 0.f, 1, 0.f);

// ---- 0 to 1.17 km: THE ASH ROAD. Everything under ash: banks along the verge, dunes on the slopes with haulers and
// roofs showing out of them, instrument masts, the stained stacks behind, terraces under the edge. The rim stands
// far off; a young cone smokes nearer.
// Second pass: spoil tips out on the flat, ash blown into crescents between the road and the lava.
Zone(TEXT("The ash road"), 0.f, 1.17f);
	Site(TEXT("AshField"), 0, 165, 3, 0, 0.95f, 1.f, 0.f);
	Site(TEXT("Stacks"), 0, 205, 4, 1, 0.9f, 1.f, 0.f);
	Far(TEXT("SM_AshDune"), 150, 164, 2, 0, 0.6f, 1.f, 0.3f, -1, -2.31f, 40.f);
	Far(TEXT("SM_AshDune"), 150, 164, 2, 0, 0.6f, 1.f, 0.3f, 1, -3.27f, 40.f);
	Far(TEXT("SM_AshDune2"), 244, 258, 2, 1, 0.7f, 1.3f, 0.2f, -1, -6.61f, 40.f);
	Far(TEXT("SM_AshDune2"), 244, 258, 2, 1, 0.7f, 1.3f, 0.2f, 1, -8.91f, 40.f);
	Scatter(TEXT("SM_CragTerrace"), 245, 270, 3, 2, 0.8f, 1.f, 0.f, 0, 4.f);
	Scatter(TEXT("SM_PoleMast"), 150, 262, 4, 2, 0.7f, 1.2f, 0.2f, 0, 0.f);
	Far(TEXT("SM_RidgeCaldera"), 560, 860, 5, 1, 1.f, 1.f, 0.3f, 0, 25.f, 12.f);
	Far(TEXT("SM_PeakCinder"), 720, 1200, 9, 4, 1.f, 1.f, 0.25f, 0, 25.f, 180.f);
	Far(TEXT("SM_PeakCone2AD"), 380, 620, 7, 2, 0.9f, 1.f, 0.25f, 0, 25.f, 180.f);
	Scatter(TEXT("SM_AshTip"), 96, 112, 4, 2, 0.9f, 1.f, 0.15f, 0, 180.f);

Zone(TEXT("The ash road, the verge"), 0.f, 0.89f);
	Site(TEXT("Fence"), 0, 18.5f, 1, 0, 0.7f, 1.f, 0.f);
	Scatter(TEXT("SM_AshBank"), 20.5f, 22.5f, 1, 0, 0.6f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_AshBank2"), 20.5f, 22.5f, 2, 1, 0.5f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_CableReels"), 20.2f, 22, 5, 2, 0.8f, 1.f, 0.f, 0, 20.f);
	Scatter(TEXT("SM_AshRipples"), 26, 30, 1, 0, 0.8f, 1.f, 0.f, 0, 3.f);

// ---- 1.17 to 2.5 km: LAVA JUMPS. The channels are worked: a gantry, a sluice and a pipe bridge over the lava every
// 90 m on each side, cooling fins beyond, a pipe along the right-hand verge. Lava falls on the far cliffs.
// Second pass: the mine. Its railway runs down the right-hand side behind the lava (66 m), a crusher, its conveyor and a tip stand on the left (112 m), pump houses on the channels' banks.
Zone(TEXT("Lava jumps"), 1.17f, 2.5f);
	Site(TEXT("ChannelWorks"), -1, 45, 4, 0, 1.f, 1.f, 0.f);
	Site(TEXT("ChannelWorks"), 1, 53.5f, 4, 2, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_PolePipeBridge"), 45, 45, 4, 2, 0.9f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("SM_PolePipeBridge"), 53.5f, 53.5f, 4, 0, 0.9f, 1.f, 0.f, 1, 0.f);
	Far(TEXT("SM_CoolingFins"), 150, 162, 3, 0, 0.8f, 1.2f, 0.1f, -1, -2.27f, 0.f);
	Far(TEXT("SM_CoolingFins"), 150, 162, 3, 0, 0.8f, 1.2f, 0.1f, 1, -3.21f, 0.f);
	Far(TEXT("SM_RockStack"), 228, 240, 3, 1, 0.8f, 1.2f, 0.25f, -1, -5.83f, 180.f);
	Far(TEXT("SM_RockStack"), 228, 240, 3, 1, 0.8f, 1.2f, 0.25f, 1, -7.89f, 180.f);
	Scatter(TEXT("SM_HutSurvey"), 180, 200, 5, 3, 0.9f, 1.f, 0.f, 0, 0.f);
	Far(TEXT("SM_RidgeFalls"), 390, 640, 6, 2, 1.f, 1.f, 0.2f, 0, 25.f, 8.f);
	Far(TEXT("SM_RidgeRim2"), 640, 920, 7, 4, 1.f, 1.f, 0.3f, 0, 25.f, 12.f);
	Far(TEXT("SM_PeakCone2AD"), 420, 700, 9, 6, 1.f, 1.2f, 0.25f, 0, 25.f, 180.f);
	Site(TEXT("Rail"), 1, 66, 1, 0, 1.f);
	Site(TEXT("Mine"), -1, 112, 6, 2, 1.f);
	Scatter(TEXT("SM_PumpHouse"), 56, 58, 8, 1, 0.9f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("SM_PumpHouse"), 39.5f, 40.5f, 8, 5, 0.9f, 1.f, 0.f, 1, 0.f);

// The right-hand mine and the slag trains stop at 2.2 km: from 2.4 km the eruption is watched over that side.
Zone(TEXT("Lava jumps, the mine on the right"), 1.17f, 2.2f);
	Site(TEXT("Mine"), 1, 118, 6, 5, 1.f);
	Scatter(TEXT("SM_OreTrain"), 66, 66, 7, 3, 0.9f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("Lava jumps, the verge (1)"), 1.25f, 1.66f);
	Site(TEXT("Pipe"), 1, 19.5f, 1, 0, 1.f, 1.f, 0.f);
	Site(TEXT("Fence"), -1, 18.5f, 2, 0, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_AshRipples"), 26, 30, 2, 0, 0.8f, 1.f, 0.f, 0, 3.f);

Zone(TEXT("Lava jumps, the verge (2)"), 1.89f, 2.5f);
	Site(TEXT("Pipe"), 1, 19.5f, 1, 0, 1.f, 1.f, 0.f);
	Site(TEXT("Fence"), -1, 18.5f, 2, 0, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_AshRipples"), 26, 30, 2, 0, 0.8f, 1.f, 0.f, 0, 3.f);

// ---- 2.5 to 3.76 km: THE CALDERA (a loop on the crater's rim, then lava jumps). Inside the crater: stained stacks,
// then three tiers of cliff and columnar basalt up both slopes with lava falls between, and the rim's inner wall
// standing near and unbroken all the way round. All of it stays through the loop.
Zone(TEXT("The caldera"), 2.5f, 3.76f);
	Far(TEXT("SM_RockStack"), 100, 108, 3, 1, 0.8f, 1.f, 0.2f, -1, 0.11f, 180.f);
	Far(TEXT("SM_RockStack"), 100, 108, 3, 1, 0.8f, 1.f, 0.2f, 1, -0.09f, 180.f);
	Far(TEXT("SM_CragColumns"), 116, 124, 2, 0, 0.95f, 1.f, 0.15f, -1, -0.62f, 8.f);
	Far(TEXT("SM_CragColumns"), 116, 124, 2, 0, 0.95f, 1.f, 0.15f, 1, -1.05f, 8.f);
	Far(TEXT("SM_CragCliff"), 118, 126, 2, 1, 0.95f, 1.f, 0.15f, -1, -0.71f, 6.f);
	Far(TEXT("SM_CragCliff"), 118, 126, 2, 1, 0.95f, 1.f, 0.15f, 1, -1.17f, 6.f);
	Far(TEXT("SM_CragColumns2"), 152, 162, 2, 1, 0.85f, 1.3f, 0.15f, -1, -2.31f, 10.f);
	Far(TEXT("SM_CragColumns2"), 152, 162, 2, 1, 0.85f, 1.3f, 0.15f, 1, -3.27f, 10.f);
	Far(TEXT("SM_CragCliff2"), 170, 180, 2, 0, 0.85f, 1.3f, 0.15f, -1, -3.14f, 8.f);
	Far(TEXT("SM_CragCliff2"), 170, 180, 2, 0, 0.85f, 1.3f, 0.15f, 1, -4.35f, 8.f);
	Far(TEXT("SM_CragFallA"), 206, 216, 3, 1, 0.95f, 1.1f, 0.f, -1, -4.78f, 6.f);
	Far(TEXT("SM_CragFallA"), 206, 216, 3, 1, 0.95f, 1.1f, 0.f, 1, -6.51f, 6.f);
	Far(TEXT("SM_CragColumns2"), 240, 250, 1, 0, 0.7f, 1.3f, 0.2f, -1, -6.34f, 10.f);
	Far(TEXT("SM_CragColumns2"), 240, 250, 1, 0, 0.7f, 1.3f, 0.2f, 1, -8.55f, 10.f);
	Far(TEXT("SM_RidgeCaldera"), 345, 520, 2, 0, 1.f, 1.2f, 0.3f, 0, 25.f, 12.f);
	Far(TEXT("SM_RidgeRim2"), 720, 1040, 4, 1, 1.f, 1.5f, 0.3f, 0, 25.f, 12.f);

Zone(TEXT("The caldera, the verge (1)"), 2.5f, 2.94f);
	Scatter(TEXT("SM_RockRubble"), 21.2f, 23, 1, 0, 0.6f, 1.f, 0.15f, 0, 12.f);

Zone(TEXT("The caldera, the verge (2)"), 3.31f, 3.76f);
	Scatter(TEXT("SM_RockRubble"), 21.2f, 23, 1, 0, 0.6f, 1.f, 0.15f, 0, 12.f);

// ---- 3.76 to 5.18 km: FIRE AND BRIMSTONE (the long jump and a corkscrew). Where the survey worked: its camp round a
// watch tower on each slope, more towers higher up (they stay through the jump), instrument masts, stakes along the
// verge; a ridge of pylons and smoking cones on the skyline.
Zone(TEXT("Fire and brimstone"), 3.76f, 5.18f);
	Site(TEXT("SurveyCamp"), -1, 182, 6, 3, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_PoleMast"), 150, 262, 2, 0, 0.6f, 1.2f, 0.2f, 0, 0.f);
	Far(TEXT("SM_TowerSurvey"), 238, 250, 5, 2, 0.9f, 1.f, 0.f, -1, -6.29f, 0.f);
	Scatter(TEXT("SM_HutSurvey"), 152, 200, 4, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Far(TEXT("SM_AshDune"), 206, 220, 3, 2, 0.6f, 1.f, 0.3f, -1, -4.87f, 40.f);
	Far(TEXT("SM_AshDune"), 206, 220, 3, 2, 0.6f, 1.f, 0.3f, 1, -6.63f, 40.f);
	Far(TEXT("SM_RidgePylons"), 370, 600, 4, 0, 1.f, 1.f, 0.15f, 0, 25.f, 6.f);
	Far(TEXT("SM_PeakCinder"), 620, 1000, 5, 2, 1.f, 1.f, 0.25f, 0, 25.f, 180.f);
	Far(TEXT("SM_RidgeCaldera"), 900, 1300, 7, 3, 1.f, 1.4f, 0.3f, 0, 25.f, 12.f);

// The right-hand camp and its tower, clear of the eruption's cone (see the head of this file).
Zone(TEXT("Fire and brimstone, the right-hand camp"), 3.95f, 5.18f);
	Site(TEXT("SurveyCamp"), 1, 178, 6, 0, 1.f, 1.f, 0.f);
	Far(TEXT("SM_TowerSurvey"), 238, 250, 5, 2, 0.9f, 1.f, 0.f, 1, -8.49f, 0.f);

Zone(TEXT("Fire and brimstone, the verge (1)"), 3.76f, 3.84f);
	Site(TEXT("Fence"), 0, 18.5f, 1, 0, 0.6f, 1.f, 0.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 3, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_AshRipples"), 26, 30, 2, 1, 0.8f, 1.f, 0.f, 0, 3.f);

Zone(TEXT("Fire and brimstone, the verge (2)"), 4.09f, 5.18f);
	Site(TEXT("Fence"), 0, 18.5f, 1, 0, 0.6f, 1.f, 0.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 3, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_AshRipples"), 26, 30, 2, 1, 0.8f, 1.f, 0.f, 0, 3.f);

// ---- 5.18 to 6.73 km: THE MAGMA RUN (a jump, a loop, the big jump). The drill sites: leaning derricks, haulers left
// in the ash, cooling fins, a pipe down the left verge, works on the channels, and the lava falling nearer.
// Second pass: a mine on the right, pump houses on the left channel.
Zone(TEXT("The magma run"), 5.18f, 6.73f);
	Site(TEXT("DrillSite"), 0, 172, 4, 0, 1.f, 1.f, 0.f);
	Site(TEXT("ChannelWorks"), -1, 45, 7, 3, 1.f, 1.f, 0.f);
	Site(TEXT("ChannelWorks"), 1, 53.5f, 7, 6, 1.f, 1.f, 0.f);
	Far(TEXT("SM_DrillRig"), 230, 240, 4, 2, 0.9f, 1.f, 0.f, -1, -5.88f, 30.f);
	Far(TEXT("SM_DrillRig"), 230, 240, 4, 2, 0.9f, 1.f, 0.f, 1, -7.95f, 30.f);
	Scatter(TEXT("SM_Hauler"), 150, 222, 3, 1, 0.7f, 1.f, 0.f, 0, 180.f);
	Far(TEXT("SM_CoolingFins"), 200, 212, 4, 2, 0.8f, 1.2f, 0.1f, -1, -4.55f, 0.f);
	Far(TEXT("SM_CoolingFins"), 200, 212, 4, 2, 0.8f, 1.2f, 0.1f, 1, -6.21f, 0.f);
	Site(TEXT("Stacks"), 0, 262, 3, 2, 0.9f, 1.f, 0.f);
	Far(TEXT("SM_CragFallA"), 254, 264, 5, 1, 0.95f, 1.2f, 0.f, -1, -6.98f, 6.f);
	Far(TEXT("SM_CragFallA"), 254, 264, 5, 1, 0.95f, 1.2f, 0.f, 1, -9.39f, 6.f);
	Far(TEXT("SM_RidgeFalls"), 370, 620, 4, 0, 1.f, 1.f, 0.2f, 0, 25.f, 8.f);
	Far(TEXT("SM_PeakCinder"), 820, 1300, 6, 3, 1.f, 1.1f, 0.25f, 0, 25.f, 180.f);
	Far(TEXT("SM_RidgePylons"), 640, 900, 9, 5, 1.f, 1.2f, 0.15f, 0, 25.f, 6.f);
	Site(TEXT("Mine"), 1, 118, 8, 4, 1.f);
	Scatter(TEXT("SM_PumpHouse"), 56, 58, 8, 3, 0.9f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("The magma run, the verge"), 5.53f, 6.73f);
	Site(TEXT("Pipe"), -1, 19.5f, 1, 0, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_AshBank"), 20.5f, 22.5f, 2, 0, 0.6f, 1.f, 0.2f, 1, 2.f);
	Scatter(TEXT("SM_AshRipples"), 26, 30, 2, 0, 0.8f, 1.f, 0.f, 0, 3.f);

// ---- 6.73 to 8.34 km: OBSIDIAN PASS (a banked wall, a corkscrew, two jumps, a loop). A canyon of black glass: columns
// right behind the lava on both sides, taller ones over them, cliffs high on the slopes, the rim hard overhead; the
// ruined observatory looks down on it.
Zone(TEXT("Obsidian pass"), 6.73f, 8.34f);
	Far(TEXT("SM_CragColumns"), 61, 68, 1, 0, 0.8f, 1.2f, 0.15f, -1, 0.78f, 8.f);
	Far(TEXT("SM_CragColumns"), 70, 77, 1, 0, 0.8f, 1.2f, 0.15f, 1, 0.77f, 8.f);
	Far(TEXT("SM_CragColumns2"), 96, 106, 2, 1, 0.8f, 1.2f, 0.15f, -1, 0.25f, 10.f);
	Far(TEXT("SM_CragColumns2"), 96, 106, 2, 1, 0.8f, 1.2f, 0.15f, 1, 0.09f, 10.f);
	Far(TEXT("SM_CragColumns2"), 128, 138, 1, 0, 0.8f, 1.5f, 0.15f, -1, -1.22f, 10.f);
	Far(TEXT("SM_CragColumns2"), 128, 138, 1, 0, 0.8f, 1.5f, 0.15f, 1, -1.83f, 10.f);
	Far(TEXT("SM_CragCliff2"), 236, 248, 1, 0, 0.8f, 1.4f, 0.15f, -1, -6.2f, 8.f);
	Far(TEXT("SM_CragCliff2"), 236, 248, 1, 0, 0.8f, 1.4f, 0.15f, 1, -8.37f, 8.f);
	Site(TEXT("Ruin"), 1, 205, 9, 4, 1.f, 1.f, 0.f);
	Site(TEXT("Ruin"), -1, 210, 13, 9, 1.f, 1.f, 0.f);
	Far(TEXT("SM_RidgeCaldera"), 335, 460, 2, 0, 1.f, 1.3f, 0.3f, 0, 25.f, 12.f);
	Far(TEXT("SM_RidgeRim2"), 345, 470, 2, 1, 1.f, 1.3f, 0.3f, 0, 25.f, 12.f);

Zone(TEXT("Obsidian pass, the verge (1)"), 6.73f, 6.83f);
	Scatter(TEXT("SM_RockRubble"), 21.2f, 23, 1, 0, 0.7f, 1.f, 0.15f, 0, 12.f);

Zone(TEXT("Obsidian pass, the verge (2)"), 7.08f, 7.25f);
	Scatter(TEXT("SM_RockRubble"), 21.2f, 23, 1, 0, 0.7f, 1.f, 0.15f, 0, 12.f);

Zone(TEXT("Obsidian pass, the verge (3)"), 7.37f, 8.34f);
	Scatter(TEXT("SM_RockRubble"), 21.2f, 23, 1, 0, 0.7f, 1.f, 0.15f, 0, 12.f);

// ---- 8.34 km to the end: THE FINALE, Engine 13's road, in three districts that come round twice.
// 8.34 to 11.1 km, THE WORKS: every channel worked on both sides, the camp and its towers on the slopes; pylons
// along the near skyline.
// Second pass: the mine on both sides, its railway and slag trains on the right, pump houses on the channels.
Zone(TEXT("Engine 13's road"), 8.34f, 11.1f);
	Site(TEXT("Fence"), 0, 18.5f, 2, 0, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_AshBank"), 20.5f, 22.5f, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 5, 3, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("ChannelWorks"), -1, 45, 4, 1, 1.f, 1.f, 0.f);
	Site(TEXT("ChannelWorks"), 1, 53.5f, 4, 3, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_PolePipeBridge"), 45, 45, 4, 3, 0.9f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("SM_PolePipeBridge"), 53.5f, 53.5f, 4, 1, 0.9f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("SurveyCamp"), 0, 180, 7, 6, 1.f, 1.f, 0.f);
	Far(TEXT("SM_TowerSurvey"), 238, 250, 9, 2, 0.9f, 1.f, 0.f, -1, -6.29f, 0.f);
	Far(TEXT("SM_TowerSurvey"), 238, 250, 9, 2, 0.9f, 1.f, 0.f, 1, -8.49f, 0.f);
	Far(TEXT("SM_CoolingFins"), 150, 162, 5, 0, 0.8f, 1.2f, 0.1f, -1, -2.27f, 0.f);
	Far(TEXT("SM_CoolingFins"), 150, 162, 5, 0, 0.8f, 1.2f, 0.1f, 1, -3.21f, 0.f);
	Scatter(TEXT("SM_HutSurvey"), 152, 200, 6, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_PoleMast"), 150, 262, 3, 0, 0.6f, 1.2f, 0.2f, 0, 0.f);
	Far(TEXT("SM_RidgePylons"), 370, 600, 4, 1, 1.f, 1.f, 0.15f, 0, 25.f, 6.f);
	Far(TEXT("SM_PeakCinder"), 720, 1200, 6, 1, 1.f, 1.f, 0.25f, 0, 25.f, 180.f);
	Far(TEXT("SM_RidgeRim2"), 900, 1300, 8, 2, 1.f, 1.5f, 0.3f, 0, 25.f, 12.f);
	Site(TEXT("Rail"), 1, 66, 1, 0, 1.f);
	Scatter(TEXT("SM_OreTrain"), 66, 66, 7, 3, 0.9f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("Mine"), -1, 112, 6, 2, 1.f);
	Site(TEXT("Mine"), 1, 118, 6, 5, 1.f);
	Scatter(TEXT("SM_PumpHouse"), 56, 58, 8, 1, 0.9f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("SM_PumpHouse"), 39.5f, 40.5f, 8, 5, 0.9f, 1.f, 0.f, 1, 0.f);
	Scatter(TEXT("SM_AshRipples"), 26, 30, 2, 0, 0.8f, 1.f, 0.f, 0, 3.f);

// 11.1 to 13.9 km, THE BURNING SLOPES: cliff and columns up both slopes, lava falls between and behind.
Zone(TEXT("The burning slopes"), 11.1f, 13.9f);
	Scatter(TEXT("SM_RockRubble"), 21.2f, 23, 2, 0, 0.7f, 1.f, 0.15f, 0, 12.f);
	Scatter(TEXT("SM_AshBank2"), 20.5f, 22.5f, 3, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Far(TEXT("SM_CragColumns"), 116, 124, 2, 0, 0.9f, 1.f, 0.15f, -1, -0.62f, 8.f);
	Far(TEXT("SM_CragColumns"), 116, 124, 2, 0, 0.9f, 1.f, 0.15f, 1, -1.05f, 8.f);
	Far(TEXT("SM_CragCliff"), 118, 126, 2, 1, 0.9f, 1.f, 0.15f, -1, -0.71f, 6.f);
	Far(TEXT("SM_CragCliff"), 118, 126, 2, 1, 0.9f, 1.f, 0.15f, 1, -1.17f, 6.f);
	Far(TEXT("SM_CragColumns2"), 152, 162, 3, 1, 0.85f, 1.3f, 0.15f, -1, -2.31f, 10.f);
	Far(TEXT("SM_CragColumns2"), 152, 162, 3, 1, 0.85f, 1.3f, 0.15f, 1, -3.27f, 10.f);
	Far(TEXT("SM_CragFallA"), 206, 216, 3, 2, 0.95f, 1.1f, 0.f, -1, -4.78f, 6.f);
	Far(TEXT("SM_CragFallA"), 206, 216, 3, 2, 0.95f, 1.1f, 0.f, 1, -6.51f, 6.f);
	Site(TEXT("Stacks"), 0, 262, 4, 3, 0.9f, 1.f, 0.f);
	Far(TEXT("SM_CragCliff2"), 240, 250, 2, 0, 0.7f, 1.3f, 0.15f, -1, -6.34f, 8.f);
	Far(TEXT("SM_CragCliff2"), 240, 250, 2, 0, 0.7f, 1.3f, 0.15f, 1, -8.55f, 8.f);
	Far(TEXT("SM_RidgeFalls"), 380, 640, 4, 3, 1.f, 1.f, 0.2f, 0, 25.f, 8.f);
	Far(TEXT("SM_RidgeCaldera"), 440, 720, 4, 0, 1.f, 1.1f, 0.3f, 0, 25.f, 12.f);
	Far(TEXT("SM_PeakCone2AD"), 700, 1000, 9, 5, 1.f, 1.2f, 0.25f, 0, 25.f, 180.f);

// 13.9 to 16.7 km, THE ASH FIELDS: dunes, haulers and drill sites under ash; cones smoking all along the skyline.
// Second pass: spoil tips, ash crescents down both verges.
Zone(TEXT("The ash fields"), 13.9f, 16.7f);
	Site(TEXT("Fence"), 0, 18.5f, 1, 0, 0.6f, 1.f, 0.f);
	Scatter(TEXT("SM_AshBank"), 20.5f, 22.5f, 1, 0, 0.5f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_CableReels"), 20.2f, 22, 6, 2, 0.8f, 1.f, 0.f, 0, 20.f);
	Site(TEXT("AshField"), 0, 166, 3, 2, 0.9f, 1.f, 0.f);
	Site(TEXT("DrillSite"), 0, 226, 7, 2, 1.f, 1.f, 0.f);
	Far(TEXT("SM_AshDune"), 206, 220, 2, 1, 0.6f, 1.f, 0.3f, -1, -4.87f, 40.f);
	Far(TEXT("SM_AshDune"), 206, 220, 2, 1, 0.6f, 1.f, 0.3f, 1, -6.63f, 40.f);
	Far(TEXT("SM_AshDune2"), 244, 258, 3, 0, 0.7f, 1.3f, 0.2f, -1, -6.61f, 40.f);
	Far(TEXT("SM_AshDune2"), 244, 258, 3, 0, 0.7f, 1.3f, 0.2f, 1, -8.91f, 40.f);
	Scatter(TEXT("SM_Hauler"), 150, 222, 4, 1, 0.7f, 1.f, 0.f, 0, 180.f);
	Scatter(TEXT("SM_CragTerrace"), 245, 270, 4, 2, 0.8f, 1.f, 0.f, 0, 4.f);
	Far(TEXT("SM_PeakCinder"), 620, 1000, 4, 0, 1.f, 1.f, 0.25f, 0, 25.f, 180.f);
	Far(TEXT("SM_PeakCone2AD"), 400, 660, 5, 2, 0.9f, 1.1f, 0.25f, 0, 25.f, 180.f);
	Far(TEXT("SM_RidgeCaldera"), 900, 1300, 6, 3, 1.f, 1.4f, 0.3f, 0, 25.f, 12.f);
	Scatter(TEXT("SM_AshTip"), 96, 112, 4, 1, 0.9f, 1.f, 0.15f, 0, 180.f);
	Scatter(TEXT("SM_AshRipples"), 26, 30, 1, 0, 0.8f, 1.f, 0.f, 0, 3.f);

// 16.7 to 19.5 km: the works again, under far lava falls.
// Second pass: as Engine 13's road.
Zone(TEXT("The far works"), 16.7f, 19.5f);
	Site(TEXT("Fence"), 0, 18.5f, 2, 1, 0.8f, 1.f, 0.f);
	Scatter(TEXT("SM_AshBank"), 20.5f, 22.5f, 2, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_PoleMarker"), 18, 19.5f, 5, 3, 0.8f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("ChannelWorks"), -1, 45, 4, 1, 1.f, 1.f, 0.f);
	Site(TEXT("ChannelWorks"), 1, 53.5f, 4, 3, 1.f, 1.f, 0.f);
	Scatter(TEXT("SM_PolePipeBridge"), 45, 45, 4, 3, 0.9f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("SM_PolePipeBridge"), 53.5f, 53.5f, 4, 1, 0.9f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("SurveyCamp"), 0, 180, 7, 5, 1.f, 1.f, 0.f);
	Far(TEXT("SM_TowerSurvey"), 238, 250, 9, 3, 0.9f, 1.f, 0.f, -1, -6.29f, 0.f);
	Far(TEXT("SM_TowerSurvey"), 238, 250, 9, 3, 0.9f, 1.f, 0.f, 1, -8.49f, 0.f);
	Far(TEXT("SM_CoolingFins"), 150, 162, 5, 1, 0.8f, 1.2f, 0.1f, -1, -2.27f, 0.f);
	Far(TEXT("SM_CoolingFins"), 150, 162, 5, 1, 0.8f, 1.2f, 0.1f, 1, -3.21f, 0.f);
	Scatter(TEXT("SM_HutSurvey"), 152, 200, 6, 1, 0.8f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_PoleMast"), 150, 262, 3, 0, 0.6f, 1.2f, 0.2f, 0, 0.f);
	Far(TEXT("SM_RidgePylons"), 420, 680, 5, 3, 1.f, 1.2f, 0.15f, 0, 25.f, 6.f);
	Far(TEXT("SM_RidgeFalls"), 700, 1000, 6, 0, 1.f, 1.4f, 0.2f, 0, 25.f, 8.f);
	Far(TEXT("SM_PeakCone2AD"), 380, 600, 8, 5, 0.9f, 1.f, 0.25f, 0, 25.f, 180.f);
	Site(TEXT("Rail"), 1, 66, 1, 0, 1.f);
	Scatter(TEXT("SM_OreTrain"), 66, 66, 7, 3, 0.9f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("Mine"), -1, 112, 6, 2, 1.f);
	Site(TEXT("Mine"), 1, 118, 6, 5, 1.f);
	Scatter(TEXT("SM_PumpHouse"), 56, 58, 8, 1, 0.9f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("SM_PumpHouse"), 39.5f, 40.5f, 8, 5, 0.9f, 1.f, 0.f, 1, 0.f);
	Scatter(TEXT("SM_AshRipples"), 26, 30, 2, 1, 0.8f, 1.f, 0.f, 0, 3.f);

// 19.5 to 22.3 km: the burning slopes again, with the rim's wall near.
Zone(TEXT("The inner wall"), 19.5f, 22.3f);
	Scatter(TEXT("SM_RockRubble"), 21.2f, 23, 2, 1, 0.7f, 1.f, 0.15f, 0, 12.f);
	Scatter(TEXT("SM_AshBank2"), 20.5f, 22.5f, 3, 1, 0.6f, 1.f, 0.2f, 0, 2.f);
	Far(TEXT("SM_CragColumns"), 116, 124, 2, 1, 0.9f, 1.f, 0.15f, -1, -0.62f, 8.f);
	Far(TEXT("SM_CragColumns"), 116, 124, 2, 1, 0.9f, 1.f, 0.15f, 1, -1.05f, 8.f);
	Far(TEXT("SM_CragCliff"), 118, 126, 2, 0, 0.9f, 1.f, 0.15f, -1, -0.71f, 6.f);
	Far(TEXT("SM_CragCliff"), 118, 126, 2, 0, 0.9f, 1.f, 0.15f, 1, -1.17f, 6.f);
	Far(TEXT("SM_CragColumns2"), 152, 162, 3, 1, 0.85f, 1.3f, 0.15f, -1, -2.31f, 10.f);
	Far(TEXT("SM_CragColumns2"), 152, 162, 3, 1, 0.85f, 1.3f, 0.15f, 1, -3.27f, 10.f);
	Far(TEXT("SM_CragFallA"), 206, 216, 3, 2, 0.95f, 1.1f, 0.f, -1, -4.78f, 6.f);
	Far(TEXT("SM_CragFallA"), 206, 216, 3, 2, 0.95f, 1.1f, 0.f, 1, -6.51f, 6.f);
	Site(TEXT("Stacks"), 0, 262, 4, 3, 0.9f, 1.f, 0.f);
	Far(TEXT("SM_CragCliff2"), 240, 250, 2, 1, 0.7f, 1.3f, 0.15f, -1, -6.34f, 8.f);
	Far(TEXT("SM_CragCliff2"), 240, 250, 2, 1, 0.7f, 1.3f, 0.15f, 1, -8.55f, 8.f);
	Far(TEXT("SM_RidgeCaldera"), 345, 520, 3, 0, 1.f, 1.2f, 0.3f, 0, 25.f, 12.f);
	Far(TEXT("SM_RidgeRim2"), 560, 860, 4, 2, 1.f, 1.3f, 0.3f, 0, 25.f, 12.f);
	Far(TEXT("SM_PeakCinder"), 900, 1300, 9, 4, 1.f, 1.2f, 0.25f, 0, 25.f, 180.f);

// 22.3 km to the end of the road: the ash fields again, a young cone close by.
// Second pass: spoil tips, ash crescents.
Zone(TEXT("The last ash"), 22.3f, 99.f);
	Site(TEXT("Fence"), 0, 18.5f, 1, 0, 0.6f, 1.f, 0.f);
	Scatter(TEXT("SM_AshBank"), 20.5f, 22.5f, 1, 0, 0.5f, 1.f, 0.2f, 0, 2.f);
	Scatter(TEXT("SM_CableReels"), 20.2f, 22, 6, 2, 0.8f, 1.f, 0.f, 0, 20.f);
	Site(TEXT("AshField"), 0, 166, 3, 1, 0.9f, 1.f, 0.f);
	Site(TEXT("DrillSite"), 0, 226, 7, 3, 1.f, 1.f, 0.f);
	Far(TEXT("SM_AshDune"), 206, 220, 2, 1, 0.6f, 1.f, 0.3f, -1, -4.87f, 40.f);
	Far(TEXT("SM_AshDune"), 206, 220, 2, 1, 0.6f, 1.f, 0.3f, 1, -6.63f, 40.f);
	Far(TEXT("SM_AshDune2"), 244, 258, 3, 1, 0.7f, 1.3f, 0.2f, -1, -6.61f, 40.f);
	Far(TEXT("SM_AshDune2"), 244, 258, 3, 1, 0.7f, 1.3f, 0.2f, 1, -8.91f, 40.f);
	Scatter(TEXT("SM_Hauler"), 150, 222, 4, 1, 0.7f, 1.f, 0.f, 0, 180.f);
	Scatter(TEXT("SM_CragTerrace"), 245, 270, 4, 2, 0.8f, 1.f, 0.f, 0, 4.f);
	Far(TEXT("SM_PeakCone2AD"), 380, 560, 4, 1, 0.9f, 1.2f, 0.25f, 0, 25.f, 180.f);
	Far(TEXT("SM_RidgePylons"), 640, 900, 7, 3, 1.f, 1.2f, 0.15f, 0, 25.f, 6.f);
	Far(TEXT("SM_RidgeRim2"), 800, 1200, 6, 0, 1.f, 1.5f, 0.3f, 0, 25.f, 12.f);
	Scatter(TEXT("SM_AshTip"), 96, 112, 4, 3, 0.9f, 1.f, 0.15f, 0, 180.f);
	Scatter(TEXT("SM_AshRipples"), 26, 30, 1, 0, 0.8f, 1.f, 0.f, 0, 3.f);
