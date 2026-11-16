# Level 4, Theme Park: Stage 1 (the world), integration note

16 Nov 2026, branch `claude/world-L4`. Only what was run and seen; the rest is under "Not checked".

## What went in

All 11 block files `C:\ftwork\s1\L4_*.inl` (s1d1 to s1d8, s1plants, s1animals, s1people) were pasted, each wrapped in
`{ }`, in that order at the END of `Source/FullThrottle/FTPeopleL4.inl` (1755 lines added, nothing existing changed;
line ends made CRLF like the rest of the file). None left out. Before pasting, for every block: its generator
`Content/Python/ft_world_park_<id>.py` exists; its make log `C:\ftwork\logs\L4<id>_make.log` says
`pieces built N failed 0` with 0 `LogPython: Error` and 0 `FAILED` lines; every mesh name in the block has its
`.uasset` in `Content/World/ThemePark` and every `.uasset` there is named in a block (219 files, 13 MB, none over 40 MB);
no Set name twice, every Set named `WldS1...`, no macro name twice, no `Lit()` or `Bank(`.

Compile (`build.ps1`, tag L4s1_build): exit 0, "Result: Succeeded", 0 warnings in the log, no change to any block needed.

## Zone plan in short (C:\ftwork\final\s1\L4_plan.md)

| id | district | km | look | hero |
|---|---|---|---|---|
| s1d1 | Front Gate and Midway | 0 to 2.22 | show street: false fronts, parapets, bulbs | GreatOrgan |
| s1d2 | Pirate Cove | 2.22 to 3.21 | stilt harbour of crooked plank houses | BottleShip |
| s1d3 | The Lagoon | 3.21 to 6.9074 | water pavilions on tall piles | SeahorseCascade |
| s1d4 | Rocket Land and Funhouse | 6.9074 to 9.2974 | tin-toy town, crooked from 8.12 | Observatory |
| s1d5 | The Last Ride: Sweet Street | 9.2974 to 11.94 | sweet shops, icing roofs | GreatSwirl |
| s1d6 | The Last Ride: Crescent Keep | 11.94 to 16.10 | storybook castle town | CrescentKeep |
| s1d7 | The Last Ride: Snowglobe Square | 16.10 to 20.50 | winter fair | SnowGlobe |
| s1d8 | The Last Ride: Giant's Garden | 20.50 to the end | houses that are garden things | Teapot |

s1plants, s1animals and s1people cover all eight districts (plants on slots s % 4 == 3, people s % 8 == 1, animals
s % 8 == 5 near and s % 4 == 1 further out, district pieces on the even slots).

## The game run (once)

`run_game.ps1` with `C:\ftwork\jobs\p3\start_L4.txt`, output `C:\ftwork\out\s1\after_L4`: exit 0 after 240 s.

- Track: `FTTRACK level=4 frames=12518 route=673f0081 solid=3e2a2030 solidTriangles=987216`, equal (diff, no
  difference) to the `^FTTRACK level=4 ` line of `SourceArt/Jam/validate/track_baseline.txt`.
- FTDRESS before (`C:\ftwork\out\p3\final_L4\run.log`): `districts=228 sets=60 pieces placed=13163 of 13266 tried shadows=6784 of 13358`.
- FTDRESS after: `districts=295 sets=239 pieces placed=16093 of 16204 tried shadows=8703 of 16288`.
  So +2930 placed, +2938 tried, +1919 shadow casters.
- FTCLEAR level=4: before `props kept=15435 (moved off their own barrier 443) left out of the road=104 pillars left out=18`,
  after `props kept=18365 (moved off their own barrier 581) left out of the road=112 pillars left out=18`.
  `land: 0 of 25036 frame sides cut short`.
- "Fatal error": 0. "LogFT: Error": 0. Lines with `/Game/World` that speak of a failure, error, warning or
  something missing: 0.
- The script's `waitfor FT CLOCK level=6.0` timed OUT (150 s) in this run; in the before run it was found. The log
  shows why: the clock line went 5.0, 6.1 (a 357 ms frame), so "6.0" was never printed. FTHITCH2 lines: 265 in this
  run, 1 in the before run (which ended at clock 6.0, so the two are not the same length). During the run the editor
  was building mesh distance fields for the new meshes: 76 "Finished distance field build" lines (10 to 22 s each;
  S1D1 18, S1D2 15, S1D3 15, S1D4 18, S1D5 10 when the run ended). The brief says Stage 1 meshes have no distance
  field; the log says they are built. Not changed here (it is the shared exporter, not a block).
- The one frame (`L4_t006.png`) was therefore taken at 2:17 of the level, on the finale's road: no Stage 1 piece can
  be told in it.

## Every new mesh (219; 259,968 triangles in all)

Triangles, top and TALL/low are copied from the make logs. "FTCLEAR" is left out / moved / placed from the after
run's FTCLEAR line; that line lists only meshes with something left out or moved, so "not listed" means the placed
count of that mesh is not known from the log.

