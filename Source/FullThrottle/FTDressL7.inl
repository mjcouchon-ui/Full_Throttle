// 3.2 dressing table, level 7: City Center (Midtown). The words (Set, Zone, Scatter, Site, Far) are explained at the top of
// FTTrackDress.cpp; the meshes are made by Content/Python/ft_dress_metro.py. Distances across are metres from the road's
// centre line; districts are in kilometres along the road.
//
// The lie of the land: flat sidewalks either side. The level's own props stay where they were: street lamps on the barrier,
// hydrants and newsstands at 16 m, a tower every 45 m on each side with its middle 33-42 m out (its front 23-32 m out),
// more towers at 51-82 m, a skyline at 165-366 m, the Chairman's tower at the road's end. The dressing makes blocks of them:
//
//   17-24 m   THE SIDEWALK   parked cars at the kerb, benches, trees in tubs, mail boxes, bus shelters: low and unlit
//   25 m      THE STREET WALL  a 32 m podium in front of each tower (shops, a picture house, a bank, a garage and its
//             ramps, a service yard ...) and, in the 13 m between two podiums, an alley, a monument or a checkpoint
//   90-160 m  mid-rise blocks with their roofs (water tanks, a sign, a pool and a landing pad), the elevated railway
//   420 m +   distant towers, the radio mast, a suspension bridge
//
// The street changes character block by block, district by district:
//   0.00  FIFTH AND MAIN        shops under striped awnings, a diner, an arcade; taxis at the kerb
//   1.10  THEATRE ROW           a picture house every other block, its name in lights; the elevated railway behind
//   2.10  THE GARAGES           parking garages and their ramps, service yards, an alley in every gap; the bridge beyond
//   3.13  THE PLAZA             open blocks: monuments ringed with water between glass slabs, the clock tower
//   4.62  WALL STREET           banks with columns, a ticker of lights, dear front doors
//   6.29  PENTHOUSE ROW         canopied doors, pools on the roofs; the Syndicate's banners and black cars begin
//   7.66  SYNDICATE TERRITORY   the finale: black walls with the gold double chevron, barrier rows, checkpoints, watch
//                               towers, banners, headquarters blocks; the Syndicate's needles on the skyline
// The Syndicate's livery (black, a gold double chevron, red lights) is on its own pieces only (SM_Syn*, SM_Checkpoint,
// SM_SearchTower, SM_FarSyndicate) and those stand only from Penthouse Row on.
//
// Near things (the sidewalk, the street wall) are left out of the corkscrews, loops and jumps by the track, and out of
// the tower blocks the road runs through. Each of those stretches has a small district of its own below, where the wall
// is carried through by Far rules (a Far rule's last-but-one number, its drop, is 0.6 m here: the sidewalk's own depth
// below the road). Round a jump the wall stands back to 50 m and more; round a loop everything lit or taller than 2 m
// stands 66 m and more from the loop's ground line (the loop comes down 33 m to the right of where it went up).

