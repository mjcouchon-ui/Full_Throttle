# Level 0 "Bayshore": Stage 1 of the final pass, the world (integrated 16 Nov 2026)

Branch `claude/world-L0`. Ten builders' blocks were pasted, each wrapped in `{ }`, at the END of
`Source/FullThrottle/FTPeopleL0.inl` (after the pass 3 fix round), in this order: s1d1 .. s1d7, s1plants, s1animals,
s1people. No pasted block was edited; the first compile succeeded. Nothing else in the table was touched.
This note says only what the integrator ran and saw; the rest is marked "not checked".

## What was run

- `git status` before the paste: untracked only the ten generators `Content/Python/ft_world_bay_s1*.py` and
  `Content/World/` (187 `.uasset` files in `Content/World/Bayshore`, 9.7 MB together, none over 40 MB). Nothing modified.
- Per block: the generator file is there; every mesh name the block uses has its `.uasset` (checked name by name by
  script); the builder's make log ends "pieces built N failed 0" with 0 lines of `LogPython: Error` or `FAILED`.
- Build (`build.ps1`, tag L0s1_build): "build exit 0 after 71 s", "Result: Succeeded", 0 lines with "warning".
- One start of the level (`run_game.ps1`, script `C:\ftwork\jobs\p3\start_L0.txt`, out `C:\ftwork\out\s1\after_L0`):
  "exit 0 after 336 s", 1 frame.

## Before and after (same script; before = `C:\ftwork\out\p3\final_L0\run.log`)

| | before Stage 1 | after Stage 1 |
|---|---|---|
| FTDRESS districts (zones) | 1174 | 1416 |
| FTDRESS sets | 127 | 220 |
| FTDRESS pieces placed / tried | 13415 / 13529 | 15854 / 15988 |
| FTDRESS shadows | 7540 of 14063 | 9427 of 16502 |
| FTCLEAR props kept | 15503 | 17942 |
| FTCLEAR moved off their own barrier | 1120 | 1165 |
| FTCLEAR left out of the road | 51 | 51 |
| FTWPO life components | 112 of 216 | 292 of 403 |

- Placed: +2439 (tried +2459: tried minus placed was 114 before and is 134 now).
- Track: `FTTRACK level=0 frames=12717 route=dc9e032b solid=d2e759f4 solidTriangles=986296` in the run log, and the
  "^FTTRACK level=0 " line of `SourceArt/Jam/validate/track_baseline.txt` is the same text (compared with diff: equal).
- "Fatal error": 0 lines. "LogFT: Error": 0 lines. Lines naming `/Game/World` with fail / can't find / error / warning: 0.
  One `LogFT: Warning: FT WARM no assets found (asset registry empty?)`: the before log has the same line once.
- FTCLEAR prints a mesh only when a placement of it was moved or left out (FTTrack.cpp, the FTCLEAR line). 19 of the
  187 new meshes are printed, all with 0 left out (tables below, last column). For the other 168 the game prints no
  count a mesh: they are "not listed" below, and that each of them was placed at least once is not checked.
  Of the 19 listed: 273 placed, 45 of them moved off the barrier, 0 left out.

## The zone plan in short (`C:\ftwork\final\s1\L0_plan.md`)

Bay on the left, town on the right. Seven districts in road order, one builder each, in the even slots
(s % 4 == 0 or 2): buildings 26 to 70 m from the centre line, low props 17 to 26 m, tall props 24 to 40 m, one hero
landmark 60 to 200 m.

| id | district | km | look | hero |
|---|---|---|---|---|
| s1d1 | The Strip | 0.0 to 3.2 | streamline deco, cream with hot pink and yellow fins | The Sunburst |
| s1d2 | The Marina | 3.2 to 6.4 | white clapboard, harbour-blue gables | The Clipper |
| s1d3 | Pleasure Beach | 6.4 to 9.8 | red and yellow tents, onion domes | The Big Sundae |
| s1d4 | Hotel Row | 9.8 to 13.2 | mint palaces in arches, gold domes | The Shell |
| s1d5 | The Point | 13.2 to 16.8 | white cubes, coral domes, dark green | Arch Rock |
| s1d6 | Downtown waterfront | 16.8 to 21.1 | dark towers outlined in pink and turquoise light | The Picture Palace |
| s1d7 | The Old Harbour | 21.1 to the end | brown timber houses, teal doors | The Big Fish |

