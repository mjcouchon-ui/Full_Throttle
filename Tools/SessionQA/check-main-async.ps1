param(
 [string]$RunsRoot=(Join-Path $PSScriptRoot 'runs-async'),
 [string]$ShopRun='main-async-03-shop-feedback',
 [switch]$CompletedOnly
)
$ErrorActionPreference='Stop'
$RunsRoot=[IO.Path]::GetFullPath($RunsRoot,$PWD.Path)
function Require([bool]$Condition,[string]$Message){if(!$Condition){throw $Message}}
for($case=1;$case -le 3;$case++){
 $script=Get-ChildItem -LiteralPath (Join-Path $PSScriptRoot 'scripts') -Filter ('main-async-{0:D2}-*.txt' -f $case) | Select-Object -First 1
 $out=Join-Path $RunsRoot $(if($case -eq 3){$ShopRun}else{$script.BaseName})
 $logPath=Join-Path $out 'game.log'
 $careerPath=Join-Path $out 'save-snapshot/career_pie.json'
 [string]$log=if(Test-Path -LiteralPath $logPath){Get-Content -Raw -LiteralPath $logPath}else{''}
 Require ($log -notmatch 'result=timeout|Fatal error:|Assertion failed:|Ensure condition failed:') "Async $case runtime error/timeout."
 if(!(Test-Path -LiteralPath $careerPath)){
  if($CompletedOnly -and !(Test-Path -LiteralPath (Join-Path $out 'save-snapshot'))){"PENDING async=$case output=$out";continue}
  throw "Async $case has no completed career snapshot: $out"
 }
 Require ($log -notmatch 'FT EXEC line=\d+ cmd=quit\s*(?:\r?\n|$)') "Async $case used fallback quit."
 Require ($log -match 'FT SYSTEM action=1 saved=1 explicitDiscard=0') "Async $case did not exit through the durable save barrier."
 Require ($log -match 'FT SESSION SAVE [^\r\n]*saving=1') "Async $case did not observe an actual pending write."
 $state=Get-Content -Raw -LiteralPath $careerPath | ConvertFrom-Json
 if($case -eq 1){
  Require ($log -match 'FT SYSTEM save_wait action=3 submitted=1') 'New Game did not wait for Reset.'
  $reset=[regex]::Match($log,'FT SAVE[^\r\n]*reason=Reset[^\r\n]*result=ok').Index
  $run=[regex]::Match($log,'FT SAVE[^\r\n]*reason=Run [^\r\n]*result=ok').Index
  $start=[regex]::Match($log,'LogFT: Display: FT TITLE start level=1').Index
  Require ($reset -gt 0 -and $run -gt $reset -and $start -gt $run) 'New Game used state before Reset/Run became durable.'
  Require ($log -match 'FT SYSTEM save_wait cancelled action=1') 'Controller B did not cancel the waiting exit.'
  Require ($log -match 'FT SESSION STATE phase=2 paused=0[\s\S]*?FT SESSION SAVE [^\r\n]*saving=1') 'Gameplay did not respond while a write was pending.'
  Require ($state.cash -eq 13579 -and $state.resume.kind -eq 'race') 'Delayed write/checkpoint was lost.'
 }
 if($case -eq 2){
  Require ($log -match 'FT SESSION continue kind=race' -and $log -match 'FT SESSION SAVE [^\r\n]*cash=13579') 'Restart did not read the prior durable race.'
  Require ($log -match 'FT SYSTEM dialog=3 action=1') 'Delayed failure did not show the save error choice.'
  Require ($log -match 'FT SYSTEM save_wait action=1 submitted=1') 'Retry was not a visible pending write.'
  Require ($log -match 'FT SYSTEM dialog=2 action=1') 'Race restart warning was lost after retry.'
  Require ($state.cash -eq 24680 -and $state.resume.kind -eq 'race') 'Retry did not persist the staged progress.'
  $writes=[regex]::Matches($log,'FT SAVE[^\r\n]*reason=QuitDesktop[^\r\n]*result=ok').Count
  Require ($writes -le 1) 'Retry submitted repeated writes.'
 }
 if($case -eq 3){
  Require ($log -match 'FT SHOP save=pending' -and $log -match 'FT SHOP save=failed' -and $log -match 'FT SHOP save=complete') 'Shop pending/failure/completion feedback was not covered.'
  Require ($state.cash -eq 2990000) 'Shop charged twice or lost the single purchase.'
  Require ($state.custom.own -contains 'p-bayshore-blue') 'Atomic purchase was not durable.'
  Require ($state.custom.own -notcontains 'p-lifeguard-yellow') 'Busy shop accepted a second purchase.'
  Require ($state.custom.paint -eq 'p-kestrel-red') 'Successful equipment retry was not durable.'
  Require ($log -match 'FTINPUT tap Enter') 'Equipment change did not use actual input.'
 }
 "ASSERT PASS async=$case checkpoint=$($state.resume.kind) cash=$($state.cash)"
}
if(!$CompletedOnly){"MAIN ASYNC SEQUENCE PASS output=$RunsRoot"}
