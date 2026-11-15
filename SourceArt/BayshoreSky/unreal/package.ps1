# Package SkyScratch (BuildCookRun) for Win64. Usage: powershell -File package.ps1 -Config Development|Shipping
param([string]$Config = "Development")
$UE = 'K:\UE_5.9'
$RunUAT = "$UE\Engine\Build\BatchFiles\RunUAT.bat"
$Proj = 'K:\Full Throttle\SkyScratch\SkyScratch.uproject'
$Root = 'K:\Full Throttle\SkyScratch'
$Map = '/BayshoreSky/Maps/L_BayshoreSkyTest'
(Get-Process -Id $PID).PriorityClass = 'BelowNormal'
. (Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Path) "ue_lock.ps1")
$lock = Enter-UeLock -ProjectDir $Root   # no editor script may save assets mid-cook
try {
$env:uebp_LogFolder = "$Root\Saved\UATLogs\$Config"
New-Item -ItemType Directory -Force $env:uebp_LogFolder | Out-Null
$Arch = "$Root\Packaged\$Config"
$extra = @()
if ($Config -eq 'Shipping') { $extra += '-nodebuginfo' }
$log = "$Root\Saved\AgentTemp\package_$Config.log"
& $RunUAT BuildCookRun "-project=$Proj" -platform=Win64 "-clientconfig=$Config" -cook "-map=$Map" -stage -pak -iostore -compressed -prereqs @extra -archive "-archivedirectory=$Arch" -nocompile -nocompileeditor -skipbuildeditor -unattended -utf8output -nop4 *> $log
$code = $LASTEXITCODE
"UAT exit code: $code"
Select-String -Path $log -Pattern "BUILD SUCCESSFUL|AutomationTool exiting|Error:|Warning:.*BayshoreSky" | Select-Object -Last 25 | ForEach-Object { $_.Line }
if (Test-Path "$Arch\Windows") { "{0:N1} MB total" -f ((Get-ChildItem "$Arch\Windows" -Recurse -File | Measure-Object Length -Sum).Sum / 1MB) }
} finally { Exit-UeLock $lock }
exit $code
