# Films the seven Brawl boss fights (the career's "nights") in the REAL game, UNCOOKED, from any checkout: one launch per
# night, straight to the boss (-brawlwave=4), on the Brawl's own autopilot (-brawlauto), no cutscenes (-brawlnocut), and
# hands back a fixed, named series of frames per night: the room, the entrance, the boss's signature move, a hit landed
# and a hit taken, his call for help at half armor, and his defeat (or Nova's knock-out when the fight is not won).
#   powershell -NoProfile -ExecutionPolicy Bypass -File brawl_bosses.ps1 -Tag <tag> [-Checkout <project folder>]
#       [-Nights "1,2,3,4,5,6,7"] [-BudgetSec 1200] [-FightSec 480] [-Full] [-ShotDt 0.2] [-Quality 3] [-Passive]
#       [-MaxRetries n] [-RealTime] [-NoKill] [-KeepRaw] [-OnScreen] [-Extra "-someflag"] [-DryRun]
# -DryRun prints each night's command line and starts nothing (no queue, no lock, no game).
# Before and after: the same call on two checkouts with two tags, e.g.
#   -Tag ovh_brawl_before -Checkout "K:\Full Throttle\OverhaulBaseline\full-throttle-unreal"
#   -Tag ovh_brawl_after  -Checkout "K:\Full Throttle\UE\full-throttle-unreal"
# Frames: SkyScratch/HandBack/jam/play/<Tag>/night<N>_<name>.png, and night<N>_manifest.txt beside them (which raw frame
# each name is, a [summary] of the encounter, and the fight's own log lines with their fight time).
# Log: <project>/Saved/Logs/brawl_bosses_<Tag>_night<N>.log.
#
# TWO BUILDS, ONE SCRIPT. The baseline checkout is OLDER code than today's main (checked 15 Nov): it has no hit-stop
# and no input buffer in the fight, no "FTBRAWL swing hit" line, no -brawlpolishqa ("FTBRAWL pose" lines), and its
# -ftexec runner cannot wait on FTBRAWL lines. So this script uses only what both have: the command-line flags
# -brawlnight -brawlwave -brawlauto -brawlnocut -brawlpassive -brawlnododge -brawlshots -brawlshotdt, the -ftexec
# lines "wait" and "quit", and the log lines named below. And because the two builds' fight code differs, the two
# fights are NOT the same fight blow for blow: frames pair by MOMENT (his entrance, his first signature move, ...),
# and "does the encounter still play the same" is answered by the two manifests' [summary] blocks (does he enter, does
# his move come, does he call for help, is he beaten, how often is Nova knocked out), not by equal [events].
#
# HOW THE FRAMES ARE CHOSEN. The Brawl has no "shot" command of its own, so the game is told to save its view every
# -ShotDt seconds of game time (-brawlshots=<dir> -brawlshotdt=<s>: <dir>/shot_NNN.png, logged as "FTBRAWL shot
# <path>"; the HUD is in the picture), and AFTER the run this script reads the log and picks the raw frames that stand
# next to the fight's own log lines:
#   01_arena        the last frame before "FTBRAWL boss <NAME>" (the boss's end of the room, Nova walking in)
#   02_entrance_a   0.4 s after that line   03_entrance_b  1.2 s   04_entrance_c  2.0 s  (his 2.2 s entrance, name card up)
#   05_fight_open   2.8 s after it (the fight has just begun)
#   06_sig_tell     the last frame before his first signature line (that line is logged when the tell ENDS)
#   07_sig_a        the first frame after it   08_sig_b  +0.4 s   09_sig_c  +0.9 s   10_sig_after  +1.8 s
#   11_boss_hit     the first frame after Nova's first hit on him ("swing hit ... on boss", a Verbose line of newer
#                   builds, or "weapon hit ... on boss"); on a build that logs neither (the baseline, bare fists): the
#                   last frame before "FTBRAWL boss calls 3 more" = just after the blow that took him to half armor.
#                   The manifest says which.
#   12_nova_hit_a   the first frame after the boss's first hit on Nova ("hurt by boss", Verbose; else "... hit nova")
#   13_nova_hit_b   0.4 s later
#   14_calls        0.6 s after "FTBRAWL boss calls 3 more" (half armor: his call and the three who answer)
#   15_t05 16_t10 17_t20   5, 10 and 20 s after the boss line (fixed moments, whatever is happening)
#   18_ko_fly       about 1 s before "FTBRAWL won" (the last blow: he is in the air)   19_ko_down  about 0.4 s before
#   20_won_a        the first frame after "FTBRAWL won"   21_won_b  +1.5 s   22_won_c  +3.0 s
#   23_nova_ko      the last frame before the first "FTBRAWL retry" (Nova knocked out), only when that happened
# The signature lines: night 1 "boss slam", 2 "boss whirlwind", 3 "boss revolver lane=", 4 "boss fire breath",
# 5 "boss strike lanes=", 6 "boss bear hug lunge", 7 "boss anchor sweep". A moment the log does not have gives no frame:
# it is listed as MISSING in the manifest and in this script's output, never filled with another frame.
#
# SAME FIGHT TWICE (on the SAME build). The fight runs in fixed 1/60 s steps on its own seeded random stream, but the
# autopilot reads the fight once per rendered FRAME, so at a free frame rate two runs drift apart. By default the engine
# is therefore run with -UseFixedTimeStep -FPS=60 -FixedSeed (one fight step per frame; game time is then NOT tied to
# the wall clock: the game runs as fast, or as slowly, as it can draw and save frames). The same build should then give
# the same fight, the same raw frame numbers and the same [summary] and [events]. In this mode a raw frame is saved
# every round(ShotDt x 60) rendered frames exactly. -RealTime turns all of that off.
# -Quality n (default 3 = EPIC) sets the scalability groups by -ExecCmds, because the FTQuality command needs a race
# game mode; -Quality -1 leaves each checkout's own settings.
# -Passive adds -brawlpassive -brawlnododge: Nova walks up to the boss, never hits him and does not step off his line,
# so the boss's moves play out and land; that fight ends at Nova's knock-out (use another tag for it).
# The career: the game is started with -ftslot=pie, so whatever it loads or saves is the test slot (career_pie), never
# the player's career. (FTUseTestSlot / FTCareerReset need a race game mode and do nothing on a night map.)
#
# HOW A NIGHT ENDS. Nothing in the Brawl quits the game, so: (1) the game gets a two-line -ftexec script, "wait
# <FightSec>" then "quit", which ends it by itself FightSec wall-clock seconds after its first frame whatever happens;
# (2) this script watches the log every 3 s and, once the fight is over (won and the frames after it are saved, or more
# than -MaxRetries knock-outs), ends the game process IT started (-NoKill: waits for (1) instead). The frames and the
# manifest are then cut at that same frame, so where the game happened to be stopped does not show in them.
# If the game has not ended within -BudgetSec it prints "TIMEOUT ... left running" and STOPS: no further night is
# started; never start another game until that process has ended by itself.
# Takes the Unreal job queue and the checkout's lock by itself, once, for all the nights asked for.
# -OnScreen leaves out -RenderOffScreen (only if a run saves no raw frames at all).
param([Parameter(Mandatory=$true)][string]$Tag, [string]$Checkout = "K:\Full Throttle\UE\full-throttle-unreal", [string]$Nights = "1,2,3,4,5,6,7",
      [int]$BudgetSec = 1200, [int]$FightSec = 480, [int]$ResX = 1720, [int]$ResY = 720, [switch]$Full, [double]$ShotDt = 0.2,
      [int]$Quality = 3, [switch]$Passive, [int]$MaxRetries = -1, [switch]$RealTime, [switch]$NoKill, [switch]$KeepRaw, [switch]$OnScreen,
      [switch]$DryRun, [string]$Extra = "", [string]$Engine = "K:\UE_5.9")
