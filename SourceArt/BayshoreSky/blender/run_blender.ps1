# Headless Blender runner: BelowNormal priority, no window, own log, exit code passed through.
# Usage: powershell -File run_blender.ps1 -Script bake_sky.py -Log <name> [-TimeoutSec 7200] [-- args after --]
param([Parameter(Mandatory=$true)][string]$Script, [string]$Log = "blender", [int]$TimeoutSec = 7200, [string]$PyArgs = "")
$exe = "C:\Program Files\Blender Foundation\Blender 5.2\blender.exe"
$dir = Split-Path -Parent $MyInvocation.MyCommand.Path
$logDir = "K:\Full Throttle\SkyScratch\Saved\AgentTemp"
New-Item -ItemType Directory -Force -Path $logDir | Out-Null
$out = Join-Path $logDir ($Log + ".log"); $err = Join-Path $logDir ($Log + ".err.log")
$a = '-b --factory-startup --python-exit-code 1 -P "' + (Join-Path $dir $Script) + '"'
if ($PyArgs) { $a += ' -- ' + $PyArgs }
$p = Start-Process -FilePath $exe -ArgumentList $a -NoNewWindow -PassThru -RedirectStandardOutput $out -RedirectStandardError $err
$null = $p.Handle
try { $p.PriorityClass = "BelowNormal" } catch {}
if (-not $p.WaitForExit($TimeoutSec * 1000)) { $p.Kill(); Write-Output "TIMEOUT"; exit 99 }
Write-Output ("EXIT=" + $p.ExitCode + " LOG=" + $out)
Select-String -LiteralPath $out -Pattern "BSKYB|Error|Traceback" | Select-Object -Last 40 | ForEach-Object { $_.Line }
Get-Content -LiteralPath $err -Tail 15 | Where-Object { $_ -notmatch "nanobind" }
exit $p.ExitCode
