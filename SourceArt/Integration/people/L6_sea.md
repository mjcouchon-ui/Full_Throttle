# Level 6, Under the Sea: the population (first round, 15 Nov 2026)

Branch `claude/people-L6`. Generator `Content/Python/ft_people_sea.py` (`ft_people_sea.run()` builds all 15 meshes
into `/Game/People/UnderTheSea/`); table `Source/FullThrottle/FTPeopleL6.inl`; crew in the `Sea` wardrobe from the
shared kit. Only what was run and seen is written here.

## What a player now sees

The water over the reef is inhabited. At the start line the crew's base camp stands on both verges and a stand of
Nereida's aquanauts in yellow, orange, lime and white suits and white helmets floats up off its decks with pennants.
Over the reef sport divers swim along the road in loose lines with a scooter or a camera, big sea turtles beat
past, a pink octopus sits on a mound of rock with one arm raised, crabs walk along rock ledges at the barrier and
hot-pink dolphins go over their hill further out. In the kelp Nereida's mounted patrol rides giant seahorses
(yellow, lime, pink, orange) between the weeds. At the station aquanauts weld a pipe junction on scaffolds while one
hangs on a line above; in the salvage field crates rise under yellow and pink lift bags with divers steadying
them. From the approach on the stands come every few slots, on both sides, and through Nereida (where the boss is
fought) the city is out on its stands, the patrol rides, and the work goes on. Far out over the trench walls giant
jellyfish rise and sink and a cargo caravan under lift bags travels along the horizon. Beside the glass tunnels
swimmers, stands, turtles and dolphins are placed by Far rules (the dressing's way), so the tunnels are not empty.

## The meshes (all made by `ft_people_sea.py`; triangles and height from the generator's log)

| Mesh | Shows | Function | Triangles | Low / tall | What moves | Where the table puts it | Placed (final run) |
|---|---|---|---|---|---|---|---|
| SM_DiveTeam_A | 5 divers in line, the leader with a scooter, one waving | `dive_team(1, 5, 'scooter')` | 1316 | tall (10.2 m) | travel along the road, kicking legs, a float, bubbles, a waving arm | 25.5-34 m all districts, x1.5 to 1.7; 28-32 m by tunnels | 60 |
| SM_DiveTeam_B | 4 divers the other way, the leader with a camera housing | `dive_team(2, 4, 'camera', sign=-1)` | 1072 | tall (9.2 m) | the same | the same, other side | 61 |
| SM_Watchers_A | 11 aquanauts on a stand of three decks, two more in the water with pennants, two tall banners | `watchers(1)` | 1920 | tall (10.4 m) | each floats up and settles out of step, arms wave, they liven as the chase nears, banners swing | 25.5-29 m every district; 40-43 m x1.7 in Nereida; by tunnels | 84 |
| SM_Watchers_B | the same, another crowd (10 on the decks) | `watchers(2, 12)` | 1920 | tall | the same | the same | 78 |
| SM_Turtles | 4 sea turtles (shells 1.8 to 3.7 m as built), one small | `turtles(1)` | 768 | tall (8.4 m) | travel, long flippers beating, a float | 34-60 m reef, kelp, salvage, Nereida; by tunnels | 82 |
| SM_CrabRow_A | 4 crabs over a metre across on a rock ledge | `crab_row(1)` | 936 | LOW (1.88 m) | walk sideways along the ledge and come round, legs, claws snapping | 17.6-19.2 m reef, kelp, salvage, Nereida's left verge | 29 |
| SM_CrabRow_B | 3 crabs, another ledge | `crab_row(2, 3)` | 720 | LOW (1.93 m) | the same | the same | 27 |
| SM_AquaWeld | a scaffold with a pipe junction: a welder (small steady light), one carrying a case along the deck, one waving, one on a line above | `aqua_weld(1)` | 1638 | tall (8.6 m) | the carrier walks the deck, arms, floats, the one on the line bobs | 25.5-27 m (behind the pipeline) and 40-52 m: station, salvage, approach, Nereida | 49 |
| SM_AquaLift | a crate and a chest under two lift bags, two divers steadying, one on a crate stack pointing up | `aqua_lift(1)` | 1040 | tall (11.6 m) | the whole lift sinks and rises 0.7 m (x1.6) very slowly, bubbles | 40-66 m station, salvage (every 3rd slot each side), Nereida | not in the log's list |
| SM_Dolphins | 5 hot-pink dolphins, cream bellies, long beaks | `dolphins(1)` | 780 | tall (12.6 m) | travel at 5 m/s, each over its hill (a swing about a point 7 m under it) | 50-130 m all districts, x1.7 to 2; by tunnels and set pieces | not in the log's list |
| SM_Octopus | the pink octopus on a mound of rock, eight arms, one raised | `octopus(1)` | 1166 | tall (6.8 m) | mantle breathes, every arm swings at its own pace, the raised one waves | 44-80 m reef, kelp, salvage, Nereida, x1.6 to 1.8 | not in the log's list |
| SM_SeahorsePatrol | 4 aquanauts riding giant seahorses, pennants on lances | `seahorse_patrol(1)` | 1956 | tall (6.7 m) | travel, rise and fall | 25.5-27 m kelp (every 3rd slot each side), station, approach, Nereida | 58 |
| SM_JellyGiants | 5 giant jellyfish, bells 5 to 9 m, 18 to 45 m up, steady light | `jelly_giants(1)` | 800 | tall (47 m) | rise and sink 1.4 m very slowly, arms swing | 140-270 m, every district | not in the log's list |
| SM_FarConvoy | a cargo caravan: 3 loads under lift bags, 9 swimmers, three times life size | `far_convoy(1)` | 2340 | tall (31 m) | travels 90 m along the road and comes round, loads float | 140-230 m, every district | not in the log's list |
| SM_SeaCamSled | this level's own crew piece: a camera sled on its line 3 m up, operator behind, two more crew, floated crew tape | `cam_sled(1)` | 1392 | tall (5.1 m) | the sled and its operator slide 2.7 m each way, the others float | 25.5-28 m with every crew cluster, and every 36 slots a side in Nereida | 34 |

The game's FTCLEAR line is cut after its 42nd mesh, so it lists only some of this level's population meshes:
"not in the log's list" means the mesh was placed (it loads and is seen in frames) but its own count is not printed. The crew pieces it does list: Camera 6, Crane 12, Stunt 3, Catering 4, Dolly 3, Sound 4; the 14 listed meshes come to 594 of the 899.
The total below is from the FTDRESS line.

**Counts (final run):** dressing alone, people table empty: `pieces placed=9597 of 9608 tried`. With the table:
`pieces placed=10496 of 10507 tried`. Population pieces placed, crew included: **899**.
FTTRACK `level=6 frames=12470 route=04d3f899 solid=8927e339 solidTriangles=930148`: equal to the baseline in every run.

The crew (Sea wardrobe): the base camp at slots 1 to 6 (applause, camera, the sled, crane, village, lights, stunt,
catering); a low piece, a tall piece and the sled at slots 10, 29, 54, 55 (before the jump at 57), 60 (the loop), 68
(the corkscrew), 90, 119, 120 (the jump at 122), 126, 135, 147, 160, and 173 to 177 (before the boss: the finale
starts at slot 175); applause, sleds, a crane and lights thinly through Nereida.

## Over budget, and why

- Triangles over the 1,500 guide: SM_Watchers_A/B 1,920 (13 figures: a crowd has to be a crowd; the figures are the
  cheap `crowd_aquanaut`, about 120 each), SM_SeahorsePatrol 1,956 (four riders at detail 1 on extruded seahorses),
  SM_AquaWeld 1,638, SM_FarConvoy 2,340 (nine swimmers and three loads; a far piece, 47 placed at most).
- The Scale column of the table is part of the design: at scale 1 a diver was a speck in the frames. Swimmers are
  1.9 to 2.4 times life size, the stands' people 1.5 times, turtles and dolphins larger still.

## What is still weak

- Everything of mine is small in a 1920 frame unless it is within about 60 m ahead; the reef is so full of small
  bright shapes that people on or near the sand are lost. What reads best: the stands (banners, white helmets),
  the seahorse patrol, the lift bags, the octopus, turtles and dolphins against plain water. What reads least: the
  divers at distance (dark with ink until close), the crabs (a rock with orange things on it), the welders.
- Pieces go through the dressing in places. Seen: a seahorse patrol through a pressure habitat's tank and through
  kelp. The table was then rewritten so that everything nearer than 31 m keeps to 25.5-27.5 m and to slots the
  habitats do not take; after that rewrite I looked at the salvage field, the kelp forest and the reef again and saw
  swimmers and seahorses among kelp (which I left: weed is soft) but did not prove that no piece meets a tower, a
  dome or the travel tube anywhere in 25 km.
- Lifts, dive teams and the octopus are one variant each (no _B/_C); the far pieces are one design each.
- The octopus's arms are lost against the reef at 50 m; its mantle and raised arm carry it.
- Dolphins were pale pink at first and came out shark-grey in the water at 80 m; they are hot pink now and read pink
  from the jump at slot 122. They still have a back fin: they stand 50 m and more from the road.
- The pieces that travel wrap round: a swimmer shrinks to nothing at the end of its line and reappears at the start.
  From the road I did not catch it in a frame; up close it will show.
- The contact sheets mix frames from several builds of the table (the first drives, the test drive, the last ones).

## Not checked

- SM_SeaCamSled in the game: placed (count above) but I did not find it in a frame I cropped. Seen only in the
  editor's sheet render.
- Movement was judged from film strips at 4 to 8 frames a second from the moving car; I did not film any piece from a
  standing camera, so the rates of the slow floats and of the arms were not judged, only that positions differ.
- The far pieces past 180 m (jellyfish, convoy) keep moving by the always-moving master: not verified in frames.
- The level between 9 km and 25 km (Nereida's later laps) was not driven: the drives were scenes 1 to 7 for about
  31 s each (scene 7 is the first 2 km of Nereida).
- Performance, shadows, the packaged build: not checked.
- One run only (the test table, a film started at level clock 1.0) logged an engine ensure
  ("Found precision loss while converting matrix to GPU format ... view transform", DoubleFloat.cpp); none of the other
  runs did, with the same meshes. Not attributed.
- Whether a crab ledge or a crew piece on the verge hides a pickup on the road from the chase camera: not checked
  beyond the frames (low pieces, 17.6 m and more).

## How it was checked

Sheet renders of all 15 meshes; in the real game: the dressing alone (table empty), a first look with four pieces,
drives of scenes 1 to 7 on both sides of the road with a film on each side, a test drive with every piece not yet
seen put at the start. Frames were cropped and enlarged before judging. No error line of ours in the logs.

## ASKS

1. A table word for "stand this many metres over the land" on Scatter (Far has drop; Scatter has none): this level's
   people live in the water, and each height is baked into a mesh now.
2. The FTCLEAR line is cut at 42 meshes on this level: the per-mesh count of everything after is not in the log.
3. A way to keep a rule out of the slots another rule took (the pressure habitats, the watch towers): I did it by
   arithmetic on every/phase, which breaks as soon as the dressing table changes.
4. If the figure builder gets a "swimming" pose and a helmet at detail 0, `swimmer()` and `crowd_aquanaut()` here could
   go; both are drawn in this kit because ft_people.py may not be edited.
