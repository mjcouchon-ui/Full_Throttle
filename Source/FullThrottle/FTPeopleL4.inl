// Theme Park (level index 4): the population layer. People, creatures and film crews, placed by rule like the dressing
// (the words and what their numbers mean are in the header of FTTrackDress.cpp; the rule kinds in FTDress.h).
// This table is read after FTDressL4.inl: start with a Zone line, and every rule here comes after every dressing
// rule, so the dressing's own placements never move. A mesh name without a leading slash is looked up in
// /Game/Maps/ThemePark/Dress/; give a full path for the population's own meshes, e.g. TEXT("/Game/People/SM_CrowdCheerA").
// Rules that bind this layer: nothing over 2 m tall nearer than 24 m to a centre line and nothing at all nearer than
// 17 m; nothing lit that blinks nearer than 30 m; nothing that could be taken for a pickup, an enemy or an attack.
//
// THE POPULATION OF THE PARK (first round, 15 Nov 2026). The meshes are made by Content/Python/ft_people_park.py
// (/Game/People/ThemePark/SM_Park...) and the film crew kit (/Game/People/Crew/SM_Crew..._Park).
//
// What decided this table (seen in the game's frames): the road has a metre of barrier and the verge behind it is a
// hoarding 1.8 m high on every other slot, so a person on the ground is hidden from a straight and shows only where a
// bend's bank tips the camera over the barrier. Everything here is therefore RAISED (floats, stilts, stages, a high
// wire, ramps, stands, a scaffold) and stands 24 m out or more; the only low pieces are the crew at their tape, right
// behind the barrier (17.8 m) or between the verge and the stalls (22.9 to 23.2 m).
//
// Where things go, across:   17.8        crew applauding at their tape (low)
//                            21.5-24     crew camera, village, dolly, stunt team (low), in slots without a parade
//                            24.9-27.1   THE PARADE LANE (floats, walkers, the balloon giant, the band's wagon), and
//                                        the crew's camera tower and lights in slots the parade does not use
//                            28-32       a mascot's podium, a cherry picker (the stalls' crowd stands at 26.7 in the
//                                        slots that have a stall row: those slots are left alone)
//                            41-46       the high wire, a queue tower
//                            50-64       the show stage, the dog show, a stand of guests, the pony ring, the queue tower
//                            60-140      doves; 136-152 the open-air theatre (the far crowd)
// Along: a district's sets stand on known slots (FTDressL4.inl), so each rule here names the slots it takes: At(mesh,
// side, from m, to m, every, k) puts the piece where the slot number is k modulo "every".
// No clown anywhere (the boss is MR. CHUCKLES). Nothing here is lit except the theatre's steady lamps at 136 m and beyond.
#define PPL(n) TEXT("/Game/People/ThemePark/SM_Park" n)
#define CREW(n) TEXT("/Game/People/Crew/SM_Crew" n "_Park")
const auto At = [&](const TCHAR* Mesh, int32 Side, float X0, float X1, int32 Every, int32 K, float Chance = 1.f)
{
	Scatter(Mesh, X0, X1, Every, (Every - (K % Every)) % Every, Chance, 1.f, 0.f, Side, 0.f);
};
// the same by a Far rule (it stays through loops, corkscrews and jumps); Drop is the land's depth under the road there
const auto AtFar = [&](const TCHAR* Mesh, int32 Side, float X0, float X1, int32 Every, int32 K, float Drop, float Chance = 1.f, float Scale = 1.f)
{
	Far(Mesh, X0, X1, Every, (Every - (K % Every)) % Every, Chance, Scale, 0.f, Side, Drop, 0.f);
};
const float Lane = 26.0f, Tape = 17.8f, Pod = 29.8f;
// the sky and the far distance, the same everywhere: doves over the stalls, the theatre's crowd beyond the rides
const auto Sky = [&](int32 DoveEvery, int32 CrowdEvery)
{
	Far(PPL("Doves"), 60, 130, DoveEvery, 1, 1.f, 1.f, 0.2f, 0, 1.3f, 180.f);
	AtFar(PPL("FarCrowd"), -1, 138, 150, CrowdEvery, 1, 2.0f, 1.f, 1.5f);
	AtFar(PPL("FarCrowd"), 1, 138, 150, CrowdEvery, 1 + CrowdEvery / 2, 2.0f, 1.f, 1.5f);
};

