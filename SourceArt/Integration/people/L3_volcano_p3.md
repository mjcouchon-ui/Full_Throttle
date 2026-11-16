# Volcano (level index 3): population pass 3

Integrated on branch `claude/people-L3-p3`. Six builders (p3a to p3f) each wrote a generator
(`Content/Python/ft_people_volcano_p3<letter>.py`), meshes (`/Game/People/Volcano/SM_P3<LETTER>_*`) and a table block.
The integrator joined the blocks to `Source/FullThrottle/FTPeopleL3.inl`, compiled, and started the level once.

## What pass 3 added

65 new meshes, every one logged TALL by its generator's make log (`C:/ftwork/logs/L3p3<letter>_make.log`).
Triangles and top height are from those logs; "estimated placed" is each builder's own figure, not measured.

| builder | focus | meshes (name, triangles / top height) | estimated placed |
|---|---|---|---|
| p3a | gaps | 11: Sample_A 1996 tri / 3.21 m; Sample_B 1900 tri / 5.0 m; Rest_A 1744 tri / 4.16 m; Rest_B 1806 tri / 3.48 m; Pass_A 1682 tri / 3.25 m; Pass_B 2152 tri / 3.67 m; Mend_A 1956 tri / 3.35 m; Mend_B 2012 tri / 3.23 m; Dike 2956 tri / 4.43 m; Knoll_A 2214 tri / 7.01 m; Knoll_B 2344 tri / 6.68 m | 298 |
| p3b | scenes | 10: Wedding 2918 tri / 5.82 m; Kitchen 2756 tri / 5.18 m; Shrine 2346 tri / 6.05 m; Rescue 2388 tri / 5.72 m; Sample 2584 tri / 4.84 m; School 3154 tri / 5.6 m; HotSpring 2302 tri / 4.43 m; Tour 3126 tri / 5.2 m; Band 2626 tri / 5.7 m; Briefing 2438 tri / 4.75 m | 390 |
| p3c | movers | 11: Runners_A 1836 tri / 4.07 m; Runners_B 1830 tri / 4.16 m; Cyclists 1928 tri / 3.99 m; HoseRun 2516 tri / 3.63 m; MuleTrain 2536 tri / 3.21 m; OreTubs 2244 tri / 3.01 m; Handcar 2064 tri / 3.43 m; GoatRun 1968 tri / 3.23 m; Ropeway 1758 tri / 11.88 m; Whim 1678 tri / 5.64 m; Skimmers 658 tri / 8.97 m | 427 |
| p3d | heights | 11: Lookout_A 2036 tri / 9.0 m; Lookout_B 2348 tri / 9.3 m; Crow 810 tri / 10.39 m; Derrick_A 2596 tri / 22.5 m; Derrick_B 2220 tri / 22.5 m; Crag_A 2164 tri / 19.84 m; Crag_B 2226 tri / 19.47 m; RopeBridge 2946 tri / 18.9 m; Headframe 2156 tri / 18.7 m; Arch 1804 tri / 18.76 m; Signal 1352 tri / 21.1 m | 352 |
| p3e | far and sky | 10: Balloons_A 2022 tri / 112.26 m; Balloons_B 2012 tri / 103.8 m; Airship 964 tri / 94.7 m; Gliders_A 864 tri / 64.17 m; Gliders_B 1376 tri / 52.94 m; Kites 2140 tri / 71.45 m; Climbers 1884 tri / 85.91 m; FarCrowd_A 2284 tri / 45.0 m; FarCrowd_B 2446 tri / 37.22 m; Cableway 1680 tri / 53.05 m | 313 |
| p3f | film shoot | 12: SecondUnit_A 2896 tri / 5.1 m; SecondUnit_B 2664 tri / 6.72 m; DroneTeam_A 3024 tri / 7.95 m; DroneTeam_B 2668 tri / 7.63 m; StuntRig_A 2404 tri / 7.65 m; StuntRig_B 2580 tri / 8.41 m; Extras_A 4238 tri / 4.8 m; Extras_B 3334 tri / 4.8 m; MakeUp 3180 tri / 4.8 m; PressPen 5156 tri / 4.75 m; CoolTent 3268 tri / 5.1 m; Rostrum 648 tri / 5.8 m | 355 |

