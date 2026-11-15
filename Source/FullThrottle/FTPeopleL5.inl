// Ice Mountain (level index 5): the population layer. People, creatures and film crews, placed by rule like the dressing
// (the words and what their numbers mean are in the header of FTTrackDress.cpp; the rule kinds in FTDress.h).
// This table is read after FTDressL5.inl: start with a Zone line, and every rule here comes after every dressing
// rule, so the dressing's own placements never move. A mesh name without a leading slash is looked up in
// /Game/Maps/Glacier/Dress/; give a full path for the population's own meshes, e.g. TEXT("/Game/People/SM_CrowdCheerA").
// Rules that bind this layer: nothing over 2 m tall nearer than 24 m to a centre line and nothing at all nearer than
// 17 m; nothing lit that blinks nearer than 30 m; nothing that could be taken for a pickup, an enemy or an attack.
//
// THE CAST (15 Nov 2026, first round; the meshes are made by Content/Python/ft_people_ice.py, the crew's by ft_people_crew.py):
// racers on big-air hills of their own and on slalom courses down the real slopes, boarders in small pipes, crowds on
// scaffold stands, apres-ski decks, snowmen and the children building them, dog sleds, climbers on blue ice, roped
// parties and wolves on snow ridges, ibex on crags, eagles overhead; and the film crew with its snowmobile camera.
//
// What this table found out about the level (read FTDressL5.inl's head first):
//  - The chase camera looks ALONG the road: a piece is seen end-on for seconds and side-on for an instant. So the stands,
//    the decks and the ice faces are put down by sets that turn them 35 degrees towards the car coming down the road
//    (R sets: yaw -35; L sets: +35), and a big-air hill's slope faces up the road (SM_SkiHill_A on the right,
//    SM_SkiHillL_A on the left: twins, like the dressing's L pieces).
//  - The cel pass inks a life-size figure 30 m off to a black scribble: people here are 1.3 times life size by the road
//    and 2 times and more on the slopes, in plain hot colours.
//  - This hill's sight line pushes anything over about 1.4 m out to 24.7 m, so nothing of the cast is written nearer
//    than 25 m; the crew's low pieces are written at 25 to 26 m.
//  - SM_PisteRun_A is laid with its +X down the fall line (the set turns it -90) and wants the 12-in-100 slope under
//    it from 106 to 225 m: its anchor is 180 m out.
//  - Scatter and Site are left out of loops, corkscrews, pipes and jumps: there the cast is written by Far rules with
//    the land's own drop (stands, eagles), as the dressing does.
//  - The finale is the Abominable's road: no snowmen at all from 8.94 km on, and none nearer than 42 m before it.

