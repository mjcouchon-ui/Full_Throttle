# Level 4, Theme Park: the population, first round (15 Nov 2026)

Branch `claude/people-L4`. Generator `Content/Python/ft_people_park.py`, meshes `/Game/People/ThemePark/` (16),
table `Source/FullThrottle/FTPeopleL4.inl`. Only what was run and seen is reported here.

## What a player now sees

The park has a show on. A parade goes along both sides of the road 25 to 27 m out: a striped cake float with dancers,
a giraffe mascot and a great candle; stilt walkers, flag bearers, ribbon dancers and a frog mascot on foot; the band's
wagon with a hippo conducting; a whale the size of a bus on four ropes walked along by the staff. Mascots meet
families on podiums under rainbow arches. Over the Midway's stalls and across the plazas a high wire, 8 m up, carries
two walkers. Behind the stalls stand show stages (a kick line, a trampolinist), a dog show, stands of guests waving at
the chase under strings of lamps, switchback queue towers to a slide, pony rings. Staff in mint and pink hang bunting
from cherry pickers. Doves wheel over the stalls and over the coaster; open-air theatres full of people stand 140 m
out. The shoot is there too: a camera tower, crane, lights, dolly, village and catering at the start, second units at
both jumps, stands and towers through the corkscrews and loops, crew at their tape along the way and before the
arena's hill.

## What decided the design (seen in the first frames)

- The level is at NIGHT and everything in it is lit. Plain-coloured figures stood in the stalls' shadow as dark shapes
  (first line-up run). Every piece of this kit now carries a small steady glow (`lift()`: 0.03 to 0.06, about a third
  to a half of the colour) and rows of steady lamps (float decks, stage fronts, stands, podiums). All of it stands
  24 m or more from the centre line; nothing blinks. After that the stage, the high wire, the floats and the stands
  read in the frames.
- The verge is a hoarding 1.8 m high on every other slot behind a metre of barrier: people on the ground are hidden
  from a straight. Everything here is raised (decks, stilts, masts, ramps, steps, scaffold) and every one of the 16
  meshes is TALL (24 m or more). The only low pieces placed are the crew kit's.
