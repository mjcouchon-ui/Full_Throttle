# FULL THROTTLE 3.3.0: the Unreal Engine 5.9 game

The 80s spy car chase (web app `full-throttle`, 1.7.2) rebuilt as a native Unreal Engine 5.9 game for Windows,
C++ plus Blueprints. WispOS opens the packaged build as the native app `full-throttle-unreal`
(`../full-throttle-unreal-app`). The web app and its art pipeline (`../full-throttle-art`) are untouched.

## Folders (WispOS Work together sections)

The sections are the game's parts, not its folders (2026-11-14). Leads are from design doc v1.2; anyone may take any section.
Paths are under `full-throttle-unreal/`; a file in two sections' folders belongs to the one naming it most closely.

| Section | Part | Lead | What is in it |
|---|---|---|---|
| Bayshore (level 1) | Levels | Matt | `Content/Maps/Bayshore`, `Content/Python/ft_city.py` |
| Chemical Works (level 2) | Levels | Matt | `Content/Maps/ChemicalWorks`, `Content/Python/ft_chem.py` |
| Nevada (level 3) | Levels | Matt | `Content/Maps/Nevada`, `Content/Python/ft_desert.py` |
| Volcano (level 4) | Levels | Matt | `Content/Maps/Volcano` (built by `Content/Python/ft_v25.py`) |
| Theme Park (level 5) | Levels | Matt | `Content/Maps/ThemePark` (built by `Content/Python/ft_v25.py`) |
| Ice Mountain: Glacier (level 6) | Levels | Matt | `Content/Maps/Glacier` (built by `Content/Python/ft_v25.py`) |
| Under the Sea: Abyss (level 7) | Levels | Matt | `Content/Maps/Abyss` (built by `Content/Python/ft_v25.py`) |
| City Center: Metro (level 8) | Levels | Matt | `Content/Maps/Metro` (built by `Content/Python/ft_v25.py`) |
| Track & road builder | Levels | Matt | `FTTrack.*`, `FTData.cpp`, `Content/Python/ft_level.py` |
| Driving: the KESTREL and how it handles | Vehicles & driving | Matt | `Content/Characters/Kestrel`, `FTPlayerCar.*`, `FTPlayerController.*`, `Config/DefaultInput.ini`, `ft_car.py` |
| Enemies, bosses & traffic | Characters | Matt | `Content/Characters/{Syndicate,Choppers,SnakeEyes,Bosses,Traffic,Projectiles,Themed,Wheels}`, `ft_fleet.py`, `ft_heli.py`, `ft_v28.py` (2.8), `ft_v33.py` (3.3) |
| Environment: materials, sky & the cel look | Environment | Justin | `Content/Core/Materials`, `Content/Core/Palette`, `ft_mats.py`, `ft_cel.py`, `ft_art.py`, `ft_buildings.py` (Level 1 glass and neon) |
| Effects: fire, smoke & explosions | Effects | Justin | `Content/FX`, `ft_fx.py`, `ft_v26.py` (2.6) |
| UI & HUD | UI | Brad | `Content/UI`, `FTHUD.*` |
| Skills, garage & money | Gameplay | Brad | `Content/Data`, `FTSkills.*`, `FTGameInstance.*`, `ft_skilltest.py`, `FTProgress.*` (2.6) |
| Game rules & director | Gameplay | Keenan | `Content/Core/BP_FTGameMode`, `Content/Characters/Pickups`, `FTGameMode.*`, `FTEntity.*`, `FTCore.h`, `FTFlow.cpp` (2.6) |
| Audio | Audio | Keenan | `Content/Audio`, `Tools/sfx_synth.py` |
| The nights (beat-em-up between the races) | Nights | | `Source/FullThrottle/Brawl`, `Content/Brawl`, `FTNightFlow.*`, `Content/Python/brawl_*.py` |
| Code base & build files | Code | | the rest of `Source`, `FullThrottle.uproject` |
| Tools & tests | Tools | | the rest of `Content/Python`, `Tools` |
| Project settings | Settings | | `Config` (but `DefaultInput.ini`) |

Levels 4-8 (2.5) share one art and map script, `Content/Python/ft_v25.py` (their props, the five new bosses, the boss
projectiles, `M_Lava`/`MI_Lava`/`MI_Facade`), so it stays in Tools & tests rather than in one level's section.
The five new bosses' meshes sit in `Content/Characters/Bosses` (Enemies) and `M_Lava`, `MI_Lava`, `MI_Facade` in `Content/Core/Materials` (Environment).

The cutscenes are the nights': every night opens on THAT NIGHT and ends on LAST CALL + END OF NIGHT (night 1 also the pier), played in the brawl itself (`Brawl/FTBrawlCutscene.cpp`); they belong to the nights section.

