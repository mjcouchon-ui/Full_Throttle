# Level 7, City Center: population pass 3 (integration)

Six builders (p3a to p3f) each wrote a generator, meshes and a table block; this note is the integrator's record of joining them.
Nobody looked at a frame: not the builders, not the integrator. Everything below is from logs and from the text of the blocks.

## What pass 3 added

78 new meshes in `/Game/People/CityCenter` (prefix `SM_P3A_` to `SM_P3F_`), six generators `Content/Python/ft_people_city_p3a.py` to `_p3f.py`,
and six blocks appended to `Source/FullThrottle/FTPeopleL7.inl` (each wrapped in `{ }`, after the watch block; nothing above them was changed).

| builder | focus | meshes | block | builder's estimate of placed pieces |
|---|---|---|---|---|
| p3a | gaps | 16 | 21 Sets, 12 Zones, 72 Site rules | 395 |
| p3b | scenes | 12 | 49 Sets, 11 Zones, 77 Site rules | 382 |
| p3c | movers | 14 | 4 Sets, 8 Zones, 54 rules (Scatter and Site) | 490 |
| p3d | heights | 12 | 44 Sets, 8 Zones, 104 Site rules | 521 |
| p3e | far and sky | 13 | no Sets, 3 Zones, 74 Far rules | 400 |
| p3f | film shoot | 11 | 65 Sets, 10 Zones, 98 Site + 2 Far rules (also places 9 first-pass crew kit meshes SM_Crew*_City) | 371 |

Estimates are the builders' own arithmetic (sum 2,559). Measured by the game: 2,128 more pieces tried, 2,123 more placed (see below).

### The meshes

Triangles, top height and TALL/low are from the make logs (`C:\ftwork\logs\L7p3<letter>_make.log`). The last column is the
`FTCLEAR level=7` entry of the one game run: left out / moved off the barrier / placed. That line lists only meshes with at least one
piece moved or left out, so "not listed" means none was moved or left out; such a mesh's own placed count is not printed anywhere
(it could also be zero) and was not measured.

