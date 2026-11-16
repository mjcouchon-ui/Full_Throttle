// Ice Mountain (level index 5): the population layer. People, creatures and film crews, placed by rule like the dressing
// (the words and what their numbers mean are in the header of FTTrackDress.cpp; the rule kinds in FTDress.h).
// This table is read after FTDressL5.inl: start with a Zone line, and every rule here comes after every dressing
// rule, so the dressing's own placements never move. A mesh name without a leading slash is looked up in
// /Game/Maps/Glacier/Dress/; give a full path for the population's own meshes, e.g. TEXT("/Game/People/SM_CrowdCheerA").
// Rules that bind this layer: nothing over 2 m tall nearer than 24 m to a centre line and nothing at all nearer than
// 17 m; nothing lit that blinks nearer than 30 m; nothing that could be taken for a pickup, an enemy or an attack.
//
// THE CAST (15 Nov 2026, second round; the meshes are made by Content/Python/ft_people_ice.py, the crew's by ft_people_crew.py):
// racers on big-air hills of their own and on slalom courses down the real slopes, boarders in small pipes, crowds on
// scaffold stands, apres-ski decks, snowmen and the children building them, dog sleds, climbers on two faces of blue
// ice, roped parties and wolves on snow ridges, ibex on crags, eagles and paragliders overhead; and the film crew with
// its snowmobile camera.
//
// What this table found out about the level (read FTDressL5.inl's head first):
//  - The chase camera looks ALONG the road: a piece is seen end-on for seconds and side-on for an instant. So the stands,
//    the decks and the ice faces are put down by sets that turn them 35 degrees towards the car coming down the road
//    (R sets: yaw -35; L sets: +35), and a big-air hill's slope faces up the road (SM_SkiHill_A on the right,
//    SM_SkiHillL_A on the left: twins, like the dressing's L pieces).
//  - The cel pass inks a life-size figure 30 m off to a black scribble: people here are 1.3 times life size by the road
//    and 2 times and more on the slopes, in plain hot colours; the crew kit's pieces (life size as built) are placed at
//    1.2, the two units at 1.3.
//  - This hill's sight line pushes anything over about 1.4 m out to 24.7 m, so nothing of the cast is written nearer
//    than 25 m; the crew's low pieces are written at 25 to 26 m. A stand turned 35 degrees reaches 5.5 m towards the
//    road: written at 34 m (30 m on the glacier, the climbs at 31 m).
//  - SM_PisteRun_A is laid with its +X down the fall line (the set turns it -90) and wants the 12-in-100 slope under
//    it from 106 to 225 m: its anchor is 180 m out.
//  - Scatter and Site are left out of loops, corkscrews, pipes and jumps: there the cast is written by Far rules with
//    the land's own drop (stands, wolves) or in the sky (eagles, paragliders: never nearer than 60 and 70 m, an eagle
//    circles 16 m and a paraglider 15 m).
//  - The finale is the Abominable's road: no snowmen at all from 8.94 km on, and none nearer than 42 m before it.
//
// SECOND ROUND, the slots (after the review: two pieces of this table stood on one slot and side in many places, and
// some stood in the dressing's chalet rows and pipe stands). Slots are global (a rule fires on the slots s with
// (s + phase) % every == 0, whatever its zone), so every near rule of a district is written on a residue of its own:
// a resort district has a near piece every third slot a side (stand, deck, stand by turns, on a cycle of 9; the right
// on 0, 3, 6 and the left on 2, 5, 8), a glacier a climb every third slot and a stand every sixth on the residues its
// nearest ice wall leaves free, a ridge a stand, a dog sled and a crag of ibex by turns every fourth. Hills, pipes,
// snowmen and sleds further out keep to residues of their own in their bands of distance. A rule is cut out of every
// slot where the dressing has a near set of its own (the streets' chalet rows, the pipes' judges' towers, the
// galleries, the gondola's pylons, the depots, the ice fields) and out of the crew's zones: that is why a district's
// rule is sometimes written again in short zones named after the kilometre where they start. The table is written and
// checked by a scratch tool (every pair of its pieces on one side, same or next slot, box against box: none meet).

