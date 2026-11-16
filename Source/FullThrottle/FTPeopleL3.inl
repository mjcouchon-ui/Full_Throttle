// Volcano (level index 3): the population layer. People, creatures and film crews, placed by rule like the dressing
// (the words and what their numbers mean are in the header of FTTrackDress.cpp; the rule kinds in FTDress.h).
// This table is read after FTDressL3.inl: start with a Zone line, and every rule here comes after every dressing
// rule, so the dressing's own placements never move. A mesh name without a leading slash is looked up in
// /Game/Maps/Volcano/Dress/; give a full path for the population's own meshes, e.g. TEXT("/Game/People/SM_CrowdCheerA").
// Rules that bind this layer: nothing over 2 m tall nearer than 24 m to a centre line and nothing at all nearer than
// 17 m; nothing lit that blinks nearer than 30 m; nothing that could be taken for a pickup, an enemy or an attack.
//
// THE POPULATION PASS, round 1 (15 Nov 2026). The meshes are made by Content/Python/ft_people_volcano.py
// (/Game/People/Volcano/) and the film crew's by ft_people_crew.py (/Game/People/Crew/SM_Crew<Piece>_Heat).
// How the land lies for people here (FTDressL3.inl says it for the dressing): the barrier is 1.6 m tall, the land is
// 0.75 m below the road out to 90 m, and a kerb of basalt stubs up to 2.2 m runs 21 m out on both sides. A person on
// the ground nearer than about 33 m shows a head at most. So:
//  - NEAR (28 m): everyone stands on a trestle deck 1.8 m up (placed 15 % over life size), open underneath (the lava channels behind it stay in
//    sight). Those pieces are tall, so they are written at 28 m and more; the fire crews on the right at 34 m, so
//    that their water falls at the right-hand channel's edge (46 m). The film crew's own pieces (all built to stand on
//    the ground) are stood ON a rostrum by the Ppl sets: Up 1.8 m.
//  - Not beside the banked bends (the land is cut away there and anything at 17 to 40 m floats: FTDressL3.inl), so the
//    near rules are written in zones of their own that leave the bends out.
//  - Not on the right between 2.4 and 3.9 km (the eruption is watched over that side): nothing of this layer at all.
//  - ENGINE 13 is a red fire engine. The fire crews here are people on foot in yellow with pumps and hose, nothing red,
//    no vehicle; and none of them stands on the boss's road (8.17 km on) nearer than 95 m.
//  - MIDDLE (70 to 105 m, past the channels and their walls): stands and crags half as big again, fire crews at the
//    fissures. FAR (128 to 236 m, on the slopes, by Far rules with the land's own drop so that they stay through the
//    loops and jumps): caravans, herds, the camps, twice life size, on the always-moving master. Flocks in the sky.
//  - The shoot: by the start, before the jumps and the loops, before the boss's road, and thinly in every district.
// The table is written by a script (the worker's scratch): keep the zones' own slot cycles when editing by hand, they
// are what keeps two near pieces out of one slot.

#define VP(Name) TEXT("/Game/People/Volcano/" Name)
#define VC(Name) TEXT("/Game/People/Crew/" Name "_Heat")

// The shoot's sets: the crew kit's pieces stood on the rostrum (its deck is 1.8 m up, 15 m long, 5.6 m deep).
Set(TEXT("PplShootA"), {    // the camera position and crew at the tape
	{ VP("SM_VolRostrum"),      0,     0,    0,    0, 1 },
	{ VC("SM_CrewCamera"),     -3.6f,  0.6f, 1.8f, 0, 1 },
	{ VC("SM_CrewApplauseA"),   3.4f, -0.2f, 1.8f, 0, 1 },
});
Set(TEXT("PplShootB"), {    // the dolly on its rails
	{ VP("SM_VolRostrum"),      0,     0,    0,    0, 1 },
	{ VC("SM_CrewDolly"),       0,     0.6f, 1.8f, 0, 1 },
});
Set(TEXT("PplShootC"), {    // the director's village
	{ VP("SM_VolRostrum"),      0,     0,    0,    0, 1 },
	{ VC("SM_CrewVillage"),    -4.f,   0.8f, 1.8f, 0, 1 },
	{ VC("SM_CrewApplauseB"),   3.4f, -0.2f, 1.8f, 0, 1 },
});
Set(TEXT("PplShootD"), {    // the stunt team and sound
	{ VP("SM_VolRostrum"),      0,     0,    0,    0, 1 },
	{ VC("SM_CrewStunt"),      -3.2f,  0.9f, 1.8f, 0, 1 },
	{ VC("SM_CrewSound"),       4.8f,  0.2f, 1.8f, 0, 1 },
});
Set(TEXT("PplShootE"), {    // catering and the lights
	{ VP("SM_VolRostrum"),      0,     0,    0,    0, 1 },
	{ VC("SM_CrewCatering"),   -4.2f,  0.4f, 1.8f, 0, 1 },
	{ VC("SM_CrewLights"),      3.8f,  0.5f, 1.8f, 0, 1 },
});


// ---- THE SHOOT: by the start, before the jumps and loops, before the boss's road. The crane stands on the ground
// (its arm reaches 5.6 m towards the road: 31 m on the left, 40 m on the right, past the fumaroles' band).

