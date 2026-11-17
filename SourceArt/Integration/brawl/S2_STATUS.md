# Brawl, stage 2: status after the one fix round

Written 16 Nov 2026 by the fix-round task. Branch `claude/brawl` in `C:\ftwork\wt\brawl`, not pushed, not rebased.
Everything stated as fact was run or looked at in this round unless it says "integrator" or "reviewer" (their numbers,
not re-measured) or "not checked".

Commits of this round (on top of `57327813`):

- `1cccebcc` Brawl fix round: drawings that reversed a motion, popped in size or broke a prop are taken off the charts
- `8ac32937` Brawl fix round: crisp fighters, bosses that get their moves out, the stamp lands, a clear lane
- a third commit holding this file

**The stage is not at the bar yet.** The largest remaining faults need art the fix round did not make (see section 8):
old key drawings that are a different size, costume or prop state than everything round them, on Nova, all six street
enemies and all seven bosses. Texture memory (about 4.5 GB of sprites) and the slow first seconds of a night are also
open; their cause sits in a file outside the brawl folder.

## 1. What a player now sees

- Fighters are crisp while they move. Before this round every swapped drawing came out smeared and speckled for the
  pictures it was up (temporal anti-aliasing blending the previous drawing in). The fight now runs on FXAA and gives the
  driving game's setting back when the map ends. Seen: the same Waiter throw film, old against new
  (`fix\s\ghost2.png`), and night 1 at 1:1 (`fix\s\n1_w1_zoom.png`).
- Motions that ran backwards or flickered are gone from the charts: Nova's kick no longer pulls the leg in on the
  strike, his special no longer flips front, back, front, his idle feet no longer slide, several get-ups no longer
  stand taller than the idle and shrink, several fly states no longer thrash between unrelated poses. The price is
  longer holds in those states (section 3).
- Bosses get their second phase out. It opens with the boss's own doubled move, told as always; Nova is pushed back
  to 180 px during the roar and is no longer hidden under the boss. A boss cannot be floored again for 1 s after getting
  up; in the second phase a warning takes the damage and goes on, and four staggers in a row make him "SHRUG IT OFF" for
  1 s. A thrown body, a full-charge heavy or the last blow still break through.
- The stamp on a floored foe lands: the kick looks 140 px ahead and Nova steps to the man.
- No pier post between the camera and a locked screen. The first-time help is a two-line strip on the bottom edge and
  names dash, sidestep, heavy, launcher and the crowd clearer. Onlookers are dimmer. The hit flash is two bright
  pictures, then a tint. "AGAIN!" is white.

## 2. Drawings per character, before and after

"Textures" = files in `Content\Brawl\Sprites\<Who>`. "Chart rows" = `FTB_ROW` lines of that character's chart(s)
(Nova: both files). Before stage 2 comes from the integrator's report; the other columns were counted in this round.

