# PS4 / DualShock 4 controller validation

Native **USB DualShock 4 revision 2 (Sony VID 054C / PID 09CC)** passed the focused physical standalone Brawl test on 2026-11-15. The user confirmed that the corrected flow “worked great.” This result covers the observed menu/story, held-confirm title return, disconnect/reconnect and desktop exit sequence below. Bluetooth, other PS4 controller models, physical rumble, a physical racing session and the complete Brawl combat button set were not tested in that sequence.

The `FTNativeGamepad` plugin owns only the configured Sony device. It prefers canonical GameInput Gamepad data when available and uses the configured generic-controller mapping as a fallback, with explicit neutral/release handling on disconnect. It selects one processor per device to avoid duplicate input. Xbox remains on its existing XInput path. The native result is not a claim that every PS4-compatible device is supported.

## Recorded evidence

Paths in this section are relative to the development workspace, outside the repository. Times are UTC from the logs.

| Check | Result and evidence |
|---|---|
| Physical standalone USB | `work/ps4-qa/physical-final-brawl.log`: custom plugin connected `DualShock4USB`, VID `054C`, PID `09CC`, platform user 0 / device 1. Continue opened Day 1; the physical Options button opened pause. |
| Held Cross on Quit to Title | Exit requested at 20:54:05.717; title screen 1 appeared at 20:54:05.943 and remained there while Cross was held. After release, `input_ready=1` appeared at 20:54:06.637. Later state snapshots remained screen 1 with frozen scene/world clocks. No automatic Continue occurred. |
| Held D-pad disconnect/reconnect | At 20:54:17.139 the plugin logged disconnect and released held input. Reconnection at 20:54:20.436 restored device 1; subsequent D-pad navigation and Cross confirmation worked. |
| Physical desktop exit | Title Quit to Desktop selected and confirmed at 20:54:25.978; normal save/exit path requested exit status 0. User confirmed the corrected behavior. |
| Disconnect automation | `FT.Input.NativeDualShockDisconnect` passed. This exercises input-state release; it supplements the physical reconnect evidence. |
| Held-confirm regression | `work/ps4-qa/held-cross-result.json`: **8/8 passed**. Injects a held confirm into the newly created controller, verifies stable paused title and clocks, then verifies a fresh press can Continue and the normal desktop exit works. |
| Main menu generic-axis regression | `scripts/main-native-axis-menu.txt`: exact observed level selections **2, 3, 4, 3** passed, including simultaneous analog/digital direction deduplication. This is simulated input, not a physical racing test. |
| Builds and packaging | Both final Development Editor targets and both Shipping packages succeeded. GameInput prerequisites were subsequently staged with UAT `-prereqs`. Packaged main runtime evidence is recorded separately from this Development physical test. |

The earlier report that Quit to Title restarted the cutscene was reproduced in the physical log: held Cross was reported as a fresh press on the replacement controller and selected Continue. Both main and Brawl controllers now wait through an initial settle period and for neutral menu input before accepting a new controller's first action. Ordinary movement between menu screens remains responsive.

## Packaging requirement

**Command-line UAT packaging/staging must include `-prereqs`.** Keep `IncludePrerequisites=True` in each project's packaging settings for editor-driven packaging. Verify the staged prerequisites include the GameInput redistributable; a successful compile or cook alone does not establish that the runtime prerequisite is available on another machine. The validated staged builds include it.

## Standalone Brawl button legend at physical validation

The following records the standalone build used for the physical test. The integrated game's controller/onboarding pass now uses family-aware prompts, Circle for kick/held-weapon throw, and Triangle for enemy grab/hurl. Its current layout is documented in [Controller onboarding](../../SourceArt/ControllerOnboarding/README.md). The historical test below does not certify the changed integrated combat layout.

The tested standalone build used generic/Xbox-style labels:

