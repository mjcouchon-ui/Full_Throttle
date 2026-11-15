# FULL THROTTLE: the population pass (brief for the second machine, 15 Nov 2026)

You are a fresh Claude Code session on a second PC. You lead one stream of tonight's visual overhaul of the game
and you may run workers of your own. Another session (the lead, on Justin's main PC) runs the other streams. The
two of you never touch the same files; git is the only line between you.

**The owner's words (Justin, who owns the game's look):** "we're trying to polish and dress the world, characters,
HUD and UI, in batches as much as possible - holistic review, optimization, and validation will happen later on
other machines. The game needs to have AAA-level quality and depth. The gameworld should feel alive, dense, and
fully populated and animated."

**Your mission: populate the world.** Eight levels were made dense with buildings, machines and scenery this week,
and almost nobody lives in them. On Bayshore, a seaside boardwalk city, there is not one figure. Put people,
animals and film crews into all eight levels: doing things, in motion, at every distance from the road, in a
style that belongs to this game. Work in batches and push each one to `main` as soon as it stands.

This file is in the repository at `full-throttle-unreal/SourceArt/Integration/POPULATION_BRIEF.md`. The lead may
change it: after every `git fetch`, see whether it has a new commit and read it again if so.

## The game, in ten lines

- **FULL THROTTLE**: an Unreal Engine **5.9** arcade chase game, '80s action-movie energy. In the fiction every
  level is a film shoot: a level ends with a "reel", a rating and a paycheck. Repository
  `mjcouchon-ui/Full_Throttle_Unreal`; the racing game is the project folder `full-throttle-unreal` (C++ module
  `FullThrottle`). Paths below are relative to that folder.
- Eight levels, about 25 km of road each, driven at 120 mph and more behind a chase camera. Index, name, boss:
  0 Bayshore (JUGGERNAUT), 1 Chemical Works (ALCHEMIST), 2 Nevada (BIG DEALER), 3 Volcano (ENGINE 13),
  4 Theme Park (MR. CHUCKLES), 5 Ice Mountain (ABOMINABLE), 6 Under the Sea (GREAT WHITE), 7 City Center (CHAIRMAN).
- **Look**: semi-cel shaded (a shared post-process bands and inks everything), chunky readable shapes, pastel
  colour with selective hot neon. `WORLD.md` and `BUILDINGS.md` describe the world; read them first.
- **All art is made by code**: editor Python generators in `Content/Python` draw every prop from primitives with
  `ft_art.Model` (boxes, cylinders, spheres, extruded outlines, a palette colour each) and save it as a static
  mesh. No downloaded assets, no image generators, no outside libraries. That is the house style and the rule.
- **Scenery is placed by rule, not by hand**: a table per level (`Source/FullThrottle/FTDressL<n>.inl`, compiled
  into the game) says which mesh stands where along the road. The words of a table (`Set`, `Zone`, `Scatter`,
  `Site`, `Far`, `Afloat`, `Lit`) are explained at the top of `Source/FullThrottle/FTTrackDress.cpp`; what the kinds
  of rule do is in `Source/FullThrottle/FTDress.h`. Placed pieces are instanced meshes without collision.
- **Scenery moves through its material**: `Content/Python/ft_life.py` makes the master material `M_Life`, with
  ways of moving a mesh in the vertex shader (Sway, Flutter, Spin and rock, Bob, Breathe, Light, and for meshes
  built with `ft_life.Kit`: Plume, Conveyor (parts that travel along a line and come round again), Burst). A box
  in the mesh's own space (`MaskMin`/`MaskMax`) keeps a movement to part of a mesh. Its header explains all of it.

## What is yours, and what is not

**Yours (nobody else writes these):**