// ---------------------------------------------------------------- the street wall: a podium a block (anchor 29 m: its front 25.5 m out,
// its awnings 24.5 m; the track itself moves anything over 2 m tall out to 24 m where the street bends towards it)
Set(TEXT("W_Shops1"),  { { TEXT("SM_Shops1"),      0, 0,    0, 0, 1 } });
Set(TEXT("W_Shops2"),  { { TEXT("SM_Shops2"),      0, 0,    0, 0, 1 } });
Set(TEXT("W_Shops3"),  { { TEXT("SM_Shops3"),      0, 0,    0, 0, 1 } });
Set(TEXT("W_Marquee"), { { TEXT("SM_Marquee"),     0, 0,    0, 0, 1 } });
Set(TEXT("W_Bank"),    { { TEXT("SM_BankFront"),   0, 0.5f, 0, 0, 1 } });
Set(TEXT("W_Pent"),    { { TEXT("SM_PentEntry"),   0, 0.5f, 0, 0, 1 } });
Set(TEXT("W_Garage"),  { { TEXT("SM_GarageRamp"),  0, 2.5f, 0, 0, 1 } });
Set(TEXT("W_Yard"),    { { TEXT("SM_ServiceYard"), 0, 0,    0, 0, 1 } });
Set(TEXT("W_Syn"),     { { TEXT("SM_SynGate"),     0, 0,    0, 0, 1 } });
// what stands in the 13 m between two podiums (half a slot on from the anchor)
Set(TEXT("G_Alley"),    { { TEXT("SM_Alley"),       22.6f,  0,    0, 0, 1 } });
Set(TEXT("G_Monument"), { { TEXT("SM_Monument"),    22.6f,  4.5f, 0, 0, 1 } });
Set(TEXT("G_Check"),    { { TEXT("SM_Checkpoint"),  21.3f, -1.5f, 0, 0, 1 } });
Set(TEXT("G_Search"),   { { TEXT("SM_SearchTower"), 22.6f,  1.5f, 0, 0, 1 } });
Set(TEXT("G_Banner"),   { { TEXT("SM_SynBanner"),   22.6f, -3.5f, 0, 0, 1 } });
// the sidewalk (anchor 17.6 m: the kerb)
Set(TEXT("Kerb1"),    { { TEXT("SM_ParkedCars"),  -10, 0,    0,   0, 1 }, { TEXT("SM_Sidewalk"),      9,    2.9f, 0,   0, 1 } });
Set(TEXT("Kerb2"),    { { TEXT("SM_BusStop"),     -12, 2.4f, 0,   0, 1 }, { TEXT("SM_ParkedCars2"),   8,    0,    0,   0, 1 } });
Set(TEXT("Kerb3"),    { { TEXT("SM_Sidewalk"),    -12, 2.9f, 0,   0, 1 }, { TEXT("SM_ParkedCars2"),   7,    0,    0, 180, 1 } });
Set(TEXT("KerbSyn"),  { { TEXT("SM_SynBarriers"),  -9, 1.5f, 0,   0, 1 }, { TEXT("SM_SynCars"),      12,    0,    0,   0, 1 } });
Set(TEXT("KerbSyn2"), { { TEXT("SM_SynBarriers"), -12, 1.5f, 0,   0, 1 }, { TEXT("SM_SynBarriers"),  12.5f, 1.5f, 0,   0, 1 } });

// ---- 0 to 1.10 km: FIFTH AND MAIN. Three kinds of shop row in turn on each side (out of step with the other side),
// an alley in every other gap, taxis and a bus shelter at the kerb.
Zone(TEXT("Fifth and Main"), 0.f, 1.10f);
	Site(TEXT("Kerb1"), -1, 17.6f, 2, 0, 0.9f);
	Site(TEXT("Kerb2"), -1, 17.6f, 2, 1, 0.9f);
	Site(TEXT("Kerb2"), 1, 17.6f, 2, 0, 0.9f);
	Site(TEXT("Kerb1"), 1, 17.6f, 2, 1, 0.9f);
	Site(TEXT("W_Shops1"), -1, 29, 3, 0, 1.f);
	Site(TEXT("W_Shops3"), -1, 29, 3, 1, 1.f);
	Site(TEXT("W_Shops2"), -1, 29, 3, 2, 1.f);
	Site(TEXT("W_Shops2"), 1, 29, 3, 0, 1.f);
	Site(TEXT("W_Shops1"), 1, 29, 3, 1, 1.f);
	Site(TEXT("W_Shops3"), 1, 29, 3, 2, 1.f);
	Site(TEXT("G_Alley"), -1, 29, 2, 0, 0.9f);
	Site(TEXT("G_Alley"), 1, 29, 2, 1, 0.9f);
	Far(TEXT("SM_MidBlock"), 96, 130, 3, 0, 0.9f, 1.f, 0.1f, 0, 0.7f, 8.f);
	Far(TEXT("SM_GlassBlock"), 104, 150, 4, 1, 0.9f, 1.f, 0.1f, 0, 0.7f, 8.f);
	Far(TEXT("SM_FarTowersA"), 420, 560, 5, 1, 1.f, 1.f, 0.1f, 0, 25.f, 10.f);
	Far(TEXT("SM_FarRadio"), 520, 700, 11, 4, 1.f, 1.f, 0.f, 1, 25.f, 0.f);
	Far(TEXT("SM_FarTowersB"), 640, 860, 7, 3, 1.f, 1.3f, 0.1f, 0, 25.f, 10.f);