### s1d1: 18 meshes, 23512 triangles

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D1_BoxOffice | 1632 | 17.39 | TALL | not listed |
| SM_S1D1_PennyArcade | 1896 | 12.84 | TALL | not listed |
| SM_S1D1_MirrorMaze | 1890 | 18.72 | TALL | not listed |
| SM_S1D1_ShootGallery | 1956 | 12.1 | TALL | not listed |
| SM_S1D1_IceParlour | 1924 | 18.06 | TALL | not listed |
| SM_S1D1_CostumeShop | 2190 | 15.82 | TALL | not listed |
| SM_S1D1_PhotoStudio | 1254 | 16.47 | TALL | not listed |
| SM_S1D1_TicketKiosk | 736 | 6.4 | TALL | not listed |
| SM_S1D1_BulbArch | 924 | 10.18 | TALL | not listed |
| SM_S1D1_PrizeRack | 1396 | 5.1 | TALL | not listed |
| SM_S1D1_WeighChair | 626 | 5.08 | TALL | not listed |
| SM_S1D1_BannerMast | 438 | 8.28 | TALL | 0 / 6 / 18 |
| SM_S1D1_BulbPost | 616 | 7.55 | TALL | 0 / 7 / 18 |
| SM_S1D1_Turnstiles | 572 | 1.38 | low | not listed |
| SM_S1D1_BarrelOrgan | 568 | 1.89 | low | not listed |
| SM_S1D1_PosterBoards | 464 | 1.72 | low | 0 / 1 / 28 |
| SM_S1D1_RingToss | 606 | 1.67 | low | not listed |
| SM_S1D1_GreatOrgan | 3824 | 34.0 | TALL | not listed |

### s1d2: 16 meshes, 26126 triangles

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D2_HarbourTower | 2678 | 20.48 | TALL | not listed |
| SM_S1D2_SailLoft | 2718 | 16.61 | TALL | not listed |
| SM_S1D2_Chandler | 2572 | 12.96 | TALL | not listed |
| SM_S1D2_BoatShed | 1852 | 10.18 | TALL | not listed |
| SM_S1D2_NetHouse | 2000 | 15.57 | TALL | not listed |
| SM_S1D2_MapHouse | 2700 | 15.12 | TALL | not listed |
| SM_S1D2_LookoutInn | 2676 | 17.73 | TALL | not listed |
| SM_S1D2_Figurehead | 560 | 6.49 | TALL | 1 / 0 / 24 |
| SM_S1D2_SignalMast | 670 | 13.57 | TALL | not listed |
| SM_S1D2_FishRack | 780 | 3.9 | TALL | not listed |
| SM_S1D2_WheelArch | 1094 | 6.8 | TALL | not listed |
| SM_S1D2_Pots | 690 | 1.7 | low | 0 / 1 / 14 |
| SM_S1D2_Capstan | 508 | 1.2 | low | not listed |
| SM_S1D2_RowBoat | 444 | 1.53 | low | not listed |
| SM_S1D2_Cannon | 582 | 1.24 | low | not listed |
| SM_S1D2_BottleShip | 3602 | 20.35 | TALL | not listed |

### s1d3: 17 meshes, 18108 triangles

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D3_Boathouse | 1164 | 34.56 | TALL | not listed |
| SM_S1D3_JellyTea | 1256 | 32.59 | TALL | not listed |
| SM_S1D3_DiveTower | 880 | 37.79 | TALL | not listed |
| SM_S1D3_ShellStand | 1108 | 32.82 | TALL | not listed |
| SM_S1D3_PearlStation | 1380 | 34.2 | TALL | not listed |
| SM_S1D3_LilyPagoda | 1764 | 39.37 | TALL | not listed |
| SM_S1D3_PumpHouse | 1288 | 37.52 | TALL | not listed |
| SM_S1D3_ChannelMarker | 540 | 30.88 | TALL | not listed |
| SM_S1D3_LanternLine | 652 | 30.7 | TALL | not listed |
| SM_S1D3_HighJetty | 1168 | 29.5 | TALL | not listed |
| SM_S1D3_FishVane | 460 | 34.4 | TALL | not listed |
| SM_S1D3_MooringJetty | 832 | 4.8 | TALL | 0 / 7 / 25 |
| SM_S1D3_PedaloDock | 824 | 5.1 | TALL | 1 / 4 / 24 |
| SM_S1D3_LilyFountain | 1116 | 8.56 | TALL | 1 / 11 / 24 |
| SM_S1D3_OarRack | 328 | 1.82 | low | 0 / 1 / 4 |
| SM_S1D3_ShellBench | 308 | 1.72 | low | 0 / 1 / 4 |
| SM_S1D3_SeahorseCascade | 3040 | 62.25 | TALL | not listed |

