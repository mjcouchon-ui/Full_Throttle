# Under the Sea (level index 6): population pass 3

Six builders (p3a..p3f) each wrote a generator (`Content/Python/ft_people_sea_p3<letter>.py`), meshes
(`/Game/People/UnderTheSea/SM_P3<LETTER>_*`, 70 in all) and a table block. The integrator appended the six blocks, each
wrapped in `{ }`, to the end of `Source/FullThrottle/FTPeopleL6.inl` (nothing above them was changed), compiled once and
started the level once. Nobody looked at a frame.

## What was run and seen

- Text check of the six blocks against the make logs: all 70 meshes the blocks name exist as `.uasset`; the ten shared crew
  kit meshes p3f's Sets use (`/Game/People/Crew/SM_Crew<Piece>_Sea`) exist; no rule has an x min under 17 m, and no rule
  placing a TALL mesh has an x min under 24 m (the seven LOW meshes of p3a stand at 21.6 to 23.6 m; the nearest TALL rule
  is 26.2 m). No rule was changed.
- Compile (`build.ps1`, tag L6p3_build): exit 0, "Result: Succeeded", first try. No block was edited for the compiler.
- One game start (`start_L6.txt`, out `C:\ftwork\out\p3\after_L6`): exit 0, the level came up, one frame saved (not looked at).
  - `FTTRACK level=6 frames=12470 route=04d3f899 solid=8927e339 solidTriangles=930148`: identical to the level-6 line of
    `SourceArt/Jam/validate/track_baseline.txt`.
  - FTDRESS before (`C:\ftwork\logs\p3_prep_L6.txt`): `districts=181 sets=17 pieces placed=11072 of 11084 tried`
  - FTDRESS after: `districts=370 sets=27 pieces placed=13521 of 13538 tried shadows=10752 of 14376`
  - So pass 3 tried 2454 pieces and 2449 were placed (the builders' estimates add up to 2459).
  - All 70 SM_P3 meshes appear in the log's "Waiting on static mesh ... being ready" lines (the game loaded every one).
  - No "Fatal error", no "LogFT: Error", no failed load of a /Game/People asset in run.log.

## Left out or moved by the track (last FTCLEAR line, name:-left out/~moved off the barrier/placed)

FTCLEAR prints a mesh only when something of it was left out or moved; 42 of the 70 are listed. Left out, 5 pieces in all:
SM_P3E_FishHerd 1, SM_P3F_DroneTeam_A 1, SM_P3F_DroneTeam_B 1, SM_P3F_StuntRig 2. The 28 meshes not listed
(SM_P3A_Commuters_A/_B, KelpFarmers, RestBuoy; SM_P3C_ManateeHerd, CableCar, RidingRing, BuoyLaps; all ten SM_P3D; ten
of the eleven SM_P3E) had nothing left out and nothing moved; the log gives no placed count for them.

## What pass 3 added

| builder | focus | mesh | triangles | top m | log | FTCLEAR -out/~moved/placed |
|---|---|---|---|---|---|---|
| p3a | gaps | SM_P3A_VergeGang_A | 1800 | 1.82 | low | -0/~3/52 |
| p3a | gaps | SM_P3A_VergeGang_B | 1824 | 1.76 | low | -0/~4/53 |
| p3a | gaps | SM_P3A_WallSitters | 1860 | 1.81 | low | -0/~2/49 |
| p3a | gaps | SM_P3A_Gardeners | 1764 | 1.76 | low | -0/~1/34 |
| p3a | gaps | SM_P3A_Surveyors | 1524 | 1.75 | low | -0/~5/36 |
| p3a | gaps | SM_P3A_EelGarden | 1500 | 1.85 | low | -0/~6/38 |
| p3a | gaps | SM_P3A_LobsterMarch | 1476 | 1.82 | low | -0/~4/32 |
| p3a | gaps | SM_P3A_Commuters_A | 1144 | 10.83 | TALL | not listed (0 out, 0 moved) |
| p3a | gaps | SM_P3A_Commuters_B | 1168 | 11.05 | TALL | not listed (0 out, 0 moved) |
| p3a | gaps | SM_P3A_SchoolClass | 1412 | 8.97 | TALL | -0/~6/53 |
| p3a | gaps | SM_P3A_KelpFarmers | 1836 | 10.8 | TALL | not listed (0 out, 0 moved) |
| p3a | gaps | SM_P3A_MastCrew | 1632 | 11.08 | TALL | -0/~2/47 |
| p3a | gaps | SM_P3A_RestBuoy | 1972 | 9.6 | TALL | not listed (0 out, 0 moved) |
| p3a | gaps | SM_P3A_Catwalk | 1840 | 5.2 | TALL | -0/~3/50 |
| p3b | scenes | SM_P3B_Wedding | 2252 | 10.58 | TALL | -0/~2/34 |
| p3b | scenes | SM_P3B_PearlMarket | 1992 | 8.36 | TALL | -0/~5/33 |
| p3b | scenes | SM_P3B_TreasureHaul | 1672 | 11.87 | TALL | -0/~5/22 |
| p3b | scenes | SM_P3B_SeahorseRace | 2548 | 12.0 | TALL | -0/~3/23 |
| p3b | scenes | SM_P3B_KelpHarvest | 1948 | 8.26 | TALL | -0/~5/24 |
| p3b | scenes | SM_P3B_SchoolTrip | 1932 | 9.56 | TALL | -0/~4/28 |
| p3b | scenes | SM_P3B_StatueDig | 1924 | 9.85 | TALL | -0/~4/29 |
| p3b | scenes | SM_P3B_BubbleBand | 1964 | 8.15 | TALL | -0/~3/33 |
| p3b | scenes | SM_P3B_RescueDrill | 2104 | 7.6 | TALL | -0/~3/31 |
| p3b | scenes | SM_P3B_SubLaunch | 2012 | 9.0 | TALL | -0/~4/28 |
| p3c | movers | SM_P3C_Scooters_A | 1704 | 12.43 | TALL | -0/~24/45 |
| p3c | movers | SM_P3C_Scooters_B | 1704 | 12.47 | TALL | -0/~21/41 |
| p3c | movers | SM_P3C_FinRace | 1752 | 10.48 | TALL | -0/~10/32 |
| p3c | movers | SM_P3C_AquaBikes | 1684 | 8.9 | TALL | -0/~19/42 |
| p3c | movers | SM_P3C_SledTrain | 1680 | 5.45 | TALL | -0/~19/46 |
| p3c | movers | SM_P3C_TurtleWagon | 1232 | 6.58 | TALL | -0/~29/31 |
| p3c | movers | SM_P3C_Walkway | 1976 | 5.94 | TALL | -0/~1/33 |
| p3c | movers | SM_P3C_ManateeHerd | 1520 | 8.83 | TALL | not listed (0 out, 0 moved) |
| p3c | movers | SM_P3C_CableCar | 1752 | 15.56 | TALL | not listed (0 out, 0 moved) |
| p3c | movers | SM_P3C_RidingRing | 2000 | 10.0 | TALL | not listed (0 out, 0 moved) |
| p3c | movers | SM_P3C_BuoyLaps | 1820 | 16.7 | TALL | not listed (0 out, 0 moved) |
| p3d | heights | SM_P3D_Gantry_A | 1400 | 16.01 | TALL | not listed (0 out, 0 moved) |
| p3d | heights | SM_P3D_Gantry_B | 1392 | 15.0 | TALL | not listed (0 out, 0 moved) |
| p3d | heights | SM_P3D_Mast | 1240 | 14.8 | TALL | not listed (0 out, 0 moved) |
| p3d | heights | SM_P3D_Buoy | 1328 | 16.18 | TALL | not listed (0 out, 0 moved) |
| p3d | heights | SM_P3D_Spire | 1340 | 14.2 | TALL | not listed (0 out, 0 moved) |
| p3d | heights | SM_P3D_WreckMast | 1520 | 13.61 | TALL | not listed (0 out, 0 moved) |
| p3d | heights | SM_P3D_Crane | 1136 | 14.1 | TALL | not listed (0 out, 0 moved) |
| p3d | heights | SM_P3D_Terraces | 1548 | 14.6 | TALL | not listed (0 out, 0 moved) |
| p3d | heights | SM_P3D_RopeBridge | 1468 | 11.0 | TALL | not listed (0 out, 0 moved) |
| p3d | heights | SM_P3D_Lift | 1396 | 13.5 | TALL | not listed (0 out, 0 moved) |
| p3e | far and sky | SM_P3E_DiveLine_A | 1250 | 68.8 | TALL | not listed (0 out, 0 moved) |
| p3e | far and sky | SM_P3E_DiveLine_B | 1120 | 68.8 | TALL | not listed (0 out, 0 moved) |
| p3e | far and sky | SM_P3E_Kites | 1550 | 62.66 | TALL | not listed (0 out, 0 moved) |
| p3e | far and sky | SM_P3E_Balloons_A | 1348 | 49.94 | TALL | not listed (0 out, 0 moved) |
| p3e | far and sky | SM_P3E_Balloons_B | 1192 | 52.58 | TALL | not listed (0 out, 0 moved) |
| p3e | far and sky | SM_P3E_Gliders | 1020 | 54.41 | TALL | not listed (0 out, 0 moved) |
| p3e | far and sky | SM_P3E_StarRing | 1936 | 70.29 | TALL | not listed (0 out, 0 moved) |
| p3e | far and sky | SM_P3E_FishHerd | 1512 | 41.27 | TALL | -1/~0/30 |
| p3e | far and sky | SM_P3E_WaterBus | 1448 | 30.14 | TALL | not listed (0 out, 0 moved) |
| p3e | far and sky | SM_P3E_RidgeClimbers | 1698 | 64.2 | TALL | not listed (0 out, 0 moved) |
| p3e | far and sky | SM_P3E_DivingBirds | 846 | 56.76 | TALL | not listed (0 out, 0 moved) |
| p3f | film shoot | SM_P3F_SecondUnit_A | 2028 | 7.12 | TALL | -0/~4/24 |
| p3f | film shoot | SM_P3F_SecondUnit_B | 2200 | 6.8 | TALL | -0/~5/21 |
| p3f | film shoot | SM_P3F_DroneTeam_A | 1684 | 8.58 | TALL | -1/~3/22 |
| p3f | film shoot | SM_P3F_DroneTeam_B | 2284 | 8.58 | TALL | -1/~3/19 |
| p3f | film shoot | SM_P3F_StuntRig | 1944 | 12.4 | TALL | -2/~3/25 |
| p3f | film shoot | SM_P3F_Extras_A | 2356 | 8.4 | TALL | -0/~4/23 |
| p3f | film shoot | SM_P3F_Extras_B | 1968 | 8.4 | TALL | -0/~2/17 |
| p3f | film shoot | SM_P3F_MakeUp | 2140 | 8.3 | TALL | -0/~1/22 |
| p3f | film shoot | SM_P3F_PressPen_A | 1972 | 6.3 | TALL | -0/~1/24 |
| p3f | film shoot | SM_P3F_PressPen_B | 2204 | 8.53 | TALL | -0/~3/21 |
| p3f | film shoot | SM_P3F_CableCam | 1788 | 7.45 | TALL | -0/~1/23 |
| p3f | film shoot | SM_P3F_LightBalloon | 1584 | 9.32 | TALL | -0/~3/21 |
| p3f | film shoot | SM_P3F_Deck_A | 608 | 6.9 | TALL | -0/~5/39 |
| p3f | film shoot | SM_P3F_Deck_B | 644 | 7.5 | TALL | -0/~5/32 |

Estimated placed, by the builders' own counts (not verified one by one): p3a 649, p3b 284, p3c 432, p3d 376, p3e 322,
p3f 396 (about 256 unit pieces and 70 Sets of a deck with one shared crew kit scene on it); 2459 together.

What each block is, in the builders' words: p3a fills the verge (21.6 to 23.6 m) and the water 30 to 58 m out where the
first passes left nobody, mostly in Nereida; p3b is ten story scenes (a wedding, a pearl market, a treasure haul, a
seahorse race, a kelp harvest, a school trip, a statue dig, a band, a rescue drill, a submarine launch) at 28 to 41 m;
p3c is everything that travels (scooter files, cyclists, sled trains, a turtle wagon, a walkway, a fin race, sea cows,
a cable car, a riding ring, buoy laps) at 26 to 31 m and 58 to 132 m; p3d is people up high on gantries, masts, a spire,
a wreck's mast, a crane, terraces, a rope bridge and a lift at 46 to 70 m; p3e is the far water from 160 m out (dive
lines, kites, balloons, gliders, a star of swimmers, a fish herd, a water bus, ridge climbers, diving birds); p3f is
more of the film shoot (second units, drone teams, stunt rig, extras, make-up, press pens, cable camera, balloon light,
and the shared crew kit stood on rostrums).

