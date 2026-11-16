# One Unreal job per project at a time (editor scripts, cooks, packages). Dot-source it, then:
#   $lock = Enter-UeLock -ProjectDir <dir>   ...   Exit-UeLock $lock
# The lock folder holds owner.txt with the owner's process id; a lock whose owner is gone is stale and is taken over,
# so a killed wrapper can no longer block every later run for 30 minutes (review finding, 14 Nov).
# A wrapper that holds a project's lock for a whole turn of several jobs (Jam\unreal\jam_turn.ps1) names the project
# in the environment variable JAM_HELD: for that project Enter-UeLock then returns "" at once (the jobs it starts are
# inside its lock already) and Exit-UeLock "" releases nothing.
function Enter-UeLock([string]$ProjectDir, [int]$WaitSec = 1800) {
  if ($env:JAM_HELD -and ($env:JAM_HELD.TrimEnd('\') -ieq $ProjectDir.TrimEnd('\'))) { return "" }
  $lock = Join-Path $ProjectDir "Saved\ue.lock"
  New-Item -ItemType Directory -Force -Path (Join-Path $ProjectDir "Saved") | Out-Null
  $deadline = (Get-Date).AddSeconds($WaitSec)
  while ($true) {
    try {
      New-Item -ItemType Directory -Path $lock -ErrorAction Stop | Out-Null
      Set-Content -LiteralPath (Join-Path $lock "owner.txt") -Value $PID -Encoding ascii
      return $lock
    } catch {
      $ownerFile = Join-Path $lock "owner.txt"
      $owner = 0
      # (an owner file that is still empty, or a lock that its owner removed between the failed New-Item and here,
      # must not throw under $ErrorActionPreference = "Stop": both are simply "look again")
      if (Test-Path -LiteralPath $ownerFile) { [int]::TryParse(("" + (Get-Content -LiteralPath $ownerFile -Raw -ErrorAction SilentlyContinue)).Trim(), [ref]$owner) | Out-Null }
      $alive = $false
      if ($owner -gt 0) { $alive = [bool](Get-Process -Id $owner -ErrorAction SilentlyContinue) }
      $li = Get-Item -LiteralPath $lock -ErrorAction SilentlyContinue
      if (-not $li) { continue }
      $age = (Get-Date) - $li.CreationTime
      if (-not $alive -and ($owner -gt 0 -or $age.TotalSeconds -gt 60)) {
        # Two takers can both find the same dead owner. Each waits a moment of its own length and looks again: the one
        # that comes second then finds the first one's live lock and leaves it alone (it used to remove it).
        Start-Sleep -Milliseconds (Get-Random -Minimum 20 -Maximum 600)
        $again = 0
        if (Test-Path -LiteralPath $ownerFile) { [int]::TryParse(("" + (Get-Content -LiteralPath $ownerFile -Raw -ErrorAction SilentlyContinue)).Trim(), [ref]$again) | Out-Null }
        if ($again -ne $owner) { continue }
        # Write-Host, not Write-Output: anything written to the output stream becomes part of the return value, and the
        # caller's $lock would hold the message as well as the path, so Exit-UeLock never released it (review)
        Write-Host "ue.lock: taking over a stale lock (owner $owner gone)"
        Remove-Item -LiteralPath $lock -Force -Recurse -ErrorAction SilentlyContinue
        continue
      }
      if ((Get-Date) -gt $deadline) { throw "ue.lock held by process $owner for $WaitSec s: $lock" }
      Start-Sleep -Seconds 5
    }
  }
}
function Exit-UeLock([string]$Lock) {
  if ($Lock) { Remove-Item -LiteralPath $Lock -Force -Recurse -ErrorAction SilentlyContinue }
}
