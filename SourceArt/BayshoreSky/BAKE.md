# BAKE.md: how the Bayshore sky is baked

**State in the game (15 Nov 2026).** The baked cloud layers and cumulus cards described here are not drawn in the game since the game-jam restyle of 14 Nov (late): `CloudLayers` 0 and `CloudCardsShown` 0 in `unreal/bsky_params.py`. Level 1's clouds are eight stylized meshes in `L_Bayshore`. The current state is in `WORLD.md` at the checkout root ("Sky" and "Clouds"); check there before re-baking or tuning the layers or cards for the game. The commands below use paths in the scratch project `K:\Full Throttle\SkyScratch`, not in this repo.

The sky's expensive parts are rendered offline and shipped only as textures: the atmosphere, the cloud layers, the cumulus and the distant shore. Nothing in the packaged game simulates atmosphere or clouds. The bakes store **continuous, physically based lighting data**: sun and sky light separately, optical depth and opacity. The runtime lights them live (phase function, tints, exposure). There are no cel bands on the sky: `MF_SemiCelRamp` is inactive (`BandGradient` 1).

Tools: Blender 5.2 LTS (Cycles on the GPU, OptiX), with its bundled Python 3.13, NumPy, OpenImageIO and OpenVDB. Every bake is a script, so a preset is re-baked by running commands, never by hand.

| What | Script | Stages |
|---|---|---|
| Atmosphere strip, distant shore, sky light cubemap | `blender/bake_sky.py` | `atmo`, `hills`, `compose` |
| Cumulus cards (lit from three sun directions) | `blender/bake_sky.py` | `cumulus3` (shapes, renders, compose), `cumulus3compose` (compose only) |
| Cloud layers (altocumulus deck, cirrus) | `blender/bake_layers.py` | `maps`, `render`, `compose` |

- Textures ready to import, plus their metadata (`layers_meta.json`, `cumulus3_meta.json`, `bake_meta.json`): `bakes/<Preset>/`.
- Raw renders, VDBs and maps: `bakes/<Preset>/raw/`, with the cloud layers' in `raw/layers/` (the shipped round is tag `_r2f`). They are never copied into the game project.
- Reference renders (not shipped): `reference/<Preset>/`.
- Not kept in the repository since 15 Nov 2026, because the bake writes them again: `bakes/<Preset>/Master_<Preset>_Atmosphere_Equirect.exr` and, in `reference/<Preset>/`, the `Ref_*`, `Data_*` and `cumulus3_look` images. `preview_bake.py` and `compare_bake_runtime.py` read `Ref_<Preset>_PhotorealStrip.png` and `Ref_<Preset>_CloudCards.png`, so run `bake_sky.py --stages compose` first (it needs the raw renders in `bakes/<Preset>/raw/`).
- Conventions (axes, channel meanings, mesh list): `SPEC.md`.

**Coordinates:** Unreal azimuth is measured from +X toward +Y. Blender's sky node and camera use a compass azimuth (0 = +Y, +90 = +X), and Blender's Y is Unreal's −Y, so **Blender azimuth = 90° + Unreal azimuth**. The scripts do this conversion; give them Unreal angles.

## Re-baking the Day preset

