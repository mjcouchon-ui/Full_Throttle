# Compiles the port's editor module (after a merge that touched C++), under the port's Unreal lock.
#   powershell -File bld_compile.ps1
$ErrorActionPreference = "Stop"
$port = "K:\Full Throttle\UE\full-throttle-unreal"
. "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal\ue_lock.ps1"
$lock = Enter-UeLock -ProjectDir $port
try {
  $log = "K:\Full Throttle\PortBuilds\compile_editor.log"
  & "K:\UE_5.9\Engine\Build\BatchFiles\Build.bat" FullThrottleEditor Win64 Development "-project=$port\FullThrottle.uproject" -WaitMutex *> $log
  $code = $LASTEXITCODE
  "UBT exit code: $code"
  Select-String -Path $log -Pattern "error |Error:|warning C|Result: |Total execution time|up to date" | Select-Object -Last 25 | ForEach-Object { $_.Line.Substring(0, [Math]::Min(260, $_.Line.Length)) }
  exit $code
} finally { Exit-UeLock $lock }