Sum of the builders' estimates: 2135. Block f also places ten pieces of the existing crew kit
(`/Game/People/Crew/SM_Crew<Piece>_Heat`: Camera, Sound, Village, Lights, Stunt, ApplauseA, ApplauseB, Catering, Dolly,
Crane) on its own rostrum, through five Sets `PplP3FKitA..E`; block c has one Set `PplP3CUphill`.

## Measured (one start of the level, `C:/ftwork/out/p3/after_L3/run.log`)

- Compile: `build.ps1 -Tag L3p3_build` exit 0, "Result: Succeeded", first try (`C:/ftwork/logs/L3p3_build.log`).
- Track: `FTTRACK level=3 frames=12497 route=506786e7 solid=eead12ba solidTriangles=900592`, identical to the level-3
  line of `SourceArt/Jam/validate/track_baseline.txt`.
- FTDRESS before (`C:/ftwork/logs/p3_prep_L3.txt`): `districts=256 sets=23 pieces placed=9828 of 9883 tried`.
- FTDRESS after: `districts=474 sets=29 pieces placed=11810 of 11865 tried shadows=3380 of 11810`.
  So 1,982 more pieces tried and 1,982 more placed; the number not placed is 55 before and after.
- FTCLEAR (last line): `props kept=14184 (moved off their own barrier 455) left out of the road=55`. Its per-mesh list
  names only four pass-3 meshes, all of block c's catwalk, and every placed one was moved out from the barrier:
  `SM_P3C_Runners_A:-0/~38/38 SM_P3C_Runners_B:-0/~40/40 SM_P3C_Cyclists:-0/~42/42 SM_P3C_HoseRun:-0/~8/8`.
  The other 61 pass-3 meshes are not in that list (in this run the list held only meshes with a left-out or moved
  count), so their placed counts were not measured one by one.
- All 65 `SM_P3*` meshes appear in the log as loaded for play ("Waiting on static mesh ... being ready before playing").
- No "Fatal error", no "LogFT: Error", no failed load of a `/Game/People` asset in the log.

## Changed, dropped, moved, left out

- Blocks in: a, b, c, d, e, f, each appended wrapped in `{ }` after the existing table, text unchanged (line ends
  made CRLF like the file). Nothing already in the file was changed. Blocks dropped: none.
- Rule check from the text: every mesh a block names exists as a `.uasset` (65 new meshes, 10 crew kit pieces). No rule
  has x min under 24 m (smallest: a 42.7 m, b 32.3 m, c 25.2 m, d 33.1 m, e 150 m, f 28.6 m), so no distance was raised.
- No generator was rerun and no generator was edited. No compile fix was needed.
- Moved by the track: all 128 placed catwalk pieces of block c (written at 25.2 to 25.4 m, 20 m long) were pushed out
  from the barrier. Where they ended up was not measured; they may now meet the main cast's trestles at 28.6 m.
- Left out by the track: none of the pass-3 pieces (left out stays 55, all dressing).

## Known departures the builders reported (not corrected here)

- Triangles: most pieces are over the 1,500 guide (worst: SM_P3F_PressPen 5,156, SM_P3F_Extras_A 4,238).
- Block a has no low pieces at 17 to 24 m; its bands are 42.7 to 63 m and 88 to 90 m. Eleven of its pieces are Far rules.
- Block d's right-hand band is 70.8 to 71.2 m; block f's second row is 34.4 / 35.6 m, in the fumaroles' band.
- Blocks b, c, d and f all use the near stretches between 25 and 39 m and did not see each other's blocks: overlaps
  between them, and with fumaroles and cable reels, are possible.