Zone(TEXT("People: the shoot at the start"), 0.f, 0.32f);
	Site(TEXT("PplShootA"), 1, 28.8f, 7, 1, 1.f);    // the first thing passed: the camera position
	Site(TEXT("PplShootC"), -1, 28.8f, 7, 1, 1.f);
	Scatter(VC("SM_CrewCrane"), 40.f, 41.f, 7, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplShootD"), -1, 28.8f, 7, 2, 1.f);
	Site(TEXT("PplShootB"), 1, 28.8f, 7, 3, 1.f);
	Site(TEXT("PplShootE"), -1, 28.8f, 7, 3, 1.f);
	Site(TEXT("PplShootC"), 1, 28.8f, 7, 4, 1.f);
	Scatter(VC("SM_CrewCrane"), 31.f, 31.5f, 7, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplShootA"), -1, 28.8f, 7, 5, 1.f);
	Scatter(VP("SM_VolWatch_A"), 28.4f, 28.8f, 7, 5, 1.f, 1.f, 0.f, 1, 0.f);    // the first villagers: a stand of them, watching the shoot
	Site(TEXT("PplShootD"), 1, 28.8f, 7, 6, 1.f);
	Scatter(VP("SM_VolWatch_B"), 28.4f, 28.8f, 7, 6, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplShootB"), -1, 28.8f, 7, 0, 1.f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 7, 0, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: the shoot before the set piece at 0.63 km"), 0.5f, 0.63f);
	Site(TEXT("PplShootD"), -1, 28.8f, 3, 0, 1.f);
	Site(TEXT("PplShootB"), 1, 28.8f, 3, 1, 1.f);
	Scatter(VC("SM_CrewCrane"), 40.f, 41.f, 3, 2, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: the shoot before the set piece at 1.35 km"), 1.24f, 1.35f);
	Site(TEXT("PplShootA"), -1, 28.8f, 3, 0, 1.f);
	Scatter(VC("SM_CrewCrane"), 31.f, 31.5f, 3, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplShootC"), 1, 28.8f, 3, 1, 1.f);

Zone(TEXT("People: the shoot before the set piece at 2.34 km"), 2.24f, 2.34f);
	Site(TEXT("PplShootB"), -1, 28.8f, 3, 0, 1.f);
	Site(TEXT("PplShootE"), 1, 28.8f, 3, 1, 1.f);
	Scatter(VC("SM_CrewCrane"), 40.f, 41.f, 3, 2, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: the shoot before the set piece at 4.35 km"), 4.24f, 4.35f);
	Site(TEXT("PplShootC"), -1, 28.8f, 3, 0, 1.f);
	Scatter(VC("SM_CrewCrane"), 31.f, 31.5f, 3, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplShootD"), 1, 28.8f, 3, 1, 1.f);

Zone(TEXT("People: the shoot before the set piece at 5.80 km"), 5.7f, 5.8f);
	Site(TEXT("PplShootE"), -1, 28.8f, 3, 0, 1.f);
	Site(TEXT("PplShootA"), 1, 28.8f, 3, 1, 1.f);
	Scatter(VC("SM_CrewCrane"), 40.f, 41.f, 3, 2, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: the shoot before the set piece at 6.50 km"), 6.38f, 6.5f);
	Site(TEXT("PplShootD"), -1, 28.8f, 3, 0, 1.f);
	Scatter(VC("SM_CrewCrane"), 31.f, 31.5f, 3, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplShootB"), 1, 28.8f, 3, 1, 1.f);

Zone(TEXT("People: the shoot before the set piece at 7.44 km"), 7.37f, 7.44f);
	Site(TEXT("PplShootA"), -1, 28.8f, 3, 0, 1.f);
	Site(TEXT("PplShootC"), 1, 28.8f, 3, 1, 1.f);
	Scatter(VC("SM_CrewCrane"), 40.f, 41.f, 3, 2, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: the shoot before Engine 13's road"), 8.18f, 8.34f);
	Site(TEXT("PplShootA"), 1, 28.8f, 4, 0, 1.f);
	Site(TEXT("PplShootC"), -1, 28.8f, 4, 0, 1.f);
	Scatter(VC("SM_CrewCrane"), 40.f, 41.f, 4, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplShootE"), -1, 28.8f, 4, 1, 1.f);
	Site(TEXT("PplShootB"), 1, 28.8f, 4, 2, 1.f);
	Site(TEXT("PplShootD"), -1, 28.8f, 4, 2, 1.f);
	Site(TEXT("PplShootC"), 1, 28.8f, 4, 3, 1.f);
	Site(TEXT("PplShootA"), -1, 28.8f, 4, 3, 1.f);

Zone(TEXT("People: the shoot before the set piece at 14.23 km"), 14.1f, 14.23f);
	Site(TEXT("PplShootB"), -1, 28.8f, 3, 0, 1.f);
	Scatter(VC("SM_CrewCrane"), 31.f, 31.5f, 3, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplShootE"), 1, 28.8f, 3, 1, 1.f);

Zone(TEXT("People: the shoot before the set piece at 16.23 km"), 16.1f, 16.23f);
	Site(TEXT("PplShootC"), -1, 28.8f, 3, 0, 1.f);
	Site(TEXT("PplShootD"), 1, 28.8f, 3, 1, 1.f);
	Scatter(VC("SM_CrewCrane"), 40.f, 41.f, 3, 2, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: the shoot before the set piece at 18.23 km"), 18.1f, 18.23f);
	Site(TEXT("PplShootE"), -1, 28.8f, 3, 0, 1.f);
	Scatter(VC("SM_CrewCrane"), 31.f, 31.5f, 3, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplShootA"), 1, 28.8f, 3, 1, 1.f);

Zone(TEXT("People: the shoot before the set piece at 20.24 km"), 20.11f, 20.24f);
	Site(TEXT("PplShootD"), -1, 28.8f, 3, 0, 1.f);
	Site(TEXT("PplShootB"), 1, 28.8f, 3, 1, 1.f);
	Scatter(VC("SM_CrewCrane"), 40.f, 41.f, 3, 2, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("People: the shoot before the set piece at 22.24 km"), 22.11f, 22.24f);
	Site(TEXT("PplShootA"), -1, 28.8f, 3, 0, 1.f);
	Scatter(VC("SM_CrewCrane"), 31.f, 31.5f, 3, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplShootC"), 1, 28.8f, 3, 1, 1.f);

// ---- NEAR: on the trestles, 28 m out (the districts with the bends, the shoot's stretches and the eruption's side cut out).

Zone(TEXT("People: the ash road, near (1)"), 0.32f, 0.5f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 5, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolWatch_A"), 28.4f, 28.8f, 5, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 5, 1, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolWatch_B"), 28.4f, 28.8f, 5, 2, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 5, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 5, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 5, 4, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolHose_A"), 34.f, 34.8f, 5, 4, 1.f, 1.15f, 0.f, 1, 0.f);
	Site(TEXT("PplShootA"), 1, 28.8f, 15, 6, 1.f);
	Site(TEXT("PplShootB"), -1, 28.8f, 15, 7, 1.f);

Zone(TEXT("People: the ash road, near (2)"), 0.63f, 0.89f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 5, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolWatch_A"), 28.4f, 28.8f, 5, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 5, 1, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolWatch_B"), 28.4f, 28.8f, 5, 2, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 5, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 5, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 5, 4, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolHose_A"), 34.f, 34.8f, 5, 4, 1.f, 1.15f, 0.f, 1, 0.f);
	Site(TEXT("PplShootA"), 1, 28.8f, 15, 6, 1.f);
	Site(TEXT("PplShootB"), -1, 28.8f, 15, 7, 1.f);

Zone(TEXT("People: lava jumps, near (1)"), 1.35f, 1.67f);
	Scatter(VP("SM_VolHose_A"), 34.f, 34.8f, 4, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 4, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolHose_B"), 28.4f, 28.8f, 4, 1, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolHose_B"), 34.f, 34.8f, 4, 2, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolHose_A"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Site(TEXT("PplShootD"), 1, 28.8f, 16, 5, 1.f);
	Site(TEXT("PplShootA"), -1, 28.8f, 16, 6, 1.f);

Zone(TEXT("People: lava jumps, near (2)"), 1.885f, 2.24f);
	Scatter(VP("SM_VolHose_A"), 34.f, 34.8f, 4, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 4, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolHose_B"), 28.4f, 28.8f, 4, 1, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolHose_B"), 34.f, 34.8f, 4, 2, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolHose_A"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Site(TEXT("PplShootD"), 1, 28.8f, 16, 5, 1.f);
	Site(TEXT("PplShootA"), -1, 28.8f, 16, 6, 1.f);

Zone(TEXT("People: lava jumps, near (3), the left"), 2.34f, 2.5f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 4, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolHose_B"), 28.4f, 28.8f, 4, 1, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolHose_A"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootA"), -1, 28.8f, 16, 6, 1.f);

Zone(TEXT("People: the caldera, near (1), the left"), 2.5f, 2.95f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 4, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolWatch_A"), 28.4f, 28.8f, 4, 1, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolSurvey_B"), 28.4f, 28.8f, 4, 2, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootC"), -1, 28.8f, 12, 7, 1.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 8, 7, 1.f, 1.15f, 0.f, -1, 0.f);

Zone(TEXT("People: the caldera, near (2), the left"), 3.31f, 3.76f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 4, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolWatch_A"), 28.4f, 28.8f, 4, 1, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolSurvey_B"), 28.4f, 28.8f, 4, 2, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootC"), -1, 28.8f, 12, 7, 1.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 8, 7, 1.f, 1.15f, 0.f, -1, 0.f);

Zone(TEXT("People: fire and brimstone, near (1)"), 4.09f, 4.24f);
	Scatter(VP("SM_VolSurvey_B"), 28.4f, 28.8f, 4, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolHose_A"), 34.f, 34.8f, 4, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 4, 1, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolHose_B"), 28.4f, 28.8f, 4, 2, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolWatch_B"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootB"), -1, 28.8f, 16, 5, 1.f);
	Site(TEXT("PplShootE"), 1, 28.8f, 16, 6, 1.f);

Zone(TEXT("People: fire and brimstone, near (2)"), 4.35f, 5.18f);
	Scatter(VP("SM_VolSurvey_B"), 28.4f, 28.8f, 4, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolHose_A"), 34.f, 34.8f, 4, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 4, 1, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolHose_B"), 28.4f, 28.8f, 4, 2, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolWatch_B"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootB"), -1, 28.8f, 16, 5, 1.f);
	Site(TEXT("PplShootE"), 1, 28.8f, 16, 6, 1.f);

Zone(TEXT("People: the magma run, near (1)"), 5.53f, 5.7f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 4, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolHose_A"), 28.4f, 28.8f, 4, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 4, 1, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolHose_B"), 34.f, 34.8f, 4, 1, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 4, 2, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootD"), 1, 28.8f, 12, 6, 1.f);

Zone(TEXT("People: the magma run, near (2)"), 5.8f, 6.38f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 4, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolHose_A"), 28.4f, 28.8f, 4, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 4, 1, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolHose_B"), 34.f, 34.8f, 4, 1, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 4, 2, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootD"), 1, 28.8f, 12, 6, 1.f);

Zone(TEXT("People: the magma run, near (3)"), 6.5f, 6.73f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 4, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolHose_A"), 28.4f, 28.8f, 4, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 4, 1, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolHose_B"), 34.f, 34.8f, 4, 1, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 4, 2, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootD"), 1, 28.8f, 12, 6, 1.f);

Zone(TEXT("People: obsidian pass, near (1)"), 6.73f, 6.83f);
	Scatter(VP("SM_VolWatch_A"), 28.4f, 28.8f, 4, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 8, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolWatch_B"), 28.4f, 28.8f, 4, 1, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 8, 2, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolSurvey_B"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolHose_B"), 28.4f, 28.8f, 8, 5, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootA"), -1, 28.8f, 16, 5, 1.f);
	Site(TEXT("PplShootC"), 1, 28.8f, 16, 6, 1.f);

Zone(TEXT("People: obsidian pass, near (2)"), 7.08f, 7.25f);
	Scatter(VP("SM_VolWatch_A"), 28.4f, 28.8f, 4, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 8, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolWatch_B"), 28.4f, 28.8f, 4, 1, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 8, 2, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolSurvey_B"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolHose_B"), 28.4f, 28.8f, 8, 5, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootA"), -1, 28.8f, 16, 5, 1.f);
	Site(TEXT("PplShootC"), 1, 28.8f, 16, 6, 1.f);

Zone(TEXT("People: obsidian pass, near (3)"), 7.44f, 8.18f);
	Scatter(VP("SM_VolWatch_A"), 28.4f, 28.8f, 4, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 8, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolWatch_B"), 28.4f, 28.8f, 4, 1, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 8, 2, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolSurvey_B"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 4, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolHose_B"), 28.4f, 28.8f, 8, 5, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootA"), -1, 28.8f, 16, 5, 1.f);
	Site(TEXT("PplShootC"), 1, 28.8f, 16, 6, 1.f);

Zone(TEXT("People: Engine 13's road, near (1)"), 8.34f, 11.1f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 5, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolWatch_B"), 28.4f, 28.8f, 5, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 5, 1, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolWatch_A"), 28.4f, 28.8f, 5, 2, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 5, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 10, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 10, 4, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootB"), 1, 28.8f, 20, 6, 1.f);
	Site(TEXT("PplShootE"), -1, 28.8f, 20, 7, 1.f);
	Site(TEXT("PplShootA"), 1, 28.8f, 20, 9, 1.f);

Zone(TEXT("People: the burning slopes, near (1)"), 11.1f, 13.9f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 10, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolSurvey_B"), 28.4f, 28.8f, 5, 1, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 10, 2, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 10, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 5, 4, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolWatch_A"), 28.4f, 28.8f, 5, 4, 1.f, 1.15f, 0.f, 1, 0.f);
	Site(TEXT("PplShootC"), 1, 28.8f, 25, 5, 1.f);
	Site(TEXT("PplShootD"), -1, 28.8f, 25, 8, 1.f);

Zone(TEXT("People: the ash fields, near (1)"), 13.9f, 14.1f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 5, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 5, 1, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolWatch_B"), 28.4f, 28.8f, 5, 2, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolWatch_A"), 28.4f, 28.8f, 5, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 10, 4, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolSurvey_B"), 28.4f, 28.8f, 10, 4, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootA"), -1, 28.8f, 25, 5, 1.f);
	Site(TEXT("PplShootB"), 1, 28.8f, 25, 8, 1.f);

Zone(TEXT("People: the ash fields, near (2)"), 14.23f, 16.1f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 5, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 5, 1, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolWatch_B"), 28.4f, 28.8f, 5, 2, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolWatch_A"), 28.4f, 28.8f, 5, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 10, 4, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolSurvey_B"), 28.4f, 28.8f, 10, 4, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootA"), -1, 28.8f, 25, 5, 1.f);
	Site(TEXT("PplShootB"), 1, 28.8f, 25, 8, 1.f);

Zone(TEXT("People: the ash fields, near (3)"), 16.23f, 16.7f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 5, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 5, 1, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolWatch_B"), 28.4f, 28.8f, 5, 2, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolWatch_A"), 28.4f, 28.8f, 5, 3, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 10, 4, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolSurvey_B"), 28.4f, 28.8f, 10, 4, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootA"), -1, 28.8f, 25, 5, 1.f);
	Site(TEXT("PplShootB"), 1, 28.8f, 25, 8, 1.f);

