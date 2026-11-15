# The release-candidate check, one batched run per level in the real game (uncooked, or a packaged build with -Root):
# the autopilot drives each level wrecking what it meets (frames at fixed race-clock times), takes both pickups, calls
# the boss (three frames of the fight), wraps the level and films the highlight reel (the replay camera). From the log,
# per level: the track's fingerprint against SkyScratch/DevLog/jam/track_baseline.txt, the dressing's placement line,
# synchronous loads and frames over 40 ms during the race, and any error the game logged.
#   powershell -File jam_verify.ps1 -Tag rc_epic [-Quality 3] [-Levels "1,2,3,4,5,6,7,8"] [-Checkout <project folder>]
#   powershell -File jam_verify.ps1 -Tag rc_low -Quality 0
#   powershell -File jam_verify.ps1 -Tag cooked -Root "K:\Full Throttle\PortBuilds\Development\Windows"
# Frames: SkyScratch/HandBack/jam/verify/<Tag>/L<level>_<what>.png.  Report: SkyScratch/DevLog/jam/verify_<Tag>.txt.
# Hitch counts mean little while other Unreal jobs share the PC: the report says how many were running.
param([Parameter(Mandatory=$true)][string]$Tag, [string]$Checkout = "", [string]$Root = "", [string]$Levels = "1,2,3,4,5,6,7,8", [int]$Quality = 3,
      [string]$Times = "8,20,35,50", [int]$ResX = 3440, [int]$ResY = 1440, [string]$Engine = "K:\UE_5.9")