- Every pass-3 colour carries a small steady glow (0.035 to 0.07; block b's ladles 0.5); nothing is nearer than 25.2 m as written.

## Not checked

- Frames were NOT looked at by anyone: not by the six builders, not by the integrator. The one frame the start script
  saved (`L3_t006.png`) was not opened. How any piece looks, reads, moves, floats, overlaps or hides something is unknown.
- Per-mesh placed counts for 61 of the 65 meshes, and whether any mesh is placed more than 60 times.
- Where the pushed-out catwalk pieces stand, and whether any pass-3 piece stands in lava, on a jump's run-up or landing,
  or in another piece.
- Frame rate and memory with 1,982 more pieces; only the first 6 seconds of the level ran (scene 1).
- The rules about glow, blinking, pickups, the boss and travel paths were not checked by sight; only x min was
  checked, from the text.
- Shadows: the builders' claim that Far-first meshes get no shadow was not checked.

## Review and fix round (16 Nov 2026)

Two reviews (a rules reader working from the text and a stub run of the generators; a driver with 13 runs of the game,
104 stills, contact sheet `sheets/L3_p3_review.jpg`, taken BEFORE this fix), then one fix round. Nobody has looked at
frames of the fixed level: only its start (6 seconds, one frame) ran.

### What the reviewers found, and what was done

| finding | severity | done |
|---|---|---|
| SM_P3E_Airship reads as a missile flying along the road, in the boss fight too | rule broken | All 23 Airship rules removed: the mesh is no longer placed. The generator was not changed. |
| Balloons and kites clutter the sky straight ahead on Engine 13's road (a warning ring drawn round a balloon) | rule broken | No Balloons_A, Balloons_B or Kites from 8.17 km on. Before that the balloons stand every second cycle (every 68) at 300 to 345 m instead of 235 to 300 m; no Kites in the two lava jumps zones. Gliders_A / _B every second cycle from 8.34 km on. |
| Hovering drones 25 to 26 m from a road with gunships (DroneTeam_A / _B at 28.6 m) | rule broken | The ten first-row rules now place SecondUnit_A (for A) and StuntRig_A (for B); the second row went altogether (below). No drone team is placed any more. |
| Dog 3.0 Hz, goat 3.5 Hz, a clapping figure 2.4 Hz on faintly lit parts nearer than 30 m | rule broken (by the letter) | Rates capped at 2.0 in `ft_people_volcano_p3c.py` (dog, running_goat) and `ft_people_volcano_p3f.py` (stunt_rig_b); Runners_A, Runners_B, StuntRig_B remade. |
| All 128 catwalk pieces pushed out to about 27 m by the sight test, into the pieces behind | looks wrong | The four catwalk meshes are saved with bounds grown 8 cm across the road instead of 60 (`CATWALK` in the p3c generator) and remade. After: none of them is in the FTCLEAR list any more and "moved off their own barrier" fell from 455 to 327 (128 fewer). |
| Blocks stand in each other and in the first rounds' pieces (b, d, f; a against c) | looks wrong | Block f's second row (34.4 / 35.6 m, 33 rules) removed. Block b's left lane narrowed to 32.5 to 32.6 m. Then, by footprints (the reviewer's stub extents, same slot and side, more than 0.5 m of overlap across the road), the yielding piece's slot was taken out: b yields to rounds 1 and 2 and to f (81 slots, 18 rules gone), d's lookouts to all of those (101 slots, 18 rules gone), a's knolls to c (12 slots), c's far movers to round 1 (11 slots). A rule that lost some slots is rewritten as single-slot rules (`every 1000`, phase 1000 - slot). After: 0 such pairs over 0.5 m, 75 over 0.3 m (b's stage edge against a trestle or deck edge). Moving b's right lane to 40 m was tried on paper and not done: it meets more watch decks there (63 against 18). |
| Eleven one-slot Far zones of block a cut with a 45.0 m slot (seven empty, four a slot early, perhaps floating) | looks wrong | The eleven zones removed. |
| Tall scaffolds read as black lattice | looks wrong | Lookout_A, Lookout_B and Crow given the level's 0.035 steady lift (they stand 33 m out) and remade. Colours not changed. |
| Skimmers everywhere on both sides | weak | One side a district (alternating), every second slot: 48 slots became 11. |

Slot counts of pass 3 before the track's own exclusions (the reviewer's method): 2,222 before, 1,778 after
(a 255, b 365, c 375, d 317, e 182, f 284). Most of any pass-3 mesh: 48 (Runners_B).

