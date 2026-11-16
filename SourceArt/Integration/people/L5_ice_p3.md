# Level 5, Ice Mountain: population pass 3

Six builders (p3a to p3f) each wrote a generator, a set of meshes and a table block. The integrator joined the blocks to
`Source/FullThrottle/FTPeopleL5.inl`, compiled, and started the level once. Nobody looked at a frame: see "Not checked".

## What pass 3 added

73 new meshes in `/Game/People/IceMountain` (prefix `SM_P3A_` to `SM_P3F_`), six generators
`Content/Python/ft_people_ice_p3a.py` to `_p3f.py`, and six blocks at the end of the table (1,232 rules, 112 sets).
Triangles, top height and TALL/low are from the make logs (`C:\ftwork\logs\L5p3<letter>_make.log`); "estimate" is the
builder's own count of its block, not a measurement.

| builder | focus | meshes | where its block puts them | estimate |
|---|---|---|---|---|
| p3a | gaps | 13 | three low pieces at 23 m; tall pieces at 46 m (ridges), 57 and 63 m (resorts), 30 m (glaciers) | 300 |
| p3b | scenes | 11 | story groups at 31.5, 38, 44 and 51 m; 57 to 58 m by Far rules under loops and corkscrews | 355 |
| p3c | movers | 13 | travelling lines at 38.5, 64, 67, 77 and 110 m; geese 80 to 130 m | 461 |
| p3d | heights | 13 | structures with people on them at 54 to 56 and 65 to 67 m; 86 to 88 m under loops and corkscrews | 418 |
| p3e | far and sky | 12 | Far rules only, 150 to about 300 m: balloons, gliders, birds, far crowds, convoys | 393 |
| p3f | film shoot | 11 (plus 9 pieces of the crew kit, wardrobe Snow) | units in clusters at 30, 33 and 46 m, scale 1.3 | 353 |
| sum | | 73 | | 2,280 |

### p3a, gaps: 13 meshes, builder's estimate 300 placed

| mesh | triangles | top m | log |
|---|---|---|---|
| SM_P3A_SnowSeat_A | 1796 | 1.11 | low |
| SM_P3A_SledKids_A | 1628 | 1.11 | low |
| SM_P3A_Huskies_A | 1632 | 1.12 | low |
| SM_P3A_PisteCrew_A | 1730 | 4.51 | TALL |
| SM_P3A_PisteCrew_B | 1766 | 4.59 | TALL |
| SM_P3A_SkiSchool_A | 2318 | 3.33 | TALL |
| SM_P3A_Hikers_A | 2318 | 4.5 | TALL |
| SM_P3A_Hikers_B | 2346 | 4.6 | TALL |
| SM_P3A_Rescue_A | 1560 | 4.9 | TALL |
| SM_P3A_HotDrinks_A | 2352 | 3.61 | TALL |
| SM_P3A_Camp_A | 1606 | 3.62 | TALL |
| SM_P3A_Surveyors_A | 1444 | 5.94 | TALL |
| SM_P3A_Igloo_A | 1690 | 4.04 | TALL |

### p3b, scenes: 11 meshes, builder's estimate 355 placed

| mesh | triangles | top m | log |
|---|---|---|---|
| SM_P3B_Rescue_A | 2064 | 7.0 | TALL |
| SM_P3B_IceSculpt_A | 2094 | 5.76 | TALL |
| SM_P3B_Wedding_A | 3142 | 5.24 | TALL |
| SM_P3B_Curling_A | 2656 | 7.26 | TALL |
| SM_P3B_Alphorns_A | 2368 | 8.1 | TALL |
| SM_P3B_SnowFort_A | 2184 | 5.4 | TALL |
| SM_P3B_BaseCamp_A | 2338 | 7.1 | TALL |
| SM_P3B_Podium_A | 2480 | 7.64 | TALL |
| SM_P3B_IceFishing_A | 2548 | 4.58 | TALL |
| SM_P3B_IceCore_A | 2278 | 7.7 | TALL |
| SM_P3B_PondHockey_A | 2492 | 6.33 | TALL |

