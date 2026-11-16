# Level 7, City Center: the population (first round, 15 Nov 2026)

Branch `claude/people-L7`. Generator `Content/Python/ft_people_city.py` (`run()` makes every mesh), meshes in
`/Game/People/CityCenter/`, table `Source/FullThrottle/FTPeopleL7.inl`. Sheets: `sheets/L7_game.jpg` (cropped game
frames), `sheets/L7_pieces.jpg` (each piece alone, plain light).

## What a player now sees

Midtown at midnight has come out to watch the last scene. From the first block: the film unit behind its tape on
both pavements, commuters walking home past the lit shop windows (both ways, they come round again), crowds behind
blue-and-white police barricades in front of the gaps between the blocks, a pretzel cart with its queue, a march
behind a long striped banner (one of them held at the kerb by a police line), an old hand feeding pigeons. On the
shops' roofs: parties under strings of lights on a lit floor, and neighbours with a banner on two poles and flags.
A queue behind a red rope at every picture house, a rally with placards on the plaza before every other monument,
window cleaners in a cradle on the glass infills, pigeons wheeling over the side streets and over the far roofs.
From Penthouse Row on the Chairman's men stand in gold jackets and black caps before his banners; through the finale
they hold every fourth pocket and every fourth black wall, while the city cheers from the roofs it still holds.

## The meshes (16 of ours; triangles from the generator's log; counts from the game's FTCLEAR line, final run)

