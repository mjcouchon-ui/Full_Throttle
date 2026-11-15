# Session acceptance probes

Build both Development Editor targets first. Run with PowerShell 7 and isolated profiles; never point a profile at a real player save. `Run-SessionProbe.ps1` sets a separate WISPOS_APP_DATA directory, the main test slot, and a separate standalone Brawl save. It records logs, screenshots and save snapshots and fails on runtime errors or wait timeouts. `-EngineRoot` overrides the UE installation. Use `-Audio` for actual audio-state checks; normal automated probes disable sound.

From this directory, choose new absolute `$out` and `$profile` paths for each sequence:

```powershell
foreach ($script in (Get-ChildItem scripts/main-0*-*.txt | Sort-Object Name)) {
  ./Run-SessionProbe.ps1 -Game main -Script $script.FullName -Output "$out/$($script.BaseName)" -Profile $profile -TimeoutSeconds 180 -Extra @('-ftday1','-ftdays')
}
./check-main-sequence.ps1 -RunsRoot $out
```

Run `brawl0*-*.txt` in numeric order with `-Game brawl`, another fresh shared profile, and the same timeout. Check with `python check-brawl-sequence.py --runs-root <out> --report <report.json>`. The five-case checker verifies checkpoint continuity, frozen pause pairs, preserved settings, real menu exits, explicit-discard file hashes, and replacement campaign identity. Its report distinguishes cancellation log/identity evidence from a before/after cancellation hash, which is not captured.

Run the two `night-0*-*.txt` scripts as main with another shared fresh profile, then `check-night-sequence.ps1`. `main-async-01` and `main-async-02` share a new career profile; the shop-feedback case uses its own fixture profile. `brawl-title-pref-failure.txt` is an independent fresh-profile probe: title settings retry succeeds, the later failed setting offers explicit discard, and no campaign is silently created.

The focused standalone delayed-write test uses a new profile and the documented expectations beside its script:

```powershell
./Run-SessionProbe.ps1 -Game brawl -Script scripts/brawl-delayed-failure-label.txt -Output "$out/brawl-delayed-failure-label" -Profile "$profile-delayed" -TimeoutSeconds 90 -Extra @('-brawlplay','-brawlnocut')
```

It captures real SAVING states, requests native close during a delayed write, cancels while the worker remains pending, checks that failure retains the old durable Continue label/timestamp, then retries and exits only after the new snapshot succeeds. `brawl-delayed-failure-label.md` lists required milestones and final file contents. The implementation keeps one active asynchronous write per owner, combines newer queued snapshots, and updates the durable descriptor only after successful completion. Accepted requests are not equivalent to persisted progress.

Run the five in-engine tests with `Automation RunTests FT.Save`:

- `FT.Save.AsyncQueueAndRollback`
- `FT.Save.AtomicFailureAndInterruptedWrite`
- `FT.Save.BackupAndUnsupportedVersion`
- `FT.Save.FirstLaunchAndMigration`
- `FT.Save.IdempotentRewardsAndPurchases`

They create unique temporary fixtures and cover migration, backups, unsupported versions, interrupted replacement, write failures, asynchronous ordering/coalescing/rollback, duplicate rewards, and successful/failed purchase, skill and equipment persistence. Warnings from deliberately failed or invalid fixtures are expected; failed test states are not.

Development-only controls:

- `ft.SaveFail 1` fails before replacement; `2` leaves an ignored verified temporary file; `0` restores writes.
- `ft.SaveDelayMs 2000` delays worker completion for responsive-menu and pending-exit checks. Reset to `0` for ordinary runs.
- `ft.WindowClose` / `ftb.WindowClose` invoke the viewport's actual native-close delegate.
- `ft.SessionState` / `ftb.SessionState` provide read-only pause/checkpoint snapshots; standalone includes staged versus durable save state and Continue text.
- `ft.AudioState` / `ftb.AudioState` report active audio-component pause/volume state; launch with `-Audio`.

Main scripts use `-ftexec=<file>`; standalone scripts use `-brawlexec=<file>`. Supported script commands include real key presses, mouse clicks, waits, log-based waits, screenshots, marks and console commands. Save fault/delay, state, audio-state and native-close QA commands are excluded from Shipping. Standalone `-brawlexec` is also Development-only. Main retains its existing `-ftexec`, `ft.Press` and screenshot harness in Shipping; packaged scripts therefore use timed real-menu input without fault injection or development state commands.

These probes inject keyboard, mouse and controller events; they do not constitute a physical-controller hardware test. Existing hooks shorten race completion and boss victory, while ordinary wave completion is also exercised. Real audio-state probes verify pause/resume and volume rather than subjective sound quality. Screenshot review complements log/file checks. Packaged Shipping validation is a separate acceptance step; see `Docs/autosave-session-flow.md` for evidence timing and final validation status.

Post-integration probes: run `merged-main-day-audio.txt` as main with `-Audio -Extra @('-ftday1','-ftdays')`, and `merged-brawl-day-audio.txt` as standalone with `-Audio`, each in a fresh profile. Require the new S_DayMusic track, paused flags 0/1/0, volume 0.8 throughout, and actual saved desktop exit. `lan-level-smoke.txt` visits all eight racing levels and their boss starts.

`shipping-main-first.txt` and `shipping-main-restart.txt` share one fresh isolated Shipping profile. Launch the actual packaged main executable through its existing WispOS ticket protocol, with `-ftexec=<script> -ftday1 -ftdays -ftslot=pie -ftnoattract`. Require natural process exit, inspect each screenshot, and compare `career_pie.json` before/after restart: Day 1 destination, currency and run count must agree. Shipping logging is disabled, so these scripts use timed waits and rendered/file evidence. Standalone Shipping has no equivalent input runner; its recorded smoke only verifies startup and a valid durable checkpoint, with full menus tested in Development.
Native PS4 USB evidence and limitations are in [PS4-CONTROLLER.md](PS4-CONTROLLER.md). `scripts/held-cross-title.txt` uses a fresh standalone profile and `check-held-cross.py <game.log> [report.json]` checks the map-travel held-confirm regression. `scripts/main-native-axis-menu.txt` is a separate simulated generic-axis deduplication test; require exact main title selections 2, 3, 4, 3. The physical USB test and disconnect automation passed; injected tests do not establish Bluetooth, rumble or native racing behavior.

Command-line UAT packaging/staging **must include `-prereqs`** to stage the GameInput runtime redistributable. Retain `IncludePrerequisites=True` for editor-driven packaging and verify the staged prerequisite files.
