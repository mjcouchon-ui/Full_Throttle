# Level 2 "Nevada": Stage 1 world, integration note (16 Nov 2026)

Branch `claude/world-L2`. Written by the integrator from what was run and seen; mesh figures are copied from the builders' make logs
(`C:\ftwork\logs\L2<id>_make.log`, line `FTPEOPLE piece ...`), counts from the one game start (`C:\ftwork\out\s1\after_L2\run.log`).

## Zone plan in short

Eight districts end to end (plan: `C:\ftwork\final\s1\L2_plan.md`), district builders on even slots (s % 4 == 0 or 2), plants on s % 4 == 3, people on s % 8 == 1 (as the block headers say).

| id | what | meshes | triangles (sum) | block lines | Set / Zone / Site / Scatter / Far words in the block |
|---|---|---|---|---|---|
| s1d1 | district 1 "State Line and the Dry Wash", 0.00 to 2.43 km: border railhead and ranch town, whitewash + red roofs; hero the Line Rider windpump | 20 | 20304 | 200 | 13 / 24 / 74 / 0 / 0 |
| s1d2 | district 2 "Snake Eyes Country and Joshua Flats", 2.43 to 5.14 km: scrap and tin houses, sage green + purple | 17 | 21368 | 232 | 36 / 1 / 1 / 0 / 0 |
| s1d3 | district 3 "Red Rock Pass and Last Chance Gas", 5.14 to 7.75 km: painted adobe, turquoise + yellow; hero the Big Pump | 19 | 22468 | 141 | 8 / 19 / 33 / 6 / 0 |
| s1d4 | district 4 "Jackpot Junction and the City Lights", 7.75 to 10.69 km: roadside modern, deep violet + neon; hero the Lucky Horseshoe | 16 | 14948 | 338 | 23 / 33 / 203 / 0 / 0 |
| s1d5 | district 5 "Salt Flats", 10.69 to 14.50 km: speed-week camp and salt works, silver + red stripes | 18 | 16500 | 62 | 11 / 3 / 14 / 0 / 0 |
| s1d6 | district 6 "Syndicate Range", 14.50 to 18.50 km: black blockhouses, cyan strips, pale domes; hero the Big Ear | 18 | 10024 | 85 | 11 / 3 / 16 / 3 / 0 |
| s1d7 | district 7 "Red Rock Return", 18.50 to 22.00 km: mine camp, teal frames + bleached lilac boards; hero the Twin-Wheel Headframe | 18 | 16844 | 125 | 9 / 1 / 1 / 0 / 0 |
| s1d8 | district 8 "Vegas Approach", 22.00 to 25.60 km: grand hotels, white + gold + crimson; hero the Crown Tower | 16 | 17100 | 253 | 18 / 39 / 170 / 0 / 0 |
| s1plants | the planting of all eight districts (slots s % 4 == 3) | 21 | 16094 | 175 | 0 / 26 / 0 / 135 / 0 |
| s1animals | the animals of all eight districts | 19 | 16738 | 110 | 0 / 29 / 0 / 58 / 7 |
| s1people | people in the gaps pass 3 left (slots s % 8 == 1), each group on what it stands on | 23 | 38312 | 337 | 27 / 54 / 54 / 0 / 0 |

The word counts are text counts of the block files (comments included; a `Site(` inside a loop counts once); they are not placed counts.

## What was integrated

- All 11 blocks pasted, each wrapped in `{ }`, in the order above, at the END of `Source/FullThrottle/FTPeopleL2.inl` (2,091 lines added; nothing above them changed). `FTDressL2.inl` untouched.
- Before pasting, for every block: generator file present, make log ends "pieces built N failed 0" with 0 `LogPython: Error` lines, and every mesh name used in the block has its `.uasset` in `Content/World/Nevada`. None missing. **Left out: nothing.**
- Compile (`build.ps1`, tag L2s1_build): exit 0 after 85 s, "Result: Succeeded", first try; no block was edited.
- 205 new meshes, 210700 triangles in all (one of each). `Content/World` is 11 MB on disk; no file over 40 MB.

## The one game start (start_L2.txt, 6 s of driving, exit 0 after 148 s)

| | before (`C:\ftwork\out\p3\final_L2\run.log`) | after |
|---|---|---|
| FTTRACK level=2 | frames=12888 route=efc95392 solid=e2d27537 solidTriangles=964188 | the same, and equal to the `^FTTRACK level=2 ` line of `SourceArt/Jam/validate/track_baseline.txt` (compared field by field) |
| FTDRESS districts / sets | 2516 / 157 | 2885 / 313 |
| FTDRESS pieces placed of tried | 11589 of 11619 | 14707 of 14737 (+3118 placed, +3118 tried) |
| FTDRESS shadows | 8461 of 12565 | 10936 of 15683 |
| FTCLEAR props kept / moved off their barrier / left out of the road | 14653 / 170 / 30 | 17771 / 189 / 30 |
| FTBANK | not compared | pieces laid=127 on 79 lengths |

- "Fatal error" lines: 0. "LogFT: Error" lines: 0. Lines naming `/Game/World` with fail/error/warning/missing: 0. The only "Failed to load" lines are four engine start-up lines for profiler DLLs (aqProf, VtuneApi, VtuneApi32e, WinPixGpuCapturer); the only two "Error:" lines are the editor's own listener on port 8000.
- FTCLEAR lists a mesh only when at least one of its pieces was left out or moved. Of the new meshes it lists 8 (left out / moved / placed): SM_S1D2_MailSnake 0/1/7, SM_S1D4_KidneyPlanter 0/4/19, SM_S1D4_RopePodium 0/3/19, SM_S1D7_Windlass 0/2/9, SM_S1D7_SluiceBox 0/1/21, SM_S1D7_TramTower 0/5/21, SM_S1PLANTS_Barrel 0/1/14, SM_S1PLANTS_BloomHedge 0/2/40. No new mesh has a piece left out; the level's "left out of the road" stayed 30.
- For the other 197 new meshes the log gives no per-mesh placed count: **not checked** per mesh. The total is the FTDRESS difference, +3118.
- 7 of the 205 new mesh names appear in the log's "Waiting for static meshes to be ready" lines (the engine prints that only for a mesh still compiling when asked for; it is not a list of what loaded).
- One screenshot, `C:\ftwork\out\s1\after_L2\L2_t006.png` (6 s in, state line): the road, the car, the HUD, and buildings, balloons and plants on both sides are drawn; which of them are Stage 1 pieces could not be told from that frame.