// ---- 1.10 to 2.10 km: THEATRE ROW. A picture house every other block on each side (turn about across the street), the
// diner and the arcade between; the elevated railway and its trains behind on the left; deco towers and the bridge beyond.
Zone(TEXT("Theatre Row"), 1.10f, 2.10f);
	Site(TEXT("Kerb3"), -1, 17.6f, 2, 0, 0.9f);
	Site(TEXT("Kerb1"), -1, 17.6f, 2, 1, 0.9f);
	Site(TEXT("Kerb1"), 1, 17.6f, 2, 0, 0.9f);
	Site(TEXT("Kerb3"), 1, 17.6f, 2, 1, 0.9f);
	Site(TEXT("W_Marquee"), -1, 29, 2, 0, 1.f);
	Site(TEXT("W_Shops3"), -1, 29, 4, 1, 1.f);
	Site(TEXT("W_Shops2"), -1, 29, 4, 3, 1.f);
	Site(TEXT("W_Marquee"), 1, 29, 2, 1, 1.f);
	Site(TEXT("W_Shops3"), 1, 29, 4, 2, 1.f);
	Site(TEXT("W_Shops1"), 1, 29, 4, 0, 1.f);
	Site(TEXT("G_Alley"), 0, 29, 3, 0, 0.9f);
	Far(TEXT("SM_GlassBlock"), 96, 130, 3, 1, 0.9f, 1.f, 0.1f, 0, 0.7f, 8.f);
	Far(TEXT("SM_MidBlock"), 104, 150, 4, 2, 0.9f, 1.f, 0.1f, 0, 0.7f, 8.f);
	Far(TEXT("SM_ElRail"), 110, 122, 4, 0, 1.f, 1.f, 0.f, -1, 0.7f, 3.f);
	Far(TEXT("SM_FarTowersB"), 420, 560, 5, 2, 1.f, 1.f, 0.1f, 0, 25.f, 10.f);
	Far(TEXT("SM_FarBridge"), 600, 760, 12, 5, 1.f, 1.f, 0.f, -1, 25.f, 8.f);
	Far(TEXT("SM_FarTowersA"), 640, 860, 7, 4, 1.f, 1.3f, 0.1f, 0, 25.f, 10.f);
Zone(TEXT("Theatre Row corkscrew"), 1.443f, 1.588f);
	Far(TEXT("SM_Marquee"), 31, 31.5f, 2, 0, 1.f, 1.f, 0.f, 0, 0.6f, 2.f);
	Far(TEXT("SM_Shops3"), 31, 31.5f, 2, 1, 1.f, 1.f, 0.f, 0, 0.6f, 2.f);
	Far(TEXT("SM_GlassBlock"), 46, 50, 2, 1, 1.f, 1.f, 0.1f, 0, 0.6f, 4.f);
	Far(TEXT("SM_MidBlock"), 47, 51, 2, 0, 1.f, 1.f, 0.1f, 0, 0.6f, 4.f);

// ---- 2.10 to 3.13 km: THE GARAGES. Parking garages with their ramps up the front and service yards, turn about; an
// alley in every gap; fewer people's things on the sidewalk. The elevated railway both sides; water-tank roofs behind;
// the bridge and the radio mast on the skyline. The jump here goes over the garages' roofs (they stand in its valley).
Zone(TEXT("The Garages"), 2.10f, 3.13f);
	Site(TEXT("Kerb3"), -1, 17.6f, 2, 0, 0.8f);
	Site(TEXT("Kerb1"), 1, 17.6f, 2, 1, 0.8f);
	Site(TEXT("W_Garage"), -1, 29, 2, 0, 1.f);
	Site(TEXT("W_Yard"), -1, 29, 2, 1, 1.f);
	Site(TEXT("W_Yard"), 1, 29, 2, 0, 1.f);
	Site(TEXT("W_Garage"), 1, 29, 2, 1, 1.f);
	Site(TEXT("G_Alley"), -1, 29, 1, 0, 0.9f);
	Site(TEXT("G_Alley"), 1, 29, 1, 0, 0.9f);
	Far(TEXT("SM_ElRail"), 100, 116, 3, 0, 1.f, 1.f, 0.f, -1, 0.7f, 3.f);
	Far(TEXT("SM_ElRail"), 104, 120, 3, 1, 1.f, 1.f, 0.f, 1, 0.7f, 3.f);
	Far(TEXT("SM_MidBlock"), 128, 160, 2, 0, 0.9f, 1.f, 0.15f, 0, 0.7f, 8.f);
	Far(TEXT("SM_FarBridge"), 560, 720, 9, 2, 1.f, 1.f, 0.f, 1, 25.f, 8.f);
	Far(TEXT("SM_FarRadio"), 480, 620, 10, 6, 1.f, 1.f, 0.f, -1, 25.f, 0.f);
	Far(TEXT("SM_FarTowersA"), 420, 560, 5, 3, 1.f, 1.f, 0.1f, 0, 25.f, 10.f);
