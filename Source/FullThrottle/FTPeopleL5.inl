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

{
// ---- BEGIN p3a (gaps, ft_people_ice_p3a.py) ----
// (Fix round, 16 Nov 2026: after two reviews rules of this block were moved, re-phased or taken out; the counts in this header are the
// builder's. What changed and why: SourceArt/Integration/people/L5_ice_p3.md, "Review and fix round".)
// THE GAPS: the general cast of the mountain (at work, at rest, passing through), where nobody stood within 60 m.
// Thirteen meshes SM_P3A_*. Three are LOW (built under 1.15 m: families on blankets, children with sleds, sled dogs
// staked out) and stand 23 m out behind the crash padding, every sixth slot a side on residues of 12 that the crew's
// own pieces leave free (right 2 and 8, left 1 and 7), only in the stretches without banked bends (the dressing's
// verge zones). Ten are TALL (people 1.3 times life size on a mound of their own) and are put down by sets that turn
// them towards the car coming down the road, as the main table does: 46 m out on the ridges at 1.2 times (three
// slots in ten a side, on the even residues the watchers and the ibex leave free), 57 m out in the resorts at 1.5
// times (every third free slot a side: no hill, pipe, snowmen, sled, tower, depot or rink on that slot; 63 m on the
// right of the resort and the last lifts, behind the service road), 30 m out on
// the glaciers (only the slots without a climb, a stand, an ice wall, a watcher or a crew piece on that side).
// A rule written "600, phase" fires on one slot (slot = 600 - phase). Nothing glows; nothing nearer than 23 m;
// no TALL piece nearer than 30 m. Not filled: the avalanche chute (galleries and rock cuts to 40 m and more).
#define P3A(Name) TEXT("/Game/People/IceMountain/SM_P3A_" Name)
Set(TEXT("PplP3AHikersAR25"), { { P3A("Hikers_A"), 0, 0, 0, -25, 1 } });
Set(TEXT("PplP3ASurveyorsAR25"), { { P3A("Surveyors_A"), 0, 0, 0, -25, 1 } });
Set(TEXT("PplP3ACampAR25"), { { P3A("Camp_A"), 0, 0, 0, -25, 1 } });
Set(TEXT("PplP3AHikersBL25"), { { P3A("Hikers_B"), 0, 0, 0, 25, 1 } });
Set(TEXT("PplP3ARescueAL25"), { { P3A("Rescue_A"), 0, 0, 0, 25, 1 } });
Set(TEXT("PplP3APisteCrewAL25"), { { P3A("PisteCrew_A"), 0, 0, 0, 25, 1 } });
Set(TEXT("PplP3ARescueAR25"), { { P3A("Rescue_A"), 0, 0, 0, -25, 1 } });
Set(TEXT("PplP3ASurveyorsAL25"), { { P3A("Surveyors_A"), 0, 0, 0, 25, 1 } });
Set(TEXT("PplP3APisteCrewBL25"), { { P3A("PisteCrew_B"), 0, 0, 0, 25, 1 } });
Set(TEXT("PplP3ACampAL20"), { { P3A("Camp_A"), 0, 0, 0, 20, 1 } });
Set(TEXT("PplP3ASurveyorsAR20"), { { P3A("Surveyors_A"), 0, 0, 0, -20, 1 } });
Set(TEXT("PplP3ARescueAR20"), { { P3A("Rescue_A"), 0, 0, 0, -20, 1 } });
Set(TEXT("PplP3APisteCrewBR20"), { { P3A("PisteCrew_B"), 0, 0, 0, -20, 1 } });
Set(TEXT("PplP3ACampAR20"), { { P3A("Camp_A"), 0, 0, 0, -20, 1 } });
Set(TEXT("PplP3ARescueAL20"), { { P3A("Rescue_A"), 0, 0, 0, 20, 1 } });
Set(TEXT("PplP3APisteCrewBL20"), { { P3A("PisteCrew_B"), 0, 0, 0, 20, 1 } });
Set(TEXT("PplP3AIglooAR30"), { { P3A("Igloo_A"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3ASkiSchoolAR30"), { { P3A("SkiSchool_A"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3AHotDrinksAR30"), { { P3A("HotDrinks_A"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3APisteCrewBR30"), { { P3A("PisteCrew_B"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3AHikersAR30"), { { P3A("Hikers_A"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3APisteCrewAL30"), { { P3A("PisteCrew_A"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3AHikersBL30"), { { P3A("Hikers_B"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3AIglooAL30"), { { P3A("Igloo_A"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3ASkiSchoolAL30"), { { P3A("SkiSchool_A"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3AHotDrinksAL30"), { { P3A("HotDrinks_A"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3APisteCrewBL30"), { { P3A("PisteCrew_B"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3AHikersAL30"), { { P3A("Hikers_A"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3APisteCrewAR30"), { { P3A("PisteCrew_A"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3AHikersBR30"), { { P3A("Hikers_B"), 0, 0, 0, -30, 1 } });

Zone(TEXT("People p3a: low, the super pipe"), 2.96f, 3.59f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: low, Black diamond"), 4.38f, 4.46f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People p3a: low, the whiteout"), 5.66f, 6.33f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: low, the valley"), 8.39f, 8.78f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: low, the Abominable's glacier"), 8.94f, 11.6f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: low, the white ridge"), 11.6f, 14.3f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: low, the resort (1)"), 14.3f, 14.62f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: low, the resort (2)"), 14.97f, 15.75f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: low, the resort (3)"), 16.11f, 16.89f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: low, the ice again (1)"), 17.24f, 18.03f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: low, the ice again (2)"), 18.38f, 19.17f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: low, the ice again and the high ridge"), 19.52f, 19.7f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: low, the high ridge (1)"), 19.7f, 20.3f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: low, the high ridge (2)"), 20.66f, 21.44f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: low, the high ridge (3)"), 21.79f, 22.4f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3A("Huskies_A"), 23.f, 23.6f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: low, the last lifts (1)"), 22.4f, 22.58f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: low, the last lifts (2)"), 22.93f, 23.72f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: low, the last lifts (3)"), 24.07f, 24.8f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3A("SledKids_A"), 23.f, 23.6f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3A("SnowSeat_A"), 23.f, 23.6f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People p3a: Black diamond"), 4.02f, 5.65f);
	Site(TEXT("PplP3AHikersAR25"), 1, 46.f, 10, 8, 1.f, 1.2f);
	Site(TEXT("PplP3ASurveyorsAR25"), 1, 46.f, 10, 2, 1.f, 1.2f);
	Site(TEXT("PplP3ACampAR25"), 1, 46.f, 10, 0, 1.f, 1.2f);
	Site(TEXT("PplP3AHikersBL25"), -1, 46.f, 10, 0, 1.f, 1.2f);
	Site(TEXT("PplP3ARescueAL25"), -1, 46.f, 10, 8, 1.f, 1.2f);

Zone(TEXT("People p3a: the white ridge"), 11.6f, 14.3f);
	Site(TEXT("PplP3AHikersAR25"), 1, 46.f, 10, 8, 1.f, 1.2f);
	Site(TEXT("PplP3ASurveyorsAR25"), 1, 46.f, 10, 2, 1.f, 1.2f);
	Site(TEXT("PplP3ACampAR25"), 1, 46.f, 10, 6, 1.f, 1.2f);
	Site(TEXT("PplP3AHikersBL25"), -1, 46.f, 10, 0, 1.f, 1.2f);
	Site(TEXT("PplP3ARescueAL25"), -1, 46.f, 10, 8, 1.f, 1.2f);
	Site(TEXT("PplP3APisteCrewAL25"), -1, 46.f, 10, 4, 1.f, 1.2f);

Zone(TEXT("People p3a: the high ridge"), 19.7f, 22.4f);
	Site(TEXT("PplP3ACampAR25"), 1, 46.f, 10, 8, 1.f, 1.2f);
	Site(TEXT("PplP3AHikersAR25"), 1, 46.f, 10, 2, 1.f, 1.2f);
	Site(TEXT("PplP3ARescueAR25"), 1, 46.f, 10, 6, 1.f, 1.2f);
	Site(TEXT("PplP3ASurveyorsAL25"), -1, 46.f, 10, 0, 1.f, 1.2f);
	Site(TEXT("PplP3AHikersBL25"), -1, 46.f, 10, 8, 1.f, 1.2f);
	Site(TEXT("PplP3APisteCrewBL25"), -1, 46.f, 10, 4, 1.f, 1.2f);

Zone(TEXT("People p3a: on the ice, the super pipe"), 2.96f, 3.59f);
	Site(TEXT("PplP3ACampAL20"), -1, 30.f, 600, 531, 1.f, 1.f);

Zone(TEXT("People p3a: on the ice, the Abominable's glacier"), 8.94f, 11.6f);
	Site(TEXT("PplP3ASurveyorsAR20"), 1, 30.f, 600, 398, 1.f, 1.f);
	Site(TEXT("PplP3ARescueAR20"), 1, 30.f, 600, 386, 1.f, 1.f);
	Site(TEXT("PplP3APisteCrewBR20"), 1, 30.f, 600, 374, 1.f, 1.f);
	Site(TEXT("PplP3ACampAR20"), 1, 30.f, 600, 362, 1.f, 1.f);
	Site(TEXT("PplP3ASurveyorsAR20"), 1, 30.f, 600, 350, 1.f, 1.f);
	Site(TEXT("PplP3ARescueAL20"), -1, 30.f, 600, 398, 1.f, 1.f);
	Site(TEXT("PplP3APisteCrewBL20"), -1, 30.f, 600, 374, 1.f, 1.f);
	Site(TEXT("PplP3ACampAL20"), -1, 30.f, 600, 350, 1.f, 1.f);

Zone(TEXT("People p3a: on the ice, the ice again (1)"), 17.24f, 18.03f);
	Site(TEXT("PplP3ASurveyorsAR20"), 1, 30.f, 600, 207, 1.f, 1.f);
	Site(TEXT("PplP3ARescueAL20"), -1, 30.f, 600, 207, 1.f, 1.f);

Zone(TEXT("People p3a: on the ice, the ice again (2)"), 18.38f, 19.17f);
	Site(TEXT("PplP3APisteCrewBR20"), 1, 30.f, 600, 183, 1.f, 1.f);
	Site(TEXT("PplP3ACampAL20"), -1, 30.f, 600, 183, 1.f, 1.f);

Zone(TEXT("People p3a: the resort cast, First tracks"), 0.26f, 1.19f);
	Site(TEXT("PplP3AIglooAR30"), 1, 57.f, 600, 594, 1.f, 1.5f);
	Site(TEXT("PplP3ASkiSchoolAR30"), 1, 57.f, 600, 588, 1.f, 1.5f);
	Site(TEXT("PplP3AHotDrinksAR30"), 1, 57.f, 600, 585, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewBR30"), 1, 57.f, 600, 580, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersAR30"), 1, 57.f, 600, 576, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewAL30"), -1, 57.f, 600, 593, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersBL30"), -1, 57.f, 600, 590, 1.f, 1.5f);
	Site(TEXT("PplP3AIglooAL30"), -1, 57.f, 600, 587, 1.f, 1.5f);
	Site(TEXT("PplP3ASkiSchoolAL30"), -1, 57.f, 600, 584, 1.f, 1.5f);
	Site(TEXT("PplP3AHotDrinksAL30"), -1, 57.f, 600, 581, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewBL30"), -1, 57.f, 600, 578, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersAL30"), -1, 57.f, 600, 575, 1.f, 1.5f);

Zone(TEXT("People p3a: the resort cast, the half pipe"), 1.19f, 2.08f);
	Site(TEXT("PplP3APisteCrewAR30"), 1, 57.f, 600, 573, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersBR30"), 1, 57.f, 600, 570, 1.f, 1.5f);
	Site(TEXT("PplP3AIglooAR30"), 1, 57.f, 600, 567, 1.f, 1.5f);
	Site(TEXT("PplP3ASkiSchoolAR30"), 1, 57.f, 600, 564, 1.f, 1.5f);
	Site(TEXT("PplP3AHotDrinksAR30"), 1, 57.f, 600, 561, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewBR30"), 1, 57.f, 600, 558, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersAR30"), 1, 57.f, 600, 555, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewAL30"), -1, 57.f, 600, 572, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersBL30"), -1, 57.f, 600, 568, 1.f, 1.5f);
	Site(TEXT("PplP3AIglooAL30"), -1, 57.f, 600, 565, 1.f, 1.5f);
	Site(TEXT("PplP3ASkiSchoolAL30"), -1, 57.f, 600, 562, 1.f, 1.5f);
	Site(TEXT("PplP3AHotDrinksAL30"), -1, 57.f, 600, 558, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewBL30"), -1, 57.f, 600, 555, 1.f, 1.5f);

Zone(TEXT("People p3a: the resort cast, the whiteout"), 5.65f, 6.85f);
	Site(TEXT("PplP3AHikersAR30"), 1, 57.f, 600, 475, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewAR30"), 1, 57.f, 600, 472, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersBR30"), 1, 57.f, 600, 468, 1.f, 1.5f);
	Site(TEXT("PplP3AIglooAR30"), 1, 57.f, 600, 465, 1.f, 1.5f);
	Site(TEXT("PplP3ASkiSchoolAR30"), 1, 57.f, 600, 462, 1.f, 1.5f);
	Site(TEXT("PplP3AHotDrinksAR30"), 1, 57.f, 600, 459, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewBR30"), 1, 57.f, 600, 456, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersAR30"), 1, 57.f, 600, 453, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewAR30"), 1, 57.f, 600, 450, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersBL30"), -1, 57.f, 600, 473, 1.f, 1.5f);
	Site(TEXT("PplP3AIglooAL30"), -1, 57.f, 600, 470, 1.f, 1.5f);
	Site(TEXT("PplP3ASkiSchoolAL30"), -1, 57.f, 600, 467, 1.f, 1.5f);
	Site(TEXT("PplP3AHotDrinksAL30"), -1, 57.f, 600, 464, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewBL30"), -1, 57.f, 600, 461, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersAL30"), -1, 57.f, 600, 458, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewAL30"), -1, 57.f, 600, 455, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersBL30"), -1, 57.f, 600, 452, 1.f, 1.5f);
	Site(TEXT("PplP3AIglooAL30"), -1, 57.f, 600, 449, 1.f, 1.5f);

Zone(TEXT("People p3a: the resort cast, the valley (1)"), 7.52f, 7.98f);
	Site(TEXT("PplP3ASkiSchoolAR30"), 1, 57.f, 600, 432, 1.f, 1.5f);
	Site(TEXT("PplP3AHotDrinksAR30"), 1, 57.f, 600, 429, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewBR30"), 1, 57.f, 600, 426, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersAL30"), -1, 57.f, 600, 433, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewAL30"), -1, 57.f, 600, 430, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersBL30"), -1, 57.f, 600, 425, 1.f, 1.5f);

Zone(TEXT("People p3a: the resort cast, the valley (2)"), 8.39f, 8.94f);
	Site(TEXT("PplP3AIglooAR30"), 1, 57.f, 600, 414, 1.f, 1.5f);
	Site(TEXT("PplP3ASkiSchoolAR30"), 1, 57.f, 600, 411, 1.f, 1.5f);
	Site(TEXT("PplP3AHotDrinksAR30"), 1, 57.f, 600, 408, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewBR30"), 1, 57.f, 600, 405, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersAL30"), -1, 57.f, 600, 413, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewAL30"), -1, 57.f, 600, 410, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersBL30"), -1, 57.f, 600, 407, 1.f, 1.5f);
	Site(TEXT("PplP3AIglooAL30"), -1, 57.f, 600, 404, 1.f, 1.5f);

Zone(TEXT("People p3a: the resort cast, the resort (1)"), 14.62f, 15.44f);
	Site(TEXT("PplP3ASkiSchoolAR30"), 1, 63.f, 600, 275, 1.f, 1.5f);
	Site(TEXT("PplP3AHotDrinksAR30"), 1, 63.f, 600, 271, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewBR30"), 1, 63.f, 600, 269, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersAR30"), 1, 63.f, 600, 263, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewAR30"), 1, 63.f, 600, 261, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersBL30"), -1, 57.f, 600, 272, 1.f, 1.5f);
	Site(TEXT("PplP3AIglooAL30"), -1, 57.f, 600, 266, 1.f, 1.5f);
	Site(TEXT("PplP3ASkiSchoolAL30"), -1, 57.f, 600, 260, 1.f, 1.5f);

Zone(TEXT("People p3a: the resort cast, the resort (2)"), 15.75f, 16.58f);
	Site(TEXT("PplP3AHotDrinksAR30"), 1, 63.f, 600, 249, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewBR30"), 1, 63.f, 600, 247, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersAR30"), 1, 63.f, 600, 243, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewAR30"), 1, 63.f, 600, 239, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersBR30"), 1, 63.f, 600, 235, 1.f, 1.5f);
	Site(TEXT("PplP3AIglooAL30"), -1, 57.f, 600, 248, 1.f, 1.5f);
	Site(TEXT("PplP3ASkiSchoolAL30"), -1, 57.f, 600, 242, 1.f, 1.5f);
	Site(TEXT("PplP3AHotDrinksAL30"), -1, 57.f, 600, 236, 1.f, 1.5f);

Zone(TEXT("People p3a: the resort cast, the last lifts (1)"), 22.58f, 23.4f);
	Site(TEXT("PplP3AHikersAR30"), 1, 63.f, 600, 99, 1.f, 1.5f);

	Site(TEXT("PplP3AHikersBR30"), 1, 63.f, 600, 91, 1.f, 1.5f);
	Site(TEXT("PplP3AIglooAR30"), 1, 63.f, 600, 89, 1.f, 1.5f);
	Site(TEXT("PplP3ASkiSchoolAR30"), 1, 63.f, 600, 83, 1.f, 1.5f);
	Site(TEXT("PplP3AHotDrinksAL30"), -1, 57.f, 600, 98, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewBL30"), -1, 57.f, 600, 92, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersAL30"), -1, 57.f, 600, 86, 1.f, 1.5f);

Zone(TEXT("People p3a: the resort cast, the last lifts (2)"), 23.72f, 24.8f);
	Site(TEXT("PplP3APisteCrewAR30"), 1, 63.f, 600, 75, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersBR30"), 1, 63.f, 600, 71, 1.f, 1.5f);
	Site(TEXT("PplP3AIglooAR30"), 1, 63.f, 600, 67, 1.f, 1.5f);
	Site(TEXT("PplP3ASkiSchoolAR30"), 1, 63.f, 600, 63, 1.f, 1.5f);
	Site(TEXT("PplP3AHotDrinksAR30"), 1, 63.f, 600, 59, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewBR30"), 1, 63.f, 600, 55, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersAR30"), 1, 63.f, 600, 53, 1.f, 1.5f);
	Site(TEXT("PplP3APisteCrewAL30"), -1, 57.f, 600, 74, 1.f, 1.5f);
	Site(TEXT("PplP3AHikersBL30"), -1, 57.f, 600, 68, 1.f, 1.5f);
	Site(TEXT("PplP3AIglooAL30"), -1, 57.f, 600, 62, 1.f, 1.5f);
	Site(TEXT("PplP3ASkiSchoolAL30"), -1, 57.f, 600, 56, 1.f, 1.5f);
#undef P3A
// ---- END p3a ----
}

{
// ---- BEGIN p3b (scenes, ft_people_ice_p3b.py) ----
// (Fix round, 16 Nov 2026: after two reviews rules of this block were moved, re-phased or taken out; the counts in this header are the
// builder's. What changed and why: SourceArt/Integration/people/L5_ice_p3.md, "Review and fix round".)
// STORY SCENES of Ice Mountain (pass 3, builder p3b): eleven meshes (SM_P3B_...), each a group of 8 to 12 people 1.5 times
// life size on a terrace of packed snow 1.1 m high with a skirt of colour blocks: a mountain-rescue drill, an ice sculpture
// contest, a wedding under an arch of skis, a curling match, alphorns with flag throwers and a ring dance, a snowball
// battle between two forts, an expedition camp with an igloo going up, a medal ceremony, an ice fishing derby, an
// ice-core drilling station, pond hockey. All TALL (tops 4.6 to 8.1 m): none nearer than 31.5 m. Nothing glows or blinks.
// Each is put down by a one-piece set that turns it 30 degrees towards the car coming down the road (R: -30, L: +30).
// Where: one piece on one slot and side, by a zone that holds just that slot (written by a scratch tool that read both
// tables: a slot and side is used only where nothing of the dressing or of the people already placed stands within
// 5.5 m of the scene (itself 5.5 m either way); 31.5 m out where the road is not banked, else 38 or 44 m; 44 or 38 m beside the banked bends; never
// two of these on neighbouring slots of a side; none beside the jumps). Under the loops and the corkscrews, where Site is
// left out, they stand 57 m out by Far rules with the drop of the land, facing the road.
// Resort districts: wedding, podium, curling, alphorns, sculptors, snow forts, pond hockey. Glacier districts: ice cores,
// ice fishing, base camp, rescue. Ridge districts: rescue, snow forts, base camp, alphorns, ice cores.
#define P3B(n) TEXT("/Game/People/IceMountain/SM_P3B_" n "_A")
Set(TEXT("PplP3BAlphornsR"), { { P3B("Alphorns"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3BAlphornsL"), { { P3B("Alphorns"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3BBaseCampR"), { { P3B("BaseCamp"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3BBaseCampL"), { { P3B("BaseCamp"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3BCurlingR"), { { P3B("Curling"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3BCurlingL"), { { P3B("Curling"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3BIceCoreR"), { { P3B("IceCore"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3BIceCoreL"), { { P3B("IceCore"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3BIceFishingR"), { { P3B("IceFishing"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3BIceFishingL"), { { P3B("IceFishing"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3BIceSculptR"), { { P3B("IceSculpt"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3BIceSculptL"), { { P3B("IceSculpt"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3BPodiumR"), { { P3B("Podium"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3BPodiumL"), { { P3B("Podium"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3BPondHockeyR"), { { P3B("PondHockey"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3BPondHockeyL"), { { P3B("PondHockey"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3BRescueR"), { { P3B("Rescue"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3BRescueL"), { { P3B("Rescue"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3BSnowFortR"), { { P3B("SnowFort"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3BSnowFortL"), { { P3B("SnowFort"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3BWeddingR"), { { P3B("Wedding"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3BWeddingL"), { { P3B("Wedding"), 0, 0, 0, 30, 1 } });
Zone(TEXT("People p3b: scenes, slot 5"), 0.220f, 0.232f);
	Site(TEXT("PplP3BWeddingR"), 1, 51.0f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 7"), 0.311f, 0.323f);
	Site(TEXT("PplP3BPodiumR"), 1, 44.f, 1, 0, 1.f);
	Site(TEXT("PplP3BAlphornsL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 9"), 0.401f, 0.413f);
	Site(TEXT("PplP3BCurlingR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 11"), 0.492f, 0.504f);
	Site(TEXT("PplP3BAlphornsR"), 1, 44.f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceSculptL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 13"), 0.582f, 0.594f);
	Site(TEXT("PplP3BSnowFortL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 15"), 0.673f, 0.685f);
	Site(TEXT("PplP3BIceSculptR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 17"), 0.763f, 0.775f);
	Site(TEXT("PplP3BSnowFortR"), 1, 44.f, 1, 0, 1.f);
	Site(TEXT("PplP3BPondHockeyL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 19"), 0.854f, 0.866f);
	Site(TEXT("PplP3BPondHockeyR"), 1, 44.f, 1, 0, 1.f);
	Site(TEXT("PplP3BWeddingL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 21"), 0.944f, 0.956f);
	Site(TEXT("PplP3BWeddingR"), 1, 44.f, 1, 0, 1.f);
	Site(TEXT("PplP3BPodiumL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 23"), 1.035f, 1.047f);
	Site(TEXT("PplP3BPodiumR"), 1, 44.f, 1, 0, 1.f);
	Site(TEXT("PplP3BCurlingL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 25"), 1.125f, 1.137f);
	Site(TEXT("PplP3BCurlingR"), 1, 44.f, 1, 0, 1.f);
	Site(TEXT("PplP3BAlphornsL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 28"), 1.261f, 1.273f);
	Site(TEXT("PplP3BAlphornsR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 30"), 1.351f, 1.363f);
	Site(TEXT("PplP3BIceSculptL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 32"), 1.442f, 1.454f);
	Site(TEXT("PplP3BIceSculptR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 36"), 1.623f, 1.635f);
	Site(TEXT("PplP3BSnowFortR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 38"), 1.713f, 1.725f);
	Site(TEXT("PplP3BSnowFortL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 40"), 1.804f, 1.816f);
	Site(TEXT("PplP3BPondHockeyR"), 1, 44.f, 1, 0, 1.f);
	Site(TEXT("PplP3BPondHockeyL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 42"), 1.894f, 1.906f);
	Site(TEXT("PplP3BWeddingR"), 1, 44.f, 1, 0, 1.f);
	Site(TEXT("PplP3BWeddingL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 44"), 1.985f, 1.997f);
	Site(TEXT("PplP3BPodiumL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 60"), 2.709f, 2.721f);
	Site(TEXT("PplP3BBaseCampL"), -1, 51.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 62"), 2.799f, 2.811f);
	Site(TEXT("PplP3BIceCoreR"), 1, 51.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 64"), 2.890f, 2.902f);
	Site(TEXT("PplP3BRescueL"), -1, 51.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 89"), 4.021f, 4.033f);
	Site(TEXT("PplP3BRescueR"), 1, 51.f, 1, 0, 1.f);
	Site(TEXT("PplP3BBaseCampL"), -1, 51.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 97"), 4.383f, 4.395f);
	Site(TEXT("PplP3BSnowFortR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BAlphornsL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 99"), 4.473f, 4.485f);
	Site(TEXT("PplP3BIceCoreL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 101"), 4.564f, 4.576f);
	Site(TEXT("PplP3BRescueL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 103"), 4.654f, 4.666f);
	Site(TEXT("PplP3BBaseCampR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 105"), 4.745f, 4.757f);
	Site(TEXT("PplP3BAlphornsR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 120"), 5.423f, 5.435f);
	Site(TEXT("PplP3BIceCoreR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BSnowFortL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 123"), 5.559f, 5.571f);
	Site(TEXT("PplP3BRescueR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 121"), 5.468f, 5.480f);
	Site(TEXT("PplP3BBaseCampL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 125"), 5.649f, 5.661f);
	Site(TEXT("PplP3BWeddingR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 126"), 5.695f, 5.707f);
	Site(TEXT("PplP3BAlphornsL"), -1, 51.0f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 129"), 5.830f, 5.842f);
	Site(TEXT("PplP3BPodiumR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 130"), 5.876f, 5.888f);
	Site(TEXT("PplP3BIceSculptL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 132"), 5.966f, 5.978f);
	Site(TEXT("PplP3BSnowFortL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 134"), 6.057f, 6.069f);
	Site(TEXT("PplP3BCurlingR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 136"), 6.147f, 6.159f);
	Site(TEXT("PplP3BPondHockeyL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 138"), 6.238f, 6.250f);
	Site(TEXT("PplP3BAlphornsR"), 1, 44.f, 1, 0, 1.f);
	Site(TEXT("PplP3BWeddingL"), -1, 51.0f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 141"), 6.373f, 6.385f);
	Site(TEXT("PplP3BIceSculptR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 142"), 6.419f, 6.431f);
	Site(TEXT("PplP3BPodiumL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 144"), 6.509f, 6.521f);
	Site(TEXT("PplP3BCurlingL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 145"), 6.554f, 6.566f);
	Site(TEXT("PplP3BSnowFortR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 146"), 6.599f, 6.611f);
	Site(TEXT("PplP3BAlphornsL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 148"), 6.690f, 6.702f);
	Site(TEXT("PplP3BIceSculptL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 149"), 6.735f, 6.747f);
	Site(TEXT("PplP3BPondHockeyR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 150"), 6.780f, 6.792f);
	Site(TEXT("PplP3BSnowFortL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 159"), 7.188f, 7.200f);
	Site(TEXT("PplP3BWeddingR"), 1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 169"), 7.640f, 7.652f);
	Site(TEXT("PplP3BPodiumR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 170"), 7.685f, 7.697f);
	Site(TEXT("PplP3BPondHockeyL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 172"), 7.776f, 7.788f);
	Site(TEXT("PplP3BWeddingL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 173"), 7.821f, 7.833f);
	Site(TEXT("PplP3BCurlingR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 174"), 7.866f, 7.878f);
	Site(TEXT("PplP3BPodiumL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 186"), 8.409f, 8.421f);
	Site(TEXT("PplP3BAlphornsR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 188"), 8.500f, 8.512f);
	Site(TEXT("PplP3BCurlingL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 189"), 8.545f, 8.557f);
	Site(TEXT("PplP3BIceSculptR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 190"), 8.590f, 8.602f);
	Site(TEXT("PplP3BAlphornsL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 192"), 8.681f, 8.693f);
	Site(TEXT("PplP3BIceSculptL"), -1, 51.0f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 193"), 8.726f, 8.738f);
	Site(TEXT("PplP3BSnowFortR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 199"), 8.997f, 9.009f);
	Site(TEXT("PplP3BIceCoreR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BBaseCampL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 202"), 9.133f, 9.145f);
	Site(TEXT("PplP3BIceFishingR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 204"), 9.224f, 9.236f);
	Site(TEXT("PplP3BRescueL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 205"), 9.269f, 9.281f);
	Site(TEXT("PplP3BBaseCampR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceCoreL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 208"), 9.405f, 9.417f);
	Site(TEXT("PplP3BRescueR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 210"), 9.495f, 9.507f);
	Site(TEXT("PplP3BIceFishingL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 211"), 9.540f, 9.552f);
	Site(TEXT("PplP3BIceCoreR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BBaseCampL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 213"), 9.631f, 9.643f);
	Site(TEXT("PplP3BIceFishingR"), 1, 44.0f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 216"), 9.766f, 9.778f);
	Site(TEXT("PplP3BRescueL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 217"), 9.812f, 9.824f);
	Site(TEXT("PplP3BBaseCampR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceCoreL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 218"), 9.857f, 9.869f);
	Site(TEXT("PplP3BRescueR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 220"), 9.947f, 9.959f);
	Site(TEXT("PplP3BIceFishingL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 223"), 10.083f, 10.095f);
	Site(TEXT("PplP3BIceCoreR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BBaseCampL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 226"), 10.219f, 10.231f);
	Site(TEXT("PplP3BIceFishingR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 228"), 10.309f, 10.321f);
	Site(TEXT("PplP3BRescueL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 229"), 10.355f, 10.367f);
	Site(TEXT("PplP3BBaseCampR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceCoreL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 232"), 10.490f, 10.502f);
	Site(TEXT("PplP3BRescueR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 234"), 10.581f, 10.593f);
	Site(TEXT("PplP3BIceFishingL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 235"), 10.626f, 10.638f);
	Site(TEXT("PplP3BIceCoreR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BBaseCampL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 237"), 10.717f, 10.729f);
	Site(TEXT("PplP3BIceFishingR"), 1, 44.0f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 240"), 10.852f, 10.864f);
	Site(TEXT("PplP3BRescueL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 241"), 10.898f, 10.910f);
	Site(TEXT("PplP3BBaseCampR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceCoreL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 242"), 10.943f, 10.955f);
	Site(TEXT("PplP3BRescueR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 244"), 11.033f, 11.045f);
	Site(TEXT("PplP3BIceFishingL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 247"), 11.169f, 11.181f);
	Site(TEXT("PplP3BIceCoreR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BBaseCampL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 250"), 11.305f, 11.317f);
	Site(TEXT("PplP3BIceFishingR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 252"), 11.395f, 11.407f);
	Site(TEXT("PplP3BRescueL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 253"), 11.440f, 11.452f);
	Site(TEXT("PplP3BBaseCampR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceCoreL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 256"), 11.576f, 11.588f);
	Site(TEXT("PplP3BRescueR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 257"), 11.621f, 11.633f);
	Site(TEXT("PplP3BRescueR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BBaseCampL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 259"), 11.712f, 11.724f);
	Site(TEXT("PplP3BAlphornsL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 260"), 11.757f, 11.769f);
	Site(TEXT("PplP3BSnowFortR"), 1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 262"), 11.848f, 11.860f);
	Site(TEXT("PplP3BBaseCampR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceCoreL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 264"), 11.938f, 11.950f);
	Site(TEXT("PplP3BAlphornsR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BRescueL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 266"), 12.029f, 12.041f);
	Site(TEXT("PplP3BIceCoreR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BSnowFortL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 268"), 12.119f, 12.131f);
	Site(TEXT("PplP3BRescueR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BBaseCampL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 270"), 12.210f, 12.222f);
	Site(TEXT("PplP3BSnowFortR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BAlphornsL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 272"), 12.300f, 12.312f);
	Site(TEXT("PplP3BBaseCampR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceCoreL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 274"), 12.391f, 12.403f);
	Site(TEXT("PplP3BAlphornsR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BRescueL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 276"), 12.481f, 12.493f);
	Site(TEXT("PplP3BIceCoreR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BSnowFortL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 278"), 12.572f, 12.584f);
	Site(TEXT("PplP3BRescueR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BBaseCampL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 280"), 12.662f, 12.674f);
	Site(TEXT("PplP3BSnowFortR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BAlphornsL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 282"), 12.753f, 12.765f);
	Site(TEXT("PplP3BBaseCampR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceCoreL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 284"), 12.843f, 12.855f);
	Site(TEXT("PplP3BAlphornsR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BRescueL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 286"), 12.933f, 12.945f);
	Site(TEXT("PplP3BIceCoreR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BSnowFortL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 288"), 13.024f, 13.036f);
	Site(TEXT("PplP3BRescueR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BBaseCampL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 290"), 13.114f, 13.126f);
	Site(TEXT("PplP3BSnowFortR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BAlphornsL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 292"), 13.205f, 13.217f);
	Site(TEXT("PplP3BBaseCampR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceCoreL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 294"), 13.295f, 13.307f);
	Site(TEXT("PplP3BAlphornsR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BRescueL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 296"), 13.386f, 13.398f);
	Site(TEXT("PplP3BIceCoreR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BSnowFortL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 298"), 13.476f, 13.488f);
	Site(TEXT("PplP3BRescueR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BBaseCampL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 300"), 13.567f, 13.579f);
	Site(TEXT("PplP3BSnowFortR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BAlphornsL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 302"), 13.657f, 13.669f);
	Site(TEXT("PplP3BBaseCampR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceCoreL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 304"), 13.748f, 13.760f);
	Site(TEXT("PplP3BAlphornsR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BRescueL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 306"), 13.838f, 13.850f);
	Site(TEXT("PplP3BIceCoreR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BSnowFortL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 308"), 13.929f, 13.941f);
	Site(TEXT("PplP3BRescueR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BBaseCampL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 310"), 14.019f, 14.031f);
	Site(TEXT("PplP3BSnowFortR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BAlphornsL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 312"), 14.110f, 14.122f);
	Site(TEXT("PplP3BBaseCampR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceCoreL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 314"), 14.200f, 14.212f);
	Site(TEXT("PplP3BAlphornsR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BRescueL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 316"), 14.291f, 14.303f);
	Site(TEXT("PplP3BIceCoreR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BSnowFortL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 319"), 14.427f, 14.439f);
	Site(TEXT("PplP3BAlphornsL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 320"), 14.472f, 14.484f);
	Site(TEXT("PplP3BWeddingR"), 1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 321"), 14.517f, 14.529f);
	Site(TEXT("PplP3BIceSculptL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 323"), 14.607f, 14.619f);
	Site(TEXT("PplP3BSnowFortL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 325"), 14.698f, 14.710f);
	Site(TEXT("PplP3BPondHockeyL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 326"), 14.743f, 14.755f);
	Site(TEXT("PplP3BPodiumR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 327"), 14.788f, 14.800f);
	Site(TEXT("PplP3BWeddingL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 329"), 14.879f, 14.891f);
	Site(TEXT("PplP3BCurlingR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BPodiumL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 333"), 15.060f, 15.072f);
	Site(TEXT("PplP3BCurlingL"), -1, 51.0f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 334"), 15.105f, 15.117f);
	Site(TEXT("PplP3BAlphornsR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 335"), 15.150f, 15.162f);
	Site(TEXT("PplP3BAlphornsL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 337"), 15.241f, 15.253f);
	Site(TEXT("PplP3BIceSculptL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 338"), 15.286f, 15.298f);
	Site(TEXT("PplP3BIceSculptR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 341"), 15.422f, 15.434f);
	Site(TEXT("PplP3BSnowFortL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 343"), 15.512f, 15.524f);
	Site(TEXT("PplP3BPondHockeyL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 344"), 15.558f, 15.570f);
	Site(TEXT("PplP3BSnowFortR"), 1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 345"), 15.603f, 15.615f);
	Site(TEXT("PplP3BWeddingL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 347"), 15.693f, 15.705f);
	Site(TEXT("PplP3BPodiumL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 349"), 15.784f, 15.796f);
	Site(TEXT("PplP3BPondHockeyR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BCurlingL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 351"), 15.874f, 15.886f);
	Site(TEXT("PplP3BAlphornsL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 353"), 15.965f, 15.977f);
	Site(TEXT("PplP3BWeddingR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceSculptL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 355"), 16.055f, 16.067f);
	Site(TEXT("PplP3BSnowFortL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 359"), 16.236f, 16.248f);
	Site(TEXT("PplP3BPondHockeyL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 361"), 16.327f, 16.339f);
	Site(TEXT("PplP3BWeddingL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 362"), 16.372f, 16.384f);
	Site(TEXT("PplP3BPodiumR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 363"), 16.417f, 16.429f);
	Site(TEXT("PplP3BPodiumL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 365"), 16.508f, 16.520f);

	Site(TEXT("PplP3BCurlingL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 368"), 16.643f, 16.655f);
	Site(TEXT("PplP3BAlphornsR"), 1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 369"), 16.689f, 16.701f);
	Site(TEXT("PplP3BAlphornsL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 370"), 16.734f, 16.746f);
	Site(TEXT("PplP3BIceSculptR"), 1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 371"), 16.779f, 16.791f);
	Site(TEXT("PplP3BIceSculptL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 373"), 16.870f, 16.882f);
	Site(TEXT("PplP3BSnowFortL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 374"), 16.915f, 16.927f);
	Site(TEXT("PplP3BSnowFortR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 376"), 17.005f, 17.017f);
	Site(TEXT("PplP3BBaseCampL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 378"), 17.096f, 17.108f);
	Site(TEXT("PplP3BIceCoreR"), 1, 44.f, 1, 0, 1.f);
	Site(TEXT("PplP3BRescueL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 380"), 17.186f, 17.198f);
	Site(TEXT("PplP3BIceFishingR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 383"), 17.322f, 17.334f);
	Site(TEXT("PplP3BIceCoreL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 384"), 17.367f, 17.379f);
	Site(TEXT("PplP3BBaseCampR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceFishingL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 387"), 17.503f, 17.515f);
	Site(TEXT("PplP3BRescueR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 389"), 17.594f, 17.606f);
	Site(TEXT("PplP3BBaseCampL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 390"), 17.639f, 17.651f);
	Site(TEXT("PplP3BIceCoreR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BRescueL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 393"), 17.774f, 17.786f);
	Site(TEXT("PplP3BIceFishingR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 395"), 17.865f, 17.877f);
	Site(TEXT("PplP3BIceCoreL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 396"), 17.910f, 17.922f);
	Site(TEXT("PplP3BBaseCampR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceFishingL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 399"), 18.046f, 18.058f);
	Site(TEXT("PplP3BRescueR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 400"), 18.091f, 18.103f);
	Site(TEXT("PplP3BBaseCampL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 402"), 18.182f, 18.194f);
	Site(TEXT("PplP3BIceCoreR"), 1, 44.f, 1, 0, 1.f);
	Site(TEXT("PplP3BRescueL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 404"), 18.272f, 18.284f);
	Site(TEXT("PplP3BIceFishingR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 406"), 18.363f, 18.375f);
	Site(TEXT("PplP3BIceCoreL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 408"), 18.453f, 18.465f);
	Site(TEXT("PplP3BBaseCampR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceFishingL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 411"), 18.589f, 18.601f);
	Site(TEXT("PplP3BRescueR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 413"), 18.679f, 18.691f);
	Site(TEXT("PplP3BBaseCampL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 414"), 18.725f, 18.737f);
	Site(TEXT("PplP3BIceCoreR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BRescueL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 417"), 18.860f, 18.872f);
	Site(TEXT("PplP3BIceFishingR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 419"), 18.951f, 18.963f);
	Site(TEXT("PplP3BIceCoreL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 420"), 18.996f, 19.008f);
	Site(TEXT("PplP3BBaseCampR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceFishingL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 423"), 19.132f, 19.144f);
	Site(TEXT("PplP3BRescueR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 424"), 19.177f, 19.189f);
	Site(TEXT("PplP3BBaseCampL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 426"), 19.268f, 19.280f);
	Site(TEXT("PplP3BIceCoreR"), 1, 44.f, 1, 0, 1.f);
	Site(TEXT("PplP3BRescueL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 428"), 19.358f, 19.370f);
	Site(TEXT("PplP3BIceFishingR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 430"), 19.448f, 19.460f);
	Site(TEXT("PplP3BIceCoreL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 432"), 19.539f, 19.551f);
	Site(TEXT("PplP3BBaseCampR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceFishingL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 435"), 19.675f, 19.687f);
	Site(TEXT("PplP3BRescueR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 436"), 19.720f, 19.732f);
	Site(TEXT("PplP3BBaseCampL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 437"), 19.765f, 19.777f);
	Site(TEXT("PplP3BRescueR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 439"), 19.856f, 19.868f);
	Site(TEXT("PplP3BAlphornsL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 440"), 19.901f, 19.913f);
	Site(TEXT("PplP3BSnowFortR"), 1, 51.0f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 441"), 19.946f, 19.958f);
	Site(TEXT("PplP3BIceCoreL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 442"), 19.991f, 20.003f);
	Site(TEXT("PplP3BBaseCampR"), 1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 444"), 20.082f, 20.094f);
	Site(TEXT("PplP3BAlphornsR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BRescueL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 446"), 20.172f, 20.184f);
	Site(TEXT("PplP3BIceCoreR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BSnowFortL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 448"), 20.263f, 20.275f);
	Site(TEXT("PplP3BRescueR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BBaseCampL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 450"), 20.353f, 20.365f);
	Site(TEXT("PplP3BSnowFortR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 451"), 20.399f, 20.411f);
	Site(TEXT("PplP3BAlphornsL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 453"), 20.489f, 20.501f);
	Site(TEXT("PplP3BBaseCampR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 452"), 20.444f, 20.456f);
	Site(TEXT("PplP3BIceCoreL"), -1, 57.0f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 455"), 20.580f, 20.592f);
	Site(TEXT("PplP3BAlphornsR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 457"), 20.670f, 20.682f);
	Site(TEXT("PplP3BRescueL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 458"), 20.715f, 20.727f);
	Site(TEXT("PplP3BIceCoreR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 460"), 20.806f, 20.818f);
	Site(TEXT("PplP3BRescueR"), 1, 51.0f, 1, 0, 1.f);
	Site(TEXT("PplP3BSnowFortL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 462"), 20.896f, 20.908f);
	Site(TEXT("PplP3BSnowFortR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BBaseCampL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 463"), 20.942f, 20.954f);
	Site(TEXT("PplP3BBaseCampR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 465"), 21.032f, 21.044f);
	Site(TEXT("PplP3BAlphornsL"), -1, 51.0f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 467"), 21.122f, 21.134f);
	Site(TEXT("PplP3BAlphornsR"), 1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 468"), 21.168f, 21.180f);
	Site(TEXT("PplP3BIceCoreL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 469"), 21.213f, 21.225f);
	Site(TEXT("PplP3BIceCoreR"), 1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 470"), 21.258f, 21.270f);
	Site(TEXT("PplP3BRescueL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 472"), 21.349f, 21.361f);
	Site(TEXT("PplP3BRescueR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BSnowFortL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 475"), 21.484f, 21.496f);
	Site(TEXT("PplP3BSnowFortR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 475"), 21.484f, 21.496f);
	Site(TEXT("PplP3BBaseCampL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 477"), 21.575f, 21.587f);
	Site(TEXT("PplP3BAlphornsL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 477"), 21.575f, 21.587f);
	Site(TEXT("PplP3BBaseCampR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 480"), 21.711f, 21.723f);
	Site(TEXT("PplP3BAlphornsR"), 1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: fix round, slot 479"), 21.665f, 21.677f);
	Site(TEXT("PplP3BIceCoreL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 482"), 21.801f, 21.813f);
	Site(TEXT("PplP3BIceCoreR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BRescueL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 484"), 21.892f, 21.904f);
	Site(TEXT("PplP3BRescueR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 485"), 21.937f, 21.949f);
	Site(TEXT("PplP3BSnowFortL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 487"), 22.027f, 22.039f);
	Site(TEXT("PplP3BSnowFortR"), 1, 51.0f, 1, 0, 1.f);
	Site(TEXT("PplP3BBaseCampL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 489"), 22.118f, 22.130f);
	Site(TEXT("PplP3BAlphornsL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 490"), 22.163f, 22.175f);
	Site(TEXT("PplP3BBaseCampR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 492"), 22.254f, 22.266f);
	Site(TEXT("PplP3BAlphornsR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BIceCoreL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 494"), 22.344f, 22.356f);
	Site(TEXT("PplP3BIceCoreR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BRescueL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 496"), 22.435f, 22.447f);
	Site(TEXT("PplP3BWeddingR"), 1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 497"), 22.480f, 22.492f);
	Site(TEXT("PplP3BAlphornsL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 499"), 22.570f, 22.582f);
	Site(TEXT("PplP3BIceSculptL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 502"), 22.706f, 22.718f);
	Site(TEXT("PplP3BPodiumR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 503"), 22.751f, 22.763f);
	Site(TEXT("PplP3BSnowFortL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 505"), 22.842f, 22.854f);
	Site(TEXT("PplP3BPondHockeyL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 506"), 22.887f, 22.899f);
	Site(TEXT("PplP3BCurlingR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 507"), 22.932f, 22.944f);
	Site(TEXT("PplP3BWeddingL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 509"), 23.023f, 23.035f);
	Site(TEXT("PplP3BAlphornsR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BPodiumL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 513"), 23.204f, 23.216f);
	Site(TEXT("PplP3BCurlingL"), -1, 51.0f, 1, 0, 1.f);

Zone(TEXT("People p3b: scenes, slot 515"), 23.294f, 23.306f);
	Site(TEXT("PplP3BAlphornsL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 517"), 23.385f, 23.397f);
	Site(TEXT("PplP3BIceSculptL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 518"), 23.430f, 23.442f);
	Site(TEXT("PplP3BSnowFortR"), 1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 519"), 23.475f, 23.487f);
	Site(TEXT("PplP3BSnowFortL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 521"), 23.566f, 23.578f);
	Site(TEXT("PplP3BPondHockeyL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 523"), 23.656f, 23.668f);
	Site(TEXT("PplP3BWeddingL"), -1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 524"), 23.701f, 23.713f);
	Site(TEXT("PplP3BPondHockeyR"), 1, 31.5f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 525"), 23.747f, 23.759f);
	Site(TEXT("PplP3BPodiumL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 527"), 23.837f, 23.849f);
	Site(TEXT("PplP3BCurlingL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 529"), 23.928f, 23.940f);
	Site(TEXT("PplP3BWeddingR"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("PplP3BAlphornsL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 531"), 24.018f, 24.030f);
	Site(TEXT("PplP3BIceSculptL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 533"), 24.109f, 24.121f);

	Site(TEXT("PplP3BSnowFortL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 535"), 24.199f, 24.211f);
	Site(TEXT("PplP3BPondHockeyL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 539"), 24.380f, 24.392f);
	Site(TEXT("PplP3BWeddingL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 541"), 24.470f, 24.482f);
	Site(TEXT("PplP3BPodiumL"), -1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 542"), 24.516f, 24.528f);
	Site(TEXT("PplP3BCurlingR"), 1, 38.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 543"), 24.561f, 24.573f);
	Site(TEXT("PplP3BCurlingL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes, slot 545"), 24.651f, 24.663f);
	Site(TEXT("PplP3BAlphornsR"), 1, 31.5f, 1, 0, 1.f);
	Site(TEXT("PplP3BAlphornsL"), -1, 44.f, 1, 0, 1.f);
Zone(TEXT("People p3b: scenes under the chutes corkscrew"), 2.510f, 2.685f);
	Far(P3B("IceCore"), 57.f, 58.f, 2, 0, 1.f, 1.f, 0.f, 1, 0.7f, 0.f);
	Far(P3B("Rescue"), 57.f, 58.f, 2, 1, 1.f, 1.f, 0.f, -1, 0.7f, 0.f);
Zone(TEXT("People p3b: scenes under Black diamonds loop"), 4.085f, 4.370f);
	Far(P3B("Rescue"), 57.f, 58.f, 2, 0, 1.f, 1.f, 0.f, 1, 0.7f, 0.f);
	Far(P3B("BaseCamp"), 57.f, 58.f, 2, 1, 1.f, 1.f, 0.f, -1, 0.7f, 0.f);
Zone(TEXT("People p3b: scenes under Black diamonds corkscrew"), 5.225f, 5.400f);
	Far(P3B("SnowFort"), 57.f, 58.f, 2, 0, 1.f, 1.f, 0.f, 1, 0.7f, 0.f);
	Far(P3B("Alphorns"), 57.f, 58.f, 2, 1, 1.f, 1.f, 0.f, -1, 0.7f, 0.f);
Zone(TEXT("People p3b: scenes under the whiteouts loop"), 6.870f, 7.155f);
	Far(P3B("Podium"), 57.f, 58.f, 2, 0, 1.f, 1.f, 0.f, 1, 0.7f, 0.f);
	Far(P3B("Curling"), 57.f, 58.f, 2, 1, 1.f, 1.f, 0.f, -1, 0.7f, 0.f);
Zone(TEXT("People p3b: scenes under the valleys corkscrew"), 7.215f, 7.385f);
	Far(P3B("Wedding"), 57.f, 58.f, 2, 0, 1.f, 1.f, 0.f, 1, 0.7f, 0.f);
	Far(P3B("PondHockey"), 57.f, 58.f, 2, 1, 1.f, 1.f, 0.f, -1, 0.7f, 0.f);
#undef P3B
// ---- END p3b ----
}

{
// ---- BEGIN p3c (movers, ft_people_ice_p3c.py) ----
// (Fix round, 16 Nov 2026: after two reviews rules of this block were moved, re-phased or taken out; the counts in this header are the
// builder's. What changed and why: SourceArt/Integration/people/L5_ice_p3.md, "Review and fix round".)
// Everything that TRAVELS, on lines of its own that lie ALONG the road (every ground piece is laid by a one-piece set at yaw 0, so its
// rule never turns it towards the road) and are 28 to 34 m long as built: inside their own 45 m of road. The hill falls 7 in 100 along
// the road, so each line runs on the level crest of a snow bank of the piece's own, footed 3 m (placed at 1.5: 4.5 m) into the ground.
// Thirteen meshes; the log calls every one of them TALL (tops 4.4 to 25 m), so nothing here is nearer than 38.5 m:
//   38.5 m, only inside the stretches where the dressing writes its own verge (no banked bend), at scale 1: SM_P3C_SkiTrail_A / _B
//     (cross-country skiers, 30 m lines both ways), SM_P3C_Snowshoe_A (a guided party, 28 m), SM_P3C_FatBikes_A (fat bikes, 32 m).
//   64 m, at 1.2: the same four again, SM_P3C_Herd_A (seven reindeer and their herder, 32 m), SM_P3C_Toboggan_A on the right and
//     SM_P3C_TobogganL_A on the left (sledders down a 5 m hill of the piece's own; top 9.9 m as built), and at 67 m and scale 1
//     SM_P3C_Skaters_A (a round rink 18 m across on a plinth of snow, eight skaters in rings).
//   77 m, the edge of the level snow, at 1.25: SM_P3C_Skidoos_A / _B (snowmobile convoys, 34 m), SM_P3C_Sleigh_A (two reindeer, a
//     sleigh, a skier on a rope, 32 m), SM_P3C_Kites_A (two snowkiters, kites 13 m up as built), the toboggan hills.
//   110 m, on the slope, at 1.5: the herd, the kiters and the sleigh (the herd and the kiters use the always-moving master).
//   The sky, 80 to 130 m out: SM_P3C_Geese_A (nine geese in a V, 24 m up, a 120 m line along the road; always-moving master).
// The slots. A rule fires on the slots s with (s + phase) % every == 0, whatever its zone. A scratch tool read FTDressL5.inl and
// FTPeopleL5.inl and gave each rule a phase on which, in its zone, no slot has a piece of theirs on that side in its band of distance
// (stands, decks, climbs and watchers by the road; lots, rinks, depots, lift lines, ice walls, the piste runs, the rack railway and
// the groomers' strips further out), none is at a jump (2.1 to 2.5, 4.83 to 5.21, 7.99 to 8.375 km: the land dips), the start or a
// gondola, and none is nearer than 4 to 6 slots to this block's next piece in the same band and side. Loops and corkscrews: the track
// leaves a Site out itself. The level's own pines and the other pass 3 builders' pieces were NOT read (not checked).
#define P3C(Name) TEXT("/Game/People/IceMountain/SM_P3C_" Name)
Set(TEXT("PplP3CFatBikesA"), { { P3C("FatBikes_A"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3CHerdA"), { { P3C("Herd_A"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3CKitesA"), { { P3C("Kites_A"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3CSkatersA"), { { P3C("Skaters_A"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3CSkiTrailA"), { { P3C("SkiTrail_A"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3CSkiTrailB"), { { P3C("SkiTrail_B"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3CSkidoosA"), { { P3C("Skidoos_A"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3CSkidoosB"), { { P3C("Skidoos_B"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3CSleighA"), { { P3C("Sleigh_A"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3CSnowshoeA"), { { P3C("Snowshoe_A"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3CTobogganL"), { { P3C("TobogganL_A"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3CTobogganR"), { { P3C("Toboggan_A"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Zone(TEXT("People p3c: First tracks"), 0.f, 1.19f);
	Site(TEXT("PplP3CFatBikesA"), 1, 64.f, 30, 23, 1.f, 1.2f);     // fat bikes, right: 1
	Site(TEXT("PplP3CSkatersA"), 1, 67.f, 30, 17, 1.f);     // skaters on a rink, right: 1
	Site(TEXT("PplP3CTobogganR"), 1, 64.f, 30, 9, 1.f, 1.2f);     // a toboggan hill, right: 1
	Site(TEXT("PplP3CSkiTrailA"), 1, 64.f, 30, 5, 1.f, 1.2f);     // cross-country skiers, right: 1
	Site(TEXT("PplP3CFatBikesA"), -1, 64.f, 30, 16, 1.f, 1.2f);     // fat bikes, left: 1
	Site(TEXT("PplP3CSkatersA"), -1, 67.f, 30, 8, 1.f);     // skaters on a rink, left: 1
	Site(TEXT("PplP3CTobogganL"), -1, 64.f, 30, 22, 1.f, 1.2f);     // a toboggan hill, left: 1
	Site(TEXT("PplP3CSleighA"), 1, 77.f, 20, 8, 1.f, 1.25f);     // a reindeer sleigh, right: 1
	Site(TEXT("PplP3CSkidoosB"), 1, 77.f, 25, 4, 1.f, 1.25f);     // the patrol on snowmobiles, right: 1
	Site(TEXT("PplP3CSleighA"), -1, 77.f, 20, 5, 1.f, 1.25f);     // a reindeer sleigh, left: 1
	Site(TEXT("PplP3CSkidoosB"), -1, 77.f, 25, 2, 1.f, 1.25f);     // the patrol on snowmobiles, left: 1
	Site(TEXT("PplP3CTobogganL"), -1, 77.f, 20, 13, 1.f, 1.25f);     // a toboggan hill, left: 1
	Site(TEXT("PplP3CKitesA"), 1, 110.f, 18, 9, 1.f, 1.5f);     // snowkiters, right: 1
	Site(TEXT("PplP3CKitesA"), -1, 110.f, 18, 10, 1.f, 1.5f);     // snowkiters, left: 2
	Site(TEXT("PplP3CSleighA"), -1, 110.f, 18, 1, 1.f, 1.5f);     // a reindeer sleigh, left: 1
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 0, 1.f, 1.f, 0.f, 1, 0.f);     // a skein of geese along the road, right: 1
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 11, 1.f, 1.f, 0.f, -1, 0.f);     // a skein of geese along the road, left: 1
Zone(TEXT("People p3c: The half pipe"), 1.19f, 2.43f);
	Site(TEXT("PplP3CSkatersA"), 1, 67.f, 30, 0, 1.f);     // skaters on a rink, right: 1
	Site(TEXT("PplP3CTobogganR"), 1, 64.f, 30, 23, 1.f, 1.2f);     // a toboggan hill, right: 1
	Site(TEXT("PplP3CSkiTrailA"), 1, 64.f, 30, 18, 1.f, 1.2f);     // cross-country skiers, right: 1
	Site(TEXT("PplP3CSkatersA"), -1, 67.f, 30, 27, 1.f);     // skaters on a rink, left: 1
	Site(TEXT("PplP3CTobogganL"), -1, 64.f, 30, 22, 1.f, 1.2f);     // a toboggan hill, left: 1
	Site(TEXT("PplP3CSkiTrailA"), -1, 64.f, 30, 16, 1.f, 1.2f);     // cross-country skiers, left: 1
	Site(TEXT("PplP3CSkatersA"), -1, 67.f, 30, 0, 1.f);     // skaters on a rink, left: 1
	Site(TEXT("PplP3CSkidoosB"), 1, 77.f, 20, 0, 1.f, 1.25f);     // the patrol on snowmobiles, right: 1
	Site(TEXT("PplP3CTobogganR"), 1, 77.f, 20, 6, 1.f, 1.25f);     // a toboggan hill, right: 1
	Site(TEXT("PplP3CSkidoosA"), 1, 77.f, 25, 21, 1.f, 1.25f);     // a snowmobile convoy, right: 1
	Site(TEXT("PplP3CSkidoosB"), -1, 77.f, 20, 19, 1.f, 1.25f);     // the patrol on snowmobiles, left: 1
	Site(TEXT("PplP3CTobogganL"), -1, 77.f, 20, 5, 1.f, 1.25f);     // a toboggan hill, left: 1
	Site(TEXT("PplP3CSkidoosA"), -1, 77.f, 25, 21, 1.f, 1.25f);     // a snowmobile convoy, left: 1
	Site(TEXT("PplP3CSleighA"), 1, 110.f, 18, 12, 1.f, 1.5f);     // a reindeer sleigh, right: 1
	Site(TEXT("PplP3CKitesA"), 1, 110.f, 24, 14, 1.f, 1.5f);     // snowkiters, right: 1
	Site(TEXT("PplP3CSleighA"), -1, 110.f, 18, 11, 1.f, 1.5f);     // a reindeer sleigh, left: 1
	Site(TEXT("PplP3CKitesA"), -1, 110.f, 18, 17, 1.f, 1.5f);     // snowkiters, left: 1
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 17, 1.f, 1.f, 0.f, 1, 0.f);     // a skein of geese along the road, right: 1
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 6, 1.f, 1.f, 0.f, -1, 0.f);     // a skein of geese along the road, left: 1
Zone(TEXT("People p3c: Avalanche chute"), 2.43f, 2.96f);
	Site(TEXT("PplP3CSkiTrailB"), 1, 64.f, 15, 14, 1.f, 1.2f);     // skating skiers and a pulk, right: 1
	Site(TEXT("PplP3CSkiTrailB"), -1, 64.f, 15, 13, 1.f, 1.2f);     // skating skiers and a pulk, left: 1
	Site(TEXT("PplP3CKitesA"), 1, 77.f, 15, 11, 1.f, 1.25f);     // snowkiters, right: 1
	Site(TEXT("PplP3CKitesA"), -1, 77.f, 15, 0, 1.f, 1.25f);     // snowkiters, left: 1
	Site(TEXT("PplP3CSkidoosA"), -1, 77.f, 15, 10, 1.f, 1.25f);     // a snowmobile convoy, left: 1
	Site(TEXT("PplP3CHerdA"), 1, 110.f, 12, 0, 1.f, 1.5f);     // a reindeer herd, right: 1
	Site(TEXT("PplP3CHerdA"), -1, 110.f, 12, 11, 1.f, 1.5f);     // a reindeer herd, left: 1
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 1, 1.f, 1.f, 0.f, -1, 0.f);     // a skein of geese along the road, left: 1
Zone(TEXT("People p3c: The super pipe"), 2.96f, 4.02f);
	Site(TEXT("PplP3CSnowshoeA"), 1, 64.f, 15, 9, 1.f, 1.2f);     // a snowshoe party, right: 2
	Site(TEXT("PplP3CHerdA"), 1, 64.f, 15, 4, 1.f, 1.2f);     // a reindeer herd, right: 2
	Site(TEXT("PplP3CSkiTrailB"), 1, 64.f, 15, 14, 1.f, 1.2f);     // skating skiers and a pulk, right: 1
	Site(TEXT("PplP3CSnowshoeA"), -1, 64.f, 15, 8, 1.f, 1.2f);     // a snowshoe party, left: 2
	Site(TEXT("PplP3CHerdA"), -1, 64.f, 15, 3, 1.f, 1.2f);     // a reindeer herd, left: 2
	Site(TEXT("PplP3CSkidoosA"), 1, 77.f, 15, 7, 1.f, 1.25f);     // a snowmobile convoy, right: 2
	Site(TEXT("PplP3CSkidoosB"), 1, 77.f, 15, 2, 1.f, 1.25f);     // the patrol on snowmobiles, right: 2
	Site(TEXT("PplP3CSkidoosA"), -1, 77.f, 15, 6, 1.f, 1.25f);     // a snowmobile convoy, left: 2
	Site(TEXT("PplP3CSkidoosB"), -1, 77.f, 15, 0, 1.f, 1.25f);     // the patrol on snowmobiles, left: 1
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 7, 1.f, 1.f, 0.f, 1, 0.f);     // a skein of geese along the road, right: 1
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 18, 1.f, 1.f, 0.f, -1, 0.f);     // a skein of geese along the road, left: 1
Zone(TEXT("People p3c: Black diamond"), 4.02f, 5.65f);
	Site(TEXT("PplP3CFatBikesA"), 1, 64.f, 20, 19, 1.f, 1.2f);     // fat bikes, right: 2
	Site(TEXT("PplP3CSkiTrailB"), 1, 64.f, 20, 15, 1.f, 1.2f);     // skating skiers and a pulk, right: 1
	Site(TEXT("PplP3CSnowshoeA"), 1, 64.f, 40, 31, 1.f, 1.2f);     // a snowshoe party, right: 1
	Site(TEXT("PplP3CFatBikesA"), -1, 64.f, 20, 18, 1.f, 1.2f);     // fat bikes, left: 2
	Site(TEXT("PplP3CSkiTrailB"), -1, 64.f, 40, 30, 1.f, 1.2f);     // skating skiers and a pulk, left: 1
	Site(TEXT("PplP3CSleighA"), 1, 77.f, 20, 16, 1.f, 1.25f);     // a reindeer sleigh, right: 2
	Site(TEXT("PplP3CSkidoosA"), 1, 77.f, 20, 2, 1.f, 1.25f);     // a snowmobile convoy, right: 1
	Site(TEXT("PplP3CSleighA"), -1, 77.f, 15, 0, 1.f, 1.25f);     // a reindeer sleigh, left: 2
	Site(TEXT("PplP3CSkidoosA"), -1, 77.f, 20, 1, 1.f, 1.25f);     // a snowmobile convoy, left: 1
	Site(TEXT("PplP3CKitesA"), 1, 110.f, 24, 17, 1.f, 1.5f);     // snowkiters, right: 1
	Site(TEXT("PplP3CKitesA"), -1, 110.f, 12, 2, 1.f, 1.5f);     // snowkiters, left: 1
	Site(TEXT("PplP3CHerdA"), -1, 110.f, 18, 9, 1.f, 1.5f);     // a reindeer herd, left: 1
Zone(TEXT("People p3c: Black diamond, the verge from 4.05 km"), 4.05f, 4.44f);
	Site(TEXT("PplP3CSnowshoeA"), 1, 38.5f, 12, 10, 1.f);     // a snowshoe party, right: 1
Zone(TEXT("People p3c: The whiteout"), 5.65f, 7.3f);
	Site(TEXT("PplP3CTobogganR"), 1, 64.f, 30, 21, 1.f, 1.2f);     // a toboggan hill, right: 2
	Site(TEXT("PplP3CFatBikesA"), 1, 64.f, 30, 29, 1.f, 1.2f);     // fat bikes, right: 1
	Site(TEXT("PplP3CSkatersA"), 1, 67.f, 30, 15, 1.f);     // skaters on a rink, right: 1
	Site(TEXT("PplP3CTobogganR"), 1, 64.f, 30, 11, 1.f, 1.2f);     // a toboggan hill, right: 1
	Site(TEXT("PplP3CSkiTrailA"), 1, 64.f, 30, 5, 1.f, 1.2f);     // cross-country skiers, right: 1
	Site(TEXT("PplP3CTobogganL"), -1, 64.f, 30, 22, 1.f, 1.2f);     // a toboggan hill, left: 2
	Site(TEXT("PplP3CFatBikesA"), -1, 64.f, 30, 28, 1.f, 1.2f);     // fat bikes, left: 1
	Site(TEXT("PplP3CSkatersA"), -1, 67.f, 30, 14, 1.f);     // skaters on a rink, left: 1
	Site(TEXT("PplP3CTobogganL"), -1, 64.f, 30, 10, 1.f, 1.2f);     // a toboggan hill, left: 1
	Site(TEXT("PplP3CSkiTrailA"), -1, 64.f, 30, 4, 1.f, 1.2f);     // cross-country skiers, left: 1
	Site(TEXT("PplP3CSkidoosB"), 1, 77.f, 20, 13, 1.f, 1.25f);     // the patrol on snowmobiles, right: 2
	Site(TEXT("PplP3CTobogganR"), 1, 77.f, 20, 1, 1.f, 1.25f);     // a toboggan hill, right: 2
	Site(TEXT("PplP3CSkidoosA"), 1, 77.f, 20, 8, 1.f, 1.25f);     // a snowmobile convoy, right: 1
	Site(TEXT("PplP3CSkidoosB"), -1, 77.f, 20, 12, 1.f, 1.25f);     // the patrol on snowmobiles, left: 2
	Site(TEXT("PplP3CTobogganL"), -1, 77.f, 25, 16, 1.f, 1.25f);     // a toboggan hill, left: 2
	Site(TEXT("PplP3CSkidoosA"), -1, 77.f, 20, 17, 1.f, 1.25f);     // a snowmobile convoy, left: 1
	Site(TEXT("PplP3CSleighA"), 1, 110.f, 12, 3, 1.f, 1.5f);     // a reindeer sleigh, right: 2
	Site(TEXT("PplP3CKitesA"), 1, 110.f, 24, 21, 1.f, 1.5f);     // snowkiters, right: 1
	Site(TEXT("PplP3CSleighA"), -1, 110.f, 18, 4, 1.f, 1.5f);     // a reindeer sleigh, left: 1
	Site(TEXT("PplP3CKitesA"), -1, 110.f, 24, 20, 1.f, 1.5f);     // snowkiters, left: 1
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 19, 1.f, 1.f, 0.f, 1, 0.f);     // a skein of geese along the road, right: 1
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 8, 1.f, 1.f, 0.f, -1, 0.f);     // a skein of geese along the road, left: 1
Zone(TEXT("People p3c: The whiteout, the verge from 5.68 km"), 5.68f, 6.31f);
	Site(TEXT("PplP3CSkiTrailB"), 1, 38.5f, 12, 11, 1.f);     // skating skiers and a pulk, right: 1
	Site(TEXT("PplP3CSkiTrailB"), -1, 38.5f, 12, 6, 1.f);     // skating skiers and a pulk, left: 2
	Site(TEXT("PplP3CSkiTrailA"), -1, 38.5f, 12, 0, 1.f);     // cross-country skiers, left: 1
Zone(TEXT("People p3c: The valley run"), 7.3f, 8.94f);
	Site(TEXT("PplP3CFatBikesA"), 1, 64.f, 30, 13, 1.f, 1.2f);     // fat bikes, right: 2
	Site(TEXT("PplP3CSkatersA"), 1, 67.f, 30, 21, 1.f);     // skaters on a rink, right: 1
	Site(TEXT("PplP3CTobogganR"), 1, 64.f, 30, 9, 1.f, 1.2f);     // a toboggan hill, right: 1
	Site(TEXT("PplP3CFatBikesA"), -1, 64.f, 30, 14, 1.f, 1.2f);     // fat bikes, left: 2
	Site(TEXT("PplP3CSkatersA"), -1, 67.f, 30, 4, 1.f);     // skaters on a rink, left: 1
	Site(TEXT("PplP3CTobogganL"), -1, 64.f, 30, 8, 1.f, 1.2f);     // a toboggan hill, left: 1
	Site(TEXT("PplP3CTobogganR"), 1, 77.f, 20, 9, 1.f, 1.25f);     // a toboggan hill, right: 2
	Site(TEXT("PplP3CSkidoosA"), 1, 77.f, 40, 35, 1.f, 1.25f);     // a snowmobile convoy, right: 1
	Site(TEXT("PplP3CTobogganL"), -1, 77.f, 20, 5, 1.f, 1.25f);     // a toboggan hill, left: 2
	Site(TEXT("PplP3CSkidoosA"), -1, 77.f, 20, 11, 1.f, 1.25f);     // a snowmobile convoy, left: 2
	Site(TEXT("PplP3CSleighA"), -1, 77.f, 40, 36, 1.f, 1.25f);     // a reindeer sleigh, left: 1
	Site(TEXT("PplP3CKitesA"), 1, 110.f, 24, 6, 1.f, 1.5f);     // snowkiters, right: 1
	Site(TEXT("PplP3CKitesA"), -1, 110.f, 24, 2, 1.f, 1.5f);     // snowkiters, left: 2
	Site(TEXT("PplP3CSleighA"), -1, 110.f, 24, 17, 1.f, 1.5f);     // a reindeer sleigh, left: 1
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 3, 1.f, 1.f, 0.f, -1, 0.f);     // a skein of geese along the road, left: 2
Zone(TEXT("People p3c: The valley run, the verge from 8.00 km"), 8.f, 8.92f);
	Site(TEXT("PplP3CSkiTrailA"), 1, 38.5f, 16, 2, 1.f);     // cross-country skiers, right: 1
	Site(TEXT("PplP3CFatBikesA"), 1, 38.5f, 24, 22, 1.f);     // fat bikes, right: 1
	Site(TEXT("PplP3CSkiTrailA"), -1, 38.5f, 16, 0, 1.f);     // cross-country skiers, left: 1
Zone(TEXT("People p3c: The Abominable's glacier"), 8.94f, 11.6f);
	Site(TEXT("PplP3CHerdA"), 1, 64.f, 15, 0, 1.f, 1.2f);     // a reindeer herd, right: 4
	Site(TEXT("PplP3CSkiTrailB"), 1, 64.f, 15, 9, 1.f, 1.2f);     // skating skiers and a pulk, right: 4
	Site(TEXT("PplP3CHerdA"), -1, 64.f, 15, 14, 1.f, 1.2f);     // a reindeer herd, left: 4
	Site(TEXT("PplP3CSkiTrailB"), -1, 64.f, 15, 8, 1.f, 1.2f);     // skating skiers and a pulk, left: 4
	Site(TEXT("PplP3CSkidoosB"), 1, 77.f, 15, 12, 1.f, 1.25f);     // the patrol on snowmobiles, right: 4
	Site(TEXT("PplP3CKitesA"), 1, 77.f, 15, 6, 1.f, 1.25f);     // snowkiters, right: 4
	Site(TEXT("PplP3CSkidoosB"), -1, 77.f, 15, 11, 1.f, 1.25f);     // the patrol on snowmobiles, left: 4
	Site(TEXT("PplP3CKitesA"), -1, 77.f, 15, 5, 1.f, 1.25f);     // snowkiters, left: 4
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 9, 1.f, 1.f, 0.f, 1, 0.f);     // a skein of geese along the road, right: 3
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 20, 1.f, 1.f, 0.f, -1, 0.f);     // a skein of geese along the road, left: 3
Zone(TEXT("People p3c: The white ridge"), 11.6f, 14.3f);
	Site(TEXT("PplP3CFatBikesA"), 1, 64.f, 20, 19, 1.f, 1.2f);     // fat bikes, right: 3
	Site(TEXT("PplP3CSkiTrailB"), 1, 64.f, 20, 13, 1.f, 1.2f);     // skating skiers and a pulk, right: 3
	Site(TEXT("PplP3CSnowshoeA"), 1, 64.f, 20, 7, 1.f, 1.2f);     // a snowshoe party, right: 3
	Site(TEXT("PplP3CFatBikesA"), -1, 64.f, 20, 16, 1.f, 1.2f);     // fat bikes, left: 3
	Site(TEXT("PplP3CSkiTrailB"), -1, 64.f, 20, 10, 1.f, 1.2f);     // skating skiers and a pulk, left: 3
	Site(TEXT("PplP3CSnowshoeA"), -1, 64.f, 20, 4, 1.f, 1.2f);     // a snowshoe party, left: 3
	Site(TEXT("PplP3CSkidoosA"), 1, 77.f, 15, 0, 1.f, 1.25f);     // a snowmobile convoy, right: 4
	Site(TEXT("PplP3CSkidoosB"), 1, 77.f, 15, 10, 1.f, 1.25f);     // the patrol on snowmobiles, right: 4
	Site(TEXT("PplP3CSleighA"), 1, 77.f, 30, 5, 1.f, 1.25f);     // a reindeer sleigh, right: 2
	Site(TEXT("PplP3CSkidoosA"), -1, 77.f, 15, 13, 1.f, 1.25f);     // a snowmobile convoy, left: 4
	Site(TEXT("PplP3CSkidoosB"), -1, 77.f, 15, 4, 1.f, 1.25f);     // the patrol on snowmobiles, left: 4
	Site(TEXT("PplP3CKitesA"), 1, 110.f, 18, 16, 1.f, 1.5f);     // snowkiters, right: 3
	Site(TEXT("PplP3CHerdA"), 1, 110.f, 24, 10, 1.f, 1.5f);     // a reindeer herd, right: 2
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 4, 1.f, 1.f, 0.f, 1, 0.f);     // a skein of geese along the road, right: 3
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 15, 1.f, 1.f, 0.f, -1, 0.f);     // a skein of geese along the road, left: 3
Zone(TEXT("People p3c: The white ridge, the verge from 11.62 km"), 11.62f, 14.28f);
	Site(TEXT("PplP3CSkiTrailB"), 1, 38.5f, 12, 8, 1.f);     // skating skiers and a pulk, right: 4
	Site(TEXT("PplP3CSkiTrailA"), 1, 38.5f, 24, 0, 1.f);     // cross-country skiers, right: 3
	Site(TEXT("PplP3CSnowshoeA"), 1, 38.5f, 24, 4, 1.f);     // a snowshoe party, right: 3
	Site(TEXT("PplP3CSkiTrailB"), -1, 38.5f, 16, 13, 1.f);     // skating skiers and a pulk, left: 4
	Site(TEXT("PplP3CSkiTrailA"), -1, 38.5f, 16, 2, 1.f);     // cross-country skiers, left: 3
Zone(TEXT("People p3c: The resort"), 14.3f, 17.f);
	Site(TEXT("PplP3CSkiTrailA"), 1, 64.f, 30, 15, 1.f, 1.2f);     // cross-country skiers, right: 2
	Site(TEXT("PplP3CSkatersA"), 1, 67.f, 600, 283, 1.f);
	Site(TEXT("PplP3CTobogganR"), 1, 64.f, 600, 277, 1.f, 1.2f);
	Site(TEXT("PplP3CFatBikesA"), 1, 64.f, 600, 267, 1.f, 1.2f);
	Site(TEXT("PplP3CSkatersA"), 1, 67.f, 60, 17, 1.f);     // skaters on a rink, right: 1
	Site(TEXT("PplP3CSkiTrailA"), -1, 64.f, 30, 20, 1.f, 1.2f);     // cross-country skiers, left: 2

	Site(TEXT("PplP3CTobogganL"), -1, 64.f, 30, 18, 1.f, 1.2f);     // a toboggan hill, left: 2
	Site(TEXT("PplP3CFatBikesA"), -1, 64.f, 30, 6, 1.f, 1.2f);     // fat bikes, left: 2

	Site(TEXT("PplP3CSkidoosB"), 1, 77.f, 20, 14, 1.f, 1.25f);     // the patrol on snowmobiles, right: 3
	Site(TEXT("PplP3CTobogganR"), 1, 77.f, 40, 28, 1.f, 1.25f);     // a toboggan hill, right: 2
	Site(TEXT("PplP3CSkidoosA"), 1, 77.f, 40, 1, 1.f, 1.25f);     // a snowmobile convoy, right: 2
	Site(TEXT("PplP3CSleighA"), 1, 77.f, 40, 23, 1.f, 1.25f);     // a reindeer sleigh, right: 1
	Site(TEXT("PplP3CSkidoosB"), -1, 77.f, 25, 11, 1.f, 1.25f);     // the patrol on snowmobiles, left: 2
	Site(TEXT("PplP3CTobogganL"), -1, 77.f, 25, 17, 1.f, 1.25f);     // a toboggan hill, left: 2
	Site(TEXT("PplP3CSkidoosA"), -1, 77.f, 40, 14, 1.f, 1.25f);     // a snowmobile convoy, left: 1
	Site(TEXT("PplP3CSleighA"), -1, 77.f, 40, 8, 1.f, 1.25f);     // a reindeer sleigh, left: 1
	Site(TEXT("PplP3CSleighA"), 1, 110.f, 24, 16, 1.f, 1.5f);     // a reindeer sleigh, right: 3
	Site(TEXT("PplP3CKitesA"), 1, 110.f, 24, 22, 1.f, 1.5f);     // snowkiters, right: 2
	Site(TEXT("PplP3CSleighA"), -1, 110.f, 12, 9, 1.f, 1.5f);     // a reindeer sleigh, left: 5
	Site(TEXT("PplP3CKitesA"), -1, 110.f, 12, 3, 1.f, 1.5f);     // snowkiters, left: 5
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 21, 1.f, 1.f, 0.f, 1, 0.f);     // a skein of geese along the road, right: 3
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 10, 1.f, 1.f, 0.f, -1, 0.f);     // a skein of geese along the road, left: 3
Zone(TEXT("People p3c: The resort, the verge from 14.99 km"), 14.99f, 15.73f);
	Site(TEXT("PplP3CSkiTrailA"), 1, 38.5f, 12, 7, 1.f);     // cross-country skiers, right: 1
	Site(TEXT("PplP3CFatBikesA"), 1, 38.5f, 16, 7, 1.f);     // fat bikes, right: 1
	Site(TEXT("PplP3CSkiTrailA"), -1, 38.5f, 12, 3, 1.f);     // cross-country skiers, left: 2
Zone(TEXT("People p3c: The resort, the verge from 16.13 km"), 16.13f, 16.87f);
	Site(TEXT("PplP3CSkiTrailA"), 1, 38.5f, 12, 11, 1.f);     // cross-country skiers, right: 1
	Site(TEXT("PplP3CFatBikesA"), 1, 38.5f, 12, 7, 1.f);     // fat bikes, right: 1
	Site(TEXT("PplP3CSkiTrailB"), 1, 38.5f, 16, 14, 1.f);     // skating skiers and a pulk, right: 1
	Site(TEXT("PplP3CSkiTrailA"), -1, 38.5f, 12, 9, 1.f);     // cross-country skiers, left: 1
	Site(TEXT("PplP3CFatBikesA"), -1, 38.5f, 16, 15, 1.f);     // fat bikes, left: 1
Zone(TEXT("People p3c: The ice again"), 17.f, 19.7f);
	Site(TEXT("PplP3CHerdA"), 1, 64.f, 15, 0, 1.f, 1.2f);     // a reindeer herd, right: 4
	Site(TEXT("PplP3CSkiTrailB"), 1, 64.f, 15, 9, 1.f, 1.2f);     // skating skiers and a pulk, right: 4
	Site(TEXT("PplP3CHerdA"), -1, 64.f, 15, 14, 1.f, 1.2f);     // a reindeer herd, left: 4
	Site(TEXT("PplP3CSkiTrailB"), -1, 64.f, 15, 8, 1.f, 1.2f);     // skating skiers and a pulk, left: 4
	Site(TEXT("PplP3CSkidoosB"), 1, 77.f, 15, 12, 1.f, 1.25f);     // the patrol on snowmobiles, right: 4
	Site(TEXT("PplP3CKitesA"), 1, 77.f, 15, 6, 1.f, 1.25f);     // snowkiters, right: 4
	Site(TEXT("PplP3CSkidoosB"), -1, 77.f, 15, 11, 1.f, 1.25f);     // the patrol on snowmobiles, left: 4
	Site(TEXT("PplP3CKitesA"), -1, 77.f, 15, 5, 1.f, 1.25f);     // snowkiters, left: 4
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 16, 1.f, 1.f, 0.f, 1, 0.f);     // a skein of geese along the road, right: 3
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 5, 1.f, 1.f, 0.f, -1, 0.f);     // a skein of geese along the road, left: 3
Zone(TEXT("People p3c: The high ridge"), 19.7f, 22.4f);
	Site(TEXT("PplP3CSkiTrailA"), 1, 64.f, 20, 19, 1.f, 1.2f);     // cross-country skiers, right: 3
	Site(TEXT("PplP3CFatBikesA"), 1, 64.f, 20, 13, 1.f, 1.2f);     // fat bikes, right: 3
	Site(TEXT("PplP3CSkiTrailB"), 1, 64.f, 20, 7, 1.f, 1.2f);     // skating skiers and a pulk, right: 3
	Site(TEXT("PplP3CSkiTrailA"), -1, 64.f, 20, 16, 1.f, 1.2f);     // cross-country skiers, left: 3
	Site(TEXT("PplP3CFatBikesA"), -1, 64.f, 20, 10, 1.f, 1.2f);     // fat bikes, left: 3
	Site(TEXT("PplP3CSnowshoeA"), -1, 64.f, 20, 4, 1.f, 1.2f);     // a snowshoe party, left: 3
	Site(TEXT("PplP3CSkidoosA"), 1, 77.f, 15, 11, 1.f, 1.25f);     // a snowmobile convoy, right: 4
	Site(TEXT("PplP3CSkidoosB"), 1, 77.f, 15, 5, 1.f, 1.25f);     // the patrol on snowmobiles, right: 4
	Site(TEXT("PplP3CSkidoosA"), -1, 77.f, 15, 14, 1.f, 1.25f);     // a snowmobile convoy, left: 4
	Site(TEXT("PplP3CSkidoosB"), -1, 77.f, 15, 8, 1.f, 1.25f);     // the patrol on snowmobiles, left: 4
	Site(TEXT("PplP3CHerdA"), 1, 110.f, 18, 1, 1.f, 1.5f);     // a reindeer herd, right: 3
	Site(TEXT("PplP3CKitesA"), 1, 110.f, 24, 1, 1.f, 1.5f);     // snowkiters, right: 2
	Site(TEXT("PplP3CHerdA"), -1, 110.f, 24, 6, 1.f, 1.5f);     // a reindeer herd, left: 2
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 11, 1.f, 1.f, 0.f, 1, 0.f);     // a skein of geese along the road, right: 3
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 0, 1.f, 1.f, 0.f, -1, 0.f);     // a skein of geese along the road, left: 3
Zone(TEXT("People p3c: The high ridge, the verge from 19.72 km"), 19.72f, 20.28f);
	Site(TEXT("PplP3CSkiTrailB"), 1, 38.5f, 12, 11, 1.f);     // skating skiers and a pulk, right: 1
	Site(TEXT("PplP3CSkiTrailA"), 1, 38.5f, 12, 4, 1.f);     // cross-country skiers, right: 1
	Site(TEXT("PplP3CSkiTrailB"), -1, 38.5f, 12, 9, 1.f);     // skating skiers and a pulk, left: 1
	Site(TEXT("PplP3CSkiTrailA"), -1, 38.5f, 12, 2, 1.f);     // cross-country skiers, left: 1
	Site(TEXT("PplP3CSnowshoeA"), -1, 38.5f, 12, 7, 1.f);     // a snowshoe party, left: 1
Zone(TEXT("People p3c: The high ridge, the verge from 20.68 km"), 20.68f, 21.42f);
	Site(TEXT("PplP3CSkiTrailB"), 1, 38.5f, 12, 8, 1.f);     // skating skiers and a pulk, right: 2
	Site(TEXT("PplP3CSkiTrailA"), 1, 38.5f, 12, 0, 1.f);     // cross-country skiers, right: 1
	Site(TEXT("PplP3CSnowshoeA"), 1, 38.5f, 12, 4, 1.f);     // a snowshoe party, right: 1
	Site(TEXT("PplP3CSkiTrailB"), -1, 38.5f, 12, 9, 1.f);     // skating skiers and a pulk, left: 2
	Site(TEXT("PplP3CSkiTrailA"), -1, 38.5f, 12, 3, 1.f);     // cross-country skiers, left: 1
Zone(TEXT("People p3c: The high ridge, the verge from 21.81 km"), 21.81f, 22.38f);
	Site(TEXT("PplP3CSkiTrailB"), 1, 38.5f, 12, 11, 1.f);     // skating skiers and a pulk, right: 1
	Site(TEXT("PplP3CSkiTrailA"), 1, 38.5f, 12, 9, 1.f);     // cross-country skiers, right: 1
	Site(TEXT("PplP3CSnowshoeA"), 1, 38.5f, 12, 5, 1.f);     // a snowshoe party, right: 1
	Site(TEXT("PplP3CSkiTrailB"), -1, 38.5f, 12, 2, 1.f);     // skating skiers and a pulk, left: 1
	Site(TEXT("PplP3CSkiTrailA"), -1, 38.5f, 12, 7, 1.f);     // cross-country skiers, left: 1
Zone(TEXT("People p3c: The last lifts"), 22.4f, 99.f);
	Site(TEXT("PplP3CFatBikesA"), 1, 64.f, 600, 69, 1.f, 1.2f);
	Site(TEXT("PplP3CSkatersA"), 1, 67.f, 35, 12, 1.f);     // skaters on a rink, right: 2
	Site(TEXT("PplP3CTobogganR"), 1, 64.f, 600, 95, 1.f, 1.2f);
	Site(TEXT("PplP3CSkiTrailA"), 1, 64.f, 30, 17, 1.f, 1.2f);     // cross-country skiers, right: 1
	Site(TEXT("PplP3CSkatersA"), 1, 67.f, 35, 6, 1.f);     // skaters on a rink, right: 1

	Site(TEXT("PplP3CTobogganL"), -1, 64.f, 30, 6, 1.f, 1.2f);     // a toboggan hill, left: 2

	Site(TEXT("PplP3CSleighA"), 1, 77.f, 20, 14, 1.f, 1.25f);     // a reindeer sleigh, right: 3
	Site(TEXT("PplP3CSkidoosB"), 1, 77.f, 40, 27, 1.f, 1.25f);     // the patrol on snowmobiles, right: 1
	Site(TEXT("PplP3CTobogganR"), 1, 77.f, 40, 8, 1.f, 1.25f);     // a toboggan hill, right: 1
	Site(TEXT("PplP3CSkidoosA"), 1, 77.f, 40, 3, 1.f, 1.25f);     // a snowmobile convoy, right: 1
	Site(TEXT("PplP3CSleighA"), -1, 77.f, 40, 33, 1.f, 1.25f);     // a reindeer sleigh, left: 1
	Site(TEXT("PplP3CSkidoosB"), -1, 77.f, 40, 39, 1.f, 1.25f);     // the patrol on snowmobiles, left: 1
	Site(TEXT("PplP3CTobogganL"), -1, 77.f, 40, 23, 1.f, 1.25f);     // a toboggan hill, left: 2
	Site(TEXT("PplP3CSkidoosA"), -1, 77.f, 40, 5, 1.f, 1.25f);     // a snowmobile convoy, left: 1
	Site(TEXT("PplP3CKitesA"), 1, 110.f, 24, 0, 1.f, 1.5f);     // snowkiters, right: 2
	Site(TEXT("PplP3CSleighA"), 1, 110.f, 24, 17, 1.f, 1.5f);     // a reindeer sleigh, right: 2
	Site(TEXT("PplP3CKitesA"), -1, 110.f, 18, 4, 1.f, 1.5f);     // snowkiters, left: 3
	Site(TEXT("PplP3CSleighA"), -1, 110.f, 18, 12, 1.f, 1.5f);     // a reindeer sleigh, left: 3
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 6, 1.f, 1.f, 0.f, 1, 0.f);     // a skein of geese along the road, right: 3
	Scatter(P3C("Geese_A"), 80.f, 130.f, 22, 17, 1.f, 1.f, 0.f, -1, 0.f);     // a skein of geese along the road, left: 2
Zone(TEXT("People p3c: The last lifts, the verge from 22.95 km"), 22.95f, 23.7f);
	Site(TEXT("PplP3CSkiTrailA"), 1, 38.5f, 12, 10, 1.f);     // cross-country skiers, right: 1
	Site(TEXT("PplP3CFatBikesA"), 1, 38.5f, 12, 2, 1.f);     // fat bikes, right: 1
	Site(TEXT("PplP3CSkiTrailA"), -1, 38.5f, 12, 3, 1.f);     // cross-country skiers, left: 1
Zone(TEXT("People p3c: The last lifts, the verge from 24.09 km"), 24.09f, 24.93f);
	Site(TEXT("PplP3CSkiTrailA"), 1, 38.5f, 12, 7, 1.f);     // cross-country skiers, right: 2
	Site(TEXT("PplP3CFatBikesA"), 1, 38.5f, 12, 11, 1.f);     // fat bikes, right: 1
	Site(TEXT("PplP3CSkiTrailA"), -1, 38.5f, 12, 9, 1.f);     // cross-country skiers, left: 1
	Site(TEXT("PplP3CFatBikesA"), -1, 38.5f, 24, 3, 1.f);     // fat bikes, left: 1
Zone(TEXT("People p3c: under the chute's corkscrew"), 2.51f, 2.685f);
	Far(P3C("Geese_A"), 85.f, 110.f, 8, 6, 1.f, 1.f, 0.f, 1, 0.5f, 0.f);     // geese past the set piece (Scatter is left out here): 1
Zone(TEXT("People p3c: under Black diamond's loop"), 4.085f, 4.37f);
	Far(P3C("Geese_A"), 85.f, 110.f, 8, 2, 1.f, 1.f, 0.f, -1, 0.5f, 0.f);     // geese past the set piece (Scatter is left out here): 1
Zone(TEXT("People p3c: under Black diamond's corkscrew"), 5.225f, 5.4f);
	Far(P3C("Geese_A"), 85.f, 110.f, 8, 2, 1.f, 1.f, 0.f, 1, 0.5f, 0.f);     // geese past the set piece (Scatter is left out here): 1
Zone(TEXT("People p3c: under the whiteout's loop"), 6.87f, 7.155f);
	Far(P3C("Geese_A"), 85.f, 110.f, 8, 5, 1.f, 1.f, 0.f, -1, 0.5f, 0.f);     // geese past the set piece (Scatter is left out here): 1
Zone(TEXT("People p3c: under the valley's corkscrew"), 7.215f, 7.385f);
	Far(P3C("Geese_A"), 85.f, 110.f, 8, 6, 1.f, 1.f, 0.f, 1, 0.5f, 0.f);     // geese past the set piece (Scatter is left out here): 1
#undef P3C
// p3c, counted by the scratch tool from these rules (before the track's own leaving out): 461 pieces: FatBikes_A 39, Geese_A 51, Herd_A 30, Kites_A 47, Skaters_A 22, SkiTrail_A 43, SkiTrail_B 50, Skidoos_A 35, Skidoos_B 50, Sleigh_A 35, Snowshoe_A 21, TobogganL_A 20, Toboggan_A 18.
// ---- END p3c ----
}

{
// ---- BEGIN p3d (heights, ft_people_ice_p3d.py) ----
// (Fix round, 16 Nov 2026: after two reviews rules of this block were moved, re-phased or taken out; the counts in this header are the
// builder's. What changed and why: SourceArt/Integration/people/L5_ice_p3.md, "Review and fix round".)
// PEOPLE UP HIGH (pass 3, builder p3d): thirteen meshes (SM_P3D_...), each a structure with its people on it, 1.9 times life
// size, all TALL: lookout towers, footbridges with walkers crossing along the road, chalet balconies, crags of rock and of
// ice with summit parties, a judges' tower, a lift pylon with its mechanics and a chair, an avalanche dam with a row of
// watchers, the rescue deck, an ice arch with a roped party. Where they stand: 64 to 67 m out in the resort and ridge
// districts (behind the cast at 30 to 55 m and the snowmen at 60 to 63), 53 to 56 m in the glacier districts (between the
// ice walls), and by Far rules 86 to 88 m out under the loops and corkscrews, which leave Scatter and Site out. Every
// set turns its piece 30 degrees towards the car coming down the road (R sets: yaw -30; L sets: +30). A side has a piece
// on every second slot at chance 0.75: the right on the even residues of 8, the left on the odd. Nothing glows or blinks.
#define P3D(Name) TEXT("/Game/People/IceMountain/SM_P3D_" Name)
Set(TEXT("PplP3DLookoutAR"), { { P3D("Lookout_A"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3DLookoutAL"), { { P3D("Lookout_A"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3DLookoutBR"), { { P3D("Lookout_B"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3DLookoutBL"), { { P3D("Lookout_B"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3DBridgeAR"), { { P3D("Bridge_A"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3DBridgeAL"), { { P3D("Bridge_A"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3DBridgeBR"), { { P3D("Bridge_B"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3DBridgeBL"), { { P3D("Bridge_B"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3DBalconyAR"), { { P3D("Balcony_A"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3DBalconyBL"), { { P3D("Balcony_B"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3DCragAR"), { { P3D("Crag_A"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3DCragAL"), { { P3D("Crag_A"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3DCragBR"), { { P3D("Crag_B"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3DCragBL"), { { P3D("Crag_B"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3DJudgesAR"), { { P3D("Judges_A"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3DPylonAR"), { { P3D("Pylon_A"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3DPylonAL"), { { P3D("Pylon_A"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3DDamAR"), { { P3D("Dam_A"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3DDamAL"), { { P3D("Dam_A"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3DHeliDeckAR"), { { P3D("HeliDeck_A"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3DHeliDeckAL"), { { P3D("HeliDeck_A"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplP3DIceArchAR"), { { P3D("IceArch_A"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplP3DIceArchAL"), { { P3D("IceArch_A"), 0, 0, 0, 30, 1 } });

Zone(TEXT("People p3d: First tracks"), 0.f, 1.19f);     // chalet balconies, the judges, a lookout and a lift pylon on the right; balconies, a footbridge, the rescue deck and a lookout on the left
	Site(TEXT("PplP3DBalconyAR"), 1, 65.f, 16, 0, 0.75f);
	Site(TEXT("PplP3DJudgesAR"), 1, 66.f, 16, 4, 0.75f);
	Site(TEXT("PplP3DLookoutAR"), 1, 67.f, 16, 8, 0.75f);
	Site(TEXT("PplP3DPylonAR"), 1, 65.f, 16, 12, 0.75f);
	Site(TEXT("PplP3DBalconyBL"), -1, 66.f, 16, 1, 0.75f);
	Site(TEXT("PplP3DBridgeAL"), -1, 67.f, 16, 5, 0.75f);
	Site(TEXT("PplP3DHeliDeckAL"), -1, 65.f, 16, 9, 0.75f);
	Site(TEXT("PplP3DLookoutBL"), -1, 66.f, 16, 13, 0.75f);

Zone(TEXT("People p3d: The half pipe"), 1.19f, 2.43f);     // chalet balconies, the judges, a lookout and a lift pylon on the right; balconies, a footbridge, the rescue deck and a lookout on the left
	Site(TEXT("PplP3DJudgesAR"), 1, 65.f, 16, 0, 0.75f);
	Site(TEXT("PplP3DLookoutAR"), 1, 66.f, 16, 4, 0.75f);
	Site(TEXT("PplP3DPylonAR"), 1, 67.f, 16, 8, 0.75f);
	Site(TEXT("PplP3DBalconyAR"), 1, 65.f, 16, 12, 0.75f);
	Site(TEXT("PplP3DBridgeAL"), -1, 66.f, 16, 1, 0.75f);
	Site(TEXT("PplP3DHeliDeckAL"), -1, 67.f, 16, 5, 0.75f);
	Site(TEXT("PplP3DLookoutBL"), -1, 65.f, 16, 9, 0.75f);
	Site(TEXT("PplP3DBalconyBL"), -1, 66.f, 16, 13, 0.75f);

Zone(TEXT("People p3d: Avalanche chute"), 2.43f, 2.96f);     // avalanche dams with their watchers, summit parties on crags, a footbridge, lookouts, a pylon crew
	Site(TEXT("PplP3DDamAR"), 1, 65.f, 8, 0, 0.75f);
	Site(TEXT("PplP3DCragAR"), 1, 66.f, 8, 2, 0.75f);
	Site(TEXT("PplP3DBridgeAR"), 1, 67.f, 8, 4, 0.75f);
	Site(TEXT("PplP3DLookoutBR"), 1, 65.f, 8, 6, 0.75f);
	Site(TEXT("PplP3DCragAL"), -1, 66.f, 8, 1, 0.75f);
	Site(TEXT("PplP3DDamAL"), -1, 67.f, 8, 3, 0.75f);
	Site(TEXT("PplP3DPylonAL"), -1, 65.f, 8, 5, 0.75f);
	Site(TEXT("PplP3DLookoutAL"), -1, 66.f, 8, 7, 0.75f);

Zone(TEXT("People p3d: The super pipe"), 2.96f, 4.02f);     // ice arches with roped parties, ice crags, ice-pier footbridges, the rescue deck: 54 m, between the ice walls at 38 to 46 and 62 to 72 m
	Site(TEXT("PplP3DIceArchAR"), 1, 54.f, 8, 0, 0.75f);
	Site(TEXT("PplP3DCragBR"), 1, 55.f, 8, 2, 0.75f);
	Site(TEXT("PplP3DBridgeBR"), 1, 56.f, 8, 4, 0.75f);
	Site(TEXT("PplP3DHeliDeckAR"), 1, 54.f, 8, 6, 0.75f);
	Site(TEXT("PplP3DCragBL"), -1, 55.f, 8, 1, 0.75f);
	Site(TEXT("PplP3DIceArchAL"), -1, 56.f, 8, 3, 0.75f);
	Site(TEXT("PplP3DHeliDeckAL"), -1, 54.f, 8, 5, 0.75f);
	Site(TEXT("PplP3DBridgeBL"), -1, 55.f, 8, 7, 0.75f);

Zone(TEXT("People p3d: Black diamond"), 4.02f, 5.65f);     // avalanche dams with their watchers, summit parties on crags, a footbridge, lookouts, a pylon crew
	Site(TEXT("PplP3DBridgeAR"), 1, 65.f, 8, 0, 0.75f);
	Site(TEXT("PplP3DLookoutBR"), 1, 66.f, 8, 2, 0.75f);
	Site(TEXT("PplP3DDamAR"), 1, 67.f, 8, 4, 0.75f);
	Site(TEXT("PplP3DCragAR"), 1, 65.f, 8, 6, 0.75f);
	Site(TEXT("PplP3DPylonAL"), -1, 66.f, 8, 1, 0.75f);
	Site(TEXT("PplP3DLookoutAL"), -1, 67.f, 8, 3, 0.75f);
	Site(TEXT("PplP3DCragAL"), -1, 65.f, 8, 5, 0.75f);
	Site(TEXT("PplP3DDamAL"), -1, 66.f, 8, 7, 0.75f);

Zone(TEXT("People p3d: The whiteout"), 5.65f, 7.3f);     // avalanche dams with their watchers, summit parties on crags, a footbridge, lookouts, a pylon crew
	Site(TEXT("PplP3DCragAR"), 1, 65.f, 8, 0, 0.75f);
	Site(TEXT("PplP3DBridgeAR"), 1, 66.f, 8, 2, 0.75f);
	Site(TEXT("PplP3DLookoutBR"), 1, 67.f, 8, 4, 0.75f);
	Site(TEXT("PplP3DDamAR"), 1, 65.f, 8, 6, 0.75f);
	Site(TEXT("PplP3DDamAL"), -1, 66.f, 8, 1, 0.75f);
	Site(TEXT("PplP3DPylonAL"), -1, 67.f, 8, 3, 0.75f);
	Site(TEXT("PplP3DLookoutAL"), -1, 65.f, 8, 5, 0.75f);
	Site(TEXT("PplP3DCragAL"), -1, 66.f, 8, 7, 0.75f);

Zone(TEXT("People p3d: The valley run (1)"), 7.3f, 7.39f);     // fix round: not behind the village street's chalet rows
	Site(TEXT("PplP3DLookoutAR"), 1, 65.f, 8, 0, 0.75f);
	Site(TEXT("PplP3DPylonAR"), 1, 66.f, 8, 2, 0.75f);
	Site(TEXT("PplP3DBalconyAR"), 1, 67.f, 8, 4, 0.75f);
	Site(TEXT("PplP3DJudgesAR"), 1, 65.f, 8, 6, 0.75f);
	Site(TEXT("PplP3DHeliDeckAL"), -1, 66.f, 8, 1, 0.75f);
	Site(TEXT("PplP3DLookoutBL"), -1, 67.f, 8, 3, 0.75f);
	Site(TEXT("PplP3DBalconyBL"), -1, 65.f, 8, 5, 0.75f);
	Site(TEXT("PplP3DBridgeAL"), -1, 66.f, 8, 7, 0.75f);
Zone(TEXT("People p3d: The valley run (2)"), 7.51f, 7.99f);     // fix round: not behind the village street's chalet rows
	Site(TEXT("PplP3DLookoutAR"), 1, 65.f, 8, 0, 0.75f);
	Site(TEXT("PplP3DPylonAR"), 1, 66.f, 8, 2, 0.75f);
	Site(TEXT("PplP3DBalconyAR"), 1, 67.f, 8, 4, 0.75f);
	Site(TEXT("PplP3DJudgesAR"), 1, 65.f, 8, 6, 0.75f);
	Site(TEXT("PplP3DHeliDeckAL"), -1, 66.f, 8, 1, 0.75f);
	Site(TEXT("PplP3DLookoutBL"), -1, 67.f, 8, 3, 0.75f);
	Site(TEXT("PplP3DBalconyBL"), -1, 65.f, 8, 5, 0.75f);
	Site(TEXT("PplP3DBridgeAL"), -1, 66.f, 8, 7, 0.75f);
Zone(TEXT("People p3d: The valley run (3)"), 8.03f, 8.21f);     // fix round: not behind the village street's chalet rows
	Site(TEXT("PplP3DLookoutAR"), 1, 65.f, 8, 0, 0.75f);
	Site(TEXT("PplP3DPylonAR"), 1, 66.f, 8, 2, 0.75f);
	Site(TEXT("PplP3DBalconyAR"), 1, 67.f, 8, 4, 0.75f);
	Site(TEXT("PplP3DJudgesAR"), 1, 65.f, 8, 6, 0.75f);
	Site(TEXT("PplP3DHeliDeckAL"), -1, 66.f, 8, 1, 0.75f);
	Site(TEXT("PplP3DLookoutBL"), -1, 67.f, 8, 3, 0.75f);
	Site(TEXT("PplP3DBalconyBL"), -1, 65.f, 8, 5, 0.75f);
	Site(TEXT("PplP3DBridgeAL"), -1, 66.f, 8, 7, 0.75f);
Zone(TEXT("People p3d: The valley run (4)"), 8.31f, 8.94f);     // fix round: not behind the village street's chalet rows
	Site(TEXT("PplP3DLookoutAR"), 1, 65.f, 8, 0, 0.75f);
	Site(TEXT("PplP3DPylonAR"), 1, 66.f, 8, 2, 0.75f);
	Site(TEXT("PplP3DBalconyAR"), 1, 67.f, 8, 4, 0.75f);
	Site(TEXT("PplP3DJudgesAR"), 1, 65.f, 8, 6, 0.75f);
	Site(TEXT("PplP3DHeliDeckAL"), -1, 66.f, 8, 1, 0.75f);
	Site(TEXT("PplP3DLookoutBL"), -1, 67.f, 8, 3, 0.75f);
	Site(TEXT("PplP3DBalconyBL"), -1, 65.f, 8, 5, 0.75f);
	Site(TEXT("PplP3DBridgeAL"), -1, 66.f, 8, 7, 0.75f);

Zone(TEXT("People p3d: The Abominable's glacier"), 8.94f, 11.6f);     // ice arches with roped parties, ice crags, ice-pier footbridges, the rescue deck: 54 m, between the ice walls at 38 to 46 and 62 to 72 m
	Site(TEXT("PplP3DCragBR"), 1, 54.f, 8, 0, 0.75f);
	Site(TEXT("PplP3DBridgeBR"), 1, 55.f, 8, 2, 0.75f);
	Site(TEXT("PplP3DHeliDeckAR"), 1, 56.f, 8, 4, 0.75f);
	Site(TEXT("PplP3DIceArchAR"), 1, 54.f, 8, 6, 0.75f);
	Site(TEXT("PplP3DIceArchAL"), -1, 55.f, 8, 1, 0.75f);
	Site(TEXT("PplP3DHeliDeckAL"), -1, 56.f, 8, 3, 0.75f);
	Site(TEXT("PplP3DBridgeBL"), -1, 54.f, 8, 5, 0.75f);
	Site(TEXT("PplP3DCragBL"), -1, 55.f, 8, 7, 0.75f);

Zone(TEXT("People p3d: The white ridge"), 11.6f, 14.3f);     // avalanche dams with their watchers, summit parties on crags, a footbridge, lookouts, a pylon crew
	Site(TEXT("PplP3DLookoutBR"), 1, 65.f, 8, 0, 0.75f);
	Site(TEXT("PplP3DDamAR"), 1, 66.f, 8, 2, 0.75f);
	Site(TEXT("PplP3DCragAR"), 1, 67.f, 8, 4, 0.75f);
	Site(TEXT("PplP3DBridgeAR"), 1, 65.f, 8, 6, 0.75f);
	Site(TEXT("PplP3DLookoutAL"), -1, 66.f, 8, 1, 0.75f);
	Site(TEXT("PplP3DCragAL"), -1, 67.f, 8, 3, 0.75f);
	Site(TEXT("PplP3DDamAL"), -1, 65.f, 8, 5, 0.75f);
	Site(TEXT("PplP3DPylonAL"), -1, 66.f, 8, 7, 0.75f);

Zone(TEXT("People p3d: The resort"), 14.3f, 17.f);     // chalet balconies, the judges, a lookout and a lift pylon on the right; balconies, a footbridge, the rescue deck and a lookout on the left
	Site(TEXT("PplP3DPylonAR"), 1, 65.f, 8, 0, 0.75f);
	Site(TEXT("PplP3DBalconyAR"), 1, 66.f, 8, 2, 0.75f);
	Site(TEXT("PplP3DJudgesAR"), 1, 67.f, 8, 4, 0.75f);
	Site(TEXT("PplP3DLookoutAR"), 1, 65.f, 8, 6, 0.75f);
	Site(TEXT("PplP3DLookoutBL"), -1, 66.f, 8, 1, 0.75f);
	Site(TEXT("PplP3DBalconyBL"), -1, 67.f, 8, 3, 0.75f);
	Site(TEXT("PplP3DBridgeAL"), -1, 65.f, 8, 5, 0.75f);
	Site(TEXT("PplP3DHeliDeckAL"), -1, 66.f, 8, 7, 0.75f);

Zone(TEXT("People p3d: The ice again"), 17.f, 19.7f);     // ice arches with roped parties, ice crags, ice-pier footbridges, the rescue deck: 54 m, between the ice walls at 38 to 46 and 62 to 72 m
	Site(TEXT("PplP3DBridgeBR"), 1, 54.f, 8, 0, 0.75f);
	Site(TEXT("PplP3DHeliDeckAR"), 1, 55.f, 8, 2, 0.75f);
	Site(TEXT("PplP3DIceArchAR"), 1, 56.f, 8, 4, 0.75f);
	Site(TEXT("PplP3DCragBR"), 1, 54.f, 8, 6, 0.75f);
	Site(TEXT("PplP3DHeliDeckAL"), -1, 55.f, 8, 1, 0.75f);
	Site(TEXT("PplP3DBridgeBL"), -1, 56.f, 8, 3, 0.75f);
	Site(TEXT("PplP3DCragBL"), -1, 54.f, 8, 5, 0.75f);
	Site(TEXT("PplP3DIceArchAL"), -1, 55.f, 8, 7, 0.75f);

Zone(TEXT("People p3d: The high ridge"), 19.7f, 22.4f);     // avalanche dams with their watchers, summit parties on crags, a footbridge, lookouts, a pylon crew
	Site(TEXT("PplP3DDamAR"), 1, 65.f, 8, 0, 0.75f);
	Site(TEXT("PplP3DCragAR"), 1, 66.f, 8, 2, 0.75f);
	Site(TEXT("PplP3DBridgeAR"), 1, 67.f, 8, 4, 0.75f);
	Site(TEXT("PplP3DLookoutBR"), 1, 65.f, 8, 6, 0.75f);
	Site(TEXT("PplP3DCragAL"), -1, 66.f, 8, 1, 0.75f);
	Site(TEXT("PplP3DDamAL"), -1, 67.f, 8, 3, 0.75f);
	Site(TEXT("PplP3DPylonAL"), -1, 65.f, 8, 5, 0.75f);
	Site(TEXT("PplP3DLookoutAL"), -1, 66.f, 8, 7, 0.75f);

Zone(TEXT("People p3d: The last lifts"), 22.4f, 99.f);     // chalet balconies, the judges, a lookout and a lift pylon on the right; balconies, a footbridge, the rescue deck and a lookout on the left
	Site(TEXT("PplP3DJudgesAR"), 1, 65.f, 8, 0, 0.75f);
	Site(TEXT("PplP3DLookoutAR"), 1, 66.f, 8, 2, 0.75f);
	Site(TEXT("PplP3DPylonAR"), 1, 67.f, 8, 4, 0.75f);
	Site(TEXT("PplP3DBalconyAR"), 1, 65.f, 8, 6, 0.75f);
	Site(TEXT("PplP3DBridgeAL"), -1, 66.f, 8, 1, 0.75f);
	Site(TEXT("PplP3DHeliDeckAL"), -1, 67.f, 8, 3, 0.75f);
	Site(TEXT("PplP3DLookoutBL"), -1, 65.f, 8, 5, 0.75f);
	Site(TEXT("PplP3DBalconyBL"), -1, 66.f, 8, 7, 0.75f);

Zone(TEXT("People p3d: under the chute's corkscrew"), 2.51f, 2.685f);     // Far rules, on the land 86 to 88 m out (it lies 0.39 m over the road's level there)
	Far(P3D("Judges_A"), 86.f, 88.f, 3, 0, 1.f, 1.f, 0.f, 1, -0.2f, 0.f);
	Far(P3D("Balcony_B"), 86.f, 88.f, 3, 1, 1.f, 1.f, 0.f, -1, -0.2f, 0.f);
	Far(P3D("Balcony_A"), 86.f, 88.f, 3, 2, 1.f, 1.f, 0.f, 1, -0.2f, 0.f);
	Far(P3D("Judges_A"), 86.f, 88.f, 3, 0, 1.f, 1.f, 0.f, -1, -0.2f, 0.f);

Zone(TEXT("People p3d: under Black diamond's loop"), 4.085f, 4.37f);     // Far rules, on the land 86 to 88 m out (it lies 0.39 m over the road's level there)
	Far(P3D("Judges_A"), 86.f, 88.f, 3, 1, 1.f, 1.f, 0.f, 1, -0.2f, 0.f);
	Far(P3D("Balcony_B"), 86.f, 88.f, 3, 2, 1.f, 1.f, 0.f, -1, -0.2f, 0.f);
	Far(P3D("Balcony_A"), 86.f, 88.f, 3, 0, 1.f, 1.f, 0.f, 1, -0.2f, 0.f);
	Far(P3D("Judges_A"), 86.f, 88.f, 3, 1, 1.f, 1.f, 0.f, -1, -0.2f, 0.f);

Zone(TEXT("People p3d: under Black diamond's corkscrew"), 5.225f, 5.4f);     // Far rules, on the land 86 to 88 m out (it lies 0.39 m over the road's level there)
	Far(P3D("Judges_A"), 86.f, 88.f, 3, 2, 1.f, 1.f, 0.f, 1, -0.2f, 0.f);
	Far(P3D("Balcony_B"), 86.f, 88.f, 3, 0, 1.f, 1.f, 0.f, -1, -0.2f, 0.f);
	Far(P3D("Balcony_A"), 86.f, 88.f, 3, 1, 1.f, 1.f, 0.f, 1, -0.2f, 0.f);
	Far(P3D("Judges_A"), 86.f, 88.f, 3, 2, 1.f, 1.f, 0.f, -1, -0.2f, 0.f);

Zone(TEXT("People p3d: under the whiteout's loop"), 6.87f, 7.155f);     // Far rules, on the land 86 to 88 m out (it lies 0.39 m over the road's level there)
	Far(P3D("Judges_A"), 86.f, 88.f, 3, 0, 1.f, 1.f, 0.f, 1, -0.2f, 0.f);
	Far(P3D("Balcony_B"), 86.f, 88.f, 3, 1, 1.f, 1.f, 0.f, -1, -0.2f, 0.f);
	Far(P3D("Balcony_A"), 86.f, 88.f, 3, 2, 1.f, 1.f, 0.f, 1, -0.2f, 0.f);
	Far(P3D("Judges_A"), 86.f, 88.f, 3, 0, 1.f, 1.f, 0.f, -1, -0.2f, 0.f);

Zone(TEXT("People p3d: under the valley's corkscrew"), 7.215f, 7.385f);     // Far rules, on the land 86 to 88 m out (it lies 0.39 m over the road's level there)
	Far(P3D("Judges_A"), 86.f, 88.f, 3, 1, 1.f, 1.f, 0.f, 1, -0.2f, 0.f);
	Far(P3D("Balcony_B"), 86.f, 88.f, 3, 2, 1.f, 1.f, 0.f, -1, -0.2f, 0.f);
	Far(P3D("Balcony_A"), 86.f, 88.f, 3, 0, 1.f, 1.f, 0.f, 1, -0.2f, 0.f);
	Far(P3D("Judges_A"), 86.f, 88.f, 3, 1, 1.f, 1.f, 0.f, -1, -0.2f, 0.f);

#undef P3D
// ---- END p3d ----
}

{
// ---- BEGIN p3e (far and sky, ft_people_ice_p3e.py) ----
// (Fix round, 16 Nov 2026: after two reviews rules of this block were moved, re-phased or taken out; the counts in this header are the
// builder's. What changed and why: SourceArt/Integration/people/L5_ice_p3.md, "Review and fix round".)
// ICE MOUNTAIN'S FAR DISTANCE AND ITS SKY, pass 3 (meshes /Game/People/IceMountain/SM_P3E_..., all saved with the
// always-moving master). Nothing of this block is nearer than 150 m to the centre line: every piece is TALL (15 to
// 110 m as built). Nothing glows, nothing blinks. Every piece is put down by a Far rule, so it stays beside the
// loops, corkscrews and pipes; a ground piece's drop is the land's own height at its distance, negative (the land is
// 0.45 m under the road's ground at 80 m and rises 12 cm a metre: FTDressL5.inl), and every ground piece has a foot
// 7 to 9 m under its z = 0 and carries its people on a deck well above it, because the hill falls 7 in 100 along the
// road and a piece stands level. Ground pieces are left out of the three jumps (2.10-2.50, 4.83-5.21, 7.99-8.375 km:
// the land dips 18 m there); the sky and the edge are not.
// Where things go, across (metres from the centre line; drop):
//   150-200  choughs swirling (25 to 50 m up; -12)            160-240  hang gliders wheeling (35 to 85 m up; -12)
//   174-180  the rink on its terrace (-11)                    186-192  snow-kiters on their bench, kites 30 m up (-12.5)
//   190-280  hot-air balloons, three (-5)                     214-220  the crowd on the terraced bank (-15.8)
//   230-320  hot-air balloons, two great ones (-10)           240-246  the gallery doing a wave (-19)
//   240-290  geese in a V along the road (45 to 60 m up; -22) 270-280  the snowmobile convoy on its causeway (-22.8)
//   302-312  ski tourers on a snow crest past the land's edge (x1.2; -24), in the slots the far kit's 305-330 m pieces leave free
// Slots: a rule fires where the slot's number is K modulo its every (16 or 32); each mesh has its own K a side, the
// right side half a round after the left. Resort districts have the whole cast; the glaciers (ice walls at 240 m hide
// the slopes) have the sky, the edge and kites; the ridges have the sky, the edge, kites and the convoy.
// Estimated count (553 slots of 45 m, chance 1 everywhere): 393 pieces. By mesh: Balloons_A 30, Balloons_B 24,
// Choughs_A 43, FarCrowd_A 28, FarCrowd_B 28, Geese_A 39, HangGliders_A 35, RidgeTrek_A 38, Skaters_A 28,
// SkidooTrain_A 39, SnowKites_A 28, SnowKites_B 33 (none over 60).
#define P3E(n) TEXT("/Game/People/IceMountain/SM_P3E_" n)
Zone(TEXT("People p3e: the top and the first pipe, the sky and the edge"), 0.f, 2.429f);
	Far(P3E("Balloons_A"), 190.f, 280.f, 16, 4, 1.f, 1.f, 0.12f, -1, -5.f, 180.f);
	Far(P3E("Balloons_A"), 190.f, 280.f, 16, 12, 1.f, 1.f, 0.12f, 1, -5.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 18, 1.f, 1.f, 0.1f, -1, -12.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 2, 1.f, 1.f, 0.1f, 1, -12.f, 180.f);
	Far(P3E("Balloons_B"), 230.f, 320.f, 32, 2, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("Balloons_B"), 230.f, 320.f, 32, 18, 1.f, 1.f, 0.12f, 1, -10.f, 180.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 32, 26, 1.f, 1.f, 0.15f, -1, -12.f, 180.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 32, 10, 1.f, 1.f, 0.15f, 1, -12.f, 180.f);
Zone(TEXT("People p3e: the top and the first pipe, the slopes"), 0.f, 2.1f);
	Far(P3E("Skaters_A"), 174.f, 180.f, 16, 15, 1.f, 1.f, 0.f, -1, -11.f, 5.f);
	Far(P3E("Skaters_A"), 174.f, 180.f, 16, 7, 1.f, 1.f, 0.f, 1, -11.f, 5.f);
	Far(P3E("SnowKites_A"), 186.f, 192.f, 16, 13, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_A"), 186.f, 192.f, 16, 5, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
	Far(P3E("FarCrowd_A"), 214.f, 220.f, 16, 11, 1.f, 1.f, 0.f, -1, -15.8f, 5.f);
	Far(P3E("FarCrowd_A"), 214.f, 220.f, 16, 3, 1.f, 1.f, 0.f, 1, -15.8f, 5.f);
	Far(P3E("FarCrowd_B"), 240.f, 246.f, 16, 9, 1.f, 1.f, 0.f, -1, -19.f, 5.f);
	Far(P3E("FarCrowd_B"), 240.f, 246.f, 16, 1, 1.f, 1.f, 0.f, 1, -19.f, 5.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 16, 6, 1.f, 1.f, 0.f, -1, -22.8f, 3.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 16, 14, 1.f, 1.f, 0.f, 1, -22.8f, 3.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 10, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 26, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
Zone(TEXT("People p3e: the chute, the sky and the edge"), 2.43f, 2.959f);
	Far(P3E("Geese_A"), 240.f, 290.f, 16, 15, 1.f, 1.f, 0.1f, -1, -22.f, 6.f);
	Far(P3E("Geese_A"), 240.f, 290.f, 16, 7, 1.f, 1.f, 0.1f, 1, -22.f, 6.f);
	Far(P3E("RidgeTrek_A"), 302.f, 312.f, 16, 12, 1.f, 1.2f, 0.f, -1, -24.f, 5.f);
	Far(P3E("RidgeTrek_A"), 302.f, 312.f, 16, 8, 1.f, 1.2f, 0.f, 1, -24.f, 5.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 32, 27, 1.f, 1.f, 0.15f, -1, -12.f, 180.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 32, 11, 1.f, 1.f, 0.15f, 1, -12.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 25, 1.f, 1.f, 0.1f, -1, -12.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 9, 1.f, 1.f, 0.1f, 1, -12.f, 180.f);
	Far(P3E("Balloons_A"), 190.f, 280.f, 8, 4, 1.f, 1.f, 0.12f, -1, -5.f, 180.f);     // fix round: the sky over the pipe walls
	Far(P3E("Balloons_A"), 190.f, 280.f, 8, 0, 1.f, 1.f, 0.12f, 1, -5.f, 180.f);
Zone(TEXT("People p3e: the chute, the slopes"), 2.5f, 2.959f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 18, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 2, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 32, 22, 1.f, 1.f, 0.f, -1, -22.8f, 3.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 32, 6, 1.f, 1.f, 0.f, 1, -22.8f, 3.f);
	Far(P3E("SnowKites_A"), 186.f, 192.f, 16, 10, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);     // fix round: kites over the walls
	Far(P3E("SnowKites_A"), 186.f, 192.f, 16, 6, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
Zone(TEXT("People p3e: the super pipe, the sky and the edge"), 2.96f, 4.019f);
	Far(P3E("Geese_A"), 240.f, 290.f, 16, 15, 1.f, 1.f, 0.1f, -1, -22.f, 6.f);
	Far(P3E("Geese_A"), 240.f, 290.f, 16, 7, 1.f, 1.f, 0.1f, 1, -22.f, 6.f);

	Far(P3E("RidgeTrek_A"), 302.f, 312.f, 16, 12, 1.f, 1.2f, 0.f, -1, -24.f, 5.f);
	Far(P3E("RidgeTrek_A"), 302.f, 312.f, 16, 8, 1.f, 1.2f, 0.f, 1, -24.f, 5.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 25, 1.f, 1.f, 0.1f, -1, -12.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 9, 1.f, 1.f, 0.1f, 1, -12.f, 180.f);
	Far(P3E("Balloons_B"), 230.f, 320.f, 32, 9, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("Balloons_B"), 230.f, 320.f, 32, 25, 1.f, 1.f, 0.12f, 1, -10.f, 180.f);
	Far(P3E("Balloons_A"), 190.f, 280.f, 8, 4, 1.f, 1.f, 0.12f, -1, -5.f, 180.f);     // fix round: the sky over the pipe walls
	Far(P3E("Balloons_A"), 190.f, 280.f, 8, 0, 1.f, 1.f, 0.12f, 1, -5.f, 180.f);
Zone(TEXT("People p3e: the super pipe, the slopes"), 2.96f, 4.019f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 18, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 2, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
	Far(P3E("SnowKites_A"), 186.f, 192.f, 16, 10, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);     // fix round: kites over the walls
	Far(P3E("SnowKites_A"), 186.f, 192.f, 16, 6, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
Zone(TEXT("People p3e: Black diamond, the sky and the edge"), 4.02f, 5.649f);
	Far(P3E("Geese_A"), 240.f, 290.f, 16, 15, 1.f, 1.f, 0.1f, -1, -22.f, 6.f);
	Far(P3E("Geese_A"), 240.f, 290.f, 16, 7, 1.f, 1.f, 0.1f, 1, -22.f, 6.f);
	Far(P3E("RidgeTrek_A"), 302.f, 312.f, 16, 12, 1.f, 1.2f, 0.f, -1, -24.f, 5.f);
	Far(P3E("RidgeTrek_A"), 302.f, 312.f, 16, 8, 1.f, 1.2f, 0.f, 1, -24.f, 5.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 32, 27, 1.f, 1.f, 0.15f, -1, -12.f, 180.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 32, 11, 1.f, 1.f, 0.15f, 1, -12.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 25, 1.f, 1.f, 0.1f, -1, -12.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 9, 1.f, 1.f, 0.1f, 1, -12.f, 180.f);
Zone(TEXT("People p3e: Black diamond, the slopes (1)"), 4.02f, 4.83f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 18, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 2, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 32, 22, 1.f, 1.f, 0.f, -1, -22.8f, 3.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 32, 6, 1.f, 1.f, 0.f, 1, -22.8f, 3.f);
Zone(TEXT("People p3e: Black diamond, the slopes (2)"), 5.21f, 5.649f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 18, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 2, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 32, 22, 1.f, 1.f, 0.f, -1, -22.8f, 3.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 32, 6, 1.f, 1.f, 0.f, 1, -22.8f, 3.f);
Zone(TEXT("People p3e: the whiteout, the sky and the edge"), 5.65f, 7.299f);
	Far(P3E("Balloons_A"), 190.f, 280.f, 16, 4, 1.f, 1.f, 0.12f, -1, -5.f, 180.f);
	Far(P3E("Balloons_A"), 190.f, 280.f, 16, 12, 1.f, 1.f, 0.12f, 1, -5.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 18, 1.f, 1.f, 0.1f, -1, -12.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 2, 1.f, 1.f, 0.1f, 1, -12.f, 180.f);
	Far(P3E("Balloons_B"), 230.f, 320.f, 32, 2, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("Balloons_B"), 230.f, 320.f, 32, 18, 1.f, 1.f, 0.12f, 1, -10.f, 180.f);

Zone(TEXT("People p3e: the whiteout, the slopes"), 5.65f, 7.299f);
	Far(P3E("Skaters_A"), 174.f, 180.f, 16, 15, 1.f, 1.f, 0.f, -1, -11.f, 5.f);
	Far(P3E("Skaters_A"), 174.f, 180.f, 16, 7, 1.f, 1.f, 0.f, 1, -11.f, 5.f);
	Far(P3E("SnowKites_A"), 186.f, 192.f, 16, 13, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_A"), 186.f, 192.f, 16, 5, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
	Far(P3E("FarCrowd_A"), 214.f, 220.f, 16, 11, 1.f, 1.f, 0.f, -1, -15.8f, 5.f);
	Far(P3E("FarCrowd_A"), 214.f, 220.f, 16, 3, 1.f, 1.f, 0.f, 1, -15.8f, 5.f);
	Far(P3E("FarCrowd_B"), 240.f, 246.f, 16, 9, 1.f, 1.f, 0.f, -1, -19.f, 5.f);
	Far(P3E("FarCrowd_B"), 240.f, 246.f, 16, 1, 1.f, 1.f, 0.f, 1, -19.f, 5.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 16, 6, 1.f, 1.f, 0.f, -1, -22.8f, 3.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 16, 14, 1.f, 1.f, 0.f, 1, -22.8f, 3.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 10, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 26, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
Zone(TEXT("People p3e: the valley, the sky and the edge"), 7.3f, 8.939f);
	Far(P3E("Balloons_A"), 190.f, 280.f, 16, 4, 1.f, 1.f, 0.12f, -1, -5.f, 180.f);
	Far(P3E("Balloons_A"), 190.f, 280.f, 16, 12, 1.f, 1.f, 0.12f, 1, -5.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 18, 1.f, 1.f, 0.1f, -1, -12.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 2, 1.f, 1.f, 0.1f, 1, -12.f, 180.f);
	Far(P3E("Balloons_B"), 230.f, 320.f, 32, 2, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("Balloons_B"), 230.f, 320.f, 32, 18, 1.f, 1.f, 0.12f, 1, -10.f, 180.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 32, 26, 1.f, 1.f, 0.15f, -1, -12.f, 180.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 32, 10, 1.f, 1.f, 0.15f, 1, -12.f, 180.f);
Zone(TEXT("People p3e: the valley, the slopes (1)"), 7.3f, 7.99f);
	Far(P3E("Skaters_A"), 174.f, 180.f, 16, 15, 1.f, 1.f, 0.f, -1, -11.f, 5.f);
	Far(P3E("Skaters_A"), 174.f, 180.f, 16, 7, 1.f, 1.f, 0.f, 1, -11.f, 5.f);
	Far(P3E("SnowKites_A"), 186.f, 192.f, 16, 13, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_A"), 186.f, 192.f, 16, 5, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
	Far(P3E("FarCrowd_A"), 214.f, 220.f, 16, 11, 1.f, 1.f, 0.f, -1, -15.8f, 5.f);
	Far(P3E("FarCrowd_A"), 214.f, 220.f, 16, 3, 1.f, 1.f, 0.f, 1, -15.8f, 5.f);
	Far(P3E("FarCrowd_B"), 240.f, 246.f, 16, 9, 1.f, 1.f, 0.f, -1, -19.f, 5.f);
	Far(P3E("FarCrowd_B"), 240.f, 246.f, 16, 1, 1.f, 1.f, 0.f, 1, -19.f, 5.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 16, 6, 1.f, 1.f, 0.f, -1, -22.8f, 3.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 16, 14, 1.f, 1.f, 0.f, 1, -22.8f, 3.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 10, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 26, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
Zone(TEXT("People p3e: the valley, the slopes (2)"), 8.375f, 8.939f);
	Far(P3E("Skaters_A"), 174.f, 180.f, 16, 15, 1.f, 1.f, 0.f, -1, -11.f, 5.f);
	Far(P3E("Skaters_A"), 174.f, 180.f, 16, 7, 1.f, 1.f, 0.f, 1, -11.f, 5.f);
	Far(P3E("SnowKites_A"), 186.f, 192.f, 16, 13, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_A"), 186.f, 192.f, 16, 5, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
	Far(P3E("FarCrowd_A"), 214.f, 220.f, 16, 11, 1.f, 1.f, 0.f, -1, -15.8f, 5.f);
	Far(P3E("FarCrowd_A"), 214.f, 220.f, 16, 3, 1.f, 1.f, 0.f, 1, -15.8f, 5.f);
	Far(P3E("FarCrowd_B"), 240.f, 246.f, 16, 9, 1.f, 1.f, 0.f, -1, -19.f, 5.f);
	Far(P3E("FarCrowd_B"), 240.f, 246.f, 16, 1, 1.f, 1.f, 0.f, 1, -19.f, 5.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 16, 6, 1.f, 1.f, 0.f, -1, -22.8f, 3.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 16, 14, 1.f, 1.f, 0.f, 1, -22.8f, 3.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 10, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 26, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
Zone(TEXT("People p3e: the Abominable's glacier, the sky and the edge"), 8.94f, 11.599f);
	Far(P3E("Geese_A"), 240.f, 290.f, 16, 15, 1.f, 1.f, 0.1f, -1, -22.f, 6.f);
	Far(P3E("Geese_A"), 240.f, 290.f, 16, 7, 1.f, 1.f, 0.1f, 1, -22.f, 6.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 16, 11, 1.f, 1.f, 0.15f, -1, -12.f, 180.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 16, 3, 1.f, 1.f, 0.15f, 1, -12.f, 180.f);
	Far(P3E("RidgeTrek_A"), 302.f, 312.f, 16, 12, 1.f, 1.2f, 0.f, -1, -24.f, 5.f);
	Far(P3E("RidgeTrek_A"), 302.f, 312.f, 16, 8, 1.f, 1.2f, 0.f, 1, -24.f, 5.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 25, 1.f, 1.f, 0.1f, -1, -12.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 9, 1.f, 1.f, 0.1f, 1, -12.f, 180.f);
	Far(P3E("Balloons_B"), 230.f, 320.f, 32, 9, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("Balloons_B"), 230.f, 320.f, 32, 25, 1.f, 1.f, 0.12f, 1, -10.f, 180.f);
Zone(TEXT("People p3e: the Abominable's glacier, the slopes"), 8.94f, 11.599f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 18, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 2, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
Zone(TEXT("People p3e: the white ridge, the sky and the edge"), 11.6f, 14.299f);
	Far(P3E("Geese_A"), 240.f, 290.f, 16, 15, 1.f, 1.f, 0.1f, -1, -22.f, 6.f);
	Far(P3E("Geese_A"), 240.f, 290.f, 16, 7, 1.f, 1.f, 0.1f, 1, -22.f, 6.f);
	Far(P3E("RidgeTrek_A"), 302.f, 312.f, 16, 12, 1.f, 1.2f, 0.f, -1, -24.f, 5.f);
	Far(P3E("RidgeTrek_A"), 302.f, 312.f, 16, 8, 1.f, 1.2f, 0.f, 1, -24.f, 5.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 32, 27, 1.f, 1.f, 0.15f, -1, -12.f, 180.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 32, 11, 1.f, 1.f, 0.15f, 1, -12.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 25, 1.f, 1.f, 0.1f, -1, -12.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 9, 1.f, 1.f, 0.1f, 1, -12.f, 180.f);
Zone(TEXT("People p3e: the white ridge, the slopes"), 11.6f, 14.299f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 18, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 2, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 32, 22, 1.f, 1.f, 0.f, -1, -22.8f, 3.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 32, 6, 1.f, 1.f, 0.f, 1, -22.8f, 3.f);
Zone(TEXT("People p3e: the resort, the sky and the edge"), 14.3f, 16.999f);
	Far(P3E("Balloons_A"), 190.f, 280.f, 16, 4, 1.f, 1.f, 0.12f, -1, -5.f, 180.f);
	Far(P3E("Balloons_A"), 190.f, 280.f, 16, 12, 1.f, 1.f, 0.12f, 1, -5.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 18, 1.f, 1.f, 0.1f, -1, -12.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 2, 1.f, 1.f, 0.1f, 1, -12.f, 180.f);
	Far(P3E("Balloons_B"), 230.f, 320.f, 32, 2, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("Balloons_B"), 230.f, 320.f, 32, 18, 1.f, 1.f, 0.12f, 1, -10.f, 180.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 32, 26, 1.f, 1.f, 0.15f, -1, -12.f, 180.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 32, 10, 1.f, 1.f, 0.15f, 1, -12.f, 180.f);
Zone(TEXT("People p3e: the resort, the slopes"), 14.3f, 16.999f);
	Far(P3E("Skaters_A"), 174.f, 180.f, 16, 15, 1.f, 1.f, 0.f, -1, -11.f, 5.f);
	Far(P3E("Skaters_A"), 174.f, 180.f, 16, 7, 1.f, 1.f, 0.f, 1, -11.f, 5.f);
	Far(P3E("SnowKites_A"), 186.f, 192.f, 16, 13, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_A"), 186.f, 192.f, 16, 5, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
	Far(P3E("FarCrowd_A"), 214.f, 220.f, 16, 11, 1.f, 1.f, 0.f, -1, -15.8f, 5.f);
	Far(P3E("FarCrowd_A"), 214.f, 220.f, 16, 3, 1.f, 1.f, 0.f, 1, -15.8f, 5.f);
	Far(P3E("FarCrowd_B"), 240.f, 246.f, 16, 9, 1.f, 1.f, 0.f, -1, -19.f, 5.f);
	Far(P3E("FarCrowd_B"), 240.f, 246.f, 16, 1, 1.f, 1.f, 0.f, 1, -19.f, 5.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 16, 6, 1.f, 1.f, 0.f, -1, -22.8f, 3.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 16, 14, 1.f, 1.f, 0.f, 1, -22.8f, 3.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 10, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 26, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
Zone(TEXT("People p3e: the ice again, the sky and the edge"), 17.f, 19.699f);
	Far(P3E("Geese_A"), 240.f, 290.f, 16, 15, 1.f, 1.f, 0.1f, -1, -22.f, 6.f);
	Far(P3E("Geese_A"), 240.f, 290.f, 16, 7, 1.f, 1.f, 0.1f, 1, -22.f, 6.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 16, 11, 1.f, 1.f, 0.15f, -1, -12.f, 180.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 16, 3, 1.f, 1.f, 0.15f, 1, -12.f, 180.f);
	Far(P3E("RidgeTrek_A"), 302.f, 312.f, 16, 12, 1.f, 1.2f, 0.f, -1, -24.f, 5.f);
	Far(P3E("RidgeTrek_A"), 302.f, 312.f, 16, 8, 1.f, 1.2f, 0.f, 1, -24.f, 5.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 25, 1.f, 1.f, 0.1f, -1, -12.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 9, 1.f, 1.f, 0.1f, 1, -12.f, 180.f);
	Far(P3E("Balloons_B"), 230.f, 320.f, 32, 9, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("Balloons_B"), 230.f, 320.f, 32, 25, 1.f, 1.f, 0.12f, 1, -10.f, 180.f);
Zone(TEXT("People p3e: the ice again, the slopes"), 17.f, 19.699f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 18, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 2, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
Zone(TEXT("People p3e: the high ridge, the sky and the edge"), 19.7f, 22.399f);
	Far(P3E("Geese_A"), 240.f, 290.f, 16, 15, 1.f, 1.f, 0.1f, -1, -22.f, 6.f);
	Far(P3E("Geese_A"), 240.f, 290.f, 16, 7, 1.f, 1.f, 0.1f, 1, -22.f, 6.f);
	Far(P3E("RidgeTrek_A"), 302.f, 312.f, 16, 12, 1.f, 1.2f, 0.f, -1, -24.f, 5.f);
	Far(P3E("RidgeTrek_A"), 302.f, 312.f, 16, 8, 1.f, 1.2f, 0.f, 1, -24.f, 5.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 32, 27, 1.f, 1.f, 0.15f, -1, -12.f, 180.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 32, 11, 1.f, 1.f, 0.15f, 1, -12.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 25, 1.f, 1.f, 0.1f, -1, -12.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 9, 1.f, 1.f, 0.1f, 1, -12.f, 180.f);
Zone(TEXT("People p3e: the high ridge, the slopes"), 19.7f, 22.399f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 18, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 2, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 32, 22, 1.f, 1.f, 0.f, -1, -22.8f, 3.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 32, 6, 1.f, 1.f, 0.f, 1, -22.8f, 3.f);
Zone(TEXT("People p3e: the last lifts, the sky and the edge"), 22.4f, 98.999f);
	Far(P3E("Balloons_A"), 190.f, 280.f, 16, 4, 1.f, 1.f, 0.12f, -1, -5.f, 180.f);
	Far(P3E("Balloons_A"), 190.f, 280.f, 16, 12, 1.f, 1.f, 0.12f, 1, -5.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 18, 1.f, 1.f, 0.1f, -1, -12.f, 180.f);
	Far(P3E("HangGliders_A"), 160.f, 240.f, 32, 2, 1.f, 1.f, 0.1f, 1, -12.f, 180.f);
	Far(P3E("Balloons_B"), 230.f, 320.f, 32, 2, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("Balloons_B"), 230.f, 320.f, 32, 18, 1.f, 1.f, 0.12f, 1, -10.f, 180.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 32, 26, 1.f, 1.f, 0.15f, -1, -12.f, 180.f);
	Far(P3E("Choughs_A"), 150.f, 200.f, 32, 10, 1.f, 1.f, 0.15f, 1, -12.f, 180.f);
Zone(TEXT("People p3e: the last lifts, the slopes"), 22.4f, 98.999f);
	Far(P3E("Skaters_A"), 174.f, 180.f, 16, 15, 1.f, 1.f, 0.f, -1, -11.f, 5.f);
	Far(P3E("Skaters_A"), 174.f, 180.f, 16, 7, 1.f, 1.f, 0.f, 1, -11.f, 5.f);
	Far(P3E("SnowKites_A"), 186.f, 192.f, 16, 13, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_A"), 186.f, 192.f, 16, 5, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
	Far(P3E("FarCrowd_A"), 214.f, 220.f, 16, 11, 1.f, 1.f, 0.f, -1, -15.8f, 5.f);
	Far(P3E("FarCrowd_A"), 214.f, 220.f, 16, 3, 1.f, 1.f, 0.f, 1, -15.8f, 5.f);
	Far(P3E("FarCrowd_B"), 240.f, 246.f, 16, 9, 1.f, 1.f, 0.f, -1, -19.f, 5.f);
	Far(P3E("FarCrowd_B"), 240.f, 246.f, 16, 1, 1.f, 1.f, 0.f, 1, -19.f, 5.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 16, 6, 1.f, 1.f, 0.f, -1, -22.8f, 3.f);
	Far(P3E("SkidooTrain_A"), 270.f, 280.f, 16, 14, 1.f, 1.f, 0.f, 1, -22.8f, 3.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 10, 1.f, 1.f, 0.f, -1, -12.5f, 4.f);
	Far(P3E("SnowKites_B"), 186.f, 192.f, 32, 26, 1.f, 1.f, 0.f, 1, -12.5f, 4.f);
#undef P3E
// ---- END p3e ----
}

{
// ---- BEGIN p3f (film shoot, ft_people_ice_p3f.py) ----
// (Fix round, 16 Nov 2026: after two reviews rules of this block were moved, re-phased or taken out; the counts in this header are the
// builder's. What changed and why: SourceArt/Integration/people/L5_ice_p3.md, "Review and fix round".)
// More of the shoot on the mountain: eleven new units (SM_P3F_*: two second-unit camera positions, two drone teams, the
// wire rig, two groups of extras, the make-up tent, the press pen, a cable camera, the snow unit with its wind machine)
// and more of the crew kit in wardrobe Snow, in clusters of two or three side by side behind their own tape.
// Written by C:/ftwork/jobs/L5p3f/make_inl.py: one Site on one 45 m slot (a zone a slot, as the first pass wrote its
// own short zones), so the count is exact: 130 sites, 353 pieces before the track leaves any out.
//   - Every set is laid 1.3 times its built size, like the first pass units (life size inks to a scribble here), so a
//     part offset comes out 1.3 times as long. All but one of the new pieces are TALL, and all are treated as tall: no
//     anchor is nearer than 30 m (by the sizes the generator logged, a turned piece then reaches no nearer than about
//     25 m; not checked in the game). R sets turn 15 degrees towards the
//     car coming (yaw -15), L sets +15.
//   - 30 m where the dressing writes its verge (the land is level there): 49 sites; 33 m where a banner mast of the
//     dressing stands on the slot (SM_PoleFeather, 26 to 28 m out): 29 sites. Beside the banked bends and the
//     pipes, where whatever stands 17 to 40 m out floats over the cutting, the same sets at 46 m: 52 sites.
//   - A slot and side is used only if no Scatter, Site or Far of FTDressL5.inl or FTPeopleL5.inl has a piece on it within
//     about 4 m of the set (stands, decks, climbs, watchers, the first pass crew, chalet rows, ice walls, snow lances,
//     galleries); never two of these sites on neighbouring slots of one side; none in a loop, a corkscrew, a jump from
//     run-up to landing, or the chute. The drones hover or circle 31 m out and more, 7 m up and more.
//   - The snow unit (white puffs of snow down the wind) is not placed on the road of the Abominable (8.94 km on): he
//     throws snowballs there. Nothing glows but the steady lamps of the kit (SM_CrewLights, 30 m out and more); nothing blinks.
#define P3F(n) TEXT("/Game/People/IceMountain/SM_P3F_" n)
#define P3FCREW(n) TEXT("/Game/People/Crew/SM_Crew" n "_Snow")
Set(TEXT("PplP3FTowerR"), {      // a camera tower, lights, crew applauding
	{ P3F("SecondUnit_A"), -14, 0, 0, -15, 1 },
	{ P3FCREW("Lights"), 0, 1, 0, -15, 1 },
	{ P3FCREW("ApplauseA"), 14, 0, 0, -15, 1 },
});
Set(TEXT("PplP3FTowerL"), {      // a camera tower, lights, crew applauding
	{ P3F("SecondUnit_A"), -14, 0, 0, 15, 1 },
	{ P3FCREW("Lights"), 0, 1, 0, 15, 1 },
	{ P3FCREW("ApplauseA"), 14, 0, 0, 15, 1 },
});
Set(TEXT("PplP3FLensR"), {      // the long lens and the press pen
	{ P3F("SecondUnit_B"), -7, 0, 0, -15, 1 },
	{ P3F("PressPen"), 7, 0, 0, -15, 1 },
});
Set(TEXT("PplP3FLensL"), {      // the long lens and the press pen
	{ P3F("SecondUnit_B"), -7, 0, 0, 15, 1 },
	{ P3F("PressPen"), 7, 0, 0, 15, 1 },
});
Set(TEXT("PplP3FDroneAR"), {      // a drone over its pad, the village, wardrobe
	{ P3F("DroneTeam_A"), -14, 2, 0, -15, 1 },
	{ P3FCREW("Village"), 0, 0, 0, -15, 1 },
	{ P3F("Extras_B"), 14, 0, 0, -15, 1 },
});
Set(TEXT("PplP3FDroneAL"), {      // a drone over its pad, the village, wardrobe
	{ P3F("DroneTeam_A"), -14, 2, 0, 15, 1 },
	{ P3FCREW("Village"), 0, 0, 0, 15, 1 },
	{ P3F("Extras_B"), 14, 0, 0, 15, 1 },
});
Set(TEXT("PplP3FDroneBR"), {      // two drones round their mast, a camera, the press
	{ P3F("DroneTeam_B"), -14, 4, 0, -15, 1 },
	{ P3FCREW("Camera"), 0, 0, 0, -15, 1 },
	{ P3F("PressPen"), 14, 0, 0, -15, 1 },
});
Set(TEXT("PplP3FDroneBL"), {      // two drones round their mast, a camera, the press
	{ P3F("DroneTeam_B"), -14, 4, 0, 15, 1 },
	{ P3FCREW("Camera"), 0, 0, 0, 15, 1 },
	{ P3F("PressPen"), 14, 0, 0, 15, 1 },
});
Set(TEXT("PplP3FStuntR"), {      // the wire rig, the stunt team, sound
	{ P3F("StuntRig"), -14, 0, 0, -15, 1 },
	{ P3FCREW("Stunt"), 0, 0, 0, -15, 1 },
	{ P3FCREW("Sound"), 14, 1, 0, -15, 1 },
});
Set(TEXT("PplP3FStuntL"), {      // the wire rig, the stunt team, sound
	{ P3F("StuntRig"), -14, 0, 0, 15, 1 },
	{ P3FCREW("Stunt"), 0, 0, 0, 15, 1 },
	{ P3FCREW("Sound"), 14, 1, 0, 15, 1 },
});
Set(TEXT("PplP3FMakeUpR"), {      // the make-up tent, the extras, catering
	{ P3F("MakeUp"), -14, 0, 0, -15, 1 },
	{ P3F("Extras_A"), 0, 0, 0, -15, 1 },
	{ P3FCREW("Catering"), 14, 1, 0, -15, 1 },
});
Set(TEXT("PplP3FMakeUpL"), {      // the make-up tent, the extras, catering
	{ P3F("MakeUp"), -14, 0, 0, 15, 1 },
	{ P3F("Extras_A"), 0, 0, 0, 15, 1 },
	{ P3FCREW("Catering"), 14, 1, 0, 15, 1 },
});
Set(TEXT("PplP3FCableR"), {      // the cable camera and the crane
	{ P3F("CableCam"), -6, 0, 0, -15, 1 },
	{ P3FCREW("Crane"), 8, 3, 0, -15, 1 },
});
Set(TEXT("PplP3FCableL"), {      // the cable camera and the crane
	{ P3F("CableCam"), -6, 0, 0, 15, 1 },
	{ P3FCREW("Crane"), 8, 3, 0, 15, 1 },
});
Set(TEXT("PplP3FWindR"), {      // the snow unit, crew applauding, the long lens
	{ P3F("WindMachine"), -14, 0, 0, -15, 1 },
	{ P3FCREW("ApplauseB"), 0, 0, 0, -15, 1 },
	{ P3F("SecondUnit_B"), 14, 0, 0, -15, 1 },
});
Set(TEXT("PplP3FWindL"), {      // the snow unit, crew applauding, the long lens
	{ P3F("WindMachine"), -14, 0, 0, 15, 1 },
	{ P3FCREW("ApplauseB"), 0, 0, 0, 15, 1 },
	{ P3F("SecondUnit_B"), 14, 0, 0, 15, 1 },
});
Set(TEXT("PplP3FHoldR"), {      // the extras, a camera, the make-up tent
	{ P3F("Extras_A"), -14, 0, 0, -15, 1 },
	{ P3FCREW("Camera"), 0, 0, 0, -15, 1 },
	{ P3F("MakeUp"), 14, 0, 0, -15, 1 },
});
Set(TEXT("PplP3FHoldL"), {      // the extras, a camera, the make-up tent
	{ P3F("Extras_A"), -14, 0, 0, 15, 1 },
	{ P3FCREW("Camera"), 0, 0, 0, 15, 1 },
	{ P3F("MakeUp"), 14, 0, 0, 15, 1 },
});
Set(TEXT("PplP3FRigR"), {      // the wire rig and a drone over its pad
	{ P3F("StuntRig"), -7, 0, 0, -15, 1 },
	{ P3F("DroneTeam_A"), 7, 2, 0, -15, 1 },
});
Set(TEXT("PplP3FRigL"), {      // the wire rig and a drone over its pad
	{ P3F("StuntRig"), -7, 0, 0, 15, 1 },
	{ P3F("DroneTeam_A"), 7, 2, 0, 15, 1 },
});
Set(TEXT("PplP3FQueueR"), {      // wardrobe, crew applauding, the long lens
	{ P3F("Extras_B"), -14, 0, 0, -15, 1 },
	{ P3FCREW("ApplauseB"), 0, 0, 0, -15, 1 },
	{ P3F("SecondUnit_B"), 14, 0, 0, -15, 1 },
});
Set(TEXT("PplP3FQueueL"), {      // wardrobe, crew applauding, the long lens
	{ P3F("Extras_B"), -14, 0, 0, 15, 1 },
	{ P3FCREW("ApplauseB"), 0, 0, 0, 15, 1 },
	{ P3F("SecondUnit_B"), 14, 0, 0, 15, 1 },
});
Zone(TEXT("People p3f: First tracks, slot 5"), 0.20359f, 0.24884f);
	Site(TEXT("PplP3FMakeUpL"), -1, 52.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: First tracks, slot 7"), 0.29408f, 0.33932f);
	Site(TEXT("PplP3FTowerR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: First tracks, slot 11"), 0.47505f, 0.52029f);
	Site(TEXT("PplP3FLensR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: First tracks, slot 11"), 0.47505f, 0.52029f);
	Site(TEXT("PplP3FCableL"), -1, 46.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: First tracks, slot 21"), 0.92748f, 0.97272f);
	Site(TEXT("PplP3FHoldL"), -1, 52.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: First tracks, slot 25"), 1.10845f, 1.1537f);
	Site(TEXT("PplP3FRigL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The half pipe, slot 40"), 1.7871f, 1.83234f);
	Site(TEXT("PplP3FStuntR"), 1, 52.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The half pipe, slot 40"), 1.7871f, 1.83234f);
	Site(TEXT("PplP3FQueueL"), -1, 52.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The half pipe, slot 44"), 1.96807f, 2.01331f);
	Site(TEXT("PplP3FMakeUpR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The half pipe, slot 44"), 1.96807f, 2.01331f);
	Site(TEXT("PplP3FTowerL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: Black diamond, slot 99"), 4.45644f, 4.50168f);
	Site(TEXT("PplP3FLensL"), -1, 46.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: fix round, slot 124"), 5.604f, 5.616f);
	Site(TEXT("PplP3FHoldR"), 1, 46.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: The whiteout, slot 131"), 5.90421f, 5.94945f);
	Site(TEXT("PplP3FWindL"), -1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The whiteout, slot 133"), 5.9947f, 6.03994f);
	Site(TEXT("PplP3FLensR"), 1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The whiteout, slot 137"), 6.17567f, 6.22091f);
	Site(TEXT("PplP3FHoldL"), -1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The whiteout, slot 139"), 6.26616f, 6.3114f);
	Site(TEXT("PplP3FQueueR"), 1, 33.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: The whiteout, slot 145"), 6.53761f, 6.58286f);
	Site(TEXT("PplP3FTowerR"), 1, 46.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: The whiteout, slot 149"), 6.71859f, 6.76383f);
	Site(TEXT("PplP3FLensR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: fix round, slot 170"), 7.685f, 7.697f);
	Site(TEXT("PplP3FDroneAR"), 1, 46.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: fix round, slot 174"), 7.866f, 7.878f);
	Site(TEXT("PplP3FDroneBR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The valley run, slot 174"), 7.84966f, 7.8949f);
	Site(TEXT("PplP3FCableL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The valley run, slot 190"), 8.57355f, 8.61879f);
	Site(TEXT("PplP3FWindR"), 1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The valley run, slot 190"), 8.57355f, 8.61879f);
	Site(TEXT("PplP3FCableL"), -1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The glacier, slot 202"), 9.11646f, 9.16171f);
	Site(TEXT("PplP3FHoldR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The glacier, slot 202"), 9.11646f, 9.16171f);
	Site(TEXT("PplP3FQueueL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The glacier, slot 214"), 9.65938f, 9.70462f);
	Site(TEXT("PplP3FQueueR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The glacier, slot 226"), 10.2023f, 10.24754f);
	Site(TEXT("PplP3FLensR"), 1, 30.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The glacier, slot 226"), 10.2023f, 10.24754f);
	Site(TEXT("PplP3FLensL"), -1, 30.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The glacier, slot 238"), 10.74521f, 10.79046f);
	Site(TEXT("PplP3FHoldR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The glacier, slot 250"), 11.28813f, 11.33337f);
	Site(TEXT("PplP3FQueueR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The glacier, slot 250"), 11.28813f, 11.33337f);
	Site(TEXT("PplP3FHoldL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The white ridge, slot 257"), 11.60483f, 11.65007f);
	Site(TEXT("PplP3FStuntR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The white ridge, slot 258"), 11.65007f, 11.69532f);
	Site(TEXT("PplP3FTowerL"), -1, 30.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The white ridge, slot 263"), 11.87629f, 11.92153f);
	Site(TEXT("PplP3FMakeUpR"), 1, 30.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The white ridge, slot 264"), 11.92153f, 11.96677f);
	Site(TEXT("PplP3FQueueL"), -1, 52.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The white ridge, slot 269"), 12.14775f, 12.19299f);
	Site(TEXT("PplP3FCableR"), 1, 30.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: The white ridge, slot 275"), 12.4192f, 12.46445f);
	Site(TEXT("PplP3FLensR"), 1, 30.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: The white ridge, slot 281"), 12.69066f, 12.7359f);
	Site(TEXT("PplP3FTowerR"), 1, 30.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: The white ridge, slot 287"), 12.96212f, 13.00736f);
	Site(TEXT("PplP3FQueueR"), 1, 30.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The white ridge, slot 288"), 13.00736f, 13.05261f);
	Site(TEXT("PplP3FCableL"), -1, 30.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The white ridge, slot 293"), 13.23358f, 13.27882f);
	Site(TEXT("PplP3FHoldR"), 1, 30.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The white ridge, slot 294"), 13.27882f, 13.32406f);
	Site(TEXT("PplP3FQueueL"), -1, 52.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The white ridge, slot 299"), 13.50504f, 13.55028f);
	Site(TEXT("PplP3FLensR"), 1, 30.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The white ridge, slot 300"), 13.55028f, 13.59552f);
	Site(TEXT("PplP3FCableL"), -1, 30.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The white ridge, slot 305"), 13.77649f, 13.82174f);
	Site(TEXT("PplP3FHoldR"), 1, 30.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The white ridge, slot 306"), 13.82174f, 13.86698f);
	Site(TEXT("PplP3FLensL"), -1, 30.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The white ridge, slot 311"), 14.04795f, 14.09319f);
	Site(TEXT("PplP3FQueueR"), 1, 30.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: The resort, slot 319"), 14.4099f, 14.45514f);
	Site(TEXT("PplP3FTowerL"), -1, 46.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: The resort, slot 323"), 14.59087f, 14.63611f);
	Site(TEXT("PplP3FDroneAL"), -1, 52.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The resort, slot 327"), 14.77184f, 14.81708f);
	Site(TEXT("PplP3FDroneBL"), -1, 52.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The resort, slot 331"), 14.95281f, 14.99805f);
	Site(TEXT("PplP3FStuntL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The resort, slot 332"), 14.99805f, 15.0433f);
	Site(TEXT("PplP3FLensR"), 1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The resort, slot 336"), 15.17903f, 15.22427f);
	Site(TEXT("PplP3FQueueL"), -1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The resort, slot 338"), 15.26951f, 15.31476f);
	Site(TEXT("PplP3FMakeUpR"), 1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The resort, slot 340"), 15.36f, 15.40524f);
	Site(TEXT("PplP3FLensL"), -1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The resort, slot 344"), 15.54097f, 15.58621f);
	Site(TEXT("PplP3FLensR"), 1, 30.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The resort, slot 349"), 15.76719f, 15.81243f);
	Site(TEXT("PplP3FMakeUpL"), -1, 52.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: The resort, slot 357"), 16.12913f, 16.17437f);
	Site(TEXT("PplP3FCableL"), -1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The resort, slot 361"), 16.3101f, 16.35534f);
	Site(TEXT("PplP3FCableL"), -1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The resort, slot 362"), 16.35534f, 16.40059f);
	Site(TEXT("PplP3FCableR"), 1, 33.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: The resort, slot 369"), 16.67205f, 16.71729f);
	Site(TEXT("PplP3FTowerL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The resort, slot 373"), 16.85302f, 16.89826f);
	Site(TEXT("PplP3FStuntL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The ice again, slot 376"), 16.98875f, 17.03399f);
	Site(TEXT("PplP3FDroneAR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: fix round, slot 379"), 17.141f, 17.153f);
	Site(TEXT("PplP3FDroneAL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: fix round, slot 381"), 17.232f, 17.244f);
	Site(TEXT("PplP3FDroneBR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The ice again, slot 393"), 17.75788f, 17.80312f);
	Site(TEXT("PplP3FHoldR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The ice again, slot 393"), 17.75788f, 17.80312f);
	Site(TEXT("PplP3FQueueL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The ice again, slot 400"), 18.07458f, 18.11982f);
	Site(TEXT("PplP3FQueueR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: fix round, slot 401"), 18.136f, 18.148f);
	Site(TEXT("PplP3FDroneBL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: fix round, slot 405"), 18.317f, 18.329f);
	Site(TEXT("PplP3FStuntR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The ice again, slot 404"), 18.25555f, 18.30079f);
	Site(TEXT("PplP3FLensL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The ice again, slot 417"), 18.84371f, 18.88895f);
	Site(TEXT("PplP3FLensR"), 1, 30.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The ice again, slot 417"), 18.84371f, 18.88895f);
	Site(TEXT("PplP3FHoldL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The ice again, slot 424"), 19.16041f, 19.20565f);
	Site(TEXT("PplP3FMakeUpR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: fix round, slot 425"), 19.222f, 19.234f);
	Site(TEXT("PplP3FMakeUpL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The ice again, slot 428"), 19.34138f, 19.38663f);
	Site(TEXT("PplP3FRigR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The ice again, slot 428"), 19.34138f, 19.38663f);
	Site(TEXT("PplP3FRigL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The high ridge, slot 436"), 19.70333f, 19.74857f);
	Site(TEXT("PplP3FHoldR"), 1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The high ridge, slot 436"), 19.70333f, 19.74857f);
	Site(TEXT("PplP3FCableL"), -1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The high ridge, slot 442"), 19.97478f, 20.02003f);
	Site(TEXT("PplP3FCableR"), 1, 30.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The high ridge, slot 442"), 19.97478f, 20.02003f);
	Site(TEXT("PplP3FTowerL"), -1, 30.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: The high ridge, slot 454"), 20.5177f, 20.56294f);
	Site(TEXT("PplP3FDroneAR"), 1, 58.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The high ridge, slot 455"), 20.56294f, 20.60819f);
	Site(TEXT("PplP3FDroneAL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The high ridge, slot 460"), 20.78916f, 20.8344f);
	Site(TEXT("PplP3FHoldR"), 1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The high ridge, slot 462"), 20.87964f, 20.92489f);
	Site(TEXT("PplP3FQueueL"), -1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The high ridge, slot 466"), 21.06062f, 21.10586f);
	Site(TEXT("PplP3FQueueR"), 1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The high ridge, slot 468"), 21.1511f, 21.19635f);
	Site(TEXT("PplP3FLensL"), -1, 30.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: The high ridge, slot 475"), 21.4678f, 21.51305f);
	Site(TEXT("PplP3FDroneBL"), -1, 52.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The high ridge, slot 478"), 21.60353f, 21.64878f);
	Site(TEXT("PplP3FLensR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The high ridge, slot 481"), 21.73926f, 21.7845f);
	Site(TEXT("PplP3FHoldL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The high ridge, slot 484"), 21.87499f, 21.92023f);
	Site(TEXT("PplP3FMakeUpR"), 1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The high ridge, slot 488"), 22.05596f, 22.10121f);
	Site(TEXT("PplP3FQueueL"), -1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The high ridge, slot 490"), 22.14645f, 22.19169f);
	Site(TEXT("PplP3FLensR"), 1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The high ridge, slot 494"), 22.32742f, 22.37266f);
	Site(TEXT("PplP3FMakeUpL"), -1, 52.0f, 1, 0, 1.f, 1.3f);

Zone(TEXT("People p3f: The last lifts, slot 499"), 22.55364f, 22.59888f);
	Site(TEXT("PplP3FTowerL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The last lifts, slot 503"), 22.73461f, 22.77985f);
	Site(TEXT("PplP3FRigL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The last lifts, slot 507"), 22.91558f, 22.96082f);
	Site(TEXT("PplP3FCableL"), -1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The last lifts, slot 509"), 23.00607f, 23.05131f);
	Site(TEXT("PplP3FCableR"), 1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The last lifts, slot 513"), 23.18704f, 23.23228f);
	Site(TEXT("PplP3FStuntL"), -1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The last lifts, slot 514"), 23.23228f, 23.27752f);
	Site(TEXT("PplP3FTowerR"), 1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The last lifts, slot 517"), 23.36801f, 23.41325f);
	Site(TEXT("PplP3FQueueL"), -1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The last lifts, slot 518"), 23.41325f, 23.4585f);
	Site(TEXT("PplP3FQueueR"), 1, 46.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The last lifts, slot 523"), 23.63947f, 23.68471f);
	Site(TEXT("PplP3FLensL"), -1, 30.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The last lifts, slot 527"), 23.82044f, 23.86568f);
	Site(TEXT("PplP3FDroneBL"), -1, 52.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The last lifts, slot 531"), 24.00141f, 24.04665f);
	Site(TEXT("PplP3FHoldL"), -1, 52.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The last lifts, slot 533"), 24.0919f, 24.13714f);
	Site(TEXT("PplP3FHoldR"), 1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The last lifts, slot 535"), 24.18238f, 24.22763f);
	Site(TEXT("PplP3FQueueL"), -1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The last lifts, slot 540"), 24.4086f, 24.45384f);
	Site(TEXT("PplP3FLensL"), -1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The last lifts, slot 542"), 24.49908f, 24.54433f);
	Site(TEXT("PplP3FQueueR"), 1, 33.0f, 1, 0, 1.f, 1.3f);
Zone(TEXT("People p3f: The last lifts, slot 544"), 24.58957f, 24.63481f);
	Site(TEXT("PplP3FMakeUpL"), -1, 30.0f, 1, 0, 1.f, 1.3f);
#undef P3F
#undef P3FCREW
// ---- END p3f ----
}
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People5 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1d1 (district 1 "First tracks", the summit station, ft_world_ice_s1d1.py) ----
// 17 meshes (8 buildings, 8 props, 1 hero). Every rule is a Site with chance 1 on ONE slot (a zone a slot, as the p3b
// and p3f blocks do), so nothing slides along its slot and every piece stands at the distance written here.
// Slots used: even slots 0 to 48 only (s % 4 == 0 or 2), 0 to 2.18 km; slots 50 and 52 (the first jump's gap and its
// lake) are left empty. Bands: low props 23 m (slots 0, 2, 4: the stretch at the top that the dressing's verge zone
// gives as not banked) and in the building sets 40 m or more out; masts (footed 12 m down) 32 m; buildings 46 to
// 66 m, each on a slot and side where neither table has a rule within 8 m of it at the same slot (the watchers' 42 m
// sets, chance 0.5, are nearer on some: see the hand-over); the hero once, right, 112 m, slot 18 (0.814 km).
// Estimate: 66 Site rules write 241 pieces (39 buildings, 201 props, 1 hero); x 0.75 (the track's share left out) FIX ROUND: the 4 rules of slot 0 never fire (18 pieces): 223 written, not 241; after the fix round fewer, see the note L5_ice_s1.md.
// = about 181 placed. Most of one mesh: LampMast, written 39 times.
// A part's Up is -0.07 x along on the right, +0.07 x along on the left (the hill falls 7 in 100).
#define S1D1(n) TEXT("/Game/World/IceMountain/SM_S1D1_" n)
Set(TEXT("WldS1D1HostelR"), {
	{ S1D1("PodHostel"), 0.0f, 0.0f, 0.0f, -15.0f, 1.f },
	{ S1D1("LampMast"), -10.5f, -2.0f, 0.74f, 0.0f, 1.f },
	{ S1D1("SkiRack"), -13.0f, -4.5f, 0.91f, 0.0f, 1.f },
	{ S1D1("LockerPods"), 10.5f, -4.5f, -0.74f, 0.0f, 1.f },
	{ S1D1("SnowStake"), 13.0f, 1.0f, -0.91f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1CafeR"), {
	{ S1D1("DrumCafe"), 0.0f, 0.0f, 0.0f, -20.0f, 1.f },
	{ S1D1("LampMast"), -9.0f, -3.0f, 0.63f, 0.0f, 1.f },
	{ S1D1("Windsock"), 9.0f, -3.0f, -0.63f, 0.0f, 1.f },
	{ S1D1("SnowStake"), -11.5f, 1.5f, 0.81f, 0.0f, 1.f },
	{ S1D1("SkiRack"), 11.5f, 1.0f, -0.81f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1TimingR"), {
	{ S1D1("TimingCabin"), 0.0f, 0.0f, 0.0f, -20.0f, 1.f },
	{ S1D1("Podium"), -12.0f, -1.0f, 0.84f, 0.0f, 1.f },
	{ S1D1("FlagLine"), -12.0f, -4.5f, 0.84f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1SchoolR"), {
	{ S1D1("SkiSchool"), 0.0f, 0.0f, 0.0f, -12.0f, 1.f },
	{ S1D1("SkiRack"), -4.0f, -5.4f, 0.28f, 0.0f, 1.f },
	{ S1D1("SnowStake"), 11.5f, -2.0f, -0.81f, 0.0f, 1.f },
	{ S1D1("Windsock"), -11.0f, 1.0f, 0.77f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1HallR"), {
	{ S1D1("LiftHall"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f },
	{ S1D1("TicketGate"), 0.0f, -6.8f, 0.0f, 0.0f, 1.f },
	{ S1D1("LampMast"), -10.5f, -4.0f, 0.74f, 0.0f, 1.f },
	{ S1D1("LockerPods"), -11.5f, -1.0f, 0.81f, 0.0f, 1.f },
	{ S1D1("Windsock"), 11.0f, 2.0f, -0.77f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1StartR"), {
	{ S1D1("StartHouse"), 0.0f, 0.0f, 0.0f, -20.0f, 1.f },
	{ S1D1("Podium"), 10.0f, -1.0f, -0.7f, 0.0f, 1.f },
	{ S1D1("TicketGate"), -8.5f, -2.5f, 0.6f, 0.0f, 1.f },
	{ S1D1("LampMast"), -6.0f, -4.5f, 0.42f, 0.0f, 1.f },
	{ S1D1("Windsock"), -7.0f, 3.0f, 0.49f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1WeatherR"), {
	{ S1D1("WeatherPod"), 0.0f, 0.0f, 0.0f, -20.0f, 1.f },
	{ S1D1("Windsock"), -7.5f, -1.0f, 0.53f, 0.0f, 1.f },
	{ S1D1("SnowStake"), 7.0f, -2.5f, -0.49f, 0.0f, 1.f },
	{ S1D1("LampMast"), 8.0f, 2.0f, -0.56f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1ShopR"), {
	{ S1D1("BoardShop"), 0.0f, 0.0f, 0.0f, -15.0f, 1.f },
	{ S1D1("SkiRack"), -9.5f, -3.0f, 0.67f, 0.0f, 1.f },
	{ S1D1("SkiRack"), 9.5f, -3.0f, -0.67f, 0.0f, 1.f },
	{ S1D1("LockerPods"), -1.5f, -5.3f, 0.11f, 0.0f, 1.f },
	{ S1D1("LampMast"), -8.5f, 1.5f, 0.6f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1MastsAR"), {
	{ S1D1("FlagLine"), -6.0f, 0.0f, 0.42f, 0.0f, 1.f },
	{ S1D1("Windsock"), 10.0f, 0.0f, -0.7f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1MastsBR"), {
	{ S1D1("LampMast"), -8.0f, 0.0f, 0.56f, 0.0f, 1.f },
	{ S1D1("FlagLine"), 6.0f, 0.0f, -0.42f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1VergeR"), {
	{ S1D1("SkiRack"), -12.0f, 0.0f, 0.84f, 0.0f, 1.f },
	{ S1D1("LockerPods"), -4.0f, 0.3f, 0.28f, 0.0f, 1.f },
	{ S1D1("TicketGate"), 4.0f, 0.0f, -0.28f, 0.0f, 1.f },
	{ S1D1("SnowStake"), 12.0f, 0.0f, -0.84f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1HostelL"), {
	{ S1D1("PodHostel"), 0.0f, 0.0f, 0.0f, 15.0f, 1.f },
	{ S1D1("LampMast"), -10.5f, -2.0f, -0.74f, 0.0f, 1.f },
	{ S1D1("SkiRack"), -13.0f, -4.5f, -0.91f, 0.0f, 1.f },
	{ S1D1("LockerPods"), 10.5f, -4.5f, 0.74f, 0.0f, 1.f },
	{ S1D1("SnowStake"), 13.0f, 1.0f, 0.91f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1CafeL"), {
	{ S1D1("DrumCafe"), 0.0f, 0.0f, 0.0f, 20.0f, 1.f },
	{ S1D1("LampMast"), -9.0f, -3.0f, -0.63f, 0.0f, 1.f },
	{ S1D1("Windsock"), 9.0f, -3.0f, 0.63f, 0.0f, 1.f },
	{ S1D1("SnowStake"), -11.5f, 1.5f, -0.81f, 0.0f, 1.f },
	{ S1D1("SkiRack"), 11.5f, 1.0f, 0.81f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1TimingL"), {
	{ S1D1("TimingCabin"), 0.0f, 0.0f, 0.0f, 20.0f, 1.f },
	{ S1D1("Podium"), -12.0f, -1.0f, -0.84f, 0.0f, 1.f },
	{ S1D1("FlagLine"), -12.0f, -4.5f, -0.84f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1SchoolL"), {
	{ S1D1("SkiSchool"), 0.0f, 0.0f, 0.0f, 12.0f, 1.f },
	{ S1D1("SkiRack"), -4.0f, -5.4f, -0.28f, 0.0f, 1.f },
	{ S1D1("SnowStake"), 11.5f, -2.0f, 0.81f, 0.0f, 1.f },
	{ S1D1("Windsock"), -11.0f, 1.0f, -0.77f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1HallL"), {
	{ S1D1("LiftHall"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f },
	{ S1D1("TicketGate"), 0.0f, -6.8f, 0.0f, 0.0f, 1.f },
	{ S1D1("LampMast"), -10.5f, -4.0f, -0.74f, 0.0f, 1.f },
	{ S1D1("LockerPods"), -11.5f, -1.0f, -0.81f, 0.0f, 1.f },
	{ S1D1("Windsock"), 11.0f, 2.0f, 0.77f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1StartL"), {
	{ S1D1("StartHouse"), 0.0f, 0.0f, 0.0f, 20.0f, 1.f },
	{ S1D1("Podium"), 10.0f, -1.0f, 0.7f, 0.0f, 1.f },
	{ S1D1("TicketGate"), -8.5f, -2.5f, -0.6f, 0.0f, 1.f },
	{ S1D1("LampMast"), -6.0f, -4.5f, -0.42f, 0.0f, 1.f },
	{ S1D1("Windsock"), -7.0f, 3.0f, -0.49f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1WeatherL"), {
	{ S1D1("WeatherPod"), 0.0f, 0.0f, 0.0f, 20.0f, 1.f },
	{ S1D1("Windsock"), -7.5f, -1.0f, -0.53f, 0.0f, 1.f },
	{ S1D1("SnowStake"), 7.0f, -2.5f, 0.49f, 0.0f, 1.f },
	{ S1D1("LampMast"), 8.0f, 2.0f, 0.56f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1ShopL"), {
	{ S1D1("BoardShop"), 0.0f, 0.0f, 0.0f, 15.0f, 1.f },
	{ S1D1("SkiRack"), -9.5f, -3.0f, -0.67f, 0.0f, 1.f },
	{ S1D1("SkiRack"), 9.5f, -3.0f, 0.67f, 0.0f, 1.f },
	{ S1D1("LockerPods"), -1.5f, -5.3f, -0.11f, 0.0f, 1.f },
	{ S1D1("LampMast"), -8.5f, 1.5f, -0.6f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1MastsAL"), {
	{ S1D1("FlagLine"), -6.0f, 0.0f, -0.42f, 0.0f, 1.f },
	{ S1D1("Windsock"), 10.0f, 0.0f, 0.7f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1MastsBL"), {
	{ S1D1("LampMast"), -8.0f, 0.0f, -0.56f, 0.0f, 1.f },
	{ S1D1("FlagLine"), 6.0f, 0.0f, 0.42f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1VergeL"), {
	{ S1D1("SkiRack"), -12.0f, 0.0f, -0.84f, 0.0f, 1.f },
	{ S1D1("LockerPods"), -4.0f, 0.3f, -0.28f, 0.0f, 1.f },
	{ S1D1("TicketGate"), 4.0f, 0.0f, 0.28f, 0.0f, 1.f },
	{ S1D1("SnowStake"), 12.0f, 0.0f, 0.84f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1Needle"), {
	{ S1D1("SummitNeedle"), 0.f, 0.f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1d1, First tracks: slot 0 (0.000 km)"), 0.0000f, 0.0060f);
	Site(TEXT("WldS1D1VergeR"), 1, 23.0f, 1, 0, 1.f);   // FIX ROUND (W1): never placed, the track has no slot 0 (its first slot is 1); left written so that no later rule is re-rolled
	Site(TEXT("WldS1D1StartR"), 1, 52.0f, 1, 0, 1.f);   // FIX ROUND (W1): never placed, the track has no slot 0 (its first slot is 1); left written so that no later rule is re-rolled
	Site(TEXT("WldS1D1VergeL"), -1, 23.0f, 1, 0, 1.f);   // FIX ROUND (W1): never placed, the track has no slot 0 (its first slot is 1); left written so that no later rule is re-rolled
	Site(TEXT("WldS1D1HallL"), -1, 66.0f, 1, 0, 1.f);   // FIX ROUND (W1): never placed, the track has no slot 0 (its first slot is 1); left written so that no later rule is re-rolled
Zone(TEXT("World s1d1, First tracks: slot 2 (0.090 km)"), 0.0845f, 0.0965f);
	Site(TEXT("WldS1D1VergeR"), 1, 23.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1HostelR"), 1, 52.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeL"), -1, 23.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1TimingL"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 4 (0.181 km)"), 0.1750f, 0.1870f);
	Site(TEXT("WldS1D1VergeR"), 1, 23.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1MastsAR"), 1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1CafeR"), 1, 52.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeL"), -1, 23.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1MastsBL"), -1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1ShopL"), -1, 46.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 6 (0.271 km)"), 0.2654f, 0.2774f);
	Site(TEXT("WldS1D1HallRFx01"), 1, 66.0f, 1, 0, 1.f);   // FIX ROUND: L6: its box was 3.8 m inside P3A_Igloo_A; L6: its box was 3.4 m inside P3A_Igloo_A
	Site(TEXT("WldS1D1MastsAL"), -1, 32.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 8 (0.362 km)"), 0.3559f, 0.3679f);
	Site(TEXT("WldS1D1MastsBR"), 1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1SchoolRFx03"), 1, 52.0f, 1, 0, 1.f);   // FIX ROUND: L6: its box was 3.1 m inside Snowcat; L6: its box was 2.2 m inside Snowcat
	Site(TEXT("WldS1D1HostelL"), -1, 52.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 10 (0.452 km)"), 0.4464f, 0.4584f);
	Site(TEXT("WldS1D1MastsAR"), 1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1WeatherR"), 1, 51.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1MastsBL"), -1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1CafeLFx02"), -1, 66.0f, 1, 0, 1.f);   // FIX ROUND: L6: its box was 3.2 m inside P3A_Hikers_B; L6: its box was 2.0 m inside P3A_Hikers_B
Zone(TEXT("World s1d1, First tracks: slot 12 (0.543 km)"), 0.5369f, 0.5489f);
	Site(TEXT("WldS1D1MastsAL"), -1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1StartL"), -1, 51.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 14 (0.633 km)"), 0.6274f, 0.6394f);
	Site(TEXT("WldS1D1MastsBR"), 1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1ShopRFx3"), 1, 52.0f, 1, 0, 1.f);   // FIX ROUND: L6: its box was 2.1 m inside WatchBank_A
	Site(TEXT("WldS1D1WeatherL"), -1, 52.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 16 (0.724 km)"), 0.7178f, 0.7298f);
	Site(TEXT("WldS1D1MastsAR"), 1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1TimingR"), 1, 52.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1MastsBL"), -1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1SchoolL"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 18 (0.814 km)"), 0.8083f, 0.8203f);
	Site(TEXT("WldS1D1HostelRFx0"), 1, 52.0f, 1, 0, 1.f);   // FIX ROUND: L4: stood on the car park of slot 17
	Site(TEXT("WldS1D1Needle"), 1, 112.0f, 1, 0, -1.f);     // THE SUMMIT NEEDLE: once, right, 112 m out, 0.814 km   // FIX ROUND: driver 5: at slot 18 the bank hid the stem (a hovering disc from the left lane); now at slot 16 (s1fix block)
	Site(TEXT("WldS1D1MastsAL"), -1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1HallLFx1"), -1, 62.0f, 1, 0, 1.f);   // FIX ROUND: L6: its box was 2.7 m inside BoardPipe_A
Zone(TEXT("World s1d1, First tracks: slot 20 (0.905 km)"), 0.8988f, 0.9108f);
	Site(TEXT("WldS1D1MastsBR"), 1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1TimingL"), -1, 52.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 22 (0.995 km)"), 0.9893f, 1.0013f);
	Site(TEXT("WldS1D1MastsAR"), 1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1CafeR"), 1, 62.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1MastsBL"), -1, 32.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 24 (1.086 km)"), 1.0798f, 1.0918f);
	Site(TEXT("WldS1D1MastsAL"), -1, 32.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 26 (1.176 km)"), 1.1702f, 1.1822f);
	Site(TEXT("WldS1D1MastsBR"), 1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1ShopL"), -1, 52.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 28 (1.267 km)"), 1.2607f, 1.2727f);
	Site(TEXT("WldS1D1StartR"), 1, 54.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1SchoolL"), -1, 46.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 30 (1.357 km)"), 1.3512f, 1.3632f);
	Site(TEXT("WldS1D1WeatherR"), 1, 46.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1HallL"), -1, 54.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 32 (1.448 km)"), 1.4417f, 1.4537f);
	Site(TEXT("WldS1D1TimingR"), 1, 54.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1CafeL"), -1, 46.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 34 (1.538 km)"), 1.5322f, 1.5442f);
	Site(TEXT("WldS1D1ShopR"), 1, 46.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1HostelL"), -1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 36 (1.629 km)"), 1.6226f, 1.6346f);
	Site(TEXT("WldS1D1WeatherL"), -1, 46.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 38 (1.719 km)"), 1.7131f, 1.7251f);
	Site(TEXT("WldS1D1SchoolR"), 1, 46.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 40 (1.810 km)"), 1.8036f, 1.8156f);
	Site(TEXT("WldS1D1MastsBL"), -1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1StartL"), -1, 64.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 42 (1.900 km)"), 1.8941f, 1.9061f);
	Site(TEXT("WldS1D1MastsAR"), 1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1ShopL"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 44 (1.991 km)"), 1.9846f, 1.9966f);
	Site(TEXT("WldS1D1HallR"), 1, 55.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1MastsAL"), -1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1TimingL"), -1, 55.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, First tracks: slot 46 (2.081 km)"), 2.0750f, 2.0870f);
	Site(TEXT("WldS1D1HostelR"), 1, 46.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1MastsAL"), -1, 32.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1CafeLFx0"), -1, 46.0f, 1, 0, 1.f);   // FIX ROUND: L4: stood on the terrain park of slot 47
Zone(TEXT("World s1d1, First tracks: slot 48 (2.172 km)"), 2.1655f, 2.1775f);
	Site(TEXT("WldS1D1StartRFx0"), 1, 50.0f, 1, 0, 1.f);   // FIX ROUND: L4: stood on the car park of slot 47
	Site(TEXT("WldS1D1WeatherL"), -1, 46.0f, 1, 0, 1.f);
#undef S1D1
// ---- END s1d1 ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People5 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1d2 (district 2 "Black diamond", the avalanche works, ft_world_ice_s1d2.py) ----
// 19 meshes (7 buildings and 2 left-hand twins, 9 props, 1 hero). 40 Site rules, each on ONE slot and side (every 600, phase 600 - slot;
// the slot is in the comment): 209 pieces written. Estimate: 209 x 0.75 (the brief's share) = about 157 placed; on the slots with no pipe
// and no jump beside them the block writes 152. Most of one mesh: DiamondPlate x 34. Even slots only (s % 4 == 0 or 2).
// Bands: every piece between 41.0 and 70.6 m from the centre line, every placement point under 70 m (max 68.0): every plain slot of
// this district is a banked bend or lies behind the super pipe's ice walls, so NOTHING is written nearer than 40 m, the props too.
// The hero: 110 m out on the left at slot 102 (4.61 km), once. Not written: slots 56, 58, 90 to 98, 110, 112, 116, 118 (corkscrews,
// the loop, the jump), 78 (the gondola), and the sides whose 40 to 70 m is taken by the cast.
// A set's parts keep the land: up = -0.07 x metres down the hill. Left-hand sets are the right-hand ones mirrored (along and yaw
// change sign; the bunker and the hut have twins SM_..._L whose uphill end is at +X). Plates are turned 55 degrees up the road.
#define S1D2(n) TEXT("/Game/World/IceMountain/SM_S1D2_" n)
Set(TEXT("WldS1D2SplitR"), {    // the splitter bunker, tetrapods uphill of its prow, a plate, bollards and the siren below it
	{ S1D2("SplitterBunker"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f },
	{ S1D2("Tetrapods"), -16.5f, -2.0f, 1.16f, 20.0f, 1.f },
	{ S1D2("Tetrapods"), -18.5f, 4.0f, 1.3f, 70.0f, 1.f },
	{ S1D2("DiamondPlate"), 15.5f, -4.5f, -1.09f, -55.0f, 1.f },
	{ S1D2("BollardRow"), 16.0f, 3.0f, -1.12f, 0.0f, 1.f },
	{ S1D2("SirenMast"), 14.0f, -0.5f, -0.98f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2SplitL"), {    // the splitter bunker, tetrapods uphill of its prow, a plate, bollards and the siren below it
	{ S1D2("SplitterBunker_L"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f },
	{ S1D2("Tetrapods"), 16.5f, -2.0f, 1.16f, -20.0f, 1.f },
	{ S1D2("Tetrapods"), 18.5f, 4.0f, 1.3f, -70.0f, 1.f },
	{ S1D2("DiamondPlate"), -15.5f, -4.5f, -1.09f, 55.0f, 1.f },
	{ S1D2("BollardRow"), -16.0f, 3.0f, -1.12f, 0.0f, 1.f },
	{ S1D2("SirenMast"), -14.0f, -0.5f, -0.98f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2ShelterR"), {    // the blast shelter between two blast walls, its vents, a siren, a plate
	{ S1D2("BlastShelter"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f },
	{ S1D2("ChevronBarrier"), -12.5f, -4.6f, 0.88f, 25.0f, 1.f },
	{ S1D2("ChevronBarrier"), 12.5f, -4.6f, -0.88f, -25.0f, 1.f },
	{ S1D2("VentCluster"), -11.0f, 3.0f, 0.77f, 0.0f, 1.f },
	{ S1D2("SirenMast"), 11.5f, 3.5f, -0.81f, 0.0f, 1.f },
	{ S1D2("DiamondPlate"), -18.5f, -2.0f, 1.3f, -55.0f, 1.f },
});
Set(TEXT("WldS1D2ShelterL"), {    // the blast shelter between two blast walls, its vents, a siren, a plate
	{ S1D2("BlastShelter"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f },
	{ S1D2("ChevronBarrier"), 12.5f, -4.6f, 0.88f, -25.0f, 1.f },
	{ S1D2("ChevronBarrier"), -12.5f, -4.6f, -0.88f, 25.0f, 1.f },
	{ S1D2("VentCluster"), 11.0f, 3.0f, 0.77f, 0.0f, 1.f },
	{ S1D2("SirenMast"), -11.5f, 3.5f, -0.81f, 0.0f, 1.f },
	{ S1D2("DiamondPlate"), 18.5f, -2.0f, 1.3f, 55.0f, 1.f },
});
Set(TEXT("WldS1D2StudyR"), {    // the snow-study tower, the depth gantry across the approach, a net anchor, a cable drum, a plate, tetrapods
	{ S1D2("StudyTower"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f },
	{ S1D2("DepthGantry"), -12.0f, 0.0f, 0.84f, 90.0f, 1.f },
	{ S1D2("NetAnchor"), 11.0f, -3.5f, -0.77f, 0.0f, 1.f },
	{ S1D2("CableSpool"), 13.5f, 2.5f, -0.95f, 30.0f, 1.f },
	{ S1D2("DiamondPlate"), 17.5f, -1.0f, -1.23f, -55.0f, 1.f },
	{ S1D2("Tetrapods"), 19.0f, 4.5f, -1.33f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2StudyL"), {    // the snow-study tower, the depth gantry across the approach, a net anchor, a cable drum, a plate, tetrapods
	{ S1D2("StudyTower"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f },
	{ S1D2("DepthGantry"), 12.0f, 0.0f, 0.84f, -90.0f, 1.f },
	{ S1D2("NetAnchor"), -11.0f, -3.5f, -0.77f, 0.0f, 1.f },
	{ S1D2("CableSpool"), -13.5f, 2.5f, -0.95f, -30.0f, 1.f },
	{ S1D2("DiamondPlate"), -17.5f, -1.0f, -1.23f, 55.0f, 1.f },
	{ S1D2("Tetrapods"), -19.0f, 4.5f, -1.33f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2PatrolR"), {    // the patrol hut, a plate, bollards, a net anchor, tetrapods, a siren
	{ S1D2("PatrolHut"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f },
	{ S1D2("DiamondPlate"), -9.5f, -4.0f, 0.67f, -55.0f, 1.f },
	{ S1D2("BollardRow"), 11.0f, -3.5f, -0.77f, 0.0f, 1.f },
	{ S1D2("NetAnchor"), -12.0f, 2.0f, 0.84f, 90.0f, 1.f },
	{ S1D2("Tetrapods"), 13.5f, 3.0f, -0.95f, 0.0f, 1.f },
	{ S1D2("SirenMast"), -8.5f, 3.2f, 0.6f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2PatrolL"), {    // the patrol hut, a plate, bollards, a net anchor, tetrapods, a siren
	{ S1D2("PatrolHut_L"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f },
	{ S1D2("DiamondPlate"), 9.5f, -4.0f, 0.67f, 55.0f, 1.f },
	{ S1D2("BollardRow"), -11.0f, -3.5f, -0.77f, 0.0f, 1.f },
	{ S1D2("NetAnchor"), 12.0f, 2.0f, 0.84f, -90.0f, 1.f },
	{ S1D2("Tetrapods"), -13.5f, 3.0f, -0.95f, 0.0f, 1.f },
	{ S1D2("SirenMast"), 8.5f, 3.2f, 0.6f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2GearL"), {    // the gear store (gable turned up the road), two cable drums, a net anchor, bollards, vents
	{ S1D2("GearStore"), 0.0f, 0.0f, 0.0f, 15.0f, 1.f },
	{ S1D2("CableSpool"), 10.5f, -3.0f, 0.74f, 0.0f, 1.f },
	{ S1D2("CableSpool"), 13.5f, 2.0f, 0.95f, -80.0f, 1.f },
	{ S1D2("NetAnchor"), -12.5f, -3.5f, -0.88f, 0.0f, 1.f },
	{ S1D2("BollardRow"), -12.0f, 3.5f, -0.84f, 0.0f, 1.f },
	{ S1D2("VentCluster"), 18.0f, -1.5f, 1.26f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2SignalR"), {    // no building (the middle of the slot is taken): a siren, a plate, tetrapods uphill; a blast wall, vents, a plate downhill
	{ S1D2("SirenMast"), -15.0f, 0.0f, 1.05f, 0.0f, 1.f },
	{ S1D2("DiamondPlate"), -18.5f, -2.5f, 1.3f, -55.0f, 1.f },
	{ S1D2("Tetrapods"), -12.5f, 2.0f, 0.88f, 0.0f, 1.f },
	{ S1D2("ChevronBarrier"), 15.5f, -1.5f, -1.09f, 0.0f, 1.f },
	{ S1D2("VentCluster"), 13.0f, 3.0f, -0.91f, 0.0f, 1.f },
	{ S1D2("DiamondPlate"), 20.5f, 2.5f, -1.44f, -55.0f, 1.f },
});
Set(TEXT("WldS1D2GalleryR"), {    // the gallery portal, a blast wall, a plate, a siren
	{ S1D2("GalleryPortal"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f },
	{ S1D2("ChevronBarrier"), -15.0f, -1.5f, 1.05f, 0.0f, 1.f },
	{ S1D2("DiamondPlate"), 13.0f, -2.5f, -0.91f, -55.0f, 1.f },
	{ S1D2("SirenMast"), 16.0f, 1.0f, -1.12f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2GalleryL"), {    // the gallery portal, a blast wall, a plate, a siren
	{ S1D2("GalleryPortal"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f },
	{ S1D2("ChevronBarrier"), 15.0f, -1.5f, 1.05f, 0.0f, 1.f },
	{ S1D2("DiamondPlate"), -13.0f, -2.5f, -0.91f, 55.0f, 1.f },
	{ S1D2("SirenMast"), -16.0f, 1.0f, -1.12f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2WinchR"), {    // the winch house, a cable drum, a plate, bollards, tetrapods
	{ S1D2("WinchHouse"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f },
	{ S1D2("CableSpool"), 11.0f, -1.0f, -0.77f, 0.0f, 1.f },
	{ S1D2("DiamondPlate"), -10.5f, -1.5f, 0.74f, -55.0f, 1.f },
	{ S1D2("BollardRow"), -15.5f, 1.5f, 1.09f, 0.0f, 1.f },
	{ S1D2("Tetrapods"), 16.5f, 1.0f, -1.16f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2WinchL"), {    // the winch house, a cable drum, a plate, bollards, tetrapods
	{ S1D2("WinchHouse"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f },
	{ S1D2("CableSpool"), -11.0f, -1.0f, -0.77f, 0.0f, 1.f },
	{ S1D2("DiamondPlate"), 10.5f, -1.5f, 0.74f, 55.0f, 1.f },
	{ S1D2("BollardRow"), 15.5f, 1.5f, 1.09f, 0.0f, 1.f },
	{ S1D2("Tetrapods"), -16.5f, 1.0f, -1.16f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2TowerR"), {    // the snow-study tower in a line: the depth gantry, a blast wall, bollards
	{ S1D2("StudyTower"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f },
	{ S1D2("DepthGantry"), -12.5f, 0.5f, 0.88f, 0.0f, 1.f },
	{ S1D2("ChevronBarrier"), 13.0f, -1.5f, -0.91f, 0.0f, 1.f },
	{ S1D2("BollardRow"), 13.0f, 1.5f, -0.91f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Hero"), {    // THE BLACK DIAMOND, alone
	{ S1D2("BlackDiamond"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f },
});
Zone(TEXT("World s1d2, Black diamond: the chute"), 2.43f, 2.96f);
	Site(TEXT("WldS1D2PatrolRFx3"), 1, 49.0f, 600, 546, 1.f);     // slot 54, 2.443 km, right (beside the first jump: its landing: the track may leave it out)   // FIX ROUND: L4: stood on the car park of slot 53
	Site(TEXT("WldS1D2SplitL"), -1, 63.5f, 600, 540, 1.f);     // slot 60, 2.714 km, left
	Site(TEXT("WldS1D2ShelterR"), 1, 48.0f, 600, 540, 1.f);     // slot 60, 2.714 km, right
	Site(TEXT("WldS1D2StudyL"), -1, 52.0f, 600, 538, 1.f);     // slot 62, 2.805 km, left
	Site(TEXT("WldS1D2SignalRFx1"), 1, 50.0f, 600, 538, 1.f);     // slot 62, 2.805 km, right   // FIX ROUND: L2: stood in the rock cutting
	Site(TEXT("WldS1D2PatrolL"), -1, 63.0f, 600, 536, 1.f);     // slot 64, 2.895 km, left
	Site(TEXT("WldS1D2SplitR"), 1, 53.5f, 600, 536, 1.f);     // slot 64, 2.895 km, right   // FIX ROUND: L2: 48 m stood in the rock cutting (35 to 47 m out); now behind it
Zone(TEXT("World s1d2, Black diamond: the super pipe"), 2.96f, 4.02f);
	Site(TEXT("WldS1D2GearL"), -1, 60.0f, 600, 534, 1.f);     // slot 66, 2.986 km, left
	Site(TEXT("WldS1D2SignalR"), 1, 55.0f, 600, 534, 1.f);     // slot 66, 2.986 km, right
	Site(TEXT("WldS1D2StudyL"), -1, 60.0f, 600, 532, 1.f);     // slot 68, 3.076 km, left (beside the super pipe: the track may leave it out)
	Site(TEXT("WldS1D2TowerR"), 1, 66.5f, 600, 532, 1.f);     // slot 68, 3.076 km, right (beside the super pipe: the track may leave it out)
	Site(TEXT("WldS1D2SplitL"), -1, 60.0f, 600, 530, 1.f);     // slot 70, 3.167 km, left (beside the super pipe: the track may leave it out)
	Site(TEXT("WldS1D2WinchR"), 1, 66.5f, 600, 530, 1.f);     // slot 70, 3.167 km, right (beside the super pipe: the track may leave it out)
	Site(TEXT("WldS1D2GalleryR"), 1, 65.0f, 600, 528, 1.f);     // slot 72, 3.257 km, right (beside the super pipe: the track may leave it out)
	Site(TEXT("WldS1D2ShelterL"), -1, 60.0f, 600, 526, 1.f);     // slot 74, 3.348 km, left (beside the super pipe: the track may leave it out)
	Site(TEXT("WldS1D2TowerR"), 1, 66.5f, 600, 526, 1.f);     // slot 74, 3.348 km, right (beside the super pipe: the track may leave it out)
	Site(TEXT("WldS1D2PatrolL"), -1, 60.0f, 600, 524, 1.f);     // slot 76, 3.438 km, left (beside the super pipe: the track may leave it out)
	Site(TEXT("WldS1D2PatrolL"), -1, 60.0f, 600, 520, 1.f);     // slot 80, 3.619 km, left
	Site(TEXT("WldS1D2GalleryR"), 1, 65.0f, 600, 520, 1.f);     // slot 80, 3.619 km, right
	Site(TEXT("WldS1D2WinchR"), 1, 66.5f, 600, 518, 1.f);     // slot 82, 3.710 km, right
	Site(TEXT("WldS1D2SplitL"), -1, 60.0f, 600, 516, 1.f);     // slot 84, 3.800 km, left
	Site(TEXT("WldS1D2TowerR"), 1, 66.5f, 600, 516, 1.f);     // slot 84, 3.800 km, right
	Site(TEXT("WldS1D2StudyL"), -1, 60.0f, 600, 514, 1.f);     // slot 86, 3.891 km, left
	Site(TEXT("WldS1D2GearL"), -1, 60.0f, 600, 512, 1.f);     // slot 88, 3.981 km, left
	Site(TEXT("WldS1D2GalleryR"), 1, 65.0f, 600, 512, 1.f);     // slot 88, 3.981 km, right
Zone(TEXT("World s1d2, Black diamond: the ridge"), 4.02f, 5.65f);
	Site(TEXT("WldS1D2ShelterL"), -1, 60.0f, 600, 500, 1.f);     // slot 100, 4.524 km, left
	Site(TEXT("WldS1D2WinchR"), 1, 55.5f, 600, 500, 1.f);     // slot 100, 4.524 km, right
	Site(TEXT("WldS1D2GalleryL"), -1, 56.0f, 600, 498, 1.f);     // slot 102, 4.614 km, left
	Site(TEXT("WldS1D2Hero"), -1, 110.0f, 600, 498, -1.f);     // slot 102, 4.614 km, left   // FIX ROUND: L1: the Black Diamond stood on the groomers strip of slot 103; it now stands at slot 100 (s1fix block)
	Site(TEXT("WldS1D2GalleryR"), 1, 56.0f, 600, 498, 1.f);     // slot 102, 4.614 km, right
	Site(TEXT("WldS1D2SplitL"), -1, 58.5f, 600, 496, 1.f);     // slot 104, 4.705 km, left
	Site(TEXT("WldS1D2GearL"), -1, 62.0f, 600, 494, 1.f);     // slot 106, 4.795 km, left
	Site(TEXT("WldS1D2StudyR"), 1, 53.0f, 600, 494, 1.f);     // slot 106, 4.795 km, right
	Site(TEXT("WldS1D2GalleryR"), 1, 55.5f, 600, 492, -1.f);     // slot 108, 4.886 km, right (beside the second jump: its run-up: the track may leave it out)   // FIX ROUND: L4: stood on the terrain park of slot 107
	Site(TEXT("WldS1D2PatrolRFx0345"), 1, 50.0f, 600, 486, 1.f);     // slot 114, 5.157 km, right (beside the second jump: its landing: the track may leave it out)   // FIX ROUND: L4: stood on the terrain park of slot 113
	Site(TEXT("WldS1D2WinchR"), 1, 55.3f, 600, 480, 1.f);     // slot 120, 5.429 km, right
	Site(TEXT("WldS1D2WinchL"), -1, 56.0f, 600, 478, 1.f);     // slot 122, 5.519 km, left
	Site(TEXT("WldS1D2TowerR"), 1, 56.0f, 600, 478, 1.f);     // slot 122, 5.519 km, right
	Site(TEXT("WldS1D2ShelterL"), -1, 58.5f, 600, 476, 1.f);     // slot 124, 5.610 km, left
	Site(TEXT("WldS1D2WinchR"), 1, 55.5f, 600, 476, 1.f);     // slot 124, 5.610 km, right
#undef S1D2
// ---- END s1d2 ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People5 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1d3 (district 3 "The valley run", 5.65 to 8.94 km, ft_world_ice_s1d3.py) ----
// 18 meshes: 1 hero (GreatCowbell), 8 buildings (TallHouse, ArcadeHouse, Bakery, ToyMaker, TownGate, Granary, OrielHouse, GalleryInn),
// 9 props (FountainTrough, SledRack, RibbonPole, MarketStall, Woodstack, LanternPost, WashLine, FingerPost, BenchPlanter).
// WHERE AND WHY (read from FTDressL5.inl and FTPeopleL5.inl; nothing seen in the game):
//   Of the district's even slots (Stage 1's district slots) all but three lie beside a half pipe (126 to 136, 186 to 194) or a banked bend
//   (140 to 150, 166 to 174), where 17 to 40 m floats over the cutting or hides behind the pipe's wall; 152 to 162 are the loop and the
//   corkscrew, 164 is the village street (nothing in front of or behind its chalet rows), 178 to 184 the third jump and the gondola.
//   So the town stands 41 to 70 m out on 25 slots, and every slot and side is laid out on its own (a scratch tool,
//   C:/ftwork/jobs/L5s1d3/place.py): the tables' own pieces mostly stand in the MIDDLE of a slot (a Site is not moved along), so a side
//   of a slot gets a cluster at either END (along -17.4 and +17.4 m) and one in the middle where that is free: a house 52 to 64 m out,
//   a taller one 10.2 m behind it where there is room, one or two props 8 m in front; a prop alone where no house fits.
//   Every piece was tried box against box (1 m apart or more) against every Site and Scatter of both tables on that slot and side
//   (their sizes from the make logs; the dressing kit's own sizes ASSUMED, see place.py) and left out where nothing was free.
//   The props stand in front of their houses (centres 44 to 56 m), not in the brief's 17 to 40 m bands: said in the hand-over.
//   No mesh stands both inside and outside 70 m. Up = -0.07 x along (right), +0.07 x along (left), as the dressing table says.
//   Town gates at the four ends of the two halves of the town (slots 126, 150, 166, 196: 5.70, 6.79, 7.51, 8.87 km).
//   THE GREAT COWBELL: right, 120 m out, slot 146 (6.605 km); left, 120 m out, slot 170 (7.691 km).
//   AS BUILT (make log: 22.3 x 28.3 m) the Cowbell has a fault: its gallery rail, shutters and lintels on three faces came out as
//   slabs (a float used as a truth value). The generator is corrected; the mesh needs one K.run(only=['SM_S1D3_GreatCowbell']).
// Estimate: every rule is a Site on one slot (a zone of one slot, every 4): 49 Sites holding 225 parts x 0.75 = 169, + 2 heroes x 0.75 = 1.5:
//   about 170 placed (227 by the slot list, before the track leaves any out).
//   Most of one mesh: TallHouse, 34 by the slot list (26 by the estimate). Hero: 2. Smallest x of a TALL mesh: 44.0 m (LOW: 44.0 m).
#define S1D3(n) TEXT("/Game/World/IceMountain/SM_S1D3_" n)
Set(TEXT("WldS1D3B126L"), {     // along +0: no house (GustL:SM_SnowGust)
	{ S1D3("ToyMaker"), -17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("TallHouse"), -17.4f, 10.2f, -1.22f, 0.f, 1.f },
	{ S1D3("LanternPost"), -17.4f, -8.0f, -1.22f, 15.f, 1.f },
	{ S1D3("WashLine"), -13.0f, -8.0f, -0.91f, 0.f, 1.f },
	{ S1D3("TownGate"), 17.4f, 0.0f, 1.22f, 0.f, 1.f },
	{ S1D3("MarketStall"), 17.4f, -8.0f, 1.22f, -12.f, 1.f },
});
Set(TEXT("WldS1D3B126R"), {
	{ S1D3("OrielHouse"), 17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 10.2f, -1.22f, -5.f, 1.f },
	{ S1D3("FingerPost"), 13.0f, -8.0f, -0.91f, 0.f, 1.f },
	{ S1D3("TownGate"), -17.4f, 0.0f, 1.22f, 0.f, 1.f },
	{ S1D3("Bakery"), 0.0f, 0.0f, -0.00f, -5.f, 1.f },
	{ S1D3("Woodstack"), 4.4f, -8.0f, -0.31f, 15.f, 1.f },
});
Set(TEXT("WldS1D3B128L"), {     // along +0: no house (GustL:SM_SnowGust)
	{ S1D3("Granary"), 17.4f, 8.0f, 1.22f, 4.f, 1.f },
	{ S1D3("TallHouse"), -17.4f, 8.0f, -1.22f, -5.f, 1.f },
	{ S1D3("Woodstack"), -13.0f, 0.0f, -0.91f, 0.f, 1.f },
	{ S1D3("RibbonPole"), 0.0f, 0.0f, 0.00f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B128R"), {     // along +0: no house (PplP3APisteCrewAR30:SM_P3A_PisteCrew_A)
	{ S1D3("ArcadeHouse"), -17.4f, 0.0f, 1.22f, 4.f, 1.f },
	{ S1D3("TallHouse"), -17.4f, 10.2f, 1.22f, -5.f, 1.f },
	{ S1D3("FountainTrough"), -17.4f, -8.0f, 1.22f, -12.f, 1.f },
	{ S1D3("Bakery"), 17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 10.2f, -1.22f, -5.f, 1.f },
	{ S1D3("WashLine"), 17.4f, -8.0f, -1.22f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B130L"), {     // along +0: no house (GustL:SM_SnowGust)
	{ S1D3("Granary"), -17.4f, 0.0f, -1.22f, -5.f, 1.f },
	{ S1D3("SledRack"), -17.4f, -8.0f, -1.22f, 0.f, 1.f },
	{ S1D3("OrielHouse"), 17.4f, 0.0f, 1.22f, -5.f, 1.f },
	{ S1D3("FingerPost"), 17.4f, -8.0f, 1.22f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B130R"), {     // along +0: no house (PplP3DDamAR:SM_P3D_Dam_A)
	{ S1D3("ArcadeHouse"), 17.4f, 4.0f, -1.22f, 0.f, 1.f },
	{ S1D3("GalleryInn"), -17.4f, 6.0f, 1.22f, 4.f, 1.f },
	{ S1D3("Woodstack"), 0.0f, 0.0f, -0.00f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B132L"), {     // along +0: no house (GustL:SM_SnowGust)
	{ S1D3("ToyMaker"), 17.4f, 0.0f, 1.22f, -5.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 10.2f, 1.22f, 0.f, 1.f },
	{ S1D3("BenchPlanter"), 17.4f, -8.0f, 1.22f, 0.f, 1.f },
	{ S1D3("Granary"), -17.4f, 0.0f, -1.22f, -5.f, 1.f },
	{ S1D3("TallHouse"), -17.4f, 10.2f, -1.22f, 0.f, 1.f },
	{ S1D3("LanternPost"), -17.4f, -8.0f, -1.22f, 8.f, 1.f },
	{ S1D3("MarketStall"), -13.0f, -8.0f, -0.91f, -12.f, 1.f },
});
Set(TEXT("WldS1D3B132R"), {     // along -17: no house (PplP3AHikersBR30:SM_P3A_Hikers_B); along +17: no house (PplP3AHikersBR30:SM_P3A_Hikers_B); along +0: no house (PplP3AHikersBR30:SM_P3A_Hikers_B)
	{ S1D3("MarketStall"), -17.4f, 0.0f, 1.22f, -12.f, 1.f },
	{ S1D3("RibbonPole"), 17.4f, 0.0f, -1.22f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B134L"), {     // along -17: no house (scatter:SM_BoardPipe_A); along +17: no house (scatter:SM_BoardPipe_A); along +0: no house (GustL:SM_SnowGust)
	{ S1D3("FountainTrough"), -17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("WashLine"), 17.4f, 0.0f, 1.22f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B134R"), {
	{ S1D3("ArcadeHouse"), 17.4f, 0.0f, -1.22f, 4.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 10.2f, -1.22f, 0.f, 1.f },
	{ S1D3("SledRack"), 17.4f, -8.0f, -1.22f, 15.f, 1.f },
	{ S1D3("LanternPost"), 13.0f, -8.0f, -0.91f, 15.f, 1.f },
	{ S1D3("Bakery"), -17.4f, 0.0f, 1.22f, 0.f, 1.f },
	{ S1D3("TallHouse"), -17.4f, 10.2f, 1.22f, 0.f, 1.f },
	{ S1D3("FingerPost"), -17.4f, -8.0f, 1.22f, 0.f, 1.f },
	{ S1D3("ToyMaker"), 0.0f, 0.0f, -0.00f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B136L"), {     // along +0: no house (GustL:SM_SnowGust)
	{ S1D3("Granary"), 17.4f, 0.0f, 1.22f, 0.f, 1.f },
	{ S1D3("Woodstack"), 17.4f, -8.0f, 1.22f, -12.f, 1.f },
	{ S1D3("OrielHouse"), -17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("BenchPlanter"), -17.4f, -8.0f, -1.22f, 15.f, 1.f },
});
Set(TEXT("WldS1D3B136R"), {     // along +0: no house (PplP3DCragAR:SM_P3D_Crag_A)
	{ S1D3("ArcadeHouse"), -17.4f, 0.0f, 1.22f, 0.f, 1.f },
	{ S1D3("TallHouse"), -17.4f, 10.2f, 1.22f, 4.f, 1.f },
	{ S1D3("LanternPost"), -17.4f, -8.0f, 1.22f, 8.f, 1.f },
	{ S1D3("GalleryInn"), 17.4f, 0.0f, -1.22f, -5.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 10.2f, -1.22f, -5.f, 1.f },
	{ S1D3("FingerPost"), 13.0f, -8.0f, -0.91f, 15.f, 1.f },
	{ S1D3("MarketStall"), 0.0f, 0.0f, -0.00f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B138L"), {     // along +0: no house (GustL:SM_SnowGust)
	{ S1D3("ToyMaker"), -17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("TallHouse"), -17.4f, 10.2f, -1.22f, 0.f, 1.f },
	{ S1D3("BenchPlanter"), -13.0f, -8.0f, -0.91f, 15.f, 1.f },
	{ S1D3("Granary"), 17.4f, 0.0f, 1.22f, 4.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 10.2f, 1.22f, -5.f, 1.f },
});
Set(TEXT("WldS1D3B138R"), {     // along +17: no house (PplP3ASkiSchoolAR30:SM_P3A_SkiSchool_A); along +0: no house (PplP3ASkiSchoolAR30:SM_P3A_SkiSchool_A)
	{ S1D3("RibbonPole"), 17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("ArcadeHouse"), -17.4f, 8.0f, 1.22f, 0.f, 1.f },
	{ S1D3("TallHouse"), -17.4f, 18.2f, 1.22f, 0.f, 1.f },
	{ S1D3("FountainTrough"), -17.4f, 0.0f, 1.22f, 8.f, 1.f },
});
Set(TEXT("WldS1D3B140L"), {     // along +17: no house (PplP3CTobogganL:SM_P3C_TobogganL_A); along -17: no house (PplP3CTobogganL:SM_P3C_TobogganL_A); along +0: no house (PplP3CTobogganL:SM_P3C_TobogganL_A)
	{ S1D3("WashLine"), 17.4f, 0.0f, 1.22f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B140R"), {
	{ S1D3("ArcadeHouse"), -17.4f, 0.0f, 1.22f, 4.f, 1.f },
	{ S1D3("TallHouse"), -17.4f, 10.2f, 1.22f, -5.f, 1.f },
	{ S1D3("SledRack"), -17.4f, -8.0f, 1.22f, -12.f, 1.f },
	{ S1D3("Bakery"), 17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 10.2f, -1.22f, 4.f, 1.f },
	{ S1D3("FingerPost"), 17.4f, -8.0f, -1.22f, -12.f, 1.f },
	{ S1D3("Woodstack"), 13.0f, -8.0f, -0.91f, -12.f, 1.f },
	{ S1D3("ToyMaker"), 0.0f, 4.0f, -0.00f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B142L"), {     // along -17: no house (PplP3AHikersAL30:SM_P3A_Hikers_A); along +17: no house (PplP3AHikersAL30:SM_P3A_Hikers_A); along +0: no house (PplP3AHikersAL30:SM_P3A_Hikers_A)
	{ S1D3("Woodstack"), -17.4f, 0.0f, -1.22f, 15.f, 1.f },
	{ S1D3("BenchPlanter"), 17.4f, 0.0f, 1.22f, 8.f, 1.f },
});
Set(TEXT("WldS1D3B142R"), {
	{ S1D3("ArcadeHouse"), 17.4f, 0.0f, -1.22f, 4.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 10.2f, -1.22f, 0.f, 1.f },
	{ S1D3("LanternPost"), 17.4f, -8.0f, -1.22f, -12.f, 1.f },
	{ S1D3("FountainTrough"), 13.0f, -8.0f, -0.91f, 15.f, 1.f },
	{ S1D3("GalleryInn"), -17.4f, 0.0f, 1.22f, 0.f, 1.f },
	{ S1D3("TallHouse"), -17.4f, 10.2f, 1.22f, 0.f, 1.f },
	{ S1D3("Bakery"), 0.0f, 0.0f, -0.00f, -5.f, 1.f },
	{ S1D3("MarketStall"), 0.0f, -8.0f, -0.00f, 15.f, 1.f },
});
Set(TEXT("WldS1D3B144L"), {
	{ S1D3("ToyMaker"), 17.4f, 0.0f, 1.22f, -5.f, 1.f },
	{ S1D3("Granary"), -17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("SledRack"), -13.0f, -8.0f, -0.91f, 8.f, 1.f },
	{ S1D3("TallHouse"), 0.0f, 4.0f, 0.00f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B144R"), {     // along +0: no house (PplP3APisteCrewBR30:SM_P3A_PisteCrew_B)
	{ S1D3("OrielHouse"), -17.4f, 0.0f, 1.22f, 0.f, 1.f },
	{ S1D3("TallHouse"), -17.4f, 10.2f, 1.22f, 4.f, 1.f },
	{ S1D3("ArcadeHouse"), 17.4f, 0.0f, -1.22f, 4.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 10.2f, -1.22f, 4.f, 1.f },
	{ S1D3("FingerPost"), 13.0f, -8.0f, -0.91f, -12.f, 1.f },
});
Set(TEXT("WldS1D3B146L"), {     // along +0: no house (PplP3CSkiTrailA:SM_P3C_SkiTrail_A)
	{ S1D3("Granary"), -17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("Woodstack"), -13.0f, -8.0f, -0.91f, 15.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 0.0f, 1.22f, 4.f, 1.f },
	{ S1D3("RibbonPole"), 0.0f, 4.0f, 0.00f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B146R"), {
	{ S1D3("ArcadeHouse"), 17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 10.2f, -1.22f, 4.f, 1.f },
	{ S1D3("FountainTrough"), 17.4f, -8.0f, -1.22f, 8.f, 1.f },
	{ S1D3("MarketStall"), 13.0f, -8.0f, -0.91f, 15.f, 1.f },
	{ S1D3("Bakery"), -17.4f, 0.0f, 1.22f, 0.f, 1.f },
	{ S1D3("TallHouse"), -17.4f, 10.2f, 1.22f, 4.f, 1.f },
	{ S1D3("WashLine"), -17.4f, -8.0f, 1.22f, 0.f, 1.f },
	{ S1D3("ToyMaker"), 0.0f, 0.0f, -0.00f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B148L"), {     // along +17: no house (PplP3AHikersBL30:SM_P3A_Hikers_B); along -17: no house (PplP3AHikersBL30:SM_P3A_Hikers_B); along +0: no house (PplP3AHikersBL30:SM_P3A_Hikers_B)
	{ S1D3("SledRack"), 17.4f, 0.0f, 1.22f, -12.f, 1.f },
	{ S1D3("FingerPost"), -17.4f, 0.0f, -1.22f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B148R"), {     // along +0: no house (PplP3DLookoutBR:SM_P3D_Lookout_B)
	{ S1D3("ArcadeHouse"), -17.4f, 4.0f, 1.22f, 0.f, 1.f },
	{ S1D3("GalleryInn"), 17.4f, 6.0f, -1.22f, 0.f, 1.f },
	{ S1D3("Woodstack"), 0.0f, 0.0f, -0.00f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B150L"), {
	{ S1D3("ToyMaker"), -17.4f, 0.0f, -1.22f, -5.f, 1.f },
	{ S1D3("TallHouse"), -17.4f, 10.2f, -1.22f, 4.f, 1.f },
	{ S1D3("BenchPlanter"), -17.4f, -8.0f, -1.22f, 0.f, 1.f },
	{ S1D3("LanternPost"), -13.0f, -8.0f, -0.91f, 8.f, 1.f },
	{ S1D3("TownGate"), 17.4f, 0.0f, 1.22f, 0.f, 1.f },
	{ S1D3("LanternPost"), 17.4f, -8.0f, 1.22f, -12.f, 1.f },
	{ S1D3("Granary"), 0.0f, 4.0f, 0.00f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B150R"), {     // along +0: no house (PplP3APisteCrewAR30:SM_P3A_PisteCrew_A)
	{ S1D3("OrielHouse"), 17.4f, 8.0f, -1.22f, 4.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 18.2f, -1.22f, 0.f, 1.f },
	{ S1D3("TownGate"), -17.4f, 8.0f, 1.22f, 0.f, 1.f },
	{ S1D3("ArcadeHouse"), -17.4f, 18.2f, 1.22f, 0.f, 1.f },
	{ S1D3("MarketStall"), -17.4f, 0.0f, 1.22f, -12.f, 1.f },
	{ S1D3("RibbonPole"), 0.0f, 0.0f, -0.00f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B166L"), {
	{ S1D3("ArcadeHouse"), 17.4f, 0.0f, 1.22f, 4.f, 1.f },
	{ S1D3("FountainTrough"), 17.4f, -8.0f, 1.22f, 15.f, 1.f },
	{ S1D3("TownGate"), -17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("WashLine"), -17.4f, -8.0f, -1.22f, 0.f, 1.f },
	{ S1D3("GalleryInn"), 0.0f, 0.0f, 0.00f, -5.f, 1.f },
	{ S1D3("SledRack"), 0.0f, -8.0f, 0.00f, 15.f, 1.f },
});
Set(TEXT("WldS1D3B166R"), {
	{ S1D3("Bakery"), -17.4f, 2.0f, 1.22f, 4.f, 1.f },
	{ S1D3("TownGate"), 17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("ToyMaker"), 0.0f, 2.0f, -0.00f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B168L"), {     // along -17: no house (scatter:SM_DogSled_A); along +17: no house (LiftLine:SM_SnowCannon); along +0: no house (LiftLine:SM_LiftStation)
	{ S1D3("FingerPost"), -17.4f, 0.0f, -1.22f, -12.f, 1.f },
	{ S1D3("Woodstack"), 17.4f, 0.0f, 1.22f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B168R"), {     // along +17: no house (DepotR:SM_SnowLot); along -17: no house (DepotR:SM_Snowcat); along +0: no house (DepotR:SM_ShedGroomer)
	{ S1D3("BenchPlanter"), 17.4f, 0.0f, -1.22f, 15.f, 1.f },
	{ S1D3("LanternPost"), -17.4f, 0.0f, 1.22f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B170L"), {     // along +0: no house (PplP3APisteCrewAL30:SM_P3A_PisteCrew_A)
	{ S1D3("ToyMaker"), 17.4f, 0.0f, 1.22f, -5.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 10.2f, 1.22f, 0.f, 1.f },
	{ S1D3("MarketStall"), 17.4f, -8.0f, 1.22f, -12.f, 1.f },
	{ S1D3("Granary"), -17.4f, 0.0f, -1.22f, -5.f, 1.f },
	{ S1D3("ArcadeHouse"), -17.4f, 10.2f, -1.22f, -5.f, 1.f },
	{ S1D3("BenchPlanter"), -13.0f, -8.0f, -0.91f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B170R"), {     // along +0: no house (PplP3DJudgesAR:SM_P3D_Judges_A)
	{ S1D3("OrielHouse"), -17.4f, 6.0f, 1.22f, -5.f, 1.f },
	{ S1D3("ArcadeHouse"), 17.4f, 2.0f, -1.22f, 0.f, 1.f },
	{ S1D3("RibbonPole"), 0.0f, 0.0f, -0.00f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B172L"), {     // along +0: no house (PplP3CTobogganL:SM_P3C_TobogganL_A)
	{ S1D3("Granary"), -17.4f, 0.0f, -1.22f, -5.f, 1.f },
	{ S1D3("FountainTrough"), -17.4f, -8.0f, -1.22f, 0.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 0.0f, 1.22f, 4.f, 1.f },
	{ S1D3("WashLine"), 17.4f, -8.0f, 1.22f, 0.f, 1.f },
	{ S1D3("SledRack"), 0.0f, 0.0f, 0.00f, 8.f, 1.f },
});
Set(TEXT("WldS1D3B172R"), {     // along +0: no house (scatter:SM_Snowmen_A)
	{ S1D3("ArcadeHouse"), 17.4f, 2.0f, -1.22f, 0.f, 1.f },
	{ S1D3("FingerPost"), 17.4f, -6.0f, -1.22f, 8.f, 1.f },
	{ S1D3("Woodstack"), 13.0f, -6.0f, -0.91f, 15.f, 1.f },
	{ S1D3("Bakery"), -17.4f, 2.0f, 1.22f, -5.f, 1.f },
	{ S1D3("Woodstack"), -17.4f, -6.0f, 1.22f, -12.f, 1.f },
	{ S1D3("BenchPlanter"), 0.0f, 0.0f, -0.00f, 15.f, 1.f },
});
Set(TEXT("WldS1D3B174L"), {     // along +17: no house (scatter:SM_Snowmen_A); along -17: no house (scatter:SM_Snowmen_A); along +0: no house (scatter:SM_Snowmen_A)
	{ S1D3("LanternPost"), 17.4f, 0.0f, 1.22f, 15.f, 1.f },
});
Set(TEXT("WldS1D3B174R"), {     // along -17: no house (DepotR:SM_Snowcat); along +17: no house (DepotR:SM_SnowLot); along +0: no house (DepotR:SM_ShedGroomer)
	{ S1D3("MarketStall"), -17.4f, 0.0f, 1.22f, 15.f, 1.f },
});
Set(TEXT("WldS1D3B176L"), {
	{ S1D3("ToyMaker"), -17.4f, 0.0f, -1.22f, 4.f, 1.f },
	{ S1D3("SledRack"), -13.0f, -8.0f, -0.91f, 8.f, 1.f },
	{ S1D3("Granary"), 17.4f, 0.0f, 1.22f, -5.f, 1.f },
	{ S1D3("TallHouse"), 0.0f, 0.0f, 0.00f, 0.f, 1.f },
	{ S1D3("FingerPost"), 4.4f, -8.0f, 0.31f, -12.f, 1.f },
});
Set(TEXT("WldS1D3B176R"), {     // along +0: no house (GroomR:SM_GroomerRun)
	{ S1D3("OrielHouse"), 17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("FingerPost"), 13.0f, -8.0f, -0.91f, 8.f, 1.f },
	{ S1D3("ArcadeHouse"), -17.4f, 0.0f, 1.22f, 4.f, 1.f },
	{ S1D3("RibbonPole"), 0.0f, 0.0f, -0.00f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B186R"), {     // along -17: no house (DepotR:SM_Snowcat); along +17: no house (DepotR:SM_SnowLot); along +0: no house (DepotR:SM_ShedGroomer)
	{ S1D3("FountainTrough"), -17.4f, 0.0f, 1.22f, -12.f, 1.f },
	{ S1D3("WashLine"), 17.4f, 0.0f, -1.22f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B188L"), {     // along +0: no house (LiftLine:SM_LiftStation)
	{ S1D3("Granary"), -17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("ArcadeHouse"), -17.4f, 10.2f, -1.22f, 0.f, 1.f },
	{ S1D3("SledRack"), -17.4f, -8.0f, -1.22f, -12.f, 1.f },
	{ S1D3("FountainTrough"), -11.8f, -8.0f, -0.83f, 8.f, 1.f },
	{ S1D3("OrielHouse"), 17.4f, 0.0f, 1.22f, -5.f, 1.f },
	{ S1D3("FingerPost"), 17.4f, -8.0f, 1.22f, 8.f, 1.f },
	{ S1D3("Woodstack"), 0.0f, 0.0f, 0.00f, 8.f, 1.f },
});
Set(TEXT("WldS1D3B188R"), {     // along +0: no house (PplP3DBalconyAR:SM_P3D_Balcony_A)
	{ S1D3("ArcadeHouse"), 17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("Granary"), 17.4f, 10.2f, -1.22f, 0.f, 1.f },
	{ S1D3("BenchPlanter"), 17.4f, -8.0f, -1.22f, 0.f, 1.f },
	{ S1D3("Woodstack"), 11.8f, -8.0f, -0.83f, 8.f, 1.f },
	{ S1D3("GalleryInn"), -17.4f, 0.0f, 1.22f, 0.f, 1.f },
	{ S1D3("Granary"), -17.4f, 10.2f, 1.22f, 0.f, 1.f },
	{ S1D3("LanternPost"), -17.4f, -8.0f, 1.22f, 0.f, 1.f },
	{ S1D3("MarketStall"), 0.0f, 0.0f, -0.00f, 15.f, 1.f },
});
Set(TEXT("WldS1D3B190L"), {     // along +0: no house (PplP3APisteCrewAL30:SM_P3A_PisteCrew_A)
	{ S1D3("ToyMaker"), 17.4f, 0.0f, 1.22f, 0.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 10.2f, 1.22f, 0.f, 1.f },
	{ S1D3("Granary"), -17.4f, 0.0f, -1.22f, 4.f, 1.f },
	{ S1D3("ArcadeHouse"), -17.4f, 10.2f, -1.22f, 0.f, 1.f },
	{ S1D3("MarketStall"), -13.0f, -8.0f, -0.91f, -12.f, 1.f },
});
Set(TEXT("WldS1D3B190R"), {
	{ S1D3("OrielHouse"), -17.4f, 0.0f, 1.22f, -5.f, 1.f },
	{ S1D3("ArcadeHouse"), 17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("Bakery"), 0.0f, 0.0f, -0.00f, 4.f, 1.f },
});
Set(TEXT("WldS1D3B192L"), {     // along +0: no house (scatter:SM_Snowmen_A)
	{ S1D3("Granary"), -17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("FountainTrough"), -13.0f, -8.0f, -0.91f, 0.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 0.0f, 1.22f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B192R"), {     // along +17: no house (DepotR:SM_SnowLot); along -17: no house (DepotR:SM_Snowcat); along +0: no house (DepotR:SM_ShedGroomer)
	{ S1D3("RibbonPole"), 17.4f, 0.0f, -1.22f, 0.f, 1.f },
	{ S1D3("FountainTrough"), -17.4f, 0.0f, 1.22f, 8.f, 1.f },
});
Set(TEXT("WldS1D3B194L"), {
	{ S1D3("TallHouse"), 17.4f, 0.0f, 1.22f, 0.f, 1.f },
	{ S1D3("Bakery"), 17.4f, 10.2f, 1.22f, -5.f, 1.f },
	{ S1D3("WashLine"), 17.4f, -8.0f, 1.22f, 0.f, 1.f },
	{ S1D3("ArcadeHouse"), -17.4f, 0.0f, -1.22f, -5.f, 1.f },
	{ S1D3("Granary"), -17.4f, 10.2f, -1.22f, 0.f, 1.f },
	{ S1D3("SledRack"), -17.4f, -8.0f, -1.22f, 15.f, 1.f },
	{ S1D3("LanternPost"), -13.0f, -8.0f, -0.91f, 0.f, 1.f },
	{ S1D3("Bakery"), 0.0f, 0.0f, 0.00f, 0.f, 1.f },
});
Set(TEXT("WldS1D3B194R"), {
	{ S1D3("Granary"), -17.4f, 0.0f, 1.22f, 0.f, 1.f },
	{ S1D3("ArcadeHouse"), -17.4f, 10.2f, 1.22f, 4.f, 1.f },
	{ S1D3("FingerPost"), -17.4f, -8.0f, 1.22f, -12.f, 1.f },
	{ S1D3("TallHouse"), 17.4f, 0.0f, -1.22f, 4.f, 1.f },
	{ S1D3("Bakery"), 17.4f, 10.2f, -1.22f, 0.f, 1.f },
	{ S1D3("Woodstack"), 17.4f, -8.0f, -1.22f, -12.f, 1.f },
	{ S1D3("FountainTrough"), 11.8f, -8.0f, -0.83f, -12.f, 1.f },
	{ S1D3("ArcadeHouse"), 0.0f, 0.0f, -0.00f, 4.f, 1.f },
});
Set(TEXT("WldS1D3B196L"), {     // along +0: no house (PplP3AIglooAL30:SM_P3A_Igloo_A)
	{ S1D3("Bakery"), -17.4f, 8.0f, -1.22f, 0.f, 1.f },
	{ S1D3("BenchPlanter"), -17.4f, 0.0f, -1.22f, 15.f, 1.f },
	{ S1D3("SledRack"), -11.8f, 0.0f, -0.83f, 0.f, 1.f },
	{ S1D3("TownGate"), 17.4f, 8.0f, 1.22f, 0.f, 1.f },
	{ S1D3("LanternPost"), 17.4f, 0.0f, 1.22f, 8.f, 1.f },
	{ S1D3("MarketStall"), 0.0f, 0.0f, 0.00f, -12.f, 1.f },
});
Set(TEXT("WldS1D3B196R"), {     // along +0: no house (PplP3DBalconyAR:SM_P3D_Balcony_A)
	{ S1D3("TallHouse"), 17.4f, 0.0f, -1.22f, 4.f, 1.f },
	{ S1D3("Bakery"), 17.4f, 10.2f, -1.22f, 4.f, 1.f },
	{ S1D3("TownGate"), -17.4f, 0.0f, 1.22f, 0.f, 1.f },
});
Set(TEXT("WldS1D3Hero"), { { S1D3("GreatCowbell"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Zone(TEXT("World s1d3, The valley run, slot 126"), 5.688f, 5.712f);       // 5.700 km
	Site(TEXT("WldS1D3B126L"), -1, 52.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D3B126R"), 1, 52.0f, 4, 2, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 128"), 5.779f, 5.803f);       // 5.791 km
	Site(TEXT("WldS1D3B128L"), -1, 44.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D3B128R"), 1, 52.0f, 4, 0, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 130"), 5.869f, 5.893f);       // 5.881 km
	Site(TEXT("WldS1D3B130L"), -1, 52.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D3B130R"), 1, 56.0f, 4, 2, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 132"), 5.960f, 5.984f);       // 5.972 km
	Site(TEXT("WldS1D3B132LFx0"), -1, 52.0f, 4, 0, 1.f);   // FIX ROUND: L6: its box was 2.5 m inside SkiHillL_A
	Site(TEXT("WldS1D3B132R"), 1, 44.0f, 4, 0, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 134"), 6.050f, 6.074f);       // 6.062 km
	Site(TEXT("WldS1D3B134L"), -1, 64.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D3B134R"), 1, 52.0f, 4, 2, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 136"), 6.141f, 6.165f);       // 6.153 km
	Site(TEXT("WldS1D3B136L"), -1, 52.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D3B136R"), 1, 52.0f, 4, 0, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 138"), 6.231f, 6.255f);       // 6.243 km
	Site(TEXT("WldS1D3B138L"), -1, 52.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D3B138R"), 1, 44.0f, 4, 2, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 140"), 6.322f, 6.346f);       // 6.334 km
	Site(TEXT("WldS1D3B140L"), -1, 56.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D3B140R"), 1, 52.0f, 4, 0, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 142"), 6.412f, 6.436f);       // 6.424 km
	Site(TEXT("WldS1D3B142L"), -1, 44.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D3B142R"), 1, 52.0f, 4, 2, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 144"), 6.503f, 6.527f);       // 6.515 km
	Site(TEXT("WldS1D3B144L"), -1, 52.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D3B144R"), 1, 52.0f, 4, 0, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 146"), 6.593f, 6.617f);       // 6.605 km
	Site(TEXT("WldS1D3B146L"), -1, 52.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D3B146R"), 1, 52.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D3Hero"), 1, 120.0f, 4, 2, 1.f);       // THE GREAT COWBELL
Zone(TEXT("World s1d3, The valley run, slot 148"), 6.684f, 6.708f);       // 6.696 km
	Site(TEXT("WldS1D3B148L"), -1, 44.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D3B148R"), 1, 56.0f, 4, 0, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 150"), 6.774f, 6.798f);       // 6.786 km
	Site(TEXT("WldS1D3B150L"), -1, 52.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D3B150R"), 1, 44.0f, 4, 2, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 166"), 7.498f, 7.522f);       // 7.510 km
	Site(TEXT("WldS1D3B166L"), -1, 52.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D3B166RFx2"), 1, 60.0f, 4, 2, 1.f);   // FIX ROUND: L6: its box was 5.0 m inside P3D_Pylon_A
Zone(TEXT("World s1d3, The valley run, slot 168"), 7.588f, 7.612f);       // 7.600 km
	Site(TEXT("WldS1D3B168L"), -1, 56.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D3B168R"), 1, 44.0f, 4, 0, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 170"), 7.679f, 7.703f);       // 7.691 km
	Site(TEXT("WldS1D3B170L"), -1, 52.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D3B170R"), 1, 56.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D3Hero"), -1, 120.0f, 4, 2, 1.f);       // THE GREAT COWBELL
Zone(TEXT("World s1d3, The valley run, slot 172"), 7.769f, 7.793f);       // 7.781 km
	Site(TEXT("WldS1D3B172L"), -1, 52.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D3B172R"), 1, 50.0f, 4, 0, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 174"), 7.860f, 7.884f);       // 7.872 km
	Site(TEXT("WldS1D3B174L"), -1, 44.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D3B174R"), 1, 60.0f, 4, 2, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 176"), 7.950f, 7.974f);       // 7.962 km
	Site(TEXT("WldS1D3B176L"), -1, 52.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D3B176RFx2"), 1, 52.0f, 4, 0, 1.f);   // FIX ROUND: L6: its box was 3.0 m inside SkiHill_A
Zone(TEXT("World s1d3, The valley run, slot 186"), 8.403f, 8.427f);       // 8.415 km
	Site(TEXT("WldS1D3B186R"), 1, 44.0f, 4, 2, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 188"), 8.493f, 8.517f);       // 8.505 km
	Site(TEXT("WldS1D3B188L"), -1, 52.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D3B188R"), 1, 52.0f, 4, 0, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 190"), 8.584f, 8.608f);       // 8.596 km
	Site(TEXT("WldS1D3B190L"), -1, 52.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D3B190R"), 1, 52.0f, 4, 2, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 192"), 8.674f, 8.698f);       // 8.686 km
	Site(TEXT("WldS1D3B192L"), -1, 52.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D3B192R"), 1, 44.0f, 4, 0, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 194"), 8.765f, 8.789f);       // 8.777 km
	Site(TEXT("WldS1D3B194LFx3"), -1, 52.0f, 4, 2, 1.f);   // FIX ROUND: L6: its box was 4.1 m inside SkiHillL_A
	Site(TEXT("WldS1D3B194R"), 1, 52.0f, 4, 2, 1.f);
Zone(TEXT("World s1d3, The valley run, slot 196"), 8.855f, 8.879f);       // 8.867 km
	Site(TEXT("WldS1D3B196L"), -1, 44.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D3B196R"), 1, 52.0f, 4, 0, 1.f);
#undef S1D3
// ---- END s1d3 ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People5 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1d4 (district 4 "The Abominable's glacier", the expedition camp, ft_world_ice_s1d4.py) ----
// 17 meshes. Every rule is on chance 1 in a Zone of ONE slot (even slots 198 to 256, 8.957 to 11.581 km): 232 pieces are written
// (camp Sites 32 x 4 = 128, near Sites 24 x 3 = 72, the ship 2, route flags 15 slots x 2 sides = 30). By the brief's
// estimate (x 0.75, the track's share left out): (128 + 72 + 2 + 30) x 0.75 = about 174 placed. Most of one mesh: RouteFlags 30, FuelDump 20, CrateStack 20.
// Bands and slots: camps (buildings) left 61 m, right 67 m, on slots s % 6 == 0 or 4, cut out of the slots where pass 3 has a
// herd or a ski trail 64 m out (p3c); the right keeps 67 m because pass 3 (p3d) stands 54 to 56 m out on every even slot
// there; the dressing table's second ice wall (62 to 72 m, s % 6 == 2) leaves those slots without a camp on either side.
// Near sets 25.8 m out: the tall piece at the anchor (along 0), two low ones 9 m either way; never on a slot with an ice
// climb 31 m out (left s % 6 == 2, right s % 6 == 0), the crew's slots (220, 232, 244, 256) or pass 3's scenes 30 m out.
// Route flags (low, 1.1 m) 17.6 to 17.9 m, both sides, slots s % 4 == 0. The ICEBOUND SHIP right, 150 m, slots 224 and 236
// (10.13 and 10.68 km): s % 6 == 2, between the ice-fall sets (120 m, s % 3 == 0) and inside the serac fields (205 m).
// Up of a set's part = -0.07 x along on the right, +0.07 x along on the left (the dressing table's own rule for this hill).
#define S1D4(n) TEXT("/Game/World/IceMountain/SM_S1D4_" n)
Set(TEXT("WldS1D4CampAR"), { { S1D4("BigDome"), 0.0f, 0.0f, 0.0f, -8.0f, 1.f }, { S1D4("VaultHut"), -15.0f, -1.0f, 1.05f, -8.0f, 1.f }, { S1D4("StoreDome"), 14.0f, 1.0f, -0.98f, -8.0f, 1.f }, { S1D4("FuelDump"), -9.5f, -6.5f, 0.67f, -8.0f, 1.f } });
Set(TEXT("WldS1D4CampAL"), { { S1D4("BigDome"), 0.0f, 0.0f, 0.0f, 8.0f, 1.f }, { S1D4("VaultHut"), -15.0f, -1.0f, -1.05f, 8.0f, 1.f }, { S1D4("StoreDome"), 14.0f, 1.0f, 0.98f, 8.0f, 1.f }, { S1D4("FuelDump"), -9.5f, -6.5f, -0.67f, 8.0f, 1.f } });
Set(TEXT("WldS1D4CampBR"), { { S1D4("MessTent"), 0.0f, 0.0f, 0.0f, -8.0f, 1.f }, { S1D4("RadioHut"), 13.0f, 2.0f, -0.91f, -8.0f, 1.f }, { S1D4("BunkTents"), -14.0f, -2.0f, 0.98f, -8.0f, 1.f }, { S1D4("DryingLine"), 7.6f, -6.5f, -0.53f, -8.0f, 1.f } });
Set(TEXT("WldS1D4CampBL"), { { S1D4("MessTent"), 0.0f, 0.0f, 0.0f, 8.0f, 1.f }, { S1D4("RadioHut"), 13.0f, 2.0f, 0.91f, 8.0f, 1.f }, { S1D4("BunkTents"), -14.0f, -2.0f, -0.98f, 8.0f, 1.f }, { S1D4("DryingLine"), 7.6f, -6.5f, 0.53f, 8.0f, 1.f } });   // FIX ROUND (W8): in the set below the BalloonShed stood 70.0 m out on the right (63 m on the left); now 69.5: one mesh on one side of the 70 m shadow line
Set(TEXT("WldS1D4CampCR"), { { S1D4("SledgeGarage"), 0.0f, 0.0f, 0.0f, -8.0f, 1.f }, { S1D4("VaultHut"), 13.0f, 0.0f, -0.91f, -8.0f, 1.f }, { S1D4("BalloonShed"), -13.0f, 2.5f, 0.91f, -8.0f, 1.f }, { S1D4("CrateStack"), 7.0f, -6.5f, -0.49f, -8.0f, 1.f } });
Set(TEXT("WldS1D4CampCL"), { { S1D4("SledgeGarage"), 0.0f, 0.0f, 0.0f, 8.0f, 1.f }, { S1D4("VaultHut"), 13.0f, 0.0f, 0.91f, 8.0f, 1.f }, { S1D4("BalloonShed"), -13.0f, 3.0f, -0.91f, 8.0f, 1.f }, { S1D4("CrateStack"), 7.0f, -6.5f, 0.49f, 8.0f, 1.f } });
Set(TEXT("WldS1D4CampDR"), { { S1D4("BalloonShed"), 0.0f, 2.0f, 0.0f, -8.0f, 1.f }, { S1D4("BigDome"), -14.5f, 0.0f, 1.02f, -8.0f, 1.f }, { S1D4("MessTent"), 13.5f, -1.0f, -0.95f, -8.0f, 1.f }, { S1D4("SignalMast"), 6.5f, -6.0f, -0.46f, -8.0f, 1.f } });
Set(TEXT("WldS1D4CampDL"), { { S1D4("BalloonShed"), 0.0f, 2.0f, 0.0f, 8.0f, 1.f }, { S1D4("BigDome"), -14.5f, 0.0f, -1.02f, 8.0f, 1.f }, { S1D4("MessTent"), 13.5f, -1.0f, 0.95f, 8.0f, 1.f }, { S1D4("SignalMast"), 6.5f, -6.0f, 0.46f, 8.0f, 1.f } });
Set(TEXT("WldS1D4NearAR"), { { S1D4("SignalMast"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f }, { S1D4("CrateStack"), -9.0f, 0.0f, 0.63f, 0.0f, 1.f }, { S1D4("LoadedSledge"), 9.0f, 0.0f, -0.63f, 0.0f, 1.f } });
Set(TEXT("WldS1D4NearAL"), { { S1D4("SignalMast"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f }, { S1D4("CrateStack"), -9.0f, 0.0f, -0.63f, 0.0f, 1.f }, { S1D4("LoadedSledge"), 9.0f, 0.0f, 0.63f, 0.0f, 1.f } });
Set(TEXT("WldS1D4NearBR"), { { S1D4("DepotCairn"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f }, { S1D4("FuelDump"), -9.0f, 0.0f, 0.63f, 0.0f, 1.f }, { S1D4("DryingLine"), 9.0f, 0.0f, -0.63f, 0.0f, 1.f } });
Set(TEXT("WldS1D4NearBL"), { { S1D4("DepotCairn"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f }, { S1D4("FuelDump"), -9.0f, 0.0f, -0.63f, 0.0f, 1.f }, { S1D4("DryingLine"), 9.0f, 0.0f, 0.63f, 0.0f, 1.f } });
Set(TEXT("WldS1D4NearCR"), { { S1D4("Theodolite"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f }, { S1D4("LoadedSledge"), -9.0f, 0.0f, 0.63f, 0.0f, 1.f }, { S1D4("CrateStack"), 9.0f, 0.0f, -0.63f, 0.0f, 1.f } });
Set(TEXT("WldS1D4NearCL"), { { S1D4("Theodolite"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f }, { S1D4("LoadedSledge"), -9.0f, 0.0f, -0.63f, 0.0f, 1.f }, { S1D4("CrateStack"), 9.0f, 0.0f, 0.63f, 0.0f, 1.f } });
Set(TEXT("WldS1D4NearDR"), { { S1D4("DepotCairn"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f }, { S1D4("DryingLine"), -9.0f, 0.0f, 0.63f, 0.0f, 1.f }, { S1D4("FuelDump"), 9.0f, 0.0f, -0.63f, 0.0f, 1.f } });
Set(TEXT("WldS1D4NearDL"), { { S1D4("DepotCairn"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f }, { S1D4("DryingLine"), -9.0f, 0.0f, -0.63f, 0.0f, 1.f }, { S1D4("FuelDump"), 9.0f, 0.0f, 0.63f, 0.0f, 1.f } });
Set(TEXT("WldS1D4Ship"), { { S1D4("IceboundShip"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Zone(TEXT("World s1d4, the expedition, slot 198"), 8.952f, 8.964f);
	Site(TEXT("WldS1D4CampAL"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4CampCRFx2"), 1, 67.0f, 1, 0, 1.f);   // FIX ROUND: L6: its box was 2.5 m inside P3C_Skidoos_B
	Site(TEXT("WldS1D4NearAL"), -1, 25.8f, 1, 0, 1.f);
Zone(TEXT("World s1d4, the expedition, slot 200"), 9.042f, 9.054f);
	Site(TEXT("WldS1D4NearBR"), 1, 25.8f, 1, 0, 1.f);
	Scatter(S1D4("RouteFlags"), 17.6f, 17.9f, 1, 0, 1.f, 1.f, 0.f, 0, 4.f);
Zone(TEXT("World s1d4, the expedition, slot 202"), 9.132f, 9.144f);
	Site(TEXT("WldS1D4CampDR"), 1, 67.0f, 1, 0, 1.f);
Zone(TEXT("World s1d4, the expedition, slot 204"), 9.223f, 9.235f);
	Site(TEXT("WldS1D4CampBL"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4CampARFx0"), 1, 67.0f, 1, 0, 1.f);   // FIX ROUND: L6: its box was 6.0 m inside P3D_HeliDeck_A
	Site(TEXT("WldS1D4NearBL"), -1, 25.8f, 1, 0, 1.f);
	Scatter(S1D4("RouteFlags"), 17.6f, 17.9f, 1, 0, 1.f, 1.f, 0.f, 0, 4.f);
Zone(TEXT("World s1d4, the expedition, slot 206"), 9.313f, 9.325f);
	Site(TEXT("WldS1D4NearCR"), 1, 25.8f, 1, 0, 1.f);
Zone(TEXT("World s1d4, the expedition, slot 208"), 9.404f, 9.416f);
	Site(TEXT("WldS1D4CampCL"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4CampBR"), 1, 67.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4NearCLFx0"), -1, 25.8f, 1, 0, 1.f);   // FIX ROUND: L6: its box was 2.8 m inside CrewApplauseB_Snow
	Site(TEXT("WldS1D4NearDRFx2"), 1, 25.8f, 1, 0, 1.f);   // FIX ROUND: L6: its box was 2.3 m inside CrewApplauseA_Snow
	Scatter(S1D4("RouteFlags"), 17.6f, 17.9f, 1, 0, 1.f, 1.f, 0.f, 0, 4.f);
Zone(TEXT("World s1d4, the expedition, slot 210"), 9.494f, 9.506f);
	Site(TEXT("WldS1D4CampDL"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4NearDL"), -1, 25.8f, 1, 0, 1.f);
Zone(TEXT("World s1d4, the expedition, slot 212"), 9.585f, 9.597f);
	Site(TEXT("WldS1D4NearAR"), 1, 25.8f, 1, 0, 1.f);
	Scatter(S1D4("RouteFlags"), 17.6f, 17.9f, 1, 0, 1.f, 1.f, 0.f, 0, 4.f);
Zone(TEXT("World s1d4, the expedition, slot 214"), 9.675f, 9.687f);
	Site(TEXT("WldS1D4CampAL"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4CampCR"), 1, 67.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4NearAL"), -1, 25.8f, 1, 0, 1.f);
Zone(TEXT("World s1d4, the expedition, slot 216"), 9.766f, 9.778f);
	Site(TEXT("WldS1D4CampBL"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4NearBL"), -1, 25.8f, 1, 0, 1.f);
	Scatter(S1D4("RouteFlags"), 17.6f, 17.9f, 1, 0, 1.f, 1.f, 0.f, 0, 4.f);
Zone(TEXT("World s1d4, the expedition, slot 218"), 9.856f, 9.868f);
	Site(TEXT("WldS1D4NearBR"), 1, 25.8f, 1, 0, 1.f);
Zone(TEXT("World s1d4, the expedition, slot 220"), 9.947f, 9.959f);
	Site(TEXT("WldS1D4CampCL"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4CampDR"), 1, 67.0f, 1, 0, 1.f);
	Scatter(S1D4("RouteFlags"), 17.6f, 17.9f, 1, 0, 1.f, 1.f, 0.f, 0, 4.f);
Zone(TEXT("World s1d4, the expedition, slot 222"), 10.037f, 10.049f);
	Site(TEXT("WldS1D4CampDL"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4CampARFx0"), 1, 67.0f, 1, 0, 1.f);   // FIX ROUND: L6: its box was 4.5 m inside P3D_Bridge_B
	Site(TEXT("WldS1D4NearCL"), -1, 25.8f, 1, 0, 1.f);
Zone(TEXT("World s1d4, the expedition, slot 224"), 10.128f, 10.140f);
	Site(TEXT("WldS1D4NearCR"), 1, 25.8f, 1, 0, 1.f);
	Site(TEXT("WldS1D4Ship"), 1, 150.0f, 1, 0, 1.f, 1.5f);   // FIX ROUND: driver 2: the ship did not show over the ice walls at scale 1; 1.5 (hull 105 m, top 52 m)
	Scatter(S1D4("RouteFlags"), 17.6f, 17.9f, 1, 0, 1.f, 1.f, 0.f, 0, 4.f);
Zone(TEXT("World s1d4, the expedition, slot 226"), 10.218f, 10.230f);
	Site(TEXT("WldS1D4CampBR"), 1, 67.0f, 1, 0, 1.f);
Zone(TEXT("World s1d4, the expedition, slot 228"), 10.309f, 10.321f);
	Site(TEXT("WldS1D4CampAL"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4CampCRFx2"), 1, 67.0f, 1, 0, 1.f);   // FIX ROUND: L6: its box was 2.5 m inside P3C_Skidoos_B
	Site(TEXT("WldS1D4NearDL"), -1, 25.8f, 1, 0, 1.f);
	Scatter(S1D4("RouteFlags"), 17.6f, 17.9f, 1, 0, 1.f, 1.f, 0.f, 0, 4.f);
Zone(TEXT("World s1d4, the expedition, slot 230"), 10.399f, 10.411f);
	Site(TEXT("WldS1D4NearDR"), 1, 25.8f, 1, 0, 1.f);
Zone(TEXT("World s1d4, the expedition, slot 232"), 10.490f, 10.502f);
	Site(TEXT("WldS1D4CampDR"), 1, 67.0f, 1, 0, 1.f);
	Scatter(S1D4("RouteFlags"), 17.6f, 17.9f, 1, 0, 1.f, 1.f, 0.f, 0, 4.f);
Zone(TEXT("World s1d4, the expedition, slot 234"), 10.580f, 10.592f);
	Site(TEXT("WldS1D4CampBL"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4CampARFx0"), 1, 67.0f, 1, 0, 1.f);   // FIX ROUND: L6: its box was 3.0 m inside P3D_IceArch_A
	Site(TEXT("WldS1D4NearAL"), -1, 25.8f, 1, 0, 1.f);
Zone(TEXT("World s1d4, the expedition, slot 236"), 10.671f, 10.683f);
	Site(TEXT("WldS1D4NearAR"), 1, 25.8f, 1, 0, 1.f);
	Site(TEXT("WldS1D4Ship"), 1, 150.0f, 1, 0, -1.f);   // FIX ROUND: L7: the second ship reached the ice-fall set of slot 237; one ship is left, larger (slot 224)
	Scatter(S1D4("RouteFlags"), 17.6f, 17.9f, 1, 0, 1.f, 1.f, 0.f, 0, 4.f);
Zone(TEXT("World s1d4, the expedition, slot 238"), 10.761f, 10.773f);
	Site(TEXT("WldS1D4CampCL"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4CampBR"), 1, 67.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4NearBL"), -1, 25.8f, 1, 0, 1.f);
Zone(TEXT("World s1d4, the expedition, slot 240"), 10.852f, 10.864f);
	Site(TEXT("WldS1D4CampDL"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4NearCL"), -1, 25.8f, 1, 0, 1.f);
	Scatter(S1D4("RouteFlags"), 17.6f, 17.9f, 1, 0, 1.f, 1.f, 0.f, 0, 4.f);
Zone(TEXT("World s1d4, the expedition, slot 242"), 10.942f, 10.954f);
	Site(TEXT("WldS1D4NearBR"), 1, 25.8f, 1, 0, 1.f);
Zone(TEXT("World s1d4, the expedition, slot 244"), 11.033f, 11.045f);
	Site(TEXT("WldS1D4CampAL"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4CampCR"), 1, 67.0f, 1, 0, 1.f);
	Scatter(S1D4("RouteFlags"), 17.6f, 17.9f, 1, 0, 1.f, 1.f, 0.f, 0, 4.f);
Zone(TEXT("World s1d4, the expedition, slot 246"), 11.123f, 11.135f);
	Site(TEXT("WldS1D4CampBL"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4NearDL"), -1, 25.8f, 1, 0, 1.f);
Zone(TEXT("World s1d4, the expedition, slot 248"), 11.214f, 11.226f);
	Site(TEXT("WldS1D4NearCR"), 1, 25.8f, 1, 0, 1.f);
	Scatter(S1D4("RouteFlags"), 17.6f, 17.9f, 1, 0, 1.f, 1.f, 0.f, 0, 4.f);
Zone(TEXT("World s1d4, the expedition, slot 250"), 11.304f, 11.316f);
	Site(TEXT("WldS1D4CampCL"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4CampDR"), 1, 67.0f, 1, 0, 1.f);
Zone(TEXT("World s1d4, the expedition, slot 252"), 11.394f, 11.406f);
	Site(TEXT("WldS1D4CampDL"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D4CampARFx0"), 1, 67.0f, 1, 0, 1.f);   // FIX ROUND: L6: its box was 6.0 m inside P3D_HeliDeck_A
	Site(TEXT("WldS1D4NearAL"), -1, 25.8f, 1, 0, 1.f);
	Scatter(S1D4("RouteFlags"), 17.6f, 17.9f, 1, 0, 1.f, 1.f, 0.f, 0, 4.f);
Zone(TEXT("World s1d4, the expedition, slot 254"), 11.485f, 11.497f);
	Site(TEXT("WldS1D4NearDR"), 1, 25.8f, 1, 0, 1.f);
Zone(TEXT("World s1d4, the expedition, slot 256"), 11.575f, 11.587f);
	Site(TEXT("WldS1D4CampBR"), 1, 67.0f, 1, 0, 1.f);
	Scatter(S1D4("RouteFlags"), 17.6f, 17.9f, 1, 0, 1.f, 1.f, 0.f, 0, 4.f);
#undef S1D4
// ---- END s1d4 ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People5 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1d5 (district 5 "The white ridge": the aurora watch, ft_world_ice_s1d5.py) ----
// 23 meshes: 14 designs (6 buildings, 7 props, the hero); the 9 that show the wind have a left-hand twin (...L).
// THE WIND blows down the road: a piece on the left is turned round, so the left uses the L twins (the same drawing
// mirrored), and every roof, chimney, streamer and drift leans the same way on both sides. The hero is an L piece.
// 11.6 to 14.28 km = slots 257 to 315, EVEN slots only (258 to 314; s % 4 == 0 or 2). Slot 316 (14.296 km) is left
// out: a set's far end stands 22 m down the road from its slot, which is past 14.3 km.
// Estimate (slots counted one by one by C:/ftwork/jobs/L5s1d5/gen_block.py, x parts; every chance is 1):
//   right: RowA 7 x 5 = 35, RowB 7 x 5 = 35, VergeA 7 x 3 = 21, VergeB 3 x 4 = 12, F1 4 x 5 = 20, F2 4 x 6 = 24,
//          F3 4 x 4 = 16, F4 3 x 6 = 18;   left: F1 3 x 5 = 15, F2 4 x 6 = 24, F3 4 x 4 = 16, F4 3 x 6 = 18,
//          VergeA 8 x 3 = 24, VergeB 7 x 4 = 28;   the hero 2.   Sum 308; x 0.75 (the brief's share) = about 231 placed.
// The most of one mesh: SkyBoard 31, Telescope 31, LampCairn 31, WindBreak 24, Anemometer 22 (all others 15 or fewer); the hero 2.
// Where they stand (both tables laid on these slots by C:/ftwork/jobs/L5s1d5/occ.py). On an even slot the older tables
// have, at the slot's MIDDLE: a pass 3 scene 31.5 m out on both sides, a watchers' bank 42 m or a pass 3 group 46 m
// (turned 25 to 35 degrees: they reach 10 m either way along the road), ski trails 38.5 m (31 m long) on the right on
// most slots of s % 4 == 0 and on the left on 270, 286, 302; lookouts 65 to 67 m on the right; snow gusts 58 m on the
// left (their snow travels 20 m: it may blow past the left's buildings, not checked). So:
//   26.4 m     LOW props (tops under 2 m), behind the snow fence (it stands at 24.7 m) at the two ENDS of a slot,
//              11.5 to 19.5 m up and down the road from its middle.
//   28.6 m     TALL props, the same ends of the slot.
//   38 m       two buildings a slot at its ends (16 to 17 m from the middle): right on s % 4 == 2, left on s % 4 == 0
//              (the slots without a ski trail on that side).
//   55 m       right, s % 4 == 0: a row of three buildings between the pass 3 groups (46 m) and the lookouts (65 m).
//   100 m      THE WIND HARP, left, slots 284 (12.85 km) and 296 (13.39 km): clear of the rack railway (its foot
//              98 m out on slots 280, 290, 300, running back up the road and away from it).
// A set's part takes its anchor's height and the hill falls 7 in 100: Up = -0.07 x along on the right, +0.07 x along on the left.
// TALL pieces: smallest x min 28.6. LOW pieces (VaneRow, LampCairn, WindBreak): 26.4. Nothing nearer than 25.7 m.
// Steady glow only (windows 0.9, lamps 1.3, stars and the ember 0.6); nothing blinks. The anemometer's cups go round
// 0.55 times a second. NOT COMPILED AND NOT SEEN IN THE GAME when written.
#define S1D5(n) TEXT("/Game/World/IceMountain/SM_S1D5_" n)
Set(TEXT("WldS1D5RowAR"), {                      // anchor 55 m: kite loft, refuge, one-leg cabin; the sky board and the anemometer between them (right)
	{ S1D5("KiteLoft"),           -15.5f,   0.0f,  1.09f,  0.0f, 1.f },
	{ S1D5("Refuge"),               0.0f,   0.0f,   0.0f,  0.0f, 1.f },
	{ S1D5("LegCabin"),            15.5f,   0.5f, -1.09f,  0.0f, 1.f },
	{ S1D5("SkyBoard"),            -8.9f,  -3.9f,  0.62f,  8.0f, 1.f },
	{ S1D5("Anemometer"),           8.9f,  -3.5f, -0.62f,  0.0f, 1.f },
});
Set(TEXT("WldS1D5RowBR"), {                      // anchor 55 m: slit dome, wind wall, instrument house; the telescope and a lamp cairn between them (right)
	{ S1D5("SlitDome"),           -15.5f,   0.0f,  1.09f,  0.0f, 1.f },
	{ S1D5("WindWall"),             0.0f,   0.0f,   0.0f,  0.0f, 1.f },
	{ S1D5("InstrumentHouse"),     15.5f,   0.0f, -1.09f,  0.0f, 1.f },
	{ S1D5("Telescope"),            8.4f,  -3.2f, -0.59f,  0.0f, 1.f },
	{ S1D5("LampCairn"),           -8.6f,  -3.6f,   0.6f, 20.0f, 1.f },
});
Set(TEXT("WldS1D5F1R"), {                        // anchor 38 m, the two ends of the slot (the cast stands in its middle): refuge and one-leg cabin; props 28.6 m, low 26.4 m (right)
	{ S1D5("Refuge"),             -16.5f,   0.0f,  1.16f,  0.0f, 1.f },
	{ S1D5("LegCabin"),            17.0f,   0.0f, -1.19f,  0.0f, 1.f },
	{ S1D5("StreamerLine"),       -15.5f,  -9.4f,  1.09f,  0.0f, 1.f },
	{ S1D5("Anemometer"),          16.0f,  -9.4f, -1.12f,  0.0f, 1.f },
	{ S1D5("VaneRow"),            -15.5f, -11.6f,  1.09f,  0.0f, 1.f },
});
Set(TEXT("WldS1D5F1L"), {                        // anchor 38 m, the two ends of the slot (the cast stands in its middle): refuge and one-leg cabin; props 28.6 m, low 26.4 m (left: the L twins)
	{ S1D5("RefugeL"),            -16.5f,   0.0f, -1.16f,  0.0f, 1.f },
	{ S1D5("LegCabinL"),           17.0f,   0.0f,  1.19f,  0.0f, 1.f },
	{ S1D5("StreamerLineL"),      -15.5f,  -9.4f, -1.09f,  0.0f, 1.f },
	{ S1D5("AnemometerL"),         16.0f,  -9.4f,  1.12f,  0.0f, 1.f },
	{ S1D5("VaneRowL"),           -15.5f, -11.6f, -1.09f,  0.0f, 1.f },
});
Set(TEXT("WldS1D5F2R"), {                        // anchor 38 m: slit dome and kite loft (right)
	{ S1D5("SlitDome"),           -17.0f,   0.0f,  1.19f,  0.0f, 1.f },
	{ S1D5("KiteLoft"),            16.0f,   0.0f, -1.12f,  0.0f, 1.f },
	{ S1D5("Telescope"),          -15.0f,  -9.2f,  1.05f,  0.0f, 1.f },
	{ S1D5("SkyBoard"),            19.0f,  -9.4f, -1.33f, -8.0f, 1.f },
	{ S1D5("LampCairn"),          -19.5f, -11.6f,  1.37f,  0.0f, 1.f },
	{ S1D5("WindBreak"),           12.5f, -11.6f, -0.88f,  0.0f, 1.f },
});
Set(TEXT("WldS1D5F2L"), {                        // anchor 38 m: slit dome and kite loft (left: the L twins)
	{ S1D5("SlitDomeL"),          -17.0f,   0.0f, -1.19f,  0.0f, 1.f },
	{ S1D5("KiteLoftL"),           16.0f,   0.0f,  1.12f,  0.0f, 1.f },
	{ S1D5("Telescope"),          -15.0f,  -9.2f, -1.05f,  0.0f, 1.f },
	{ S1D5("SkyBoard"),            19.0f,  -9.4f,  1.33f,  8.0f, 1.f },
	{ S1D5("LampCairn"),          -19.5f, -11.6f, -1.37f,  0.0f, 1.f },
	{ S1D5("WindBreak"),           12.5f, -11.6f,  0.88f,  0.0f, 1.f },
});
Set(TEXT("WldS1D5F3R"), {                        // anchor 38 m: instrument house and wind wall (right)
	{ S1D5("InstrumentHouse"),    -17.0f,   0.0f,  1.19f,  0.0f, 1.f },
	{ S1D5("WindWall"),            16.5f,   0.0f, -1.16f,  0.0f, 1.f },
	{ S1D5("Anemometer"),         -15.5f,  -9.4f,  1.09f,  0.0f, 1.f },
	{ S1D5("StreamerLine"),        15.5f,  -9.4f, -1.09f,  0.0f, 1.f },
});
Set(TEXT("WldS1D5F3L"), {                        // anchor 38 m: instrument house and wind wall (left: the L twins)
	{ S1D5("InstrumentHouseL"),   -17.0f,   0.0f, -1.19f,  0.0f, 1.f },
	{ S1D5("WindWallL"),           16.5f,   0.0f,  1.16f,  0.0f, 1.f },
	{ S1D5("AnemometerL"),        -15.5f,  -9.4f, -1.09f,  0.0f, 1.f },
	{ S1D5("StreamerLineL"),       15.5f,  -9.4f,  1.09f,  0.0f, 1.f },
});
Set(TEXT("WldS1D5F4R"), {                        // anchor 38 m: kite loft and refuge (right)
	{ S1D5("KiteLoft"),           -17.0f,   0.0f,  1.19f,  0.0f, 1.f },
	{ S1D5("Refuge"),              16.5f,   0.0f, -1.16f,  0.0f, 1.f },
	{ S1D5("SkyBoard"),           -18.0f,  -9.4f,  1.26f,  8.0f, 1.f },
	{ S1D5("Telescope"),           14.5f,  -9.2f, -1.02f,  0.0f, 1.f },
	{ S1D5("WindBreak"),          -12.5f, -11.6f,  0.88f,  0.0f, 1.f },
	{ S1D5("LampCairn"),           19.5f, -11.6f, -1.37f, 30.0f, 1.f },
});
Set(TEXT("WldS1D5F4L"), {                        // anchor 38 m: kite loft and refuge (left: the L twins)
	{ S1D5("KiteLoftL"),          -17.0f,   0.0f, -1.19f,  0.0f, 1.f },
	{ S1D5("RefugeL"),             16.5f,   0.0f,  1.16f,  0.0f, 1.f },
	{ S1D5("SkyBoard"),           -18.0f,  -9.4f, -1.26f, -8.0f, 1.f },
	{ S1D5("Telescope"),           14.5f,  -9.2f,  1.02f,  0.0f, 1.f },
	{ S1D5("WindBreak"),          -12.5f, -11.6f, -0.88f,  0.0f, 1.f },
	{ S1D5("LampCairn"),           19.5f, -11.6f,  1.37f, -30.0f, 1.f },
});
Set(TEXT("WldS1D5VergeAR"), {                    // anchor 28.6 m: a streamer line with a row of vanes before it, an anemometer mast (right)
	{ S1D5("StreamerLine"),       -15.5f,   0.0f,  1.09f,  0.0f, 1.f },
	{ S1D5("VaneRow"),            -15.5f,  -2.2f,  1.09f,  0.0f, 1.f },
	{ S1D5("Anemometer"),          16.0f,   0.0f, -1.12f,  0.0f, 1.f },
});
Set(TEXT("WldS1D5VergeAL"), {                    // anchor 28.6 m: a streamer line with a row of vanes before it, an anemometer mast (left: the L twins)
	{ S1D5("StreamerLineL"),      -15.5f,   0.0f, -1.09f,  0.0f, 1.f },
	{ S1D5("VaneRowL"),           -15.5f,  -2.2f, -1.09f,  0.0f, 1.f },
	{ S1D5("AnemometerL"),         16.0f,   0.0f,  1.12f,  0.0f, 1.f },
});
Set(TEXT("WldS1D5VergeBR"), {                    // anchor 28.6 m: the telescope and a lamp cairn, a wind-break and the sky board (right)
	{ S1D5("Telescope"),          -15.0f,   0.2f,  1.05f,  0.0f, 1.f },
	{ S1D5("LampCairn"),          -19.5f,  -2.2f,  1.37f, 15.0f, 1.f },
	{ S1D5("WindBreak"),           11.5f,  -2.2f, -0.81f,  0.0f, 1.f },
	{ S1D5("SkyBoard"),            18.0f,   0.0f, -1.26f, -8.0f, 1.f },
});
Set(TEXT("WldS1D5VergeBL"), {                    // anchor 28.6 m: the telescope and a lamp cairn, a wind-break and the sky board (left: the L twins)
	{ S1D5("Telescope"),          -15.0f,   0.2f, -1.05f,  0.0f, 1.f },
	{ S1D5("LampCairn"),          -19.5f,  -2.2f, -1.37f, -15.0f, 1.f },
	{ S1D5("WindBreak"),           11.5f,  -2.2f,  0.81f,  0.0f, 1.f },
	{ S1D5("SkyBoard"),            18.0f,   0.0f,  1.26f,  8.0f, 1.f },
});
Set(TEXT("WldS1D5Harp"), { { S1D5("WindHarp"), 0.f, 0.f, 0.f, 0.f, 1.f } });   // the hero: left only (an L piece)
Zone(TEXT("World s1d5, The white ridge"), 11.6f, 14.28f);
	Site(TEXT("WldS1D5RowAR"), 1, 55.0f, 8, 0, -1.f);     // right, s % 8 == 0: 7 slots (264 to 312)   // FIX ROUND: L6: its box was 2.0 m inside P3A_Hikers_A; slots 264 280 288 296 304 kept in the s1fix block
	Site(TEXT("WldS1D5RowBR"), 1, 55.0f, 8, 4, 1.f);     // right, s % 8 == 4: 7 slots (260 to 308)
	Site(TEXT("WldS1D5VergeAR"), 1, 28.6f, 8, 0, 1.f);   // right, s % 8 == 0: 7 slots (264 to 312)
	Site(TEXT("WldS1D5VergeBR"), 1, 28.6f, 16, 4, 1.f);   // right, s % 16 == 12: 3 slots (268 to 300)
	Site(TEXT("WldS1D5F1R"), 1, 38.0f, 16, 14, 1.f);       // right, s % 16 == 2: 4 slots (258 to 306)
	Site(TEXT("WldS1D5F2R"), 1, 38.0f, 16, 10, 1.f);       // right, s % 16 == 6: 4 slots (262 to 310)
	Site(TEXT("WldS1D5F3R"), 1, 38.0f, 16, 6, 1.f);       // right, s % 16 == 10: 4 slots (266 to 314)
	Site(TEXT("WldS1D5F4R"), 1, 38.0f, 16, 2, 1.f);       // right, s % 16 == 14: 3 slots (270 to 302)
	Site(TEXT("WldS1D5F1L"), -1, 38.0f, 32, 16, 1.f);       // left, s % 16 == 0: 3 slots (272 to 304)   // FIX ROUND: L6: its box was 3.9 m inside P3F_CableCam; L6: its box was 2.5 m inside CrewCrane_Snow; L6: its box was 2.3 m inside CrewCrane_Snow; now only slots 272 304
	Site(TEXT("WldS1D5F2L"), -1, 38.0f, 16, 12, 1.f);       // left, s % 16 == 4: 4 slots (260 to 308)
	Site(TEXT("WldS1D5F3L"), -1, 38.0f, 16, 8, 1.f);       // left, s % 16 == 8: 4 slots (264 to 312)
	Site(TEXT("WldS1D5F4L"), -1, 38.0f, 16, 4, -1.f);       // left, s % 16 == 12: 3 slots (268 to 300)   // FIX ROUND: L6: its box was 3.5 m inside P3F_CableCam; L6: its box was 2.6 m inside CrewCrane_Snow; L6: its box was 2.5 m inside CrewCrane_Snow; slots 268 284 kept in the s1fix block
	Site(TEXT("WldS1D5VergeAL"), -1, 28.6f, 8, 6, -1.f);   // left, s % 8 == 2: 8 slots (258 to 314)   // FIX ROUND: L6: its box was 4.2 m inside CrewApplauseA_Snow; L6: its box was 3.2 m inside P3F_SecondUnit_B; L6: its box was 2.3 m inside P3F_SecondUnit_A; L6: its box was 2.2 m inside P3F_PressPen; slots 266 274 282 290 298 314 kept in the s1fix block
	Site(TEXT("WldS1D5VergeBL"), -1, 28.6f, 8, 2, 1.f);   // left, s % 8 == 6: 7 slots (262 to 310)
Zone(TEXT("World s1d5, the Wind Harp"), 12.8f, 13.42f);
	Site(TEXT("WldS1D5Harp"), -1, 100.f, 12, 4, 1.f, 1.3f);            // left, 100 m: slots 284 and 296 (12.85 and 13.39 km)   // FIX ROUND: driver 8: the harp read as a faint comb; 1.3 times, thicker masts and deeper streamers in the mesh
#undef S1D5
// ---- END s1d5 ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People5 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1d6 (district 6 "The resort", the grand spa, 14.3 to 17.0 km, ft_world_ice_s1d6.py) ----
// 17 meshes /Game/World/IceMountain/SM_S1D6_*: 8 buildings (PalaceHotel, SpaHotel, TurretVilla, PumpRoom, BandstandHall, ColonnadeWalk,
// Funicular, Kurhaus), 8 props (BenchPair, LuggageCart, FlowerRail: LOW; Fountain, ChangingCabins, Kiosk, GlobeLamp, TowelLine: tall) and the
// hero (PalaceOfBaths). NOT COMPILED AND NOT SEEN IN THE GAME when written. The zone has slots 317 to 375; every rule is a Site on ONE even
// slot (every 1000, phase 1000 - slot: s % 4 == 0 or 2), one Set a slot and side (WldS1D6R<slot> / WldS1D6L<slot>), anchor 24 m, so a
// piece stands exactly where it was worked out. A part's "up" is -0.07 x its metres down the road (the hill falls 7 in 100).
// WHERE, and why (read from FTDressL5.inl, FTPeopleL5.inl, FTData.cpp's sections and the makers' logs for sizes; nothing measured in the game):
//   GAP LINES  where two slots meet (22.62 m along from a slot's middle; 20.5 or 24.74 m beside a street front, to stand 2 m off its end):
//           what is there already stands at a slot's middle or is scattered within 11.3 m of it, so this line is the freest ground.
//           On each line, a side: a LOW prop 24.8 m out (only on the three straights, 14.3 to 14.6, 15.44 to 15.73, 16.58 to 16.86 km:
//           beside a half pipe a low piece is behind its wall), a tall prop 30.6 m, a building 39.4 to 40.3 m (hotels and villas beside
//           the pipes, pavilions on the straights), and on some a big building in the far band: right 63.6 to 65 m (its front 56 m,
//           behind the service road), left 53.3 to 55 m (its front 46 m). Beside the banked bends (14.59 to 15.02, 15.73 to 16.16 km,
//           16.86 km on) only the far band is used, and on the left a colonnade or a pump room 43 to 45.2 m out (its front 40.1 m):
//           nothing of this block stands nearer than 40 m there.
//   CHECKED by a scratch tool (C:/ftwork/jobs/L5s1d6/mkblock.py), box against box on the same side: every piece against every Scatter and
//           Site piece of both tables that fires on slots 315 to 377 (a scattered piece taken at its whole slide), none meet. NOT in that
//           check: Far rules, the two strips laid up the slope from 86 m (SM_GroomerRun, SM_RackTrain), the film crew's pieces (their
//           sizes were not found: taken as 14 x 6 m), the race banners' masts (26 to 28 m: only the fountain was kept off them), and the
//           other ten Stage 1 builders' blocks.
//   HERO    THE PALACE OF BATHS once, left, 125 m out (its terrace 100 to 147 m), slot 354 moved 8 m down the road (16.02 km), between
//           the lift line of slot 353 and the lodge of slot 357, in front of the snowcats (150 m and more).
// Light: steady only (warm windows, lamp globes, lanterns), on tall pieces alone; the nearest lit piece is the globe lamp and the kiosk,
// 30.6 m out. The three LOW props are unlit and still. Nothing blinks. No figure, no animal, nothing that flies, no vehicle.
// Estimate (the brief's): 243 Site parts x 0.75 = 182.2, + the hero = about 183 placed (244 before the track's share; no loop, tube, corkscrew
//   or jump lies in this district, so the share left out may be smaller). 57 Sets, 57 rules.
//   Parts by mesh: GlobeLamp 26, LuggageCart 24, TurretVilla 21, SpaHotel 18, Funicular 17, BenchPair 16, FlowerRail 16, Kurhaus 15, ColonnadeWalk 15, Kiosk 14, ChangingCabins 14, PumpRoom 12, PalaceHotel 11, TowelLine 10, Fountain 8, BandstandHall 6, PalaceOfBaths 1.
//   Smallest x: TALL 30.6 m (Fountain, ChangingCabins, Kiosk, GlobeLamp, TowelLine; the fountain's edge 27.7 m); LOW 24.8 m.
#define S1D6(n) TEXT("/Game/World/IceMountain/SM_S1D6_" n)
#define S1D6_AT(set, s, side) Site(TEXT(set), side, 24.f, 1000, 1000 - (s), 1.f)
// lambdas of its own, so that the population function stays small for the compiler (as the pass 3 blocks do)
auto WldS1D6Sets = [&]()
{
	Set(TEXT("WldS1D6L318"), { { S1D6("FlowerRail"), 22.62f, 0.8f, 1.58f, 0.f, 1.f }, { S1D6("FlowerRail"), -19.87f, 0.8f, -1.39f, 0.f, 1.f }, { S1D6("LuggageCart"), -24.92f, 0.8f, -1.74f, 0.f, 1.f }, { S1D6("GlobeLamp"), 24.74f, 6.6f, 1.73f, 0.f, 1.f }, { S1D6("TowelLine"), -24.74f, 6.6f, -1.73f, 0.f, 1.f }, { S1D6("ColonnadeWalk"), 24.74f, 16.3f, 1.73f, 0.f, 1.f }, { S1D6("Funicular"), 24.74f, 29.3f, 1.73f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R318"), { { S1D6("BenchPair"), -24.97f, 0.8f, 1.75f, 0.f, 1.f }, { S1D6("LuggageCart"), -20.12f, 0.8f, 1.41f, 0.f, 1.f }, { S1D6("BenchPair"), 22.62f, 0.8f, -1.58f, 0.f, 1.f }, { S1D6("Fountain"), -20.5f, 6.6f, 1.44f, 0.f, 1.f }, { S1D6("Kiosk"), 20.5f, 6.6f, -1.44f, 0.f, 1.f }, { S1D6("BandstandHall"), 20.5f, 15.4f, -1.44f, 0.f, 1.f }, { S1D6("PumpRoom"), -20.5f, 16.3f, 1.44f, 0.f, 1.f }, { S1D6("PalaceHotel"), -20.5f, 40.6f, 1.44f, 0.f, 1.f }, { S1D6("Kurhaus"), 20.5f, 41.0f, -1.44f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L320"), { { S1D6("BenchPair"), 24.97f, 0.8f, 1.75f, 0.f, 1.f }, { S1D6("LuggageCart"), 20.12f, 0.8f, 1.41f, 0.f, 1.f }, { S1D6("BenchPair"), -22.62f, 0.8f, -1.58f, 0.f, 1.f }, { S1D6("GlobeLamp"), 24.74f, 6.6f, 1.73f, 0.f, 1.f }, { S1D6("ChangingCabins"), -24.74f, 6.6f, -1.73f, 0.f, 1.f }, { S1D6("PumpRoom"), -24.74f, 16.3f, -1.73f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R320"), { { S1D6("LuggageCart"), -22.62f, 0.8f, 1.58f, 0.f, 1.f }, { S1D6("FlowerRail"), 22.62f, 0.8f, -1.58f, 0.f, 1.f }, { S1D6("ChangingCabins"), -20.5f, 6.6f, 1.44f, 0.f, 1.f }, { S1D6("Fountain"), 20.5f, 6.6f, -1.44f, 0.f, 1.f }, { S1D6("TurretVilla"), -20.5f, 16.3f, 1.44f, 0.f, 1.f }, { S1D6("SpaHotel"), 20.5f, 16.3f, -1.44f, 0.f, 1.f }, { S1D6("Funicular"), -20.5f, 39.6f, 1.44f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L322"), { { S1D6("LuggageCart"), 22.62f, 0.8f, 1.58f, 0.f, 1.f }, { S1D6("FlowerRail"), -22.62f, 0.8f, -1.58f, 0.f, 1.f }, { S1D6("Fountain"), 24.74f, 6.6f, 1.73f, 0.f, 1.f }, { S1D6("Kiosk"), -24.74f, 6.6f, -1.73f, 0.f, 1.f }, { S1D6("BandstandHall"), 24.74f, 15.4f, 1.73f, 0.f, 1.f }, { S1D6("SpaHotel"), -24.74f, 16.3f, -1.73f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R322"), { { S1D6("FlowerRail"), -25.37f, 0.8f, 1.78f, 0.f, 1.f }, { S1D6("LuggageCart"), -20.32f, 0.8f, 1.42f, 0.f, 1.f }, { S1D6("BenchPair"), 20.27f, 0.8f, -1.42f, 0.f, 1.f }, { S1D6("LuggageCart"), 25.12f, 0.8f, -1.76f, 0.f, 1.f }, { S1D6("GlobeLamp"), -20.5f, 6.6f, 1.44f, 0.f, 1.f }, { S1D6("GlobeLamp"), 22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("ColonnadeWalk"), -20.5f, 16.3f, 1.44f, 0.f, 1.f }, { S1D6("TurretVilla"), 22.62f, 16.3f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L324"), { { S1D6("PumpRoom"), 22.62f, 21.2f, 1.58f, 0.f, 1.f }, { S1D6("PumpRoom"), -22.62f, 21.2f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R324"), { { S1D6("Funicular"), 22.62f, 39.6f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L326"), { { S1D6("ColonnadeWalk"), 22.62f, 19.0f, 1.58f, 0.f, 1.f }, { S1D6("ColonnadeWalk"), -22.62f, 19.0f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R326"), { { S1D6("PalaceHotel"), 22.62f, 40.6f, -1.58f, 0.f, 1.f }, { S1D6("Kurhaus"), -22.62f, 41.0f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L328"), { { S1D6("ColonnadeWalk"), 22.62f, 19.0f, 1.58f, 0.f, 1.f }, { S1D6("PumpRoom"), -22.62f, 21.2f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R328"), { { S1D6("Funicular"), 22.62f, 39.6f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R330"), { { S1D6("PalaceHotel"), 22.62f, 40.6f, -1.58f, 0.f, 1.f }, { S1D6("Kurhaus"), -22.62f, 41.0f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L332"), { { S1D6("Kiosk"), -22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("SpaHotel"), -22.62f, 16.3f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R332"), { { S1D6("GlobeLamp"), 22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), 22.62f, 16.3f, -1.58f, 0.f, 1.f }, { S1D6("Funicular"), -22.62f, 39.6f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L334"), { { S1D6("ChangingCabins"), 22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("GlobeLamp"), -22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), 22.62f, 16.3f, 1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), -22.62f, 16.3f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R334"), { { S1D6("GlobeLamp"), -22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("TowelLine"), 22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), -22.62f, 16.3f, 1.58f, 0.f, 1.f }, { S1D6("SpaHotel"), 22.62f, 16.3f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L336"), { { S1D6("Kurhaus"), 22.62f, 31.0f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R336"), { { S1D6("Kiosk"), -22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("GlobeLamp"), 22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), -22.62f, 16.3f, 1.58f, 0.f, 1.f }, { S1D6("PalaceHotel"), 22.62f, 40.6f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L338"), { { S1D6("ChangingCabins"), 22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("TowelLine"), -22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("SpaHotel"), 22.62f, 16.3f, 1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), -22.62f, 16.3f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R338"), { { S1D6("Funicular"), -22.62f, 39.6f, 1.58f, 0.f, 1.f }, { S1D6("Kurhaus"), 22.62f, 41.0f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L340"), { { S1D6("Kiosk"), 22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("ChangingCabins"), -22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("SpaHotel"), 22.62f, 16.3f, 1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), -22.62f, 16.3f, -1.58f, 0.f, 1.f }, { S1D6("Funicular"), -22.62f, 29.3f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R340"), { { S1D6("GlobeLamp"), -22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("GlobeLamp"), 22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), -22.62f, 16.3f, 1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), 22.62f, 16.3f, -1.58f, 0.f, 1.f }, { S1D6("PalaceHotel"), 22.62f, 40.6f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L342"), { { S1D6("FlowerRail"), 25.37f, 0.8f, 1.78f, 0.f, 1.f }, { S1D6("LuggageCart"), 20.32f, 0.8f, 1.42f, 0.f, 1.f }, { S1D6("BenchPair"), -20.27f, 0.8f, -1.42f, 0.f, 1.f }, { S1D6("LuggageCart"), -25.12f, 0.8f, -1.76f, 0.f, 1.f }, { S1D6("ChangingCabins"), 24.74f, 6.6f, 1.73f, 0.f, 1.f }, { S1D6("Fountain"), -24.74f, 6.6f, -1.73f, 0.f, 1.f }, { S1D6("ColonnadeWalk"), 24.74f, 16.3f, 1.73f, 0.f, 1.f }, { S1D6("TurretVilla"), -24.74f, 16.3f, -1.73f, 0.f, 1.f }, { S1D6("Kurhaus"), 24.74f, 31.0f, 1.73f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R342"), { { S1D6("BenchPair"), -22.62f, 0.8f, 1.58f, 0.f, 1.f }, { S1D6("LuggageCart"), 22.62f, 0.8f, -1.58f, 0.f, 1.f }, { S1D6("TowelLine"), -22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("GlobeLamp"), 20.5f, 6.6f, -1.44f, 0.f, 1.f }, { S1D6("BandstandHall"), 20.5f, 15.4f, -1.44f, 0.f, 1.f }, { S1D6("PumpRoom"), -22.62f, 16.3f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L344"), { { S1D6("BenchPair"), 22.62f, 0.8f, 1.58f, 0.f, 1.f }, { S1D6("LuggageCart"), -22.62f, 0.8f, -1.58f, 0.f, 1.f }, { S1D6("Kiosk"), 24.74f, 6.6f, 1.73f, 0.f, 1.f }, { S1D6("GlobeLamp"), -24.74f, 6.6f, -1.73f, 0.f, 1.f }, { S1D6("BandstandHall"), -24.74f, 15.4f, -1.73f, 0.f, 1.f }, { S1D6("PumpRoom"), 24.74f, 16.3f, 1.73f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R344"), { { S1D6("FlowerRail"), -22.62f, 0.8f, 1.58f, 0.f, 1.f }, { S1D6("FlowerRail"), 19.87f, 0.8f, -1.39f, 0.f, 1.f }, { S1D6("LuggageCart"), 24.92f, 0.8f, -1.74f, 0.f, 1.f }, { S1D6("ChangingCabins"), -20.5f, 6.6f, 1.44f, 0.f, 1.f }, { S1D6("Fountain"), 20.5f, 6.6f, -1.44f, 0.f, 1.f }, { S1D6("SpaHotel"), -20.5f, 16.3f, 1.44f, 0.f, 1.f }, { S1D6("ColonnadeWalk"), 20.5f, 16.3f, -1.44f, 0.f, 1.f }, { S1D6("PalaceHotel"), -20.5f, 40.6f, 1.44f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L346"), { { S1D6("FlowerRail"), 22.62f, 0.8f, 1.58f, 0.f, 1.f }, { S1D6("FlowerRail"), -19.87f, 0.8f, -1.39f, 0.f, 1.f }, { S1D6("LuggageCart"), -24.92f, 0.8f, -1.74f, 0.f, 1.f }, { S1D6("TowelLine"), 24.74f, 6.6f, 1.73f, 0.f, 1.f }, { S1D6("GlobeLamp"), -24.74f, 6.6f, -1.73f, 0.f, 1.f }, { S1D6("SpaHotel"), 24.74f, 16.3f, 1.73f, 0.f, 1.f }, { S1D6("SpaHotel"), -24.74f, 16.3f, -1.73f, 0.f, 1.f }, { S1D6("Funicular"), 24.74f, 29.3f, 1.73f, 0.f, 1.f }, { S1D6("Funicular"), -24.74f, 29.3f, -1.73f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R346"), { { S1D6("BenchPair"), -24.97f, 0.8f, 1.75f, 0.f, 1.f }, { S1D6("LuggageCart"), -20.12f, 0.8f, 1.41f, 0.f, 1.f }, { S1D6("BenchPair"), 22.62f, 0.8f, -1.58f, 0.f, 1.f }, { S1D6("Kiosk"), -20.5f, 6.6f, 1.44f, 0.f, 1.f }, { S1D6("ChangingCabins"), 20.5f, 6.6f, -1.44f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L348"), { { S1D6("BenchPair"), 24.97f, 0.8f, 1.75f, 0.f, 1.f }, { S1D6("LuggageCart"), 20.12f, 0.8f, 1.41f, 0.f, 1.f }, { S1D6("ChangingCabins"), 22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("ColonnadeWalk"), 22.62f, 16.3f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R348"), { { S1D6("LuggageCart"), -22.62f, 0.8f, 1.58f, 0.f, 1.f }, { S1D6("Fountain"), -20.5f, 6.6f, 1.44f, 0.f, 1.f }, { S1D6("PumpRoom"), -20.5f, 16.3f, 1.44f, 0.f, 1.f }, { S1D6("Funicular"), -20.5f, 39.6f, 1.44f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L350"), { { S1D6("ColonnadeWalk"), 22.62f, 19.0f, 1.58f, 0.f, 1.f }, { S1D6("ColonnadeWalk"), -22.62f, 19.0f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R350"), { { S1D6("Funicular"), 22.62f, 39.6f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L352"), { { S1D6("ColonnadeWalk"), 22.62f, 19.0f, 1.58f, 0.f, 1.f }, { S1D6("PumpRoom"), -22.62f, 21.2f, -1.58f, 0.f, 1.f }, { S1D6("PalaceHotel"), 22.62f, 31.0f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R352"), { { S1D6("Funicular"), 22.62f, 39.6f, -1.58f, 0.f, 1.f }, { S1D6("Kurhaus"), -22.62f, 41.0f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L354"), { { S1D6("ColonnadeWalk"), 22.62f, 19.0f, 1.58f, 0.f, 1.f }, { S1D6("PumpRoom"), -22.62f, 21.2f, -1.58f, 0.f, 1.f }, { S1D6("Funicular"), 22.62f, 29.3f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R354"), { { S1D6("Funicular"), 22.62f, 39.6f, -1.58f, 0.f, 1.f }, { S1D6("Kurhaus"), -22.62f, 41.0f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L356"), { { S1D6("ColonnadeWalk"), 22.62f, 19.0f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R356"), { { S1D6("Funicular"), 22.62f, 39.6f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L358"), { { S1D6("GlobeLamp"), 22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("GlobeLamp"), -22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), 22.62f, 16.3f, 1.58f, 0.f, 1.f }, { S1D6("SpaHotel"), -22.62f, 16.3f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R358"), { { S1D6("TowelLine"), -22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("Kiosk"), 22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("SpaHotel"), -22.62f, 16.3f, 1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), 22.62f, 16.3f, -1.58f, 0.f, 1.f }, { S1D6("PalaceHotel"), 22.62f, 40.6f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L360"), { { S1D6("TowelLine"), 22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("Kiosk"), -22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("SpaHotel"), 22.62f, 16.3f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R360"), { { S1D6("ChangingCabins"), -22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("GlobeLamp"), 22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), -22.62f, 16.3f, 1.58f, 0.f, 1.f }, { S1D6("SpaHotel"), 22.62f, 16.3f, -1.58f, 0.f, 1.f }, { S1D6("Funicular"), -22.62f, 39.6f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L362"), { { S1D6("ChangingCabins"), 22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("GlobeLamp"), -22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), 22.62f, 16.3f, 1.58f, 0.f, 1.f }, { S1D6("SpaHotel"), -22.62f, 16.3f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R362"), { { S1D6("GlobeLamp"), -22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("GlobeLamp"), 22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), -22.62f, 16.3f, 1.58f, 0.f, 1.f }, { S1D6("Kurhaus"), -22.62f, 41.0f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L364"), { { S1D6("GlobeLamp"), 22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("TowelLine"), -22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), 22.62f, 16.3f, 1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), -22.62f, 16.3f, -1.58f, 0.f, 1.f }, { S1D6("PalaceHotel"), 22.62f, 31.0f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R364"), { { S1D6("Kiosk"), -22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("ChangingCabins"), 22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("Kurhaus"), 22.62f, 41.0f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L366"), { { S1D6("BenchPair"), -22.62f, 0.8f, -1.58f, 0.f, 1.f }, { S1D6("Kiosk"), 22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("GlobeLamp"), -22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), 22.62f, 16.3f, 1.58f, 0.f, 1.f }, { S1D6("TurretVilla"), -22.62f, 16.3f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R366"), { { S1D6("FlowerRail"), 22.62f, 0.8f, -1.58f, 0.f, 1.f }, { S1D6("GlobeLamp"), -22.62f, 6.6f, 1.58f, 0.f, 1.f }, { S1D6("Kiosk"), 20.5f, 6.6f, -1.44f, 0.f, 1.f }, { S1D6("BandstandHall"), 20.5f, 15.4f, -1.44f, 0.f, 1.f }, { S1D6("SpaHotel"), -22.62f, 16.3f, 1.58f, 0.f, 1.f }, { S1D6("PalaceHotel"), 20.5f, 40.6f, -1.44f, 0.f, 1.f }, { S1D6("Kurhaus"), -22.62f, 41.0f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L368"), { { S1D6("LuggageCart"), 22.62f, 0.8f, 1.58f, 0.f, 1.f }, { S1D6("FlowerRail"), -22.62f, 0.8f, -1.58f, 0.f, 1.f }, { S1D6("GlobeLamp"), 24.74f, 6.6f, 1.73f, 0.f, 1.f }, { S1D6("ChangingCabins"), -24.74f, 6.6f, -1.73f, 0.f, 1.f }, { S1D6("PumpRoom"), 24.74f, 16.3f, 1.73f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R368"), { { S1D6("FlowerRail"), -25.37f, 0.8f, 1.78f, 0.f, 1.f }, { S1D6("LuggageCart"), -20.32f, 0.8f, 1.42f, 0.f, 1.f }, { S1D6("BenchPair"), 20.27f, 0.8f, -1.42f, 0.f, 1.f }, { S1D6("LuggageCart"), 25.12f, 0.8f, -1.76f, 0.f, 1.f }, { S1D6("GlobeLamp"), -20.5f, 6.6f, 1.44f, 0.f, 1.f }, { S1D6("TowelLine"), 20.5f, 6.6f, -1.44f, 0.f, 1.f }, { S1D6("SpaHotel"), -20.5f, 16.3f, 1.44f, 0.f, 1.f }, { S1D6("ColonnadeWalk"), 20.5f, 16.3f, -1.44f, 0.f, 1.f }, { S1D6("Kurhaus"), -20.5f, 41.0f, 1.44f, 0.f, 1.f }, { S1D6("Kurhaus"), 20.5f, 41.0f, -1.44f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L370"), { { S1D6("FlowerRail"), 25.37f, 0.8f, 1.78f, 0.f, 1.f }, { S1D6("LuggageCart"), 20.32f, 0.8f, 1.42f, 0.f, 1.f }, { S1D6("BenchPair"), -20.27f, 0.8f, -1.42f, 0.f, 1.f }, { S1D6("LuggageCart"), -25.12f, 0.8f, -1.76f, 0.f, 1.f }, { S1D6("Fountain"), 24.74f, 6.6f, 1.73f, 0.f, 1.f }, { S1D6("Kiosk"), -24.74f, 6.6f, -1.73f, 0.f, 1.f }, { S1D6("SpaHotel"), -24.74f, 16.3f, -1.73f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R370"), { { S1D6("BenchPair"), -22.62f, 0.8f, 1.58f, 0.f, 1.f }, { S1D6("LuggageCart"), 22.62f, 0.8f, -1.58f, 0.f, 1.f }, { S1D6("GlobeLamp"), -20.5f, 6.6f, 1.44f, 0.f, 1.f }, { S1D6("ChangingCabins"), 20.5f, 6.6f, -1.44f, 0.f, 1.f }, { S1D6("BandstandHall"), -20.5f, 15.4f, 1.44f, 0.f, 1.f }, { S1D6("SpaHotel"), 20.5f, 16.3f, -1.44f, 0.f, 1.f } });
	Set(TEXT("WldS1D6L372"), { { S1D6("BenchPair"), 22.62f, 0.8f, 1.58f, 0.f, 1.f }, { S1D6("LuggageCart"), -22.62f, 0.8f, -1.58f, 0.f, 1.f }, { S1D6("GlobeLamp"), 24.74f, 6.6f, 1.73f, 0.f, 1.f }, { S1D6("TowelLine"), -24.74f, 6.6f, -1.73f, 0.f, 1.f }, { S1D6("ColonnadeWalk"), 24.74f, 16.3f, 1.73f, 0.f, 1.f }, { S1D6("Kurhaus"), 24.74f, 31.0f, 1.73f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R372"), { { S1D6("FlowerRail"), -22.62f, 0.8f, 1.58f, 0.f, 1.f }, { S1D6("FlowerRail"), 19.87f, 0.8f, -1.39f, 0.f, 1.f }, { S1D6("LuggageCart"), 24.92f, 0.8f, -1.74f, 0.f, 1.f }, { S1D6("Fountain"), -20.5f, 6.6f, 1.44f, 0.f, 1.f }, { S1D6("Kiosk"), 22.62f, 6.6f, -1.58f, 0.f, 1.f }, { S1D6("PumpRoom"), -20.5f, 16.3f, 1.44f, 0.f, 1.f }, { S1D6("PalaceHotel"), -20.5f, 40.6f, 1.44f, 0.f, 1.f }, { S1D6("Kurhaus"), 22.62f, 41.0f, -1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6R374"), { { S1D6("Funicular"), -22.62f, 39.6f, 1.58f, 0.f, 1.f } });
	Set(TEXT("WldS1D6Baths"), { { S1D6("PalaceOfBaths"), -8.0f, 0.f, -0.56f, 0.f, 1.f } });
};
WldS1D6Sets();
auto WldS1D6Rules = [&]()
{
Zone(TEXT("World s1d6, the grand spa"), 14.3f, 17.f);
	S1D6_AT("WldS1D6L318", 318, -1); S1D6_AT("WldS1D6R318", 318, 1); S1D6_AT("WldS1D6L320", 320, -1); S1D6_AT("WldS1D6R320Fx4", 320, 1);   // FIX ROUND: 6R320: L6: its box was 3.5 m inside ChaletRow
	S1D6_AT("WldS1D6L322", 322, -1); S1D6_AT("WldS1D6R322", 322, 1); S1D6_AT("WldS1D6L324", 324, -1); S1D6_AT("WldS1D6R324", 324, 1);
	S1D6_AT("WldS1D6L326", 326, -1); S1D6_AT("WldS1D6R326", 326, 1); S1D6_AT("WldS1D6L328", 328, -1); S1D6_AT("WldS1D6R328", 328, 1);
	S1D6_AT("WldS1D6R330", 330, 1); S1D6_AT("WldS1D6L332", 332, -1); S1D6_AT("WldS1D6R332Fx1", 332, 1); S1D6_AT("WldS1D6L334", 334, -1);   // FIX ROUND: 6R332: L6: its box was 2.4 m inside P3F_PressPen
	S1D6_AT("WldS1D6R334", 334, 1); S1D6_AT("WldS1D6L336", 336, -1); S1D6_AT("WldS1D6R336", 336, 1); S1D6_AT("WldS1D6L338", 338, -1);
	S1D6_AT("WldS1D6R338", 338, 1); S1D6_AT("WldS1D6L340", 340, -1); S1D6_AT("WldS1D6R340", 340, 1); S1D6_AT("WldS1D6L342", 342, -1);
	S1D6_AT("WldS1D6R342", 342, 1); S1D6_AT("WldS1D6L344", 344, -1); S1D6_AT("WldS1D6R344", 344, 1); S1D6_AT("WldS1D6L346", 346, -1);
	S1D6_AT("WldS1D6R346", 346, 1); S1D6_AT("WldS1D6L348", 348, -1); S1D6_AT("WldS1D6R348", 348, 1); S1D6_AT("WldS1D6L350", 350, -1);
	S1D6_AT("WldS1D6R350", 350, 1); S1D6_AT("WldS1D6L352", 352, -1); S1D6_AT("WldS1D6R352", 352, 1); S1D6_AT("WldS1D6L354", 354, -1);
	S1D6_AT("WldS1D6R354", 354, 1); S1D6_AT("WldS1D6L356", 356, -1); S1D6_AT("WldS1D6R356", 356, 1); S1D6_AT("WldS1D6L358", 358, -1);
	S1D6_AT("WldS1D6R358", 358, 1); S1D6_AT("WldS1D6L360", 360, -1); S1D6_AT("WldS1D6R360", 360, 1); S1D6_AT("WldS1D6L362", 362, -1);
	S1D6_AT("WldS1D6R362Fx2", 362, 1); S1D6_AT("WldS1D6L364", 364, -1); S1D6_AT("WldS1D6R364", 364, 1); S1D6_AT("WldS1D6L366", 366, -1);   // FIX ROUND: 6R362: L6: its box was 2.8 m inside P3C_SkiTrail_A
	S1D6_AT("WldS1D6R366", 366, 1); S1D6_AT("WldS1D6L368", 368, -1); S1D6_AT("WldS1D6R368", 368, 1); S1D6_AT("WldS1D6L370", 370, -1);
	S1D6_AT("WldS1D6R370", 370, 1); S1D6_AT("WldS1D6L372", 372, -1); S1D6_AT("WldS1D6R372", 372, 1); S1D6_AT("WldS1D6R374", 374, 1);
// THE PALACE OF BATHS, left, 125 m
	Site(TEXT("WldS1D6Baths"), -1, 125.0f, 1000, 1000 - 354, 1.f);
};
WldS1D6Rules();
#undef S1D6_AT
#undef S1D6
// ---- END s1d6 ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People5 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1d7 (district 7 "The ice again": the crystal mine, ft_world_ice_s1d7.py) ----
// 18 meshes: 8 buildings, 9 props (5 low, 4 tall), the hero. Every rule is a Site (a Set at the slot's middle), on even slots only.
// Estimate: slots x parts of every rule added up = 256, x 0.75 (the track's share left out) = about 192 placed.
//   buildings 85 (left 54.5 m on all 30 even slots, right 67 m on 16), low props 93 (24.9 m), tall props 76 (27.6 m), the hero 2 (right, 160 m).
//   Most of one mesh: OreCart, 31 slots (about 23 placed). Smallest x of a TALL mesh: 27.6 m (the four tall props).
// Bands and slots used: s % 4 == 0 or 2 only (every 4, 12 or 60; phases even).
//   17 to 19.7 km: buildings left at 54.5 m (the footbridges and decks of pass 3 stand at 54 to 56 m on the RIGHT on these
//   slots, the left is free there), right at 67 m behind them; the two tall buildings on the slots where an ice wall stands
//   at 33.5 m (left s % 3 == 2, right s % 3 == 1) so that they show over it. Right, not on 390, 396, 420, 426 (pass 3c at 64 m).
//   The verge zones only (17.24 to 18.03, 18.38 to 19.17, 19.52 to 19.7: where the level's own verge is written, the
//   banked bends left out): low props at 24.9 m, three to a slot (12 m apart along), tall props at 27.6 m, 17 and 16 m
//   either side of the slot's middle. Right, the low sets leave s % 12 == 2 and 8 to pass 3a (23 m).
//   The hero: right, 160 m, slots 402 and 414 (18.19 and 18.73 km).
// A set's part along the road takes the anchor's height: up = -0.07 x along on the right, +0.07 x along on the left
// (the rule of FTDressL5.inl's head).
#define S1D7(n) TEXT("/Game/World/IceMountain/SM_S1D7_" n)
Set(TEXT("WldS1D7CampAL"), { { S1D7("Bunkhouse"), -9.f, 0.f, -0.63f, 0.f, 1.f }, { S1D7("AssayOffice"), 9.f, 0.f, 0.63f, 0.f, 1.f } });     // the bunkhouse and the assay office, left
Set(TEXT("WldS1D7CampBL"), { { S1D7("WindingHouse"), -8.f, 0.f, -0.56f, 0.f, 1.f }, { S1D7("ToolStore"), 9.f, 0.f, 0.63f, 0.f, 1.f } });     // the winding house and the tool store, left
Set(TEXT("WldS1D7CampCL"), { { S1D7("Breaker"), -6.f, 0.f, -0.42f, 0.f, 1.f }, { S1D7("ToolStore"), 13.f, 0.f, 0.91f, 0.f, 1.f } });     // the breaker and the tool store, left
Set(TEXT("WldS1D7CampDL"), { { S1D7("Trestle"), 0.f, 0.f, 0.f, 0.f, 1.f } });     // the trestle, left
Set(TEXT("WldS1D7CampEL"), { { S1D7("ChuteHouse"), -8.f, 0.f, -0.56f, 0.f, 1.f }, { S1D7("TankTower"), 10.f, 0.f, 0.7f, 0.f, 1.f } });     // the two tall ones (they show over the ice walls): the ore chute house and the tank tower, left
Set(TEXT("WldS1D7CampER"), { { S1D7("ChuteHouse"), -8.f, 0.f, 0.56f, 0.f, 1.f }, { S1D7("TankTower"), 10.f, 0.f, -0.7f, 0.f, 1.f } });     // the two tall ones (they show over the ice walls): the ore chute house and the tank tower, right
Set(TEXT("WldS1D7CampAR"), { { S1D7("Bunkhouse"), -9.f, 1.f, 0.63f, 0.f, 1.f }, { S1D7("AssayOffice"), 9.f, 0.f, -0.63f, 0.f, 1.f } });     // the bunkhouse and the assay office, right
Set(TEXT("WldS1D7CampCR"), { { S1D7("Breaker"), -6.f, 0.f, 0.42f, 0.f, 1.f }, { S1D7("ToolStore"), 13.f, 0.f, -0.91f, 0.f, 1.f } });     // the breaker and the tool store, right
Set(TEXT("WldS1D7CampDR"), { { S1D7("Trestle"), 0.f, 0.f, 0.f, 0.f, 1.f } });     // the trestle, right
Set(TEXT("WldS1D7CampBR"), { { S1D7("WindingHouse"), -8.f, 0.f, 0.56f, 0.f, 1.f }, { S1D7("ToolStore"), 9.f, 0.f, -0.63f, 0.f, 1.f } });     // the winding house and the tool store, right
Set(TEXT("WldS1D7HeroR"), { { S1D7("Headframe"), 0.f, 0.f, 0.f, 0.f, 1.f } });     // THE CRYSTAL HEADFRAME, right
Set(TEXT("WldS1D7LowAL"), { { S1D7("OreCart"), -12.f, 0.f, -0.84f, 0.f, 1.f }, { S1D7("PitProps"), 0.f, 0.f, 0.f, 0.f, 1.f }, { S1D7("Winch"), 12.f, 0.f, 0.84f, 0.f, 1.f } });     // low, by the tape: an ore cart, pit props, the hand winch, left
Set(TEXT("WldS1D7LowBL"), { { S1D7("SortTable"), -12.f, 0.f, -0.84f, 0.f, 1.f }, { S1D7("BufferStop"), 0.f, 0.f, 0.f, 0.f, 1.f }, { S1D7("OreCart"), 12.f, 0.f, 0.84f, 0.f, 1.f } });     // low: the sorting table, the buffer stop, an ore cart, left
Set(TEXT("WldS1D7LowAR"), { { S1D7("OreCart"), -12.f, 0.f, 0.84f, 0.f, 1.f }, { S1D7("PitProps"), 0.f, 0.f, 0.f, 0.f, 1.f }, { S1D7("Winch"), 12.f, 0.f, -0.84f, 0.f, 1.f } });     // low, by the tape: an ore cart, pit props, the hand winch, right
Set(TEXT("WldS1D7LowBR"), { { S1D7("SortTable"), -12.f, 0.f, 0.84f, 0.f, 1.f }, { S1D7("BufferStop"), 0.f, 0.f, 0.f, 0.f, 1.f }, { S1D7("OreCart"), 12.f, 0.f, -0.84f, 0.f, 1.f } });     // low: the sorting table, the buffer stop, an ore cart, right
Set(TEXT("WldS1D7TallAL"), { { S1D7("LampPost"), -17.f, 0.f, -1.19f, 0.f, 1.f }, { S1D7("CrystalRock"), 16.f, 0.f, 1.12f, 0.f, 1.f } });     // a lamp post and a crystal rock, left
Set(TEXT("WldS1D7TallBL"), { { S1D7("BellFrame"), -17.f, 0.f, -1.19f, 0.f, 1.f }, { S1D7("Sluice"), 16.f, 0.f, 1.12f, 0.f, 1.f } });     // the shift bell and a sluice, left
Set(TEXT("WldS1D7TallCR"), { { S1D7("LampPost"), -17.f, 0.f, 1.19f, 0.f, 1.f }, { S1D7("Sluice"), 16.f, 0.f, -1.12f, 0.f, 1.f } });     // a lamp post and a sluice, right
Set(TEXT("WldS1D7TallDR"), { { S1D7("BellFrame"), -17.f, 0.f, 1.19f, 0.f, 1.f }, { S1D7("CrystalRock"), 16.f, 0.f, -1.12f, 0.f, 1.f } });     // the shift bell and a crystal rock, right
Zone(TEXT("World s1d7, the crystal mine: the camp"), 17.0f, 19.7f);
	Site(TEXT("WldS1D7CampAL"), -1, 54.5f, 12, 0, 1.f);     // left slots 384 396 408 420 432
	Site(TEXT("WldS1D7CampBL"), -1, 54.5f, 12, 6, 1.f);     // left slots 378 390 402 414 426
	Site(TEXT("WldS1D7CampCL"), -1, 54.5f, 12, 8, -1.f);     // left slots 376 388 400 412 424   // FIX ROUND: R1: the ToolStore of slot 376 stood at 16.998 km, before the district begins; slots 388 400 412 424 kept in the s1fix block
	Site(TEXT("WldS1D7CampDL"), -1, 54.5f, 12, 2, 1.f);     // left slots 382 394 406 418 430
	Site(TEXT("WldS1D7CampEL"), -1, 54.5f, 12, 10, 1.f);     // left slots 386 398 410 422 434
	Site(TEXT("WldS1D7CampEL"), -1, 54.5f, 12, 4, 1.f);     // left slots 380 392 404 416 428
	Site(TEXT("WldS1D7CampER"), 1, 67.f, 12, 8, 1.f);     // right slots 376 388 400 412 424
	Site(TEXT("WldS1D7CampER"), 1, 67.f, 12, 2, 1.f);     // right slots 382 394 406 418 430
	Site(TEXT("WldS1D7CampAR"), 1, 67.f, 60, 42, 1.f);     // right slots 378
	Site(TEXT("WldS1D7CampCR"), 1, 67.f, 60, 36, 1.f);     // right slots 384
	Site(TEXT("WldS1D7CampDR"), 1, 67.f, 60, 18, 1.f);     // right slots 402
	Site(TEXT("WldS1D7CampBR"), 1, 67.f, 60, 12, 1.f);     // right slots 408
	Site(TEXT("WldS1D7CampAR"), 1, 67.f, 60, 6, 1.f);     // right slots 414
	Site(TEXT("WldS1D7CampDR"), 1, 67.f, 60, 48, 1.f);     // right slots 432
Zone(TEXT("World s1d7, the crystal mine: the headframe"), 18.0f, 19.0f);
	Site(TEXT("WldS1D7HeroR"), 1, 160.f, 12, 6, 1.f);     // right slots 402 414
Zone(TEXT("World s1d7, the crystal mine: the verge (1)"), 17.24f, 18.03f);
	Site(TEXT("WldS1D7LowAL"), -1, 24.9f, 4, 0, 1.f);     // left slots 384 388 392 396
	Site(TEXT("WldS1D7LowBL"), -1, 24.9f, 4, 2, 1.f);     // left slots 382 386 390 394 398
	Site(TEXT("WldS1D7LowAR"), 1, 24.9f, 12, 0, 1.f);     // right slots 384 396
	Site(TEXT("WldS1D7LowBR"), 1, 24.9f, 12, 8, 1.f);     // right slots 388
	Site(TEXT("WldS1D7LowBR"), 1, 24.9f, 12, 6, 1.f);     // right slots 390
	Site(TEXT("WldS1D7LowBR"), 1, 24.9f, 12, 2, 1.f);     // right slots 382 394
	Site(TEXT("WldS1D7TallAL"), -1, 27.6f, 4, 0, -1.f);     // left slots 384 388 392 396   // FIX ROUND: L6: its box was 2.4 m inside PoleFeather; slots 388 392 396 kept in the s1fix block
	Site(TEXT("WldS1D7TallBL"), -1, 27.6f, 4, 2, -1.f);     // left slots 382 386 390 394 398   // FIX ROUND: L6: its box was 2.1 m inside PoleFeather; slots 382 390 394 398 kept in the s1fix block
	Site(TEXT("WldS1D7TallCR"), 1, 27.6f, 4, 0, 1.f);     // right slots 384 388 392 396
	Site(TEXT("WldS1D7TallDR"), 1, 27.6f, 4, 2, -1.f);     // right slots 382 386 390 394 398   // FIX ROUND: L6: its box was 2.0 m inside PoleFeather; slots 386 390 394 398 kept in the s1fix block
Zone(TEXT("World s1d7, the crystal mine: the verge (2)"), 18.38f, 19.17f);
	Site(TEXT("WldS1D7LowAL"), -1, 24.9f, 4, 0, 1.f);     // left slots 408 412 416 420
	Site(TEXT("WldS1D7LowBL"), -1, 24.9f, 4, 2, 1.f);     // left slots 410 414 418 422
	Site(TEXT("WldS1D7LowAR"), 1, 24.9f, 12, 0, 1.f);     // right slots 408 420
	Site(TEXT("WldS1D7LowBR"), 1, 24.9f, 12, 8, 1.f);     // right slots 412
	Site(TEXT("WldS1D7LowBR"), 1, 24.9f, 12, 6, 1.f);     // right slots 414
	Site(TEXT("WldS1D7LowBR"), 1, 24.9f, 12, 2, 1.f);     // right slots 418
	Site(TEXT("WldS1D7TallAL"), -1, 27.6f, 4, 0, -1.f);     // left slots 408 412 416 420   // FIX ROUND: L6: its box was 2.0 m inside PoleFeather; slots 412 416 420 kept in the s1fix block
	Site(TEXT("WldS1D7TallBL"), -1, 27.6f, 4, 2, -1.f);     // left slots 410 414 418 422   // FIX ROUND: L6: its box was 2.2 m inside PoleFeather; slots 410 418 422 kept in the s1fix block
	Site(TEXT("WldS1D7TallCR"), 1, 27.6f, 4, 0, 1.f);     // right slots 408 412 416 420
	Site(TEXT("WldS1D7TallDR"), 1, 27.6f, 4, 2, 1.f);     // right slots 410 414 418 422
Zone(TEXT("World s1d7, the crystal mine: the verge (3)"), 19.52f, 19.7f);
	Site(TEXT("WldS1D7LowAL"), -1, 24.9f, 4, 0, 1.f);     // left slots 432
	Site(TEXT("WldS1D7LowBL"), -1, 24.9f, 4, 2, 1.f);     // left slots 434
	Site(TEXT("WldS1D7LowAR"), 1, 24.9f, 12, 0, 1.f);     // right slots 432
	Site(TEXT("WldS1D7TallAL"), -1, 27.6f, 4, 0, 1.f);     // left slots 432
	Site(TEXT("WldS1D7TallBL"), -1, 27.6f, 4, 2, 1.f);     // left slots 434
	Site(TEXT("WldS1D7TallCR"), 1, 27.6f, 4, 0, 1.f);     // right slots 432
	Site(TEXT("WldS1D7TallDR"), 1, 27.6f, 4, 2, 1.f);     // right slots 434
#undef S1D7
// ---- END s1d7 ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People5 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1d8 (district 8 "The high ridge", the beacons, 19.7 km to the end, ft_world_ice_s1d8.py) ----
// 17 meshes: 7 buildings (Beacon, HighBeacon, KeeperHouse, SignalStation, HornHouse, LiftTop, RidgeShelter), 9 props (tall: SignalMast,
// MarkerCone, FogBell, FlagHoist, BeaconLamp; low, 1.9 m and under: Bollards, LampCairn, FlagLocker, RopeWalk) and the hero (GreatBeacon).
// NOT COMPILED, NOT SEEN IN THE GAME when written. Every rule is a Site with chance 1 on ONE even slot (every 1000, phase 1000 - slot):
// nothing slides along its slot. A set holds two pieces, 20.5 m (buildings), 17 m (tall props) or 16 m (low props) either side of the
// slot's middle, where the two tables' own pieces (written at the slot's middle, up to 13.5 m either way) do not reach.
// A part's Up is -0.07 x along on the right, +0.07 x along on the left (the hill falls 7 in 100).
// Estimate: 144 Site rules write 301 pieces (102 buildings, 81 tall props, 116 low props, 2 hero); x 0.75 (the track's share left out) = about 226 placed. FIX ROUND: the 7 rules of slots 548 and 550 never fire (14 pieces): 287 written, not 301; after the fix round fewer, see the note L5_ice_s1.md.
//   By mesh, written (x 0.75): RopeWalk 31 (23.2), LampCairn 30 (22.5), FlagLocker 28 (21.0), Bollards 27 (20.2), KeeperHouse 22 (16.5), Beacon 22 (16.5), BeaconLamp 21 (15.8), SignalMast 21 (15.8), SignalStation 15 (11.2), RidgeShelter 15 (11.2), HornHouse 14 (10.5), MarkerCone 13 (9.8), FogBell 13 (9.8), FlagHoist 13 (9.8), HighBeacon 7 (5.2), LiftTop 7 (5.2), GreatBeacon 2 (1.5)
// Bands (metres from the centre line; slots s % 4 == 0 or 2 only, 436 to 550 = 19.72 to 24.93 km):
//   buildings, the high ridge (to 22.4 km): right 55 m on s % 4 == 0 (behind p3a's 46 m sets, before p3d's 65 to 67 m; a lamp or a mast between
//     the two at 58 m), left 47 m on s % 4 == 2 (the gusts lift their snow at 55 to 60 m on the left's even slots).
//   buildings, the last lifts: right 61.5 m (past the service road's far bank, 55.6 m) on s % 4 == 2 but not the depots' slots (500, 514, 528, 542);
//     left 45.5 m on slots 502, 508, 514, 516, 526, 532, 534, 538, 544, 550 (not the rink's, the street rows', the big-air hill's or the small pipe's).
//   tall props 32.5 m (behind the race banners' cloth, 26 to 31 m; before p3c's 38 m trails) and low props 25.0 m (behind p3a's 23 m pieces, before
//     the banners' masts): ONLY on the slots inside the dressing's verge zones (19.7 to 20.3, 20.66 to 21.44, 21.79 to 22.4, 22.93 to 23.72, 24.07 km on;
//     not the street slots 518 to 524, not the pipe-side slots 520 and 538), since beside a banked bend 17 to 40 m floats over the cutting.
//     Tall props: left on s % 4 == 0, right on s % 4 == 2 (the other way round on 466, 488, 514, 542, 544, where p3f has a three-part set at 30 to 34 m).
//     Low props: both sides; on the right not where p3a has a low piece. No tall props on 436 and 442 (p3f on both sides), no building on 494 left (p3f at 52 m).
//   hero THE GREAT BEACON: left, 110 m (its crag 95 to 125 m), slots 460 (20.83 km) and 516 (23.37 km).
// Light: steady only (lamp rooms 1.2, windows 0.6, the cairn's lamp 0.9); nothing blinks or turns. Smallest x of a lit piece: 25.0 m (LampCairn).
// Smallest x min of a TALL mesh: 32.5 m. Smallest of all: 25.0 m (the low props).
#define S1D8(n) TEXT("/Game/World/IceMountain/SM_S1D8_" n)
Set(TEXT("WldS1D8BldARLamp"), { { S1D8("KeeperHouse"), -20.5f, 0.0f, 1.44f, -12.0f, 1.f }, { S1D8("Beacon"), 20.5f, 0.5f, -1.44f, -12.0f, 1.f }, { S1D8("BeaconLamp"), 0.0f, 3.0f, 0.0f, 0.0f, 1.f } });
Set(TEXT("WldS1D8BldGL"), { { S1D8("Beacon"), -20.5f, 0.0f, -1.44f, 12.0f, 1.f }, { S1D8("SignalStation"), 20.5f, 0.5f, 1.44f, 12.0f, 1.f } });
Set(TEXT("WldS1D8BldBRMast"), { { S1D8("SignalStation"), -20.5f, 0.0f, 1.44f, -12.0f, 1.f }, { S1D8("RidgeShelter"), 20.5f, 0.5f, -1.44f, -12.0f, 1.f }, { S1D8("SignalMast"), 0.0f, 3.0f, 0.0f, 0.0f, 1.f } });
Set(TEXT("WldS1D8BldCL"), { { S1D8("HornHouse"), -20.5f, 0.0f, -1.44f, 12.0f, 1.f }, { S1D8("KeeperHouse"), 20.5f, 0.5f, 1.44f, 12.0f, 1.f } });
Set(TEXT("WldS1D8BldERLamp"), { { S1D8("HighBeacon"), -20.5f, 0.0f, 1.44f, -12.0f, 1.f }, { S1D8("KeeperHouse"), 20.5f, 0.5f, -1.44f, -12.0f, 1.f }, { S1D8("BeaconLamp"), 0.0f, 3.0f, 0.0f, 0.0f, 1.f } });
Set(TEXT("WldS1D8BldFL"), { { S1D8("RidgeShelter"), -20.5f, 0.0f, -1.44f, 12.0f, 1.f }, { S1D8("HornHouse"), 20.5f, 0.5f, 1.44f, 12.0f, 1.f } });
Set(TEXT("WldS1D8BldGRMast"), { { S1D8("Beacon"), -20.5f, 0.0f, 1.44f, -12.0f, 1.f }, { S1D8("SignalStation"), 20.5f, 0.5f, -1.44f, -12.0f, 1.f }, { S1D8("SignalMast"), 0.0f, 3.0f, 0.0f, 0.0f, 1.f } });
Set(TEXT("WldS1D8BldAL"), { { S1D8("KeeperHouse"), -20.5f, 0.0f, -1.44f, 12.0f, 1.f }, { S1D8("Beacon"), 20.5f, 0.5f, 1.44f, 12.0f, 1.f } });
Set(TEXT("WldS1D8BldCRLamp"), { { S1D8("HornHouse"), -20.5f, 0.0f, 1.44f, -12.0f, 1.f }, { S1D8("KeeperHouse"), 20.5f, 0.5f, -1.44f, -12.0f, 1.f }, { S1D8("BeaconLamp"), 0.0f, 3.0f, 0.0f, 0.0f, 1.f } });
Set(TEXT("WldS1D8BldBL"), { { S1D8("SignalStation"), -20.5f, 0.0f, -1.44f, 12.0f, 1.f }, { S1D8("RidgeShelter"), 20.5f, 0.5f, 1.44f, 12.0f, 1.f } });
Set(TEXT("WldS1D8BldFRMast"), { { S1D8("RidgeShelter"), -20.5f, 0.0f, 1.44f, -12.0f, 1.f }, { S1D8("HornHouse"), 20.5f, 0.5f, -1.44f, -12.0f, 1.f }, { S1D8("SignalMast"), 0.0f, 3.0f, 0.0f, 0.0f, 1.f } });
Set(TEXT("WldS1D8BldEL"), { { S1D8("HighBeacon"), -20.5f, 0.0f, -1.44f, 12.0f, 1.f }, { S1D8("KeeperHouse"), 20.5f, 0.5f, 1.44f, 12.0f, 1.f } });
Set(TEXT("WldS1D8BldDR"), { { S1D8("LiftTop"), -20.5f, 0.0f, 1.44f, -12.0f, 1.f }, { S1D8("Beacon"), 20.5f, 0.5f, -1.44f, -12.0f, 1.f } });
Set(TEXT("WldS1D8BldAR"), { { S1D8("KeeperHouse"), -20.5f, 0.0f, 1.44f, -12.0f, 1.f }, { S1D8("Beacon"), 20.5f, 0.5f, -1.44f, -12.0f, 1.f } });
Set(TEXT("WldS1D8BldFR"), { { S1D8("RidgeShelter"), -20.5f, 0.0f, 1.44f, -12.0f, 1.f }, { S1D8("HornHouse"), 20.5f, 0.5f, -1.44f, -12.0f, 1.f } });
Set(TEXT("WldS1D8BldBR"), { { S1D8("SignalStation"), -20.5f, 0.0f, 1.44f, -12.0f, 1.f }, { S1D8("RidgeShelter"), 20.5f, 0.5f, -1.44f, -12.0f, 1.f } });
Set(TEXT("WldS1D8BldER"), { { S1D8("HighBeacon"), -20.5f, 0.0f, 1.44f, -12.0f, 1.f }, { S1D8("KeeperHouse"), 20.5f, 0.5f, -1.44f, -12.0f, 1.f } });
Set(TEXT("WldS1D8BldCR"), { { S1D8("HornHouse"), -20.5f, 0.0f, 1.44f, -12.0f, 1.f }, { S1D8("KeeperHouse"), 20.5f, 0.5f, -1.44f, -12.0f, 1.f } });
Set(TEXT("WldS1D8BldGR"), { { S1D8("Beacon"), -20.5f, 0.0f, 1.44f, -12.0f, 1.f }, { S1D8("SignalStation"), 20.5f, 0.5f, -1.44f, -12.0f, 1.f } });
Set(TEXT("WldS1D8BldDL"), { { S1D8("LiftTop"), -20.5f, 0.0f, -1.44f, 12.0f, 1.f }, { S1D8("Beacon"), 20.5f, 0.5f, 1.44f, 12.0f, 1.f } });
Set(TEXT("WldS1D8TallAR"), { { S1D8("SignalMast"), -17.0f, 0.0f, 1.19f, 0.0f, 1.f }, { S1D8("MarkerCone"), 17.0f, 0.0f, -1.19f, 0.0f, 1.f } });
Set(TEXT("WldS1D8TallBL"), { { S1D8("FogBell"), -17.0f, 0.0f, -1.19f, 0.0f, 1.f }, { S1D8("FlagHoist"), 17.0f, 0.0f, 1.19f, 0.0f, 1.f } });
Set(TEXT("WldS1D8TallCL"), { { S1D8("BeaconLamp"), -17.0f, 0.0f, -1.19f, 0.0f, 1.f }, { S1D8("SignalMast"), 17.0f, 0.0f, 1.19f, 0.0f, 1.f } });
Set(TEXT("WldS1D8TallDR"), { { S1D8("FlagHoist"), -17.0f, 0.0f, 1.19f, 0.0f, 1.f }, { S1D8("MarkerCone"), 17.0f, 0.0f, -1.19f, 0.0f, 1.f } });
Set(TEXT("WldS1D8TallEL"), { { S1D8("BeaconLamp"), -17.0f, 0.0f, -1.19f, 0.0f, 1.f }, { S1D8("FogBell"), 17.0f, 0.0f, 1.19f, 0.0f, 1.f } });
Set(TEXT("WldS1D8TallCR"), { { S1D8("BeaconLamp"), -17.0f, 0.0f, 1.19f, 0.0f, 1.f }, { S1D8("SignalMast"), 17.0f, 0.0f, -1.19f, 0.0f, 1.f } });
Set(TEXT("WldS1D8TallDL"), { { S1D8("FlagHoist"), -17.0f, 0.0f, -1.19f, 0.0f, 1.f }, { S1D8("MarkerCone"), 17.0f, 0.0f, 1.19f, 0.0f, 1.f } });
Set(TEXT("WldS1D8TallAL"), { { S1D8("SignalMast"), -17.0f, 0.0f, -1.19f, 0.0f, 1.f }, { S1D8("MarkerCone"), 17.0f, 0.0f, 1.19f, 0.0f, 1.f } });
Set(TEXT("WldS1D8TallBR"), { { S1D8("FogBell"), -17.0f, 0.0f, 1.19f, 0.0f, 1.f }, { S1D8("FlagHoist"), 17.0f, 0.0f, -1.19f, 0.0f, 1.f } });
Set(TEXT("WldS1D8TallER"), { { S1D8("BeaconLamp"), -17.0f, 0.0f, 1.19f, 0.0f, 1.f }, { S1D8("FogBell"), 17.0f, 0.0f, -1.19f, 0.0f, 1.f } });
Set(TEXT("WldS1D8LowAL"), { { S1D8("Bollards"), -16.0f, 0.0f, -1.12f, 0.0f, 1.f }, { S1D8("LampCairn"), 16.0f, 0.0f, 1.12f, 0.0f, 1.f } });
Set(TEXT("WldS1D8LowCR"), { { S1D8("LampCairn"), -16.0f, 0.0f, 1.12f, 0.0f, 1.f }, { S1D8("RopeWalk"), 16.0f, 0.0f, -1.12f, 0.0f, 1.f } });
Set(TEXT("WldS1D8LowBL"), { { S1D8("RopeWalk"), -16.0f, 0.0f, -1.12f, 0.0f, 1.f }, { S1D8("FlagLocker"), 16.0f, 0.0f, 1.12f, 0.0f, 1.f } });
Set(TEXT("WldS1D8LowDR"), { { S1D8("FlagLocker"), -16.0f, 0.0f, 1.12f, 0.0f, 1.f }, { S1D8("Bollards"), 16.0f, 0.0f, -1.12f, 0.0f, 1.f } });
Set(TEXT("WldS1D8LowCL"), { { S1D8("LampCairn"), -16.0f, 0.0f, -1.12f, 0.0f, 1.f }, { S1D8("RopeWalk"), 16.0f, 0.0f, 1.12f, 0.0f, 1.f } });
Set(TEXT("WldS1D8LowDL"), { { S1D8("FlagLocker"), -16.0f, 0.0f, -1.12f, 0.0f, 1.f }, { S1D8("Bollards"), 16.0f, 0.0f, 1.12f, 0.0f, 1.f } });
Set(TEXT("WldS1D8LowBR"), { { S1D8("RopeWalk"), -16.0f, 0.0f, 1.12f, 0.0f, 1.f }, { S1D8("FlagLocker"), 16.0f, 0.0f, -1.12f, 0.0f, 1.f } });
Set(TEXT("WldS1D8LowAR"), { { S1D8("Bollards"), -16.0f, 0.0f, 1.12f, 0.0f, 1.f }, { S1D8("LampCairn"), 16.0f, 0.0f, -1.12f, 0.0f, 1.f } });
Set(TEXT("WldS1D8GreatBeacon"), { { S1D8("GreatBeacon"), 0.0f, 0.0f, 0.0f, 0.0f, 1.f } });
Zone(TEXT("World s1d8, The beacons"), 19.7f, 99.f);
	Site(TEXT("WldS1D8LowAL"), -1, 25.0f, 1000, 564, 1.f);	// low props, left, slot 436 (19.75 km)
	Site(TEXT("WldS1D8LowCR"), 1, 25.0f, 1000, 564, 1.f);	// low props, right, slot 436 (19.75 km)
	Site(TEXT("WldS1D8BldARLamp"), 1, 55.0f, 1000, 564, 1.f);	// buildings, right, slot 436 (19.75 km)
	Site(TEXT("WldS1D8LowBL"), -1, 25.0f, 1000, 562, 1.f);	// low props, left, slot 438 (19.84 km)
	Site(TEXT("WldS1D8BldGL"), -1, 47.0f, 1000, 562, 1.f);	// buildings, left, slot 438 (19.84 km)
	Site(TEXT("WldS1D8LowDR"), 1, 25.0f, 1000, 562, 1.f);	// low props, right, slot 438 (19.84 km)
	Site(TEXT("WldS1D8TallAR"), 1, 32.5f, 1000, 562, 1.f);	// tall props, right, slot 438 (19.84 km)
	Site(TEXT("WldS1D8LowCL"), -1, 25.0f, 1000, 560, 1.f);	// low props, left, slot 440 (19.93 km)
	Site(TEXT("WldS1D8TallBL"), -1, 32.5f, 1000, 560, 1.f);	// tall props, left, slot 440 (19.93 km)
	Site(TEXT("WldS1D8BldBRMast"), 1, 55.0f, 1000, 560, 1.f);	// buildings, right, slot 440 (19.93 km)
	Site(TEXT("WldS1D8LowDL"), -1, 25.0f, 1000, 558, 1.f);	// low props, left, slot 442 (20.02 km)
	Site(TEXT("WldS1D8BldCL"), -1, 47.0f, 1000, 558, 1.f);	// buildings, left, slot 442 (20.02 km)
	Site(TEXT("WldS1D8LowBR"), 1, 25.0f, 1000, 558, 1.f);	// low props, right, slot 442 (20.02 km)
	Site(TEXT("WldS1D8LowAL"), -1, 25.0f, 1000, 556, 1.f);	// low props, left, slot 444 (20.11 km)
	Site(TEXT("WldS1D8TallCL"), -1, 32.5f, 1000, 556, 1.f);	// tall props, left, slot 444 (20.11 km)
	Site(TEXT("WldS1D8LowCR"), 1, 25.0f, 1000, 556, 1.f);	// low props, right, slot 444 (20.11 km)
	Site(TEXT("WldS1D8BldERLamp"), 1, 55.0f, 1000, 556, 1.f);	// buildings, right, slot 444 (20.11 km)
	Site(TEXT("WldS1D8LowBL"), -1, 25.0f, 1000, 554, 1.f);	// low props, left, slot 446 (20.20 km)
	Site(TEXT("WldS1D8BldFL"), -1, 47.0f, 1000, 554, 1.f);	// buildings, left, slot 446 (20.20 km)
	Site(TEXT("WldS1D8TallDR"), 1, 32.5f, 1000, 554, 1.f);	// tall props, right, slot 446 (20.20 km)
	Site(TEXT("WldS1D8LowCL"), -1, 25.0f, 1000, 552, 1.f);	// low props, left, slot 448 (20.29 km)
	Site(TEXT("WldS1D8TallEL"), -1, 32.5f, 1000, 552, 1.f);	// tall props, left, slot 448 (20.29 km)
	Site(TEXT("WldS1D8LowAR"), 1, 25.0f, 1000, 552, 1.f);	// low props, right, slot 448 (20.29 km)
	Site(TEXT("WldS1D8BldGRMast"), 1, 55.0f, 1000, 552, 1.f);	// buildings, right, slot 448 (20.29 km)
	Site(TEXT("WldS1D8BldAL"), -1, 47.0f, 1000, 550, 1.f);	// buildings, left, slot 450 (20.38 km)
	Site(TEXT("WldS1D8BldCRLamp"), 1, 55.0f, 1000, 548, 1.f);	// buildings, right, slot 452 (20.47 km)
	Site(TEXT("WldS1D8BldBLFx0"), -1, 47.0f, 1000, 546, 1.f);	// buildings, left, slot 454 (20.56 km)   // FIX ROUND: L6: its box was 3.3 m inside P3F_Extras_B
	Site(TEXT("WldS1D8BldFRMast"), 1, 55.0f, 1000, 544, 1.f);	// buildings, right, slot 456 (20.65 km)
	Site(TEXT("WldS1D8LowDL"), -1, 25.0f, 1000, 542, 1.f);	// low props, left, slot 458 (20.74 km)
	Site(TEXT("WldS1D8BldEL"), -1, 47.0f, 1000, 542, 1.f);	// buildings, left, slot 458 (20.74 km)
	Site(TEXT("WldS1D8TallAR"), 1, 32.5f, 1000, 542, 1.f);	// tall props, right, slot 458 (20.74 km)
	Site(TEXT("WldS1D8LowAL"), -1, 25.0f, 1000, 540, 1.f);	// low props, left, slot 460 (20.83 km)
	Site(TEXT("WldS1D8TallBL"), -1, 32.5f, 1000, 540, 1.f);	// tall props, left, slot 460 (20.83 km)
	Site(TEXT("WldS1D8GreatBeacon"), -1, 110.0f, 1000, 540, 1.f);	// THE GREAT BEACON, left, slot 460 (20.83 km)
	Site(TEXT("WldS1D8LowCR"), 1, 25.0f, 1000, 540, 1.f);	// low props, right, slot 460 (20.83 km)
	Site(TEXT("WldS1D8BldARLamp"), 1, 55.0f, 1000, 540, 1.f);	// buildings, right, slot 460 (20.83 km)
	Site(TEXT("WldS1D8LowBL"), -1, 25.0f, 1000, 538, 1.f);	// low props, left, slot 462 (20.92 km)
	Site(TEXT("WldS1D8BldGL"), -1, 47.0f, 1000, 538, 1.f);	// buildings, left, slot 462 (20.92 km)
	Site(TEXT("WldS1D8LowDR"), 1, 25.0f, 1000, 538, 1.f);	// low props, right, slot 462 (20.92 km)
	Site(TEXT("WldS1D8TallCR"), 1, 32.5f, 1000, 538, 1.f);	// tall props, right, slot 462 (20.92 km)
	Site(TEXT("WldS1D8LowCL"), -1, 25.0f, 1000, 536, 1.f);	// low props, left, slot 464 (21.01 km)
	Site(TEXT("WldS1D8TallDL"), -1, 32.5f, 1000, 536, 1.f);	// tall props, left, slot 464 (21.01 km)
	Site(TEXT("WldS1D8BldBRMast"), 1, 55.0f, 1000, 536, 1.f);	// buildings, right, slot 464 (21.01 km)
	Site(TEXT("WldS1D8LowDL"), -1, 25.0f, 1000, 534, 1.f);	// low props, left, slot 466 (21.10 km)
	Site(TEXT("WldS1D8TallEL"), -1, 32.5f, 1000, 534, 1.f);	// tall props, left, slot 466 (21.10 km)
	Site(TEXT("WldS1D8BldCL"), -1, 47.0f, 1000, 534, 1.f);	// buildings, left, slot 466 (21.10 km)
	Site(TEXT("WldS1D8LowBR"), 1, 25.0f, 1000, 534, 1.f);	// low props, right, slot 466 (21.10 km)
	Site(TEXT("WldS1D8LowAL"), -1, 25.0f, 1000, 532, 1.f);	// low props, left, slot 468 (21.19 km)
	Site(TEXT("WldS1D8TallAL"), -1, 32.5f, 1000, 532, 1.f);	// tall props, left, slot 468 (21.19 km)
	Site(TEXT("WldS1D8LowCR"), 1, 25.0f, 1000, 532, 1.f);	// low props, right, slot 468 (21.19 km)
	Site(TEXT("WldS1D8BldERLamp"), 1, 55.0f, 1000, 532, 1.f);	// buildings, right, slot 468 (21.19 km)
	Site(TEXT("WldS1D8LowBL"), -1, 25.0f, 1000, 530, 1.f);	// low props, left, slot 470 (21.29 km)
	Site(TEXT("WldS1D8BldFL"), -1, 47.0f, 1000, 530, 1.f);	// buildings, left, slot 470 (21.29 km)
	Site(TEXT("WldS1D8TallBR"), 1, 32.5f, 1000, 530, 1.f);	// tall props, right, slot 470 (21.29 km)
	Site(TEXT("WldS1D8LowCL"), -1, 25.0f, 1000, 528, 1.f);	// low props, left, slot 472 (21.38 km)
	Site(TEXT("WldS1D8TallCL"), -1, 32.5f, 1000, 528, 1.f);	// tall props, left, slot 472 (21.38 km)
	Site(TEXT("WldS1D8LowAR"), 1, 25.0f, 1000, 528, 1.f);	// low props, right, slot 472 (21.38 km)
	Site(TEXT("WldS1D8BldGRMast"), 1, 55.0f, 1000, 528, 1.f);	// buildings, right, slot 472 (21.38 km)
	Site(TEXT("WldS1D8BldAL"), -1, 47.0f, 1000, 526, 1.f);	// buildings, left, slot 474 (21.47 km)
	Site(TEXT("WldS1D8BldCRLamp"), 1, 55.0f, 1000, 524, 1.f);	// buildings, right, slot 476 (21.56 km)
	Site(TEXT("WldS1D8BldBL"), -1, 47.0f, 1000, 522, 1.f);	// buildings, left, slot 478 (21.65 km)
	Site(TEXT("WldS1D8BldFRMast"), 1, 55.0f, 1000, 520, 1.f);	// buildings, right, slot 480 (21.74 km)
	Site(TEXT("WldS1D8LowDL"), -1, 25.0f, 1000, 518, 1.f);	// low props, left, slot 482 (21.83 km)
	Site(TEXT("WldS1D8BldELFx1"), -1, 47.0f, 1000, 518, 1.f);	// buildings, left, slot 482 (21.83 km)   // FIX ROUND: L6: its box was 3.7 m inside P3F_Extras_A
	Site(TEXT("WldS1D8TallDR"), 1, 32.5f, 1000, 518, 1.f);	// tall props, right, slot 482 (21.83 km)
	Site(TEXT("WldS1D8LowAL"), -1, 25.0f, 1000, 516, 1.f);	// low props, left, slot 484 (21.92 km)
	Site(TEXT("WldS1D8TallEL"), -1, 32.5f, 1000, 516, 1.f);	// tall props, left, slot 484 (21.92 km)
	Site(TEXT("WldS1D8LowCR"), 1, 25.0f, 1000, 516, 1.f);	// low props, right, slot 484 (21.92 km)
	Site(TEXT("WldS1D8BldARLamp"), 1, 55.0f, 1000, 516, 1.f);	// buildings, right, slot 484 (21.92 km)
	Site(TEXT("WldS1D8LowBL"), -1, 25.0f, 1000, 514, 1.f);	// low props, left, slot 486 (22.01 km)
	Site(TEXT("WldS1D8BldGL"), -1, 47.0f, 1000, 514, 1.f);	// buildings, left, slot 486 (22.01 km)
	Site(TEXT("WldS1D8LowDR"), 1, 25.0f, 1000, 514, 1.f);	// low props, right, slot 486 (22.01 km)
	Site(TEXT("WldS1D8TallAR"), 1, 32.5f, 1000, 514, 1.f);	// tall props, right, slot 486 (22.01 km)
	Site(TEXT("WldS1D8LowCL"), -1, 25.0f, 1000, 512, 1.f);	// low props, left, slot 488 (22.10 km)
	Site(TEXT("WldS1D8TallBR"), 1, 32.5f, 1000, 512, 1.f);	// tall props, right, slot 488 (22.10 km)
	Site(TEXT("WldS1D8BldBRMast"), 1, 55.0f, 1000, 512, 1.f);	// buildings, right, slot 488 (22.10 km)
	Site(TEXT("WldS1D8LowDL"), -1, 25.0f, 1000, 510, 1.f);	// low props, left, slot 490 (22.19 km)
	Site(TEXT("WldS1D8BldCL"), -1, 47.0f, 1000, 510, 1.f);	// buildings, left, slot 490 (22.19 km)
	Site(TEXT("WldS1D8LowBR"), 1, 25.0f, 1000, 510, 1.f);	// low props, right, slot 490 (22.19 km)
	Site(TEXT("WldS1D8TallCR"), 1, 32.5f, 1000, 510, 1.f);	// tall props, right, slot 490 (22.19 km)
	Site(TEXT("WldS1D8LowAL"), -1, 25.0f, 1000, 508, 1.f);	// low props, left, slot 492 (22.28 km)
	Site(TEXT("WldS1D8TallDL"), -1, 32.5f, 1000, 508, 1.f);	// tall props, left, slot 492 (22.28 km)
	Site(TEXT("WldS1D8LowCR"), 1, 25.0f, 1000, 508, 1.f);	// low props, right, slot 492 (22.28 km)
	Site(TEXT("WldS1D8BldERLamp"), 1, 55.0f, 1000, 508, 1.f);	// buildings, right, slot 492 (22.28 km)
	Site(TEXT("WldS1D8LowBL"), -1, 25.0f, 1000, 506, 1.f);	// low props, left, slot 494 (22.37 km)
	Site(TEXT("WldS1D8TallER"), 1, 32.5f, 1000, 506, 1.f);	// tall props, right, slot 494 (22.37 km)
	Site(TEXT("WldS1D8BldDR"), 1, 61.5f, 1000, 502, 1.f);	// buildings, right, slot 498 (22.55 km)
	Site(TEXT("WldS1D8BldDLFx1"), -1, 45.5f, 1000, 498, 1.f);	// buildings, left, slot 502 (22.73 km)   // FIX ROUND: L6: its box was 4.7 m inside SkiHillL_A
	Site(TEXT("WldS1D8BldAR"), 1, 61.5f, 1000, 498, 1.f);	// buildings, right, slot 502 (22.73 km)
	Site(TEXT("WldS1D8BldFR"), 1, 61.5f, 1000, 494, 1.f);	// buildings, right, slot 506 (22.91 km)
	Site(TEXT("WldS1D8LowCL"), -1, 25.0f, 1000, 492, 1.f);	// low props, left, slot 508 (23.00 km)
	Site(TEXT("WldS1D8TallAL"), -1, 32.5f, 1000, 492, 1.f);	// tall props, left, slot 508 (23.00 km)
	Site(TEXT("WldS1D8BldBL"), -1, 45.5f, 1000, 492, 1.f);	// buildings, left, slot 508 (23.00 km)
	Site(TEXT("WldS1D8LowAR"), 1, 25.0f, 1000, 492, 1.f);	// low props, right, slot 508 (23.00 km)
	Site(TEXT("WldS1D8LowDL"), -1, 25.0f, 1000, 490, 1.f);	// low props, left, slot 510 (23.10 km)
	Site(TEXT("WldS1D8LowBR"), 1, 25.0f, 1000, 490, 1.f);	// low props, right, slot 510 (23.10 km)
	Site(TEXT("WldS1D8TallBR"), 1, 32.5f, 1000, 490, 1.f);	// tall props, right, slot 510 (23.10 km)
	Site(TEXT("WldS1D8BldDR"), 1, 61.5f, 1000, 490, 1.f);	// buildings, right, slot 510 (23.10 km)
	Site(TEXT("WldS1D8LowAL"), -1, 25.0f, 1000, 488, 1.f);	// low props, left, slot 512 (23.19 km)
	Site(TEXT("WldS1D8TallCL"), -1, 32.5f, 1000, 488, 1.f);	// tall props, left, slot 512 (23.19 km)
	Site(TEXT("WldS1D8LowBL"), -1, 25.0f, 1000, 486, 1.f);	// low props, left, slot 514 (23.28 km)
	Site(TEXT("WldS1D8TallDL"), -1, 32.5f, 1000, 486, 1.f);	// tall props, left, slot 514 (23.28 km)
	Site(TEXT("WldS1D8BldEL"), -1, 45.5f, 1000, 486, 1.f);	// buildings, left, slot 514 (23.28 km)
	Site(TEXT("WldS1D8LowDR"), 1, 25.0f, 1000, 486, 1.f);	// low props, right, slot 514 (23.28 km)
	Site(TEXT("WldS1D8LowCL"), -1, 25.0f, 1000, 484, 1.f);	// low props, left, slot 516 (23.37 km)
	Site(TEXT("WldS1D8TallEL"), -1, 32.5f, 1000, 484, 1.f);	// tall props, left, slot 516 (23.37 km)
	Site(TEXT("WldS1D8BldCL"), -1, 45.5f, 1000, 484, 1.f);	// buildings, left, slot 516 (23.37 km)
	Site(TEXT("WldS1D8GreatBeacon"), -1, 110.0f, 1000, 484, 1.f);	// THE GREAT BEACON, left, slot 516 (23.37 km)
	Site(TEXT("WldS1D8LowAR"), 1, 25.0f, 1000, 484, 1.f);	// low props, right, slot 516 (23.37 km)
	Site(TEXT("WldS1D8BldBR"), 1, 61.5f, 1000, 482, 1.f);	// buildings, right, slot 518 (23.46 km)
	Site(TEXT("WldS1D8BldER"), 1, 61.5f, 1000, 478, -1.f);	// buildings, right, slot 522 (23.64 km)   // FIX ROUND: L4: stood on the car park of slot 521
	Site(TEXT("WldS1D8BldGL"), -1, 45.5f, 1000, 474, 1.f);	// buildings, left, slot 526 (23.82 km)
	Site(TEXT("WldS1D8BldCR"), 1, 61.5f, 1000, 474, 1.f);	// buildings, right, slot 526 (23.82 km)
	Site(TEXT("WldS1D8BldGR"), 1, 61.5f, 1000, 470, 1.f);	// buildings, right, slot 530 (24.00 km)
	Site(TEXT("WldS1D8BldDLFx1"), -1, 45.5f, 1000, 468, 1.f);	// buildings, left, slot 532 (24.09 km)   // FIX ROUND: L6: its box was 2.4 m inside P3F_Extras_A
	Site(TEXT("WldS1D8LowDL"), -1, 25.0f, 1000, 466, 1.f);	// low props, left, slot 534 (24.18 km)
	Site(TEXT("WldS1D8BldAL"), -1, 45.5f, 1000, 466, 1.f);	// buildings, left, slot 534 (24.18 km)
	Site(TEXT("WldS1D8LowBR"), 1, 25.0f, 1000, 466, 1.f);	// low props, right, slot 534 (24.18 km)
	Site(TEXT("WldS1D8TallAR"), 1, 32.5f, 1000, 466, 1.f);	// tall props, right, slot 534 (24.18 km)
	Site(TEXT("WldS1D8BldDR"), 1, 61.5f, 1000, 466, 1.f);	// buildings, right, slot 534 (24.18 km)
	Site(TEXT("WldS1D8LowAL"), -1, 25.0f, 1000, 464, 1.f);	// low props, left, slot 536 (24.27 km)
	Site(TEXT("WldS1D8TallBL"), -1, 32.5f, 1000, 464, 1.f);	// tall props, left, slot 536 (24.27 km)
	Site(TEXT("WldS1D8BldFL"), -1, 45.5f, 1000, 462, 1.f);	// buildings, left, slot 538 (24.36 km)
	Site(TEXT("WldS1D8BldAR"), 1, 61.5f, 1000, 462, 1.f);	// buildings, right, slot 538 (24.36 km)
	Site(TEXT("WldS1D8LowBL"), -1, 25.0f, 1000, 460, 1.f);	// low props, left, slot 540 (24.45 km)
	Site(TEXT("WldS1D8TallCL"), -1, 32.5f, 1000, 460, 1.f);	// tall props, left, slot 540 (24.45 km)
	Site(TEXT("WldS1D8LowDR"), 1, 25.0f, 1000, 460, 1.f);	// low props, right, slot 540 (24.45 km)
	Site(TEXT("WldS1D8LowCL"), -1, 25.0f, 1000, 458, 1.f);	// low props, left, slot 542 (24.54 km)
	Site(TEXT("WldS1D8TallDL"), -1, 32.5f, 1000, 458, 1.f);	// tall props, left, slot 542 (24.54 km)
	Site(TEXT("WldS1D8LowDL"), -1, 25.0f, 1000, 456, 1.f);	// low props, left, slot 544 (24.63 km)
	Site(TEXT("WldS1D8BldDL"), -1, 45.5f, 1000, 456, 1.f);	// buildings, left, slot 544 (24.63 km)
	Site(TEXT("WldS1D8LowBR"), 1, 25.0f, 1000, 456, 1.f);	// low props, right, slot 544 (24.63 km)
	Site(TEXT("WldS1D8TallER"), 1, 32.5f, 1000, 456, 1.f);	// tall props, right, slot 544 (24.63 km)
	Site(TEXT("WldS1D8LowAL"), -1, 25.0f, 1000, 454, 1.f);	// low props, left, slot 546 (24.72 km)
	Site(TEXT("WldS1D8LowCR"), 1, 25.0f, 1000, 454, 1.f);	// low props, right, slot 546 (24.72 km)
	Site(TEXT("WldS1D8TallAR"), 1, 32.5f, 1000, 454, 1.f);	// tall props, right, slot 546 (24.72 km)
	Site(TEXT("WldS1D8BldFR"), 1, 61.5f, 1000, 454, 1.f);	// buildings, right, slot 546 (24.72 km)
	Site(TEXT("WldS1D8LowBL"), -1, 25.0f, 1000, 452, 1.f);	// low props, left, slot 548 (24.81 km)   // FIX ROUND (W1): never placed, the road ends at slot 547; left written so that no later rule is re-rolled
	Site(TEXT("WldS1D8TallBL"), -1, 32.5f, 1000, 452, 1.f);	// tall props, left, slot 548 (24.81 km)   // FIX ROUND (W1): never placed, the road ends at slot 547; left written so that no later rule is re-rolled
	Site(TEXT("WldS1D8LowCL"), -1, 25.0f, 1000, 450, 1.f);	// low props, left, slot 550 (24.90 km)   // FIX ROUND (W1): never placed, the road ends at slot 547; left written so that no later rule is re-rolled
	Site(TEXT("WldS1D8BldBL"), -1, 45.5f, 1000, 450, 1.f);	// buildings, left, slot 550 (24.90 km)   // FIX ROUND (W1): never placed, the road ends at slot 547; left written so that no later rule is re-rolled
	Site(TEXT("WldS1D8LowAR"), 1, 25.0f, 1000, 450, 1.f);	// low props, right, slot 550 (24.90 km)   // FIX ROUND (W1): never placed, the road ends at slot 547; left written so that no later rule is re-rolled
	Site(TEXT("WldS1D8TallCR"), 1, 32.5f, 1000, 450, 1.f);	// tall props, right, slot 550 (24.90 km)   // FIX ROUND (W1): never placed, the road ends at slot 547; left written so that no later rule is re-rolled
	Site(TEXT("WldS1D8BldDR"), 1, 61.5f, 1000, 450, 1.f);	// buildings, right, slot 550 (24.90 km)   // FIX ROUND (W1): never placed, the road ends at slot 547; left written so that no later rule is re-rolled
#undef S1D8
// ---- END s1d8 ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People5 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1plants (what grows on the mountain, district by district, ft_world_ice_s1plants.py) ----
// Written by C:/ftwork/jobs/L5s1plants/make_inl.py. 20 meshes, 405 rule lines, chance 1, one side a line, every 8 on slots s % 4 == 3 only
// (phase 5 = s % 8 == 3, phase 1 = s % 8 == 7). Counted slot by slot: 580 rule-slots = 580 pieces before the track leaves any out;
// x 0.75 (the share the track leaves out) = about 435 placed. Loops, corkscrews and jumps are not written at all, so the true share may be higher.
// Rule-slots by district: First tracks 34, Black diamond 41, The valley run 37, The Abominable's glacier 60, The white ridge 123, The resort 85, The ice again 38, The high ridge 97, The high ridge, the last lifts 65.
// By mesh (before the track's share): StormFir 57, LichenRock 55, DwarfPine 54, SnowFir 51, SnowGrass 34, FrostFern 34, Edelweiss 33, JuniperCone 33, Rowan 29, Heather 29, IceReed 27, FirGrove 23, JuniperHedge 23, GlowMoss 23, BlueSpruce 20, GoldLarch 14, AlpineRose 13, RoseBoxes 12, BirchCopse 11, FrostBirch 5.
// Bands (m from the centre line): LOW pieces (top 1.0 m and less as drawn) 17.5 to 23.6 at scale 1 where the road is not banked; the same pieces
// at 1.3 to 1.8 times from 25 m (40 m and more beside banked bends). TALL pieces 26 to 58; smallest x min of a TALL mesh 30.0, of a glowing one 28.0,
// of anything 22.0. Rules whose band starts nearer than 40 m (less the piece's half depth) are written only on slots that lie whole inside the
// dressing table's own verge zones (which leave the banked bends out). A slot is left out where a rule of FTDressL5.inl or FTPeopleL5.inl
// stands on the same slot and side within 1.5 m (a Site: 3.5 m; the service road 5.8 m, a depot 6 m; a street's chalet row: 15 m) of the band grown by
// the piece's half depth (a tree: by 1.5 m, its trunk and lowest boughs): 1252 rule-slots found no room and are not written.
// StormFir on the LEFT is a Site of a one-piece Set turned 180: the flagged side then points down the hill on both sides (one wind).
#define S1PLANTS(n) TEXT("/Game/World/IceMountain/SM_S1PLANTS_" n)
Set(TEXT("WldS1PLANTSStormL"), { { S1PLANTS("StormFir"), 0.f, 0.f, 0.f, 180.f, 1.f } });
Zone(TEXT("World s1plants, First tracks, 0.32 km"), 0.2941f, 0.3393f);   // slots 7 to 7
	Scatter(S1PLANTS("SnowFir"), 50.5f, 51.5f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("SnowFir"), 54.0f, 55.0f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("SnowGrass"), 57.5f, 58.5f, 8, 1, -1.f, 1.70f, 0.10f, 1, 180.0f);   // FIX ROUND: L6: its box was 2.1 m inside P3C_FatBikes_A
Zone(TEXT("World s1plants, First tracks, 0.50 km"), 0.4750f, 0.5203f);   // slots 11 to 11
	Scatter(S1PLANTS("BirchCopse"), 55.0f, 56.0f, 8, 5, 1.f, 1.00f, 0.08f, -1, 20.0f);
Zone(TEXT("World s1plants, First tracks, 0.68 km"), 0.6560f, 0.7012f);   // slots 15 to 15
	Scatter(S1PLANTS("FrostBirch"), 43.5f, 44.5f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("SnowFir"), 50.5f, 51.5f, 8, 1, -1.f, 1.00f, 0.12f, 1, 180.0f);   // FIX ROUND: L6: its box was 3.5 m inside P3A_HotDrinks_A
Zone(TEXT("World s1plants, First tracks, 0.68 km"), 0.6560f, 1.7870f);   // slots 15 to 39
	Scatter(S1PLANTS("SnowFir"), 54.0f, 55.0f, 16, 9, 1.f, 1.00f, 0.12f, -1, 180.0f);   // FIX ROUND: L6: its box was 2.7 m inside SkiHillL_A; L6: its box was 2.6 m inside SkiHillL_A; now only slots 23 39
	Scatter(S1PLANTS("SnowGrass"), 57.5f, 58.5f, 8, 1, 1.f, 1.70f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, First tracks, 0.86 km"), 0.8369f, 0.8822f);   // slots 19 to 19
	Scatter(S1PLANTS("BirchCopse"), 50.5f, 51.5f, 8, 5, 1.f, 1.00f, 0.08f, -1, 20.0f);
	Scatter(S1PLANTS("BirchCopse"), 54.0f, 55.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 20.0f);
	Scatter(S1PLANTS("FirGrove"), 49.5f, 50.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 20.0f);
Zone(TEXT("World s1plants, First tracks, 1.04 km"), 1.0179f, 1.0631f);   // slots 23 to 23
	Scatter(S1PLANTS("FrostBirch"), 49.5f, 50.5f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
Zone(TEXT("World s1plants, First tracks, 1.22 km"), 1.1989f, 1.2441f);   // slots 27 to 27
	Scatter(S1PLANTS("BirchCopse"), 54.0f, 55.0f, 8, 5, 1.f, 1.00f, 0.08f, -1, 20.0f);
	Scatter(S1PLANTS("FirGrove"), 48.0f, 49.0f, 8, 5, 1.f, 1.00f, 0.08f, -1, 20.0f);
	Scatter(S1PLANTS("SnowGrass"), 57.5f, 58.5f, 8, 5, 1.f, 1.70f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("BirchCopse"), 50.5f, 51.5f, 8, 5, -1.f, 1.00f, 0.08f, 1, 20.0f);   // FIX ROUND: L4: stood on the car park of slot 26
Zone(TEXT("World s1plants, First tracks, 1.40 km"), 1.3798f, 1.4251f);   // slots 31 to 31
	Scatter(S1PLANTS("SnowFir"), 54.0f, 55.0f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("FrostBirch"), 48.0f, 49.0f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("SnowGrass"), 57.5f, 58.5f, 8, 1, 1.f, 1.70f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, First tracks, 1.58 km"), 1.5608f, 1.6060f);   // slots 35 to 35
	Scatter(S1PLANTS("BirchCopse"), 50.5f, 51.5f, 8, 5, -1.f, 1.00f, 0.08f, -1, 20.0f);   // FIX ROUND: L6: its box was 4.2 m inside P3A_Igloo_A
	Scatter(S1PLANTS("BirchCopse"), 48.5f, 49.5f, 8, 5, -1.f, 1.00f, 0.08f, 1, 20.0f);   // FIX ROUND: L6: its box was 2.4 m inside WatchTower_A
Zone(TEXT("World s1plants, First tracks, 1.76 km"), 1.7417f, 1.7870f);   // slots 39 to 39
	Scatter(S1PLANTS("FrostBirch"), 48.0f, 49.0f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("SnowFir"), 50.5f, 51.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, First tracks, 1.95 km"), 1.9227f, 1.9679f);   // slots 43 to 43
	Scatter(S1PLANTS("BirchCopse"), 54.0f, 55.0f, 8, 5, 1.f, 1.00f, 0.08f, -1, 20.0f);
	Scatter(S1PLANTS("SnowGrass"), 57.5f, 58.5f, 8, 5, 1.f, 1.70f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("BirchCopse"), 54.0f, 55.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 20.0f);
	Scatter(S1PLANTS("FirGrove"), 48.0f, 49.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 20.0f);
	Scatter(S1PLANTS("SnowGrass"), 57.5f, 58.5f, 8, 5, -1.f, 1.70f, 0.10f, 1, 180.0f);   // FIX ROUND: L5: stood in the animals of this slot (S1ANIMALS_Kennel)
Zone(TEXT("World s1plants, Black diamond, 2.85 km"), 2.8275f, 2.8727f);   // slots 63 to 63
	Scatter(S1PLANTS("SnowFir"), 54.0f, 55.0f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Edelweiss"), 43.0f, 44.0f, 8, 1, -1.f, 1.70f, 0.10f, -1, 180.0f);   // FIX ROUND: L2: stood in the rock cutting
	Site(TEXT("WldS1PLANTSStormL"), -1, 47.5f, 8, 1, 1.f, 1.00f, 0.12f);
	Scatter(S1PLANTS("Edelweiss"), 45.0f, 46.0f, 8, 1, -1.f, 1.70f, 0.10f, 1, 180.0f);   // FIX ROUND: L2: stood in the rock cutting
	Scatter(S1PLANTS("StormFir"), 48.5f, 49.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 8.0f);
Zone(TEXT("World s1plants, Black diamond, 2.85 km"), 2.8275f, 3.9585f);   // slots 63 to 87
	Scatter(S1PLANTS("SnowFir"), 54.0f, 55.0f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, Black diamond, 3.03 km"), 3.0085f, 3.7775f);   // slots 67 to 83
	Scatter(S1PLANTS("StormFir"), 54.0f, 55.0f, 8, 5, 1.f, 1.00f, 0.12f, 1, 8.0f);
	Scatter(S1PLANTS("LichenRock"), 49.5f, 50.5f, 8, 5, 1.f, 1.70f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("SnowFir"), 57.5f, 58.5f, 8, 5, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, Black diamond, 3.21 km"), 3.1894f, 3.9585f);   // slots 71 to 87
	Scatter(S1PLANTS("Edelweiss"), 49.5f, 50.5f, 8, 1, 1.f, 1.70f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("StormFir"), 57.5f, 58.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 8.0f);
Zone(TEXT("World s1plants, Black diamond, 4.48 km"), 4.4561f, 4.5014f);   // slots 99 to 99
	Site(TEXT("WldS1PLANTSStormL"), -1, 54.5f, 8, 5, 1.f, 1.00f, 0.12f);
	Scatter(S1PLANTS("SnowFir"), 57.5f, 58.5f, 8, 5, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("StormFir"), 54.0f, 55.0f, 8, 5, 1.f, 1.00f, 0.12f, 1, 8.0f);
	Scatter(S1PLANTS("LichenRock"), 49.5f, 50.5f, 8, 5, 1.f, 1.70f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("SnowFir"), 57.5f, 58.5f, 8, 5, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, Black diamond, 4.66 km"), 4.6371f, 4.6823f);   // slots 103 to 103
	Scatter(S1PLANTS("SnowFir"), 54.0f, 55.0f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Edelweiss"), 49.5f, 50.5f, 8, 1, 1.f, 1.70f, 0.10f, -1, 180.0f);
	Site(TEXT("WldS1PLANTSStormL"), -1, 58.0f, 8, 1, 1.f, 1.00f, 0.12f);
	Scatter(S1PLANTS("SnowFir"), 54.0f, 55.0f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Edelweiss"), 49.5f, 50.5f, 8, 1, 1.f, 1.70f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("StormFir"), 57.5f, 58.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 8.0f);
Zone(TEXT("World s1plants, Black diamond, 5.56 km"), 5.5419f, 5.5871f);   // slots 123 to 123
	Site(TEXT("WldS1PLANTSStormL"), -1, 54.5f, 8, 5, 1.f, 1.00f, 0.12f);
	Scatter(S1PLANTS("LichenRock"), 49.5f, 50.5f, 8, 5, 1.f, 1.70f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("SnowFir"), 57.5f, 58.5f, 8, 5, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("StormFir"), 54.0f, 55.0f, 8, 5, 1.f, 1.00f, 0.12f, 1, 8.0f);
	Scatter(S1PLANTS("LichenRock"), 49.5f, 50.5f, 8, 5, 1.f, 1.70f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("SnowFir"), 57.5f, 58.5f, 8, 5, -1.f, 1.00f, 0.12f, 1, 180.0f);   // FIX ROUND: L5: stood in the animals of this slot (S1ANIMALS_Ptarmigan)
Zone(TEXT("World s1plants, The valley run, 5.75 km"), 5.7229f, 5.7681f);   // slots 127 to 127
	Scatter(S1PLANTS("Rowan"), 50.5f, 51.5f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Rowan"), 51.5f, 52.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("GoldLarch"), 47.5f, 48.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, The valley run, 5.93 km"), 5.9038f, 5.9491f);   // slots 131 to 131
	Scatter(S1PLANTS("GoldLarch"), 54.0f, 55.0f, 8, 5, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("AlpineRose"), 57.5f, 58.5f, 8, 5, 1.f, 1.60f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("GoldLarch"), 48.5f, 49.5f, 8, 5, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, The valley run, 6.11 km"), 6.0848f, 6.1300f);   // slots 135 to 135
	Scatter(S1PLANTS("Rowan"), 54.0f, 55.0f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("GoldLarch"), 48.0f, 49.0f, 8, 1, -1.f, 1.00f, 0.12f, -1, 180.0f);   // FIX ROUND: L6: its box was 3.0 m inside WatchBank_B
	Scatter(S1PLANTS("SnowGrass"), 57.5f, 58.5f, 8, 1, 1.f, 1.70f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Rowan"), 50.5f, 51.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, The valley run, 6.29 km"), 6.2657f, 6.3110f);   // slots 139 to 139
	Scatter(S1PLANTS("GoldLarch"), 50.5f, 51.5f, 8, 5, -1.f, 1.00f, 0.12f, -1, 180.0f);   // FIX ROUND: L6: its box was 4.2 m inside P3A_PisteCrew_B
Zone(TEXT("World s1plants, The valley run, 6.47 km"), 6.4467f, 6.4919f);   // slots 143 to 143
	Scatter(S1PLANTS("Rowan"), 55.5f, 56.5f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Rowan"), 48.5f, 49.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, The valley run, 6.65 km"), 6.6277f, 6.6729f);   // slots 147 to 147
	Scatter(S1PLANTS("GoldLarch"), 54.0f, 55.0f, 8, 5, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("BirchCopse"), 48.0f, 49.0f, 8, 5, 1.f, 1.00f, 0.08f, -1, 20.0f);
	Scatter(S1PLANTS("AlpineRose"), 57.5f, 58.5f, 8, 5, 1.f, 1.60f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("GoldLarch"), 50.5f, 51.5f, 8, 5, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, The valley run, 7.56 km"), 7.5325f, 7.5777f);   // slots 167 to 167
	Scatter(S1PLANTS("Rowan"), 48.5f, 49.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, The valley run, 7.56 km"), 7.5325f, 7.9396f);   // slots 167 to 175
	Scatter(S1PLANTS("Rowan"), 50.5f, 51.5f, 1000, 825, 1.f, 1.00f, 0.12f, -1, 180.0f);   // FIX ROUND: L6: its box was 3.7 m inside P3A_Hikers_A; now only slot 175
Zone(TEXT("World s1plants, The valley run, 7.74 km"), 7.7134f, 7.7587f);   // slots 171 to 171
	Scatter(S1PLANTS("GoldLarch"), 55.5f, 56.5f, 8, 5, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("GoldLarch"), 50.5f, 51.5f, 8, 5, -1.f, 1.00f, 0.12f, 1, 180.0f);   // FIX ROUND: L6: its box was 2.2 m inside P3A_HotDrinks_A
Zone(TEXT("World s1plants, The valley run, 7.92 km"), 7.8944f, 7.9396f);   // slots 175 to 175
	Scatter(S1PLANTS("Rowan"), 54.0f, 55.0f, 8, 1, -1.f, 1.00f, 0.12f, 1, 180.0f);   // FIX ROUND: L4: stood on the car park of slot 174
	Scatter(S1PLANTS("SnowGrass"), 57.5f, 58.5f, 8, 1, -1.f, 1.70f, 0.10f, 1, 180.0f);   // FIX ROUND: L4: stood on the car park of slot 174
Zone(TEXT("World s1plants, The valley run, 8.46 km"), 8.4373f, 8.4825f);   // slots 187 to 187
	Scatter(S1PLANTS("GoldLarch"), 50.5f, 51.5f, 8, 5, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("GoldLarch"), 54.0f, 55.0f, 8, 5, -1.f, 1.00f, 0.12f, 1, 180.0f);   // FIX ROUND: L4: stood on the car park of slot 186
	Scatter(S1PLANTS("BirchCopse"), 48.0f, 49.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 20.0f);
	Scatter(S1PLANTS("AlpineRose"), 57.5f, 58.5f, 8, 5, -1.f, 1.60f, 0.10f, 1, 180.0f);   // FIX ROUND: L4: stood on the car park of slot 186
Zone(TEXT("World s1plants, The valley run, 8.64 km"), 8.6182f, 8.6635f);   // slots 191 to 191
	Scatter(S1PLANTS("Rowan"), 54.0f, 55.0f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("GoldLarch"), 48.0f, 49.0f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("SnowGrass"), 57.5f, 58.5f, 8, 1, 1.f, 1.70f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Rowan"), 48.5f, 49.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, The valley run, 8.82 km"), 8.7992f, 8.8444f);   // slots 195 to 195
	Scatter(S1PLANTS("GoldLarch"), 54.0f, 55.0f, 8, 5, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("AlpineRose"), 57.5f, 58.5f, 8, 5, 1.f, 1.60f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("GoldLarch"), 50.5f, 51.5f, 8, 5, 1.f, 1.00f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Rowan"), 30.0f, 31.0f, 8, 5, 1.f, 1.00f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("FrostBirch"), 35.0f, 36.0f, 8, 5, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, The Abominable's glacier, 9.00 km"), 8.9801f, 11.5588f);   // slots 199 to 255
	Scatter(S1PLANTS("FrostFern"), 48.5f, 49.5f, 24, 1, 1.f, 1.60f, 0.10f, -1, 180.0f);   // FIX ROUND: L6: its box was 4.8 m inside P3D_IceArch_A; L6: its box was 4.3 m inside P3D_IceArch_A; L6: its box was 3.4 m inside P3D_IceArch_A; L6: its box was 2.6 m inside P3D_IceArch_A; slots 207 247 also given up (one rule cannot keep them without the others); now only slots 215 239
	Scatter(S1PLANTS("FrostFern"), 51.0f, 52.0f, 8, 1, 1.f, 1.60f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The Abominable's glacier, 9.18 km"), 9.1611f, 9.2063f);   // slots 203 to 203
	Scatter(S1PLANTS("FrostFern"), 36.5f, 37.5f, 8, 5, -1.f, 1.30f, 0.10f, -1, 180.0f);   // FIX ROUND: L6: its box was 4.4 m inside IceClimb_B
	Scatter(S1PLANTS("IceReed"), 42.0f, 43.0f, 8, 5, 1.f, 1.60f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("FrostFern"), 29.0f, 30.0f, 8, 5, 1.f, 1.30f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The Abominable's glacier, 9.18 km"), 9.1611f, 9.5683f);   // slots 203 to 211
	Scatter(S1PLANTS("IceReed"), 56.5f, 57.5f, 1000, 797, 1.f, 1.60f, 0.10f, 1, 180.0f);   // FIX ROUND: L5: stood in the animals of this slot (S1ANIMALS_Seals); now only slot 203
Zone(TEXT("World s1plants, The Abominable's glacier, 9.18 km"), 9.1611f, 11.3779f);   // slots 203 to 251
	Scatter(S1PLANTS("LichenRock"), 47.5f, 48.5f, 24, 5, 1.f, 1.80f, 0.10f, -1, 180.0f);   // FIX ROUND: L6: its box was 6.1 m inside P3D_Bridge_B; L6: its box was 4.3 m inside P3D_Bridge_B; L6: its box was 2.5 m inside P3D_Bridge_B; slots 203 243 also given up (one rule cannot keep them without the others); now only slots 211 235
	Scatter(S1PLANTS("LichenRock"), 51.0f, 52.0f, 8, 5, 1.f, 1.80f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The Abominable's glacier, 9.36 km"), 9.3421f, 9.3873f);   // slots 207 to 207
	Scatter(S1PLANTS("IceReed"), 28.0f, 29.0f, 8, 1, 1.f, 1.30f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("IceReed"), 36.5f, 37.5f, 8, 1, 1.f, 1.30f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The Abominable's glacier, 9.73 km"), 9.7040f, 9.7492f);   // slots 215 to 215
	Scatter(S1PLANTS("IceReed"), 36.5f, 37.5f, 8, 1, 1.f, 1.30f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The Abominable's glacier, 9.91 km"), 9.8849f, 9.9302f);   // slots 219 to 219
	Scatter(S1PLANTS("FrostFern"), 29.0f, 30.0f, 8, 5, 1.f, 1.30f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("FrostFern"), 36.5f, 37.5f, 8, 5, 1.f, 1.30f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("IceReed"), 45.5f, 46.5f, 8, 5, 1.f, 1.60f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The Abominable's glacier, 9.91 km"), 9.8849f, 10.2921f);   // slots 219 to 227
	Scatter(S1PLANTS("IceReed"), 42.0f, 43.0f, 8, 5, 1.f, 1.60f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The Abominable's glacier, 10.27 km"), 10.2469f, 10.2921f);   // slots 227 to 227
	Scatter(S1PLANTS("FrostFern"), 36.5f, 37.5f, 8, 5, 1.f, 1.30f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("FrostFern"), 29.0f, 30.0f, 8, 5, 1.f, 1.30f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The Abominable's glacier, 10.27 km"), 10.2469f, 10.6540f);   // slots 227 to 235
	Scatter(S1PLANTS("IceReed"), 56.5f, 57.5f, 1000, 773, 1.f, 1.60f, 0.10f, 1, 180.0f);   // FIX ROUND: L5: stood in the animals of this slot (S1ANIMALS_Seals); now only slot 227
Zone(TEXT("World s1plants, The Abominable's glacier, 10.45 km"), 10.4278f, 10.4731f);   // slots 231 to 231
	Scatter(S1PLANTS("IceReed"), 28.0f, 29.0f, 8, 1, 1.f, 1.30f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("IceReed"), 36.5f, 37.5f, 8, 1, 1.f, 1.30f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The Abominable's glacier, 10.81 km"), 10.7897f, 10.8350f);   // slots 239 to 239
	Scatter(S1PLANTS("IceReed"), 36.5f, 37.5f, 8, 1, 1.f, 1.30f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The Abominable's glacier, 10.99 km"), 10.9707f, 11.0159f);   // slots 243 to 243
	Scatter(S1PLANTS("FrostFern"), 29.0f, 30.0f, 8, 5, 1.f, 1.30f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("FrostFern"), 36.5f, 37.5f, 8, 5, 1.f, 1.30f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("IceReed"), 45.5f, 46.5f, 8, 5, 1.f, 1.60f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The Abominable's glacier, 10.99 km"), 10.9707f, 11.3779f);   // slots 243 to 251
	Scatter(S1PLANTS("IceReed"), 42.0f, 43.0f, 8, 5, 1.f, 1.60f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The Abominable's glacier, 11.36 km"), 11.3326f, 11.3779f);   // slots 251 to 251
	Scatter(S1PLANTS("FrostFern"), 36.5f, 37.5f, 8, 5, 1.f, 1.30f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("FrostFern"), 29.0f, 30.0f, 8, 5, 1.f, 1.30f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("IceReed"), 56.5f, 57.5f, 8, 5, 1.f, 1.60f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The Abominable's glacier, 11.54 km"), 11.5136f, 11.5588f);   // slots 255 to 255
	Scatter(S1PLANTS("IceReed"), 28.0f, 29.0f, 8, 1, 1.f, 1.30f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("IceReed"), 36.5f, 37.5f, 8, 1, 1.f, 1.30f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 11.72 km"), 11.6945f, 11.7398f);   // slots 259 to 259
	Site(TEXT("WldS1PLANTSStormL"), -1, 44.5f, 8, 5, 1.f, 1.00f, 0.12f);
	Scatter(S1PLANTS("Edelweiss"), 55.0f, 56.0f, 8, 5, 1.f, 1.80f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 35.5f, 36.5f, 8, 5, -1.f, 1.40f, 0.10f, 1, 180.0f);   // FIX ROUND: L6: its box was 2.2 m inside DogSled_A
Zone(TEXT("World s1plants, The white ridge, 11.72 km"), 11.6945f, 12.1017f);   // slots 259 to 267
	Scatter(S1PLANTS("DwarfPine"), 51.0f, 52.0f, 8, 5, 1.f, 1.80f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 11.72 km"), 11.6945f, 14.2732f);   // slots 259 to 315
	Scatter(S1PLANTS("DwarfPine"), 51.0f, 52.0f, 8, 5, 1.f, 1.80f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Heather"), 23.0f, 23.6f, 8, 5, 1.f, 1.00f, 0.06f, 1, 8.0f);
Zone(TEXT("World s1plants, The white ridge, 11.90 km"), 11.8755f, 11.9207f);   // slots 263 to 263
	Scatter(S1PLANTS("DwarfPine"), 22.5f, 23.1f, 8, 1, 1.f, 1.00f, 0.06f, -1, 8.0f);
	Scatter(S1PLANTS("DwarfPine"), 54.5f, 55.5f, 8, 1, 1.f, 1.70f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Edelweiss"), 36.5f, 37.5f, 8, 1, 1.f, 1.50f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Heather"), 40.0f, 41.0f, 8, 1, 1.f, 1.80f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("StormFir"), 43.5f, 44.5f, 8, 1, -1.f, 1.00f, 0.12f, 1, 8.0f);   // FIX ROUND: L6: its box was 2.6 m inside Ibex_A
Zone(TEXT("World s1plants, The white ridge, 11.90 km"), 11.8755f, 12.2827f);   // slots 263 to 271
	Scatter(S1PLANTS("Edelweiss"), 29.0f, 30.0f, 8, 1, 1.f, 1.50f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 11.90 km"), 11.8755f, 14.0923f);   // slots 263 to 311
	Scatter(S1PLANTS("Heather"), 37.0f, 38.0f, 8, 1, 1.f, 1.80f, 0.10f, -1, 180.0f);
	Site(TEXT("WldS1PLANTSStormL"), -1, 51.5f, 8, 1, 1.f, 1.00f, 0.12f);
	Scatter(S1PLANTS("DwarfPine"), 23.0f, 23.6f, 8, 1, 1.f, 1.00f, 0.06f, 1, 8.0f);
Zone(TEXT("World s1plants, The white ridge, 12.08 km"), 12.0565f, 12.1017f);   // slots 267 to 267
	Scatter(S1PLANTS("LichenRock"), 29.5f, 30.5f, 8, 5, 1.f, 1.40f, 0.10f, -1, 180.0f);
	Site(TEXT("WldS1PLANTSStormL"), -1, 37.5f, 8, 5, 1.f, 1.00f, 0.12f);
	Scatter(S1PLANTS("Edelweiss"), 44.0f, 45.0f, 8, 5, 1.f, 1.80f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 27.5f, 28.5f, 8, 5, 1.f, 1.40f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("StormFir"), 39.5f, 40.5f, 8, 5, 1.f, 1.00f, 0.12f, 1, 8.0f);
	Scatter(S1PLANTS("Edelweiss"), 44.0f, 45.0f, 8, 5, 1.f, 1.80f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 12.26 km"), 12.2374f, 12.2827f);   // slots 271 to 271
	Scatter(S1PLANTS("Edelweiss"), 35.5f, 36.5f, 8, 1, 1.f, 1.50f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("DwarfPine"), 47.5f, 48.5f, 8, 1, 1.f, 1.70f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 12.26 km"), 12.2374f, 12.6446f);   // slots 271 to 279
	Scatter(S1PLANTS("StormFir"), 51.0f, 52.0f, 8, 1, 1.f, 1.00f, 0.12f, 1, 8.0f);
Zone(TEXT("World s1plants, The white ridge, 12.26 km"), 12.2374f, 13.3684f);   // slots 271 to 295
	Scatter(S1PLANTS("DwarfPine"), 44.0f, 45.0f, 8, 1, 1.f, 1.70f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 12.44 km"), 12.4184f, 12.4636f);   // slots 275 to 275
	Scatter(S1PLANTS("Heather"), 22.5f, 23.1f, 8, 5, 1.f, 1.00f, 0.06f, -1, 8.0f);
	Site(TEXT("WldS1PLANTSStormL"), -1, 44.5f, 8, 5, 1.f, 1.00f, 0.12f);
	Scatter(S1PLANTS("DwarfPine"), 43.5f, 44.5f, 8, 5, 1.f, 1.80f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 12.44 km"), 12.4184f, 12.8255f);   // slots 275 to 283
	Scatter(S1PLANTS("LichenRock"), 29.0f, 30.0f, 8, 5, 1.f, 1.40f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 35.5f, 36.5f, 8, 5, 1.f, 1.40f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("StormFir"), 39.0f, 40.0f, 8, 5, 1.f, 1.00f, 0.12f, 1, 8.0f);
Zone(TEXT("World s1plants, The white ridge, 12.44 km"), 12.4184f, 13.1875f);   // slots 275 to 291
	Scatter(S1PLANTS("Edelweiss"), 55.0f, 56.0f, 8, 5, 1.f, 1.80f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 12.62 km"), 12.5993f, 12.6446f);   // slots 279 to 279
	Scatter(S1PLANTS("Edelweiss"), 29.5f, 30.5f, 8, 1, 1.f, 1.50f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Edelweiss"), 27.5f, 28.5f, 8, 1, 1.f, 1.50f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 12.80 km"), 12.7803f, 12.8255f);   // slots 283 to 283
	Site(TEXT("WldS1PLANTSStormL"), -1, 37.5f, 8, 5, 1.f, 1.00f, 0.12f);
	Scatter(S1PLANTS("Edelweiss"), 44.0f, 45.0f, 8, 5, 1.f, 1.80f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 12.80 km"), 12.7803f, 13.1875f);   // slots 283 to 291
	Scatter(S1PLANTS("DwarfPine"), 51.0f, 52.0f, 8, 5, 1.f, 1.80f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 12.98 km"), 12.9613f, 13.0065f);   // slots 287 to 287
	Scatter(S1PLANTS("DwarfPine"), 22.5f, 23.1f, 8, 1, 1.f, 1.00f, 0.06f, -1, 8.0f);
	Scatter(S1PLANTS("StormFir"), 43.5f, 44.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 8.0f);
Zone(TEXT("World s1plants, The white ridge, 12.98 km"), 12.9613f, 13.3684f);   // slots 287 to 295
	Scatter(S1PLANTS("Edelweiss"), 29.0f, 30.0f, 8, 1, 1.f, 1.50f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Edelweiss"), 35.5f, 36.5f, 8, 1, 1.f, 1.50f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Heather"), 39.0f, 40.0f, 8, 1, 1.f, 1.80f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 13.16 km"), 13.1422f, 13.1875f);   // slots 291 to 291
	Scatter(S1PLANTS("LichenRock"), 29.5f, 30.5f, 8, 5, 1.f, 1.40f, 0.10f, -1, 180.0f);
	Site(TEXT("WldS1PLANTSStormL"), -1, 44.5f, 8, 5, 1.f, 1.00f, 0.12f);
	Scatter(S1PLANTS("LichenRock"), 27.5f, 28.5f, 8, 5, 1.f, 1.40f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 13.35 km"), 13.3232f, 13.7303f);   // slots 295 to 303
	Scatter(S1PLANTS("StormFir"), 51.0f, 52.0f, 8, 1, 1.f, 1.00f, 0.12f, 1, 8.0f);
	Scatter(S1PLANTS("DwarfPine"), 44.0f, 45.0f, 8, 1, 1.f, 1.70f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 13.53 km"), 13.5041f, 13.5494f);   // slots 299 to 299
	Scatter(S1PLANTS("Heather"), 22.5f, 23.1f, 8, 5, 1.f, 1.00f, 0.06f, -1, 8.0f);
	Site(TEXT("WldS1PLANTSStormL"), -1, 37.5f, 8, 5, 1.f, 1.00f, 0.12f);
	Scatter(S1PLANTS("Edelweiss"), 44.0f, 45.0f, 8, 5, 1.f, 1.80f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 13.53 km"), 13.5041f, 13.9113f);   // slots 299 to 307
	Scatter(S1PLANTS("LichenRock"), 29.0f, 30.0f, 8, 5, 1.f, 1.40f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 35.5f, 36.5f, 8, 5, 1.f, 1.40f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 13.71 km"), 13.6851f, 13.7303f);   // slots 303 to 303
	Scatter(S1PLANTS("Edelweiss"), 29.5f, 30.5f, 8, 1, 1.f, 1.50f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("DwarfPine"), 54.5f, 55.5f, 8, 1, 1.f, 1.70f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Edelweiss"), 27.5f, 28.5f, 8, 1, 1.f, 1.50f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Heather"), 39.5f, 40.5f, 8, 1, 1.f, 1.80f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 13.89 km"), 13.8661f, 13.9113f);   // slots 307 to 307
	Site(TEXT("WldS1PLANTSStormL"), -1, 44.5f, 8, 5, 1.f, 1.00f, 0.12f);
	Scatter(S1PLANTS("Edelweiss"), 55.0f, 56.0f, 8, 5, 1.f, 1.80f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("StormFir"), 39.0f, 40.0f, 8, 5, 1.f, 1.00f, 0.12f, 1, 8.0f);
Zone(TEXT("World s1plants, The white ridge, 13.89 km"), 13.8661f, 14.2732f);   // slots 307 to 315
	Scatter(S1PLANTS("DwarfPine"), 51.0f, 52.0f, 8, 5, 1.f, 1.80f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Edelweiss"), 44.0f, 45.0f, 8, 5, 1.f, 1.80f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 14.07 km"), 14.0470f, 14.0923f);   // slots 311 to 311
	Scatter(S1PLANTS("DwarfPine"), 22.5f, 23.1f, 8, 1, 1.f, 1.00f, 0.06f, -1, 8.0f);
	Scatter(S1PLANTS("Edelweiss"), 29.0f, 30.0f, 8, 1, 1.f, 1.50f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("DwarfPine"), 44.0f, 45.0f, 8, 1, 1.f, 1.70f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Edelweiss"), 35.5f, 36.5f, 8, 1, 1.f, 1.50f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The white ridge, 14.25 km"), 14.2280f, 14.2732f);   // slots 315 to 315
	Scatter(S1PLANTS("LichenRock"), 29.5f, 30.5f, 8, 5, 1.f, 1.40f, 0.10f, -1, 180.0f);
	Site(TEXT("WldS1PLANTSStormL"), -1, 37.5f, 8, 5, 1.f, 1.00f, 0.12f);
	Scatter(S1PLANTS("Edelweiss"), 44.0f, 45.0f, 8, 5, 1.f, 1.80f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 27.5f, 28.5f, 8, 5, 1.f, 1.40f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("StormFir"), 39.5f, 40.5f, 8, 5, 1.f, 1.00f, 0.12f, 1, 8.0f);
Zone(TEXT("World s1plants, The resort, 14.43 km"), 14.4089f, 14.4542f);   // slots 319 to 319
	Scatter(S1PLANTS("Rowan"), 54.0f, 55.0f, 8, 1, -1.f, 1.00f, 0.12f, -1, 180.0f);   // FIX ROUND: L4: stood on the terrain park of slot 320
	Scatter(S1PLANTS("JuniperCone"), 40.0f, 41.0f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("BlueSpruce"), 57.5f, 58.5f, 8, 1, -1.f, 1.00f, 0.06f, -1, 0.0f);   // FIX ROUND: L4: stood on the terrain park of slot 320
	Scatter(S1PLANTS("BlueSpruce"), 37.0f, 38.0f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("Rowan"), 57.5f, 58.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, The resort, 14.61 km"), 14.5899f, 14.6351f);   // slots 323 to 323
	Scatter(S1PLANTS("BlueSpruce"), 57.5f, 58.5f, 8, 5, 1.f, 1.00f, 0.06f, 1, 0.0f);
Zone(TEXT("World s1plants, The resort, 14.97 km"), 14.9518f, 14.9971f);   // slots 331 to 331
	Scatter(S1PLANTS("AlpineRose"), 57.5f, 58.5f, 8, 5, -1.f, 1.60f, 0.10f, -1, 180.0f);   // FIX ROUND: L4: stood on the terrain park of slot 332
	Scatter(S1PLANTS("JuniperCone"), 40.0f, 41.0f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("JuniperHedge"), 51.5f, 52.5f, 8, 5, -1.f, 1.00f, 0.06f, -1, 0.0f);   // FIX ROUND: L4: stood on the terrain park of slot 332
	Scatter(S1PLANTS("JuniperCone"), 38.0f, 39.0f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
Zone(TEXT("World s1plants, The resort, 14.97 km"), 14.9518f, 15.3590f);   // slots 331 to 339
	Scatter(S1PLANTS("JuniperCone"), 57.0f, 58.0f, 8, 5, 1.f, 1.00f, 0.06f, 1, 0.0f);
Zone(TEXT("World s1plants, The resort, 14.97 km"), 14.9518f, 16.8067f);   // slots 331 to 371
	Scatter(S1PLANTS("BlueSpruce"), 54.0f, 55.0f, 24, 21, 1.f, 1.00f, 0.06f, -1, 0.0f);   // FIX ROUND: L4: stood on the terrain park of slot 332; L4: stood on the terrain park of slot 356; slots 347 371 also given up (one rule cannot keep them without the others); now only slots 339 363
Zone(TEXT("World s1plants, The resort, 15.16 km"), 15.1328f, 15.1780f);   // slots 335 to 335
	Scatter(S1PLANTS("JuniperCone"), 49.0f, 50.0f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("RoseBoxes"), 51.0f, 52.0f, 8, 1, 1.f, 1.50f, 0.10f, -1, 0.0f);
	Scatter(S1PLANTS("RoseBoxes"), 22.0f, 22.6f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("JuniperHedge"), 38.5f, 39.5f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("RoseBoxes"), 22.0f, 22.6f, 8, 1, 1.f, 1.00f, 0.06f, 1, 0.0f);
	Scatter(S1PLANTS("BlueSpruce"), 31.5f, 32.5f, 8, 1, 1.f, 1.00f, 0.06f, 1, 0.0f);
	Scatter(S1PLANTS("JuniperCone"), 35.0f, 36.0f, 8, 1, 1.f, 1.00f, 0.06f, 1, 0.0f);
Zone(TEXT("World s1plants, The resort, 15.16 km"), 15.1328f, 15.5399f);   // slots 335 to 343
	Scatter(S1PLANTS("Rowan"), 57.5f, 58.5f, 8, 1, -1.f, 1.00f, 0.12f, 1, 180.0f);   // FIX ROUND: L4: stood on the car park of slot 334; L6: its box was 2.5 m inside P3C_Skaters_A
Zone(TEXT("World s1plants, The resort, 15.16 km"), 15.1328f, 16.9876f);   // slots 335 to 375
	Scatter(S1PLANTS("Rowan"), 54.0f, 55.0f, 24, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);   // FIX ROUND: L4: stood on the terrain park of slot 368; L4: stood on the terrain park of slot 344; slots 351 375 also given up (one rule cannot keep them without the others); now only slots 335 359
Zone(TEXT("World s1plants, The resort, 15.34 km"), 15.3137f, 15.3590f);   // slots 339 to 339
	Scatter(S1PLANTS("AlpineRose"), 42.0f, 43.0f, 8, 5, 1.f, 1.60f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Rowan"), 57.5f, 58.5f, 8, 5, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("JuniperHedge"), 44.5f, 45.5f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("JuniperCone"), 32.0f, 33.0f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("AlpineRose"), 35.0f, 36.0f, 8, 5, -1.f, 1.50f, 0.10f, -1, 180.0f);   // FIX ROUND: L5: stood in S1D6_TurretVilla of slot 338
	Scatter(S1PLANTS("RoseBoxes"), 37.5f, 38.5f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("JuniperHedge"), 22.0f, 22.6f, 8, 5, 1.f, 1.00f, 0.06f, 1, 0.0f);
Zone(TEXT("World s1plants, The resort, 15.34 km"), 15.3137f, 15.7209f);   // slots 339 to 347
	Scatter(S1PLANTS("JuniperHedge"), 22.0f, 22.6f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
Zone(TEXT("World s1plants, The resort, 15.52 km"), 15.4947f, 15.5399f);   // slots 343 to 343
	Scatter(S1PLANTS("JuniperCone"), 42.0f, 43.0f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("BlueSpruce"), 46.0f, 47.0f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("AlpineRose"), 50.0f, 51.0f, 8, 1, -1.f, 1.60f, 0.10f, -1, 180.0f);   // FIX ROUND: L4: stood on the terrain park of slot 344
	Scatter(S1PLANTS("RoseBoxes"), 57.0f, 58.0f, 8, 1, -1.f, 1.50f, 0.10f, -1, 0.0f);   // FIX ROUND: L4: stood on the terrain park of slot 344
	Scatter(S1PLANTS("BlueSpruce"), 37.0f, 38.0f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
Zone(TEXT("World s1plants, The resort, 15.70 km"), 15.6757f, 15.7209f);   // slots 347 to 347
	Scatter(S1PLANTS("JuniperCone"), 57.0f, 58.0f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("BlueSpruce"), 57.5f, 58.5f, 8, 5, 1.f, 1.00f, 0.06f, 1, 0.0f);
Zone(TEXT("World s1plants, The resort, 15.70 km"), 15.6757f, 16.8067f);   // slots 347 to 371
	Scatter(S1PLANTS("AlpineRose"), 49.5f, 50.5f, 17, 11, 1.f, 1.60f, 0.10f, -1, 180.0f);   // FIX ROUND: L4: stood on the terrain park of slot 356; L6: its box was 2.5 m inside P3B_Podium_A; slot 371 also given up (one rule cannot keep them without the others); now only slot 363
Zone(TEXT("World s1plants, The resort, 15.88 km"), 15.8566f, 15.9019f);   // slots 351 to 351
	Scatter(S1PLANTS("BlueSpruce"), 57.5f, 58.5f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("JuniperCone"), 57.0f, 58.0f, 8, 1, 1.f, 1.00f, 0.06f, 1, 0.0f);
Zone(TEXT("World s1plants, The resort, 15.88 km"), 15.8566f, 16.2638f);   // slots 351 to 359
	Scatter(S1PLANTS("JuniperCone"), 49.0f, 50.0f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("RoseBoxes"), 51.0f, 52.0f, 8, 1, 1.f, 1.50f, 0.10f, -1, 0.0f);
Zone(TEXT("World s1plants, The resort, 16.06 km"), 16.0376f, 16.4447f);   // slots 355 to 363
	Scatter(S1PLANTS("Rowan"), 57.5f, 58.5f, 1000, 637, 1.f, 1.00f, 0.12f, -1, 180.0f);   // FIX ROUND: L4: stood on the terrain park of slot 356; now only slot 363
Zone(TEXT("World s1plants, The resort, 16.24 km"), 16.2185f, 16.2638f);   // slots 359 to 359
	Scatter(S1PLANTS("RoseBoxes"), 22.0f, 22.6f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("JuniperHedge"), 38.5f, 39.5f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("RoseBoxes"), 22.0f, 22.6f, 8, 1, 1.f, 1.00f, 0.06f, 1, 0.0f);
	Scatter(S1PLANTS("BlueSpruce"), 31.5f, 32.5f, 8, 1, 1.f, 1.00f, 0.06f, 1, 0.0f);
	Scatter(S1PLANTS("JuniperCone"), 35.0f, 36.0f, 8, 1, 1.f, 1.00f, 0.06f, 1, 0.0f);
Zone(TEXT("World s1plants, The resort, 16.24 km"), 16.2185f, 16.9876f);   // slots 359 to 375
	Scatter(S1PLANTS("Rowan"), 57.5f, 58.5f, 9, 2, 1.f, 1.00f, 0.12f, 1, 180.0f);   // FIX ROUND: L5: stood in the animals of this slot (S1ANIMALS_SleighBlue); slot 375 also given up (one rule cannot keep them without the others); now only slot 367
Zone(TEXT("World s1plants, The resort, 16.42 km"), 16.3995f, 16.4447f);   // slots 363 to 363
	Scatter(S1PLANTS("JuniperCone"), 32.0f, 33.0f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("JuniperHedge"), 22.0f, 22.6f, 8, 5, 1.f, 1.00f, 0.06f, 1, 0.0f);
Zone(TEXT("World s1plants, The resort, 16.42 km"), 16.3995f, 16.8067f);   // slots 363 to 371
	Scatter(S1PLANTS("JuniperHedge"), 22.0f, 22.6f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("BlueSpruce"), 57.5f, 58.5f, 1000, 629, 1.f, 1.00f, 0.06f, 1, 0.0f);   // FIX ROUND: L4: stood on the car park of slot 362; now only slot 371
Zone(TEXT("World s1plants, The resort, 16.60 km"), 16.5805f, 16.6257f);   // slots 367 to 367
	Scatter(S1PLANTS("BlueSpruce"), 46.0f, 47.0f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("AlpineRose"), 50.0f, 51.0f, 8, 1, -1.f, 1.60f, 0.10f, -1, 180.0f);   // FIX ROUND: L4: stood on the terrain park of slot 368
	Scatter(S1PLANTS("RoseBoxes"), 57.0f, 58.0f, 8, 1, -1.f, 1.50f, 0.10f, -1, 0.0f);   // FIX ROUND: L4: stood on the terrain park of slot 368
	Scatter(S1PLANTS("BlueSpruce"), 38.5f, 39.5f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
Zone(TEXT("World s1plants, The resort, 16.60 km"), 16.5805f, 16.9876f);   // slots 367 to 375
	Scatter(S1PLANTS("JuniperCone"), 42.0f, 43.0f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
Zone(TEXT("World s1plants, The resort, 16.78 km"), 16.7614f, 16.8067f);   // slots 371 to 371
	Scatter(S1PLANTS("JuniperCone"), 57.0f, 58.0f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("RoseBoxes"), 38.5f, 39.5f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
Zone(TEXT("World s1plants, The resort, 16.96 km"), 16.9424f, 16.9876f);   // slots 375 to 375
	Scatter(S1PLANTS("BlueSpruce"), 57.5f, 58.5f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("RoseBoxes"), 44.5f, 45.5f, 8, 1, 1.f, 1.50f, 0.10f, -1, 0.0f);
Zone(TEXT("World s1plants, The ice again, 17.15 km"), 17.1233f, 19.7020f);   // slots 379 to 435
	Scatter(S1PLANTS("FrostFern"), 51.0f, 52.0f, 8, 5, 1.f, 1.60f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("GlowMoss"), 56.0f, 57.0f, 16, 13, 1.f, 1.70f, 0.10f, 1, 180.0f);   // FIX ROUND: L5: stood in the animals of this slot (S1ANIMALS_Foxes); slots 395 411 427 also given up (one rule cannot keep them without the others); now only slots 387 403 419 435
Zone(TEXT("World s1plants, The ice again, 17.33 km"), 17.3043f, 19.5211f);   // slots 383 to 431
	Scatter(S1PLANTS("GlowMoss"), 51.0f, 52.0f, 8, 1, 1.f, 1.70f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("IceReed"), 56.0f, 57.0f, 24, 1, 1.f, 1.60f, 0.10f, 1, 180.0f);   // FIX ROUND: L5: stood in the animals of this slot (S1ANIMALS_Seals); L5: stood in the animals of this slot (S1ANIMALS_Foxes); now only slots 383 407 431
Zone(TEXT("World s1plants, The ice again, 17.51 km"), 17.4853f, 17.5305f);   // slots 387 to 387
	Scatter(S1PLANTS("GlowMoss"), 31.5f, 32.5f, 8, 5, 1.f, 1.30f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("GlowMoss"), 31.5f, 32.5f, 8, 5, 1.f, 1.30f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The ice again, 17.87 km"), 17.8472f, 17.8924f);   // slots 395 to 395
	Scatter(S1PLANTS("GlowMoss"), 28.0f, 29.0f, 8, 5, 1.f, 1.30f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The ice again, 18.59 km"), 18.5710f, 18.6163f);   // slots 411 to 411
	Scatter(S1PLANTS("GlowMoss"), 31.5f, 32.5f, 8, 5, 1.f, 1.30f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("GlowMoss"), 31.5f, 32.5f, 8, 5, 1.f, 1.30f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The ice again, 18.96 km"), 18.9329f, 18.9782f);   // slots 419 to 419
	Scatter(S1PLANTS("GlowMoss"), 28.0f, 29.0f, 8, 5, 1.f, 1.30f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The ice again, 19.68 km"), 19.6568f, 19.7020f);   // slots 435 to 435
	Scatter(S1PLANTS("GlowMoss"), 31.5f, 32.5f, 8, 5, 1.f, 1.30f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("GlowMoss"), 31.5f, 32.5f, 8, 5, 1.f, 1.30f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, 19.86 km"), 19.8377f, 19.8830f);   // slots 439 to 439
	Scatter(S1PLANTS("SnowFir"), 54.0f, 55.0f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 49.5f, 50.5f, 8, 1, 1.f, 1.60f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("SnowGrass"), 57.5f, 58.5f, 8, 1, 1.f, 1.70f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 31.5f, 32.5f, 8, 1, 1.f, 1.40f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("SnowFir"), 35.5f, 36.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, 19.86 km"), 19.8377f, 20.2449f);   // slots 439 to 447
	Scatter(S1PLANTS("SnowFir"), 51.5f, 52.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, 20.04 km"), 20.0187f, 20.0639f);   // slots 443 to 443
	Scatter(S1PLANTS("SnowGrass"), 35.5f, 36.5f, 8, 5, 1.f, 1.50f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("FirGrove"), 51.5f, 52.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 20.0f);
	Scatter(S1PLANTS("DwarfPine"), 42.0f, 43.0f, 8, 5, 1.f, 1.70f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("StormFir"), 47.0f, 48.0f, 8, 5, 1.f, 1.00f, 0.12f, 1, 8.0f);
	Scatter(S1PLANTS("SnowGrass"), 30.0f, 31.0f, 8, 5, 1.f, 1.50f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("DwarfPine"), 35.0f, 36.0f, 8, 5, 1.f, 1.50f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, 20.04 km"), 20.0187f, 20.4259f);   // slots 443 to 451
	Scatter(S1PLANTS("FirGrove"), 54.0f, 55.0f, 8, 5, 1.f, 1.00f, 0.08f, -1, 20.0f);
	Scatter(S1PLANTS("DwarfPine"), 49.5f, 50.5f, 8, 5, 1.f, 1.70f, 0.10f, -1, 180.0f);
	Site(TEXT("WldS1PLANTSStormL"), -1, 58.0f, 8, 5, 1.f, 1.00f, 0.12f);
Zone(TEXT("World s1plants, The high ridge, 20.22 km"), 20.1997f, 20.2449f);   // slots 447 to 447
	Scatter(S1PLANTS("SnowFir"), 57.5f, 58.5f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 30.0f, 31.0f, 8, 1, 1.f, 1.40f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 42.0f, 43.0f, 8, 1, 1.f, 1.60f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Heather"), 47.0f, 48.0f, 8, 1, 1.f, 1.80f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 30.0f, 31.0f, 8, 1, 1.f, 1.40f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("SnowFir"), 35.0f, 36.0f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, 20.40 km"), 20.3806f, 20.4259f);   // slots 451 to 451
	Scatter(S1PLANTS("FirGrove"), 54.0f, 55.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 20.0f);
	Scatter(S1PLANTS("DwarfPine"), 47.5f, 48.5f, 8, 5, 1.f, 1.70f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, 20.58 km"), 20.5616f, 20.6068f);   // slots 455 to 455
	Scatter(S1PLANTS("SnowFir"), 54.0f, 55.0f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 49.5f, 50.5f, 8, 1, 1.f, 1.60f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("SnowGrass"), 57.5f, 58.5f, 8, 1, 1.f, 1.70f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, 20.58 km"), 20.5616f, 20.9687f);   // slots 455 to 463
	Scatter(S1PLANTS("SnowFir"), 54.0f, 55.0f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("SnowGrass"), 57.5f, 58.5f, 8, 1, 1.f, 1.70f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, 20.77 km"), 20.7425f, 20.7878f);   // slots 459 to 459
	Scatter(S1PLANTS("FirGrove"), 57.5f, 58.5f, 8, 5, -1.f, 1.00f, 0.08f, -1, 20.0f);   // FIX ROUND: L6: its box was 2.8 m inside P3D_Pylon_A
	Scatter(S1PLANTS("SnowGrass"), 31.5f, 32.5f, 8, 5, 1.f, 1.50f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("SnowGrass"), 31.5f, 32.5f, 8, 5, 1.f, 1.50f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("DwarfPine"), 35.0f, 36.0f, 8, 5, 1.f, 1.50f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, 20.77 km"), 20.7425f, 21.1497f);   // slots 459 to 467
	Scatter(S1PLANTS("FirGrove"), 51.5f, 52.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 20.0f);
Zone(TEXT("World s1plants, The high ridge, 20.95 km"), 20.9235f, 20.9687f);   // slots 463 to 463
	Scatter(S1PLANTS("LichenRock"), 49.5f, 50.5f, 8, 1, 1.f, 1.60f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 43.5f, 44.5f, 8, 1, 1.f, 1.60f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Heather"), 47.5f, 48.5f, 8, 1, 1.f, 1.80f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 31.5f, 32.5f, 8, 1, -1.f, 1.40f, 0.10f, 1, 180.0f);   // FIX ROUND: L6: its box was 3.2 m inside PoleFeather
Zone(TEXT("World s1plants, The high ridge, 20.95 km"), 20.9235f, 21.3307f);   // slots 463 to 471
	Scatter(S1PLANTS("SnowFir"), 51.5f, 52.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, 21.13 km"), 21.1045f, 21.1497f);   // slots 467 to 467
	Scatter(S1PLANTS("FirGrove"), 54.0f, 55.0f, 8, 5, 1.f, 1.00f, 0.08f, -1, 20.0f);
	Scatter(S1PLANTS("DwarfPine"), 42.0f, 43.0f, 8, 5, 1.f, 1.70f, 0.10f, -1, 180.0f);
	Site(TEXT("WldS1PLANTSStormL"), -1, 47.5f, 8, 5, 1.f, 1.00f, 0.12f);
	Scatter(S1PLANTS("SnowGrass"), 35.5f, 36.5f, 8, 5, 1.f, 1.50f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("DwarfPine"), 38.5f, 39.5f, 8, 5, 1.f, 1.50f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("DwarfPine"), 42.0f, 43.0f, 8, 5, 1.f, 1.70f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("StormFir"), 47.0f, 48.0f, 8, 5, 1.f, 1.00f, 0.12f, 1, 8.0f);
	Scatter(S1PLANTS("SnowGrass"), 37.0f, 38.0f, 8, 5, 1.f, 1.50f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, 21.31 km"), 21.2854f, 21.3307f);   // slots 471 to 471
	Scatter(S1PLANTS("SnowFir"), 57.5f, 58.5f, 8, 1, -1.f, 1.00f, 0.12f, -1, 180.0f);   // FIX ROUND: L6: its box was 3.3 m inside Ibex_A
	Scatter(S1PLANTS("LichenRock"), 30.0f, 31.0f, 8, 1, 1.f, 1.40f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 47.5f, 48.5f, 8, 1, 1.f, 1.60f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 30.0f, 31.0f, 8, 1, 1.f, 1.40f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("SnowFir"), 35.0f, 36.0f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, 21.49 km"), 21.4664f, 21.5116f);   // slots 475 to 475
	Scatter(S1PLANTS("FirGrove"), 54.0f, 55.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 20.0f);
	Scatter(S1PLANTS("DwarfPine"), 49.5f, 50.5f, 8, 5, -1.f, 1.70f, 0.10f, 1, 180.0f);   // FIX ROUND: L6: its box was 2.1 m inside P3B_SnowFort_A
Zone(TEXT("World s1plants, The high ridge, 21.67 km"), 21.6473f, 21.6926f);   // slots 479 to 479
	Scatter(S1PLANTS("SnowFir"), 54.0f, 55.0f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 49.5f, 50.5f, 8, 1, 1.f, 1.60f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("SnowGrass"), 57.5f, 58.5f, 8, 1, 1.f, 1.70f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, 21.67 km"), 21.6473f, 22.0545f);   // slots 479 to 487
	Scatter(S1PLANTS("SnowFir"), 54.0f, 55.0f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 49.5f, 50.5f, 8, 1, 1.f, 1.60f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("SnowGrass"), 57.5f, 58.5f, 8, 1, 1.f, 1.70f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, 21.85 km"), 21.8283f, 21.8735f);   // slots 483 to 483
	Scatter(S1PLANTS("FirGrove"), 57.5f, 58.5f, 8, 5, 1.f, 1.00f, 0.08f, -1, 20.0f);
	Scatter(S1PLANTS("SnowGrass"), 31.5f, 32.5f, 8, 5, 1.f, 1.50f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("DwarfPine"), 35.0f, 36.0f, 8, 5, 1.f, 1.50f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("DwarfPine"), 44.0f, 45.0f, 8, 5, -1.f, 1.70f, 0.10f, 1, 180.0f);   // FIX ROUND: L6: its box was 2.6 m inside P3C_SkiTrail_A
	Scatter(S1PLANTS("StormFir"), 47.5f, 48.5f, 8, 5, 1.f, 1.00f, 0.12f, 1, 8.0f);
Zone(TEXT("World s1plants, The high ridge, 21.85 km"), 21.8283f, 22.2355f);   // slots 483 to 491
	Scatter(S1PLANTS("FirGrove"), 51.5f, 52.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 20.0f);
	Scatter(S1PLANTS("SnowGrass"), 31.5f, 32.5f, 1000, 517, 1.f, 1.50f, 0.10f, 1, 180.0f);   // FIX ROUND: L6: its box was 2.4 m inside PoleFeather; now only slot 483
Zone(TEXT("World s1plants, The high ridge, 22.03 km"), 22.0093f, 22.0545f);   // slots 487 to 487
	Scatter(S1PLANTS("SnowFir"), 44.5f, 45.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 31.5f, 32.5f, 8, 1, -1.f, 1.40f, 0.10f, 1, 180.0f);   // FIX ROUND: L6: its box was 3.8 m inside PoleFeather
Zone(TEXT("World s1plants, The high ridge, 22.21 km"), 22.1902f, 22.2355f);   // slots 491 to 491
	Scatter(S1PLANTS("FirGrove"), 54.0f, 55.0f, 8, 5, 1.f, 1.00f, 0.08f, -1, 20.0f);
	Scatter(S1PLANTS("DwarfPine"), 42.0f, 43.0f, 8, 5, 1.f, 1.70f, 0.10f, -1, 180.0f);
	Site(TEXT("WldS1PLANTSStormL"), -1, 47.5f, 8, 5, 1.f, 1.00f, 0.12f);
	Scatter(S1PLANTS("SnowGrass"), 35.5f, 36.5f, 8, 5, 1.f, 1.50f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("DwarfPine"), 38.5f, 39.5f, 8, 5, 1.f, 1.50f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("DwarfPine"), 47.5f, 48.5f, 8, 5, 1.f, 1.70f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("DwarfPine"), 35.0f, 36.0f, 8, 5, 1.f, 1.50f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, 22.39 km"), 22.3712f, 22.4164f);   // slots 495 to 495
	Scatter(S1PLANTS("SnowFir"), 57.5f, 58.5f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 42.0f, 43.0f, 8, 1, 1.f, 1.60f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 30.0f, 31.0f, 8, 1, 1.f, 1.40f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("SnowFir"), 35.0f, 36.0f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("SnowFir"), 51.5f, 52.5f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 42.0f, 43.0f, 8, 1, 1.f, 1.60f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Heather"), 47.0f, 48.0f, 8, 1, 1.f, 1.80f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("LichenRock"), 30.0f, 31.0f, 8, 1, 1.f, 1.40f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("SnowFir"), 35.0f, 36.0f, 8, 1, 1.f, 1.00f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, the last lifts, 22.57 km"), 22.5521f, 22.5974f);   // slots 499 to 499
	Site(TEXT("WldS1PLANTSStormL"), -1, 54.5f, 8, 5, -1.f, 1.00f, 0.12f);   // FIX ROUND: L4: stood on the terrain park of slot 500
	Scatter(S1PLANTS("Heather"), 57.5f, 58.5f, 8, 5, -1.f, 1.80f, 0.10f, -1, 180.0f);   // FIX ROUND: L4: stood on the terrain park of slot 500
	Scatter(S1PLANTS("StormFir"), 57.5f, 58.5f, 8, 5, 1.f, 1.00f, 0.12f, 1, 8.0f);
Zone(TEXT("World s1plants, The high ridge, the last lifts, 22.76 km"), 22.7331f, 22.7783f);   // slots 503 to 503
	Scatter(S1PLANTS("JuniperCone"), 57.0f, 58.0f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
Zone(TEXT("World s1plants, The high ridge, the last lifts, 22.76 km"), 22.7331f, 23.5022f);   // slots 503 to 519
	Scatter(S1PLANTS("FirGrove"), 54.0f, 55.0f, 16, 9, 1.f, 1.00f, 0.08f, -1, 20.0f);   // FIX ROUND: L4: stood on the terrain park of slot 512; now only slots 503 519
Zone(TEXT("World s1plants, The high ridge, the last lifts, 22.94 km"), 22.9141f, 22.9593f);   // slots 507 to 507
	Site(TEXT("WldS1PLANTSStormL"), -1, 55.5f, 8, 5, 1.f, 1.00f, 0.12f);
	Scatter(S1PLANTS("JuniperCone"), 57.5f, 58.5f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("JuniperCone"), 32.0f, 33.0f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("SnowGrass"), 35.0f, 36.0f, 8, 5, 1.f, 1.50f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, the last lifts, 22.94 km"), 22.9141f, 23.3212f);   // slots 507 to 515
	Scatter(S1PLANTS("JuniperHedge"), 22.0f, 22.6f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("JuniperHedge"), 22.0f, 22.6f, 8, 5, 1.f, 1.00f, 0.06f, 1, 0.0f);
Zone(TEXT("World s1plants, The high ridge, the last lifts, 23.12 km"), 23.0950f, 23.1403f);   // slots 511 to 511
	Scatter(S1PLANTS("DwarfPine"), 47.0f, 48.0f, 8, 1, -1.f, 1.70f, 0.10f, -1, 180.0f);   // FIX ROUND: L4: stood on the terrain park of slot 512
	Scatter(S1PLANTS("JuniperHedge"), 50.0f, 51.0f, 8, 1, -1.f, 1.00f, 0.06f, -1, 0.0f);   // FIX ROUND: L4: stood on the terrain park of slot 512
	Scatter(S1PLANTS("SnowFir"), 38.5f, 39.5f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("JuniperCone"), 57.0f, 58.0f, 8, 1, 1.f, 1.00f, 0.06f, 1, 0.0f);
Zone(TEXT("World s1plants, The high ridge, the last lifts, 23.12 km"), 23.0950f, 23.5022f);   // slots 511 to 519
	Scatter(S1PLANTS("JuniperCone"), 42.0f, 43.0f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
Zone(TEXT("World s1plants, The high ridge, the last lifts, 23.30 km"), 23.2760f, 23.3212f);   // slots 515 to 515
	Scatter(S1PLANTS("Heather"), 49.5f, 50.5f, 8, 5, 1.f, 1.80f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("SnowFir"), 57.5f, 58.5f, 8, 5, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("JuniperCone"), 31.0f, 32.0f, 8, 5, 1.f, 1.00f, 0.06f, 1, 0.0f);
	Scatter(S1PLANTS("SnowGrass"), 35.0f, 36.0f, 8, 5, 1.f, 1.50f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, the last lifts, 23.30 km"), 23.2760f, 23.6831f);   // slots 515 to 523
	Site(TEXT("WldS1PLANTSStormL"), -1, 54.5f, 1000, 485, 1.f, 1.00f, 0.12f);   // FIX ROUND: L4: stood on the terrain park of slot 524; now only slot 515
Zone(TEXT("World s1plants, The high ridge, the last lifts, 23.48 km"), 23.4569f, 23.5022f);   // slots 519 to 519
	Scatter(S1PLANTS("JuniperHedge"), 44.5f, 45.5f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("SnowGrass"), 22.5f, 23.1f, 8, 1, 1.f, 1.00f, 0.06f, -1, 8.0f);
	Scatter(S1PLANTS("SnowFir"), 37.0f, 38.0f, 8, 1, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("FirGrove"), 57.5f, 58.5f, 8, 1, -1.f, 1.00f, 0.08f, 1, 20.0f);   // FIX ROUND: L6: its box was 3.5 m inside P3C_Skaters_A
Zone(TEXT("World s1plants, The high ridge, the last lifts, 23.66 km"), 23.6379f, 23.6831f);   // slots 523 to 523
	Scatter(S1PLANTS("Heather"), 42.0f, 43.0f, 8, 5, 1.f, 1.80f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("SnowFir"), 47.0f, 48.0f, 8, 5, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("JuniperCone"), 57.5f, 58.5f, 8, 5, -1.f, 1.00f, 0.06f, -1, 0.0f);   // FIX ROUND: L4: stood on the terrain park of slot 524
	Scatter(S1PLANTS("JuniperCone"), 36.5f, 37.5f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
Zone(TEXT("World s1plants, The high ridge, the last lifts, 23.66 km"), 23.6379f, 24.0451f);   // slots 523 to 531
	Scatter(S1PLANTS("StormFir"), 57.5f, 58.5f, 8, 5, 1.f, 1.00f, 0.12f, 1, 8.0f);
Zone(TEXT("World s1plants, The high ridge, the last lifts, 24.02 km"), 23.9998f, 24.0451f);   // slots 531 to 531
	Site(TEXT("WldS1PLANTSStormL"), -1, 58.0f, 8, 5, 1.f, 1.00f, 0.12f);
Zone(TEXT("World s1plants, The high ridge, the last lifts, 24.20 km"), 24.1808f, 24.2260f);   // slots 535 to 535
	Scatter(S1PLANTS("JuniperCone"), 43.0f, 44.0f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("DwarfPine"), 47.0f, 48.0f, 8, 1, 1.f, 1.70f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("JuniperHedge"), 50.0f, 51.0f, 8, 1, -1.f, 1.00f, 0.06f, -1, 0.0f);   // FIX ROUND: L4: stood on the terrain park of slot 536
	Scatter(S1PLANTS("SnowFir"), 31.5f, 32.5f, 8, 1, -1.f, 1.00f, 0.12f, 1, 180.0f);   // FIX ROUND: L6: its box was 2.1 m inside PoleFeather
	Scatter(S1PLANTS("JuniperHedge"), 35.0f, 36.0f, 8, 1, 1.f, 1.00f, 0.06f, 1, 0.0f);
Zone(TEXT("World s1plants, The high ridge, the last lifts, 24.20 km"), 24.1808f, 24.5879f);   // slots 535 to 543
	Scatter(S1PLANTS("FirGrove"), 54.0f, 55.0f, 1000, 457, 1.f, 1.00f, 0.08f, -1, 20.0f);   // FIX ROUND: L4: stood on the terrain park of slot 536; now only slot 543
Zone(TEXT("World s1plants, The high ridge, the last lifts, 24.38 km"), 24.3617f, 24.4070f);   // slots 539 to 539
	Scatter(S1PLANTS("Heather"), 49.5f, 50.5f, 8, 5, -1.f, 1.80f, 0.10f, -1, 180.0f);   // FIX ROUND: L6: its box was 2.9 m inside P3B_Wedding_A
	Scatter(S1PLANTS("SnowFir"), 57.5f, 58.5f, 8, 5, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("JuniperHedge"), 22.0f, 22.6f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("JuniperHedge"), 22.0f, 22.6f, 8, 5, 1.f, 1.00f, 0.06f, 1, 0.0f);
	Scatter(S1PLANTS("JuniperCone"), 31.0f, 32.0f, 8, 5, 1.f, 1.00f, 0.06f, 1, 0.0f);
	Scatter(S1PLANTS("SnowGrass"), 35.0f, 36.0f, 8, 5, 1.f, 1.50f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, the last lifts, 24.38 km"), 24.3617f, 24.7689f);   // slots 539 to 547
	Site(TEXT("WldS1PLANTSStormL"), -1, 54.5f, 8, 5, 1.f, 1.00f, 0.12f);
Zone(TEXT("World s1plants, The high ridge, the last lifts, 24.57 km"), 24.5427f, 24.5879f);   // slots 543 to 543
	Scatter(S1PLANTS("JuniperCone"), 49.0f, 50.0f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("JuniperHedge"), 51.0f, 52.0f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("JuniperHedge"), 33.0f, 34.0f, 8, 1, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("FirGrove"), 57.5f, 58.5f, 8, 1, -1.f, 1.00f, 0.08f, 1, 20.0f);   // FIX ROUND: L4: stood on the car park of slot 542
	Scatter(S1PLANTS("SnowGrass"), 22.5f, 23.1f, 8, 1, 1.f, 1.00f, 0.06f, 1, 8.0f);
	Scatter(S1PLANTS("JuniperHedge"), 28.5f, 29.5f, 8, 1, 1.f, 1.00f, 0.06f, 1, 0.0f);
	Scatter(S1PLANTS("DwarfPine"), 26.0f, 27.0f, 8, 1, 1.f, 1.50f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, The high ridge, the last lifts, 24.75 km"), 24.7237f, 24.7689f);   // slots 547 to 547
	Scatter(S1PLANTS("Heather"), 42.0f, 43.0f, 8, 5, 1.f, 1.80f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("SnowFir"), 47.0f, 48.0f, 8, 5, 1.f, 1.00f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("JuniperCone"), 57.5f, 58.5f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("JuniperCone"), 38.0f, 39.0f, 8, 5, 1.f, 1.00f, 0.06f, -1, 0.0f);
	Scatter(S1PLANTS("JuniperCone"), 57.0f, 58.0f, 8, 5, 1.f, 1.00f, 0.06f, 1, 0.0f);
#undef S1PLANTS
// ---- END s1plants ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People5 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1animals (the animals of all eight districts, ft_world_ice_s1animals.py) ----
// 18 meshes. Estimate: ground 307 Scatter lines (one piece each, chance 1) x 0.75 = 230; sky 65 Far pieces x 1 = 65; about 295 placed.
// Written (before the 0.75): Chamois 40, Foxes 25, Goats 4, Hares 28, Kennel 8, Marmots 29, Penguins 25, Ptarmigan 20, Reindeer 30, Seals 25, SkyBuntings 12, SkyGulls 20, SkyOwl 9, SkySwans 24, SleighBlue 4, SleighPink 4, Swans 40, YakTrain 25.
// Bands and slots used: ground 31.5 to 45 m on s % 8 == 5; 46 to 68 m on s % 4 == 1 (61 to 68 m also s % 4 == 3);
// 72 to 112 m on s % 4 == 1 and 3 (Chamois, YakTrain, Reindeer, Swans: those four never nearer than 72 m, the others never
// past 68 m, so no mesh stands on both sides of the 70 m shadow line). One Zone a slot (as the p3f block): every line was
// tried against both tables by a scratch tool (C:/ftwork/jobs/L5s1animals/plan.py: same slot and side, 5 m clear of every
// Scatter band and Site part, 3 m of a banner mast) and left out of loops, corkscrews, jumps, pipes, gondolas and streets.
// Outside the dressing's verge zones (banked or not known) nothing nearer than 40.5 m. Sky: Far, straight lines along the road.
#define S1ANIMALS(n) TEXT("/Game/World/IceMountain/SM_S1ANIMALS_" n)
Zone(TEXT("World s1animals, slot 1"), 0.02307f, 0.06741f);
	Scatter(S1ANIMALS("Hares"), 63.5f, 64.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Kennel"), 50.5f, 51.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 9"), 0.38499f, 0.42933f);
	Scatter(S1ANIMALS("Hares"), 60.5f, 61.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 11"), 0.47547f, 0.51981f);
	Scatter(S1ANIMALS("Kennel"), 63.5f, 64.5f, 1, 0, 1.f, 1.2f, 0.f, 1, 6.0f);
Zone(TEXT("World s1animals, slot 13"), 0.56595f, 0.61029f);
	Scatter(S1ANIMALS("Hares"), 65.5f, 66.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 15"), 0.65643f, 0.70077f);
	Scatter(S1ANIMALS("Kennel"), 65.5f, 66.5f, 1, 0, 1.f, 1.2f, 0.f, 1, 6.0f);
Zone(TEXT("World s1animals, slot 17"), 0.74691f, 0.79125f);
	Scatter(S1ANIMALS("Hares"), 63.5f, 64.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 21"), 0.92787f, 0.97221f);
	Scatter(S1ANIMALS("Kennel"), 52.5f, 53.5f, 1, 0, 1.f, 1.2f, 0.f, 1, 6.0f);
Zone(TEXT("World s1animals, slot 23"), 1.01835f, 1.06269f);
	Scatter(S1ANIMALS("Hares"), 63.5f, 64.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
Zone(TEXT("World s1animals, slot 25"), 1.10883f, 1.15317f);
	Scatter(S1ANIMALS("Kennel"), 65.5f, 66.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 39"), 1.74219f, 1.78653f);
	Scatter(S1ANIMALS("Hares"), 65.5f, 66.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
Zone(TEXT("World s1animals, slot 41"), 1.83267f, 1.87701f);
	Scatter(S1ANIMALS("Kennel"), 62.0f, 63.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Hares"), 63.5f, 64.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 43"), 1.92315f, 1.96749f);
	Scatter(S1ANIMALS("Kennel"), 61.0f, 62.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 6.0f);
Zone(TEXT("World s1animals, slot 45"), 2.01363f, 2.05797f);
	Scatter(S1ANIMALS("Hares"), 65.5f, 66.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Kennel"), 65.5f, 66.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 61"), 2.73747f, 2.78181f);
	Scatter(S1ANIMALS("Ptarmigan"), 48.0f, 49.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 90.0f, 92.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Ptarmigan"), 48.0f, 49.0f, 1, 0, -1.f, 1.3f, 0.f, -1, 10.0f);   // FIX ROUND: L2: stood in the rock cutting
	Scatter(S1ANIMALS("Chamois"), 77.0f, 79.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 63"), 2.82795f, 2.87229f);
	Scatter(S1ANIMALS("Ptarmigan"), 61.0f, 62.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 72.0f, 74.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Chamois"), 90.0f, 92.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 65"), 2.91843f, 2.96277f);
	Scatter(S1ANIMALS("Ptarmigan"), 48.0f, 49.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Ptarmigan"), 48.0f, 49.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 81"), 3.64227f, 3.68661f);
	Scatter(S1ANIMALS("Ptarmigan"), 54.5f, 55.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 74.0f, 76.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Ptarmigan"), 63.5f, 64.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 72.0f, 74.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 83"), 3.73275f, 3.77709f);
	Scatter(S1ANIMALS("Ptarmigan"), 61.0f, 62.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Ptarmigan"), 62.5f, 63.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 72.0f, 74.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 85"), 3.82323f, 3.86757f);
	Scatter(S1ANIMALS("Ptarmigan"), 54.5f, 55.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 72.0f, 74.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Ptarmigan"), 64.5f, 65.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 72.0f, 74.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 87"), 3.91371f, 3.95805f);
	Scatter(S1ANIMALS("Ptarmigan"), 61.0f, 62.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 72.0f, 74.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Chamois"), 74.0f, 76.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 89"), 4.00419f, 4.04853f);
	Scatter(S1ANIMALS("Chamois"), 74.0f, 76.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
Zone(TEXT("World s1animals, slot 99"), 4.45659f, 4.50093f);
	Scatter(S1ANIMALS("Ptarmigan"), 61.0f, 62.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 72.0f, 74.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Chamois"), 87.0f, 89.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 101"), 4.54707f, 4.59141f);
	Scatter(S1ANIMALS("Ptarmigan"), 50.5f, 51.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 74.0f, 76.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Ptarmigan"), 52.5f, 53.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 77.0f, 79.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 103"), 4.63755f, 4.68189f);
	Scatter(S1ANIMALS("Chamois"), 94.0f, 96.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Chamois"), 96.0f, 98.0f, 1, 0, -1.f, 1.2f, 0.f, -1, 8.0f);   // FIX ROUND: L6: stood on the strip of GroomerRun
Zone(TEXT("World s1animals, slot 105"), 4.72803f, 4.77237f);
	Scatter(S1ANIMALS("Ptarmigan"), 52.5f, 53.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 74.0f, 76.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Ptarmigan"), 49.5f, 50.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);   // FIX ROUND: L5: 3.5 m further out, clear of the people decks at 42.6 m
	Scatter(S1ANIMALS("Chamois"), 87.0f, 89.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 121"), 5.45187f, 5.49621f);
	Scatter(S1ANIMALS("Ptarmigan"), 50.5f, 51.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 74.0f, 76.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Ptarmigan"), 52.5f, 53.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 94.0f, 96.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 123"), 5.54235f, 5.58669f);
	Scatter(S1ANIMALS("Ptarmigan"), 61.0f, 62.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 72.0f, 74.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Chamois"), 75.0f, 77.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 125"), 5.63283f, 5.67717f);
	Scatter(S1ANIMALS("Goats"), 66.0f, 67.0f, 1, 0, 1.f, 1.15f, 0.f, 1, 5.0f);
	Scatter(S1ANIMALS("Swans"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Swans"), 77.0f, 79.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 139"), 6.26619f, 6.31053f);
	Scatter(S1ANIMALS("Swans"), 75.0f, 77.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 141"), 6.35667f, 6.40101f);
	Scatter(S1ANIMALS("SleighPink"), 65.5f, 66.5f, 1, 0, 1.f, 1.15f, 0.f, 1, 5.0f);
	Scatter(S1ANIMALS("Swans"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Goats"), 51.0f, 52.0f, 1, 0, 1.f, 1.15f, 0.f, -1, 5.0f);
	Scatter(S1ANIMALS("Swans"), 77.0f, 79.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 143"), 6.44715f, 6.49149f);
	Scatter(S1ANIMALS("SleighPink"), 63.5f, 64.5f, 1, 0, 1.f, 1.15f, 0.f, 1, 5.0f);
	Scatter(S1ANIMALS("Swans"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
Zone(TEXT("World s1animals, slot 145"), 6.53763f, 6.58197f);
	Scatter(S1ANIMALS("Swans"), 74.0f, 76.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Swans"), 76.0f, 78.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 147"), 6.62811f, 6.67245f);
	Scatter(S1ANIMALS("Goats"), 66.0f, 67.0f, 1, 0, 1.f, 1.15f, 0.f, 1, 5.0f);
	Scatter(S1ANIMALS("Swans"), 75.0f, 77.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 149"), 6.71859f, 6.76293f);
	Scatter(S1ANIMALS("SleighPink"), 54.5f, 55.5f, 1, 0, 1.f, 1.15f, 0.f, 1, 5.0f);
	Scatter(S1ANIMALS("Swans"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Swans"), 78.0f, 80.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 151"), 6.80907f, 6.85341f);
	Scatter(S1ANIMALS("Swans"), 74.0f, 76.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
Zone(TEXT("World s1animals, slot 167"), 7.53291f, 7.57725f);
	Scatter(S1ANIMALS("Swans"), 78.0f, 80.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Swans"), 76.0f, 78.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 169"), 7.62339f, 7.66773f);
	Scatter(S1ANIMALS("Goats"), 51.0f, 52.0f, 1, 0, 1.f, 1.15f, 0.f, -1, 5.0f);
Zone(TEXT("World s1animals, slot 171"), 7.71387f, 7.75821f);
	Scatter(S1ANIMALS("Swans"), 75.0f, 77.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 173"), 7.80435f, 7.84869f);
	Scatter(S1ANIMALS("Swans"), 78.0f, 80.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
Zone(TEXT("World s1animals, slot 175"), 7.89483f, 7.93917f);
	Scatter(S1ANIMALS("Swans"), 78.0f, 80.0f, 1, 0, -1.f, 1.3f, 0.f, 1, 8.0f);   // FIX ROUND: L4: stood on the car park of slot 174
Zone(TEXT("World s1animals, slot 195"), 8.79963f, 8.84397f);
	Scatter(S1ANIMALS("SleighPink"), 65.5f, 66.5f, 1, 0, 1.f, 1.15f, 0.f, 1, 5.0f);
	Scatter(S1ANIMALS("Swans"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
Zone(TEXT("World s1animals, slot 197"), 8.89011f, 8.93445f);
	Scatter(S1ANIMALS("Swans"), 74.0f, 76.0f, 1, 0, -1.f, 1.3f, 0.f, 1, 8.0f);   // FIX ROUND: L6: stood on the strip of GroomerRun
	Scatter(S1ANIMALS("Swans"), 77.0f, 79.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 199"), 8.98059f, 9.02493f);
	Scatter(S1ANIMALS("Seals"), 61.0f, 62.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Penguins"), 64.5f, 65.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 201"), 9.07107f, 9.11541f);
	Scatter(S1ANIMALS("Reindeer"), 73.5f, 75.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Seals"), 65.0f, 66.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 205"), 9.25203f, 9.29637f);
	Scatter(S1ANIMALS("Penguins"), 47.5f, 48.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Seals"), 66.0f, 67.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 207"), 9.34251f, 9.38685f);
	Scatter(S1ANIMALS("Penguins"), 61.0f, 62.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Seals"), 65.0f, 66.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 209"), 9.43299f, 9.47733f);
	Scatter(S1ANIMALS("Penguins"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 211"), 9.52347f, 9.56781f);
	Scatter(S1ANIMALS("Seals"), 61.0f, 62.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Reindeer"), 73.5f, 75.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 213"), 9.61395f, 9.65829f);
	Scatter(S1ANIMALS("Penguins"), 55.5f, 56.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Penguins"), 44.0f, 45.0f, 1, 0, 1.f, 1.0f, 0.f, -1, 0.0f);
	Scatter(S1ANIMALS("Seals"), 66.0f, 67.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 217"), 9.79491f, 9.83925f);
	Scatter(S1ANIMALS("Seals"), 48.0f, 49.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Reindeer"), 73.5f, 75.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 219"), 9.88539f, 9.92973f);
	Scatter(S1ANIMALS("Penguins"), 61.0f, 62.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Seals"), 64.0f, 65.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 221"), 9.97587f, 10.02021f);
	Scatter(S1ANIMALS("Penguins"), 44.0f, 45.0f, 1, 0, 1.f, 1.0f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Seals"), 46.0f, 47.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Penguins"), 40.5f, 41.5f, 1, 0, 1.f, 1.0f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 223"), 10.06635f, 10.11069f);
	Scatter(S1ANIMALS("Seals"), 61.0f, 62.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Penguins"), 64.5f, 65.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 225"), 10.15683f, 10.20117f);
	Scatter(S1ANIMALS("Seals"), 46.0f, 47.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Reindeer"), 73.5f, 75.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Penguins"), 64.5f, 65.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 229"), 10.33779f, 10.38213f);
	Scatter(S1ANIMALS("Seals"), 48.0f, 49.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Penguins"), 65.5f, 66.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 231"), 10.42827f, 10.47261f);
	Scatter(S1ANIMALS("Reindeer"), 73.5f, 75.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Seals"), 65.0f, 66.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 233"), 10.51875f, 10.56309f);
	Scatter(S1ANIMALS("Penguins"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 235"), 10.60923f, 10.65357f);
	Scatter(S1ANIMALS("Seals"), 61.0f, 62.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Penguins"), 63.5f, 64.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 237"), 10.69971f, 10.74405f);
	Scatter(S1ANIMALS("Seals"), 56.0f, 57.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Penguins"), 44.0f, 45.0f, 1, 0, 1.f, 1.0f, 0.f, -1, 0.0f);
	Scatter(S1ANIMALS("Seals"), 66.0f, 67.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 241"), 10.88067f, 10.92501f);
	Scatter(S1ANIMALS("Penguins"), 47.5f, 48.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Reindeer"), 73.5f, 75.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 243"), 10.97115f, 11.01549f);
	Scatter(S1ANIMALS("Seals"), 61.0f, 62.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Penguins"), 63.5f, 64.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 245"), 11.06163f, 11.10597f);
	Scatter(S1ANIMALS("Penguins"), 44.0f, 45.0f, 1, 0, 1.f, 1.0f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Seals"), 46.0f, 47.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Penguins"), 40.5f, 41.5f, 1, 0, 1.f, 1.0f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 247"), 11.15211f, 11.19645f);
	Scatter(S1ANIMALS("Seals"), 61.0f, 62.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Reindeer"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Reindeer"), 73.5f, 75.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 6.0f);
Zone(TEXT("World s1animals, slot 249"), 11.24259f, 11.28693f);
	Scatter(S1ANIMALS("Seals"), 56.0f, 57.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Penguins"), 64.5f, 65.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 253"), 11.42355f, 11.46789f);
	Scatter(S1ANIMALS("Seals"), 48.0f, 49.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Penguins"), 65.5f, 66.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 255"), 11.51403f, 11.55837f);
	Scatter(S1ANIMALS("Seals"), 65.0f, 66.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 257"), 11.60451f, 11.64885f);
	Scatter(S1ANIMALS("YakTrain"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Marmots"), 52.5f, 53.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 261"), 11.78547f, 11.82981f);
	Scatter(S1ANIMALS("YakTrain"), 73.0f, 75.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Marmots"), 40.5f, 41.5f, 1, 0, 1.f, 1.0f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Marmots"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("YakTrain"), 76.0f, 78.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 265"), 11.96643f, 12.01077f);
	Scatter(S1ANIMALS("Marmots"), 49.5f, 50.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);   // FIX ROUND: L5: 3.5 m further out, clear of the people decks at 42.6 m
	Scatter(S1ANIMALS("YakTrain"), 75.0f, 77.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 269"), 12.14739f, 12.19173f);
	Scatter(S1ANIMALS("YakTrain"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Marmots"), 42.0f, 43.0f, 1, 0, 1.f, 1.0f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Marmots"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("YakTrain"), 76.0f, 78.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 273"), 12.32835f, 12.37269f);
	Scatter(S1ANIMALS("Marmots"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("YakTrain"), 73.0f, 75.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Marmots"), 52.5f, 53.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("YakTrain"), 75.0f, 77.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 277"), 12.50931f, 12.55365f);
	Scatter(S1ANIMALS("Marmots"), 40.5f, 41.5f, 1, 0, 1.f, 1.0f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Marmots"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("YakTrain"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("YakTrain"), 76.0f, 78.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 281"), 12.69027f, 12.73461f);
	Scatter(S1ANIMALS("YakTrain"), 73.0f, 75.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Marmots"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 285"), 12.87123f, 12.91557f);
	Scatter(S1ANIMALS("Marmots"), 40.5f, 41.5f, 1, 0, 1.f, 1.0f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Marmots"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Marmots"), 40.5f, 41.5f, 1, 0, 1.f, 1.0f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Marmots"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("YakTrain"), 76.0f, 78.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 289"), 13.05219f, 13.09653f);
	Scatter(S1ANIMALS("YakTrain"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("YakTrain"), 75.0f, 77.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 293"), 13.23315f, 13.27749f);
	Scatter(S1ANIMALS("Marmots"), 40.5f, 41.5f, 1, 0, 1.f, 1.0f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Marmots"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("YakTrain"), 73.0f, 75.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("YakTrain"), 76.0f, 78.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 297"), 13.41411f, 13.45845f);
	Scatter(S1ANIMALS("Marmots"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("YakTrain"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Marmots"), 49.5f, 50.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);   // FIX ROUND: L5: 3.5 m further out, clear of the people decks at 42.6 m
	Scatter(S1ANIMALS("YakTrain"), 75.0f, 77.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 301"), 13.59507f, 13.63941f);
	Scatter(S1ANIMALS("YakTrain"), 73.0f, 75.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("YakTrain"), 76.0f, 78.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 305"), 13.77603f, 13.82037f);
	Scatter(S1ANIMALS("Marmots"), 49.5f, 50.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);   // FIX ROUND: L5: 3.5 m further out, clear of the people decks at 42.6 m
	Scatter(S1ANIMALS("Marmots"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("YakTrain"), 75.0f, 77.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 309"), 13.95699f, 14.00133f);
	Scatter(S1ANIMALS("YakTrain"), 72.0f, 74.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Marmots"), 40.5f, 41.5f, 1, 0, 1.f, 1.0f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Marmots"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("YakTrain"), 76.0f, 78.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 313"), 14.13795f, 14.18229f);
	Scatter(S1ANIMALS("Marmots"), 49.5f, 50.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);   // FIX ROUND: L5: 3.5 m further out, clear of the people decks at 42.6 m
	Scatter(S1ANIMALS("YakTrain"), 73.0f, 75.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("YakTrain"), 75.0f, 77.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 325"), 14.68083f, 14.72517f);
	Scatter(S1ANIMALS("SleighBlue"), 40.5f, 41.5f, 1, 0, 1.f, 1.0f, 0.f, 1, 5.0f);
	Scatter(S1ANIMALS("Swans"), 73.0f, 75.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Swans"), 77.0f, 79.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 327"), 14.77131f, 14.81565f);
	Scatter(S1ANIMALS("Swans"), 80.0f, 82.0f, 1, 0, -1.f, 1.3f, 0.f, 1, 8.0f);   // FIX ROUND: L6: its box was 3.5 m inside ShedGroomer
	Scatter(S1ANIMALS("Swans"), 76.0f, 78.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 329"), 14.86179f, 14.90613f);
	Scatter(S1ANIMALS("Swans"), 73.0f, 75.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
Zone(TEXT("World s1animals, slot 331"), 14.95227f, 14.99661f);
	Scatter(S1ANIMALS("Swans"), 75.0f, 77.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 335"), 15.13323f, 15.17757f);
	Scatter(S1ANIMALS("Swans"), 80.0f, 82.0f, 1, 0, -1.f, 1.3f, 0.f, 1, 8.0f);   // FIX ROUND: L4: stood on the car park of slot 334
Zone(TEXT("World s1animals, slot 337"), 15.22371f, 15.26805f);
	Scatter(S1ANIMALS("SleighBlue"), 46.5f, 47.5f, 1, 0, 1.f, 1.15f, 0.f, -1, 5.0f);
	Scatter(S1ANIMALS("Swans"), 76.0f, 78.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 339"), 15.31419f, 15.35853f);
	Scatter(S1ANIMALS("Swans"), 73.0f, 75.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
Zone(TEXT("World s1animals, slot 349"), 15.76659f, 15.81093f);
	Scatter(S1ANIMALS("Swans"), 77.0f, 79.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 351"), 15.85707f, 15.90141f);
	Scatter(S1ANIMALS("Swans"), 73.0f, 75.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Swans"), 76.0f, 78.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 353"), 15.94755f, 15.99189f);
	Scatter(S1ANIMALS("Swans"), 73.0f, 75.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
Zone(TEXT("World s1animals, slot 355"), 16.03803f, 16.08237f);
	Scatter(S1ANIMALS("Swans"), 80.0f, 82.0f, 1, 0, -1.f, 1.3f, 0.f, 1, 8.0f);   // FIX ROUND: L6: its box was 3.0 m inside ShedGroomer
	Scatter(S1ANIMALS("Swans"), 75.0f, 77.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 357"), 16.12851f, 16.17285f);
	Scatter(S1ANIMALS("Swans"), 73.0f, 75.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Swans"), 77.0f, 79.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 359"), 16.21899f, 16.26333f);
	Scatter(S1ANIMALS("SleighBlue"), 61.0f, 62.0f, 1, 0, 1.f, 1.15f, 0.f, 1, 5.0f);
Zone(TEXT("World s1animals, slot 361"), 16.30947f, 16.35381f);
	Scatter(S1ANIMALS("Swans"), 80.0f, 82.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("SleighBlue"), 46.5f, 47.5f, 1, 0, -1.f, 1.15f, 0.f, -1, 5.0f);   // FIX ROUND: L5: 3.5 m further out, clear of the people decks at 42.6 m; L5: 3.5 m further out it would stand on the terrain park of slot 362; taken out
	Scatter(S1ANIMALS("Swans"), 76.0f, 78.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 363"), 16.39995f, 16.44429f);
	Scatter(S1ANIMALS("Swans"), 80.0f, 82.0f, 1, 0, -1.f, 1.3f, 0.f, 1, 8.0f);   // FIX ROUND: L4: stood on the car park of slot 362
Zone(TEXT("World s1animals, slot 379"), 17.12379f, 17.16813f);
	Scatter(S1ANIMALS("Foxes"), 61.0f, 62.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Foxes"), 63.0f, 64.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 381"), 17.21427f, 17.25861f);
	Scatter(S1ANIMALS("Foxes"), 65.0f, 66.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 385"), 17.39523f, 17.43957f);
	Scatter(S1ANIMALS("Foxes"), 55.0f, 56.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Foxes"), 64.0f, 65.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 387"), 17.48571f, 17.53005f);
	Scatter(S1ANIMALS("Foxes"), 61.0f, 62.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Foxes"), 63.0f, 64.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 391"), 17.66667f, 17.71101f);
	Scatter(S1ANIMALS("Foxes"), 61.0f, 62.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 393"), 17.75715f, 17.80149f);
	Scatter(S1ANIMALS("Foxes"), 55.0f, 56.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Foxes"), 64.0f, 65.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 397"), 17.93811f, 17.98245f);
	Scatter(S1ANIMALS("Foxes"), 55.0f, 56.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 399"), 18.02859f, 18.07293f);
	Scatter(S1ANIMALS("Foxes"), 61.0f, 62.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Foxes"), 64.0f, 65.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 403"), 18.20955f, 18.25389f);
	Scatter(S1ANIMALS("Foxes"), 61.0f, 62.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Foxes"), 63.0f, 64.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 405"), 18.30003f, 18.34437f);
	Scatter(S1ANIMALS("Foxes"), 65.0f, 66.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 409"), 18.48099f, 18.52533f);
	Scatter(S1ANIMALS("Foxes"), 55.0f, 56.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Foxes"), 64.0f, 65.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 411"), 18.57147f, 18.61581f);
	Scatter(S1ANIMALS("Foxes"), 63.0f, 64.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 415"), 18.75243f, 18.79677f);
	Scatter(S1ANIMALS("Foxes"), 61.0f, 62.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Foxes"), 64.0f, 65.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 417"), 18.84291f, 18.88725f);
	Scatter(S1ANIMALS("Foxes"), 55.0f, 56.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Penguins"), 64.5f, 65.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 421"), 19.02387f, 19.06821f);
	Scatter(S1ANIMALS("Foxes"), 55.0f, 56.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 423"), 19.11435f, 19.15869f);
	Scatter(S1ANIMALS("Seals"), 61.0f, 62.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Penguins"), 64.5f, 65.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 427"), 19.29531f, 19.33965f);
	Scatter(S1ANIMALS("Foxes"), 61.0f, 62.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 429"), 19.38579f, 19.43013f);
	Scatter(S1ANIMALS("Seals"), 56.0f, 57.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Penguins"), 65.5f, 66.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 433"), 19.56675f, 19.61109f);
	Scatter(S1ANIMALS("Foxes"), 55.0f, 56.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Seals"), 65.0f, 66.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 435"), 19.65723f, 19.70157f);
	Scatter(S1ANIMALS("Penguins"), 63.5f, 64.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 437"), 19.74771f, 19.79205f);
	Scatter(S1ANIMALS("Hares"), 47.0f, 48.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 441"), 19.92867f, 19.97301f);
	Scatter(S1ANIMALS("Chamois"), 74.0f, 76.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Hares"), 46.5f, 47.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 76.0f, 78.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 445"), 20.10963f, 20.15397f);
	Scatter(S1ANIMALS("Hares"), 31.5f, 32.5f, 1, 0, 1.f, 1.0f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Hares"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 449"), 20.29059f, 20.33493f);
	Scatter(S1ANIMALS("Hares"), 52.5f, 53.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 72.0f, 74.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Hares"), 49.5f, 50.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);   // FIX ROUND: L5: 3.5 m further out, clear of the people decks at 42.6 m
	Scatter(S1ANIMALS("Chamois"), 76.0f, 78.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 453"), 20.47155f, 20.51589f);
	Scatter(S1ANIMALS("Hares"), 52.5f, 53.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 457"), 20.65251f, 20.69685f);
	Scatter(S1ANIMALS("Chamois"), 72.0f, 74.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Hares"), 52.5f, 53.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 108.0f, 110.0f, 1, 0, -1.f, 1.2f, 0.f, -1, 8.0f);   // FIX ROUND: L6: stood on the strip of RackTrain
Zone(TEXT("World s1animals, slot 461"), 20.83347f, 20.87781f);
	Scatter(S1ANIMALS("Hares"), 41.0f, 42.0f, 1, 0, 1.f, 1.0f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Hares"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 465"), 21.01443f, 21.05877f);
	Scatter(S1ANIMALS("Hares"), 49.5f, 50.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);   // FIX ROUND: L5: 3.5 m further out, clear of the people decks at 42.6 m
	Scatter(S1ANIMALS("Chamois"), 72.0f, 74.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Chamois"), 76.0f, 78.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 469"), 21.19539f, 21.23973f);
	Scatter(S1ANIMALS("Hares"), 31.5f, 32.5f, 1, 0, 1.f, 1.0f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Hares"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 473"), 21.37635f, 21.42069f);
	Scatter(S1ANIMALS("Hares"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 74.0f, 76.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Hares"), 52.5f, 53.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 76.0f, 78.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 477"), 21.55731f, 21.60165f);
	Scatter(S1ANIMALS("Hares"), 52.5f, 53.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Marmots"), 52.5f, 53.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 481"), 21.73827f, 21.78261f);
	Scatter(S1ANIMALS("Chamois"), 74.0f, 76.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Hares"), 54.5f, 55.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Chamois"), 87.0f, 89.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 485"), 21.91923f, 21.96357f);
	Scatter(S1ANIMALS("Hares"), 42.0f, 43.0f, 1, 0, 1.f, 1.0f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Marmots"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Marmots"), 52.5f, 53.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 489"), 22.10019f, 22.14453f);
	Scatter(S1ANIMALS("Chamois"), 72.0f, 74.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Hares"), 50.0f, 51.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);   // FIX ROUND: L5: 3.5 m further out, clear of the people decks at 42.6 m
	Scatter(S1ANIMALS("Chamois"), 76.0f, 78.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 493"), 22.28115f, 22.32549f);
	Scatter(S1ANIMALS("Hares"), 31.5f, 32.5f, 1, 0, 1.f, 1.0f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 505"), 22.82403f, 22.86837f);
	Scatter(S1ANIMALS("Chamois"), 74.0f, 76.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Chamois"), 76.0f, 78.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 517"), 23.36691f, 23.41125f);
	Scatter(S1ANIMALS("Marmots"), 46.5f, 47.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 541"), 24.45267f, 24.49701f);
	Scatter(S1ANIMALS("Marmots"), 46.5f, 47.5f, 1, 0, 1.f, 1.3f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, sky, First tracks: snow buntings"), 0.3f, 2.43f);
	Far(S1ANIMALS("SkyBuntings"), 75.0f, 95.0f, 8, 3, 1.f, 1.f, 0.f, 0, -9.0f, 0.f);     // slots 13 21 29 37 45 53: 12
Zone(TEXT("World s1animals, sky, Black diamond: the pale owl"), 2.43f, 5.65f);
	Far(S1ANIMALS("SkyOwl"), 110.0f, 140.0f, 16, 11, 1.f, 1.f, 0.f, 1, -24.0f, 0.f);     // slots 69 85 101 117: 4
	Far(S1ANIMALS("SkyOwl"), 110.0f, 140.0f, 16, 10, 1.f, 1.f, 0.f, -1, -28.0f, 0.f);     // slots 54 70 86 102 118: 5
Zone(TEXT("World s1animals, sky, The Abominable's glacier: pale gulls"), 8.94f, 11.6f);
	Far(S1ANIMALS("SkyGulls"), 85.0f, 115.0f, 12, 0, 1.f, 1.f, 0.f, 0, -14.0f, 0.f);     // slots 204 216 228 240 252: 10
Zone(TEXT("World s1animals, sky, The white ridge: swans in a V"), 11.6f, 14.3f);
	Far(S1ANIMALS("SkySwans"), 80.0f, 110.0f, 12, 6, 1.f, 1.f, 0.f, 0, -20.0f, 0.f);     // slots 258 270 282 294 306: 10
Zone(TEXT("World s1animals, sky, The ice again: pale gulls"), 17.0f, 19.7f);
	Far(S1ANIMALS("SkyGulls"), 85.0f, 115.0f, 12, 0, 1.f, 1.f, 0.f, 0, -14.0f, 0.f);     // slots 384 396 408 420 432: 10
Zone(TEXT("World s1animals, sky, The high ridge: swans in a V"), 19.7f, 99.0f);
	Far(S1ANIMALS("SkySwans"), 80.0f, 110.0f, 16, 8, 1.f, 1.f, 0.f, 0, -20.0f, 0.f);     // slots 440 456 472 488 504 520 536: 14
#undef S1ANIMALS
// ---- END s1animals ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People5 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1people (the people pass 3 left out: raised groups for the banked bends, the far sides and the empty stretches, ft_world_ice_s1people.py) ----
// 26 meshes, all made by the one make run (C:/ftwork/logs/L5s1people_make.log, 26 built, 0 failed). All TALL. Each brings its own raised floor
// (1.6 to 3.1 m as built, footed 2.2 m into the ground) so its people clear the barrier and a bend's cutting; three pieces a district, four on
// the high ridge, each doing what its district does, and one scaffold tower (decks 5.2 and 8 m as built) for the half pipes.
// Placed at 1.3 like the cast. Steady glow 0.08 on still parts only (lit windows, lamps, the mine's crystals); nothing blinks. No Lit, no Bank.
// Slots: s % 8 == 1 only: every Site is "every 8, phase 7" in a Zone that holds exactly one slot (slot s: (s - 0.45) to (s + 0.45) x 0.04524 km).
// Distances: 29 m (boxes 26.2 to 31.8 m) where the road is level; 42.6 m (40.4 to 44.8 m; the deepest box, the cafe's with its roof eave, 40.0 to 45.2) beside a banked bend
// or a half pipe (a slot counts as banked when the dressing's race banners SM_PoleFeather stand on it, or it lies in the bends the level's plan lists).
// Turned towards the car coming down the road like the cast's stands: yaw -20 / +20 at 29 m, -12 / +12 at 42.6 m (right / left).
// Up = -0.07 x along on the right, +0.07 x along on the left (the hill falls 7 in 100; FTDressL5.inl's rule for a set's parts).
// Where: a scratch replay of both tables (C:/ftwork/jobs/L5s1people/plan.py on the pass 3 fixer's pl.py, every chance taken as 1) gave, for
// each slot and side, up to three places 6.2 m apart whose box is 1 m clear of every piece the two tables put on that slot and side.
// Left alone: the slots under loops, corkscrews and jumps (49, 57, 97, 113, 153, 161, 177, 185: the track leaves Site out there), and 14
// slot-sides with no clear place (9 L, 41 R, 145 R, 193 R, 337 R, 345 L, 361 R, 425 L, 457 R, 465 L, 481 L, 481 R, 513 L, 537 L).
// Estimate: 108 Sites, 300 parts tried (120 at 29 m, 180 at 42.6 m; the 9 towers stand beside half pipes: whether the track keeps a Site there is
// not checked). By the brief's rule x 0.75 = about 225 placed; the left-out slots are already taken off, so up to 300.
// The most of one mesh: SM_S1PEOPLE_D8_Signal, 18 tried. The smallest x min of a TALL mesh: 29 m (its box from 26.2 m). NOT COMPILED, not seen in the game.
#define S1PEOPLE(n) TEXT("/Game/World/IceMountain/SM_S1PEOPLE_" n)
Set(TEXT("WldS1PEOPLE001L"), {
	{ S1PEOPLE("D1_StartPod"), -18.6f, 0.f, -1.3f, 20.0f, 1.3f },
	{ S1PEOPLE("D1_Coaches"), -12.4f, 0.f, -0.87f, 20.0f, 1.3f },
	{ S1PEOPLE("D1_Cafe"), 12.4f, 0.f, 0.87f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE001R"), {
	{ S1PEOPLE("D1_Coaches"), -12.4f, 0.f, 0.87f, -20.0f, 1.3f },
	{ S1PEOPLE("D1_Cafe"), 12.4f, 0.f, -0.87f, -20.0f, 1.3f },
	{ S1PEOPLE("D1_StartPod"), 18.6f, 0.f, -1.3f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE009R"), {
	{ S1PEOPLE("D1_StartPod"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D1_Coaches"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D1_Cafe"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE017L"), {
	{ S1PEOPLE("D1_Coaches"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D1_Cafe"), -12.4f, 0.f, -0.87f, 12.0f, 1.3f },
	{ S1PEOPLE("D1_StartPod"), 12.4f, 0.f, 0.87f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE017R"), {
	{ S1PEOPLE("D1_Cafe"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D1_StartPod"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D1_Coaches"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE025L"), {
	{ S1PEOPLE("D1_StartPod"), 18.6f, 0.f, 1.3f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE025R"), {
	{ S1PEOPLE("D1_Coaches"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D1_Cafe"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D1_StartPod"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE033L"), {
	{ S1PEOPLE("PipeTower"), -14.0f, 0.f, -0.98f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE033R"), {
	{ S1PEOPLE("PipeTower"), -14.0f, 0.f, 0.98f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE041L"), {
	{ S1PEOPLE("D1_Cafe"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D1_StartPod"), 18.6f, 0.f, 1.3f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE065L"), {
	{ S1PEOPLE("D2_Bunker"), -6.2f, 0.f, -0.43f, 12.0f, 1.3f },
	{ S1PEOPLE("D2_Gantry"), 0.0f, 0.f, 0.0f, 12.0f, 1.3f },
	{ S1PEOPLE("D2_Experts"), 6.2f, 0.f, 0.43f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE065R"), {
	{ S1PEOPLE("D2_Gantry"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D2_Experts"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D2_Bunker"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE073L"), {
	{ S1PEOPLE("PipeTower"), 0.0f, 0.f, 0.0f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE073R"), {
	{ S1PEOPLE("PipeTower"), 0.0f, 0.f, 0.0f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE081L"), {
	{ S1PEOPLE("D2_Experts"), -6.2f, 0.f, -0.43f, 12.0f, 1.3f },
	{ S1PEOPLE("D2_Bunker"), 0.0f, 0.f, 0.0f, 12.0f, 1.3f },
	{ S1PEOPLE("D2_Gantry"), 6.2f, 0.f, 0.43f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE081R"), {
	{ S1PEOPLE("D2_Bunker"), -6.2f, 0.f, 0.43f, -12.0f, 1.3f },
	{ S1PEOPLE("D2_Gantry"), 0.0f, 0.f, 0.0f, -12.0f, 1.3f },
	{ S1PEOPLE("D2_Experts"), 6.2f, 0.f, -0.43f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE089L"), {
	{ S1PEOPLE("D2_Gantry"), -6.2f, 0.f, -0.43f, 12.0f, 1.3f },
	{ S1PEOPLE("D2_Experts"), 0.0f, 0.f, 0.0f, 12.0f, 1.3f },
	{ S1PEOPLE("D2_Bunker"), 6.2f, 0.f, 0.43f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE089R"), {
	{ S1PEOPLE("D2_Experts"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D2_Bunker"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D2_Gantry"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE105L"), {
	{ S1PEOPLE("D2_Bunker"), -6.2f, 0.f, -0.43f, 12.0f, 1.3f },
	{ S1PEOPLE("D2_Gantry"), 0.0f, 0.f, 0.0f, 12.0f, 1.3f },
	{ S1PEOPLE("D2_Experts"), 6.2f, 0.f, 0.43f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE105R"), {
	{ S1PEOPLE("D2_Gantry"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D2_Experts"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D2_Bunker"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE121L"), {
	{ S1PEOPLE("D2_Experts"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D2_Bunker"), -12.4f, 0.f, -0.87f, 12.0f, 1.3f },
	{ S1PEOPLE("D2_Gantry"), 12.4f, 0.f, 0.87f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE121R"), {
	{ S1PEOPLE("D2_Bunker"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D2_Gantry"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D2_Experts"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE129L"), {
	{ S1PEOPLE("PipeTower"), -14.0f, 0.f, -0.98f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE129R"), {
	{ S1PEOPLE("PipeTower"), -14.0f, 0.f, 0.98f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE137L"), {
	{ S1PEOPLE("PipeTower"), -14.0f, 0.f, -0.98f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE137R"), {
	{ S1PEOPLE("PipeTower"), 0.0f, 0.f, 0.0f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE145L"), {
	{ S1PEOPLE("D3_Bandstand"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D3_Judges"), 18.6f, 0.f, 1.3f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE169L"), {
	{ S1PEOPLE("D3_Market"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D3_Bandstand"), 18.6f, 0.f, 1.3f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE169R"), {
	{ S1PEOPLE("D3_Bandstand"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D3_Judges"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D3_Market"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE193L"), {
	{ S1PEOPLE("PipeTower"), -14.0f, 0.f, -0.98f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE201L"), {
	{ S1PEOPLE("D4_Haul"), -6.2f, 0.f, -0.43f, 20.0f, 1.3f },
	{ S1PEOPLE("D4_Mess"), 0.0f, 0.f, 0.0f, 20.0f, 1.3f },
	{ S1PEOPLE("D4_Photo"), 6.2f, 0.f, 0.43f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE201R"), {
	{ S1PEOPLE("D4_Mess"), -18.6f, 0.f, 1.3f, -20.0f, 1.3f },
	{ S1PEOPLE("D4_Photo"), -12.4f, 0.f, 0.87f, -20.0f, 1.3f },
	{ S1PEOPLE("D4_Haul"), 12.4f, 0.f, -0.87f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE209L"), {
	{ S1PEOPLE("D4_Photo"), -18.6f, 0.f, -1.3f, 20.0f, 1.3f },
	{ S1PEOPLE("D4_Haul"), -12.4f, 0.f, -0.87f, 20.0f, 1.3f },
	{ S1PEOPLE("D4_Mess"), 12.4f, 0.f, 0.87f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE209R"), {
	{ S1PEOPLE("D4_Haul"), -6.2f, 0.f, 0.43f, -20.0f, 1.3f },
	{ S1PEOPLE("D4_Mess"), 0.0f, 0.f, 0.0f, -20.0f, 1.3f },
	{ S1PEOPLE("D4_Photo"), 6.2f, 0.f, -0.43f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE217L"), {
	{ S1PEOPLE("D4_Mess"), -18.6f, 0.f, -1.3f, 20.0f, 1.3f },
	{ S1PEOPLE("D4_Photo"), -12.4f, 0.f, -0.87f, 20.0f, 1.3f },
	{ S1PEOPLE("D4_Haul"), 12.4f, 0.f, 0.87f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE217R"), {
	{ S1PEOPLE("D4_Photo"), -18.6f, 0.f, 1.3f, -20.0f, 1.3f },
	{ S1PEOPLE("D4_Haul"), -12.4f, 0.f, 0.87f, -20.0f, 1.3f },
	{ S1PEOPLE("D4_Mess"), 12.4f, 0.f, -0.87f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE225L"), {
	{ S1PEOPLE("D4_Haul"), -6.2f, 0.f, -0.43f, 20.0f, 1.3f },
	{ S1PEOPLE("D4_Mess"), 0.0f, 0.f, 0.0f, 20.0f, 1.3f },
	{ S1PEOPLE("D4_Photo"), 6.2f, 0.f, 0.43f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE225R"), {
	{ S1PEOPLE("D4_Mess"), -18.6f, 0.f, 1.3f, -20.0f, 1.3f },
	{ S1PEOPLE("D4_Photo"), -12.4f, 0.f, 0.87f, -20.0f, 1.3f },
	{ S1PEOPLE("D4_Haul"), 12.4f, 0.f, -0.87f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE233L"), {
	{ S1PEOPLE("D4_Photo"), -18.6f, 0.f, -1.3f, 20.0f, 1.3f },
	{ S1PEOPLE("D4_Haul"), -12.4f, 0.f, -0.87f, 20.0f, 1.3f },
	{ S1PEOPLE("D4_Mess"), 12.4f, 0.f, 0.87f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE233R"), {
	{ S1PEOPLE("D4_Haul"), -6.2f, 0.f, 0.43f, -20.0f, 1.3f },
	{ S1PEOPLE("D4_Mess"), 0.0f, 0.f, 0.0f, -20.0f, 1.3f },
	{ S1PEOPLE("D4_Photo"), 6.2f, 0.f, -0.43f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE241L"), {
	{ S1PEOPLE("D4_Mess"), -18.6f, 0.f, -1.3f, 20.0f, 1.3f },
	{ S1PEOPLE("D4_Photo"), -12.4f, 0.f, -0.87f, 20.0f, 1.3f },
	{ S1PEOPLE("D4_Haul"), 12.4f, 0.f, 0.87f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE241R"), {
	{ S1PEOPLE("D4_Photo"), -18.6f, 0.f, 1.3f, -20.0f, 1.3f },
	{ S1PEOPLE("D4_Haul"), -12.4f, 0.f, 0.87f, -20.0f, 1.3f },
	{ S1PEOPLE("D4_Mess"), 12.4f, 0.f, -0.87f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE249L"), {
	{ S1PEOPLE("D4_Haul"), -6.2f, 0.f, -0.43f, 20.0f, 1.3f },
	{ S1PEOPLE("D4_Mess"), 0.0f, 0.f, 0.0f, 20.0f, 1.3f },
	{ S1PEOPLE("D4_Photo"), 6.2f, 0.f, 0.43f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE249R"), {
	{ S1PEOPLE("D4_Mess"), -18.6f, 0.f, 1.3f, -20.0f, 1.3f },
	{ S1PEOPLE("D4_Photo"), -12.4f, 0.f, 0.87f, -20.0f, 1.3f },
	{ S1PEOPLE("D4_Haul"), 12.4f, 0.f, -0.87f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE257L"), {
	{ S1PEOPLE("D5_SkyWatch"), -18.6f, 0.f, -1.3f, 20.0f, 1.3f },
	{ S1PEOPLE("D5_Kites"), -12.4f, 0.f, -0.87f, 20.0f, 1.3f },
	{ S1PEOPLE("D5_Cairn"), 12.4f, 0.f, 0.87f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE257R"), {
	{ S1PEOPLE("D5_Kites"), -18.6f, 0.f, 1.3f, -20.0f, 1.3f },
	{ S1PEOPLE("D5_Cairn"), -12.4f, 0.f, 0.87f, -20.0f, 1.3f },
	{ S1PEOPLE("D5_SkyWatch"), 12.4f, 0.f, -0.87f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE265L"), {
	{ S1PEOPLE("D5_Cairn"), -6.2f, 0.f, -0.43f, 20.0f, 1.3f },
	{ S1PEOPLE("D5_SkyWatch"), 0.0f, 0.f, 0.0f, 20.0f, 1.3f },
	{ S1PEOPLE("D5_Kites"), 6.2f, 0.f, 0.43f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE265R"), {
	{ S1PEOPLE("D5_SkyWatch"), -6.2f, 0.f, 0.43f, -12.0f, 1.3f },
	{ S1PEOPLE("D5_Kites"), 0.0f, 0.f, 0.0f, -12.0f, 1.3f },
	{ S1PEOPLE("D5_Cairn"), 6.2f, 0.f, -0.43f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE273L"), {
	{ S1PEOPLE("D5_Kites"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D5_Cairn"), -12.4f, 0.f, -0.87f, 12.0f, 1.3f },
	{ S1PEOPLE("D5_SkyWatch"), 12.4f, 0.f, 0.87f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE273R"), {
	{ S1PEOPLE("D5_Cairn"), -6.2f, 0.f, 0.43f, -20.0f, 1.3f },
	{ S1PEOPLE("D5_SkyWatch"), 0.0f, 0.f, 0.0f, -20.0f, 1.3f },
	{ S1PEOPLE("D5_Kites"), 6.2f, 0.f, -0.43f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE281L"), {
	{ S1PEOPLE("D5_SkyWatch"), -18.6f, 0.f, -1.3f, 20.0f, 1.3f },
	{ S1PEOPLE("D5_Kites"), -12.4f, 0.f, -0.87f, 20.0f, 1.3f },
	{ S1PEOPLE("D5_Cairn"), 12.4f, 0.f, 0.87f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE281R"), {
	{ S1PEOPLE("D5_Kites"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D5_Cairn"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D5_SkyWatch"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE289L"), {
	{ S1PEOPLE("D5_Cairn"), -6.2f, 0.f, -0.43f, 20.0f, 1.3f },
	{ S1PEOPLE("D5_SkyWatch"), 0.0f, 0.f, 0.0f, 20.0f, 1.3f },
	{ S1PEOPLE("D5_Kites"), 6.2f, 0.f, 0.43f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE289R"), {
	{ S1PEOPLE("D5_SkyWatch"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D5_Kites"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D5_Cairn"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE297L"), {
	{ S1PEOPLE("D5_Kites"), -6.2f, 0.f, -0.43f, 12.0f, 1.3f },
	{ S1PEOPLE("D5_Cairn"), 0.0f, 0.f, 0.0f, 12.0f, 1.3f },
	{ S1PEOPLE("D5_SkyWatch"), 6.2f, 0.f, 0.43f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE297R"), {
	{ S1PEOPLE("D5_Cairn"), -6.2f, 0.f, 0.43f, -20.0f, 1.3f },
	{ S1PEOPLE("D5_SkyWatch"), 0.0f, 0.f, 0.0f, -20.0f, 1.3f },
	{ S1PEOPLE("D5_Kites"), 6.2f, 0.f, -0.43f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE305L"), {
	{ S1PEOPLE("D5_SkyWatch"), -18.6f, 0.f, -1.3f, 20.0f, 1.3f },
	{ S1PEOPLE("D5_Kites"), -12.4f, 0.f, -0.87f, 20.0f, 1.3f },
	{ S1PEOPLE("D5_Cairn"), 12.4f, 0.f, 0.87f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE305R"), {
	{ S1PEOPLE("D5_Kites"), -6.2f, 0.f, 0.43f, -12.0f, 1.3f },
	{ S1PEOPLE("D5_Cairn"), 0.0f, 0.f, 0.0f, -12.0f, 1.3f },
	{ S1PEOPLE("D5_SkyWatch"), 6.2f, 0.f, -0.43f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE313L"), {
	{ S1PEOPLE("D5_Cairn"), -6.2f, 0.f, -0.43f, 20.0f, 1.3f },
	{ S1PEOPLE("D5_SkyWatch"), 0.0f, 0.f, 0.0f, 20.0f, 1.3f },
	{ S1PEOPLE("D5_Kites"), 6.2f, 0.f, 0.43f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE313R"), {
	{ S1PEOPLE("D5_SkyWatch"), -6.2f, 0.f, 0.43f, -12.0f, 1.3f },
	{ S1PEOPLE("D5_Kites"), 0.0f, 0.f, 0.0f, -12.0f, 1.3f },
	{ S1PEOPLE("D5_Cairn"), 6.2f, 0.f, -0.43f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE321L"), {
	{ S1PEOPLE("D6_Quartet"), -18.6f, 0.f, -1.3f, 20.0f, 1.3f },
	{ S1PEOPLE("D6_Baths"), -12.4f, 0.f, -0.87f, 20.0f, 1.3f },
	{ S1PEOPLE("D6_Gallery"), 12.4f, 0.f, 0.87f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE321R"), {
	{ S1PEOPLE("D6_Baths"), -18.6f, 0.f, 1.3f, -20.0f, 1.3f },
	{ S1PEOPLE("D6_Gallery"), -12.4f, 0.f, 0.87f, -20.0f, 1.3f },
	{ S1PEOPLE("D6_Quartet"), 12.4f, 0.f, -0.87f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE329L"), {
	{ S1PEOPLE("D6_Gallery"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D6_Quartet"), -12.4f, 0.f, -0.87f, 12.0f, 1.3f },
	{ S1PEOPLE("D6_Baths"), 12.4f, 0.f, 0.87f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE329R"), {
	{ S1PEOPLE("D6_Quartet"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D6_Baths"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D6_Gallery"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE337L"), {
	{ S1PEOPLE("D6_Baths"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D6_Gallery"), -12.4f, 0.f, -0.87f, 12.0f, 1.3f },
	{ S1PEOPLE("D6_Quartet"), 12.4f, 0.f, 0.87f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE345R"), {
	{ S1PEOPLE("D6_Baths"), -18.6f, 0.f, 1.3f, -20.0f, 1.3f },
	{ S1PEOPLE("D6_Gallery"), -12.4f, 0.f, 0.87f, -20.0f, 1.3f },
	{ S1PEOPLE("D6_Quartet"), 12.4f, 0.f, -0.87f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE353L"), {
	{ S1PEOPLE("D6_Gallery"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D6_Quartet"), -12.4f, 0.f, -0.87f, 12.0f, 1.3f },
	{ S1PEOPLE("D6_Baths"), 12.4f, 0.f, 0.87f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE353R"), {
	{ S1PEOPLE("D6_Quartet"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D6_Baths"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D6_Gallery"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE361L"), {
	{ S1PEOPLE("D6_Baths"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D6_Gallery"), -12.4f, 0.f, -0.87f, 12.0f, 1.3f },
	{ S1PEOPLE("D6_Quartet"), 18.6f, 0.f, 1.3f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE369L"), {
	{ S1PEOPLE("D6_Quartet"), -18.6f, 0.f, -1.3f, 20.0f, 1.3f },
	{ S1PEOPLE("D6_Baths"), -12.4f, 0.f, -0.87f, 20.0f, 1.3f },
	{ S1PEOPLE("D6_Gallery"), 12.4f, 0.f, 0.87f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE369R"), {
	{ S1PEOPLE("D6_Baths"), -18.6f, 0.f, 1.3f, -20.0f, 1.3f },
	{ S1PEOPLE("D6_Gallery"), -12.4f, 0.f, 0.87f, -20.0f, 1.3f },
	{ S1PEOPLE("D6_Quartet"), 12.4f, 0.f, -0.87f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE377L"), {
	{ S1PEOPLE("D7_Staging"), -6.2f, 0.f, -0.43f, 12.0f, 1.3f },
	{ S1PEOPLE("D7_Cart"), 0.0f, 0.f, 0.0f, 12.0f, 1.3f },
	{ S1PEOPLE("D7_Trader"), 6.2f, 0.f, 0.43f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE377R"), {
	{ S1PEOPLE("D7_Cart"), -6.2f, 0.f, 0.43f, -12.0f, 1.3f },
	{ S1PEOPLE("D7_Trader"), 0.0f, 0.f, 0.0f, -12.0f, 1.3f },
	{ S1PEOPLE("D7_Staging"), 6.2f, 0.f, -0.43f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE385L"), {
	{ S1PEOPLE("D7_Trader"), -6.2f, 0.f, -0.43f, 12.0f, 1.3f },
	{ S1PEOPLE("D7_Staging"), 0.0f, 0.f, 0.0f, 12.0f, 1.3f },
	{ S1PEOPLE("D7_Cart"), 6.2f, 0.f, 0.43f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE385R"), {
	{ S1PEOPLE("D7_Staging"), -6.2f, 0.f, 0.43f, -12.0f, 1.3f },
	{ S1PEOPLE("D7_Cart"), 0.0f, 0.f, 0.0f, -12.0f, 1.3f },
	{ S1PEOPLE("D7_Trader"), 6.2f, 0.f, -0.43f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE393L"), {
	{ S1PEOPLE("D7_Cart"), -18.6f, 0.f, -1.3f, 20.0f, 1.3f },
	{ S1PEOPLE("D7_Trader"), -12.4f, 0.f, -0.87f, 20.0f, 1.3f },
	{ S1PEOPLE("D7_Staging"), 12.4f, 0.f, 0.87f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE393R"), {
	{ S1PEOPLE("D7_Trader"), -18.6f, 0.f, 1.3f, -20.0f, 1.3f },
	{ S1PEOPLE("D7_Staging"), -12.4f, 0.f, 0.87f, -20.0f, 1.3f },
	{ S1PEOPLE("D7_Cart"), 12.4f, 0.f, -0.87f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE401L"), {
	{ S1PEOPLE("D7_Staging"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D7_Cart"), -12.4f, 0.f, -0.87f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE401R"), {
	{ S1PEOPLE("D7_Cart"), -6.2f, 0.f, 0.43f, -12.0f, 1.3f },
	{ S1PEOPLE("D7_Trader"), 0.0f, 0.f, 0.0f, -12.0f, 1.3f },
	{ S1PEOPLE("D7_Staging"), 6.2f, 0.f, -0.43f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE409L"), {
	{ S1PEOPLE("D7_Trader"), -6.2f, 0.f, -0.43f, 12.0f, 1.3f },
	{ S1PEOPLE("D7_Staging"), 0.0f, 0.f, 0.0f, 12.0f, 1.3f },
	{ S1PEOPLE("D7_Cart"), 6.2f, 0.f, 0.43f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE409R"), {
	{ S1PEOPLE("D7_Staging"), -6.2f, 0.f, 0.43f, -12.0f, 1.3f },
	{ S1PEOPLE("D7_Cart"), 0.0f, 0.f, 0.0f, -12.0f, 1.3f },
	{ S1PEOPLE("D7_Trader"), 6.2f, 0.f, -0.43f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE417L"), {
	{ S1PEOPLE("D7_Cart"), -18.6f, 0.f, -1.3f, 20.0f, 1.3f },
	{ S1PEOPLE("D7_Trader"), -12.4f, 0.f, -0.87f, 20.0f, 1.3f },
	{ S1PEOPLE("D7_Staging"), 12.4f, 0.f, 0.87f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE417R"), {
	{ S1PEOPLE("D7_Trader"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D7_Staging"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D7_Cart"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE425R"), {
	{ S1PEOPLE("D7_Cart"), -6.2f, 0.f, 0.43f, -12.0f, 1.3f },
	{ S1PEOPLE("D7_Trader"), 0.0f, 0.f, 0.0f, -12.0f, 1.3f },
	{ S1PEOPLE("D7_Staging"), 6.2f, 0.f, -0.43f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE433L"), {
	{ S1PEOPLE("D7_Trader"), -6.2f, 0.f, -0.43f, 12.0f, 1.3f },
	{ S1PEOPLE("D7_Staging"), 0.0f, 0.f, 0.0f, 12.0f, 1.3f },
	{ S1PEOPLE("D7_Cart"), 6.2f, 0.f, 0.43f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE433R"), {
	{ S1PEOPLE("D7_Staging"), -6.2f, 0.f, 0.43f, -12.0f, 1.3f },
	{ S1PEOPLE("D7_Cart"), 0.0f, 0.f, 0.0f, -12.0f, 1.3f },
	{ S1PEOPLE("D7_Trader"), 6.2f, 0.f, -0.43f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE441L"), {
	{ S1PEOPLE("D8_Gallery"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D8_Signal"), -12.4f, 0.f, -0.87f, 12.0f, 1.3f },
	{ S1PEOPLE("D8_Lift"), 12.4f, 0.f, 0.87f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE441R"), {
	{ S1PEOPLE("D8_Signal"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D8_Lift"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D8_Hoist"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE449L"), {
	{ S1PEOPLE("D8_Lift"), -6.2f, 0.f, -0.43f, 12.0f, 1.3f },
	{ S1PEOPLE("D8_Hoist"), 0.0f, 0.f, 0.0f, 12.0f, 1.3f },
	{ S1PEOPLE("D8_Gallery"), 6.2f, 0.f, 0.43f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE449R"), {
	{ S1PEOPLE("D8_Hoist"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D8_Gallery"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D8_Signal"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE457L"), {
	{ S1PEOPLE("D8_Gallery"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D8_Signal"), -12.4f, 0.f, -0.87f, 12.0f, 1.3f },
	{ S1PEOPLE("D8_Lift"), 12.4f, 0.f, 0.87f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE465R"), {
	{ S1PEOPLE("D8_Hoist"), -6.2f, 0.f, 0.43f, -12.0f, 1.3f },
	{ S1PEOPLE("D8_Gallery"), 0.0f, 0.f, 0.0f, -12.0f, 1.3f },
	{ S1PEOPLE("D8_Signal"), 6.2f, 0.f, -0.43f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE473L"), {
	{ S1PEOPLE("D8_Gallery"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D8_Signal"), -12.4f, 0.f, -0.87f, 12.0f, 1.3f },
	{ S1PEOPLE("D8_Lift"), 12.4f, 0.f, 0.87f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE473R"), {
	{ S1PEOPLE("D8_Signal"), -18.6f, 0.f, 1.3f, -20.0f, 1.3f },
	{ S1PEOPLE("D8_Lift"), -12.4f, 0.f, 0.87f, -20.0f, 1.3f },
	{ S1PEOPLE("D8_Hoist"), 12.4f, 0.f, -0.87f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE489L"), {
	{ S1PEOPLE("D8_Gallery"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D8_Signal"), -12.4f, 0.f, -0.87f, 12.0f, 1.3f },
	{ S1PEOPLE("D8_Lift"), 12.4f, 0.f, 0.87f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE489R"), {
	{ S1PEOPLE("D8_Signal"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D8_Lift"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D8_Hoist"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE497L"), {
	{ S1PEOPLE("D8_Lift"), -18.6f, 0.f, -1.3f, 20.0f, 1.3f },
	{ S1PEOPLE("D8_Hoist"), -12.4f, 0.f, -0.87f, 20.0f, 1.3f },
	{ S1PEOPLE("D8_Gallery"), 12.4f, 0.f, 0.87f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE497R"), {
	{ S1PEOPLE("D8_Hoist"), -18.6f, 0.f, 1.3f, -20.0f, 1.3f },
	{ S1PEOPLE("D8_Gallery"), -12.4f, 0.f, 0.87f, -20.0f, 1.3f },
	{ S1PEOPLE("D8_Signal"), 12.4f, 0.f, -0.87f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE505L"), {
	{ S1PEOPLE("D8_Gallery"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D8_Signal"), -12.4f, 0.f, -0.87f, 12.0f, 1.3f },
	{ S1PEOPLE("D8_Lift"), 12.4f, 0.f, 0.87f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE505R"), {
	{ S1PEOPLE("D8_Signal"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D8_Lift"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D8_Hoist"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE513R"), {
	{ S1PEOPLE("D8_Hoist"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D8_Gallery"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D8_Signal"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE521L"), {
	{ S1PEOPLE("D8_Gallery"), -18.6f, 0.f, -1.3f, 20.0f, 1.3f },
	{ S1PEOPLE("D8_Signal"), -12.4f, 0.f, -0.87f, 20.0f, 1.3f },
	{ S1PEOPLE("D8_Lift"), 12.4f, 0.f, 0.87f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE521R"), {
	{ S1PEOPLE("D8_Signal"), -18.6f, 0.f, 1.3f, -20.0f, 1.3f },
	{ S1PEOPLE("D8_Lift"), -12.4f, 0.f, 0.87f, -20.0f, 1.3f },
	{ S1PEOPLE("D8_Hoist"), 12.4f, 0.f, -0.87f, -20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE529L"), {
	{ S1PEOPLE("D8_Lift"), -18.6f, 0.f, -1.3f, 12.0f, 1.3f },
	{ S1PEOPLE("D8_Hoist"), -12.4f, 0.f, -0.87f, 12.0f, 1.3f },
	{ S1PEOPLE("D8_Gallery"), 12.4f, 0.f, 0.87f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE529R"), {
	{ S1PEOPLE("D8_Hoist"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D8_Gallery"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D8_Signal"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE537R"), {
	{ S1PEOPLE("D8_Signal"), -18.6f, 0.f, 1.3f, -12.0f, 1.3f },
	{ S1PEOPLE("D8_Lift"), -12.4f, 0.f, 0.87f, -12.0f, 1.3f },
	{ S1PEOPLE("D8_Hoist"), 12.4f, 0.f, -0.87f, -12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE545L"), {
	{ S1PEOPLE("D8_Lift"), -18.6f, 0.f, -1.3f, 20.0f, 1.3f },
	{ S1PEOPLE("D8_Hoist"), -12.4f, 0.f, -0.87f, 20.0f, 1.3f },
	{ S1PEOPLE("D8_Gallery"), 12.4f, 0.f, 0.87f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE545R"), {
	{ S1PEOPLE("D8_Hoist"), -18.6f, 0.f, 1.3f, -20.0f, 1.3f },
	{ S1PEOPLE("D8_Gallery"), -12.4f, 0.f, 0.87f, -20.0f, 1.3f },
	{ S1PEOPLE("D8_Signal"), 12.4f, 0.f, -0.87f, -20.0f, 1.3f },
});
Zone(TEXT("World s1people, slot 1, First tracks"), 0.0249f, 0.0656f);     // 0.05 km, level
	Site(TEXT("WldS1PEOPLE001L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE001R"), 1, 29.0f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 9, First tracks"), 0.3868f, 0.4275f);     // 0.41 km, banked
	Site(TEXT("WldS1PEOPLE009R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 17, First tracks"), 0.7487f, 0.7894f);     // 0.77 km, banked
	Site(TEXT("WldS1PEOPLE017L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE017R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 25, First tracks"), 1.1106f, 1.1514f);     // 1.13 km, banked
	Site(TEXT("WldS1PEOPLE025L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE025R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 33, First tracks"), 1.4726f, 1.5133f);     // 1.49 km, half pipe
	Site(TEXT("WldS1PEOPLE033L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE033R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 41, First tracks"), 1.8345f, 1.8752f);     // 1.85 km, banked
	Site(TEXT("WldS1PEOPLE041L"), -1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 65, Black diamond"), 2.9202f, 2.9610f);     // 2.94 km, banked
	Site(TEXT("WldS1PEOPLE065L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE065R"), 1, 42.6f, 8, 7, -1.f);   // FIX ROUND: L2: these people stood inside the rock cutting of slot 65
Zone(TEXT("World s1people, slot 73, Black diamond"), 3.2822f, 3.3229f);     // 3.30 km, half pipe
	Site(TEXT("WldS1PEOPLE073L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE073R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 81, Black diamond"), 3.6441f, 3.6848f);     // 3.66 km, banked
	Site(TEXT("WldS1PEOPLE081L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE081R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 89, Black diamond"), 4.0060f, 4.0467f);     // 4.03 km, banked
	Site(TEXT("WldS1PEOPLE089L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE089R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 105, Black diamond"), 4.7298f, 4.7706f);     // 4.75 km, banked
	Site(TEXT("WldS1PEOPLE105L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE105R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 121, Black diamond"), 5.4537f, 5.4944f);     // 5.47 km, banked
	Site(TEXT("WldS1PEOPLE121L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE121R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 129, The valley run"), 5.8156f, 5.8563f);     // 5.84 km, half pipe
	Site(TEXT("WldS1PEOPLE129L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE129R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 137, The valley run"), 6.1775f, 6.2182f);     // 6.20 km, half pipe
	Site(TEXT("WldS1PEOPLE137L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE137R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 145, The valley run"), 6.5394f, 6.5802f);     // 6.56 km, banked
	Site(TEXT("WldS1PEOPLE145L"), -1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 169, The valley run"), 7.6252f, 7.6659f);     // 7.65 km, banked
	Site(TEXT("WldS1PEOPLE169L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE169R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 193, The valley run"), 8.7110f, 8.7517f);     // 8.73 km, half pipe
	Site(TEXT("WldS1PEOPLE193L"), -1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 201, The Abominable's glacier"), 9.0729f, 9.1136f);     // 9.09 km, level
	Site(TEXT("WldS1PEOPLE201L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE201R"), 1, 29.0f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 209, The Abominable's glacier"), 9.4348f, 9.4755f);     // 9.46 km, level
	Site(TEXT("WldS1PEOPLE209L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE209R"), 1, 29.0f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 217, The Abominable's glacier"), 9.7967f, 9.8374f);     // 9.82 km, level
	Site(TEXT("WldS1PEOPLE217L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE217R"), 1, 29.0f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 225, The Abominable's glacier"), 10.1586f, 10.1994f);     // 10.18 km, level
	Site(TEXT("WldS1PEOPLE225L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE225R"), 1, 29.0f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 233, The Abominable's glacier"), 10.5206f, 10.5613f);     // 10.54 km, level
	Site(TEXT("WldS1PEOPLE233L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE233R"), 1, 29.0f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 241, The Abominable's glacier"), 10.8825f, 10.9232f);     // 10.90 km, level
	Site(TEXT("WldS1PEOPLE241L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE241R"), 1, 29.0f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 249, The Abominable's glacier"), 11.2444f, 11.2851f);     // 11.26 km, level
	Site(TEXT("WldS1PEOPLE249L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE249R"), 1, 29.0f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 257, The white ridge"), 11.6063f, 11.6470f);     // 11.63 km, level
	Site(TEXT("WldS1PEOPLE257L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE257R"), 1, 29.0f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 265, The white ridge"), 11.9682f, 12.0090f);     // 11.99 km, level
	Site(TEXT("WldS1PEOPLE265L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE265R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 273, The white ridge"), 12.3302f, 12.3709f);     // 12.35 km, level
	Site(TEXT("WldS1PEOPLE273L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE273R"), 1, 29.0f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 281, The white ridge"), 12.6921f, 12.7328f);     // 12.71 km, level
	Site(TEXT("WldS1PEOPLE281L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE281R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 289, The white ridge"), 13.0540f, 13.0947f);     // 13.07 km, level
	Site(TEXT("WldS1PEOPLE289L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE289R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 297, The white ridge"), 13.4159f, 13.4566f);     // 13.44 km, level
	Site(TEXT("WldS1PEOPLE297L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE297R"), 1, 29.0f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 305, The white ridge"), 13.7778f, 13.8186f);     // 13.80 km, level
	Site(TEXT("WldS1PEOPLE305L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE305R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 313, The white ridge"), 14.1398f, 14.1805f);     // 14.16 km, level
	Site(TEXT("WldS1PEOPLE313L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE313R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 321, The resort"), 14.5017f, 14.5424f);     // 14.52 km, level
	Site(TEXT("WldS1PEOPLE321LFx0"), -1, 29.0f, 8, 7, 1.f);   // FIX ROUND: L5: this deck stood in S1D6_Fountain of slot 322
	Site(TEXT("WldS1PEOPLE321R"), 1, 29.0f, 8, 7, -1.f);   // FIX ROUND: L3: these decks stood 0.2 m in front of the chalet row street front
Zone(TEXT("World s1people, slot 329, The resort"), 14.8636f, 14.9043f);     // 14.88 km, banked
	Site(TEXT("WldS1PEOPLE329L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE329R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 337, The resort"), 15.2255f, 15.2662f);     // 15.25 km, banked
	Site(TEXT("WldS1PEOPLE337LFx0"), -1, 42.6f, 8, 7, 1.f);   // FIX ROUND: L5: this deck stood in S1D6_SpaHotel of slot 338
Zone(TEXT("World s1people, slot 345, The resort"), 15.5874f, 15.6282f);     // 15.61 km, level
	Site(TEXT("WldS1PEOPLE345R"), 1, 29.0f, 8, 7, -1.f);   // FIX ROUND: L3: these decks stood 0.2 m in front of the chalet row street front
Zone(TEXT("World s1people, slot 353, The resort"), 15.9494f, 15.9901f);     // 15.97 km, banked
	Site(TEXT("WldS1PEOPLE353LFx0"), -1, 42.6f, 8, 7, 1.f);   // FIX ROUND: L5: this deck stood in S1D6_ColonnadeWalk of slot 354
	Site(TEXT("WldS1PEOPLE353R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 361, The resort"), 16.3113f, 16.3520f);     // 16.33 km, banked
	Site(TEXT("WldS1PEOPLE361L"), -1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 369, The resort"), 16.6732f, 16.7139f);     // 16.69 km, level
	Site(TEXT("WldS1PEOPLE369LFx0"), -1, 29.0f, 8, 7, 1.f);   // FIX ROUND: L5: this deck stood in S1D6_Fountain of slot 370
	Site(TEXT("WldS1PEOPLE369R"), 1, 29.0f, 8, 7, -1.f);   // FIX ROUND: L3: these decks stood 0.2 m in front of the chalet row street front
Zone(TEXT("World s1people, slot 377, The ice again"), 17.0351f, 17.0758f);     // 17.06 km, banked
	Site(TEXT("WldS1PEOPLE377L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE377R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 385, The ice again"), 17.3970f, 17.4378f);     // 17.42 km, banked
	Site(TEXT("WldS1PEOPLE385L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE385R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 393, The ice again"), 17.7590f, 17.7997f);     // 17.78 km, level
	Site(TEXT("WldS1PEOPLE393L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE393R"), 1, 29.0f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 401, The ice again"), 18.1209f, 18.1616f);     // 18.14 km, banked
	Site(TEXT("WldS1PEOPLE401L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE401R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 409, The ice again"), 18.4828f, 18.5235f);     // 18.50 km, banked
	Site(TEXT("WldS1PEOPLE409L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE409R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 417, The ice again"), 18.8447f, 18.8854f);     // 18.87 km, level
	Site(TEXT("WldS1PEOPLE417L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE417R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 425, The ice again"), 19.2066f, 19.2474f);     // 19.23 km, banked
	Site(TEXT("WldS1PEOPLE425R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 433, The ice again"), 19.5686f, 19.6093f);     // 19.59 km, banked
	Site(TEXT("WldS1PEOPLE433L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE433R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 441, The high ridge"), 19.9305f, 19.9712f);     // 19.95 km, banked
	Site(TEXT("WldS1PEOPLE441L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE441R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 449, The high ridge"), 20.2924f, 20.3331f);     // 20.31 km, banked
	Site(TEXT("WldS1PEOPLE449L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE449R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 457, The high ridge"), 20.6543f, 20.6950f);     // 20.67 km, banked
	Site(TEXT("WldS1PEOPLE457L"), -1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 465, The high ridge"), 21.0162f, 21.0570f);     // 21.04 km, banked
	Site(TEXT("WldS1PEOPLE465R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 473, The high ridge"), 21.3782f, 21.4189f);     // 21.40 km, level
	Site(TEXT("WldS1PEOPLE473L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE473R"), 1, 29.0f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 489, The high ridge"), 22.1020f, 22.1427f);     // 22.12 km, banked
	Site(TEXT("WldS1PEOPLE489L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE489R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 497, The high ridge"), 22.4639f, 22.5046f);     // 22.48 km, level
	Site(TEXT("WldS1PEOPLE497L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE497R"), 1, 29.0f, 8, 7, -1.f);   // FIX ROUND: L3: these decks stood 0.2 m in front of the chalet row street front
Zone(TEXT("World s1people, slot 505, The high ridge"), 22.8258f, 22.8666f);     // 22.85 km, banked
	Site(TEXT("WldS1PEOPLE505L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE505R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 513, The high ridge"), 23.1878f, 23.2285f);     // 23.21 km, banked
	Site(TEXT("WldS1PEOPLE513R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 521, The high ridge"), 23.5497f, 23.5904f);     // 23.57 km, level
	Site(TEXT("WldS1PEOPLE521L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE521R"), 1, 29.0f, 8, 7, -1.f);   // FIX ROUND: L3: these decks stood 0.2 m in front of the chalet row street front
Zone(TEXT("World s1people, slot 529, The high ridge"), 23.9116f, 23.9523f);     // 23.93 km, banked
	Site(TEXT("WldS1PEOPLE529L"), -1, 42.6f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE529R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 537, The high ridge"), 24.2735f, 24.3142f);     // 24.29 km, banked
	Site(TEXT("WldS1PEOPLE537R"), 1, 42.6f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 545, The high ridge"), 24.6354f, 24.6762f);     // 24.66 km, level
	Site(TEXT("WldS1PEOPLE545L"), -1, 29.0f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE545R"), 1, 29.0f, 8, 7, 1.f);
#undef S1PEOPLE
// ---- END s1people ----
}();
[&]() {	// Stage 1, the fix round: its own function like the blocks above
// ---- BEGIN s1fix (the fix round after Stage 1's review: pieces moved to another slot, and sets with a part taken out) ----
// Rules above this block were changed in place or switched off (chance -1: never placed), so that every rule keeps its
// running index and nothing that was not meant to move is re-rolled. What is ADDED stands here, after every other rule.
// Every rule is a Site with chance 1 in a Zone of one slot. A Set named WldS1...Fx<n> is the set of that name without its part(s) n (counted from 0).
Set(TEXT("WldS1D1CafeLFx0"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D1_LampMast"), -9.0f, -3.0f, -0.63f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_Windsock"), 9.0f, -3.0f, 0.63f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_SnowStake"), -11.5f, 1.5f, -0.81f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_SkiRack"), 11.5f, 1.0f, 0.81f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D1CafeLFx02"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D1_LampMast"), -9.0f, -3.0f, -0.63f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_SnowStake"), -11.5f, 1.5f, -0.81f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_SkiRack"), 11.5f, 1.0f, 0.81f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D1HallLFx1"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D1_LiftHall"), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_LampMast"), -10.5f, -4.0f, -0.74f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_LockerPods"), -11.5f, -1.0f, -0.81f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_Windsock"), 11.0f, 2.0f, 0.77f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D1HallRFx01"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D1_LampMast"), -10.5f, -4.0f, 0.74f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_LockerPods"), -11.5f, -1.0f, 0.81f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_Windsock"), 11.0f, 2.0f, -0.77f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D1HostelRFx0"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D1_LampMast"), -10.5f, -2.0f, 0.74f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_SkiRack"), -13.0f, -4.5f, 0.91f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_LockerPods"), 10.5f, -4.5f, -0.74f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_SnowStake"), 13.0f, 1.0f, -0.91f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D1SchoolRFx03"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D1_SkiRack"), -4.0f, -5.4f, 0.28f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_SnowStake"), 11.5f, -2.0f, -0.81f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D1ShopRFx3"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D1_BoardShop"), 0.0f, 0.0f, 0.0f, -15.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_SkiRack"), -9.5f, -3.0f, 0.67f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_SkiRack"), 9.5f, -3.0f, -0.67f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_LampMast"), -8.5f, 1.5f, 0.6f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D1StartRFx0"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D1_Podium"), 10.0f, -1.0f, -0.7f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_TicketGate"), -8.5f, -2.5f, 0.6f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_LampMast"), -6.0f, -4.5f, 0.42f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D1_Windsock"), -7.0f, 3.0f, 0.49f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D2PatrolRFx0345"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D2_DiamondPlate"), -9.5f, -4.0f, 0.67f, -55.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D2_BollardRow"), 11.0f, -3.5f, -0.77f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D2PatrolRFx3"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D2_PatrolHut"), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D2_DiamondPlate"), -9.5f, -4.0f, 0.67f, -55.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D2_BollardRow"), 11.0f, -3.5f, -0.77f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D2_Tetrapods"), 13.5f, 3.0f, -0.95f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D2_SirenMast"), -8.5f, 3.2f, 0.6f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D2SignalRFx1"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D2_SirenMast"), -15.0f, 0.0f, 1.05f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D2_Tetrapods"), -12.5f, 2.0f, 0.88f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D2_ChevronBarrier"), 15.5f, -1.5f, -1.09f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D2_VentCluster"), 13.0f, 3.0f, -0.91f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D2_DiamondPlate"), 20.5f, 2.5f, -1.44f, -55.0f, 1.0f },
});
Set(TEXT("WldS1D3B132LFx0"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D3_TallHouse"), 17.4f, 10.2f, 1.22f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D3_BenchPlanter"), 17.4f, -8.0f, 1.22f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D3_Granary"), -17.4f, 0.0f, -1.22f, -5.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D3_TallHouse"), -17.4f, 10.2f, -1.22f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D3_LanternPost"), -17.4f, -8.0f, -1.22f, 8.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D3_MarketStall"), -13.0f, -8.0f, -0.91f, -12.0f, 1.0f },
});
Set(TEXT("WldS1D3B166RFx2"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D3_Bakery"), -17.4f, 2.0f, 1.22f, 4.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D3_TownGate"), 17.4f, 0.0f, -1.22f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D3B176RFx2"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D3_OrielHouse"), 17.4f, 0.0f, -1.22f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D3_FingerPost"), 13.0f, -8.0f, -0.91f, 8.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D3_RibbonPole"), 0.0f, 0.0f, -0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D3B194LFx3"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D3_TallHouse"), 17.4f, 0.0f, 1.22f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D3_Bakery"), 17.4f, 10.2f, 1.22f, -5.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D3_WashLine"), 17.4f, -8.0f, 1.22f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D3_Granary"), -17.4f, 10.2f, -1.22f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D3_SledRack"), -17.4f, -8.0f, -1.22f, 15.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D3_LanternPost"), -13.0f, -8.0f, -0.91f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D3_Bakery"), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D4CampARFx0"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D4_VaultHut"), -15.0f, -1.0f, 1.05f, -8.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D4_StoreDome"), 14.0f, 1.0f, -0.98f, -8.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D4_FuelDump"), -9.5f, -6.5f, 0.67f, -8.0f, 1.0f },
});
Set(TEXT("WldS1D4CampCRFx2"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D4_SledgeGarage"), 0.0f, 0.0f, 0.0f, -8.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D4_VaultHut"), 13.0f, 0.0f, -0.91f, -8.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D4_CrateStack"), 7.0f, -6.5f, -0.49f, -8.0f, 1.0f },
});
Set(TEXT("WldS1D4NearCLFx0"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D4_LoadedSledge"), -9.0f, 0.0f, -0.63f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D4_CrateStack"), 9.0f, 0.0f, 0.63f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D4NearDRFx2"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D4_DepotCairn"), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D4_DryingLine"), -9.0f, 0.0f, 0.63f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5F1LFx123"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D5_RefugeL"), -16.5f, 0.0f, -1.16f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D5_VaneRowL"), -15.5f, -11.6f, -1.09f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5F4LFx134"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D5_KiteLoftL"), -17.0f, 0.0f, -1.19f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D5_SkyBoard"), -18.0f, -9.4f, -1.26f, -8.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D5_LampCairn"), 19.5f, -11.6f, 1.37f, -30.0f, 1.0f },
});
Set(TEXT("WldS1D5RowARFx3"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D5_KiteLoft"), -15.5f, 0.0f, 1.09f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D5_Refuge"), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D5_LegCabin"), 15.5f, 0.5f, -1.09f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D5_Anemometer"), 8.9f, -3.5f, -0.62f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5VergeALFx02"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D5_VaneRowL"), -15.5f, -2.2f, -1.09f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D6R320Fx4"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D6_LuggageCart"), -22.62f, 0.8f, 1.58f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D6_FlowerRail"), 22.62f, 0.8f, -1.58f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D6_ChangingCabins"), -20.5f, 6.6f, 1.44f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D6_Fountain"), 20.5f, 6.6f, -1.44f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D6_SpaHotel"), 20.5f, 16.3f, -1.44f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D6_Funicular"), -20.5f, 39.6f, 1.44f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D6R332Fx1"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D6_GlobeLamp"), 22.62f, 6.6f, -1.58f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D6_Funicular"), -22.62f, 39.6f, 1.58f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D6R362Fx2"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D6_GlobeLamp"), -22.62f, 6.6f, 1.58f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D6_GlobeLamp"), 22.62f, 6.6f, -1.58f, 0.0f, 1.0f },
	{ TEXT("/Game/World/IceMountain/SM_S1D6_Kurhaus"), -22.62f, 41.0f, 1.58f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7CampCLFx1"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D7_Breaker"), -6.0f, 0.0f, -0.42f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7TallALFx1"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D7_LampPost"), -17.0f, 0.0f, -1.19f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7TallBLFx1"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D7_BellFrame"), -17.0f, 0.0f, -1.19f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7TallDRFx1"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D7_BellFrame"), -17.0f, 0.0f, 1.19f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D8BldBLFx0"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D8_RidgeShelter"), 20.5f, 0.5f, 1.44f, 12.0f, 1.0f },
});
Set(TEXT("WldS1D8BldDLFx1"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D8_LiftTop"), -20.5f, 0.0f, -1.44f, 12.0f, 1.0f },
});
Set(TEXT("WldS1D8BldELFx1"), {
	{ TEXT("/Game/World/IceMountain/SM_S1D8_HighBeacon"), -20.5f, 0.0f, -1.44f, 12.0f, 1.0f },
});
Set(TEXT("WldS1PEOPLE321LFx0"), {
	{ TEXT("/Game/World/IceMountain/SM_S1PEOPLE_D6_Baths"), -12.4f, 0.0f, -0.87f, 20.0f, 1.3f },
	{ TEXT("/Game/World/IceMountain/SM_S1PEOPLE_D6_Gallery"), 12.4f, 0.0f, 0.87f, 20.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE337LFx0"), {
	{ TEXT("/Game/World/IceMountain/SM_S1PEOPLE_D6_Gallery"), -12.4f, 0.0f, -0.87f, 12.0f, 1.3f },
	{ TEXT("/Game/World/IceMountain/SM_S1PEOPLE_D6_Quartet"), 12.4f, 0.0f, 0.87f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE353LFx0"), {
	{ TEXT("/Game/World/IceMountain/SM_S1PEOPLE_D6_Quartet"), -12.4f, 0.0f, -0.87f, 12.0f, 1.3f },
	{ TEXT("/Game/World/IceMountain/SM_S1PEOPLE_D6_Baths"), 12.4f, 0.0f, 0.87f, 12.0f, 1.3f },
});
Set(TEXT("WldS1PEOPLE369LFx0"), {
	{ TEXT("/Game/World/IceMountain/SM_S1PEOPLE_D6_Baths"), -12.4f, 0.0f, -0.87f, 20.0f, 1.3f },
	{ TEXT("/Game/World/IceMountain/SM_S1PEOPLE_D6_Gallery"), 12.4f, 0.0f, 0.87f, 20.0f, 1.3f },
});
Zone(TEXT("World s1fix, slot 16 (0.724 km)"), 0.70353f, 0.74425f);
	Site(TEXT("WldS1D1Needle"), 1, 112.0f, 1, 0, 1.f, 1.0f);     // THE SUMMIT NEEDLE, moved from slot 18 (line 3190) to 0.724 km, on the approach where its stem shows
Zone(TEXT("World s1fix, slot 100 (4.524 km)"), 4.50394f, 4.54466f);
	Site(TEXT("WldS1D2Hero"), -1, 114.0f, 1, 0, 1.f, 1.0f);     // THE BLACK DIAMOND, moved from slot 102 (line 3399): clear of the groomers strip
Zone(TEXT("World s1fix, slot 258 (11.673 km)"), 11.65233f, 11.69305f);
	Site(TEXT("WldS1D5VergeALFx02"), -1, 28.6f, 1, 0, 1.f, 1.0f);     // from line 4116, a part taken out
Zone(TEXT("World s1fix, slot 264 (11.944 km)"), 11.92379f, 11.96451f);
	Site(TEXT("WldS1D5RowAR"), 1, 55.0f, 1, 0, 1.f, 1.0f);     // kept from line 4104
Zone(TEXT("World s1fix, slot 266 (12.035 km)"), 12.01428f, 12.05500f);
	Site(TEXT("WldS1D5VergeAL"), -1, 28.6f, 1, 0, 1.f, 1.0f);     // kept from line 4116
Zone(TEXT("World s1fix, slot 268 (12.125 km)"), 12.10476f, 12.14548f);
	Site(TEXT("WldS1D5F4L"), -1, 38.0f, 1, 0, 1.f, 1.0f);     // kept from line 4115
Zone(TEXT("World s1fix, slot 272 (12.306 km)"), 12.28574f, 12.32646f);
	Site(TEXT("WldS1D5RowARFx3"), 1, 55.0f, 1, 0, 1.f, 1.0f);     // from line 4104, a part taken out
Zone(TEXT("World s1fix, slot 274 (12.397 km)"), 12.37622f, 12.41694f);
	Site(TEXT("WldS1D5VergeAL"), -1, 28.6f, 1, 0, 1.f, 1.0f);     // kept from line 4116
Zone(TEXT("World s1fix, slot 280 (12.668 km)"), 12.64768f, 12.68840f);
	Site(TEXT("WldS1D5RowAR"), 1, 55.0f, 1, 0, 1.f, 1.0f);     // kept from line 4104
Zone(TEXT("World s1fix, slot 282 (12.759 km)"), 12.73817f, 12.77889f);
	Site(TEXT("WldS1D5VergeAL"), -1, 28.6f, 1, 0, 1.f, 1.0f);     // kept from line 4116
Zone(TEXT("World s1fix, slot 284 (12.849 km)"), 12.82865f, 12.86937f);
	Site(TEXT("WldS1D5F4L"), -1, 38.0f, 1, 0, 1.f, 1.0f);     // kept from line 4115
Zone(TEXT("World s1fix, slot 288 (13.030 km)"), 13.00962f, 13.05034f);
	Site(TEXT("WldS1D5F1LFx123"), -1, 38.0f, 1, 0, 1.f, 1.0f);     // from line 4112, a part taken out
Zone(TEXT("World s1fix, slot 288 (13.030 km)"), 13.00962f, 13.05034f);
	Site(TEXT("WldS1D5RowAR"), 1, 55.0f, 1, 0, 1.f, 1.0f);     // kept from line 4104
Zone(TEXT("World s1fix, slot 290 (13.120 km)"), 13.10011f, 13.14083f);
	Site(TEXT("WldS1D5VergeAL"), -1, 28.6f, 1, 0, 1.f, 1.0f);     // kept from line 4116
Zone(TEXT("World s1fix, slot 296 (13.392 km)"), 13.37157f, 13.41229f);
	Site(TEXT("WldS1D5RowAR"), 1, 55.0f, 1, 0, 1.f, 1.0f);     // kept from line 4104
Zone(TEXT("World s1fix, slot 298 (13.482 km)"), 13.46205f, 13.50277f);
	Site(TEXT("WldS1D5VergeAL"), -1, 28.6f, 1, 0, 1.f, 1.0f);     // kept from line 4116
Zone(TEXT("World s1fix, slot 300 (13.573 km)"), 13.55254f, 13.59326f);
	Site(TEXT("WldS1D5F4LFx134"), -1, 38.0f, 1, 0, 1.f, 1.0f);     // from line 4115, a part taken out
Zone(TEXT("World s1fix, slot 304 (13.754 km)"), 13.73351f, 13.77423f);
	Site(TEXT("WldS1D5RowAR"), 1, 55.0f, 1, 0, 1.f, 1.0f);     // kept from line 4104
Zone(TEXT("World s1fix, slot 306 (13.844 km)"), 13.82400f, 13.86472f);
	Site(TEXT("WldS1D5VergeALFx02"), -1, 28.6f, 1, 0, 1.f, 1.0f);     // from line 4116, a part taken out
Zone(TEXT("World s1fix, slot 312 (14.116 km)"), 14.09546f, 14.13618f);
	Site(TEXT("WldS1D5RowARFx3"), 1, 55.0f, 1, 0, 1.f, 1.0f);     // from line 4104, a part taken out
Zone(TEXT("World s1fix, slot 314 (14.206 km)"), 14.18594f, 14.22666f);
	Site(TEXT("WldS1D5VergeAL"), -1, 28.6f, 1, 0, 1.f, 1.0f);     // kept from line 4116
Zone(TEXT("World s1fix, slot 376 (17.011 km)"), 16.99101f, 17.03173f);
	Site(TEXT("WldS1D7CampCLFx1"), -1, 54.5f, 1, 0, 1.f, 1.0f);     // from line 4280, a part taken out
Zone(TEXT("World s1fix, slot 382 (17.283 km)"), 17.26247f, 17.30319f);
	Site(TEXT("WldS1D7TallBL"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4302
Zone(TEXT("World s1fix, slot 382 (17.283 km)"), 17.26247f, 17.30319f);
	Site(TEXT("WldS1D7TallDRFx1"), 1, 27.6f, 1, 0, 1.f, 1.0f);     // from line 4304, a part taken out
Zone(TEXT("World s1fix, slot 384 (17.373 km)"), 17.35295f, 17.39367f);
	Site(TEXT("WldS1D7TallALFx1"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // from line 4301, a part taken out
Zone(TEXT("World s1fix, slot 386 (17.464 km)"), 17.44344f, 17.48416f);
	Site(TEXT("WldS1D7TallBLFx1"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // from line 4302, a part taken out
Zone(TEXT("World s1fix, slot 386 (17.464 km)"), 17.44344f, 17.48416f);
	Site(TEXT("WldS1D7TallDR"), 1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4304
Zone(TEXT("World s1fix, slot 388 (17.554 km)"), 17.53392f, 17.57464f);
	Site(TEXT("WldS1D7CampCL"), -1, 54.5f, 1, 0, 1.f, 1.0f);     // kept from line 4280
Zone(TEXT("World s1fix, slot 388 (17.554 km)"), 17.53392f, 17.57464f);
	Site(TEXT("WldS1D7TallAL"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4301
Zone(TEXT("World s1fix, slot 390 (17.645 km)"), 17.62441f, 17.66513f);
	Site(TEXT("WldS1D7TallBL"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4302
Zone(TEXT("World s1fix, slot 390 (17.645 km)"), 17.62441f, 17.66513f);
	Site(TEXT("WldS1D7TallDR"), 1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4304
Zone(TEXT("World s1fix, slot 392 (17.735 km)"), 17.71490f, 17.75562f);
	Site(TEXT("WldS1D7TallAL"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4301
Zone(TEXT("World s1fix, slot 394 (17.826 km)"), 17.80538f, 17.84610f);
	Site(TEXT("WldS1D7TallBL"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4302
Zone(TEXT("World s1fix, slot 394 (17.826 km)"), 17.80538f, 17.84610f);
	Site(TEXT("WldS1D7TallDR"), 1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4304
Zone(TEXT("World s1fix, slot 396 (17.916 km)"), 17.89587f, 17.93659f);
	Site(TEXT("WldS1D7TallAL"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4301
Zone(TEXT("World s1fix, slot 398 (18.007 km)"), 17.98635f, 18.02707f);
	Site(TEXT("WldS1D7TallBL"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4302
Zone(TEXT("World s1fix, slot 398 (18.007 km)"), 17.98635f, 18.02707f);
	Site(TEXT("WldS1D7TallDR"), 1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4304
Zone(TEXT("World s1fix, slot 400 (18.097 km)"), 18.07684f, 18.11756f);
	Site(TEXT("WldS1D7CampCL"), -1, 54.5f, 1, 0, 1.f, 1.0f);     // kept from line 4280
Zone(TEXT("World s1fix, slot 408 (18.459 km)"), 18.43878f, 18.47950f);
	Site(TEXT("WldS1D7TallALFx1"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // from line 4312, a part taken out
Zone(TEXT("World s1fix, slot 410 (18.550 km)"), 18.52927f, 18.56999f);
	Site(TEXT("WldS1D7TallBL"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4313
Zone(TEXT("World s1fix, slot 412 (18.640 km)"), 18.61976f, 18.66048f);
	Site(TEXT("WldS1D7CampCL"), -1, 54.5f, 1, 0, 1.f, 1.0f);     // kept from line 4280
Zone(TEXT("World s1fix, slot 412 (18.640 km)"), 18.61976f, 18.66048f);
	Site(TEXT("WldS1D7TallAL"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4312
Zone(TEXT("World s1fix, slot 414 (18.731 km)"), 18.71024f, 18.75096f);
	Site(TEXT("WldS1D7TallBLFx1"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // from line 4313, a part taken out
Zone(TEXT("World s1fix, slot 416 (18.821 km)"), 18.80073f, 18.84145f);
	Site(TEXT("WldS1D7TallAL"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4312
Zone(TEXT("World s1fix, slot 418 (18.912 km)"), 18.89121f, 18.93193f);
	Site(TEXT("WldS1D7TallBL"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4313
Zone(TEXT("World s1fix, slot 420 (19.002 km)"), 18.98170f, 19.02242f);
	Site(TEXT("WldS1D7TallAL"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4312
Zone(TEXT("World s1fix, slot 422 (19.093 km)"), 19.07219f, 19.11291f);
	Site(TEXT("WldS1D7TallBL"), -1, 27.6f, 1, 0, 1.f, 1.0f);     // kept from line 4313
Zone(TEXT("World s1fix, slot 424 (19.183 km)"), 19.16267f, 19.20339f);
	Site(TEXT("WldS1D7CampCL"), -1, 54.5f, 1, 0, 1.f, 1.0f);     // kept from line 4280
// ---- END s1fix ----
}();
