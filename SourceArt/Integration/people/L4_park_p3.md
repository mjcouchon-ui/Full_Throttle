# Level 4, Theme Park: population pass 3 (integration)

Six builders (p3a to p3f) each wrote a generator, meshes and a table block. The integrator joined the six blocks
onto the end of `Source/FullThrottle/FTPeopleL4.inl`, compiled once and started the level once. Nobody looked at a
frame: see "Not checked".

## What was run and seen

- Meshes: all 71 `SM_P3*` meshes the blocks name exist in `Content/People/ThemePark` (10 + 13 + 13 + 10 + 11 + 14);
  the four shared crew meshes block f names (`SM_CrewLights_Park`, `SM_CrewCrane_Park`, `SM_CrewSound_Park`,
  `SM_CrewCatering_Park`) exist in `Content/People/Crew`. No generator was rerun.
- Rules check from the text: no rule places a TALL mesh nearer than 24 m (nearest tall rule: 27.5 m, block f); the
  only rules under 24 m are the four low `SM_P3A_Kerb_*` pieces at 21 m (log: top 1.80 to 1.84 m, "low"). No rule
  was changed.
- Compile (`L4p3_build`): succeeded at the first try, no change to any block.
- One game start (`C:\ftwork\out\p3\after_L4`): the level came up (`FT LEVEL id=L5 scene=1`), no "Fatal error",
  no "LogFT: Error", no failed load of a /Game/People asset in the log. The log shows a mesh build line for each of
  the 71 `SM_P3*` meshes.
- Track: `FTTRACK level=4 frames=12518 route=673f0081 solid=3e2a2030 solidTriangles=987216`, identical to the
  level 4 line of `SourceArt/Jam/validate/track_baseline.txt`.

## FTDRESS before and after

| | districts | sets | pieces placed | tried |
|---|---|---|---|---|
| before (`C:\ftwork\logs\p3_prep_L4.txt`) | 161 | 49 | 10951 | 11034 |
| after | 228 | 60 | 13169 | 13283 |
| pass 3 | +67 | +11 | +2218 | +2249 |

The six builders' estimates add up to 2337.

## What pass 3 added

"left out / moved / placed" is the mesh's entry in the run's `FTCLEAR level=4` line. That line carried an entry for
24 of the 71 meshes (each has a piece left out or moved); for the other 47 the run printed no count of its own
("no entry" below), so their placed counts are not measured one by one.