### s1d4: 20 meshes, 15078 triangles

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D4_RocketGarage | 860 | 12.0 | TALL | not listed |
| SM_S1D4_ClockworkCafe | 1292 | 15.82 | TALL | not listed |
| SM_S1D4_ControlTower | 832 | 21.93 | TALL | not listed |
| SM_S1D4_Planetarium | 1128 | 11.24 | TALL | not listed |
| SM_S1D4_BoxFlats | 1124 | 16.8 | TALL | not listed |
| SM_S1D4_CrookedHouse | 820 | 18.7 | TALL | not listed |
| SM_S1D4_MirrorHall | 772 | 10.8 | TALL | not listed |
| SM_S1D4_CrookedTower | 904 | 20.71 | TALL | not listed |
| SM_S1D4_PropStore | 616 | 7.7 | TALL | not listed |
| SM_S1D4_SpareKey | 336 | 5.26 | TALL | not listed |
| SM_S1D4_FuelPump | 440 | 3.92 | TALL | not listed |
| SM_S1D4_TinDog | 452 | 2.92 | TALL | not listed |
| SM_S1D4_SignalDish | 304 | 6.43 | TALL | not listed |
| SM_S1D4_WobblyMirrors | 168 | 1.96 | low | 0 / 1 / 14 |
| SM_S1D4_SpringBench | 532 | 1.42 | low | 0 / 1 / 15 |
| SM_S1D4_AerialLamp | 426 | 7.21 | TALL | not listed |
| SM_S1D4_GearBed | 468 | 1.9 | low | 0 / 2 / 15 |
| SM_S1D4_StageFlats | 276 | 4.63 | TALL | not listed |
| SM_S1D4_NowhereStair | 392 | 5.97 | TALL | not listed |
| SM_S1D4_Observatory | 2936 | 55.85 | TALL | not listed |

### s1d5: 16 meshes, 16684 triangles

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D5_WaferHouse | 1656 | 11.7 | TALL | not listed |
| SM_S1D5_ToffeeWorks | 1736 | 14.62 | TALL | not listed |
| SM_S1D5_SugarMill | 1472 | 13.44 | TALL | not listed |
| SM_S1D5_FountainHall | 1656 | 11.3 | TALL | not listed |
| SM_S1D5_CaneCafe | 1732 | 11.38 | TALL | not listed |
| SM_S1D5_JamJar | 948 | 17.0 | TALL | not listed |
| SM_S1D5_CaneRow | 792 | 7.36 | TALL | not listed |
| SM_S1D5_SugarCubes | 120 | 1.52 | low | not listed |
| SM_S1D5_ConeRack | 828 | 3.74 | TALL | not listed |
| SM_S1D5_LiquoriceGate | 528 | 5.64 | TALL | not listed |
| SM_S1D5_SyrupCart | 484 | 1.79 | low | not listed |
| SM_S1D5_WaferFence | 580 | 1.52 | low | not listed |
| SM_S1D5_WashLine | 404 | 3.49 | TALL | not listed |
| SM_S1D5_ChocBench | 308 | 1.31 | low | not listed |
| SM_S1D5_SugarSacks | 396 | 1.6 | low | not listed |
| SM_S1D5_GreatSwirl | 3044 | 67.7 | TALL | not listed |

### s1d6: 18 meshes, 21112 triangles

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D6_RoundTower_A | 1004 | 21.33 | TALL | not listed |
| SM_S1D6_RoundTower_B | 1292 | 28.72 | TALL | not listed |
| SM_S1D6_TwinTurrets | 1536 | 25.43 | TALL | not listed |
| SM_S1D6_Gatehouse | 2188 | 21.81 | TALL | not listed |
| SM_S1D6_Astronomer | 1548 | 34.53 | TALL | not listed |
| SM_S1D6_BellTower | 1260 | 32.01 | TALL | not listed |
| SM_S1D6_BridgeHouse | 1376 | 20.08 | TALL | not listed |
| SM_S1D6_BannerHall | 1728 | 16.14 | TALL | not listed |
| SM_S1D6_Dovecote | 1312 | 22.22 | TALL | not listed |
| SM_S1D6_GardenWall | 704 | 3.59 | TALL | not listed |
| SM_S1D6_Armour | 476 | 4.32 | TALL | not listed |
| SM_S1D6_BannerPole | 404 | 9.7 | TALL | not listed |
| SM_S1D6_Well | 564 | 4.84 | TALL | not listed |
| SM_S1D6_MoonDial | 516 | 1.8 | low | 0 / 1 / 20 |
| SM_S1D6_TopiaryKnight | 376 | 5.22 | TALL | not listed |
| SM_S1D6_MarketCart | 572 | 1.88 | low | not listed |
| SM_S1D6_MoonLamp | 288 | 4.6 | TALL | not listed |
| SM_S1D6_CrescentKeep | 3968 | 79.67 | TALL | not listed |

