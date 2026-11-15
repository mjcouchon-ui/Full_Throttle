// 3.2 dressing table, level 1: Chemical Works. The words (Set, Zone, Scatter, Site, Far, Lit) are explained at the top of
// FTTrackDress.cpp; the meshes are made by Content/Python/ft_dress_chem.py. Distances across are metres from the road's
// centre line; districts are in kilometres along the road.
//
// The works, stage by stage, following the level's own scenes: the gate, the tank farm (THE TUBES), the process units
// (SPIN CYCLE), the reactor block (THE REACTOR), pipe alley and the cooling field (PIPE DREAMS), the loading yards
// (MEGA VAT), the waste run (THE TOXIC RUN), and the Alchemist's export terminal for the finale. Each district has
// its own near layer, its own sets in the middle distance and its own horizon. Within 24 m of the centre line there
// are only low, unlit things, none higher than the road's own barrier: concrete barriers and ground pipe. The
// perimeter fence (27.5 m), the drum piles, the signs and the valve stations stand from 24 m out. The fence, the
// ground pipe and the rack corridor are Sites, not Scatters: a Site stands exactly on its slot, so slot-long pieces join.

// ---- sets: pieces composed round a landmark (along m, out m, up m, yaw, scale)
Set(TEXT("GateHouse"), {
	{ TEXT("SM_PumpHouse"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_HazSign"), 9, -6, 0, 0, 1.f },
	{ TEXT("SM_JerseyRow"), -5, -15, 0, 0, 1.f },
	{ TEXT("SM_TankerTrailer"), -15, 9, 0, 12, 1.f },
	{ TEXT("SM_DrumPile"), 11, 7, 0, 30, 1.f },
	{ TEXT("SM_Substation"), 30, 16, 0, 0, 1.f },
});
Set(TEXT("TankFarm"), {
	{ TEXT("SM_TankCluster"), 0, 0, 0, 0, 1.f },
	{ TEXT("/Game/Maps/ChemicalWorks/Props/SM_Tank2"), 27, 6, 0, 90, 1.f },
	{ TEXT("/Game/Maps/ChemicalWorks/Props/SM_Tank"), -31, 15, 0, 90, 1.15f },
	{ TEXT("SM_ValveStation"), 4, -17, 0, 0, 1.f },
	{ TEXT("SM_PipeRun"), -8, -22, 0, 0, 1.f },
	{ TEXT("SM_Platform"), 25, -15, 0, 0, 1.f },
	{ TEXT("SM_DrumPile2"), -21, -15, 0, 0, 1.f },
	{ TEXT("SM_TankCluster"), 4, 34, 0, 180, 0.9f },
});
Set(TEXT("ProcessUnit"), {
	{ TEXT("SM_PipeRack"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_PipeRack"), 33, 0, 0, 0, 1.f },
	{ TEXT("/Game/Maps/ChemicalWorks/Props/SM_Tower"), 6, 17, 0, 90, 1.f },
	{ TEXT("/Game/Maps/ChemicalWorks/Props/SM_Tower"), 31, 23, 0, 90, 0.8f },
	{ TEXT("SM_Platform"), -15, 12, 0, 0, 1.f },
	{ TEXT("SM_ValveStation"), -8, -10, 0, 0, 1.f },
	{ TEXT("SM_PumpHouse"), 46, -9, 0, 0, 1.f },
	{ TEXT("SM_CoolingCells"), 18, 40, 0, 0, 1.f },
});
Set(TEXT("ReactorBlock"), {
	{ TEXT("SM_ReactorHall"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_Substation"), -32, -3, 0, 0, 1.f },
	{ TEXT("SM_PipeRack"), 6, -24, 0, 0, 1.f },
	{ TEXT("SM_CoolingCells"), 36, 5, 0, 0, 1.f },
	{ TEXT("SM_HazSign"), -13, -32, 0, 0, 1.2f },
	{ TEXT("SM_DrumPile"), 24, -18, 0, 0, 1.f },
	{ TEXT("SM_FenceRun"), -6, -36, 0, 0, 1.f },
	{ TEXT("SM_FenceRun"), 10, -36, 0, 0, 1.f },
});
Set(TEXT("LoadingYard"), {
	{ TEXT("SM_LoadingBay"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_GantryCrane"), 35, 9, 0, 0, 1.f },
	{ TEXT("SM_RailSpur"), 8, 27, 0, 0, 1.f },
	{ TEXT("SM_TankerTrailer"), -25, -6, 0, -15, 1.f },
	{ TEXT("SM_DrumPile"), -17, 10, 0, 0, 1.f },
	{ TEXT("SM_DrumPile2"), 21, -13, 0, 40, 1.f },
	{ TEXT("SM_Warehouse"), -42, 26, 0, 0, 1.f },
});
Set(TEXT("Gasworks"), {
	{ TEXT("SM_Gasholder"), 0, 0, 0, 0, 1.f },
	{ TEXT("/Game/Maps/ChemicalWorks/Props/SM_Sphere"), 31, -6, 0, 90, 1.f },
	{ TEXT("SM_ValveStation"), 10, -21, 0, 0, 1.f },
	{ TEXT("SM_PipeRun"), 0, -27, 0, 0, 1.f },
	{ TEXT("SM_PumpHouse"), -23, -10, 0, 0, 1.f },
	{ TEXT("SM_Gasholder"), -34, 22, 0, 0, 0.8f },
});
Set(TEXT("WasteYard"), {
	{ TEXT("SM_DrumPile"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_DrumPile2"), 7, 5, 0, 60, 1.f },
	{ TEXT("SM_DrumPile"), -8, 6, 0, 120, 1.1f },
	{ TEXT("/Game/Maps/ChemicalWorks/Props/SM_Vat"), 2, 28, 0, 90, 1.f },
	{ TEXT("SM_JerseyRow"), 0, -9, 0, 0, 1.f },
	{ TEXT("SM_TankerTrailer"), -19, 15, 0, 35, 1.f },
	{ TEXT("SM_HazSign"), 13, -10, 0, 0, 1.f },
	{ TEXT("SM_DrumPile2"), 16, 14, 0, 200, 1.2f },
});
Set(TEXT("CoolingField"), {
	{ TEXT("SM_CoolingCells"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_CoolingTower"), -8, 46, 0, 0, 1.f },
	{ TEXT("SM_CoolingCells"), 2, 15, 0, 0, 1.f },
	{ TEXT("SM_PumpHouse"), -22, -4, 0, 0, 1.f },
	{ TEXT("SM_PipeRun"), 0, -15, 0, 0, 1.f },
	{ TEXT("SM_Platform"), 24, 2, 0, 0, 1.f },
	{ TEXT("SM_PipeRack"), 0, -26, 0, 0, 1.f },
});
Set(TEXT("RailYard"), {
	{ TEXT("SM_RailSpur"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_RailSpur"), 5, 7, 0, 0, 1.f },
	{ TEXT("SM_GantryCrane"), -8, -16, 0, 0, 1.f },
	{ TEXT("SM_Warehouse"), 12, 32, 0, 0, 1.f },
	{ TEXT("SM_DrumPile"), 26, -14, 0, 0, 1.f },
	{ TEXT("SM_JerseyRow"), -30, -18, 0, 0, 1.f },
});

Set(TEXT("FenceLine"), {
	{ TEXT("SM_FenceRun"), -15, 0, 0, 0, 1.f },
	{ TEXT("SM_FenceRun"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_FenceRun"), 15, 0, 0, 0, 1.f },
});
Set(TEXT("PipeLine"), {
	{ TEXT("SM_PipeRun"), -11.25f, 0, 0, 0, 1.f },
	{ TEXT("SM_PipeRun"), 11.25f, 0, 0, 0, 1.f },
});
Set(TEXT("RackLine"), {
	{ TEXT("SM_PipeRack"), 0, 0, 0, 0, 1.35f },
});

// ---- the gate: fences, the lodge, tankers waiting, the works lit up ahead
Zone(TEXT("Factory gate"), 0.0f, 1.23f);
	Site(TEXT("GateHouse"), 0, 36.f, 4, 1, 1.f);
	Site(TEXT("FenceLine"), -1, 27.5f, 1, 0, 0.85f);
	Site(TEXT("FenceLine"), 1, 27.5f, 1, 0, 0.85f);
	Scatter(TEXT("SM_HazSign"), 26.f, 30.f, 3, 2, 0.8f);
	Scatter(TEXT("SM_JerseyRow"), 17.4f, 17.8f, 2, 1, 0.6f);
	Scatter(TEXT("SM_TankerTrailer"), 30.f, 44.f, 5, 3, 0.8f, 1.f, 0.f, 0, 12.f);
	Scatter(TEXT("SM_DrumPile"), 29.5f, 40.f, 2, 0, 0.5f, 1.f, 0.15f, 0, 180.f);
	Scatter(TEXT("SM_Warehouse"), 150.f, 230.f, 4, 2, 0.8f, 1.f, 0.1f);
	Scatter(TEXT("SM_PumpHouse"), 44.f, 70.f, 3, 2, 0.7f);
	Scatter(TEXT("SM_Substation"), 60.f, 100.f, 4, 1, 0.8f);
	Scatter(TEXT("SM_PipeRack"), 110.f, 190.f, 2, 0, 0.7f, 1.f, 0.f, 0, 8.f);
	Scatter(TEXT("SM_TankCluster"), 220.f, 285.f, 3, 1, 0.8f, 1.2f, 0.15f);
	Far(TEXT("SM_FarPlant"), 380.f, 620.f, 4, 0, 0.9f, 1.f, 0.15f);
	Far(TEXT("SM_FarStacks"), 720.f, 1100.f, 8, 3, 0.9f, 1.f, 0.15f);

// ---- the tank farm (THE TUBES): bunded tanks in rows, gas holders behind, spheres on the skyline
Zone(TEXT("Tank farm"), 1.23f, 2.6f);
	Site(TEXT("TankFarm"), 0, 88.f, 3, 0, 0.95f);
	Site(TEXT("Gasworks"), 0, 160.f, 5, 2, 0.9f);
	Site(TEXT("PipeLine"), -1, 19.1f, 1, 0, 0.7f);
	Site(TEXT("PipeLine"), 1, 19.1f, 1, 0, 0.7f);
	Scatter(TEXT("SM_ValveStation"), 25.f, 32.f, 3, 1, 0.7f);
	Scatter(TEXT("SM_DrumPile2"), 29.5f, 40.f, 4, 2, 0.6f, 1.f, 0.15f, 0, 180.f);
	Scatter(TEXT("SM_HazSign"), 26.f, 30.f, 6, 4, 0.8f);
	Scatter(TEXT("SM_TankCluster"), 200.f, 270.f, 3, 1, 0.8f, 1.1f, 0.15f);
	Scatter(TEXT("/Game/Maps/ChemicalWorks/Props/SM_Tank"), 40.f, 75.f, 2, 1, 0.6f, 0.9f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_Platform"), 46.f, 76.f, 3, 0, 0.7f);
	Scatter(TEXT("/Game/Maps/ChemicalWorks/Props/SM_Tank2"), 130.f, 285.f, 1, 0, 0.8f, 1.3f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_Gasholder"), 215.f, 285.f, 3, 2, 0.8f, 1.1f, 0.15f);
	Far(TEXT("SM_FarGasworks"), 370.f, 640.f, 4, 1, 0.9f, 1.f, 0.15f);
	Far(TEXT("SM_FarPlant2"), 720.f, 1050.f, 6, 2, 0.9f, 1.1f, 0.1f);

// ---- the process units (SPIN CYCLE): columns behind long pipe racks, platforms, stacks on the horizon
Zone(TEXT("Process units"), 2.6f, 3.98f);
	Site(TEXT("ProcessUnit"), 0, 62.f, 3, 1, 0.95f);
	Site(TEXT("PipeLine"), -1, 19.1f, 1, 0, 0.6f);
	Site(TEXT("PipeLine"), 1, 19.1f, 1, 0, 0.6f);
	Scatter(TEXT("SM_Platform"), 40.f, 55.f, 4, 0, 0.7f);
	Scatter(TEXT("SM_ValveStation"), 25.f, 31.f, 4, 2, 0.7f);
	Scatter(TEXT("SM_PumpHouse"), 28.f, 40.f, 5, 3, 0.8f);
	Lit();
	Scatter(TEXT("SM_CoolingCells"), 170.f, 250.f, 4, 1, 0.8f);
	Scatter(TEXT("SM_PipeRack"), 105.f, 175.f, 1, 0, 0.65f, 1.f, 0.f, 0, 6.f);
	Scatter(TEXT("/Game/Maps/ChemicalWorks/Props/SM_Tower"), 120.f, 270.f, 1, 0, 0.75f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_TankCluster"), 200.f, 285.f, 3, 2, 0.8f);
	Far(TEXT("SM_FarStacks"), 380.f, 640.f, 4, 0, 0.9f, 1.f, 0.15f);
	Far(TEXT("SM_FarPlant"), 680.f, 980.f, 5, 2, 0.9f, 1.1f, 0.1f);

// ---- the reactor block (THE REACTOR): the dark halls with the violet light, substations, cooling towers close
Zone(TEXT("Reactor block"), 3.98f, 5.35f);
	Site(TEXT("ReactorBlock"), 0, 98.f, 4, 0, 1.f);
	Site(TEXT("CoolingField"), 0, 84.f, 4, 2, 0.9f);
	Site(TEXT("FenceLine"), -1, 27.5f, 1, 0, 0.8f);
	Site(TEXT("FenceLine"), 1, 27.5f, 1, 0, 0.8f);
	Scatter(TEXT("SM_HazSign"), 26.f, 29.f, 2, 1, 0.8f);
	Scatter(TEXT("SM_JerseyRow"), 17.4f, 17.8f, 3, 0, 0.7f);
	Scatter(TEXT("SM_Substation"), 40.f, 60.f, 6, 4, 0.9f);
	Scatter(TEXT("SM_ReactorHall"), 210.f, 270.f, 5, 3, 0.8f, 1.2f, 0.1f);
	Scatter(TEXT("SM_Platform"), 44.f, 70.f, 3, 1, 0.7f);
	Scatter(TEXT("SM_Substation"), 115.f, 190.f, 3, 0, 0.8f, 1.2f, 0.1f);
	Scatter(TEXT("SM_CoolingCells"), 150.f, 230.f, 2, 1, 0.8f);
	Scatter(TEXT("SM_Gasholder"), 230.f, 285.f, 4, 1, 0.8f);
	Scatter(TEXT("SM_CoolingTower"), 225.f, 280.f, 3, 1, 0.8f, 1.f, 0.15f);
	Far(TEXT("SM_FarCooling"), 400.f, 700.f, 4, 0, 0.95f, 1.f, 0.15f);
	Far(TEXT("SM_FarStacks"), 820.f, 1200.f, 7, 3, 0.9f, 1.1f, 0.1f);

// ---- pipe alley (PIPE DREAMS): racks both sides like a corridor, the cooling field beyond
Zone(TEXT("Pipe alley"), 5.35f, 6.72f);
	Site(TEXT("RackLine"), -1, 48.f, 1, 0, 0.9f);
	Site(TEXT("RackLine"), 1, 50.f, 1, 0, 0.9f);
	Site(TEXT("PipeLine"), -1, 19.1f, 1, 0, 0.8f);
	Site(TEXT("PipeLine"), 1, 19.1f, 1, 0, 0.8f);
	Scatter(TEXT("SM_ValveStation"), 25.f, 31.f, 2, 1, 0.7f);
	Scatter(TEXT("SM_Platform"), 72.f, 100.f, 3, 0, 0.8f);
	Scatter(TEXT("SM_CoolingCells"), 100.f, 140.f, 4, 1, 0.8f);
	Site(TEXT("CoolingField"), 0, 150.f, 5, 3, 0.9f);
	Scatter(TEXT("SM_PumpHouse"), 30.f, 40.f, 3, 2, 0.7f);
	Scatter(TEXT("SM_PipeRack"), 84.f, 108.f, 1, 0, 0.6f);
	Scatter(TEXT("SM_PipeRack"), 165.f, 245.f, 1, 0, 0.7f, 1.2f, 0.1f, 0, 8.f);
	Scatter(TEXT("SM_TankCluster"), 205.f, 285.f, 3, 0, 0.8f);
	Scatter(TEXT("SM_CoolingTower"), 230.f, 285.f, 4, 2, 0.8f, 1.1f, 0.15f);
	Far(TEXT("SM_FarCooling"), 380.f, 600.f, 3, 1, 0.95f, 1.1f, 0.12f);
	Far(TEXT("SM_FarPlant2"), 760.f, 1100.f, 6, 2, 0.9f, 1.1f, 0.1f);

// ---- the loading yards (MEGA VAT): road and rail loading, cranes over stacks, sheds
Zone(TEXT("Loading yards"), 6.72f, 8.09f);
	Site(TEXT("LoadingYard"), 0, 80.f, 4, 0, 1.f);
	Site(TEXT("RailYard"), 0, 72.f, 4, 2, 0.95f);
	Scatter(TEXT("SM_JerseyRow"), 17.4f, 17.8f, 1, 0, 0.7f);
	Scatter(TEXT("SM_DrumPile"), 29.5f, 40.f, 1, 0, 0.6f, 1.f, 0.15f, 0, 180.f);
	Scatter(TEXT("SM_TankerTrailer"), 28.f, 42.f, 3, 1, 0.7f, 1.f, 0.f, 0, 14.f);
	Scatter(TEXT("SM_HazSign"), 26.f, 29.f, 4, 2, 0.8f);
	Scatter(TEXT("SM_Warehouse"), 160.f, 230.f, 3, 1, 0.8f, 1.1f, 0.1f);
	Scatter(TEXT("SM_TankerTrailer"), 44.f, 70.f, 1, 0, 0.6f, 1.f, 0.f, 0, 25.f);
	Scatter(TEXT("SM_RailSpur"), 112.f, 150.f, 2, 0, 0.7f);
	Scatter(TEXT("SM_GantryCrane"), 135.f, 200.f, 3, 2, 0.8f);
	Scatter(TEXT("SM_LoadingBay"), 215.f, 280.f, 3, 0, 0.8f, 1.2f, 0.1f);
	Far(TEXT("SM_FarPlant"), 400.f, 650.f, 4, 0, 0.9f, 1.f, 0.15f);
	Far(TEXT("SM_FarGasworks"), 820.f, 1150.f, 7, 3, 0.9f, 1.1f, 0.1f);

// ---- the waste run (THE TOXIC RUN): drum fields, a stack forest
Zone(TEXT("Waste run"), 8.09f, 9.46f);
	Site(TEXT("WasteYard"), 0, 46.f, 3, 0, 1.f);
	Site(TEXT("Gasworks"), 0, 150.f, 6, 3, 0.9f);
	Scatter(TEXT("SM_DrumPile"), 29.5f, 42.f, 1, 0, 0.7f, 1.f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_DrumPile2"), 29.5f, 46.f, 1, 0, 0.7f, 1.1f, 0.2f, 0, 180.f);
	Site(TEXT("FenceLine"), -1, 27.5f, 1, 0, 0.7f);
	Site(TEXT("FenceLine"), 1, 27.5f, 1, 0, 0.7f);
	Scatter(TEXT("SM_JerseyRow"), 17.4f, 17.8f, 2, 1, 0.5f);
	Scatter(TEXT("SM_HazSign"), 26.f, 29.f, 2, 0, 0.9f);
	Scatter(TEXT("SM_DrumPile"), 46.f, 90.f, 1, 0, 0.7f, 1.4f, 0.2f, 0, 180.f);
	Scatter(TEXT("/Game/Maps/ChemicalWorks/Props/SM_Vat"), 60.f, 125.f, 2, 0, 0.7f, 1.f, 0.2f, 0, 180.f);
	Scatter(TEXT("SM_TankCluster"), 110.f, 165.f, 3, 2, 0.8f);
	Scatter(TEXT("SM_Warehouse"), 175.f, 255.f, 3, 1, 0.8f, 1.1f, 0.1f);
	Far(TEXT("SM_FarStacks"), 360.f, 600.f, 3, 0, 0.95f, 1.1f, 0.15f);
	Far(TEXT("SM_FarCooling"), 820.f, 1200.f, 7, 3, 0.9f, 1.1f, 0.1f);

// ---- the finale: the Alchemist's export terminal, then the same works seen again in longer stretches
Zone(TEXT("Export terminal"), 9.46f, 13.4f);
	Site(TEXT("LoadingYard"), 0, 82.f, 5, 0, 1.f);
	Site(TEXT("RailYard"), 0, 74.f, 5, 2, 0.95f);
	Scatter(TEXT("SM_TankerTrailer"), 28.f, 44.f, 2, 1, 0.7f, 1.f, 0.f, 0, 14.f);
	Scatter(TEXT("SM_JerseyRow"), 17.4f, 17.8f, 1, 0, 0.6f);
	Site(TEXT("PipeLine"), -1, 19.1f, 2, 0, 0.7f);
	Site(TEXT("PipeLine"), 1, 19.1f, 2, 0, 0.7f);
	Scatter(TEXT("SM_DrumPile"), 29.5f, 40.f, 2, 1, 0.5f, 1.f, 0.15f, 0, 180.f);
	Scatter(TEXT("SM_Warehouse"), 170.f, 250.f, 3, 0, 0.8f, 1.1f, 0.1f);
	Scatter(TEXT("SM_Substation"), 50.f, 72.f, 5, 3, 0.8f);
	Scatter(TEXT("SM_RailSpur"), 112.f, 150.f, 2, 1, 0.7f);
	Scatter(TEXT("SM_GantryCrane"), 135.f, 205.f, 3, 1, 0.8f);
	Scatter(TEXT("SM_LoadingBay"), 215.f, 280.f, 4, 2, 0.8f, 1.2f, 0.1f);
	Far(TEXT("SM_FarPlant"), 400.f, 660.f, 4, 0, 0.9f, 1.f, 0.15f);
	Far(TEXT("SM_FarGasworks"), 800.f, 1150.f, 7, 3, 0.9f, 1.1f, 0.1f);
Zone(TEXT("Cooling field"), 13.4f, 17.4f);
	Site(TEXT("CoolingField"), 0, 86.f, 3, 0, 0.95f);
	Scatter(TEXT("SM_PipeRack"), 46.f, 54.f, 2, 0, 0.75f);
	Scatter(TEXT("SM_CoolingCells"), 120.f, 180.f, 2, 1, 0.8f);
	Site(TEXT("PipeLine"), -1, 19.1f, 1, 0, 0.7f);
	Site(TEXT("PipeLine"), 1, 19.1f, 1, 0, 0.7f);
	Scatter(TEXT("SM_ValveStation"), 25.f, 31.f, 3, 1, 0.7f);
	Scatter(TEXT("SM_PumpHouse"), 30.f, 42.f, 4, 1, 0.7f);
	Scatter(TEXT("SM_Platform"), 60.f, 110.f, 3, 2, 0.8f);
	Scatter(TEXT("SM_Gasholder"), 150.f, 200.f, 5, 3, 0.8f);
	Scatter(TEXT("SM_CoolingCells"), 200.f, 285.f, 2, 0, 0.8f, 1.2f, 0.1f);
	Scatter(TEXT("SM_CoolingTower"), 120.f, 170.f, 4, 3, 0.8f, 0.8f, 0.1f);
	Scatter(TEXT("SM_CoolingTower"), 215.f, 285.f, 2, 1, 0.85f, 1.15f, 0.15f);
	Far(TEXT("SM_FarCooling"), 380.f, 700.f, 3, 0, 0.95f, 1.1f, 0.15f);
	Far(TEXT("SM_FarStacks"), 900.f, 1300.f, 6, 2, 0.9f, 1.1f, 0.1f);
Zone(TEXT("Tank farm B"), 17.4f, 21.4f);
	Site(TEXT("TankFarm"), 0, 90.f, 3, 0, 0.95f);
	Site(TEXT("Gasworks"), 0, 165.f, 4, 2, 0.9f);
	Site(TEXT("PipeLine"), -1, 19.1f, 1, 0, 0.7f);
	Site(TEXT("PipeLine"), 1, 19.1f, 1, 0, 0.7f);
	Scatter(TEXT("SM_ValveStation"), 25.f, 32.f, 3, 1, 0.7f);
	Site(TEXT("FenceLine"), -1, 27.5f, 2, 1, 0.7f);
	Site(TEXT("FenceLine"), 1, 27.5f, 2, 1, 0.7f);
	Scatter(TEXT("SM_DrumPile2"), 29.5f, 40.f, 4, 2, 0.6f, 1.f, 0.15f, 0, 180.f);
	Scatter(TEXT("SM_Platform"), 46.f, 76.f, 4, 0, 0.7f);
	Scatter(TEXT("/Game/Maps/ChemicalWorks/Props/SM_Sphere"), 110.f, 160.f, 3, 2, 0.8f, 1.f, 0.2f, 0, 180.f);
	Scatter(TEXT("/Game/Maps/ChemicalWorks/Props/SM_Tank"), 130.f, 285.f, 1, 0, 0.8f, 1.3f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_TankCluster"), 200.f, 285.f, 2, 1, 0.8f, 1.1f, 0.15f);
	Far(TEXT("SM_FarGasworks"), 370.f, 640.f, 3, 1, 0.9f, 1.f, 0.15f);
	Far(TEXT("SM_FarPlant2"), 720.f, 1050.f, 5, 2, 0.9f, 1.1f, 0.1f);
Zone(TEXT("Reactor two"), 21.4f, 25.6f);
	Site(TEXT("ReactorBlock"), 0, 98.f, 4, 0, 1.f);
	Site(TEXT("ProcessUnit"), 0, 64.f, 4, 2, 0.95f);
	Site(TEXT("FenceLine"), -1, 27.5f, 1, 0, 0.8f);
	Site(TEXT("FenceLine"), 1, 27.5f, 1, 0, 0.8f);
	Scatter(TEXT("SM_HazSign"), 26.f, 29.f, 3, 1, 0.8f);
	Scatter(TEXT("SM_Platform"), 40.f, 55.f, 5, 3, 0.7f);
	Scatter(TEXT("SM_Substation"), 44.f, 62.f, 5, 2, 0.8f);
	Scatter(TEXT("/Game/Maps/ChemicalWorks/Props/SM_Tower"), 120.f, 200.f, 2, 0, 0.7f, 1.f, 0.3f, 0, 180.f);
	Scatter(TEXT("SM_CoolingCells"), 150.f, 210.f, 3, 1, 0.8f);
	Scatter(TEXT("SM_ReactorHall"), 215.f, 280.f, 4, 3, 0.8f, 1.2f, 0.1f);
	Scatter(TEXT("SM_CoolingTower"), 230.f, 285.f, 4, 0, 0.8f, 1.f, 0.15f);
	Far(TEXT("SM_FarStacks"), 380.f, 640.f, 3, 0, 0.9f, 1.f, 0.15f);
	Far(TEXT("SM_FarCooling"), 820.f, 1200.f, 6, 3, 0.9f, 1.1f, 0.1f);