## Every new mesh (from the make logs)

### s1d1: district 1 "State Line and the Dry Wash", 0.00 to 2.43 km: border railhead and ranch town, whitewash + red roofs; hero the Line Rider windpump

Generator `Content/Python/ft_world_nevada_s1d1.py`; meshes `/Game/World/Nevada/SM_S1D1_*`.

| mesh | triangles | size m (along x deep) | top m | TALL/low | FTCLEAR left out / moved / placed |
|---|---|---|---|---|---|
| SM_S1D1_Border | 1504 | 14.9 x 7.6 | 8.45 | TALL | not listed |
| SM_S1D1_Depot | 1616 | 17.2 x 11.3 | 9.31 | TALL | not listed |
| SM_S1D1_Store | 1604 | 12.0 x 9.2 | 10.13 | TALL | not listed |
| SM_S1D1_Barn | 1688 | 14.5 x 14.5 | 12.7 | TALL | not listed |
| SM_S1D1_School | 1356 | 10.2 x 10.2 | 9.28 | TALL | not listed |
| SM_S1D1_Hotel | 1724 | 12.4 x 9.3 | 11.16 | TALL | not listed |
| SM_S1D1_Livery | 1632 | 12.7 x 8.9 | 8.29 | TALL | not listed |
| SM_S1D1_RimCabin | 1274 | 10.0 x 12.7 | 26.88 | TALL | not listed |
| SM_S1D1_RimTank | 1408 | 11.6 x 9.4 | 29.56 | TALL | not listed |
| SM_S1D1_HitchRail | 288 | 7.0 x 0.5 | 1.43 | low | not listed |
| SM_S1D1_Trough | 180 | 4.7 x 1.6 | 1.8 | low | not listed |
| SM_S1D1_HayStack | 216 | 3.7 x 0.8 | 1.87 | low | not listed |
| SM_S1D1_Corral | 396 | 7.2 x 0.9 | 1.3 | low | not listed |
| SM_S1D1_Semaphore | 340 | 4.4 x 1.0 | 9.68 | TALL | not listed |
| SM_S1D1_Wagon | 868 | 5.5 x 1.9 | 2.72 | TALL | not listed |
| SM_S1D1_Marker | 320 | 6.7 x 3.0 | 7.38 | TALL | not listed |
| SM_S1D1_WashLine | 280 | 5.3 x 1.8 | 2.6 | TALL | not listed |
| SM_S1D1_YardPump | 596 | 4.8 x 4.6 | 8.6 | TALL | not listed |
| SM_S1D1_LampSign | 286 | 2.6 x 1.0 | 5.24 | TALL | not listed |
| SM_S1D1_LineRider | 2728 | 24.7 x 21.5 | 42.0 | TALL | not listed |

### s1d2: district 2 "Snake Eyes Country and Joshua Flats", 2.43 to 5.14 km: scrap and tin houses, sage green + purple

Generator `Content/Python/ft_world_nevada_s1d2.py`; meshes `/Game/World/Nevada/SM_S1D2_*`.

| mesh | triangles | size m (along x deep) | top m | TALL/low | FTCLEAR left out / moved / placed |
|---|---|---|---|---|---|
| SM_S1D2_QuonsetGarage | 1368 | 15.8 x 10.0 | 8.54 | TALL | not listed |
| SM_S1D2_CrusherShed | 1504 | 15.9 x 7.9 | 10.25 | TALL | not listed |
| SM_S1D2_GrainBinBar | 1444 | 13.6 x 13.1 | 10.28 | TALL | not listed |
| SM_S1D2_TyreWorkshop | 2340 | 12.9 x 7.9 | 8.02 | TALL | not listed |
| SM_S1D2_LinemanHouse | 1876 | 13.8 x 6.7 | 8.84 | TALL | not listed |
| SM_S1D2_TankHouse | 1600 | 12.0 x 10.7 | 12.48 | TALL | not listed |
| SM_S1D2_CarWallHouse | 2008 | 12.1 x 7.7 | 9.94 | TALL | not listed |
| SM_S1D2_HubcapFence | 556 | 8.3 x 0.2 | 1.33 | low | not listed |
| SM_S1D2_DrumPlanters | 632 | 5.5 x 1.1 | 1.52 | low | not listed |
| SM_S1D2_MailSnake | 540 | 7.9 x 0.6 | 1.82 | low | 0 / 1 / 7 |
| SM_S1D2_CarStack | 360 | 6.2 x 2.6 | 2.94 | TALL | not listed |
| SM_S1D2_Whirligig | 568 | 4.2 x 2.2 | 8.06 | TALL | not listed |
| SM_S1D2_TyreTower | 1004 | 2.8 x 2.0 | 5.62 | TALL | not listed |
| SM_S1D2_WashLine | 412 | 7.0 x 2.0 | 3.0 | TALL | not listed |
| SM_S1D2_ScrapSign | 536 | 4.7 x 0.9 | 6.04 | TALL | not listed |
| SM_S1D2_InsulatorStack | 1568 | 4.7 x 2.1 | 8.82 | TALL | not listed |
| SM_S1D2_TinRattler | 3052 | 30.0 x 32.7 | 25.65 | TALL | not listed |

### s1d3: district 3 "Red Rock Pass and Last Chance Gas", 5.14 to 7.75 km: painted adobe, turquoise + yellow; hero the Big Pump