## Changed, dropped, moved

- Changed in a block: nothing. Dropped blocks: none. Rules removed: none. Generators rerun: none (every mesh existed).
- Moved or left out by the track: the list above (5 left out; "moved" pieces are pushed out from the barrier by the
  track's own test).

## Known departures the builders reported (not fixed here)

- Most pieces are over the 1,500-triangle guide (up to 2,548, SM_P3B_SeahorseRace).
- p3f made a second editor run to add SM_P3F_Deck_A/_B; its chest band is pink while the shared kit's Sea wardrobe on
  its decks has a cyan band.
- p3b places nothing on the research station's left side and only 3 scenes in the approach.
- p3a: low left-verge pieces in Nereida can share a slot with a crab row in front; left-side tall pieces stand behind
  the travel tube.
- The blocks were written without sight of each other: pieces of different builders can share a slot and a distance.

## Not checked

- FRAMES WERE NOT LOOKED AT BY ANYONE: not by the builders, not by the integrator. No contact sheet was made. Shape,
  pose, colour, readability from the road, whether anything floats, sinks, interpenetrates or is hidden (barrier,
  pipeline, travel tube, kelp), and all movement (travel, orbit, bob) are unseen.
- Overlaps between the six blocks, with the first passes' people and with the dressing.
- Placed counts of the 28 meshes FTCLEAR does not list, and the per-builder estimates.
- Glow and blink rules beyond the builders' statements (p3d, p3e and p3f use steady glow, by their word 24 m out or
  more); likeness to pickups, enemies or the boss.