| Character | Textures before stage 2 | After integration | After fix round | Rows shown before stage 2 (integrator) | Chart rows after integration | Chart rows after fix round |
|---|---:|---:|---:|---:|---:|---:|
| Nova | 110 | 374 | 342 | 109 | 454 | 407 |
| Punk | 30 | 90 | 89 | 53 | 131 | 130 |
| Biker | 28 | 85 | 83 | 46 | 109 | 107 |
| Sleaze | 28 | 81 | 80 | 44 | 124 | 123 |
| Bouncer | 28 | 66 | 66 | 39 | 86 | 84 |
| Guard | 29 | 86 | 86 | 41 | 110 | 110 |
| Dealer | 28 | 84 | 78 | 47 | 120 | 109 |
| Earl | 33 | 93 | 90 | 53 | 170 | 163 |
| Vinnie | 33 | 111 | 109 | 88 | 189 | 184 |
| Pitboss | 33 | 106 | 106 | 60 | 186 | 186 |
| Frankie | 33 | 91 | 91 | 50 | 188 | 188 |
| Kingpin | 33 | 126 | 124 | 52 | 191 | 187 |
| Sven | 33 | 118 | 118 | 60 | 237 | 237 |
| Commodore | 33 | 87 | 85 | 52 | 153 | 146 |
| Disco | 8 | 91 | 88 | 13 | 114 | 107 |
| Hustler | 8 | 89 | 87 | 14 | 106 | 104 |
| Barman | 8 | 75 | 75 | 13 | 104 | 104 |
| Roller | 8 | 76 | 73 | 13 | 98 | 91 |
| Muscle | 8 | 54 | 54 | 12 | 64 | 64 |
| Firedancer | 8 | 81 | 78 | 13 | 108 | 105 |
| Surfer | 8 | 72 | 68 | 14 | 109 | 104 |
| Tourist | 8 | 74 | 70 | 13 | 99 | 94 |
| Pitcook | 8 | 65 | 62 | 12 | 95 | 92 |
| Pinsetter | 8 | 58 | 57 | 13 | 112 | 110 |
| Leaguer | 8 | 66 | 66 | 14 | 103 | 103 |
| Shoeclerk | 8 | 100 | 100 | 13 | 122 | 122 |
| Lanehog | 8 | 65 | 64 | 12 | 94 | 93 |
| Skibum | 8 | 80 | 78 | 13 | 119 | 117 |
| Linemate | 8 | 99 | 99 | 14 | 122 | 122 |
| Boarder | 8 | 102 | 102 | 13 | 120 | 120 |
| Goalie | 8 | 75 | 75 | 12 | 90 | 90 |
| Deckhand | 8 | 79 | 76 | 13 | 113 | 107 |
| Firstmate | 8 | 77 | 77 | 13 | 108 | 108 |
| Waiter | 8 | 94 | 94 | 13 | 122 | 122 |
| Tuxbrute | 8 | 67 | 58 | 12 | 98 | 85 |
| Dock | 8 | 70 | 68 | 12 | 83 | 81 |
| **All 36** | **688** | **3,307** | **3,216** | | | |

Game sweep after the fix round: 3,555 drawings added between the keys (integration: 3,687). 180 of 399 moving states
never hold a drawing over 42 ms (integration: 205): taking a bad drawing out makes its neighbour hold longer.

## 3. The chart fix round