No file in the project is over 50 MB. `Binaries`, `Intermediate`, `Saved` and `DerivedDataCache` are made on each computer.

## How it is made

- **Track:** `AFTTrack` integrates the 1.x section lists (`FTData.cpp`) into frames and builds the road, walls, neon,
  glass pipes and ground as procedural mesh chunks, and the roadside props as hierarchical instanced meshes (low-poly,
  not Nanite). Since 2.7 nothing is placed where it would stand in any part of the road (see the 2.7 notes).
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

- **Level 1 sky (BayshoreSky plugin):** `Plugins/BayshoreSky` replaces Bayshore's NightSky, Sun and SkyLight with a semi-cel daylight
  sky (baked horizon and clouds, sun, sea, hills, blimp, gulls). It is a Level Instance (`/BayshoreSky/Maps/LI_BayshoreSky`) added by
  `import ft_sky; ft_sky.apply(0)`; run it after `ft_level.build(0)` / `ft_cel.apply_level(0)`. It keeps the old actors in the map,
  switched off, and `ft_sky.remove(0)` brings them back. Tuning, bakes and costs: `Plugins/BayshoreSky/INTEGRATION.md` and `SourceArt/BayshoreSky/BAKE.md`.

- **Level 1 buildings (`BUILDINGS.md`):** Bayshore's hotels and skyline have real glass and neon behind the cel look.
  `Content/Python/ft_buildings.py` builds two emissive-only materials in `Maps/Bayshore/Materials`: `M_BayGlass` (a lit room
  behind every pane by interior mapping, no textures; the BayshoreSky gradient reflected in the glass; rooms switching on
  through the evening) and `M_BayNeon` (HDR tubes that hum and now and then dip). Their GBuffer base colour is black, so
  `M_PP_Cel` does not band them; the shared post-process is unchanged. `ft_city.py` dresses every side of the four hotels
  (`hotel()`, and `liner()` for the Streamline one) and lights the skyline pane by pane. Found on the way: no mesh of the
  game was ever Nanite (`ft_export.save_mesh` asked for it and never got it). Measured on Level 1, getting it made the
  frame slower, so `save_mesh` now defaults to `nanite=False` and says why (see `BUILDINGS.md`). Rebuild:
  `import ft_buildings, ft_export; ft_buildings.build_materials(); ft_export.run(['bayshore'])`. Placement
  (`FTTrack.cpp`) is unchanged.

- **Bayshore billboards:** the 13 printed adverts of `SourceArt/BayshoreBillboards/Set01|Set02`. `import ft_billboards;
  ft_billboards.build()` imports the PNGs (BC7, sRGB, World, streamed; max 1024 for Set01, cut from a nine-panel sheet, 2048 for
  Set02), builds `Maps/Bayshore/Billboards/Materials/M_BB_Printed` (opaque Default Lit, ISM + Nanite usage) with one `MI_BB_<Stem>`
  each, and `Meshes/SM_BB_<Stem>`: `ft_city.ad_billboard()` (the sunset sign's posts and backing, a cream frame lip, a catwalk in
  the brand colour, three lamps) plus the 1200 x 600 cm face in its own `Face` slot. `AFTTrack::PlanAds` (`DefaultAds`) places
  them on every rebuild, deterministically: a lawyer on each jump's run-up (where a clear slot exists), Rear-End Rick on a calm
  run-out after every other landing, a casino after, Captain Liability's closing alternate sequences; Big Dick's / Last Dime as
  1.4x landmarks at every third Ferris wheel / lighthouse; then each district's designs in rotation (pier: food; marina: the
  casinos, the motel and Dick Settle; the hotel strip: the other lawyers, casinos, the motel, Burn Notice and Big Dick's), never
  in a set piece (loops, gaps, pipes, coasters, tower blocks), its warning paint or the old sunset sign's slots, clear of every
  road section. Every ground prop (in practice palms, umbrellas and lifeguard towers) keeps out of a sign's footprint.
  `ft.Ads 0` / `-ftads=0` builds the track without them (before/after benches); the log line
  `FTADS level 0: N billboards ... layout <hash>` shows the layout is the same each build (Level 1: 58 billboards, 13 designs).
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

## The nights between the races (merged 15 Nov 2026 from `../beat-em-up-unreal` 0.8.0 + night 3)

