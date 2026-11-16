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
