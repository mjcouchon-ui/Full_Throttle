# Level 7 "City Center": Stage 1, the world (16 Nov 2026)

What the integrator ran and saw. Branch `claude/world-L7`. Nothing here was reviewed in frames.

## Zone plan in short (C:\ftwork\final\s1\L7_plan.md)

| builder | district | km | look |
|---|---|---|---|
| s1d1 | Fifth and Main | 0.00 to 1.10 | old shopping street, red and cream; hero The Percolator |
| s1d2 | Theatre Row | 1.10 to 2.10 | playhouses, magenta and plum; hero The Grand Curtain |
| s1d3 | The Garages | 2.10 to 3.13 | motor quarter, blue and white; hero The Corkscrew Stack |
| s1d4 | The Plaza | 3.13 to 4.62 | civic park, teal, white and green; hero The Orrery |
| s1d5 | Wall Street | 4.62 to 6.29 | money district, sandstone and copper green; hero The Great Scales |
| s1d6 | Penthouse Row | 6.29 to 7.66 | white terraces, blush and pool cyan; hero The Cascade |
| s1d7 | Syndicate Territory, the arena | 7.66 to 13.56 | black and gold fortress blocks, all still; hero The Vault |
| s1d8 | Syndicate Territory, the circuit | 13.56 to the end (25.44) | steel works, cold cyan, all still; hero The Hourglass |
| s1plants | plants, one zone a district | whole road | slots s % 4 == 3 |
| s1animals | animals, ground, basins, pale doves far out | whole road | slots s % 8 == 5 and far bands |
| s1people | raised people in the gaps pass 3 left | whole road | slots s % 8 == 1 |

## What was done

- All 11 block files `C:\ftwork\s1\L7_*.inl` were pasted, each wrapped in `{ }`, in the order above, at the END of
  `Source/FullThrottle/FTPeopleL7.inl` (after the p3f block), unchanged. None left out.
- Checked before pasting, per block: its generator `Content/Python/ft_world_city_<id>.py` exists; its make log
  (`C:\ftwork\logs\L7<id>_make.log`) ends "pieces built N failed 0" with 0 lines "LogPython: Error" or "FAILED"; every mesh name
  in the block exists as a `.uasset` in `Content/World/CityCenter`; no Set name is used twice across the blocks; every Set is `WldS1...`.
- Compile (`build.ps1`, tag L7s1_build): "build exit 0 after 58 s", "Result: Succeeded". No change to any block was needed.
- Level started once (`run_game.ps1`, script `C:\ftwork\jobs\p3\start_L7.txt`, out `C:\ftwork\out\s1\after_L7`): "exit 0 after 52 s", 1 frame.

## Counts, before and after (run.log)

| | FTDRESS level=7 |
|---|---|
| before (`C:\ftwork\out\p3\final_L7\run.log`) | districts=154 sets=268 pieces placed=10018 of 10073 tried shadows=3678 of 10162 |
| after (`C:\ftwork\out\s1\after_L7\run.log`) | districts=225 sets=459 pieces placed=12573 of 12629 tried shadows=5218 of 12717 |

- Placed: +2555 (tried +2556). Shadow casters: +1540.
- Track: `FTTRACK level=7 frames=12717 route=5faaeb1d solid=25fa2576 solidTriangles=957884`, equal to the `^FTTRACK level=7 ` line of
  `SourceArt/Jam/validate/track_baseline.txt` (compared as strings).
- "Fatal error": 0. "LogFT: Error": 0. Lines with `/Game/World` and a load failure: 0. The only two "Error:" lines in the log are the
  editor's own HTTP listener failing to bind port 8000 (LogHttpListener, LogModelContextProtocol).
- FTCLEAR level=7: props kept=16248 (moved off their own barrier 2018) left out of the road=65 pillars left out=0. FTCLEAR names a mesh
  only when at least one of its placements was moved or left out: 80 of the 194 new meshes are named (1 placement left out:
  SM_S1D6_HatBox; 212 moved; 1216 placed in all on those 80). The other 114 are not named, so their placed counts are not in the log.
