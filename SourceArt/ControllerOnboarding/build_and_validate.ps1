param(
  [string]$Checkout = 'K:\Full Throttle\ControllerWorktree\full-throttle-unreal',
  [string]$Evidence = 'K:\Full Throttle\IntegrationReceipts\ControllerOnboarding',
  [string[]]$Modes = @('automation','race','brawl','persistence','controls','race_failure','race_extras','brawl_grab','day_cut','session_controls')
)
$ErrorActionPreference = 'Stop'
. (Join-Path $Checkout 'SourceArt\BayshoreSky\unreal\ue_lock.ps1')
. (Join-Path $Checkout 'SourceArt\Jam\unreal\jam_global.ps1')
New-Item -ItemType Directory -Force -Path $Evidence | Out-Null
$globalLock = Enter-JamGlobal -Checkout $Checkout -WaitSec 7200
try {
  $checkoutLock = Enter-UeLock -ProjectDir $Checkout -WaitSec 7200
  try {
    & 'K:\UE_5.9\Engine\Build\BatchFiles\Build.bat' FullThrottleEditor Win64 Development "-Project=$Checkout\FullThrottle.uproject" -NoMutex -NoHotReloadFromIDE -NoUBA -MaxParallelActions=3 2>&1 | Tee-Object -FilePath (Join-Path $Evidence 'build.log')
    if ($LASTEXITCODE -ne 0) { throw "Build failed with exit $LASTEXITCODE" }
    foreach ($mode in $Modes) {
      & (Join-Path $PSScriptRoot 'run_validation.ps1') -Mode $mode -Checkout $Checkout -Evidence $Evidence -LocksAlreadyHeld
    }
  } finally { Exit-UeLock $checkoutLock }
} finally { Exit-UeLock $globalLock }
