# Level 2 (Nevada): population pass 3, integration

Branch `claude/people-L2-p3`. Six builders (p3a..p3f) wrote generators, meshes and table blocks; this note is the
integrator's record of joining them. The integrator compiled once and started the level once. Nobody looked at a frame.

## What pass 3 added

71 new meshes in `/Game/People/Nevada` (`SM_P3A_*` .. `SM_P3F_*`), six generators
`Content/Python/ft_people_nevada_p3<letter>.py`, and six blocks appended to `Source/FullThrottle/FTPeopleL2.inl`
(each wrapped in `{ }`, after the `watch` block; nothing above them was changed). Triangles, top height and TALL/low
are from the builders' make logs (`C:\ftwork\logs\L2p3<letter>_make.log`); "estimated placed" is the builder's own
figure, not a measurement.

| builder | focus | meshes | block | estimated placed |
|---|---|---|---|---|
| p3a | gaps | 11 | 302 rows (1 explicit Site + 301 row Sites) | 302 |
| p3b | scenes | 10 | 402 rows | 402 |
| p3c | movers | 12 | 343 rows | 343 |
| p3d | heights | 13 | 73 Scatter rules in 20 zones, by their own chances | 464 |
| p3e | far and sky | 12 | 147 rules in 19 zones: 294 by Far, 150 by Scatter | 444 |
| p3f | film shoot | 13 | 325 rows; 53 of them two-piece sets with the existing SM_CrewRiser, so 378 pieces | 325 |
| all | | 71 | | 2,280 (2,333 pieces counting p3f's 53 risers) |

### p3a (gaps)

| mesh | triangles | top m | TALL/low |
|---|---|---|---|
| SM_P3A_RoadCrew_A | 1764 | 4.67 | TALL |
| SM_P3A_RoadCrew_B | 2084 | 3.84 | TALL |
| SM_P3A_Picnic | 1696 | 4.45 | TALL |
| SM_P3A_Lookout | 2012 | 6.2 | TALL |
| SM_P3A_FenceMenders | 1552 | 4.59 | TALL |
| SM_P3A_Campers | 1402 | 3.98 | TALL |
| SM_P3A_Painters | 1740 | 4.55 | TALL |
| SM_P3A_Tourists | 1976 | 5.75 | TALL |
| SM_P3A_RimHikers | 1130 | 21.85 | TALL |
| SM_P3A_RimSurvey | 1100 | 21.3 | TALL |
| SM_P3A_RimBighorns | 1020 | 19.72 | TALL |

### p3b (scenes)

| mesh | triangles | top m | TALL/low |
|---|---|---|---|
| SM_P3B_Rodeo | 2468 | 5.54 | TALL |
| SM_P3B_SkyWatch | 2004 | 6.51 | TALL |
| SM_P3B_SaltRecord | 2324 | 5.16 | TALL |
| SM_P3B_Revue | 2436 | 5.65 | TALL |
| SM_P3B_HoeDown | 2300 | 7.2 | TALL |
| SM_P3B_Dig | 2652 | 5.8 | TALL |
| SM_P3B_PoolParty | 2266 | 5.01 | TALL |
| SM_P3B_CookOff | 2652 | 5.4 | TALL |
| SM_P3B_PrizeFight | 2260 | 6.1 | TALL |
| SM_P3B_RimRescue | 1668 | 21.7 | TALL |

### p3c (movers)

| mesh | triangles | top m | TALL/low |
|---|---|---|---|
| SM_P3C_Runners_A | 2212 | 4.77 | TALL |
| SM_P3C_Runners_B | 2120 | 4.62 | TALL |
| SM_P3C_Hikers | 2120 | 4.65 | TALL |
| SM_P3C_Cyclists_A | 1964 | 5.42 | TALL |
| SM_P3C_Parade | 2868 | 5.77 | TALL |
| SM_P3C_StripWalkers | 2496 | 5.22 | TALL |
| SM_P3C_Cyclists_B | 2376 | 7.5 | TALL |
| SM_P3C_MuleTrain | 1684 | 8.21 | TALL |
| SM_P3C_Stagecoach | 1376 | 8.78 | TALL |
| SM_P3C_Handcar | 1468 | 7.89 | TALL |
| SM_P3C_LandYachts | 876 | 13.46 | TALL |
| SM_P3C_RoundPen | 2396 | 6.25 | TALL |

### p3d (heights)

| mesh | triangles | top m | TALL/low |
|---|---|---|---|
| SM_P3D_Scaffold_A | 1688 | 13.8 | TALL |
| SM_P3D_Scaffold_B | 1928 | 9.8 | TALL |
| SM_P3D_WaterTower | 1488 | 10.92 | TALL |
| SM_P3D_Billboard | 1720 | 12.72 | TALL |
| SM_P3D_RockSpire_A | 1528 | 17.98 | TALL |
| SM_P3D_RockSpire_B | 1484 | 25.77 | TALL |
| SM_P3D_MesaLedge | 1936 | 10.2 | TALL |
| SM_P3D_RoofParty | 1620 | 11.13 | TALL |
| SM_P3D_SignRig | 1768 | 13.47 | TALL |
| SM_P3D_Trestle | 1696 | 9.75 | TALL |
| SM_P3D_Derrick | 1456 | 14.9 | TALL |
| SM_P3D_Lookout | 1196 | 13.9 | TALL |
| SM_P3D_RVRoof | 1560 | 8.15 | TALL |

### p3e (far and sky)

| mesh | triangles | top m | TALL/low |
|---|---|---|---|
| SM_P3E_Balloons_A | 1328 | 95.52 | TALL |
| SM_P3E_Balloons_B | 1328 | 99.88 | TALL |
| SM_P3E_HangGliders | 600 | 63.71 | TALL |
| SM_P3E_ParaGliders | 840 | 57.04 | TALL |
| SM_P3E_Skydivers | 1512 | 79.38 | TALL |
| SM_P3E_Geese | 1034 | 35.29 | TALL |
| SM_P3E_Paramotors | 1120 | 50.83 | TALL |
| SM_P3E_Kites | 1436 | 52.2 | TALL |
| SM_P3E_DirtBikes | 1420 | 9.4 | TALL |
| SM_P3E_LandYachts | 1344 | 15.1 | TALL |
| SM_P3E_Climbers | 1256 | 59.24 | TALL |
| SM_P3E_Rodeo | 1736 | 19.9 | TALL |

### p3f (film shoot)

| mesh | triangles | top m | TALL/low |
|---|---|---|---|
| SM_P3F_SecondUnit_A | 2320 | 5.5 | TALL |
| SM_P3F_SecondUnit_B | 2368 | 5.5 | TALL |
| SM_P3F_CamTruck | 1688 | 4.75 | TALL |
| SM_P3F_DroneTeam_A | 2132 | 6.76 | TALL |
| SM_P3F_DroneTeam_B | 2616 | 7.78 | TALL |
| SM_P3F_StuntRig_A | 2084 | 7.56 | TALL |
| SM_P3F_StuntRig_B | 1932 | 8.77 | TALL |
| SM_P3F_Extras_A | 2520 | 5.5 | TALL |
| SM_P3F_Extras_B | 2572 | 5.5 | TALL |
| SM_P3F_MakeUp | 2484 | 5.5 | TALL |
| SM_P3F_PressPen_A | 2842 | 5.5 | TALL |
| SM_P3F_PressPen_B | 2914 | 5.5 | TALL |
| SM_P3F_CamTower | 1468 | 9.4 | TALL |

All 71 are TALL. 49 of the 71 are over the 1,500-triangle guide (the heaviest: SM_P3F_PressPen_B 2,914,
SM_P3C_Parade 2,868, SM_P3F_PressPen_A 2,842); none was trimmed.

## FTDRESS before and after

| | districts | sets | pieces placed | tried |
|---|---|---|---|---|
| before (`C:\ftwork\logs\p3_prep_L2.txt`, at 008edbd) | 1212 | 89 | 9404 | 9434 |
| after (`C:\ftwork\out\p3\after_L2\run.log`) | 2625 | 166 | 11676 | 11706 |

Pass 3 added 2,272 tried and 2,272 placed (30 not placed, the same count as before).

FTTRACK after: `FTTRACK level=2 frames=12888 route=efc95392 solid=e2d27537 solidTriangles=964188`, identical to the
`FTTRACK level=2` line of `SourceArt/Jam/validate/track_baseline.txt`.

## Checks, and what was changed, dropped, moved or left out

- Every mesh named by the six blocks exists as a `.uasset` (the 71 new ones, plus the existing `SM_CrewRiser` and the
  ten `SM_Crew<Piece>_Desert` kit pieces p3f places). No generator was rerun; no generator was edited.
- Distance rules, from the text: all 71 meshes are TALL; the smallest x in each block is a 33.1 m, b 33.1 m, c 36.0 m,
  d 44 m (Scatter x min), e 160 m, f 26.8 m. No rule is under 24 m: nothing was raised.
- Blocks: all six went in as written. Nothing was changed inside a block and none was dropped.
- Compile: tag `L2p3_build`, first try, exit 0, "Result: Succeeded" (`C:\ftwork\logs\L2p3_build.log`). No fix was needed.
- Level start (game level 3, one run, exit 0, one frame saved at clock 6.0): no "Fatal error", no "LogFT: Error" and no
  failed load of a `/Game/People` asset in the log. The log has a "Waiting on static mesh" line for each of the 71 new
  meshes (a 11, b 10, c 12, d 13, e 12, f 13): the level loads every one.
- FTCLEAR (last line; it lists only meshes with something left out or moved). Left out of the road: none of the new
  meshes. Moved out from the barrier, all of them p3f's near pieces (written 26.8 to 29.3 m out):

| mesh | left out | moved | placed |
|---|---|---|---|
| SM_P3F_SecondUnit_A | 0 | 2 | 18 |
| SM_P3F_DroneTeam_A | 0 | 5 | 18 |
| SM_P3F_Extras_A | 0 | 3 | 18 |
| SM_P3F_StuntRig_A | 0 | 3 | 18 |
| SM_P3F_PressPen_A | 0 | 6 | 18 |
| SM_P3F_CamTower | 0 | 3 | 31 |
| SM_P3F_MakeUp | 0 | 3 | 18 |
| SM_P3F_SecondUnit_B | 0 | 7 | 17 |
| SM_P3F_StuntRig_B | 0 | 3 | 30 |
| SM_P3F_Extras_B | 0 | 7 | 17 |
| SM_P3F_DroneTeam_B | 0 | 4 | 17 |
| SM_P3F_PressPen_B | 0 | 3 | 17 |
| SM_P3F_CamTruck | 0 | 4 | 17 |

- p3f's own meshes: 254 placed, 53 of them moved, 0 left out. `SM_CrewRiser` (first round and p3f together): 0 left
  out, 4 moved, 108 placed. No SM_P3A..SM_P3E mesh is in the FTCLEAR list: none of theirs was left out or moved, and
  the log therefore does not print their per-mesh placed counts.
- The builders expected the track to leave out rows beside loops, jumps and corkscrews. It left out no pass 3 piece.

## Not checked

- Frames were NOT looked at by anyone: not by the six builders (no contact sheets, no game run) and not by the
  integrator (the start script saved one frame; it was not opened). How any of the 71 meshes looks, reads from the
  road, moves, floats or interpenetrates is unseen.
- Per-mesh placed counts for p3a..p3e (not printed when nothing is left out or moved); only the total of 2,272 and
  that every mesh is loaded.
- Why 53 of p3f's pieces were moved off the barrier, and where they stand now.
- Overlaps between the six blocks, with the first cast, or with the dressing (signs, wrecks, rock banks, herds).
- Whether pieces sit on the rock cuts' rims, whether rim and air pieces show over the cut walls, whether the negative
  Far drop lifts p3e's balloons, whether the 22.6 m half-slot offset lands between slots on both sides.
- Glow, blinking and "nothing like a pickup or the boss": taken from the builders' reports, not checked in the generators.
- Performance (frame time with 2,272 more pieces and 49 meshes over the triangle guide).
- The log line "LogFT: Warning: FT WARM no assets found (asset registry empty?)" is in this run; whether it was there
  before pass 3 was not checked.
- Only the level's first six seconds ran, from its first scene; later scenes and the boss were not started.

## Review and fix round

Two reviewers looked at pass 3 after the integration: a rules reader (text only: the table, the six generators and the
make logs, through a stand-in for Unreal) and a driver (11 game runs, frames from 0 to about 16.7 km; contact sheet
`sheets/L2_p3_review.jpg`, 12 crops). One fixer then made ONE round of changes, compiled, started the level once and
committed. The fixer looked at no frame: nothing below has been seen in the game since it was changed.

### What the reviewers found

- Rule broken (rules reader): `SM_P3F_DroneTeam_A` had a camera drone hovering free 4.3 m over its pad, 27 m from the
  centre line (18 rows); `SM_P3F_DroneTeam_B` had a second drone circling free over the riser, to 24.1 m from the centre
  line (17 rows). The brief: drones are props on poles or hovering by bob, never near the road; the level's enemies fly.
- Looks wrong (driver): two pass 3 pieces on the same spot (p3a's fillers on p3c's berms, on p3b's half-slot scenes
  and on p3b's rim rescue); dark winged flyers in the same sky as the enemy helicopters (dry wash, Joshua Flats, Last
  Chance Gas); three or four sky pieces heaped on one slot at the state line; balloons low and huge over the camps of
  the salt flats; kite strings, climbing ropes and lattice towers drawing as dashes and hatching.
- Weak: nobody in sight through Red Rock Pass's wall bends (5.14 to 6.0 km); rim pieces small and dark over the dry
  wash's long cut; the roadside three rows deep from 6.3 to 10.7 km; p3c's far ground movers not found from road
  level; ten p3e rules whose phase meets no slot of their zone; nine p3b Sets that no row uses; a wrong distance in
  the balloons' comment; `SM_CrewRiser` placed over 100 times; a handled engine ensure (DoubleFloat.cpp) in five runs.

### What was fixed

| finding | change |
|---|---|
| free-flying drones (rule broken, both) | Generator `ft_people_nevada_p3f.py`: team A's drone now stands ON its pad with its blades still; team B's circling drone is deleted (the docked one on its mast stays). Both meshes regenerated (`C:\ftwork\logs\L2p3_fix_make.log`: DroneTeam_A 2,084 triangles, top 5.5 m, TALL; DroneTeam_B 2,248 triangles, top 6.66 m, TALL; built 2, failed 0). No drone of pass 3 flies any more. The rows were left where they were. |
| two pieces on one spot | p3a: the 91 rows that shared slot, side, half slot and distance (within 5 m) with a row of p3b or p3c are out (301 -> 210 rows), and so is p3a's one explicit row (FenceMenders, slot 6 left, on p3c's cyclists). p3b's and p3c's rows are untouched. |
| flyers beside the helicopters | p3e, zones dry wash (west cut, between the cuts, long cut), Joshua Flats, Last Chance Gas: the HangGliders and Paramotors rules are out; Skydivers and ParaGliders stand 300 to 360 m out. The balloons stay. |
| the heap at the state line | p3e: in every open district the sky (Far) rules of one side now share one `every` (12; in the short zones one slot each) with phases spread so that no two meet on the same or the next slot (by count: no such meeting left on any side of any zone). |
| low balloons on the salt flats | p3e: both balloon rules of the salt flats have drop -40 (40 m higher). |
| empty Red Rock Pass | p3e: balloons on both sides every third slot (Balloons_B and Balloons_A alternating on the left, Balloons_A and Skydivers on the right; Balloons_A stands 160 to 200 m out there, lifted 120 m). Sky only; nothing on the ground. |
| three rows deep, 6.3 to 10.7 km | p3f: every second row of slots 140 to 236 is out (36 -> 18 rows; 325 -> 307 rows in all). |
| dead phases | p3e: no rule is left that meets no slot of its zone (by count). |
| unused Sets | p3b: the nine plain scene Sets are deleted (the level has 157 Sets now, was 166). |
| balloon comment | `ft_people_nevada_p3e.py`: "230 m and more" -> "160 m and more" (comment only; no p3e mesh was regenerated). |