$ErrorActionPreference = "Stop"
$inv = [Globalization.CultureInfo]::InvariantCulture

# ---------------------------------------------------------------- the log and the frames
# The whole log as lines, read while the game still writes it.
function Read-BrawlLog([string]$Path) {
  $none = New-Object string[] 0
  if (-not (Test-Path -LiteralPath $Path)) { return ,$none }
  try {
    $share = [IO.FileShare]::ReadWrite -bor [IO.FileShare]::Delete
    $fs = New-Object IO.FileStream($Path, [IO.FileMode]::Open, [IO.FileAccess]::Read, $share)
    try { $sr = New-Object IO.StreamReader($fs, [Text.Encoding]::UTF8, $true); $text = $sr.ReadToEnd() } finally { $fs.Dispose() }
    return ,($text -split "`r?`n")
  } catch { return ,$none }
}

# Where the fight's moments are in the log (line numbers; -1 = not there), and where each saved frame is.
# Only log MESSAGES that start "FTBRAWL " count (": FTBRAWL " in the line), never an echo of a command line.
# Trace = the FTBRAWL lines both builds can log, with their fight time (frames so far x Dt); Poses = the "FTBRAWL pose"
# lines of -brawlpolishqa (newer builds only), kept apart so they do not flood a comparison of two builds.
# $LastLine >= 0: read no further than that log line (the cut at the end of the film).
function Get-BrawlEvents([string[]]$Lines, [string]$SigPattern, [double]$Dt, [int]$LastLine = -1) {
  $ev = @{ Shots = (New-Object 'System.Collections.Generic.List[int]'); ShotNo = (New-Object 'System.Collections.Generic.List[int]')
           Trace = (New-Object System.Collections.ArrayList); Poses = (New-Object System.Collections.ArrayList)
           RetryLines = (New-Object 'System.Collections.Generic.List[int]')
           Start = -1; Boss = -1; Sig = -1; SigCount = 0; BossHit = -1; BossHitBy = ""; SwingHits = 0
           NovaHit = -1; NovaHit2 = -1; NovaHurt = 0; NovaHurtBoss = 0
           Calls = -1; Won = -1; WonText = ""; Retry = -1; Retries = 0; Verbose = 0 }
  $ci = [Globalization.CultureInfo]::InvariantCulture
  $ord = [StringComparison]::Ordinal
  $last = $Lines.Count - 1
  if ($LastLine -ge 0 -and $LastLine -lt $last) { $last = $LastLine }
  for ($i = 0; $i -le $last; $i++) {
    $l = $Lines[$i]
    $j = $l.IndexOf(": FTBRAWL ", $ord)
    if ($j -lt 0) { continue }
    $t = $l.Substring($j + 10).TrimEnd()
    if ($l.IndexOf("Verbose: FTBRAWL ", $ord) -ge 0) { $ev.Verbose++ }
    if ($t.StartsWith("shot ")) { if ($t -match 'shot_(\d+)\.png') { $ev.Shots.Add($i); $ev.ShotNo.Add([int]$Matches[1]) }; continue }
    if ($t.StartsWith("sfx ") -or $t.StartsWith("rumble ")) { continue }
    $stamp = [string]::Format($ci, "t={0,7:0.0}  {1}", ($ev.Shots.Count * $Dt), $t)
    if ($t.StartsWith("pose ")) { [void]$ev.Poses.Add($stamp); continue }
    if ($t.StartsWith("swing hit ")) {
      # newer builds only (Verbose): kept out of the trace, used for 11_boss_hit
      if ($ev.Boss -ge 0 -and $t.EndsWith(" on boss")) { $ev.SwingHits++; if ($ev.BossHit -lt 0) { $ev.BossHit = $i; $ev.BossHitBy = "swing hit" } }
      continue
    }
    [void]$ev.Trace.Add($stamp)
    if ($ev.Start -lt 0 -and $t.StartsWith("start ")) { $ev.Start = $i }
    # "FTBRAWL boss BIG EARL" is the entrance (his name, in capitals); his moves are "boss slam", "boss calls 3 more" ...
    if ($ev.Boss -lt 0) { if ($t -cmatch '^boss [A-Z][A-Z]') { $ev.Boss = $i }; continue }
    if ($t -match $SigPattern) { $ev.SigCount++; if ($ev.Sig -lt 0) { $ev.Sig = $i } }
    if ($ev.BossHit -lt 0 -and $t -match '^weapon hit .+ on boss( |$)') { $ev.BossHit = $i; $ev.BossHitBy = "weapon hit" }
    if ($t.StartsWith("hurt by ")) {
      $ev.NovaHurt++
      if ($t.StartsWith("hurt by boss ")) { $ev.NovaHurtBoss++; if ($ev.NovaHit -lt 0) { $ev.NovaHit = $i } }
    }
    if ($ev.NovaHit2 -lt 0 -and $t -match '^(fire|anchor|ball|bullet) hit nova|^boss bear hug caught|^burned hp') { $ev.NovaHit2 = $i }
    if ($ev.Calls -lt 0 -and $t.StartsWith("boss calls 3 more")) { $ev.Calls = $i }
    if ($ev.Won -lt 0 -and $t.StartsWith("won ")) { $ev.Won = $i; $ev.WonText = $t }
    if ($t.StartsWith("retry wave")) { $ev.Retries++; $ev.RetryLines.Add($i); if ($ev.Retry -lt 0) { $ev.Retry = $i } }
  }
  return $ev
}

