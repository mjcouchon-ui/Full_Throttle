# Level 6, Under the Sea: far life and creatures (extra worker, focus "far")

Branch `claude/people-L6-f`, worktree `C:\ftwork\wt\L6f`. Generator `Content/Python/ft_people_sea_far.py`; meshes
`/Game/People/UnderTheSea/SM_Far*` (6); rules: the block `far` in `Source/FullThrottle/FTPeopleL6.inl` (43 Scatter rules,
6 zones). Contact sheet: `sheets/L6_far_game.jpg` (crops of real game frames, enlarged 1.5 to 2 times).

## What a player now sees

Out in the blue, 150 m and more from the road, the water is inhabited by big things: a coral-red giant squid with its
arms trailing; shoals of striped nautilus shells hanging at different heights; orange spider crabs as tall as houses
walking the trench wall with their claws up; a white-and-pink ribbon of a sea dragon (an oarfish) with a red crest;
a dome of Nereida going up under a yellow crane, with aquanauts on its ribs and on a rope up to a buoy; and flights of
aquanauts on scooters trailing striped streamers. Each district has its own mix (reef: nautilus, squid; kelp: crabs,
dragons; station: dome works, scooter wings; salvage: squid, crabs; approach: wings, domes; Nereida: all, thinly).

## The meshes (all made by `ft_people_sea_far.py`, all saved with far=True, all TALL and all placed 150 m or more out)

| mesh | shows | function | triangles | what moves | where (m from centre line), scale |
|---|---|---|---|---|---|
| SM_FarSquid | one giant squid, 43 m as built | `squid` | 1048 | travels 60 m of X and comes round; 8 arms and 2 tentacles swing, bubbles float | 175-215, x2.2 |
| SM_FarDomeWorks | a dome being built: crane, 5 aquanauts on the ribs (2 with a steady welding light), 4 on a rope to a buoy, 3 scooters | `dome_works` | 3060 | crane jib swings, aquanauts float and kick, scooters circle the dome, buoy and bubbles float | 170-225, x1.5 |
| SM_FarNautilus | 6 giant nautiluses | `nautilus_shoal` | 1488 | each rises, sinks and rocks in its own time; arms swing | 150-200, x2 |
| SM_FarCrabMarch | 4 giant spider crabs walking sideways | `crab_march` | 2104 | travel along X; legs work in turn, claws wave, bodies hop | 205-270 (the trench wall), x2.5 |
| SM_FarSeaDragon | two oarfish, 60 m and 35 m as built | `sea_dragon` | 1408 | travel along X; a wave runs down the ribbon (each slab floats behind the last) | 170-210, x1.8 |
| SM_FarScooterWing | 7 aquanauts on scooters in a vee, 4 times life, streamers | `scooter_wing` | 1596 | travel along X; legs kick, streamers swing, each floats | 170-210, x1.8 |

Placed: 190 pieces in all (final check after the last rebuild of all six meshes: FTDRESS level=6 `pieces placed=9787 of 9799 tried` with this table; the main worker's run with
the dressing alone, `C:\ftwork\out\L6\base\run.log`, has 9597). Per-mesh counts: not counted (the FTCLEAR list in my
runs' logs does not name these meshes; the first test, 4 rules every 8 slots, gave +210, the second, 10 rules every 12,
gave +355, which is what the rules should give).

FTTRACK level=6 in every run: `frames=12470 route=04d3f899 solid=8927e339 solidTriangles=930148`, equal to the baseline.

## What was learned (for the lead)

- The chase camera looks ahead: a piece 180 m to the side is seen 300 m and more away, through this level's haze. At
  the size built (a 43 m squid) the first test showed a 26-pixel smudge and a yellow crane gone dark green. What fixed
  it: placing at about twice the size (the rule's scale), a small steady light of the part's own colour on every
  coloured part (`lit()`, glow 0.15: no blinking, and nothing nearer than 150 m), and height (20 to 60 m up).
- A part's float and swing use its own rate and phase, so a creature whose limbs swing at their own rate cannot also
  float as a whole (limbs would leave the body). Whole-creature movement here is travel or orbit.
- A travelling piece scaled 2.2 with a long travel and 20 degrees of yaw came right over the road in the second test
  (a tentacle across the top of the frame). Travel was shortened (60 to 90 m as built) and the yaw held to 6 degrees.

## Still weak

- The nautilus reads as a round striped shell with a red eye; from far it can look like a fried egg. It needs a
  clearer spiral.
- The squid's arms and the crane are stiff boxes up close in the plain renders; from the road they read.
- The squid glows a strong orange at the start (first curve, left): bright for a "far" thing. Glow could come down to 0.1.
- SM_FarDomeWorks (3060) and SM_FarCrabMarch (2104) are over the 1,500 guide: eleven and four big figures of many parts.
- Pieces scaled 2 to 2.5 look nearer than they are; crabs at 205 m and more read as if they stood close behind the barrier.
- In the boss fight (scene 7) wings' red-and-white streamers and the dragon cross the upper frame; not judged against
  the boss's attack warnings.

## Not checked

- Movement in detail: films of 8 to 16 frames were taken while driving; the sea dragon's wave and the nautiluses'
  heights differ from frame to frame, but the camera moves too. Not checked from a standing camera. Travel wrap-round
  (a piece vanishing at one end and coming in at the other) not seen in frames. The crane's swing, the scooters' orbit
  and the crabs' legs: not confirmed in frames.
- Scenes 2, 3, 5 and 6 of the level: not driven (scenes 1, 4 and 7 were, 33 s each, default lane).
- Whether any piece stands inside a dressing piece far out (hubs and domes at 110 to 285 m): not checked piece by piece.
- Together with the main worker's and the watcher's pieces: not run (only this block and the dressing).
- Frame rate, shadows, the packaged build: not checked.
- The log has `Ensure condition failed: OriginX <= OriginMax` (DoubleFloat.cpp) once in the first run; the same
  line is in other levels' logs (`C:\ftwork\out\L0\s1\run.log`); cause not looked into.

## ASKS

- The main kit (`ft_people_sea.py`) already has SM_Turtles, SM_Dolphins, SM_Octopus, SM_JellyGiants, SM_SeahorsePatrol,
  SM_CrabRow and SM_FarConvoy, so this kit made none of those; if the far worker should own them, say so.
  Its SM_FarConvoy carries the `SM_Far` prefix that the extra brief gives to this kit.
- `run_game.ps1` prints no count for population meshes whose rules are Scatter rules in my runs (the FTCLEAR line does
  not name them): a per-mesh placed count would help.
