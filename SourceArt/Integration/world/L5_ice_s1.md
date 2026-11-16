# Level 5, Ice Mountain: Stage 1 of the final pass (the world), integration note

16 Nov 2026. Branch `claude/world-L5`. Written by the integrator from what was run and seen; what was not is under "Not checked".

## What a player now sees

Eight districts down the mountain, each with a lead colour, its own buildings and props and one landmark, plus plants, animals and raised groups of people in the stretches pass 3 left empty. The plan this follows: `C:\ftwork\final\s1\L5_plan.md`.

| id | district | km | look |
|---|---|---|---|
| s1d1 | First tracks | 0.0 to 2.43 | the summit station: teal pods on navy stilts; hero THE SUMMIT NEEDLE |
| s1d2 | Black diamond | 2.43 to 5.65 | the avalanche works: charcoal and yellow bunkers; hero THE BLACK DIAMOND |
| s1d3 | The valley run | 5.65 to 8.94 | the painted valley town: pink shingle and timber townhouses; hero THE GREAT COWBELL |
| s1d4 | The Abominable's glacier | 8.94 to 11.6 | the expedition camp: yellow domes, vault huts, tents; hero THE ICEBOUND SHIP |
| s1d5 | The white ridge | 11.6 to 14.3 | the aurora watch: violet wind-leaning refuges and masts; hero THE WIND HARP |
| s1d6 | The resort | 14.3 to 17.0 | the grand spa: cream hotels under blue mansards; hero THE PALACE OF BATHS |
| s1d7 | The ice again | 17.0 to 19.7 | the crystal mine: dark timber frames, lit teal crystal; hero THE CRYSTAL HEADFRAME |
| s1d8 | The high ridge | 19.7 to the end (Zone to 99) | the beacons: red and cream banded towers; hero THE GREAT BEACON |
| s1plants | all districts | one Zone a district | 20 plant pieces on slots s % 4 == 3 |
| s1animals | all districts | | 18 animal pieces: on the ground, at their own melt pools, pale birds far out (Far) |
| s1people | all districts | | 26 raised groups on slots s % 8 == 1, 29 m out (level road) or 42.6 m (banked bend, half pipe) |

## Counts (seen in the logs)

- Before (`C:\ftwork\out\p3\final_L5\run.log`): `FTDRESS level=5 districts=708 sets=168 pieces placed=11796 of 11824 tried shadows=3745 of 11871`
- After (`C:\ftwork\out\s1\after_L5\run.log`): `FTDRESS level=5 districts=1146 sets=513 pieces placed=15034 of 15062 tried shadows=5898 of 15109`
- So 3,238 more pieces placed and 2,153 more shadow casters. Tried minus placed is 28 before and after.
- Track: `FTTRACK level=5 frames=12415 route=b2fc812a solid=9cada242 solidTriangles=931996`, equal (compared as text) to the `^FTTRACK level=5 ` line of `SourceArt/Jam/validate/track_baseline.txt`.
- `FTCLEAR level=5 props kept=17841 (moved off their own barrier 808) left out of the road=28` (before: kept=14603, moved 808, left out 28). That line names only meshes that were moved or left out (FTTrack.cpp, the loop before the log call). NO Stage 1 mesh is named in it: none of the 210 was left out of the road or moved off the barrier. For the same reason the log gives NO placed count per new mesh: per-mesh placed counts are not checked.
- "Fatal error": 0. "LogFT: Error": 0. `/Game/World` load failures: none found (the only `/Game/World` lines are 8 of "Waiting for static meshes to be ready"). The two "Error:" lines in the log are the editor's own listener failing to bind port 8000.
- New meshes: 210 `.uasset` under `Content/World/IceMountain`, 212,206 triangles in all (summed from the make logs). Every builder's make log ends "pieces built N failed 0" and has 0 lines of "LogPython: Error".
- Compile: `build exit 0`, "Result: Succeeded" (on the second try, see below).

## What the integrator changed besides pasting

- All 11 blocks (`C:\ftwork\s1\L5_*.inl`) are at the end of `Source/FullThrottle/FTPeopleL5.inl`, in the order s1d1 to s1d8, s1plants, s1animals, s1people. No block was left out; no line inside a block was changed. Every mesh name used in a block is a file on disk (checked); no Set name is used twice or exists in the older tables (checked).
- The first compile failed: `FTTrackDress.cpp(166) : error C4883: FTableWriter::People5: function size suppresses optimizations` (the table had grown from 2,998 to 6,281 lines). The smallest change: each block is wrapped in `[&]() { ... }();` instead of `{ ... }`, so each block is a function of its own. The calls, their order and the running rule index are the same. `FTTrackDress.cpp` is unchanged.

## Left out, and known faults carried in

