# Level 6 "Under the Sea": Stage 1 (the world), integration note (16 Nov 2026)

Only what was run and seen. Checkout `C:\ftwork\wt\L6`, branch `claude/world-L6`, on top of `df6441f7`.

## What went in

Eleven builders' blocks, each wrapped in `{ }`, appended in this order to the END of
`Source/FullThrottle/FTPeopleL6.inl` (lines 1132 to 3905): s1d1, s1d2, s1d3, s1d4, s1d5, s1d6, s1d7, s1d8, s1plants,
s1animals, s1people. No block was left out and no line of any block was changed: the first compile passed
(`build exit 0 after 160 s`, `Result: Succeeded`, 0 lines with "error" in `C:\ftwork\logs\L6s1_build.log`).

Checked before pasting, for each block: its generator exists, its make log ends `pieces built N failed 0` with 0
`LogPython: Error` and 0 `FAILED` lines, every mesh name used in the block has its `.uasset` in
`Content/World/UnderTheSea`, no Set name is used twice, every Set is named `WldS1...`, the blocks are ASCII.

The generators are named `Content/Python/ft_world_sea_<id>.py` (the brief's people key `sea`), not `ft_world_abyss_*`.
194 new meshes, `/Game/World/UnderTheSea/SM_S1*`, 11 MB together, none over 40 MB; 203,794 triangles in all (sum of the
make logs' `triangles`; a mesh is drawn once per placement).

## The zone plan in short

| id | district | km | look | hero |
|---|---|---|---|---|
| s1d1 | The reef | 0.0-1.7 | a village grown out of coral: urchin huts, conch houses, shell roofs | THE GREAT CONCH |
| s1d2 | The kelp forest | 1.7-3.4 | the kelp farmers' stilt village, yellow thatched roofs | THE REEL |
| s1d3 | The research station | 3.4-5.0 | white drum stacks, glass domes, dishes, all on three legs | THE BIG EAR |
| s1d4 | The salvage field and the approach | 5.0-7.94 | the wreckers' town made of ships | THE STANDING STERN |
| s1d5 | Nereida: the Pearl Quarter | 7.94-12.2 | sunken classical: colonnades, pink domes | THE GREAT PEARL |
| s1d6 | Nereida: the Bubble Works | 12.2-16.1 | glass bubbles on copper stems | THE BUBBLE TREE |
| s1d7 | Nereida: Lamp Row | 16.1-20.6 | red and white banded lamp towers | THE GREAT LAMP |
| s1d8 | Nereida: the Crown | 20.6-25.1 | violet palace gardens, gold domes | THE CROWN |

Across all eight: s1plants (16 plants, slots s % 4 == 3), s1animals (16 animal pieces, ground and far water),
s1people (26 raised groups on plinths, stairs and decks, slots s % 8 == 1). Full plan: `C:\ftwork\final\s1\L6_plan.md`.

## Before and after (the game's own lines, same start script `C:\ftwork\jobs\p3\start_L6.txt`)

| | districts | sets | placed | tried | shadows |
|---|---|---|---|---|---|
| before (`C:\ftwork\out\p3\final_L6\run.log`) | 370 | 27 | 13377 | 13394 | 10608 of 14232 |
| after (`C:\ftwork\out\s1\after_L6\run.log`) | 751 | 211 | 16138 | 16159 | 12688 of 16993 |

Stage 1 adds 2,761 placed pieces (2,765 tried). FTCLEAR props kept 16720 -> 19481, moved off their own barrier
1719 -> 1794, left out of the road 26 -> 30.

Track: `FTTRACK level=6 frames=12470 route=04d3f899 solid=8927e339 solidTriangles=930148` in the run, equal to the
`^FTTRACK level=6 ` line of `SourceArt/Jam/validate/track_baseline.txt` (compared as strings).

Errors in the after run: "Fatal error" 0, "LogFT: Error" 0, load failures naming `/Game/World` 0.

One engine ensure, NOT caused by Stage 1: `Ensure condition failed: OriginX <= OriginMax ... DoubleFloat.cpp`
("precision loss while converting matrix to GPU format") once, about 4 s into the drive, and the script's
`waitfor FT CLOCK level=6.0` timed out because the game ran at about 3 to 5 frames a second and the clock line skipped
6.0 (the frame was taken at 134 s instead of 6 s). Five other Unreal editors were running on the PC at that time
(the menu itself ran at about 4 frames a second before the level was loaded). Control: the same binary rebuilt WITHOUT the
Stage 1 blocks (FTDRESS back to 13377 of 13394), run under the same load (`C:\ftwork\out\s1\ctl_L6_no_s1`), gave the
same ensure and the same timeout. With `-ftnodress` (`C:\ftwork\out\s1\after_L6_nodress`): no ensure, clock 6.0 found.
The quiet-machine before run had neither. So: an ensure that level 6 with its dressing shows when frames are very slow;
it was there before Stage 1. A second after run (`C:\ftwork\out\s1\after_L6_run2`) gave the same FTTRACK, FTDRESS and
FTCLEAR numbers. First start built 120 distance fields for the new meshes (none in the second run).

## What the track left out or moved (FTCLEAR, after run)

FTCLEAR names a mesh only when it left out or moved one; it names 31 of the 194 new meshes. Left out, 4 pieces:
SM_S1D2_PressHouse 1, SM_S1D2_FloatStore 1, SM_S1D4_WinchNet 1, SM_S1D4_AwningStall 1. Moved (pushed out from the
road), the most: SM_S1D5_PearlLamp 11 of 35, SM_S1D8_BubbleFountain 11 of 11, SM_S1D8_LampTree 9 of 14.

## Every new mesh

Triangles, top and TALL/low from each builder's make log (`C:\ftwork\logs\L6<id>_make.log`, the same numbers as the
hand-overs). FTCLEAR column: `-left out/~moved/placed`, blank = not listed by FTCLEAR (its placed count: not checked).

### s1d1 (18 meshes, 19926 triangles)

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1D1_UrchinHut | 1568 | 7.42 | TALL |  |
| SM_S1D1_LimpetStore | 1158 | 5.58 | TALL |  |
| SM_S1D1_ConchHouse | 2052 | 12.62 | TALL |  |
| SM_S1D1_ScallopMarket | 1680 | 6.09 | TALL |  |
| SM_S1D1_BarnacleTower | 1898 | 13.4 | TALL |  |
| SM_S1D1_ClamBandShell | 1250 | 8.34 | TALL |  |
| SM_S1D1_BrainBath | 1684 | 5.07 | TALL |  |
| SM_S1D1_GreatConch | 3948 | 48.44 | TALL |  |
| SM_S1D1_CowrieStools | 334 | 0.92 | low | -0/~1/12 |
| SM_S1D1_BasketStack | 646 | 1.26 | low | -0/~1/12 |
| SM_S1D1_PearlFence | 576 | 1.23 | low | -0/~2/24 |
| SM_S1D1_ConchPlanters | 564 | 1.48 | low | -0/~2/24 |
| SM_S1D1_CoralLamp | 328 | 4.83 | TALL | -0/~2/19 |
| SM_S1D1_ShellSign | 492 | 4.12 | TALL | -0/~1/10 |
| SM_S1D1_ShellChimes | 348 | 5.03 | TALL |  |
| SM_S1D1_SpongeRack | 608 | 3.06 | TALL |  |
| SM_S1D1_ShellWell | 448 | 3.13 | TALL | -0/~1/9 |
| SM_S1D1_WashLine | 344 | 3.39 | TALL | -0/~1/9 |

### s1d2 (17 meshes, 14832 triangles)

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1D2_StiltHouse | 1136 | 28.76 | TALL |  |
| SM_S1D2_TallHouse | 1256 | 29.84 | TALL |  |
| SM_S1D2_DryingBarn | 1056 | 23.19 | TALL |  |
| SM_S1D2_MastHouse | 1072 | 34.2 | TALL |  |
| SM_S1D2_RopeWalk | 1168 | 20.14 | TALL |  |
| SM_S1D2_PressHouse | 1528 | 25.11 | TALL | -1/~0/12 |
| SM_S1D2_FloatStore | 1200 | 20.36 | TALL | -1/~0/12 |
| SM_S1D2_RopeBridge | 1048 | 16.91 | TALL |  |
| SM_S1D2_DryingRack | 248 | 4.75 | TALL |  |
| SM_S1D2_FloatLantern | 292 | 5.42 | TALL |  |
| SM_S1D2_BaleStack | 264 | 4.36 | TALL |  |
| SM_S1D2_ScytheRack | 428 | 4.37 | TALL |  |
| SM_S1D2_BellPost | 336 | 5.81 | TALL |  |
| SM_S1D2_RopeSledge | 296 | 1.4 | low | -0/~4/8 |
| SM_S1D2_PlankWalk | 424 | 1.57 | low | -0/~3/5 |
| SM_S1D2_SheafRow | 492 | 1.36 | low | -0/~3/7 |
| SM_S1D2_TheReel | 2588 | 65.7 | TALL |  |

### s1d3 (17 meshes, 15448 triangles)

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1D3_DrumLab | 1776 | 27.56 | TALL |  |
| SM_S1D3_Greenhouse | 1222 | 9.18 | TALL |  |
| SM_S1D3_TankHall | 1068 | 12.6 | TALL |  |
| SM_S1D3_RingDorm | 1184 | 14.1 | TALL |  |
| SM_S1D3_Workshop | 740 | 12.55 | TALL |  |
| SM_S1D3_CurrentTower | 1236 | 30.07 | TALL |  |
| SM_S1D3_DeckHouse | 1338 | 12.36 | TALL |  |
| SM_S1D3_Tripod | 288 | 1.86 | low | -0/~3/11 |
| SM_S1D3_SampleRack | 732 | 1.72 | low | -0/~2/8 |
| SM_S1D3_Lockers | 336 | 1.93 | low | -0/~1/14 |
| SM_S1D3_CableDrum | 448 | 1.82 | low | -0/~2/7 |
| SM_S1D3_Manifold | 632 | 1.7 | low | -0/~2/12 |
| SM_S1D3_CurrentVane | 276 | 5.1 | TALL |  |
| SM_S1D3_FloodMast | 372 | 9.13 | TALL |  |
| SM_S1D3_SignPost | 228 | 3.65 | TALL |  |
| SM_S1D3_AntennaFarm | 536 | 6.74 | TALL |  |
| SM_S1D3_BigEar | 3036 | 61.32 | TALL |  |

### s1d4 (16 meshes, 12570 triangles)

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1D4_HullHouse | 824 | 9.06 | TALL |  |
| SM_S1D4_FunnelTower | 1040 | 18.61 | TALL |  |
| SM_S1D4_BridgeInn | 820 | 10.96 | TALL |  |
| SM_S1D4_JibWorkshop | 844 | 12.72 | TALL |  |
| SM_S1D4_LifeboatRow | 1244 | 7.64 | TALL |  |
| SM_S1D4_ForgeRock | 592 | 16.17 | TALL |  |
| SM_S1D4_Tenement | 1400 | 12.56 | TALL |  |
| SM_S1D4_ChainFence | 420 | 0.96 | low | -0/~1/19 |
| SM_S1D4_PortholeLamp | 312 | 4.76 | TALL |  |
| SM_S1D4_PropMonument | 516 | 5.3 | TALL |  |
| SM_S1D4_WheelCapstan | 568 | 1.74 | low |  |
| SM_S1D4_AwningStall | 500 | 3.64 | TALL | -1/~0/19 |
| SM_S1D4_WinchNet | 368 | 6.62 | TALL | -1/~0/15 |
| SM_S1D4_SignalMast | 432 | 7.88 | TALL |  |
| SM_S1D4_BoilerPlanter | 354 | 1.39 | low |  |
| SM_S1D4_StandingStern | 2336 | 68.8 | TALL |  |

### s1d5 (16 meshes, 12332 triangles)

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1D5_Hall | 1196 | 12.18 | TALL |  |
| SM_S1D5_BathHouse | 880 | 14.05 | TALL |  |
| SM_S1D5_StepTemple | 856 | 14.56 | TALL |  |
| SM_S1D5_ShopArcade | 1464 | 11.38 | TALL |  |
| SM_S1D5_Treasury | 1052 | 13.62 | TALL |  |
| SM_S1D5_ArchGate | 828 | 12.74 | TALL |  |
| SM_S1D5_TownHouse | 744 | 13.65 | TALL |  |
| SM_S1D5_BrokenColumn | 198 | 1.77 | low |  |
| SM_S1D5_BenchUrn | 240 | 1.87 | low |  |
| SM_S1D5_PlanterRail | 288 | 1.82 | low |  |
| SM_S1D5_PearlLamp | 252 | 5.46 | TALL | -0/~11/35 |
| SM_S1D5_Seahorse | 240 | 6.62 | TALL |  |
| SM_S1D5_Fountain | 468 | 5.36 | TALL |  |
| SM_S1D5_BannerMast | 264 | 10.3 | TALL | -0/~3/15 |
| SM_S1D5_AmphoraStall | 698 | 3.18 | TALL |  |
| SM_S1D5_GreatPearl | 2664 | 55.55 | TALL |  |

### s1d6 (20 meshes, 25280 triangles)

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1D6_ThreeBubble | 1700 | 18.72 | TALL |  |
| SM_S1D6_GrapeTenement | 1980 | 20.84 | TALL |  |
| SM_S1D6_FurnaceHall | 1560 | 20.6 | TALL |  |
| SM_S1D6_BlowpipeTower | 1232 | 27.37 | TALL |  |
| SM_S1D6_BottleShop | 1572 | 15.6 | TALL |  |
| SM_S1D6_PumpHouse | 1788 | 18.22 | TALL |  |
| SM_S1D6_DomeRow | 2128 | 9.7 | TALL |  |
| SM_S1D6_StemVilla | 1740 | 20.58 | TALL |  |
| SM_S1D6_BubbleLamp | 660 | 6.52 | TALL |  |
| SM_S1D6_FloatRack | 1140 | 4.38 | TALL |  |
| SM_S1D6_SeahorseVane | 540 | 8.91 | TALL |  |
| SM_S1D6_ChimneyStack | 576 | 16.11 | TALL |  |
| SM_S1D6_BottleTree | 1052 | 5.1 | TALL |  |
| SM_S1D6_SignPost | 676 | 5.11 | TALL |  |
| SM_S1D6_GlassStall | 772 | 3.04 | TALL |  |
| SM_S1D6_MouldBench | 492 | 1.84 | low |  |
| SM_S1D6_PipeVent | 604 | 1.82 | low | -0/~2/23 |
| SM_S1D6_SandSledge | 328 | 1.61 | low | -0/~1/23 |
| SM_S1D6_GazingGlobes | 796 | 1.43 | low |  |
| SM_S1D6_BubbleTree | 3944 | 71.68 | TALL |  |

### s1d7 (17 meshes, 18602 triangles)

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1D7_LampTower_A | 1432 | 20.58 | TALL |  |
| SM_S1D7_LampTower_B | 1548 | 25.65 | TALL |  |
| SM_S1D7_TwinTowers | 1516 | 21.64 | TALL |  |
| SM_S1D7_KeeperCottage | 890 | 7.5 | TALL |  |
| SM_S1D7_PilotHouse | 1196 | 13.3 | TALL |  |
| SM_S1D7_BellTower | 888 | 14.73 | TALL |  |
| SM_S1D7_ChartHouse | 1260 | 9.25 | TALL |  |
| SM_S1D7_BuoyShed | 1856 | 7.41 | TALL |  |
| SM_S1D7_BuoySide | 376 | 2.38 | TALL |  |
| SM_S1D7_BollardRow | 596 | 0.93 | low | -0/~1/15 |
| SM_S1D7_BracketLamp | 312 | 5.15 | TALL | -0/~1/17 |
| SM_S1D7_SignalMast | 404 | 11.44 | TALL |  |
| SM_S1D7_OilJarRack | 868 | 1.85 | low |  |
| SM_S1D7_LadderBench | 324 | 1.71 | low |  |
| SM_S1D7_Spyglass | 312 | 2.8 | TALL |  |
| SM_S1D7_GalleryWalk | 1116 | 7.62 | TALL |  |
| SM_S1D7_GreatLamp | 3708 | 82.79 | TALL |  |

### s1d8 (15 meshes, 14752 triangles)

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1D8_OnionPavilion | 1194 | 18.57 | TALL |  |
| SM_S1D8_CrystalSpireHouse | 1000 | 24.66 | TALL |  |
| SM_S1D8_TerraceBlock | 1854 | 22.82 | TALL |  |
| SM_S1D8_GateLodge | 992 | 16.2 | TALL |  |
| SM_S1D8_MusicRotunda | 1182 | 15.01 | TALL |  |
| SM_S1D8_Orangery | 1122 | 14.64 | TALL |  |
| SM_S1D8_ArchBridge | 1388 | 18.55 | TALL |  |
| SM_S1D8_Urn | 246 | 1.89 | low | -0/~2/28 |
| SM_S1D8_SeahorseTopiary | 286 | 4.21 | TALL | -0/~2/11 |
| SM_S1D8_LampTree | 454 | 6.0 | TALL | -0/~9/14 |
| SM_S1D8_HedgeGate | 316 | 1.89 | low |  |
| SM_S1D8_BubbleFountain | 442 | 4.72 | TALL | -0/~11/11 |
| SM_S1D8_PennantMast | 280 | 9.38 | TALL |  |
| SM_S1D8_Parterre | 320 | 1.8 | low |  |
| SM_S1D8_Crown | 3676 | 85.0 | TALL |  |

### s1plants (16 meshes, 15216 triangles)

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1PLANTS_SeaFans | 952 | 3.0 | TALL |  |
| SM_S1PLANTS_TableCoral | 796 | 4.16 | TALL |  |
| SM_S1PLANTS_BrainCoral | 1068 | 1.33 | low |  |
| SM_S1PLANTS_TubeSponges | 900 | 2.46 | TALL |  |
| SM_S1PLANTS_SeaPalm | 1336 | 9.01 | TALL |  |
| SM_S1PLANTS_RibbonHedge | 812 | 1.47 | low |  |
| SM_S1PLANTS_SeaLettuce | 824 | 0.72 | low |  |
| SM_S1PLANTS_SeaPens | 1192 | 2.5 | TALL |  |
| SM_S1PLANTS_SpiralWhip | 932 | 5.08 | TALL |  |
| SM_S1PLANTS_WreckRib | 756 | 6.02 | TALL |  |
| SM_S1PLANTS_TubeWorms | 788 | 3.14 | TALL |  |
| SM_S1PLANTS_PearlVine | 1188 | 4.31 | TALL |  |
| SM_S1PLANTS_GardenUrn | 608 | 2.24 | TALL |  |
| SM_S1PLANTS_SeaLily | 1296 | 8.26 | TALL |  |
| SM_S1PLANTS_LanternWeed | 1188 | 4.03 | TALL |  |
| SM_S1PLANTS_StarMoss | 580 | 0.36 | low |  |

### s1animals (16 meshes, 17186 triangles)

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1ANIMALS_HermitCrabs | 1154 | 3.11 | TALL |  |
| SM_S1ANIMALS_Starfish | 1040 | 1.82 | low |  |
| SM_S1ANIMALS_SeaSlugs | 1000 | 2.29 | TALL |  |
| SM_S1ANIMALS_Flounders | 984 | 1.61 | low |  |
| SM_S1ANIMALS_Shrimps | 1436 | 3.09 | TALL |  |
| SM_S1ANIMALS_SeaPigs | 1080 | 2.41 | TALL |  |
| SM_S1ANIMALS_PigKeeper | 1128 | 3.25 | TALL |  |
| SM_S1ANIMALS_SpiderCrabs | 1180 | 2.76 | TALL |  |
| SM_S1ANIMALS_ParrotFish | 1186 | 5.07 | TALL |  |
| SM_S1ANIMALS_Cuttlefish | 1000 | 6.77 | TALL |  |
| SM_S1ANIMALS_Puffers | 1152 | 7.3 | TALL |  |
| SM_S1ANIMALS_Lionfish | 1232 | 7.66 | TALL |  |
| SM_S1ANIMALS_Seals | 932 | 10.92 | TALL |  |
| SM_S1ANIMALS_BaitBall | 1152 | 10.33 | TALL |  |
| SM_S1ANIMALS_Sunfish | 872 | 7.53 | TALL |  |
| SM_S1ANIMALS_SeaAngels | 658 | 6.09 | TALL |  |

### s1people (26 meshes, 37650 triangles)

| mesh | triangles | top m | | FTCLEAR |
|---|---|---|---|---|
| SM_S1PEOPLE_D1_Gardeners | 1378 | 6.56 | TALL |  |
| SM_S1PEOPLE_D1_PearlStair | 1460 | 6.29 | TALL |  |
| SM_S1PEOPLE_D1_Conch | 1484 | 8.9 | TALL |  |
| SM_S1PEOPLE_D2_StiltDeck | 1256 | 7.5 | TALL |  |
| SM_S1PEOPLE_D2_CuttersMast | 1520 | 18.1 | TALL |  |
| SM_S1PEOPLE_D3_RoofDeck | 1450 | 8.82 | TALL |  |
| SM_S1PEOPLE_D3_DiverClass | 1548 | 4.85 | TALL |  |
| SM_S1PEOPLE_D4_HullDeck | 1332 | 6.43 | TALL |  |
| SM_S1PEOPLE_D4_Forge | 1312 | 8.26 | TALL |  |
| SM_S1PEOPLE_D4_BridgeHouse | 1376 | 10.5 | TALL |  |
| SM_S1PEOPLE_D5_TempleSteps | 1334 | 6.9 | TALL |  |
| SM_S1PEOPLE_D5_Guards | 1502 | 9.94 | TALL |  |
| SM_S1PEOPLE_D5_Arcade | 1762 | 10.1 | TALL |  |
| SM_S1PEOPLE_D5_GrandStair | 1628 | 9.21 | TALL |  |
| SM_S1PEOPLE_D6_BlowDeck | 1284 | 8.39 | TALL |  |
| SM_S1PEOPLE_D6_FloatStairs | 1502 | 11.2 | TALL |  |
| SM_S1PEOPLE_D6_BubbleTerrace | 1060 | 9.65 | TALL |  |
| SM_S1PEOPLE_D6_Blowpipe | 1464 | 11.14 | TALL |  |
| SM_S1PEOPLE_D7_Keepers | 1448 | 9.25 | TALL |  |
| SM_S1PEOPLE_D7_Pilots | 1428 | 7.0 | TALL |  |
| SM_S1PEOPLE_D7_BellTower | 1476 | 10.13 | TALL |  |
| SM_S1PEOPLE_D7_PilotHouse | 1464 | 9.8 | TALL |  |
| SM_S1PEOPLE_D8_Rotunda | 1712 | 9.9 | TALL |  |
| SM_S1PEOPLE_D8_Terrace | 1488 | 6.46 | TALL |  |
| SM_S1PEOPLE_D8_Turret | 1484 | 11.1 | TALL |  |
| SM_S1PEOPLE_D8_ArchBridge | 1498 | 10.1 | TALL |  |

What each piece is and what moves on it: the header of each block in `FTPeopleL6.inl` and each generator's docstrings.
The s1people builder's hand-over text did not reach the integrator; its numbers above are from its make log and its
block's header (67 Sites, 224 parts tried; 35 slot sides left alone as having no clear place; 15 sites "may touch").

## Not checked

- No frame of the new world was judged: the two frames taken are at 134 s and 131 s of 3 to 5 frames a second runs,
  not the script's 6 s frame; the second was opened once (road, a barrier wall, the HUD saying FINALE) and nothing was judged from it. Whether any piece reads as a pickup, an enemy, the
  boss or an attack, colours against the sand and fog, light inside 24 m, movement: not checked.
- Frame time with Stage 1: not checked (five to six other Unreal editors were running on the PC during the runs here).
- Placed count of the 163 meshes FTCLEAR does not name; which rules the track left out per mesh.
- The builders' distances and slots against each other and against pass 3 (only that it compiles and places).
- The cause of the engine ensure at very low frame rates (it is there without Stage 1).
- A cooked or packaged build.

## Review and fix round (16 Nov 2026)

One round after Stage 1's review (a rules reader who replayed the tables with the game's own random numbers, and a
driver with 6 game runs). Only what was run and seen. Scratch of this round: `C:\ftwork\jobs\L6s1fix`, `C:\ftwork\out\L6s1fix`.

