// 3.2 dressing table, level 0: Bayshore. The words (Set, Zone, Scatter, Site, Far, Afloat, SiteAfloat, Lit, Bank) are explained
// at the top of FTTrackDress.cpp; the meshes are made by Content/Python/ft_dress_bay.py. Distances across are metres
// from the road's centre line; districts are in kilometres along the road.
//
// The lie of the land: the bay is on the LEFT of travel (boardwalk at 22 m, the beach running 300 m down to the sea,
// the shore about 340 m out), the town on the RIGHT (the level's own palms to 73 m, its first row of hotels at
// 100-155 m, a second at 240-310 m, its skyline at 380-600 m).
//
// The town is laid out as BLOCKS: cars and hedges at the kerb (18.6 m), a belt of palms with cafes and market stalls
// in it (40-50 m), shop rows facing the road (78-90 m), a service street behind them (93 m) that the level's hotels
// front onto, and a side street back from the road every 180 m (which is what divides it into blocks). Behind: more
// hotels and signed blocks (165-230 m), masts and towers, two further skylines on the horizon.
// The bay: an unbroken boardwalk, beach clubs, bars, towels on the upper beach, dune grass lower down, piers out to
// the water, boats near the shore, a marina, and liners and islands on the horizon.
// Each slot of the road is 45 m. On the town side the slots go, four at a time: side street, cafe, shops, market.

Set(TEXT("Walk"), { { TEXT("SM_Boardwalk"), 0, 0, 0, 0, 1 } });
Set(TEXT("Kerb"), { { TEXT("SM_ParkedCars"), 0, 0, 0, 0, 1 } });
Set(TEXT("Kerb2"), { { TEXT("SM_KerbStalls"), 0, 0, 0, 0, 1 } });         // third pass: these kerb slots are a market now (were more parked cars)
Set(TEXT("Back"), { { TEXT("SM_BackStreet"), 0, 0, 0, 0, 1 } });
Set(TEXT("Side"), { { TEXT("SM_CrossStreet"), 0, 0, 0, 0, 1 } });
Set(TEXT("PierSet"), { { TEXT("SM_Pier"), 0, 0, 0, 0, 1 } });
Set(TEXT("BeachClub"), {                                  // a managed stretch of beach beyond the walk
	{ TEXT("SM_BeachPath"),   6,  3, 0,   0, 1 },
	{ TEXT("SM_Cabanas"),   -14, 14, 0,   0, 1 },
	{ TEXT("SM_Sunbeds"),    15, 20, 0,  20, 1 },
	{ TEXT("SM_Sunbeds"),    -3, 32, 0, -35, 1 },
	{ TEXT("SM_SurfRack"),   19,  8, 0,   0, 1 },
});
Set(TEXT("Tiki"), {                                       // a bar on the sand and its customers
	{ TEXT("SM_TikiBar"),     0,  0, 0,   0, 1 },
	{ TEXT("SM_Sunbeds"),    11,  7, 0,  25, 1 },
	{ TEXT("SM_Sunbeds"),   -12,  6, 0, -20, 1 },
	{ TEXT("SM_Towels"),      2, 17, 0,   0, 1 },
	{ TEXT("SM_SurfRack"),   -8, -5, 0,   0, 1 },
});
Set(TEXT("Volley"), {
	{ TEXT("SM_Volleyball"),  0,  0, 0,   0, 1 },
	{ TEXT("SM_Sunbeds"),    17,  4, 0,  30, 1 },
	{ TEXT("SM_SurfRack"),  -15, -5, 0,   0, 1 },
	{ TEXT("SM_Flags"),       0, -9, 0,   0, 1 },
	{ TEXT("SM_Towels"),   -20,  6, 0,   0, 1 },
});
Set(TEXT("StreetCorner"), {                               // the town's edge along the road
	{ TEXT("SM_Planters"),    0,  0, 0,   0, 1 },
	{ TEXT("SM_Kiosk"),      15,  7, 0,   0, 1 },
	{ TEXT("SM_BusStop"),   -13,  7, 0,   0, 1 },
	{ TEXT("SM_PhoneBooths"), 8,  7, 0,   0, 1 },
});
Set(TEXT("StreetCorner2"), {
	{ TEXT("SM_Planters"),    0,  0, 0,   0, 1 },
	{ TEXT("SM_KioskDrinks"), -14, 7, 0,  0, 1 },
	{ TEXT("SM_Flags"),      12,  9, 0,   0, 1 },
});
Set(TEXT("Cafe"), {                                       // among the palms
	{ TEXT("SM_CafeTerrace"),  0,  0, 0,   0, 1 },
	{ TEXT("SM_Planters"),     0, -8, 0,   0, 0.8f },
});
Set(TEXT("Market"), {
	{ TEXT("SM_MarketStalls"), 0,  0, 0,   0, 1 },
	{ TEXT("SM_MarketStalls"), 3, 12, 0, 180, 1 },
});
Set(TEXT("ShopBlock"), {                                  // a block's front: two shop rows, cars beside, the yard round the back
	{ TEXT("SM_Shops"),        0,  0, 0,   0, 1 },
	{ TEXT("SM_Shops2"),      37,  0, 0,   0, 1 },
	{ TEXT("SM_CarPark"),    -36,  3, 0,   0, 1 },
	{ TEXT("SM_ServiceYard"), 18, 16, 0, 180, 1 },
});
Set(TEXT("MotelCourt"), {
	{ TEXT("SM_Motel"),        0,  0, 0,   0, 1 },
	{ TEXT("SM_Pool"),       -36, -3, 0,   0, 1 },
	{ TEXT("SM_CarPark"),     44, -2, 0,   0, 1 },
});
Set(TEXT("Villas"), {
	{ TEXT("SM_Villa"),        0,  0, 0,   0, 1 },
	{ TEXT("SM_Villa2"),      26,  3, 0,   0, 1 },
	{ TEXT("SM_Pool"),        12, 16, 0,   0, 0.8f },
	{ TEXT("SM_Villa"),      -27,  5, 0,   0, 0.9f },
});
Set(TEXT("Funfair"), {                                    // beside the level's own big wheel
	{ TEXT("SM_Coaster"),      0,  0, 0,   0, 1 },
	{ TEXT("SM_Carousel"),   -40, -34, 0,  0, 1.3f },
	{ TEXT("SM_Carousel"),    46, -30, 0,  0, 1 },
	{ TEXT("SM_Flags"),        0, -44, 0,  0, 1.2f },
	{ TEXT("SM_MarketStalls"), 10, -52, 0, 0, 1 },
	{ TEXT("SM_SwingRide"),   -92, -46, 0,  0, 1 },        // third pass: a chair swing that turns
});
Set(TEXT("Marina"), {                                     // afloat: the anchor stands at sea level
	{ TEXT("SM_MarinaDock"),   0,  0, 0,   0, 1 },
	{ TEXT("SM_MotorYacht"),  20, 22, 0,  25, 1 },
	{ TEXT("SM_Sailboat"),   -24, 26, 0, -15, 1 },
	{ TEXT("SM_Sailboat2"),   -6, 44, 0,  40, 1 },
	{ TEXT("SM_MotorYacht"), -44,  8, 0, 160, 1 },
	{ TEXT("SM_Sailboat"),    44, 40, 0,  70, 1 },
});

// ---- SECOND PASS (art pass 2, 15 Nov 2026): the ground plane, and the level's shot. Its sets; its districts are at
// the end of the table, laid over the first pass's (nothing was put in between the first pass's rules: a rule's
// random numbers come from its place in the table, so the first pass's pieces stand where they stood).
// From a jump, a loop or a banked bend the camera looks down on the land, and the land was one plain colour with small
// things standing on it. So the land near the road has a designed surface of flat pieces (ft_dress_bay.py, "the third
// round"), each with its own height over the land so that one may lie over another:
//   town side   Prom (a mosaic promenade, 26-34 m; the side streets cross over it), PoolDeck (a hotel's lagoon pool,
//               34 x 22 m), Courts (two tennis courts), Plaza (a sun laid in the paving round a fountain), Walkway (the
//               promenade's piece turned to run back from the road: for the hills), and among the hotels, 160 m out
//               and more, SM_ParkBlock (a garden the size of a block) and the pool and the plaza again, larger
//   beach       WalkShops (three businesses on the boardwalk's seaward side), Lido (canopies in rows), ClubDeck (a
//               beach club's deck and pool, banners), BoatBeach (boats drawn up in a row), GroyneSet (a rock groyne from
//               120 m down to the water, built for 205 m), Yard (a boatyard)
//   horizon     SM_Downtown: towers that grow as the level goes on (none before Hotel Row, most in the finale)
Set(TEXT("Prom"), { { TEXT("SM_Promenade"), 0, 0, 0, 0, 1 } });
Set(TEXT("Walkway"), { { TEXT("SM_Promenade"), 0, 0, 0, 90, 1 } });
Set(TEXT("PoolDeck"), { { TEXT("SM_ResortPool"), 0, 0, 0, 0, 1 } });
Set(TEXT("Courts"), { { TEXT("SM_Courts"), 0, 0, 0, 0, 1 } });
Set(TEXT("Plaza"), { { TEXT("SM_SunPlaza"), 0, 0, 0, 0, 1 } });
// the promenade and the same four, stood 0.9 m up as terraces (their bodies go 3 m down): for gentle slopes
Set(TEXT("PromTerrace"), { { TEXT("SM_Promenade"), 0, 0, 0.9f, 0, 1 } });
Set(TEXT("PoolTerrace"), { { TEXT("SM_ResortPool"), 0, 0, 0.9f, 0, 1 } });
Set(TEXT("CourtsTerrace"), { { TEXT("SM_Courts"), 0, 0, 0.9f, 0, 1 } });
Set(TEXT("PlazaTerrace"), { { TEXT("SM_SunPlaza"), 0, 0, 0.9f, 0, 1 } });
Set(TEXT("ClubTerrace"), { { TEXT("SM_BeachClub"), 0, 0, 0.9f, 0, 1 } });
Set(TEXT("Lido"), { { TEXT("SM_LidoBlock"), 0, 0, 0, 0, 1 } });
// (third pass: the five Bank sets of SM_BankWaves stood here. Bank rules lay the banks now: end of the table.)
Set(TEXT("ClubDeck"), { { TEXT("SM_BeachClub"), 0, 0, 0, 0, 1 } });
Set(TEXT("BoatBeach"), { { TEXT("SM_BoatRow"), 0, 0, 0, 0, 1 } });
Set(TEXT("GroyneSet"), { { TEXT("SM_Groyne"), 18, 0, 0, 0, 1 } });      // 18 m along: clear of a pier in the same slot
Set(TEXT("WalkShops"), { { TEXT("SM_BoardShops"), 0, 0, 0, 0, 1 } });
Set(TEXT("Yard"), {                                       // a boatyard, and the boats waiting beside it
	{ TEXT("SM_Boatyard"),   0,  0, 0,   0, 1 },
	{ TEXT("SM_BoatRow"),  -36, -2, 0,   0, 1 },
});

