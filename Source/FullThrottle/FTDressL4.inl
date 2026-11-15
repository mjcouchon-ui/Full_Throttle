// 3.2 dressing table, level 4: Theme Park (Coaster Cove). The words (Set, Zone, Scatter, Site, Far, SiteAfloat) are explained at the
// top of FTTrackDress.cpp; the meshes are made by Content/Python/ft_dress_park.py. Distances across are metres from the road's
// centre line; districts are in kilometres along the road.
//
// The lie of the land: a sandy promenade to 42 m either side, lawns beyond falling 3 m to the land's edge at 300 m. The
// level's own props stay where they were: bulb lamps on the barrier, trees 27-119 m, booths 30-44 m, balloons 24-64 m,
// big tops on the left and carousels on the right at 46-137 m, a Ferris wheel (right, 219 m), drop towers (left, 238 m) and
// lattice at 238-384 m. The dressing puts a park round them, one LAND after another, each with its own landmark ride, its own
// horizon and its name in lights where it begins:
//
//   0.00  FRONT GATE     the ticket gates and their forecourts (STARRY the mascot, fountains), the first stalls, the big wheel
//   1.15  THE MIDWAY     an unbroken double row of game stalls and food stands each side; chair swings behind; big tops beyond
//   2.22  PIRATE COVE    swinging ships, skull rocks with lit eyes, barrels and palms; a fort and its ship's masts on the skyline
//   3.21  THE LAGOON     the coaster's valleys: lakes under the rails, lit paths across them, other coasters end on, either side
//   5.84  ROCKET LAND    rocket rides, saucers, planets on poles; a moon rocket and its gantry on the skyline
//   7.24  THE FUNHOUSE   funhouses, chair swings, the mascot; then the back lot: sheds, spare cars, generators, plain fence
//   8.42  THE LAST RIDE  the finale's promenade: every land comes round again, a block at a time, under all the horizons
//
// Three rules of this table:
//   - within 24 m of the centre line only unlit things under 2 m stand (fences, queue rails, benches, barrels). Round a
//     loop that means 66 m from the loop's ground line: the loop comes down 33 m to the right of where it went up;
//   - sets (several pieces composed round a landmark) are used only where the road runs level. Where it climbs, drops or
//     jumps, single pieces are used instead: each stands on its own bit of ground. Those stretches have districts of
//     their own below (the big jump, the lift hill, splashdown, the funhouse jump);
//   - the middle distance and the horizon are Far rules, which stay through the corkscrews, loops and jumps (near things
//     are left out of those by the track). A Far rule's last-but-one number is its drop: for a piece that is to stand ON
//     the land it is the land's own depth below the road at that distance (0.9 m at 20 m, 1.1 at 60, 1.45 at 100, 1.9 at
//     150, 2.3 at 200, 2.8 at 260), a little more so that nothing floats.
//
// THE LAGOON'S WATER is one level. The land under the coaster falls 26 m and follows the rails' hills, so each coaster
// section has a hollow at its middle, all nine at the same height (the corkscrewing section's valley is flat). The lakes
// (SM_Lake, SM_Lake2) are stood by SiteAfloat at 3.65 m above the level's floor, which is 15 cm under those hollows' ground:
// the water then lies 1.15 m deep in every hollow and the humps between rise out of it. The two jump lakes lie the same way
// in the valleys under the jumps. These heights come from the track's own numbers (floor z -1935, hollows at -1485 - 70,
// the jump valleys' beds at -1014 and -255): if the track's heights ever change, FTBED's "floor z" in the log says so.

// ---------------------------------------------------------------- the verge (low, unlit: nearer than 24 m)
Set(TEXT("FenceRun"), { { TEXT("SM_ParkFence"), -11.3f, 0, 0, 0, 1 }, { TEXT("SM_ParkFence"), 11.3f, 0, 0, 0, 1 } });
Set(TEXT("QueueLine"), { { TEXT("SM_QueueRails"), -10, 0.6f, 0, 0, 1 }, { TEXT("SM_PlazaSeats"), 13, 0, 0, 0, 1 } });
Set(TEXT("SeatsRun"), { { TEXT("SM_PlazaSeats"), -13, 0, 0, 0, 1 }, { TEXT("SM_ParkFence"), 9, 0, 0, 0, 1 } });
Set(TEXT("CoveVerge"), { { TEXT("SM_CoveBarrels"), -12, 0.5f, 0, 0, 1 }, { TEXT("SM_ParkFence"), 9, 0, 0, 0, 1 } });

