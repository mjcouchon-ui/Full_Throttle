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
	Scatter(TEXT("SM_SignTower"), 34.f, 42.f, 4, 1, 0.9f, 0.85f, 0.05f);
	Far(TEXT("SM_CasinoGrand"), 120.f, 150.f, 6, 2, 1.f, 0.85f, 0.05f, 0, 0.9f, 8.f);
Zone(TEXT("Approach: city lights"), 9.05f, 10.69f);
	Scatter(TEXT("SM_SignTower"), 34.f, 44.f, 2, 1, 0.9f, 1.f, 0.08f);
	Far(TEXT("SM_CasinoGrand"), 112.f, 150.f, 6, 1, 0.95f, 1.f, 0.06f, 0, 0.9f, 8.f);
	Far(TEXT("SM_CasinoGrand2"), 112.f, 150.f, 6, 4, 0.95f, 1.f, 0.06f, 0, 0.9f, 8.f);
	Scatter(TEXT("SM_MotelCourt"), 60.f, 66.f, 10, 6, 1.f, 1.f, 0.f, -1);
Zone(TEXT("Approach: vegas"), 22.f, 25.6f);
	Scatter(TEXT("SM_SignTower"), 34.f, 44.f, 2, 0, 0.95f, 1.15f, 0.1f);
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
