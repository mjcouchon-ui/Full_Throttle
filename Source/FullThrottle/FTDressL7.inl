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

// SECOND PASS (15 Nov 2026):
//   - SIDE STREETS: in every gap between two podiums that holds no alley, monument or checkpoint, a street runs away
//     from the avenue for 50 m (SM_FloorCross: a zebra crossing at its mouth, cars parked along it). Flat, so it may
//     start at the kerb;
//   - THE KERB, brighter: pavement cafes under striped parasols and road works in orange and white, in turn with the
//     cars and benches (all of it under 2 m and unlit, like everything on the sidewalk);
//   - THE ELEVATED RAILWAY is no longer lengths strewn along the avenue (a line beside a road this twisted folds over
//     itself): each "ElLine" is three lengths end to end running AWAY from the avenue, 66 to 246 m out, a station at
//     its near end, like the cross-town line it would be;
//   - THE FLOOR: parking lots and corporate plazas by Far rules ("Floor ..." districts at the end), kept to the level
//     stretches. The sidewalk lies 0.4 m under the road from 27 m out: a floor piece's drop is that plus 10 cm;
//   - THE BANKS: beside a bend banked 52 degrees or more the land is cut down to the road's sunken inner edge, and that
//     cut fills a third of the frame. SM_BankTiles and SM_SynBank are faces built to lie on it; the "Bank..." sets stand
//     them with a negative "up", one where the bank is at its full angle (the "Bend ..." districts at the end, one
//     slot each, from the track's section list). The Syndicate's black and gold from 7 km on, the city's tiles before.

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

// second pass: a side street in a gap (it starts at the kerb, 19 m out), the brighter kerb, a whole elevated line
// (anchor 96 m: 66 to 246 m out), the bank faces (anchor and size go with the bank's angle; "up" is from the sidewalk)
Set(TEXT("G_Street"),  { { TEXT("SM_FloorCross"), 22.6f, 15, 0, 0, 1 } });
Set(TEXT("KerbCafe"),  { { TEXT("SM_KerbCafe"),   -8, 3.4f, 0, 0, 1 }, { TEXT("SM_ParkedCars"),  12, 0, 0,   0, 1 } });
Set(TEXT("KerbWorks"), { { TEXT("SM_KerbWorks"),  -6, 2.6f, 0, 0, 1 }, { TEXT("SM_ParkedCars2"), 13, 0, 0, 180, 1 } });
Set(TEXT("ElLine"),    { { TEXT("SM_ElRail"), 0, 0, 0, -90, 1 }, { TEXT("SM_ElRail"), 0, 60, 0, -90, 1 }, { TEXT("SM_ElRail"), 0, 120, 0, 90, 1 } });
Set(TEXT("Bank10T"), { { TEXT("SM_BankTiles"), 0, 0, -5.82f, 0, 1 } });     // anchor 12.1 m
Set(TEXT("Bank10S"), { { TEXT("SM_SynBank"),   0, 0, -5.82f, 0, 1 } });
Set(TEXT("Bank09T"), { { TEXT("SM_BankTiles"), 0, 0, -5.814f, 0, 1 } });    // anchor 12.72 m, size 0.9
Set(TEXT("Bank09S"), { { TEXT("SM_SynBank"),   0, 0, -5.814f, 0, 1 } });
// a second face beside the first, one length round the bend (a set for each radius and side: r86p = an 86 m bend, the
// second face on the first one's +along side)
Set(TEXT("Bank09T_r93p"), { { TEXT("SM_BankTiles"), 24.004f, 3.27f, -5.814f, 15.516f, 1 } });
Set(TEXT("Bank10S_r86m"), { { TEXT("SM_SynBank"), -23.861f, 3.976f, -5.82f, -18.92f, 1 } });
Set(TEXT("Bank10S_r86p"), { { TEXT("SM_SynBank"), 23.861f, 3.976f, -5.82f, 18.92f, 1 } });
Set(TEXT("Bank10T_r69m"), { { TEXT("SM_BankTiles"), -23.556f, 5.15f, -5.82f, -24.664f, 1 } });
Set(TEXT("Bank10T_r73m"), { { TEXT("SM_BankTiles"), -23.67f, 4.75f, -5.82f, -22.695f, 1 } });
Set(TEXT("Bank10T_r73p"), { { TEXT("SM_BankTiles"), 23.67f, 4.75f, -5.82f, 22.695f, 1 } });
Set(TEXT("Bank10T_r79p"), { { TEXT("SM_BankTiles"), 23.771f, 4.359f, -5.82f, 20.782f, 1 } });
Set(TEXT("Bank10T_r86m"), { { TEXT("SM_BankTiles"), -23.861f, 3.976f, -5.82f, -18.92f, 1 } });