The story flow (`~/Wisper/docs/full-throttle-flow/full-throttle-flow.html`) puts a night between the drives:
L1 BAYSHORE -> N1 THE RUSTY ANCHOR -> L2 CHEMICAL WORKS -> N2 THE VELVET ROOM -> L3 NEVADA -> N3 THE LUCKY 7 -> ...
The night is the side-scrolling 2.5D brawl, merged as-is: code `Source/FullThrottle/Brawl/` (classes `FTBrawl*`, cvars `ftb.*`,
log `FTBRAWL`), content `Content/Brawl/` (maps `Maps/L_RustyAnchor`, `Maps/L_VelvetRoom`, `Maps/L_Lucky7`; their World Settings name
`/Script/FullThrottleBrawl.FTBrawlGameMode`, which `[CoreRedirects]` in DefaultEngine.ini points at this module).
The brawl project `../beat-em-up-unreal` stays the place to build nights; copy `Brawl/` and `Content/Brawl/` over again as
they are (since 15 Nov, run 32365, the files are identical in both projects: `FTBrawlGameMode.cpp` compiles the career hand-off
in when `FTNightFlow.h` is there, `#if __has_include`). New night maps also go in `DefaultGame.ini` `MapsToCook`.

- **Hand-off (`FTNightFlow.h/.cpp`, a game-instance subsystem).** The races are untouched. After a wrapped level, ENTER on
  the PAYCHECK calls `UFTNightFlow::GoToNight` (one line in `TickPaycheck`): it saves the career's pending level (as
  PRE-PRODUCTION would) and opens the night that follows. When the night is over (night 1: its END OF NIGHT card + ENTER;
  every night since run 32365) `NightOver` reopens the finished level's map on PRE-PRODUCTION for the next level, the screen the
  paycheck led to before, and its READY loads the next race exactly as before. Got away / out of film / reshoot / title
  never go to a night. Quitting during a night reopens PRE-PRODUCTION for the next level (the night is skipped).
- **Which night:** `NightMaps` in FTNightFlow.cpp (after L1 `L_RustyAnchor`, L2 `L_VelvetRoom`, L3 `L_Lucky7`, then
  N4-N7 by their story names). A night whose map is not in the project is skipped, so a new night only needs its map
  and brawl code merged. All seven are in (run 32365): N1 THE RUSTY ANCHOR, N2 THE VELVET ROOM, N3 THE LUCKY 7, N4 THE TIKI
  TORCH (after L4 VOLCANO), N5 STRIKE CITY (L5), N6 THE FROSTBITE LODGE (L6), N7 THE SEA BISCUIT (L7), each with its
  THAT NIGHT and LAST CALL cutscenes (see `../beat-em-up-unreal/README.md`). L8 has no night. `L_Day1Set` (day 1's BEFORE THE
  DRIVE scene) came along with the brawl content but is not in the flow: the owner's rule was nights only, between the races.
- **Grab and throw (every night, run 42137):** L / C / pad B lifts the foe in front over Nova's head, L / J / K hurls him the way
  you steer; the thrown body knocks down every foe it hits. Same code as `../beat-em-up-unreal` (its README, "Grab and throw").
- **Tests:** `-ftnonights` turns the nights off (benches never go to them). `-brawlgrab` makes the autopilot grab and throw,
  `-brawlkeytest` plays the autopilot through key presses (the keyboard path). In a night, `-brawlauto` fights by itself and
  `-brawlnext` presses ENTER after the win / on the last card, `-brawlshots=<dir>` writes a frame every 4 s. Logs:
  `FT NIGHT start after=L1 map=... next=L2`, `FT NIGHT over next=L2 preproduction on=...`.

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

- **2.5.1 (owner's queue): level one's cel-shaded look on every level.** (Environment section: `ft_cel.py`, plus each
  level's `MI_PP_Cel_*`, `MI_Sky_*`, `MI_Road_*` and map.)
  - `ft_cel.apply_level` gave Bayshore's grade only to day levels; the night levels kept the 2.0 grade (bloom 1.15 from
    threshold 1.0, lens flare 0.6, colour tint) and 0.9-opacity volumetric fog, which glowed the cel bands and the ink away.
  - Now every level gets the same grade, night levels get Bayshore's fog (`ft_v25.night_fog`, no volumetric), and
    `NIGHT_CEL` is Key 1.1 / Bright 1.3 / EmisCut 6.
  - The sun or moon lights the car from behind along each track's main heading, with the painted disc ahead (Volcano,
    Glacier and Metro lights and the Theme Park / Metro moons were the wrong way round).
  - Check the look with `-ftshots=N` in a packaged build (real viewport, post-process included):
    `../full-throttle-tests/v251/run_ship.sh`.