#define ICE(Name) TEXT("/Game/People/IceMountain/" Name)
#define CREW(Name) TEXT("/Game/People/Crew/SM_Crew" Name "_Snow")
Set(TEXT("PplStandAR"), { { ICE("SM_SnowStand_A"), 0, 0, 0, -35, 1 } });
Set(TEXT("PplStandAL"), { { ICE("SM_SnowStand_A"), 0, 0, 0, 35, 1 } });
Set(TEXT("PplStandBR"), { { ICE("SM_SnowStand_B"), 0, 0, 0, -35, 1 } });
Set(TEXT("PplStandBL"), { { ICE("SM_SnowStand_B"), 0, 0, 0, 35, 1 } });
Set(TEXT("PplDeckR"), { { ICE("SM_LodgeDeck_A"), 0, 0, 0, -30, 1 } });
Set(TEXT("PplDeckL"), { { ICE("SM_LodgeDeck_A"), 0, 0, 0, 30, 1 } });
Set(TEXT("PplClimbR"), { { ICE("SM_IceClimb_A"), 0, 0, 0, -35, 1 } });
Set(TEXT("PplClimbL"), { { ICE("SM_IceClimb_A"), 0, 0, 0, 35, 1 } });
Set(TEXT("PplPisteRun"), { { ICE("SM_PisteRun_A"), 0, 0, 0, -90, 1 } });
// The crew by the start: one cluster a side (the anchor is 26 m out).
Set(TEXT("PplUnitR"), {
	{ CREW("Camera"),     0,   0, 0, -20, 1 },
	{ CREW("Crane"),    -14,   8, 0,   0, 1 },
	{ CREW("Lights"),    13,  10, 0, -20, 1 },
	{ CREW("Catering"),  -2,  20, 0,   0, 1 },
});
Set(TEXT("PplUnitL"), {
	{ CREW("Village"),    0,   0, 0,  20, 1 },
	{ CREW("Sound"),    -12,   7, 0,   0, 1 },
	{ CREW("Lights"),    13,  10, 0,  20, 1 },
	{ ICE("SM_CrewSkidoo"), 12, 0, 0, 20, 1 },
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

// ---- The crew: by the start, at the jumps, the loops and the corkscrews, and before the Abominable's road.
Zone(TEXT("People: the unit by the start"), 0.0f, 0.1f);
	Site(TEXT("PplUnitR"), 1, 26.f, 2, 1, 1.f);     // slot 1
	Site(TEXT("PplUnitL"), -1, 26.f, 2, 0, 1.f);     // slot 2
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 2, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 2, 1, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: the crew past the summit street"), 0.16f, 0.22f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(CREW("Stunt"), 25.f, 26.f, 1, 0, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: the crew at the first jump"), 1.98f, 2.1f);
	Site(TEXT("PplShotR"), 1, 27.f, 2, 0, 1.f);
	Site(TEXT("PplShotL"), -1, 27.f, 2, 1, 1.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 2, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 2, 0, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: the crew where the first jump lands"), 2.4f, 2.5f);
	Site(TEXT("PplShotR"), 1, 27.f, 2, 0, 1.f);
	Site(TEXT("PplShotL"), -1, 27.f, 2, 1, 1.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 2, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 2, 0, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: the crew at Black diamond's loop"), 3.96f, 4.08f);
	Site(TEXT("PplShotR"), 1, 27.f, 2, 0, 1.f);
	Site(TEXT("PplShotL"), -1, 27.f, 2, 1, 1.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 2, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 2, 0, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: the crew at the second jump"), 4.72f, 4.83f);
	Site(TEXT("PplShotR"), 1, 27.f, 2, 0, 1.f);
	Site(TEXT("PplShotL"), -1, 27.f, 2, 1, 1.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 2, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 2, 0, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: the crew at the whiteout's loop"), 6.74f, 6.86f);
	Site(TEXT("PplShotR"), 1, 27.f, 2, 0, 1.f);
	Site(TEXT("PplShotL"), -1, 27.f, 2, 1, 1.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 2, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 2, 0, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: the crew at the third jump"), 7.87f, 7.98f);
	Site(TEXT("PplShotR"), 1, 27.f, 2, 0, 1.f);
	Site(TEXT("PplShotL"), -1, 27.f, 2, 1, 1.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 2, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 2, 0, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("People: the unit before the Abominable"), 8.8f, 8.93f);
	Site(TEXT("PplUnitR"), 1, 26.f, 2, 0, 1.f);
	Site(TEXT("PplUnitL"), -1, 26.f, 2, 1, 1.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 1, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);

// ---- Under the loops and the corkscrews (Far rules: they stay).
Zone(TEXT("People: under the chute's corkscrew"), 2.51f, 2.685f);
	Far(ICE("SM_SnowStand_A"), 34.f, 35.f, 3, 0, 1.f, 1.f, 0.f, 1, 0.74f, 0.f);
	Far(ICE("SM_SnowStand_B"), 34.f, 35.f, 3, 1, 1.f, 1.f, 0.f, -1, 0.74f, 0.f);
	Far(ICE("SM_Eagles_A"), 40.f, 90.f, 4, 0, 1.f, 1.f, 0.f, 1, 0.6f, 0.f);
	Far(ICE("SM_Eagles_A"), 40.f, 90.f, 4, 2, 1.f, 1.f, 0.f, -1, 0.6f, 0.f);
	Far(ICE("SM_WolfRidge_A"), 100.f, 108.f, 3, 2, 1.f, 1.f, 0.f, 0, -2.5f, 0.f);

Zone(TEXT("People: under Black diamond's loop"), 4.085f, 4.37f);
	Far(ICE("SM_SnowStand_A"), 34.f, 35.f, 3, 1, 1.f, 1.f, 0.f, 1, 0.74f, 0.f);
	Far(ICE("SM_SnowStand_B"), 34.f, 35.f, 3, 2, 1.f, 1.f, 0.f, -1, 0.74f, 0.f);
	Far(ICE("SM_Eagles_A"), 40.f, 90.f, 4, 0, 1.f, 1.f, 0.f, 1, 0.6f, 0.f);
	Far(ICE("SM_Eagles_A"), 40.f, 90.f, 4, 2, 1.f, 1.f, 0.f, -1, 0.6f, 0.f);
	Far(ICE("SM_WolfRidge_A"), 100.f, 108.f, 3, 0, 1.f, 1.f, 0.f, 0, -2.5f, 0.f);

Zone(TEXT("People: under Black diamond's corkscrew"), 5.225f, 5.4f);
	Far(ICE("SM_SnowStand_A"), 34.f, 35.f, 3, 2, 1.f, 1.f, 0.f, 1, 0.74f, 0.f);
	Far(ICE("SM_SnowStand_B"), 34.f, 35.f, 3, 0, 1.f, 1.f, 0.f, -1, 0.74f, 0.f);
	Far(ICE("SM_Eagles_A"), 40.f, 90.f, 4, 0, 1.f, 1.f, 0.f, 1, 0.6f, 0.f);
	Far(ICE("SM_Eagles_A"), 40.f, 90.f, 4, 2, 1.f, 1.f, 0.f, -1, 0.6f, 0.f);
	Far(ICE("SM_WolfRidge_A"), 100.f, 108.f, 3, 1, 1.f, 1.f, 0.f, 0, -2.5f, 0.f);

Zone(TEXT("People: under the whiteout's loop"), 6.87f, 7.155f);
	Far(ICE("SM_SnowStand_A"), 34.f, 35.f, 3, 0, 1.f, 1.f, 0.f, 1, 0.74f, 0.f);
	Far(ICE("SM_SnowStand_B"), 34.f, 35.f, 3, 1, 1.f, 1.f, 0.f, -1, 0.74f, 0.f);
	Far(ICE("SM_Eagles_A"), 40.f, 90.f, 4, 0, 1.f, 1.f, 0.f, 1, 0.6f, 0.f);
	Far(ICE("SM_Eagles_A"), 40.f, 90.f, 4, 2, 1.f, 1.f, 0.f, -1, 0.6f, 0.f);
	Far(ICE("SM_WolfRidge_A"), 100.f, 108.f, 3, 2, 1.f, 1.f, 0.f, 0, -2.5f, 0.f);

Zone(TEXT("People: under the valley's corkscrew"), 7.215f, 7.385f);
	Far(ICE("SM_SnowStand_A"), 34.f, 35.f, 3, 1, 1.f, 1.f, 0.f, 1, 0.74f, 0.f);
	Far(ICE("SM_SnowStand_B"), 34.f, 35.f, 3, 2, 1.f, 1.f, 0.f, -1, 0.74f, 0.f);
	Far(ICE("SM_Eagles_A"), 40.f, 90.f, 4, 0, 1.f, 1.f, 0.f, 1, 0.6f, 0.f);
	Far(ICE("SM_Eagles_A"), 40.f, 90.f, 4, 2, 1.f, 1.f, 0.f, -1, 0.6f, 0.f);
	Far(ICE("SM_WolfRidge_A"), 100.f, 108.f, 3, 0, 1.f, 1.f, 0.f, 0, -2.5f, 0.f);

// ---- The districts, as the dressing cuts them.
Zone(TEXT("People: First tracks, the top of the resort"), 0.24f, 1.19f);
	Site(TEXT("PplStandAR"), 1, 34.f, 6, 0, 1.f);     // crowds on stands, turned to the car coming
	Site(TEXT("PplStandBL"), -1, 34.f, 6, 2, 1.f);
	Scatter(ICE("SM_SkiHill_A"), 48.f, 50.f, 8, 1, 1.f, 1.f, 0.f, 1, 0.f);     // big-air hills: racers come down towards the car
	Scatter(ICE("SM_SkiHillL_A"), 48.f, 50.f, 8, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_BoardPipe_A"), 44.f, 46.f, 12, 3, 1.f, 1.f, 0.f, -1, 0.f);     // boarders swinging in a pipe
	Scatter(ICE("SM_BoardPipe_A"), 78.f, 82.f, 12, 9, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 7, 4, 1.f);     // slalom racers down the real slope
	Site(TEXT("PplPisteRun"), 1, 180.f, 7, 1, 1.f);
	Scatter(ICE("SM_DogSled_A"), 62.f, 68.f, 20, 6, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_DogSled_A"), 84.f, 90.f, 20, 16, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 16, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 16, 10, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_Snowmen_A"), 57.f, 60.f, 9, 6, 1.f, 1.f, 0.f, 1, 20.f);     // snowmen (never nearer than 42 m)
	Scatter(ICE("SM_Snowmen_A"), 56.f, 60.f, 9, 1, 1.f, 1.f, 0.f, -1, 20.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 25, 1, 1.f, 1.f, 0.f, 1, 0.f);     // the crew, thinly: applauding at the tape
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 25, 13, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 3, 1.f, 1.f, 0.f, 1, 0.f);     // the snowmobile camera
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 23, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 37, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 31, 12, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The half pipe: competition day"), 1.19f, 2.43f);
	Site(TEXT("PplStandAR"), 1, 34.f, 4, 3, 1.f);     // crowds on stands, turned to the car coming
	Site(TEXT("PplStandBL"), -1, 34.f, 4, 1, 1.f);
	Scatter(ICE("SM_SkiHill_A"), 48.f, 50.f, 8, 4, 1.f, 1.f, 0.f, 1, 0.f);     // big-air hills: racers come down towards the car
	Scatter(ICE("SM_SkiHillL_A"), 48.f, 50.f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_BoardPipe_A"), 44.f, 46.f, 12, 6, 1.f, 1.f, 0.f, -1, 0.f);     // boarders swinging in a pipe
	Scatter(ICE("SM_BoardPipe_A"), 78.f, 82.f, 12, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 7, 0, 1.f);     // slalom racers down the real slope
	Site(TEXT("PplPisteRun"), 1, 180.f, 7, 4, 1.f);
	Scatter(ICE("SM_DogSled_A"), 62.f, 68.f, 20, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_DogSled_A"), 84.f, 90.f, 20, 19, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 16, 5, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 16, 13, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_Snowmen_A"), 57.f, 60.f, 9, 0, 1.f, 1.f, 0.f, 1, 20.f);     // snowmen (never nearer than 42 m)
	Scatter(ICE("SM_Snowmen_A"), 56.f, 60.f, 9, 4, 1.f, 1.f, 0.f, -1, 20.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 25, 4, 1.f, 1.f, 0.f, 1, 0.f);     // the crew, thinly: applauding at the tape
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 25, 16, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 6, 1.f, 1.f, 0.f, 1, 0.f);     // the snowmobile camera
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 26, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 37, 10, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 31, 15, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: Avalanche chute"), 2.43f, 2.96f);
	Scatter(ICE("SM_Eagles_A"), 50.f, 110.f, 3, 0, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(ICE("SM_Ibex_A"), 62.f, 80.f, 4, 1, 1.f, 1.f, 0.f, 0, 25.f);
	Scatter(ICE("SM_RopeTeam_A"), 150.f, 230.f, 4, 2, 1.f, 1.5f, 0.f, 0, 0.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 25, 2, 1.f, 1.f, 0.f, 1, 0.f);     // the crew, thinly: applauding at the tape
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 25, 14, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 4, 1.f, 1.f, 0.f, 1, 0.f);     // the snowmobile camera
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 24, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 37, 8, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 31, 13, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The super pipe: the glacier"), 2.96f, 4.02f);
	Site(TEXT("PplClimbR"), 1, 29.f, 4, 0, 1.f);     // climbers on blue ice, in front of the ice walls
	Site(TEXT("PplClimbL"), -1, 29.f, 4, 2, 1.f);
	Site(TEXT("PplStandBR"), 1, 28.f, 8, 5, 1.f);
	Site(TEXT("PplStandAL"), -1, 28.f, 8, 3, 1.f);
	Scatter(ICE("SM_RopeTeam_A"), 150.f, 230.f, 12, 5, 1.f, 1.5f, 0.f, -1, 0.f);     // a roped party and wolves, where the walls leave a gap
	Scatter(ICE("SM_WolfRidge_A"), 130.f, 210.f, 14, 10, 1.f, 1.4f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 60.f, 130.f, 14, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 60.f, 130.f, 14, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 25, 1, 1.f, 1.f, 0.f, 1, 0.f);     // the crew, thinly: applauding at the tape
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 25, 13, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 3, 1.f, 1.f, 0.f, 1, 0.f);     // the snowmobile camera
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 23, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 37, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 31, 12, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: Black diamond: the exposed ridge"), 4.02f, 5.65f);
	Scatter(ICE("SM_Ibex_A"), 36.f, 44.f, 10, 0, 1.f, 1.f, 0.f, 1, 25.f);     // ibex on crags
	Scatter(ICE("SM_Ibex_A"), 36.f, 44.f, 10, 5, 1.f, 1.f, 0.f, -1, 25.f);
	Scatter(ICE("SM_WolfRidge_A"), 84.f, 120.f, 13, 2, 1.f, 1.f, 0.f, -1, 0.f);     // wolves along the ridges
	Scatter(ICE("SM_WolfRidge_A"), 90.f, 160.f, 13, 8, 1.f, 1.3f, 0.f, 1, 0.f);
	Scatter(ICE("SM_RopeTeam_A"), 100.f, 140.f, 12, 3, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_RopeTeam_A"), 150.f, 240.f, 12, 8, 1.f, 1.5f, 0.f, -1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 50.f, 120.f, 12, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 50.f, 120.f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_DogSled_A"), 30.f, 32.f, 12, 5, 1.f, 1.f, 0.f, 1, 0.f);     // dog sleds along the fence line
	Scatter(ICE("SM_DogSled_A"), 30.f, 32.f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplStandAR"), 1, 34.f, 9, 7, 1.f);
	Site(TEXT("PplStandBL"), -1, 34.f, 9, 3, 1.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 11, 6, 1.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 25, 1, 1.f, 1.f, 0.f, 1, 0.f);     // the crew, thinly: applauding at the tape
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 25, 13, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 3, 1.f, 1.f, 0.f, 1, 0.f);     // the snowmobile camera
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 23, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 37, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 31, 12, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The whiteout: every gate on the mountain"), 5.65f, 7.3f);
	Site(TEXT("PplStandAR"), 1, 34.f, 4, 1, 1.f);     // crowds on stands, turned to the car coming
	Site(TEXT("PplStandBL"), -1, 34.f, 4, 3, 1.f);
	Scatter(ICE("SM_SkiHill_A"), 48.f, 50.f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);     // big-air hills: racers come down towards the car
	Scatter(ICE("SM_SkiHillL_A"), 48.f, 50.f, 8, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_BoardPipe_A"), 44.f, 46.f, 12, 8, 1.f, 1.f, 0.f, -1, 0.f);     // boarders swinging in a pipe
	Scatter(ICE("SM_BoardPipe_A"), 78.f, 82.f, 12, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 7, 2, 1.f);     // slalom racers down the real slope
	Site(TEXT("PplPisteRun"), 1, 180.f, 7, 6, 1.f);
	Scatter(ICE("SM_DogSled_A"), 62.f, 68.f, 20, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_DogSled_A"), 84.f, 90.f, 20, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 16, 7, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 16, 15, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 25, 6, 1.f, 1.f, 0.f, 1, 0.f);     // the crew, thinly: applauding at the tape
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 25, 18, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 8, 1.f, 1.f, 0.f, 1, 0.f);     // the snowmobile camera
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 28, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 37, 12, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 31, 17, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The valley run"), 7.3f, 8.94f);
	Site(TEXT("PplStandAR"), 1, 34.f, 6, 1, 1.f);     // crowds on stands, turned to the car coming
	Site(TEXT("PplStandBL"), -1, 34.f, 6, 3, 1.f);
	Scatter(ICE("SM_SkiHill_A"), 48.f, 50.f, 8, 2, 1.f, 1.f, 0.f, 1, 0.f);     // big-air hills: racers come down towards the car
	Scatter(ICE("SM_SkiHillL_A"), 48.f, 50.f, 8, 6, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_BoardPipe_A"), 44.f, 46.f, 12, 4, 1.f, 1.f, 0.f, -1, 0.f);     // boarders swinging in a pipe
	Scatter(ICE("SM_BoardPipe_A"), 78.f, 82.f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 7, 5, 1.f);     // slalom racers down the real slope
	Site(TEXT("PplPisteRun"), 1, 180.f, 7, 2, 1.f);
	Scatter(ICE("SM_DogSled_A"), 62.f, 68.f, 20, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_DogSled_A"), 84.f, 90.f, 20, 17, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 16, 3, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 16, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplDeckR"), 1, 36.f, 10, 8, 1.f);     // apres-ski decks
	Site(TEXT("PplDeckL"), -1, 36.f, 10, 5, 1.f);
	Scatter(ICE("SM_Snowmen_A"), 57.f, 60.f, 9, 7, 1.f, 1.f, 0.f, 1, 20.f);     // snowmen (never nearer than 42 m)
	Scatter(ICE("SM_Snowmen_A"), 56.f, 60.f, 9, 2, 1.f, 1.f, 0.f, -1, 20.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 25, 2, 1.f, 1.f, 0.f, 1, 0.f);     // the crew, thinly: applauding at the tape
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 25, 14, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 4, 1.f, 1.f, 0.f, 1, 0.f);     // the snowmobile camera
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 24, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 37, 8, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 31, 13, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The Abominable's glacier"), 8.94f, 11.6f);
	Site(TEXT("PplClimbR"), 1, 29.f, 4, 2, 1.f);     // climbers on blue ice, in front of the ice walls
	Site(TEXT("PplClimbL"), -1, 29.f, 4, 0, 1.f);
	Site(TEXT("PplStandBR"), 1, 28.f, 8, 7, 1.f);
	Site(TEXT("PplStandAL"), -1, 28.f, 8, 5, 1.f);
	Scatter(ICE("SM_RopeTeam_A"), 150.f, 230.f, 12, 7, 1.f, 1.5f, 0.f, -1, 0.f);     // a roped party and wolves, where the walls leave a gap
	Scatter(ICE("SM_WolfRidge_A"), 130.f, 210.f, 14, 12, 1.f, 1.4f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 60.f, 130.f, 14, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 60.f, 130.f, 14, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 25, 3, 1.f, 1.f, 0.f, 1, 0.f);     // the crew, thinly: applauding at the tape
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 25, 15, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 5, 1.f, 1.f, 0.f, 1, 0.f);     // the snowmobile camera
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 25, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 37, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 31, 14, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The white ridge"), 11.6f, 14.3f);
	Scatter(ICE("SM_Ibex_A"), 36.f, 44.f, 10, 3, 1.f, 1.f, 0.f, 1, 25.f);     // ibex on crags
	Scatter(ICE("SM_Ibex_A"), 36.f, 44.f, 10, 8, 1.f, 1.f, 0.f, -1, 25.f);
	Scatter(ICE("SM_WolfRidge_A"), 84.f, 120.f, 13, 5, 1.f, 1.f, 0.f, -1, 0.f);     // wolves along the ridges
	Scatter(ICE("SM_WolfRidge_A"), 90.f, 160.f, 13, 11, 1.f, 1.3f, 0.f, 1, 0.f);
	Scatter(ICE("SM_RopeTeam_A"), 100.f, 140.f, 12, 6, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_RopeTeam_A"), 150.f, 240.f, 12, 11, 1.f, 1.5f, 0.f, -1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 50.f, 120.f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 50.f, 120.f, 12, 10, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_DogSled_A"), 30.f, 32.f, 12, 8, 1.f, 1.f, 0.f, 1, 0.f);     // dog sleds along the fence line
	Scatter(ICE("SM_DogSled_A"), 30.f, 32.f, 12, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplStandAR"), 1, 34.f, 9, 1, 1.f);
	Site(TEXT("PplStandBL"), -1, 34.f, 9, 6, 1.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 11, 9, 1.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 25, 4, 1.f, 1.f, 0.f, 1, 0.f);     // the crew, thinly: applauding at the tape
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 25, 16, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 6, 1.f, 1.f, 0.f, 1, 0.f);     // the snowmobile camera
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 26, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 37, 10, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 31, 15, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The resort"), 14.3f, 17.0f);
	Site(TEXT("PplStandAR"), 1, 34.f, 6, 2, 1.f);     // crowds on stands, turned to the car coming
	Site(TEXT("PplStandBL"), -1, 34.f, 6, 4, 1.f);
	Scatter(ICE("SM_SkiHill_A"), 40.f, 41.f, 8, 3, 1.f, 1.f, 0.f, 1, 0.f);     // big-air hills: racers come down towards the car
	Scatter(ICE("SM_SkiHillL_A"), 48.f, 50.f, 8, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_BoardPipe_A"), 44.f, 46.f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);     // boarders swinging in a pipe
	Scatter(ICE("SM_BoardPipe_A"), 78.f, 82.f, 12, 11, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 7, 6, 1.f);     // slalom racers down the real slope
	Site(TEXT("PplPisteRun"), 1, 180.f, 7, 3, 1.f);
	Scatter(ICE("SM_DogSled_A"), 62.f, 68.f, 20, 8, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_DogSled_A"), 84.f, 90.f, 20, 18, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 16, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 16, 12, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplDeckR"), 1, 36.f, 10, 9, 1.f);     // apres-ski decks
	Site(TEXT("PplDeckL"), -1, 36.f, 10, 6, 1.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 25, 3, 1.f, 1.f, 0.f, 1, 0.f);     // the crew, thinly: applauding at the tape
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 25, 15, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 5, 1.f, 1.f, 0.f, 1, 0.f);     // the snowmobile camera
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 25, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 37, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 31, 14, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The ice again"), 17.0f, 19.7f);
	Site(TEXT("PplClimbR"), 1, 29.f, 4, 0, 1.f);     // climbers on blue ice, in front of the ice walls
	Site(TEXT("PplClimbL"), -1, 29.f, 4, 2, 1.f);
	Site(TEXT("PplStandBR"), 1, 28.f, 8, 1, 1.f);
	Site(TEXT("PplStandAL"), -1, 28.f, 8, 7, 1.f);
	Scatter(ICE("SM_RopeTeam_A"), 150.f, 230.f, 12, 9, 1.f, 1.5f, 0.f, -1, 0.f);     // a roped party and wolves, where the walls leave a gap
	Scatter(ICE("SM_WolfRidge_A"), 130.f, 210.f, 14, 0, 1.f, 1.4f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 60.f, 130.f, 14, 6, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 60.f, 130.f, 14, 13, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 25, 5, 1.f, 1.f, 0.f, 1, 0.f);     // the crew, thinly: applauding at the tape
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 25, 17, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 7, 1.f, 1.f, 0.f, 1, 0.f);     // the snowmobile camera
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 27, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 37, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 31, 16, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The high ridge"), 19.7f, 22.4f);
	Scatter(ICE("SM_Ibex_A"), 36.f, 44.f, 10, 5, 1.f, 1.f, 0.f, 1, 25.f);     // ibex on crags
	Scatter(ICE("SM_Ibex_A"), 36.f, 44.f, 10, 0, 1.f, 1.f, 0.f, -1, 25.f);
	Scatter(ICE("SM_WolfRidge_A"), 84.f, 120.f, 13, 7, 1.f, 1.f, 0.f, -1, 0.f);     // wolves along the ridges
	Scatter(ICE("SM_WolfRidge_A"), 90.f, 160.f, 13, 0, 1.f, 1.3f, 0.f, 1, 0.f);
	Scatter(ICE("SM_RopeTeam_A"), 100.f, 140.f, 12, 8, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_RopeTeam_A"), 150.f, 240.f, 12, 1, 1.f, 1.5f, 0.f, -1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 50.f, 120.f, 12, 6, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 50.f, 120.f, 12, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_DogSled_A"), 30.f, 32.f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);     // dog sleds along the fence line
	Scatter(ICE("SM_DogSled_A"), 30.f, 32.f, 12, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplStandAR"), 1, 34.f, 9, 3, 1.f);
	Site(TEXT("PplStandBL"), -1, 34.f, 9, 8, 1.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 11, 0, 1.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 25, 6, 1.f, 1.f, 0.f, 1, 0.f);     // the crew, thinly: applauding at the tape
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 25, 18, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 8, 1.f, 1.f, 0.f, 1, 0.f);     // the snowmobile camera
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 28, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 37, 12, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 31, 17, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: The last lifts"), 22.4f, 99.0f);
	Site(TEXT("PplStandAR"), 1, 34.f, 6, 0, 1.f);     // crowds on stands, turned to the car coming
	Site(TEXT("PplStandBL"), -1, 34.f, 6, 2, 1.f);
	Scatter(ICE("SM_SkiHill_A"), 40.f, 41.f, 8, 7, 1.f, 1.f, 0.f, 1, 0.f);     // big-air hills: racers come down towards the car
	Scatter(ICE("SM_SkiHillL_A"), 48.f, 50.f, 8, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_BoardPipe_A"), 44.f, 46.f, 12, 9, 1.f, 1.f, 0.f, -1, 0.f);     // boarders swinging in a pipe
	Scatter(ICE("SM_BoardPipe_A"), 78.f, 82.f, 12, 3, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplPisteRun"), -1, 180.f, 7, 3, 1.f);     // slalom racers down the real slope
	Site(TEXT("PplPisteRun"), 1, 180.f, 7, 0, 1.f);
	Scatter(ICE("SM_DogSled_A"), 62.f, 68.f, 20, 12, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_DogSled_A"), 84.f, 90.f, 20, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 16, 8, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(ICE("SM_Eagles_A"), 70.f, 130.f, 16, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplDeckR"), 1, 36.f, 10, 3, 1.f);     // apres-ski decks
	Site(TEXT("PplDeckL"), -1, 36.f, 10, 0, 1.f);
	Scatter(CREW("ApplauseA"), 25.f, 26.f, 25, 7, 1.f, 1.f, 0.f, 1, 0.f);     // the crew, thinly: applauding at the tape
	Scatter(CREW("ApplauseB"), 25.f, 26.f, 25, 19, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 9, 1.f, 1.f, 0.f, 1, 0.f);     // the snowmobile camera
	Scatter(ICE("SM_CrewSkidoo"), 25.f, 26.f, 40, 29, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Dolly"), 25.f, 26.f, 37, 13, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Camera"), 25.f, 26.f, 31, 18, 1.f, 1.f, 0.f, 1, 0.f);

#undef ICE
#undef CREW
