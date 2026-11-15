# R1 repair toolbox and M2 missile carrier

Justin approved **R1 + M2** on 2026-11-14, using the existing Full Throttle art and the supplied brief as references. The orange carrier exposes three pointed missiles; the green toolbox has an open carrying arch and a raised, original double-ended wrench on both broad faces. Chamfers, dark structural breaks and restrained emissive strips match the existing semi-cel art.

## Runtime scope

| Pickup | Asset path (unchanged) | Triangles | Sections | Overall bounds, cm |
|---|---|---:|---:|---|
| Missile carrier | `/Game/Characters/Pickups/SM_Crate` | 1,484 | 4 | 105 × 152 × 142 |
| Repair toolbox | `/Game/Characters/Pickups/SM_Wrench` | 1,376 | 4 | 102 × 142 × 131 |

The previous meshes had 60 and 84 triangles, respectively, with three sections each. These are mesh-section counts, not a measured total GPU draw-call count: shadow, depth and other passes can submit additional draws. Nanite is explicitly disabled. Both props remain inside the unchanged 200 × 200 cm logical pickup footprint throughout their ±18° yaw.

The asset exporter updates the existing static meshes in place. It retains their existing simple collision data; gameplay proxies still have collision disabled and collection uses the unchanged track-space hitbox. Spawn rules, placement, lanes, rewards, Pit Crew values and part restoration are unchanged.

`FTEntity::Setup` clears stale material overrides when a pooled actor becomes a pickup. This prevents prior vehicle Alert materials from replacing the dedicated pickup palette.

## Materials and motion

- Dedicated orange, green, guards and ivory instances inherit the existing `M_Paint`. Shared palette instances and `M_Paint` itself are unchanged.
- `M_PickupGlow` is an isolated opaque, unlit material. Scene-referred emissive is `Color × Gain × (1 + Pulse × sin(Time × 4.2))`, with Gain 3 and Pulse 0.12. There is no HDR clamp. Unlit surfaces use the existing cel pass's bypass; the shared cel material is unchanged.
- Idle motion changes only the mesh component: a 4–16 cm bob at 2.1 radians/second and ±18° yaw at 0.7 radians/second. Phase differs between pickup types without calling gameplay RNG.
- The existing unshadowed point light is reduced from 250 to 2 cd, raised from 120 to 220 cm, and disabled on Low. Its 7 m radius is unchanged. The original intensity washed out the raised ivory and dark surfaces in actual game captures. No additional light, beacon, decal, texture or translucent loop was added.
- Collection emits eight shrinking colored square chips for 0.18–0.36 seconds. The masked unlit material has no textures. Each Niagara system uses a fixed seed of 88, pool prime size 2 and maximum pool size 8. Low effects quality skips the burst entirely.
- Setup submits mesh and Niagara PSO precache requests before pickups appear. Niagara's asset PostLoad and first world tick prime the configured component pools. This implementation alone is not proof of hitch-free first use; see validation evidence.

## Tuning

| Control | Default | Purpose |
|---|---:|---|
| `ft.PickupMotion` | 1 | Set to 0 for static-mesh comparison |
| `ft.PickupBurst` | 1 | Set to 0 to isolate collection FX cost |
| `ft.PickupLight` | 2 cd | Existing unshadowed light; 0 disables it |
| `sg.EffectsQuality` | project quality | 0 always disables the collection burst and local light |
| `MI_Pickup_Amber` / `MI_Pickup_Mint`: Gain, Pulse | 3, 0.12 | Local HDR trim intensity and modulation |

Change procedural forms, colors and effect distributions in `Content/Python/ft_powerups.py`, then regenerate. Keep the four-section and 5,000-triangle checks in place.

## Reproduction

In the intended project's Unreal Python console:

```python
import ft_powerups
ft_powerups.build()
```

`ft_export.fleet()` also invokes this builder. It regenerates both meshes, dedicated material instances, the isolated glow/chip materials and both collection systems. It writes `Saved/Powerups/mesh-report.json`. The script uses the project's Geometry Script model helpers and the engine's existing stateless Niagara burst template. It requires the project's FullThrottleEditor module, Geometry Script and Niagara plugins.

