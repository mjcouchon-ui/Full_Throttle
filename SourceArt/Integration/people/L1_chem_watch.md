# L1 Chemical Works: the watchers (extra worker "watch", 15 Nov 2026)

Branch `claude/people-L1-w` (worktree `C:\ftwork\wt\L1w`). Generator `Content/Python/ft_people_chem_watch.py`; table block
`watch` in `Source/FullThrottle/FTPeopleL1.inl`; meshes `/Game/People/ChemicalWorks/SM_Watch*`.
Contact sheet: `sheets/L1_watch_game.jpg` (eight cropped game frames, then the six meshes close up under plain light).

## What a player now sees

The night shift has downed tools to watch the chase. Forty metres from the road, over the pipe racks and the tank rows,
stand works grandstands run up out of scaffold: stepped decks 7 to 13 m up, white lit sheeting behind every row, two
dozen of the shift in orange, lime, yellow and pink hi-vis on each, flags in the front row and a striped banner under
the first deck. Between them mast lifts hold a cage of three or four workers and a pink flag 14 to 17 m over the
plant, and control blocks have the office staff and the red-capped guards on the balcony and a line of people along
the roof edge. Further out (70 to 110 m) pale storage tanks have a crowd on the roof against a lit sheet, two great
flags, and more people on the stair landings.

## The meshes

| mesh | shows | function | triangles | top | moves | where the table puts it | placed |
|---|---|---|---|---|---|---|---|
| SM_WatchStand_A | scaffold grandstand, 4 decks (7, 9, 11, 13 m), 26 people | `stand(1)` | 3308 | 17.7 m TALL | every person hops on its own beat, arms wave, 2 flags swing, banner stirs; react | Scatter 40.5 m, alternate tenth slots, both sides | 53 |
| SM_WatchStand_B | longer stand, 3 decks (7.6 to 12 m), 25 people, orange and lime banner | `stand(7, ...)` | 3404 | 16.7 m TALL | as A | Scatter 40.5 m | 49 |
| SM_WatchLift_A | mast lift, cage 16 m up, 4 workers, a flag | `lift(2)` | 1536 | 20.8 m TALL | workers hop and wave, the flag swings; react | Scatter 40 m | 56 |
| SM_WatchLift_B | two mast lifts (13.5 and 17.5 m), 3 workers each | `lift(9, ...)` | 2592 | 22.3 m TALL | as A | Scatter 40 m | 50 |
| SM_WatchRoof | control block: 6 staff and guards on the balcony (4.5 m), 11 along the roof (9 m), lit window band, banner | `roof(4)` | 2112 | 14.2 m TALL | people hop and wave, banner stirs; react | Scatter 40.5 m | 61 |
| SM_WatchTankFar | storage tank 22 m tall, 16 people on its roof (2.4 times life size), 4 on the stair landings, two great flags; always-moving master (far=True) | `tank_far(5)` | 2056 | 29.4 m TALL | crowd hops, arms wave, flags swing | Far 70 to 110 m, every 12th slot, both sides, 1.2 m under road level | 91 |

"Moves" is what the generator writes into the mesh; see "Not checked" for what was confirmed in the game.

Placed in the last game run (FTCLEAR): 360 pieces (269 at 40 m, 91 far tanks); left out by the track's own clearance: 31.
`FTDRESS level=1 ... pieces placed=9396 of 10245 tried` with this block alone (the main worker's and the far worker's
rules are not in this worktree). `FTTRACK level=1 frames=12792 route=09fbdb9f solid=a2d79366 solidTriangles=950948`
in all three runs: equal to the line in `track_baseline.txt`.

Rules: nothing nearer than 40 m (all pieces are tall); steady light only (work lamps 0.8, lit sheeting 0.2, the
people's own 0.07 to 0.22, one phone screen on the roof block's balcony); nothing blinks. Stripes and blocks only on
banners and flags. The stands and lifts take phases of the dressing's own 5-slot and 6-slot periods that its blow-down
stacks (41 to 43 m) and light masts (37 to 40 m) do not use.

## Runs and what was looked at

- Mesh job twice (all six, then the lifts and the tank again after the fixes), close-up sheets looked at.
- Three game runs, cropped and enlarged: a drive from the start (frames from scenes 1 to 7: its clock waits timed out
  on the loaded PC, so the frames fell where they fell), a hop through scenes 2 to 8 on alternate sides with short films,
  and a third run (start, scenes 4, 6, 8, films) after the fixes.
- Seen reading from the road: Stand_A and Stand_B (left and right, over a tank row, from a raised road, in the finale),
  Roof (right side, scene 6), Lift_A / Lift_B (from below at the start; in the finale after the fix), TankFar (scene 4
  and the finale).
- Fixed after the first look: the lift's workers were hidden by the cage floor from the low camera and dark (moved to
  the front edge, shallower floor, more of their own light); the far tank was 14 m and not found in any frame (now 22 m).

## Still weak

- Over the triangle guide (about 1,500 a piece) on all six: 1,536 to 3,404. The stands are masses of 25 people; the
  back rows are already 48-triangle block figures. Say if they must come down.
- Behind a tank row (11 m) only the upper two decks of a stand show from level road; the lower decks and the banner
  show from raised or banked road.
- The lift's cage is seen from below at 40 m: it reads as "people in a raised cage with a flag", not as faces.
- The far tank's second stair landing stands off the tank's curve on its own post.
- Stand_A / Stand_B at 40.5 m are 6.8 / 5.1 m deep (37 to 44 m): where the dressing has a tank row at 34 m (to 37.8 m) or
  a kiln at 46 m (from 44.2 m) the gap is under a metre.

## Not checked

- Crowd movement frame against frame in the game: the films were taken while the camera moved at speed; only the
  lift's flag was seen at different angles in successive frames. The hop, the waves and the banner are not confirmed
  from game frames. The sheets cannot show movement.
- react (livelier as the chase passes) not checked.
- Every district: scenes 1 to 8 were visited for a few seconds each; no frame-by-frame pass of the 25 km.
- Standing inside dressing: no stand, lift or roof block was seen inside another piece in the frames looked at, but
  the far tanks (Far rule, 70 to 110 m) can land inside the first pass's sets (TankFarm, Gasworks, ReactorBlock...):
  not checked beyond the three tanks seen.
- Together with the main worker's and the far worker's pieces (their rules are not in this worktree): not checked.
  The main table has SM_Gantry_C at 38.5 to 39.5 m in the tank farm (every 3rd slot, phase 2): it can share a slot
  with a stand or a lift there.
- The second lap of the doubled circuit (22.18 to 25.58 km), the flare avenue (left empty on purpose), performance.
- The log's `Ensure condition failed: OriginX <= OriginMax` appears once in each run, between the level 0 and the level
  1 track builds; the same line is in the other workers' logs (L0, L0w, L1, L1f). Whether it has to do with these
  meshes: not checked.

## ASKS

- Lead: when joining the blocks, the `#define WATCH` / `#undef WATCH` pair travels with the block.
- Lead: if the main table keeps SM_Gantry_C at 38.5 to 39.5 m in the tank farm, say which of us moves.
- The figure builder has no light of its own on a figure: this kit (like the main one) wraps a piece to give its moving
  parts a little glow. A `glow` value on `P.figure` would save that.