### p3c, movers: 13 meshes, builder's estimate 461 placed

| mesh | triangles | top m | log |
|---|---|---|---|
| SM_P3C_SkiTrail_A | 1828 | 4.37 | TALL |
| SM_P3C_SkiTrail_B | 1934 | 4.37 | TALL |
| SM_P3C_Snowshoe_A | 2002 | 4.37 | TALL |
| SM_P3C_FatBikes_A | 1668 | 4.37 | TALL |
| SM_P3C_Skidoos_A | 1484 | 4.47 | TALL |
| SM_P3C_Skidoos_B | 1472 | 4.47 | TALL |
| SM_P3C_Sleigh_A | 1742 | 4.47 | TALL |
| SM_P3C_Skaters_A | 1956 | 5.2 | TALL |
| SM_P3C_Toboggan_A | 2256 | 9.9 | TALL |
| SM_P3C_TobogganL_A | 2262 | 9.9 | TALL |
| SM_P3C_Herd_A | 1774 | 4.36 | TALL |
| SM_P3C_Kites_A | 792 | 13.39 | TALL |
| SM_P3C_Geese_A | 846 | 25.23 | TALL |

### p3d, heights: 13 meshes, builder's estimate 418 placed

| mesh | triangles | top m | log |
|---|---|---|---|
| SM_P3D_Lookout_A | 1276 | 16.46 | TALL |
| SM_P3D_Lookout_B | 1280 | 16.25 | TALL |
| SM_P3D_Bridge_A | 1696 | 13.98 | TALL |
| SM_P3D_Bridge_B | 1650 | 14.29 | TALL |
| SM_P3D_Balcony_A | 1750 | 17.0 | TALL |
| SM_P3D_Balcony_B | 1808 | 17.0 | TALL |
| SM_P3D_Crag_A | 1188 | 17.54 | TALL |
| SM_P3D_Crag_B | 1250 | 18.21 | TALL |
| SM_P3D_Judges_A | 1370 | 19.67 | TALL |
| SM_P3D_Pylon_A | 1292 | 12.28 | TALL |
| SM_P3D_Dam_A | 1734 | 14.04 | TALL |
| SM_P3D_HeliDeck_A | 1332 | 9.44 | TALL |
| SM_P3D_IceArch_A | 1196 | 18.27 | TALL |

### p3e, far and sky: 12 meshes, builder's estimate 393 placed

| mesh | triangles | top m | log |
|---|---|---|---|
| SM_P3E_Balloons_A | 1428 | 103.7 | TALL |
| SM_P3E_Balloons_B | 1252 | 110.22 | TALL |
| SM_P3E_HangGliders_A | 720 | 82.61 | TALL |
| SM_P3E_Geese_A | 1316 | 59.64 | TALL |
| SM_P3E_Choughs_A | 1128 | 48.41 | TALL |
| SM_P3E_SnowKites_A | 996 | 39.09 | TALL |
| SM_P3E_SnowKites_B | 740 | 34.59 | TALL |
| SM_P3E_FarCrowd_A | 1584 | 33.4 | TALL |
| SM_P3E_FarCrowd_B | 1884 | 26.0 | TALL |
| SM_P3E_SkidooTrain_A | 1056 | 21.6 | TALL |
| SM_P3E_Skaters_A | 1256 | 14.79 | TALL |
| SM_P3E_RidgeTrek_A | 1212 | 33.0 | TALL |

### p3f, film shoot: 11 meshes, builder's estimate 353 placed

