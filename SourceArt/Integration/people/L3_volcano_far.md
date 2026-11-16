# L3 Volcano: far life and creatures (extra worker "f"), 15 Nov 2026

Branch `claude/people-L3-f`, worktree `C:\ftwork\wt\L3f`. Generator `Content/Python/ft_people_volcano_far.py`; rules: the
"far" block of `Source/FullThrottle/FTPeopleL3.inl`; meshes `/Game/People/Volcano/SM_Far*`. Only what was run and seen.

## What a player now sees

Over the Volcano's slopes the sky is busy: strict V's of great white cranes with black wing tips cross the orange horizon,
strings of blue herons go the other way, and gold vultures wheel round the sun. On the slopes 165 m out stand basalt
crags with white goats on their ledges and a goatherd waving a pink cloth from the top; in the caldera, the pass and the
far works a survey team in white coats works a crater's rim under a pink-banded weather balloon; and along the ash
districts the villagers leave on a raised ridge path, ox wagons under pink and cyan tilts, bundles on heads, banners.

## The meshes (6; all saved far=True, all colours lifted with a small steady glow, all TALL and 150 m or more out)

| mesh | shows | function | triangles | what moves (as built) | where the table puts it |
|---|---|---|---|---|---|
| SM_FarSkein_A | 13 cranes in a V stood on edge, birds 15 to 17.5 m across, 40 to 125 m up | `skein_v` | 1404 | wings beat, the V travels 360 m along the road and comes round | Far 210 to 300 m, drop 0: the ash districts every 14 slots, the survey districts every 16 |
| SM_FarSkein_B | 13 blue herons in a double string, going the other way | `skein_string` | 1404 | wings beat, the string travels | Far 210 to 300 m: lava jumps, magma run, burning slopes, inner wall, every 12 |
| SM_FarKettle | 9 gold vultures on circles of their own, 36 to 86 m up | `kettle` | 864 | each circles the thermal, wings barely beat | Far 150 to 260 m, any way round: every 14 or 16 |
| SM_FarLedgeGoats | a stepped basalt crag 48 m long, 9 goats five times life size, the goatherd with a cloth | `ledge_goats` | 2584 | 5 goats walk the two ledges and come round, 4 leap on the heads, the cloth waves | Far 162 to 174 m at the land's own drop, every 18 or 22 |
| SM_FarRidgeLine | a causeway 144 m long, 8 m up: 2 ox wagons, 6 people with bundles and banners, a child, 4 goats, five times life size | `ridge_line` | 2884 | everything walks the causeway and comes round, legs stride, a child waves | Far 180 to 188 m, every 22, in the five ash districts |
| SM_FarRimSurvey | a crater cone 15 m tall, 4 surveyors at 4.2 times life size, theodolite, striped staff, flag, anemometer, balloon | `rim_survey` | 1680 | the theodolite pans, the flag waves, the cups circle the mast, the balloon sways | Far 164 to 172 m, every 10: the caldera (left only), obsidian pass, the far works |

Placed: the game's count line rose from 8287 (the dressing alone, the main worker's empty run) to **8491: 204 pieces**
of this block (69 rules). The game does not print a count per mesh for these; not counted per mesh.
Nothing on the right between 2.4 and 3.9 km (the eruption's side).

## What was run and looked at

- 4 game runs of level 4 (index 3): the start through lava jumps (twice), scene 3 (caldera), scene 4 (fire and
  brimstone), scene 6 (obsidian pass) into Engine 13's road, scene 7. Cropped and enlarged frames of all six meshes.
- FTTRACK level=3 `frames=12497 route=506786e7 solid=eead12ba solidTriangles=900592` in every run: equal to the baseline.
- Seen to read from the road: both skeins, the kettle (against the sun in the caldera), the goats' crag (white goats,
  the herder's cyan coat and pink cloth), the rim survey (the balloon first, then the white coats), the ridge line
  (the pink and cyan tilts carry it). Contact sheet: `sheets/L3_far_1.jpg`.
- Fixed after the first look: birds made half as big again; goats from 4 to 5 times life size and brighter; the ridge
  line moved from 286 m (the cinder cones and the mine hid most of it) to 184 m.

## Still weak

- SM_FarLedgeGoats (2584) and SM_FarRidgeLine (2884) are over the 1,500-triangle guide: the animals cost about 170
  triangles each. The ridge line is one piece 144 m long where three would otherwise stand.
- Beside the mine on Engine 13's road the ridge line shows only in the gaps between the conveyor and the tip.
- The crag is seen end-on while it is far ahead; the goats on the ledges show best from alongside.
- The rim survey stands among the caldera's own basalt, so its cone hardly shows there; the balloon and the team do.
- The ridge line (180 to 188 m) and the main table's caravan (196 to 204 m) can stand one behind the other.

## Not checked

- Movement of the ridge line's walkers, the goats and the survey's instruments in the game: not confirmed. The films
  were shot from a car at 80 to 145 mph and the frames do not settle it. Only the skeins were seen to move (their place
  against the far peaks changes from frame to frame).
- Together with the main worker's pieces and the crew (my runs have only the dressing and this block).
- The last three districts (16.7 km on), the inner wall, the burning slopes: not driven.
- Frame rate. 204 always-moving instances were added; not measured.
- One handled ensure (`OriginX <= OriginMax`, DoubleFloat.cpp, "view transform is invalid") appeared in 2 of the 4 runs,
  both at 340 to 390 m of the opening straight and both when the car was NOT on the plain autopilot (ft.AutoX 900; no
  autopilot with the brake held): in both the car's body tumbled and vanished from the frames, wheels left. The two
  plain autopilot runs with the same skeins and crags in place had none. Cause not found; not shown to be this block's.

## ASKS

- A count per population mesh in the FTCLEAR line even when nothing was left out (it lists only meshes with a
  left-out or moved count, so a Far-only block can only be counted as a total).
- A harness way to stand the car still (the brake holds 80 mph): movement of far pieces cannot be judged otherwise.
- The lead joins this block after the main worker's; the two `#define`s (VP/VC there, VF here, undefined at the block's end)
  do not collide.