Generator `Content/Python/ft_world_nevada_s1d3.py`; meshes `/Game/World/Nevada/SM_S1D3_*`.

| mesh | triangles | size m (along x deep) | top m | TALL/low | FTCLEAR left out / moved / placed |
|---|---|---|---|---|---|
| SM_S1D3_TradingPost | 2638 | 15.0 x 6.9 | 8.98 | TALL | not listed |
| SM_S1D3_Cantina | 1790 | 11.6 x 7.3 | 9.26 | TALL | not listed |
| SM_S1D3_MotorCourt | 1934 | 17.4 x 5.0 | 7.82 | TALL | not listed |
| SM_S1D3_FillingStation | 1854 | 14.0 x 6.5 | 6.55 | TALL | not listed |
| SM_S1D3_RepairShop | 1528 | 11.7 x 6.5 | 9.1 | TALL | not listed |
| SM_S1D3_PotShop | 1360 | 6.4 x 5.1 | 5.0 | TALL | not listed |
| SM_S1D3_CliffHouse | 2298 | 13.3 x 12.9 | 25.9 | TALL | not listed |
| SM_S1D3_CliffTower | 1832 | 10.6 x 10.5 | 26.46 | TALL | not listed |
| SM_S1D3_WaterJar | 416 | 2.9 x 2.8 | 4.9 | TALL | not listed |
| SM_S1D3_DryingRack | 504 | 4.0 x 1.2 | 3.43 | TALL | not listed |
| SM_S1D3_BrushShade | 296 | 4.4 x 3.0 | 3.18 | TALL | not listed |
| SM_S1D3_AirTower | 372 | 2.6 x 1.3 | 5.38 | TALL | not listed |
| SM_S1D3_LampPost | 244 | 3.0 x 1.0 | 4.03 | TALL | not listed |
| SM_S1D3_BlanketLine | 308 | 6.1 x 1.0 | 3.2 | TALL | not listed |
| SM_S1D3_PotStall | 636 | 5.2 x 2.1 | 3.1 | TALL | not listed |
| SM_S1D3_ClayOven | 422 | 3.7 x 2.1 | 1.78 | low | not listed |
| SM_S1D3_RadiatorStand | 560 | 5.0 x 1.1 | 1.79 | low | not listed |
| SM_S1D3_GardenWall | 524 | 7.1 x 1.0 | 1.82 | low | not listed |
| SM_S1D3_BigPump | 2952 | 13.2 x 10.6 | 30.38 | TALL | not listed |

### s1d4: district 4 "Jackpot Junction and the City Lights", 7.75 to 10.69 km: roadside modern, deep violet + neon; hero the Lucky Horseshoe

Generator `Content/Python/ft_world_nevada_s1d4.py`; meshes `/Game/World/Nevada/SM_S1D4_*`.

| mesh | triangles | size m (along x deep) | top m | TALL/low | FTCLEAR left out / moved / placed |
|---|---|---|---|---|---|
| SM_S1D4_CoffeeShop | 1212 | 16.6 x 9.6 | 14.9 | TALL | not listed |
| SM_S1D4_DrumLounge | 1512 | 12.4 x 12.4 | 22.79 | TALL | not listed |
| SM_S1D4_MotelOffice | 1200 | 15.6 x 9.8 | 23.8 | TALL | not listed |
| SM_S1D4_BowlingAlley | 1196 | 16.2 x 9.6 | 16.3 | TALL | not listed |
| SM_S1D4_DriveIn | 976 | 12.2 x 11.1 | 19.4 | TALL | not listed |
| SM_S1D4_ArcadeHall | 1304 | 15.0 x 9.4 | 13.05 | TALL | not listed |
| SM_S1D4_SodaBar | 878 | 9.0 x 5.9 | 8.82 | TALL | not listed |
| SM_S1D4_StarLamp | 904 | 3.4 x 2.1 | 8.63 | TALL | not listed |
| SM_S1D4_ArrowSign | 484 | 4.5 x 1.2 | 10.6 | TALL | not listed |
| SM_S1D4_TubeCactus | 492 | 3.0 x 1.8 | 7.44 | TALL | not listed |
| SM_S1D4_PhoneKiosk | 462 | 3.8 x 2.5 | 4.3 | TALL | not listed |
| SM_S1D4_AtomPylon | 614 | 3.1 x 3.1 | 9.6 | TALL | not listed |
| SM_S1D4_FinPylons | 264 | 4.7 x 0.9 | 7.0 | TALL | not listed |
| SM_S1D4_KidneyPlanter | 266 | 3.3 x 1.8 | 1.85 | low | 0 / 4 / 19 |
| SM_S1D4_RopePodium | 480 | 4.2 x 1.5 | 1.32 | low | 0 / 3 / 19 |
| SM_S1D4_LuckyHorseshoe | 2704 | 33.4 x 12.2 | 47.23 | TALL | not listed |

### s1d5: district 5 "Salt Flats", 10.69 to 14.50 km: speed-week camp and salt works, silver + red stripes

Generator `Content/Python/ft_world_nevada_s1d5.py`; meshes `/Game/World/Nevada/SM_S1D5_*`.