// ---------------------------------------------------------------- the lands' own corners
Set(TEXT("StallRow"), {                                   // anchor 31 m: a row of stalls, fronts to the road
	{ TEXT("SM_GameStall"),  -15,    0,    0,  0, 1 },
	{ TEXT("SM_GameStall2"), -7.4f,  0,    0,  0, 1 },
	{ TEXT("SM_FoodStand"),   0.2f,  0.3f, 0,  0, 1 },
	{ TEXT("SM_GameStall3"),  7.6f,  0,    0,  0, 1 },
	{ TEXT("SM_CandyCart"),  14.2f, -2,    0, 20, 1 },
});
Set(TEXT("StallRow2"), {
	{ TEXT("SM_FoodStand"),  -16,    0.3f, 0,  0, 1 },
	{ TEXT("SM_GameStall3"), -8.6f,  0,    0,  0, 1 },
	{ TEXT("SM_HighStriker"), -2.6f, 1,    0,  0, 1 },
	{ TEXT("SM_GameStall"),   3.4f,  0,    0,  0, 1 },
	{ TEXT("SM_GameStall2"), 11,     0,    0,  0, 1 },
});
Set(TEXT("Gateway"), {                                    // anchor 46 m: a ticket gate, its forecourt between it and the road
	{ TEXT("SM_TicketGate"),   0,   0,     0,  0, 1 },
	{ TEXT("SM_Mascot"),       0, -14,     0,  0, 1 },
	{ TEXT("SM_Fountain"),    18, -12,     0,  0, 1 },
	{ TEXT("SM_QueueRails"), -19, -12,     0,  0, 1 },
	{ TEXT("SM_PlazaSeats"),  18, -19.5f,  0,  0, 1 },
	{ TEXT("SM_GameStall"),  -22,  -1,     0,  0, 1 },
	{ TEXT("SM_GameStall3"),  22,  -1,     0,  0, 1 },
});
Set(TEXT("WheelPlaza"), {                                 // anchor 170 m: the big wheel over its own little fair
	{ TEXT("SM_BigWheel"),     0,   0, 0, 0, 1 },
	{ TEXT("SM_GameStall2"), -24, -13, 0, 0, 1 },
	{ TEXT("SM_FoodStand"),  -15, -13, 0, 0, 1 },
	{ TEXT("SM_GameStall"),   15, -13, 0, 0, 1 },
	{ TEXT("SM_Fountain"),     0, -22, 0, 0, 1.3f },
	{ TEXT("SM_Bandstand"),   27, -15, 0, 0, 1 },
});
Set(TEXT("PirateDock"), {                                 // anchor 40 m: the swinging ship on its dock
	{ TEXT("SM_PirateShip"),    0,  0, 0,   0, 1 },
	{ TEXT("SM_CoveBarrels"), -16, -4, 0,   0, 1 },
	{ TEXT("SM_CoveBarrels"),  17, -3, 0, 180, 1 },
	{ TEXT("/Game/Maps/Bayshore/Props/SM_Palm"),  -14,  5, 0,  40, 1 },
	{ TEXT("/Game/Maps/Bayshore/Props/SM_Palm2"),  15,  6, 0, 200, 1 },
	{ TEXT("/Game/Maps/Bayshore/Props/SM_Palm"),   21, -5, 0, 130, 0.85f },
});
Set(TEXT("SkullCove"), {                                  // anchor 64 m: the skull rock in its palms
	{ TEXT("SM_SkullRock"),     0,   0, 0,   0, 1 },
	{ TEXT("/Game/Maps/Bayshore/Props/SM_Palm"),  -15,  -4, 0,  70, 1 },
	{ TEXT("/Game/Maps/Bayshore/Props/SM_Palm2"),  14,  -6, 0, 250, 1 },
	{ TEXT("/Game/Maps/Bayshore/Props/SM_Palm"),   17,   7, 0, 160, 1.1f },
	{ TEXT("SM_CoveBarrels"),  -4, -11, 0,  20, 1 },
	{ TEXT("SM_Pond"),        -17, -14, 0,   0, 0.8f },
});
Set(TEXT("RocketPad"), {                                  // anchor 44 m: the rocket ride between its planets
	{ TEXT("SM_JetRide"),       0,   0, 0,   0, 1 },
	{ TEXT("SM_PlanetPosts"), -15,  -6, 0,   0, 1 },
	{ TEXT("SM_PlanetPosts"),  15,  -5, 0, 180, 1 },
	{ TEXT("SM_CandyCart"),    11, -11, 0, -20, 1 },
});
Set(TEXT("SaucerPark"), {                                 // anchor 38 m
	{ TEXT("SM_Saucer"),        0,  0, 0, 0, 1 },
	{ TEXT("SM_PlanetPosts"),  13, -1, 0, 0, 1 },
	{ TEXT("SM_PlazaSeats"),  -14, -3, 0, 0, 1 },
	{ TEXT("SM_GameStall3"),  -14,  6, 0, 0, 1 },
});
Set(TEXT("FunCourt"), {                                   // anchor 40 m: the funhouse, the mascot, a chair swing behind
	{ TEXT("SM_Funhouse"),      0,  0, 0,   0, 1 },
	{ TEXT("SM_Mascot"),       12, -5, 0, -10, 1 },
	{ TEXT("SM_GameStall2"),  -12, -3, 0,   0, 1 },
	{ TEXT("SM_SwingRide"),   -30, 10, 0,   0, 1 },
});
Set(TEXT("BackLot"), {                                    // anchor 58 m: two sheds back to back, their yard fenced
	{ TEXT("SM_Maintenance"),   0,  0, 0,   0, 1 },
	{ TEXT("SM_Maintenance"),  26,  7, 0, 180, 1 },
	{ TEXT("SM_ParkFence"),    -4, -8, 0,   0, 1 },
	{ TEXT("SM_ParkFence"),    18, -8, 0,   0, 1 },
	{ TEXT("SM_CoveBarrels"), -17, -3, 0,  90, 1 },
});
Set(TEXT("PathCross"), { { TEXT("SM_PathLights"), 0, 0, 0, 90, 1 } });       // a lit path running away from the road
Set(TEXT("HillRibR"), { { TEXT("SM_CoasterHill"), 0, 0, 0, -90, 1 } });      // a coaster end on, its front to the oncoming car
Set(TEXT("HillRibL"), { { TEXT("SM_CoasterHill"), 0, 0, 0, 90, 1 } });
// the lagoon's lakes: placed afloat, so "up" is metres above the level's floor (see THE LAGOON'S WATER above); "along"
// moves each lake from its slot to the middle of its hollow
Set(TEXT("LakeA_m20"), { { TEXT("SM_Lake"),  -20.6f, 0, 3.65f, 0, 1 } });
Set(TEXT("LakeA_m9"),  { { TEXT("SM_Lake"),   -9.0f, 0, 3.65f, 0, 1 } });
Set(TEXT("LakeA_p20"), { { TEXT("SM_Lake"),   20.6f, 0, 3.65f, 0, 1 } });
Set(TEXT("LakeB_p5"),  { { TEXT("SM_Lake2"),   5.5f, 0, 3.65f, 0, 1 } });
Set(TEXT("LakeB_m9"),  { { TEXT("SM_Lake2"),  -8.2f, 0, 3.65f, 0, 1 } });
Set(TEXT("LakeB_p20"), { { TEXT("SM_Lake2"),  20.6f, 0, 3.65f, 0, 1 } });
Set(TEXT("LakeB_p12"), { { TEXT("SM_Lake2"),  12.3f, 0, 3.65f, 0, 1 } });
Set(TEXT("HelixRibR"), { { TEXT("SM_CoasterHill"), 20, 0, 3.2f, -90, 1 } });
Set(TEXT("HelixRibL"), { { TEXT("SM_CoasterHill"), 20, 0, 3.2f,  90, 1 } });
Set(TEXT("JumpLake1"), { { TEXT("SM_Lake"),   6.8f, 0, 10.56f, 0, 1 } });
Set(TEXT("JumpLake2"), { { TEXT("SM_Lake2"), 19.7f, 0, 18.15f, 0, 1 } });