# How many frames were saved up to log line $LineIdx = the position (0-based) of the first frame after it.
function Get-ShotPos($Ev, [int]$LineIdx) {
  $c = 0
  foreach ($s in $Ev.Shots) { if ($s -gt $LineIdx) { break }; $c++ }
  return $c
}
function Get-Steps([double]$Sec, [double]$Dt) { return [int][Math]::Ceiling($Sec / $Dt - 0.001) }

# One named frame: $Sec after the log line (or, with $Before, the last frame before it and $Sec further back).
# The newest frame is never taken: its file may still be being written.
function Add-BrawlPick($List, $Ev, [double]$Dt, [string]$Name, [int]$LineIdx, [double]$Sec, [bool]$Before, [string]$Why, [string]$Note = "") {
  $pos = -1
  if ($LineIdx -ge 0) {
    $p0 = Get-ShotPos $Ev $LineIdx
    $n = Get-Steps $Sec $Dt
    if ($Before) { $pos = $p0 - 1 - $n } else { $pos = $p0 + $n }
    if ($pos -lt 0 -or $pos -gt ($Ev.Shots.Count - 2)) { $pos = -1 }
  }
  $no = 0
  if ($pos -ge 0) { $no = $Ev.ShotNo[$pos] }
  [void]$List.Add((New-Object psobject -Property @{ Name = $Name; Pos = $pos; No = $no; Why = $Why; Note = $Note; HasEvent = ($LineIdx -ge 0) }))
}

