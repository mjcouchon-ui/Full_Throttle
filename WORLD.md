# The world beside the track (3.1 and 3.2)

How the scenery, the sea, the sky and the things that move are made, and how to rebuild or tune them. The track
itself is locked: none of this changes the route, the road, the barriers or any collision.

## The ground under and round the road

`AFTTrack::BuildMeshes` (`FTTrack.cpp`) lays, besides the road:

| Piece | What it is | Notes |
|---|---|---|
| land | the ribbon each side of the road (the level's ground profile) | as before; has collision |
| bed | the land under and right beside the road | level where the road stands above the land, cut down under it (and under a pipe's tube) where it sinks, with banks back up to the land. Section 11, no collision |
| skirts | the land's outer edges running on, down to the floor | part of section 11 |
| floor | one sheet under the whole level, below its lowest land | Bayshore: the sea (`MI_BaySea`). Others: the land's outer colour |
| surface | Under the Sea only: the sea's surface far overhead | `MI_AbyssSurface` |

Why the bed exists: a banked turn rolls the road up to 57 degrees about its centre line, so its low edge sinks
11 m under the land. The land used to leave a 32 m strip open where the road sits; the chase camera, rolled with
the road, looked through it and saw the sky dome's below-the-horizon painting ("the sky comes through the ground
on turns"). Proof frames: `SourceArt/JamLivingWorld/clip_proof.jpg`.

Roadside props follow the land down where it falls away from the road (Bayshore's beach); where it rises they
keep the roadside height, as they always did. Dressed pieces (below) stand on the land wherever they are put.

The log line `FTTRACK level=N frames=... route=XXXXXXXX solid=XXXXXXXX` is a fingerprint of the route (every
frame's place and attitude) and of the four solid sections. A scenery change must leave both numbers as they were.

## Bayshore: the sea and the sky

- **Sea** `ft_world.bay_sea()` builds `M_BaySea` / `MI_BaySea` (`/Game/Maps/Bayshore/Materials`): flat turquoise
  tones in steps, swell that drifts, crests near the camera, a glitter path to the sun, the sky's warmth at the
  horizon. Parameters on the instance: `Deep`, `Mid`, `Light`, `Swell` (cm), `Steps`, `Speed`, `CrestAmt`,
  `GlintAmt`, `Sheen`, `Gain`.
- **Beach and town ground**: the level table in `ft_cel.py` (`LEVELS[0]['ground']`): the beach runs 300 m down
  to the sea, the town's ground reaches 640 m out, under the far skyline.
- **Sky**: the BayshoreSky plugin still draws the gradient, the sun, the far shore, the blimp and the gulls. Its
  photoreal cloud layers are off (`CloudLayers` 0 in `MPC_BayshoreSky`; 1 brings them back), so are its photoreal
  cloud cards (`CloudCardsShown` 0) and lens ghosts (`FlareStrength` 0); `HorizonWarmth` scales the golden band
  under the sun. Values: `SourceArt/BayshoreSky/unreal/bsky_params.py`; rebuild with that folder's
  `port_chain.ps1 -Stages "mat,copy"`.
- **Clouds**: eight chunky clouds are actors in `L_Bayshore` (folder `Sky`, `BayCloud_*`): three flat tones, flat
  bases, a slow drift. Move them freely in the editor. `ft_world.place_bay_clouds()` puts them back to the
  layout in `ft_world.CLOUD_LAYOUT`; `MI_BayCloud` has the colours (`LitC`, `MidC`, `ShadowC`, `BaseC`), the
  tone cuts and the drift.

## Under the Sea and the volcano

- `ft_world.build_abyss()`: the surface from below, caustics (a light function on the level's key light, put
  there by `ft_world.abyss_apply()` with the map open), light shafts, two fish schools, a whale, a ray and
  bubble strings. The swimming is in `M_AbyssFish`'s vertex shader (`Speed`, `Radius`, `WagAmp`, `FlapAmp` per
  instance); they are placed by rules at the end of level 6's list in `FTTrack.cpp`.
- `ft_world.build_volcano()`: `SM_Eruption` and `MI_Eruption`. `AFTTrack` puts one on every volcano on the
  horizon; the material plays a 75 s cycle from the engine clock: a rumble, the blast (a ring of fire thrown from
  the rim), the main event (an ash plume with a cap, lit from below by the fire and from the side by the level's
  light, lightning in it; five fountains; thrown fragments; lava running down the flanks), the aftermath, a quiet
  spell. No particles and no ticking: it costs the same at every quality level.
  Timing on `MI_Eruption`: `Period`, `Ignite`, `MainEnd`, `Quiet`, `Feed`, `Rise`, `Fade`, `RingTime`, `FlowTime`,
  `BombEvery`. Look: `AshDark`, `AshMid`, `AshLit`, `Fire`, `Core`, `Gain`, `Lightning` (the share of sixths of a
  second a puff flashes), `ToSun` (towards the level's key light), `Wind`, `DriftSpeed`.
  `Scrub`: set it to a second of the cycle to hold the eruption there (stills, tuning); -1 is the clock.
  Each cone's start second is set in `FTTrack.cpp` (custom primitive data 0).
  `SourceArt/Jam/unreal/29_eruption.py` films the cycle second by second from the road.

## The dressing: districts, sets, horizon

`FTDress.h` explains the three kinds of rule; `FTTrackDress.cpp` explains the words a table is written in. Each
level has one table, `Source/FullThrottle/FTDressL<n>.inl`, and one kit module, `Content/Python/ft_dress_<level>.py`:

| Level | Table | Kit | Assets |
|---|---|---|---|
| 0 Bayshore | `FTDressL0.inl` | `ft_dress_bay.py` | `/Game/Maps/Bayshore/Dress` |
| 1 Chemical Plant | `FTDressL1.inl` | `ft_dress_chem.py` | `/Game/Maps/ChemicalWorks/Dress` |
| 2 Nevada | `FTDressL2.inl` | `ft_dress_nevada.py` | `/Game/Maps/Nevada/Dress` |
| 3 Volcano | `FTDressL3.inl` | `ft_dress_volcano.py` | `/Game/Maps/Volcano/Dress` |
| 4 Theme Park | `FTDressL4.inl` | `ft_dress_park.py` | `/Game/Maps/ThemePark/Dress` |
| 5 Ice Mountain | `FTDressL5.inl` | `ft_dress_ice.py` | `/Game/Maps/Glacier/Dress` |
| 6 Under the Sea | `FTDressL6.inl` | `ft_dress_abyss.py` | `/Game/Maps/Abyss/Dress` |
| 7 City Center | `FTDressL7.inl` | `ft_dress_metro.py` | `/Game/Maps/Metro/Dress` |

To change what a stretch looks like, edit its district in the table and compile. To change or add a piece, edit
the kit module and run, in the editor's Python:

```python
import ft_dress
ft_dress.run(['chem'])                        # one level's kit
ft_dress.run(['chem'], only=['SM_TankBig'])   # one piece
```

Every piece goes through the road-clearance test the props use: one that would touch its own barrier is pushed
out, one that would touch another part of the road is left out, and near pieces keep out of loops, pipes,
corkscrews, jump gaps, run-ups and landings. The build logs `FTDRESS level=N ... pieces placed=A of B tried`
and, per mesh, what `FTCLEAR` moved or dropped.

**The sight line.** A dressed piece that stands more than 2 m over a road keeps 24 m from that road's centre
line (pushed out from its own road, left out beside another stretch). On the tightest bend (128 m radius) a
150 m sight line bulges 21 m inside the centre line, so the inside of every bend still shows that much road
ahead. Low things (fences, hedges, parked cars, sea grass) may stand right behind the barrier. The rule is in
`FTTrack.cpp` (`Clear`, its `Sight` argument); a table need not allow for it.

The finish pass's added detail (`ft_visual_art.refine`: service panels and facets sized by a piece's bounding
box) is skipped for dressing pieces; they take that pass's materials only (`ft_dress.export`).

`ft.Dress 0` (console) or `-ftnodress` (command line) builds levels without the dressing: the same build gives a
before and an after.

## Checking a change

The scripts are in `SourceArt/Jam/unreal/` (their headers say how to run them): `26_tour_editor.py` films the
same chase-camera poses on any level, `27_dress_build.py` rebuilds kits and films in one editor job,
`jam_play.ps1` runs the real game uncooked for frames at fixed race-clock times or for the game's benchmark,
`jam_tour.ps1` does the tour against a packaged build, `jam_sheet.py` makes contact sheets.