Zone(TEXT("People: the far works, near (1)"), 16.7f, 18.1f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 7, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolWatch_A"), 28.4f, 28.8f, 7, 2, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 7, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 7, 5, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolWatch_B"), 28.4f, 28.8f, 7, 6, 1.f, 1.15f, 0.f, 1, 0.f);
	Site(TEXT("PplShootA"), -1, 28.8f, 21, 8, 1.f);
	Site(TEXT("PplShootC"), 1, 28.8f, 21, 11, 1.f);

Zone(TEXT("People: the far works, near (2)"), 18.23f, 19.5f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 7, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolWatch_A"), 28.4f, 28.8f, 7, 2, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 7, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 7, 5, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolWatch_B"), 28.4f, 28.8f, 7, 6, 1.f, 1.15f, 0.f, 1, 0.f);
	Site(TEXT("PplShootA"), -1, 28.8f, 21, 8, 1.f);
	Site(TEXT("PplShootC"), 1, 28.8f, 21, 11, 1.f);

Zone(TEXT("People: the inner wall, near (1)"), 19.5f, 20.11f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 7, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolSurvey_B"), 28.4f, 28.8f, 7, 2, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 7, 4, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 7, 5, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 7, 6, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootB"), 1, 28.8f, 21, 8, 1.f);
	Site(TEXT("PplShootE"), -1, 28.8f, 21, 10, 1.f);