// ---- 0 to 1.15 km: THE FRONT GATE. A ticket gate and its forecourt every 540 m on each side (270 m apart, turn about),
// stall rows between them, ponds and carts in the gaps; bandstands, fountains and chair swings behind; the big wheel on
// the left over its own fair; the park's coasters across the back.
Zone(TEXT("Front Gate"), 0.f, 1.15f);
	Site(TEXT("FenceRun"), -1, 19, 2, 0, 1.f);
	Site(TEXT("QueueLine"), -1, 19, 2, 1, 0.9f);
	Site(TEXT("SeatsRun"), 1, 19, 2, 0, 0.9f);
	Site(TEXT("FenceRun"), 1, 19, 2, 1, 1.f);
	Site(TEXT("Gateway"), -1, 46, 12, 8, 1.f);
	Site(TEXT("Gateway"), 1, 46, 12, 2, 1.f);
	Site(TEXT("StallRow"), -1, 31, 6, 5, 1.f);
	Site(TEXT("StallRow2"), 1, 31, 6, 5, 1.f);
	Site(TEXT("StallRow2"), -1, 31, 12, 2, 1.f);
	Site(TEXT("StallRow"), 1, 31, 12, 8, 1.f);
	Scatter(TEXT("SM_Pond"), 34, 44, 3, 0, 0.7f, 1.f, 0.1f, 0, 180.f);
	Scatter(TEXT("SM_CandyCart"), 27, 33, 3, 1, 0.6f, 1.f, 0.f, 0, 30.f);
	Scatter(TEXT("SM_Mascot"), 38, 43, 6, 1, 1.f, 1.f, 0.f, 0, 8.f);
	Far(TEXT("SM_Bandstand"), 54, 66, 4, 2, 0.9f, 1.f, 0.f, 0, 1.2f, 20.f);
	Far(TEXT("SM_Fountain"), 52, 68, 4, 0, 0.9f, 1.2f, 0.f, 0, 1.2f, 20.f);
	Far(TEXT("SM_SwingRide"), 84, 108, 6, 3, 1.f, 1.f, 0.f, 0, 1.55f, 20.f);
	Far(TEXT("SM_Maintenance"), 125, 140, 9, 4, 1.f, 1.f, 0.f, 1, 1.8f, 12.f);
	Site(TEXT("WheelPlaza"), -1, 170, 14, 6, 1.f);
	Far(TEXT("SM_CoasterHill"), 160, 230, 4, 1, 0.9f, 1.f, 0.1f, 1, 2.6f, 10.f);
	Far(TEXT("SM_CoasterHill"), 235, 285, 5, 3, 0.9f, 1.3f, 0.1f, -1, 3.1f, 10.f);
	Far(TEXT("SM_FarWheel"), 380, 460, 12, 5, 1.f, 1.f, 0.1f, -1, 25.f, 15.f);
	Far(TEXT("SM_FarBigTops"), 340, 420, 7, 2, 1.f, 1.6f, 0.15f, 0, 22.f, 12.f);
	Far(TEXT("SM_FarCoaster"), 520, 700, 9, 4, 1.f, 1.3f, 0.1f, 1, 25.f, 10.f);
	Far(TEXT("SM_FarChute"), 620, 820, 13, 9, 1.f, 1.f, 0.1f, 0, 25.f, 0.f);

// ---- 1.15 to 1.84 km: THE MIDWAY. Stalls without a gap on both sides, a second row behind them across an alley, high
// strikers, giant STARRYs and bandstands in the crowd, chair swings; three big tops fill the sky on both sides.
Zone(TEXT("The Midway"), 1.15f, 1.84f);
	Scatter(TEXT("SM_SignMidway"), 27.5f, 27.5f, 15, 4, 1.f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("QueueLine"), -1, 19, 2, 0, 0.9f);
	Site(TEXT("SeatsRun"), -1, 19, 2, 1, 0.9f);
	Site(TEXT("QueueLine"), 1, 19, 2, 1, 0.9f);
	Site(TEXT("SeatsRun"), 1, 19, 2, 0, 0.9f);
	Site(TEXT("StallRow"), -1, 31, 2, 0, 1.f);
	Site(TEXT("StallRow2"), -1, 31, 2, 1, 1.f);
	Site(TEXT("StallRow2"), 1, 31, 2, 0, 1.f);
	Site(TEXT("StallRow"), 1, 31, 2, 1, 1.f);
	Far(TEXT("SM_GameStall2"), 44, 47, 2, 0, 1.f, 1.f, 0.f, 0, 1.0f, 4.f);
	Far(TEXT("SM_FoodStand"), 44, 47, 2, 1, 1.f, 1.f, 0.f, 0, 1.0f, 4.f);
	Far(TEXT("SM_HighStriker"), 53, 58, 3, 1, 0.9f, 1.3f, 0.f, 0, 1.1f, 6.f);
	Far(TEXT("SM_Bandstand"), 62, 76, 4, 2, 0.9f, 1.f, 0.f, 0, 1.25f, 20.f);
	Far(TEXT("SM_Mascot"), 60, 72, 4, 0, 0.9f, 1.4f, 0.f, 0, 1.2f, 10.f);
	Far(TEXT("SM_SwingRide"), 86, 110, 4, 1, 1.f, 1.f, 0.f, 0, 1.55f, 20.f);
	Far(TEXT("/Game/Maps/ThemePark/Props/SM_Tent"), 110, 140, 3, 1, 0.8f, 1.1f, 0.15f, 1, 1.8f, 180.f);
	Far(TEXT("SM_CoasterHill"), 170, 250, 4, 2, 0.9f, 1.1f, 0.1f, 0, 2.75f, 10.f);
	Far(TEXT("SM_FarBigTops"), 340, 430, 5, 1, 1.f, 1.8f, 0.15f, 0, 22.f, 12.f);
	Far(TEXT("SM_FarChute"), 560, 760, 9, 3, 1.f, 1.1f, 0.1f, -1, 25.f, 0.f);
	Far(TEXT("SM_FarWheel"), 600, 800, 11, 6, 1.f, 1.3f, 0.1f, 1, 25.f, 15.f);