| mesh | triangles | size m (along x deep) | top m | TALL/low | FTCLEAR left out / moved / placed |
|---|---|---|---|---|---|
| SM_S1D5_TimingTower | 1036 | 11.6 x 6.8 | 13.36 | TALL | not listed |
| SM_S1D5_PitShelter | 1022 | 11.8 x 8.4 | 6.28 | TALL | not listed |
| SM_S1D5_EvaporatorHouse | 1648 | 33.9 x 12.2 | 16.8 | TALL | not listed |
| SM_S1D5_ScrutineerShed | 1120 | 15.6 x 8.8 | 8.65 | TALL | not listed |
| SM_S1D5_CaravanDiner | 1030 | 10.1 x 5.8 | 4.1 | TALL | not listed |
| SM_S1D5_CommentaryBox | 766 | 7.3 x 4.8 | 10.53 | TALL | not listed |
| SM_S1D5_CaravanCamp | 1032 | 14.1 x 4.5 | 4.35 | TALL | not listed |
| SM_S1D5_SaltTower | 1612 | 14.1 x 8.3 | 16.55 | TALL | not listed |
| SM_S1D5_SaltCone | 360 | 7.5 x 6.5 | 5.45 | TALL | not listed |
| SM_S1D5_PennantMasts | 500 | 13.5 x 0.7 | 7.52 | TALL | not listed |
| SM_S1D5_MarkerDrums | 492 | 8.1 x 0.6 | 1.05 | low | not listed |
| SM_S1D5_TyreRack | 600 | 5.2 x 0.8 | 1.81 | low | not listed |
| SM_S1D5_Windsock | 460 | 3.3 x 1.7 | 8.5 | TALL | not listed |
| SM_S1D5_ResultsBoard | 502 | 5.5 x 1.7 | 4.7 | TALL | not listed |
| SM_S1D5_SpeakerPole | 380 | 3.1 x 2.0 | 7.24 | TALL | not listed |
| SM_S1D5_SunShade | 632 | 6.6 x 3.4 | 2.9 | TALL | not listed |
| SM_S1D5_FloodMast | 644 | 3.1 x 2.3 | 8.34 | TALL | not listed |
| SM_S1D5_BigStopwatch | 2664 | 36.4 x 19.6 | 36.2 | TALL | not listed |

### s1d6: district 6 "Syndicate Range", 14.50 to 18.50 km: black blockhouses, cyan strips, pale domes; hero the Big Ear

Generator `Content/Python/ft_world_nevada_s1d6.py`; meshes `/Game/World/Nevada/SM_S1D6_*`.

| mesh | triangles | size m (along x deep) | top m | TALL/low | FTCLEAR left out / moved / placed |
|---|---|---|---|---|---|
| SM_S1D6_RadomeTower | 1030 | 13.3 x 9.4 | 19.13 | TALL | not listed |
| SM_S1D6_Magazines | 980 | 27.6 x 9.4 | 10.86 | TALL | not listed |
| SM_S1D6_CommandBunker | 492 | 14.0 x 9.1 | 13.39 | TALL | not listed |
| SM_S1D6_TestStand | 964 | 21.2 x 9.8 | 17.7 | TALL | not listed |
| SM_S1D6_GateTower | 642 | 14.3 x 7.4 | 17.89 | TALL | not listed |
| SM_S1D6_AerialHut | 748 | 15.7 x 8.7 | 15.1 | TALL | not listed |
| SM_S1D6_TrackHouse | 570 | 10.4 x 6.5 | 9.16 | TALL | not listed |
| SM_S1D6_BlastWall | 276 | 13.6 x 5.9 | 4.9 | TALL | not listed |
| SM_S1D6_WindSock | 520 | 3.7 x 2.9 | 5.63 | TALL | not listed |
| SM_S1D6_CameraMast | 144 | 1.3 x 1.2 | 7.0 | TALL | not listed |
| SM_S1D6_WarnBoard | 192 | 3.2 x 1.5 | 3.4 | TALL | not listed |
| SM_S1D6_Pylons | 120 | 2.5 x 0.8 | 4.46 | TALL | not listed |
| SM_S1D6_Cowls | 304 | 3.8 x 2.4 | 4.44 | TALL | not listed |
| SM_S1D6_CalTarget | 240 | 4.5 x 2.6 | 4.53 | TALL | not listed |
| SM_S1D6_TankTraps | 116 | 9.4 x 1.9 | 1.3 | low | not listed |
| SM_S1D6_CableDrums | 420 | 5.2 x 1.8 | 1.66 | low | not listed |
| SM_S1D6_Hatch | 228 | 3.6 x 2.5 | 1.54 | low | not listed |
| SM_S1D6_BigEar | 2038 | 38.0 x 29.7 | 41.9 | TALL | not listed |

### s1d7: district 7 "Red Rock Return", 18.50 to 22.00 km: mine camp, teal frames + bleached lilac boards; hero the Twin-Wheel Headframe

Generator `Content/Python/ft_world_nevada_s1d7.py`; meshes `/Game/World/Nevada/SM_S1D7_*`.

| mesh | triangles | size m (along x deep) | top m | TALL/low | FTCLEAR left out / moved / placed |
|---|---|---|---|---|---|
| SM_S1D7_OreMill | 1260 | 10.1 x 15.8 | 23.96 | TALL | not listed |
| SM_S1D7_Tipple | 824 | 6.5 x 12.5 | 13.96 | TALL | not listed |
| SM_S1D7_AssayTower | 856 | 5.7 x 6.2 | 21.93 | TALL | not listed |
| SM_S1D7_PowderHouse | 768 | 8.1 x 7.2 | 9.0 | TALL | not listed |
| SM_S1D7_BoardingRim | 1032 | 9.3 x 8.3 | 28.03 | TALL | not listed |
| SM_S1D7_HoistHouse | 1192 | 12.7 x 9.4 | 25.1 | TALL | not listed |
| SM_S1D7_RimFrame | 1152 | 9.0 x 7.9 | 33.16 | TALL | not listed |
| SM_S1D7_BunkHouse | 1452 | 8.5 x 10.7 | 15.42 | TALL | not listed |
| SM_S1D7_OreCart | 756 | 9.1 x 1.9 | 1.52 | low | not listed |
| SM_S1D7_AditPortal | 548 | 8.4 x 7.3 | 5.16 | TALL | not listed |
| SM_S1D7_StampBattery | 640 | 6.4 x 5.6 | 7.46 | TALL | not listed |
| SM_S1D7_Flume | 688 | 15.6 x 2.6 | 7.04 | TALL | not listed |
| SM_S1D7_SluiceBox | 632 | 8.8 x 2.7 | 1.78 | low | 0 / 1 / 21 |
| SM_S1D7_TramTower | 520 | 7.6 x 4.5 | 14.7 | TALL | 0 / 5 / 21 |
| SM_S1D7_Windlass | 512 | 5.7 x 4.0 | 3.6 | TALL | 0 / 2 / 9 |
| SM_S1D7_BellFrame | 592 | 5.1 x 2.0 | 6.05 | TALL | not listed |
| SM_S1D7_WashLine | 572 | 6.1 x 2.9 | 2.8 | TALL | not listed |
| SM_S1D7_TwinHeadframe | 2848 | 33.7 x 11.3 | 45.2 | TALL | not listed |