### s1d7: 16 meshes, 16644 triangles

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D7_SkateHire | 1472 | 11.44 | TALL | not listed |
| SM_S1D7_IceArcade | 1556 | 7.87 | TALL | not listed |
| SM_S1D7_ChocHut | 1120 | 12.6 | TALL | not listed |
| SM_S1D7_TobogganTower | 936 | 18.7 | TALL | not listed |
| SM_S1D7_BellChalet | 1512 | 16.53 | TALL | not listed |
| SM_S1D7_PalaceGate | 1272 | 14.6 | TALL | not listed |
| SM_S1D7_MittenShop | 932 | 11.48 | TALL | not listed |
| SM_S1D7_Sleigh | 320 | 1.76 | low | 0 / 1 / 24 |
| SM_S1D7_SkiStack | 492 | 1.83 | low | not listed |
| SM_S1D7_SnowFence | 508 | 1.5 | low | 0 / 1 / 24 |
| SM_S1D7_IceSeats | 504 | 1.88 | low | not listed |
| SM_S1D7_SnowmanFamily | 1080 | 3.36 | TALL | not listed |
| SM_S1D7_IceFish | 376 | 4.95 | TALL | not listed |
| SM_S1D7_LampPair | 672 | 4.83 | TALL | not listed |
| SM_S1D7_VanePost | 412 | 6.61 | TALL | not listed |
| SM_S1D7_SnowGlobe | 3480 | 39.3 | TALL | not listed |

### s1d8: 16 meshes, 22392 triangles

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D8_ToadHouse | 2314 | 17.07 | TALL | not listed |
| SM_S1D8_PotTower | 1776 | 19.33 | TALL | not listed |
| SM_S1D8_Boot | 1396 | 15.9 | TALL | not listed |
| SM_S1D8_Birdhouse | 1428 | 18.0 | TALL | not listed |
| SM_S1D8_Can | 1464 | 12.36 | TALL | not listed |
| SM_S1D8_SeedShop | 1232 | 12.82 | TALL | not listed |
| SM_S1D8_Acorn | 1796 | 14.18 | TALL | not listed |
| SM_S1D8_TrowelFork | 676 | 11.34 | TALL | not listed |
| SM_S1D8_SnailKiosk | 1120 | 7.7 | TALL | not listed |
| SM_S1D8_Barrow | 748 | 10.33 | TALL | not listed |
| SM_S1D8_Lamp | 372 | 8.12 | TALL | not listed |
| SM_S1D8_WashLine | 680 | 7.36 | TALL | not listed |
| SM_S1D8_ToadSeats | 1644 | 1.89 | low | 0 / 2 / 25 |
| SM_S1D8_PegFence | 700 | 1.88 | low | 0 / 1 / 25 |
| SM_S1D8_Thimbles | 1042 | 1.9 | low | 0 / 3 / 30 |
| SM_S1D8_Teapot | 4004 | 45.7 | TALL | not listed |

### s1plants: 32 meshes, 31544 triangles

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1PLANTS_TopiaryTubs | 1026 | 1.88 | low | 0 / 4 / 20 |
| SM_S1PLANTS_RoseStandards | 712 | 1.84 | low | 0 / 2 / 43 |
| SM_S1PLANTS_BulbPlane | 912 | 10.0 | TALL | not listed |
| SM_S1PLANTS_TopiaryTiers | 748 | 6.64 | TALL | not listed |
| SM_S1PLANTS_Mangrove | 732 | 6.32 | TALL | not listed |
| SM_S1PLANTS_SeaGrassDrift | 844 | 1.76 | low | not listed |
| SM_S1PLANTS_Hibiscus | 760 | 4.06 | TALL | not listed |
| SM_S1PLANTS_GiantBulrush | 928 | 30.8 | TALL | 0 / 11 / 11 |
| SM_S1PLANTS_LotusTower | 1024 | 30.72 | TALL | 1 / 16 / 16 |
| SM_S1PLANTS_Papyrus | 788 | 30.8 | TALL | 0 / 12 / 12 |
| SM_S1PLANTS_LilyRaft | 1376 | 0.8 | low | 1 / 11 / 39 |
| SM_S1PLANTS_Willow | 712 | 10.8 | TALL | 0 / 2 / 3 |
| SM_S1PLANTS_SilverSpirals | 1336 | 4.6 | TALL | not listed |
| SM_S1PLANTS_CactusTins | 1418 | 1.83 | low | not listed |
| SM_S1PLANTS_MoonVine | 842 | 6.09 | TALL | not listed |
| SM_S1PLANTS_BlossomSwirl | 912 | 8.7 | TALL | not listed |
| SM_S1PLANTS_BlossomCones | 874 | 5.7 | TALL | not listed |
| SM_S1PLANTS_CandyTulips | 1340 | 1.13 | low | not listed |
| SM_S1PLANTS_Cypress | 1092 | 10.8 | TALL | not listed |
| SM_S1PLANTS_TopiaryMoon | 612 | 5.19 | TALL | not listed |
| SM_S1PLANTS_WisteriaArch | 1032 | 5.96 | TALL | not listed |
| SM_S1PLANTS_LavenderBed | 1280 | 1.05 | low | not listed |
| SM_S1PLANTS_SnowFirTall | 714 | 13.66 | TALL | not listed |
| SM_S1PLANTS_SnowFirGroup | 1186 | 8.04 | TALL | not listed |
| SM_S1PLANTS_FrostBirch | 1116 | 9.61 | TALL | not listed |
| SM_S1PLANTS_HollyLow | 786 | 1.81 | low | not listed |
| SM_S1PLANTS_Sunflower | 852 | 12.44 | TALL | not listed |
| SM_S1PLANTS_DandelionClocks | 1236 | 7.21 | TALL | not listed |
| SM_S1PLANTS_BeanPoles | 1032 | 7.92 | TALL | not listed |
| SM_S1PLANTS_GiantFern | 1276 | 5.54 | TALL | not listed |
| SM_S1PLANTS_Bluebells | 1504 | 3.96 | TALL | not listed |
| SM_S1PLANTS_CloverPatch | 542 | 1.69 | low | not listed |

