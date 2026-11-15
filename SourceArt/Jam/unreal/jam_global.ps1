# The Unreal job queue on this PC: at most TWO Unreal jobs at a time, and they are always jobs of DIFFERENT checkouts.
# (15 Nov: with three checkouts and another session's playtest build all running editors and compiles at once, the
# machine ran out of memory, its hard disks were saturated, and each job took ten times as long.)
# Dot-source after ue_lock.ps1:   $g = Enter-JamGlobal -Checkout <project folder>   ...   Exit-UeLock $g
# Always take it BEFORE the checkout's own lock.
#
# Each checkout has its own queue (GlobalUnrealLock\queue_<checkout>): a waiter leaves a ticket there (named by its
# start time and process id; it holds the process id, the checkout's folder and the word "beat") and goes ahead only
# when it is the oldest live ticket of its checkout, the checkout's own lock is free, one of the two slots is free,
# AND no other checkout has an older ticket that could go now (its checkout free too). So jobs of one checkout run in
# the order they came, a job never holds a slot while it waits for its checkout, a busy checkout never keeps another
# one idle, and a free slot goes to whoever has waited longest. (Before that last rule a free slot went to whichever
# waiter happened to look first: on the afternoon of 15 Nov one checkout's jobs waited 25 minutes while two others
# took every slot in turn.)
# A waiter touches its own ticket every time it looks (every few seconds). A ticket that has not been touched for a
# minute is not waited for by other checkouts, whoever owns it: its owner got its slot and the ticket was left behind
# (a delete can fail while someone else is reading the file), or its owner was stopped and its process id has gone to
# another program, or it was written by an older copy of this file, which never touches its ticket (copies are still
# in use in checkouts this file is not copied to; their waiters take a free slot as they find it). Without this, one
# such ticket would have stopped every checkout's waiters with both slots free (review, 15 Nov).
# A slot is the same kind of lock as a checkout's (a folder with the owner's process id; a dead owner's lock is taken
# over): slot 1 in SkyScratch\GlobalUnrealLock, slot 2 in SkyScratch\GlobalUnrealLock\slot2.
# A wrapper that holds a checkout for a whole turn of several jobs (jam_turn.ps1) names the checkout in the
# environment variable JAM_HELD: Enter-JamGlobal for that checkout then returns "" at once.
function Test-JamCheckoutFree([string]$Checkout) {
  $lock = Join-Path $Checkout "Saved\ue.lock"
  if (-not (Test-Path -LiteralPath $lock)) { return $true }
  $ownerFile = Join-Path $lock "owner.txt"
  $owner = 0
  if (Test-Path -LiteralPath $ownerFile) { [int]::TryParse(("" + (Get-Content -LiteralPath $ownerFile -Raw -ErrorAction SilentlyContinue)).Trim(), [ref]$owner) | Out-Null }
  if ($owner -gt 0 -and -not (Get-Process -Id $owner -ErrorAction SilentlyContinue)) { return $true }   # stale: Enter-UeLock takes it over
  return $false
}
# The oldest live ticket in one checkout's queue folder, or $null. Removed on the way: tickets whose owner is gone,
# tickets over six hours old, and tickets of this version (third line "beat") that have not been touched for 90 s:
# their owner is alive but is not waiting here any more.
function Get-JamHead([string]$QueueDir) {
  foreach ($t in @(Get-ChildItem -LiteralPath $QueueDir -Filter *.txt -ErrorAction SilentlyContinue | Sort-Object Name)) {
    $tp = 0
    [int]::TryParse(($t.BaseName -split '_')[1], [ref]$tp) | Out-Null
    $dead = ((Get-Date) - $t.CreationTime).TotalHours -gt 6
    if (-not $dead -and $tp -gt 0 -and -not (Get-Process -Id $tp -ErrorAction SilentlyContinue)) { $dead = $true }
    if (-not $dead -and ((Get-Date) - $t.LastWriteTime).TotalSeconds -gt 90) {
      $lines = @(Get-Content -LiteralPath $t.FullName -ErrorAction SilentlyContinue)
      if ($lines.Count -ge 3 -and ("" + $lines[2]).Trim() -eq "beat") { $dead = $true }
    }
    if ($dead) {
      Remove-Item -LiteralPath $t.FullName -Force -ErrorAction SilentlyContinue
      continue
    }
    return $t
  }
  return $null
}
# The checkout a ticket waits for: its second line, or (a ticket of the earlier function, which wrote the process id
# only: copies of it are still in use) the folder its queue is named after, if there is one. $null when it cannot be
# told: such a ticket is not waited for.
function Get-JamTicketCheckout($Ticket) {
  $lines = @(Get-Content -LiteralPath $Ticket.FullName -ErrorAction SilentlyContinue)
  if ($lines.Count -ge 2 -and ("" + $lines[1]).Trim()) { return ("" + $lines[1]).Trim() }
  $name = $Ticket.Directory.Name -replace '^queue_', ''
  $guess = Join-Path (Join-Path "K:\Full Throttle" $name) "full-throttle-unreal"
  if ($name -and (Test-Path -LiteralPath $guess)) { return $guess }
  return $null
}
function Enter-JamGlobal([int]$WaitSec = 14400, [int]$Slots = 2, [string]$Checkout = "", [string]$Root = "K:\Full Throttle\SkyScratch\GlobalUnrealLock") {
  $root = $Root
  if (-not $Checkout) { $Checkout = if ($env:JAM_PORT) { $env:JAM_PORT } else { "K:\Full Throttle\UE\full-throttle-unreal" } }
  # inside a turn that jam_turn.ps1 already holds for this checkout (slot and checkout lock both): nothing to wait for
  if ($env:JAM_HELD -and ($env:JAM_HELD.TrimEnd('\') -ieq $Checkout.TrimEnd('\'))) { return "" }
  $q = Join-Path $root ("queue_" + (Split-Path -Leaf (Split-Path -Parent $Checkout)))
  New-Item -ItemType Directory -Force -Path $q | Out-Null
  $ticket = Join-Path $q ("{0:D20}_{1}.txt" -f (Get-Date).Ticks, $PID)
  $mine = Split-Path -Leaf $ticket
  $deadline = (Get-Date).AddSeconds($WaitSec)
  try {
    while ($true) {
      # the ticket's beat: (re)written if it is not there (a first look, or someone took it for a dead one while this
      # process was held up), touched otherwise
      try {
        if (Test-Path -LiteralPath $ticket) { [IO.File]::SetLastWriteTimeUtc($ticket, [DateTime]::UtcNow) }
        else { Set-Content -LiteralPath $ticket -Value @("$PID", $Checkout, "beat") -Encoding ascii }
      } catch { }
      $head = $null
      try { $head = Get-JamHead $q } catch { }
      $free = $false
      try { $free = Test-JamCheckoutFree $Checkout } catch { }
      if ($null -ne $head -and $head.FullName -eq $ticket -and $free) {
        # our checkout's turn. Is another checkout's turn older, and able to go? Then the free slot is its.
        $ahead = $false
        foreach ($oq in @(Get-ChildItem -LiteralPath $root -Directory -Filter "queue_*" -ErrorAction SilentlyContinue)) {
          if ($oq.FullName -eq $q) { continue }
          try {
            $oh = Get-JamHead $oq.FullName
            if ($null -eq $oh -or [string]::CompareOrdinal($oh.Name, $mine) -ge 0) { continue }   # none, or younger than ours
            $oh.Refresh()
            if (((Get-Date) - $oh.LastWriteTime).TotalSeconds -gt 60) { continue }                 # its owner has not looked for a minute
            $oc = Get-JamTicketCheckout $oh
            if ($oc -and (Test-JamCheckoutFree $oc)) { $ahead = $true; break }
          } catch { }
        }
        if (-not $ahead) {
          # take whichever slot is free (a held one is retried on the next round; the place in the queue is kept)
          foreach ($s in 1..$Slots) {
            $dir = if ($s -eq 1) { $root } else { Join-Path $root ("slot{0}" -f $s) }
            try { return (Enter-UeLock -ProjectDir $dir -WaitSec 1) } catch { }
          }
        }
      }
      if ((Get-Date) -gt $deadline) { throw "Unreal job queue: waited $WaitSec s without a turn" }
      Start-Sleep -Seconds 3
    }
  } finally {
    # the ticket must go: another waiter may be reading it at this moment, which makes one delete fail
    foreach ($try in 1..50) {
      Remove-Item -LiteralPath $ticket -Force -ErrorAction SilentlyContinue
      if (-not (Test-Path -LiteralPath $ticket)) { break }
      Start-Sleep -Milliseconds 100
    }
  }
}
