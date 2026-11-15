param(
    [Parameter(Mandatory=$true)][string]$BuildRoot,
    [Parameter(Mandatory=$true)][string]$Output,
    [ValidateSet('shots','bench')][string]$Mode = 'shots',
    [int[]]$Levels = @(0,1,2,3,4,5,6,7),
    [int[]]$Qualities = @(3,0),
    [int]$Seconds = 24,
    [string]$ExtraCmds = '',
    [string]$EditorProject = ''
)
$ErrorActionPreference = 'Stop'
if ($Mode -eq 'bench' -and $ExtraCmds -match 'ft\.PickupReview') { throw 'Inspection proxies are visual evidence only. Use the normal game for timings.' }
$BuildRoot = (Resolve-Path -LiteralPath $BuildRoot).Path
$Output = [System.IO.Path]::GetFullPath($Output, $PWD.Path)
$launchPrefix = @()
$gameModuleHash = $null
$sourceRevision = $null
$sourceParents = @()
$sourceTrackedChanges = @()
if ($EditorProject) {
    if ($Mode -ne 'shots') { throw 'Uncooked editor runs are visual checks only.' }
    if (-not (Test-Path -LiteralPath $EditorProject)) { throw "Missing project: $EditorProject" }
    $EditorProject = (Resolve-Path -LiteralPath $EditorProject).Path
    $exe = Join-Path $BuildRoot 'UnrealEditor-Cmd.exe'
    $launchPrefix = @("`"$EditorProject`"", '-game')
    $gameModule = Join-Path (Split-Path -Parent $EditorProject) 'Binaries\Win64\UnrealEditor-FullThrottle.dll'
    $gameModuleHash = (Get-FileHash -LiteralPath $gameModule -Algorithm SHA256).Hash
    $sourceDir = Split-Path -Parent $EditorProject
    $sourceRevision = (& git -C $sourceDir rev-parse HEAD).Trim()
    if ($LASTEXITCODE -ne 0) { throw 'Cannot record source revision for uncooked capture.' }
    $sourceParents = @((& git -C $sourceDir show -s --format=%P HEAD).Trim() -split ' ')
    $sourceTrackedChanges = @(& git -C $sourceDir status --porcelain --untracked-files=no)
} else {
    $exe = Join-Path $BuildRoot 'FullThrottle\Binaries\Win64\FullThrottle.exe'
}
if (-not (Test-Path -LiteralPath $exe)) { throw "Missing Development executable: $exe" }
if (Test-Path -LiteralPath $Output) { throw "Refusing to overwrite: $Output" }
New-Item -ItemType Directory -Path $Output | Out-Null
$binaryHash = (Get-FileHash -LiteralPath $exe -Algorithm SHA256).Hash
foreach ($quality in $Qualities) {
    foreach ($level in $Levels) {
        $run = Join-Path $Output "L$level-Q$quality"
        New-Item -ItemType Directory -Path $run | Out-Null
        $gpuBefore = @()
        if ($Mode -eq 'bench') {
            for ($sample = 0; $sample -lt 5; $sample++) {
                $util = (& nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader,nounits).Trim()
                $gpuBefore += [int]$util
                Start-Sleep -Seconds 1
            }
            if (($gpuBefore | Measure-Object -Maximum).Maximum -gt 5) { throw "GPU is not idle: $($gpuBefore -join ',') percent. No timing run launched." }
        }
        $commands = 'DisableAllScreenMessages,r.ScreenPercentage 100,t.MaxFPS 0,r.VSync 0'
        if ($ExtraCmds) { $commands += ',' + $ExtraCmds }
        $runArgs = $launchPrefix + @('-windowed','-ResX=3440','-ResY=1440','-RenderOffScreen','-nosound','-unattended','-nosplash',
            '-ftbench',"-ftlevel=$level","-ftquality=$quality","-ftseconds=$Seconds",'-ftnodynres','-FixedSeed','-UseFixedTimeStep','-FPS=60','-ftslot=pie',
            "-ExecCmds=`"$commands`"","-abslog=`"$(Join-Path $run 'game.log')`"")
        if ($Mode -eq 'shots') { $runArgs += '-ftshots=0.25' }
        [ordered]@{mode=$Mode;level=$level;quality=$quality;seconds=$Seconds;exe=$exe;exe_sha256=$binaryHash;game_module_sha256=$gameModuleHash;source_revision=$sourceRevision;source_parents=$sourceParents;source_tracked_changes=$sourceTrackedChanges;arguments=$runArgs;gpu_idle_samples=$gpuBefore;started=(Get-Date).ToString('o')} |
            ConvertTo-Json -Depth 4 | Set-Content -LiteralPath (Join-Path $run 'manifest.json') -Encoding utf8NoBOM
        $p = Start-Process -FilePath $exe -ArgumentList $runArgs -WorkingDirectory (Split-Path -Parent $exe) -WindowStyle Hidden -PassThru -Environment @{WISPOS_APP_DATA=$run}
        $deadline = (Get-Date).AddMinutes(10)
        while (-not $p.WaitForExit(1000)) { if ((Get-Date) -gt $deadline) { $p.Kill(); throw "Capture timed out: $run" } }
        $count = @(Get-ChildItem -LiteralPath (Join-Path $run 'shots') -Filter '*.png' -ErrorAction SilentlyContinue).Count
        "POWERUPS $Mode L$level Q$quality exit=$($p.ExitCode) screenshots=$count"
        if ($p.ExitCode -ne 0) { throw "Run failed: $run" }
        if ($Mode -eq 'shots' -and $count -eq 0) { throw "No screenshots: $run" }
    }
}
