# Level 1, Chemical Works: population pass 3 (integration)

Branch `claude/people-L1-p3`. Six builders (p3a to p3f) each wrote a generator, meshes and a table block; this note is
the integrator's record of joining them. The integrator did not look at any frame.

## What pass 3 added

67 new meshes in `/Game/People/ChemicalWorks/` (prefix `SM_P3A_` to `SM_P3F_`), six generators
`Content/Python/ft_people_chem_p3a.py` to `_p3f.py`, and six blocks appended to `Source/FullThrottle/FTPeopleL1.inl`
(each wrapped in `{ }`, after the watch block; nothing above them was changed). Builder f's block also places more of
the existing crew kit (`/Game/People/Crew/SM_Crew<Piece>_Work`).

| builder | focus | meshes | rules in block | builder's estimate of pieces (before the track's clearance) |
|---|---|---|---|---|
| p3a | gaps | 11 | 189 | 355 |
| p3b | scenes | 10 | 359 | 359 |
| p3c | movers | 11 | 60 | 346 |
| p3d | heights | 11 | 68 | 374 |
| p3e | far and sky | 12 | 138 | 398 |
| p3f | film shoot | 12 | 122 | 346 |
| all | | 67 | 936 | 2178 |

Triangles, top height and TALL/low are from the make logs (`C:/ftwork/logs/L1p3<letter>_make.log`). The last column is
that mesh's entry in the FTCLEAR line of the one game start: left out of the road / moved off the barrier / kept.
"not listed" means the mesh has no entry: the track prints an entry only for a mesh with at least one piece left out
or moved (FTTrack.cpp, the loop over `Report`), so the count of those meshes is not known from this run. All 67
meshes were loaded by the level (one "Waiting on static mesh ... being ready before playing" line each in run.log).

