# L0 Bayshore: the population, first round (15 Nov 2026)

Branch `claude/people-L0`. Only what was run and seen; "not checked" where it was not.

## What a player now sees

- **Town side (right).** People on the pavement in front of the shop rows: shoppers with bags and cups walking both
  ways under the awnings, some stopped at the kerb side waving as the chase comes by. They read as whole figures
  against the lit shop windows, from the first second of the level. Over some awnings a balcony of five people
  leaning out, waving, filming.
- **Bay side (left).** Walkers, joggers, a skater and dogs travelling along the boardwalk in both directions; stepped
  sun terraces packed with people cheering right behind the boardwalk (a crowd of 14 to 16, raised over the barrier's
  line); lifeguard towers with a guard waving a flag; bandstands with a band and dancers. On the open beach parasol
  groups with children round a ball, a kite 9 m up over each (the kite is what the eye catches), ice-cream queues,
  volleyball, surfers carrying boards. In the sky flocks of big gulls wheeling; out on the water kite-surfers' kites.
- **The shoot.** Film crew in sun wardrobe behind pink-and-white tape: a unit by the start, stunt team before each
  jump, a camera and crane (or the dolly and the director's village) in the side street before each loop, a camera or
  dolly before each corkscrew, crew applauding on the boardwalk, units in the last kilometre.

## Meshes (`/Game/People/Bayshore/`, 16; made by `ft_people_bay.run()`)

Generator files: `Content/Python/ft_people_bay.py` (B) and `Content/Python/ft_people_bay_beach.py` (BB). The table
`Source/FullThrottle/FTPeopleL0.inl` is WRITTEN by `Content/Python/ft_people_bay_table.py` (plain Python).
"Placed" is from the game's FTCLEAR line of the last run where the log lists the mesh (it lists a mesh only when
at least one of its pieces was moved or left out); otherwise as noted.