- FTWPO level=7: life components=308 of 382 stop moving past 18000 cm.

## New meshes: 194, 192,850 triangles in all (from the make logs; the builders' hand-overs copy the same lines)

Columns: triangles / top m / TALL or low as the make log printed / FTCLEAR "left out / moved / placed" where the mesh is named ("-" = not named).
Over the 2,700 guide: the heroes GrandCurtain 3872, CorkscrewStack 3932, Orrery 3664, GreatScales 3184, Cascade 3564, Vault 2848, Hourglass 3660
(hero guide 4,000), and SM_S1PEOPLE_StandCrates 3044, SM_S1PEOPLE_ChoirSteps 2760.

### s1d1 (ft_world_city_s1d1.py): 18 meshes, 18560 triangles

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1D1_CornerStore | 1476 | 84.5 | TALL | - |
| SM_S1D1_WalkUp | 1084 | 70.71 | TALL | 0 / 1 / 28 |
| SM_S1D1_HatHouse | 1088 | 70.46 | TALL | - |
| SM_S1D1_Bakery | 1188 | 66.13 | TALL | - |
| SM_S1D1_ToyEmporium | 1320 | 75.04 | TALL | - |
| SM_S1D1_ArcadeHall | 1576 | 63.2 | TALL | - |
| SM_S1D1_GableHouse | 1152 | 81.15 | TALL | 0 / 1 / 14 |
| SM_S1D1_BarberClock | 604 | 6.81 | TALL | 0 / 3 / 11 |
| SM_S1D1_HatSign | 556 | 16.46 | TALL | - |
| SM_S1D1_CupSign | 516 | 14.97 | TALL | 0 / 4 / 9 |
| SM_S1D1_Bunting | 876 | 15.28 | TALL | 0 / 3 / 7 |
| SM_S1D1_WaterTank | 544 | 16.92 | TALL | 0 / 1 / 4 |
| SM_S1D1_ShoeShine | 476 | 3.6 | TALL | 0 / 2 / 4 |
| SM_S1D1_FruitStall | 892 | 1.93 | low | 0 / 2 / 14 |
| SM_S1D1_Boards | 828 | 1.05 | low | 0 / 2 / 14 |
| SM_S1D1_Tricycles | 1008 | 1.53 | low | 0 / 2 / 14 |
| SM_S1D1_HatboxCart | 888 | 1.7 | low | 0 / 2 / 10 |
| SM_S1D1_Percolator | 2488 | 112.1 | TALL | - |

### s1d2 (ft_world_city_s1d2.py): 16 meshes, 18792 triangles

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1D2_PuppetHouse | 1308 | 60.8 | TALL | 0 / 7 / 10 |
| SM_S1D2_RehearsalLoft | 1340 | 58.0 | TALL | 0 / 7 / 13 |
| SM_S1D2_CostumeHouse | 1480 | 76.26 | TALL | - |
| SM_S1D2_DanceDrum | 1382 | 77.04 | TALL | - |
| SM_S1D2_MusicHall | 1404 | 97.76 | TALL | - |
| SM_S1D2_FlyTower | 1250 | 97.6 | TALL | - |
| SM_S1D2_MaskMast | 1008 | 55.2 | TALL | 0 / 7 / 11 |
| SM_S1D2_Chandelier | 1156 | 51.54 | TALL | 0 / 4 / 5 |
| SM_S1D2_TicketMast | 1012 | 53.49 | TALL | 0 / 3 / 5 |
| SM_S1D2_MoonFlats | 360 | 5.6 | TALL | 0 / 1 / 3 |
| SM_S1D2_VelvetBenches | 618 | 1.58 | low | 0 / 1 / 1 |
| SM_S1D2_StageTrunks | 736 | 1.91 | low | - |
| SM_S1D2_SpotPlanters | 828 | 1.9 | low | - |
| SM_S1D2_HatBoxKiosk | 490 | 1.95 | low | - |
| SM_S1D2_PosterDrums | 548 | 1.87 | low | - |
| SM_S1D2_GrandCurtain | 3872 | 101.08 | TALL | - |