For an unattended build, use a rendering-enabled Python commandlet (material compilation needs an RHI). Use forward slashes in `-script` paths because Unreal's Python argument parser interprets backslash escape sequences:

```powershell
& 'K:/UE_5.9/Engine/Binaries/Win64/UnrealEditor-Cmd.exe' `
  'K:/Full Throttle/PowerupsWorktree/full-throttle-unreal/FullThrottle.uproject' `
  '-run=pythonscript' `
  '-script=K:/Full Throttle/PowerupsWorktree/full-throttle-unreal/SourceArt/Powerups/build_assets.py' `
  '-AllowCommandletRendering' '-RenderOffScreen' '-unattended' '-nosound'
```

## Capture and measurement

`Tools/powerups_capture.ps1` launches a packaged Development build at 3440 × 1440, native screen percentage, fixed random seed and 60 Hz simulation, with dynamic resolution disabled. Each run gets private save data, a launch manifest and a log. The real viewport screenshot path includes the cel post-process. Existing outputs are never overwritten.

```powershell
./Tools/powerups_capture.ps1 -BuildRoot '.../Windows' -Output '.../after' -Mode shots
./Tools/powerups_capture.ps1 -BuildRoot '.../Windows' -Output '.../timings' -Mode bench
```

Screenshot runs are visual evidence only. Bench mode checks five idle GPU samples before launching and refuses to run if any exceed 5%. It takes no screenshots. Use actual `gpu_ms`/`gpu_p99_ms` for comparisons: fixed simulation makes the stock FPS/frame-delta fields unsuitable as wall-clock performance measures. Hardware, live scalability and output resolution are retained in the game's benchmark JSON.

The opt-in, Development-only inspection mode creates separate visual proxies on the road, outside the gameplay entity list. It never changes normal spawn rules, rewards or collision. Its camera renders through the actual game viewport and cel pass:

| Console setting | Capture |
|---|---|
| `ft.PickupReview 1` | Close turntable, 30 degrees/second |
| `ft.PickupReview 2` | Both props at 100 m ground distance, 86° horizontal FOV |
| `ft.PickupReview 3` | Staged approach at 54 m/s (~121 mph), then visual burst demonstration |
| `ft.PickupReviewYaw 180` | Opposite camera azimuth |
| `ft.PickupReviewSun 1` / `2` | Face toward / away from the brightest visible directional light; overrides orbit/yaw |
| `ft.PickupReview 0` | Remove inspection proxies and resume the normal camera |