// the corkscrew on the Midway (slots 27-30): the track leaves near things out of it, so the rows are carried through
// it by Far rules, a stall at a time, in two staggered lines
Zone(TEXT("Midway corkscrew"), 1.22f, 1.36f);
	Far(TEXT("SM_GameStall"), 28.5f, 29.5f, 1, 0, 1.f, 1.f, 0.f, 0, 0.92f, 3.f);
	Far(TEXT("SM_GameStall3"), 36.5f, 37.5f, 1, 0, 0.9f, 1.f, 0.f, 0, 0.95f, 3.f);

// ---- 1.84 to 2.22 km: THE BIG JUMP. The road climbs and jumps a valley: single pieces only, and a lake in the valley
// under the flight. Nothing nearer than 40 m at the take-off and the landing (the track sees to the near ones; the
// Far rules here start at 46 m).
Zone(TEXT("The big jump"), 1.84f, 2.22f);
	Scatter(TEXT("SM_PlazaSeats"), 19, 19, 1, 0, 0.9f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_GameStall"), 30, 32, 2, 0, 0.9f, 1.f, 0.f, 0, 4.f);
	Scatter(TEXT("SM_GameStall3"), 30, 32, 2, 1, 0.9f, 1.f, 0.f, 0, 4.f);
	Far(TEXT("SM_GameStall2"), 46, 49, 2, 0, 1.f, 1.f, 0.f, 0, 1.02f, 4.f);
	Far(TEXT("SM_FoodStand"), 46, 49, 2, 1, 1.f, 1.f, 0.f, 0, 1.02f, 4.f);
	Far(TEXT("SM_Pond"), 58, 72, 2, 1, 0.8f, 1.f, 0.1f, 0, 1.2f, 180.f);
	Far(TEXT("SM_Bandstand"), 60, 74, 4, 0, 1.f, 1.f, 0.f, 0, 1.25f, 20.f);
	Far(TEXT("/Game/Maps/ThemePark/Props/SM_Tent"), 85, 135, 2, 0, 0.8f, 1.f, 0.15f, 0, 1.75f, 180.f);
	Far(TEXT("SM_SwingRide"), 90, 110, 5, 3, 1.f, 1.f, 0.f, 0, 1.55f, 20.f);
	Far(TEXT("SM_CoasterHill"), 170, 250, 3, 1, 0.9f, 1.1f, 0.1f, 0, 2.75f, 10.f);
	Far(TEXT("SM_FarBigTops"), 340, 430, 4, 0, 1.f, 1.8f, 0.15f, 0, 22.f, 12.f);
	Far(TEXT("SM_FarWheel"), 600, 800, 7, 3, 1.f, 1.3f, 0.1f, 1, 25.f, 15.f);
Zone(TEXT("Jump lake 1"), 2.0772f, 2.0852f);
	SiteAfloat(TEXT("JumpLake1"), 1, 0, 1, 0, 1.f);

// ---- 2.22 to 2.98 km: PIRATE COVE. Swinging ships on their docks, skull rocks opposite them, barrels on the verge,
// Bayshore's palms. Round the loop (near things are left out of it) ships, rocks and palms are stood by Far rules:
// on the right they keep beyond the loop's own way out, which lies 33 m to that side.
Zone(TEXT("Pirate Cove"), 2.22f, 2.98f);
	Scatter(TEXT("SM_SignPirate"), 27.5f, 27.5f, 16, 14, 1.f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("CoveVerge"), -1, 19, 2, 0, 0.9f);
	Site(TEXT("FenceRun"), -1, 19, 2, 1, 0.8f);
	Site(TEXT("CoveVerge"), 1, 19, 2, 1, 0.9f);
	Site(TEXT("FenceRun"), 1, 19, 2, 0, 0.8f);
	Site(TEXT("PirateDock"), -1, 40, 4, 1, 1.f);
	Site(TEXT("PirateDock"), 1, 40, 4, 3, 1.f);
	Site(TEXT("SkullCove"), 1, 64, 4, 1, 1.f);
	Site(TEXT("SkullCove"), -1, 64, 4, 3, 1.f);
	Scatter(TEXT("/Game/Maps/Bayshore/Props/SM_Palm"), 26, 60, 1, 0, 0.6f, 1.f, 0.2f, 0, 180.f);
	Scatter(TEXT("/Game/Maps/Bayshore/Props/SM_Palm2"), 28, 70, 2, 1, 0.6f, 1.1f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_CoveBarrels"), 26, 34, 2, 0, 0.6f, 1.f, 0.f, 0, 180.f);
	Far(TEXT("SM_Bandstand"), 110, 130, 5, 2, 0.8f, 1.f, 0.f, 0, 1.75f, 20.f);
	Far(TEXT("SM_CoasterHill"), 180, 260, 4, 0, 0.9f, 1.2f, 0.1f, 0, 2.85f, 10.f);
	Far(TEXT("SM_FarFort"), 360, 460, 8, 3, 1.f, 1.f, 0.1f, -1, 25.f, 10.f);
	Far(TEXT("SM_FarFort"), 520, 700, 9, 6, 1.f, 1.5f, 0.1f, 1, 25.f, 10.f);
	Far(TEXT("SM_FarCoaster"), 420, 560, 6, 2, 1.f, 1.f, 0.1f, 1, 25.f, 10.f);
	Far(TEXT("SM_FarWheel"), 600, 800, 10, 5, 1.f, 1.3f, 0.1f, -1, 25.f, 15.f);
Zone(TEXT("Cove loop"), 2.665f, 2.945f);
	Far(TEXT("SM_CoveBarrels"), 24, 30, 1, 0, 0.8f, 1.f, 0.f, -1, 0.92f, 180.f);
	Far(TEXT("SM_PirateShip"), 70, 76, 3, 1, 1.f, 1.f, 0.f, -1, 1.25f, 5.f);
	Far(TEXT("SM_SkullRock"), 84, 96, 3, 2, 1.f, 1.f, 0.f, -1, 1.4f, 20.f);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Palm"), 66, 110, 1, 0, 0.9f, 1.f, 0.2f, 0, 1.5f, 180.f);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Palm2"), 70, 120, 1, 0, 0.7f, 1.1f, 0.2f, 0, 1.6f, 180.f);
	Far(TEXT("SM_PirateShip"), 76, 84, 3, 2, 1.f, 1.f, 0.f, 1, 1.3f, 5.f);
	Far(TEXT("SM_SkullRock"), 95, 110, 3, 0, 1.f, 1.2f, 0.f, 1, 1.55f, 20.f);

