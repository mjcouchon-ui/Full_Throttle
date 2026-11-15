param(
 [ValidateSet('main','brawl')][string]$Game='main',
 [Parameter(Mandatory=$true)][string]$Script,
 [Parameter(Mandatory=$true)][string]$Output,
 [Parameter(Mandatory=$true)][string]$Profile,
 [int]$TimeoutSeconds=150,
 [switch]$Audio,
 [string]$EngineRoot='C:\Program Files\Epic Games\UE_5.9',
 [string[]]$Extra=@()
)
$ErrorActionPreference='Stop'
$repo=Split-Path (Split-Path (Split-Path $PSScriptRoot -Parent) -Parent) -Parent
$project=if($Game -eq 'main'){Join-Path $repo 'full-throttle-unreal\FullThrottle.uproject'}else{Join-Path $repo 'beat-em-up-unreal\FullThrottleBrawl.uproject'}
$engine=Join-Path $EngineRoot 'Engine\Binaries\Win64\UnrealEditor-Cmd.exe'
$outPath=[IO.Path]::GetFullPath($Output,$PWD.Path)
$profilePath=[IO.Path]::GetFullPath($Profile,$PWD.Path)
$scriptPath=(Resolve-Path -LiteralPath $Script).Path
New-Item -ItemType Directory -Path $outPath,$profilePath -Force | Out-Null
$scriptBody=Get-Content -Raw -LiteralPath $scriptPath
$scriptPath=Join-Path $outPath 'run.txt'
[IO.File]::WriteAllText($scriptPath,"r.SetRes 1280x720w`nwait 1`n"+$scriptBody,[Text.UTF8Encoding]::new($false))
$log=Join-Path $outPath 'game.log'
$scriptSwitch=if($Game -eq 'main'){'-ftexec='}else{'-brawlexec='}
$argsForGame=@(('"'+$project+'"'),'-game','-windowed','-ForceRes','-ResX=1280','-ResY=720','-nosplash','-unattended','-RenderOffScreen','-FixedSeed','-ftslot=pie','-ftnoattract',('-abslog="'+$log+'"'),($scriptSwitch+'"'+$scriptPath+'"'),'-ini:EditorPerProjectUserSettings:[/Script/ModelContextProtocol.ModelContextProtocolSettings]:bAutoStartServer=False')
if(!$Audio){$argsForGame+='-nosound'}
if($Game -eq 'brawl'){$argsForGame+=('-brawlsave="'+(Join-Path $profilePath 'brawl-campaign.json')+'"')}
$argsForGame+=$Extra
$gameProcess=Start-Process -FilePath $engine -ArgumentList $argsForGame -WindowStyle Hidden -PassThru -Environment @{WISPOS_APP_DATA=$profilePath}
"SESSION game=$Game pid=$($gameProcess.Id) log=$log"
if(!$gameProcess.WaitForExit($TimeoutSeconds*1000)){$gameProcess.Kill();throw "Session probe timed out: $log"}
$text=Get-Content -Raw -LiteralPath $log
if($gameProcess.ExitCode -ne 0 -or $text -match 'result=timeout|Fatal error:|Assertion failed:|Ensure condition failed:'){throw "Session probe failed (exit $($gameProcess.ExitCode)): $log"}
$snapshotPath=Join-Path $outPath 'save-snapshot'
New-Item -ItemType Directory -Path $snapshotPath -Force | Out-Null
Get-ChildItem -LiteralPath $profilePath -File -Filter '*.json*' | ForEach-Object { Copy-Item -LiteralPath $_.FullName -Destination $snapshotPath }
"PASS game=$Game exit=0 log=$log"