### s1d3 (ft_world_city_s1d3.py): 20 meshes, 18120 triangles

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1D3_CorkscrewStack | 3932 | 117.0 | TALL | - |
| SM_S1D3_DeckGarage | 2368 | 99.4 | TALL | - |
| SM_S1D3_BodyShop | 1084 | 57.3 | TALL | - |
| SM_S1D3_TyreDepot | 1780 | 63.4 | TALL | - |
| SM_S1D3_CarWash | 740 | 61.64 | TALL | - |
| SM_S1D3_ElevatorTower | 636 | 101.2 | TALL | - |
| SM_S1D3_PaintShop | 1324 | 65.8 | TALL | - |
| SM_S1D3_RampDrum | 1640 | 59.65 | TALL | - |
| SM_S1D3_PayBooth | 280 | 5.63 | TALL | - |
| SM_S1D3_TyreStack | 628 | 1.25 | low | - |
| SM_S1D3_BollardRow | 420 | 1.02 | low | - |
| SM_S1D3_HoistFrame | 408 | 5.43 | TALL | - |
| SM_S1D3_OilDrums | 768 | 1.74 | low | - |
| SM_S1D3_HeightGantry | 244 | 5.66 | TALL | - |
| SM_S1D3_BaySign | 168 | 6.92 | TALL | - |
| SM_S1D3_ToolCart | 244 | 1.22 | low | - |
| SM_S1D3_ArrowSign | 240 | 15.4 | TALL | - |
| SM_S1D3_DeckLamps | 488 | 11.07 | TALL | - |
| SM_S1D3_CarStack | 440 | 9.3 | TALL | - |
| SM_S1D3_CarVane | 288 | 15.94 | TALL | - |

### s1d4 (ft_world_city_s1d4.py): 16 meshes, 19708 triangles

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1D4_Bandstand | 1760 | 8.18 | TALL | - |
| SM_S1D4_TeaPavilion | 1430 | 9.38 | TALL | - |
| SM_S1D4_CarouselHouse | 2144 | 8.51 | TALL | - |
| SM_S1D4_FountainCourt | 1580 | 5.79 | TALL | - |
| SM_S1D4_PalmHouse | 1356 | 16.92 | TALL | - |
| SM_S1D4_Library | 1244 | 14.64 | TALL | - |
| SM_S1D4_Belvedere | 1344 | 17.37 | TALL | - |
| SM_S1D4_Orrery | 3664 | 120.65 | TALL | - |
| SM_S1D4_GlobeLamps | 556 | 7.01 | TALL | 0 / 3 / 28 |
| SM_S1D4_Sundial | 454 | 3.18 | TALL | - |
| SM_S1D4_BenchRing | 512 | 5.4 | TALL | - |
| SM_S1D4_BoatRack | 416 | 4.2 | TALL | - |
| SM_S1D4_FountainJets | 542 | 5.37 | TALL | - |
| SM_S1D4_Pergola | 1348 | 3.53 | TALL | - |
| SM_S1D4_UrnBalustrade | 764 | 1.88 | low | - |
| SM_S1D4_YachtPond | 594 | 1.67 | low | - |

