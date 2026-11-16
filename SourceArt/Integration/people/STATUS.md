# The population pass: status (second machine)

Last written: 16 Nov 2026, after population pass 3 (built, reviewed once, fixed once, joined with `main`).
Report rule: only what was run and seen. "Not checked" means exactly that.

## Machine

i9-12900KF (24 threads), 64 GB, RTX 3080 Ti, Unreal 5.9.0 (launcher build), MSVC 14.44.
Branch `claude/people`. A table change compiles in about 12 s; one start of a level takes about 90 s.

## What exists

| What | Where | How it was made |
|---|---|---|
| The figure builder, birds, four-legged animals | `Content/Python/ft_people.py` | `figure()`, `bird()`, `quadruped()`, `Scene`, `export()` |
| The population's material | `/Game/People/Core/M_People`, `M_PeopleFar` | `ft_people.build_core()` |
| The film crew kit, 10 pieces x 8 wardrobes | `/Game/People/Crew/SM_Crew<Piece>_<Wardrobe>` | `ft_people_crew.run()` |
| First and second pass, per level | `Content/Python/ft_people_<key>.py`, `FTPeopleL<n>.inl` | notes `L<n>_<key>.md` |
| Pass 3, six builders a level | `Content/Python/ft_people_<key>_p3<a..f>.py`, meshes `SM_P3<A..F>_<Name>`, blocks `p3a`..`p3f` at the end of `FTPeopleL<n>.inl` | notes `L<n>_<key>_p3.md` |

Keys: bay, chem, nevada, volcano, park, ice, sea, city. `M_PP_Cel`, `ft_life.py` and `M_Life` are untouched.
A population mesh has ONE section (one draw call an instanced mesh); colour and movement are in eight UV channels,
vertex shader only.

## Pass 3

Six builders a level, each with one focus (a gaps, b scenes, c movers, d heights, e far and sky, f film shoot),
nobody reviewing while building. Then one review a level (one reader of the table and generators against the rules,
one driver of the whole level with frames, neither of whom built it), one fix round, one compile, one start.

How it differed from the plan: the six builders of a level shared one built checkout and each wrote a block file;
one integrator a level joined the blocks, compiled and committed. (48 separate checkouts did not fit on the disk.)

Numbers are the game's own `FTDRESS ... pieces placed=` line (dressing and population together), one start of each
level on the joined build with `main` merged (`ecafa7c3` plus this file). The road's `FTTRACK` line equals
`track_baseline.txt` on all eight; no `Fatal error` and no `LogFT: Error` in any of the eight logs.

| Level | New meshes | Placed before pass 3 | After the build | After the fix round | Review: fixed / left |
|---|---|---|---|---|---|
| 0 Bayshore | 70 | 11,608 | 13,641 | 13,415 | 7 / 8 |
| 1 Chemical Works | 67 | 10,460 | 11,932 | 11,878 | 15 / 12 |
| 2 Nevada | 71 | 9,404 | 11,676 | 11,589 | 12 / 7 |
| 3 Volcano | 65 | 9,828 | 11,810 | 11,436 | 10 / 11 |
| 4 Theme Park | 71 | 10,951 | 13,169 | 13,163 | 10 / 7 |
| 5 Ice Mountain | 73 | about 9,600 (not read from a log line) | 11,896 | 11,796 | 14 / 7 |
| 6 Under the Sea | 70 | 11,072 | 13,521 | 13,377 | 10 / 8 |
| 7 City Center | 78 | 8,283 | 10,406 | 10,018 | 8 / 11 |

565 new meshes. No builder's block was dropped; every level compiled at the first try.

### What the review found that broke a rule, and what was done

- Camera drones circling or hovering near the road (levels with enemy helicopters): on every level the circling was
  taken out of the generators or the drone teams were replaced or moved; Bayshore's drones are smaller and only bob.
- Bayshore: parachutists read as the helicopter's dropped men: all parachute rules off; paragliders off in every
  second district.
- Volcano: the airship read as a missile: all its rules removed; no balloons or kites over the boss's road.
- Ice Mountain: snowballs in flight removed; dark circling birds recoloured; a snowmobile in the patrol's colours and
  an orange pickup-sized balloon recoloured.
- Chemical Works: steady light nearer than 24 m on three first-round pieces removed; a far flock that could pass over
  the road moved out; orange lit lanterns that resembled a pickup redrawn taller and pale.
- Volcano, City Center: lit limbs moving faster than 2 a second inside 30 m slowed.
- Theme Park: a far duck flight whose path could cross the road straightened.
- Everywhere: pieces of different builders standing in the same spot were shared out or moved (details in each note).

### Left as found (each note lists its own)

- Most pass 3 meshes are over the 1,500-triangle guide (scenes and press pens 3,000 to 3,700).
- Banked bends, loops, wall rides and jump approaches still have nobody near: the track clears pieces there.
- Low kerb figures show as heads and shoulders over the barrier from the near lane.
- Chemical Works: clapping arms and dogs at 2.4 to 2.6 a second on first-round lit pieces inside 30 m were reported
  as a broken rule and NOT fixed in this round.
- Some overlaps remain between pass 3 pieces and first-pass pieces (counts in the notes).
- Older meshes placed more than 60 times (first passes' rules) were not thinned.
- The player's car drawn as four wheels after jumps was seen by several drivers; it is not population and was not touched.

## Not checked

- Performance of any kind after pass 3 (frame time, draw calls, triangles, memory, load time).
- Movement: no reviewer could judge it from films taken at racing speed; no standing-camera run was made.
- The joined build was started once a level (6 s of driving each); the whole-level drives were made on the level
  checkouts before the fix round, and each fixer started its level once after it. Nobody drove a whole level after
  the fixes.
- Vertex movement past 180 m: `main` now switches it off for population and life components (`FTWPO` line; Bayshore
  reports 112 of 216 components). How that looks on far pieces was not looked at.

## ASKS

- None.