### s1animals: 19 meshes, 21784 triangles

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1ANIMALS_PeacockWall | 948 | 4.32 | TALL | not listed |
| SM_S1ANIMALS_GeeseLine | 1304 | 3.11 | TALL | not listed |
| SM_S1ANIMALS_PelicanPosts | 1156 | 5.04 | TALL | not listed |
| SM_S1ANIMALS_CrabRock | 1048 | 2.59 | TALL | not listed |
| SM_S1ANIMALS_Swans | 1406 | 2.55 | TALL | not listed |
| SM_S1ANIMALS_KoiLeap | 1520 | 2.02 | TALL | not listed |
| SM_S1ANIMALS_HeronsOtters | 1300 | 3.13 | TALL | not listed |
| SM_S1ANIMALS_Rabbits | 1242 | 2.74 | TALL | not listed |
| SM_S1ANIMALS_BackLotRaccoons | 1444 | 3.66 | TALL | not listed |
| SM_S1ANIMALS_RibbonPonies | 1466 | 2.51 | TALL | not listed |
| SM_S1ANIMALS_WhiteDeer | 1078 | 5.2 | TALL | not listed |
| SM_S1ANIMALS_Penguins | 1432 | 4.51 | TALL | not listed |
| SM_S1ANIMALS_Reindeer | 1626 | 3.6 | TALL | not listed |
| SM_S1ANIMALS_GardenFolk | 1656 | 2.54 | TALL | not listed |
| SM_S1ANIMALS_Butterflies | 1468 | 4.9 | TALL | not listed |
| SM_S1ANIMALS_Egrets | 530 | 2.97 | TALL | 2 / 0 / 28 |
| SM_S1ANIMALS_Owls | 300 | 2.17 | TALL | not listed |
| SM_S1ANIMALS_SwanFlight | 390 | 1.86 | low | not listed |
| SM_S1ANIMALS_Songbirds | 470 | 2.63 | TALL | not listed |

### s1people: 31 meshes, 46984 triangles

| mesh | triangles | top m | class | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1PEOPLE_Sw_Ices | 1708 | 4.13 | TALL | not listed |
| SM_S1PEOPLE_Sw_Toffee | 1408 | 4.14 | TALL | not listed |
| SM_S1PEOPLE_Sw_Wagon | 1432 | 4.14 | TALL | not listed |
| SM_S1PEOPLE_Sw_Window | 1380 | 4.4 | TALL | not listed |
| SM_S1PEOPLE_Sw_Baker | 1288 | 5.4 | TALL | not listed |
| SM_S1PEOPLE_Sw_Bench | 1656 | 4.67 | TALL | not listed |
| SM_S1PEOPLE_Kp_Heralds | 1464 | 5.28 | TALL | not listed |
| SM_S1PEOPLE_Kp_Juggler | 1188 | 4.54 | TALL | not listed |
| SM_S1PEOPLE_Kp_Dance | 1560 | 4.72 | TALL | not listed |
| SM_S1PEOPLE_Kp_Painter | 1250 | 4.64 | TALL | not listed |
| SM_S1PEOPLE_Kp_Bridge | 1104 | 4.38 | TALL | not listed |
| SM_S1PEOPLE_Kp_Stargazer | 1312 | 5.23 | TALL | not listed |
| SM_S1PEOPLE_Sn_Rink | 1190 | 4.12 | TALL | 0 / 4 / 12 |
| SM_S1PEOPLE_Sn_Sledge | 1274 | 4.08 | TALL | 0 / 2 / 9 |
| SM_S1PEOPLE_Sn_Snowman | 1282 | 3.96 | TALL | 0 / 1 / 9 |
| SM_S1PEOPLE_Sn_Mugs | 1462 | 4.5 | TALL | not listed |
| SM_S1PEOPLE_Sn_Bell | 916 | 5.14 | TALL | 0 / 2 / 11 |
| SM_S1PEOPLE_Sn_Skis | 1126 | 4.96 | TALL | 0 / 2 / 8 |
| SM_S1PEOPLE_Gd_Ladder | 1236 | 5.6 | TALL | 0 / 1 / 11 |
| SM_S1PEOPLE_Gd_TeaCup | 1616 | 3.42 | TALL | 0 / 1 / 9 |
| SM_S1PEOPLE_Gd_Toadstools | 1576 | 4.24 | TALL | not listed |
| SM_S1PEOPLE_Gd_Hose | 1000 | 4.44 | TALL | 0 / 1 / 8 |
| SM_S1PEOPLE_Gd_Beans | 1256 | 5.01 | TALL | 0 / 4 / 12 |
| SM_S1PEOPLE_Gd_Thimbles | 1616 | 3.9 | TALL | 0 / 2 / 8 |
| SM_S1PEOPLE_JumpTerrace | 2396 | 5.72 | TALL | not listed |
| SM_S1PEOPLE_JumpSteps | 2244 | 6.02 | TALL | not listed |
| SM_S1PEOPLE_CoveDeck_A | 1916 | 7.06 | TALL | not listed |
| SM_S1PEOPLE_CoveDeck_B | 1788 | 7.29 | TALL | not listed |
| SM_S1PEOPLE_BackLotDock | 1460 | 4.4 | TALL | not listed |
| SM_S1PEOPLE_LagoonJetty_A | 2452 | 27.81 | TALL | 0 / 3 / 3 |
| SM_S1PEOPLE_LagoonJetty_B | 2428 | 26.72 | TALL | 1 / 5 / 5 |

