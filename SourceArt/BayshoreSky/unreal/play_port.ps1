# Runs the packaged port (Development) on Level 1 and collects real in-game evidence:
#   -Mode shots : auto-drive with screenshots every -Every seconds for -Seconds (the game's -ftauto -ftshots), into
#                 SkyScratch/HandBack/ingame/<Tag>/ (real frames: cel pass, exposure, fog, HUD, light shafts)
#   -Mode bench : the game's own benchmark (-ftbench), its JSON (fps, GPU ms, p99) into SkyScratch/DevLog/perf/ingame/<Tag>.json
# Renders off-screen by default (-Windowed shows a window). ExtraCmds go to -ExecCmds (e.g. "r.VolumetricFog 0").
# Development builds only: a Shipping build needs a WispOS launch ticket (FTGameInstance.cpp) and stops at a dialog.
param([ValidateSet("shots","bench")][string]$Mode = "shots", [string]$Tag = "run", [int]$Seconds = 60, [float]$Every = 4,
      [ValidateSet("Development")][string]$Config = "Development", [string]$ExtraCmds = "", [switch]$Windowed,
      [int]$Quality = 3, [string]$Root = "")
# -Root: another packaged build's Windows folder (perf_ab.ps1 keeps the with-sky and without-sky builds side by side)
$root = if ($Root) { $Root } else { "K:\Full Throttle\PortBuilds\$Config\Windows" }
$exe = Join-Path $root "FullThrottle.exe"
if (-not (Test-Path $exe)) { throw "no packaged build at $exe (package_port.ps1 -Config $Config)" }
$saved = Join-Path $root "FullThrottle\Saved\FullThrottle"
# the game writes bench_L0_<QUALITY><tag>.json; match the tag exactly ("sky_r1" must not pick up "nosky_r1")
$benchName = '^bench_L\d+_[A-Z]+' + [regex]::Escape($Tag) + '\.json$'
$ga = @("-windowed", "-ResX=1920", "-ResY=1080", "-nosound", "-log")
if (-not $Windowed) { $ga += "-RenderOffScreen" }
if ($Mode -eq "shots") {
  Remove-Item -Recurse -Force (Join-Path $saved "shots") -ErrorAction SilentlyContinue
  $ga += @("-ftauto", "-ftshots=$Every", "-ftquit=$Seconds")
} else {
  # an earlier run's JSON with the same tag must not be taken for this run's result (review)
  Get-ChildItem (Join-Path $saved "bench") -Filter *.json -ErrorAction SilentlyContinue | Where-Object { $_.Name -match $benchName } | Remove-Item -Force
  $ga += @("-ftbench", "-ftlevel=0", "-ftquality=$Quality", "-ftseconds=$Seconds", "-fttag=$Tag")
}
if ($ExtraCmds) { $ga += "-ExecCmds=`"$ExtraCmds`"" }
$p = Start-Process -FilePath $exe -ArgumentList ($ga -join " ") -WorkingDirectory $root -PassThru
$null = $p.Handle
if (-not $p.WaitForExit(($Seconds + 180) * 1000)) { $p.Kill(); Write-Output "TIMEOUT (killed)" }
Write-Output ("exit " + $p.ExitCode)
$log = Get-ChildItem (Join-Path $root "FullThrottle\Saved\Logs") -Filter *.log -ErrorAction SilentlyContinue | Sort-Object LastWriteTime -Descending | Select-Object -First 1
if ($log) { Select-String -LiteralPath $log.FullName -Pattern "Fatal|Error: |FTBENCH|BayshoreSky" | Select-Object -Last 12 | ForEach-Object { $_.Line.Substring(0, [Math]::Min(300, $_.Line.Length)) } }
if ($Mode -eq "shots") {
  $dst = "K:\Full Throttle\SkyScratch\HandBack\ingame\$Tag"
  Remove-Item -Recurse -Force $dst -ErrorAction SilentlyContinue     # no frames left over from an earlier, longer run
  New-Item -ItemType Directory -Force $dst | Out-Null
  Get-ChildItem (Join-Path $saved "shots") -Filter *.png -ErrorAction SilentlyContinue | Copy-Item -Destination $dst
  "{0} shots -> {1}" -f (Get-ChildItem $dst -Filter *.png).Count, $dst
} else {
  $dst = "K:\Full Throttle\SkyScratch\DevLog\perf\ingame"
  New-Item -ItemType Directory -Force $dst | Out-Null
  $json = @(Get-ChildItem (Join-Path $saved "bench") -Filter *.json -ErrorAction SilentlyContinue | Where-Object { $_.Name -match $benchName })
  if (-not $json.Count) { throw "no benchmark JSON for $Tag (the run crashed, timed out or quit early)" }
  $json | ForEach-Object { Copy-Item $_.FullName (Join-Path $dst ($Tag + ".json")); Get-Content $_.FullName }
}
