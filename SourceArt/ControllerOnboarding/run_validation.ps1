param(
  [ValidateSet('automation','race','race_extras','race_failure','brawl','brawl_grab','persistence','controls','day_cut','session_controls')][string]$Mode = 'automation',
  [string]$Checkout = 'K:\Full Throttle\ControllerWorktree\full-throttle-unreal',
  [string]$Evidence = 'K:\Full Throttle\IntegrationReceipts\ControllerOnboarding',
  [int]$TimeoutSec = 900,
  [switch]$LocksAlreadyHeld
)
$ErrorActionPreference = 'Stop'
. (Join-Path $Checkout 'SourceArt\BayshoreSky\unreal\ue_lock.ps1')
. (Join-Path $Checkout 'SourceArt\Jam\unreal\jam_global.ps1')
New-Item -ItemType Directory -Force -Path $Evidence | Out-Null
$globalLock = if (-not $LocksAlreadyHeld) { Enter-JamGlobal -Checkout $Checkout -WaitSec 3600 } else { $null }
try {
  $checkoutLock = if (-not $LocksAlreadyHeld) { Enter-UeLock -ProjectDir $Checkout -WaitSec 3600 } else { $null }
  try {
    $env:WISPOS_APP_DATA = Join-Path $Evidence 'TestSave'
    New-Item -ItemType Directory -Force -Path $env:WISPOS_APP_DATA | Out-Null
    if ($Mode -eq 'brawl_grab') {
      # Keep the first-fight tips visible in repeatable item/buff layout captures.
      $settingsPath = Join-Path $env:WISPOS_APP_DATA 'settings.json'
      if (Test-Path -LiteralPath $settingsPath) {
        $testSettings = Get-Content -LiteralPath $settingsPath -Raw | ConvertFrom-Json
        $testSettings | Add-Member -NotePropertyName brawlHelpSeen -NotePropertyValue $false -Force
        $testSettings | ConvertTo-Json -Depth 64 | Set-Content -LiteralPath $settingsPath -Encoding utf8
      }
    }
    $log = Join-Path $Evidence ($Mode + '.log')
    $project = Join-Path $Checkout 'FullThrottle.uproject'
    if ($Mode -eq 'automation') {
      $exe = 'K:\UE_5.9\Engine\Binaries\Win64\UnrealEditor-Cmd.exe'
      $report = Join-Path $Evidence 'Automation'
      $argsLine = '"{0}" /Engine/Maps/Entry -nullrhi -unattended -nop4 -nosplash -NoSound -ExecCmds="Automation RunTests FullThrottle+FT.Save+FT.Input.NativeDualShockDisconnect" -TestExit="Automation Test Queue Empty" -ReportExportPath="{1}" -abslog="{2}"' -f $project,$report,$log
    } else {
      $exe = 'K:\UE_5.9\Engine\Binaries\Win64\UnrealEditor.exe'
      $commands = Join-Path $Checkout ('SourceArt\ControllerOnboarding\' + $Mode + '.txt')
      $map = if ($Mode -in @('brawl','brawl_grab')) { '/Game/Brawl/Maps/L_RustyAnchor' } else { '/Game/Maps/Bayshore/L_Bayshore' }
      $extra = if ($Mode -eq 'brawl_grab') { '-brawlnocut -brawlpadtest -brawlgrab' } elseif ($Mode -eq 'brawl') { '-brawlnocut' } elseif ($Mode -eq 'day_cut') { '-ftday1' } else { '' }
      $argsLine = '"{0}" {1} -game -windowed -ResX=1920 -ResY=1080 -RenderOffScreen -nosound -unattended -nosplash -ftexec="{2}" -abslog="{3}" {4}' -f $project,$map,$commands,$log,$extra
    }
    # These tests execute an already-built Win64 target. Avoid the editor's
    # redundant Build.bat SDK probe serializing this run behind other checkouts.
    # Scope the engine-supported switch to the child, not the user's environment.
    $previousSdkSetup = $env:UE_SKIP_UBT_SDK_SETUP
    try {
      $env:UE_SKIP_UBT_SDK_SETUP = '1'
      $process = Start-Process -FilePath $exe -ArgumentList $argsLine -WindowStyle Hidden -PassThru
    } finally {
      if ($null -eq $previousSdkSetup) { Remove-Item Env:UE_SKIP_UBT_SDK_SETUP -ErrorAction SilentlyContinue }
      else { $env:UE_SKIP_UBT_SDK_SETUP = $previousSdkSetup }
    }
    $null = $process.Handle
    if (-not $process.WaitForExit($TimeoutSec * 1000)) { Stop-Process -Id $process.Id -Force; throw "Validation $Mode timed out; see $log" }
    Write-Output ("Validation mode={0} exit={1} log={2}" -f $Mode,$process.ExitCode,$log)
    if ($process.ExitCode -ne 0) { throw "Unreal validation exited $($process.ExitCode)" }
    $failures = @(Select-String -LiteralPath $log -Pattern 'FTCONTROL FAIL|FT EXEC waitfor=.*result=timeout|Test Completed. Result=\{Fail\}|Fatal error:' -ErrorAction SilentlyContinue)
    if ($failures.Count) { $failures | ForEach-Object { Write-Output $_.Line }; throw 'Validation checks failed' }
    if ($Mode -eq 'automation') {
      $requiredTests = @(
        'FullThrottle.Input.DeviceFamiliesAndBindings', 'FullThrottle.Input.IntentionalSwitchAndDisconnect',
        'FullThrottle.Onboarding.ActionProgress', 'FT.Save.FirstLaunchAndMigration',
        'FT.Save.BackupAndUnsupportedVersion', 'FT.Save.AtomicFailureAndInterruptedWrite',
        'FT.Save.IdempotentRewardsAndPurchases', 'FT.Save.AsyncQueueAndRollback',
        'FT.Input.NativeDualShockDisconnect'
      )
      foreach ($testName in $requiredTests) {
        if (-not (Select-String -LiteralPath $log -Pattern ('Test Completed\. Result=\{Success\}.*Path=\{' + [regex]::Escape($testName) + '\}'))) { throw "Missing successful automation result: $testName" }
      }
    } else {
      $expected = @(Select-String -LiteralPath $commands -Pattern '^(ft\.AssertControl|ftb\.AssertAction) ').Count
      $passed = @(Select-String -LiteralPath $log -Pattern 'FTCONTROL PASS ').Count
      if ($passed -ne $expected) { throw "Incomplete runtime assertions: expected=$expected passed=$passed" }
      if (-not (Select-String -LiteralPath $log -Pattern 'FT EXEC line=\d+ cmd=quit')) { throw 'Runtime script did not reach its final quit command' }
    }
    if ($Mode -eq 'brawl_grab') {
      $grabs = @(Select-String -LiteralPath $log -Pattern 'FTBRAWL grab \S+ hp=')
      $hurls = @(Select-String -LiteralPath $log -Pattern 'FTBRAWL hurl \S+ dir=')
      Write-Output ("Brawl pad gameplay evidence: grabs={0} hurls={1}" -f $grabs.Count,$hurls.Count)
      @($grabs + $hurls) | ForEach-Object { Write-Output $_.Line }
      if (-not $grabs.Count -or -not $hurls.Count) { throw "Brawl pad grab/hurl coverage incomplete; actual gameplay evidence missing in $log" }
    }
    if ($Mode -eq 'day_cut') {
      $starts = @(Select-String -LiteralPath $log -Pattern 'FTBRAWL cut start 10(?:\s|$)')
      $skips = @(Select-String -LiteralPath $log -Pattern 'FTBRAWL cut 10 skipped')
      if ($starts.Count -lt 2 -or $skips.Count -lt 2) { throw "Cutscene replay/skip evidence incomplete: starts=$($starts.Count) skips=$($skips.Count)" }
      Write-Output ("Day cut real-input evidence: starts={0} skips={1}" -f $starts.Count,$skips.Count)
      Select-String -LiteralPath $log -Pattern 'FTBRAWL cut start 10|FTBRAWL cut 10 skipped|FT DAY1' | ForEach-Object { Write-Output $_.Line }
    }
    Select-String -LiteralPath $log -Pattern 'FTCONTROL |FTINPUT status|FT TUTORIAL|FTBRAWLCONTROLS|Test Completed' | ForEach-Object { Write-Output $_.Line }
  } finally { Exit-UeLock $checkoutLock }
} finally { Exit-UeLock $globalLock }