// ---- 0 to 0.3 km: THE SHOOT AT THE START. The camera tower and the lights on the left, the crane on the right, crew
// applauding at the tape on both sides, the camera, the village and the dolly between the verge and the stalls,
// catering behind.
Zone(TEXT("People: the shoot at the start"), 0.f, 0.30f);
	At(PPL("CrewTower"), -1, 26.6f, 26.6f, 6, 2);
	At(CREW("Crane"), 1, 31, 31, 6, 2);
	At(CREW("ApplauseA"), 1, Tape, Tape, 6, 3);
	At(CREW("ApplauseB"), -1, Tape, Tape, 6, 4);
	At(CREW("ApplauseB"), 1, Tape, Tape, 6, 6);
	At(CREW("Lights"), -1, 26.5f, 26.5f, 6, 5);
	At(CREW("Camera"), 1, 23.f, 23.f, 6, 1);
	At(CREW("Village"), -1, 23.2f, 23.2f, 6, 1);
	At(CREW("Dolly"), 1, 22.9f, 22.9f, 6, 4);
	At(CREW("Catering"), 1, 34, 34, 6, 3);

// ---- 0 to 1.15 km: THE FRONT GATE. The gate's sets stand on slots 1 (mod 3) on both sides: the parade takes slots
// 0 (mod 3), the mascots' podiums, the high wire and the cherry pickers slots 2 (mod 3); stages, stands, the queue
// tower and the pony ring behind, on the odd slots (the bandstands and fountains have the even ones).
Zone(TEXT("People: the front gate"), 0.f, 1.15f);
	At(PPL("Parade_A"), -1, Lane, Lane, 12, 0);  At(PPL("Parade_B"), -1, Lane, Lane, 12, 3);
	At(PPL("Parade_C"), -1, Lane, Lane, 12, 6);  At(PPL("Parade_D"), -1, Lane, Lane, 12, 9);
	At(PPL("Parade_C"), 1, Lane, Lane, 12, 0);   At(PPL("Parade_D"), 1, Lane, Lane, 12, 3);
	At(PPL("Parade_B"), 1, Lane, Lane, 12, 6);   At(PPL("Parade_A"), 1, Lane, Lane, 12, 9);
	At(PPL("Meet_A"), -1, Pod, Pod, 12, 2);      At(PPL("Meet_B"), -1, Pod, Pod, 12, 8);
	At(PPL("Meet_B"), 1, Pod, Pod, 12, 5);       At(PPL("Meet_A"), 1, Pod, Pod, 12, 11);
	At(PPL("HighWire"), -1, 41, 41, 12, 5);      At(PPL("HighWire"), 1, 41, 41, 12, 2);
	At(PPL("CherryPicker"), -1, 28.5f, 28.5f, 12, 11); At(PPL("CherryPicker"), 1, 28.5f, 28.5f, 12, 8);
	At(PPL("Stage_A"), -1, 52, 52, 12, 1);       At(PPL("Stage_A"), 1, 52, 52, 12, 7);
	At(PPL("Stage_B"), -1, 52, 52, 12, 7);       At(PPL("Stage_B"), 1, 52, 52, 12, 1);
	At(PPL("Grandstand"), -1, 58, 58, 12, 11);   At(PPL("Grandstand"), 1, 58, 58, 12, 5);
	At(PPL("QueueTower"), -1, 64, 64, 12, 9);    At(PPL("QueueTower"), 1, 64, 64, 12, 3);
	At(PPL("PonyRing"), -1, 62, 62, 12, 3);      At(PPL("PonyRing"), 1, 62, 62, 12, 9);
	Sky(8, 8);

// ---- 1.15 to 1.84 km: THE MIDWAY. Stalls and their crowd on every slot, so the performers go OVER the crowd: a high
// wire before the stalls, turn about; stages and stands behind the second row. Through the corkscrew (near things are
// left out of it) stands of guests and a camera tower by Far rules.
Zone(TEXT("People: the midway"), 1.15f, 1.84f);
	At(PPL("HighWire"), -1, 27.3f, 27.3f, 2, 0); At(PPL("HighWire"), 1, 27.3f, 27.3f, 2, 1);
	At(PPL("Stage_A"), -1, 57, 57, 6, 0);        At(PPL("Stage_A"), 1, 57, 57, 6, 3);
	At(PPL("Stage_B"), -1, 57, 57, 6, 3);        At(PPL("Stage_B"), 1, 57, 57, 6, 0);
	At(PPL("Grandstand"), -1, 68, 68, 4, 1);     At(PPL("Grandstand"), 1, 68, 68, 4, 3);
	Sky(6, 8);
Zone(TEXT("People: the midway corkscrew"), 1.22f, 1.36f);
	AtFar(PPL("Grandstand"), -1, 51, 52, 2, 0, 1.05f); AtFar(PPL("Grandstand"), 1, 51, 52, 2, 1, 1.05f);
	AtFar(PPL("CrewTower"), 1, 40.6f, 40.6f, 4, 0, 1.0f);
	AtFar(CREW("Crane"), -1, 40.6f, 40.6f, 4, 2, 1.0f);

