# The world, second dressing pass (15 November 2026)

Eight levels, two a worker, on top of the first pass (districts, sets, a horizon). The aim of this pass: the ground
plane where the camera sees mostly land (banked bends, elevated stretches, jumps, loops, corkscrews), each level's
"shot to remember" from the owner's brief, the first pass's weak points, and movement. The track is untouched: every
level's `FTTRACK` line is what it was, with the dressing on and off (`ft.Dress 0`), and `FTTRACKSUM` agrees in the
packaged game. Tables: `Source/FullThrottle/FTDressL<n>.inl`. Kits: `Content/Python/ft_dress_<level>.py` (run
`ft_dress.run([...])`, then the kit's own `animate()` or `life()`: an export puts the plain materials back).

| Level | Placed, first pass to second | Kit meshes | What was added |
|---|---|---|---|
| Bayshore | 5,237 to 7,434 | 63 | Mosaic beds on the cut banks of the 18 steep bends; pool decks, courts, plazas and gardens among the hotels; boardwalk shops, lidos, club decks and a boatyard on the beach; districts through the loops, corkscrews and jumps; a downtown that grows to the finale |
| Chemical Works | 4,693 to 6,337 | 43 | The reactor dead ahead of the jump at 4.4 km and passed at 5.2 km; column trios and cooling towers over the wall hairpins, lit catwalks through the S-bends; three rows of yards, sidings, channels, ponds and helipads |
| Nevada | 6,319 to 7,677 | 46 | Motel courts through Last Chance Gas, then sign towers and two casinos that grow towards the finale; canyon wall outside the wall bends; fins and small mesas among the hoodoos; scrub, wash braids, ranch tracks, an airstrip |
| Volcano | 3,812 to 5,218 | 46 | The mine (crusher, conveyor, tips, a railway with slag trains, pump houses); pale ash crescents on the verge; far lava as broken cascades; rafts and chevrons on the channels. Nothing tall stands between the road and the erupting cone from 2.4 to 3.9 km |
| Theme Park | 5,599 to 7,285 | 51 | The table follows 3.3's longer coaster (every district after it stood 877 m early); 41 lakes; planted beds on the banked cuttings; about 1,670 floor pieces (plazas, pools, paths, queue yards, kart tracks); the rides turn |
| Ice Mountain | 3,889 to 4,687 | 51 | Avalanche fences as rows of grey bars (they inked to black scribble); race banners on masts down every half pipe and bend; a rink, a car park, a terrain park, a lake under each jump; a hotel above the villages; new horizon shapes |
| Under the Sea | 6,387 to 7,683 | 41 | Every glass tunnel has its district beside the glass (reef shelves, kelp, station domes, wreckage, vents) and reef pavement at its foot; cables in trenches, submarine aprons, a whale's bones |
| City Center | 4,139 to 5,082 | 40 | Tiled and Syndicate faces on the banks of all 51 banked bends; 86 side streets, 331 lots, 267 plazas, kerb cafes and road works; the elevated railway as 17 whole lines |

Before and after sheets: `K:\Full Throttle\SkyScratch\HandBack\jam\dress2\<level>_before_after.jpg`. Each worker's
notes ("Second pass") are in `K:\Full Throttle\SkyScratch\DevLog\jam\dress_*.md`.

## Still weak (the workers' own lists, checked against their sheets)

- **Road corkscrews, every level**: the frame is the bed's trench. Only the land's material shows there
  (`ft_world.land_materials`).
- **Banked bends**: faced since the pass on Chemical Works, Nevada, the Volcano, Ice Mountain and Under the Sea, by
  a rule of the track's own (see "After the pass: the cut banks" below). Bayshore, Theme Park and City Center keep
  the pass's tilted pieces (`SM_BankWaves` in the Bayshore kit is the plainest example): the first 60 m of each of
  their steep bends and their bends banked under 40 degrees are still bare, and one bed a slot leaves bare bank
  between beds. A Bank rule with a flat piece would cover all of that.
- **Bayshore**: the opening 3.2 km gained least (flat pieces stand level, and that stretch is hills); banks under
  40 degrees and the first 60 m of each steep bend are bare; the downtown is one piece repeated.
- **Chemical Works**: the finale's doubled circuit (14.0 to 17.4 km) has one sparse ground row; a few glowing ponds
  stand 25.6 m from another stretch of road (the pass's rule was 30 m for anything lit).
- **Nevada**: the wall bend at 5.2 km is mostly sky; three designs carry the whole casino approach.
- **Volcano**: loops are black; the mine reads at 9 to 10 km and is small elsewhere; far lava is still strokes on black.
- **Theme Park**: coaster loops and corkscrews unchanged (the track keeps everything 39.6 m from the rails there);
  the bank beds are loud (one number in `_bank_len`); jump valleys are plain.
- **Ice Mountain**: the skyline is still mostly similar cones; about 135 more horizon instances that never cull.
- **Under the Sea**: open sand on the first 8 km's hills is thin. (The pale wall at 20 s was the level's own cut bank:
  faced since.)
- **City Center**: straight street views are unchanged (the level's own bright towers dominate); most of the 83 blade
  signs stand inside those towers at 30 m.
- **Movement** was run on every level and seen in stills only.
- **Frame time was not measured on any level** (about 800 to 2,000 more instances a level; no idle GPU that night).
  Bayshore, Nevada and Under the Sea are near the pass's guide of 8,000 placed pieces.

## What the workers asked of the shared files (none was changed)

1. `FTTrackDress.cpp`: a **Ground** word that lands like a Scatter and stays through loops, pipes, corkscrews and
   gaps like a Far (three workers used Far rules with a drop as a stand-in), and an "outside of the bend" flag.
2. `FTTrack.cpp`, the dressing's `Put`: pitch a piece to the land along the road (on grades a level piece floats at
   one end: Ice Mountain falls 7 in 100), and keep near pieces off the cutting beside a banked bend. (Not made. The
   cut banks' panels are pitched and turned by their own rule; Scatter and Site pieces still stand level.)
3. `AFTTrack::RoadSpan`: coaster loops and corkscrews reserve 38.1 m below the rails; the rails sweep about 16 m.
4. `ft_dress.run()` should call each kit's `animate()` / `life()`, so an export cannot leave things standing still.
5. `Clear()`: a 30 m rule for lit pieces.

## After the pass: the cut banks (15 November, midday)

The inside of a banked bend is the bed's own cutting: a third of the frame from the rolled camera, in the land's one
colour, and nothing could stand on it. The pass's three faces were pieces built tilted for one bank angle and stood by
hand-listed slots, which is why four levels had none: Ice Mountain falls 7 in 100 all the way, two of the Chemical
Works' hairpins climb while banked, and no piece could follow a bend's lead-in. The dressing now has a **Bank** rule
(`WORLD.md`, "The cut banks"; `FTDress.h`): the track reads the cutting from its own bed and lays flat-drawn panels on
it, turned, pitched and stretched to fit, 11.3 m of road each. The track is untouched: `FTTRACK` equals
`track_baseline.txt` on every level with the panels on and off (`ft.DressBank 0`).

| Level | Pieces laid | Along | What |
|---|---|---|---|
| Chemical Works | 971 (589 before the lead-ins were refitted) | 5.9 km | The works' concrete facing: slabs, the teal main on its saddles, a hazard placard or leaking outlets (`SM_CutRevet`, `SM_CutRevet2`); one course of slabs on the shallow cuttings (`SM_CutKerb`, `SM_CutKerb2`). 50 lengths are left bare where the finale laps beside itself, 3 where the bank is too bent |
| Nevada | 127 (87) | 0.8 km | The highway cut through the red rock: seven beds that run from panel to panel, sage and fallen blocks (`SM_CutStrata`, `SM_CutStrata2`); a low ledge (`SM_CutLedge`) |
| Volcano | 210 (144) | 1.4 km | Basalt columns in three greys, ash on their heads (`SM_CutBasalt`, `SM_CutBasalt2`); slumped ash (`SM_CutAsh`). The finale's bends are banked 14 degrees: too little cutting shows over the barrier, so they are left alone |
| Ice Mountain | 874 (516) | 5.3 km | Layers of blue ice, splinters standing proud, a cornice of snow, the mountain's rock on every third panel (`SM_CutIce`, `SM_CutIce2`); a ploughed snow bank with a seam of ice (`SM_CutSnow`) |
| Under the Sea | 578 (412) | 4.3 km | Beds of reef rock and packed sand, crusts of coral and weed (`SM_CutReef`, `SM_CutReef2`); a low shoal (`SM_CutShoal`). Not one of the four levels asked for: its pale wall was on the list above and the rule made it three table lines |

Before and after, from one build: `K:\Full Throttle\SkyScratch\HandBack\jam\dress2\banks_<level>_before_after.jpg`
(the editor's chase camera) and `banks_play.jpg` (the real game).

**The lead-ins, refitted the same afternoon.** A review found that through a bend's lead-in and lead-out, where the
cutting deepens under a panel (its foot sinks as the bank comes on, its top stays at the land's edge), a panel sized
from its middle stood a corner 30 to 60 cm out of the ground at the land's edge, one or two panels a wall bend, and
left bare wedges at its deep end. A piece now runs with the land's edge and is kept inside the cutting at both of its
ends: one level top line half a metre under the land's edge, a foot that steps down as the cutting deepens, and
halves, quarters and eighths where it deepens fast (`WORLD.md`). That is why there are more pieces on the same
lengths: 127 of the Chemical Works' 617 lengths are laid in shorter pieces, 14 of Nevada's 79, 22 of the Volcano's
138, 113 of Ice Mountain's 506, 74 of Under the Sea's 409. No length is left bare for being too bent any more (3 on
the Chemical Works and 1 on the Volcano were). Every other rule's placements are the same piece for piece, and the
track's fingerprint is the baseline's on all eight levels. Before and after, the first wall bend of the Chemical
Works from the road and from over the land's edge: `K:\Full Throttle\SkyScratch\HandBack\jam\dress2\banks_leadin_before_after.jpg`.

Still weak here:
- Panels are flat and 11 m long: at a lap a faint tick shows where two meet, most on Ice Mountain's snow.
- In a lead-in every piece is the whole design at its own height, so a design's level bands (the Chemical Works'
  teal main, Nevada's beds, the ice layers) step from piece to piece instead of running through, and a quarter or
  an eighth squeezes the design's width: joints stand close and a placard becomes a sliver. A top strip of fixed
  height along the land's edge with the body stretched under it would carry a band through; it needs the designs
  drawn in two parts.
- On the Chemical Works' two climbing hairpins (2.6 and 3.6 km) up to 12 cm of bank shows between two whole panels'
  bottom corners, closing 1.5 m up: the 3% lap closes the tightest bend only on a level road. 4.5% would close it,
  with 15 cm more lap on every panel of every level; not changed without frames of 2.69 km.
- The Chemical Works' facing is the brightest thing near the road on a dark level. It is unlit; if it pulls the eye,
  the slab colour is one word in the kit (`cut_revet`). The Volcano's columns and ash were darkened once after the
  first real-game frames: under the game's own light they stood as the palest big thing on screen (the editor's
  captures are darker than the game and had hidden it).
- Under the Sea is now over the pass's guide of 8,000 placed pieces (8,261 with the refitted lead-ins). Frame time is
  still not measured.
- The Chemical Works' finale: the 51 bare lengths are where a panel's box came within 15 m of the other lap. The
  test is cautious (a box round a tilted panel); a finer one would fill most of them.