### s1d5 (ft_world_city_s1d5.py): 20 meshes, 18194 triangles

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1D5_GreatScales | 3184 | 131.2 | TALL | - |
| SM_S1D5_PostTower | 1352 | 79.87 | TALL | 0 / 2 / 7 |
| SM_S1D5_CountingTower | 1032 | 87.2 | TALL | 0 / 1 / 8 |
| SM_S1D5_VaultDrum | 1228 | 72.72 | TALL | 0 / 2 / 8 |
| SM_S1D5_LanternTower | 1000 | 76.09 | TALL | 0 / 1 / 7 |
| SM_S1D5_ExchangeTower | 1768 | 103.58 | TALL | - |
| SM_S1D5_CoinPressHall | 1652 | 93.4 | TALL | - |
| SM_S1D5_ClerksBlock | 1252 | 91.8 | TALL | - |
| SM_S1D5_ZigguratMinor | 1224 | 104.0 | TALL | - |
| SM_S1D5_CopperCupola | 280 | 4.56 | TALL | - |
| SM_S1D5_VentCowls | 380 | 2.98 | TALL | 0 / 1 / 5 |
| SM_S1D5_TeaCanopy | 582 | 2.67 | TALL | 0 / 1 / 4 |
| SM_S1D5_BellArch | 292 | 4.79 | TALL | 0 / 1 / 4 |
| SM_S1D5_ParapetUrns | 592 | 1.54 | low | 0 / 3 / 19 |
| SM_S1D5_PennantMast | 216 | 6.59 | TALL | - |
| SM_S1D5_UrnRow | 432 | 1.59 | low | 0 / 1 / 8 |
| SM_S1D5_TapeBench | 372 | 1.51 | low | 0 / 1 / 4 |
| SM_S1D5_LedgerCart | 556 | 1.51 | low | 0 / 1 / 4 |
| SM_S1D5_PineappleSteps | 284 | 1.78 | low | 0 / 1 / 9 |
| SM_S1D5_StreetClock | 516 | 5.76 | TALL | 0 / 2 / 4 |

### s1d6 (ft_world_city_s1d6.py): 16 meshes, 13264 triangles

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1D6_TerraceTower | 1280 | 95.0 | TALL | - |
| SM_S1D6_FinTower | 736 | 104.0 | TALL | - |
| SM_S1D6_LanternTower | 996 | 104.16 | TALL | - |
| SM_S1D6_PoolBlock | 992 | 55.4 | TALL | 0 / 8 / 12 |
| SM_S1D6_WinterGarden | 728 | 72.35 | TALL | - |
| SM_S1D6_HatBox | 708 | 51.73 | TALL | 1 / 3 / 7 |
| SM_S1D6_Observatory | 812 | 69.56 | TALL | - |
| SM_S1D6_Cascade | 3564 | 126.0 | TALL | - |
| SM_S1D6_Canopy | 468 | 3.62 | TALL | - |
| SM_S1D6_Luggage | 628 | 1.77 | low | - |
| SM_S1D6_Loungers | 460 | 1.86 | low | - |
| SM_S1D6_Piano | 288 | 1.88 | low | - |
| SM_S1D6_CakeFountain | 684 | 6.05 | TALL | - |
| SM_S1D6_LiftKiosk | 376 | 5.32 | TALL | - |
| SM_S1D6_PlungePool | 272 | 1.7 | low | 0 / 1 / 5 |
| SM_S1D6_Sunfin | 272 | 5.12 | TALL | - |

### s1d7 (ft_world_city_s1d7.py): 16 meshes, 11136 triangles

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1D7_CountingTower | 1192 | 103.6 | TALL | - |
| SM_S1D7_KeepTower | 1248 | 87.0 | TALL | - |
| SM_S1D7_Strongroom | 372 | 14.1 | TALL | 0 / 1 / 18 |
| SM_S1D7_Mint | 564 | 27.85 | TALL | - |
| SM_S1D7_ClerksBastion | 536 | 17.6 | TALL | - |
| SM_S1D7_LedgerArchive | 780 | 53.75 | TALL | 0 / 2 / 41 |
| SM_S1D7_TollHouse | 584 | 40.5 | TALL | - |
| SM_S1D7_LanternCluster | 360 | 5.8 | TALL | 0 / 1 / 28 |
| SM_S1D7_PennantRow | 432 | 5.22 | TALL | 0 / 1 / 18 |
| SM_S1D7_SentryBox | 264 | 3.34 | TALL | - |
| SM_S1D7_RoofVents | 348 | 4.55 | TALL | - |
| SM_S1D7_BollardRow | 396 | 1.0 | low | - |
| SM_S1D7_StrongboxCart | 472 | 1.68 | low | - |
| SM_S1D7_MountingBlock | 188 | 1.27 | low | - |
| SM_S1D7_LedgerTable | 552 | 1.49 | low | - |
| SM_S1D7_Vault | 2848 | 112.8 | TALL | - |

