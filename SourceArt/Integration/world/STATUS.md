# Final pass, Stage 1 (the world): status

Written 16 Nov 2026 on the second machine. Report rule: only what was run and seen. "Not checked" means exactly that.

## What was built

Every driving level is cut into named districts along the road (the plan of each is in `L<n>_<key>_s1.md`).
Per district one builder made buildings, props and a hero landmark; three more builders a level made plants,
animals and people for the gaps pass 3 left. Generators `Content/Python/ft_world_<key>_s1*.py`, meshes
`/Game/World/<Level>/SM_S1<ID>_<Name>`, table blocks at the end of `FTPeopleL<n>.inl` (a block at the end of the
dressing table would have re-rolled every population rule). One review a level (a reader of the rules and a driver,
neither of whom built it), one fix round, one compile.

Numbers: the game's `FTDRESS ... pieces placed=` line on the joined build with `main` (70f9fcd3) merged, one run a
level (`C:\ftwork\final\perf\s1\out_L<n>\run.log`). The road's `FTTRACK` line equals `track_baseline.txt` on all
eight; 0 `Fatal error`, 0 `LogFT: Error`, 0 failed loads of `/Game/World` in the eight logs.

| Level | Districts | New mesh files | Placed before | Placed after Stage 1 |
|---|---|---|---|---|
| 0 Bayshore | 7 | 187 | 13,415 | 15,805 |
| 1 Chemical Works | 8 | 204 | 11,878 | 14,312 |
| 2 Nevada | 8 | 205 | 11,589 | 14,345 |
| 3 Volcano | 8 | 208 | 11,436 | 14,381 |
| 4 Theme Park | 8 | 219 | 13,163 | 15,918 |
| 5 Ice Mountain | 8 | 210 | 11,796 | 14,852 |
| 6 Under the Sea | 8 | 194 | 13,377 | 16,025 |
| 7 City Center | 8 | 194 | 10,018 | 12,454 |

## Frame time, before and after (four frozen spots a level, 1920x1080, quality 3, 5 s each)

Before = `C:\ftwork\final\perf\BASELINE_DRIVING.md` (df6441f7). After = this build. Average frame ms and draw calls.

| Level | start | dense district | loop | boss run-in |
|---|---|---|---|---|
| 0 Bayshore | 5.5 > 5.4 ms, 2444 > 2520 | 6.3 > 6.1, 3054 > 3044 | 4.6 > 4.5, 994 > 984 | 5.8 > 5.8, 2876 > 2951 |
| 1 Chemical Works | 5.9 > 5.7, 2605 > 2595 | 5.7 > 5.7, 2931 > 2817 | 5.3 > 5.4, 2378 > 2575 | 5.9 > 5.8, 3039 > 2926 |
| 2 Nevada | 6.2 > 6.2, 2292 > 2365 | 6.2 > 6.1, 2463 > 2509 | 5.9 > 5.8, 2534 > 2512 | 6.3 > 6.2, 2849 > 2809 |
| 3 Volcano | 5.3 > 5.3, 1667 > 1751 | 5.5 > 5.5, 2078 > 2064 | 5.1 > 5.0, 1670 > 1639 | 4.9 > 5.1, 1465 > 1409 |
| 4 Theme Park | 5.4 > 5.6, 2512 > 2464 | 5.9 > 8.6, 2509 > 2503 | 5.7 > 5.6, 2449 > 2454 | 5.3 > 8.7, 1861 > 1889 |
| 5 Ice Mountain | 6.6 > 13.9, 2169 > 2215 | 5.7 > 13.5, 1789 > 1889 | 5.3 > 5.2, 1782 > 1826 | 5.7 > 5.6, 2082 > 2115 |
| 6 Under the Sea | 6.3 > 6.3, 1559 > 1643 | 5.2 > 5.2, 1564 > 1512 | 4.8 > 4.7, 807 > 802 | 6.1 > 6.1, 1882 > 1924 |
| 7 City Center | 5.4 > 5.3, 1977 > 1942 | 5.9 > 5.9, 2077 > 2205 | 5.3 > 5.2, 1982 > 1894 | 6.0 > 5.9, 2051 > 2087 |

The four slow captures (Theme Park dense and boss run-in, Ice Mountain start and dense) have the same draw calls and
triangles as before and a longer GPU time; another Unreal process of the brawl stage was running on this PC during
the run. They were not captured again with the PC idle: not checked.

## What the review found and what was done (details in each level's note)

- Bayshore: no broken rule found. New pieces standing inside the chair lift, the beach race, walkers' lanes and the
  film crew were moved.
- Chemical Works: a lit ring by the road removed; landmarks moved to open road.
- Nevada: new pieces standing inside older ones moved.
- Volcano: a ring-shaped gate (pickup-like) redrawn as a moon gate; people taken out of the lava channel.
- Theme Park: two props nearer than 17 m moved out.
- Ice Mountain: heroes made readable; pieces standing in each other moved.
- Under the Sea: heroes moved to where a player sees them; near pieces off the banked bends.
- City Center: one mesh placed more often than the cap thinned; a hedge that read as a green box redrawn.

## Left as found

- Nevada, Ice Mountain, Under the Sea: the reviewers' drives ended before the last two or three districts (the escape
  clock or the level's end); what stands there was built by the table and not seen in a drive.
- Heroes that stand far out behind tall rows are hard to make out on Theme Park and parts of Nevada.
- Many Stage 1 meshes are over the 1,500-triangle guide; several rules are over the per-builder piece guides.
- Residual box overlaps with older pieces (counts in the notes).

## Not checked

- A whole-level drive of any level after the fix round on the joined build (one start and four frozen spots each).
- Movement of plants and animals at racing speed.
- Instances in view and distinct meshes in view (no counter was found for them).
- Memory and load time after Stage 1 were captured in the logs but not tabulated here.
