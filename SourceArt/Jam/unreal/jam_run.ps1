# Runs a game-jam script in a checkout's editor (off-screen), through the sky pass's ue_run.ps1 and its Unreal lock.
#   powershell -File jam_run.ps1 -Script 20_sky_clip_sweep.py [-Commandlet] [-TimeoutSec 1800] [-Env "A=1;B=2"]
#       [-Checkout "K:\Full Throttle\VisualAssetsWorktree\full-throttle-unreal"]
# -Checkout: the project folder to run in (default: env JAM_PORT, else the UE checkout). Each checkout has its own lock,
# and on top of that every job takes the machine-wide lock (jam_global.ps1): one Unreal job at a time on this PC.
# Prints the script's own lines (they start with "JAM ") and anything that looks like an error.
param([Parameter(Mandatory=$true)][string]$Script, [switch]$Commandlet, [int]$TimeoutSec = 1800, [string]$ExtraArgs = "", [string]$Env = "",
      [string]$Checkout = "")
$ErrorActionPreference = "Stop"
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$u = "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal"
if (-not $Checkout) { $Checkout = if ($env:JAM_PORT) { $env:JAM_PORT } else { "K:\Full Throttle\UE\full-throttle-unreal" } }
$proj = Join-Path $Checkout "FullThrottle.uproject"
foreach ($kv in ($Env -split ';' | Where-Object { $_ })) { $k, $v = $kv -split '=', 2; Set-Item -Path "Env:$k" -Value $v }
$path = if ([IO.Path]::IsPathRooted($Script)) { $Script } else { Join-Path $here $Script }
. (Join-Path $u "ue_lock.ps1")
. (Join-Path $here "jam_global.ps1")
$pa = @("-NoProfile", "-File", (Join-Path $u "ue_run.ps1"), "-Script", $path, "-Project", $proj, "-TimeoutSec", $TimeoutSec)
if (-not $Commandlet) { $pa += "-Editor" }
if ($ExtraArgs) { $pa += @("-ExtraArgs", $ExtraArgs) }
$code = 1
$out = @()
$global = Enter-JamGlobal -Checkout $Checkout
try {
  $out = @(powershell @pa 2>&1 | ForEach-Object { "$_" })
  $code = $LASTEXITCODE
} finally { Exit-UeLock $global }
$out | Where-Object { $_ -match "EXIT=|ue.lock|Timed out" } | ForEach-Object { $_ }
# ue_run prints only its own tags, so read the editor log for this script's lines
$log = Join-Path (Join-Path $Checkout "Saved\AgentTemp") (([IO.Path]::GetFileNameWithoutExtension($path)) + ".log")
if (Test-Path $log) {
  Select-String -LiteralPath $log -Pattern "JAM |Traceback|LogPython: Error|error X|missing usage" | Where-Object { $_.Line -notmatch 'RHIResources|HttpListener|ModelContext' } |
    Select-Object -First 400 | ForEach-Object { $l = $_.Line; $i = $l.IndexOf('JAM '); if ($i -ge 0) { $l.Substring($i, [Math]::Min(1200, $l.Length - $i)) } else { $l.Substring(0, [Math]::Min(400, $l.Length)) } }
}
exit $code