### s1d8 (ft_world_city_s1d8.py): 14 meshes, 14678 triangles

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1D8_Gasholder | 1404 | 26.0 | TALL | 0 / 6 / 24 |
| SM_S1D8_WaterTank | 1292 | 22.96 | TALL | 0 / 1 / 24 |
| SM_S1D8_TramShed | 864 | 13.86 | TALL | 0 / 9 / 26 |
| SM_S1D8_SortingHall | 856 | 19.3 | TALL | 0 / 5 / 26 |
| SM_S1D8_SignalCabin | 648 | 8.82 | TALL | 0 / 1 / 13 |
| SM_S1D8_SwitchHouse | 828 | 9.62 | TALL | 0 / 2 / 12 |
| SM_S1D8_LampMast | 732 | 9.02 | TALL | 0 / 2 / 23 |
| SM_S1D8_HandCrane | 608 | 5.66 | TALL | 0 / 3 / 26 |
| SM_S1D8_VentStack | 706 | 7.4 | TALL | 0 / 3 / 25 |
| SM_S1D8_BufferStop | 720 | 1.86 | low | - |
| SM_S1D8_CableDrums | 608 | 1.9 | low | - |
| SM_S1D8_PipeRack | 952 | 1.94 | low | 0 / 2 / 21 |
| SM_S1D8_YardGate | 800 | 1.95 | low | 0 / 1 / 19 |
| SM_S1D8_Hourglass | 3660 | 120.0 | TALL | - |

### s1plants (ft_world_city_s1plants.py): 24 meshes, 16918 triangles

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1PLANTS_GeraniumRun | 816 | 1.74 | low | - |
| SM_S1PLANTS_CamelliaTubs | 930 | 1.83 | low | 0 / 2 / 8 |
| SM_S1PLANTS_SumacLow | 690 | 1.82 | low | 0 / 8 / 44 |
| SM_S1PLANTS_TulipBed | 1296 | 0.97 | low | - |
| SM_S1PLANTS_BoxHedge | 472 | 1.41 | low | 0 / 2 / 14 |
| SM_S1PLANTS_RoseBed | 796 | 1.04 | low | 0 / 1 / 67 |
| SM_S1PLANTS_ThistlePampas | 844 | 1.83 | low | 0 / 4 / 33 |
| SM_S1PLANTS_HangTrough | 554 | 1.85 | low | 0 / 3 / 10 |
| SM_S1PLANTS_LaurelUrns | 856 | 1.88 | low | 0 / 2 / 9 |
| SM_S1PLANTS_Bramble | 600 | 1.58 | low | 0 / 3 / 31 |
| SM_S1PLANTS_YewDrums | 524 | 1.52 | low | - |
| SM_S1PLANTS_PlaneBox | 546 | 5.37 | TALL | - |
| SM_S1PLANTS_Cherry | 580 | 4.58 | TALL | 0 / 1 / 3 |
| SM_S1PLANTS_SumacTall | 480 | 3.86 | TALL | 0 / 3 / 18 |
| SM_S1PLANTS_IvyTrellis | 552 | 3.56 | TALL | 0 / 1 / 17 |
| SM_S1PLANTS_TomatoRoof | 630 | 2.4 | TALL | - |
| SM_S1PLANTS_CypressUrns | 564 | 4.26 | TALL | 0 / 1 / 1 |
| SM_S1PLANTS_PalmTub | 468 | 5.0 | TALL | 0 / 1 / 2 |
| SM_S1PLANTS_BambooScreen | 1068 | 3.63 | TALL | - |
| SM_S1PLANTS_YewObelisk | 424 | 3.43 | TALL | - |
| SM_S1PLANTS_LimeRow | 624 | 3.64 | TALL | - |
| SM_S1PLANTS_BirchPair | 744 | 5.3 | TALL | 0 / 2 / 16 |
| SM_S1PLANTS_Willow | 644 | 6.81 | TALL | - |
| SM_S1PLANTS_Magnolia | 1216 | 5.04 | TALL | - |

