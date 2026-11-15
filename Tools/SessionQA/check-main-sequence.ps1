param(
 [string]$RunsRoot=(Join-Path $PSScriptRoot 'runs'),
 [switch]$CompletedOnly
)
$ErrorActionPreference='Stop'
$RunsRoot=[IO.Path]::GetFullPath($RunsRoot,$PWD.Path)
function Require([bool]$Condition,[string]$Message){if(!$Condition){throw $Message}}
$previous=$null
for($case=1;$case -le 5;$case++){
 $script=Get-ChildItem -LiteralPath (Join-Path $PSScriptRoot 'scripts') -Filter ('main-{0:D2}-*.txt' -f $case) | Select-Object -First 1
 $out=Join-Path $RunsRoot $script.BaseName
 $logPath=Join-Path $out 'game.log'
 $snapshotPath=Join-Path $out 'save-snapshot'
 $careerPath=Join-Path $snapshotPath 'career_pie.json'
 $log=if(Test-Path -LiteralPath $logPath){Get-Content -Raw -LiteralPath $logPath}else{''}
 Require ($log -notmatch 'result=timeout|Fatal error:|Assertion failed:|Ensure condition failed:') "Case $case has a runtime error or timeout."
 if(!(Test-Path -LiteralPath $snapshotPath)){
  if($CompletedOnly){"PENDING case=$case output=$out";continue}
  throw "Case $case has no completed save-snapshot directory: $out"
 }
 if($case -eq 1){
  Require ($log -match 'FT SESSION STATE phase=0 paused=0') 'Fresh launch did not remain on the title.'
  Require (!(Test-Path -LiteralPath $careerPath)) 'Fresh title created a campaign file before New Game.'
  Require ($log -match 'FT SESSION SAVE kind=none.*valid=0') 'Fresh title unexpectedly had Continue.'
  "ASSERT PASS case=1 freshTitle=1 noCareerCreated=1"
  continue
 }
 Require ($log -notmatch 'FT EXEC line=\d+ cmd=quit\s*(?:\r?\n|$)') "Case $case used the fallback test quit, not the requested menu/native-close exit."
 Require (Test-Path -LiteralPath $careerPath) "Case $case did not save a career."
 $state=Get-Content -Raw -LiteralPath $careerPath | ConvertFrom-Json
 Require ($state.version -eq 2 -or $state.v -eq 2) "Case $case did not write version 2."
 # Paused SessionState lines are sampled 2 real seconds apart. Every phase-specific field must remain identical.
 $paused=[regex]::Matches($log,'FT SESSION STATE [^\r\n]*paused=1[^\r\n]*')
 $pairs=0
 for($i=1;$i -lt $paused.Count;$i++){if($paused[$i-1].Value -eq $paused[$i].Value){$pairs++}}
 $needed=if($case -eq 2){2}else{1}
 Require ($pairs -ge $needed) "Case $case did not prove $needed frozen state pair(s)."
 if($case -eq 2){
  Require ($state.resume.kind -eq 'race' -and $state.resume.level -eq 0) 'New campaign did not leave a race-start checkpoint.'
  Require ($log -match 'FTINPUT cursor at 640,312') 'Mouse options interaction was not injected.'
  Require ($log -match 'FT SYSTEM action=2 saved=1' -and $log -match 'FT SYSTEM action=1 saved=1') 'Title/desktop save exit missing.'
 }
 if($case -eq 3){
  Require ($log -match 'FT SYSTEM dialog=1 action=3') 'New Game replacement confirmation was not tested.'
  Require ($state.cash -eq $previous.cash -and $state.driver.xp -eq $previous.driver.xp -and $state.runId -eq $previous.runId) 'Cancel or Continue modified progress/run identity.'
  Require ($state.resume.kind -eq 'race') 'Race Continue no longer restarts the race.'
  Require ($log -match 'cmd=ft.WindowClose' -and $log -match 'FT SYSTEM action=1 saved=1') 'Native window-close path did not save and exit.'
 }
 if($case -eq 4){
  Require ($state.resume.kind -eq 'day' -and $state.resume.level -eq 0 -and $state.resume.wrap) 'Paycheck B overwrote the next-day-wrap checkpoint.'
  Require (![string]::IsNullOrEmpty($state.runId) -and $state.runId -eq $state.lastCommitId) 'Completed race was not recorded as one committed run.'
  Require ($state.cash -gt $previous.cash) 'Wrap did not award its paycheck.'
  Require ([regex]::Matches($log,'FT PAY total=').Count -eq 1) 'Wrap did not produce exactly one paycheck.'
  Require ($log -match 'FTINPUT tap Gamepad_FaceButton_Right' -and $log -match 'FT SESSION return=title') 'Paycheck controller-B exit did not reach title.'
 }
 if($case -eq 5){
  Require ($log -match 'FT SESSION continue kind=day level=1 stage=0 wrap=1') 'Continue failed to dispatch the committed wrap scene.'
  Require ($state.cash -eq $previous.cash -and $state.driver.xp -eq $previous.driver.xp -and $state.lastCommitId -eq $previous.lastCommitId) 'Continue replayed or lost committed rewards.'
  Require ($log -notmatch 'FT PAY total=') 'Story Continue recomputed a race paycheck.'
  Require ($log -match 'FT BRAWL SYSTEM exit=desktop discard=0') 'Campaign story native-close exit failed.'
 }
 $previous=$state
 "ASSERT PASS case=$case frozenPairs=$pairs checkpoint=$($state.resume.kind) cash=$($state.cash) xp=$($state.driver.xp)"
}
if(!$CompletedOnly){"MAIN SEQUENCE PASS output=$RunsRoot"}