// ---- 1.84 to 2.22 km: THE BIG JUMP. The shoot's second unit: tower, crane, stunt team and lights at the run-up (the
// track itself keeps the take-off and the landing clear within 40 m), a stand of guests each side beyond 50 m.
Zone(TEXT("People: the big jump"), 1.84f, 2.22f);
	At(PPL("CrewTower"), -1, 26.6f, 26.6f, 2, 1);
	At(CREW("Crane"), 1, 31, 31, 4, 1);
	At(CREW("Stunt"), 1, 22.9f, 22.9f, 4, 3);
	At(CREW("ApplauseA"), -1, Tape, Tape, 3, 0); At(CREW("ApplauseB"), 1, Tape, Tape, 3, 1);
	AtFar(CREW("Lights"), 1, 42, 43, 3, 0, 1.0f);
	AtFar(CREW("Catering"), -1, 42, 43, 4, 2, 1.0f);
	AtFar(PPL("Grandstand"), -1, 52.5f, 54, 2, 0, 1.05f); AtFar(PPL("Grandstand"), 1, 52.5f, 54, 2, 1, 1.05f);
	Sky(4, 4);

// ---- 2.22 to 2.98 km: PIRATE COVE. Docks on slots 3 (mod 4) left and 1 (mod 4) right, skull rocks opposite: the
// parade and the shows take the even slots. Round the loop: a stand on the left, the theatre beyond the palms.
Zone(TEXT("People: pirate cove"), 2.22f, 2.98f);
	At(PPL("Parade_B"), -1, Lane, Lane, 4, 0);   At(PPL("Parade_D"), -1, Lane, Lane, 4, 2);
	At(PPL("Parade_C"), 1, Lane, Lane, 4, 0);    At(PPL("Parade_A"), 1, Lane, Lane, 4, 2);
	At(PPL("Stage_B"), -1, 52, 52, 4, 2);        At(PPL("Stage_B"), 1, 52, 52, 4, 0);
	At(PPL("HighWire"), -1, 44, 44, 4, 0);       At(PPL("HighWire"), 1, 44, 44, 4, 2);
	At(PPL("CherryPicker"), -1, 29, 29, 8, 1);
	At(CREW("ApplauseA"), 1, Tape, Tape, 6, 3);
	Sky(6, 8);
Zone(TEXT("People: the cove's loop"), 2.665f, 2.945f);
	AtFar(PPL("Grandstand"), -1, 46, 49, 2, 0, 1.0f);
	AtFar(PPL("CrewTower"), -1, 68, 69, 3, 0, 1.25f, 1.f, 1.3f);
	AtFar(PPL("Grandstand"), 1, 124, 128, 2, 1, 1.7f, 1.f, 1.5f);

// ---- 2.98 to 3.21 km: THE LIFT HILL. STARRY lines the climb: the queue towers stand behind the stalls.
Zone(TEXT("People: the lift hill"), 2.98f, 3.21f);
	At(PPL("QueueTower"), -1, 46, 46, 2, 1);     At(PPL("QueueTower"), 1, 46, 46, 2, 0);
	At(CREW("ApplauseA"), 1, Tape, Tape, 2, 1);  At(CREW("ApplauseB"), -1, Tape, Tape, 2, 0);
	Sky(4, 4);

// ---- 3.21 to 6.72 km: THE LAGOON. The road is the coaster, 26 m over the valley, and looks DOWN on it: stages and
// stands on the valley floor 62 to 78 m out (by Far rules: they stay under the loops and corkscrews; where a hollow's
// lake reaches them their decks stand over the water), the theatre beyond the lakes, and doves at the rails' own height.
// The left keeps off the carousels' slots (3 mod 4) and the coasters that stand end on (2 mod 3); the right off the
// big tops (even slots) and its own coasters (0 mod 3).
Zone(TEXT("People: the lagoon"), 3.21f, 6.7174f);
	AtFar(PPL("Stage_A"), -1, 62, 70, 12, 0, 1.3f);    AtFar(PPL("Grandstand"), -1, 72, 78, 12, 4, 1.35f, 1.f, 1.3f);
	AtFar(PPL("Stage_B"), -1, 62, 70, 12, 6, 1.3f);    AtFar(PPL("Grandstand"), -1, 72, 78, 12, 10, 1.35f, 1.f, 1.3f);
	AtFar(PPL("Stage_A"), -1, 62, 70, 12, 9, 1.3f);
	AtFar(PPL("Stage_A"), 1, 62, 70, 12, 7, 1.3f);     AtFar(PPL("Grandstand"), 1, 72, 78, 12, 1, 1.35f, 1.f, 1.3f);
	AtFar(PPL("Stage_B"), 1, 62, 70, 12, 11, 1.3f);    AtFar(PPL("Grandstand"), 1, 72, 78, 12, 5, 1.35f, 1.f, 1.3f);
	AtFar(PPL("FarCrowd"), -1, 136, 142, 6, 2, 1.9f, 1.f, 1.5f);
	AtFar(PPL("FarCrowd"), 1, 136, 142, 6, 5, 1.9f, 1.f, 1.5f);
	Far(PPL("Doves"), 46, 90, 8, 0, 1.f, 1.f, 0.2f, 0, -22.f, 180.f);

