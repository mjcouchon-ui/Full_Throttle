# Population pass, level index 2: Nevada (game level 3). First round, 15 Nov 2026

Branch `claude/people-L2` (worktree `C:\ftwork\wt\L2`). Not pushed, not merged.

## What a player now sees

Highway 93 is lived on from the state line to the strip, and the shoot is visible beside it.

- **The start**: the film unit drawn up along the first 500 m on scaffold risers two metres up, both sides: camera
  position, crane, lights, the director's village, applause, the stunt team's air bag, the dolly, catering.
- **State line, Joshua Flats, the salt flats, the Syndicate's range**: ranch hands riding the fence line on a low dyke
  (three at the trot, a banner, hats in the air, a longhorn ahead and the dog behind), hitch-hikers on earth banks
  BEHIND a crash rail under a parasol (a thumb out, a striped card, one jumping; never at the verge), prospectors'
  claims on rock ledges (a pick swinging, a pan, the windlass, the mule, a red flag), boulders with collared
  lizards doing press-ups, a second line of riders 42 m out, herds beyond.
- **Snake Eyes country**: the club's beer deck in front of every roadhouse and between them: bikers at the rail,
  fists and bottles up, the colours on two poles, a string of pennants. A diner's terrace in front of every truck
  stop (parasols, a family, a waitress on skates, a trucker waving his cap).
- **The rock cuts (the dry wash, Red Rock Pass, the three straights of the return)**: nobody can stand by the road
  there (the cut is 25 m out), so the cast is on the RIM against the sky: coyotes, one howling, and a posse sitting
  their horses with a red banner. In the open bends between the cuts the same pieces stand as hoodoos.
- **Jackpot Junction, the city lights, the Vegas approach**: both sides of nearly every slot: grandstands of gamblers
  and tourists under pennants (one with a card wave along its front row), casino steps with valets running under a
  striped marquee, a terrace of slot machines seen side on (levers, a jackpot, balloons), a chapel-of-chance wedding
  on its dais (the couple jumping, confetti, balloons), and more grandstands 43 m out, 1.5 times the size.
- **The film crew** again at the first loop, before and after the jumps at 4.6, 7.3 and 9.7 km, at the second loop
  and corkscrew, in a cluster on the run in to the finale (10.3 to 10.5 km), every 590 m along the way, and at the end.

## How it is made

- `Content/Python/ft_people_nevada.py`: the kit (17 meshes in `/Game/People/Nevada/`). `import ft_people_nevada; ft_people_nevada.run()`.
- `Content/Python/ft_people_nevada_table.py`: plain Python, WRITES `Source/FullThrottle/FTPeopleL2.inl`
  (`python Content/Python/ft_people_nevada_table.py` in the project folder). The table is slot by slot: every row is
  one Site rule in a zone of its own, so the writer decides exactly what stands on which slot, side and distance, and
  keeps clear of what the dressing table has there (rock cuts and their taller walls, rock banks, ventilation heads,
  mile signs, phone poles) and puts decks in front of the roadhouses, truck stops and road stops.
