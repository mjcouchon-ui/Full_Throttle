# L3 Volcano: the population, round 1 (15 Nov 2026)

Branch `claude/people-L3`. Generator `Content/Python/ft_people_volcano.py` (`import ft_people_volcano as K; K.run()`),
meshes `/Game/People/Volcano/` (16), table `Source/FullThrottle/FTPeopleL3.inl`. Crew wardrobe: Heat.
Only what was run and seen is reported here; "not checked" means exactly that.

## What a player now sees

- **The start line is a film set.** On both sides of the first 300 m the shoot stands on raised rostrums with pink and
  white pennants: the camera position, the dolly, the director's village, the stunt team with its blue air bag, catering
  and the lights; a crane on the ground; and two stands of villagers who have stopped to watch, under strung-up cloth.
  Far to the right, up the slope, the striped tents of the assembly camp with a line of lanterns.
- **People stand on trestles.** The ground here is hot and lies under the road's barrier, so everyone near the road is
  on an open deck 1.8 m up, 28 m out: villagers walking out along boardwalks with bundles on their heads, a handcart, a
  mule, goats on a lead, children waving; stands of villagers cheering the chase; the survey's scientists in white coats
  and coloured hard hats with a turning dish, a spinning anemometer, a pink and white balloon on its line; fire crews in
  yellow throwing white-blue water over the lava channel's edge.
- **Goats** on basalt crags among the kerb's own stubs, a child minding them from the top.
- **Out by the fissures** (the curtain of fire, 0.36 to 0.54 km, and on through the districts) lines of firefighters
  half as big again throw tall arcs of water at the fire.
- **The sky**: skeins of white egrets and of pink flamingos leaving, wings beating, the whole way round.
- **Up the slopes** (twice life size, lantern-lit): caravans of villagers on the move, herds of goats with a herder and
  a dog, the camps.
- **On Engine 13's road** there are no fire crews near the road at all: villagers leaving, stands, scientists, goats, the
  shoot. The crews before it are on foot, in yellow, with pumps and hose: nothing red and no vehicle anywhere in the kit.

## The meshes (all from `ft_people_volcano.py`; one section, `M_People` / `M_PeopleFar`)

