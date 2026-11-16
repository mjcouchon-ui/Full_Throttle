# Level 5, Ice Mountain: far life and creatures (extra worker "far", 15 Nov 2026)

Branch `claude/people-L5-f` (worktree `C:\ftwork\wt\L5f`). Generator `Content/Python/ft_people_ice_far.py`; meshes
`/Game/People/IceMountain/SM_Far*`; rules: the block `far` at the end of `Source/FullThrottle/FTPeopleL5.inl`.
Contact sheet: `sheets/L5_far_game.jpg` (cropped game frames).

## What a player now sees

Past the edge of the land (305 to 330 m out, on both sides, all 25 km) the mountain has life on it, 5 to 11 times
life size so that it reads from the road:

- dark rock faces with a white switchback piste: skiers and snowboarders in hot suits come down the upper leg one
  way and the lower leg the other, out of yellow-and-teal start arches, past red and blue gates (the top, the first
  pipe, the valley, the resort, the last lifts);
- dark rock walls with three roped parties climbing, a hanging camp, a climber swinging across on a rope and two
  people waving a flag on the summit (the chute and Black diamond, the glacier, the ice again);
- a herd of ibex leaping along a rock arete (Black diamond, the whiteout, the ridges);
- a wolf pack on a snow crest: three howling on a rock, five running the skyline (the whiteout, the glacier, the ridges);
- three dog teams racing along a snow crest between flags (the top, the valley, the resort, the last lifts);
- great eagles wheeling over all of it, on both sides, the whole way down.

A side has one of these every 360 m, the two sides out of step, and a group of eagles between them. A district has
two kinds of piece and they change with the district.

## The meshes (all by `ft_people_ice_far.py`, all saved with far=True, all TALL, all placed by Far rules)

| Mesh | What it shows | Function | Triangles | What moves | Where (m from the centre line) | Scale |
|---|---|---|---|---|---|---|
| SM_FarSkiFace | switchback piste on a rock face, 12 skiers and boarders, gates, two start arches | `ski_face` | 3076 | every slider comes down its leg and round again (swing and travel on one clock) | 305 to 330, foot 20 m over the road's level | 1.6 |
| SM_FarClimbWall | rock wall, 7 climbers on three ropes, a swinging climber, a hanging camp, two on the summit with a flag | `climb_wall` | 1728 | climbers inch up and down, one swings on his rope, the camp sways, the summit pair hop and wave, the flag flies | the same | 1.6 |
| SM_FarIbexArete | 9 ibex leaping along a rock ledge under spires | `ibex_arete` | 1884 | legs stride, each leaps (hop 2.3 m as built) and travels the ledge and round again | the same | 1.6 |
| SM_FarWolfCrest | 3 wolves howling on a rock, 5 running a snow crest | `wolf_crest` | 1228 | heads nod, tails wag; runners stride, bound and travel | the same | 1.6 |
| SM_FarSledRace | 3 dog teams (4 dogs, a sled, a musher, a pennant) on a snow crest, flags at the ends | `sled_race` | 2438 | dogs stride and bound, teams travel the crest and round again, pennants and flags fly | the same | 1.6 |
| SM_FarEagles | 5 eagles, 14 m across the wings as built, 60 to 110 m up | `eagles` | 580 | each circles (two the other way round), slow wing beats | 240 to 420, over the land and past it | 1.4 |

Counts: the table has 60 Far rules (10 districts x 6). The game's `FTDRESS level=5 ... pieces placed` line read 8298
with this table. Far pieces are not in the FTCLEAR list, so the count of mine is by difference, not read directly:
see "not checked".

## How it was made, and what was found

- Downhill movement is the main kit's trick (a swing about a pivot 600 m underground on the same clock as the
  travel), written out again in this kit: a worker does not import another worker's file.
- At its built size, white on white, nothing read: the first frames showed specks. What made it read: every piece
  brings DARK rock (the piste is a white band on a dark face, the crests have a dark foot), the figures are 5 times
  life size as built, and the rules lay every piece at 1.6 (eagles 1.4).
- A slider climbing back inside the hill showed an arm through the hill's road face when its lane was near the
  face: the lanes are 6 m and more back from it now.
- The summit flag was red with a yellow band at first (too near a country's flag): it is pink with a teal block.

## Over budget, and why

SM_FarSkiFace (3076 triangles) and SM_FarSledRace (2438), SM_FarIbexArete (1884) and SM_FarClimbWall (1728) are over
the 1,500 guide: each is a whole far mountain scene with 9 to 14 creatures, and there are about 30 of each.
