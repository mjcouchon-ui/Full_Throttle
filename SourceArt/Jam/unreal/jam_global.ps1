# The Unreal job queue on this PC: at most FOUR Unreal jobs at a time, and they are always jobs of DIFFERENT checkouts.
# (Two until 15 Nov 16:00, while another session's playtest shared the PC. Four is what the PC carries alone, with
# the owner's word to push it (measured then: 11 GB of the 32 free and 17 GB of commit left with one game up): a
# staged fight runs at 100 frames a second and more with other jobs beside it and a game takes 4 to 5 GB of the 32,
# but the two hard disks are full with two games LOADING at once, so loads stretch when three start together.)
# (15 Nov: with three checkouts and another session's playtest build all running editors and compiles at once, the
# machine ran out of memory, its hard disks were saturated, and each job took ten times as long.)
# Dot-source after ue_lock.ps1:   $g = Enter-JamGlobal -Checkout <project folder>   ...   Exit-UeLock $g
# Always take it BEFORE the checkout's own lock.
#
# Each checkout has its own queue (GlobalUnrealLock\queue_<checkout>): a waiter leaves a ticket there (named by its
# start time and process id; it holds the process id, the checkout's folder and the word "beat") and goes ahead only
# when it is the oldest live ticket of its checkout, the checkout's own lock is free, one of the slots is free,
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
# over): slot 1 in SkyScratch\GlobalUnrealLock, slots 2 to 4 in SkyScratch\GlobalUnrealLock\slot2 ... \slot4.
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
# Memory. An Unreal job (an editor job, a game at Epic, a compile of four files at once) commits 8 to 10 GB, and this
# PC's commit limit is 32 GB of memory plus a fixed 33 GB page file, of which everything else that runs here already
# holds about half. On 15 Nov four jobs at once stood at 97 % of the limit (0.4 GB of memory free): one more
# allocation and a job dies with "ran out of memory". So a waiter takes a free slot only when at least $NeedGB of the
# commit limit are free AND the last job to start did so at least $ApartSec ago (a job's memory grows for half a
# minute after its start, so two that start together both see the room that only one of them can have). If nothing at
# all is running (no slot held) the job goes whatever the numbers say: waiting could not make more room.
# Anything that goes wrong in here lets the job go (the slots alone are then the limit, as before).
function Test-JamRoom([string]$Root, [int]$Slots, [double]$NeedGB = 11.0, [int]$ApartSec = 40) {
  try {
    $held = 0
    foreach ($s in 1..$Slots) {
      $dir = if ($s -eq 1) { $Root } else { Join-Path $Root ("slot{0}" -f $s) }
      if (-not (Test-JamCheckoutFree $dir)) { $held++ }
    }
    if ($held -eq 0) { return $true }
    $mark = Join-Path $Root "laststart.txt"
    if (Test-Path -LiteralPath $mark) {
      if (((Get-Date) - (Get-Item -LiteralPath $mark).LastWriteTime).TotalSeconds -lt $ApartSec) { return $false }
    }
    $os = Get-CimInstance Win32_OperatingSystem -ErrorAction Stop
    return (($os.FreeVirtualMemory / 1MB) -ge $NeedGB)
  } catch { return $true }
}
function Enter-JamGlobal([int]$WaitSec = 14400, [int]$Slots = 4, [string]$Checkout = "", [string]$Root = "K:\Full Throttle\SkyScratch\GlobalUnrealLock") {
  $root = $Root
  # A maintenance gate: while the file HOLD.txt stands in the queue's folder, no new Unreal job starts, not even the
  # next step of a turn that already holds its checkout (15 Nov: the engine was moved to a faster drive, which needs
  # a moment when nothing runs from its folder). The file's first line says why; delete the file to let all go on.
  $holdFile = Join-Path $Root "HOLD.txt"
  $holdSaid = $false
  while (Test-Path -LiteralPath $holdFile) {
    if (-not $holdSaid) { Write-Host ("Unreal job queue: HELD by " + $holdFile + ": " + (Get-Content -LiteralPath $holdFile -TotalCount 1 -ErrorAction SilentlyContinue) + " (waiting)"); $holdSaid = $true }
    Start-Sleep -Seconds 5
  }
  if ($holdSaid) { Write-Host ("Unreal job queue: hold lifted {0:HH:mm:ss}" -f (Get-Date)) }
  if (-not $Checkout) { $Checkout = if ($env:JAM_PORT) { $env:JAM_PORT } else { "K:\Full Throttle\UE\full-throttle-unreal" } }
  # inside a turn that jam_turn.ps1 already holds for this checkout (slot and checkout lock both): nothing to wait for
  if ($env:JAM_HELD -and ($env:JAM_HELD.TrimEnd('\') -ieq $Checkout.TrimEnd('\'))) { return "" }
  $q = Join-Path $root ("queue_" + (Split-Path -Leaf (Split-Path -Parent $Checkout)))
  New-Item -ItemType Directory -Force -Path $q | Out-Null
  $ticket = Join-Path $q ("{0:D20}_{1}.txt" -f (Get-Date).Ticks, $PID)
  $mine = Split-Path -Leaf $ticket
  $idleSince = $null
  $deadline = (Get-Date).AddSeconds($WaitSec)
  try {
    while ($true) {
      # the ticket's beat: (re)written if it is not there (a first look, or someone took it for a dead one while this
      # process was held up), touched otherwise
      try {
        if (Test-Path -LiteralPath $ticket) { [IO.File]::SetLastWriteTimeUtc($ticket, [DateTime]::UtcNow) }
        else { Set-Content -LiteralPath $ticket -Value @("$PID", $Checkout, "beat") -Encoding ascii }
      } catch { }
      # the maintenance gate (see the top of this function): keep the place in the queue, take nothing
      if (Test-Path -LiteralPath $holdFile) { Start-Sleep -Seconds 5; continue }
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
        # An older waiter that could go takes a free slot within a few seconds. If a slot has stood free for 20 s all
        # the same, that waiter cannot use it (it runs an older copy of this file that knows fewer slots) or is held
        # up: the slot is not left idle for it.
        if ($ahead) {
          $idle = $false
          foreach ($s in 1..$Slots) {
            $dir = if ($s -eq 1) { $root } else { Join-Path $root ("slot{0}" -f $s) }
            try { if (Test-JamCheckoutFree $dir) { $idle = $true } } catch { }
          }
          if (-not $idle) { $idleSince = $null }
          elseif ($null -eq $idleSince) { $idleSince = Get-Date }
          elseif (((Get-Date) - $idleSince).TotalSeconds -ge 20) { $ahead = $false }
        } else { $idleSince = $null }
        if (-not $ahead) {
          # take whichever slot is free (a held one is retried on the next round; the place in the queue is kept)
          if (Test-JamRoom $root $Slots) {
            foreach ($s in 1..$Slots) {
              $dir = if ($s -eq 1) { $root } else { Join-Path $root ("slot{0}" -f $s) }
              try {
                $got = Enter-UeLock -ProjectDir $dir -WaitSec 1
                try { Set-Content -LiteralPath (Join-Path $root "laststart.txt") -Value ("{0:HH:mm:ss} {1}" -f (Get-Date), $Checkout) -Encoding ascii } catch { }
                return $got
              } catch { }
            }
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