### s1animals (ft_world_city_s1animals.py): 18 meshes, 14482 triangles

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1ANIMALS_SparrowWire | 1220 | 1.77 | low | - |
| SM_S1ANIMALS_ShopCat | 642 | 1.67 | low | - |
| SM_S1ANIMALS_DrayHorse | 1034 | 2.05 | TALL | - |
| SM_S1ANIMALS_Greyhounds | 714 | 1.29 | low | - |
| SM_S1ANIMALS_RaccoonDog | 804 | 1.18 | low | - |
| SM_S1ANIMALS_Gulls | 820 | 3.24 | TALL | - |
| SM_S1ANIMALS_Squirrels | 1008 | 3.6 | TALL | - |
| SM_S1ANIMALS_DucksSwans | 932 | 1.17 | low | 0 / 1 / 3 |
| SM_S1ANIMALS_KoiTrough | 460 | 0.96 | low | 0 / 1 / 4 |
| SM_S1ANIMALS_Flamingos | 748 | 1.65 | low | - |
| SM_S1ANIMALS_Peacock | 612 | 1.71 | low | - |
| SM_S1ANIMALS_SynDogs | 880 | 1.54 | low | - |
| SM_S1ANIMALS_FoxPipe | 1154 | 1.59 | low | 0 / 2 / 25 |
| SM_S1ANIMALS_Heron | 592 | 1.76 | low | 0 / 2 / 25 |
| SM_S1ANIMALS_YardCats | 832 | 1.9 | low | 0 / 2 / 25 |
| SM_S1ANIMALS_Owls | 902 | 1.28 | low | - |
| SM_S1ANIMALS_Doves_A | 658 | 4.07 | TALL | - |
| SM_S1ANIMALS_Doves_B | 470 | 1.55 | low | - |

### s1people (ft_world_city_s1people.py): 16 meshes, 28998 triangles

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1PEOPLE_PressRiser | 1778 | 4.2 | TALL | 0 / 4 / 28 |
| SM_S1PEOPLE_WallSitters | 1840 | 4.0 | TALL | 0 / 4 / 28 |
| SM_S1PEOPLE_PlinthBarrel | 456 | 3.25 | TALL | 0 / 4 / 28 |
| SM_S1PEOPLE_UmpireChair | 576 | 3.89 | TALL | 0 / 4 / 28 |
| SM_S1PEOPLE_NewsStack | 504 | 3.29 | TALL | - |
| SM_S1PEOPLE_StandCrates | 3044 | 5.36 | TALL | 0 / 1 / 10 |
| SM_S1PEOPLE_StandLadders | 2286 | 4.06 | TALL | 0 / 10 / 25 |
| SM_S1PEOPLE_PassQueue | 2116 | 3.14 | TALL | 0 / 7 / 28 |
| SM_S1PEOPLE_GirderLunch | 2616 | 4.02 | TALL | 0 / 6 / 15 |
| SM_S1PEOPLE_TaiChi | 1516 | 4.36 | TALL | - |
| SM_S1PEOPLE_StoryTerrace | 2460 | 3.92 | TALL | - |
| SM_S1PEOPLE_EaselPergola | 1956 | 4.94 | TALL | - |
| SM_S1PEOPLE_ChoirSteps | 2760 | 8.13 | TALL | - |
| SM_S1PEOPLE_LiftLunch | 1752 | 4.01 | TALL | 0 / 1 / 3 |
| SM_S1PEOPLE_TuningBalcony | 1602 | 1.89 | low | 0 / 1 / 2 |
| SM_S1PEOPLE_ClerksLedge | 1736 | 1.92 | low | 0 / 2 / 2 |