Zone(TEXT("People: the inner wall, near (2)"), 20.24f, 22.11f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 7, 0, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolSurvey_B"), 28.4f, 28.8f, 7, 2, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolSurvey_A"), 28.4f, 28.8f, 7, 4, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolGoats"), 28.4f, 28.8f, 7, 5, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 7, 6, 1.f, 1.15f, 0.f, -1, 0.f);
	Site(TEXT("PplShootB"), 1, 28.8f, 21, 8, 1.f);
	Site(TEXT("PplShootE"), -1, 28.8f, 21, 10, 1.f);

Zone(TEXT("People: the last ash, near (1)"), 22.3f, 99.f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 7, 0, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolEvac_B"), 28.4f, 28.8f, 7, 1, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolWatch_B"), 28.4f, 28.8f, 7, 3, 1.f, 1.15f, 0.f, 1, 0.f);
	Scatter(VP("SM_VolWatch_A"), 28.4f, 28.8f, 7, 5, 1.f, 1.15f, 0.f, -1, 0.f);
	Scatter(VP("SM_VolEvac_A"), 28.4f, 28.8f, 7, 6, 1.f, 1.15f, 0.f, 1, 0.f);
	Site(TEXT("PplShootD"), -1, 28.8f, 21, 9, 1.f);
	Site(TEXT("PplShootA"), 1, 28.8f, 21, 11, 1.f);

// ---- THE MIDDLE DISTANCE, THE SLOPES AND THE SKY, district by district (left-hand rules; then the right-hand ones,
// which stop for the eruption between 2.4 and 3.9 km).

Zone(TEXT("People: the ash road, beyond: the left"), 0.f, 1.17f);
	Far(VP("SM_VolFlock_A"), 55.f, 130.f, 9, 1, 1.f, 1.f, 0.15f, -1, 0.f, 8.f);
	Far(VP("SM_VolCaravan"), 196.f, 204.f, 9, 2, 1.f, 1.f, 0.f, -1, -4.31f, 4.f);
	Scatter(VP("SM_VolWatch_B"), 72.f, 78.f, 7, 6, 1.f, 1.4f, 0.f, -1, 4.f);

Zone(TEXT("People: the ash road, beyond: the right"), 0.f, 1.17f);
	Far(VP("SM_VolFlock_A"), 55.f, 130.f, 9, 3, 1.f, 1.f, 0.15f, 1, 0.f, 8.f);
	Far(VP("SM_VolCaravan"), 196.f, 204.f, 9, 5, 1.f, 1.f, 0.f, 1, -5.85f, 4.f);
	Far(VP("SM_VolCamp"), 228.f, 236.f, 13, 6, 1.f, 1.f, 0.f, 1, -7.77f, 4.f);
	Scatter(VP("SM_VolWatch_A"), 72.f, 78.f, 7, 3, 1.f, 1.4f, 0.f, 1, 4.f);
	Scatter(VP("SM_VolCaravan"), 78.f, 86.f, 9, 4, 1.f, 0.7f, 0.f, 1, 4.f);

Zone(TEXT("People: the curtain of fire, beyond: the left"), 0.34f, 0.56f);
	Scatter(VP("SM_VolFireLine"), 97.f, 101.f, 2, 0, 1.f, 1.f, 0.f, -1, 4.f);

Zone(TEXT("People: lava jumps, beyond: the left"), 1.17f, 2.5f);
	Far(VP("SM_VolFlock_B"), 55.f, 130.f, 9, 0, 1.f, 1.f, 0.15f, -1, 0.f, 8.f);
	Scatter(VP("SM_VolFireLine"), 84.f, 92.f, 8, 3, 1.f, 1.f, 0.f, -1, 4.f);
	Far(VP("SM_VolHerd"), 128.f, 138.f, 11, 4, 1.f, 1.f, 0.f, -1, -1.228f, 4.f);

Zone(TEXT("People: lava jumps, beyond: the right"), 1.17f, 2.4f);
	Far(VP("SM_VolFlock_A"), 55.f, 130.f, 9, 2, 1.f, 1.f, 0.15f, 1, 0.f, 8.f);
	Scatter(VP("SM_VolFireLine"), 86.f, 94.f, 8, 7, 1.f, 1.f, 0.f, 1, 4.f);
	Far(VP("SM_VolCaravan"), 196.f, 204.f, 13, 8, 1.f, 1.f, 0.f, 1, -5.85f, 4.f);

Zone(TEXT("People: the caldera, beyond: the left"), 2.5f, 3.76f);
	Far(VP("SM_VolFlock_B"), 55.f, 130.f, 7, 1, 1.f, 1.f, 0.15f, -1, 0.f, 8.f);
	Far(VP("SM_VolFlock_A"), 55.f, 130.f, 7, 3, 1.f, 1.f, 0.15f, -1, 0.f, 8.f);
	Scatter(VP("SM_VolGoats"), 70.f, 84.f, 8, 2, 1.f, 1.6f, 0.f, -1, 4.f);
	Far(VP("SM_VolHerd"), 128.f, 138.f, 10, 5, 1.f, 1.f, 0.f, -1, -1.228f, 4.f);

Zone(TEXT("People: fire and brimstone, beyond: the left"), 3.76f, 5.18f);
	Far(VP("SM_VolFlock_A"), 55.f, 130.f, 9, 0, 1.f, 1.f, 0.15f, -1, 0.f, 8.f);
	Scatter(VP("SM_VolFireLine"), 98.f, 106.f, 7, 4, 1.f, 1.f, 0.f, -1, 4.f);
	Far(VP("SM_VolCamp"), 228.f, 236.f, 12, 5, 1.f, 1.f, 0.f, -1, -5.782f, 4.f);

Zone(TEXT("People: fire and brimstone, beyond: the right"), 3.9f, 5.18f);
	Far(VP("SM_VolFlock_B"), 55.f, 130.f, 9, 2, 1.f, 1.f, 0.15f, 1, 0.f, 8.f);
	Scatter(VP("SM_VolFireLine"), 98.f, 106.f, 7, 1, 1.f, 1.f, 0.f, 1, 4.f);
	Far(VP("SM_VolCaravan"), 196.f, 204.f, 11, 7, 1.f, 1.f, 0.f, 1, -5.85f, 4.f);

