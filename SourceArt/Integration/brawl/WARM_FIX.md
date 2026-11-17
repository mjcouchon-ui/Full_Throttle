# Brawl: warm only the night, and no load when a foe appears

17 Nov 2026 (UTC), checkout `C:\ftwork\wt\brawl`, branch `claude/brawl`, on top of `2683bdc5`. Not committed (the lead commits).
Last compile: tag `warm_build7`, "Result: Succeeded". Everything below was run through `run_game.ps1`, uncooked
(`UnrealEditor -game`), with the switches of the earlier brawl runs (`-brawlnocut -ftslot=pie -ftnoattract`), one run per case.
Files: `C:\ftwork\final\brawl\warm_fix\` (logs, CSV, memreports, films, `look\` contact sheets, `jobs\` scripts, `run.sh`).

## What changed (file:line)

New: `Source/FullThrottle/Brawl/FTBrawlSpriteBank.h`, `.cpp`: a world subsystem, so it lives and dies with the brawl map.
- `Table(Who)` (`.cpp:107`): one table per character, built once per map from the asset registry's listing of his folder.
  It holds every drawing the folder has, so a name that is not in it does not exist: hits and misses, without asking the
  loader. Held in `UPROPERTY` maps (`Tables`, `Extras`) and one streamable handle.
- `WarmNight` (`.cpp:67`): one blocking request (`RequestSyncLoad`) for the night's cast, the item and prop folders and
  the foes' material. No async or high-priority loading during play.
- `FinishTextures` (`.cpp:38`): uncooked game only (`WITH_EDITOR`): waits until the engine has finished preparing the
  loaded textures. Without it the first second of a night drew grey checkers (see "Found on the way").
- `Deinitialize` (`.cpp:121`): drops tables and handle when the map ends.
- `Get` (`.cpp:20`): `-brawlnobank` on the command line switches the bank off (the old lookup by path), for comparisons.

`Brawl/FTBrawlFighter.cpp:231-246, 255, 280`: `SetSprite` reads its three name lists from the table instead of
`LoadObject` per name. Same lists, same order, same "chart missing drawing" error. `FTBrawlFighter.h:49`, `.cpp:327`: the
foes' material path in one place.

`Brawl/FTBrawlGameMode.cpp`: `SpriteChoices` (`:396`) is the role-to-folder decision that was inside `SpawnFighter`,
moved out unchanged; `SpawnFighter` (`:420`) walks it and `NightSprites` (`:446`) reads the same list for the waves, the
boss's call (`BossHelpKind`, `:433`, the tables that were inline at what is now `:1147`) and the boss, so spawn and
warm-up cannot differ. `WarmNight` (`:467`) is called in `BeginPlay` (`:275`) before the room is built; `FinishTextures`
at `:327`. `FTBrawlGameMode.h:341-347`: the declarations. `Brawl/FTBrawlCutscene.cpp:219-234`: `CutSprites`, the
cutscene people (night 1's five names are written out there; nights 2 to 7 and the days read the cutscene tables).

`FTGameInstance.cpp:475, 488-490`: the sprite folder is no longer part of the once-per-process brawl warm-up; the
sounds still are, the driving warm-up is untouched. `ft.WarmBrawlSprites 1` gives the old behaviour back.

Not touched: `M_PP_Cel`, `FTTrack*`, the road, any timing, damage, AI or input.

## Measurements (1920x1080 off screen, uncapped)

Before = `perf_fix\N1` (night 1) and `perf_after\N3boss`, `N7boss` (PERF_AFTER_S2.md). After = `warm_fix\N1`, `N3boss`, `N7boss`.
No other UnrealEditor process was listed by `tasklist` before each run.

| | N1 before | N1 after | N3 boss before | after | N7 boss before | after |
|---|---|---|---|---|---|---|
| FrameTime avg / p99 / max (ms), 5 s capture | 7.53 / 19.34 / 23.30 | 4.54 / 5.82 / 10.82 | 6.00 / 19.12 / 23.53 | 4.58 / 5.89 / 10.04 | 6.45 / 19.27 / 23.85 | 4.59 / 6.01 / 10.44 |
| GameThread avg / p99 / max | 5.55 / 19.29 / 21.70 | 2.02 / 2.80 / 5.94 | 3.23 / 19.08 / 21.89 | 1.88 / 2.43 / 5.86 | 3.56 / 19.22 / 22.32 | 1.98 / 2.60 / 6.50 |
| Frames over 16.7 ms in the capture | 144 | 0 | 61 | 0 | 68 | 0 |
| Slow start: last frame over 16.7 ms after `FTBRAWL start` (from the log's frame times) | 5.6 s | gone (one long first frame, see below) | 5.75 s | gone | 5.87 s | gone |
| Sprite textures in memory | 4,544 MB in 3,305 | 1,121 MB in 815 | 4,670 MB in 3,396 | 1,084 MB in 788 | 4,670 MB in 3,396 | 1,085 MB in 789 |
| Warm-up | 3,329 assets, 2.85 s, async during play | 816 assets, 1.07 s, blocking before the first frame | 3,420, 2.89 s | 789, 1.03 s | 3,420, 2.91 s | 790, 1.04 s |
| `FTGo` to `FTBRAWL start` | 0.41 s | 1.35 s | 0.58 s | 1.32 s | 0.61 s | 1.34 s |
| PhysicalUsedMB avg | 5,957 | 5,534 | 5,868 | 5,556 | 5,939 | 5,499 |

- After the fix the first frame after the map opens is 102 to 108 ms in every run (one frame, 0.1 s after
  `FTBRAWL start`); the other frames over 16.7 ms in the first 8 s are the script's own commands. Whether that first
  frame was as long before: not checked.
- The 1.07 s is about 0.27 s of loading and 0.8 s of waiting for the uncooked game's texture preparation. A cooked
  build has no such preparation: not measured.

Wave locks and the boss (the frame that logs the line; `FTHITCH2` = a frame over 40 ms):

| | Before | After (`locks_N1`, `locks_N7`: one capture from the night's start to 3 s after the boss) |
|---|---|---|
| Night 1 wave 1 | 101 ms, `FTHITCH2 ms=106 loads=583` (fix round: 119 to 135 ms) | 4 ms, no FTHITCH2 line |
| Night 1 wave 2 | 66 ms, `ms=67 loads=506` | 4 ms |
| Night 1 wave 3 | 84 ms (S2_STATUS) | 3 ms |
| Night 1 boss | not measured | 5 ms |
| Night 7 waves 1, 2, 3, boss | not measured | 5, 4, 4, 5 ms |
| Whole capture | | night 1: 9,196 frames, max 23.85 ms, 4 over 16.7 ms; night 7: 11,669 frames, max 24.66 ms, 3 over 16.7 ms; none over 33.3 ms |

The only FTHITCH2 lines left in those runs are the frames that write the CSV, the memreport and the screenshot.

Two nights and a drive in one process (`two`: night 1, then `FTGo N2`, then `FTGo L1`, a memreport in each):

| Map | Sprite textures | Sprite folders in memory |
|---|---|---|
| Night 1 | 1,121 MB in 815 | Nova, Punk, Sleaze, Biker, Bouncer, Earl, Photo, Kip, Items, Props |
| Night 2 | 1,240 MB in 902 | Nova, Disco, Barman, Hustler, Guard, Bouncer, Vinnie, Items, Props (none of night 1's own) |
| Bayshore (drive) | 0 MB in 0, and no `/Game/Brawl` texture at all | |

Night 2 in that process: warm-up 903 assets in 1.14 s; the charts it reached (Disco, Barman, Hustler, Guard) logged
`missing=0`; 0 "sprites late" lines. It was played to wave 2 only.

## Proofs

- Contract: `jobs\sweep.txt` -> `warm_fix\sweep\run.log`, `contract_check.py --log`: **492 states checked, 0 FAILED, 3555
  drawings added between the keys, "CONTRACT HOLDS"** (`warm_fix\contract.txt`). 36 charts logged, all `missing=0`.
  (The sweep poses all 36 characters in night 1's room, so it logs 30 "FTBRAWL sprites late" lines: each is a whole
  blocking load of his folder, 35 to 59 ms, before he is shown. That is the test tool, not a night.)
- Autopilot, door to win, 1280x720 (`n1`, `n3`, `n7`): all three won; 0 Fatal, 0 `LogFT: Error`, 0 `Error:` lines of
  any kind, 0 "chart missing drawing", 0 "sprites late", every chart `missing=0`, no FTHITCH2 line with loads.
- Same fight: the fix round's own script `s2_fix_n1.txt`, unchanged, ended `won kos=18 best=10 hits=18 time=58`, the
  same line as `out\s2_fix_n1c`. (My scripts film at other moments and end 19/11/17/65, twice the same.)
- All of the above was run on build 4. Builds 5 and 6 were two attempts at the black frame below, both taken out
  again; build 7 is build 4's source again. On build 7 only night 1's perf capture (0 frames over 16.7 ms, 1,121 MB)
  and the short film run were repeated.

## Looked at

- First second of night 1: 70 film frames (`look\final_first.png`) and stills from 0.1 s to 3 s (`look\shots_new.png`):
  the room, the banner and Nova; no checker, no blank card. In the first still three of the four onlookers are not
  there yet; in the next (0.8 s) they are.
- The first frames at each wave lock and at the boss, nights 1, 3 and 7 (`look\final_locks_n1.png`, `_n3`, `_n7`):
  every foe kind has his drawing in the first frame he is on screen.
- **Seen and not fixed:** in the one frame in which wave 1 locks, a black square stands at the top left of the screen
  (nights 1, 3 and 7; not at waves 2 and 3 or the boss). The old way shows a grey checker square in that place for at
  least four frames (`look\ab_w1.png`, bottom row). Pre-making the material's pipeline states, and hiding a fighter
  until his first pose, did not remove it. Cause not found.

## Found on the way

- The "slow start" was never work on the game thread: in those frames the game thread's own items add up to under
  2 ms and the process uses 3 % CPU against 7 % later. It lasted as long as the uncooked game was still preparing
  textures in the background: 5.7 s with 3,329 textures, about 1 s with 816, and about 1 s (50 to 70 frames) in the
  pre-stage-2 baseline logs too.
- Loading the night's cast up front put those textures ahead of the room's own in that queue, so for about 1 s the
  room drew as grey checkers (`look\ab_first_new.png`, `look\n1_first_second.png`, made before the cure). Hence
  `FinishTextures`; with it both the checkers and the slow frames are gone.

## Not checked

- A cooked or shipping build (load time, the 100 ms first frame, whether the black frame exists there).
- Nights 2, 4, 5 and 6 as whole nights. A night opened through its cutscene ran once (night 1, on the first build, by
  accident) and was not looked at. The day scenes, a checkpoint restore, the career flow from a drive into a night.
- Memory from night to night through the real flow (only `FTGo` was used). Video memory in use.
- Repeatability: one run per case. Nothing was watched in motion or listened to.

---

# Final section: the review round (17 Nov 2026 UTC)

This section replaces what the text above says where the two differ (the "black square", the 35 to 59 ms, the "N1 after"
column). Last compile: tag `warm_build9`, "Result: Succeeded" (DLL 20:13:07, newer than every changed source). Still
uncommitted. Runs: `run_game.ps1`, uncooked, `-brawlnocut -ftslot=pie -ftnoattract` unless said, one run per case unless
said. Files: `C:\ftwork\final\brawl\warm_fix2\` (run folders, `look\` sheets, `jobs\` scripts, `run.sh`, `lockframes.py`).
Builds: nights 1, 2, 4, 5, 6, `n1cut`, `sweep` and `perfN1`/`b`/`c` ran on build 8; build 9 is build 8 plus one test
switch (`-brawlnoshaders`); on build 9: `sweep9`, the four `ab_*` perf runs, `night2_noshaders`, `samefight`.

## Findings and what was done

1. **BROKEN, real, fixed: foes were dark cards, or absent, when wave 1 locked.**
   - Cause: the uncooked game compiles a material's shaders the first time something is drawn with it, and until they
     arrive draws the engine's default material in its place (engine source, `MaterialShared.cpp`, `FMaterial::TryGetShaders`,
     the `WITH_EDITOR` branch). The default material is opaque and one-sided: a foe facing right was a whole dark card,
     a foe facing left (the ones entering from the right) showed the card's back and was not drawn. It happens once per
     process for `M_BrawlEnemyV`, which is why waves 2 and 3 and a retry were clean. It is not texture residency.
   - Fix: `FTBrawlSpriteBank.cpp:48` `FinishMaterials`, called from `WarmNight` (`:123`) before the night's first frame:
     the engine's own `UMaterialInterface::EnsureIsComplete()` on the two fighter materials (`M_BrawlSprite`,
     `M_BrawlEnemyV`; paths in `FTBrawlFighter.h:49-52`). It does nothing in a cooked build. Cost in the warm-up line:
     `shaders=0.08` to `0.13` s in 15 runs. The reviewer's proposal (a covered real quad) was not needed and not built.
   - Proof, same script and same build, switch on and off (night 2, a film of 300 frames that starts before the lock):
     with `-brawlnoshaders` the first picture that has foes shows a dark card at the left edge and nobody on the right
     (`look\n2_lock_noshaders.png`, picture 0103); without the switch the same picture shows all of them as people
     (`look\n2_lock.png`). Both runs end `won kos=16 best=29 hits=6 time=80`.
   - Filmed across the lock on nights 1, 2, 4 (left and right entry) and 5, 6 (right entry): in every one the first
     picture with foes shows them drawn, no card (`look\locks_all.png`, one row per night; `look\n1_lock.png` ...).
     Also the reviewer's own script and frame: `n2\n2_w1_0001.png`, four foes drawn.
   - Seen, not looked into: between the second and third picture of each row the view jumps (Nova stands about 80 px
     further right). It is in the `-brawlnoshaders` run too and in the reviewer's films ("60 to 100 px one frame later").
2. **WEAK, real, partly fixed: the late path.**
   - The figure was wrong. Measured per un-warmed character in the sweeps: 83 to 299 ms (`sweep`), 92 to 292 ms
     (`sweep9`); the reviewer's 86 to 262 ms. "35 to 59 ms" above is withdrawn.
   - `Table()` (`FTBrawlSpriteBank.cpp:129`) now loads the folder with one `RequestSyncLoad` (shared `Load`, `:59`)
     instead of asset by asset. It did not get cheaper (see the figures): the time is not the loading. What it is was
     not measured; the uncooked texture preparation that follows is my guess.
   - Failure rule: after the night's warm-up a late load is logged as `Error: FTBRAWL sprites late ... afterwarm=1`
     (`:144-147`), so the night runs' "0 `Error:` lines" check fails on it; `run.sh` also counts "sprites late".
     The three development commands that stand other characters in the room say so first (`TestStage`:
     `FTBrawlPose.cpp:177`, `FTBrawlMoves.cpp:856`, `FTBrawlVariety.cpp:425`) and keep the Display line
     (`teststage=1`; 30 such lines in each sweep, 0 `Error:`).
   - The Error line itself was never produced: no run had a character outside the night's list.
3. **WEAK, real, covered by the rule in 2, not by a shared table.** `CutSprites` is still a hand copy of the `AddCast`
   names. I did not make the two read one table: night 1's `AddCast` calls are spread over the three cutscenes'
   code and moving them is a bigger edit than this round should make in that file. Somebody added to a cutscene and
   not to `CutSprites` now gives an `Error:` line in that cutscene's run (comment at `FTBrawlCutscene.cpp:219-221`).
   Night 1 with its cutscenes (`n1cut`, no `-brawlnocut`): cut 1 started and ended, the night was won, cuts 2 and 3
   started; 0 "sprites late", 0 `Error:`. The pictures of those cutscenes were not looked at.
4. **WEAK, real, fixed: the "N1 after" row.** Re-measured into folders of their own (`perfN1`, `perfN1b`, `perfN1c`,
   `ab_*`; CSV and memreport in each). The machine was busier in this round (15 to 32 % CPU from other processes
   between runs; no other UnrealEditor), and frame times were higher and uneven: see the table. The earlier
   4.54 / 5.82 / 10.82 row has no file left and is withdrawn; the reviewer's `N1_check` (4.53 / 5.75 / 9.92, build 7)
   is the only build 7 figure with a file.
5. Rejected: none. Not acted on (the review's OK notes): the bank is still created in every world; night 1 still warms
   Photo and Kip with `-brawlnocut`; line endings were left as each file had them (my edits keep each file's own).

## Final before / after (night 1, `jobs\perf_N1.txt`, 1920x1080, 5 s capture from 0.3 s after the wave 1 lock)

| | Before (`perf_fix\N1`, committed code) | After, build 7 (`warm_fix\N1_check`, reviewer's reading) | After, this round, 7 runs |
|---|---|---|---|
| FrameTime avg / p99 / max (ms) | 7.53 / 19.32 / 23.30 | 4.53 / 5.75 / 9.92 | avg 4.77 to 6.87, p99 6.64 to 9.74, max 10.99 to 14.28 |
| GameThread avg (ms) | 5.55 | 2.02 (first report) | 2.17 to 3.30 |
| Frames over 16.7 ms in the capture | 144 | 0 | 0 in all 7 |
| Sprite textures in memory | 4,544 MB in 3,305 | 1,121 MB in 815 | 1,121 MB in 815 (`perfN1`, `perfN1c`) |
| Warm-up | 3,329 assets, 2.85 s, async during play | 816 assets, 1.08 to 1.19 s, blocking | 817 assets, 1.50 to 1.97 s, blocking, of which shaders 0.09 to 0.11 s |
| `FTGo` to `FTBRAWL start` | 0.41 s | 1.46 s | 1.86 s (`perfN1c`), 2.17 s (`perfN1`) |
| First frames over 16.7 ms after start | until 5.6 s | 6 frames, last at 1.61 s | 6 frames, last at 1.83 s (`perfN1c`), 2.40 s (`perfN1`) |
| Wave 1 lock, foes | grey checker cards, 4+ frames (`ab_old`) | dark cards / absent | drawn in the first picture (5 nights) |

- The seven runs of this round: with the shader step `perfN1` 6.87, `perfN1b` 6.71, `perfN1c` 5.25, `ab_on1` 4.77,
  `ab_on2` 6.67; with `-brawlnoshaders` `ab_off1` 5.14, `ab_off2` 6.45 (FrameTime avg, ms). On and off overlap, so the
  slower frames of this round are not from the shader step; I put them down to the machine's load, which I did not
  control. Whether build 9 on a quiet machine gives 4.5 ms again: not checked.
- The warm-up was 0.3 to 0.8 s longer than in the build 7 logs in this round; the shader step accounts for about 0.1 s
  of it, and the night 1 runs with `-brawlnoshaders` were as long (1.50, 1.76 s). The sounds' warm-up, which this
  change does not touch, was also slower in the same runs (0.22 to 0.43 s against 0.19 to 0.22 s).
- Nights 3 and 7 boss rows, the lock table and the two-nights memory table above were not re-measured in this round.

## Proofs of this round

- Contract, after the last code change (`sweep9`, build 9): **492 states checked, 0 FAILED, 3555 drawings added
  between the keys, "CONTRACT HOLDS"** (`warm_fix2\contract9.txt`); 36 charts, all `missing=0`. The same on build 8
  (`sweep`, `contract.txt`).
- Nights 1, 2, 4, 5, 6, autopilot, door to win, 1280x720: all five won. In each: 0 Fatal, 0 `LogFT: Error`, 0 `Error:`,
  0 "chart missing drawing", 0 "sprites late", every chart `missing=0`, no FTHITCH2 with loads above 0, no
  LogStreaming or LogLinker warning.
- Same fight: `jobs\s2_fix_n1.txt`, unchanged, ended `won kos=18 best=10 hits=18 time=58`, the same line as
  `out\s2_fix_n1c` (`samefight`, build 9).

## Files changed (`git status --porcelain`, paths from the repository root)

```
 M full-throttle-unreal/Source/FullThrottle/Brawl/FTBrawlCutscene.cpp
 M full-throttle-unreal/Source/FullThrottle/Brawl/FTBrawlFighter.cpp
 M full-throttle-unreal/Source/FullThrottle/Brawl/FTBrawlFighter.h
 M full-throttle-unreal/Source/FullThrottle/Brawl/FTBrawlGameMode.cpp
 M full-throttle-unreal/Source/FullThrottle/Brawl/FTBrawlGameMode.h
 M full-throttle-unreal/Source/FullThrottle/Brawl/FTBrawlMoves.cpp
 M full-throttle-unreal/Source/FullThrottle/Brawl/FTBrawlPose.cpp
 M full-throttle-unreal/Source/FullThrottle/Brawl/FTBrawlVariety.cpp
 M full-throttle-unreal/Source/FullThrottle/FTGameInstance.cpp