Plants 17 to 60 m in s % 4 == 3. People 17 to 45 m in s % 8 == 1, raised to clear the barrier. Animals 20 to 45 m in
s % 8 == 5 and 46 to 120 m in s % 4 == 1, afloat on the bay side, and in the sky as Far pieces.

## Every new mesh (187; 200,634 triangles together, summed from the make logs)

All are `/Game/World/Bayshore/<mesh>`. Triangles, top and TALL/low are copied from each builder's make log
(`C:\ftwork\logs\L0s1<id>_make.log`, the "FTPEOPLE piece" lines: the source of the hand-overs' numbers).
What each piece is and what moves on it: the builder's generator and its block header. The builders' estimates are
their own arithmetic from the block headers, not a count by the game.

### s1d1 The Strip, 0 to 3.2 km

19 meshes, 25656 triangles. Generator `Content/Python/ft_world_bay_s1d1.py`. Builder's estimate: about 191 placed.

| mesh | triangles | top m | log | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D1_Diner | 2072 | 10.6 | TALL | not listed |
| SM_S1D1_RecordShop | 1978 | 15.36 | TALL | not listed |
| SM_S1D1_PictureHouse | 1998 | 20.63 | TALL | not listed |
| SM_S1D1_Bowling | 2110 | 15.58 | TALL | not listed |
| SM_S1D1_Parlour | 1748 | 11.06 | TALL | not listed |
| SM_S1D1_MilkBar | 2232 | 13.7 | TALL | not listed |
| SM_S1D1_DanceHall | 1744 | 11.58 | TALL | not listed |
| SM_S1D1_Flats | 2122 | 18.2 | TALL | not listed |
| SM_S1D1_BathHouse | 2024 | 9.7 | TALL | not listed |
| SM_S1D1_Bollards | 456 | 1.05 | low | 0 / 3 / 21 |
| SM_S1D1_Bench | 400 | 1.09 | low | 0 / 2 / 28 |
| SM_S1D1_PylonA | 384 | 6.64 | TALL | not listed |
| SM_S1D1_PylonB | 284 | 8.94 | TALL | not listed |
| SM_S1D1_Clock | 560 | 5.46 | TALL | not listed |
| SM_S1D1_NeonArrow | 304 | 6.96 | TALL | not listed |
| SM_S1D1_JukeKiosk | 644 | 3.76 | TALL | not listed |
| SM_S1D1_DeckHut | 498 | 3.75 | TALL | not listed |
| SM_S1D1_Fountain | 688 | 3.22 | TALL | not listed |
| SM_S1D1_Sunburst | 3410 | 46.78 | TALL | not listed |

### s1d2 The Marina, 3.2 to 6.4 km

15 meshes, 13564 triangles. Generator `Content/Python/ft_world_bay_s1d2.py`. Builder's estimate: 241 if the track leaves none out placed.

| mesh | triangles | top m | log | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D2_HarbourOffice | 1096 | 15.22 | TALL | not listed |
| SM_S1D2_SailLoft | 736 | 15.1 | TALL | not listed |
| SM_S1D2_Chandlery | 1084 | 9.35 | TALL | not listed |
| SM_S1D2_BoatShed | 784 | 12.52 | TALL | not listed |
| SM_S1D2_ClubHouse | 1252 | 16.04 | TALL | not listed |
| SM_S1D2_Smokehouse | 640 | 14.36 | TALL | not listed |
| SM_S1D2_PilotCottage | 820 | 9.55 | TALL | not listed |
| SM_S1D2_Dinghies | 564 | 1.55 | low | not listed |
| SM_S1D2_Anchor | 460 | 4.31 | TALL | not listed |
| SM_S1D2_LobsterPots | 640 | 1.71 | low | not listed |
| SM_S1D2_SignalMast | 540 | 14.98 | TALL | not listed |
| SM_S1D2_RopeFence | 608 | 1.1 | low | not listed |
| SM_S1D2_ShipsBell | 324 | 3.88 | TALL | not listed |
| SM_S1D2_DryDinghy | 444 | 5.68 | TALL | not listed |
| SM_S1D2_Clipper | 3572 | 50.34 | TALL | not listed |

### s1d3 Pleasure Beach, 6.4 to 9.8 km

16 meshes, 20872 triangles. Generator `Content/Python/ft_world_bay_s1d3.py`. Builder's estimate: about 179 placed.

| mesh | triangles | top m | log | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D3_BigTop | 1328 | 12.69 | TALL | not listed |
| SM_S1D3_DuckGallery | 2512 | 9.08 | TALL | not listed |
| SM_S1D3_FunHouse | 966 | 13.94 | TALL | not listed |
| SM_S1D3_MirrorHall | 1772 | 16.55 | TALL | not listed |
| SM_S1D3_DodgemPavilion | 2112 | 12.95 | TALL | not listed |
| SM_S1D3_PennyPalace | 1146 | 12.62 | TALL | not listed |
| SM_S1D3_PuppetTheatre | 1024 | 9.5 | TALL | not listed |
| SM_S1D3_CandyFloss | 1032 | 10.2 | TALL | not listed |
| SM_S1D3_StrengthTower | 544 | 9.7 | TALL | not listed |
| SM_S1D3_TicketDrum | 588 | 6.07 | TALL | 0 / 2 / 6 |
| SM_S1D3_Lollipops | 384 | 5.96 | TALL | not listed |
| SM_S1D3_BuntingMast | 948 | 8.38 | TALL | not listed |
| SM_S1D3_PrizeStall | 1176 | 4.68 | TALL | not listed |
| SM_S1D3_MirrorRow | 572 | 1.9 | low | 0 / 4 / 13 |
| SM_S1D3_RockingHorses | 828 | 1.86 | low | 0 / 3 / 13 |
| SM_S1D3_BigSundae | 3940 | 45.74 | TALL | not listed |

### s1d4 Hotel Row, 9.8 to 13.2 km

20 meshes, 25240 triangles. Generator `Content/Python/ft_world_bay_s1d4.py`. Builder's estimate: about 203 placed.

| mesh | triangles | top m | log | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D4_Casino | 1640 | 28.26 | TALL | not listed |
| SM_S1D4_Ballroom | 1288 | 22.55 | TALL | not listed |
| SM_S1D4_PalmCourt | 1346 | 16.34 | TALL | not listed |
| SM_S1D4_RoofGarden | 1788 | 17.56 | TALL | not listed |
| SM_S1D4_Gatehouse | 830 | 9.28 | TALL | not listed |
| SM_S1D4_GrandHotel | 2384 | 35.16 | TALL | not listed |
| SM_S1D4_Belvedere | 2118 | 31.42 | TALL | not listed |
| SM_S1D4_Loggia | 2006 | 29.28 | TALL | not listed |
| SM_S1D4_Rotunda | 1990 | 29.26 | TALL | not listed |
| SM_S1D4_SpaHouse | 1382 | 11.83 | TALL | not listed |
| SM_S1D4_BathPavilion | 1480 | 14.35 | TALL | not listed |
| SM_S1D4_Fountain | 632 | 4.84 | TALL | not listed |
| SM_S1D4_Bandstand | 790 | 7.0 | TALL | not listed |
| SM_S1D4_Flagpoles | 582 | 9.94 | TALL | not listed |
| SM_S1D4_Valet | 516 | 3.25 | TALL | not listed |
| SM_S1D4_Lamp | 286 | 4.29 | TALL | not listed |
| SM_S1D4_Balustrade | 420 | 1.87 | low | 0 / 1 / 16 |
| SM_S1D4_Trolley | 512 | 1.92 | low | not listed |
| SM_S1D4_TeaTables | 766 | 1.81 | low | not listed |
| SM_S1D4_Shell | 2484 | 41.9 | TALL | not listed |

### s1d5 The Point, 13.2 to 16.8 km

19 meshes, 15240 triangles. Generator `Content/Python/ft_world_bay_s1d5.py`. Builder's estimate: about 227 by the brief's formula, about 199 counted slot by slot placed.

| mesh | triangles | top m | log | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D5_DomeVilla | 1294 | 8.46 | TALL | not listed |
| SM_S1D5_TowerHouse | 1046 | 11.66 | TALL | not listed |
| SM_S1D5_Windmill | 712 | 11.36 | TALL | not listed |
| SM_S1D5_Pavilion | 642 | 10.3 | TALL | not listed |
| SM_S1D5_StepRow | 1310 | 8.24 | TALL | not listed |
| SM_S1D5_GateHouse | 1386 | 7.51 | TALL | not listed |
| SM_S1D5_CubeHouse | 784 | 5.34 | TALL | not listed |
| SM_S1D5_TerraceVilla | 1530 | 8.83 | TALL | not listed |
| SM_S1D5_Well | 474 | 1.91 | low | 0 / 1 / 10 |
| SM_S1D5_Sundial | 424 | 1.72 | low | not listed |
| SM_S1D5_JarRow | 822 | 1.87 | low | 0 / 1 / 8 |
| SM_S1D5_Cairn | 304 | 4.54 | TALL | not listed |
| SM_S1D5_Scope | 282 | 1.86 | low | not listed |
| SM_S1D5_DryWall | 390 | 1.62 | low | not listed |
| SM_S1D5_Exedra | 340 | 1.48 | low | not listed |
| SM_S1D5_Dovecote | 604 | 5.6 | TALL | not listed |
| SM_S1D5_WashLine | 318 | 2.34 | TALL | not listed |
| SM_S1D5_FingerPost | 362 | 3.6 | TALL | not listed |
| SM_S1D5_ArchRock | 2216 | 40.15 | TALL | not listed |

### s1d6 Downtown waterfront, 16.8 to 21.1 km

21 meshes, 23976 triangles. Generator `Content/Python/ft_world_bay_s1d6.py`. Builder's estimate: 217 if the track leaves none out, about 163 with its usual share left out placed.

| mesh | triangles | top m | log | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D6_NightClub | 1726 | 20.7 | TALL | not listed |
| SM_S1D6_Theatre | 1432 | 17.18 | TALL | not listed |
| SM_S1D6_DeptStore | 1618 | 23.3 | TALL | not listed |
| SM_S1D6_CornerTower | 1398 | 28.5 | TALL | not listed |
| SM_S1D6_GlassOffice | 1638 | 39.8 | TALL | not listed |
| SM_S1D6_StepTower | 1680 | 39.07 | TALL | not listed |
| SM_S1D6_CarDeck | 1696 | 19.0 | TALL | not listed |
| SM_S1D6_WalkUp | 1080 | 17.0 | TALL | not listed |
| SM_S1D6_NeonBar | 1100 | 15.6 | TALL | not listed |
| SM_S1D6_Pavilion | 1500 | 10.41 | TALL | not listed |
| SM_S1D6_WinterGarden | 1336 | 12.24 | TALL | not listed |
| SM_S1D6_SkyBridge | 524 | 18.7 | TALL | not listed |
| SM_S1D6_NeonPalm | 280 | 8.95 | TALL | not listed |
| SM_S1D6_StreetClock | 546 | 6.88 | TALL | not listed |
| SM_S1D6_PosterDrum | 350 | 4.4 | TALL | not listed |
| SM_S1D6_NewsStand | 632 | 3.0 | TALL | not listed |
| SM_S1D6_TaxiRank | 444 | 2.86 | TALL | not listed |
| SM_S1D6_RopeCarpet | 500 | 1.14 | low | 0 / 6 / 10 |
| SM_S1D6_NewsRacks | 444 | 1.29 | low | 0 / 3 / 11 |
| SM_S1D6_PremiereWall | 372 | 1.72 | low | 0 / 5 / 10 |
| SM_S1D6_PicturePalace | 3680 | 46.1 | TALL | not listed |

### s1d7 The Old Harbour, 21.1 km to the end

16 meshes, 12816 triangles. Generator `Content/Python/ft_world_bay_s1d7.py`. Builder's estimate: 247 if the track leaves none out, about 185 with its usual share left out placed.

| mesh | triangles | top m | log | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1D7_NetLoft | 800 | 13.19 | TALL | not listed |
| SM_S1D7_SailLoft | 764 | 16.68 | TALL | not listed |
| SM_S1D7_Smokery | 784 | 13.23 | TALL | not listed |
| SM_S1D7_BoatShed | 792 | 9.5 | TALL | not listed |
| SM_S1D7_HarbourInn | 980 | 13.39 | TALL | not listed |
| SM_S1D7_IceHouse | 620 | 12.46 | TALL | not listed |
| SM_S1D7_TwinGables | 872 | 13.21 | TALL | not listed |
| SM_S1D7_DryRack | 744 | 1.72 | low | not listed |
| SM_S1D7_FishBoxes | 464 | 1.42 | low | not listed |
| SM_S1D7_Capstan | 392 | 1.46 | low | not listed |
| SM_S1D7_NetBoat | 246 | 1.52 | low | not listed |
| SM_S1D7_QuayCrane | 396 | 7.32 | TALL | not listed |
| SM_S1D7_Bollards | 628 | 0.94 | low | not listed |
| SM_S1D7_NetMast | 358 | 5.52 | TALL | not listed |
| SM_S1D7_QuayWall | 440 | 1.84 | low | not listed |
| SM_S1D7_BigFish | 3536 | 20.2 | TALL | not listed |

### s1plants: plants for all seven districts

22 meshes, 16546 triangles. Generator `Content/Python/ft_world_bay_s1plants.py`. Builder's estimate: about 539 (719 rule-slots x 0.75) placed.

| mesh | triangles | top m | log | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1PLANTS_RoyalPalms | 1338 | 16.11 | TALL | not listed |
| SM_S1PLANTS_FanPalm | 1096 | 6.6 | TALL | not listed |
| SM_S1PLANTS_CocoPalm | 794 | 11.75 | TALL | not listed |
| SM_S1PLANTS_Hibiscus | 716 | 1.65 | low | 0 / 2 / 19 |
| SM_S1PLANTS_Pergola | 860 | 3.97 | TALL | not listed |
| SM_S1PLANTS_SeaGrape | 640 | 1.62 | low | 0 / 5 / 26 |
| SM_S1PLANTS_Agave | 632 | 1.7 | low | 0 / 1 / 20 |
| SM_S1PLANTS_BirdOfParadise | 892 | 1.68 | low | not listed |
| SM_S1PLANTS_Topiary | 604 | 3.29 | TALL | not listed |
| SM_S1PLANTS_FigBalls | 576 | 2.78 | TALL | not listed |
| SM_S1PLANTS_PricklyPear | 548 | 1.68 | low | 0 / 1 / 15 |
| SM_S1PLANTS_Geraniums | 826 | 1.47 | low | 0 / 1 / 11 |
| SM_S1PLANTS_BigFig | 1392 | 11.78 | TALL | not listed |
| SM_S1PLANTS_UmbrellaPine | 576 | 11.02 | TALL | not listed |
| SM_S1PLANTS_FlameTree | 708 | 7.5 | TALL | not listed |
| SM_S1PLANTS_Mangrove | 1012 | 5.53 | TALL | not listed |
| SM_S1PLANTS_QuayTree | 566 | 7.07 | TALL | not listed |
| SM_S1PLANTS_Reeds | 688 | 3.13 | TALL | not listed |
| SM_S1PLANTS_CarpetSun | 400 | 0.76 | low | 0 / 2 / 16 |
| SM_S1PLANTS_CarpetWedge | 588 | 0.54 | low | 0 / 1 / 15 |
| SM_S1PLANTS_CarpetWave | 534 | 0.48 | low | not listed |
| SM_S1PLANTS_CarpetNeon | 560 | 0.48 | low | not listed |

### s1animals: animals on the ground, afloat and in the sky

20 meshes, 17688 triangles. Generator `Content/Python/ft_world_bay_s1animals.py`. Builder's estimate: about 264 placed.

| mesh | triangles | top m | log | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1ANIMALS_PelicanPosts | 1208 | 1.83 | low | not listed |
| SM_S1ANIMALS_Sandpipers | 1042 | 0.62 | low | not listed |
| SM_S1ANIMALS_Crabs | 1148 | 1.22 | low | not listed |
| SM_S1ANIMALS_MacawPerch | 646 | 1.67 | low | not listed |
| SM_S1ANIMALS_CatsPots | 760 | 1.4 | low | not listed |
| SM_S1ANIMALS_Peacocks | 1244 | 1.85 | low | not listed |
| SM_S1ANIMALS_GoatWall | 988 | 2.29 | TALL | not listed |
| SM_S1ANIMALS_SealRock | 816 | 2.48 | TALL | not listed |
| SM_S1ANIMALS_BeachDog | 690 | 1.71 | low | not listed |
| SM_S1ANIMALS_Doves | 1312 | 1.36 | low | not listed |
| SM_S1ANIMALS_HeronReeds | 1224 | 1.84 | low | not listed |
| SM_S1ANIMALS_Donkey | 758 | 1.72 | low | not listed |
| SM_S1ANIMALS_CarriageHorse | 982 | 2.2 | TALL | not listed |
| SM_S1ANIMALS_Tortoise | 458 | 0.78 | low | 0 / 1 / 5 |
| SM_S1ANIMALS_SwanPool | 786 | 1.65 | low | not listed |
| SM_S1ANIMALS_Turtles | 624 | 0.38 | low | not listed |
| SM_S1ANIMALS_FlyingFish | 864 | -0.09 | low | not listed |
| SM_S1ANIMALS_Whale | 728 | 3.19 | TALL | not listed |
| SM_S1ANIMALS_Spoonbills | 658 | 1.24 | low | not listed |
| SM_S1ANIMALS_TernRing | 752 | 3.66 | TALL | not listed |

### s1people: people raised on podiums, decks, terraces and wagons

19 meshes, 29036 triangles. Generator `Content/Python/ft_world_bay_s1people.py`. Builder's estimate: about 220 (294 piece-places x 0.75) placed.

| mesh | triangles | top m | log | FTCLEAR left out / moved / placed |
|---|---|---|---|---|
| SM_S1PEOPLE_JukePodium | 1524 | 6.08 | TALL | not listed |
| SM_S1PEOPLE_DinerSteps | 1516 | 5.92 | TALL | not listed |
| SM_S1PEOPLE_Wagon_A | 1368 | 5.15 | TALL | not listed |
| SM_S1PEOPLE_Wagon_B | 1368 | 5.15 | TALL | not listed |
| SM_S1PEOPLE_NetDeck | 1444 | 5.05 | TALL | not listed |
| SM_S1PEOPLE_HullPainters | 1352 | 6.9 | TALL | not listed |
| SM_S1PEOPLE_Veranda_A | 1572 | 7.4 | TALL | not listed |
| SM_S1PEOPLE_Veranda_B | 1556 | 7.4 | TALL | not listed |
| SM_S1PEOPLE_ShowPlinths | 1548 | 5.5 | TALL | not listed |
| SM_S1PEOPLE_StiltWalkers | 1708 | 4.02 | TALL | not listed |
| SM_S1PEOPLE_BalusTerrace | 1840 | 5.26 | TALL | not listed |
| SM_S1PEOPLE_StringTrio | 1356 | 6.66 | TALL | not listed |
| SM_S1PEOPLE_PaintersTerrace | 1488 | 4.32 | TALL | not listed |
| SM_S1PEOPLE_PicnicWall | 1600 | 3.91 | TALL | not listed |
| SM_S1PEOPLE_PremiereSteps | 1680 | 6.13 | TALL | not listed |
| SM_S1PEOPLE_PhotoRiser | 1492 | 5.41 | TALL | not listed |
| SM_S1PEOPLE_FishStalls | 1860 | 5.7 | TALL | not listed |
| SM_S1PEOPLE_NetHaulers | 1296 | 4.21 | TALL | not listed |
| SM_S1PEOPLE_Accordion | 1468 | 6.4 | TALL | not listed |

## What was left out

- No block was left out and no line of any block was changed or removed: ten of ten are in, and every one of the 187
  meshes is named by a block.
- There is no `L0_s1d8.inl`: the plan has seven districts on this level.
- By the game: none of the listed new meshes had a placement left out of the road (FTCLEAR's total stayed 51).
  FTDRESS tried 2459 more pieces and placed 2439 more; which 20 it did not place, and why, is not checked.

## Not checked

- Only one frame was looked at, 6 s into the level (`C:\ftwork\out\s1\after_L0\L0_t006.png`): the level starts and
  draws. No new piece was judged in it. No drive along the level, no review of any district.
- Placed counts of the 168 meshes FTCLEAR does not list; whether every one of the 187 meshes loaded and stands somewhere.
- Frame time and memory with 2439 more pieces and 180 more moving components.
- The play rules (nothing nearer than 17 m, tall from 24 m, steady light from 24 m, nothing like a pickup, an enemy
  or the boss, no real names on signs, no brand or likeness): stated by the builders, not checked by the integrator.
- Whether heroes or buildings stand inside the level's own hotels, wheels or funfair (the plan marks the town-side
  hero bands "NOT checked free").
- The builders' estimates against the game's counts, mesh by mesh.
- Two builders (s1d5, s1d7) reported one read-only `git status` each in the checkout; `git status` before the paste
  showed only the files listed under "What was run".

## Review and fix round (16 Nov 2026)

One fix round after two reviews (a rules reader, who evaluated both tables with the compiler and box-tested every Stage 1
placement, and a driver, eight game runs; their files are in `C:\ftwork\out\L0s1rev`). This section says what the fixer
ran and saw. Changed: Stage 1's blocks in `Source/FullThrottle/FTPeopleL0.inl` (and one new block `s1fix` at its very
end), `Content/Python/ft_world_bay_s1d5.py`, the mesh `SM_S1D5_ArchRock`, this note. Nothing else.

