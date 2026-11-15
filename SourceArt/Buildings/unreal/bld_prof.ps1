# Where the GPU frame goes, before and after the buildings: the same auto-drive in both packaged builds, with three
# `profilegpu` dumps at fixed times (the game's -ftexec harness), summed per pass from the log.
# Idle GPU only. Output: SkyScratch/DevLog/perf/buildings/prof_<tag>.txt and a side-by-side table.
#   powershell -File bld_prof.ps1 [-At 22] [-Tags before,after] [-Cmds "r.Some.CVar 0"]
param([int]$At = 22, [string]$Tags = "before,after", [string]$Cmds = "", [string]$Suffix = "")
$ErrorActionPreference = "Stop"
$out = "K:\Full Throttle\PortBuilds"
$dst = "K:\Full Throttle\SkyScratch\DevLog\perf\buildings"
New-Item -ItemType Directory -Force $dst | Out-Null
$exec = Join-Path $dst "prof_exec.txt"
@("wait $At", "profilegpu", "wait 4", "profilegpu", "wait 4", "profilegpu", "wait 3", "quit") | Set-Content -Encoding ascii $exec
$sum = @{}
foreach ($tag in ($Tags -split ",")) {
  $root = "$out\Dev_bld_$tag\Windows"
  $log = "$dst\prof_$tag$Suffix.log"
  Remove-Item $log -ErrorAction SilentlyContinue
  $ga = @("-windowed", "-ResX=1920", "-ResY=1080", "-nosound", "-RenderOffScreen", "-ftauto", "-ftquality=3", "-ftexec=`"$exec`"", "-abslog=`"$log`"")
  $cmds = "DisableAllScreenMessages"; if ($Cmds) { $cmds += "," + $Cmds }
  $ga += "-ExecCmds=`"$cmds`""
  $p = Start-Process -FilePath "$root\FullThrottle.exe" -ArgumentList ($ga -join " ") -WorkingDirectory $root -PassThru
  $null = $p.Handle
  if (-not $p.WaitForExit(($At + 120) * 1000)) { $p.Kill(); "TIMEOUT $tag" }
  # profilegpu prints a tree: "  12.3%  1.05ms   PassName ..." ; keep the top two levels, averaged over the dumps
  $rows = @{}; $dumps = 0
  foreach ($l in Get-Content $log) {
    if ($l -match "Perf marker hierarchy|Profiling the GPU") { if ($l -match "Perf marker hierarchy") { $dumps++ } ; continue }
    if ($l -match "LogRHI:\s*(\s*)([\d\.]+)%\s+([\d\.]+)ms\s+(.+?)\s+\d+ draws") {
      $depth = [int]($Matches[1].Length / 3)
      if ($depth -le 2) {
        $name = ("  " * $depth) + ($Matches[4] -replace "\s+\d+x\d+.*$", "" -replace "\s*\(.*$", "")
        if (-not $rows.ContainsKey($name)) { $rows[$name] = 0.0 }
        $rows[$name] += [double]$Matches[3]
      }
    }
  }
  if (-not $dumps) { "no profilegpu output in $log"; continue }
  $sum[$tag] = @{}
  foreach ($k in $rows.Keys) { $sum[$tag][$k] = $rows[$k] / $dumps }
  "$tag : $dumps dumps, $($rows.Count) passes"
}
$tagList = @($Tags -split "," | Where-Object { $sum.ContainsKey($_) })   # not $tags: PowerShell variables ignore case
if ($tagList.Count) {
  $names = @($tagList | ForEach-Object { $sum[$_].Keys } | Sort-Object -Unique)
  $table = foreach ($n in $names) {
    $v = @($tagList | ForEach-Object { if ($sum[$_].ContainsKey($n)) { $sum[$_][$n] } else { 0.0 } })
    [pscustomobject]@{ Pass = $n; A = $v[0]; B = $(if ($v.Count -gt 1) { $v[1] } else { 0.0 }); Delta = $(if ($v.Count -gt 1) { $v[1] - $v[0] } else { 0.0 }) }
  }
  $lines = $table | Where-Object { $_.A -ge 0.03 -or $_.B -ge 0.03 } | Sort-Object { -[math]::Abs($_.Delta) } | Select-Object -First 40 |
    ForEach-Object { "{0,-58} {1,7:N2} {2,7:N2} {3,7:+0.00;-0.00}" -f $_.Pass, $_.A, $_.B, $_.Delta }
  @("{0,-58} {1,7} {2,7} {3,7}" -f "pass (ms, mean of the dumps)", $tagList[0], $(if ($tagList.Count -gt 1) { $tagList[1] } else { "" }), "delta") + $lines | Tee-Object -FilePath "$dst\prof_table$Suffix.txt"
}
