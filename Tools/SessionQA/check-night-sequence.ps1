param([string]$RunsRoot=(Join-Path $PSScriptRoot 'runs'),[switch]$CompletedOnly)
$ErrorActionPreference='Stop'
function Require([bool]$Condition,[string]$Message){if(!$Condition){throw $Message}}
$previous=$null
for($case=1;$case -le 2;$case++){
 $script=Get-ChildItem -LiteralPath (Join-Path $PSScriptRoot 'scripts') -Filter ('night-{0:D2}-*.txt' -f $case) | Select-Object -First 1
 $out=Join-Path $RunsRoot $script.BaseName
 $logPath=Join-Path $out 'game.log'
 $snapshotPath=Join-Path $out 'save-snapshot'
 [string]$log=if(Test-Path -LiteralPath $logPath){Get-Content -Raw -LiteralPath $logPath}else{''}
 Require ($log -notmatch 'result=timeout|Fatal error:|Assertion failed:|Ensure condition failed:') "Night case $case has a runtime error or timeout."
 if(!(Test-Path -LiteralPath $snapshotPath)){if($CompletedOnly){"PENDING night=$case";continue};throw "Night case $case has not completed."}
 Require ($log -notmatch 'FT EXEC line=\d+ cmd=quit\s*(?:\r?\n|$)') "Night case $case needed the fallback quit."
 $state=Get-Content -Raw -LiteralPath (Join-Path $snapshotPath 'career_pie.json') | ConvertFrom-Json
 Require ($state.v -eq 2 -and $state.resume.level -eq 1) "Night case $case did not save Night2 in the main career."
 Require ($log -match 'FT BRAWL SYSTEM exit=title discard=0' -and $log -match 'FT BRAWL SYSTEM exit=desktop discard=0') "Night case $case missed title/native-desktop save exit."
 Require ($log -match 'cmd=ft.WindowClose') "Night case $case did not exercise the native close delegate."
 $paused=[regex]::Matches($log,'FT SESSION STATE [^\r\n]*paused=1[^\r\n]*')
 $pairs=0;for($i=1;$i -lt $paused.Count;$i++){if($paused[$i-1].Value -eq $paused[$i].Value){$pairs++}}
 Require ($pairs -ge 1) "Night case $case did not freeze story/combat state."
 Require ($log -notmatch 'FT PAY total=') "Night case $case unexpectedly paid race rewards."
 if($case -eq 1){
  Require ($state.resume.kind -eq 'night' -and $state.resume.stage -ge 0) 'Initial night encounter checkpoint was not preserved.'
  Require ($log -match ('FT BRAWL CONTINUE night=2 wave='+$state.resume.stage+' cut=0 hp=150')) 'Main Continue did not restore the original encounter with full health.'
 }else{
  Require ($state.resume.kind -eq 'nightoutro' -and $state.resume.stage -eq 2) 'Boss completion did not preserve the outro checkpoint.'
  Require ($log -match ('FT BRAWL CONTINUE night=2 wave='+$previous.resume.stage+' cut=0 hp=150')) 'Process restart did not restore the saved encounter.'
  Require ($log -match 'FT SESSION continue kind=nightoutro level=2 stage=2' -and $log -match 'FT BRAWL CONTINUE night=2 wave=\d+ cut=2 hp=150') 'Main Continue did not restore LAST CALL.'
  Require ($state.cash -eq $previous.cash -and $state.driver.xp -eq $previous.driver.xp -and $state.lastCommitId -eq $previous.lastCommitId) 'Night restart/Continue changed unrelated race rewards.'
 }
 $previous=$state
 "ASSERT PASS night=$case frozenPairs=$pairs checkpoint=$($state.resume.kind) stage=$($state.resume.stage)"
}
if(!$CompletedOnly){'NIGHT INTEGRATION SEQUENCE PASS'}
