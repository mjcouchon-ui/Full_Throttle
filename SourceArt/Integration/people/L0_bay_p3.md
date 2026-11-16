# Bayshore (level index 0), population pass 3: integration

Branch `claude/people-L0-p3`. Six builders (p3a..p3f) each wrote a generator, meshes and a table block; the integrator
joined the six blocks into `Source/FullThrottle/FTPeopleL0.inl`, compiled once and started the level once.
Nobody looked at a frame: see "Not checked".

## What pass 3 added

New meshes in `/Game/People/Bayshore` (`SM_P3A_*` .. `SM_P3F_*`), made by `Content/Python/ft_people_bay_p3a.py` ..
`ft_people_bay_p3f.py`. Triangles, top height and TALL/low are from the builders' make logs
(`C:\ftwork\logs\L0p3<letter>_make.log`). The last column is the mesh's entry in the game's FTCLEAR line
(left out / moved off the barrier / kept); the game lists only meshes with at least one piece moved or left out, so
"-" means no piece of that mesh was moved or left out (its placed count is then not printed by the game).

| builder | focus | mesh | triangles | top m | log | FTCLEAR -out/~moved/kept |
|---|---|---|---|---|---|---|
| p3a | gaps | SM_P3A_Pave_A | 1748 | 1.9 | low | -0/~8/46 |
| p3a | gaps | SM_P3A_Pave_B | 1648 | 1.92 | low | -0/~6/40 |
| p3a | gaps | SM_P3A_Sand_A | 1868 | 2.88 | TALL | - |
| p3a | gaps | SM_P3A_Sand_B | 1836 | 3.16 | TALL | - |
| p3a | gaps | SM_P3A_Sand_C | 2164 | 2.79 | TALL | - |
| p3a | gaps | SM_P3A_Sand_D | 2656 | 2.76 | TALL | - |
| p3a | gaps | SM_P3A_SideSt_A | 1848 | 1.8 | low | - |
| p3a | gaps | SM_P3A_SideSt_B | 1860 | 1.86 | low | - |
| p3a | gaps | SM_P3A_SideSt_C | 1860 | 1.91 | low | - |
| p3a | gaps | SM_P3A_WalkRest_A | 1924 | 1.6 | low | -0/~8/43 |
| p3a | gaps | SM_P3A_WalkRest_B | 1896 | 1.76 | low | -0/~12/43 |
| p3a | gaps | SM_P3A_WalkWork_A | 1682 | 1.72 | low | -0/~8/35 |
| p3a | gaps | SM_P3A_WalkWork_B | 2008 | 1.83 | low | -0/~10/38 |
| p3b | scenes | SM_P3B_Carnival_A | 3376 | 5.1 | TALL | -0/~1/23 |
| p3b | scenes | SM_P3B_Carnival_B | 3168 | 7.0 | TALL | -0/~1/25 |
| p3b | scenes | SM_P3B_FishMarket | 2522 | 6.85 | TALL | - |
| p3b | scenes | SM_P3B_Luau | 3240 | 3.45 | TALL | - |
| p3b | scenes | SM_P3B_Mural | 2676 | 4.87 | TALL | -0/~5/33 |
| p3b | scenes | SM_P3B_SandCastle | 3288 | 4.55 | TALL | - |
| p3b | scenes | SM_P3B_StreetShow | 3144 | 5.26 | TALL | -0/~2/29 |
| p3b | scenes | SM_P3B_SurfBoat | 2920 | 3.3 | TALL | - |
| p3b | scenes | SM_P3B_SurfContest | 3148 | 5.17 | TALL | - |
| p3b | scenes | SM_P3B_TugOfWar | 3252 | 3.15 | TALL | - |
| p3b | scenes | SM_P3B_Wedding | 3148 | 3.85 | TALL | - |
| p3b | scenes | SM_P3B_Yoga | 1876 | 3.45 | TALL | - |
| p3c | movers | SM_P3C_BeachPath_A | 3016 | 2.4 | TALL | - |
| p3c | movers | SM_P3C_BeachPath_B | 2424 | 2.17 | TALL | - |
| p3c | movers | SM_P3C_BeachRace | 2272 | 2.15 | TALL | - |
| p3c | movers | SM_P3C_Circles | 2012 | 5.2 | TALL | - |
| p3c | movers | SM_P3C_Paddlers | 2736 | 2.07 | TALL | - |
| p3c | movers | SM_P3C_Ponies | 2084 | 1.93 | low | - |
| p3c | movers | SM_P3C_Riders | 1708 | 2.58 | TALL | - |
| p3c | movers | SM_P3C_SandTrain | 2152 | 2.46 | TALL | - |
| p3c | movers | SM_P3C_Wheels_A | 2048 | 1.79 | low | -0/~14/33 |
| p3c | movers | SM_P3C_Wheels_B | 2404 | 1.77 | low | -0/~17/37 |
| p3c | movers | SM_P3C_Wheels_C | 1672 | 1.92 | low | -0/~11/35 |
| p3c | movers | SM_P3C_Wheels_D | 2040 | 1.9 | low | -0/~15/37 |
| p3d | heights | SM_P3D_BalconyBlock | 2476 | 16.2 | TALL | - |
| p3d | heights | SM_P3D_ClimbWall | 1844 | 13.04 | TALL | - |
| p3d | heights | SM_P3D_FootBridge_A | 2072 | 11.2 | TALL | - |
| p3d | heights | SM_P3D_FootBridge_B | 2296 | 11.2 | TALL | - |
| p3d | heights | SM_P3D_JudgeTower | 2240 | 11.1 | TALL | - |
| p3d | heights | SM_P3D_Lookout | 1634 | 12.8 | TALL | - |
| p3d | heights | SM_P3D_RoofParty_A | 2128 | 8.46 | TALL | - |
| p3d | heights | SM_P3D_RoofParty_B | 1960 | 8.46 | TALL | - |
| p3d | heights | SM_P3D_ScissorLift | 1344 | 8.04 | TALL | - |
| p3d | heights | SM_P3D_SignPainters | 1180 | 11.4 | TALL | - |
| p3d | heights | SM_P3D_SkyBikes | 2244 | 10.05 | TALL | - |
| p3e | far and sky | SM_P3E_Balloons_A | 1212 | 87.44 | TALL | -1/~0/30 |
| p3e | far and sky | SM_P3E_Balloons_B | 888 | 81.36 | TALL | - |
| p3e | far and sky | SM_P3E_BeachGame | 1516 | 8.0 | TALL | - |
| p3e | far and sky | SM_P3E_Chutes_A | 1056 | 81.31 | TALL | - |
| p3e | far and sky | SM_P3E_Chutes_B | 1200 | 79.71 | TALL | - |
| p3e | far and sky | SM_P3E_Flamingos | 1188 | 23.51 | TALL | -1/~0/30 |
| p3e | far and sky | SM_P3E_Gliders_A | 840 | 55.15 | TALL | - |
| p3e | far and sky | SM_P3E_Gliders_B | 672 | 61.77 | TALL | - |
| p3e | far and sky | SM_P3E_JetSkis | 1248 | 3.53 | TALL | - |
| p3e | far and sky | SM_P3E_KiteSurf | 1244 | 28.09 | TALL | -4/~0/27 |
| p3e | far and sky | SM_P3E_Riders | 1636 | 5.85 | TALL | -1/~0/27 |
| p3e | far and sky | SM_P3E_Rowers | 1116 | 2.47 | TALL | - |
| p3f | film shoot | SM_P3F_DroneTeam_A | 1684 | 6.64 | TALL | - |
| p3f | film shoot | SM_P3F_DroneTeam_B | 1832 | 8.0 | TALL | - |
| p3f | film shoot | SM_P3F_Extras_A | 2256 | 4.5 | TALL | - |
| p3f | film shoot | SM_P3F_Extras_B | 2180 | 4.5 | TALL | - |
| p3f | film shoot | SM_P3F_MakeUp | 2040 | 4.2 | TALL | - |
| p3f | film shoot | SM_P3F_PressPen | 1988 | 4.3 | TALL | - |
| p3f | film shoot | SM_P3F_Reflector | 1496 | 5.66 | TALL | - |
| p3f | film shoot | SM_P3F_SecondUnit_A | 1844 | 5.4 | TALL | - |
| p3f | film shoot | SM_P3F_SecondUnit_B | 2076 | 4.5 | TALL | - |
| p3f | film shoot | SM_P3F_StuntRig | 1808 | 8.45 | TALL | - |