Inspection captures are explicitly staged, not gameplay collection/reward tests or normal-play GPU benchmarks. Natural auto-drive captures provide the separate gameplay evidence. Level indexes are zero-based: L0 is Bayshore (the brief's Level 1); L1 is Chemical Plant (Chemical Works in the art scripts).

For compatibility checks with uncooked content, supply `-EditorProject '.../FullThrottle.uproject'` and use the engine's `Binaries/Win64` directory as `-BuildRoot`. This path accepts shots only and records the game module hash as well as the editor executable hash.

## Validation and cost evidence

Visual validation and performance validation have separate receipts. Do not treat screenshots or successful compilation as a performance approval:

| Check | Recorded result |
|---|---|
| C++ Development Game and Editor builds | Passed on integrated main 2.8 (`89eb95d`); Niagara pool priming uses the engine's exported/automatic paths |
| Packaged Windows Development build | Passed on 2.8; cook completed with zero errors and zero warnings |
| Procedural asset generation | Passed, zero errors; mesh report verifies both triangle/section limits |
| Python syntax and git whitespace | Passed |
| Independent code/shader review | Pooled material and RNG findings fixed; follow-up review in `SourceArt/Powerups/CODE_REVIEW.md` |
| All-level before/after, Epic/Low | Completed on 2.7: 16 runs per build, 114 native frames per run (3,648 total); all non-benchmark gameplay log events match in all 16 pairs. |
| Final 2.8 staged inspection matrix | Completed: all 16 level/quality combinations exited 0 with 25 frames each (400 total). Independent art review covered every native 100 m frame and every far/approach/close/FX contact sheet. |
| Bayshore golden-hour compatibility | Six uncooked viewport runs exited 0 with 21 frames each: toward/away sun and 100 m, Epic/Low. Independent review used frame 017 after Level Instance streaming. Temporary integration `8cbdacb` combines the feature with Claude's committed sky/billboards `f99e5de`; that content is not in this feature PR. Direct sun-disc overlap was not demonstrated. |
| Reference/style critique and grayscale test | Close day/night, Epic/Low and grayscale reviewed. At 100 m, orange/green detection is supported; function recognition from geometry alone does not pass. See `ART_REVIEW.md`. |
| Idle GPU baseline/after/technique costs | Deferred by Justin: “Finish visuals; leave timings pending.” No +0.2 ms claim. |
| Cold first-pickup hitch validation | Timing pending; precache implementation is not a measured hitch-free guarantee. |

Concepts, exact prompts, provenance, source references, credits and development history are in `SourceArt/Powerups`. No shared config or shared material asset is changed by this feature.

Final compatibility review found no new pickup disappearance, whiteout or close-form blocker in the loaded golden-hour sky. Low toward-sun produces tan trim highlights and a broad road reflection; identity stays intact. The native 100 m limitation remains. The source branch merges cleanly with current main 2.9 (`f039571`) in a merge-tree check; compilation and packaged visual evidence above are from 2.8, not a new 2.9 package.

### Technique costs to measure

| Normal-play build | Epic GPU time | Low GPU time | Cold first-use hitch |
|---|---|---|---|
| Baseline on the same main revision | Pending idle run | Pending idle run | Pending cold run |
| R1 + M2 feature | Pending idle run | Pending idle run | Pending cold run |
| Feature minus baseline | Not measured; +0.2 ms budget unverified | Not measured | Not certified |

| Technique | Structural cost | Epic GPU delta | Low GPU delta |
|---|---|---|---|
| Mesh replacement | Missile +1,424 triangles; repair +1,292; each +1 material section | Pending idle run | Pending idle run |
| Local HDR trim | Existing fourth section; opaque/unlit, no texture sampling | Pending isolation run | Pending isolation run |
| Idle motion | Two mesh transform updates per visible pickup tick; logical actor position unchanged | Pending isolation run | Pending isolation run |
| Existing local light | One unshadowed light per pickup, reduced to 2 cd | Pending isolation run | Disabled |
| Collection chips | Eight short-lived masked sprites per collection | Pending peak collection run | Disabled |
| PSO warm-up / Niagara pool | Two hidden mesh and two inactive effect components during setup; effect pools primed | First-use timing pending | First-use timing pending |

The +0.2 ms budget and absence of a cold first-use hitch are **not certified**. Justin explicitly requested finishing visual evidence while leaving timings pending. Before approval, compare baseline and feature on the same main revision, native 3440 x 1440, idle RTX 4080 SUPER, with the maximum normal-play pickup count. Test each technique toggle independently. Do not time the staged review actors.

Materials preserve scene-referred HDR values, but the supplied viewport images are SDR-tonemapped captures. HDR display output has not been visually certified.

The portable review is `SourceArt/Powerups/evidence/index.html`. Rebuild its labeled contact sheets, video and source-hash manifest with Python and ffmpeg:

```powershell
python Tools/powerups_evidence.py --captures 'K:/Full Throttle/PowerupsReview' --output SourceArt/Powerups/evidence
```

## Open questions for runtime acceptance

- Justin's final look approval follows the actual-game evidence; R1 + M2 concept approval is already recorded.
- Is established orange/green recognition sufficient at 100 m? The close grayscale test separates the forms, but geometry alone does not reliably communicate function at the native distant pixel size. A requirement for distant function recognition without color would need another art iteration.
- When an idle GPU is available, finish the baseline/feature/technique cost table and cold first-use check before claiming the performance budget.