| mesh | triangles | top m | | left out / moved / kept |
|---|---|---|---|---|
| SM_P3A_Break_A | 2464 | 1.63 | low | 0 / 2 / 21 |
| SM_P3A_Break_B | 2428 | 1.69 | low | 2 / 3 / 25 |
| SM_P3A_Sweepers | 1564 | 1.72 | low | 4 / 5 / 21 |
| SM_P3A_Porters | 1920 | 1.64 | low | 3 / 1 / 24 |
| SM_P3A_Fitters | 2148 | 1.62 | low | 3 / 2 / 23 |
| SM_P3A_Marshals | 1876 | 1.82 | low | 1 / 2 / 20 |
| SM_P3A_Hose | 2200 | 1.74 | low | 2 / 4 / 18 |
| SM_P3A_StairTower | 2240 | 19.55 | TALL | 4 / 0 / 16 |
| SM_P3A_Painters | 2104 | 16.6 | TALL | 2 / 0 / 16 |
| SM_P3A_Mess | 2016 | 13.02 | TALL | 1 / 0 / 16 |
| SM_P3A_Steeplejacks | 1984 | 20.26 | TALL | 2 / 0 / 15 |
| SM_P3B_Decon | 2168 | 16.88 | TALL | 2 / 0 / 24 |
| SM_P3B_FireDrill | 2148 | 16.88 | TALL | 7 / 0 / 21 |
| SM_P3B_Inspection | 2392 | 16.88 | TALL | not listed |
| SM_P3B_Meeting | 2388 | 16.88 | TALL | 5 / 0 / 25 |
| SM_P3B_TugOfWar | 2116 | 16.88 | TALL | 5 / 0 / 23 |
| SM_P3B_Capstan | 2328 | 16.88 | TALL | 3 / 0 / 15 |
| SM_P3B_Briefing | 2448 | 16.88 | TALL | 4 / 0 / 17 |
| SM_P3B_SafeDays | 2212 | 16.88 | TALL | 5 / 0 / 23 |
| SM_P3B_CraneLift | 1732 | 17.74 | TALL | not listed |
| SM_P3B_Band | 2628 | 16.88 | TALL | 4 / 0 / 25 |
| SM_P3C_Walkers_A | 1940 | 18.65 | TALL | 3 / 0 / 20 |
| SM_P3C_Walkers_B | 2200 | 17.15 | TALL | 3 / 0 / 9 |
| SM_P3C_Cyclists_A | 2076 | 18.15 | TALL | 5 / 0 / 23 |
| SM_P3C_Cyclists_B | 2148 | 17.15 | TALL | 3 / 0 / 21 |
| SM_P3C_PipeGang | 2028 | 17.65 | TALL | 2 / 0 / 24 |
| SM_P3C_FireDrill | 1680 | 17.15 | TALL | 3 / 0 / 15 |
| SM_P3C_ManLift | 1940 | 19.32 | TALL | 4 / 0 / 19 |
| SM_P3C_Ropeway | 2024 | 27.2 | TALL | not listed |
| SM_P3C_RailCart | 1768 | 24.12 | TALL | 2 / 0 / 29 |
| SM_P3C_TankPatrol | 1652 | 25.6 | TALL | not listed |
| SM_P3C_KerbBikes | 1312 | 1.76 | low | 3 / 4 / 30 |
| SM_P3D_StackGallery | 2264 | 26.3 | TALL | 1 / 0 / 13 |
| SM_P3D_PipeBridge_A | 2264 | 15.68 | TALL | not listed |
| SM_P3D_PipeBridge_B | 2424 | 13.68 | TALL | 2 / 0 / 24 |
| SM_P3D_CraneBasket | 1552 | 26.1 | TALL | 2 / 0 / 27 |
| SM_P3D_ColumnDecks | 2932 | 24.4 | TALL | 2 / 0 / 17 |
| SM_P3D_SphereCrown | 1976 | 18.48 | TALL | 3 / 0 / 18 |
| SM_P3D_Balconies | 2800 | 21.78 | TALL | 3 / 0 / 23 |
| SM_P3D_BoomLifts | 2064 | 17.38 | TALL | 2 / 0 / 20 |
| SM_P3D_SiloWalk | 2604 | 19.68 | TALL | 1 / 0 / 25 |
| SM_P3D_WaterTower | 2292 | 18.4 | TALL | 1 / 0 / 27 |
| SM_P3D_Hoarding | 2112 | 17.8 | TALL | not listed |
| SM_P3E_Steeplejacks | 1720 | 127.86 | TALL | not listed |
| SM_P3E_FarCrowd_A | 1884 | 80.19 | TALL | not listed |
| SM_P3E_FarCrowd_B | 1932 | 80.19 | TALL | not listed |
| SM_P3E_TowerRim | 1236 | 123.6 | TALL | 3 / 0 / 30 |
| SM_P3E_Viaduct | 1452 | 58.7 | TALL | not listed |
| SM_P3E_Cableway | 1176 | 73.3 | TALL | 1 / 0 / 24 |
| SM_P3E_Balloons_A | 1176 | 140.55 | TALL | 2 / 0 / 45 |
| SM_P3E_Balloons_B | 1272 | 140.55 | TALL | 2 / 0 / 45 |
| SM_P3E_Blimp | 396 | 130.0 | TALL | 3 / 0 / 44 |
| SM_P3E_Kites | 1092 | 121.07 | TALL | not listed |
| SM_P3E_GullTower | 1272 | 155.26 | TALL | 1 / 0 / 30 |
| SM_P3E_Lanterns | 1248 | 160.67 | TALL | 1 / 0 / 28 |
| SM_P3F_SecondUnit_A | 2300 | 5.17 | TALL | 1 / 0 / 11 |
| SM_P3F_SecondUnit_B | 1952 | 5.16 | TALL | 1 / 0 / 10 |
| SM_P3F_DroneTeam_A | 2104 | 15.29 | TALL | 3 / 10 / 20 |
| SM_P3F_DroneTeam_B | 2268 | 14.29 | TALL | 1 / 8 / 16 |
| SM_P3F_StuntRig_A | 2256 | 12.22 | TALL | 1 / 3 / 20 |
| SM_P3F_StuntRig_B | 2028 | 13.43 | TALL | 1 / 0 / 16 |
| SM_P3F_Extras_A | 3152 | 5.99 | TALL | 1 / 0 / 14 |
| SM_P3F_Extras_B | 3032 | 5.99 | TALL | 4 / 0 / 6 |
| SM_P3F_MakeUp | 2040 | 5.11 | TALL | not listed |
| SM_P3F_PressPen | 3136 | 5.18 | TALL | 4 / 1 / 12 |
| SM_P3F_CamTower | 2076 | 14.12 | TALL | 3 / 0 / 15 |
| SM_P3F_LightBalloon | 1880 | 14.83 | TALL | 4 / 0 / 8 |

