# Runs one of the game's own test scripts (-ftexec) in the REAL game, UNCOOKED, from any checkout, and collects the
# frames its "shot" and "film" lines take. For staged scenes: a boss's entrance and attacks, an effect, a menu walk,
# a busy fight.
#   powershell -File jam_exec.ps1 -Tag <tag> -Script <file.txt> -Checkout <project folder>
#       [-BudgetSec 600] [-GraceSec 1800] [-ResX 3440 -ResY 1440] [-Extra "-dpcvars=name=value -someflag"] [-Small]
# The script file holds one command a line, as the game's runner takes them: any console command or FT* test command,
# "wait <seconds>" (real seconds), "waitfor <text>|<timeout s>" (the text of a game log line that begins "FT ",
# "FTBRAWL " or "FTINPUT ": the runner sees no others, so not FTTRACK, FTDRESS or FTBANK lines; a timeout does not
# stop the script), "shot <name>", "film <name> <frames> [fps]" (a frame sequence at a fixed step of game time:
# <name>_0001.png ...), and it should end with "quit".
# (jam_verify.ps1 and jam_play.ps1 write such files: read them for working lines.)
# The same script on two checkouts gives a before and an after: -Checkout "K:\Full Throttle\OverhaulBaseline\full-throttle-unreal"
# is today's game untouched (it has no "film" line: use "shot" there).
# Frames: SkyScratch/HandBack/jam/play/<Tag>/<name>.png (the folder's old frames are removed first, so what is there
# afterwards is this run's). Log: <project>/Saved/Logs/jam_exec_<Tag>.log (the lines that matter are printed).
# -Small films at 1720x720 (a quarter of the pixels: for frame sequences).
# Takes the Unreal job queue and the checkout's lock by itself. If the game does not end within the budget it says
# "TIMEOUT ... left running" at once, then KEEPS the queue slot and the checkout for as long as that game is still
# running (up to -GraceSec more), so that no other Unreal job is started on top of it; it never kills the game.
# Whoever called this must stop at a TIMEOUT line: never start another game behind one that has not ended.
# Start it so that it can outlive the wait in the queue, the budget and the grace (a background job, or as a step of
# jam_turn.ps1): if this script itself is stopped, its queue slot and its checkout lock count as free while the game
# may still be running, and GameUserSettings.ini is not put back.
param([Parameter(Mandatory=$true)][string]$Tag, [Parameter(Mandatory=$true)][string]$Script, [string]$Checkout = "",
      [int]$BudgetSec = 600, [int]$GraceSec = 1800, [int]$ResX = 3440, [int]$ResY = 1440, [string]$Extra = "", [switch]$Small, [string]$Engine = "K:\UE_5.9")
