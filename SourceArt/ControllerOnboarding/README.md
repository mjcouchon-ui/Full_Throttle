# Controller support and first-play onboarding

This pass connects the racing and night-fight controls through one input-prompt system, adds controller-aware Controls screens, and introduces a short first-play driving lesson. The target is consistent physical action positions across both modes, with keyboard and mouse retained.

## Audit baseline

The starting main revision was `d771da5`. Source inspection established that the game already had basic gamepad gameplay support. Racing supported steering, speed adjustment, drift, turbo, guns, missiles, ammo cycling, and several menu actions. The night fights supported movement, punch, kick, jump, grab, cutscene progression, and result confirmation. Racing already provided two editable keyboard layouts and menu overlays.

The baseline did not provide a shared controller-family resolver, saved family override, complete night-fight pause/Controls flow, or persisted first-play driving tutorial. The night-fight kick and grab buttons also differed from the equivalent secondary/utility positions in racing. This was a source audit, not evidence of physical-controller coverage.

## Shared layout

Button positions are the stable contract. Changing the prompt family changes labels, not the bindings.

| Position | Xbox label | PlayStation label | Nintendo label | Racing | Night fights |
| --- | --- | --- | --- | --- | --- |
| West face | X | Square | Y | Fire guns | Punch / swing / pick up an item |
| East face | B | Circle | A | Launch missile | Kick / throw held weapon |
| South face | A | Cross | B | Drift | Jump |
| North face | Y | Triangle | X | Cycle ammo | Grab / throw an enemy |
| Left stick / D-pad | LS / D-pad | LS / D-pad | LS / D-pad | Steer; vertical input adjusts speed | Move through the fighting area |
| Right trigger | RT | R2 | ZR | Turbo | Unassigned |
| Left trigger | LT | L2 | ZL | Brake | Unassigned |
| Right shoulder | RB | R1 | R | Alternate drift | Unassigned |
| Left shoulder | LB | L1 | L | Alternate turbo | Unassigned |
| Right menu button | Menu | Options | + | Pause | Pause |
| Left menu button | View | Share | - | Skip active driving tutorial | Skip cutscene |

Menus use the south face button to confirm and east face button to go back. The left stick and D-pad navigate. The controller menu button opens/closes pause, including racing result and Pre-Production screens; it is not a Ready, night-fight attack, or cutscene-advance button. The north face button reshoots from racing Rating/Paycheck screens, activates Ready in Pre-Production, and replays a completed story cutscene. A held press must be released before it can activate the next screen.

The car accelerates forward automatically. Up increases its target speed; Down or the left trigger brakes. Drift requires steering while holding drift, then releasing after charge builds. Pickups are collected automatically by driving through them.

### Keyboard and mouse defaults

| Action | WASD layout | Arrow layout | Mouse |
| --- | --- | --- | --- |
| Racing steer / night movement | A/D; W/S for vertical movement | Arrow keys | — |
| Racing throttle up / brake | W / S | Up / Down | — |
| Drift | Shift | Shift | — |
| Turbo | Space | Space | — |
| Guns / punch / swing / collect item | J | Z | Left button |
| Missile / kick / throw held weapon | K | X | Right button |
| Cycle ammo / grab and throw enemy | L | C | Middle button cycles racing ammo |
| Night-fight jump | Space | Space | — |
| Pause | Esc; P also works in races | Esc; P also works in races | Pause-menu buttons |
| Confirm / back | Enter / Esc | Enter / Esc | Click menu button |
| Skip driving tutorial | F2 | F2 | Controls → Skip Current Tutorial |
| Skip night cutscene | Tab | Tab | — |
| Racing results reshoot / story replay | R | R | Screen controls where offered |
| Pre-Production Ready | Space | Space | Ready button |
| Racing results return to title | B | B | Pause → Quit to Title |

Movement and primary/secondary/utility keyboard bindings carry across modes. Keyboard steering selects the corresponding layout. Night-fight jump normally uses Space; if a remapped movement or combat action already occupies Space, the shared resolver chooses a free jump key and displays that same key in the HUD and Controls screen. Gamepad and mouse bindings are fixed in this pass.

## Menus and campaign sessions

The racing title presents Continue, New Game, Options, Quit Desktop, Play Selected, and Controls. Continue opens the saved checkpoint. New Game asks for replacement confirmation when a campaign exists and preserves controls, display/audio preferences, controller style, and first-play tutorial flags. Play Selected starts the highlighted shoot. Tab/right shoulder switches between shoot selection and the action buttons; G/View opens Pre-Production.

Racing Options includes Graphics Preset, Show Frame Rate, VSync, Reduced Motion, and Audio. Audio can be toggled with gamepad navigation and confirm or left/right, as well as the existing keyboard M shortcut. The shared mute setting carries between driving and fighting.