### Measured after the fix (`C:/ftwork/out/p3/fixed_L3/run.log`)

- Meshes remade (`C:/ftwork/logs/L3p3fix_make.log`, 8 built, 0 failed, triangles and tops as before): SM_P3C_Runners_A,
  Runners_B, Cyclists, HoseRun; SM_P3F_StuntRig_B; SM_P3D_Lookout_A, Lookout_B, Crow.
- Compile: `build.ps1 -Tag L3p3_fix_build` exit 0, "Result: Succeeded".
- `FTTRACK level=3 frames=12497 route=506786e7 solid=eead12ba solidTriangles=900592`: equal to the baseline's level-3 line.
- `FTDRESS level=3 districts=465 sets=29 pieces placed=11436 of 11491 tried shadows=3239 of 11436` (before the fix:
  11810 of 11865, shadows 3380). 374 fewer pieces; not placed stays 55.
- `FTCLEAR level=3 props kept=13810 (moved off their own barrier 327) left out of the road=55`; no pass-3 mesh in its list.
- 62 `SM_P3*` meshes named in the log (65 less Airship, DroneTeam_A, DroneTeam_B). No "Fatal error", no "LogFT: Error",
  no failed `/Game/People` load. One engine ensure (DoubleFloat.cpp line 19) before the level started; the driver saw
  the same one in first-pass logs.

### Not fixed

- The slot length of 45.0 m in the scratch scripts of blocks a to e (zone edges up to three slots off at the end of the
  level): would mean regenerating five blocks, which is a new table, not a fix. Only block a's Far zones were dealt with.
- The dark tower by the grandstand at the start (0.04 to 0.28 km, left): pass 3 has no near rule there (only block a's
  decks at 54 m and knolls at 88 m); by the frame it is the first round's shoot. Not pass 3, not touched.
- Airship, DroneTeam_A and DroneTeam_B still exist as meshes and in their generators but are placed nowhere; the
  airship's shape (pointed nose, fins, boom) was not redrawn.
- The scaffolds' timber and steel were not given a lighter colour (lift only). Block f's towers already carried the
  lift and were not changed.
- Nothing was added to the empty stretches at the loops, corkscrews and wall rides (the driver: no fault of placement).
- Sky pieces' glow of 0.25 (burners 1.2) and the gliders coming to about 100 m: left for a frame to judge.
- The 146 rules that match no slot, the meshes over 3,000 triangles, the older meshes placed more than 60 times.
- The 75 remaining edge overlaps of 0.3 to 0.5 m (b's stages against trestles and decks).
- The "car drawn as four wheels after a jump" seen by the driver: not population.

### Never checked

- No frame of the fixed level was judged: whether the catwalks now read in front of the trestles, whether the lifted
  lookouts read, whether the remaining balloons at 300 m and more sit well, whether any overlap remains to the eye.
- Overlaps were computed from the reviewer's stub extents with the catwalk at its written 25.2 m, across the road only;
  along the road every pair in a slot was treated as meeting. The dressing's pieces (fumaroles, cable reels) were not
  in that test.
- Movement of block c's movers (the driver could not confirm it either); the capped rates in play.
- Scenes beyond the first 6 seconds after the fix; frame rate; memory; per-mesh placed counts (slot counts only).
- Whether any piece stands in a lava lake or beside a jump's landing beyond the track's own exclusion.
- Removing block a's Far rules means its bank meshes' first placed piece is no longer a Far one, so the track may now
  give those meshes shadows (the builder's trick against the shadow queue); only the total (3239 of 11436) was read.