| builder | focus | mesh | triangles | top m | log | left out / moved / placed |
|---|---|---|---|---|---|---|
| p3a | gaps | SM_P3A_Kerb_A | 1540 | 1.82 | low | 0 / 5 / 43 |
| p3a | gaps | SM_P3A_Kerb_B | 1608 | 1.84 | low | 0 / 3 / 39 |
| p3a | gaps | SM_P3A_Kerb_C | 1536 | 1.8 | low | 0 / 6 / 38 |
| p3a | gaps | SM_P3A_Kerb_D | 1412 | 1.81 | low | 0 / 3 / 38 |
| p3a | gaps | SM_P3A_Painters | 1960 | 6.94 | TALL | no entry |
| p3a | gaps | SM_P3A_Photo | 1700 | 7.14 | TALL | no entry |
| p3a | gaps | SM_P3A_Prizes | 1780 | 5.42 | TALL | no entry |
| p3a | gaps | SM_P3A_RailGang | 1852 | 32.89 | TALL | 4 / 29 / 35 |
| p3a | gaps | SM_P3A_Terrace | 1900 | 6.65 | TALL | no entry |
| p3a | gaps | SM_P3A_Train | 1852 | 5.1 | TALL | no entry |
| p3b | scenes | SM_P3B_Backstage | 2884 | 7.28 | TALL | no entry |
| p3b | scenes | SM_P3B_BigPrize | 2498 | 8.06 | TALL | no entry |
| p3b | scenes | SM_P3B_Birthday | 3140 | 7.28 | TALL | no entry |
| p3b | scenes | SM_P3B_Cadets | 3014 | 7.28 | TALL | no entry |
| p3b | scenes | SM_P3B_Dance | 2600 | 7.33 | TALL | no entry |
| p3b | scenes | SM_P3B_Dance_Sky | 2828 | 22.73 | TALL | 3 / 8 / 15 |
| p3b | scenes | SM_P3B_Magic | 2620 | 7.74 | TALL | no entry |
| p3b | scenes | SM_P3B_Magic_Sky | 2848 | 23.14 | TALL | 0 / 8 / 16 |
| p3b | scenes | SM_P3B_PieContest | 3276 | 7.28 | TALL | no entry |
| p3b | scenes | SM_P3B_RideDrill | 2976 | 7.28 | TALL | 0 / 1 / 31 |
| p3b | scenes | SM_P3B_Treasure | 2690 | 7.28 | TALL | no entry |
| p3b | scenes | SM_P3B_Wedding | 3134 | 8.34 | TALL | no entry |
| p3b | scenes | SM_P3B_Wedding_Sky | 3362 | 23.74 | TALL | 2 / 7 / 15 |
| p3c | movers | SM_P3C_Flamingos | 1170 | 7.36 | TALL | 2 / 0 / 52 |
| p3c | movers | SM_P3C_HorseTram | 2628 | 6.52 | TALL | 0 / 7 / 21 |
| p3c | movers | SM_P3C_LogRide_Hi | 1628 | 31.7 | TALL | 2 / 0 / 24 |
| p3c | movers | SM_P3C_LogRide_Lo | 1752 | 11.7 | TALL | no entry |
| p3c | movers | SM_P3C_MiniRail | 2524 | 6.0 | TALL | 0 / 4 / 19 |
| p3c | movers | SM_P3C_Parrots | 1188 | 17.04 | TALL | 3 / 1 / 47 |
| p3c | movers | SM_P3C_Rink | 1792 | 11.5 | TALL | no entry |
| p3c | movers | SM_P3C_SkyBikes | 1536 | 11.8 | TALL | no entry |
| p3c | movers | SM_P3C_Skywalk_Hi | 2404 | 34.2 | TALL | 4 / 0 / 22 |
| p3c | movers | SM_P3C_Skywalk_Lo | 2380 | 12.82 | TALL | no entry |
| p3c | movers | SM_P3C_Walk_A | 2764 | 6.52 | TALL | 1 / 4 / 19 |
| p3c | movers | SM_P3C_Walk_B | 2776 | 6.52 | TALL | 0 / 5 / 19 |
| p3c | movers | SM_P3C_Walk_C | 3072 | 6.52 | TALL | 0 / 6 / 20 |
| p3d | heights | SM_P3D_Balcony_A | 1920 | 15.8 | TALL | no entry |
| p3d | heights | SM_P3D_Balcony_B | 1948 | 21.3 | TALL | no entry |
| p3d | heights | SM_P3D_Castle | 1644 | 13.9 | TALL | no entry |
| p3d | heights | SM_P3D_ClockTower | 2008 | 20.0 | TALL | no entry |
| p3d | heights | SM_P3D_CrowsNest | 2172 | 31.9 | TALL | no entry |
| p3d | heights | SM_P3D_Gantry | 2264 | 18.45 | TALL | no entry |
| p3d | heights | SM_P3D_Lighthouse | 2176 | 28.94 | TALL | 4 / 0 / 40 |
| p3d | heights | SM_P3D_RoofParty | 2112 | 10.74 | TALL | no entry |
| p3d | heights | SM_P3D_RopeBridge | 1816 | 13.1 | TALL | no entry |
| p3d | heights | SM_P3D_TreeHouse | 1820 | 19.5 | TALL | no entry |
| p3e | far and sky | SM_P3E_ClimbSpire | 2224 | 40.03 | TALL | no entry |
| p3e | far and sky | SM_P3E_Flamingos | 972 | 21.64 | TALL | 1 / 0 / 40 |
| p3e | far and sky | SM_P3E_Kites_A | 1148 | 43.5 | TALL | no entry |
| p3e | far and sky | SM_P3E_Kites_B | 1660 | 42.49 | TALL | no entry |
| p3e | far and sky | SM_P3E_Monorail | 1416 | 20.28 | TALL | no entry |
| p3e | far and sky | SM_P3E_Parrots | 1008 | 24.45 | TALL | no entry |
| p3e | far and sky | SM_P3E_PicnicHill | 1984 | 20.22 | TALL | no entry |
| p3e | far and sky | SM_P3E_SkyTeam | 1300 | 48.12 | TALL | no entry |
| p3e | far and sky | SM_P3E_Train | 1724 | 17.7 | TALL | no entry |
| p3e | far and sky | SM_P3E_WaveStand | 2332 | 22.9 | TALL | no entry |
| p3e | far and sky | SM_P3E_Zipline | 1544 | 24.74 | TALL | no entry |
| p3f | film shoot | SM_P3F_Applause_B | 2840 | 2.69 | TALL | no entry |
| p3f | film shoot | SM_P3F_Dolly | 2308 | 2.67 | TALL | 0 / 2 / 12 |
| p3f | film shoot | SM_P3F_DroneTeam_A | 2152 | 7.0 | TALL | no entry |
| p3f | film shoot | SM_P3F_DroneTeam_B | 2096 | 5.73 | TALL | no entry |
| p3f | film shoot | SM_P3F_Extras_A | 3256 | 4.15 | TALL | no entry |
| p3f | film shoot | SM_P3F_Extras_B | 3184 | 4.15 | TALL | no entry |
| p3f | film shoot | SM_P3F_LagoonTower_A | 2032 | 31.9 | TALL | 3 / 17 / 17 |
| p3f | film shoot | SM_P3F_LagoonTower_B | 2092 | 34.27 | TALL | 2 / 6 / 17 |
| p3f | film shoot | SM_P3F_MakeUp | 2276 | 4.23 | TALL | no entry |
| p3f | film shoot | SM_P3F_PressPen_A | 3682 | 3.74 | TALL | no entry |
| p3f | film shoot | SM_P3F_PressPen_B | 3270 | 3.93 | TALL | no entry |
| p3f | film shoot | SM_P3F_SecondUnit_A | 2084 | 2.84 | TALL | 0 / 1 / 19 |
| p3f | film shoot | SM_P3F_SecondUnit_B | 2628 | 5.62 | TALL | no entry |
| p3f | film shoot | SM_P3F_StuntRig | 2060 | 6.25 | TALL | no entry |

