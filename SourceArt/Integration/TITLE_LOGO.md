# The title menu's animated logo

What the title now does:

- **Intro**, once on each arrival at the title, done inside one second: the masthead starts empty, "Full" is thrown in
  and lands, THROTTLE and the underlines wipe on left to right behind a bright slanted edge, the two stars twinkle on.
- **Alive**, until a selection is confirmed: the neon "Full" breathes (3.1 s cycle) and now and then flickers, the
  stars glint on their own fixed timers, a faint light crosses the chrome every 9.5 s, a quiet hum plays underneath.
- **Confirm**: both stars flash together once, with a chime, and the logo holds at rest.

At rest the game draws the same flat `T_TitleLogo` tile it always drew, from the same numbers, so the masthead is
pixel for pixel what it was. The layers are used only during the intro; everything alive is light added on top of
the flat logo, inside its rectangle.

## Switching it off

- In the console: `ft.TitleLogo 0`. The HUD then draws the flat logo with its old three lines, and no logo sound plays.
- To make it stick: `-ExecCmds="ft.TitleLogo 0"` on the command line, or a line `ft.TitleLogo=0` under
  `[ConsoleVariables]` in `Config\DefaultEngine.ini` (the ini form was **not tried**; the console form was filmed).
- If any of the eleven layer textures is missing or has the wrong size, the flat logo is drawn the old way by itself
  (log: `FT TITLE logo layers=<n> ok=0 ... missing=<names>`). A missing sound is silent.

Other switches (console variables): `ft.TitleLogoVol` (1.0, every logo sound), `ft.TitleLogoHum` (0.035, 0 = no hum),
`ft.TitleLogoIdleChime` (1; 0 = idle twinkles silent), `ft.TitleLogoSweep` (1; 0 = no chrome sweep),
`ft.TitleLogoRearm` (1; 0 = after a confirm the logo stays steady for the rest of that title visit),
`ft.TitleLogoLog` (0; 1 = log each idle flicker, twinkle and sweep), `ft.TitleLogo 2` (test: the new draw path held at
rest). Test command, not in shipping builds: `ft.TitleLogoConfirm` (the confirm twinkle without touching the menu).
REDUCED MOTION (OPTIONS): no intro, shake, flare, wipe, flicker, twinkles or sweep, and none of their sounds; the slow
glow and the hum stay; a confirm plays only the chime.

## Files

New code: `Source/FullThrottle/FTTitleLogo.h`, `FTTitleLogo.cpp` (a game-instance subsystem: the HUD and the game
mode are rebuilt on every attract cut, the game instance is not).

Existing code, two small hunks each. **Please keep them whole in any hand merge** (`FTHUD.cpp` is still being
committed to by the menus-and-HUD work):

`FTHUD.cpp`: `#include "FTTitleLogo.h"` after `FTProgress.h`, and in `DrawTitle` the three lines that drew the logo
tile are wrapped, unchanged, in one `if`:

```cpp
			// (the animated logo, FTTitleLogo.cpp; when it is off or a layer is missing, the flat logo exactly as before)
			if (!UFTTitleLogo::Draw(Canvas, G, TLogo, LogoX, LogoY, LogoDrawW, LogoH, (.74f + .26f * In) * Fade, S, bInteractive, bReducedMotion))
			{
				FCanvasTileItem Logo(FVector2D(LogoX, LogoY), TLogo->GetResource(), FVector2D(LogoDrawW, LogoH), FLinearColor(1.f, 1.f, 1.f, (.74f + .26f * In) * Fade));
				Logo.BlendMode = SE_BLEND_Translucent;
				Canvas->DrawItem(Logo);
			}
```

`FTGameMode.cpp`: `#include "FTTitleLogo.h"` after `FTNightFlow.h`, and in `TickTitle`, before `switch (Activate)`:

```cpp
	if (Activate != INDEX_NONE) UFTTitleLogo::Confirm(this, Activate);
```

Nothing else on the title menu is moved, resized or reordered. `FTHUD.h` and `FTHUDSkin.*` are untouched.

Harness and process files: `SourceArt/Jam/validate/menus_walk.txt` (its set-up now sets `ft.TitleLogo 0`, so its
title stills at 0.2 to 0.7 s still show the flat logo and compare with older runs), a line in
`notes/notes_menus_walk.txt`, a paragraph in `validate/README.md`, the log lines and console variables in
`notes/harness_reference.txt`, and one row in `IN_FLIGHT.md` beside this file.

Art (the artwork itself is unchanged; the flat PNG renders byte for byte as before):
- `Tools/title/logo.html`: a `?layer=throttle|star1|star2|full|lines` switch; no parameter draws all of it.
- `Tools/title/render_layers.ps1` (headless Edge, then `make_layers.py`), `make_layers.py`, `layers.json`,
  `L_*.png` (the five full-canvas layers), `T_TitleLogo_*.png` (eleven textures: five cropped layers, two "lit"
  stars, a soft and two glow copies of "Full", a light mask of the chrome).
