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
//  - SM_FarRidgeLine at 180 to 188 m (at 286 m, the land's last rise, the cinder cones and the mine hid most of it):
//    it brings its own causeway 8 m tall, so the line walks above the slope's huts and haulers; the main table's
//    caravan (196 to 204 m, half the size) passes behind it. The goats' crag in the same districts is written half a
//    cycle away from it, so the two never stand in one slot.
//  - SM_FarSkein_A / _B (their height is in the mesh: 40 to 125 m up) 210 to 300 m out; SM_FarKettle (36 to 86 m up)
//    150 to 260 m out, turned any way round.
//  - Nothing on the right between 2.4 and 3.9 km (the eruption is watched over that side: FTDressL3.inl).
// The districts take turns: the villagers leaving, cranes and goats (the ash road, fire and brimstone, Engine 13's road,
// the ash fields, the last ash); herons, vultures and goats (lava jumps, the magma run, the burning slopes, the inner
// wall); the survey on its crater rims, vultures and cranes (the caldera, obsidian pass, the far works).
// Written by a script (the worker's scratch: jobs/L3f/table.py).
#define VF(Name) TEXT("/Game/People/Volcano/" Name)
Zone(TEXT("People far: the ash road, the left"), 0.f, 1.17f);
	Far(VF("SM_FarRidgeLine"), 180.f, 188.f, 22, 3, 1.f, 1.f, 0.f, -1, -3.574f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 1, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 22, 14, 1.f, 1.f, 0.f, -1, -2.838f, 22.f);
Zone(TEXT("People far: the ash road, the right"), 0.f, 1.17f);
	Far(VF("SM_FarRidgeLine"), 180.f, 188.f, 22, 14, 1.f, 1.f, 0.f, 1, -4.89f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 8, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 22, 3, 1.f, 1.f, 0.f, 1, -3.93f, 22.f);
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
	Far(VF("SM_FarRidgeLine"), 180.f, 188.f, 22, 18, 1.f, 1.f, 0.f, -1, -3.574f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 10, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 22, 7, 1.f, 1.f, 0.f, -1, -2.838f, 22.f);
Zone(TEXT("People far: fire and brimstone, the right"), 3.9f, 5.18f);
	Far(VF("SM_FarRidgeLine"), 180.f, 188.f, 22, 7, 1.f, 1.f, 0.f, 1, -4.89f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 3, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 22, 18, 1.f, 1.f, 0.f, 1, -3.93f, 22.f);
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
	Far(VF("SM_FarRidgeLine"), 180.f, 188.f, 22, 11, 1.f, 1.f, 0.f, -1, -3.574f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 5, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 22, 0, 1.f, 1.f, 0.f, -1, -2.838f, 22.f);
Zone(TEXT("People far: Engine 13's road, the right"), 8.34f, 11.1f);
	Far(VF("SM_FarRidgeLine"), 180.f, 188.f, 22, 0, 1.f, 1.f, 0.f, 1, -4.89f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 12, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 22, 11, 1.f, 1.f, 0.f, 1, -3.93f, 22.f);
Zone(TEXT("People far: the burning slopes, the left"), 11.1f, 13.9f);
	Far(VF("SM_FarSkein_B"), 210.f, 300.f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
	Far(VF("SM_FarKettle"), 150.f, 260.f, 16, 10, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 3, 1.f, 1.f, 0.f, -1, -2.838f, 22.f);
Zone(TEXT("People far: the burning slopes, the right"), 11.1f, 13.9f);
	Far(VF("SM_FarSkein_B"), 210.f, 300.f, 12, 7, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
	Far(VF("SM_FarKettle"), 150.f, 260.f, 16, 2, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 18, 12, 1.f, 1.f, 0.f, 1, -3.93f, 22.f);
Zone(TEXT("People far: the ash fields, the left"), 13.9f, 16.7f);
	Far(VF("SM_FarRidgeLine"), 180.f, 188.f, 22, 21, 1.f, 1.f, 0.f, -1, -3.574f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 11, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 22, 10, 1.f, 1.f, 0.f, -1, -2.838f, 22.f);
Zone(TEXT("People far: the ash fields, the right"), 13.9f, 16.7f);
	Far(VF("SM_FarRidgeLine"), 180.f, 188.f, 22, 10, 1.f, 1.f, 0.f, 1, -4.89f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 4, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 22, 21, 1.f, 1.f, 0.f, 1, -3.93f, 22.f);
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
	Far(VF("SM_FarRidgeLine"), 180.f, 188.f, 22, 14, 1.f, 1.f, 0.f, -1, -3.574f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 6, 1.f, 1.f, 0.f, -1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 22, 3, 1.f, 1.f, 0.f, -1, -2.838f, 22.f);
Zone(TEXT("People far: the last ash, the right"), 22.3f, 99.f);
	Far(VF("SM_FarRidgeLine"), 180.f, 188.f, 22, 3, 1.f, 1.f, 0.f, 1, -4.89f, 3.f);
	Far(VF("SM_FarSkein_A"), 210.f, 300.f, 14, 13, 1.f, 1.f, 0.f, 1, 0.f, 6.f);
	Far(VF("SM_FarLedgeGoats"), 162.f, 174.f, 22, 14, 1.f, 1.f, 0.f, 1, -3.93f, 22.f);
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
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 16, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 16, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 16, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 16, 8, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the far works, the decks (2)"), 18.23f, 19.5f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 16, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 16, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 16, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 16, 8, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the inner wall, the decks (1)"), 19.5f, 20.11f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 16, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 16, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 16, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 16, 8, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the inner wall, the decks (2)"), 20.24f, 22.11f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 16, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 16, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 16, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 16, 8, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People watch: the last ash, the decks (1)"), 22.3f, 99.f);
	Scatter(VW("SM_WatchDeck_B"), 35.8f, 36.4f, 16, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 35.8f, 36.4f, 16, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(VW("SM_WatchDeck_A"), 41.8f, 42.4f, 16, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(VW("SM_WatchDeck_B"), 41.8f, 42.4f, 16, 8, 1.f, 1.f, 0.f, 1, 0.f);
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
	Scatter(VW("SM_WatchTerrace_A"), 62.3f, 62.9f, 16, 0, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTower"), 64.f, 64.6f, 16, 8, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTerrace_B"), 73.2f, 73.8f, 16, 4, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(VW("SM_WatchTower"), 73.2f, 73.8f, 16, 12, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("People watch: the inner wall, behind the lava"), 19.5f, 22.3f);
	Scatter(VW("SM_WatchTerrace_B"), 62.3f, 62.9f, 15, 2, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTower"), 64.f, 64.6f, 15, 7, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTerrace_A"), 73.2f, 73.8f, 15, 0, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(VW("SM_WatchTower"), 73.2f, 73.8f, 15, 5, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("People watch: the last ash, behind the lava"), 22.3f, 99.f);
	Scatter(VW("SM_WatchTerrace_A"), 62.3f, 62.9f, 16, 2, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTower"), 64.f, 64.6f, 16, 10, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(VW("SM_WatchTerrace_B"), 73.2f, 73.8f, 16, 6, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(VW("SM_WatchTower"), 73.2f, 73.8f, 16, 14, 1.f, 1.f, 0.f, 1, 3.f);
#undef VW
// ---- END watch ----
}

{
// ---- BEGIN p3a (gaps, ft_people_volcano_p3a.py) ----
// FIX ROUND (16 Nov 2026): the eleven one-slot Far zones under the loops and corkscrews are gone (cut with a 45.0 m slot: seven placed nothing,
// four stood a slot early, none was seen in a frame). Knoll rules written 'every 1000' are single slots (phase = 1000 - slot): the slots
// shared with block c's movers at 87 to 92 m were taken out.
// THE GAPS (pass 3, builder p3a, 15 Nov 2026): the general cast of the mountain, at work, at rest and passing through, where nobody
// stood within 60 m. Meshes /Game/People/Volcano/SM_P3A_* (Content/Python/ft_people_volcano_p3a.py). All of them are TALL (tops 3.2 to
// 7 m as built); nothing here is nearer than 42 m. Where they stand, and why:
//  - THE FAR BANKS of the lava channels: 54 m out on the left (the channel is 38 to 52 m, the basalt wall behind it reaches from 52.6 m),
//    62.2 to 63 m on the right (channel 46 to 61 m, wall from 61.1 m, the railway of the mine at 66 m: there the piece is 1.25 big, elsewhere
//    1.4). A bank piece is a trestle deck 1.4 m up, 2.3 m deep, open underneath. Only on the slots that the basalt walls, the lava falls
//    behind the channels, the channel works and the left-hand pump houses do not take (a rule places at slots with
//    (slot + phase) % every == 0). Where those are also the slots of the watchers behind (62 m left, 73.5 m right), the deck stands before them.
//  - BESIDE THE BANKED BENDS (0.9, 1.67, 3.95, 5.22, 6.84 and 7.26 km: the main cast stops there, the land nearer than 40 m is cut away): the
//    same decks on the NEAR bank of the right-hand channel, 43 m out (a deck reaches from 41.6 to 44.4 m), on the slots the vent stacks leave.
//  - KNOLLS of basalt columns with people on them, half as big again, 88 to 90 m out (free between the channel works at 78 m and the
//    haulers and tips at 95 m), not in lava jumps, the caldera or obsidian pass, and off the slots of the ash tips.
//  - UNDER THE LOOPS AND CORKSCREWS (Scatter is left out there): eleven bank pieces by Far rules with the drop of the land (0.8 m), one slot
//    each. They are written first, so that the first piece of each bank mesh is a Far one (the track then gives the mesh no shadow, as the
//    dressing does for its kerbs and walls: the Volcano fills the queue of the shadow maps).
//  - Nothing on the right between 2.4 and 3.9 km (the eruption is watched over that side) and nothing on the left from 0.3 to 0.6 km
//    (the curtain of fire). The slots beside the 21 lava lakes (20 m either way) and under the loops are cut out of every Scatter zone.
//  - No low piece at 17 to 24 m: the kerb of basalt stubs stands 20.9 m out and the barrier hides anything under 2 m there.
// Written by a script (the scratch of this builder: C:/ftwork/jobs/L3p3a/table.py).
#define P3A(n) TEXT("/Game/People/Volcano/SM_P3A_" n)
Zone(TEXT("People p3a: the ash road, 0.04 km"), 0.035f, 0.28f);
	Scatter(P3A("Pass_A"), 54.f, 54.6f, 4, 2, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 8, 4, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Pass_B"), 62.4f, 63.f, 4, 3, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 8, 5, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: the ash road, 0.32 km"), 0.305f, 0.595f);
	Scatter(P3A("Rest_A"), 62.4f, 63.f, 4, 3, 1.f, 1.4f, 0.f, 1, 3.f);
Zone(TEXT("People p3a: the ash road, 0.63 km"), 0.62f, 0.64f);
	Scatter(P3A("Rest_B"), 54.f, 54.6f, 4, 2, 1.f, 1.3f, 0.f, -1, 3.f);
Zone(TEXT("People p3a: the ash road, 0.77 km"), 0.755f, 0.865f);
	Scatter(P3A("Dike"), 62.4f, 63.f, 4, 3, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 8, 5, 1.f, 1.5f, 0.f, 1, 8.f);
	Scatter(P3A("Pass_B"), 54.f, 54.6f, 4, 2, 1.f, 1.3f, 0.f, -1, 3.f);
Zone(TEXT("People p3a: the ash road, 0.90 km"), 0.89f, 1.135f);
	Scatter(P3A("Pass_A"), 62.4f, 63.f, 4, 3, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Rest_A"), 54.f, 54.6f, 4, 2, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 8, 4, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Rest_B"), 42.7f, 43.2f, 2, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("People p3a: lava jumps, 1.17 km"), 1.16f, 1.225f);
	Scatter(P3A("Mend_A"), 54.f, 54.6f, 8, 5, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Pass_B"), 42.7f, 43.2f, 2, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("People p3a: lava jumps, 1.26 km"), 1.25f, 1.36f);
	Scatter(P3A("Mend_B"), 62.2f, 62.6f, 4, 3, 1.f, 1.25f, 0.f, 1, 3.f);
Zone(TEXT("People p3a: lava jumps, 1.53 km"), 1.52f, 1.675f);
	Scatter(P3A("Dike"), 54.f, 54.6f, 8, 5, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Sample_A"), 62.2f, 62.6f, 4, 3, 1.f, 1.25f, 0.f, 1, 3.f);
Zone(TEXT("People p3a: lava jumps, 1.71 km"), 1.7f, 1.855f);
	Scatter(P3A("Pass_B"), 62.2f, 62.6f, 4, 3, 1.f, 1.25f, 0.f, 1, 3.f);
	Scatter(P3A("Mend_B"), 42.7f, 43.2f, 2, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("People p3a: lava jumps, 1.89 km"), 1.88f, 1.945f);
	Scatter(P3A("Mend_B"), 54.f, 54.6f, 8, 5, 1.f, 1.3f, 0.f, -1, 3.f);
Zone(TEXT("People p3a: lava jumps, 2.16 km"), 2.15f, 2.35f);
	Scatter(P3A("Sample_A"), 54.f, 54.6f, 8, 5, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Mend_A"), 62.2f, 62.6f, 4, 3, 1.f, 1.25f, 0.f, 1, 3.f);
Zone(TEXT("People p3a: the caldera, 2.52 km"), 2.51f, 2.53f);
	Scatter(P3A("Sample_A"), 54.f, 54.6f, 8, 0, 1.f, 1.3f, 0.f, -1, 3.f);
Zone(TEXT("People p3a: the caldera, 2.88 km"), 2.87f, 2.935f);
	Scatter(P3A("Sample_B"), 54.f, 54.6f, 8, 0, 1.f, 1.3f, 0.f, -1, 3.f);
Zone(TEXT("People p3a: the caldera, 2.97 km"), 2.96f, 3.385f);
	Scatter(P3A("Rest_A"), 54.f, 54.6f, 8, 0, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Pass_A"), 54.f, 54.6f, 8, 4, 1.f, 1.3f, 0.f, -1, 3.f);
Zone(TEXT("People p3a: the caldera, 3.51 km"), 3.5f, 3.61f);
	Scatter(P3A("Sample_A"), 54.f, 54.6f, 8, 0, 1.f, 1.3f, 0.f, -1, 3.f);
Zone(TEXT("People p3a: fire and brimstone, 3.78 km"), 3.77f, 3.925f);
	Scatter(P3A("Sample_B"), 54.f, 54.6f, 4, 2, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 8, 4, 1.f, 1.5f, 0.f, -1, 8.f);
Zone(TEXT("People p3a: fire and brimstone, 3.96 km"), 3.95f, 4.06f);
	Scatter(P3A("Sample_A"), 54.f, 54.6f, 4, 2, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Mend_B"), 62.4f, 63.f, 4, 3, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Rest_A"), 42.7f, 43.2f, 2, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("People p3a: fire and brimstone, 4.09 km"), 4.085f, 4.375f);
	Scatter(P3A("Mend_B"), 54.f, 54.6f, 4, 2, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 8, 4, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Rest_B"), 62.4f, 63.f, 4, 3, 1.f, 1.4f, 0.f, 1, 3.f);
Zone(TEXT("People p3a: fire and brimstone, 4.54 km"), 4.535f, 4.87f);
	Scatter(P3A("Rest_B"), 54.f, 54.6f, 4, 2, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 8, 4, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Pass_A"), 62.4f, 63.f, 4, 3, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 8, 5, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: fire and brimstone, 5.08 km"), 5.075f, 5.185f);
	Scatter(P3A("Pass_A"), 54.f, 54.6f, 4, 2, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Sample_B"), 62.4f, 63.f, 4, 3, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 8, 5, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: the magma run, 5.22 km"), 5.21f, 5.5f);
	Scatter(P3A("Mend_A"), 54.f, 54.6f, 8, 1, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Dike"), 54.f, 54.6f, 8, 7, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Sample_A"), 62.4f, 63.f, 8, 6, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Pass_A"), 42.7f, 43.2f, 4, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("People p3a: the magma run, 5.54 km"), 5.525f, 5.59f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 8, 4, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: the magma run, 5.71 km"), 5.705f, 5.77f);
	Scatter(P3A("Sample_A"), 54.f, 54.6f, 8, 1, 1.f, 1.3f, 0.f, -1, 3.f);
Zone(TEXT("People p3a: the magma run, 6.12 km"), 6.11f, 6.49f);
	Scatter(P3A("Mend_B"), 54.f, 54.6f, 8, 1, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 8, 5, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Pass_B"), 62.4f, 63.f, 8, 6, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 8, 4, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: obsidian pass, 6.84 km"), 6.83f, 7.03f);
	Scatter(P3A("Pass_A"), 54.f, 54.6f, 8, 4, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Rest_A"), 62.4f, 63.f, 8, 6, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Rest_B"), 42.7f, 43.2f, 4, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("People p3a: obsidian pass, 7.29 km"), 7.28f, 7.345f);
	Scatter(P3A("Pass_B"), 62.4f, 63.f, 8, 6, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Mend_B"), 42.7f, 43.2f, 2, 0, 1.f, 1.f, 0.f, 1, 2.f);
Zone(TEXT("People p3a: obsidian pass, 7.38 km"), 7.37f, 7.435f);
	Scatter(P3A("Sample_B"), 54.f, 54.6f, 8, 4, 1.f, 1.3f, 0.f, -1, 3.f);
Zone(TEXT("People p3a: obsidian pass, 7.56 km"), 7.55f, 7.66f);
	Scatter(P3A("Dike"), 62.4f, 63.f, 8, 6, 1.f, 1.4f, 0.f, 1, 3.f);
Zone(TEXT("People p3a: Engine 13's road, 8.37 km"), 8.36f, 11.08f);
	Scatter(P3A("Mend_A"), 54.f, 54.6f, 16, 12, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Mend_B"), 54.f, 54.6f, 16, 4, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 1000, 808, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 1000, 776, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 1000, 760, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 16, 8, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Dike"), 62.2f, 62.6f, 8, 6, 1.f, 1.25f, 0.f, 1, 3.f);
	Scatter(P3A("Pass_B"), 62.2f, 62.6f, 8, 2, 1.f, 1.25f, 0.f, 1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 1000, 788, 1.f, 1.5f, 0.f, 1, 8.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 1000, 772, 1.f, 1.5f, 0.f, 1, 8.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 1000, 756, 1.f, 1.5f, 0.f, 1, 8.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 1000, 812, 1.f, 1.5f, 0.f, 1, 8.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 1000, 796, 1.f, 1.5f, 0.f, 1, 8.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 1000, 780, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: the burning slopes, 11.12 km"), 11.105f, 13.87f);
	Scatter(P3A("Sample_A"), 54.f, 54.6f, 10, 8, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Dike"), 54.f, 54.6f, 10, 3, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 20, 16, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 20, 6, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Sample_B"), 62.4f, 63.f, 10, 0, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Pass_A"), 62.4f, 63.f, 10, 5, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 20, 18, 1.f, 1.5f, 0.f, 1, 8.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 20, 8, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: the ash fields, 13.90 km"), 13.895f, 14.275f);
	Scatter(P3A("Pass_A"), 54.f, 54.6f, 6, 5, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Rest_A"), 54.f, 54.6f, 12, 7, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 12, 10, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Pass_B"), 62.4f, 63.f, 6, 3, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Rest_B"), 62.4f, 63.f, 12, 11, 1.f, 1.4f, 0.f, 1, 3.f);
Zone(TEXT("People p3a: the ash fields, 14.40 km"), 14.39f, 15.265f);
	Scatter(P3A("Pass_B"), 54.f, 54.6f, 6, 5, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Rest_B"), 54.f, 54.6f, 12, 7, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 12, 10, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Mend_B"), 62.4f, 63.f, 12, 9, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Sample_B"), 62.4f, 63.f, 12, 3, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Pass_A"), 62.4f, 63.f, 12, 11, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 12, 6, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: the ash fields, 15.39 km"), 15.38f, 16.255f);
	Scatter(P3A("Mend_B"), 54.f, 54.6f, 12, 11, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Sample_B"), 54.f, 54.6f, 12, 5, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Pass_A"), 54.f, 54.6f, 12, 7, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 12, 10, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Rest_A"), 62.4f, 63.f, 6, 3, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Pass_B"), 62.4f, 63.f, 12, 11, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 12, 6, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: the ash fields, 16.43 km"), 16.415f, 16.705f);
	Scatter(P3A("Rest_A"), 54.f, 54.6f, 6, 5, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Pass_B"), 54.f, 54.6f, 12, 7, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Rest_B"), 62.4f, 63.f, 6, 3, 1.f, 1.4f, 0.f, 1, 3.f);
Zone(TEXT("People p3a: the far works, 16.74 km"), 16.73f, 17.29f);
	Scatter(P3A("Mend_B"), 54.f, 54.6f, 8, 0, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 8, 4, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Dike"), 62.2f, 62.6f, 4, 2, 1.f, 1.25f, 0.f, 1, 3.f);
Zone(TEXT("People p3a: the far works, 17.41 km"), 17.405f, 18.28f);
	Scatter(P3A("Mend_A"), 54.f, 54.6f, 8, 0, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 1000, 604, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 1000, 596, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Rest_B"), 62.2f, 62.6f, 8, 6, 1.f, 1.25f, 0.f, 1, 3.f);
	Scatter(P3A("Pass_B"), 62.2f, 62.6f, 8, 2, 1.f, 1.25f, 0.f, 1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 8, 0, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: the far works, 18.41 km"), 18.395f, 19.27f);
	Scatter(P3A("Rest_B"), 54.f, 54.6f, 8, 0, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 8, 4, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Sample_B"), 62.2f, 62.6f, 8, 6, 1.f, 1.25f, 0.f, 1, 3.f);
	Scatter(P3A("Mend_B"), 62.2f, 62.6f, 8, 2, 1.f, 1.25f, 0.f, 1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 1000, 592, 1.f, 1.5f, 0.f, 1, 8.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 1000, 576, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: the far works, 19.39 km"), 19.385f, 19.495f);
	Scatter(P3A("Pass_B"), 54.f, 54.6f, 8, 0, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 8, 0, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: the inner wall, 19.53 km"), 19.52f, 20.26f);
	Scatter(P3A("Sample_B"), 54.f, 54.6f, 5, 3, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 10, 6, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Sample_A"), 62.4f, 63.f, 10, 0, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Rest_A"), 62.4f, 63.f, 10, 5, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 10, 7, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: the inner wall, 20.43 km"), 20.42f, 21.295f);
	Scatter(P3A("Sample_A"), 54.f, 54.6f, 10, 8, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Rest_A"), 54.f, 54.6f, 10, 3, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 10, 6, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Mend_A"), 62.4f, 63.f, 10, 0, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Pass_A"), 62.4f, 63.f, 10, 5, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 10, 7, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: the inner wall, 21.42 km"), 21.41f, 22.285f);
	Scatter(P3A("Mend_A"), 54.f, 54.6f, 10, 8, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Pass_A"), 54.f, 54.6f, 10, 3, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 10, 6, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Sample_B"), 62.4f, 63.f, 10, 0, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Sample_A"), 62.4f, 63.f, 10, 5, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 10, 7, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: the last ash, 22.41 km"), 22.4f, 23.275f);
	Scatter(P3A("Pass_B"), 54.f, 54.6f, 12, 0, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Rest_B"), 54.f, 54.6f, 12, 6, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Pass_A"), 54.f, 54.6f, 12, 9, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 12, 2, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Rest_A"), 62.4f, 63.f, 6, 2, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Mend_A"), 62.4f, 63.f, 12, 11, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 1000, 501, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: the last ash, 23.40 km"), 23.39f, 24.265f);
	Scatter(P3A("Rest_A"), 54.f, 54.6f, 6, 0, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Mend_A"), 54.f, 54.6f, 12, 9, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Knoll_B"), 88.f, 90.f, 12, 2, 1.f, 1.5f, 0.f, -1, 8.f);
	Scatter(P3A("Pass_B"), 62.4f, 63.f, 12, 8, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Rest_B"), 62.4f, 63.f, 12, 2, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Pass_A"), 62.4f, 63.f, 12, 11, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3A("Knoll_A"), 88.f, 90.f, 12, 5, 1.f, 1.5f, 0.f, 1, 8.f);
Zone(TEXT("People p3a: the last ash, 24.43 km"), 24.425f, 99.f);
	Scatter(P3A("Dike"), 54.f, 54.6f, 12, 9, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3A("Rest_A"), 62.4f, 63.f, 6, 2, 1.f, 1.4f, 0.f, 1, 3.f);
#undef P3A
// ---- END p3a ----
}

{
// ---- BEGIN p3b (scenes, ft_people_volcano_p3b.py) ----
// FIX ROUND (16 Nov 2026): the left lane is 32.5 to 32.6 m. Rules written 'every 1000' are single slots (phase = 1000 - slot): what is left of
// a rule after the slots were taken out where its stage stood more than 0.5 m inside a piece of the first rounds or of block f.
// STORY SCENES (pass 3, builder p3b). Ten scenes that could only be on Mount Kaldera, one mesh each
// (/Game/People/Volcano/SM_P3B_<Name>): Wedding, Kitchen (the steam kitchen), Shrine (the offering), Rescue (the drill:
// cyan jackets, no vehicle, nothing red), Sample (the heat-suit team), School, HotSpring, Tour, Band, Briefing.
// Where they stand, and why:
//  - On their own open stages 2.6 m up (TALL: every rule here is written past 24 m), in the lane BEHIND the main cast's
//    trestles (28.4 to 28.8 m, 3.5 m deep) and short of the watchers' decks (36 m left, 42 m right) and the channels
//    (38 m left, 46 m right): 32.3 to 32.7 m on the left, 38.3 to 38.7 m on the right (past the right-hand fire
//    crews' deck at 34 m). The stages are 3.2 m deep and open underneath (pieces 3.4 to 4.3 m deep as built; the
//    samplers' ladles reach 2 m out of the back, 5.4 m in all: to 36.4 m on the left, short of the channel).
//  - As built (the generator's log): 2,302 to 3,154 triangles a piece, tops 4.4 to 6.1 m. Nominal count 460 pieces,
//    44 to 50 a mesh, before the track's own leave-outs at the jumps.
//  - Only in the main cast's own near stretches: no banked bends, no set pieces, nothing on the right from 2.4 to
//    3.9 km (the eruption), nothing in the shoot's stretch at the start.
//  - One scene to every second slot on each side (every 10, five scenes a side, the other five on the other side; the
//    sides swap from zone to zone). The phases keep off the slots where the main cast's fire crews throw water over
//    this lane (the left: odd slots in lava jumps, 2 of 4 in fire and brimstone, 0 of 4 in the magma run, 5 of 8 in the
//    pass; the right: even slots in lava jumps, 4 of 5 on the ash road, 0 of 4 in fire and brimstone, 1 of 4 in the
//    magma run) and, where that leaves a choice, off the fumaroles' slots.
//  - No fire crew among them, so nothing here needs keeping off Engine 13's road.
// Written by a script (the builder's scratch: jobs/L3p3b/table.py).
#define P3B(Name) TEXT("/Game/People/Volcano/SM_P3B_" Name)
Zone(TEXT("People p3b: the ash road (1)"), 0.32f, 0.5f);
	Scatter(P3B("Wedding"), 32.5f, 32.6f, 10, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("HotSpring"), 32.5f, 32.6f, 10, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Tour"), 32.5f, 32.6f, 10, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 38.3f, 38.7f, 10, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Briefing"), 38.3f, 38.7f, 10, 8, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: the ash road (2)"), 0.63f, 0.89f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 10, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Band"), 32.5f, 32.6f, 10, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Briefing"), 32.5f, 32.6f, 10, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 10, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 10, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("HotSpring"), 38.3f, 38.7f, 10, 5, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Tour"), 38.3f, 38.7f, 10, 6, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Wedding"), 38.3f, 38.7f, 10, 8, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: lava jumps (1)"), 1.35f, 1.67f);
	Scatter(P3B("Shrine"), 32.5f, 32.6f, 10, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("HotSpring"), 32.5f, 32.6f, 10, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Tour"), 32.5f, 32.6f, 10, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Wedding"), 32.5f, 32.6f, 10, 6, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Kitchen"), 32.5f, 32.6f, 10, 8, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Sample"), 38.3f, 38.7f, 10, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Band"), 38.3f, 38.7f, 10, 3, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Briefing"), 38.3f, 38.7f, 10, 5, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Rescue"), 38.3f, 38.7f, 10, 7, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: lava jumps (2)"), 1.885f, 2.24f);
	Scatter(P3B("Band"), 32.5f, 32.6f, 10, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Briefing"), 32.5f, 32.6f, 10, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 32.5f, 32.6f, 10, 6, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("HotSpring"), 38.3f, 38.7f, 10, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Wedding"), 38.3f, 38.7f, 10, 5, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 10, 7, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Shrine"), 38.3f, 38.7f, 10, 9, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: lava jumps and the caldera, the left"), 2.34f, 2.95f);
	Scatter(P3B("Tour"), 32.5f, 32.6f, 10, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Kitchen"), 32.5f, 32.6f, 10, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Shrine"), 32.5f, 32.6f, 1000, 946, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("HotSpring"), 32.5f, 32.6f, 1000, 948, 1.f, 1.f, 0.f, -1, 0.f);
Zone(TEXT("People p3b: the caldera (2), the left"), 3.31f, 3.76f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 10, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 10, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Band"), 32.5f, 32.6f, 10, 7, 1.f, 1.f, 0.f, -1, 0.f);
Zone(TEXT("People p3b: fire and brimstone (1)"), 4.09f, 4.24f);
	Scatter(P3B("Kitchen"), 32.5f, 32.6f, 10, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Shrine"), 32.5f, 32.6f, 10, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("HotSpring"), 32.5f, 32.6f, 10, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 38.3f, 38.7f, 10, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Sample"), 38.3f, 38.7f, 10, 3, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Band"), 38.3f, 38.7f, 10, 5, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: fire and brimstone (2)"), 4.35f, 5.18f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 1000, 901, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Band"), 32.5f, 32.6f, 1000, 903, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Briefing"), 32.5f, 32.6f, 10, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 1000, 897, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 32.5f, 32.6f, 1000, 889, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Shrine"), 38.3f, 38.7f, 1000, 891, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("HotSpring"), 38.3f, 38.7f, 10, 3, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Tour"), 38.3f, 38.7f, 10, 5, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Wedding"), 38.3f, 38.7f, 1000, 887, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 1000, 899, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: the magma run (1)"), 5.53f, 5.7f);
	Scatter(P3B("Tour"), 32.5f, 32.6f, 10, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Wedding"), 32.5f, 32.6f, 10, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Kitchen"), 32.5f, 32.6f, 10, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Briefing"), 38.3f, 38.7f, 10, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Rescue"), 38.3f, 38.7f, 10, 4, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: the magma run (2)"), 5.8f, 6.38f);
	Scatter(P3B("Briefing"), 32.5f, 32.6f, 10, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 10, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 32.5f, 32.6f, 10, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 10, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Band"), 32.5f, 32.6f, 10, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Tour"), 38.3f, 38.7f, 1000, 870, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Wedding"), 38.3f, 38.7f, 10, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Shrine"), 38.3f, 38.7f, 10, 6, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: the magma run and the gate of the pass"), 6.5f, 6.83f);
	Scatter(P3B("Wedding"), 32.5f, 32.6f, 10, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("HotSpring"), 32.5f, 32.6f, 10, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Tour"), 32.5f, 32.6f, 10, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 38.3f, 38.7f, 10, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Band"), 38.3f, 38.7f, 10, 6, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Briefing"), 38.3f, 38.7f, 10, 8, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: obsidian pass (2)"), 7.08f, 7.25f);
	Scatter(P3B("School"), 32.5f, 32.6f, 10, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 10, 8, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 10, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Tour"), 38.3f, 38.7f, 10, 7, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Wedding"), 38.3f, 38.7f, 10, 9, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: obsidian pass (3)"), 7.44f, 8.18f);
	Scatter(P3B("Shrine"), 32.5f, 32.6f, 1000, 820, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("HotSpring"), 32.5f, 32.6f, 10, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Tour"), 32.5f, 32.6f, 1000, 824, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Kitchen"), 32.5f, 32.6f, 10, 8, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Sample"), 38.3f, 38.7f, 10, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Band"), 38.3f, 38.7f, 10, 3, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Briefing"), 38.3f, 38.7f, 10, 5, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Rescue"), 38.3f, 38.7f, 10, 7, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("School"), 38.3f, 38.7f, 10, 9, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: Engine 13's road"), 8.34f, 11.1f);
	Scatter(P3B("Band"), 32.5f, 32.6f, 10, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Briefing"), 32.5f, 32.6f, 10, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 1000, 815, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 1000, 795, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 1000, 785, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 1000, 765, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 1000, 755, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 32.5f, 32.6f, 1000, 797, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 32.5f, 32.6f, 1000, 777, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 32.5f, 32.6f, 1000, 757, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 1000, 779, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 1000, 759, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("HotSpring"), 38.3f, 38.7f, 10, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Tour"), 38.3f, 38.7f, 10, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Wedding"), 38.3f, 38.7f, 10, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 1000, 806, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 1000, 796, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 1000, 786, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 1000, 776, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 1000, 756, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Shrine"), 38.3f, 38.7f, 1000, 798, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Shrine"), 38.3f, 38.7f, 1000, 788, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Shrine"), 38.3f, 38.7f, 1000, 768, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Shrine"), 38.3f, 38.7f, 1000, 758, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: the burning slopes"), 11.1f, 14.1f);
	Scatter(P3B("Tour"), 32.5f, 32.6f, 1000, 741, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Tour"), 32.5f, 32.6f, 1000, 731, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Tour"), 32.5f, 32.6f, 1000, 711, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Tour"), 32.5f, 32.6f, 1000, 701, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Tour"), 32.5f, 32.6f, 1000, 691, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Wedding"), 32.5f, 32.6f, 1000, 753, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Wedding"), 32.5f, 32.6f, 1000, 743, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Wedding"), 32.5f, 32.6f, 1000, 723, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Wedding"), 32.5f, 32.6f, 1000, 713, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Wedding"), 32.5f, 32.6f, 1000, 703, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Wedding"), 32.5f, 32.6f, 1000, 693, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Kitchen"), 32.5f, 32.6f, 10, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("HotSpring"), 32.5f, 32.6f, 10, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Briefing"), 38.3f, 38.7f, 10, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Rescue"), 38.3f, 38.7f, 10, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("School"), 38.3f, 38.7f, 10, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Sample"), 38.3f, 38.7f, 10, 6, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Band"), 38.3f, 38.7f, 10, 8, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: the ash fields"), 14.23f, 16.1f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 10, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 32.5f, 32.6f, 10, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 1000, 685, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 1000, 675, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 1000, 665, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 1000, 645, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Briefing"), 32.5f, 32.6f, 10, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Wedding"), 38.3f, 38.7f, 10, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 10, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Shrine"), 38.3f, 38.7f, 10, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("HotSpring"), 38.3f, 38.7f, 10, 6, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Tour"), 38.3f, 38.7f, 10, 8, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: the ash fields and the far works"), 16.23f, 18.1f);
	Scatter(P3B("Kitchen"), 32.5f, 32.6f, 10, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Shrine"), 32.5f, 32.6f, 1000, 633, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Shrine"), 32.5f, 32.6f, 1000, 613, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("HotSpring"), 32.5f, 32.6f, 10, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Tour"), 32.5f, 32.6f, 10, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Wedding"), 32.5f, 32.6f, 1000, 639, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Wedding"), 32.5f, 32.6f, 1000, 629, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Wedding"), 32.5f, 32.6f, 1000, 619, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 38.3f, 38.7f, 10, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Sample"), 38.3f, 38.7f, 10, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Band"), 38.3f, 38.7f, 10, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Briefing"), 38.3f, 38.7f, 10, 6, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Rescue"), 38.3f, 38.7f, 1000, 638, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Rescue"), 38.3f, 38.7f, 1000, 628, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Rescue"), 38.3f, 38.7f, 1000, 618, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: the far works and the inner wall"), 18.23f, 20.11f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 1000, 591, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 1000, 581, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 1000, 561, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Band"), 32.5f, 32.6f, 10, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Briefing"), 32.5f, 32.6f, 10, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 1000, 597, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 1000, 577, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 1000, 567, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 1000, 557, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 32.5f, 32.6f, 1000, 589, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 32.5f, 32.6f, 1000, 579, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 32.5f, 32.6f, 1000, 569, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Shrine"), 38.3f, 38.7f, 1000, 590, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Shrine"), 38.3f, 38.7f, 1000, 580, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Shrine"), 38.3f, 38.7f, 1000, 570, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("HotSpring"), 38.3f, 38.7f, 10, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Tour"), 38.3f, 38.7f, 10, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Wedding"), 38.3f, 38.7f, 10, 6, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 1000, 588, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 1000, 578, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 1000, 558, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: the inner wall"), 20.24f, 22.11f);
	Scatter(P3B("HotSpring"), 32.5f, 32.6f, 1000, 551, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("HotSpring"), 32.5f, 32.6f, 1000, 541, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("HotSpring"), 32.5f, 32.6f, 1000, 521, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Tour"), 32.5f, 32.6f, 10, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Wedding"), 32.5f, 32.6f, 1000, 535, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Wedding"), 32.5f, 32.6f, 1000, 525, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Wedding"), 32.5f, 32.6f, 1000, 515, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Kitchen"), 32.5f, 32.6f, 1000, 547, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Kitchen"), 32.5f, 32.6f, 1000, 537, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Shrine"), 32.5f, 32.6f, 1000, 529, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Shrine"), 32.5f, 32.6f, 1000, 519, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Band"), 38.3f, 38.7f, 10, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Briefing"), 38.3f, 38.7f, 10, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Rescue"), 38.3f, 38.7f, 1000, 534, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Rescue"), 38.3f, 38.7f, 1000, 524, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Rescue"), 38.3f, 38.7f, 1000, 514, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("School"), 38.3f, 38.7f, 10, 6, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Sample"), 38.3f, 38.7f, 1000, 548, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Sample"), 38.3f, 38.7f, 1000, 538, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Sample"), 38.3f, 38.7f, 1000, 518, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3b: the last ash"), 22.3f, 99.f);
	Scatter(P3B("Briefing"), 32.5f, 32.6f, 10, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 1000, 503, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 1000, 493, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 1000, 473, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Rescue"), 32.5f, 32.6f, 1000, 453, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 32.5f, 32.6f, 1000, 485, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 32.5f, 32.6f, 1000, 475, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 32.5f, 32.6f, 1000, 465, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("School"), 32.5f, 32.6f, 1000, 455, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 1000, 497, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 1000, 487, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 1000, 477, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Sample"), 32.5f, 32.6f, 1000, 457, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Band"), 32.5f, 32.6f, 1000, 499, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Band"), 32.5f, 32.6f, 1000, 489, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Band"), 32.5f, 32.6f, 1000, 479, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Band"), 32.5f, 32.6f, 1000, 469, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Band"), 32.5f, 32.6f, 1000, 459, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3B("Tour"), 38.3f, 38.7f, 10, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Wedding"), 38.3f, 38.7f, 10, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 1000, 504, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 1000, 494, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 1000, 484, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 1000, 474, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Kitchen"), 38.3f, 38.7f, 1000, 454, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Shrine"), 38.3f, 38.7f, 1000, 506, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Shrine"), 38.3f, 38.7f, 1000, 486, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Shrine"), 38.3f, 38.7f, 1000, 476, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("Shrine"), 38.3f, 38.7f, 1000, 466, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3B("HotSpring"), 38.3f, 38.7f, 10, 8, 1.f, 1.f, 0.f, 1, 0.f);
#undef P3B
// ---- END p3b ----
}

{
// ---- BEGIN p3c (movers, ft_people_volcano_p3c.py) ----
// FIX ROUND (16 Nov 2026): the four catwalk meshes are saved with bounds grown 8 cm across the road (not 60), so the track's 24 m sight
// test no longer pushes them out to 27 m. Skimmers: one side a district, every second of its slots. 'every 1000' = one slot.
// THE MOVERS (pass 3, builder p3c, 15 Nov 2026). Meshes /Game/People/Volcano/SM_P3C_* (Content/Python/ft_people_volcano_p3c.py).
// Everything in this block TRAVELS, along the road (a piece's X is laid along it) or round a point of its own; nothing
// travels towards the road. Every mesh is TALL by the generator's log (top 3.0 to 11.9 m): no rule here is nearer than 25 m.
//  - NEAR, a narrow catwalk 1.8 m up (1.9 m deep, 20 m long), written at 25.2 to 25.4 m, so its road-side edge is at
//    24.25 m: in front of the main cast's trestles (28.6 m), behind the kerb (21 m). Runners_A / _B (villagers running,
//    a dog, a goat), Cyclists (couriers), HoseRun (the fire crew hauling hose reels: yellow, no vehicle, nothing red, and
//    not placed from 8.17 km on, Engine 13's road). Only in the main cast's own near zones (no banked bends, no set
//    pieces, nothing on the right from 2.4 to 3.9 km). One catwalk every 6 slots a side, the two sides out of step.
//    These carry the level's small steady lift (0.035, as the main cast's near pieces): they are past 24 m.
//  - BEHIND THE LAVA, 87 to 95 m (between the watchers' stands, 62 to 86 m, and the mine and ash tips, 96 m on), on a
//    low corduroy road or a narrow-gauge track of their own, 1.3 to 1.7 times life size: MuleTrain, GoatRun, OreTubs,
//    Handcar (pump trolleys), Ropeway (open cars both ways on three pylons, 15 m tall as placed), Whim (a mule gin:
//    mules walking a winding drum round). One every 5 slots a side, the district's cast in turn. Not on the left from
//    0.3 to 0.6 km (the curtain of fire), not on the right from 2.4 to 3.95 km (the eruption).
//  - OVER THE LAVA CHANNELS (45 m left, 53.5 m right): Skimmers, a string of terns 5 to 9 m up, in the districts
//    without channel works.
//  - AWAY FROM THE ROAD: PplP3CUphill, a mule train turned a quarter on a road that climbs the slope, 98 to 134 m out.
// Estimated placed (zone length / 45 m * chance / every, before the track leaves out its jumps and loops): 427.
//    SM_P3C_Cyclists      47.0
//    SM_P3C_GoatRun       37.3
//    SM_P3C_Handcar       35.9
//    SM_P3C_HoseRun        9.7
//    SM_P3C_MuleTrain     51.8
//    SM_P3C_OreTubs       33.1
//    SM_P3C_Ropeway       34.0
//    SM_P3C_Runners_A     48.6
//    SM_P3C_Runners_B     47.0
//    SM_P3C_Skimmers      48.1
//    SM_P3C_Whim          34.9
// Written by a script (the builder's scratch: C:/ftwork/jobs/L3p3c/table.py).

#define P3C(Name) TEXT("/Game/People/Volcano/" Name)

Set(TEXT("PplP3CUphill"), { { P3C("SM_P3C_MuleTrain"), 0.f, 0.f, 0.f, 90.f, 1.f } });

Zone(TEXT("P3C the ash road, the catwalk (1)"), 0.32f, 0.5f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_HoseRun"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C the ash road, the catwalk (2)"), 0.63f, 0.89f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_HoseRun"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C lava jumps, the catwalk (1)"), 1.35f, 1.67f);
	Scatter(P3C("SM_P3C_HoseRun"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C lava jumps, the catwalk (2)"), 1.885f, 2.24f);
	Scatter(P3C("SM_P3C_HoseRun"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C lava jumps, the catwalk (3), the left"), 2.34f, 2.5f);
	Scatter(P3C("SM_P3C_HoseRun"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("P3C the caldera, the catwalk (1), the left"), 2.5f, 2.95f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("P3C the caldera, the catwalk (2), the left"), 3.31f, 3.76f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);

Zone(TEXT("P3C fire and brimstone, the catwalk (1)"), 4.09f, 4.24f);
	Scatter(P3C("SM_P3C_HoseRun"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_HoseRun"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C fire and brimstone, the catwalk (2)"), 4.35f, 5.18f);
	Scatter(P3C("SM_P3C_HoseRun"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_HoseRun"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C the magma run, the catwalk (1)"), 5.53f, 5.7f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_HoseRun"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C the magma run, the catwalk (2)"), 5.8f, 6.38f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_HoseRun"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C the magma run, the catwalk (3)"), 6.5f, 6.73f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_HoseRun"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C obsidian pass, the catwalk (1)"), 6.73f, 6.83f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_HoseRun"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C obsidian pass, the catwalk (2)"), 7.08f, 7.25f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_HoseRun"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C obsidian pass, the catwalk (3)"), 7.44f, 8.17f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_HoseRun"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C Engine 13's road, the catwalk"), 8.34f, 11.1f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C the burning slopes, the catwalk"), 11.1f, 13.9f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C the ash fields, the catwalk (1)"), 13.9f, 14.1f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C the ash fields, the catwalk (2)"), 14.23f, 16.1f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C the ash fields, the catwalk (3)"), 16.23f, 16.7f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C the far works, the catwalk (1)"), 16.7f, 18.1f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C the far works, the catwalk (2)"), 18.23f, 19.5f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C the inner wall, the catwalk (1)"), 19.5f, 20.11f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C the inner wall, the catwalk (2)"), 20.24f, 22.11f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C the last ash, the catwalk"), 22.3f, 99.f);
	Scatter(P3C("SM_P3C_Cyclists"), 25.2f, 25.4f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("SM_P3C_Runners_A"), 25.2f, 25.4f, 12, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("SM_P3C_Runners_B"), 25.2f, 25.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);

Zone(TEXT("P3C the ash road, behind the lava: the left, before the fire"), 0.f, 0.3f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 10, 2, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_GoatRun"), 87.f, 91.f, 10, 7, 1.f, 1.7f, 0.f, -1, 3.f);

Zone(TEXT("P3C the ash road, behind the lava: the left"), 0.6f, 1.17f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 10, 2, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_GoatRun"), 87.f, 91.f, 10, 7, 1.f, 1.7f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Skimmers"), 44.f, 46.f, 16, 2, 1.f, 1.f, 0.f, -1, 4.f);    // over the lava channel

Zone(TEXT("P3C the ash road, behind the lava: the right"), 0.f, 1.17f);
	Scatter(P3C("SM_P3C_GoatRun"), 87.f, 91.f, 15, 4, 1.f, 1.7f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 15, 9, 1.f, 1.6f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Ropeway"), 89.f, 93.f, 15, 14, 1.f, 1.3f, 0.f, 1, 3.f);

Zone(TEXT("P3C lava jumps, behind the lava: the left"), 1.17f, 2.5f);
	Scatter(P3C("SM_P3C_OreTubs"), 87.f, 91.f, 15, 2, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Ropeway"), 89.f, 93.f, 1000, 962, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Handcar"), 87.f, 91.f, 15, 12, 1.f, 1.6f, 0.f, -1, 3.f);

Zone(TEXT("P3C lava jumps, behind the lava: the right"), 1.17f, 2.4f);
	Scatter(P3C("SM_P3C_Handcar"), 87.f, 91.f, 1000, 974, 1.f, 1.6f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Whim"), 92.f, 95.f, 15, 9, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_OreTubs"), 87.f, 91.f, 15, 14, 1.f, 1.6f, 0.f, 1, 3.f);

Zone(TEXT("P3C the caldera, behind the lava: the left"), 2.5f, 3.76f);
	Scatter(P3C("SM_P3C_GoatRun"), 87.f, 91.f, 15, 2, 1.f, 1.7f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Whim"), 92.f, 95.f, 15, 7, 1.f, 1.4f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 1000, 937, 1.f, 1.6f, 0.f, -1, 3.f);

Zone(TEXT("P3C fire and brimstone, behind the lava: the left"), 3.76f, 5.18f);
	Scatter(P3C("SM_P3C_Ropeway"), 89.f, 93.f, 15, 2, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Handcar"), 87.f, 91.f, 15, 7, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 15, 12, 1.f, 1.6f, 0.f, -1, 3.f);

Zone(TEXT("P3C fire and brimstone, behind the lava: the right"), 3.95f, 5.18f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 15, 4, 1.f, 1.6f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Ropeway"), 89.f, 93.f, 15, 9, 1.f, 1.3f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Whim"), 92.f, 95.f, 15, 14, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Skimmers"), 52.5f, 54.5f, 22, 2, 1.f, 1.f, 0.f, 1, 4.f);    // over the lava channel

Zone(TEXT("P3C the magma run, behind the lava: the left"), 5.18f, 6.73f);
	Scatter(P3C("SM_P3C_OreTubs"), 87.f, 91.f, 15, 2, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Whim"), 92.f, 95.f, 15, 7, 1.f, 1.4f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_GoatRun"), 87.f, 91.f, 15, 12, 1.f, 1.7f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Skimmers"), 44.f, 46.f, 1000, 874, 1.f, 1.f, 0.f, -1, 4.f);    // over the lava channel

Zone(TEXT("P3C the magma run, behind the lava: the right"), 5.18f, 6.73f);
	Scatter(P3C("SM_P3C_Whim"), 92.f, 95.f, 1000, 869, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_OreTubs"), 87.f, 91.f, 1000, 859, 1.f, 1.6f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Handcar"), 87.f, 91.f, 1000, 879, 1.f, 1.6f, 0.f, 1, 3.f);

Zone(TEXT("P3C obsidian pass, behind the lava: the left"), 6.73f, 8.34f);
	Scatter(P3C("SM_P3C_GoatRun"), 87.f, 91.f, 15, 2, 1.f, 1.7f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 15, 7, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Ropeway"), 89.f, 93.f, 15, 12, 1.f, 1.3f, 0.f, -1, 3.f);

Zone(TEXT("P3C obsidian pass, behind the lava: the right"), 6.73f, 8.34f);
	Scatter(P3C("SM_P3C_Ropeway"), 89.f, 93.f, 15, 4, 1.f, 1.3f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_GoatRun"), 87.f, 91.f, 15, 9, 1.f, 1.7f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 15, 14, 1.f, 1.6f, 0.f, 1, 3.f);

Zone(TEXT("P3C Engine 13's road, behind the lava: the left"), 8.34f, 11.1f);
	Scatter(P3C("SM_P3C_OreTubs"), 87.f, 91.f, 20, 2, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Handcar"), 87.f, 91.f, 20, 7, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Whim"), 92.f, 95.f, 20, 12, 1.f, 1.4f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Ropeway"), 89.f, 93.f, 20, 17, 1.f, 1.3f, 0.f, -1, 3.f);

Zone(TEXT("P3C Engine 13's road, behind the lava: the right"), 8.34f, 11.1f);
	Scatter(P3C("SM_P3C_Handcar"), 87.f, 91.f, 20, 4, 1.f, 1.6f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Whim"), 92.f, 95.f, 20, 9, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_OreTubs"), 87.f, 91.f, 20, 14, 1.f, 1.6f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Ropeway"), 89.f, 93.f, 20, 19, 1.f, 1.3f, 0.f, 1, 3.f);

Zone(TEXT("P3C the burning slopes, behind the lava: the left"), 11.1f, 13.9f);
	Scatter(P3C("SM_P3C_GoatRun"), 87.f, 91.f, 20, 2, 1.f, 1.7f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Ropeway"), 89.f, 93.f, 20, 7, 1.f, 1.3f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 20, 12, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Whim"), 92.f, 95.f, 20, 17, 1.f, 1.4f, 0.f, -1, 3.f);

Zone(TEXT("P3C the burning slopes, behind the lava: the right"), 11.1f, 13.9f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 20, 4, 1.f, 1.6f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_GoatRun"), 87.f, 91.f, 20, 9, 1.f, 1.7f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Ropeway"), 89.f, 93.f, 20, 14, 1.f, 1.3f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Handcar"), 87.f, 91.f, 20, 19, 1.f, 1.6f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Skimmers"), 52.5f, 54.5f, 30, 9, 1.f, 1.f, 0.f, 1, 4.f);    // over the lava channel

Zone(TEXT("P3C the ash fields, behind the lava: the left"), 13.9f, 16.7f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 20, 2, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_GoatRun"), 87.f, 91.f, 20, 7, 1.f, 1.7f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Handcar"), 87.f, 91.f, 1000, 692, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Handcar"), 87.f, 91.f, 1000, 672, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Handcar"), 87.f, 91.f, 1000, 632, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_OreTubs"), 87.f, 91.f, 20, 17, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Skimmers"), 44.f, 46.f, 30, 5, 1.f, 1.f, 0.f, -1, 4.f);    // over the lava channel

Zone(TEXT("P3C the ash fields, behind the lava: the right"), 13.9f, 16.7f);
	Scatter(P3C("SM_P3C_GoatRun"), 87.f, 91.f, 20, 4, 1.f, 1.7f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 20, 9, 1.f, 1.6f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_OreTubs"), 87.f, 91.f, 20, 14, 1.f, 1.6f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Whim"), 92.f, 95.f, 20, 19, 1.f, 1.4f, 0.f, 1, 3.f);

Zone(TEXT("P3C the far works, behind the lava: the left"), 16.7f, 19.5f);
	Scatter(P3C("SM_P3C_OreTubs"), 87.f, 91.f, 20, 2, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Handcar"), 87.f, 91.f, 20, 7, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Whim"), 92.f, 95.f, 20, 12, 1.f, 1.4f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Ropeway"), 89.f, 93.f, 20, 17, 1.f, 1.3f, 0.f, -1, 3.f);

Zone(TEXT("P3C the far works, behind the lava: the right"), 16.7f, 19.5f);
	Scatter(P3C("SM_P3C_Handcar"), 87.f, 91.f, 20, 4, 1.f, 1.6f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_OreTubs"), 87.f, 91.f, 20, 9, 1.f, 1.6f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Ropeway"), 89.f, 93.f, 20, 14, 1.f, 1.3f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Whim"), 92.f, 95.f, 20, 19, 1.f, 1.4f, 0.f, 1, 3.f);

Zone(TEXT("P3C the inner wall, behind the lava: the left"), 19.5f, 22.3f);
	Scatter(P3C("SM_P3C_Whim"), 92.f, 95.f, 20, 2, 1.f, 1.4f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_GoatRun"), 87.f, 91.f, 20, 7, 1.f, 1.7f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 20, 12, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_OreTubs"), 87.f, 91.f, 20, 17, 1.f, 1.6f, 0.f, -1, 3.f);

Zone(TEXT("P3C the inner wall, behind the lava: the right"), 19.5f, 22.3f);
	Scatter(P3C("SM_P3C_GoatRun"), 87.f, 91.f, 1000, 564, 1.f, 1.7f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_GoatRun"), 87.f, 91.f, 1000, 524, 1.f, 1.7f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Whim"), 92.f, 95.f, 20, 9, 1.f, 1.4f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Handcar"), 87.f, 91.f, 20, 14, 1.f, 1.6f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 20, 19, 1.f, 1.6f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Skimmers"), 52.5f, 54.5f, 30, 7, 1.f, 1.f, 0.f, 1, 4.f);    // over the lava channel

Zone(TEXT("P3C the last ash, behind the lava: the left"), 22.3f, 99.f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 1000, 502, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 1000, 462, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_GoatRun"), 87.f, 91.f, 20, 7, 1.f, 1.7f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_OreTubs"), 87.f, 91.f, 20, 12, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Handcar"), 87.f, 91.f, 20, 17, 1.f, 1.6f, 0.f, -1, 3.f);
	Scatter(P3C("SM_P3C_Skimmers"), 44.f, 46.f, 30, 4, 1.f, 1.f, 0.f, -1, 4.f);    // over the lava channel

Zone(TEXT("P3C the last ash, behind the lava: the right"), 22.3f, 99.f);
	Scatter(P3C("SM_P3C_GoatRun"), 87.f, 91.f, 20, 4, 1.f, 1.7f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_MuleTrain"), 87.f, 91.f, 20, 9, 1.f, 1.6f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Handcar"), 87.f, 91.f, 20, 14, 1.f, 1.6f, 0.f, 1, 3.f);
	Scatter(P3C("SM_P3C_Ropeway"), 89.f, 93.f, 20, 19, 1.f, 1.3f, 0.f, 1, 3.f);

Zone(TEXT("P3C the ash fields, the road up the slope: the left"), 13.9f, 16.7f);
	Site(TEXT("PplP3CUphill"), -1, 116.f, 20, 3, 1.f);

Zone(TEXT("P3C the ash fields, the road up the slope: the right"), 13.9f, 16.7f);
	Site(TEXT("PplP3CUphill"), 1, 116.f, 20, 13, 1.f);

Zone(TEXT("P3C the last ash, the road up the slope: the left"), 22.3f, 99.f);
	Site(TEXT("PplP3CUphill"), -1, 116.f, 20, 8, 1.f);

Zone(TEXT("P3C the last ash, the road up the slope: the right"), 22.3f, 99.f);
	Site(TEXT("PplP3CUphill"), 1, 116.f, 20, 18, 1.f);

Zone(TEXT("P3C the ash road, the road up the slope: the right"), 0.6f, 1.17f);
	Site(TEXT("PplP3CUphill"), 1, 116.f, 20, 8, 1.f);

#undef P3C
// ---- END p3c ----
}

{
// ---- BEGIN p3d (heights, ft_people_volcano_p3d.py) ----
// FIX ROUND (16 Nov 2026): lookout slots shared with a stage of block b, a unit of block f or a first-round piece were taken out
// ('every 1000' = one slot, phase = 1000 - slot); the three lookout meshes carry the level's 0.035 steady lift.
// Volcano, pass 3, builder p3d: people UP HIGH, every piece bringing what it stands on. All of them TALL: no rule here
// starts nearer than 33 m to the centre line. Meshes /Game/People/Volcano/SM_P3D_... (ft_people_volcano_p3d.py).
//  - BEHIND THE LAVA CHANNELS (68.8 to 69.2 m on the left, 70.8 to 71.2 m on the right: behind the basalt walls, the
//    watchers' terraces and towers and the mine's railway): derricks, crags, a rope bridge, a headframe, an arch, signal
//    masts. Their people stand 12.5 m up and more (over the walls) and are 1.8 times life size.
//  - THE LOOKOUTS (33.1 m on the left, 37.2 m on the right: the strip between the main cast's trestles and the watch
//    decks): slim open scaffolds and crow's nests, the people 5 m up and more, nothing solid under the deck. Only in
//    the main cast's own near stretches (no banked bends, no set pieces), never on the right from 2.34 to 3.95 km
//    (the eruption is watched over that side).
// The slots were picked by a script (the builder's scratch, C:/ftwork/jobs/L3p3d/block.py) from the two tables as they
// stood before pass 3: a rule takes only slots where nothing of the dressing or of the population stands within 7 m
// of its distance on its side (2 to 3 m for the lookouts, 8 m from the fire hoses), never two neighbouring slots.
// Chance 1 everywhere. A slot s is taken when (s + phase) % every == 0 (FTTrack.cpp).
// Estimate: 426 written; about 352 placed once the track has left out the loops, corkscrews and jumps and (for the
// lookouts) the jumps' run-ups and landings. The most of any one mesh: 54 (Lookout_B).
#define P3D(n) TEXT("/Game/People/Volcano/SM_P3D_" n)
Zone(TEXT("P3D heights: the ash road, behind the lava, the left"), 0.6f, 1.17f);
	Scatter(P3D("Derrick_A"), 68.8f, 69.2f, 5, 2, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Derrick_B"), 68.8f, 69.2f, 7, 0, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: the ash road, behind the lava, the right"), 0.05f, 1.17f);
	Scatter(P3D("Crag_A"), 70.8f, 71.2f, 12, 3, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("Crag_B"), 70.8f, 71.2f, 15, 13, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("RopeBridge"), 70.8f, 71.2f, 14, 1, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: lava jumps, behind the lava, the left"), 1.17f, 2.5f);
	Scatter(P3D("Headframe"), 68.8f, 69.2f, 8, 7, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Arch"), 68.8f, 69.2f, 15, 7, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Signal"), 68.8f, 69.2f, 16, 2, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: lava jumps, behind the lava, the right"), 1.17f, 2.4f);
	Scatter(P3D("Derrick_A"), 70.8f, 71.2f, 8, 0, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("RopeBridge"), 70.8f, 71.2f, 16, 4, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("Derrick_B"), 70.8f, 71.2f, 16, 11, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: the caldera, behind the lava, the left"), 2.5f, 3.76f);
	Scatter(P3D("Crag_A"), 68.8f, 69.2f, 8, 1, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Crag_B"), 68.8f, 69.2f, 9, 6, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Arch"), 68.8f, 69.2f, 12, 11, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: fire and brimstone, behind the lava, the left"), 3.76f, 5.18f);
	Scatter(P3D("Signal"), 68.8f, 69.2f, 4, 3, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("RopeBridge"), 68.8f, 69.2f, 12, 5, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: fire and brimstone, behind the lava, the right"), 3.95f, 5.18f);
	Scatter(P3D("Arch"), 70.8f, 71.2f, 6, 0, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("Derrick_B"), 70.8f, 71.2f, 6, 4, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: the magma run, behind the lava, the left"), 5.18f, 6.64f);
	Scatter(P3D("Headframe"), 68.8f, 69.2f, 12, 3, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Derrick_A"), 68.8f, 69.2f, 12, 11, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Crag_B"), 68.8f, 69.2f, 12, 6, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Headframe"), 68.8f, 69.2f, 12, 8, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: the magma run, behind the lava, the right"), 5.18f, 6.64f);
	Scatter(P3D("Crag_B"), 70.8f, 71.2f, 6, 1, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("Derrick_A"), 70.8f, 71.2f, 12, 11, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("Derrick_B"), 70.8f, 71.2f, 18, 9, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: Engine 13's road, behind the lava, the left"), 8.34f, 11.1f);
	Scatter(P3D("Crag_A"), 68.8f, 69.2f, 8, 6, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("RopeBridge"), 68.8f, 69.2f, 16, 2, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: Engine 13's road, behind the lava, the right"), 8.34f, 11.1f);
	Scatter(P3D("Headframe"), 70.8f, 71.2f, 16, 9, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("Arch"), 70.8f, 71.2f, 20, 13, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("Derrick_B"), 70.8f, 71.2f, 27, 4, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("RopeBridge"), 70.8f, 71.2f, 29, 8, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("Signal"), 70.8f, 71.2f, 29, 12, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: the burning slopes, behind the lava, the left"), 11.1f, 13.9f);
	Scatter(P3D("Derrick_A"), 68.8f, 69.2f, 5, 3, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Derrick_B"), 68.8f, 69.2f, 10, 1, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: the burning slopes, behind the lava, the right"), 11.1f, 13.9f);
	Scatter(P3D("Crag_B"), 70.8f, 71.2f, 5, 2, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("RopeBridge"), 70.8f, 71.2f, 10, 4, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: the ash fields, behind the lava, the left"), 13.9f, 16.7f);
	Scatter(P3D("Arch"), 68.8f, 69.2f, 6, 5, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Signal"), 68.8f, 69.2f, 12, 3, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Crag_A"), 68.8f, 69.2f, 18, 7, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: the ash fields, behind the lava, the right"), 13.9f, 16.7f);
	Scatter(P3D("Crag_A"), 70.8f, 71.2f, 6, 2, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("Derrick_B"), 70.8f, 71.2f, 18, 11, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("RopeBridge"), 70.8f, 71.2f, 18, 6, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: the far works, behind the lava, the left"), 16.7f, 19.5f);
	Scatter(P3D("Headframe"), 68.8f, 69.2f, 4, 3, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: the far works, behind the lava, the right"), 16.7f, 19.5f);
	Scatter(P3D("Signal"), 70.8f, 71.2f, 4, 1, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: the inner wall, behind the lava, the left"), 19.5f, 22.3f);
	Scatter(P3D("RopeBridge"), 68.8f, 69.2f, 6, 3, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Derrick_B"), 68.8f, 69.2f, 12, 6, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Arch"), 68.8f, 69.2f, 1000, 543, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Arch"), 68.8f, 69.2f, 1000, 519, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Derrick_A"), 68.8f, 69.2f, 24, 1, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: the inner wall, behind the lava, the right"), 19.5f, 22.3f);
	Scatter(P3D("Arch"), 70.8f, 71.2f, 9, 4, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("Crag_B"), 70.8f, 71.2f, 14, 5, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("Derrick_A"), 70.8f, 71.2f, 20, 12, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("Derrick_B"), 70.8f, 71.2f, 27, 20, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("Crag_A"), 70.8f, 71.2f, 24, 4, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: the last ash, behind the lava, the left"), 22.3f, 24.5f);
	Scatter(P3D("Crag_B"), 68.8f, 69.2f, 4, 0, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Derrick_A"), 68.8f, 69.2f, 24, 6, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: the last ash, behind the lava, the right"), 22.3f, 24.5f);
	Scatter(P3D("Derrick_B"), 70.8f, 71.2f, 4, 0, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("Crag_A"), 70.8f, 71.2f, 16, 2, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: the lookouts, 0.32 to 0.5 km"), 0.32f, 0.5f);
	Scatter(P3D("Lookout_A"), 33.1f, 33.2f, 3, 2, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: the lookouts, 0.63 to 0.89 km"), 0.63f, 0.89f);
	Scatter(P3D("Crow"), 33.1f, 33.2f, 3, 2, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Lookout_A"), 37.1f, 37.3f, 1000, 981, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: the lookouts, 1.35 to 1.67 km"), 1.35f, 1.67f);
	Scatter(P3D("Lookout_B"), 37.1f, 37.3f, 8, 3, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: the lookouts, 2.5 to 2.95 km"), 2.5f, 2.95f);
Zone(TEXT("P3D heights: the lookouts, 3.31 to 3.76 km"), 3.31f, 3.76f);
Zone(TEXT("P3D heights: the lookouts, 5.53 to 5.7 km"), 5.53f, 5.7f);
Zone(TEXT("P3D heights: the lookouts, 5.8 to 6.38 km"), 5.8f, 6.38f);
Zone(TEXT("P3D heights: the lookouts, 6.5 to 6.73 km"), 6.5f, 6.73f);
	Scatter(P3D("Lookout_A"), 33.1f, 33.2f, 1000, 853, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: the lookouts, 7.08 to 7.25 km"), 7.08f, 7.25f);
	Scatter(P3D("Crow"), 33.1f, 33.2f, 3, 0, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Lookout_A"), 37.1f, 37.3f, 3, 1, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: the lookouts, 7.44 to 8.17 km"), 7.44f, 8.17f);
	Scatter(P3D("Lookout_B"), 33.1f, 33.2f, 1000, 831, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: the lookouts, 8.34 to 11.1 km"), 8.34f, 11.1f);
Zone(TEXT("P3D heights: the lookouts, 11.1 to 13.9 km"), 11.1f, 13.9f);
Zone(TEXT("P3D heights: the lookouts, 13.9 to 14.1 km"), 13.9f, 14.1f);
	Scatter(P3D("Lookout_B"), 33.1f, 33.2f, 1000, 692, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Lookout_B"), 37.1f, 37.3f, 3, 0, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: the lookouts, 14.23 to 16.1 km"), 14.23f, 16.1f);
	Scatter(P3D("Crow"), 33.1f, 33.2f, 1000, 668, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Crow"), 33.1f, 33.2f, 1000, 662, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Crow"), 33.1f, 33.2f, 1000, 656, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Crow"), 33.1f, 33.2f, 1000, 650, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Crow"), 37.1f, 37.3f, 6, 3, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: the lookouts, 16.23 to 16.7 km"), 16.23f, 16.7f);
	Scatter(P3D("Lookout_A"), 33.1f, 33.2f, 6, 4, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Lookout_B"), 37.1f, 37.3f, 1000, 637, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3D("Lookout_B"), 37.1f, 37.3f, 1000, 631, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: the lookouts, 16.7 to 18.1 km"), 16.7f, 18.1f);
	Scatter(P3D("Lookout_B"), 33.1f, 33.2f, 1000, 626, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Lookout_B"), 33.1f, 33.2f, 1000, 620, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Lookout_B"), 33.1f, 33.2f, 1000, 614, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Lookout_B"), 33.1f, 33.2f, 1000, 602, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: the lookouts, 18.23 to 19.5 km"), 18.23f, 19.5f);
	Scatter(P3D("Crow"), 33.1f, 33.2f, 1000, 594, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Crow"), 33.1f, 33.2f, 1000, 582, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Crow"), 33.1f, 33.2f, 1000, 576, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Crow"), 33.1f, 33.2f, 1000, 570, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: the lookouts, 19.5 to 20.11 km"), 19.5f, 20.11f);
	Scatter(P3D("Lookout_A"), 33.1f, 33.2f, 1000, 558, 1.f, 1.f, 0.f, -1, 3.f);
Zone(TEXT("P3D heights: the lookouts, 20.24 to 22.11 km"), 20.24f, 22.11f);
	Scatter(P3D("Lookout_B"), 33.1f, 33.2f, 1000, 540, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Lookout_B"), 33.1f, 33.2f, 1000, 528, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Lookout_B"), 33.1f, 33.2f, 1000, 516, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3D("Crow"), 37.1f, 37.3f, 6, 3, 1.f, 1.f, 0.f, 1, 3.f);
Zone(TEXT("P3D heights: the lookouts, 22.3 to 24.5 km"), 22.3f, 24.5f);
	Scatter(P3D("Lookout_B"), 37.1f, 37.3f, 6, 3, 1.f, 1.f, 0.f, 1, 3.f);
#undef P3D
// ---- END p3d ----
}

{
// ---- BEGIN p3e (far and sky, ft_people_volcano_p3e.py) ----
// FIX ROUND (16 Nov 2026): SM_P3E_Airship is no longer placed (it read as a missile). No balloons and no kites from 8.17 km on
// (Engine 13's road: the sky ahead is where the boss and its warnings are read); before that the balloons stand every second cycle at
// 300 to 345 m; no kites in lava jumps; the gliders every second cycle on the boss's road.
// FAR AND SKY of the Volcano (pass 3, builder p3e, 15 Nov 2026). Meshes: Content/Python/ft_people_volcano_p3e.py ->
// /Game/People/Volcano/SM_P3E_*. Nothing here stands nearer than 150 m; every piece is TALL (the log: tops 37 to 112 m),
// four to five times life size, carries a steady light of its own (nothing blinks) and was saved with the always-moving
// master (far=True). All of it by Far rules, so it stays beside the jumps, loops and corkscrews; nothing red, no vehicle.
//   the sky (drop 0: the height is in the mesh)
//     150 to 205 m    SM_P3E_Gliders_A: eight hang gliders wheeling on a thermal, 30 to 64 m up
//     195 to 250 m    SM_P3E_Gliders_B: five paragliders in a string along the slope (220 m and round again), to 53 m up
//     235 to 300 m    SM_P3E_Balloons_A (three hot-air balloons circling, 31 to 112 m up) and, from 240 m,
//                     SM_P3E_Balloons_B (three drifting down the wind, 180 m and round again)
//     250 to 295 m    SM_P3E_Airship: the film unit's camera airship, 85 m up, crossing along the road (300 m and round)
//   the slopes (the drop is the land's own at the band's middle: 0.75 m under the road out to 90 m, then rising 4.6 cm a
//   metre on the left and 6 cm on the right; every piece is rooted 25 m into it)
//     216 to 224 m    SM_P3E_FarCrowd_A (the refuge terraces, 45 m tall with their people) and SM_P3E_FarCrowd_B (the ring
//                     dance on a mesa, 37 m)
//     268 to 286 m    SM_P3E_Kites (kites to 71 m from a basalt stack), SM_P3E_Climbers (a rope team on a needle 86 m tall
//                     with its flag), SM_P3E_Cableway (two pylons 52 m tall 260 m apart, six cabins crossing)
// How it is laid: one cycle of 34 slots holds the ten pieces 3 or 4 slots apart (so on one side one of them every
// 150 m or so, and the cableway's 273 m have its band to themselves); each district and side starts the cycle where
// it meets fewest standing pieces of the dressing and of rounds 1 and 2 in the same band (counted by the scratch script
// C:/ftwork/jobs/L3p3e/mkblock.py: 20 such meetings are left). Nothing on the right between 2.4 and 3.9 km (the eruption
// is watched over that side).
// Estimate (slots counted by that script before the track's own clearance test): 313 pieces: Airship 31, Balloons_A 29, Balloons_B 32, Cableway 27, Climbers 32, FarCrowd_A 28, FarCrowd_B 32, Gliders_A 34, Gliders_B 32, Kites 36
#define P3E(n) TEXT("/Game/People/Volcano/SM_P3E_" n)
// A lambda of its own, so the level's population function stays small (C4883).
auto PplP3E = [&]()
{
Zone(TEXT("People p3e: the ash road, the left"), 0.05f, 1.17f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 0, 1.f, 1.f, 0.f, -1, -8.082f, 3.f);
	Far(P3E("Balloons_A"), 300.f, 345.f, 68, 31, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 27, 1.f, 1.f, 0.f, -1, -5.23f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 34, 24, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 20, 1.f, 1.f, 0.f, -1, -7.898f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 14, 1.f, 1.f, 0.f, -1, -5.23f, 15.f);
	Far(P3E("Kites"), 268.f, 280.f, 34, 10, 1.f, 1.f, 0.f, -1, -7.714f, 12.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 34, 7, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 300.f, 345.f, 68, 3, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
Zone(TEXT("People p3e: the ash road, the right"), 0.05f, 1.17f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 6, 1.f, 1.f, 0.f, 1, -10.77f, 3.f);
	Far(P3E("Balloons_A"), 300.f, 345.f, 68, 3, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 33, 1.f, 1.f, 0.f, 1, -7.05f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 34, 30, 1.f, 1.f, 0.f, 1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 26, 1.f, 1.f, 0.f, 1, -10.53f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 20, 1.f, 1.f, 0.f, 1, -7.05f, 15.f);
	Far(P3E("Kites"), 268.f, 280.f, 34, 16, 1.f, 1.f, 0.f, 1, -10.29f, 12.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 34, 13, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 300.f, 345.f, 68, 9, 1.f, 1.f, 0.f, 1, 0.f, 8.f);
Zone(TEXT("People p3e: lava jumps, the left"), 1.17f, 2.5f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 29, 1.f, 1.f, 0.f, -1, -8.082f, 3.f);
	Far(P3E("Balloons_A"), 300.f, 345.f, 68, 26, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 22, 1.f, 1.f, 0.f, -1, -5.23f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 34, 19, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 15, 1.f, 1.f, 0.f, -1, -7.898f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 9, 1.f, 1.f, 0.f, -1, -5.23f, 15.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 34, 2, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 300.f, 345.f, 68, 32, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
Zone(TEXT("People p3e: lava jumps, the right"), 1.17f, 2.4f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 17, 1.f, 1.f, 0.f, 1, -10.77f, 3.f);
	Far(P3E("Balloons_A"), 300.f, 345.f, 68, 14, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 10, 1.f, 1.f, 0.f, 1, -7.05f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 34, 7, 1.f, 1.f, 0.f, 1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 3, 1.f, 1.f, 0.f, 1, -10.53f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 31, 1.f, 1.f, 0.f, 1, -7.05f, 15.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 34, 24, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 300.f, 345.f, 68, 20, 1.f, 1.f, 0.f, 1, 0.f, 8.f);
Zone(TEXT("People p3e: the caldera, the left"), 2.5f, 3.76f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 25, 1.f, 1.f, 0.f, -1, -8.082f, 3.f);
	Far(P3E("Balloons_A"), 300.f, 345.f, 68, 22, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 18, 1.f, 1.f, 0.f, -1, -5.23f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 34, 15, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 11, 1.f, 1.f, 0.f, -1, -7.898f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 5, 1.f, 1.f, 0.f, -1, -5.23f, 15.f);
	Far(P3E("Kites"), 268.f, 280.f, 34, 1, 1.f, 1.f, 0.f, -1, -7.714f, 12.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 34, 32, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 300.f, 345.f, 68, 28, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
Zone(TEXT("People p3e: fire and brimstone, the left"), 3.76f, 5.18f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 20, 1.f, 1.f, 0.f, -1, -8.082f, 3.f);
	Far(P3E("Balloons_A"), 300.f, 345.f, 68, 17, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 13, 1.f, 1.f, 0.f, -1, -5.23f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 34, 10, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 6, 1.f, 1.f, 0.f, -1, -7.898f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 0, 1.f, 1.f, 0.f, -1, -5.23f, 15.f);
	Far(P3E("Kites"), 268.f, 280.f, 34, 30, 1.f, 1.f, 0.f, -1, -7.714f, 12.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 34, 27, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 300.f, 345.f, 68, 23, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
Zone(TEXT("People p3e: fire and brimstone, the right"), 3.9f, 5.18f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 2, 1.f, 1.f, 0.f, 1, -10.77f, 3.f);
	Far(P3E("Balloons_A"), 300.f, 345.f, 68, 33, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 29, 1.f, 1.f, 0.f, 1, -7.05f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 34, 26, 1.f, 1.f, 0.f, 1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 22, 1.f, 1.f, 0.f, 1, -10.53f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 16, 1.f, 1.f, 0.f, 1, -7.05f, 15.f);
	Far(P3E("Kites"), 268.f, 280.f, 34, 12, 1.f, 1.f, 0.f, 1, -10.29f, 12.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 34, 9, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 300.f, 345.f, 68, 5, 1.f, 1.f, 0.f, 1, 0.f, 8.f);
Zone(TEXT("People p3e: the magma run, the left"), 5.18f, 6.73f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 8, 1.f, 1.f, 0.f, -1, -8.082f, 3.f);
	Far(P3E("Balloons_A"), 300.f, 345.f, 68, 5, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 1, 1.f, 1.f, 0.f, -1, -5.23f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 34, 32, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 28, 1.f, 1.f, 0.f, -1, -7.898f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 22, 1.f, 1.f, 0.f, -1, -5.23f, 15.f);
	Far(P3E("Kites"), 268.f, 280.f, 34, 18, 1.f, 1.f, 0.f, -1, -7.714f, 12.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 34, 15, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 300.f, 345.f, 68, 11, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
Zone(TEXT("People p3e: the magma run, the right"), 5.18f, 6.73f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 33, 1.f, 1.f, 0.f, 1, -10.77f, 3.f);
	Far(P3E("Balloons_A"), 300.f, 345.f, 68, 30, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 26, 1.f, 1.f, 0.f, 1, -7.05f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 34, 23, 1.f, 1.f, 0.f, 1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 19, 1.f, 1.f, 0.f, 1, -10.53f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 13, 1.f, 1.f, 0.f, 1, -7.05f, 15.f);
	Far(P3E("Kites"), 268.f, 280.f, 34, 9, 1.f, 1.f, 0.f, 1, -10.29f, 12.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 34, 6, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 300.f, 345.f, 68, 2, 1.f, 1.f, 0.f, 1, 0.f, 8.f);
Zone(TEXT("People p3e: obsidian pass, the left"), 6.73f, 8.34f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 9, 1.f, 1.f, 0.f, -1, -8.082f, 3.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 2, 1.f, 1.f, 0.f, -1, -5.23f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 34, 33, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 29, 1.f, 1.f, 0.f, -1, -7.898f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 23, 1.f, 1.f, 0.f, -1, -5.23f, 15.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 34, 16, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
Zone(TEXT("People p3e: obsidian pass, the sky short of Engine 13's road (fix round)"), 6.73f, 8.17f);
	Far(P3E("Balloons_A"), 300.f, 345.f, 68, 6, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(P3E("Kites"), 268.f, 280.f, 34, 19, 1.f, 1.f, 0.f, -1, -7.714f, 12.f);
	Far(P3E("Balloons_B"), 300.f, 345.f, 68, 12, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
Zone(TEXT("People p3e: obsidian pass, the right"), 6.73f, 8.34f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 22, 1.f, 1.f, 0.f, 1, -10.77f, 3.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 15, 1.f, 1.f, 0.f, 1, -7.05f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 34, 12, 1.f, 1.f, 0.f, 1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 8, 1.f, 1.f, 0.f, 1, -10.53f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 2, 1.f, 1.f, 0.f, 1, -7.05f, 15.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 34, 29, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
Zone(TEXT("People p3e: obsidian pass, the sky short of Engine 13's road (fix round)"), 6.73f, 8.17f);
	Far(P3E("Balloons_A"), 300.f, 345.f, 68, 19, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(P3E("Kites"), 268.f, 280.f, 34, 32, 1.f, 1.f, 0.f, 1, -10.29f, 12.f);
	Far(P3E("Balloons_B"), 300.f, 345.f, 68, 25, 1.f, 1.f, 0.f, 1, 0.f, 8.f);
Zone(TEXT("People p3e: Engine 13's road, the left"), 8.34f, 11.1f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 4, 1.f, 1.f, 0.f, -1, -8.082f, 3.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 31, 1.f, 1.f, 0.f, -1, -5.23f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 68, 28, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 24, 1.f, 1.f, 0.f, -1, -7.898f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 18, 1.f, 1.f, 0.f, -1, -5.23f, 15.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 68, 11, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
Zone(TEXT("People p3e: Engine 13's road, the right"), 8.34f, 11.1f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 22, 1.f, 1.f, 0.f, 1, -10.77f, 3.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 15, 1.f, 1.f, 0.f, 1, -7.05f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 68, 12, 1.f, 1.f, 0.f, 1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 8, 1.f, 1.f, 0.f, 1, -10.53f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 2, 1.f, 1.f, 0.f, 1, -7.05f, 15.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 68, 29, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
Zone(TEXT("People p3e: the burning slopes, the left"), 11.1f, 13.9f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 1, 1.f, 1.f, 0.f, -1, -8.082f, 3.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 28, 1.f, 1.f, 0.f, -1, -5.23f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 68, 25, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 21, 1.f, 1.f, 0.f, -1, -7.898f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 15, 1.f, 1.f, 0.f, -1, -5.23f, 15.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 68, 8, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
Zone(TEXT("People p3e: the burning slopes, the right"), 11.1f, 13.9f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 7, 1.f, 1.f, 0.f, 1, -10.77f, 3.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 0, 1.f, 1.f, 0.f, 1, -7.05f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 68, 31, 1.f, 1.f, 0.f, 1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 27, 1.f, 1.f, 0.f, 1, -10.53f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 21, 1.f, 1.f, 0.f, 1, -7.05f, 15.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 68, 14, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
Zone(TEXT("People p3e: the ash fields, the left"), 13.9f, 16.7f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 1, 1.f, 1.f, 0.f, -1, -8.082f, 3.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 28, 1.f, 1.f, 0.f, -1, -5.23f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 68, 25, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 21, 1.f, 1.f, 0.f, -1, -7.898f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 15, 1.f, 1.f, 0.f, -1, -5.23f, 15.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 68, 8, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
Zone(TEXT("People p3e: the ash fields, the right"), 13.9f, 16.7f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 1, 1.f, 1.f, 0.f, 1, -10.77f, 3.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 28, 1.f, 1.f, 0.f, 1, -7.05f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 68, 25, 1.f, 1.f, 0.f, 1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 21, 1.f, 1.f, 0.f, 1, -10.53f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 15, 1.f, 1.f, 0.f, 1, -7.05f, 15.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 68, 8, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
Zone(TEXT("People p3e: the far works, the left"), 16.7f, 19.5f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 26, 1.f, 1.f, 0.f, -1, -8.082f, 3.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 19, 1.f, 1.f, 0.f, -1, -5.23f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 68, 16, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 12, 1.f, 1.f, 0.f, -1, -7.898f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 6, 1.f, 1.f, 0.f, -1, -5.23f, 15.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 68, 33, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
Zone(TEXT("People p3e: the far works, the right"), 16.7f, 19.5f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 15, 1.f, 1.f, 0.f, 1, -10.77f, 3.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 8, 1.f, 1.f, 0.f, 1, -7.05f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 68, 5, 1.f, 1.f, 0.f, 1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 1, 1.f, 1.f, 0.f, 1, -10.53f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 29, 1.f, 1.f, 0.f, 1, -7.05f, 15.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 68, 22, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
Zone(TEXT("People p3e: the inner wall, the left"), 19.5f, 22.3f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 16, 1.f, 1.f, 0.f, -1, -8.082f, 3.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 9, 1.f, 1.f, 0.f, -1, -5.23f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 68, 6, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 2, 1.f, 1.f, 0.f, -1, -7.898f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 30, 1.f, 1.f, 0.f, -1, -5.23f, 15.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 68, 23, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
Zone(TEXT("People p3e: the inner wall, the right"), 19.5f, 22.3f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 28, 1.f, 1.f, 0.f, 1, -10.77f, 3.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 21, 1.f, 1.f, 0.f, 1, -7.05f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 68, 18, 1.f, 1.f, 0.f, 1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 14, 1.f, 1.f, 0.f, 1, -10.53f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 8, 1.f, 1.f, 0.f, 1, -7.05f, 15.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 68, 1, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
Zone(TEXT("People p3e: the last ash, the left"), 22.3f, 99.f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 17, 1.f, 1.f, 0.f, -1, -8.082f, 3.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 10, 1.f, 1.f, 0.f, -1, -5.23f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 68, 7, 1.f, 1.f, 0.f, -1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 3, 1.f, 1.f, 0.f, -1, -7.898f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 31, 1.f, 1.f, 0.f, -1, -5.23f, 15.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 68, 24, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
Zone(TEXT("People p3e: the last ash, the right"), 22.3f, 99.f);
	Far(P3E("Cableway"), 278.f, 286.f, 34, 23, 1.f, 1.f, 0.f, 1, -10.77f, 3.f);
	Far(P3E("FarCrowd_A"), 216.f, 224.f, 34, 16, 1.f, 1.f, 0.f, 1, -7.05f, 8.f);
	Far(P3E("Gliders_B"), 195.f, 250.f, 68, 13, 1.f, 1.f, 0.f, 1, 0.f, 8.f);
	Far(P3E("Climbers"), 272.f, 284.f, 34, 9, 1.f, 1.f, 0.f, 1, -10.53f, 20.f);
	Far(P3E("FarCrowd_B"), 216.f, 224.f, 34, 3, 1.f, 1.f, 0.f, 1, -7.05f, 15.f);
	Far(P3E("Gliders_A"), 150.f, 205.f, 68, 30, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
};
PplP3E();
#undef P3E
// ---- END p3e ----
}

{
// ---- BEGIN p3f (film shoot, ft_people_volcano_p3f.py) ----
// FIX ROUND (16 Nov 2026): the second row (34.4 / 35.6 m) is gone (it stood in block b's stages and block d's lookouts), and no drone
// team is placed any more (hovering drones 25 m from a road with gunships): their first-row slots hold SecondUnit_A and StuntRig_A.
// MORE OF THE SHOOT (pass 3, builder p3f). Meshes: Content/Python/ft_people_volcano_p3f.py -> /Game/People/Volcano/SM_P3F_*
// (second unit, long-lens tower, two drone teams, two stunt rigs, extras' holding and rehearsal, make-up, press pen, the
// cooling station, a rostrum) and more of the kit /Game/People/Crew/SM_Crew<Piece>_Heat, stood on SM_P3F_Rostrum (Up 1.8 m).
// Every mesh here is TALL (its people stand on a deck 1.8 m up, as the level's first round does): every rule's x min is
// 28.6 m or more (the kit's crane, on the ground, 31.2 m: its arm reaches 5.6 m towards the road). Steady lift only, no lamp,
// nothing blinks, nothing red, no vehicle.
// Where: only inside the first round's own "near" zones (they leave out the banked bends, the set pieces and the right-hand
// side from 2.4 to 3.9 km, the eruption's side), and only on slots that round leaves free on that side:
//  - the first row, 28.6 m (Sites 28.8 m): slots with nothing of the first round nearer than 35 m on that side; from 8.34 km
//    on, half of those, never two next to each other, and none from 150 m before to 110 m after the finale's eleven lava jumps;
//    (and not on the left before 0.6 km: the curtain of fire is watched over that side);
//  - a second row up to 8.34 km (where the first row is full): 34.4 m on the left (short of the channel at 38 m), 35.6 m on
//    the right, only the units that reach over the trestles in front (drones, wire rigs, the long-lens tower), on slots
//    with no watchers' deck, fire hose or pump house on that side.
// A rule with "every 1000" places exactly one slot (phase = 1000 - slot). Slots are 45.243 m; the level was taken as 552.
// Written by C:/ftwork/jobs/L3p3f/mkblock.py (scratch). Estimated placed: 255 slots, 355 pieces (a kit Site is a rostrum
// and one or two kit pieces), before the track's own clearance. No mesh more than 56 times (SM_P3F_Rostrum: 56).
#define P3F(n) TEXT("/Game/People/Volcano/SM_P3F_" n)
#define P3FK(n) TEXT("/Game/People/Crew/SM_Crew" n "_Heat")
Set(TEXT("PplP3FKitA"), {    // a camera position and its sound
	{ P3F("Rostrum"),       0,     0,    0,    0, 1 },
	{ P3FK("Camera"),      -3.6f,  0.6f, 1.8f, 0, 1 },
	{ P3FK("Sound"),        4.8f,  0.2f, 1.8f, 0, 1 },
});
Set(TEXT("PplP3FKitB"), {    // the village and the lights
	{ P3F("Rostrum"),       0,     0,    0,    0, 1 },
	{ P3FK("Village"),     -4.f,   0.8f, 1.8f, 0, 1 },
	{ P3FK("Lights"),       3.8f,  0.5f, 1.8f, 0, 1 },
});
Set(TEXT("PplP3FKitC"), {    // the stunt team, crew at the tape
	{ P3F("Rostrum"),       0,     0,    0,    0, 1 },
	{ P3FK("Stunt"),       -3.2f,  0.9f, 1.8f, 0, 1 },
	{ P3FK("ApplauseB"),    3.4f, -0.2f, 1.8f, 0, 1 },
});
Set(TEXT("PplP3FKitD"), {    // catering, crew at the tape
	{ P3F("Rostrum"),       0,     0,    0,    0, 1 },
	{ P3FK("Catering"),    -4.2f,  0.4f, 1.8f, 0, 1 },
	{ P3FK("ApplauseA"),    3.4f, -0.2f, 1.8f, 0, 1 },
});
Set(TEXT("PplP3FKitE"), {    // the dolly on its rails
	{ P3F("Rostrum"),       0,     0,    0,    0, 1 },
	{ P3FK("Dolly"),        0,     0.6f, 1.8f, 0, 1 },
});
Zone(TEXT("People p3f: the ash road, near (1)"), 0.32f, 0.5f);
Zone(TEXT("People p3f: the ash road, near (2)"), 0.63f, 0.89f);
	Scatter(P3F("SecondUnit_A"), 28.6f, 28.6f, 1000, 982, 1.f, 1.1f, 0.f, -1, 0.f);
	Site(TEXT("PplP3FKitA"), 1, 28.8f, 5, 1, 1.f);
Zone(TEXT("People p3f: lava jumps, near (1)"), 1.35f, 1.67f);
	Scatter(P3F("Extras_A"), 28.6f, 28.6f, 4, 2, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3F("SecondUnit_A"), 28.6f, 28.6f, 4, 1, 1.f, 1.1f, 0.f, 1, 0.f);
Zone(TEXT("People p3f: lava jumps, near (2)"), 1.885f, 2.24f);
	Site(TEXT("PplP3FKitB"), -1, 28.8f, 1000, 954, 1.f);
	Scatter(P3F("MakeUp"), 28.6f, 28.6f, 1000, 953, 1.f, 1.1f, 0.f, 1, 0.f);
Zone(TEXT("People p3f: lava jumps, near (3), the left"), 2.34f, 2.5f);
	Scatter(P3F("StuntRig_A"), 28.6f, 28.6f, 1000, 946, 1.f, 1.1f, 0.f, -1, 0.f);
Zone(TEXT("People p3f: the caldera, near (1), the left"), 2.5f, 2.95f);
	Site(TEXT("PplP3FKitC"), -1, 28.8f, 1000, 939, 1.f);
Zone(TEXT("People p3f: the caldera, near (2), the left"), 3.31f, 3.76f);
Zone(TEXT("People p3f: fire and brimstone, near (1)"), 4.09f, 4.24f);
Zone(TEXT("People p3f: fire and brimstone, near (2)"), 4.35f, 5.18f);
	Scatter(P3F("PressPen"), 28.6f, 28.6f, 8, 1, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3F("CoolTent"), 28.6f, 28.6f, 1000, 901, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3FK("Crane"), 31.2f, 31.2f, 8, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplP3FKitD"), 1, 28.8f, 16, 14, 1.f);
Zone(TEXT("People p3f: the magma run, near (1)"), 5.53f, 5.7f);
Zone(TEXT("People p3f: the magma run, near (2)"), 5.8f, 6.38f);
	Scatter(P3F("SecondUnit_B"), 28.6f, 28.6f, 1000, 870, 1.f, 1.1f, 0.f, 1, 0.f);
	Scatter(P3F("Extras_B"), 28.6f, 28.6f, 1000, 866, 1.f, 1.1f, 0.f, 1, 0.f);
Zone(TEXT("People p3f: the magma run, near (3)"), 6.5f, 6.73f);
	Site(TEXT("PplP3FKitE"), 1, 28.8f, 1000, 854, 1.f);
Zone(TEXT("People p3f: obsidian pass, near (1)"), 6.73f, 6.83f);
	Scatter(P3F("StuntRig_A"), 28.6f, 28.6f, 1000, 850, 1.f, 1.1f, 0.f, 1, 0.f);
Zone(TEXT("People p3f: obsidian pass, near (2)"), 7.08f, 7.25f);
	Scatter(P3F("StuntRig_B"), 28.6f, 28.6f, 1000, 841, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3F("SecondUnit_A"), 28.6f, 28.6f, 1000, 842, 1.f, 1.1f, 0.f, 1, 0.f);
Zone(TEXT("People p3f: obsidian pass, near (3)"), 7.44f, 8.18f);
	Site(TEXT("PplP3FKitA"), -1, 28.8f, 8, 1, 1.f);
	Scatter(P3F("Extras_A"), 28.6f, 28.6f, 8, 6, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3F("SecondUnit_A"), 28.6f, 28.6f, 6, 2, 1.f, 1.1f, 0.f, 1, 0.f);
	Site(TEXT("PplP3FKitB"), 1, 28.8f, 1000, 826, 1.f);
	Scatter(P3F("MakeUp"), 28.6f, 28.6f, 1000, 820, 1.f, 1.1f, 0.f, 1, 0.f);
Zone(TEXT("People p3f: Engine 13's road, near (1)"), 8.34f, 11.1f);
	Scatter(P3F("StuntRig_A"), 28.6f, 28.6f, 20, 2, 1.f, 1.1f, 0.f, -1, 0.f);
	Site(TEXT("PplP3FKitC"), -1, 28.8f, 20, 9, 1.f);
	Scatter(P3F("PressPen"), 28.6f, 28.6f, 20, 12, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3F("CoolTent"), 28.6f, 28.6f, 20, 1, 1.f, 1.1f, 0.f, 1, 0.f);
	Scatter(P3FK("Crane"), 31.2f, 31.2f, 20, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplP3FKitD"), 1, 28.8f, 20, 8, 1.f);
	Scatter(P3F("SecondUnit_B"), 28.6f, 28.6f, 20, 11, 1.f, 1.1f, 0.f, 1, 0.f);
Zone(TEXT("People p3f: the burning slopes, near (1)"), 11.1f, 13.9f);
	Scatter(P3F("Extras_B"), 28.6f, 28.6f, 14, 11, 1.f, 1.1f, 0.f, -1, 0.f);
	Site(TEXT("PplP3FKitE"), -1, 28.8f, 15, 6, 1.f);
	Scatter(P3F("StuntRig_A"), 28.6f, 28.6f, 15, 11, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3F("StuntRig_B"), 28.6f, 28.6f, 15, 13, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3F("SecondUnit_A"), 28.6f, 28.6f, 30, 23, 1.f, 1.1f, 0.f, -1, 0.f);
	Site(TEXT("PplP3FKitA"), 1, 28.8f, 15, 2, 1.f);
	Scatter(P3F("Extras_A"), 28.6f, 28.6f, 15, 7, 1.f, 1.1f, 0.f, 1, 0.f);
	Scatter(P3F("SecondUnit_A"), 28.6f, 28.6f, 15, 10, 1.f, 1.1f, 0.f, 1, 0.f);
	Site(TEXT("PplP3FKitB"), 1, 28.8f, 15, 12, 1.f);
Zone(TEXT("People p3f: the ash fields, near (1)"), 13.9f, 14.1f);
	Scatter(P3F("MakeUp"), 28.6f, 28.6f, 3, 1, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3F("StuntRig_A"), 28.6f, 28.6f, 1000, 691, 1.f, 1.1f, 0.f, 1, 0.f);
Zone(TEXT("People p3f: the ash fields, near (2)"), 14.23f, 16.1f);
	Site(TEXT("PplP3FKitC"), -1, 28.8f, 10, 7, 1.f);
	Scatter(P3F("PressPen"), 28.6f, 28.6f, 17, 9, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3F("CoolTent"), 28.6f, 28.6f, 10, 6, 1.f, 1.1f, 0.f, 1, 0.f);
	Scatter(P3FK("Crane"), 31.2f, 31.2f, 18, 11, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplP3FKitD"), 1, 28.8f, 18, 13, 1.f);
Zone(TEXT("People p3f: the ash fields, near (3)"), 16.23f, 16.7f);
	Scatter(P3F("SecondUnit_B"), 28.6f, 28.6f, 1000, 635, 1.f, 1.1f, 0.f, -1, 0.f);
Zone(TEXT("People p3f: the far works, near (1)"), 16.7f, 18.1f);
	Scatter(P3F("Extras_B"), 28.6f, 28.6f, 10, 1, 1.f, 1.1f, 0.f, -1, 0.f);
	Site(TEXT("PplP3FKitE"), -1, 28.8f, 15, 13, 1.f);
	Scatter(P3F("StuntRig_A"), 28.6f, 28.6f, 20, 5, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3F("StuntRig_B"), 28.6f, 28.6f, 1000, 629, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3F("SecondUnit_A"), 28.6f, 28.6f, 10, 3, 1.f, 1.1f, 0.f, 1, 0.f);
	Site(TEXT("PplP3FKitA"), 1, 28.8f, 16, 10, 1.f);
	Scatter(P3F("Extras_A"), 28.6f, 28.6f, 24, 14, 1.f, 1.1f, 0.f, 1, 0.f);
Zone(TEXT("People p3f: the far works, near (2)"), 18.23f, 19.5f);
	Scatter(P3F("SecondUnit_A"), 28.6f, 28.6f, 11, 9, 1.f, 1.1f, 0.f, -1, 0.f);
	Site(TEXT("PplP3FKitB"), -1, 28.8f, 1000, 587, 1.f);
	Scatter(P3F("MakeUp"), 28.6f, 28.6f, 1000, 584, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3F("StuntRig_A"), 28.6f, 28.6f, 1000, 577, 1.f, 1.1f, 0.f, -1, 0.f);
	Site(TEXT("PplP3FKitC"), 1, 28.8f, 10, 8, 1.f);
	Scatter(P3F("PressPen"), 28.6f, 28.6f, 1000, 585, 1.f, 1.1f, 0.f, 1, 0.f);
	Scatter(P3F("CoolTent"), 28.6f, 28.6f, 1000, 582, 1.f, 1.1f, 0.f, 1, 0.f);
Zone(TEXT("People p3f: the inner wall, near (1)"), 19.5f, 20.11f);
	Scatter(P3FK("Crane"), 31.2f, 31.2f, 5, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("PplP3FKitD"), -1, 28.8f, 1000, 564, 1.f);
	Scatter(P3F("SecondUnit_B"), 28.6f, 28.6f, 4, 2, 1.f, 1.1f, 0.f, 1, 0.f);
	Scatter(P3F("Extras_B"), 28.6f, 28.6f, 1000, 560, 1.f, 1.1f, 0.f, 1, 0.f);
Zone(TEXT("People p3f: the inner wall, near (2)"), 20.24f, 22.11f);
	Site(TEXT("PplP3FKitE"), -1, 28.8f, 12, 2, 1.f);
	Scatter(P3F("StuntRig_A"), 28.6f, 28.6f, 14, 1, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3F("StuntRig_B"), 28.6f, 28.6f, 17, 12, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3F("SecondUnit_A"), 28.6f, 28.6f, 26, 9, 1.f, 1.1f, 0.f, -1, 0.f);
	Site(TEXT("PplP3FKitA"), 1, 28.8f, 10, 4, 1.f);
	Scatter(P3F("Extras_A"), 28.6f, 28.6f, 16, 1, 1.f, 1.1f, 0.f, 1, 0.f);
	Scatter(P3F("SecondUnit_A"), 28.6f, 28.6f, 21, 1, 1.f, 1.1f, 0.f, 1, 0.f);
	Site(TEXT("PplP3FKitB"), 1, 28.8f, 25, 16, 1.f);
Zone(TEXT("People p3f: the last ash, near (1)"), 22.3f, 99.0f);
	Scatter(P3F("MakeUp"), 28.6f, 28.6f, 11, 6, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3F("StuntRig_A"), 28.6f, 28.6f, 17, 2, 1.f, 1.1f, 0.f, -1, 0.f);
	Site(TEXT("PplP3FKitC"), -1, 28.8f, 23, 1, 1.f);
	Scatter(P3F("PressPen"), 28.6f, 28.6f, 28, 2, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3F("CoolTent"), 28.6f, 28.6f, 1000, 503, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(P3FK("Crane"), 31.2f, 31.2f, 18, 13, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3F("SecondUnit_B"), 28.6f, 28.6f, 18, 16, 1.f, 1.1f, 0.f, 1, 0.f);
	Scatter(P3F("Extras_B"), 28.6f, 28.6f, 27, 10, 1.f, 1.1f, 0.f, 1, 0.f);
	Site(TEXT("PplP3FKitE"), 1, 28.8f, 28, 12, 1.f);
	Scatter(P3F("StuntRig_A"), 28.6f, 28.6f, 1000, 501, 1.f, 1.1f, 0.f, 1, 0.f);
#undef P3F
#undef P3FK
// ---- END p3f ----
}
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People3 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1d1 (district 1 "The Ash Road", 0 to 3.76 km: CINDER SPRINGS, a hot-spring spa town; ft_world_volcano_s1d1.py) ----
// 17 meshes: 7 buildings, 4 tall open props, 5 low props, the hero SM_S1D1_SteamClock.
// Estimate (slots counted rule by rule from the 45.24 m slot; see C:/ftwork/jobs/L3s1d1/est.py):
//   streets  left 65 m: 14 slots x 3 = 42; left 70 m (caldera): 7 x 3 = 21; right 70 m: 12 x 3 = 36      ->  99
//   gardens  46 set-slots (left 25, right 21): A 12 x 4, B 13 x 4, C 12 x 4, D 9 x 5                      -> 193
//   low sets by the curtain of fire (left 0.3 to 0.6 km): 3 x 3 -> 9;  two Steam Clocks -> 2
//   303 written; 301 x 0.75 (the track's share left out) + the 2 clocks = about 228 placed (303 if nothing were left out).
//   The most of any one mesh: SM_S1D1_Loungers, 26 written (about 20 placed). The hero: 2.
// Bands and slots used (all even slots: s % 4 == 0 or 2; every Site stands at its slot's middle, nothing slides):
//   buildings  LEFT 0 to 0.26, 0.6 to 2.5 km: anchor 65 m (61 to 69 m), s % 4 == 0 and 2 before the fire, s % 4 == 2 after
//              (the watch terraces and towers at 62 to 65 m hold s % 8 == 0 and 4 there; the channel works' fins and hut
//              at 69 to 70 m hold s % 4 == 0 in lava jumps). LEFT 2.5 to 3.76 km: anchor 70 m, s % 4 == 0 (the lava falls
//              at 57 m hold s % 4 == 2; the terraces and towers in front reach 67.3 m), shallow buildings only.
//              RIGHT 0.14 to 2.4 km: anchor 70 m, s % 4 == 2, shallow buildings only (the wall at 64.5 to 66 m in the
//              ash road and the railway at 66 m in lava jumps stand in front). Nothing on the right from 2.4 km on.
//              Nothing tall on the left from 0.26 to 0.6 km (the curtain of fire).
//   props      anchor 26.7 m: tall open pieces at 26.7 to 27.0 m, low still pieces at 23.6 m (out -3.1). Zones leave
//              out the banked bends (0.90 to 1.24, 1.67 to 1.885, 2.95 to 3.3 km) and the right from 2.4 km.
//   hero       left, 134 m (slot 18, 0.81 to 0.86 km) and 138 m (slot 74, 3.35 to 3.39 km).
#define S1D1(n) TEXT("/Game/World/Volcano/SM_S1D1_" n)
#define S1D1_GARDENS(side) \
	Site(TEXT("WldS1D1GardenA"), side, 26.7f, 8, 0, 1.f); \
	Site(TEXT("WldS1D1GardenB"), side, 26.7f, 8, 6, 1.f); \
	Site(TEXT("WldS1D1GardenC"), side, 26.7f, 8, 4, 1.f); \
	Site(TEXT("WldS1D1GardenD"), side, 26.7f, 8, 2, 1.f);
// The streets: three bath-houses to a slot.          { mesh, along m, out m, up m, yaw deg, scale }
Set(TEXT("WldS1D1StreetA"), {                       // deep (left, 65 m)
	{ S1D1("GuestHouse"),     -15.5f,  1.0f, 0.f,  0.f, 1.f },
	{ S1D1("BathHouse"),       -1.5f,  0.f,  0.f,  0.f, 1.f },
	{ S1D1("BoilerHouse"),     12.5f,  0.4f, 0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D1StreetB"), {
	{ S1D1("ChangingTowers"), -14.5f,  1.0f, 0.f,  0.f, 1.f },
	{ S1D1("TeaPavilion"),      0.5f,  0.f,  0.f,  0.f, 1.f },
	{ S1D1("PumpRoom"),        12.5f,  0.8f, 0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D1StreetC"), {
	{ S1D1("TeaPavilion"),    -16.5f,  0.f,  0.f,  0.f, 1.f },
	{ S1D1("BathHouse"),       -3.5f,  0.f,  0.f,  0.f, 1.f },
	{ S1D1("Laundry"),         11.5f,  1.0f, 0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D1RowA"), {                          // shallow (70 m)
	{ S1D1("GuestHouse"),     -15.0f,  0.f,  0.f,  0.f, 1.f },
	{ S1D1("Laundry"),         -1.0f,  0.f,  0.f,  0.f, 1.f },
	{ S1D1("ChangingTowers"),  14.5f,  0.f,  0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D1RowB"), {
	{ S1D1("PumpRoom"),       -16.0f,  0.f,  0.f,  0.f, 1.f },
	{ S1D1("ChangingTowers"),  -3.0f,  0.f,  0.f,  0.f, 1.f },
	{ S1D1("GuestHouse"),      10.5f,  0.f,  0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D1RowC"), {
	{ S1D1("Laundry"),        -12.5f,  0.f,  0.f,  0.f, 1.f },
	{ S1D1("PumpRoom"),         0.f,   0.f,  0.f,  0.f, 1.f },
	{ S1D1("ChangingTowers"),  13.0f,  0.f,  0.f,  0.f, 1.f },
});
// The gardens by the road: tall open pieces on the anchor (26.7 m), low still pieces 3.1 m nearer (23.6 m).
Set(TEXT("WldS1D1GardenA"), {
	{ S1D1("ParasolRow"),       0.f,   0.f,  0.f,  0.f, 1.f },
	{ S1D1("SteamVane"),       19.0f,  0.3f, 0.f,  0.f, 1.f },
	{ S1D1("Loungers"),       -12.0f, -3.1f, 0.f,  0.f, 1.f },
	{ S1D1("PlungeRim"),        8.0f, -3.1f, 0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D1GardenB"), {
	{ S1D1("TowelLine"),        2.0f,  0.f,  0.f,  0.f, 1.f },
	{ S1D1("ParasolRow"),     -15.0f,  0.2f, 0.f,  0.f, 1.f },
	{ S1D1("TubStack"),        -9.0f, -3.1f, 0.f,  0.f, 1.f },
	{ S1D1("SpringTap"),        6.0f, -3.1f, 0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D1GardenC"), {
	{ S1D1("SunArch"),         -3.0f,  0.f,  0.f,  0.f, 1.f },
	{ S1D1("TowelLine"),       14.0f,  0.2f, 0.f,  0.f, 1.f },
	{ S1D1("TeaBench"),       -12.0f, -3.1f, 0.f,  0.f, 1.f },
	{ S1D1("Loungers"),         6.0f, -3.1f, 0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D1GardenD"), {
	{ S1D1("SteamVane"),       -8.0f,  0.f,  0.f,  0.f, 1.f },
	{ S1D1("SunArch"),          7.0f,  0.2f, 0.f,  0.f, 1.f },
	{ S1D1("PlungeRim"),      -14.0f, -3.1f, 0.f,  0.f, 1.f },
	{ S1D1("TeaBench"),        -3.0f, -3.1f, 0.f,  0.f, 1.f },
	{ S1D1("TubStack"),        13.0f, -3.1f, 0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D1LowA"), {                          // low only: the anchor is 23.6 m
	{ S1D1("Loungers"),       -12.0f,  0.f,  0.f,  0.f, 1.f },
	{ S1D1("SpringTap"),        0.f,   0.f,  0.f,  0.f, 1.f },
	{ S1D1("TubStack"),        11.0f,  0.f,  0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D1LowB"), {
	{ S1D1("PlungeRim"),      -11.0f,  0.f,  0.f,  0.f, 1.f },
	{ S1D1("TeaBench"),         1.0f,  0.f,  0.f,  0.f, 1.f },
	{ S1D1("SpringTap"),       10.0f,  0.f,  0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D1Clock"), { { S1D1("SteamClock"), 0.f, 0.f, 0.f, 0.f, 1.f } });

Zone(TEXT("World s1d1, Cinder Springs: the first street (left, before the fire)"), 0.f, 0.26f);
	Site(TEXT("WldS1D1StreetA"), -1, 65.f, 4, 0, 1.f);
	Site(TEXT("WldS1D1StreetB"), -1, 65.f, 4, 2, 1.f);
	S1D1_GARDENS(-1)
Zone(TEXT("World s1d1, Cinder Springs: by the curtain of fire (left, low pieces only)"), 0.3f, 0.6f);
	Site(TEXT("WldS1D1LowA"), -1, 23.6f, 4, 0, 1.f);
	Site(TEXT("WldS1D1LowB"), -1, 23.6f, 4, 2, 1.f);
Zone(TEXT("World s1d1, Cinder Springs: the left bank past the fire"), 0.6f, 1.17f);
	Site(TEXT("WldS1D1StreetC"), -1, 61.5f, 8, 2, 1.f);   // review round: was 65 m; the backs ran into the pass 3 derricks, signals and arch at 68.9 to 69 m
	Site(TEXT("WldS1D1StreetA"), -1, 61.5f, 8, 6, 1.f);   // review round: was 65 m; the backs ran into the pass 3 derricks, signals and arch at 68.9 to 69 m
Zone(TEXT("World s1d1, Cinder Springs: the gardens past the fire (left)"), 0.6f, 0.89f);
	S1D1_GARDENS(-1)
Zone(TEXT("World s1d1, Cinder Springs: the gardens of the ash road (right)"), 0.f, 0.89f);
	S1D1_GARDENS(1)
Zone(TEXT("World s1d1, Cinder Springs: the right bank"), 0.14f, 2.4f);
	Site(TEXT("WldS1D1RowA"), 1, 69.5f, 12, 10, 1.f);   // review round: was 70.0 m: one mesh on both sides of the 70 m shadow line
	Site(TEXT("WldS1D1RowB"), 1, 69.5f, 24, 18, 1.f);   // review round: was 70.0 m: one mesh on both sides of the 70 m shadow line; was every 12 phase 6 (slots 6 18 30 42), now slots 6 and 30: at slot 18 the changing towers stood in a watch deck (VolWatch_A, 72.2 m)
	Site(TEXT("WldS1D1RowC"), 1, 69.5f, 12, 2, 1.f);   // review round: was 70.0 m: one mesh on both sides of the 70 m shadow line
Zone(TEXT("World s1d1, Cinder Springs: the Steam Clock (0.8 km)"), 0.80f, 0.83f);
	Site(TEXT("WldS1D1Clock"), -1, 134.f, 4, 2, 1.f);
Zone(TEXT("World s1d1, Cinder Springs: the left bank, lava jumps"), 1.17f, 2.5f);
	Site(TEXT("WldS1D1StreetB"), -1, 61.5f, 12, 10, 1.f);   // review round: was 65 m; the backs ran into the pass 3 derricks, signals and arch at 68.9 to 69 m
	Site(TEXT("WldS1D1StreetC"), -1, 61.5f, 12, 6, 1.f);   // review round: was 65 m; the backs ran into the pass 3 derricks, signals and arch at 68.9 to 69 m
	Site(TEXT("WldS1D1StreetA"), -1, 61.5f, 12, 2, 1.f);   // review round: was 65 m; the backs ran into the pass 3 derricks, signals and arch at 68.9 to 69 m
Zone(TEXT("World s1d1, Cinder Springs: the gardens of lava jumps (1)"), 1.25f, 1.66f);
	S1D1_GARDENS(-1)
	S1D1_GARDENS(1)
Zone(TEXT("World s1d1, Cinder Springs: the gardens of lava jumps (2, left)"), 1.89f, 2.5f);
	S1D1_GARDENS(-1)
Zone(TEXT("World s1d1, Cinder Springs: the gardens of lava jumps (2, right, to the eruption's side)"), 1.89f, 2.4f);
	S1D1_GARDENS(1)
Zone(TEXT("World s1d1, Cinder Springs: the caldera (left only)"), 2.5f, 3.76f);
	Site(TEXT("WldS1D1RowA"), -1, 69.95f, 12, 0, 1.f);   // review round: was 70.0 m: one mesh on both sides of the 70 m shadow line
	Site(TEXT("WldS1D1RowB"), -1, 69.95f, 12, 8, 1.f);   // review round: was 70.0 m: one mesh on both sides of the 70 m shadow line
	Site(TEXT("WldS1D1RowC"), -1, 69.95f, 12, 4, 1.f);   // review round: was 70.0 m: one mesh on both sides of the 70 m shadow line
Zone(TEXT("World s1d1, Cinder Springs: the gardens before the loop (left)"), 2.5f, 2.56f);
	S1D1_GARDENS(-1)
Zone(TEXT("World s1d1, Cinder Springs: the gardens after the loop (left)"), 2.86f, 2.94f);
	S1D1_GARDENS(-1)
Zone(TEXT("World s1d1, Cinder Springs: the gardens of the caldera (left)"), 3.31f, 3.76f);
	S1D1_GARDENS(-1)
Zone(TEXT("World s1d1, Cinder Springs: the Steam Clock (3.4 km)"), 3.33f, 3.36f);
	Site(TEXT("WldS1D1Clock"), -1, 138.f, 4, 2, 1.f);
#undef S1D1_GARDENS
#undef S1D1
// ---- END s1d1 ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People3 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1d2 (district 2 "Fire and Brimstone", Stormwatch, ft_world_volcano_s1d2.py) ----
// 16 meshes (7 buildings, 4 low props, 4 tall open props, 1 hero). Everything stands between 3.76 and 8.34 km; nothing
// on the right before 3.95 km (first right-hand slot 88 = 3.98 km); nothing from 6.64 to 6.8 km (the gate of the pass).
// Estimate (brief's rule: slots / every x chance x parts x 0.75):
//   buildings, Sites of 3: (31/8 x 2 + 27/12 x 2 + 32/8 x 6 + 34/8 x 2 + 34/16) = 46.9 sites x 3 x 0.75 = 105
//   tall props: (24 + 24 + 22) slots / 8 x 8 rules x 0.75 = 52.5, and 1 at slot 84                          =  53
//   low props, Sites of 2, chance 0.7: 70 / 8 x 8 rules x 2 x 0.7 x 0.75 = 73.5, and 2 at slot 84             =  75
//   the hero: three zones of one slot each (slots 102, 138, 168), chance 1                                  =   3
//   about 236 placed. The most of one mesh: SM_S1D2_SphereTank, about 20. The hero 3 times.
// Bands and slots used (every 4, 8, 12 or 16; phase % 4 == 0 or 2, so slots s % 4 == 0 or 2 only):
//   low props   17.9 to 18.0 m (in front of the kerb at 20.9, clear of the fence at 18.5), both sides, even slots
//   tall props  left 24.8 to 25.6 m (26.3 to 27.0 in the magma run, where the pipe stands at 24.7); right 26.3 to 27.0 m
//               (behind the catwalk at 25.2, in front of the trestles at 28.4); none beside the banked bends
//   buildings   BEHIND the lava channels and tall enough to top the basalt walls: fire and brimstone left 63.6 m on
//               s % 4 == 2 (the walls and the watchers' tower and terrace take the others), right 69.6 m on s % 12 == 4
//               and 10 (clear of the arch and derrick at 71); the magma run left 63.4 m on every even slot, right
//               67.6 m on s % 4 == 2; obsidian pass left 55.3 m on s % 4 == 0 (between the channel and the columns,
//               the falls take s % 4 == 2), right 64.0 m on s % 8 == 6
//   the hero    RIGHT, 128 m (4.61 km), 146 m (6.24 km, outside the mine at 110 to 126), 116 m (7.60 km, between the
//               two ranks of columns at 96 to 106 and 128 to 138)
// Cyan glow (steady, never blinking) only on the buildings, the tall props and the hero: smallest x min 24.8 m.
#define S1D2(n) TEXT("/Game/World/Volcano/SM_S1D2_" n)
Set(TEXT("WldS1D2YardA"), {                       // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D2("CapShed"),         0.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D2("DrumHall"),       12.5f,  0.4f, 0.f,  -8.f, 1.f },
	{ S1D2("InsulTower"),    -11.5f, -0.6f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D2YardB"), {
	{ S1D2("CoilHouse"),       0.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D2("SphereTank"),      9.5f,  0.5f, 0.f,  20.f, 1.f },
	{ S1D2("WatchOffice"),   -10.f,   0.3f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D2YardC"), {
	{ S1D2("DrumHall"),        0.f,   0.4f, 0.f,   6.f, 1.f },
	{ S1D2("Kiln"),            9.5f, -0.4f, 0.f,   0.f, 1.f },
	{ S1D2("CoilHouse"),     -11.5f,  0.f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D2YardD"), {
	{ S1D2("CapShed"),         0.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D2("WatchOffice"),    10.5f,  0.2f, 0.f,   0.f, 1.f },
	{ S1D2("SphereTank"),    -11.5f, -0.3f, 0.f, -30.f, 1.f },
});
Set(TEXT("WldS1D2RowA"), {
	{ S1D2("InsulTower"),      0.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D2("Kiln"),            9.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D2("SphereTank"),     -8.f,   0.f, 0.f,  40.f, 1.f },
});
Set(TEXT("WldS1D2RowB"), {
	{ S1D2("WatchOffice"),     0.f,   0.4f, 0.f,   0.f, 1.f },
	{ S1D2("InsulTower"),      8.f,   0.f, 0.f,   0.f, 0.85f },
	{ S1D2("Kiln"),           -9.5f,  0.f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D2LowA"), {
	{ S1D2("CoilDrum"),       -8.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D2("InsulBollards"),   7.f,   0.1f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D2LowB"), {
	{ S1D2("ShardPlinths"),   -7.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D2("MeterBench"),      7.f,   0.05f, 0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D2LowC"), {
	{ S1D2("InsulBollards"),  -7.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D2("ShardPlinths"),    8.f,   0.1f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D2LowD"), {
	{ S1D2("MeterBench"),     -7.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D2("CoilDrum"),        7.f,   0.1f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D2Harp"), { { S1D2("StormHarp"), 0.f, 0.f, 0.f, 0.f, 1.f } });

// The stations, behind the lava channels.
Zone(TEXT("World s1d2, Stormwatch: fire and brimstone, the left"), 3.76f, 5.18f);          // slots 84 to 114
	Site(TEXT("WldS1D2YardA"), -1, 63.6f, 8, 2, 1.f);                                          // s % 8 == 6
	Site(TEXT("WldS1D2YardB"), -1, 63.6f, 8, 6, 1.f);                                          // s % 8 == 2
Zone(TEXT("World s1d2, Stormwatch: fire and brimstone, the right"), 3.95f, 5.18f);         // slots 88 to 114
	Site(TEXT("WldS1D2RowA"), 1, 69.1f, 12, 8, 1.f);                                           // s % 12 == 4   // review round: was 69.6 m: the watch office stood at 70.0, on the 70 m shadow line
	Site(TEXT("WldS1D2RowB"), 1, 69.1f, 12, 2, 1.f);                                           // s % 12 == 10   // review round: was 69.6 m: the watch office stood at 70.0, on the 70 m shadow line
Zone(TEXT("World s1d2, Stormwatch: the magma run"), 5.18f, 6.64f);                         // slots 115 to 146
	Site(TEXT("WldS1D2YardC"), -1, 61.8f, 8, 0, 1.f);                                          // s % 8 == 0   // review round: was 63.4 m; backs in the pass 3 crags and headframe at 68.8 to 69 m
	Site(TEXT("WldS1D2YardA"), -1, 61.8f, 8, 4, 1.f);                                          // s % 8 == 4   // review round: was 63.4 m; backs in the pass 3 crags and headframe at 68.8 to 69 m
	Site(TEXT("WldS1D2YardB"), -1, 61.8f, 8, 6, 1.f);                                          // s % 8 == 2   // review round: was 63.4 m; backs in the pass 3 crags and headframe at 68.8 to 69 m
	Site(TEXT("WldS1D2YardD"), -1, 61.8f, 8, 2, 1.f);                                          // s % 8 == 6   // review round: was 63.4 m; backs in the pass 3 crags and headframe at 68.8 to 69 m
	Site(TEXT("WldS1D2YardD"), 1, 69.1f, 8, 2, 1.f);                                           // s % 8 == 6   // review round: was 67.6 m; the drum hall front in a pass 3 piece at 62.8 m
	Site(TEXT("WldS1D2YardC"), 1, 69.1f, 8, 6, 1.f);                                           // s % 8 == 2   // review round: was 67.6 m; the drum hall front in a pass 3 piece at 62.8 m
Zone(TEXT("World s1d2, Stormwatch: obsidian pass"), 6.8f, 8.34f);                          // slots 151 to 184
	Site(TEXT("WldS1D2RowA"), -1, 55.3f, 8, 0, 1.f);                                           // s % 8 == 0
	Site(TEXT("WldS1D2RowB"), -1, 55.3f, 16, 4, 1.f);                                          // s % 16 == 12 (164 is taken at 54 m)
	Site(TEXT("WldS1D2RowA"), 1, 64.f, 8, 2, 1.f);                                             // s % 8 == 6

// The verge: masts and low things. Not beside the banked bends (3.85 to 4.08, 5.22 to 5.525, 6.84 to 7.07, 7.255 to 7.36).
Zone(TEXT("World s1d2, Stormwatch: the first verge, the left"), 3.76f, 3.84f);             // slot 84
	Scatter(S1D2("LightningRod"), 24.8f, 25.6f, 4, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Site(TEXT("WldS1D2LowA"), -1, 17.9f, 4, 0, 1.f);
Zone(TEXT("World s1d2, Stormwatch: the verge, fire and brimstone"), 4.09f, 5.18f);         // slots 91 to 114
	Scatter(S1D2("LightningRod"), 24.8f, 25.6f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(S1D2("CableArch"), 24.8f, 25.6f, 8, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(S1D2("InstrumentVane"), 24.8f, 25.6f, 8, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(S1D2("SignalMast"), 24.8f, 25.6f, 8, 6, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(S1D2("InstrumentVane"), 26.3f, 27.f, 8, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(S1D2("SignalMast"), 26.3f, 27.f, 8, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(S1D2("LightningRod"), 26.3f, 27.f, 8, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(S1D2("CableArch"), 26.3f, 27.f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("WldS1D2LowA"), -1, 17.9f, 8, 0, 0.7f);
	Site(TEXT("WldS1D2LowB"), -1, 17.9f, 8, 4, 0.7f);
	Site(TEXT("WldS1D2LowC"), -1, 17.9f, 8, 2, 0.7f);
	Site(TEXT("WldS1D2LowD"), -1, 17.9f, 8, 6, 0.7f);
	Site(TEXT("WldS1D2LowC"), 1, 17.9f, 8, 0, 0.7f);
	Site(TEXT("WldS1D2LowD"), 1, 17.9f, 8, 4, 0.7f);
	Site(TEXT("WldS1D2LowA"), 1, 17.9f, 8, 2, 0.7f);
	Site(TEXT("WldS1D2LowB"), 1, 17.9f, 8, 6, 0.7f);
Zone(TEXT("World s1d2, Stormwatch: the verge, the magma run"), 5.53f, 6.64f);              // slots 123 to 146
	Scatter(S1D2("SignalMast"), 26.3f, 27.f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(S1D2("InstrumentVane"), 26.3f, 27.f, 8, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(S1D2("CableArch"), 26.3f, 27.f, 8, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(S1D2("LightningRod"), 26.3f, 27.f, 8, 6, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(S1D2("CableArch"), 26.3f, 27.f, 8, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(S1D2("LightningRod"), 26.3f, 27.f, 8, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(S1D2("SignalMast"), 26.3f, 27.f, 8, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(S1D2("InstrumentVane"), 26.3f, 27.f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("WldS1D2LowB"), -1, 17.9f, 8, 0, 0.7f);
	Site(TEXT("WldS1D2LowC"), -1, 17.9f, 8, 4, 0.7f);
	Site(TEXT("WldS1D2LowD"), -1, 17.9f, 8, 2, 0.7f);
	Site(TEXT("WldS1D2LowA"), -1, 17.9f, 8, 6, 0.7f);
	Site(TEXT("WldS1D2LowD"), 1, 17.9f, 8, 0, 0.7f);
	Site(TEXT("WldS1D2LowA"), 1, 17.9f, 8, 4, 0.7f);
	Site(TEXT("WldS1D2LowB"), 1, 17.9f, 8, 2, 0.7f);
	Site(TEXT("WldS1D2LowC"), 1, 17.9f, 8, 6, 0.7f);
Zone(TEXT("World s1d2, Stormwatch: the verge, obsidian pass"), 7.37f, 8.34f);              // slots 163 to 184
	Scatter(S1D2("CableArch"), 24.8f, 25.6f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(S1D2("SignalMast"), 24.8f, 25.6f, 8, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(S1D2("LightningRod"), 24.8f, 25.6f, 8, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(S1D2("InstrumentVane"), 24.8f, 25.6f, 8, 6, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(S1D2("LightningRod"), 26.3f, 27.f, 8, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(S1D2("InstrumentVane"), 26.3f, 27.f, 8, 4, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(S1D2("CableArch"), 26.3f, 27.f, 8, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(S1D2("SignalMast"), 26.3f, 27.f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("WldS1D2LowC"), -1, 17.9f, 8, 0, 0.7f);
	Site(TEXT("WldS1D2LowD"), -1, 17.9f, 8, 4, 0.7f);
	Site(TEXT("WldS1D2LowA"), -1, 17.9f, 8, 2, 0.7f);
	Site(TEXT("WldS1D2LowB"), -1, 17.9f, 8, 6, 0.7f);
	Site(TEXT("WldS1D2LowA"), 1, 17.9f, 8, 0, 0.7f);
	Site(TEXT("WldS1D2LowB"), 1, 17.9f, 8, 4, 0.7f);
	Site(TEXT("WldS1D2LowC"), 1, 17.9f, 8, 2, 0.7f);
	Site(TEXT("WldS1D2LowD"), 1, 17.9f, 8, 6, 0.7f);

// The hero, the Storm Harp: three times, on the right, one slot each.
Zone(TEXT("World s1d2, Stormwatch: the Storm Harp at 4.61 km"), 4.6f, 4.63f);              // slot 102
	Site(TEXT("WldS1D2Harp"), 1, 128.f, 4, 2, 1.f);
Zone(TEXT("World s1d2, Stormwatch: the Storm Harp at 6.24 km"), 6.23f, 6.26f);             // slot 138
	Site(TEXT("WldS1D2Harp"), 1, 146.f, 4, 2, 1.f);
Zone(TEXT("World s1d2, Stormwatch: the Storm Harp at 7.60 km"), 7.59f, 7.62f);             // slot 168
	Site(TEXT("WldS1D2Harp"), 1, 116.f, 4, 0, 1.f);
#undef S1D2
// ---- END s1d2 ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People3 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1d3 (district 3 "Engine 13's Road" = SLAGTOWN, the miners' company town, 8.34 to 11.1 km, ft_world_volcano_s1d3.py) ----
// 18 meshes: 9 buildings (timber and tin sheds on stilts), 8 props (4 low, 4 tall and open), 1 hero (THE HOPPER, a 45 m ore tower).
// Every rule is one Site on one slot (every 1000, phase 1000 - slot: the way pass 3 names a slot), even slots 186 to 244 only
// (s % 4 == 0 or 2). Estimate: 90 Sites, 247 pieces as written x 0.75 = about 185 placed.
// Most of one mesh as written: SackBarrow 21, PayOffice 20, TankHouse 20. Smallest x of a TALL piece: 25.4 m (the tall open props); of a low piece: 24.8 m.
// LEFT, buildings (all inside 70 m: they cast): thin rows 63.4 m out on s % 8 == 2 and s % 16 == 14 (between the basalt wall, back
//   at 61.2 m, and the pass 3 crags and rope bridges, front at 65.5 m); thin rows 69.4 m out on s % 12 == 0 and 8 (behind the watch
//   terraces and towers, backs at 66.6 and 67.3 m); deep rows 64.5 m out on s % 12 == 4 and s % 16 == 6 (nothing of pass 3 there).
// RIGHT, buildings 72.5 m out (NOT inside the brief's 26 to 70 m: the mine's railway and the slag trains' sheds reach 69 m; three
//   meshes used only here, all outside 70 m: no shadow), on s % 12 == 2, 4, 8, 10 without 212, 220, 224 (pass 3 heights at 71 m).
// VERGE, both sides: anchor 24.8 m (behind the kerb, in front of the people rows at 26.8 m and more): low pieces at 24.8, tall open
//   pieces at 25.4. Left: every even slot but 188 198 208 218 228 238 (stunt rigs, press pens). Right: s % 12 == 0, 4, 6, 10 (no
//   catwalk at 25.2 m there) without 192 214 232 234 (film sets).
// HERO: left, 84 m out, slots 194, 212, 230 (8.80, 9.61, 10.43 km): in front of the mine at 104 m and more, clear of the knolls.
#define S1D3(n) TEXT("/Game/World/Volcano/SM_S1D3_" n)
Set(TEXT("WldS1D3ThinA"), {
	{ S1D3("RowHouses"), -12.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("PayOffice"), 0.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("LampRoom"), 12.5f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3ThinB"), {
	{ S1D3("TankHouse"), -17.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("LampRoom"), -6.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("RowHouses"), 10.5f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3ThinC"), {
	{ S1D3("PayOffice"), -16.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("RowHouses"), -3.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("TankHouse"), 9.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3ThinD"), {
	{ S1D3("LampRoom"), -13.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("TankHouse"), -2.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("PayOffice"), 6.5f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3DeepA"), {
	{ S1D3("Bunkhouse"), -11.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("CookHouse"), 5.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("TankHouse"), 15.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3DeepB"), {
	{ S1D3("CookHouse"), -14.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("Bunkhouse"), 1.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("PayOffice"), 15.5f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3RailA"), {
	{ S1D3("CompanyStore"), -10.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("AssayShed"), 9.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3RailB"), {
	{ S1D3("BandHall"), -10.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("CompanyStore"), 9.5f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3RailC"), {
	{ S1D3("AssayShed"), -10.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("BandHall"), 9.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3VergeA"), {
	{ S1D3("WashLine"), -12.0f, 0.6f, 0.f, 0.f, 1.f },
	{ S1D3("SackBarrow"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("PitProps"), 11.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3VergeB"), {
	{ S1D3("PickRack"), -12.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("NoticeBoard"), -1.0f, 0.6f, 0.f, 0.f, 1.f },
	{ S1D3("TinBath"), 10.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3VergeC"), {
	{ S1D3("SwingSeat"), -11.0f, 0.6f, 0.f, 0.f, 1.f },
	{ S1D3("PitProps"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("StiltLarder"), 11.0f, 0.6f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3VergeD"), {
	{ S1D3("TinBath"), -11.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("WashLine"), 1.0f, 0.6f, 0.f, 0.f, 1.f },
	{ S1D3("PickRack"), 13.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3VergeE"), {
	{ S1D3("SackBarrow"), -12.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("StiltLarder"), -1.0f, 0.6f, 0.f, 0.f, 1.f },
	{ S1D3("NoticeBoard"), 10.0f, 0.6f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3VergeF"), {
	{ S1D3("NoticeBoard"), -12.0f, 0.6f, 0.f, 0.f, 1.f },
	{ S1D3("SackBarrow"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D3("SwingSeat"), 11.0f, 0.6f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3Hopper"), {
	{ S1D3("Hopper"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1d3, Slagtown (Engine 13's road)"), 8.34f, 11.1f);
	// buildings, left
	Site(TEXT("WldS1D3ThinA"), -1, 63.4f, 1000, 814, 1.f);    // slot 186, 8.44 km
	Site(TEXT("WldS1D3ThinB"), -1, 69.4f, 1000, 812, 1.f);    // slot 188, 8.53 km
	Site(TEXT("WldS1D3ThinC"), -1, 63.4f, 1000, 810, 1.f);    // slot 190, 8.62 km
	Site(TEXT("WldS1D3ThinD"), -1, 69.4f, 1000, 808, 1.f);    // slot 192, 8.71 km
	Site(TEXT("WldS1D3ThinA"), -1, 63.4f, 1000, 806, 1.f);    // slot 194, 8.80 km
	Site(TEXT("WldS1D3DeepA"), -1, 64.5f, 1000, 804, 1.f);    // slot 196, 8.89 km
	Site(TEXT("WldS1D3DeepB"), -1, 64.5f, 1000, 802, 1.f);    // slot 198, 8.98 km
	Site(TEXT("WldS1D3ThinB"), -1, 69.4f, 1000, 800, 1.f);    // slot 200, 9.07 km
	Site(TEXT("WldS1D3ThinC"), -1, 63.4f, 1000, 798, 1.f);    // slot 202, 9.16 km
	Site(TEXT("WldS1D3ThinD"), -1, 69.4f, 1000, 796, 1.f);    // slot 204, 9.25 km
	Site(TEXT("WldS1D3ThinA"), -1, 63.4f, 1000, 794, 1.f);    // slot 206, 9.34 km
	Site(TEXT("WldS1D3DeepA"), -1, 64.5f, 1000, 792, 1.f);    // slot 208, 9.43 km
	Site(TEXT("WldS1D3ThinB"), -1, 63.4f, 1000, 790, 1.f);    // slot 210, 9.52 km
	Site(TEXT("WldS1D3ThinC"), -1, 69.4f, 1000, 788, 1.f);    // slot 212, 9.61 km
	Site(TEXT("WldS1D3DeepB"), -1, 64.5f, 1000, 786, 1.f);    // slot 214, 9.70 km
	Site(TEXT("WldS1D3ThinD"), -1, 69.4f, 1000, 784, 1.f);    // slot 216, 9.80 km
	Site(TEXT("WldS1D3ThinA"), -1, 63.4f, 1000, 782, 1.f);    // slot 218, 9.89 km
	Site(TEXT("WldS1D3DeepA"), -1, 64.5f, 1000, 780, 1.f);    // slot 220, 9.98 km
	Site(TEXT("WldS1D3ThinB"), -1, 63.4f, 1000, 778, 1.f);    // slot 222, 10.07 km
	Site(TEXT("WldS1D3ThinC"), -1, 69.4f, 1000, 776, 1.f);    // slot 224, 10.16 km
	Site(TEXT("WldS1D3ThinD"), -1, 63.4f, 1000, 774, 1.f);    // slot 226, 10.25 km
	Site(TEXT("WldS1D3ThinA"), -1, 69.4f, 1000, 772, 1.f);    // slot 228, 10.34 km
	Site(TEXT("WldS1D3DeepB"), -1, 64.5f, 1000, 770, 1.f);    // slot 230, 10.43 km
	Site(TEXT("WldS1D3DeepA"), -1, 64.5f, 1000, 768, 1.f);    // slot 232, 10.52 km
	Site(TEXT("WldS1D3ThinB"), -1, 63.4f, 1000, 766, 1.f);    // slot 234, 10.61 km
	Site(TEXT("WldS1D3ThinC"), -1, 69.4f, 1000, 764, 1.f);    // slot 236, 10.70 km
	Site(TEXT("WldS1D3ThinD"), -1, 63.4f, 1000, 762, 1.f);    // slot 238, 10.79 km
	Site(TEXT("WldS1D3ThinA"), -1, 69.4f, 1000, 760, 1.f);    // slot 240, 10.88 km
	Site(TEXT("WldS1D3ThinB"), -1, 63.4f, 1000, 758, 1.f);    // slot 242, 10.97 km
	Site(TEXT("WldS1D3DeepB"), -1, 64.5f, 1000, 756, 1.f);    // slot 244, 11.06 km
	// buildings, right
	Site(TEXT("WldS1D3RailA"), 1, 72.5f, 1000, 812, 1.f);    // slot 188, 8.53 km
	Site(TEXT("WldS1D3RailB"), 1, 72.5f, 1000, 810, 1.f);    // slot 190, 8.62 km
	Site(TEXT("WldS1D3RailC"), 1, 72.5f, 1000, 806, 1.f);    // slot 194, 8.80 km
	Site(TEXT("WldS1D3RailA"), 1, 72.5f, 1000, 804, 1.f);    // slot 196, 8.89 km
	Site(TEXT("WldS1D3RailB"), 1, 72.5f, 1000, 800, 1.f);    // slot 200, 9.07 km
	Site(TEXT("WldS1D3RailC"), 1, 72.5f, 1000, 798, 1.f);    // slot 202, 9.16 km
	Site(TEXT("WldS1D3RailA"), 1, 72.5f, 1000, 794, 1.f);    // slot 206, 9.34 km
	Site(TEXT("WldS1D3RailB"), 1, 72.5f, 1000, 792, 1.f);    // slot 208, 9.43 km
	Site(TEXT("WldS1D3RailC"), 1, 72.5f, 1000, 786, 1.f);    // slot 214, 9.70 km
	Site(TEXT("WldS1D3RailA"), 1, 72.5f, 1000, 782, 1.f);    // slot 218, 9.89 km
	Site(TEXT("WldS1D3RailB"), 1, 72.5f, 1000, 774, 1.f);    // slot 226, 10.25 km
	Site(TEXT("WldS1D3RailC"), 1, 72.5f, 1000, 770, 1.f);    // slot 230, 10.43 km
	Site(TEXT("WldS1D3RailA"), 1, 72.5f, 1000, 768, 1.f);    // slot 232, 10.52 km
	Site(TEXT("WldS1D3RailB"), 1, 72.5f, 1000, 764, 1.f);    // slot 236, 10.70 km
	Site(TEXT("WldS1D3RailC"), 1, 72.5f, 1000, 762, 1.f);    // slot 238, 10.79 km
	Site(TEXT("WldS1D3RailA"), 1, 72.5f, 1000, 758, 1.f);    // slot 242, 10.97 km
	Site(TEXT("WldS1D3RailB"), 1, 72.5f, 1000, 756, 1.f);    // slot 244, 11.06 km
	// the verge, left
	Site(TEXT("WldS1D3VergeA"), -1, 24.8f, 1000, 814, 1.f);    // slot 186, 8.44 km
	Site(TEXT("WldS1D3VergeB"), -1, 24.8f, 1000, 810, 1.f);    // slot 190, 8.62 km
	Site(TEXT("WldS1D3VergeC"), -1, 24.8f, 1000, 808, 1.f);    // slot 192, 8.71 km
	Site(TEXT("WldS1D3VergeD"), -1, 24.8f, 1000, 806, 1.f);    // slot 194, 8.80 km
	Site(TEXT("WldS1D3VergeE"), -1, 24.8f, 1000, 804, 1.f);    // slot 196, 8.89 km
	Site(TEXT("WldS1D3VergeF"), -1, 24.8f, 1000, 800, 1.f);    // slot 200, 9.07 km
	Site(TEXT("WldS1D3VergeA"), -1, 24.8f, 1000, 798, 1.f);    // slot 202, 9.16 km
	Site(TEXT("WldS1D3VergeB"), -1, 24.8f, 1000, 796, 1.f);    // slot 204, 9.25 km
	Site(TEXT("WldS1D3VergeC"), -1, 24.8f, 1000, 794, 1.f);    // slot 206, 9.34 km
	Site(TEXT("WldS1D3VergeD"), -1, 24.8f, 1000, 790, 1.f);    // slot 210, 9.52 km
	Site(TEXT("WldS1D3VergeE"), -1, 24.8f, 1000, 788, 1.f);    // slot 212, 9.61 km
	Site(TEXT("WldS1D3VergeF"), -1, 24.8f, 1000, 786, 1.f);    // slot 214, 9.70 km
	Site(TEXT("WldS1D3VergeA"), -1, 24.8f, 1000, 784, 1.f);    // slot 216, 9.80 km
	Site(TEXT("WldS1D3VergeB"), -1, 24.8f, 1000, 780, 1.f);    // slot 220, 9.98 km
	Site(TEXT("WldS1D3VergeC"), -1, 24.8f, 1000, 778, 1.f);    // slot 222, 10.07 km
	Site(TEXT("WldS1D3VergeD"), -1, 24.8f, 1000, 776, 1.f);    // slot 224, 10.16 km
	Site(TEXT("WldS1D3VergeE"), -1, 24.8f, 1000, 774, 1.f);    // slot 226, 10.25 km
	Site(TEXT("WldS1D3VergeF"), -1, 24.8f, 1000, 770, 1.f);    // slot 230, 10.43 km
	Site(TEXT("WldS1D3VergeA"), -1, 24.8f, 1000, 768, 1.f);    // slot 232, 10.52 km
	Site(TEXT("WldS1D3VergeB"), -1, 24.8f, 1000, 766, 1.f);    // slot 234, 10.61 km
	Site(TEXT("WldS1D3VergeC"), -1, 24.8f, 1000, 764, 1.f);    // slot 236, 10.70 km
	Site(TEXT("WldS1D3VergeD"), -1, 24.8f, 1000, 760, 1.f);    // slot 240, 10.88 km
	Site(TEXT("WldS1D3VergeE"), -1, 24.8f, 1000, 758, 1.f);    // slot 242, 10.97 km
	Site(TEXT("WldS1D3VergeF"), -1, 24.8f, 1000, 756, 1.f);    // slot 244, 11.06 km
	// the verge, right
	Site(TEXT("WldS1D3VergeD"), 1, 24.8f, 1000, 814, 1.f);    // slot 186, 8.44 km
	Site(TEXT("WldS1D3VergeE"), 1, 24.8f, 1000, 810, 1.f);    // slot 190, 8.62 km
	Site(TEXT("WldS1D3VergeF"), 1, 24.8f, 1000, 804, 1.f);    // slot 196, 8.89 km
	Site(TEXT("WldS1D3VergeA"), 1, 24.8f, 1000, 802, 1.f);    // slot 198, 8.98 km
	Site(TEXT("WldS1D3VergeB"), 1, 24.8f, 1000, 798, 1.f);    // slot 202, 9.16 km
	Site(TEXT("WldS1D3VergeC"), 1, 24.8f, 1000, 796, 1.f);    // slot 204, 9.25 km
	Site(TEXT("WldS1D3VergeD"), 1, 24.8f, 1000, 792, 1.f);    // slot 208, 9.43 km
	Site(TEXT("WldS1D3VergeE"), 1, 24.8f, 1000, 790, 1.f);    // slot 210, 9.52 km
	Site(TEXT("WldS1D3VergeF"), 1, 24.8f, 1000, 784, 1.f);    // slot 216, 9.80 km
	Site(TEXT("WldS1D3VergeA"), 1, 24.8f, 1000, 780, 1.f);    // slot 220, 9.98 km
	Site(TEXT("WldS1D3VergeB"), 1, 24.8f, 1000, 778, 1.f);    // slot 222, 10.07 km
	Site(TEXT("WldS1D3VergeC"), 1, 24.8f, 1000, 774, 1.f);    // slot 226, 10.25 km
	Site(TEXT("WldS1D3VergeD"), 1, 24.8f, 1000, 772, 1.f);    // slot 228, 10.34 km
	Site(TEXT("WldS1D3VergeE"), 1, 24.8f, 1000, 762, 1.f);    // slot 238, 10.79 km
	Site(TEXT("WldS1D3VergeF"), 1, 24.8f, 1000, 760, 1.f);    // slot 240, 10.88 km
	Site(TEXT("WldS1D3VergeA"), 1, 24.8f, 1000, 756, 1.f);    // slot 244, 11.06 km
	// the hero
	Site(TEXT("WldS1D3Hopper"), -1, 84.0f, 1000, 806, 1.f);    // slot 194, 8.80 km
	Site(TEXT("WldS1D3Hopper"), -1, 84.0f, 1000, 788, 1.f);    // slot 212, 9.61 km
	Site(TEXT("WldS1D3Hopper"), -1, 84.0f, 1000, 770, 1.f);    // slot 230, 10.43 km
#undef S1D3
// ---- END s1d3 ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People3 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1d4 (district 4 "The Burning Slopes", ft_world_volcano_s1d4.py) ----
// THE CLIFF TEMPLE, 11.1 to 13.9 km (slots 246 to 307, the even ones). 19 meshes: 8 buildings, 10 props, the Bell Tower.
// Every rule is a Site of a set at chance 1. Estimate: 130 sets placed, their parts added up = 257 pieces written;
// x 0.75 (the share the track leaves) = about 193 placed. No loop, corkscrew or jump lies in this district, so up
// to 257 may stand. Most of one mesh: LanternRow 21, IncenseUrn 21, RockShrine 20. The Bell Tower: 3.
// Bands (metres from the centre line). A building set is anchored on the line its pieces FRONTS stand on:
//   25.2  the near row, both sides: low props (under 2 m) and thin tall ones (poles 14 to 16 cm), 24.5 to 25.9 m with sway
//   35.7 left / 35.3 right  open tall props (moon arch, gong, robe line) where no fumarole, deck or stage stands
//   left 62.0 (behind a basalt wall) or 57.2 (no wall)  buildings; middles 59 to 70 m
//   right 62.0 (slots with no wall, fall or terrace)  buildings; 65.6  two rock shrines;  92 left  the Bell Tower
// Slots: every = 12, 20, 24 or 1000 (a single slot), phase even: s % 4 == 0 or 2 only. A set stands at its slot
// middle; each slot was tested against the pieces both tables already stand there (C:/ftwork/jobs/L3s1d4/gen_block.py).
#define S1D4(n) TEXT("/Game/World/Volcano/SM_S1D4_" n)
Set(TEXT("WldS1D4Bell"), {
	{ S1D4("BellTower"), 0.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Gate"), {
	{ S1D4("MoonTerrace"), 0.f, 3.38f, 0.f, 0.f, 1.f },
	{ S1D4("RockShrine"), -12.5f, 2.7f, 0.f, 12.f, 1.f },
	{ S1D4("RockShrine"), 12.5f, 2.7f, 0.f, -12.f, 1.f },
});
Set(TEXT("WldS1D4Yard"), {
	{ S1D4("Kitchen"), -7.5f, 3.56f, 0.f, 0.f, 1.f },
	{ S1D4("StoreTower"), 9.5f, 3.38f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Hall"), {
	{ S1D4("PrayerHall"), -4.f, 4.16f, 0.f, 0.f, 1.f },
	{ S1D4("DrumPavilion"), 13.2f, 4.21f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Lodge"), {
	{ S1D4("Lodge"), -8.6f, 4.25f, 0.f, 0.f, 1.f },
	{ S1D4("GateHouse"), 6.6f, 3.98f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Gate2"), {
	{ S1D4("GateHouse"), -6.2f, 3.98f, 0.f, 0.f, 1.f },
	{ S1D4("StoreTower"), 8.2f, 3.38f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Stair"), {
	{ S1D4("StoreTower"), -11.6f, 3.38f, 0.f, 0.f, 1.f },
	{ S1D4("MoonTerrace"), 3.2f, 3.38f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Court"), {
	{ S1D4("PrayerHall"), 0.f, 4.16f, 0.f, 0.f, 1.f },
	{ S1D4("StoreTower"), -14.6f, 8.68f, 0.f, 0.f, 1.f },
	{ S1D4("StoreTower"), 14.6f, 8.68f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Rows"), {
	{ S1D4("Lodge"), -6.2f, 4.25f, 0.f, 0.f, 1.f },
	{ S1D4("Kitchen"), 8.8f, 3.56f, 0.f, 0.f, 1.f },
	{ S1D4("DrumPavilion"), 0.5f, 12.61f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4RHall"), {
	{ S1D4("GateHouse"), -8.6f, 3.98f, 0.f, 0.f, 1.f },
	{ S1D4("PrayerHall"), 6.2f, 4.16f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4RYard"), {
	{ S1D4("Kitchen"), -8.2f, 3.56f, 0.f, 0.f, 1.f },
	{ S1D4("DrumPavilion"), 4.9f, 4.21f, 0.f, 0.f, 1.f },
	{ S1D4("StoreTower"), 13.f, 4.18f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Shrines"), {
	{ S1D4("RockShrine"), -9.f, 1.78f, 0.f, 10.f, 0.85f },
	{ S1D4("RockShrine"), 8.f, 1.78f, 0.f, -14.f, 0.85f },
});
Set(TEXT("WldS1D4N1"), {
	{ S1D4("LanternRow"), -11.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("PennantLine"), 7.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4N2"), {
	{ S1D4("PrayerDrums"), -12.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("BannerMast"), -1.f, -0.25f, 0.f, 0.f, 1.f },
	{ S1D4("WaterJars"), 10.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4N3"), {
	{ S1D4("StoneTortoise"), -10.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("WaterJars"), 6.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4N4"), {
	{ S1D4("BannerMast"), -7.f, -0.25f, 0.f, 0.f, 1.f },
	{ S1D4("IncenseUrn"), 6.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4N5"), {
	{ S1D4("PennantLine"), -8.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("PrayerDrums"), 7.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4N6"), {
	{ S1D4("LanternRow"), -6.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("IncenseUrn"), 8.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4M1"), {
	{ S1D4("MoonArch"), 0.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4M2"), {
	{ S1D4("GongFrame"), -6.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("RobeLine"), 5.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4M3"), {
	{ S1D4("GongFrame"), 0.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4M4"), {
	{ S1D4("RobeLine"), -5.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("MoonArch"), 5.f, 0.f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1d4, The Burning Slopes: the Cliff Temple"), 11.1f, 13.9f);
	Site(TEXT("WldS1D4Bell"), -1, 92.0f, 20, 8, 1.f);    // slots 252 272 292
	Site(TEXT("WldS1D4Gate"), -1, 62.0f, 20, 14, 1.f);    // slots 246 266 286 306
	Site(TEXT("WldS1D4Yard"), -1, 59.6f, 20, 4, 1.f);   // review round: was 62.0 m; the kitchen and store tower backed into the goats (VolGoats) at 70 to 72 m //    // slots 256 276 296
	Site(TEXT("WldS1D4Hall"), -1, 62.0f, 20, 0, 1.f);    // slots 260 280 300
	Site(TEXT("WldS1D4Lodge"), -1, 62.0f, 20, 10, 1.f);    // slots 250 270 290
	Site(TEXT("WldS1D4Gate2"), -1, 57.2f, 20, 18, 1.f);    // slots 262 282 302
	Site(TEXT("WldS1D4Stair"), -1, 57.2f, 20, 8, 1.f);    // slots 252 272 292
	Site(TEXT("WldS1D4Court"), -1, 57.2f, 20, 16, 1.f);    // slots 264 284 304
	Site(TEXT("WldS1D4Rows"), -1, 57.2f, 20, 6, 1.f);    // slots 254 274 294
	Site(TEXT("WldS1D4RHall"), 1, 62.0f, 20, 18, 1.f);    // slots 262 282 302
	Site(TEXT("WldS1D4RYard"), 1, 62.0f, 20, 8, 1.f);    // slots 252 272 292
	Site(TEXT("WldS1D4Shrines"), 1, 65.6f, 20, 0, 1.f);    // slots 260 280 300
	Site(TEXT("WldS1D4Shrines"), 1, 65.6f, 20, 10, 1.f);    // slots 250 270 290
	Site(TEXT("WldS1D4N1"), -1, 25.2f, 12, 0, 1.f);    // slots 252 264 276 288 300
	Site(TEXT("WldS1D4N1"), 1, 25.2f, 12, 6, 1.f);    // slots 246 258 270 282 294 306
	Site(TEXT("WldS1D4N2"), -1, 25.2f, 12, 10, 1.f);    // slots 254 266 278 290 302
	Site(TEXT("WldS1D4N3"), -1, 25.2f, 12, 8, 1.f);    // slots 256 268 280 292 304
	Site(TEXT("WldS1D4N3"), 1, 25.2f, 12, 2, 1.f);    // slots 250 262 274 286 298
	Site(TEXT("WldS1D4N4"), -1, 25.2f, 12, 6, 1.f);    // slots 246 258 270 282 294 306
	Site(TEXT("WldS1D4N4"), 1, 25.2f, 12, 0, 1.f);    // slots 252 264 276 288 300
	Site(TEXT("WldS1D4N5"), -1, 25.2f, 12, 4, 1.f);    // slots 248 260 272 284 296
	Site(TEXT("WldS1D4N6"), -1, 25.2f, 12, 2, 1.f);    // slots 250 262 274 286 298
	Site(TEXT("WldS1D4N6"), 1, 25.2f, 12, 8, 1.f);    // slots 256 268 280 292 304
	Site(TEXT("WldS1D4M1"), -1, 35.7f, 24, 8, 1.f);    // slots 256 280 304
	Site(TEXT("WldS1D4M1"), -1, 35.7f, 1000, 736, 1.f);    // slots 264
	Site(TEXT("WldS1D4M1"), 1, 35.3f, 24, 4, 1.f);    // slots 260 284
	Site(TEXT("WldS1D4M1"), 1, 35.3f, 24, 12, 1.f);    // slots 252 276 300
	Site(TEXT("WldS1D4M2"), -1, 35.7f, 1000, 750, 1.f);    // slots 250
	Site(TEXT("WldS1D4M2"), -1, 35.7f, 1000, 726, 1.f);    // slots 274
	Site(TEXT("WldS1D4M2"), -1, 35.7f, 1000, 718, 1.f);    // slots 282
	Site(TEXT("WldS1D4M2"), -1, 35.7f, 1000, 694, 1.f);    // slots 306
	Site(TEXT("WldS1D4M2"), 1, 35.3f, 24, 10, 1.f);    // slots 254 278 302
	Site(TEXT("WldS1D4M2"), 1, 35.3f, 24, 18, 1.f);    // slots 246 270 294
	Site(TEXT("WldS1D4M3"), -1, 35.7f, 24, 12, 1.f);    // slots 252 276 300
	Site(TEXT("WldS1D4M3"), -1, 35.7f, 1000, 708, 1.f);    // slots 292
	Site(TEXT("WldS1D4M3"), 1, 35.3f, 24, 0, 1.f);    // slots 264 288
	Site(TEXT("WldS1D4M3"), 1, 35.3f, 24, 16, 1.f);    // slots 248 272 296
	Site(TEXT("WldS1D4M4"), -1, 35.7f, 24, 2, 1.f);    // slots 262 286
	Site(TEXT("WldS1D4M4"), -1, 35.7f, 24, 18, 1.f);    // slots 246 270 294
	Site(TEXT("WldS1D4M4"), 1, 35.3f, 24, 6, 1.f);    // slots 258 282 306
	Site(TEXT("WldS1D4M4"), 1, 35.3f, 24, 22, 1.f);    // slots 266 290
#undef S1D4
// ---- END s1d4 ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People3 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1d5 (district 5 "The Ash Fields" = Windmill Flats, ft_world_volcano_s1d5.py) ----
// 18 meshes, /Game/World/Volcano/SM_S1D5_*: 7 buildings (PostMill, Barn, Granary, Farmhouse, SieveHouse, SailLoft, Dovecote),
// 10 props (low: SackRow, BasketRack, Millstone, HandCart, QuiltFence; tall and open: WindPump, ScareFigure, WindSock,
// MillSign, Bunting), the hero (GrandMill, 45.1 m). The ash farmers' mills: pink, sky blue, white, lemon cream, ash lilac.
// The zone has slots 308 to 369 (62); this block uses the even ones only (every 4, 8, 12 or 24 with an even phase).
// By s % 12: 0: 5 slots, 2: 5, 4: 5, 6: 5, 8: 6, 10: 5.  s % 4 == 0: 16, == 2: 15.  s % 8 == 0: 8, 2: 7, 4: 8, 6: 8.
// WHERE, and why (read from FTDressL3.inl and FTPeopleL3.inl; nothing simulated, nothing seen in the game):
//   LOW ROW 1  17.6 to 18 m, both sides: SackRow (s % 4 == 0) and BasketRack (s % 4 == 2), each 0.7 m deep or less, in
//              front of the guard fence (18.5 m) and the kerb (20.9 m).
//   LOW ROW 2  22.9 to 23.8 m, both sides, behind the kerb and inside the cable reels (24.5 m): Millstone (s % 8 == 2),
//              QuiltFence (6), SackRow (0), BasketRack (4).
//   THIN ROW   26.2 to 27 m: thin tall pieces (a pole and sock, a cloth figure, a blank sign), in front of the people's
//              trestles (28.4 m and more). Left s % 12 == 0, 2, 6, 8 (the reels stand on 4 and 10); right 0 and 6 (pass
//              3's catwalk is written on 2 and 8 there).
//   OPEN ROW   left 31.5 to 34.5 m on s % 12 == 4 and 10 (fumaroles on s % 3 == 0, block b's stages on odd slots, block
//              d's crows on 332, 338, 344, 350, which are s % 12 == 2 and 8); right 33.5 to 35.5 m on 0, 4, 6, 10
//              (fumaroles on s % 3 == 2; block b's stages 38.3 m on every even slot). The wind pumps stand by a Site,
//              turned 70 degrees, so that the tail (3 m long) lies along the road and not across these rows.
//   FARMS      left, anchor 65 m, behind the lava channel and its wall (56 to 57.5 m): three buildings and a washing
//              line or a hand cart (60.4 m: the carts stand 26 m or more out) on s % 12 == 0, 4, 6, 10. Pass 3 there:
//              the watch terrace and tower (62.3 to 64.6 m) on s % 18 == 8 and 14, block d (68.8 m) on odd slots.
//   MILL ROWS  right, anchor 70 m, behind the wall (64.5 to 66 m), the three tallest buildings on s % 12 == 2 and 8:
//              the only even slots there without block d (70.8 m, s % 6 == 4 and s % 18 == 12) or the watch pieces
//              (73.2 m, s % 9 == 0 and 6). The basalt wall stands on s % 3 == 2 there, so in front of both rows.
//   HERO       THE GRAND MILL, right, 125 m out, s % 24 == 6: slots 310, 334, 358 (14.0, 15.1 and 16.2 km); the road up
//              the slope (116 m) is on 327, 347, 367, the ash tips (96 to 112 m) on odd slots, the ash field sets from 154 m.
// No light anywhere. Movement (vertex shader only): sails, a tail fan, wind wheels, a paper windmill (a swing of 180
// degrees about the axle: a wheel goes round and comes back), vanes, pennants, a wind sock, sieves, cloths, sleeves, a
// hoisted sack, four sitting doves that bob 3 cm. No figures, no words, no numerals.
// Estimate (listed = slots x parts x chance, both sides counted; placed = listed x 0.75):
//   farms 4 sets x 5 slots x 4 parts = 80;  mill rows 5 x 3 + 6 x 3 = 33;  hero 3;
//   thin and open rows: 12 rules of one piece, eleven on 5 slots and one on 6 = 61;
//   low rows: 16 x 2 x 0.7 + 15 x 2 x 0.7 + 7 x 2 + 8 x 2 + 8 x 2 x 0.7 + 8 x 2 x 0.7 = 95.8;
//   272.8 listed x 0.75 = about 205 placed.
//   Most of one mesh: SackRow 33.6 listed (about 25 placed); 48 if every chance came up and nothing were left out.
//   Hero: 3 listed (1 to 3). Smallest x min of a TALL mesh: 26.2 m (WindSock, ScareFigure, MillSign).
#define S1D5(n) TEXT("/Game/World/Volcano/SM_S1D5_" n)
Set(TEXT("WldS1D5FarmA"), {                          // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D5("Farmhouse"), -15.5f,  0.f,  0.f,  4.f, 1.f },
	{ S1D5("Barn"),        0.5f,  1.f,  0.f, -3.f, 1.f },
	{ S1D5("Granary"),    14.5f, -0.5f, 0.f,  0.f, 1.f },
	{ S1D5("Bunting"),    -7.5f, -4.6f, 0.f,  6.f, 1.f },
});
Set(TEXT("WldS1D5FarmB"), {
	{ S1D5("PostMill"),  -17.5f,  0.f,  0.f,  0.f, 1.f },
	{ S1D5("SailLoft"),   -4.f,   1.5f, 0.f,  2.f, 1.f },
	{ S1D5("Barn"),       11.5f,  1.f,  0.f, -4.f, 1.f },
	{ S1D5("HandCart"),    4.5f, -4.6f, 0.f, 20.f, 1.f },
});
Set(TEXT("WldS1D5FarmC"), {
	{ S1D5("SieveHouse"), -16.f,  0.f,  0.f,  0.f, 1.f },
	{ S1D5("Farmhouse"),  -5.5f,  0.5f, 0.f, -3.f, 1.f },
	{ S1D5("SailLoft"),   10.f,   1.5f, 0.f,  0.f, 1.f },
	{ S1D5("Bunting"),   -11.5f, -4.6f, 0.f, -5.f, 1.f },
});
Set(TEXT("WldS1D5FarmD"), {
	{ S1D5("Granary"),   -16.5f, -0.5f, 0.f,  0.f, 1.f },
	{ S1D5("SailLoft"),   -4.5f,  1.5f, 0.f,  0.f, 1.f },
	{ S1D5("Barn"),       11.5f,  1.f,  0.f,  3.f, 1.f },
	{ S1D5("HandCart"),    3.5f, -4.6f, 0.f, -15.f, 1.f },
});
Set(TEXT("WldS1D5MillRowA"), {
	{ S1D5("PostMill"),  -13.5f,  0.f,  0.f,  0.f, 1.f },
	{ S1D5("SieveHouse"),  0.f,  -0.5f, 0.f,  0.f, 1.f },
	{ S1D5("Dovecote"),   13.f,   0.f,  0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D5MillRowB"), {
	{ S1D5("Dovecote"),  -13.f,   0.f,  0.f,  0.f, 1.f },
	{ S1D5("PostMill"),    0.5f,  0.f,  0.f,  0.f, 1.f },
	{ S1D5("SieveHouse"), 13.5f, -0.5f, 0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D5PumpL"), { { S1D5("WindPump"), 0.f, 0.f, 0.f, 70.f, 1.f } });
Set(TEXT("WldS1D5PumpR"), { { S1D5("WindPump"), 0.f, 0.f, 0.f, -70.f, 1.f } });
Set(TEXT("WldS1D5GrandMill"), { { S1D5("GrandMill"), 0.f, 0.f, 0.f, -20.f, 1.f } });
Zone(TEXT("World s1d5, Windmill Flats"), 13.9f, 16.7f);
	// the farms (left) and the mill rows (right), behind the lava channels and their walls
	Site(TEXT("WldS1D5FarmA"), -1, 65.f, 12, 0, 1.f);                                 // s % 12 == 0
	Site(TEXT("WldS1D5FarmB"), -1, 65.f, 12, 8, 1.f);                                 // s % 12 == 4
	Site(TEXT("WldS1D5FarmC"), -1, 65.f, 12, 6, 1.f);                                 // s % 12 == 6
	Site(TEXT("WldS1D5FarmD"), -1, 65.f, 12, 2, 1.f);                                 // s % 12 == 10
	Site(TEXT("WldS1D5MillRowA"), 1, 69.5f, 12, 10, 1.f);                              // s % 12 == 2   // review round: was 70.0 m: the post mill on both sides of the 70 m shadow line
	Site(TEXT("WldS1D5MillRowB"), 1, 69.5f, 12, 4, 1.f);                               // s % 12 == 8   // review round: was 70.0 m: the post mill on both sides of the 70 m shadow line
	// the hero: the Grand Mill, right, s % 24 == 6 (slots 310, 334, 358)
	Site(TEXT("WldS1D5GrandMill"), 1, 125.f, 24, 18, 1.f);
	// the thin row, left: s % 12 == 0, 2, 6, 8
	Scatter(S1D5("WindSock"),    26.2f, 26.9f, 12, 0,  1.f, 1.f, 0.f, -1, 4.f);
	Scatter(S1D5("ScareFigure"), 26.2f, 26.9f, 12, 10, 1.f, 1.f, 0.1f, -1, 10.f);
	Scatter(S1D5("ScareFigure"), 26.2f, 26.9f, 12, 6,  1.f, 1.f, 0.1f, -1, 10.f);
	Scatter(S1D5("MillSign"),    26.2f, 26.9f, 12, 4,  1.f, 1.f, 0.f, -1, 6.f);
	// the open row, left: s % 12 == 4 (a washing line) and 10 (a wind pump, side on)
	Scatter(S1D5("Bunting"),     31.5f, 34.5f, 12, 8,  1.f, 1.f, 0.f, -1, 6.f);
	Site(TEXT("WldS1D5PumpL"), -1, 33.f, 12, 2, 1.f);
	// the thin row, right: s % 12 == 0 and 6
	Scatter(S1D5("ScareFigure"), 26.4f, 27.f, 12, 0,  1.f, 1.f, 0.1f, 1, 10.f);
	Scatter(S1D5("WindSock"),    26.4f, 27.f, 12, 6,  1.f, 1.f, 0.f, 1, 4.f);
	// the open row, right: s % 12 == 0 (a wind pump, side on), 4, 6, 10
	Site(TEXT("WldS1D5PumpR"), 1, 34.5f, 12, 0, 1.f);
	Scatter(S1D5("MillSign"),    33.5f, 35.5f, 12, 8,  1.f, 1.f, 0.f, 1, 6.f);
	Scatter(S1D5("Bunting"),     33.5f, 35.5f, 12, 6,  1.f, 1.f, 0.f, 1, 6.f);
	Scatter(S1D5("HandCart"),    33.5f, 35.5f, 12, 2,  1.f, 1.f, 0.f, 1, 25.f);
	// low row 1, both sides
	Scatter(S1D5("SackRow"),     17.6f, 18.f, 4, 0, 0.7f, 1.f, 0.f, 0, 2.f);          // s % 4 == 0
	Scatter(S1D5("BasketRack"),  17.6f, 18.f, 4, 2, 0.7f, 1.f, 0.f, 0, 2.f);          // s % 4 == 2
	// low row 2, both sides
	Scatter(S1D5("Millstone"),   22.9f, 23.8f, 8, 6, 1.f,  1.f, 0.f, 0, 8.f);         // s % 8 == 2
	Scatter(S1D5("QuiltFence"),  22.9f, 23.8f, 8, 2, 1.f,  1.f, 0.f, 0, 3.f);         // s % 8 == 6
	Scatter(S1D5("SackRow"),     22.9f, 23.8f, 8, 0, 0.7f, 1.f, 0.f, 0, 3.f);         // s % 8 == 0
	Scatter(S1D5("BasketRack"),  22.9f, 23.8f, 8, 4, 0.7f, 1.f, 0.f, 0, 3.f);         // s % 8 == 4
#undef S1D5
// ---- END s1d5 ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People3 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1d6 (district 6 "The Far Works" = GEOTHERM, the steam power station, 16.7 to 19.5 km, ft_world_volcano_s1d6.py) ----
// 20 meshes: 9 buildings (5 left, 4 right), 10 props (5 low, 5 tall and open), 1 hero (THE THREE SISTERS, three hourglass
// cooling towers 56, 48 and 40 m). Even slots 370 to 430 only (s % 4 == 0 or 2).
// Estimate (slots x parts x 0.75): hero 3 x 0.75 = 2.25; left yards 4 x 3 x 0.75 + 4 x 3 x 0.75 = 18; left singles
// (6 + 8) x 0.75 = 10.5; right rail works 5 x 3 x 0.75 + 2 x 0.75 = 12.75; small towers 8 x 0.75 = 6; tank yards
// 8 x 2 x 0.75 = 12; verge left (15 + 15 + 8 + 8 + 16 + 15) x 0.75 = 57.75; verge right (7 + 8 + 8 + 8 + 16 + 5 + 5 + 5 +
// 6 + 11 + 10) x 0.75 = 66.75. Sum: about 186 placed. Most of one mesh: Bollards and StiltDrum, about 23 each.
// Smallest x min of a TALL piece: 25.2 m (the four thin open verge pieces). Low pieces: 17.5 m (Bollards), 20.2 m (the rest).
//
// What stands where, and what it dodges (both tables were read for 16.7 to 19.5 km):
// HERO, left, anchor 81 m (the piece reaches 68.4 to 94 m), slots 376, 390, 410 (17.03, 17.67, 18.57 km): behind the
//   channel and its wall, in front of the mine sets (haulers from 91 m, crushers from 103 m), no pass 3 knoll, tub line,
//   whim, ropeway or handcar in the slot or its neighbours, no flock of the first population pass in a neighbouring slot,
//   clear of the lava lakes (17.31, 18.31, 19.31 km).
// LEFT buildings, all inside 70 m (they cast): two yards 63 m out on s % 16 == 4 and 12 (no basalt wall, no watch terrace
//   there); single tall pieces 68 to 69 m out behind the basalt walls (backs at 61.5 m) on s % 12 == 10 and, in three
//   zones that leave the hero's slots 390 and 410 out, on s % 12 == 2 and 6. Nothing on s % 8 == 0 (watch terraces, towers).
// RIGHT buildings: the mine's railway (ballast to 68.1 m) and the slag trains' sheds (to 69 m) fill the brief's building
//   band behind the channel, so: the pipe-bridge office STRADDLES the railway (anchor 66 m, legs at 62.0 and 70.2 m,
//   6.9 m clear; a Site, so it is not slid along; s % 8 == 0, where pass 3 has nothing at 62 m or 73 m), and the
//   separator, the tank farm and the small tower stand 77 to 78 m out (fronts 70.8 to 74 m): PAST the brief's 70 m line
//   for buildings. These three meshes are used only there (all outside 70 m: no shadow). s % 24 == 8 (392, 416) gets the
//   office alone: a flock of the first population pass flies 12 m up and more over those two slots.
// VERGE, tall and open, 25.2 to 25.5 m (behind the kerb, in front of the people rows from 26.8 m): left every even slot;
//   right s % 12 == 0, 4, 6, 10 (the catwalks stand at 25.2 m and the fumaroles at 31.5 m and more on s % 12 == 2, 8).
// VERGE, second row (Sites: never slid along, the pieces are 9 m long or less, so the channel works' pipe bridges 14 m
//   either side of an even slot's middle stay clear): left 36.3 m (35.2 to 37.4; the channel starts at 38), right 32.5 m.
//   Everything in it is open and carries its mass 4.7 m up and more: the lava shows under it.
// VERGE, low: right 20.2 to 22 m (the kerb is on the odd slots there); bollards 17.5 to 17.9 m both sides.
#define S1D6(n) TEXT("/Game/World/Volcano/SM_S1D6_" n)
Set(TEXT("WldS1D6Sisters"), { { S1D6("ThreeSisters"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D6HallYard"), {                    // the turbine hall between the valve house and a ball tank
	{ S1D6("TurbineHall"),    0.f,  0.f, 0.f,  0.f, 1.f },
	{ S1D6("ValveHouse"),   -23.5f, 0.f, 0.f,  0.f, 1.f },
	{ S1D6("BallTank"),      21.5f, 0.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6SwitchYard"), {                  // the switch hall, the control block on its stalk, a ball tank
	{ S1D6("SwitchHall"),    -9.f,  0.f, 0.f,  0.f, 1.f },
	{ S1D6("ControlStalk"),   7.5f, 1.f, 0.f,  0.f, 1.f },
	{ S1D6("BallTank"),      21.5f, 0.f, 0.f, 40.f, 1.f },
});
Set(TEXT("WldS1D6RailWorks"), {                   // the office astride the railway, the separator and the tanks behind it
	{ S1D6("PipeOffice"),     0.f,  0.f, 0.f,  0.f, 1.f },
	{ S1D6("Separator"),     13.f, 11.f, 0.f,  0.f, 1.f },
	{ S1D6("TankFarm"),     -15.f, 11.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6RailBridge"), { { S1D6("PipeOffice"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D6TankYard"), {
	{ S1D6("TankFarm"),      -9.f,  0.f, 0.f,  0.f, 1.f },
	{ S1D6("Separator"),      9.f,  0.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Trestle"), { { S1D6("PipeTrestle"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D6Drum"), { { S1D6("StiltDrum"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Zone(TEXT("World s1d6, the Far Works: Geotherm"), 16.7f, 19.5f);
	Site(TEXT("WldS1D6Sisters"), -1, 81.f, 1000, 624, 1.f);                          // slot 376
	Site(TEXT("WldS1D6Sisters"), -1, 81.f, 1000, 610, 1.f);                          // slot 390
	Site(TEXT("WldS1D6Sisters"), -1, 81.f, 1000, 590, 1.f);                          // slot 410
	Site(TEXT("WldS1D6HallYard"), -1, 63.f, 16, 12, 1.f);                            // left, s % 16 == 4
	Site(TEXT("WldS1D6SwitchYard"), -1, 63.f, 16, 4, 1.f);                           // left, s % 16 == 12
	Scatter(S1D6("BallTank"), 68.f, 69.f, 12, 2, 1.f, 1.f, 0.05f, -1, 180.f);        // left, behind the walls, s % 12 == 10
	Site(TEXT("WldS1D6RailWorks"), 1, 66.f, 24, 0, 1.f);                             // right, s % 24 == 0
	Site(TEXT("WldS1D6RailWorks"), 1, 66.f, 24, 8, 1.f);                             // right, s % 24 == 16
	Site(TEXT("WldS1D6RailBridge"), 1, 66.f, 24, 16, 1.f);                           // right, s % 24 == 8
	Scatter(S1D6("SmallTower"), 75.8f, 76.8f, 8, 6, 1.f, 1.f, 0.06f, 1, 25.f);         // right, s % 8 == 2
	Site(TEXT("WldS1D6TankYard"), 1, 77.f, 8, 2, 1.f);                               // right, s % 8 == 6
	Scatter(S1D6("Bollards"), 17.5f, 17.9f, 4, 0, 1.f, 1.f, 0.f, -1, 0.f);           // low, left, s % 4 == 0
	Scatter(S1D6("WhistleMast"), 25.2f, 25.5f, 4, 0, 1.f, 1.f, 0.f, -1, 0.f);        // left, s % 4 == 0
	Scatter(S1D6("PipeLoop"), 25.2f, 25.5f, 8, 6, 1.f, 1.f, 0.f, -1, 0.f);           // left, s % 8 == 2
	Scatter(S1D6("ChevronGantry"), 25.2f, 25.5f, 8, 2, 1.f, 1.f, 0.f, -1, 0.f);      // left, s % 8 == 6
	Site(TEXT("WldS1D6Trestle"), -1, 36.3f, 4, 2, 1.f);                              // left, s % 4 == 2
	Site(TEXT("WldS1D6Drum"), -1, 36.3f, 4, 0, 1.f);                                 // left, s % 4 == 0
	Scatter(S1D6("ValveWheel"), 20.2f, 22.f, 8, 0, 1.f, 1.f, 0.f, 1, 10.f);          // low, right, s % 8 == 0
	Scatter(S1D6("GaugeBoard"), 20.2f, 22.f, 8, 4, 1.f, 1.f, 0.f, 1, 8.f);           // low, right, s % 8 == 4
	Scatter(S1D6("PipeRings"), 20.2f, 22.f, 8, 6, 1.f, 1.f, 0.f, 1, 15.f);           // low, right, s % 8 == 2
	Scatter(S1D6("LowManifold"), 20.2f, 22.f, 8, 2, 1.f, 1.f, 0.f, 1, 0.f);          // low, right, s % 8 == 6
	Scatter(S1D6("Bollards"), 17.5f, 17.9f, 4, 2, 1.f, 1.f, 0.f, 1, 0.f);            // low, right, s % 4 == 2
	Scatter(S1D6("WhistleMast"), 25.2f, 25.5f, 12, 0, 1.f, 1.f, 0.f, 1, 0.f);        // right, s % 12 == 0
	Scatter(S1D6("ChevronGantry"), 25.2f, 25.5f, 12, 8, 1.f, 1.f, 0.f, 1, 0.f);      // right, s % 12 == 4
	Scatter(S1D6("PipeLoop"), 25.2f, 25.5f, 12, 6, 1.f, 1.f, 0.f, 1, 0.f);           // right, s % 12 == 6
	Scatter(S1D6("StiltDrum"), 25.3f, 25.5f, 12, 2, 1.f, 1.f, 0.f, 1, 0.f);          // right, s % 12 == 10
	Site(TEXT("WldS1D6Trestle"), 1, 32.5f, 12, 0, 1.f);                              // right, s % 12 == 0
	Site(TEXT("WldS1D6Trestle"), 1, 32.5f, 12, 2, 1.f);                              // right, s % 12 == 10
	Site(TEXT("WldS1D6Drum"), 1, 32.5f, 12, 8, 1.f);                                 // right, s % 12 == 4
	Site(TEXT("WldS1D6Drum"), 1, 32.5f, 12, 6, 1.f);                                 // right, s % 12 == 6
// The control block on its stalk behind the basalt walls, left, s % 12 == 2 and 6, in three zones: slots 370 to 389,
// 392 to 409, 412 to 431 (the hero stands in 390 and 410).
Zone(TEXT("World s1d6, the Far Works: behind the walls (1)"), 16.7f, 17.64f);
	Scatter(S1D6("ControlStalk"), 68.2f, 69.f, 12, 10, 1.f, 1.f, 0.05f, -1, 20.f);
	Scatter(S1D6("ControlStalk"), 68.2f, 69.f, 12, 6, 1.f, 1.f, 0.05f, -1, 20.f);
Zone(TEXT("World s1d6, the Far Works: behind the walls (2)"), 17.69f, 18.54f);
	Scatter(S1D6("ControlStalk"), 68.2f, 69.f, 12, 10, 1.f, 1.f, 0.05f, -1, 20.f);
	Scatter(S1D6("ControlStalk"), 68.2f, 69.f, 12, 6, 1.f, 1.f, 0.05f, -1, 20.f);
Zone(TEXT("World s1d6, the Far Works: behind the walls (3)"), 18.6f, 19.5f);
	Scatter(S1D6("ControlStalk"), 68.2f, 69.f, 12, 10, 1.f, 1.f, 0.05f, -1, 20.f);
	Scatter(S1D6("ControlStalk"), 68.2f, 69.f, 12, 6, 1.f, 1.f, 0.05f, -1, 20.f);
#undef S1D6
// ---- END s1d6 ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People3 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1d7 (district 7 "The Inner Wall" = THE WALL KEEP, 19.5 to 22.3 km, ft_world_volcano_s1d7.py) ----
// 16 meshes: 7 buildings, 4 tall open props, 4 low props, 1 hero (THE CROWN KEEP). Every name below is one the make log printed
// (C:/ftwork/logs/L3s1d7_make.log: pieces built 16 failed 0).
// The district is slots 432 to 492 (61 slots); this block uses its EVEN slots only (s % 4 == 0 or 2), every Site in a ONE-SLOT Zone,
// every 4, the phase that picks the slot, chance 1, because what stands there already differs slot by slot. Not used: 450 and 472 (the
// jump gaps over the lava lakes at 20.32 and 21.32 km: the track leaves a Site out there). Read from FTDressL3.inl and FTPeopleL3.inl,
// on the even slots of 19.5 to 22.3 km:
//   LEFT: the kerb 20.9 m (to 23.6) on every even slot; trestle people 28.4 to 28.8 m (decks to 31.4); fumaroles 31 to 34 m when s % 6 == 2;
//     lookouts 33.1 m (442 460 472 484); the lava channel 38 to 52 m; basalt wall 56 to 57.5 m when s % 10 == 0 (its back to 61.8); a lava
//     fall cliff 56.5 to 57.5 m when s % 10 == 8 (34 m wide, 19 m tall); watch terrace / tower 62.3 to 64.6 m (448 458 478 488); a derrick
//     68.8 to 69.2 m when s % 12 == 6; knolls and trails 87 to 95 m.
//   RIGHT: no kerb on even slots; catwalk 25.2 m when s % 12 == 2 or 8; trestle people 28.4 to 28.8 m; fumaroles 31.5 to 38 m when
//     s % 6 == 4; stages 38.3 to 38.7 m on most even slots; watch decks 41.8 m; the channel 46 to 61 m; far-bank people 62.4 to 63 m when
//     s % 10 == 0; basalt wall 64.5 to 66 m when s % 10 == 4; fall cliff 65.5 to 66.5 m when s % 10 == 6; arches, crags, derricks
//     70.8 to 71.2 m (448 452 464 466 468 476 482 488); watch pieces 73.2 to 73.8 m (450 460 480 490); goats 72 to 86 m (434 456 478).
//   Cliffs 116 to 126 m on every slot of both sides; fissure fire 134 to 144 m (right when s % 5 == 2).
// So: BUILDINGS behind the channels and their walls. Left, anchor 64 m (62.5 m where a derrick stands, 66 m behind a basalt wall), never
//   on a fall-cliff slot (s % 10 == 8). Right, anchor 66.5 m (69 m at 470, behind far-bank people), only on the slots with nothing at 61 to
//   72 m: 432 438 442 458 462 470 478 492. TALL OPEN PROPS: left 36 m on the slots without a fumarole or a lookout, right 33.6 m (the lane
//   between the trestle decks and the stages) on the slots without a fumarole or a catwalk. LOW PROPS: right 23.4 m on every used slot,
//   left 24.8 m (behind the kerb) when s % 8 == 0. THE CROWN KEEP: right, 100 m, slots 444 (20.09 km) and 474 (21.45 km): in front of the
//   cliffs (116 m and more), on slots with no trail piece at 87 to 95 m and no fissure fire.
// Not dodged, and saying so: ash banks 20.5 to 22.5 m (chance 0.6, s % 6 == 2: the right-hand low props stand 22.1 to 24.5 m); on the
//   left the low props stand behind the 1.2 to 2 m kerb; the keep's back (to 117 m with its turrets) reaches the cliffs' band (116 m on);
//   where a basalt wall slides along its slot its back rank (to 61.8 m) may touch a gate's ramp at 61.3 m; the right-hand buildings on
//   438 and 458 stand on slots whose left side has a fall cliff (nothing of this block stands on that left side).
// Slots 448, 470 and 492 lie just before the jumps' run-ups (20.27, 21.27, 22.27 km): if the track flags them as run-up it leaves out
//   what stands nearer than 40 m there (the low and tall props: up to 14 pieces). Not checked.
// Count (exact: every Site is written out, chance 1; nothing taken off for the track's share):
//   WardA   8 sites x 3 parts =  24   432L 444L 456L 466L 484L 492L 442R 462R
//   WardB   7 sites x 4 parts =  28   434L 446L 460L 476L 480L 438R 458R
//   WardC  11 sites x 3 parts =  33   436L 440L 454L 462L 470L 474L 486L 490L 432R 470R 492R
//   WardD   4 sites x 4 parts =  16   442L 452L 464L 482L
//   WardW   1 sites x 2 parts =   2   478R
//   TallA   9 sites x 2 parts =  18   432L 444L 456L 468L 480L 492L 444R 468R 486R
//   TallB   9 sites x 2 parts =  18   436L 448L 462L 474L 486L 432R 456R 474R 492R
//   TallC   8 sites x 2 parts =  16   438L 454L 466L 478L 490L 438R 462R 480R
//   LowA   18 sites x 2 parts =  36   432R 436R 440R 444R 448R 452R 456R 460R 464R 468R 476R 480R 484R 488R 492R 440L 456L 480L
//   LowB   18 sites x 2 parts =  36   434R 438R 442R 446R 454R 458R 462R 466R 470R 474R 478R 482R 486R 490R 432L 448L 464L 488L
//   Hero    2 sites x 1 parts =   2   444R 474R
//   SUM 229 placed in 95 sites.
//   Per mesh: CurtainWall 35, GateTower 19, CornerTower 19, BannerPole 18, HorseTrough 18, StoneBalls 18, MarketStall 18, Barrels 18, ShieldBoard 17, BellCote 11, PennantLine 9, GreatHall 8, ScaffoldArch 8, Granary 7, WellBastion 4, CrownKeep 2.
// Smallest x of a TALL mesh: 33.6 m (BannerPole). Smallest x of a low mesh: 23.4 m (Barrels).
// Glow: steady lit windows only, on GateTower, CornerTower, GreatHall (62.5 m out and more) and CrownKeep (100 m). Nothing blinks.
// No mesh stands both inside and outside 70 m (the hero is the only one outside; it is saved on the always-moving master).
#define S1D7(n) TEXT("/Game/World/Volcano/SM_S1D7_" n)
// the gate ward: the gate tower, the great hall beside it, a corner tower
Set(TEXT("WldS1D7WardA"), {                       // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("GateTower"), -14.5f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 22.1 m
	{ S1D7("GreatHall"), 1.0f, 2.0f, 0.f, 0.f, 1.f },   // TALL, top 18.9 m
	{ S1D7("CornerTower"), 16.0f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 29.3 m
});
// the store ward: a corner tower, a length of curtain wall, the buttressed granary, the bell tower
Set(TEXT("WldS1D7WardB"), {                       // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("CornerTower"), -17.5f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 29.3 m
	{ S1D7("CurtainWall"), -5.0f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 16.2 m
	{ S1D7("Granary"), 9.5f, 2.0f, 0.f, 0.f, 1.f },   // TALL, top 16.2 m
	{ S1D7("BellCote"), 18.5f, 1.0f, 0.f, 0.f, 1.f },   // TALL, top 26.9 m
});
// the wall itself: the gate tower between two lengths of curtain wall (44 m of wall to the slot)
Set(TEXT("WldS1D7WardC"), {                       // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("CurtainWall"), -13.7f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 16.2 m
	{ S1D7("GateTower"), 0.0f, 0.5f, 0.f, 0.f, 1.f },   // TALL, top 22.1 m
	{ S1D7("CurtainWall"), 13.9f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 16.2 m
});
// the well ward: the covered well on its bastion, a length of wall with the bell tower behind it, a corner tower
Set(TEXT("WldS1D7WardD"), {                       // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("WellBastion"), -14.0f, 1.0f, 0.f, 0.f, 1.f },   // TALL, top 15.3 m
	{ S1D7("CurtainWall"), 2.5f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 16.2 m
	{ S1D7("BellCote"), 2.0f, 5.8f, 0.f, 0.f, 1.f },   // TALL, top 26.9 m
	{ S1D7("CornerTower"), 15.0f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 29.3 m
});
// two lengths of curtain wall, nothing behind (a goat run of the first pass stands 68.5 m out and more in this slot)
Set(TEXT("WldS1D7WardW"), {                       // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("CurtainWall"), -8.3f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 16.2 m
	{ S1D7("CurtainWall"), 8.3f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 16.2 m
});
// a banner and the shield board
Set(TEXT("WldS1D7TallA"), {                       // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("BannerPole"), -11.0f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 9.5 m
	{ S1D7("ShieldBoard"), 9.0f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 5.9 m
});
// the pennant lines and a banner
Set(TEXT("WldS1D7TallB"), {                       // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("PennantLine"), -3.0f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 7.4 m
	{ S1D7("BannerPole"), 11.0f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 9.5 m
});
// the masons' scaffold and a shield board
Set(TEXT("WldS1D7TallC"), {                       // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("ScaffoldArch"), -8.0f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 8.4 m
	{ S1D7("ShieldBoard"), 8.0f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 5.9 m
});
// the horse trough and the row of stone balls
Set(TEXT("WldS1D7LowA"), {                       // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("HorseTrough"), -8.0f, 0.0f, 0.f, 0.f, 1.f },   // low, top 1.8 m
	{ S1D7("StoneBalls"), 7.5f, 0.0f, 0.f, 0.f, 1.f },   // low, top 1.8 m
});
// the market stall and the barrels
Set(TEXT("WldS1D7LowB"), {                       // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("MarketStall"), -8.0f, 0.0f, 0.f, 0.f, 1.f },   // low, top 1.9 m
	{ S1D7("Barrels"), 7.0f, 0.0f, 0.f, 0.f, 1.f },   // low, top 1.8 m
});
// THE CROWN KEEP
Set(TEXT("WldS1D7Hero"), {                       // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("CrownKeep"), 0.0f, 0.0f, 0.f, 0.f, 1.f },   // TALL, top 57.5 m
});
Zone(TEXT("World s1d7, The Inner Wall, slot 432"), 19.5224f, 19.5676f);
	Site(TEXT("WldS1D7WardA"), -1, 64.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7WardC"), 1, 66.5f, 4, 0, 1.f);
	Site(TEXT("WldS1D7TallA"), -1, 36.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7TallB"), 1, 33.6f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowB"), -1, 24.8f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), 1, 23.4f, 4, 0, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 434"), 19.6128f, 19.6581f);
	Site(TEXT("WldS1D7WardB"), -1, 64.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7LowB"), 1, 23.4f, 4, 2, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 436"), 19.7033f, 19.7486f);
	Site(TEXT("WldS1D7WardC"), -1, 64.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7TallB"), -1, 36.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), 1, 23.4f, 4, 0, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 438"), 19.7938f, 19.8391f);
	Site(TEXT("WldS1D7WardB"), 1, 66.5f, 4, 2, 1.f);
	Site(TEXT("WldS1D7TallC"), -1, 36.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7TallC"), 1, 33.6f, 4, 2, 1.f);
	Site(TEXT("WldS1D7LowB"), 1, 23.4f, 4, 2, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 440"), 19.8843f, 19.9295f);
	Site(TEXT("WldS1D7WardC"), -1, 66.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), -1, 24.8f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), 1, 23.4f, 4, 0, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 442"), 19.9748f, 20.0200f);
	Site(TEXT("WldS1D7WardD"), -1, 64.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7WardA"), 1, 66.5f, 4, 2, 1.f);
	Site(TEXT("WldS1D7LowB"), 1, 23.4f, 4, 2, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 444"), 20.0653f, 20.1105f);
	Site(TEXT("WldS1D7WardA"), -1, 64.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7Hero"), 1, 100.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7TallA"), -1, 36.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7TallA"), 1, 33.6f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), 1, 23.4f, 4, 0, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 446"), 20.1558f, 20.2010f);
	Site(TEXT("WldS1D7WardB"), -1, 64.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7LowB"), 1, 23.4f, 4, 2, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 448"), 20.2462f, 20.2915f);
	Site(TEXT("WldS1D7TallB"), -1, 36.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowB"), -1, 24.8f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), 1, 23.4f, 4, 0, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 452"), 20.4272f, 20.4725f);
	Site(TEXT("WldS1D7WardD"), -1, 64.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), 1, 23.4f, 4, 0, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 454"), 20.5177f, 20.5629f);
	Site(TEXT("WldS1D7WardC"), -1, 64.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7TallC"), -1, 36.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7LowB"), 1, 23.4f, 4, 2, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 456"), 20.6082f, 20.6534f);
	Site(TEXT("WldS1D7WardA"), -1, 64.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7TallA"), -1, 36.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7TallB"), 1, 33.6f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), -1, 24.8f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), 1, 23.4f, 4, 0, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 458"), 20.6987f, 20.7439f);
	Site(TEXT("WldS1D7WardB"), 1, 66.5f, 4, 2, 1.f);
	Site(TEXT("WldS1D7LowB"), 1, 23.4f, 4, 2, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 460"), 20.7892f, 20.8344f);
	Site(TEXT("WldS1D7WardB"), -1, 66.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), 1, 23.4f, 4, 0, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 462"), 20.8796f, 20.9249f);
	Site(TEXT("WldS1D7WardC"), -1, 62.5f, 4, 2, 1.f);
	Site(TEXT("WldS1D7WardA"), 1, 66.5f, 4, 2, 1.f);
	Site(TEXT("WldS1D7TallB"), -1, 36.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7TallC"), 1, 33.6f, 4, 2, 1.f);
	Site(TEXT("WldS1D7LowB"), 1, 23.4f, 4, 2, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 464"), 20.9701f, 21.0154f);
	Site(TEXT("WldS1D7WardD"), -1, 64.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowB"), -1, 24.8f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), 1, 23.4f, 4, 0, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 466"), 21.0606f, 21.1059f);
	Site(TEXT("WldS1D7WardA"), -1, 64.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7TallC"), -1, 36.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7LowB"), 1, 23.4f, 4, 2, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 468"), 21.1511f, 21.1963f);
	Site(TEXT("WldS1D7TallA"), -1, 36.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7TallA"), 1, 33.6f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), 1, 23.4f, 4, 0, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 470"), 21.2416f, 21.2868f);
	Site(TEXT("WldS1D7WardC"), -1, 66.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7WardC"), 1, 69.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7LowB"), 1, 23.4f, 4, 2, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 474"), 21.4226f, 21.4678f);
	Site(TEXT("WldS1D7WardC"), -1, 62.5f, 4, 2, 1.f);
	Site(TEXT("WldS1D7Hero"), 1, 100.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7TallB"), -1, 36.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7TallB"), 1, 33.6f, 4, 2, 1.f);
	Site(TEXT("WldS1D7LowB"), 1, 23.4f, 4, 2, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 476"), 21.5130f, 21.5583f);
	Site(TEXT("WldS1D7WardB"), -1, 64.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), 1, 23.4f, 4, 0, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 478"), 21.6035f, 21.6488f);
	Site(TEXT("WldS1D7WardW"), 1, 66.5f, 4, 2, 1.f);
	Site(TEXT("WldS1D7TallC"), -1, 36.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7LowB"), 1, 23.4f, 4, 2, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 480"), 21.6940f, 21.7393f);
	Site(TEXT("WldS1D7WardB"), -1, 66.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7TallA"), -1, 36.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7TallC"), 1, 33.6f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), -1, 24.8f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), 1, 23.4f, 4, 0, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 482"), 21.7845f, 21.8297f);
	Site(TEXT("WldS1D7WardD"), -1, 64.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7LowB"), 1, 23.4f, 4, 2, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 484"), 21.8750f, 21.9202f);
	Site(TEXT("WldS1D7WardA"), -1, 64.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), 1, 23.4f, 4, 0, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 486"), 21.9655f, 22.0107f);
	Site(TEXT("WldS1D7WardC"), -1, 62.5f, 4, 2, 1.f);
	Site(TEXT("WldS1D7TallB"), -1, 36.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7TallA"), 1, 33.6f, 4, 2, 1.f);
	Site(TEXT("WldS1D7LowB"), 1, 23.4f, 4, 2, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 488"), 22.0560f, 22.1012f);
	Site(TEXT("WldS1D7LowB"), -1, 24.8f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), 1, 23.4f, 4, 0, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 490"), 22.1464f, 22.1917f);
	Site(TEXT("WldS1D7WardC"), -1, 66.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7TallC"), -1, 36.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D7LowB"), 1, 23.4f, 4, 2, 1.f);
Zone(TEXT("World s1d7, The Inner Wall, slot 492"), 22.2369f, 22.2822f);
	Site(TEXT("WldS1D7WardA"), -1, 64.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7WardC"), 1, 66.5f, 4, 0, 1.f);
	Site(TEXT("WldS1D7TallA"), -1, 36.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D7TallB"), 1, 33.6f, 4, 0, 1.f);
	Site(TEXT("WldS1D7LowA"), 1, 23.4f, 4, 0, 1.f);
#undef S1D7
// ---- END s1d7 ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People3 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1d8 (district 8 "The Last Ash", THE BACKLOT, ft_world_volcano_s1d8.py) ----
// 19 meshes: 8 buildings (StreetFront, StudioBarn, CostumeStore, PaintShop 7 to 11 m; PalaceFront, LighthouseFront, TownHallFront, WaterTower 14.6 to 19 m),
// 10 props (PaintTins, ChairRow, CableCart, PropCutouts, CostumeRail low; LampStand, Reflector, BackdropRoll, StarArch, PaintCradle TALL and open), the hero (BigSlate, twice).
// A small steady glow (0.04 to 0.05) on the pieces that stand 33.4 m out and more and on LampStand (26.2 m and more); none on the low props or Reflector; nothing blinks.
// NOT COMPILED, NOT SEEN IN THE GAME when written. Written by C:/ftwork/jobs/L3s1d8/block.py.
// One Zone a slot (the 30 even slots 494 to 552: 22.37 to 25.00 km), Site rules only (every 1, chance 1): a Site stands on the middle of its slot
// (the brief: "a Set at the slot's middle"), so every piece keeps the place it was given.
// Estimate: 255 rules, 255 pieces before the track leaves any out; x 0.75 (the share the brief gives) = about 191 placed.
//   by mesh, before the 0.75: PaintTins 24, ChairRow 24, CableCart 24, PropCutouts 24, CostumeRail 24, LampStand 24, BackdropRoll 20, StarArch 20, PaintCradle 20, Reflector 9, StudioBarn 8, CostumeStore 7, StreetFront 7, PaintShop 7, PalaceFront 3, TownHallFront 3, LighthouseFront 3, BigSlate 2, WaterTower 2
// Where, against FTDressL3.inl and FTPeopleL3.inl (read from the tables, none of it seen in the game):
//   LOW  left 23.4 m, right 23.2 m, 9 m along either way: behind the fence (18.5 m) and the kerb (20.9 m; left on even slots), in front of the cable reels and catwalk (24.5 m on).
//   NEAR 26.2 m, on the slot: a reflector or an unlit lamp, only on the slots where the tables stand nothing at 24 to 31 m on that side (cable reels s % 6 == 4; the cast every 7;
//        the shoots every 21; the catwalk right s % 12 == 2, 8; the film shoot's rows). Fumaroles (31 m on, left s % 3 == 0, right s % 3 == 2) are not counted against this band.
//   MID  left 35.0 m on s % 6 == 2, 4; right 33.4 m on s % 6 == 0, 4 (the fumaroles have the other even slots), 11 m along either way: open pieces on thin legs, between the
//        cast's trestles (to 31 m) and the lava channels (left 38 m, right 46 m; right 38.5 m: the scenes of pass 3, fronts 36.5 m).
//   DEEP behind the lava channels (the channel rule: nothing solid between the road and a channel). Left 60 m (58.2 to 63.4 m): low fronts on s % 4 == 0 (the crags, 65.5 m on,
//        stand 19.5 m tall behind them), tall fronts on s % 16 == 2, 10; none on s % 16 == 6, 14 (the watch terrace and tower, 62.3 to 64.6 m) and none on the hero's two slots.
//        Right 64.2 m (62.4 to 67.6 m): any front on s % 12 == 0, 8 (the derricks, 68 m on, 22.5 m tall, behind), low fronts on s % 12 == 2, 6 (terraces 69.3 m on behind);
//        none on s % 12 == 4, 10 (people of pass 3, 62.4 to 63 m).
//   HERO THE BIG SLATE, left, 130 m out (126 to 140 m), slots 506 and 544 (22.91 and 24.63 km): no ash tip (odd slots), no uphill set (116 m: 512, 532, 552), no crag (ends 24.5 km)
//        or terrace on the left of either; the ash-field sets stand 166 m out.
//   NOT dodged: SM_AshBank (20.5 to 22.5 m, every slot, chance 0.5) and SM_AshRipples (26 to 30 m, flat) can lie under a low or near piece; SM_VolFlock_A (Far, 55 to 130 m).
// Smallest x of a TALL piece: 26.2 m (Reflector, LampStand). The five low pieces: 23.2 m.
#define S1D8(n) TEXT("/Game/World/Volcano/SM_S1D8_" n)
Set(TEXT("WldS1D8PaintTinsM9"), { { S1D8("PaintTins"), -9.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8ChairRowP9"), { { S1D8("ChairRow"), 9.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8ReflectorZ0"), { { S1D8("Reflector"), 0.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8BackdropRollM11"), { { S1D8("BackdropRoll"), -11.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8StarArchP11"), { { S1D8("StarArch"), 11.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8CableCartM9"), { { S1D8("CableCart"), -9.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8PropCutoutsP9"), { { S1D8("PropCutouts"), 9.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8CostumeStoreZ0"), { { S1D8("CostumeStore"), 0.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8CostumeRailM9"), { { S1D8("CostumeRail"), -9.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8PaintTinsP9"), { { S1D8("PaintTins"), 9.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8PaintCradleM11"), { { S1D8("PaintCradle"), -11.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8LampStandP11Y20"), { { S1D8("LampStand"), 11.0f, 0.0f, 0.f, 20.0f, 1.f } });
Set(TEXT("WldS1D8StreetFrontZ0"), { { S1D8("StreetFront"), 0.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8ChairRowM9"), { { S1D8("ChairRow"), -9.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8CableCartP9"), { { S1D8("CableCart"), 9.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8PropCutoutsM9"), { { S1D8("PropCutouts"), -9.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8CostumeRailP9"), { { S1D8("CostumeRail"), 9.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8PalaceFrontZ0"), { { S1D8("PalaceFront"), 0.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8PaintShopZ0"), { { S1D8("PaintShop"), 0.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8StudioBarnZ0"), { { S1D8("StudioBarn"), 0.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8TownHallFrontZ0"), { { S1D8("TownHallFront"), 0.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8LampStandZ0Y25"), { { S1D8("LampStand"), 0.0f, 0.0f, 0.f, 25.0f, 1.f } });
Set(TEXT("WldS1D8BigSlateZ0"), { { S1D8("BigSlate"), 0.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8WaterTowerZ0"), { { S1D8("WaterTower"), 0.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D8LighthouseFrontZ0"), { { S1D8("LighthouseFront"), 0.0f, 0.0f, 0.f, 0.0f, 1.f } });
Zone(TEXT("World s1d8, The Last Ash"), 22.330464f, 22.366656f);	// slot 494, 22.37 km
	Site(TEXT("WldS1D8PaintTinsM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ReflectorZ0"), -1, 26.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeStoreZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 22.420944f, 22.457136f);	// slot 496, 22.46 km
	Site(TEXT("WldS1D8CostumeRailM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StreetFrontZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), 1, 33.4f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 22.511424f, 22.547616f);	// slot 498, 22.55 km
	Site(TEXT("WldS1D8PropCutoutsM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ReflectorZ0"), -1, 26.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PalaceFrontZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintShopZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 22.601904f, 22.638096f);	// slot 500, 22.64 km
	Site(TEXT("WldS1D8CableCartM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StudioBarnZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8TownHallFrontZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 22.692384f, 22.728576f);	// slot 502, 22.73 km
	Site(TEXT("WldS1D8ChairRowM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), 1, 33.4f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 22.782864f, 22.819056f);	// slot 504, 22.82 km
	Site(TEXT("WldS1D8PaintTinsM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandZ0Y25"), -1, 26.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeStoreZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StudioBarnZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 22.873344f, 22.909536f);	// slot 506, 22.91 km
	Site(TEXT("WldS1D8CostumeRailM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StreetFrontZ0"), 1, 64.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BigSlateZ0"), -1, 130.0f, 1, 0, 1.f);	// THE BIG SLATE, 22.91 km
Zone(TEXT("World s1d8, The Last Ash"), 22.963824f, 23.000016f);	// slot 508, 23.00 km
	Site(TEXT("WldS1D8PropCutoutsM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintShopZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), 1, 33.4f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 23.054304f, 23.090496f);	// slot 510, 23.10 km
	Site(TEXT("WldS1D8CableCartM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ReflectorZ0"), 1, 26.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StudioBarnZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 23.144784f, 23.180976f);	// slot 512, 23.19 km
	Site(TEXT("WldS1D8ChairRowM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ReflectorZ0"), -1, 26.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StreetFrontZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8WaterTowerZ0"), 1, 64.6f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 23.235264f, 23.271456f);	// slot 514, 23.28 km
	Site(TEXT("WldS1D8PaintTinsM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LighthouseFrontZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), 1, 33.4f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 23.325744f, 23.361936f);	// slot 516, 23.37 km
	Site(TEXT("WldS1D8CostumeRailM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StudioBarnZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandZ0Y25"), 1, 26.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeStoreZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 23.416224f, 23.452416f);	// slot 518, 23.46 km
	Site(TEXT("WldS1D8PropCutoutsM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ReflectorZ0"), -1, 26.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeStoreZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 23.506704f, 23.542896f);	// slot 520, 23.55 km
	Site(TEXT("WldS1D8CableCartM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeStoreZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), 1, 33.4f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 23.597184f, 23.633376f);	// slot 522, 23.64 km
	Site(TEXT("WldS1D8ChairRowM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8TownHallFrontZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintShopZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 23.687664f, 23.723856f);	// slot 524, 23.73 km
	Site(TEXT("WldS1D8PaintTinsM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintShopZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LighthouseFrontZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 23.778144f, 23.814336f);	// slot 526, 23.82 km
	Site(TEXT("WldS1D8CostumeRailM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), 1, 33.4f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 23.868624f, 23.904816f);	// slot 528, 23.91 km
	Site(TEXT("WldS1D8PropCutoutsM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StreetFrontZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ReflectorZ0"), 1, 26.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintShopZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 23.959104f, 23.995296f);	// slot 530, 24.00 km
	Site(TEXT("WldS1D8CableCartM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8WaterTowerZ0"), -1, 61.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StreetFrontZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 24.049584f, 24.085776f);	// slot 532, 24.09 km
	Site(TEXT("WldS1D8ChairRowM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StudioBarnZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), 1, 33.4f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 24.140064f, 24.176256f);	// slot 534, 24.18 km
	Site(TEXT("WldS1D8PaintTinsM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandZ0Y25"), 1, 26.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StudioBarnZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 24.230544f, 24.266736f);	// slot 536, 24.27 km
	Site(TEXT("WldS1D8CostumeRailM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ReflectorZ0"), -1, 26.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeStoreZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PalaceFrontZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 24.321024f, 24.357216f);	// slot 538, 24.36 km
	Site(TEXT("WldS1D8PropCutoutsM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PalaceFrontZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), 1, 33.4f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 24.411504f, 24.447696f);	// slot 540, 24.45 km
	Site(TEXT("WldS1D8CableCartM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ReflectorZ0"), -1, 26.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintShopZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StreetFrontZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 24.501984f, 24.538176f);	// slot 542, 24.54 km
	Site(TEXT("WldS1D8ChairRowM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeStoreZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 24.592464f, 24.628656f);	// slot 544, 24.63 km
	Site(TEXT("WldS1D8PaintTinsM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BigSlateZ0"), -1, 130.0f, 1, 0, 1.f);	// THE BIG SLATE, 24.63 km
Zone(TEXT("World s1d8, The Last Ash"), 24.682944f, 24.719136f);	// slot 546, 24.72 km
	Site(TEXT("WldS1D8CostumeRailM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandZ0Y25"), -1, 26.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LighthouseFrontZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintShopZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 24.773424f, 24.809616f);	// slot 548, 24.81 km
	Site(TEXT("WldS1D8PropCutoutsM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StreetFrontZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ChairRowP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8TownHallFrontZ0"), 1, 64.2f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 24.863904f, 24.900096f);	// slot 550, 24.90 km
	Site(TEXT("WldS1D8CableCartM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), -1, 35.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintTinsP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BackdropRollM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StarArchP11"), 1, 33.4f, 1, 0, 1.f);
Zone(TEXT("World s1d8, The Last Ash"), 24.954384f, 24.990576f);	// slot 552, 25.00 km
	Site(TEXT("WldS1D8ChairRowM9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CableCartP9"), -1, 23.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StudioBarnZ0"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PropCutoutsM9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8CostumeRailP9"), 1, 23.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ReflectorZ0"), 1, 26.2f, 1, 0, 1.f);
	Site(TEXT("WldS1D8PaintCradleM11"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandP11Y20"), 1, 33.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D8StudioBarnZ0"), 1, 64.2f, 1, 0, 1.f);
#undef S1D8
// ---- END s1d8 ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People3 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1plants (the planting of the eight districts, ft_world_volcano_s1plants.py) ----
// Written by C:/ftwork/jobs/L3s1plants/make_inl.py. 21 meshes, all Scatter, chance 1, slots s % 4 == 3 only (every 4 phase 1; every 8 phase 5 =
// s % 8 == 3; every 8 phase 1 = s % 8 == 7). Counted slot by slot: 525 rule-slots = 525 pieces before the track leaves any out; x 0.75 (the
// brief's share) = about 394 placed. Rule-slots by district: s1d1 54, s1d2 54, s1d3 63, s1d4 80, s1d5 72, s1d6 51, s1d7 85, s1d8 66.
// By mesh (rule-slots): AshFern 60, MossCushion 56, Fireweed 55, Tussock 50, BoxTub 38, BentPine 33, Horsetail 28, PropPalm 26, PlumBlossom 24, PinkThrift 23, Willow 19, Dandelion 18, Bamboo 17, AshSunflower 16, GlassThorn 13, Chestnut 10, LemonTub 9, TreeFern 9, BirchTub 8, ReedBed 7, BentPineL 6.
// Bands (m from the centre line). Both sides: F 18.0-18.5 (pieces of 1.3 m and under, before the kerb of basalt stubs; not where a fence
// or marker stands); T 26.2-27.4 (thin stems and thin-trunked trees; pines and bamboo 28.0-28.8; in slots with no trestle, rostrum, catwalk or pipe on that side).
// Left: L1 32.0-33.4 (the story stages lane, where no stage or fumarole stands), L2 34.3-35.0 (2 m and under), ML 36.9-37.5 (the
// channel near bank), B 59.3-60.0 (big trees, scaled 1.4 to 2, in the back rank of the basalt wall: their crowns are meant to show
// over it). Right: R0 30.2-31.2, R1 34.0-35.4, R15 36.9-37.9 (2 m and under), R2 39.4-40.6, R3 41.7-42.5 (2 m and under), MR 43.6-45.0
// (the near bank of the channel). In a slot a band takes its first choice that fits, else its second (a low piece where a tree does not fit).
// THE CHANNEL RULE: solid pieces are 1.3 m or lower as built; every taller piece is thin stems or a crown that starts 3.3 m up on a thin trunk.
// Left out, slot by slot: the lava lakes, loops and corkscrews; beside a banked bend everything nearer than 40 m; on the right from 2.4 to
// 3.9 km everything but moss and fern; on the left from 0.3 to 0.6 km everything over 1.2 m; past 38 m at the gate of the pass; slots 35
// (the viaduct) and 139 (a river of lava); under a gantry of the channel works everything over 2.6 m, under a pipe bridge everything over 4.2 m; and every slot and
// side where a rule of FTDressL3.inl or FTPeopleL3.inl stands something in the band (a rule's chance was not rolled: taken counts as taken).
// Smallest x min of a TALL mesh: 26.2 m; of a low one: 18.0 m. BentPineL is BentPine's mirror twin, for the left of the road.
#define S1PLANTS(n) TEXT("/Game/World/Volcano/SM_S1PLANTS_" n)
Zone(TEXT("World s1plants, Cinder Springs"), 0.1131f, 0.3393f);   // plant slots 3 to 7
	Scatter(S1PLANTS("MossCushion"), 34.3f, 35.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 6.0f);
	Scatter(S1PLANTS("LemonTub"), 36.9f, 37.5f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("AshFern"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("LemonTub"), 34.0f, 35.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("TreeFern"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("AshFern"), 36.9f, 37.9f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Horsetail"), 39.4f, 40.6f, 8, 5, 1.f, 1.2f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("TreeFern"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("AshFern"), 41.7f, 42.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("TreeFern"), 43.6f, 45.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("LemonTub"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Cinder Springs"), 0.4750f, 0.7012f);   // plant slots 11 to 15
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.5f, 8, 5, 1.f, 1.4f, 0.10f, -1, 6.0f);
	Scatter(S1PLANTS("LemonTub"), 26.2f, 27.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Horsetail"), 39.4f, 40.6f, 8, 5, 1.f, 1.2f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("AshFern"), 41.7f, 42.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("TreeFern"), 43.6f, 45.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Cinder Springs"), 0.8369f, 1.0631f);   // plant slots 19 to 23
	Scatter(S1PLANTS("TreeFern"), 59.3f, 60.0f, 8, 5, 1.f, 2.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("AshFern"), 41.7f, 42.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("TreeFern"), 43.6f, 45.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("LemonTub"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Cinder Springs"), 1.7417f, 1.9679f);   // plant slots 39 to 43
	Scatter(S1PLANTS("LemonTub"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Cinder Springs"), 2.1037f, 2.3299f);   // plant slots 47 to 51
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 34.3f, 35.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("LemonTub"), 36.9f, 37.5f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Horsetail"), 30.2f, 31.2f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Cinder Springs"), 2.4656f, 2.6918f);   // plant slots 55 to 59
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("LemonTub"), 32.0f, 33.4f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 34.3f, 35.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 6.0f);
	Scatter(S1PLANTS("TreeFern"), 36.9f, 37.5f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("MossCushion"), 34.0f, 35.4f, 8, 1, 1.f, 1.3f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("AshFern"), 36.9f, 37.9f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("AshFern"), 39.4f, 40.6f, 8, 1, 1.f, 1.3f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("AshFern"), 43.6f, 45.0f, 8, 1, 1.f, 1.4f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Cinder Springs"), 2.8275f, 3.0537f);   // plant slots 63 to 67
	Scatter(S1PLANTS("TreeFern"), 59.3f, 60.0f, 8, 5, 1.f, 2.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("AshFern"), 41.7f, 42.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 43.6f, 45.0f, 8, 5, 1.f, 1.4f, 0.10f, 1, 6.0f);
Zone(TEXT("World s1plants, Cinder Springs"), 3.1894f, 3.4156f);   // plant slots 71 to 75
	Scatter(S1PLANTS("AshFern"), 43.6f, 45.0f, 8, 1, 1.f, 1.4f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Cinder Springs"), 3.5513f, 3.7775f);   // plant slots 79 to 83
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("LemonTub"), 36.9f, 37.5f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Horsetail"), 36.9f, 37.5f, 8, 1, 1.f, 1.2f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("TreeFern"), 59.3f, 60.0f, 8, 5, 1.f, 2.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 34.0f, 35.4f, 8, 5, 1.f, 1.3f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 34.0f, 35.4f, 8, 1, 1.f, 1.3f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("AshFern"), 36.9f, 37.9f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 39.4f, 40.6f, 8, 5, 1.f, 1.4f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("AshFern"), 39.4f, 40.6f, 8, 1, 1.f, 1.3f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("AshFern"), 41.7f, 42.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 43.6f, 45.0f, 8, 5, 1.f, 1.4f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("AshFern"), 43.6f, 45.0f, 8, 1, 1.f, 1.4f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Stormwatch"), 3.9133f, 4.1395f);   // plant slots 87 to 91
	Scatter(S1PLANTS("GlassThorn"), 34.3f, 35.0f, 8, 5, 1.f, 0.8f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 36.9f, 37.5f, 8, 5, 1.f, 1.3f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("GlassThorn"), 36.9f, 37.9f, 8, 5, 1.f, 0.8f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, 1, 25.0f);
Zone(TEXT("World s1plants, Stormwatch"), 4.2752f, 4.3204f);   // plant slots 95 to 95
	Scatter(S1PLANTS("Fireweed"), 26.2f, 27.4f, 4, 1, 1.f, 1.0f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("Fireweed"), 34.3f, 35.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("GlassThorn"), 36.9f, 37.5f, 8, 1, 1.f, 1.2f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("GlassThorn"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, 1, 25.0f);
Zone(TEXT("World s1plants, Stormwatch"), 4.4561f, 4.6823f);   // plant slots 99 to 103
	Scatter(S1PLANTS("GlassThorn"), 36.9f, 37.5f, 8, 1, 1.f, 1.2f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, Stormwatch"), 4.8181f, 5.0443f);   // plant slots 107 to 111
	Scatter(S1PLANTS("GlassThorn"), 34.3f, 35.0f, 8, 5, 1.f, 0.8f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 36.9f, 37.5f, 8, 5, 1.f, 1.3f, 0.10f, -1, 25.0f);
Zone(TEXT("World s1plants, Stormwatch"), 5.1800f, 5.4062f);   // plant slots 115 to 119
	Scatter(S1PLANTS("MossCushion"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("GlassThorn"), 43.6f, 45.0f, 8, 5, 1.f, 1.3f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, 1, 25.0f);
Zone(TEXT("World s1plants, Stormwatch"), 5.5419f, 5.5871f);   // plant slots 123 to 123
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("Fireweed"), 36.9f, 37.5f, 8, 5, 1.f, 1.3f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Fireweed"), 26.2f, 27.4f, 4, 1, 1.f, 1.0f, 0.10f, 1, 25.0f);
	Scatter(S1PLANTS("Fireweed"), 39.4f, 40.6f, 8, 5, 1.f, 1.2f, 0.10f, 1, 25.0f);
	Scatter(S1PLANTS("GlassThorn"), 43.6f, 45.0f, 8, 5, 1.f, 1.3f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Stormwatch"), 5.7229f, 5.7681f);   // plant slots 127 to 127
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("Fireweed"), 26.2f, 27.4f, 4, 1, 1.f, 1.0f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("Fireweed"), 32.0f, 33.4f, 8, 1, 1.f, 1.2f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("Fireweed"), 34.3f, 35.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("GlassThorn"), 36.9f, 37.5f, 8, 1, 1.f, 1.2f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Fireweed"), 26.2f, 27.4f, 4, 1, 1.f, 1.0f, 0.10f, 1, 25.0f);
	Scatter(S1PLANTS("Fireweed"), 30.2f, 31.2f, 8, 1, 1.f, 1.0f, 0.10f, 1, 25.0f);
Zone(TEXT("World s1plants, Stormwatch"), 6.4467f, 6.4919f);   // plant slots 143 to 143
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("Fireweed"), 26.2f, 27.4f, 4, 1, 1.f, 1.0f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("Fireweed"), 32.0f, 33.4f, 8, 1, 1.f, 1.2f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("Fireweed"), 34.3f, 35.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("GlassThorn"), 36.9f, 37.5f, 8, 1, 1.f, 1.2f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("GlassThorn"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, 1, 25.0f);
Zone(TEXT("World s1plants, Stormwatch"), 6.6277f, 7.0348f);   // plant slots 147 to 155
	Scatter(S1PLANTS("MossCushion"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, 1, 25.0f);
Zone(TEXT("World s1plants, Stormwatch"), 7.5325f, 7.7587f);   // plant slots 167 to 171
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("Fireweed"), 34.3f, 35.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("GlassThorn"), 36.9f, 37.5f, 8, 1, 1.f, 1.2f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, 1, 25.0f);
Zone(TEXT("World s1plants, Stormwatch"), 8.2563f, 8.3015f);   // plant slots 183 to 183
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("Fireweed"), 34.3f, 35.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("GlassThorn"), 36.9f, 37.5f, 8, 1, 1.f, 1.2f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Fireweed"), 26.2f, 27.4f, 4, 1, 1.f, 1.0f, 0.10f, 1, 25.0f);
	Scatter(S1PLANTS("Fireweed"), 30.2f, 31.2f, 8, 1, 1.f, 1.0f, 0.10f, 1, 25.0f);
	Scatter(S1PLANTS("Fireweed"), 34.0f, 35.4f, 8, 1, 1.f, 1.2f, 0.10f, 1, 25.0f);
Zone(TEXT("World s1plants, Slagtown"), 8.4373f, 8.4825f);   // plant slots 187 to 187
	Scatter(S1PLANTS("Tussock"), 36.9f, 37.5f, 8, 5, 1.f, 1.2f, 0.10f, -1, 30.0f);
Zone(TEXT("World s1plants, Slagtown"), 8.6182f, 8.8444f);   // plant slots 191 to 195
	Scatter(S1PLANTS("Tussock"), 18.0f, 18.5f, 4, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 34.3f, 35.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Tussock"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Tussock"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("AshFern"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, 1, 25.0f);
Zone(TEXT("World s1plants, Slagtown"), 8.9801f, 9.2063f);   // plant slots 199 to 203
	Scatter(S1PLANTS("Tussock"), 18.0f, 18.5f, 4, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("Tussock"), 36.9f, 37.5f, 8, 5, 1.f, 1.2f, 0.10f, -1, 30.0f);
	Scatter(S1PLANTS("Fireweed"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Tussock"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("BentPine"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("Tussock"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("AshFern"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, 1, 25.0f);
Zone(TEXT("World s1plants, Slagtown"), 9.3421f, 9.3873f);   // plant slots 207 to 207
	Scatter(S1PLANTS("BentPine"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("Tussock"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("AshFern"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, 1, 25.0f);
Zone(TEXT("World s1plants, Slagtown"), 9.5230f, 9.7492f);   // plant slots 211 to 215
	Scatter(S1PLANTS("Tussock"), 18.0f, 18.5f, 4, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("Tussock"), 34.3f, 35.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 30.0f);
	Scatter(S1PLANTS("Tussock"), 36.9f, 37.5f, 8, 5, 1.f, 1.2f, 0.10f, -1, 30.0f);
	Scatter(S1PLANTS("Fireweed"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Tussock"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Tussock"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("AshFern"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, 1, 25.0f);
Zone(TEXT("World s1plants, Slagtown"), 9.8849f, 10.1111f);   // plant slots 219 to 223
	Scatter(S1PLANTS("Tussock"), 18.0f, 18.5f, 4, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("Fireweed"), 26.2f, 27.4f, 8, 1, 1.f, 1.0f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("Fireweed"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Tussock"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("BentPine"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("Tussock"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("AshFern"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, 1, 25.0f);
Zone(TEXT("World s1plants, Slagtown"), 10.2469f, 10.2921f);   // plant slots 227 to 227
	Scatter(S1PLANTS("Tussock"), 36.9f, 37.5f, 8, 5, 1.f, 1.2f, 0.10f, -1, 30.0f);
Zone(TEXT("World s1plants, Slagtown"), 10.4278f, 10.6540f);   // plant slots 231 to 235
	Scatter(S1PLANTS("Tussock"), 18.0f, 18.5f, 4, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("Tussock"), 36.9f, 37.5f, 8, 5, 1.f, 1.2f, 0.10f, -1, 30.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Tussock"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("BentPine"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("Tussock"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("AshFern"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, 1, 25.0f);
Zone(TEXT("World s1plants, Slagtown"), 10.7897f, 11.0159f);   // plant slots 239 to 243
	Scatter(S1PLANTS("Tussock"), 18.0f, 18.5f, 4, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("Fireweed"), 26.2f, 27.4f, 8, 5, 1.f, 1.0f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("Fireweed"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Tussock"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Tussock"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("AshFern"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, 1, 25.0f);
Zone(TEXT("World s1plants, the Cliff Temple"), 11.1517f, 11.3779f);   // plant slots 247 to 251
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.5f, 8, 1, 1.f, 1.4f, 0.10f, -1, 6.0f);
	Scatter(S1PLANTS("PlumBlossom"), 59.3f, 60.0f, 8, 5, 1.f, 2.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Bamboo"), 34.0f, 35.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PlumBlossom"), 39.4f, 40.6f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Bamboo"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("BentPine"), 43.6f, 45.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("PlumBlossom"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, the Cliff Temple"), 11.5136f, 11.7398f);   // plant slots 255 to 259
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.5f, 8, 1, 1.f, 1.4f, 0.10f, -1, 6.0f);
	Scatter(S1PLANTS("PlumBlossom"), 59.3f, 60.0f, 8, 5, 1.f, 2.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Bamboo"), 59.3f, 60.0f, 8, 1, 1.f, 1.8f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Bamboo"), 28.0f, 28.8f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PlumBlossom"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PlumBlossom"), 39.4f, 40.6f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("BentPine"), 43.6f, 45.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 12.0f);
Zone(TEXT("World s1plants, the Cliff Temple"), 11.8755f, 12.1017f);   // plant slots 263 to 267
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("BentPineL"), 28.0f, 28.8f, 8, 1, 1.f, 1.0f, 0.10f, -1, 12.0f);
	Scatter(S1PLANTS("Bamboo"), 36.9f, 37.5f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Bamboo"), 34.0f, 35.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PlumBlossom"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PlumBlossom"), 39.4f, 40.6f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Bamboo"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("BentPine"), 43.6f, 45.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("PlumBlossom"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, the Cliff Temple"), 12.2374f, 12.4636f);   // plant slots 271 to 275
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.5f, 8, 1, 1.f, 1.4f, 0.10f, -1, 6.0f);
	Scatter(S1PLANTS("PlumBlossom"), 59.3f, 60.0f, 8, 5, 1.f, 2.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Bamboo"), 59.3f, 60.0f, 8, 1, 1.f, 1.8f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Bamboo"), 34.0f, 35.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Bamboo"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PlumBlossom"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, the Cliff Temple"), 12.5993f, 12.8255f);   // plant slots 279 to 283
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("PlumBlossom"), 36.9f, 37.5f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Bamboo"), 59.3f, 60.0f, 8, 1, 1.f, 1.8f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("PlumBlossom"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PlumBlossom"), 39.4f, 40.6f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Bamboo"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("BentPine"), 43.6f, 45.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("PlumBlossom"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, the Cliff Temple"), 12.9613f, 13.1875f);   // plant slots 287 to 291
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.5f, 8, 1, 1.f, 1.4f, 0.10f, -1, 6.0f);
	Scatter(S1PLANTS("PlumBlossom"), 59.3f, 60.0f, 8, 5, 1.f, 2.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Bamboo"), 34.0f, 35.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PlumBlossom"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PlumBlossom"), 39.4f, 40.6f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Bamboo"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("BentPine"), 43.6f, 45.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("PlumBlossom"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, the Cliff Temple"), 13.3232f, 13.5494f);   // plant slots 295 to 299
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.5f, 8, 1, 1.f, 1.4f, 0.10f, -1, 6.0f);
	Scatter(S1PLANTS("PlumBlossom"), 59.3f, 60.0f, 8, 5, 1.f, 2.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Bamboo"), 59.3f, 60.0f, 8, 1, 1.f, 1.8f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Bamboo"), 34.0f, 35.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PlumBlossom"), 39.4f, 40.6f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("BentPine"), 43.6f, 45.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 12.0f);
Zone(TEXT("World s1plants, the Cliff Temple"), 13.6851f, 13.9113f);   // plant slots 303 to 307
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("BentPineL"), 28.0f, 28.8f, 8, 1, 1.f, 1.0f, 0.10f, -1, 12.0f);
	Scatter(S1PLANTS("PlumBlossom"), 59.3f, 60.0f, 8, 5, 1.f, 2.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 34.0f, 35.4f, 8, 1, 1.f, 1.3f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PlumBlossom"), 39.4f, 40.6f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Bamboo"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("BentPine"), 43.6f, 45.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("PlumBlossom"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Windmill Flats"), 14.0470f, 14.0923f);   // plant slots 311 to 311
	Scatter(S1PLANTS("Tussock"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 30.0f);
	Scatter(S1PLANTS("AshSunflower"), 26.2f, 27.4f, 4, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Tussock"), 30.2f, 31.2f, 8, 1, 1.f, 1.0f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("PinkThrift"), 36.9f, 37.9f, 4, 1, 1.f, 1.0f, 0.10f, 1, 20.0f);
	Scatter(S1PLANTS("AshSunflower"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Tussock"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("Willow"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Windmill Flats"), 14.2280f, 14.2732f);   // plant slots 315 to 315
	Scatter(S1PLANTS("AshSunflower"), 26.2f, 27.4f, 4, 1, 1.f, 1.0f, 0.10f, -1, 6.0f);
	Scatter(S1PLANTS("PinkThrift"), 36.9f, 37.5f, 8, 5, 1.f, 1.4f, 0.10f, -1, 20.0f);
	Scatter(S1PLANTS("Willow"), 59.3f, 60.0f, 8, 5, 1.f, 1.9f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, Windmill Flats"), 14.4089f, 14.4542f);   // plant slots 319 to 319
	Scatter(S1PLANTS("Tussock"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 30.0f);
	Scatter(S1PLANTS("AshSunflower"), 26.2f, 27.4f, 4, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Tussock"), 30.2f, 31.2f, 8, 1, 1.f, 1.0f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("Willow"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PinkThrift"), 36.9f, 37.9f, 4, 1, 1.f, 1.0f, 0.10f, 1, 20.0f);
	Scatter(S1PLANTS("AshSunflower"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Tussock"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("Willow"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Windmill Flats"), 14.5899f, 14.6351f);   // plant slots 323 to 323
	Scatter(S1PLANTS("PinkThrift"), 36.9f, 37.9f, 4, 1, 1.f, 1.0f, 0.10f, 1, 20.0f);
	Scatter(S1PLANTS("Willow"), 39.4f, 40.6f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PinkThrift"), 41.7f, 42.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 20.0f);
	Scatter(S1PLANTS("PinkThrift"), 43.6f, 45.0f, 8, 5, 1.f, 1.4f, 0.10f, 1, 20.0f);
Zone(TEXT("World s1plants, Windmill Flats"), 14.7709f, 14.8161f);   // plant slots 327 to 327
	Scatter(S1PLANTS("Tussock"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 30.0f);
	Scatter(S1PLANTS("AshSunflower"), 26.2f, 27.4f, 4, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Tussock"), 30.2f, 31.2f, 8, 1, 1.f, 1.0f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("Tussock"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("Willow"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Windmill Flats"), 14.9518f, 14.9971f);   // plant slots 331 to 331
	Scatter(S1PLANTS("PinkThrift"), 36.9f, 37.5f, 8, 5, 1.f, 1.4f, 0.10f, -1, 20.0f);
	Scatter(S1PLANTS("AshSunflower"), 26.2f, 27.4f, 4, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("PinkThrift"), 36.9f, 37.9f, 4, 1, 1.f, 1.0f, 0.10f, 1, 20.0f);
	Scatter(S1PLANTS("Willow"), 39.4f, 40.6f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PinkThrift"), 41.7f, 42.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 20.0f);
	Scatter(S1PLANTS("PinkThrift"), 43.6f, 45.0f, 8, 5, 1.f, 1.4f, 0.10f, 1, 20.0f);
Zone(TEXT("World s1plants, Windmill Flats"), 15.1328f, 15.1780f);   // plant slots 335 to 335
	Scatter(S1PLANTS("Tussock"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 30.0f);
	Scatter(S1PLANTS("PinkThrift"), 36.9f, 37.9f, 4, 1, 1.f, 1.0f, 0.10f, 1, 20.0f);
Zone(TEXT("World s1plants, Windmill Flats"), 15.4947f, 15.7209f);   // plant slots 343 to 347
	Scatter(S1PLANTS("PinkThrift"), 36.9f, 37.5f, 8, 5, 1.f, 1.4f, 0.10f, -1, 20.0f);
	Scatter(S1PLANTS("Willow"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PinkThrift"), 36.9f, 37.9f, 4, 1, 1.f, 1.0f, 0.10f, 1, 20.0f);
	Scatter(S1PLANTS("Willow"), 39.4f, 40.6f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("AshSunflower"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("PinkThrift"), 41.7f, 42.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 20.0f);
	Scatter(S1PLANTS("Tussock"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("PinkThrift"), 43.6f, 45.0f, 8, 5, 1.f, 1.4f, 0.10f, 1, 20.0f);
	Scatter(S1PLANTS("Willow"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Windmill Flats"), 15.8566f, 15.9019f);   // plant slots 351 to 351
	Scatter(S1PLANTS("AshSunflower"), 26.2f, 27.4f, 4, 1, 1.f, 1.0f, 0.10f, -1, 6.0f);
	Scatter(S1PLANTS("Tussock"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 30.0f);
	Scatter(S1PLANTS("AshSunflower"), 26.2f, 27.4f, 4, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Tussock"), 30.2f, 31.2f, 8, 1, 1.f, 1.0f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("Tussock"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("Willow"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Windmill Flats"), 16.0376f, 16.0828f);   // plant slots 355 to 355
	Scatter(S1PLANTS("AshSunflower"), 26.2f, 27.4f, 4, 1, 1.f, 1.0f, 0.10f, -1, 6.0f);
	Scatter(S1PLANTS("PinkThrift"), 36.9f, 37.5f, 8, 5, 1.f, 1.4f, 0.10f, -1, 20.0f);
	Scatter(S1PLANTS("AshSunflower"), 34.0f, 35.4f, 8, 5, 1.f, 1.2f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("PinkThrift"), 36.9f, 37.9f, 4, 1, 1.f, 1.0f, 0.10f, 1, 20.0f);
Zone(TEXT("World s1plants, Windmill Flats"), 16.2185f, 16.2638f);   // plant slots 359 to 359
	Scatter(S1PLANTS("Tussock"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 30.0f);
	Scatter(S1PLANTS("AshSunflower"), 26.2f, 27.4f, 4, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Tussock"), 30.2f, 31.2f, 8, 1, 1.f, 1.0f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("PinkThrift"), 36.9f, 37.9f, 4, 1, 1.f, 1.0f, 0.10f, 1, 20.0f);
	Scatter(S1PLANTS("AshSunflower"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Tussock"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("Willow"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Windmill Flats"), 16.3995f, 16.4447f);   // plant slots 363 to 363
	Scatter(S1PLANTS("AshSunflower"), 26.2f, 27.4f, 4, 1, 1.f, 1.0f, 0.10f, -1, 6.0f);
	Scatter(S1PLANTS("Willow"), 59.3f, 60.0f, 8, 5, 1.f, 1.9f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("PinkThrift"), 41.7f, 42.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 20.0f);
	Scatter(S1PLANTS("PinkThrift"), 43.6f, 45.0f, 8, 5, 1.f, 1.4f, 0.10f, 1, 20.0f);
Zone(TEXT("World s1plants, Windmill Flats"), 16.5805f, 16.6257f);   // plant slots 367 to 367
	Scatter(S1PLANTS("Tussock"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 30.0f);
	Scatter(S1PLANTS("PinkThrift"), 34.0f, 35.4f, 8, 1, 1.f, 1.3f, 0.10f, 1, 20.0f);
	Scatter(S1PLANTS("PinkThrift"), 36.9f, 37.9f, 4, 1, 1.f, 1.0f, 0.10f, 1, 20.0f);
	Scatter(S1PLANTS("AshSunflower"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Tussock"), 41.7f, 42.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 30.0f);
	Scatter(S1PLANTS("Willow"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Geotherm"), 16.7614f, 17.1686f);   // plant slots 371 to 379
	Scatter(S1PLANTS("Horsetail"), 36.9f, 37.5f, 8, 5, 1.f, 0.8f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("ReedBed"), 36.9f, 37.5f, 8, 1, 1.f, 0.9f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Willow"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.9f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Horsetail"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Horsetail"), 43.6f, 45.0f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Geotherm"), 17.3043f, 17.5305f);   // plant slots 383 to 387
	Scatter(S1PLANTS("Horsetail"), 36.9f, 37.5f, 8, 5, 1.f, 0.8f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, Geotherm"), 17.6662f, 17.8924f);   // plant slots 391 to 395
	Scatter(S1PLANTS("Horsetail"), 36.9f, 37.5f, 8, 5, 1.f, 0.8f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("ReedBed"), 36.9f, 37.5f, 8, 1, 1.f, 0.9f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Horsetail"), 26.2f, 27.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("BirchTub"), 26.2f, 27.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("BirchTub"), 30.2f, 31.2f, 8, 5, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("BirchTub"), 30.2f, 31.2f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Willow"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.9f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Horsetail"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Horsetail"), 43.6f, 45.0f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Geotherm"), 18.0281f, 18.2543f);   // plant slots 399 to 403
	Scatter(S1PLANTS("BirchTub"), 26.2f, 27.4f, 8, 5, 1.f, 1.0f, 0.10f, -1, 6.0f);
	Scatter(S1PLANTS("Horsetail"), 36.9f, 37.5f, 8, 5, 1.f, 0.8f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("ReedBed"), 36.9f, 37.5f, 8, 1, 1.f, 0.9f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Willow"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.9f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Horsetail"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Horsetail"), 43.6f, 45.0f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Geotherm"), 18.3901f, 18.6163f);   // plant slots 407 to 411
	Scatter(S1PLANTS("Horsetail"), 36.9f, 37.5f, 8, 5, 1.f, 0.8f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("ReedBed"), 36.9f, 37.5f, 8, 1, 1.f, 0.9f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Horsetail"), 26.2f, 27.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("BirchTub"), 30.2f, 31.2f, 8, 5, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.9f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Horsetail"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Horsetail"), 43.6f, 45.0f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Geotherm"), 18.7520f, 18.9782f);   // plant slots 415 to 419
	Scatter(S1PLANTS("Horsetail"), 36.9f, 37.5f, 8, 5, 1.f, 0.8f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("ReedBed"), 36.9f, 37.5f, 8, 1, 1.f, 0.9f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Horsetail"), 26.2f, 27.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("BirchTub"), 30.2f, 31.2f, 8, 5, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Willow"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.9f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Horsetail"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Horsetail"), 43.6f, 45.0f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Geotherm"), 19.1139f, 19.3401f);   // plant slots 423 to 427
	Scatter(S1PLANTS("ReedBed"), 36.9f, 37.5f, 8, 1, 1.f, 0.9f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("BirchTub"), 26.2f, 27.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("BirchTub"), 30.2f, 31.2f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Willow"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.9f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Horsetail"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Horsetail"), 43.6f, 45.0f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Geotherm"), 19.4758f, 19.5211f);   // plant slots 431 to 431
	Scatter(S1PLANTS("ReedBed"), 36.9f, 37.5f, 8, 1, 1.f, 0.9f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.9f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Horsetail"), 39.4f, 40.6f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Horsetail"), 43.6f, 45.0f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, the Wall Keep"), 19.6568f, 19.7020f);   // plant slots 435 to 435
	Scatter(S1PLANTS("BoxTub"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 36.9f, 37.5f, 8, 5, 1.f, 1.3f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("BentPine"), 28.0f, 28.8f, 4, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("BoxTub"), 30.2f, 31.2f, 8, 5, 1.f, 1.0f, 0.10f, 1, 10.0f);
	Scatter(S1PLANTS("BentPine"), 34.0f, 35.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("BoxTub"), 39.4f, 40.6f, 8, 5, 1.f, 1.2f, 0.10f, 1, 10.0f);
	Scatter(S1PLANTS("Chestnut"), 43.6f, 45.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, the Wall Keep"), 19.8377f, 19.8830f);   // plant slots 439 to 439
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("BoxTub"), 36.9f, 37.5f, 8, 1, 1.f, 1.2f, 0.10f, -1, 10.0f);
	Scatter(S1PLANTS("Chestnut"), 59.3f, 60.0f, 4, 1, 1.f, 1.4f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("BoxTub"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("BentPine"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("BentPine"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
Zone(TEXT("World s1plants, the Wall Keep"), 20.0187f, 20.0639f);   // plant slots 443 to 443
	Scatter(S1PLANTS("BoxTub"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 36.9f, 37.5f, 8, 5, 1.f, 1.3f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("BentPine"), 34.0f, 35.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("BoxTub"), 39.4f, 40.6f, 8, 5, 1.f, 1.2f, 0.10f, 1, 10.0f);
	Scatter(S1PLANTS("Chestnut"), 43.6f, 45.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, the Wall Keep"), 20.1997f, 20.2449f);   // plant slots 447 to 447
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("BoxTub"), 34.3f, 35.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 10.0f);
	Scatter(S1PLANTS("BoxTub"), 36.9f, 37.5f, 8, 1, 1.f, 1.2f, 0.10f, -1, 10.0f);
	Scatter(S1PLANTS("BoxTub"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("BentPine"), 28.0f, 28.8f, 4, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("BoxTub"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 10.0f);
	Scatter(S1PLANTS("BentPine"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("BentPine"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
Zone(TEXT("World s1plants, the Wall Keep"), 20.3806f, 20.4259f);   // plant slots 451 to 451
	Scatter(S1PLANTS("BoxTub"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("BoxTub"), 32.0f, 33.4f, 8, 5, 1.f, 1.0f, 0.10f, -1, 10.0f);
	Scatter(S1PLANTS("BentPineL"), 36.9f, 37.5f, 8, 5, 1.f, 1.0f, 0.10f, -1, 12.0f);
	Scatter(S1PLANTS("Chestnut"), 59.3f, 60.0f, 4, 1, 1.f, 1.4f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("BentPine"), 28.0f, 28.8f, 4, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("BoxTub"), 30.2f, 31.2f, 8, 5, 1.f, 1.0f, 0.10f, 1, 10.0f);
	Scatter(S1PLANTS("BoxTub"), 39.4f, 40.6f, 8, 5, 1.f, 1.2f, 0.10f, 1, 10.0f);
	Scatter(S1PLANTS("Chestnut"), 43.6f, 45.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, the Wall Keep"), 20.5616f, 20.6068f);   // plant slots 455 to 455
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("BoxTub"), 34.3f, 35.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 10.0f);
	Scatter(S1PLANTS("BoxTub"), 36.9f, 37.5f, 8, 1, 1.f, 1.2f, 0.10f, -1, 10.0f);
	Scatter(S1PLANTS("BoxTub"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("BentPine"), 28.0f, 28.8f, 4, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("BoxTub"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 10.0f);
	Scatter(S1PLANTS("BentPine"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("BentPine"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
Zone(TEXT("World s1plants, the Wall Keep"), 20.7425f, 20.9687f);   // plant slots 459 to 463
	Scatter(S1PLANTS("BoxTub"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 36.9f, 37.5f, 8, 5, 1.f, 1.3f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("BoxTub"), 36.9f, 37.5f, 8, 1, 1.f, 1.2f, 0.10f, -1, 10.0f);
	Scatter(S1PLANTS("BoxTub"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("BentPine"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("BoxTub"), 43.6f, 45.0f, 8, 5, 1.f, 1.2f, 0.10f, 1, 10.0f);
	Scatter(S1PLANTS("BentPine"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
Zone(TEXT("World s1plants, the Wall Keep"), 21.1045f, 21.3307f);   // plant slots 467 to 471
	Scatter(S1PLANTS("BoxTub"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 36.9f, 37.5f, 8, 5, 1.f, 1.3f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("BentPine"), 34.0f, 35.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("BoxTub"), 39.4f, 40.6f, 8, 5, 1.f, 1.2f, 0.10f, 1, 10.0f);
	Scatter(S1PLANTS("Chestnut"), 43.6f, 45.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, the Wall Keep"), 21.4664f, 21.5116f);   // plant slots 475 to 475
	Scatter(S1PLANTS("BoxTub"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 36.9f, 37.5f, 8, 5, 1.f, 1.3f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Chestnut"), 59.3f, 60.0f, 4, 1, 1.f, 1.4f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("BoxTub"), 39.4f, 40.6f, 8, 5, 1.f, 1.2f, 0.10f, 1, 10.0f);
	Scatter(S1PLANTS("Chestnut"), 43.6f, 45.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, the Wall Keep"), 21.6473f, 21.6926f);   // plant slots 479 to 479
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("BoxTub"), 36.9f, 37.5f, 8, 1, 1.f, 1.2f, 0.10f, -1, 10.0f);
	Scatter(S1PLANTS("BoxTub"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("BoxTub"), 34.0f, 35.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 10.0f);
	Scatter(S1PLANTS("BentPine"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("BentPine"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
Zone(TEXT("World s1plants, the Wall Keep"), 21.8283f, 22.0545f);   // plant slots 483 to 487
	Scatter(S1PLANTS("BoxTub"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("AshFern"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("BoxTub"), 32.0f, 33.4f, 8, 5, 1.f, 1.0f, 0.10f, -1, 10.0f);
	Scatter(S1PLANTS("BentPineL"), 36.9f, 37.5f, 8, 5, 1.f, 1.0f, 0.10f, -1, 12.0f);
	Scatter(S1PLANTS("BoxTub"), 36.9f, 37.5f, 8, 1, 1.f, 1.2f, 0.10f, -1, 10.0f);
	Scatter(S1PLANTS("BoxTub"), 18.0f, 18.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("BentPine"), 28.0f, 28.8f, 4, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("BoxTub"), 30.2f, 31.2f, 8, 5, 1.f, 1.0f, 0.10f, 1, 10.0f);
	Scatter(S1PLANTS("BentPine"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("BoxTub"), 43.6f, 45.0f, 8, 5, 1.f, 1.2f, 0.10f, 1, 10.0f);
	Scatter(S1PLANTS("BentPine"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 12.0f);
Zone(TEXT("World s1plants, the Wall Keep"), 22.1902f, 22.2355f);   // plant slots 491 to 491
	Scatter(S1PLANTS("BoxTub"), 18.0f, 18.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("BentPineL"), 28.0f, 28.8f, 4, 1, 1.f, 1.0f, 0.10f, -1, 12.0f);
	Scatter(S1PLANTS("BentPineL"), 36.9f, 37.5f, 8, 5, 1.f, 1.0f, 0.10f, -1, 12.0f);
	Scatter(S1PLANTS("Chestnut"), 59.3f, 60.0f, 4, 1, 1.f, 1.4f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("BentPine"), 34.0f, 35.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 12.0f);
	Scatter(S1PLANTS("BoxTub"), 39.4f, 40.6f, 8, 5, 1.f, 1.2f, 0.10f, 1, 10.0f);
	Scatter(S1PLANTS("Chestnut"), 43.6f, 45.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, the Backlot"), 22.3712f, 22.5974f);   // plant slots 495 to 499
	Scatter(S1PLANTS("PropPalm"), 36.9f, 37.5f, 8, 5, 1.f, 1.0f, 0.10f, -1, 8.0f);
	Scatter(S1PLANTS("PropPalm"), 34.0f, 35.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 8.0f);
	Scatter(S1PLANTS("Dandelion"), 39.4f, 40.6f, 8, 5, 1.f, 1.3f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Dandelion"), 41.7f, 42.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 5, 1.f, 1.3f, 0.10f, 1, 25.0f);
Zone(TEXT("World s1plants, the Backlot"), 22.7331f, 22.9593f);   // plant slots 503 to 507
	Scatter(S1PLANTS("PropPalm"), 26.2f, 27.4f, 8, 5, 1.f, 1.0f, 0.10f, -1, 8.0f);
	Scatter(S1PLANTS("PropPalm"), 36.9f, 37.5f, 8, 5, 1.f, 1.0f, 0.10f, -1, 8.0f);
	Scatter(S1PLANTS("Fireweed"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("Dandelion"), 26.2f, 27.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 30.2f, 31.2f, 8, 5, 1.f, 1.0f, 0.10f, 1, 25.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.9f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("PropPalm"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 8.0f);
	Scatter(S1PLANTS("Dandelion"), 41.7f, 42.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 5, 1.f, 1.3f, 0.10f, 1, 25.0f);
	Scatter(S1PLANTS("PropPalm"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 8.0f);
Zone(TEXT("World s1plants, the Backlot"), 23.0950f, 23.3212f);   // plant slots 511 to 515
	Scatter(S1PLANTS("Fireweed"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("Dandelion"), 34.0f, 35.4f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.9f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("PropPalm"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 8.0f);
	Scatter(S1PLANTS("PropPalm"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 8.0f);
Zone(TEXT("World s1plants, the Backlot"), 23.4569f, 23.6831f);   // plant slots 519 to 523
	Scatter(S1PLANTS("PropPalm"), 26.2f, 27.4f, 8, 5, 1.f, 1.0f, 0.10f, -1, 8.0f);
	Scatter(S1PLANTS("Dandelion"), 26.2f, 27.4f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("PropPalm"), 36.9f, 37.5f, 8, 5, 1.f, 1.0f, 0.10f, -1, 8.0f);
	Scatter(S1PLANTS("Fireweed"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("Dandelion"), 26.2f, 27.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 30.2f, 31.2f, 8, 5, 1.f, 1.0f, 0.10f, 1, 25.0f);
	Scatter(S1PLANTS("PropPalm"), 34.0f, 35.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 8.0f);
	Scatter(S1PLANTS("Dandelion"), 39.4f, 40.6f, 8, 5, 1.f, 1.3f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Dandelion"), 41.7f, 42.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 5, 1.f, 1.3f, 0.10f, 1, 25.0f);
	Scatter(S1PLANTS("PropPalm"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 8.0f);
Zone(TEXT("World s1plants, the Backlot"), 23.8189f, 24.0451f);   // plant slots 527 to 531
	Scatter(S1PLANTS("PropPalm"), 36.9f, 37.5f, 8, 5, 1.f, 1.0f, 0.10f, -1, 8.0f);
	Scatter(S1PLANTS("Fireweed"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("Dandelion"), 26.2f, 27.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PropPalm"), 26.2f, 27.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 8.0f);
	Scatter(S1PLANTS("Fireweed"), 30.2f, 31.2f, 8, 5, 1.f, 1.0f, 0.10f, 1, 25.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.9f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("PropPalm"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 8.0f);
	Scatter(S1PLANTS("Dandelion"), 41.7f, 42.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 5, 1.f, 1.3f, 0.10f, 1, 25.0f);
	Scatter(S1PLANTS("PropPalm"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 8.0f);
Zone(TEXT("World s1plants, the Backlot"), 24.1808f, 24.5879f);   // plant slots 535 to 543
	Scatter(S1PLANTS("Dandelion"), 26.2f, 27.4f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("Dandelion"), 34.0f, 35.4f, 8, 1, 1.f, 1.2f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.9f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("PropPalm"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 8.0f);
	Scatter(S1PLANTS("PropPalm"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 8.0f);
Zone(TEXT("World s1plants, the Backlot"), 24.7237f, 99.f);   // plant slots 547 to 551
	Scatter(S1PLANTS("PropPalm"), 26.2f, 27.4f, 8, 5, 1.f, 1.0f, 0.10f, -1, 8.0f);
	Scatter(S1PLANTS("AshFern"), 34.3f, 35.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("PropPalm"), 36.9f, 37.5f, 8, 5, 1.f, 1.0f, 0.10f, -1, 8.0f);
	Scatter(S1PLANTS("Fireweed"), 36.9f, 37.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 25.0f);
	Scatter(S1PLANTS("PropPalm"), 26.2f, 27.4f, 8, 1, 1.f, 1.0f, 0.10f, 1, 8.0f);
	Scatter(S1PLANTS("Dandelion"), 30.2f, 31.2f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PropPalm"), 34.0f, 35.4f, 8, 5, 1.f, 1.0f, 0.10f, 1, 8.0f);
	Scatter(S1PLANTS("MossCushion"), 36.9f, 37.9f, 8, 1, 1.f, 1.0f, 0.10f, 1, 6.0f);
	Scatter(S1PLANTS("Dandelion"), 39.4f, 40.6f, 8, 5, 1.f, 1.3f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PropPalm"), 39.4f, 40.6f, 8, 1, 1.f, 1.0f, 0.10f, 1, 8.0f);
	Scatter(S1PLANTS("Dandelion"), 41.7f, 42.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Fireweed"), 43.6f, 45.0f, 8, 5, 1.f, 1.3f, 0.10f, 1, 25.0f);
	Scatter(S1PLANTS("PropPalm"), 43.6f, 45.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 8.0f);
#undef S1PLANTS
// ---- END s1plants ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People3 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1animals (the animals of the eight districts, ft_world_volcano_s1animals.py) ----
// 19 meshes, /Game/World/Volcano/SM_S1ANIMALS_*. Estimate (every rule counted slot by slot against its zone, both tables read:
// C:/ftwork/jobs/L3s1animals/occ.py, block.py, wishes.py): Scatter 247 places x 0.75 = 185.2, Far 36 x 1 = 36: about 221 placed.
// By mesh (placed): GlassIguanas 30, PitPonies 29, WhiteHorse 23, MothCloud 23, CraneSkein 13, SpringMonkeys 12, StudioPets 12, CurlHornSheep 10, Cranes 10, Peacocks 10, ShardLizard 8, Hens 8, PackBeasts 8, Egrets 6, KoiBasin 6, GreatTortoise 4, Frogs 4, PinkWaders 3, Geese 2.
// The most of one mesh: GlassIguanas, 40 places (about 30 placed). The smallest x min of a TALL mesh: 60 m (GreatTortoise, Cranes, PinkWaders).
// Bands and slots (the brief's): LOW pieces by a channel's near bank, right 43 to 44.4 m (once on the left, 34 to 35.6 m), s%8==5;
// behind the channels where no basalt wall stands in the slot, left 60 to 61.6 m and right 63.8 to 64.6 m (66.6 to 67.2), s%4==1;
// two rows on the flat beyond, 80 to 86 m and 100 to 106 m (78 to 84, 88 to 94), s%4==1 and s%4==3. Sky (Far, a negative drop): MothCloud
// 76 to 112 m out, 7.5 to 13 m over the road, only from 3.76 to 7.88 km (before the boss's road); CraneSkein 155 to 185 m out, 43 to 46 m up,
// flying ALONG the road and round again (it never circles), in the Cliff Temple and Geotherm only.
// Each run of slots between two slots where a piece must not stand is a Zone of its own (so a rule never reaches into a wall's slot, a lava
// lake, a set of the dressing or a row of the population). Left out: the right side 2.35 to 3.95 km (the eruption), the left 0.27 to 0.63 km
// (the curtain of fire), 6.6 to 6.84 km behind the channels (the gate of the pass), 50 m either side of every lava lake, the near band beside
// the banked bends, every slot with a basalt wall on that side (behind-the-channel and beyond), the loops and corkscrews for the sky pieces.
// No tall mesh stands both inside and outside 70 m: inside Cranes, PinkWaders, GreatTortoise; outside SpringMonkeys, GlassIguanas, PitPonies,
// PackBeasts, CurlHornSheep, WhiteHorse. The eight low meshes (bounds under 2.5 m with their footing) stand at scale 1 everywhere.
#define S1ANIMALS(n) TEXT("/Game/World/Volcano/SM_S1ANIMALS_" n)
Zone(TEXT("World s1animals, Cinder Springs, the great tortoise by the start"), 0.033f, 0.057f);
	Scatter(S1ANIMALS("GreatTortoise"), 60.f, 61.6f, 4, 3, 1.f, 1.f, 0.f, -1, 3.f);   // THE GREAT TORTOISE behind the left channel: 1 slot (s%4==1), left
Zone(TEXT("World s1animals, Cinder Springs, the right"), 0.395f, 0.6f);
	Scatter(S1ANIMALS("SpringMonkeys"), 80.f, 86.f, 4, 3, 1.f, 1.3f, 0.05f, 1, 8.f);   // monkeys soaking in a hot pool: 2 slots (s%4==1), right
Zone(TEXT("World s1animals, Cinder Springs, the right, the far row"), 0.214f, 0.238f);
	Scatter(S1ANIMALS("SpringMonkeys"), 100.f, 106.f, 4, 3, 1.f, 1.3f, 0.05f, 1, 8.f);   // monkeys soaking in a hot pool: 1 slot (s%4==1), right
Zone(TEXT("World s1animals, Cinder Springs, the right, the far row"), 0.938f, 1.143f);
	Scatter(S1ANIMALS("SpringMonkeys"), 100.f, 106.f, 4, 3, 1.f, 1.3f, 0.05f, 1, 8.f);   // monkeys soaking in a hot pool: 2 slots (s%4==1), right
Zone(TEXT("World s1animals, Cinder Springs, the right, the far row"), 0.124f, 0.51f);
	Scatter(S1ANIMALS("SpringMonkeys"), 100.f, 106.f, 4, 1, 1.f, 1.3f, 0.05f, 1, 8.f);   // monkeys soaking in a hot pool: 3 slots (s%4==3), right
Zone(TEXT("World s1animals, Cinder Springs, the right, the far row"), 0.848f, 1.053f);
	Scatter(S1ANIMALS("SpringMonkeys"), 100.f, 106.f, 4, 1, 1.f, 1.3f, 0.05f, 1, 8.f);   // monkeys soaking in a hot pool: 2 slots (s%4==3), right
Zone(TEXT("World s1animals, Cinder Springs, by the right channel (lava jumps)"), 1.3f, 1.324f);
	Scatter(S1ANIMALS("Egrets"), 43.f, 44.4f, 8, 3, 1.f, 1.f, 0.f, 1, 4.f);   // egrets in a pool, low: 1 slot (s%8==5), right
Zone(TEXT("World s1animals, Cinder Springs, by the left channel (lava jumps)"), 1.3f, 1.324f);
	Scatter(S1ANIMALS("Egrets"), 34.f, 35.6f, 8, 3, 1.f, 1.f, 0.f, -1, 4.f);   // egrets in a pool, low: 1 slot (s%8==5), left
Zone(TEXT("World s1animals, Cinder Springs, lava jumps, the left"), 1.209f, 1.233f);
	Scatter(S1ANIMALS("SpringMonkeys"), 80.f, 86.f, 4, 1, 1.f, 1.3f, 0.05f, -1, 8.f);   // monkeys soaking in a hot pool: 1 slot (s%4==3), left
Zone(TEXT("World s1animals, Cinder Springs, lava jumps, the left"), 1.571f, 2.319f);
	Scatter(S1ANIMALS("SpringMonkeys"), 76.f, 80.f, 4, 1, 1.f, 1.3f, 0.05f, -1, 8.f);   // monkeys soaking in a hot pool: 3 slots (s%4==3), left   // review round: was 80 to 86 m; one pool stood in the pass 3 ore tubs at 88.6 m (1.94 km)
Zone(TEXT("World s1animals, Cinder Springs, lava jumps, the right"), 1.3f, 1.686f);
	Scatter(S1ANIMALS("SpringMonkeys"), 80.f, 86.f, 8, 3, 1.f, 1.3f, 0.05f, 1, 8.f);   // monkeys soaking in a hot pool: 2 slots (s%8==5), right
Zone(TEXT("World s1animals, Stormwatch, by the right channel"), 4.195f, 4.581f);
	Scatter(S1ANIMALS("ShardLizard"), 43.f, 44.4f, 8, 3, 1.f, 1.f, 0.f, 1, 4.f);   // lizards on an obsidian slab, low: 2 slots (s%8==5), right
Zone(TEXT("World s1animals, Stormwatch, by the right channel"), 6.729f, 6.753f);
	Scatter(S1ANIMALS("ShardLizard"), 43.f, 44.4f, 8, 3, 1.f, 1.f, 0.f, 1, 4.f);   // lizards on an obsidian slab, low: 1 slot (s%8==5), right
Zone(TEXT("World s1animals, Stormwatch, by the right channel"), 7.815f, 7.839f);
	Scatter(S1ANIMALS("ShardLizard"), 43.f, 44.4f, 8, 3, 1.f, 1.f, 0.f, 1, 4.f);   // lizards on an obsidian slab, low: 1 slot (s%8==5), right
Zone(TEXT("World s1animals, Stormwatch, fire and brimstone, the right"), 4.014f, 4.219f);
	Scatter(S1ANIMALS("GlassIguanas"), 80.f, 86.f, 4, 3, 1.f, 1.4f, 0.06f, 1, 10.f);   // great iguanas on an obsidian outcrop: 2 slots (s%4==1), right
Zone(TEXT("World s1animals, Stormwatch, fire and brimstone, the right"), 4.738f, 5.124f);
	Scatter(S1ANIMALS("GlassIguanas"), 80.f, 86.f, 4, 3, 1.f, 1.4f, 0.06f, 1, 10.f);   // great iguanas on an obsidian outcrop: 2 slots (s%4==1), right
Zone(TEXT("World s1animals, Stormwatch, fire and brimstone, the right, the far row"), 4.014f, 4.219f);
	Scatter(S1ANIMALS("GlassIguanas"), 100.f, 106.f, 4, 3, 1.f, 1.4f, 0.06f, 1, 10.f);   // great iguanas on an obsidian outcrop: 2 slots (s%4==1), right
Zone(TEXT("World s1animals, Stormwatch, fire and brimstone, the right, the far row"), 4.557f, 5.124f);
	Scatter(S1ANIMALS("GlassIguanas"), 100.f, 106.f, 4, 3, 1.f, 1.4f, 0.06f, 1, 10.f);   // great iguanas on an obsidian outcrop: 3 slots (s%4==1), right
Zone(TEXT("World s1animals, Stormwatch, fire and brimstone, the right, the far row"), 4.286f, 4.31f);
	Scatter(S1ANIMALS("GlassIguanas"), 100.f, 106.f, 4, 1, 1.f, 1.4f, 0.06f, 1, 10.f);   // great iguanas on an obsidian outcrop: 1 slot (s%4==3), right
Zone(TEXT("World s1animals, Stormwatch, fire and brimstone, the right, the far row"), 4.648f, 4.853f);
	Scatter(S1ANIMALS("GlassIguanas"), 100.f, 106.f, 4, 1, 1.f, 1.4f, 0.06f, 1, 10.f);   // great iguanas on an obsidian outcrop: 2 slots (s%4==3), right
Zone(TEXT("World s1animals, Stormwatch, the magma run, the left"), 5.281f, 5.486f);
	Scatter(S1ANIMALS("GlassIguanas"), 80.f, 86.f, 4, 3, 1.f, 1.4f, 0.06f, -1, 10.f);   // great iguanas on an obsidian outcrop: 2 slots (s%4==1), left
Zone(TEXT("World s1animals, Stormwatch, the magma run, the left"), 6.186f, 6.391f);
	Scatter(S1ANIMALS("GlassIguanas"), 80.f, 86.f, 4, 3, 1.f, 1.4f, 0.06f, -1, 10.f);   // great iguanas on an obsidian outcrop: 2 slots (s%4==1), left
Zone(TEXT("World s1animals, Stormwatch, the magma run, the left, the far row"), 5.281f, 5.486f);
	Scatter(S1ANIMALS("GlassIguanas"), 100.f, 106.f, 4, 3, 1.f, 1.4f, 0.06f, -1, 10.f);   // great iguanas on an obsidian outcrop: 2 slots (s%4==1), left
Zone(TEXT("World s1animals, Stormwatch, the magma run, the left, the far row"), 6.186f, 6.391f);
	Scatter(S1ANIMALS("GlassIguanas"), 100.f, 106.f, 4, 3, 1.f, 1.4f, 0.06f, -1, 10.f);   // great iguanas on an obsidian outcrop: 2 slots (s%4==1), left
Zone(TEXT("World s1animals, Stormwatch, the magma run, the left"), 5.191f, 5.396f);
	Scatter(S1ANIMALS("GlassIguanas"), 80.f, 86.f, 4, 1, 1.f, 1.4f, 0.06f, -1, 10.f);   // great iguanas on an obsidian outcrop: 2 slots (s%4==3), left
Zone(TEXT("World s1animals, Stormwatch, the magma run, the left"), 5.733f, 5.757f);
	Scatter(S1ANIMALS("GlassIguanas"), 80.f, 86.f, 4, 1, 1.f, 1.4f, 0.06f, -1, 10.f);   // great iguanas on an obsidian outcrop: 1 slot (s%4==3), left
Zone(TEXT("World s1animals, Stormwatch, the magma run, the left, the far row"), 5.191f, 6.3f);
	Scatter(S1ANIMALS("GlassIguanas"), 100.f, 106.f, 4, 1, 1.f, 1.4f, 0.06f, -1, 10.f);   // great iguanas on an obsidian outcrop: 5 slots (s%4==3), left
Zone(TEXT("World s1animals, Stormwatch, moths, the right"), 5.281f, 6.21f);
	Far(S1ANIMALS("MothCloud"), 88.f, 96.f, 20, 3, 1.f, 1.8f, 0.05f, 1, -3.f, 180.f);   // pale moths 7.5 to 13 m over the road, circles of 6 m at the widest: 2 slots (s%20==17), right
Zone(TEXT("World s1animals, Stormwatch, moths, the right"), 5.462f, 6.391f);
	Far(S1ANIMALS("MothCloud"), 104.f, 112.f, 20, 19, 1.f, 1.8f, 0.05f, 1, -3.f, 180.f);   // pale moths 7.5 to 13 m over the road, circles of 6 m at the widest: 2 slots (s%20==1), right
Zone(TEXT("World s1animals, Stormwatch, moths, the left"), 3.924f, 4.31f);
	Far(S1ANIMALS("MothCloud"), 76.f, 84.f, 4, 1, 1.f, 1.8f, 0.05f, -1, -3.f, 180.f);   // pale moths 7.5 to 13 m over the road, circles of 6 m at the widest: 3 slots (s%4==3), left
Zone(TEXT("World s1animals, Stormwatch, moths, the left"), 4.648f, 4.672f);
	Far(S1ANIMALS("MothCloud"), 104.f, 112.f, 4, 1, 1.f, 1.8f, 0.05f, -1, -3.f, 180.f);   // pale moths 7.5 to 13 m over the road, circles of 6 m at the widest: 1 slot (s%4==3), left
Zone(TEXT("World s1animals, Stormwatch, moths, the left"), 4.829f, 4.853f);
	Far(S1ANIMALS("MothCloud"), 76.f, 84.f, 4, 1, 1.f, 1.8f, 0.05f, -1, -3.f, 180.f);   // pale moths 7.5 to 13 m over the road, circles of 6 m at the widest: 1 slot (s%4==3), left
Zone(TEXT("World s1animals, Stormwatch, moths, the left"), 6.457f, 6.481f);
	Far(S1ANIMALS("MothCloud"), 76.f, 84.f, 4, 1, 1.f, 1.8f, 0.05f, -1, -3.f, 180.f);   // pale moths 7.5 to 13 m over the road, circles of 6 m at the widest: 1 slot (s%4==3), left
Zone(TEXT("World s1animals, Stormwatch, moths, the right, the other slots"), 4.105f, 4.129f);
	Far(S1ANIMALS("MothCloud"), 104.f, 112.f, 4, 1, 1.f, 1.8f, 0.05f, 1, -3.f, 180.f);   // pale moths 7.5 to 13 m over the road, circles of 6 m at the widest: 1 slot (s%4==3), right
Zone(TEXT("World s1animals, Stormwatch, moths, the right, the other slots"), 5.191f, 5.577f);
	Far(S1ANIMALS("MothCloud"), 76.f, 84.f, 8, 5, 1.f, 1.8f, 0.05f, 1, -3.f, 180.f);   // pale moths 7.5 to 13 m over the road, circles of 6 m at the widest: 2 slots (s%8==3), right
Zone(TEXT("World s1animals, Stormwatch, moths, the right, the other slots"), 5.372f, 5.757f);
	Far(S1ANIMALS("MothCloud"), 88.f, 96.f, 8, 1, 1.f, 1.8f, 0.05f, 1, -3.f, 180.f);   // pale moths 7.5 to 13 m over the road, circles of 6 m at the widest: 2 slots (s%8==7), right
Zone(TEXT("World s1animals, Stormwatch, moths, the right, the other slots"), 6.276f, 6.3f);
	Far(S1ANIMALS("MothCloud"), 76.f, 84.f, 8, 5, 1.f, 1.8f, 0.05f, 1, -3.f, 180.f);   // pale moths 7.5 to 13 m over the road, circles of 6 m at the widest: 1 slot (s%8==3), right
Zone(TEXT("World s1animals, Stormwatch, moths, the right, the other slots"), 6.457f, 6.481f);
	Far(S1ANIMALS("MothCloud"), 88.f, 96.f, 8, 1, 1.f, 1.8f, 0.05f, 1, -3.f, 180.f);   // pale moths 7.5 to 13 m over the road, circles of 6 m at the widest: 1 slot (s%8==7), right
Zone(TEXT("World s1animals, Stormwatch, moths, the left, the other slots"), 3.833f, 4.038f);
	Far(S1ANIMALS("MothCloud"), 76.f, 84.f, 4, 3, 1.f, 1.8f, 0.05f, -1, -3.f, 180.f);   // pale moths 7.5 to 13 m over the road, circles of 6 m at the widest: 2 slots (s%4==1), left
Zone(TEXT("World s1animals, Stormwatch, moths, the left, the other slots"), 4.195f, 5.124f);
	Far(S1ANIMALS("MothCloud"), 104.f, 112.f, 20, 7, 1.f, 1.8f, 0.05f, -1, -3.f, 180.f);   // pale moths 7.5 to 13 m over the road, circles of 6 m at the widest: 2 slots (s%20==13), left
Zone(TEXT("World s1animals, Stormwatch, moths, the left, the other slots"), 4.557f, 4.762f);
	Far(S1ANIMALS("MothCloud"), 76.f, 84.f, 4, 3, 1.f, 1.8f, 0.05f, -1, -3.f, 180.f);   // pale moths 7.5 to 13 m over the road, circles of 6 m at the widest: 2 slots (s%4==1), left
Zone(TEXT("World s1animals, Slagtown, by the right channel"), 8.538f, 11.096f);
	Scatter(S1ANIMALS("Hens"), 43.f, 44.4f, 8, 3, 1.f, 1.f, 0.f, 1, 4.f);   // a hen house in its run, the dog asleep, low: 8 slots (s%8==5), right
Zone(TEXT("World s1animals, Slagtown, the left"), 8.629f, 9.015f);
	Scatter(S1ANIMALS("PitPonies"), 80.f, 86.f, 4, 1, 1.f, 1.5f, 0.06f, -1, 6.f);   // ponies at a rail and trough: 3 slots (s%4==3), left
Zone(TEXT("World s1animals, Slagtown, the left"), 9.353f, 9.92f);
	Scatter(S1ANIMALS("PitPonies"), 80.f, 86.f, 4, 1, 1.f, 1.5f, 0.06f, -1, 6.f);   // ponies at a rail and trough: 4 slots (s%4==3), left
Zone(TEXT("World s1animals, Slagtown, the left"), 10.257f, 10.824f);
	Scatter(S1ANIMALS("PitPonies"), 80.f, 86.f, 4, 1, 1.f, 1.5f, 0.06f, -1, 6.f);   // ponies at a rail and trough: 4 slots (s%4==3), left
Zone(TEXT("World s1animals, Slagtown, the left, the far row"), 8.629f, 8.653f);
	Scatter(S1ANIMALS("PitPonies"), 100.f, 106.f, 12, 1, 1.f, 1.5f, 0.06f, -1, 6.f);   // ponies at a rail and trough: 1 slot (s%12==11), left
Zone(TEXT("World s1animals, Slagtown, the left, the far row"), 8.81f, 10.462f);
	Scatter(S1ANIMALS("PitPonies"), 100.f, 106.f, 12, 9, 1.f, 1.5f, 0.06f, -1, 6.f);   // ponies at a rail and trough: 4 slots (s%12==3), left
Zone(TEXT("World s1animals, Slagtown, the left, the far row"), 9.715f, 10.824f);
	Scatter(S1ANIMALS("PitPonies"), 100.f, 106.f, 12, 1, 1.f, 1.5f, 0.06f, -1, 6.f);   // ponies at a rail and trough: 3 slots (s%12==11), left
Zone(TEXT("World s1animals, Slagtown, the right, beyond the railway"), 8.357f, 8.924f);
	Scatter(S1ANIMALS("PitPonies"), 88.f, 94.f, 4, 3, 1.f, 1.5f, 0.06f, 1, 6.f);   // ponies at a rail and trough: 4 slots (s%4==1), right
Zone(TEXT("World s1animals, Slagtown, the right, beyond the railway"), 9.262f, 9.829f);
	Scatter(S1ANIMALS("PitPonies"), 88.f, 94.f, 4, 3, 1.f, 1.5f, 0.06f, 1, 6.f);   // ponies at a rail and trough: 4 slots (s%4==1), right
Zone(TEXT("World s1animals, Slagtown, the right, beyond the railway"), 10.167f, 10.734f);
	Scatter(S1ANIMALS("PitPonies"), 88.f, 94.f, 4, 3, 1.f, 1.5f, 0.06f, 1, 6.f);   // ponies at a rail and trough: 4 slots (s%4==1), right
Zone(TEXT("World s1animals, Slagtown, the right, beyond the railway"), 11.072f, 11.096f);
	Scatter(S1ANIMALS("PitPonies"), 88.f, 94.f, 4, 3, 1.f, 1.5f, 0.06f, 1, 6.f);   // ponies at a rail and trough: 1 slot (s%4==1), right
Zone(TEXT("World s1animals, the Cliff Temple, by the right channel"), 11.434f, 12.543f);
	Scatter(S1ANIMALS("KoiBasin"), 43.f, 44.4f, 8, 3, 1.f, 1.f, 0.f, 1, 4.f);   // a stone basin of koi, low: 4 slots (s%8==5), right
Zone(TEXT("World s1animals, the Cliff Temple, by the right channel"), 13.243f, 13.629f);
	Scatter(S1ANIMALS("KoiBasin"), 43.f, 44.4f, 8, 3, 1.f, 1.f, 0.f, 1, 4.f);   // a stone basin of koi, low: 2 slots (s%8==5), right
Zone(TEXT("World s1animals, the Cliff Temple, behind the left channel"), 11.253f, 11.639f);
	Scatter(S1ANIMALS("Cranes"), 60.f, 61.6f, 8, 7, 1.f, 1.f, 0.f, -1, 3.f);   // white cranes on a terrace: 2 slots (s%8==1), left
Zone(TEXT("World s1animals, the Cliff Temple, behind the left channel"), 13.062f, 13.448f);
	Scatter(S1ANIMALS("Cranes"), 60.f, 61.6f, 8, 7, 1.f, 1.f, 0.f, -1, 3.f);   // white cranes on a terrace: 2 slots (s%8==1), left
Zone(TEXT("World s1animals, the Cliff Temple, behind the left channel, koi"), 12.158f, 12.543f);
	Scatter(S1ANIMALS("KoiBasin"), 60.f, 61.6f, 8, 3, 1.f, 1.f, 0.f, -1, 3.f);   // a stone basin of koi: 2 slots (s%8==5), left
Zone(TEXT("World s1animals, the Cliff Temple, behind the right channel"), 11.615f, 13.448f);
	Scatter(S1ANIMALS("Cranes"), 63.8f, 64.6f, 20, 3, 1.f, 1.f, 0.f, 1, 3.f);   // white cranes on a terrace: 3 slots (s%20==17), right
Zone(TEXT("World s1animals, the Cliff Temple, the left"), 12.158f, 12.543f);
	Scatter(S1ANIMALS("PackBeasts"), 80.f, 86.f, 8, 3, 1.f, 1.4f, 0.f, -1, 3.f);   // pack animals and their keeper on a causeway: 2 slots (s%8==5), left
Zone(TEXT("World s1animals, the Cliff Temple, the left"), 11.705f, 12.091f);
	Scatter(S1ANIMALS("PackBeasts"), 80.f, 86.f, 8, 5, 1.f, 1.4f, 0.f, -1, 3.f);   // pack animals and their keeper on a causeway: 2 slots (s%8==3), left
Zone(TEXT("World s1animals, the Cliff Temple, the left"), 12.791f, 12.815f);
	Scatter(S1ANIMALS("PackBeasts"), 72.f, 76.f, 8, 5, 1.f, 1.4f, 0.f, -1, 3.f);   // pack animals and their keeper on a causeway: 1 slot (s%8==3), left   // review round: was 80 to 86 m; it stood in the pass 3 whim at 92.2 m
Zone(TEXT("World s1animals, the Cliff Temple, the left"), 13.515f, 13.9f);
	Scatter(S1ANIMALS("PackBeasts"), 80.f, 86.f, 8, 5, 1.f, 1.4f, 0.f, -1, 3.f);   // pack animals and their keeper on a causeway: 2 slots (s%8==3), left
Zone(TEXT("World s1animals, the Cliff Temple, the right"), 12.067f, 12.453f);
	Scatter(S1ANIMALS("PackBeasts"), 80.f, 86.f, 8, 5, 1.f, 1.4f, 0.f, 1, 3.f);   // pack animals and their keeper on a causeway: 2 slots (s%8==3), right
Zone(TEXT("World s1animals, the Cliff Temple, the right"), 13.877f, 13.9f);
	Scatter(S1ANIMALS("PackBeasts"), 80.f, 86.f, 8, 5, 1.f, 1.4f, 0.f, 1, 3.f);   // pack animals and their keeper on a causeway: 1 slot (s%8==3), right
Zone(TEXT("World s1animals, the Cliff Temple, the skein, the left"), 11.615f, 13.81f);
	Far(S1ANIMALS("CraneSkein"), 155.f, 185.f, 16, 15, 1.f, 1.8f, 0.05f, -1, -38.f, 5.f);   // white cranes in a V along the road, 43 to 46 m up, 126 m each way: 4 slots (s%16==1), left
Zone(TEXT("World s1animals, the Cliff Temple, the skein, the right"), 11.253f, 13.448f);
	Far(S1ANIMALS("CraneSkein"), 155.f, 185.f, 16, 7, 1.f, 1.8f, 0.05f, 1, -38.f, 5.f);   // white cranes in a V along the road, 43 to 46 m up, 126 m each way: 4 slots (s%16==9), right
Zone(TEXT("World s1animals, Windmill Flats, by the right channel"), 13.967f, 13.991f);
	Scatter(S1ANIMALS("Geese"), 43.f, 44.4f, 16, 11, 1.f, 1.f, 0.f, 1, 4.f);   // geese walking a bank with the goose-girl, low: 1 slot (s%16==5), right
Zone(TEXT("World s1animals, Windmill Flats, by the right channel"), 15.415f, 16.163f);
	Scatter(S1ANIMALS("Geese"), 43.f, 44.4f, 16, 11, 1.f, 1.f, 0.f, 1, 4.f);   // geese walking a bank with the goose-girl, low: 2 slots (s%16==5), right
Zone(TEXT("World s1animals, Windmill Flats, by the right channel, hens"), 15.053f, 15.801f);
	Scatter(S1ANIMALS("Hens"), 43.f, 44.4f, 16, 3, 1.f, 1.f, 0.f, 1, 4.f);   // a hen house in its run, low: 2 slots (s%16==13), right
Zone(TEXT("World s1animals, Windmill Flats, the great tortoise"), 14.691f, 14.715f);
	Scatter(S1ANIMALS("GreatTortoise"), 60.f, 61.6f, 4, 3, 1.f, 1.f, 0.f, -1, 3.f);   // THE GREAT TORTOISE behind the left channel: 1 slot (s%4==1), left
Zone(TEXT("World s1animals, Windmill Flats, behind the left channel"), 14.148f, 14.172f);
	Scatter(S1ANIMALS("PinkWaders"), 60.f, 61.6f, 4, 3, 1.f, 1.f, 0.f, -1, 3.f);   // pink waders in a mineral pool: 1 slot (s%4==1), left
Zone(TEXT("World s1animals, Windmill Flats, behind the left channel"), 15.234f, 15.258f);
	Scatter(S1ANIMALS("PinkWaders"), 60.f, 61.6f, 4, 3, 1.f, 1.f, 0.f, -1, 3.f);   // pink waders in a mineral pool: 1 slot (s%4==1), left
Zone(TEXT("World s1animals, Windmill Flats, behind the left channel"), 15.777f, 15.982f);
	Scatter(S1ANIMALS("PinkWaders"), 60.f, 61.6f, 4, 3, 1.f, 1.f, 0.f, -1, 3.f);   // pink waders in a mineral pool: 2 slots (s%4==1), left
Zone(TEXT("World s1animals, Windmill Flats, the left"), 14.691f, 15.439f);
	Scatter(S1ANIMALS("CurlHornSheep"), 80.f, 86.f, 16, 11, 1.f, 1.4f, 0.06f, -1, 6.f);   // curl-horned sheep and their shepherd: 2 slots (s%16==5), left
Zone(TEXT("World s1animals, Windmill Flats, the left"), 15.777f, 16.525f);
	Scatter(S1ANIMALS("CurlHornSheep"), 80.f, 86.f, 16, 3, 1.f, 1.4f, 0.06f, -1, 6.f);   // curl-horned sheep and their shepherd: 2 slots (s%16==13), left
Zone(TEXT("World s1animals, Windmill Flats, the left"), 14.058f, 14.444f);
	Scatter(S1ANIMALS("CurlHornSheep"), 80.f, 86.f, 8, 1, 1.f, 1.4f, 0.06f, -1, 6.f);   // curl-horned sheep and their shepherd: 2 slots (s%8==7), left
Zone(TEXT("World s1animals, Windmill Flats, the left"), 15.143f, 15.167f);
	Scatter(S1ANIMALS("CurlHornSheep"), 80.f, 86.f, 8, 1, 1.f, 1.4f, 0.06f, -1, 6.f);   // curl-horned sheep and their shepherd: 1 slot (s%8==7), left
Zone(TEXT("World s1animals, Windmill Flats, the left"), 16.229f, 16.615f);
	Scatter(S1ANIMALS("CurlHornSheep"), 80.f, 86.f, 8, 1, 1.f, 1.4f, 0.06f, -1, 6.f);   // curl-horned sheep and their shepherd: 2 slots (s%8==7), left
Zone(TEXT("World s1animals, Windmill Flats, the right"), 16.048f, 16.434f);
	Scatter(S1ANIMALS("CurlHornSheep"), 80.f, 86.f, 8, 5, 1.f, 1.4f, 0.06f, 1, 6.f);   // curl-horned sheep and their shepherd: 2 slots (s%8==3), right
Zone(TEXT("World s1animals, Windmill Flats, the right"), 14.148f, 14.172f);
	Scatter(S1ANIMALS("CurlHornSheep"), 80.f, 86.f, 8, 7, 1.f, 1.4f, 0.06f, 1, 6.f);   // curl-horned sheep and their shepherd: 1 slot (s%8==1), right
Zone(TEXT("World s1animals, Windmill Flats, the right"), 15.234f, 15.62f);
	Scatter(S1ANIMALS("CurlHornSheep"), 80.f, 86.f, 8, 7, 1.f, 1.4f, 0.06f, 1, 6.f);   // curl-horned sheep and their shepherd: 2 slots (s%8==1), right
Zone(TEXT("World s1animals, Geotherm, by the right channel"), 16.863f, 17.61f);
	Scatter(S1ANIMALS("Frogs"), 43.f, 44.4f, 16, 11, 1.f, 1.f, 0.f, 1, 4.f);   // frogs on a warm pipe, low: 2 slots (s%16==5), right
Zone(TEXT("World s1animals, Geotherm, by the right channel"), 19.034f, 19.058f);
	Scatter(S1ANIMALS("Frogs"), 43.f, 44.4f, 16, 11, 1.f, 1.f, 0.f, 1, 4.f);   // frogs on a warm pipe, low: 1 slot (s%16==5), right
Zone(TEXT("World s1animals, Geotherm, by the right channel, egrets"), 17.224f, 18.696f);
	Scatter(S1ANIMALS("Egrets"), 43.f, 44.4f, 16, 3, 1.f, 1.f, 0.f, 1, 4.f);   // egrets in a pool, low: 3 slots (s%16==13), right
Zone(TEXT("World s1animals, Geotherm, behind the left channel"), 17.043f, 17.067f);
	Scatter(S1ANIMALS("Cranes"), 60.f, 61.6f, 8, 7, 1.f, 1.f, 0.f, -1, 3.f);   // white cranes on a terrace: 1 slot (s%8==1), left
Zone(TEXT("World s1animals, Geotherm, behind the left channel"), 17.767f, 19.239f);
	Scatter(S1ANIMALS("Cranes"), 60.f, 61.6f, 8, 7, 1.f, 1.f, 0.f, -1, 3.f);   // white cranes on a terrace: 5 slots (s%8==1), left
Zone(TEXT("World s1animals, Geotherm, behind the left channel, egrets"), 16.863f, 17.61f);
	Scatter(S1ANIMALS("Egrets"), 60.f, 61.6f, 16, 11, 1.f, 1.f, 0.f, -1, 3.f);   // egrets in a pool: 2 slots (s%16==5), left
Zone(TEXT("World s1animals, Geotherm, behind the left channel, egrets"), 19.034f, 19.058f);
	Scatter(S1ANIMALS("Egrets"), 60.f, 61.6f, 16, 11, 1.f, 1.f, 0.f, -1, 3.f);   // egrets in a pool: 1 slot (s%16==5), left
Zone(TEXT("World s1animals, Geotherm, behind the left channel, frogs"), 17.224f, 18.696f);
	Scatter(S1ANIMALS("Frogs"), 60.f, 61.6f, 16, 3, 1.f, 1.f, 0.f, -1, 3.f);   // frogs on a warm pipe: 3 slots (s%16==13), left
Zone(TEXT("World s1animals, Geotherm, the skein, the left"), 17.586f, 19.058f);
	Far(S1ANIMALS("CraneSkein"), 155.f, 185.f, 32, 27, 1.f, 1.8f, 0.05f, -1, -38.f, 5.f);   // white cranes in a V along the road, 43 to 46 m up, 126 m each way: 2 slots (s%32==5), left
Zone(TEXT("World s1animals, Geotherm, the skein, the right"), 17.224f, 18.696f);
	Far(S1ANIMALS("CraneSkein"), 155.f, 185.f, 16, 3, 1.f, 1.8f, 0.05f, 1, -38.f, 5.f);   // white cranes in a V along the road, 43 to 46 m up, 126 m each way: 3 slots (s%16==13), right
Zone(TEXT("World s1animals, the Wall Keep, by the right channel"), 19.758f, 21.953f);
	Scatter(S1ANIMALS("Peacocks"), 43.f, 44.4f, 16, 11, 1.f, 1.f, 0.f, 1, 4.f);   // peacocks on a low wall, low: 4 slots (s%16==5), right
Zone(TEXT("World s1animals, the Wall Keep, by the right channel, lizards"), 20.844f, 21.591f);
	Scatter(S1ANIMALS("ShardLizard"), 43.f, 44.4f, 16, 3, 1.f, 1.f, 0.f, 1, 4.f);   // lizards on an obsidian slab, low: 2 slots (s%16==13), right
Zone(TEXT("World s1animals, the Wall Keep, behind the left channel"), 19.939f, 19.963f);
	Scatter(S1ANIMALS("Peacocks"), 60.f, 61.6f, 8, 7, 1.f, 1.f, 0.f, -1, 3.f);   // peacocks on a low wall: 1 slot (s%8==1), left
Zone(TEXT("World s1animals, the Wall Keep, behind the left channel"), 20.663f, 20.687f);
	Scatter(S1ANIMALS("Peacocks"), 60.f, 61.6f, 8, 7, 1.f, 1.f, 0.f, -1, 3.f);   // peacocks on a low wall: 1 slot (s%8==1), left
Zone(TEXT("World s1animals, the Wall Keep, behind the left channel"), 21.748f, 22.134f);
	Scatter(S1ANIMALS("Peacocks"), 60.f, 61.6f, 8, 7, 1.f, 1.f, 0.f, -1, 3.f);   // peacocks on a low wall: 2 slots (s%8==1), left
Zone(TEXT("World s1animals, the Wall Keep, behind the left channel, lizards"), 19.758f, 19.782f);
	Scatter(S1ANIMALS("ShardLizard"), 60.f, 61.6f, 8, 3, 1.f, 1.f, 0.f, -1, 3.f);   // lizards on an obsidian slab: 1 slot (s%8==5), left
Zone(TEXT("World s1animals, the Wall Keep, behind the left channel, lizards"), 20.844f, 21.591f);
	Scatter(S1ANIMALS("ShardLizard"), 60.f, 61.6f, 8, 3, 1.f, 1.f, 0.f, -1, 3.f);   // lizards on an obsidian slab: 3 slots (s%8==5), left
Zone(TEXT("World s1animals, the Wall Keep, behind the right channel"), 19.577f, 20.506f);
	Scatter(S1ANIMALS("Peacocks"), 63.8f, 64.6f, 20, 7, 1.f, 1.f, 0.f, 1, 3.f);   // peacocks on a low wall: 2 slots (s%20==13), right
Zone(TEXT("World s1animals, the Wall Keep, behind the right channel"), 19.758f, 21.591f);
	Scatter(S1ANIMALS("Peacocks"), 63.8f, 64.6f, 20, 3, 1.f, 1.f, 0.f, 1, 3.f);   // peacocks on a low wall: 3 slots (s%20==17), right
Zone(TEXT("World s1animals, the Wall Keep, the left"), 19.758f, 19.782f);
	Scatter(S1ANIMALS("PitPonies"), 80.f, 86.f, 8, 3, 1.f, 1.5f, 0.06f, -1, 6.f);   // ponies at a rail and trough: 1 slot (s%8==5), left
Zone(TEXT("World s1animals, the Wall Keep, the left"), 20.844f, 21.591f);
	Scatter(S1ANIMALS("PitPonies"), 80.f, 86.f, 8, 3, 1.f, 1.5f, 0.06f, -1, 6.f);   // ponies at a rail and trough: 3 slots (s%8==5), left
Zone(TEXT("World s1animals, the Wall Keep, the left"), 20.753f, 21.139f);
	Scatter(S1ANIMALS("PitPonies"), 80.f, 86.f, 8, 5, 1.f, 1.5f, 0.06f, -1, 6.f);   // ponies at a rail and trough: 2 slots (s%8==3), left
Zone(TEXT("World s1animals, the Wall Keep, the left"), 22.201f, 22.225f);
	Scatter(S1ANIMALS("PitPonies"), 80.f, 86.f, 8, 5, 1.f, 1.5f, 0.06f, -1, 6.f);   // ponies at a rail and trough: 1 slot (s%8==3), left
Zone(TEXT("World s1animals, the Wall Keep, iguanas, the left"), 19.848f, 20.234f);
	Scatter(S1ANIMALS("GlassIguanas"), 80.f, 86.f, 8, 1, 1.f, 1.4f, 0.06f, -1, 10.f);   // great iguanas on an obsidian outcrop: 2 slots (s%8==7), left
Zone(TEXT("World s1animals, the Wall Keep, iguanas, the left"), 21.658f, 22.044f);
	Scatter(S1ANIMALS("GlassIguanas"), 80.f, 86.f, 8, 1, 1.f, 1.4f, 0.06f, -1, 10.f);   // great iguanas on an obsidian outcrop: 2 slots (s%8==7), left
Zone(TEXT("World s1animals, the Wall Keep, iguanas, the left"), 19.939f, 19.963f);
	Scatter(S1ANIMALS("GlassIguanas"), 80.f, 86.f, 8, 7, 1.f, 1.4f, 0.06f, -1, 10.f);   // great iguanas on an obsidian outcrop: 1 slot (s%8==1), left
Zone(TEXT("World s1animals, the Wall Keep, iguanas, the left"), 20.663f, 20.687f);
	Scatter(S1ANIMALS("GlassIguanas"), 80.f, 86.f, 8, 7, 1.f, 1.4f, 0.06f, -1, 10.f);   // great iguanas on an obsidian outcrop: 1 slot (s%8==1), left
Zone(TEXT("World s1animals, the Wall Keep, iguanas, the left"), 21.748f, 22.134f);
	Scatter(S1ANIMALS("GlassIguanas"), 80.f, 86.f, 8, 7, 1.f, 1.4f, 0.06f, -1, 10.f);   // great iguanas on an obsidian outcrop: 2 slots (s%8==1), left
Zone(TEXT("World s1animals, the Wall Keep, iguanas, the right"), 20.21f, 20.596f);
	Scatter(S1ANIMALS("GlassIguanas"), 80.f, 86.f, 8, 1, 1.f, 1.4f, 0.06f, 1, 10.f);   // great iguanas on an obsidian outcrop: 2 slots (s%8==7), right
Zone(TEXT("World s1animals, the Wall Keep, iguanas, the right"), 22.02f, 22.044f);
	Scatter(S1ANIMALS("GlassIguanas"), 80.f, 86.f, 8, 1, 1.f, 1.4f, 0.06f, 1, 10.f);   // great iguanas on an obsidian outcrop: 1 slot (s%8==7), right
Zone(TEXT("World s1animals, the Wall Keep, iguanas, the right"), 20.663f, 20.687f);
	Scatter(S1ANIMALS("GlassIguanas"), 80.f, 86.f, 8, 7, 1.f, 1.4f, 0.06f, 1, 10.f);   // great iguanas on an obsidian outcrop: 1 slot (s%8==1), right
Zone(TEXT("World s1animals, the Backlot, by the right channel"), 22.653f, 23.039f);
	Scatter(S1ANIMALS("StudioPets"), 43.f, 44.4f, 8, 3, 1.f, 1.f, 0.f, 1, 4.f);   // the studio cat, a parrot and a little dog, low: 2 slots (s%8==5), right
Zone(TEXT("World s1animals, the Backlot, by the right channel"), 23.739f, 24.849f);
	Scatter(S1ANIMALS("StudioPets"), 43.f, 44.4f, 8, 3, 1.f, 1.f, 0.f, 1, 4.f);   // the studio cat, a parrot and a little dog, low: 4 slots (s%8==5), right
Zone(TEXT("World s1animals, the Backlot, the great tortoise"), 22.472f, 24.668f);
	Scatter(S1ANIMALS("GreatTortoise"), 60.f, 61.6f, 16, 15, 1.f, 1.f, 0.f, -1, 3.f);   // THE GREAT TORTOISE behind the left channel: 4 slots (s%16==1), left
Zone(TEXT("World s1animals, the Backlot, behind the left channel"), 22.653f, 23.039f);
	Scatter(S1ANIMALS("StudioPets"), 60.f, 61.6f, 8, 3, 1.f, 1.f, 0.f, -1, 3.f);   // the studio cat, a parrot and a little dog: 2 slots (s%8==5), left
Zone(TEXT("World s1animals, the Backlot, behind the left channel"), 23.739f, 24.849f);
	Scatter(S1ANIMALS("StudioPets"), 60.f, 61.6f, 8, 3, 1.f, 1.f, 0.f, -1, 3.f);   // the studio cat, a parrot and a little dog: 4 slots (s%8==5), left
Zone(TEXT("World s1animals, the Backlot, behind the right channel"), 22.472f, 24.668f);
	Scatter(S1ANIMALS("StudioPets"), 63.8f, 64.6f, 24, 7, 1.f, 1.f, 0.f, 1, 3.f);   // the studio cat, a parrot and a little dog: 3 slots (s%24==17), right
Zone(TEXT("World s1animals, the Backlot, behind the right channel"), 23.196f, 23.22f);
	Scatter(S1ANIMALS("StudioPets"), 63.8f, 64.6f, 24, 15, 1.f, 1.f, 0.f, 1, 3.f);   // the studio cat, a parrot and a little dog: 1 slot (s%24==9), right
Zone(TEXT("World s1animals, the Backlot, the left"), 22.472f, 22.858f);
	Scatter(S1ANIMALS("WhiteHorse"), 78.f, 84.f, 4, 3, 1.f, 1.2f, 0.f, -1, 180.f);   // the trained white horse trotting its ring: 3 slots (s%4==1), left
Zone(TEXT("World s1animals, the Backlot, the left"), 23.558f, 23.944f);
	Scatter(S1ANIMALS("WhiteHorse"), 78.f, 84.f, 4, 3, 1.f, 1.2f, 0.f, -1, 180.f);   // the trained white horse trotting its ring: 3 slots (s%4==1), left
Zone(TEXT("World s1animals, the Backlot, the left"), 24.463f, 24.487f);
	Scatter(S1ANIMALS("WhiteHorse"), 78.f, 84.f, 4, 3, 1.f, 1.2f, 0.f, -1, 180.f);   // the trained white horse trotting its ring: 1 slot (s%4==1), left
Zone(TEXT("World s1animals, the Backlot, the left"), 24.825f, 24.849f);
	Scatter(S1ANIMALS("WhiteHorse"), 72.f, 76.f, 4, 3, 1.f, 1.2f, 0.f, -1, 180.f);   // the trained white horse trotting its ring: 1 slot (s%4==1), left   // review round: was 78 to 84 m; it stood in the pass 3 ore tubs at 87.6 m
Zone(TEXT("World s1animals, the Backlot, the left"), 22.563f, 23.13f);
	Scatter(S1ANIMALS("WhiteHorse"), 78.f, 84.f, 12, 5, 1.f, 1.2f, 0.f, -1, 180.f);   // the trained white horse trotting its ring: 2 slots (s%12==7), left
Zone(TEXT("World s1animals, the Backlot, the left"), 22.925f, 24.034f);
	Scatter(S1ANIMALS("WhiteHorse"), 78.f, 84.f, 12, 9, 1.f, 1.2f, 0.f, -1, 180.f);   // the trained white horse trotting its ring: 3 slots (s%12==3), left
Zone(TEXT("World s1animals, the Backlot, the left"), 24.191f, 24.758f);
	Scatter(S1ANIMALS("WhiteHorse"), 78.f, 84.f, 12, 5, 1.f, 1.2f, 0.f, -1, 180.f);   // the trained white horse trotting its ring: 2 slots (s%12==7), left
Zone(TEXT("World s1animals, the Backlot, the left"), 24.915f, 24.939f);
	Scatter(S1ANIMALS("WhiteHorse"), 78.f, 84.f, 12, 1, 1.f, 1.2f, 0.f, -1, 180.f);   // the trained white horse trotting its ring: 1 slot (s%12==11), left
Zone(TEXT("World s1animals, the Backlot, the right"), 22.472f, 22.496f);
	Scatter(S1ANIMALS("WhiteHorse"), 78.f, 84.f, 4, 3, 1.f, 1.2f, 0.f, 1, 180.f);   // the trained white horse trotting its ring: 1 slot (s%4==1), right
Zone(TEXT("World s1animals, the Backlot, the right"), 22.834f, 23.22f);
	Scatter(S1ANIMALS("WhiteHorse"), 78.f, 84.f, 4, 3, 1.f, 1.2f, 0.f, 1, 180.f);   // the trained white horse trotting its ring: 3 slots (s%4==1), right
Zone(TEXT("World s1animals, the Backlot, the right"), 23.739f, 24.125f);
	Scatter(S1ANIMALS("WhiteHorse"), 78.f, 84.f, 4, 3, 1.f, 1.2f, 0.f, 1, 180.f);   // the trained white horse trotting its ring: 3 slots (s%4==1), right
Zone(TEXT("World s1animals, the Backlot, the right"), 24.644f, 24.849f);
	Scatter(S1ANIMALS("WhiteHorse"), 78.f, 84.f, 4, 3, 1.f, 1.2f, 0.f, 1, 180.f);   // the trained white horse trotting its ring: 2 slots (s%4==1), right
Zone(TEXT("World s1animals, the Backlot, the right"), 22.744f, 22.949f);
	Scatter(S1ANIMALS("WhiteHorse"), 78.f, 84.f, 4, 1, 1.f, 1.2f, 0.f, 1, 180.f);   // the trained white horse trotting its ring: 2 slots (s%4==3), right
Zone(TEXT("World s1animals, the Backlot, the right"), 23.468f, 23.492f);
	Scatter(S1ANIMALS("WhiteHorse"), 78.f, 84.f, 4, 1, 1.f, 1.2f, 0.f, 1, 180.f);   // the trained white horse trotting its ring: 1 slot (s%4==3), right
Zone(TEXT("World s1animals, the Backlot, the right"), 23.829f, 23.853f);
	Scatter(S1ANIMALS("WhiteHorse"), 78.f, 84.f, 4, 1, 1.f, 1.2f, 0.f, 1, 180.f);   // the trained white horse trotting its ring: 1 slot (s%4==3), right
Zone(TEXT("World s1animals, the Backlot, the right"), 24.553f, 24.758f);
	Scatter(S1ANIMALS("WhiteHorse"), 78.f, 84.f, 4, 1, 1.f, 1.2f, 0.f, 1, 180.f);   // the trained white horse trotting its ring: 2 slots (s%4==3), right
#undef S1ANIMALS
// ---- END s1animals ----
}();
[&]() {	// Stage 1: a block is a function of its own (a lambda), or People3 outgrows what the compiler will optimise (C4883, an error in this build)
// ---- BEGIN s1people (the people pass 3 left out, ft_world_volcano_s1people.py) ----
// 25 meshes, all printed by the one make run (C:/ftwork/logs/L3s1people_make.log: built 25, failed 0). All TALL (tops 5.65 to 11.18 m).
// Each brings what its people stand on (a deck on legs, an open stair, a terrace, a veranda, a scaffold, a cradle, a pipe landing, a wall
// walk on an arch: floors 2.6 to 4.5 m up, 6.2 m on the two scaffolds' top stages), 4.2 to 4.8 m long, 2.2 to 3.6 m deep, people a
// quarter over life size. Three meshes a district, in the district's own dress; one basalt pier (LavaPier) for the left of the bends.
// Steady glow 0.4 on two meshes only (StormGallery's bead, KilnMouth's mouth); the nearest rule that places them is 36.2 m out.
// Nothing blinks, flies or circles; ShiftWalk's three miners walk 4.3 m along their own deck (along the road) and come round again.
// No Lit, no Bank, no Far, no Afloat, no Scatter. NOT COMPILED, NOT SEEN IN THE GAME.
// Slots: s % 8 == 1 only (every 8, phase 7), each rule in a one-slot Zone of its own, so a set stands on the slot and side chosen for it.
// What decided the places (read from both tables with C:/ftwork/jobs/L3s1people/occ.py and table.py; nothing was seen in the game):
//   LEFT, 36.2 m (a piece reaches from 34.4 to 38.0 m): behind the main cast's trestles (28.4 to 28.8 m), the catwalks (25.2 m) and
//     p3b's stages (32.5 m, backs to 34.75 m), short of the left-hand lava channel (38 m). Not where a watch deck or lookout stands 33 to
//     37 m out, not where p3b's samplers stand (their ladles reach 36.4 m), not where three rules of the people table already stand.
//   RIGHT, 43.0 m, 1.15 big (41.0 to 45.1 m): behind p3b's right-hand stages (38.3 to 38.7 m, backs to 40.85 m) and the fumaroles, short
//     of the right-hand channel (46 m). Not where a watch deck (41.8 to 42.4 m) or anything else is written 39.5 to 46.5 m out.
//   A row of 3, 4 or 5 pieces, 6.6 m apart along the road (gaps of about 2 m: the channel shows between them), turned 3 or 4 degrees
//     either way: 5 where the people table has nothing on that side of the slot, 4 where it has one rule, 3 where it has two.
//   BESIDE THE BANKED BENDS (slots 25, 41, 89, 121, 153, 161: the plan's km): right side, the same row of 5 at 43.0 m (fronts 41.0 m:
//     past the 40 m where the land is cut away; every piece is footed 2 m under z = 0). Left side: 40 m and more is the lava channel
//     (38 to 52 m), so two LavaPier (a stack of pale basalt columns footed 4 m down, people on top 4.5 m up) stand IN the channel's near
//     edge, 42.0 m out (40.7 to 43.1 m). That is this builder's own reading of "40 to 45 m, on a tall plinth": the six Pier zones are
//     the ones to take out if lava rafts are seen to pass through the piers (each has "left of a banked bend" in its comment).
//   Left EMPTY by standing rule: the right from 2.4 to 3.95 km (slots 57 to 81: the eruption's side), the left at slot 9 (the curtain of
//     fire), slots 57, 129 and 177 (under the loops: the track leaves Site out). Slot 65 left (the bend at 2.95 km begins in it).
// REVIEW ROUND: the six pier zones (people in the lava channel), the four right-hand rows beside a jump's run-up or landing (slots 17,
// 33, 473, 537) and the left row in the bend at slot 73 are OUT; every row of 5 is now a row of 3 (the two outer pieces at +-13.2 m
// taken out of the n5 sets; the sets keep their names). LavaPier is no longer placed. The numbers below are the first hand-over's.
// Estimate (Site: parts x 0.75): 99 rules, 385 parts (93 rows of 3 to 5 = 373, 6 pier sets of two = 12) x 0.75 = about 289 placed.
// By district: D1 41, D2 41, D3 44, D4 41, D5 53, D6 49, D7 60, D8 44 parts, piers 12. The most of one mesh: 22 places (MarketSteps:
// about 17 placed). The smallest x min of a TALL mesh: 36.2 m (left rows; their fronts 34.4 m).
#define S1PEOPLE(n) TEXT("/Game/World/Volcano/SM_S1PEOPLE_" n)
Set(TEXT("WldS1PEOPLED1n4a"), {
	{ S1PEOPLE("BathDeck"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("TeaTerrace"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("BalconyWatch"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("BathDeck"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED1n4b"), {
	{ S1PEOPLE("TeaTerrace"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("BalconyWatch"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("BathDeck"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("TeaTerrace"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED1n4c"), {
	{ S1PEOPLE("BalconyWatch"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("BathDeck"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("TeaTerrace"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("BalconyWatch"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED1n5b"), {
	{ S1PEOPLE("BalconyWatch"), -6.6f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("BathDeck"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("TeaTerrace"), 6.6f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED1n3c"), {
	{ S1PEOPLE("BalconyWatch"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("BathDeck"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("TeaTerrace"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED1n5a"), {
	{ S1PEOPLE("TeaTerrace"), -6.6f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("BalconyWatch"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("BathDeck"), 6.6f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED1n3b"), {
	{ S1PEOPLE("TeaTerrace"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("BalconyWatch"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("BathDeck"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED2n5a"), {
	{ S1PEOPLE("KilnMouth"), -6.6f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("RiggerFrame"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("StormGallery"), 6.6f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED2n3b"), {
	{ S1PEOPLE("KilnMouth"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("RiggerFrame"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("StormGallery"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED2n3c"), {
	{ S1PEOPLE("RiggerFrame"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("StormGallery"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("KilnMouth"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED2n3a"), {
	{ S1PEOPLE("StormGallery"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("KilnMouth"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("RiggerFrame"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED2n5b"), {
	{ S1PEOPLE("RiggerFrame"), -6.6f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("StormGallery"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("KilnMouth"), 6.6f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED2n4a"), {
	{ S1PEOPLE("StormGallery"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("KilnMouth"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("RiggerFrame"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("StormGallery"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED2n4b"), {
	{ S1PEOPLE("KilnMouth"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("RiggerFrame"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("StormGallery"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("KilnMouth"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED2n5c"), {
	{ S1PEOPLE("StormGallery"), -6.6f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("KilnMouth"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("RiggerFrame"), 6.6f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED3n4a"), {
	{ S1PEOPLE("ShiftWalk"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("CookVeranda"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("CardStair"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("ShiftWalk"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED3n4b"), {
	{ S1PEOPLE("CookVeranda"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("CardStair"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("ShiftWalk"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("CookVeranda"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED3n5c"), {
	{ S1PEOPLE("ShiftWalk"), -6.6f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("CookVeranda"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("CardStair"), 6.6f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED3n3a"), {
	{ S1PEOPLE("ShiftWalk"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("CookVeranda"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("CardStair"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED3n4c"), {
	{ S1PEOPLE("CardStair"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("ShiftWalk"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("CookVeranda"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("CardStair"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED3n3c"), {
	{ S1PEOPLE("CardStair"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("ShiftWalk"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("CookVeranda"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED3n5b"), {
	{ S1PEOPLE("CardStair"), -6.6f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("ShiftWalk"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("CookVeranda"), 6.6f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED4n4b"), {
	{ S1PEOPLE("YokeStair"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("BellTerrace"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("SweepSteps"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("YokeStair"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED4n4c"), {
	{ S1PEOPLE("BellTerrace"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("SweepSteps"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("YokeStair"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("BellTerrace"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED4n4a"), {
	{ S1PEOPLE("SweepSteps"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("YokeStair"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("BellTerrace"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("SweepSteps"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED4n3a"), {
	{ S1PEOPLE("SweepSteps"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("YokeStair"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("BellTerrace"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED4n3b"), {
	{ S1PEOPLE("YokeStair"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("BellTerrace"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("SweepSteps"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED4n3c"), {
	{ S1PEOPLE("BellTerrace"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("SweepSteps"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("YokeStair"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED5n4a"), {
	{ S1PEOPLE("LoadingStage"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("SailMender"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("RibbonStair"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("LoadingStage"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED5n5b"), {
	{ S1PEOPLE("RibbonStair"), -6.6f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("LoadingStage"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("SailMender"), 6.6f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED5n4b"), {
	{ S1PEOPLE("SailMender"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("RibbonStair"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("LoadingStage"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("SailMender"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED5n4c"), {
	{ S1PEOPLE("RibbonStair"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("LoadingStage"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("SailMender"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("RibbonStair"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED5n3a"), {
	{ S1PEOPLE("LoadingStage"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("SailMender"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("RibbonStair"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED5n3b"), {
	{ S1PEOPLE("SailMender"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("RibbonStair"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("LoadingStage"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED6n3c"), {
	{ S1PEOPLE("TeaLanding"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("GaugeDeck"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("ValveCrew"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED6n4a"), {
	{ S1PEOPLE("GaugeDeck"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("ValveCrew"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("TeaLanding"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("GaugeDeck"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED6n4b"), {
	{ S1PEOPLE("ValveCrew"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("TeaLanding"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("GaugeDeck"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("ValveCrew"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED6n4c"), {
	{ S1PEOPLE("TeaLanding"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("GaugeDeck"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("ValveCrew"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("TeaLanding"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED6n5c"), {
	{ S1PEOPLE("GaugeDeck"), -6.6f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("ValveCrew"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("TeaLanding"), 6.6f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED6n5a"), {
	{ S1PEOPLE("ValveCrew"), -6.6f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("TeaLanding"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("GaugeDeck"), 6.6f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED7n3a"), {
	{ S1PEOPLE("HeraldWalk"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("MarketSteps"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("MasonScaffold"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED7n4b"), {
	{ S1PEOPLE("MarketSteps"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("MasonScaffold"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("HeraldWalk"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("MarketSteps"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED7n5c"), {
	{ S1PEOPLE("HeraldWalk"), -6.6f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("MarketSteps"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("MasonScaffold"), 6.6f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED7n5a"), {
	{ S1PEOPLE("MarketSteps"), -6.6f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("MasonScaffold"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("HeraldWalk"), 6.6f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED7n4a"), {
	{ S1PEOPLE("HeraldWalk"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("MarketSteps"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("MasonScaffold"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("HeraldWalk"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED7n3b"), {
	{ S1PEOPLE("MarketSteps"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("MasonScaffold"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("HeraldWalk"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED7n4c"), {
	{ S1PEOPLE("MasonScaffold"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("HeraldWalk"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("MarketSteps"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("MasonScaffold"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED7n3c"), {
	{ S1PEOPLE("MasonScaffold"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("HeraldWalk"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("MarketSteps"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED8n4a"), {
	{ S1PEOPLE("PainterCradle"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("Carpenters"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("ExtrasQueue"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("PainterCradle"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED8n4b"), {
	{ S1PEOPLE("Carpenters"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("ExtrasQueue"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("PainterCradle"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("Carpenters"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED8n4c"), {
	{ S1PEOPLE("ExtrasQueue"), -9.9f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("PainterCradle"), -3.3f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("Carpenters"), 3.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("ExtrasQueue"), 9.9f, 0.0f, 0.f, -4.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED8n3c"), {
	{ S1PEOPLE("ExtrasQueue"), -6.6f, 0.0f, 0.f, -3.0f, 1.f },
	{ S1PEOPLE("PainterCradle"), 0.0f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("Carpenters"), 6.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1PEOPLED8n5a"), {
	{ S1PEOPLE("Carpenters"), -6.6f, 0.0f, 0.f, 4.0f, 1.f },
	{ S1PEOPLE("ExtrasQueue"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1PEOPLE("PainterCradle"), 6.6f, 0.0f, 0.f, -4.0f, 1.f },
});
Zone(TEXT("World s1people, slot 1, Cinder Springs"), 0.0271f, 0.0633f);
	Site(TEXT("WldS1PEOPLED1n4a"), -1, 36.2f, 8, 7, 1.f);    // 0.05 km, Cinder Springs, left, 4 in the set
Zone(TEXT("World s1people, slot 1, Cinder Springs"), 0.0271f, 0.0633f);
	Site(TEXT("WldS1PEOPLED1n4b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 0.05 km, Cinder Springs, right, 4 in the set
Zone(TEXT("World s1people, slot 9, Cinder Springs"), 0.3891f, 0.4253f);
	Site(TEXT("WldS1PEOPLED1n4c"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 0.41 km, Cinder Springs, right, 4 in the set
Zone(TEXT("World s1people, slot 25, Cinder Springs"), 1.1129f, 1.1491f);
	Site(TEXT("WldS1PEOPLED1n5b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 1.13 km, Cinder Springs, right, beside a banked bend, 5 in the set
Zone(TEXT("World s1people, slot 41, Cinder Springs"), 1.8367f, 1.8729f);
	Site(TEXT("WldS1PEOPLED1n5a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 1.85 km, Cinder Springs, right, beside a banked bend, 5 in the set
Zone(TEXT("World s1people, slot 49, Cinder Springs"), 2.1987f, 2.2349f);
	Site(TEXT("WldS1PEOPLED1n3b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 2.22 km, Cinder Springs, right, 3 in the set
Zone(TEXT("World s1people, slot 81, Cinder Springs"), 3.6463f, 3.6825f);
	Site(TEXT("WldS1PEOPLED1n4b"), -1, 36.2f, 8, 7, 1.f);    // 3.66 km, Cinder Springs, left, 4 in the set
Zone(TEXT("World s1people, slot 89, Stormwatch"), 4.0083f, 4.0445f);
	Site(TEXT("WldS1PEOPLED2n5a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 4.03 km, Stormwatch, right, beside a banked bend, 5 in the set
Zone(TEXT("World s1people, slot 97, Stormwatch"), 4.3702f, 4.4064f);
	Site(TEXT("WldS1PEOPLED2n3b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 4.39 km, Stormwatch, right, 3 in the set
Zone(TEXT("World s1people, slot 105, Stormwatch"), 4.7321f, 4.7683f);
	Site(TEXT("WldS1PEOPLED2n3c"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 4.75 km, Stormwatch, right, 3 in the set
Zone(TEXT("World s1people, slot 113, Stormwatch"), 5.0940f, 5.1302f);
	Site(TEXT("WldS1PEOPLED2n3a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 5.11 km, Stormwatch, right, 3 in the set
Zone(TEXT("World s1people, slot 121, Stormwatch"), 5.4559f, 5.4921f);
	Site(TEXT("WldS1PEOPLED2n5b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 5.47 km, Stormwatch, right, beside a banked bend, 5 in the set
Zone(TEXT("World s1people, slot 137, Stormwatch"), 6.1798f, 6.2160f);
	Site(TEXT("WldS1PEOPLED2n4a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 6.20 km, Stormwatch, right, 4 in the set
Zone(TEXT("World s1people, slot 145, Stormwatch"), 6.5417f, 6.5779f);
	Site(TEXT("WldS1PEOPLED2n4a"), -1, 36.2f, 8, 7, 1.f);    // 6.56 km, Stormwatch, left, 4 in the set
Zone(TEXT("World s1people, slot 145, Stormwatch"), 6.5417f, 6.5779f);
	Site(TEXT("WldS1PEOPLED2n4b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 6.56 km, Stormwatch, right, 4 in the set
Zone(TEXT("World s1people, slot 153, Stormwatch"), 6.9036f, 6.9398f);
	Site(TEXT("WldS1PEOPLED2n5c"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 6.92 km, Stormwatch, right, beside a banked bend, 5 in the set
Zone(TEXT("World s1people, slot 161, Stormwatch"), 7.2655f, 7.3017f);
	Site(TEXT("WldS1PEOPLED2n5a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 7.28 km, Stormwatch, right, beside a banked bend, 5 in the set
Zone(TEXT("World s1people, slot 185, Slagtown"), 8.3513f, 8.3875f);
	Site(TEXT("WldS1PEOPLED3n4a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 8.37 km, Slagtown, right, 4 in the set
Zone(TEXT("World s1people, slot 193, Slagtown"), 8.7132f, 8.7494f);
	Site(TEXT("WldS1PEOPLED3n4a"), -1, 36.2f, 8, 7, 1.f);    // 8.73 km, Slagtown, left, 4 in the set
Zone(TEXT("World s1people, slot 193, Slagtown"), 8.7132f, 8.7494f);
	Site(TEXT("WldS1PEOPLED3n4b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 8.73 km, Slagtown, right, 4 in the set
Zone(TEXT("World s1people, slot 201, Slagtown"), 9.0751f, 9.1113f);
	Site(TEXT("WldS1PEOPLED3n5c"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 9.09 km, Slagtown, right, 5 in the set
Zone(TEXT("World s1people, slot 209, Slagtown"), 9.4371f, 9.4733f);
	Site(TEXT("WldS1PEOPLED3n4a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 9.46 km, Slagtown, right, 4 in the set
Zone(TEXT("World s1people, slot 217, Slagtown"), 9.7990f, 9.8352f);
	Site(TEXT("WldS1PEOPLED3n3a"), -1, 36.2f, 8, 7, 1.f);    // 9.82 km, Slagtown, left, 3 in the set
Zone(TEXT("World s1people, slot 217, Slagtown"), 9.7990f, 9.8352f);
	Site(TEXT("WldS1PEOPLED3n4b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 9.82 km, Slagtown, right, 4 in the set
Zone(TEXT("World s1people, slot 225, Slagtown"), 10.1609f, 10.1971f);
	Site(TEXT("WldS1PEOPLED3n4c"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 10.18 km, Slagtown, right, 4 in the set
Zone(TEXT("World s1people, slot 233, Slagtown"), 10.5228f, 10.5590f);
	Site(TEXT("WldS1PEOPLED3n3c"), -1, 36.2f, 8, 7, 1.f);    // 10.54 km, Slagtown, left, 3 in the set
Zone(TEXT("World s1people, slot 233, Slagtown"), 10.5228f, 10.5590f);
	Site(TEXT("WldS1PEOPLED3n4a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 10.54 km, Slagtown, right, 4 in the set
Zone(TEXT("World s1people, slot 241, Slagtown"), 10.8847f, 10.9209f);
	Site(TEXT("WldS1PEOPLED3n5b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 10.90 km, Slagtown, right, 5 in the set
Zone(TEXT("World s1people, slot 249, the Cliff Temple"), 11.2467f, 11.2829f);
	Site(TEXT("WldS1PEOPLED4n4b"), -1, 36.2f, 8, 7, 1.f);    // 11.26 km, the Cliff Temple, left, 4 in the set
Zone(TEXT("World s1people, slot 249, the Cliff Temple"), 11.2467f, 11.2829f);
	Site(TEXT("WldS1PEOPLED4n4c"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 11.26 km, the Cliff Temple, right, 4 in the set
Zone(TEXT("World s1people, slot 257, the Cliff Temple"), 11.6086f, 11.6448f);
	Site(TEXT("WldS1PEOPLED4n4a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 11.63 km, the Cliff Temple, right, 4 in the set
Zone(TEXT("World s1people, slot 265, the Cliff Temple"), 11.9705f, 12.0067f);
	Site(TEXT("WldS1PEOPLED4n4a"), -1, 36.2f, 8, 7, 1.f);    // 11.99 km, the Cliff Temple, left, 4 in the set
Zone(TEXT("World s1people, slot 273, the Cliff Temple"), 12.3324f, 12.3686f);
	Site(TEXT("WldS1PEOPLED4n4c"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 12.35 km, the Cliff Temple, right, 4 in the set
Zone(TEXT("World s1people, slot 281, the Cliff Temple"), 12.6943f, 12.7305f);
	Site(TEXT("WldS1PEOPLED4n4a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 12.71 km, the Cliff Temple, right, 4 in the set
Zone(TEXT("World s1people, slot 289, the Cliff Temple"), 13.0563f, 13.0925f);
	Site(TEXT("WldS1PEOPLED4n3a"), -1, 36.2f, 8, 7, 1.f);    // 13.07 km, the Cliff Temple, left, 3 in the set
Zone(TEXT("World s1people, slot 289, the Cliff Temple"), 13.0563f, 13.0925f);
	Site(TEXT("WldS1PEOPLED4n4b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 13.07 km, the Cliff Temple, right, 4 in the set
Zone(TEXT("World s1people, slot 297, the Cliff Temple"), 13.4182f, 13.4544f);
	Site(TEXT("WldS1PEOPLED4n3b"), -1, 36.2f, 8, 7, 1.f);    // 13.44 km, the Cliff Temple, left, 3 in the set
Zone(TEXT("World s1people, slot 297, the Cliff Temple"), 13.4182f, 13.4544f);
	Site(TEXT("WldS1PEOPLED4n4c"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 13.44 km, the Cliff Temple, right, 4 in the set
Zone(TEXT("World s1people, slot 305, the Cliff Temple"), 13.7801f, 13.8163f);
	Site(TEXT("WldS1PEOPLED4n3c"), -1, 36.2f, 8, 7, 1.f);    // 13.80 km, the Cliff Temple, left, 3 in the set
Zone(TEXT("World s1people, slot 313, Windmill Flats"), 14.1420f, 14.1782f);
	Site(TEXT("WldS1PEOPLED5n4a"), -1, 36.2f, 8, 7, 1.f);    // 14.16 km, Windmill Flats, left, 4 in the set
Zone(TEXT("World s1people, slot 313, Windmill Flats"), 14.1420f, 14.1782f);
	Site(TEXT("WldS1PEOPLED5n5b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 14.16 km, Windmill Flats, right, 5 in the set
Zone(TEXT("World s1people, slot 321, Windmill Flats"), 14.5039f, 14.5401f);
	Site(TEXT("WldS1PEOPLED5n4b"), -1, 36.2f, 8, 7, 1.f);    // 14.52 km, Windmill Flats, left, 4 in the set
Zone(TEXT("World s1people, slot 321, Windmill Flats"), 14.5039f, 14.5401f);
	Site(TEXT("WldS1PEOPLED5n4c"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 14.52 km, Windmill Flats, right, 4 in the set
Zone(TEXT("World s1people, slot 329, Windmill Flats"), 14.8659f, 14.9021f);
	Site(TEXT("WldS1PEOPLED5n4a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 14.88 km, Windmill Flats, right, 4 in the set
Zone(TEXT("World s1people, slot 337, Windmill Flats"), 15.2278f, 15.2640f);
	Site(TEXT("WldS1PEOPLED5n3a"), -1, 36.2f, 8, 7, 1.f);    // 15.25 km, Windmill Flats, left, 3 in the set
Zone(TEXT("World s1people, slot 337, Windmill Flats"), 15.2278f, 15.2640f);
	Site(TEXT("WldS1PEOPLED5n5b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 15.25 km, Windmill Flats, right, 5 in the set
Zone(TEXT("World s1people, slot 345, Windmill Flats"), 15.5897f, 15.6259f);
	Site(TEXT("WldS1PEOPLED5n4b"), -1, 36.2f, 8, 7, 1.f);    // 15.61 km, Windmill Flats, left, 4 in the set
Zone(TEXT("World s1people, slot 353, Windmill Flats"), 15.9516f, 15.9878f);
	Site(TEXT("WldS1PEOPLED5n4a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 15.97 km, Windmill Flats, right, 4 in the set
Zone(TEXT("World s1people, slot 361, Windmill Flats"), 16.3135f, 16.3497f);
	Site(TEXT("WldS1PEOPLED5n4a"), -1, 36.2f, 8, 7, 1.f);    // 16.33 km, Windmill Flats, left, 4 in the set
Zone(TEXT("World s1people, slot 361, Windmill Flats"), 16.3135f, 16.3497f);
	Site(TEXT("WldS1PEOPLED5n5b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 16.33 km, Windmill Flats, right, 5 in the set
Zone(TEXT("World s1people, slot 369, Windmill Flats"), 16.6755f, 16.7117f);
	Site(TEXT("WldS1PEOPLED5n3b"), -1, 36.2f, 8, 7, 1.f);    // 16.69 km, Windmill Flats, left, 3 in the set
Zone(TEXT("World s1people, slot 369, Windmill Flats"), 16.6755f, 16.7117f);
	Site(TEXT("WldS1PEOPLED5n4c"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 16.69 km, Windmill Flats, right, 4 in the set
Zone(TEXT("World s1people, slot 377, Geotherm"), 17.0374f, 17.0736f);
	Site(TEXT("WldS1PEOPLED6n3c"), -1, 36.2f, 8, 7, 1.f);    // 17.06 km, Geotherm, left, 3 in the set
Zone(TEXT("World s1people, slot 377, Geotherm"), 17.0374f, 17.0736f);
	Site(TEXT("WldS1PEOPLED6n4a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 17.06 km, Geotherm, right, 4 in the set
Zone(TEXT("World s1people, slot 385, Geotherm"), 17.3993f, 17.4355f);
	Site(TEXT("WldS1PEOPLED6n4a"), -1, 36.2f, 8, 7, 1.f);    // 17.42 km, Geotherm, left, 4 in the set
Zone(TEXT("World s1people, slot 385, Geotherm"), 17.3993f, 17.4355f);
	Site(TEXT("WldS1PEOPLED6n4b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 17.42 km, Geotherm, right, 4 in the set
Zone(TEXT("World s1people, slot 393, Geotherm"), 17.7612f, 17.7974f);
	Site(TEXT("WldS1PEOPLED6n4b"), -1, 36.2f, 8, 7, 1.f);    // 17.78 km, Geotherm, left, 4 in the set
Zone(TEXT("World s1people, slot 393, Geotherm"), 17.7612f, 17.7974f);
	Site(TEXT("WldS1PEOPLED6n4c"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 17.78 km, Geotherm, right, 4 in the set
Zone(TEXT("World s1people, slot 401, Geotherm"), 18.1231f, 18.1593f);
	Site(TEXT("WldS1PEOPLED6n5c"), -1, 36.2f, 8, 7, 1.f);    // 18.14 km, Geotherm, left, 5 in the set
Zone(TEXT("World s1people, slot 401, Geotherm"), 18.1231f, 18.1593f);
	Site(TEXT("WldS1PEOPLED6n4a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 18.14 km, Geotherm, right, 4 in the set
Zone(TEXT("World s1people, slot 409, Geotherm"), 18.4851f, 18.5213f);
	Site(TEXT("WldS1PEOPLED6n4b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 18.50 km, Geotherm, right, 4 in the set
Zone(TEXT("World s1people, slot 417, Geotherm"), 18.8470f, 18.8832f);
	Site(TEXT("WldS1PEOPLED6n4b"), -1, 36.2f, 8, 7, 1.f);    // 18.87 km, Geotherm, left, 4 in the set
Zone(TEXT("World s1people, slot 417, Geotherm"), 18.8470f, 18.8832f);
	Site(TEXT("WldS1PEOPLED6n4c"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 18.87 km, Geotherm, right, 4 in the set
Zone(TEXT("World s1people, slot 425, Geotherm"), 19.2089f, 19.2451f);
	Site(TEXT("WldS1PEOPLED6n5a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 19.23 km, Geotherm, right, 5 in the set
Zone(TEXT("World s1people, slot 433, the Wall Keep"), 19.5708f, 19.6070f);
	Site(TEXT("WldS1PEOPLED7n3a"), -1, 36.2f, 8, 7, 1.f);    // 19.59 km, the Wall Keep, left, 3 in the set
Zone(TEXT("World s1people, slot 433, the Wall Keep"), 19.5708f, 19.6070f);
	Site(TEXT("WldS1PEOPLED7n4b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 19.59 km, the Wall Keep, right, 4 in the set
Zone(TEXT("World s1people, slot 441, the Wall Keep"), 19.9327f, 19.9689f);
	Site(TEXT("WldS1PEOPLED7n4b"), -1, 36.2f, 8, 7, 1.f);    // 19.95 km, the Wall Keep, left, 4 in the set
Zone(TEXT("World s1people, slot 441, the Wall Keep"), 19.9327f, 19.9689f);
	Site(TEXT("WldS1PEOPLED7n5c"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 19.95 km, the Wall Keep, right, 5 in the set
Zone(TEXT("World s1people, slot 449, the Wall Keep"), 20.2947f, 20.3309f);
	Site(TEXT("WldS1PEOPLED7n5a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 20.31 km, the Wall Keep, right, 5 in the set
Zone(TEXT("World s1people, slot 457, the Wall Keep"), 20.6566f, 20.6928f);
	Site(TEXT("WldS1PEOPLED7n4a"), -1, 36.2f, 8, 7, 1.f);    // 20.67 km, the Wall Keep, left, 4 in the set
Zone(TEXT("World s1people, slot 457, the Wall Keep"), 20.6566f, 20.6928f);
	Site(TEXT("WldS1PEOPLED7n4b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 20.67 km, the Wall Keep, right, 4 in the set
Zone(TEXT("World s1people, slot 465, the Wall Keep"), 21.0185f, 21.0547f);
	Site(TEXT("WldS1PEOPLED7n3b"), -1, 36.2f, 8, 7, 1.f);    // 21.04 km, the Wall Keep, left, 3 in the set
Zone(TEXT("World s1people, slot 465, the Wall Keep"), 21.0185f, 21.0547f);
	Site(TEXT("WldS1PEOPLED7n4c"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 21.04 km, the Wall Keep, right, 4 in the set
Zone(TEXT("World s1people, slot 473, the Wall Keep"), 21.3804f, 21.4166f);
	Site(TEXT("WldS1PEOPLED7n3c"), -1, 36.2f, 8, 7, 1.f);    // 21.40 km, the Wall Keep, left, 3 in the set
Zone(TEXT("World s1people, slot 481, the Wall Keep"), 21.7423f, 21.7785f);
	Site(TEXT("WldS1PEOPLED7n3a"), -1, 36.2f, 8, 7, 1.f);    // 21.76 km, the Wall Keep, left, 3 in the set
Zone(TEXT("World s1people, slot 481, the Wall Keep"), 21.7423f, 21.7785f);
	Site(TEXT("WldS1PEOPLED7n4b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 21.76 km, the Wall Keep, right, 4 in the set
Zone(TEXT("World s1people, slot 489, the Wall Keep"), 22.1043f, 22.1405f);
	Site(TEXT("WldS1PEOPLED7n4b"), -1, 36.2f, 8, 7, 1.f);    // 22.12 km, the Wall Keep, left, 4 in the set
Zone(TEXT("World s1people, slot 489, the Wall Keep"), 22.1043f, 22.1405f);
	Site(TEXT("WldS1PEOPLED7n5c"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 22.12 km, the Wall Keep, right, 5 in the set
Zone(TEXT("World s1people, slot 497, the Backlot"), 22.4662f, 22.5024f);
	Site(TEXT("WldS1PEOPLED8n4a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 22.48 km, the Backlot, right, 4 in the set
Zone(TEXT("World s1people, slot 505, the Backlot"), 22.8281f, 22.8643f);
	Site(TEXT("WldS1PEOPLED8n4a"), -1, 36.2f, 8, 7, 1.f);    // 22.85 km, the Backlot, left, 4 in the set
Zone(TEXT("World s1people, slot 505, the Backlot"), 22.8281f, 22.8643f);
	Site(TEXT("WldS1PEOPLED8n4b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 22.85 km, the Backlot, right, 4 in the set
Zone(TEXT("World s1people, slot 513, the Backlot"), 23.1900f, 23.2262f);
	Site(TEXT("WldS1PEOPLED8n4c"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 23.21 km, the Backlot, right, 4 in the set
Zone(TEXT("World s1people, slot 521, the Backlot"), 23.5519f, 23.5881f);
	Site(TEXT("WldS1PEOPLED8n3c"), -1, 36.2f, 8, 7, 1.f);    // 23.57 km, the Backlot, left, 3 in the set
Zone(TEXT("World s1people, slot 521, the Backlot"), 23.5519f, 23.5881f);
	Site(TEXT("WldS1PEOPLED8n5a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 23.57 km, the Backlot, right, 5 in the set
Zone(TEXT("World s1people, slot 529, the Backlot"), 23.9139f, 23.9501f);
	Site(TEXT("WldS1PEOPLED8n4a"), -1, 36.2f, 8, 7, 1.f);    // 23.93 km, the Backlot, left, 4 in the set
Zone(TEXT("World s1people, slot 529, the Backlot"), 23.9139f, 23.9501f);
	Site(TEXT("WldS1PEOPLED8n4b"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 23.93 km, the Backlot, right, 4 in the set
Zone(TEXT("World s1people, slot 537, the Backlot"), 24.2758f, 24.3120f);
	Site(TEXT("WldS1PEOPLED8n4b"), -1, 36.2f, 8, 7, 1.f);    // 24.29 km, the Backlot, left, 4 in the set
Zone(TEXT("World s1people, slot 545, the Backlot"), 24.6377f, 24.6739f);
	Site(TEXT("WldS1PEOPLED8n4a"), 1, 43.0f, 8, 7, 1.f, 1.15f);    // 24.66 km, the Backlot, right, 4 in the set
#undef S1PEOPLE
// ---- END s1people ----
}();
