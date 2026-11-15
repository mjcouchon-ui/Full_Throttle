# The audit fixes of 15 Nov 2026 (SourceArt/Integration/AUDIT.md), proved in the real game (uncooked: the editor binary
# in -game mode) through the game's own harness. Five short launches, each with its own -ftexec script:
#   A  pickups at the cap say what was gained; the level's pay is banked before the highlight reel; quit during the reel
#   B  relaunch: the pay is still there; then a reshoot from THE DAILIES starts clean (no DOUBLE KILL on the first wreck)
#   C  a damaged career with a good backup beside it: the backup is used, and saving works
#   D  a damaged career and a damaged backup: a new career that can save, the damaged file kept
#   E  a career written by a newer build: never written over
# The career and settings live in a scratch folder (WISPOS_APP_DATA), never the checkout's own. Prints PASS / FAIL lines
# and writes them, with the log lines they rest on, to SkyScratch/DevLog/jam/audit_fixes/<Tag>.txt.
#   powershell -File jam_audit.ps1 [-Checkout <project folder>] [-Tag audit] [-Only "A,B"]
param([string]$Checkout = "", [string]$Tag = "audit", [string]$Only = "A,B,C,D,E", [string]$Engine = "K:\UE_5.9")
$ErrorActionPreference = "Stop"
if (-not $Checkout) { $Checkout = if ($env:JAM_PORT) { $env:JAM_PORT } else { "K:\Full Throttle\UE\full-throttle-unreal" } }
$proj = Join-Path $Checkout "FullThrottle.uproject"
$exe = Join-Path $Engine "Engine\Binaries\Win64\UnrealEditor.exe"
$outDir = "K:\Full Throttle\SkyScratch\DevLog\jam\audit_fixes"
$save = Join-Path $outDir "save_$Tag"
$shots = "K:\Full Throttle\SkyScratch\HandBack\jam\audit\$Tag"
. "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal\ue_lock.ps1"
. (Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Path) "jam_global.ps1")
$which = @($Only -split "[,\s]+" | Where-Object { $_ })
$report = New-Object System.Collections.Generic.List[string]
function Say([string]$s) { Write-Host $s; $script:report.Add($s) }   # (not the pipeline: Launch's output is its log)
function Check([string]$name, [bool]$ok, [string]$why) { Say ("{0}  {1}: {2}" -f $(if ($ok) { "PASS" } else { "FAIL" }), $name, $why) }
function Launch([string]$id, [string[]]$lines) {
  $execFile = Join-Path $env:TEMP ("jam_audit_{0}_{1}.txt" -f $Tag, $id)
  Set-Content -LiteralPath $execFile -Value $lines -Encoding ascii
  $log = Join-Path $Checkout ("Saved\Logs\jam_audit_{0}_{1}.log" -f $Tag, $id)
  Remove-Item -LiteralPath $log -Force -ErrorAction SilentlyContinue
  $a = @("`"$proj`"", "-game", "-windowed", "-ResX=1280", "-ResY=720", "-RenderOffScreen", "-nosound", "-unattended", "-nosplash", "-ftexec=`"$execFile`"", "-abslog=`"$log`"")
  $p = Start-Process -FilePath $exe -ArgumentList ($a -join " ") -WorkingDirectory $Checkout -PassThru
  $null = $p.Handle
  if (-not $p.WaitForExit(900 * 1000)) { Say "TIMEOUT launch $id (left running: stop it by hand if it never ends)" } else { Say ("launch {0}: exit {1}" -f $id, $p.ExitCode) }
  if (Test-Path -LiteralPath $log) { return ,@(Get-Content -LiteralPath $log) } else { return ,@() }
}
function Lines($log, [string]$pat) { return ,@($log | Where-Object { $_ -match $pat }) }
function Cash($line) { if ($line -match "cash=(-?\d+) cashEarned=(-?\d+)") { return [int64]$Matches[1] } else { return -1 } }
function Show($log, [string]$pat, [int]$n = 6) { foreach ($x in (@(Lines $log $pat) | Select-Object -First $n)) { $t = ("" + $x) -replace "^\[[^\]]*\]\[[^\]]*\]", ""; $script:report.Add("      " + $t.Substring(0, [Math]::Min(210, $t.Length))) } }

