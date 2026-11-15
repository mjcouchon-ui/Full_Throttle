// 3.2 dressing table, level 0: Bayshore. The words (Set, Zone, Scatter, Site, Far, Afloat, SiteAfloat, Lit) are explained
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
Set(TEXT("Kerb2"), { { TEXT("SM_ParkedCars2"), 0, 0, 0, 0, 1 } });
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
	{ TEXT("SM_Towels2"),   -20,  6, 0,   0, 1 },
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
});
Set(TEXT("Marina"), {                                     // afloat: the anchor stands at sea level
	{ TEXT("SM_MarinaDock"),   0,  0, 0,   0, 1 },
	{ TEXT("SM_MotorYacht"),  20, 22, 0,  25, 1 },
	{ TEXT("SM_Sailboat"),   -24, 26, 0, -15, 1 },
	{ TEXT("SM_Sailboat2"),   -6, 44, 0,  40, 1 },
	{ TEXT("SM_MotorYacht"), -44,  8, 0, 160, 1 },
	{ TEXT("SM_Sailboat"),    44, 40, 0,  70, 1 },
});

// ---- 0 to 3.2 km: THE STRIP. The densest stretch: an unbroken boardwalk, a beach club or a bar every 90 m, the
// kerb full of cars, cafes and markets among the palms, shop blocks, and the new hotels crowding in behind.
Zone(TEXT("The Strip"), 0.0f, 3.2f);
	Site(TEXT("Walk"), -1, 22, 1, 0, 1.f);
	Site(TEXT("BeachClub"), -1, 34, 4, 1, 0.95f);
	Site(TEXT("Tiki"), -1, 52, 4, 3, 0.9f);
	Scatter(TEXT("SM_Towels"), 30, 100, 1, 0, 0.7f, 1.f, 0.f, -1);
	Scatter(TEXT("SM_Towels2"), 34, 104, 2, 1, 0.8f, 1.f, 0.f, -1);
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
	Scatter(TEXT("SM_Towels2"), 30, 100, 2, 0, 0.7f, 1.f, 0.f, -1);
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
	Scatter(TEXT("SM_Towels2"), 34, 104, 1, 0, 0.6f, 1.f, 0.f, -1);
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
	Scatter(TEXT("SM_Sunbeds"), 30, 70, 1, 0, 0.7f, 1.f, 0.f, -1, 40.f);
	Scatter(TEXT("SM_Towels"), 34, 100, 2, 0, 0.7f, 1.f, 0.f, -1);
	Scatter(TEXT("SM_DuneGrass"), 80, 270, 1, 0, 0.85f, 1.3f, 0.3f, -1, 180.f);
	Site(TEXT("Tiki"), -1, 56, 7, 3, 0.9f);
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