| mesh | triangles | top m | log |
|---|---|---|---|
| SM_P3F_SecondUnit_A | 2566 | 4.14 | TALL |
| SM_P3F_SecondUnit_B | 2106 | 3.58 | TALL |
| SM_P3F_DroneTeam_A | 2490 | 5.76 | TALL |
| SM_P3F_DroneTeam_B | 2512 | 6.95 | TALL |
| SM_P3F_StuntRig | 2274 | 6.1 | TALL |
| SM_P3F_Extras_A | 2708 | 2.76 | TALL |
| SM_P3F_Extras_B | 2412 | 1.91 | low |
| SM_P3F_MakeUp | 2260 | 2.59 | TALL |
| SM_P3F_PressPen | 3748 | 2.47 | TALL |
| SM_P3F_CableCam | 2298 | 6.4 | TALL |
| SM_P3F_WindMachine | 2002 | 3.16 | TALL |
50 of the 73 meshes are over the 1,500-triangle guide (the heaviest: SM_P3F_PressPen 3,748, SM_P3B_Wedding_A 3,142).
None was slimmed.

## Measured (one start of the level, `C:\ftwork\out\p3\after_L5\run.log`)

- Compile: succeeded at the first try, no change to any block (`C:\ftwork\logs\L5p3_build.log`).
- Track: `FTTRACK level=5 frames=12415 route=b2fc812a solid=9cada242 solidTriangles=931996`, identical to the line in
  `SourceArt/Jam/validate/track_baseline.txt`.
- FTDRESS before: `districts=234 sets=59 pieces placed=9614 of 9642 tried` (`C:\ftwork\logs\p3_prep_L5.txt`).
- FTDRESS after: `districts=724 sets=171 pieces placed=11896 of 11924 tried shadows=3740 of 11971`.
  So 2,282 more pieces tried and 2,282 more placed; the 28 left out are the same number as before.
- FTCLEAR after: `props kept=14703 (moved off their own barrier 778) left out of the road=28`; before: kept 12421,
  moved 778, left out 28. The line lists a mesh only when some of it were left out or moved, and it has no `SM_P3`
  entry: no pass 3 piece was left out or pushed out from the barrier. For the same reason the log gives no placed
  count per pass 3 mesh.
- Crew kit pieces that p3f also places (they were in the list already): SM_CrewCamera_Snow 25 to 52 placed,
  SM_CrewVillage_Snow 3 to 15, SM_CrewStunt_Snow 17 to 28, SM_CrewApplauseA_Snow 18 to 30, SM_CrewApplauseB_Snow 20 to 39;
  their moved counts did not change.
- All 73 pass 3 meshes appear in the log as loaded static meshes. No "Fatal error", no "LogFT: Error", no failed load of
  a /Game/People asset.

## Changed, dropped, moved, left out

- Nothing was changed in any block, no block and no rule was dropped, no generator was rerun.
- Checked from the text of the blocks: every mesh named exists as a .uasset; every Site names a set of its block; no
  rule puts a TALL mesh nearer than 24 m (nearest tall anchor 30 m) or anything nearer than 17 m (nearest: the three
  low pieces of p3a at 23 m). No distance had to be raised.
- Left as the builders reported them, not acted on: the triangle counts above the guide; p3a has nothing in the
  avalanche chute (2.43 to 2.96 km), under loops, corkscrews and jumps, or in the first 0.26 km; p3b has only 3 Site
  pieces from 2.43 to 4.02 km; p3f has nothing from about 1.99 to 4.48 km on one side; p3a's SM_P3A_Surveyors_A and
  p3b's SM_P3B_IceCore_A carry a weather mast that turns continuously (nearest placement 30 m and 31.5 m); pieces
  outside the builders' preferred bands (p3a 63 m, p3b 51 and 57 m, p3d 86 to 88 m).

## Not checked

- Frames were NOT looked at by anyone: not by the six builders (no contact sheets, no game run) and not by the
  integrator (the one frame the start script saves, `L5_t006.png`, was not opened). So nothing is known about how any
  piece looks, reads, moves, stands on its ground, floats beside a banked bend, or runs into another piece.
- Overlap between the six blocks, and with the dressing and the earlier people passes, was only estimated by each
  builder's own scratch tool against the two existing tables; the builders did not read each other's blocks.