- Frame rate and memory with 2449 more pieces: the level ran for its first 6 seconds only, on quality 3.
- Later scenes of the level (Nereida, the finale) were not started.

## Review and fix round

Two reviewers looked at pass 3 (a rules reader working from the table and an emulation of the generators; a driver with
12 game runs and about 100 frames: contact sheet `sheets/L6_p3_review.jpg`), then one fix round was made. Nobody has
looked at a frame since the fixes: the level was started once.

### What the reviewers found

- No rule broken and nothing that blocks play: no x min under 17 m, no TALL piece under 24 m, no glow inside 24 m,
  nothing that blinks, nothing of the population on the road, no pickup label, boss or road mark hidden.
- Looks wrong (driver): pieces of different builders stood inside each other. Nereida's right at 30 to 34 m (p3a's
  swimmers through p3f's rostrums and p3c's movers), the salvage field and the approach at 28.8 to 34 m (p3a, p3c and
  p3f in one slot and band), Nereida's left in front of and behind the travel tube (crews read as standing in the tube,
  heads over it); a balloon bunch seen just over the barrier of a banked bend near 3.9 km beside a MISSILES label.
- Weak: the turtle wagon's, scooters' and cyclists' saved bounds reach inside 24 m, so the track pushes them out; the
  sound rostrum of the kelp forest has a point over 2 m at 23.9 m; an orange, pickup-sized box on the sled train's tug;
  drones hover 27 m out; p3b's kelp forest scenes half hidden among the stalks; dark rostrum legs in front of the
  station's tube; density (single ideas no longer read at speed); more slow-frame lines than in the first pass's drives.