| Mesh | Shows | Function | Triangles | Low / tall | What moves | Where the table puts it | Placed |
|---|---|---|---|---|---|---|---|
| SM_Strollers_A | couple and child, jogger, skater, dog walker, shopper on the boardwalk | B `strollers(11, 0)` | 2,296 | low (top 1.91 m as built; stood 0.48 m up on the deck by a Set) | everyone travels 30 m and comes round; legs, arms, dog's legs and tail | bay, 22.9 m, on the dressing's boardwalk, every 4th to 6th slot | 92 (all moved out with the boardwalk) |
| SM_Strollers_B | two joggers, skater, family of three, man with two dogs | B `strollers(12, 1)` | 2,388 | low (1.93 m) | as above | as above, alternating with A | 87 |
| SM_Shoppers_A | shoppers with bags and cups, three stopped to wave | B `shoppers(21, 0)` | 2,548 | low (1.88 m) | walkers travel; the three wave and hop (react) | town, 24.6 m, in the slots of the front shop rows | 118 (44 moved) |
| SM_SunDeck_A | stepped sun terrace, 16 people cheering | B `sun_deck(31, 0)` | 3,364 | tall (4.09 m) | every figure waves, claps or hops out of step (react); two pennants | bay, 26.4 m, slots free of the arcade and the walk's shops | 48 (counted in a run with only this mesh in the table) |
| SM_SunDeck_B | the same, 14 people, other colours | B `sun_deck(32, 1)` | 2,764 | tall (4.07 m) | as above | as above | not listed by the log |
| SM_Balcony_A | balcony on two posts, five people | B `balcony(41, 0)` | 1,532 | tall (6.21 m) | figures wave, film, hop (react); a towel swings | town, on the wall of the dressing's front rows at 27.5 m (one district per `Street(...)` line of FTDressL0.inl) | not listed by the log |
| SM_Volley_A | volleyball: two teams of three, referee, six watching | B `volley(51)` | 3,204 | tall (5.46 m) | the ball lobs over the net; players jump; watchers cheer | bay, 72-82 m (Strip, Pleasure Beach, Downtown) | not listed by the log |
| SM_GuardTower | lifeguard tower, two guards, two people below | BB `lifeguard(45)` | 1,396 | tall (6.35 m) | one guard turns with binoculars, the other waves a flag (react); roof flag; a child hops | bay, 27 m | 31 (10 moved) |
| SM_Bandstand | band of four under a canopy, seven dancers | BB `bandstand(47)` | 3,200 | tall (5.51 m) | drummer's arms, guitarists rock, singer, dancers twist and hop | bay, 28.2 m (Strip, Pleasure Beach, Downtown) | 17 (1 moved) |
| SM_BeachDay_A | three parasols, five on towels, three children and a ball, a kite, a dog | BB `beach_day(61, 0)` | 2,560 | tall (kite at 10.2 m) | ball hops, children hop, kite swings on its line, dog runs rings, sunbathers breathe | bay, 34-80 m by district | not listed by the log |
| SM_BeachDay_B | parasols, ice-cream cart and queue, two throwing a disc, children and ball | BB `beach_day(62, 1)` | 3,696 | tall (3.13 m) | disc slides to and fro, throwers' arms, ball, queue shifts | bay, 44-86 m by district | not listed by the log |
| SM_SurfWalk | five surfers carrying boards, a dog, two stood by their boards | BB `surf_walk(71)` | 1,736 | tall (2.44 m) | the line travels 24 m and comes round | bay, 40-98 m by district | not listed by the log |
| SM_Anglers | nine people on a pier (seven fishing), gulls on the rail and overhead | BB `anglers(81)` | 3,118 | tall; far master | rods dip, two wave, gulls wheel | on the dressing's piers (same slots as its PierSet) | not listed by the log |
| SM_Gulls | nine gulls, 2.5-3.4 m across, wheeling | BB `gulls(91)` | 846 | in the air 9-23 m; far master | each circles and beats its wings | bay, 40-200 m, every 4th to 8th slot | 70 (2 moved) |
| SM_SurfRiders | four surfers riding, two waiting, a kite-surfer | BB `surf_riders(95)` | 1,572 | afloat, at 2.6 x; far master | riders travel 70 m and come round; kite swings | afloat 420-520 m | 59 (1 left out) |
| SM_Dolphins | six dolphins | BB `dolphins(97)` | 648 | afloat, at 3 x; far master | each swings out of the water and under again | afloat 430-620 m | not listed by the log |

Crew (`/Game/People/Crew/SM_Crew*_Sun`, the shared kit, not counted in the 16): Camera 28, ApplauseA 24, ApplauseB 20, Dolly 13, Village 10, Crane 9, Stunt 7 (111); Lights, Sound and Catering are in the sets but not listed by the log.

Totals from the game's log: dressing alone `pieces placed=9767` (table empty); with this table `pieces placed=10838`:
**1,071 added**, crew included. The meshes the log lists come to 633 (522 population, 111 crew); the other 438 are
SunDeck_B, Balcony_A, Volley_A, BeachDay_A/B, SurfWalk, Anglers, Dolphins and three crew pieces together (not split:
a counting run took 30 minutes in the queue tonight, so only SunDeck_A was counted that way). Population alone is
about 930 by subtraction with the three unlisted crew pieces unknown: at or a little over the 900 guide.

Over the guides, and why: nine pieces are over 1,500 triangles (2,300 to 3,700). They are crowds of 9 to 18 figures;
a figure with a hat is 150 to 250 triangles and the crowd is the point of the piece. Mesh count is 16.

## What was run and looked at