// ---- 0 to 1.10 km: FIFTH AND MAIN. Three kinds of shop row in turn on each side (out of step with the other side),
// an alley in every other gap, taxis and a bus shelter at the kerb.
Zone(TEXT("Fifth and Main"), 0.f, 1.10f);
	Site(TEXT("Kerb1"), -1, 17.6f, 3, 0, 0.9f);
	Site(TEXT("KerbCafe"), -1, 17.6f, 3, 1, 0.9f);
	Site(TEXT("Kerb2"), -1, 17.6f, 3, 2, 0.9f);
	Site(TEXT("Kerb2"), 1, 17.6f, 3, 0, 0.9f);
	Site(TEXT("Kerb1"), 1, 17.6f, 3, 1, 0.9f);
	Site(TEXT("KerbCafe"), 1, 17.6f, 3, 2, 0.9f);
	Site(TEXT("W_Shops1"), -1, 29, 3, 0, 1.f);
	Site(TEXT("W_Shops3"), -1, 29, 3, 1, 1.f);
	Site(TEXT("W_Shops2"), -1, 29, 3, 2, 1.f);
	Site(TEXT("W_Shops2"), 1, 29, 3, 0, 1.f);
	Site(TEXT("W_Shops1"), 1, 29, 3, 1, 1.f);
	Site(TEXT("W_Shops3"), 1, 29, 3, 2, 1.f);
	Site(TEXT("G_Alley"), -1, 29, 2, 0, 0.9f);
	Site(TEXT("G_Alley"), 1, 29, 2, 1, 0.9f);
	Site(TEXT("G_Street"), -1, 29, 2, 1, 1.f);
	Site(TEXT("G_Street"), 1, 29, 2, 0, 1.f);
	Far(TEXT("SM_MidBlock"), 96, 130, 3, 0, 0.9f, 1.f, 0.1f, 0, 0.7f, 8.f);
	Far(TEXT("SM_GlassBlock"), 104, 150, 4, 1, 0.9f, 1.f, 0.1f, 0, 0.7f, 8.f);
	Far(TEXT("SM_FarTowersA"), 420, 560, 5, 1, 1.f, 1.f, 0.1f, 0, 25.f, 10.f);
	Far(TEXT("SM_FarRadio"), 520, 700, 11, 4, 1.f, 1.f, 0.f, 1, 25.f, 0.f);
	Far(TEXT("SM_FarTowersB"), 640, 860, 7, 3, 1.f, 1.3f, 0.1f, 0, 25.f, 10.f);