Sums of the listed entries only (left out / moved / kept): p3a 24 / 19 / 215, p3b 35 / 0 / 173, p3c 28 / 4 / 190, p3d 17 / 0 / 194, p3e 13 / 0 / 246, p3f 24 / 22 / 148.
Not listed (12): SM_P3B_Inspection, SM_P3B_CraneLift, SM_P3C_Ropeway, SM_P3C_TankPatrol, SM_P3D_PipeBridge_A, SM_P3D_Hoarding, SM_P3E_Steeplejacks, SM_P3E_FarCrowd_A, SM_P3E_FarCrowd_B, SM_P3E_Viaduct, SM_P3E_Kites, SM_P3F_MakeUp.

Triangle guide: 57 of the 67 meshes are over the brief's "about 1,500 triangles" (up to 3,152: SM_P3F_Extras_A). Not
reduced: the builders were allowed a rerun only for an error, and the integrator does not redesign.

## FTDRESS before and after

- before (C:/ftwork/logs/p3_prep_L1.txt): `FTDRESS level=1 districts=172 sets=21 pieces placed=10460 of 11406 tried`
- after (C:/ftwork/out/p3/after_L1/run.log): `FTDRESS level=1 districts=242 sets=21 pieces placed=11932 of 13031 tried shadows=6347 of 12611`
- difference: 1,472 more placed, 1,625 more tried, 70 more districts (the blocks' zones). The builders' estimates add up
  to 2,178; why "tried" rose by less than that was not looked into.
- FTCLEAR after: `props kept=13856 (moved off their own barrier 828) left out of the road=1167 pillars left out=50` (no before line was measured).
- Track: `FTTRACK level=1 frames=12792 route=09fbdb9f solid=a2d79366 solidTriangles=950948`, identical to the
  `FTTRACK level=1` line of SourceArt/Jam/validate/track_baseline.txt.
- run.log: no "Fatal error", no "LogFT: Error", no failed load of a /Game/People asset found.

## What was changed, dropped, moved or left out

- Blocks changed: none. All six went in as the builders wrote them (only line ends made the same as the file's).
- Blocks dropped: none. Rules removed: none (every mesh a block names exists, the eight crew kit meshes too).
- Distance rules, checked from the text of every rule (936 rules): no rule places a TALL mesh with x min under 24 m and
  none is under 17 m, so nothing was raised. The eight low meshes (seven of p3a at 24.3 m, SM_P3C_KerbBikes at 24.2 m)
  are the nearest; the nearest TALL rule is p3f's at 27.2 m.
- Generators: none had to be rerun; no Python was edited.
- Compile: `build.ps1 -Tag L1p3_build` succeeded at the first try, no warnings in its log.
- Moved or left out by the track at the start of the level: see the table above (per mesh).

## Not checked

- Frames were NOT looked at by anyone: not by the six builders (no contact sheets, no game run) and not by the
  integrator (the one start saved one frame, L1_t006.png, which was not opened). Shape, pose, colour, readability,
  floating or interpenetrating parts, pieces standing in dressing or in each other, and every movement are unseen.
- The level was started once and left 6 seconds in; nothing was driven. Frame rate and memory with 67 more meshes
  were not measured.
- How many pieces of the 12 unlisted meshes are placed.
- Whether the low pieces' lit parts really stand 24 m or more out (p3a's own arithmetic), the glow and blink rules,
  and the rules about pickups, enemies and the boss: only the x min of each rule was checked.
- Overlaps between the six builders' pieces and with the dressing: each builder chose slots from the tables alone.

## Review and fix round

Two reviewers looked at pass 3 (a rules reader, from the text and a stub simulation of the generators, and a driver, from
about 100 game frames up to 13 km: contact sheet `sheets/L1_p3_review.jpg`). One fixer then made one round of changes.
The fixer looked at no frame: everything below was checked from the table's text, the make log and one start of the level.
Scratch: `C:/ftwork/jobs/L1p3fix/` (`fix.py` makes every table change from the table as it was; `ov_after.txt` is the
reader's overlap count run again).

### Fixed

| finding | what was done |
|---|---|
| rule: the film crew's camera drones circled 25.6 m from the centre line (this level has enemy helicopters) | `ft_people_chem_p3f.py`: the five orbiting drones now hover by bob only, like the docked one; bounds growth 5.6 m to 0.6 m. SM_P3F_DroneTeam_A / _B made again (2104 / 2268 triangles, tops 15.29 / 14.29 m, as before). The track now moves none of them (it moved 10 and 8). |
| rule: a steady light nearer than 24 m (first round's SM_KerbCrew_A / _B, SM_Patrol at 24.1 m, lit parts at 23.7 to 24.0 m) | table: `K` 24.1 to 24.5 m. The generator was not touched. |
| rule: the skein SM_FarFlock_B could cross over the road (90 m out, 15 degrees of yaw, a 300 m straight run) | table: 130 m out, no yaw. |
| looks wrong: the scenes' stages (p3b, 34 m) stood in the movers' galleries (p3c, 35.6 m) and in the film units (p3f, 30.3 to 36.3 m) | (1) the 52 high-way rules of p3c keep their distance and "every" but 31 have a new phase, chosen against the film units, the first rounds and the slots the scenes can use (three districts keep their slot class: tank farm, loading yards, tank farm B). (2) the p3b block was planned again by its own script with one more condition: never a slot and side where a piece of p3a, c, d, e or f stands within 31 to 37 m. 277 scenes instead of 359. By the reader's count: p3b + p3c 75 to 0 expected pairs, p3b + p3f 77 to 0, p3c + p3f 26 to 9. |
| looks wrong: the 24 m lane was booked twice | p3a's low rules: 14 of 78 have a new class of twelve, chosen against the 26 m line; SM_P3C_KerbBikes is placed district by district in the class left over, at 24.0 m (was 24.2); p3f's compact units in tank farm B 27.2 to 28.5 m. p3a + first round 87 to 50 expected pairs, p3a + p3c 19 to 0. |
| weak: p3a's low pieces had 2 cm of margin on the 24 m light line | `LOWX` 24.3 to 24.5 m. |
| weak: the kerb cyclists' one zone covered the wall turns | the zones are now the low lane's 13 districts (no wall turn, mega vat, jump or run-in to the finale). |
| weak: SM_P3E_Lanterns could read as the pickup (an orange lit box that bobs) | `ft_people_chem_p3e.py`: lanterns twice as tall as wide, pale yellow and pink only, glow 0.9 to 0.5. Made again (1248 triangles). |
| looks wrong: lanterns, gulls, kites over the boss in the finale | export terminal (9.46 to 12.72 km): the lanterns and the kites are left out, the gull towers stand 250 to 320 m out (were 190 to 260). |
| weak: the kites too loud | every other kites rule 150-215 m to 215-270 m. Not scaled, not thinned. |
| weak: drone meshes' grown bounds | went with the drone fix. |
| weak: macro `TALL_AT` | now `P3A_AT`. |
| weak: nobody readable in the flare avenue | SM_P3A_StairTower at 44 m, every sixth slot a side, never a derrick slot (46 m, s % 3 == 1). |

### Not fixed

- "Blink" on the first round's pieces (clapping arms and dogs at 2.4 to 2.6 Hz, crows at 2.0 Hz, carrying lit()'s steady
  0.06 light, 24 to 26 m out): the numbers are as the reader says (`ft_people_chem.py` lines 65, 336, 352, 410, 458). The
  light itself is constant, and POPULATION_BRIEF's light rule is about lights that blink, chase or pulse and a flicker
  faster than three a second; by that text this is not a breach. A fix is two numbers in `ft_people_chem.py` (rate 1.9)
  and eight meshes made again; that file was not the fixer's to edit, and moving ten first-round rule groups (about 560
  placed pieces) past 30 m would be a redesign. Left for the owner of the first round.
- p3d's hoarding and boom lifts sharing slots with the far block's crows and yard dogs (27 expected pairs): the driver
  names the crows in front of the white hoarding at 4.5 km as a pairing to leave alone. Nothing changed.
- p3c + p3f in the tank farm (9 expected pairs left, 1.40 to 2.60 km): both may use only the same one slot class in
  three there (watch towers and gantries take the others).
- p3a's low pieces in front of the first round's 26 m line: 50 expected pairs left; on the right side of tank farm B and
  reactor two every class the kerb line leaves free has a 26 m piece behind it.
- With `K` and `LOWX` at 24.5 m the first round's kerb crews and p3a's low pieces stand 0.4 and 0.2 m deeper into what is
  behind them (the reader's count, first round against itself: 83 to 126 expected pairs). The light rule was put first.
- p3a's tall pieces (44 m) against p3f's units at 41.3 m in tank farm B: 21 expected pairs, not in a finding, not touched.
- The white boxy shape with a pink box at the gate (0.4 to 0.5 km, right): not identified, needs a single-mesh run.
- Low pieces hidden behind the barrier (plinths, thinning): needs new geometry or a redesign.
- Waste run 8.1 to 8.3 km: the p3 zones start at 8.12 km because the third wall turn (banked 58 degrees) ends at 8.11.
- Movement of the pieces: needs a stationary camera.
- Triangle and placement budgets: reported by the reader, nothing asked.

### After the round

- Make log `C:/ftwork/logs/L1p3fix_make.log`: 3 pieces built, 0 failed. Build `L1p3_fix_build`: succeeded.
- One start (`C:/ftwork/out/p3/fixed_L1/run.log`): `FTTRACK level=1 frames=12792 route=09fbdb9f solid=a2d79366 solidTriangles=950948`,
  identical to the baseline's level 1 line. `FTDRESS level=1 districts=255 sets=21 pieces placed=11878 of 12963 tried shadows=6297 of 12557`
  (before the round: 11932 of 13031). `props kept=13802 (moved off their own barrier 806) left out of the road=1153 pillars left out=50`.
  No "Fatal error", no "LogFT: Error", no failed /Game/People load; 67 SM_P3 meshes loaded.

### Never checked

- No frame was looked at after the round (the start saved one, L1_t006.png, unopened). Whether the stages, galleries and
  film units now stand clear on screen, how the hovering drones, the new lanterns, the further kites and the flare
  avenue's stair towers look, and whether a stair tower stands in dressing other than the derricks.
- The overlap counts are the reader's model (same slot and side, distance bands from stub geometry; the crew kit's
  footprint is a guess of 10 by 5 m): not neighbouring slots, not the dressing.
- The new p3c phases against the dressing: p3c's own notes give a slot class only for the three districts that kept it.
- The tables' estimates in the blocks' header comments (p3c's by mesh) are the builders' and were not recounted.
- 13 km to the end of the road was not seen by anyone.
