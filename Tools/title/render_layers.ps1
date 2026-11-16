# Renders the title logo's layers with headless Edge and builds the animated logo's textures.
#   powershell -NoProfile -ExecutionPolicy Bypass -File Tools\title\render_layers.ps1
# Writes L_throttle / L_star1 / L_star2 / L_full / L_lines .png (1600x608, one part of logo.html each) beside this file,
# then runs make_layers.py (the eleven T_TitleLogo_*.png and layers.json). The flat logo is rendered too, but only to
# the scratch folder as flat_check.png, to prove logo.html still draws T_TitleLogo.png: it is never written over it.
# Import into the project afterwards with Content/Python/ft_title_logo_import.py.
param(
  [string]$Scratch = "C:\ftreel\titlework\A",
  [string]$Edge = "C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe"
)
$ErrorActionPreference = "Stop"
$here = $PSScriptRoot
New-Item -ItemType Directory -Force $Scratch | Out-Null
$edgeProfile = Join-Path $Scratch "edgeprofile"
$page = "file:///" + (($here -replace '\\', '/') -replace ' ', '%20') + "/logo.html"

function Render([string]$Out, [string]$Layer) {
  $url = $page
  if ($Layer) { $url = $page + "?layer=" + $Layer }
  if (Test-Path -LiteralPath $Out) { Remove-Item -LiteralPath $Out }
  $a = @("--headless=new", "--disable-gpu", "--hide-scrollbars", "--allow-file-access-from-files",
         "--default-background-color=00000000", "--window-size=1600,608", "--force-device-scale-factor=1",
         "--virtual-time-budget=8000", ('--user-data-dir="' + $edgeProfile + '"'), ('--screenshot="' + $Out + '"'), ('"' + $url + '"'))
  $p = Start-Process -FilePath $Edge -ArgumentList $a -PassThru -WindowStyle Hidden
  $p.WaitForExit()
  # Edge can hand the job to a process that outlives the one started here: wait for the file itself
  for ($i = 0; $i -lt 100 -and -not (Test-Path -LiteralPath $Out); $i++) { Start-Sleep -Milliseconds 200 }
  if (-not (Test-Path -LiteralPath $Out)) { throw "Edge did not write $Out" }
  Start-Sleep -Milliseconds 300
  "rendered " + $Out + " (" + (Get-Item -LiteralPath $Out).Length + " bytes)"
}

$flat = Join-Path $Scratch "flat_check.png"
Render $flat ""
foreach ($n in "throttle", "star1", "star2", "full", "lines") { Render (Join-Path $here ("L_" + $n + ".png")) $n }

$same = (Get-FileHash -LiteralPath $flat).Hash -eq (Get-FileHash -LiteralPath (Join-Path $here "T_TitleLogo.png")).Hash
"flat_check.png identical to T_TitleLogo.png: " + $same

& python (Join-Path $here "make_layers.py")
if ($LASTEXITCODE -ne 0) { throw "make_layers.py failed ($LASTEXITCODE)" }