#define ICE(Name) TEXT("/Game/People/IceMountain/" Name)
#define CREW(Name) TEXT("/Game/People/Crew/SM_Crew" Name "_Snow")
Set(TEXT("PplStandAR"), { { ICE("SM_SnowStand_A"), 0, 0, 0, -35, 1 } });
Set(TEXT("PplStandAL"), { { ICE("SM_SnowStand_A"), 0, 0, 0, 35, 1 } });
Set(TEXT("PplStandBR"), { { ICE("SM_SnowStand_B"), 0, 0, 0, -35, 1 } });
Set(TEXT("PplStandBL"), { { ICE("SM_SnowStand_B"), 0, 0, 0, 35, 1 } });
Set(TEXT("PplDeckR"), { { ICE("SM_LodgeDeck_A"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplDeckL"), { { ICE("SM_LodgeDeck_A"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplClimbAR"), { { ICE("SM_IceClimb_A"), 0, 0, 0, -35, 1 } });
Set(TEXT("PplClimbAL"), { { ICE("SM_IceClimb_A"), 0, 0, 0, 35, 1 } });
Set(TEXT("PplClimbBR"), { { ICE("SM_IceClimb_B"), 0, 0, 0, -35, 1 } });
Set(TEXT("PplClimbBL"), { { ICE("SM_IceClimb_B"), 0, 0, 0, 35, 1 } });
Set(TEXT("PplPisteRun"), { { ICE("SM_PisteRun_A"), 0, 0, 0, -90, 1 } });
// The crew's unit: one cluster a side, placed at 1.3 (so these offsets come out a third longer), anchor 27 m out.
Set(TEXT("PplUnitR"), {
	{ CREW("Camera"),     0,   0, 0, -20, 1 },
	{ CREW("Crane"),    -15,   7, 0,   0, 1 },
	{ CREW("Lights"),    15,   9, 0, -20, 1 },
	{ CREW("Catering"),  -3,  18, 0,   0, 1 },
});
Set(TEXT("PplUnitL"), {
	{ CREW("Village"),    0,   0, 0,  20, 1 },
	{ CREW("Sound"),    -14,   7, 0,   0, 1 },
	{ CREW("Lights"),    15,   9, 0,  20, 1 },
	{ ICE("SM_CrewSkidoo"), 14, 0, 0, 20, 1 },
});
// A camera position at a set piece: the crane, a camera, the stunt team behind them.
Set(TEXT("PplShotR"), {
	{ CREW("Crane"),      0,   4, 0,   0, 1 },
	{ CREW("Camera"),    13,   0, 0, -20, 1 },
	{ CREW("Stunt"),    -14,   2, 0,   0, 1 },
});
Set(TEXT("PplShotL"), {
	{ CREW("Crane"),      0,   4, 0,   0, 1 },
	{ CREW("Camera"),    13,   0, 0,  20, 1 },
	{ CREW("Stunt"),    -14,   2, 0,   0, 1 },
});
Zone(TEXT("People: the unit by the start"), 0.f, 0.1f);
	Site(TEXT("PplUnitR"), 1, 27.f, 2, 1, 1.f, 1.3f);     // slot 1: the unit, a third over life size (life size, it inked to a dark tangle from the start line)
	Site(TEXT("PplUnitL"), -1, 27.f, 2, 0, 1.f, 1.3f);     // slot 2
	Site(TEXT("PplStandBL"), -1, 34.f, 2, 1, 1.f);     // and a stand a side, so the first thing on the mountain is a crowd in colour
	Site(TEXT("PplStandAR"), 1, 34.f, 2, 0, 1.f);

Zone(TEXT("People: the crew past the summit street"), 0.16f, 0.22f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 1, 0, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("Stunt"), 25.f, 26.f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.f);

Zone(TEXT("People: the crew at the first jump"), 1.98f, 2.1f);
	Site(TEXT("PplShotR"), 1, 28.f, 2, 0, 1.f, 1.2f);
	Site(TEXT("PplShotL"), -1, 28.f, 2, 1, 1.f, 1.2f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 2, 1, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 2, 0, 1.f, 1.2f, 0.f, -1, 0.f);

Zone(TEXT("People: the crew where the first jump lands"), 2.4f, 2.5f);
	Site(TEXT("PplShotR"), 1, 28.f, 2, 0, 1.f, 1.2f);
	Site(TEXT("PplShotL"), -1, 28.f, 2, 1, 1.f, 1.2f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 2, 1, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 2, 0, 1.f, 1.2f, 0.f, -1, 0.f);

Zone(TEXT("People: the crew at Black diamond's loop"), 3.96f, 4.08f);
	Site(TEXT("PplShotR"), 1, 28.f, 2, 0, 1.f, 1.2f);
	Site(TEXT("PplShotL"), -1, 28.f, 2, 1, 1.f, 1.2f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 2, 1, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 2, 0, 1.f, 1.2f, 0.f, -1, 0.f);

Zone(TEXT("People: the crew at the second jump"), 4.72f, 4.83f);
	Site(TEXT("PplShotR"), 1, 28.f, 2, 0, 1.f, 1.2f);
	Site(TEXT("PplShotL"), -1, 28.f, 2, 1, 1.f, 1.2f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 2, 1, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 2, 0, 1.f, 1.2f, 0.f, -1, 0.f);

Zone(TEXT("People: the crew at the whiteout's loop"), 6.74f, 6.86f);
	Site(TEXT("PplShotR"), 1, 28.f, 2, 0, 1.f, 1.2f);
	Site(TEXT("PplShotL"), -1, 28.f, 2, 1, 1.f, 1.2f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 2, 1, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 2, 0, 1.f, 1.2f, 0.f, -1, 0.f);

Zone(TEXT("People: the crew at the third jump"), 7.87f, 7.98f);
	Site(TEXT("PplShotR"), 1, 28.f, 2, 0, 1.f, 1.2f);
	Site(TEXT("PplShotL"), -1, 28.f, 2, 1, 1.f, 1.2f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 2, 1, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 2, 0, 1.f, 1.2f, 0.f, -1, 0.f);

Zone(TEXT("People: the unit before the Abominable"), 8.8f, 8.93f);
	Site(TEXT("PplUnitR"), 1, 27.f, 2, 0, 1.f, 1.3f);
	Site(TEXT("PplUnitL"), -1, 27.f, 2, 1, 1.f, 1.3f);

Zone(TEXT("People: under the chute's corkscrew"), 2.51f, 2.685f);
	Far(ICE("SM_SnowStand_A"), 34.f, 35.f, 3, 0, 1.f, 1.f, 0.f, 1, 0.74f, 0.f);
	Far(ICE("SM_SnowStand_B"), 34.f, 35.f, 3, 2, 1.f, 1.f, 0.f, -1, 0.74f, 0.f);
	Far(ICE("SM_Eagles_A"), 60.f, 110.f, 4, 0, 1.f, 1.f, 0.f, 1, 0.6f, 0.f);     // (review) 60 m and more: a bird circles 16 m and its wing is 2.6 m
	Far(ICE("SM_Eagles_A"), 60.f, 110.f, 4, 2, 1.f, 1.f, 0.f, -1, 0.6f, 0.f);
	Far(ICE("SM_Paragliders_A"), 75.f, 110.f, 4, 2, 1.f, 1.f, 0.f, 1, 0.f, 0.f);
	Far(ICE("SM_Paragliders_A"), 75.f, 110.f, 4, 0, 1.f, 1.f, 0.f, -1, 0.f, 0.f);
	Far(ICE("SM_WolfRidge_A"), 70.f, 78.f, 3, 1, 1.f, 1.f, 0.f, 1, 0.5f, 0.f);
	Far(ICE("SM_WolfRidge_A"), 70.f, 78.f, 3, 0, 1.f, 1.f, 0.f, -1, 0.5f, 0.f);

Zone(TEXT("People: under Black diamond's loop"), 4.085f, 4.37f);
	Far(ICE("SM_SnowStand_A"), 34.f, 35.f, 3, 2, 1.f, 1.f, 0.f, 1, 0.74f, 0.f);
	Far(ICE("SM_SnowStand_B"), 34.f, 35.f, 3, 1, 1.f, 1.f, 0.f, -1, 0.74f, 0.f);
	Far(ICE("SM_Eagles_A"), 60.f, 110.f, 4, 0, 1.f, 1.f, 0.f, 1, 0.6f, 0.f);     // (review) 60 m and more: a bird circles 16 m and its wing is 2.6 m
	Far(ICE("SM_Eagles_A"), 60.f, 110.f, 4, 2, 1.f, 1.f, 0.f, -1, 0.6f, 0.f);
	Far(ICE("SM_Paragliders_A"), 75.f, 110.f, 4, 2, 1.f, 1.f, 0.f, 1, 0.f, 0.f);
	Far(ICE("SM_Paragliders_A"), 75.f, 110.f, 4, 0, 1.f, 1.f, 0.f, -1, 0.f, 0.f);
	Far(ICE("SM_WolfRidge_A"), 70.f, 78.f, 3, 0, 1.f, 1.f, 0.f, 1, 0.5f, 0.f);
	Far(ICE("SM_WolfRidge_A"), 70.f, 78.f, 3, 2, 1.f, 1.f, 0.f, -1, 0.5f, 0.f);

Zone(TEXT("People: under Black diamond's corkscrew"), 5.225f, 5.4f);
	Far(ICE("SM_SnowStand_A"), 34.f, 35.f, 3, 1, 1.f, 1.f, 0.f, 1, 0.74f, 0.f);
	Far(ICE("SM_SnowStand_B"), 34.f, 35.f, 3, 0, 1.f, 1.f, 0.f, -1, 0.74f, 0.f);
	Far(ICE("SM_Eagles_A"), 60.f, 110.f, 4, 0, 1.f, 1.f, 0.f, 1, 0.6f, 0.f);     // (review) 60 m and more: a bird circles 16 m and its wing is 2.6 m
	Far(ICE("SM_Eagles_A"), 60.f, 110.f, 4, 2, 1.f, 1.f, 0.f, -1, 0.6f, 0.f);
	Far(ICE("SM_Paragliders_A"), 75.f, 110.f, 4, 2, 1.f, 1.f, 0.f, 1, 0.f, 0.f);
	Far(ICE("SM_Paragliders_A"), 75.f, 110.f, 4, 0, 1.f, 1.f, 0.f, -1, 0.f, 0.f);
	Far(ICE("SM_WolfRidge_A"), 70.f, 78.f, 3, 2, 1.f, 1.f, 0.f, 1, 0.5f, 0.f);
	Far(ICE("SM_WolfRidge_A"), 70.f, 78.f, 3, 1, 1.f, 1.f, 0.f, -1, 0.5f, 0.f);

Zone(TEXT("People: under the whiteout's loop"), 6.87f, 7.155f);
	Far(ICE("SM_SnowStand_A"), 34.f, 35.f, 3, 0, 1.f, 1.f, 0.f, 1, 0.74f, 0.f);
	Far(ICE("SM_SnowStand_B"), 34.f, 35.f, 3, 2, 1.f, 1.f, 0.f, -1, 0.74f, 0.f);
	Far(ICE("SM_Eagles_A"), 60.f, 110.f, 4, 0, 1.f, 1.f, 0.f, 1, 0.6f, 0.f);     // (review) 60 m and more: a bird circles 16 m and its wing is 2.6 m
	Far(ICE("SM_Eagles_A"), 60.f, 110.f, 4, 2, 1.f, 1.f, 0.f, -1, 0.6f, 0.f);
	Far(ICE("SM_Paragliders_A"), 75.f, 110.f, 4, 2, 1.f, 1.f, 0.f, 1, 0.f, 0.f);
	Far(ICE("SM_Paragliders_A"), 75.f, 110.f, 4, 0, 1.f, 1.f, 0.f, -1, 0.f, 0.f);
	Far(ICE("SM_WolfRidge_A"), 70.f, 78.f, 3, 1, 1.f, 1.f, 0.f, 1, 0.5f, 0.f);
	Far(ICE("SM_WolfRidge_A"), 70.f, 78.f, 3, 0, 1.f, 1.f, 0.f, -1, 0.5f, 0.f);

Zone(TEXT("People: under the valley's corkscrew"), 7.215f, 7.385f);
	Far(ICE("SM_SnowStand_A"), 34.f, 35.f, 3, 2, 1.f, 1.f, 0.f, 1, 0.74f, 0.f);
	Far(ICE("SM_SnowStand_B"), 34.f, 35.f, 3, 1, 1.f, 1.f, 0.f, -1, 0.74f, 0.f);
	Far(ICE("SM_Eagles_A"), 60.f, 110.f, 4, 0, 1.f, 1.f, 0.f, 1, 0.6f, 0.f);     // (review) 60 m and more: a bird circles 16 m and its wing is 2.6 m
	Far(ICE("SM_Eagles_A"), 60.f, 110.f, 4, 2, 1.f, 1.f, 0.f, -1, 0.6f, 0.f);
	Far(ICE("SM_Paragliders_A"), 75.f, 110.f, 4, 2, 1.f, 1.f, 0.f, 1, 0.f, 0.f);
	Far(ICE("SM_Paragliders_A"), 75.f, 110.f, 4, 0, 1.f, 1.f, 0.f, -1, 0.f, 0.f);
	Far(ICE("SM_WolfRidge_A"), 70.f, 78.f, 3, 0, 1.f, 1.f, 0.f, 1, 0.5f, 0.f);
	Far(ICE("SM_WolfRidge_A"), 70.f, 78.f, 3, 2, 1.f, 1.f, 0.f, -1, 0.5f, 0.f);

Zone(TEXT("People: over the valley's bends"), 7.525f, 7.975f);
	Far(ICE("SM_Paragliders_A"), 70.f, 85.f, 2, 0, 1.f, 1.f, 0.f, -1, 0.f, 0.f);     // (review) the left of these bends showed nobody for 500 m
	Far(ICE("SM_Paragliders_A"), 70.f, 85.f, 2, 1, 1.f, 1.f, 0.f, 1, 0.f, 0.f);

Zone(TEXT("People: First tracks, the top of the resort"), 0.24f, 1.19f);
	Site(TEXT("PplStandAR"), 1, 34.f, 9, 0, 1.f);     // crowds on stands and apres-ski decks, turned to the car coming: every third slot a side
	Site(TEXT("PplStandBL"), -1, 34.f, 9, 7, 1.f);
	Site(TEXT("PplDeckR"), 1, 36.f, 9, 6, 1.f);
	Site(TEXT("PplStandAL"), -1, 34.f, 9, 4, 1.f);
	Site(TEXT("PplStandBR"), 1, 34.f, 9, 3, 1.f);
	Site(TEXT("PplDeckL"), -1, 36.f, 9, 1, 1.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 54, 53, 1.f, 1.2f, 0.f, 1, 0.f);     // the crew, thinly, at the tape
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 54, 51, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 54, 35, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 54, 33, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 54, 17, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 54, 15, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_SkiHill_A"), 48.f, 50.f, 9, 5, 1.f, 1.f, 0.f, 1, 0.f);     // big-air hills: racers come down towards the car
	Scatter(ICE("SM_SkiHillL_A"), 48.f, 50.f, 9, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_BoardPipe_A"), 50.f, 52.f, 9, 0, 1.f, 1.f, 0.f, -1, 0.f);     // boarders swinging in a pipe
	Scatter(ICE("SM_BoardPipe_A"), 78.f, 82.f, 9, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Snowmen_A"), 60.f, 63.f, 9, 8, 1.f, 1.f, 0.f, 1, 20.f);     // snowmen (never nearer than 42 m, and none on the Abominable's road)
	Scatter(ICE("SM_Snowmen_A"), 60.f, 63.f, 9, 6, 1.f, 1.f, 0.f, -1, 20.f);
	Scatter(ICE("SM_DogSled_A"), 62.f, 68.f, 18, 12, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_DogSled_A"), 84.f, 90.f, 18, 8, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 18, 17, 1.f);     // slalom racers down the real slope
	Site(TEXT("PplPisteRun"), 1, 180.f, 18, 7, 1.f);
	Scatter(ICE("SM_Paragliders_A"), 95.f, 130.f, 12, 7, 1.f, 1.f, 0.f, 1, 0.f);     // paragliders over the pistes
	Scatter(ICE("SM_Paragliders_A"), 95.f, 130.f, 12, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 24, 15, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 24, 3, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: The half pipe: competition day"), 1.19f, 2.43f);
	Site(TEXT("PplDeckR"), 1, 36.f, 12, 0, 1.f);     // crowds on stands and apres-ski decks, turned to the car coming: every third slot a side
	Site(TEXT("PplStandAL"), -1, 34.f, 12, 10, 1.f);
	Site(TEXT("PplStandBR"), 1, 34.f, 12, 8, 1.f);
	Site(TEXT("PplDeckL"), -1, 36.f, 12, 6, 1.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 72, 71, 1.f, 1.2f, 0.f, 1, 0.f);     // the crew, thinly, at the tape
	Scatter(CREW("Dolly"), 25.f, 26.f, 72, 69, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 72, 47, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 72, 45, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 72, 23, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 72, 21, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_SkiHill_A"), 48.f, 50.f, 12, 7, 1.f, 1.f, 0.f, 1, 0.f);     // big-air hills: racers come down towards the car
	Scatter(ICE("SM_SkiHillL_A"), 48.f, 50.f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_BoardPipe_A"), 78.f, 82.f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Snowmen_A"), 60.f, 63.f, 12, 11, 1.f, 1.f, 0.f, -1, 20.f);
	Scatter(ICE("SM_DogSled_A"), 62.f, 68.f, 24, 17, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 24, 13, 1.f);     // slalom racers down the real slope
	Site(TEXT("PplPisteRun"), 1, 180.f, 24, 12, 1.f);
	Scatter(ICE("SM_Paragliders_A"), 95.f, 130.f, 12, 7, 1.f, 1.f, 0.f, 1, 0.f);     // paragliders over the pistes
	Scatter(ICE("SM_Paragliders_A"), 95.f, 130.f, 12, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 24, 14, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 24, 2, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: The half pipe: competition day, 1.45 km"), 1.43f, 1.466f);
	Site(TEXT("PplStandAR"), 1, 34.f, 12, 4, 1.f);

Zone(TEXT("People: The half pipe: competition day, 1.54 km"), 1.52f, 1.556f);
	Site(TEXT("PplStandBL"), -1, 34.f, 12, 2, 1.f);
	Scatter(ICE("SM_BoardPipe_A"), 50.f, 52.f, 12, 2, 1.f, 1.f, 0.f, -1, 0.f);     // boarders swinging in a pipe

Zone(TEXT("People: The half pipe: competition day, 1.58 km"), 1.565f, 1.602f);
	Scatter(ICE("SM_Snowmen_A"), 60.f, 63.f, 12, 1, 1.f, 1.f, 0.f, 1, 20.f);     // snowmen (never nearer than 42 m, and none on the Abominable's road)

Zone(TEXT("People: Avalanche chute"), 2.43f, 2.96f);
	Scatter(ICE("SM_Eagles_A"), 60.f, 110.f, 4, 0, 1.f, 1.f, 0.f, 1, 0.f);     // the chute: galleries both sides, so the cast is above and behind them
	Scatter(ICE("SM_Eagles_A"), 60.f, 110.f, 4, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_RopeTeam_A"), 150.f, 230.f, 4, 2, 1.f, 1.5f, 0.f, 1, 0.f);
	Scatter(ICE("SM_RopeTeam_A"), 150.f, 230.f, 4, 0, 1.f, 1.5f, 0.f, -1, 0.f);

Zone(TEXT("People: Avalanche chute, 2.76 km"), 2.742f, 2.959f);
	Scatter(ICE("SM_Ibex_A"), 62.f, 80.f, 4, 3, 1.f, 1.4f, 0.f, 1, 25.f);

Zone(TEXT("People: Avalanche chute, 2.85 km"), 2.832f, 2.868f);
	Scatter(ICE("SM_Ibex_A"), 62.f, 80.f, 4, 1, 1.f, 1.4f, 0.f, -1, 25.f);

Zone(TEXT("People: The super pipe: the glacier"), 2.96f, 4.02f);
	Site(TEXT("PplClimbAR"), 1, 31.f, 6, 4, 1.f);     // climbers on blue ice, two faces by turns (31 m: nothing of them is moved by the track)
	Site(TEXT("PplClimbBR"), 1, 31.f, 6, 1, 1.f);
	Site(TEXT("PplClimbAL"), -1, 31.f, 6, 5, 1.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 36, 33, 1.f, 1.2f, 0.f, 1, 0.f);     // the crew, thinly, at the tape
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 36, 33, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 36, 21, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 36, 21, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 36, 9, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 36, 9, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_RopeTeam_A"), 150.f, 230.f, 12, 7, 1.f, 1.5f, 0.f, -1, 0.f);     // a roped party and wolves, where the walls leave a gap
	Scatter(ICE("SM_WolfRidge_A"), 130.f, 210.f, 12, 2, 1.f, 1.4f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 60.f, 130.f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);     // eagles over the ice
	Scatter(ICE("SM_Eagles_A"), 60.f, 130.f, 12, 4, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: The super pipe: the glacier, 3.26 km"), 3.239f, 3.276f);
	Site(TEXT("PplStandBR"), 1, 30.f, 6, 0, 1.f);

Zone(TEXT("People: The super pipe: the glacier, 3.80 km"), 3.782f, 3.819f);
	Site(TEXT("PplStandBR"), 1, 30.f, 6, 0, 1.f);
	Site(TEXT("PplStandAL"), -1, 30.f, 6, 0, 1.f);

Zone(TEXT("People: The super pipe: the glacier, 3.17 km"), 3.149f, 3.728f);
	Site(TEXT("PplClimbBL"), -1, 31.f, 6, 2, 1.f);

Zone(TEXT("People: The super pipe: the glacier, 2.99 km"), 2.968f, 3.276f);
	Site(TEXT("PplStandAL"), -1, 30.f, 6, 0, 1.f);

Zone(TEXT("People: Black diamond: the exposed ridge"), 4.02f, 5.65f);
	Site(TEXT("PplStandBR"), 1, 34.f, 24, 12, 1.f);
	Scatter(ICE("SM_DogSled_A"), 30.f, 32.f, 12, 8, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplStandBL"), -1, 34.f, 24, 22, 1.f);
	Site(TEXT("PplStandAL"), -1, 34.f, 24, 10, 1.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 36, 0, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_RopeTeam_A"), 100.f, 140.f, 12, 3, 1.f, 1.f, 0.f, 1, 0.f);     // roped parties on the snow ridges
	Scatter(ICE("SM_RopeTeam_A"), 150.f, 240.f, 12, 9, 1.f, 1.5f, 0.f, -1, 0.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 12, 5, 1.f);
	Scatter(ICE("SM_Eagles_A"), 60.f, 120.f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 60.f, 120.f, 12, 4, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: Black diamond: the exposed ridge, 4.71 km"), 4.687f, 4.723f);
	Scatter(ICE("SM_Ibex_A"), 48.f, 54.f, 12, 4, 1.f, 1.4f, 0.f, 1, 25.f);     // (review) the ibex 1.4 times their size, on a crag of their own slot

Zone(TEXT("People: Black diamond: the exposed ridge, 4.61 km"), 4.597f, 5.176f);
	Scatter(ICE("SM_DogSled_A"), 30.f, 32.f, 12, 6, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: Black diamond: the exposed ridge, 4.80 km"), 4.778f, 4.814f);
	Scatter(ICE("SM_Ibex_A"), 48.f, 54.f, 12, 2, 1.f, 1.4f, 0.f, -1, 25.f);

Zone(TEXT("People: Black diamond: the exposed ridge, 4.93 km"), 4.913f, 5.493f);
	Scatter(ICE("SM_WolfRidge_A"), 66.f, 84.f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);     // (review) wolves 66 to 84 m out (they were 84 to 160: not to be made out)

Zone(TEXT("People: Black diamond: the exposed ridge, 4.66 km"), 4.642f, 4.678f);
	Scatter(ICE("SM_WolfRidge_A"), 66.f, 84.f, 12, 5, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The whiteout: every gate on the mountain, 5.70 km"), 5.683f, 6.533f);
	Site(TEXT("PplStandBR"), 1, 34.f, 9, 0, 1.f);     // crowds on stands and apres-ski decks, turned to the car coming: every third slot a side

Zone(TEXT("People: The whiteout: every gate on the mountain, 5.75 km"), 5.728f, 6.578f);
	Site(TEXT("PplDeckL"), -1, 36.f, 9, 8, 1.f);

Zone(TEXT("People: The whiteout: every gate on the mountain, 5.84 km"), 5.818f, 6.669f);
	Site(TEXT("PplStandAR"), 1, 34.f, 9, 6, 1.f);

Zone(TEXT("People: The whiteout: every gate on the mountain, 5.88 km"), 5.863f, 6.714f);
	Site(TEXT("PplStandBL"), -1, 34.f, 9, 5, 1.f);
	Scatter(ICE("SM_SkiHill_A"), 48.f, 50.f, 9, 5, 1.f, 1.f, 0.f, 1, 0.f);     // big-air hills: racers come down towards the car

Zone(TEXT("People: The whiteout: every gate on the mountain, 5.97 km"), 5.954f, 6.397f);
	Site(TEXT("PplDeckR"), 1, 36.f, 9, 3, 1.f);

Zone(TEXT("People: The whiteout: every gate on the mountain, 6.02 km"), 5.999f, 6.443f);
	Site(TEXT("PplStandAL"), -1, 34.f, 9, 2, 1.f);
	Scatter(ICE("SM_BoardPipe_A"), 78.f, 82.f, 9, 2, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The whiteout: every gate on the mountain"), 5.65f, 7.3f);
	Scatter(CREW("Camera"), 25.f, 26.f, 54, 53, 1.f, 1.2f, 0.f, 1, 0.f);     // the crew, thinly, at the tape
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 54, 48, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 54, 35, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 54, 30, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 54, 17, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(ICE("SM_DogSled_A"), 62.f, 68.f, 18, 13, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 18, 0, 1.f);     // slalom racers down the real slope
	Site(TEXT("PplPisteRun"), 1, 180.f, 18, 7, 1.f);
	Scatter(ICE("SM_Paragliders_A"), 95.f, 130.f, 12, 7, 1.f, 1.f, 0.f, 1, 0.f);     // paragliders over the pistes
	Scatter(ICE("SM_Paragliders_A"), 95.f, 130.f, 12, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 24, 13, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 24, 1, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: The whiteout: every gate on the mountain, 5.93 km"), 5.909f, 6.759f);
	Scatter(ICE("SM_SkiHillL_A"), 48.f, 50.f, 9, 4, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: The whiteout: every gate on the mountain, 5.66 km"), 5.65f, 6.488f);
	Scatter(ICE("SM_BoardPipe_A"), 50.f, 52.f, 9, 1, 1.f, 1.f, 0.f, -1, 0.f);     // boarders swinging in a pipe

Zone(TEXT("People: The whiteout: every gate on the mountain, 6.15 km"), 6.135f, 6.171f);
	Scatter(ICE("SM_DogSled_A"), 84.f, 90.f, 18, 8, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The valley run, 7.74 km"), 7.718f, 8.569f);
	Site(TEXT("PplStandAR"), 1, 34.f, 9, 0, 1.f);     // crowds on stands and apres-ski decks, turned to the car coming: every third slot a side
	Scatter(ICE("SM_BoardPipe_A"), 50.f, 52.f, 9, 0, 1.f, 1.f, 0.f, -1, 0.f);     // boarders swinging in a pipe

Zone(TEXT("People: The valley run, 7.83 km"), 7.809f, 7.845f);
	Site(TEXT("PplStandBL"), -1, 34.f, 9, 7, 1.f);

Zone(TEXT("People: The valley run, 8.64 km"), 8.623f, 8.66f);
	Site(TEXT("PplStandBL"), -1, 34.f, 9, 7, 1.f);

Zone(TEXT("People: The valley run, 8.69 km"), 8.669f, 8.705f);
	Site(TEXT("PplDeckR"), 1, 36.f, 9, 6, 1.f);

Zone(TEXT("People: The valley run, 7.56 km"), 7.537f, 7.574f);
	Site(TEXT("PplStandAL"), -1, 34.f, 9, 4, 1.f);

Zone(TEXT("People: The valley run, 8.37 km"), 8.352f, 8.795f);
	Site(TEXT("PplStandAL"), -1, 34.f, 9, 4, 1.f);

Zone(TEXT("People: The valley run, 7.60 km"), 7.583f, 7.619f);
	Site(TEXT("PplStandBR"), 1, 34.f, 9, 3, 1.f);
	Scatter(ICE("SM_SkiHillL_A"), 48.f, 50.f, 9, 3, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: The valley run, 8.42 km"), 8.397f, 8.433f);
	Site(TEXT("PplStandBR"), 1, 34.f, 9, 3, 1.f);

Zone(TEXT("People: The valley run, 7.69 km"), 7.673f, 8.524f);
	Site(TEXT("PplDeckL"), -1, 36.f, 9, 1, 1.f);

Zone(TEXT("People: The valley run"), 7.3f, 8.94f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 54, 35, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 54, 17, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 54, 15, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_SkiHill_A"), 48.f, 50.f, 9, 5, 1.f, 1.f, 0.f, 1, 0.f);     // big-air hills: racers come down towards the car
	Scatter(ICE("SM_Snowmen_A"), 60.f, 63.f, 9, 6, 1.f, 1.f, 0.f, -1, 20.f);
	Scatter(ICE("SM_DogSled_A"), 62.f, 68.f, 18, 12, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_DogSled_A"), 84.f, 90.f, 18, 8, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 18, 17, 1.f);     // slalom racers down the real slope
	Site(TEXT("PplPisteRun"), 1, 180.f, 18, 7, 1.f);
	Scatter(ICE("SM_Paragliders_A"), 95.f, 130.f, 12, 7, 1.f, 1.f, 0.f, 1, 0.f);     // paragliders over the pistes
	Scatter(ICE("SM_Paragliders_A"), 95.f, 130.f, 12, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 24, 15, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 24, 3, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: The valley run, 8.42 km"), 8.397f, 8.84f);
	Scatter(ICE("SM_SkiHillL_A"), 48.f, 50.f, 9, 3, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: The valley run, 8.87 km"), 8.85f, 8.886f);
	Scatter(ICE("SM_BoardPipe_A"), 78.f, 82.f, 9, 2, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The valley run, 7.78 km"), 7.764f, 7.8f);
	Scatter(ICE("SM_Snowmen_A"), 60.f, 63.f, 9, 8, 1.f, 1.f, 0.f, 1, 20.f);     // snowmen (never nearer than 42 m, and none on the Abominable's road)

Zone(TEXT("People: The valley run, 8.60 km"), 8.578f, 8.614f);
	Scatter(ICE("SM_Snowmen_A"), 60.f, 63.f, 9, 8, 1.f, 1.f, 0.f, 1, 20.f);     // snowmen (never nearer than 42 m, and none on the Abominable's road)

Zone(TEXT("People: The Abominable's glacier"), 8.94f, 11.6f);
	Site(TEXT("PplClimbAR"), 1, 31.f, 6, 0, 1.f);     // climbers on blue ice, two faces by turns (31 m: nothing of them is moved by the track)
	Site(TEXT("PplClimbBR"), 1, 31.f, 6, 3, 1.f);
	Site(TEXT("PplStandBR"), 1, 30.f, 6, 5, 1.f);
	Site(TEXT("PplClimbAL"), -1, 31.f, 6, 4, 1.f);
	Site(TEXT("PplClimbBL"), -1, 31.f, 6, 1, 1.f);
	Site(TEXT("PplStandAL"), -1, 30.f, 6, 5, 1.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 36, 32, 1.f, 1.2f, 0.f, 1, 0.f);     // the crew, thinly, at the tape
	Scatter(CREW("Dolly"), 25.f, 26.f, 36, 32, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 36, 20, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 36, 20, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 36, 8, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 36, 8, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_RopeTeam_A"), 150.f, 230.f, 12, 6, 1.f, 1.5f, 0.f, -1, 0.f);     // a roped party and wolves, where the walls leave a gap
	Scatter(ICE("SM_WolfRidge_A"), 130.f, 210.f, 12, 1, 1.f, 1.4f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 60.f, 130.f, 12, 9, 1.f, 1.f, 0.f, 1, 0.f);     // eagles over the ice
	Scatter(ICE("SM_Eagles_A"), 60.f, 130.f, 12, 3, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: The white ridge"), 11.6f, 14.3f);
	Site(TEXT("PplStandAR"), 1, 34.f, 24, 21, 1.f);     // a stand, a dog sled on the fence line, ibex on a crag: one of them every fourth slot a side
	Site(TEXT("PplStandBR"), 1, 34.f, 24, 9, 1.f);
	Scatter(ICE("SM_DogSled_A"), 30.f, 32.f, 12, 5, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Ibex_A"), 48.f, 54.f, 12, 1, 1.f, 1.4f, 0.f, 1, 25.f);     // (review) the ibex 1.4 times their size, on a crag of their own slot
	Site(TEXT("PplStandBL"), -1, 34.f, 24, 19, 1.f);
	Site(TEXT("PplStandAL"), -1, 34.f, 24, 7, 1.f);
	Scatter(ICE("SM_DogSled_A"), 30.f, 32.f, 12, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_Ibex_A"), 48.f, 54.f, 12, 11, 1.f, 1.4f, 0.f, -1, 25.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 36, 23, 1.f, 1.2f, 0.f, 1, 0.f);     // the crew, thinly, at the tape
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 36, 33, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 36, 11, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 36, 21, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 36, 35, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 36, 9, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_WolfRidge_A"), 66.f, 84.f, 12, 8, 1.f, 1.f, 0.f, -1, 0.f);     // (review) wolves 66 to 84 m out (they were 84 to 160: not to be made out)
	Scatter(ICE("SM_WolfRidge_A"), 66.f, 84.f, 12, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_RopeTeam_A"), 100.f, 140.f, 12, 0, 1.f, 1.f, 0.f, 1, 0.f);     // roped parties on the snow ridges
	Scatter(ICE("SM_RopeTeam_A"), 150.f, 240.f, 12, 6, 1.f, 1.5f, 0.f, -1, 0.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 12, 2, 1.f);
	Scatter(ICE("SM_Eagles_A"), 60.f, 120.f, 12, 7, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 60.f, 120.f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: The resort, 14.66 km"), 14.641f, 16.306f);
	Site(TEXT("PplDeckR"), 1, 36.f, 9, 0, 1.f);     // crowds on stands and apres-ski decks, turned to the car coming: every third slot a side

Zone(TEXT("People: The resort, 14.34 km"), 14.324f, 15.175f);
	Site(TEXT("PplStandAL"), -1, 34.f, 9, 7, 1.f);

Zone(TEXT("People: The resort, 15.97 km"), 15.953f, 16.803f);
	Site(TEXT("PplStandAL"), -1, 34.f, 9, 7, 1.f);

Zone(TEXT("People: The resort, 14.39 km"), 14.369f, 15.22f);
	Site(TEXT("PplStandBR"), 1, 34.f, 9, 6, 1.f);

Zone(TEXT("People: The resort, 16.02 km"), 15.998f, 16.848f);
	Site(TEXT("PplStandBR"), 1, 34.f, 9, 6, 1.f);

Zone(TEXT("People: The resort, 14.88 km"), 14.867f, 16.939f);
	Site(TEXT("PplDeckL"), -1, 36.f, 9, 4, 1.f);

Zone(TEXT("People: The resort, 14.93 km"), 14.912f, 16.984f);
	Site(TEXT("PplStandAR"), 1, 34.f, 9, 3, 1.f);

Zone(TEXT("People: The resort, 14.61 km"), 14.595f, 16.26f);
	Site(TEXT("PplStandBL"), -1, 34.f, 9, 1, 1.f);

Zone(TEXT("People: The resort"), 14.3f, 17.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 54, 53, 1.f, 1.2f, 0.f, 1, 0.f);     // the crew, thinly, at the tape
	Scatter(CREW("Dolly"), 25.f, 26.f, 54, 51, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 54, 33, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 54, 17, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 54, 15, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_SkiHillL_A"), 48.f, 50.f, 9, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_DogSled_A"), 62.f, 68.f, 18, 12, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 18, 17, 1.f);     // slalom racers down the real slope
	Site(TEXT("PplPisteRun"), 1, 180.f, 18, 7, 1.f);
	Scatter(ICE("SM_Paragliders_A"), 95.f, 130.f, 12, 7, 1.f, 1.f, 0.f, 1, 0.f);     // paragliders over the pistes
	Scatter(ICE("SM_Paragliders_A"), 95.f, 130.f, 12, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 24, 14, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 24, 2, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: The resort, 14.84 km"), 14.822f, 16.894f);
	Scatter(ICE("SM_SkiHill_A"), 40.f, 41.f, 9, 5, 1.f, 1.f, 0.f, 1, 0.f);     // big-air hills: racers come down towards the car (the service road lies at 50 m here)

Zone(TEXT("People: The resort, 14.66 km"), 14.641f, 14.677f);
	Scatter(ICE("SM_BoardPipe_A"), 50.f, 52.f, 9, 0, 1.f, 1.f, 0.f, -1, 0.f);     // boarders swinging in a pipe

Zone(TEXT("People: The resort, 15.47 km"), 15.455f, 15.491f);
	Scatter(ICE("SM_BoardPipe_A"), 50.f, 52.f, 9, 0, 1.f, 1.f, 0.f, -1, 0.f);     // boarders swinging in a pipe

Zone(TEXT("People: The resort, 16.29 km"), 16.269f, 16.306f);
	Scatter(ICE("SM_BoardPipe_A"), 50.f, 52.f, 9, 0, 1.f, 1.f, 0.f, -1, 0.f);     // boarders swinging in a pipe

Zone(TEXT("People: The resort, 14.57 km"), 14.55f, 15.401f);
	Scatter(ICE("SM_BoardPipe_A"), 78.f, 82.f, 9, 2, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The resort, 16.20 km"), 16.179f, 16.622f);
	Scatter(ICE("SM_BoardPipe_A"), 78.f, 82.f, 9, 2, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The resort, 15.93 km"), 15.907f, 15.944f);
	Scatter(ICE("SM_DogSled_A"), 84.f, 90.f, 18, 8, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The ice again"), 17.f, 19.7f);
	Site(TEXT("PplClimbAR"), 1, 31.f, 6, 4, 1.f);     // climbers on blue ice, two faces by turns (31 m: nothing of them is moved by the track)
	Site(TEXT("PplClimbBR"), 1, 31.f, 6, 1, 1.f);
	Site(TEXT("PplStandBR"), 1, 30.f, 6, 0, 1.f);
	Site(TEXT("PplClimbAL"), -1, 31.f, 6, 5, 1.f);
	Site(TEXT("PplClimbBL"), -1, 31.f, 6, 2, 1.f);
	Site(TEXT("PplStandAL"), -1, 30.f, 6, 0, 1.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 36, 33, 1.f, 1.2f, 0.f, 1, 0.f);     // the crew, thinly, at the tape
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 36, 33, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 36, 21, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 36, 21, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 36, 9, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 36, 9, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_RopeTeam_A"), 150.f, 230.f, 12, 5, 1.f, 1.5f, 0.f, -1, 0.f);     // a roped party and wolves, where the walls leave a gap
	Scatter(ICE("SM_WolfRidge_A"), 130.f, 210.f, 12, 0, 1.f, 1.4f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 60.f, 130.f, 12, 8, 1.f, 1.f, 0.f, 1, 0.f);     // eagles over the ice
	Scatter(ICE("SM_Eagles_A"), 60.f, 130.f, 12, 2, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: The high ridge"), 19.7f, 22.4f);
	Site(TEXT("PplStandAR"), 1, 34.f, 24, 19, 1.f);     // a stand, a dog sled on the fence line, ibex on a crag: one of them every fourth slot a side
	Site(TEXT("PplStandBR"), 1, 34.f, 24, 7, 1.f);
	Scatter(ICE("SM_DogSled_A"), 30.f, 32.f, 12, 3, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Ibex_A"), 48.f, 54.f, 12, 11, 1.f, 1.4f, 0.f, 1, 25.f);     // (review) the ibex 1.4 times their size, on a crag of their own slot
	Site(TEXT("PplStandBL"), -1, 34.f, 24, 17, 1.f);
	Site(TEXT("PplStandAL"), -1, 34.f, 24, 5, 1.f);
	Scatter(ICE("SM_DogSled_A"), 30.f, 32.f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_Ibex_A"), 48.f, 54.f, 12, 9, 1.f, 1.4f, 0.f, -1, 25.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 36, 21, 1.f, 1.2f, 0.f, 1, 0.f);     // the crew, thinly, at the tape
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 36, 31, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 36, 9, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 36, 19, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 36, 33, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 36, 7, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_WolfRidge_A"), 66.f, 84.f, 12, 6, 1.f, 1.f, 0.f, -1, 0.f);     // (review) wolves 66 to 84 m out (they were 84 to 160: not to be made out)
	Scatter(ICE("SM_WolfRidge_A"), 66.f, 84.f, 12, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_RopeTeam_A"), 100.f, 140.f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);     // roped parties on the snow ridges
	Scatter(ICE("SM_RopeTeam_A"), 150.f, 240.f, 12, 4, 1.f, 1.5f, 0.f, -1, 0.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 12, 0, 1.f);
	Scatter(ICE("SM_Eagles_A"), 60.f, 120.f, 12, 5, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 60.f, 120.f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: The last lifts"), 22.4f, 99.f);
	Site(TEXT("PplStandBR"), 1, 34.f, 9, 0, 1.f);     // crowds on stands and apres-ski decks, turned to the car coming: every third slot a side
	Site(TEXT("PplDeckL"), -1, 36.f, 9, 7, 1.f);
	Site(TEXT("PplStandAR"), 1, 34.f, 9, 6, 1.f);
	Site(TEXT("PplStandAL"), -1, 34.f, 9, 1, 1.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 54, 53, 1.f, 1.2f, 0.f, 1, 0.f);     // the crew, thinly, at the tape
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 54, 51, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 54, 35, 1.f, 1.2f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 54, 33, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 54, 15, 1.f, 1.2f, 0.f, -1, 0.f);
	Scatter(ICE("SM_SkiHill_A"), 40.f, 41.f, 9, 5, 1.f, 1.f, 0.f, 1, 0.f);     // big-air hills: racers come down towards the car (the service road lies at 50 m here)
	Scatter(ICE("SM_SkiHillL_A"), 48.f, 50.f, 9, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_DogSled_A"), 62.f, 68.f, 18, 12, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 18, 17, 1.f);     // slalom racers down the real slope
	Site(TEXT("PplPisteRun"), 1, 180.f, 18, 7, 1.f);
	Scatter(ICE("SM_Paragliders_A"), 95.f, 130.f, 12, 7, 1.f, 1.f, 0.f, 1, 0.f);     // paragliders over the pistes
	Scatter(ICE("SM_Paragliders_A"), 95.f, 130.f, 12, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 24, 13, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 24, 1, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: The last lifts, 22.62 km"), 22.603f, 23.047f);
	Site(TEXT("PplStandBL"), -1, 34.f, 9, 4, 1.f);

Zone(TEXT("People: The last lifts, 23.84 km"), 23.825f, 24.676f);
	Site(TEXT("PplStandBL"), -1, 34.f, 9, 4, 1.f);

Zone(TEXT("People: The last lifts, 22.67 km"), 22.649f, 23.092f);
	Site(TEXT("PplDeckR"), 1, 36.f, 9, 3, 1.f);

Zone(TEXT("People: The last lifts, 23.89 km"), 23.87f, 24.721f);
	Site(TEXT("PplDeckR"), 1, 36.f, 9, 3, 1.f);

Zone(TEXT("People: The last lifts, 22.80 km"), 22.784f, 22.821f);
	Scatter(ICE("SM_BoardPipe_A"), 50.f, 52.f, 9, 0, 1.f, 1.f, 0.f, -1, 0.f);     // boarders swinging in a pipe

Zone(TEXT("People: The last lifts, 23.62 km"), 23.599f, 23.635f);
	Scatter(ICE("SM_BoardPipe_A"), 50.f, 52.f, 9, 0, 1.f, 1.f, 0.f, -1, 0.f);     // boarders swinging in a pipe

Zone(TEXT("People: The last lifts, 24.43 km"), 24.413f, 24.449f);
	Scatter(ICE("SM_BoardPipe_A"), 50.f, 52.f, 9, 0, 1.f, 1.f, 0.f, -1, 0.f);     // boarders swinging in a pipe

Zone(TEXT("People: The last lifts, 22.71 km"), 22.694f, 23.544f);
	Scatter(ICE("SM_BoardPipe_A"), 78.f, 82.f, 9, 2, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The last lifts, 24.34 km"), 24.323f, 24.766f);
	Scatter(ICE("SM_BoardPipe_A"), 78.f, 82.f, 9, 2, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The last lifts, 22.44 km"), 22.422f, 22.459f);
	Scatter(ICE("SM_DogSled_A"), 84.f, 90.f, 18, 8, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The last lifts, 24.07 km"), 24.051f, 24.087f);
	Scatter(ICE("SM_DogSled_A"), 84.f, 90.f, 18, 8, 1.f, 1.f, 0.f, 1, 0.f);

#undef ICE
#undef CREW

{
// ---- BEGIN far (ft_people_ice_far.py) ----
// Far life and creatures (the extra worker "far"): everything here stands past the land's edge (305 to 330 m, its foot
// 20 m over the road's level: the land is 26 m up at 300 m) or flies over it, by Far rules, so it stays through loops,
// jumps and pipes and casts no shadow. Slots of 45 m: a side has a piece every 8 slots (360 m), the two sides out of
// step, and the eagles between them; every piece is laid 1.6 times its built size (at 1.0 it was a speck); a district has two kinds of piece, and they change with the district.
#define ICEFAR(Name) TEXT("/Game/People/IceMountain/SM_Far" Name)
Zone(TEXT("People far: the top and the first pipe"), 0.0f, 2.43f);
	Far(ICEFAR("SkiFace"), 305.f, 330.f, 16, 0, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("SledRace"), 305.f, 330.f, 16, 8, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("SledRace"), 305.f, 330.f, 16, 4, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("SkiFace"), 305.f, 330.f, 16, 12, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 2, 1.f, 1.4f, 0.f, -1, -10.f, 180.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 10, 1.f, 1.4f, 0.f, 1, -10.f, 180.f);
Zone(TEXT("People far: the chute and Black diamond"), 2.43f, 5.65f);
	Far(ICEFAR("ClimbWall"), 305.f, 330.f, 16, 0, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("IbexArete"), 305.f, 330.f, 16, 8, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("IbexArete"), 305.f, 330.f, 16, 4, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("ClimbWall"), 305.f, 330.f, 16, 12, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 2, 1.f, 1.4f, 0.f, -1, -10.f, 180.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 10, 1.f, 1.4f, 0.f, 1, -10.f, 180.f);
Zone(TEXT("People far: the whiteout"), 5.65f, 7.3f);
	Far(ICEFAR("WolfCrest"), 305.f, 330.f, 16, 0, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("IbexArete"), 305.f, 330.f, 16, 8, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("IbexArete"), 305.f, 330.f, 16, 4, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("WolfCrest"), 305.f, 330.f, 16, 12, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 2, 1.f, 1.4f, 0.f, -1, -10.f, 180.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 10, 1.f, 1.4f, 0.f, 1, -10.f, 180.f);
Zone(TEXT("People far: the valley"), 7.3f, 8.94f);
	Far(ICEFAR("SkiFace"), 305.f, 330.f, 16, 0, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("SledRace"), 305.f, 330.f, 16, 8, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("SledRace"), 305.f, 330.f, 16, 4, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("SkiFace"), 305.f, 330.f, 16, 12, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 2, 1.f, 1.4f, 0.f, -1, -10.f, 180.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 10, 1.f, 1.4f, 0.f, 1, -10.f, 180.f);
Zone(TEXT("People far: the glacier"), 8.94f, 11.6f);
	Far(ICEFAR("ClimbWall"), 305.f, 330.f, 16, 0, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("ClimbWall"), 305.f, 330.f, 16, 8, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);     // the ice walls by the road hide a low crest here: walls on every slot of the cycle
	Far(ICEFAR("ClimbWall"), 305.f, 330.f, 16, 4, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);     // the ice walls by the road hide a low crest here: walls on every slot of the cycle
	Far(ICEFAR("ClimbWall"), 305.f, 330.f, 16, 12, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 2, 1.f, 1.4f, 0.f, -1, -10.f, 180.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 10, 1.f, 1.4f, 0.f, 1, -10.f, 180.f);
Zone(TEXT("People far: the white ridge"), 11.6f, 14.3f);
	Far(ICEFAR("WolfCrest"), 305.f, 330.f, 16, 0, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("IbexArete"), 305.f, 330.f, 16, 8, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("IbexArete"), 305.f, 330.f, 16, 4, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("WolfCrest"), 305.f, 330.f, 16, 12, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 2, 1.f, 1.4f, 0.f, -1, -10.f, 180.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 10, 1.f, 1.4f, 0.f, 1, -10.f, 180.f);
Zone(TEXT("People far: the resort"), 14.3f, 17.0f);
	Far(ICEFAR("SkiFace"), 305.f, 330.f, 16, 0, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("SledRace"), 305.f, 330.f, 16, 8, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("SledRace"), 305.f, 330.f, 16, 4, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("SkiFace"), 305.f, 330.f, 16, 12, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 2, 1.f, 1.4f, 0.f, -1, -10.f, 180.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 10, 1.f, 1.4f, 0.f, 1, -10.f, 180.f);
Zone(TEXT("People far: the ice again"), 17.0f, 19.7f);
	Far(ICEFAR("ClimbWall"), 305.f, 330.f, 16, 0, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("ClimbWall"), 305.f, 330.f, 16, 8, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);     // the ice walls by the road hide a low crest here: walls on every slot of the cycle
	Far(ICEFAR("ClimbWall"), 305.f, 330.f, 16, 4, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);     // the ice walls by the road hide a low crest here: walls on every slot of the cycle
	Far(ICEFAR("ClimbWall"), 305.f, 330.f, 16, 12, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 2, 1.f, 1.4f, 0.f, -1, -10.f, 180.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 10, 1.f, 1.4f, 0.f, 1, -10.f, 180.f);
Zone(TEXT("People far: the high ridge"), 19.7f, 22.4f);
	Far(ICEFAR("IbexArete"), 305.f, 330.f, 16, 0, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("WolfCrest"), 305.f, 330.f, 16, 8, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("WolfCrest"), 305.f, 330.f, 16, 4, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("IbexArete"), 305.f, 330.f, 16, 12, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 2, 1.f, 1.4f, 0.f, -1, -10.f, 180.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 10, 1.f, 1.4f, 0.f, 1, -10.f, 180.f);
Zone(TEXT("People far: the last lifts"), 22.4f, 99.0f);
	Far(ICEFAR("SkiFace"), 305.f, 330.f, 16, 0, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("SledRace"), 305.f, 330.f, 16, 8, 1.f, 1.6f, 0.f, -1, -20.f, 6.f);
	Far(ICEFAR("SledRace"), 305.f, 330.f, 16, 4, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("SkiFace"), 305.f, 330.f, 16, 12, 1.f, 1.6f, 0.f, 1, -20.f, 6.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 2, 1.f, 1.4f, 0.f, -1, -10.f, 180.f);
	Far(ICEFAR("Eagles"), 240.f, 420.f, 16, 10, 1.f, 1.4f, 0.f, 1, -10.f, 180.f);
// ---- END far ----
}

{
// ---- BEGIN watch (ft_people_ice_watch.py) ----
// THE WATCHERS: the people who watch the chase, raised and massed, 30 to 56 m out. Six meshes (SM_Watch...): a race-finish
// stand under a chequered gantry, two snow banks of spectators behind a net, a scaffold tower of flag wavers, a snow
// groomer used as a stand, a lift platform with its queue. Each is put down by a set that turns it 35 degrees towards
// the car coming down the road (R sets: yaw -35; L sets: +35), as the main table does with its stands.
// Where they stand: 42 m (resort districts and ridges: behind the main cast's 34 m stands and in other slots than its
// big-air hills and pipes), 55 m (towers: tall, they show over what is in front), 30 m in the glacier districts
// (the ice walls stand 34 m out there: anything behind them is hidden), and by Far rules at 46 m under the loops and
// the corkscrews, which leave Scatter and Site out. All are TALL: none nearer than 30 m. Nothing glows.
#define WATCH(Name) TEXT("/Game/People/IceMountain/SM_Watch" Name)
Set(TEXT("PplWatchFinishR"), { { WATCH("Finish_A"), 0, 0, 0, -35, 1 } });
Set(TEXT("PplWatchFinishL"), { { WATCH("Finish_A"), 0, 0, 0, 35, 1 } });
Set(TEXT("PplWatchBankAR"), { { WATCH("Bank_A"), 0, 0, 0, -35, 1 } });
Set(TEXT("PplWatchBankAL"), { { WATCH("Bank_A"), 0, 0, 0, 35, 1 } });
Set(TEXT("PplWatchBankBR"), { { WATCH("Bank_B"), 0, 0, 0, -35, 1 } });
Set(TEXT("PplWatchBankBL"), { { WATCH("Bank_B"), 0, 0, 0, 35, 1 } });
Set(TEXT("PplWatchTowerR"), { { WATCH("Tower_A"), 0, 0, 0, -35, 1 } });
Set(TEXT("PplWatchTowerL"), { { WATCH("Tower_A"), 0, 0, 0, 35, 1 } });
Set(TEXT("PplWatchGroomerR"), { { WATCH("Groomer_A"), 0, 0, 0, -35, 1 } });
Set(TEXT("PplWatchGroomerL"), { { WATCH("Groomer_A"), 0, 0, 0, 35, 1 } });
Set(TEXT("PplWatchLiftR"), { { WATCH("Lift_A"), 0, 0, 0, -35, 1 } });
Set(TEXT("PplWatchLiftL"), { { WATCH("Lift_A"), 0, 0, 0, 35, 1 } });

Zone(TEXT("People watch: the start"), 0.0f, 0.1f);
	Site(TEXT("PplWatchBankAR"), 1, 42.0f, 2, 0, 1.00f);     // a bank of people either side of the start, flag towers behind
	Site(TEXT("PplWatchBankBL"), -1, 42.0f, 2, 1, 1.00f);
	Site(TEXT("PplWatchTowerL"), -1, 55.0f, 2, 0, 1.00f);
	Site(TEXT("PplWatchTowerR"), 1, 55.0f, 2, 1, 1.00f);

Zone(TEXT("People watch: below the summit street"), 0.15f, 0.24f);
	Site(TEXT("PplWatchFinishR"), 1, 42.0f, 2, 0, 1.00f);     // the finish stand on the right, the lift queue on the left
	Site(TEXT("PplWatchLiftL"), -1, 42.0f, 2, 1, 1.00f);

Zone(TEXT("People watch: First tracks"), 0.24f, 1.19f);
	Site(TEXT("PplWatchFinishR"), 1, 42.0f, 8, 0, 0.50f);     // right: finish stand, lift queue, bank, groomer in turn
	Site(TEXT("PplWatchLiftR"), 1, 42.0f, 8, 4, 0.50f);
	Site(TEXT("PplWatchBankAR"), 1, 42.0f, 8, 2, 0.50f);
	Site(TEXT("PplWatchGroomerR"), 1, 42.0f, 8, 6, 0.50f);
	Site(TEXT("PplWatchBankBL"), -1, 42.0f, 8, 0, 0.50f);     // left: the same cast, other slots
	Site(TEXT("PplWatchFinishL"), -1, 42.0f, 8, 4, 0.50f);
	Site(TEXT("PplWatchLiftL"), -1, 42.0f, 8, 2, 0.50f);
	Site(TEXT("PplWatchBankAL"), -1, 42.0f, 8, 6, 0.50f);
	Site(TEXT("PplWatchTowerR"), 1, 55.0f, 12, 1, 1.00f);     // flag towers further out
	Site(TEXT("PplWatchTowerL"), -1, 55.0f, 12, 7, 1.00f);

Zone(TEXT("People watch: The half pipe, competition day"), 1.19f, 2.43f);
	Site(TEXT("PplWatchFinishR"), 1, 42.0f, 8, 1, 0.50f);     // right: finish stand, lift queue, bank, groomer in turn
	Site(TEXT("PplWatchLiftR"), 1, 42.0f, 8, 5, 0.50f);
	Site(TEXT("PplWatchBankAR"), 1, 42.0f, 8, 3, 0.50f);
	Site(TEXT("PplWatchGroomerR"), 1, 42.0f, 8, 7, 0.50f);
	Site(TEXT("PplWatchBankBL"), -1, 42.0f, 8, 1, 0.50f);     // left: the same cast, other slots
	Site(TEXT("PplWatchFinishL"), -1, 42.0f, 8, 5, 0.50f);
	Site(TEXT("PplWatchLiftL"), -1, 42.0f, 8, 3, 0.50f);
	Site(TEXT("PplWatchBankAL"), -1, 42.0f, 8, 7, 0.50f);
	Site(TEXT("PplWatchTowerR"), 1, 55.0f, 12, 1, 1.00f);     // flag towers further out
	Site(TEXT("PplWatchTowerL"), -1, 55.0f, 12, 7, 1.00f);

Zone(TEXT("People watch: The whiteout"), 5.65f, 7.3f);
	Site(TEXT("PplWatchFinishR"), 1, 42.0f, 8, 0, 0.50f);     // right: finish stand, lift queue, bank, groomer in turn
	Site(TEXT("PplWatchLiftR"), 1, 42.0f, 8, 4, 0.50f);
	Site(TEXT("PplWatchBankAR"), 1, 42.0f, 8, 1, 0.50f);
	Site(TEXT("PplWatchGroomerR"), 1, 42.0f, 8, 5, 0.50f);
	Site(TEXT("PplWatchBankBL"), -1, 42.0f, 8, 1, 0.50f);     // left: the same cast, other slots
	Site(TEXT("PplWatchFinishL"), -1, 42.0f, 8, 5, 0.50f);
	Site(TEXT("PplWatchLiftL"), -1, 42.0f, 8, 3, 0.50f);
	Site(TEXT("PplWatchBankAL"), -1, 42.0f, 8, 7, 0.50f);
	Site(TEXT("PplWatchTowerR"), 1, 55.0f, 12, 1, 1.00f);     // flag towers further out
	Site(TEXT("PplWatchTowerL"), -1, 55.0f, 12, 7, 1.00f);

Zone(TEXT("People watch: The valley run"), 7.3f, 8.94f);
	Site(TEXT("PplWatchFinishR"), 1, 42.0f, 8, 0, 0.50f);     // right: finish stand, lift queue, bank, groomer in turn
	Site(TEXT("PplWatchLiftR"), 1, 42.0f, 8, 4, 0.50f);
	Site(TEXT("PplWatchBankAR"), 1, 42.0f, 8, 1, 0.50f);
	Site(TEXT("PplWatchGroomerR"), 1, 42.0f, 8, 5, 0.50f);
	Site(TEXT("PplWatchBankBL"), -1, 42.0f, 8, 1, 0.50f);     // left: the same cast, other slots
	Site(TEXT("PplWatchFinishL"), -1, 42.0f, 8, 5, 0.50f);
	Site(TEXT("PplWatchLiftL"), -1, 42.0f, 8, 3, 0.50f);
	Site(TEXT("PplWatchBankAL"), -1, 42.0f, 8, 7, 0.50f);
	Site(TEXT("PplWatchTowerR"), 1, 55.0f, 12, 1, 1.00f);     // flag towers further out
	Site(TEXT("PplWatchTowerL"), -1, 55.0f, 12, 7, 1.00f);

Zone(TEXT("People watch: The resort"), 14.3f, 17.0f);
	Site(TEXT("PplWatchFinishR"), 1, 42.0f, 8, 0, 0.50f);     // right: finish stand, lift queue, bank, groomer in turn
	Site(TEXT("PplWatchLiftR"), 1, 42.0f, 8, 4, 0.50f);
	Site(TEXT("PplWatchBankAR"), 1, 42.0f, 8, 1, 0.50f);
	Site(TEXT("PplWatchGroomerR"), 1, 42.0f, 8, 5, 0.50f);
	Site(TEXT("PplWatchBankBL"), -1, 42.0f, 8, 0, 0.50f);     // left: the same cast, other slots
	Site(TEXT("PplWatchFinishL"), -1, 42.0f, 8, 4, 0.50f);
	Site(TEXT("PplWatchLiftL"), -1, 42.0f, 8, 2, 0.50f);
	Site(TEXT("PplWatchBankAL"), -1, 42.0f, 8, 6, 0.50f);
	Site(TEXT("PplWatchTowerL"), -1, 55.0f, 12, 7, 1.00f);

Zone(TEXT("People watch: The last lifts"), 22.4f, 99.0f);
	Site(TEXT("PplWatchFinishR"), 1, 42.0f, 8, 0, 0.50f);     // right: finish stand, lift queue, bank, groomer in turn
	Site(TEXT("PplWatchLiftR"), 1, 42.0f, 8, 4, 0.50f);
	Site(TEXT("PplWatchBankAR"), 1, 42.0f, 8, 1, 0.50f);
	Site(TEXT("PplWatchGroomerR"), 1, 42.0f, 8, 5, 0.50f);
	Site(TEXT("PplWatchBankBL"), -1, 42.0f, 8, 0, 0.50f);     // left: the same cast, other slots
	Site(TEXT("PplWatchFinishL"), -1, 42.0f, 8, 4, 0.50f);
	Site(TEXT("PplWatchLiftL"), -1, 42.0f, 8, 2, 0.50f);
	Site(TEXT("PplWatchBankAL"), -1, 42.0f, 8, 6, 0.50f);
	Site(TEXT("PplWatchTowerR"), 1, 55.0f, 12, 1, 1.00f);     // flag towers further out
	Site(TEXT("PplWatchTowerL"), -1, 55.0f, 12, 7, 1.00f);

Zone(TEXT("People watch: Black diamond"), 4.02f, 5.65f);
	Site(TEXT("PplWatchTowerR"), 1, 44.0f, 10, 1, 1.00f);     // flag towers on the open ridge
	Site(TEXT("PplWatchBankAR"), 1, 42.0f, 10, 4, 0.50f);
	Site(TEXT("PplWatchGroomerR"), 1, 42.0f, 10, 9, 0.50f);
	Site(TEXT("PplWatchBankBL"), -1, 42.0f, 10, 2, 0.50f);
	Site(TEXT("PplWatchTowerL"), -1, 44.0f, 10, 7, 1.00f);
	Site(TEXT("PplWatchGroomerL"), -1, 42.0f, 10, 6, 0.50f);

Zone(TEXT("People watch: The white ridge"), 11.6f, 14.3f);
	Site(TEXT("PplWatchTowerR"), 1, 44.0f, 10, 1, 1.00f);     // flag towers on the open ridge
	Site(TEXT("PplWatchBankAR"), 1, 42.0f, 10, 4, 0.50f);
	Site(TEXT("PplWatchGroomerR"), 1, 42.0f, 10, 9, 0.50f);
	Site(TEXT("PplWatchBankBL"), -1, 42.0f, 10, 2, 0.50f);
	Site(TEXT("PplWatchTowerL"), -1, 44.0f, 10, 7, 1.00f);
	Site(TEXT("PplWatchGroomerL"), -1, 42.0f, 10, 6, 0.50f);

Zone(TEXT("People watch: The high ridge"), 19.7f, 22.4f);
	Site(TEXT("PplWatchTowerR"), 1, 44.0f, 10, 1, 1.00f);     // flag towers on the open ridge
	Site(TEXT("PplWatchBankAR"), 1, 42.0f, 10, 4, 0.50f);
	Site(TEXT("PplWatchGroomerR"), 1, 42.0f, 10, 9, 0.50f);
	Site(TEXT("PplWatchBankBL"), -1, 42.0f, 10, 2, 0.50f);
	Site(TEXT("PplWatchTowerL"), -1, 44.0f, 10, 7, 1.00f);
	Site(TEXT("PplWatchGroomerL"), -1, 42.0f, 10, 6, 0.50f);

Zone(TEXT("People watch: Avalanche chute"), 2.43f, 2.96f);
	Site(TEXT("PplWatchTowerR"), 1, 30.0f, 8, 2, 0.50f);     // in front of the ice walls (34 m): a tower, a bank, a groomer
	Site(TEXT("PplWatchBankAR"), 1, 30.5f, 8, 3, 0.50f);
	Site(TEXT("PplWatchGroomerR"), 1, 30.0f, 8, 6, 0.50f);
	Site(TEXT("PplWatchBankBL"), -1, 30.5f, 8, 0, 0.50f);
	Site(TEXT("PplWatchTowerL"), -1, 30.0f, 8, 1, 0.50f);

Zone(TEXT("People watch: The super pipe"), 2.96f, 4.02f);
	Site(TEXT("PplWatchTowerR"), 1, 30.0f, 8, 2, 0.50f);     // in front of the ice walls (34 m): a tower, a bank, a groomer
	Site(TEXT("PplWatchBankAR"), 1, 30.5f, 8, 3, 0.50f);
	Site(TEXT("PplWatchGroomerR"), 1, 30.0f, 8, 6, 0.50f);
	Site(TEXT("PplWatchBankBL"), -1, 30.5f, 8, 0, 0.50f);
	Site(TEXT("PplWatchTowerL"), -1, 30.0f, 8, 1, 0.50f);

Zone(TEXT("People watch: The Abominable's glacier"), 8.94f, 11.6f);
	Site(TEXT("PplWatchTowerR"), 1, 30.0f, 8, 0, 0.50f);     // in front of the ice walls (34 m): a tower, a bank, a groomer
	Site(TEXT("PplWatchBankAR"), 1, 30.5f, 8, 1, 0.50f);
	Site(TEXT("PplWatchGroomerR"), 1, 30.0f, 8, 4, 0.50f);
	Site(TEXT("PplWatchBankBL"), -1, 30.5f, 8, 2, 0.50f);
	Site(TEXT("PplWatchTowerL"), -1, 30.0f, 8, 3, 0.50f);

Zone(TEXT("People watch: The ice again"), 17.0f, 19.7f);
	Site(TEXT("PplWatchTowerR"), 1, 30.0f, 8, 2, 0.50f);     // in front of the ice walls (34 m): a tower, a bank, a groomer
	Site(TEXT("PplWatchBankAR"), 1, 30.5f, 8, 3, 0.50f);
	Site(TEXT("PplWatchGroomerR"), 1, 30.0f, 8, 6, 0.50f);
	Site(TEXT("PplWatchBankBL"), -1, 30.5f, 8, 0, 0.50f);
	Site(TEXT("PplWatchTowerL"), -1, 30.0f, 8, 1, 0.50f);

Zone(TEXT("People watch: under the chute's corkscrew"), 2.51f, 2.685f);
	Far(WATCH("Tower_A"), 46.0f, 47.0f, 3, 1, 1.f, 1.f, 0.f, 1, 0.80f, 0.f);
	Far(WATCH("Bank_A"), 46.0f, 47.0f, 3, 2, 1.f, 1.f, 0.f, 1, 0.80f, 0.f);
	Far(WATCH("Tower_A"), 46.0f, 47.0f, 3, 0, 1.f, 1.f, 0.f, -1, 0.80f, 0.f);
	Far(WATCH("Bank_B"), 46.0f, 47.0f, 3, 1, 1.f, 1.f, 0.f, -1, 0.80f, 0.f);

Zone(TEXT("People watch: under Black diamond's loop"), 4.085f, 4.37f);
	Far(WATCH("Tower_A"), 46.0f, 47.0f, 3, 2, 1.f, 1.f, 0.f, 1, 0.80f, 0.f);
	Far(WATCH("Bank_A"), 46.0f, 47.0f, 3, 0, 1.f, 1.f, 0.f, 1, 0.80f, 0.f);
	Far(WATCH("Tower_A"), 46.0f, 47.0f, 3, 1, 1.f, 1.f, 0.f, -1, 0.80f, 0.f);
	Far(WATCH("Bank_B"), 46.0f, 47.0f, 3, 2, 1.f, 1.f, 0.f, -1, 0.80f, 0.f);

Zone(TEXT("People watch: under Black diamond's corkscrew"), 5.225f, 5.4f);
	Far(WATCH("Tower_A"), 46.0f, 47.0f, 3, 0, 1.f, 1.f, 0.f, 1, 0.80f, 0.f);
	Far(WATCH("Bank_A"), 46.0f, 47.0f, 3, 1, 1.f, 1.f, 0.f, 1, 0.80f, 0.f);
	Far(WATCH("Tower_A"), 46.0f, 47.0f, 3, 2, 1.f, 1.f, 0.f, -1, 0.80f, 0.f);
	Far(WATCH("Bank_B"), 46.0f, 47.0f, 3, 0, 1.f, 1.f, 0.f, -1, 0.80f, 0.f);

Zone(TEXT("People watch: under the whiteout's loop"), 6.87f, 7.155f);
	Far(WATCH("Tower_A"), 46.0f, 47.0f, 3, 1, 1.f, 1.f, 0.f, 1, 0.80f, 0.f);
	Far(WATCH("Bank_A"), 46.0f, 47.0f, 3, 2, 1.f, 1.f, 0.f, 1, 0.80f, 0.f);
	Far(WATCH("Tower_A"), 46.0f, 47.0f, 3, 0, 1.f, 1.f, 0.f, -1, 0.80f, 0.f);
	Far(WATCH("Bank_B"), 46.0f, 47.0f, 3, 1, 1.f, 1.f, 0.f, -1, 0.80f, 0.f);

Zone(TEXT("People watch: under the valley's corkscrew"), 7.215f, 7.385f);
	Far(WATCH("Tower_A"), 46.0f, 47.0f, 3, 2, 1.f, 1.f, 0.f, 1, 0.80f, 0.f);
	Far(WATCH("Bank_A"), 46.0f, 47.0f, 3, 0, 1.f, 1.f, 0.f, 1, 0.80f, 0.f);
	Far(WATCH("Tower_A"), 46.0f, 47.0f, 3, 1, 1.f, 1.f, 0.f, -1, 0.80f, 0.f);
	Far(WATCH("Bank_B"), 46.0f, 47.0f, 3, 2, 1.f, 1.f, 0.f, -1, 0.80f, 0.f);
#undef WATCH
// ---- END watch ----
}