?? full-throttle-unreal/Source/FullThrottle/Brawl/FTBrawlSpriteBank.cpp
?? full-throttle-unreal/Source/FullThrottle/Brawl/FTBrawlSpriteBank.h
```

New in this round: `FTBrawlMoves.cpp`, `FTBrawlPose.cpp`, `FTBrawlVariety.cpp` (one include and one `TestStage` line
each). `FTGameInstance.cpp` was not touched in this round (still one cvar, one condition, two comment lines).
`git diff --stat`: 9 tracked files, 132 insertions, 40 deletions; no whole-file diff. Git warned about line endings on four of the Brawl files at the start of this round; not looked at again.

## Not checked

- A cooked build: load time, whether a foe's first appearance waits there for pipeline states, sprite residency.
- The first time on a machine, or after `M_BrawlSprite` / `M_BrawlEnemyV` is edited: how long the shader step blocks
  (here it was 0.1 s every time, so the shaders were already in the local cache).
- Any other brawl material drawn for the first time during a fight (sparks, rings, items, the bosses' effects): the
  same one-frame default material can happen there in the uncooked game; only the fighters' two materials are made whole.
- A night opened straight at its boss (`FTGo N3boss`) or from a checkpoint, a retry, nights 3 and 7, a day scene, the
  career flow from a drive into a night, two nights in one process: not re-run in this round.
- The `Error:` late line in a real case; `-brawlnobank`; `ft.WarmBrawlSprites 1`.
- The pictures of night 1's cutscenes; waves 2 and 3 and the boss entrance of this round's films (filmed, not looked at).
- Video memory; anything in motion or sound; frame times on a quiet machine.