// ---- 1.10 to 2.10 km: THEATRE ROW. A picture house every other block on each side (turn about across the street), the
// diner and the arcade between; the elevated railway and its trains behind on the left; deco towers and the bridge beyond.
Zone(TEXT("Theatre Row"), 1.10f, 2.10f);
	Site(TEXT("Kerb3"), -1, 17.6f, 3, 0, 0.9f);
	Site(TEXT("KerbCafe"), -1, 17.6f, 3, 1, 0.9f);
	Site(TEXT("Kerb1"), -1, 17.6f, 3, 2, 0.9f);
	Site(TEXT("Kerb1"), 1, 17.6f, 3, 0, 0.9f);
	Site(TEXT("Kerb3"), 1, 17.6f, 3, 1, 0.9f);
	Site(TEXT("KerbCafe"), 1, 17.6f, 3, 2, 0.9f);
	Site(TEXT("W_Marquee"), -1, 29, 2, 0, 1.f);
	Site(TEXT("W_Shops3"), -1, 29, 4, 1, 1.f);
	Site(TEXT("W_Shops2"), -1, 29, 4, 3, 1.f);
	Site(TEXT("W_Marquee"), 1, 29, 2, 1, 1.f);
	Site(TEXT("W_Shops3"), 1, 29, 4, 2, 1.f);
	Site(TEXT("W_Shops1"), 1, 29, 4, 0, 1.f);
	Site(TEXT("G_Alley"), 0, 29, 3, 0, 0.9f);
	Site(TEXT("G_Street"), -1, 29, 3, 1, 1.f);
	Site(TEXT("G_Street"), 1, 29, 3, 2, 1.f);
	Far(TEXT("SM_GlassBlock"), 96, 130, 3, 1, 0.9f, 1.f, 0.1f, 0, 0.7f, 8.f);
	Far(TEXT("SM_MidBlock"), 104, 150, 4, 2, 0.9f, 1.f, 0.1f, 0, 0.7f, 8.f);
	Site(TEXT("ElLine"), -1, 96, 5, 0, 1.f);
	Site(TEXT("ElLine"), 1, 96, 5, 2, 1.f);
	Far(TEXT("SM_FarTowersB"), 420, 560, 5, 2, 1.f, 1.f, 0.1f, 0, 25.f, 10.f);
	Far(TEXT("SM_FarBridge"), 600, 760, 12, 5, 1.f, 1.f, 0.f, -1, 25.f, 8.f);
	Far(TEXT("SM_FarTowersA"), 640, 860, 7, 4, 1.f, 1.3f, 0.1f, 0, 25.f, 10.f);
Zone(TEXT("Theatre Row corkscrew"), 1.443f, 1.588f);
	Far(TEXT("SM_Marquee"), 31, 31.5f, 2, 0, 1.f, 1.f, 0.f, 0, 0.6f, 2.f);
	Far(TEXT("SM_Shops3"), 31, 31.5f, 2, 1, 1.f, 1.f, 0.f, 0, 0.6f, 2.f);
	Far(TEXT("SM_GlassBlock"), 46, 50, 2, 1, 1.f, 1.f, 0.1f, 0, 0.6f, 4.f);
	Far(TEXT("SM_MidBlock"), 47, 51, 2, 0, 1.f, 1.f, 0.1f, 0, 0.6f, 4.f);

// ---- 2.10 to 3.13 km: THE GARAGES. Parking garages with their ramps up the front and service yards, turn about; an
// alley in every gap; road works on the sidewalk. The elevated railway's lines either side; water-tank roofs behind;
// the bridge and the radio mast on the skyline. The jump here goes over the garages' roofs (they stand in its valley).
Zone(TEXT("The Garages"), 2.10f, 3.13f);
	Site(TEXT("Kerb3"), -1, 17.6f, 2, 0, 0.8f);
	Site(TEXT("Kerb1"), 1, 17.6f, 2, 1, 0.8f);
	Site(TEXT("KerbWorks"), -1, 17.6f, 2, 1, 0.7f);
	Site(TEXT("KerbWorks"), 1, 17.6f, 2, 0, 0.7f);
	Site(TEXT("W_Garage"), -1, 29, 2, 0, 1.f);
	Site(TEXT("W_Yard"), -1, 29, 2, 1, 1.f);
	Site(TEXT("W_Yard"), 1, 29, 2, 0, 1.f);
	Site(TEXT("W_Garage"), 1, 29, 2, 1, 1.f);
	Site(TEXT("G_Alley"), -1, 29, 1, 0, 0.9f);
	Site(TEXT("G_Alley"), 1, 29, 1, 0, 0.9f);
	Site(TEXT("ElLine"), -1, 96, 4, 0, 1.f);
	Site(TEXT("ElLine"), 1, 96, 4, 2, 1.f);
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
	Site(TEXT("KerbCafe"), -1, 17.6f, 3, 2, 0.9f);
	Site(TEXT("KerbCafe"), 1, 17.6f, 3, 0, 0.9f);
	Site(TEXT("W_Shops1"), -1, 29, 4, 0, 1.f);
	Site(TEXT("W_Pent"), -1, 29, 4, 2, 1.f);
	Site(TEXT("W_Shops2"), 1, 29, 4, 1, 1.f);
	Site(TEXT("W_Pent"), 1, 29, 4, 3, 1.f);
	Site(TEXT("G_Monument"), -1, 29, 2, 1, 1.f);
	Site(TEXT("G_Monument"), 1, 29, 2, 0, 1.f);
	Site(TEXT("G_Street"), -1, 29, 2, 0, 1.f);
	Site(TEXT("G_Street"), 1, 29, 2, 1, 1.f);
	Far(TEXT("SM_ClockTower"), 88, 96, 6, 2, 1.f, 1.3f, 0.f, 0, 0.65f, 4.f);
	Far(TEXT("SM_GlassBlock"), 100, 150, 2, 0, 0.9f, 1.f, 0.15f, 0, 0.7f, 8.f);
	Far(TEXT("SM_FarTowersA"), 420, 560, 4, 1, 1.f, 1.f, 0.1f, 0, 25.f, 10.f);
	Far(TEXT("SM_FarRadio"), 500, 640, 13, 6, 1.f, 1.f, 0.f, -1, 25.f, 0.f);
	Far(TEXT("SM_FarTowersB"), 640, 860, 6, 3, 1.f, 1.3f, 0.1f, 0, 25.f, 10.f);
