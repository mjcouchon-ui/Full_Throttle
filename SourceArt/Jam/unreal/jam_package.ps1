# Packages a checkout for Win64 (build, cook, stage, pak) into its own folder under K:\Full Throttle\PortBuilds, under
# the PC's Unreal job queue and the checkout's own lock (no editor job may save assets while the cook reads them).
#   powershell -File jam_package.ps1 -Name RC_Development [-Config Development|Shipping] [-Checkout <project folder>]
# The build lands in PortBuilds\<Name>\Windows (FullThrottle.exe). Check it with:
#   powershell -File jam_verify.ps1 -Tag cooked -Root "K:\Full Throttle\PortBuilds\<Name>\Windows"
# The same command line as the sky pass's package_port.ps1; a folder of its own, so no earlier build is written over.
param([Parameter(Mandatory=$true)][string]$Name, [ValidateSet("Development","Shipping")][string]$Config = "Development", [string]$Checkout = "",
      [string]$Engine = "K:\UE_5.9")
$ErrorActionPreference = "Stop"
if (-not $Checkout) { $Checkout = if ($env:JAM_PORT) { $env:JAM_PORT } else { "K:\Full Throttle\UE\full-throttle-unreal" } }
$proj = Join-Path $Checkout "FullThrottle.uproject"
$out = "K:\Full Throttle\PortBuilds"
$dst = Join-Path $out $Name
. "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal\ue_lock.ps1"
. (Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Path) "jam_global.ps1")
(Get-Process -Id $PID).PriorityClass = 'BelowNormal'
$code = 1
$global = Enter-JamGlobal -Checkout $Checkout
try {
  $lock = Enter-UeLock -ProjectDir $Checkout
  try {
    $env:uebp_LogFolder = "$out\UATLogs\$Name"
    New-Item -ItemType Directory -Force $env:uebp_LogFolder | Out-Null
    $log = "$out\package_$Name.log"
    $extra = @(); if ($Config -eq 'Shipping') { $extra += '-nodebuginfo' }
    $t0 = Get-Date
    & "$Engine\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun "-project=$proj" -platform=Win64 "-clientconfig=$Config" -build -cook -stage -pak -iostore -compressed -prereqs @extra -archive "-archivedirectory=$dst" -nocompileeditor -unattended -utf8output -nop4 *> $log
    $code = $LASTEXITCODE
    "UAT exit code: $code  ({0:N0} s)" -f ((Get-Date) - $t0).TotalSeconds
    Select-String -Path $log -Pattern "BUILD SUCCESSFUL|BUILD FAILED|AutomationTool exiting|Error:|error C\d+|LogCook: Error|missing usage flag|Warning/Error Summary" | Select-Object -Last 25 | ForEach-Object { $_.Line.Substring(0, [Math]::Min(230, $_.Line.Length)) }
    if (Test-Path "$dst\Windows") { "{0:N1} MB in {1}\Windows" -f ((Get-ChildItem "$dst\Windows" -Recurse -File | Measure-Object Length -Sum).Sum / 1MB), $dst }
  } finally { Exit-UeLock $lock }
} finally { Exit-UeLock $global }
exit $code
