# FULL THROTTLE 2.5: the Unreal Engine 5.9 game

The 80s spy car chase (web app `full-throttle`, 1.7.2) rebuilt as a native Unreal Engine 5.9 game for Windows,
C++ plus Blueprints. WispOS opens the packaged build as the native app `full-throttle-unreal`
(`../full-throttle-unreal-app`). The web app and its art pipeline (`../full-throttle-art`) are untouched.

## Folders (WispOS Work together sections)

| Section | Folder | What is in it |
|---|---|---|
| Maps | `Content/Maps/Bayshore`, `ChemicalWorks`, `Nevada`, `Volcano`, `ThemePark`, `Glacier`, `Abyss`, `Metro` | one folder per level: the map, its props (Nanite meshes) and its sky/material instances |
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
- **Cel look (2.2, the 1.0 style):** `ft_cel.py`. One post-process material, `Core/Materials/M_PP_Cel`, at "scene colour
  before DOF" in each level's `Grade80s` volume (instance `Maps/<Level>/Materials/MI_PP_Cel_<Level>`): it reads light as
  scene colour / GBuffer base colour and cuts it into the 1.0 toon bands (lit, mid 0.78, shadow 0.58, cool shadow tint),
  keeping Lumen's shadows, headlights and explosion light; unlit pixels and emissive glow pass through. The same pass
  draws ink (#1b1033, like 1.0's bake.py) where 1/depth bends or the normal turns sharply. Bayshore is the 1.0 pastel
  beach town by day (pastel sky, sun from behind-left like 1.0's M_Toon, pastel sand, plaza and sea, the 1.0 grey-violet
  road, aqua walls); since 2.4 the chemical plant is by moonlight and Nevada by day (see the 2.4 notes below).
  Explosions and burning wrecks (`NS_Explosion`, `NS_Fire`) use `FX/M_Fire`, the 1.4 cel temperature fire (white, yellow,
  orange, red, dark red, soot bands; masked, so it gets ink too). Rebuild: `import ft_cel; ft_cel.build_materials();
  ft_cel.apply_level(0); ft_cel.apply_level(1); ft_cel.apply_level(2)`. Tune a level live in PIE with
  `ft_cel.tune(i, Key=..., Debug=1)` (Debug paints light/Key as false colour), then `ft_cel.cel_mi(i)` after PIE to save.
  Check it with `ft_cel.shot(name)` (a HighResShot of the game viewport): the `ft.Shots` hook films through a
  SceneCapture, which skips post-process materials, so it never shows the cel pass.

- **Skill points and the garage (2.3):** `Content/Data/skills.json` (staged as a UFS file, `DirectoriesToAlwaysStageAsUFS=Data`)
  holds the points a level pays, the stats and the upgrades (ranks, costs, effects, `branch`/`tier`/`requires` for the coming
  skill tree). `FTSkills.cpp` reads it; `UFTGameInstance` keeps the bank and ranks in settings.json (`skills`) and answers
  `Stat(name)` (kind `mul` = max(0.2, 1 + sum), `add` = sum). The game applies `steer` (controller), `driftCharge`, `turboBurn`,
  `turboRegen`, `damageTaken`, `gunDamage` and `missiles`. The dailies pay the points; Enter there opens `EFTPhase::Garage`
  (`AFTGameMode::TickGarage`, `AFTHUD::DrawGarage`), and only its START LEVEL button (pad Start, Enter/A on it, a click) loads
  the next level. The web app's `app/skills.js` is the same data: change both together.
- **Highlight reel (2.3):** it loops until the dedicated CONTINUE button: Enter, pad Start, or a left click inside the button
  (`ReelContinueRect`, drawn by the HUD). Any other key or click does nothing. The cursor shows on the reel and the garage
  (`AFTPlayerController::SetMenuMouse`).
- **Test hooks (2.3):** `-ftwrap=N` wraps the level after N s of racing; console `ft.Press <Key>` and `ft.Click <x> <y> [Right]`
  feed keys, pad buttons and clicks through the player's input; `Content/Python/ft_skilltest.py` wraps them for PIE.
  Logs: `FTFLOW` (reel, garage, buy/refund) and `FTSKILLS` (points, the stats at each race start).

- **2.4 (owner's queue): glued to the road, cel fire, day/moonlight skies.**
  - *Road glue* (`AFTPlayerCar::ApplyArcade`, `ft.Arcade` bit 64; tuning `Downforce` 0.8 g, `GlueK` 150, `GlueDamp` 24.5):
    off on real jumps only (a jump section's kicker and gap, its landing while airborne, and 1.6 s after an upward
    `Shove` such as the car-carrier ramp). Lift = chassis height over the road minus ride height (~85 cm); a critically
    damped spring on it pulls the car back (never pushes up, never adds speed into a landing), the constant downforce
    only acts at ride height, and a car on its belly is lifted back onto its wheels. In pipes/loops/twists the
    surface turns away from the car, so there the spring pulls in full (to 20 g) and the car is also held across
    (a spring from 9 m out, sideways speed damped). Lessons: a flat pull keyed to the wheels' "in air" flag pinned the
    car on its belly (traces start under the road once the springs are past their 14 cm); a pull with no lateral
    hold let the car slide off the pipe ribbon sideways.
  - *Car fire*: the KESTREL burns with `FX/NS_CarFire` + `FX/MI_Fire_Car` (M_Fire, Gain 0.85 so no band blooms,
    `Rim` 0.07 inks the flame edge), low flames on the engine deck; `NS_Smoke` is smaller and thinner. Explosions and
    wrecks still use `NS_Fire`/`NS_Explosion`. Build: `ft_cel.car_fire(); ft_cel.car_smoke()` (car_smoke edits in place:
    NS_Smoke is referenced, so delete-and-duplicate fails).
  - *Skies* (`ft_mats.SKY_HLSL`, `ft_cel.LEVELS`): the sky draws the 1.x canvas disc (render.js drawSky/drawNight):
    pale upper half, deeper lower half, four bands cut out of the lower half, halo, glow, slow rays (`DiscCol`,
    `DiscCol2`, `HaloCol`, `Halo`, `Glow`, `Rays`). Bayshore day with the sun at azimuth 100 (the track runs mostly
    +Y; 2.2 had it behind the player), Chemical Works by moonlight (banded cyan moon at 130, 5.5 lux cool light),
    Nevada by day (sun ahead-left of the +X highway). The sky sphere is 60 km (it was 2 km while tracks run 12-23 km
    out) and the height fog has `FogCutoffDistance` 50 km so the painted sky keeps its colours.
  - *Test hooks*: `-fthp=N` / `ft.Hp N` race with that armour (30 = burning; bench mode holds it); autopilot logs
    `FTJUMP` (every flight: kind road/gap/ramp, air time, rise, distance, lift range) and `FTGLUE` (5 s summaries).
    `../full-throttle-tests/v240/run_dev.sh <tag> <level> <arcade> <hp> <seconds>` runs the Development package.

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

- **2.5 (owner's queue): the car sticks to the road, keeps its speed through set pieces, and five new levels.**
  - *Stick* (`AFTPlayerCar::ApplyArcade`): on everything but a real jump, any speed away from the road is taken out as
    soon as the car lifts 4 cm over ride height, and a car more than `StickMaxLift` (40 cm) up with every wheel off is put
    back down (at most 35 cm a frame, so a bad height reading can't push it through the deck). In pipes, loops and
    corkscrews it can't leave the ribbon sideways (put back 1.5 m inside the barrier). A car wedged under 600 cm/s with
    the throttle open for 1.2 s is put back on the road just ahead, rolling (`FTSTALL`). Lesson: the first 2.5 build
    turned the upward speed along the road keeping its size; a bounce off a pipe barrier came back as 14,000 cm/s
    sideways and threw the car out of the Chem double-roll pipe. Only remove it.
  - *Speed through set pieces*: entering a loop, corkscrew, pipe or coaster the way-in speed (at least cruising) becomes a
    floor held all the way through (a turbo inside raises it); the governor never brakes in there and keeps pushing
    off the wheels. In 2.4 the Chem corkscrew took the car from 4,932 to 292 cm/s; now 98-100 % is kept (`FTSET` logs
    entry/min/exit per set piece). Jumps: off the kicker at 6,800 cm/s or more (2.4 fell short of the gaps at ~5,000).
  - *New track shapes* (`FTData.cpp` section strings): `half` (snowboard half pipe: flat floor, quarter circles up to 68
    degrees at the barrier; `AFTTrack::SurfaceH` gives height and slope across, the car glues to its normal), `coaster:mode:hills`
    (two rails instead of a road; mode 0 side by side, hop across by steering hard; 1 split high/low and wide; 2 double
    helix; 3 cross-over; `AFTTrack::RailsAt`; the car follows its rail by continuity, `RailHop`), `bldg:height` (the
    road runs through a tower's ground floor: facade `MI_Facade`, lobby, ceiling lights). Level fields: `Slope` (the ski
    level runs downhill), `bLava` + `LavaBands` (lava lakes under every jump gap, lava rivers; `MI_Lava`), `bLightning`.
  - *Levels* 4-8: VOLCANO (Mount Kaldera, boss ENGINE 13: flame lanes that sweep, fireballs, burning barrels), THEME PARK
    (Coaster Cove, tighter and tighter turns, then the coaster; boss MR. CHUCKLES: balloon bombs onto warned marks, a
    carpet of bombs with one free lane, pie fans), ICE MOUNTAIN (Frostbite Peak, downhill all the way, half pipes; boss
    THE ABOMINABLE: snowballs, ice-spike lanes, avalanches with one gap), UNDER THE SEA (to the sunken city of Nereida,
    glass tunnels; boss THE GREAT WHITE: swordfish fans, flying fish, the jaws lunge), CITY CENTER (Midtown, hairpins,
    through the towers; boss THE CHAIRMAN, a hovering limo gunship: walls of shots with one gap, a trailing stream,
    aimed bursts, rocket rings; every pattern has a way through). Art and maps: `Content/Python/ft_v25.py`
    (`materials(); export(); build_maps()`). New hazard types: 4 flame lane, 5 lobbed thing (bomb, snowball, fish,
    fireball) flying in from the boss onto a warned ring.
  - *Title*: eight level cards, arrows or 1-8. Best takes saved for eight levels.
  - *Tests*: `../full-throttle-tests/v250/run_dev.sh <tag> <level 0-7> <seconds>` (Development package, autopilot; FTSET,
    FTGLUE, FTJUMP, FTRAIL, FTSTALL lines and shots in out/<tag>); `all.sh` runs all eight.