$ErrorActionPreference = "Stop"
# Runs that are already lined up (the steps of a jam_turn.ps1 turn) can be called off without stopping anything: a
# file jam_exec.skip beside this script, one tag pattern a line (wildcards), makes a run whose tag matches end at once.
$skipFile = Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Path) "jam_exec.skip"
if (Test-Path -LiteralPath $skipFile) {
  foreach ($pat in @(Get-Content -LiteralPath $skipFile | ForEach-Object { $_.Trim() } | Where-Object { $_ -and -not $_.StartsWith("#") })) {
    if ($Tag -like $pat) { "SKIPPED: tag '$Tag' is called off by jam_exec.skip ($pat)"; exit 0 }
  }
}
if (-not $Checkout) { $Checkout = if ($env:JAM_PORT) { $env:JAM_PORT } else { "K:\Full Throttle\UE\full-throttle-unreal" } }
if (-not (Test-Path -LiteralPath $Script)) { throw "no script at $Script" }
if ($Small) { $ResX = 1720; $ResY = 720 }
$proj = Join-Path $Checkout "FullThrottle.uproject"
$exe = Join-Path $Engine "Engine\Binaries\Win64\UnrealEditor.exe"
$saved = Join-Path $Checkout "Saved\FullThrottle"
. "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal\ue_lock.ps1"
. (Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Path) "jam_global.ps1")
$global = Enter-JamGlobal -Checkout $Checkout
$lock = $null; $gus = $null; $gusWas = $null; $gusTouched = $false; $appWas = $env:WISPOS_APP_DATA
try {
  $lock = Enter-UeLock -ProjectDir $Checkout
  # The frame's size: the game applies its saved settings at every map start, so the size goes into this checkout's
  # GameUserSettings.ini for the run and the file is put back afterwards (as jam_play.ps1 does).
  $gus = Join-Path $Checkout "Saved\Config\WindowsEditor\GameUserSettings.ini"
  $gusWas = if (Test-Path -LiteralPath $gus) { [IO.File]::ReadAllText($gus) } else { $null }
  $set = [ordered]@{ ResolutionSizeX = $ResX; ResolutionSizeY = $ResY; LastUserConfirmedResolutionSizeX = $ResX; LastUserConfirmedResolutionSizeY = $ResY
    DesiredScreenWidth = $ResX; DesiredScreenHeight = $ResY; LastUserConfirmedDesiredScreenWidth = $ResX; LastUserConfirmedDesiredScreenHeight = $ResY
    FullscreenMode = 2; LastConfirmedFullscreenMode = 2; PreferredFullscreenMode = 2 }
  # (made from nothing it needs its version line, or the engine takes it for an old file and resets the sizes)
  $txt = if ($gusWas) { $gusWas } else { "[/Script/Engine.GameUserSettings]`r`nVersion=5`r`n" }
  foreach ($k in $set.Keys) {
    if ($txt -match "(?m)^$k=") { $txt = [regex]::Replace($txt, "(?m)^$k=.*?(\r?)$", ("$k=" + $set[$k] + '$1')) }
    else { $txt = [regex]::Replace($txt, "(?m)^\[/Script/Engine\.GameUserSettings\]\r?\n", ('$0' + "$k=" + $set[$k] + "`r`n")) }
  }
  New-Item -ItemType Directory -Force (Split-Path -Parent $gus) | Out-Null
  [IO.File]::WriteAllText($gus, $txt); $gusTouched = $true
  $execFile = Join-Path $env:TEMP ("jam_exec_{0}.txt" -f $Tag)
  Copy-Item -LiteralPath $Script -Destination $execFile -Force
  # A scratch career and settings for the run, new every time (as jam_verify.ps1 does): the game keeps its saves and
  # its shots under WISPOS_APP_DATA when that is set, so two runs of one script start from the same state and the
  # checkout's own saved settings are never touched.
  $save = "K:\Full Throttle\SkyScratch\DevLog\jam\exec_save_$Tag"
  Remove-Item -Recurse -Force $save -ErrorAction SilentlyContinue
  New-Item -ItemType Directory -Force $save | Out-Null
  $env:WISPOS_APP_DATA = $save
  # this run's frames and log only: what an earlier run with the same tag left is removed first
  $shots = Join-Path $save "shots"
  $dst = "K:\Full Throttle\SkyScratch\HandBack\jam\play\$Tag"
  New-Item -ItemType Directory -Force $dst | Out-Null
  Get-ChildItem $dst -Filter *.png -ErrorAction SilentlyContinue | Remove-Item -Force -ErrorAction SilentlyContinue
  $log = Join-Path $Checkout "Saved\Logs\jam_exec_$Tag.log"
  Remove-Item -LiteralPath $log -Force -ErrorAction SilentlyContinue
  # the game's own no-HUD captures (the console variable ft.Shots) go to the project's Saved\Shots, not to the save folder
  $plain = Join-Path $Checkout "Saved\Shots"
  Get-ChildItem $plain -Filter "ft_L*.png" -ErrorAction SilentlyContinue | Remove-Item -Force -ErrorAction SilentlyContinue
  $a = @("`"$proj`"", "-game", "-windowed", "-ResX=$ResX", "-ResY=$ResY", "-RenderOffScreen", "-nosound", "-unattended", "-nosplash", "-ftexec=`"$execFile`"", "-abslog=`"$log`"")
  if ($Extra) { $a += $Extra }
  $t0 = Get-Date
  $p = Start-Process -FilePath $exe -ArgumentList ($a -join " ") -WorkingDirectory $Checkout -PassThru
  $null = $p.Handle
  if ($p.WaitForExit($BudgetSec * 1000)) { "exit " + $p.ExitCode + " after " + [int]((Get-Date) - $t0).TotalSeconds + " s" }
  else {
    "TIMEOUT after $BudgetSec s (left running, process " + $p.Id + ": do not start another game until it has ended by itself)"
    # the slot and the checkout stay taken while that game runs: nothing else may start on top of it
    if ($GraceSec -gt 0 -and $p.WaitForExit($GraceSec * 1000)) { "the game ended by itself " + [int]((Get-Date) - $t0).TotalSeconds + " s after its start (exit " + $p.ExitCode + ")" }
    else { "STILL RUNNING " + [int]((Get-Date) - $t0).TotalSeconds + " s after its start (process " + $p.Id + "): the queue slot and the checkout are given back now, with it still alive" }
  }
  if (Test-Path -LiteralPath $log) {
    Select-String -LiteralPath $log -Pattern "Fatal error|LogFT: Error|Ensure condition failed|result=timeout|FTTRACK level|FTDRESS level|FTBANK level|FT BOSS |FT SCREEN |FT EXEC film=|FT EXEC done|FT GO |FTBENCH " | Select-Object -Last 40 |
      ForEach-Object { $l = $_.Line; $i = $l.IndexOf("]Log"); if ($i -ge 0 -and $i -lt 40) { $l = $l.Substring($i + 1) }; $l.Substring(0, [Math]::Min(220, $l.Length)) }
  } else { "NO LOG at $log (the game never started?)" }
  if ((Select-String -LiteralPath $Script -Pattern '^\s*film ' -Quiet) -and -not ((Test-Path -LiteralPath $log) -and (Select-String -LiteralPath $log -Pattern 'FT EXEC film=' -Quiet))) {
    "NO FILM: the script has a film line and the game logged none (this checkout's built game has no film line, or the script never reached it)"
  }
  $got = @(Get-ChildItem $shots -Filter *.png -ErrorAction SilentlyContinue) + @(Get-ChildItem $plain -Filter "ft_L*.png" -ErrorAction SilentlyContinue)
  $got | Copy-Item -Destination $dst -Force
  "{0} frames -> {1}" -f $got.Count, $dst
  # a benchmark's result (-ftbench): the game writes it under its save folder
  $bench = @(Get-ChildItem (Join-Path $save "bench") -Filter *.json -ErrorAction SilentlyContinue)
  if ($bench.Count) { $bench | Copy-Item -Destination $dst -Force; "{0} benchmark file(s) -> {1}" -f $bench.Count, $dst }
} finally {
  $env:WISPOS_APP_DATA = $appWas
  if ($gusTouched) { if ($null -ne $gusWas) { [IO.File]::WriteAllText($gus, $gusWas) } else { Remove-Item -LiteralPath $gus -Force -ErrorAction SilentlyContinue } }
  if ($null -ne $lock) { Exit-UeLock $lock }
  Exit-UeLock $global
}
