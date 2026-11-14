# FULL THROTTLE 2.1: the Unreal Engine 5.9 game

The 80s spy car chase (web app `full-throttle`, 1.7.2) rebuilt as a native Unreal Engine 5.9 game for Windows,
C++ plus Blueprints. WispOS opens the packaged build as the native app `full-throttle-unreal`
(`../full-throttle-unreal-app`). The web app and its art pipeline (`../full-throttle-art`) are untouched.

## Folders (WispOS Work together sections)

| Section | Folder | What is in it |
|---|---|---|
| Maps | `Content/Maps/Bayshore`, `ChemicalWorks`, `Nevada` | one folder per level: the map, its props (Nanite meshes) and its sky/material instances |
| Characters | `Content/Characters` | the vehicles: `Kestrel` (the player car, its 7 detachable parts, wheels, missile), `Syndicate`, `Choppers`, `SnakeEyes`, `Traffic`, `Bosses`, `Pickups` |
| UI | `Content/UI` | `BP_FTHUD` (the canvas HUD: title, clapper, dashboard, director, menus, rebinding) |
| Code | `Source`, `FullThrottle.uproject` | `FullThrottle` (game) and `FullThrottleEditor` (editor tools) modules |
| Config | `Config` | renderer (Lumen, Nanite, VSM, TSR), physics substepping, vehicle cvars, packaging |
| (add if wanted) | `Content/FX`, `Content/Audio`, `Content/Core`, `Content/Python`, `Tools` | Niagara and Chaos destruction assets, music and SFX, shared materials and the game mode, the editor scripts that built the art |

No file in the project is over 50 MB. `Binaries`, `Intermediate`, `Saved` and `DerivedDataCache` are made on each computer.

## How it is made

- **Track:** `AFTTrack` integrates the 1.x section lists (`FTData.cpp`) into frames and builds the road, walls, neon,
  glass pipes and ground as procedural mesh chunks, and the roadside props as Nanite instances.
- **Car:** `AFTPlayerCar` is a Chaos Vehicles car: four-spring suspension, RWD engine and gearbox, rear-tyre drift, arcade
  assists (cvar `ft.Arcade`, a bitmask) that hold it to loops, pipes and walls. Damage knocks its parts off as rigid bodies.
- **Game:** `AFTGameMode` runs the scenes, foes, bosses, director, scoring, slow motion, the highlight reel and the
  chase camera. Barrels and crates break into Chaos geometry collections; wrecks are burning rigid bodies.
- **Art:** every model is built in code. `Content/Python/ft_*.py` makes the meshes, materials, Niagara systems and maps in
  the editor (`ft_export.py`, `ft_mats.py`, `ft_fx.py`, `ft_level.py`). `Tools/sfx_synth.py` makes the sound effects.

## Build and test

- Engine: Unreal Engine 5.9.0 (`EngineAssociation` "5.9", `C:\Program Files\Epic Games\UE_5.9`). Since 2.1 nothing here uses 5.8.
- C++ needs Visual Studio with "Game development with C++". UE 5.9 wants MSVC 14.44.35211+ or 14.50.35723+ (14.51 is banned
  by Epic for a miscompile) and Windows SDK 10.0.22621 or newer.
- What 2.1 takes from 5.9: Chaos Vehicles' wheel fixes (zero radius, NaN torque curves, divide-by-zero in the wheel force,
  teardown at exit) come with the rebuild; Niagara systems are recompiled with 5.9; Nanite's split simple/complex culling and
  Lumen's clip-sphere card culling on Low are 5.9 defaults; `r.SkyLight.RealTimeReflectionCapture.FreezeAfterCycles=2`
  (DefaultEngine.ini) stops recapturing the static night sky once it has converged. `r.Lumen.Reflections.HardwareRayTracing`
  no longer exists in 5.9 and was removed. 5.9's Lumen World Cache and LumenPT need hardware ray tracing; this project
  uses software Lumen, so they are off.
- Material usage flags: the cooked game falls back to the default material when a flag is missing. Niagara materials need
  Niagara Sprites (and Ribbons); M_Paint needs Geometry Collections; every prop material needs Instanced Static Meshes.
  Check a Development build's log for "missing usage flag" after adding materials.
- Test flags (packaged build, launched the way WispOS does): `-ftbench -ftlevel=0..2 -ftquality=0..3 -ftseconds=60`
  (the autopilot drives, then `bench/bench_L<level>_<PRESET>.json` is written to the app's data folder and the game quits),
  `-ftauto` (autopilot), `-ftshots=N` (a screenshot every N s), `-ftquit=N`. `Tools/bench.sh` runs a set of benches.
- Shipping builds open only from WispOS (launch ticket); a development build or the editor opens anywhere.
