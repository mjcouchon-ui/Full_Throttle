# BayshoreSky: the Level 1 sky for the Full Throttle UE 5.9 port

**State in the game (15 Nov 2026).** The hyper-real look this document describes was superseded by the game-jam restyle of 14 Nov (late). In the game the plugin still draws the gradient, the sun, the far shore, the blimp and the gulls; its photoreal cloud layers (`CloudLayers` 0; 1 brings them back), its photoreal cumulus cards (`CloudCardsShown` 0) and its lens ghosts (`FlareStrength` 0) are off, and `HorizonWarmth` is 0.5. Level 1's clouds are eight stylized meshes placed in `L_Bayshore` (`BayCloud_*`, `ft_world.CLOUD_LAYOUT`), not part of this plugin. The current state is in `WORLD.md` at the checkout root ("Sky" and "Clouds"); the values are in `SourceArt/BayshoreSky/unreal/bsky_params.py`. Read the sections on the cloud layers, the cards and the ghosts below as a record of what the plugin can draw, not of what is on screen.

BayshoreSky is a self-contained, content-only plugin: every asset lives under `/BayshoreSky/` and references only itself and Engine content. It needs no C++ and no other plugin, and it replaces the port's own Level 1 sky (`NightSky`, `Sun`, `SkyLight`). It was built in a scratch project (`K:\Full Throttle\SkyScratch`) and integrated into the port on branch `level1-sky` (section 10).

**The look** (owner, 14 Nov): a hyper-real golden-hour sky over the bay, matched to the owner's reference photo of Brickell at sunset (`SkyScratch/HandBack/refs/R7_brickell_target.png`): a lavender-grey horizon, a blue sky that is already deep 20° up (measured on the photo; the chase camera sees about 0–25° above the horizon), an orange-to-pink glow on the sun side, a few pink cirrus streaks, scattered altocumulus and cumulus clusters lit salmon-pink by the low sun, with clear blue between them. **No cel shading on the sky** (the buildings and nearly everything else are cel shaded; the sky is not). **Alive while driving**: every cloud layer parallaxes and drifts. **HDR**: the sky is scene-referred and never clamped, so an HDR display shows the glow and the sun above SDR white.

## 1. Enable it

1. Copy `Plugins/BayshoreSky/` into the port's `Plugins/` folder. Keep the source art beside it: `SourceArt/BayshoreSky/` (Blender and Unreal build scripts, bakes; Git LFS in SkyScratch).
2. Add `{ "Name": "BayshoreSky", "Enabled": true }` to the `.uproject`'s `Plugins` list (it is also `EnabledByDefault`).
3. For HDR output, the project needs `r.AllowHDR=True` (`DefaultEngine.ini`, `[/Script/Engine.RendererSettings]`); HDR is then switched on per player (section 6).
4. `Plugins/BayshoreSky/Config/Game.ini` adds `/BayshoreSky/Bake` to **Directories to Never Cook**, so the live Sky Atmosphere / Volumetric Cloud bake map never reaches a packaged build.

## 2. Place it

**One actor:** `/BayshoreSky/Blueprints/BP_BayshoreSky`, at the centre of Level 1's play area, at sea level. In the port it comes in as a Level Instance of `/BayshoreSky/Maps/LI_BayshoreSky` (`ft_sky.apply(0)`, section 10).

| Component | What | Notes |
|---|---|---|
| `SkyDome` | one sky-pass draw: the art-directed gradient with its HDR golden-hour glow, the baked atmosphere strip (−8° to +32°), the HDR sun disc, two **photoreal parallax cloud layers** and the backdrop sea (which reflects the sky, its glow and the cloud deck) | 60 km, `Is Sky`; `ShowSea` (MPC `SeaEnabled`) turns the sea off where the level has its own water |
| `HorizonHills` | baked distant shore, with the cloud deck's shadows drifting across it | 20 km ring, base pass; `ShowHorizonHills`; drawn at `HillsHeight` (0.35) of the baked height, a flat bay shore as in the reference |
| `Clouds` | **38 photoreal cumulus cards** in 14 clusters, 17–33 km out (`SourceArt/BayshoreSky/unreal/bsky_layout.py`) | Cycles renders lit from three sun directions; the card blends them by its angle to the sun |
| `SunGlow`, `FlareGhosts` | the sun's halo and lens ghosts | dimmed by cloud crossing the sun and by anything nearer than the dome |
| `SunDisc` | retired, hidden in game | the disc is drawn inside the dome, under the clouds |
| `Blimp` | the BAYSHORE blimp | circles the actor at 300 m, 2.5 km radius; its ink hull `BlimpOutline` is retired |
| `Gulls` | 7 gulls that follow the car along the road and scatter as it passes | |
| `SunLight` | golden-hour sun: elevation 6°, azimuth 30° (rotation pitch −6°, yaw 210°), 7 lux, warm (1.0, 0.74, 0.52) | light-shaft bloom, volumetric-fog scattering 1.6 with volumetric shadows; `UseBuiltInSunLight` |
| `SkyLight` | sky light from the baked HDR cubemap, no real-time capture | volumetric scattering 1; `UseBuiltInSkyLight` |