### s1d8: district 8 "Vegas Approach", 22.00 to 25.60 km: grand hotels, white + gold + crimson; hero the Crown Tower

Generator `Content/Python/ft_world_nevada_s1d8.py`; meshes `/Game/World/Nevada/SM_S1D8_*`.

| mesh | triangles | size m (along x deep) | top m | TALL/low | FTCLEAR left out / moved / placed |
|---|---|---|---|---|---|
| SM_S1D8_CarriagePorch | 1842 | 23.0 x 18.4 | 15.69 | TALL | not listed |
| SM_S1D8_HotelTower | 1548 | 21.0 x 17.9 | 29.77 | TALL | not listed |
| SM_S1D8_WinterGarden | 1614 | 25.0 x 14.5 | 16.23 | TALL | not listed |
| SM_S1D8_Arcade | 1690 | 29.2 x 9.4 | 15.5 | TALL | not listed |
| SM_S1D8_Theatre | 984 | 25.2 x 15.7 | 17.08 | TALL | not listed |
| SM_S1D8_ClockPavilion | 1280 | 7.6 x 8.2 | 26.43 | TALL | not listed |
| SM_S1D8_Belvedere | 1586 | 8.2 x 8.7 | 15.34 | TALL | not listed |
| SM_S1D8_LampStandard | 380 | 2.5 x 2.4 | 7.62 | TALL | not listed |
| SM_S1D8_Topiary | 328 | 5.4 x 1.7 | 5.06 | TALL | not listed |
| SM_S1D8_UrnBalustrade | 362 | 6.2 x 2.1 | 4.54 | TALL | not listed |
| SM_S1D8_FlagCourt | 286 | 6.0 x 4.6 | 10.49 | TALL | not listed |
| SM_S1D8_GateArch | 548 | 7.6 x 1.5 | 8.54 | TALL | not listed |
| SM_S1D8_VictoryColumn | 316 | 3.0 x 3.0 | 11.28 | TALL | not listed |
| SM_S1D8_PoolEdge | 312 | 9.1 x 2.2 | 1.72 | low | not listed |
| SM_S1D8_RopeLine | 740 | 9.2 x 1.9 | 1.2 | low | not listed |
| SM_S1D8_CrownTower | 3284 | 19.0 x 19.1 | 58.36 | TALL | not listed |

### s1plants: the planting of all eight districts (slots s % 4 == 3)

Generator `Content/Python/ft_world_nevada_s1plants.py`; meshes `/Game/World/Nevada/SM_S1PLANTS_*`.

| mesh | triangles | size m (along x deep) | top m | TALL/low | FTCLEAR left out / moved / placed |
|---|---|---|---|---|---|
| SM_S1PLANTS_Yucca | 724 | 3.6 x 3.0 | 4.19 | TALL | not listed |
| SM_S1PLANTS_Cholla | 936 | 3.3 x 2.4 | 2.67 | TALL | not listed |
| SM_S1PLANTS_Barrel | 854 | 3.4 x 1.9 | 1.12 | low | 0 / 1 / 14 |
| SM_S1PLANTS_Beavertail | 612 | 2.6 x 1.3 | 1.34 | low | not listed |
| SM_S1PLANTS_Creosote | 768 | 2.3 x 2.7 | 2.51 | TALL | not listed |
| SM_S1PLANTS_Rabbitbrush | 504 | 2.9 x 2.6 | 1.4 | low | not listed |
| SM_S1PLANTS_Ocotillo | 1020 | 5.2 x 4.6 | 4.93 | TALL | not listed |
| SM_S1PLANTS_Mesquite | 692 | 7.5 x 5.2 | 5.05 | TALL | not listed |
| SM_S1PLANTS_Cottonwood | 664 | 7.8 x 6.6 | 11.25 | TALL | not listed |
| SM_S1PLANTS_DesertWillow | 864 | 6.3 x 4.6 | 7.35 | TALL | not listed |
| SM_S1PLANTS_Century | 864 | 3.5 x 3.7 | 6.4 | TALL | not listed |
| SM_S1PLANTS_Pinyon | 372 | 6.3 x 6.1 | 7.05 | TALL | not listed |
| SM_S1PLANTS_RimPines | 876 | 9.1 x 5.6 | 25.66 | TALL | not listed |
| SM_S1PLANTS_Saltbush | 696 | 4.5 x 3.5 | 0.93 | low | not listed |
| SM_S1PLANTS_Tamarisk | 744 | 5.8 x 5.8 | 4.96 | TALL | not listed |
| SM_S1PLANTS_Bunchgrass | 788 | 6.7 x 2.2 | 1.31 | low | not listed |
| SM_S1PLANTS_Wildflowers | 1156 | 7.7 x 2.9 | 0.89 | low | not listed |
| SM_S1PLANTS_FanPalm | 760 | 4.6 x 4.9 | 12.45 | TALL | not listed |
| SM_S1PLANTS_DatePalms | 1012 | 9.4 x 6.5 | 10.79 | TALL | not listed |
| SM_S1PLANTS_BloomHedge | 672 | 7.3 x 1.7 | 1.78 | low | 0 / 2 / 40 |
| SM_S1PLANTS_JuniperFlames | 516 | 7.2 x 1.7 | 5.07 | TALL | not listed |

### s1animals: the animals of all eight districts

