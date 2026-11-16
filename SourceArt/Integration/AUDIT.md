# Architecture audit, 15 November 2026

Three read-only audits of `main` at 8a5920e (FULL THROTTLE 3.3.0 with the Canvas redesign), each tracing the code
end to end, plus an independent line-by-line review of the attack-presentation change. Nothing here was found by
playing; the "how to prove it" lines use the game's own test harness (`-ftexec`, the `FT*` commands in `FTFlow.cpp`).

Status words: **FIXED** (in the commit named), **OPEN** (confirmed in code, not changed: the fix is larger or is
somebody's design decision), **LIKELY** (could not be confirmed without running or without the assets).

## Fixed in this pass

| # | What was wrong | Where | Fix |
|---|---|---|---|
| 1 | **Every boss fired the moment it appeared.** 3.3.0 added a comment in the middle of the line that sets the bosses' first-attack timers; the comment swallowed `E.T1 = 1.6f; E.T2 = 1.0f; E.T3 = 6.f;`, so all three started at 0 | `FTGameMode.cpp`, boss spawn | The three statements are on their own line again |
| 2 | **The level's pay could be lost.** Cash, XP, stats, feats and the best take were banked only when the highlight reel ended. Close the game during THAT'S A WRAP or the highlights and the level paid nothing | `FTGameMode.cpp` Wrap phase, `FinishRating` | `RateAndCommit()` banks and saves once, before the reel; `FinishRating` only opens THE DAILIES |
| 3 | **A damaged career refused every later save, silently.** An empty, cut-off or garbage `career.json` set `bLoadError`; every save was then refused and no screen said so: LV 1, $0 on every launch, for ever | `FTProgress.cpp` `Load` / `Save` | The damaged file is kept as `career.json.damaged`; the last good save (`.bak`) is used if there is one, otherwise a new career starts that can save; the title shows SAVE RESTORED FROM BACKUP or SAVE DAMAGED: NEW CAREER. A newer build's save is still never written over, and the title now says NEWER SAVE: NOT SAVING |
| 4 | **Saves were written in place.** A crash or power cut mid-write left half a file (which then hit 3) | `FTProgress.cpp`, `FTGameInstance.cpp` | Written beside the file and moved over it; the career keeps the file it replaces as `.bak`. An unreadable `settings.json` is copied to `settings.json.unread` before the defaults overwrite it |
| 5 | Hand-edited values were used as they came: negative cash and XP, a pending level of 99 ("LEVEL 99"), huge numbers cast to int64 | `FTProgress.cpp` `Load`, `Fix` | Clamped on load |
| 6 | **Pickups lied at the cap.** Always consumed; at full missiles or armour they granted nothing and still showed "+3 MISSILES" / "+30 ARMOR" with the normal sound and line; partial fills were overstated | `FTGameMode.cpp`, pickup contact | What is granted is unchanged. The popup says what was gained ("+1 MISSILE", "+12 ARMOR") or "MISSILES FULL" / "ARMOR FULL" with the Empty sound; Pit Crew's 10 armour from a crate is announced |
| 7 | A reshoot (R on THE DAILIES) kept the multi-kill and near-miss clocks, so the first wreck scored a DOUBLE KILL; the boss's panel stayed on the HUD from scene 1 | `FTGameMode.cpp` `StartRace` | Reset with the rest of the run |
| 8 | A scene retake kept the combo although the score went back to the checkpoint; popups froze under CUT! and came back; the escape warning never repeated | `StartRace` | Reset on a retake |
| 9 | Clicking RESUME fired a shot (the click acts on button-down; the held button was FIRE on the next frame, spending a bought round) | `FTPlayerController.cpp` | No shot until the button is released |
| 10 | `-ftwrap=N` paid out a full wrap after N seconds in a Shipping build | `FTGameInstance.cpp` | Compiled out of Shipping |

### How these were checked

In the running game (uncooked), by `SourceArt/Jam/unreal/jam_audit.ps1`: five short launches driven through the game's
own harness, with a scratch career. Runs of 15 Nov (`audit1` on the build of commit cbaac52, `audit2` on 0d7fb31:
fifteen checks, all passing):

| Item | Check | Result |
|---|---|---|
| 1 | the boss's first-attack timers as it arrives (`FT BOSS firstAttack`) | `t1=1.6 t2=1.0 t3=6.0` (they were 0, 0, 0) |
| 2 | THAT'S A WRAP: `FT RATING`, `FT PAY total=`, `FT SAVE ... reason=Paycheck result=ok`, and only then `FT SCREEN name=Reel` | in that order; cash 0 before the wrap, 31,990 while the reel played |
| 2 | the game closed during the reel, then started again | cash 31,990 after the relaunch |
| 6 | a crate at 6 of 9 missiles, then a second at 9 of 9 | `gained=3`, then `gained=0`; the frames show "+3 MISSILES", "MISSILES FULL", "ARMOR FULL", "+12 ARMOR" |
| 7 | a reshoot from THE DAILIES (R, READY), then one wreck two seconds in | no multi-kill stunt after it (the take before had a real DOUBLE KILL) |
| 3, 4 | `career_pie.json` cut in half with a good `.bak` beside it | `recovery=backup`, cash 4,242 (the backup's), a save and a load back (4,300), the damaged file kept |
| 3 | the career and its backup both unreadable | `recovery=new_career`, cash 0, a save and a load back (77) |
| 3 | a career marked `"v": 2` | `result=too_new`; the file is byte for byte what it was after a save was asked for |

The title's three messages are in the frames `C1_title_banner.png`, `D1_title_banner.png` and `E1_title_banner.png`
of that run. Not exercised by a run: item 8, item 9 (the click on RESUME) and item 10.

### The per-level check

`SourceArt/Jam/unreal/jam_verify.ps1` drives every level in the real game on the autopilot (wrecking what it meets),
takes both pickups, calls the boss, wraps the level and films the highlight reel, then reads the log: the track's
fingerprint against the baseline, the dressing's placement, errors, and whether the boss arrived, the reel played
and the pay was banked. Run of 15 Nov 04:10 (uncooked, Epic, 3440x1440, `main` d92117c with the land's surface):
all eight levels pass; every track fingerprint is unchanged; no error logged; 88 frames in
`SkyScratch/HandBack/jam/verify/rc1_epic`. Its load and hitch counts are not a measure of the shipped game: the run
was uncooked (see the note on the preload below), each screenshot at that size is itself a long frame, and other
Unreal jobs shared the PC.

### The packaged build

`SourceArt/Jam/unreal/jam_package.ps1 -Name RC_Development` packages a checkout into its own folder under
`K:\Full Throttle\PortBuilds`; `jam_verify.ps1 -Root <that folder>\Windows` runs the per-level check on the packaged
game. Runs of 15 Nov on `main` 223c349 (Development, 1,357 MB, built in 12 to 19 minutes):

- Every level: the boss arrives, the reel plays, the pay is banked, 11 frames, no error in the log.
- 3.4's preload runs (`FT WARM start assets=278`, done in 0.25 s) and **no level loads anything mid-race**
  (the uncooked runs above show 19 to 23 a level only because the preload finds nothing there).
- The track: its checksums differ from the editor binary's on every level, with the same frame and triangle counts.
  The checksums are of raw numbers, and the same code compiled two ways rounds differently while the route is summed
  step by step. The `FTTRACKSUM` log line (added for this: length, box, mean position and mean up vector) shows the
  two tracks **within 0.5 cm of each other over 25 km on every level**. `track_baseline.txt` now holds three tables:
  the editor's checksums, a packaged game's, and the summaries; `jam_verify.ps1` compares a build against its own
  checksum table and the summaries within 2 cm.
- Frames over 40 ms were counted but mean little: each 3440x1440 screenshot is itself one, and other Unreal jobs
  shared the PC.

### The final state of this pass (`main` deaca78, 15 Nov, about 07:00)

With the second dressing pass on all eight levels (`WORLD_PASS_2.md`):

| Check | Result |
|---|---|
| Per-level check, uncooked, Epic, 3440x1440 (`rc3_epic`) | 8 of 8 pass |
| The same at Low (`final_low`) | 8 of 8 pass |
| Packaged Development build, per-level check (`final_cooked`, 1,378 MB) | 8 of 8 pass; nothing loaded mid-race; on seven levels the only frames over 40 ms were the eleven screenshots (City Center: 17) |
| Packaged Shipping build, each level run for 25 s in the game's benchmark mode (`jam_ship.ps1`, `final_shipping`, 778 MB) | 8 of 8: the game exits by itself, writes its report, four frames a level |

A Shipping build runs only with WispOS's launch ticket (`UFTGameInstance::Init`): without it every launch quit in
two seconds, which is what the first Shipping run of the night showed. `jam_ship.ps1` stands in for the launcher (it
writes the ticket into its scratch folder and passes the same value in the environment). So the Shipping game itself
is checked; the launcher handing over a real ticket is not.

The Shipping runs' own reports (Epic, 3440x1440, a screenshot every six seconds, another Unreal editor on the same
GPU): median frame 14.8 to 16.0 ms, 99th percentile 18.6 to 30.5 ms, GPU 11.3 to 12.7 ms, game thread 2.2 to 3.5 ms.
They are what the build did under those conditions, not a benchmark: no run that night had the GPU to itself.

### After the pass: the cut banks (15 Nov, midday)

The dressing's Bank rule and its panels on five levels (`WORLD_PASS_2.md`, "After the pass"; `WORLD.md`, "The cut
banks"), merged with 3.6.0 and the enemy visual overhaul:

| Check | Result |
|---|---|
| Per-level check, uncooked, Epic, 3440x1440, before the merge (`banks_epic`) | 8 of 8 pass; every track fingerprint equal to the baseline |
| The same on the merged tree (`banks_merged_epic`; no other Unreal job on the PC) | 8 of 8 pass; every track fingerprint equal to the baseline: 3.6.0's change to the bed under corkscrews moves neither the route nor anything solid |
| The kits after the merge (`30_bank_build.py`, `BANK_EXPORT=check`) | all five import and draw their panels; the same panel counts as before the merge |
| The real game through the bends, with the panels and without (`play/banks_play`, `banks_play_off`) | 95 frames each; the engine's shadow-queue warning appears once a level in both, at the same moment: it is not the panels' (they cast no shadow) |

Not run for this change: Low, the packaged builds. Frame time: see `WORLD_PASS_2.md`.

**Refitted the same afternoon (the lead-ins).** An independent review of the rule (five readers, two skeptics a finding)
confirmed one defect in play: a panel in a bend's lead-in stood a corner 30 to 60 cm above the land's edge. The fix
(a piece runs with the land's edge, is kept inside the cutting at both ends, and is laid in halves, quarters or
eighths where the cutting deepens fast) was itself simulated over every banked bend of five levels by an independent
port of the code and reviewed three ways before it was compiled: no corner passes the land's edge, no length is left
bare for being too bent, and the simulation's piece count for Ice Mountain (874) is the game's. Checked in the build:
all eight levels loaded (tracks' fingerprints equal the baseline, every other rule's placements identical piece for
piece: the placed totals differ by exactly the bank pieces added), close frames of the two steepest bends' lead-ins
on the five levels, and Chemical Works, Ice Mountain and City Center whole in the real game.
Not checked: Low settings, a packaged build, frame time (about 1,000 more flat, shadowless instances over five levels).

## Open: confirmed in code, not changed

15 Nov 2026 evening: audit pass. Two items under "HUD, menus, input" were found closed in the tree and are marked
FIXED below (the rotor loop, the Options text). The other items in this file are left as written.

Progression and saves
- An out-of-film run marks Director's Notes done and gives their XP, but the pay function returns before the notes'
  line (`FTFlow.cpp` CommitLevel, `FTProgress.cpp` Commit). Skip notes when out of film.
- The wrap sweep scores survivors with the combo and awards multi-kill stunts (free style types and BIG FINISH pay).
- If shots in flight finish the boss during CUT!, the kill is discarded but the level clock stays frozen, which
  inflates SCHEDULE pay and the best time.
- `skills.json` missing or invalid: `Levels[Level]` and `Bosses[Level]` are indexed while empty: a crash at the title.
  Fill eight default rows before the early returns in `FTProgress.cpp`.
- `GI->TitleBanner` is only written on the way back from a level, so THAT'S A WRAP returns on later title visits.
- Extra reels bought "for this run only" are cleared only on one route back to the title.

HUD, menus, input
- Controls navigation: no dead end found, by keyboard or by pad. Rough edges: an action can be left unbound with no
  warning (RESET TO DEFAULTS recovers); Right Shift/Ctrl alias the left keys, so two actions can share one; RESHOOT on
  the results screens and mute are keyboard-only.
- On the results and pre-production screens the arrow keys flip a WASD player to the ARROWS layout and rewrite
  `settings.json` (`bRaceInput` is cleared only on pause and attract).
- FIXED on `main` by 3.4's loop cycle 5: HUD timers ran on through a pause, so the scene name, the intro line and
  LAST CHANCE! expired unread.
- FIXED on `main` by 3.4's loop cycle 4 (`a9f42805`): the chopper's rotor loop played through pause and the results
  screens (`FTGameMode.cpp` pauses `Rotor` with the race audio; log `FT AUDIO loops paused=`).
- FIXED on `main` (`cafef2f4`): Options said Low/Medium use TSR and High enables Lumen. The text in `FTHUD.cpp` now
  reads "Low and Medium use TAA with Lumen off. High uses TSR. Epic adds Lumen lighting and reflections."
- FIXED on `main` by 3.4's loop cycle 6: mute was lost at every map load. Still LIKELY: the title song leaks at
  attract cuts.

Runtime
- First-use synchronous loads during play (about 50 sounds, effects, foe and boss meshes): a load and a flush on the
  game thread each, again every level. Load the level's list in `SetupLevel`; only pickups are warmed today.
- `ApplyCosmetics` calls `StaticLoadObject` twice a frame with freshly built strings; a missing variant would be a
  failed load every frame.
- The first race reloads the map the title just built (one extra map load and track build).
- A kill sets up three geometry collections, a wreck, effects and pickups in one frame; pooled proxies keep every
  wheel and part they ever had, and bullets reuse ex-cars.
- Replay snapshots stop being pruned once a highlight exists (tens of MB by the end of a level).
- Frame-rate dependence: the limo's stream is about 12 % sparser at 30 fps; player missiles have no swept test and
  pass through a bike below about 27 fps.
- FIXED on `main` by 3.4's loop cycle 7: the generic range cull covered the boss, and a culled boss left a level
  that could not end until a wreck.
- LIKELY, for Matt: on Ice Mountain the harness's autopilot comes to a stop against the track's own wall now and
  then and is moved on (`FTSTALL`): at 2.37 and 2.39 km (just after the first jump) and once at 5.77 km, three times in
  about nine autopilot runs of 15 Nov, on builds with and without the second dressing pass (dressed pieces have no
  collision). No other level logged one that night bar a single stall on Chemical Works at 1.23 km. It may be the
  autopilot's line and nothing a player meets; the landing after the first jump is the place to look.
- A level's first build in a session read its scenery meshes from a cold disk in 6.2 s (Theme Park, `props=6155ms`,
  an uncooked run on a hard disk); with the meshes in memory the same placement takes 0.05 to 0.3 s.
- Shipping: `-ftexec`, `ft.Press`, `ft.Click`, `-ftauto` and `-fthp` still work, and Remote Control ships in every
  target. `/Engine/BasicShapes/Cylinder` (pillars, the SWAT rope) is named only in code: check it is cooked.

Attack presentation (the review of the effects change found no change to damage, cooldowns, speeds, ranges,
targeting, collision or hit timing)
- A henchman's landing mark on a half pipe sits at flat-road height; the pie is drawn about 124 cm across on a
  40 cm hit box; blast rings are circles where the real areas are boxes 1.5 times longer along the road; the red
  hit burst shows even when no damage was applied; a ghost missile loses its flame in the reel.

## Scenery and rendering cost

- Every prop cast a shadow into the virtual shadow maps as non-Nanite geometry. FIXED in part: a dressed piece now
  casts only within 70 m of the road and over 2.5 m tall (`FTTrack.cpp`, the dressing's `Put`; `ft.DressShadows 0`
  for the old behaviour; the `FTDRESS` log line ends `shadows=A of B`).
- In dense Bayshore the engine reports "[VSM] Non-Nanite Marking Job Queue overflow" on screen through the first
  12 seconds of the race (3440x1440, Epic: the frames at 8 s and 20 s say "2 seconds ago" and "8 seconds ago"; gone
  by 35 s). With `r.Shadow.Virtual.NonNanite.IncludeInCoarsePages 0` it shows only as the level starts ("2 seconds
  ago" at 4 s, "7 seconds ago" at 8 s, gone by 12 s; run `shadow_coarse0`), and the frames look the same. OPEN, for
  the performance pass: that setting also changes how volumetric fog is shadowed on every level, and its cost and
  gain were not measured (no idle GPU that night). Next step: one `[ShadowQuality@N]` line, measured and looked at
  level by level, as the 3.4 pass did for distance-field shadows.
- First-use loads: the eight-level run of 15 Nov (02:27, before 3.4's preload) logged 15 to 22 `FlushAsyncLoading`
  lines in the first minute of every level before a single wreck. FIXED on `main` by 3.4's loop cycle 1
  (`UFTGameInstance::WarmGameplayAssets`: sounds, effects and vehicle meshes preloaded while the title is up).
  `SourceArt/Jam/unreal/jam_late.ps1` counts what is left, level by level.
  OPEN, for the performance pass: in an uncooked run (`UnrealEditor -game`) that preload finds nothing. The log says
  `FT WARM no assets found (asset registry empty?)`: the registry is still gathering when the title starts, and
  `bWarmStarted` is already set, so it is not tried again. The eight-level run of 03:42 on 3.4 therefore still shows
  17 to 24 synchronous loads a level and frames of 66 to 332 ms at the first spawns. A packaged build reads its
  registry from the cook and should not be affected (to be confirmed by the packaged run). Next step: do not latch
  `bWarmStarted` while `IAssetRegistry::IsLoadingAssets()` is true or the list is empty, so the next map start tries again.
- The dressing's shadow rule, counted (instances in the dressing's components that cast a shadow; the same build
  with `ft.DressShadows 0` and with the rule, runs `shadow_ruleoff` and `shadow_ruleon` of 15 Nov):

  | Level | Without the rule | With it | Of |
  |---|---|---|---|
  | Bayshore | 4,542 | 2,447 | 5,319 |
  | Chemical Works | 4,925 | 3,712 | 5,372 |
  | Nevada | 6,914 | 5,990 | 7,295 |
  | Volcano | 2,353 | 1,238 | 3,812 |
  | Theme Park | 4,289 | 4,243 | 5,794 |
  | Ice Mountain | 3,395 | 1,320 | 3,964 |
  | Under the Sea | 6,195 | 5,272 | 6,899 |
  | City Center | 3,048 | 1,556 | 4,139 |

  What that is worth in milliseconds was not measured (no idle GPU that night).
- One instanced component per mesh for the whole 25 km; a mesh whose radius is over 40 m never culls, and the cull
  distance comes from whichever rule made the component first.
- One synchronous load per scenery mesh at each level start and at each attract cut.

## What to measure (not measured in this pass)

1. Cold-start hitches: a Development package, `-ftbench -ftseconds=120` with a load-time trace; worst and p99 frame.
2. Scenery before and after: the same benchmark with and without `-ftnodress`; shadow depths in `profilegpu`.
3. Boss fights: `FTBossNow` on the Volcano with `stat Niagara`, at the start and after 60 s.
4. The same autopilot level at 30 and at 144 fps.