"Estimated placed" is each builder's own estimate, not measured.

| builder | focus | meshes | rules in block | estimated placed (builder) |
|---|---|---|---|---|
| p3a | gaps | 13 | 45 | 390 |
| p3b | scenes | 12 | 105 | 285 |
| p3c | movers | 12 | 170 | 404 |
| p3d | heights | 11 | 44 | 329 |
| p3e | far and sky | 12 | 72 | 404 |
| p3f | film shoot | 10 | 131 | 330 |
| all | | 70 | 567 | 2142 |

p3f's block also places eight meshes of the existing crew kit
(`/Game/People/Crew/SM_Crew{Lights,Camera,ApplauseA,ApplauseB,Catering,Village,Crane,Sound}_Sun`); all eight exist.

## FTDRESS before and after (dressing and people together)

- Before (`C:\ftwork\logs\p3_prep_L0.txt`): `FTDRESS level=0 districts=722 sets=77 pieces placed=11608 of 11704 tried`
- After (`C:\ftwork\out\p3\after_L0\run.log`): `FTDRESS level=0 districts=1114 sets=130 pieces placed=13641 of 13755 tried shadows=7540 of 14289`
- Difference: 2,051 more tried, 2,033 more placed (the builders' estimates add up to 2,142).

Track: `FTTRACK level=0 frames=12717 route=dc9e032b solid=d2e759f4 solidTriangles=986296`, identical to the
`FTTRACK level=0` line of `SourceArt/Jam/validate/track_baseline.txt`.

Run log: no "Fatal error", no "LogFT: Error", no failed load of a `/Game/People` asset. The level came up
(`FT LEVEL id=L1 scene=1`), the run ended with exit 0.

## What was changed, dropped, moved or left out

- Blocks in the table: p3a, p3b, p3c, p3d, p3e, p3f, appended in that order to the end of `FTPeopleL0.inl`, each wrapped in `{ }`.
  Nothing above them in the file was changed. Line ends were converted to the file's CRLF; otherwise the blocks are as the builders wrote them.
- Blocks dropped: none. Rules removed: none. Meshes missing: none (every mesh a block names exists as a `.uasset`; no generator was rerun).
- Distance rules (checked from the text by `C:\ftwork\jobs\p3int_L0\check.py`, Set offsets included): no rule under 17 m and no TALL mesh
  under 24 m, so nothing was raised. Nearest low pieces: p3a WalkRest/WalkWork/Pave at 21.3 m, p3c Wheels_A/B at 21.7 m and Wheels_C/D
  at 22.4 m. Nearest TALL: crew kit pieces of p3f at 26 m.
- Compile: `build.ps1 -Tag L0p3_build` succeeded at the first try (FTTrackDress.cpp recompiled); no compile fix was needed.
- Moved by the track off its own barrier (the "~" count): p3a WalkRest_A 8, WalkRest_B 12, WalkWork_A 8, WalkWork_B 10, Pave_A 8, Pave_B 6;
  p3b Mural 5, Carnival_A 1, Carnival_B 1, StreetShow 2; p3c Wheels_A 14, Wheels_B 17, Wheels_C 11, Wheels_D 15.
- Left out by the track: p3e KiteSurf 4, Balloons_A 1, Riders 1, Flamingos 1.

## Departures the builders reported (not corrected here)

- Most meshes are over the 1,500-triangle guide (up to 3,376, `SM_P3B_Carnival_A`).
- p3c Wheels_A/B/C/D stand at 21.7 / 22.4 m (builder a's band); p3c Paddlers are Afloat at 395 to 420 m (builder e's band).
- p3e's balloons carry one small steady glow each (335 m out and more).
- Banked bends are not left out by the p3a and p3c rules.

## Not checked

- Frames were NOT looked at by anyone: not by the six builders (no contact sheets, no game run) and not by the integrator.
  The one frame the start script saved (`C:\ftwork\out\p3\after_L0\L0_t006.png`) was not opened.
- So unseen: how any piece looks or reads from the road, colours and poses, whether anything floats, sinks or interpenetrates,
  whether any movement (travel, orbit, bob, swing) looks right, and every overlap between the six blocks, the first cast and the dressing.
- Per-mesh placed counts of the meshes that are not in the FTCLEAR list (the game prints only meshes with a piece moved or left out).
- Where the moved pieces ended up, and whether they still stand on the deck or pavement they were drawn for.
- Glow, blink and "could be taken for a pickup or the boss" rules: the integrator's text check covered distances and mesh names only.
- Frame rate and memory with about 2,000 more pieces; only the first 6 seconds of the level were run (scene 1, quality 3).
- The later districts in play.

## Review and fix round

Two reviewers looked at pass 3 (a rules reader, text only; a driver, eight 24 s drives: 64 stills, two films), then one
fix round, one compile, one start of the level. Contact sheet of the driver's crops: `sheets/L0_p3_review.jpg`
(frames from BEFORE the fixes). Nobody has looked at a frame since the fixes.

### What the reviewers found

- Rule broken (driver): the parachutes (`SM_P3E_Chutes_A/B`) and the paragliders (`SM_P3E_Gliders_A`) fill the sky with
  dark canopies with a figure under each, on a level whose enemy helicopter "drops men on you" (gunships from 3.7 km on).
- Rule broken, as placed (reader): the six boardwalk meshes (p3a WalkRest/WalkWork, p3c Wheels_A/B) are lifted 0.48 m by
  their Sets, so their tops stand 2.08 to 2.31 m over the land at 21.3 / 21.7 m; the track moved 69 of them out past 24 m.
- Looks wrong (reader): with the town side's Pave_A/B and Wheels_C/D, 109 near pieces were moved by the track, to about
  26.5 m: off the boardwalk (it ends 24.1 m out), in among the shoppers (24.6 m) and the shop fronts (27.5 m).
- Looks wrong (driver): a "paraglider" trailing a long thin dark hooked line; figures standing in a kiosk on the town side
  (scene 4); the boardwalk's people show only as heads and shoulders over the barrier.
- Weak: the camera drones (dark rotorcraft 3.4 m across, 29 m out, one idling like a pickup); `SM_CrewSound_Sun`'s boom
  reaching to 23.4 m and `SM_FarFlock`'s wing tips to 23.6 m (older rules); three Sets never used; nobody in sight on the
  town side on the banked bends and the cliff road; far beach figures read as black blots; movement not judged.

### What was fixed

- Parachutes: all 12 `Chutes_A` / `Chutes_B` rules are off (chance 0.f; the rules keep their places so that no later rule's
  random numbers change). Paragliders: `Gliders_A` is off in every second district (the Marina, Hotel Row, the Point); it
  stays over the Strip, Pleasure Beach and Downtown.
- The hooked line: it is not on the paragliders. `gliders_a` has no streamer; `chutes_a` has two ("one trails a long pink
  streamer, one a yellow one") and the crop shows them under SQUARE canopies. It went with the parachutes; `Gliders_A` was
  not regenerated.
- The pieces the track moved: the 48 rules of the ten near meshes (p3a 24 Sites, p3c 12 Sites and 12 Scatters) are off in
  their blocks and stand again in a new block at the end of the table ("p3 fix round"), same Sets, distances and slot
  residues, but only in the 263 slots where the dressing has its level and gentle street rows (60 runs, the same runs p3b's
  beach scenes use). Reason: counted per mesh, the moved pieces were as many as each mesh had in the dressing's slope and
  steep slots (the long cyclists' lines nearly all of them, the short pieces about half). This also takes these pieces off
  the steep slots where the kiosks stand, the likeliest place of the figures-in-a-kiosk frame (not identified, not re-seen).
  Result (FTCLEAR, moved/kept): WalkRest_A 2/23 (was 8/43), WalkRest_B 6/31 (12/43), WalkWork_A 3/23 (8/35), WalkWork_B
  4/25 (10/38), Pave_A 2/26 (8/46), Pave_B 1/28 (6/40), Wheels_A 1/17 (14/33), Wheels_B 4/22 (17/37), Wheels_C 1/23
  (11/35), Wheels_D 5/21 (15/37): 29 moved of 239 kept, where it was 109 of 387.
- Drones: `drone_team_a` and `drone_team_b` draw their flying drones at 0.9 (were 1.5 and 1.4: about 2 m across now), and
  the hovering one no longer turns its nose from side to side (bob only). Both meshes regenerated
  (`C:\ftwork\logs\L0p3_fix_make.log`: DroneTeam_A 1684 triangles top 6.54 m TALL, DroneTeam_B 1832 triangles top 8.0 m
  TALL, built 2 failed 0). Their distance (29 m and more) is unchanged.
- Older rules: `PplShootB`'s sound cart out 4.6 -> 5.3 m; the six `FarFlock` scatters x min 65 -> 66 m.
- The three unused Sets (`PplP3BWeddingR`, `PplP3BSurfBoatR`, `PplP3BLuauR`) are deleted.

After the fix (`C:\ftwork\out\p3\fixed_L0\run.log`, build `L0p3_fix_build` succeeded):
`FTTRACK level=0 frames=12717 route=dc9e032b solid=d2e759f4 solidTriangles=986296` (equal to the baseline's level 0 line);
`FTDRESS level=0 districts=1174 sets=127 pieces placed=13415 of 13529 tried shadows=7540 of 14063` (226 fewer placed than
before the fix: the parachutes, half the paragliders, and the near pieces on the hills); FTCLEAR moved off the barrier
1120 (was 1200). No "Fatal error", no "LogFT: Error", no failed `/Game/People` load. One handled ensure (DoubleFloat.cpp
line 19, the renderer's precision warning the driver also met in three of eight runs and in a first-pass log).

### What was not fixed

- 29 near pieces are still moved by the track (the list above). By the same count they should be the ones in the
  "gentle" slots (ShopRowB); restricting the fix block to the level rows only would be the next step. Not done: one round.
- The sight rule for the lifted boardwalk pieces rests, as before, on the track (it moves what stands over 2 m within 24 m);
  the first cast's strollers do the same (92 of 92 and 87 of 87 moved) and were not touched.
- Heads and shoulders over the barrier: left. Raising the Sets to 0.9 m would lift the figures off the deck and over 2 m.
- Nobody in sight on the town side on the banked bends and the cliff road: no rule added. The bands the driver proposed
  (30 to 40 m) hold the shop rows (27.5 and 33 m), the kiosks (31.6 m) and the hills' walkway; p3d's free town slots are
  all in use; placing blind there is how the kiosk overlap came about.
- Far beach figures as black blots: no change (the driver asked for none).
- The player's car drawn as wheels only, the films' movement: outside this round.
- Triangle budgets (most meshes over 1,500), Gliders_A placed with Far inside the land: reported by the reader, not changed.

### Never checked

- No frame was looked at after the fixes: not the smaller drones, not the sky without parachutes, not the near pieces in
  their fewer slots, not where the 29 still-moved pieces stand. The start run's one frame (`L0_t006.png`) was not opened.
- Whether the remaining paragliders still read as an attack; whether the drones still read as an enemy or a pickup.
- Movement of any pass 3 piece; flicker; overlaps between pass 3, the first cast and the dressing beyond the one kiosk frame.
- Most pass 3 meshes were never identified by name in a frame (the driver's list); scenes 2, 4, 6, 8, 10, 12, 14, 16.
- The Python of the two drone builders ran in the editor once; the other eight p3f meshes and all other generators were
  not rerun.
- Frame rate and memory.