The racing pause rows are Resume, Options, Controls, Quit to Title, and Quit to Desktop, followed by Restart Race during a race's opening card, active play, wreck, or wrap. Retry Save appears last after a save error. Restart Race opens a confirmation with Cancel selected, then restarts the current race after confirmation. In the night fights, pause offers Resume, Controls, Restart Section, Options, Save and Quit to Title, and Save and Quit to Desktop. Restart Section also uses an explicit Cancel/Restart confirmation. Both modes retain keyboard/mouse navigation and family-aware gamepad prompts.

Pause now stops the world simulation and active gameplay audio in both modes. Menus and input processing remain available so a disconnected controller can be replaced by keyboard input. Closing a menu restores the previous audio/input state and requires held action buttons to be released before they affect gameplay or the next screen.

Quit to Title keeps the saved campaign checkpoint, and Continue restores it. Races restart from their beginning; night fights resume at the saved encounter boundary with fresh health/pickups; day scenes, night outros, and Pre-Production retain their supported story checkpoints. Arbitrary position, held items, active power-ups, and partially played scene timing are not serialized. Exit dialogs describe the checkpoint behavior before leaving. Save failures offer retry/cancel, or an explicit exit without saving where applicable; the previous successful save remains available.

Career saves run asynchronously. Leaving a screen, restarting, replaying the tutorial, or advancing the story waits for the required save to finish. The saving screen keeps a focused Stay Here/cancel option available; canceling navigation leaves the queued write running. Pre-Production disables purchase, undo, and Ready actions during a pending save and shows saving feedback until the result is known. Keyboard and gamepad cancellation use the current family's prompts.

## Input presentation and device handling

`UFTInputSubsystem` in `FTInput.h/.cpp` centralizes button names, gameplay prompt mappings, the active input source, controller-family overrides, and active-controller disconnect state. Both player-controller classes report input to it.

Auto classification uses the input device descriptor and hardware identifier exposed by Unreal. Recognized families are Xbox, PlayStation, and Nintendo; unknown devices receive positional face-button names. The exposed XInput interface takes precedence over possible physical-device names. Steam Input or another translation layer can hide the actual controller, so physical make/model identification is not guaranteed.

Controls offers Auto, Xbox, PlayStation, Nintendo, and Generic styles. Manual styles are useful when a backend's device information is incomplete. They do not remap controls; translation layers that alter face-button semantics require hardware verification.

The current Windows configuration keeps Xbox-compatible devices and XInput translations on Unreal's XInput path. The incoming `FTNativeGamepad` plugin adds a separate native GameInput path for the configured USB DualShock 4 revision 2, Sony VID `054C` / PID `09CC`. It selects one processor for that device, prefers canonical gamepad data with a configured controller fallback, and releases held controls on focus loss or disconnect. The shared prompt resolver can identify the exposed DualShock descriptor as PlayStation. This does not establish support for Bluetooth, other DualShock models, or native Nintendo devices; those still need a supported backend or translation layer. Translation can hide the physical family, so Auto/manual prompt styles remain available.

The incoming teammate's [native DS4 integration notes](../../Docs/native-ps4-input.md) and [physical-controller evidence](../../Tools/SessionQA/PS4-CONTROLLER.md) report a focused real USB revision-2 standalone Brawl check: menu/story navigation, held-Cross title return, held-D-pad disconnect/reconnect, and desktop exit. They also report Editor/Shipping builds and packaged main menu/save acceptance for that incoming revision. Those are attributed teammate results, not physical tests or Shipping certification performed by this controller/onboarding validation run. The incoming standalone combat-button legend predates this integrated shared layout; use the layout table above for the merged game. Physical racing, all combat actions, rumble, Bluetooth, and other models remain unverified here.

The earlier local XInput probe found no device in its four slots; it did not probe the native DS4 path. The final controller/onboarding batch uses synthetic input and descriptors and must be reported separately from the teammate's physical evidence. For command-line packaging of the merged native backend, include UAT `-prereqs` and verify that the staged prerequisites contain `GameInputRedist.msi`; `IncludePrerequisites=True` is enabled for editor-driven packaging.

Prompts follow deliberate button/key presses or sustained analog input. Mouse motion and small stick noise should not replace the active prompts. Controller disconnection flushes held keys and requests a recoverable pause when that controller was the active input source. Reconnection does not automatically resume gameplay. Keyboard input remains available for recovery.

Night-fight input also reconciles cached held keys when the application or game viewport loses focus, then waits for neutral input on return. Ordinary menu transitions retain their held-button guard, so confirming a menu cannot become an unintended combat action.

## Driving tutorial

The first ordinary Level 1 run starts six action-based lessons:

1. Steer deliberately.
2. Hold drift while steering, build at least one charge level, then release.
3. Use turbo briefly.
4. Fire the guns.
5. Collect a real missile or repair pickup.
6. Launch a missile.

Each card remains visible for at least four seconds and acknowledges success before advancing. The minimum full sequence is approximately 24 seconds; reading and practice normally extend it toward 30 seconds. There is no failure timer. Timers and gameplay events stop while the pause/Controls overlays are open.

For a slower learner who reaches the finale, the boss escape clock waits while lessons are active. Defeating the boss during practice defers the race result until the tutorial completes or is explicitly skipped, keeping the remaining actions available.

