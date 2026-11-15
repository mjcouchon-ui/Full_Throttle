// 3.2 dressing table, level 7: City Center (Midtown). The words (Set, Zone, Scatter, Site, Far, SiteAfloat, Bank) are explained at the top of
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
//     cut fills a third of the frame. SM_BankTiles and SM_SynBank WERE faces built to lie on it (gone since the third pass: see below); the "Bank..." sets stood
//     them with a negative "up", one where the bank is at its full angle (the "Bend ..." districts at the end, one
//     slot each, from the track's section list). The Syndicate's black and gold from 7 km on, the city's tiles before.
//
// THIRD PASS (15 Nov 2026, the overhaul: tight canyons, lit storefronts, signs that move, traffic on its own paths):
//   - THE BANKS are faced by the track itself: a Bank rule ("Cut banks" below: SM_CutTiles to 7 km, SM_CutSyn after,
//     SM_CutCity on the shallow cuttings) instead of the two tilted faces, which stood only where a bend held its
//     full bank. Those faces and their one-slot "Bend" districts are gone;
//   - THE CANYON: behind every alley stands a narrow brick building 34 m tall (SM_Infill1, in the G_Alley set), and
//     the gaps that held nothing in Theatre Row, Wall Street and Penthouse Row are closed by a glass one flush with
//     the podiums (G_Fill: SM_Infill2). Their windows go on and off; something turns on each roof;
//   - SIGNS: the blade signs stand 26 m out now, at the podiums' own line (at 31.8 m most were inside the level's
//     towers), each district has its own words, and their letters light one after the other. Signs on the podiums'
//     roofs chase and swell, steam comes off the roofs, a picture house has a searchlight on its roof: all of these
//     are in the sets the SIX districts before Syndicate Territory use ("W_Shops2s", "W_Banks", "W_Yards": the same
//     podium with the roof's sign or steam). The finale keeps the plain sets: nothing near the road there blinks,
//     chases or turns at the height the Chairman's shots fly;
//   - (OFF since round 2, see the districts) A SIGN AT THE END OF EVERY STREET: on the outside of each hairpin before the finale, dead ahead as the street
//     runs up to the bend, a roof sign at three times its size on its legs (28 m of lights), turned to face the street;
//   - TRAFFIC ON ITS OWN PATHS: the elevated train runs every ElLine (SM_ElTrain), cars drive away down every side
//     street (SM_CrossTraffic), lights cross the far bridge, an airship goes over (SM_Blimp, 125 m up). And under the
//     flights (OFF since round 2: not seen from the air): THE GARAGE JUMP goes over the elevated railway, its train running under the car; THE WALL STREET
//     JUMP over a boulevard of traffic. Both are afloat sets, stood from the level's floor (FTBED's "floor z"
//     -1637: if the track's heights ever change they must be laid out again);
//   - tower cranes over the Garages and Wall Street, searchlights round both loops, the Syndicate's gate (two
//     headquarters towers at half size, one each side) where its territory begins;
//   - THE HORIZON: two rows of flat skyline cards (SM_SkyCard1 at 0.9 to 1.1 km, SM_SkyCard2 at 1.5 to 1.8 km).
// This pass's own districts are at the END of the table (a rule added in the middle moves the dice of every rule
// after it); in place, only numbers and set names were changed.