| mesh | builder | triangles | top m | TALL/low | left out / moved / placed |
|---|---|---|---|---|---|
| SM_P3A_Bleacher_A | p3a | 1592 | 1.92 | low | not listed |
| SM_P3A_Bleacher_B | p3a | 1592 | 1.93 | low | 0 / 1 / 15 |
| SM_P3A_Buskers_A | p3a | 1432 | 1.91 | low | not listed |
| SM_P3A_Buskers_B | p3a | 1564 | 1.93 | low | 0 / 1 / 16 |
| SM_P3A_Chess_A | p3a | 1576 | 1.86 | low | 0 / 2 / 17 |
| SM_P3A_Flowers_A | p3a | 1500 | 1.92 | low | 0 / 1 / 15 |
| SM_P3A_Mural_A | p3a | 1388 | 1.94 | low | not listed |
| SM_P3A_Mural_B | p3a | 1388 | 1.94 | low | 0 / 1 / 13 |
| SM_P3A_Porters_A | p3a | 1392 | 1.9 | low | not listed |
| SM_P3A_Press_A | p3a | 1868 | 1.88 | low | 0 / 1 / 18 |
| SM_P3A_Press_B | p3a | 2000 | 1.88 | low | 0 / 1 / 18 |
| SM_P3A_Strip_A | p3a | 1720 | 1.85 | low | 0 / 3 / 25 |
| SM_P3A_Strip_B | p3a | 1584 | 1.84 | low | 0 / 6 / 31 |
| SM_P3A_Strip_C | p3a | 1804 | 1.85 | low | 0 / 1 / 36 |
| SM_P3A_Strip_D | p3a | 2024 | 1.84 | low | not listed |
| SM_P3A_Sweepers_A | p3a | 1512 | 1.92 | low | 0 / 1 / 21 |
| SM_P3B_Brokers | p3b | 2524 | 4.28 | TALL | 0 / 6 / 34 |
| SM_P3B_Mayor | p3b | 1962 | 4.0 | TALL | 0 / 5 / 12 |
| SM_P3B_NeonCrew | p3b | 1904 | 4.8 | TALL | 0 / 3 / 30 |
| SM_P3B_PigeonLoft | p3b | 1844 | 4.72 | TALL | 0 / 4 / 33 |
| SM_P3B_RoofBoxing | p3b | 1812 | 3.3 | TALL | 0 / 3 / 26 |
| SM_P3B_RoofCatwalk | p3b | 2284 | 2.92 | TALL | 0 / 1 / 15 |
| SM_P3B_RoofCinema | p3b | 1900 | 3.5 | TALL | 0 / 1 / 16 |
| SM_P3B_RoofDojo | p3b | 1584 | 3.2 | TALL | 0 / 4 / 22 |
| SM_P3B_RoofFarm | p3b | 2312 | 3.3 | TALL | 0 / 2 / 26 |
| SM_P3B_RoofHoops | p3b | 1680 | 3.61 | TALL | 0 / 1 / 28 |
| SM_P3B_RoofJazz | p3b | 2062 | 3.52 | TALL | 0 / 1 / 15 |
| SM_P3B_RoofWedding | p3b | 2256 | 4.18 | TALL | not listed |
| SM_P3C_DogWalk_A | p3c | 1546 | 1.86 | low | 0 / 5 / 32 |
| SM_P3C_MarchMove_A | p3c | 1596 | 2.01 | TALL | 0 / 5 / 29 |
| SM_P3C_Mounted_A | p3c | 1488 | 2.29 | TALL | 0 / 16 / 16 |
| SM_P3C_Patrol_A | p3c | 1548 | 1.71 | low | 0 / 2 / 41 |
| SM_P3C_Patrol_B | p3c | 1536 | 1.68 | low | 0 / 3 / 42 |
| SM_P3C_Porters_A | p3c | 1648 | 1.84 | low | 0 / 5 / 33 |
| SM_P3C_RoofRun_A | p3c | 992 | 1.64 | low | 0 / 2 / 16 |
| SM_P3C_RoofRun_B | p3c | 1070 | 1.72 | low | not listed |
| SM_P3C_Runners_A | p3c | 2032 | 1.67 | low | 0 / 5 / 32 |
| SM_P3C_Runners_B | p3c | 2040 | 1.71 | low | 0 / 4 / 32 |
| SM_P3C_School_A | p3c | 2438 | 1.74 | low | 0 / 3 / 18 |
| SM_P3C_SideStreet_A | p3c | 1840 | 2.22 | TALL | 0 / 15 / 43 |
| SM_P3C_SideStreet_B | p3c | 1788 | 2.21 | TALL | 0 / 14 / 43 |
| SM_P3C_Skaters_A | p3c | 1448 | 1.84 | low | 0 / 2 / 27 |
| SM_P3D_Abseil_A | p3d | 1304 | 36.64 | TALL | 0 / 7 / 26 |
| SM_P3D_Band_A | p3d | 1848 | 4.7 | TALL | 0 / 3 / 33 |
| SM_P3D_Billboard_A | p3d | 1716 | 6.4 | TALL | 0 / 8 / 44 |
| SM_P3D_Bleacher_A | p3d | 1848 | 4.97 | TALL | 0 / 5 / 46 |
| SM_P3D_Bleacher_B | p3d | 1524 | 4.38 | TALL | 0 / 5 / 33 |
| SM_P3D_CrowsNest_A | p3d | 984 | 9.46 | TALL | 0 / 2 / 46 |
| SM_P3D_HighWire_A | p3d | 1236 | 6.14 | TALL | 0 / 8 / 28 |
| SM_P3D_Lookout_A | p3d | 1464 | 6.39 | TALL | 0 / 4 / 42 |
| SM_P3D_Lookout_B | p3d | 1616 | 6.55 | TALL | 0 / 6 / 38 |
| SM_P3D_Lookout_C | p3d | 1564 | 6.44 | TALL | 0 / 2 / 46 |
| SM_P3D_SkyWalk_A | p3d | 2032 | 10.25 | TALL | 0 / 4 / 12 |
| SM_P3D_Ticker_A | p3d | 1944 | 4.2 | TALL | 0 / 2 / 39 |
| SM_P3E_Abseil | p3e | 1724 | 69.28 | TALL | 1 / 0 / 37 |
| SM_P3E_Balloons_A | p3e | 1000 | 36.93 | TALL | not listed |
| SM_P3E_Balloons_B | p3e | 1520 | 45.06 | TALL | not listed |
| SM_P3E_DragonKite | p3e | 552 | 43.08 | TALL | not listed |
| SM_P3E_FarStand | p3e | 2064 | 66.76 | TALL | not listed |
| SM_P3E_HighWire | p3e | 1940 | 71.14 | TALL | 1 / 0 / 36 |
| SM_P3E_KiteRoof | p3e | 1524 | 94.71 | TALL | not listed |
| SM_P3E_Lanterns_A | p3e | 792 | 50.9 | TALL | not listed |
| SM_P3E_Lanterns_B | p3e | 720 | 42.56 | TALL | not listed |
| SM_P3E_Murmur_A | p3e | 1296 | 19.41 | TALL | not listed |
| SM_P3E_Murmur_B | p3e | 1560 | 16.17 | TALL | not listed |
| SM_P3E_RoofCinema | p3e | 1788 | 74.16 | TALL | not listed |
| SM_P3E_Viaduct | p3e | 1644 | 61.6 | TALL | not listed |
| SM_P3F_DroneTeam_A | p3f | 1588 | 3.6 | TALL | 0 / 1 / 10 |
| SM_P3F_DroneTeam_B | p3f | 1708 | 3.83 | TALL | 0 / 4 / 7 |
| SM_P3F_Extras_A | p3f | 2108 | 3.52 | TALL | 0 / 3 / 18 |
| SM_P3F_Extras_B | p3f | 2168 | 2.24 | TALL | 0 / 3 / 22 |
| SM_P3F_LowUnit | p3f | 1684 | 1.88 | low | 3 / 2 / 3 |
| SM_P3F_MakeUp | p3f | 1800 | 2.6 | TALL | not listed |
| SM_P3F_Playback | p3f | 1868 | 2.17 | TALL | 0 / 1 / 20 |
| SM_P3F_PressPen | p3f | 2060 | 3.72 | TALL | 0 / 1 / 18 |
| SM_P3F_SecondUnit_A | p3f | 2100 | 3.3 | TALL | 0 / 3 / 19 |
| SM_P3F_SecondUnit_B | p3f | 1810 | 3.47 | TALL | 0 / 1 / 19 |
| SM_P3F_StuntRig | p3f | 1626 | 4.32 | TALL | 0 / 2 / 18 |