// ---- 6.72 to 6.91 km: SPLASHDOWN. Off the rails: crew at the tape, a stand each side behind the saucers.
Zone(TEXT("People: splashdown"), 6.7174f, 6.9074f);
	At(CREW("ApplauseA"), -1, Tape, Tape, 2, 0); At(CREW("ApplauseB"), 1, Tape, Tape, 2, 1);
	AtFar(PPL("Grandstand"), -1, 55, 57, 2, 0, 1.08f); AtFar(PPL("Grandstand"), 1, 55, 57, 2, 1, 1.08f);
	Sky(4, 4);

// ---- 6.91 to 8.12 km: ROCKET LAND. Rocket pads and saucers on the even slots: the parade, the high wire and the
// shows take the odd ones. Stands and a camera tower through the corkscrew and round the loop.
Zone(TEXT("People: rocket land"), 6.9074f, 8.1174f);
	At(PPL("Parade_A"), -1, Lane, Lane, 4, 1);   At(PPL("Parade_D"), -1, Lane, Lane, 4, 3);
	At(PPL("Parade_B"), 1, Lane, Lane, 4, 1);    At(PPL("Parade_C"), 1, Lane, Lane, 4, 3);
	At(PPL("HighWire"), -1, 46, 46, 4, 1);       At(PPL("HighWire"), 1, 46, 46, 4, 3);
	At(PPL("Stage_A"), -1, 60, 60, 8, 3);        At(PPL("Stage_A"), 1, 60, 60, 8, 7);
	At(PPL("PonyRing"), -1, 57, 57, 8, 7);       At(PPL("PonyRing"), 1, 57, 57, 8, 3);
	At(PPL("QueueTower"), -1, 63, 63, 8, 5);     At(PPL("QueueTower"), 1, 63, 63, 8, 1);
	At(CREW("ApplauseB"), -1, Tape, Tape, 8, 2); At(CREW("ApplauseA"), 1, Tape, Tape, 8, 6);
	Sky(8, 8);
Zone(TEXT("People: the rocket corkscrew"), 7.33f, 7.52f);
	AtFar(PPL("Grandstand"), -1, 57, 59, 2, 0, 1.08f); AtFar(PPL("Grandstand"), 1, 57, 59, 2, 1, 1.08f);
	AtFar(PPL("CrewTower"), 1, 58, 59, 4, 2, 1.08f, 1.f, 1.3f);
Zone(TEXT("People: the rocket loop"), 7.8404f, 8.1204f);
	AtFar(PPL("Grandstand"), -1, 46, 49, 2, 0, 1.0f);
	AtFar(PPL("Grandstand"), 1, 78, 82, 2, 1, 1.4f, 1.f, 1.3f);
	AtFar(PPL("CrewTower"), -1, 47, 48, 4, 1, 1.0f);

// ---- 8.12 to 8.44 km: THE FUNHOUSE. Courts on slots 2 (mod 3) left and 1 (mod 3) right, STARRY on 0 (mod 3).
Zone(TEXT("People: the funhouse"), 8.1174f, 8.4374f);
	At(PPL("Parade_A"), -1, Lane, Lane, 6, 1);   At(PPL("Parade_B"), -1, Lane, Lane, 6, 4);
	At(PPL("Parade_C"), 1, Lane, Lane, 6, 2);    At(PPL("Parade_D"), 1, Lane, Lane, 6, 5);
	At(PPL("Stage_B"), -1, 54, 54, 6, 0);        At(PPL("Stage_B"), 1, 54, 54, 6, 3);
	At(PPL("Grandstand"), -1, 57, 57, 6, 3);     At(PPL("Grandstand"), 1, 57, 57, 6, 0);
	Sky(6, 6);

