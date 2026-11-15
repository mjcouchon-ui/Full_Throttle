# Package the Full Throttle port (its own MapsToCook) for Win64. Usage: powershell -File package_port.ps1 -Config Shipping|Development
param([string]$Config = "Shipping")
$UE = 'K:\UE_5.9'
$Proj = 'K:\Full Throttle\UE\full-throttle-unreal\FullThrottle.uproject'
$Out = 'K:\Full Throttle\PortBuilds'
(Get-Process -Id $PID).PriorityClass = 'BelowNormal'
. (Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Path) "ue_lock.ps1")
$lock = Enter-UeLock -ProjectDir (Split-Path -Parent $Proj)   # no editor script may save assets mid-cook
try {
$env:uebp_LogFolder = "$Out\UATLogs\$Config"
New-Item -ItemType Directory -Force $env:uebp_LogFolder | Out-Null
$log = "$Out\package_$Config.log"
$extra = @(); if ($Config -eq 'Shipping') { $extra += '-nodebuginfo' }
& "$UE\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun "-project=$Proj" -platform=Win64 "-clientconfig=$Config" -build -cook -stage -pak -iostore -compressed -prereqs @extra -archive "-archivedirectory=$Out\$Config" -nocompileeditor -unattended -utf8output -nop4 *> $log
$code = $LASTEXITCODE
"UAT exit code: $code"
Select-String -Path $log -Pattern "BUILD SUCCESSFUL|AutomationTool exiting|Error:|Warning:.*BayshoreSky|missing usage flag" | Select-Object -Last 20 | ForEach-Object { $_.Line }
if (Test-Path "$Out\$Config\Windows") { "{0:N1} MB total" -f ((Get-ChildItem "$Out\$Config\Windows" -Recurse -File | Measure-Object Length -Sum).Sum / 1MB) }
} finally { Exit-UeLock $lock }
exit $code
