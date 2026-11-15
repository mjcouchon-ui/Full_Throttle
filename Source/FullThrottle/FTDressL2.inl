// 3.2 dressing table, level 2: Nevada. The words (Set, Zone, Scatter, Site, Far, Lit) are explained at the top of
// FTTrackDress.cpp; the meshes are made by Content/Python/ft_dress_nevada.py. Distances across are metres from the road's
// centre line; districts are in kilometres along the road.
//
// Highway 93, stretch by stretch, following the level's own scenes: the state line, the dry wash and its fallen
// bridge, Snake Eyes country (the roadhouse, the camps, the scrapyards), Joshua Flats under a power corridor, Red
// Rock Pass between canyon walls, Last Chance Gas with the Syndicate's hide out in the rocks behind it, Jackpot
// Junction, the run in under the city lights, and four more stretches for the finale. The ground cannot change its
// profile from stretch to stretch, so the land itself is dressed: cut banks by the road, low ridges, canyon walls,
// arches and hoodoos in the middle distance, mesas, monument buttes and far ranges on the horizon.
// Within 24 m of the centre line there are only low, unlit things, none higher than the road's own barrier: ranch
// fence, sage, junk and the wash's bed. The cut banks stand from 24 m out, the phone line 30 m out (a Site, like the
// ranch fence: a Site stands exactly on its slot, so the wires and the strands join from piece to piece).

