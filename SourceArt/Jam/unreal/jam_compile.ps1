# Compiles a checkout's editor module under the machine-wide Unreal lock and that checkout's own.
#   powershell -File jam_compile.ps1 [-WaitSec 900] [-Checkout "K:\Full Throttle\VisualAssetsWorktree\full-throttle-unreal"]
# -Checkout: the project folder (default: env JAM_PORT, else the UE checkout).
# The switches, and why (15 Nov, the night three checkouts and the Codex session's playtest shared this PC):
#   -NoHotReloadFromIDE  without it the build tool refuses to build while ANY game or editor on this engine has a Live
#                        Coding session open ("Unable to build while Live Coding is active"), even one running from
#                        another copy of the project, such as the owner's open playtest. That session is not touched:
#                        this builds this checkout's own module only. Do not run it while THIS checkout's editor is open.
#   -NoMutex             the locks here already keep this to one build at a time, and without the engine-wide build
#                        mutex a compile no longer makes every editor start in every checkout wait (an editor asks the
#                        build tool for its targets on start-up, behind that mutex).
#   -NoUBA               the build accelerator froze compiles for many minutes when memory ran short.
#   -MaxParallelActions=3  each compiler process takes close to 3 GB.
#   -Touch <file>        a source file under Source\FullThrottle to mark as changed once the locks are held, so it is
#                        compiled again (for a table .inl swapped in while another job's compile may still have been
#                        reading the old one: the build tool would otherwise call the stale object up to date).
param([int]$WaitSec = 900, [string]$Checkout = "", [string]$Touch = "")
$ErrorActionPreference = "Stop"
if (-not $Checkout) { $Checkout = if ($env:JAM_PORT) { $env:JAM_PORT } else { "K:\Full Throttle\UE\full-throttle-unreal" } }
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
. "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal\ue_lock.ps1"
. (Join-Path $here "jam_global.ps1")
$tagName = Split-Path -Leaf (Split-Path -Parent $Checkout)
$log = "K:\Full Throttle\PortBuilds\compile_editor_$tagName.log"
$code = 1
$global = Enter-JamGlobal -Checkout $Checkout
try {
  $lock = Enter-UeLock -ProjectDir $Checkout
  try {
    if ($Touch) { (Get-Item -LiteralPath (Join-Path $Checkout ("Source\FullThrottle\" + $Touch))).LastWriteTime = Get-Date }
    & "K:\UE_5.9\Engine\Build\BatchFiles\Build.bat" FullThrottleEditor Win64 Development "-project=$Checkout\FullThrottle.uproject" -NoMutex -NoUBA -NoHotReloadFromIDE -MaxParallelActions=3 *> $log
    $code = $LASTEXITCODE
  } finally { Exit-UeLock $lock }
} finally { Exit-UeLock $global }
"UBT exit code: $code"
Select-String -Path $log -Pattern "error |Error:|warning C|Result: |Total execution time|up to date|Live Coding" | Select-Object -Last 25 | ForEach-Object { $_.Line.Substring(0, [Math]::Min(260, $_.Line.Length)) }
exit $code