Placed counts: the game log gives the total only (a mesh is listed in FTCLEAR only when a piece of it was moved or left
out, and none of this layer's were), so the per-mesh numbers are the table's own slot arithmetic scaled to the logged
total (Scatter/Site x 0.825, Far x 1): **estimates**. The total is exact: 9187 placed with this table, 8287 with the
table empty = **900** (of 900 tried; none left out by the road test, none pushed).

| Mesh | Shows | Function | Triangles | Tall | What moves | Where the table puts it | Placed (est.) |
|---|---|---|---|---|---|---|---|
| SM_VolEvac_A | villagers leaving along a boardwalk (+X): bundles, cart, goats | `evac(21, 95)` | 2518 | tall | walkers travel and stride, cart and goats travel | near, right side, 28.4 m, x1.15 | 60 |
| SM_VolEvac_B | the same the other way (-X), with a pack mule | `evac(22, -95, mule=True)` | 2374 | tall | as above | near, left side | 67 |
| SM_VolHose_A | three firefighters, three jets, a pump; two wave the chase on | `hose_a` | 2274 | tall | jets sweep, crew rock, wavers hop (react) | near: left 28.4 m, right 34 m; 0.3 to 8.2 km only | 17 |
| SM_VolHose_B | two on one big jet, two carrying hose down the deck, one resting | `hose_b` | 2374 | tall | jet sweeps, carriers travel | as Hose_A | 18 |
| SM_VolSurvey_A | scientists: theodolite, dish, anemometer, balloon | `survey_a` | 2334 | tall | dish and cups turn, balloon floats, two react | near | 47 |
| SM_VolSurvey_B | the gas team: wind sock, radar bar, thermal camera, sampler | `survey_b` | 2264 | tall | sock swings, bar turns, pole dips, three react | near | 35 |
| SM_VolWatch_A | fourteen villagers on a stepped stand under strung cloth | `watch(61)` | 2710 | tall | everyone hops and waves out of step (react), cloth sways | near; middle (72 to 80 m) x1.4 | 67 |
| SM_VolWatch_B | the same, other people, other cloth | `watch(62, ...)` | 2634 | tall | as above | near; middle x1.4 | 60 |
| SM_VolRostrum | the shoot's raised deck (the crew kit's pieces stand on it) | `rostrum` | 600 | tall | two pennants swing | under every Ppl set, 28.8 m | 69 |
| SM_VolGoats | goats on a basalt crag and the child minding them | `goats_a` | 1416 | tall | goats hop and graze, tails wag, child waves | near; middle (70 to 86 m) x1.6 | 72 |
| SM_VolHerd | thirteen goats on the move, a herder, a dog (x2.2) | `herd` | 2364 | tall | all travel, trot, hop (always-moving) | slopes, 128 to 138 m (Far) | 36 |
| SM_VolCaravan | villagers leaving, far: carts, mules, lanterns (x2) | `caravan` | 2790 | tall | all travel and stride (always-moving) | slopes 196 to 204 m (Far); flat 78 to 86 m x0.7 | 62 |
| SM_VolCamp | the assembly camp: three tents, lantern line, twenty people (x2) | `camp` | 3606 | tall | crowd hops and waves (always-moving) | slopes 228 to 236 m (Far) | 18 |
| SM_VolFireLine | five firefighters on the ground (x1.7), five tall jets | `fire_line` | 1946 | tall | jets sweep, crew rock (always-moving) | 84 to 112 m, at the fissures | 23 |
| SM_VolFlock_A | eleven egrets (3.3 to 4.5 m span) | `flock(51)` | 1056 | sky | wings beat, the skein travels 110 m and comes round | sky, 55 to 130 m out (Far) | 71 |
| SM_VolFlock_B | nine flamingos | `flock(52, ...)` | 972 | sky | as above | sky (Far) | 42 |
| crew kit (Heat) | Camera, ApplauseA/B, Dolly, Village, Stunt, Sound, Catering, Lights on rostrums; Crane on the ground | `ft_people_crew` (the lead's) | - | - | the kit's own | Ppl sets at 28.8 m, Up 1.8; crane 31 m left / 40 m right | 125 + 11 |

Over the triangle guide (1,500): every crowd piece is 2,300 to 2,700 (ten to fourteen figures, already at detail 0 or 1,
plus an open trestle of about 300), the caravan 2,790 and the camp 3,606. Reason: on this level one figure is a speck
and a mass reads; I cut each once (the stand from 17 to 14, the camp from 26 to 20, the caravan from 18 groups to 13)
and stopped where the read began to suffer. 900 pieces is at the cap.

Glow: the Volcano is dark and an unlit figure past 60 m is ink on black. The far pieces carry a steady self-light of
0.05 to 0.06 (`lifted()` in the generator), the near ones 0.035, the jets 0.08, the birds 0.2, lanterns 0.5. All of
them stand 28 m or more from the centre line; nothing blinks.

## What I ran and looked at

- Build: `FullThrottleEditor` compiles with the table (FTTrackDress.cpp, 10 to 18 s).
- Game drives (frames cropped and read): the start (scene 1) four times; scenes 2, 4, 5, 6, 7 at 145 mph on both lanes;
  scenes 1, 2, 5, 7 again at 108 mph (brake held) with 2 s films; scene 6 run on into the boss fight (34 to 49 s).
- Every run: `FTTRACK level=3 frames=12497 route=506786e7 solid=eead12ba solidTriangles=900592`, equal to the baseline.
  `FTDRESS level=3 districts=145 sets=20 pieces placed=9187 of 9232 tried shadows=1820 of 9187` (table empty:
  `districts=82 sets=15 pieces placed=8287 of 8332 tried shadows=1247 of 8287`). No `Error` line and no failed load
  naming /Game/People in the logs; no `result=timeout` in the scripts.
- Seen in game frames and judged to read: the start's sets and stands, Watch_A, Watch_B (near, from the air, from the
  wall ride, on the boss's road), Hose_A, Hose_B (the jet over the left channel, four film frames), Survey_A (near, twice),
  Survey_B, Goats (near, right side), Evac (as a row of coloured walkers on a boardwalk, 60 m ahead), the fire line
  at the curtain of fire, the camp's tents at the start, both flocks, the balloon, a crew set (catering and lights)
  passing in a film.
- Fixed after looking: water and far figures were navy/black (gave them their own light); white birds read black
  against the sky when small (they are 3 to 4.5 m now and lit: white near, silhouettes far); red hair under a white
  helmet read as a red helmet on the fire crews (hair fixed dark: nothing red on them); tripod legs went through the
  deck; the first table placed 1,159 pieces (now 900); near pieces placed 15 % over life size.
- Sheets: `sheets/L3_volcano_near.jpg`, `sheets/L3_volcano_far_move.jpg` (game frames, cropped), and
  `sheets/L3_volcano_meshes.jpg` (the sixteen meshes under plain light).

## Still weak

- At 145 mph a near piece is two or three frames of motion blur; what reads is the mass, the colour and the pale deck,
  not what each person is doing. From 100 m ahead a deck of people is a row of coloured dots.
- On Engine 13's road the works (gantries, sluices, pump houses) are dense and dark, and the decks sit in among them:
  the people there read less than on the ash road.
- A third of the level is in the air, upside down or banked. The near rules leave the banked bends out (the land is cut
  away there), so in the bends (0.9 to 1.24, 1.67 to 1.89, 2.95 to 3.3, 3.85 to 4.08, 5.22 to 5.53, 6.84 to 7.08, 7.25
  to 7.37 km) and on the right between 2.4 and 3.9 km (kept clear for the eruption) there are only the flocks and the
  slopes' far pieces: stretches of more than 300 m with nobody near.
- The trestles are 3.7 m to the people's heads at 28 m, between the road and the lava channels. They are open
  underneath and one to a slot at most, but the dressing's own rule is "nothing taller than the kerb between the road
  and a channel": the lead should look at whether the channels' glow suffers.
- Shadows: this layer adds 573 shadow-casting instances (1247 to 1820). The dressing's notes say the Volcano already
  fills the shadow queue.
- The caravans and herds walk one way on the right of the road and the other way on the left (a left-hand piece is
  turned round). The near boardwalks are paired so that both sides walk the way the chase goes.
- Variants: the far pieces (herd, caravan, camp, fire line) and the goats have one variant each.

## Not checked

- Movement of each piece one by one. Seen moving: the big jet (four film frames, its fall point shifts along the bank).
  Not separately confirmed in game frames: walkers travelling, hopping crowds, the dish, anemometer, wind sock, radar
  bar, balloon, pennants, goats, flapping wings. They use the same builder values as the crew kit; the close-up renders
  cannot show movement.
- SM_VolHerd, SM_VolCaravan and the crew's crane itself: not identified in any frame I read (they are placed: the
  total counts them).