### The cloud layers (photoreal, parallax)

The dome casts each view ray onto two curved cloud shells (a virtual 300 km radius, so the layers reach the horizon at a finite distance):

| Layer | Altitude | Tile | Drift | Textures |
|---|---|---|---|---|
| Altocumulus deck | 2.5 km | 24 km | `CloudDriftSpeed` along `WindDirection` | `T_BSky_Day_AltoLight` (HDR BC6H: R sun, G sky, B optical depth), `T_BSky_Day_AltoAlpha` (BC4 opacity) |
| Cirrus | 7 km | 48 km | 2.2× faster, 15° off the deck wind (wind shear) | `T_BSky_Day_CirrusLight`, `T_BSky_Day_CirrusAlpha` |

Both are **Cycles volume renders** of a periodic cloud slab seen from below (`SourceArt/BayshoreSky/blender/bake_layers.py`), so they tile seamlessly in both directions. As the car drives, the deck slides against the cirrus and the horizon exactly as real cloud does.

**Clear sky between the clouds:** the reference is mostly clear blue between groups of cloud. The round-2 bakes carry that themselves (deck opacity above 0.5 on 7% of the tile, cirrus above 0.1 on 15%, in bundles), so the runtime **cloud fields are off by default** (`AltoCoverage` = `CirrusCoverage` = 1). Below 1 they thin each layer further in large, soft fields (two octaves of hashed value noise in the layer's own tile space, drifting with it: deck patches about 9 km across, cirrus bundles about 20 × 10 km along the fibres), roughly to that share of the sky, and skip the layer's texture reads outside them. The sea reflection, the hill shadows and the sun occlusion use the same fields, so they always agree with the sky.

**Filtering:** the layers are sampled with an isotropic mip taken from the larger axis of the pixel footprint, so cloud seen edge-on near the horizon blurs into haze as real distant cloud does. Anisotropic filtering kept the cirrus fibres sharp along the view, and in game they read as speed lines converging on the road's vanishing point.

**Volumetric lighting, live:** each layer stores the light scattered from the sun and from the sky separately, plus its optical depth. The shader applies a two-lobe Henyey–Greenstein phase function (g 0.8 forward, −0.2 back), relative to the bake's straight-up view, to the thin parts. Thin cloud toward the sun glows far above SDR white (the silver lining), while thick cloud scatters evenly. How thin "thin" is comes from the bake itself: `30_materials.py` reads each layer's recommended multiplier on the optical-depth channel from `bakes/Day/layers_meta.json` when it builds the material, so a re-bake that rescales that channel cannot silently change the look. The slant path through each layer thickens it toward the horizon. Distant cloud fades toward the sky colour behind it (aerial perspective, twice the gradient's `Haze`). The deck and the cirrus have their own exposures (`CloudExposure`, `CirrusExposure`). With `LowQuality` the cirrus layer is skipped.

### The cumulus cards

The 8 cumulus shapes (cauliflower tops, flat bases) were rendered in Cycles three times each: sun **behind** the cloud (back-lit, φ 20°), at its **side** (φ 90°) and **behind the camera** (front-lit, φ 160°), into `T_BSky_Day_CumulusSun` (HDR BC6H, R/G/B = the three) and `T_BSky_Day_CumulusAux` (BC7: sky light, opacity, thickness). The vertex shader computes φ between the view and the sun for each card and blends the three. Cards toward the sun come out dark with glowing rims, cards away from it evenly pink, as in the reference. Each card's mirror is fixed when it is placed (custom data 5), so a card never flips while the car drives.

The cards are 2.4× their instance scale and are lifted so the cloud base sits at the instance height (the bake's base line, `cloud_base_v` in `cumulus3_meta.json`, is read when the material is built). Their exposure (`CardExposure` 1.058) comes from the same file. The layout lives in `bsky_layout.py`: 38 cards in 14 clusters, 17–33 km out. Their show order is dealt round-robin across the clusters, so a lower `CloudCardsShown` (Low quality halves it) thins every cluster instead of emptying one side of the horizon. `41_bp_tweaks.py` writes the layout to the Blueprint template, and `82_place_cumulus.py` writes it to the placed actors, because their ISM instances do not follow the template.

**With your own level layout:** add a Level Instance pointing at `/BayshoreSky/Maps/LI_BayshoreSky` (or a Data Layer under World Partition). The test map `/BayshoreSky/Maps/L_BayshoreSkyTest` (camera rail with loops, rolls and corkscrews) is not meant to ship.

## 3. Volumetric lighting

Costs measured in the packaged game (section 8).

| What | Where | Cost (GPU, Epic, 3440×1440) |
|---|---|---|
| Light-shaft bloom from the sun: rays from the bright sky round the sun, broken up by cloud and buildings | `SunLight` (`41_bp_tweaks.py`): bloom scale 0.16, threshold 1.2, tint (255, 214, 176), occlusion off | 0.18 ms |
| Cloud lighting: phase function, silver lining, slant path | the dome shader | in the sky cost |
| Cloud crossing the sun dims the halo and lens ghosts | `SunGlow`, `FlareGhosts` vertex shaders | in the sky cost |
| Fog matched to the sky: the lavender horizon away from the sun, an orange glow toward it | `HeightFog`, set by `ft_sky.apply(0)`: inscattering luminance (0.50, 0.40, 0.58), directional inscattering (1.70, 0.48, 0.20), exponent 6, from 1.5 km | none |
| **Off by default:** volumetric fog lit by the golden sun, with volumetric shadows | `HeightFog`: `ft_sky.LEVELS[0]['fog']['volumetric'] = True` (distance 800 m, extinction 0.55, scattering distribution 0.72; the sun's volumetric scattering is already 1.6 with volumetric shadows) | 1.1 ms; 0.35 ms with `r.VolumetricFog.GridPixelSize 16` |

Volumetric fog is off because, in driving views, it showed as a slightly hazier veil rather than shafts, for 1.1 ms. The port's 2.6 had also already turned Bayshore's volumetric fog off, because it greyed the cel bands and the ink (`ft_v25.py`).

## 4. HDR

- The sky is **scene-referred HDR**. The golden-hour glow peaks around 2–3× SDR white, the sun disc around 20×, and thin cloud toward the sun goes higher still. Nothing in the sky materials is clamped.
- Baked radiance is stored as HDR (BC6H): the atmosphere strip, the sky light cubemap, the cloud layers' sun and sky light, and the cumulus sun light. The cumulus sky light is the exception: it sits in an 8-bit channel of `T_BSky_Day_CumulusAux` (normalised, dithered), next to the data channels (opacity, thickness).
- On an SDR display the tonemapper rolls the highlights off, so the photo-like white-hot core is expected there.
- **To play in HDR:** Windows HDR on, then `r.HDR.EnableHDROutput 1` in the console, or `-ExecCmds="r.HDR.EnableHDROutput 1"` on the command line. Both work in Development builds only. The project allows it (`r.AllowHDR=True`). A Shipping build has no console, so players need a setting that calls `UGameUserSettings::EnableHDRDisplayOutput`; the game's Options menu has none yet.

## 5. Settings that may need changing

| Setting | Why |
|---|---|
| Exposure | The sky is unlit; its brightness is `MPC_BayshoreSky.SkyBrightness`, tuned in game against Level 1's own exposure (manual, bias 0: `ft_level.build` sets manual, `ft_cel.apply_level(0)` sets the bias). With other exposure, change `SkyBrightness` (and `SunIntensity`), not the textures. |
| Sun and sky light | With the level's own lights (`UseBuiltInSunLight` / `UseBuiltInSkyLight` off), set the level's sun to the preset's `SunLightRotation` (pitch −6°, yaw 210°), or the sky's sun and the shadows disagree. |
| Height fog | `ft_sky.apply(0)` matches the level's fog to the sky (section 3); `ft_sky.remove(0)` gives it back to `ft_cel.apply_level(0)`. |
| Substrate | Built with legacy shading (Substrate off, the 5.9 default). |
| Lumen / ray tracing | Not required; sky meshes are hidden from ray tracing and distance fields. |

## 6. Tuning at runtime (no graph or code changes)

All live values are in one Material Parameter Collection, `/BayshoreSky/Materials/MPC_BayshoreSky`. A look is a Primary Data Asset (`DA_BayshoreSkyPreset_Day`) that `BP_BayshoreSky.ApplyPreset` writes into the MPC, the material instances and the lights; it runs from the construction script and BeginPlay. The values below come from `SourceArt/BayshoreSky/unreal/bsky_params.py`, and `45_preset_values.py` writes them in. Values marked MPC-only are not stored in the preset, so adding one needs no Blueprint rebuild.

| Parameter | Value | What it does |
|---|---|---|
| `BandCount` | 3 | semi-cel bands (inactive: BandGradient 1) |
| `BandSoftness` | 0.4 | semi-cel band softness (inactive) |
| `BandGradient` | 1 | 1 = no bands: the sky is not cel shaded |
| `CloudDriftSpeed` | 600 | cloud drift, cm/s, along WindDirection (cirrus 2.2x) |
| `OutlineWidth` | 0 | ink outline width (0: no ink on the sky) |
| `Haze` | 0.3 | horizon haze; distant cloud fades toward the sky behind it at twice this |
| `CloudCardsShown` | 0 | how many cumulus cards are drawn (dealt round-robin across the clusters) |
| `SunDiscSize` | 1.05 | angular radius of the sun disc (deg) |
| `SunHaloSize` | 12 | angular size of the halo (deg) |
| `SunRayStrength` | 0 | painted rays in the halo (0: the light shafts do this now) |
| `FlareStrength` | 0 | lens ghosts |
| `GradientMidPoint` | 0.14 | elevation (0-1 of 90 deg) of the mid colour; the zenith colour is reached at 3x it |
| `PanoramaExposure` | 0.7 | brightness of the baked atmosphere strip |
| `PanoramaStylize` | 0.9 | how far the atmosphere strip is pulled toward the art-directed gradient (1 = gradient only) |
| `CloudRimStrength` | 0.8 | (legacy) rim strength of the old procedural deck |
| `SeaGlitterStrength` | 1 | glitter path and glints on the backdrop sea |
| `LowQuality` (set by the Blueprint) | 0 | set to 1 by the scalability hook |
| `CloudEdgeSoftness` | 3 | soft edge of the hills, in pixels |
| `SkyBrightness` | 0.9 | overall sky emissive multiplier (tuned in game against Level 1's manual exposure, bias 0) |
| `BlimpSpeed` | 0.012 | blimp angular speed (rad/s) |
| `BlimpRadius` | 250000 | blimp path radius (cm) |
| `BlimpOutlineWidth` | 0 | blimp ink hull width (0; the hull is retired) |
| `HazeDistance` | 2.5e+06 | distance over which clouds, blimp and gulls fade into the haze (cm) |
| `CloudAmbientGradient` | 0.7 | (legacy) ambient gradient of the old cel cards |
| `SunDistance` | 4e+06 | distance of the halo and ghost quads (cm) |
| `GullFlapSpeed` | 7 | gull wing beat |
| `SunIntensity` | 9 | sun disc brightness (HDR; about 20x SDR white) |
| `SeaEnabled` (set by the Blueprint) | 1 | the backdrop sea on or off (written by ShowSea) |
| `CloudExposure` (MPC-only) | 0.45 | exposure of the photoreal altocumulus deck (HDR) |
| `CirrusExposure` (MPC-only) | 0.45 | exposure of the photoreal cirrus layer (HDR) |
| `CardExposure` (MPC-only) | 1.058 | exposure of the cumulus cards (HDR) |
| `CloudTauScale` (MPC-only) | 1 | optical depth at which a layer stops following the phase function (thin cloud glows toward the sun, thick cloud scatters evenly) |
| `CumulusSkyNorm` (MPC-only) | 1 | extra gain on the cumulus sky light (1: the bake's sun/sky balance as stored, cumulus3_meta.json) |
| `AltoCoverage` (MPC-only) | 1 | share of the sky the deck's cloud fields cover (clear blue between them; 1 = the bake everywhere) |
| `CirrusCoverage` (MPC-only) | 1 | share of the sky the cirrus bundles cover (1 = the bake everywhere) |
| `HillsHeight` (MPC-only) | 0.35 | the distant hills drawn at this share of their baked height (a flat bay shore, as in the reference) |
| `CloudLayers` (MPC-only) | 0 |  |
| `HorizonWarmth` (MPC-only) | 0.5 |  |
| Colours | | `ZenithColor` #4A6CB4, `MidColor` #7F8FCB, `HorizonColor` #B9A8D2, `HazeColor` #F4B8B2, `SunColor` #FFD9A6, `SunHaloColor` #FF9C78, `CloudLitColor` #F09DB0, `CloudShadowColor` #8784BA, `CloudRimColor` #FFA28E, `OutlineColor` #6A4E7C, `HillNearColor` #5F6280, `HillFarColor` #9C90AE, `SkylineColor` #E7CFC6, `SeaDeepColor` #2C97AA, `SeaHorizonColor` #8CCFD0, `GlitterColor` #FFD7A8, `BlimpBodyColor` #F2EFF2, `BlimpAccentColor` #E8456B, `BlimpLetterColor` #1E3F8A, `GullColor` #FBF6F2 |
| `SunDirection` | azimuth 30 deg, elevation 6 deg | must match the bakes |
| `WindDirection` | (1, 0.3) | the deck drifts along it; the cirrus 15 deg off it |
| Lights (preset) | sun 7 lux, colour (1.0, 0.74, 0.52), rotation pitch -6 yaw 210; sky light 1.2 | |

What needs a re-bake instead (`SourceArt/BayshoreSky/BAKE.md`): the sun position (the atmosphere strip, the sky light, the hills, and the cloud layers' and cumulus lighting), cloud shapes and coverage.

## 7. Scalability

- **Low effects quality** (`GameUserSettings` visual effects quality 0), on BeginPlay: `LowQuality` = 1. That drops the cirrus layer, the lens ghosts and the rays, halves `CloudCardsShown` and hides `FlareGhosts`. The check runs once, at BeginPlay. The title screen runs in Level 1's world, so a quality change made there (or in the pause menu) reaches the sky only on the next level load.
- **Material quality Low** (`QualitySwitch`): the lens ghosts compile out.
- **Volumetric fog**, when switched on, follows the engine's shadow quality (off at Low and Medium).

## 8. Performance

The game's own benchmark (`-ftbench`, `play_port.ps1 -Mode bench`, `perf_ab.ps1`), Level 1, a packaged Development build at 3440×1440, RTX 4080 SUPER, idle GPU, 40 s per run, two runs each, 14 Nov. "No sky" is the original Level 1 (`84_port_sky_off.py`).

| Case | GPU ms (median of the run) | Frame ms |
|---|---|---|
| Epic, no sky | 8.45 / 8.59 | 9.22 / 9.33 |
| Epic, the sky as shipped (volumetric fog off) | 8.63 / 8.57 | 9.39 / 9.35 |
| Epic, the sky with volumetric fog on | 9.70 / 9.74 | 10.32 / 10.37 |
| — and `r.VolumetricFog.GridPixelSize 16` | 8.94 / 9.00 | 9.55 / 9.61 |
| — and light shafts off (`r.LightShaftQuality 0`) | 9.58 / 9.50 | 10.21 / 10.11 |
| — and a milder shadow LOD (`r.Shadow.Virtual.ResolutionLodBiasDirectional -0.5`) | 9.64 / 9.65 | 10.23 / 10.27 |
| Low, no sky / the sky | 1.62, 1.63 / 1.66, 1.65 | 2.62, 2.89 / 2.64, 2.71 |

- **The sky as shipped costs about 0.1 ms GPU at Epic** over the original Level 1 (dome, cloud layers, cumulus cards, the low sun's shadows, light shafts), and 0.03 ms at Low.
- Volumetric fog would add 1.1 ms (0.35 ms on the High grid).
- The light shafts cost 0.18 ms.
- **Shadow warning.** In Development builds the game shows `[VSM] Non-Nanite Marking Job Queue overflow` on screen. The original Level 1, with its sun at 50°, does not. At 6° the shadows are about 11× longer, and Level 1's ~2,100 non-Nanite, shadow-casting props pile up in light space. It is a performance warning, but the milder shadow LOD that removes most of it gained nothing measurable (row above), so the shipped shadow settings are unchanged. Making the props Nanite (the buildings pass) removes the cause. Shipping builds do not show on-screen messages.

## 9. Rebuilding everything from scripts

`SourceArt/BayshoreSky/unreal/` holds the build scripts, run in an offscreen editor (`ue_run.ps1 -Editor -Script <file>`, no window; one Unreal job per project at a time, guarded by `Saved/ue.lock`, which records its owner, so a stale lock is taken over). `ue_run.ps1` works on the project the scripts sit in (SkyScratch, or the game's copy), unless `-Project` says otherwise. The game's copy of the tooling is for reference and small rebuilds; the full chain (`port_chain.ps1`) runs from SkyScratch.

1. Bakes (Blender, `SourceArt/BayshoreSky/BAKE.md`): `bake_sky.py` (atmosphere, sky light, hills, and the cumulus stages `cumulus3` / `cumulus3compose`) and `bake_layers.py` (the cloud layers). `make_placeholders.py` writes stand-ins in the final channel layout, for building before the bakes exist.
2. `20_import_meshes.py` → `21_import_textures.py` → `30_materials.py` (also builds the MPC) → `40_blueprints.py` (only for a new Blueprint; delete the three Blueprint/preset assets first) → `41_bp_tweaks.py` → `45_preset_values.py` → `82_place_cumulus.py` → `50_test_map.py` / `52_level_instance.py` / `55_bake_map.py` (only when rebuilding those maps) → `60_capture.py` (screenshots).
3. Port: `port_chain.ps1 -Stages "build,copy,apply,package"` does the following and stops at the first failed step:
   - builds here and regenerates this document's parameter table (`doc_params.py`);
   - copies the plugin and source art into the port, under the port's lock;
   - runs `82_place_cumulus.py` and `80_port_apply.py` (`ft_sky.apply(0)`) in the port's editor;
   - packages.

   `-Stages "mat,copy,play"` is the quick look loop (about 5 minutes): materials and values, the copy, and in-game shots of the port run uncooked from the editor binary (`play_uncooked.ps1`, 1280×720).
4. Evidence: `play_port.ps1 -Mode shots|bench` plays the packaged Level 1 off-screen, writing auto-drive screenshots into `HandBack/ingame/<tag>/` or the game's own benchmark into `DevLog/perf/ingame/<tag>.json`. `perf_ab.ps1` builds the with-sky and without-sky packages side by side and benchmarks them (section 8). Run benchmarks only on an idle GPU.

## 10. In the port (branch `level1-sky`)

| Change | What |
|---|---|
| `Plugins/BayshoreSky/` | the plugin, as built here |
| `SourceArt/BayshoreSky/` | Blender and Unreal build scripts, bakes (no raw renders) |
| `FullThrottle.uproject` | +4 lines: `{ "Name": "BayshoreSky", "Enabled": true }` at the end of `Plugins`, nothing reformatted |
| `Config/DefaultEngine.ini` | +2 lines: `r.AllowHDR=True` and its comment |
| `README.md` | +5 lines: the "Level 1 sky" note |
| `Content/Python/ft_sky.py` | `ft_sky.apply(0)` adds the Level Instance `BayshoreSky` to `L_Bayshore` at the play area's centre (X 1.4 km, Y 6.2 km, sea level, unrotated). It switches off `NightSky`, `Sun` (or `Moonlight`, its name before `ft_cel` renames it) and `SkyLight`, and matches the `HeightFog` to the sky, with volumetric fog off. `ft_sky.remove(0)` undoes it: it resets the fog fields only the sky sets, then reruns `ft_cel.apply_level(0)` |
| `Content/Maps/Bayshore/L_Bayshore.umap` | the result of `ft_sky.apply(0)` |

The old actors stay in the map, switched off, because `ft_cel.apply_level(0)` finds them by label. The order is `ft_level.build(0)` (which rebuilds the map from scratch), then `ft_cel.apply_level(0)`, then `ft_sky.apply(0)`. Running `ft_cel.apply_level(0)` again afterwards resets the fog to its own values, so run `ft_sky.apply(0)` after it again.
