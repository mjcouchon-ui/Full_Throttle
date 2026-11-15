# Work in flight on main: the world and polish pass

Two sessions push to `main` since 15 November (Justin's instruction to both). This file says what the world-and-polish
session (checkout `K:\Full Throttle\UE`, and its workers in `K:\Full Throttle\VisualAssetsWorktree`) is changing, so
the other session, and Matt, Brad and Keenan, can see it before touching the same files. It is updated with each push
from that session; a line goes when its work has landed.

How that session lands work: short-lived `claude/*` branches, compiled and run, then an ordinary push to `main`
(never forced; `origin/main` is merged first if it moved). It never writes in `K:\Full Throttle\MainWorktree`.
Please do not snapshot or merge a `claude/*` branch or those two checkouts' uncommitted files: what is ready is on
`main`, and what is not would break the build half-written.

| Work | Files | State |
|---|---|---|
| Audit fixes (`AUDIT.md`, ten items; fifteen harness checks pass) | `FTGameMode.*`, `FTProgress.cpp`, `FTGameInstance.cpp`, `FTPlayerController.*`, `FTHUD.cpp` | on `main` |
| Dressing shadows: a piece casts only where its shadow reaches the road | `FTTrack.cpp`, the dressing's `Put` only | on `main` |
| Attack presentation: the telegraph fixes from the review | `FTEntity.cpp`, drawing code in `FTGameMode.cpp` | on `main` |
| The land's own surface, level by level | `FTTrack.cpp` (which material the land wears), `Content/Python/ft_world.py`, `Content/Maps/*/Materials/MI_Land`, `Content/Maps/Land` | on `main` |
| Dressing, second pass, all eight levels (`WORLD_PASS_2.md`) | `FTDressL0.inl` to `FTDressL7.inl`, `Content/Python/ft_dress_*.py`, `Content/Maps/*/Dress`, new instances in `Content/Core/Life/Instances` | on `main` |
| The per-level release check and the packaged build (`AUDIT.md`) | `SourceArt/Jam/unreal/jam_verify.ps1`, `jam_package.ps1`; one log line in `FTTrack.cpp` (`FTTRACKSUM`) | on `main` |
| The cut banks inside banked bends: a Bank rule for the dressing, and faces on five levels (`WORLD.md`, "The cut banks") | `FTDress.h`, `FTTrackDress.cpp`, `FTTrack.h`, `FTTrack.cpp` (the dressing's part of `BuildProps`; in `BuildMeshes` two lines that only remember the bed's line), `FTDressL1/2/3/5/6.inl`, `Content/Python/ft_dress_bank.py` and five kits, `Content/Maps/*/Dress/SM_Cut*` | on `main` |
| The cut banks' lead-ins refitted (a panel stood a corner out of the ground where a bend's bank comes on); the dressing tables read one function per level | `FTTrack.cpp` (the Bank rule in `BuildProps` only), `FTDress.h`, `FTTrackDress.cpp` | on `main` |
| Tools: a job queue that serves the longest-waiting checkout, several jobs in one turn, a runner for staged scenes; the test runner's `film` line | `SourceArt/Jam/unreal/jam_global.ps1`, `jam_turn.ps1`, `jam_exec.ps1`, `jam_verify.ps1`; `FTGameInstance.cpp` (`TickExec` only) | on `main` |
| **The overhaul (the owner's brief of 15 Nov), zones**: every level's third dressing pass (denser districts, moving pieces, landmarks, faced cuttings) | `FTDressL0.inl` to `FTDressL7.inl`, `Content/Python/ft_dress_*.py`, `Content/Maps/*/Dress`, `Content/Core/Life/Instances`, palette instances | on `main`; a further round (the built world in motion: traffic, trains, boats, machines, light) is in flight in the same files |
| **The overhaul, the population layer**: people, animals and film crews on all eight levels, placed by rule after each level's dressing | `FTPeopleL0.inl` to `FTPeopleL7.inl` (read by `FTTrackDress.cpp`, which gained eight small functions), `Content/Python/ft_people*.py`, `Content/People/**`, `SourceArt/Integration/people/**` | the empty tables and their reader are on `main`; a second machine (another session of Justin's) fills them and pushes its own batches: brief in `SourceArt/Integration/POPULATION_BRIEF.md` |
| **The overhaul, menus and HUD: a real visual style** ("the studio lot, inked": production paperwork drawn in the world's ink-and-cel hand; same information, places, order and inputs on every screen) | `FTHUD.cpp` and `FTHUD.h`: the file will be SPLIT BY SCREEN into several `FTHUD*.cpp` of the same class and every draw call restyled; new `Content/UI/Kit/**`, `Content/UI/Fonts/F_Bungee`, `Tools/ui/**`, `SourceArt/UI/Kit/**` | in flight (kit, mock-ups and code plan being made on 15 Nov evening). **Until it lands, please keep changes to `FTHUD.cpp` on `main` small and say in the commit message what they do**: each one is carried into the split files by hand |
| **The overhaul, racing bosses, combat effects, sound and transitions** | `FTEntity.*`, the boss and effect code in `FTGameMode.cpp`, `FTFx.*`, `Content/Python/ft_fx*.py`, `ft_v*.py`, `ft_enemy_vehicles.py`, `Content/FX/**`, the sound generator and `Content/Audio/**` | in flight; lands stream by stream as each compiles and has been looked at in the real game |
| Frame-time and load-time measurements | none | left to the 3.4 performance loop; findings for it are in `AUDIT.md` |

In flight at the time of the last push: the three overhaul rows marked so above (checking is light for this pass, by the owner's word of 15 Nov: it compiles, it comes up in the real game, the frames were looked at, the track's `FTTRACK` line is unchanged; full review, optimisation and validation follow on other machines). Until it lands, a change on `main` to `FTHUD.cpp`,
`FTFlow.cpp`, `FTEntity.cpp` or to the boss and effect code in `FTGameMode.cpp` is merged with it by hand by this
session (the session menus of `d999efe` are being merged that way, kept whole): if something in such a change must
not be touched, say so in the commit message. The next things after it are listed in `WORLD_PASS_2.md` ("Still
weak" and "What the workers asked of the shared files").

The job queue (`jam_global.ps1`) changed on 15 Nov: a checkout that has merged this push queues fairly with the
others; a checkout still on the older copy takes a free slot as it finds it, ahead of jobs that have waited longer.

Not this session's: the track and its collision (locked), gameplay values, the skill tree, missions, the Canvas HUD's
layout, vehicle rigs and wheels, `M_PP_Cel`.

Changed here that belongs to somebody else's lane, for their review:

- **Matt**: `FTGameMode.cpp`, boss spawn. 3.3.0 put a comment in the middle of the line that set the bosses' first
  attack timers, so `E.T1 = 1.6f; E.T2 = 1.0f; E.T3 = 6.f;` became part of the comment and every boss fired the moment
  it appeared. The three statements are back on their own line, as in 3.2. If the instant attack was wanted, delete
  that line.
- **Matt**: `FTTrack.cpp`, `BuildMeshes`. The cut banks' rule needs to know where the bed's cutting is, so the bed's
  ring loop now also stores, per frame, the two points its last straight runs start from (`BedCut`, `BedInL`,
  `BedInR`: a new array and two numbers in `FTTrack.h`). The bed is drawn exactly as before and nothing of the track
  reads them: `FTTRACK` is equal to `track_baseline.txt` on all eight levels with the panels on and off. If the bed's
  shape ever changes, the panels follow it by themselves.
- **Keenan**: the level's pay is now banked when the level is won, before the highlight reel (`RateAndCommit`), and a
  reshoot or a retake resets the multi-kill clock, the near-miss clock and the combo.
- **Brad**: pickup popups say what was gained ("+1 MISSILE", "+12 ARMOR", "MISSILES FULL", "ARMOR FULL"); the title
  shows SAVE RESTORED FROM BACKUP, SAVE DAMAGED: NEW CAREER or NEWER SAVE: NOT SAVING when a career could not be read.