What each piece is and what moves on it: the header of each block in `FTPeopleL4.inl` and the generators.

## Left out

Nothing: all 11 blocks are in. By the track (after run, Stage 1 meshes only, from FTCLEAR): 8 placements left out
(S1D2_Figurehead 1, S1D3_LilyFountain 1, S1D3_PedaloDock 1, S1PLANTS_LilyRaft 1, S1PLANTS_LotusTower 1,
S1ANIMALS_Egrets 2, S1PEOPLE_LagoonJetty_B 1), 138 moved, in the 39 meshes that line lists.

## Not checked

- Nothing was LOOKED at: no frame shows a Stage 1 piece. Whether any piece stands in another, floats, reads as a
  pickup, enemy or the boss, or hides the road: not checked.
- Placed counts of the 180 meshes FTCLEAR does not list; whether every one of the 219 meshes has at least one placement.
- Frame time. The run hitched (above) while distance fields were built; what the level costs once they are cached
  was not measured (no second run).
- Whether the distance fields are wanted, and why the exporter's meshes get them.
- The builders' rules (distances, slots, glow, speeds, "must not be built"): read by nobody here beyond the checks
  listed under "What went in".
- The other seven levels; a packaged build.

## Review and fix round

16 Nov 2026, one round, after two reviews of commit 457f4577: a rules reader (tables and generators by number, no
frame) and a driver (7 runs, frames of 0 to 17.3 km only; the boss's clock ends the level before the road does).
Changed: `Source/FullThrottle/FTPeopleL4.inl` only (the Stage 1 blocks, and a new block `s1fix` at the end) and this
note. No generator, no mesh, no older rule, no road, barrier or material was touched. Every changed line says
"fix round" in its comment.

How the table was checked here (scratch `C:\ftwork\jobs\L4s1fix`, outputs `C:\ftwork\out\L4s1fix`): the rules
reader's own stand-alone table dump and box scripts, copied and re-run on the edited table, with one addition: the
track's dice (`FRandomStream(s * 7919 + rule * 104729 + level * 31 + 977)`, written out from FTTrack.cpp), so that a
piece placed by chance, or sliding along its slot, is compared where the dice put it and not anywhere in its range.
The one check of that against the game: SM_S1ANIMALS_Egrets, 30 by the dice, 2 left out + 28 placed in the log.
All of it is boxes on a straight road: bends, the land's height and what the track itself moves are not in it; the
level's own trees, booths, tents and balloons (FTTrack.cpp) and 18 older meshes without a footprint are not in it.

A rule is never deleted and none is added inside a block: the track seeds a rule's dice with its running number, so a
rule taken out is left in place with a slot outside its zone ("slot 0") or chance -1 (it places nothing), and rules
that had to be written new stand in `s1fix` at the very end. So every rule that was not changed stands where it
stood when the driver looked.

### Found and fixed

Rule broken:

- Splashdown's two low props reached inside 17 m (SM_S1D3_OarRack left, SM_S1D3_ShellBench right, slots 150 and 152:
  front edges 16.52 and 16.96 m). Checked in the file (x 17.5 to 18.5, the pieces 0.98 and 0.54 m deep to the
  front). Now x 18.5 to 19.0: nearest front edge by the table 17.52 m. The re-run of the rules reader's distance
  check prints no line at all (no origin or edge under 17 m, no part over 2 m or glowing nearer than 24 m).

Looks wrong (pieces standing in other pieces; fixed by taking the Stage 1 piece out or moving it):

- The chair swing of the next slot's FunCourt (right, slots s % 12 == 2, +6.3 to +24.2 m along, 41 to 59 m out):
  s1d5's second jam jar (5 slots), s1d7's building at plus along (6 slots: one building stays), s1d8's Boot and Can x2
  (one building stays on 482, 506, 542; slot 494 is off: its other building stood in SM_P3A_Terrace).
- The galleon (left, slot 54): s1d2's Yard and Lane2 there are off (9 pieces).
- The stall row's candy cart: the last two pieces of the four "B" show rows of s1people are out (30 pieces).
- The Gateway's fountain and crowd (left): s1d8's lamp out of ToolsL, its barrow out of KioskL, the snail kiosk 2.5 m along.
- s1d4's prop store in the maintenance shed (slot 200 left): out. s1people: the cove decks 0.7 m further out (they
  were 0.6 m into the watchers' terrace) and without the middle deck (it stood in pass 3's crow's nest), the two
  back-lot docks 1.6 m further apart (one was 1.7 m into SM_P3A_Prizes).
