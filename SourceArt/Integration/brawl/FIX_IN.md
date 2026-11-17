# Brawl: the code-corrected drawings put into the game

17 Nov 2026 (UTC). Checkout `C:\ftwork\wt\brawl` (project `full-throttle-unreal`, branch `claude/brawl`, on top of
`c447593d`). Nothing is committed; git was not run. No outside service was used; images were handled with PIL / numpy
only. Everything below was run and seen in this pass unless it says "not checked". Scratch: `C:\ftwork\final\brawl\fixin\`.

## 1. Counts

Read: `fixart\<Who>\out` and `out_new` of 37 folders (36 characters + NovaMoves): **729 PNGs**, all 1024x1024 RGBA
(707 in `out`, 22 in `out_new`). One line each in `MANIFEST.csv` (who, file, folder, kind, texture asset, action,
importer, chart states, source, whether the source PNG was replaced, where the original is kept, why not).

| | files | what happened |
|---|---:|---|
| corrected key drawings replaced | 387 | 385 from `out`, 2 from `out_new` (Barman `throw`, Tourist `throw`); `brawl_fix_import.py` |
| corrected in-betweens (s2) replaced | 297 | `brawl_s2_import.py` |
| new textures | 17 | all `_s2_` names, `brawl_s2_import.py`, each with a chart row (section 3) |
| **imported** | **701** | 684 existing textures replaced + 17 new |
| not imported | 28 | section 4 |

Imported per folder: Barman 31, Biker 35, Boarder 17, Bouncer 21, Commodore 22, Dealer 22, Deckhand 16, Disco 6, Dock 4,
Earl 31, Firedancer 18, Firstmate 10, Frankie 32, Goalie 1, Guard 58, Hustler 7, Kingpin 21, Lanehog 9, Leaguer 20,
Linemate 10, Muscle 8, Nova 84, NovaMoves 8, Pinsetter 11, Pitboss 16, Pitcook 7, Punk 16, Roller 3, Shoeclerk 5, Skibum 6,
Sleaze 21, Surfer 21, Sven 23, Tourist 8, Tuxbrute 14, Vinnie 56, Waiter 3.

## 2. How it was imported

- **Keys**: new script `Content\Python\brawl_fix_import.py` (list file in `FT_FIX_IMPORT`, one `png|package` per line; it
  only replaces textures that exist, refuses `_s2_` names, reads the settings before, imports with `replace_existing`,
  puts back any setting the import changed, refuses to save if size or a setting differs, logs one line per texture).
  Run: tag `fixin_keys`, log `C:\ftwork\logs\fixin_keys.log`; its lines are in `import_keys_lines.txt`:
  **387 lines "settings=unchanged", 0 refused, 0 settings had to be put back**; per-texture settings before and after in
  `keys.txt.report.json`. (The first try refused all 387 and saved nothing: a texture just loaded reports 32x32, so the
  size before is now read from the asset registry's `Dimensions` tag.)
- The key PNGs were first copied over their sources in the checkout (all 387 are under
  `beat-em-up-unreal\ArtSource\`: `frames` 107, `studio-pass-v2\player-street\frames` 160,
  `studio-pass-v2\bosses\frames` 112, `boss-polish` 8; before that, every source was byte-identical to the copy in
  `src\<Who>\`) and imported FROM those paths, so source and asset agree. Whether git tracks each of those PNGs: not
  checked (git not run); they are the paths in `src\<Who>\index.json`.
- The two studio-pass `manifest.json` files carry a `sha256` per source: updated for the 272 replaced sources
  (160 + 112), same formatting (the files round-trip byte for byte before the change).
- **In-betweens**: `brawl_s2_import.py` with `FT_S2_IMPORT` = 31 folders staged in `fixin\stage\<Who>\frames`
  (Nova's folder holds Nova's and NovaMoves' files). Tag `fixin_s2`, `import_s2_lines.txt`: **imported 314, refused 0**;
  reports in `stage\<Who>\import-report.json`. The scratch originals in `art\<Who>\frames` were left as they were.
- **Settings proof over the whole folder**: `tools\dump_settings.py` wrote size (registry tag) and 17 settings of every
  texture under `/Game/Brawl/Sprites` before (`settings_before.json`, 3,305 textures) and after (`settings_after.json`,
  3,322): **0 textures differ in size or any setting, 0 gone, 17 new**, and each of the 17 has exactly the settings of its
  character's `T_<who>_idle`.
- **Nova's hand anchors** (the corrected studio keys sit 3 to 5 px lower): `FTBrawlStudioAssets.inl` - 65 anchors moved
  to the values in `fixart\Nova\grips_new.txt` (64 studio keys + `s2_drink_09`), and 4 added for new drawings:
  `s2_idle_11` (628,622), `s2_idle_12` (628,625), `s2_idle_13` (640,657) (their key's new anchor carried through the
  x0.99 / x0.98 / x1.01 dip above row 850: my arithmetic), `s2_wpndash_08` (628,630) (NovaMoves `NEW.md`). The same 64
  grips were changed in `player-street\manifest.json`, which is what `write_tables.py` regenerates the .inl from; the 4
  added lines and `s2_drink_09` are only in the .inl (as the other s2 anchors already were). **Not seen with a weapon in
  the hand**: the pose stage cannot give Nova one.
- Compile: `build.ps1 -Tag fixin_build1`: `FTBrawlFighter.cpp` recompiled, linked, **"Result: Succeeded"**
  (`C:\ftwork\logs\fixin_build1.log`).

## 3. New drawings and chart rows (no `// KEY` row moved, no state length changed)