Zone(TEXT("People: the magma run, beyond: the left"), 5.18f, 6.73f);
	Far(VP("SM_VolFlock_B"), 55.f, 130.f, 9, 1, 1.f, 1.f, 0.15f, -1, 0.f, 8.f);
	Far(VP("SM_VolHerd"), 128.f, 138.f, 9, 5, 1.f, 1.f, 0.f, -1, -1.228f, 4.f);
	Scatter(VP("SM_VolFireLine"), 96.f, 104.f, 9, 6, 1.f, 1.f, 0.f, -1, 4.f);

Zone(TEXT("People: the magma run, beyond: the right"), 5.18f, 6.73f);
	Far(VP("SM_VolFlock_A"), 55.f, 130.f, 9, 3, 1.f, 1.f, 0.15f, 1, 0.f, 8.f);
	Far(VP("SM_VolHerd"), 128.f, 138.f, 9, 2, 1.f, 1.f, 0.f, 1, -1.83f, 4.f);
	Scatter(VP("SM_VolGoats"), 72.f, 86.f, 10, 4, 1.f, 1.6f, 0.f, 1, 4.f);

Zone(TEXT("People: obsidian pass, beyond: the left"), 6.73f, 8.34f);
	Far(VP("SM_VolFlock_A"), 55.f, 130.f, 7, 0, 1.f, 1.f, 0.15f, -1, 0.f, 8.f);
	Scatter(VP("SM_VolGoats"), 70.f, 84.f, 9, 1, 1.f, 1.6f, 0.f, -1, 4.f);

Zone(TEXT("People: obsidian pass, beyond: the right"), 6.73f, 8.34f);
	Far(VP("SM_VolFlock_B"), 55.f, 130.f, 7, 2, 1.f, 1.f, 0.15f, 1, 0.f, 8.f);
	Scatter(VP("SM_VolWatch_A"), 74.f, 80.f, 9, 5, 1.f, 1.4f, 0.f, 1, 4.f);

Zone(TEXT("People: Engine 13's road, beyond: the left"), 8.34f, 11.1f);
	Far(VP("SM_VolFlock_A"), 55.f, 130.f, 10, 1, 1.f, 1.f, 0.15f, -1, 0.f, 8.f);
	Far(VP("SM_VolCaravan"), 196.f, 204.f, 10, 3, 1.f, 1.f, 0.f, -1, -4.31f, 4.f);
	Scatter(VP("SM_VolWatch_B"), 72.f, 78.f, 9, 2, 1.f, 1.4f, 0.f, -1, 4.f);

Zone(TEXT("People: Engine 13's road, beyond: the right"), 8.34f, 11.1f);
	Far(VP("SM_VolFlock_B"), 55.f, 130.f, 10, 4, 1.f, 1.f, 0.15f, 1, 0.f, 8.f);
	Far(VP("SM_VolCaravan"), 196.f, 204.f, 10, 7, 1.f, 1.f, 0.f, 1, -5.85f, 4.f);
	Scatter(VP("SM_VolWatch_A"), 74.f, 80.f, 9, 6, 1.f, 1.4f, 0.f, 1, 4.f);
	Far(VP("SM_VolCamp"), 228.f, 236.f, 16, 9, 1.f, 1.f, 0.f, 1, -7.77f, 4.f);

Zone(TEXT("People: the burning slopes, beyond: the left"), 11.1f, 13.9f);
	Far(VP("SM_VolFlock_B"), 55.f, 130.f, 10, 0, 1.f, 1.f, 0.15f, -1, 0.f, 8.f);
	Far(VP("SM_VolHerd"), 128.f, 138.f, 10, 2, 1.f, 1.f, 0.f, -1, -1.228f, 4.f);
	Scatter(VP("SM_VolGoats"), 70.f, 84.f, 10, 4, 1.f, 1.6f, 0.f, -1, 4.f);

Zone(TEXT("People: the burning slopes, beyond: the right"), 11.1f, 13.9f);
	Far(VP("SM_VolFlock_A"), 55.f, 130.f, 10, 3, 1.f, 1.f, 0.15f, 1, 0.f, 8.f);
	Far(VP("SM_VolHerd"), 128.f, 138.f, 10, 6, 1.f, 1.f, 0.f, 1, -1.83f, 4.f);
	Scatter(VP("SM_VolGoats"), 72.f, 86.f, 10, 1, 1.f, 1.6f, 0.f, 1, 4.f);
	Scatter(VP("SM_VolFireLine"), 104.f, 112.f, 10, 5, 1.f, 1.f, 0.f, 1, 4.f);

Zone(TEXT("People: the ash fields, beyond: the left"), 13.9f, 16.7f);
	Far(VP("SM_VolFlock_A"), 55.f, 130.f, 10, 2, 1.f, 1.f, 0.15f, -1, 0.f, 8.f);
	Far(VP("SM_VolCaravan"), 196.f, 204.f, 9, 1, 1.f, 1.f, 0.f, -1, -4.31f, 4.f);
	Far(VP("SM_VolCamp"), 228.f, 236.f, 14, 6, 1.f, 1.f, 0.f, -1, -5.782f, 4.f);
	Scatter(VP("SM_VolCaravan"), 78.f, 86.f, 9, 3, 1.f, 0.7f, 0.f, -1, 4.f);

Zone(TEXT("People: the ash fields, beyond: the right"), 13.9f, 16.7f);
	Far(VP("SM_VolFlock_A"), 55.f, 130.f, 10, 5, 1.f, 1.f, 0.15f, 1, 0.f, 8.f);
	Far(VP("SM_VolCaravan"), 196.f, 204.f, 9, 4, 1.f, 1.f, 0.f, 1, -5.85f, 4.f);
	Scatter(VP("SM_VolWatch_B"), 74.f, 80.f, 9, 7, 1.f, 1.4f, 0.f, 1, 4.f);

Zone(TEXT("People: the far works, beyond: the left"), 16.7f, 19.5f);
	Far(VP("SM_VolFlock_B"), 55.f, 130.f, 12, 1, 1.f, 1.f, 0.15f, -1, 0.f, 8.f);
	Far(VP("SM_VolCaravan"), 196.f, 204.f, 12, 3, 1.f, 1.f, 0.f, -1, -4.31f, 4.f);

Zone(TEXT("People: the far works, beyond: the right"), 16.7f, 19.5f);
	Far(VP("SM_VolFlock_A"), 55.f, 130.f, 12, 4, 1.f, 1.f, 0.15f, 1, 0.f, 8.f);
	Far(VP("SM_VolHerd"), 128.f, 138.f, 12, 7, 1.f, 1.f, 0.f, 1, -1.83f, 4.f);

Zone(TEXT("People: the inner wall, beyond: the left"), 19.5f, 22.3f);
	Far(VP("SM_VolFlock_A"), 55.f, 130.f, 12, 0, 1.f, 1.f, 0.15f, -1, 0.f, 8.f);
	Far(VP("SM_VolHerd"), 128.f, 138.f, 11, 2, 1.f, 1.f, 0.f, -1, -1.228f, 4.f);