Zone(TEXT("Garage jump"), 2.665f, 2.81f);
	Far(TEXT("SM_GarageRamp"), 52, 54, 2, 0, 1.f, 1.f, 0.f, 0, 0.62f, 2.f);
	Far(TEXT("SM_MidBlock"), 54, 58, 2, 1, 1.f, 1.f, 0.1f, 0, 0.62f, 4.f);

// ---- 3.13 to 4.62 km: THE PLAZA. Every other block is open: a monument ringed with water in the gaps, cafes and dear
// front doors on the blocks between, glass slabs with pools on their roofs behind, the clock tower.
Zone(TEXT("The Plaza"), 3.13f, 4.62f);
	Site(TEXT("Kerb1"), -1, 17.6f, 3, 0, 0.9f);
	Site(TEXT("Kerb2"), -1, 17.6f, 3, 1, 0.9f);
	Site(TEXT("Kerb2"), 1, 17.6f, 3, 2, 0.9f);
	Site(TEXT("Kerb1"), 1, 17.6f, 3, 1, 0.9f);
	Site(TEXT("W_Shops1"), -1, 29, 4, 0, 1.f);
	Site(TEXT("W_Pent"), -1, 29, 4, 2, 1.f);
	Site(TEXT("W_Shops2"), 1, 29, 4, 1, 1.f);
	Site(TEXT("W_Pent"), 1, 29, 4, 3, 1.f);
	Site(TEXT("G_Monument"), -1, 29, 2, 1, 1.f);
	Site(TEXT("G_Monument"), 1, 29, 2, 0, 1.f);
	Far(TEXT("SM_ClockTower"), 88, 96, 6, 2, 1.f, 1.3f, 0.f, 0, 0.65f, 4.f);
	Far(TEXT("SM_GlassBlock"), 100, 150, 2, 0, 0.9f, 1.f, 0.15f, 0, 0.7f, 8.f);
	Far(TEXT("SM_FarTowersA"), 420, 560, 4, 1, 1.f, 1.f, 0.1f, 0, 25.f, 10.f);
	Far(TEXT("SM_FarRadio"), 500, 640, 13, 6, 1.f, 1.f, 0.f, -1, 25.f, 0.f);
	Far(TEXT("SM_FarTowersB"), 640, 860, 6, 3, 1.f, 1.3f, 0.1f, 0, 25.f, 10.f);
Zone(TEXT("Plaza loop"), 3.208f, 3.488f);
	Far(TEXT("SM_Monument"), 66, 70, 2, 1, 1.f, 1.2f, 0.f, -1, 0.62f, 10.f);
	Far(TEXT("SM_GlassBlock"), 72, 84, 1, 0, 0.9f, 1.f, 0.1f, 0, 0.62f, 4.f);
	Far(TEXT("SM_ClockTower"), 68, 72, 3, 1, 1.f, 1.3f, 0.f, 1, 0.62f, 4.f);