- MR. CHUCKLES flies under a bunch of balloons and bombs the road with them (seen in the boss frames). The kit
  had balloons (a bunch on the cake float, a stilt walker's, a balloon arch): all removed. The float carries a
  candle, the arch is a rainbow of bands. No clown anywhere in the kit. The one inflatable left is the whale
  (a judgement: it is cyan, 6 m long and not round; say so if it should go).

## The meshes (all made by `ft_people_park.run()`; all TALL; triangles and tops from the generator's log)

| Mesh `SM_Park...` | Shows | Function | Triangles | Top m | What moves | Where the table puts it | Placed |
|---|---|---|---|---|---|---|---|
| Parade_A | the cake float: 3 dancers, the giraffe, a candle | `parade_a` | 2090 | 6.9 | the float travels its lane and comes round; dancers hop and wave; giraffe waves, head nods; flame sways | lane 26 m | 33 |
| Parade_B | walkers: frog mascot, 2 flag bearers, 3 stilt walkers, 2 ribbon dancers | `parade_b` | 2272 | 4.1 | all walk 16 m and come round; legs, stilts, arms, ribbons swing | lane 26 m | 34 |
| Parade_C | the band's wagon: drum, horns, cymbals, the hippo conducting | `parade_c` | 2320 | 4.2 | travels; players rock, drum arm beats, cymbals clap, hippo hops and waves | lane 26 m | 35 |
| Parade_D | the whale on four ropes, 4 staff, 2 children | `parade_d` | 1444 | 7.7 | travels; whale floats up and down; handlers walk, children run | lane 26 m | 34 |
| HighWire | two masts, a wire 8.2 m up, 3 performers, net, 6 guests | `high_wire` | 1968 | 10.8 | one walker rocks with the pole, one slides to and fro under a parasol, one waves; guests hop | 27.3 m over the Midway's crowd; 41 to 46 m elsewhere | 46 |
| Stage_A | show stage: kick line of 4, trampolinist, frog compere | `stage_a` | 2140 | 7.7 | legs kick in turn, trampolinist goes up 2.6 m, frog waves | 52 to 70 m | 52 |
| Stage_B | dog show: 3 dogs and hoops, trainer, staff, a child | `stage_b` | 2064 | 5.3 | dogs hop 1.5 m one after another, tails wag, staff claps | 52 to 70 m | 46 |
| Meet_A | giraffe on a podium, rainbow arch, 2 children, family on the ramp, photographer | `_meet('giraffe')` | 2112 | 5.1 | mascot waves and livens up as the chase passes, children hop | 29.8 m | not in the log's list |
| Meet_B | the same with the hippo | `_meet('hippo')` | 2056 | 3.8 | as Meet_A | 29.8 m | not in the log's list |
| QueueTower | two ramps, a station, a slide's chute; 9 guests, staff, a child on the chute | `queue_tower` | 1840 | 7.5 | guests shuffle and wave, staff waves, child slides up and down the chute | 46 m (lift hill), 63 to 64 m | not in the log's list |
| Grandstand | 16 guests on 4 steps under an awning with lamps, 2 flags | `grandstand` | 2184 | 5.9 | all hop, wave, clap out of step; livelier as the chase passes | 46 to 82 m, through corkscrews, loops and at both jumps | 98 |
| CherryPicker | 2 staff in the basket hanging bunting, 1 on the ground | `cherry_picker` | 1520 | 8.5 | boom and basket rise and fall, ground staff waves | 28.5 to 29 m | not in the log's list |
| PonyRing | 4 ponies with child riders round a raised ring, staff, 2 guests | `pony_ring` | 2212 | 6.5 | ponies and riders circle, legs stride, tails wag | 57 to 62 m | not in the log's list |
| FarCrowd | open-air theatre: 70 block people, roof, steady lamps (always-moving master) | `far_crowd` | 2292 | 10.2 | a wave of hops runs along the stand | 136 to 150 m at 1.5 size | not in the log's list |
| Doves | 10 doves 2.6 m across, 9 to 16 m up (always-moving master) | `doves` | 940 | 16.3 | wings beat, each circles at its own speed | 60 to 130 m; over the lagoon 22 m above the valley floor | 95 |
| CrewTower | the shoot's camera tower: scaffold, panning camera and operator, assistant, 2 crew cheering, crew tape | `crew_tower` | 2116 | 5.0 | camera and operator pan, assistant points, crew hop and clap | 26.6 m; 40 to 69 m by Far rules | not in the log's list |

Crew kit placed (wardrobe Park): ApplauseA 51, ApplauseB 54, Stunt 3 (from the log's list); Camera, Village, Dolly,
Crane, Lights, Catering are in the table and not in the log's list.

**Counts.** `FTDRESS level=4 ... pieces placed=9785 of 9822` with the people table empty; `10684 of 10726` with it:
**899 population pieces placed** (crew included). The game's FTCLEAR line lists a mesh only when one of its pieces was
moved or left out, so the "Placed" column has a number only for those; the rest add up to 899 - 615 = 284 and were
not counted one by one. Left out by the track: Stage_A 1, Stage_B 1, Grandstand 2, Doves 1. Moved off the sight
line: Parade_A 2, B 1, C 1, D 6, HighWire 6, Stage_A 2, Doves 8, ApplauseA 8, ApplauseB 11, Stunt 3.

**Over the guide.** 13 of 16 meshes are over the 1,500-triangle guide (1,840 to 2,320): a figure costs more than
the estimate (about 130 at detail 0, 250 to 300 at detail 2) and these are scenes of 6 to 16 people plus their stage.
Not reduced further in this round. 16 meshes: at the cap. 899 pieces: at the cap.

## Districts (the table's own comments give the slots)

Start 0 to 0.3 km: the shoot. Front Gate: parade on slots 0 (mod 3), podiums, high wire and cherry pickers on 2
(mod 3), stages, stands, queue towers, pony rings behind. Midway: high wire over the crowd on every slot, turn about;
stages and stands behind the second row; stands and a tower through the corkscrew. Big jump: second unit, a stand
each side. Pirate Cove: parade and shows on the even slots; a stand and a tower round the loop. Lift hill: queue
towers. Lagoon (the coaster): stages and stands on the valley floor 62 to 78 m out, theatres at 136 m, doves at the
rails' height. Splashdown: crew, stands. Rocket Land: parade, high wire, stage, ponies, queue towers on the odd
slots; stands through the corkscrew and loop. Funhouse: parade, dog show, stands. Funhouse jump: second unit, stands.
Back lot: staff on cherry pickers, catering, a float and the whale waiting. Last Ride: sixteen slots to a round on
each side, 55 % kept; a tower every 2 km a side, crew at the tape; the main unit before the arena's hill
(10.9 to 11.23 km).

## Checks run

- Build after every table change: succeeded. `FTTRACK level=4 frames=12518 route=673f0081 solid=3e2a2030
  solidTriangles=987216` in every run, equal to `track_baseline.txt`.
- Game runs looked at (cropped): a baseline, two test drives of the first batch, two line-up runs (every mesh 25 to
  60 m from the start line, a temporary table), a 10-minute drive of the whole level with a frame a second (start to
  the boss leaving), and the final drive of scenes 1, 3, 5, 6 and 7 (start, cove loop, rocket land, funhouse, the
  boss on the Last Ride), left and right lanes.
- Log of the final drive: no `LogFT: Error`, no People asset that failed to load. One engine ensure
  (`DoubleFloat.cpp`, "precision loss while converting matrix to GPU format"); the same ensure is in other levels'
  runs on this PC from today, and it was not in this level's six earlier runs.
- Sheets: `sheets/L4_game_1.jpg` (cropped frames of the final drive: the high wire, the rainbow podium, the whale,
  a stand with its lamps, crew at the tape), `sheets/L4_pieces.jpg` (the 16 meshes close up, editor render, no cel
  pass).

## Still weak

- The Last Ride (15 km, two thirds of the level) is thin: about one piece a slot over both sides, and in the boss
  frames whole seconds pass with only the dressing in view. The cap of 900 is spent; more there means fewer elsewhere
  or a higher cap.
- At 145 mph with the game's motion blur a near piece is a smear of colour for most of its second on screen; what
  reads is the silhouette (whale, giraffe, rainbow, masts and bunting, a lit stand), not what the people are doing.
- The crew kit's own pieces are unlit and read as dark figures at night here (ApplauseA/B at the tape, Lights).
- Doves read as small specks; not enlarged.
- Pieces standing inside dressing pieces: slots were chosen from the dressing table to avoid it, and no case was
  seen in the frames looked at, but only a small share of 899 placements was looked at.
- In the lagoon, a stage or stand placed at a hollow's slot stands in the lake (decks 1.1 to 1.6 m, water 1.15 m).
  Not seen in a frame; expected at up to 11 of the 78 slots.

## Not checked

- Movement in the game: not confirmed by eye for any piece. The films were taken from a moving car (and the camera
  drifts during the slate), so frame-to-frame change could not be told from the camera's own. The editor renders
  show the trampolinist and the dogs displaced, nothing more.
- Per-mesh placed counts for the ten meshes the log does not list.
- The lagoon's pieces from the rails (only two wide frames looked at; none of my pieces identified in them for sure).
- Scenes 2 and 4 in the final drive (seen only in the earlier 10-minute drive, before the glow and the balloon fix).
- Performance, shadows, low quality settings: not checked.
- Lake wading (above), the travel wrap-around of the floats, the react-to-the-chase livening.

## ASKS

1. The FTCLEAR line should list every mesh with its placed count, not only those moved or left out: the population
   count cannot be read per mesh without it.
2. The crew kit at night: a wardrobe option or a lift for level 4 (the shared kit is not mine to change).
3. Is the whale acceptable beside a boss who attacks with balloons? If not, Parade_D goes and the lane keeps A, B, C.
4. A way to film from a fixed camera (or a harness command that holds the car still in play) so that movement can
   be checked.
5. `ft_people.figure()` has no glow value: the lift is done in this kit by re-keying the scene's colours after the
   figures are drawn. A `glow=` on `figure()` (or on `Scene.save`) would make that a one-liner for night levels.
6. `figure(travel=...)` gives every figure its own place on the line, so riders of a travelling float wrap one by one;
   this kit sets one base for the group after drawing (`together()`). A `base=` on `figure()` would do it cleanly.
7. If the cap can rise for this level, the Last Ride needs about 200 more pieces.
