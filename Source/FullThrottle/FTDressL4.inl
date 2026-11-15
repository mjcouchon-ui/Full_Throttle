// 3.2 dressing table, level 4: Theme Park (Coaster Cove). The words (Set, Zone, Scatter, Site, Far, SiteAfloat, Bank) are explained at the
// top of FTTrackDress.cpp; the meshes are made by Content/Python/ft_dress_park.py. Distances across are metres from the road's
// centre line; districts are in kilometres along the road.
//
// The lie of the land: a sandy promenade to 42 m either side, lawns beyond falling 3 m to the land's edge at 300 m. The
// level's own props stay where they were: bulb lamps on the barrier, trees 27-119 m, booths 30-44 m, balloons 24-64 m,
// big tops on the left and carousels on the right at 46-137 m, a Ferris wheel (right, 219 m), drop towers (left, 238 m) and
// lattice at 238-384 m. The dressing puts a park round them, one LAND after another, each with its own landmark ride, its own
// horizon and its name in lights where it begins (kilometres on the track of 3.3: the coaster grew by 877 m):
//
//   0.00  FRONT GATE     the ticket gates and their forecourts (STARRY the mascot, fountains), the first stalls, the big wheel
//   1.15  THE MIDWAY     an unbroken double row of game stalls and food stands each side; chair swings behind; big tops beyond
//   2.22  PIRATE COVE    swinging ships, skull rocks with lit eyes, barrels and palms; a fort and its ship's masts on the skyline
//   3.21  THE LAGOON     the coaster's valleys: lakes under the rails, lit paths across them, other coasters end on, either side
//   6.72  ROCKET LAND    rocket rides, saucers, planets on poles; a moon rocket and its gantry on the skyline
//   8.12  THE FUNHOUSE   funhouses, chair swings, the mascot; then the back lot: sheds, spare cars, generators, plain fence
//   9.30  THE LAST RIDE  the finale's promenade: every land comes round again, a block at a time, under all the horizons
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
// section has a hollow at its middle, all at the same height. The lakes (SM_Lake, SM_Lake2) are stood by SiteAfloat at
// 3.65 m above the level's floor, which is 15 cm under those hollows' ground: the water then lies 1.15 m deep in every
// hollow and the humps between rise out of it. These heights come from the track's own numbers (floor z -1935, hollows at
// -1485 - 70, the jump valleys' beds at -1014 and -252): if the track's heights ever change, FTBED's "floor z" in the log
// says so, and the "Lagoon" districts and both jump rivers must be laid out again.
//
// SECOND PASS (15 Nov 2026), on the track of 3.3:
//   - the coaster now has fifteen sections, two of them loops and two corkscrews. Eleven have a hollow that takes a lake
//     under the rails. Under a coaster loop or corkscrew the track leaves out EVERYTHING within 39.6 m of the centre line,
//     whatever its height (FTTrack.cpp, RoadSpan), so those four valleys have their lakes lengthways along both shores,
//     from 42 m out, and the ground straight under the rails there is the land's own;
//   - THE FLOOR: flat pieces (SM_Floor...: plazas, paths, lawns, painted games, queue yards, pools, a kart track, service
//     yards) under the sets and, by Far rules, across the level stretches ("Floor ..." districts at the end). Flat pieces
//     are made for level ground: those districts stop at every hill, rise and jump. A Far rule's drop for a floor piece
//     is the land's depth at the NEAR end of its range plus 10 cm, so it never drowns;
//   - THE BANKS: beside a bend banked 46 degrees or more the land is cut down to the road's sunken inner edge, and that
//     cut fills a third of the frame. SM_BankStar and SM_BankBands are planted beds built to lie on it; the "Bank..."
//     sets stand them with a negative "up", one where the bank is at its full angle (the "Bend ..." districts at the end,
//     one slot each, worked out from the track's section list). Three sets, because the cut's place and size go with the
//     bank's angle: 57 degrees (bank 1.0), 52 (0.9, at 0.9 size) and 46 (0.8, at 0.78 size). Each bend also gets one giant
//     on its OUTSIDE (a horizon piece at a third of its size, 44-62 m out): the camera, rolled with the road, looks up
//     that side;
//   - a river crosses under each jump (five lakes in a row across the valley) instead of the one lake under the flight.
//
// THIRD PASS (15 Nov 2026, the overhaul: dense, distinctive and alive at racing speed). What it changed here:
//   - THE VERGE reads now (the same sets, the pieces redrawn in the kit: a striped hoarding under pennants that
//     flutter, guests in the queue rails and on the benches) and the stall rows, gates and courts have a crowd (SM_Crowd);
//   - THE BANKS are faced by the track itself: a Bank rule ("Cut banks" below; SM_CutLawn on cuttings of 5.5 m and
//     more, SM_CutLawnLow on the rest) instead of the second pass's two tilted beds, which were loud and stood only
//     where a bend held its full bank. The beds and their one-slot "Bend" districts are gone; the giants outside the
//     bends stay;
//   - WHAT TRAVELS (ft_life.Kit props of the kit): the SKY RIDE, a line of 45 m spans whose gondolas go from span to
//     span (every span is in step), along the Front Gate, the Midway and the Last Ride on the left, and at three times
//     the size over the lagoon's straight valleys (afloat: its foot on the valley floor, its cable 30 m over the
//     rails); WOODEN COASTERS as stage flats 120 m long with a train going over the hill: a near row everywhere but the
//     lagoon (its own coasters stand end on there) and a far, taller row the whole level long; SWAN BOATS on every
//     lake of the lagoon;
//   - MECHANICAL FIGURES: STARRY waves and nods everywhere he stands, a tin robot waves at every rocket pad;
//   - SIGNATURE MOMENTS, each one slot: THE GALLEON in the kraken's arms, dead ahead of the big jump's landing and
//     passed outside the first hairpin of the cove (2.44 km); LOG FALLS, a mountain with a log flume down its face,
//     dead ahead from the top of the lift hill and passed on the left at 3.69 km; THE LAST RIDE'S GATE, two pairs of marquee
//     towers either side of its sign (9.27 to 9.37 km). A pair of marquee towers also stands where every land begins;
//   - THE MIDDLE DISTANCE: the park's main street as stage flats (SM_FacadeRow) behind the stalls of the Front Gate,
//     the Midway, the Funhouse and the Last Ride; lakes either side of every hollow of the lagoon; parterres and lakes
//     on the lawns round both jumps' landings, hot-air balloons beside both flights and far out over the whole park;
//   - THE HORIZON: two rows of flat skyline cards (SM_Sky1 at 0.9 to 1.05 km, SM_Sky2 at 1.5 to 1.75 km) behind the
//     lit rides, and the far wheel turns.
// A rule put into or taken out of the middle of this table moves the dice of every rule after it (a rule's random
// stream is seeded by its place in the list), and the second pass's districts are to stay piece for piece where they
// were. So this pass's own districts are at the END; in the list above them it changed set contents only, and the one
// rule it retired (the Midway's backstage sheds) stays in its place at chance 0. The second pass's bank beds were the
// list's last districts: they are gone, and the giants that stood among them keep their slots (the few metres of
// jitter each has are rolled anew).

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
	{ TEXT("SM_FloorPave"),   0,    -1,    0,  0, 1 },   // the terrace the row stands on
	{ TEXT("SM_Crowd"),       -3, -4.3f, 0.3f, 0, 1 },   // third pass: the crowd before it
});
Set(TEXT("StallRow2"), {
	{ TEXT("SM_FoodStand"),  -16,    0.3f, 0,  0, 1 },
	{ TEXT("SM_GameStall3"), -8.6f,  0,    0,  0, 1 },
	{ TEXT("SM_HighStriker"), -2.6f, 1,    0,  0, 1 },
	{ TEXT("SM_GameStall"),   3.4f,  0,    0,  0, 1 },
	{ TEXT("SM_GameStall2"), 11,     0,    0,  0, 1 },
	{ TEXT("SM_FloorPave"),   0,    -1,    0,  0, 1 },
	{ TEXT("SM_Crowd"),       4, -4.3f, 0.3f, 0, 1 },   // third pass: the crowd before it
});
Set(TEXT("Gateway"), {                                    // anchor 46 m: a ticket gate, its forecourt between it and the road
	{ TEXT("SM_TicketGate"),   0,   0,     0,  0, 1 },
	{ TEXT("SM_Mascot"),       0, -14,     0,  0, 1 },
	{ TEXT("SM_Fountain"),    18, -12,     0,  0, 1 },
	{ TEXT("SM_QueueRails"), -19, -12,     0,  0, 1 },
	{ TEXT("SM_PlazaSeats"),  18, -19.5f,  0,  0, 1 },
	{ TEXT("SM_GameStall"),  -22,  -1,     0,  0, 1 },
	{ TEXT("SM_GameStall3"),  22,  -1,     0,  0, 1 },
	{ TEXT("SM_FloorPlaza"),   0, -13.5f,  0,  0, 1 },   // the forecourt: a round plaza, 19 to 45 m out
	{ TEXT("SM_Crowd"),       -4, -8, 0.4f, 0, 1 },   // third pass: the crowd before it
});
Set(TEXT("WheelPlaza"), {                                 // anchor 170 m: the big wheel over its own little fair
	{ TEXT("SM_BigWheel"),     0,   0, 0, 0, 1 },
	{ TEXT("SM_GameStall2"), -24, -13, 0, 0, 1 },
	{ TEXT("SM_FoodStand"),  -15, -13, 0, 0, 1 },
	{ TEXT("SM_GameStall"),   15, -13, 0, 0, 1 },
	{ TEXT("SM_Fountain"),     0, -22, 0, 0, 1.3f },
	{ TEXT("SM_Bandstand"),   27, -15, 0, 0, 1 },
	{ TEXT("SM_FloorPlaza"),    0, -24, 0, 0, 1.5f },
	{ TEXT("SM_FloorLawn"),   -40, -34, 0, 0, 1 },
	{ TEXT("SM_FloorLawn"),    40, -34, 0, 0, 1 },
});
Set(TEXT("PirateDock"), {                                 // anchor 40 m: the swinging ship on its dock
	{ TEXT("SM_PirateShip"),    0,  0, 0,   0, 1 },
	{ TEXT("SM_CoveBarrels"), -16, -4, 0,   0, 1 },
	{ TEXT("SM_CoveBarrels"),  17, -3, 0, 180, 1 },
	{ TEXT("/Game/Maps/Bayshore/Props/SM_Palm"),  -14,  5, 0,  40, 1 },
	{ TEXT("/Game/Maps/Bayshore/Props/SM_Palm2"),  15,  6, 0, 200, 1 },
	{ TEXT("/Game/Maps/Bayshore/Props/SM_Palm"),   21, -5, 0, 130, 0.85f },
	{ TEXT("SM_FloorPool"),     0,  1, 0,   0, 1.35f },   // the ship swings over water
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
	{ TEXT("SM_FloorPlaza"),    0,   0, 0,   0, 0.9f },   // the pad
	{ TEXT("SM_Robot"),       -14, -12.5f, 0, 12, 1 },   // third pass: the tin robot waves the cars past
});
Set(TEXT("SaucerPark"), {                                 // anchor 38 m
	{ TEXT("SM_Saucer"),        0,  0, 0, 0, 1 },
	{ TEXT("SM_PlanetPosts"),  13, -1, 0, 0, 1 },
	{ TEXT("SM_PlazaSeats"),  -14, -3, 0, 0, 1 },
	{ TEXT("SM_GameStall3"),  -14,  6, 0, 0, 1 },
	{ TEXT("SM_FloorPlaza"),    0,  1, 0, 0, 0.7f },
});
Set(TEXT("FunCourt"), {                                   // anchor 40 m: the funhouse, the mascot, a chair swing behind
	{ TEXT("SM_Funhouse"),      0,  0, 0,   0, 1 },
	{ TEXT("SM_Mascot"),       12, -5, 0, -10, 1 },
	{ TEXT("SM_GameStall2"),  -12, -3, 0,   0, 1 },
	{ TEXT("SM_SwingRide"),   -30, 10, 0,   0, 1 },
	{ TEXT("SM_FloorGames"),    5, -9, 0,   0, 1 },   // painted games before the door
});
Set(TEXT("BackLot"), {                                    // anchor 58 m: two sheds back to back, their yard fenced
	{ TEXT("SM_Maintenance"),   0,  0, 0,   0, 1 },
	{ TEXT("SM_Maintenance"),  26,  7, 0, 180, 1 },
	{ TEXT("SM_ParkFence"),    -4, -8, 0,   0, 1 },
	{ TEXT("SM_ParkFence"),    18, -8, 0,   0, 1 },
	{ TEXT("SM_CoveBarrels"), -17, -3, 0,  90, 1 },
	{ TEXT("SM_FloorYard"),    11, -17, 0,   0, 1 },   // the service yard before the sheds
});
Set(TEXT("PathCross"), { { TEXT("SM_PathLights"), 0, 0, 0, 90, 1 } });       // a lit path running away from the road
Set(TEXT("HillRibR"), { { TEXT("SM_CoasterHill"), 0, 0, 0, -90, 1 } });      // a coaster end on, its front to the oncoming car
Set(TEXT("HillRibL"), { { TEXT("SM_CoasterHill"), 0, 0, 0, 90, 1 } });
// the lagoon's lakes: placed afloat, so "up" is metres above the level's floor (see THE LAGOON'S WATER above); "along"
// moves each lake from its slot to the middle of its hollow (the number in a set's name is the track's section)
Set(TEXT("Hollow23"), { { TEXT("SM_Lake"), -2.742f, 0, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), -2.742f, 0, 4.95f, 0, 1 }, { TEXT("SM_Lake2"), -2.742f, -87, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), -2.742f, -87, 4.95f, 0, 1 }, { TEXT("SM_Lake2"), -2.742f, 87, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), -2.742f, 87, 4.95f, 0, 1 } });
Set(TEXT("Hollow24"), { { TEXT("SM_Lake2"), -17.823f, 0, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), -17.823f, 0, 4.95f, 0, 1 }, { TEXT("SM_Lake"), -17.823f, -87, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), -17.823f, -87, 4.95f, 0, 1 }, { TEXT("SM_Lake"), -17.823f, 87, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), -17.823f, 87, 4.95f, 0, 1 } });
Set(TEXT("Hollow25"), { { TEXT("SM_Lake"), -18.28f, 0, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), -18.28f, 0, 9.117f, 0, 1 }, { TEXT("SM_Lake2"), -18.28f, -145, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), -18.28f, -145, 9.117f, 0, 1 }, { TEXT("SM_Lake2"), -18.28f, 145, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), -18.28f, 145, 9.117f, 0, 1 } });     // a short hollow next to a loop or corkscrew: stood at 0.6 size
Set(TEXT("Hollow27"), { { TEXT("SM_Lake2"), 11.425f, 0, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), 11.425f, 0, 9.117f, 0, 1 }, { TEXT("SM_Lake"), 11.425f, -145, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), 11.425f, -145, 9.117f, 0, 1 }, { TEXT("SM_Lake"), 11.425f, 145, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), 11.425f, 145, 9.117f, 0, 1 } });     // a short hollow next to a loop or corkscrew: stood at 0.6 size
Set(TEXT("Hollow28"), { { TEXT("SM_Lake"), 13.71f, 0, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), 13.71f, 0, 4.95f, 0, 1 }, { TEXT("SM_Lake2"), 13.71f, -87, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), 13.71f, -87, 4.95f, 0, 1 }, { TEXT("SM_Lake2"), 13.71f, 87, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), 13.71f, 87, 4.95f, 0, 1 } });
Set(TEXT("Hollow29"), { { TEXT("SM_Lake2"), 34.275f, 0, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), 34.275f, 0, 9.117f, 0, 1 }, { TEXT("SM_Lake"), 34.275f, -145, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), 34.275f, -145, 9.117f, 0, 1 }, { TEXT("SM_Lake"), 34.275f, 145, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), 34.275f, 145, 9.117f, 0, 1 } });     // a short hollow next to a loop or corkscrew: stood at 0.6 size
Set(TEXT("Hollow31"), { { TEXT("SM_Lake"), -18.28f, 0, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), -18.28f, 0, 9.117f, 0, 1 }, { TEXT("SM_Lake2"), -18.28f, -145, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), -18.28f, -145, 9.117f, 0, 1 }, { TEXT("SM_Lake2"), -18.28f, 145, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), -18.28f, 145, 9.117f, 0, 1 } });     // a short hollow next to a loop or corkscrew: stood at 0.6 size
Set(TEXT("Hollow33"), { { TEXT("SM_Lake2"), 11.425f, 0, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), 11.425f, 0, 9.117f, 0, 1 }, { TEXT("SM_Lake"), 11.425f, -145, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), 11.425f, -145, 9.117f, 0, 1 }, { TEXT("SM_Lake"), 11.425f, 145, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), 11.425f, 145, 9.117f, 0, 1 } });     // a short hollow next to a loop or corkscrew: stood at 0.6 size
Set(TEXT("Hollow34"), { { TEXT("SM_Lake"), -17.823f, 0, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), -17.823f, 0, 4.95f, 0, 1 }, { TEXT("SM_Lake2"), -17.823f, -87, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), -17.823f, -87, 4.95f, 0, 1 }, { TEXT("SM_Lake2"), -17.823f, 87, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), -17.823f, 87, 4.95f, 0, 1 } });
Set(TEXT("Hollow35"), { { TEXT("SM_Lake2"), 4.57f, 0, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), 4.57f, 0, 9.117f, 0, 1 }, { TEXT("SM_Lake"), 4.57f, -145, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), 4.57f, -145, 9.117f, 0, 1 }, { TEXT("SM_Lake"), 4.57f, 145, 6.95f, 0, 1 }, { TEXT("SM_SwanBoats"), 4.57f, 145, 9.117f, 0, 1 } });     // a short hollow next to a loop or corkscrew: stood at 0.6 size
Set(TEXT("Hollow37"), { { TEXT("SM_Lake"), -15.081f, 0, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), -15.081f, 0, 4.95f, 0, 1 }, { TEXT("SM_Lake2"), -15.081f, -87, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), -15.081f, -87, 4.95f, 0, 1 }, { TEXT("SM_Lake2"), -15.081f, 87, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), -15.081f, 87, 4.95f, 0, 1 } });
// a lake on a shore of a corkscrew's valley (anchor 87 m: 51 to 123 m out) and beside a coaster loop's pit (anchor 108 m;
// "along" brings it to the middle of the pit, and its sign goes with the side, because the loop's frame points backwards there)
Set(TEXT("ShoreLakeA"), { { TEXT("SM_Lake"), 0, 0, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), 0, 0, 4.95f, 0, 1 } });
Set(TEXT("ShoreLakeB"), { { TEXT("SM_Lake2"), 0, 0, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), 0, 0, 4.95f, 0, 1 } });
Set(TEXT("PitLakeR"), { { TEXT("SM_Lake"), -7.9f, 0, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), -7.9f, 0, 4.95f, 0, 1 } });
Set(TEXT("PitLakeL"), { { TEXT("SM_Lake2"), 7.9f, 0, 3.65f, 0, 1 }, { TEXT("SM_SwanBoats"), 7.9f, 0, 4.95f, 0, 1 } });
Set(TEXT("HelixRibR"), { { TEXT("SM_CoasterHill"), 20, 0, 3.2f, -90, 1 } });
Set(TEXT("HelixRibL"), { { TEXT("SM_CoasterHill"), 20, 0, 3.2f,  90, 1 } });
// a river across each jump's valley: five lakes in a row, 72 m apart, the middle one under the flight
Set(TEXT("JumpRiver1"), { { TEXT("SM_Lake"), 6.822f, 0, 10.555f, 0, 1 }, { TEXT("SM_Lake2"), 6.822f, -72, 10.555f, 0, 1 }, { TEXT("SM_Lake"), 6.822f, 72, 10.555f, 0, 1 }, { TEXT("SM_Lake2"), 6.822f, -144, 10.555f, 0, 1 }, { TEXT("SM_Lake"), 6.822f, 144, 10.555f, 0, 1 } });
Set(TEXT("JumpRiver2"), { { TEXT("SM_Lake2"), -7.142f, 0, 18.184f, 0, 1 }, { TEXT("SM_Lake"), -7.142f, -72, 18.184f, 0, 1 }, { TEXT("SM_Lake2"), -7.142f, 72, 18.184f, 0, 1 }, { TEXT("SM_Lake"), -7.142f, -144, 18.184f, 0, 1 }, { TEXT("SM_Lake2"), -7.142f, 144, 18.184f, 0, 1 } });
// THIRD PASS: a span of the sky ride (anchor 35 m: its cables 32.7 and 37.3 m out, its gondolas 15 m up, over the stalls
// and clear of the rides; a span is one slot long and every span is in step, so a row of them is one line); the same at
// three times the size over the lagoon, afloat (its z = 0 is 3.65 m over the level's floor: the valley's floor, like the
// lakes); a marquee tower each side where a land begins; the two landmarks, each on its own slot
Set(TEXT("SkyRun"),    { { TEXT("SM_SkyRide"), 0, 0, 0, 0, 1 } });
Set(TEXT("SkyLagoon"), { { TEXT("SM_SkyRide"), 0, 0, 1.2167f, 0, 1 } });
Set(TEXT("MarqueeGate"), { { TEXT("SM_MarqueeTower"), 0, 0, 0, 0, 1 } });
Set(TEXT("GalleonSpot"), { { TEXT("SM_Galleon"), 0, 0, 0, 25, 1 } });       // turned to face the landing straight she is first seen from
Set(TEXT("FlumeSpot"),   { { TEXT("SM_Flume"), 22.62f, 0, 0, 45, 1 } });     // half a slot back (midway between two of the lagoon's coasters), turned to face the lift hill it is first seen from

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
	Scatter(TEXT("SM_Pond"), 36, 46, 3, 0, 0.7f, 1.f, 0.1f, 0, 180.f);
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
Zone(TEXT("Jump river 1"), 2.0782f, 2.0842f);
	SiteAfloat(TEXT("JumpRiver1"), 1, 0, 1, 0, 1.f);

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