### What was fixed (in `FTPeopleL6.inl`'s pass-3 blocks unless said; every changed line says "fix round")

- Nereida's right: p3a's Commuters_A to 33.5-36 m in slot 10 of 12 (was slot 1: a p3f unit and a p3b scene in every
  one); Commuters_B and SchoolClass to 47-50 m in slots 3 and 9 (over and behind the pressure habitats, where nothing
  of the population stood); Catwalk to slot 2 of 60 at 30-30.5 m (was every 12, a p3c mover in four of five);
  RestBuoy to 46.5-48 m (stood inside the p3b scene of its slot); p3b's sixteen right rules start at 37.5 m (36.5);
  p3c's TurtleWagon there every 30 (was 15: half stood in the slots of p3f's kit rostrums and p3a's kelp farmers).
- Nereida's left: p3f's six units in front of the tube every 72 slots (was 36); p3b's ten scenes there every 96 (48)
  and off the slots of p3f's kit rostrums; behind the tube p3a now stands only where nothing stands in front of it
  (RestBuoy, Commuters_A, Commuters_B at 42-43.5 m in slots 0, 12 and 8 of 24; MastCrew and SchoolClass in slots 2
  and 26 of 48); p3a's KelpFarmers rule behind the tube taken out.
- Salvage field and approach: p3a's Commuters_A / _B from 30-34 m every 6 to 45-48 m every 12, in slots where they
  stand behind one piece; MastCrew (salvage, right) to 36-38 m every 12; p3a's Catwalk taken out of the salvage field
  and the research station (each of its slots held a p3c mover in the same band).
- Reef, kelp forest, station's right: p3c's scooter files moved to slots p3f's units do not use (kelp forest: every
  slot of theirs held a p3f unit); p3c's AquaBikes and FinRace rules in front of the station's tube taken out (their
  slots hold a p3f unit and a p3b scene).
- The rules reader's measured overhangs: TurtleWagon 30.6-32.3 m (28.8-30.5), the kelp forest's Scooters 26.8-27.4 m
  and AquaBikes 26.4-27.0 m, the kelp forest's sound rostrum Site at 28 m (27.5).