// ---------------------------------------------------------------- the street wall: a podium a block (anchor 29 m: its front 25.5 m out,
// its awnings 24.5 m; the track itself moves anything over 2 m tall out to 24 m where the street bends towards it)
Set(TEXT("W_Shops1"),  { { TEXT("SM_Shops1"),      0, 0,    0, 0, 1 }, { TEXT("SM_RoofSign1"), -10.7f, 1.4f, 5.5f, 0, 1 }, { TEXT("/Game/Maps/Metro/Life/SM_SteamGrate"), 10.7f, 1.2f, 5.5f, 0, 1 } });   // third pass: JAZZ in lights on one low roof, steam off the other
Set(TEXT("W_Shops2"),  { { TEXT("SM_Shops2"),      0, 0,    0, 0, 1 } });
Set(TEXT("W_Shops3"),  { { TEXT("SM_Shops3"),      0, 0,    0, 0, 1 }, { TEXT("SM_RoofSign3"), 11.5f, 1.4f, 6.f, 0, 1 } });       // third pass: FILM over the pawn shop
Set(TEXT("W_Marquee"), { { TEXT("SM_Marquee"),     0, 0,    0, 0, 1 }, { TEXT("SM_Klieg"), 12, 1.7f, 11.9f, 0, 1 } });           // third pass: a premiere searchlight on its roof
Set(TEXT("W_Bank"),    { { TEXT("SM_BankFront"),   0, 0.5f, 0, 0, 1 } });
Set(TEXT("W_Pent"),    { { TEXT("SM_PentEntry"),   0, 0.5f, 0, 0, 1 } });
Set(TEXT("W_Garage"),  { { TEXT("SM_GarageRamp"),  0, 2.5f, 0, 0, 1 } });
Set(TEXT("W_Yard"),    { { TEXT("SM_ServiceYard"), 0, 0,    0, 0, 1 } });
Set(TEXT("W_Syn"),     { { TEXT("SM_SynGate"),     0, 0,    0, 0, 1 } });
// what stands in the 13 m between two podiums (half a slot on from the anchor)
Set(TEXT("G_Alley"),    { { TEXT("SM_Alley"),       22.6f,  0,    0, 0, 1 }, { TEXT("SM_Infill1"), 22.6f, 4.9f, 0, 0, 1 } });     // third pass: the building behind the alley closes the gap to 34 m
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
// (anchor 96 m: 66 to 246 m out). The bank faces that were set here are gone (third pass: the track faces the cuttings)
Set(TEXT("G_Street"),  { { TEXT("SM_FloorCross"), 22.6f, 15, 0, 0, 1 }, { TEXT("SM_CrossTraffic"), 22.6f, 15, 0, 0, 1 } });      // third pass: cars drive away down it
Set(TEXT("KerbCafe"),  { { TEXT("SM_KerbCafe"),   -8, 3.4f, 0, 0, 1 }, { TEXT("SM_ParkedCars"),  12, 0, 0,   0, 1 } });
Set(TEXT("KerbWorks"), { { TEXT("SM_KerbWorks"),  -6, 2.6f, 0, 0, 1 }, { TEXT("SM_ParkedCars2"), 13, 0, 0, 180, 1 } });
Set(TEXT("ElLine"),    { { TEXT("SM_ElRail"), 0, 0, 0, -90, 1 }, { TEXT("SM_ElRail"), 0, 60, 0, -90, 1 }, { TEXT("SM_ElRail"), 0, 120, 0, 90, 1 }, { TEXT("SM_ElTrain"), 0, 60, 0, -90, 1 } });   // third pass: its train runs the line
// THIRD PASS: the same podiums with what the six districts before the finale carry on their roofs (signs that chase,
// steam); the gap closed flush with the podiums; a roof sign at three times its size on the outside of a hairpin, turned
// to face the street that runs up to it (L: outside a right-hand bend, on the left; R: the other way); the Syndicate's
// gate; and the two sets under the jumps, afloat ("up" is metres over the level's floor: the valley under the flight)
Set(TEXT("W_Shops2s"), { { TEXT("SM_Shops2"),      0, 0,    0, 0, 1 }, { TEXT("SM_RoofSign2"), 0, 1.4f, 5.5f, 0, 1 } });
Set(TEXT("W_Banks"),   { { TEXT("SM_BankFront"),   0, 0.5f, 0, 0, 1 }, { TEXT("SM_RoofSign4"), 10.5f, 1.4f, 10.4f, 0, 1 } });
Set(TEXT("W_Yards"),   { { TEXT("SM_ServiceYard"), 0, 0,    0, 0, 1 }, { TEXT("/Game/Maps/Metro/Life/SM_SteamGrate"), 4, 1.7f, 6.7f, 0, 1 } });
Set(TEXT("G_Fill"),    { { TEXT("SM_Infill2"),     22.6f, -2.2f, 0, 0, 1 } });
Set(TEXT("BigJazzL"), { { TEXT("SM_RoofSign1"), 0, 0, 0,  25, 3 } });   Set(TEXT("BigJazzR"), { { TEXT("SM_RoofSign1"), 0, 0, 0, -25, 3 } });
Set(TEXT("BigEatL"), { { TEXT("SM_RoofSign2"), 0, 0, 0,  25, 3 } });   Set(TEXT("BigEatR"), { { TEXT("SM_RoofSign2"), 0, 0, 0, -25, 3 } });
Set(TEXT("BigFilmL"), { { TEXT("SM_RoofSign3"), 0, 0, 0,  25, 3 } });   Set(TEXT("BigFilmR"), { { TEXT("SM_RoofSign3"), 0, 0, 0, -25, 3 } });
Set(TEXT("BigCashL"), { { TEXT("SM_RoofSign4"), 0, 0, 0,  25, 3 } });   Set(TEXT("BigCashR"), { { TEXT("SM_RoofSign4"), 0, 0, 0, -25, 3 } });
Set(TEXT("SynGatePair"), { { TEXT("SM_SynHQ"), 0, 0, 0, 0, 0.55f } });
Set(TEXT("ElUnder"), { { TEXT("SM_ElRail"), -13.823f, -60, 5.422f, -90, 1 }, { TEXT("SM_ElRail"), -13.823f, 0, 5.422f, -90, 1 }, { TEXT("SM_ElRail"), -13.823f, 60, 5.422f, 90, 1 }, { TEXT("SM_ElTrain"), -13.823f, 0, 5.422f, -90, 1 } });
Set(TEXT("BlvdUnder"), { { TEXT("SM_FloorCross"), -11.104f, -42, 12.015f, 0, 1.6f }, { TEXT("SM_FloorCross"), -11.104f, 38, 12.015f, 0, 1.6f }, { TEXT("SM_CrossTraffic"), -11.104f, -42, 12.015f, 0, 1.6f }, { TEXT("SM_CrossTraffic"), -11.104f, 38, 12.015f, 0, 1.6f } });

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
	Site(TEXT("W_Shops2s"), -1, 29, 3, 2, 1.f);
	Site(TEXT("W_Shops2s"), 1, 29, 3, 0, 1.f);
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
	Site(TEXT("W_Shops2s"), -1, 29, 4, 3, 1.f);
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
	Site(TEXT("W_Yards"), -1, 29, 2, 1, 1.f);
	Site(TEXT("W_Yards"), 1, 29, 2, 0, 1.f);
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
	Site(TEXT("W_Shops2s"), 1, 29, 4, 1, 1.f);
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
	Site(TEXT("W_Banks"), -1, 29, 2, 0, 1.f);
	Site(TEXT("W_Pent"), -1, 29, 4, 1, 1.f);
	Site(TEXT("W_Shops2s"), -1, 29, 4, 3, 1.f);
	Site(TEXT("W_Banks"), 1, 29, 2, 1, 1.f);
	Site(TEXT("W_Pent"), 1, 29, 4, 2, 1.f);
	Site(TEXT("W_Yards"), 1, 29, 4, 0, 1.f);
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
	Site(TEXT("W_Banks"), -1, 29, 3, 2, 1.f);
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