Generator `Content/Python/ft_world_nevada_s1animals.py`; meshes `/Game/World/Nevada/SM_S1ANIMALS_*`.

| mesh | triangles | size m (along x deep) | top m | TALL/low | FTCLEAR left out / moved / placed |
|---|---|---|---|---|---|
| SM_S1ANIMALS_BurroShelf | 996 | 14.4 x 4.8 | 3.92 | TALL | not listed |
| SM_S1ANIMALS_SheepFlock | 1388 | 18.0 x 7.7 | 2.77 | TALL | not listed |
| SM_S1ANIMALS_Jackrabbits | 564 | 9.1 x 5.2 | 3.66 | TALL | not listed |
| SM_S1ANIMALS_Roadrunners | 636 | 17.9 x 3.3 | 3.85 | TALL | not listed |
| SM_S1ANIMALS_FoxDen | 804 | 12.0 x 6.4 | 2.76 | TALL | not listed |
| SM_S1ANIMALS_Tortoises | 524 | 8.1 x 5.2 | 2.73 | TALL | not listed |
| SM_S1ANIMALS_QuailWall | 1188 | 11.9 x 1.3 | 3.48 | TALL | not listed |
| SM_S1ANIMALS_Peacocks | 996 | 13.6 x 1.4 | 4.16 | TALL | not listed |
| SM_S1ANIMALS_MuleDeer | 1116 | 17.0 x 7.1 | 3.49 | TALL | not listed |
| SM_S1ANIMALS_DuckTank | 852 | 7.2 x 6.3 | 3.0 | TALL | not listed |
| SM_S1ANIMALS_BrinePool | 986 | 15.2 x 9.0 | 1.54 | low | not listed |
| SM_S1ANIMALS_SwanLagoon | 708 | 14.8 x 5.8 | 3.06 | TALL | not listed |
| SM_S1ANIMALS_PelicanLine | 658 | 14.3 x 3.6 | 3.69 | TALL | not listed |
| SM_S1ANIMALS_Dovecote | 900 | 14.6 x 13.5 | 9.72 | TALL | not listed |
| SM_S1ANIMALS_Owls | 462 | 9.8 x 7.7 | 4.54 | TALL | not listed |
| SM_S1ANIMALS_FlamingoPool | 944 | 11.5 x 6.9 | 2.86 | TALL | not listed |
| SM_S1ANIMALS_HenCoop | 1100 | 8.5 x 5.5 | 4.66 | TALL | not listed |
| SM_S1ANIMALS_CraneDance | 848 | 13.0 x 8.0 | 2.05 | TALL | not listed |
| SM_S1ANIMALS_PrairieDogs | 1068 | 13.0 x 7.0 | 1.42 | low | not listed |

### s1people: people in the gaps pass 3 left (slots s % 8 == 1), each group on what it stands on

Generator `Content/Python/ft_world_nevada_s1people.py`; meshes `/Game/World/Nevada/SM_S1PEOPLE_*`.

| mesh | triangles | size m (along x deep) | top m | TALL/low | FTCLEAR left out / moved / placed |
|---|---|---|---|---|---|
| SM_S1PEOPLE_SendOff | 1708 | 5.2 x 3.1 | 7.04 | TALL | not listed |
| SM_S1PEOPLE_PorchRail | 1788 | 4.7 x 3.2 | 6.95 | TALL | not listed |
| SM_S1PEOPLE_MailWagon | 1712 | 5.8 x 2.7 | 4.9 | TALL | not listed |
| SM_S1PEOPLE_DrumTable | 1642 | 4.5 x 3.1 | 5.25 | TALL | not listed |
| SM_S1PEOPLE_PorchBand | 1556 | 4.9 x 3.3 | 5.85 | TALL | not listed |
| SM_S1PEOPLE_Linemen | 1512 | 4.5 x 2.4 | 7.6 | TALL | not listed |
| SM_S1PEOPLE_AdobeTerrace | 1708 | 4.7 x 3.3 | 5.19 | TALL | not listed |
| SM_S1PEOPLE_PotStall | 1640 | 4.6 x 3.1 | 5.52 | TALL | not listed |
| SM_S1PEOPLE_Bellhops | 1492 | 4.8 x 3.1 | 6.6 | TALL | not listed |
| SM_S1PEOPLE_SnapShooters | 1528 | 4.7 x 3.5 | 7.09 | TALL | not listed |
| SM_S1PEOPLE_TimingStand | 1600 | 6.0 x 3.1 | 7.4 | TALL | not listed |
| SM_S1PEOPLE_UmbrellaWagon | 1760 | 5.7 x 3.0 | 5.6 | TALL | not listed |
| SM_S1PEOPLE_PitDeck | 1792 | 4.5 x 3.1 | 5.86 | TALL | not listed |
| SM_S1PEOPLE_Catwalk | 1688 | 4.5 x 2.6 | 6.69 | TALL | not listed |
| SM_S1PEOPLE_BunkerRoof | 1362 | 4.9 x 3.2 | 5.38 | TALL | not listed |
| SM_S1PEOPLE_Surveyors | 1772 | 4.9 x 3.0 | 7.63 | TALL | not listed |
| SM_S1PEOPLE_MineGallery | 1716 | 4.7 x 3.2 | 8.65 | TALL | not listed |
| SM_S1PEOPLE_HoistDeck | 1556 | 4.7 x 3.1 | 7.59 | TALL | not listed |
| SM_S1PEOPLE_RimFlags | 1340 | 7.5 x 3.8 | 31.28 | TALL | not listed |
| SM_S1PEOPLE_RimParasols | 1668 | 5.1 x 4.2 | 27.79 | TALL | not listed |
| SM_S1PEOPLE_GrandStair | 1552 | 4.5 x 3.2 | 6.44 | TALL | not listed |
| SM_S1PEOPLE_Quartet | 1532 | 4.7 x 3.0 | 6.8 | TALL | not listed |
| SM_S1PEOPLE_JumpStand | 2688 | 9.3 x 3.6 | 12.2 | TALL | not listed |