- **What was learned here** (it changes the brief's point 1 for this level): from the chase camera the land right
  behind the barrier IS seen on this road (the ranch fence 19 m out shows whole). What does not read is small and
  finely detailed: at 145 mph, through motion blur and the cel pass's ink, a 1.7 m figure with hands and eyes is a
  dark smudge. So: every near piece is placed at 1.35 times its built size (1.2 on the phone line's side, where it
  also stands 7.5 m along its slot, between two poles; the crew 1.25 and 1.0), figures are detail 1 or 0 in hot
  colours, and every piece carries a big bright marker (flags, pennants, parasols, balloons, a banner).

## The meshes

Triangles from the generator's log. "In table" = rows of the table; the game placed 814 of the table's 883 pieces
(FTDRESS 8892 with the table, 8078 without; the other 69 are in loops, corkscrews, jump run-ups and landings, where
the track itself places nothing). Per mesh placed counts were not taken (the game's log lists only meshes with a
piece moved or left out by the clearance test: none of these).

| Mesh | What it shows | Function | Triangles | Low / tall | What moves | Where | In table |
|---|---|---|---|---|---|---|---|
| SM_RoadhouseDeck | bikers on the Snake Eyes' beer deck | `roadhouse_deck` | 2136 | tall | hops, fists, bottle, claps, hand flag, two flags | Snake Eyes: in front of each roadhouse and between | 15 |
| SM_DinerDeck | a diner's terrace | `diner_deck` | 2924 | tall | waitress travels on skates, child hops, waves, cook points | in front of truck stops and road stops, all flats and the strip | 44 |
| SM_Hitchers_A | hitch-hikers on a bank behind a crash rail | `hitchers(51, 0)` | 1268 | tall | thumb arm, one jumping, a wave, the dog's tail | state line, Snake Eyes, Joshua, gas, salt, range, city | 29 |
| SM_Hitchers_B | three with one big striped card, a child | `hitchers(52, 1)` | 1320 | tall | card and its two bearers hop, hat wave, child jumps | the same | 30 |
| SM_Prospectors | a claim on a rock ledge | `prospectors` | 1454 | tall | pick swings, windlass crank, bucket rises, pan rocks, mule nods, flag | state line, Snake Eyes, Joshua, gas, salt, range | 38 |
| SM_Lizards | collared lizards on a boulder, a tortoise | `lizards` | 848 | tall | press-ups, tails sweep, one scuttles the length, tortoise's head | the same | 30 |
| SM_Riders | ranch hands riding a dyke, a longhorn, a dog | `riders` | 2024 | tall | all travel 38 m and come round, legs stride, hats and a banner wave | fence line of the flats (27.6 to 29 m) and a second line 42 m out, 1.5x | 86 |
| SM_StripStand_A | grandstand of gamblers and tourists | `stand(141, 0)` | 3428 | tall | 20 people hopping out of step, arms, flags | the strip, both bands | 45 |
| SM_StripStand_B | the same, a card wave along the front row | `stand(142, 1)` | 3684 | tall | the wave, hops, flags | the strip, both bands | 39 |
| SM_Valets | a casino's carpeted steps | `valets` | 1904 | tall | two valets run the length, doorman ushers, couple waves, luggage cart slides | the strip | 46 |
| SM_Wedding | a chapel-of-chance wedding | `wedding` | 2552 | tall | couple jumps, six guests throw, confetti floats, balloons | the strip | 43 |
| SM_Gamblers | a terrace of slot machines | `gamblers` | 2372 | tall | levers and arms pull, winner jumps, coins hop, clapping, balloons | the strip | 45 |
| SM_RimCoyotes | coyotes on a 16.5 m column of the cut's rock | `rim_coyotes` | 804 | tall | the howl nods, one paces the rim, tails | on the rim of the rock cuts (33.1 m); as a hoodoo 40 to 70 m out | 80 |
| SM_RimPosse | four riders on the rim, a banner | `rim_posse` | 1996 | tall | hats and banner wave, horses nod, tails | the same | 71 |
| SM_WildHorses | nine horses and a foal at the gallop, dust (far=True) | `wild_horses` | 1788 | far | travel 140 m and come round, gallop, dust swells | 96 to 230 m: state line, wash, Snake Eyes, salt, range | 49 |
| SM_Cattle | a dozen head, calves, a rider circling (far=True) | `cattle` | 2468 | far | grazing nods, tails, three walk, rider and dog orbit the herd | 90 to 160 m: state line, Joshua, gas, salt, range | 40 |
| SM_CrewRiser | the crew's scaffold riser, a pink pennant | `crew_riser` | 612 | tall | the pennant | under every low crew piece (a set: the piece 2 m up) | 61 |

Crew kit pieces in the table (wardrobe Desert): Camera 12, ApplauseA 11, ApplauseB 11, Stunt 10, Village 8, Dolly 9
(each on a riser), Crane 12, Lights 12, Sound 5, Catering 2.

## Checked (what was run and seen)

- Builds; the level plays. `FTTRACK level=2 frames=12888 route=efc95392 solid=e2d27537 solidTriangles=964188` equals
  `track_baseline.txt`. `FTDRESS level=2 ... pieces placed=8892 of 8922 tried` (8078 of 8108 with the table empty).
  None of the kit's meshes appears in the FTCLEAR list (none moved or left out by the clearance test).
- Frames looked at, cropped and enlarged: the start (right lane and left lane), the dry wash's cuts and the jump,
  Snake Eyes country (left lane), Jackpot Junction and its loop and corkscrew (middle lane), the finale's first
  2 km with BIG DEALER on the road (right lane), and stray frames of the range and the salt flats.
  Seen and reading in game frames: SM_CrewRiser with the stunt and village sets, the crane, SM_Riders (both bands),
  SM_Hitchers_A, SM_Hitchers_B, SM_Prospectors, SM_Lizards, SM_RoadhouseDeck, SM_DinerDeck (dark under the truck
  stop's canopy), SM_StripStand_A, SM_StripStand_B, SM_Gamblers, SM_Wedding, SM_Valets, SM_RimCoyotes, SM_RimPosse.
- Contact sheets: `sheets/L2_nevada_road.jpg`, `sheets/L2_nevada_strip_rim.jpg`.

## Weak

- Triangles are over the 1,500 guide on most pieces (2,000 to 2,900; the grandstands 3,400 and 3,700): a figure costs
  about 200 even at detail 1. 17 meshes, one over the guide of 16: the crew's riser.
- People are 2.3 to 2.7 m tall as placed (the 1.35). It reads; beside a petrol pump it is a giant.
- SM_DinerDeck stands in the truck stop's shadow and reads darker than the rest.
- Every piece stands on its slot's middle (or 7.5 m on): the rhythm is regular, 45 m.
- The rim figures are silhouettes of a few pixels until the car is under them.
- Scenes 2, 4, 5, 6 and 8 were seen only in part; the last 13 km (the return and the Vegas approach) were not driven.
- SM_Riders' dyke is 55 m long as placed: on the phone line's side a pole stands in its back edge.

## Not checked

- The far herds (SM_WildHorses, SM_Cattle): NOT found in any game frame looked at (they stand 90 to 230 m out and
  were looked for in the finale's frames only at full-frame size). Whether they read at that distance is not known.
- Movement in the game: NOT confirmed. Films were taken (12 and 16 frames at 8 a second), but from the moving car
  (it would not stop: FTHold Brake holds it at 108 mph) a figure's pose cannot be compared from frame to frame.
  The movement is the shared M_People master's (ft_people.py); the pieces were looked at standing still only in the
  generator's close-up renders, which cannot show movement.
- Performance (no benchmark), the packaged build, quality levels other than 3.
- Red rock return, the Vegas approach, the end of the level, Red Rock Pass's wall bends: not driven.
- Per mesh placed counts.

## ASKS

1. A scaffold riser belongs in the shared crew kit (`ft_people_crew.py`): on a road like this one every LOW crew
   piece needs it. Mine is `crew_riser` in `ft_people_nevada.py` (SM_CrewRiser), placed as a set with the crew piece
   2 m up; move it if you agree, and my 17 meshes become 16.
2. The figure builder has no way to draw a figure from the hips up (a rider, somebody behind a counter): `mount()`
   here seats a 'sit' figure on the horse and adds legs down the flanks; the sitting thighs still show on the saddle.
3. The game's log gives no per mesh placed count unless a piece was moved or dropped: a count line per mesh for
   `/Game/People/` would let every worker fill this table.
4. The dressing's phone line (three poles a slot, 30 m out) takes the back of the 24 to 30 m band on one side of
   most districts; if it could stand at 31.5 m the population would not have to be smaller on that side.