// ---- 4.62 to 6.29 km: WALL STREET. A bank every other block (columns, a pediment and its clock, a ticker of lights),
// dear front doors and a loan shop between them; glass and stone behind; deco towers all along the skyline, the bridge.
Zone(TEXT("Wall Street"), 4.62f, 6.29f);
	Site(TEXT("Kerb3"), -1, 17.6f, 2, 0, 0.9f);
	Site(TEXT("Kerb1"), -1, 17.6f, 2, 1, 0.8f);
	Site(TEXT("Kerb1"), 1, 17.6f, 2, 0, 0.8f);
	Site(TEXT("Kerb3"), 1, 17.6f, 2, 1, 0.9f);
	Site(TEXT("W_Bank"), -1, 29, 2, 0, 1.f);
	Site(TEXT("W_Pent"), -1, 29, 4, 1, 1.f);
	Site(TEXT("W_Shops2"), -1, 29, 4, 3, 1.f);
	Site(TEXT("W_Bank"), 1, 29, 2, 1, 1.f);
	Site(TEXT("W_Pent"), 1, 29, 4, 2, 1.f);
	Site(TEXT("W_Yard"), 1, 29, 4, 0, 1.f);
	Site(TEXT("G_Alley"), 0, 29, 2, 0, 0.8f);
	Far(TEXT("SM_GlassBlock"), 96, 130, 3, 1, 0.9f, 1.f, 0.1f, 0, 0.7f, 8.f);
	Far(TEXT("SM_MidBlock"), 104, 150, 4, 2, 0.9f, 1.f, 0.1f, 0, 0.7f, 8.f);
	Far(TEXT("SM_ClockTower"), 88, 96, 9, 4, 1.f, 1.3f, 0.f, 0, 0.65f, 4.f);
	Far(TEXT("SM_FarTowersB"), 420, 560, 4, 0, 1.f, 1.f, 0.1f, 0, 25.f, 10.f);
	Far(TEXT("SM_FarBridge"), 600, 760, 12, 7, 1.f, 1.f, 0.f, 1, 25.f, 8.f);
	Far(TEXT("SM_FarTowersA"), 640, 860, 6, 2, 1.f, 1.3f, 0.1f, 0, 25.f, 10.f);
Zone(TEXT("Wall Street corkscrew"), 4.701f, 4.846f);
	Far(TEXT("SM_BankFront"), 31, 31.5f, 2, 0, 1.f, 1.f, 0.f, 0, 0.6f, 2.f);
	Far(TEXT("SM_PentEntry"), 31, 31.5f, 2, 1, 1.f, 1.f, 0.f, 0, 0.6f, 2.f);
	Far(TEXT("SM_GlassBlock"), 46, 50, 1, 0, 0.9f, 1.f, 0.1f, 0, 0.6f, 4.f);
Zone(TEXT("Wall Street jump"), 5.696f, 5.841f);
	Far(TEXT("SM_BankFront"), 50, 52, 2, 0, 1.f, 1.f, 0.f, 0, 0.62f, 2.f);
	Far(TEXT("SM_GlassBlock"), 56, 62, 2, 1, 1.f, 1.f, 0.1f, 0, 0.62f, 4.f);

// ---- 6.29 to 7.66 km: PENTHOUSE ROW. Canopied doors with lanterns and clipped trees, block after block; pools and
// landing pads on the roofs behind. The Syndicate shows itself: its banners in the gaps, its black cars at the kerb, a
// headquarters block behind on the left, its needles on the left skyline.
Zone(TEXT("Penthouse Row"), 6.29f, 7.66f);
	Site(TEXT("Kerb3"), -1, 17.6f, 2, 0, 0.9f);
	Site(TEXT("KerbSyn"), -1, 17.6f, 4, 1, 1.f);
	Site(TEXT("Kerb1"), 1, 17.6f, 2, 1, 0.9f);
	Site(TEXT("KerbSyn"), 1, 17.6f, 4, 2, 1.f);
	Site(TEXT("W_Pent"), -1, 29, 3, 0, 1.f);
	Site(TEXT("W_Pent"), -1, 29, 3, 1, 1.f);
	Site(TEXT("W_Bank"), -1, 29, 3, 2, 1.f);
	Site(TEXT("W_Pent"), 1, 29, 3, 1, 1.f);
	Site(TEXT("W_Pent"), 1, 29, 3, 2, 1.f);
	Site(TEXT("W_Marquee"), 1, 29, 3, 0, 1.f);
	Site(TEXT("G_Banner"), 0, 29, 3, 0, 1.f);
	Site(TEXT("G_Monument"), 0, 29, 3, 1, 0.9f);
	Far(TEXT("SM_GlassBlock"), 96, 140, 2, 0, 0.9f, 1.f, 0.15f, 0, 0.7f, 8.f);
	Far(TEXT("SM_SynHQ"), 120, 150, 9, 4, 1.f, 1.f, 0.f, -1, 0.7f, 6.f);
	Far(TEXT("SM_FarSyndicate"), 420, 560, 8, 3, 1.f, 1.f, 0.f, -1, 25.f, 8.f);
	Far(TEXT("SM_FarTowersA"), 420, 560, 5, 1, 1.f, 1.f, 0.1f, 1, 25.f, 10.f);
	Far(TEXT("SM_FarTowersB"), 640, 860, 6, 4, 1.f, 1.3f, 0.1f, 0, 25.f, 10.f);
