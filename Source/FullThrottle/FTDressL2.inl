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
	{ TEXT("SM_WireFence"), -15, 0, 0, 0, 1.f },
	{ TEXT("SM_WireFence"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_WireFence"), 15, 0, 0, 0, 1.f },
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
	Scatter(TEXT("SM_RockSpires"), 40.f, 110.f, 2, 0, 0.8f, 1.f, 0.3f, 0, 180.f);
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
	Scatter(TEXT("SM_RockSpires2"), 44.f, 90.f, 2, 0, 0.8f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RockArch"), 70.f, 130.f, 7, 4, 0.9f, 1.f, 0.15f, 0, 30.f);
	Scatter(TEXT("SM_Sagebrush2"), 20.f, 80.f, 1, 0, 0.6f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_Sagebrush"), 60.f, 200.f, 1, 0, 0.8f, 1.4f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_RockSpires"), 100.f, 180.f, 2, 1, 0.8f, 1.3f, 0.3f, 0, 180.f);
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