| Physical PS4 control | Unreal key | Brawl action |
|---|---|---|
| Cross | Gamepad_FaceButton_Bottom | A: confirm menus/story/end cards; jump |
| Circle | Gamepad_FaceButton_Right | B: back/cancel; grab or hurl |
| Square | Gamepad_FaceButton_Left | X: punch, pick up or swing; drink at the bar |
| Triangle | Gamepad_FaceButton_Top | Y: kick or throw a held weapon |
| Options | Gamepad_Special_Right | START: pause/resume or back from a submenu |
| Share | Gamepad_Special_Left | BACK: skip a story scene |
| Left stick / D-pad | Gamepad_LeftX/LeftY / Gamepad_DPad_* | Move in combat; navigate menus |

Brawl does not use L2/R2 or shoulder buttons for combat. Movement selects full direction past its dead zone; menu stick navigation has a threshold and repeat delay. Racing uses additional inputs, including R2 analog turbo, and needs its own physical acceptance when that scope is tested. Touchpad, gyro and PS button are not substitutes for Options or Share.

## Repeat the focused physical check

Use an isolated save profile in a rendered build with audio. For standalone, pass `-brawlsave=<absolute-isolated-directory>/brawl-campaign.json`. Test the native device without Steam Input, DS4Windows or another remapping layer. Record the controller model, transport, build and hardware IDs.

1. Continue a saved Day 1 scene with Cross. Press Options to pause; navigate the pause menu with D-pad and stick. A stationary mouse must not steal selection.
2. Choose Quit to Title, cancel once with Circle, then choose it again. Hold Cross through the confirmed transition. The title must stay visible; release Cross, then make a fresh press to Continue.
3. Return to title and hold a D-pad direction while unplugging USB. Reconnect, release the control, then navigate again. Confirm no stuck or duplicated input.
4. Choose Quit to Desktop and confirm with Cross. Require a normal process exit.

For broader future coverage, separately exercise the remaining combat buttons, native racing controls, rumble and any additional transport/model. Those are explicit limits of the current evidence, not prerequisites for accepting the focused USB fix above. Use a disposable profile for any New Game replacement test.

## Reproduce the automated regressions

Run `scripts/held-cross-title.txt` with standalone Development and a **fresh** isolated profile. Then run:

```powershell
python ./check-held-cross.py <run-directory>/game.log <report.json>
```

The script first holds Cross on the old controller, then promptly replays the held event on the new title controller to reproduce native polling across map travel. The checker requires that replay to occur before the neutral gate opens, so an injection that arrives too late cannot produce a false pass. It also checks two frozen title snapshots, no premature Continue, successful fresh confirmation, actual desktop exit and absence of runtime failures. UTF-8 and BOM-marked UTF-16 logs are supported.

Run `scripts/main-native-axis-menu.txt` as main Development with a fresh title profile. Require exactly `2,3,4,3` from the logged title level selections; its final fallback console quit is test cleanup, not desktop-menu exit evidence. Run `Automation RunTests FT.Input.NativeDualShockDisconnect` for the device-state regression.

## Development trace

Enable `ftb.InputTrace 1` in Development Brawl, or launch with `-ExecCmds="ftb.InputTrace 1"`; disable it with `ftb.InputTrace 0`. Default is off and the diagnostic is excluded from Shipping. It observes dispatch without consuming or remapping input.

```text
FTBRAWL INPUT source=platform simulated=0 device=<id> user=<id> controller=<id> key=Gamepad_FaceButton_Bottom event=<event> value=<value> timestamp=<cycles>
```

Press/release events are recorded; repeats are omitted and analog changes are rate limited with neutral crossings retained. `ft.Press`, `ftb.Press`, `-brawlpadtest` and other UE simulated events are excluded. A virtual OS controller can still emit platform events, so correlate the trace with the GameInput hardware record and the physical action. Device IDs alone do not identify a model or prove native support. Shipping acceptance uses visible outcomes and save files because this trace is unavailable there.

Final packaged main acceptance also passed: fresh title/New Game, pause, Quit to Title, normal desktop exit, application restart and Continue to Day 1. The saved run count stayed at one. All eight integrated racing maps/boss starts rendered and exited successfully after the first zone integration; later LAN revisions are recorded separately in the release note.
