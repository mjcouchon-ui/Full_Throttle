# Level 1 "Chemical Works": Stage 1 world, integration note (16 Nov 2026)

Branch `claude/world-L1`. Written by the integrator from what was run and seen: one compile, one start of the level.
No review was done and frame time was not measured.

## What was done

- All 11 block files `C:\ftwork\s1\L1_*.inl` were pasted, each wrapped in `{ }`, at the end of
  `Source/FullThrottle/FTPeopleL1.inl`, in this order: s1d1 to s1d8, s1plants, s1animals, s1people. No block was edited. None was left out.
- Before pasting (by script): every mesh name used in every block has its `.uasset` in `Content/World/ChemicalWorks`;
  each of the 11 make logs ends `pieces built N failed 0`; 204 `.uasset` files and 204 `FTPEOPLE piece` lines; no Set name is used
  twice among the blocks; every Set is named `WldS1...`; no `Lit(` or `Bank(` rule in any block.
- Compile (`build.ps1`, tag L1s1_build): "build exit 0 after 91 s", "Result: Succeeded". No compile error; nothing was changed in a block.
- The level was started once (`run_game.ps1`, script `C:\ftwork\jobs\p3\start_L1.txt`, out `C:\ftwork\out\s1\after_L1`): "exit 0 after 92 s", 1 frame saved.

## Before and after

| | FTDRESS level=1 |
|---|---|
| before (`C:\ftwork\out\p3\final_L1\run.log`) | districts=255 sets=21 pieces placed=11878 of 12963 tried shadows=6297 of 12557 |
| after (`C:\ftwork\out\s1\after_L1\run.log`) | districts=309 sets=143 pieces placed=14405 of 15674 tried shadows=7836 of 15084 |

- Stage 1 adds 2,527 placed pieces of 2,711 tried (54 zones, 122 Sets); pieces that cast a shadow 6,297 to 7,836.
- FTCLEAR level=1: props kept 13802 to 16329; moved off their own barrier 806 to 883; left out of the road 1153 to 1337; pillars left out 50 and 50.
- Track: the run log has `FTTRACK level=1 frames=12792 route=09fbdb9f solid=a2d79366 solidTriangles=950948`, equal to the
  `^FTTRACK level=1 ` line of `SourceArt/Jam/validate/track_baseline.txt` (line 10; compared field by field).
- "Fatal error" lines: 0. "LogFT: Error" lines: 0. `/Game/World` load failure: none found; the only `/Game/World` line is the Display
  line "Waiting for static meshes to be ready 0/1 (/Game/World/ChemicalWorks/SM_S1D4_RagStore)".
- One `LogFT: Warning: FT WARM no assets found (asset registry empty?)`; the before log has the same line once.

## Zone plan in short (the plan: `C:\ftwork\final\s1\L1_plan.md`)

Eight districts in road order with no gaps, each led by its own colour: cream and brick / purple and cyan / yellow, red and blue /
magenta and black / white, red and teal / lavender and periwinkle / teal with yellow bands / copper orange.

| builder | where | what | meshes | triangles (sum) | the block's own estimate of placed |
|---|---|---|---|---|---|
| s1d1 | Factory Gate, 0.00 to 2.60 km | the brick company town at the gate; hero THE CLOCK GATE | 19 | 15888 | about 240 |
| s1d2 | Reactor Block, 2.60 to 5.35 km | the research quarter of domes and drums; hero THE GREAT RETORT | 19 | 13100 | about 152 |
| s1d3 | Loading Yards, 5.35 to 8.09 km (its Zones end at 7.26 km) | the freight town of containers, numerals and signal cabins; hero THE GREAT SCALES | 19 | 15616 | about 163 |
| s1d4 | Waste Run, 8.09 to 9.46 km | the crooked sludge works on stilts; hero THE SLUDGE FOUNTAIN | 19 | 19276 | not read (no estimate line found by my search of the block) |
| s1d5 | Export Terminal, 9.46 to 13.40 km | the port: quay warehouses, customs house, silos; hero THE ANCHOR | 17 | 8702 | about 240 |
| s1d6 | Cooling Field, 13.40 to 17.40 km | the spa in the steam: rotunda, winter garden, colonnades; hero THE CASCADE | 16 | 13072 | about 244 |
| s1d7 | Tank Farm B, 17.40 to 21.40 km | the round town made of tanks; hero THE GIANT KETTLE | 16 | 16816 | about 243 |
| s1d8 | Reactor Two, 21.40 to 25.76 km | the copper quarter: battered walls, buttress fins; hero THE HOURGLASS | 17 | 11888 | about 189 |
| s1plants | plants, whole level, one Zone a district | weeds, hedges, topiary, reeds, trees; they sway | 22 | 19308 | about 590 |
| s1animals | animals, whole level | goat, cats, rabbits, carthorse, hens, water birds in their own pools, and more | 19 | 15274 | about 306 |
| s1people | people, whole level (one Zone 0 to 99 km, sites by slot) | people raised on plinths, steps and terraces | 21 | 34976 | about 201 |

