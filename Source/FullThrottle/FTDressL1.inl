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

// The table is written as three parts (first, second and third pass), each the body of a lambda called at once, in
// order. Nothing else differs from one plain list: FTDress::Build holds all eight tables in one function body, and on
// 15 Nov 2026 that body passed the size at which the compiler stops optimising a function (error C4883 in every
// worker's compile). As lambdas the parts are functions of their own. If FTTrackDress.cpp is changed to give each
// level's table its own function, the three wrappers here can simply be taken out.
auto FirstPass = [&]()
{
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
	Scatter(TEXT("SM_JerseyRow"), 17.4f, 17.8f, 2, 1, 0.f);   // third pass: the kerb runs here now
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
	Scatter(TEXT("SM_JerseyRow"), 17.4f, 17.8f, 3, 0, 0.f);   // third pass: the kerb runs here now
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
Zone(TEXT("Loading yards"), 6.72f, 7.94f);   // third pass: to slot 175; the mega vat has 176 to 178
	Site(TEXT("LoadingYard"), 0, 80.f, 4, 0, 1.f);
	Site(TEXT("RailYard"), 0, 72.f, 4, 2, 0.95f);
	Scatter(TEXT("SM_JerseyRow"), 17.4f, 17.8f, 1, 0, 0.f);   // third pass: the kerb runs here now
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
	Scatter(TEXT("SM_JerseyRow"), 17.4f, 17.8f, 2, 1, 0.f);   // third pass: the kerb runs here now
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
	Scatter(TEXT("SM_JerseyRow"), 17.4f, 17.8f, 1, 0, 0.f);   // third pass: the kerb runs here now
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
	Scatter(TEXT("SM_PipeRack"), 46.f, 54.f, 3, 0, 0.75f);
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
};
FirstPass();

