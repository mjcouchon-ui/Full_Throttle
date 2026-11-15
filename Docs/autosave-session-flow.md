# Autosave, Continue and session menus — 15 November 2026

Full Throttle now has checkpoint autosaves, Continue/New Game, and pause/exit menus across the racing career, integrated Brawl, and standalone Brawl. New Game confirms replacement of existing progress and preserves preferences.

Saved career data includes campaign position, completed races and bosses, rewards, currency, XP, pending and confirmed skills, equipment, upgrades, inventory, and progression flags. Purchases and equipment changes are saved when confirmed; failed writes retain the previous valid file and roll back unconfirmed transactions. Rewards and the following checkpoint are stored together, with attempt IDs preventing duplicate payouts.

Continue resumes at a supported boundary:

- Racing: the current race restarts from its beginning, including a race interrupted during its boss. Mid-race position, damage, score, and physics are not restored.
- Brawl: the saved wave or boss encounter restarts with full health, fresh enemies, and fresh pickups. Held items and temporary power-ups reset.
- Story: the current before-drive, wrap, introduction, or post-boss scene restarts from its beginning. A saved boss defeat resumes its outro instead of replaying the fight.
- Progression: preparation/shop checkpoints reopen their saved stage. Completed race rewards are not paid again by Continue.
- Standalone Brawl uses its own campaign save and retains its existing story-only route between fights; it does not load racing gameplay.

Escape or controller Start opens the system menu during play. Resume restores input and audio; Options, Quit to Title, and Quit to Desktop remain accessible. Exit messages identify the checkpoint when current race, combat, or scene timing would be lost. Failed saves offer Retry and an explicit Quit Without Saving for exits. Ordinary window-close requests use the same orderly exit path where practical.

Save files use validated temporary writes, atomic replacement, and a recoverable backup. Missing saves do not create a campaign automatically. Damaged or unsupported files remain untouched during loading; a valid backup can supply Continue. Unrecoverable files require a confirmed New Game before replacement.

Validation completed in Unreal Engine 5.9 on Windows:

- Both Development Editor targets compiled after the final fixes.
- All four FT.Save automation tests passed, covering migration, backup recovery, corrupt/unsupported data, actual failed replacement, interrupted writes, duplicate rewards, successful purchase/equipment/skill reloads, and failed transaction rollback.
- Fifteen application launches passed: five main-career scenarios, five standalone Brawl scenarios, two integrated boss/outro scenarios, and three focused preference-failure/backup/unsupported-save scenarios.
- Main career retained exactly $30,300 and 1,650 XP across reward presentation and application restart without another payout. Standalone explicit discard preserved primary save bytes. Confirmed New Game changed campaign identity while preserving preferences.
- Pause snapshots proved race, story and combat time/health stayed unchanged. Keyboard, mouse and injected controller-button events exercised menus, confirmation/cancellation, repeated title/Continue cycles, and the native window-close delegate.
- Screenshots were inspected for title, pause, options, replacement and save-failure readability.

Validation used Development builds and input injection, not a packaged Shipping build or physical-controller hardware. Most probes disabled audio; audio restoration was reviewed in code, not assessed by listening. The existing development VSM-overflow warning can still appear in the racing world. Track geometry and level layout were not changed.

Checkpoint limits are intentional: Continue restarts the supported race, wave, boss encounter or scene, rather than an arbitrary frame. Forced process termination cannot save new progress; the previous validated checkpoint and backup remain the recovery path. Small checkpoint writes run synchronously and unchanged checkpoints are coalesced; no full-game performance benchmark was run for this feature pass.

Reproducible acceptance scripts and isolated-save instructions are in `full-throttle-unreal/Tools/SessionQA/README.md`. Main and standalone saves remain separate. Existing valid legacy main-career saves migrate in memory and are rewritten only on the next explicit save operation.