// ---- 2.98 to 3.21 km: THE LIFT HILL. The road climbs 9 m to the coaster: STARRY lines the climb on both sides, a
// stall or a food stand behind each one, ponds and bandstands beyond.
Zone(TEXT("The lift hill"), 2.98f, 3.21f);
	Scatter(TEXT("SM_SignLagoon"), 27.5f, 27.5f, 5, 0, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_Mascot"), 27, 30, 1, 0, 0.9f, 1.f, 0.f, 0, 6.f);
	Scatter(TEXT("SM_GameStall2"), 36, 40, 2, 0, 0.9f, 1.f, 0.f, 0, 4.f);
	Scatter(TEXT("SM_FoodStand"), 36, 40, 2, 1, 0.9f, 1.f, 0.f, 0, 4.f);
	Far(TEXT("SM_Bandstand"), 52, 66, 2, 0, 0.9f, 1.f, 0.f, 0, 1.2f, 20.f);
	Far(TEXT("SM_Pond"), 50, 70, 2, 1, 0.8f, 1.f, 0.1f, 0, 1.25f, 180.f);
	Far(TEXT("SM_SwingRide"), 86, 110, 3, 1, 1.f, 1.f, 0.f, 0, 1.55f, 20.f);
	Far(TEXT("SM_CoasterHill"), 150, 250, 2, 0, 0.9f, 1.2f, 0.1f, 0, 2.75f, 10.f);
	Far(TEXT("SM_FarCoaster"), 360, 460, 3, 1, 1.f, 1.f, 0.1f, 0, 30.f, 10.f);
	Far(TEXT("SM_FarWheel"), 520, 640, 5, 2, 1.f, 1.2f, 0.1f, -1, 30.f, 15.f);

// ---- 3.21 to 5.84 km: THE LAGOON, under the coaster. The land is 26 m under the rails here (and comes up to meet them
// where two coaster sections join), so low things may stand right under the rails: lit paths running out across the
// valley from under them, ponds on the humps. The park's other coasters stand end on at 52-116 m, a loop nearest the
// rails; bandstands, big tops and carousels between; a big wheel and more coasters behind; coasters all round the sky.
// The corkscrewing section (4.14-4.50 km) gets no near things from the track, so its valley is dressed by the Far rules
// here and by the afloat sets in "Lagoon 4" below.
Zone(TEXT("The Lagoon"), 3.21f, 5.84f);
	Site(TEXT("PathCross"), -1, 24.5f, 2, 0, 1.f);
	Site(TEXT("PathCross"), 1, 24.5f, 2, 1, 1.f);
	Site(TEXT("HillRibR"), 1, 84, 3, 0, 1.f);
	Site(TEXT("HillRibL"), -1, 84, 3, 1, 1.f);
	Far(TEXT("SM_Bandstand"), 44, 58, 3, 2, 0.9f, 1.f, 0.f, 0, 1.1f, 20.f);
	Far(TEXT("/Game/Maps/ThemePark/Props/SM_Tent"), 60, 135, 2, 0, 0.9f, 1.1f, 0.15f, 1, 1.75f, 180.f);
	Far(TEXT("/Game/Maps/ThemePark/Props/SM_Carousel"), 55, 120, 4, 1, 1.f, 1.f, 0.f, -1, 1.6f, 180.f);
	Far(TEXT("SM_SwingRide"), 120, 140, 5, 3, 1.f, 1.2f, 0.f, 0, 1.8f, 20.f);
	Far(TEXT("SM_BigWheel"), 185, 200, 13, 6, 1.f, 1.f, 0.f, 1, 2.3f, 8.f);
	Far(TEXT("SM_CoasterHill"), 150, 260, 2, 1, 0.9f, 1.2f, 0.15f, 0, 2.85f, 14.f);
	Far(TEXT("SM_FarCoaster"), 340, 430, 5, 0, 1.f, 1.f, 0.1f, 0, 34.f, 10.f);
	Far(TEXT("SM_FarCoaster"), 560, 760, 7, 3, 1.f, 1.5f, 0.1f, 0, 34.f, 10.f);
	Far(TEXT("SM_FarWheel"), 480, 560, 11, 4, 1.f, 1.f, 0.1f, -1, 34.f, 15.f);
	Far(TEXT("SM_FarChute"), 620, 820, 13, 8, 1.f, 1.1f, 0.1f, 1, 34.f, 0.f);
// the ponds under the rails keep clear of the coaster's first and last 40 m, where the road is back on the land
Zone(TEXT("Lagoon, under the rails"), 3.25f, 5.80f);
	Scatter(TEXT("SM_Pond"), 6, 34, 2, 1, 0.8f, 1.f, 0.15f, -1, 180.f);
	Scatter(TEXT("SM_Pond"), 6, 34, 2, 0, 0.8f, 1.f, 0.15f, 1, 180.f);
// the lagoon's lakes, one in each section's hollow (a district of one slot each), five in a row along the corkscrew's valley
Zone(TEXT("Lagoon 1"), 3.3892f, 3.3972f); SiteAfloat(TEXT("LakeA_m20"), 1, 0, 1, 0, 1.f);
Zone(TEXT("Lagoon 2"), 3.7059f, 3.7139f); SiteAfloat(TEXT("LakeB_p5"), 1, 0, 1, 0, 1.f);
Zone(TEXT("Lagoon 3"), 4.0226f, 4.0306f); SiteAfloat(TEXT("LakeA_m9"), 1, 0, 1, 0, 1.f);
Zone(TEXT("Lagoon 4"), 4.19f, 4.40f);
	SiteAfloat(TEXT("LakeA_p20"), 1, 0, 2, 1, 1.f);
	SiteAfloat(TEXT("LakeB_p20"), 1, 0, 2, 0, 1.f);
	SiteAfloat(TEXT("HelixRibR"), 1, 84, 2, 1, 1.f);
	SiteAfloat(TEXT("HelixRibL"), -1, 84, 2, 0, 1.f);
Zone(TEXT("Lagoon 5"), 4.6108f, 4.6188f); SiteAfloat(TEXT("LakeB_m9"), 1, 0, 1, 0, 1.f);
Zone(TEXT("Lagoon 6"), 4.8822f, 4.8902f); SiteAfloat(TEXT("LakeA_p20"), 1, 0, 1, 0, 1.f);
Zone(TEXT("Lagoon 7"), 5.1989f, 5.2069f); SiteAfloat(TEXT("LakeB_p20"), 1, 0, 1, 0, 1.f);
Zone(TEXT("Lagoon 8"), 5.5156f, 5.5236f); SiteAfloat(TEXT("LakeA_m9"), 1, 0, 1, 0, 1.f);
Zone(TEXT("Lagoon 9"), 5.7419f, 5.7499f); SiteAfloat(TEXT("LakeB_p12"), 1, 0, 1, 0, 1.f);

// ---- 5.84 to 6.03 km: SPLASHDOWN. Off the rails and down 8 m: planets on poles line the drop, saucers behind.
Zone(TEXT("Splashdown"), 5.84f, 6.03f);
	Scatter(TEXT("SM_SignRocket"), 27.5f, 27.5f, 5, 0, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_PlanetPosts"), 28, 31, 1, 0, 0.9f, 1.f, 0.f, 0, 10.f);
	Scatter(TEXT("SM_GameStall3"), 36, 40, 2, 0, 0.9f, 1.f, 0.f, 0, 4.f);
	Scatter(TEXT("SM_Saucer"), 44, 52, 2, 1, 0.9f, 1.f, 0.f, 0, 30.f);
	Far(TEXT("SM_Pond"), 58, 72, 2, 0, 0.8f, 1.f, 0.1f, 0, 1.25f, 180.f);
	Far(TEXT("SM_JetRide"), 84, 104, 2, 1, 1.f, 1.f, 0.f, 0, 1.5f, 20.f);
	Far(TEXT("SM_CoasterHill"), 150, 250, 2, 0, 0.9f, 1.2f, 0.1f, 0, 2.75f, 10.f);
	Far(TEXT("SM_FarRocket"), 360, 460, 3, 1, 1.f, 1.f, 0.1f, -1, 30.f, 20.f);

// ---- 6.03 to 7.24 km: ROCKET LAND. A rocket ride or a saucer every 90 m on each side, planets on poles between; more
// rides and saucers behind, carried through the corkscrew and the loop; a moon rocket on its pad on the skyline.
Zone(TEXT("Rocket Land"), 6.03f, 7.24f);
	Site(TEXT("FenceRun"), -1, 19, 2, 0, 0.9f);
	Site(TEXT("SeatsRun"), -1, 19, 2, 1, 0.9f);
	Site(TEXT("FenceRun"), 1, 19, 2, 1, 0.9f);
	Site(TEXT("SeatsRun"), 1, 19, 2, 0, 0.9f);
	Site(TEXT("RocketPad"), -1, 44, 4, 2, 1.f);
	Site(TEXT("RocketPad"), 1, 44, 4, 0, 1.f);
	Site(TEXT("SaucerPark"), 1, 38, 4, 2, 1.f);
	Site(TEXT("SaucerPark"), -1, 38, 4, 0, 1.f);
	Scatter(TEXT("SM_PlanetPosts"), 28, 33, 2, 1, 0.8f, 1.f, 0.f, 0, 15.f);
	Scatter(TEXT("SM_GameStall3"), 34, 38, 4, 1, 0.8f, 1.f, 0.f, 0, 4.f);
	Scatter(TEXT("SM_CandyCart"), 34, 38, 4, 3, 0.7f, 1.f, 0.f, 0, 30.f);
	Far(TEXT("SM_PlanetPosts"), 66, 72, 3, 2, 0.9f, 1.3f, 0.f, 0, 1.2f, 15.f);
	Far(TEXT("SM_Saucer"), 70, 80, 3, 1, 0.9f, 1.2f, 0.f, 0, 1.3f, 30.f);
	Far(TEXT("SM_JetRide"), 80, 100, 3, 0, 1.f, 1.f, 0.f, 0, 1.45f, 20.f);
	Far(TEXT("SM_Maintenance"), 125, 140, 8, 5, 1.f, 1.f, 0.f, -1, 1.8f, 12.f);
	Far(TEXT("SM_CoasterHill"), 170, 250, 4, 1, 0.9f, 1.1f, 0.1f, 0, 2.75f, 10.f);
	Far(TEXT("SM_FarRocket"), 340, 420, 7, 3, 1.f, 1.f, 0.1f, 1, 25.f, 20.f);
	Far(TEXT("SM_FarRocket"), 520, 700, 9, 1, 1.f, 1.5f, 0.1f, -1, 25.f, 20.f);
	Far(TEXT("SM_FarChute"), 420, 520, 8, 6, 1.f, 1.f, 0.1f, -1, 25.f, 0.f);
	Far(TEXT("SM_FarCoaster"), 600, 800, 9, 5, 1.f, 1.4f, 0.1f, 1, 25.f, 10.f);
Zone(TEXT("Rocket corkscrew"), 6.465f, 6.61f);
	Far(TEXT("SM_PlanetPosts"), 28, 30, 1, 0, 0.9f, 1.f, 0.f, 0, 0.9f, 15.f);
	Far(TEXT("SM_PlanetPosts"), 46, 52, 2, 0, 0.9f, 1.3f, 0.f, 0, 1.05f, 15.f);
	Far(TEXT("SM_GameStall3"), 33, 36, 2, 0, 1.f, 1.f, 0.f, 0, 0.94f, 4.f);
	Far(TEXT("SM_Saucer"), 37, 42, 2, 1, 1.f, 1.f, 0.f, 0, 0.97f, 30.f);
Zone(TEXT("Rocket loop"), 6.963f, 7.243f);
	Far(TEXT("SM_PlanetPosts"), 66, 72, 2, 1, 0.9f, 1.3f, 0.f, -1, 1.2f, 15.f);
	Far(TEXT("SM_Saucer"), 68, 78, 2, 0, 1.f, 1.2f, 0.f, -1, 1.3f, 30.f);
	Far(TEXT("SM_PlanetPosts"), 68, 76, 1, 0, 0.9f, 1.2f, 0.f, 1, 1.25f, 15.f);

// ---- 7.24 to 7.56 km: THE FUNHOUSE. A funhouse court every 135 m on each side (the mascot at its door, a chair swing
// behind), STARRY between them, bigger funhouses behind.
Zone(TEXT("The Funhouse"), 7.24f, 7.56f);
	Scatter(TEXT("SM_SignFun"), 27.5f, 27.5f, 7, 0, 1.f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("SeatsRun"), -1, 19, 2, 0, 0.9f);
	Site(TEXT("QueueLine"), -1, 19, 2, 1, 0.9f);
	Site(TEXT("SeatsRun"), 1, 19, 2, 1, 0.9f);
	Site(TEXT("QueueLine"), 1, 19, 2, 0, 0.9f);
	Site(TEXT("FunCourt"), -1, 40, 3, 1, 1.f);
	Site(TEXT("FunCourt"), 1, 40, 3, 2, 1.f);
	Scatter(TEXT("SM_Mascot"), 28, 32, 3, 0, 0.9f, 1.f, 0.f, 0, 8.f);
	Far(TEXT("SM_Funhouse"), 62, 74, 3, 0, 1.f, 1.2f, 0.f, 0, 1.25f, 8.f);
	Far(TEXT("SM_SwingRide"), 90, 110, 3, 1, 1.f, 1.f, 0.f, 0, 1.55f, 20.f);
	Far(TEXT("/Game/Maps/ThemePark/Props/SM_Tent"), 110, 140, 3, 2, 0.8f, 1.1f, 0.15f, 1, 1.8f, 180.f);
	Far(TEXT("SM_CoasterHill"), 170, 250, 3, 1, 0.9f, 1.1f, 0.1f, 0, 2.75f, 10.f);
	Far(TEXT("SM_FarBigTops"), 340, 430, 4, 1, 1.f, 1.8f, 0.15f, 0, 22.f, 12.f);
	Far(TEXT("SM_FarWheel"), 520, 700, 6, 3, 1.f, 1.2f, 0.1f, 1, 25.f, 15.f);

// ---- 7.56 to 8.03 km: THE FUNHOUSE JUMP. A steep climb, a long jump over a valley and down 14 m: single pieces, a lake
// with the lagoon's serpent in the valley under the flight.
Zone(TEXT("The funhouse jump"), 7.56f, 8.03f);
	Scatter(TEXT("SM_Mascot"), 27, 30, 1, 0, 0.9f, 1.f, 0.f, 0, 6.f);
	Scatter(TEXT("SM_GameStall"), 36, 39, 2, 0, 0.9f, 1.f, 0.f, 0, 4.f);
	Scatter(TEXT("SM_HighStriker"), 36, 39, 2, 1, 0.9f, 1.2f, 0.f, 0, 4.f);
	Far(TEXT("SM_GameStall2"), 46, 49, 2, 0, 1.f, 1.f, 0.f, 0, 1.02f, 4.f);
	Far(TEXT("SM_FoodStand"), 46, 49, 2, 1, 1.f, 1.f, 0.f, 0, 1.02f, 4.f);
	Far(TEXT("SM_Pond"), 58, 72, 2, 0, 0.8f, 1.f, 0.1f, 0, 1.2f, 180.f);
	Far(TEXT("SM_Funhouse"), 64, 76, 4, 1, 1.f, 1.2f, 0.f, 0, 1.25f, 8.f);
	Far(TEXT("/Game/Maps/ThemePark/Props/SM_Tent"), 85, 135, 2, 1, 0.8f, 1.f, 0.15f, 0, 1.75f, 180.f);
	Far(TEXT("SM_SwingRide"), 90, 110, 4, 3, 1.f, 1.f, 0.f, 0, 1.55f, 20.f);
	Far(TEXT("SM_CoasterHill"), 170, 250, 3, 0, 0.9f, 1.1f, 0.1f, 0, 2.75f, 10.f);
	Far(TEXT("SM_FarBigTops"), 340, 430, 4, 2, 1.f, 1.8f, 0.15f, 0, 40.f, 12.f);
	Far(TEXT("SM_FarWheel"), 520, 700, 6, 1, 1.f, 1.2f, 0.1f, -1, 40.f, 15.f);
Zone(TEXT("Jump lake 2"), 7.8683f, 7.8763f);
	SiteAfloat(TEXT("JumpLake2"), 1, 0, 1, 0, 1.f);

// ---- 8.03 to 8.42 km: THE BACK LOT. Where the park is not on show: sheds by the road with spare coaster cars and
// generators, yards behind plain fence, the back of a coaster close by. No lights but the lamp over each door.
Zone(TEXT("The back lot"), 8.03f, 8.42f);
	Site(TEXT("FenceRun"), 0, 19, 1, 0, 0.9f);
	Site(TEXT("BackLot"), -1, 58, 3, 0, 1.f);
	Site(TEXT("BackLot"), 1, 58, 3, 1, 1.f);
	Scatter(TEXT("SM_Maintenance"), 31, 34, 3, 2, 0.9f, 1.f, 0.f, 0, 6.f);
	Scatter(TEXT("SM_CoveBarrels"), 24, 28, 2, 1, 0.7f, 1.f, 0.f, 0, 180.f);
	Far(TEXT("SM_CoasterHill"), 110, 150, 2, 0, 1.f, 1.f, 0.1f, 0, 1.9f, 10.f);
	Far(TEXT("SM_BigWheel"), 190, 210, 7, 3, 1.f, 1.f, 0.f, -1, 2.4f, 8.f);
	Far(TEXT("SM_FarBigTops"), 340, 430, 4, 0, 1.f, 1.8f, 0.15f, 0, 30.f, 12.f);
	Far(TEXT("SM_FarCoaster"), 480, 640, 5, 2, 1.f, 1.3f, 0.1f, 0, 30.f, 10.f);

// ---- 8.42 km to the end: THE LAST RIDE, the finale's promenade. Every land comes round again, a block at a time: on
// each side a corner of one land every 90 m (stall rows, a pirate dock, a rocket pad or a saucer, a ticket gate, a
// funhouse court), ponds, carts and STARRY in the gaps; behind them the rides of all the lands, kept through the
// circuit's loops; two big wheels; and every horizon the level has, turn about.
Zone(TEXT("The Last Ride"), 8.42f, 99.f);
	Scatter(TEXT("SM_SignRide"), 27.5f, 27.5f, 28, 9, 1.f, 1.f, 0.f, 0, 0.f);
	Site(TEXT("FenceRun"), -1, 19, 3, 0, 0.9f);
	Site(TEXT("QueueLine"), -1, 19, 3, 1, 0.8f);
	Site(TEXT("FenceRun"), 1, 19, 3, 1, 0.9f);
	Site(TEXT("SeatsRun"), 1, 19, 3, 2, 0.8f);
	Site(TEXT("StallRow"), -1, 31, 12, 0, 1.f);
	Site(TEXT("PirateDock"), -1, 40, 12, 10, 1.f);
	Site(TEXT("StallRow2"), -1, 31, 12, 8, 1.f);
	Site(TEXT("RocketPad"), -1, 44, 12, 6, 1.f);
	Site(TEXT("Gateway"), -1, 46, 12, 4, 1.f);
	Site(TEXT("FunCourt"), -1, 40, 12, 2, 1.f);
	Site(TEXT("StallRow2"), 1, 31, 12, 11, 1.f);
	Site(TEXT("FunCourt"), 1, 40, 12, 9, 1.f);
	Site(TEXT("StallRow"), 1, 31, 12, 7, 1.f);
	Site(TEXT("Gateway"), 1, 46, 12, 5, 1.f);
	Site(TEXT("SaucerPark"), 1, 38, 12, 3, 1.f);
	Site(TEXT("PirateDock"), 1, 40, 12, 1, 1.f);
	Scatter(TEXT("SM_Pond"), 34, 44, 4, 1, 0.7f, 1.f, 0.1f, -1, 180.f);
	Scatter(TEXT("SM_Mascot"), 28, 34, 4, 3, 0.8f, 1.f, 0.f, -1, 8.f);
	Scatter(TEXT("SM_Pond"), 34, 44, 4, 0, 0.7f, 1.f, 0.1f, 1, 180.f);
	Scatter(TEXT("SM_PlanetPosts"), 29, 35, 4, 2, 0.8f, 1.f, 0.f, 1, 15.f);
	Scatter(TEXT("SM_Bandstand"), 56, 70, 5, 0, 0.9f, 1.f, 0.f, 0, 20.f);
	Scatter(TEXT("SM_Fountain"), 54, 68, 5, 2, 0.9f, 1.3f, 0.f, 0, 20.f);
	Scatter(TEXT("SM_Mascot"), 58, 70, 7, 3, 1.f, 1.5f, 0.f, 0, 10.f);
	Far(TEXT("SM_SkullRock"), 76, 96, 9, 4, 1.f, 1.1f, 0.1f, 0, 1.45f, 25.f);
	Far(TEXT("SM_JetRide"), 84, 104, 7, 1, 1.f, 1.f, 0.f, 0, 1.5f, 20.f);
	Far(TEXT("SM_SwingRide"), 86, 108, 7, 5, 1.f, 1.f, 0.f, 0, 1.55f, 20.f);
	Far(TEXT("SM_Funhouse"), 112, 128, 11, 6, 1.f, 1.3f, 0.f, 0, 1.75f, 8.f);
	Far(TEXT("SM_Maintenance"), 128, 142, 13, 2, 1.f, 1.f, 0.f, 1, 1.85f, 12.f);
	Far(TEXT("/Game/Maps/ThemePark/Props/SM_Tent"), 100, 140, 4, 1, 0.8f, 1.1f, 0.15f, 1, 1.8f, 180.f);
	Far(TEXT("SM_CoasterHill"), 160, 250, 3, 0, 0.9f, 1.2f, 0.15f, 0, 2.75f, 12.f);
	Far(TEXT("SM_BigWheel"), 180, 200, 17, 7, 1.f, 1.f, 0.f, -1, 2.35f, 8.f);
	Far(TEXT("SM_BigWheel"), 250, 270, 23, 11, 1.f, 1.3f, 0.f, 1, 2.9f, 8.f);
	Far(TEXT("SM_FarBigTops"), 340, 430, 6, 1, 1.f, 1.8f, 0.15f, 0, 30.f, 12.f);
	Far(TEXT("SM_FarCoaster"), 480, 640, 7, 3, 1.f, 1.3f, 0.1f, 0, 30.f, 10.f);
	Far(TEXT("SM_FarWheel"), 420, 520, 13, 5, 1.f, 1.f, 0.1f, 1, 30.f, 15.f);
	Far(TEXT("SM_FarWheel"), 640, 820, 17, 9, 1.f, 1.4f, 0.1f, -1, 30.f, 15.f);
	Far(TEXT("SM_FarChute"), 560, 760, 11, 6, 1.f, 1.1f, 0.1f, 0, 30.f, 0.f);
	Far(TEXT("SM_FarRocket"), 620, 820, 19, 12, 1.f, 1.2f, 0.1f, 1, 30.f, 20.f);
	Far(TEXT("SM_FarFort"), 620, 820, 19, 3, 1.f, 1.3f, 0.1f, -1, 30.f, 10.f);
