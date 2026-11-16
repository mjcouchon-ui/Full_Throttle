# Level 3 "Volcano": Stage 1 of the final pass (the world), integrated 16 Nov 2026

Branch `claude/world-L3`. Everything here is what the integrator ran and saw; what was not looked at is under "Not checked".

## What a player now sees

Eight districts along the 25 km, each with its own buildings, props and one landmark, plus plants, animals and raised
groups of people over the whole road. Zone plan: `C:\ftwork\final\s1\L3_plan.md`.

| builder | district | km | look | hero landmark |
|---|---|---|---|---|
| s1d1 | The Ash Road = CINDER SPRINGS | 0 to 3.76 | whitewashed spa town, cobalt domes, lemon awnings | SM_S1D1_SteamClock |
| s1d2 | Fire and Brimstone = STORMWATCH | 3.76 to 8.34 | violet riveted metal, rings and balls, steady cyan | SM_S1D2_StormHarp |
| s1d3 | Engine 13's Road = SLAGTOWN | 8.34 to 11.1 | mustard, cream and teal tin sheds on stilts | SM_S1D3_Hopper |
| s1d4 | The Burning Slopes = THE CLIFF TEMPLE | 11.1 to 13.9 | white walls, jade tiered roofs, gold | SM_S1D4_BellTower (far=True) |
| s1d5 | The Ash Fields = WINDMILL FLATS | 13.9 to 16.7 | pink round mills, sky blue sails | SM_S1D5_GrandMill |
| s1d6 | The Far Works = GEOTHERM | 16.7 to 19.5 | white hourglass towers, lime bands, graphite | SM_S1D6_ThreeSisters |
| s1d7 | The Inner Wall = THE WALL KEEP | 19.5 to 22.3 | sand stone, slate cones, turquoise banners | SM_S1D7_CrownKeep |
| s1d8 | The Last Ash = THE BACKLOT | 22.3 to 25 | mint and cream painted fronts on braces | SM_S1D8_BigSlate (far=True) |
| s1plants | plants | whole road | 21 pieces, slots s % 4 == 3 | |
| s1animals | animals | whole road | 19 pieces | |
| s1people | people on their own decks, stairs, terraces | whole road | 25 pieces | |

## How it went in

- All 11 blocks `C:\ftwork\s1\L3_*.inl` were pasted, in the order above, at the END of
  `Source/FullThrottle/FTPeopleL3.inl` (2,868 lines added, nothing existing changed). None was left out.
- Before pasting, for every block: its generator exists, its make log (`C:\ftwork\logs\L3<id>_make.log`) ends
  `pieces built N failed 0` with 0 `LogPython: Error` and 0 `FAILED` lines, every mesh name the block uses has its
  `.uasset` in `Content/World/Volcano`, every Set is named `WldS1...`, no Set name is used twice, no `Lit` or `Bank`.
- The first compile failed: `FTTrackDress.cpp(150) : error C4883: 'FTableWriter::People3': function size suppresses
  optimizations`. Fix, in the pasted part only: each block's wrapper is `[&]() { ... }();` instead of `{ ... }` (a
  function of its own; the same thing level 5's table already does). Rule order is unchanged. Second compile:
  `build exit 0`, `Result: Succeeded`. No line inside any block was edited.
- The level was started once (`C:\ftwork\out\s1\after_L3\run.log`, one frame `L3_t006.png`).

## Before and after (same code, same start script)

| | before (`C:\ftwork\out\p3\final_L3\run.log`) | after |
|---|---|---|
| FTDRESS level=3 placed / tried | 11436 of 11491 | 14450 of 14505 (+3014 placed) |
| FTDRESS shadows | 3239 of 11436 | 5139 of 14450 |
| FTDRESS districts / sets | 465 / 29 | 869 / 198 |
| FTCLEAR props kept, moved off their barrier, left out of the road | 13810, 327, 55 | 16824, 363, 55 |
| FTWPO life components | 102 of 172 stop moving past 180 m | 305 of 380 (380 - 172 = 208 = the number of new meshes) |
| FTBUILD props / total | 1153 ms / 1542 ms | 9472 ms / 10101 ms (first load of the new meshes) |
| hitches logged on L_Volcano (FTHITCH2 lines) | 1 (517 ms at t=9.5 s, loads=1) | 32: 31 of 40 to 99 ms (median 49) in the first 8.5 s, and one of 1782 ms at t=9.6 s; all loads=0 gc=0 |

