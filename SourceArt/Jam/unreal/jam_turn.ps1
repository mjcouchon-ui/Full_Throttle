# ONE turn of the Unreal job queue for several jobs of one checkout, run one after another: one wait in the queue
# instead of one wait per job. (15 Nov: with eight workers on two slots a job waited 20 to 35 minutes for its turn,
# and a check made of four jobs spent two hours waiting.)
#   powershell -File jam_turn.ps1 -Checkout <project folder> -Steps <steps file> [-Name <name for the printed lines>]
# The steps file has one job a line, written as the arguments you would give PowerShell to run it, e.g.
#   -File "K:\Full Throttle\SkyScratch\SourceArt\Jam\unreal\jam_run.ps1" -Checkout "<project folder>" -Script "<job.py>" -Env "A=1;B=2"
#   -File "<project folder>\SourceArt\Jam\unreal\jam_verify.ps1" -Tag mytag -Checkout "<project folder>" -Levels "2,6"
# Empty lines and lines that start with # are skipped. A line that starts with ! is a step whose failure does not
# stop the turn (the steps after it still run).
# How it works: this script takes the queue slot and the checkout's lock ONCE and names the checkout in the
# environment variable JAM_HELD; Enter-JamGlobal and Enter-UeLock in the jobs it starts see that and go straight
# ahead (ue_lock.ps1, jam_global.ps1). Every job must be for THIS checkout: a job for another one would wait for
# that checkout's own lock as usual, with this turn's slot held meanwhile, so do not put one here.
# A step ends the turn early when it returns an error code or prints a line with TIMEOUT, "left running",
# "STILL RUNNING" or "COMPILE FAILED". Nothing is ever killed: if a game or an editor of this checkout is still
# running then, the slot and the lock are kept until it has ended by itself (up to -HoldSec), so that nothing else
# starts on top of it.
param([Parameter(Mandatory=$true)][string]$Checkout, [Parameter(Mandatory=$true)][string]$Steps, [string]$Name = "turn", [int]$HoldSec = 2400)
$ErrorActionPreference = "Stop"
if (-not (Test-Path -LiteralPath $Steps)) { throw "no steps file at $Steps" }
. "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal\ue_lock.ps1"
. (Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Path) "jam_global.ps1")
$lines = @(Get-Content -LiteralPath $Steps | ForEach-Object { $_.Trim() } | Where-Object { $_ -and -not $_.StartsWith("#") })
if (-not $lines.Count) { throw "no steps in $Steps" }
"turn '{0}': {1} step(s) for {2}; queued {3}" -f $Name, $lines.Count, $Checkout, (Get-Date -Format "HH:mm:ss")
function Get-CheckoutUnreal([string]$C) {
  @(Get-CimInstance Win32_Process -Filter "Name like 'UnrealEditor%'" -ErrorAction SilentlyContinue | Where-Object { ("" + $_.CommandLine).ToLower().Contains($C.TrimEnd('\').ToLower() + "\") })
}
$env:JAM_HELD = $null
$global = Enter-JamGlobal -Checkout $Checkout
$lock = $null; $failed = $false; $n = 0
try {
  $lock = Enter-UeLock -ProjectDir $Checkout
  $env:JAM_HELD = $Checkout
  "turn taken {0}" -f (Get-Date -Format "HH:mm:ss")
  foreach ($l in $lines) {
    $n++
    $soft = $l.StartsWith("!")
    $cmd = $l.TrimStart("!").Trim()
    "=== step {0} of {1}, {2}: {3}" -f $n, $lines.Count, (Get-Date -Format "HH:mm:ss"), $cmd.Substring(0, [Math]::Min(260, $cmd.Length))
    $so = Join-Path $env:TEMP ("jam_turn_{0}_{1}.out" -f $PID, $n); $se = Join-Path $env:TEMP ("jam_turn_{0}_{1}.err" -f $PID, $n)
    $t0 = Get-Date
    $p = Start-Process -FilePath "powershell.exe" -ArgumentList ("-NoProfile -ExecutionPolicy Bypass " + $cmd) -NoNewWindow -PassThru -RedirectStandardOutput $so -RedirectStandardError $se
    $null = $p.Handle
    $p.WaitForExit()
    $text = @(); if (Test-Path -LiteralPath $so) { $text += @(Get-Content -LiteralPath $so) }
    $errText = @(); if (Test-Path -LiteralPath $se) { $errText = @(Get-Content -LiteralPath $se | Where-Object { $_.Trim() }) }
    $text | ForEach-Object { $_ }
    if ($errText.Count) { "--- the step's error stream:"; $errText | Select-Object -First 30 | ForEach-Object { "  " + $_ } }
    Remove-Item -LiteralPath $so, $se -Force -ErrorAction SilentlyContinue
    $bad = ($p.ExitCode -ne 0) -or (@($text | Where-Object { $_ -match "TIMEOUT|left running|STILL RUNNING|COMPILE FAILED" }).Count -gt 0)
    "--- step {0} ended {1} after {2:N0} s, exit {3}{4}" -f $n, (Get-Date -Format "HH:mm:ss"), ((Get-Date) - $t0).TotalSeconds, $p.ExitCode, $(if ($bad) { ": NOT CLEAN" } else { "" })
    # whatever kind of step it was: no further job is started while an Unreal process of this checkout is still alive
    if (@(Get-CheckoutUnreal $Checkout).Count -gt 0) { $failed = $true; "TURN STOPPED at step ${n}: an Unreal process of this checkout is still running; the steps after it were not run"; break }
    if ($bad -and -not $soft) { $failed = $true; "TURN STOPPED at step ${n}: the steps after it were not run"; break }
  }
} finally {
  $env:JAM_HELD = $null
  # never give the slot back under a game or an editor of this checkout that is still running
  $t1 = Get-Date
  while (@(Get-CheckoutUnreal $Checkout).Count -gt 0 -and ((Get-Date) - $t1).TotalSeconds -lt $HoldSec) {
    if (((Get-Date) - $t1).TotalSeconds -lt 1) { "an Unreal process of this checkout is still running: keeping the slot and the lock until it ends" }
    Start-Sleep -Seconds 10
  }
  if (@(Get-CheckoutUnreal $Checkout).Count -gt 0) { "STILL RUNNING after $HoldSec s more: the slot and the lock are given back with it alive" }
  if ($lock) { Exit-UeLock $lock }
  Exit-UeLock $global
  "turn '{0}' over {1}: {2} of {3} step(s) run{4}" -f $Name, (Get-Date -Format "HH:mm:ss"), $n, $lines.Count, $(if ($failed) { ", STOPPED EARLY" } else { "" })
}
if ($failed) { exit 3 }
