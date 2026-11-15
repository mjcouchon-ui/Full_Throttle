# The Unreal job queue on this PC: at most TWO Unreal jobs at a time, and they are always jobs of DIFFERENT checkouts.
# (15 Nov: with three checkouts and another session's playtest build all running editors and compiles at once, the
# machine ran out of memory, its hard disks were saturated, and each job took ten times as long.)
# Dot-source after ue_lock.ps1:   $g = Enter-JamGlobal -Checkout <project folder>   ...   Exit-UeLock $g
# Always take it BEFORE the checkout's own lock.
#
# Each checkout has its own queue (GlobalUnrealLock\queue_<checkout>): a waiter leaves a ticket there (its start time
# and process id) and goes ahead only when it is the oldest live ticket of its checkout, the checkout's own lock is
# free, and one of the two slots is free. So jobs of one checkout run in the order they came, a job never holds a slot
# while it waits for its checkout, and a busy checkout never keeps the other one idle (the first queue here was one
# line for everything: jobs for the idle checkout sat behind jobs for the busy one).
# A slot is the same kind of lock as a checkout's (a folder with the owner's process id; a dead owner's lock is taken
# over): slot 1 in SkyScratch\GlobalUnrealLock, slot 2 in SkyScratch\GlobalUnrealLock\slot2.
function Test-JamCheckoutFree([string]$Checkout) {
  $lock = Join-Path $Checkout "Saved\ue.lock"
  if (-not (Test-Path -LiteralPath $lock)) { return $true }
  $ownerFile = Join-Path $lock "owner.txt"
  $owner = 0
  if (Test-Path -LiteralPath $ownerFile) { [int]::TryParse(("" + (Get-Content -LiteralPath $ownerFile -Raw -ErrorAction SilentlyContinue)).Trim(), [ref]$owner) | Out-Null }
  if ($owner -gt 0 -and -not (Get-Process -Id $owner -ErrorAction SilentlyContinue)) { return $true }   # stale: Enter-UeLock takes it over
  return $false
}
function Enter-JamGlobal([int]$WaitSec = 14400, [int]$Slots = 2, [string]$Checkout = "") {
  $root = "K:\Full Throttle\SkyScratch\GlobalUnrealLock"
  if (-not $Checkout) { $Checkout = if ($env:JAM_PORT) { $env:JAM_PORT } else { "K:\Full Throttle\UE\full-throttle-unreal" } }
  $q = Join-Path $root ("queue_" + (Split-Path -Leaf (Split-Path -Parent $Checkout)))
  New-Item -ItemType Directory -Force -Path $q | Out-Null
  $ticket = Join-Path $q ("{0:D20}_{1}.txt" -f (Get-Date).Ticks, $PID)
  Set-Content -LiteralPath $ticket -Value $PID -Encoding ascii
  $deadline = (Get-Date).AddSeconds($WaitSec)
  try {
    while ($true) {
      $first = $null
      foreach ($t in @(Get-ChildItem -LiteralPath $q -Filter *.txt -ErrorAction SilentlyContinue | Sort-Object Name)) {
        $tp = 0
        [int]::TryParse(($t.BaseName -split '_')[1], [ref]$tp) | Out-Null
        $old = ((Get-Date) - $t.CreationTime).TotalHours -gt 6
        if ($old -or ($tp -gt 0 -and -not (Get-Process -Id $tp -ErrorAction SilentlyContinue))) {
          Remove-Item -LiteralPath $t.FullName -Force -ErrorAction SilentlyContinue   # its owner is gone
          continue
        }
        $first = $t.FullName
        break
      }
      if ($first -eq $ticket -and (Test-JamCheckoutFree $Checkout)) {
        # our turn: take whichever slot is free (a held one is retried on the next round; the place in the queue is kept)
        foreach ($s in 1..$Slots) {
          $dir = if ($s -eq 1) { $root } else { Join-Path $root ("slot{0}" -f $s) }
          try { return (Enter-UeLock -ProjectDir $dir -WaitSec 1) } catch { }
        }
      }
      if ((Get-Date) -gt $deadline) { throw "Unreal job queue: waited $WaitSec s without a turn" }
      Start-Sleep -Seconds 3
    }
  } finally { Remove-Item -LiteralPath $ticket -Force -ErrorAction SilentlyContinue }
}