$global = Enter-JamGlobal -Checkout $Checkout
$lock = Enter-UeLock -ProjectDir $Checkout
$was = $env:WISPOS_APP_DATA
try {
  if ($which -contains "A") { Remove-Item -Recurse -Force $save -ErrorAction SilentlyContinue }
  New-Item -ItemType Directory -Force $save, $shots | Out-Null
  $env:WISPOS_APP_DATA = $save
  $career = Join-Path $save "career_pie.json"
  $start = @("FTUseTestSlot", "FTQuality 1")

  if ($which -contains "A") {
    Say "== A: pickups at the cap; pay banked before the reel; quit during the reel"
    $log = Launch "A" (@("FTUseTestSlot", "FTCareerReset", "FTQuality 1", "FTStartLevel 1", "waitfor FT LEVEL id=|240", "FTAutoRun wreck=0", "wait 4",
      "FTDumpLevel", "FTPickup Crate", "wait 0.8", "shot A1_crate_gain", "wait 1.6",
      "FTPickup Crate", "wait 0.8", "shot A2_crate_full", "wait 1.6",
      "FTPickup Wrench", "wait 0.8", "shot A3_wrench_full", "wait 1.6",
      "FTDamagePlayer 12", "FTPickup Wrench", "wait 0.8", "shot A4_wrench_gain", "FTDumpLevel",
      "FTSpawn Biker 2", "wait 0.6", "FTWreck all", "wait 2.5",
      "FTBossNow", "wait 0.5", "shot A5_boss_0s5", "wait 2.2", "shot A6_boss_2s7",
      "FTDumpCareer", "FTWrapNow", "waitfor FT RATING value=|60", "waitfor FT SCREEN name=Reel|30", "FTDumpCareer", "wait 1.0", "shot A7_reel", "quit"))
    $got = Lines $log "FT MISSILES count=\d+ pickup=Crate gained=(\d+)"
    $g = @($got | ForEach-Object { if ($_ -match "gained=(\d+)") { [int]$Matches[1] } })
    Check "crate below the cap" ($g.Count -ge 1 -and $g[0] -gt 0) ("first crate gained " + $(if ($g.Count) { $g[0] } else { "nothing logged" }))
    Check "crate at the cap" ($g.Count -ge 2 -and $g[1] -eq 0) ("second crate gained " + $(if ($g.Count -ge 2) { $g[1] } else { "nothing logged" }) + " (popup MISSILES FULL: see A2_crate_full.png)")
    Show $log "FT MISSILES count=\d+ pickup=Crate"
    $fa = Lines $log "FT BOSS firstAttack t1=1\.6 t2=1\.0 t3=6\.0"
    Check "the boss waits before its first attack" ($fa.Count -ge 1) ("FT BOSS firstAttack lines with t1=1.6 t2=1.0 t3=6.0: " + $fa.Count + " (frames A5_boss_0s5.png and A6_boss_2s7.png)")
    Show $log "FT BOSS firstAttack"
    $iRate = -1; $iReel = -1; $iSave = -1; $iDailies = -1
    for ($i = 0; $i -lt $log.Count; $i++) {
      if ($iRate -lt 0 -and $log[$i] -match "LogFT: Display: FT RATING value=") { $iRate = $i }
      if ($iRate -ge 0 -and $iSave -lt 0 -and $log[$i] -match "FT SAVE slot=career_pie reason=\w+ result=ok") { $iSave = $i }
      if ($iReel -lt 0 -and $log[$i] -match "FT SCREEN name=Reel\s*$") { $iReel = $i }
      if ($iDailies -lt 0 -and $log[$i] -match "FT SCREEN name=Dailies") { $iDailies = $i }
    }
    Check "rated and saved before the reel" ($iRate -ge 0 -and $iSave -gt $iRate -and $iReel -gt $iSave) ("log lines: rating $iRate, save $iSave, reel $iReel, dailies $iDailies (-1 = never reached: the game was closed during the reel)")
    $dumps = Lines $log "FT CAREER dump"
    $before = if ($dumps.Count -ge 1) { Cash $dumps[0] } else { -1 }
    $inReel = if ($dumps.Count -ge 2) { Cash $dumps[$dumps.Count - 1] } else { -1 }
    Check "cash banked while the reel plays" ($inReel -gt $before -and $before -ge 0) "cash before the wrap $before, during the reel $inReel"
    Show $log "LogFT: Display: FT RATING value=|FT PAY total=|FT SAVE slot=career_pie reason=Paycheck|LogFT: Display: FT SCREEN name=Reel\s*$"
    Set-Content -LiteralPath (Join-Path $save "cash_in_reel.txt") -Value $inReel
  }

  if ($which -contains "B") {
    Say "== B: relaunch after closing during the reel; then a reshoot from THE DAILIES"
    $inReel = if (Test-Path (Join-Path $save "cash_in_reel.txt")) { [int64](Get-Content (Join-Path $save "cash_in_reel.txt")) } else { -1 }
    $log = Launch "B" (@("FTUseTestSlot", "FTDumpCareer", "FTQuality 1", "FTStartLevel 1", "waitfor FT LEVEL id=|240", "FTAutoRun wreck=0", "wait 8",
      "FTSpawn Biker 2", "wait 0.6", "FTWreck all", "wait 2", "FTWrapNow", "waitfor FT SCREEN name=Reel state=hold|150", "FTPress Continue",
      "waitfor FT SCREEN name=Dailies|20", "wait 1.6", "shot B1_dailies", "ft.Press R", "waitfor FT SCREEN name=PreProduction next=L1|20", "wait 0.5", "FTPress Ready",
      "waitfor FT LEVEL id=|240", "wait 2", "FTSpawn Biker 1", "wait 0.6", "FTWreck all", "wait 1.5", "shot B2_reshoot_first_wreck", "FTDumpLevel", "quit"))
    $dumps = Lines $log "FT CAREER dump"
    $after = if ($dumps.Count) { Cash $dumps[0] } else { -1 }
    Check "pay kept after closing during the reel" ($after -eq $inReel -and $after -gt 0) "cash during the reel $inReel, after the relaunch $after"
    $lv = @(); for ($i = 0; $i -lt $log.Count; $i++) { if ($log[$i] -match "LogFT: Display: FT LEVEL id=") { $lv += $i } }   # (not the script runner's own lines, which quote it)
    $multi = @(); $first = @()
    for ($i = 0; $i -lt $log.Count; $i++) {
      if ($log[$i] -match 'FT STUNT name="(DOUBLE KILL|TRIPLE KILL|MASSACRE)"') { if ($lv.Count -ge 2 -and $i -gt $lv[1]) { $multi += $log[$i] } else { $first += $log[$i] } }
    }
    Check "the first take's double kill is real" ($first.Count -ge 1) ("multi-kill stunts in the first take: " + $first.Count)
    Check "reshoot starts clean" ($lv.Count -ge 2 -and $multi.Count -eq 0) ("level starts " + $lv.Count + ", multi-kill stunts after the reshoot's single wreck: " + $multi.Count)
    Show $log 'FT STUNT name="(DOUBLE KILL|TRIPLE KILL|MASSACRE)|LogFT: Display: FT SCREEN name=PreProduction next=|LogFT: Display: FT LEVEL id=|LogFT: Display: FT WRECK ' 12
  }

  if ($which -contains "C") {
    Say "== C: a damaged career, a good backup"
    # two saves, so there is a .bak (cash 4242) under the file (cash 5000); then the file is cut short
    $null = Launch "C0" (@("FTUseTestSlot", "FTCareerReset", "FTSetCash 4242", "FTSaveCareer", "FTSetCash 5000", "FTSaveCareer", "FTDumpCareer", "quit"))
    Check "a backup is kept beside the save" (Test-Path -LiteralPath ($career + ".bak")) ($career + ".bak")
    $bytes = [IO.File]::ReadAllBytes($career); [IO.File]::WriteAllBytes($career, $bytes[0..([int]($bytes.Length / 2))])
    $log = Launch "C" (@("FTUseTestSlot", "FTDumpCareer", "FTSetCash 4300", "FTSaveCareer", "FTLoadCareer", "FTDumpCareer", "FTGoto Title", "wait 1.5", "shot C1_title_banner", "quit"))
    $dumps = Lines $log "FT CAREER dump"
    Check "the backup is used" ((Lines $log "FT LOAD slot=career_pie recovery=backup").Count -ge 1 -and $dumps.Count -ge 1 -and (Cash $dumps[0]) -eq 4242) ("cash after the load: " + $(if ($dumps.Count) { Cash $dumps[0] } else { "no dump" }) + " (the backup held 4242)")
    Check "saving works after the recovery" ((Lines $log "FT SAVE slot=career_pie reason=Harness result=ok").Count -ge 1 -and $dumps.Count -ge 2 -and (Cash $dumps[$dumps.Count - 1]) -eq 4300) ("cash saved and loaded back: " + $(if ($dumps.Count -ge 2) { Cash $dumps[$dumps.Count - 1] } else { "no dump" }))
    Check "the damaged file is kept" (Test-Path -LiteralPath ($career + ".damaged")) ($career + ".damaged")
    Show $log "FT LOAD slot=career_pie|FT SAVE slot=career_pie reason=Harness"
  }

  if ($which -contains "D") {
    Say "== D: a damaged career and a damaged backup"
    [IO.File]::WriteAllText($career, "{ `"v`": 1, `"career`": { `"level`": 7, "); [IO.File]::WriteAllText($career + ".bak", "not json at all")
    $log = Launch "D" (@("FTUseTestSlot", "FTDumpCareer", "FTSetCash 77", "FTSaveCareer", "FTLoadCareer", "FTDumpCareer", "FTGoto Title", "wait 1.5", "shot D1_title_banner", "quit"))
    $dumps = Lines $log "FT CAREER dump"
    Check "a new career starts" ((Lines $log "FT LOAD slot=career_pie recovery=new_career").Count -ge 1 -and $dumps.Count -ge 1 -and (Cash $dumps[0]) -eq 0) ("cash after the load: " + $(if ($dumps.Count) { Cash $dumps[0] } else { "no dump" }))
    Check "the new career can save" ((Lines $log "FT SAVE slot=career_pie reason=Harness result=ok").Count -ge 1 -and $dumps.Count -ge 2 -and (Cash $dumps[$dumps.Count - 1]) -eq 77) ("cash saved and loaded back: " + $(if ($dumps.Count -ge 2) { Cash $dumps[$dumps.Count - 1] } else { "no dump" }))
    Show $log "FT LOAD slot=career_pie|FT SAVE slot=career_pie reason=Harness"
  }

  if ($which -contains "E") {
    Say "== E: a career from a newer build"
    $newer = "{ `"v`": 2, `"career`": { `"level`": 30, `"xp`": 123456 }, `"somethingNew`": true }"
    Remove-Item -LiteralPath ($career + ".bak"), ($career + ".damaged"), ($career + ".tmp") -Force -ErrorAction SilentlyContinue
    [IO.File]::WriteAllText($career, $newer)
    $log = Launch "E" (@("FTUseTestSlot", "FTSetCash 5", "FTSaveCareer", "FTGoto Title", "wait 1.5", "shot E1_title_banner", "quit"))
    Check "a newer save is refused, not replaced" ((Lines $log "FT LOAD slot=career_pie result=too_new").Count -ge 1 -and ([IO.File]::ReadAllText($career) -eq $newer)) "the file is byte for byte what the newer build wrote"
    Show $log "FT LOAD slot=career_pie|FT SAVE slot=career_pie"
  }

  Get-ChildItem (Join-Path $save "shots") -Filter *.png -ErrorAction SilentlyContinue | Copy-Item -Destination $shots -Force
  Say ("{0} frames -> {1}" -f (Get-ChildItem $shots -Filter *.png).Count, $shots)
} finally {
  $env:WISPOS_APP_DATA = $was
  Exit-UeLock $lock; Exit-UeLock $global
  New-Item -ItemType Directory -Force $outDir | Out-Null
  Set-Content -LiteralPath (Join-Path $outDir "$Tag.txt") -Value $report -Encoding utf8
}
