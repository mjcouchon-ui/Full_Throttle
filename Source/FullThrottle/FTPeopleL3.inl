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