Zone(TEXT("Plaza loop"), 3.208f, 3.488f);
	Far(TEXT("SM_Monument"), 66, 70, 2, 1, 1.f, 1.2f, 0.f, -1, 0.62f, 10.f);
	Far(TEXT("SM_GlassBlock"), 72, 84, 1, 0, 0.9f, 1.f, 0.1f, 0, 0.62f, 4.f);
	Far(TEXT("SM_ClockTower"), 68, 72, 3, 1, 1.f, 1.3f, 0.f, 1, 0.62f, 4.f);
	Far(TEXT("SM_FloorPlaza"), 38, 44, 2, 0, 1.f, 1.2f, 0.f, -1, 0.5f, 0.f);        // second pass: the open side of the loop
	Far(TEXT("SM_FloorLot"), 66, 72, 2, 1, 1.f, 1.f, 0.f, 1, 0.53f, 4.f);

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
	Site(TEXT("G_Street"), -1, 29, 4, 1, 1.f);
	Site(TEXT("G_Street"), 1, 29, 4, 3, 1.f);
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
	Site(TEXT("KerbCafe"), -1, 17.6f, 4, 3, 0.9f);
	Site(TEXT("KerbCafe"), 1, 17.6f, 4, 0, 0.9f);
	Site(TEXT("W_Pent"), -1, 29, 3, 0, 1.f);
	Site(TEXT("W_Pent"), -1, 29, 3, 1, 1.f);
	Site(TEXT("W_Bank"), -1, 29, 3, 2, 1.f);
	Site(TEXT("W_Pent"), 1, 29, 3, 1, 1.f);
	Site(TEXT("W_Pent"), 1, 29, 3, 2, 1.f);
	Site(TEXT("W_Marquee"), 1, 29, 3, 0, 1.f);
	Site(TEXT("G_Banner"), 0, 29, 3, 0, 1.f);
	Site(TEXT("G_Monument"), 0, 29, 3, 1, 0.9f);
	Site(TEXT("G_Street"), -1, 29, 6, 2, 1.f);
	Site(TEXT("G_Street"), 1, 29, 6, 5, 1.f);
	Far(TEXT("SM_GlassBlock"), 96, 140, 2, 0, 0.9f, 1.f, 0.15f, 0, 0.7f, 8.f);
	Far(TEXT("SM_SynHQ"), 120, 150, 9, 4, 1.f, 1.f, 0.f, -1, 0.7f, 6.f);
	Far(TEXT("SM_FarSyndicate"), 420, 560, 8, 3, 1.f, 1.f, 0.f, -1, 25.f, 8.f);
	Far(TEXT("SM_FarTowersA"), 420, 560, 5, 1, 1.f, 1.f, 0.1f, 1, 25.f, 10.f);
	Far(TEXT("SM_FarTowersB"), 640, 860, 6, 4, 1.f, 1.3f, 0.1f, 0, 25.f, 10.f);