function Get-BrawlPicks($Ev, [double]$Dt, [string]$SigName) {
  $list = New-Object System.Collections.ArrayList
  $b = $Ev.Boss; $s = $Ev.Sig; $w = $Ev.Won
  $nh = $Ev.NovaHit
  if ($nh -lt 0) { $nh = $Ev.NovaHit2 }
  Add-BrawlPick $list $Ev $Dt "01_arena" $b 0 $true "the boss's entrance line"
  Add-BrawlPick $list $Ev $Dt "02_entrance_a" $b 0.4 $false "the boss's entrance line"
  Add-BrawlPick $list $Ev $Dt "03_entrance_b" $b 1.2 $false "the boss's entrance line"
  Add-BrawlPick $list $Ev $Dt "04_entrance_c" $b 2.0 $false "the boss's entrance line"
  Add-BrawlPick $list $Ev $Dt "05_fight_open" $b 2.8 $false "the boss's entrance line"
  Add-BrawlPick $list $Ev $Dt "06_sig_tell" $s 0 $true ("his signature line (" + $SigName + ")")
  Add-BrawlPick $list $Ev $Dt "07_sig_a" $s 0 $false ("his signature line (" + $SigName + ")")
  Add-BrawlPick $list $Ev $Dt "08_sig_b" $s 0.4 $false ("his signature line (" + $SigName + ")")
  Add-BrawlPick $list $Ev $Dt "09_sig_c" $s 0.9 $false ("his signature line (" + $SigName + ")")
  Add-BrawlPick $list $Ev $Dt "10_sig_after" $s 1.8 $false ("his signature line (" + $SigName + ")")
  if ($Ev.BossHit -ge 0) {
    Add-BrawlPick $list $Ev $Dt "11_boss_hit" $Ev.BossHit 0 $false "a hit on the boss" ("anchor: Nova's first hit on him (" + $Ev.BossHitBy + ")")
  } else {
    # a build that does not log Nova's hits: the blow that took the boss to half armor is just before his call
    Add-BrawlPick $list $Ev $Dt "11_boss_hit" $Ev.Calls 0 $true "a hit on the boss (no 'swing hit' / 'weapon hit ... on boss' line, and no 'boss calls 3 more' either)" "anchor: just before 'boss calls 3 more' (this build does not log Nova's hits)"
  }
  Add-BrawlPick $list $Ev $Dt "12_nova_hit_a" $nh 0 $false "a hit on Nova by the boss (hurt by boss, or ... hit nova)"
  Add-BrawlPick $list $Ev $Dt "13_nova_hit_b" $nh 0.4 $false "a hit on Nova by the boss (hurt by boss, or ... hit nova)"
  Add-BrawlPick $list $Ev $Dt "14_calls" $Ev.Calls 0.6 $false "boss calls 3 more (half armor)"
  Add-BrawlPick $list $Ev $Dt "15_t05" $b 5 $false "the boss's entrance line"
  Add-BrawlPick $list $Ev $Dt "16_t10" $b 10 $false "the boss's entrance line"
  Add-BrawlPick $list $Ev $Dt "17_t20" $b 20 $false "the boss's entrance line"
  Add-BrawlPick $list $Ev $Dt "18_ko_fly" $w 1.0 $true "won (the fight was not won)"
  Add-BrawlPick $list $Ev $Dt "19_ko_down" $w 0.4 $true "won (the fight was not won)"
  Add-BrawlPick $list $Ev $Dt "20_won_a" $w 0 $false "won (the fight was not won)"
  Add-BrawlPick $list $Ev $Dt "21_won_b" $w 1.5 $false "won (the fight was not won)"
  Add-BrawlPick $list $Ev $Dt "22_won_c" $w 3.0 $false "won (the fight was not won)"
  if ($Ev.Retry -ge 0) { Add-BrawlPick $list $Ev $Dt "23_nova_ko" $Ev.Retry 0 $true "retry" }
  return ,$list
}

# Where the film ends: the position of its last raw frame (Pos; -1 = the fight is not over yet, or that frame is not
# saved yet) and why. More than $MaxRetries knock-outs: one frame after that "retry" line. Won: the last frame a pick
# needs (3 s after "won", or 20 s after the boss came in if that is later) and two spare frames.
function Get-BrawlEnd($Ev, [double]$Dt, [int]$MaxRetries) {
  $n = $Ev.Shots.Count
  if ($Ev.RetryLines.Count -gt $MaxRetries) {
    $end = (Get-ShotPos $Ev $Ev.RetryLines[$MaxRetries]) + 1
    if ($end -le ($n - 1)) { return @{ Pos = $end; Why = "knock-outs" } }
    return @{ Pos = -1; Why = "" }
  }
  if ($Ev.Won -ge 0) {
    $need = (Get-ShotPos $Ev $Ev.Won) + (Get-Steps 3.0 $Dt)
    if ($Ev.Boss -ge 0) { $t20 = (Get-ShotPos $Ev $Ev.Boss) + (Get-Steps 20.0 $Dt); if ($t20 -gt $need) { $need = $t20 } }
    $end = $need + 2
    if ($end -le ($n - 1)) { return @{ Pos = $end; Why = "won" } }
  }
  return @{ Pos = -1; Why = "" }
}