Estimated placed, the builders' own figures: p3a 487, p3b 290, p3c 456, p3d 350, p3e 454, p3f 300.

What stands where, from the blocks' own text (not seen in a frame):

- p3a, gaps: low kerb groups at 21 m (guests at a rail, staff at work, benches, walkers behind a hedge) on the lift
  hill, splashdown, the funhouse jump and the free verge slots of the Last Ride; a cafe terrace, painters' scaffold,
  miniature railway, photo spot and prize delivery at 29 to 49 m; rail gang towers in the lagoon.
- p3b, scenes: ten story stages at 38 to 42 m (wedding, birthday, high striker, dance class, magic show, pie
  contest, ride drill, backstage, treasure hunt, space cadets), three of them again on towers in the lagoon.
- p3c, movers: raised promenades, a miniature railway and a horse tram at 28 to 29.5 m; high walks, log ride,
  pedal cars and a roller rink at 60 to 69 m; macaws and flamingos further out.
- p3d, heights: hotel balconies, clock tower, castle wall, tree house, rope bridge, lighthouse, crow's nest, launch
  gantry and a roof party at 39.5 to 68 m.
- p3e, far and sky: kite hills, flocks, a far railway and monorail, a zip line, a climbing rock, a parachute team,
  a picnic hill and a wave stand, all 150 m out or more.
- p3f, film shoot: second units, drone teams, stunt rig, extras areas, make-up tent, press pens, a dolly and an
  applauding crew at 27.5 to 36 m, camera and drone towers in the lagoon, and 4 pieces of the shared crew kit.

## Changed, dropped, moved, left out

- Changed by the integrator: nothing. The six block files went in as written, each wrapped in `{ }`, after the
  "watch" block. No rule distance was raised, no block was dropped, no generator was edited.