// ================================================================ SECOND PASS: BLADE SIGNS (third pass: moved, more, their own words)
// On the podiums' fronts, read along the street: 26 m out (their lights 24.2 m: the nearest a lit thing may stand), their
// letters lighting one after the other from the top. Each district has its own words. None in Syndicate Territory
// (nothing lit is added at the Chairman's height).
Zone(TEXT("Signs: Fifth and Main"), 0.f, 1.1f);
	Scatter(TEXT("SM_BladeHotel"), 26.f, 26.f, 2, 1, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_BladeDiner"), 26.f, 26.f, 4, 0, 1.f, 1.f, 0.f, 0, 0.f);
Zone(TEXT("Signs: Theatre Row"), 1.1f, 2.1f);
	Scatter(TEXT("SM_BladeShow"), 26.f, 26.f, 2, 0, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_BladeJazz"), 26.f, 26.f, 4, 1, 1.f, 1.f, 0.f, 0, 0.f);
Zone(TEXT("Signs: the Plaza"), 3.13f, 4.62f);
	Scatter(TEXT("SM_BladeHotel"), 26.f, 26.f, 4, 0, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_BladeClub"), 26.f, 26.f, 4, 2, 1.f, 1.f, 0.f, 0, 0.f);
Zone(TEXT("Signs: Wall Street"), 4.62f, 6.29f);
	Scatter(TEXT("SM_BladeBank"), 26.f, 26.f, 2, 0, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_BladeDiner"), 26.f, 26.f, 4, 1, 1.f, 1.f, 0.f, 0, 0.f);
Zone(TEXT("Signs: Penthouse Row"), 6.29f, 7.6f);
	Scatter(TEXT("SM_BladeHotel"), 26.f, 26.f, 2, 0, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(TEXT("SM_BladeClub"), 26.f, 26.f, 4, 1, 1.f, 1.f, 0.f, 0, 0.f);

// ================================================================ THIRD PASS (the overhaul)
// ---- the cut banks, faced by the track (FTDress.h, Bank): the city's tiles to 7 km, the Syndicate's black and gold
// after; the tall panel where 5.5 m and more of cutting shows, the low one (the same for both) on the rest
Zone(TEXT("Cut banks: the city"), 0.f, 7.f);
	Bank(TEXT("SM_CutTiles"), 4, 1, 0, 1.f, 5.5f);
	Bank(TEXT("SM_CutCity"), 4, 1, 0, 1.f, 2.4f, 5.5f);
Zone(TEXT("Cut banks: the Syndicate"), 7.f, 99.f);
	Bank(TEXT("SM_CutSyn"), 4, 1, 0, 1.f, 5.5f);
	Bank(TEXT("SM_CutCity"), 4, 1, 0, 1.f, 2.4f, 5.5f);

// ---- the canyon closed: a glass building in every gap the second pass left empty (which gap, each side, follows from
// the alleys', monuments', banners' and side streets' own every-and-phase in the district above)
Zone(TEXT("Closed gaps: Theatre Row"), 1.10f, 2.10f);
	Site(TEXT("G_Fill"), -1, 29, 3, 2, 1.f);
	Site(TEXT("G_Fill"), -1, 29, 6, 3, 1.f);
	Site(TEXT("G_Fill"), 1, 29, 3, 1, 1.f);
	Site(TEXT("G_Fill"), 1, 29, 6, 0, 1.f);
Zone(TEXT("Closed gaps: Wall Street"), 4.62f, 6.29f);
	Site(TEXT("G_Fill"), -1, 29, 4, 2, 1.f);
	Site(TEXT("G_Fill"), -1, 29, 4, 3, 1.f);
	Site(TEXT("G_Fill"), 1, 29, 4, 0, 1.f);
	Site(TEXT("G_Fill"), 1, 29, 4, 1, 1.f);
Zone(TEXT("Closed gaps: Penthouse Row"), 6.29f, 7.66f);
	Site(TEXT("G_Fill"), -1, 29, 6, 3, 1.f);
	Site(TEXT("G_Fill"), -1, 29, 6, 1, 1.f);
	Site(TEXT("G_Fill"), 1, 29, 6, 0, 1.f);
	Site(TEXT("G_Fill"), 1, 29, 6, 4, 1.f);
Zone(TEXT("Signs: the Garages"), 2.1f, 3.13f);
	Scatter(TEXT("SM_BladePark"), 26.f, 26.f, 3, 1, 1.f, 1.f, 0.f, 0, 0.f);

// ---- ROUND 2: ALL FOURTEEN ARE OFF (chance 0; the rules stay, so every later rule keeps its dice). At 32 m they stand in
// the band of the level's own towers and no frame of round 1 showed one. To try one again: a site clear of its tower,
// its lights 30 m out or more (they chase). The words are spread now (three EAT, two JAZZ, four FILM, five CASH).
// ---- a sign at the end of every street: on the outside of each hairpin before the finale (one slot each, from the
// track's section list), a roof sign at three times its size, turned to face the street that runs up to the bend
Zone(TEXT("Street's end 0.95"), 0.9471f, 0.9531f); Site(TEXT("BigEatL"), -1, 32, 1, 0, 0.f);
Zone(TEXT("Street's end 1.09"), 1.0828f, 1.0888f); Site(TEXT("BigJazzR"), 1, 32, 1, 0, 0.f);
Zone(TEXT("Street's end 1.72"), 1.7162f, 1.7222f); Site(TEXT("BigFilmL"), -1, 32, 1, 0, 0.f);
Zone(TEXT("Street's end 2.31"), 2.3044f, 2.3104f); Site(TEXT("BigEatR"), 1, 32, 1, 0, 0.f);
Zone(TEXT("Street's end 2.44"), 2.4401f, 2.4461f); Site(TEXT("BigEatL"), -1, 32, 1, 0, 0.f);
Zone(TEXT("Street's end 3.66"), 3.6617f, 3.6677f); Site(TEXT("BigFilmR"), 1, 32, 1, 0, 0.f);
Zone(TEXT("Street's end 3.80"), 3.7974f, 3.8034f); Site(TEXT("BigFilmL"), -1, 32, 1, 0, 0.f);
Zone(TEXT("Street's end 4.39"), 4.3856f, 4.3916f); Site(TEXT("BigFilmL"), -1, 32, 1, 0, 0.f);
Zone(TEXT("Street's end 4.61"), 4.6118f, 4.6178f); Site(TEXT("BigCashR"), 1, 32, 1, 0, 0.f);
Zone(TEXT("Street's end 4.98"), 4.9737f, 4.9797f); Site(TEXT("BigCashL"), -1, 32, 1, 0, 0.f);
Zone(TEXT("Street's end 5.07"), 5.0642f, 5.0702f); Site(TEXT("BigCashR"), 1, 32, 1, 0, 0.f);
Zone(TEXT("Street's end 6.11"), 6.1048f, 6.1108f); Site(TEXT("BigCashL"), -1, 32, 1, 0, 0.f);
Zone(TEXT("Street's end 6.24"), 6.2405f, 6.2465f); Site(TEXT("BigCashR"), 1, 32, 1, 0, 0.f);
Zone(TEXT("Street's end 7.19"), 7.1906f, 7.1966f); Site(TEXT("BigJazzR"), 1, 32, 1, 0, 0.f);

// ---- under the flights (afloat sets, one slot each). THE GARAGE JUMP goes over the elevated railway: three lengths of
// viaduct across the valley under the car, its train running; THE WALL STREET JUMP over a boulevard: two lengths of
// street across the valley, cars driving along it
// ROUND 2: BOTH ARE OFF (chance 0). A flight is 80 m long: what lies under it is under the car within a second and never
// in the chase camera's frame. To be stood beside the landings instead.
Zone(TEXT("Under the garage jump"), 2.7568f, 2.7628f);
	SiteAfloat(TEXT("ElUnder"), 1, 0, 1, 0, 0.f);
Zone(TEXT("Under the Wall Street jump"), 5.7881f, 5.7941f);
	SiteAfloat(TEXT("BlvdUnder"), 1, 0, 1, 0, 0.f);

// ---- machinery over the roofs: tower cranes behind the Garages and Wall Street (the jib slews; the track keeps its
// whole swing off the road), searchlights round both loops, an airship over the towers every kilometre or two
Zone(TEXT("Cranes: the Garages"), 2.1f, 3.13f);
	Far(TEXT("SM_Crane"), 76, 92, 6, 2, 1.f, 1.f, 0.f, 0, 0.6f, 180.f);
Zone(TEXT("Cranes: Wall Street"), 4.62f, 6.29f);
	Far(TEXT("SM_Crane"), 76, 92, 9, 4, 1.f, 1.f, 0.f, 0, 0.6f, 180.f);
Zone(TEXT("Searchlights: the Plaza loop"), 3.208f, 3.488f);
	Far(TEXT("SM_Klieg"), 68, 84, 2, 0, 1.f, 1.5f, 0.f, 0, 0.62f, 0.f);
Zone(TEXT("Searchlights: the Penthouse loop"), 6.737f, 6.972f);
	Far(TEXT("SM_Klieg"), 68, 84, 2, 1, 1.f, 1.5f, 0.f, 0, 0.62f, 0.f);
Zone(TEXT("Airships"), 0.f, 99.f);
	Far(TEXT("SM_Blimp"), 30, 90, 47, 9, 1.f, 1.f, 0.f, -1, -125.f, 40.f);
	Far(TEXT("SM_Blimp"), 30, 90, 61, 31, 1.f, 1.f, 0.f, 1, -140.f, 40.f);

// ---- the Syndicate's gate: two of its headquarters towers at half size, one each side, where its territory begins
Zone(TEXT("The Syndicate's gate"), 7.7336f, 7.7396f);
	Site(TEXT("SynGatePair"), -1, 33, 1, 0, 1.f);
	Site(TEXT("SynGatePair"), 1, 33, 1, 0, 1.f);

// ---- the horizon's cards: two rows of flat skyline behind the lit towers, each row its own outline and tone
Zone(TEXT("Horizon cards"), 0.f, 99.f);
	Far(TEXT("SM_SkyCard1"), 900, 1100, 9, 0, 1.f, 1.2f, 0.1f, -1, 30.f, 6.f);
	Far(TEXT("SM_SkyCard1"), 900, 1100, 9, 4, 1.f, 1.2f, 0.1f, 1, 30.f, 6.f);
	Far(TEXT("SM_SkyCard2"), 1500, 1800, 12, 3, 1.f, 1.5f, 0.1f, -1, 35.f, 6.f);
	Far(TEXT("SM_SkyCard2"), 1500, 1800, 12, 9, 1.f, 1.5f, 0.1f, 1, 35.f, 6.f);
