# Level 1 (Bayshore) buildings

What the buildings pass added to Level 1, how to tune it, and how to rebuild it. Branch `level1-buildings`, stacked on
`level1-sky` (it reads the sky's `MPC_BayshoreSky`).

## What changed

| Part | Before | Now |
|---|---|---|
| Windows | flat blue paint (`M_Paint`), banded by the cel pass | `M_BayGlass`: a lit room behind every pane, the sky reflected in the glass, the low sun flashing in single panes |
| Neon | `M_Paint` with Glow under the cel pass's `EmisCut`, so it was banded as bright paint | `M_BayNeon`: HDR tubes with mains shimmer; here and there a stretch buzzes and dips |
| Hotels | one decorated face (looking back down the road), bare sides, back and roof | every side dressed; four different buildings (below) |
| Far skyline | boxes with white and blue stripes | every floor a band of panes lit one by one, on all four faces; parapets and tanks |

The 16 Bayshore props are ordinary instanced meshes, as before (see "Nanite" below).

The walls are still palette paint, so the cel pass bands and inks them as before. `M_PP_Cel` is unchanged.

### Why glass and neon escape the cel bands

`M_PP_Cel` does not band a pixel whose GBuffer base colour is black (`ft_cel.py`, `bl > 0.004`). `M_BayGlass` and
`M_BayNeon` have a black base colour and no specular, so only what they emit reaches the screen, in scene-referred HDR.
Nothing is clamped.

They are Default Lit, not Unlit, on purpose (`ft_buildings._emissive_only`). An Unlit material writes no normal, and
the cel pass's ink stage then reads (-1, -1, -1) for its pixels: it inked every edge between glass or neon and a wall
facing +X, +Y or +Z, and none on a wall facing the other way, so window rims and thin tubes changed with the road's
heading. As lit materials they carry the wall's own normal and the ink treats them like the wall.

Two things to know:
- The height fog still lies over glass and neon, while the cel pass rebuilds walls from their base colour within 600 m
  and so drops the fog on them. Far windows are a little hazier than the walls round them.
- The glass needs the BayshoreSky plugin (`MPC_BayshoreSky`). After `ft_sky.remove(0)` it goes on mirroring the
  golden-hour sky.

### The four hotels (`ft_city.py`)

- **CABANA** (`SM_HotelPink`), **BAYSHORE** (`SM_HotelLav`), **ALIBI** (`SM_HotelPeach`): `hotel()`. Ribbon windows with
  eyebrow ledges, sills and mullions on the front and on the side that looks at the road; a lobby level with a canopy
  and shopfronts; a glass-block stair strip, a side door and a neon blade sign on the road side; plain window bands on
  the back and far side; a parapet, air handlers, a water tank and a stair head house on the roof; a stepped crown
  with the name in neon; the corner tower in rings of glass block, with a stepped cap in neon hoops and a spire.
- **LIDO** (`SM_HotelMint`): `liner()`, Streamline Moderne. The corner toward the road is rounded; the ribbon windows
  and their ledges run round it unbroken; speed lines between the floors; portholes up the road side; a ship's rail, a
  round pilot house, a mast with a flag and a red light on the roof; the name stacked down a pylon.

The names are our own, in block capitals drawn stroke by stroke in `ft_city.GLYPHS` (no font, no real hotel's sign).
Change them in `ft_export.bayshore()`.

Placement is untouched: the same rules in `FTTrack.cpp` place the same meshes. The log line
`FTCLEAR level=0 props kept=2088 ... left out of the road=1` and the billboard layout hash are the same as before.

## How it is built

- `ft_buildings.Panes` collects window quads into one mesh section with two UV sets. UV0 = (pane index + across, floor
  index + up). UV1 = (pane width, band height) in metres; their signs pick the kind of glass:
  - both positive: a room behind the pane (interior mapping, no textures);
  - width negative (`room='flat'`): a pane lit on its own with nothing behind it (far skyline, portholes, curved glass);
  - height negative (`room=False`): glass block, milky, the whole floor's ring lit as one.
  - Floor index below 0 is always lit (the lobby level).
- `ft_city.Wall` addresses a wall as (along, up, proud of the wall), so the same facade code dresses any side.
  `walls()` gives a box body's four. Walls must stand upright: the shader rebuilds the pane's frame from the face
  normal and world up.
- `ft_art.Model.part = ft_buildings.NEON_PART` while a builder draws neon: those primitives become `M_BayNeon` sections
  (`MI_BayNeon_<colour>_g<glow>`), one per colour.
- `ft_export.bayshore()` passes the glass and neon as `extra` sections to
  `save_mesh(..., uv_sets=2, no_shadow=('Glass', 'Neon'))`: flat glass and tubes lying on a wall cast no shadow.

### Nanite

- No mesh of the game was ever Nanite. `save_mesh(nanite=True)` set only the create options' `enable_nanite`; the engine
  copies the options' `nanite_settings` struct onto the mesh, and that struct's own switch stayed off. A GPU profile of
  Level 1 before this pass shows no Nanite pass at all.
- With that fixed, the 16 Bayshore props were measured as Nanite meshes: slower (see "Cost"). The Nanite passes have a
  fixed cost that props of a few thousand triangles do not earn back.
- So `save_mesh` now defaults to `nanite=False`, which is what it always produced: re-exporting any other group gives
  the same meshes as before. `nanite=True` now works, for a mesh heavy enough to want it; its materials then need the
  Nanite usage flag (`ft_buildings.paint_nanite()` sets it on `M_Paint`; `M_BayGlass` and `M_BayNeon` have it).
- `M_Paint` is unchanged by this pass.

## Tuning (`MI_BayGlass`, parameters of `M_BayGlass`)

| Parameter | Default | What it does |
|---|---|---|
| `Interior` | 2.6 | brightness of a lit room, scene-linear (the cel pass's lit wall band is about 1.4-2.4). Brighter only turns panes cream beside sunlit pastel walls: the lamps are saturated instead |
| `LitStart`, `LitEnd`, `Dusk` | 0.34, 0.72, 240 s | share of rooms lit at the start, once the evening has come on, and how long that takes. Each room also thinks again every 4-10 minutes and fades over half a second |
| `RoomDepth`, `RoomH` | 4.2 m, 3.0 m | the room behind a pane (depth varies 0.7-1.35x per room) |
| `Bands` | 0.35 | 0 = smooth light falloff inside, 1 = four hard steps |
| `Dress` | 1 | blinds and curtains |
| `Ambient` | 0.16 | sky light in an unlit room |
| `F0`, `Reflect` | 0.3, 1.4 | how much sky the glass mirrors head-on, and the reflection's strength |
| `Warp` | 0.035 | how far panes sit out of true, so each catches its own patch of sky |
| `Glint`, `GlintPow` | 26, 900 | HDR peak and tightness of the sun's flash in a pane |
| `BlockSun` | 1.1 | sunlight on glass block |
| `Frame` | 0.045 m | ink frame round each pane |
| `Tint`, `InkC` | | glass tint; the frame's colour (the cel pass's ink) |

The reflection is the sky dome's own gradient, fed by `MPC_BayshoreSky` (zenith, mid, horizon and haze colours, sun
direction and colour, halo): retune the sky and the glass follows. `MPC_BayshoreSky.LowQuality` = 1 switches the rooms
off (flat lit panes).

`M_BayNeon`: `Glow` x `GlowGain` (14) is the tube's HDR level, `Hum` the mains shimmer, `Faulty` the share of neon
that buzzes and dips (by stretches about a metre long, not by tube: on a sign a few letters dip together).

Blinds, curtain folds, glass-block joints and the floor lamp's pole are drawn only while they are wider than about two
pixels and fade to their mean beyond (the hotels stand 100 m and more from the road, 6-7 cm of wall to a pixel).

## Rebuild

In the editor, after `ft_mats.build_all()` and with the BayshoreSky plugin in place:

```python
import ft_buildings, ft_export
ft_buildings.build_materials()      # M_BayGlass + MI_BayGlass, M_BayNeon
ft_export.run(['bayshore'])         # the 16 props, with their glass and neon sections
```

Nothing in the map changes: `AFTTrack` places the props when the level loads.

## Cost

Packaged Development builds, the game's own benchmark (`-ftbench`, Level 1, 40 s of auto-drive), 3440x1440, RTX 4080
SUPER, no other Unreal process running, two runs each (14 Nov 2026). GPU ms per frame:

| Build | Epic | Low |
|---|---|---|
| Before (`level1-sky` with main 2.8 merged), first session | 6.75, 6.83 | 1.21, 1.22 |
| The buildings pass, props as Nanite meshes | 7.50, 7.56 | 1.64, 1.65 |
| Before, measured again beside the next row | 6.76, 6.66 | 1.22, 1.19 |
| The buildings pass, props not Nanite, glass and neon still Unlit | 6.74, 6.63 | 1.22, 1.23 |
| Before, measured again beside the last row | 6.71 | not run |
| **The buildings pass as it is (after the review's fixes)** | **6.73** | not run |

- Everything in this pass together (glass, neon, every side dressed, about 1.5 million more triangles along the level)
  costs nothing measurable at Epic or at Low. Budget was +1.0 ms.
- The last two rows are one run each, at Epic only: another session kept the GPU busy and the Low runs were given up.
  Low was measured on the row above them, before the two materials changed from Unlit to emissive-only Default Lit.
- Nanite would have cost +0.74 ms at Epic and +0.43 ms at Low (a third of the Low frame). Per pass (three `profilegpu`
  dumps at the same auto-drive times, averaged): Nanite visibility buffer 0.31 ms, virtual shadow maps +0.40 (the Nanite
  path), a velocity-buffer clear 0.16, Nanite streaming readback 0.15, Nanite base pass and depth export 0.19. The
  ordinary base pass, where glass and neon shade, moved by 0.04 ms.
- `M_BayGlass`: 800 pixel-shader instructions as the editor counts a lit material (539 while it was unlit: the
  difference is the lit base pass's own code, not the glass). It samples no texture, so it adds no texture memory.
  `M_BayNeon`: 340 (121 unlit).
- Triangles: Pink 3,988 (was 2,244), Lav 5,208 (2,864), Peach 2,914 (1,380), Mint 5,020, skyline about 3,700 (2,724).
- Sections (instanced draws) per mesh: hotels 7 (were 9), the liner 9, the skyline 12 (was 12). Glass and neon
  sections cast no shadow.
- No textures, so nothing over 50 MB and no new texture memory.

Measured with `SkyScratch/SourceArt/Buildings/unreal/bld_perf.ps1` and `bld_prof.ps1` (raw results in
`SkyScratch/DevLog/perf/buildings/`).