$ErrorActionPreference = "Stop"
if (-not $Checkout) { $Checkout = if ($env:JAM_PORT) { $env:JAM_PORT } else { "K:\Full Throttle\UE\full-throttle-unreal" } }
. "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal\ue_lock.ps1"
. (Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Path) "jam_global.ps1")
$lv = @($Levels -split "[,\s]+" | Where-Object { $_ })
$lines = @("# jam_verify $Tag", "FTUseTestSlot", "FTCareerReset", "FTQuality $Quality", "wait 6")
foreach ($l in $lv) {
  $lines += @("FTStartLevel $l", "waitfor FT LEVEL id=|240", "FTAutoRun wreck=all")
  foreach ($t in ($Times -split "[,\s]+" | Where-Object { $_ })) { $lines += @(("waitfor FT CLOCK level={0}.0|150" -f [int]$t), ("shot L{0}_t{1:000}" -f $l, [int]$t)) }
  $lines += @("FTPickup Crate", "wait 0.7", "shot L${l}_pickup_missiles", "wait 1.0", "FTDamagePlayer 30", "FTPickup Wrench", "wait 0.7", "shot L${l}_pickup_repair",
              "FTBossNow", "wait 2.5", "shot L${l}_boss_a", "wait 4", "shot L${l}_boss_b", "wait 5", "shot L${l}_boss_c",
              "FTWrapNow", "waitfor FT SCREEN name=Reel|40", "wait 4", "shot L${l}_reel_a", "wait 4", "shot L${l}_reel_b", "wait 1")
}
$lines += "quit"
$execFile = Join-Path $env:TEMP ("jam_verify_{0}.txt" -f $Tag)
Set-Content -LiteralPath $execFile -Value $lines -Encoding ascii
$packaged = [bool]$Root
if ($packaged) {
  $exe = Join-Path $Root "FullThrottle.exe"
  if (-not (Test-Path $exe)) { throw "no packaged build at $exe" }
  $work = $Root
  $logDir = Join-Path $Root "FullThrottle\Saved\Logs"
  $a = @("-windowed", "-ResX=$ResX", "-ResY=$ResY", "-nosound", "-log", "-RenderOffScreen", "-unattended", "-ftexec=`"$execFile`"")
  $queueAs = $Root
} else {
  $exe = Join-Path $Engine "Engine\Binaries\Win64\UnrealEditor.exe"
  $work = $Checkout
  $log = Join-Path $Checkout "Saved\Logs\jam_verify_$Tag.log"
  $a = @(("`"" + (Join-Path $Checkout "FullThrottle.uproject") + "`""), "-game", "-windowed", "-ResX=$ResX", "-ResY=$ResY", "-RenderOffScreen", "-nosound", "-unattended", "-nosplash",
         "-ftexec=`"$execFile`"", "-abslog=`"$log`"")
  $queueAs = $Checkout
}
$save = "K:\Full Throttle\SkyScratch\DevLog\jam\verify_save_$Tag"   # a scratch career and settings; the shots land here too
Remove-Item -Recurse -Force $save -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force $save | Out-Null
$was = $env:WISPOS_APP_DATA
$global = Enter-JamGlobal -Checkout $queueAs
$lock = if ($packaged) { $null } else { Enter-UeLock -ProjectDir $Checkout }
# the frame size: see jam_play.ps1 (the game re-applies its saved settings at every map start)
$gus = if ($packaged) { $null } else { Join-Path $Checkout "Saved\Config\WindowsEditor\GameUserSettings.ini" }
$gusWas = $null
$others = @(Get-Process -Name UnrealEditor, FullThrottle -ErrorAction SilentlyContinue).Count
try {
  if ($gus) {
    $gusWas = if (Test-Path -LiteralPath $gus) { [IO.File]::ReadAllText($gus) } else { $null }
    $set = [ordered]@{ ResolutionSizeX = $ResX; ResolutionSizeY = $ResY; LastUserConfirmedResolutionSizeX = $ResX; LastUserConfirmedResolutionSizeY = $ResY
      DesiredScreenWidth = $ResX; DesiredScreenHeight = $ResY; LastUserConfirmedDesiredScreenWidth = $ResX; LastUserConfirmedDesiredScreenHeight = $ResY
      FullscreenMode = 2; LastConfirmedFullscreenMode = 2; PreferredFullscreenMode = 2 }
    $txt = if ($gusWas) { $gusWas } else { "[/Script/Engine.GameUserSettings]`r`n" }
    foreach ($k in $set.Keys) {
      if ($txt -match "(?m)^$k=") { $txt = [regex]::Replace($txt, "(?m)^$k=.*?(\r?)$", ("$k=" + $set[$k] + '$1')) }
      else { $txt = [regex]::Replace($txt, "(?m)^\[/Script/Engine\.GameUserSettings\]\r?\n", ('$0' + "$k=" + $set[$k] + "`r`n")) }
    }
    New-Item -ItemType Directory -Force (Split-Path -Parent $gus) | Out-Null
    [IO.File]::WriteAllText($gus, $txt)
  }
  $env:WISPOS_APP_DATA = $save
  if (-not $packaged) { Remove-Item -LiteralPath $log -Force -ErrorAction SilentlyContinue }
  $p = Start-Process -FilePath $exe -ArgumentList ($a -join " ") -WorkingDirectory $work -PassThru
  $null = $p.Handle
  $budget = 600 + 300 * $lv.Count
  if (-not $p.WaitForExit($budget * 1000)) { "TIMEOUT after $budget s (left running: close it by hand if it never ends)" } else { "exit " + $p.ExitCode }
} finally {
  $env:WISPOS_APP_DATA = $was
  if ($gus) { if ($null -ne $gusWas) { [IO.File]::WriteAllText($gus, $gusWas) } else { Remove-Item -LiteralPath $gus -Force -ErrorAction SilentlyContinue } }
  if ($lock) { Exit-UeLock $lock }
  Exit-UeLock $global
}
if ($packaged) {
  $newest = Get-ChildItem $logDir -Filter *.log -ErrorAction SilentlyContinue | Sort-Object LastWriteTime -Descending | Select-Object -First 1
  if ($newest) { $log = Join-Path "K:\Full Throttle\SkyScratch\DevLog\jam" ("verify_{0}.log" -f $Tag); Copy-Item $newest.FullName $log -Force } else { $log = "" }
}
if (-not $log -or -not (Test-Path -LiteralPath $log)) { "no log"; exit 1 }
$dst = "K:\Full Throttle\SkyScratch\HandBack\jam\verify\$Tag"
Remove-Item -Recurse -Force $dst -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force $dst | Out-Null
Get-ChildItem (Join-Path $save "shots") -Filter *.png -ErrorAction SilentlyContinue | Copy-Item -Destination $dst
# ---- the report
# The baseline file holds three tables: FTTRACK (the editor binary's checksums), FTTRACK-PACKAGED (a packaged game's:
# the checksums are of raw numbers and differ between the two binaries) and FTTRACKSUM (length, box and means to a
# tenth of a centimetre: the same in both).
$base = @{}; $baseSum = @{}
$crcTag = if ($packaged) { "FTTRACK-PACKAGED" } else { "FTTRACK" }
foreach ($b in (Get-Content "K:\Full Throttle\SkyScratch\DevLog\jam\track_baseline.txt" -ErrorAction SilentlyContinue)) {
  if ($b -match "^$crcTag level=(\d+) (.*)$") { $base[[int]$Matches[1]] = $Matches[2].Trim() }
  if ($b -match "^FTTRACKSUM level=(\d+) (.*)$") { $baseSum[[int]$Matches[1]] = $Matches[2].Trim() }
}
$names = @("Bayshore", "Chemical Works", "Nevada", "Volcano", "Theme Park", "Ice Mountain", "Under the Sea", "City Center")
# Two summary lines are the same track if every length and position is within 2 cm and the mean up vector within
# 0.001: the editor binary and a packaged game drift apart by a few millimetres over 25 km (15 Nov: 0.5 cm at most),
# because the same code compiled two ways rounds differently and the route is summed step by step.
function Same-Sum([string]$a, [string]$b) {
  $na = @([regex]::Matches($a, "-?\d+\.\d+") | ForEach-Object { [double]$_.Value })
  $nb = @([regex]::Matches($b, "-?\d+\.\d+") | ForEach-Object { [double]$_.Value })
  if ($na.Count -ne 13 -or $nb.Count -ne 13) { return $false }
  for ($i = 0; $i -lt 13; $i++) { $tol = if ($i -lt 10) { 2.0 } else { 0.001 }; if ([Math]::Abs($na[$i] - $nb[$i]) -gt $tol) { return $false } }
  return $true
}
$out = New-Object System.Collections.Generic.List[string]
$out.Add(("# jam_verify {0}  {1}  quality {2}  {3}x{4}  {5}  (other Unreal processes running at the start: {6})" -f $Tag, (Get-Date -Format "yyyy-MM-dd HH:mm"), $Quality, $ResX, $ResY, $(if ($packaged) { "packaged: $Root" } else { "uncooked: $Checkout" }), $others))
$cur = -1; $racing = $false; $st = $null; $all = @{}
foreach ($line in [IO.File]::ReadLines($log)) {
  if ($line -match "LogFT: (Display|Warning): FT WARM ") { $out.Add("  " + $line.Substring($line.IndexOf("FT WARM"))) }
  if ($line -match "LogFT: Display: FTTRACK level=(\d+) (.*)$") {
    $cur = [int]$Matches[1]
    $st = @{ track = $Matches[2].Trim(); dress = ""; flush = 0; hitch = 0; worst = 0.0; errors = @(); boss = $false; reel = $false; pay = $false; stall = @(); sum = "" }
    $all[$cur] = $st; $racing = $false; continue
  }
  if ($null -eq $st) { continue }
  if ($line -match "LogFT: Display: FTDRESS level=\d+ (.*)$") { $st.dress = $Matches[1].Trim() }
  if ($line -match "LogFT: Display: FTTRACKSUM level=\d+ (.*)$") { $st.sum = $Matches[1].Trim() }
  if ($line -match "LogFT: Display: FT LEVEL id=") { $racing = $true }
  if ($line -match "FT EXEC line=\d+ cmd=(FTStartLevel|quit)") { $racing = $false }
  if ($line -match "LogFT: Display: FT BOSS state=Arrive") { $st.boss = $true }
  if ($line -match "LogFT: Display: FTSTALL (D=\S+ X=\S+ H=\S+)") { $st.stall += $Matches[1] }   # the autopilot stood still and was moved on
  if ($line -match "LogFT: Display: FT SCREEN name=Reel\s*$") { $st.reel = $true }
  if ($line -match "LogFT: Display: FT SAVE slot=career_pie reason=Paycheck result=ok") { $st.pay = $true }
  if ($line -match "Fatal error|LogFT: Error|Ensure condition failed|result=timeout") { if ($st.errors.Count -lt 6) { $st.errors += $line.Substring(0, [Math]::Min(200, $line.Length)) } }
  if ($racing) {
    if ($line -match "LogStreaming: Display: FlushAsyncLoading") { $st.flush++ }
    elseif ($line -match "LogFT: Display: FTHITCH ms=(\d+)") { $st.hitch++; $st.worst = [Math]::Max($st.worst, [double]$Matches[1]) }
  }
}
$bad = 0
# Only the levels that were asked for: the title screen builds Bayshore's track behind the menu, and that line used to
# be reported as "FAIL level 1" whenever level 1 was not in the run.
$asked = @($lv | ForEach-Object { [int]$_ - 1 } | Sort-Object)
foreach ($k in $asked) {
  if (-not $all.ContainsKey($k)) { $bad++; $out.Add(("FAIL  level {0} {1}: never built (no FTTRACK line for it in the log)" -f ($k + 1), $names[$k])); continue }
  $s = $all[$k]
  $crcKnown = $base.ContainsKey($k); $sumKnown = $baseSum.ContainsKey($k) -and $s.sum
  $crcSame = $crcKnown -and ($base[$k] -eq $s.track); $sumSame = $sumKnown -and (Same-Sum $baseSum[$k] $s.sum)
  # the track passes if every table that has this level agrees (and at least one has it)
  $same = ($crcKnown -or $sumKnown) -and (-not $crcKnown -or $crcSame) -and (-not $sumKnown -or $sumSame)
  $trackSays = if (-not ($crcKnown -or $sumKnown)) { "NO BASELINE " + $s.track } elseif ($same) { "unchanged" + $(if ($sumKnown -and -not $crcKnown) { " (by its summary, within 2 cm)" } else { "" }) } else { "CHANGED " + $s.track + " " + $s.sum }
  $frames = @(Get-ChildItem $dst -Filter ("L{0}_*.png" -f ($k + 1))).Count
  $ok = $same -and $s.boss -and $s.reel -and $s.pay -and ($s.errors.Count -eq 0) -and ($frames -ge 11)
  if (-not $ok) { $bad++ }
  $out.Add(("{0}  level {1} {2}: track {3}; boss {4}; reel {5}; pay banked {6}; frames {7}; loads mid-race {8}; frames over 40 ms {9}{10}" -f $(if ($ok) { "PASS" } else { "FAIL" }),
    ($k + 1), $names[$k], $trackSays, $s.boss, $s.reel, $s.pay, $frames, $s.flush, $s.hitch, $(if ($s.hitch) { " (worst {0:N0} ms)" -f $s.worst } else { "" })))
  $out.Add("        dressing: " + $s.dress)
  if ($s.sum) { $out.Add("        track: " + $s.track + " | " + $s.sum) }
  if ($s.stall.Count) { $out.Add("        the autopilot stalled " + $s.stall.Count + " time(s): " + (($s.stall | Select-Object -First 3) -join "; ")) }
  foreach ($e in $s.errors) { $out.Add("        ! " + $e) }
}
$first = Get-ChildItem $dst -Filter *.png | Select-Object -First 1
if ($first) { Add-Type -AssemblyName System.Drawing; $im = [System.Drawing.Image]::FromFile($first.FullName); $out.Add(("frame size {0}x{1} (asked for {2}x{3}); {4} frames -> {5}" -f $im.Width, $im.Height, $ResX, $ResY, (Get-ChildItem $dst -Filter *.png).Count, $dst)); $im.Dispose() }
$out.Add(("levels checked {0} of {1}; not passing {2}" -f @($asked | Where-Object { $all.ContainsKey($_) }).Count, $lv.Count, $bad))
$rep = "K:\Full Throttle\SkyScratch\DevLog\jam\verify_$Tag.txt"
Set-Content -LiteralPath $rep -Value $out -Encoding utf8
$out
"-> $rep"