// ---- 0 to 3.2 km: THE STRIP. The densest stretch: an unbroken boardwalk, a beach club or a bar every 90 m, the
// kerb full of cars, cafes and markets among the palms, shop blocks, and the new hotels crowding in behind.
Zone(TEXT("The Strip"), 0.0f, 3.2f);
	Site(TEXT("Walk"), -1, 22, 1, 0, 1.f);
	Site(TEXT("BeachClub"), -1, 34, 4, 1, 0.95f);
	Site(TEXT("Tiki"), -1, 52, 4, 3, 0.9f);
	Scatter(TEXT("SM_Towels"), 30, 100, 1, 0, 0.7f, 1.f, 0.f, -1);
	Scatter(TEXT("SM_Towels"), 34, 104, 2, 1, 0.8f, 1.f, 0.f, -1);
	Scatter(TEXT("SM_DuneGrass"), 110, 270, 1, 0, 0.8f, 1.2f, 0.3f, -1, 180.f);
	Site(TEXT("PierSet"), -1, 190, 14, 9, 1.f);
	Site(TEXT("Kerb"), 1, 18.6f, 2, 0, 0.9f);
	Site(TEXT("Kerb2"), 1, 18.6f, 4, 1, 0.9f);
	Site(TEXT("StreetCorner"), 1, 18.5f, 8, 3, 1.f); Lit();
	Site(TEXT("StreetCorner2"), 1, 18.5f, 8, 7, 1.f);
	Site(TEXT("Side"), 1, 90, 4, 0, 1.f);
	Site(TEXT("Cafe"), 1, 44, 4, 3, 0.9f); Lit();
	Site(TEXT("ShopBlock"), 1, 82, 4, 2, 1.f);
	Site(TEXT("Market"), 1, 42, 4, 1, 0.8f);
	Site(TEXT("Back"), 1, 93.5f, 1, 0, 1.f);
	Scatter(TEXT("SM_HotelCoral"), 172, 215, 3, 0, 0.75f, 1.f, 0.f, 1);
	Scatter(TEXT("SM_HotelSky"), 168, 205, 3, 1, 0.7f, 1.f, 0.f, 1);
	Scatter(TEXT("SM_BlockOcean"), 185, 228, 5, 2, 0.9f, 1.f, 0.1f, 1);
	Scatter(TEXT("SM_HotelGrand"), 330, 362, 6, 4, 0.9f, 1.15f, 0.f, 1);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Skyline"), 720, 1000, 5, 1, 1.f, 2.4f, 0.15f, 1, 10.f, 6.f);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Skyline"), 1350, 1900, 8, 4, 1.f, 3.8f, 0.15f, 1, 20.f, 6.f);
	Afloat(TEXT("SM_Speedboat"), 350, 430, 3, 1, 0.7f, 1.5f, 0.2f, -1, 180.f);
	Afloat(TEXT("SM_Sailboat"), 400, 640, 2, 0, 0.75f, 1.3f, 0.25f, -1, 180.f);
	Afloat(TEXT("SM_Sailboat2"), 450, 760, 3, 2, 0.7f, 1.4f, 0.25f, -1, 180.f);
	Afloat(TEXT("SM_MotorYacht"), 480, 820, 5, 3, 0.9f, 1.3f, 0.2f, -1, 180.f);
	Afloat(TEXT("SM_CruiseShip"), 950, 1500, 23, 6, 1.f, 1.1f, 0.2f, -1, 25.f);
	Afloat(TEXT("SM_Island"), 1500, 2300, 31, 11, 1.f, 1.6f, 0.3f, -1, 180.f);

// ---- 3.2 to 6.4 km: THE MARINA. Fewer beach huts; the bay fills with moored boats round the marina's jetties, a
// pier runs out to them every 400 m. Motels with pools and a radio mast over the town.
Zone(TEXT("The Marina"), 3.2f, 6.4f);
	Site(TEXT("Walk"), -1, 22, 1, 0, 1.f);
	Scatter(TEXT("SM_Flags"), 27, 30, 3, 1, 0.8f, 1.f, 0.f, -1);
	Scatter(TEXT("SM_SurfRack"), 30, 48, 4, 2, 0.7f, 1.f, 0.f, -1, 20.f);
	Scatter(TEXT("SM_Towels"), 30, 100, 2, 0, 0.7f, 1.f, 0.f, -1);
	Scatter(TEXT("SM_DuneGrass"), 90, 270, 1, 0, 0.85f, 1.2f, 0.3f, -1, 180.f);
	Site(TEXT("Tiki"), -1, 52, 6, 5, 0.9f);
	Site(TEXT("PierSet"), -1, 190, 9, 4, 1.f);
	SiteAfloat(TEXT("Marina"), -1, 385, 3, 1, 1.f, 1.25f);
	Afloat(TEXT("SM_Sailboat"), 400, 700, 1, 0, 0.6f, 1.3f, 0.25f, -1, 180.f);
	Afloat(TEXT("SM_Sailboat2"), 420, 800, 1, 0, 0.5f, 1.4f, 0.25f, -1, 180.f);
	Afloat(TEXT("SM_MotorYacht"), 460, 780, 2, 1, 0.8f, 1.3f, 0.2f, -1, 180.f);
	Afloat(TEXT("SM_Speedboat"), 350, 430, 4, 3, 0.7f, 1.5f, 0.2f, -1, 180.f);
	Afloat(TEXT("SM_CruiseShip"), 950, 1500, 19, 11, 1.f, 1.2f, 0.2f, -1, 25.f);
	Site(TEXT("Kerb2"), 1, 18.6f, 2, 0, 0.9f);
	Site(TEXT("Kerb"), 1, 18.6f, 4, 1, 0.9f);
	Site(TEXT("StreetCorner2"), 1, 18.5f, 4, 3, 0.9f); Lit();
	Site(TEXT("Side"), 1, 90, 4, 0, 1.f);
	Site(TEXT("MotelCourt"), 1, 76, 4, 2, 0.95f);
	Site(TEXT("Cafe"), 1, 44, 8, 3, 0.9f);
	Site(TEXT("Market"), 1, 42, 8, 7, 0.8f);
	Site(TEXT("Back"), 1, 93.5f, 1, 0, 1.f);
	Scatter(TEXT("SM_HotelSky"), 168, 210, 3, 2, 0.7f, 1.f, 0.f, 1);
	Scatter(TEXT("SM_LinerPink"), 170, 215, 4, 1, 0.8f, 1.f, 0.f, 1);
	Scatter(TEXT("SM_RadioMast"), 330, 370, 13, 5, 1.f, 1.f, 0.f, 1);
	Scatter(TEXT("SM_WaterTower"), 200, 232, 17, 9, 1.f, 1.f, 0.f, 1);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Skyline"), 760, 1050, 6, 2, 1.f, 2.2f, 0.15f, 1, 10.f, 6.f);
	Afloat(TEXT("SM_Island"), 1400, 2100, 29, 7, 1.f, 1.8f, 0.3f, -1, 180.f);

// ---- 6.4 to 9.8 km: PLEASURE BEACH. The beach at its busiest: the funfair's coaster and carousels by the big
// wheel, clubs, volleyball, flags, bars, a pier.
Zone(TEXT("Pleasure Beach"), 6.4f, 9.8f);
	Site(TEXT("Walk"), -1, 22, 1, 0, 1.f);
	Site(TEXT("BeachClub"), -1, 34, 2, 0, 0.9f);
	Site(TEXT("Volley"), -1, 62, 4, 1, 0.9f);
	Site(TEXT("Tiki"), -1, 54, 4, 3, 0.9f);
	Site(TEXT("Funfair"), -1, 170, 8, 5, 1.f); Lit();
	Site(TEXT("PierSet"), -1, 190, 12, 1, 1.f);
	Scatter(TEXT("SM_Cabanas"), 70, 110, 3, 2, 0.7f, 1.f, 0.f, -1);
	Scatter(TEXT("SM_KioskDrinks"), 28, 31, 5, 3, 0.9f, 1.f, 0.f, -1); Lit();
	Scatter(TEXT("SM_Towels"), 30, 100, 1, 0, 0.8f, 1.f, 0.f, -1);
	Scatter(TEXT("SM_Towels"), 34, 104, 1, 0, 0.6f, 1.f, 0.f, -1);
	Scatter(TEXT("SM_DuneGrass"), 110, 270, 2, 0, 0.8f, 1.2f, 0.3f, -1, 180.f);
	Site(TEXT("Kerb"), 1, 18.6f, 2, 1, 0.9f);
	Site(TEXT("StreetCorner"), 1, 18.5f, 4, 0, 0.9f); Lit();
	Site(TEXT("StreetCorner2"), 1, 18.5f, 4, 2, 0.9f);
	Site(TEXT("Side"), 1, 90, 4, 0, 1.f);
	Site(TEXT("Market"), 1, 42, 4, 3, 0.9f);
	Site(TEXT("ShopBlock"), 1, 82, 4, 2, 1.f);
	Site(TEXT("Cafe"), 1, 44, 4, 1, 0.9f);
	Site(TEXT("Back"), 1, 93.5f, 1, 0, 1.f);
	Scatter(TEXT("SM_HotelCoral"), 170, 215, 4, 2, 0.8f, 1.f, 0.f, 1);
	Scatter(TEXT("SM_BlockVista"), 185, 228, 5, 4, 0.9f, 1.f, 0.1f, 1);
	Scatter(TEXT("SM_WaterTower"), 330, 365, 19, 3, 1.f, 1.f, 0.f, 1);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Skyline"), 720, 1000, 5, 3, 1.f, 2.4f, 0.15f, 1, 10.f, 6.f);
	Afloat(TEXT("SM_Speedboat"), 350, 430, 2, 0, 0.7f, 1.5f, 0.2f, -1, 180.f);
	Afloat(TEXT("SM_Sailboat"), 400, 700, 2, 1, 0.7f, 1.3f, 0.25f, -1, 180.f);
	Afloat(TEXT("SM_MotorYacht"), 500, 850, 6, 2, 0.9f, 1.3f, 0.2f, -1, 180.f);
	Afloat(TEXT("SM_CruiseShip"), 950, 1500, 21, 13, 1.f, 1.1f, 0.2f, -1, 25.f);

