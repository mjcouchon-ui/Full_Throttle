# Runs one of the game's own test scripts (-ftexec) in the REAL game, uncooked, on ANY machine: no job queue, no
# locks, no fixed paths (the jam_*.ps1 tools next door assume the lead's PC: K:\UE_5.9 and K:\Full Throttle\SkyScratch).
#   powershell -NoProfile -ExecutionPolicy Bypass -File run_exec.ps1 -Engine "<Unreal 5.9 folder>" -Project "<folder with FullThrottle.uproject>"
#       -Script <file.txt> -Out "<folder for the frames>" [-ResX 3440 -ResY 1440] [-BudgetSec 900] [-Extra " -ftnoattract -ftnodynres"]
# The game keeps its saves and its "shot" / "film" frames under the environment variable WISPOS_APP_DATA when that is
# set: this script points it at a new, empty folder for every run (so two runs start from the same state and no real
# save is touched) and copies the frames to -Out afterwards. The log goes to <Out>\run.log; the lines that matter are
# printed. If the game has not ended after -BudgetSec it says TIMEOUT and waits on (it never kills the game): close
# the game by hand if it truly hangs, and do not start another run before it has gone.
# -Extra: more command line for the game. Start the value with a space so that PowerShell does not take a leading
# dash for a parameter name. Run one game at a time when you compare timings or frame rates.
param([Parameter(Mandatory=$true)][string]$Engine, [Parameter(Mandatory=$true)][string]$Project, [Parameter(Mandatory=$true)][string]$Script,
      [Parameter(Mandatory=$true)][string]$Out, [int]$ResX = 3440, [int]$ResY = 1440, [int]$BudgetSec = 900, [string]$Extra = "")
$ErrorActionPreference = "Stop"
$exe = Join-Path $Engine "Engine\Binaries\Win64\UnrealEditor.exe"
$proj = Join-Path $Project "FullThrottle.uproject"
if (-not (Test-Path -LiteralPath $exe)) { throw "no editor at $exe" }
if (-not (Test-Path -LiteralPath $proj)) { throw "no project at $proj" }
if (-not (Test-Path -LiteralPath $Script)) { throw "no script at $Script" }
New-Item -ItemType Directory -Force $Out | Out-Null
Get-ChildItem $Out -Filter *.png -ErrorAction SilentlyContinue | Remove-Item -Force -ErrorAction SilentlyContinue
$save = Join-Path $Out "_save"
Remove-Item -Recurse -Force $save -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force $save | Out-Null
$log = Join-Path $Out "run.log"
Remove-Item -LiteralPath $log -Force -ErrorAction SilentlyContinue
# The game applies its saved window size at every map start: put the wanted size into this project's
# GameUserSettings.ini for the run and put the file back afterwards.
$gus = Join-Path $Project "Saved\Config\WindowsEditor\GameUserSettings.ini"
$gusWas = if (Test-Path -LiteralPath $gus) { [IO.File]::ReadAllText($gus) } else { $null }
$set = [ordered]@{ ResolutionSizeX = $ResX; ResolutionSizeY = $ResY; LastUserConfirmedResolutionSizeX = $ResX; LastUserConfirmedResolutionSizeY = $ResY
  DesiredScreenWidth = $ResX; DesiredScreenHeight = $ResY; LastUserConfirmedDesiredScreenWidth = $ResX; LastUserConfirmedDesiredScreenHeight = $ResY
  FullscreenMode = 2; LastConfirmedFullscreenMode = 2; PreferredFullscreenMode = 2 }
$txt = if ($gusWas) { $gusWas } else { "[/Script/Engine.GameUserSettings]`r`nVersion=5`r`n" }
foreach ($k in $set.Keys) {
  if ($txt -match "(?m)^$k=") { $txt = [regex]::Replace($txt, "(?m)^$k=.*?(\r?)$", ("$k=" + $set[$k] + '$1')) }
  else { $txt = [regex]::Replace($txt, "(?m)^\[/Script/Engine\.GameUserSettings\]\r?\n", ('$0' + "$k=" + $set[$k] + "`r`n")) }
}
New-Item -ItemType Directory -Force (Split-Path -Parent $gus) | Out-Null
[IO.File]::WriteAllText($gus, $txt)
$appWas = $env:WISPOS_APP_DATA
try {
  $env:WISPOS_APP_DATA = $save
  $a = @("`"$proj`"", "-game", "-windowed", "-ResX=$ResX", "-ResY=$ResY", "-RenderOffScreen", "-nosound", "-unattended", "-nosplash", "-ftexec=`"$Script`"", "-abslog=`"$log`"")
  if ($Extra) { $a += $Extra }
  $t0 = Get-Date
  $p = Start-Process -FilePath $exe -ArgumentList ($a -join " ") -WorkingDirectory $Project -PassThru
  $null = $p.Handle
  if (-not $p.WaitForExit($BudgetSec * 1000)) { "TIMEOUT after $BudgetSec s: waiting on for the game to end by itself (process " + $p.Id + ")"; $p.WaitForExit() }
  "exit " + $p.ExitCode + " after " + [int]((Get-Date) - $t0).TotalSeconds + " s"
  if (Test-Path -LiteralPath $log) {
    Select-String -LiteralPath $log -Pattern "Fatal error|LogFT: Error|Ensure condition failed|result=timeout|FTTRACK level|FTDRESS level|FTBANK level|FT BOSS state|FT SCREEN |FT EXEC film=|FT EXEC done|FT GO |FTBENCH " | Select-Object -Last 60 |
      ForEach-Object { $l = $_.Line; $i = $l.IndexOf("]Log"); if ($i -ge 0 -and $i -lt 40) { $l = $l.Substring($i + 1) }; $l.Substring(0, [Math]::Min(240, $l.Length)) }
  } else { "NO LOG at $log (the game never started?)" }
  $got = @(Get-ChildItem (Join-Path $save "shots") -Filter *.png -ErrorAction SilentlyContinue)
  $got | Copy-Item -Destination $Out -Force
  "{0} frames -> {1}" -f $got.Count, $Out
  $bench = @(Get-ChildItem (Join-Path $save "bench") -Filter *.json -ErrorAction SilentlyContinue)
  if ($bench.Count) { $bench | Copy-Item -Destination $Out -Force; "{0} benchmark file(s) -> {1}" -f $bench.Count, $Out }
} finally {
  $env:WISPOS_APP_DATA = $appWas
  if ($null -ne $gusWas) { [IO.File]::WriteAllText($gus, $gusWas) } else { Remove-Item -LiteralPath $gus -Force -ErrorAction SilentlyContinue }
}
