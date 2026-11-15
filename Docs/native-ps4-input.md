# Native PS4 input and title-return fix — 15 November 2026

Native USB DualShock 4 revision 2 support is enabled in both Full Throttle and standalone Brawl. The project plugin handles Sony VID 054C / PID 09CC through GameInput and leaves Xbox input on XInput. It chooses one input processor per device and releases held buttons, D-pad directions, sticks and triggers on focus loss or disconnect. GameInput prerequisites are included in both packaged builds; command-line packaging must use `-prereqs`.

Quit to Title now waits for the confirmation button to be released before accepting title input. This fixes the physical PS4 case where holding Cross through the transition immediately selected Continue and restarted the cutscene. Main title arrival has the same protection.

Validation:

- Both Development Editor targets and Win64 Shipping packages built successfully.
- The real USB controller started/continued Brawl, paused, navigated with the D-pad and stick, returned to title while Cross remained held, disconnected/reconnected while D-pad Down was held, and quit through the desktop menu. Justin confirmed the corrected sequence worked.
- `FT.Input.NativeDualShockDisconnect` passed; the held-confirm regression passed all eight assertions. Simulated main stick input produced exactly the expected 2, 3, 4, 3 selections without duplicate steps.
- Packaged main passed fresh New Game, pause, Quit to Title, desktop exit, restart and Continue with the expected Day 1 save and unchanged run count.
- Both archives contain the 2,920,448-byte `GameInputRedist.msi`, plus the Visual C++ prerequisites.
- Integrated GitHub through `326ca2c` and LAN through `10f6fb0`, including the photographer and latest zone updates. All eight racing maps/boss starts passed a smoke test. After the final LAN merge, the main Editor/Shipping build and focused Chemical Works, Nevada and Ice Mountain runtime smoke passed again, including title return and normal exit.

Physical validation covers the connected USB revision 2 controller. Bluetooth, other controller models, physical rumble, a full physical racing session and every combat action were not certified. Current generic A/B/X/Y hints correspond to Cross/Circle/Square/Triangle. The detailed evidence and reusable probes are in `Tools/SessionQA/PS4-CONTROLLER.md`.