By the fixer's count p3e now places 480 pieces, the most of one mesh 58 (Balloons_B; Balloons_A 54, Skydivers 52).

### What was not fixed

- Kite strings, climbing ropes: the generator already draws the kite lines 22 cm thick in white and the ropes 24 cm
  thick in hot colours, so the suggested fix (8 cm or thicker, light) is what is there. Not changed. The lattice towers
  of p3d and p3f were not changed either (a redraw, not a table edit).
- Rim pieces small and dark over the cuts: scaling them 1.3 would lift their people 5 m off the rim (the piece brings
  its own 16.5 m column), and flags are a new drawing. Not changed.
- Jump approaches empty (1.3 to 1.6 km, 3.0 km): the track keeps pieces out there. Not changed.
- p3c's far ground movers behind the near rows: left as depth.
- The camera truck inside 40 m: the driver saw nothing that looks like the Big Dealer's truck. Not changed.
- `SM_CrewRiser` is still placed 105 times (was 108), 61 of them by the first round. Older meshes over 3,000
  triangles and the other older meshes placed over 60 times were not touched (not pass 3's).

### After the fix (one compile, one start of the level)

- Compile: tag `L2p3_fix_build`, exit 0, "Result: Succeeded".
- Level start (`C:\ftwork\out\p3\fixed_L2\run.log`, exit 0): `FTTRACK level=2 frames=12888 route=efc95392
  solid=e2d27537 solidTriangles=964188`, identical to the baseline's level 2 line.
  `FTDRESS level=2 districts=2516 sets=157 pieces placed=11589 of 11619 tried` (before the fix: districts 2625, sets
  166, 11676 of 11706). No "Fatal error", no "LogFT: Error", no "Ensure condition failed", no failed load of a
  `/Game/People` asset; a "Waiting on static mesh" line for 71 SM_P3 meshes.
- FTCLEAR: no pass 3 mesh left out; p3f's near pieces are still the ones moved off the barrier (50 moved).

### Never checked

- No frame was looked at after the fix. Unseen: the drone on its pad, the lifted balloons of the salt flats and of Red
  Rock Pass (whether they show over the rim from the banked road), the sky pieces at 300 m, the new spacing of the sky
  pieces, the thinned rows.
- 16.7 to 25.6 km was never driven by anyone (about a third of the road and of pass 3's pieces).
- Movement of the 71 meshes one by one; flicker of thin parts in motion; performance.
- Whether the geese's and the powered paragliders' long flight lines (420 m, 500 m) come over the road in a bend.
- Overlaps of pass 3 with the first cast and with the dressing; p3f's rows against the others; where the 50 moved p3f
  pieces stand.
- The DoubleFloat ensure: it is also in a first-round run log from before pass 3 (`C:\ftwork\out\L2\d3\run.log`) and
  not in this round's one start; whether pass 3 makes it more frequent was not checked.
- Per-mesh placed counts of p3a to p3e are counts from the table, not figures from the log.
