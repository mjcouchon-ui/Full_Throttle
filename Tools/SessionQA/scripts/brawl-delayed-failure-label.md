Use `work/run-session-probe.ps1 -Game brawl` with a new profile, script `brawl-delayed-failure-label.txt`, TimeoutSeconds90 and Extra `-brawlplay`,`-brawlnocut`. It opens Rusty Anchor explicitly. No engine was launched while preparing this test.

The recorded `FT BRAWL QA SAVE` snapshots should show:

1. Initial saving0/error0, staged=night:0:0, durable=night:0:0.
2. During delayed failed result and queued-close screen7: saving1, staged=nightoutro:0:2, durable=night:0:0. Label must still say restart wave1 and retain its original saved timestamp.
3. Immediately after Cancel: screen2, saving1, same old durable checkpoint. This proves the menu responds while I/O is running.
4. After failure and on screen6: saving0/error1, staged outro2, durable wave1. The failure screenshot must visibly name the old checkpoint, not the unsaved boss result.
5. During successful delayed Retry: saving1, old durable wave1. Native close queues again, with no actual desktop exit yet.
6. After successful completion: screen5, saving0/error0, durable=nightoutro:0:2 and updated saved timestamp. Only the subsequent confirmation should emit `exit=desktop discard=0`.

Require both actual `exit queued=desktop` events, the Cancel transition7→2 before failed completion, one failed BossDefeatedTest save, one successful Exit retry, no script timeout/fatal/ensure, and process exit0. The `SAVING...` screenshots must show a real pending worker state. The final save-snapshot primary should be version1/kindnightoutro/level0/stage2; its backup should remain a valid previous wave checkpoint. No physical-controller claims are made by this injected-input test.
