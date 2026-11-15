# BayshoreSky: shared build spec

Everything that bakes or builds a sky asset follows this file, so the bakes, materials and presets fit together.
Matt's port is not available yet (checked 14 Nov 2026 06:59 UTC), so these are our own conventions; each is listed in `Plugins/BayshoreSky/INTEGRATION.md` for him to accept or change.

## Look

80s beach town, daylight, semi-cel: a few clear light bands with soft, narrow edges, a gentle gradient inside each band, thin ink outline on outer silhouettes only. Dome gradient, haze and sun halo stay smooth. No photographic texture.

## Coordinates

- Unreal: X forward, Y right, Z up, centimetres, left-handed. Azimuth `az` is measured from +X toward +Y; elevation `el` up from the horizon. Direction = (cos el · cos az, cos el · sin az, sin el).
- Blender is right-handed: Unreal (x, y, z) = Blender (x, −y, z). So Unreal azimuth `az` is Blender azimuth `−az` (measured from +X toward +Y in Blender).
- Preset **Day** sun: `az = 30°`, `el = 35°` → Unreal direction to the sun (0.7094, 0.4096, 0.5736).

## Baked textures (preset Day)

All data textures are linear and hold continuous values. Banding happens at runtime in `MF_SemiCelRamp`.

| Texture | Size | Projection | Channels | Import |
|---|---|---|---|---|
| `T_BSky_Day_HorizonAtmo` | 4096 × 1024 | cylindrical: u = (az + 180°)/360°, so u = 0.5 looks along +X and u grows toward +Y; v = 0 at el +32°, v = 1 at el −8°, linear in elevation | RGB scene-linear radiance of the clear atmosphere (no clouds, no sun disc) | HDR, BC6H, no mips, wrap U, clamp V |
| `T_BSky_Day_HorizonClouds` | 4096 × 1024 | same as above | far cloud bank: R coverage (0–1), G sun-lit term (0–1), B ambient/density term (0–1), A soft edge falloff (0 outside → 1 well inside) | BC7, sRGB off, no mips, wrap U, clamp V |
| `T_BSky_Day_CloudAtlas` | 2048 × 2048 | 8 cells, 2 columns × 4 rows, each 1024 × 512; cell index = row · 2 + column, row 0 at the top; each cloud centred with at least 24 px of empty margin | same four channels as the far bank | BC7, sRGB off, mips on, clamp |
| `T_BSky_Day_Horizon` | 4096 × 512 | cylindrical, same u; v = 0 at el +6.5°, v = 1 at el −10° | R baked sun-lit shading (0–1), G distance (0 nearest hill → 1 farthest), B kind (0 hills, 1 skyline), A silhouette coverage | BC7, sRGB off, mips on, wrap U, clamp V |
| `T_BSky_Day_SkyLight` | 256 per face | long-lat .hdr → cubemap | radiance of the baked sky with clouds, sun disc removed | HDR cubemap |
| `T_BSky_Blimp_Masks` | 2048 × 1024 | blimp UV0 (hull u 0–0.9, fins and gondola u 0.93–0.99) | R lettering (BAYSHORE) and G accent as soft edge fields (thresholded in the material), B baked occlusion, A 1 | BC7, sRGB off, mips on, never stream |

Cloud channel meaning, the same for cards and the far bank:

- **R coverage**: view transmittance turned into opacity, 0 clear sky, 1 fully opaque cloud.
- **G sun-lit**: brightness under the preset sun only, white cloud, normalised so the brightest lit tops reach about 0.95 and deep self-shadow sits near 0.1. Continuous, no banding.
- **B ambient/density**: brightness under a uniform white sky only (soft occlusion; thick cores and undersides darker), normalised 0–1.
- **A edge**: a blurred silhouette (gaussian of coverage > 0.5): 0.5 on the silhouette, rising to 1 inside and falling to 0 outside over a few texels. Materials turn it into screen pixels with `fwidth`, so the soft edge and the ink outline keep a constant on-screen width.

## Meshes

Exported from Blender as FBX in centimetres, +X forward, Z up, no collision, custom normals kept.

| Mesh | What |
|---|---|
| `SM_BSky_Dome` | inward-facing unit sphere (placed at 60 km), 64 × 32. `M_SkyDome` (Is Sky) paints all of it by view direction: maths gradient above +32°, the baked horizon strip (atmosphere and far cloud bank) from −8° to +32°, the sea below the horizon |
| `SM_BSky_HorizonRing` | inward-facing open cylinder, unit radius (placed at 8 km), el −2.5° to +6.5°, 128 × 2 (below −2.5° the level's own terrain fills the view); textured by direction from the ring centre |
| `SM_BSky_CloudCard` | one quad, 1 m × 0.5 m, centred, facing −X, UV0 0–1 |
| `SM_BSky_SunQuad` | one quad, 1 m × 1 m, centred, UV0 0–1 |
| `SM_BSky_FlareGhosts` | six unit quads in one mesh, ghost index 0–5 in vertex colour R as index/5, UV0 0–1 each |
| `SM_BSky_Blimp` | under 3,000 triangles, 60 m long along +X, LOD0–LOD2, UV0 for `T_BSky_Blimp_Masks`; plus `SM_BSky_BlimpOutline`, the same hull with reversed winding for the ink outline |
| `SM_BSky_Gull` | about 40 triangles, 1.2 m wingspan along Y, body along +X; vertex colour R = wing flap weight (0 at body, 1 at wingtip) |

## Runtime parameters (`MPC_BayshoreSky`)

Scalars and colours: the live list with the Day values is `SourceArt/BayshoreSky/unreal/bsky_params.py` (also in `Plugins/BayshoreSky/INTEGRATION.md`). `SeaEnabled` and `LowQuality` are written by `BP_BayshoreSky` (options and scalability), not by presets.

Colours and vectors: `SunDirection`, `ZenithColor`, `MidColor`, `HorizonColor`, `HazeColor`, `SunColor`, `SunHaloColor`, `CloudLitColor`, `CloudShadowColor`, `CloudRimColor`, `OutlineColor`, `HillNearColor`, `HillFarColor`, `SkylineColor`, `SeaDeepColor`, `SeaHorizonColor`, `GlitterColor`, `BlimpBodyColor`, `BlimpAccentColor`.

`MF_SemiCelRamp(v)`: with n = `BandCount` levels, s = `BandSoftness`, g = `BandGradient`:
`x = saturate(v)·(n−1)`, `w = max(s/2, fwidth(x))`, `q = (floor(x) + smoothstep(0.5 − w, 0.5 + w, frac(x))) / (n−1)`, result `lerp(q, v, g)`. On low quality (`LowQuality` = 1) the softness drops to a quarter (harder edges).

## Source art layout

`SourceArt/BayshoreSky/blender/` scripts and .blend files, `bakes/Day/` raw EXR and the processed textures ready to import, `unreal/` the Python that builds the Unreal assets, `fonts/` the lettering font and its licence, `reference/` photoreal reference renders (not shipped).
