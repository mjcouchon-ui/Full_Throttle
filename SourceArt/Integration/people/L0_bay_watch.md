# Bayshore (level 0): the watchers (extra worker "watch", 15 Nov 2026)

Branch `claude/people-L0-w`, worktree `C:\ftwork\wt\L0w`. Generator `Content/Python/ft_people_bay_watch.py`
(`import ft_people_bay_watch as K; K.run()`), meshes `/Game/People/Bayshore/SM_Watch*`, rules: the block
`BEGIN watch` ... `END watch` in `Source/FullThrottle/FTPeopleL0.inl`. Only what was run and seen is reported.

## What a player now sees

The town has built itself places to watch the chase from. On the bay side, right behind the boardwalk, a scaffold
grandstand every 180 m or so (pink and teal, or yellow and coral): four tiers of holidaymakers a quarter larger than
life, hopping out of step with both arms up, two of them swinging big two-colour flags, pennants on the back boards.
From the left lane a stand is a block of hot colour full of moving people for about two seconds (frames: sheet
`sheets/L0_watch_1.jpg`, top row and bottom right). On the town side a scaffold platform 4.3 m up stands in the mouth
of every second side street, eight people at its rail in front of a striped board (sheet, middle row): it shows over
the barrier and beside the shop rows, also over the banked barrier of the first bend. In the other side streets a
hotel's two-floor sun terrace bridges the street, guests at both rails (sheet, bottom left).

## Meshes (6)

| Mesh | What it shows | Function | Triangles | Tall | What moves | Where the table puts it | Placed |
|---|---|---|---|---|---|---|---|
| SM_WatchStand_A | grandstand, pink/white/teal, a sun on the back boards, 24 people | `stand(101, 0)` | 3484 | TALL 9.8 m | every figure hops or rocks, arms swing, 2 or 3 hand flags, 3 pennants | bay, 30 m, 15 m along from the slot's middle | 50 (1 pushed out) |
| SM_WatchStand_B | the same in yellow/white/coral, a wave on the boards | `stand(102, 1)` | 3632 | TALL 9.8 m | as A | bay, 30 m | not counted separately |
| SM_WatchTower_A | scaffold platform 4.3 m up, teal/white board, 7 people, a flag | `tower(201, 0)` | 1752 | TALL 9.9 m | figures hop, arms swing, hand flag, pennant | town 31 m in side-street mouths; bay 28 m at the end of an arcade's slot | 61 (5 pushed out) |
| SM_WatchTower_B | the same in coral/white/yellow | `tower(202, 1)` | 1716 | TALL 9.9 m | as A | bay 28 m; town 31 m at the Point | 26 (1 pushed out) |
| SM_WatchDeck | a beach bar's deck on stilts, parasols, 13 people at the rail | `deck(301)` | 2884 | TALL 7.7 m | figures, a hand flag, 2 pennants | bay 30.5 m, every 16th slot (not in the Marina or at the Point) | not counted separately |
| SM_WatchTerrace | hotel sun terrace across a side street, two floors, 18 people | `terrace(401)` | 3840 | TALL 13.2 m | figures, 2 hand flags, 3 pennants | town 34 m, side-street slots s % 8 == 0 (not at the Point) | not counted separately |

Total placed by this block: 243 (FTDRESS placed 10010 with the block, 9767 for the dressing alone in the main
worker's base run). Stand_B + Deck + Terrace together: 106 (the log lists a mesh only when one of its pieces was
moved or left out). FTTRACK equals the baseline in every run (`frames=12717 route=dc9e032b solid=d2e759f4
solidTriangles=986296`). No error line naming People in the logs.

Half to two thirds of each crowd is `react=True` (livelier as the car comes by); every third figure never rests, so a
stand moves from far off. Nothing glows, nothing blinks. No words: stripes, a sun disc, blocks.

## Runs and what was looked at

Four game runs on the level's start (both lanes), one of them also from scene 8 and scene 15. Cropped and looked at:
Stand_A and Stand_B (start and scene 8, bay side), Tower_A (town side: start, first banked bend, scene 8), the
Terrace (town side, start, one sighting). Movement: consecutive film frames of Stand_B 0.25 s apart show the hand
flags at different angles and figures at different heights.

## Weak

- SM_WatchTerrace: its people stand under the floor above and the canopy and come out dark; from the right lane the
  promenade's clipped trees stand in front of its lower floor. It reads as a building with a crowd, not as strongly as
  the stands. Needs the roof opened or the figures brought forward of the slab.
- Triangles are over the 1,500 guide on five of six (a crowd of 24 at about 105 each plus the structure). The stands
  could lose a tier of people if the budget must hold.
- A boardwalk banner mast of the dressing stands in front of each bay-side stand (seen in the frames); the stand is
  still read.
- First look (before the fix): a stand centred on its slot stood behind the boardwalk's canopy and beside the
  boardwalk shops and did not read. Fixed by raising the first tier to 2.5 m and moving it 15 m along the slot.

## Not checked

- SM_WatchDeck in the game: placed, never found in a frame (not checked).
- SM_WatchTower_B in the game (same builder as Tower_A; not checked). Tower on the BAY side: not checked.
- Marina, Pleasure Beach, Hotel Row, the Point: no frames looked at for this block (scene 8 and scene 15 were driven;
  only a stand and a tower were identified in scene 8, nothing of mine identified in scene 15's frames, which were a loop).
- Whether any piece stands inside a dressing piece in those districts (the slots were chosen from the dressing
  table's rules and the main worker's notes, not from frames).
- The sheet renders of `ft_people_sheet` for the last four meshes (the `_all` images came out byte-identical; not used).
- Frame rate.

## ASKS

- The town-side Terrace (34 m) and Tower (31 m) stand in side-street mouths (s % 4 == 0), where the main worker's film
  crew sets stand in a few slots (anchored 21.9 m, tall pieces to about 30 m). When the blocks are joined, drop my
  rule's slot or the crew's where they meet.
- `waitfor FT CLOCK level=5.0` can miss under load (the log printed `level=5.1`) and then waits out its 150 s: use
  `waitfor FT CLOCK level=5.|150` in drive scripts.
- The FTCLEAR list shows a mesh only when a piece of it was moved or left out: a per-mesh placed count for all
  population meshes would help.