- Sheet renders of all 16 meshes (`ft_people_sheet`, plain light): shape, colour, pose.
- Game drives, frames cropped and enlarged: the start (five drives, both lanes), scene 8, scene 15, scene 16.
  The last drive (the committed table) was the start only; scenes 8, 15 and 16 were driven with the table before
  its last trim (four rules fewer: strollers in the Marina and Hotel Row, shoppers downtown).
  Seen and judged to read: Shoppers_A (clear, whole figures), Strollers_A/B (from the left lane and from every
  raised stretch; from the middle of the road on the level only heads and shoulders, and the dressing's windbreaks
  hide some), SunDeck_A (a crowd in colour over the barrier), Balcony_A (people before a lit window), the kite of
  BeachDay_A and its parasol groups, Gulls (a flock against the sky), a lifeguard tower, the crew at a side street
  and on the boardwalk, a kite-surfer's kite over the horizon (small).
- FTTRACK equals the baseline in every run: `frames=12717 route=dc9e032b solid=d2e759f4 solidTriangles=986296`.
- No load failure for any `/Game/People` asset in the logs searched by the tool.

## Weak

- The bay side by the road is crowded with the dressing's own pieces (windbreaks, parasols, shops); walkers on the
  boardwalk are often behind them. People read best there where they are raised (terraces, towers) or seen from above.
- From the middle of a level straight, figures at 22 m show from the chest up only.
- Movement was judged from film frames taken while driving at speed (figures' arms and heights differ between
  frames); no still-camera film was made, so how each movement looks over a full cycle is not shown.
- The far sea pieces are small from the road: kites show, riders and dolphins were not made out.
- Slot rules avoid the dressing's tall pieces by arithmetic on its table as read today; where its chances are under 1
  or its slots change, a terrace may meet an arcade or a cabana. No such overlap was seen in the frames looked at,
  but only a small part of 25 km was looked at.
- A balcony whose shop row the track has pushed back on a bend stands free in front of it (it has posts for that).
- The last kilometre's town side lies far back from the road; little of the population shows there.

## Not checked

- SM_Volley_A, SM_BeachDay_B, SM_SurfWalk, SM_Anglers, SM_Dolphins, SM_Bandstand, SM_SunDeck_B: not identified in
  any game frame (sheet renders only). SM_SurfRiders: only its kite.
- Per-mesh placed counts for meshes the FTCLEAR line does not list (see table).
- The Marina, Hotel Row and The Point districts: no drive there (scene 8 is the only middle drive).
- Whether any piece stands inside a dressing piece outside the stretches driven.
- Performance, shadows, quality levels other than 3.
- An engine `Ensure condition failed ... DoubleFloat.cpp` ("precision loss while converting matrix to GPU format ...
  view transform") appeared once in each of six of the seven game runs with population (not in the empty-table run and not in
  the first run with six meshes placed; the second run, with the same table as the first, had it). Cause not established; it is a view-matrix message, logged about 17 s after start.
- The crew's exact place relative to the boss arena: units were put on slots 532-554 by the slot count, not by
  finding the arena.

## ASKS

1. A count per mesh in the FTCLEAR line even when nothing was moved (it lists only meshes with a moved or left-out
   piece), so placed counts need no extra runs.
2. The level's own prop `SM_Lifeguard` shares its report name with any mesh so named: mine is `SM_GuardTower`.
3. The sight rule in code pushes any piece whose bounds stand 2 m over the road out to 24 m, bounds grown by
   `Scene.save`'s default 60 cm included: a tall piece meant for 24.2 m needs `grow_down=(.., 0, ..)` or it is always
   pushed. Worth a line in the worker brief.
4. People standing ON the boardwalk at 22 m are 2.2 m over the sand (deck 0.48 m + a person) though each mesh is
   under 2 m as built and their heads are below the barrier's top. Say if that reading of the rule is wrong.
5. The figure builder has no lying pose (sunbathers are drawn by hand in `_lying`), and a walker can travel along
   the mesh's X only (nobody can walk out along a pier).
6. If the dressing's `Street(...)` lines change, run `python Content/Python/ft_people_bay_table.py` again (balconies).