| Chart | Change | Drawing |
|---|---|---|
| Kingpin | `wind` + row 200 | `s2_wind_10` (new) |
| Kingpin | `fly` + rows 133.333 `s2_fly_04`, 200 `s2_fly_01`, 266.667 `s2_fly_05`, 313.333 `s2_fly_01` | `s2_fly_04`, `s2_fly_05` (new) |
| Nova | `idle` + rows 187.5 `s2_idle_14`, 500 `s2_idle_11`, 625 `s2_idle_12`, 937.5 `s2_idle_13`, 1312.5 `s2_idle_11` | 4 new |
| NovaMoves | `heavy` 316.667 `nweapon04` -> `s2_heavy_10`; `wpndash` 300 `nweapon04` -> `s2_wpndash_08`; `slam` 0 `overheadaim` -> `s2_slam_07` (the three rows were marked `// OLD`, not KEY) | 3 new |
| Surfer | `wind` + row 116.667 `s2_wind_07`; `wind` 166.667 `s2_wind_01` -> `s2_wind_08`; `grab` + row 116.667 `s2_grab_08` | 3 new |
| Tourist | `throw` 316.667 `throw` -> `s2_throw_21` ("not a contract row" in the chart's own comment), with the new `throw` key (drink in the hand) | 1 new + key |
| Vinnie | `wind` + rows 216.667 `s2_wind_07`, 325 `s2_wind_08` | 2 new |
| Barman | `throw` 316.667 `throw` -> `s2_throw_rel`, with `out_new\throw.png` as the key (Barman `REPORT.md`) | 1 new + key |

The optional rows of the NEW.md files (Kingpin's second `wind` row, Vinnie's `aim` row) were not added. Each edited chart
was checked with `contract_check.py --chart` before it went in (all "CONTRACT HOLDS"); copies in `charts_new\`, script
`tools\chart_edit.py`. The sweep after (section 5) shows exactly these changes and no other
(`table_before.json` against `table_after.json`; Nova's `walk>idle`, `jump>idle`, `dive>idle` run on into `idle` and so
show `s2_idle_14` too).

## 4. Not imported (28 files) and proposals not applied

- **Nova `out\turn.png`, `jumpfall.png`, `overheadaim.png`**: enlarged 8 to 22 %. Their hand anchors are written per
  drawing in `FTBrawlGameMode.cpp` (`GripByFrame`: turn 569,614; jumpfall 632,649; overheadaim 432,394), a fight file this
  pass did not edit, and no corrector gave new values: a held weapon would sit about 40 to 50 px off the hand
  (my estimate from the scale factors). They go in once those three anchors are set.
- **Nova `out\T_nova_s2_heavy_08.png`, `T_nova_s2_stomp_01.png`**: two versions exist; the NovaMoves ones were used
  (NovaMoves `REPORT.md`: use ONE; its heavy_08 at 0.97 is the one the heavy / special / slam measures were balanced on).
- **Barman `out\throw.png`**: superseded by `out_new\throw.png` plus the row above.
- **Commodore `out_new\T_commodore_s2_hurt_01.png`** (rows hurt 33.333, fly 33.333): its `NEW.md` says the reviewer asked
  that the rows are not put back before this version has been looked at. Left a proposal.
- **Sleaze `out_new\srecover01b.png`**: the two rows that would use it (swing 140, hurt 210) are `// KEY` rows.
- **Guard `out_new\srecover01h.png`**: the hurt row it would replace is a `// KEY` row (Guard `REPORT.md` leaves it to the lead).
- **19 corrected in-betweens that are on no chart and have no texture in the checkout** (taken off by the first fix
  round; importing them would only add unused textures to the folders the night warms): Dealer `s2_idle_02`, `s2_idle_05`;
  Deckhand `s2_dash_02`, `s2_walk_08`, `s2_walk_11`; Pitcook `s2_swing_05`, `s2_wind_02`; Roller `s2_dash_04`;
  Skibum `s2_getup_10`, `s2_getup_11`; Tuxbrute `s2_fly_01` to `_08`, `s2_wind_03`.
- Row proposals that are not `out_new` drawings, **not applied** (they would put those off-chart drawings, or existing
  ones, back): Dealer `NEW.md` (idle 233.333 / 1166.667; the card-fan pair in `out_with_chart\` with the getup row),
  Dock (fly 333.333 / 366.667 / 400), Earl (getup 49.7 / 99.7, defeat 83.033 / 166.367), Pitcook (wind 83.333, swing 150),
  Tuxbrute (wind 100, down 33.333; fly withdrawn by its author), Commodore (two `wind` rows for `s2_wind_04`),
  Deckhand `proposed_needs_yes\` (2 files, not in `out`). Pinsetter proposes nothing.

## 5. Proofs (build `fixin_build1`, uncooked, `run_game.ps1`, `-brawlnocut -ftslot=pie -ftnoattract`, one run each)

| Proof | Result | Files |
|---|---|---|
| Contract sweep, before (untouched build) | 492 states checked, 0 FAILED, 3555 added, CONTRACT HOLDS | `runs\sweep_before\run.log`, `before\contract_before.txt` |
| Contract sweep, after | **492 states checked, 0 FAILED, 3571 drawings added between the keys, CONTRACT HOLDS**; 36 chart lines, all `missing=0`; 0 `Error:` | `runs\sweep_after\run.log`, `after\contract_after.txt` (job `jobs\sweep_all.txt` = the 492 states + Nova's 20 newer moves, 512 sweeps) |
| Night 1, autopilot, door to win | won (`kos=19 best=7 hits=18 time=63`) | `runs\n1\run.log` |
| Night 2 | won (`kos=16 best=29 hits=6 time=80`) | `runs\n2\run.log` |
| Night 3 | won (`kos=17 best=21 hits=7 time=79`) | `runs\n3\run.log` |
| Night 4 | won (`kos=19 best=11 hits=20 time=85`) | `runs\n4\run.log` |
| Night 5 | won (`kos=17 best=18 hits=11 time=72`) | `runs\n5\run.log` |
| Night 6 | won (`kos=16 best=37 hits=6 time=68`) | `runs\n6\run.log` |
| Night 7 | won (`kos=17 best=15 hits=10 time=84`) | `runs\n7\run.log` |
| In each of the seven | 0 Fatal, 0 `LogFT: Error`, 0 `Error:`, 0 "chart missing drawing", 0 "sprites late", every chart line `missing=0` | counts in `runs\prove.txt` |
| Sprite memory, night 1 (`jobs\perf_N1.txt`, 1920x1080, memreport) | before **1,121 MB in 815**; after **1,130 MB in 822** (+7 textures = Nova's 4 idle + 3 move drawings, +9.6 MB); warm-up line `assets=824` | `runs\perfN1_before`, `runs\perfN1_after` (memreport + CSV in each) |
| Frame time in the same capture (not a goal of this pass) | before avg 4.49 / p99 5.64 ms, after 4.50 / 5.71 ms, 0 frames over 16.7 ms in both | same |

The sweeps and film runs pose characters of other nights in night 1's room, so they log "sprites late ... teststage=1"
Display lines (30 per sweep); that is the test stage, as in WARM_FIX.md. Night scripts: `jobs\n1.txt` ... `n7.txt`
(copies of `warm_fix2\jobs` and `warm_fix\jobs\n3_full`, `n7_full`). The nights' film pictures were thinned to every
tenth of the first hundred to save disk (the disk has about 20 GB free); looked at: `runs\n1\n1_w1x_0091.png`,
`runs\n7\n7_p2_0041.png` (normal).

## 6. Before / after evidence

- States filmed: every state of the BEFORE sweep in which a drawing to be replaced is shown, plus the states whose chart
  rows changed: **339 states of 512** (`touched_states.json`), the same list before and after.
- Films: the game's pose stage, `ftb.PoseFilm <who> <state> 1 t0=<start + 2 ms>` + `film ... 60`, 1280x720, six runs
  per side (`jobs\film_00.txt` ... `film_05.txt`, `runs\filmB_*` before, `runs\filmA_*` after; 12,344 pictures a side).
  Cropped to the fighter (the same box before and after, `before\boxes.json`) into `before\<Who>\<state>_<nnnn>.jpg` and
  `after\<Who>\...`, with a strip per state (`<state>_strip.png`: the first picture of each drawing) and
  `before\films.jsonl` / `after\films.jsonl` (which drawing each picture shows, from the sweep). The uncropped pictures
  were deleted. The BEFORE films were made on the untouched build, before any file in the checkout was changed.
- Sheets: `compare\<Who>\<state>.png`, **339 sheets in 36 folders** (NovaMoves' states are in `compare\Nova`), BEFORE row
  above AFTER row, one column at every picture where the drawing changes; green label = corrected drawing, yellow = new
  on that row. `compare\index.json` lists per sheet the corrected drawings and row changes it shows. **Every one of the
  701 imported drawings is on at least one sheet** (checked by script).
- Looked at by me: `compare\Nova\idle.png`, `Punk\walk.png`, `Tourist\throw.png`, `Kingpin\fly.png`, `Biker\wind.png`
  (the corrected drawings are the ones in the game; nothing broken seen). The other 334 sheets were made, not looked at.

## 7. Files changed in the checkout (from my own actions; `CHANGED_FILES.txt` has all 1,099 lines)

The list was also found a second way, by modification time since the import began: the same 1,099 files, none other
(`changed_files_found.txt`; `Saved`, `Intermediate`, `DerivedDataCache`, `Binaries` not counted).

- 684 modified + 17 new `full-throttle-unreal/Content/Brawl/Sprites/<Who>/T_*.uasset`
- 387 modified source PNGs under `beat-em-up-unreal/ArtSource/` (`frames`, `studio-pass-v2/player-street/frames`,
  `studio-pass-v2/bosses/frames`, `boss-polish`)
- `beat-em-up-unreal/ArtSource/studio-pass-v2/player-street/manifest.json`, `.../bosses/manifest.json` (sha256; Nova grips)
- `full-throttle-unreal/Source/FullThrottle/Brawl/FTBrawlStudioAssets.inl` (65 anchors moved, 4 added)
- `full-throttle-unreal/Source/FullThrottle/Brawl/Charts/FTBrawlChart_Barman.inl`, `_Kingpin`, `_Nova`, `_NovaMoves`,
  `_Surfer`, `_Tourist`, `_Vinnie` (line endings kept as each file had them)
- new: `full-throttle-unreal/Content/Python/brawl_fix_import.py`

Not touched: `M_PP_Cel`, `FTTrack*`, the road, any `.cpp` / `.h`, any other chart.

## 8. Putting one drawing back

Every replaced original is in `fixin\orig\` under the same relative path as in the checkout: 684 `.uasset`, 387 source
PNGs, the two manifests, `FTBrawlStudioAssets.inl`, the seven charts.

- One drawing, with no Unreal process open on the checkout: `python fixin\tools\put_back.py T_punk_swalk01` copies
  `orig\full-throttle-unreal\Content\Brawl\Sprites\Punk\T_punk_swalk01.uasset` and, for a key, its source PNG back
  (written, **not run**). By hand it is those two copies. No compile is needed for a replaced drawing.
- Or through the editor: put the original PNG on a list for `brawl_fix_import.py` (keys) or in a folder for
  `brawl_s2_import.py` (in-betweens; originals in `art\<Who>\frames`).
- A new drawing (the 17): take its row(s) of section 3 out of the chart (or copy the chart back from `orig\`), delete
  its `.uasset`, compile. A chart row that names a drawing with no texture logs "chart missing drawing".
- A Nova studio key put back alone leaves its anchor 3 to 5 px low: also restore its line in
  `FTBrawlStudioAssets.inl` (old values in `fixart\Nova\grips_new.txt`) and compile. Putting a studio source PNG back
  leaves its `sha256` in the manifest stale (the old manifest is in `orig\`).

## 9. Not checked / to know

- Nothing was watched in motion or listened to; the sheets are still pictures of the game. 334 of 339 sheets not looked at.
- Nova with a weapon in the hand after the anchor changes; the corrected `idle` key (enlarged about 15 % above the
  belt) and `s2_idle_14` use the state's default anchor (620,605) as `idle` did before: how the weapon sits on them was
  not seen. No anchors exist for `s2_heavy_10` and `s2_slam_07` (their neighbours have none either).
- A cooked build; a checkpoint restore; the days and cutscenes; nights opened at the boss.
- Whether git tracks each replaced source PNG; `git status` (forbidden here).
- The Windows user name was replaced by `USER` in the logs under `fixin\` (paths in engine logs), nothing else in them was altered.

---

# Repair round (17 Nov 2026 UTC, after the independent in-game review)

Checkout as the lead left it (said to be clean at `c447593d`; git was not run here). No outside service. No Unreal
process of this checkout was open when the files were copied (`tasklist`: 0 UnrealEditor).

## Put back (exactly two drawings, nothing else)

| Who | Drawing | Why (the review's words, shortened) |
|---|---|---|
| Disco | `T_disco_s2_fly_01` | low confidence, "revert when unsure": enlarging it to the hurt key's size only moved the size pop to between `s2_fly_02` and the untouched `s2_fly_03` |
| Disco | `T_disco_s2_fly_02` | goes with `s2_fly_01`: after the change it was visibly larger than `s2_fly_03` |

- How: `python tools\put_back.py T_disco_s2_fly_01 T_disco_s2_fly_02` (its first run): it copied
  `orig\full-throttle-unreal\Content\Brawl\Sprites\Disco\T_disco_s2_fly_01.uasset` and `..._02.uasset` over the
  checkout's. sha256 of the checkout's two files after the copy equals the originals' (`9962ed5f...`, `a310aa47...`;
  before the copy they were `69cf32ec...`, `920df4ae...`).
- Tracked source PNG: there is none for these two. They are in-betweens; their source is the scratch
  `art\Disco\frames\` (not in the repository, never replaced: `MANIFEST.csv`, column "source PNG replaced" = no). A search
  of the checkout for `*disco_s2_fly_0[12]*` finds only the two `.uasset`.
- No chart, manifest, anchor or code was touched; no compile was needed or made (no file under `Source` is newer than
  `UnrealEditor-FullThrottle.dll`, 21:20:41 local). Disco's other four corrected drawings (`hurt`, `punch`, `walk2`,
  `s2_getup_04`) stay in.
- For the record, the review's condition for taking them again: bring `s2_fly_03` onward to the same size as well.

## Proofs after the put-back (same build, same scripts and switches as section 5, one run each)

Script `tools\prove_rr.sh`, output `runs\prove_rr.txt`.

| Proof | Result | Files |
|---|---|---|
| Contract sweep (`jobs\sweep_all.txt`) | **492 states checked, 0 FAILED, 3571 drawings added between the keys, CONTRACT HOLDS**; 36 chart lines, all `missing=0` (Disco: `states=12 rows=107 drawings=88 missing=0`); 0 Fatal, 0 `LogFT: Error`, 0 `Error:`, 0 "chart missing drawing"; 30 "sprites late" lines, the test stage's own (as before) | `runs\rr_sweep\run.log`, `after\contract_rr.txt` |
| Night 1 | won (`kos=19 best=7 hits=18 time=63`) | `runs\rr_n1\run.log` |
| Night 2 (Disco's night) | won (`kos=16 best=29 hits=6 time=80`) | `runs\rr_n2\run.log` |
| Night 3 | won (`kos=17 best=21 hits=7 time=79`) | `runs\rr_n3\run.log` |
| Night 4 | won (`kos=19 best=11 hits=20 time=85`) | `runs\rr_n4\run.log` |
| Night 5 | won (`kos=17 best=18 hits=11 time=72`) | `runs\rr_n5\run.log` |
| Night 6 | won (`kos=16 best=37 hits=6 time=68`) | `runs\rr_n6\run.log` |
| Night 7 | won (`kos=17 best=15 hits=10 time=84`) | `runs\rr_n7\run.log` |
| In each of the seven | 0 Fatal, 0 `LogFT: Error`, 0 `Error:`, 0 "chart missing drawing", 0 "sprites late", every chart line `missing=0` (6, 7, 6, 6, 6, 6, 6 lines) | counted from each `run.log` |

All seven win lines are the same as in section 5. Each run ended "exit 0".

## Final list of changed files in the checkout

- Against the state before the fix-in pass: **1,097 files** = section 7's 1,099 less the two Disco assets
  (`CHANGED_FILES_after_repair.txt`; `CHANGED_FILES.txt` is left as the first pass wrote it):
  682 modified + 17 new sprite `.uasset`, 387 source PNGs, the two `manifest.json`, `FTBrawlStudioAssets.inl`,
  seven charts, `Content/Python/brawl_fix_import.py`.
- Against the lead's commit of the fix-in pass (if that commit holds all 1,099): **2 files modified by this round**:
  `full-throttle-unreal/Content/Brawl/Sprites/Disco/T_disco_s2_fly_01.uasset`
  `full-throttle-unreal/Content/Brawl/Sprites/Disco/T_disco_s2_fly_02.uasset`
  (both byte-identical to what they were before the fix-in pass). Nothing else in the checkout was written by this round
  (`Saved`, `Intermediate`, `DerivedDataCache` not counted).
- Imported drawings now in the game: 699 (682 replaced + 17 new). `MANIFEST.csv`, `compare\` and the before/after films
  were not redone: their two Disco `fly` rows and `compare\Disco\fly.png` still show the corrected versions that are no
  longer in the game.

## Not checked in this round

- The two put-back drawings were not looked at in the game (proof is the checksum and the sweep); Disco's `fly` was not
  filmed again. `git status` (forbidden here). A cooked build. Nothing watched in motion.
- The nights' film pictures were thinned as before (every tenth of the first hundred per film kept). The Windows user
  name was replaced by `USER` in the new logs under `runs\rr_*`.
