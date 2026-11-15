# Session acceptance probes

Build both Development Editor targets first. Run these with PowerShell 7 and isolated profiles; never point a profile at a real player save. `Run-SessionProbe.ps1` sets a separate WISPOS_APP_DATA directory, the main test slot, and a separate standalone Brawl save. It records logs, screenshots and save snapshots and fails on runtime errors or wait timeouts. `-EngineRoot` overrides the UE installation. Use `-Audio` when checking actual sound; the automated checks normally run with sound disabled.

From this directory, choose a new absolute `$out` and `$profile` for every sequence:

```powershell
foreach ($script in (Get-ChildItem scripts/main-0*-*.txt | Sort-Object Name)) {
  ./Run-SessionProbe.ps1 -Game main -Script $script.FullName -Output "$out/$($script.BaseName)" -Profile $profile -TimeoutSeconds 180 -Extra @('-ftday1','-ftdays')
}
./check-main-sequence.ps1 -RunsRoot $out
```

Run `brawl0*-*.txt` in numeric order with `-Game brawl`, another fresh profile, and the same timeout. Check with `python check-brawl-sequence.py --runs-root <out> --report <report.json>`. Run the two `night-0*-*.txt` scripts as main with another shared fresh profile, then `check-night-sequence.ps1`. The independent `brawl-title-pref-failure.txt` uses its own fresh Brawl profile.

The scripts inject actual keyboard, mouse and controller key events. Existing development hooks shorten race completion and boss victory; ordinary wave completion is also exercised. They do not claim physical-controller hardware testing, packaged Shipping testing or audio-quality verification. Screenshot review complements the log/file checks.

Run the four in-engine subsystem tests with `Automation RunTests FT.Save`. They create unique temporary fixture files and cover migration, backups, unsupported versions, interrupted replacement, write failures, duplicate rewards, and successful/failed purchase, skill and equipment persistence.

Development-only commands: `ft.SaveFail 1` fails before replacement; `2` leaves an ignored verified temporary file; `0` restores writes. `ft.WindowClose` / `ftb.WindowClose` invoke the viewport's actual close delegate. `ft.SessionState` / `ftb.SessionState` provide read-only pause/checkpoint snapshots. Main scripts use `-ftexec=<file>`; standalone scripts use `-brawlexec=<file>`. These QA commands are excluded from Shipping builds.
