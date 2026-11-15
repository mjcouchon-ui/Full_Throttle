# Loads and hitches during a race, counted per level: the real game (uncooked) drives every level on its autopilot,
# wrecking what it meets, then calls the boss and wraps. From the log, between each level's start and the command that
# leaves it:
#   - the engine's FlushAsyncLoading lines (a synchronous load on the game thread: something was read from disk mid-race)
#   - the game's FTHITCH lines (3.4: any frame over 40 ms, with the phase and the distance)
#   - the FT WARM lines (3.4: the background preload of the gameplay assets while the title is up)
# The same script against two builds gives a before and an after.
#   powershell -File jam_late.ps1 -Tag main [-Checkout <project folder>] [-Levels "1,2,3,4,5,6,7,8"] [-Drive 45] [-Extra "-someswitch"]
# Output: SkyScratch/DevLog/jam/late_<Tag>.txt. A hitch count means little while other Unreal jobs share the PC: say so.
param([Parameter(Mandatory=$true)][string]$Tag, [string]$Checkout = "", [string]$Levels = "1,2,3,4,5,6,7,8", [int]$Drive = 45, [string]$Extra = "",
      [string]$Engine = "K:\UE_5.9")
$ErrorActionPreference = "Stop"
if (-not $Checkout) { $Checkout = if ($env:JAM_PORT) { $env:JAM_PORT } else { "K:\Full Throttle\UE\full-throttle-unreal" } }
$proj = Join-Path $Checkout "FullThrottle.uproject"
$exe = Join-Path $Engine "Engine\Binaries\Win64\UnrealEditor.exe"
. "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal\ue_lock.ps1"
. (Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Path) "jam_global.ps1")
$lv = @($Levels -split "[,\s]+" | Where-Object { $_ })
# the title stays up for a few seconds first, as it does for a player: the preload runs behind it
$lines = @("# jam_late $Tag", "FTUseTestSlot", "FTQuality 1", "wait 6")
foreach ($l in $lv) {
  $lines += @("FTStartLevel $l", "waitfor FT LEVEL id=|240", "FTAutoRun wreck=all", ("waitfor FT CLOCK level={0}.0|150" -f $Drive),
              "FTPickup Crate", "FTPickup Wrench", "FTBossNow", "wait 12", "FTWrapNow", "wait 3")
}
$lines += "quit"
$execFile = Join-Path $env:TEMP ("jam_late_{0}.txt" -f $Tag)
Set-Content -LiteralPath $execFile -Value $lines -Encoding ascii
$log = Join-Path $Checkout "Saved\Logs\jam_late_$Tag.log"
$a = @("`"$proj`"", "-game", "-windowed", "-ResX=1280", "-ResY=720", "-RenderOffScreen", "-nosound", "-unattended", "-nosplash", "-ftexec=`"$execFile`"", "-abslog=`"$log`"")
if ($Extra) { $a += $Extra }
$save = "K:\Full Throttle\SkyScratch\DevLog\jam\late_save"   # a scratch career and settings, never the checkout's own
New-Item -ItemType Directory -Force $save | Out-Null
$was = $env:WISPOS_APP_DATA
$global = Enter-JamGlobal -Checkout $Checkout
$lock = Enter-UeLock -ProjectDir $Checkout
try {
  $env:WISPOS_APP_DATA = $save
  Remove-Item -LiteralPath $log -Force -ErrorAction SilentlyContinue
  $p = Start-Process -FilePath $exe -ArgumentList ($a -join " ") -WorkingDirectory $Checkout -PassThru
  $null = $p.Handle
  $budget = 600 + 240 * $lv.Count
  if (-not $p.WaitForExit($budget * 1000)) { "TIMEOUT after $budget s (left running: close it by hand if it never ends)" } else { "exit " + $p.ExitCode }
} finally { $env:WISPOS_APP_DATA = $was; Exit-UeLock $lock; Exit-UeLock $global }
if (-not (Test-Path -LiteralPath $log)) { "no log at $log"; exit 1 }
$out = New-Object System.Collections.Generic.List[string]
$out.Add("# jam_late $Tag  " + (Get-Date -Format "yyyy-MM-dd HH:mm") + $(if ($Extra) { "  ($Extra)" } else { "" }))
$level = ""; $racing = $false; $flush = 0; $hitch = 0; $worst = 0.0; $where = @(); $totalFlush = 0; $totalHitch = 0
function Close-Level {
  if ($script:level) {
    $script:out.Add(("{0}: synchronous loads during the race {1}; frames over 40 ms {2}{3}" -f $script:level, $script:flush, $script:hitch, $(if ($script:hitch) { " (worst {0:N0} ms)" -f $script:worst } else { "" })))
    foreach ($x in ($script:where | Select-Object -First 8)) { $script:out.Add("    " + $x) }
  }
}
foreach ($line in [IO.File]::ReadLines($log)) {
  if ($line -match "LogFT: (Display|Warning): FT WARM ") { $out.Add("  " + $line.Substring($line.IndexOf("FT WARM"))) }
  if ($line -match "LogFT: Display: FT LEVEL id=(\S+)") { Close-Level; $level = $Matches[1]; $racing = $true; $flush = 0; $hitch = 0; $worst = 0.0; $where = @(); continue }
  if ($line -match "FT EXEC line=\d+ cmd=(FTStartLevel|quit)") { $racing = $false }   # the next map's own loading is not counted
  if (-not $racing) { continue }
  if ($line -match "LogStreaming: Display: FlushAsyncLoading") { $flush++; $totalFlush++ }
  elseif ($line -match "LogFT: Display: FTHITCH ms=(\d+) (.*)$") { $hitch++; $totalHitch++; $worst = [Math]::Max($worst, [double]$Matches[1]); $where += ("{0,5} ms  {1}" -f $Matches[1], $Matches[2]) }
}
Close-Level
$out.Add(("TOTAL: synchronous loads during races {0}; frames over 40 ms {1}" -f $totalFlush, $totalHitch))
$dst = "K:\Full Throttle\SkyScratch\DevLog\jam\late_$Tag.txt"
Set-Content -LiteralPath $dst -Value $out -Encoding utf8
$out | Select-Object -Last 70
"-> $dst"
