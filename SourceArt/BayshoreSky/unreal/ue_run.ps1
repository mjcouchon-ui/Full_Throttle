# Runs a Python script inside UnrealEditor-Cmd (no window) against the SkyScratch project.
# Usage: powershell -File ue_run.ps1 -Script <file.py> [-Render] [-ExtraArgs "..."]
# One Unreal process at a time: takes a lock folder under Saved\ and releases it on exit.
param(
  [Parameter(Mandatory=$true)][string]$Script,
  [switch]$Render,
  [switch]$Editor,
  [string]$ExtraArgs = "",
  [string]$Project = "",   # default: the .uproject three folders up (SkyScratch, or the game when run from its copy)
  [string]$Engine = "K:\UE_5.9",
  [int]$TimeoutSec = 3600
)
$ErrorActionPreference = "Stop"
if (-not $Project) {
  # SourceArt/BayshoreSky/unreal -> the project root, in SkyScratch and in the game alike (review: the game's copy
  # defaulted to SkyScratch and rebuilt the wrong project)
  $root = (Resolve-Path (Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Path) "..\..\..")).Path
  $Project = @(Get-ChildItem -LiteralPath $root -Filter *.uproject | Select-Object -First 1 -ExpandProperty FullName)[0]
  if (-not $Project) { throw "no .uproject in $root; pass -Project" }
}
$projDir = Split-Path -Parent $Project
. (Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Path) "ue_lock.ps1")
New-Item -ItemType Directory -Force -Path (Join-Path $projDir "Saved\AgentTemp") | Out-Null
$lock = Enter-UeLock -ProjectDir $projDir
try {
  $name = [IO.Path]::GetFileNameWithoutExtension($Script)
  $log = Join-Path $projDir ("Saved\AgentTemp\" + $name + ".log")
  $exe = Join-Path $Engine "Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
  $scriptFwd = $Script -replace "\\","/"
  if ($Editor) {
    # full editor (subsystems, Slate, renderer) with no window; quits when the script ends
    $argList = @("`"$Project`"", "-ExecutePythonScript=`"$scriptFwd`"", "-RenderOffScreen", "-unattended", "-nopause", "-nosplash", "-NoSound", "-stdout", "-FullStdOutLogOutput", "-abslog=`"$log`"")
  } else {
    $argList = @("`"$Project`"", "-run=pythonscript", "-script=`"$scriptFwd`"", "-unattended", "-nopause", "-nosplash", "-stdout", "-FullStdOutLogOutput", "-abslog=`"$log`"")
    if ($Render) { $argList += "-AllowCommandletRendering" } else { $argList += "-nullrhi" }
  }
  if ($ExtraArgs) { $argList += $ExtraArgs }
  $p = Start-Process -FilePath $exe -ArgumentList ($argList -join " ") -NoNewWindow -PassThru -RedirectStandardOutput (Join-Path $projDir ("Saved\AgentTemp\" + $name + ".stdout.txt"))
  $null = $p.Handle
  try { $p.PriorityClass = "BelowNormal" } catch {}
  if (-not $p.WaitForExit($TimeoutSec * 1000)) { $p.Kill(); throw "Timed out after $TimeoutSec s" }
  Write-Output ("EXIT=" + $p.ExitCode + " LOG=" + $log)
  Select-String -LiteralPath $log -Pattern "BSKY|Traceback|Error:|LogPython: Error" | Select-Object -Last 60 | ForEach-Object { $_.Line }
  exit $p.ExitCode
} finally { Exit-UeLock $lock }
