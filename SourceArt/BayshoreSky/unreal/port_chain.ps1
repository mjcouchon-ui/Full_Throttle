# Build the sky in SkyScratch, carry it into the port, apply it to Level 1 and package the port.
#   -Stages: comma-separated, any of build, mat, copy, apply, package, play (default: build,copy,apply,package, in that
#   order). A plain string, because powershell -File passes "a,b,c" as one string, not an array (a [string[]] parameter
#   silently matched nothing).
#   mat  = materials and MPC/preset values only (30, 45): the quick loop for shader and look-value changes
#   play = in-game shots of the uncooked port (play_uncooked.ps1 -Tag $Tag), for fast look checks
param([string]$Stages = "build,copy,apply,package", [string]$Config = "Development", [string]$Tag = "chain", [int]$Seconds = 40)
$ErrorActionPreference = "Continue"
$st = @($Stages -split "[,\s]+" | Where-Object { $_ })     # a new variable: $Stages is typed [string] and would re-join
$unknown = @($st | Where-Object { $_ -notin @("build", "mat", "copy", "apply", "package", "play") })
if ($unknown.Count) { throw "unknown stage(s): $($unknown -join ', ')" }
"stages: $($st -join ', ')"
$u = Split-Path -Parent $MyInvocation.MyCommand.Path
$scratch = "K:\Full Throttle\SkyScratch"
$port = "K:\Full Throttle\UE\full-throttle-unreal"
$portProj = "$port\FullThrottle.uproject"
function Run($script, $project) {
  $pa = @("-NoProfile", "-File", "$u\ue_run.ps1", "-Editor", "-Script", "$u\$script")
  if ($project) { $pa += @("-Project", $project) }
  "== $script $(if ($project) { '(port)' })"
  $out = @(powershell @pa 2>&1 | ForEach-Object { "$_" })
  $code = $LASTEXITCODE
  $out | Where-Object { $_ -match "BSKY |Traceback|Error: |error X|STAND-IN|ft_sky|EXIT=|ue.lock|Timed out" -and $_ -notmatch 'RHIResources|HttpListener|ModelContext' } |
    Select-Object -First 25 | ForEach-Object { $_.Substring([Math]::Max(0, $_.IndexOf('LogPython'))) }
  # stop the chain on a failed step: a later stage would otherwise package or copy a half-built sky (review)
  if ($code -ne 0 -or ($out -match "Traceback|LogPython: Error")) { throw "$script failed (exit $code); see Saved\AgentTemp\$([IO.Path]::GetFileNameWithoutExtension($script)).log" }
}
function Docs {
  python -I "$u\doc_params.py" "$scratch\Plugins\BayshoreSky\INTEGRATION.md"
  if ($LASTEXITCODE -ne 0) { throw "doc_params.py failed" }
}
if ($st -contains "build") {
  foreach ($s in "21_import_textures.py", "30_materials.py", "41_bp_tweaks.py", "45_preset_values.py", "82_place_cumulus.py") { Run $s $null }
  Docs
} elseif ($st -contains "mat") {
  foreach ($s in "30_materials.py", "45_preset_values.py") { Run $s $null }
  Docs
}
if ($st -contains "copy") {
  # under the port's Unreal lock, so no editor job or cook reads the plugin while it is being replaced
  . "$u\ue_lock.ps1"
  $lock = Enter-UeLock -ProjectDir $port
  try {
    robocopy "$scratch\Plugins\BayshoreSky" "$port\Plugins\BayshoreSky" /MIR /XD Intermediate Binaries Saved /NFL /NDL /NJH /NJS /NP /R:2 /W:2 | Out-Null
    $rc1 = $LASTEXITCODE
    robocopy "$scratch\SourceArt\BayshoreSky" "$port\SourceArt\BayshoreSky" /E /XD raw Test __pycache__ pre_golden _placeholder /NFL /NDL /NJH /NJS /NP /R:2 /W:2 | Out-Null
    $rc2 = $LASTEXITCODE
  } finally { Exit-UeLock $lock }
  if ($rc1 -ge 8 -or $rc2 -ge 8) { throw "copy into the port failed (robocopy $rc1 / $rc2)" }
  "== copied plugin and source art into the port (robocopy $rc1 / $rc2)"
}
if ($st -contains "apply") {
  Run "82_place_cumulus.py" $portProj
  Run "80_port_apply.py" $portProj
}
if ($st -contains "package") {
  "== package $Config"
  powershell -NoProfile -File "$u\package_port.ps1" -Config $Config
  if ($LASTEXITCODE -ne 0) { throw "package failed (exit $LASTEXITCODE)" }
}
if ($st -contains "play") {
  "== play (uncooked) $Tag"
  powershell -NoProfile -File "$u\play_uncooked.ps1" -Tag $Tag -Seconds $Seconds -Every 5
}