### Found

- Rule broken: none, by the reader's numbers (0 of 3203 placements inside 17 m; no piece over 2 m and no glow inside 24 m)
  and none seen in the driver's frames.
- Looks wrong: near pieces (17 to 40 m) beside banked bends in s1d1, s1d2, s1d3 and all of s1d8's walks; three heroes
  (Crown, the second Great Lamp, a Great Pearl) on older far pieces; 172 box overlaps of half a footprint or more with
  older solid pieces; five plants on the watchers' wall; the finale ends at 13.13 to 13.17 km in every run, so the Bubble
  Tree at 13.12 km and everything after it was never on screen; right-hand heroes come into view under the HUD's panel.
- Weak: 36 pieces the track pushed out; the Reel, Big Ear, Standing Stern, second Conch and second Pearl not picked out;
  district 6 not told from district 5; the tube hides the left side of Nereida; district 4 dark; shapes to judge (a buoy
  lying like a torpedo, a propeller blade standing like a fin, sea angels, seals, copper green drums).

### Fixed (table `FTPeopleL6.inl`; every changed line says "fix round")

How the table was changed: no rule was deleted and none inserted before another, so every rule keeps its running index
and its random numbers; rules that had to be ADDED stand in a new block `s1fix` at the very end of the file. Checked by
dumping both versions of the table: 2831 rules before, 2896 after; the first 2831 differ in exactly the 53 rules meant.