- Left out by the track (first number of the entries above): RailGang 4, Wedding_Sky 2, Dance_Sky 3, Walk_A 1,
  Parrots (p3c) 3, Flamingos (p3c) 2, Skywalk_Hi 4, LogRide_Hi 2, Lighthouse 4, Flamingos (p3e) 1, LagoonTower_A 3,
  LagoonTower_B 2: 31 pieces.
- Moved off the barrier by the track (second number): most of all the lagoon towers (RailGang 29 of 35,
  LagoonTower_A 17 of 17, the three _Sky scenes 7 to 8 of 15 or 16, LagoonTower_B 6 of 17); the near ways of p3c 4
  to 7 each; the kerb groups 3 to 6 each. Where they were moved to was not looked at.
- Over the builders' triangle guide (about 1,500 a piece) and left as built: most meshes, up to 3,682
  (SM_P3F_PressPen_A).
- Risks the builders named and nobody resolved: SM_SignRide sharing Last Ride slots with Prizes and the make-up
  tent; palms and barrels scattered through Pirate Cove's 26 to 60 m; ponds at 36 to 46 m under scenes and decks;
  stunt rig and drone team rostrums in the Midway's stall crowd slots; RideDrill's sheets reaching 7 m forward of
  its stage; the shared crew kit meshes are unlit in this night level; the p3e train and monorail are vehicles.
- The run log has one `LogFT: Warning: FT WARM no assets found (asset registry empty?)`; whether it was there
  before pass 3 was not checked.

## Not checked

- Frames were NOT looked at by anyone: not by the six builders (no contact sheets, no game runs) and not by the
  integrator (the one frame the start script saves, `L4_t006.png`, was not opened). Shape, pose, colour, floating or
  interpenetrating parts, whether anything moves as meant, whether pieces read at night, whether any piece stands
  in a dressing piece or in another builder's piece: all not checked.
- Placed counts for the 47 meshes without an FTCLEAR entry; only the total (+2218) is measured.
- Glow and blink distances were taken from the builders' reports and block comments, not measured.
- Frame rate and memory with 2218 more pieces: not measured.
- Only the level's first scene was started, for 6 seconds of level clock; later scenes were not started.

## Review and fix round

Two reviewers looked at pass 3 after the integration: a rules reader (text only: the table, the make logs, the
generators run outside Unreal) and a driver (12 game runs, 98 shots, 5 films; contact sheet
`sheets/L4_p3_review.jpg`). One fix round followed. Nobody has looked at the fixes in frames.

### What the reviewers found

- Rule broken (older rule): SM_FarDucks was placed with 15 degrees of yaw dice; its skein flies 360 m along its own
  X as placed, so a turned skein's flight line came over the road.
- Looks wrong: in the lagoon three builders' towers stood inside each other on both sides (left slots 1 mod 3:
  p3b tower stages at 64 m, p3c high ways at 62 to 64 m, p3d lighthouse and mast at 64 to 66 m; right slots 5, 8,
  11 mod 12: p3a rail gang and p3b tower stage both at 51 to 52 m; right slots 2 mod 3: p3c at 62 to 64 m and p3d
  at 64 to 66 m).
- Looks wrong: free-flying quadcopters where the level's enemies fly on rotors (SM_P3F_LagoonTower_B at the rails'
  height in the lagoon; the drone teams' hovering drone 7 m up, 25 to 28 m from the road, also in the boss fight).
- Looks wrong: SM_P3A_Kerb_C had nothing between its people and the road, and its pram slid towards the road.
- Weak: unlit kerb groups read as dark clusters on splashdown and the funhouse jump's run-out; nobody readable on
  the right at 3.63 to 3.75 km; limbs with the cast's small steady glow moving faster than 2 Hz at 26 to 29 m;
  SM_FarBalloons beside the boss fight (older rule, 75 m out); cream pies on the pie contest's table; a red X in
  the treasure hunt; ten rules that can never take a slot; movement of pass 3 seen for one piece only.

### Fixed