- **2.6.0 (owner's queue): the progression of design v6** (spec pack `full-throttle-spec`, 14 Nov 2026, files 00-12).
  - *Data*: `Content/Data/skills.json` is now format `full-throttle.skills/2`, written by `Tools/spec_to_skills.py` from the
    spec's Markdown tables (it checks the spec's totals: 30 levels to 448,920 XP, 35 SP; 30 nodes, 75 SP; garage $1,770,000;
    108 cosmetics, 104 for sale, $6,378,000; 114 / 51 / 66 Syndicate; par 300 / 185 / 220). It replaces format 1 (2.3's
    four point-bought upgrades); `FTSkills.*` is gone, `FTProgress.*` reads it. Run:
    `python -I Tools/spec_to_skills.py <spec dir> Content/Data/skills.json`.
  - *Career* (`FTProgress.*`, `UFTCareerSubsystem`): the spec 09 JSON as `career.json` beside `settings.json` (test slot
    `career_pie.json`, `-ftslot=pie`); XP and driver levels, the skill tree with pending nodes, greenlights and respecs, the
    garage, the asset library, extras, feats, Director's Notes, stats, the back-catalogue bonus (best takes / 30) and the
    load integrity rules. The paycheck is one function, `FTPay::Compute`, with the automation test `FT.Paycheck.Examples`
    (03 §7: $40,450 / $46,830 / $93,950).
  - *Race* (`FTGameMode.cpp`, `FTFlow.cpp`): XP beside every award, Carnage tags (script spawns; boss adds at half pay and
    XP), the scene checkpoint for all level counters, car stats from the garage and skills at ACTION!, the 05 speed model
    (the governor now reaches its target at 420 / 900 su/s), the Syndicate's road paces with the slide and LEFT BEHIND, the
    boss escape clock (rate), GAP bar, LAST CHANCE and RUN DOWN, AP / HE ammo, all 30 skill-node behaviours, 3 retakes.
  - *Screens* (`FTHUD.cpp`): the reel holds on its last shot until CONTINUE and skips only on a 1 s hold; THE DAILIES then
    the PAYCHECK; PRE-PRODUCTION (SKILLS, GARAGE, CUSTOM, SFX DEPT, EXTRAS; G on the title); the AMMO line and pips; the
    900 su speed arc; REDUCED MOTION in OPTIONS.
  - *Effects* (`ft_v26.py`): recoloured copies of the turbo flame, tyre smoke, fireball and sparks for the SFX DEPT (the
    stock systems are stateless and expose no colour).
  - *Tests*: `../full-throttle-tests/v260/run.sh <career|preprod|race1|speed|flow|skills|ram>` runs `scripts/<name>.txt`
    in the Development package through `-ftexec=<file>` (the FT* harness of spec 12 §9, "wait", "waitfor <text>|<s>",
    "shot <name>"); `real_input_260.py <exe>` plays the owner's flow with real Windows keyboard and mouse input.


- **2.6.1 (owner's queue): every level has its own music.** (Audio section: `Content/Audio/Music`; the level → track
  table is `FFTLevelDef::Music` in `FTData.cpp`.)
  - Bayshore keeps "Pixel Adventure" (`M_PixelAdventure`) and the title keeps "Bit-Beast Dash" (`M_TitleTheme`); neither
    asset was touched.
  - New looping SoundWaves, imported from the owner's mp3s (48 kHz stereo, compression 80 like Pixel Adventure):
    Chemical Works `M_FactoryCombatLoop` ("Factory Combat Loop"), Nevada `M_DesertFrontier` ("Desert Frontier"), Volcano
    `M_InfernoLevel` ("Inferno Level"), Theme Park `M_CarnivalChaos` ("Carnival Chaos"), Glacier `M_BlizzardRun`
    ("Blizzard Run"), Abyss `M_ToTheCityInTheSea` ("To the city in the sea"), Metro `M_UrbanChase` ("Urban Chase").
  - `AFTGameMode` loads `Def().Music` where it used to hard-code Pixel Adventure (falls back to it if a track is missing)
    and logs `FTMUSIC level=N track=...` at ACTION!. Same volume (0.34) and ducking as before.
  - *Tests*: `../full-throttle-tests/v261/run.sh music` (all eight levels via `FTStartLevel`) and `run.sh pick1` ... `pick8`
    (picks the level on the title with the Right arrow and Enter); `FTDumpLevel` now also prints
    `FTMUSIC dump level=Ln track=... playing=1 looping=1`.

