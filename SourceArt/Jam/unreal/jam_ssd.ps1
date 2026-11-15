# Moves what Unreal jobs read and write all the time OFF the two spinning disks (K:) onto the NVMe drive (C:), without
# changing a single path: the folder is copied, the original is renamed aside (<name>_hdd, kept whole), and a
# junction of the original name points at the copy. To go back: remove the junction (cmd /c rmdir "<path>": that
# removes the link only) and rename <name>_hdd back.
# Why (15 Nov 2026): with four jobs at once three compilers had used 3 to 5 s of processor time in 13 to 25 minutes.
# Each was waiting to read the engine's headers and its own precompiled header (1 to 2 GB, read at random) from disks
# that the other jobs were reading too. The owner had closed everything else on the PC and said to move the engine.
#   powershell -File jam_ssd.ps1 -Mode status
#   powershell -File jam_ssd.ps1 -Mode engine                      K:\UE_5.9 -> C:\UE_5.9 (copy first with robocopy; this does the last pass and the switch)
#   powershell -File jam_ssd.ps1 -Mode folders -Checkout "<project folder>"    its Intermediate and Binaries -> C:\FT\<checkout>\
# Nothing is switched while a process still uses the folder: the mode says so and changes nothing. Never deletes.
param([Parameter(Mandatory=$true)][ValidateSet("status","engine","folders")][string]$Mode, [string]$Checkout = "",
      [string]$EngineFrom = "K:\UE_5.9", [string]$EngineTo = "C:\UE_5.9", [string]$FoldersTo = "C:\FT")
