# Level 6, Under the Sea: the watchers (extra worker "w", 15 Nov 2026)

Branch `claude/people-L6-w`, worktree `C:\ftwork\wt\L6w`. Generator `Content/Python/ft_people_sea_watch.py`,
meshes `/Game/People/UnderTheSea/SM_Watch*`, rules: the `watch` block of `Source/FullThrottle/FTPeopleL6.inl`.
Only what was run and seen is reported here.

## What a player now sees

Nereida has come out to watch the chase. Along the whole road, 26 to 38 m out, aquanauts in yellow, orange, pink,
white, lime and red suits (white helmets, dark visors: the same figure as the main kit's crowds) stand massed on small
buildings of their own: the balcony round a habitat drum on a pylon, the two landing platforms of an airlock tower,
the rim walkway of a viewing dome (a row of them sitting on the edge, legs over the side). Where there is no room for
a building (in front of the kelp walls and in front of the travel tube) fourteen divers hang upright in the water in
three rows, 3 to 9 m over the sand, with a long striped banner floating between them. Out on the trench walls stand
42 m galleries of giant figures in front of a wall of colour blocks. Striped pennants on every piece; people hop
slowly, wave, point; a few hang in the water over the crowd with flags.

## The meshes (6; all made by `ft_people_sea_watch.run(everything=True)`)

| mesh | what it shows | function | triangles | low / tall | what moves | where the table puts it |
|---|---|---|---|---|---|---|
| SM_WatchBalcony_A | habitat drum on a pylon, balcony 4.3 m up, 12 at the rail, 2 floating over it with flags | `balcony(1)` | 2700 | TALL (top 13.9 m) | slow hops and floats out of step, arms wave (react), pennants swing, floaters rise and settle | 33-37 m: reef L, station L, salvage R, approach L, Nereida R |
| SM_WatchBalcony_B | the same, a second crowd of 5 on the drum's roof | `balcony(2, roof=True)` | 2892 | TALL (13.1 m) | as A | 33-37 m: reef R, kelp R, salvage L, approach R, Nereida R |
| SM_WatchAirlock | airlock tower 10 m, landing decks at 3 and 6 m, 7 + 5 people, one rising beside the tower with a flag | `airlock(1)` | 2300 | TALL (11.4 m) | hops, waves, the riser floats 0.9 m up and down, pennants | 33-37 m: reef R, kelp L, salvage L, approach L, Nereida R |
| SM_WatchDome | viewing dome, 9 standing on the rim walkway (3.2 m), 6 sitting on its edge, 3 on the crown | `dome(1)` | 2884 | TALL (11.2 m) | hops, waves (the sitters wave only), pennants | 34-38 m: reef L, station L, salvage R, approach R, Nereida R |
| SM_WatchWall | 14 divers upright in the water in three rows (2.5, 5.6, 8 m), a 15 m striped banner between two anchor lines | `wall(1)` | 2164 | TALL (11.7 m) | every diver floats up and down out of step, arms wave, the banner floats | 26-27.5 m in front of the kelp and of the travel tube (kelp both sides, station R, Nereida L); 42-50 m over the coral elsewhere |
| SM_WatchFarGallery | 42 m gallery on stilts, 22 figures 2.5 times life size, 5 m wall of colour blocks, three 6 m pennants; `far=True` | `far_gallery(1)` | 2388 | TALL (20.2 m) | hops of 0.5 to 0.85 m out of step, arms, pennants (always-moving master) | 120-190 m both sides, every 10th to 14th slot |

Counts (game log, `FTDRESS level=6 pieces placed`): dressing alone 9597 (the main worker's base run); with the first
version of this block 9901 = 304. After the last change (the rules behind the travel tube taken out, the station's
diver wall moved in front of the tube) 9873 of 9884 tried = **276 pieces of this block** (the 11 not placed are the
dressing's own, as in the base run: 9597 of 9608). SM_WatchWall: 87 placed, 8 of them pushed out from the barrier. Per-mesh counts are not in the log (FTCLEAR
lists a mesh only when one of its pieces was moved or left out): not counted per mesh.

`FTTRACK level=6 frames=12470 route=04d3f899 solid=8927e339 solidTriangles=930148` in every run = the baseline line.

## What was looked at (cropped, enlarged game frames; sheet `sheets/L6_watch_1.jpg`: eleven game crops and, last, the six meshes under plain light)

- Drive from the start (scene 1, 46 s, right then left lane, two films): the diver wall over the coral reads as people
  in the water from about 100 m (rows, banner, anchors); the dome with its rim crowd at 34 m; balcony B with its roof
  crowd and pennants.
- Scene 4 (the research station): the far gallery reads as a band of coloured figures behind the pressure spheres.
  Most of this drive is corkscrew, loop and tunnel, where Scatter rules place nothing.
- Scene 5 (the salvage field, after the last table change): see "Not checked".
- Scene 7 (Nereida, the boss on the road): the diver wall in front of the travel tube (left), balcony B, the airlock
  with both decks lit, the dome behind the pipeline (right).
- Movement: in the film frames the divers' arm poses differ from frame to frame. The car moves between frames, so the
  size of the hops was not measured.

Fixed after looking: the airlock's lower deck was under the upper one and its crowd in shadow (deck brought 1.6 m
forward); rail panels hid the balcony crowd to the chest (lowered, figures 1.42 size); pieces behind the travel tube
(an airlock at 47 m scaled 1.5, a diver wall at 44 m) were hidden by the tube up to about 8 m: taken out, the divers
moved in front of the tube.

## Still weak

- The kelp forest (1.7 to 3.4 km): the weed stands from 27 m and is 20 m tall; only the diver walls in front of it
  show, and in the one frame that holds one it is small. The balconies and airlocks at 33-35 m there stand inside the
  weed.
- The research station has few near pieces (4 or 5 a side) and much of it is set pieces without Scatter slots.
- Pieces stand among the dressing's coral gardens and reef floor (a deck on stilts over coral reads well); a piece
  can still meet a tall dressing piece (kelp, a pressure habitat) where the phases did not keep them apart: seen once
  (a kelp stand beside a diver wall in Nereida), not searched for along the whole road.
- Every mesh is over the 1,500-triangle guide (2,164 to 2,892): a crowd of 14 to 22 at about 120 triangles each plus
  the building it stands on. One variant only of airlock, dome, wall and gallery (the limit was 6 meshes).

## Not checked

- The salvage field: the last drive (scene 5) is jump, pipe and corkscrew for most of its 30 s and no frame of it
  holds a watcher piece close enough to judge. One frame at the start of scene 6 shows a diver wall hanging against a
  wreck's hull plate, a beam of the wreck across it: an overlap with the dressing, not fixed.
- The approach (6.6 to 7.94 km): not driven.
- The glass tunnels, loops, corkscrews and jumps: Scatter rules place nothing there, so there are no watchers there
  (Far rules used as land rules, the dressing's way, were not tried).
- Together with the main worker's pieces and the film crew (only this block and the dressing were in these runs).
- Frame rate. Shadows. The light rule was kept by construction (steady port lights only, glow 0.5, nothing nearer
  than 26 m), not measured.
- The far gallery past 180 m in motion (it uses the always-moving master; seen only as a still frame).

## ASKS

- `SM_Watchers_A/_B` (the main kit's stand) begin with the prefix `SM_Watch` that the extra brief gives to this worker:
  the names do not collide (mine are Balcony, Airlock, Dome, Wall, FarGallery) but a tool that selects `SM_Watch*`
  takes both.
- `run_game.ps1` prints the baseline of the FIRST level in the log (level 0, the attract map), not of the level run:
  compare by hand. FTCLEAR lists only meshes with a moved or refused piece, so a per-mesh count of placed pieces is
  not available from the log.
- `waitfor FT CLOCK level=8.0` misses when the clock line reads 8.1: write `level=8.` (my first drive lost 15 minutes).
- A table word that places on the land inside tunnel, loop and jump districts with the road test (the dressing uses
  Far with a drop for this) would let watchers stand beside the glass tunnels.