The tutorial protects the car from incoming damage while a lesson is active. During pickup practice it places a real crate ahead in the current lane and retries after a miss; it avoids placing practice crates on unsupported track sections. The missile lesson ensures one missile is available if the player has already spent their stock. Normal spawns and the existing combat/vehicle systems remain in use. Completion or skipping returns damage to normal after a short handover grace.

F2 or View/Share/minus skips the active lesson sequence. Controls also provides Skip Current Tutorial and Replay Tutorial / Level 1. Replay clears transient menu/story handoffs and starts Level 1 fresh without resetting cash, XP, skills, or gear; the active race checkpoint becomes Level 1. An unfinished lesson can restart on a fresh run or checkpoint Continue; an automatic retake retains its current objective. New Game preserves completed/skipped first-play flags, so experienced players use Replay Tutorial when they want the lesson again. Attract mode, benchmarks, and automatic driving runs do not begin the first-play lesson.

The night fights also have a short contextual control reminder, shown during the first fight and replayable from their Controls page. This is a reminder of movement and combat actions, not the racing tutorial.

## Persistence

The existing version-2 settings JSON stores these additive fields:

- `controllerPromptStyle`: 0 Auto, 1 Xbox, 2 PlayStation, 3 Nintendo, 4 Generic.
- `tutorialCompleted` and `tutorialSkipped`: first-play driving lesson disposition.
- `brawlHelpSeen`: whether the night-fight reminder has been shown.

Missing fields retain defaults, preserving compatibility with existing settings. Replay requests are session-only. The current save location remains `WISPOS_APP_DATA` when supplied, otherwise the project's `Saved/FullThrottle` directory. A save failure leaves the current session's state available but cannot guarantee persistence after relaunch.

The merged career save is version 2 and records the supported campaign checkpoint separately from these settings. Version-1 careers migrate in memory. Career writes use an ordered asynchronous queue, verify a temporary sibling before replacing the primary, retain a backup, and preserve unreadable/unsupported files for recovery. Rewards and the following checkpoint are committed together with a run identifier to prevent a continued result from paying twice. Screen transitions resume only after their required write completes; failures retain retry/cancel behavior. These career durability changes do not replace or reset the tutorial/settings file.

## Validation record

On 15 November 2026, Justin explicitly stopped the extended validation sweep to prioritize polishing and merging. The completed checks below are retained; remaining runtime scenarios were intentionally canceled rather than reported as passing.

Validated source: `189979ca27940f18000f0aab7a0232fc82e768d3`, including main `21bb7e1`, native USB DualShock input, the latest environment passes, controller onboarding, asynchronous saves, the logging compile fix, and focus-loss recovery.

- **FullThrottleEditor Win64 Development build: passed** on JV-WORK-PC with UE 5.9.
- **9/9 automation tests: passed**: input family/bindings, intentional input switching/disconnect, tutorial action progression, native DualShock disconnect release, first-launch save migration, backup/unsupported versions, interrupted/failed writes, reward/purchase persistence, and ordered asynchronous saves/rollback.
- **Final extended runtime sweep: stopped at Justin's request.** The batch had reached the racing scenario after automation. Its planned 294 runtime assertions are not a completed validation result.
- **Final combined Shipping package and full physical-controller playthrough: not performed in this pass.** The incoming native DS4 release's focused physical results are described above and remain separate from this integrated layout.

Authoritative remote evidence is under `\\JUSTINPC\FullThrottle\IntegrationReceipts\ControllerOnboarding\JVValidation\189979c`, available on JustinPC at `K:\Full Throttle\UE\IntegrationReceipts\ControllerOnboarding\JVValidation\189979c`. The parent `status.json` records the final cancellation and any checks that completed before stopping. `build.log` and `automation.log` establish the completed build and nine successful tests.

Earlier evidence remains in `K:\Full Throttle\IntegrationReceipts\ControllerOnboarding\FinalMerge` and `SessionMerge`. Historical runs exercised gameplay actions, tutorial completion in 25.3–25.4 active seconds, skip/replay/persistence, disconnect recovery, menu focus, and pause behavior. Reviewed Controls/tutorial captures were 3440 x 1440. These historical checks do not certify the latest native-input, asynchronous-save, focus-loss, or slow-learner endpoint changes. SessionMerge's interrupted-grab failure remains recorded; the isolated actual-grab/hurl fixture has compiled but has not completed the final runtime rerun.

The reusable scripts remain available for later focused checking. They use isolated saves through `WISPOS_APP_DATA`, synthetic input through real player-controller paths, actual action/screen-transition logs, and captured gameplay. Modes deliberately share a fresh evidence directory to test persistence. The current ordered batch is automation, race, brawl, persistence, controls, race_failure, race_extras, brawl_grab, day_cut, session_controls. Runtime test children scope `UE_SKIP_UBT_SDK_SETUP=1` to their own process after a successful build. These tests do not establish physical controller coverage or Shipping certification.