// ---- 3.21 to 6.72 km: THE LAGOON, under the coaster. The land is 26 m under the rails here (and comes up to meet them
// where two coaster sections join), so low things may stand right under the rails: lit paths running out across the
// valley from under them, ponds on the humps. The park's other coasters stand end on at 52-116 m, a loop nearest the
// rails; bandstands, big tops and carousels between; a big wheel and more coasters behind; coasters all round the sky.
// The loops and corkscrews (3.95-4.24, 4.84-5.17, 5.31-5.59 and 6.22-6.55 km) get no near things from the track, so
// their valleys are dressed by the Far rules here and by the afloat sets in the "Lagoon" districts below.
Zone(TEXT("The Lagoon"), 3.21f, 6.7174f);
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
Zone(TEXT("Lagoon, under the rails"), 3.25f, 6.68f);
	Scatter(TEXT("SM_Pond"), 6, 34, 2, 1, 0.8f, 1.f, 0.15f, -1, 180.f);
	Scatter(TEXT("SM_Pond"), 6, 34, 2, 0, 0.8f, 1.f, 0.15f, 1, 180.f);
// the lagoon's lakes on today's coaster: one under the rails in each hollow (a district of one slot each)
Zone(TEXT("Lagoon 23"), 3.3450f, 3.3510f); SiteAfloat(TEXT("Hollow23"), 1, 0, 1, 0, 1.f);
Zone(TEXT("Lagoon 24"), 3.6617f, 3.6677f); SiteAfloat(TEXT("Hollow24"), 1, 0, 1, 0, 1.f);
Zone(TEXT("Lagoon 25"), 3.8879f, 3.8939f); SiteAfloat(TEXT("Hollow25"), 1, 0, 1, 0, 1.f, 0.6f);
Zone(TEXT("Lagoon 27"), 4.2951f, 4.3011f); SiteAfloat(TEXT("Hollow27"), 1, 0, 1, 0, 1.f, 0.6f);
Zone(TEXT("Lagoon 28"), 4.5213f, 4.5273f); SiteAfloat(TEXT("Hollow28"), 1, 0, 1, 0, 1.f);
Zone(TEXT("Lagoon 29"), 4.7475f, 4.7535f); SiteAfloat(TEXT("Hollow29"), 1, 0, 1, 0, 1.f, 0.6f);
Zone(TEXT("Lagoon 31"), 5.2452f, 5.2512f); SiteAfloat(TEXT("Hollow31"), 1, 0, 1, 0, 1.f, 0.6f);
Zone(TEXT("Lagoon 33"), 5.6524f, 5.6584f); SiteAfloat(TEXT("Hollow33"), 1, 0, 1, 0, 1.f, 0.6f);
Zone(TEXT("Lagoon 34"), 5.9238f, 5.9298f); SiteAfloat(TEXT("Hollow34"), 1, 0, 1, 0, 1.f);
Zone(TEXT("Lagoon 35"), 6.1500f, 6.1560f); SiteAfloat(TEXT("Hollow35"), 1, 0, 1, 0, 1.f, 0.6f);
Zone(TEXT("Lagoon 37"), 6.6477f, 6.6537f); SiteAfloat(TEXT("Hollow37"), 1, 0, 1, 0, 1.f);
// the two corkscrews' valleys (sections 30 and 36, flat for 210 m): a lake on each shore at every slot, so each shore is
// one sheet of water 180 m long from 51 to 123 m out, and the park's coasters end on behind it
Zone(TEXT("Lagoon 30, the corkscrew's shores"), 4.925f, 5.075f);
	SiteAfloat(TEXT("ShoreLakeA"), -1, 87, 2, 1, 1.f);
	SiteAfloat(TEXT("ShoreLakeB"), -1, 87, 2, 0, 1.f);
	SiteAfloat(TEXT("ShoreLakeB"), 1, 87, 2, 1, 1.f);
	SiteAfloat(TEXT("ShoreLakeA"), 1, 87, 2, 0, 1.f);
	SiteAfloat(TEXT("HelixRibL"), -1, 162, 2, 0, 1.f);
	SiteAfloat(TEXT("HelixRibR"), 1, 162, 2, 1, 1.f);
