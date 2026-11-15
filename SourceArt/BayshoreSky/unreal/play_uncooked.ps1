# Runs the port's Level 1 uncooked, from the editor binary in game mode (-game), with the game's own autopilot and
# screenshots: the real in-game frame (exposure, cel pass, fog, HUD) without packaging, for fast look iterations.
# Use play_port.ps1 on a packaged build for final evidence and for any performance number.
#   powershell -File play_uncooked.ps1 -Tag <name> [-Seconds 45] [-Every 4] [-ExtraCmds "r.VolumetricFog 0"]
# Shots go to SkyScratch/HandBack/ingame/<Tag>/. Takes the port's Unreal lock, like ue_run.ps1.
param([string]$Tag = "uncooked", [int]$Seconds = 45, [float]$Every = 4, [string]$ExtraCmds = "",
      [int]$ResX = 3440, [int]$ResY = 1440, [string]$Engine = "K:\UE_5.9")
$ErrorActionPreference = "Stop"
$port = "K:\Full Throttle\UE\full-throttle-unreal"
$proj = Join-Path $port "FullThrottle.uproject"
. (Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Path) "ue_lock.ps1")
$lock = Enter-UeLock -ProjectDir $port
try {
  $shots = Join-Path $port "Saved\FullThrottle\shots"
  Remove-Item -Recurse -Force $shots -ErrorAction SilentlyContinue
  $log = Join-Path $port "Saved\Logs\play_uncooked.log"
  $exe = Join-Path $Engine "Engine\Binaries\Win64\UnrealEditor.exe"
  $ga = @("`"$proj`"", "-game", "-windowed", "-ResX=$ResX", "-ResY=$ResY", "-RenderOffScreen", "-nosound", "-unattended",
          "-nosplash", "-ftauto", "-ftshots=$Every", "-ftquit=$Seconds", "-abslog=`"$log`"")
  if ($ExtraCmds) { $ga += "-ExecCmds=`"$ExtraCmds`"" }
  $p = Start-Process -FilePath $exe -ArgumentList ($ga -join " ") -WorkingDirectory $port -PassThru
  $null = $p.Handle
  # shader compiles for changed materials happen before the race starts, so allow for them on top of the run itself
  if (-not $p.WaitForExit(($Seconds + 900) * 1000)) { $p.Kill(); Write-Output "TIMEOUT (killed)" }
  Write-Output ("exit " + $p.ExitCode)
  if (Test-Path $log) {
    Select-String -LiteralPath $log -Pattern "Fatal error|Error: |LogMaterial: Error|BayshoreSky|Shader compile" |
      Where-Object { $_.Line -notmatch 'HttpListener|ModelContext' } | Select-Object -Last 12 |
      ForEach-Object { $_.Line.Substring(0, [Math]::Min(300, $_.Line.Length)) }
  }
  $dst = "K:\Full Throttle\SkyScratch\HandBack\ingame\$Tag"
  Remove-Item -Recurse -Force $dst -ErrorAction SilentlyContinue
  New-Item -ItemType Directory -Force $dst | Out-Null
  Get-ChildItem $shots -Filter *.png -ErrorAction SilentlyContinue | Copy-Item -Destination $dst
  "{0} shots -> {1}" -f (Get-ChildItem $dst -Filter *.png).Count, $dst
} finally { Exit-UeLock $lock }