- **2.7.0 (owner's queue): the performance pass, and the scenery taken out of the road.** (Track and Settings sections:
  `Source/FullThrottle/FTTrack.*`, `FTGameInstance.cpp`, `Config/DefaultScalability.ini`, `Config/DefaultEngine.ini`.)
  - *Scenery off the road* (`AFTTrack`): the road's frames now sit on a 2D grid (`BuildRoadGrid`, `RoadSpan`,
    `RoadHitBox`, `GroundHitsRoad`), so every placed thing is tested against the *whole* road, not just the frame it was
    placed beside. Where a circuit laps back past itself or bends tightly, roadside props and skylines used to land on
    another stretch of road. Now: a prop that only touches its own barrier is pushed out; one that lands on any other
    part of the road is left out; a deck pillar standing in a road below is left out; each level's distant destination
    (Vegas, the volcano, the park skyline, the peak, the sunken city, the Metro spire) is tried further out and a little
    to either side, scaled with the distance so it looks the same size; the land beside the road (and Bayshore's sea)
    stops short of any other part of the road instead of running 300 m straight across it. Each level logs
    `FTCLEAR level=N land: ... props kept= ... left out of the road= ... pillars left out=`.
    `ft.ClearScenery 0` (then reload the level) places everything the 2.6 way, for before/after checks.
  - Independent check (`../full-throttle-tests/v270/check_road.py`, run in the editor: reads the built road mesh's deck
    vertices and counts placed boxes that contain any): 2.6 had 94 scenery pieces standing in the road (Metro 53 skyline
    blocks and the spire, Chemical Works 29 tanks, vats, pipework, stacks, skylines and pillars, Abyss 4 fish schools and
    2 sunken cities, and the destination backdrop in Bayshore, Nevada, Volcano and the Theme Park); 2.7 has 0 on all eight
    levels. Chemical Works also cut 1,314 strips of land that crossed the road where the circuit runs under itself.
  - *Performance*: Lumen GI and reflections are on at EPIC only (`DefaultScalability.ini`); below Epic the cel pass looks
    the same with the sky light (checked side by side, `v270/lumen`). Dynamic resolution (TSR) is on: when the GPU can't
    hold 60 fps it renders fewer pixels, between the preset's own resolution and half of it (`ApplyQuality`,
    `r.DynamicRes.*`; `-ftnodynres` turns it off). Props stop drawing beyond about 120× their size (small things 600 m,
    houses ~2 km; skylines and backdrops always draw). Bench on this PC (RTX 5060 Ti, 2048×1152, autopilot 30 s,
    `v270/bench.sh`): HIGH 200 → 261 fps (GPU 4.3 → 3.5 ms), MEDIUM 249 → 312 fps, EPIC and LOW unchanged (~136 / ~420).
    Forcing a 4.5 ms budget dropped the resolution to 51 % and the GPU to 4.1 ms (from 6.8): that is what a slower card
    gets instead of a lower frame rate.

