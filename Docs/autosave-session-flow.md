# Autosave, Continue and session menus — 15 November 2026

Full Throttle has checkpoint autosaves, Continue/New Game, and pause/exit menus across the racing career, integrated Brawl, and standalone Brawl. New Game confirms replacement of existing progress and preserves preferences.

Saved career data includes campaign position, completed content, rewards, currency, XP, pending and confirmed skills, equipment, upgrades, inventory, and progression flags. Purchases and equipment changes join the save queue when confirmed. A failed transaction retains the previous valid file and restores the durable career state; rewards and their following checkpoint are stored together, with attempt IDs preventing duplicate payouts.

Continue resumes at a supported boundary:

- Racing: the current race restarts from its beginning, including a race interrupted during its boss. Mid-race position, damage, score, and physics are not restored.
- Brawl: the saved wave or boss encounter restarts with full health, fresh enemies, and fresh pickups. Held items and temporary power-ups reset.
- Story: the current before-drive, wrap, introduction, or post-boss scene restarts from its beginning. A saved boss defeat resumes its outro instead of replaying the fight.
- Progression: preparation/shop checkpoints reopen their saved stage. Completed race rewards are not paid again by Continue.
- Standalone Brawl uses its own campaign save and keeps its story-only route between fights; it does not load racing gameplay.

Continue and its displayed timestamp describe the last successfully persisted checkpoint. Staged progress never becomes the Continue destination merely because a write was accepted. If a new write fails, the old destination remains visible in the failure panel.

Writes run on a worker thread, using verified temporary files, atomic replacement, and a recoverable backup. Each save owner keeps one active write and combines newer queued snapshots; standalone campaign and preference writes share its ordered queue. The interface stays responsive and shows SAVING while work remains. Exit and New Game completion wait for the queue to drain successfully. Cancelling a queued exit cancels the exit request, while the already accepted save continues. A completion message appears only after success.

Escape or controller Start opens the system menu during play, story and result screens. Resume restores input and applicable audio. Options, Quit to Title, and Quit to Desktop remain accessible. Exit messages identify the supported checkpoint when current race, combat, or scene timing would be lost. Failed saves offer Retry and an explicit Quit Without Saving for exits, including preference failures from the standalone title. Ordinary window-close requests use the same orderly path where practical.

Loading a missing save does not create a campaign. Damaged or unsupported files remain untouched during loading; a valid backup can supply Continue and the title explains recovery. Unrecoverable files require a confirmed New Game before replacement. Standalone uses `Saved/SaveGames/brawl-campaign.json`, its `.bak`, and a separate `.settings` preference file; its test override is `-brawlsave=<path>`. Main and standalone saves remain separate. Valid legacy main-career saves migrate in memory and are rewritten only on a later explicit save.

The validation phase corrected stationary-cursor focus stealing, stale discarded-save errors, title preference retry/exit access, pending-write exits, and labels that previously described unsaved staged checkpoints. Save completion copy is now generic SAVED so preference-only writes do not imply a campaign checkpoint exists.

Validation evidence in Unreal Engine 5.9 on Windows:

- Both Development Editor targets compiled for the asynchronous implementation.
- All five `FT.Save` automation tests passed: `AsyncQueueAndRollback`, `AtomicFailureAndInterruptedWrite`, `BackupAndUnsupportedVersion`, `FirstLaunchAndMigration`, and `IdempotentRewardsAndPurchases`. Four report expected warnings from intentionally invalid/failing fixtures; none failed.
- The five ordered standalone restart scenarios passed 78 log/file checks. A delayed-write/error-label probe and real-audio pause probe passed 12 additional checks. Three further standalone runs passed title preference retry/discard, backup recovery, and incompatible-save cancellation: ten standalone scenarios in total, excluding superseded failed attempts.
- The ordered main-career and integrated-night scenarios passed, with additional asynchronous New Game/exit, purchase-feedback and real-audio probes. Reward evidence retained exactly $30,300 and 1,650 XP across presentation and restart without another payout.
- Standalone discard preserved primary bytes; confirmed New Game changed campaign identity and preserved preference bytes. Recovery and incompatible-load title/cancel probes left primary files unchanged. New Game cancellation was verified through input, no write in the cancellation interval and retained campaign identity; no intermediate cancellation hash is claimed.
- Pause snapshots showed race, story and combat time/health unchanged. Keyboard, mouse and injected controller buttons exercised menus, confirmations, repeated title/Continue cycles and the native close delegate. Real active music components were observed pausing and resuming; standalone fight music retained volume 0.8.
- Saving, failure, recovery, incompatible-save, options and confirmation screenshots were inspected for readable text, correct checkpoint/timestamp, disabled Continue when appropriate, safe default focus and explicit discard choices.

Final integration includes LAN revision `7cd8646` and GitHub `695d00b`, plus the subsequent `c44fcdf` staged-capture tooling update, preserving the teammate day-music, per-monitor DPI, first-launch copy, cut-bank and release-script updates. After integration and the final SAVED-footer change, both Development Editor targets and both Win64 Shipping packages built successfully. The main Shipping package passed first launch, New Game, pause, Save and Quit to Title, clean exit, application restart, Continue into the saved Day 1 scene, and Save and Quit to Desktop. Rendered screenshots and campaign JSON confirm the destination, unchanged run count and unchanged currency.

Standalone Shipping passed startup and a validated durable Day 1 checkpoint in an isolated profile. Its owned test process was stopped after verification; this is a startup/save smoke test, not packaged menu-navigation acceptance. Full standalone menu and orderly-exit coverage comes from the Development runs. Native desktop-control tools could not initialize against the LAN workspace, so no physical clicks in the standalone Shipping window are claimed. After the merge, both games also loaded the new `S_DayMusic` and demonstrated active audio pause/resume (0 -> 1 -> 0) at unchanged volume 0.8, followed by orderly saved desktop exits.

The broader automation and regression suite ran before the incoming music/DPI/copy merge. Affected scenes and final packages were checked again after it; unchanged save tests were not repeated. All eight integrated racing levels and bosses also completed a launch/render/pause/exit smoke before this final merge. The final `c44fcdf` change only updates the staged-capture PowerShell runner; it parsed successfully and did not change packaged game code or assets. No generated package, save profile or test screenshot is committed as game source.

No physical controller was connected during validation; controller inputs were injected. Audio state and volume restoration were checked, while subjective audio quality was not separately evaluated. No full-game performance benchmark was run for this feature pass. The existing development VSM-overflow warning can still appear in the racing world. Track geometry and level layout were not changed by this feature.

Checkpoint limits are intentional: Continue restarts the supported race, wave, boss encounter or scene, rather than an arbitrary frame. Forced process termination cannot save newly staged progress; the previous validated checkpoint and backup remain the recovery path.

Reproducible acceptance scripts and isolated-save instructions are in `full-throttle-unreal/Tools/SessionQA/README.md`.