Zone(TEXT("Penthouse loop"), 6.737f, 6.972f);
	Far(TEXT("SM_GlassBlock"), 72, 84, 1, 0, 0.9f, 1.f, 0.1f, -1, 0.62f, 4.f);
	Far(TEXT("SM_SynHQ"), 80, 90, 3, 0, 1.f, 1.f, 0.f, 1, 0.62f, 6.f);
	Far(TEXT("SM_FloorPlaza"), 38, 44, 2, 0, 1.f, 1.2f, 0.f, -1, 0.5f, 0.f);
	Far(TEXT("SM_FloorLot"), 66, 72, 2, 1, 1.f, 1.f, 0.f, 1, 0.53f, 4.f);

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

// ================================================================ SECOND PASS: THE FLOOR
// Parking lots between the towers of the second row and plazas behind them, by Far rules (they stay through the loops
// and corkscrews), on the level stretches only: these districts stop at both rises and both jumps.
const auto CityFloor = [&]()
{
	Far(TEXT("SM_FloorLot"), 56, 62, 3, 1, 0.9f, 1.f, 0.f, 0, 0.52f, 4.f);
	Far(TEXT("SM_FloorPlaza"), 86, 96, 4, 2, 0.9f, 1.f, 0.1f, 0, 0.54f, 0.f);
};
Zone(TEXT("Floor: to the garage jump"), 0.f, 2.52f);
	CityFloor();
Zone(TEXT("Floor: the plaza and wall street"), 2.89f, 5.46f);
	CityFloor();
	Far(TEXT("SM_FloorPlaza"), 54, 60, 6, 3, 1.f, 1.f, 0.f, 0, 0.52f, 0.f);
Zone(TEXT("Floor: penthouse row and the finale"), 5.95f, 99.f);
	CityFloor();