// ---- 8.44 to 8.91 km: THE FUNHOUSE JUMP. The second unit again, and a stand of guests each side.
Zone(TEXT("People: the funhouse jump"), 8.4374f, 8.9074f);
	AtFar(PPL("CrewTower"), 1, 42, 43, 3, 0, 1.0f);
	AtFar(CREW("Crane"), -1, 42.5f, 42.5f, 3, 1, 1.0f);
	AtFar(CREW("Lights"), 1, 42, 43, 3, 1, 1.0f);
	At(CREW("Stunt"), -1, 22.9f, 22.9f, 4, 0);
	At(CREW("ApplauseB"), 1, Tape, Tape, 3, 1);  At(CREW("ApplauseA"), -1, Tape, Tape, 3, 2);
	AtFar(PPL("Grandstand"), -1, 52.5f, 54, 2, 0, 1.05f); AtFar(PPL("Grandstand"), 1, 52.5f, 54, 2, 1, 1.05f);
	Sky(4, 5);

// ---- 8.91 to 9.30 km: THE BACK LOT. Where the park is not on show: staff on cherry pickers, the shoot's catering and
// lights, a float and the balloon giant waiting their turn.
Zone(TEXT("People: the back lot"), 8.9074f, 9.2974f);
	At(PPL("CherryPicker"), -1, 29, 29, 3, 2);   At(PPL("CherryPicker"), 1, 29, 29, 3, 0);
	At(CREW("Catering"), -1, 40, 40, 6, 5);      At(CREW("Lights"), 1, 26.5f, 26.5f, 6, 2);
	At(PPL("Parade_C"), -1, Lane, Lane, 6, 4);   At(PPL("Parade_D"), 1, Lane, Lane, 6, 4);
	Sky(6, 9);

// ---- 9.30 km to the end: THE LAST RIDE. Its sets stand on the even slots on the left and the odd ones on the right,
// so the population takes the others, sixteen slots to a round: the four parade pieces, the two podiums, the high
// wire, both stages, a stand, the queue tower and the pony ring on each side. Three in five are kept, so no two
// rounds are alike.
Zone(TEXT("People: the last ride"), 9.2974f, 99.f);
	const float Keep = 0.55f;
	At(PPL("Parade_A"), -1, Lane, Lane, 16, 1, Keep);   At(PPL("Parade_B"), -1, Lane, Lane, 16, 5, Keep);
	At(PPL("Parade_C"), -1, Lane, Lane, 16, 9, Keep);   At(PPL("Parade_D"), -1, Lane, Lane, 16, 13, Keep);
	At(PPL("Meet_A"), -1, Pod, Pod, 16, 3, Keep);       At(PPL("Meet_B"), -1, Pod, Pod, 16, 11, Keep);
	At(PPL("Stage_B"), -1, 52, 52, 16, 3, Keep);        At(PPL("Stage_A"), -1, 52, 52, 16, 15, Keep);
	At(PPL("HighWire"), -1, 42, 42, 16, 7, Keep);       At(PPL("QueueTower"), -1, 64, 64, 16, 7, Keep);
	At(PPL("Grandstand"), -1, 58, 58, 16, 11, Keep);    At(PPL("PonyRing"), -1, 62, 62, 16, 1, Keep);
	At(PPL("CherryPicker"), -1, 28.5f, 28.5f, 32, 15, Keep);
	At(PPL("Parade_C"), 1, Lane, Lane, 16, 0, Keep);    At(PPL("Parade_D"), 1, Lane, Lane, 16, 4, Keep);
	At(PPL("Parade_A"), 1, Lane, Lane, 16, 8, Keep);    At(PPL("Parade_B"), 1, Lane, Lane, 16, 12, Keep);
	At(PPL("Meet_B"), 1, Pod, Pod, 16, 0, Keep);        At(PPL("Meet_A"), 1, Pod, Pod, 16, 8, Keep);
	At(PPL("Stage_A"), 1, 52, 52, 16, 2, Keep);         At(PPL("Stage_B"), 1, 52, 52, 16, 10, Keep);
	At(PPL("HighWire"), 1, 42, 42, 16, 6, Keep);        At(PPL("PonyRing"), 1, 62, 62, 16, 6, Keep);
	At(PPL("Grandstand"), 1, 58, 58, 16, 14, Keep);     At(PPL("QueueTower"), 1, 64, 64, 16, 14, Keep);
	At(PPL("CherryPicker"), 1, 28.5f, 28.5f, 32, 30, Keep);
	// the shoot, thinly along the way: a camera tower every kilometre a side, crew at the tape
	At(PPL("CrewTower"), -1, 26.6f, 26.6f, 48, 7);      At(PPL("CrewTower"), 1, 26.6f, 26.6f, 48, 30);
	At(CREW("ApplauseA"), -1, Tape, Tape, 16, 4);       At(CREW("ApplauseB"), 1, Tape, Tape, 16, 11);
	At(CREW("ApplauseB"), -1, Tape, Tape, 16, 12);      At(CREW("ApplauseA"), 1, Tape, Tape, 16, 3);
	Sky(20, 10);