// ---- 9.8 to 13.2 km: HOTEL ROW. The tall hotels in blocks, service yards and car parks between; a quieter beach.
Zone(TEXT("Hotel Row"), 9.8f, 13.2f);
	Site(TEXT("Walk"), -1, 22, 1, 0, 1.f);
	Scatter(TEXT("SM_Sunbeds"), 66, 110, 1, 0, 0.7f, 1.f, 0.f, -1, 40.f);     // second pass: was 30-70, where the hotels' decks and lidos now are
	Scatter(TEXT("SM_Towels"), 34, 100, 2, 0, 0.7f, 1.f, 0.f, -1);
	Scatter(TEXT("SM_DuneGrass"), 80, 270, 1, 0, 0.85f, 1.3f, 0.3f, -1, 180.f);
	Site(TEXT("Tiki"), -1, 98, 7, 3, 0.9f);                                    // second pass: was at 56 m, where the decks and lidos now are
	Site(TEXT("Kerb"), 1, 18.6f, 2, 0, 0.95f);
	Site(TEXT("Kerb2"), 1, 18.6f, 4, 1, 0.95f);
	Site(TEXT("StreetCorner2"), 1, 18.5f, 4, 3, 0.9f);
	Site(TEXT("Side"), 1, 90, 4, 0, 1.f);
	Site(TEXT("Cafe"), 1, 44, 4, 2, 0.9f); Lit();
	Site(TEXT("Back"), 1, 93.5f, 1, 0, 1.f);
	Scatter(TEXT("SM_CarPark"), 74, 78, 4, 1, 0.9f, 1.f, 0.f, 1);
	Scatter(TEXT("SM_ServiceYard"), 80, 84, 4, 3, 0.9f, 1.f, 0.f, 1);
	Scatter(TEXT("SM_HotelGrand"), 172, 205, 4, 0, 0.9f, 1.f, 0.f, 1);
	Scatter(TEXT("SM_HotelCoral"), 172, 215, 4, 2, 0.9f, 1.f, 0.f, 1);
	Scatter(TEXT("SM_LinerPink"), 170, 210, 4, 1, 0.8f, 1.f, 0.f, 1);
	Scatter(TEXT("SM_HotelGrand"), 330, 365, 3, 1, 0.8f, 1.2f, 0.1f, 1);
	Scatter(TEXT("SM_BlockOcean"), 190, 228, 4, 3, 0.9f, 1.f, 0.1f, 1);
	Scatter(TEXT("SM_RadioMast"), 420, 470, 15, 6, 1.f, 1.f, 0.f, 1);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Skyline"), 720, 1000, 4, 1, 1.f, 2.6f, 0.15f, 1, 10.f, 6.f);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Skyline"), 1350, 1900, 6, 2, 1.f, 4.f, 0.15f, 1, 20.f, 6.f);
	Afloat(TEXT("SM_Sailboat2"), 420, 800, 3, 0, 0.7f, 1.4f, 0.25f, -1, 180.f);
	Afloat(TEXT("SM_Speedboat"), 350, 430, 5, 2, 0.7f, 1.5f, 0.2f, -1, 180.f);
	Afloat(TEXT("SM_CruiseShip"), 900, 1400, 17, 4, 1.f, 1.3f, 0.2f, -1, 25.f);

// ---- 13.2 to 16.8 km: THE POINT. The town thins to white villas behind hedges; rocks and dune grass along the
// beach towards the lighthouse.
Zone(TEXT("The Point"), 13.2f, 16.8f);
	Scatter(TEXT("SM_Breakwater"), 120, 215, 2, 0, 0.8f, 1.f, 0.25f, -1, 60.f);
	Scatter(TEXT("SM_Breakwater"), 60, 110, 3, 1, 0.7f, 0.7f, 0.25f, -1, 60.f);
	Scatter(TEXT("SM_Sunbeds"), 30, 60, 4, 1, 0.6f, 1.f, 0.f, -1, 40.f);
	Scatter(TEXT("SM_DuneGrass"), 24, 270, 1, 0, 0.95f, 1.4f, 0.3f, -1, 180.f);
	Scatter(TEXT("SM_DuneGrass"), 30, 200, 1, 0, 0.8f, 1.2f, 0.3f, -1, 180.f);
	Site(TEXT("Walk"), -1, 22, 3, 0, 1.f);
	Site(TEXT("Villas"), 1, 84, 2, 0, 0.95f);
	Scatter(TEXT("SM_Planters"), 18.5f, 18.5f, 2, 0, 0.9f, 1.f, 0.f, 1);
	Site(TEXT("Kerb2"), 1, 18.6f, 4, 1, 0.8f);
	Site(TEXT("Side"), 1, 90, 6, 3, 1.f);
	Scatter(TEXT("SM_Pool"), 40, 60, 3, 1, 0.7f, 1.f, 0.f, 1);
	Scatter(TEXT("SM_Villa2"), 170, 220, 2, 1, 0.7f, 1.2f, 0.1f, 1, 20.f);
	Scatter(TEXT("SM_Villa"), 110, 150, 2, 0, 0.7f, 1.1f, 0.1f, 1, 20.f);
	Scatter(TEXT("SM_WaterTower"), 200, 232, 13, 5, 1.f, 1.f, 0.f, 1);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Skyline"), 900, 1300, 7, 2, 1.f, 2.6f, 0.15f, 1, 10.f, 6.f);
	Afloat(TEXT("SM_Sailboat"), 420, 760, 2, 0, 0.7f, 1.3f, 0.25f, -1, 180.f);
	Afloat(TEXT("SM_Island"), 1100, 1800, 11, 4, 1.f, 1.8f, 0.3f, -1, 180.f);
	Afloat(TEXT("SM_CruiseShip"), 900, 1400, 23, 15, 1.f, 1.2f, 0.2f, -1, 25.f);

// ---- 16.8 km to the end: THE FINALE, downtown's waterfront. Everything at once.
Zone(TEXT("Downtown waterfront"), 16.8f, 99.f);
	Site(TEXT("Walk"), -1, 22, 1, 0, 1.f);
	Site(TEXT("BeachClub"), -1, 34, 3, 1, 0.9f);
	Site(TEXT("Tiki"), -1, 54, 5, 2, 0.9f);
	Site(TEXT("Volley"), -1, 62, 7, 3, 0.9f);
	Site(TEXT("Funfair"), -1, 170, 19, 8, 1.f);
	Site(TEXT("PierSet"), -1, 190, 11, 5, 1.f);
	Scatter(TEXT("SM_Towels"), 30, 100, 1, 0, 0.7f, 1.f, 0.f, -1);
	Scatter(TEXT("SM_DuneGrass"), 110, 270, 1, 0, 0.8f, 1.2f, 0.3f, -1, 180.f);
	Site(TEXT("Kerb"), 1, 18.6f, 2, 0, 0.9f);
	Site(TEXT("Kerb2"), 1, 18.6f, 4, 1, 0.9f);
	Site(TEXT("StreetCorner"), 1, 18.5f, 8, 3, 1.f); Lit();
	Site(TEXT("StreetCorner2"), 1, 18.5f, 8, 7, 1.f);
	Site(TEXT("Side"), 1, 90, 4, 0, 1.f);
	Site(TEXT("Cafe"), 1, 44, 4, 3, 0.9f); Lit();
	Site(TEXT("ShopBlock"), 1, 82, 8, 2, 1.f);
	Site(TEXT("MotelCourt"), 1, 76, 8, 6, 0.95f);
	Site(TEXT("Market"), 1, 42, 4, 1, 0.8f);
	Site(TEXT("Back"), 1, 93.5f, 1, 0, 1.f);
	Scatter(TEXT("SM_HotelGrand"), 172, 205, 3, 0, 0.85f, 1.f, 0.f, 1);
	Scatter(TEXT("SM_HotelCoral"), 172, 215, 3, 1, 0.85f, 1.f, 0.f, 1);
	Scatter(TEXT("SM_HotelSky"), 168, 210, 3, 2, 0.8f, 1.f, 0.f, 1);
	Scatter(TEXT("SM_BlockOcean"), 330, 365, 4, 1, 0.9f, 1.25f, 0.1f, 1);
	Scatter(TEXT("SM_BlockVista"), 190, 228, 4, 3, 0.9f, 1.f, 0.1f, 1);
	Scatter(TEXT("SM_RadioMast"), 420, 470, 11, 5, 1.f, 1.f, 0.f, 1);
	Scatter(TEXT("SM_WaterTower"), 330, 365, 13, 8, 1.f, 1.f, 0.f, 1);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Skyline"), 720, 1000, 4, 0, 1.f, 2.6f, 0.15f, 1, 10.f, 6.f);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Skyline"), 1350, 1900, 6, 3, 1.f, 4.f, 0.15f, 1, 20.f, 6.f);
	SiteAfloat(TEXT("Marina"), -1, 385, 9, 4, 1.f, 1.25f);
	Afloat(TEXT("SM_Speedboat"), 350, 430, 3, 1, 0.7f, 1.5f, 0.2f, -1, 180.f);
	Afloat(TEXT("SM_Sailboat"), 400, 700, 2, 0, 0.7f, 1.3f, 0.25f, -1, 180.f);
	Afloat(TEXT("SM_MotorYacht"), 480, 820, 5, 2, 0.9f, 1.3f, 0.2f, -1, 180.f);
	Afloat(TEXT("SM_CruiseShip"), 950, 1500, 17, 9, 1.f, 1.2f, 0.2f, -1, 25.f);
	Afloat(TEXT("SM_Island"), 1500, 2300, 23, 9, 1.f, 1.6f, 0.3f, -1, 180.f);
// ================================================================================================= SECOND PASS
// First the things that stand anywhere: groynes, the marina off the Strip, downtown on the horizon. One district
// over each of the first pass's.
Zone(TEXT("The Strip, second pass"), 0.0f, 3.2f);
	Site(TEXT("GroyneSet"), -1, 205, 4, 2, 1.f);
	SiteAfloat(TEXT("Marina"), -1, 385, 5, 2, 1.f, 1.25f);         // seen from the first jump and the first loop
