# L5 Ice Mountain: the population, first round (15 Nov 2026)

Branch `claude/people-L5`. Generator `Content/Python/ft_people_ice.py`, meshes `/Game/People/IceMountain/`, table
`Source/FullThrottle/FTPeopleL5.inl`. Contact sheets: `sheets/L5_ice_game.jpg` (cropped frames of the real game),
`sheets/L5_ice_closeups.jpg` (the sheet tool's plain renders, no cel pass).

## What a player now sees

A ski resort on a film day. By the road, scaffold stands of people in hot-coloured ski suits and padded coats, turned
towards the car coming down the road, hopping, waving, flying flags. Behind them big-air hills: racers and boarders
burst out of a start hut and come down eight metres of slope towards the car. Small half pipes end-on to the road
with boarders swinging from wall to wall and friends on the lips. Snowmen with children hopping round them. On the
glacier, climbers in pink and orange on red ropes on faces of blue ice, right in front of the dressing's ice walls. On
the ridges, ibex on crags, dog sleds along the fence line, wolf packs running snow ridges further out, eagles
wheeling overhead. The film crew at the start, before each jump and loop and before the Abominable's road, and thinly
all the way, with a snowmobile camera of its own.

## Numbers (from the game's log)

- `FTDRESS level=5 ... pieces placed=8094 of 8111 tried` with the people table empty; `placed=8986 of 9011` with it:
  **892 population pieces placed** (crew included), 900 tried.
- `FTTRACK level=5 frames=12415 route=b2fc812a solid=9cada242 solidTriangles=931996`: equal to the baseline line.
- No `LogFT: Error` line and no load failure naming People, IceMountain or Crew in the last run's log.
- The game's FTCLEAR line names only meshes that were moved or left out, so it gives no count for a mesh that went
  down untouched. The "written" column below is the table's own arithmetic (slots x rules, before the track leaves
  pieces out of loops, pipes, corkscrews and jumps): it adds up to 951 against 892 placed. Per-mesh placed counts
  were NOT measured.

## The meshes