// ---- 10.9 to 11.23 km: BEFORE THE ARENA'S HILL. The shoot's main unit: towers and lights each side, catering,
// crew at the tape on every slot.
Zone(TEXT("People: before the arena"), 10.9f, 11.23f);
	At(PPL("CrewTower"), -1, 26.6f, 26.6f, 4, 3);       At(CREW("Lights"), 1, 26.5f, 26.5f, 4, 2);
	At(CREW("Catering"), 1, 36, 36, 8, 6);
	At(CREW("ApplauseA"), -1, Tape, Tape, 2, 0);        At(CREW("ApplauseB"), 1, Tape, Tape, 2, 1);
	At(CREW("Camera"), -1, 23.f, 23.f, 4, 2);           At(CREW("Village"), 1, 23.2f, 23.2f, 4, 1);
#undef PPL
#undef CREW

{
// ---- BEGIN far (ft_people_park_far.py) ----
#define FARP(n) TEXT("/Game/People/ThemePark/SM_Far" n)
Zone(TEXT("People far: first look"), 0.f, 99.f);
	Far(FARP("March_A"), 150, 158, 6, 1, 1.f, 2.f, 0.f, 0, 1.95f, 0.f);
	Far(FARP("March_B"), 270, 280, 6, 4, 1.f, 3.f, 0.f, 0, 3.1f, 0.f);
	Far(FARP("Ducks"), 90, 200, 4, 0, 1.f, 2.f, 0.2f, 0, -6.f, 15.f);
	Far(FARP("DuckPond"), 112, 124, 5, 2, 1.f, 1.f, 0.f, 0, 1.65f, 180.f);
	Far(FARP("Queue"), 205, 215, 7, 3, 1.f, 1.6f, 0.f, 0, 2.5f, 20.f);
	Far(FARP("Balloons"), 74, 82, 5, 0, 1.f, 1.3f, 0.f, 0, 1.3f, 10.f);
#undef FARP
// ---- END far ----
}

