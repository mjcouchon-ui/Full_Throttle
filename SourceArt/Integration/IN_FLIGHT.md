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
| Frame-time and load-time measurements | none | left to the 3.4 performance loop; findings for it are in `AUDIT.md` |

Nothing of this session's is in flight at the time of the last push. The next things it would take up are listed
in `WORLD_PASS_2.md` ("Still weak" and "What the workers asked of the shared files").

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