| Mesh | Shows | Function | Tris | Low/tall | What moves | Where the table puts it | Placed |
|---|---|---|---|---|---|---|---|
| SM_Onlookers_A / _B | ten people two deep behind a police barricade | `onlookers` | 1740 / 1744 | low | each hops, waves, claps, points out of step; livelier as the chase passes | pockets before the gaps, 21.5 m (22.3 m in the finale); inside corkscrews and loops by Far rules | 57 / 53 |
| SM_March_A | a march behind a 5.6 m striped banner | `march` | 1320 | low | banner sways, fists pump | pockets, mostly on the way to a rally | 10 |
| SM_March_B | the same, another banner, a police line of four holding it | `march(police=True)` | 1872 | low | as A; the officers rock | pockets | 13 |
| SM_Commuters_A / _B | seven or eight walkers on 30 m of pavement, a dog; B has a runner and a towed child | `commuters` | 1732 / 1920 | low | they walk the length both ways and come round | the strip 23.5 to 24.1 m out, every 2nd or 3rd slot a side, the six districts before the finale | 47 / 49 |
| SM_Vendor_A | pretzel cart, seller, queue with cups, two watching | `vendor` | 1656 | low (1.95 m) | steam floats, seller serves, two wave | pockets | 21 |
| SM_Queue_A | eight in evening dress behind a red rope, an usher | `queue` | 1984 | low | they turn and rock, two wave, the usher waves them on | 22.9 m out before every W_Marquee | 7 |
| SM_Syndicate_A | six of the Chairman's men in two ranks, their dog | `syndicate` | 1560 | low (1.95 m) | one scans through glasses, one points, others shift | pockets before the Syndicate's banners (Penthouse Row), every 4th pocket and every 4th W_Syn roof in the finale | 122 |
| SM_Pigeons_A | eight pigeons pecking, four circling at knee height, someone feeding them | `pigeons` | 1434 | low | pecking hop, wings, the circle, the throwing arm | pockets | 15 |
| SM_RoofParty_A / _B | 10 m / 8 m roof party: rail, lit floor, strings of bulbs, DJ, dancers | `roof_party` | 1752 / 1516 | tall | dancers hop, turn, rock; those at the rail cheer | on W_Shops2s, W_Shops1, W_Marquee, W_Garage, W_Yards roofs (set "up" = the roof) | 57 / 37 |
| SM_RoofRally_A | ten neighbours at a roof rail, banner on two poles, two flags, bulbs | `roof_rally` | 1416 | tall | banner sways, flags wave, all cheer | W_Pent roof terraces, some shop roofs | 82 |
| SM_WindowCleaners_A | cradle on two cables down a 34 m glass face, two cleaners | `window_cleaners` | 884 | tall | the cradle and both cleaners go 2.9 m up and down; arms wipe | at SM_Infill2 (G_Fill's own offsets) | 14 |
| SM_Rally_A | speaker on a crate with a megaphone, placards, flags, banner on poles | `rally` | 1580 | tall | everyone hops out of step, placards and flags swing | the plaza before SM_Monument, 24.4 to 27.3 m; inside the two loops | 14 |
| SM_PigeonFlock | fourteen big pigeons wheeling on three rings (always-moving master) | `flock` | 1364 | tall, far | rings turn opposite ways, wings beat | small (x0.36) 15 m over side streets; x2.2, 55 m up, 110 to 190 m out | 87 |

Crew (kit meshes, wardrobe City), as listed by FTCLEAR: Camera 11, ApplauseA 35, ApplauseB 11, Stunt 11, Crane 15
(on shop and W_Pent roofs, and by the jumps), Dolly 9, Sound 3, Catering 1. SM_CrewLights_City and SM_CrewVillage_City
are in the table and the log shows both meshes loaded for play, but neither is in the FTCLEAR list: their counts are
not known.

Totals: `FTDRESS level=7 ... pieces placed=7984 of 8013 tried` with the table, `7192 of 7210` with it empty: 792
population pieces placed. (The FTCLEAR counts above add up to 796: a difference of 4 I did not explain.)
`FTTRACK level=7 frames=12717 route=5faaeb1d solid=25fa2576 solidTriangles=957884` in every run: equal to the baseline.
No error or load failure of ours in the game log.

## Over the guides, and why

- Triangles: nine pieces are over 1,500 (the most: SM_Queue_A 1984, SM_Commuters_B 1920, SM_March_B 1872). Crowds of
  ten read where five did not; back rows are already at detail 0.
- Light: figures on pieces stood 24 m and more out carry a faint glow (0.09; `lit()` in the generator re-keys the
  builder's part meshes), because unlit figures on a roof at night were nearly black. Nothing nearer than 24 m glows.

## What was looked at

Cropped game frames of: both onlooker pieces, March_A, Commuters, Vendor, RoofParty_A, RoofRally_A, Rally_A,
Syndicate_A at the kerb (after its recolour), crew Applause (and, before the Plaza loop, dark crew pieces I could not tell apart). Drives: the start (scene 1, three runs), the
Lobby and Theatre Row corkscrew (scene 2), the Plaza (scene 4), the finale arena (scene 7, two runs).

## Still weak

- The finale (70 % of the road) is thin near the road: gold men every 180 m a side, small in frame.
- Roof parties read as a cluster of coloured figures; the strings of bulbs do not carry at speed.
- Far life is only the flocks. Nothing stands on the mid-rise roofs (their height varies with the Far rule's scale).
- The crew's City wardrobe is dark: behind its tape it reads as silhouettes.
- A pocket piece stands before whatever gap the dressing's dice gave: before an alley rule's 10 % empty gaps too.

## Not checked

- Movement in the game: the three `film` sequences caught banked bends or motion blur; no piece's movement was
  confirmed from frames. Movement was only set in the generator.
- Not seen in any game frame: SM_Queue_A, SM_WindowCleaners_A, SM_Pigeons_A, SM_PigeonFlock (either size), SM_March_B,
  SM_RoofParty_B, Syndicate_A on a W_Syn roof in its gold (seen there only in its first, black version), the crew's
  Crane and Lights on roofs, the crew at the jumps and loops (Far rules at a guessed drop), Camera, Catering, Dolly, Sound, Stunt, Village.
- The run-in through Penthouse Row (scene 6): the drive ended in the Dailies screen after 3 s; no frames.
- The right-hand pavement from the right lane in most districts; the circuit past 13.56 km.
- Whether any piece stands inside a dressing piece: none seen in the frames looked at; not checked piece by piece.
- Frame rate, shadows.

## ASKS

- `figure()` has no glow value: a night level needs one (see `lit()`); and a detail level between 0 and 1.
- A table word to place a piece only where a given dressing rule placed (same dice), so people can stand on an
  alley's fire escape (dropped this round: 10 % would have floated).
- FTCLEAR lists only some meshes: a full per-mesh count would let a worker count the crew.
- The crew's City wardrobe could take one bright colour.
