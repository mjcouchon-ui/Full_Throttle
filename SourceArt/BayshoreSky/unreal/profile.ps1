# Profile the packaged Development build offscreen (no window). Writes logs and CSVs to DevLog\perf\<Run>.
# Usage: powershell -File profile.ps1 -Run A|B|C|D  [-Frames 2400] [-ResX 2560 -ResY 1440]
#   A: sky on, CSV with per-pass GPU stats       B: sky meshes hidden (A - B = sky cost)
#   C: ProfileGPU at three moments with per-material draw events     D: Unreal Insights trace (hitches, PSO)
param([string]$Run = "A", [int]$Frames = 2400, [int]$ResX = 2560, [int]$ResY = 1440, [string]$Config = "Development")
$Root = 'K:\Full Throttle\SkyScratch'
$pkg = "$Root\Packaged\$Config\Windows"
$exe = "$pkg\Engine\Binaries\Win64\UnrealGame.exe"
$out = "$Root\DevLog\perf\$Run"
New-Item -ItemType Directory -Force $out | Out-Null
$common = @('../../../SkyScratch/SkyScratch.uproject', '/BayshoreSky/Maps/L_BayshoreSkyTest', '-RenderOffScreen', '-nosplash', '-unattended',
            '-windowed', "-ResX=$ResX", "-ResY=$ResY", '-ForceRes', '-benchmark', '-fps=60', '-nosound', "-abslog=`"$out\run.log`"")
$exec = 't.MaxFPS 0,r.VSync 0,r.ProfileGPU.ShowUI 0,r.ProfileGPU.UnicodeOutput 0,r.ScreenPercentage 100'
switch ($Run) {
  'A' { $extra = @("-csvCaptureFrames=$Frames", '-csvGpuStats', '-ExitAfterCsvProfiling', "-ExecCmds=`"$exec`"") }
  'B' { $extra = @("-csvCaptureFrames=$Frames", '-csvGpuStats', '-ExitAfterCsvProfiling', "-ExecCmds=`"$exec,ShowFlag.StaticMeshes 0,ShowFlag.InstancedStaticMeshes 0`"") }
  'C' { $extra = @("-csvCaptureFrames=1500", '-ExitAfterCsvProfiling', '-csvExecCmds=300:ProfileGPU,780:ProfileGPU,1260:ProfileGPU',
                   "-ExecCmds=`"$exec,r.RDG.Events 3,r.ShowMaterialDrawEvents 1,r.EmitMeshDrawEvents 1,r.ProfileGPU.TableFormatting 0`"") }
  'L' { $extra = @("-csvCaptureFrames=$Frames", '-csvGpuStats', '-ExitAfterCsvProfiling', '-ini:GameUserSettings:[ScalabilityGroups]:sg.EffectsQuality=0',
                   "-ExecCmds=`"$exec,sg.EffectsQuality 0`"") }
  'D' { $extra = @('-benchmarkseconds=45', '-trace=default,gpu,counters,stats', '-statnamedevents', "-tracefile=`"$out\run.utrace`"",
                   "-ExecCmds=`"$exec,r.RDG.Events 3`"", '"-LogCmds=LogPSOHitching Verbose"') }
}
$argLine = (($common + $extra) -join ' ')
$p = Start-Process -FilePath $exe -ArgumentList $argLine -WorkingDirectory (Split-Path $exe) -NoNewWindow -PassThru -RedirectStandardOutput "$out\stdout.txt" -RedirectStandardError "$out\stderr.txt"
$null = $p.Handle
if (-not $p.WaitForExit(900000)) { $p.Kill(); "TIMEOUT"; exit 99 }
"exit $($p.ExitCode)  priority Normal  res ${ResX}x${ResY}"
$csvDirs = @("$pkg\SkyScratch\Saved\Profiling\CSV", "$pkg\Engine\Saved\Profiling\CSV") | Where-Object { Test-Path $_ }
if ($csvDirs) {
  $latest = Get-ChildItem $csvDirs -Filter *.csv | Sort-Object LastWriteTime -Descending | Select-Object -First 1
  if ($latest -and $latest.LastWriteTime -gt (Get-Date).AddMinutes(-15)) { Copy-Item $latest.FullName "$out\profile.csv" -Force; "csv -> $out\profile.csv" }
}
Select-String -Path "$out\run.log" -Pattern "Error|Fatal" | Select-Object -First 5 | ForEach-Object { $_.Line }
exit $p.ExitCode