- **2.8.0 (owner's queue): the second performance pass, a new yeti, and every level's Syndicate in its own look.**
  (Settings, Enemies and Game rules sections: `Config/DefaultScalability.ini`, `FTGameInstance.cpp`, `FTEntity.*`,
  `FTGameMode.cpp`, `FTFlow.cpp`, `Content/Python/ft_v28.py`, `Content/Characters/{Bosses,Themed}`.)
  - *Where the frame went* (`profilegpu` in a Development build, `../full-throttle-tests/v280/prof.sh` + `gpusum.py`):
    at EPIC, Lumen's screen-probe gather 2.5-5.1 ms and Lumen reflections up to 2.4 ms (Metro's glass); TSR a third of
    the frame at HIGH and MEDIUM (1.2 / 0.8 ms) and a quarter at LOW; two SSAO passes at HIGH; and every moving foe,
    bullet, wreck and piece of debris forcing a global distance field update and Lumen card captures each frame.
    Draw calls were never the problem (Nanite: ~860 a frame), and every model is Nanite, so it LODs itself.
  - *Changes*: EPIC runs Lumen at the HIGH preset's gather settings (screen probes 1/32, half-res reflections, no Lumen
    translucency volume, slower scene updates) and TSR history at 100 % (`DefaultScalability.ini`); HIGH keeps TSR
    without flicker detection and one SSAO pass; MEDIUM and LOW use TAA instead of TSR (`ApplyQuality`, cvar
    `r.AntiAliasingMethod`; `-fttsr` keeps TSR for comparisons) and no SSAO; LOW no motion blur. Foes, bullets, pickups,
    hazards, wrecks and Chaos debris are out of the distance fields and the Lumen scene (`bAffectDistanceFieldLighting`,
    `bAffectDynamicIndirectLighting`); the cel pass bands their light the same either way.
  - *Bench* (Shipping, this PC's RTX 5060 Ti, 1920×1080 window, autopilot 30 s, `../full-throttle-tests/v270/bench.sh`,
    2.7.0 → 2.8.0, GPU ms per frame): EPIC Bayshore 6.40 → 4.68, Metro 7.92 → 5.56, Abyss 6.36 → 4.66 (−27 to −30 %);
    HIGH 3.0-3.2 → 2.7-2.8 (−10 %); MEDIUM 2.1-2.2 → 1.6-1.7 (−23 to −26 %); LOW 0.85-0.95 → 0.67-0.76 (−20 %).
    Frame rate EPIC 147 → 199 (Bayshore), 122 → 171 (Metro); MEDIUM 435 → 527. A slower card gains the same share.
    Look checked side by side (EPIC Metro, MEDIUM Bayshore): same bands, ink and glow (`../full-throttle-tests/v280/look_*`).
  - *THE ABOMINABLE* (`ft_v28.yeti`, `yeti_arms`): a new original yeti in the spirit of the old ski-game snowman: huge,
    white and shaggy (fur tufts all over), a small head sunk in his shoulders, a grey-blue face with angry brows and a
    roaring mouth, both arms thrown up over his head with black claws. The arms are their own mesh
    (`SM_YetiArms` on the boss's Extra component, pivot `AFTEntityActor::YetiShoulderZ`) and flail as he lopes. He now
    *chases*: he arrives 26 m behind the car, bounds past in the next lane over 4.5 s (`FT BOSS yeti=alongside`), turns
    round to face you (`yeti=turned`), and then keeps charging back up the run at the car with his arms up
    (`yeti=charge`, every 9 / 7 / 5.5 s by phase; a hit is the boss wall). Snowballs, ice spikes and avalanches as before.
    `FTBossNow` keeps his real entrance.
  - *Themed Syndicate* (`ft_v28.THEMES`, `Content/Characters/Themed/L<n>/SM_{Moto,Rammer,Suv,Truck,Heli,Henchman}`,
    picked in `AFTEntityActor::MeshFor`, the classic model if one is missing): Bayshore keeps the classic purple
    Syndicate; Chemical Works HAZMAT CREW (hazard yellow, gas masks, glowing toxic drums, a sprayer turret); Nevada DUST
    DEVILS (rust and sand, dusters, goggles and bandanas, bull horns, a roll cage, a cowboy hat); Volcano MAGMA CORPS
    (charcoal with glowing lava cracks, silver fire suits, gold visors, heat shields); Theme Park CARNIVAL CREW (jesters,
    a bumper car, a popcorn wagon, a circus wagon, a chopper with balloons, clowns); Glacier ICE PATROL (snowmobiles on a
    ski and a track, parkas with fur ruffs, a plough, a snowcat); Abyss DEEP DIVERS (brass diving helmets, yellow
    mini-subs with portholes, propellers, a periscope); Metro THE SUITS (gloss black and gold, mirrored visors, a cash
    truck with a vault door). Same footprints as the classic models, so hit boxes are unchanged.
  - *Tests* (Development package): `../full-throttle-tests/v280/run.sh yeti` (the real finale spawn on the Glacier,
    shots of the chase, the turn and a charge) and `run.sh crews` (every level's crew on its own road, 24 shots);
    0 errors, 0 missing material usage flags.

- **2.9.0 (owner's queue): the Mac performance audit (Apple M1 Max, 14 Nov 2026) taken in.** (Settings section:
  `FTGameInstance.*`, `FTHUD.cpp`, `FTGameMode.*`, `FTFlow.cpp`, `Config/DefaultEngine.ini`, `Config/Mac/MacEngine.ini`.)
  - *AUTO on an Apple GPU* (`UFTGameInstance::CapAutoForApple`): Unreal's hardware benchmark rates Apple chips far above
    what they deliver (AUTO picked EPIC, 19-27 fps). Now AUTO there caps view distance, anti-aliasing, shadows, post,
    foliage, shading and landscape at HIGH, keeps effects, textures, GI and reflections as measured (so Lumen stays on:
    Metro's lobby loses its bounce light without it), caps the render target at ~1.65 megapixels (67 % at 2560×1440, 84 %
    at 2048×1152, this PC's test window, 89 % at 1920×1080) and lets dynamic resolution drop to 50 % to hold 60 fps.
    LOW/MEDIUM/HIGH/EPIC are unchanged on every GPU, and so is AUTO on Windows. Log line: `FT QUALITY auto=apple ...`.
    `-ftfakeapple` (or console `FTQuality apple` in a Development build) runs the Apple path on any PC.
  - AUTO is AUTO again after picking another preset in the same session (the benchmark result is kept).
  - Dynamic-resolution GPU budget 15.5 ms (was 16.6), so a frame that barely fits doesn't miss a 60 Hz VSync.
  - Mac-only config for a later Mac package: no DBuffer decals on the Mac (`Config/Mac/MacEngine.ini`; the game has no
    decals and on Metal SM5 they force a depth prepass), no Metal ray-tracing shaders (Lumen here is software).
  - Clang fix: `SetupLevel`'s one-pass for-loop (rejected on Mac) is an if.
  - Options screen text no longer says Nanite/VSM run at every preset (false on a Mac's SM5 path).
  - Test hooks: `FTFreeze` (holds one frame) and `FTQuality <-1..4|apple>` for same-frame A/B shots
    (`../full-throttle-tests/v290/ab.sh`, `pngdiff.py`). AUTO-on-Apple vs EPIC on the same frame, all 8 levels: mean
    pixel difference 1.0-1.8 / 255, the same as EPIC against itself a moment later (0.7-1.5): the cel look holds.
  - Bench (Shipping, this PC, 2048×1152 window; the run was stopped after levels 1-4 on the owner's order):
    2.8.0 → 2.9.0 on Windows within ±3.4 % GPU ms at every preset, as intended (`../full-throttle-tests/v290/*.txt`).

- **3.0.0 (owner's queue): the new title screen.** (UI & HUD and Game rules sections: `FTHUD.*`, `FTGameMode.*`,
  `FTFlow.cpp`, `FTGameInstance.*`, `FTTrack.*`, `Content/UI/Fonts`, `Content/UI/Title`, `Tools/title`.)
  - *Stylized 80s title*: the lettered logo `Content/UI/Title/T_TitleLogo` (chrome THROTTLE in Orbitron Black, neon
    "Full" in Mr Dafoe; source `Tools/title/logo.html` + `T_TitleLogo.png`), menu text in Orbitron Black and Rajdhani
    Bold (`Content/UI/Fonts`). All three fonts are SIL Open Font License 1.1 (`Content/Data/Licenses/OFL-*.txt`).
    Buttons ROLL CAMERA, PRE-PRODUCTION, CONTROLS, OPTIONS, QUIT, clickable with the mouse.
  - *Attract mode*: behind the title the game plays itself on autopilot (a VHS-style "DEMO" on-screen display, the car
    sometimes already beaten up) and cuts to the next level every 11 s behind a channel-change wipe. A cut loads the
    next map with its track built light around the shot (`AFTTrack::IsLight`: collision and small scenery only near it),
    and the title song (Bit-Beast Dash) is kept by the game instance so it plays on across the loads. Starting a level
    from the title always does a full build. `ft.Attract 0` or `-ftnoattract` turns it off; benches skip it.
  - Log lines: `FT ATTRACT shot ...`, `FTBUILD level= light= ...`; `FTDumpLevel` also prints the title song and attract state.
  - Checked by the owner in the Development package (`../full-throttle-tests/v300/dev-build`); the Shipping package
    went out without further test runs on the owner's order.

- **3.3.0 (owner's queue): choppers and SWAT drops, separated wheels and body springs, half pipe tricks, the Tesla truck,
  the three-track coaster, the replay camera, performance.** (Enemies, Driving, Track, Game rules and Audio sections:
  `FTEntity.*`, `FTPlayerCar.*`, `FTTrack.*`, `FTGameMode.*`, `FTData.cpp`, `Content/Python/ft_v33.py`, `Content/Data/wheels.json`.)
  - *Models* (`ft_v33.py`, run after `ft_export.fleet()`, `ft_v25.export()` and `ft_v28.export()`, which would put the old
    meshes back): the 80s gunship chopper (open doors with rappel arms, stub wings, chin gun, separate `SM_HeliTailRotor`),
    SWAT troopers (`SM_Henchman`, themed per level; `SM_HenchmanHang` clinging to the car), the level 2 Tesla truck
    (`SM_Tanker`, `SM_TeslaTop`, `SM_TeslaArc`), the clown car's propeller rig (`SM_ClownRig`, `SM_ClownProp`). Every car,
    truck and boss built with `ft_car.wheel` is exported body-only; its wheels are listed in `Content/Data/wheels.json`
    and drawn as `Content/Characters/Wheels/SM_FoeWheel*` with `SM_FoeStrut` struts. Sounds: `S_TeslaCharge`,
    `S_TeslaZap`, `S_PropBuzz`, `S_BombWhistle` (synthesized, `../full-throttle-tests/v330/synth_audio.py`).
  - *Suspension (looks only)*: foes' bodies roll, pitch and bounce on springs over wheels that roll on the road; the
    KESTREL's body leans on its springs too (its wheels now hang off the chassis). Gameplay and physics are unchanged.
  - *Chopper drops*: a trooper slides down the rope from the door to the car's door in 1.35 s; more than ~2.6 m of
    swerve beats him, otherwise he catches the side (3.2 timed out ~9 m short, so most drops missed). `FT HENCH` lines.
  - *Half pipe* (`AFTPlayerCar::TickHalfPipe`): up the wall faster than 12.5 m/s across launches the car off the lip (steer
    spins, drift rolls; within 50 degrees of square or it bails), gently onto the edge grinds the coping until you steer
    down. `FT HALF` lines; the autopilot rides it too.
  - *Coaster*: three rails (`FFTRails` X[3]/H[3], `Neighbour`), modes 4 (vertical loop), 5 (the centre rail corkscrews) and 6
    (double helix); the theme park coaster is 25,600 units (was 19,200), everything after it moved on by 6,400. Choppers fly
    over it (`AFTTrack::AirTransformAt`, `AirClearAt`) and fire rail-aimed rockets; MR. CHUCKLES flies over on propellers
    and bombs your rail (`TickFlyer`, `FT FLYER` lines): hop rails to dodge (hazards carry `Rail`).
  - *Replay camera*: shots use the road's frame (never the car's spin), ease their offset, sweep out of the scenery and
    stay above the deck; the ghost KESTREL has its wheels and sits on the road.
  - *Bugs fixed*: pooled foes kept the last owner's dynamic materials; a henchman's NaN position (an ensure in Dev);
    degenerate track samples guarded. *Performance*: one collision cook per track chunk (level build 5.8 s -> 0.85 s),
    material/light updates only on change, far foes' wheels every third frame.
  - Tests: `../full-throttle-tests/v330/run_dev.sh coaster|halfpipe|tesla|heli|reel` (Development package), bench in
    `../full-throttle-tests/v330/bench.txt`.

- **3.6.0 (owner's queue, 15 Nov 2026): corkscrews out of the ground, the level 2 hits, louder race music, a perf sweep.**
  (Track, Driving and Audio sections: `FTTrack.*`, `FTPlayerCar.*`, `FTGameMode.*`.)
  - *Corkscrews in the ground*: the bed (3.1, the land under the road) was cut only under the deck, so upside down in a
    corkscrew the car hung under the deck inside the land and the chase camera sat 3-9 m inside it. Where the road leans
    past 60 degrees (corkscrews, coaster rolls, fast pipes) the bed is now also cut under the room the car and camera take
    (`BedRoom` 4.5 m over the deck), and a corkscrew's bed looks 14 m either way (`TwistBedReach`). `ft.BedRoom 0` = 3.5.
    Probe: `-ftbedprobe` logs `FTBEDPROBE` (camera and car clearance over the ground per set piece).
  - *Level 2 hits*: `FTBUMP` logs every sudden speed loss (non-Shipping). Found: every gap-jump landing took 35-45% of the
    speed in one hit (Level 2's pond and Mega Vat jumps ~7,200 -> ~4,100 cm/s); a landing now keeps its flying speed and
    eases back to the governor's over ~1 s (`FTLAND`). Off the centreline a corkscrew's rolling deck struck the car
    (Level 2's corkscrews: ~8,200 -> ~2,700 cm/s for a frame; the set-piece floor already gave the speed back): the car now
    rides the deck's sweep in corkscrews. Tried and left out: the corkscrew's centre hold in the fast pipes (halved the
    triple pipe's put-backs but doubled the first double pipe's). Test cvar `ft.AutoX <cm>` makes the autopilot hold one
    line across the road.
  - *Music*: race music volume 0.34 -> 0.5 (+3.4 dB; title song unchanged at 0.55).
  - Tests: `../full-throttle-tests/v350` (`run.sh`/`run2.sh <script> [args]`, `secat.py`, `bumps.py`).

- **Mac performance audit (14 Nov 2026; taken into the game as 2.9.0 above, Mac bench notes).** On an Apple M1 Max the game ran 19-27 fps: Unreal's synth benchmark reads
  Apple GPUs as far faster than they are, so AUTO picked EPIC (TSR with 200% history ~22 ms, Lumen at Epic ~16 ms; the CPU
  is idle at ~3 ms). This Mac runs Metal SM5: no Nanite, no Virtual Shadow Maps (fallback meshes, cascaded shadows).
  `UFTGameInstance::CapAutoForApple` (AUTO + Apple GPU only): every group capped at HIGH but effects and textures, at most
  ~1.65 MP rendered (67% of 2560x1440) and dynamic resolution 50%-cap at a 15.5 ms budget, upscaled by TSR. Now 58-68 fps on
  all eight levels with the same cel look (Lumen stays on: Metro's lobby loses its bounce light without it). Props are
  already HISM instances and occlusion queries are on; hiding every prop saves nothing measurable.
  Measure with `Tools/bench_mac.sh <tag> <level> <quality> <seconds> 2560x1440` (needs `-NoHotReloadFromIDE` builds while
  another editor runs, and FullscreenMode=2 in `Saved/Config/MacEditor/GameUserSettings.ini`). Report:
  `~/Wisper/Log/Reports/2026-11-14-full-throttle-unreal-performance-audit.html`.
