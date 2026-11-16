# L4 Theme Park, the watchers (extra worker "w"), 15 Nov 2026

Branch `claude/people-L4-w`, worktree `C:\ftwork\wt\L4w`. Generator `Content/Python/ft_people_park_watch.py`,
meshes `/Game/People/ThemePark/SM_Watch*`, rules: the `watch` block of `Source/FullThrottle/FTPeopleL4.inl`.
In my game runs only the dressing and these six meshes were there (not the main worker's cast, not the crew).

## What a player now sees

Behind the rows of stalls, over their roofs, the park's guests have crowded onto whatever is high: a queue bridge
with a striped parapet and a line of people holding flags, signs and foam hands; a food court's terrace under
parasols with a giant striped cup on its corner; a look-out tower of three balconies with a flag on top. They are
there from the first second of the level (a bridge on the left and a terrace on the right at 45 m, a tower at 90 m)
and come round again along the Midway, Pirate Cove, Rocket Land and the Last Ride.

## The meshes (all built by `ft_people_park_watch.run()`; triangles from the generator's log)

| mesh | what it shows | function | triangles | low / tall | what moves | where the table puts it |
|---|---|---|---|---|---|---|
| SM_WatchBridge_A | queue footbridge 6.2 m up, 11 guests at a striped parapet, 7 on a step behind, 4 pennants | `bridge_a` | 2360 | tall (10.4 m) | hops, waving arms, flags, a filming figure that turns, pennants swing; react | Site 37.5 m behind stall rows (43.5 m in Rocket Land) |
| SM_WatchBridge_B | bridge with a striped pavilion roof, 11 guests, two hold a striped banner up | `bridge_b` | 1896 | tall (11.4 m) | the same; the banner hops with its pair | the same slots, turn about |
| SM_WatchTerrace | food court terrace on columns, 9 guests at the rail, 3 parasols, a giant cup, a waiter | `terrace` | 2528 | tall (10.0 m) | hops and waves, parasols sway, the waiter travels the deck | the same slots, turn about |
| SM_WatchPlatform | a coaster's station: a held train with 6 riders, 6 guests at the platform's rail above | `platform` | 2204 | tall (10.1 m) | riders' arms, the guests' hops and waves | the same slots; by Far at 42-43 m in the lagoon |
| SM_WatchTower | look-out tower, three balconies (4.5, 7.6, 10.7 m), 4 guests on each, a flag | `tower` | 1984 | tall (18.0 m) | hops and waves, the flag swings | Site 47-48 m on free slots; Far 55-60 m at the jump, the lift hill, the rocket loop |
| SM_WatchBleachers | show bleachers, 30 light figures on 5 steps, a wave runs along, top rows lift cards | `bleachers` | 2400 | tall (10.8 m) | everyone hops in a wave, some arms wave, pennants | Far 47-88 m at 1.15 to 1.45 size |

Placed (game log, final run): FTDRESS level=4 placed 10053 of 10094 tried. The main worker's empty-table run of the same
dressing gave 9785 of 9822, so this block adds 268 placed of 272 tried. Per mesh the log lists only meshes with a piece
left out or pushed: SM_WatchBleachers 92 placed (4 left out, 19 pushed out from the barrier), SM_WatchPlatform 39 placed
(7 pushed). The other four are not in that list (nothing left out or pushed): their own counts were not read.
FTTRACK level=4 frames=12518 route=673f0081 solid=3e2a2030 solidTriangles=987216 in every run, equal to the baseline.

## What I looked at

Four game runs (the first one's frames fell at 150 s steps because its `waitfor` lines never matched), cropped and
enlarged. Contact sheet `sheets/L4_watch_1.jpg`: bridge A and a tower at the start (left); the terrace at the start
(right); bridge B behind a stall row on the Midway's approach; the terrace and a tower on the Last Ride (boss run-in);
the tower in three film frames; the editor close-ups of the other four. The first four crops are from the second
build (glow 0.09, parapet 80 cm); the tower strip is from the final build (glow 0.14, parapet 58 cm).

Fixed after looking: bridge A was sparse (11 people on 18 m): shortened to 15 m and given a back row; people were dark
specks against the lit stalls: the glow was raised twice; the parapet hid the shirts: lowered from 80 to 58 cm.

## Still weak

- People are still small at 37 m and darker than the stalls' lamps: the structures (parapet stripes, parasols, the
  tower) read first, the crowd second.
- Every piece is over the 1,500 triangle guide (1,896 to 2,528): crowds of 12 to 30 at about 100 triangles a figure.
- The tower's two stair flights are plain slabs and look stuck on.
- The bleachers' figures are box people (about 70 triangles each), coarser than the figure builder's.
- Pieces at 37.5 m stand in the same slots as stall rows by design; I did not see one standing inside another piece
  in the frames I cropped, but only a handful of the 268 were looked at.

## Not checked

- SM_WatchPlatform and SM_WatchBleachers were NOT seen in a cropped game frame (only in the editor's close-ups).
- Movement in the game: only the tower's flag was seen to change between film frames; the hops, waves, the wave along
  the bleachers, the waiter's and the banner's movement were not confirmed in game frames. react was not confirmed.
- The final build's bridges and terrace up close (the crops of them are from the build before).
- The lagoon (bleachers and stations on the valley floor), both jumps, the loops and corkscrews, Rocket Land's and
  the Funhouse's pieces with the final meshes, the right side past the start.
- How these sit with the main worker's pieces and the crew (never run together): the main table uses 24.9-32, 41-46
  and 50-78 m; this block uses 37.5/43.5, 47-48 and Far 42-88 m and names its slots, but overlaps are possible.
- The game log has one `Ensure condition failed: OriginX <= OriginMax` (a view transform, at frame 19, before the
  level starts) in my runs; whether a population mesh causes it was not established.

## ASKS

- Lead: when joining the blocks, mine is wrapped in `{ }` and uses `WatchSite`, `WatchFar`, `WAT`, `WDeck` and sets
  named `PplWatch...`, so nothing collides with the main block's `At`, `AtFar`, `PPL`.
- A game script note for other workers: `waitfor FT CLOCK level=8.0|150` often never matches (the clock prints 8.1);
  `waitfor FT CLOCK level=8.|60` does.
