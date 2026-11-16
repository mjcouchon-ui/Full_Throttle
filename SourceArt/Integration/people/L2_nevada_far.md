# L2 Nevada, far life and creatures (extra worker "f"), 15 Nov 2026

Branch `claude/people-L2-f` (worktree `C:\ftwork\wt\L2f`). Generator `Content/Python/ft_people_nevada_far.py`
(`run()` builds all six through `ft_people.export`). Rules: the block `BEGIN far` .. `END far` in
`Source/FullThrottle/FTPeopleL2.inl` (68 Scatter rules in 13 zones; written by a scratch script, not in the repository).
Contact sheet: `sheets/L2_far_game.jpg` (cropped game frames, enlarged 2 to 4 times; the last cell is the editor's plain render).

## What a player now sees

On the open stretches of Highway 93, out beyond the roadside businesses: a line of riders on dark horses crossing a
rock ridge against the sky, a red and white pennant at their head; covered wagons with banded canvas and red, teal
and magenta wheels behind mule teams; gold pronghorn bounding in a band; a mixed herd of mustangs led by a white
horse; longhorns on a trail with two drovers. From the jump at 7.3 km the wagon train is seen from above.

## The meshes (all in /Game/People/Nevada/, all one section, M_PeopleFar except the last)

| mesh | shows | function | triangles | low/tall | what moves | where the table puts it |
|---|---|---|---|---|---|---|
| SM_FarWagonTrain | 3 covered wagons, 6 mules, 2 outriders (pennant, hat), a pack mule, on a 165 m pale trail | `wagon_train` | 3012 | TALL (16.3 m) | everything travels the trail and comes round; legs, wagons rock, dust, pennant and hat arms | 215-272 m at 1.4x; 195-235 at 1.2x; 140-195 in one gap of the red rock return |
| SM_FarRidgeRiders | a 110 m ridge 11 m high, 5 riders in single file on its crest, 5x life size | `ridge_riders` | 1272 | TALL (29 m) | riders travel the crest; legs, hop, pennant, two hat arms | 255-288 m |
| SM_FarMustangs | 14 horses and 2 foals at the gallop on a 215 m swell, dust | `mustangs` | 2520 | TALL (9.9 m) | travel 15 m/s, hop out of step, legs, body pitch, dust | 140-195 m; 185-275 at 1.4x |
| SM_FarCattleDrive | 12 longhorns, 2 drovers waving hats, dust | `cattle_drive` | 2572 | TALL (12.1 m) | travel, legs, hat arms, dust | 140-195 m; 185-272 at 1.2 to 1.4x |
| SM_FarPronghorns | 12 pronghorn bounding, white rumps | `pronghorns` | 1964 | TALL (8.1 m) | travel 13 m/s, leaps of 2.4 to 3.6 m out of step, legs | 140-215 m; 250-280 at 1.4x |
| SM_FarRunners | a 27 m rock ledge 2.4 m high: 3 roadrunners sprinting, 4 jackrabbits bounding | `runners` | 1348 | TALL (5.5 m) | travel, legs, hops, body rock (M_People) | 38-46 m (44-52 in the Syndicate range) |

Placed: 202 pieces in all (FTDRESS placed 8280 with my block, 8078 in the main worker's run with an empty table,
`C:\ftwork\out\L2\drive0`). Per mesh: not counted (the FTCLEAR line lists only meshes that had a piece left out or
moved, and none of mine is in it).

FTTRACK level=2 frames=12888 route=efc95392 solid=e2d27537 solidTriangles=964188: equal to the baseline line, in every run.

## What I looked at (cropped frames of the real game)

Six drives: the start (three times, as the pieces changed), scene 4 (Joshua Flats into Red Rock Pass), scene 6 (Last
Chance Gas, the jump, Jackpot Junction), scene 9 (the boss's run on the salt flats).

- Reads: ridge riders (scene 9 and the start), wagon train (scene 9 from the road, scene 6 from the air), pronghorns
  (the start). Mustangs: a row of animals with white and dark ones among them; at the side of the frame the game's
  motion blur smears them.
- First versions did not read and were changed: herds at 3x life in dark coats were ink blots at 170 m (now 3.3 to
  3.8x, white, gold, cream, black; deeper bodies, thicker legs); wagons were plain white barrels (now banded canvas,
  coloured wheels and bed); ridge riders in light coats were lost in the haze (now dark horses, 5x life); the pale
  trail rendered pure white (toned down); the far pieces of the dry wash, Red Rock Pass and the red rock return were
  behind the 15 m rock cuts and are gone from the table (open stretches only).

## Still weak

- Every piece is over the 1,500-triangle guide except two (table above): massed herds of 100-triangle animals. About
  200 instances in all.
- From the air (the jumps) a herd's swell shows as a long dark-sided slab.
- At 150 m and more the pieces are 10 to 40 pixels tall in a 1080p frame: they read ahead of the car, not beside it.
- Joshua Flats: the trees hide most of the far band; what showed there was coloured specks.
- The animals are 3.3 to 5 times life size. Beside a Joshua tree or a water tank at the same distance that can be seen.
- Mustangs and cattle overlap in idea with the main worker's SM_WildHorses and SM_Cattle (which stand 90 to 140 m out).

## Not checked

- SM_FarRunners in its final colours and size in the game (seen in the game only in its first version, and in the
  editor's render); SM_FarCattleDrive close enough to judge (seen only as a strip of animals from the air).
- Movement: the films show the pieces shifting against the background; limb movement cannot be made out at that size.
- Jackpot Junction to the city lights and the Vegas approach from the road (pieces at 250-288 m behind the casinos);
  the Syndicate range; the two gaps of the red rock return.
- Pieces standing inside dressing pieces at the same distance (dunes, spires, rock walls, the Syndicate's hide out):
  none seen in the frames I cropped, not searched for.
- Together with the main worker's and the watchers' pieces (my runs have only mine and the dressing).
- The log has the engine's own "precision loss ... view transform" ensure once a run; it is in other levels' runs too.
  A distance field is built for my meshes when the game loads them (Scene.save asks for none): not looked into.

## ASKS

- Per-mesh placed counts: FTCLEAR could list every /Game/People mesh, also those with nothing left out.
- Lead: choose between SM_FarMustangs / SM_FarCattleDrive and the main kit's SM_WildHorses / SM_Cattle where both
  land in one stretch (mine start at 140 m, the main's end at 140 m).
- `waitfor FT CLOCK level=11.0` can miss (the clock printed 11.1): `level=11.` is safe. Worth a line in the brief.
- `run_game.ps1` prints the baseline of the FIRST FTTRACK line in the log, which is level 0 (the attract level), not
  the level driven.