- The dressing's ponds on the right's slots s % 4 == 0 (chance 0.7, sliding): with the dice, 13 of s1d7's 18 courts
  and 5 of its rows had pieces in the water (counted from 0.3 m in for a prop, 1.5 m for a building). Each of those
  slots has a set of its own now without those pieces; 3 of the rows are off whole. Two more s1d7 rows are off (slot 360 in a bandstand,
  slot 410 in a bandstand and the far mascot).
- s1d8's right-hand tall props (37 m, s % 4 == 2): pass 3's stage scenes (35 to 44 m out, 17 m long) stand on every
  one of those slots; with the dice 44 of the 66 props stood in a stage or in the planets on posts. No other distance
  is free there (30 to 33 m tried: 25 to 29 of them in the planets). The eight rules are off; the 22 props that are
  clear stand one by one in `s1fix`.
- s1d8 against the far mascot (15 m tall, 58 to 70 m, s % 7 == 4): the left rows of slots 466, 494, 522, 536 are out
  (the four rules are off and written slot by slot in `s1fix` without those slots), right slot 522 keeps its pot tower only.
- The animals' near bands on the Last Ride's LEFT (40.5 to 45 m, s % 8 == 5; 47 to 55 m, s % 8 == 1): pass 3 stands
  there. With the dice: Reindeer 13 of 13, GardenFolk 12 of 12, WhiteDeer 11 of 12, Rabbits 9 of 13, PeacockWall 7 of
  11, GeeseLine 5 of 7 stood in a pass 3 piece. Now: WhiteDeer and Giant's Garden's Rabbits right side only; the left
  Reindeer rule off (the right one stays); GeeseLine, PeacockWall and GardenFolk on the RIGHT on s % 24 == 5 and 13
  (21 is the saucer park's); the butterflies of Giant's Garden keep s % 24 == 21 only.
- Plants: 34 single plants are off: 23 tall lagoon stems on the right that stood on the dressing's lit path (9.7 m
  wide at the middle of every odd slot there), 6 in a chair swing, 4 in a pond (not the water plants), 1 in a fountain.
- s1people's show row round the press pen (it slides by dice): 3 pieces out of KeepF, 2 out of GardenF.
- By the boxes, with the dice, Stage 1 pieces standing 1 m or more (both ways) in an older solid piece on the land:
  340 before, 119 after (s1d7 51 to 11, s1d8 68 to 2, plants 56 to 22, animals 106 to 49, people 11 to 1, s1d2 13 to 5).
- Driver 1 (Sweet Street, Crescent Keep and Snowglobe Square look alike at the kerb): the district's own tall prop
  now stands in the front row where the tables leave a place clear by 0.5 m: 20 candy-cane rows 25.2 m out in Sweet
  Street (9.30 to 11.23 km, both sides), 25 vane posts 26.8 m out in Snowglobe Square (only its level stretches).
  Both are existing meshes with a steady glow, 24 m or more out, under the cap of 40 a mesh (CaneRow 38, VanePost 34
  by rule). NOT seen in the game. Crescent Keep got nothing: its turrets were the one signal the driver could see.
- Driver 3 (things close beside both jumps): by the table no Stage 1 rule places anything nearer than 40 m on slots
  40 to 49 or 187 to 195 (the nearest: plants 23 m on slot 39 and s1d4's low props on slot 186, both before the
  run-ups; a Site is left out by the track within 40 m of a run-up or landing anyway). What the driver saw there is
  older. The one Stage 1 piece a Far rule puts at a landing (SM_S1PEOPLE_JumpTerrace, slot 49 right) stands 44 m out
  now (was 42.3: front 40.3 m).

Weak:

- Driver 5 and rules reader 9 (the lagoon's right side reads as noise; ChannelMarker 52 over the cap of 40): the
  channel marker is out of the three right-hand sets (now 33 by rule), the lantern line out of one, and the 23 stems above.
- The four s1d2 houses that stood 70.0 m out (the same meshes stand nearer elsewhere) stand 69.5 m: all inside 70 m.
- By rule, with the dice, Stage 1 now places 3269 pieces (3499 before by the reader's count): animals 415 to 357
  with the sky (guide 150 to 300), plants 618 to 584 (300 to 600), s1d7 321 to 281 + 25 in `s1fix`, s1d8 274 to
  156 + 54 in `s1fix`. No district mesh over 40, no mesh without a placement.

### Not fixed, and why

- Driver 2 (five of six heroes not identifiable): not fixed. Each is 42 to 58 m wide and stands 92 to 156 m out
  behind rows 15 to 20 m tall; a place 60 to 80 m out on a straight would have to be cleared of other pieces and
  could not be looked at in this round (one start of the level; its one frame is at about 0.35 km). The driver's
  frames r5_t006 and r5_t007 (0.35 and 0.42 km; the organ stands at 0.452 km, not 0.6) were looked at here: no organ
  to be made out in them either.
- Driver 4 (a bunch of balloons at the kerb like the boss's): SM_Balloons is the level's own prop (FTTrack.cpp),
  older than Stage 1; the finding calls it weak, not a broken rule, so it was not touched.
- Driver 6: (a) and (c) are the road's shape. (b) left side, 0.4 to 0.6 km: by the table Stage 1 already has kerb
  props (17.5 m), masts (25.4 m) and a row of buildings (51 to 58 m) on those slots; nothing added without a frame.
- Rules reader 7 (the lagoon's tall stems, so tight to 24 m that the track moved every one): 2 m further out now
  (30.0 to 32.3 m), but the after run still says moved: GiantBulrush 10 of 10, LotusTower 12 of 12, Papyrus 4 of 4,
  Willow 1 of 2. The cause is not the 24 m line: they are 30 m tall on the valley floor, and the track pushes anything
  that comes up into the coaster's height out to 39.6 or 48 m (s1d3's header). Where they stand is not in the table.
- Rules reader 10 (two rules that place nothing, WldS1D8KioskL in verges 1 and 2): left in, with a comment, so that
  later rules keep their numbers.
- Rules reader 12 (bulbs at glow 2.0 in s1d1, the sugar mill's roof wheel, the inn's lantern, the gallery's plates:
  "to look at in a frame"): no mesh regenerated. No number rule is broken, and the driver saw no Stage 1 piece like
  the boss, a bomb or a pie in 0 to 17.3 km, where all four stand.
- Overlaps of about 1 m by the boxes, left: s1d4 RocketGarage/JetRide x4, SignalDish/PlanetPosts x4,
  SignalDish/Robot, AerialLamp/SwingRide; s1d6 BannerPole/Fountain x6; s1d8 WashLine/Crowd x4 (0.5 m); s1d2 Chandler/Pond.
- Left, deeper, by the boxes and the dice (a periodic rule cannot leave one slot out): animals 49 of 251 on the land
  (Rabbits of Rocket Land 8 of 22, PelicanPosts 7 of 11, RibbonPonies 6 of 14, BackLotRaccoons 5 of 8, WhiteDeer
  4 of 12, Penguins 4 of 24 and others); s1d7 11 (most are box corners on a round pond); s1d3 12 and plants 22 (jetties, fountains, docks and
  water plants in ponds: left, water things on water); s1d6 and s1d2 buildings under pass 3's high ways and parrots
  (Far pieces: 8 and about 15 by the boxes, not judged). SM_S1ANIMALS_GeeseLine 1.8 m into SM_S1D1_BoxOffice at
  slot 24 left (dice).

### The game run after the fix (once)

Compile (`build.ps1`, tag L4s1_fix_build): exit 0 after 77 s, "Result: Succeeded", 0 lines with "warning".
`run_game.ps1` with `C:\ftwork\jobs\p3\start_L4.txt`, output `C:\ftwork\out\s1\fixed_L4`: exit 0 after 49 s.

- Track: `FTTRACK level=4 frames=12518 route=673f0081 solid=3e2a2030 solidTriangles=987216`, compared here with the
  `^FTTRACK level=4 ` line of `SourceArt/Jam/validate/track_baseline.txt` (diff: no difference).
- FTDRESS: `districts=301 sets=312 pieces placed=15918 of 16028 tried shadows=8528 of 16113` (before the fix round
  295 / 239 / 16093 of 16204 / 8703 of 16288: 175 fewer placed, 175 fewer shadow casters).
- FTCLEAR: `props kept=18190 (moved off their own barrier 567) left out of the road=111 pillars left out=18`
  (before 18365 / 581 / 112). Stage 1 meshes in that line: SM_S1D7_VanePost 0 left out, 3 moved, 22 kept (the track
  moved 3 of the front-row posts further out); OarRack and ShellBench 1 moved of 4 each, as before.
- "Fatal error" 0, "LogFT: Error" 0, lines with `/Game/World` and a failure, error, warning or something missing 0.
  Two "Error:" lines, both the editor's own listener on port 8000 (LogHttpListener, LogModelContextProtocol).
  FTHITCH lines 15; "Finished distance field build" 0. `waitfor FT CLOCK level=6.0`: found.
- The one frame (`L4_t006.png`, looked at): scene 1 at 0:06, the Front Gate. None of the changed places is in it.

### Not checked

- NOTHING of this round was seen in the game: not the front-row cane rows and vane posts, not one removed overlap.
- Whether the remaining box overlaps are visible; whether any removed piece was in fact clear (boxes, straight road).
- The level's own props (trees 27 m and further out on every slot, booths, tents, balloons) against any Stage 1
  piece, old or new; the 18 older meshes without a footprint.
- 17.3 to 25.08 km in a frame (all of Giant's Garden, most of Snowglobe Square), as in the driver's review.
- Where the track put the pieces it moved; frame time; a packaged build; the other seven levels.
- The driver's contact sheet `C:\ftwork\out\L4s1rev\L4_s1_review.jpg` is 1,703,382 bytes (over 1 MB): not copied
  to `SourceArt/Integration/world/sheets/`.
