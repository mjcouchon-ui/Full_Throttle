# The population pass: status (second machine)

Last written: 15 Nov 2026, after the core and the film crew kit; the level workers are running.
Report rule: only what was run and seen. "Not checked" means exactly that.

## Machine

i9-12900KF (24 threads), 64 GB, RTX 3080 Ti, Unreal 5.9.0 (launcher build), MSVC 14.44, 447 GB free.
Branch `claude/people`, started from `origin/claude/overhaul-integration` (`1df93f9`).
First editor build: succeeded. A table change compiles in about 12 s; a 40-second drive of a level takes about 60 s.

## What exists

| What | Where | How it was made |
|---|---|---|
| The figure builder, birds, four-legged animals | `Content/Python/ft_people.py` | `figure()`, `bird()`, `quadruped()`, `Scene`, `export()` |
| The population's material | `/Game/People/Core/M_People`, `M_PeopleFar` | `ft_people.build_core()` (`_master`) |
| The film crew kit, 10 pieces x 8 wardrobes = 80 meshes | `/Game/People/Crew/SM_Crew<Piece>_<Wardrobe>` | `ft_people_crew.run()`; one function a piece (`camera`, `applause`, `stunt`, `village`, `dolly`, `crane`, `lights`, `sound`, `catering`) |
| Close-up contact sheets | `Content/Python/ft_people_sheet.py` | `sheet()` (plain light, no cel pass) |

**M_People.** One ordinary lit material for every population mesh (the cel pass inks it like any prop; `M_PP_Cel`,
`ft_life.py` and `M_Life` are untouched). A population mesh has ONE section, so one draw call an instanced mesh,
whatever its colours and however its parts move. Each part's colour and movement are written into the mesh's eight
UV channels, the same for every vertex of the part: colour and glow; phase and rate; a hop or a float; a swing in
degrees about a pivot and an axis (an arm waves, a leg strides, a wing beats, a camera pans); travel along a line and
round again (walkers), or round a circle (a wheeling flock). A part can be marked to liven up as the camera comes
within 25 to 110 m (crowds hop higher as the chase passes). Vertex shader only; no ticking actor, no skeleton.
Shader size reported by the editor: 329 pixel and 375 vertex instructions.

**The figure.** Chunky, big head and hands, flat palette colours; six skins, eight hair colours, seven hair styles,
nine hats, four builds, sleeves, trousers, shorts, skirt, dress, coat, swimwear; 13 named arm poses; things in the
hand. About 250 triangles by the road, about 100 for crowds.

**The crew.** Marked on every level by a pink band across the chest, headsets, yellow-and-dark equipment and
pink-and-white tape. Sheets: `sheets/crew_kit_sun.jpg`, `sheets/crew_wardrobes.jpg`.
Low pieces (top under 2 m as built, may stand from 17 m): Camera 1.94 m, ApplauseA 1.87, ApplauseB 1.79, Stunt 1.75,
Village 1.88, Dolly 1.77. Tall (24 m or more): Crane 4.3 m, Lights 3.87, Sound 2.15, Catering 2.47.

## Over budget, and why

- Crew pieces with six or seven near-detail figures are 1,800 to 2,700 triangles (the guide is about 1,500): a
  near figure with a hat and something in its hand is about 300. They are single-section meshes. To be trimmed in
  the second round if the lead wants it (fewer tape segments, detail 1 for the back row).

## Seen in the game so far

- One test drive of Bayshore with two test meshes placed (since removed): the level came up, drove to 30 s, frames
  read. `FTTRACK level=0 frames=12717 route=dc9e032b solid=d2e759f4 solidTriangles=986296`: equal to the baseline.
  The figures are inked by the cel pass and sit in the world's style. They are SMALL from the chase camera, and the
  raised, banked road's barrier hides people who stand on low ground near it: the level workers are told to raise
  people (stands, platforms, terraces), mass them, and move them.
- Movement in the game (`film` frames): not yet judged by me; the level workers and their reviewers do it per level.

## Levels

Being made now, one worker a level in its own worktree, each followed by a reviewer who did not make the work and
then a fix pass. Per-level status files will be `L<n>_<key>.md` beside this file. Nothing is placed on any level yet
in this commit: the eight `FTPeopleL<n>.inl` are still empty.

## Not checked

- Performance of any kind (by the owner's order: later, elsewhere).
- Whether vertex movement really stops past 180 m for these meshes (I did not find the switch in the track's code;
  far pieces take `M_PeopleFar`, which always moves).
- The crew kit in the game's own frames (only close-up sheets under plain light so far).

## ASKS

- None yet.