Listed p3 meshes together: 5 left out, 208 moved, 1546 placed (59 of 78 meshes listed). 60 of 78 meshes are over the 1,500-triangle guide (not reduced).
No listed mesh is placed more than 60 times (highest: 46).

## FTDRESS before and after

- Before (`C:\ftwork\logs\p3_prep_L7.txt`): `FTDRESS level=7 districts=102 sets=83 pieces placed=8283 of 8332 tried`
- After (`C:\ftwork\out\p3\after_L7\run.log`): `FTDRESS level=7 districts=154 sets=266 pieces placed=10406 of 10460 tried shadows=4043 of 10550`
- Difference: +52 zones, +183 sets, +2,128 tried, +2,123 placed.
- `FTTRACK level=7 frames=12717 route=5faaeb1d solid=25fa2576 solidTriangles=957884`: identical to the `FTTRACK level=7` line of `SourceArt/Jam/validate/track_baseline.txt`.
- `FTCLEAR level=7 props kept=14081 (moved off their own barrier 1855) left out of the road=63 pillars left out=0`.
- No "Fatal error", no "LogFT: Error", no failed load of a /Game/People asset in the run log. The level came up (`FT LEVEL id=L8 scene=1`).

## What was changed, dropped, moved or left out

- Changed by the integrator: nothing. All 78 meshes the blocks name exist as .uasset (and the 9 crew kit meshes p3f places); no generator was rerun.
  From the text of the blocks, no rule places a TALL mesh under 24 m and none places anything under 17 m (a Site was checked both at its x and at
  x plus its set part's "out"), so no distance was raised. The first compile succeeded (`C:\ftwork\logs\L7p3_build.log`, exit 0); no block was edited or taken out.
- Dropped blocks: none.
- Left out by the track (pieces): SM_P3E_Abseil 1, SM_P3E_HighWire 1, SM_P3F_LowUnit 3 (it is the only p3 piece placed inside the loops).
- Moved off the barrier by the track: 208 pieces of the listed p3 meshes. Most: SM_P3C_Mounted_A 16 of 16 (every one), SM_P3C_SideStreet_A 15 of 43,
  SM_P3C_SideStreet_B 14 of 43, SM_P3D_Billboard_A 8 of 44, SM_P3D_HighWire_A 8 of 28, SM_P3D_Abseil_A 7 of 26. A moved roof piece may no longer
  stand on the roof edge it was written for; this was not looked at.
- Not listed in FTCLEAR (no piece moved or left out; placed count not measured): SM_P3A_Bleacher_A, SM_P3A_Buskers_A, SM_P3A_Mural_A, SM_P3A_Porters_A, SM_P3A_Strip_D, SM_P3B_RoofWedding, SM_P3C_RoofRun_B, SM_P3E_Balloons_A, SM_P3E_Balloons_B, SM_P3E_DragonKite, SM_P3E_FarStand, SM_P3E_KiteRoof, SM_P3E_Lanterns_A, SM_P3E_Lanterns_B, SM_P3E_Murmur_A, SM_P3E_Murmur_B, SM_P3E_RoofCinema, SM_P3E_Viaduct, SM_P3F_MakeUp.

## Known from the builders, not acted on

- Triangle guide exceeded on 60 meshes (worst: SM_P3B_Brokers 2,524, SM_P3C_School_A 2,438, SM_P3B_RoofFarm 2,312).
- Nobody placed in the suggested ground bands at 26 to 60 m: on this level that ground is podiums and towers. b, d and f all stand on podium roofs
  by Sites at 29 m copying the dressing's podium every/phase, and were written without seeing each other's blocks: two or three roof pieces can
  share one roof spot (for example the tall-shop spot at -10 m along is used by sets of b and of d; f uses -11.2 m on the same shops).
- a's walkway (23.72 m) and c's door lane (24.42 m) are 0.7 m apart and both 1 to 1.8 m deep: they can overlap where a slot has both, and with the
  first pass's commuters (23.5 to 24.1 m).
- d's SM_P3D_Abseil_A stands at chance 1 at alley gaps whose own chance is 0.8 to 0.9, so in about one gap in ten it stands without the building.
- e's travelling sky pieces fly straight lines 200 to 360 m long, 95 m and more up; on bends a line can pass over another part of the road.
- No near zone is cut round banked bends (as in the first table).

## Not checked

- FRAMES WERE NOT LOOKED AT BY ANYONE. The one game run saved one frame (`C:\ftwork\out\p3\after_L7\L7_t006.png`); it was not opened.
  So not checked: how any piece looks, whether it moves, floats, sinks into a roof, stands inside a dressing piece or another builder's piece,
  whether anything reads from the road, light levels at night, and whether anything can be taken for a pickup, an enemy or the Chairman.
- Roof heights and free roof spots (taken by the builders from ft_dress_metro.py), the mirroring of "along" offsets on the left side.
- The placed count of the 19 meshes FTCLEAR does not list, and of p3f's extra crew kit pieces separately from the first pass's.
- Frame rate, memory, load time. Only level 7 scene 1 was started, for 6 seconds of clock; later scenes and the boss arena were not started.
- The glow and blink rules (no glow under 24 m, no blink under 30 m): taken from the builders' statements, not verified in the generators or in the game.
- Travel paths not crossing the road: from the builders' statements only.

## Review and fix round (16 Nov 2026)

Two reviewers who had not built the pass looked at it: a rules reader (the whole table and the six generators, by script, no frame) and a
driver (22 game runs over all 7 scenes, 120 stills read; contact sheet `sheets/L7_p3_review.jpg`). One fix round followed, by a third hand.
Everything below is what was run and seen in that round; the reviewers' own "not checked" lists still stand.

### What the reviewers found

- Rules reader. No rule under 17 m, no TALL mesh with its origin under 24 m, no glowing part under 24 m, no missing set or mesh. One
  "rule broken" by its own reading of the light rule: ten pieces that call `lit()` (every part faintly lit) had limbs, sticks or wings
  swinging at 2.2 to 3.0 Hz, 25 to 29 m out (RoofJazz, PigeonLoft, RoofBoxing, RoofHoops, RoofCatwalk, RoofWedding, Brokers, Band_A, Extras_A,
  RoofRun_B). The light itself never changes. Weak: Mural_A's waving roller reached 2.04 m at 21.9 m; MarchMove_A's and Mounted_A's fronts
  23.6 and 23.9 m out; two drones circling a mast 25 to 30 m out on a level whose enemies include helicopters; the sweeper's colours; the far
  viaduct and lanterns; three older far and crew rules; the budget facts.
- Driver. Looks wrong: two or three pass-3 roof pieces in one roof spot on most podium roofs (132 rule pairs of p3b, p3d, p3f and p3c in the
  same side, slot and place), and the mayor inside the lantern bridge on every monument plaza. Weak: the right-hand pavement of the arena
  empty at street level for long stretches; the Plaza's open blocks empty; brokers hidden to the waist from the near lane; low kerb pieces
  reading as dark heads; sky pieces bright at the frame's edge on banked bends; one unidentified hanging cloth. Nothing on the road, nothing
  hiding the road, a pickup, an enemy or the Chairman; nothing read as one of them.

### What was fixed

- Fast lit limbs: every rate over 2.0 on those ten pieces is now 2.0 (p3b: its own capped copy of the first cast's CHEER and ten lines;
  p3c, p3d, p3f one line each). The ten meshes were rebuilt. Checked with the rules reader's own simulation of the generators: no lit part
  faster than 2 Hz on any p3 piece nearer than 30 m (SideStreet_A/B, 52 m out, keep 2.15 and 2.3 Hz).
- Mural_A / Mural_B: the waving painter no longer hops (bob 5 to 0). Moving top by the same simulation 1.99 m (was 2.04). Both rebuilt.
- DroneTeam_B: the two drones no longer circle the mast; they hover either side of it and bob, 27.5 m out or more. Rebuilt.
- Roof spots: `C:\ftwork\jobs\L7p3fix\resolve.py` shares out the slots wherever Site rules of two p3 blocks met (same side, zones
  overlapping, a common slot, set pieces within 6.5 m along, 3 m out and 3 m up: the driver's own test). 187 of the 364 p3 Site rules now have
  a multiple of their podium rule's every (two pieces took turns: twice; three: three times), some as two or four lines; the changed lines
  say what they were. The driver's `collide.py` on the new table: 0 pairs between p3 blocks (132 before). Mayor and SkyWalk_A now stand on
  alternate monuments. Left: 8 pairs with the first pass's crew Sound and Catering in the two short crew zones of the Plaza (about 2 spots).
- Arena, right side, street level: two new sets (Press_B and Bleacher_A at a block's middle) and three rules in "P3A gaps: Syndicate
  Territory, the arena" (Press_B every 8, Bleacher_A every 8, one more Strip_D walkway every 16, chance 0.7 each), in slots where the block
  had no Md or St piece. No new mesh.
- Regenerated meshes (13, `C:\ftwork\logs\L7p3_fix_make.log`, 0 failed; triangles, size and top as in the first make logs): SM_P3A_Mural_A,
  SM_P3A_Mural_B, SM_P3B_RoofWedding, SM_P3B_RoofJazz, SM_P3B_RoofBoxing, SM_P3B_PigeonLoft, SM_P3B_RoofCatwalk, SM_P3B_RoofHoops,
  SM_P3B_Brokers, SM_P3C_RoofRun_B, SM_P3D_Band_A, SM_P3F_Extras_A, SM_P3F_DroneTeam_B.

### After the fix (`C:\ftwork\out\p3\fixed_L7\run.log`)

Built and started twice: once with the lane pieces at 24.9 m (that run is kept in `C:\ftwork\out\L7p3fix\run1_march249`), once after they
were put back. The lines below are from the second, final run; both builds succeeded (`C:\ftwork\logs\L7p3_fix_build.log`).


- `FTTRACK level=7 frames=12717 route=5faaeb1d solid=25fa2576 solidTriangles=957884`: identical to the baseline's level 7 line.
- `FTDRESS level=7 districts=154 sets=268 pieces placed=10018 of 10073 tried shadows=3678 of 10162` (before the round 10406 of 10460: 388
  fewer pieces, the price of one piece a roof spot; pass 3 now adds 1,735 to the 8,283 before it).
- `FTCLEAR level=7 props kept=13693 (moved off their own barrier 1806) left out of the road=64`. No "Fatal error", no "LogFT: Error", no
  failed load of a /Game/People asset; the level came up (`FT LEVEL id=L8 scene=1`).
- Placed by FTCLEAR after the round (listed meshes only): RoofJazz 8, RoofCatwalk 6, Mayor 4, SkyWalk_A 8, HighWire_A 8, Billboard_A 10,
  NeonCrew 12, PigeonLoft 14, RoofBoxing 13, Band_A 19, Bleacher_A (p3d) 20, Bleacher_B (p3d) 15; Press_B 29 and Bleacher_A (p3a) 28 (more);
  Lookout_A/B/C 41/34/42, CrowsNest_A 49, Abseil_A 26, Brokers 34 and the lane and side-street movers as before.

### What was not fixed, and why

- MarchMove_A and Mounted_A at 24.42 m. Moving them to 24.9 m was tried and taken back: the track's own sight test then pushed out every
  MarchMove_A (31 of 31; 5 of 29 before, 7 of 31 now at 24.42 m), towards the shop fronts at 25.5 m. The rule as written is met (24.42 m);
  Mounted_A is still moved by the track 16 times of 16, and where it then stands was not looked at.
- The Plaza's open blocks: the suggested fix (raised pieces instead of the low chess and flowers pieces in the Plaza pockets) does not fit
  the table: pass 3 has no pocket piece in the Plaza; every pocket there is the first pass's, and no free ground spot is known from the table.
- Sweepers_A's colours, the Viaduct, the lanterns and balloons, the brokers' height, the low kerb pieces, the hanging cloth: each needs a
  frame to judge or has no table fix; left as they are.
- Older rules (SM_FarGeese, SM_FarGliders over the road in plan but 95 m above it; SM_CrewSound_City's boom): not pass 3, rule as written met.
- Budgets: 60 of 78 meshes over the 1,500-triangle guide, 78 meshes against the guide of 16, older meshes placed over 60 times: not touched.

### Never checked in this round

- NO FRAME WAS LOOKED AT AFTER THE FIX (the one saved frame, `fixed_L7\L7_t006.png`, was not opened). Whether the roofs now read as one
  clear piece each, whether the hovering drones read, whether the new arena pieces stand free of the dressing and of the lane movers,
  whether the capped rates still read as drumming, boxing and clapping: not seen.
- The roof-spot test is the driver's rule-pair arithmetic on the table's numbers, not a measurement in the game; pieces further apart than
  6.5 m along that still touch, and pieces the track moves, are not covered. Pairs with the first pass and the watchers were not shared out.
- The 2 Hz check is the rules reader's simulation of the generators outside Unreal, rerun; nothing was measured in the game.
- Only scene 1 was started, for 6 seconds of clock. Frame rate, memory, load time, later scenes and the boss arena: not run.