- `ft_people_sea_p3c.py` line 181: the box on the sled train's tug is magenta (was orange, 154 x 182 x 106 cm as
  placed, bobbing: the missile pickup's colour and size). `SM_P3C_SledTrain` regenerated alone (log
  `C:\ftwork\logs\L6p3fix_make.log`: 1680 triangles, 44 parts, 16.5 x 2.7 m, top 5.45 m, TALL: as before).

Counted from the table (scripts in `C:\ftwork\jobs\L6p3fix`, built on the reviewers' parser and emulated bounds):

| | before | after |
|---|---|---|
| pairs sharing a slot, a side and an overlapping distance range, with a pass-3 piece (the driver's `pairs.py`) | 382 | 167 |
| pairs of two pass-3 pieces whose emulated depths overlap by over 0.5 m (same slot and side, within 60 m) | 494 | 183 |
| the same, Nereida only | 316 | 64 |
| slot sides with 3 or more population pieces within 50 m (of 850 outside tunnels and set pieces) | 414 | 314 |
| slot sides with 5 or more | 65 | 43 |
| slot sides with nobody within 60 m | 83 | 12 |
| pass-3 pieces by the table's count | 2406 | 2261 |

Compile `L6p3_fix_build`: exit 0, "Result: Succeeded". One start (`C:\ftwork\out\p3\fixed_L6`, exit 0, the level
came up, scene 1/7, one frame at 6 s):
`FTTRACK level=6 frames=12470 route=04d3f899 solid=8927e339 solidTriangles=930148` (equal to the baseline's level-6
line); `FTDRESS level=6 districts=370 sets=27 pieces placed=13377 of 13394 tried shadows=10608 of 14232` (before the
fix round: 13521 of 13538); FTCLEAR `props kept=16720 (moved off their own barrier 1719) left out of the road=26`;
no "Fatal error", no "LogFT: Error", no ensure, all 70 SM_P3 meshes in the "Waiting on static mesh" lines. Moved by
the track now: TurtleWagon 9 of 19 (was 29 of 31), Scooters_A 21 of 45 (24 of 45), Scooters_B 16 of 39 (21 of 41),
AquaBikes 18 of 41 (19 of 42), SledTrain 19 of 46 (unchanged).

### What was not fixed

- The balloon bunch near 3.9 km: left as it is. That place (slot 86) is in none of the table's loop or corkscrew slot
  ranges (62-65, 71-76, 128-131, 148-154), so keeping p3e's Far rules out of those slots would not change the frame;
  the only balloon rules on that side there place 200 to 400 m out, and nothing measured shows the bunch near the
  road (over a banked bend far pieces stand just above the barrier).
- p3c and p3f still share slots and a band in the salvage field (slots 2, 5, 7 and 8 of 12 on both sides), the
  approach (Scooters_B with PressPen_A, the wagon and the walkway with Extras_A) and the reef (Walkway with the dolly
  rostrum): no free slot is left there in that band between the first pass's cast, the stands and the pressure
  habitats. The scooter files and cyclists of the other districts (28.8 m, scale 1.4 and 1.5) are still pushed out
  by the track, and so are 9 of the 19 turtle wagons.
- The drone teams stay 27 m out (kelp forest, station's right, Nereida's left): they stand in front of the weed and
  the tube, and 30 m would put them inside those. On Nereida's left there are half as many now.
- p3b's kelp forest scenes stay at 28.0-28.2 m: at 26 m their 3.4 m half depth would stand inside 24 m.
- Rostrum leg colour, SchoolTrip's orange ball, RescueDrill's red stretcher, the triangle counts, the first passes'
  overhanging rules (they pass the x-min test) and their meshes placed more than 60 times: unchanged.

### Never checked

- No frame was looked at after the fixes except that the one start frame shows the level running. Whether the moved
  pieces read (swimmers over the pressure habitats at 47-50 m, pieces 42-43.5 m out before the watch towers, the
  magenta box), and whether anything now stands in a dressing piece that the table's comments do not name (vents,
  reef gardens, kelp stands, coral placed by chance), is unseen.
- Five rules were taken out, so every pass-3 rule after each of them draws other random numbers: pieces with a
  distance range stand elsewhere inside their ranges than in the reviewed frames.
- The overlap counts are table arithmetic on emulated bounds (a piece assumed in every slot its rule names, anywhere
  in its range), not positions from the game.
- Frame time and memory; the level past 13 km; the handled ensure the driver saw in three scene-7 runs (not in this
  start); the two port-8000 listener errors in run.log (also in the run before the fixes).
