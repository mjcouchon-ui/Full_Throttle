# Runs a buildings script in the port's editor (off-screen), through the sky pass's ue_run.ps1 and its Unreal lock.
#   powershell -File bld_run.ps1 -Script 01_probe.py [-Commandlet] [-TimeoutSec 1800]
# Another session may be benchmarking a packaged build: wait for any running -ftbench game to finish first, so this
# job does not skew its numbers (benchmarks need an idle GPU).
param([Parameter(Mandatory=$true)][string]$Script, [switch]$Commandlet, [int]$TimeoutSec = 1800, [string]$ExtraArgs = "")
$ErrorActionPreference = "Stop"
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$u = "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal"
$port = "K:\Full Throttle\UE\full-throttle-unreal\FullThrottle.uproject"
function Wait-Bench([int]$MaxSec = 45) {
  $until = (Get-Date).AddSeconds($MaxSec)
  while ((Get-Date) -lt $until) {
    $b = @(Get-CimInstance Win32_Process -Filter "Name='FullThrottle.exe'" | Where-Object { $_.CommandLine -match '-ftbench' })
    if (-not $b.Count) { return }
    Start-Sleep -Seconds 5
  }
  Write-Host "bld_run: a benchmark is still running after $MaxSec s; going ahead"
}
Wait-Bench
$path = if ([IO.Path]::IsPathRooted($Script)) { $Script } else { Join-Path $here $Script }
$pa = @("-NoProfile", "-File", "$u\ue_run.ps1", "-Script", $path, "-Project", $port, "-TimeoutSec", $TimeoutSec)
if (-not $Commandlet) { $pa += "-Editor" }
if ($ExtraArgs) { $pa += @("-ExtraArgs", $ExtraArgs) }
$out = @(powershell @pa 2>&1 | ForEach-Object { "$_" })
$code = $LASTEXITCODE
$out | Where-Object { $_ -match "BLD |Traceback|Error: |error X|EXIT=|ue.lock|Timed out|missing usage" -and $_ -notmatch 'RHIResources|HttpListener|ModelContext' } |
  Select-Object -First 200 | ForEach-Object { $i = $_.IndexOf('BLD '); if ($i -ge 0) { $_.Substring($i) } else { $_.Substring(0, [Math]::Min(400, $_.Length)) } }
exit $code