Zone(TEXT("The Marina, second pass"), 3.2f, 6.4f);
	Site(TEXT("GroyneSet"), -1, 205, 6, 0, 1.f);                    // a groyne at every second marina
Zone(TEXT("Pleasure Beach, second pass"), 6.4f, 9.8f);
	Site(TEXT("GroyneSet"), -1, 205, 8, 2, 1.f);
Zone(TEXT("Hotel Row, second pass"), 9.8f, 13.2f);
	Site(TEXT("GroyneSet"), -1, 205, 6, 3, 1.f);
	Far(TEXT("SM_Downtown"), 520, 640, 12, 7, 1.f, 0.8f, 0.1f, 1, 2.4f, 20.f);       // downtown's first towers, over the town
Zone(TEXT("The Point, second pass"), 13.2f, 16.8f);
	Site(TEXT("GroyneSet"), -1, 205, 5, 1, 1.f);
	Far(TEXT("SM_Downtown"), 560, 700, 16, 3, 1.f, 1.f, 0.1f, 1, 2.5f, 20.f);        // (third pass: every second of these is SM_Downtown2 now, end of the table)
Zone(TEXT("Downtown waterfront, second pass"), 16.8f, 99.f);
	Site(TEXT("GroyneSet"), -1, 205, 6, 2, 1.f);
	Far(TEXT("SM_Downtown"), 430, 560, 10, 1, 1.f, 1.25f, 0.15f, 1, 2.2f, 180.f);    // downtown itself: a near row of towers (third pass: every second is SM_Downtown2)
	Far(TEXT("SM_Downtown"), 760, 1000, 12, 3, 1.f, 1.7f, 0.15f, 1, 6.f, 180.f);     // and a taller one behind (the same)