What each piece is and what moves on it: in the builders' hand-overs and in the header comment of each block in `FTPeopleL2.inl` (BEGIN s1...). Not repeated here; none of it was checked by the integrator.

## Not checked

- Nothing was looked at in the game beyond the one frame at 6 s: no district past the state line was seen; no mesh was inspected for look, palette, movement, glow, shadows, culling or how it reads at speed.
- Whether any new piece stands inside another piece, in a rock cut where it cannot be seen, or too near a jump, loop or banked bend: not checked (the track left no new piece out of the road; that is all the log says).
- Frame rate and hitches with the new pieces: not measured, not compared with before.
- Per-mesh placed counts for meshes FTCLEAR does not list; the builders' estimates against the real counts.
- The content rules (no real names, brands, likenesses; nothing demeaning) were not reviewed by the integrator beyond reading the hand-overs.
- The other seven levels, a packaged build. M_PP_Cel was not touched (`git status` shows only the table file, the 11 generators, `Content/World/` and this note).

## Review and fix round (16 Nov 2026)

One fix round after Stage 1's two reviews (a rules reader who computed from the tables, a driver who drove 0 to 19.1 km). Written from what was run and seen in this round. Scratch: `C:\ftwork\jobs\L2s1fix`, `C:\ftwork\out\L2s1fix`.

### Found

- **No play rule broken** (rules reader, by the numbers). Checked again on the changed table with the reader's own scripts: no centre nearer than 17 m, no TALL centre nearer than 24 m (smallest TALL built edge 24.46 m, as before), no lit piece inside 24 m, nothing blinks, people 36.0 to 44.4 m, ground animals 42.5 m and more, slot classes kept.
- **Looks wrong, one cause through four blocks:** pass 3b's half-slot scenes stand 28.25 m along from their slot (22.6 x their 1.25 scale), not 22.6. With the other overlaps, about 160 Stage 1 pieces stood 1 m or more inside an older piece (the reader's box-against-box test: 421 overlaps of 0.3 m or more).
- **Looks wrong (driver):** a white twin-barrelled piece by the right fence at 15.19 km read as a twin gun; nothing past 19.1 km can be reached before the boss leaves.
- **Weak:** three heroes did not read or were not found (Big Pump, Tin Rattler, Big Ear); the 25 to 60 m band read as one dark tangle in the open districts; green metre-sized domes with pink caps near the right barrier; counts over the brief's guides; a few trivial edges.

### Fixed