function Format-BrawlTime($Ev, [int]$LineIdx, [double]$Dt) {
  if ($LineIdx -lt 0) { return "-" }
  return [string]::Format([Globalization.CultureInfo]::InvariantCulture, "{0:0.0}s", ((Get-ShotPos $Ev $LineIdx) * $Dt))
}

# ---------------------------------------------------------------- what was asked for
if ($Tag -notmatch '^[A-Za-z0-9_.-]+$') { throw "Tag: letters, digits, _ . - only (it names folders and files)" }
if ($Full) { $ResX = 3440; $ResY = 1440 }
if ($ShotDt -lt 0.05 -or $ShotDt -gt 1.0) { throw "ShotDt: 0.05 to 1.0 seconds" }
if ($Quality -lt -1 -or $Quality -gt 4) { throw "Quality: -1 (leave alone) or 0 to 4" }
if ($FightSec -lt 60) { throw "FightSec: at least 60" }
if ($BudgetSec -lt $FightSec + 120) { throw "BudgetSec must leave at least 120 s for the map load on top of FightSec ($FightSec)" }
if ($MaxRetries -lt 0) { $MaxRetries = if ($Passive) { 0 } else { 2 } }
$nightList = @($Nights -split '[,; ]+' | Where-Object { $_ -ne "" } | ForEach-Object { [int]$_ })
if ($nightList.Count -eq 0) { throw "Nights: nothing to film" }
foreach ($n in $nightList) { if ($n -lt 1 -or $n -gt 7) { throw "Nights: 1 to 7, got $n" } }