// ================================================================ SECOND PASS: BLADE SIGNS
// On the towers' fronts over the podiums, read along the street: lit, steady, their lights 30 m out and more. None in
// the Garages (no night trade there) and none in Syndicate Territory (nothing lit is added at the Chairman's height).
Zone(TEXT("Signs: Fifth and Main"), 0.f, 1.1f);
	Scatter(TEXT("SM_BladeHotel"), 31.8f, 31.8f, 4, 1, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_BladeDiner"), 31.8f, 31.8f, 4, 3, 1.f, 1.f, 0.f, 0, 0.f);
Zone(TEXT("Signs: Theatre Row"), 1.1f, 2.1f);
	Scatter(TEXT("SM_BladeClub"), 31.8f, 31.8f, 2, 0, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_BladeHotel"), 31.8f, 31.8f, 4, 1, 1.f, 1.f, 0.f, 0, 0.f);
Zone(TEXT("Signs: the Plaza"), 3.13f, 4.62f);
	Scatter(TEXT("SM_BladeHotel"), 31.8f, 31.8f, 4, 0, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_BladeClub"), 31.8f, 31.8f, 6, 3, 1.f, 1.f, 0.f, 0, 0.f);
Zone(TEXT("Signs: Wall Street"), 4.62f, 6.29f);
	Scatter(TEXT("SM_BladeClub"), 31.8f, 31.8f, 5, 2, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_BladeDiner"), 31.8f, 31.8f, 5, 4, 1.f, 1.f, 0.f, 0, 0.f);
Zone(TEXT("Signs: Penthouse Row"), 6.29f, 7.6f);
	Scatter(TEXT("SM_BladeHotel"), 31.8f, 31.8f, 3, 0, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_BladeClub"), 31.8f, 31.8f, 3, 1, 1.f, 1.f, 0.f, 0, 0.f);

// ================================================================ SECOND PASS: THE BANKED BENDS
// One district a slot: the bank's face on the bend's INSIDE where the bank is at its full angle for the face's whole
// length (24 m). From the track's section list; the track is locked.
Zone(TEXT("Bend 0.95"), 0.9471f, 0.9531f); Site(TEXT("Bank10T"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10T_r86m"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 1.04"), 1.0376f, 1.0436f); Site(TEXT("Bank10T"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 1.09"), 1.0828f, 1.0888f); Site(TEXT("Bank10T"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 1.67"), 1.6710f, 1.6770f); Site(TEXT("Bank10T"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 1.72"), 1.7162f, 1.7222f); Site(TEXT("Bank10T"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 2.31"), 2.3044f, 2.3104f); Site(TEXT("Bank10T"), -1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10T_r73p"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 2.40"), 2.3949f, 2.4009f); Site(TEXT("Bank10T"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 2.44"), 2.4401f, 2.4461f); Site(TEXT("Bank10T"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 3.66"), 3.6617f, 3.6677f); Site(TEXT("Bank10T"), -1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10T_r79p"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 3.80"), 3.7974f, 3.8034f); Site(TEXT("Bank10T"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10T_r79p"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 4.34"), 4.3403f, 4.3463f); Site(TEXT("Bank10T"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 4.39"), 4.3856f, 4.3916f); Site(TEXT("Bank10T"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 4.57"), 4.5665f, 4.5725f); Site(TEXT("Bank10T"), -1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10T_r69m"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 4.93"), 4.9285f, 4.9345f); Site(TEXT("Bank09T"), 1, 12.72f, 1, 0, 1.f, 0.9f); Site(TEXT("Bank09T_r93p"), 1, 12.72f, 1, 0, 1.f, 0.9f);
Zone(TEXT("Bend 5.07"), 5.0642f, 5.0702f); Site(TEXT("Bank09T"), -1, 12.72f, 1, 0, 1.f, 0.9f); Site(TEXT("Bank09T_r93p"), -1, 12.72f, 1, 0, 1.f, 0.9f);
Zone(TEXT("Bend 6.11"), 6.1048f, 6.1108f); Site(TEXT("Bank10T"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10T_r73p"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 6.24"), 6.2405f, 6.2465f); Site(TEXT("Bank10T"), -1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10T_r73m"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 7.15"), 7.1454f, 7.1514f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86m"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 7.28"), 7.2811f, 7.2871f); Site(TEXT("Bank10S"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86p"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 13.80"), 13.7961f, 13.8021f); Site(TEXT("Bank10S"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86p"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 13.93"), 13.9318f, 13.9378f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86m"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 14.66"), 14.6557f, 14.6617f); Site(TEXT("Bank10S"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86m"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 14.75"), 14.7462f, 14.7522f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 14.79"), 14.7915f, 14.7975f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 15.47"), 15.4701f, 15.4761f); Site(TEXT("Bank10S"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86p"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 15.61"), 15.6058f, 15.6118f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 16.33"), 16.3297f, 16.3357f); Site(TEXT("Bank10S"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86m"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 16.42"), 16.4202f, 16.4262f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 16.47"), 16.4655f, 16.4715f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 17.15"), 17.1441f, 17.1501f); Site(TEXT("Bank10S"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86p"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 17.28"), 17.2798f, 17.2858f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 18.01"), 18.0037f, 18.0097f); Site(TEXT("Bank10S"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86m"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 18.10"), 18.0942f, 18.1002f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 18.14"), 18.1394f, 18.1454f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 18.82"), 18.8181f, 18.8241f); Site(TEXT("Bank10S"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86p"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 18.96"), 18.9538f, 18.9598f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 19.68"), 19.6777f, 19.6837f); Site(TEXT("Bank10S"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86m"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 19.77"), 19.7682f, 19.7742f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 19.82"), 19.8134f, 19.8194f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 20.50"), 20.4921f, 20.4981f); Site(TEXT("Bank10S"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86p"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 20.63"), 20.6278f, 20.6338f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86p"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 21.35"), 21.3517f, 21.3577f); Site(TEXT("Bank10S"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86m"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 21.45"), 21.4422f, 21.4482f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86m"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 22.17"), 22.1661f, 22.1721f); Site(TEXT("Bank10S"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86p"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 22.30"), 22.3018f, 22.3078f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86p"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 23.03"), 23.0257f, 23.0317f); Site(TEXT("Bank10S"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86m"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 23.12"), 23.1162f, 23.1222f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86m"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 23.84"), 23.8401f, 23.8461f); Site(TEXT("Bank10S"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86p"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 23.98"), 23.9758f, 23.9818f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86p"), -1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 24.70"), 24.6997f, 24.7057f); Site(TEXT("Bank10S"), 1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86m"), 1, 12.1f, 1, 0, 1.f, 1);
Zone(TEXT("Bend 24.79"), 24.7902f, 24.7962f); Site(TEXT("Bank10S"), -1, 12.1f, 1, 0, 1.f, 1); Site(TEXT("Bank10S_r86m"), -1, 12.1f, 1, 0, 1.f, 1);