| What | Where |
|---|---|
| The population tables, one a level | `Source/FullThrottle/FTPeopleL0.inl` ... `FTPeopleL7.inl` |
| Your generators | `Content/Python/ft_people*.py` (any number of files with that prefix) |
| Your meshes and materials | `Content/People/**` (`/Game/People/...` in the editor) |
| Your notes, contact sheets and status | `SourceArt/Integration/people/**` |

The eight `FTPeopleL<n>.inl` files exist and are empty. Each is read straight after the level's dressing table, by
a function of its own, with the same words. Start a file with a `Zone` line. A mesh name without a leading slash
is looked up in that level's dressing folder, so name your meshes by full path:
`Scatter(TEXT("/Game/People/Bayshore/SM_Strollers_A"), 17.5f, 21.f, 2, 0, 1.f);`. A mesh that does not exist is
skipped silently, so check the count line (below) after every change. Because your rules come after every
dressing rule, nothing you add moves a piece the dressing placed.

**Not yours. Read them, call them, never edit them:** every other file. In particular the track
(`FTTrack*.cpp`, the level data: Matt owns the track's design and it is locked), the dressing tables and kits
(`FTDressL<n>.inl`, `ft_dress*.py`: the lead's workers are in them now), `ft_life.py` and `M_Life`, `ft_art.py`,
`ft_export.py`, the HUD (`FTHUD.*`), the game's rules and entities, and the shared cel post-process `M_PP_Cel`
(never, for any reason). You may import and call `ft_art`, `ft_export`, `ft_life` (`life_mi`, `animate`, `Kit`,
`Group`) from your own generators. If `M_Life` cannot do a movement you need (a walk, a wave, a wing beat), make
a master of your own (`M_People`, built by your generator, an ordinary lit material so that the cel pass inks it
like everything else), do not change `ft_life.py`. If you need a new table word or anything from the track's
code, do not write it: ask (see "Talking to the lead").

**Who does what, so nothing is made twice.** The lead's workers do the built world: buildings, signs, machines,
vehicles and traffic, boats, aircraft, weather, light. You do the living: people, animals, and the film crews.
Living things that already exist stay as they are and you do not copy them: the Theme Park's guests
(`crowd` and `_guest` in `ft_dress_park.py`: read them, your figures must sit beside them without a seam),
Under the Sea's fish schools, rays, sharks and whales, Nevada's buzzards.

## What "populated" means here

Aim at what a player feels in the first minute of a level: this place is inhabited, and it is watching the chase.

1. **A figure that belongs to this game.** One figure builder, used everywhere: chunky, big head and hands,
   strong silhouette, flat palette colours, 120 to 300 triangles, readable at 120 mph from 20 to 150 m. Many
   bodies from one builder: build, height, age, clothes, hair, hats, props in hand. Make it first, look at it up
   close and from the road, and get it right before you make a hundred pieces with it.
2. **People do things.** Not rows of standing figures: they queue, dance, film, fish, repair, sunbathe, ski, weld,
   sell, carry, point at the chase, run for cover, cheer. A piece is a small scene of several figures (one mesh,
   so one instance), not one figure.
3. **They move.** Every piece moves in some way: a crowd bobs person after person, arms wave (a rocking Spin
   masked to the arm), walkers and joggers travel along a boardwalk and come round again (Conveyor), skiers come
   down a slope, divers swim, birds wheel and flap, flags and scarves flutter. Slow and readable beats fast.
4. **Every distance.** By the barrier (17 to 30 m from the centre line): low scenes behind a fence, cheering,
   filming, fleeing. Middle distance (30 to 150 m): scenes round the places the dressing built (a pier, a
   roadhouse, a ski lodge, a plaza, a loading dock), life on roofs and balconies. Far (150 m to the horizon):
   flocks, herds, a crowd on a grandstand, lines of skiers, a parade, shoals.
5. **Each level its own cast.** Starting points, yours to better:
   Bayshore: sunbathers, volleyball, surfers, skaters, strollers and shoppers on the boardwalk, anglers on the
   piers, gulls, dogs, a lifeguard. Chemical Works: crews in hard hats and suits, guards, people on gantries,
   crows. Nevada: bikers at a roadhouse, hitch-hikers, prospectors, gamblers and valets on the strip, wild horses,
   cattle, coyotes, lizards. Volcano: fire crews, scientists with instruments, villagers leaving, goats, birds
   fleeing the ash. Theme Park: staff, mascots, performers, a parade, queues (beside the guests already there).
   Ice Mountain: skiers, snowboarders, climbers on ropes, a crowd at the lodges, snowmen, goats, wolves, eagles.
   Under the Sea: divers, aquanauts of the city of Nereida at work, turtles, an octopus, dolphins, crabs,
   jellyfish, seahorses. City Center: commuters, vendors, police lines, crowds behind barricades with banners,
   window cleaners, roof-top parties, pigeons.
6. **The film crew, on every level.** The level is a shoot: camera cranes and dollies, a director's chair and its
   cluster, lighting towers, a clapper loader, sound booms, a stunt team with crash mats, crew applauding,
   catering. Put them where a shoot would have them: by the start, at jumps and big bends, before the boss's
   arena. One shared kit (`/Game/People/Crew/`), dressed for each level's weather.
7. **Depth.** No visible repeats: several variants of every scene, mixed by rule, and a district's people change
   with the district. Small stories for the player who looks twice.

## Rules that protect play (these are not negotiable)

- **Looks only.** No collision, no game rule, no handling, no timing, no track change.
- **The sight rule.** Distances are metres from the road's centre line; the barrier's outer face is at 14.1 m.
  Nothing at all nearer than **17 m**. Anything taller than **2 m** stays at **24 m** or more. Nothing may hide
  the road ahead, a pickup, an enemy or an attack warning.
- **People are never in harm's way.** Never on the road, never between the barriers, never on a jump's run-up or
  landing. By the road they stand behind a fence, on a stand, a balcony, a boardwalk or a bank: a player must
  never feel that they could hit someone. Nothing reacts to being hit; no injury, no blood.
- **Nothing that can be mistaken** for a pickup, an enemy, a boss or an attack (read `POWERUPS.md` for what
  pickups look like). Nothing that looks like a level's boss within 40 m of the road.
- **Light.** A steady light stays 24 m or more from the centre line; anything that blinks, chases or pulses 30 m
  or more; nothing flickers by more than a fifth of its brightness or faster than three times a second. No strobing.
- **Original and clean.** No real person's likeness, no real brand, logo or team, no outside asset or texture.
  A cast of every kind of person, drawn with affection; nothing demeaning, nothing sexualised.
- **Budgets (guides; say where you went over and why).** A piece under about 1,500 triangles (a single great
  animal up to 4,000). At most about 900 placed population pieces a level and 16 of your meshes a level, plus the
  shared kits. Movement by the vertex shader only (no ticking actors, no skeletal meshes: there may be hundreds).
  The track switches vertex movement off past 180 m for roadside pieces: a far piece that must keep moving takes
  the always-moving master (`far=True` in `ft_life`). Every file under 50 MB (the repository has no LFS).
- Record in `SourceArt/Integration/people/STATUS.md` how every asset was made (which generator, which function).

## Your machine: the first ten minutes

1. Find Unreal Engine **5.9** (the Epic launcher's installed build is what the main PC uses: a folder with
   `Engine\Binaries\Win64\UnrealEditor.exe`, `UnrealEditor-Cmd.exe` and `Engine\Build\BatchFiles\Build.bat`) and a
   Visual Studio C++ toolchain. Note the CPU, RAM, GPU and free disk (you need about 40 GB).
   **If there is no Unreal 5.9 or no compiler, stop and tell Justin at once.**
2. Clone the repository, or `git fetch origin` in an existing clone. Start your branch from the lead's
   integration branch, which has the empty tables and the reader for them:
   `git switch -c claude/people origin/claude/overhaul-integration`. (If `origin/main` already has
   `full-throttle-unreal/Source/FullThrottle/FTPeopleL0.inl`, start from `origin/main` instead.)
3. Build the editor module, then start the editor once so that it compiles its shaders (the first start on a new
   machine can take a long time: start both now and read the code meanwhile):
   `"<UE>\Engine\Build\BatchFiles\Build.bat" FullThrottleEditor Win64 Development -project="<clone>\full-throttle-unreal\FullThrottle.uproject" -waitmutex`

**Running a generator** (the editor, without a window, runs one Python file and quits):
`"<UE>\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" "<project>\FullThrottle.uproject" -ExecutePythonScript="<file.py, forward slashes>" -RenderOffScreen -unattended -nopause -nosplash -NoSound -stdout -FullStdOutLogOutput -abslog="<log file>"`
Write a small job file that imports your generator and calls it; read the log for your own log lines and for
`Error`. `Content/Python/ft_dress.py` (how a kit is exported) and `ft_dress_park.py` (figures) are the models.

**Seeing it in the real game**: `SourceArt/Jam/validate/run_exec.ps1` runs one of the game's own test scripts on
any machine (its header says how; `SourceArt/Jam/validate/README.md` and `notes/harness_reference.txt` list every
command). A drive of level n (1 to 8) with frames at five moments:

```
FTUseTestSlot
FTCareerReset
FTQuality 3
wait 6
FTStartLevel 1
waitfor FT LEVEL id=|240
FTAutoRun wreck=all
waitfor FT CLOCK level=8.0|150
shot L1_t008
waitfor FT CLOCK level=20.0|150
shot L1_t020
waitfor FT CLOCK level=35.0|150
shot L1_t035
waitfor FT CLOCK level=50.0|150
film L1_move 24 12
waitfor FT CLOCK level=80.0|150
shot L1_t080
quit
```

`film <name> <frames> [fps]` saves a frame sequence at a fixed step of game time: that is how you see whether a
thing moves, and how. Add `-Extra " -ExecCmds=DisableAllScreenMessages"`. In the game's log, the line
`FTDRESS level=<index> ... pieces placed=<n> of <m> tried` counts the dressing and your pieces together: it must
rise by what you meant to add. The `jam_*.ps1` tools in `SourceArt/Jam/unreal` assume the main PC's folders: read
them for working command lines, do not run them as they are.

**Never two Unreal processes on one checkout at a time.** For workers in parallel, give each its own
`git worktree`, and size the fleet to the machine (an editor job or a game takes 6 to 10 GB of memory; a compile
takes the processors). While `Build.bat` runs, every editor or game start on the PC waits for it.

## How to work

- **Use workers and check their work.** Build the figure builder and the film-crew kit first, alone, and look
  hard at them. Then fan out: one worker to a pair of levels, each in its own worktree, each owning its levels'
  `FTPeopleL<n>.inl`, `ft_people_<level>.py` and `Content/People/<Level>/`. Have a different agent, who did not
  make the work, look at every batch's frames against this brief before it is pushed.
- **Look at frames.** A piece is done when you have read frames of it in the real game, from the road, at speed,
  and it reads: you can tell what the people are and what they are doing, they move, and nothing breaks a rule
  above. A frame you have not looked at is not done. Stills cannot show movement: use `film`.
- **Checking is light, by the owner's order**: it compiles, the level comes up and plays to the times you shot,
  you looked at the frames, the log has no `Error` line of yours and no asset that failed to load, and the
  level's `FTTRACK level=<index> frames=.. route=.. solid=.. solidTriangles=..` line equals its line in
  `SourceArt/Jam/validate/track_baseline.txt` (the proof that the track is as it was). No benchmarks, no packaged
  builds, no release checks: those come later, elsewhere.
- **Batches.** First batch, as soon as it stands: the figure builder, the film crew, and the first two levels
  (Bayshore and City Center need people most). Then pairs of levels. Then a second round everywhere: animals, far
  life, the staged moments. Do not wait for a perfect whole.

## Pushing

You push your own batches straight to `main`; that is the owner's order for tonight ("push it straight to main").

1. Commit only your own files (the four places in the table above), with explicit paths. Messages say what a
   player now sees, and end with: `Co-Authored-By: Claude Fable 5.2 <noreply@anthropic.com>`
2. `git fetch origin`, then `git merge origin/main` into your branch (never rebase, never a forced push). `main`
   moves fast tonight: several sessions and people push to it.
3. If the merge brought new C++ in, compile again. A push that does not compile stops the whole team.
4. `git push origin claude/people:main` (an ordinary push). If it is refused because `main` moved, fetch, merge and
   push again. If `main` refuses you for another reason, push `claude/people` instead and say so in your status.
5. Also push your branch itself (`git push origin claude/people`) so that the lead can see work in flight.

## Talking to the lead

- You to the lead: `SourceArt/Integration/people/STATUS.md`, pushed with every batch. For each level: what a
  player now sees, the pieces and their counts, what moves, what is still weak, what you did not check. A section
  **ASKS** for anything you need from the lead (a table word, a place the dressing should leave free, a question
  for Matt about the track). Contact sheets as JPEG in `SourceArt/Integration/people/sheets/` (under 5 MB each;
  no raw frame dumps, no logs).
- The lead to you: this file, section "Notes from the lead" below.
- Tell Justin the commit after each push; he relays what matters.

Report only what you ran and saw. If something was not checked, say "not checked". Do not put anyone's name,
e-mail address or this project's details into any outside service.

## Notes from the lead

- 15 Nov, 16:25: the reader for the population tables is commit `ef29afe`; it is on `claude/overhaul-integration`
  and reaches `main` with the lead's next push (17:30 or so). Start from whichever has
  `Source/FullThrottle/FTPeopleL0.inl`.
- 15 Nov, 17:15, about compiles on a hard disk (it cost the main PC most of a day): a compiler reads its 2.4 GB
  precompiled header by page faults; from a spinning disk with anything else running that takes tens of minutes.
  Keep the engine and the project's `Intermediate` on an SSD, or read the `.pch` file straight through once before
  a compile (14 s). `Build.bat` holds a lock that every editor start waits behind: run one or the other.
- `main` moves fast and the menus are being restyled there (`FTHUD.cpp` will be split into several files): none of
  that touches your files, but compile after every merge of `main`.

- **15 Nov, 18:00. The owner at 17:53: "build absolutely everything that's planned now, compile and push later."**
  Author all eight levels' casts back to back; use the editor only to make assets; commit and push as you go (to
  `claude/people`, and to `main` when a batch compiles); looking in the game can come in one pass at the end.
- **15 Nov, 18:00. Corrections to this brief** (it was fact-checked against the repo; where these differ from the
  text above, these are right):
  1. **A packaged game would have had no people.** `/Game/People` was not cooked (tables name meshes by string).
     The lead has added it to `Config/DefaultGame.ini` on `main`. A master material of your own must also set
     `used_with_instanced_static_meshes`.
  2. **Commit what your generators save outside your folders**: every NEW file under `Content/Core/Palette` and
     `Content/Core/Life/Instances` (`ft_export.save_mesh` and `ft_life.life_mi` make them; names come from the
     colours, so both PCs can make the same file: on a merge conflict there keep `main`'s). Never a changed one.
  3. **Give your Sets names of your own** (a Set named like one of the dressing's replaces it for the dressing's
     Sites too), and **do not use `Lit()`** (it takes a real light from a pool of 18 that the road's lamps share).
     When a dressing table changes on `main`, your Scatter pieces are re-rolled (a rule's random stream is seeded by
     its running index): pin a staged scene with a one-piece Set and a Site at chance 1.
  4. **Nothing stops vertex movement at 180 m**: that sentence is only a comment in `ft_life.py`; the track's code
     never set it. A piece stops being DRAWN at its cull distance (`FTTrack.cpp`, `Comp`: 120 x bounds radius x 1.3,
     600 m at least, 4 km at most).
  5. **Heights are measured from the ROAD, by the code**: a piece whose bounds reach 2 m above the road's level is
     pushed out to 24 m by the track itself, or left out (read the log's `FTCLEAR` line per mesh). The land 16 m out
     lies about 0.85 m under the road's line, so roughly 2.8 m of piece fits before that. The 17 m rule is yours to
     keep (the code stops only at 15.6 m). Scatter and Site pieces within 40 m of the centre line at a jump's run-up
     or landing are left out, and at any distance beside a loop, jump gap, pipe or corkscrew: a crew "at a jump"
     stands 40 m out. Beside a banked bend, whatever stands 17 to 40 m out floats over the cutting: write your near
     zones to leave the banked bends out, as the dressing tables do. A piece under 2.5 m tall, or more than 70 m
     out, casts no shadow.
  6. **The table word `Far` is past the land's edge (300 m), 25 m under the road.** From 150 to 295 m use Scatter
     or Site. The example line's side defaults to BOTH sides: write the side
     (`..., 17.5f, 21.f, 2, 0, 1.f, 1.f, 0.f, 1);` = scale 1, jitter 0, right side) under a Zone of a few kilometres.
  7. **In test scripts wait for `FT CLOCK level=8.|150`** (not `8.0`: a slow frame prints 8.1 and the wait runs out),
     add ` -ftnoattract` to `-Extra`, give `-Script` a full path, and read the `FTDRESS` line that follows YOUR
     level's `FTTRACK` line (its level is the number minus 1): "tried" up and "placed" not = a wrong mesh path;
     neither up = a wrong Set name. If `route=` or `solid=` differ on your PC with `frames=` and `solidTriangles=`
     equal, compare the `FTTRACKSUM` line with the baseline's before calling the track changed.
  8. **People that exist already, not to be made twice**, beside the park's `crowd` and `_guest`: City Center's
     `_ped` (pavements, bus stops, kerb cafes: `ft_dress_metro.py`), the park's `queue_rails`, `plaza_seats` and
     `mascot`, Under the Sea's `jellyfish`, Bayshore's gulls (the sky plugin). Yours sit beside them without a seam.
  9. Build first, then start the editor (not both at once).
- **15 Nov, 18:00. Where the zones want people** (from the zone workers; kilometres along the road, metres out):
  Theme Park: ticket-gate forecourts 0.1 to 0.9 km, stall rows 1.15 to 2.0 km and from 9.3 km (25 to 30 m), the
  galleon's deck 2.44 km, Log Falls's pool 3.69 km, lagoon bandstands and lake bridges 3.3 to 6.6 km, rocket pads
  6.7 to 8.1 km, the Last Ride's gate 9.27 km. City Center: pavements, bus stops and cafes 0 to 7.6 km (17 to 24 m,
  under 2 m), premiere crowds at the picture houses 1.1 to 2.1 and 6.3 to 7.6 km, bank steps 4.6 to 6.3 km,
  penthouse roof terraces, Syndicate guards from 7.7 km (plainly scenery). Bayshore: the boardwalk and its shop
  fronts the whole level, the beach behind the barrier, the piers (9.18 km and the three later ones), the chair
  lift. Under the Sea: divers at the salvage ship (5.2 km) and the diving bell, aquanauts between Nereida's towers
  (8.1 km). More lists follow as the zone workers hand back.
- Seen from here: your figure builder and the crew kit (10d079d) read as this game's. Crew pieces of 1,800 to
  2,700 triangles are fine for pieces placed a dozen times a level; keep 1,500 for anything scattered by the hundred.