- SM_FarDucks: yaw dice 15 -> 0 (both rules of FarLife): the flight line stays parallel to the road.
- Lagoon, both sides: the p3c high ways (Skywalk_Hi, LogRide_Hi) moved from 62-64 m to 78-80 m.
- Lagoon, left: the p3d lighthouse and mast moved from slots 1 and 4 (mod 6) at 64-66 m to slots 0 and 3 (mod 6)
  at 68-69 m, behind the first pass's look-outs.
- Lagoon, right: the three p3a rail gang rules taken out (the p3b tower stage keeps those slots).
- Lagoon: SM_P3F_LagoonTower_B is no longer placed; SM_P3F_LagoonTower_A (the camera tower) stands on its slots.
- Drone teams (generator `_drone_team`, both meshes made again): the hovering drone stands on a steel pole and only
  rocks; variant A's second, circling drone is gone. Log: DroneTeam_A 1892 triangles, top 5.73 m, TALL;
  DroneTeam_B 2108 triangles, top 5.73 m, TALL.
- SM_P3A_Kerb_C (generator `kerb_c`, mesh made again): a rail along its front edge like Kerb_A's; the pram rolls
  along the road (axis 90). Log: 1620 triangles, top 1.8 m, low.
- Splashdown: the p3a kerb pieces taken out (P3AHill() in that zone).
- p3b: the rule for slot 554 deleted (the last slot is 553).

### Not fixed

- Nobody readable on the right at 3.63 to 3.75 km: every right slot there already has a tower of the first pass,
  p3b or p3f at 51 to 57 m; adding more would stack them again. Left as it is.
- Dark kerb groups on the funhouse jump's run-out and the lift hill: nothing nearer than 24 m may glow, and new
  colours would be a redesign of four meshes.
- Glowing limbs faster than 2 Hz at 26 to 29 m (Photo, Walk_A..C, Applause_B and older pieces): the glow is
  steady and small (0.07 to 0.10), no lamp is on a fast part; the reader's own verdict was no change unless the
  definition is meant literally.
- SM_FarBalloons in the Last Ride: an older rule, 75 m out, not a broken rule; the dressing itself places
  SM_Balloons beside the road.
- The pie contest's pies and the treasure hunt's red X: the reader asked for no change.
- The nine other rules that take no slot (older rules, and PplP3APainters K 25 before 10.9 km): harmless.
- SM_P3F_LagoonTower_B was not changed and is now unused.

### Run after the fix

- Compile succeeded. Level started once (`C:\ftwork\out\p3\fixed_L4`):
  `FTTRACK level=4 frames=12518 route=673f0081 solid=3e2a2030 solidTriangles=987216`, equal to the baseline line;
  `FTDRESS level=4 districts=228 sets=60 pieces placed=13163 of 13266 tried` (before: 13169 of 13283).
  0 "Fatal error", 0 "LogFT: Error", 0 failed /Game/People loads. One handled renderer ensure (DoubleFloat.cpp
  line 19) before the level loaded, as the driver saw in two of his runs.
- The start script's `waitfor FT CLOCK level=6.0` timed out because this run logged 6.1 and never 6.0; the level
  ran on to about 150 s of clock before the shot and the quit.
- FTCLEAR after the fix: RailGang -2/~18/18 (was -4/~29/35), LagoonTower_A -5/~20/34, Lighthouse -2/~2/42,
  CrowsNest -2/~0/41, FarDucks -8/~0/42 (was -10/~0/40); Skywalk_Hi and LogRide_Hi have no entry any more.

### Never checked

- No frame of any fix was judged: the un-stacked lagoon, the drones on poles, Kerb_C's rail, splashdown. The one
  frame of the run (`L4_t006.png`, in the lagoon) was opened only to see that the level draws.
- Whether the moved pieces now stand in a dressing piece (carousels at 55 to 120 m on the left, tents at 60 to
  135 m on the right, the sky ride at 86 m, the coasters end on from 52 m) or in the p3e pieces.
- Whether the p3f towers at 53 to 57 m stand in the dressing's bandstands (44 to 58 m, slots 1 mod 3).
- Road the driver did not drive: 4.35 to 6.8 km, 7.7 to 8.1 km, 13.8 km to the end.
- Movement of every pass 3 piece but the parachute team; frame rate and memory.