Zone(TEXT("Penthouse loop"), 6.737f, 6.972f);
	Far(TEXT("SM_GlassBlock"), 72, 84, 1, 0, 0.9f, 1.f, 0.1f, -1, 0.62f, 4.f);
	Far(TEXT("SM_SynHQ"), 80, 90, 3, 0, 1.f, 1.f, 0.f, 1, 0.62f, 6.f);

// ---- 7.66 km to the end: SYNDICATE TERRITORY, the finale. Every other block on each side is the Syndicate's black wall
// with the gold double chevron over a steel shutter; dear doors, a bank, a garage, a yard between them. A checkpoint, a
// watch tower or a pair of banners in every gap. Barrier rows and black cars the whole length of the kerb. Headquarters
// blocks behind, both sides; the Syndicate's needles all round the sky, the city's own towers behind them.
Zone(TEXT("Syndicate Territory"), 7.66f, 99.f);
	Site(TEXT("KerbSyn"), -1, 17.6f, 2, 0, 1.f);
	Site(TEXT("KerbSyn2"), -1, 17.6f, 2, 1, 0.9f);
	Site(TEXT("KerbSyn2"), 1, 17.6f, 2, 0, 0.9f);
	Site(TEXT("KerbSyn"), 1, 17.6f, 2, 1, 1.f);
	Site(TEXT("W_Syn"), -1, 29, 2, 0, 1.f);
	Site(TEXT("W_Pent"), -1, 29, 4, 1, 1.f);
	Site(TEXT("W_Bank"), -1, 29, 8, 3, 1.f);
	Site(TEXT("W_Garage"), -1, 29, 8, 7, 1.f);
	Site(TEXT("W_Syn"), 1, 29, 2, 1, 1.f);
	Site(TEXT("W_Pent"), 1, 29, 4, 2, 1.f);
	Site(TEXT("W_Yard"), 1, 29, 8, 0, 1.f);
	Site(TEXT("W_Shops2"), 1, 29, 8, 4, 1.f);
	Site(TEXT("G_Check"), -1, 29, 3, 0, 1.f);
	Site(TEXT("G_Search"), -1, 29, 3, 1, 1.f);
	Site(TEXT("G_Banner"), -1, 29, 3, 2, 1.f);
	Site(TEXT("G_Banner"), 1, 29, 3, 0, 1.f);
	Site(TEXT("G_Check"), 1, 29, 3, 1, 1.f);
	Site(TEXT("G_Search"), 1, 29, 3, 2, 1.f);
	Far(TEXT("SM_SynHQ"), 96, 150, 4, 0, 1.f, 1.f, 0.1f, 0, 0.7f, 6.f);
	Far(TEXT("SM_GlassBlock"), 100, 150, 4, 2, 0.9f, 1.f, 0.1f, 0, 0.7f, 8.f);
	Far(TEXT("SM_FarSyndicate"), 420, 560, 6, 1, 1.f, 1.f, 0.f, -1, 25.f, 8.f);
	Far(TEXT("SM_FarSyndicate"), 420, 560, 6, 4, 1.f, 1.f, 0.f, 1, 25.f, 8.f);
	Far(TEXT("SM_FarTowersB"), 640, 860, 7, 2, 1.f, 1.4f, 0.1f, 0, 25.f, 10.f);
	Far(TEXT("SM_FarTowersA"), 660, 880, 9, 5, 1.f, 1.4f, 0.1f, 0, 25.f, 10.f);
	Far(TEXT("SM_FarRadio"), 560, 760, 23, 9, 1.f, 1.f, 0.f, -1, 25.f, 0.f);