204 new meshes, 183,916 triangles together (sum of the make logs' `FTPEOPLE piece` lines). All are under `/Game/World/ChemicalWorks`;
the folder is 9.4 MB, no file over 40 MB. The estimates add up to more than the 2,527 placed; which block fell short was not worked out.

Heroes by triangles: ClockGate 3528, GreatRetort 2608, GreatScales 2628, SludgeFountain 3122, Anchor 940, Cascade 2696, GiantKettle 3152,
Hourglass 1876 (guide 4,000). Builder s1d7 reports SM_S1D7_DomeHouse 1516 against the 1,500 guide. The other guides against the real
placement counts: not checked.

Meshes built but used by no rule of their block (so never placed):
- none

## Every new mesh

Triangles, top and TALL/low are copied from the make logs `C:\ftwork\logs\L1<id>_make.log` (the lines the builders' hand-overs copy).
What each piece is and what moves: the generators' docstrings and the builders' hand-overs.

FTCLEAR column = `left out / moved / placed` in the after run. The game prints a mesh in FTCLEAR ONLY when one of its placements was
left out or moved by the road clearance test (FTTrack.cpp, the loop before the log line). "not listed" = none left out or moved by
that test; its placed count is not in the log: NOT CHECKED. 85 of the 204 meshes are listed: 184 left out, 77 moved, 941 placed.
No mesh of s1d5 or s1d7 is listed, so the log gives no placed count for those two districts.

### s1d1: Factory Gate, 0.00 to 2.60 km

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D1_Terrace | 1640 | 11.21 | TALL | not listed |
| SM_S1D1_Lodge | 756 | 8.14 | TALL | not listed |
| SM_S1D1_Canteen | 1088 | 12.51 | TALL | not listed |
| SM_S1D1_TimeOffice | 972 | 13.87 | TALL | not listed |
| SM_S1D1_FireStation | 1104 | 13.26 | TALL | not listed |
| SM_S1D1_PayOffice | 1080 | 11.1 | TALL | not listed |
| SM_S1D1_Smithy | 880 | 14.7 | TALL | not listed |
| SM_S1D1_Institute | 1056 | 12.29 | TALL | not listed |
| SM_S1D1_ClockGate | 3528 | 52.35 | TALL | not listed |
| SM_S1D1_Lamp | 228 | 5.62 | TALL | not listed |
| SM_S1D1_NoticeBoard | 180 | 4.33 | TALL | not listed |
| SM_S1D1_SentryBox | 316 | 3.7 | TALL | not listed |
| SM_S1D1_Washing | 312 | 3.0 | TALL | not listed |
| SM_S1D1_GatePiers | 436 | 6.64 | TALL | not listed |
| SM_S1D1_Stack | 248 | 18.82 | TALL | not listed |
| SM_S1D1_TimeCards | 384 | 1.93 | low | 0 / 1 / 12 |
| SM_S1D1_BikeRack | 772 | 1.89 | low | 0 / 3 / 11 |
| SM_S1D1_ChurnCart | 680 | 1.25 | low | 0 / 1 / 12 |
| SM_S1D1_Trough | 228 | 1.6 | low | 0 / 1 / 11 |

### s1d2: Reactor Block, 2.60 to 5.35 km

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D2_Institute | 900 | 23.41 | TALL | not listed |
| SM_S1D2_Lecture | 416 | 11.0 | TALL | not listed |
| SM_S1D2_Observatory | 684 | 24.58 | TALL | 1 / 0 / 16 |
| SM_S1D2_Annex | 652 | 14.3 | TALL | not listed |
| SM_S1D2_Igloos | 964 | 5.24 | TALL | not listed |
| SM_S1D2_Hostel | 1236 | 22.01 | TALL | 1 / 0 / 11 |
| SM_S1D2_Refectory | 888 | 15.15 | TALL | not listed |
| SM_S1D2_Lodge | 388 | 5.75 | TALL | not listed |
| SM_S1D2_Dish | 264 | 5.48 | TALL | not listed |
| SM_S1D2_Weather | 476 | 5.27 | TALL | 1 / 1 / 9 |
| SM_S1D2_Footbridge | 508 | 7.25 | TALL | not listed |
| SM_S1D2_PendantLamp | 244 | 4.32 | TALL | not listed |
| SM_S1D2_FlaskSign | 236 | 3.37 | TALL | 1 / 0 / 12 |
| SM_S1D2_FlaskRack | 680 | 1.86 | low | 0 / 1 / 4 |
| SM_S1D2_Cylinders | 828 | 1.8 | low | not listed |
| SM_S1D2_Molecule | 344 | 1.91 | low | not listed |
| SM_S1D2_Blackboard | 436 | 1.95 | low | not listed |
| SM_S1D2_VentCaps | 348 | 1.92 | low | not listed |
| SM_S1D2_GreatRetort | 2608 | 61.75 | TALL | not listed |

### s1d3: Loading Yards, 5.35 to 8.09 km (its Zones end at 7.26 km)

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D3_ContainerHotel | 1708 | 10.22 | TALL | not listed |
| SM_S1D3_SignalCabin | 956 | 14.02 | TALL | not listed |
| SM_S1D3_YardOffice | 820 | 18.2 | TALL | 1 / 0 / 6 |
| SM_S1D3_WagonCafe | 1060 | 6.34 | TALL | 1 / 1 / 7 |
| SM_S1D3_WeighHouse | 776 | 6.2 | TALL | 2 / 0 / 6 |
| SM_S1D3_CustomsShed | 988 | 9.74 | TALL | not listed |
| SM_S1D3_EngineShed | 1032 | 14.66 | TALL | not listed |
| SM_S1D3_BoxTower | 1028 | 15.12 | TALL | not listed |
| SM_S1D3_PalletStack | 288 | 1.85 | low | 0 / 6 / 29 |
| SM_S1D3_SackTrucks | 660 | 1.52 | low | 2 / 3 / 27 |
| SM_S1D3_LoadingGauge | 348 | 5.88 | TALL | 1 / 1 / 3 |
| SM_S1D3_WaterCrane | 332 | 4.96 | TALL | 0 / 3 / 4 |
| SM_S1D3_BufferStop | 364 | 1.85 | low | not listed |
| SM_S1D3_ForkliftBay | 716 | 3.09 | TALL | 1 / 0 / 2 |
| SM_S1D3_NumeralBoard | 308 | 6.35 | TALL | 0 / 1 / 3 |
| SM_S1D3_Semaphore | 424 | 8.95 | TALL | 0 / 2 / 4 |
| SM_S1D3_TallyTower | 540 | 7.76 | TALL | not listed |
| SM_S1D3_DrumDock | 640 | 1.8 | low | not listed |
| SM_S1D3_GreatScales | 2628 | 58.1 | TALL | not listed |

### s1d4: Waste Run, 8.09 to 9.46 km

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D4_SkimmersHut | 1420 | 9.02 | TALL | 1 / 1 / 11 |
| SM_S1D4_FilterHouse | 1918 | 16.9 | TALL | not listed |
| SM_S1D4_DrumWash | 1626 | 6.87 | TALL | 0 / 3 / 9 |
| SM_S1D4_RagStore | 1320 | 10.28 | TALL | not listed |
| SM_S1D4_AssayHut | 1318 | 8.94 | TALL | 1 / 1 / 11 |
| SM_S1D4_OldPumpHouse | 1012 | 5.23 | TALL | 1 / 0 / 6 |
| SM_S1D4_StiltRow | 1936 | 7.83 | TALL | 1 / 1 / 8 |
| SM_S1D4_BoilHouse | 1920 | 17.0 | TALL | not listed |
| SM_S1D4_Troughs | 380 | 1.32 | low | 2 / 8 / 13 |
| SM_S1D4_Duckboards | 484 | 1.52 | low | 0 / 9 / 15 |
| SM_S1D4_Punt | 292 | 1.07 | low | 0 / 7 / 12 |
| SM_S1D4_SludgePump | 396 | 1.52 | low | 1 / 4 / 5 |
| SM_S1D4_DripBarrels | 372 | 1.1 | low | 1 / 1 / 5 |
| SM_S1D4_SignPost | 220 | 4.48 | TALL | 0 / 2 / 15 |
| SM_S1D4_Scarecrow | 276 | 3.11 | TALL | 1 / 2 / 13 |
| SM_S1D4_StovePipes | 500 | 6.11 | TALL | 0 / 1 / 8 |
| SM_S1D4_HopperStand | 412 | 5.65 | TALL | 2 / 0 / 9 |
| SM_S1D4_Lamp | 352 | 4.46 | TALL | not listed |
| SM_S1D4_SludgeFountain | 3122 | 52.79 | TALL | not listed |

### s1d5: Export Terminal, 9.46 to 13.40 km

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D5_QuayWarehouse | 724 | 27.75 | TALL | not listed |
| SM_S1D5_SteppedRow | 660 | 21.04 | TALL | not listed |
| SM_S1D5_CustomsHouse | 732 | 22.96 | TALL | not listed |
| SM_S1D5_HarbourOffice | 728 | 19.76 | TALL | not listed |
| SM_S1D5_TwinSilos | 444 | 24.4 | TALL | not listed |
| SM_S1D5_SeamensMission | 852 | 15.2 | TALL | not listed |
| SM_S1D5_Chandlery | 684 | 16.02 | TALL | not listed |
| SM_S1D5_BollardRow | 384 | 1.02 | low | not listed |
| SM_S1D5_FenderStack | 456 | 1.22 | low | not listed |
| SM_S1D5_BoatTrestles | 252 | 1.53 | low | not listed |
| SM_S1D5_CrabPots | 314 | 1.87 | low | not listed |
| SM_S1D5_KedgeAnchor | 292 | 1.88 | low | not listed |
| SM_S1D5_QuayLamp | 168 | 9.06 | TALL | not listed |
| SM_S1D5_SignalMast | 288 | 11.79 | TALL | not listed |
| SM_S1D5_Gangway | 440 | 3.56 | TALL | not listed |
| SM_S1D5_NetRack | 344 | 3.67 | TALL | not listed |
| SM_S1D5_Anchor | 940 | 60.0 | TALL | not listed |

### s1d6: Cooling Field, 13.40 to 17.40 km

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D6_BathHouse | 1528 | 11.9 | TALL | 2 / 0 / 7 |
| SM_S1D6_WinterGarden | 1028 | 10.25 | TALL | 3 / 0 / 6 |
| SM_S1D6_PumpRoom | 708 | 7.62 | TALL | 2 / 0 / 10 |
| SM_S1D6_Colonnade | 900 | 6.3 | TALL | 2 / 0 / 6 |
| SM_S1D6_TeaPavilion | 972 | 9.22 | TALL | 4 / 0 / 4 |
| SM_S1D6_CabinTerrace | 824 | 4.29 | TALL | 3 / 0 / 5 |
| SM_S1D6_Hydro | 1092 | 11.98 | TALL | 2 / 0 / 7 |
| SM_S1D6_PlungePool | 320 | 1.82 | low | 6 / 0 / 20 |
| SM_S1D6_DeckChairs | 428 | 1.9 | low | 4 / 0 / 9 |
| SM_S1D6_Urn | 204 | 1.87 | low | 4 / 1 / 17 |
| SM_S1D6_Fountain | 388 | 1.77 | low | 3 / 0 / 18 |
| SM_S1D6_GlobeLamp | 496 | 5.4 | TALL | 7 / 0 / 11 |
| SM_S1D6_TowelCart | 440 | 1.88 | low | 5 / 1 / 13 |
| SM_S1D6_VaneKiosk | 544 | 4.71 | TALL | 6 / 0 / 17 |
| SM_S1D6_ChessTable | 504 | 1.18 | low | 4 / 0 / 19 |
| SM_S1D6_Cascade | 2696 | 89.2 | TALL | not listed |

### s1d7: Tank Farm B, 17.40 to 21.40 km

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D7_CylHouse | 1356 | 15.24 | TALL | not listed |
| SM_S1D7_SphereCabin | 1092 | 10.17 | TALL | not listed |
| SM_S1D7_GaugeHouse | 1360 | 13.52 | TALL | not listed |
| SM_S1D7_WaterTowerHouse | 1108 | 16.7 | TALL | not listed |
| SM_S1D7_DomeHouse | 1516 | 18.66 | TALL | not listed |
| SM_S1D7_DrumCottage | 888 | 8.55 | TALL | not listed |
| SM_S1D7_Inn | 1492 | 15.54 | TALL | not listed |
| SM_S1D7_TankHall | 1256 | 9.39 | TALL | not listed |
| SM_S1D7_Dipsticks | 772 | 6.74 | TALL | not listed |
| SM_S1D7_LidTable | 592 | 1.0 | low | not listed |
| SM_S1D7_ElbowStack | 592 | 1.51 | low | not listed |
| SM_S1D7_WashLine | 360 | 1.85 | low | not listed |
| SM_S1D7_ValveWheel | 508 | 1.86 | low | not listed |
| SM_S1D7_RainButt | 400 | 1.94 | low | not listed |
| SM_S1D7_PipeLamp | 372 | 5.07 | TALL | not listed |
| SM_S1D7_GiantKettle | 3152 | 60.8 | TALL | not listed |

### s1d8: Reactor Two, 21.40 to 25.76 km

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D8_PowerHall | 1704 | 26.8 | TALL | 2 / 0 / 1 |
| SM_S1D8_Foundry | 1108 | 32.1 | TALL | 1 / 0 / 2 |
| SM_S1D8_SwitchHouse | 532 | 17.48 | TALL | 1 / 0 / 3 |
| SM_S1D8_Club | 1444 | 23.5 | TALL | 1 / 0 / 6 |
| SM_S1D8_SmithsRow | 1136 | 23.44 | TALL | 2 / 0 / 2 |
| SM_S1D8_GateBastion | 1084 | 29.0 | TALL | not listed |
| SM_S1D8_QuenchTower | 560 | 39.1 | TALL | 1 / 0 / 3 |
| SM_S1D8_IngotStack | 252 | 3.69 | TALL | 3 / 0 / 5 |
| SM_S1D8_LadleCar | 372 | 1.78 | low | 1 / 0 / 9 |
| SM_S1D8_RivetForge | 308 | 5.83 | TALL | 3 / 0 / 3 |
| SM_S1D8_AnvilRack | 328 | 1.88 | low | 2 / 0 / 6 |
| SM_S1D8_BollardChain | 296 | 1.08 | low | 3 / 0 / 11 |
| SM_S1D8_VentCowl | 152 | 6.9 | TALL | 1 / 0 / 10 |
| SM_S1D8_FoundryLamp | 184 | 6.22 | TALL | 2 / 0 / 16 |
| SM_S1D8_SignPylon | 240 | 4.71 | TALL | 4 / 0 / 10 |
| SM_S1D8_TubeRack | 312 | 4.19 | TALL | 1 / 0 / 12 |
| SM_S1D8_Hourglass | 1876 | 70.0 | TALL | not listed |

### s1plants: plants, whole level, one Zone a district

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1PLANTS_CobbleWeeds | 924 | 1.06 | low | not listed |
| SM_S1PLANTS_PrivetHedge | 516 | 1.81 | low | 0 / 5 / 16 |
| SM_S1PLANTS_TopiaryTubs | 740 | 1.8 | low | 4 / 0 / 17 |
| SM_S1PLANTS_Thistles | 980 | 1.87 | low | 0 / 1 / 10 |
| SM_S1PLANTS_Fungi | 932 | 1.44 | low | not listed |
| SM_S1PLANTS_ReedBed | 784 | 1.8 | low | 2 / 1 / 41 |
| SM_S1PLANTS_Allotment | 1052 | 1.87 | low | 0 / 3 / 28 |
| SM_S1PLANTS_Agaves | 1088 | 1.82 | low | 7 / 0 / 21 |
| SM_S1PLANTS_PlaneTree | 656 | 14.12 | TALL | not listed |
| SM_S1PLANTS_Poplars | 756 | 18.0 | TALL | 3 / 0 / 36 |
| SM_S1PLANTS_Buddleia | 804 | 2.89 | TALL | not listed |
| SM_S1PLANTS_DeadTree | 436 | 9.06 | TALL | not listed |
| SM_S1PLANTS_ReedsTall | 1276 | 2.88 | TALL | 3 / 0 / 13 |
| SM_S1PLANTS_Willow | 682 | 11.95 | TALL | 8 / 0 / 18 |
| SM_S1PLANTS_TreeFerns | 1316 | 4.63 | TALL | 7 / 0 / 19 |
| SM_S1PLANTS_Cypress | 588 | 9.9 | TALL | 5 / 0 / 38 |
| SM_S1PLANTS_MopTrees | 776 | 5.27 | TALL | not listed |
| SM_S1PLANTS_Tamarisk | 1054 | 2.74 | TALL | not listed |
| SM_S1PLANTS_WindPine | 640 | 8.06 | TALL | not listed |
| SM_S1PLANTS_Pergola | 1056 | 3.4 | TALL | not listed |
| SM_S1PLANTS_PollardLimes | 972 | 6.16 | TALL | not listed |
| SM_S1PLANTS_Sunflowers | 1280 | 3.39 | TALL | not listed |

### s1animals: animals, whole level

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1ANIMALS_GateGoat | 848 | 1.32 | low | not listed |
| SM_S1ANIMALS_WallCats | 720 | 2.56 | TALL | not listed |
| SM_S1ANIMALS_LabRabbits | 1188 | 1.47 | low | not listed |
| SM_S1ANIMALS_Carthorse | 736 | 2.35 | TALL | not listed |
| SM_S1ANIMALS_PipeRats | 920 | 2.0 | low | not listed |
| SM_S1ANIMALS_HenRun | 1456 | 1.97 | low | not listed |
| SM_S1ANIMALS_Greyhounds | 584 | 1.99 | low | 2 / 0 / 9 |
| SM_S1ANIMALS_Salamanders | 872 | 4.35 | TALL | 7 / 0 / 13 |
| SM_S1ANIMALS_PelicanPiles | 788 | 4.06 | TALL | not listed |
| SM_S1ANIMALS_Flamingos | 996 | 2.3 | TALL | not listed |
| SM_S1ANIMALS_Swans | 960 | 2.7 | TALL | 8 / 0 / 12 |
| SM_S1ANIMALS_KoiPool | 920 | 0.68 | low | 3 / 0 / 17 |
| SM_S1ANIMALS_SealSlip | 748 | 1.82 | low | not listed |
| SM_S1ANIMALS_LagoonFrogs | 1048 | 1.95 | low | not listed |
| SM_S1ANIMALS_WhiteOwl | 94 | 0.51 | low | not listed |
| SM_S1ANIMALS_GullWheel | 290 | 3.63 | TALL | not listed |
| SM_S1ANIMALS_StorkStack | 620 | 19.54 | TALL | 1 / 0 / 14 |
| SM_S1ANIMALS_Doves | 1030 | 8.86 | TALL | not listed |
| SM_S1ANIMALS_LampMoths | 456 | 15.3 | TALL | not listed |

### s1people: people, whole level (one Zone 0 to 99 km, sites by slot)

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1PEOPLE_Doorstep | 1560 | 8.76 | TALL | not listed |
| SM_S1PEOPLE_ClockOn | 1676 | 6.85 | TALL | not listed |
| SM_S1PEOPLE_HoseTower | 2096 | 18.06 | TALL | not listed |
| SM_S1PEOPLE_Blackboard | 1556 | 6.4 | TALL | not listed |
| SM_S1PEOPLE_FlaskWalk | 1492 | 6.5 | TALL | not listed |
| SM_S1PEOPLE_Observatory | 1652 | 15.7 | TALL | not listed |
| SM_S1PEOPLE_CafeHatch | 1776 | 7.4 | TALL | not listed |
| SM_S1PEOPLE_TallyDock | 1820 | 6.76 | TALL | not listed |
| SM_S1PEOPLE_SignalCabin | 1676 | 16.4 | TALL | not listed |
| SM_S1PEOPLE_SkimJetty | 1724 | 6.5 | TALL | not listed |
| SM_S1PEOPLE_DrumWash | 1776 | 8.87 | TALL | not listed |
| SM_S1PEOPLE_AssayTower | 1800 | 16.96 | TALL | not listed |
| SM_S1PEOPLE_MissionSteps | 1708 | 7.08 | TALL | not listed |
| SM_S1PEOPLE_CustomsDesk | 1652 | 6.6 | TALL | not listed |
| SM_S1PEOPLE_HoistLoft | 1292 | 16.8 | TALL | not listed |
| SM_S1PEOPLE_ChessTerrace | 1772 | 6.2 | TALL | 2 / 0 / 5 |
| SM_S1PEOPLE_PlungeRim | 1536 | 5.76 | TALL | 2 / 0 / 5 |
| SM_S1PEOPLE_Belvedere | 1796 | 16.54 | TALL | 2 / 0 / 2 |
| SM_S1PEOPLE_ClubTerrace | 1512 | 7.18 | TALL | 4 / 0 / 4 |
| SM_S1PEOPLE_ForgeStep | 1524 | 9.0 | TALL | 4 / 0 / 4 |
| SM_S1PEOPLE_ChoirBastion | 1580 | 14.77 | TALL | 2 / 0 / 2 |

## Left out

- Blocks left out: none. Meshes missing: none. Builders that died: none seen (11 of 11 make logs end "failed 0").
- Left out by the track (FTCLEAR, new meshes only): 184 placements of 85 listed meshes.
- No listed mesh has 0 placed.
- Most left out of one new mesh: SM_S1PLANTS_Willow 8, SM_S1ANIMALS_Swans 8, SM_S1D6_GlobeLamp 7, SM_S1PLANTS_TreeFerns 7, SM_S1PLANTS_Agaves 7, SM_S1ANIMALS_Salamanders 7.

## Not checked

- The one saved frame (`C:\ftwork\out\s1\after_L1\L1_t006.png`, 6 s into the level) was opened: the level draws, the road is clear, both roadsides are full. Which pieces in it are Stage 1 was not worked out; nothing else was looked at in play.
- Placed counts of the meshes "not listed" above, and so the placed count of each block; whether each hero stands where the plan says.
- The rules that protect play (17 m / 24 m distances, steady light, nothing like a pickup, an enemy or the boss): no review was done.
- Frame time, memory, shadows on screen, culling distances, movement in the game.
- The doubled circuit (14.02 km on and 22.18 to 25.58 km): whether a piece of one lap stands in the other lap's view.
- Packaged build, the other seven levels, the generators run a second time.

## Review and fix round (16 Nov 2026)

Two reviewers looked at Stage 1 (a rules reader working from the tables and the generators' geometry, and a driver with 8 game
runs). One fix round followed. Written from what was run and seen in that round; where this section and the sections above differ
(where the heroes stand, the counts), this section is the later one.

### Found

- Rule broken (driver): each end of SM_S1D2_Footbridge was a lit cyan disc with a dark centre, 5.75 m up, end-on to the driver at
  38.3 m: a lit ring facing the car (seen at 3.40 km). Verified in ft_world_chem_s1d2.py before changing.
- Borderline (reader): SM_S1D2_Observatory, a stalk with a lit cyan ring, a lit violet ring and a white ball on top, its ring 39.84 m
  out in slot 70: the boss's outline inside 40 m.
- Looks wrong: three heroes never seen from the road (Great Retort, Great Scales, both Cascades: behind or inside a loop), the second
  Clock Gate inside a pipe, the Anchor under the HUD panel and behind old stands; d6 "Cooling Field" has no identity (its lead colour
  is the steam's colour); the brick street of d1 stood in the old "yard dogs" in 7 slot-sides; BathHouse, Hydro, Club and Colonnade
  ran into older pass 3 pieces; the low row of plants stood in front of the older low line of people.
- Weak: animals of lap two in the band of pass 3's back line; one drop (1.2 m) for every Far piece of d2 and d3; masts and nets in
  the flare avenue; the fountain a black funnel on a night sky; d7 reads as more tanks; d1 thin after 1.1 km; d3 thin after 6.4 km;
  d4 seen for 150 m; triangle and count guides.
- By the reader's numbers no distance, light or movement rule was broken on a piece's own lap.

### Fixed (what changed)

Generators (six meshes regenerated with one run_job, tag L1s1fix_make: "pieces built 2 / 1 / 3, failed 0"; no size or top changed):
- SM_S1D2_Footbridge: the end caps are plain white, unlit, no dark centre (444 triangles, was 508).
- SM_S1D2_Observatory: the ring, the band above it and the five portholes are unlit (the lens tip on the dome keeps its light).
- SM_S1D6_BathHouse, SM_S1D6_Hydro, SM_S1D6_Colonnade: the dome, roofs, cornices, entablature and the colonnade's back wall are the
  plan's purple #6a5ab0 where they were lilac. Colour only.
- SM_S1D4_SludgeFountain: eight unlit pink bands down the road face of the funnel (3218 triangles, was 3122); saved `far=True`
  (its falls keep moving past 180 m).

Table (Stage 1 blocks of FTPeopleL1.inl only; every changed line carries "fix round"):
- Heroes moved to open road and placed by Far rules (placed whatever the road does; the drop is the land's height there + 10 cm):
  Great Retort slot 74 to slot 64 (2.895 km), right, 150 m; Great Scales slot 136 right to slot 132 (5.97 km), LEFT, 132 m (the loop
  leans right; slot 132 lies between the gull wheels of slots 128 and 134); Cascade slots 340 and 368 to slot 316 (14.30 km, 150 m)
  and slot 348 (15.74 km, 155 m), right; second Clock Gate slot 40 to slot 48 (2.17 km), right, 122 m; Anchor to the LEFT of slot 222.
- d1 and the yard dogs: left slots 10, 18, 26 (canteen) and 30 (yard) left out; left slot 46 (terrace, lodge) and right slots 32, 48
  (canteen, time office) moved to 64 m. A first try at 62 m on the left put the canteen into pass 3's pipe bridge at 63 m: left out.
- d6: BathHouse left 63.5 to 67 m; Hydro (s % 12 == 8) 63.5 to 65.5 m; the colonnade walk taken out of slots 336 and 348.
- d8: the lap club on the left 2.5 m further out (club 71.5 m).
- d2, d3: the Far macros take the land's own drop: 0.692 + 0.00676 x m (0.87 at 26 m, 0.95 at 38.5, 1.02 at 49, 1.13 at 65).
- d5: nothing of Stage 1 in the flare avenue but the three unlit building rows 60 m out (8 mast-and-net strips and one kerb set out).
- Plants: the five low plants 1.8 to 1.9 m tall (Allotment, ReedBed, PrivetHedge, TopiaryTubs, Thistles) at chance 0.5 (22 rules).
- Animals, lap two: Greyhounds and Salamanders 56 to 64 m moved to 67 to 69.5 m.

### After the fix round (one compile, one start of the level)

- Compile (tag L1s1_fix_build): "build exit 0 after 51 s", "Result: Succeeded".
- Level started once (`C:\ftwork\out\s1\fixed_L1`): "exit 0 after 39 s", 1 frame; the frame was opened: the level draws, the road is clear.
- Track: `FTTRACK level=1 frames=12792 route=09fbdb9f solid=a2d79366 solidTriangles=950948`, equal to the `^FTTRACK level=1 ` line of
  `SourceArt/Jam/validate/track_baseline.txt` (compared with diff by me).
- `FTDRESS level=1 districts=309 sets=143 pieces placed=14312 of 15570 tried shadows=7792 of 14991` (before the round: 14405 of 15674,
  shadows 7836 of 15084). FTCLEAR: props kept 16236, moved off their own barrier 880, left out of the road 1326, pillars left out 50.
- "Fatal error" 0, "LogFT: Error" 0; one "FT WARM no assets found" warning as before.
- No hero is in the FTCLEAR list (ClockGate, GreatRetort, GreatScales, Anchor, Cascade, SludgeFountain), so none was left out or moved
  by the clearance test; a Far rule has no other way to be left out in FTTrack.cpp (read). That is an inference from the code and the
  log, not a count of instances.
- The reader's scripts were run again on the changed table and generators (copies in `C:\ftwork\jobs\L1s1fix\rev`, output
  `C:\ftwork\out\L1s1fix`): 741 Stage 1 rules, 3,521 potential placements; nearest geometry 22.85 m (ReedBed), nearest part over 2 m
  27.46 m and nearest light 27.6 m (QuayLamp), nearest lit moving part 36.87 m: the same as before the round; no rule flagged.

### Not fixed, and why

- d7 "Tank Farm B" still reads as more tanks; d1 is thinner still after 1.1 km (4 slot-sides left out); d3 after 6.4 km; d4's short
  window: each needs new pieces or a new street, which is a second round of invention.
- d6's identity: only the colour was changed and the first Cascade put on the open stretch; no new street was laid.
- Reader's 3, last point (BathHouse / WinterGarden in the band of the rail-cart viaduct's piers): not changed; BathHouse at 67 m still
  shows a possible box overlap with SM_P3C_RailCart in slots 338 and 374.
- Possible box overlaps that remain where pieces were moved (boxes, older sizes partly unknown): d1's lamp, notice board and washing
  at 55 m with the yard dogs (to 57.4 m) in slots 32, 46, 48; the terrace at 64 m with WatchTankFar (70 to 110 m) in slot 46; the
  Great Retort with the old scattered pipe racks and towers (as at its old slot).
- Animals at 40 to 45 m against watchers and film units (about 20 slot-sides): not worked out, not changed.
- SM_S1D8_Club now stands on both sides of 70 m (68 to 71.5 m): the mesh casts shadows at every placement. SM_S1D1_ClockGate is not
  saved far=True (it was not seen frozen). Triangle guides, counts a district, the depot lots outside 70 m, Far rules without
  shadows in d2, d3 and d4, the boss-vocabulary shapes over 40 m out or unlit (Kettle knob, GlobeLamp, Molecule, ValveWheel): left.
- The driver's item 9 (distance field builds on first run, the car's body missing in frames, frame rate, pillars under jump 2) is
  not Stage 1's work: not touched.
- The driver's contact sheet (`C:\ftwork\out\L1s1rev\L1_s1_review.jpg`) is 1,179,367 bytes, over 1 MB: not copied into the repository.

### Not checked

- Nothing was driven after the fix round: whether any moved hero is now SEEN from the road, how the purple d6 pieces, the pink bands
  and the plain footbridge look in play, the Far heroes' footing on the land (drop worked out from GroundBands, not seen).
- Which side each loop leans to (taken from the driver's frames for the Opening's loops; for the circuit's loops not known), and
  whether slot 64 (inside wall turn 1) shows the Retort from the bend.
- Instance counts of any mesh not in the FTCLEAR list; the placed count of each block.
- The other lap: whether a low Stage 1 piece stands 15.6 to 17 m from the other lap's centre line (the track guarantees 15.61 m).
- Overlaps with the dressing's pieces whose sizes are in no log; triangles against real placement counts.
- Frame time, memory, shadows on screen, culling, movement on screen, the packaged build, the generators run in full a second time.