## Left out

Nothing. All 11 blocks compiled as written.

## Not checked

- No frame was reviewed for the new pieces: the one frame of the start script (6 s into the level) was opened only to see the level runs;
  no new mesh was identified in it. Looks, colours, sizes, movement, glow, whether anything hides the road, reads as a pickup, an enemy or
  the boss, or stands inside a podium or tower: not checked.
- Placed counts of the 114 new meshes FTCLEAR does not name; the builders' "no mesh more than 40 times" and distance rules: not checked.
- The whole road was not driven; only the level's start. Frame time: not measured.
- s1people: no hand-over text reached the integrator; its block, generator, make log (16 built, 0 failed) and meshes were checked like the others.
- The generators were not re-run; the committed meshes are the builders' one run each.

## Review and fix round (16 Nov 2026)

One round after Stage 1's review (a rules reader by the numbers, a driver with 8 game runs). What was run and seen here; nothing else.

### Found (the two reports)

- Rule broken: SM_S1D7_LedgerArchive placed 41 times (cap 40); SM_S1PLANTS_YewDrums read as a green box on a gold ring behind the barrier
  (8.09 km right), the size and colour family of the repair pickup.
- Looks wrong: s1d1's blocks of four across the far ends of side streets; s1d4's left "lot" where a second-row tower stands (SM_Tower4 is
  placed on both sides, FTTrack.cpp); s1d4's pocket at slot 102 left with no monument before it; the Vault in slots shared with older far
  pieces; "a car stack with a crowd on its roof" at 2.79 km right.
- Weak: six of eight heroes never read from the road; the Plaza, the Garages and Penthouse Row do not read at street level; benches in a side
  street's mouth; pipes with black bores end-on to the road; RoseBed swaying 3 in the still arena; and smaller items listed below.

### Fixed

- LedgerArchive: the fourth Archive rule every 24 (was 12). FTCLEAR after: 35 placed. TollHouse: the TollC rule every 24: 37 slots at most
  (was 43; its placed count is not in the log).
- YewDrums: redrawn as four clipped cones, tall and short in turn (no cube, no drum); top 1.61 m, low, 516 triangles. Seen after, 8.08 km
  right (C:\ftwork\out\L7s1fix\wR\wR_t011.png): a row of four dark cones in tubs, not a box.
- s1d1: four new sets of three houses (WldS1D1BlockAL, BL, AR, BR); the house at the side street's end is gone on both sides. Same rule count.
- s1d4: the five lot parts are out of the four left sets (YachtPond and UrnBalustrade are no longer placed anywhere); the s % 8 == 6 left
  rule has its own Zone ending 4.59 km, so slot 102 has no pocket.
- s1d7: the Vault stands at slots 210 (9.50 km), 242 (10.95 km), 274 (12.40 km); 182 and 278 are left. 210 and 274 are s % 16 == 2, where
  the block puts no tower in front.
- s1d2: a left kerb set without VelvetBenches (WldS1D2KerbK1L) for the two left rules.
- s1d8 PipeRack: the tube ends are steel caps, not black bores. s1plants RoseBed: swing 1.5 (was 3). Both regenerated with YewDrums
  (C:\ftwork\logs\L7s1fix_make.log: 3 built, 0 failed).

