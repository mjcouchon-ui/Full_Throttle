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
- **Banked bends**: Bayshore, Theme Park and City Center carry faces on the cut bank (the method: a tilted piece laid
  on the cutting, `SM_BankWaves` in the Bayshore kit is the plainest example). Chemical Works, Nevada, the Volcano and
  Ice Mountain do not yet: there the inside of a bend is still the cutting's wall and sky. First thing to do next.
- **Bayshore**: the opening 3.2 km gained least (flat pieces stand level, and that stretch is hills); banks under
  40 degrees and the first 60 m of each steep bend are bare; the downtown is one piece repeated.
- **Chemical Works**: the finale's doubled circuit (14.0 to 17.4 km) has one sparse ground row; a few glowing ponds
  stand 25.6 m from another stretch of road (the pass's rule was 30 m for anything lit).
- **Nevada**: the wall bend at 5.2 km is mostly sky; three designs carry the whole casino approach.
- **Volcano**: loops are black; the mine reads at 9 to 10 km and is small elsewhere; far lava is still strokes on black.
- **Theme Park**: coaster loops and corkscrews unchanged (the track keeps everything 39.6 m from the rails there);
  the bank beds are loud (one number in `_bank_len`); jump valleys are plain.
- **Ice Mountain**: the skyline is still mostly similar cones; about 135 more horizon instances that never cull.
- **Under the Sea**: open sand on the first 8 km's hills is thin; the pale wall at 20 s is the level's own cut bank.
- **City Center**: straight street views are unchanged (the level's own bright towers dominate); most of the 83 blade
  signs stand inside those towers at 30 m.
- **Movement** was run on every level and seen in stills only.
- **Frame time was not measured on any level** (about 800 to 2,000 more instances a level; no idle GPU that night).
  Bayshore, Nevada and Under the Sea are near the pass's guide of 8,000 placed pieces.

## What the workers asked of the shared files (none was changed)

1. `FTTrackDress.cpp`: a **Ground** word that lands like a Scatter and stays through loops, pipes, corkscrews and
   gaps like a Far (three workers used Far rules with a drop as a stand-in), and an "outside of the bend" flag.
2. `FTTrack.cpp`, the dressing's `Put`: pitch a piece to the land along the road (on grades a level piece floats at
   one end: Ice Mountain falls 7 in 100), and keep near pieces off the cutting beside a banked bend.
3. `AFTTrack::RoadSpan`: coaster loops and corkscrews reserve 38.1 m below the rails; the rails sweep about 16 m.
4. `ft_dress.run()` should call each kit's `animate()` / `life()`, so an export cannot leave things standing still.
5. `Clear()`: a 30 m rule for lit pieces.
