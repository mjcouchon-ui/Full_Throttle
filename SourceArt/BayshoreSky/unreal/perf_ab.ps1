# Performance A/B of the sky on Level 1, with the game's own benchmark (-ftbench, 3440x1440 from the game's settings).
# RUN ONLY ON AN IDLE GPU (no Blender/Cycles, no other Unreal): a busy GPU doubles the frame time.
#   -Stage builds : keeps the current packaged build as Dev_sky, packages Level 1 without the sky (84_port_sky_off.py)
#                   as Dev_nosky, then puts the sky back (80_port_apply.py). About 25 minutes.
#   -Stage bench  : benchmarks, each -Runs times, results in SkyScratch/DevLog/perf/ingame/<tag>.json:
#                   nosky (the original Level 1), sky, and the sky with one feature off at a time (volumetric fog,
#                   light shafts, a milder VSM resolution), at Epic; and nosky / sky at Low.
param([ValidateSet("builds", "bench", "all")][string]$Stage = "all", [int]$Runs = 2, [int]$Seconds = 40, [string]$Only = "")
# -Only: comma-separated case tags to run (default: all cases)
$u = Split-Path -Parent $MyInvocation.MyCommand.Path
$out = "K:\Full Throttle\PortBuilds"
$proj = "K:\Full Throttle\UE\full-throttle-unreal\FullThrottle.uproject"
function Mirror($from, $to) {
  robocopy $from $to /MIR /NFL /NDL /NJH /NJS /NP /R:2 /W:2 | Out-Null
  if ($LASTEXITCODE -ge 8) { throw "robocopy $from -> $to failed ($LASTEXITCODE)" }
  "mirrored $from -> $to"
}
function PortScript($script, $mustSay) {
  # stops on a failed editor script: a sky that did not come off would be benchmarked as "nosky" (review)
  $o = @(powershell -NoProfile -File "$u\ue_run.ps1" -Editor -Project $proj -Script "$u\$script" 2>&1 | ForEach-Object { "$_" })
  $code = $LASTEXITCODE
  $o | Where-Object { $_ -match "BSKY|Traceback|EXIT=" } | ForEach-Object { $_ }
  if ($code -ne 0 -or -not ($o -match [regex]::Escape($mustSay))) { throw "$script failed (exit $code)" }
}
if ($Stage -in "builds", "all") {
  if (-not (Test-Path "$out\Development\Windows\FullThrottle.exe")) { throw "no packaged build; run port_chain.ps1 -Stages package first" }
  Mirror "$out\Development" "$out\Dev_sky"
  try {
    PortScript "84_port_sky_off.py" "BSKY port sky removed"
    powershell -NoProfile -File "$u\package_port.ps1" -Config Development | Select-Object -Last 4
    if ($LASTEXITCODE -ne 0) { throw "the no-sky package failed ($LASTEXITCODE)" }
    Mirror "$out\Development" "$out\Dev_nosky"
  } finally {
    PortScript "80_port_apply.py" "BSKY port apply"      # always put the sky back; a failure here is raised
    Mirror "$out\Dev_sky" "$out\Development"
  }
}
if ($Stage -in "bench", "all") {
  $cases = @(
    @{ tag = "nosky";        root = "$out\Dev_nosky\Windows"; q = 3; cmds = "" },
    @{ tag = "sky";          root = "$out\Dev_sky\Windows";   q = 3; cmds = "" },
    @{ tag = "sky_novolfog"; root = "$out\Dev_sky\Windows";   q = 3; cmds = "r.VolumetricFog 0" },
    @{ tag = "sky_noshafts"; root = "$out\Dev_sky\Windows";   q = 3; cmds = "r.LightShaftQuality 0" },
    @{ tag = "sky_vsm05";    root = "$out\Dev_sky\Windows";   q = 3; cmds = "r.Shadow.Virtual.ResolutionLodBiasDirectional -0.5" },
    @{ tag = "nosky_low";    root = "$out\Dev_nosky\Windows"; q = 0; cmds = "" },
    @{ tag = "sky_low";      root = "$out\Dev_sky\Windows";   q = 0; cmds = "" }
  )
  # a new name: PowerShell variables are case-insensitive, so "$only" would overwrite the [string] -Only and re-join it
  $sel = @($Only -split "[,\s]+" | Where-Object { $_ })
  foreach ($r in 1..$Runs) {
    foreach ($c in $cases) {
      if ($sel.Count -and $c.tag -notin $sel) { continue }
      $t = "$($c.tag)_r$r"
      "== $t"
      # an empty -ExtraCmds is dropped by powershell -File and the parameter then has no value: pass it only when set
      $pa = @("-NoProfile", "-File", "$u\play_port.ps1", "-Mode", "bench", "-Tag", $t, "-Seconds", $Seconds, "-Quality", $c.q, "-Root", $c.root)
      if ($c.cmds) { $pa += @("-ExtraCmds", $c.cmds) }
      powershell @pa | Select-Object -Last 1
    }
  }
}
