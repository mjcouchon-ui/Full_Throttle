# One Unreal job per project at a time (editor scripts, cooks, packages). Dot-source it, then:
#   $lock = Enter-UeLock -ProjectDir <dir>   ...   Exit-UeLock $lock
# The lock folder holds owner.txt with the owner's process id; a lock whose owner is gone is stale and is taken over,
# so a killed wrapper can no longer block every later run for 30 minutes (review finding, 14 Nov).
function Enter-UeLock([string]$ProjectDir, [int]$WaitSec = 1800) {
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
      if (Test-Path -LiteralPath $ownerFile) { [int]::TryParse((Get-Content -LiteralPath $ownerFile -Raw).Trim(), [ref]$owner) | Out-Null }
      $alive = $false
      if ($owner -gt 0) { $alive = [bool](Get-Process -Id $owner -ErrorAction SilentlyContinue) }
      $age = (Get-Date) - (Get-Item -LiteralPath $lock -ErrorAction SilentlyContinue).CreationTime
      if (-not $alive -and ($owner -gt 0 -or $age.TotalSeconds -gt 60)) {
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