{
// ---- BEGIN watch (ft_people_park_watch.py) ----
// THE WATCHERS: the park's guests where they would crowd to watch a car chase, on things RAISED over the stalls' roofs
// (/Game/People/ThemePark/SM_Watch...). Where they stand, across:
//   37.5 m   a queue bridge, a food court's terrace or a coaster's station, its people 6 m up, in the slots that have a
//            ROW OF STALLS (the row's back wall is 33.1 m out, its roof 3.2 m up and its sign 4.7 m): the watchers
//            look over the stalls. Under the sky ride's cables on the left (19 m up; its pylons stand where two slots
//            meet, and these pieces are put down by Site, at a slot's middle)
//   43.5 m   the same in Rocket Land, behind its single stalls
//   47-60 m  a look-out tower of three balconies, in slots without a set of the dressing's
//   47-88 m  the show bleachers, by Far rules (they stay through the corkscrews, loops and jumps), 1.15 to 1.45 times
//            their size; in the lagoon, where the road looks down on the valley, they and a station are the watchers
// Along: every rule names its slots (a slot is 45 m; WatchSite and WatchFar put a piece where the slot's number is K
// modulo Every), worked out from the slots the dressing's sets stand on and kept off the main table's parade lane
// where the land allows. Everything here is over 2 m tall and stands 37 m out or more; a steady glow only.
{
#define WAT(n) TEXT("/Game/People/ThemePark/SM_Watch" n)
Set(TEXT("PplWatchBridgeA"),  { { WAT("Bridge_A"),  0, 0, 0, 0, 1 } });
Set(TEXT("PplWatchBridgeB"),  { { WAT("Bridge_B"),  0, 0, 0, 0, 1 } });
Set(TEXT("PplWatchTerrace"),  { { WAT("Terrace"),   0, 0, 0, 0, 1 } });
Set(TEXT("PplWatchPlatform"), { { WAT("Platform"),  0, 0, 0, 0, 1 } });
Set(TEXT("PplWatchTower"),    { { WAT("Tower"),     0, 0, 0, 0, 1 } });
const auto WatchSite = [&](const TCHAR* SetName, int32 Side, float X, int32 Every, int32 K, float Chance = 1.f)
{
	Site(SetName, Side, X, Every, (Every - (K % Every)) % Every, Chance, 1.f, 0.f);
};
const auto WatchFar = [&](const TCHAR* Mesh, int32 Side, float X0, float X1, int32 Every, int32 K, float Drop, float Scale = 1.f, float Chance = 1.f)
{
	Far(Mesh, X0, X1, Every, (Every - (K % Every)) % Every, Chance, Scale, 0.f, Side, Drop, 0.f);
};
const float WDeck = 37.5f;

// ---- 0 to 1.15 km: THE FRONT GATE. Stall rows stand on slots 1 (mod 6) and 10 (mod 12) on the left, 1 (mod 6) and
// 4 (mod 12) on the right: a bridge, a terrace or a station behind each. Towers on two free slots, bleachers far back.
Zone(TEXT("People watch: the front gate"), 0.f, 1.15f);
	WatchSite(TEXT("PplWatchBridgeA"), -1, WDeck, 12, 1);   WatchSite(TEXT("PplWatchTerrace"), -1, WDeck, 12, 7);
	WatchSite(TEXT("PplWatchBridgeB"), -1, WDeck, 12, 10);
	WatchSite(TEXT("PplWatchTerrace"), 1, WDeck, 12, 1);    WatchSite(TEXT("PplWatchBridgeB"), 1, WDeck, 12, 7);
	WatchSite(TEXT("PplWatchPlatform"), 1, WDeck, 12, 4);
	WatchSite(TEXT("PplWatchTower"), -1, 48, 12, 2);        WatchSite(TEXT("PplWatchTower"), 1, 48, 12, 8);
	WatchFar(WAT("Bleachers"), -1, 84, 88, 12, 0, 1.4f, 1.45f);  WatchFar(WAT("Bleachers"), 1, 84, 88, 12, 6, 1.4f, 1.45f);

// ---- 1.15 to 1.84 km: THE MIDWAY. Stalls on every slot: the watchers' decks behind them on every other one (the
// main table's high wire stands before the stalls on the others). Bleachers through the corkscrew.
Zone(TEXT("People watch: the midway"), 1.15f, 1.84f);
	WatchSite(TEXT("PplWatchBridgeA"), -1, WDeck, 8, 1);    WatchSite(TEXT("PplWatchPlatform"), -1, WDeck, 8, 3);
	WatchSite(TEXT("PplWatchBridgeB"), -1, WDeck, 8, 5);    WatchSite(TEXT("PplWatchTerrace"), -1, WDeck, 8, 7);
	WatchSite(TEXT("PplWatchTerrace"), 1, WDeck, 8, 0);     WatchSite(TEXT("PplWatchBridgeA"), 1, WDeck, 8, 2);
	WatchSite(TEXT("PplWatchBridgeB"), 1, WDeck, 8, 4);     WatchSite(TEXT("PplWatchPlatform"), 1, WDeck, 8, 6);
Zone(TEXT("People watch: the midway corkscrew"), 1.22f, 1.36f);
	WatchFar(WAT("Bleachers"), -1, 62, 64, 2, 1, 1.2f, 1.2f);   WatchFar(WAT("Bleachers"), 1, 62, 64, 2, 0, 1.2f, 1.2f);

// ---- 1.84 to 2.22 km: THE BIG JUMP. Single pieces by Far rules: towers and bleachers beyond the second row.
Zone(TEXT("People watch: the big jump"), 1.84f, 2.22f);
	WatchFar(WAT("Tower"), -1, 55, 56, 4, 1, 1.15f);            WatchFar(WAT("Tower"), 1, 55, 56, 4, 2, 1.15f);
	WatchFar(WAT("Bleachers"), -1, 78, 82, 4, 3, 1.35f, 1.4f);  WatchFar(WAT("Bleachers"), 1, 78, 82, 4, 1, 1.35f, 1.4f);

// ---- 2.22 to 2.98 km: PIRATE COVE. Docks on slots 3 (mod 4) left and 1 (mod 4) right: a terrace or a bridge on the
// slot opposite each. Bleachers on the left round the loop.
Zone(TEXT("People watch: pirate cove"), 2.22f, 2.98f);
	WatchSite(TEXT("PplWatchTerrace"), -1, WDeck, 8, 1);    WatchSite(TEXT("PplWatchBridgeB"), -1, WDeck, 8, 5);
	WatchSite(TEXT("PplWatchBridgeA"), 1, WDeck, 8, 3);     WatchSite(TEXT("PplWatchTerrace"), 1, WDeck, 8, 7);
Zone(TEXT("People watch: the cove's loop"), 2.665f, 2.945f);
	WatchFar(WAT("Bleachers"), -1, 58, 60, 2, 1, 1.2f, 1.2f);

// ---- 2.98 to 3.21 km: THE LIFT HILL. A tower each side behind the stalls.
Zone(TEXT("People watch: the lift hill"), 2.98f, 3.21f);
	WatchFar(WAT("Tower"), -1, 55, 56, 4, 0, 1.15f);            WatchFar(WAT("Tower"), 1, 55, 56, 4, 1, 1.15f);

// ---- 3.21 to 6.72 km: THE LAGOON. The road is the coaster and looks down on the valley: bleachers (their steps show
// from above) and a station on the valley's floor, off the bandstands' slots (1 mod 3).
Zone(TEXT("People watch: the lagoon"), 3.21f, 6.7174f);
	WatchFar(WAT("Bleachers"), -1, 47, 50, 6, 3, 1.1f, 1.3f);   WatchFar(WAT("Bleachers"), 1, 47, 50, 6, 0, 1.1f, 1.3f);
	WatchFar(WAT("Platform"), -1, 42, 43, 12, 5, 1.05f);        WatchFar(WAT("Platform"), 1, 42, 43, 12, 2, 1.05f);

// ---- 6.91 to 8.12 km: ROCKET LAND. Pads and saucers on the even slots: the decks on odd ones, behind the single
// stalls (the main table's high wire has the other odd slots). Bleachers through the corkscrew, towers round the loop.
Zone(TEXT("People watch: rocket land"), 6.9074f, 8.1174f);
	WatchSite(TEXT("PplWatchPlatform"), -1, 43.5f, 8, 3);   WatchSite(TEXT("PplWatchBridgeB"), -1, 43.5f, 8, 7);
	WatchSite(TEXT("PplWatchBridgeA"), 1, 43.5f, 8, 1);     WatchSite(TEXT("PplWatchTerrace"), 1, 43.5f, 8, 5);
Zone(TEXT("People watch: the rocket corkscrew"), 7.33f, 7.52f);
	WatchFar(WAT("Bleachers"), -1, 72, 76, 2, 1, 1.3f, 1.4f);   WatchFar(WAT("Bleachers"), 1, 72, 76, 2, 0, 1.3f, 1.4f);
Zone(TEXT("People watch: the rocket loop"), 7.8404f, 8.1204f);
	WatchFar(WAT("Tower"), -1, 58, 60, 3, 0, 1.15f);

// ---- 8.12 to 8.44 km: THE FUNHOUSE. Courts on slots 2 (mod 3) left and 1 (mod 3) right: towers on free slots.
Zone(TEXT("People watch: the funhouse"), 8.1174f, 8.4374f);
	WatchSite(TEXT("PplWatchTower"), -1, 47, 3, 0);         WatchSite(TEXT("PplWatchTower"), 1, 47, 3, 2);

// ---- 8.44 to 8.91 km: THE FUNHOUSE JUMP. Bleachers each side, off the main table's stands.
Zone(TEXT("People watch: the funhouse jump"), 8.4374f, 8.9074f);
	WatchFar(WAT("Bleachers"), -1, 60, 62, 4, 1, 1.2f, 1.15f);  WatchFar(WAT("Bleachers"), 1, 60, 62, 4, 2, 1.2f, 1.15f);

// ---- 9.30 km to the end: THE LAST RIDE. Stall rows on slots 0 and 4 (mod 12) on the left, 1 and 5 (mod 12) on the
// right: the four decks behind them, turn about, four in five kept. A tower every twelfth slot a side (three in five),
// bleachers far back off the facade rows' slots (three in four); the bleachers stay through the circuit's loops.
Zone(TEXT("People watch: the last ride"), 9.2974f, 99.f);
	WatchSite(TEXT("PplWatchBridgeA"), -1, WDeck, 24, 0, 0.8f);   WatchSite(TEXT("PplWatchTerrace"), -1, WDeck, 24, 4, 0.8f);
	WatchSite(TEXT("PplWatchBridgeB"), -1, WDeck, 24, 12, 0.8f);  WatchSite(TEXT("PplWatchPlatform"), -1, WDeck, 24, 16, 0.8f);
	WatchSite(TEXT("PplWatchTerrace"), 1, WDeck, 24, 1, 0.8f);    WatchSite(TEXT("PplWatchBridgeB"), 1, WDeck, 24, 5, 0.8f);
	WatchSite(TEXT("PplWatchPlatform"), 1, WDeck, 24, 13, 0.8f);  WatchSite(TEXT("PplWatchBridgeA"), 1, WDeck, 24, 17, 0.8f);
	WatchSite(TEXT("PplWatchTower"), -1, 48, 12, 5, 0.6f);        WatchSite(TEXT("PplWatchTower"), 1, 48, 12, 6, 0.6f);
	WatchFar(WAT("Bleachers"), -1, 84, 88, 12, 9, 1.4f, 1.45f, 0.75f);
	WatchFar(WAT("Bleachers"), 1, 84, 88, 12, 10, 1.4f, 1.45f, 0.75f);
#undef WAT
}
// ---- END watch ----
}