Zone(TEXT("Lagoon 36, the double corkscrew's shores"), 6.328f, 6.476f);
	SiteAfloat(TEXT("ShoreLakeA"), -1, 87, 2, 1, 1.f);
	SiteAfloat(TEXT("ShoreLakeB"), -1, 87, 2, 0, 1.f);
	SiteAfloat(TEXT("ShoreLakeB"), 1, 87, 2, 1, 1.f);
	SiteAfloat(TEXT("ShoreLakeA"), 1, 87, 2, 0, 1.f);
	SiteAfloat(TEXT("HelixRibL"), -1, 162, 2, 0, 1.f);
	SiteAfloat(TEXT("HelixRibR"), 1, 162, 2, 1, 1.f);
// the two coaster loops' pits (sections 26 and 32): a lake either side of the loop, seen from its top
Zone(TEXT("Lagoon 26, the loop's pit"), 4.0689f, 4.0749f);
	SiteAfloat(TEXT("PitLakeL"), -1, 108, 1, 0, 1.f);
	SiteAfloat(TEXT("PitLakeR"), 1, 108, 1, 0, 1.f);
Zone(TEXT("Lagoon 32, the second loop's pit"), 5.4262f, 5.4322f);
	SiteAfloat(TEXT("PitLakeL"), -1, 108, 1, 0, 1.f);
	SiteAfloat(TEXT("PitLakeR"), 1, 108, 1, 0, 1.f);

