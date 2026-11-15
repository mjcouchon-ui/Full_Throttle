# Staged validation scripts

Scripts for the game's own test runner (`-ftexec=<file>`, compiled out of Shipping builds). Each one stages a scene
and takes named frames, so the same script run on two builds gives frames that pair up: a before and an after.
Written and cross-checked on 15 November 2026 for the world, boss and gameplay polish overhaul; `boss_L1.txt` and
`boss_L2.txt` ran clean on that day's untouched build (36 and 47 frames, no timeouts).

| File | What it stages |
|---|---|
| `boss_L1.txt` ... `boss_L8.txt` | One racing boss each (game levels 1 to 8): the road before him, his entrance, every attack's wind-up, release and landing, a hurt reaction, each damage phase, the defeat, the reel, dailies and paycheck. 35 to 47 frames, taken with the world frozen (`FTFreeze`), so two runs match frame for frame |
| `combat_dense.txt` | The busiest fight the harness can stage, on City Center: guns, missiles, every vehicle kind, the boss called over a live convoy, everything wrecked at once, the aftermath |
| `combat_dense_bench.txt` | The same scene for the game's benchmark: run it with `-ftbench -ftlevel=7 -ftquality=<0..3> -ftseconds=10 -fttag=_dense` on the command line; the result is the log line `FTBENCH {...}` and a JSON file |
| `combat_cleanup.txt` | Missiles that hit and miss, wrecks, the wrap, a retake: frames of the fresh scene afterwards, to show nothing is left burning |
| `menus_walk.txt` | Every screen: title, pre-production, skill tree, options, controls, level card, HUD, pause, restart, CUT!, the reel, dailies, paycheck, with a short burst across each entrance |
| `film_test.txt` | The runner's `film <name> <frames> [fps]` line: two seconds of Bayshore as 60 frames |
| `brawl_bosses.ps1` | A runner for the seven Brawl bosses (the nights). It assumes the lead's PC layout: read it before use |
| `track_baseline.txt` | The eight tracks' fingerprints: the log line `FTTRACK level=N frames= route= solid= solidTriangles=` of every level must equal its line here (the track is Matt's and is not to change) |
| `notes/` | For every script: what its checker found and fixed, how to run it, what only a real run can settle, and the facts from the code it rests on (timers, armour, hit boxes). `harness_reference.txt` lists every script line type, every `FT*` command and the log lines to wait for |

## Running one

On the lead's PC: `SourceArt/Jam/unreal/jam_exec.ps1` (it takes the Unreal job queue; several in one turn with
`jam_turn.ps1`). Anywhere else: `run_exec.ps1` in this folder, which needs only the engine and the project:

```
powershell -NoProfile -ExecutionPolicy Bypass -File run_exec.ps1 -Engine "<Unreal 5.9>" -Project "<project folder>" -Script boss_L3.txt -Out "<frames folder>" -Extra " -ftnoattract -ftnodynres -ftslot=pie -ExecCmds=DisableAllScreenMessages"
```

Each script's first lines and its file in `notes/` give its own extra switches and budget. Use the same frame size
and the same switches for the two runs of a pair. `-ExecCmds=DisableAllScreenMessages` keeps the engine's yellow
on-screen warnings out of the frames (a virtual-shadow-map queue warning is printed on most levels, before and after
this overhaul alike; Shipping builds never show such text).

Script waits are in real seconds and the game runs on game time: the stills hang off `FT CLOCK` log lines, so they
hold as long as the game makes about 20 frames a second. Run one game at a time when timing matters.