- FTTRACK: `FTTRACK level=3 frames=12497 route=506786e7 solid=eead12ba solidTriangles=900592` in the run, equal to the
  `^FTTRACK level=3 ` line of `SourceArt/Jam/validate/track_baseline.txt` (compared by hand; the tool printed level 0's).
- `Fatal error`: 0. `LogFT: Error`: 0. No `/Game/World` load failure line (21 lines name `/Game/World`, all
  `LogStaticMesh: Display: Waiting for static meshes to be ready`).
- The one frame (6 s in, by the start): whitewashed buildings with blue domes stand on the right of the road.
- THE HITCHES ARE NEW AND NOT EXPLAINED. The run before had one; this one has 32 in about 9.6 s. Whether they come from
  the first load of 208 freshly made meshes or stay on a second run was not checked (the level was started once).

## The new meshes (208, 194,988 triangles together; 154 TALL, 54 low)

Triangles, top and class are the `FTPEOPLE piece` lines of each builder's make log (what the hand-overs copied).
FTCLEAR names a mesh only when one of its pieces was moved or left out; "not listed" = no count per mesh was printed.
Of the 17 listed: 0 left out, 36 moved, 352 placed.

### s1d1, "The Ash Road" = CINDER SPRINGS, 0 to 3.76 km

Generator `Content/Python/ft_world_volcano_s1d1.py`. 17 meshes, 16128 triangles together.

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D1_BathHouse | 1616 | 13.27 | TALL | not listed |
| SM_S1D1_TeaPavilion | 1116 | 12.27 | TALL | not listed |
| SM_S1D1_BoilerHouse | 1252 | 15.6 | TALL | not listed |
| SM_S1D1_PumpRoom | 1436 | 14.27 | TALL | not listed |
| SM_S1D1_GuestHouse | 1212 | 16.84 | TALL | not listed |
| SM_S1D1_Laundry | 1584 | 14.13 | TALL | not listed |
| SM_S1D1_ChangingTowers | 1320 | 13.18 | TALL | not listed |
| SM_S1D1_SteamClock | 3120 | 40.94 | TALL | not listed |
| SM_S1D1_ParasolRow | 460 | 3.15 | TALL | not listed |
| SM_S1D1_TowelLine | 248 | 3.49 | TALL | not listed |
| SM_S1D1_SunArch | 444 | 5.96 | TALL | not listed |
| SM_S1D1_SteamVane | 484 | 6.11 | TALL | not listed |
| SM_S1D1_PlungeRim | 216 | 1.32 | low | 0 / 2 / 10 |
| SM_S1D1_SpringTap | 492 | 1.8 | low | 0 / 2 / 15 |
| SM_S1D1_TubStack | 432 | 1.44 | low | 0 / 2 / 18 |
| SM_S1D1_Loungers | 368 | 0.86 | low | 0 / 2 / 14 |
| SM_S1D1_TeaBench | 328 | 1.74 | low | 0 / 1 / 12 |

### s1d2, "Fire and Brimstone" = STORMWATCH, 3.76 to 8.34 km

Generator `Content/Python/ft_world_volcano_s1d2.py`. 16 meshes, 12850 triangles together.

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D2_DrumHall | 992 | 17.2 | TALL | not listed |
| SM_S1D2_InsulTower | 1044 | 18.75 | TALL | not listed |
| SM_S1D2_SphereTank | 692 | 14.58 | TALL | not listed |
| SM_S1D2_CapShed | 1048 | 13.7 | TALL | not listed |
| SM_S1D2_WatchOffice | 980 | 16.47 | TALL | not listed |
| SM_S1D2_CoilHouse | 1196 | 12.78 | TALL | not listed |
| SM_S1D2_Kiln | 600 | 12.5 | TALL | not listed |
| SM_S1D2_CoilDrum | 362 | 1.89 | low | 0 / 3 / 15 |
| SM_S1D2_InsulBollards | 772 | 1.62 | low | 0 / 5 / 13 |
| SM_S1D2_ShardPlinths | 360 | 1.93 | low | 0 / 6 / 15 |
| SM_S1D2_MeterBench | 284 | 1.68 | low | 0 / 4 / 17 |
| SM_S1D2_LightningRod | 412 | 10.89 | TALL | not listed |
| SM_S1D2_CableArch | 456 | 8.1 | TALL | not listed |
| SM_S1D2_InstrumentVane | 248 | 7.03 | TALL | not listed |
| SM_S1D2_SignalMast | 336 | 9.29 | TALL | not listed |
| SM_S1D2_StormHarp | 3068 | 57.28 | TALL | not listed |

### s1d3, "Engine 13's Road" = SLAGTOWN, 8.34 to 11.1 km

Generator `Content/Python/ft_world_volcano_s1d3.py`. 18 meshes, 18840 triangles together.

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D3_PayOffice | 1064 | 16.54 | TALL | not listed |
| SM_S1D3_LampRoom | 1312 | 11.16 | TALL | not listed |
| SM_S1D3_TankHouse | 1096 | 15.36 | TALL | not listed |
| SM_S1D3_RowHouses | 1488 | 12.2 | TALL | not listed |
| SM_S1D3_Bunkhouse | 1604 | 11.7 | TALL | not listed |
| SM_S1D3_CookHouse | 1396 | 15.36 | TALL | not listed |
| SM_S1D3_AssayShed | 1316 | 13.18 | TALL | not listed |
| SM_S1D3_BandHall | 1568 | 10.59 | TALL | not listed |
| SM_S1D3_CompanyStore | 1320 | 13.48 | TALL | not listed |
| SM_S1D3_SackBarrow | 328 | 1.8 | low | not listed |
| SM_S1D3_PitProps | 612 | 1.37 | low | not listed |
| SM_S1D3_TinBath | 460 | 1.8 | low | not listed |
| SM_S1D3_PickRack | 528 | 1.84 | low | not listed |
| SM_S1D3_WashLine | 616 | 2.9 | TALL | not listed |
| SM_S1D3_NoticeBoard | 348 | 3.34 | TALL | not listed |
| SM_S1D3_SwingSeat | 360 | 2.79 | TALL | not listed |
| SM_S1D3_StiltLarder | 408 | 3.66 | TALL | not listed |
| SM_S1D3_Hopper | 3016 | 46.47 | TALL | not listed |

### s1d4, "The Burning Slopes" = THE CLIFF TEMPLE, 11.1 to 13.9 km

Generator `Content/Python/ft_world_volcano_s1d4.py`. 19 meshes, 14804 triangles together.

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D4_GateHouse | 1192 | 10.53 | TALL | not listed |
| SM_S1D4_PrayerHall | 1572 | 12.54 | TALL | not listed |
| SM_S1D4_StoreTower | 652 | 15.68 | TALL | not listed |
| SM_S1D4_Kitchen | 1184 | 9.46 | TALL | not listed |
| SM_S1D4_Lodge | 1600 | 11.2 | TALL | not listed |
| SM_S1D4_RockShrine | 436 | 8.36 | TALL | not listed |
| SM_S1D4_DrumPavilion | 760 | 9.98 | TALL | not listed |
| SM_S1D4_MoonTerrace | 660 | 7.46 | TALL | not listed |
| SM_S1D4_BellTower | 3008 | 49.85 | TALL | not listed |
| SM_S1D4_LanternRow | 384 | 1.9 | low | not listed |
| SM_S1D4_IncenseUrn | 280 | 1.9 | low | not listed |
| SM_S1D4_PrayerDrums | 756 | 1.95 | low | not listed |
| SM_S1D4_StoneTortoise | 200 | 1.96 | low | not listed |
| SM_S1D4_WaterJars | 680 | 1.7 | low | not listed |
| SM_S1D4_PennantLine | 240 | 8.77 | TALL | not listed |
| SM_S1D4_BannerMast | 280 | 10.32 | TALL | not listed |
| SM_S1D4_MoonArch | 312 | 6.95 | TALL | not listed |
| SM_S1D4_GongFrame | 260 | 4.16 | TALL | not listed |
| SM_S1D4_RobeLine | 348 | 3.2 | TALL | not listed |

### s1d5, "The Ash Fields" = WINDMILL FLATS, 13.9 to 16.7 km

Generator `Content/Python/ft_world_volcano_s1d5.py`. 18 meshes, 15260 triangles together.

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D5_PostMill | 1168 | 11.62 | TALL | not listed |
| SM_S1D5_Barn | 976 | 9.63 | TALL | not listed |
| SM_S1D5_Granary | 828 | 10.3 | TALL | not listed |
| SM_S1D5_Farmhouse | 1236 | 10.38 | TALL | not listed |
| SM_S1D5_SieveHouse | 1016 | 13.7 | TALL | not listed |
| SM_S1D5_SailLoft | 1304 | 8.95 | TALL | not listed |
| SM_S1D5_Dovecote | 1132 | 13.01 | TALL | not listed |
| SM_S1D5_SackRow | 480 | 1.93 | low | not listed |
| SM_S1D5_BasketRack | 580 | 1.91 | low | not listed |
| SM_S1D5_Millstone | 320 | 1.72 | low | not listed |
| SM_S1D5_HandCart | 428 | 1.79 | low | not listed |
| SM_S1D5_QuiltFence | 360 | 1.93 | low | not listed |
| SM_S1D5_WindPump | 372 | 7.3 | TALL | not listed |
| SM_S1D5_ScareFigure | 432 | 3.31 | TALL | not listed |
| SM_S1D5_WindSock | 336 | 7.12 | TALL | not listed |
| SM_S1D5_MillSign | 388 | 5.35 | TALL | not listed |
| SM_S1D5_Bunting | 432 | 3.59 | TALL | not listed |
| SM_S1D5_GrandMill | 3472 | 45.1 | TALL | not listed |

### s1d6, "The Far Works" = GEOTHERM, 16.7 to 19.5 km

Generator `Content/Python/ft_world_volcano_s1d6.py`. 20 meshes, 18076 triangles together.

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D6_ThreeSisters | 2884 | 69.33 | TALL | not listed |
| SM_S1D6_TurbineHall | 1552 | 20.64 | TALL | not listed |
| SM_S1D6_BallTank | 904 | 16.15 | TALL | not listed |
| SM_S1D6_ControlStalk | 768 | 19.24 | TALL | not listed |
| SM_S1D6_SwitchHall | 1292 | 13.63 | TALL | not listed |
| SM_S1D6_ValveHouse | 1512 | 8.86 | TALL | not listed |
| SM_S1D6_SmallTower | 888 | 29.09 | TALL | not listed |
| SM_S1D6_TankFarm | 1260 | 16.37 | TALL | not listed |
| SM_S1D6_Separator | 776 | 18.06 | TALL | not listed |
| SM_S1D6_PipeOffice | 1000 | 11.61 | TALL | not listed |
| SM_S1D6_ValveWheel | 456 | 1.86 | low | not listed |
| SM_S1D6_PipeRings | 648 | 1.92 | low | not listed |
| SM_S1D6_Bollards | 504 | 1.92 | low | 0 / 1 / 27 |
| SM_S1D6_GaugeBoard | 408 | 1.97 | low | not listed |
| SM_S1D6_LowManifold | 604 | 1.7 | low | not listed |
| SM_S1D6_WhistleMast | 336 | 9.53 | TALL | not listed |
| SM_S1D6_PipeLoop | 712 | 7.57 | TALL | not listed |
| SM_S1D6_ChevronGantry | 492 | 8.25 | TALL | not listed |
| SM_S1D6_PipeTrestle | 648 | 5.98 | TALL | not listed |
| SM_S1D6_StiltDrum | 432 | 6.24 | TALL | not listed |

### s1d7, "The Inner Wall" = THE WALL KEEP, 19.5 to 22.3 km

Generator `Content/Python/ft_world_volcano_s1d7.py`. 16 meshes, 12788 triangles together.

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D7_GateTower | 1312 | 22.07 | TALL | not listed |
| SM_S1D7_CurtainWall | 696 | 16.2 | TALL | not listed |
| SM_S1D7_CornerTower | 672 | 29.27 | TALL | not listed |
| SM_S1D7_GreatHall | 972 | 18.85 | TALL | not listed |
| SM_S1D7_Granary | 640 | 16.25 | TALL | not listed |
| SM_S1D7_BellCote | 544 | 26.9 | TALL | not listed |
| SM_S1D7_WellBastion | 892 | 15.34 | TALL | not listed |
| SM_S1D7_BannerPole | 284 | 9.46 | TALL | not listed |
| SM_S1D7_ShieldBoard | 476 | 5.89 | TALL | not listed |
| SM_S1D7_ScaffoldArch | 524 | 8.4 | TALL | not listed |
| SM_S1D7_PennantLine | 576 | 7.42 | TALL | not listed |
| SM_S1D7_HorseTrough | 336 | 1.8 | low | 0 / 1 / 18 |
| SM_S1D7_StoneBalls | 408 | 1.84 | low | not listed |
| SM_S1D7_MarketStall | 700 | 1.87 | low | 0 / 1 / 18 |
| SM_S1D7_Barrels | 756 | 1.75 | low | not listed |
| SM_S1D7_CrownKeep | 3000 | 57.5 | TALL | not listed |

### s1d8, "The Last Ash" = THE BACKLOT, 22.3 to 25 km

Generator `Content/Python/ft_world_volcano_s1d8.py`. 19 meshes, 12640 triangles together.

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D8_StreetFront | 1152 | 11.0 | TALL | not listed |
| SM_S1D8_PalaceFront | 1168 | 18.26 | TALL | not listed |
| SM_S1D8_StudioBarn | 512 | 9.81 | TALL | not listed |
| SM_S1D8_LighthouseFront | 612 | 18.95 | TALL | not listed |
| SM_S1D8_CostumeStore | 1060 | 7.72 | TALL | not listed |
| SM_S1D8_PaintShop | 820 | 7.04 | TALL | not listed |
| SM_S1D8_WaterTower | 792 | 14.58 | TALL | not listed |
| SM_S1D8_TownHallFront | 860 | 16.0 | TALL | not listed |
| SM_S1D8_PaintTins | 888 | 1.93 | low | 0 / 1 / 21 |
| SM_S1D8_ChairRow | 692 | 1.88 | low | 0 / 2 / 20 |
| SM_S1D8_CableCart | 412 | 1.9 | low | 0 / 1 / 21 |
| SM_S1D8_PropCutouts | 224 | 1.9 | low | not listed |
| SM_S1D8_CostumeRail | 248 | 1.93 | low | not listed |
| SM_S1D8_LampStand | 352 | 4.72 | TALL | not listed |
| SM_S1D8_Reflector | 212 | 4.09 | TALL | not listed |
| SM_S1D8_BackdropRoll | 376 | 6.28 | TALL | not listed |
| SM_S1D8_StarArch | 708 | 7.88 | TALL | not listed |
| SM_S1D8_PaintCradle | 772 | 5.22 | TALL | not listed |
| SM_S1D8_BigSlate | 780 | 32.01 | TALL | not listed |

### s1plants, whole road

Generator `Content/Python/ft_world_volcano_s1plants.py`. 21 meshes, 15814 triangles together.

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1PLANTS_MossCushion | 590 | 0.57 | low | not listed |
| SM_S1PLANTS_AshFern | 648 | 0.46 | low | 0 / 1 / 60 |
| SM_S1PLANTS_Horsetail | 1272 | 3.2 | TALL | not listed |
| SM_S1PLANTS_TreeFern | 1280 | 5.32 | TALL | not listed |
| SM_S1PLANTS_LemonTub | 418 | 4.86 | TALL | not listed |
| SM_S1PLANTS_GlassThorn | 868 | 1.05 | low | not listed |
| SM_S1PLANTS_Fireweed | 728 | 1.88 | low | not listed |
| SM_S1PLANTS_Tussock | 736 | 1.26 | low | not listed |
| SM_S1PLANTS_BentPine | 590 | 7.05 | TALL | not listed |
| SM_S1PLANTS_BentPineL | 590 | 7.05 | TALL | not listed |
| SM_S1PLANTS_Bamboo | 1436 | 7.33 | TALL | not listed |
| SM_S1PLANTS_PlumBlossom | 520 | 6.07 | TALL | not listed |
| SM_S1PLANTS_AshSunflower | 832 | 2.48 | TALL | not listed |
| SM_S1PLANTS_PinkThrift | 448 | 0.81 | low | not listed |
| SM_S1PLANTS_Willow | 842 | 6.41 | TALL | not listed |
| SM_S1PLANTS_ReedBed | 584 | 2.84 | TALL | not listed |
| SM_S1PLANTS_BirchTub | 960 | 6.05 | TALL | not listed |
| SM_S1PLANTS_BoxTub | 366 | 1.28 | low | 0 / 1 / 38 |
| SM_S1PLANTS_Chestnut | 944 | 10.18 | TALL | not listed |
| SM_S1PLANTS_PropPalm | 562 | 4.72 | TALL | not listed |
| SM_S1PLANTS_Dandelion | 600 | 1.89 | low | not listed |

### s1animals, whole road

Generator `Content/Python/ft_world_volcano_s1animals.py`. 19 meshes, 23854 triangles together.

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1ANIMALS_Egrets | 1156 | 1.92 | low | not listed |
| SM_S1ANIMALS_Geese | 1200 | 1.85 | low | not listed |
| SM_S1ANIMALS_Frogs | 1502 | 1.79 | low | not listed |
| SM_S1ANIMALS_Hens | 1436 | 1.91 | low | not listed |
| SM_S1ANIMALS_KoiBasin | 764 | 1.43 | low | not listed |
| SM_S1ANIMALS_Peacocks | 1380 | 1.87 | low | not listed |
| SM_S1ANIMALS_ShardLizard | 864 | 1.91 | low | not listed |
| SM_S1ANIMALS_StudioPets | 1160 | 1.94 | low | not listed |
| SM_S1ANIMALS_SpringMonkeys | 1344 | 6.4 | TALL | not listed |
| SM_S1ANIMALS_PitPonies | 1156 | 1.98 | low | not listed |
| SM_S1ANIMALS_PackBeasts | 2028 | 3.61 | TALL | not listed |
| SM_S1ANIMALS_CurlHornSheep | 1564 | 2.56 | TALL | not listed |
| SM_S1ANIMALS_GreatTortoise | 1300 | 4.37 | TALL | not listed |
| SM_S1ANIMALS_WhiteHorse | 1104 | 3.15 | TALL | not listed |
| SM_S1ANIMALS_Cranes | 1224 | 3.68 | TALL | not listed |
| SM_S1ANIMALS_CraneSkein | 910 | 4.25 | TALL | not listed |
| SM_S1ANIMALS_PinkWaders | 1386 | 2.45 | TALL | not listed |
| SM_S1ANIMALS_MothCloud | 1440 | 5.38 | TALL | not listed |
| SM_S1ANIMALS_GlassIguanas | 936 | 4.65 | TALL | not listed |

### s1people, whole road (no hand-over text reached the integrator; block header and make log only)

Generator `Content/Python/ft_world_volcano_s1people.py`. 25 meshes, 33934 triangles together.

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1PEOPLE_BathDeck | 1424 | 6.14 | TALL | not listed |
| SM_S1PEOPLE_TeaTerrace | 1446 | 6.31 | TALL | not listed |
| SM_S1PEOPLE_BalconyWatch | 1308 | 7.28 | TALL | not listed |
| SM_S1PEOPLE_StormGallery | 1640 | 8.22 | TALL | not listed |
| SM_S1PEOPLE_KilnMouth | 1252 | 6.21 | TALL | not listed |
| SM_S1PEOPLE_RiggerFrame | 1468 | 11.18 | TALL | not listed |
| SM_S1PEOPLE_ShiftWalk | 1380 | 7.5 | TALL | not listed |
| SM_S1PEOPLE_CookVeranda | 1368 | 7.14 | TALL | not listed |
| SM_S1PEOPLE_CardStair | 1392 | 8.44 | TALL | not listed |
| SM_S1PEOPLE_SweepSteps | 1208 | 6.97 | TALL | not listed |
| SM_S1PEOPLE_YokeStair | 1200 | 7.78 | TALL | not listed |
| SM_S1PEOPLE_BellTerrace | 1232 | 7.32 | TALL | not listed |
| SM_S1PEOPLE_LoadingStage | 1180 | 9.14 | TALL | not listed |
| SM_S1PEOPLE_SailMender | 1288 | 6.99 | TALL | not listed |
| SM_S1PEOPLE_RibbonStair | 1344 | 7.01 | TALL | not listed |
| SM_S1PEOPLE_GaugeDeck | 1660 | 6.4 | TALL | not listed |
| SM_S1PEOPLE_ValveCrew | 1456 | 6.68 | TALL | not listed |
| SM_S1PEOPLE_TeaLanding | 1648 | 5.65 | TALL | not listed |
| SM_S1PEOPLE_HeraldWalk | 1224 | 7.6 | TALL | not listed |
| SM_S1PEOPLE_MarketSteps | 1344 | 6.03 | TALL | not listed |
| SM_S1PEOPLE_MasonScaffold | 1180 | 8.27 | TALL | not listed |
| SM_S1PEOPLE_PainterCradle | 1328 | 8.21 | TALL | not listed |
| SM_S1PEOPLE_Carpenters | 1208 | 5.8 | TALL | not listed |
| SM_S1PEOPLE_ExtrasQueue | 1668 | 7.3 | TALL | not listed |
| SM_S1PEOPLE_LavaPier | 1088 | 8.4 | TALL | not listed |

## Left out

Nothing. All 11 blocks are in; no block was edited apart from its wrapper.

## Not checked

- Placed counts per mesh for the 191 meshes FTCLEAR does not list; whether every Set and rule placed what its
  builder estimated (only the level's totals were read).
- Frame time. The 32 hitch lines were not run down; no second run, no profile.
- Anything by eye beyond one frame at 6 s: no district past the start was seen, no hero landmark, no movement, no
  colour under this level's light, no check that a piece hides a lava channel, the eruption or a signature view.
- The rules that protect play (17 m / 24 m, light distances, nothing like a pickup, enemy or the boss): read by nobody
  here; that is the review's job. The blocks' own headers state their distances; they were not re-derived.
- The s1people builder's hand-over text (not received); its 25 meshes' numbers are from its make log.
- The generators were not read line by line and were not re-run.
- Other levels; a packaged build.

## Review and fix round (16 Nov 2026, one round)

Two reviews: a rules reader (table and generators, computed) and a driver (8 runs, 0.08 to 12.78 km seen). Everything
below is what the fixer ran and saw. The fixed pieces were NOT seen in the game (one start, one frame, see the end).

### Found

- Rules reader: none of the five play rules broken by the numbers. Three "looks wrong" candidates (people on piers in
  the lava channel; new pieces whose boxes run into older ones; set parts that may hang over a jump's dip) and eleven
  "weak" points.
- Driver: 13.9 to 25.0 km (districts 5 to 8, four heroes) cannot be reached in the game as driven: the boss fight
  starts at 8.34 km and the level ends at 10.3 to 12.8 km. The moon arch (a pale ring on thin legs, 35 m out, both
  sides, 11.4 to 12.8 km) could be taken for the boss's ring marks. Five pale hourglass shapes hang in the sky on the
  left at 3.40 and 3.55 km. Lime moss on bare ground reads as slime. The gong reads as a bull's-eye.

### Fixed

| what | how | where |
|---|---|---|
| Moon arch could be taken for an attack ring | The mesh is now a moon GATE: the upper half of the arch on two solid square piers joined by a stone sill, jade cap and tassel kept. `FTPEOPLE piece SM_S1D4_MoonArch triangles 216 parts 8 size m 5.7 1.0 top m 7.15 TALL`. Same 19 places (35.3 / 35.7 m) | `ft_world_volcano_s1d4.py` moon_arch |
| People standing in the lava channel | The six LavaPier zones and their Set are out. The mesh stays in the folder, unplaced | s1people block |
| People rows beside a jump's landing or run-up | The four right-hand rows at slots 17, 33, 473, 537 are out | s1people block |
| People pieces inside the bend 2.95 to 3.30 km | The left row at slot 73 is out | s1people block |
| s1people over its guide | Every row of 5 is a row of 3 (the two outer parts at +-13.2 m taken out of the 13 n5 sets). Asked 385 -> 318 (re-run of the reviewer's `a2.py`) | s1people block |
| Cinder Springs houses backing into the pass 3 derricks, signals and arch (0.63 to 2.08 km left) | The left streets past the fire and in lava jumps stand at 61.5 m instead of 65 m (5 rules) | s1d1 block |
| Changing towers standing in a watch deck (0.81 km right) | RowB on the right is every 24, phase 18 (slots 6 and 30) instead of every 12, phase 6: slots 18 and 42 lose their three houses | s1d1 block |
| Stormwatch yards in the pass 3 crags and headframe (5.6 to 6.25 km left) and the drum hall in a pass 3 piece (6.24 km right) | Magma run, left yards 63.4 -> 61.8 m (4 rules); right yards 67.6 -> 69.1 m (2 rules) | s1d2 block |
| Temple kitchen and store tower backing into the goats (11.57, 12.48, 12.50 km left) | The Yard set stands at 59.6 m instead of 62.0 m | s1d4 block |
| Small tower in the ropeway (19.27 km right) | Its band is 75.8 to 76.8 m instead of 77 to 78 m | s1d6 block |
| Pack beasts in the whim (12.79 km), monkeys' pool in the ore tubs (1.94 km), white horse in the ore tubs (24.83 km) | Those three rules' bands moved nearer: 72 to 76, 76 to 80, 72 to 76 m | s1animals block |
| One mesh on both sides of the 70 m shadow line | s1d1 rows 70 -> 69.5 m (right) and 69.95 m (left, caldera: at 69.5 the laundry and pump room touched a watch tower at 64.6 m); s1d2 right rows 69.6 -> 69.1 m; s1d5 mill rows 70 -> 69.5 m | three blocks |
| Steam Clock and Storm Harp stop moving past 180 m | Both saved `far=True` and re-made (triangles and size unchanged: 3120 / 40.94 m, 3068 / 57.28 m) | s1d1, s1d2 generators |
| Moss reads as spilt slime | The five lumps under the cushions are pale stone, wider than the cushions and 22 to 30 cm proud; cushions smaller, on top. `SM_S1PLANTS_MossCushion triangles 590 parts 22 size m 5.8 1.7 top m 0.73 low` (was 0.57) | `ft_world_volcano_s1plants.py` moss |
| Gong reads as a bull's-eye | The dark centre is a small pale boss. `SM_S1D4_GongFrame triangles 260 ... top m 4.16` | `ft_world_volcano_s1d4.py` gong_frame |

How the table was changed: outside the s1people block (the last one) every change is an edit of an existing rule line,
so the number and order of rules before the people block is the same and no random draw of any other rule moved.
Checked after the change with the reviewer's own scripts (`C:\ftwork\jobs\L3s1rev\rules\a1..a4.py`, output in
`C:\ftwork\out\L3s1fix\`): no low piece nearer than 17.2 m (box edge), no tall piece nearer than 24.3 m, no mesh on
both sides of 70 m, nothing under 40 m in a banked bend, and the overlap list `a4.txt` went from 40 lines to 20 with
no new line. The make log is `C:\ftwork\logs\L3s1fix_make.log` (5 pieces built, 0 failed, 0 `LogPython: Error`).

### Not fixed, and why

- **13.9 to 25 km cannot be reached** (driver 1): not in Stage 1's files; the owner decides.
- **Five pale hourglass shapes in the sky at 3.40 / 3.55 km** (driver 3): the cause was not verified. In the crop
  they are a group of three (large, taller, small) and a group of two or three, which fits two of the three
  ThreeSisters placements (17.03, 17.67, 18.57 km), not the SmallTower (single, a slot apart). The track never culls
  a mesh whose bounds radius x 1.3 is over 40 m (`FTTrack.cpp` 1941), which is every hero; the table has no word for
  a cull distance. To make the Sisters cull they would have to be scaled to about 0.6 (towers 34, 29, 24 m, the
  SmallTower's size) and would then draw to 4 km, which may still reach. That takes the landmark away on a guess, so
  it was left: a decision for the lead.
- **Overlaps left** (20 lines in `C:\ftwork\out\L3s1fix\a4.txt`): slivers under 2 m along the road or 1 m deep
  (PackBeasts / PitPonies and the mule trains, Hopper and a handcar 1.3 m, GateTower and a derrick 0.9 x 0.8 m,
  StoreTower 0.6 m, Bunting 0.6 m, PipeTrestle 0.5 m, WindPump 1.1 x 1.3 m); plants whose crowns reach over an older
  piece's box (9 lines); the PostMill over a basalt wall's centre at 14.74 and 16.11 km right (the wall's size is in
  no log); the PipeOffice astride the railway (on purpose); the Storm Harp and a horizon kettle 60 m behind it. Each
  needs one slot taken out of a many-slot rule, which means new rules, and a new rule re-rolls every later Scatter.
- **Parts that may hang over a jump's dip** (rules reader 3, 15 set-slots; the people one at slot 33 is gone with
  its row): not fixed. In the placer (`FTTrack.cpp`, the Site branch) a part stands at the anchor's ground plus the
  land profile at its own distance; whether the land or a lake dips there was not checked, and each needs a single
  slot taken out.
- **Crane skein from 11.26 km and moths at 3.85 to 6.47 km** (rules reader 6, 7): the plan says both things
  (section 0 keeps the sky clear, section 4 orders these two pieces). Left as built: a decision for the lead.
- **Valve wheel as a pickup** (rules reader 8): left. `POWERUPS.md` describes the pickups as a crate and a toolbox;
  the wheel was not seen in the game by anyone (17 km).
- **Tall props in front of older crowds** (9), **districts 4 and 5 a little over 250** (12), **buildings past 70 m in
  districts 3 and 6** (14), **Slagtown's right side hard to read** (driver 6), **left 7.25 to 7.85 km thin** (driver 7):
  not small changes; left.
- **Second Steam Clock at 3.35 km not found by the driver** (driver 8): the rule is there (slot 74, 138 m left); whether
  the track placed it was not checked.

### Compile and the one start

- `C:\ftwork\logs\L3s1_fix_build.log`: `build exit 0`, `Result: Succeeded`.
- `C:\ftwork\out\s1\fixed_L3\run.log`: `FTTRACK level=3 frames=12497 route=506786e7 solid=eead12ba
  solidTriangles=900592`, equal to the `^FTTRACK level=3 ` line of `track_baseline.txt` (compared by hand; the tool
  printed level 0's).
- `FTDRESS level=3 districts=858 sets=197 pieces placed=14381 of 14436 tried shadows=5070 of 14381` (before the fix
  14450 of 14505, shadows 5139, districts 869, sets 198). `FTCLEAR props kept=16755 (moved off their own barrier 364)
  left out of the road=55`. `FTWPO life components=302 of 379 stop moving past 18000 cm`.
- `Fatal error` 0, `LogFT: Error` 0, `Error: Ensure` 0, no `/Game/World` load failure line.
- THE RUN WAS SLOW AND ITS FRAME IS NOT AT 6 s: `waitfor="FT CLOCK level=6.0" result=timeout`. The level ran at about
  9 frames a second for 145 s of level clock (1355 FTHITCH2 lines on L_Volcano, median 114 ms, loads=0 gc=0), the
  clock never printed 6.0, and the one frame `L3_t006.png` was taken at 2:19 in scene 5 (the magma run). The title map
  in the same run made 315 frames in 6 s. Three other levels' game runs were writing frames on this PC during those
  150 s (L5, L6, L7 folders under `C:\ftwork\out`; one of their logs has 1686 hitch lines), against two during the
  run before the fix. So this says nothing sure about the fix, and it was NOT separated: the level was started once.

### Not checked

- Anything by eye: no fixed piece was seen in the game (the moon gate, the gong, the moss, the moved houses, the
  two heroes moving past 180 m). The one frame shows the magma run and none of them.
- Frame time (see above). 13.9 to 25 km. The cause of the shapes in the sky.
- The reviewer's simulation of the track's placer is what the distances and overlaps above rest on; the track's real
  flags (jump run-ups, landings, lake zones) were not read out of the game.
- The driver's contact sheet `C:\ftwork\out\L3s1rev\L3_s1_review.jpg` is 1,278,622 bytes (over 1 MB): not copied.
- Re-running the reviewer's `a1.py` rewrote his scratch file `C:\ftwork\out\L3s1rev\per_mesh.txt` with the values
  after the fix (the script writes there itself); the table before the fix is kept as
  `C:\ftwork\jobs\L3s1fix\FTPeopleL3.before.inl`.
