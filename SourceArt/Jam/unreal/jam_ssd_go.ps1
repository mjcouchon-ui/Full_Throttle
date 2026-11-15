# One-off (15 Nov 2026): waits for the last ungated job (a process id), then switches the engine folder to the NVMe
# copy (jam_ssd.ps1 -Mode engine), moves the named checkouts' build folders, and smoke-tests the result: a build that
# must find everything up to date, and an editor start that runs one line of Python. Lifts no gate: the lead reads
# the result first.
param([int]$WaitPid = 0, [string[]]$Folders = @(), [string]$TestCheckout = "K:\Full Throttle\UE\full-throttle-unreal")
$ErrorActionPreference = "Continue"
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$ssd = Join-Path $here "jam_ssd.ps1"
if ($WaitPid -gt 0) {
  "waiting for process $WaitPid, {0:HH:mm:ss}" -f (Get-Date)
  while (Get-Process -Id $WaitPid -ErrorAction SilentlyContinue) { Start-Sleep -Seconds 5 }
  "it has ended, {0:HH:mm:ss}" -f (Get-Date)
}
# let the last game's helpers go (the crash reporter lingers a few seconds)
$until = (Get-Date).AddSeconds(240)
while ((Get-Date) -lt $until) {
  $u = @(Get-Process | Where-Object { $_.Path -and $_.Path.StartsWith("K:\UE_5.9\", [StringComparison]::OrdinalIgnoreCase) })
  if (-not $u.Count) { break }
  Start-Sleep -Seconds 3
}
"=== engine"
& powershell -NoProfile -ExecutionPolicy Bypass -File $ssd -Mode engine
$eng = $LASTEXITCODE
"engine mode exit $eng"
foreach ($c in $Folders) {
  "=== folders: $c"
  & powershell -NoProfile -ExecutionPolicy Bypass -File $ssd -Mode folders -Checkout $c
  "folders mode exit $LASTEXITCODE"
}
"=== status"
& powershell -NoProfile -ExecutionPolicy Bypass -File $ssd -Mode status
if ($eng -ne 0) { "ENGINE NOT SWITCHED: no smoke test"; exit 2 }
"=== smoke test 1: the build tool must find the integration checkout up to date, {0:HH:mm:ss}" -f (Get-Date)
$t0 = Get-Date
& (Join-Path $here "jam_ubt.bat") FullThrottleEditor Win64 Development "-project=$TestCheckout\FullThrottle.uproject" -NoMutex -NoUBA -UBADisableWaitOnMem -NoHotReloadFromIDE -MaxParallelActions=4 "-log=C:\FT\smoke_ubt.txt" *> "C:\FT\smoke_ubt_out.txt"
"build tool exit $LASTEXITCODE after {0:N0} s" -f ((Get-Date) - $t0).TotalSeconds
Select-String -Path "C:\FT\smoke_ubt_out.txt" -Pattern "up to date|Result:|action\(s\)|error|Total execution" | Select-Object -Last 8 | ForEach-Object { "   " + $_.Line.Substring(0, [Math]::Min(200, $_.Line.Length)) }
"=== smoke test 2: the editor starts from the junction and runs one line of Python, {0:HH:mm:ss}" -f (Get-Date)
Set-Content -LiteralPath "C:\FT\smoke.py" -Value 'import unreal; unreal.log("JAMSSD smoke ok " + unreal.SystemLibrary.get_engine_version())' -Encoding ascii
Remove-Item "C:\FT\smoke.log" -Force -ErrorAction SilentlyContinue
$t0 = Get-Date
$p = Start-Process -FilePath "K:\UE_5.9\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" -ArgumentList @("`"$TestCheckout\FullThrottle.uproject`"", "-run=pythonscript", "-script=`"C:/FT/smoke.py`"", "-unattended", "-nopause", "-nosplash", "-nullrhi", "-abslog=`"C:\FT\smoke.log`"") -WindowStyle Hidden -PassThru
if (-not $p.WaitForExit(600000)) { "the editor has not ended after 600 s (left running, pid $($p.Id))" } else { "editor exit $($p.ExitCode) after {0:N0} s" -f ((Get-Date) - $t0).TotalSeconds }
if (Test-Path "C:\FT\smoke.log") {
  Select-String -Path "C:\FT\smoke.log" -Pattern "JAMSSD smoke ok|Fatal error|LogModuleManager: Warning|could not be loaded|Engine Version|Base Directory|LogInit: Command Line" | Select-Object -First 10 | ForEach-Object { "   " + $_.Line.Substring(0, [Math]::Min(220, $_.Line.Length)) }
} else { "no editor log at C:\FT\smoke.log" }
"=== done {0:HH:mm:ss}" -f (Get-Date)