- s1d1: the verge (20.3 m) and the lane (25.9 m) end at slot 21 and start again at slot 30 (`s1fix`): nothing of them in
  the banked S-bend of slots 22 to 29.
- s1d2: the 13 props of the four farm Sets stand 40 m out (were 37.5 to 39 m), also beside the S-bend of slots 45 to 52.
- s1d3: the near rules after the steep bend end at slot 109 (slot 110 is where the next banked bend begins). The left Big
  Ear moved from slot 94 (the exit of the steepest S-bend; no frame showed it) to slot 100 (4.52 km, a straight). The
  greenhouse of two Sets moved 6 m along, off the kit's marker post.
- s1d5: the Great Pearl stands LEFT now, slots 186 (8.42 km) and 242 (10.95 km), out of the HUD panel's corner and with
  no older station hub, dome or gallery in its footprint by the replay. PearlLamp from 24.6 m and BannerMast (left) from
  25.3 m: the track no longer moves them (FTCLEAR does not name them any more) and the lamp's glass is 24 m out. The
  right seahorse statues moved from 31.3 m (the lane of pass 3's wagons and sled trains) to 28.5 m in slots s % 12 == 4.
- s1d6: the first Bubble Tree moved from slot 290 (13.12 km, where the finale ends) to slot 274 (12.40 km). Three Sets'
  float rack / bottle tree moved out of the kit's watch tower.
- s1d7: the second Great Lamp moved from slot 432 to slot 428 (19.36 km). In five Sets parts moved ALONG, off the kit's
  marker posts and out of its habitat dome. The buoy lying on its side 25.4 m out (could be taken for a torpedo) is a
  spyglass in that Set; the buoy remains only in the buoy yard, 42 m out.
- s1d8: all 14 rules of the walks by the road are written for the five straights only (slots 462-467 in the block,
  483-488, 504-509, 524-530, 545-550 in `s1fix`), as s1d7 does. The Crown stands at slots 474 and 528 (was 470 and 530).
  BubbleFountain from 26.5 m, LampTree and SeahorseTopiary from 25 m. The right walk of the last two straights uses the
  s % 8 == 6 slot (526, 550): pass 3's patrol and welders have 24 to 29 m in the other.
- s1plants: five pieces on the watchers' wall (26 to 27.5 m, left) are never placed (chance -1; the lines stay).
- s1people: ten groups that touched a pressure habitat stand at 45 m (were 42.5 m).
- Generators, three meshes regenerated (`C:\ftwork\logs\L6s1fix_make.log`, built 3, failed 0, 0 `LogPython: Error`):
  `SM_S1D2_TheReel` (rim and spokes yellow and straw, about twice as thick; 2588 triangles, top 65.7 m, as before);
  `SM_S1D4_StandingStern` (four broad bands, cream and teal, round the hull; 2384 triangles, was 2336; top 68.8 m);
  `SM_S1D4_PropMonument` (no blade straight up: top 4.77 m, was 5.3 m; 516 triangles).

By the replay after the fix (the reader's own scripts, re-run on the changed table): 0 centres and 0 nearest points
inside 17 m; 0 pieces over 2 m with a centre inside 24 m; 0 tall pieces whose nearest point is inside 24 m (was 43);
0 glowing parts inside 24 m (was the PearlLamp's glass at 23.66 m); box overlaps of half a footprint with older solid
pieces 172 -> 126 (s1d7 41 -> 16, s1d8 22 -> 11 with `s1fix`, s1d3 17 -> 10, s1d6 7 -> 5, s1people 8 -> 4, s1d2 10 -> 13).

### The game after the fix (one start, `C:\ftwork\out\s1\fixed_L6\run.log`, script `C:\ftwork\jobs\p3\start_L6.txt`)

- Compile: `build exit 0 after 75 s`, `Result: Succeeded`, 0 lines with "error" (`C:\ftwork\logs\L6s1_fix_build.log`).
  Compiled once more after the game start (`build exit 0 after 76 s`, 0 lines with "error"; the same log file, written
  over), because two comments of the table were corrected after it; the dumped rules of the table are the same before
  and after those two edits. The game was not started again.
- `FTTRACK level=6 frames=12470 route=04d3f899 solid=8927e339 solidTriangles=930148`: equal, compared as strings, to
  the `^FTTRACK level=6 ` line of `SourceArt/Jam/validate/track_baseline.txt`. (The tool prints the level 0 baseline,
  because the menu's level 0 line comes first; level 6 was compared separately.)
- `FTDRESS level=6 districts=759 sets=211 pieces placed=16025 of 16046 tried shadows=12629 of 16880`
  (before the fix: districts 751, placed 16138 of 16159, shadows 12688 of 16993).
- FTCLEAR: props kept 19368 (was 19481), moved off their own barrier 1757 (was 1794), left out of the road 30 (30).
  Of the new meshes it no longer names PearlLamp, BannerMast, LampTree, SeahorseTopiary, Urn; BubbleFountain ~1 of 5.
- "Fatal error" 0, "LogFT: Error" 0, "Ensure condition failed" 0, the clock line 6.0 found.
- The one frame (`L6_t006.png`, 6 s into the drive): road clear, the reef both sides. None of the changed pieces is in it.

### Not fixed, and why

- Districts s1d7 and s1d8 and most of s1d6 (past 13.17 km) are never on screen in the game as it plays now (the level
  ends there in every one of the driver's finale runs): a decision about the level, not a table fix. Only the first
  Bubble Tree was brought forward.
- s1d4's 46 box overlaps with the kit's containers, anchors, crates and wreck bow: the boxes are axis-aligned boxes of
  turned pieces, the free ground of each slot is not known without seeing it, and the driver's frames of district 4
  reported no clash. Left as built.
- The remaining overlaps of other districts (bell towers in pass 3's mast crews and rest buoy: 4; single buildings in
  containers; slim masts and lamps at the edge of pass 3's groups): each would need a rule of its own.
  PearlLamp (right, s % 4 == 0) still shares 24 to 29 m with the watchers' stands in slots s % 12 == 0.
  s1people's group at slot 161 left stays at 42.5 m: at 45 m it stood in the pig keeper of s1animals.
- District 6 not told from district 5; the tube hiding the left side; district 4 dark; the second Great Conch; the
  thin far right of the reef; clutter at the second jump's landing: would need new pieces, a second round of invention.
- Plants growing through each other (151 same-block overlaps of a quarter), counts over the brief's guides (s1d6 261,
  seven meshes over the triangle guide), three pieces 0.5 to 1 m before a district's edge: left.
- Sea angels (57 to 72 m out, 30 to 39 m up), seals (76 m and more), the copper green furnace drum (26.7 m) and bottle
  tree (26.2 m): candidates only; they stand past 13.17 km and no frame shows them. Left as built.
- The driver's "canisters and pentagon slabs" at 12.56 km left are the kit's own `SM_Anemones` (tube anemones and two
  sea fans by its docstring; the replay has one at 23.3 m in slot 277 left), not Stage 1: looked at in `d5L_t135.png`.
  The dark box with an amber face is the road's own lamp by the driver's notes. Neither was changed.
- The driver's contact sheet (`C:\ftwork\out\L6s1rev\L6_s1_review.jpg`) is 1,097,156 bytes, over 1 MB: not copied.

### Not checked

- No frame of any FIXED piece: the one start gives the 6 s frame only. The Reel's and the Stern's new colours, the moved
  heroes (Pearl left, Bubble Tree at 12.4 km, Big Ear at 4.52 km), the props at 40 m in the kelp forest: not seen.
- Which side is the inside of each banked bend; the banked slots are the plan's table, not read from the game.
- The free places of the moved heroes against older meshes whose bounds the replay does not have (SM_Dome, SM_City,
  SM_SharkRing, SM_WatchFarGallery, SM_ShaftSway: taken as boxes of 15 m either way) and against swimmers' paths (the
  box of a far scooter wing's path reaches the Great Lamp's new place; a fish herd is 6 m along from it at 214 m).
- All overlaps are box arithmetic in road coordinates (curvature and the land's height ignored).
- Frame time; a cooked build; whether a shape reads as a pickup, enemy, boss or attack.