How it was tested: the reader's harness dumps the table (`rules.tsv`), `place.py` re-does the track's placing loop, and every Stage 1 piece is tested box against box against every older piece (`coll.py`, `who.py`: a travelling piece's whole lane counts; an old dressing mesh of unknown size counts when its centre lies in the new footprint). Jump, loop and clearance exclusions are not modelled.

| | before | after |
|---|---|---|
| Stage 1 pieces written (before the track leaves any out) | 3,305 | 2,935 |
| overlaps of 0.3 m or more with an older piece (the reader's test) | 421 | 8 |
| of them 1 m or more | about 160 | 0 |

- **F1, s1d5:** the block is written slot by slot now (`d5_table.py`), each piece only where it overlaps nothing older by 0.3 m; 48 places left open. 264 written before, 189 now.
- **F2, s1d6:** compounds slot by slot (`d6_table.py`), anchor 66 m, or 69.5 or 63 m beside pass 3a's groups; 5 compounds left out.
- **F3, s1animals:** the Red Rock Return gaps' deer and burros stand 75 to 77 m out, short of the trails (the reader's 111 to 120 m is where the second rock wall and the mesa stubs stand); Last Chance Gas roadrunners moved to the right; 21 rules moved nearer or further (`opt.py`); 24 rules lost their bad slots (`thin.py`: same rule index, so every remaining piece keeps its place). 385 written before, 292 now.
- **F4, s1d8:** the towers of 498R, 518R, 558R use the +19.2 sets (off the Jackpot set's dice pylon).
- **F5, s1d8:** the near props are single pieces placed where they clear the scenes by 0.1 m and each other by 1 m (`d8_fix.py`); 35 have no free place and are left out. Also 516R's porch and 536R's arcade are off (an older chapel and trailer stand in their footprints). 283 written before, 242 now.
- **F6, s1d3:** 146R, 156L, 166R left open (the far runners' ledge), slot 170 both sides and 152L left open (the level's sign frame in the footprint).
- **F7, s1d1:** the yards of 8L and 10L stand 60 m out (61 m, the reader's figure, still left 1.0 m in the cyclists' lane).
- **F8, s1d7:** CampC's flume 4 m along instead of 16; 470R left open.
- **F9, s1people:** eight rows moved: 145L 169L 249L 281L 433L 465L to 42 m, 297L to 39.5 m, the rim pair of 457L to 44 m.
- **F10, s1plants:** the rim pines' rules leave the needle's slots and sides; 44 rules moved nearer or further; 18 rules lost their bad slots. 616 written before, 538 now.
- **Driver 2 (twin gun):** `SM_S1D6_Cowls` is turned round in its set (yaw 180): the road sees the backs and the white vane. Which mesh the driver saw was not checked by him; this is the only twin-mouthed piece of the block.
- **Driver 3, 4, 5 (heroes):** Big Pump 1.5 times as built (95 m); Tin Rattler 1.3 times as built and 100 m out (was 130 and 132 m, not 112); Big Ear 1.3 times as built, slot 346 100 m out, slot 394 124 m out (at 112 m it stood in the lane of the first cast's wild horses: an overlap the reader's test had missed).
- **Driver 7 (tangle):** district 5's thin tall props on every second slot of their class; district 6's thin tall props on a quarter of their slots, single pieces instead of pairs.
- **Driver 8 (green domes):** Barrel and Rabbitbrush no longer stand 20.5 to 23 m out; Bunchgrass stands there.
- **W1 (counts):** d5 189, d6 232, d8 242, plants 538, animals 292 are inside the guides now. d4 279 and d1 251 unchanged.
- **W4:** s1d7's last zone ends at 22.0 km.
- **W5:** `SM_S1D5_TyreRack` has 0.6 m of footing (was 1 m). Regenerated (`C:\ftwork\logs\L2s1fix_make.log`): triangles 600, parts 5, size m 5.2 0.8, top m 1.81, low; failed 0. Its bounds were not read back.
- **W6:** the car stack's "Mid" set stands 41.5 m out (was 39.5).
- **W8:** the Tin Rattler's cream squares are gone (generator). Regenerated in the same job: triangles 2440 (was 3052), parts 17, size m 30.0 32.7, top m 25.65, TALL; failed 0; 0 `LogPython: Error` lines.

### Not fixed, and why

- **Driver 1, nothing past 19.1 km is reached:** the escape clock is not Stage 1's. Moving a Headframe or a Crown Tower before 16 km would put a district's hero outside its district, which the brief forbids. Districts 7 and 8 stay where they are, unseen in play by the driver.
- **Driver 6 (second Line Rider, second Lucky Horseshoe not found):** not changed; why they do not show was not found out.
- **Driver 7 in district 1:** its thin props are parts of its yard sets; not changed.
- **Driver 9 (empty 18.8 to 19.1 km and 2.0 to 2.3 km right):** nothing added; no second round of invention.
- **Driver 10 (black disc in a white box at the barrier):** not Stage 1's.
- **W1:** more than 40 of one mesh for five plants (Century 47, DatePalms 43, FanPalm 43, BloomHedge 43, Creosote 41, as written).
- **W2 (nine people meshes of 1,532 to 1,792 triangles placed 15 to 20 times):** would need new meshes; not changed.
- **W3 (plants in pairs):** not changed on purpose; the band moves separated some. Not counted.
- **W4, s1d4's slot 236 props reach 10.6928 km (3 m past the district's edge):** not changed.
- **W7 (SodaBar's bubbles, AditPortal's lantern: moving light inside 30 m, neither blinks):** left for whoever sees it in the game.
- **Left over from the overlap test:** 8 overlaps of 0.3 to 0.9 m (d1 3, d2 3, d7 1, one plant with another plant). Old dressing of unknown size whose centre lies within 0.5 m of a new footprint: d1 4 (rock banks, a trailer), d3 1 (a sign frame at a low radiator stand, 140R), d4 1 (mile sign), d7 7 (rock banks, which the builder said he did not dodge).
- **s1d6:** five meshes (BlastWall, RadomeTower, Magazines, CommandBunker, TrackHouse) now have placements both inside and outside 70 m (anchor 69.5 m). They cast shadows as before; the brief's guide against it is not met.

### The run after the fix

- Compile (`build.ps1`, tag L2s1_fix_build): exit 0 after 76 s, "Result: Succeeded".
- One game start (`run_game.ps1`, `start_L2.txt`, out `C:\ftwork\out\s1\fixed_L2`): exit 0.

| | Stage 1 as integrated (`after_L2`) | after the fix (`fixed_L2`) |
|---|---|---|
| FTTRACK level=2 | frames=12888 route=efc95392 solid=e2d27537 solidTriangles=964188 | the same; compared by me with the `^FTTRACK level=2 ` line of `SourceArt/Jam/validate/track_baseline.txt`: equal (the tool's own "baseline" print shows the level=0 line) |
| FTDRESS districts / sets | 2885 / 313 | 3022 / 345 |
| FTDRESS pieces placed of tried | 14707 of 14737 | 14345 of 14375 (Stage 1: 2756 placed, was 3118) |
| FTDRESS shadows | 10936 of 15683 | 10650 of 15321 |
| FTCLEAR props kept / moved off their barrier / left out of the road | 17771 / 189 / 30 | 17409 / 188 / 30 |
| FTBANK | pieces laid=127 on 79 lengths | the same |

- "Fatal error" lines: 0. "LogFT: Error" lines: 0. Lines naming `/Game/World` with fail/error/warning/missing: 0.
- One engine ensure, `Ensure condition failed: OriginX <= OriginMax ...` (DoubleFloat.cpp), 4 s before `FTStartLevel`, while the title's attract level (level 0) was up. The driver reports the same line in one of his runs and in a pass 3 run from before Stage 1; the integration run did not have it.
- FTCLEAR lists these Stage 1 meshes (left out / moved / placed): SM_S1D2_MailSnake 0/1/7, SM_S1D4_KidneyPlanter 0/4/19, SM_S1D4_RopePodium 0/3/19, SM_S1D7_SluiceBox 0/1/20, SM_S1D7_TramTower 0/5/20, SM_S1D7_Windlass 0/2/9, SM_S1PLANTS_BloomHedge 0/1/41, SM_S1PLANTS_Bunchgrass 0/1/32. None has a piece left out.
- The script's wait for `FT CLOCK level=6.0` timed out after 150 s: the game printed 5.1 then 6.1 (about 6 frames a second; six UnrealEditor processes were listed on the PC before the run), so the one frame `L2_t006.png` was taken at level clock 143.1 s, in Joshua Flats (scene 4 of 9). It shows the road, the car, the HUD, a stand of people on the left, balloons and rock on both sides; which pieces are Stage 1's could not be told from it.

### Not checked

- Nothing was looked at in the game in this round beyond the one frame at 6 s: whether the turned cowls, the bigger heroes and the thinner rows read as meant was not seen. No drive, no film.
- The overlap test is flat (along, out), ignores road curvature and does not know which pieces the track leaves out or pushes.
- Sizes of the old dressing meshes: still only the centre-in-footprint test.
- Frame rate: not measured.
- The driver's contact sheet `C:\ftwork\out\L2s1rev\L2_s1_review.jpg` is 1.66 MB, over the 1 MB limit: not copied into the repository.
- M_PP_Cel: not touched.