- `Content/Python/ft_title_logo_import.py`, `Content/UI/Title/T_TitleLogo_*.uasset` (eleven, about 5.5 MB in memory).

Sound (made by the repo's generator, imported by `ft_audio.build` with the settings of `S_Pickup`, played 2D like
every menu sound, x 0.4 under the attract demo like every menu sound; the mute option silences them with the rest):
- `Tools/sfx_synth.py`: a block "the title's logo" (`over()` gained an optional `loop` argument; every older WAV
  still comes out byte for byte the same). `Content/Python/ft_audio.py`: `S_TitleNeonHum` added to `LOOPS`.
- `Content/Audio/SFX/S_TitleSlam` (whoosh into a thud, the thud 0.18 s in), `S_TitleWipe`, `S_TitleTwinkle`,
  `S_TitleTwinklePair`, `S_TitleNeonTick`, `S_TitleNeonHum` (4 s seamless loop).

## What was measured (16 Nov 2026, uncooked game, 1920x1080, a second PC under load from other jobs)

Film scripts and tools are outside the repo (`C:\ftreel\shots\tl_*.txt`, frames and sheets in
`C:\ftreel\titlework\fin\`). All of it was filmed again on the final code, with `-nosound` unless said:

- Intro at 60 fps, title time: empty until 0.10 s; "Full" in flight 0.10 to 0.28 s; lands 0.283 s with the flare and
  the shake on the logo only; wipe 0.283 to 0.63 s; left star from 0.64 s, right star from 0.74 s; flat logo from
  1.017 s. Zero / landing / end logged at 0.104 / 0.285 / 1.007 s at 144 fps and 0.100 / 0.300 / 1.000 s at 30 fps.
- From the first flat frame on (frames 59 to 240 of the arrival film) no pixel outside the logo's own rectangle
  differs from the build before this change (three masked noise regions aside: the live view, the CONTINUE bulbs,
  the LAST SAVE clock). During the landing (frames 15 to 58) nothing differs beyond 50 px round the logo. During the
  flight (0.10 to 0.28 s) the soft "Full" crosses the board and the view by design. Frames 59 and 60 equal the flat
  logo exactly.
- After a highlight move, 2 frames of 12 and 2 of 60 differ from the old build by 1 level in at most 30 pixels of the
  shoot rows. The same kind of difference (2 frames, 32 pixels, 2 levels) shows between two runs with the logo
  switched off, and between two runs of the old build, so it is taken to be run-to-run noise of the highlight (it is
  timed by the real clock); that cause was not proven.
- `ft.TitleLogo 0`: every filmed frame equal to the old build, logo included (the same noise aside).
  `ft.TitleLogo 2`: logo rectangle equal in all 313 frames.
- A key during the intro (Down at 0.40 s): the next filmed frame shows the flat logo exactly (0 levels of
  difference) and `FT TITLE sel=2` follows one frame later; O during the intro opens OPTIONS.
- Alive (27 s filmed at 30 fps): the "Full" area brightens by up to 25 levels of 255 on average with the breath; a
  flicker dip darkens it by 13 on average (54 at most) for one frame at 30 fps (two at 60 by the arithmetic, not
  filmed); no pixel outside the logo's rectangle changes in the frames compared (every third of 810).
- A highlight move does not stop it (`sel=2`, no `confirm=` line, a star twinkle plays during the film).
- Confirm by the test command: both stars flash together for 24 frames, then 10 frames equal the flat logo exactly,
  then the glow fades back in. A confirm placed in the middle of a star's idle twinkle: the half-done twinkle does
  not come back when the logo wakes, and the alive patterns go on from where they stood.
- A confirm straight after OPTIONS closes (0.2 s, the logo still steady) chimes and twinkles again: by the test
  command and by a real Enter (`confirm=0`, the pair cue logged).
- `ft.TitleLogo 0` for 30 s and back to 1: no burst of missed ticks or chimes; the next events come at their own
  alive times.
- REDUCED MOTION switched on in OPTIONS: flat logo from the first frame, the glow only (no star, sweep or dip in
  240 frames), a confirm leaves the frames exactly flat.
- The real title (attract demo on, audio device on): across attract cuts there is no second intro and the hum is not
  restarted; the one-shots are asked for at 0.4 of their volume. The last chime before the first cut is logged
  1.7 s before the map load (it used to be 0.04 s).
- Back from PRE-PRODUCTION (READY by the harness and by a real Space): the intro plays in full both times.
- 960x540 was filmed and looked at. `C:\ftreel\shots\s14_menus.txt` (the title capture script) runs with no timeout.
- No engine ensure in any of these 13 runs.

## Read this before trusting the sound

- **Nobody listened to any of it.** Levels come from measuring the WAV files and from two simulated two-minute mixes
  against the title theme (`C:\ftreel\titlework\B\title_2min_attract.wav`, `title_2min_noattract.wav`). "Not annoying
  after two minutes" is unproven: that listening test is still owed by a person. The attract demo's own race sounds
  are not in those mixes, and they were made before one idle chime moved from 10.6 s to 8.9 s.
- The WAV files as made (44.1 kHz mono; seconds, peak dBFS, RMS dBFS): slam 0.45, -0.92, -14.55; wipe 0.36, -6.02,
  -21.83; twinkle 0.50, -4.44, -21.41; pair 0.65, -4.44, -21.93; tick 0.03, -6.02, -24.07; hum 4.00, -6.02, -14.66.
  They are played at 0.32, 0.28, 0.32 (0.09 idle), 0.22, 0.12 and 0.035 of that, and the one-shots at 0.4 of that
  again under the attract demo.
- The log proves each cue is asked for at the right moment (`FT CUE logo=<name> vol= pitch= found=1`), and one run
  with the audio device showed the hum's component being made (`FT TITLE logo hum=1 dev=1`). Whether the hum's fades
  and loop point are clean through the engine was not heard.
- Under the attract demo (the title's normal state) the logo's one-shots play at 0.4, so the thud peaks near
  -19 dBFS. If the intro is too quiet raise `ft.TitleLogoVol`; if the hum or the idle chimes tire, `ft.TitleLogoHum 0`
  and `ft.TitleLogoIdleChime 0`.
- Summed peak in the simulation: -6.7 dBFS over two minutes. The worst case on paper (attract off, a key pressed on
  the thud, the music's loudest sample at the same instant) is -1.06 dBFS.

## Reading choices the owner may overrule

- After a confirm that leaves the player on the title (OPTIONS, CONTROLS, a dialog) the logo comes alive again 0.6 s
  after the title is back. `ft.TitleLogoRearm 0` keeps it steady instead.
- **After a real confirm the paired twinkle is not seen.** PLAY SELECTED, CONTINUE and PRE-PRODUCTION leave the
  title in the same frame as the confirm, so the twinkle is drawn for no frame at all; OPTIONS, CONTROLS and the
  NEW GAME dialog cover it. Only its chime is asked for. Showing it needs a short hold on the title (about 0.3 s)
  before the transition, which changes the menu's own timing and belongs to the menus-and-HUD work: the owner's
  call. It was filmed with the test command only.
- The alive state keeps its own time: it stands still whenever the alive logo is not drawn (under an overlay,
  while the logo is steady after a confirm, with `ft.TitleLogo 0`), and goes on from there. It does not depend on
  how long the title has been up.
- A press that the menu itself ignores while it waits for a held button's release after a map load does not skip
  the intro either. A push of the stick skips on any pad. A mouse move alone does not skip.
- A skip or a confirm cuts the intro's sounds over 0.04 s.
- The shake lasts 0.05 s (3 frames at 60 fps, 2 at 30). A flicker dip lasts 0.034 s (2 frames at 60 fps, 1 at 30).
- Enter, Space and pad A do nothing in the title's first 0.4 s, as before this change; such a press still skips the
  intro.
- On a machine slower than about 14 frames a second the intro waits until 0.4 s before it starts.
- In its first 0.1 s the thrown "Full" is many times its size and passes over the board and the live view, and under
  the shoot rows and buttons (they are drawn after the masthead).

## Did not work, or was not done

- `SourceArt/Jam/validate/menus_walk.txt` does not run through on this code, with or without this change in play: it
  loses its place at section 8 (`ft.Press R` on the pause menu, then `waitfor FT MUTE reapplied` times out; the pause
  menu in today's `FTGameMode.cpp` has no R shortcut for RESHOOT) and everything after it times out in turn.
  Sections 1 to 7 give their lines, except the two `FT TITLE overlay=` waits (the game logs
  `FT SCREEN name=Pause state=opened` there). Three runs gave the same 20 timeouts and 132 frames: the logo on, the
  logo off by a first line, and the walk as it now stands (`ft.TitleLogo 0` in its set-up; its title stills show
  the flat logo). There was no build of the code before this change on the PC that did this work, so "the same as
  before" rests on those runs and on reading the code.
- `jam_audit.ps1` and `jam_verify.ps1` need the lead's PC and were not run. The boss, combat and brawl scripts do
  not touch the title and were not run.
- The `DefaultEngine.ini` form of the off switch was not tried.
- No frame-by-frame comparison with the trailer's end card was made beyond looking at both. The game's throw starts
  at 35 percent opacity and softer than the trailer's.