- Left out: nothing.
- `SM_S1D3_GreatCowbell` is the faulty mesh AS BUILT (builder: logged 22.3 x 28.3 m where about 16.4 x 16.4 m was meant; on three of its four faces the gallery rail and pink panel are 14.2 m slabs 7 m off centre, the shutters hide the lit windows, the arch lintels are 7 m slabs). The generator on disk has the three-line fix but was NOT re-run: it needs one editor run of `K.run(only=['SM_S1D3_GreatCowbell'])`. The as-built source is `C:\ftwork\jobs\L5s1d3\gen_as_built.py`.
- s1d6: roofs that are not square in plan came out as lozenges (PalaceHotel logged 17.0 x 15.7 m, meant 17.3 x 13.9; also Kurhaus, Funicular, PumpRoom and ColonnadeWalk's end caps; about 0.6 m at the corners on SpaHotel and TurretVilla). The builder rewrote `frustum()` in the generator AFTER the run and did not run it: the generator on disk is NOT what built the meshes (as-run copy: `C:\ftwork\jobs\L5s1d6\ft_world_ice_s1d6_asrun.py`).
- s1d3: the props stand in front of their houses (centres 44 to 56 m out), not in the brief's 17 to 40 m bands (block header).
- s1d5 has 23 meshes for 14 designs (left-hand twins, so everything leans down one wind on both sides); s1d2 has two left-hand twins.
- Over 2,700 triangles (from the logs; the heroes' guide is 4,000): SM_S1D1_SummitNeedle 3460, SM_S1D3_GreatCowbell 3360, SM_S1D4_IceboundShip 3944, SM_S1D6_PalaceHotel 2768, SM_S1D6_PalaceOfBaths 4232, SM_S1D7_Headframe 3300, SM_S1D8_GreatBeacon 3992. `SM_S1D6_PalaceOfBaths` (4232) is over the hero guide as well.

## Every new mesh (triangles, size m, top m, TALL/low: from the make logs `C:\ftwork\logs\L5<id>_make.log`)

### s1d1: 17 meshes, 21462 triangles. Builder's estimate (written x 0.75): 241 written, about 181 placed

| mesh | triangles | size m | top m | class |
|---|---|---|---|---|
| SM_S1D1_PodHostel | 2614 | 14.2 x 5.2 | 14.65 | TALL |
| SM_S1D1_DrumCafe | 2184 | 13.3 x 12.4 | 13.06 | TALL |
| SM_S1D1_TimingCabin | 1006 | 14.3 x 4.5 | 9.79 | TALL |
| SM_S1D1_SkiSchool | 2144 | 14.1 x 7.0 | 6.86 | TALL |
| SM_S1D1_LiftHall | 1758 | 15.8 x 9.3 | 8.65 | TALL |
| SM_S1D1_StartHouse | 1970 | 8.0 x 9.7 | 13.59 | TALL |
| SM_S1D1_WeatherPod | 1210 | 5.7 x 6.5 | 9.59 | TALL |
| SM_S1D1_BoardShop | 1228 | 9.5 x 6.1 | 9.41 | TALL |
| SM_S1D1_SkiRack | 416 | 5.4 x 1.0 | 1.19 | low |
| SM_S1D1_LockerPods | 768 | 5.2 x 0.9 | 1.14 | low |
| SM_S1D1_TicketGate | 444 | 4.6 x 1.3 | 1.08 | low |
| SM_S1D1_SnowStake | 364 | 2.7 x 2.7 | 1.3 | low |
| SM_S1D1_Podium | 548 | 7.1 x 2.0 | 5.6 | TALL |
| SM_S1D1_LampMast | 560 | 2.0 x 1.8 | 7.07 | TALL |
| SM_S1D1_Windsock | 348 | 3.0 x 0.8 | 8.29 | TALL |
| SM_S1D1_FlagLine | 440 | 12.1 x 0.5 | 6.45 | TALL |
| SM_S1D1_SummitNeedle | 3460 | 22.6 x 22.6 | 56.0 | TALL |

### s1d2: 19 meshes, 12224 triangles. Builder's estimate (written x 0.75): 209 written, about 157 placed

| mesh | triangles | size m | top m | class |
|---|---|---|---|---|
| SM_S1D2_SplitterBunker | 1000 | 23.6 x 10.8 | 10.6 | TALL |
| SM_S1D2_SplitterBunker_L | 1000 | 23.6 x 10.8 | 10.6 | TALL |
| SM_S1D2_BlastShelter | 676 | 13.6 x 10.7 | 6.83 | TALL |
| SM_S1D2_StudyTower | 724 | 9.9 x 8.1 | 23.3 | TALL |
| SM_S1D2_PatrolHut | 636 | 11.1 x 8.3 | 9.1 | TALL |
| SM_S1D2_PatrolHut_L | 636 | 11.1 x 8.3 | 9.1 | TALL |
| SM_S1D2_GalleryPortal | 884 | 20.0 x 7.9 | 9.56 | TALL |
| SM_S1D2_GearStore | 672 | 12.9 x 8.7 | 9.7 | TALL |
| SM_S1D2_WinchHouse | 576 | 15.1 x 6.2 | 7.72 | TALL |
| SM_S1D2_BlackDiamond | 1372 | 44.0 x 44.0 | 49.62 | TALL |
| SM_S1D2_DiamondPlate | 132 | 2.7 x 1.3 | 5.47 | TALL |
| SM_S1D2_BollardRow | 680 | 7.8 x 0.8 | 1.38 | low |
| SM_S1D2_Tetrapods | 680 | 4.7 x 4.7 | 1.84 | low |
| SM_S1D2_NetAnchor | 324 | 7.8 x 1.3 | 1.71 | low |
| SM_S1D2_SirenMast | 648 | 3.2 x 2.6 | 11.08 | TALL |
| SM_S1D2_DepthGantry | 444 | 9.7 x 1.5 | 8.82 | TALL |
| SM_S1D2_ChevronBarrier | 300 | 8.2 x 1.4 | 1.4 | low |
| SM_S1D2_CableSpool | 372 | 4.2 x 2.3 | 1.92 | low |
| SM_S1D2_VentCluster | 468 | 3.8 x 2.5 | 4.2 | TALL |

### s1d3: 18 meshes, 19428 triangles. Builder's estimate (written x 0.75): see the block header

| mesh | triangles | size m | top m | class |
|---|---|---|---|---|
| SM_S1D3_TallHouse | 1476 | 8.4 x 8.8 | 21.69 | TALL |
| SM_S1D3_ArcadeHouse | 1296 | 11.2 x 8.9 | 16.3 | TALL |
| SM_S1D3_Bakery | 1344 | 9.5 x 9.8 | 18.88 | TALL |
| SM_S1D3_ToyMaker | 1828 | 7.2 x 9.0 | 19.84 | TALL |
| SM_S1D3_TownGate | 1684 | 9.5 x 9.5 | 22.2 | TALL |
| SM_S1D3_Granary | 900 | 9.2 x 8.2 | 10.08 | TALL |
| SM_S1D3_OrielHouse | 1680 | 10.6 x 9.9 | 18.1 | TALL |
| SM_S1D3_GalleryInn | 1644 | 12.1 x 9.7 | 15.9 | TALL |
| SM_S1D3_FountainTrough | 288 | 3.4 x 2.4 | 1.92 | low |
| SM_S1D3_SledRack | 312 | 4.2 x 1.6 | 1.84 | low |
| SM_S1D3_RibbonPole | 728 | 5.8 x 5.8 | 9.44 | TALL |
| SM_S1D3_MarketStall | 696 | 3.6 x 2.6 | 3.02 | TALL |
| SM_S1D3_Woodstack | 824 | 5.0 x 1.5 | 1.84 | low |
| SM_S1D3_LanternPost | 276 | 0.7 x 1.8 | 4.38 | TALL |
| SM_S1D3_WashLine | 268 | 6.9 x 1.4 | 3.08 | TALL |
| SM_S1D3_FingerPost | 264 | 1.7 x 0.8 | 4.05 | TALL |
| SM_S1D3_BenchPlanter | 560 | 4.3 x 1.1 | 1.27 | low |
| SM_S1D3_GreatCowbell | 3360 | 22.3 x 28.3 | 53.7 | TALL |

### s1d4: 17 meshes, 15792 triangles. Builder's estimate (written x 0.75): 232 written, about 174 placed

| mesh | triangles | size m | top m | class |
|---|---|---|---|---|
| SM_S1D4_BigDome | 1472 | 14.4 x 17.4 | 10.16 | TALL |
| SM_S1D4_StoreDome | 1200 | 9.2 x 9.5 | 6.5 | TALL |
| SM_S1D4_VaultHut | 1048 | 11.1 x 5.8 | 4.05 | TALL |
| SM_S1D4_RadioHut | 1376 | 9.8 x 6.7 | 19.3 | TALL |
| SM_S1D4_MessTent | 1144 | 13.8 x 9.3 | 6.02 | TALL |
| SM_S1D4_SledgeGarage | 1204 | 12.3 x 6.6 | 6.0 | TALL |
| SM_S1D4_BalloonShed | 1044 | 8.4 x 11.9 | 12.9 | TALL |
| SM_S1D4_BunkTents | 660 | 12.0 x 3.8 | 3.4 | TALL |
| SM_S1D4_RouteFlags | 224 | 5.4 x 0.5 | 1.1 | low |
| SM_S1D4_CrateStack | 252 | 3.5 x 1.1 | 1.88 | low |
| SM_S1D4_LoadedSledge | 276 | 5.2 x 1.0 | 1.07 | low |
| SM_S1D4_FuelDump | 516 | 4.0 x 0.8 | 1.38 | low |
| SM_S1D4_DepotCairn | 444 | 2.6 x 1.7 | 4.3 | TALL |
| SM_S1D4_Theodolite | 412 | 3.5 x 2.0 | 2.68 | TALL |
| SM_S1D4_SignalMast | 304 | 6.1 x 0.8 | 8.84 | TALL |
| SM_S1D4_DryingLine | 272 | 4.7 x 0.7 | 1.91 | low |
| SM_S1D4_IceboundShip | 3944 | 69.7 x 35.3 | 34.37 | TALL |

### s1d5: 23 meshes, 16216 triangles. Builder's estimate (written x 0.75): 308 written, about 231 placed

| mesh | triangles | size m | top m | class |
|---|---|---|---|---|
| SM_S1D5_Refuge | 1052 | 11.7 x 7.4 | 6.67 | TALL |
| SM_S1D5_RefugeL | 1052 | 11.7 x 7.4 | 6.67 | TALL |
| SM_S1D5_SlitDome | 804 | 8.7 x 8.2 | 7.83 | TALL |
| SM_S1D5_SlitDomeL | 804 | 8.7 x 8.2 | 7.83 | TALL |
| SM_S1D5_LegCabin | 804 | 10.6 x 7.3 | 10.68 | TALL |
| SM_S1D5_LegCabinL | 804 | 10.6 x 7.3 | 10.68 | TALL |
| SM_S1D5_KiteLoft | 780 | 8.1 x 6.8 | 12.6 | TALL |
| SM_S1D5_KiteLoftL | 780 | 8.1 x 6.8 | 12.6 | TALL |
| SM_S1D5_WindWall | 708 | 11.1 x 5.9 | 4.54 | TALL |
| SM_S1D5_WindWallL | 708 | 11.1 x 5.9 | 4.54 | TALL |
| SM_S1D5_InstrumentHouse | 832 | 5.3 x 5.8 | 9.23 | TALL |
| SM_S1D5_InstrumentHouseL | 832 | 5.3 x 5.8 | 9.23 | TALL |
| SM_S1D5_VaneRow | 264 | 6.4 x 0.7 | 1.9 | low |
| SM_S1D5_VaneRowL | 264 | 6.4 x 0.7 | 1.9 | low |
| SM_S1D5_StreamerLine | 432 | 11.4 x 1.1 | 5.74 | TALL |
| SM_S1D5_StreamerLineL | 432 | 11.4 x 1.1 | 5.74 | TALL |
| SM_S1D5_Anemometer | 444 | 6.9 x 4.0 | 7.17 | TALL |
| SM_S1D5_AnemometerL | 444 | 6.9 x 4.0 | 7.17 | TALL |
| SM_S1D5_Telescope | 600 | 4.0 x 4.8 | 4.37 | TALL |
| SM_S1D5_LampCairn | 228 | 2.5 x 1.5 | 1.93 | low |
| SM_S1D5_WindBreak | 396 | 5.3 x 3.0 | 1.84 | low |
| SM_S1D5_SkyBoard | 432 | 3.5 x 1.3 | 3.62 | TALL |
| SM_S1D5_WindHarp | 2320 | 97.4 x 6.2 | 54.3 | TALL |

### s1d6: 17 meshes, 18820 triangles. Builder's estimate (written x 0.75): see the block header

| mesh | triangles | size m | top m | class |
|---|---|---|---|---|
| SM_S1D6_PalaceHotel | 2768 | 17.0 x 15.7 | 28.0 | TALL |
| SM_S1D6_SpaHotel | 1460 | 10.3 x 10.2 | 22.49 | TALL |
| SM_S1D6_TurretVilla | 1336 | 11.4 x 10.1 | 22.42 | TALL |
| SM_S1D6_PumpRoom | 1228 | 10.2 x 10.2 | 11.42 | TALL |
| SM_S1D6_BandstandHall | 1336 | 9.2 x 9.6 | 12.62 | TALL |
| SM_S1D6_ColonnadeWalk | 1076 | 15.3 x 5.3 | 9.32 | TALL |
| SM_S1D6_Funicular | 1052 | 11.8 x 14.0 | 12.1 | TALL |
| SM_S1D6_Kurhaus | 1340 | 15.7 x 16.2 | 21.4 | TALL |
| SM_S1D6_BenchPair | 312 | 4.5 x 0.8 | 1.14 | low |
| SM_S1D6_LuggageCart | 360 | 2.6 x 1.1 | 1.22 | low |
| SM_S1D6_FlowerRail | 216 | 5.3 x 0.5 | 1.19 | low |
| SM_S1D6_Fountain | 588 | 4.1 x 4.1 | 3.24 | TALL |
| SM_S1D6_ChangingCabins | 308 | 4.2 x 2.3 | 3.43 | TALL |
| SM_S1D6_Kiosk | 448 | 3.7 x 3.7 | 5.1 | TALL |
| SM_S1D6_GlobeLamp | 488 | 1.5 x 1.5 | 6.72 | TALL |
| SM_S1D6_TowelLine | 272 | 5.7 x 1.0 | 2.89 | TALL |
| SM_S1D6_PalaceOfBaths | 4232 | 66.4 x 45.5 | 43.6 | TALL |

### s1d7: 18 meshes, 14828 triangles. Builder's estimate (written x 0.75): 256 written, about 192 placed

| mesh | triangles | size m | top m | class |
|---|---|---|---|---|
| SM_S1D7_Bunkhouse | 1108 | 12.0 x 9.3 | 13.04 | TALL |
| SM_S1D7_ChuteHouse | 1004 | 13.1 x 8.0 | 18.28 | TALL |
| SM_S1D7_WindingHouse | 908 | 10.5 x 8.6 | 12.66 | TALL |
| SM_S1D7_AssayOffice | 884 | 7.7 x 7.4 | 9.45 | TALL |
| SM_S1D7_Trestle | 1072 | 23.6 x 5.9 | 10.1 | TALL |
| SM_S1D7_ToolStore | 696 | 7.2 x 6.7 | 5.56 | TALL |
| SM_S1D7_Breaker | 1264 | 22.4 x 7.9 | 17.44 | TALL |
| SM_S1D7_TankTower | 888 | 5.6 x 5.9 | 18.07 | TALL |
| SM_S1D7_Headframe | 3300 | 56.4 x 14.2 | 50.75 | TALL |
| SM_S1D7_OreCart | 484 | 3.2 x 1.2 | 1.76 | low |
| SM_S1D7_PitProps | 400 | 4.7 x 1.4 | 1.51 | low |
| SM_S1D7_Winch | 292 | 3.1 x 1.4 | 1.91 | low |
| SM_S1D7_SortTable | 576 | 3.2 x 1.0 | 1.92 | low |
| SM_S1D7_BufferStop | 344 | 3.2 x 1.3 | 1.9 | low |
| SM_S1D7_CrystalRock | 496 | 3.3 x 2.3 | 4.4 | TALL |
| SM_S1D7_LampPost | 228 | 2.1 x 1.3 | 5.31 | TALL |
| SM_S1D7_BellFrame | 216 | 2.9 x 1.0 | 4.82 | TALL |
| SM_S1D7_Sluice | 668 | 8.8 x 1.9 | 4.46 | TALL |

### s1d8: 17 meshes, 18148 triangles. Builder's estimate (written x 0.75): 301 written, about 226 placed

| mesh | triangles | size m | top m | class |
|---|---|---|---|---|
| SM_S1D8_GreatBeacon | 3992 | 29.4 x 29.0 | 60.72 | TALL |
| SM_S1D8_Beacon | 1324 | 11.4 x 8.4 | 21.01 | TALL |
| SM_S1D8_HighBeacon | 1500 | 11.9 x 10.6 | 29.63 | TALL |
| SM_S1D8_KeeperHouse | 1556 | 11.5 x 8.2 | 9.61 | TALL |
| SM_S1D8_SignalStation | 1452 | 8.4 x 8.3 | 13.96 | TALL |
| SM_S1D8_HornHouse | 1568 | 13.0 x 7.4 | 10.1 | TALL |
| SM_S1D8_LiftTop | 1636 | 12.1 x 9.4 | 8.26 | TALL |
| SM_S1D8_RidgeShelter | 1100 | 9.1 x 8.0 | 8.2 | TALL |
| SM_S1D8_SignalMast | 516 | 3.9 x 1.3 | 9.31 | TALL |
| SM_S1D8_MarkerCone | 292 | 3.2 x 2.2 | 4.88 | TALL |
| SM_S1D8_FogBell | 300 | 2.4 x 2.4 | 4.3 | TALL |
| SM_S1D8_FlagHoist | 408 | 7.0 x 1.1 | 7.9 | TALL |
| SM_S1D8_BeaconLamp | 512 | 1.3 x 1.3 | 5.11 | TALL |
| SM_S1D8_Bollards | 816 | 7.7 x 0.5 | 1.18 | low |
| SM_S1D8_LampCairn | 276 | 4.2 x 1.7 | 1.89 | low |
| SM_S1D8_FlagLocker | 348 | 5.3 x 1.0 | 1.62 | low |
| SM_S1D8_RopeWalk | 552 | 8.2 x 1.4 | 1.33 | low |

### s1plants: 20 meshes, 18260 triangles. Builder's estimate (written x 0.75): 580 written, about 435 placed

| mesh | triangles | size m | top m | class |
|---|---|---|---|---|
| SM_S1PLANTS_SnowFir | 1188 | 5.1 x 4.9 | 9.35 | TALL |
| SM_S1PLANTS_FirGrove | 1476 | 10.6 x 5.6 | 9.92 | TALL |
| SM_S1PLANTS_StormFir | 716 | 6.4 x 2.9 | 8.59 | TALL |
| SM_S1PLANTS_DwarfPine | 876 | 6.3 x 2.5 | 0.88 | low |
| SM_S1PLANTS_SnowGrass | 840 | 5.5 x 1.4 | 0.87 | low |
| SM_S1PLANTS_FrostBirch | 1024 | 5.3 x 5.4 | 7.0 | TALL |
| SM_S1PLANTS_BirchCopse | 1408 | 9.5 x 6.0 | 7.15 | TALL |
| SM_S1PLANTS_GoldLarch | 1028 | 5.3 x 5.3 | 10.81 | TALL |
| SM_S1PLANTS_Rowan | 1296 | 5.2 x 5.2 | 5.41 | TALL |
| SM_S1PLANTS_AlpineRose | 816 | 5.3 x 2.2 | 0.88 | low |
| SM_S1PLANTS_BlueSpruce | 684 | 4.4 x 4.4 | 7.88 | TALL |
| SM_S1PLANTS_JuniperCone | 548 | 1.9 x 1.9 | 4.66 | TALL |
| SM_S1PLANTS_JuniperHedge | 536 | 6.7 x 1.2 | 0.98 | low |
| SM_S1PLANTS_RoseBoxes | 720 | 5.6 x 1.0 | 1.0 | low |
| SM_S1PLANTS_Heather | 1028 | 5.5 x 2.2 | 0.74 | low |
| SM_S1PLANTS_Edelweiss | 812 | 3.5 x 2.6 | 0.91 | low |
| SM_S1PLANTS_LichenRock | 502 | 4.5 x 2.9 | 0.91 | low |
| SM_S1PLANTS_FrostFern | 1372 | 5.6 x 4.9 | 0.87 | low |
| SM_S1PLANTS_IceReed | 868 | 5.9 x 5.6 | 0.96 | low |
| SM_S1PLANTS_GlowMoss | 522 | 4.7 x 2.7 | 0.9 | low |

### s1animals: 18 meshes, 17198 triangles. Builder's estimate (written x 0.75): about 295 placed

| mesh | triangles | size m | top m | class |
|---|---|---|---|---|
| SM_S1ANIMALS_Hares | 1110 | 6.0 x 4.2 | 1.84 | low |
| SM_S1ANIMALS_Kennel | 944 | 7.2 x 4.6 | 3.32 | TALL |
| SM_S1ANIMALS_Ptarmigan | 996 | 5.2 x 3.8 | 1.44 | low |
| SM_S1ANIMALS_Goats | 1340 | 8.0 x 5.8 | 3.43 | TALL |
| SM_S1ANIMALS_SleighPink | 812 | 8.0 x 5.0 | 2.65 | TALL |
| SM_S1ANIMALS_SleighBlue | 812 | 8.0 x 5.0 | 2.65 | TALL |
| SM_S1ANIMALS_Marmots | 1036 | 5.0 x 4.0 | 1.44 | low |
| SM_S1ANIMALS_Penguins | 1240 | 8.2 x 5.0 | 1.02 | low |
| SM_S1ANIMALS_Seals | 844 | 8.1 x 6.5 | 0.94 | low |
| SM_S1ANIMALS_Foxes | 820 | 11.5 x 4.3 | 1.01 | low |
| SM_S1ANIMALS_Chamois | 768 | 10.2 x 6.7 | 7.5 | TALL |
| SM_S1ANIMALS_YakTrain | 1554 | 13.8 x 3.1 | 2.9 | TALL |
| SM_S1ANIMALS_Reindeer | 1402 | 9.1 x 5.6 | 3.0 | TALL |
| SM_S1ANIMALS_Swans | 1164 | 7.0 x 6.4 | 1.22 | low |
| SM_S1ANIMALS_SkySwans | 910 | 10.3 x 14.2 | 2.55 | TALL |
| SM_S1ANIMALS_SkyOwl | 130 | 2.5 x 3.5 | 1.67 | low |
| SM_S1ANIMALS_SkyGulls | 470 | 10.1 x 5.8 | 2.67 | TALL |
| SM_S1ANIMALS_SkyBuntings | 846 | 8.2 x 4.7 | 2.64 | TALL |

### s1people: 26 meshes, 39830 triangles. Builder's estimate (written x 0.75): see the block header

| mesh | triangles | size m | top m | class |
|---|---|---|---|---|
| SM_S1PEOPLE_D1_StartPod | 1396 | 4.1 x 2.9 | 5.9 | TALL |
| SM_S1PEOPLE_D1_Coaches | 1576 | 4.0 x 2.6 | 5.6 | TALL |
| SM_S1PEOPLE_D1_Cafe | 1648 | 4.1 x 3.2 | 4.6 | TALL |
| SM_S1PEOPLE_D2_Bunker | 1532 | 4.1 x 2.6 | 5.52 | TALL |
| SM_S1PEOPLE_D2_Gantry | 1520 | 4.2 x 2.3 | 6.06 | TALL |
| SM_S1PEOPLE_D2_Experts | 1386 | 4.2 x 2.6 | 5.54 | TALL |
| SM_S1PEOPLE_D3_Bandstand | 1760 | 4.0 x 2.8 | 5.78 | TALL |
| SM_S1PEOPLE_D3_Judges | 1586 | 4.6 x 2.0 | 5.26 | TALL |
| SM_S1PEOPLE_D3_Market | 1672 | 4.1 x 2.7 | 4.84 | TALL |
| SM_S1PEOPLE_D4_Haul | 1208 | 4.1 x 2.7 | 5.48 | TALL |
| SM_S1PEOPLE_D4_Mess | 1376 | 4.2 x 2.5 | 5.3 | TALL |
| SM_S1PEOPLE_D4_Photo | 1368 | 4.3 x 2.5 | 5.49 | TALL |
| SM_S1PEOPLE_D5_SkyWatch | 1380 | 4.0 x 2.7 | 4.44 | TALL |
| SM_S1PEOPLE_D5_Kites | 1428 | 4.0 x 2.5 | 5.6 | TALL |
| SM_S1PEOPLE_D5_Cairn | 1420 | 4.1 x 2.5 | 5.95 | TALL |
| SM_S1PEOPLE_D6_Quartet | 1428 | 4.0 x 2.6 | 5.3 | TALL |
| SM_S1PEOPLE_D6_Baths | 1512 | 4.1 x 2.5 | 5.12 | TALL |
| SM_S1PEOPLE_D6_Gallery | 1652 | 4.3 x 2.5 | 5.4 | TALL |
| SM_S1PEOPLE_D7_Staging | 1484 | 4.0 x 2.5 | 4.88 | TALL |
| SM_S1PEOPLE_D7_Cart | 1736 | 4.0 x 2.5 | 5.26 | TALL |
| SM_S1PEOPLE_D7_Trader | 1520 | 4.0 x 2.5 | 4.8 | TALL |
| SM_S1PEOPLE_D8_Gallery | 1508 | 3.4 x 2.7 | 5.3 | TALL |
| SM_S1PEOPLE_D8_Signal | 1376 | 4.2 x 2.5 | 6.1 | TALL |
| SM_S1PEOPLE_D8_Lift | 1456 | 4.1 x 2.5 | 5.52 | TALL |
| SM_S1PEOPLE_D8_Hoist | 1636 | 4.0 x 2.6 | 6.72 | TALL |
| SM_S1PEOPLE_PipeTower | 2266 | 3.5 x 2.6 | 12.3 | TALL |

Placed count per mesh: not printed by the game for these meshes (see Counts). What each piece is and what moves: the comments of `Content/Python/ft_world_ice_<id>.py` and each block's header in `FTPeopleL5.inl`.

## Not checked

- Nothing was reviewed in frames. One frame exists (`C:\ftwork\out\s1\after_L5\L5_t006.png`, 6 s into the level): the game runs and draws; no Stage 1 piece was judged in it.
- Placed count per new mesh; which of the 3,238 pieces belong to which builder.
- Frame time, memory, draw calls.
- Whether any piece hides the road, a pickup, an enemy or the boss, could be taken for one of them, or stands in or in front of an older piece (the builders' slot replays were not re-run by the integrator).
- The play rules of the brief (17 m / 24 m distances, glow distances, movement rates) were not re-measured; only the builders' own checks exist.
- The level was started once and driven for 6 s; the rest of the road was not driven.
- The faulty cowbell and the s1d6 roofs were not looked at in the game.

## Review and fix round

16 Nov 2026, after Stage 1's review (a rules reader from the numbers, a driver from 8 game runs). One round; no new pieces. Tools and outputs: `C:\ftwork\jobs\L5s1fix` (`plan.py` holds every decision, `fix.py` writes the table, `sim.py` / `check1.py` / `check3.py` are the rules reader's replay and checks, re-run on the result), `C:\ftwork\out\L5s1fix`.

### How the table was changed

- Every rule's random stream is seeded by its running index, so no rule was deleted or inserted. A rule is switched off in place with chance `-1.f` (never placed), or changed in place (distance, scale, set name, `every`/`phase` to keep only some of its slots). Whatever had to be added (a hero on another slot, a multi-slot Site whose slots now differ) stands in ONE new block at the very end of `FTPeopleL5.inl`, `s1fix`: 37 Sets named `WldS1...Fx<n>` (the set of that name without its part n) and 48 one-slot Sites. Every changed line carries a `// FIX ROUND:` comment with the finding. 136 rule lines changed.
- Box checks in this round use the real bounds of the 66 dressing meshes of `/Game/Maps/Glacier/Dress` (printed by the editor job below), not the guessed sizes the review had. With them the committed table had 793 box hits deeper than 0.3 m (the review counted 636 with its sizes); after the round 567, of which 247 are against things in the air (eagles, kites, lift cables) or the snow gusts, and 90 against the service road's banks.

### After (seen in the logs)

- Editor job `C:\ftwork\logs\L5s1fix_make.log`: exit 0; 6 meshes rebuilt, "failed 0" four times, 0 lines of "LogPython: Error". `SM_S1D3_GreatCowbell` now logs 16.4 x 16.4 m (was 22.3 x 28.3).
- Compile `C:\ftwork\logs\L5s1_fix_build.log`: `build exit 0`, "Result: Succeeded" (FTTrackDress.cpp recompiled).
- Level started once (`C:\ftwork\out\s1\fixed_L5\run.log`, one frame `L5_t006.png`): `FTTRACK level=5 frames=12415 route=b2fc812a solid=9cada242 solidTriangles=931996`, equal (compared as text) to the `^FTTRACK level=5 ` line of `SourceArt/Jam/validate/track_baseline.txt`.
- `FTDRESS level=5 districts=1194 sets=550 pieces placed=14852 of 14880 tried shadows=5758 of 14927` (before the round: 15034 of 15062, shadows 5898 of 15109). 182 fewer pieces, as the replay says (Stage 1 pieces written: 3,238 before, 3,056 now).
- `FTCLEAR level=5 props kept=17659 (moved off their own barrier 808) left out of the road=28`; no Stage 1 mesh is named in it. "Fatal error" 0, "LogFT: Error" 0, lines with "Error:" 0.
- Replay of the changed table (`check1.py`): 0 pieces nearer than 17 m; 0 pieces taller than 2 m nearer than 24 m (centre or box edge); no district mesh outside its district's km; no district mesh more than 40 times (most: LampMast 37); districts on even slots, plants on s % 4 == 3, people on s % 8 == 1 at 29 or 42.6 m, animals from 31.6 m. Written per builder, before -> now: s1d1 223 -> 212, s1d2 209 -> 199, s1d3 227 -> 223, s1d4 232 -> 223, s1d5 308 -> 296, s1d6 244 -> 241, s1d7 256 -> 250, s1d8 287 -> 281, s1plants 580 -> 491, s1animals 372 -> 362, s1people 300 -> 278.

### Found and fixed

Rule broken:
- R1. The ToolStore of slot 376 (16.998 km, 1.6 m before district 7 begins): taken out of that slot's set.

Looks wrong (rules reader):
- L1. THE BLACK DIAMOND stood on the piste groomers' strip of slot 103: now at slot 100 (4.524 km), left, 114 m. Left there: its box touches the herd of slot 99 by 0.8 m.
- L2. Inside the chute's rock cutting (real bounds: 35 to 47 m out): the three people decks of slot 65 right taken out; the splitter bunker's set of slot 64 right moved from 48 to 53.5 m; the Edelweiss of slot 63 both sides, a DiamondPlate (slot 62 right) and the Ptarmigan of slot 61 left taken out. Box hits 1 m or deeper with the cutting: 9 -> 0.
- L3. The five right-hand people Sites in front of the chalet rows (slots 321, 345, 369, 497, 521): taken out.
- L4. On the terrain park lanes and car parks: every piece whose box was 1 m or more inside one is taken out (48 hits -> 0), among them the HighBeacon of slot 522, the GalleryPortal set of slot 108, most of the patrol set of slot 114, the DrumCafe of slot 46 left, trees and ground cover on the park lanes. 6 hits under 1 m are left.
- L5. Stage 1 blocks in each other: animals written 46 to 47 m on the people's slots now stand 3.5 m further out (one, a sleigh at slot 361, taken out: it would have reached the terrain park); plants in the animals' 61 m line taken out; the four people decks that stood in a spa hotel, the colonnade and two fountains taken out, and one AlpineRose. Hits between different builders: 29 -> 2 (both under 1 m).
- L6. Stage 1 in older pieces: every part whose box was 2 m or more inside an older solid piece is taken out (80 hits -> 0), also the chamois and swans on the groomers' and the rack train's strips. That takes 4 of the 16 BigDomes (right, slots 204, 222, 234, 252: pass 3's heli deck, bridge and ice arch stand there), two ToyMakers, two ArcadeHouses, the film crew's slots of district 5, and others (each line says which). Hits against eagles, kites, lift cables and snow gusts were NOT counted as faults. 150 hits under 2 m are left. Seven plant rule-slots that had no fault were given up with them (one Scatter rule cannot keep some of its slots without the others; the lines say which).
- L7 and driver 2. The second ICEBOUND SHIP (slot 236) is taken out; the one of slot 224 is 1.5 times as large (hull about 105 m, top about 52 m, near edge about 108 m out). No box hit for it in the replay.

Driver:
- 3. THE GREAT COWBELL rebuilt from the corrected generator (the one editor run that was prepared).
- 4 (W6). The sky buntings' wings are pale grey (`SNOW_SH`) instead of timber brown; mesh rebuilt.
- 5. THE SUMMIT NEEDLE moved from slot 18 to slot 16 (0.724 km), right, 112 m.
- 6. The splitter bunkers' walls and the gallery portal's back wall are charcoal instead of concrete, the bunker's yellow band 50 cm instead of 24, the portal's yellow feet taller; 3 meshes rebuilt. Only these two designs.
- 8. THE WIND HARP: masts 2 m thick at the foot (were 1.2), streamers 2.2 to 3 m deep (were 0.8 to 1.2), top lamps 90 cm; mesh rebuilt; both Sites at scale 1.3.

Weak:
- W1. The 11 rules that can never fire (slot 0; slots 548 and 550) are marked in the table and the two headers' counts corrected (223, 287). They are left written: taking them out would re-roll every later rule.
- W8. The BalloonShed of the right-hand set stood 70.0 m out; now 69.5 m.

### Not fixed, and why

- Driver 1: districts 6, 7 and 8 are never reached in a normal run (the boss's 80 s clock ends the level at about 13.8 km). Not a table fault; whether to move their heroes into 8.94 to 13.8 km is the owner's decision. Nothing was moved.
- Driver 7 (the yellow camp hidden behind ice walls): tried by box test at 48, 51, 54, 57, 60 m. On the right 13 of 16 camp Sites hit pass 3's decks, bridges, crags or film crew at one or more of those distances; the left could come 7 m nearer on 12 Sites, but which wall hides what was not found in the tables. Left as it is.
- Driver 6, the rest: the other seven building designs of district 2 are unchanged.
- Driver 9 and 10 (district 5 busy, the first 150 m read as the old village): no change asked.
- L8 (pieces nearer than 40 m on the low side of the finale's gentle bends): not changed; needs a frame, see Not checked.
- W2: s1animals 362 and s1d5 296 are still over the brief's guides (300, 250).
- W3 (low pieces beside half pipes), W4 (plants in plants: 39 box pairs left of 54), W5 (the spa's big buildings' boxes in the service road's banks: 90 hits): unchanged.
- W7 (GlowMoss, mint and glowing, 28 m out on the boss's road, 17.0 to 19.7 km): unchanged, not judged; that stretch is not reached in a run.
- W9 (older verge pieces the track pushes out): unchanged. A MODEL only: with the real bounds, and every tall dressing piece written nearer than 24 m pushed until its near edge is 24.75 m out (a reading of `Clear` in FTTrack.cpp, not measured), 265 Stage 1 pieces would have a box hit with a snow fence, a snow bank or a marker pole (district 5: 154 hits, district 8: 62, people 40). Their true places are not known. If a frame shows it, the near sets of districts 5 and 8 (24.8 to 28.8 m) are the ones to move.
- The lozenge roofs of district 6 (carried in above): the rewritten `frustum()` has never been run and district 6 cannot be reached to look at the result. Not run.
- The driver's contact sheet `C:\ftwork\out\L5s1rev\L5_s1_review.jpg` is 1,645,646 bytes, over 1 MB: not copied to `sheets/`.

### Not checked

- No Stage 1 piece was looked at in the game after the round. One frame exists (6 s into the level); it shows the game drawing, and several dark birds in the sky on the right. Which mesh they are (the older eagles or geese, or the buntings) is not checked.
- Whether the ship at 1.5, the harp at 1.3, the needle on slot 16, the diamond on slot 100, the cowbell and the charcoal bunkers now read from the road: not driven.
- Whether the moved or enlarged heroes hide anything: only the box replay (no hit), no frame.
- L8, W3, W7, W9 as said above. Frame time, memory, draw calls. Districts 6 to 8 in the game. A packaged build.
- The replay does not model the track's own pushing-out or leaving-out (`Clear`, `KeptOut`); the FTCLEAR line names no Stage 1 mesh.
