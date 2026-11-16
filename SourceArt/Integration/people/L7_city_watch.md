# L7 City Center, the watchers (extra worker "watch", 15 Nov 2026)

Generator `Content/Python/ft_people_city_watch.py`; rules: the `watch` block of `Source/FullThrottle/FTPeopleL7.inl`;
meshes `/Game/People/CityCenter/SM_Watch*`. Branch `claude/people-L7-w`, worktree `C:\ftwork\wt\L7w`.
Only what was run and seen is reported here.

## What a player now sees

The city has climbed onto everything to watch the chase. Over the mouths of the side streets stand footbridges with a
yellow or cyan girder, a row of people at the rail and a second row behind, flags, a string of lights and three striped
banners hung under the deck. In the alleys' mouths stand scaffold stands of four steps: a police barricade, a police
line on the first step, three rows of people above, placards, flags and a banner on two poles. Up the brick buildings
behind the alleys run scaffolds with three decks of people behind coloured cloths, 13 to 26 m up. The garages' second
and third decks have people along their parapets with cloths and hung banners (also on the one garage the Syndicate's
territory has, every 8th block on the left). The dear doors' roof terraces have a crowd behind a striped cloth with four
tall flags and three banners down the facade, from the Plaza to the end of the level.

## The meshes (all TALL, all 24 m or more out, all lit steadily: figures 0.11, cloth 0.17, bulbs 1.6; nothing blinks)

| mesh | shows | function | triangles | moves | where the table puts it | placed |
|---|---|---|---|---|---|---|
| SM_WatchBridge_A | footbridge over a side street, 15 people, flags, 3 hung banners, bulbs | `bridge(1)` | 2048 | each figure hops and waves out of step (react), flags wave, banners stir | G_Street gaps, 26.6 m out | 23 |
| SM_WatchBridge_B | the same, cyan girder, three police among them | `bridge(21, police=True)` | 2180 | as A; police rock | G_Street gaps | 28 |
| SM_WatchSteps_A | scaffold stand of four steps, police line and barricade, 17 people, banner | `steps(3)` | 2272 | as above | G_Alley mouths, 27.1 m out | 33 |
| SM_WatchBalcony_A | scaffold up SM_Infill1, three decks of 7 people (1.3 x life size), cloths, 6 hung banners | `balcony(5)` | 1992 | as above | at SM_Infill1 (its own offsets) | 26 |
| SM_WatchDeck_A | 24 people on the garage's 2nd and 3rd decks, cloths, 2 hung banners | `deck(7)` | 1732 | as above | at SM_GarageRamp (its own offsets) | 40 |
| SM_WatchRoof_A | 15 people behind a striped cloth on a roof edge, 4 tall flags, 3 banners down the front, bulbs | `roof(9)` | 1396 | as above | W_Pent roofs, 5.05 m along from the middle, up 9.6 m | 73 |

Placed total 223 (from the FTCLEAR list of the first drive; the rules were not changed after it). Pushed out by the
track ("~"): bridges 9 and 12, steps 18, balcony 6, deck 6, roof 13.
FTTRACK level=7 frames=12717 route=5faaeb1d solid=25fa2576 solidTriangles=957884: equal to the baseline line.
FTDRESS level=7 pieces placed=7415 of 7433 tried (dressing and these pieces; the main cast is not in this worktree).

## What was looked at

Six game runs (start twice, from scene 3, start again, scene 4, scene 6), frames cropped and enlarged; the sheet
`sheets/L7_watch_game.jpg` has six crops: a bridge (before the figures were brightened), a stand with the scaffold
above it (left), a stand over the barrier (right), a roof terrace twice, the garage decks in Syndicate Territory
(before brightening). First drive: figures read dark under the night; figure light raised 0.05 -> 0.11 (balcony 0.13),
cloth 0.14 -> 0.17, meshes rebuilt, looked at again (stand, scaffold, roof: figures now read).

## Weak

- Five of six pieces are over the 1,500-triangle guide (1,700 to 2,270): a watcher piece is a crowd of 15 to 24 with
  its structure. The stand is the heaviest (its two barricades and four police with caps).
- A blade sign of the dressing (Scatter at 26 m) can stand in front of a roof crowd: seen once (CLUB, the Plaza).
- A roof sign of the dressing hides part of the scaffold behind an alley from some angles (seen at the start, JAZZ).
- The dressing's alleys have chance 0.8 to 0.9 with dice of their own, so about one stand or scaffold in ten stands in
  a gap with no alley and no brick building: not seen in a frame, follows from the rule.
- The roof piece does not avoid the terrace's parasols and tubs (heads may touch a parasol's rim): not seen close.
- The garage decks are thin for their 30 m (24 people).
- One variant only of the stand, scaffold, deck and roof (the six-mesh limit).

## Not checked

- Movement in the game: the film frames taken (five films) did not hold one of these pieces steadily enough to compare
  poses; that the pieces move rests on the builder's motion values, not on frames.
- SM_WatchBridge_B and the bridges in general after brightening; the roof piece in Syndicate Territory; the balcony up
  close; any piece from the right-hand lane in the later districts.
- Whether pieces pushed out by the track ("~") still sit on their roofs, decks and gaps.
- The pieces beside the main cast's (this worktree has only the dressing and the watchers): overlaps with the main
  cast's roof parties, cradles and crew were avoided by rule, not looked at.
- Frame rate, memory.

## ASKS

- The game's `waitfor FT CLOCK level=N.0` missed often on this loaded PC (the clock line skips); `wait <seconds>` worked.
- A table word that ties a rule to another rule's dice (so that a piece stands only where the dressing's alley was
  actually placed) would remove the one-in-ten stray stand.
- The lead joins the `watch` block into FTPeopleL7.inl after the main worker's rules; its sets are all named PplWatch*.