- The placed count per pass 3 mesh (the cap of 60 a mesh) is the builders' estimate only.
- Whether any piece can be taken for a pickup, an enemy or the boss; the 1.3-scale drones of p3f at about 31 m.
- Frame rate: 2,282 more pieces, 393 of them far pieces that always move, 50 meshes over the triangle guide.
- Only the start of the level was run (6 seconds of game time); later districts and the boss were not driven.

## Review and fix round (16 Nov 2026)

Two reviewers looked at pass 3 after the integration: a rules reader (text only: the table, the generators, the make
logs, a replay of the placement loop) and a driver (10 drives of the real game, 76 stills and 48 film frames). One
fixer then made one round of changes, compiled, and started the level once. The fixer drove nothing: what the fixes
look like in the game is under "Never checked" below. The driver's contact sheet, `sheets/L5_p3_review.jpg`, shows the
level BEFORE this round.

### What the reviewers found, and what was done

| # | who | finding | done |
|---|---|---|---|
| 1 | rules (rule broken) | SM_P3B_SnowFort_A had six snowballs in flight, beside the road of a boss who throws snowballs | The loop that drew them is out of `snowfort()`; the mesh was regenerated (2,184 to 2,112 triangles). The stock on the ground and the throwing arms stay. |
| 2 | driver (rule broken) | The black circling birds (SM_P3E_Choughs_A, 150 to 200 m) have the size and tone of the enemy chopper and circled round it during the CHOPPER! warning (seen at 3.0 km) | The mesh was regenerated with yellow bodies and lime wings, and its four rules are out of the two districts the driver named (the super pipe 2.96 to 4.02 km, the whiteout 5.65 to 7.3 km). |
| 3 | rules (looks wrong) | Pieces of different blocks on the same slot and side at nearly the same distance (141 pairs by the reader's replay) | See "The same-spot pairs" below: 163 possible pairs by the fixer's replay before, 3 after. |
| 4 | rules (looks wrong) | A blue snowmobile with a white bonnet in SM_P3C_Skidoos_A: the ICE PATROL bikes' colours | That hull is violet; the mesh was regenerated. |
| 5 | rules (looks wrong) | Camera drones 30 to 38 m from the road (the brief: never near the road) | All 15 sites at 30 or 33 m whose set had a drone now name a set of the same side without one (DroneA to Queue, DroneB to Hold, Rig to Lens or Cable by turns). Drones remain only in sites 46 m out and more. |
| 6 | rules (weak) | An orange ball the size of the missile pickup bobbing over SM_P3B_IceCore_A | The balloon is violet; the mesh was regenerated. |
| 7 | driver (looks wrong) | A p3a low piece sunk into the snow at its uphill end (9.0 km right): the hill falls 7 in 100 and the piece stood level on z = 0 | The three low meshes (SnowSeat, SledKids, Huskies) now stand on a 40 cm plinth of snow footed 1.2 m into the ground (`on_plinth` in ft_people_ice_p3a.py); regenerated, tops 1.51 to 1.52 m, still "low" in the log. |
| 8 | driver (weak) | The low pieces in the first half pipe are hidden behind the pipe wall | The zone "People p3a: low, the half pipe" (four rules) is out. |
| 9 | driver (looks wrong) | A rescue terrace runs into the blue ice wall of the first-pass ice climb | On the glaciers p3b wrote scenes 38 m out on slots where the first pass has a climb 31 m out. Every pass 3 piece whose box overlapped a climb's on its slot was moved: 29 to the next slot, 2 out to 44 or 51 m. |
| 10 | driver (looks wrong) | Too many pieces on the same slots on the right, 0.6 to 1.0 and 1.6 to 1.8 km | p3d fires on every fourth slot a side instead of every second from 0 to 2.43 km; the p3b/p3f pairs there were separated (finding 3). |
| 11 | driver (looks wrong) | A group against the foot of the gondola's pylon at 0.23 km | The two pass 3 sites on the gondola's slot (slot 5) moved out past 50 m: the wedding (right) 44 to 51 m, the make-up unit (left) 30 to 52 m. No pass 3 site stands nearer than 50 m on the other two gondola slots (78, 184). |
| 12 | driver (weak) | Stretches with nobody in sight (inside the pipes, the chute) | Far rules of SM_P3E_Balloons_A (every eighth slot a side) and SM_P3E_SnowKites_A (every sixteenth) added to the chute and the super pipe. Nothing was added on the ground. |
| 13 | driver (weak) | A p3d structure hidden behind the village street's chalet rows | The p3d valley zone is written as four zones that leave out 7.4 to 7.5, 8.0 to 8.02 and 8.22 to 8.3 km. |
| 14 | rules (weak) | Sets never used | PplP3DBalconyAL, PplP3DBalconyBR and PplP3DJudgesAL are out. |

### The same-spot pairs

Scratch tools in `C:\ftwork\jobs\L5p3fix\` (pl.py, fixA.py, fixB2.py, fin.py; the change list is `B.log`, copied to
`C:\ftwork\out\L5p3fix\`) replay where every Scatter and Site rule of both tables fires, with every chance taken
as 1 (so a p3d rule at chance 0.75 counts on every slot it could use). "Same spot" is the reader's test: two pieces of
different blocks on one slot and side whose centres are closer than half the smaller piece along and out.

- Before: 163 such pairs (p3c/p3d 57, p3b/p3f 44, p3b/p3c 18, p3a/p3f 13, p3a/p3d 13, p3a/p3b 11, p3a/p3c 4, first pass/p3d 3).
- After: 3 (all three a first-pass dog sled 65 m out with a p3d structure: see "Not fixed").
- How: a one-slot Site (p3a, p3b, p3f) was moved to the next slot at the same distance (only pieces written 46 m out
  or more, p3b from 38 m: nearer ones were written only where the road is not banked) or further out on its own slot;
  a repeating p3c rule got another phase. A new place was taken only if the piece's box (logged size, turned by the
  set's yaw, 1.5 m of overlap let pass) is clear of every other people piece on that slot and side and 1 m clear of
  every dressing anchor there; never onto a jump (2.10 to 2.50, 4.83 to 5.21, 7.99 to 8.375 km) and never under a
  gondola nearer than 50 m.
- Counts: 40 sites moved further out (p3f 27, p3b 13, the gondola's two and two at the climbs among them), 62 moved to a
  neighbouring slot (p3b 42, p3a 12, p3f 8), 29 p3c rules re-phased, 5 p3c rules rewritten to fire only on their free
  slots, and 31 rules taken out where no free place was found: 20 p3f sites, 3 p3b scenes, 2 p3a pieces, 6 p3c lines
  (the left of the resort and of the last lifts has no free slot in the 64 to 67 m band).
- The table went from 1,761 rules and 139 sets to 1,754 and 136.

### Measured after the fix (one start of the level, `C:\ftwork\out\p3\fixed_L5\run.log`)

- Seven meshes regenerated in one editor job (`C:\ftwork\logs\L5p3_fix_make.log`, 0 failed): SM_P3A_SnowSeat_A 1,820
  triangles top 1.51 m low, SM_P3A_SledKids_A 1,652 / 1.51 low, SM_P3A_Huskies_A 1,656 / 1.52 low, SM_P3B_SnowFort_A
  2,112 / 5.4 TALL, SM_P3B_IceCore_A 2,278 / 7.7 TALL, SM_P3C_Skidoos_A 1,484 / 4.47 TALL, SM_P3E_Choughs_A 1,128 /
  48.41 TALL.
- Compile: succeeded (`C:\ftwork\logs\L5p3_fix_build.log`).
- Track: `FTTRACK level=5 frames=12415 route=b2fc812a solid=9cada242 solidTriangles=931996`, identical to the level 5
  line of `SourceArt/Jam/validate/track_baseline.txt`.
- `FTDRESS level=5 districts=708 sets=168 pieces placed=11796 of 11824 tried shadows=3745 of 11871` (before the fix:
  724, 171, 11896 of 11924): 100 fewer pieces.
- `FTCLEAR props kept=14603 (moved off their own barrier 808) left out of the road=28`. Moved was 778: the three low
  pieces on their plinth are now over the height at which this hill's sight line pushes a piece out (the builder's
  note says to about 24.7 m): SM_P3A_Huskies_A 10 of 39 moved, SM_P3A_SnowSeat_A 2 of 37, SM_P3A_SledKids_A 18 of 33.
  None left out. Crew kit after the fix: SM_CrewCamera_Snow 46 placed (was 52), SM_CrewVillage_Snow 9, SM_CrewStunt_Snow
  23, SM_CrewApplauseA_Snow 28, SM_CrewApplauseB_Snow 41.
- No "Fatal error", no "LogFT: Error", no failed load of a /Game/People asset, no engine ensure in this run.
- By the fixer's replay no pass 3 rule puts anything nearer than 17 m or a tall piece nearer than 24 m, before or after
  (pieces were only moved outwards or along the road), and no pass 3 mesh comes to more than 60 pieces (the highest, as an
  upper bound that does not take off what the track leaves out at loops, pipes and jumps: SM_P3C_Geese_A 53,
  SM_P3B_Rescue_A 51, SM_P3C_Skidoos_B, SM_P3C_SkiTrail_B and SM_P3C_Kites_A 50).

### Not fixed

- The bride all in white in SM_P3B_Wedding_A: the reader asked for no change unless a frame shows a likeness to the
  boss; the driver saw "nothing white and furry besides the boss". Left.
- A pine of the level standing through the first-pass groomer piece at about 6.4 km right: not a pass 3 rule (the
  driver listed it so that pass 3 is not blamed). Left.
- Three possible same-spot pairs of a first-pass SM_DogSled_A (65 m) with a p3d structure (slots 31, 131 and 149, left):
  both rules repeat along their zone, so no one-line change removes one slot. Left.
- Box overlaps looser than "same spot" (the reader's second list, 313 pairs with 2 m of overlap) were not worked
  through: only the same-spot pairs, the ice climbs and the gondola slots were.
- Triangle budgets (50 of 73 meshes over the 1,500 guide), `ICEFAR` never #undef'd and the unused first-pass set
  PplWatchBankBR (both in the older part of the table): reported by the reader as facts, left.
- First-pass eagles (SM_Eagles_A, dark brown, 60 to 130 m, in every district) and the pass 3 geese are still dark
  birds. The driver named the choughs for the likeness to the chopper; the start frame of the fixed level still
  shows dark birds in the sky at 0.25 km (not looked into which mesh).

### Never checked (after the fix)

- No drive was made after the fix: one start of the level, one frame at 6 seconds, which shows the first 300 m only.
  None of the seven regenerated meshes was looked at in a frame or a render: the yellow flock, the violet snowmobile
  and balloon, the snow fort without snowballs, the three low pieces on their plinth (whether they now sit right on
  the falling ground and still clear nothing they should not at 1.5 m).
- Whether the yellow birds, the remaining dark birds or the drones still at 46 m and more can be taken for an enemy.
- The new places: every move was chosen from the tables' text and the logged sizes. Pines, the level's ground
  (cuttings, falling land) and the true shapes of dressing pieces were not read. A piece moved one slot along may
  stand beside a banked bend it was not written for (p3b at 38 m and more, p3a and p3f at 46 m and more).
- p3d's draws at chance 0.75 are new (rules before it were taken out, and a rule's number seeds its draws), so the
  p3d structures stand on other slots than in the driver's frames; what was said about single p3d pieces no longer
  applies one to one.
- Placed counts per pass 3 mesh in the game (the log lists a mesh only when some were moved or left out), frame
  rate, movement, the finale past 14 km, the other seven levels.