Zone(TEXT("People: the inner wall, beyond: the right"), 19.5f, 22.3f);
	Far(VP("SM_VolFlock_B"), 55.f, 130.f, 12, 3, 1.f, 1.f, 0.15f, 1, 0.f, 8.f);
	Scatter(VP("SM_VolGoats"), 72.f, 86.f, 11, 6, 1.f, 1.6f, 0.f, 1, 4.f);

Zone(TEXT("People: the last ash, beyond: the left"), 22.3f, 99.f);
	Far(VP("SM_VolFlock_A"), 55.f, 130.f, 12, 2, 1.f, 1.f, 0.15f, -1, 0.f, 8.f);
	Far(VP("SM_VolCamp"), 228.f, 236.f, 14, 6, 1.f, 1.f, 0.f, -1, -5.782f, 4.f);
	Scatter(VP("SM_VolCaravan"), 78.f, 86.f, 9, 4, 1.f, 0.7f, 0.f, -1, 4.f);

Zone(TEXT("People: the last ash, beyond: the right"), 22.3f, 99.f);
	Far(VP("SM_VolFlock_A"), 55.f, 130.f, 12, 5, 1.f, 1.f, 0.15f, 1, 0.f, 8.f);
	Far(VP("SM_VolCaravan"), 196.f, 204.f, 10, 1, 1.f, 1.f, 0.f, 1, -5.85f, 4.f);

#undef VP
#undef VC