### Not fixed, and why

- "CarStack with a crowd on its roof" (2.79 km right): not a Stage 1 piece. With CarStack taken out of both right sets the same rack was
  still there (C:\ftwork\out\L7s1fix\vR\vR_t091.png): it is the kit's parking garage with pass 3's watchers on its decks. CarStack was put back.
- Heroes that do not read: tried the Percolator at 1.6, the Grand Curtain, Corkscrew Stack and Orrery at 1.4; in the 73 frames looked at of two drives
  (0.2 to 4.5 km right lane, 0.5 to 4.5 km left) none of them came into view (the Orrery again only as an arc, at 4.37 km), so the sizes were set back. They need new sites chosen with the
  road's sight lines in hand: a second round of placing, not a small change.
- The Plaza, the Garages, Penthouse Row at street level: the one open stretch of the Plaza (3.44 to 3.56 km) is the loop's exit, where the
  track leaves Site and Scatter pieces out and the plan keeps tall things 66 m off; the others need new near pieces. Not a small change.
- Stage 1 buildings 54 to 70 m out that share ground with the second row of towers (s1d1, s1d3 left, s1d5, s1d6, s1d7): left. Not seen from
  the road in any frame either way; their crowns were what the driver did see (s1d5, s1d6, s1d7).
- The second Great Scales (slot 136, with a pass 3 abseil piece in the same slot): left; it is one of the two heroes the driver saw read,
  and dropping its rule would re-roll every random rule after it.
- Roof-edge animals and one ClerksLedge on the inside of banked hairpins; doves over corkscrew, loop and jump slots; s1d8's 22 yard rules
  that take no slot; pieces FTCLEAR reports as moved; plants in the circuit swaying over 2; crates and barrels under the raised people;
  the red LanternCluster: left as they are. None was seen wrong in a frame; the crates at 8.3 km right read as brown crates.

### Compile and run

- `build.ps1 -Tag L7s1_fix_build`: "build exit 0 after 83 s", "Result: Succeeded".
- Level started once (`start_L7.txt`, out `C:\ftwork\out\s1\fixed_L7`): "exit 0 after 45 s", 1 frame.
- `FTTRACK level=7 frames=12717 route=5faaeb1d solid=25fa2576 solidTriangles=957884`: equal to the level's line in
  `SourceArt/Jam/validate/track_baseline.txt` (string compare).
- `FTDRESS level=7 districts=226 sets=464 pieces placed=12454 of 12510 tried shadows=5103 of 12598` (before the round: 225 / 459 / 12573 of 12629 / 5218 of 12717).
- "Fatal error" 0, "LogFT: Error" 0, load failures naming World/CityCenter 0; the two "Error:" lines are the editor's port 8000 listener.
- Check drives (C:/ftwork/out/L7s1fix: vR 86 frames, vL 60, wR 33 in the arena, right lane from 7.70 km): nothing of Stage 1 on the
  road or between the barriers in the 85 frames looked at (sheets sh_1..4, sl_1..2, sw_1). Both long drives ran slower than the
  driver's (the car was stopped at least once by an older dark box on a pole at the barrier, the driver's note 7), so they covered less road.

### Not checked

- District 8, the circuit (13.56 km on): not driven; PipeRack's new caps, the Hourglass and everything else there not seen.
- The Vault at its two new slots, the s1d1 blocks of three, the s1d4 left sets, the benchless kerb set, RoseBed's sway: changed in the
  table or generator, not picked out in a frame. Whether 9.50 and 12.40 km are straights: not checked.
- Arena frames of the check drive after 9.0 km and the left lane of the arena: not looked at. Frame time: not measured.
- The driver's contact sheet (C:\ftwork\out\L7s1rev\L7_s1_review.jpg) is 1.35 MB, over the 1 MB limit: not copied into the repository.
- Placed counts of meshes FTCLEAR does not name (TollHouse among them).