$ErrorActionPreference = "Stop"
function Test-Junction([string]$Path) {
  if (-not (Test-Path -LiteralPath $Path)) { return $false }
  return [bool]((Get-Item -LiteralPath $Path -Force).Attributes -band [IO.FileAttributes]::ReparsePoint)
}
function Get-EngineUsers([string]$Root) {
  @(Get-Process | Where-Object { $_.Path -and $_.Path.StartsWith($Root + "\", [StringComparison]::OrdinalIgnoreCase) })
}
function Get-CheckoutUsers([string]$Co) {
  $a = $Co.TrimEnd('\'); $b = $a.Replace('\', '/')
  @(Get-CimInstance Win32_Process | Where-Object { $_.Name -match '^(UnrealEditor.*|cl|link|dotnet|ShaderCompileWorker|UnrealPak|lib)\.exe$' -and $_.CommandLine -and
      ($_.CommandLine.IndexOf($a, [StringComparison]::OrdinalIgnoreCase) -ge 0 -or $_.CommandLine.IndexOf($b, [StringComparison]::OrdinalIgnoreCase) -ge 0) })
}
function Copy-Tree([string]$From, [string]$To, [string]$Log, [switch]$NewerOnly) {
  $a = @("`"$From`"", "`"$To`"", "/E", "/XJ", "/MT:16", "/R:1", "/W:1", "/COPY:DAT", "/DCOPY:DAT", "/NFL", "/NDL", "/NP", "/LOG:`"$Log`"")
  if ($NewerOnly) { $a += "/XO" }
  $p = Start-Process -FilePath robocopy.exe -ArgumentList $a -WindowStyle Hidden -PassThru -Wait
  $tail = @(Get-Content -LiteralPath $Log -Tail 12 -ErrorAction SilentlyContinue | Where-Object { $_ -match 'Dirs :|Files :|Bytes :|Ended' })
  $tail | ForEach-Object { Write-Host ("    " + $_.Trim()) }   # to the host: this function's only output is the exit code
  return [int]$p.ExitCode     # robocopy: 8 and above = something was not copied
}
$logDir = "K:\Full Throttle\SkyScratch\DevLog\jam\overhaul"

if ($Mode -eq "status") {
  "engine: {0} is {1}" -f $EngineFrom, $(if (Test-Junction $EngineFrom) { "a junction (moved)" } else { "the real folder (on the hard disks)" })
  $u = @(Get-EngineUsers $EngineFrom)
  "processes running from the engine folder: {0}" -f $u.Count
  $u | ForEach-Object { "   {0,-28} pid {1,6} start {2:HH:mm:ss} cpu {3,6:N0} s" -f $_.ProcessName, $_.Id, $_.StartTime, $_.CPU }
  foreach ($c in @("UE", "VisualAssetsWorktree", "OverhaulBosses", "OverhaulFx")) {
    $co = "K:\Full Throttle\$c\full-throttle-unreal"
    $n = @(Get-CheckoutUsers $co).Count
    "{0,-22} Intermediate {1,-9} Binaries {2,-9} Unreal or compiler processes: {3}" -f $c, $(if (Test-Junction "$co\Intermediate") { "junction" } else { "hard disk" }), $(if (Test-Junction "$co\Binaries") { "junction" } else { "hard disk" }), $n
  }
  $hold = "K:\Full Throttle\SkyScratch\GlobalUnrealLock\HOLD.txt"
  "gate: {0}" -f $(if (Test-Path -LiteralPath $hold) { "HELD (" + (Get-Content -LiteralPath $hold -TotalCount 1) + ")" } else { "open" })
  exit 0
}

if ($Mode -eq "engine") {
  if (Test-Junction $EngineFrom) { "engine: already a junction, nothing to do"; exit 0 }
  $u = @(Get-EngineUsers $EngineFrom)
  if ($u.Count) { "NOT SWITCHED: {0} process(es) still run from {1}:" -f $u.Count, $EngineFrom; $u | ForEach-Object { "   {0} pid {1}" -f $_.ProcessName, $_.Id }; exit 2 }
  "last pass of the copy, {0:HH:mm:ss}" -f (Get-Date)
  $code = Copy-Tree $EngineFrom $EngineTo (Join-Path $logDir "engine_copy_last.log")
  if ($code -ge 8) { "NOT SWITCHED: the copy reports failures (robocopy exit $code); see engine_copy_last.log"; exit 3 }
  $u = @(Get-EngineUsers $EngineFrom)
  if ($u.Count) { "NOT SWITCHED: a process started from the engine folder during the last pass"; exit 2 }
  $aside = $EngineFrom + "_hdd"
  if (Test-Path -LiteralPath $aside) { "NOT SWITCHED: $aside exists already"; exit 4 }
  try { Rename-Item -LiteralPath $EngineFrom -NewName (Split-Path -Leaf $aside) -ErrorAction Stop }
  catch { "NOT SWITCHED: the folder could not be renamed (something holds a file in it): $($_.Exception.Message)"; exit 5 }
  cmd /c mklink /J "$EngineFrom" "$EngineTo" | Out-Null
  if (-not (Test-Path -LiteralPath (Join-Path $EngineFrom "Engine\Binaries\Win64\UnrealEditor.exe"))) {
    "the junction does not resolve: putting the folder back"
    if (Test-Junction $EngineFrom) { cmd /c rmdir "$EngineFrom" | Out-Null }
    Rename-Item -LiteralPath $aside -NewName (Split-Path -Leaf $EngineFrom)
    exit 6
  }
  "SWITCHED {0:HH:mm:ss}: {1} is now a junction to {2}; the original is {3}" -f (Get-Date), $EngineFrom, $EngineTo, $aside
  exit 0
}

if ($Mode -eq "folders") {
  if (-not $Checkout -or -not (Test-Path -LiteralPath (Join-Path $Checkout "FullThrottle.uproject"))) { throw "no project at '$Checkout'" }
  $label = Split-Path -Leaf (Split-Path -Parent $Checkout)
  $bad = 0
  foreach ($d in @("Intermediate", "Binaries")) {
    $p = Join-Path $Checkout $d
    if (-not (Test-Path -LiteralPath $p)) { "{0}\{1}: not there" -f $label, $d; continue }
    if (Test-Junction $p) { "{0}\{1}: already a junction" -f $label, $d; continue }
    $u = @(Get-CheckoutUsers $Checkout)
    if ($u.Count) { "NOT MOVED {0}\{1}: {2} Unreal or compiler process(es) of this checkout are running" -f $label, $d, $u.Count; $bad++; continue }
    $to = Join-Path (Join-Path $FoldersTo $label) $d
    New-Item -ItemType Directory -Force $to | Out-Null
    "{0}\{1}: copying to {2}, {3:HH:mm:ss}" -f $label, $d, $to, (Get-Date)
    $code = Copy-Tree $p $to (Join-Path $logDir ("ssd_{0}_{1}.log" -f $label, $d))
    if ($code -ge 8) { "NOT MOVED {0}\{1}: the copy reports failures (robocopy exit {2})" -f $label, $d, $code; $bad++; continue }
    $u = @(Get-CheckoutUsers $Checkout)
    if ($u.Count) { "NOT MOVED {0}\{1}: a job of this checkout started during the copy" -f $label, $d; $bad++; continue }
    $aside = $p + "_hdd"
    if (Test-Path -LiteralPath $aside) { "NOT MOVED {0}\{1}: {2} exists already" -f $label, $d, $aside; $bad++; continue }
    try { Rename-Item -LiteralPath $p -NewName ($d + "_hdd") -ErrorAction Stop }
    catch { "NOT MOVED {0}\{1}: could not rename (something holds a file in it): {2}" -f $label, $d, $_.Exception.Message; $bad++; continue }
    cmd /c mklink /J "$p" "$to" | Out-Null
    if (-not (Test-Junction $p)) { "the junction was not made: putting {0} back" -f $p; Rename-Item -LiteralPath $aside -NewName $d; $bad++; continue }
    # what was written to the old folder while the copy ran
    $code2 = Copy-Tree $aside $to (Join-Path $logDir ("ssd_{0}_{1}_2.log" -f $label, $d)) -NewerOnly
    "MOVED {0}\{1} {2:HH:mm:ss} (second pass exit {3}); the original is {4}" -f $label, $d, (Get-Date), $code2, $aside
  }
  exit $bad
}