- The level beyond 11.3 km (the burning slopes, the ash fields and the second time round): not driven.
- The districts' middle-distance goats and stands at 70 to 86 m: seen once (a stand, ash road); not checked against
  the level's own spires and the mine's pieces for overlaps.
- Whether any trestle stands inside a dressing piece (pipe run, fumarole, vent stack): none seen in the frames read,
  not checked slot by slot.
- Frame rate, memory, packaged build: not checked (not asked).
- The eruption's view (2.4 to 3.9 km, right): the table writes nothing there; I did not look at frames of it.

## ASKS for the lead

1. A way to say "this piece casts no shadow" from a table or from `Scene.save` (the dressing does it by placing a mesh
   first with a Far rule; this layer's meshes are first placed near).
2. A per-mesh placed count in the log for meshes that were neither moved nor left out (FTCLEAR lists only the others):
   the counts above are estimates because of it.
3. `ft_people.figure()` has no glow value: on dark levels figures need a little self-light past 60 m. I set it after
   the fact by re-keying the scene's part meshes (`lifted()`); a `glow=` on `figure`, `bird` and `quadruped` would be
   the clean way.
4. `run_game.ps1` prints the baseline of the first FTTRACK line in the log (level 0, the attract map), not of the
   level run: compare by hand.
5. A harness command that stops the car (or a free camera) would let a worker film a piece's movement; `FTHold Brake 1`
   only slows to 108 mph.
6. The first build of a worktree compiled `FTTrackDress.cpp` with a table edited while it ran, and the next build then
   saw nothing to do: touch the `.inl` after a long build.
