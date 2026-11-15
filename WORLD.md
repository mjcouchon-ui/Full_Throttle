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

**The land's own surface (3.4).** The land and the bed wear `/Game/Maps/<Level>/Materials/MI_Land` when it exists
(`ft_world.land_materials()` builds `M_Land` in `/Game/Maps/Land` and one instance a level): slabs with joints,
ripples, or cracked ground, drawn in the material over the level's own ground colours (the vertex colours, with
`M_Ground`'s washes and grain kept). On banked turns, elevated stretches, loops and corkscrews the camera sees
mostly land, and nothing tall may stand near the road there; this dresses those views without placing anything.
Per level in `ft_world.LAND_LEVELS`: `Kind` (0 slabs, 1 ripples, 2 cracked ground), `KindWarm` (the same choice
for ground of a warm colour: Bayshore's sand ripples while its town ground is laid stone), `Cell` (cm), `Angle`,
`Joint`, `Dark`, `Vary`, `Warp`, `Duty`, `Amount`. The pattern fades out before it can shimmer (by how many cells
a pixel covers). `ft.Land 0` builds the levels with the plain ground: the same build gives a before and an after.

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

`FTDress.h` explains the kinds of rule (Scatter, Site and Far here; Bank in the next section); `FTTrackDress.cpp`
explains the words a table is written in. Each
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

## The cut banks inside banked bends (3.4)

A banked bend rolls the road about its centre line. Its inner edge sinks (11.5 m at 57 degrees) and the bed cuts
the land down to it: one straight slope from under the low barrier up to the land's inner edge 16 m out, 4 to
13 m long. The chase camera, rolled with the road, looks straight at that cutting. It was a third of the frame in
the land's one colour, and no Scatter or Site piece can stand on it: they stand level, at the uncut land's height.

A **Bank** rule has the track face the cutting itself:

```
Zone(TEXT("Cut banks"), 0.f, 99.f);
	Bank(TEXT("SM_CutStrata"), 4, 2, 0, 1.f, 5.5f);         // per slot, every, phase, chance, run min m
	Bank(TEXT("SM_CutStrata2"), 4, 2, 1, 1.f, 5.5f);
	Bank(TEXT("SM_CutLedge"), 4, 1, 0, 1.f, 2.4f, 5.5f);    // ... run min m, run max m
```

- `BuildMeshes` remembers, for every frame, where the bed's last straight run up to the land's edge starts
  (`AFTTrack::BedCut`: the very line the bed is drawn with). Nothing of the track reads it.
- `BuildProps` lays a panel on that run every `45 m / per slot` of road: turned to the slope, its long side along
  the land's edge, pitched to the grade, stretched along to its share of the bend and up the slope to the cutting's
  length, lapped 3% over the next. So one flat piece serves every bank angle and a hill (Ice Mountain falls 7 in
  100 all the way; two of the Chemical Works' hairpins climb 1 in 8 while banked 57 degrees).
- Every and phase count panels, so two designs can take alternate panels. `run min` and `run max` give a tall
  design to the deep cuttings (5.5 m and more: bends banked 32 degrees and up) and a low one to the rest; the run
  is measured at the panel's middle. Under a gentle bank the bed's foot is hidden behind the barrier: the run is
  counted from 30 cm under the barrier's top, and a bend that shows under 2.4 m of cutting (banked under about
  19 degrees) is left alone.
- Through a bend's lead-in and lead-out the cutting deepens under a panel: its foot sinks as the bank comes on,
  its top stays at the land's edge. The panel is a rectangle and is kept inside the cutting at both of its ends:
  its top edge runs half a metre under the land's edge from panel to panel, and its bottom edge stops where the
  cutting starts at its shallower end, which leaves a bare wedge at the foot of the deeper end. The facing steps
  down along the foot, like a wall's footing on falling ground. (Sized from its middle alone, as it first was, a
  panel there stood a corner 30 to 60 cm out of the ground at the land's edge.)
- A piece that would leave over 80 cm bare at the foot of its deeper end, or would stand more than 12 cm off the
  cutting (a lead-in at its quickest), is laid as two halves, a half as two quarters, a quarter as two eighths:
  the panel's own design, squeezed along the road. An eighth (1.4 m of road) takes whatever wedge is left. A
  shorter piece lies only where its own middle has two fifths of the run its design was drawn for.
- The same trim applies in the body of a bend: a flat piece is a chord of the bend, so panels there come out up
  to 9 cm shorter than their middle alone would give, and up to 31 cm on the Chemical Works' climbing hairpins.
- A panel is tested against OTHER parts of the road only, and left out if it would touch one (the Chemical Works'
  finale laps beside itself). Its own road is the bed it lies on. Panels cast no shadow.

Kits draw a panel flat, as if the bank were the floor (x along the road, y up the slope, z out of the bank):
`Content/Python/ft_dress_bank.py` has the rules and the helpers. Unlit, no arrows, nothing a hand thin.

| Level | Tall panels | Low panels |
|---|---|---|
| Chemical Works | `SM_CutRevet`, `SM_CutRevet2`: concrete slabs, the teal main, a hazard placard or leaking outlets | `SM_CutKerb`, `SM_CutKerb2` |
| Nevada | `SM_CutStrata`, `SM_CutStrata2`: seven beds of red rock, sage and fallen blocks | `SM_CutLedge` |
| Volcano | `SM_CutBasalt`, `SM_CutBasalt2`: basalt columns, ash on their heads | `SM_CutAsh` |
| Ice Mountain | `SM_CutIce`, `SM_CutIce2`: layers of blue ice, splinters, a cornice; the mountain's rock | `SM_CutSnow` |
| Under the Sea | `SM_CutReef`, `SM_CutReef2`: beds of reef rock and packed sand, coral crusts | `SM_CutShoal` |

Bayshore, the Theme Park and City Center keep the second pass's faces: pieces built already tilted and bent,
stood by one-piece Sets where the bank is at its full angle (`ft_dress_bay.py`, "the banks of the banked bends").
The first 60 m of each of their bends, and their bends banked under 40 degrees, are still bare; a Bank rule and a
flat piece would cover those too.

The build logs `FTBANK level=N pieces laid=A on B lengths of cutting (C in shorter pieces; left bare, wholly or in
part: too bent D, at another part of the road E, too little cutting F) along G m`: a length is 11.3 m of one side's
cutting that a rule wanted to face, a piece is a whole panel, a half, a quarter or an eighth, and "too little
cutting" counts the lengths where a cutting begins or ends inside the panel.
`ft.DressBank 0` builds without the panels: the same build gives a before and an after.

## Checking a change

The scripts are in `SourceArt/Jam/unreal/` (their headers say how to run them): `26_tour_editor.py` films the
same chase-camera poses on any level, `27_dress_build.py` rebuilds kits and films in one editor job,
`jam_play.ps1` runs the real game uncooked for frames at fixed race-clock times or for the game's benchmark,
`jam_tour.ps1` does the tour against a packaged build, `jam_sheet.py` makes contact sheets.
`jam_audit.ps1` proves the save, pay and pickup fixes of `SourceArt/Integration/AUDIT.md` in the running game, and
`jam_late.ps1` counts the loads a race still has to make from disk (below).

`jam_play.ps1` writes the frame size into the checkout's `Saved/Config/WindowsEditor/GameUserSettings.ini` for the
run and puts the file back: the game re-applies its saved settings at every map start, so `-ResX`/`-ResY` alone
size only the first window.

## Shadows from the dressing (3.3)

A dressed piece casts a shadow only if it stands within 70 m of the road and is over 2.5 m tall; flat pieces, small
ones and the middle distance cast nothing (the horizon never did). The component is one to a mesh, so a mesh used
both near and far casts everywhere, and a component the level's own props made first keeps its setting. The log line
`FTDRESS level=N ... shadows=A of B` says how many of the instances in the dressing's components cast.
`ft.DressShadows 0` (set before the level is built) gives the old behaviour for a comparison from the same build.

## Loading: nothing read from disk mid-race (3.4)

Matt's 3.4 (`UFTGameInstance::WarmGameplayAssets`) loads every sound, effect and vehicle mesh in the background
while the title is up and holds them for the session; the game mode logs `FTHITCH` for any frame over 40 ms.
`jam_late.ps1` drives all eight levels on the autopilot (wrecks, a boss, a wrap) and counts, per level, the
synchronous loads the engine still reports during the race and those hitch lines.