### Found

- Rule broken: none, by either reviewer (17 m, tall from 24 m, light, movement, travel paths, look-alikes, words).
- Looks wrong (rules reader, 11): Stage 1 pieces standing in older things: the chair lift (1, 2), the beach race (3),
  the side streets' walkers (4, 5), the third pass's beach scenes (6), the film crew (7), the cliff road's banked
  esses (8), the Point's side streets (9), the watchers' stand (10), a department store's corner (11).
- Looks wrong (driver, 2): an unreadable heap at 7.71 km behind the town barrier; a level palm in front of the Shell.
- Weak: palms in the chair lift's line, walkers clipping three raised pieces, people beside jumps, six smaller
  overlaps (rules reader 12 to 15); the old shop boards at the kerb of the last two districts, three heroes that do
  not read from the road, the Marina's and the Old Harbour's town side (driver 2 to 7).

### Fixed (each verified in the file first; numbers are boxes from the make logs, re-evaluated after the change)

How the table was changed: a rule's random numbers come from its place in the table, so no rule was added or removed
before the s1people block (the evaluator's dump: every one of the 5976 rules before it keeps its index; 51 of them
changed in place). s1people's rules are certain Sites, so rules could be added there and after it.

| finding | what was changed |
|---|---|
| reader 1, pavilion in the chair lift | `WldS1D6BayD` in slots 390, 394, 426 anchored 57 m out (was 45.5; the cables run 41.5 and 46.5 m). In slot 378 the Site is switched off (chance -1): a beach bar stands 54 m out there. |
| reader 2, lift masts in the raised people | Pleasure Beach bay Set: stilt walkers -19.5 to -14.0, wagon 19.0 to 13.5 (masts 20.4 m either way). Downtown bay: no people Set in slots 377, 393, 409, 425 (the beach scene fills the place beside the mast, and the steps reached the chairs' height); the bay rules are written in four stretches without them. |
| reader 3, beach race through heroes | the second Shell moved from slot 268 to 266 (12.03 km; the race of slot 269 begins 75 m after it). The Big Fish anchored 152 m out (was 140; the race's line ends at 144 m). |
| reader 4, Marina props in walkers' lanes | the three town Sets: every piece now begins 8.5 m or more from the street's middle (11 offsets; chandlery 15.5, boat shed -16.5). |
| reader 5, fin totem in the lane | `WldS1D1CornerB`: the fin totem to -10.5 along, 35.2 m out (the reader's -10.5 alone would have stood it inside the milk bar's box). |
| reader 6, bandstand and fountain in beach scenes | `WldS1D4LawnA/B`: -16 to -19 along. |
| reader 7, kerb pieces under the film crew | `WldS1D4Kerb`: tea tables and trolley back to 22.6 m (they end 23.9 m; the crew's units begin 24.55 m), balustrades 2.5 m further apart. Not by splitting the stretches: that adds rules. Single slots: the Strip's bollards to 23.7 m (film crew, slot 40), the neon arrow to -9.2 along (crane, slot 32), the Point's stepped row 2.2 m back (crane, slot 308). |
| reader 8, near pieces beside the banked esses | s1d3's "after the bend" zone ends at 9.21 km (was 9.8): slots 204 to 216 lose kerb, pavement and mast pieces. |
| reader 9, plants and animals in the Point's side streets | 18 plant rules and 9 animal rules (town side) keep one slot in three at their old place; 12 rules in the new block `s1fix` add the other slot that is no side street. After: 0 town-side plant or animal placements in a slot s % 6 == 3 (23 before). |
| reader 10, pelicans in the watchers' stand | the Old Harbour's seven quay rules are Sites now: one Set, 30 m out, 12 m before the middle (the stand is 7.3 to 22.7 m after it). |
| reader 11, premiere steps in the store | `WldS1PEOPLEDownTown`: steps -17.5 to -15.0. |
| reader 13, walkers clip raised pieces | the bay podium (Strip), string trio (Hotel Row) and premiere steps (Downtown) stand 1.2 / 1.2 / 0.6 m further out. |
| reader 15, two of six | the Point's egrets from 48.5 m (clear of the painters' terrace); the stepped tower 0.9 m back in three Downtown Sets (it stood inside the theatre's back). |
| driver 1, the heap at 7.71 km | identified from the frame and the slot (170): the rocking horses at the kerb with the ribbon mast of `WldS1D3StreetB` straight behind them. The mast is taken out of that Set (it still stands at the side streets). |
| driver 3, the Picture Palace | its three Sites place it at 1.3 times its size (60 m tall, 121 to 149 m out). |
| driver 4, Arch Rock | the arch's two rock greys one step darker (`#8a86a0`, `#6f6a88`, both the kit's); regenerated: "pieces built 1 failed 0", 2216 triangles, size and top unchanged. |

### Not fixed, and why

- Reader 12 (palms in the chair lift's line): the crowns are 7.7 m in radius; clear of the cables means 54 m and
  more out, where the same zones' other palm and flame tree rules already stand. No small change does it.
- Reader 14 (people 43 m out beside four jumps): they stand outside the 40 m the track itself keeps open at a run-up
  or landing; left as built.
- Reader 15, the rest: the Sunburst and the far crowd of slot 51, the Ferris wheel behind the Big Sundae and the Big
  Fish, crowns of two plant rules in one slot, no plants in slots 292 to 294: chance overlaps or gaps; a fix would
  be new placing.
- Driver 2 (old shop boards at the kerb of Downtown and the Old Harbour): not Stage 1's rows; the integrator's call.
- Driver 5 (the Sunburst from the right lane): left, as the driver allows; larger it would reach further into the
  far crowd of slot 51.
- Driver 6 and 7 (the Marina and the Old Harbour from the town side): needs new pieces in new places, a second
  round of invention.
- Driver 8 (a level palm in front of the first Shell): the palm is the level's own.

### After the fix (one start of the level, same script; `C:\ftwork\out\s1\fixed_L0\run.log`)

- Build (`build.ps1`, tag L0s1_fix_build): "build exit 0 after 90 s", "Result: Succeeded"; 1 line with "warning"
  (the engine's Fab plugin dependency, not this module).
- `FTTRACK level=0 frames=12717 route=dc9e032b solid=d2e759f4 solidTriangles=986296`: equal to the level's line in
  `SourceArt/Jam/validate/track_baseline.txt` (compared with diff).
- `FTDRESS level=0 districts=1424 sets=221 pieces placed=15805 of 15936 tried shadows=9379 of 16453`
  (before the fix: 1416, 220, 15854 of 15988, 9427 of 16502). 49 fewer placed.
- FTCLEAR: props kept 17893 (17942), moved off their own barrier 1159 (1165), left out of the road 51 (51).
  FTWPO life components 292 of 403 (the same).
- "Fatal error" 0, "LogFT: Error" 0, lines naming `/Game/World` with fail / can't find / error / warning 0,
  "Ensure condition failed" 0. `FT WARM no assets found` once, as before.

### Not checked

- No drive after the fix: one frame only (`fixed_L0\L0_t006.png`, 6 s into the level: it starts and draws). None of
  the fixes was looked at in a frame, the darker Arch Rock and the larger Picture Palace included.
- All overlap tests are bounding boxes from the make logs; the level's own props (FTTrack.cpp) and the 67 older
  meshes without a logged size were not tested against the moved pieces.
- Which slots carry loop, jump or bend flags in the game; whether the pieces moved to 57 m and 152 m float or sink.
- The larger Picture Palace against the far crowd of slot 405 (a chance overlap that was there at size 1 too).
- Frame time and memory.
- The driver's contact sheet (`C:\ftwork\out\L0s1rev\L0_s1_review.jpg`) is 1.37 MB, over the 1 MB limit: not copied.