- Method: a row named by a reviewer was first measured (height, width, feet, area of the drawing against its
  neighbours and keys, `fix\m.py`) and the worst were looked at as strips (`fix\s\`). A confirmed drawing was REMOVED
  from the chart, or a row was moved in time. **Nothing was repainted, shifted, scaled or regenerated. The image tool
  was not used in this round (0 generations)**, so every finding whose cure is a new drawing is NOT FIXED.
- 123 edits in `fix\edits.txt` (applied by `fix\apply.py`) plus 9 automatic removals where a drawing was left twice
  in a row. 25 chart files changed. The charts before the round: `fix\charts_before\`.
- No `// KEY` row was moved, renamed or removed (the script refuses to).
- The 91 textures no chart row names any more were deleted from the checkout (`fix\removed_textures.txt`). The source
  drawings stay in `art\<Who>\frames`. The charts in `art\<Who>\` are the art agents' and were NOT updated: the
  checkout's charts are now the truth.
- Holds that got longer because of it (examples): Nova idle is back on its three keys (375 ms holds); Nova jump holds
  130 ms at the top; Nova drink's gulp is two drawings at 67 ms; Kingpin, Vinnie and Tuxbrute fly end on one held
  drawing; Earl's get-up holds the kneel 150 ms; Tourist's recovery holds 100 ms.

## 4. New moves, looks, behaviours, stages, hit feel

Not re-reviewed one by one in this round; what follows is what this round ran.

- Moves (fixture run `out\s2_fix_a`): the stamp now comes out and hits (`move stomp`, `stomp rusher dmg=12`), Nova
  stepping from x=566 to 623. Launcher, two air jabs and a jump kick still juggle (n=1, 2, 3). In the seven autopilot
  nights the stamp was used 0 times: in real play it is still unseen.
- Seven nights with the autopilot after the last code change (night 1: `out\s2_fix_n1c`; nights 2 to 7:
  `out\s2_fix_n2` .. `n7`): all won, no retry, 0 lines matching Fatal / "Ensure condition" / "LogFT: Error" / "Error:".

| Night | Time to clear (s) | Hits taken | Knock-downs of Nova | Second phase began at | Boss moves seen in it |
|---|---:|---:|---:|---|---|
| 1 | 58 | 18 | 3 | 99/210 | slam encore, 1 shrug |
| 2 | 72 | 6 | 2 | 110/230 | none logged (Vinnie has no encore by design) |
| 3 | 68 | 4 | 3 | 118/240 | draw encore, 1 shrug, revolver interrupted once |
| 4 | 71 | 16 | 1 | 109/240 | swig encore |
| 5 | 76 | 13 | 5 | 116/240 | 3 lane strikes, hold encore x2, rush encore, 1 shrug |
| 6 | 75 | 11 | 2 | 124/260 | gloves encore, 1 shrug |
| 7 | 84 | 8 | 6 | 133/280 | windup encore x2, rush encore x2 |

  Reviewer's numbers before the round: 61 / 81 / 70 / 89 / 70 / 93 / 90 s, hits 12 / 7 / 7 / 18 / 13 / 15 / 8; the
  Kingpin had 0 lane strikes and no encore. The runs are not the same fights (the stage's dice were split, so drops
  differ).
- The Kingpin alone (`out\s2_fix_boss4`, autopilot): phase 2 at 113/240, lanes 360,428,496, a ball hit Nova, encore on
  lanes 394,462,530 (three new lanes), won in 25 s with 2 hits taken. Before the boss changes the same script gave no
  lane strike at all (`out\s2_fix_boss`).
- A first version of the boss protection (every blow shrugged off for 1.6 s after three or four staggers, in both
  phases) made Earl beat the autopilot once on night 1 (`out\s2_fix_n1b`: retry, 29 hits, 13 knock-downs). It was cut
  back to what section 1 describes; the numbers above are from the cut-back version.
- Stages: night 1 built 19 breakables as before. Looked at: night 1 waves 1 to 3, the furious beat, the K.O.
  (`fix\s\n1_six.png`) and the Kingpin's second phase (`fix\s\k_six.png`). No post in front of the fight in those.
- Hit feel: the flash is `0.42` for the first pictures then `0.14` (was `0.7` throughout). Not compared side by side.

## 5. Proofs

- Compile: tag `s2_fix_build`, "Result: Succeeded" (last build after the last code change).
- Contract, from the game (`jobs\sweep.txt` -> `out\s2_fix_sweep\run.log`, run after the last chart and code change):
  **492 states checked, 0 FAILED, 3555 drawings added between the keys; ends "CONTRACT HOLDS"** (`out\s2_fix_contract.txt`).
  36 charts logged `missing=0`. Offline check of all 36 charts: 0 FAILED each.
- With `--checkout` the tool still ends "CONTRACT BROKEN" (`out\s2_fix_contract_checkout.txt`), only because
  `FTBrawlGameMode.cpp` differs from `b2e38c54` (now 191 added / 38 removed lines). The lines that end states were not
  changed in this round: `getup` 0.45 and `hurt` 0.26 got text appended, the numbers are the same. State lengths of
  non-looping states rest on that reading, not on a run. The 20 new Nova states are not in `sweep.txt`: not swept.
- Road: `C:\ftwork\jobs\p3\start_L0.txt` -> `FTTRACK level=0 frames=12717 route=dc9e032b solid=d2e759f4
  solidTriangles=986296`, equal to `track_baseline.txt`; both waitfors found (`out\s2_fix_start_L0`).
- Nothing outside `Source/FullThrottle/Brawl`, `Content/Brawl` and one new `Content/Python/brawl_s2_pixelanim.py` was
  touched. `M_PP_Cel` not touched. Three sprite materials (`M_BrawlSprite`, `M_BrawlEnemy`, `M_BrawlEnemyV`) had
  "Has Pixel Animation" switched on by that job; by itself it did not cure the smear (seen), FXAA did.

## 6. Performance

Night 1, wave 1, `jobs\perf_N1.txt`, 1920x1080 off screen, uncapped, one run each. Files: `perf_fix\N1\`.

| | Before stage 2 | After integration (integrator) | After fix round |
|---|---|---|---|
| FrameTime avg / p99 / max, whole capture (ms) | 5.40 / 7.36 / 13.30 | 8.43 / 19.53 / 23.38 | 7.53 / 19.34 / 23.30 |
| Frames over 16.7 ms | 0 | 151 | 144 |
| Slow start inside the capture | none | 2.84 s | 2.71 s |
| Steady part avg / p99 / max (ms) | (whole capture) | 5.05 / 6.65 / 7.38 | 4.53 / 6.10 / 15.88 |
| GPU avg (ms) | 4.05 | 4.72 | 4.02 |
| Draw calls avg | 186 | 241 | 243 |
| Sprite textures in memory | 1,068 MB in 777 | 4,670 MB in 3,396 | 4,544 MB in 3,305 |
| Sprite warm-up | 801 assets, 1.2 to 1.75 s | 3,420 assets, 2.8 to 2.9 s | 3,329 assets, 2.85 s |

- The slow first seconds of a night and the 4.5 GB are **not fixed**. No frame over 16.7 ms after the slow start.
- Nights 3 and 7 were not measured again.
- The hitch when a wave locks is unchanged (night 1: 119 to 135 ms at wave 1, 66 and 84 ms at waves 2 and 3).

## 7. Every finding

FIXED = changed and then run or looked at. PART = the removable part is done, the rest needs art. NOT FIXED = why.

### Player (Nova)

| # | Finding | Result |
|---|---|---|
| 1 | kick: leg out, in, out | FIXED: `s2_kick_02` removed (strip looked at) |
| 2 | idle: back foot slides | FIXED by removal: all 8 `s2_idle` drawings off; idle is back on its 3 keys. Needs regeneration to be smooth |
| 3 | punch:3 stance jumps | FIXED: `s2_punch_05/06/07` removed |
| 4 | drink: laugh flash, gulp jitter | PART: `s2_drink_10` removed; gulp is now 08/09 alternating at 67 ms (feet still 11 px apart) |
| 5 | carry_06 key-colour patch | FIXED by removal (`s2_carry_06` off the chart, texture deleted). Specks on `s2_jump_18` and `s2_stomp_01`: NOT FIXED, not looked at |
| 6 | heavy: narrow small figure | FIXED: `s2_heavy_07` removed, 08 starts at 216.7. `nweapon04` open hands: NOT FIXED |
| 7 | special: front, back, front | FIXED: `s2_special_07/08` removed (strip looked at) |
| 8 | runatk: leg length, arm | PART: `s2_runatk_07` removed. Leg length and the larger `_10`: NOT FIXED (regeneration) |
| 9 | slim A keys flash small | NOT FIXED: key art, the lead's decision |
| 10 | jump: legs pump | FIXED: `s2_jump_03/09/10/11/14` removed |
| 11 | walk: stride, arms never swap | NOT FIXED (regeneration) |
| 12 | getup_03 too upright | FIXED: removed |
| 13 | carry_04 foot | FIXED: removed |
| 14 | throwback_05 | FIXED: removed |
| 15 | pummel_03 | FIXED: removed. Held foe against the hands: not checked |
| 16 | dashatk has no shoulder pose | NOT FIXED (regeneration) |
| 17 | charging_04 fist | FIXED: removed from the loop |
| 18 | dive shakes | FIXED: one drawing held after the first two |
| 19 | small last drawings | FIXED by removal: `s2_wpn_08`, `s2_kick_09`, `s2_dashfwd_10` |
| 20 | wpndash_05, backatk_01 | FIXED: both removed (backatk now starts on `_02` at 0) |
| 21 | states still short | NOT FIXED (more drawings) |
| 22 | film check mismatches | NOT FIXED, not examined; the after films were not re-made |

### Street

| # | Finding | Result |
|---|---|---|
| 1 | Biker: grey and red model alternate | NOT FIXED: the keys themselves are the grey model; lead's decision, then regeneration |
| 2 | Biker getup: squat, lying, squat | PART: `s2_getup_02/03` removed, so it reads kneel, lying (the contract's key), squat, up |
| 3 | Guard colour pop | NOT FIXED: key art |
| 4 | Guard second baton | NOT FIXED: confirmed at 1:1 (`fix\s\z1.png`), but the `wind` key itself shows it; removing the in-betweens would not cure it |
| 5 | Guard cap | NOT FIXED: key art |
| 6 | all six: `s*` keys larger and softer | NOT FIXED: key art, lead's decision |
| 7 | Sleaze bottle | PART: `s2_throw_10` removed. The keys without the bottle: NOT FIXED |
| 8 | Dealer idle cards | PART: the one-card drawings `s2_idle_02/05` removed (seen at 1:1). `sidle01/02` without cards are keys: NOT FIXED |
| 9 | Dealer throw / wind stance snap | FIXED: the three `s2_throw_02` rows removed. Glasses on the `throw`/`punch` keys: NOT FIXED |
| 10 | Dealer getup at attention | FIXED: `s2_getup_08` removed. Floor cards on the `down` key: NOT FIXED |
| 11 | Dealer fly thrash | FIXED: `s2_fly_02/05` off, one arc 04, 01, key, 01, 03, 06. Visor on the keys: NOT FIXED |
| 12 | Bouncer fly head flip | FIXED: second `hurtrecover` row and the early `s2_fly_03` removed. Only 6 drawings: NOT FIXED |
| 13 | walk1 held 88 to 91 ms | NOT FIXED (new drawings) |
| 14 | Punk walk shuffle | NOT FIXED (regeneration) |
| 15 | Punk wind head back, no retraction | PART: `s2_wind_05` removed |
| 16 | Sleaze, Guard fly repeats | NOT FIXED |
| 17 | Biker hold_03, wind holds | NOT FIXED |
| 18 | few drawings / long holds | NOT FIXED |
| 19 | get-up order kneel, lying, up | NOT FIXED: the contract's order |
| 20 | Bouncer swing bob, earring | NOT FIXED |
| 21 | 10 ms rows in the windows | NOT FIXED (still listed as notes by the checker) |
| - | Biker getup film blurred at the end | Not examined separately; the smear cure (section 1) was not re-filmed for this state |

### Bosses

| # | Finding | Result |
|---|---|---|
| 1 | Commodore sweep size steps | NOT FIXED: measured; the new drawings are at the size of the `sweep` key, the two bridge keys are 1.7 times its area. Key art |
| 2 | Commodore windup | PART: `s2_windup_05` and the `s2_windup_19` row after `bsigreach` removed. Soft 07 to 13: NOT FIXED |
| 3 | all seven: bridge keys bulky and soft | NOT FIXED: key art, lead's decision |
| 4, 5 | Frankie torch changes side | NOT FIXED: the `swig` and `bkneel` keys carry it on the other side |
| 6 | Kingpin fly upside down | FIXED by removal: `s2_fly_02/03` off; fly ends on `s2_fly_01` held |
| 7 | Earl getup / defeat small slam drawings | FIXED by removal: `s2_slam_10/11/12` off those two states (kneel held 150 ms in getup) |
| 8 | Sven key pictures slim | NOT FIXED: key art |
| 9 | Earl slamwind jitter | FIXED: `s2_slamwind_04/06/14` removed |
| 10 | Commodore cap in the air | FIXED: `s2_hurt_01` removed from hurt and fly |
| 11 | Vinnie fly order | FIXED by removal: `s2_fly_02/03` off |
| 12 | Commodore wind small drawing | FIXED: `s2_wind_04` removed |
| 13 to 16 | walks and fly on few drawings | NOT FIXED |
| 17 | Earl dead rows (3.6 ms) | FIXED: moved to 50 ms in swing, swing:1 and hurt |
| 18 | Earl hurt / punch keys small | NOT FIXED: key art |
| 19 | wind-up flicker | PART: Kingpin `s2_wind_05` and Vinnie `s2_wind_05` removed; Vinnie's 117 ms hold stays |
| 20 | Sven, Pitboss softer | NOT FIXED (re-cut not done) |
| 21 | keys that contradict the move | NOT FIXED: key art |
| 22, 23 | intros, defeat settle | NOT FIXED |

### Regulars A

| # | Finding | Result |
|---|---|---|
| 1 | Tourist throw | PART: the release key at 0 now shows for one step (17 ms) instead of 33; the look-out drawings removed. The two empty-hand recover drawings (`s2_throw_20`, `_12`) still end the state: NOT FIXED. The flash itself needs a contract exception (lead) |
| 2 | Barman throw | PART: the same one-step change. The bottle drawn in the key: NOT FIXED |
| 3 | Pitcook wind | FIXED: `s2_wind_02/08` removed |
| 4 | Roller dash | FIXED by removal: `s2_dash_04/05/07` |
| 5 | Firedancer walk | PART: the march kicks `s2_walk_09/18` removed. Squat drawings after the keys: NOT FIXED |
| 6 | Firedancer wind | PART: early punch `s2_wind_07` removed. Mouth: NOT FIXED |
| 7 | Surfer grab | FIXED by removal: `s2_grab_03/05`. Closed fists against the open-hand key: NOT FIXED |
| 8 | Hustler grab yo-yo | FIXED: `s2_grab_05/08` removed |
| 9 | Tourist fly on 1 drawing | NOT FIXED (generation) |
| 10 | Disco fly | FIXED: order 01, 02, 03, 04, 05, 08 once at 67 ms; 06/07 off |
| 11 to 14 | Muscle | NOT FIXED |
| 15 | Surfer wind | FIXED: `s2_wind_02/06` removed |
| 16 | Pitcook swing | FIXED: `s2_swing_05` removed |
| 17 to 21, 23, 24 | Pitcook down, Tourist swing, Roller, Hustler, Barman, Firedancer dash, idle breath | NOT FIXED |
| 22 | Disco wind early punch | PART: `s2_wind_08` removed; the rest NOT FIXED |

### Regulars B

| # | Finding | Result |
|---|---|---|
| 1 | drawings smeared in the game | FIXED: FXAA in the fight (section 1). Seen on the Waiter throw film and in night 1; Lanehog and the other characters not looked at one by one |
| 2 | Lanehog getup_06 | FIXED: removed (strip looked at) |
| 3 | Deckhand walk tall drawings | FIXED: `s2_walk_08/11` removed |
| 4 | Waiter airborne bottle | NOT FIXED (regeneration) |
| 5 | Waiter / Shoeclerk throw prop | NOT FIXED |
| 6 | measured tall drawings | PART: Skibum `s2_getup_10/11` removed (looked at). Skibum walk_14, Goalie getup_09, Linemate, Pinsetter walk_09: not looked at, left |
| 7 | deep crouch for one step | FIXED: Tuxbrute `s2_wind_03`, Pinsetter `s2_wind_02`, Deckhand `s2_dash_02` removed |
| 8 | wind-up runs backwards | NOT FIXED (looked at Deckhand: the last four are near-identical guards; reordering not tried) |
| 9 | walk bob | NOT FIXED |
| 10 | fly | PART: Tuxbrute fly now hurt, `s2_hurt_01`, `s2_fly_09` (the eight slim drawings measured at two thirds of his area and were removed); Dock `s2_fly_01/02` off. Leaguer, Lanehog: NOT FIXED |
| 11, 12 | softness, holds | NOT FIXED |
| 13 | Tuxbrute down bounce | FIXED: `s2_down_01` removed |

### Review by play

| # | Finding | Result |
|---|---|---|
| 1 | scenery in front of the fight | FIXED for the pier post (no post within 260 cm of a locked screen; night 1 wave 2 looked at). The tiki totem of night 4: not looked at |
| 2 | stamp never comes out | FIXED in the fixture; 0 uses in the seven autopilot nights |
| 3 | bosses chain-floored | FIXED as far as run: every boss with an encore showed one, the Kingpin struck his lanes (section 4). Fairness to a person: not checked |
| 4 | boss hides Nova in the beat | FIXED: Nova slides to 180 px (looked at on Earl and the Kingpin) |
| 5 | hitch when a wave locks | NOT FIXED |
| 6 | help panel over heads | FIXED: bottom strip (looked at) |
| 7 | big foes hide Nova | PART: Nova is drawn in front of a foe within 3 px of his lane. A foe on a nearer lane still covers him. Not looked at |
| 8 | hit flash | FIXED in code; not compared by eye |
| 9 | block looks like a hit | PART: the same weaker flash applies; no separate rim |
| 10 | rage outline in the room's colour | PART: "AGAIN!" is white. Outline colour: NOT FIXED |
| 11, 12 | rage grade, floors | NOT FIXED |
| 13 | onlookers too bright | FIXED: tint 0.7 to 0.5 (looked at night 1). Back views: NOT FIXED (art) |
| 14 | Kingpin encore lanes | FIXED (log: 360,428,496 then 394,462,530) |
| 15 | AIR counters pile up, juggle on the beaten | FIXED in code; not looked at |
| 16 | carried foe is a white bar | NOT FIXED (art) |

### Code and rules

| # | Finding | Result |
|---|---|---|
| 1 | +3.6 GB texture memory, slow start | NOT FIXED: the warm-up is in `FTGameInstance.cpp`, outside the brawl. 91 unused textures deleted (126 MB less). Lead's decision |
| 2 | account name reached the image service | PART: `gen\gen_sheet.ps1` no longer asks the model to copy the file and forbids shell commands and paths; it honours `CODEX_HOME`. The script parses. **No generation was run after the change, and the login was not moved to a neutral folder.** Old copy: `fix\gen_sheet.before.ps1` |
| 3 | contract tool ends BROKEN with `--checkout`; state lengths | PART: (a) the boss's hurt is no longer cut short at phase 2 (he waits for it to end); `TIMING_AFTER.md` log path corrected and marked as older than this round. (b) down restarting on the first stamp, (c) fly reset by a juggle, (d) punch/kick cut by the special, (e) carry lengthened by pummels: unchanged, for the lead to accept or reject |
| 4 | mashing fires the special | FIXED in code (both buttons held for two steps); not run |
| 5 | presses during the beat | FIXED in code; not run by hand |
| 6 | stage dice depend on frame rate | FIXED: onlookers and chips use their own stream |
| 7 | stale encore flag | FIXED in code |
| 8 | weapon breaks mid-swing | FIXED in code; not looked at |
| 9 | 170 frames moved after the cutter | NOT FIXED: lead's decision |
| 10 | Jug / JugAt two purposes | FIXED: `StompAt`; no juggle on a beaten foe |
| 11 | unchecked spawns, guessed volume | PART: three spawns checked. The post-process volume is still the first unbound one |
| 12 | hit-stop in the night's clock, repo size, absolute paths | NOT FIXED |
| 13 | holds over 1/24 s | NOT FIXED (now 180 of 399; was 205) |

## 8. What is still needed, in order of weight

1. Key art on model, under the same names and times (lead's decision): Nova's kind A keys; the street `s*` keys and the
   Biker / Guard / Sleaze / Dealer keys named above; the bosses' "b" bridge keys, Sven's three, Frankie's torch side.
2. The sprite warm-up (memory and the first seconds of a night).
3. Regeneration of the drawings taken out in this round, and of the states the reviewers list as still short.

## 9. Not checked

- Nothing was watched in motion: stills, strips and logs only. Nothing was listened to.
- The after films and `compare\` strips were not re-made; `TIMING_AFTER.md` was not regenerated.
- Each removed row was confirmed by measurement; only these were also looked at: Nova kick and special; Biker getup,
  Guard wind, Dealer fly, getup and idle; Tourist throw, Roller dash, Pitcook wind, Disco fly, Lanehog getup, Skibum
  getup, Tuxbrute fly, Deckhand wind. The charts after the edits were not looked at as strips.
- Nova's weapon grip on the changed rows; the held foe in pummel; the 20 new Nova states in a sweep.
- The mash special, presses in the beat, the weapon breaking after its swing, the AIR counter, Nova drawn in front,
  the block flash: changed in code, compiled, not exercised by a fixture.
- FXAA at other resolutions and quality settings, and whether the options screen can still change anti-aliasing while
  in a fight (the fight re-asserts FXAA every frame). The cel post-process under FXAA was seen only in the pictures above.
- Performance of nights 3 and 7; a cooked build; more than one run per case.
- A person playing: fairness of the boss changes, the feel of the stamp's step, the dimmer onlookers.
- `music restarted` is logged every frame while a film is recorded (8,051 lines in the reviewer's night 1 log). Seen,
  not investigated, not caused by this round.
- Night 2: the script's wait for the last-blow beat timed out (the night was won); cause not looked for.