// ---- 6.72 to 6.91 km: SPLASHDOWN. Off the rails and down 8 m: planets on poles line the drop, saucers behind.
Zone(TEXT("Splashdown"), 6.7174f, 6.9074f);
	Scatter(TEXT("SM_SignRocket"), 27.5f, 27.5f, 5, 0, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_PlanetPosts"), 28, 31, 1, 0, 0.9f, 1.f, 0.f, 0, 10.f);
	Scatter(TEXT("SM_GameStall3"), 36, 40, 2, 0, 0.9f, 1.f, 0.f, 0, 4.f);
	Scatter(TEXT("SM_Saucer"), 44, 52, 2, 1, 0.9f, 1.f, 0.f, 0, 30.f);
	Far(TEXT("SM_Pond"), 58, 72, 2, 0, 0.8f, 1.f, 0.1f, 0, 1.25f, 180.f);
	Far(TEXT("SM_JetRide"), 84, 104, 2, 1, 1.f, 1.f, 0.f, 0, 1.5f, 20.f);
	Far(TEXT("SM_CoasterHill"), 150, 250, 2, 0, 0.9f, 1.2f, 0.1f, 0, 2.75f, 10.f);
	Far(TEXT("SM_FarRocket"), 360, 460, 3, 1, 1.f, 1.f, 0.1f, -1, 30.f, 20.f);

// ---- 6.91 to 8.12 km: ROCKET LAND. A rocket ride or a saucer every 90 m on each side, planets on poles between; more
// rides and saucers behind, carried through the corkscrew and the loop; a moon rocket on its pad on the skyline.
Zone(TEXT("Rocket Land"), 6.9074f, 8.1174f);
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
Zone(TEXT("Rocket corkscrew"), 7.33f, 7.52f);
	Far(TEXT("SM_PlanetPosts"), 28, 30, 1, 0, 0.9f, 1.f, 0.f, 0, 0.9f, 15.f);
	Far(TEXT("SM_PlanetPosts"), 46, 52, 2, 0, 0.9f, 1.3f, 0.f, 0, 1.05f, 15.f);
	Far(TEXT("SM_GameStall3"), 33, 36, 2, 0, 1.f, 1.f, 0.f, 0, 0.94f, 4.f);
	Far(TEXT("SM_Saucer"), 37, 42, 2, 1, 1.f, 1.f, 0.f, 0, 0.97f, 30.f);
Zone(TEXT("Rocket loop"), 7.8404f, 8.1204f);
	Far(TEXT("SM_PlanetPosts"), 66, 72, 2, 1, 0.9f, 1.3f, 0.f, -1, 1.2f, 15.f);
	Far(TEXT("SM_Saucer"), 68, 78, 2, 0, 1.f, 1.2f, 0.f, -1, 1.3f, 30.f);
	Far(TEXT("SM_PlanetPosts"), 68, 76, 1, 0, 0.9f, 1.2f, 0.f, 1, 1.25f, 15.f);

// ---- 8.12 to 8.44 km: THE FUNHOUSE. A funhouse court every 135 m on each side (the mascot at its door, a chair swing
// behind), STARRY between them, bigger funhouses behind.
Zone(TEXT("The Funhouse"), 8.1174f, 8.4374f);
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

// ---- 8.44 to 8.91 km: THE FUNHOUSE JUMP. A steep climb, a long jump over a valley and down 14 m: single pieces, a lake
// with the lagoon's serpent in the valley under the flight.
Zone(TEXT("The funhouse jump"), 8.4374f, 8.9074f);
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
Zone(TEXT("Jump river 2"), 8.7741f, 8.7801f);
	SiteAfloat(TEXT("JumpRiver2"), 1, 0, 1, 0, 1.f);

// ---- 8.91 to 9.30 km: THE BACK LOT. Where the park is not on show: sheds by the road with spare coaster cars and
// generators, yards behind plain fence, the back of a coaster close by. No lights but the lamp over each door.
Zone(TEXT("The back lot"), 8.9074f, 9.2974f);
	Site(TEXT("FenceRun"), 0, 19, 1, 0, 0.9f);
	Site(TEXT("BackLot"), -1, 58, 3, 0, 1.f);
	Site(TEXT("BackLot"), 1, 58, 3, 1, 1.f);
	Scatter(TEXT("SM_Maintenance"), 31, 34, 3, 2, 0.9f, 1.f, 0.f, 0, 6.f);
	Scatter(TEXT("SM_CoveBarrels"), 24, 28, 2, 1, 0.7f, 1.f, 0.f, 0, 180.f);
	Far(TEXT("SM_CoasterHill"), 110, 150, 2, 0, 1.f, 1.f, 0.1f, 0, 1.9f, 10.f);
	Far(TEXT("SM_BigWheel"), 190, 210, 7, 3, 1.f, 1.f, 0.f, -1, 2.4f, 8.f);
	Far(TEXT("SM_FarBigTops"), 340, 430, 4, 0, 1.f, 1.8f, 0.15f, 0, 30.f, 12.f);
	Far(TEXT("SM_FarCoaster"), 480, 640, 5, 2, 1.f, 1.3f, 0.1f, 0, 30.f, 10.f);

// ---- 9.30 km to the end: THE LAST RIDE, the finale's promenade. Every land comes round again, a block at a time: on
// each side a corner of one land every 90 m (stall rows, a pirate dock, a rocket pad or a saucer, a ticket gate, a
// funhouse court), ponds, carts and STARRY in the gaps; behind them the rides of all the lands, kept through the
// circuit's loops; two big wheels; and every horizon the level has, turn about.
Zone(TEXT("The Last Ride"), 9.2974f, 99.f);
	Scatter(TEXT("SM_SignRide"), 27.5f, 27.5f, 28, 18, 1.f, 1.f, 0.f, 0, 0.f);
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
	Scatter(TEXT("SM_Pond"), 36, 46, 4, 1, 0.7f, 1.f, 0.1f, -1, 180.f);
	Scatter(TEXT("SM_Mascot"), 28, 34, 4, 3, 0.8f, 1.f, 0.f, -1, 8.f);
	Scatter(TEXT("SM_Pond"), 36, 46, 4, 0, 0.7f, 1.f, 0.1f, 1, 180.f);
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

// ================================================================ SECOND PASS: THE FLOOR
// Flat pieces across the level stretches, by Far rules (they stay through the loops and corkscrews; the track's own test
// still moves them off the road). Every range starts 26 m out or more: nearer, beside a banked bend, is the cut bank.
// Odd slots carry the lawns and the far pools, even slots the courts, plazas and yards, and the paths keep to the slots
// between a land's sets, so two kinds seldom lie on each other; where they do, each kind has its own height.
const auto ParkLawns = [&]()
{
	Far(TEXT("SM_FloorLawn"), 58, 72, 2, 1, 0.9f, 1.f, 0.1f, 0, 1.04f, 4.f);
	Far(TEXT("SM_FloorPool"), 92, 110, 4, 1, 0.9f, 1.3f, 0.1f, 0, 1.33f, 30.f);
};
Zone(TEXT("Floor: the front gate"), 0.f, 0.53f);
	ParkLawns();
	Far(TEXT("SM_FloorSpoke"), 46, 46, 3, 0, 1.f, 1.f, 0.f, 0, 0.93f, 0.f);
	Far(TEXT("SM_FloorQueue"), 36, 38, 6, 3, 0.9f, 1.f, 0.f, 0, 0.88f, 3.f);
	Far(TEXT("SM_FloorGames"), 50, 54, 4, 0, 0.9f, 1.f, 0.f, 0, 0.97f, 4.f);
	Far(TEXT("SM_FloorKart"), 108, 118, 12, 6, 1.f, 1.f, 0.f, 1, 1.46f, 3.f);
Zone(TEXT("Floor: the gate's end"), 0.72f, 1.14f);
	ParkLawns();
	Far(TEXT("SM_FloorSpoke"), 46, 46, 3, 0, 1.f, 1.f, 0.f, 0, 0.93f, 0.f);
	Far(TEXT("SM_FloorGames"), 50, 54, 4, 0, 0.9f, 1.f, 0.f, 0, 0.97f, 4.f);
	Far(TEXT("SM_FloorKart"), 108, 118, 12, 0, 1.f, 1.f, 0.f, -1, 1.46f, 3.f);
// the midway's rows are unbroken, so no paths cross them: behind the second row the park is backstage (service yards,
// a shed every 180 m), then lawns
Zone(TEXT("Floor: behind the midway"), 1.15f, 1.86f);
	ParkLawns();
	Far(TEXT("SM_FloorYard"), 60, 66, 2, 0, 0.9f, 1.f, 0.f, 0, 1.05f, 4.f);
	Far(TEXT("SM_Maintenance"), 78, 84, 4, 2, 0.f, 1.f, 0.f, 0, 1.3f, 8.f);       // third pass: chance 0, the main street's flats stand there now. Left in the list: taken out, every rule after it would roll other dice
	Far(TEXT("SM_FloorPlaza"), 84, 96, 6, 4, 1.f, 1.2f, 0.1f, 0, 1.26f, 0.f);
Zone(TEXT("Floor: pirate cove"), 2.23f, 3.0f);
	ParkLawns();
	Far(TEXT("SM_FloorSpoke"), 46, 46, 6, 0, 1.f, 1.f, 0.f, 0, 0.93f, 0.f);
	Far(TEXT("SM_FloorPool"), 52, 62, 2, 0, 0.9f, 1.2f, 0.1f, 0, 0.99f, 40.f);
	Far(TEXT("SM_FloorPlaza"), 84, 96, 6, 2, 1.f, 1.2f, 0.1f, 0, 1.26f, 0.f);
Zone(TEXT("Floor: rocket land"), 6.92f, 8.11f);
	ParkLawns();
	Far(TEXT("SM_FloorSpoke"), 46, 46, 4, 1, 1.f, 1.f, 0.f, 0, 0.93f, 0.f);
	Far(TEXT("SM_FloorPlaza"), 62, 70, 4, 3, 1.f, 1.f, 0.1f, 0, 1.07f, 0.f);
	Far(TEXT("SM_FloorKart"), 108, 118, 12, 4, 1.f, 1.f, 0.f, 1, 1.46f, 3.f);
	Far(TEXT("SM_FloorKart"), 108, 118, 12, 10, 1.f, 1.f, 0.f, -1, 1.46f, 3.f);
Zone(TEXT("Floor: the funhouse"), 8.12f, 8.45f);
	ParkLawns();
	Far(TEXT("SM_FloorSpoke"), 46, 46, 3, 0, 1.f, 1.f, 0.f, 0, 0.93f, 0.f);
	Far(TEXT("SM_FloorGames"), 50, 54, 2, 0, 0.9f, 1.f, 0.f, 0, 0.97f, 4.f);
	Far(TEXT("SM_FloorQueue"), 36, 38, 3, 0, 0.9f, 1.f, 0.f, 0, 0.88f, 3.f);
Zone(TEXT("Floor: the back lot"), 8.93f, 9.29f);
	Far(TEXT("SM_FloorYard"), 78, 86, 2, 1, 1.f, 1.2f, 0.f, 0, 1.21f, 4.f);
// the finale: its sets stand on even slots on the left and odd slots on the right, so the paths take the others
const auto ParkFinale = [&]()
{
	ParkLawns();
	Far(TEXT("SM_FloorSpoke"), 46, 46, 4, 1, 1.f, 1.f, 0.f, -1, 0.93f, 0.f);
	Far(TEXT("SM_FloorSpoke"), 46, 46, 4, 0, 1.f, 1.f, 0.f, 1, 0.93f, 0.f);
	Far(TEXT("SM_FloorGames"), 50, 54, 6, 0, 0.9f, 1.f, 0.f, 1, 0.97f, 4.f);
	Far(TEXT("SM_FloorGames"), 50, 54, 6, 3, 0.9f, 1.f, 0.f, -1, 0.97f, 4.f);
	Far(TEXT("SM_FloorPlaza"), 84, 96, 6, 2, 1.f, 1.2f, 0.1f, 0, 1.26f, 0.f);
	Far(TEXT("SM_FloorYard"), 64, 70, 8, 6, 1.f, 1.f, 0.f, 0, 1.09f, 4.f);
	Far(TEXT("SM_FloorQueue"), 36, 38, 6, 1, 0.9f, 1.f, 0.f, -1, 0.88f, 3.f);
	Far(TEXT("SM_FloorQueue"), 36, 38, 6, 4, 0.9f, 1.f, 0.f, 1, 0.88f, 3.f);
	Far(TEXT("SM_FloorKart"), 108, 118, 12, 8, 1.f, 1.f, 0.f, -1, 1.46f, 3.f);
	Far(TEXT("SM_FloorKart"), 108, 118, 12, 2, 1.f, 1.f, 0.f, 1, 1.46f, 3.f);
};
Zone(TEXT("Floor: the last ride, to the arena's hill"), 9.3f, 11.23f);
	ParkFinale();
Zone(TEXT("Floor: the last ride, after the hill"), 11.94f, 99.f);
	ParkFinale();

// ================================================================ SECOND PASS: THE BANKED BENDS
// One giant on the OUTSIDE of each steep bend (one district a slot, from the track's section list; the track is locked).
// The second pass's bank beds on the bends' insides are gone: the third pass's "Cut banks" rule faces every cutting.
Zone(TEXT("Bend 1.09, outside"), 1.0828f, 1.0888f); Far(TEXT("SM_FarBigTops"), 55, 57, 1, 0, 1.f, 0.42f, 0.f, -1, 1.06f, 6.f);
Zone(TEXT("Bend 1.63, outside"), 1.6257f, 1.6317f); Far(TEXT("SM_FarWheel"), 62, 64, 1, 0, 1.f, 0.5f, 0.f, -1, 1.12f, 6.f);
Zone(TEXT("Bend 1.76, outside"), 1.7615f, 1.7675f); Far(TEXT("SM_FarWheel"), 62, 64, 1, 0, 1.f, 0.5f, 0.f, 1, 1.12f, 6.f);
Zone(TEXT("Bend 2.58, outside"), 2.5759f, 2.5819f); Far(TEXT("SM_FarFort"), 50, 52, 1, 0, 1.f, 0.3f, 0.f, 1, 1.03f, 6.f);
Zone(TEXT("Bend 7.01, outside"), 7.0097f, 7.0157f); Far(TEXT("SM_FarRocket"), 46, 48, 1, 0, 1.f, 0.32f, 0.f, -1, 1, 6.f);
Zone(TEXT("Bend 7.15, outside"), 7.1454f, 7.1514f); Far(TEXT("SM_FarRocket"), 46, 48, 1, 0, 1.f, 0.32f, 0.f, 1, 1, 6.f);
Zone(TEXT("Bend 7.24, outside"), 7.2359f, 7.2419f); Far(TEXT("SM_FarRocket"), 46, 48, 1, 0, 1.f, 0.32f, 0.f, -1, 1, 6.f);
Zone(TEXT("Bend 7.60, outside"), 7.5978f, 7.6038f); Far(TEXT("SM_FarRocket"), 46, 48, 1, 0, 1.f, 0.32f, 0.f, 1, 1, 6.f);
Zone(TEXT("Bend 7.74, outside"), 7.7336f, 7.7396f); Far(TEXT("SM_FarRocket"), 46, 48, 1, 0, 1.f, 0.32f, 0.f, -1, 1, 6.f);
Zone(TEXT("Bend 8.28, outside"), 8.2765f, 8.2825f); Far(TEXT("SM_FarBigTops"), 55, 57, 1, 0, 1.f, 0.42f, 0.f, 1, 1.06f, 6.f);
Zone(TEXT("Bend 8.42, outside"), 8.4122f, 8.4182f); Far(TEXT("SM_FarBigTops"), 55, 57, 1, 0, 1.f, 0.42f, 0.f, -1, 1.06f, 6.f);
Zone(TEXT("Bend 9.09, outside"), 9.0908f, 9.0968f); Far(TEXT("SM_FarChute"), 46, 48, 1, 0, 1.f, 0.3f, 0.f, 1, 1, 6.f);
Zone(TEXT("Bend 9.23, outside"), 9.2266f, 9.2326f); Far(TEXT("SM_FarChute"), 46, 48, 1, 0, 1.f, 0.3f, 0.f, -1, 1, 6.f);
Zone(TEXT("Bend 15.07, outside"), 15.0629f, 15.0689f); Far(TEXT("SM_FarWheel"), 62, 64, 1, 0, 1.f, 0.5f, 0.f, -1, 1.12f, 6.f);
Zone(TEXT("Bend 15.20, outside"), 15.1986f, 15.2046f); Far(TEXT("SM_FarRocket"), 46, 48, 1, 0, 1.f, 0.32f, 0.f, 1, 1, 6.f);
Zone(TEXT("Bend 16.33, outside"), 16.3297f, 16.3357f); Far(TEXT("SM_FarFort"), 50, 52, 1, 0, 1.f, 0.3f, 0.f, -1, 1.03f, 6.f);
Zone(TEXT("Bend 16.47, outside"), 16.4655f, 16.4715f); Far(TEXT("SM_FarBigTops"), 55, 57, 1, 0, 1.f, 0.42f, 0.f, 1, 1.06f, 6.f);
Zone(TEXT("Bend 17.55, outside"), 17.5513f, 17.5573f); Far(TEXT("SM_FarChute"), 46, 48, 1, 0, 1.f, 0.3f, 0.f, -1, 1, 6.f);
Zone(TEXT("Bend 17.69, outside"), 17.6870f, 17.6930f); Far(TEXT("SM_FarWheel"), 62, 64, 1, 0, 1.f, 0.5f, 0.f, 1, 1.12f, 6.f);
Zone(TEXT("Bend 18.82, outside"), 18.8181f, 18.8241f); Far(TEXT("SM_FarRocket"), 46, 48, 1, 0, 1.f, 0.32f, 0.f, -1, 1, 6.f);
Zone(TEXT("Bend 18.96, outside"), 18.9538f, 18.9598f); Far(TEXT("SM_FarFort"), 50, 52, 1, 0, 1.f, 0.3f, 0.f, 1, 1.03f, 6.f);
Zone(TEXT("Bend 20.09, outside"), 20.0849f, 20.0909f); Far(TEXT("SM_FarBigTops"), 55, 57, 1, 0, 1.f, 0.42f, 0.f, -1, 1.06f, 6.f);
Zone(TEXT("Bend 20.22, outside"), 20.2206f, 20.2266f); Far(TEXT("SM_FarChute"), 46, 48, 1, 0, 1.f, 0.3f, 0.f, 1, 1, 6.f);
Zone(TEXT("Bend 21.35, outside"), 21.3517f, 21.3577f); Far(TEXT("SM_FarWheel"), 62, 64, 1, 0, 1.f, 0.5f, 0.f, -1, 1.12f, 6.f);
Zone(TEXT("Bend 21.49, outside"), 21.4874f, 21.4934f); Far(TEXT("SM_FarRocket"), 46, 48, 1, 0, 1.f, 0.32f, 0.f, 1, 1, 6.f);
Zone(TEXT("Bend 22.62, outside"), 22.6185f, 22.6245f); Far(TEXT("SM_FarFort"), 50, 52, 1, 0, 1.f, 0.3f, 0.f, -1, 1.03f, 6.f);
Zone(TEXT("Bend 22.76, outside"), 22.7542f, 22.7602f); Far(TEXT("SM_FarBigTops"), 55, 57, 1, 0, 1.f, 0.42f, 0.f, 1, 1.06f, 6.f);
Zone(TEXT("Bend 23.89, outside"), 23.8853f, 23.8913f); Far(TEXT("SM_FarChute"), 46, 48, 1, 0, 1.f, 0.3f, 0.f, -1, 1, 6.f);
Zone(TEXT("Bend 24.02, outside"), 24.0210f, 24.0270f); Far(TEXT("SM_FarWheel"), 62, 64, 1, 0, 1.f, 0.5f, 0.f, 1, 1.12f, 6.f);

// ================================================================ THIRD PASS (the overhaul)
// ---- the cut banks, faced by the track: a planted hedge bank on every cutting a banked bend shows (FTDress.h, Bank).
// The tall panel where 5.5 m and more of cutting shows (bends banked 32 degrees and up), the low one on the rest.
Zone(TEXT("Cut banks"), 0.f, 99.f);
	Bank(TEXT("SM_CutLawn"), 4, 1, 0, 1.f, 5.5f);
	Bank(TEXT("SM_CutLawnLow"), 4, 1, 0, 1.f, 2.4f, 5.5f);

// ---- the sky ride, on the left: a span a slot (they join into one line where the road runs straight; on a bend each
// span stands on its own two pylons). Near pieces: the track leaves them out of the corkscrew and the loops.
Zone(TEXT("Sky ride: the front gate"), 0.07f, 0.96f);
	Site(TEXT("SkyRun"), -1, 35, 1, 0, 1.f);
Zone(TEXT("Sky ride: the midway"), 1.16f, 1.56f);
	Site(TEXT("SkyRun"), -1, 35, 1, 0, 1.f);
Zone(TEXT("Sky ride: the last ride"), 9.32f, 14.f);
	Site(TEXT("SkyRun"), -1, 35, 1, 0, 1.f);
// over the lagoon it runs the straight valleys at three times the size (a span is three slots), 86 m out, afloat
Zone(TEXT("Sky ride: over the lagoon, 3.26 km"), 3.2394f, 3.4113f);
	SiteAfloat(TEXT("SkyLagoon"), 1, 86, 3, 0, 1.f, 3.f);
Zone(TEXT("Sky ride: over the lagoon, 3.89 km"), 3.8728f, 4.1805f);
	SiteAfloat(TEXT("SkyLagoon"), -1, 86, 3, 1, 1.f, 3.f);
Zone(TEXT("Sky ride: over the lagoon, 4.43 km"), 4.4157f, 5.1306f);
	SiteAfloat(TEXT("SkyLagoon"), 1, 86, 3, 1, 1.f, 3.f);
Zone(TEXT("Sky ride: over the lagoon, 5.38 km"), 5.3658f, 5.6735f);
	SiteAfloat(TEXT("SkyLagoon"), -1, 86, 3, 1, 1.f, 3.f);
Zone(TEXT("Sky ride: over the lagoon, 6.15 km"), 6.1350f, 6.7141f);
	SiteAfloat(TEXT("SkyLagoon"), 1, 86, 3, 2, 1.f, 3.f);

// ---- wooden coasters as stage flats, 120 m long with a train going over the hill: a near row (not in the lagoon: the
// coasters that stand end on there reach 118 m out) and a far, taller one the whole level long
Zone(TEXT("Wooden coasters: to the lift hill"), 0.f, 3.2f);
	Far(TEXT("SM_WoodCoaster"), 108, 124, 8, 4, 1.f, 1.f, 0.f, -1, 2.013f, 8.f);
	Far(TEXT("SM_WoodCoaster"), 108, 124, 8, 0, 1.f, 1.f, 0.f, 1, 2.013f, 8.f);
Zone(TEXT("Wooden coasters: from rocket land on"), 6.72f, 99.f);
	Far(TEXT("SM_WoodCoaster"), 108, 124, 8, 4, 1.f, 1.f, 0.f, -1, 2.013f, 8.f);
	Far(TEXT("SM_WoodCoaster"), 108, 124, 8, 0, 1.f, 1.f, 0.f, 1, 2.013f, 8.f);
Zone(TEXT("Wooden coasters, far"), 0.f, 99.f);
	Far(TEXT("SM_WoodCoaster"), 205, 235, 12, 6, 1.f, 1.35f, 0.f, -1, 3.14f, 8.f);
	Far(TEXT("SM_WoodCoaster"), 205, 235, 12, 0, 1.f, 1.35f, 0.f, 1, 3.14f, 8.f);

// ---- the park's main street as stage flats, behind the stalls and before the rides
Zone(TEXT("Main street: the front gate"), 0.f, 1.14f);
	Far(TEXT("SM_FacadeRow"), 72, 78, 3, 1, 1.f, 1.f, 0.f, 0, 1.506f, 4.f);
Zone(TEXT("Main street: behind the midway"), 1.15f, 1.86f);
	Far(TEXT("SM_FacadeRow"), 80, 86, 2, 0, 1.f, 1.15f, 0.f, 0, 1.574f, 4.f);
Zone(TEXT("Main street: the funhouse"), 8.12f, 8.45f);
	Far(TEXT("SM_FacadeRow"), 80, 86, 2, 1, 1.f, 1.f, 0.f, 0, 1.574f, 4.f);
Zone(TEXT("Main street: the last ride"), 9.3f, 99.f);
	Far(TEXT("SM_FacadeRow"), 72, 80, 4, 2, 1.f, 1.f, 0.f, -1, 1.506f, 4.f);
	Far(TEXT("SM_FacadeRow"), 72, 80, 4, 0, 1.f, 1.f, 0.f, 1, 1.506f, 4.f);

// ---- a pair of marquee towers where a land begins (their star turns, their bulbs climb), one slot each
Zone(TEXT("Marquee: the gate"), 0.1327f, 0.1387f); Site(TEXT("MarqueeGate"), -1, 27.4f, 1, 0, 1.f); Site(TEXT("MarqueeGate"), 1, 27.4f, 1, 0, 1.f);
Zone(TEXT("Marquee: the midway"), 1.2186f, 1.2246f); Site(TEXT("MarqueeGate"), -1, 27.4f, 1, 0, 1.f); Site(TEXT("MarqueeGate"), 1, 27.4f, 1, 0, 1.f);
Zone(TEXT("Marquee: pirate cove"), 2.2139f, 2.2199f); Site(TEXT("MarqueeGate"), -1, 27.4f, 1, 0, 1.f); Site(TEXT("MarqueeGate"), 1, 27.4f, 1, 0, 1.f);
Zone(TEXT("Marquee: the lagoon"), 2.9830f, 2.9890f); Site(TEXT("MarqueeGate"), -1, 27.4f, 1, 0, 1.f); Site(TEXT("MarqueeGate"), 1, 27.4f, 1, 0, 1.f);
Zone(TEXT("Marquee: rocket land"), 6.8287f, 6.8347f); Site(TEXT("MarqueeGate"), -1, 27.4f, 1, 0, 1.f); Site(TEXT("MarqueeGate"), 1, 27.4f, 1, 0, 1.f);
Zone(TEXT("Marquee: the funhouse"), 8.1407f, 8.1467f); Site(TEXT("MarqueeGate"), -1, 27.4f, 1, 0, 1.f); Site(TEXT("MarqueeGate"), 1, 27.4f, 1, 0, 1.f);
Zone(TEXT("Marquee: the last ride"), 9.2718f, 9.2778f); Site(TEXT("MarqueeGate"), -1, 27.4f, 1, 0, 1.f); Site(TEXT("MarqueeGate"), 1, 27.4f, 1, 0, 1.f);
Zone(TEXT("Marquee: the last ride, on"), 9.3623f, 9.3683f); Site(TEXT("MarqueeGate"), -1, 27.4f, 1, 0, 1.f); Site(TEXT("MarqueeGate"), 1, 27.4f, 1, 0, 1.f);

// ---- THE GALLEON (signature): aground on her rock in the kraken's arms, on the outside of the cove's first hairpin:
// dead ahead from the big jump's take-off (450 m off) and all the way down the landing straight, passed at 2.44 km
Zone(TEXT("The galleon"), 2.4401f, 2.4461f);
	Site(TEXT("GalleonSpot"), -1, 80, 1, 0, 1.f);
// ---- LOG FALLS (signature): the lagoon's landmark, a mountain 52 m tall with a log flume down its face, on the left
// between two of the coasters that stand end on there. Dead ahead from the top of the lift hill, 590 m off; the rails
// turn right in front of it and pass it at 3.69 km
Zone(TEXT("Log Falls"), 3.7069f, 3.7129f);
	Site(TEXT("FlumeSpot"), -1, 100, 1, 0, 1.f);
// and both landmarks come round again in the finale, far apart
Zone(TEXT("Landmarks of the last ride"), 9.6f, 99.f);
	Far(TEXT("SM_Galleon"), 100, 112, 43, 9, 1.f, 1.f, 0.f, -1, 1.745f, 20.f);
	Far(TEXT("SM_Flume"), 124, 136, 47, 20, 1.f, 1.f, 0.f, 1, 1.949f, 25.f);

// ---- hot-air balloons: beside both jumps' flights (the vista from the air), and far out over the whole park
Zone(TEXT("Balloons: the big jump"), 1.96f, 2.26f);
	Far(TEXT("SM_Balloon"), 60, 150, 1, 0, 0.9f, 1.5f, 0.25f, 0, -38.f, 180.f);
Zone(TEXT("Balloons: the funhouse jump"), 8.62f, 8.95f);
	Far(TEXT("SM_Balloon"), 60, 150, 1, 0, 0.9f, 1.5f, 0.25f, 0, -44.f, 180.f);
Zone(TEXT("Balloons, far out"), 0.f, 99.f);
	Far(TEXT("SM_Balloon"), 190, 290, 12, 5, 0.9f, 2.2f, 0.3f, 0, -55.f, 180.f);

// ---- the vistas from both jumps: parterres (the lawn panel at twice its size) and lakes on the lawns round the landings
Zone(TEXT("Vista: the cove from the air"), 2.17f, 2.52f);
	Far(TEXT("SM_FloorLawn"), 104, 120, 2, 0, 1.f, 2.f, 0.f, 0, 1.579f, 4.f);
	Far(TEXT("SM_Lake"), 165, 185, 3, 1, 1.f, 1.f, 0.f, 0, 3.284f, 6.f);
Zone(TEXT("Vista: the back lot from the air"), 8.86f, 9.1f);
	Far(TEXT("SM_FloorLawn"), 104, 120, 2, 0, 1.f, 2.f, 0.f, 0, 1.579f, 4.f);
	Far(TEXT("SM_Lake2"), 165, 185, 3, 0, 1.f, 1.f, 0.f, 0, 3.284f, 6.f);

// ---- the horizon's cards: two rows of flat skyline behind the lit rides, each row its own outline and tone
Zone(TEXT("Horizon cards"), 0.f, 99.f);
	Far(TEXT("SM_Sky1"), 900, 1050, 9, 0, 1.f, 1.25f, 0.1f, -1, 30.f, 6.f);
	Far(TEXT("SM_Sky1"), 900, 1050, 9, 4, 1.f, 1.25f, 0.1f, 1, 30.f, 6.f);
	Far(TEXT("SM_Sky2"), 1500, 1750, 12, 3, 1.f, 1.6f, 0.1f, -1, 35.f, 6.f);
	Far(TEXT("SM_Sky2"), 1500, 1750, 12, 9, 1.f, 1.6f, 0.1f, 1, 35.f, 6.f);
