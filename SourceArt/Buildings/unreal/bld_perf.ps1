# Performance of the buildings pass on Level 1, with the game's own benchmark (-ftbench, 3440x1440 from the game's
# settings). RUN ONLY ON AN IDLE GPU: a busy GPU doubles the frame time (this script stops if another Unreal, game or
# Blender process is running).
#   -Stage keep   : keeps the current packaged build (level1-sky, before the buildings) as PortBuilds\Dev_bld_before.
#                   Run it once, BEFORE packaging the buildings branch.
#   -Stage package: packages the port as it is now and keeps it as PortBuilds\Dev_bld_<As> (default "after").
#   -Stage bench  : every build named in -Tags (PortBuilds\Dev_bld_<tag>), at Epic and Low, each -Runs times.
#                   Results: SkyScratch/DevLog/perf/buildings/<tag>[_low]_r<n>.json.
#                   -Cmds "name=r.Some.CVar 0;name2=..." adds Epic cases on the last tag with console commands.
# Builds so far: before (level1-sky), after (props as Nanite, 14 Nov), after2 (props not Nanite).
param([ValidateSet("keep", "package", "bench")][string]$Stage = "bench", [int]$Runs = 2, [int]$Seconds = 40,
      [string]$As = "after", [string]$Tags = "before,after", [string]$Cmds = "", [switch]$NoLow)
$ErrorActionPreference = "Stop"
$u = "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal"
$out = "K:\Full Throttle\PortBuilds"
$dst = "K:\Full Throttle\SkyScratch\DevLog\perf\buildings"
function Mirror($from, $to) {
  robocopy $from $to /MIR /NFL /NDL /NJH /NJS /NP /R:2 /W:2 | Out-Null
  if ($LASTEXITCODE -ge 8) { throw "robocopy $from -> $to failed ($LASTEXITCODE)" }
  "mirrored $from -> $to"
}
function Busy {
  # Another Unreal, game or Blender process means the GPU is not ours. (The utilisation percentage is no guide: with
  # only the desktop, remote-desktop streaming and this app running, the idle card reads 30-40 % at its low clock.)
  @(Get-CimInstance Win32_Process -Filter "Name='FullThrottle.exe' OR Name='UnrealEditor.exe' OR Name='UnrealEditor-Cmd.exe' OR Name='blender.exe' OR Name='ShaderCompileWorker.exe'" | ForEach-Object { "$($_.Name) ($($_.ProcessId))" })
}
if ($Stage -eq "keep") {
  if (Test-Path "$out\Dev_bld_before\Windows\FullThrottle.exe") { throw "Dev_bld_before already exists; delete it first if the baseline really should be replaced" }
  Mirror "$out\Development" "$out\Dev_bld_before"
  return
}
if ($Stage -eq "package") {
  powershell -NoProfile -File "$u\package_port.ps1" -Config Development | Select-Object -Last 6
  if ($LASTEXITCODE -ne 0) { throw "package failed ($LASTEXITCODE)" }
  Mirror "$out\Development" "$out\Dev_bld_$As"
  return
}
New-Item -ItemType Directory -Force $dst | Out-Null
$tagList = @($Tags -split "[,\s]+" | Where-Object { $_ })   # a new name: PowerShell variables ignore case, $tags would be the [string] parameter
$cases = @()
foreach ($t in $tagList) { $cases += @{ tag = $t; root = "$out\Dev_bld_$t\Windows"; q = 3; cmds = "" } }
if (-not $NoLow) { foreach ($t in $tagList) { $cases += @{ tag = "${t}_low"; root = "$out\Dev_bld_$t\Windows"; q = 0; cmds = "" } } }
foreach ($c in @($Cmds -split ";" | Where-Object { $_ })) {
  $k, $v = $c -split "=", 2
  $cases += @{ tag = "$($tagList[-1])_$k"; root = "$out\Dev_bld_$($tagList[-1])\Windows"; q = 3; cmds = $v }
}
function WaitIdle([int]$MaxSec = 1500) {
  # other sessions share this machine: wait until no other Unreal has run for 15 s
  $until = (Get-Date).AddSeconds($MaxSec); $quiet = 0
  while ((Get-Date) -lt $until) {
    if ((Busy).Count) { $quiet = 0 } else { $quiet++ }
    if ($quiet -ge 3) { return }
    Start-Sleep -Seconds 5
  }
  throw "the machine did not go idle within $MaxSec s"
}
function Others {
  # Another Unreal, game or Blender process still running when ours has exited overlapped it. (One that both started
  # and ended inside our 40 s is not seen; the other sessions' runs last minutes.)
  @(Get-CimInstance Win32_Process -Filter "Name='FullThrottle.exe' OR Name='UnrealEditor.exe' OR Name='UnrealEditor-Cmd.exe' OR Name='blender.exe'" | ForEach-Object { "$($_.Name) ($($_.ProcessId))" })
}
foreach ($r in 1..$Runs) {
  foreach ($c in $cases) {
    $t = "bld_$($c.tag)_r$r"
    $pa = @("-NoProfile", "-File", "$u\play_port.ps1", "-Mode", "bench", "-Tag", $t, "-Seconds", $Seconds, "-Quality", $c.q, "-Root", $c.root)
    if ($c.cmds) { $pa += @("-ExtraCmds", $c.cmds) }
    $json = $null
    foreach ($try in 1..6) {
      WaitIdle
      $json = powershell @pa | Select-Object -Last 1
      $o = Others
      if (-not $o.Count) { break }
      "{0,-18} discarded (try {1}): {2} ran during the benchmark" -f "$($c.tag) r$r", $try, ($o -join ", ")
      $json = $null
    }
    if (-not $json) { throw "$($c.tag) r${r}: no clean run in 6 tries (another session keeps the GPU busy)" }
    Copy-Item "K:\Full Throttle\SkyScratch\DevLog\perf\ingame\$t.json" "$dst\$($c.tag)_r$r.json" -Force
    $j = $json | ConvertFrom-Json
    "{0,-18} gpu {1,6} ms  p99 {2,6}  frame median {3,6} ms  render {4,6} ms  fps {5,6}  {6} {7}" -f "$($c.tag) r$r", $j.gpu_ms, $j.gpu_p99_ms, $j.median_ms, $j.render_thread_ms, $j.avg_fps, $j.quality, $j.resolution
  }
}