| Mesh | What it shows | Function | Triangles | Low/tall | What moves | Where the table puts it | Written |
|---|---|---|---|---|---|---|---|
| SM_SnowStand_A | scaffold stand, 11 people cheering, flags | `stand(4)` | 1976 | tall | hops, arms, flags; livens as the chase passes | sets PplStandAR/AL (turned 35 deg to the oncoming car), 34 m (28 m on the glacier); Far 34 m under loops | 87 |
| SM_SnowStand_B | the same, other people and colours | `stand(24)` | 1920 | tall | as A | sets PplStandBL/BR; Far under loops | 88 |
| SM_SkiHill_A | big-air hill, start hut, 6 racers and boarders coming down (right of road) | `ski_hill(-1)` | 1790 | tall | sliders descend, run out, come round; flags | Scatter 48-50 m right (40-41 in the finale's resort) | 30 |
| SM_SkiHillL_A | its twin for the left | `ski_hill(1)` | 1786 | tall | as above | Scatter 48-50 m left | 28 |
| SM_PisteRun_A | slalom course on the real slope: gates, start arch, 7 racers 2.3x, finish crowd | `piste_run` | 2764 | tall | racers come down the fall line; crowd hops (far=True) | set PplPisteRun (turned -90), anchor 180 m | 80 |
| SM_BoardPipe_A | small half pipe, 3 boarders, 6 friends on the lips | `board_pipe` | 2048 | tall | boarders swing wall to wall (a pendulum), friends hop and wave, flags | Scatter 44-46 m left, 78-82 m right | 37 |
| SM_LodgeDeck_A | apres-ski deck: parasols, a bar, 8 dancing | `lodge_deck` | 1670 | tall | dancing (hop, twist, rock), parasols nod | sets PplDeckR/L, 36 m, valley and resort districts | 31 |
| SM_Snowmen_A | two snowmen, four children, a sledge | `snowmen` | 1348 | tall | children hop, wave, twist | Scatter 57-60 m right, 56-60 m left; none from 8.94 km on | 19 |
| SM_IceClimb_A | blue ice face, 3 climbers on red ropes, 2 at the foot | `ice_climb` | 1374 | tall | climbers inch up and down, hammer | sets PplClimbR/L (turned 35 deg), 29 m, glacier districts | 72 |
| SM_DogSled_A | six huskies, sled and driver on a snow bank | `dog_sled` | 1326 | tall | the team runs 34 m and comes round; legs, hop | Scatter 30-32 m (ridges), 62-90 m (resort) | 51 |
| SM_Ibex_A | five ibex on a crag | `ibex` | 1012 | tall | one hops, others graze, a tail | Scatter 36-44 m (ridges), 62-80 m (chute) | 37 |
| SM_WolfRidge_A | six wolves (2.4x) running a snow ridge | `wolf_ridge` | 876 | tall | they run and come round (far=True) | Scatter 84-210 m, ridges and glacier; Far 100 m under loops | 52 |
| SM_RopeTeam_A | five roped mountaineers (2x) on a snow ridge | `rope_team` | 918 | tall | they walk and come round (far=True) | Scatter 100-240 m, ridges, glacier, chute | 43 |
| SM_Eagles_A | three eagles, 5 m wings, 24-32 m up | `eagles` | 348 | tall | wheel in circles, wings beat (far=True) | Scatter 50-130 m every district; Far under loops | 99 |
| SM_CrewSkidoo | the crew's snowmobile camera behind crew tape | `crew_skidoo` | 1840 | low (1.91 m) | camera and operator pan, one cheers, one points | Scatter 25-26 m, thinly; in the start unit | 31 |
| crew kit (Snow) | Camera, ApplauseA/B, Stunt, Village, Dolly, Crane, Lights, Sound, Catering | `ft_people_crew` (not mine) | | | | units at 0-0.1 km and 8.8-8.93 km; camera positions before the 3 jumps and 2 loops and where the first jump lands; thinly along | 166 |

15 meshes of mine. Over the 1,500-triangle guide: both stands, both hills, the pipe, the deck, the skidoo (crowds of
1.3x people at detail 0 cost about 150 triangles a head) and the piste run (2,764: seven racers, six spectators, 9
gates). Said here as the brief asks.

## What I looked at (cropped frames of the real game)

Drives: the start on both sides (three times), the Abominable's glacier (8.94 km on) on both sides, the finale's
white ridge on the right, the whiteout's loop and corkscrew and the village street on the right.

- Read well: the stands (after two fixes), the board pipe and its boarder in three different places on the walls in
  three frames, the big-air hill with racers at different heights and one in the hut's doorway, the climbers, the
  snowmen, the ibex, the eagles, a stand under the whiteout's loop.
- Seen but small: the wolf pack (a line of dark blobs on white at about 100 m), the dog sled (its driver; the dogs
  were small in the frame I have), the piste run's finish banner and crowd, the crew's applause and dolly pieces.

## What was learned (for the other levels)

1. The cel pass inks a life-size figure 30 m off into a black scribble, and a packed crowd into one black mass. What
   fixed it: people 1.3 to 1.45 times life size, detail 0, plain bright clothes (no dark legs, no visors), well apart.
2. The chase camera looks ALONG the road: the first stand showed its end wall. Pieces that face the road are now put
   down by a set that turns them 35 degrees towards the oncoming car (yaw -35 on the right, +35 on the left), and a
   stand has no end walls.
3. A steady descent out of M_People, which has no such movement: see the header of `ft_people_ice.py` (a swing about
   a pivot deep underground at the travel's own rate). The skis stay on the snow in the frames I read.
4. On the glacier the dressing's ice walls (33 to 46 m) hide everything behind them: the cast stands at 28-29 m.
5. `film` with several `FTStartLevel 6 <scene>` in one script works. Scene 7 is the finale (8.94 km); the valley has
   no scene of its own (it is the end of scene 6).

## Still weak

- The crew kit's life-size figures in dark gear ink to dark clusters at any distance over about 40 m here.
- The stands are still a dark cluster from further than about 80 m; they read from there in.
- The big-air hill is a large white block from behind and the side; only its slope face carries colour.
- The piste run's racers: I have no frame that shows them on the slope (see below).
- No variants B/C of the hill, pipe, deck, climb, snowmen (only the stand has two).
- A district's cast changes with the district, but the three resort districts share one mix.

## NOT checked

- SM_LodgeDeck_A, SM_RopeTeam_A and SM_CrewSkidoo in game frames: not seen in any frame I read (their plain
  close-up renders were looked at: `sheets/L5_ice_closeups.jpg`).
- SM_PisteRun_A's racers in the game: only its finish banner and crowd were seen. Whether the racers sit on the real
  12-in-100 slope, and what the start looks like where they rise out of the land, is not checked.
- Movement by a `film` at 12 fps: two were taken and both fell on a banked wall with nothing in view. Movement was
  read only from frames 0.5 s apart (the boarder, the racers); arms, hops, flags, wolves, eagles, the dog team:
  not checked for how they move.
- The valley run (7.3 to 8.94 km) on the left; the half pipe, the chute, Black diamond and the whiteout's open
  stretches; everything past about 13 km (the Abominable escapes at 80 s and the level ends, so the autopilot never
  reaches the finale's resort, the second glacier, the high ridge or the last lifts).
- Whether any piece stands inside a dressing piece. Seen once: a string of pennants (not mine) standing on or beside
  a big-air hill on the right at about 0.3 km. Pines, the service road, lift lines, chalet rows: not checked.
- The sight rule and the 40 m rule for the boss were kept by the table's distances, not measured in frames.
- Per-mesh placed counts; performance; shadows.

## ASKS

1. A table word or a rule to put a population set on the SAME slots as a dressing rule (same roll of the dice): the
   dressing's `SM_PipeStands` benches and the lodge's balcony stand empty, and I could not fill them, because a rule
   of mine with chance 1 would put a crowd in the air where the dressing's own 0.9 chance left the stand out.
2. In M_People: a travel that also falls or rises (a slope along the line), so that a descent needs no trick; and a
   per-mesh count in the FTCLEAR line for meshes that were placed untouched.
3. The Far rule has no fixed yaw: under loops and corkscrews the stands cannot be turned towards the car.
4. The finale past about 13 km cannot be driven to with the boss on the road: a harness switch that keeps the boss
   from escaping would let the last 11 km be looked at.
