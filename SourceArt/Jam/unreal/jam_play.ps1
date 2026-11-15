# The real game, UNCOOKED (the editor binary in -game mode) from any checkout, under that checkout's Unreal lock:
#   tour   frames at fixed race-clock times on the game's autopilot, level after level (like jam_tour.ps1, which
#          does the same against a packaged build)
#   bench  the game's own benchmark (-ftbench) on one level: fps, frame and GPU milliseconds as JSON
# The same build gives a before and an after: -NoDress builds the levels without the dressing (-ftnodress).
#   powershell -File jam_play.ps1 -Mode tour  -Tag after  -Checkout <project folder> [-Levels "1,4"] [-Times "8,20,35,50"] [-NoDress]
#   powershell -File jam_play.ps1 -Mode bench -Tag dense  -Checkout <project folder> -Levels "1" [-Seconds 25] [-Quality 3] [-NoDress]
# Tour frames: SkyScratch/HandBack/jam/play/<Tag>/L<level>_t<seconds>.png. Bench JSON: SkyScratch/DevLog/perf/jam/<Tag>_L<level>.json
# A benchmark means something only on an idle GPU: say so in the notes if other Unreal jobs were running.
param([ValidateSet("tour","bench")][string]$Mode = "tour", [Parameter(Mandatory=$true)][string]$Tag, [string]$Checkout = "",
      [string]$Levels = "1,2,3,4,5,6,7,8", [string]$Times = "8,20,35,50", [int]$Quality = 3, [int]$Seconds = 25,
      [int]$ResX = 3440, [int]$ResY = 1440, [switch]$NoDress, [string]$Engine = "K:\UE_5.9")
$ErrorActionPreference = "Stop"
if (-not $Checkout) { $Checkout = if ($env:JAM_PORT) { $env:JAM_PORT } else { "K:\Full Throttle\UE\full-throttle-unreal" } }
$proj = Join-Path $Checkout "FullThrottle.uproject"
$exe = Join-Path $Engine "Engine\Binaries\Win64\UnrealEditor.exe"
$saved = Join-Path $Checkout "Saved\FullThrottle"
. "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal\ue_lock.ps1"
. (Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Path) "jam_global.ps1")
$lv = @($Levels -split "[,\s]+" | Where-Object { $_ })
$common = @("`"$proj`"", "-game", "-windowed", "-ResX=$ResX", "-ResY=$ResY", "-RenderOffScreen", "-nosound", "-unattended", "-nosplash")
if ($NoDress) { $common += "-ftnodress" }
$global = Enter-JamGlobal -Checkout $Checkout   # the Unreal job queue (see jam_global.ps1)
$lock = Enter-UeLock -ProjectDir $Checkout
try {
  if ($Mode -eq "tour") {
    $lines = @("# jam_play tour $Tag", "FTQuality $Quality")
    foreach ($l in $lv) {
      $lines += @("FTStartLevel $l", "waitfor FT LEVEL id=|240", "FTAutoRun wreck=0")
      foreach ($t in ($Times -split "[,\s]+" | Where-Object { $_ })) { $lines += @(("waitfor FT CLOCK level={0}.0|150" -f [int]$t), ("shot L{0}_t{1:000}" -f $l, [int]$t)) }
    }
    $lines += "quit"
    $execFile = Join-Path $env:TEMP ("jam_play_{0}.txt" -f $Tag)
    Set-Content -LiteralPath $execFile -Value $lines -Encoding ascii
    Remove-Item -Recurse -Force (Join-Path $saved "shots") -ErrorAction SilentlyContinue
    $log = Join-Path $Checkout "Saved\Logs\jam_play_$Tag.log"
    $p = Start-Process -FilePath $exe -ArgumentList (($common + @("-ftexec=`"$execFile`"", "-abslog=`"$log`"")) -join " ") -WorkingDirectory $Checkout -PassThru
    $null = $p.Handle
    $budget = 900 + 200 * $lv.Count
    if (-not $p.WaitForExit($budget * 1000)) { "TIMEOUT after $budget s (left running: stop it by hand if it never ends)" } else { "exit " + $p.ExitCode }
    if (Test-Path $log) {
      Select-String -LiteralPath $log -Pattern "Fatal error|LogFT: Error|result=timeout|FTTRACK|FTDRESS level|FTBUILD|FTLIFE" | Select-Object -Last 40 | ForEach-Object { $_.Line.Substring(0, [Math]::Min(230, $_.Line.Length)) }
    }
    $dst = "K:\Full Throttle\SkyScratch\HandBack\jam\play\$Tag"
    New-Item -ItemType Directory -Force $dst | Out-Null
    Get-ChildItem (Join-Path $saved "shots") -Filter *.png -ErrorAction SilentlyContinue | Copy-Item -Destination $dst -Force
    "{0} frames -> {1}" -f (Get-ChildItem $dst -Filter *.png).Count, $dst
  } else {
    $dst = "K:\Full Throttle\SkyScratch\DevLog\perf\jam"
    New-Item -ItemType Directory -Force $dst | Out-Null
    foreach ($l in $lv) {
      $t = "{0}_L{1}" -f $Tag, $l
      $log = Join-Path $Checkout "Saved\Logs\jam_bench_$t.log"
      Get-ChildItem (Join-Path $saved "bench") -Filter "*$t.json" -ErrorAction SilentlyContinue | Remove-Item -Force
      $p = Start-Process -FilePath $exe -ArgumentList (($common + @("-ftbench", ("-ftlevel=" + ([int]$l - 1)), "-ftquality=$Quality", "-ftseconds=$Seconds", "-fttag=$t", "-abslog=`"$log`"")) -join " ") -WorkingDirectory $Checkout -PassThru
      $null = $p.Handle
      if (-not $p.WaitForExit(($Seconds + 900) * 1000)) { "TIMEOUT level $l (left running)"; continue }
      $json = @(Get-ChildItem (Join-Path $saved "bench") -Filter "*$t.json" -ErrorAction SilentlyContinue)
      if ($json.Count) { Copy-Item $json[0].FullName (Join-Path $dst "$t.json") -Force; "== $t"; Get-Content $json[0].FullName } else { "no benchmark JSON for $t" }
      if (Test-Path $log) { Select-String -LiteralPath $log -Pattern "FTDRESS level|FTBENCH|Fatal error" | Select-Object -Last 4 | ForEach-Object { $_.Line.Substring(0, [Math]::Min(230, $_.Line.Length)) } }
    }
  }
} finally { Exit-UeLock $lock; Exit-UeLock $global }
