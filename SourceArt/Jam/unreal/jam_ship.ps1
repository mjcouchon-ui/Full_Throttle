# A packaged SHIPPING build, level by level, on its own executable. The FT* harness commands are compiled out of
# Shipping, so each level is one launch in the game's benchmark mode (-ftbench: the autopilot drives the level for
# -ftseconds and the game writes a JSON report and quits) with a screenshot every few seconds (-ftshots). A level
# passes if the game exits by itself, the report is there and frames were taken. The career, settings, reports and
# frames go to a scratch folder (WISPOS_APP_DATA), never the player's own save in %LOCALAPPDATA%. A Shipping build
# only runs with WispOS's launch ticket: the script writes one into that scratch folder for each launch, as the
# launcher would (see the comment at the launch below).
#   powershell -File jam_ship.ps1 -Tag final_shipping -Root "K:\Full Throttle\PortBuilds\RC_Shipping\Windows" [-Seconds 25] [-Quality 3]
# Frames: SkyScratch/HandBack/jam/verify/<Tag>/L<level>_*.png.  Report: SkyScratch/DevLog/jam/ship_<Tag>.txt.
# The report's frame times are not a measurement unless the GPU was idle: the script says how many other Unreal
# processes were running.
param([Parameter(Mandatory=$true)][string]$Tag, [Parameter(Mandatory=$true)][string]$Root, [string]$Levels = "1,2,3,4,5,6,7,8", [int]$Seconds = 25,
      [int]$Quality = 3, [int]$ResX = 3440, [int]$ResY = 1440)
$ErrorActionPreference = "Stop"
$exe = Join-Path $Root "FullThrottle.exe"
if (-not (Test-Path $exe)) { throw "no packaged build at $exe" }
. "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal\ue_lock.ps1"
. (Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Path) "jam_global.ps1")
$names = @("Bayshore", "Chemical Works", "Nevada", "Volcano", "Theme Park", "Ice Mountain", "Under the Sea", "City Center")
$save = "K:\Full Throttle\SkyScratch\DevLog\jam\ship_save_$Tag"
Remove-Item -Recurse -Force $save -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force $save | Out-Null
$dst = "K:\Full Throttle\SkyScratch\HandBack\jam\verify\$Tag"
Remove-Item -Recurse -Force $dst -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force $dst | Out-Null
$out = New-Object System.Collections.Generic.List[string]
$others = @(Get-Process -Name UnrealEditor, FullThrottle -ErrorAction SilentlyContinue).Count
$out.Add(("# jam_ship {0}  {1}  quality {2}  {3}x{4}  {5}  (other Unreal processes running at the start: {6})" -f $Tag, (Get-Date -Format "yyyy-MM-dd HH:mm"), $Quality, $ResX, $ResY, $Root, $others))
$was = $env:WISPOS_APP_DATA
$bad = 0
$slot = Enter-JamGlobal -Checkout $Root
try {
  $env:WISPOS_APP_DATA = $save
  foreach ($l in @($Levels -split "[,\s]+" | Where-Object { $_ })) {
    $t = "{0}_L{1}" -f $Tag, $l
    Remove-Item -Recurse -Force (Join-Path $save "shots") -ErrorAction SilentlyContinue
    $a = @("-windowed", "-ResX=$ResX", "-ResY=$ResY", "-RenderOffScreen", "-nosound", "-unattended", "-ftbench", ("-ftlevel=" + ([int]$l - 1)), "-ftquality=$Quality",
           "-ftseconds=$Seconds", "-fttag=$t", "-ftshots=6")
    # A Shipping build runs only with WispOS's launch ticket (UFTGameInstance::Init): the launcher writes
    # $WISPOS_APP_DATA/launch-ticket and passes the same value in WISPOS_LAUNCH_TICKET; the game compares, deletes the
    # file and runs, or says "Open FULL THROTTLE from WispOS." and quits. This script stands in for the launcher.
    $ticket = [guid]::NewGuid().ToString("N")
    Set-Content -LiteralPath (Join-Path $save "launch-ticket") -Value $ticket -Encoding ascii -NoNewline
    $env:WISPOS_LAUNCH_TICKET = $ticket
    $t0 = Get-Date
    $p = Start-Process -FilePath $exe -ArgumentList ($a -join " ") -WorkingDirectory $Root -PassThru
    $null = $p.Handle
    $exited = $p.WaitForExit(($Seconds + 420) * 1000)
    $took = ((Get-Date) - $t0).TotalSeconds
    $json = @(Get-ChildItem (Join-Path $save "bench") -Filter "*$t.json" -ErrorAction SilentlyContinue)
    $shots = @(Get-ChildItem (Join-Path $save "shots") -Filter *.png -ErrorAction SilentlyContinue)
    $n = 0
    foreach ($s in $shots) { $n++; Copy-Item $s.FullName (Join-Path $dst ("L{0}_{1:00}.png" -f $l, $n)) -Force }
    $ok = $exited -and ($p.ExitCode -eq 0) -and ($json.Count -ge 1) -and ($shots.Count -ge 2)
    if (-not $ok) { $bad++ }
    $out.Add(("{0}  level {1} {2}: {3}; report {4}; frames {5}; {6:N0} s from launch to exit" -f $(if ($ok) { "PASS" } else { "FAIL" }), $l, $names[[int]$l - 1],
      $(if ($exited) { "exited with " + $p.ExitCode } else { "DID NOT EXIT (left running)" }), $(if ($json.Count) { "written" } else { "MISSING" }), $shots.Count, $took))
    if ($json.Count) { Copy-Item $json[0].FullName (Join-Path $dst ("L{0}_bench.json" -f $l)) -Force }
  }
} finally { $env:WISPOS_APP_DATA = $was; $env:WISPOS_LAUNCH_TICKET = $null; Exit-UeLock $slot }
$first = Get-ChildItem $dst -Filter *.png | Select-Object -First 1
if ($first) { Add-Type -AssemblyName System.Drawing; $im = [System.Drawing.Image]::FromFile($first.FullName); $out.Add(("frame size {0}x{1} (asked for {2}x{3}); frames -> {4}" -f $im.Width, $im.Height, $ResX, $ResY, $dst)); $im.Dispose() }
$out.Add(("levels not passing: {0}" -f $bad))
$rep = "K:\Full Throttle\SkyScratch\DevLog\jam\ship_$Tag.txt"
Set-Content -LiteralPath $rep -Value $out -Encoding utf8
$out
"-> $rep"