auto SecondPass = [&]()
{
// =====================================================================================================================
// SECOND PASS (15 Nov 2026): the ground plane, what stands in the sky of the banked turns, and the reactor.
// Everything from here on is APPENDED on purpose: a rule's random stream is seeded by its place in this file, so a rule
// added among the districts above would re-deal every first-pass piece. These zones lie over the districts.
//
// The lie of this road (measured: SkyScratch\DevLog\jam\art_pass2\w2\track_L1.json): it is hardly ever straight.
// S-bends banked 20 to 34 degrees, three "wall" hairpins banked 58 (2.62-2.99 km climbing 38 m to a plateau, 3.59-3.94
// coming down off it, 7.90-8.11), fourteen glass pipes, eleven loops, two corkscrews, two jumps; and from 14.06 km the
// finale laps a circuit that 22.18-25.58 km runs round a second time, a few metres beside the first.
//
// 1. SKY. A camera rolled with a banked road looks up over the outside of the bend: only tall things near the road
//    stand there. SM_ColumnTrio (88 m) and SM_CoolingTower go on the OUTSIDE of the three hairpins and, more thinly,
//    both sides of the S-bends; SM_Catwalk adds the lit walkways at 46 to 70 m.
// 2. THE REACTOR. The jump at 4.37 km takes off heading straight at a point the road then swerves round (the S-bend at
//    5.02-5.37 km): slot 115, 134 m to the left. SM_ReactorCore stands exactly there: dead ahead from the hairpin's
//    foot at 3.9 km (1.3 km out), in the middle of the windscreen over the jump, and passed on the outside of a banked
//    right-hander, where the camera looks up at it. The finale's stands the same way outside the hairpin at 21.2 km.
// 3. GROUND. Flat pieces (under 2 m). The track keeps Scatter and Site pieces out of loops, pipes, corkscrews and jump
//    gaps, which is where the camera sees most land. So, apart from the plant roads (Scatter, from 26 m: the track's
//    rule applies to them), the ground is laid by Far rules with a metre's drop (a Far piece stands "drop" below the
//    road's own level, and the land 26 to 100 m out lies 0.9 to 1.4 m below it), and so runs on under loops and
//    beside pipes as the horizon does:
//      26 to 47 m: unlit yards, sidings, channels, pipe fields (the "Yards:" zones at the very end of this file);
//      43 to 65 m and 67 to 97 m: lots, ponds, pads, lagoons (the "Ground:" zones). The ponds glow a little: 40 m out.
//    Every piece still goes through the clearance test. The ground zones stop short of the ramps (land steeper than
//    6%: a level slab would hang or drown there) and of the two jumps (a jump's run-up and landing stay open), and
//    the finale's doubled circuit (14.02-17.4 km, lapped again at 22.18-25.58) gets one unlit row 74 m out only.
//    Within a zone the phases keep the ground's slots clear of the slots the district's own sets stand in.

// ---- sky: tall pieces beside the banked road
Zone(TEXT("Sky: gate"), 0.05f, 1.23f);
	Scatter(TEXT("SM_ColumnTrio"), 85.f, 135.f, 4, 2, 0.8f, 1.f, 0.08f);
Zone(TEXT("Sky: tank farm"), 1.23f, 2.6f);
	Scatter(TEXT("SM_Gasholder"), 80.f, 120.f, 3, 1, 0.5f, 1.1f, 0.1f);
	Scatter(TEXT("SM_ColumnTrio"), 95.f, 140.f, 6, 3, 0.9f, 1.f, 0.08f);
Zone(TEXT("Sky: process units"), 2.6f, 3.98f);
	Far(TEXT("SM_ColumnTrio"), 130.f, 210.f, 3, 0, 0.85f, 1.f, 0.1f, 0, 2.f, 180.f);
	Scatter(TEXT("SM_Catwalk"), 46.f, 66.f, 4, 3, 0.8f);
Zone(TEXT("Wall climb"), 2.6f, 2.99f);
	Scatter(TEXT("SM_ColumnTrio"), 62.f, 100.f, 1, 0, 0.9f, 1.f, 0.08f, -1);
	Scatter(TEXT("SM_CoolingTower"), 130.f, 180.f, 2, 1, 1.f, 0.9f, 0.1f, -1);
Zone(TEXT("Wall descent"), 3.59f, 3.94f);
	Scatter(TEXT("SM_ColumnTrio"), 62.f, 100.f, 1, 0, 0.9f, 1.f, 0.08f, 1);
	Scatter(TEXT("SM_CoolingTower"), 130.f, 180.f, 2, 0, 1.f, 0.9f, 0.1f, 1);
Zone(TEXT("Sky: reactor block"), 3.98f, 5.35f);
	Scatter(TEXT("SM_Catwalk"), 46.f, 62.f, 5, 2, 0.8f);
Zone(TEXT("Reactor core"), 5.19f, 5.22f);
	Far(TEXT("SM_ReactorCore"), 134.f, 134.f, 1, 0, 1.f, 1.25f, 0.f, -1, 1.6f, 0.f);
Zone(TEXT("Sky: pipe alley"), 5.35f, 6.72f);
	Scatter(TEXT("SM_Catwalk"), 60.f, 70.f, 3, 1, 0.7f);
	Far(TEXT("SM_ColumnTrio"), 140.f, 200.f, 5, 2, 0.8f, 1.f, 0.1f, 0, 2.f, 180.f);
Zone(TEXT("Wall turn"), 7.88f, 7.94f);   // third pass: slot 175 only; the mega vat has 176 to 178
	Scatter(TEXT("SM_ColumnTrio"), 62.f, 100.f, 1, 0, 0.9f, 1.f, 0.08f, -1);
	Scatter(TEXT("SM_Gasholder"), 115.f, 155.f, 2, 0, 1.f, 1.2f, 0.1f, -1);
Zone(TEXT("Sky: waste run"), 8.09f, 9.46f);
	Far(TEXT("SM_TankCluster"), 115.f, 175.f, 2, 1, 0.8f, 1.f, 0.1f, 0, 1.9f, 20.f);
	Far(TEXT("/Game/Maps/ChemicalWorks/Props/SM_Vat"), 105.f, 150.f, 3, 0, 0.8f, 1.2f, 0.15f, 0, 1.8f, 180.f);
	Far(TEXT("SM_Warehouse"), 190.f, 260.f, 3, 2, 0.8f, 1.1f, 0.1f, 0, 2.4f, 10.f);
Zone(TEXT("Sky: export terminal"), 9.46f, 13.4f);
	Scatter(TEXT("SM_Catwalk"), 48.f, 66.f, 6, 4, 0.7f);
Zone(TEXT("Sky: cooling field"), 13.4f, 17.4f);
	Scatter(TEXT("SM_ColumnTrio"), 70.f, 125.f, 5, 2, 0.8f, 1.f, 0.08f, -1);
Zone(TEXT("Sky: tank farm B"), 17.4f, 21.4f);
	Scatter(TEXT("SM_ColumnTrio"), 70.f, 125.f, 5, 3, 0.8f, 1.f, 0.08f, -1);
Zone(TEXT("Reactor core two"), 21.24f, 21.27f);
	Far(TEXT("SM_ReactorCore"), 150.f, 150.f, 1, 0, 1.f, 1.1f, 0.f, -1, 1.7f, 0.f);
Zone(TEXT("Sky: reactor two"), 21.4f, 25.6f);
	Scatter(TEXT("SM_ColumnTrio"), 75.f, 125.f, 4, 1, 0.8f, 1.f, 0.08f);
	Scatter(TEXT("SM_Catwalk"), 46.f, 62.f, 5, 2, 0.8f);

// ---- ground: the plant roads (26 m), then two rows of yards, ponds and pads (43-65 m and 67-97 m)
Zone(TEXT("Ground: gate"), 0.05f, 1.24f);
	Scatter(TEXT("SM_AccessRoad"), 43.f, 44.f, 5, 2, 0.85f);
	Far(TEXT("SM_CarPark"), 51.f, 56.f, 4, 0, 0.8f, 1.f, 0.03f, 0, 1.1f, 5.f);
	Far(TEXT("SM_YardLot"), 51.f, 57.f, 4, 2, 0.8f, 1.f, 0.03f, 0, 1.1f, 6.f);
	Far(TEXT("SM_YardLot2"), 52.f, 57.f, 4, 3, 0.8f, 1.f, 0.03f, 0, 1.1f, 6.f);
	Far(TEXT("SM_YardLot3"), 80.f, 87.f, 4, 1, 0.8f, 1.f, 0.03f, 0, 1.3f, 6.f);
	Far(TEXT("SM_TankPad"), 80.f, 87.f, 4, 3, 0.8f, 1.f, 0.03f, 0, 1.3f, 6.f);
	Far(TEXT("SM_HeliPad"), 82.f, 86.f, 9, 4, 1.f, 1.f, 0.f, 1, 1.3f, 20.f);
Zone(TEXT("Ground: tank farm"), 1.4f, 2.04f);
	Scatter(TEXT("SM_AccessRoad"), 43.f, 44.f, 6, 1, 0.85f);
	Far(TEXT("SM_TankPad"), 51.f, 56.f, 3, 0, 0.8f, 1.f, 0.03f, 0, 1.1f, 5.f);
	Far(TEXT("SM_Pond"), 51.f, 56.f, 3, 1, 0.8f, 1.f, 0.03f, 0, 1.1f, 5.f);
	Far(TEXT("SM_PipeField"), 57.f, 60.f, 3, 2, 0.8f, 1.f, 0.03f, 0, 1.15f, 4.f);
	Far(TEXT("SM_Clarifier"), 81.f, 86.f, 3, 1, 0.7f, 1.f, 0.03f, 0, 1.3f, 180.f);
	Far(TEXT("SM_TankPad"), 80.f, 86.f, 3, 2, 0.7f, 1.f, 0.03f, 0, 1.3f, 5.f);
Zone(TEXT("Ground: tank farm low"), 2.19f, 2.6f);
	Far(TEXT("SM_TankPad"), 51.f, 56.f, 3, 0, 0.8f, 1.f, 0.03f, 0, 1.1f, 5.f);
	Far(TEXT("SM_Pond"), 51.f, 56.f, 3, 1, 0.8f, 1.f, 0.03f, 0, 1.1f, 5.f);
	Far(TEXT("SM_PipeField"), 57.f, 60.f, 3, 2, 0.8f, 1.f, 0.03f, 0, 1.15f, 4.f);
	Far(TEXT("SM_Clarifier"), 81.f, 86.f, 3, 1, 0.7f, 1.f, 0.03f, 0, 1.3f, 180.f);
	Far(TEXT("SM_YardLot3"), 80.f, 86.f, 3, 2, 0.7f, 1.f, 0.03f, 0, 1.3f, 5.f);
Zone(TEXT("Ground: process units"), 2.93f, 3.6f);
	Far(TEXT("SM_PipeField"), 57.f, 60.f, 3, 0, 0.85f, 1.f, 0.03f, 0, 1.15f, 4.f);
	Far(TEXT("SM_Clarifier"), 52.f, 56.f, 3, 2, 0.85f, 1.f, 0.03f, 0, 1.1f, 180.f);
	Far(TEXT("SM_Pond2"), 81.f, 86.f, 3, 0, 0.8f, 1.f, 0.03f, 0, 1.3f, 5.f);
	Far(TEXT("SM_YardLot"), 80.f, 86.f, 3, 2, 0.8f, 1.f, 0.03f, 0, 1.3f, 6.f);
Zone(TEXT("Ground: reactor plain"), 3.89f, 4.26f);
	Far(TEXT("SM_Canal"), 50.f, 54.f, 3, 0, 0.85f, 1.f, 0.03f, 0, 1.1f, 3.f);
	Far(TEXT("SM_Pond2"), 51.f, 56.f, 3, 1, 0.85f, 1.f, 0.03f, 0, 1.1f, 5.f);
	Far(TEXT("SM_YardLot2"), 52.f, 57.f, 3, 2, 0.85f, 1.f, 0.03f, 0, 1.1f, 6.f);
	Far(TEXT("SM_Clarifier"), 81.f, 86.f, 4, 1, 0.8f, 1.f, 0.03f, 0, 1.3f, 180.f);
	Far(TEXT("SM_HeliPad"), 82.f, 86.f, 4, 3, 0.8f, 1.f, 0.f, 0, 1.3f, 20.f);
Zone(TEXT("Ground: reactor block"), 4.55f, 5.35f);
	Scatter(TEXT("SM_AccessRoad"), 43.f, 44.f, 6, 3, 0.85f);
	Far(TEXT("SM_Canal"), 50.f, 54.f, 3, 0, 0.85f, 1.f, 0.03f, 0, 1.1f, 3.f);
	Far(TEXT("SM_Pond2"), 51.f, 56.f, 3, 1, 0.85f, 1.f, 0.03f, 0, 1.1f, 5.f);
	Far(TEXT("SM_YardLot2"), 52.f, 57.f, 3, 2, 0.85f, 1.f, 0.03f, 0, 1.1f, 6.f);
	Far(TEXT("SM_Clarifier"), 81.f, 86.f, 4, 1, 0.8f, 1.f, 0.03f, 0, 1.3f, 180.f);
	Far(TEXT("SM_CarPark"), 80.f, 85.f, 4, 3, 0.8f, 1.f, 0.03f, 0, 1.3f, 5.f);
Zone(TEXT("Ground: pipe alley"), 5.35f, 6.43f);
	Far(TEXT("SM_PipeField"), 57.f, 60.f, 3, 0, 0.8f, 1.f, 0.03f, 0, 1.15f, 4.f);
	Far(TEXT("SM_Canal"), 60.f, 64.f, 3, 1, 0.8f, 1.f, 0.03f, 0, 1.15f, 3.f);
	Far(TEXT("SM_YardLot"), 62.f, 66.f, 3, 2, 0.8f, 1.f, 0.03f, 0, 1.15f, 6.f);
	Far(TEXT("SM_Pond"), 84.f, 88.f, 4, 0, 0.7f, 1.f, 0.03f, 0, 1.3f, 5.f);
	Far(TEXT("SM_TankPad"), 84.f, 88.f, 4, 2, 0.7f, 1.f, 0.03f, 0, 1.3f, 5.f);
Zone(TEXT("Ground: pipe alley low"), 6.58f, 6.72f);
	Far(TEXT("SM_PipeField"), 57.f, 60.f, 2, 0, 0.8f, 1.f, 0.03f, 0, 1.15f, 4.f);
	Far(TEXT("SM_YardLot"), 62.f, 66.f, 2, 1, 0.8f, 1.f, 0.03f, 0, 1.15f, 6.f);
Zone(TEXT("Ground: loading yards"), 6.72f, 7.32f);
	Scatter(TEXT("SM_AccessRoad"), 43.f, 44.f, 4, 1, 0.85f);
	Far(TEXT("SM_RailFan"), 48.f, 52.f, 3, 0, 0.85f, 1.f, 0.03f, 0, 1.1f, 3.f);
	Far(TEXT("SM_YardLot2"), 52.f, 57.f, 3, 1, 0.85f, 1.f, 0.03f, 0, 1.1f, 6.f);
	Far(TEXT("SM_YardLot3"), 51.f, 56.f, 3, 2, 0.85f, 1.f, 0.03f, 0, 1.1f, 6.f);
	Far(TEXT("SM_YardLot"), 82.f, 87.f, 4, 1, 0.8f, 1.f, 0.03f, 0, 1.3f, 6.f);
	Far(TEXT("SM_CarPark"), 82.f, 86.f, 4, 3, 0.8f, 1.f, 0.03f, 0, 1.3f, 5.f);
Zone(TEXT("Ground: loading yards east"), 7.72f, 8.09f);
	Far(TEXT("SM_RailFan"), 48.f, 52.f, 3, 0, 0.85f, 1.f, 0.03f, 0, 1.1f, 3.f);
	Far(TEXT("SM_YardLot2"), 52.f, 57.f, 3, 1, 0.85f, 1.f, 0.03f, 0, 1.1f, 6.f);
	Far(TEXT("SM_YardLot3"), 51.f, 56.f, 3, 2, 0.85f, 1.f, 0.03f, 0, 1.1f, 6.f);
	Far(TEXT("SM_YardLot"), 82.f, 87.f, 4, 1, 0.8f, 1.f, 0.03f, 0, 1.3f, 6.f);
Zone(TEXT("Ground: waste run"), 8.09f, 9.46f);
	Far(TEXT("SM_Lagoon"), 78.f, 86.f, 3, 0, 0.8f, 1.f, 0.05f, 0, 1.3f, 180.f);
	Far(TEXT("SM_Lagoon"), 54.f, 58.f, 3, 1, 0.85f, 0.9f, 0.05f, 0, 1.1f, 180.f);
	Far(TEXT("SM_YardLot3"), 51.f, 56.f, 3, 2, 0.85f, 1.f, 0.03f, 0, 1.1f, 6.f);
	Far(TEXT("SM_Pond2"), 82.f, 87.f, 3, 1, 0.7f, 1.f, 0.03f, 0, 1.3f, 5.f);
	Far(TEXT("SM_TankPad"), 82.f, 87.f, 3, 2, 0.7f, 1.f, 0.03f, 0, 1.3f, 5.f);
Zone(TEXT("Ground: export terminal"), 9.46f, 13.4f);
	Scatter(TEXT("SM_AccessRoad"), 43.f, 44.f, 5, 1, 0.85f);
	Far(TEXT("SM_RailFan"), 48.f, 52.f, 3, 0, 0.8f, 1.f, 0.03f, 0, 1.1f, 3.f);
	Far(TEXT("SM_YardLot"), 52.f, 57.f, 3, 1, 0.8f, 1.f, 0.03f, 0, 1.1f, 6.f);
	Far(TEXT("SM_CarPark"), 51.f, 56.f, 6, 2, 0.8f, 1.f, 0.03f, 0, 1.1f, 5.f);
	Far(TEXT("SM_YardLot2"), 52.f, 57.f, 6, 5, 0.8f, 1.f, 0.03f, 0, 1.1f, 6.f);
	Far(TEXT("SM_TankPad"), 82.f, 87.f, 5, 1, 0.8f, 1.f, 0.03f, 0, 1.3f, 5.f);
	Far(TEXT("SM_YardLot3"), 82.f, 87.f, 5, 3, 0.8f, 1.f, 0.03f, 0, 1.3f, 6.f);
	Far(TEXT("SM_HeliPad"), 82.f, 86.f, 15, 4, 1.f, 1.f, 0.f, 0, 1.3f, 20.f);
Zone(TEXT("Ground: cooling field"), 14.02f, 17.4f);
	Far(TEXT("SM_Canal"), 86.f, 90.f, 3, 0, 0.5f, 1.f, 0.03f, 0, 1.3f, 3.f);
	Far(TEXT("SM_YardLot"), 84.f, 88.f, 3, 1, 0.8f, 1.f, 0.03f, 0, 1.3f, 6.f);
	Far(TEXT("SM_YardLot3"), 84.f, 88.f, 3, 2, 0.8f, 1.f, 0.03f, 0, 1.3f, 6.f);
	Far(TEXT("SM_RailFan"), 101.f, 104.f, 6, 3, 0.7f, 1.f, 0.03f, 0, 1.4f, 3.f);
	Far(TEXT("SM_YardLot2"), 106.f, 109.f, 6, 0, 0.7f, 1.f, 0.03f, 0, 1.45f, 6.f);
Zone(TEXT("Ground: tank farm B"), 17.4f, 21.4f);
	Scatter(TEXT("SM_AccessRoad"), 43.f, 44.f, 6, 4, 0.85f);
	Far(TEXT("SM_TankPad"), 51.f, 56.f, 3, 0, 0.8f, 1.f, 0.03f, 0, 1.1f, 5.f);
	Far(TEXT("SM_Pond"), 51.f, 56.f, 3, 1, 0.8f, 1.f, 0.03f, 0, 1.1f, 5.f);
	Far(TEXT("SM_PipeField"), 57.f, 60.f, 3, 2, 0.8f, 1.f, 0.03f, 0, 1.15f, 4.f);
	Far(TEXT("SM_Clarifier"), 81.f, 86.f, 3, 1, 0.7f, 1.f, 0.03f, 0, 1.3f, 180.f);
	Far(TEXT("SM_YardLot"), 82.f, 87.f, 3, 2, 0.7f, 1.f, 0.03f, 0, 1.3f, 6.f);
Zone(TEXT("Ground: reactor two"), 21.4f, 22.15f);
	Far(TEXT("SM_Canal"), 50.f, 54.f, 4, 1, 0.85f, 1.f, 0.03f, 0, 1.1f, 3.f);
	Far(TEXT("SM_Pond2"), 51.f, 56.f, 4, 3, 0.85f, 1.f, 0.03f, 0, 1.1f, 5.f);
	Far(TEXT("SM_HeliPad"), 82.f, 86.f, 4, 1, 0.8f, 1.f, 0.f, 0, 1.3f, 20.f);
	Far(TEXT("SM_Clarifier"), 81.f, 86.f, 4, 3, 0.8f, 1.f, 0.03f, 0, 1.3f, 180.f);

// ---- correction round. The cooling field's ground before the circuit closes on itself (13.4-14.02 km). From 14.02 km
// the finale's second lap weaves across the first, 2 to 50 m either side of it, crossing it again and again: there the
// zone above lays one unlit row 74 m out and beyond, which no stretch of either lap comes nearer than 23 m to.
Zone(TEXT("Ground: cooling field west"), 13.4f, 14.02f);
	Far(TEXT("SM_Canal"), 50.f, 54.f, 3, 0, 0.8f, 1.f, 0.03f, 0, 1.1f, 3.f);
	Far(TEXT("SM_Clarifier"), 52.f, 56.f, 3, 1, 0.8f, 1.f, 0.03f, 0, 1.1f, 180.f);
	Far(TEXT("SM_Pond"), 51.f, 56.f, 3, 2, 0.8f, 1.f, 0.03f, 0, 1.1f, 5.f);
	Far(TEXT("SM_Pond2"), 82.f, 87.f, 3, 1, 0.7f, 1.f, 0.03f, 0, 1.3f, 5.f);
	Far(TEXT("SM_PipeField"), 82.f, 86.f, 3, 2, 0.7f, 1.f, 0.03f, 0, 1.3f, 4.f);

// ---- correction round. Ground, the near row (26 to 47 m): UNLIT yards, sidings, channels and pipe fields under the
// roadside's own pieces (the fence, the drums, the pump houses and the parked tankers then stand on something).
// A camera rolled 20 to 30 degrees with an S-bend sees the land from 16 to 45 m and little beyond it; a camera in a
// corkscrew, a loop or a pipe sees the same strip from above. Far rules, so the row runs on through those; it starts
// 26 m out (beside a banked stretch the land is cut down to the road's low edge inside that), nothing in it is lit,
// and it stops short of the ramps, the jumps and the finale's doubled circuit like the rows behind it.
Zone(TEXT("Yards: gate"), 0.05f, 1.24f);
	Far(TEXT("SM_YardLot2"), 37.5f, 38.f, 4, 1, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);
	Far(TEXT("SM_YardLot3"), 36.f, 37.f, 4, 3, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);
Zone(TEXT("Yards: tank farm"), 1.4f, 2.04f);
	Far(TEXT("SM_PipeField"), 41.8f, 42.2f, 4, 0, 0.8f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_YardLot3"), 36.f, 37.f, 4, 2, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);
Zone(TEXT("Yards: tank farm low"), 2.19f, 2.6f);
	Far(TEXT("SM_PipeField"), 41.8f, 42.2f, 4, 0, 0.8f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_YardLot3"), 36.f, 37.f, 4, 2, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);
Zone(TEXT("Yards: process units"), 2.93f, 3.6f);
	Far(TEXT("SM_YardLot"), 36.5f, 37.f, 4, 1, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);
	Far(TEXT("SM_YardLot2"), 37.5f, 38.f, 4, 3, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);
Zone(TEXT("Yards: reactor plain"), 3.89f, 4.26f);
	Far(TEXT("SM_Canal"), 32.5f, 33.f, 4, 0, 0.8f, 1.f, 0.03f, 0, 0.95f, 3.f);
	Far(TEXT("SM_YardLot2"), 37.5f, 38.f, 4, 2, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);
Zone(TEXT("Yards: reactor block"), 4.55f, 5.35f);
	Far(TEXT("SM_Canal"), 32.5f, 33.f, 4, 0, 0.8f, 1.f, 0.03f, 0, 0.95f, 3.f);
	Far(TEXT("SM_YardLot2"), 37.5f, 38.f, 4, 2, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);
Zone(TEXT("Yards: pipe alley"), 5.35f, 6.43f);
	Far(TEXT("SM_Canal"), 32.5f, 33.f, 4, 1, 0.8f, 1.f, 0.03f, 0, 0.95f, 3.f);
	Far(TEXT("SM_YardLot"), 36.5f, 37.f, 4, 3, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);
Zone(TEXT("Yards: loading yards"), 6.58f, 7.32f);
	Far(TEXT("SM_RailFan"), 31.f, 32.f, 4, 0, 0.8f, 1.f, 0.03f, 0, 0.95f, 3.f);
	Far(TEXT("SM_YardLot2"), 37.5f, 38.f, 4, 2, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);
Zone(TEXT("Yards: loading yards east"), 7.72f, 8.09f);
	Far(TEXT("SM_RailFan"), 31.f, 32.f, 4, 0, 0.8f, 1.f, 0.03f, 0, 0.95f, 3.f);
	Far(TEXT("SM_YardLot2"), 37.5f, 38.f, 4, 2, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);
Zone(TEXT("Yards: waste run"), 8.09f, 9.46f);
	Far(TEXT("SM_YardLot3"), 36.f, 37.f, 4, 0, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);
	Far(TEXT("SM_Canal"), 32.5f, 33.f, 4, 2, 0.7f, 1.f, 0.03f, 0, 0.95f, 3.f);
Zone(TEXT("Yards: export terminal"), 9.46f, 14.02f);
	Far(TEXT("SM_RailFan"), 31.f, 32.f, 4, 1, 0.8f, 1.f, 0.03f, 0, 0.95f, 3.f);
	Far(TEXT("SM_YardLot"), 36.5f, 37.f, 4, 3, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);
	Far(TEXT("SM_YardLot2"), 37.5f, 38.f, 8, 0, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);
Zone(TEXT("Yards: tank farm B"), 17.4f, 21.4f);
	Far(TEXT("SM_PipeField"), 41.8f, 42.2f, 4, 0, 0.8f, 1.f, 0.03f, 0, 1.f, 4.f);
	Far(TEXT("SM_YardLot3"), 36.f, 37.f, 4, 2, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);
Zone(TEXT("Yards: reactor two"), 21.4f, 22.15f);
	Far(TEXT("SM_Canal"), 32.5f, 33.f, 4, 0, 0.8f, 1.f, 0.03f, 0, 0.95f, 3.f);
	Far(TEXT("SM_YardLot2"), 37.5f, 38.f, 4, 2, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);

// ---- THE CUT BANKS (15 Nov 2026, after the second pass). Inside a banked bend the track cuts the land down to the
// road's low edge; that cutting was the land's one plain colour, a third of the frame from the rolled camera, and no
// Scatter or Site piece can stand on it. Bank rules (FTDress.h) have the track lay panels on it itself, fitted to the
// bank's own angle and grade: four to a 45 m slot (11.3 m of road each), tall designs where a panel's run up the
// cutting is 5.5 m or more (bends banked 32 degrees and up), a low one on the shallower cuttings (down to 2.4 m: about
// 20 degrees of bank). Every and phase count panels here. ft.DressBank 0 leaves them out, for a before and an after.
// Chemical Works: the works' concrete facing. SM_CutRevet (slabs, the teal main, a hazard placard) and SM_CutRevet2
// (two leaking outlets) take alternate panels of the deep cuttings: the two climbing hairpins (2.6 and 3.6 km), the
// wall turn at 7.9 km, the S-bends at 5.0 km and the finale's eight bends. SM_CutKerb on the shallow ones, every
// third panel the stained one (SM_CutKerb2).
Zone(TEXT("Cut banks"), 0.f, 99.f);
	Bank(TEXT("SM_CutRevet"), 4, 2, 0, 1.f, 5.5f);
	Bank(TEXT("SM_CutRevet2"), 4, 2, 1, 1.f, 5.5f);
	Bank(TEXT("SM_CutKerb"), 4, 3, 0, 1.f, 2.4f, 5.5f);
	Bank(TEXT("SM_CutKerb"), 4, 3, 1, 1.f, 2.4f, 5.5f);
	Bank(TEXT("SM_CutKerb2"), 4, 3, 2, 1.f, 2.4f, 5.5f);
};
SecondPass();

auto ThirdPass = [&]()
{
// =====================================================================================================================
// THIRD PASS (15 Nov 2026, the overhaul): dense, distinctive and alive at racing speed. "Pipe racks, loading bays, and
// tanks, with cycling machinery, steam releases, and moving work lights." APPENDED, like the second pass (three zones
// above were changed in place and say so: the first pass's loose barriers have a chance of nothing, and "Loading
// yards" and "Wall turn" stop at slot 175).
//
// What the chase camera sees of this road (the geometry, worked out once: DevLog\jam\dress_chem_nevada.md, third pass):
//   - on level road with the car in the middle, the road's own barrier hides everything under 2 m nearer than 26 m,
//     and the ground itself out to 33 m. The 17 to 24 m row is seen when the car is on that side of the road, from the
//     low side of a banked bend (this road is banked nearly everywhere: the rolled camera looks DOWN on the land
//     beyond the cutting's rim), from loops and jumps, and by the highlight reel's roadside cameras;
//   - on the HIGH side of a banked bend the camera looks up over the raised barrier: at 26 degrees it sees nothing
//     under 11 m tall at 24 m out, 16 m at 40 m; so the high side wants TALL things at the sight line;
//   - a piece a slot long is a chord: on the outside of a bend of 190 m radius two of them leave 4 to 7 m between
//     them, on the inside they lap.
// So: 1. THE KERB (17.3 m): SM_KerbRun and SM_StoreRow in turn, every slot, both sides: pale, low, unlit.
//     2. THE SIGHT LINE (24 to 38 m): tank rows, the roadside rack and compressor sheds, all unlit (so they may stand
//        24 m out), district by district; and the lit loading bays brought up to 31 m. Behind the perimeter fence
//        (27.5 m) where a district has one.
//     3. STEAM AND MACHINES (37 to 50 m): blow-down stacks that let go on their own clocks, floodlight masts whose
//        heads swing, rotary kilns turning, works roads with tugs driving them.
//     4. THE FLARE AVENUE (12.8 to 13.8 km): eight pairs of 82 m flare derricks over the finale's long run.
//     5. THE MEGA VAT (8.0 km): dead ahead of the jump at 7.5 km, where the road swerves round it.
//     6. THE DOUBLED CIRCUIT'S GROUND: unlit yards tried at every distance on both laps; the clearance test keeps the
//        ones that stand clear of the other lap.
//     7. THE HORIZON'S CARDS: three rows of flat skyline in three receding tones.
//     8. A CROWN OF FLARES round the outside of each 57 degree wall turn (2.6 to 3.0 km, 3.6 to 3.9 km).

// ---- sets
Set(TEXT("KerbLine"), { { TEXT("SM_KerbRun"), 0, 0, 0, 0, 1.f } });
Set(TEXT("StoreLine"), { { TEXT("SM_StoreRow"), 0, 0, 0, 0, 1.f } });
Set(TEXT("TankLine"), { { TEXT("SM_TankRow"), 0, 0, 0, 0, 1.f } });
Set(TEXT("RackNearLine"), { { TEXT("SM_RackNear"), 0, 0, 0, 0, 1.f } });
Set(TEXT("BayNear"), {
	{ TEXT("SM_LoadingBay"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_DrumPile2"), -15, -2, 0, 20, 1.f },
});
Set(TEXT("FlareGate"), { { TEXT("SM_FlareTower"), 0, 0, 0, 0, 1.f } });
Set(TEXT("MegaVat"), {
	{ TEXT("SM_MegaVat"), 0, 0, 0, 0, 1.f },
	{ TEXT("SM_BlowStack"), -62, 12, 0, 0, 1.5f },
	{ TEXT("SM_BlowStack"), 64, 16, 0, 0, 1.3f },
	{ TEXT("SM_LightMast"), -54, -30, 0, 0, 1.f },
	{ TEXT("SM_LightMast"), 54, -30, 0, 0, 1.f },
});

// ---- 1. the kerb: every slot, both sides, the two designs in turn (opposite turns on the two sides). Its road face
// stands 17.3 m from the centre line; the ground pipe of the first pass runs in the strip it leaves (18.2 to 20 m)
Zone(TEXT("Kerb"), 0.03f, 99.f);
	Site(TEXT("KerbLine"), -1, 20.15f, 2, 0, 1.f);
	Site(TEXT("StoreLine"), -1, 20.15f, 2, 1, 1.f);
	Site(TEXT("KerbLine"), 1, 20.15f, 2, 1, 1.f);
	Site(TEXT("StoreLine"), 1, 20.15f, 2, 0, 1.f);

// ---- 2. the sight line: tall and unlit, district by district. Where a district has the perimeter fence (27.5 m) the
// rows stand behind it (31.5 and 34 m); elsewhere at 26.5 m (the rack) and 28.5 m (the tanks). The lit loading bays'
// canopy edge stands 31.5 m out
Zone(TEXT("Near: gate"), 0.03f, 1.23f);
	Site(TEXT("RackNearLine"), -1, 31.5f, 3, 0, 0.9f);
	Site(TEXT("RackNearLine"), 1, 31.5f, 3, 1, 0.9f);
	Site(TEXT("TankLine"), -1, 34.f, 3, 2, 0.8f);
	Site(TEXT("TankLine"), 1, 34.f, 3, 0, 0.8f);
Zone(TEXT("Near: tank farm"), 1.23f, 2.6f);
	Site(TEXT("TankLine"), -1, 28.5f, 1, 0, 0.85f);
	Site(TEXT("TankLine"), 1, 28.5f, 1, 0, 0.85f);
Zone(TEXT("Near: process units"), 2.6f, 3.98f);
	Site(TEXT("RackNearLine"), -1, 26.5f, 1, 0, 0.9f);
	Site(TEXT("RackNearLine"), 1, 26.5f, 1, 0, 0.9f);
	Scatter(TEXT("SM_Compressor"), 33.f, 36.f, 3, 1, 0.8f);
Zone(TEXT("Near: reactor block"), 3.98f, 5.35f);
	Site(TEXT("RackNearLine"), -1, 31.5f, 2, 0, 0.9f);
	Site(TEXT("RackNearLine"), 1, 31.5f, 2, 1, 0.9f);
	Site(TEXT("TankLine"), -1, 34.f, 4, 1, 0.8f);
	Site(TEXT("TankLine"), 1, 34.f, 4, 0, 0.8f);
Zone(TEXT("Near: pipe alley"), 5.35f, 6.72f);
	Site(TEXT("RackNearLine"), -1, 26.5f, 1, 0, 0.92f);
	Site(TEXT("RackNearLine"), 1, 26.5f, 1, 0, 0.92f);
	Scatter(TEXT("SM_Compressor"), 33.f, 36.f, 4, 2, 0.8f);
Zone(TEXT("Near: loading yards"), 6.72f, 7.94f);
	Site(TEXT("BayNear"), -1, 36.5f, 3, 0, 0.9f);
	Site(TEXT("BayNear"), 1, 36.5f, 3, 1, 0.9f);
	Site(TEXT("TankLine"), -1, 28.5f, 3, 1, 0.85f);
	Site(TEXT("TankLine"), 1, 28.5f, 3, 2, 0.85f);
Zone(TEXT("Near: waste run"), 8.09f, 9.46f);
	Site(TEXT("TankLine"), -1, 34.f, 2, 0, 0.85f);
	Site(TEXT("TankLine"), 1, 34.f, 2, 1, 0.85f);
Zone(TEXT("Near: export terminal"), 9.46f, 12.72f);
	Site(TEXT("RackNearLine"), -1, 26.5f, 2, 0, 0.9f);
	Site(TEXT("TankLine"), -1, 28.5f, 4, 1, 0.9f);
	Site(TEXT("BayNear"), -1, 36.5f, 4, 3, 0.9f);
	Site(TEXT("RackNearLine"), 1, 26.5f, 2, 1, 0.9f);
	Site(TEXT("TankLine"), 1, 28.5f, 4, 0, 0.9f);
	Site(TEXT("BayNear"), 1, 36.5f, 4, 2, 0.9f);
Zone(TEXT("Near: flare avenue"), 12.72f, 13.85f);
	Site(TEXT("RackNearLine"), -1, 26.5f, 1, 0, 0.9f);
	Site(TEXT("RackNearLine"), 1, 26.5f, 1, 0, 0.9f);
Zone(TEXT("Near: cooling field"), 13.85f, 17.4f);
	Site(TEXT("RackNearLine"), -1, 26.5f, 2, 0, 0.9f);
	Site(TEXT("RackNearLine"), 1, 26.5f, 2, 1, 0.9f);
	Site(TEXT("TankLine"), -1, 28.5f, 4, 1, 0.85f);
	Site(TEXT("TankLine"), 1, 28.5f, 4, 2, 0.85f);
	Scatter(TEXT("SM_Compressor"), 33.f, 36.f, 5, 3, 0.8f);
Zone(TEXT("Near: tank farm B"), 17.4f, 21.4f);
	Site(TEXT("TankLine"), -1, 34.f, 1, 0, 0.8f);
	Site(TEXT("TankLine"), 1, 34.f, 1, 0, 0.8f);
Zone(TEXT("Near: reactor two"), 21.4f, 25.6f);
	Site(TEXT("RackNearLine"), -1, 31.5f, 2, 0, 0.9f);
	Site(TEXT("RackNearLine"), 1, 31.5f, 2, 1, 0.9f);
	Site(TEXT("TankLine"), -1, 34.f, 4, 1, 0.8f);
	Site(TEXT("TankLine"), 1, 34.f, 4, 0, 0.8f);

// ---- 3. steam and machines. A blow-down stack's steam drifts 9 m: it stands 41 m out, its plume never nearer than
// 27 m. The masts' lamp heads are 20 m up and 37 m out. Kilns and works roads take the 46 to 50 m band, which the
// first pass's sets (52 m and beyond) leave mostly free. None in the flare avenue (12.72 to 13.85 km)
Zone(TEXT("Steam"), 0.03f, 12.72f);
	Scatter(TEXT("SM_BlowStack"), 41.f, 43.f, 5, 2, 0.85f, 1.f, 0.12f);
	Scatter(TEXT("SM_LightMast"), 37.f, 40.f, 5, 4, 0.7f, 1.f, 0.1f, 0, 180.f);
Zone(TEXT("Steam: finale"), 13.85f, 99.f);
	Scatter(TEXT("SM_BlowStack"), 41.f, 43.f, 6, 4, 0.85f, 1.f, 0.12f);
	Scatter(TEXT("SM_LightMast"), 37.f, 40.f, 6, 1, 0.7f, 1.f, 0.1f, 0, 180.f);
Zone(TEXT("Machines: tank farm"), 1.23f, 2.6f);
	Scatter(TEXT("SM_WorkRoad"), 49.f, 50.f, 9, 3, 1.f, 1.f, 0.f, 0, 2.f);
Zone(TEXT("Machines: process units"), 2.6f, 3.98f);
	Scatter(TEXT("SM_Kiln"), 46.f, 48.f, 4, 2, 0.f, 1.f, 0.f, 0, 5.f);
Zone(TEXT("Machines: loading yards"), 6.72f, 7.94f);
	Scatter(TEXT("SM_Kiln"), 46.f, 48.f, 6, 4, 0.9f, 1.f, 0.f, 0, 5.f);
Zone(TEXT("Machines: export terminal"), 9.46f, 12.72f);
	Scatter(TEXT("SM_WorkRoad"), 49.f, 50.f, 8, 5, 1.f, 1.f, 0.f, 0, 2.f);
	Scatter(TEXT("SM_Kiln"), 58.f, 62.f, 9, 1, 0.9f, 1.f, 0.f, 0, 5.f);
Zone(TEXT("Machines: cooling field"), 13.85f, 17.4f);
	Scatter(TEXT("SM_Kiln"), 46.f, 48.f, 6, 2, 0.9f, 1.f, 0.f, 0, 5.f);
Zone(TEXT("Machines: tank farm B"), 17.4f, 21.4f);
	Scatter(TEXT("SM_WorkRoad"), 49.f, 50.f, 8, 2, 1.f, 1.f, 0.f, 0, 2.f);
Zone(TEXT("Machines: reactor two"), 21.4f, 25.6f);
	Scatter(TEXT("SM_Kiln"), 46.f, 48.f, 8, 5, 0.9f, 1.f, 0.f, 0, 5.f);

// ---- 4. THE FLARE AVENUE: the finale's long run from the bend at 12.76 km to the bend at 13.85 km (a rise, then
// 800 m of gentle S). Eight pairs of derricks, 46 m out, every third slot (283 to 304): from the bend they are a
// file of flames a kilometre long; then they pass, one pair after another, 82 m overhead
Zone(TEXT("Flare avenue"), 12.78f, 13.8f);
	Site(TEXT("FlareGate"), -1, 46.f, 3, 2, 1.f);
	Site(TEXT("FlareGate"), 1, 46.f, 3, 2, 1.f);

// ---- 5. THE MEGA VAT. The jump at 7.53 km takes off heading 51 degrees; the road lands, runs straight to 7.87 km and
// then turns right through 95 degrees, banked 54 (the wall turn). The take-off's line passes 60 m to the LEFT of slot
// 177: the vat stands there (92 m out and laid 1.35 times the mesh's size: 103 m across, 70 m to its rim, its near
// wall 41 m from the centre line), dead ahead through the jump and the straight, and passed on the outside of the
// wall turn, where the rolled camera looks up at its rim, its stirrer and its steam. The three
// slots it takes (176 to 178) are dressed here: the yards on the inside, plant behind it, the horizon
Zone(TEXT("Mega vat: round it"), 7.95f, 8.09f);
	Site(TEXT("RailYard"), 1, 74.f, 2, 0, 1.f);
	Scatter(TEXT("SM_Warehouse"), 160.f, 230.f, 1, 0, 0.8f, 1.1f, 0.1f, 1);
	Scatter(TEXT("SM_Gasholder"), 185.f, 225.f, 2, 0, 1.f, 1.2f, 0.1f, -1);
	Scatter(TEXT("SM_CoolingTower"), 200.f, 260.f, 2, 1, 1.f, 1.f, 0.1f, -1);
	Far(TEXT("SM_FarPlant"), 400.f, 650.f, 2, 0, 0.9f, 1.f, 0.15f);
Zone(TEXT("Mega vat"), 8.f, 8.02f);
	Site(TEXT("MegaVat"), -1, 92.f, 1, 0, 1.f, 1.35f);
Zone(TEXT("Wall turn: the last trio"), 8.09f, 8.11f);
	Scatter(TEXT("SM_ColumnTrio"), 62.f, 100.f, 1, 0, 1.f, 1.f, 0.08f, -1);

// ---- 6. the doubled circuit's ground. From 14.02 km the finale laps a circuit that 22.18 to 25.58 km runs round
// again, 2 to 50 m beside the first lap and across it. The second pass left it one row 74 m out. These rules try
// unlit yards at every distance, from both laps; a piece that would touch the other lap is left out by the track's
// own clearance test, so what stands is what fits between and round the two roads. (Nothing lit: the test knows
// nothing of light.) Each yard has its own slab height, so two that overlap never share a top
Zone(TEXT("Yards: doubled circuit"), 14.02f, 17.4f);
	Far(TEXT("SM_YardLot3"), 36.f, 37.f, 3, 0, 0.9f, 1.f, 0.03f, 0, 0.98f, 5.f);
	Far(TEXT("SM_RailFan"), 31.f, 32.f, 3, 1, 0.9f, 1.f, 0.03f, 0, 0.95f, 3.f);
	Far(TEXT("SM_YardLot"), 52.f, 57.f, 3, 2, 0.9f, 1.f, 0.03f, 0, 1.1f, 6.f);
	Far(TEXT("SM_PipeField"), 57.f, 60.f, 3, 0, 0.8f, 1.f, 0.03f, 0, 1.15f, 4.f);
	Far(TEXT("SM_YardLot2"), 52.f, 57.f, 3, 1, 0.8f, 1.f, 0.03f, 0, 1.1f, 6.f);
Zone(TEXT("Yards: doubled circuit, second lap"), 22.18f, 25.58f);
	Far(TEXT("SM_YardLot2"), 37.5f, 38.f, 4, 1, 0.8f, 1.f, 0.03f, 0, 0.98f, 5.f);
	Far(TEXT("SM_Canal"), 32.5f, 33.f, 4, 3, 0.8f, 1.f, 0.03f, 0, 0.95f, 3.f);
	Far(TEXT("SM_YardLot3"), 51.f, 56.f, 4, 0, 0.8f, 1.f, 0.03f, 0, 1.1f, 6.f);
	Far(TEXT("SM_YardLot"), 52.f, 57.f, 4, 2, 0.8f, 1.f, 0.03f, 0, 1.1f, 6.f);

// ---- 7. the horizon's cards: three rows of flat skyline, each its own outline and tone (the nearest the bluest and
// darkest, the furthest the palest and most violet, as this sky goes towards its horizon). A card's footing is a
// slab far wider than the card (ft_dress_chem.py, JOBS): the clearance test then keeps row A 120 m, row B 350 m and
// row C 700 m from EVERY stretch of this road, which folds back on itself everywhere, so a card is only ever a
// far thing. Most of the tries are left out for that reason, by design
Zone(TEXT("Skyline cards"), 0.f, 99.f);
	Far(TEXT("SM_SkyCardA"), 335.f, 400.f, 9, 0, 1.f, 1.f, 0.1f, 0, 25.f, 6.f);
	Far(TEXT("SM_SkyCardB"), 660.f, 800.f, 13, 5, 1.f, 1.f, 0.1f, 0, 25.f, 6.f);
	Far(TEXT("SM_SkyCardC"), 1150.f, 1350.f, 20, 11, 1.f, 1.f, 0.08f, 0, 25.f, 5.f);

// ---- 8. the crown of flares on the two wall turns of the process units: 2.62 to 2.99 km, a right-hander that climbs
// 39 m, and 3.59 to 3.94 km, a left-hander that comes down again, both banked 57 degrees. On the OUTSIDE of such a
// turn the rolled camera looks up over the raised barrier and sees nothing that stands under 60 m (it was black sky
// with a few far things adrift in it). Seven derricks on each, a third larger than the avenue's (107 m and a 34 m
// flame), 50 m out, one to a slot (59 to 65, 80 to 86): their top decks, their beacons and their flames stand in that
// sky, and from the straight before each turn they are a crown round it
Zone(TEXT("Flares: first wall turn"), 2.647f, 2.963f);
	Site(TEXT("FlareGate"), -1, 50.f, 1, 0, 1.f, 1.3f);
Zone(TEXT("Flares: second wall turn"), 3.597f, 3.914f);
	Site(TEXT("FlareGate"), 1, 50.f, 1, 0, 1.f, 1.3f);
// and one beyond the mega vat on the third wall turn (slot 179: still banked 50 degrees, the vat already behind)
Zone(TEXT("Flares: third wall turn"), 8.076f, 8.121f);
	Site(TEXT("FlareGate"), -1, 54.f, 1, 0, 1.f, 1.3f);
};
ThirdPass();

auto FourthPass = [&]()
{
// =====================================================================================================================
// FOURTH PASS (15 Nov 2026, "the built world in motion"): traffic that is not the chase, a helicopter, machines at
// work, one flare that lets go on a clock, columns for the sky half of the banked bends. APPENDED. Every mesh here is
// one of the kit's ft_life.Kit pieces (ft_dress_chem.py, MOVING4): it moves in the vertex shader, nothing ticks. All of
// it stands 70 m and more from the road or is laid by a Far rule, so none of it casts a shadow (the track's shadow rule:
// within 70 m AND over 2.5 m). The works are dense near the road: from the driving seat a thing 80 m out shows only
// above about 20 m, so what travels here travels HIGH (a train on a viaduct, a road on portal frames, drums in a
// gallery, a ring road on piers, a helicopter). NOT COMPILED and NOT SEEN when it was written (the owner's order of
// 15 Nov 17:53: build everything now, compile and look later): counts and clearances are from zones_w2\r4\w2_est3.py.
Set(TEXT("BigFlareSet"), { { TEXT("SM_BigFlare"), 0, 0, 0, 0, 1.f } });
Set(TEXT("BerthSet"), { { TEXT("SM_ShipLoader"), 0, 0, 0, 0, 1.f } });

// ---- 1. the sky half of the banked bends: a row of process columns 54 to 70 m tall on the OUTSIDE (the high side) of
// every run of three slots and more banked 18 to 48 degrees, 78 m out, where the road's own raised edge hides all that
// stands under about 26 m. Two designs in turn. (The wall turns have their flare derricks already.)
Zone(TEXT("Columns: 0.18 km"), 0.158f, 0.294f);
	Scatter(TEXT("SM_ColumnRow"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Columns: 0.50 km"), 0.475f, 0.611f);
	Scatter(TEXT("SM_ColumnRow2"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Columns: 1.31 km"), 1.289f, 1.425f);
	Scatter(TEXT("SM_ColumnRow"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("Columns: 1.45 km"), 1.425f, 1.561f);
	Scatter(TEXT("SM_ColumnRow2"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Columns: 2.08 km"), 2.059f, 2.194f);
	Scatter(TEXT("SM_ColumnRow"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("Columns: 2.26 km"), 2.240f, 2.375f);
	Scatter(TEXT("SM_ColumnRow2"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Columns: 4.03 km"), 4.004f, 4.140f);
	Scatter(TEXT("SM_ColumnRow"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Columns: 5.07 km"), 5.045f, 5.180f);
	Scatter(TEXT("SM_ColumnRow2"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Columns: 5.25 km"), 5.226f, 5.361f);
	Scatter(TEXT("SM_ColumnRow"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("Columns: 6.47 km"), 6.447f, 6.583f);
	Scatter(TEXT("SM_ColumnRow2"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Columns: 6.79 km"), 6.764f, 6.900f);
	Scatter(TEXT("SM_ColumnRow"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Columns: 6.92 km"), 6.900f, 7.035f);
	Scatter(TEXT("SM_ColumnRow2"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("Columns: 7.24 km"), 7.216f, 7.352f);
	Scatter(TEXT("SM_ColumnRow"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("Columns: 14.48 km"), 14.455f, 14.591f);
	Scatter(TEXT("SM_ColumnRow2"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Columns: 14.61 km"), 14.591f, 14.727f);
	Scatter(TEXT("SM_ColumnRow"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("Columns: 15.88 km"), 15.858f, 15.994f);
	Scatter(TEXT("SM_ColumnRow2"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Columns: 16.02 km"), 15.994f, 16.129f);
	Scatter(TEXT("SM_ColumnRow"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("Columns: 17.24 km"), 17.215f, 17.351f);
	Scatter(TEXT("SM_ColumnRow2"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Columns: 17.42 km"), 17.396f, 17.532f);
	Scatter(TEXT("SM_ColumnRow"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("Columns: 18.64 km"), 18.618f, 18.753f);
	Scatter(TEXT("SM_ColumnRow2"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Columns: 18.82 km"), 18.799f, 18.934f);
	Scatter(TEXT("SM_ColumnRow"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("Columns: 20.04 km"), 20.020f, 20.156f);
	Scatter(TEXT("SM_ColumnRow2"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Columns: 20.22 km"), 20.201f, 20.337f);
	Scatter(TEXT("SM_ColumnRow"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("Columns: 21.45 km"), 21.423f, 21.558f);
	Scatter(TEXT("SM_ColumnRow2"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Columns: 21.63 km"), 21.604f, 21.739f);
	Scatter(TEXT("SM_ColumnRow"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("Columns: 22.85 km"), 22.825f, 22.961f);
	Scatter(TEXT("SM_ColumnRow2"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Columns: 23.03 km"), 23.006f, 23.142f);
	Scatter(TEXT("SM_ColumnRow"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("Columns: 24.25 km"), 24.228f, 24.364f);
	Scatter(TEXT("SM_ColumnRow2"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Columns: 24.43 km"), 24.409f, 24.544f);
	Scatter(TEXT("SM_ColumnRow"), 78.f, 80.f, 1, 0, 1.f, 1.f, 0.f, 1, 2.f);

// ---- 2. traffic on the works' own ways, all of it up in the air where it can be seen over the near rows: the tank
// train on its viaduct (362 m), the road-tanker convoy on its elevated road (136 m), the drum galleries (136 m)
Zone(TEXT("Traffic: tank farm"), 1.23f, 2.6f);
	Scatter(TEXT("SM_TankerRoad"), 76.f, 78.f, 12, 0, 1.f, 1.f, 0.f, 0, 2.f);
Zone(TEXT("Traffic: reactor block"), 3.98f, 5.35f);
	Scatter(TEXT("SM_DrumGallery"), 75.f, 77.f, 14, 6, 1.f, 1.f, 0.f, 0, 2.f);
Zone(TEXT("Traffic: pipe alley"), 5.35f, 6.72f);
	Scatter(TEXT("SM_TankerRoad"), 76.f, 78.f, 15, 7, 1.f, 1.f, 0.f, 0, 2.f);
Zone(TEXT("Traffic: loading yards"), 6.72f, 7.94f);
	Scatter(TEXT("SM_DrumGallery"), 75.f, 77.f, 12, 4, 1.f, 1.f, 0.f, 0, 2.f);
Zone(TEXT("Traffic: waste run"), 8.09f, 9.46f);
	Scatter(TEXT("SM_DrumGallery"), 75.f, 77.f, 14, 3, 1.f, 1.f, 0.f, 0, 2.f);
Zone(TEXT("Traffic: export terminal"), 9.46f, 12.72f);
	Scatter(TEXT("SM_TankTrain"), 80.f, 82.f, 24, 5, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(TEXT("SM_TankerRoad"), 76.f, 78.f, 24, 17, 1.f, 1.f, 0.f, -1, 2.f);
Zone(TEXT("Traffic: tank farm B"), 17.4f, 21.4f);
	Scatter(TEXT("SM_TankTrain"), 80.f, 82.f, 30, 12, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("SM_TankTrain"), 80.f, 82.f, 30, 27, 1.f, 1.f, 0.f, 1, 0.f);

// ---- 3. the ring road on its piers, for the horizon: 720 m lengths 270 to 300 m out, streaks of head and tail lamps
Zone(TEXT("Ring road"), 13.4f, 25.6f);
	Far(TEXT("SM_FarHighway"), 270.f, 300.f, 44, 7, 1.f, 1.f, 0.05f, 0, 25.f, 4.f);
Zone(TEXT("Ring road: the first half"), 0.5f, 12.7f);
	Far(TEXT("SM_FarHighway"), 280.f, 310.f, 60, 15, 1.f, 1.f, 0.05f, 0, 25.f, 4.f);

// ---- 4. the works' helicopter, three of them, each on a circle 300 m across whose middle is 300 m from the road:
// behind the mega vat, over the reactor block, at the far end of the flare avenue. Never nearer than 130 m.
Zone(TEXT("Helicopter: the vat"), 7.985f, 8.031f);
	Far(TEXT("SM_Heli"), 300.f, 300.f, 1, 0, 1.f, 1.f, 0.f, -1, 25.f, 0.f);
Zone(TEXT("Helicopter: the reactor"), 4.773f, 4.818f);
	Far(TEXT("SM_Heli"), 300.f, 300.f, 1, 0, 1.f, 1.f, 0.f, 1, 25.f, 0.f);
Zone(TEXT("Helicopter: the avenue"), 13.777f, 13.822f);
	Far(TEXT("SM_Heli"), 300.f, 300.f, 1, 0, 1.f, 1.f, 0.f, -1, 25.f, 0.f);

// ---- 5. machines at work: furnace halls whose doors rise on a clock (72 m and more), floodlight towers with the
// rain falling through their light
Zone(TEXT("Furnaces: process units"), 2.6f, 3.98f);
	Scatter(TEXT("SM_FurnaceHall"), 74.f, 78.f, 9, 4, 0.9f, 1.f, 0.03f, 0, 3.f);
Zone(TEXT("Furnaces: reactor two"), 21.4f, 25.6f);
	Scatter(TEXT("SM_FurnaceHall"), 74.f, 78.f, 16, 9, 0.9f, 1.f, 0.03f, 0, 3.f);
Zone(TEXT("Rain: gate"), 0.03f, 1.23f);
	Scatter(TEXT("SM_RainLamp"), 72.f, 76.f, 9, 2, 0.9f, 1.15f, 0.05f, 0, 6.f);
Zone(TEXT("Rain: tank farm B"), 17.4f, 21.4f);
	Scatter(TEXT("SM_RainLamp"), 72.f, 76.f, 11, 2, 0.9f, 1.15f, 0.05f, 0, 6.f);

// ---- 6. two moments. THE EXPORT BERTH: a tanker being loaded, 190 m to the left of 11.07 km, where the road's heading
// at 10.6 km points (it is dead ahead for 500 m, then passed on the left); and a second berth on the right further on.
// THE GROUND FLARE: 170 m out on the outside of the hard right-hander at 18.45 km, dead ahead of the 300 m before it.
Zone(TEXT("Berth: dead ahead"), 11.062f, 11.107f);
	Site(TEXT("BerthSet"), -1, 190.f, 1, 0, 1.f);
Zone(TEXT("Berth: the second"), 12.284f, 12.329f);
	Site(TEXT("BerthSet"), 1, 130.f, 1, 0, 1.f);
Zone(TEXT("Ground flare"), 18.437f, 18.482f);
	Site(TEXT("BigFlareSet"), -1, 170.f, 1, 0, 1.f);

// ---- 7. from the review: the process units' kilns, on the side the crown of flare derricks is not on (the third
// pass's rule for them, above, has a chance of nothing now: its kilns stood through the crowns at slots 62, 82, 86)
Zone(TEXT("Machines: process units, right of the first crown"), 2.6f, 3.3f);
	Scatter(TEXT("SM_Kiln"), 46.f, 48.f, 4, 2, 0.9f, 1.f, 0.f, 1, 5.f);
Zone(TEXT("Machines: process units, left of the second crown"), 3.3f, 3.98f);
	Scatter(TEXT("SM_Kiln"), 46.f, 48.f, 4, 2, 0.9f, 1.f, 0.f, -1, 5.f);
};
FourthPass();