// The table is written as three parts (first, second and third pass), each the body of a lambda called at once, in
// order. Nothing else differs from one plain list: FTDress::Build holds all eight tables in one function body, and on
// 15 Nov 2026 that body passed the size at which the compiler stops optimising a function (error C4883 in every
// worker's compile). As lambdas the parts are functions of their own. If FTTrackDress.cpp is changed to give each
// level's table its own function, the three wrappers here can simply be taken out.
auto FirstPass = [&]()
{
Set(TEXT("StateLine"), {
	{ TEXT("SM_Garage"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_SignFrame"), -24, -8, 0, 0, 1.f },
	{ TEXT("SM_WaterTank"), 22, 10, 0, 0, 1.f },
	{ TEXT("SM_Trailer"), 30, -6, 0, 0, 1.f },
	{ TEXT("SM_WireFence"), -6, -22, 0, 0, 1.f },
	{ TEXT("SM_WireFence"), 12, -22, 0, 0, 1.f },
	{ TEXT("SM_MileSign"), -16, -18, 0, 0, 1.f },
});
Set(TEXT("RoadStop"), {
	{ TEXT("SM_Diner"), 0, 0, 0, 0, 1.f },
	{ TEXT("/Game/Maps/Nevada/Props/SM_Gas"), 30, 2, 0, 90, 1.f },
	{ TEXT("SM_SignFrame"), -26, -10, 0, 0, 1.f },
	{ TEXT("SM_JunkPile"), 44, 14, 0, 0, 1.f },
	{ TEXT("SM_Trailer"), -24, 22, 0, 0, 1.f },
	{ TEXT("SM_WaterTank"), 14, 26, 0, 0, 1.f },
	{ TEXT("SM_WreckCar"), -34, 8, 0, 30, 1.f },
});
Set(TEXT("BikerDen"), {
	{ TEXT("SM_Roadhouse"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_BikerCamp"), 26, 12, 0, 0, 1.f },
	{ TEXT("SM_BikerCamp"), -28, 16, 0, 140, 1.f },
	{ TEXT("SM_JunkPile"), -18, -4, 0, 0, 1.f },
	{ TEXT("SM_WreckCar"), 20, -10, 0, -25, 1.f },
	{ TEXT("SM_Trailer"), 8, 30, 0, 0, 1.f },
	{ TEXT("SM_SignFrame"), 38, -6, 0, 0, 1.f },
});
Set(TEXT("WashCrossing"), {
	{ TEXT("SM_BridgeRuin"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_RockBank"), -34, -8, 0, 0, 1.f },
	{ TEXT("SM_RockBank2"), 30, -8, 0, 0, 1.f },
	{ TEXT("SM_RockBank"), 2, 16, 0, 180, 1.2f },
	{ TEXT("SM_RockSpires"), -40, 22, 0, 0, 1.f },
	{ TEXT("SM_RockSpires2"), 44, 26, 0, 0, 0.8f },
});
Set(TEXT("PowerCorridor"), {
	{ TEXT("SM_PowerSpan"), -180, 0, 0, 0, 1.f },
	{ TEXT("SM_PowerSpan"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_PowerSpan"), 180, 0, 0, 0, 1.f },
});
Set(TEXT("Syndicate"), {
	{ TEXT("SM_Hangar"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_RadarPost"), 34, 6, 0, 0, 1.f },
	{ TEXT("SM_WireFence"), -18, -22, 0, 0, 1.f },
	{ TEXT("SM_WireFence"), 0, -22, 0, 0, 1.f },
	{ TEXT("SM_WireFence"), 18, -22, 0, 0, 1.f },
	{ TEXT("SM_RockSpires"), -34, 4, 0, 0, 1.f },
	{ TEXT("SM_RockWall2"), 6, 34, 0, 0, 1.f },
});
Set(TEXT("Canyon"), {
	{ TEXT("SM_RockWall"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_RockWall2"), 60, 8, 0, 0, 1.f },
	{ TEXT("SM_RockSpires"), -20, -22, 0, 0, 1.f },
	{ TEXT("SM_RockSpires2"), 30, -26, 0, 0, 1.f },
	{ TEXT("SM_RockBank"), 4, -34, 0, 0, 1.f },
});
Set(TEXT("Jackpot"), {
	{ TEXT("SM_Casino"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_Chapel"), -34, -8, 0, 0, 1.f },
	{ TEXT("/Game/Maps/Nevada/Props/SM_Pylon"), 26, -20, 0, 90, 1.f },
	{ TEXT("SM_SignFrame"), 40, -4, 0, 0, 1.f },
	{ TEXT("SM_Trailer"), -30, 22, 0, 0, 1.f },
	{ TEXT("SM_Diner"), 44, 20, 0, 0, 1.f },
});
Set(TEXT("Scrapyard"), {
	{ TEXT("SM_Garage"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_WreckCar"), -22, -6, 0, 20, 1.f },
	{ TEXT("SM_WreckCar"), -26, 6, 0, -40, 1.f },
	{ TEXT("SM_WreckCar"), 22, -8, 0, 75, 1.f },
	{ TEXT("SM_JunkPile"), 18, 6, 0, 0, 1.f },
	{ TEXT("SM_JunkPile"), -14, 14, 0, 90, 1.2f },
	{ TEXT("SM_WaterTank"), 30, 14, 0, 0, 0.8f },
});

Set(TEXT("PhoneLine"), {
	{ TEXT("SM_WireLine"), 0, 0, 0, 0, 1.f },
});
Set(TEXT("RanchFence"), {
	{ TEXT("SM_RanchRow"), 0, 0, 0, 0, 1.f },   // third pass: one slot-long piece (the fence, the shoulder, the marker posts); it was three 15 m fences
});

Set(TEXT("WashBedRun"), {
	{ TEXT("SM_WashBed"), 0, 0, 0, 90, 1.f },
});

// ---- the state line: fence, a phone line, the first roadside business, low mesas a long way off
Zone(TEXT("State line"), 0.0f, 1.17f);
	Site(TEXT("StateLine"), 0, 62.f, 4, 1, 1.f);
	Site(TEXT("RanchFence"), -1, 18.9f, 1, 0, 0.75f);
	Site(TEXT("RanchFence"), 1, 18.9f, 1, 0, 0.75f);
	Site(TEXT("PhoneLine"), -1, 30.f, 1, 0, 0.92f);
	Scatter(TEXT("SM_Sagebrush"), 20.f, 60.f, 1, 0, 0.8f, 1.f, 0.25f, 0, 180.f);
	Scatter(TEXT("SM_Sagebrush2"), 60.f, 160.f, 1, 0, 0.8f, 1.3f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_MileSign"), 24.6f, 27.5f, 4, 2, 0.9f);
	Scatter(TEXT("SM_RidgeDune"), 130.f, 250.f, 3, 1, 0.8f, 1.f, 0.25f, 0, 25.f);
	Scatter(TEXT("SM_Sagebrush2"), 22.f, 110.f, 1, 0, 0.9f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_Sagebrush"), 150.f, 290.f, 1, 0, 0.9f, 1.6f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_WreckCar"), 28.f, 50.f, 6, 3, 0.8f, 1.f, 0.f, 0, 180.f);
	Scatter(TEXT("SM_Trailer"), 80.f, 130.f, 5, 2, 0.8f, 1.f, 0.f, 0, 40.f);
	Scatter(TEXT("SM_RockWall2"), 245.f, 290.f, 5, 3, 0.8f, 1.f, 0.2f);
	Far(TEXT("SM_ButteFar"), 520.f, 820.f, 7, 0, 0.9f, 0.9f, 0.2f);
	Far(TEXT("SM_PeakRange"), 1200.f, 1700.f, 10, 4, 1.f, 1.f, 0.1f);

// ---- the dry wash: cut banks right by the road, the old bridge down, hoodoos, monument buttes
Zone(TEXT("Dry wash"), 1.17f, 2.43f);
	Site(TEXT("WashCrossing"), 0, 64.f, 4, 1, 1.f);
	Scatter(TEXT("SM_RockBank"), 28.5f, 35.f, 1, 0, 0.8f);
	Scatter(TEXT("SM_RockBank2"), 40.f, 70.f, 1, 0, 0.7f, 1.2f, 0.2f);
	Scatter(TEXT("SM_Sagebrush"), 20.f, 70.f, 1, 0, 0.7f, 1.f, 0.25f, 0, 180.f);
	Scatter(TEXT("SM_WreckCar"), 30.f, 60.f, 6, 3, 0.8f, 1.f, 0.f, 0, 180.f);
	Scatter(TEXT("SM_RockSpires"), 90.f, 200.f, 3, 0, 0.85f, 1.2f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RockSpires2"), 120.f, 260.f, 3, 2, 0.85f, 1.5f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_Sagebrush2"), 70.f, 200.f, 1, 0, 0.9f, 1.4f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RockBank2"), 80.f, 150.f, 2, 0, 0.7f, 1.5f, 0.2f, 0, 30.f);
	Scatter(TEXT("SM_RidgeDune"), 160.f, 280.f, 2, 1, 0.8f, 1.f, 0.25f, 0, 25.f);
	Site(TEXT("WashBedRun"), -1, 52.f, 7, 3, 1.f);
	Site(TEXT("WashBedRun"), 1, 52.f, 7, 3, 1.f);
	Far(TEXT("SM_ButteSpires"), 390.f, 660.f, 5, 1, 0.9f, 1.f, 0.2f);
	Far(TEXT("SM_ButteFar2"), 820.f, 1250.f, 6, 3, 0.9f, 1.2f, 0.15f);

// ---- Snake Eyes country: the roadhouse, camps off the road, scrapyards, wrecks in the sage
Zone(TEXT("Snake Eyes country"), 2.43f, 3.83f);
	Site(TEXT("BikerDen"), 0, 70.f, 4, 0, 1.f);
	Site(TEXT("Scrapyard"), 0, 62.f, 4, 2, 0.95f);
	Scatter(TEXT("SM_JunkPile"), 22.f, 40.f, 2, 0, 0.6f, 1.f, 0.15f, 0, 180.f);
	Scatter(TEXT("SM_WreckCar"), 26.f, 50.f, 3, 1, 0.7f, 1.f, 0.f, 0, 180.f);
	Site(TEXT("PhoneLine"), 1, 30.f, 1, 0, 0.92f);
	Scatter(TEXT("SM_Sagebrush2"), 20.f, 90.f, 1, 0, 0.8f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_BikerCamp"), 90.f, 170.f, 3, 1, 0.8f, 1.f, 0.f, 0, 180.f);
	Scatter(TEXT("SM_RidgeDune"), 180.f, 270.f, 4, 2, 0.8f, 1.f, 0.25f, 0, 25.f);
	Scatter(TEXT("SM_Sagebrush"), 90.f, 240.f, 1, 0, 0.9f, 1.5f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_Trailer"), 60.f, 120.f, 4, 3, 0.8f, 1.f, 0.f, 0, 40.f);
	Scatter(TEXT("SM_WaterTank"), 100.f, 180.f, 6, 2, 0.9f);
	Scatter(TEXT("SM_RockSpires2"), 200.f, 290.f, 3, 1, 0.8f, 1.3f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RockWall"), 240.f, 290.f, 4, 2, 0.8f, 1.1f, 0.2f);
	Far(TEXT("SM_ButteSpires"), 430.f, 720.f, 6, 2, 0.9f, 1.1f, 0.2f);
	Far(TEXT("SM_PeakRange"), 1100.f, 1600.f, 9, 0, 1.f, 1.f, 0.1f);

// ---- Joshua Flats: the trees thick on the flat, the power corridor marching through them, one long mesa
Zone(TEXT("Joshua Flats"), 3.83f, 5.14f);
	Site(TEXT("PowerCorridor"), 1, 115.f, 12, 3, 1.f);
	Site(TEXT("PowerCorridor"), -1, 190.f, 12, 9, 1.f);
	Scatter(TEXT("/Game/Maps/Nevada/Props/SM_Joshua"), 24.f, 150.f, 1, 0, 0.9f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("/Game/Maps/Nevada/Props/SM_Joshua2"), 40.f, 240.f, 1, 0, 0.9f, 1.2f, 0.3f, 0, 180.f);
	Scatter(TEXT("/Game/Maps/Nevada/Props/SM_Joshua"), 150.f, 280.f, 1, 0, 0.8f, 1.3f, 0.3f, 0, 180.f);
	Scatter(TEXT("/Game/Maps/Nevada/Props/SM_Joshua2"), 24.f, 90.f, 1, 0, 0.85f, 0.9f, 0.3f, 0, 180.f);
	Scatter(TEXT("/Game/Maps/Nevada/Props/SM_Joshua"), 60.f, 200.f, 1, 0, 0.85f, 1.1f, 0.35f, 0, 180.f);
	Scatter(TEXT("/Game/Maps/Nevada/Props/SM_Joshua2"), 100.f, 280.f, 1, 0, 0.85f, 1.4f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_Sagebrush"), 20.f, 120.f, 1, 0, 0.9f, 1.f, 0.3f, 0, 180.f);
	Site(TEXT("RanchFence"), -1, 18.9f, 2, 1, 0.7f);
	Site(TEXT("RanchFence"), 1, 18.9f, 2, 1, 0.7f);
	Scatter(TEXT("SM_RidgeDune"), 150.f, 260.f, 3, 0, 0.8f, 1.f, 0.25f, 0, 25.f);
	Scatter(TEXT("SM_Sagebrush2"), 120.f, 290.f, 1, 0, 0.9f, 1.5f, 0.3f, 0, 180.f);
	Site(TEXT("PhoneLine"), 1, 30.f, 1, 0, 0.92f);
	Far(TEXT("SM_ButteFar"), 520.f, 820.f, 5, 1, 0.9f, 1.3f, 0.15f);
	Far(TEXT("SM_PeakRange"), 1000.f, 1500.f, 8, 4, 1.f, 1.f, 0.1f);

// ---- Red Rock Pass: canyon walls both sides, hoodoos at their feet, an arch now and then
Zone(TEXT("Red Rock Pass"), 5.14f, 6.31f);
	Site(TEXT("Canyon"), 0, 100.f, 3, 0, 1.f);
	Scatter(TEXT("SM_RockWall"), 170.f, 250.f, 2, 0, 0.9f, 1.25f, 0.2f);
	Scatter(TEXT("SM_RockWall2"), 110.f, 160.f, 2, 1, 0.8f, 1.1f, 0.2f);
	Scatter(TEXT("SM_RockBank"), 28.5f, 35.f, 1, 0, 0.8f);
	Scatter(TEXT("SM_RockSpires"), 44.f, 80.f, 2, 1, 0.8f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RockArch"), 70.f, 120.f, 6, 3, 0.9f, 1.f, 0.15f, 0, 30.f);
	Scatter(TEXT("/Game/Maps/Nevada/Props/SM_Rock2"), 25.f, 60.f, 1, 0, 0.6f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_Sagebrush"), 24.f, 80.f, 1, 0, 0.8f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RockSpires2"), 60.f, 110.f, 2, 0, 0.8f, 1.2f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RockWall2"), 240.f, 290.f, 2, 0, 0.8f, 1.5f, 0.2f);
	Site(TEXT("WashBedRun"), -1, 52.f, 12, 5, 1.f);
	Site(TEXT("WashBedRun"), 1, 52.f, 12, 5, 1.f);
	Far(TEXT("SM_ButteSpires"), 360.f, 560.f, 3, 0, 0.95f, 1.2f, 0.2f);
	Far(TEXT("SM_ButteFar2"), 700.f, 1050.f, 4, 2, 0.9f, 1.3f, 0.15f);

// ---- Last Chance Gas: roadside stops, trailers and tanks, and the Syndicate's hide out in the rocks behind them
Zone(TEXT("Last Chance Gas"), 6.31f, 7.75f);
	Site(TEXT("RoadStop"), 0, 64.f, 4, 0, 1.f);
	Site(TEXT("Syndicate"), -1, 160.f, 6, 3, 1.f);
	Site(TEXT("Syndicate"), 1, 175.f, 8, 6, 1.f);
	Site(TEXT("PhoneLine"), -1, 30.f, 1, 0, 0.92f);
	Scatter(TEXT("SM_SignFrame"), 30.f, 45.f, 3, 1, 0.8f);
	Scatter(TEXT("SM_JunkPile"), 22.f, 36.f, 3, 2, 0.6f, 1.f, 0.15f, 0, 180.f);
	Scatter(TEXT("SM_Trailer"), 70.f, 130.f, 3, 0, 0.8f, 1.f, 0.f, 0, 40.f);
	Scatter(TEXT("SM_WaterTank"), 90.f, 160.f, 5, 2, 0.9f);
	Scatter(TEXT("SM_Sagebrush"), 20.f, 90.f, 1, 0, 0.7f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RadarPost"), 210.f, 280.f, 5, 4, 0.9f);
	Scatter(TEXT("SM_WreckCar"), 28.f, 60.f, 4, 1, 0.7f, 1.f, 0.f, 0, 180.f);
	Scatter(TEXT("SM_Sagebrush2"), 90.f, 260.f, 1, 0, 0.9f, 1.5f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RidgeDune"), 120.f, 220.f, 4, 1, 0.8f, 1.f, 0.25f, 0, 25.f);
	Scatter(TEXT("SM_RockSpires"), 180.f, 290.f, 3, 2, 0.8f, 1.2f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RockWall2"), 235.f, 290.f, 3, 0, 0.8f, 1.2f, 0.2f);
	Far(TEXT("SM_ButteFar"), 420.f, 700.f, 6, 2, 0.9f, 1.1f, 0.2f);
	Far(TEXT("SM_PeakRange"), 1100.f, 1600.f, 9, 5, 1.f, 1.f, 0.1f);

// ---- Jackpot Junction: the casinos and their signs, a chapel, a neon town down the valley
Zone(TEXT("Jackpot Junction"), 7.75f, 9.05f);
	Site(TEXT("Jackpot"), 0, 74.f, 3, 0, 1.f);
	Scatter(TEXT("SM_SignFrame"), 32.f, 46.f, 3, 1, 0.8f);
	Scatter(TEXT("SM_Chapel"), 52.f, 90.f, 4, 2, 0.8f);
	Lit();
	Scatter(TEXT("SM_Casino"), 160.f, 250.f, 3, 1, 0.8f, 1.1f, 0.15f);
	Site(TEXT("PhoneLine"), 1, 30.f, 1, 0, 0.92f);
	Scatter(TEXT("SM_MileSign"), 24.6f, 27.5f, 3, 0, 0.9f);
	Scatter(TEXT("SM_Sagebrush2"), 20.f, 80.f, 1, 0, 0.6f, 1.f, 0.3f, 0, 180.f);
	Site(TEXT("PowerCorridor"), -1, 200.f, 12, 5, 1.f);
	Scatter(TEXT("SM_Trailer"), 60.f, 120.f, 3, 2, 0.7f, 1.f, 0.f, 0, 40.f);
	Scatter(TEXT("SM_Sagebrush"), 80.f, 250.f, 1, 0, 0.8f, 1.5f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_Diner"), 110.f, 160.f, 5, 3, 0.8f);
	Scatter(TEXT("SM_WaterTank"), 120.f, 200.f, 6, 4, 0.9f);
	Far(TEXT("SM_FarTown"), 430.f, 700.f, 4, 0, 0.9f, 1.f, 0.15f);
	Far(TEXT("SM_ButteSpires"), 900.f, 1350.f, 7, 3, 0.9f, 1.3f, 0.15f);

// ---- the city lights: hoardings and trailers thickening towards town, the neon strung along the horizon
Zone(TEXT("City lights"), 9.05f, 10.69f);
	Site(TEXT("RoadStop"), 0, 66.f, 5, 0, 1.f);
	Site(TEXT("Jackpot"), 0, 80.f, 5, 2, 1.f);
	Site(TEXT("PowerCorridor"), 1, 150.f, 12, 7, 1.f);
	Scatter(TEXT("SM_SignFrame"), 32.f, 48.f, 2, 0, 0.8f);
	Scatter(TEXT("SM_Trailer"), 60.f, 140.f, 2, 1, 0.8f, 1.f, 0.f, 0, 40.f);
	Site(TEXT("PhoneLine"), -1, 30.f, 1, 0, 0.92f);
	Scatter(TEXT("SM_Sagebrush"), 20.f, 80.f, 1, 0, 0.6f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_Casino"), 190.f, 270.f, 4, 3, 0.8f, 1.2f, 0.15f);
	Scatter(TEXT("SM_Chapel"), 52.f, 90.f, 5, 3, 0.8f);
	Scatter(TEXT("SM_Sagebrush2"), 80.f, 260.f, 1, 0, 0.8f, 1.5f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_Garage"), 100.f, 160.f, 5, 1, 0.8f);
	Scatter(TEXT("SM_WaterTank"), 120.f, 220.f, 6, 2, 0.9f);
	Far(TEXT("SM_FarTown"), 380.f, 640.f, 3, 1, 0.95f, 1.15f, 0.15f);
	Far(TEXT("SM_PeakRange"), 1200.f, 1700.f, 9, 2, 1.f, 1.f, 0.1f);

// ---- the finale: four long stretches under the carrier
Zone(TEXT("Salt flats"), 10.69f, 14.5f);
	Site(TEXT("PowerCorridor"), 1, 120.f, 12, 2, 1.f);
	Site(TEXT("PowerCorridor"), -1, 170.f, 12, 8, 1.f);
	Site(TEXT("RanchFence"), -1, 18.9f, 1, 0, 0.75f);
	Site(TEXT("RanchFence"), 1, 18.9f, 1, 0, 0.75f);
	Scatter(TEXT("SM_Sagebrush"), 20.f, 120.f, 1, 0, 0.8f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_Sagebrush2"), 100.f, 260.f, 1, 0, 0.8f, 1.4f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RidgeDune"), 140.f, 260.f, 2, 0, 0.8f, 1.f, 0.25f, 0, 25.f);
	Scatter(TEXT("SM_MileSign"), 24.6f, 27.5f, 5, 2, 0.9f);
	Scatter(TEXT("SM_Sagebrush2"), 22.f, 90.f, 1, 0, 0.8f, 1.f, 0.3f, 0, 180.f);
	Site(TEXT("PhoneLine"), -1, 30.f, 1, 0, 0.92f);
	Scatter(TEXT("SM_WreckCar"), 30.f, 70.f, 7, 3, 0.8f, 1.f, 0.f, 0, 180.f);
	Far(TEXT("SM_ButteFar2"), 700.f, 1050.f, 8, 0, 0.9f, 1.3f, 0.15f);
	Far(TEXT("SM_PeakRange"), 1100.f, 1600.f, 7, 3, 1.f, 1.f, 0.1f);
Zone(TEXT("Syndicate range"), 14.5f, 18.5f);
	Site(TEXT("Syndicate"), 0, 150.f, 4, 0, 1.f);
	Scatter(TEXT("SM_RadarPost"), 90.f, 200.f, 3, 1, 0.8f);
	Site(TEXT("RanchFence"), -1, 18.9f, 1, 0, 0.8f);
	Site(TEXT("RanchFence"), 1, 18.9f, 1, 0, 0.8f);
	Scatter(TEXT("SM_RockSpires"), 40.f, 58.f, 4, 0, 0.8f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RockBank2"), 28.5f, 40.f, 2, 1, 0.7f);
	Scatter(TEXT("SM_Sagebrush"), 20.f, 90.f, 1, 0, 0.7f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_Sagebrush2"), 90.f, 270.f, 1, 0, 0.9f, 1.5f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RidgeDune"), 110.f, 200.f, 3, 0, 0.8f, 1.f, 0.25f, 0, 25.f);
	Scatter(TEXT("SM_RockSpires2"), 120.f, 220.f, 3, 1, 0.8f, 1.3f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RockWall2"), 230.f, 290.f, 3, 2, 0.8f, 1.3f, 0.2f);
	Far(TEXT("SM_ButteSpires"), 400.f, 680.f, 5, 1, 0.9f, 1.1f, 0.2f);
	Far(TEXT("SM_ButteFar2"), 850.f, 1250.f, 6, 3, 0.9f, 1.3f, 0.15f);
Zone(TEXT("Red rock return"), 18.5f, 22.0f);
	Site(TEXT("Canyon"), 0, 105.f, 4, 0, 1.f);
	Scatter(TEXT("SM_RockWall2"), 180.f, 260.f, 3, 1, 0.9f, 1.3f, 0.2f);
	Scatter(TEXT("SM_RockBank"), 28.5f, 35.f, 1, 0, 0.7f);
	Scatter(TEXT("SM_RockSpires2"), 44.f, 90.f, 4, 0, 0.8f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RockArch"), 70.f, 130.f, 7, 4, 0.9f, 1.f, 0.15f, 0, 30.f);
	Scatter(TEXT("SM_Sagebrush2"), 20.f, 80.f, 1, 0, 0.6f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_Sagebrush"), 60.f, 200.f, 1, 0, 0.8f, 1.4f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RockSpires"), 100.f, 180.f, 4, 1, 0.8f, 1.3f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RidgeDune"), 130.f, 220.f, 4, 2, 0.8f, 1.f, 0.25f, 0, 25.f);
	Scatter(TEXT("SM_RockWall"), 230.f, 290.f, 3, 0, 0.8f, 1.4f, 0.2f);
	Site(TEXT("WashBedRun"), -1, 52.f, 14, 6, 1.f);
	Site(TEXT("WashBedRun"), 1, 52.f, 14, 6, 1.f);
	Far(TEXT("SM_ButteFar"), 420.f, 700.f, 4, 2, 0.9f, 1.1f, 0.2f);
	Far(TEXT("SM_ButteSpires"), 800.f, 1200.f, 6, 0, 0.9f, 1.3f, 0.15f);
Zone(TEXT("Vegas approach"), 22.0f, 25.6f);
	Site(TEXT("RoadStop"), 0, 66.f, 5, 0, 1.f);
	Site(TEXT("Jackpot"), 0, 80.f, 5, 3, 1.f);
	Scatter(TEXT("SM_SignFrame"), 32.f, 48.f, 2, 0, 0.8f);
	Site(TEXT("PhoneLine"), 1, 30.f, 1, 0, 0.92f);
	Scatter(TEXT("SM_Trailer"), 60.f, 140.f, 3, 1, 0.8f, 1.f, 0.f, 0, 40.f);
	Scatter(TEXT("SM_Sagebrush"), 20.f, 80.f, 1, 0, 0.6f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_Chapel"), 52.f, 90.f, 4, 2, 0.8f);
	Scatter(TEXT("SM_Sagebrush2"), 80.f, 260.f, 1, 0, 0.8f, 1.5f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_Garage"), 100.f, 160.f, 5, 4, 0.8f);
	Scatter(TEXT("SM_Casino"), 180.f, 270.f, 3, 1, 0.8f, 1.2f, 0.15f);
	Far(TEXT("SM_FarTown"), 380.f, 640.f, 3, 0, 0.95f, 1.15f, 0.15f);
	Far(TEXT("SM_PeakRange"), 1200.f, 1700.f, 9, 4, 1.f, 1.f, 0.1f);
};
FirstPass();

auto SecondPass = [&]()
{
// =====================================================================================================================
// SECOND PASS (15 Nov 2026): the ground plane, the motel courts, the casino approach, more builds of rock.
// Everything from here on is APPENDED on purpose: a rule's random stream is seeded by its place in this file, so a rule
// added among the districts above would re-deal every first-pass piece. These zones lie over the districts. (Three
// first-pass hoodoo rules above were thinned in place, every 2nd slot to every 4th: that keeps the survivors where
// they stood.)
//
// The lie of this road (measured: SkyScratch\DevLog\jam\art_pass2\w2\track_L2.json): a straight, level highway with
// four jumps (1.5, 4.6, 7.3, 9.7 km), three loops (3.1-3.3, 8.1-8.4, 9.9-10.1), two corkscrews (5.5-5.6, 8.5-8.6) and
// one pair of "wall" bends banked 54 degrees in Red Rock Pass (5.17-5.40 right-handed, 5.75-5.99 left-handed).
//
// 1. GROUND. Flat pieces (under 2 m). The track keeps Scatter and Site pieces out of loops, corkscrews and jump gaps,
//    which is where the camera sees most land. So, apart from the ranch tracks (Scatter, from 26 m: the track's rule
//    applies to them), the ground is laid by Far rules with a metre's drop (a Far piece stands "drop" below the
//    road's own level, and the land 26 to 115 m out lies 0.9 to 1.25 m below it), and so runs on under the loops as
//    the horizon does. Three rows: 26 to 46 m (scrub, braids of the wash), 41 to 62 m (the businesses' forecourts,
//    scrub), 76 to 105 m (scrub, and the Syndicate's strip). All unlit. Every piece still goes through the clearance
//    test. The ground zones stop short of the ramps and the jumps (a level piece would hang or drown on land that
//    steep, and a jump's run-up and landing stay open).
//    Within a zone the phases put a forecourt in front of each business set and keep scrub out of the sets' slots.
// 2. THE APPROACH. "Roadside businesses and worn motel courts, then an increasingly extravagant casino approach":
//    SM_MotelCourt from Last Chance Gas on; SM_SignTower and the two SM_CasinoGrand from Jackpot Junction, more of
//    them, nearer and larger stretch by stretch (every 4th and 6th slot at 0.85, then every 2nd and 3rd at 1.0, and in
//    the finale's last stretch every 2nd and 3rd at 1.15 to 1.2). The motel courts take the slots between the road
//    stops, both sides, through Last Chance Gas.
// 3. ROCK. The two wall bends get canyon wall, 1.7 times the size (60 m), on their OUTSIDE (the rolled camera looks up
//    there, and saw sky);
//    SM_RockSpires3 (fins and a balanced rock) and SM_MesaStub share the finale's two rock stretches with the hoodoos.

// ---- rock: the wall bends of Red Rock Pass, and a third and fourth build in the finale
Zone(TEXT("Wall bend right"), 5.17f, 5.4f);
	Scatter(TEXT("SM_RockWall"), 72.f, 90.f, 1, 0, 0.9f, 1.7f, 0.1f, -1);
	Scatter(TEXT("SM_MesaStub"), 120.f, 170.f, 2, 1, 1.f, 1.2f, 0.1f, -1);
Zone(TEXT("Corkscrew in the pass"), 5.4f, 5.75f);
	Far(TEXT("SM_MesaStub"), 120.f, 190.f, 2, 0, 0.9f, 1.3f, 0.15f, 0, 0.8f, 180.f);
Zone(TEXT("Wall bend left"), 5.75f, 5.99f);
	Scatter(TEXT("SM_RockWall"), 72.f, 90.f, 1, 0, 0.9f, 1.7f, 0.1f, 1);
	Scatter(TEXT("SM_MesaStub"), 120.f, 170.f, 2, 0, 1.f, 1.2f, 0.1f, 1);
Zone(TEXT("Rock: syndicate range"), 14.5f, 18.5f);
	Scatter(TEXT("SM_RockSpires3"), 40.f, 58.f, 4, 2, 0.8f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_MesaStub"), 125.f, 200.f, 5, 3, 0.8f, 1.f, 0.25f, 0, 180.f);
	Scatter(TEXT("SM_RockSpires3"), 110.f, 180.f, 6, 1, 0.8f, 1.4f, 0.3f, 0, 180.f);
Zone(TEXT("Rock: red rock return"), 18.5f, 22.f);
	Scatter(TEXT("SM_RockSpires3"), 44.f, 90.f, 4, 2, 0.8f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_MesaStub"), 110.f, 180.f, 4, 3, 0.8f, 1.1f, 0.25f, 0, 180.f);

// ---- the approach: motel courts, then the signs and the big houses, thicker and larger stretch by stretch
Zone(TEXT("Motels: last chance gas"), 6.31f, 7.75f);
	Scatter(TEXT("SM_MotelCourt"), 60.f, 66.f, 4, 2, 1.f, 1.f, 0.f, -1);
	Scatter(TEXT("SM_MotelCourt"), 60.f, 66.f, 4, 2, 1.f, 1.f, 0.f, 1);
Zone(TEXT("Approach: jackpot junction"), 7.75f, 9.05f);
	Scatter(TEXT("SM_SignTower"), 39.f, 45.f, 4, 1, 0.9f, 0.85f, 0.05f);   // third pass: 5 m further out (the sign stands across the road now)
	Far(TEXT("SM_CasinoGrand"), 120.f, 150.f, 6, 2, 1.f, 0.85f, 0.05f, 0, 0.9f, 8.f);
Zone(TEXT("Approach: city lights"), 9.05f, 10.69f);
	Scatter(TEXT("SM_SignTower"), 39.f, 46.f, 2, 1, 0.9f, 1.f, 0.08f);   // third pass: 5 m further out (the sign stands across the road now)
	Far(TEXT("SM_CasinoGrand"), 112.f, 150.f, 6, 1, 0.95f, 1.f, 0.06f, 0, 0.9f, 8.f);
	Far(TEXT("SM_CasinoGrand2"), 112.f, 150.f, 6, 4, 0.95f, 1.f, 0.06f, 0, 0.9f, 8.f);
	Scatter(TEXT("SM_MotelCourt"), 60.f, 66.f, 10, 6, 1.f, 1.f, 0.f, -1);
Zone(TEXT("Approach: vegas"), 22.f, 25.6f);
	Scatter(TEXT("SM_SignTower"), 40.f, 47.f, 2, 0, 0.95f, 1.15f, 0.1f);   // third pass: 6 m further out (the sign stands across the road now)
	Scatter(TEXT("SM_CasinoGrand"), 105.f, 140.f, 6, 1, 0.95f, 1.2f, 0.08f, 0, 8.f);
	Scatter(TEXT("SM_CasinoGrand2"), 105.f, 140.f, 6, 4, 0.95f, 1.2f, 0.08f, 0, 8.f);
	Scatter(TEXT("SM_MotelCourt"), 60.f, 66.f, 10, 6, 1.f, 1.f, 0.f, 1);

// ---- ground: ranch tracks (Scatter), then three rows by Far rules: 26-46 m, 41-62 m, 76-105 m
Zone(TEXT("Ground: state line"), 0.05f, 1.17f);
	Scatter(TEXT("SM_DirtTrack"), 43.f, 44.f, 6, 1, 0.9f);
	Far(TEXT("SM_ScrubPatch"), 36.f, 37.f, 2, 0, 0.75f, 1.f, 0.08f, 0, 0.95f, 25.f);
	Far(TEXT("SM_ScrubPatch2"), 52.f, 58.f, 4, 0, 0.8f, 1.f, 0.1f, 0, 1.f, 180.f);
	Far(TEXT("SM_WashBraid"), 52.f, 58.f, 4, 2, 0.7f, 1.f, 0.05f, 0, 1.f, 12.f);
	Far(TEXT("SM_ScrubPatch2"), 84.f, 94.f, 4, 1, 0.7f, 1.f, 0.1f, 0, 1.2f, 180.f);
Zone(TEXT("Ground: dry wash west"), 1.17f, 1.34f);
	Far(TEXT("SM_WashBraid"), 33.f, 34.f, 2, 1, 0.75f, 1.f, 0.05f, 0, 0.93f, 10.f);
	Far(TEXT("SM_ScrubPatch"), 51.f, 58.f, 2, 0, 0.8f, 1.f, 0.1f, 0, 1.f, 180.f);
Zone(TEXT("Ground: dry wash"), 1.64f, 2.43f);
	Far(TEXT("SM_WashBraid"), 33.f, 34.f, 2, 1, 0.75f, 1.f, 0.05f, 0, 0.93f, 10.f);
	Far(TEXT("SM_ScrubPatch"), 51.f, 58.f, 4, 0, 0.8f, 1.f, 0.1f, 0, 1.f, 180.f);
	Far(TEXT("SM_ScrubPatch2"), 52.f, 58.f, 4, 2, 0.7f, 1.f, 0.1f, 0, 1.f, 180.f);
	Far(TEXT("SM_ScrubPatch2"), 84.f, 94.f, 4, 1, 0.7f, 1.f, 0.1f, 0, 1.2f, 180.f);
Zone(TEXT("Ground: snake eyes country"), 2.43f, 3.83f);
	Scatter(TEXT("SM_DirtTrack"), 43.f, 44.f, 6, 3, 0.9f);
	Far(TEXT("SM_ScrubPatch"), 36.f, 37.f, 2, 1, 0.75f, 1.f, 0.08f, 0, 0.95f, 25.f);
	Far(TEXT("SM_ParkingLot"), 50.f, 52.f, 4, 0, 0.9f, 1.f, 0.03f, 0, 1.f, 5.f);
	Far(TEXT("SM_ScrubPatch2"), 52.f, 58.f, 2, 1, 0.7f, 1.f, 0.1f, 0, 1.f, 180.f);
Zone(TEXT("Ground: joshua flats"), 3.83f, 4.46f);
	Scatter(TEXT("SM_DirtTrack"), 43.f, 44.f, 7, 2, 0.9f);
	Far(TEXT("SM_ScrubPatch"), 36.f, 37.f, 2, 0, 0.75f, 1.f, 0.08f, 0, 0.95f, 25.f);
	Far(TEXT("SM_ScrubPatch2"), 52.f, 58.f, 2, 1, 0.75f, 1.f, 0.1f, 0, 1.f, 180.f);
	Far(TEXT("SM_ScrubPatch"), 84.f, 94.f, 4, 2, 0.6f, 1.f, 0.1f, 0, 1.2f, 180.f);
Zone(TEXT("Ground: joshua flats east"), 4.77f, 5.14f);
	Far(TEXT("SM_ScrubPatch"), 36.f, 37.f, 2, 0, 0.75f, 1.f, 0.08f, 0, 0.95f, 25.f);
	Far(TEXT("SM_ScrubPatch2"), 52.f, 58.f, 2, 1, 0.75f, 1.f, 0.1f, 0, 1.f, 180.f);
Zone(TEXT("Ground: red rock pass"), 5.14f, 5.97f);
	Far(TEXT("SM_WashBraid"), 33.f, 34.f, 3, 1, 0.75f, 1.f, 0.05f, 0, 0.93f, 10.f);
	Far(TEXT("SM_ScrubPatch2"), 52.f, 58.f, 3, 2, 0.8f, 1.f, 0.1f, 0, 1.f, 180.f);
	Far(TEXT("SM_ScrubPatch"), 51.f, 58.f, 3, 0, 0.6f, 1.f, 0.1f, 0, 1.f, 180.f);
Zone(TEXT("Ground: red rock pass east"), 6.07f, 6.31f);
	Far(TEXT("SM_WashBraid"), 33.f, 34.f, 3, 1, 0.75f, 1.f, 0.05f, 0, 0.93f, 10.f);
	Far(TEXT("SM_ScrubPatch2"), 52.f, 58.f, 3, 2, 0.8f, 1.f, 0.1f, 0, 1.f, 180.f);
Zone(TEXT("Ground: last chance gas"), 6.31f, 7.01f);
	Scatter(TEXT("SM_DirtTrack"), 43.f, 44.f, 8, 5, 0.9f);
	Far(TEXT("SM_ScrubPatch"), 36.f, 37.f, 2, 1, 0.7f, 1.f, 0.08f, 0, 0.95f, 25.f);
	Far(TEXT("SM_ParkingLot"), 50.f, 52.f, 4, 0, 0.95f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_ScrubPatch2"), 52.f, 58.f, 2, 1, 0.7f, 1.f, 0.1f, 0, 1.f, 180.f);
	Far(TEXT("SM_ScrubPatch2"), 86.f, 94.f, 4, 1, 0.6f, 1.f, 0.1f, 0, 1.2f, 180.f);
	Far(TEXT("SM_Airstrip"), 100.f, 104.f, 12, 9, 1.f, 1.f, 0.f, -1, 0.95f, 3.f);
Zone(TEXT("Ground: last chance gas east"), 7.39f, 7.75f);
	Far(TEXT("SM_ScrubPatch"), 36.f, 37.f, 2, 1, 0.7f, 1.f, 0.08f, 0, 0.95f, 25.f);
	Far(TEXT("SM_ParkingLot"), 50.f, 52.f, 4, 0, 0.95f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_ScrubPatch2"), 52.f, 58.f, 2, 1, 0.7f, 1.f, 0.1f, 0, 1.f, 180.f);
Zone(TEXT("Ground: jackpot junction"), 7.75f, 8.67f);
	Far(TEXT("SM_ScrubPatch"), 36.f, 37.f, 3, 1, 0.7f, 1.f, 0.08f, 0, 0.95f, 25.f);
	Far(TEXT("SM_ParkingLot"), 50.f, 52.f, 3, 0, 0.95f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_ParkingLot"), 52.f, 56.f, 3, 2, 0.6f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_ScrubPatch2"), 52.f, 58.f, 3, 1, 0.7f, 1.f, 0.1f, 0, 1.f, 180.f);
Zone(TEXT("Ground: jackpot junction east"), 8.75f, 9.05f);
	Far(TEXT("SM_ScrubPatch"), 36.f, 37.f, 3, 1, 0.7f, 1.f, 0.08f, 0, 0.95f, 25.f);
	Far(TEXT("SM_ParkingLot"), 50.f, 52.f, 3, 0, 0.95f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_ScrubPatch2"), 52.f, 58.f, 3, 1, 0.7f, 1.f, 0.1f, 0, 1.f, 180.f);
Zone(TEXT("Ground: city lights"), 9.05f, 9.47f);
	Far(TEXT("SM_ScrubPatch"), 36.f, 37.f, 2, 1, 0.6f, 1.f, 0.08f, 0, 0.95f, 25.f);
	Far(TEXT("SM_ParkingLot"), 50.f, 52.f, 5, 0, 0.95f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_ParkingLot"), 50.f, 52.f, 5, 2, 0.95f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_ParkingLot"), 52.f, 56.f, 5, 3, 0.7f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_ScrubPatch2"), 52.f, 58.f, 5, 4, 0.7f, 1.f, 0.1f, 0, 1.f, 180.f);
Zone(TEXT("Ground: city lights east"), 9.76f, 10.69f);
	Far(TEXT("SM_ScrubPatch"), 36.f, 37.f, 2, 1, 0.6f, 1.f, 0.08f, 0, 0.95f, 25.f);
	Far(TEXT("SM_ParkingLot"), 50.f, 52.f, 5, 0, 0.95f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_ParkingLot"), 50.f, 52.f, 5, 2, 0.95f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_ParkingLot"), 52.f, 56.f, 5, 3, 0.7f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_ScrubPatch2"), 52.f, 58.f, 5, 4, 0.7f, 1.f, 0.1f, 0, 1.f, 180.f);
Zone(TEXT("Ground: salt flats"), 10.69f, 14.5f);
	Scatter(TEXT("SM_DirtTrack"), 43.f, 44.f, 8, 3, 0.9f);
	Far(TEXT("SM_ScrubPatch"), 36.f, 37.f, 2, 0, 0.75f, 1.f, 0.08f, 0, 0.95f, 25.f);
	Far(TEXT("SM_WashBraid"), 33.f, 34.f, 4, 1, 0.7f, 1.f, 0.05f, 0, 0.93f, 10.f);
	Far(TEXT("SM_ScrubPatch2"), 52.f, 58.f, 2, 1, 0.75f, 1.f, 0.1f, 0, 1.f, 180.f);
	Far(TEXT("SM_WashBraid"), 52.f, 58.f, 4, 2, 0.7f, 1.f, 0.05f, 0, 1.f, 12.f);
	Far(TEXT("SM_ScrubPatch"), 86.f, 96.f, 4, 0, 0.6f, 1.f, 0.1f, 0, 1.2f, 180.f);
Zone(TEXT("Ground: syndicate range"), 14.5f, 18.5f);
	Scatter(TEXT("SM_DirtTrack"), 43.f, 44.f, 8, 6, 0.9f);
	Far(TEXT("SM_Airstrip"), 72.f, 73.f, 8, 0, 1.f, 1.f, 0.f, -1, 1.1f, 2.f);
	Far(TEXT("SM_Airstrip"), 72.f, 73.f, 8, 4, 1.f, 1.f, 0.f, 1, 1.1f, 2.f);
	Far(TEXT("SM_ScrubPatch"), 36.f, 37.f, 2, 1, 0.7f, 1.f, 0.08f, 0, 0.95f, 25.f);
	Far(TEXT("SM_ScrubPatch2"), 50.f, 51.f, 2, 1, 0.7f, 1.f, 0.08f, 0, 1.f, 180.f);
	Far(TEXT("SM_ScrubPatch"), 106.f, 114.f, 4, 2, 0.6f, 1.f, 0.1f, 0, 1.25f, 180.f);
Zone(TEXT("Ground: red rock return"), 18.5f, 22.f);
	Far(TEXT("SM_WashBraid"), 33.f, 34.f, 2, 1, 0.75f, 1.f, 0.05f, 0, 0.93f, 10.f);
	Far(TEXT("SM_ScrubPatch2"), 52.f, 58.f, 4, 1, 0.8f, 1.f, 0.1f, 0, 1.f, 180.f);
	Far(TEXT("SM_ScrubPatch"), 51.f, 58.f, 4, 3, 0.7f, 1.f, 0.1f, 0, 1.f, 180.f);
Zone(TEXT("Ground: vegas approach"), 22.f, 25.6f);
	Far(TEXT("SM_ScrubPatch"), 36.f, 37.f, 2, 1, 0.5f, 1.f, 0.08f, 0, 0.95f, 25.f);
	Far(TEXT("SM_ParkingLot"), 50.f, 52.f, 5, 0, 0.95f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_ParkingLot"), 50.f, 52.f, 5, 3, 0.95f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_ParkingLot"), 52.f, 56.f, 5, 4, 0.8f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_ParkingLot"), 52.f, 56.f, 5, 2, 0.7f, 1.f, 0.03f, 0, 1.f, 4.f);

// ---- THE CUT BANKS (15 Nov 2026, after the second pass). Inside a banked bend the track cuts the land down to the
// road's low edge; that cutting was the land's one plain colour, a third of the frame from the rolled camera, and no
// Scatter or Site piece can stand on it. Bank rules (FTDress.h) have the track lay panels on it itself, fitted to the
// bank's own angle and grade: four to a 45 m slot (11.3 m of road each), tall designs where a panel's run up the
// cutting is 5.5 m or more (bends banked 32 degrees and up), a low one on the shallower cuttings (down to 2.4 m: about
// 20 degrees of bank). Every and phase count panels here. ft.DressBank 0 leaves them out, for a before and an after.
// Nevada: the highway cut through the red rock. SM_CutStrata and SM_CutStrata2 (the same seven beds, so they meet
// from panel to panel; the second has sage on its ledges and fallen blocks) take alternate panels of the two wall
// bends of Red Rock Pass (5.2 and 5.8 km). SM_CutLedge on the shallow cuttings (4.3, 4.4, 7.8 and 8.0 km).
Zone(TEXT("Cut banks"), 0.f, 99.f);
	Bank(TEXT("SM_CutStrata"), 4, 2, 0, 1.f, 5.5f);
	Bank(TEXT("SM_CutStrata2"), 4, 2, 1, 1.f, 5.5f);
	Bank(TEXT("SM_CutLedge"), 4, 1, 0, 1.f, 2.4f, 5.5f);
};
SecondPass();

auto ThirdPass = [&]()
{
// =====================================================================================================================
// THIRD PASS (15 Nov 2026, the overhaul): dense, distinctive and alive at racing speed. "Close rock cuts, roadside
// stops, and long vistas, with windblown dust, spinning ventilation fans, and flickering signs." APPENDED, like the
// second pass. (Changed in place above, and saying so: the set RanchFence is one slot-long piece now, and the three
// rules for SM_SignTower stand it 5 m further out, because the sign now stands across the road.)
//
// What the chase camera sees of this road (the geometry: DevLog\jam\dress_chem_nevada.md, third pass): it is level
// and straight, so with the car in the middle the road's own barrier hides everything under 2 m nearer than 26 m and
// the ground itself out to 33 m. What reads at speed is what stands 24 to 45 m out and 3 m tall or more. So the pass
// alternates CLOSE and OPEN: rock cuts 15 m tall laid end to end 25 m out on both sides (the dry wash, Red Rock Pass,
// the three straights of Red rock return), then the flats left open to the far tableland with dust blowing across
// them and, on the salt flats, a freight train running beside the road.

// ---- sets
Set(TEXT("KerbStopLine"), { { TEXT("SM_KerbStop"), 0, 0, 0, 0, 1.f } });
Set(TEXT("CutLine"), { { TEXT("SM_RockCut"), 0, 0, 0, 0, 1.f } });
Set(TEXT("CutLineB"), { { TEXT("SM_RockWall2"), 0, 0, 0, 0, 0.87f } });
Set(TEXT("CliffLine"), { { TEXT("SM_RockCliff"), 0, 0, 0, 0, 1.f } });
Set(TEXT("Monument"), {
	{ TEXT("SM_RockMonument"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_RockSpires3"), 62, -34, 0, 0, 1.3f },
	{ TEXT("SM_RockSpires"), -64, -30, 0, 40, 1.2f },
});
Set(TEXT("RailLine"), { { TEXT("SM_FreightTrain"), 0, 0, 0, 0, 1.f } });
// A FACE THAT IS TO BE READ IS TURNED TO THE DRIVER. A piece stands square to its own road (its front looks across
// it), so a flat sign 35 m out is seen at 10 to 40 degrees from the driving seat, and a thing that stands dead ahead
// of a straight is seen edge-on. A set gives its part a turn: on the LEFT of the road (where the piece is turned
// round) +90 degrees would face straight back down the road, on the RIGHT -90; so each such set comes as a pair.
// The wheel (always on the left, dead ahead of a straight) is turned 65: it faces the driver who comes at it and still
// shows as an ellipse when it is passed. Signs are turned 75, the flat fronts 35.
Set(TEXT("Wheel"), { { TEXT("SM_WheelSign"), 0, 0, 0, 65, 1.f } });
Set(TEXT("WheelLot"), { { TEXT("SM_ParkingLot"), 0, 0, 0, 0, 1.f } });
Set(TEXT("NeonL"), { { TEXT("SM_NeonSign"), 0, 0, 0, 75, 1.f } });
Set(TEXT("NeonR"), { { TEXT("SM_NeonSign"), 0, 0, 0, -75, 1.f } });
Set(TEXT("FrontAL"), { { TEXT("SM_Facade1"), 0, 0, 0, 35, 1.f } });
Set(TEXT("FrontAR"), { { TEXT("SM_Facade1"), 0, 0, 0, -35, 1.f } });
Set(TEXT("FrontBL"), { { TEXT("SM_Facade2"), 0, 0, 0, 35, 1.f } });
Set(TEXT("FrontBR"), { { TEXT("SM_Facade2"), 0, 0, 0, -35, 1.f } });
Set(TEXT("TruckStopSet"), { { TEXT("SM_TruckStop"), 0, 0, 0, 0, 1.f } });

// ---- 1. the kerb in front of the roadside businesses (17.5 m): every second slot, the two sides in turn, so the low
// wall that says EAT comes round every 45 m on one side or the other; the ranch fence takes the slots between
Zone(TEXT("Kerb: snake eyes country"), 2.43f, 3.83f);
	Site(TEXT("KerbStopLine"), -1, 19.f, 2, 0, 0.9f);
	Site(TEXT("KerbStopLine"), 1, 19.f, 2, 1, 0.9f);
	Site(TEXT("RanchFence"), -1, 18.9f, 2, 1, 0.85f);
	Site(TEXT("RanchFence"), 1, 18.9f, 2, 0, 0.85f);
Zone(TEXT("Kerb: last chance gas to the city lights"), 6.31f, 10.69f);
	Site(TEXT("KerbStopLine"), -1, 19.f, 2, 0, 0.9f);
	Site(TEXT("KerbStopLine"), 1, 19.f, 2, 1, 0.9f);
	Site(TEXT("RanchFence"), -1, 18.9f, 2, 1, 0.85f);
	Site(TEXT("RanchFence"), 1, 18.9f, 2, 0, 0.85f);
Zone(TEXT("Kerb: vegas approach"), 22.f, 25.6f);
	Site(TEXT("KerbStopLine"), -1, 19.f, 2, 0, 0.9f);
	Site(TEXT("KerbStopLine"), 1, 19.f, 2, 1, 0.9f);
	Site(TEXT("RanchFence"), -1, 18.9f, 2, 1, 0.85f);
	Site(TEXT("RanchFence"), 1, 18.9f, 2, 0, 0.85f);

// ---- 2. close rock cuts (see the head of this pass). One zone to each unbroken run of slots; the slots where the wash's
// bed crosses under the road are left open, so the wash comes out through the wall
Zone(TEXT("Cut: dry wash west 1"), 1.154f, 1.335f);
	Site(TEXT("CutLine"), -1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLine"), 1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLineB"), -1, 36.5f, 3, 1, 0.8f);
	Site(TEXT("CutLineB"), 1, 36.5f, 3, 2, 0.8f);
Zone(TEXT("Cut: dry wash 1"), 1.606f, 1.742f);
	Site(TEXT("CutLine"), -1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLine"), 1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLineB"), -1, 36.5f, 3, 1, 0.8f);
	Site(TEXT("CutLineB"), 1, 36.5f, 3, 2, 0.8f);
Zone(TEXT("Cut: dry wash 2"), 1.787f, 2.059f);
	Site(TEXT("CutLine"), -1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLine"), 1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLineB"), -1, 36.5f, 3, 1, 0.8f);
	Site(TEXT("CutLineB"), 1, 36.5f, 3, 2, 0.8f);
Zone(TEXT("Cut: the corkscrew in the pass"), 5.452f, 5.633f);
	Far(TEXT("SM_RockCut"), 30.f, 30.f, 1, 0, 1.f, 1.f, 0.f, 0, 0.86f, 0.f);
Zone(TEXT("Cut: red rock pass 1"), 5.407f, 5.452f);
	Site(TEXT("CutLine"), -1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLine"), 1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLineB"), -1, 36.5f, 3, 1, 0.8f);
	Site(TEXT("CutLineB"), 1, 36.5f, 3, 2, 0.8f);
Zone(TEXT("Cut: red rock pass, after the corkscrew 1"), 5.633f, 5.723f);
	Site(TEXT("CutLine"), -1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLine"), 1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLineB"), -1, 36.5f, 3, 1, 0.8f);
	Site(TEXT("CutLineB"), 1, 36.5f, 3, 2, 0.8f);
Zone(TEXT("Cut: red rock pass east 1"), 5.995f, 6.311f);
	Site(TEXT("CutLine"), -1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLine"), 1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLineB"), -1, 36.5f, 3, 1, 0.8f);
	Site(TEXT("CutLineB"), 1, 36.5f, 3, 2, 0.8f);
Zone(TEXT("Cut: red rock return, first straight 1"), 18.572f, 18.708f);
	Site(TEXT("CutLine"), -1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLine"), 1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLineB"), -1, 36.5f, 3, 1, 0.8f);
	Site(TEXT("CutLineB"), 1, 36.5f, 3, 2, 0.8f);
Zone(TEXT("Cut: red rock return, first straight 2"), 18.753f, 19.342f);
	Site(TEXT("CutLine"), -1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLine"), 1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLineB"), -1, 36.5f, 3, 1, 0.8f);
	Site(TEXT("CutLineB"), 1, 36.5f, 3, 2, 0.8f);
Zone(TEXT("Cut: red rock return, second straight 1"), 19.930f, 19.975f);
	Site(TEXT("CutLine"), -1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLine"), 1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLineB"), -1, 36.5f, 3, 1, 0.8f);
	Site(TEXT("CutLineB"), 1, 36.5f, 3, 2, 0.8f);
Zone(TEXT("Cut: red rock return, second straight 2"), 20.020f, 20.608f);
	Site(TEXT("CutLine"), -1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLine"), 1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLineB"), -1, 36.5f, 3, 1, 0.8f);
	Site(TEXT("CutLineB"), 1, 36.5f, 3, 2, 0.8f);
Zone(TEXT("Cut: red rock return, second straight 3"), 20.654f, 20.699f);
	Site(TEXT("CutLine"), -1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLine"), 1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLineB"), -1, 36.5f, 3, 1, 0.8f);
	Site(TEXT("CutLineB"), 1, 36.5f, 3, 2, 0.8f);
Zone(TEXT("Cut: red rock return, third straight 1"), 21.332f, 21.875f);
	Site(TEXT("CutLine"), -1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLine"), 1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLineB"), -1, 36.5f, 3, 1, 0.8f);
	Site(TEXT("CutLineB"), 1, 36.5f, 3, 2, 0.8f);
Zone(TEXT("Cut: red rock return, third straight 2"), 21.920f, 22.011f);
	Site(TEXT("CutLine"), -1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLine"), 1, 30.f, 1, 0, 0.96f);
	Site(TEXT("CutLineB"), -1, 36.5f, 3, 1, 0.8f);
	Site(TEXT("CutLineB"), 1, 36.5f, 3, 2, 0.8f);

// ---- 3. the two wall bends of Red Rock Pass, and the monuments. The outside of each bend gets the 76 m cliff, a slot
// long and 1.27 times the size (the outside of a 129 m bend is a longer way round: at its own size the pieces left 11 m
// between them), 25 m out; the inside gets the cut. A monument stands dead ahead of each straight that ends in a bend:
// the flats' straight points 88 m left of slot 117, the corkscrew's 92 m right of slot 130; in the finale each straight
// points 108 m left of the road beyond its S-bend (slots 440 and 471)
Zone(TEXT("Cliff: wall bend right"), 5.135f, 5.452f);
	Site(TEXT("CliffLine"), -1, 35.5f, 1, 0, 1.f, 1.27f);
	Site(TEXT("CutLine"), 1, 30.f, 1, 0, 0.9f);
Zone(TEXT("Cliff: wall bend left"), 5.678f, 6.040f);
	Site(TEXT("CliffLine"), 1, 35.5f, 1, 0, 1.f, 1.27f);
	Site(TEXT("CutLine"), -1, 30.f, 1, 0, 0.9f);
Zone(TEXT("Monument: the gate of the pass"), 5.271f, 5.316f);
	Site(TEXT("Monument"), -1, 88.f, 1, 0, 1.f);
Zone(TEXT("Monument: the second gate"), 5.859f, 5.904f);
	Site(TEXT("Monument"), 1, 92.f, 1, 0, 1.f);
Zone(TEXT("Monument: red rock return"), 19.884f, 19.930f);
	Site(TEXT("Monument"), -1, 108.f, 1, 0, 1.f);
Zone(TEXT("Monument: red rock return, second"), 21.287f, 21.332f);
	Site(TEXT("Monument"), -1, 108.f, 1, 0, 1.f, 0.9f);

// ---- 4. the Syndicate's ventilation heads: a fan as tall as a truck turning slowly in a bunker's face, unlit, at the
// sight line. Thick through their range, a few by Last Chance Gas (the hide is in the rocks behind it)
Zone(TEXT("Vents: syndicate range"), 14.5f, 18.5f);
	Scatter(TEXT("SM_VentShaft"), 27.f, 29.f, 3, 0, 0.85f, 1.f, 0.f, -1);
	Scatter(TEXT("SM_VentShaft"), 27.f, 29.f, 3, 1, 0.85f, 1.f, 0.f, 1);
Zone(TEXT("Vents: last chance gas"), 6.31f, 7.75f);
	Scatter(TEXT("SM_VentShaft"), 27.f, 29.f, 7, 3, 1.f);

// ---- 5. signs that flicker, and stops up at the road. A sign stands in front of every motel court of Last Chance Gas
// (the courts are 60 m out and one storey more than they were: the sign is what carries from the road), turned to the
// driver (the sets NeonL and NeonR); the stops take the free slot between the first pass's road stops and the courts,
// the two sides in turn (their pole sign and the courts' signs stand across the road in the mesh itself, two-faced)
Zone(TEXT("Signs: state line"), 0.3f, 1.17f);
	Site(TEXT("NeonR"), 1, 35.f, 9, 4, 1.f);
Zone(TEXT("Signs: snake eyes country"), 2.43f, 3.83f);
	Site(TEXT("NeonL"), -1, 35.f, 4, 1, 1.f);
	Site(TEXT("NeonR"), 1, 35.f, 4, 3, 1.f);
Zone(TEXT("Signs: last chance gas"), 6.31f, 7.75f);
	Site(TEXT("NeonL"), -1, 35.f, 4, 2, 1.f);
	Site(TEXT("NeonR"), 1, 35.f, 4, 0, 1.f);
Zone(TEXT("Stops: state line"), 0.3f, 1.17f);
	Site(TEXT("TruckStopSet"), 0, 40.f, 8, 3, 1.f);
Zone(TEXT("Stops: snake eyes country"), 2.43f, 3.83f);
	Site(TEXT("TruckStopSet"), 0, 40.f, 8, 3, 1.f);
Zone(TEXT("Stops: last chance gas to the city lights"), 6.31f, 10.69f);
	Site(TEXT("TruckStopSet"), 0, 40.f, 8, 3, 1.f);
Zone(TEXT("Stops: vegas approach"), 22.f, 25.6f);
	Site(TEXT("TruckStopSet"), 0, 40.f, 8, 3, 1.f);

// ---- 6. two more houses for the casino approach, as flat fronts in the middle distance, each turned 35 degrees to
// the driver (a front that stood square to the road 180 m out was seen at 25 to 40 degrees: half its width)
Zone(TEXT("Fronts: jackpot junction"), 7.75f, 9.05f);
	Site(TEXT("FrontAL"), -1, 176.f, 6, 5, 0.9f);
	Site(TEXT("FrontAR"), 1, 190.f, 6, 2, 0.9f);
	Site(TEXT("FrontBL"), -1, 192.f, 6, 2, 0.9f);
	Site(TEXT("FrontBR"), 1, 174.f, 6, 5, 0.9f);
Zone(TEXT("Fronts: city lights"), 9.05f, 10.69f);
	Site(TEXT("FrontAL"), -1, 170.f, 4, 0, 0.9f);
	Site(TEXT("FrontAR"), 1, 188.f, 4, 2, 0.9f);
	Site(TEXT("FrontBL"), -1, 190.f, 4, 2, 0.9f);
	Site(TEXT("FrontBR"), 1, 172.f, 4, 0, 0.9f);
Zone(TEXT("Fronts: vegas approach"), 22.f, 25.6f);
	Site(TEXT("FrontAL"), -1, 168.f, 4, 1, 0.9f, 1.15f);
	Site(TEXT("FrontAR"), 1, 186.f, 4, 3, 0.9f, 1.15f);
	Site(TEXT("FrontBL"), -1, 188.f, 4, 3, 0.9f, 1.15f);
	Site(TEXT("FrontBR"), 1, 170.f, 4, 1, 0.9f, 1.15f);

// ---- 7. the wheel of fortune: dead ahead of the jump at 7.2 km (its line runs 100 to 118 m left of the S-bend and the
// loop beyond it: slot 179), and of the last two straights of the finale (108 m left of slots 531 and 562). 72 to
// 84 m across and up to 134 m to its top: it is seen from the jump, 900 m off, grows for ten seconds, and is passed
// 100 m to the left. Turned to the driver (the set)
Zone(TEXT("Wheel: jackpot junction"), 8.076f, 8.121f);
	Site(TEXT("Wheel"), -1, 106.f, 1, 0, 1.f, 1.8f);
	Site(TEXT("WheelLot"), -1, 62.f, 1, 0, 1.f);
Zone(TEXT("Wheel: vegas approach"), 24.002f, 24.047f);
	Site(TEXT("Wheel"), -1, 110.f, 1, 0, 1.f, 1.9f);
	Site(TEXT("WheelLot"), -1, 64.f, 1, 0, 1.f);
Zone(TEXT("Wheel: the last"), 25.404f, 25.449f);
	Site(TEXT("Wheel"), -1, 112.f, 1, 0, 1.f, 2.1f);
	Site(TEXT("WheelLot"), -1, 64.f, 1, 0, 1.f);

// ---- 8. the long vistas: dust and a train. Dust blows off patches of rippled sand 44 m out and beyond (the wind is the
// level's own, along the highway and a little to its right: no plume comes nearer than 29 m). Out of the loop at
// 10.15 km the road is dead straight to 11.36 km, and from 10.69 km it is on the open salt flats: a railway runs
// beside it there, 80 m to the LEFT, one 905 m length laid on slot 246 (10.68 to 11.58 km: its last 200 m go straight
// on where the highway swings right and leaves the line) with two trains on it. A monument stands dead ahead of that
// straight (134 m left of slot 263), where the swing begins
Zone(TEXT("Dust: state line"), 0.1f, 1.17f);
	Scatter(TEXT("SM_DustBlow"), 44.f, 70.f, 4, 1, 0.8f, 1.f, 0.15f);
Zone(TEXT("Dust: joshua flats"), 3.83f, 5.14f);
	Scatter(TEXT("SM_DustBlow"), 44.f, 70.f, 4, 3, 0.8f, 1.f, 0.15f);
Zone(TEXT("Dust: salt flats"), 10.69f, 14.5f);
	Scatter(TEXT("SM_DustBlow"), 44.f, 70.f, 3, 0, 0.8f, 1.f, 0.15f);
Zone(TEXT("Dust: syndicate range"), 14.5f, 18.5f);
	Scatter(TEXT("SM_DustBlow"), 46.f, 70.f, 5, 2, 0.8f, 1.f, 0.15f);
Zone(TEXT("Dust: vegas approach"), 22.f, 25.6f);
	Scatter(TEXT("SM_DustBlow"), 50.f, 70.f, 8, 6, 0.8f, 1.f, 0.15f);
Zone(TEXT("Railway"), 11.107f, 11.152f);
	Site(TEXT("RailLine"), -1, 80.f, 1, 0, 1.f);
Zone(TEXT("Monument: salt flats"), 11.877f, 11.92f);
	Site(TEXT("Monument"), -1, 134.f, 1, 0, 1.f);

// ---- 9. the far tableland: a flat card 900 m long in the haze's colour, 1.9 km out, behind the mesas and the far range
Zone(TEXT("Far tableland"), 0.f, 99.f);
	Far(TEXT("SM_MesaCard"), 1880.f, 1980.f, 18, 0, 1.f, 1.f, 0.08f, 0, 40.f, 4.f);
};
ThirdPass();