// ---- The ground plane. The track stands a piece level at its middle, and Bayshore's land rises and falls with the
// road: on a hill a long flat piece has one end under the ground and the other in the air (a 36 m pool deck on a 1 in
// 10 slope: 1.8 m each way). So the flat pieces go on LEVEL ground only: each district's rules are written once, as
// a function, and given the stretches of 45 m slots where the land rises or falls no more than 15 cm within 19 m of
// the slot's middle (make_inl_L0.py in SkyScratch/DevLog/jam/art_pass2/w1 works them out from the level's sections:
// 63 slots in a hundred). The slots the rules use are the ones the first pass left free. On the town side its slots
// go, four at a time: side street, cafe, shops, market (in the Marina: side street, cafe or market, motel, nothing).
auto SlotKm = [](float S) { return S * 0.045243f; };                // a slot is 330 road units: 45.243 m
auto StripLevel = [&](float A, float B)
{
	Zone(TEXT("The Strip, level ground"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	// town: the promenade; a pool deck in front of the shops; courts or a plaza beside a cafe
	Site(TEXT("Prom"), 1, 30, 1, 0, 1.f);
	Site(TEXT("PoolDeck"), 1, 52, 4, 2, 0.9f);
	Site(TEXT("Courts"), 1, 63, 8, 7, 0.9f);
	Site(TEXT("Plaza"), 1, 66, 8, 3, 0.9f);
	// beach: shops on the walk with a lido behind them; a club deck or boats; a lido lower down
	Site(TEXT("WalkShops"), -1, 32, 4, 0, 0.9f);
	Site(TEXT("Lido"), -1, 64, 4, 0, 0.9f);
	Site(TEXT("ClubDeck"), -1, 48, 8, 6, 1.f);
	Site(TEXT("BoatBeach"), -1, 52, 8, 2, 1.f);
	Site(TEXT("Lido"), -1, 88, 4, 2, 0.8f);
};
StripLevel(1, 3); StripLevel(9, 10); StripLevel(23, 27); StripLevel(42, 42); StripLevel(49, 53); StripLevel(62, 63); StripLevel(67, 67);
auto MarinaLevel = [&](float A, float B)
{
	Zone(TEXT("The Marina, level ground"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	// town: a pool deck in the free slot, a plaza or courts in front of the motel
	Site(TEXT("Prom"), 1, 30, 1, 0, 1.f);
	Site(TEXT("PoolDeck"), 1, 50, 4, 1, 0.9f);
	Site(TEXT("Plaza"), 1, 50, 8, 6, 0.9f);
	Site(TEXT("Courts"), 1, 50, 8, 2, 0.9f);
	// beach: the boatyards, boats on the sand
	Site(TEXT("Yard"), -1, 72, 9, 0, 1.f);
	Site(TEXT("BoatBeach"), -1, 56, 6, 2, 1.f);
	Site(TEXT("Lido"), -1, 62, 6, 4, 0.9f);
	Site(TEXT("WalkShops"), -1, 32, 6, 3, 0.9f);
};
MarinaLevel(74, 74); MarinaLevel(82, 87); MarinaLevel(93, 93); MarinaLevel(101, 108); MarinaLevel(124, 137);
auto PleasureLevel = [&](float A, float B)
{
	Zone(TEXT("Pleasure Beach, level ground"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Site(TEXT("Prom"), 1, 30, 1, 0, 1.f);
	Site(TEXT("PoolDeck"), 1, 52, 4, 2, 0.9f);
	Site(TEXT("Courts"), 1, 63, 8, 5, 0.9f);
	Site(TEXT("Plaza"), 1, 66, 8, 1, 0.9f);
	Site(TEXT("WalkShops"), -1, 32, 4, 3, 0.9f);
	Site(TEXT("Lido"), -1, 88, 8, 2, 0.9f);
};
PleasureLevel(145, 152); PleasureLevel(160, 160); PleasureLevel(177, 184); PleasureLevel(193, 193); PleasureLevel(199, 204); PleasureLevel(214, 216);
auto HotelLevel = [&](float A, float B)
{
	Zone(TEXT("Hotel Row, level ground"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	// the hotels' own grounds: a pool deck, courts or a plaza on the town side; a club deck or a lido on the beach
	Site(TEXT("Prom"), 1, 30, 1, 0, 1.f);
	Site(TEXT("PoolDeck"), 1, 52, 4, 3, 0.9f);
	Site(TEXT("Courts"), 1, 52, 8, 5, 0.9f);
	Site(TEXT("Plaza"), 1, 54, 8, 1, 0.9f);
	Site(TEXT("ClubDeck"), -1, 46, 4, 0, 0.9f);
	Site(TEXT("Lido"), -1, 54, 4, 2, 0.9f);
	Site(TEXT("WalkShops"), -1, 32, 4, 1, 0.8f);
	Site(TEXT("BoatBeach"), -1, 84, 8, 5, 1.f);
};
HotelLevel(217, 217); HotelLevel(222, 223); HotelLevel(228, 228); HotelLevel(236, 237); HotelLevel(243, 249); HotelLevel(261, 262); HotelLevel(267, 275); HotelLevel(281, 281); HotelLevel(289, 289);
auto PointLevel = [&](float A, float B)
{
	Zone(TEXT("The Point, level ground"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	// quiet: the promenade between the villas' gates, courts and a pool deck now and then, boats on the sand
	Site(TEXT("Prom"), 1, 30, 2, 1, 1.f);
	Site(TEXT("Courts"), 1, 56, 6, 0, 0.9f);
	Site(TEXT("PoolDeck"), 1, 52, 6, 2, 0.9f);
	Site(TEXT("BoatBeach"), -1, 46, 6, 1, 1.f);
};
PointLevel(296, 304); PointLevel(310, 318); PointLevel(327, 327); PointLevel(334, 345); PointLevel(354, 359); PointLevel(366, 370);
auto DowntownLevel = [&](float A, float B)
{
	Zone(TEXT("Downtown waterfront, level ground"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Site(TEXT("Prom"), 1, 30, 1, 0, 1.f);
	Site(TEXT("PoolDeck"), 1, 52, 4, 2, 0.9f);
	Site(TEXT("Courts"), 1, 52, 8, 7, 0.9f);
	Site(TEXT("Plaza"), 1, 66, 8, 3, 0.9f);
	Site(TEXT("WalkShops"), -1, 32, 4, 0, 0.9f);
	Site(TEXT("Lido"), -1, 90, 3, 0, 0.9f);
	Site(TEXT("BoatBeach"), -1, 92, 6, 4, 1.f);
	Site(TEXT("Yard"), -1, 94, 9, 8, 1.f);
};
DowntownLevel(377, 381); DowntownLevel(391, 396); DowntownLevel(408, 413); DowntownLevel(422, 427); DowntownLevel(440, 444); DowntownLevel(454, 459); DowntownLevel(472, 476); DowntownLevel(486, 491); DowntownLevel(504, 508);
DowntownLevel(518, 523); DowntownLevel(536, 540); DowntownLevel(550, 555);
// On gentle slopes (the land rises or falls 15 to 95 cm within 19 m of the slot's middle): the promenade, pool decks, courts,
// plazas and club decks only, in the same slots, stood 0.9 m up as terraces: the high end still clears the land, the
// low end shows up to 2 m of wall.
auto StripSlope = [&](float A, float B)
{
	Zone(TEXT("The Strip, gentle slopes"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Site(TEXT("PromTerrace"), 1, 30, 1, 0, 1.f);
	Site(TEXT("PoolTerrace"), 1, 52, 4, 2, 0.9f);
	Site(TEXT("CourtsTerrace"), 1, 63, 8, 7, 0.9f);
	Site(TEXT("PlazaTerrace"), 1, 66, 8, 3, 0.9f);
	Site(TEXT("ClubTerrace"), -1, 48, 8, 6, 1.f);
};
StripSlope(4, 8); StripSlope(11, 11); StripSlope(14, 14); StripSlope(30, 30); StripSlope(33, 33); StripSlope(36, 36); StripSlope(40, 41); StripSlope(54, 54); StripSlope(57, 58);
StripSlope(61, 61);
auto MarinaSlope = [&](float A, float B)
{
	Zone(TEXT("The Marina, gentle slopes"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Site(TEXT("PromTerrace"), 1, 30, 1, 0, 1.f);
	Site(TEXT("PoolTerrace"), 1, 50, 4, 1, 0.9f);
	Site(TEXT("PlazaTerrace"), 1, 50, 8, 6, 0.9f);
	Site(TEXT("CourtsTerrace"), 1, 50, 8, 2, 0.9f);
};
MarinaSlope(80, 81); MarinaSlope(88, 92); MarinaSlope(109, 109); MarinaSlope(122, 123); MarinaSlope(138, 141);
auto PleasureSlope = [&](float A, float B)
{
	Zone(TEXT("Pleasure Beach, gentle slopes"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Site(TEXT("PromTerrace"), 1, 30, 1, 0, 1.f);
	Site(TEXT("PoolTerrace"), 1, 52, 4, 2, 0.9f);
	Site(TEXT("CourtsTerrace"), 1, 63, 8, 5, 0.9f);
	Site(TEXT("PlazaTerrace"), 1, 66, 8, 1, 0.9f);
};
PleasureSlope(142, 144); PleasureSlope(161, 161); PleasureSlope(169, 169); PleasureSlope(171, 176); PleasureSlope(191, 192); PleasureSlope(205, 205);
auto HotelSlope = [&](float A, float B)
{
	Zone(TEXT("Hotel Row, gentle slopes"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Site(TEXT("PromTerrace"), 1, 30, 1, 0, 1.f);
	Site(TEXT("PoolTerrace"), 1, 52, 4, 3, 0.9f);
	Site(TEXT("CourtsTerrace"), 1, 52, 8, 5, 0.9f);
	Site(TEXT("PlazaTerrace"), 1, 54, 8, 1, 0.9f);
	Site(TEXT("ClubTerrace"), -1, 46, 4, 0, 0.9f);
};
HotelSlope(221, 221); HotelSlope(238, 239); HotelSlope(250, 250); HotelSlope(253, 254); HotelSlope(259, 260); HotelSlope(264, 264); HotelSlope(277, 278);
auto PointSlope = [&](float A, float B)
{
	Zone(TEXT("The Point, gentle slopes"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Site(TEXT("PromTerrace"), 1, 30, 2, 1, 1.f);
	Site(TEXT("CourtsTerrace"), 1, 56, 6, 0, 0.9f);
	Site(TEXT("PoolTerrace"), 1, 52, 6, 2, 0.9f);
};
PointSlope(305, 309); PointSlope(325, 326); PointSlope(371, 371);
auto DowntownSlope = [&](float A, float B)
{
	Zone(TEXT("Downtown waterfront, gentle slopes"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Site(TEXT("PromTerrace"), 1, 30, 1, 0, 1.f);
	Site(TEXT("PoolTerrace"), 1, 52, 4, 2, 0.9f);
	Site(TEXT("CourtsTerrace"), 1, 52, 8, 7, 0.9f);
	Site(TEXT("PlazaTerrace"), 1, 66, 8, 3, 0.9f);
};
DowntownSlope(376, 376); DowntownSlope(390, 390); DowntownSlope(403, 403); DowntownSlope(432, 432); DowntownSlope(464, 464); DowntownSlope(515, 515); DowntownSlope(530, 530); DowntownSlope(535, 535); DowntownSlope(549, 549);
// Among the hotels, 160 to 320 m out, where the land was bare and is seen only from above: gardens, and the pool
// deck and the plaza again at half as large again. Where the land is level for 42 m either way.
auto TownBlocks = [&](float A, float B)
{
	Zone(TEXT("Among the hotels"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Scatter(TEXT("SM_ParkBlock"), 165, 235, 2, 0, 0.85f, 1.f, 0.15f, 1, 90.f);
	Scatter(TEXT("SM_ParkBlock"), 250, 330, 2, 1, 0.85f, 1.1f, 0.15f, 1, 90.f);
	Scatter(TEXT("SM_SunPlaza"), 165, 300, 3, 1, 0.8f, 1.8f, 0.2f, 1, 180.f);
	Scatter(TEXT("SM_ResortPool"), 160, 290, 3, 2, 0.8f, 1.5f, 0.2f, 1, 180.f);
};
TownBlocks(1, 3); TownBlocks(23, 23); TownBlocks(49, 50); TownBlocks(63, 63); TownBlocks(82, 87); TownBlocks(93, 93); TownBlocks(103, 105); TownBlocks(124, 124); TownBlocks(131, 137);
TownBlocks(149, 152); TownBlocks(160, 160); TownBlocks(178, 183); TownBlocks(200, 204); TownBlocks(222, 223); TownBlocks(228, 228); TownBlocks(236, 236); TownBlocks(249, 249); TownBlocks(272, 275);
TownBlocks(281, 281); TownBlocks(289, 289); TownBlocks(300, 303); TownBlocks(315, 317); TownBlocks(327, 327); TownBlocks(334, 334); TownBlocks(339, 345); TownBlocks(355, 359); TownBlocks(366, 370);
TownBlocks(377, 381); TownBlocks(395, 396); TownBlocks(409, 413); TownBlocks(427, 427); TownBlocks(441, 444); TownBlocks(459, 459); TownBlocks(472, 476); TownBlocks(491, 491); TownBlocks(504, 508);
TownBlocks(523, 523); TownBlocks(536, 540); TownBlocks(554, 555);
// On the hills (the slots that are neither level nor a set piece): only what a slope cannot spoil. On the town side
// the promenade's piece turned to run back from the road, 26 to 64 m out (across the slope the land is level, and
// the piece is only 8 m wide along it). On the beach small things: sunbeds under parasols, a row of huts.
auto Hills = [&](float A, float B)
{
	Zone(TEXT("On a hill"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Site(TEXT("Walkway"), 1, 45, 2, 1, 1.f);
	Scatter(TEXT("SM_Sunbeds"), 30, 64, 1, 0, 0.9f, 1.f, 0.f, -1, 40.f);
	Scatter(TEXT("SM_Sunbeds"), 50, 96, 1, 0, 0.8f, 1.f, 0.f, -1, 40.f);
	Scatter(TEXT("SM_Cabanas"), 40, 80, 3, 1, 0.8f, 1.f, 0.f, -1);
};
Hills(4, 8); Hills(11, 22); Hills(28, 36); Hills(40, 41); Hills(54, 61); Hills(68, 73); Hills(79, 81); Hills(88, 92); Hills(109, 117);
Hills(121, 123); Hills(138, 144); Hills(161, 164); Hills(169, 176); Hills(185, 186); Hills(191, 192); Hills(194, 198); Hills(205, 213); Hills(218, 221);
Hills(238, 242); Hills(250, 254); Hills(259, 260); Hills(263, 266); Hills(276, 280); Hills(294, 295); Hills(305, 309); Hills(319, 320); Hills(325, 326);
Hills(350, 353); Hills(371, 376); Hills(388, 390); Hills(401, 407); Hills(420, 421); Hills(432, 439); Hills(452, 453); Hills(464, 471); Hills(484, 485);
Hills(496, 503); Hills(515, 517); Hills(528, 535); Hills(547, 549); Hills(560, 561);
// ---- The set pieces. Scatter and Site rules (and the level's own palms and hotels) leave a loop, a corkscrew and a
// jump's gap bare: 139 of the level's 561 slots, and the very ones the camera looks down on the land from. Far rules
// stay through them. So each set piece has districts of its own, of Far rules used as land rules: the distance is
// the place across, the drop stands the piece on the land there (town side 0.8 m under the road's level, 1.2 m at
// 200 m; the beach 0.95 m at the walk, 1.4 at 50 m, 1.85 at 78 m), the last number is the yaw jitter (0: square to
// the road). A Far rule shifts its piece up to 11 m along the road, so the big flat pieces go one to a slot, on the
// inner slots of the set piece, and in bands across that cannot meet; FTTrack's road test still moves or drops
// whatever would touch the road (a loop comes down 33 m to the right of where it went up). The land under a loop
// and a corkscrew is level.
// The numbers are the 45 m slots the set piece is flagged on at their middles: the first and the last.
auto LoopSite = [&](float A, float B)
{
	Zone(TEXT("Loop"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Far(TEXT("SM_HotelSky"), 112, 150, 2, 0, 0.9f, 1.f, 0.f, 1, 0.85f, 0.f);
	Far(TEXT("SM_LinerPink"), 112, 150, 2, 1, 0.9f, 1.f, 0.f, 1, 0.85f, 0.f);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Palm"), 28, 84, 1, 0, 0.8f, 1.f, 0.25f, 0, 1.2f, 180.f);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Palm2"), 30, 80, 2, 1, 0.8f, 1.1f, 0.25f, 0, 1.2f, 180.f);
	Zone(TEXT("Loop, going up"), SlotKm(A + 0.5f), SlotKm(A + 1.5f));
	Far(TEXT("SM_ResortPool"), 60, 60, 1, 0, 1.f, 1.f, 0.f, 1, 0.76f, 0.f);
	Far(TEXT("SM_ParkBlock"), 190, 212, 1, 0, 1.f, 1.f, 0.1f, 1, 1.18f, 90.f);
	Far(TEXT("SM_BeachClub"), 70, 70, 1, 0, 1.f, 1.f, 0.f, -1, 1.71f, 0.f);
	Zone(TEXT("Loop, coming down"), SlotKm(B - 1.5f), SlotKm(B - 0.5f));
	Far(TEXT("SM_SunPlaza"), 66, 66, 1, 0, 1.f, 1.f, 0.f, 1, 0.75f, 0.f);
	Far(TEXT("SM_SunPlaza"), 245, 268, 1, 0, 1.f, 1.8f, 0.2f, 1, 1.37f, 180.f);
	Far(TEXT("SM_LidoBlock"), 78, 78, 1, 0, 1.f, 1.f, 0.f, -1, 1.84f, 0.f);
};
auto TwistSite = [&](float A, float B)
{
	Zone(TEXT("Corkscrew"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Far(TEXT("SM_Shops"), 86, 86, 2, 1, 1.f, 1.f, 0.f, 1, 0.75f, 0.f);
	Far(TEXT("SM_HotelSky"), 112, 150, 2, 0, 0.9f, 1.f, 0.f, 1, 0.85f, 0.f);
	Far(TEXT("SM_HotelCoral"), 115, 150, 2, 1, 0.9f, 1.f, 0.f, 1, 0.85f, 0.f);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Palm"), 28, 84, 1, 0, 0.8f, 1.f, 0.25f, 0, 1.2f, 180.f);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Palm2"), 30, 80, 2, 1, 0.8f, 1.1f, 0.25f, 0, 1.2f, 180.f);
	Zone(TEXT("Corkscrew, middle"), SlotKm(A + 0.5f), SlotKm(A + 1.5f));
	Far(TEXT("SM_Promenade"), 30, 30, 1, 0, 1.f, 1.f, 0.f, 1, 0.82f, 0.f);
	Far(TEXT("SM_ResortPool"), 48, 48, 1, 0, 1.f, 1.f, 0.f, 1, 0.78f, 0.f);
	Far(TEXT("SM_ParkBlock"), 190, 212, 1, 0, 1.f, 1.f, 0.1f, 1, 1.18f, 90.f);
	Far(TEXT("SM_Boardwalk"), 22, 22, 1, 0, 1.f, 1.f, 0.f, -1, 0.95f, 0.f);
	Far(TEXT("SM_BeachClub"), 50, 50, 1, 0, 1.f, 1.f, 0.f, -1, 1.39f, 0.f);
	Zone(TEXT("Corkscrew, end"), SlotKm(B - 0.5f), SlotKm(B + 0.5f));
	Far(TEXT("SM_SunPlaza"), 72, 72, 1, 0, 1.f, 1.f, 0.f, 1, 0.76f, 0.f);
	Far(TEXT("SM_LidoBlock"), 76, 76, 1, 0, 1.f, 1.f, 0.f, -1, 1.81f, 0.f);
};
// Under a jump the land dips 18 m, with sides as steep as 2 in 5: nothing level and long can stand there. A grove
// of palms can (sixteen to a slot), and dune grass on the beach side.
auto JumpSite = [&](float A, float B)
{
	Zone(TEXT("Under the jump"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Palm"), 26, 60, 1, 0, 1.f, 1.f, 0.25f, 0, 1.3f, 180.f);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Palm2"), 34, 80, 1, 0, 1.f, 1.1f, 0.25f, 0, 1.3f, 180.f);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Palm"), 60, 110, 1, 0, 1.f, 1.1f, 0.25f, 0, 1.4f, 180.f);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Palm2"), 84, 140, 1, 0, 1.f, 1.2f, 0.25f, 0, 1.5f, 180.f);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Palm2"), 26, 50, 1, 0, 1.f, 1.f, 0.25f, 0, 1.3f, 180.f);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Palm"), 40, 76, 1, 0, 1.f, 1.2f, 0.25f, 0, 1.3f, 180.f);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Palm2"), 56, 100, 1, 0, 1.f, 1.f, 0.25f, 0, 1.4f, 180.f);
	Far(TEXT("/Game/Maps/Bayshore/Props/SM_Palm"), 96, 150, 1, 0, 1.f, 1.2f, 0.25f, 0, 1.5f, 180.f);
	Far(TEXT("SM_DuneGrass"), 30, 120, 1, 0, 1.f, 1.5f, 0.3f, -1, 1.6f, 180.f);
};
LoopSite(43, 48); LoopSite(94, 100); LoopSite(153, 159); LoopSite(229, 235); LoopSite(282, 288); LoopSite(328, 333); LoopSite(360, 365);
LoopSite(382, 387); LoopSite(414, 419); LoopSite(445, 451); LoopSite(477, 483); LoopSite(509, 514); LoopSite(541, 546);
TwistSite(64, 66); TwistSite(165, 168); TwistSite(224, 227); TwistSite(290, 293); TwistSite(346, 349); TwistSite(397, 400);
TwistSite(428, 431); TwistSite(460, 463); TwistSite(492, 495); TwistSite(524, 527); TwistSite(556, 559);
JumpSite(38, 39); JumpSite(77, 78); JumpSite(119, 120); JumpSite(189, 190); JumpSite(256, 257); JumpSite(323, 324);

// ================================================================================================= THIRD PASS
// The zone pass (the overhaul, 15 Nov 2026): "coastal storefronts and boardwalk structures close to the road, with
// snapping awnings, moving surf, and turning amusement rides beyond". Written by make_l0.py (SkyScratch/DevLog/jam/
// overhaul/zones_w1), which works out how the land lies under every slot; ft_dress_bay.py, "the fourth round", has
// the pieces. The chase camera rides 3.5 m over the road and the barrier is 1.6 m high: on a straight it hides the
// land out to 32 m, so what the player sees first is whatever stands tall at the sight line (24 m). This pass puts
// a street there: on the town side rows of shopfronts (two new rows at 27.5 m, the first pass's shop row set back
// to 33 m behind the kiosk corners, the side streets left open), on the bay side an arcade on the boardwalk and the
// boardwalk shops on more of it, and right behind the barrier windbreaks, deckchairs and parasols.
// A long piece stands level at its slot's middle while the land follows the road: where the land rises or falls up
// to 0.9 m within 20 m the rows stand 0.45 m up, up to 1.9 m they stand 0.95 m up (their footings go 3.5 m down: a
// row on a hill is a terrace); on anything steeper only kiosks stand (31.6 m out: their neon pulses).
// After the first film and the independent review: every row and beach-edge rule is certain (a chance under 1 is
// not a sprinkle in this game: the first random number of a slot and a rule steps by 0.039 from slot to slot, so 0.9
// is "23 slots on, 3 off" and neighbouring rules are off together: four-slot holes in the street); The Point has
// the two front rows; only the arcades keep a chance (they come in runs, with open boardwalk between).
Set(TEXT("Front1"), { { TEXT("SM_Front1"), 0, 0, 0, 0, 1 } });
Set(TEXT("Front1B"), { { TEXT("SM_Front1"), 0, 0, 0.45f, 0, 1 } });
Set(TEXT("Front1C"), { { TEXT("SM_Front1"), 0, 0, 0.95f, 0, 1 } });
Set(TEXT("Front2"), { { TEXT("SM_Front2"), 0, 0, 0, 0, 1 } });
Set(TEXT("Front2B"), { { TEXT("SM_Front2"), 0, 0, 0.45f, 0, 1 } });
Set(TEXT("Front2C"), { { TEXT("SM_Front2"), 0, 0, 0.95f, 0, 1 } });
Set(TEXT("ShopRow"), { { TEXT("SM_Shops"), 0, 0, 0, 0, 1 } });
Set(TEXT("ShopRowB"), { { TEXT("SM_Shops"), 0, 0, 0.45f, 0, 1 } });
Set(TEXT("ShopRowC"), { { TEXT("SM_Shops"), 0, 0, 0.95f, 0, 1 } });
Set(TEXT("Edge"), { { TEXT("SM_BeachEdge"), 0, 0, 0, 0, 1 } });
Set(TEXT("EdgeR"), { { TEXT("SM_BeachEdge"), 0, 0, 0, 180, 1 } });
Set(TEXT("ArcadeSet"), { { TEXT("SM_Arcade"), 0, 0, 0, 0, 1 } });
Set(TEXT("ArcadeSetB"), { { TEXT("SM_Arcade"), 0, 0, 0.3f, 0, 1 } });
Set(TEXT("WalkShopsB"), { { TEXT("SM_BoardShops"), 0, 0, 0.3f, 0, 1 } });
Set(TEXT("Sky"), { { TEXT("SM_SkyRide"), 0, 0, 0, 0, 1 } });
// The big wheel's fairground, on the outside of the first banked bend (slot 26, 1.18 km). The wheel is turned -54.3 degrees
// to face the road that comes up to it (it is dead ahead from 0.85 to 1.05 km), and it turns (ft_life).
Set(TEXT("WheelPark"), {
	{ TEXT("SM_BigWheel"),      0,   0, 0, -54.3f, 1 },
	{ TEXT("SM_SwingRide"),    40, -30, 0,   0, 1 },
	{ TEXT("SM_Carousel"),    -36, -32, 0,   0, 1.2f },
	{ TEXT("SM_Flags"),         2, -56, 0,   0, 1.2f },
	{ TEXT("SM_MarketStalls"), 24, -64, 0,   0, 1 },
	{ TEXT("SM_MarketStalls"), -22, -66, 0,  0, 1 },
});
// The wheel at the end of the pleasure pier at slot 203 (9.18 km): larger, stood on the pier's own level (the sand
// is 10 m lower there), turned -41.8 degrees to face the fourth jump, which lands pointing straight at it from 700 m.
Set(TEXT("PierWheel"), { { TEXT("SM_BigWheel"), 0, 0, 9.39f, -41.8f, 1.35f } });
// Three of downtown's piers carry a wheel too (slots 413, 435, 523), each turned to face the road 450 m before it.
Set(TEXT("PierWheel413"), { { TEXT("SM_BigWheel"), 0, 0, 9.45f, -12.4f, 1.2f } });
Set(TEXT("PierWheel435"), { { TEXT("SM_BigWheel"), 0, 0, 9.45f, -17.8f, 1.2f } });
Set(TEXT("PierWheel523"), { { TEXT("SM_BigWheel"), 0, 0, 9.45f, -64.6f, 1.2f } });

auto Street = [&](float A, float B, const TCHAR* RowSet, const TCHAR* FrontA, const TCHAR* FrontB, int32 PhRow, int32 PhA, int32 PhB, float ChRow, float ChA, float ChB,
	const TCHAR* ArcSet, const TCHAR* ShopSet, int32 PhArc, float ChArc, int32 EvShop, int32 PhShop, float ChShop, float ChEdge)
{
	Zone(TEXT("The street"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Site(RowSet, 1, 33, 4, PhRow, ChRow);                        // the first pass's shop row, set back behind the kiosk corner
	Site(FrontA, 1, 27.5f, 4, PhA, ChA);                          // the new rows: their awnings' edge is at the sight line
	Site(FrontB, 1, 27.5f, 4, PhB, ChB);
	Site(TEXT("Edge"), -1, 20.4f, 2, 0, ChEdge);             // the beach right behind the barrier
	Site(TEXT("EdgeR"), -1, 20.4f, 2, 1, ChEdge);
	Site(ArcSet, -1, 33.5f, 4, PhArc, ChArc);                    // the arcade, astride the boardwalk's seaward edge
	Site(ShopSet, -1, 32, EvShop, PhShop, ChShop);                    // the boardwalk's shops where the land is not quite level
};
auto Steep = [&](float A, float B)
{
	Zone(TEXT("The street, steep"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Scatter(TEXT("SM_KioskDrinks"), 31.6f, 33.f, 2, 0, 1.f, 1.f, 0.f, 1);   // (31.6 m: the kiosk's strip of neon pulses, and light that pulses keeps 30 m)
	Scatter(TEXT("SM_Kiosk"), 31.6f, 33.f, 2, 1, 1.f, 1.f, 0.f, 1);
};
auto ChairLift = [&](float A, float B)
{
	Zone(TEXT("The chair lift"), SlotKm(A - 0.5f), SlotKm(B + 0.5f));
	Site(TEXT("Sky"), -1, 44, 1, 0, 1.f);
};
// ---- Beside the first jump (1.73 km) and the fourth (8.56 km): a chair swing at 2.2 times its size, 37 m tall, on the
// floor of the dip (18 m under the land), 52 m from the centre line: the bay side at the first jump, the town side at
// the fourth. Its crown turns 4 m over the car's flight: it shows over the land from 350 m before the kicker, and the
// car flies past it. Round one stood a swing and two carousels UNDER each jump: the chase camera never looks down
// there and the kicker hides the dip on the way up, so they were in no frame. Their six rules keep their places (a
// rule's random numbers come from its place in the table): two are the tall swings now, four have a chance of 0.f.
// A Far rule shifts its piece up to 11 m along the road by such a number, and a dip's floor is level for 12 m only:
// these rules stand where that number puts them within 3 m of its middle (make_l0.py found the places).
Street(1, 3, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // The Strip, level
Zone(TEXT("Under a jump"), 8.5283f, 8.5735f);   // slot 189, rule 1317: it lands at 8562 m (the dip is lowest at 8559 m)
	Far(TEXT("SM_Carousel"), 0, 0, 1, 0, 0.f, 1.5f, 0.f, 1, 1.15f, 0.f);                // (off: nothing under a jump is in any frame)
Street(9, 10, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // The Strip, level
Zone(TEXT("Under a jump"), 8.5283f, 8.5735f);   // slot 189, rule 1325: it lands at 8562 m (the dip is lowest at 8559 m)
	Far(TEXT("SM_SwingRide"), 52, 52, 1, 0, 1.f, 2.2f, 0.f, 1, 1.15f, 0.f);             // the fourth jump: the town side
Street(23, 27, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // The Strip, level
Zone(TEXT("Under a jump"), 8.5283f, 8.5735f);   // slot 189, rule 1333: it lands at 8562 m (the dip is lowest at 8559 m)
	Far(TEXT("SM_SwingRide"), 33, 33, 1, 0, 0.f, 1.f, 0.f, -1, 1.15f, 0.f);             // (off)
Street(42, 42, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // The Strip, level
Street(49, 53, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // The Strip, level
Street(62, 63, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // The Strip, level
Street(67, 67, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // The Strip, level
Street(5, 6, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // The Strip, gentle
Street(8, 8, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // The Strip, gentle
Zone(TEXT("Under a jump"), 1.6966f, 1.7419f);   // slot 38, rule 1376: it lands at 1731 m (the dip is lowest at 1732 m)
	Far(TEXT("SM_SwingRide"), 52, 52, 1, 0, 1.f, 2.2f, 0.f, -1, 1.15f, 0.f);            // the first jump: the bay side
Street(11, 11, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // The Strip, gentle
Zone(TEXT("Under a jump"), 1.6966f, 1.7419f);   // slot 38, rule 1384: it lands at 1730 m (the dip is lowest at 1732 m)
	Far(TEXT("SM_Carousel"), 31, 31, 1, 0, 0.f, 1.3f, 0.f, 1, 1.15f, 0.f);              // (off)
Street(14, 14, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // The Strip, gentle
Zone(TEXT("Under a jump"), 1.6966f, 1.7419f);   // slot 38, rule 1392: it lands at 1730 m (the dip is lowest at 1732 m)
	Far(TEXT("SM_Carousel"), 31, 31, 1, 0, 0.f, 1.3f, 0.f, -1, 1.15f, 0.f);             // (off)
Street(30, 30, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // The Strip, gentle
Street(33, 33, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // The Strip, gentle
Street(36, 36, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // The Strip, gentle
Street(40, 41, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // The Strip, gentle
Street(54, 54, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // The Strip, gentle
Street(57, 57, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // The Strip, gentle
Street(4, 4, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // The Strip, a slope
Street(7, 7, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // The Strip, a slope
Street(12, 13, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // The Strip, a slope
Street(16, 17, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // The Strip, a slope
Street(19, 20, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // The Strip, a slope
Street(22, 22, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // The Strip, a slope
Street(28, 29, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // The Strip, a slope
Street(31, 32, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // The Strip, a slope
Street(58, 61, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // The Strip, a slope
Street(70, 70, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // The Strip, a slope
Steep(15, 15);   // The Strip
Steep(18, 18);   // The Strip
Steep(21, 21);   // The Strip
Steep(34, 35);   // The Strip
Steep(37, 37);   // The Strip
Steep(55, 56);   // The Strip
Steep(68, 69);   // The Strip
Street(74, 74, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 0.6f, 6, 3, 0.f, 1.f);   // The Marina, level
Street(82, 87, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 0.6f, 6, 3, 0.f, 1.f);   // The Marina, level
Street(93, 93, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 0.6f, 6, 3, 0.f, 1.f);   // The Marina, level
Street(101, 108, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 0.6f, 6, 3, 0.f, 1.f);   // The Marina, level
Street(124, 137, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 0.6f, 6, 3, 0.f, 1.f);   // The Marina, level
Street(81, 81, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.6f, 6, 3, 1.f, 1.f);   // The Marina, gentle
Street(88, 88, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.6f, 6, 3, 1.f, 1.f);   // The Marina, gentle
Street(90, 90, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.6f, 6, 3, 1.f, 1.f);   // The Marina, gentle
Street(92, 92, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.6f, 6, 3, 1.f, 1.f);   // The Marina, gentle
Street(109, 109, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.6f, 6, 3, 1.f, 1.f);   // The Marina, gentle
Street(122, 123, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.6f, 6, 3, 1.f, 1.f);   // The Marina, gentle
Street(140, 141, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.6f, 6, 3, 1.f, 1.f);   // The Marina, gentle
Street(71, 71, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 6, 3, 0.f, 0.f);   // The Marina, a slope
Street(75, 75, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 6, 3, 0.f, 0.f);   // The Marina, a slope
Street(80, 80, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 6, 3, 0.f, 0.f);   // The Marina, a slope
Street(89, 89, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 6, 3, 0.f, 0.f);   // The Marina, a slope
Street(91, 91, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 6, 3, 0.f, 0.f);   // The Marina, a slope
Street(110, 115, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 6, 3, 0.f, 0.f);   // The Marina, a slope
Street(117, 117, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 6, 3, 0.f, 0.f);   // The Marina, a slope
Street(138, 139, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 6, 3, 0.f, 0.f);   // The Marina, a slope
Steep(72, 73);   // The Marina
Steep(76, 76);   // The Marina
Steep(79, 79);   // The Marina
Steep(116, 116);   // The Marina
Steep(118, 118);   // The Marina
Steep(121, 121);   // The Marina
Street(145, 152, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 1, 1.f, 4, 3, 0.f, 1.f);   // Pleasure Beach, level
Street(160, 160, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 1, 1.f, 4, 3, 0.f, 1.f);   // Pleasure Beach, level
Street(177, 184, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 1, 1.f, 4, 3, 0.f, 1.f);   // Pleasure Beach, level
Street(193, 193, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 1, 1.f, 4, 3, 0.f, 1.f);   // Pleasure Beach, level
Street(199, 204, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 1, 1.f, 4, 3, 0.f, 1.f);   // Pleasure Beach, level
Street(214, 216, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 1, 1.f, 4, 3, 0.f, 1.f);   // Pleasure Beach, level
Street(142, 144, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 1, 1.f, 4, 3, 1.f, 1.f);   // Pleasure Beach, gentle
Street(169, 169, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 1, 1.f, 4, 3, 1.f, 1.f);   // Pleasure Beach, gentle
Street(174, 174, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 1, 1.f, 4, 3, 1.f, 1.f);   // Pleasure Beach, gentle
Street(176, 176, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 1, 1.f, 4, 3, 1.f, 1.f);   // Pleasure Beach, gentle
Street(187, 187, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 1, 1.f, 4, 3, 1.f, 1.f);   // Pleasure Beach, gentle
Street(191, 192, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 1, 1.f, 4, 3, 1.f, 1.f);   // Pleasure Beach, gentle
Street(205, 205, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 1, 1.f, 4, 3, 1.f, 1.f);   // Pleasure Beach, gentle
Street(161, 161, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 1, 0.f, 4, 3, 0.f, 0.f);   // Pleasure Beach, a slope
Street(164, 164, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 1, 0.f, 4, 3, 0.f, 0.f);   // Pleasure Beach, a slope
Street(171, 173, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 1, 0.f, 4, 3, 0.f, 0.f);   // Pleasure Beach, a slope
Street(175, 175, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 1, 0.f, 4, 3, 0.f, 0.f);   // Pleasure Beach, a slope
Street(194, 198, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 1, 0.f, 4, 3, 0.f, 0.f);   // Pleasure Beach, a slope
Street(209, 213, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 2, 3, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 1, 0.f, 4, 3, 0.f, 0.f);   // Pleasure Beach, a slope
Steep(162, 163);   // Pleasure Beach
Steep(170, 170);   // Pleasure Beach
Steep(185, 186);   // Pleasure Beach
Steep(188, 188);   // Pleasure Beach
Steep(206, 208);   // Pleasure Beach
Street(217, 217, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 3, 0.6f, 4, 1, 0.f, 1.f);   // Hotel Row, level
Street(222, 223, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 3, 0.6f, 4, 1, 0.f, 1.f);   // Hotel Row, level
Street(228, 228, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 3, 0.6f, 4, 1, 0.f, 1.f);   // Hotel Row, level
Street(236, 237, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 3, 0.6f, 4, 1, 0.f, 1.f);   // Hotel Row, level
Street(243, 249, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 3, 0.6f, 4, 1, 0.f, 1.f);   // Hotel Row, level
Street(261, 262, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 3, 0.6f, 4, 1, 0.f, 1.f);   // Hotel Row, level
Street(267, 275, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 3, 0.6f, 4, 1, 0.f, 1.f);   // Hotel Row, level
Street(281, 281, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 3, 0.6f, 4, 1, 0.f, 1.f);   // Hotel Row, level
Street(289, 289, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 3, 0.6f, 4, 1, 0.f, 1.f);   // Hotel Row, level
Street(250, 250, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 3, 0.6f, 4, 1, 1.f, 1.f);   // Hotel Row, gentle
Street(253, 254, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 3, 0.6f, 4, 1, 1.f, 1.f);   // Hotel Row, gentle
Street(277, 278, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 3, 0.6f, 4, 1, 1.f, 1.f);   // Hotel Row, gentle
Street(218, 218, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 3, 0.f, 4, 1, 0.f, 0.f);   // Hotel Row, a slope
Street(221, 221, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 3, 0.f, 4, 1, 0.f, 0.f);   // Hotel Row, a slope
Street(238, 242, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 3, 0.f, 4, 1, 0.f, 0.f);   // Hotel Row, a slope
Street(259, 260, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 3, 0.f, 4, 1, 0.f, 0.f);   // Hotel Row, a slope
Street(263, 266, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 3, 0.f, 4, 1, 0.f, 0.f);   // Hotel Row, a slope
Street(276, 276, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 3, 0.f, 4, 1, 0.f, 0.f);   // Hotel Row, a slope
Street(279, 280, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 3, 0.f, 4, 1, 0.f, 0.f);   // Hotel Row, a slope
Steep(219, 220);   // Hotel Row
Steep(251, 252);   // Hotel Row
Steep(255, 255);   // Hotel Row
Steep(258, 258);   // Hotel Row
Street(296, 304, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 0.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 1.f);   // The Point, level
Street(310, 318, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 0.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 1.f);   // The Point, level
Street(327, 327, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 0.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 1.f);   // The Point, level
Street(334, 345, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 0.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 1.f);   // The Point, level
Street(354, 359, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 0.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 1.f);   // The Point, level
Street(366, 370, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 0.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 1.f);   // The Point, level
Street(305, 309, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 0.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 1.f);   // The Point, gentle
Street(321, 321, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 0.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 1.f);   // The Point, gentle
Street(325, 326, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 0.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 1.f);   // The Point, gentle
Street(371, 371, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 0.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 1.f);   // The Point, gentle
Street(295, 295, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 0.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // The Point, a slope
Street(350, 353, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 0.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // The Point, a slope
Steep(294, 294);   // The Point
Steep(319, 320);   // The Point
Steep(322, 322);   // The Point
Street(377, 381, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // Downtown waterfront, level
Street(391, 396, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // Downtown waterfront, level
Street(408, 413, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // Downtown waterfront, level
Street(422, 427, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // Downtown waterfront, level
Street(440, 444, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // Downtown waterfront, level
Street(454, 459, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // Downtown waterfront, level
Street(472, 476, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // Downtown waterfront, level
Street(486, 491, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // Downtown waterfront, level
Street(504, 508, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // Downtown waterfront, level
Street(518, 523, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // Downtown waterfront, level
Street(536, 540, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // Downtown waterfront, level
Street(550, 555, TEXT("ShopRow"), TEXT("Front1"), TEXT("Front2"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSet"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 0.f, 1.f);   // Downtown waterfront, level
Street(376, 376, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // Downtown waterfront, gentle
Street(390, 390, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // Downtown waterfront, gentle
Street(403, 403, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // Downtown waterfront, gentle
Street(432, 432, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // Downtown waterfront, gentle
Street(515, 515, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // Downtown waterfront, gentle
Street(535, 535, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // Downtown waterfront, gentle
Street(549, 549, TEXT("ShopRowB"), TEXT("Front1B"), TEXT("Front2B"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 1.f, 4, 0, 1.f, 1.f);   // Downtown waterfront, gentle
Street(372, 375, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // Downtown waterfront, a slope
Street(404, 407, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // Downtown waterfront, a slope
Street(435, 439, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // Downtown waterfront, a slope
Street(464, 464, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // Downtown waterfront, a slope
Street(467, 471, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // Downtown waterfront, a slope
Street(496, 496, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // Downtown waterfront, a slope
Street(498, 503, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // Downtown waterfront, a slope
Street(517, 517, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // Downtown waterfront, a slope
Street(530, 534, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // Downtown waterfront, a slope
Street(547, 547, TEXT("ShopRowC"), TEXT("Front1C"), TEXT("Front2C"), 3, 2, 1, 1.f, 1.f, 1.f, TEXT("ArcadeSetB"), TEXT("WalkShopsB"), 2, 0.f, 4, 0, 0.f, 0.f);   // Downtown waterfront, a slope
Steep(388, 389);   // Downtown waterfront
Steep(401, 402);   // Downtown waterfront
Steep(420, 421);   // Downtown waterfront
Steep(433, 434);   // Downtown waterfront
Steep(452, 453);   // Downtown waterfront
Steep(465, 466);   // Downtown waterfront
Steep(484, 485);   // Downtown waterfront
Steep(497, 497);   // Downtown waterfront
Steep(516, 516);   // Downtown waterfront
Steep(528, 529);   // Downtown waterfront
Steep(548, 548);   // Downtown waterfront
Steep(560, 561);   // Downtown waterfront
ChairLift(142, 152);
ChairLift(160, 160);
ChairLift(169, 169);
ChairLift(174, 174);
ChairLift(176, 184);
ChairLift(187, 187);
ChairLift(191, 193);
ChairLift(199, 205);
ChairLift(214, 216);
ChairLift(376, 381);
ChairLift(390, 396);
ChairLift(403, 403);
ChairLift(408, 413);
ChairLift(422, 427);
ChairLift(432, 432);
ChairLift(440, 440);
// ---- The big wheel at the first banked bend, and the wheel on the pleasure pier (their sets are above).
Zone(TEXT("The big wheel"), 1.1537f, 1.1989f);
	Site(TEXT("WheelPark"), -1, 105, 1, 0, 1.f);
Zone(TEXT("The pier wheel"), 9.1617f, 9.2070f);
	Site(TEXT("PierWheel"), -1, 250, 1, 0, 1.f);
Zone(TEXT("A pier wheel"), 18.6627f, 18.7080f); Site(TEXT("PierWheel413"), -1, 250, 1, 0, 1.f);
Zone(TEXT("A pier wheel"), 19.6581f, 19.7033f); Site(TEXT("PierWheel435"), -1, 250, 1, 0, 1.f);
Zone(TEXT("A pier wheel"), 23.6395f, 23.6847f); Site(TEXT("PierWheel523"), -1, 250, 1, 0, 1.f);
// ---- Moving surf. The sea is the level's floor: its edge is 355 to 359 m from the road wherever the road goes (the
// beach's last 60 m fall to it). SM_Surf floats there, 135 m to a piece, one every third slot: a sheet of foam that
// slides up the sand and back, and breakers that come in from 230 m out (ft_life). From the road on the level the
// beach's own brow hides the water's edge (it shows from every crest, jump, loop and falling stretch), so a second
// line breaks on a bar 640 m out, where the sea is in view from the road as well.
Zone(TEXT("Surf"), 0.f, 99.f);
	Afloat(TEXT("SM_Surf"), 357, 357, 3, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Afloat(TEXT("SM_Surf"), 640, 640, 4, 2, 1.f, 1.5f, 0.f, -1, 4.f);          // a bar offshore, half as large again: this line shows from the road
// ---- The horizon. Behind downtown's solid towers a pale row: the far skyline as a flat card in the haze's lilac
// (SM_SkyCard, 560 m to a piece, 1.8 km out), from Hotel Row on: the skyline has a row behind its coloured ones.
// Only from slots where that place is 1.3 km and more from every other stretch of the road (make_l0.py).
auto SkyRow = [&](float S, float Size)
{
	Zone(TEXT("The far skyline"), SlotKm(S - 0.5f), SlotKm(S + 0.5f));
	Far(TEXT("SM_SkyCard"), 1800, 1800, 1, 0, 1.f, Size, 0.f, 1, 30.f, 6.f);
};
SkyRow(293, 1.062f); SkyRow(305, 1.f); SkyRow(309, 1.188f); SkyRow(313, 1.062f); SkyRow(317, 1.25f); SkyRow(321, 1.125f); SkyRow(325, 1.f); SkyRow(329, 1.188f); SkyRow(333, 1.062f); SkyRow(337, 1.25f); SkyRow(361, 1.125f); SkyRow(365, 1.f); SkyRow(373, 1.062f); SkyRow(393, 1.062f); SkyRow(405, 1.f); SkyRow(425, 1.f); SkyRow(437, 1.25f); SkyRow(457, 1.25f); SkyRow(469, 1.188f);

// ---- The cut banks (FTDress.h, "Bank"). Inside a banked bend the track cuts the land down to the road's low edge;
// the second pass laid 45 hand-tilted mosaic beds there (SM_BankWaves: one bank angle each, none on a bend's first
// 60 m, none under 40 degrees). They are gone: Bank rules have the track face every cutting itself, lead-in to
// lead-out, at any angle and on any grade. Bayshore's is a sea wall: cream render, two mosaic waves that run from
// panel to panel, suns, a planted strip under the kerb (SM_CutSea, cuttings of 5.5 m and more: banked 32 degrees and
// up); one wave between a teal foot and a white kerb on the shallow ones (SM_CutProm, down to 2.4 m: 19 degrees).
// ft.DressBank 0 leaves them out.
Zone(TEXT("Cut banks"), 0.f, 99.f);
	Bank(TEXT("SM_CutSea"), 4, 1, 0, 1.f, 5.5f);
	Bank(TEXT("SM_CutProm"), 4, 1, 0, 1.f, 1.f, 5.5f);                  // from 1 m of cutting over the barrier, the least the track will face (bends banked 12 degrees and up; round one stopped at 2.4 m and left 2.4 km of pale wall)
// ---- Downtown's second block of towers (SM_Downtown2: twin towers and their bridge, a spire, a sail, a needle with a
// ball). One block repeated read as a comb of equal towers: the three rows that place SM_Downtown keep every second
// place (their rules, far above, count twice as far now) and these take the others, each at its own size and with
// its long side to the road. Written after the table's last rule, so that no rule before them changes its place.
Zone(TEXT("The Point, a second skyline"), 13.2f, 16.8f);
	Far(TEXT("SM_Downtown2"), 560, 700, 16, 11, 1.f, 0.9f, 0.12f, 1, 2.5f, 25.f);
Zone(TEXT("Downtown, a second skyline"), 16.8f, 99.f);
	Far(TEXT("SM_Downtown2"), 430, 560, 10, 6, 1.f, 1.05f, 0.2f, 1, 2.2f, 30.f);
	Far(TEXT("SM_Downtown2"), 760, 1000, 12, 9, 1.f, 1.95f, 0.2f, 1, 6.f, 30.f);