The sun is at Unreal azimuth **30°**, elevation **6°** (golden hour, the owner's choice). It must match `unreal/bsky_params.py` (`SUN_AZ`, `SUN_EL`), and the scripts default to it. Run each step through `blender/run_blender.ps1`, which sets BelowNormal priority and writes the log to `Saved/AgentTemp/<Log>.log`:

```powershell
$b = "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\blender"
# 1. Atmosphere strip, distant shore and sky light (a few minutes)
powershell -NoProfile -File "$b\run_blender.ps1" -Script bake_sky.py -Log bake_day -PyArgs "--preset Day --az 30 --el 6 --stages atmo,hills,compose"
# 2. Cumulus cards (about 11 minutes: 8 VDB shapes, 32 renders, compose)
powershell -NoProfile -File "$b\run_blender.ps1" -Script bake_sky.py -Log cu3 -TimeoutSec 2400 -PyArgs "--preset Day --el 6 --stages cumulus3"
# 3. Cloud layers (about 5 minutes)
powershell -NoProfile -File "$b\run_blender.ps1" -Script bake_layers.py -Log layers -TimeoutSec 3000 -PyArgs "--stages maps,render,compose --tag _r2f"
# 4. Into Unreal: the textures, then materials, values and placement, then the game (unreal/port_chain.ps1)
powershell -NoProfile -File "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal\port_chain.ps1" -Stages "build,copy,apply,package"
```

- Compose-only reruns are about 10 s each and need no render: `bake_layers.py --stages compose --tag _r2f`, or `bake_sky.py --stages cumulus3compose`.
- `preview_layers.py` previews the layers in perspective outside Unreal (images in `bakes/Day/raw/previews`). Judge the final look in game, though: `unreal/play_uncooked.ps1`, or `play_port.ps1` on a packaged build.
- `30_materials.py` reads the runtime constants each bake recommends from its metadata when it builds the materials: the layers' multiplier on optical depth, and the cumulus cloud-base line. The exposures are look values in `bsky_params.py`, set from each bake's recommendation and then checked in game.

## What each bake makes

### Cloud layers (`bake_layers.py`)

- Two periodic slabs, seen from below by an orthographic camera looking straight up:
  - an altocumulus deck at 2.5 km, on a 24 km tile;
  - cirrus at 7 km, on a 48 km tile.
- The clouds are 2.5D height fields from periodic NumPy maps (base, top, density).
- Rendering: Cycles, 256 / 512 spp, 16 volume bounces. The phase function is the runtime's own (0.7 HG(0.8) + 0.3 HG(−0.2)), so the runtime can rescale thin cloud exactly.
- Light groups: a white sun lamp at 6°, and a multiple-scattering sky with no disc.
- Round 2 (14 Nov) is sparse: the clear sky between cloud groups is in the bake. Coverage:
  - deck opacity above 0.5 on 7% of the tile;
  - cirrus above 0.1 on 15%, in brush-stroke bundles.
- Outputs:
  - `T_BSky_Day_<Alto|Cirrus>Light.exr`, imported as BC6H: R = sun light and G = sky light per unit opacity, B = optical depth / 10;
  - `T_BSky_Day_<Alto|Cirrus>Alpha.png`, imported as BC4: opacity = 1 − exp(−τ).
- `layers_meta.json` records the tiles, altitudes, wind axes, the sun's angle in tile space, the recommended exposure (0.45) and the optical-depth multiplier (50).

### Cumulus cards (`bake_sky.py`, `cumulus3`)

- 8 VDB cumulus, with many thermals per card for the 4.8–11.5 km the cards are drawn at.
- Rendering: Cycles, 512 bounces, OpenImageDenoise.
- Each card is rendered with the sun behind it (φ 20°), at its side (φ 90°) and behind the camera (φ 160°), all at 6° elevation. The camera is pitched 6° so the base reads from below. A sky pass adds the sky light.
- Outputs:
  - `T_BSky_Day_CumulusSun.exr` (BC6H): R/G/B = the three sun lightings, with the sun/sky balance baked in;
  - `T_BSky_Day_CumulusAux.png` (BC7, linear): R = sky light (8-bit, dithered), G = opacity, B = thickness.
- `cumulus3_meta.json` records the cell layout, the cloud base line (v 0.80) and the recommended exposure (1.058).

### Atmosphere strip, shore and sky light (`bake_sky.py`)

- **`atmo`:** Blender's Sky Texture (`MULTIPLE_SCATTERING`, sun disc off, altitude 30 m, aerosol 1.6), rendered as a full-sphere equirectangular panorama at 8192 × 4096.
- **`hills`:** a polar height field, 3.2–19 km out, lit by the 6° sun, rendered as a strip from −10° to +6.5°. `--town 0` (the default) leaves out the stand-in skyline boxes, because Level 1 has its own skyline props. At runtime the ring is drawn at `HillsHeight` (0.35) of the baked height: a flat bay shore.
- **`compose`:** writes the following.
  - `T_BSky_Day_HorizonAtmo.exr`: the strip from −8° to +32°, BC6H.
  - `T_BSky_Day_Horizon.png`: the shore. R = sun-lit term, G = distance, B = kind, A = coverage.
  - `T_BSky_Day_SkyLight.hdr`: the sky light cubemap, long-lat 1024 × 512, BC6H. It is the atmosphere with the far cloud bank composited.
  - `Master_Day_Atmosphere_Equirect.exr` and `bake_meta.json`.
  - Two retired textures, which are no longer imported: `T_BSky_Day_HorizonClouds.png` (the far bank) and `T_BSky_Day_CloudAtlas.png` (the first cloud cards). Delete them after a compose. Their raw renders, `raw/bank_strip.exr` and `raw/card_*.exr`, come from the retired `bank` and `atlas` stages.
  - **Known gap:** the far bank in the sky light cubemap was rendered with the earlier 35° sun. It only tints the ambient light near the horizon; re-run `--stages bank,compose` to bring it to 6°.

## Texture budget

Measured on disk (`.uasset`, source data included) after import, 14 Nov, against the 40 MB sky budget:

| Texture | Format | On disk |
|---|---|---|
| T_BSky_Day_CumulusSun | BC6H 2048² | 10.1 MB |
| T_BSky_Day_AltoLight | BC6H 2048² | 9.1 MB |
| T_BSky_Day_CirrusLight | BC6H 2048² | 7.9 MB |
| T_BSky_Day_HorizonAtmo | BC6H 4096 × 1024 | 5.7 MB |
| T_BSky_Day_CumulusAux | BC7 2048² | 2.4 MB |
| T_BSky_Day_AltoAlpha | BC4 2048² | 0.6 MB |
| T_BSky_Day_CirrusAlpha | BC4 2048² | 0.5 MB |
| T_BSky_Day_Horizon | BC7 4096 × 512 | 0.3 MB |
| T_BSky_Day_SkyLight | BC6H cube | 0.15 MB |
| T_BSky_Blimp_Masks | BC7 1024 × 512 | 0.07 MB |
| **Total** | | **36.7 MB of 40 MB** |

## Other source art

| Asset | Source | Command |
|---|---|---|
| All sky meshes: blimp (LOD0 1,916 tris, LOD1 540, LOD2 188), its retired ink hull (540) and the gull (16) | `blender/make_meshes.py` → `meshes/*.fbx` | `blender.exe -b --factory-startup --python-exit-code 1 -P make_meshes.py` |
| `T_BSky_Blimp_Masks` (1024 × 512: R BAYSHORE lettering in Bungee, G accent, B occlusion) | `blender/make_blimp_masks.py` → `textures_src/T_BSky_Blimp_Masks.png` | `python -I make_blimp_masks.py` (Pillow); import BC7, sRGB off, mips, clamp U, wrap V |