{
// ---- BEGIN far (ft_people_volcano_far.py) ----
// FAR LIFE AND CREATURES (the extra worker, 15 Nov 2026). Meshes /Game/People/Volcano/SM_Far*, all on the always-moving
// master, all by Far rules so that they stay through the jumps, loops and corkscrews. Where they stand, and why:
//  - SM_FarLedgeGoats and SM_FarRimSurvey on the slopes 162 to 174 m out, between the main table's herd (128 to 138 m)
//    and its caravan (196 to 204 m); the drop is the land's own there (0.75 m under the road out to 90 m, then rising
//    4.6 cm a metre on the left, 6 cm on the right), and both are rooted 4 to 5 m into it.
//  - SM_FarRidgeLine at 282 to 290 m, the land's last rise, past the dressing's crag terraces (245 to 270 m): it brings
//    its own causeway 8 m tall, so the line walks the skyline.
//  - SM_FarSkein_A / _B (their height is in the mesh: 40 to 125 m up) 210 to 300 m out; SM_FarKettle (36 to 86 m up)
//    150 to 260 m out, turned any way round.
//  - Nothing on the right between 2.4 and 3.9 km (the eruption is watched over that side: FTDressL3.inl).
// The districts take turns: the villagers leaving, cranes and goats (the ash road, fire and brimstone, Engine 13's road,
// the ash fields, the last ash); herons, vultures and goats (lava jumps, the magma run, the burning slopes, the inner
// wall); the survey on its crater rims, vultures and cranes (the caldera, obsidian pass, the far works).
// Written by a script (the worker's scratch: jobs/L3f/table.py).
#define VF(Name) TEXT("/Game/People/Volcano/" Name)
Zone(TEXT("People far: the ash road, the left"), 0.f, 1.17f);
	Far(VF("SM_FarRidgeLine"), 282.f, 290.f, 22, 3, 1.f, 1.f, 0.f, -1, -8.266f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 1, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 8, 1.f, 1.f, 0.f, -1, -2.838f, 22.f);
Zone(TEXT("People far: the ash road, the right"), 0.f, 1.17f);
	Far(VF("SM_FarRidgeLine"), 282.f, 290.f, 22, 14, 1.f, 1.f, 0.f, 1, -11.01f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 8, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 17, 1.f, 1.f, 0.f, 1, -3.93f, 22.f);
Zone(TEXT("People far: lava jumps, the left"), 1.17f, 2.5f);
	Far(VF("SM_FarSkein_B"), 210.f, 300.f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
	Far(VF("SM_FarKettle"), 150.f, 260.f, 16, 8, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 15, 1.f, 1.f, 0.f, -1, -2.838f, 22.f);
Zone(TEXT("People far: lava jumps, the right"), 1.17f, 2.4f);
	Far(VF("SM_FarSkein_B"), 210.f, 300.f, 12, 1, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
	Far(VF("SM_FarKettle"), 150.f, 260.f, 16, 0, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 6, 1.f, 1.f, 0.f, 1, -3.93f, 22.f);
Zone(TEXT("People far: the caldera, the left"), 2.5f, 3.76f);
	Far(VF("SM_FarRimSurvey"), 164.f, 172.f, 10, 8, 1.f, 1.f, 0.f, -1, -2.838f, 14.f);
	Far(VF("SM_FarKettle"), 150.f, 260.f, 14, 12, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 16, 3, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
Zone(TEXT("People far: fire and brimstone, the left"), 3.76f, 5.18f);
	Far(VF("SM_FarRidgeLine"), 282.f, 290.f, 22, 18, 1.f, 1.f, 0.f, -1, -8.266f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 10, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 2, 1.f, 1.f, 0.f, -1, -2.838f, 22.f);
Zone(TEXT("People far: fire and brimstone, the right"), 3.9f, 5.18f);
	Far(VF("SM_FarRidgeLine"), 282.f, 290.f, 22, 7, 1.f, 1.f, 0.f, 1, -11.01f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 3, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 11, 1.f, 1.f, 0.f, 1, -3.93f, 22.f);
Zone(TEXT("People far: the magma run, the left"), 5.18f, 6.73f);
	Far(VF("SM_FarSkein_B"), 210.f, 300.f, 12, 10, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
	Far(VF("SM_FarKettle"), 150.f, 260.f, 16, 1, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 9, 1.f, 1.f, 0.f, -1, -2.838f, 22.f);
Zone(TEXT("People far: the magma run, the right"), 5.18f, 6.73f);
	Far(VF("SM_FarSkein_B"), 210.f, 300.f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
	Far(VF("SM_FarKettle"), 150.f, 260.f, 16, 9, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 0, 1.f, 1.f, 0.f, 1, -3.93f, 22.f);
Zone(TEXT("People far: obsidian pass, the left"), 6.73f, 8.34f);
	Far(VF("SM_FarRimSurvey"), 164.f, 172.f, 10, 7, 1.f, 1.f, 0.f, -1, -2.838f, 14.f);
	Far(VF("SM_FarKettle"), 150.f, 260.f, 14, 7, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 16, 2, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
Zone(TEXT("People far: obsidian pass, the right"), 6.73f, 8.34f);
	Far(VF("SM_FarRimSurvey"), 164.f, 172.f, 10, 2, 1.f, 1.f, 0.f, 1, -3.93f, 14.f);
	Far(VF("SM_FarKettle"), 150.f, 260.f, 14, 0, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 16, 10, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
Zone(TEXT("People far: Engine 13's road, the left"), 8.34f, 11.1f);
	Far(VF("SM_FarRidgeLine"), 282.f, 290.f, 22, 11, 1.f, 1.f, 0.f, -1, -8.266f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 5, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 14, 1.f, 1.f, 0.f, -1, -2.838f, 22.f);
Zone(TEXT("People far: Engine 13's road, the right"), 8.34f, 11.1f);
	Far(VF("SM_FarRidgeLine"), 282.f, 290.f, 22, 0, 1.f, 1.f, 0.f, 1, -11.01f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 12, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 5, 1.f, 1.f, 0.f, 1, -3.93f, 22.f);
Zone(TEXT("People far: the burning slopes, the left"), 11.1f, 13.9f);
	Far(VF("SM_FarSkein_B"), 210.f, 300.f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
	Far(VF("SM_FarKettle"), 150.f, 260.f, 16, 10, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 3, 1.f, 1.f, 0.f, -1, -2.838f, 22.f);
Zone(TEXT("People far: the burning slopes, the right"), 11.1f, 13.9f);
	Far(VF("SM_FarSkein_B"), 210.f, 300.f, 12, 7, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
	Far(VF("SM_FarKettle"), 150.f, 260.f, 16, 2, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 12, 1.f, 1.f, 0.f, 1, -3.93f, 22.f);
Zone(TEXT("People far: the ash fields, the left"), 13.9f, 16.7f);
	Far(VF("SM_FarRidgeLine"), 282.f, 290.f, 22, 21, 1.f, 1.f, 0.f, -1, -8.266f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 11, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 4, 1.f, 1.f, 0.f, -1, -2.838f, 22.f);
Zone(TEXT("People far: the ash fields, the right"), 13.9f, 16.7f);
	Far(VF("SM_FarRidgeLine"), 282.f, 290.f, 22, 10, 1.f, 1.f, 0.f, 1, -11.01f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 4, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 13, 1.f, 1.f, 0.f, 1, -3.93f, 22.f);
Zone(TEXT("People far: the far works, the left"), 16.7f, 19.5f);
	Far(VF("SM_FarRimSurvey"), 164.f, 172.f, 10, 9, 1.f, 1.f, 0.f, -1, -2.838f, 14.f);
	Far(VF("SM_FarKettle"), 150.f, 260.f, 14, 5, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 16, 6, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
Zone(TEXT("People far: the far works, the right"), 16.7f, 19.5f);
	Far(VF("SM_FarRimSurvey"), 164.f, 172.f, 10, 4, 1.f, 1.f, 0.f, 1, -3.93f, 14.f);
	Far(VF("SM_FarKettle"), 150.f, 260.f, 14, 12, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 16, 14, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
Zone(TEXT("People far: the inner wall, the left"), 19.5f, 22.3f);
	Far(VF("SM_FarSkein_B"), 210.f, 300.f, 12, 4, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
	Far(VF("SM_FarKettle"), 150.f, 260.f, 16, 3, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 15, 1.f, 1.f, 0.f, -1, -2.838f, 22.f);
Zone(TEXT("People far: the inner wall, the right"), 19.5f, 22.3f);
	Far(VF("SM_FarSkein_B"), 210.f, 300.f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
	Far(VF("SM_FarKettle"), 150.f, 260.f, 16, 11, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 6, 1.f, 1.f, 0.f, 1, -3.93f, 22.f);
Zone(TEXT("People far: the last ash, the left"), 22.3f, 99.f);
	Far(VF("SM_FarRidgeLine"), 282.f, 290.f, 22, 14, 1.f, 1.f, 0.f, -1, -8.266f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 6, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 16, 1.f, 1.f, 0.f, -1, -2.838f, 22.f);
Zone(TEXT("People far: the last ash, the right"), 22.3f, 99.f);
	Far(VF("SM_FarRidgeLine"), 282.f, 290.f, 22, 3, 1.f, 1.f, 0.f, 1, -11.01f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 13, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 7, 1.f, 1.f, 0.f, 1, -3.93f, 22.f);
#undef VF
// ---- END far ----
}

{
// ---- BEGIN watch (ft_people_volcano_watch.py) ----
// THE WATCHERS (an extra worker's block, 15 Nov 2026): the people who watch the chase and the mountain. Meshes
// /Game/People/Volcano/SM_Watch... (Content/Python/ft_people_volcano_watch.py). Where they stand, and why:
//  - NEAR, on open scaffold decks 2.8 m up, 36 m out on the left and 42 m on the right: past the main cast's trestles
//    (28 m) and the fumaroles, short of the lava channels (38 m left, 46 m right). The decks are open underneath: the
//    channel behind shows through. Only where the main cast's own near zones are (no banked bends, no set pieces),
//    never on the right between 2.4 and 3.9 km (the eruption), and on slots that the main cast's fire hoses (34 m on
//    the right, their water falls at 46 m) and the right-hand pump houses (40 m) do not take.
//  - BEHIND THE CHANNELS, big: 62.6 m (terraces) and 64.3 m (towers) on the left, 73.5 m on the right (past its walls
//    and the mine's railway). On the slots that the basalt walls, the channel works, the pipe bridges and the pump
//    houses leave free. Not on the left from 0.3 to 0.6 km (the curtain of fire is watched over that side).
//  - The fire crew's stand (SM_WatchFire: no vehicle, nothing red) is not placed from 8.17 km on: Engine 13's road.
#define VW(Name) TEXT("/Game/People/Volcano/" Name)
Zone(TEXT("People watch: the ash road, the decks (1)"), 0.32f, 0.5f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 5, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 10, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 5, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 10, 2, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the ash road, the decks (2)"), 0.63f, 0.89f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 5, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 10, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 5, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 10, 2, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: lava jumps, the decks (1)"), 1.35f, 1.67f);
	Scatter(VW("SM_WatchFire"), 35.8f, 36.4f, 8, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 8, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchFire"), 41.8f, 42.4f, 8, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 8, 3, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: lava jumps, the decks (2)"), 1.885f, 2.24f);
	Scatter(VW("SM_WatchFire"), 35.8f, 36.4f, 8, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 8, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchFire"), 41.8f, 42.4f, 8, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 8, 3, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: lava jumps, the decks (3)"), 2.34f, 2.5f);
	Scatter(VW("SM_WatchFire"), 35.8f, 36.4f, 8, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 8, 7, 1.f, 1.f, 0.f, -1, 0.f);
Zone(TEXT("People watch: the caldera, the decks (1)"), 2.5f, 2.95f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 8, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 8, 6, 1.f, 1.f, 0.f, -1, 0.f);
Zone(TEXT("People watch: the caldera, the decks (2)"), 3.31f, 3.76f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 8, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 8, 6, 1.f, 1.f, 0.f, -1, 0.f);
Zone(TEXT("People watch: fire and brimstone, the decks (1)"), 4.09f, 4.24f);
	Scatter(VW("SM_WatchFire"), 35.8f, 36.4f, 8, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 8, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchFire"), 41.8f, 42.4f, 8, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 8, 5, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: fire and brimstone, the decks (2)"), 4.35f, 5.18f);
	Scatter(VW("SM_WatchFire"), 35.8f, 36.4f, 8, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 8, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchFire"), 41.8f, 42.4f, 8, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 8, 5, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the magma run, the decks (1)"), 5.53f, 5.7f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 8, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchFire"), 35.8f, 36.4f, 8, 6, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 8, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 8, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchFire"), 41.8f, 42.4f, 8, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the magma run, the decks (2)"), 5.8f, 6.38f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 8, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchFire"), 35.8f, 36.4f, 8, 6, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 8, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 8, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchFire"), 41.8f, 42.4f, 8, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the magma run, the decks (3)"), 6.5f, 6.73f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 8, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchFire"), 35.8f, 36.4f, 8, 6, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 8, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 8, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchFire"), 41.8f, 42.4f, 8, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: obsidian pass, the decks (1)"), 6.73f, 6.83f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 8, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 8, 6, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchFire"), 35.8f, 36.4f, 8, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 8, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 8, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchFire"), 41.8f, 42.4f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: obsidian pass, the decks (2)"), 7.08f, 7.25f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 8, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 8, 6, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchFire"), 35.8f, 36.4f, 8, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 8, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 8, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchFire"), 41.8f, 42.4f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: obsidian pass, the decks (3)"), 7.44f, 8.17f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 8, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 8, 6, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchFire"), 35.8f, 36.4f, 8, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 8, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 8, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchFire"), 41.8f, 42.4f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: Engine 13's road, the decks (1)"), 8.34f, 11.1f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 12, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 12, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 12, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 12, 6, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the burning slopes, the decks (1)"), 11.1f, 13.9f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 10, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 10, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 10, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 10, 5, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the ash fields, the decks (1)"), 13.9f, 14.1f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 10, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 10, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 10, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 10, 5, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the ash fields, the decks (2)"), 14.23f, 16.1f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 10, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 10, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 10, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 10, 5, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the ash fields, the decks (3)"), 16.23f, 16.7f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 10, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 10, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 10, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 10, 5, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the far works, the decks (1)"), 16.7f, 18.1f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 12, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 12, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 12, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 12, 6, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the far works, the decks (2)"), 18.23f, 19.5f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 12, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 12, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 12, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 12, 6, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the inner wall, the decks (1)"), 19.5f, 20.11f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 12, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 12, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 12, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 12, 6, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the inner wall, the decks (2)"), 20.24f, 22.11f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 12, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 12, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 12, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 12, 6, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the last ash, the decks (1)"), 22.3f, 99.f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 12, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 12, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 12, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 12, 6, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the start, behind the lava"), 0.05f, 0.3f);
	Scatter(VW("SM_WatchTerrace_A"), 62.3f, 62.9f, 8, 5, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTower"), 64.f, 64.6f, 8, 3, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("People watch: the ash road, behind the lava"), 0.f, 1.17f);
	Scatter(VW("SM_WatchTerrace_A"), 73.2f, 73.8f, 8, 1, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(VW("SM_WatchTower"), 73.2f, 73.8f, 8, 5, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("People watch: the ash road, past the fire, behind the lava"), 0.6f, 1.17f);
	Scatter(VW("SM_WatchTerrace_B"), 62.3f, 62.9f, 8, 0, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTower"), 64.f, 64.6f, 8, 4, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("People watch: lava jumps, behind the lava"), 1.17f, 2.5f);
	Scatter(VW("SM_WatchTerrace_A"), 62.3f, 62.9f, 8, 5, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTower"), 64.f, 64.6f, 8, 1, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("People watch: lava jumps, the right, behind the lava"), 1.17f, 2.4f);
	Scatter(VW("SM_WatchTerrace_B"), 73.2f, 73.8f, 8, 1, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(VW("SM_WatchTower"), 73.2f, 73.8f, 8, 5, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("People watch: the caldera, behind the lava"), 2.5f, 3.76f);
	Scatter(VW("SM_WatchTerrace_B"), 62.3f, 62.9f, 8, 0, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTower"), 64.f, 64.6f, 8, 4, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("People watch: fire and brimstone, behind the lava"), 3.76f, 5.18f);
	Scatter(VW("SM_WatchTerrace_A"), 62.3f, 62.9f, 8, 0, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTower"), 64.f, 64.6f, 8, 4, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("People watch: fire and brimstone, the right, behind the lava"), 3.95f, 5.18f);
	Scatter(VW("SM_WatchTerrace_B"), 73.2f, 73.8f, 8, 1, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(VW("SM_WatchTower"), 73.2f, 73.8f, 8, 5, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("People watch: the magma run, behind the lava"), 5.18f, 6.64f);
	Scatter(VW("SM_WatchTerrace_B"), 62.3f, 62.9f, 8, 1, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTower"), 64.f, 64.6f, 8, 5, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTerrace_A"), 73.2f, 73.8f, 8, 0, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(VW("SM_WatchTower"), 73.2f, 73.8f, 8, 4, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("People watch: obsidian pass, behind the lava"), 6.8f, 8.34f);
	Scatter(VW("SM_WatchTerrace_A"), 62.3f, 62.9f, 8, 0, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTower"), 64.f, 64.6f, 8, 4, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTerrace_B"), 73.2f, 73.8f, 8, 0, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(VW("SM_WatchTower"), 73.2f, 73.8f, 8, 4, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("People watch: Engine 13's road, behind the lava"), 8.34f, 11.1f);
	Scatter(VW("SM_WatchTerrace_B"), 62.3f, 62.9f, 12, 0, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTower"), 64.f, 64.6f, 12, 4, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTerrace_A"), 73.2f, 73.8f, 12, 0, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(VW("SM_WatchTower"), 73.2f, 73.8f, 12, 6, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("People watch: the burning slopes, behind the lava"), 11.1f, 13.9f);
	Scatter(VW("SM_WatchTerrace_A"), 62.3f, 62.9f, 10, 2, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTower"), 64.f, 64.6f, 10, 7, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTerrace_B"), 73.2f, 73.8f, 10, 0, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(VW("SM_WatchTower"), 73.2f, 73.8f, 10, 5, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("People watch: the ash fields, behind the lava"), 13.9f, 16.7f);
	Scatter(VW("SM_WatchTerrace_B"), 62.3f, 62.9f, 9, 1, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTower"), 64.f, 64.6f, 9, 4, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTerrace_A"), 73.2f, 73.8f, 9, 0, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(VW("SM_WatchTower"), 73.2f, 73.8f, 9, 3, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("People watch: the far works, behind the lava"), 16.7f, 19.5f);
	Scatter(VW("SM_WatchTerrace_A"), 62.3f, 62.9f, 12, 0, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTower"), 64.f, 64.6f, 12, 4, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTerrace_B"), 73.2f, 73.8f, 10, 0, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(VW("SM_WatchTower"), 73.2f, 73.8f, 10, 5, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("People watch: the inner wall, behind the lava"), 19.5f, 22.3f);
	Scatter(VW("SM_WatchTerrace_B"), 62.3f, 62.9f, 10, 2, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTower"), 64.f, 64.6f, 10, 7, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTerrace_A"), 73.2f, 73.8f, 10, 0, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(VW("SM_WatchTower"), 73.2f, 73.8f, 10, 5, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("People watch: the last ash, behind the lava"), 22.3f, 99.f);
	Scatter(VW("SM_WatchTerrace_A"), 62.3f, 62.9f, 10, 2, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTower"), 64.f, 64.6f, 10, 7, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTerrace_B"), 73.2f, 73.8f, 10, 0, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(VW("SM_WatchTower"), 73.2f, 73.8f, 10, 5, 1.f, 1.f, 0.f, 1, 3.f);
#undef VW
// ---- END watch ----
}