# night -> its map, its boss, his signature move, and the log line that marks it (FTBrawlGameMode.cpp)
$defs = @{
  1 = @{ Map = "L_RustyAnchor";    Boss = "BIG EARL";       Move = "GROUND SLAM";     Sig = '^boss slam' }
  2 = @{ Map = "L_VelvetRoom";     Boss = "VINNIE VELOUR";  Move = "DISCO WHIRLWIND"; Sig = '^boss whirlwind' }
  3 = @{ Map = "L_Lucky7";         Boss = "THE PIT BOSS";   Move = "THE REVOLVER";    Sig = '^boss revolver lane=' }
  4 = @{ Map = "L_TikiTorch";      Boss = "FLAMBE FRANKIE"; Move = "FIRE BREATH";     Sig = '^boss fire breath' }
  5 = @{ Map = "L_StrikeCity";     Boss = "THE KINGPIN";    Move = "STRIKE!";         Sig = '^boss strike lanes=' }
  6 = @{ Map = "L_FrostbiteLodge"; Boss = "BIG SVEN";       Move = "BEAR HUG";        Sig = '^boss bear hug lunge' }
  7 = @{ Map = "L_SeaBiscuit";     Boss = "THE COMMODORE";  Move = "ANCHOR SWEEP";    Sig = '^boss anchor sweep' }
}
$proj = Join-Path $Checkout "FullThrottle.uproject"
$exe = Join-Path $Engine "Engine\Binaries\Win64\UnrealEditor.exe"
if (-not (Test-Path -LiteralPath $proj)) { throw "no project at $proj" }
if (-not (Test-Path -LiteralPath $exe)) { throw "no engine at $exe" }
foreach ($n in $nightList) {
  $umap = Join-Path $Checkout ("Content\Brawl\Maps\" + $defs[$n].Map + ".umap")
  if (-not (Test-Path -LiteralPath $umap)) { throw "night $n has no map in this checkout: $umap" }
}
$dst = "K:\Full Throttle\SkyScratch\HandBack\jam\play\$Tag"
$rawRoot = Join-Path $env:TEMP "brawl_bosses_$Tag"
# How often the game saves a raw frame. With the fixed step a frame of the game is exactly 1/60 s and the game's timer
# counts whole frames up to its threshold, so the threshold is put half a frame BELOW the wanted count (12 frames for
# 0.2 s): a threshold exactly on it could come out as 12 or as 13 frames by float rounding. $dt = seconds of game time
# between two raw frames, which every offset below is counted in.
if ($RealTime) { $dt = $ShotDt; $gameDt = $ShotDt }
else {
  $shotFrames = [int][Math]::Max(1.0, [Math]::Round($ShotDt * 60.0))
  $dt = $shotFrames / 60.0
  $gameDt = ($shotFrames - 0.5) / 60.0
}
$dtText = $dt.ToString("0.####", $inv)
$gameDtText = $gameDt.ToString("0.#####", $inv)
$modeText = if ($Passive) { "passive" } else { "fight" }
$stepText = if ($RealTime) { "0" } else { "1" }
$qualityCmds = ""
if ($Quality -ge 0) {
  $aa = if ($Quality -le 1) { 2 } else { 4 }   # as the game's own presets: TAA at LOW and MEDIUM, TSR above (FTGameInstance.cpp ApplyQuality)
  $groups = @("ViewDistance", "AntiAliasing", "Shadow", "GlobalIllumination", "Reflection", "PostProcess", "Texture", "Effects", "Foliage", "Shading", "Landscape")
  $qualityCmds = "sg.ResolutionQuality 100," + (($groups | ForEach-Object { "sg.{0}Quality {1}" -f $_, $Quality }) -join ",") + ",r.AntiAliasingMethod $aa,r.DynamicRes.OperationMode 0"
}

# One night's launch: where its raw frames, its log and its two-line exit script go, and the game's command line.
function Get-BrawlLaunch([int]$N) {
  $d = $defs[$N]
  $raw = Join-Path $rawRoot "night$N"
  $log = Join-Path $Checkout ("Saved\Logs\brawl_bosses_{0}_night{1}.log" -f $Tag, $N)
  $execFile = Join-Path $env:TEMP ("brawl_bosses_{0}_night{1}.txt" -f $Tag, $N)
  $a = @("`"$proj`"", ("/Game/Brawl/Maps/" + $d.Map), "-game", "-windowed", "-ResX=$ResX", "-ResY=$ResY")
  if (-not $OnScreen) { $a += "-RenderOffScreen" }
  $a += @("-nosound", "-unattended", "-nosplash",
          "-brawlnight=$N", "-brawlwave=4", "-brawlauto", "-brawlnocut", "-brawlpolishqa",
          ("-brawlshots=`"" + $raw.Replace('\', '/') + "`""), "-brawlshotdt=$gameDtText",
          "-ftslot=pie", "-ftnodynres", "-ftexec=`"$execFile`"", "-LogCmds=`"LogFTBrawl Verbose`"", "-abslog=`"$log`"")
  if ($Passive) { $a += "-brawlpassive"; $a += "-brawlnododge" }
  if (-not $RealTime) { $a += "-UseFixedTimeStep"; $a += "-FPS=60"; $a += "-FixedSeed" }
  if ($qualityCmds) { $a += ("-ExecCmds=`"" + $qualityCmds + "`"") }
  if ($Extra) { $a += $Extra }
  return @{ Raw = $raw; Log = $log; Exec = $execFile; Cmd = ($a -join " ") }
}
if ($DryRun) {
  foreach ($n in $nightList) { $launch = Get-BrawlLaunch $n; "night $n " + $defs[$n].Boss + ":"; "  `"$exe`" " + $launch.Cmd }
  "one raw frame every $dtText s of game time (the game is given -brawlshotdt=$gameDtText); FightSec $FightSec, BudgetSec $BudgetSec a night"
  "frames would go to $dst (nothing was started)"
  exit 0
}

. "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal\ue_lock.ps1"
. "K:\Full Throttle\SkyScratch\SourceArt\Jam\unreal\jam_global.ps1"
$queue = $null; $lock = $null
$gus = Join-Path $Checkout "Saved\Config\WindowsEditor\GameUserSettings.ini"
$gusWas = $null; $gusSet = $false
$stopped = $false; $notRun = @()
$running = $null   # the game this script started and has not yet seen end
try {
  $queue = Enter-JamGlobal -Checkout $Checkout
  $lock = Enter-UeLock -ProjectDir $Checkout
  # The frame's size: the game applies its saved settings at every map start, so the size goes into this checkout's
  # GameUserSettings.ini for the run and the file is put back afterwards (as jam_exec.ps1 does). It is written again
  # before every night: a game that ends by itself may save the file on its way out.
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
  $gusSet = $true
  [IO.File]::WriteAllText($gus, $txt)
  New-Item -ItemType Directory -Force $dst | Out-Null

  foreach ($n in $nightList) {
    if ($stopped) { $notRun += $n; continue }
    $d = $defs[$n]
    $launch = Get-BrawlLaunch $n
    $raw = $launch.Raw; $log = $launch.Log
    Remove-Item -LiteralPath $raw -Recurse -Force -ErrorAction SilentlyContinue
    New-Item -ItemType Directory -Force $raw | Out-Null
    Remove-Item -LiteralPath $log -Force -ErrorAction SilentlyContinue
    if (Test-Path -LiteralPath $log) { throw "cannot clear $log (is a game from an earlier run still running?)" }
    # the game's own way out: it quits by itself FightSec wall-clock seconds after its first frame
    [IO.File]::WriteAllText($launch.Exec, "# brawl_bosses.ps1: the game ends itself, whatever the fight does`r`nwait $FightSec`r`nquit`r`n")
    [IO.File]::WriteAllText($gus, $txt)

    "night {0} {1}: started {2:HH:mm:ss} (budget {3} s)" -f $n, $d.Boss, (Get-Date), $BudgetSec
    $sw = [Diagnostics.Stopwatch]::StartNew()
    $p = Start-Process -FilePath $exe -ArgumentList $launch.Cmd -WorkingDirectory $Checkout -PassThru
    $null = $p.Handle
    $running = $p
    $state = ""; $saidUp = $false; $saidBoss = $false
    while ($true) {
      if ($p.WaitForExit(3000)) { $state = "exit"; break }
      $ev = Get-BrawlEvents (Read-BrawlLog $log) $d.Sig $dt
      if (-not $saidUp -and $ev.Start -ge 0) { $saidUp = $true; "  night {0}: the room is up after {1:0} s" -f $n, $sw.Elapsed.TotalSeconds }
      if (-not $saidBoss -and $ev.Boss -ge 0) { $saidBoss = $true; "  night {0}: the boss is in after {1:0} s" -f $n, $sw.Elapsed.TotalSeconds }
      if ((Get-BrawlEnd $ev $dt $MaxRetries).Pos -ge 0) { $state = "done"; break }
      if ($sw.Elapsed.TotalSeconds -gt $BudgetSec) { $state = "timeout"; break }
    }
    $how = ""
    if ($state -eq "exit") { $how = "the game ended by itself, exit " + $p.ExitCode }
    elseif ($state -eq "done") {
      if ($NoKill) {
        $ms = [int][Math]::Max(1000.0, ($BudgetSec - $sw.Elapsed.TotalSeconds) * 1000.0)
        if ($p.WaitForExit($ms)) { $how = "fight over, the game ended by itself, exit " + $p.ExitCode } else { $state = "timeout" }
      } else {
        # two seconds first: the last frames' files may still be on their way to the disk
        if (-not $p.WaitForExit(2000)) { try { $p.Kill() } catch { } }
        if ($p.WaitForExit(60000)) { $how = "fight over, game ended by this script" } else { $state = "timeout" }
      }
    }
    if ($state -eq "timeout") {
      $how = "TIMEOUT"
      "TIMEOUT after {0:0} s on night {1} (left running, process {2}: do not start another game until it has ended by itself)" -f $sw.Elapsed.TotalSeconds, $n, $p.Id
      $stopped = $true
    } else { $running = $null }

    # ---- the frames of this night
    Start-Sleep -Milliseconds 800
    $lines = Read-BrawlLog $log
    $ev = Get-BrawlEvents $lines $d.Sig $dt
    $rawCount = $ev.Shots.Count
    # the film is cut at its own last frame, wherever the game was stopped after it
    $end = Get-BrawlEnd $ev $dt $MaxRetries
    $cutText = "none (the log's end)"
    if ($end.Pos -ge 0) {
      $cutText = ("after shot_{0:D3} ({1})" -f $ev.ShotNo[$end.Pos], $end.Why)
      $ev = Get-BrawlEvents $lines $d.Sig $dt $ev.Shots[$end.Pos]
    }
    $picks = Get-BrawlPicks $ev $dt $d.Move
    Get-ChildItem -LiteralPath $dst -Filter ("night{0}_*" -f $n) -ErrorAction SilentlyContinue | Remove-Item -Force -ErrorAction SilentlyContinue
    $wonText = if ($ev.Won -ge 0) { "won " + (Format-BrawlTime $ev $ev.Won $dt) } else { "NOT WON" }
    $man = New-Object System.Collections.ArrayList
    [void]$man.Add("# brawl_bosses night $n : " + $d.Boss + ", " + $d.Move + "  (" + $d.Map + ")")
    [void]$man.Add("# tag=$Tag checkout=$Checkout")
    [void]$man.Add("# mode=$modeText fixedStep=$stepText rawFrameEvery=${dtText}s size=${ResX}x${ResY} quality=$Quality end=$how cut=$cutText")
    [void]$man.Add("# log=$log")
    [void]$man.Add("# Lines starting # differ between two runs by design. Two runs of the SAME build (fixed step) should give the same")
    [void]$man.Add("# [summary], [shots] and [events]. Two BUILDS fight differently in detail: compare their [summary] blocks.")
    [void]$man.Add("[summary]")
    [void]$man.Add("room up (FTBRAWL start): " + $(if ($ev.Start -ge 0) { "yes" } else { "NO" }))
    [void]$man.Add("boss enters: " + (Format-BrawlTime $ev $ev.Boss $dt))
    [void]$man.Add(("signature move ({0}): first {1}, times shown {2}" -f $d.Move, (Format-BrawlTime $ev $ev.Sig $dt), $ev.SigCount))
    [void]$man.Add(("Nova hurt (Verbose lines): {0} times, {1} by the boss himself; first by the boss {2}" -f $ev.NovaHurt, $ev.NovaHurtBoss, (Format-BrawlTime $ev $ev.NovaHit $dt)))
    [void]$man.Add("boss calls 3 more (half armor): " + (Format-BrawlTime $ev $ev.Calls $dt))
    [void]$man.Add("Nova knocked out: " + $ev.Retries + " times")
    [void]$man.Add("result: " + $wonText + $(if ($ev.WonText) { "  (" + $ev.WonText + ")" } else { "" }))
    [void]$man.Add("# Nova's hits on the boss (swing hit lines, newer builds only): " + $ev.SwingHits)
    [void]$man.Add("# Verbose FTBRAWL lines in the log: " + $ev.Verbose + $(if ($ev.Verbose -eq 0) { "  (none: -LogCmds had no effect, so 'Nova hurt' above counts nothing)" } else { "" }))
    [void]$man.Add("[shots]")
    $made = 0; $missing = @()
    foreach ($pk in $picks) {
      $file = "night{0}_{1}.png" -f $n, $pk.Name
      $src = ""
      if ($pk.Pos -ge 0) { $src = Join-Path $raw ("shot_{0:D3}.png" -f $pk.No) }
      if ($src -and (Test-Path -LiteralPath $src) -and ((Get-Item -LiteralPath $src).Length -gt 0)) {
        Copy-Item -LiteralPath $src -Destination (Join-Path $dst $file) -Force
        $made++
        $noteText = if ($pk.Note) { "  (" + $pk.Note + ")" } else { "" }
        [void]$man.Add([string]::Format($inv, "{0}  raw=shot_{1:D3}  t={2:0.0}{3}", $file, $pk.No, ($pk.Pos * $dt), $noteText))
      } else {
        $missing += $pk.Name
        $why = if (-not $pk.HasEvent) { "the log has no line for: " + $pk.Why } elseif ($pk.Pos -lt 0) { "no frame was saved at that moment" } else { "the frame's file is not there" }
        [void]$man.Add("$file  MISSING ($why)")
      }
    }
    [void]$man.Add("[events]")
    [void]$man.Add("# t = seconds of game time since the night began, to the nearest raw frame ($dtText s)")
    foreach ($e in $ev.Trace) { [void]$man.Add($e) }
    [void]$man.Add("[poses]")
    [void]$man.Add("# -brawlpolishqa: each sprite texture the first time it is really shown (newer builds only; empty on the baseline)")
    foreach ($e in $ev.Poses) { [void]$man.Add($e) }
    Set-Content -LiteralPath (Join-Path $dst ("night{0}_manifest.txt" -f $n)) -Value $man -Encoding UTF8

    "night {0} {1}: {2}; raw frames {3}; boss {4}; {5} {6} (x{7}); half armor {8}; {9}; knock-outs {10}; {11} of {12} frames" -f $n, $d.Boss, $how, $rawCount,
      (Format-BrawlTime $ev $ev.Boss $dt), $d.Move, (Format-BrawlTime $ev $ev.Sig $dt), $ev.SigCount, (Format-BrawlTime $ev $ev.Calls $dt), $wonText, $ev.Retries, $made, $picks.Count
    if ($ev.Start -lt 0) { "  night $n : the log has no 'FTBRAWL start' line: the night's map did not come up (see $log)" }
    elseif ($rawCount -eq 0) { "  night $n : the room came up but no raw frame was saved (-brawlshots): try -OnScreen (see $log)" }
    if ($ev.Start -ge 0 -and $ev.Verbose -eq 0) { "  night $n : no Verbose FTBRAWL lines in the log (-LogCmds had no effect): 12/13 fall back to the '... hit nova' lines" }
    if ($missing.Count -gt 0) { "  missing: " + ($missing -join ", ") }
    $lines | Where-Object { $_ -match 'Fatal error|LogFT: Error|Ensure condition failed|FT EXEC cannot read|: FTBRAWL no .* art' } | Select-Object -First 12 |
      ForEach-Object { $l = $_; $i = $l.IndexOf("]Log"); if ($i -ge 0 -and $i -lt 40) { $l = $l.Substring($i + 1) }; "  " + $l.Substring(0, [Math]::Min(220, $l.Length)) }
    if (-not $KeepRaw -and -not $stopped) { Remove-Item -LiteralPath $raw -Recurse -Force -ErrorAction SilentlyContinue }
  }
  if ($notRun.Count -gt 0) { "STOPPED after the timeout: nights not run: " + ($notRun -join ", ") }
  if (-not $KeepRaw -and -not $stopped) { Remove-Item -LiteralPath $rawRoot -Recurse -Force -ErrorAction SilentlyContinue }
  "{0} frames -> {1}" -f @(Get-ChildItem -LiteralPath $dst -Filter "night*_*.png").Count, $dst
} finally {
  # This script failed or was stopped (Ctrl+C) while its game was still up: the locks are about to be given back, so
  # the game it started must not stay behind them. (After a TIMEOUT the game is left alone, as announced.)
  if ($running -and -not $stopped) {
    try {
      if (-not $running.HasExited) {
        if ($NoKill) { "LEFT RUNNING: process " + $running.Id + " (this script stopped early): do not start another game until it has ended by itself" }
        else { "this script stopped early: ending the game it started (process " + $running.Id + ")"; $running.Kill(); [void]$running.WaitForExit(60000) }
      }
    } catch { }
  }
  if ($gusSet) {
    if ($null -ne $gusWas) { [IO.File]::WriteAllText($gus, $gusWas) } else { Remove-Item -LiteralPath $gus -Force -ErrorAction SilentlyContinue }
  }
  Exit-UeLock $lock; Exit-UeLock $queue
}
if ($stopped) { exit 2 }
