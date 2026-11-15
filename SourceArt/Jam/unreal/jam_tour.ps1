# A tour of all eight levels in a PACKAGED build, on the game's own autopilot, with frames at fixed race-clock times:
# the same script against the build before and the build after gives matched pairs (same level, same second).
#   powershell -File jam_tour.ps1 -Tag before -Root "K:\Full Throttle\PortBuilds\Dev_bld_after3\Windows" [-Quality 3]
#       [-Levels "1,2,3,4,5,6,7,8"] [-Times "8,20,35,50"] [-ResX 3440 -ResY 1440]
# Frames: SkyScratch/HandBack/jam/tour/<Tag>/L<level>_t<seconds>.png (HUD on: they are the game's own screenshots).
# Uses the game's -ftexec runner (FTStartLevel, FTAutoRun, FTQuality, waitfor on the "FT CLOCK" log line).
param([Parameter(Mandatory=$true)][string]$Tag, [string]$Root = "K:\Full Throttle\PortBuilds\Development\Windows", [int]$Quality = 3,
      [string]$Levels = "1,2,3,4,5,6,7,8", [string]$Times = "8,20,35,50", [string]$L1Times = "8,20,24,26,35,50",
      [int]$ResX = 3440, [int]$ResY = 1440, [string]$Extra = "")
$ErrorActionPreference = "Stop"
$exe = Join-Path $Root "FullThrottle.exe"
if (-not (Test-Path $exe)) { throw "no packaged build at $exe" }
$saved = Join-Path $Root "FullThrottle\Saved\FullThrottle"
$lines = @("# jam_tour $Tag", "FTQuality $Quality")
foreach ($lv in ($Levels -split "[,\s]+" | Where-Object { $_ })) {
  $lines += @("FTStartLevel $lv", "waitfor FT LEVEL id=|90", "FTAutoRun wreck=0")
  if ($Extra) { $lines += $Extra -split ";" }
  $ts = if ($lv -eq "1") { $L1Times } else { $Times }
  foreach ($t in ($ts -split "[,\s]+" | Where-Object { $_ })) {
    $lines += @(("waitfor FT CLOCK level={0}.0|120" -f [int]$t), ("shot L{0}_t{1:000}" -f $lv, [int]$t))
  }
}
$lines += "quit"
$execFile = Join-Path $env:TEMP ("jam_tour_{0}.txt" -f $Tag)
Set-Content -LiteralPath $execFile -Value $lines -Encoding ascii
Remove-Item -Recurse -Force (Join-Path $saved "shots") -ErrorAction SilentlyContinue
$ga = @("-windowed", "-ResX=$ResX", "-ResY=$ResY", "-nosound", "-log", "-RenderOffScreen", "-unattended", "-ftexec=`"$execFile`"")
$p = Start-Process -FilePath $exe -ArgumentList ($ga -join " ") -WorkingDirectory $Root -PassThru
$null = $p.Handle
$budget = 240 + 120 * @($Levels -split "[,\s]+" | Where-Object { $_ }).Count
if (-not $p.WaitForExit($budget * 1000)) { $p.Kill(); "TIMEOUT (killed after $budget s)" }
"exit " + $p.ExitCode
$log = Get-ChildItem (Join-Path $Root "FullThrottle\Saved\Logs") -Filter *.log -ErrorAction SilentlyContinue | Sort-Object LastWriteTime -Descending | Select-Object -First 1
if ($log) {
  Select-String -LiteralPath $log.FullName -Pattern "Fatal|LogFT: Error|result=timeout|FTBUILD|FTBED" | Select-Object -Last 24 | ForEach-Object { $_.Line.Substring(0, [Math]::Min(220, $_.Line.Length)) }
  Copy-Item $log.FullName (Join-Path "K:\Full Throttle\SkyScratch\DevLog\jam" ("tour_{0}.log" -f $Tag)) -Force -ErrorAction SilentlyContinue
}
$dst = "K:\Full Throttle\SkyScratch\HandBack\jam\tour\$Tag"
Remove-Item -Recurse -Force $dst -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force $dst | Out-Null
Get-ChildItem (Join-Path $saved "shots") -Filter *.png -ErrorAction SilentlyContinue | Copy-Item -Destination $dst
"{0} frames -> {1}" -f (Get-ChildItem $dst -Filter *.png).Count, $dst
