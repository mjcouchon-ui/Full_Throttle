// Theme Park (level index 4): the population layer. People, creatures and film crews, placed by rule like the dressing
// (the words and what their numbers mean are in the header of FTTrackDress.cpp; the rule kinds in FTDress.h).
// This table is read after FTDressL4.inl: start with a Zone line, and every rule here comes after every dressing
// rule, so the dressing's own placements never move. A mesh name without a leading slash is looked up in
// /Game/Maps/ThemePark/Dress/; give a full path for the population's own meshes, e.g. TEXT("/Game/People/SM_CrowdCheerA").
// Rules that bind this layer: nothing over 2 m tall nearer than 24 m to a centre line and nothing at all nearer than
// 17 m; nothing lit that blinks nearer than 30 m; nothing that could be taken for a pickup, an enemy or an attack.
//
// THE POPULATION OF THE PARK (second round, 15 Nov 2026, after the review). The meshes are made by
// Content/Python/ft_people_park.py (/Game/People/ThemePark/SM_Park...). The film crew is the shared kit's own scenes
// (ft_people_crew, wardrobe Park) built again by that generator on a rostrum and lit (SM_ParkCrew...): the shared
// meshes are unlit, stand at the barrier where no light is allowed, and read as black shapes in this night level.
//
// What decided this table (seen in the game's frames): the road has a metre of barrier and the verge behind it is a
// hoarding 1.8 m high on every other slot, so a person on the ground is hidden from a straight and shows only where a
// bend's bank tips the camera over the barrier. Everything here is therefore RAISED (floats, stilts, stages, a high
// wire, ramps, stands, a scaffold, a rostrum) and stands 24 m out or more. Nothing of this layer is nearer than 24 m.
// A piece 50 m and more behind the stall rows was not found from the road in 150 frames, so the performers stand in
// the 26 to 35 m band, in the slots the dressing's sets leave free.
//
// Where things go, across:   24.9-27.1   THE PARADE LANE (floats, walkers, the whale inflatable, the band's wagon)
//                            26.4-26.6   the crew on their rostrums (front edge 24.1 m or more), the camera tower
//                            29-35       a mascot's podium, the high wire, the show stages, a stand of guests, a cherry
//                                        picker, the queue tower, the pony ring; the crew's crane, lights and catering
//                            41-46       the high wire and the shows where planets on posts fill the nearer band
//                                        (Rocket Land); over the lagoon the look-out towers, their decks at the rails
//                            46-69       stands and camera towers through the corkscrews and loops (Far rules)
//                            60-130      doves; 118-152 the open-air theatre (the far crowd)
// Along: a district's sets stand on known slots (FTDressL4.inl), so each rule here names the slots it takes: At(mesh,
// side, from m, to m, every, k) puts the piece where the slot number is k modulo "every".
// LIGHT: every park piece carries a steady glow of 0.05 to 0.1 (the doves 0.3) and rows of steady lamps; the nearest
// lit geometry is a rostrum's front edge at 24.4 m; nothing blinks. No clown anywhere (the boss is MR. CHUCKLES), and
// no balloons (he bombs the road with them): the one inflatable is the whale.
// A Far rule's drop is counted from the LAND's own level, as in the dressing table: over the lagoon the land is 26 m
// under the rails, and a drop of 1.2 stands a piece on the valley floor there.
#define PPL(n) TEXT("/Game/People/ThemePark/SM_Park" n)
// Every near piece stands at 1.3 times its built size: the park's own things are toy-big (a 15 m STARRY, a 15 m robot)
// and a life-size cast beside them is lost at 145 mph. The distances below allow for it (nearest edge 24.1 m).
const float Big = 1.3f;
const auto At = [&](const TCHAR* Mesh, int32 Side, float X0, float X1, int32 Every, int32 K, float Chance = 1.f)
{
	Scatter(Mesh, X0, X1, Every, (Every - (K % Every)) % Every, Chance, Big, 0.f, Side, 0.f);
};
// the same by a Far rule (it stays through loops, corkscrews and jumps); Drop is the land's depth under the road there
const auto AtFar = [&](const TCHAR* Mesh, int32 Side, float X0, float X1, int32 Every, int32 K, float Drop, float Chance = 1.f, float Scale = 1.f)
{
	Far(Mesh, X0, X1, Every, (Every - (K % Every)) % Every, Chance, Scale, 0.f, Side, Drop, 0.f);
};
const float Lane = 26.0f, Pod = 29.8f, Ros = 26.9f, Twr = 27.4f;
// the sky and the far distance, the same everywhere: doves over the stalls, the theatre's crowd beyond the rides
const auto Sky = [&](int32 DoveEvery, int32 CrowdEvery)
{
	Far(PPL("Doves"), 60, 130, DoveEvery, 1, 1.f, 1.f, 0.2f, 0, 1.3f, 180.f);
	AtFar(PPL("FarCrowd"), -1, 138, 150, CrowdEvery, 1, 2.0f, 1.f, 1.5f);
	AtFar(PPL("FarCrowd"), 1, 138, 150, CrowdEvery, 1 + CrowdEvery / 2, 2.0f, 1.f, 1.5f);
};

// ---- 0 to 0.27 km: THE SHOOT AT THE START (slots 0 to 5; the gate's sets stand on 1 and 4, candy carts on 2 and 5).
// Crew applauding on their rostrum and the camera tower on the left; the camera on its rostrum under the crane and
// more crew on the right; lights and catering behind the carts.
Zone(TEXT("People: the shoot at the start"), 0.f, 0.27f);
	At(PPL("CrewApplause"), -1, Ros, Ros, 6, 0);     At(PPL("CrewTower"), -1, Twr, Twr, 6, 3);
	At(PPL("CrewCamera"), 1, Ros, Ros, 6, 0);        At(PPL("CrewCrane"), 1, 33.5f, 33.5f, 6, 0);
	At(PPL("CrewApplause"), 1, Ros, Ros, 6, 3);
	At(PPL("CrewLights"), -1, 35, 35, 6, 5);         At(PPL("CrewCatering"), 1, 35, 35, 6, 5);

// ---- 0.27 to 1.15 km: THE FRONT GATE. The gate's sets stand on slots 1 (mod 3) on both sides: the parade takes slots
// 0 (mod 3). Slots 2 (mod 3) have candy carts at 27 to 33 m, so the mascots' podiums, the high wire and the cherry
// pickers stand behind them at 34.5 m; stages, stands, the queue tower and the pony ring further back.
Zone(TEXT("People: the front gate"), 0.27f, 1.15f);
	const float Pod2 = 34.5f;
	At(PPL("Parade_A"), -1, Lane, Lane, 12, 0);  At(PPL("Parade_B"), -1, Lane, Lane, 12, 3);
	At(PPL("Parade_C"), -1, Lane, Lane, 12, 6);  At(PPL("Parade_D"), -1, Lane, Lane, 12, 9);
	At(PPL("Parade_C"), 1, Lane, Lane, 12, 0);   At(PPL("Parade_D"), 1, Lane, Lane, 12, 3);
	At(PPL("Parade_B"), 1, Lane, Lane, 12, 6);   At(PPL("Parade_A"), 1, Lane, Lane, 12, 9);
	At(PPL("Meet_A"), -1, Pod2, Pod2, 12, 2);    At(PPL("Meet_B"), -1, Pod2, Pod2, 12, 8);
	At(PPL("Meet_B"), 1, Pod2, Pod2, 12, 5);     At(PPL("Meet_A"), 1, Pod2, Pod2, 12, 11);
	At(PPL("HighWire"), -1, Pod2, Pod2, 12, 5);  At(PPL("HighWire"), 1, Pod2, Pod2, 12, 2);
	At(PPL("CherryPicker"), -1, Pod2, Pod2, 12, 11); At(PPL("CherryPicker"), 1, Pod2, Pod2, 12, 8);
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
	At(PPL("HighWire"), -1, 28.2f, 28.2f, 2, 0); At(PPL("HighWire"), 1, 28.2f, 28.2f, 2, 1);
	At(PPL("Stage_A"), -1, 57, 57, 6, 0);        At(PPL("Stage_A"), 1, 57, 57, 6, 3);
	At(PPL("Stage_B"), -1, 57, 57, 6, 3);        At(PPL("Stage_B"), 1, 57, 57, 6, 0);
	At(PPL("Grandstand"), -1, 68, 68, 4, 1);     At(PPL("Grandstand"), 1, 68, 68, 4, 3);
	Sky(6, 8);
Zone(TEXT("People: the midway corkscrew"), 1.22f, 1.36f);
	AtFar(PPL("Grandstand"), -1, 51, 52, 2, 0, 1.05f); AtFar(PPL("Grandstand"), 1, 51, 52, 2, 1, 1.05f);
	AtFar(PPL("CrewTower"), 1, 40.6f, 40.6f, 4, 0, 1.0f);
	AtFar(PPL("CrewCrane"), -1, 40.6f, 40.6f, 4, 2, 1.0f);

// ---- 1.84 to 2.22 km: THE BIG JUMP. The shoot's second unit: tower, crane, stunt team and crew on rostrums at the
// run-up (the track itself keeps the take-off and the landing clear within 40 m), lights and catering behind, a stand
// of guests each side beyond 50 m.
Zone(TEXT("People: the big jump"), 1.84f, 2.22f);
	At(PPL("CrewApplause"), -1, Ros, Ros, 4, 0);  At(PPL("CrewTower"), -1, Twr, Twr, 4, 2);
	At(PPL("CrewApplause"), 1, Ros, Ros, 4, 0);   At(PPL("CrewCrane"), 1, 31, 31, 4, 1);
	At(PPL("CrewStunt"), 1, Twr, Twr, 4, 3);      At(PPL("CrewCamera"), -1, Ros, Ros, 4, 1);
	AtFar(PPL("CrewLights"), 1, 42, 43, 3, 0, 1.0f);
	AtFar(PPL("CrewCatering"), -1, 42, 43, 4, 2, 1.0f);
	AtFar(PPL("Grandstand"), -1, 52.5f, 54, 2, 0, 1.05f); AtFar(PPL("Grandstand"), 1, 52.5f, 54, 2, 1, 1.05f);
	Sky(4, 4);

// ---- 2.22 to 2.98 km: PIRATE COVE. Docks (the swinging ships) on slots 3 (mod 4) left and 1 (mod 4) right, skull
// rocks opposite: the parade and the shows take the even slots. The whale goes where the NEXT slot is a skull rock and
// not a swinging ship (it read as part of that ride's frame). Round the loop: a stand on the left, the theatre beyond.
Zone(TEXT("People: pirate cove"), 2.22f, 2.98f);
	At(PPL("Parade_D"), -1, Lane, Lane, 4, 0);   At(PPL("Parade_B"), -1, Lane, Lane, 4, 2);
	At(PPL("Parade_C"), 1, Lane, Lane, 4, 0);    At(PPL("Parade_A"), 1, Lane, Lane, 4, 2);
	At(PPL("Stage_B"), -1, 52, 52, 4, 2);        At(PPL("Stage_B"), 1, 52, 52, 4, 0);
	At(PPL("HighWire"), -1, 44, 44, 4, 0);       At(PPL("HighWire"), 1, 44, 44, 4, 2);
	At(PPL("CherryPicker"), -1, 29, 29, 8, 1);
	Sky(6, 8);
Zone(TEXT("People: the cove's loop"), 2.665f, 2.945f);
	AtFar(PPL("Grandstand"), -1, 46, 49, 2, 0, 1.0f);
	AtFar(PPL("CrewTower"), -1, 68, 69, 3, 0, 1.25f, 1.f, 1.3f);
	AtFar(PPL("Grandstand"), 1, 124, 128, 2, 1, 1.7f, 1.f, 1.5f);

// ---- 2.98 to 3.21 km: THE LIFT HILL. STARRY lines the climb: the queue towers stand behind the stalls.
Zone(TEXT("People: the lift hill"), 2.98f, 3.21f);
	At(PPL("QueueTower"), -1, 46, 46, 2, 1);     At(PPL("QueueTower"), 1, 46, 46, 2, 0);
	Sky(4, 4);

// ---- 3.21 to 6.72 km: THE LAGOON. The road is the coaster, 26 m over the valley, and looks DOWN on it: a stage or a
// stand on the valley floor is a few pixels from up there (none was found in the review's frames). So the people come
// UP to the rails: look-out towers 29 m high, 51 m out (a Far piece keeps 30 m from every road's edge, or the track moves and
// enlarges it), their decks of guests about the height of the rails, on the
// slots the dressing's bandstands (2 mod 3) leave free: 0 (mod 3) on the left, 1 (mod 3) on the right, so there is one
// every 135 m a side and they alternate. They are Far rules: they stay beside the loops and corkscrews, which keep
// everything out to 39.6 m. On the valley floor the theatre at twice its size (48 m wide, 20 m high), 118 to 126 m
// out; doves over the rails (their flock's zero is 22 m above the valley floor: the birds fly 5 to 12 m over the rails).
Zone(TEXT("People: the lagoon"), 3.21f, 6.7174f);
	AtFar(PPL("SkyDeck"), -1, 51, 52, 3, 0, 1.3f, 1.f, 1.15f);
	AtFar(PPL("SkyDeck"), 1, 51, 52, 3, 1, 1.3f, 1.f, 1.15f);
	AtFar(PPL("FarCrowd"), -1, 118, 126, 4, 2, 1.9f, 1.f, 2.0f);
	AtFar(PPL("FarCrowd"), 1, 118, 126, 4, 0, 1.9f, 1.f, 2.0f);
	Far(PPL("Doves"), 46, 90, 6, 0, 1.f, 1.f, 0.2f, 0, -22.f, 180.f);

// ---- 6.72 to 6.91 km: SPLASHDOWN. Off the rails: crew on their rostrums, a stand each side behind the saucers.
Zone(TEXT("People: splashdown"), 6.7174f, 6.9074f);
	At(PPL("CrewApplause"), -1, Ros, Ros, 2, 0); At(PPL("CrewApplause"), 1, Ros, Ros, 2, 1);
	AtFar(PPL("Grandstand"), -1, 55, 57, 2, 0, 1.08f); AtFar(PPL("Grandstand"), 1, 55, 57, 2, 1, 1.08f);
	Sky(4, 4);

// ---- 6.91 to 8.12 km: ROCKET LAND. Rocket pads and saucers on the even slots; planets on posts at 28 to 33 m on the
// odd ones. The parade takes the odd slots' lane, turn about with the shows at 41 to 44 m behind the planets (the high
// wire, a stage, the pony ring, the queue tower); crew on a rostrum on some even slots. Stands and a camera tower
// through the corkscrew and round the loop.
Zone(TEXT("People: rocket land"), 6.9074f, 8.1174f);
	At(PPL("Parade_A"), -1, Lane, Lane, 16, 1);  At(PPL("Parade_D"), -1, Lane, Lane, 16, 5);
	At(PPL("Parade_B"), -1, Lane, Lane, 16, 9);  At(PPL("Parade_C"), -1, Lane, Lane, 16, 13);
	At(PPL("HighWire"), -1, 41, 41, 16, 3);      At(PPL("PonyRing"), -1, 44, 44, 16, 7);
	At(PPL("Stage_A"), -1, 43, 43, 16, 11);      At(PPL("QueueTower"), -1, 43, 43, 16, 15);
	At(PPL("Parade_B"), 1, Lane, Lane, 16, 3);   At(PPL("Parade_C"), 1, Lane, Lane, 16, 7);
	At(PPL("Parade_A"), 1, Lane, Lane, 16, 11);  At(PPL("Parade_D"), 1, Lane, Lane, 16, 15);
	At(PPL("Stage_B"), 1, 43, 43, 16, 1);        At(PPL("QueueTower"), 1, 43, 43, 16, 5);
	At(PPL("HighWire"), 1, 41, 41, 16, 9);       At(PPL("PonyRing"), 1, 44, 44, 16, 13);
	At(PPL("CrewApplause"), -1, Ros, Ros, 8, 2); At(PPL("CrewApplause"), 1, Ros, Ros, 8, 6);
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
	AtFar(PPL("CrewCrane"), -1, 42.5f, 42.5f, 3, 1, 1.0f);
	AtFar(PPL("CrewLights"), 1, 42, 43, 3, 1, 1.0f);
	At(PPL("CrewStunt"), -1, Twr, Twr, 4, 0);
	At(PPL("CrewApplause"), 1, Ros, Ros, 3, 1);  At(PPL("CrewApplause"), -1, Ros, Ros, 4, 2);
	AtFar(PPL("Grandstand"), -1, 52.5f, 54, 2, 0, 1.05f); AtFar(PPL("Grandstand"), 1, 52.5f, 54, 2, 1, 1.05f);
	Sky(4, 5);

// ---- 8.91 to 9.30 km: THE BACK LOT. Where the park is not on show: staff on cherry pickers, the shoot's catering and
// lights, a float and the whale waiting their turn.
Zone(TEXT("People: the back lot"), 8.9074f, 9.2974f);
	At(PPL("CherryPicker"), -1, 29, 29, 3, 2);   At(PPL("CherryPicker"), 1, 29, 29, 3, 0);
	At(PPL("CrewCatering"), -1, 40, 40, 6, 5);   At(PPL("CrewLights"), 1, 26.5f, 26.5f, 6, 2);
	At(PPL("Parade_C"), -1, Lane, Lane, 6, 4);   At(PPL("Parade_D"), 1, Lane, Lane, 6, 4);
	Sky(6, 9);

// ---- 9.30 km to the end: THE LAST RIDE (two thirds of the level, and the boss fight). Its sets stand on the even
// slots on the left and the odd ones on the right; the population takes EVERY other slot, by slot number and not by
// dice, so there is a piece every 90 m on each side and the two sides alternate: something of the cast every 45 m.
// Left: STARRY stands at 28 to 34 m on slots 1 (mod 4), so the parade's lane takes those and the big pieces (29 to
// 32 m) take 3 (mod 4). Right: planets on posts on 2 (mod 4), so the lane takes those and the big pieces 0 (mod 4).
// The big pieces go round in 32 or 64 slots: the mascots' podiums, the high wire, both stages, a stand of guests, a
// cherry picker, the queue tower, the pony ring, and the shoot (crew on a rostrum, a camera tower).
const auto LastRide = [&]()
{
	At(PPL("Parade_A"), -1, Lane, Lane, 16, 1);         At(PPL("Parade_B"), -1, Lane, Lane, 16, 5);
	At(PPL("Parade_C"), -1, Lane, Lane, 16, 9);         At(PPL("Parade_D"), -1, Lane, Lane, 16, 13);
	At(PPL("Meet_A"), -1, Pod, Pod, 32, 3);             At(PPL("Meet_B"), -1, Pod, Pod, 32, 19);
	At(PPL("HighWire"), -1, 29, 29, 32, 7);             At(PPL("Grandstand"), -1, 30, 30, 64, 23);
	At(PPL("CrewTower"), -1, Twr, Twr, 64, 55);
	At(PPL("Stage_A"), -1, 31, 31, 32, 11);             At(PPL("Stage_B"), -1, 31, 31, 32, 27);
	At(PPL("CherryPicker"), -1, 29, 29, 64, 15);        At(PPL("CrewApplause"), -1, Ros, Ros, 64, 31);
	At(PPL("QueueTower"), -1, 32, 32, 64, 47);          At(PPL("PonyRing"), -1, 31.5f, 31.5f, 64, 63);
	At(PPL("Parade_C"), 1, Lane, Lane, 16, 2);          At(PPL("Parade_D"), 1, Lane, Lane, 16, 6);
	At(PPL("Parade_A"), 1, Lane, Lane, 16, 10);         At(PPL("Parade_B"), 1, Lane, Lane, 16, 14);
	At(PPL("Stage_B"), 1, 31, 31, 32, 0);               At(PPL("Stage_A"), 1, 31, 31, 32, 16);
	At(PPL("Meet_B"), 1, Pod, Pod, 32, 4);              At(PPL("Meet_A"), 1, Pod, Pod, 32, 20);
	At(PPL("Grandstand"), 1, 30, 30, 64, 8);            At(PPL("CrewApplause"), 1, Ros, Ros, 64, 40);
	At(PPL("HighWire"), 1, 29, 29, 32, 24);
	At(PPL("PonyRing"), 1, 32, 32, 64, 12);             At(PPL("CherryPicker"), 1, 29, 29, 64, 28);
	At(PPL("CrewTower"), 1, Twr, Twr, 64, 44);          At(PPL("QueueTower"), 1, 32, 32, 64, 60);
	Sky(24, 10);
};
Zone(TEXT("People: the last ride"), 9.2974f, 10.9f);
	LastRide();
// ---- 10.9 to 11.23 km: BEFORE THE ARENA'S HILL. The shoot's main unit, in the Last Ride's own free slots (odd on the
// left, even on the right): crew on rostrums, the camera, the village, a tower, the crane, lights and catering.
Zone(TEXT("People: before the arena"), 10.9f, 11.23f);
	At(PPL("CrewApplause"), -1, Ros, Ros, 4, 1);        At(PPL("CrewTower"), -1, Twr, Twr, 8, 3);
	At(PPL("CrewVillage"), -1, Ros, Ros, 8, 7);         At(PPL("CrewLights"), -1, 34, 34, 4, 3);
	At(PPL("CrewApplause"), 1, Ros, Ros, 4, 2);         At(PPL("CrewCamera"), 1, Ros, Ros, 8, 0);
	At(PPL("CrewCrane"), 1, 33.5f, 33.5f, 8, 0);        At(PPL("CrewStunt"), 1, Twr, Twr, 8, 4);
	At(PPL("CrewCatering"), 1, 36, 36, 8, 6);
	Sky(4, 4);
Zone(TEXT("People: the last ride, after the hill"), 11.23f, 99.f);
	LastRide();
#undef PPL

{
// ---- BEGIN far (ft_people_park_far.py) ----
// The park's far life and creatures (the extra worker's block; meshes /Game/People/ThemePark/SM_Far...). What decided it,
// seen in the game's frames: the park is so densely built that nothing on the ground past the stalls shows from a level
// road, so everything here is BIG and HIGH and placed by Far rules at twice its built size or so: the marchers of a
// parade viaduct 150 m out show over the roofs, a helter-skelter queue 210 m out stands with the wheels, ducks fly in
// the sky, a balloon seller's bunch stands over the stalls at 75 to 82 m. From the lagoon's coaster the road looks DOWN
// on all of it: the duck ponds are there (and at the jumps), nowhere else. Every piece has a deep foot, because the
// lagoon's land falls away under a Far piece. Nothing here is nearer than 75 m. FarAt(mesh, side, from m, to m, every,
// k, drop m, scale, yaw jitter): the slots whose number is k modulo "every".
#define FARP(n) TEXT("/Game/People/ThemePark/SM_Far" n)
const auto FarAt = [&](const TCHAR* Mesh, int32 Side, float X0, float X1, int32 Every, int32 K, float Drop, float Scale, float YawJ = 0.f)
{
	Far(Mesh, X0, X1, Every, (Every - (K % Every)) % Every, 1.f, Scale, 0.f, Side, Drop, YawJ);
};
// a district's far life: how often (in slots) a parade viaduct, a skein of ducks, a queue tower and a balloon seller come
// round on a side; DuckDrop lifts the skein (negative = higher). The two sides are half a round apart.
const auto FarLife = [&](int32 March, int32 Ducks, int32 Queue, int32 Balloons, float DuckDrop)
{
	FarAt(FARP("March_A"), -1, 150, 160, March * 2, 1, 2.0f, 2.f);
	FarAt(FARP("March_B"), -1, 150, 160, March * 2, 1 + March, 2.0f, 2.f);
	FarAt(FARP("March_B"), 1, 150, 160, March * 2, 1 + March / 2, 2.0f, 2.f);
	FarAt(FARP("March_A"), 1, 150, 160, March * 2, 1 + March + March / 2, 2.0f, 2.f);
	FarAt(FARP("Ducks"), -1, 80, 150, Ducks, 3, DuckDrop, 2.f, 0.f);   // no yaw dice (review of pass 3): the skein flies 360 m along its own X, and turned 15 degrees that line came over the road
	FarAt(FARP("Ducks"), 1, 80, 150, Ducks, 3 + Ducks / 2, DuckDrop, 2.f, 0.f);
	FarAt(FARP("Queue"), -1, 205, 215, Queue, 5, 2.5f, 2.f, 20.f);
	FarAt(FARP("Queue"), 1, 205, 215, Queue, 5 + Queue / 2, 2.5f, 2.f, 20.f);
	FarAt(FARP("Balloons"), -1, 75, 82, Balloons, 2, 1.3f, 1.3f, 10.f);
	FarAt(FARP("Balloons"), 1, 75, 82, Balloons, 2 + Balloons / 2, 1.3f, 1.3f, 10.f);
};
// ---- 0 to 1.84 km: the front gate and the midway. Opening night: the parades are out, a balloon seller every 450 m.
Zone(TEXT("People far: the gate and the midway"), 0.f, 1.84f);
	FarLife(6, 14, 20, 10, -6.f);
// ---- 1.84 to 3.21 km: the big jump, pirate cove, the lift hill. The air belongs to the ducks here (the flight looks
// down on their ponds beside the jump's river), queue towers behind the cove.
Zone(TEXT("People far: the jump, the cove, the lift hill"), 1.84f, 3.21f);
	FarLife(16, 8, 10, 14, -6.f);
	FarAt(FARP("DuckPond"), -1, 112, 124, 10, 4, 1.7f, 1.2f, 180.f);
	FarAt(FARP("DuckPond"), 1, 112, 124, 10, 9, 1.7f, 1.2f, 180.f);
// ---- 3.21 to 6.72 km: the lagoon. The coaster looks down on the valley: duck ponds on its lawns, the skeins at the
// rails' own height, a parade viaduct and a queue tower now and then beyond the lakes.
Zone(TEXT("People far: the lagoon"), 3.21f, 6.7174f);
	FarLife(20, 12, 26, 26, -16.f);
	FarAt(FARP("DuckPond"), -1, 94, 106, 12, 7, 1.5f, 1.2f, 180.f);
	FarAt(FARP("DuckPond"), 1, 94, 106, 12, 1, 1.5f, 1.2f, 180.f);
// ---- 6.72 to 9.30 km: splashdown, rocket land, the funhouse and its jump, the back lot. Queue towers with the rockets.
Zone(TEXT("People far: rocket land to the back lot"), 6.7174f, 9.2974f);
	FarLife(14, 16, 14, 16, -6.f);
	FarAt(FARP("DuckPond"), -1, 112, 124, 28, 11, 1.7f, 1.2f, 180.f);
	FarAt(FARP("DuckPond"), 1, 112, 124, 28, 25, 1.7f, 1.2f, 180.f);
// ---- 9.30 km to the end: the last ride. Everything comes round again, thinly: 16 km of it.
Zone(TEXT("People far: the last ride"), 9.2974f, 99.f);
	FarLife(32, 40, 50, 40, -6.f);
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

{
// ---- BEGIN p3a (gaps, ft_people_park_p3a.py) ----
// THE GAPS of the Theme Park: the park's general cast (at work, at rest, passing through) where the tables above put
// nobody. Meshes /Game/People/ThemePark/SM_P3A_... (Content/Python/ft_people_park_p3a.py). Slots are the track's own
// (45.243 m; a rule takes the slots whose number is K modulo Every: P3ASite / P3AFar turn K into the phase). Every
// near piece is put down by a Site of a one-piece Set, so it stands at its slot's middle (a Scatter slides a piece up
// to 11 m along the road, into the next slot's furniture).
//
// What was free, and what stands there now:
//   21 m       THE VERGE. The dressing's hoarding, queue rails and benches (19 m) take two slots in three of the Last
//              Ride and every slot elsewhere; the lift hill, splashdown and the funhouse jump have no verge at all.
//              In the free slots (Last Ride: left 1 mod 3, right 0 mod 3) and on those three hills: the four LOW
//              pieces SM_P3A_Kerb_A..D (top under 2 m as built, unlit, at their built size: 19.9 to 22.1 m out, clear
//              of the parade lane at 24.9 m). The Last Ride's verge is written as ten stretches that leave out the
//              arena's hill and 60 m and more either side of each of its sixteen banked bends (the cut bank).
//   29-29.5 m  the slots whose only tenant is a ride of the dressing's, with nobody in them: before a pirate dock the
//              photo spot (SM_P3A_Photo), before a funhouse court the prize delivery (SM_P3A_Prizes). Their front
//              edges are 25.4 m and 27.3 m out.
//   41-49 m    behind the parade lane, where the first table has only a float at 26 m: a cafe terrace, the painters'
//              scaffold and the miniature railway on its trestle (SM_P3A_Terrace, _Painters, _Train), 1.2 times
//              their size, turn about by slot so that no two neighbours are alike.
//   47.5-52 m  THE LAGOON: the rail gang's tower (SM_P3A_RailGang, 25 m high, on the valley floor like the look-out
//              towers, its deck at the rails) in the slots 2 (mod 3) that the look-outs and the bandstands leave free,
//              by Far rules (they stay beside the loops and corkscrews). Left at 47.5 m, before the coasters that stand
//              end on from 52 m; right at 51 m. Off the watchers' stations (left 5, right 2 modulo 12).
// Light: the six tall pieces carry the park cast's steady glow (0.09) and rows of steady lamps, the nearest lit edge
// 25.4 m out; the four low ones have no glow at all. Nothing blinks. No clown, no balloon.
// Estimated placed (rules counted by slots, before the track leaves pieces out of jumps, loops and corkscrews):
//   Kerb_A..D 51 each (204), Terrace 44, Painters 48, Train 49, Photo 51, Prizes 52, RailGang 39: about 487.
#define P3A(n) TEXT("/Game/People/ThemePark/SM_P3A_" n)
Set(TEXT("PplP3AKerbA"),    { { P3A("Kerb_A"),   0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3AKerbB"),    { { P3A("Kerb_B"),   0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3AKerbC"),    { { P3A("Kerb_C"),   0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3AKerbD"),    { { P3A("Kerb_D"),   0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3ATerrace"),  { { P3A("Terrace"),  0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3APainters"), { { P3A("Painters"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3ATrain"),    { { P3A("Train"),    0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3APhoto"),    { { P3A("Photo"),    0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3APrizes"),   { { P3A("Prizes"),   0.f, 0.f, 0.f, 0.f, 1.f } });
const auto P3ASite = [&](const TCHAR* SetName, int32 Side, float X, int32 Every, int32 K, float Chance = 1.f, float Scale = 1.f)
{
	Site(SetName, Side, X, Every, (Every - (K % Every)) % Every, Chance, Scale, 0.f);
};
const auto P3AFar = [&](const TCHAR* Mesh, int32 Side, float X0, float X1, int32 Every, int32 K, float Drop, float Scale)
{
	Far(Mesh, X0, X1, Every, (Every - (K % Every)) % Every, 1.f, Scale, 0.f, Side, Drop, 0.f);
};
const float P3AKerbX = 21.f, P3ABig = 1.2f, P3AMid = 1.15f;
// the verge of a hill that has no verge furniture: a low piece in every slot on both sides, the four turn about
const auto P3AHill = [&]()
{
	P3ASite(TEXT("PplP3AKerbA"), -1, P3AKerbX, 4, 0);   P3ASite(TEXT("PplP3AKerbB"), -1, P3AKerbX, 4, 1);
	P3ASite(TEXT("PplP3AKerbC"), -1, P3AKerbX, 4, 2);   P3ASite(TEXT("PplP3AKerbD"), -1, P3AKerbX, 4, 3);
	P3ASite(TEXT("PplP3AKerbC"), 1, P3AKerbX, 4, 0);    P3ASite(TEXT("PplP3AKerbD"), 1, P3AKerbX, 4, 1);
	P3ASite(TEXT("PplP3AKerbA"), 1, P3AKerbX, 4, 2);    P3ASite(TEXT("PplP3AKerbB"), 1, P3AKerbX, 4, 3);
};

// ---- 0.27 to 1.15 km: THE FRONT GATE (slots 6 to 25). Behind the parade lane and the ponds, before the bandstands
// and fountains (52 m and out): left on slots 6, 8, 9 and 11 (mod 12), right on 0, 2, 3 and 6. Fourteen pieces.
Zone(TEXT("People p3a: the front gate"), 0.27f, 1.15f);
	P3ASite(TEXT("PplP3ATerrace"), -1, 48.5f, 12, 6, 1.f, P3ABig);
	P3ASite(TEXT("PplP3APainters"), -1, 43.f, 12, 8, 1.f, P3ABig);
	P3ASite(TEXT("PplP3ATrain"), -1, 49.f, 12, 9, 1.f, P3ABig);
	P3ASite(TEXT("PplP3APrizes"), -1, 48.f, 12, 11, 1.f, P3AMid);
	P3ASite(TEXT("PplP3ATrain"), 1, 47.5f, 12, 0, 1.f, P3ABig);
	P3ASite(TEXT("PplP3APhoto"), 1, 43.f, 12, 2, 1.f, P3AMid);
	P3ASite(TEXT("PplP3ATerrace"), 1, 49.f, 12, 3, 1.f, P3ABig);
	P3ASite(TEXT("PplP3APainters"), 1, 48.f, 12, 6, 1.f, P3ABig);

// ---- 2.22 to 2.50 km: PIRATE COVE, to its first hairpin (slots 50 to 55). The docks (left 3 mod 4, right 1 mod 4)
// have a swinging ship and nobody: the photo spot before each. Three pieces. The rest of the cove is the bend and
// the loop, where the track keeps near things out.
Zone(TEXT("People p3a: pirate cove"), 2.22f, 2.50f);
	P3ASite(TEXT("PplP3APhoto"), -1, 29.f, 4, 3, 1.f, P3AMid);
	P3ASite(TEXT("PplP3APhoto"), 1, 29.f, 4, 1, 1.f, P3AMid);

// ---- 2.98 to 3.21 km: THE LIFT HILL (slots 66 to 70). No verge here: the low pieces line the climb. Ten pieces.
Zone(TEXT("People p3a: the lift hill"), 2.98f, 3.21f);
	P3AHill();

// ---- 3.21 to 6.72 km: THE LAGOON (slots 71 to 148). The rail gang's towers, three slots in twelve on each side:
// about 39.
Zone(TEXT("People p3a: the lagoon"), 3.21f, 6.7174f);
	P3AFar(P3A("RailGang"), -1, 47.5f, 48.f, 12, 2, 1.3f, P3AMid);
	P3AFar(P3A("RailGang"), -1, 47.5f, 48.f, 12, 8, 1.3f, P3AMid);
	P3AFar(P3A("RailGang"), -1, 47.5f, 48.f, 12, 11, 1.3f, P3AMid);
	// review of pass 3: the three right-hand rules (51 to 52 m, slots 5, 8, 11 mod 12) are out: p3b's tower stages stand
	// 52 m out on every right slot 2 (mod 3), and the two towers stood inside each other.

// ---- 6.72 to 6.91 km: SPLASHDOWN (slots 149 to 152). No verge: the low pieces down the drop. Eight pieces.
Zone(TEXT("People p3a: splashdown"), 6.7174f, 6.9074f);
	// review of pass 3: P3AHill() taken out here. The unlit low pieces showed as dark clusters on the pale paving beside
	// the crew's lit rostrums, which already fill this verge.

// ---- 6.91 to 8.12 km: ROCKET LAND (slots 153 to 179). Pads and saucers on the even slots; on the odd ones the
// first table has the parade (left 1 mod 4, right 3 mod 4) with nothing behind it: a tall piece there at 44 m, behind
// the carts and single stalls (34 to 38 m). Fourteen by the slots; the corkscrew and the loop take some.
Zone(TEXT("People p3a: rocket land"), 6.9074f, 8.1174f);
	P3ASite(TEXT("PplP3ATrain"), -1, 44.f, 12, 1, 1.f, P3ABig);
	P3ASite(TEXT("PplP3ATerrace"), -1, 44.f, 12, 5, 1.f, P3ABig);
	P3ASite(TEXT("PplP3APrizes"), -1, 44.f, 12, 9, 1.f, P3AMid);
	P3ASite(TEXT("PplP3APainters"), 1, 44.f, 12, 3, 1.f, P3ABig);
	P3ASite(TEXT("PplP3APhoto"), 1, 44.f, 12, 7, 1.f, P3AMid);
	P3ASite(TEXT("PplP3ATrain"), 1, 44.f, 12, 11, 1.f, P3ABig);

// ---- 8.12 to 8.44 km: THE FUNHOUSE (slots 180 to 186). Courts on 2 (mod 3) left and 1 (mod 3) right: behind the
// parade on the left (1 mod 3) and behind STARRY on the right (0 mod 3), at 41 m. Five pieces.
Zone(TEXT("People p3a: the funhouse"), 8.1174f, 8.4374f);
	P3ASite(TEXT("PplP3APrizes"), -1, 41.f, 6, 1, 1.f, P3AMid);
	P3ASite(TEXT("PplP3ATerrace"), -1, 41.f, 6, 4, 1.f, P3ABig);
	P3ASite(TEXT("PplP3ATrain"), 1, 41.f, 6, 0, 1.f, P3ABig);
	P3ASite(TEXT("PplP3APainters"), 1, 41.f, 6, 3, 1.f, P3ABig);

// ---- 8.48 to 8.91 km: THE FUNHOUSE JUMP (slots 188 to 196, after the bend at 8.42 km). No verge: the low pieces
// line the climb and the run-out (the track itself keeps the take-off and the landing clear). Eighteen by the slots.
Zone(TEXT("People p3a: the funhouse jump"), 8.48f, 8.9074f);
	P3AHill();

// ---- 8.91 to 9.30 km: THE BACK LOT (slots 197 to 205). The staff at work where the park is not on show: the
// painters and a delivery behind the cherry pickers (left 2 mod 6, right 0 mod 3), at 42 m. Four pieces.
Zone(TEXT("People p3a: the back lot"), 8.9074f, 9.2974f);
	P3ASite(TEXT("PplP3APainters"), -1, 42.f, 6, 2, 1.f, P3ABig);
	P3ASite(TEXT("PplP3APainters"), 1, 42.f, 6, 0, 1.f, P3ABig);
	P3ASite(TEXT("PplP3APrizes"), 1, 42.f, 6, 3, 1.f, P3AMid);

// ---- 9.30 km to the end: THE LAST RIDE (slots 206 to 553, without the shoot's main unit at 10.9 to 11.23 km).
// The dressing's sets stand on the even slots on the left and the odd ones on the right; the first table's cast takes
// the others at 26 to 32 m. This block adds, in every twelve slots of a side:
//   42 m    behind the parade lane (left slots 1 and 9, right 2 and 10 modulo 12; the watchers' towers have 5 and 6):
//           the terrace, the railway and the painters, going round in 36 slots;
//   29 m    before the pirate docks (left 2, right 11): the photo spot, four in five;
//   29.5 m  before the funhouse courts (left 10, right 3): the prize delivery, four in five.
const auto P3ARide = [&]()
{
	P3ASite(TEXT("PplP3ATerrace"), -1, 42.f, 36, 1, 1.f, P3ABig);    P3ASite(TEXT("PplP3ATrain"), -1, 42.f, 36, 13, 1.f, P3ABig);
	P3ASite(TEXT("PplP3APainters"), -1, 42.f, 36, 25, 1.f, P3ABig);  P3ASite(TEXT("PplP3APainters"), -1, 42.f, 36, 9, 1.f, P3ABig);
	P3ASite(TEXT("PplP3ATerrace"), -1, 42.f, 36, 21, 1.f, P3ABig);   P3ASite(TEXT("PplP3ATrain"), -1, 42.f, 36, 33, 1.f, P3ABig);
	P3ASite(TEXT("PplP3ATrain"), 1, 42.f, 36, 2, 1.f, P3ABig);       P3ASite(TEXT("PplP3APainters"), 1, 42.f, 36, 14, 1.f, P3ABig);
	P3ASite(TEXT("PplP3ATerrace"), 1, 42.f, 36, 26, 1.f, P3ABig);    P3ASite(TEXT("PplP3ATerrace"), 1, 42.f, 36, 10, 1.f, P3ABig);
	P3ASite(TEXT("PplP3ATrain"), 1, 42.f, 36, 22, 1.f, P3ABig);      P3ASite(TEXT("PplP3APainters"), 1, 42.f, 36, 34, 1.f, P3ABig);
	P3ASite(TEXT("PplP3APhoto"), -1, 29.f, 12, 2, 0.8f, P3AMid);     P3ASite(TEXT("PplP3APhoto"), 1, 29.f, 12, 11, 0.8f, P3AMid);
	P3ASite(TEXT("PplP3APrizes"), -1, 29.5f, 12, 10, 0.8f, P3AMid);  P3ASite(TEXT("PplP3APrizes"), 1, 29.5f, 12, 3, 0.8f, P3AMid);
};
Zone(TEXT("People p3a: the last ride"), 9.2974f, 10.9f);
	P3ARide();
Zone(TEXT("People p3a: the last ride, after the hill"), 11.23f, 99.f);
	P3ARide();
// the Last Ride's verge: its free slots (left 1 mod 3, right 0 mod 3), nine in ten, on the level stretches between
// the banked bends (15.07, 15.20, 16.33, 16.47 km and so on every 1.27 km) and off the arena's hill
const auto P3AVerge = [&](float FromKm, float ToKm)
{
	Zone(TEXT("People p3a: the last ride's verge"), FromKm, ToKm);
	P3ASite(TEXT("PplP3AKerbA"), -1, P3AKerbX, 12, 1, 0.9f);    P3ASite(TEXT("PplP3AKerbB"), -1, P3AKerbX, 12, 4, 0.9f);
	P3ASite(TEXT("PplP3AKerbC"), -1, P3AKerbX, 12, 7, 0.9f);    P3ASite(TEXT("PplP3AKerbD"), -1, P3AKerbX, 12, 10, 0.9f);
	P3ASite(TEXT("PplP3AKerbC"), 1, P3AKerbX, 12, 0, 0.9f);     P3ASite(TEXT("PplP3AKerbD"), 1, P3AKerbX, 12, 3, 0.9f);
	P3ASite(TEXT("PplP3AKerbA"), 1, P3AKerbX, 12, 6, 0.9f);     P3ASite(TEXT("PplP3AKerbB"), 1, P3AKerbX, 12, 9, 0.9f);
};
P3AVerge(9.2974f, 10.9f);
P3AVerge(11.94f, 15.f);
P3AVerge(15.27f, 16.27f);
P3AVerge(16.54f, 17.49f);
P3AVerge(17.76f, 18.76f);
P3AVerge(19.03f, 20.02f);
P3AVerge(20.29f, 21.29f);
P3AVerge(21.56f, 22.56f);
P3AVerge(22.83f, 23.82f);
P3AVerge(24.09f, 99.f);
#undef P3A
// ---- END p3a ----
}

{
// ---- BEGIN p3b (scenes, ft_people_park_p3b.py) ----
// SCENES of Coaster Cove (pass 3, builder p3b): ten story groups, one mesh a scene (/Game/People/ThemePark/SM_P3B_...):
// a wedding under a heart arch, a birthday table, the high striker and its giant bear, the giraffe's dance class, a
// magic show, a pie-eating contest, a ride evacuation drill, backstage (the hippo with its head off), the children's
// treasure hunt, the space cadets' passing-out. Each stands on a stage of its own (a striped rostrum 2.6 m high, a back
// cloth, steady lamps), is built 1.2 times life size and is placed at 1.5 times its size: the deck is 3.9 m up, over the
// hoarding and the stalls' roofs, the cast 3 m tall, the back cloth's top 11 to 12.5 m. All TALL (the log: top 7.28 to
// 8.34 m as built): none is nearer than 38 m to the centre line (its front edge 31.9 m or more). Three of them stand
// again on towers (deck 27 m up as placed, legs 6 m under their zero) for the lagoon, where the road is the coaster.
// Steady light only (a glow of 0.09 on every colour, lamps of 1.3); nothing blinks. No clown and no balloon.
// Every scene has a rule of its own that fits one 45 m slot (every 1000, phase 1000 - slot), worked out by
// C:/ftwork/jobs/L4p3b/mkblock.py from the dressing table and the rules above: on a side never two in one slot, never
// in a slot whose set of the dressing's reaches 38 to 46 m (docks, pads, gates, courts) or that has a deck of the
// watchers. Near scenes are Scatter rules (the track leaves them out of loops, corkscrews and jumps); those at the
// jumps, round two loops and in the lagoon are Far rules (drop = the land's depth there, as in the dressing table).
// 331 rules: Backstage 26, BigPrize 28, Birthday 27, Cadets 30, Dance 28, Dance_Sky 18, Magic 27, Magic_Sky 16, PieContest 27, RideDrill 35, Treasure 27, Wedding 25, Wedding_Sky 17.
// A lambda of its own, so that the population function stays small for the compiler.
auto PplP3BScenes = [&]()
{
#define P3B(n) TEXT("/Game/People/ThemePark/SM_P3B_" n)
#define P3B_AT(n, s, side, x) Scatter(P3B(n), x, x, 1000, 1000 - (s), 1.f, 1.5f, 0.f, side, 0.f)
#define P3B_FAR(n, s, side, x, drop) Far(P3B(n), x, x, 1000, 1000 - (s), 1.f, 1.5f, 0.f, side, drop, 0.f)
Zone(TEXT("People p3b: scenes"), 0.f, 99.f);
// the front gate (slots 6 to 24): 14. The slots 0 (mod 3), behind the parade lane: the gate sets, stall rows and the watchers have 1 (mod 3), the podiums 2 (mod 3).
	P3B_AT("Wedding", 6, -1, 40.0f); P3B_AT("Birthday", 9, -1, 40.0f); P3B_AT("Dance", 12, -1, 40.0f); P3B_AT("BigPrize", 15, -1, 40.0f);
	P3B_AT("Magic", 18, -1, 40.0f); P3B_AT("PieContest", 21, -1, 40.0f); P3B_AT("Wedding", 24, -1, 40.0f); P3B_AT("BigPrize", 6, 1, 40.0f);
	P3B_AT("Magic", 9, 1, 40.0f); P3B_AT("PieContest", 12, 1, 40.0f); P3B_AT("Wedding", 15, 1, 40.0f); P3B_AT("Birthday", 18, 1, 40.0f);
	P3B_AT("Dance", 21, 1, 40.0f); P3B_AT("BigPrize", 24, 1, 40.0f);
// the midway (slots 26 to 40, not its corkscrew): 11. In the alley between the two rows of stalls, 38 m: left on even slots, right on odd ones (the watchers' decks have the others).
	P3B_AT("BigPrize", 26, -1, 38.0f); P3B_AT("PieContest", 32, -1, 38.0f); P3B_AT("Magic", 34, -1, 38.0f); P3B_AT("Birthday", 36, -1, 38.0f);
	P3B_AT("Dance", 38, -1, 38.0f); P3B_AT("BigPrize", 40, -1, 38.0f); P3B_AT("Magic", 31, 1, 38.0f); P3B_AT("Birthday", 33, 1, 38.0f);
	P3B_AT("Dance", 35, 1, 38.0f); P3B_AT("BigPrize", 37, 1, 38.0f); P3B_AT("PieContest", 39, 1, 38.0f);
// the big jump (slots 43, 47): 2. By a Far rule 58 m out on the left (the track keeps near things off a jump).
	P3B_FAR("RideDrill", 43, -1, 58.0f, 1.10f); P3B_FAR("RideDrill", 47, -1, 58.0f, 1.10f);
// pirate cove (slots 50 to 64): 8. Left on 2 (mod 4), right on 0 (mod 4): the docks and skull rocks have the odd slots; round the loop on the right by Far rules, 62 m.
	P3B_AT("Treasure", 50, -1, 40.0f); P3B_AT("PieContest", 54, -1, 40.0f); P3B_AT("Treasure", 58, -1, 40.0f); P3B_AT("Treasure", 52, 1, 40.0f);
	P3B_AT("PieContest", 56, 1, 40.0f); P3B_FAR("Treasure", 60, 1, 62.0f, 1.15f); P3B_FAR("Treasure", 62, 1, 62.0f, 1.15f); P3B_FAR("Treasure", 64, 1, 62.0f, 1.15f);
// the lift hill (slots 67 to 71): 5. 45 m, behind the stalls that stand 36 to 40 m out.
	P3B_AT("RideDrill", 68, -1, 45.0f); P3B_AT("Dance", 70, -1, 45.0f); P3B_AT("Dance", 67, 1, 45.0f); P3B_AT("Birthday", 69, 1, 45.0f);
	P3B_AT("RideDrill", 71, 1, 45.0f);
// the lagoon (slots 73 to 148): 51. The three scenes on towers, by Far rules: their decks 27 m up, at the rails. Left on 1 (mod 3) 64 m out, right on 2 (mod 3) 52 m out (the look-outs of the first pass have left 0 and right 1).
	P3B_FAR("Wedding_Sky", 73, -1, 64.0f, 1.40f); P3B_FAR("Dance_Sky", 76, -1, 64.0f, 1.40f); P3B_FAR("Magic_Sky", 79, -1, 64.0f, 1.40f); P3B_FAR("Wedding_Sky", 82, -1, 64.0f, 1.40f);
	P3B_FAR("Dance_Sky", 85, -1, 64.0f, 1.40f); P3B_FAR("Magic_Sky", 88, -1, 64.0f, 1.40f); P3B_FAR("Wedding_Sky", 91, -1, 64.0f, 1.40f); P3B_FAR("Dance_Sky", 94, -1, 64.0f, 1.40f);
	P3B_FAR("Magic_Sky", 97, -1, 64.0f, 1.40f); P3B_FAR("Wedding_Sky", 100, -1, 64.0f, 1.40f); P3B_FAR("Dance_Sky", 103, -1, 64.0f, 1.40f); P3B_FAR("Magic_Sky", 106, -1, 64.0f, 1.40f);
	P3B_FAR("Wedding_Sky", 109, -1, 64.0f, 1.40f); P3B_FAR("Dance_Sky", 112, -1, 64.0f, 1.40f); P3B_FAR("Magic_Sky", 115, -1, 64.0f, 1.40f); P3B_FAR("Wedding_Sky", 118, -1, 64.0f, 1.40f);
	P3B_FAR("Dance_Sky", 121, -1, 64.0f, 1.40f); P3B_FAR("Magic_Sky", 124, -1, 64.0f, 1.40f); P3B_FAR("Wedding_Sky", 127, -1, 64.0f, 1.40f); P3B_FAR("Dance_Sky", 130, -1, 64.0f, 1.40f);
	P3B_FAR("Magic_Sky", 133, -1, 64.0f, 1.40f); P3B_FAR("Wedding_Sky", 136, -1, 64.0f, 1.40f); P3B_FAR("Dance_Sky", 139, -1, 64.0f, 1.40f); P3B_FAR("Magic_Sky", 142, -1, 64.0f, 1.40f);
	P3B_FAR("Wedding_Sky", 145, -1, 64.0f, 1.40f); P3B_FAR("Dance_Sky", 148, -1, 64.0f, 1.40f); P3B_FAR("Dance_Sky", 74, 1, 52.0f, 1.30f); P3B_FAR("Magic_Sky", 77, 1, 52.0f, 1.30f);
	P3B_FAR("Wedding_Sky", 80, 1, 52.0f, 1.30f); P3B_FAR("Dance_Sky", 83, 1, 52.0f, 1.30f); P3B_FAR("Magic_Sky", 86, 1, 52.0f, 1.30f); P3B_FAR("Wedding_Sky", 89, 1, 52.0f, 1.30f);
	P3B_FAR("Dance_Sky", 92, 1, 52.0f, 1.30f); P3B_FAR("Magic_Sky", 95, 1, 52.0f, 1.30f); P3B_FAR("Wedding_Sky", 98, 1, 52.0f, 1.30f); P3B_FAR("Dance_Sky", 101, 1, 52.0f, 1.30f);
	P3B_FAR("Magic_Sky", 104, 1, 52.0f, 1.30f); P3B_FAR("Wedding_Sky", 107, 1, 52.0f, 1.30f); P3B_FAR("Dance_Sky", 110, 1, 52.0f, 1.30f); P3B_FAR("Magic_Sky", 113, 1, 52.0f, 1.30f);
	P3B_FAR("Wedding_Sky", 116, 1, 52.0f, 1.30f); P3B_FAR("Dance_Sky", 119, 1, 52.0f, 1.30f); P3B_FAR("Magic_Sky", 122, 1, 52.0f, 1.30f); P3B_FAR("Wedding_Sky", 125, 1, 52.0f, 1.30f);
	P3B_FAR("Dance_Sky", 128, 1, 52.0f, 1.30f); P3B_FAR("Magic_Sky", 131, 1, 52.0f, 1.30f); P3B_FAR("Wedding_Sky", 134, 1, 52.0f, 1.30f); P3B_FAR("Dance_Sky", 137, 1, 52.0f, 1.30f);
	P3B_FAR("Magic_Sky", 140, 1, 52.0f, 1.30f); P3B_FAR("Wedding_Sky", 143, 1, 52.0f, 1.30f); P3B_FAR("Dance_Sky", 146, 1, 52.0f, 1.30f);
// splashdown (slots 150, 152): 4. 45 m, behind the stalls.
	P3B_AT("Cadets", 150, -1, 45.0f); P3B_AT("RideDrill", 152, -1, 45.0f); P3B_AT("RideDrill", 150, 1, 45.0f); P3B_AT("Cadets", 152, 1, 45.0f);
// rocket land (slots 155 to 180): 10. Left on 1 (mod 4) 42 m, right on 3 (mod 4) 44 m (pads and saucers have the even slots, the first pass's shows the other odd ones); round the loop on the right by Far rules, 60 m.
	P3B_AT("Cadets", 157, -1, 42.0f); P3B_AT("RideDrill", 161, -1, 42.0f); P3B_AT("Cadets", 169, -1, 42.0f); P3B_AT("Magic", 173, -1, 42.0f);
	P3B_AT("Cadets", 155, 1, 44.0f); P3B_AT("RideDrill", 159, 1, 44.0f); P3B_AT("Cadets", 171, 1, 44.0f); P3B_FAR("Cadets", 176, 1, 60.0f, 1.15f);
	P3B_FAR("Cadets", 178, 1, 60.0f, 1.15f); P3B_FAR("Cadets", 180, 1, 60.0f, 1.15f);
// the funhouse (slots 181 to 187): 5. Left on 1 (mod 3), right on 2 (mod 3): off the courts.
	P3B_AT("Magic", 181, -1, 40.0f); P3B_AT("Dance", 184, -1, 40.0f); P3B_AT("Backstage", 187, -1, 40.0f); P3B_AT("Birthday", 182, 1, 39.0f);
	P3B_AT("BigPrize", 185, 1, 39.0f);
// the funhouse jump (slots 188 to 196): 3. By a Far rule 56 m out on the right.
	P3B_FAR("RideDrill", 188, 1, 56.0f, 1.10f); P3B_FAR("RideDrill", 192, 1, 56.0f, 1.10f); P3B_FAR("RideDrill", 196, 1, 56.0f, 1.10f);
// the back lot (slots 198 to 206): 6. Left on 0 (mod 3), right on 2 (mod 3): off the sheds.
	P3B_AT("Backstage", 198, -1, 41.0f); P3B_AT("RideDrill", 201, -1, 41.0f); P3B_AT("Backstage", 204, -1, 41.0f); P3B_AT("RideDrill", 200, 1, 41.0f);
	P3B_AT("Backstage", 203, 1, 41.0f); P3B_AT("Backstage", 206, 1, 41.0f);
// the last ride (slots 207 to 241 and 250 to 554): 212. Its sets stand on the even slots on the left and the odd ones on the right. Left: the slots 1 (mod 4), 40 m (39 m where a look-out tower may stand behind), and 42 m behind the mascots' podiums (3 and 19 mod 32). Right: 2 (mod 4), and behind the podiums (4 and 20 mod 32). Not from 10.9 to 11.23 km (the shoot before the arena).
	P3B_AT("Wedding", 209, -1, 39.0f); P3B_AT("RideDrill", 210, 1, 39.0f); P3B_AT("Cadets", 211, -1, 42.0f); P3B_AT("Magic", 212, 1, 42.0f);
	P3B_AT("BigPrize", 213, -1, 40.0f); P3B_AT("Birthday", 214, 1, 40.0f); P3B_AT("Treasure", 217, -1, 40.0f); P3B_AT("Backstage", 218, 1, 40.0f);
	P3B_AT("Dance", 221, -1, 39.0f); P3B_AT("PieContest", 222, 1, 39.0f); P3B_AT("RideDrill", 225, -1, 40.0f); P3B_AT("Wedding", 226, 1, 40.0f);
	P3B_AT("Magic", 227, -1, 42.0f); P3B_AT("Cadets", 228, 1, 42.0f); P3B_AT("Birthday", 229, -1, 40.0f); P3B_AT("BigPrize", 230, 1, 40.0f);
	P3B_AT("Backstage", 233, -1, 39.0f); P3B_AT("Treasure", 234, 1, 39.0f); P3B_AT("PieContest", 237, -1, 40.0f); P3B_AT("Dance", 238, 1, 40.0f);
	P3B_AT("Wedding", 241, -1, 40.0f); P3B_AT("RideDrill", 250, 1, 40.0f); P3B_AT("Cadets", 253, -1, 40.0f); P3B_AT("Magic", 254, 1, 40.0f);
	P3B_AT("BigPrize", 257, -1, 39.0f); P3B_AT("Birthday", 258, 1, 39.0f); P3B_AT("Treasure", 259, -1, 42.0f); P3B_AT("Backstage", 260, 1, 42.0f);
	P3B_AT("Dance", 261, -1, 40.0f); P3B_AT("PieContest", 262, 1, 40.0f); P3B_AT("RideDrill", 265, -1, 40.0f); P3B_AT("Wedding", 266, 1, 40.0f);
	P3B_AT("Magic", 269, -1, 39.0f); P3B_AT("Cadets", 270, 1, 39.0f); P3B_AT("Birthday", 273, -1, 40.0f); P3B_AT("BigPrize", 274, 1, 40.0f);
	P3B_AT("Backstage", 275, -1, 42.0f); P3B_AT("Treasure", 276, 1, 42.0f); P3B_AT("PieContest", 277, -1, 40.0f); P3B_AT("Dance", 278, 1, 40.0f);
	P3B_AT("Wedding", 281, -1, 39.0f); P3B_AT("RideDrill", 282, 1, 39.0f); P3B_AT("Cadets", 285, -1, 40.0f); P3B_AT("Magic", 286, 1, 40.0f);
	P3B_AT("BigPrize", 289, -1, 40.0f); P3B_AT("Birthday", 290, 1, 40.0f); P3B_AT("Treasure", 291, -1, 42.0f); P3B_AT("Backstage", 292, 1, 42.0f);
	P3B_AT("Dance", 293, -1, 39.0f); P3B_AT("PieContest", 294, 1, 39.0f); P3B_AT("RideDrill", 297, -1, 40.0f); P3B_AT("Wedding", 298, 1, 40.0f);
	P3B_AT("Magic", 301, -1, 40.0f); P3B_AT("Cadets", 302, 1, 40.0f); P3B_AT("Birthday", 305, -1, 39.0f); P3B_AT("BigPrize", 306, 1, 39.0f);
	P3B_AT("Backstage", 307, -1, 42.0f); P3B_AT("Treasure", 308, 1, 42.0f); P3B_AT("PieContest", 309, -1, 40.0f); P3B_AT("Dance", 310, 1, 40.0f);
	P3B_AT("Wedding", 313, -1, 40.0f); P3B_AT("RideDrill", 314, 1, 40.0f); P3B_AT("Cadets", 317, -1, 39.0f); P3B_AT("Magic", 318, 1, 39.0f);
	P3B_AT("BigPrize", 321, -1, 40.0f); P3B_AT("Birthday", 322, 1, 40.0f); P3B_AT("Treasure", 323, -1, 42.0f); P3B_AT("Backstage", 324, 1, 42.0f);
	P3B_AT("Dance", 325, -1, 40.0f); P3B_AT("PieContest", 326, 1, 40.0f); P3B_AT("RideDrill", 329, -1, 39.0f); P3B_AT("Wedding", 330, 1, 39.0f);
	P3B_AT("Magic", 333, -1, 40.0f); P3B_AT("Cadets", 334, 1, 40.0f); P3B_AT("Birthday", 337, -1, 40.0f); P3B_AT("BigPrize", 338, 1, 40.0f);
	P3B_AT("Backstage", 339, -1, 42.0f); P3B_AT("Treasure", 340, 1, 42.0f); P3B_AT("PieContest", 341, -1, 39.0f); P3B_AT("Dance", 342, 1, 39.0f);
	P3B_AT("Wedding", 345, -1, 40.0f); P3B_AT("RideDrill", 346, 1, 40.0f); P3B_AT("Cadets", 349, -1, 40.0f); P3B_AT("Magic", 350, 1, 40.0f);
	P3B_AT("BigPrize", 353, -1, 39.0f); P3B_AT("Birthday", 354, 1, 39.0f); P3B_AT("Treasure", 355, -1, 42.0f); P3B_AT("Backstage", 356, 1, 42.0f);
	P3B_AT("Dance", 357, -1, 40.0f); P3B_AT("PieContest", 358, 1, 40.0f); P3B_AT("RideDrill", 361, -1, 40.0f); P3B_AT("Wedding", 362, 1, 40.0f);
	P3B_AT("Magic", 365, -1, 39.0f); P3B_AT("Cadets", 366, 1, 39.0f); P3B_AT("Birthday", 369, -1, 40.0f); P3B_AT("BigPrize", 370, 1, 40.0f);
	P3B_AT("Backstage", 371, -1, 42.0f); P3B_AT("Treasure", 372, 1, 42.0f); P3B_AT("PieContest", 373, -1, 40.0f); P3B_AT("Dance", 374, 1, 40.0f);
	P3B_AT("Wedding", 377, -1, 39.0f); P3B_AT("RideDrill", 378, 1, 39.0f); P3B_AT("Cadets", 381, -1, 40.0f); P3B_AT("Magic", 382, 1, 40.0f);
	P3B_AT("BigPrize", 385, -1, 40.0f); P3B_AT("Birthday", 386, 1, 40.0f); P3B_AT("Treasure", 387, -1, 42.0f); P3B_AT("Backstage", 388, 1, 42.0f);
	P3B_AT("Dance", 389, -1, 39.0f); P3B_AT("PieContest", 390, 1, 39.0f); P3B_AT("RideDrill", 393, -1, 40.0f); P3B_AT("Wedding", 394, 1, 40.0f);
	P3B_AT("Magic", 397, -1, 40.0f); P3B_AT("Cadets", 398, 1, 40.0f); P3B_AT("Birthday", 401, -1, 39.0f); P3B_AT("BigPrize", 402, 1, 39.0f);
	P3B_AT("Backstage", 403, -1, 42.0f); P3B_AT("Treasure", 404, 1, 42.0f); P3B_AT("PieContest", 405, -1, 40.0f); P3B_AT("Dance", 406, 1, 40.0f);
	P3B_AT("Wedding", 409, -1, 40.0f); P3B_AT("RideDrill", 410, 1, 40.0f); P3B_AT("Cadets", 413, -1, 39.0f); P3B_AT("Magic", 414, 1, 39.0f);
	P3B_AT("BigPrize", 417, -1, 40.0f); P3B_AT("Birthday", 418, 1, 40.0f); P3B_AT("Treasure", 419, -1, 42.0f); P3B_AT("Backstage", 420, 1, 42.0f);
	P3B_AT("Dance", 421, -1, 40.0f); P3B_AT("PieContest", 422, 1, 40.0f); P3B_AT("RideDrill", 425, -1, 39.0f); P3B_AT("Wedding", 426, 1, 39.0f);
	P3B_AT("Magic", 429, -1, 40.0f); P3B_AT("Cadets", 430, 1, 40.0f); P3B_AT("Birthday", 433, -1, 40.0f); P3B_AT("BigPrize", 434, 1, 40.0f);
	P3B_AT("Backstage", 435, -1, 42.0f); P3B_AT("Treasure", 436, 1, 42.0f); P3B_AT("PieContest", 437, -1, 39.0f); P3B_AT("Dance", 438, 1, 39.0f);
	P3B_AT("Wedding", 441, -1, 40.0f); P3B_AT("RideDrill", 442, 1, 40.0f); P3B_AT("Cadets", 445, -1, 40.0f); P3B_AT("Magic", 446, 1, 40.0f);
	P3B_AT("BigPrize", 449, -1, 39.0f); P3B_AT("Birthday", 450, 1, 39.0f); P3B_AT("Treasure", 451, -1, 42.0f); P3B_AT("Backstage", 452, 1, 42.0f);
	P3B_AT("Dance", 453, -1, 40.0f); P3B_AT("PieContest", 454, 1, 40.0f); P3B_AT("RideDrill", 457, -1, 40.0f); P3B_AT("Wedding", 458, 1, 40.0f);
	P3B_AT("Magic", 461, -1, 39.0f); P3B_AT("Cadets", 462, 1, 39.0f); P3B_AT("Birthday", 465, -1, 40.0f); P3B_AT("BigPrize", 466, 1, 40.0f);
	P3B_AT("Backstage", 467, -1, 42.0f); P3B_AT("Treasure", 468, 1, 42.0f); P3B_AT("PieContest", 469, -1, 40.0f); P3B_AT("Dance", 470, 1, 40.0f);
	P3B_AT("Wedding", 473, -1, 39.0f); P3B_AT("RideDrill", 474, 1, 39.0f); P3B_AT("Cadets", 477, -1, 40.0f); P3B_AT("Magic", 478, 1, 40.0f);
	P3B_AT("BigPrize", 481, -1, 40.0f); P3B_AT("Birthday", 482, 1, 40.0f); P3B_AT("Treasure", 483, -1, 42.0f); P3B_AT("Backstage", 484, 1, 42.0f);
	P3B_AT("Dance", 485, -1, 39.0f); P3B_AT("PieContest", 486, 1, 39.0f); P3B_AT("RideDrill", 489, -1, 40.0f); P3B_AT("Wedding", 490, 1, 40.0f);
	P3B_AT("Magic", 493, -1, 40.0f); P3B_AT("Cadets", 494, 1, 40.0f); P3B_AT("Birthday", 497, -1, 39.0f); P3B_AT("BigPrize", 498, 1, 39.0f);
	P3B_AT("Backstage", 499, -1, 42.0f); P3B_AT("Treasure", 500, 1, 42.0f); P3B_AT("PieContest", 501, -1, 40.0f); P3B_AT("Dance", 502, 1, 40.0f);
	P3B_AT("Wedding", 505, -1, 40.0f); P3B_AT("RideDrill", 506, 1, 40.0f); P3B_AT("Cadets", 509, -1, 39.0f); P3B_AT("Magic", 510, 1, 39.0f);
	P3B_AT("BigPrize", 513, -1, 40.0f); P3B_AT("Birthday", 514, 1, 40.0f); P3B_AT("Treasure", 515, -1, 42.0f); P3B_AT("Backstage", 516, 1, 42.0f);
	P3B_AT("Dance", 517, -1, 40.0f); P3B_AT("PieContest", 518, 1, 40.0f); P3B_AT("RideDrill", 521, -1, 39.0f); P3B_AT("Wedding", 522, 1, 39.0f);
	P3B_AT("Magic", 525, -1, 40.0f); P3B_AT("Cadets", 526, 1, 40.0f); P3B_AT("Birthday", 529, -1, 40.0f); P3B_AT("BigPrize", 530, 1, 40.0f);
	P3B_AT("Backstage", 531, -1, 42.0f); P3B_AT("Treasure", 532, 1, 42.0f); P3B_AT("PieContest", 533, -1, 39.0f); P3B_AT("Dance", 534, 1, 39.0f);
	P3B_AT("Wedding", 537, -1, 40.0f); P3B_AT("RideDrill", 538, 1, 40.0f); P3B_AT("Cadets", 541, -1, 40.0f); P3B_AT("Magic", 542, 1, 40.0f);
	P3B_AT("BigPrize", 545, -1, 39.0f); P3B_AT("Birthday", 546, 1, 39.0f); P3B_AT("Treasure", 547, -1, 42.0f); P3B_AT("Backstage", 548, 1, 42.0f);
	P3B_AT("Dance", 549, -1, 40.0f); P3B_AT("PieContest", 550, 1, 40.0f); P3B_AT("RideDrill", 553, -1, 40.0f);
#undef P3B_FAR
#undef P3B_AT
#undef P3B
};
PplP3BScenes();
// ---- END p3b ----
}

{
// ---- BEGIN p3c (movers, ft_people_park_p3c.py) ----
// THE PARK'S MOVERS (population pass 3, builder p3c; meshes /Game/People/ThemePark/SM_P3C_...). Everything here travels
// along the road or circles a point of its own, on a raised way it brings with it; nothing crosses the road.
// Every mesh is TALL by the generator's log (tops 6.0 to 34.2 m as built), so no rule here is nearer than 28 m.
// Where things go, across:
//   28-29.5 m  THE NEAR WAYS, at 1.3 times their size (3.6 m deep as built: front edge 25.6 m or more): the promenade
//              3 m up (Walk_A strollers, Walk_B the fun run, Walk_C wheels), the miniature railway on its viaduct, the
//              horse tram. By Scatter, on slots where the dressing's set stands 38 m out or more and the main table's
//              parade lane and podiums are not (worked out from FTDressL4.inl and the table above): the track itself
//              leaves them out of the jumps, loops and corkscrews.
//   60-68 m    THE HIGH WAYS, 7 to 8 m up, at 1.3 times their size, by Far rules (they stay through the loops): the high
//              walk, the log ride's trough, the pedal cars on their rails, the roller rink (66 m and more: its drum is
//              14.7 m across, and the monorail stands 52 m out on the right).
//   62-64 m    in the lagoon, where the road is a coaster 26 m over the valley: the high walk and the log ride on 27 m
//              piers with a deep foot (at 1.15: their decks 3 to 4 m over the rails' own height by my arithmetic).
//   66-112 m   birds: flamingos flying two slots' length and round again (over the rails in the lagoon), macaws
//              wheeling round a perch tower (thick in Pirate Cove).
// Along: P3CAt and P3CFar put a piece where the slot's number is K modulo Every (a slot is 45 m), as the table above does.
// LIGHT: a steady glow of 0.07 to 0.09 on every colour (the birds 0.3) and rows of steady lamps; nothing blinks; the
// nearest lit geometry is 25.6 m out. No clown, no balloons.
// My estimate of what this block places: about 450 tried (the level is 25.08 km, 557 slots), fewer where the track
// leaves near pieces out; no mesh more than 55 times.
#define P3C(n) TEXT("/Game/People/ThemePark/SM_P3C_" n)
const auto P3CAt = [&](const TCHAR* Mesh, int32 Side, float X, int32 Every, int32 K, float Chance = 1.f, float Scale = 1.3f)
{
	Scatter(Mesh, X, X, Every, (Every - (K % Every)) % Every, Chance, Scale, 0.f, Side, 0.f);
};
const auto P3CFar = [&](const TCHAR* Mesh, int32 Side, float X0, float X1, int32 Every, int32 K, float Drop, float Scale = 1.3f, float YawJ = 0.f)
{
	Far(Mesh, X0, X1, Every, (Every - (K % Every)) % Every, 1.f, Scale, 0.f, Side, Drop, YawJ);
};
const float P3CNear = 28.f;

// ---- 0 to 1.15 km: THE FRONT GATE. A promenade across each ticket gate's forecourt (the gates stand 46 m out on slots
// 4 (mod 12) left and 10 (mod 12) right; the parade has slots 0 (mod 3), the podiums 2 (mod 3)). High ways behind.
Zone(TEXT("People p3c: the front gate"), 0.f, 1.15f);
	P3CAt(P3C("Walk_A"), -1, P3CNear, 12, 4);            P3CAt(P3C("Walk_B"), 1, P3CNear, 12, 10);
	P3CFar(P3C("Skywalk_Lo"), -1, 61.f, 63.f, 12, 3, 1.2f);  P3CFar(P3C("SkyBikes"), -1, 61.f, 63.f, 12, 9, 1.2f);
	P3CFar(P3C("LogRide_Lo"), 1, 61.f, 63.f, 12, 0, 1.2f);   P3CFar(P3C("Rink"), 1, 66.f, 68.f, 12, 6, 1.25f);
	P3CFar(P3C("Parrots"), -1, 96.f, 112.f, 10, 7, 2.0f, 1.6f, 180.f);
	P3CFar(P3C("Parrots"), 1, 96.f, 112.f, 10, 2, 2.0f, 1.6f, 180.f);
	P3CFar(P3C("Flamingos"), -1, 70.f, 110.f, 12, 5, -9.f, 1.5f);
	P3CFar(P3C("Flamingos"), 1, 70.f, 110.f, 12, 11, -9.f, 1.5f);

// ---- 1.15 to 1.84 km: THE MIDWAY. Stalls on every slot: only the high ways, over the second row's roofs.
Zone(TEXT("People p3c: the midway"), 1.15f, 1.84f);
	P3CFar(P3C("SkyBikes"), -1, 64.f, 66.f, 5, 1, 1.2f);     P3CFar(P3C("LogRide_Lo"), 1, 64.f, 66.f, 5, 3, 1.2f);
	P3CFar(P3C("Skywalk_Lo"), 1, 64.f, 66.f, 5, 0, 1.2f);
	P3CFar(P3C("Flamingos"), -1, 70.f, 110.f, 8, 2, -9.f, 1.5f);
	P3CFar(P3C("Parrots"), 1, 96.f, 112.f, 8, 6, 2.0f, 1.6f, 180.f);

// ---- 1.84 to 2.22 km: THE BIG JUMP. A high walk and a rink beyond the second row; flamingos beside the flight.
Zone(TEXT("People p3c: the big jump"), 1.84f, 2.22f);
	P3CFar(P3C("Skywalk_Lo"), -1, 64.f, 66.f, 4, 1, 1.2f);   P3CFar(P3C("Rink"), 1, 67.f, 69.f, 4, 3, 1.25f);
	P3CFar(P3C("Flamingos"), -1, 70.f, 110.f, 4, 0, -12.f, 1.5f);
	P3CFar(P3C("Flamingos"), 1, 70.f, 110.f, 4, 2, -12.f, 1.5f);

// ---- 2.22 to 2.98 km: PIRATE COVE. Docks on slots 3 (mod 4) left and 1 (mod 4) right, the parade on the even slots:
// the near ways take the slots opposite the docks (skull rocks 64 m out there). The macaws are the cove's own.
Zone(TEXT("People p3c: pirate cove"), 2.22f, 2.98f);
	P3CAt(P3C("MiniRail"), -1, P3CNear, 8, 1);           P3CAt(P3C("Walk_C"), -1, P3CNear, 8, 5);
	P3CAt(P3C("HorseTram"), 1, P3CNear, 8, 3);           P3CAt(P3C("Walk_A"), 1, P3CNear, 8, 7);
	P3CFar(P3C("Parrots"), -1, 84.f, 100.f, 3, 0, 1.8f, 1.6f, 180.f);
	P3CFar(P3C("Parrots"), 1, 84.f, 100.f, 3, 1, 1.8f, 1.6f, 180.f);

// ---- 2.98 to 3.21 km: THE LIFT HILL. High ways behind the stalls, flamingos on their way to the lagoon.
Zone(TEXT("People p3c: the lift hill"), 2.98f, 3.21f);
	P3CFar(P3C("Skywalk_Lo"), -1, 66.f, 68.f, 3, 0, 1.25f);  P3CFar(P3C("LogRide_Lo"), 1, 66.f, 68.f, 3, 1, 1.25f);
	P3CFar(P3C("Flamingos"), -1, 70.f, 110.f, 3, 2, -12.f, 1.5f);

// ---- 3.21 to 6.72 km: THE LAGOON. The road is the coaster, 26 m over the valley: the tall high walk and the tall log
// ride on the valley floor behind the look-out towers (51 m) and the bandstands (44 to 58 m), one every 135 m a side,
// the sides out of step. Flamingos at the rails' own height (their zero is 24 m over the valley floor).
Zone(TEXT("People p3c: the lagoon"), 3.21f, 6.7174f);
	// review of pass 3: 78 to 80 m (they were 62 to 64 m, inside p3b's tower stages at 64 m and p3d's lighthouse and mast)
	P3CFar(P3C("Skywalk_Hi"), -1, 78.f, 80.f, 6, 1, 1.3f, 1.15f);   P3CFar(P3C("LogRide_Hi"), -1, 78.f, 80.f, 6, 4, 1.3f, 1.15f);
	P3CFar(P3C("Skywalk_Hi"), 1, 78.f, 80.f, 6, 5, 1.3f, 1.15f);    P3CFar(P3C("LogRide_Hi"), 1, 78.f, 80.f, 6, 2, 1.3f, 1.15f);
	P3CFar(P3C("Flamingos"), -1, 66.f, 110.f, 12, 0, -24.f, 1.5f);
	P3CFar(P3C("Flamingos"), 1, 66.f, 110.f, 12, 6, -24.f, 1.5f);

// ---- 6.72 to 6.91 km: SPLASHDOWN. Off the rails: pedal cars and a high walk behind the saucers.
Zone(TEXT("People p3c: splashdown"), 6.7174f, 6.9074f);
	P3CFar(P3C("SkyBikes"), -1, 62.f, 64.f, 2, 0, 1.2f);     P3CFar(P3C("Skywalk_Lo"), 1, 62.f, 64.f, 2, 1, 1.2f);
	P3CFar(P3C("Flamingos"), 1, 70.f, 110.f, 4, 2, -12.f, 1.5f);

// ---- 6.91 to 8.12 km: ROCKET LAND. Pads (44 m) and saucers (38 m) on the even slots, the parade and the planets on
// the odd ones; crew on rostrums on slots 2 (mod 8) left and 6 (mod 8) right. The near ways take the even slots the crew
// leaves; the high ways stand 60 m out, before the far planets and saucers.
Zone(TEXT("People p3c: rocket land"), 6.9074f, 8.1174f);
	P3CAt(P3C("Walk_B"), -1, P3CNear, 8, 6);             P3CAt(P3C("Walk_C"), 1, P3CNear, 8, 2);
	P3CAt(P3C("HorseTram"), 1, P3CNear, 8, 4);
	P3CFar(P3C("SkyBikes"), -1, 60.f, 61.f, 6, 1, 1.2f);     P3CFar(P3C("LogRide_Lo"), -1, 60.f, 61.f, 6, 4, 1.2f);
	P3CFar(P3C("Rink"), 1, 64.f, 66.f, 6, 1, 1.25f);         P3CFar(P3C("Skywalk_Lo"), 1, 60.f, 61.f, 6, 4, 1.2f);
	P3CFar(P3C("Flamingos"), -1, 70.f, 110.f, 12, 3, -9.f, 1.5f);
	P3CFar(P3C("Flamingos"), 1, 70.f, 110.f, 12, 9, -9.f, 1.5f);
	P3CFar(P3C("Parrots"), -1, 96.f, 112.f, 13, 8, 2.0f, 1.6f, 180.f);

// ---- 8.12 to 8.44 km: THE FUNHOUSE. Funhouses 62 to 74 m out on slots 0 (mod 3): the high ways on the others.
Zone(TEXT("People p3c: the funhouse"), 8.1174f, 8.4374f);
	P3CFar(P3C("LogRide_Lo"), -1, 60.f, 61.f, 3, 1, 1.2f);   P3CFar(P3C("Skywalk_Lo"), 1, 60.f, 61.f, 3, 2, 1.2f);
	P3CFar(P3C("Flamingos"), -1, 70.f, 110.f, 5, 2, -9.f, 1.5f);

// ---- 8.44 to 8.91 km: THE FUNHOUSE JUMP. A rink and pedal cars beyond the stands; birds beside the flight.
Zone(TEXT("People p3c: the funhouse jump"), 8.4374f, 8.9074f);
	P3CFar(P3C("Rink"), -1, 67.f, 69.f, 4, 2, 1.25f);        P3CFar(P3C("SkyBikes"), 1, 62.f, 64.f, 4, 1, 1.2f);
	P3CFar(P3C("Parrots"), -1, 96.f, 112.f, 5, 0, 2.0f, 1.6f, 180.f);
	P3CFar(P3C("Flamingos"), 1, 70.f, 110.f, 5, 3, -12.f, 1.5f);

// ---- 8.91 to 9.30 km: THE BACK LOT. The railway and the tram on their way back to the sheds (the lot's sets stand 58 m
// out on slots 0 (mod 3) left and 2 (mod 3) right; cherry pickers and the waiting floats have the other near slots).
Zone(TEXT("People p3c: the back lot"), 8.9074f, 9.2974f);
	P3CAt(P3C("MiniRail"), -1, P3CNear, 6, 0);           P3CAt(P3C("HorseTram"), 1, 29.5f, 6, 5);
	P3CFar(P3C("Skywalk_Lo"), -1, 66.f, 68.f, 6, 4, 1.25f);  P3CFar(P3C("LogRide_Lo"), 1, 66.f, 68.f, 6, 1, 1.25f);
	P3CFar(P3C("Flamingos"), -1, 70.f, 110.f, 6, 2, -9.f, 1.5f);

// ---- 9.30 km to the end: THE LAST RIDE (351 slots). The dressing's sets stand on the even slots on the left and the odd
// ones on the right, and the main table's cast has every other slot at 26 to 32 m. So the near ways stand in FRONT of the
// sets that keep well back: the gateways (46 m: slots 8 (mod 12) left, 7 (mod 12) right), the rocket pads (44 m: 6 (mod 12)
// left) and the saucer parks (38 m: 9 (mod 12) right), five kinds going round in 36 slots. The high ways take the slots
// the main street's flats (72 to 80 m) leave: odd on the left, 2 (mod 4) on the right, four kinds going round in 24.
Zone(TEXT("People p3c: the last ride"), 9.2974f, 99.f);
	P3CAt(P3C("Walk_A"), -1, P3CNear, 36, 8);            P3CAt(P3C("MiniRail"), -1, P3CNear, 36, 20);
	P3CAt(P3C("Walk_C"), -1, P3CNear, 36, 32);           P3CAt(P3C("HorseTram"), -1, P3CNear, 36, 6);
	P3CAt(P3C("Walk_B"), -1, P3CNear, 36, 18);
	P3CAt(P3C("Walk_B"), 1, P3CNear, 36, 7);             P3CAt(P3C("HorseTram"), 1, P3CNear, 36, 19);
	P3CAt(P3C("Walk_A"), 1, P3CNear, 36, 31);            P3CAt(P3C("MiniRail"), 1, P3CNear, 36, 9);
	P3CAt(P3C("Walk_C"), 1, P3CNear, 36, 21);
	P3CFar(P3C("Skywalk_Lo"), -1, 61.f, 63.f, 24, 1, 1.2f);  P3CFar(P3C("LogRide_Lo"), -1, 61.f, 63.f, 24, 7, 1.2f);
	P3CFar(P3C("SkyBikes"), -1, 61.f, 63.f, 24, 13, 1.2f);   P3CFar(P3C("Rink"), -1, 66.f, 68.f, 24, 19, 1.25f);
	P3CFar(P3C("SkyBikes"), 1, 61.f, 63.f, 24, 2, 1.2f);     P3CFar(P3C("LogRide_Lo"), 1, 61.f, 63.f, 24, 6, 1.2f);
	P3CFar(P3C("Skywalk_Lo"), 1, 61.f, 63.f, 24, 14, 1.2f);  P3CFar(P3C("Rink"), 1, 66.f, 68.f, 24, 22, 1.25f);
	P3CFar(P3C("Parrots"), -1, 96.f, 112.f, 24, 5, 2.0f, 1.6f, 180.f);
	P3CFar(P3C("Parrots"), 1, 96.f, 112.f, 24, 17, 2.0f, 1.6f, 180.f);
	P3CFar(P3C("Flamingos"), -1, 70.f, 110.f, 36, 11, -9.f, 1.5f);
	P3CFar(P3C("Flamingos"), 1, 70.f, 110.f, 36, 29, -9.f, 1.5f);
#undef P3C
// ---- END p3c ----
}

{
// ---- BEGIN p3d (heights, ft_people_park_p3d.py) ----
// PEOPLE UP HIGH (/Game/People/ThemePark/SM_P3D_...): balconies, a clock tower's heralds, a castle wall, a tree house,
// a rope bridge between two towers, a lighthouse, a ship's mast with two crow's nests, Rocket Land's launch gantry, a
// party on a kiosk's roof. Every piece brings its own structure, is TALL (tops 10.7 to 31.9 m in the generator's log)
// and stands 39.5 m or more from the centre line: nothing of this block is nearer. Steady glow only, nothing blinks.
// NOT COMPILED AND NOT SEEN IN THE GAME when written.
// Where they stand, across (worked out from the slots FTDressL4.inl's sets and FTPeopleL4.inl's rules take):
//   44-48 m   behind the parade lane and the verge's pieces, in slots without a set of the dressing's
//   52 m      on the left of the Last Ride, behind the main table's big pieces (29 to 32 m) and the ponds
//   60-68 m   behind the high wire and the stages in Pirate Cove; on the right of the Last Ride, beyond the monorail
//             (52 m); beside both jumps and the rocket corkscrew by Far rules (they stay there)
//   64-66 m   over the lagoon, by Far rules: the lighthouse and the mast at 1.12 times their size on the valley floor
//             (both have a foot 6 m deep), their top galleries about level with the coaster's rails; on the slots the
//             coasters end on (left 2 mod 3, right 0 mod 3) and the main table's look-outs (left 0, right 1) leave free
// Along: P3DAt and P3DFar put a piece where the slot's number is K modulo Every (a slot is 45 m), like the tables above.
#define P3D(n) TEXT("/Game/People/ThemePark/SM_P3D_" n)
const auto P3DAt = [&](const TCHAR* Mesh, int32 Side, float X, int32 Every, int32 K, float Chance = 1.f)
{
	Scatter(Mesh, X, X, Every, (Every - (K % Every)) % Every, Chance, 1.f, 0.f, Side, 0.f);
};
const auto P3DFar = [&](const TCHAR* Mesh, int32 Side, float X0, float X1, int32 Every, int32 K, float Drop, float Scale = 1.f)
{
	Far(Mesh, X0, X1, Every, (Every - (K % Every)) % Every, 1.f, Scale, 0.f, Side, Drop, 0.f);
};
// the funhouse's left side: a court's chair swing stands 30 m on from the court, 50 m out, so the piece in the slot
// before it is pinned by a Site (no dice along the road) and moved 8 m back down the road
Set(TEXT("PplP3DFunRoof"),    { { P3D("RoofParty"), 8.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3DFunBalcony"), { { P3D("Balcony_A"), 8.f, 0.f, 0.f, 0.f, 1.f } });

// ---- 0 to 1.15 km: THE FRONT GATE (slots 1 to 25). The slots 0 (mod 3) have no set and no podium: the hotel's front
// and the clock tower on the left, the tall house and the roof party on the right, 47 m out behind the parade lane.
Zone(TEXT("People p3d: the front gate"), 0.f, 1.15f);
	P3DAt(P3D("Balcony_A"), -1, 47.f, 12, 0);     P3DAt(P3D("ClockTower"), -1, 47.f, 12, 6);
	P3DAt(P3D("Balcony_B"), 1, 47.f, 12, 3);      P3DAt(P3D("RoofParty"), 1, 47.f, 12, 9);

// ---- 1.15 to 1.84 km: THE MIDWAY (slots 26 to 40; 27 to 30 are the corkscrew, which keeps near things out). The
// clock tower between the second row of stalls and the high strikers on the left; the mast between the two rows on
// the right, on the odd slots the watchers' decks leave free.
Zone(TEXT("People p3d: the midway"), 1.15f, 1.84f);
	P3DAt(P3D("ClockTower"), -1, 50.5f, 6, 4);
	P3DAt(P3D("CrowsNest"), 1, 39.5f, 4, 1);

// ---- 1.84 to 2.22 km: THE BIG JUMP (slots 41 to 49). The lighthouse and the mast beside the flight, by Far rules.
Zone(TEXT("People p3d: the big jump"), 1.84f, 2.22f);
	P3DFar(P3D("Lighthouse"), -1, 64.f, 66.f, 4, 1, 1.25f);
	P3DFar(P3D("CrowsNest"), 1, 64.f, 66.f, 4, 3, 1.25f);

// ---- 2.22 to 2.98 km: PIRATE COVE (slots 50 to 65; the loop from 59 on keeps near things out). Docks on the left's
// slots 3 (mod 4) and the right's 1 (mod 4): the mast and the lighthouse behind the watchers' decks on the slots
// opposite; the rope bridge and the castle wall behind the high wire on the even slots.
Zone(TEXT("People p3d: pirate cove"), 2.22f, 2.98f);
	P3DAt(P3D("CrowsNest"), -1, 46.f, 4, 1);      P3DAt(P3D("Lighthouse"), 1, 46.f, 4, 3);
	P3DAt(P3D("RopeBridge"), -1, 60.f, 4, 0);     P3DAt(P3D("Castle"), 1, 60.f, 4, 2);

// ---- 2.98 to 3.21 km: THE LIFT HILL (slots 66 to 70). Behind the stalls, on the slots the queue towers leave free.
Zone(TEXT("People p3d: the lift hill"), 2.98f, 3.21f);
	P3DAt(P3D("TreeHouse"), -1, 47.5f, 2, 0);     P3DAt(P3D("RoofParty"), 1, 47.5f, 2, 1);

// ---- 3.21 to 6.72 km: THE LAGOON (slots 71 to 148). The road is the coaster, 26 m over the valley: the lighthouse
// and the mast stand on the valley floor, turn about, one every 135 m a side.
Zone(TEXT("People p3d: the lagoon"), 3.21f, 6.7174f);
	// review of pass 3: on the left they stood in p3b's tower stages (64 m, every slot 1 mod 3). Now on the slots 0 (mod 3),
	// 68 m out, behind the first pass's look-outs (51 m). On the right the stages are 52 m out: no change there.
	P3DFar(P3D("Lighthouse"), -1, 68.f, 69.f, 6, 0, 1.4f, 1.12f);
	P3DFar(P3D("CrowsNest"), -1, 68.f, 69.f, 6, 3, 1.4f, 1.12f);
	P3DFar(P3D("CrowsNest"), 1, 64.f, 66.f, 6, 2, 1.4f, 1.12f);
	P3DFar(P3D("Lighthouse"), 1, 64.f, 66.f, 6, 5, 1.4f, 1.12f);

// ---- 6.91 to 8.12 km: ROCKET LAND (slots 153 to 179). Pads and saucers on the even slots; the main table's shows and
// the watchers' decks on the left's 3 (mod 4) and the right's 1 (mod 4): the gantry on the others, turn about with a
// roof party (left) and the clock tower (right). Through the corkscrew the gantry by Far rules.
Zone(TEXT("People p3d: rocket land"), 6.9074f, 8.1174f);
	P3DAt(P3D("Gantry"), -1, 44.f, 8, 1);         P3DAt(P3D("RoofParty"), -1, 44.f, 8, 5);
	P3DAt(P3D("Gantry"), 1, 44.f, 8, 3);          P3DAt(P3D("ClockTower"), 1, 44.f, 8, 7);
Zone(TEXT("People p3d: the rocket corkscrew"), 7.33f, 7.52f);
	P3DFar(P3D("Gantry"), -1, 64.f, 66.f, 2, 1, 1.25f);
	P3DFar(P3D("Gantry"), 1, 64.f, 66.f, 2, 0, 1.25f);

// ---- 8.12 to 8.44 km: THE FUNHOUSE (slots 180 to 186). Courts on the left's slots 2 (mod 3) and the right's 1 (mod 3).
Zone(TEXT("People p3d: the funhouse"), 8.1174f, 8.4374f);
	Site(TEXT("PplP3DFunRoof"), -1, 44.f, 6, 5, 1.f);         // slot 1 (mod 6)
	Site(TEXT("PplP3DFunBalcony"), -1, 44.f, 6, 2, 1.f);      // slot 4 (mod 6)
	P3DAt(P3D("Balcony_B"), 1, 44.f, 6, 0);       P3DAt(P3D("ClockTower"), 1, 44.f, 6, 3);

// ---- 8.44 to 8.91 km: THE FUNHOUSE JUMP (slots 187 to 196). The castle wall and the tree house beside the flight.
Zone(TEXT("People p3d: the funhouse jump"), 8.4374f, 8.9074f);
	P3DFar(P3D("Castle"), -1, 66.f, 68.f, 4, 2, 1.25f);
	P3DFar(P3D("TreeHouse"), 1, 66.f, 68.f, 4, 0, 1.25f);

// ---- 8.91 to 9.30 km: THE BACK LOT (slots 197 to 205). Sheds on the left's slots 0 (mod 3) and the right's 2 (mod 3).
Zone(TEXT("People p3d: the back lot"), 8.9074f, 9.2974f);
	P3DAt(P3D("RopeBridge"), -1, 48.f, 6, 2);     P3DAt(P3D("Castle"), -1, 48.f, 6, 5);
	P3DAt(P3D("TreeHouse"), 1, 46.f, 6, 1);       P3DAt(P3D("RopeBridge"), 1, 46.f, 6, 4);

// ---- 9.30 km to the end: THE LAST RIDE (slots 206 to 553). Its sets stand on the even slots on the left and the odd
// ones on the right; this block takes the others, in a round of 24 slots, twenty pieces to a round:
//   left   44 m on slots 1 and 9 (mod 12): behind the parade lane (the watchers' tower has 5, a court's chair swing
//          stands beside 11); 52 m on slots 3 and 7 (mod 12), behind the main table's big pieces; 64 m on slot 11
//   right  44 m on slot 10 (mod 12); 62 m on slots 0, 4 and 8, beyond the monorail; 64 m on slot 2 (a court's chair
//          swing stands beside it at 50 m); the watchers' tower has 6
Zone(TEXT("People p3d: the last ride"), 9.2974f, 99.f);
	P3DAt(P3D("Balcony_A"), -1, 44.f, 24, 1);     P3DAt(P3D("TreeHouse"), -1, 44.f, 24, 9);
	P3DAt(P3D("RoofParty"), -1, 44.f, 24, 13);    P3DAt(P3D("ClockTower"), -1, 44.f, 24, 21);
	P3DAt(P3D("RopeBridge"), -1, 52.f, 24, 3);    P3DAt(P3D("Balcony_B"), -1, 52.f, 24, 7);
	P3DAt(P3D("Castle"), -1, 52.f, 24, 15);       P3DAt(P3D("Lighthouse"), -1, 52.f, 24, 19);
	P3DAt(P3D("CrowsNest"), -1, 64.f, 24, 11);    P3DAt(P3D("TreeHouse"), -1, 64.f, 24, 23);
	P3DAt(P3D("Gantry"), 1, 44.f, 24, 10);        P3DAt(P3D("RoofParty"), 1, 44.f, 24, 22);
	P3DAt(P3D("Castle"), 1, 62.f, 24, 0);         P3DAt(P3D("Balcony_A"), 1, 62.f, 24, 4);
	P3DAt(P3D("Gantry"), 1, 62.f, 24, 8);         P3DAt(P3D("RopeBridge"), 1, 62.f, 24, 12);
	P3DAt(P3D("ClockTower"), 1, 62.f, 24, 16);    P3DAt(P3D("Balcony_B"), 1, 62.f, 24, 20);
	P3DAt(P3D("TreeHouse"), 1, 64.f, 24, 2);      P3DAt(P3D("RoofParty"), 1, 64.f, 24, 14);
#undef P3D
// ---- END p3d ----
}

{
// ---- BEGIN p3e (far and sky, ft_people_park_p3e.py) ----
// THE PARK'S FAR DISTANCE AND ITS SKY, pass 3 (meshes /Game/People/ThemePark/SM_P3E_..., all saved with the always-moving
// master). Nothing of this block is nearer than 150 m to the centre line: every piece is TALL (17 to 48 m as built) and
// carries a small steady glow (a night level); nothing blinks. No clown and no balloons. Every piece is put down by a Far
// rule, so it stays beside the loops, corkscrews and jumps; its drop is counted from the land's own level at its
// distance, as in the blocks above (2.1 m at 165 m to 3.0 m at 270 m), and every piece has a 15 m foot under it because
// the lagoon's land falls away. The flocks and the parachute team take a NEGATIVE drop: it lifts them into the sky.
// Where things go, across (metres from the centre line, scale):
//   150-170   parrots wheeling (x1.6, 24 to 40 m up)          162-168   the sky glide between its two towers (x1.5)
//   172-178   the little railway on its trestle (x1.3)        186-192   the monorail and its station (x1.5)
//   194-200   the climbing rock (x1.6: 64 m)                  170-250   flamingos in a skein (x2)
//   200-270   the parachute display team (x1.8)               218-228   the stand doing a wave (x1.8)
//   236-250   the picnic hill (x1.7)                          258-285   the kite hills (x2: kites 55 to 85 m up)
// The table above this block has its far parades at 150-160 m and its queue towers at 205-215 m; the dressing has big
// wheels at 180-270 m, coaster hills at 150-285 m and hot-air balloons at 190-290 m, all by dice: these rules name their
// slots (a piece where the slot's number is K modulo Every; the right side half a round after the left).
// Estimated count (557 slots of 45 m): 47 + 42 + 78 + 73 + 214 = about 454 pieces; no mesh more than about 45 times.
#define P3E(n) TEXT("/Game/People/ThemePark/SM_P3E_" n)
const auto P3EAt = [&](const TCHAR* Mesh, int32 Side, float X0, float X1, int32 Every, int32 K, float Drop, float Scale, float YawJ)
{
	Far(Mesh, X0, X1, Every, (Every - (K % Every)) % Every, 1.f, Scale, 0.f, Side, Drop, YawJ);
};
// one word a piece: (every, the left side's slot, the right side's slot); the birds and the team also take their lift
const auto P3ETrain = [&](int32 E, int32 KL, int32 KR)
{
	P3EAt(P3E("Train"), -1, 172.f, 178.f, E, KL, 2.2f, 1.3f, 0.f);       P3EAt(P3E("Train"), 1, 172.f, 178.f, E, KR, 2.2f, 1.3f, 0.f);
};
const auto P3EMono = [&](int32 E, int32 KL, int32 KR)
{
	P3EAt(P3E("Monorail"), -1, 186.f, 192.f, E, KL, 2.3f, 1.5f, 0.f);    P3EAt(P3E("Monorail"), 1, 186.f, 192.f, E, KR, 2.3f, 1.5f, 0.f);
};
const auto P3EZip = [&](int32 E, int32 KL, int32 KR)
{
	P3EAt(P3E("Zipline"), -1, 162.f, 168.f, E, KL, 2.1f, 1.5f, 0.f);     P3EAt(P3E("Zipline"), 1, 162.f, 168.f, E, KR, 2.1f, 1.5f, 0.f);
};
const auto P3ESpire = [&](int32 E, int32 KL, int32 KR)
{
	P3EAt(P3E("ClimbSpire"), -1, 194.f, 200.f, E, KL, 2.4f, 1.6f, 10.f); P3EAt(P3E("ClimbSpire"), 1, 194.f, 200.f, E, KR, 2.4f, 1.6f, 10.f);
};
const auto P3EWave = [&](int32 E, int32 KL, int32 KR)
{
	P3EAt(P3E("WaveStand"), -1, 218.f, 228.f, E, KL, 2.6f, 1.8f, 0.f);   P3EAt(P3E("WaveStand"), 1, 218.f, 228.f, E, KR, 2.6f, 1.8f, 0.f);
};
const auto P3EHill = [&](int32 E, int32 KL, int32 KR)
{
	P3EAt(P3E("PicnicHill"), -1, 236.f, 250.f, E, KL, 2.8f, 1.7f, 8.f);  P3EAt(P3E("PicnicHill"), 1, 236.f, 250.f, E, KR, 2.8f, 1.7f, 8.f);
};
const auto P3EKitesA = [&](int32 E, int32 KL, int32 KR)
{
	P3EAt(P3E("Kites_A"), -1, 258.f, 285.f, E, KL, 3.0f, 2.0f, 8.f);     P3EAt(P3E("Kites_A"), 1, 258.f, 285.f, E, KR, 3.0f, 2.0f, 8.f);
};
const auto P3EKitesB = [&](int32 E, int32 KL, int32 KR)
{
	P3EAt(P3E("Kites_B"), -1, 258.f, 285.f, E, KL, 3.0f, 2.0f, 8.f);     P3EAt(P3E("Kites_B"), 1, 258.f, 285.f, E, KR, 3.0f, 2.0f, 8.f);
};
const auto P3EParrots = [&](int32 E, int32 KL, int32 KR, float Lift)
{
	P3EAt(P3E("Parrots"), -1, 150.f, 170.f, E, KL, -Lift, 1.6f, 180.f);  P3EAt(P3E("Parrots"), 1, 150.f, 170.f, E, KR, -Lift, 1.6f, 180.f);
};
const auto P3EFlam = [&](int32 E, int32 KL, int32 KR, float Lift)
{
	P3EAt(P3E("Flamingos"), -1, 170.f, 250.f, E, KL, -Lift, 2.0f, 12.f); P3EAt(P3E("Flamingos"), 1, 170.f, 250.f, E, KR, -Lift, 2.0f, 12.f);
};
const auto P3ETeam = [&](int32 E, int32 KL, int32 KR, float Lift)
{
	P3EAt(P3E("SkyTeam"), -1, 200.f, 270.f, E, KL, -Lift, 1.8f, 180.f);  P3EAt(P3E("SkyTeam"), 1, 200.f, 270.f, E, KR, -Lift, 1.8f, 180.f);
};

// ---- 0 to 1.84 km: THE FRONT GATE AND THE MIDWAY (41 slots; a round of 14). Opening night's traffic: the little railway,
// the monorail and the sky glide behind the main street, a stand doing a wave, a kite hill, parrots and flamingos over it.
Zone(TEXT("People p3e: the gate and the midway"), 0.f, 1.84f);
	P3ETrain(14, 0, 7);        P3EKitesA(14, 2, 9);       P3EZip(14, 4, 11);         P3EParrots(14, 5, 12, 6.f);
	P3EWave(14, 7, 0);         P3EMono(14, 9, 2);         P3EFlam(14, 11, 4, 8.f);   P3ESpire(14, 12, 5);

// ---- 1.84 to 3.21 km: THE BIG JUMP, PIRATE COVE, THE LIFT HILL (30 slots; a round of 10). The cove's parrots, the
// display team coming down beside the jump, a climbing rock, the dragon kite, a picnic hill, the sky glide.
Zone(TEXT("People p3e: the jump, the cove, the lift hill"), 1.84f, 3.21f);
	P3EParrots(10, 0, 5, 6.f); P3EFlam(10, 1, 6, 8.f);    P3ETeam(10, 3, 8, 12.f);   P3ESpire(10, 4, 9);
	P3EKitesB(10, 6, 1);       P3EHill(10, 7, 2);         P3EZip(10, 9, 4);

// ---- 3.21 to 6.72 km: THE LAGOON (78 slots; a round of 16). The road is the coaster, 26 m over the valley: the birds
// and the team are lifted to the rails' height and over it, the kites (66 to 85 m over the valley floor) stand well
// above the rails, the hills, the stand and the monorail are looked down on.
Zone(TEXT("People p3e: the lagoon"), 3.21f, 6.7174f);
	P3EKitesA(16, 0, 8);       P3EParrots(16, 2, 10, 18.f); P3EHill(16, 4, 12);      P3EFlam(16, 6, 14, 18.f);
	P3EKitesB(16, 8, 0);       P3ETeam(16, 10, 2, 22.f);  P3EWave(16, 12, 4);        P3EMono(16, 14, 6);

// ---- 6.72 to 9.30 km: SPLASHDOWN, ROCKET LAND, THE FUNHOUSE AND ITS JUMP, THE BACK LOT (57 slots; a round of 14).
Zone(TEXT("People p3e: rocket land to the back lot"), 6.7174f, 9.2974f);
	P3ETrain(14, 0, 7);        P3ETeam(14, 1, 8, 12.f);   P3EZip(14, 3, 10);         P3EKitesA(14, 4, 11);
	P3EWave(14, 6, 13);        P3EMono(14, 8, 1);         P3EHill(14, 9, 2);         P3ESpire(14, 11, 4);
	P3EKitesB(14, 12, 5);

// ---- 9.30 km to the end: THE LAST RIDE (351 slots; a round of 36: all eleven on each side, about every third slot,
// the right side half a round after the left).
Zone(TEXT("People p3e: the last ride"), 9.2974f, 99.f);
	P3ETrain(36, 0, 18);       P3EKitesA(36, 3, 21);      P3EParrots(36, 6, 24, 6.f); P3EWave(36, 10, 28);
	P3EZip(36, 13, 31);        P3ETeam(36, 16, 34, 12.f); P3EHill(36, 19, 1);        P3EMono(36, 23, 5);
	P3EFlam(36, 26, 8, 8.f);   P3ESpire(36, 29, 11);      P3EKitesB(36, 33, 15);
#undef P3E
// ---- END p3e ----
}

{
// ---- BEGIN p3f (film shoot, ft_people_park_p3f.py) ----
// MORE OF THE SHOOT (pass 3, builder p3f; meshes /Game/People/ThemePark/SM_P3F_..., made by ft_people_park_p3f.py).
// The main table has the main unit at the start, the two jumps and before the arena. This block spreads the rest of a
// big shoot along the whole road: second units, drone teams (a drone hovering over its pad, never over the road), stunt
// rigging, the extras' holding area, the make-up tent, press pens, the dolly on its rails, and over the lagoon camera
// and drone towers with their decks at the coaster's rails. It also stands three scenes of the shared crew kit
// (/Game/People/Crew/SM_Crew..._Park: lights, crane, sound) behind units of its own.
// Every piece of this block is over 2 m tall (raised on a rostrum or a scaffold, like the main table's: the verge is a
// hoarding 1.8 m high) and stands 27.5 m out or more; a steady glow and steady lamps only, nothing blinks.
// Where, across: 27.5 m  a unit on its rostrum at 1.25 times its size (its bounds' front edge 24.4 m out or more),
//                        only in slots where neither the dressing's sets nor the main table's parade lane (24.9 to
//                        27.1 m) and big pieces (29 to 35 m) stand in that band: each rule names its slots
//                32.5-36 the same behind the parade lane or the start's crew, where the nearer band is taken
//                33-35   the shared kit's lights, crane and sound, behind a unit
//                45.5-57 by Far rules through the jumps, loops and corkscrews, and the lagoon's towers
// P3FAt(mesh, side, x m, every, k): the slots whose number is k modulo "every" (a slot is 45 m).
#define P3F(n) TEXT("/Game/People/ThemePark/SM_P3F_" n)
#define P3FK(n) TEXT("/Game/People/Crew/SM_Crew" n "_Park")
const auto P3FAt = [&](const TCHAR* Mesh, int32 Side, float X, int32 Every, int32 K, float Scale = 1.25f, float Chance = 1.f)
{
	Scatter(Mesh, X, X, Every, (Every - (K % Every)) % Every, Chance, Scale, 0.f, Side, 0.f);
};
const auto P3FFar = [&](const TCHAR* Mesh, int32 Side, float X0, float X1, int32 Every, int32 K, float Drop, float Scale = 1.25f)
{
	Far(Mesh, X0, X1, Every, (Every - (K % Every)) % Every, 1.f, Scale, 0.f, Side, Drop, 0.f);
};
const float P3FNear = 27.5f;

// ---- 0 to 0.27 km: THE START (slots 1 to 5). The main unit has slots 0, 3 and 5 at 26.9 m; stall rows stand on slot 1
// and a gate on 4. The press and the make-up tent behind them: press pens on slot 2 behind the candy carts, the tent
// behind the camera tower, the extras behind the applauding crew.
Zone(TEXT("People p3f: the start"), 0.f, 0.265f);
	P3FAt(P3F("PressPen_A"), -1, 36.f, 6, 2);       P3FAt(P3F("PressPen_B"), 1, 36.f, 6, 2);
	P3FAt(P3F("MakeUp"), -1, 33.5f, 6, 3);          P3FAt(P3F("Extras_A"), 1, 33.5f, 6, 3);

// ---- 0.27 to 1.15 km: THE FRONT GATE. Slots 0 (mod 3) have the parade lane and nothing behind it before the ponds at
// 36 m: a unit at 32.5 m on each, a different one every time for 540 m.
Zone(TEXT("People p3f: the front gate"), 0.275f, 1.15f);
	P3FAt(P3F("SecondUnit_A"), -1, 32.5f, 12, 0);   P3FAt(P3F("Extras_B"), -1, 32.5f, 12, 3);
	P3FAt(P3F("DroneTeam_A"), -1, 32.5f, 12, 6);    P3FAt(P3F("PressPen_A"), -1, 32.5f, 12, 9);
	P3FAt(P3F("MakeUp"), 1, 32.5f, 12, 0);          P3FAt(P3F("StuntRig"), 1, 32.5f, 12, 3);
	P3FAt(P3F("SecondUnit_B"), 1, 32.5f, 12, 6);    P3FAt(P3F("Extras_A"), 1, 32.5f, 12, 9);

// ---- 1.15 to 1.84 km: THE MIDWAY. Stalls on every slot and the main table's high wire before them on every other one
// (left even, right odd): the stunt gantry and a drone team on the others, a tall thing over the crowd like the wire.
Zone(TEXT("People p3f: the midway"), 1.15f, 1.84f);
	P3FAt(P3F("StuntRig"), -1, 28.5f, 4, 1);        P3FAt(P3F("StuntRig"), 1, 28.5f, 4, 2);
	P3FAt(P3F("DroneTeam_B"), -1, 28.5f, 8, 3);     P3FAt(P3F("DroneTeam_B"), 1, 28.5f, 8, 0);

// ---- 1.84 to 2.22 km: THE BIG JUMP. The main unit has most slots at 26.9 m: the dolly and a press pen on the two it
// leaves; a drone team and the stunt rigging 46 m out by Far rules (the track keeps the run-up and the landing clear
// within 40 m; the main table's lights and catering are at 42 to 43 m on other slots, the stands from 52.5 m).
Zone(TEXT("People p3f: the big jump"), 1.84f, 2.22f);
	P3FAt(P3F("Dolly"), -1, P3FNear, 4, 3);         P3FAt(P3F("PressPen_A"), 1, P3FNear, 4, 2);
	P3FFar(P3F("DroneTeam_A"), -1, 45.5f, 46.5f, 3, 1, 1.05f);
	P3FFar(P3F("StuntRig"), 1, 45.5f, 46.5f, 3, 2, 1.05f);

// ---- 2.22 to 2.98 km: PIRATE COVE. The parade and the shows have the even slots: the units take the odd ones (the
// docks stand 40 m out on them, the skull rocks 64 m), off the cherry picker's slots (1 mod 8, left).
Zone(TEXT("People p3f: pirate cove"), 2.22f, 2.98f);
	P3FAt(P3F("SecondUnit_A"), -1, P3FNear, 8, 3);  P3FAt(P3F("PressPen_B"), -1, P3FNear, 8, 5);
	P3FAt(P3F("Extras_A"), -1, P3FNear, 8, 7);
	P3FAt(P3F("DroneTeam_A"), 1, P3FNear, 8, 1);    P3FAt(P3F("StuntRig"), 1, P3FNear, 8, 3);
	P3FAt(P3F("MakeUp"), 1, P3FNear, 8, 5);         P3FAt(P3F("SecondUnit_B"), 1, P3FNear, 8, 7);
Zone(TEXT("People p3f: the cove's loop"), 2.665f, 2.945f);
	P3FFar(P3F("SecondUnit_B"), -1, 53.f, 54.f, 2, 1, 1.05f, 1.3f);

// ---- 2.98 to 3.21 km: THE LIFT HILL. STARRY lines the climb at 27 to 30 m and stalls stand from 36 m: one unit a
// side between them.
Zone(TEXT("People p3f: the lift hill"), 2.98f, 3.21f);
	P3FAt(P3F("Extras_B"), -1, 33.f, 4, 0, 1.f);    P3FAt(P3F("PressPen_A"), 1, 33.f, 4, 1, 1.f);

// ---- 3.21 to 6.72 km: THE LAGOON. The road is the coaster, 26 m over the valley: the shoot comes up to the rails on
// towers 25 m high (Far rules, like the main table's look-outs at 51 to 52 m, and with their drop and size). The
// look-outs stand on slots 0 (mod 3) left and 1 (mod 3) right, the bandstands on 2 (mod 3): the towers take 1 (mod 3)
// on the left and 3 (mod 6) on the right (off the bleachers' 0 mod 6). The camera tower 53 to 54 m out, the drone
// tower 56 to 57 m (its second drone circles 5.6 m from the tower's middle, 49 m from the centre line at its nearest).
Zone(TEXT("People p3f: the lagoon"), 3.21f, 6.7174f);
	P3FFar(P3F("LagoonTower_A"), -1, 53.f, 54.f, 6, 1, 1.3f, 1.15f);
	// review of pass 3: the drone tower (LagoonTower_B: two big quadcopters flying free at the rails' height, where this
	// level's enemies are helicopters) is not placed any more: the camera tower stands on its slots.
	P3FFar(P3F("LagoonTower_A"), -1, 56.f, 57.f, 6, 4, 1.3f, 1.15f);
	P3FFar(P3F("LagoonTower_A"), 1, 53.f, 54.f, 12, 3, 1.3f, 1.15f);
	P3FFar(P3F("LagoonTower_A"), 1, 56.f, 57.f, 12, 9, 1.3f, 1.15f);

// ---- 6.72 to 6.91 km: SPLASHDOWN. The main table's crew has the left's even slots and the right's odd ones.
Zone(TEXT("People p3f: splashdown"), 6.7174f, 6.9074f);
	P3FAt(P3F("SecondUnit_A"), -1, P3FNear, 4, 1);  P3FAt(P3F("Applause_B"), 1, P3FNear, 4, 0);

// ---- 6.91 to 8.12 km: ROCKET LAND. The parade lane and the planets have the odd slots: the units take the even ones
// (pads 44 m out and saucers 38 m), off the main table's crew (2 mod 8 left, 6 mod 8 right). By Far rules through the
// corkscrew and round the loop, before the main table's stands.
Zone(TEXT("People p3f: rocket land"), 6.9074f, 8.1174f);
	P3FAt(P3F("SecondUnit_A"), -1, P3FNear, 8, 0);  P3FAt(P3F("MakeUp"), -1, P3FNear, 8, 4);
	P3FAt(P3F("Extras_A"), -1, P3FNear, 8, 6);
	P3FAt(P3F("StuntRig"), 1, P3FNear, 8, 0);       P3FAt(P3F("DroneTeam_B"), 1, P3FNear, 8, 2);
	P3FAt(P3F("PressPen_B"), 1, P3FNear, 8, 4);
Zone(TEXT("People p3f: the rocket corkscrew"), 7.33f, 7.52f);
	P3FFar(P3F("SecondUnit_B"), -1, 50.f, 51.5f, 2, 1, 1.05f, 1.3f);
	P3FFar(P3F("DroneTeam_A"), 1, 50.f, 51.5f, 2, 0, 1.05f, 1.3f);
Zone(TEXT("People p3f: the rocket loop"), 7.8404f, 8.1204f);
	P3FFar(P3F("SecondUnit_B"), -1, 53.f, 54.5f, 4, 3, 1.05f, 1.3f);

// ---- 8.12 to 8.44 km: THE FUNHOUSE. Courts on slots 2 (mod 3) left and 1 (mod 3) right, 40 m out: a unit before each
// (the parade has 1 mod 3 left and 2 mod 3 right, STARRY 0 mod 3).
Zone(TEXT("People p3f: the funhouse"), 8.1174f, 8.4374f);
	P3FAt(P3F("Extras_B"), -1, P3FNear, 6, 2);      P3FAt(P3F("PressPen_A"), -1, P3FNear, 6, 5);
	P3FAt(P3F("SecondUnit_A"), 1, P3FNear, 6, 1);   P3FAt(P3F("DroneTeam_A"), 1, P3FNear, 6, 4);

// ---- 8.44 to 8.91 km: THE FUNHOUSE JUMP. By Far rules 46 m out, off the slots of the main table's tower, crane and
// lights (42 to 43 m): the stunt rigging and a drone team on the left, the long-lens unit on the right.
Zone(TEXT("People p3f: the funhouse jump"), 8.4374f, 8.9074f);
	P3FFar(P3F("StuntRig"), -1, 45.5f, 46.5f, 3, 0, 1.05f);
	P3FFar(P3F("DroneTeam_B"), -1, 45.5f, 46.5f, 3, 2, 1.05f);
	P3FFar(P3F("SecondUnit_B"), 1, 45.5f, 46.5f, 3, 2, 1.05f);

// ---- 8.91 to 9.30 km: THE BACK LOT, the shoot's base camp. Sheds stand 31 to 34 m out on slots 1 (mod 3), barrels at
// 24 to 28 m on the odd slots, the main table's cherry pickers, lights, catering and floats on others: the make-up
// tent, the extras and, from the shared kit, catering and sound in the slots between.
Zone(TEXT("People p3f: the back lot"), 8.9074f, 9.2974f);
	P3FAt(P3F("MakeUp"), -1, P3FNear, 6, 0);        P3FAt(P3FK("Sound"), -1, 35.f, 6, 0, 1.f);
	P3FAt(P3F("Extras_A"), -1, 33.f, 6, 3);
	P3FAt(P3F("Extras_B"), 1, 33.f, 6, 5);          P3FAt(P3FK("Catering"), 1, 34.f, 6, 2, 1.f);

// ---- 9.30 km to the end: THE LAST RIDE (two thirds of the road, and the boss fight). The dressing's sets stand on the
// left's even slots and the right's odd ones; the main table's parade lane and big pieces on the others. In the 24 to
// 31 m band the sets leave these slots free: left 2 and 10 (mod 12: a dock and a court, 40 m out) and 6 (a rocket pad,
// 44 m: its robot is 31.5 m out); right 3, 9 and 11 (a court, a saucer park at 38 m, a dock). The units go round
// those in 24 slots, so on each side there is one every 180 m and the same one every 1080 m. The shared kit's lights,
// crane and sound stand behind three of them (33 to 34 m, as built). The land's sign (27.5 m, every 28th slot) meets
// the make-up tent's slot twice in the level.
Zone(TEXT("People p3f: the last ride"), 9.2974f, 99.f);
	P3FAt(P3F("SecondUnit_A"), -1, P3FNear, 24, 2);  P3FAt(P3F("Extras_A"), -1, P3FNear, 24, 6);
	P3FAt(P3F("MakeUp"), -1, P3FNear, 24, 10);       P3FAt(P3F("PressPen_A"), -1, P3FNear, 24, 14);
	P3FAt(P3F("DroneTeam_A"), -1, P3FNear, 24, 18);  P3FAt(P3F("StuntRig"), -1, P3FNear, 24, 22);
	P3FAt(P3F("DroneTeam_B"), 1, P3FNear, 24, 3);    P3FAt(P3F("PressPen_B"), 1, P3FNear, 24, 9);
	P3FAt(P3F("Dolly"), 1, P3FNear, 24, 11);         P3FAt(P3F("SecondUnit_B"), 1, P3FNear, 24, 15);
	P3FAt(P3F("Extras_B"), 1, P3FNear, 24, 21);      P3FAt(P3F("Applause_B"), 1, P3FNear, 24, 23);
	P3FAt(P3FK("Lights"), -1, 33.5f, 24, 2, 1.f);    P3FAt(P3FK("Crane"), 1, 33.f, 24, 15, 1.f);
	P3FAt(P3FK("Sound"), 1, 33.5f, 24, 11, 1.f);
#undef P3F
#undef P3FK
// ---- END p3f ----
}

{
// ---- BEGIN s1d1 (district 1 "Front Gate and Midway", ft_world_park_s1d1.py) ----
// SHOW STREET. 18 meshes (7 buildings, 6 tall props, 4 low props, 1 hero), all /Game/World/ThemePark/SM_S1D1_*.
// Only even slots (every 4, 8 or 12; phase % 4 == 0 or 2), only 0 to 2.22 km. Nothing blinks; light is steady and only
// on TALL pieces (smallest x min of a TALL piece: 25.4 m, the masts; the front of their bounds 24.4 m).
// Estimate (slots = 22.1 x km; x 0.75 for what the track leaves out; x parts in the set):
//   buildings, front gate   8 Sites, every 8, 0 to 1.15 km (25.4 slots):   8 x 25.4/8 x 0.75 x 2 parts     = 38.1
//   buildings, midway       4 Sites, every 4, 1.15 to 1.84 km (15.25):     4 x 15.25/4 x 0.75 x 2          = 22.9
//   buildings, big jump     2 Sites, every 4, 1.84 to 2.22 km (8.4):       2 x 8.4/4 x 0.75 x 2            =  6.3
//   masts                   10 Sites, every 12, 0 to 1.04 km (23.0):       10 x 23/12 x 0.75 x 2           = 28.8
//   booths, arches          8 Sites, every 12, 0 to 1.04 km:               8 x 23/12 x 0.75 x 2            = 23.0
//   kerb, front gate        4 Sites, every 4, 0 to 1.04 km:                23/4 x 0.75 x (4 + 4 + 3 + 3)   = 60.4
//   kerb, midway            4 Sites, every 4, 1.15 to 1.57 km (9.28):      9.28/4 x 0.75 x (4 + 4 + 3 + 3) = 24.4
//   hero                    2 one-slot Sites                                                               =  2
//   sum about 206 placed (counted slot by slot, if the track leaves nothing out: 270). Most of one mesh: Turnstiles and PosterBoards
//   about 24 each (32 at the most); a building about 9 to 10.5 (14 at the most); the hero twice.
// Bands and slots used, and what stands there already (read from FTDressL4.inl, FTPeopleL4.inl and the sizes in the
// make logs; NOT seen in the game):
//   17.1 to 18.0 m  LOW PROPS (anchor 17.55 m, each 0.9 m deep or less, tops 1.38 to 1.89 m) before the show hoarding
//               (SM_ParkFence at 19 m: the front of its planter is 18.1 m). Left: every even slot has the hoarding along
//               the whole slot. Right: the even slots have SeatsRun (benches at -13 m along, hoarding from -1 to +19 m):
//               the right sets stand only before that hoarding, +0.5 to +18.5 m along. If a verge rule of the Front Gate
//               or the Midway is changed in FTDressL4.inl (every, phase, side), change these.
//   22.25 to 23.15 m  the same LOW PROPS on the Midway's left (anchor 22.7 m), behind QueueLine (rails and benches to
//               21.5 m) and before the high wire (front 24.5 m)
//   25.4 m      MASTS, a pair a slot at -21 and +21 m along (the seams between the slots' own pieces: parade floats, candy
//               carts and stall rows reach 18.2 m along at the most). Not on the gate's own slots (left s % 12 == 4,
//               right s % 12 == 10: SM_PlazaSeats stands there)
//   26.2 m      BOOTHS (a ticket kiosk and a weighing chair) on s % 12 == 2 and 8, the candy carts' slots (27 to 33 m,
//               chance 0.6: a cart can stand against a booth; not avoided)
//   39.3 m      ARCHES (a bulb arch and a prize rack at -12 and +12 m along) on s % 12 == 2 and 8, between the podiums
//               and the high wire (34.5 m, backs to 38.2 m) and SM_P3A_Painters / SM_P3A_Photo (43 m, 4.9 m either side
//               of the slot's middle)
//   54.5 m      BUILDINGS of the Front Gate, a pair a slot at -22 and +22 m along (17.2 to 26.8 m: the fountains and
//               bandstands of the even slots reach 16.8 m along, the stages, stands and rings of the odd slots come to
//               26.7 m from this slot's middle). Fronts 51.0 m (the ice parlour's balcony 50.6), backs 57.9 m: behind the
//               gate (46 m), the watch towers (to 52.1 m, 4.1 m along) and SM_P3A_Train; before the high ways (58.4 m on)
//   57.5 m      BUILDINGS of the Midway, the same pairs, behind the second row of stalls (to 49 m) and the monorail (52 m),
//               before the mascots, bandstands and high ways (60 m on)
//   59 m        BUILDINGS beside the big jump's rise (the track leaves the flight's slots out)
//   100 / 156 m THE GREAT ORGAN, right: slot 10 (0.452 km) at 100 m, 34 m tall, before the wooden coasters (108 m on);
//               slot 34 (1.538 km) at 156 m and 1.3 times the size (44 m tall), behind the big tops (110 to 140 m) and
//               SM_P3C_Parrots (96 to 112 m), before the lattice coaster (170 m on)
// Near things (under 40 m) stop at 1.04 km and at 1.57 km: the bends at 1.09, 1.63 and 1.76 km are banked.
#define S1D1(n) TEXT("/Game/World/ThemePark/SM_S1D1_" n)
// the show buildings, two a slot, one in each seam (anchor 54.5, 57.5 or 59 m)
Set(TEXT("WldS1D1RowA"), { { S1D1("BoxOffice"),    -22.f, 0.f,  0.f, 0.f, 1.f }, { S1D1("PennyArcade"),  22.f, 0.f,  0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1RowB"), { { S1D1("MirrorMaze"),   -22.f, 0.f,  0.f, 0.f, 1.f }, { S1D1("ShootGallery"), 22.f, 0.f,  0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1RowC"), { { S1D1("IceParlour"),   -22.f, 0.6f, 0.f, 0.f, 1.f }, { S1D1("CostumeShop"),  22.f, 0.f,  0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1RowD"), { { S1D1("PhotoStudio"),  -22.f, 0.f,  0.f, 0.f, 1.f }, { S1D1("BoxOffice"),    22.f, 0.f,  0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1RowE"), { { S1D1("PennyArcade"),  -22.f, 0.f,  0.f, 0.f, 1.f }, { S1D1("MirrorMaze"),   22.f, 0.f,  0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1RowF"), { { S1D1("ShootGallery"), -22.f, 0.f,  0.f, 0.f, 1.f }, { S1D1("IceParlour"),   22.f, 0.6f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1RowG"), { { S1D1("CostumeShop"),  -22.f, 0.f,  0.f, 0.f, 1.f }, { S1D1("PhotoStudio"),  22.f, 0.f,  0.f, 0.f, 1.f } });
// the street's furniture, TALL (anchors 25.4, 26.2 and 39.3 m)
Set(TEXT("WldS1D1MastsA"), { { S1D1("BannerMast"), -21.f, 0.f, 0.f, 0.f, 1.f }, { S1D1("BulbPost"),   21.f, 0.f,  0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1MastsB"), { { S1D1("BulbPost"),   -21.f, 0.f, 0.f, 0.f, 1.f }, { S1D1("BannerMast"), 21.f, 0.f,  0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1Booth"),  { { S1D1("TicketKiosk"), -5.f, 0.f, 0.f, 0.f, 1.f }, { S1D1("WeighChair"), 4.5f, 0.3f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1Arch"),   { { S1D1("BulbArch"),   -12.f, 0.f, 0.f, 0.f, 1.f }, { S1D1("PrizeRack"),  12.f, 0.2f, 0.f, 0.f, 1.f } });
// the kerb, LOW (anchor 17.55 m; on the Midway's left 22.7 m)
Set(TEXT("WldS1D1KerbA"),  { { S1D1("Turnstiles"), -14.5f, 0.f, 0.f, 0.f, 1.f }, { S1D1("PosterBoards"), -5.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D1("BarrelOrgan"), 3.f, 0.f, 0.f, 0.f, 1.f }, { S1D1("RingToss"), 10.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1KerbA2"), { { S1D1("RingToss"), -13.f, 0.f, 0.f, 0.f, 1.f }, { S1D1("BarrelOrgan"), -6.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D1("Turnstiles"), 2.5f, 0.f, 0.f, 0.f, 1.f }, { S1D1("PosterBoards"), 13.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1KerbB"),  { { S1D1("PosterBoards"), 3.5f, 0.f, 0.f, 0.f, 1.f }, { S1D1("BarrelOrgan"), 9.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D1("Turnstiles"), 15.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1KerbB2"), { { S1D1("Turnstiles"), 4.f, 0.f, 0.f, 0.f, 1.f }, { S1D1("RingToss"), 10.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D1("PosterBoards"), 15.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1Organ"),  { { S1D1("GreatOrgan"), 0.f, 0.f, 0.f, 0.f, 1.f } });
// ---- 0 to 1.15 km: the Front Gate's show street (left s % 8 == 0, 2, 4, 6: A, B, C, D; right: G, A, E, F)
Zone(TEXT("World s1d1, Show Street: the front gate"), 0.f, 1.15f);
	Site(TEXT("WldS1D1RowA"), -1, 54.5f, 8, 0, 1.f);
	Site(TEXT("WldS1D1RowB"), -1, 54.5f, 8, 6, 1.f);
	Site(TEXT("WldS1D1RowC"), -1, 54.5f, 8, 4, 1.f);
	Site(TEXT("WldS1D1RowD"), -1, 54.5f, 8, 2, 1.f);
	Site(TEXT("WldS1D1RowG"), 1, 54.5f, 8, 0, 1.f);
	Site(TEXT("WldS1D1RowA"), 1, 54.5f, 8, 6, 1.f);
	Site(TEXT("WldS1D1RowE"), 1, 54.5f, 8, 4, 1.f);
	Site(TEXT("WldS1D1RowF"), 1, 54.5f, 8, 2, 1.f);
// ---- 0 to 1.04 km: masts, booths, arches and the kerb (they stop before the banked bend at 1.09 km)
Zone(TEXT("World s1d1, Show Street: the front gate, near"), 0.f, 1.04f);
	Site(TEXT("WldS1D1MastsA"), -1, 25.4f, 12, 0, 1.f);     // left: s % 12 == 0, 2, 6, 8, 10 (4 is the gate's forecourt)
	Site(TEXT("WldS1D1MastsB"), -1, 25.4f, 12, 10, 1.f);
	Site(TEXT("WldS1D1MastsA"), -1, 25.4f, 12, 6, 1.f);
	Site(TEXT("WldS1D1MastsB"), -1, 25.4f, 12, 4, 1.f);
	Site(TEXT("WldS1D1MastsA"), -1, 25.4f, 12, 2, 1.f);
	Site(TEXT("WldS1D1MastsB"), 1, 25.4f, 12, 0, 1.f);      // right: s % 12 == 0, 2, 4, 6, 8 (10 is the gate's forecourt)
	Site(TEXT("WldS1D1MastsA"), 1, 25.4f, 12, 10, 1.f);
	Site(TEXT("WldS1D1MastsB"), 1, 25.4f, 12, 8, 1.f);
	Site(TEXT("WldS1D1MastsA"), 1, 25.4f, 12, 6, 1.f);
	Site(TEXT("WldS1D1MastsB"), 1, 25.4f, 12, 4, 1.f);
	Site(TEXT("WldS1D1Booth"), -1, 26.2f, 12, 10, 1.f);     // s % 12 == 2 and 8, both sides
	Site(TEXT("WldS1D1Booth"), -1, 26.2f, 12, 4, 1.f);
	Site(TEXT("WldS1D1Booth"), 1, 26.2f, 12, 10, 1.f);
	Site(TEXT("WldS1D1Booth"), 1, 26.2f, 12, 4, 1.f);
	Site(TEXT("WldS1D1Arch"), -1, 39.3f, 12, 10, 1.f);
	Site(TEXT("WldS1D1Arch"), -1, 39.3f, 12, 4, 1.f);
	Site(TEXT("WldS1D1Arch"), 1, 39.3f, 12, 10, 1.f);
	Site(TEXT("WldS1D1Arch"), 1, 39.3f, 12, 4, 1.f);
	Site(TEXT("WldS1D1KerbA"), -1, 17.55f, 4, 0, 1.f);
	Site(TEXT("WldS1D1KerbA2"), -1, 17.55f, 4, 2, 1.f);
	Site(TEXT("WldS1D1KerbB"), 1, 17.55f, 4, 0, 1.f);
	Site(TEXT("WldS1D1KerbB2"), 1, 17.55f, 4, 2, 1.f);
// ---- 1.15 to 1.84 km: the Midway. Buildings only, behind the second row of stalls (the track leaves the corkscrew out)
Zone(TEXT("World s1d1, Show Street: the midway"), 1.15f, 1.84f);
	Site(TEXT("WldS1D1RowF"), -1, 57.5f, 4, 0, 1.f);
	Site(TEXT("WldS1D1RowG"), -1, 57.5f, 4, 2, 1.f);
	Site(TEXT("WldS1D1RowB"), 1, 57.5f, 4, 0, 1.f);
	Site(TEXT("WldS1D1RowC"), 1, 57.5f, 4, 2, 1.f);
// ---- 1.15 to 1.57 km: the Midway's kerb (it stops before the banked bends at 1.63 and 1.76 km)
Zone(TEXT("World s1d1, Show Street: the midway, near"), 1.15f, 1.57f);
	Site(TEXT("WldS1D1KerbA"), -1, 22.7f, 4, 0, 1.f);
	Site(TEXT("WldS1D1KerbA2"), -1, 22.7f, 4, 2, 1.f);
	Site(TEXT("WldS1D1KerbB"), 1, 17.55f, 4, 0, 1.f);
	Site(TEXT("WldS1D1KerbB2"), 1, 17.55f, 4, 2, 1.f);
// ---- 1.84 to 2.22 km: the big jump. A pair each side on the rise (s % 4 == 2), where the track lets them stand
Zone(TEXT("World s1d1, Show Street: the big jump"), 1.84f, 2.22f);
	Site(TEXT("WldS1D1RowD"), -1, 59.f, 4, 2, 1.f);
	Site(TEXT("WldS1D1RowE"), 1, 59.f, 4, 2, 1.f);
// ---- THE GREAT ORGAN, right, one slot each: slot 10 (0.452 km) and slot 34 (1.538 km)
Zone(TEXT("World s1d1, the Great Organ at the gate"), 0.4494f, 0.4554f);
	Site(TEXT("WldS1D1Organ"), 1, 100.f, 4, 2, 1.f);
Zone(TEXT("World s1d1, the Great Organ on the midway"), 1.5352f, 1.5412f);
	Site(TEXT("WldS1D1Organ"), 1, 156.f, 4, 2, 1.f, 1.3f);
#undef S1D1
// ---- END s1d1 ----
}

{
// ---- BEGIN s1d2 (district 2 "Pirate Cove", ft_world_park_s1d2.py) ----
// A STILT HARBOUR, 2.22 to 3.21 km (slots 50 to 70, the even ones only). 16 meshes, all /Game/World/ThemePark/SM_S1D2_*:
// 7 buildings (HarbourTower, SailLoft, Chandler, BoatShed, NetHouse, MapHouse, LookoutInn), 4 tall props (Figurehead,
// SignalMast, FishRack, WheelArch), 4 low props (Pots, Capstan, RowBoat, Cannon: unlit, still) and the hero, BottleShip.
// Every rule names ONE slot (every 1000, phase 1000 - slot; the phases are 0 or 2 mod 4), chance 1, no jitter: what
// stands where was read from FTDressL4.inl and FTPeopleL4.inl, not seen in the game.
// Estimate: 38 Site rules place 192 parts and 10 Scatter rules 10 (x 0.75 for what the track leaves out = 151.5), 8 Far rules
// place 8 (x 1): about 160 placed (210 if the track leaves nothing out).
// By mesh (nothing left out): HarbourTower 12, SailLoft 10, Chandler 10, BoatShed 11, NetHouse 11, MapHouse 11, LookoutInn 10, Figurehead 25, SignalMast 19, FishRack 22, WheelArch 19, Pots 14, Capstan 10, RowBoat 14, Cannon 10, BottleShip 2.
// Most of one mesh: Figurehead 25. Hero: 2 placements. NOT compiled, NOT seen in the game.
// Bands (m from the centre line) and the slots they take:
//   23.0-23.4  LOW props on the quay, seven in a row (behind the verge at 19 m, before the parade lane at 26 m):
//              left 50, 52, 58; right 50, 56, 58 (not on the inside of the two hairpins, where the verge is a cutting)
//   24.0       the lift hill: one low prop a side on 66, 68, 70 (single pieces, before STARRY at 27 m)
//   31         tall props (Landing) where a scene of p3b stands 40 m out: left 50, 58; right 56
//   33-34.5    WHARF, a row of three houses and two props, where the high wire stands 44 m out: left 52; right 50, 58
//   46.6       tall props (Yard) between the p3b scenes (40 m) and the show stages (52 m): left 50, 54, 58; right 52, 56
//   51.5-52.5  STILTS, a row between the high wire (44 m) and the rope bridge / castle wall (60 m): left 52, 56; right 50, 54, 58
//   59.4-60.4  LANE, a row behind the show stages (52 m): left 50, 54, 58; right 52, 56
//   67.3-69.4  BACK and REAR rows: left 50, 52, 56, 58; right 50, 52, 54, 56, 58 (not left 54: the galleon, 80 m)
//   the loop (60, 62, 64) by Far rules: left a house 57.5 m and a tall prop 41 m out; right a house 70 m out on 60, 64
//   the lift hill: a house 70 m out, left 68, 70, right 66, 68
//   HERO: left, 96 m out on slot 50 (outside of the first hairpin) and 126 m out on slot 66 (the lift hill).
// Smallest x of a TALL mesh: 31.0 m (Figurehead in Landing: anchor 31, out 0). Smallest x of a low mesh: 23.0 m.
// Light: steady only, on the buildings, tall props and hero (all 24 m and more out). Nothing blinks.
#define S1D2(n) TEXT("/Game/World/ThemePark/SM_S1D2_" n)
#define S1D2_SITE(set, side, x, slot) Site(TEXT("WldS1D2" set), side, x, 1000, 1000 - (slot), 1.f)
#define S1D2_ONE(mesh, side, x, slot) Scatter(S1D2(mesh), x, x, 1000, 1000 - (slot), 1.f, 1.f, 0.f, side, 0.f)
#define S1D2_FAR(mesh, side, x, slot, drop) Far(S1D2(mesh), x, x, 1000, 1000 - (slot), 1.f, 1.f, 0.f, side, drop, 0.f)
Set(TEXT("WldS1D2Quay1"), {                       // 41.4 m along
	{ S1D2("Pots"), -19.1f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("RowBoat"), -12.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Capstan"), -5.5f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Cannon"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Pots"), 5.4f, 0.0f, 0.f, 180.0f, 1.f },
	{ S1D2("RowBoat"), 12.2f, 0.0f, 0.f, 180.0f, 1.f },
	{ S1D2("Capstan"), 19.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Quay2"), {                       // 40.5 m along
	{ S1D2("Cannon"), -19.1f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Capstan"), -13.6f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("RowBoat"), -6.8f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Pots"), 0.1f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Cannon"), 5.5f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Pots"), 10.9f, 0.0f, 0.f, 180.0f, 1.f },
	{ S1D2("RowBoat"), 17.7f, 0.0f, 0.f, 180.0f, 1.f },
});
Set(TEXT("WldS1D2Landing"), {                       // 37.6 m along
	{ S1D2("Figurehead"), -17.6f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("FishRack"), -11.4f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("WheelArch"), -3.5f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("SignalMast"), 4.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Figurehead"), 10.2f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("FishRack"), 16.4f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Yard"), {                       // 33.2 m along
	{ S1D2("SignalMast"), -14.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("FishRack"), -4.6f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Figurehead"), 4.1f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("WheelArch"), 13.5f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Wharf1"), {                       // 42.1 m along
	{ S1D2("Chandler"), -16.4f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("FishRack"), -8.3f, -1.5f, 0.f, 0.0f, 1.f },
	{ S1D2("MapHouse"), -0.5f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Figurehead"), 6.1f, -1.5f, 0.f, 0.0f, 1.f },
	{ S1D2("BoatShed"), 14.7f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Wharf2"), {                       // 42.4 m along
	{ S1D2("SailLoft"), -15.1f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("SignalMast"), -5.8f, -1.5f, 0.f, 0.0f, 1.f },
	{ S1D2("HarbourTower"), 1.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("FishRack"), 8.1f, -1.5f, 0.f, 0.0f, 1.f },
	{ S1D2("LookoutInn"), 16.4f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Stilts1"), {                       // 38.8 m along
	{ S1D2("NetHouse"), -13.9f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("SignalMast"), -5.2f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("HarbourTower"), 1.7f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Figurehead"), 7.5f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("LookoutInn"), 14.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Stilts2"), {                       // 44.0 m along
	{ S1D2("MapHouse"), -17.6f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("WheelArch"), -9.2f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("NetHouse"), 0.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("FishRack"), 9.2f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Chandler"), 17.4f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Back1"), {                       // 41.2 m along
	{ S1D2("SailLoft"), -14.6f, 0.5f, 0.f, 0.0f, 1.f },
	{ S1D2("Figurehead"), -6.3f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Chandler"), 0.6f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("WheelArch"), 9.4f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("HarbourTower"), 17.1f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Back2"), {                       // 44.6 m along
	{ S1D2("BoatShed"), -15.9f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("SignalMast"), -6.3f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("LookoutInn"), 1.9f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("FishRack"), 10.2f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("MapHouse"), 18.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Back3"), {                       // 44.1 m along
	{ S1D2("SailLoft"), -16.0f, 0.5f, 0.f, 0.0f, 1.f },
	{ S1D2("FishRack"), -6.7f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("BoatShed"), 2.9f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Figurehead"), 11.3f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("MapHouse"), 17.7f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Back4"), {                       // 43.9 m along
	{ S1D2("HarbourTower"), -18.4f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("WheelArch"), -11.5f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("SailLoft"), -2.0f, 0.5f, 0.f, 0.0f, 1.f },
	{ S1D2("SignalMast"), 6.6f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("BoatShed"), 15.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Lane1"), {                       // 42.4 m along
	{ S1D2("MapHouse"), -16.9f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("FishRack"), -9.1f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("NetHouse"), -0.1f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("SignalMast"), 8.6f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Chandler"), 16.6f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Lane2"), {                       // 42.2 m along
	{ S1D2("HarbourTower"), -17.5f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("WheelArch"), -9.9f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("BoatShed"), 0.6f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Figurehead"), 9.2f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("LookoutInn"), 16.2f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Rear1"), {                       // 41.6 m along
	{ S1D2("SailLoft"), -14.8f, 0.4f, 0.f, 0.0f, 1.f },
	{ S1D2("Figurehead"), -6.5f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("LookoutInn"), 0.6f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("WheelArch"), 9.6f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("HarbourTower"), 17.2f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Rear2"), {                       // 43.8 m along
	{ S1D2("Chandler"), -17.2f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("SignalMast"), -9.8f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("NetHouse"), -1.5f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("FishRack"), 6.9f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("SailLoft"), 15.9f, 0.4f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Bottle"), {                       // 53.5 m along
	{ S1D2("BottleShip"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Zone(TEXT("World s1d2, Pirate Cove"), 2.22f, 3.21f);
	// slot 50 (2.262 km)
	S1D2_SITE("Quay1", -1, 23.4f, 50);
	S1D2_SITE("Landing", -1, 31.0f, 50);
	S1D2_SITE("Yard", -1, 46.6f, 50);
	S1D2_SITE("Lane1", -1, 60.4f, 50);
	S1D2_SITE("Rear1", -1, 69.0f, 50);
	S1D2_SITE("Bottle", -1, 96.0f, 50);
	S1D2_SITE("Quay2", 1, 23.4f, 50);
	S1D2_SITE("Wharf2", 1, 34.5f, 50);
	S1D2_SITE("Stilts2", 1, 52.5f, 50);
	S1D2_SITE("Back2", 1, 68.3f, 50);
	// slot 52 (2.353 km)
	S1D2_SITE("Quay2", -1, 23.0f, 52);
	S1D2_SITE("Wharf1", -1, 34.5f, 52);
	S1D2_SITE("Stilts1", -1, 52.5f, 52);
	S1D2_SITE("Back1", -1, 68.3f, 52);
	S1D2_SITE("Yard", 1, 46.6f, 52);
	S1D2_SITE("Lane2", 1, 60.4f, 52);
	S1D2_SITE("Rear2", 1, 69.0f, 52);
	// slot 54 (2.443 km)
	S1D2_SITE("Yard", -1, 46.6f, 0);     // fix round: was slot 54, inside the galleon's box (46 to 114 m out). Slot 0 is outside the zone:
	S1D2_SITE("Lane2", -1, 60.4f, 0);    // the two rules place nothing and stay, so every later rule keeps its number and its dice
	S1D2_SITE("Stilts1", 1, 52.5f, 54);
	S1D2_SITE("Back3", 1, 68.3f, 54);
	// slot 56 (2.534 km)
	S1D2_SITE("Stilts2", -1, 52.5f, 56);
	S1D2_SITE("Back4", -1, 68.3f, 56);
	S1D2_SITE("Quay1", 1, 23.4f, 56);
	S1D2_SITE("Landing", 1, 31.0f, 56);
	S1D2_SITE("Yard", 1, 46.6f, 56);
	S1D2_SITE("Lane1", 1, 60.4f, 56);
	S1D2_SITE("Rear1", 1, 69.0f, 56);
	// slot 58 (2.624 km)
	S1D2_SITE("Quay1", -1, 23.4f, 58);
	S1D2_SITE("Landing", -1, 31.0f, 58);
	S1D2_SITE("Yard", -1, 46.6f, 58);
	S1D2_SITE("Lane2", -1, 60.4f, 58);
	S1D2_SITE("Rear2", -1, 69.0f, 58);
	S1D2_SITE("Quay2", 1, 23.4f, 58);
	S1D2_SITE("Wharf1", 1, 34.5f, 58);
	S1D2_SITE("Stilts1", 1, 52.5f, 58);
	S1D2_SITE("Back1", 1, 68.3f, 58);
	// slot 66 (2.986 km)
	S1D2_SITE("Bottle", -1, 126.0f, 66);
	// the loop, 2.665 to 2.945 km: Far rules (drop = the land's depth under the road there)
	S1D2_FAR("BoatShed", -1, 57.5f, 60, 1.1f);
	S1D2_FAR("MapHouse", -1, 57.5f, 62, 1.1f);
	S1D2_FAR("NetHouse", -1, 57.5f, 64, 1.1f);
	S1D2_FAR("SignalMast", -1, 41.0f, 60, 1.0f);
	S1D2_FAR("WheelArch", -1, 41.0f, 62, 1.0f);
	S1D2_FAR("Figurehead", -1, 41.0f, 64, 1.0f);
	S1D2_FAR("MapHouse", 1, 70.0f, 60, 1.2f);
	S1D2_FAR("BoatShed", 1, 70.0f, 64, 1.2f);
	// the lift hill, 2.98 to 3.21 km: single pieces
	S1D2_ONE("Pots", -1, 24.0f, 66);
	S1D2_ONE("RowBoat", 1, 24.0f, 66);
	S1D2_ONE("Capstan", -1, 24.0f, 68);
	S1D2_ONE("Cannon", 1, 24.0f, 68);
	S1D2_ONE("RowBoat", -1, 24.0f, 70);
	S1D2_ONE("Pots", 1, 24.0f, 70);
	// fix round: the four houses below stood 70.0 m out; the same meshes stand 34.5 to 69 m out elsewhere and the brief wants a mesh all inside or all outside 70 m
	S1D2_ONE("BoatShed", -1, 69.5f, 68);
	S1D2_ONE("MapHouse", -1, 69.5f, 70);
	S1D2_ONE("NetHouse", 1, 69.5f, 66);
	S1D2_ONE("SailLoft", 1, 69.5f, 68);
#undef S1D2_FAR
#undef S1D2_ONE
#undef S1D2_SITE
#undef S1D2
// ---- END s1d2 ----
}

{
// ---- BEGIN s1d3 (district 3 "The Lagoon", 3.21 to 6.9074 km, ft_world_park_s1d3.py) ----
// 17 meshes: 1 hero (SeahorseCascade), 7 buildings (Boathouse, JellyTea, DiveTower, ShellStand, PearlStation near; LilyPagoda,
// PumpHouse far), 9 props (ChannelMarker, LanternLine, HighJetty at the rails' height; FishVane far; MooringJetty, PedaloDock,
// LilyFountain on the valley floor; OarRack, ShellBench low, Splashdown only).
// WHERE AND WHY (read from FTTrack.cpp, FTDressL4.inl and FTPeopleL4.inl; nothing seen in the game):
//   THE TRACK'S RULE ON THE COASTER (FTTrack.cpp, RoadSpan and Clear): the coaster's road counts 38.1 m wide either side of the centre
//   line and from 14.4 m under the rails to 32 m over them. A piece that comes up into that height is pushed out to 39.6 m, and one
//   that stands 2 m over it to 48 m (the pass 3 note: "moved off the barrier", RailGang 29 of 35). So NOTHING at the rails' height can
//   stand in the brief's 24 to 40 m band here, and the buildings' band starts at 48 m. Said in the hand-over.
//   What stands at the rails' height already (slots modulo 12, even ones only: Stage 1's district slots):
//     LEFT   0, 6: look-out 51-52, lighthouse 68-69          RIGHT  4, 10: look-out 51-52, bandstand on the floor 44-58
//            2, 8: rail gang 47.5 (pushed out), a coaster           0, 6: a coaster end on 52-116 at along 0, bleachers on the floor
//                  end on 52-116 at along 0                         2, 8: tower stage 52, mast 64-66, log ride 78-80, a platform 42-43
//            4, 10: camera tower 56-57, tower stage 64,             every even slot: a big top somewhere 60-135 (dice; on the floor)
//                  log ride 78-80, bandstand on the floor
//   So, all by Site (a Site has no slide along the road; every lot keeps inside +-22.6 m of its slot's middle):
//     RING lots, anchor 61 m (pieces 6.4 m deep and less: 57.8 to 64.2 m): left 0 and 6, right 4 and 10. Two pavilions joined by
//       the high jetty, a channel marker at one end.
//     RIB lots, anchor 63 m, either side of the coaster that stands end on (it is 6 m wide at along 0; nothing within 8.5 m of it):
//       left 2 and 8 (two pavilions), right 0 and 6 (one). A channel marker and a lantern line (69.5 m at the furthest).
//     FAR lots, anchor 96 m (behind the log ride at 78-80, off the sky ride's line at 86): left 4 and 10, right 2 and 8. One big
//       building and the fish vane; these three meshes stand ONLY here (never inside 70 m). The left rule for slots 10 (mod 12)
//       starts after Log Falls (slot 82, left, 100 m out).
//     FLOOR lots, anchor 32 m (27 to 38 m), on the valley floor under the rails' height, to be looked down on: left on s % 4 == 0
//       beside the lit path that runs out at along 0 (kept 6.5 m clear), right on s % 4 == 2 (a pond may lie there: 6 to 34 m, dice).
//       All three are under 8.6 m tall; where the land comes up to the rails (a coaster section's ends) the track pushes them out.
//   SPLASHDOWN (6.7174 to 6.9074, slots 150 and 152; the road is on the land again, no verge): two LOW unlit props only, 17.5 to
//       22.5 m (the crew's rostrums stand 26.9 m out, planets on posts 28 to 31 m on every slot).
//   THE SEAHORSE CASCADE: left, 128 m out (its pool 107 to 149 m), slots 76, 100 and 136 (3.438, 4.524, 6.153 km): slots s % 4 == 0
//       (the far crowd's theatre stands 118-126 m on the left's s % 4 == 2), off the swing rides (s % 5 == 2, 120-140 m), the
//       coasters end on (left 2 mod 3, to 116 m) and Log Falls; nearer than the lattice coasters (150 m on). 100 and 136 are the
//       middles of hollows 28 and 35, where the dressing's water sheets lie 95 to 295 m out. All three are far lots A: the pagoda
//       and the vane stand 15 and 17 m either side of its middle, before its outer basins only.
// Estimate. By the brief's formula (zone 3.5074 km = 77.5 slots; a Site every 12: 77.5 / 12 x 0.75 = 4.84 placements a rule):
//   ring 4 rules x 4.84 x 4 parts = 77.5; rib left 2 x 4.84 x 4 = 38.8; rib right 2 x 4.84 x 3 = 29.1; far 4 x 4.84 x 2 = 38.8;
//   floor 2 rules x (77.5 / 4 x 0.75 = 14.5) x 3 = 87.2; Splashdown 4 rules x 1.57 = 6.3; hero 3 x 0.75 = 2.3: about 280.
//   By the slot list (even slots 72 to 148 less those in the loops and corkscrews of the dressing's header, 3.95-4.24, 4.84-5.17,
//   5.31-5.59, 6.22-6.55 km: 25 slots stay): ring 16 lots x 4 = 64, rib 9 x 4 + 7 x 3 = 57, far 17 x 2 = 34, floor 24 x 3 = 72,
//   Splashdown 8, hero 3: about 238 placed. Most of one mesh: ChannelMarker 32 by the slot list (38.7 by the formula). Hero: 3.
#define S1D3(n) TEXT("/Game/World/ThemePark/SM_S1D3_" n)
// { mesh, along m, out m, up m, yaw deg, scale }
Set(TEXT("WldS1D3RingA"), {                        // anchor 61 m
	{ S1D3("Boathouse"),     -12.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("HighJetty"),       2.5f,  0.f,  0.f, 0.f, 1.f },
	{ S1D3("JellyTea"),       12.5f,  0.f,  0.f, 0.f, 1.f },
	{ S1D3("ChannelMarker"), -21.5f,  0.f,  0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3RingB"), {
	{ S1D3("ShellStand"),    -11.5f,  0.f,  0.f, 0.f, 1.f },
	{ S1D3("HighJetty"),       0.5f,  0.f,  0.f, 0.f, 1.f },
	{ S1D3("PearlStation"),   12.5f,  0.f,  0.f, 0.f, 1.f },
	{ S1D3("ChannelMarker"),  21.5f,  0.f,  0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3RingC"), {
	{ S1D3("DiveTower"),     -12.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("HighJetty"),      -1.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("Boathouse"),      11.5f,  0.f,  0.f, 0.f, 1.f },
});                                                // fix round: its channel marker taken out (right side only: thin pieces read as noise there)
Set(TEXT("WldS1D3RibA"), {                         // anchor 63 m; the coaster end on at along 0
	{ S1D3("JellyTea"),      -15.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("DiveTower"),      15.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("ChannelMarker"), -21.5f,  0.f,  0.f, 0.f, 1.f },
	{ S1D3("LanternLine"),   -15.f,   6.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3RibB"), {
	{ S1D3("PearlStation"),  -15.5f,  0.f,  0.f, 0.f, 1.f },
	{ S1D3("ShellStand"),     13.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("ChannelMarker"),  21.3f,  0.f,  0.f, 0.f, 1.f },
	{ S1D3("LanternLine"),    13.f,   6.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3RibC"), {
	{ S1D3("Boathouse"),     -14.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("LanternLine"),    16.f,   0.f,  0.f, 0.f, 1.f },
});                                                // fix round: its channel marker taken out (right side only)
Set(TEXT("WldS1D3RibD"), {
	{ S1D3("JellyTea"),       14.f,   0.f,  0.f, 0.f, 1.f },
});                                                // fix round: its channel marker and lantern line taken out (right side only)
Set(TEXT("WldS1D3FarA"), {                         // anchor 96 m
	{ S1D3("LilyPagoda"),    -15.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("FishVane"),       17.f,   0.f,  0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3FarB"), {
	{ S1D3("PumpHouse"),     -13.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("FishVane"),       15.f,   0.f,  0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3FloorL"), {                       // anchor 32 m, on the valley floor; the lit path runs out at along 0
	{ S1D3("MooringJetty"),  -14.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("LilyFountain"),   10.f,  -1.5f, 0.f, 0.f, 1.f },
	{ S1D3("PedaloDock"),     13.f,   4.8f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3FloorR"), {
	{ S1D3("MooringJetty"),   14.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("LilyFountain"),  -10.f,  -1.5f, 0.f, 0.f, 1.f },
	{ S1D3("PedaloDock"),    -13.f,   4.8f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3Hero"), { { S1D3("SeahorseCascade"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Zone(TEXT("World s1d3, The Lagoon"), 3.21f, 6.7174f);                 // slots 71 to 148; a rule takes s where (s + phase) % 12 == 0
	Site(TEXT("WldS1D3RingA"), -1, 61.f, 12, 0, 1.f);                 // left, s % 12 == 0
	Site(TEXT("WldS1D3RingB"), -1, 61.f, 12, 6, 1.f);                 // left, s % 12 == 6
	Site(TEXT("WldS1D3RingC"), 1, 61.f, 12, 8, 1.f);                  // right, s % 12 == 4
	Site(TEXT("WldS1D3RingB"), 1, 61.f, 12, 2, 1.f);                  // right, s % 12 == 10
	Site(TEXT("WldS1D3RibA"), -1, 63.f, 12, 10, 1.f);                 // left, s % 12 == 2
	Site(TEXT("WldS1D3RibB"), -1, 63.f, 12, 4, 1.f);                  // left, s % 12 == 8
	Site(TEXT("WldS1D3RibC"), 1, 63.f, 12, 0, 1.f);                   // right, s % 12 == 0
	Site(TEXT("WldS1D3RibD"), 1, 63.f, 12, 6, 1.f);                   // right, s % 12 == 6
	Site(TEXT("WldS1D3FarA"), -1, 96.f, 12, 8, 1.f);                  // left, s % 12 == 4
	Site(TEXT("WldS1D3FarB"), 1, 96.f, 12, 10, 1.f);                  // right, s % 12 == 2
	Site(TEXT("WldS1D3FarA"), 1, 96.f, 12, 4, 1.f);                   // right, s % 12 == 8
	Site(TEXT("WldS1D3FloorL"), -1, 32.f, 4, 0, 1.f);                 // left, s % 4 == 0
	Site(TEXT("WldS1D3FloorR"), 1, 32.f, 4, 2, 1.f);                  // right, s % 4 == 2
Zone(TEXT("World s1d3, The Lagoon after Log Falls"), 3.73f, 6.7174f); // slots 83 to 148: slot 82 has Log Falls on the left, 100 m out
	Site(TEXT("WldS1D3FarB"), -1, 96.f, 12, 2, 1.f);                  // left, s % 12 == 10
Zone(TEXT("World s1d3, Splashdown"), 6.7174f, 6.9074f);               // slots 149 to 152; low and unlit, on the even ones
	Scatter(S1D3("OarRack"), 18.5f, 19.f, 2, 0, 1.f, 1.f, 0.f, -1, 0.f);     // fix round: was 17.5 to 18.5, the rack's front edge came to 16.52 m (it is 0.98 m deep to the front)
	Scatter(S1D3("ShellBench"), 21.f, 22.5f, 2, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(S1D3("ShellBench"), 18.5f, 19.f, 2, 0, 1.f, 1.f, 0.f, 1, 0.f);   // fix round: was 17.5 to 18.5, the bench's front edge came to 16.96 m
	Scatter(S1D3("OarRack"), 21.f, 22.5f, 2, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("World s1d3, the Seahorse Cascade 3.44"), 3.4355f, 3.4415f);    // slot 76 only
	Site(TEXT("WldS1D3Hero"), -1, 128.f, 1, 0, 1.f);
Zone(TEXT("World s1d3, the Seahorse Cascade 4.52"), 4.5213f, 4.5273f);    // slot 100 only (hollow 28)
	Site(TEXT("WldS1D3Hero"), -1, 128.f, 1, 0, 1.f);
Zone(TEXT("World s1d3, the Seahorse Cascade 6.15"), 6.1500f, 6.1560f);    // slot 136 only (hollow 35)
	Site(TEXT("WldS1D3Hero"), -1, 128.f, 1, 0, 1.f);
#undef S1D3
// ---- END s1d3 ----
}

{
// ---- BEGIN s1d4 (district 4 "Rocket Land and Funhouse", ft_world_park_s1d4.py) ----
// 20 meshes: 9 buildings, 10 props, 1 hero (SM_S1D4_Observatory). TIN-TOY TOWN, 6.9074 to 9.2974 km (slots 153 to 205).
// Every Site stands in a Zone of ONE slot (6 m of road about the slot's own start, as the dressing's "Bend" and "Marquee"
// zones are written), so each Set below is one known place: 28 Sets on 28 slot sides, 199 parts; the jump has 2 Scatter rules
// (5 by the slots they name); the hero 2 Sites. Estimate, by the slots: 199 + 5 + 2 = 206 placed if the track leaves nothing
// out; with the brief's 0.75 for what the track leaves out: (199 + 5) x 0.75 + 2 = about 155. (The brief's "slots / every"
// does not fit a one-slot Zone: its one slot is the rule's own.) Most of one mesh: AerialLamp, 24. Smallest x of a TALL mesh:
// 31.0 m (anchor + out); of a low one (WobblyMirrors, SpringBench, GearBed): 22.6 m. Nothing past 67.0 m but the hero.
// Slots: every 4, phase 0 or 2 (s % 4 == 0 or 2) only. No mesh stands both inside and outside 70 m.
// Where, and why there (read from FTDressL4.inl and FTPeopleL4.inl; NOT seen in the game):
//   ROCKET LAND, even slots 154-162 and 168-172 (the corkscrew 163-166 and the loop 174-179 are left alone: their Far
//   grandstands, towers and saucers fill 46 to 78 m). A rocket pad (44 m, its jet ride to 52.6 m at the slot's middle) or a
//   saucer park (38 m) stands on every even slot and side, the cast at 26 to 28 m, the high ways at 60 to 61 m on slots 4
//   (mod 6). So: two buildings 52.5 m out, 13.5 m either side of the slot's middle (clear of the jet ride's circle); the
//   control tower between them on the saucer sides, or 61 m out behind the pad on slots 0 (mod 6); four tall props at the
//   slot's two ends (21.5 m along: the neighbours' Scatter pieces slide 11 m at most about their own middles, 45 m away),
//   31.5 to 41.5 m out; three low ones 22.6 m out, between the hoarding (19 m) and the parade lane (24.9 m). On the INSIDE
//   of a banked bend (7.01 R, 7.15 L, 7.24 R, 7.60 L, 7.74 R) nothing stands nearer than 42 m and the low props are left out.
//   THE FUNHOUSE, slots 182 and 184: the same tin gone crooked, behind the courts; on the right of 182, where no court
//   stands, a crooked row 35 to 41 m out. Slots 180 and 186 are full from 40 m out (stages, towers, far funhouses): there
//   only a stair to nowhere and a lamp at the slot ends, 34 m out, and three low props (not on the right of 186: a bend).
//   THE FUNHOUSE JUMP (it climbs, jumps and drops: single pieces): crooked towers and houses on the right, 52 to 56 m,
//   between the far stalls (46-49 m) and the bleachers (60-62 m). Its left is full (stands 52.5-54 m, units 42-46.5 m).
//   THE BACK LOT, slots 198-204: prop stores, flats and crooked houses in the gaps the sheds (58 m: left 0 mod 3, right
//   2 mod 3) leave. HERO: right of slot 170 (7.69-7.74 km) 125 m out; left of slot 198 (8.96-9.00 km) 92 m out, between
//   the sheds (to 70 m) and the coaster hills (110-150 m).
#define S1D4(n) TEXT("/Game/World/ThemePark/SM_S1D4_" n)
Set(TEXT("WldS1D4Hero"), { { S1D4("Observatory"), 0.f, 0.f, 0.f, 0.f, 1.f } });   // { mesh, along m, out m, up m, yaw deg, scale }
Set(TEXT("WldS1D4_154L"), {   // anchor 52.5 m: Rocket Land, left of slot 154, a rocket pad; props from 22.6 m
	{ S1D4("ClockworkCafe"),  -13.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("BoxFlats"),        13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("SpareKey"),       -21.5f,  -20.5f, 0.f, 12.0f, 1.f },
	{ S1D4("FuelPump"),        21.5f,  -21.0f, 0.f, -12.0f, 1.f },
	{ S1D4("TinDog"),         -21.5f,  -11.5f, 0.f,  0.0f, 1.f },
	{ S1D4("SignalDish"),      21.5f,  -11.0f, 0.f,  0.0f, 1.f },
	{ S1D4("WobblyMirrors"),  -14.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("SpringBench"),      0.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("GearBed"),         14.0f,  -29.9f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_154R"), {   // anchor 52.5 m: Rocket Land, right of slot 154, a saucer park; INSIDE a banked bend: nothing nearer than 42 m
	{ S1D4("RocketGarage"),   -13.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("Planetarium"),     13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("ControlTower"),     0.0f,    1.0f, 0.f,  0.0f, 1.f },
	{ S1D4("FuelPump"),       -21.5f,  -10.5f, 0.f, 12.0f, 1.f },
	{ S1D4("TinDog"),          21.5f,  -10.0f, 0.f, -12.0f, 1.f },
	{ S1D4("SignalDish"),     -21.5f,   -4.5f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),      21.5f,   -4.0f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_156L"), {   // anchor 52.5 m: Rocket Land, left of slot 156, a saucer park; props from 22.6 m
	{ S1D4("BoxFlats"),       -13.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("Planetarium"),     13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("ControlTower"),     0.0f,    1.0f, 0.f,  0.0f, 1.f },
	{ S1D4("TinDog"),         -21.5f,  -20.5f, 0.f, 12.0f, 1.f },
	{ S1D4("SignalDish"),      21.5f,  -21.0f, 0.f, -12.0f, 1.f },
	{ S1D4("AerialLamp"),     -21.5f,  -11.5f, 0.f,  0.0f, 1.f },
	{ S1D4("SpareKey"),        21.5f,  -11.0f, 0.f,  0.0f, 1.f },
	{ S1D4("GearBed"),        -14.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("WobblyMirrors"),    0.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("SpringBench"),     14.0f,  -29.9f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_156R"), {   // anchor 52.5 m: Rocket Land, right of slot 156, a rocket pad; INSIDE a banked bend: nothing nearer than 42 m
	{ S1D4("Planetarium"),    -13.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("ClockworkCafe"),   13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("ControlTower"),     0.0f,    8.5f, 0.f,  0.0f, 1.f },
	{ S1D4("SignalDish"),     -21.5f,  -10.5f, 0.f, 12.0f, 1.f },
	{ S1D4("AerialLamp"),      21.5f,  -10.0f, 0.f, -12.0f, 1.f },
	{ S1D4("SpareKey"),       -21.5f,   -4.5f, 0.f,  0.0f, 1.f },
	{ S1D4("FuelPump"),        21.5f,   -4.0f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_158L"), {   // anchor 52.5 m: Rocket Land, left of slot 158, a rocket pad; INSIDE a banked bend: nothing nearer than 42 m
	{ S1D4("RocketGarage"),   -13.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("ClockworkCafe"),   13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),     -21.5f,  -10.5f, 0.f, 12.0f, 1.f },
	{ S1D4("SpareKey"),        21.5f,  -10.0f, 0.f, -12.0f, 1.f },
	{ S1D4("FuelPump"),       -21.5f,   -4.5f, 0.f,  0.0f, 1.f },
	{ S1D4("TinDog"),          21.5f,   -4.0f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_158R"), {   // anchor 52.5 m: Rocket Land, right of slot 158, a saucer park; props from 22.6 m
	{ S1D4("BoxFlats"),       -13.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("RocketGarage"),    13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("ControlTower"),     0.0f,    1.0f, 0.f,  0.0f, 1.f },
	{ S1D4("SpareKey"),       -21.5f,  -20.5f, 0.f, 12.0f, 1.f },
	{ S1D4("FuelPump"),        21.5f,  -21.0f, 0.f, -12.0f, 1.f },
	{ S1D4("TinDog"),         -21.5f,  -11.5f, 0.f,  0.0f, 1.f },
	{ S1D4("SignalDish"),      21.5f,  -11.0f, 0.f,  0.0f, 1.f },
	{ S1D4("GearBed"),        -14.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("WobblyMirrors"),    0.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("SpringBench"),     14.0f,  -29.9f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_160L"), {   // anchor 52.5 m: Rocket Land, left of slot 160, a saucer park; props from 22.6 m
	{ S1D4("BoxFlats"),       -13.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("ClockworkCafe"),   13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("ControlTower"),     0.0f,    1.0f, 0.f,  0.0f, 1.f },
	{ S1D4("FuelPump"),       -21.5f,  -20.5f, 0.f, 12.0f, 1.f },
	{ S1D4("TinDog"),          21.5f,  -21.0f, 0.f, -12.0f, 1.f },
	{ S1D4("SignalDish"),     -21.5f,  -11.5f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),      21.5f,  -11.0f, 0.f,  0.0f, 1.f },
	{ S1D4("WobblyMirrors"),  -14.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("SpringBench"),      0.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("GearBed"),         14.0f,  -29.9f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_160R"), {   // anchor 52.5 m: Rocket Land, right of slot 160, a rocket pad; INSIDE a banked bend: nothing nearer than 42 m
	{ S1D4("Planetarium"),    -13.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("RocketGarage"),    13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("TinDog"),         -21.5f,  -10.5f, 0.f, 12.0f, 1.f },
	{ S1D4("SignalDish"),      21.5f,  -10.0f, 0.f, -12.0f, 1.f },
	{ S1D4("AerialLamp"),     -21.5f,   -4.5f, 0.f,  0.0f, 1.f },
	{ S1D4("SpareKey"),        21.5f,   -4.0f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_162L"), {   // anchor 52.5 m: Rocket Land, left of slot 162, a rocket pad; props from 22.6 m
	{ S1D4("ClockworkCafe"),  -13.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("RocketGarage"),    13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("ControlTower"),     0.0f,    8.5f, 0.f,  0.0f, 1.f },
	{ S1D4("SignalDish"),     -21.5f,  -20.5f, 0.f, 12.0f, 1.f },
	{ S1D4("AerialLamp"),      21.5f,  -21.0f, 0.f, -12.0f, 1.f },
	{ S1D4("SpareKey"),       -21.5f,  -11.5f, 0.f,  0.0f, 1.f },
	{ S1D4("FuelPump"),        21.5f,  -11.0f, 0.f,  0.0f, 1.f },
	{ S1D4("GearBed"),        -14.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("WobblyMirrors"),    0.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("SpringBench"),     14.0f,  -29.9f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_162R"), {   // anchor 52.5 m: Rocket Land, right of slot 162, a saucer park; props from 22.6 m
	{ S1D4("RocketGarage"),   -13.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("BoxFlats"),        13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("ControlTower"),     0.0f,    1.0f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),     -21.5f,  -20.5f, 0.f, 12.0f, 1.f },
	{ S1D4("SpareKey"),        21.5f,  -21.0f, 0.f, -12.0f, 1.f },
	{ S1D4("FuelPump"),       -21.5f,  -11.5f, 0.f,  0.0f, 1.f },
	{ S1D4("TinDog"),          21.5f,  -11.0f, 0.f,  0.0f, 1.f },
	{ S1D4("WobblyMirrors"),  -14.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("SpringBench"),      0.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("GearBed"),         14.0f,  -29.9f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_168L"), {   // anchor 52.5 m: Rocket Land, left of slot 168, a saucer park; INSIDE a banked bend: nothing nearer than 42 m
	{ S1D4("Planetarium"),    -13.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("BoxFlats"),        13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("ControlTower"),     0.0f,    1.0f, 0.f,  0.0f, 1.f },
	{ S1D4("SpareKey"),       -21.5f,  -10.5f, 0.f, 12.0f, 1.f },
	{ S1D4("FuelPump"),        21.5f,  -10.0f, 0.f, -12.0f, 1.f },
	{ S1D4("TinDog"),         -21.5f,   -4.5f, 0.f,  0.0f, 1.f },
	{ S1D4("SignalDish"),      21.5f,   -4.0f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_168R"), {   // anchor 52.5 m: Rocket Land, right of slot 168, a rocket pad; props from 22.6 m
	{ S1D4("ClockworkCafe"),  -13.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("Planetarium"),     13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("ControlTower"),     0.0f,    8.5f, 0.f,  0.0f, 1.f },
	{ S1D4("FuelPump"),       -21.5f,  -20.5f, 0.f, 12.0f, 1.f },
	{ S1D4("TinDog"),          21.5f,  -21.0f, 0.f, -12.0f, 1.f },
	{ S1D4("SignalDish"),     -21.5f,  -11.5f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),      21.5f,  -11.0f, 0.f,  0.0f, 1.f },
	{ S1D4("GearBed"),        -14.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("WobblyMirrors"),    0.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("SpringBench"),     14.0f,  -29.9f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_170L"), {   // anchor 52.5 m: Rocket Land, left of slot 170, a rocket pad; props from 22.6 m
	{ S1D4("BoxFlats"),       -13.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("ClockworkCafe"),   13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("TinDog"),         -21.5f,  -20.5f, 0.f, 12.0f, 1.f },
	{ S1D4("SignalDish"),      21.5f,  -21.0f, 0.f, -12.0f, 1.f },
	{ S1D4("AerialLamp"),     -21.5f,  -11.5f, 0.f,  0.0f, 1.f },
	{ S1D4("SpareKey"),        21.5f,  -11.0f, 0.f,  0.0f, 1.f },
	{ S1D4("WobblyMirrors"),  -14.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("SpringBench"),      0.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("GearBed"),         14.0f,  -29.9f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_170R"), {   // anchor 52.5 m: Rocket Land, right of slot 170, a saucer park; INSIDE a banked bend: nothing nearer than 42 m
	{ S1D4("Planetarium"),    -13.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("RocketGarage"),    13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("ControlTower"),     0.0f,    1.0f, 0.f,  0.0f, 1.f },
	{ S1D4("SignalDish"),     -21.5f,  -10.5f, 0.f, 12.0f, 1.f },
	{ S1D4("AerialLamp"),      21.5f,  -10.0f, 0.f, -12.0f, 1.f },
	{ S1D4("SpareKey"),       -21.5f,   -4.5f, 0.f,  0.0f, 1.f },
	{ S1D4("FuelPump"),        21.5f,   -4.0f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_172L"), {   // anchor 52.5 m: Rocket Land, left of slot 172, a saucer park; props from 22.6 m
	{ S1D4("ClockworkCafe"),  -13.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("Planetarium"),     13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("ControlTower"),     0.0f,    1.0f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),     -21.5f,  -20.5f, 0.f, 12.0f, 1.f },
	{ S1D4("SpareKey"),        21.5f,  -21.0f, 0.f, -12.0f, 1.f },
	{ S1D4("FuelPump"),       -21.5f,  -11.5f, 0.f,  0.0f, 1.f },
	{ S1D4("TinDog"),          21.5f,  -11.0f, 0.f,  0.0f, 1.f },
	{ S1D4("GearBed"),        -14.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("WobblyMirrors"),    0.0f,  -29.9f, 0.f,  0.0f, 1.f },
	{ S1D4("SpringBench"),     14.0f,  -29.9f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_172R"), {   // anchor 52.5 m: Rocket Land, right of slot 172, a rocket pad; INSIDE a banked bend: nothing nearer than 42 m
	{ S1D4("RocketGarage"),   -13.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("ClockworkCafe"),   13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("SpareKey"),       -21.5f,  -10.5f, 0.f, 12.0f, 1.f },
	{ S1D4("FuelPump"),        21.5f,  -10.0f, 0.f, -12.0f, 1.f },
	{ S1D4("TinDog"),         -21.5f,   -4.5f, 0.f,  0.0f, 1.f },
	{ S1D4("SignalDish"),      21.5f,   -4.0f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_182L"), {   // anchor 54.0 m: the Funhouse, left of slot 182, behind the court; INSIDE the bend at 8.28 km: 42 m and out
	{ S1D4("CrookedHouse"),   -12.0f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("MirrorHall"),      11.5f,    1.0f, 0.f,  0.0f, 1.f },
	{ S1D4("CrookedTower"),     0.0f,    9.0f, 0.f,  0.0f, 1.f },
	{ S1D4("NowhereStair"),    21.5f,  -12.0f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),     -21.5f,  -12.0f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_184L"), {   // anchor 52.0 m: the Funhouse, left of slot 184; INSIDE the bend at 8.28 km: 42 m and out
	{ S1D4("MirrorHall"),     -12.0f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("CrookedHouse"),    12.0f,    1.0f, 0.f,  0.0f, 1.f },
	{ S1D4("NowhereStair"),   -21.5f,  -10.0f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),      21.5f,  -10.0f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_182R"), {   // anchor 38.0 m: the Funhouse, right of slot 182 (no court here): a crooked row 35 to 41 m out, the crooked house behind
	{ S1D4("CrookedTower"),   -15.0f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("MirrorHall"),      13.5f,    0.5f, 0.f,  0.0f, 1.f },
	{ S1D4("CrookedHouse"),   -13.0f,   17.0f, 0.f,  0.0f, 1.f },
	{ S1D4("NowhereStair"),     0.0f,   -5.0f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),     -22.0f,   -7.0f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),      22.0f,   -7.0f, 0.f,  0.0f, 1.f },
	{ S1D4("WobblyMirrors"),  -12.0f,  -15.4f, 0.f,  0.0f, 1.f },
	{ S1D4("GearBed"),          0.0f,  -15.4f, 0.f,  0.0f, 1.f },
	{ S1D4("SpringBench"),     12.0f,  -15.4f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_184R"), {   // anchor 54.0 m: the Funhouse, right of slot 184, behind the court
	{ S1D4("CrookedHouse"),   -12.0f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("CrookedTower"),    12.0f,    1.0f, 0.f,  0.0f, 1.f },
	{ S1D4("MirrorHall"),       0.0f,   10.5f, 0.f,  0.0f, 1.f },
	{ S1D4("NowhereStair"),   -21.5f,  -22.0f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),      21.5f,  -22.0f, 0.f,  0.0f, 1.f },
	{ S1D4("WobblyMirrors"),  -12.0f,  -31.4f, 0.f,  0.0f, 1.f },
	{ S1D4("SpringBench"),      0.0f,  -31.4f, 0.f,  0.0f, 1.f },
	{ S1D4("GearBed"),         12.0f,  -31.4f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_180L"), {   // anchor 34.0 m: the Funhouse, left of slot 180 (full from 40 m out): a stair and a lamp at the slot ends, three low props
	{ S1D4("NowhereStair"),   -21.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),      21.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("WobblyMirrors"),  -12.0f,  -11.4f, 0.f,  0.0f, 1.f },
	{ S1D4("SpringBench"),      0.0f,  -11.4f, 0.f,  0.0f, 1.f },
	{ S1D4("GearBed"),         12.0f,  -11.4f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_180R"), {   // anchor 34.0 m: the Funhouse, right of slot 180 (full from 40 m out): a stair and a lamp at the slot ends, three low props
	{ S1D4("NowhereStair"),   -21.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),      21.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("WobblyMirrors"),  -12.0f,  -11.4f, 0.f,  0.0f, 1.f },
	{ S1D4("SpringBench"),      0.0f,  -11.4f, 0.f,  0.0f, 1.f },
	{ S1D4("GearBed"),         12.0f,  -11.4f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_186L"), {   // anchor 34.0 m: the Funhouse, left of slot 186 (full from 40 m out): a stair and a lamp at the slot ends, three low props
	{ S1D4("NowhereStair"),   -21.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),      21.5f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("WobblyMirrors"),  -12.0f,  -11.4f, 0.f,  0.0f, 1.f },
	{ S1D4("SpringBench"),      0.0f,  -11.4f, 0.f,  0.0f, 1.f },
	{ S1D4("GearBed"),         12.0f,  -11.4f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_198R"), {   // anchor 56.0 m: the back lot, right of slot 198
	{ S1D4("PropStore"),      -12.0f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("CrookedHouse"),    12.5f,    2.0f, 0.f,  0.0f, 1.f },
	{ S1D4("CrookedTower"),     0.0f,   11.0f, 0.f,  0.0f, 1.f },
	{ S1D4("StageFlats"),     -21.5f,  -21.0f, 0.f, 10.0f, 1.f },
	{ S1D4("SpareKey"),        21.5f,  -22.0f, 0.f,  0.0f, 1.f },
	{ S1D4("GearBed"),        -10.0f,  -33.4f, 0.f,  0.0f, 1.f },
	{ S1D4("SpringBench"),     10.0f,  -33.4f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_200L"), {   // anchor 60.0 m: the back lot, left of slot 200; INSIDE the bend at 9.09 km
	{ S1D4("MirrorHall"),      12.5f,    1.0f, 0.f,  0.0f, 1.f },
	{ S1D4("StageFlats"),       0.0f,   -2.0f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),       0.0f,   -8.0f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_202L"), {   // anchor 51.0 m: the back lot, left of slot 202; INSIDE the bend at 9.09 km
	{ S1D4("CrookedHouse"),   -12.0f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("PropStore"),       12.0f,    1.0f, 0.f,  0.0f, 1.f },
	{ S1D4("StageFlats"),       0.0f,   -2.0f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),       0.0f,   -8.0f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_202R"), {   // anchor 58.0 m: the back lot, right of slot 202
	{ S1D4("PropStore"),      -12.0f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("CrookedTower"),    12.0f,    1.0f, 0.f,  0.0f, 1.f },
	{ S1D4("StageFlats"),       0.0f,   -2.0f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),       0.0f,    5.0f, 0.f,  0.0f, 1.f },
});
Set(TEXT("WldS1D4_204R"), {   // anchor 57.0 m: the back lot, right of slot 204; INSIDE the bend at 9.23 km
	{ S1D4("CrookedHouse"),   -12.0f,    0.0f, 0.f,  0.0f, 1.f },
	{ S1D4("PropStore"),       12.5f,    1.0f, 0.f,  0.0f, 1.f },
	{ S1D4("CrookedTower"),     0.0f,   10.0f, 0.f,  0.0f, 1.f },
	{ S1D4("StageFlats"),     -22.0f,  -13.0f, 0.f,  0.0f, 1.f },
	{ S1D4("AerialLamp"),      22.0f,  -13.0f, 0.f,  0.0f, 1.f },
});
Zone(TEXT("World s1d4, Rocket Land, slot 154"), 6.964f, 6.97f);
	Site(TEXT("WldS1D4_154L"), -1, 52.5f, 4, 2, 1.f);
	Site(TEXT("WldS1D4_154R"), 1, 52.5f, 4, 2, 1.f);
Zone(TEXT("World s1d4, Rocket Land, slot 156"), 7.0544f, 7.0604f);
	Site(TEXT("WldS1D4_156L"), -1, 52.5f, 4, 0, 1.f);
	Site(TEXT("WldS1D4_156R"), 1, 52.5f, 4, 0, 1.f);
Zone(TEXT("World s1d4, Rocket Land, slot 158"), 7.1449f, 7.1509f);
	Site(TEXT("WldS1D4_158L"), -1, 52.5f, 4, 2, 1.f);
	Site(TEXT("WldS1D4_158R"), 1, 52.5f, 4, 2, 1.f);
Zone(TEXT("World s1d4, Rocket Land, slot 160"), 7.2354f, 7.2414f);
	Site(TEXT("WldS1D4_160L"), -1, 52.5f, 4, 0, 1.f);
	Site(TEXT("WldS1D4_160R"), 1, 52.5f, 4, 0, 1.f);
Zone(TEXT("World s1d4, Rocket Land, slot 162"), 7.3259f, 7.3319f);
	Site(TEXT("WldS1D4_162L"), -1, 52.5f, 4, 2, 1.f);
	Site(TEXT("WldS1D4_162R"), 1, 52.5f, 4, 2, 1.f);
Zone(TEXT("World s1d4, Rocket Land, slot 168"), 7.5973f, 7.6033f);
	Site(TEXT("WldS1D4_168L"), -1, 52.5f, 4, 0, 1.f);
	Site(TEXT("WldS1D4_168R"), 1, 52.5f, 4, 0, 1.f);
Zone(TEXT("World s1d4, Rocket Land, slot 170"), 7.6878f, 7.6938f);
	Site(TEXT("WldS1D4_170L"), -1, 52.5f, 4, 2, 1.f);
	Site(TEXT("WldS1D4_170R"), 1, 52.5f, 4, 2, 1.f);
	Site(TEXT("WldS1D4Hero"), 1, 125.0f, 4, 2, 1.f);   // THE WIND-UP OBSERVATORY
Zone(TEXT("World s1d4, Rocket Land, slot 172"), 7.7783f, 7.7843f);
	Site(TEXT("WldS1D4_172L"), -1, 52.5f, 4, 0, 1.f);
	Site(TEXT("WldS1D4_172R"), 1, 52.5f, 4, 0, 1.f);
Zone(TEXT("World s1d4, the Funhouse, slot 180"), 8.1402f, 8.1462f);
	Site(TEXT("WldS1D4_180L"), -1, 34.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D4_180R"), 1, 34.0f, 4, 0, 1.f);
Zone(TEXT("World s1d4, the Funhouse, slot 182"), 8.2307f, 8.2367f);
	Site(TEXT("WldS1D4_182L"), -1, 54.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D4_182R"), 1, 38.0f, 4, 2, 1.f);
Zone(TEXT("World s1d4, the Funhouse, slot 184"), 8.3212f, 8.3272f);
	Site(TEXT("WldS1D4_184L"), -1, 52.0f, 4, 0, 1.f);
	Site(TEXT("WldS1D4_184R"), 1, 54.0f, 4, 0, 1.f);
Zone(TEXT("World s1d4, the Funhouse, slot 186"), 8.4116f, 8.4176f);
	Site(TEXT("WldS1D4_186L"), -1, 34.0f, 4, 2, 1.f);
Zone(TEXT("World s1d4, the funhouse jump"), 8.4374f, 8.9074f);   // slots 187 to 196
	Scatter(S1D4("CrookedTower"), 53.0f, 56.0f, 4, 0, 1.f, 1.f, 0.f, 1, 4.f);   // right, s % 4 == 0
	Scatter(S1D4("CrookedHouse"), 52.0f, 55.0f, 4, 2, 1.f, 1.f, 0.f, 1, 4.f);   // right, s % 4 == 2
Zone(TEXT("World s1d4, the back lot, slot 198"), 8.9545f, 8.9605f);
	Site(TEXT("WldS1D4_198R"), 1, 56.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D4Hero"), -1, 92.0f, 4, 2, 1.f);   // THE WIND-UP OBSERVATORY
Zone(TEXT("World s1d4, the back lot, slot 200"), 9.045f, 9.051f);
	Site(TEXT("WldS1D4_200L"), -1, 60.0f, 4, 0, 1.f);
Zone(TEXT("World s1d4, the back lot, slot 202"), 9.1355f, 9.1415f);
	Site(TEXT("WldS1D4_202L"), -1, 51.0f, 4, 2, 1.f);
	Site(TEXT("WldS1D4_202R"), 1, 58.0f, 4, 2, 1.f);
Zone(TEXT("World s1d4, the back lot, slot 204"), 9.226f, 9.232f);
	Site(TEXT("WldS1D4_204R"), 1, 57.0f, 4, 0, 1.f);
#undef S1D4
// ---- END s1d4 ----
}

{
// ---- BEGIN s1d5 (district 5 "The Last Ride: Sweet Street", ft_world_park_s1d5.py) ----
// 16 meshes (6 buildings, 9 props, the hero), 9.2974 to 11.94 km = slots 206 to 263, EVEN slots only (s % 4 == 0 or 2).
// Estimate (slots counted one by one from the rules below, x parts, x 0.75): 220 x 0.75 = 165 placed.
//   StreetA 4 slots x 5 = 20, StreetB 5 x 6 = 30, Canes 9 x 2 = 18, Behind 4 x 2 = 8, PairX 4 x 3 = 12, PairY 5 x 2 = 10,
//   Gate 2 x 4 = 8, Jars 5 x 2 = 10, VergeA 14 x 3 = 42, VergeB 15 x 4 = 60, the hero 2.
// The most of one mesh: WaferFence 29, SugarSacks 29, JamJar 21, CaneRow 18 (all others 15 or fewer); the hero 2.
// Where they stand (worked out from both tables by C:/ftwork/jobs/L4s1d5/occ.py: every rule of FTDressL4.inl and
// FTPeopleL4.inl laid on slots 206 to 263; the Last Ride's sets take the left's even slots at 19 to 47.5 m):
//   23 m       the verge's LOW props (unlit, still, tops 1.3 to 1.8 m) behind the kerb people and the queue lines
//              (they end at 22.1 m) and before the cast (front edge 24.1 m); left on slots 2, 4, 8, 10 (mod 12),
//              right on 0, 4, 6, 10 (mod 12): the slots where the verge is not a whole hoarding. Only to 11.23 km:
//              the arena's hill has no verge in the tables above, and none here.
//   25 m       candy-cane rows, left, at the two ends of a stall row's slot (0 and 4 mod 12): the one TALL piece
//              nearer than 40 m (its front edge 24.55 m; it is still, so its bounds do not grow).
//   41.3-51 m  SWEET STREET: three shops in a row behind each stall row on the left (the watchers' decks end at
//              40.2 m), a cone rack, the washing, a liquorice gate in the alleys between them.
//   47.9 m     right, slots 2 (mod 12): two jam-jar towers either side of the stage that stands there (its back
//              45.1 m), under the monorail's beam (the jar is 2 m in radius: 49.9 m; the train's side is 50.85 m).
//   50.3-69 m  left, behind the ticket gates (slots 224 and 236), and at 1.25 times their size behind the docks, the
//              rocket pads and the funhouse courts and in a second row behind the street, ONLY in the slots where
//              the dressing's dice have no bandstand (0 mod 5), fountain (3 mod 5) or far mascot (4 mod 7): hence
//              the rules of every 20, 40, 60, 80 and 120. Right, beyond the monorail: slots 226 and 234 only.
//   124 m      THE GREAT SWIRL, left, slots 224 (10.13 km) and 256 (11.58 km, the arena's hill): the plan's 140 m
//              is 16 m nearer because the parade viaducts of slots 225 and 257 run through 142.5 to 166 m there.
// TALL pieces: smallest x min 25.0 (Canes); the next is 41.3 (the washing in StreetA). LOW pieces: 23.0.
// Steady glow only (0.08 on every colour of the pieces at 25 m and beyond, windows 0.9, lamps 1.3); nothing blinks.
// NOT COMPILED AND NOT SEEN IN THE GAME when written.
#define S1D5(n) TEXT("/Game/World/ThemePark/SM_S1D5_" n)
Set(TEXT("WldS1D5StreetA"), {                         // anchor 46.5 m: wafer house, toffee works, colonnade cafe
	{ S1D5("WaferHouse"),   -15.5f,  0.f,  0.f, 0.f, 1.f },
	{ S1D5("ToffeeWorks"),   -1.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D5("CaneCafe"),      15.5f,  0.f,  0.f, 0.f, 1.f },
	{ S1D5("ConeRack"),      -8.8f, -4.9f, 0.f, 0.f, 1.f },
	{ S1D5("WashLine"),       8.4f, -5.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D5StreetB"), {                         // anchor 46.5 m: sugar mill, fountain hall, jam jar
	{ S1D5("SugarMill"),    -15.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D5("FountainHall"),   0.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D5("JamJar"),        14.2f,  0.5f, 0.f, 0.f, 1.2f },
	{ S1D5("LiquoriceGate"),  8.4f,  0.5f, 0.f, 0.f, 0.8f },
	{ S1D5("ConeRack"),      -8.1f, -3.2f, 0.f, 0.f, 1.f },
	{ S1D5("WashLine"),      19.6f, -2.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D5Canes"), {                           // anchor 25 m: a row of three canes at each end of the slot
	{ S1D5("CaneRow"), -17.f, 0.f, 0.f,   0.f, 1.f },
	{ S1D5("CaneRow"),  17.f, 0.f, 0.f, 180.f, 1.f },
});
Set(TEXT("WldS1D5Behind"), {                          // anchor 62.5 m: the second row behind the street
	{ S1D5("JamJar"),   -11.f, 0.f, 0.f, 0.f, 1.3f },
	{ S1D5("CaneCafe"),   7.f, 0.f, 0.f, 0.f, 1.3f },
});
Set(TEXT("WldS1D5PairX"), {                           // anchor 63 m
	{ S1D5("WaferHouse"),    -12.5f,  0.f, 0.f, 0.f, 1.25f },
	{ S1D5("FountainHall"),    9.5f,  0.f, 0.f, 0.f, 1.25f },
	{ S1D5("LiquoriceGate"),  -1.9f, -2.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D5PairY"), {                           // anchor 63 m
	{ S1D5("ToffeeWorks"), -11.f, 0.f, 0.f, 0.f, 1.25f },
	{ S1D5("SugarMill"),    13.f, 0.f, 0.f, 0.f, 1.25f },
});
Set(TEXT("WldS1D5Gate"), {                            // anchor 55 m: behind a ticket gate (its back 47.5 m)
	{ S1D5("WaferHouse"), -14.5f,  0.f,  0.f, 0.f, 1.f },
	{ S1D5("JamJar"),       0.f,   0.5f, 0.f, 0.f, 1.2f },
	{ S1D5("CaneCafe"),    14.8f,  0.f,  0.f, 0.f, 1.f },
	{ S1D5("ConeRack"),    -6.2f, -3.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D5Jars"), {                            // anchor 47.9 m, right: either side of a stage
	{ S1D5("JamJar"), -13.5f, 0.f, 0.f, 0.f, 1.f },
});                                                   // fix round: the jar at +13.5 stood in SM_SwingRide (FunCourt of the next slot: +6.3 to +24.2 m along, 41 to 59 m out)
Set(TEXT("WldS1D5VergeA"), {                          // anchor 23 m, LOW: round the kerb people (they stand 5 m either side of the middle)
	{ S1D5("WaferFence"), -14.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D5("ChocBench"),   11.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D5("SugarSacks"),  17.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D5VergeB"), {                          // anchor 23 m, LOW
	{ S1D5("SugarCubes"), -17.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D5("WaferFence"),  -7.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D5("SyrupCart"),    4.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D5("SugarSacks"),  13.f, 0.f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1d5, Sweet Street"), 9.2974f, 11.94f);
	Site(TEXT("WldS1D5StreetA"), -1, 46.5f, 12, 0, 1.f);          // left, s % 12 == 0: 216 228 240 252
	Site(TEXT("WldS1D5StreetB"), -1, 46.5f, 12, 8, 1.f);          // left, s % 12 == 4: 208 220 232 244 256
	Site(TEXT("WldS1D5Canes"), -1, 25.f, 12, 0, 1.f);             // the same nine slots
	Site(TEXT("WldS1D5Canes"), -1, 25.f, 12, 8, 1.f);
	Site(TEXT("WldS1D5Behind"), -1, 62.5f, 60, 24, 1.f);          // left, slot 216
	Site(TEXT("WldS1D5Behind"), -1, 62.5f, 60, 8, 1.f);           // 232
	Site(TEXT("WldS1D5Behind"), -1, 62.5f, 60, 56, 1.f);          // 244
	Site(TEXT("WldS1D5Behind"), -1, 62.5f, 60, 48, 1.f);          // 252
	Site(TEXT("WldS1D5PairX"), -1, 63.f, 20, 14, 1.f);            // left, behind a dock or a court: 206 226 246
	Site(TEXT("WldS1D5PairY"), -1, 63.f, 40, 18, 1.f);            // left, behind a rocket pad or a court: 222 262
	Site(TEXT("WldS1D5PairY"), -1, 63.f, 40, 6, 1.f);             // 234
	Site(TEXT("WldS1D5PairY"), -1, 63.f, 80, 66, 1.f);            // 254
	Site(TEXT("WldS1D5Gate"), -1, 55.f, 60, 16, 1.f);             // left, behind the ticket gate of slot 224
	Site(TEXT("WldS1D5Gate"), -1, 55.f, 60, 4, 1.f);              // 236
	Site(TEXT("WldS1D5Jars"), 1, 47.9f, 12, 10, 1.f);             // right, s % 12 == 2: 206 218 230 242 254
	Site(TEXT("WldS1D5PairY"), 1, 63.f, 120, 14, 1.f);            // right, beyond the monorail: 226
	Site(TEXT("WldS1D5PairX"), 1, 63.f, 120, 6, 1.f);             // 234
	Scatter(S1D5("GreatSwirl"), 124.f, 124.f, 32, 0, 1.f, 1.f, 0.f, -1, 0.f);   // the hero, left: 224 and 256
Zone(TEXT("World s1d5, Sweet Street: the verge"), 9.2974f, 11.23f);   // slots 206 to 248
	Site(TEXT("WldS1D5VergeA"), -1, 23.f, 12, 8, 1.f);            // left, kerb people's slots: s % 12 == 4
	Site(TEXT("WldS1D5VergeA"), -1, 23.f, 12, 2, 1.f);            // s % 12 == 10
	Site(TEXT("WldS1D5VergeB"), -1, 23.f, 12, 10, 1.f);           // left, queue lines' slots: s % 12 == 2
	Site(TEXT("WldS1D5VergeB"), -1, 23.f, 12, 4, 1.f);            // s % 12 == 8
	Site(TEXT("WldS1D5VergeA"), 1, 23.f, 12, 0, 1.f);             // right, kerb people's slots: s % 12 == 0
	Site(TEXT("WldS1D5VergeA"), 1, 23.f, 12, 6, 1.f);             // s % 12 == 6
	Site(TEXT("WldS1D5VergeB"), 1, 23.f, 12, 8, 1.f);             // right, seats' slots: s % 12 == 4
	Site(TEXT("WldS1D5VergeB"), 1, 23.f, 12, 2, 1.f);             // s % 12 == 10
#undef S1D5
// ---- END s1d5 ----
}

{
// ---- BEGIN s1d6 (district 6 "The Last Ride: Crescent Keep", 11.94 to 16.1 km, ft_world_park_s1d6.py) ----
// 18 meshes /Game/World/ThemePark/SM_S1D6_*: 9 buildings (RoundTower_A, RoundTower_B, TwinTurrets, Gatehouse, Astronomer, BellTower,
// BridgeHouse, BannerHall, Dovecote), 8 props (GardenWall, Armour, BannerPole, Well, MoonDial, TopiaryKnight, MarketCart, MoonLamp), the hero
// (CrescentKeep). The zone has slots 264 to 355; every rule takes ONE even slot (every 1000, phase 1000 - slot, as the p3b block does:
// s % 4 == 0 or 2), by Site, so a piece stands exactly where it was worked out. NOT COMPILED AND NOT SEEN IN THE GAME when written.
// WHERE, and why (read from FTDressL4.inl, FTPeopleL4.inl and the makers' logs for sizes; nothing measured in the game):
//   COURTS  two to five buildings at a slot's middle, 57.5 to 65 m out, behind the dressing's sets (which reach about 56 m) and before
//           the facade rows (from 70.7 m). LEFT: every even slot but those of the dressing's dice (bandstand s % 5 == 0, fountain
//           s % 5 == 3, big mascot s % 7 == 4, all 54 to 70 m); a gatehouse court at the first and the last, and at 63.5 m in a funhouse
//           court's slot (s % 12 == 10: its chair swing stands 50 m out). RIGHT: only s % 24 == 10 or 18 (elsewhere p3d stands at 62 to
//           64 m, p3c at 61 to 63 m or the rink at 66 to 68 m), clear of the monorail (52 m).
//   GAP LINES  where two slots meet (22.62 m along from a slot's middle): nearly everything else stands at a slot's middle and reaches
//           under 21 m either way, so this line is the freest ground. RIGHT, from every s % 4 == 0 slot, both ways: a low piece 23.0 m
//           out (between the verge, 19 to 22.1 m, and the parade lane, from 24 m), tall slim props 29.5 and 32.5 m, a well or a topiary
//           knight 38.5 m, a thin tower 56.2 m (beyond the monorail, before p3d at 62 m). Left out: the 29.5 m prop where a p3c walkway
//           (42 m long, 26.2 to 29.8 m out) comes over the line; the 32.5 and 38.5 m props beside a gate's fountain or a dock's palm.
//           LEFT, one line a slot (s % 12 == 0 or 4 towards s - 1, s % 4 == 2 towards s + 1; not a gate's slot, s % 12 == 8): one tall
//           prop 31.0 m out (walkways reach 29.8 m, the sky ride's pylon heads start at 32.7 m), every other one with a low piece 23.0 m.
//   BENDS   the banked bends at 15.07 and 15.20 km: no rule puts anything nearer than 40 m on a line between 14.93 and 15.33 km
//           (the towers at 56.2 m and the courts stay). The loop 15.30 to 15.59 km: the track leaves the land's pieces out itself.
//   HERO    THE CRESCENT KEEP three times on the right, 150 m out (135 to 168 m), slots 278, 314, 350 (12.6, 14.2, 15.85 km):
//           s % 3 == 2 (the coaster hills at 160 to 250 m take s % 3 == 0), s % 36 == 26 (no p3e piece of the right side), not the
//           flume's slot 309, the marchers' (odd slots) or the wooden coaster's (s % 8 == 0).
// Light: steady only. Glow on the tall pieces alone (windows, lanterns, the moon); the nearest lit piece is the moon lamp, 29.5 m out.
// MoonDial and MarketCart (low, 23.0 m) are unlit and still. Nothing blinks; M_People cannot. No figure, no animal, nothing that flies.
// Estimate (the brief's: Site parts x 0.75, the three hero sites whole): courts 94 parts, right gap lines 168, left gap lines 54 = 316 x 0.75
//   = 237.0, + 3 = about 240 placed (319 before the track's share). 177 rules.
//   Parts by mesh before the track's share: BannerPole 39, MoonLamp 34, MarketCart 30, Armour 30, RoundTower_B 29, RoundTower_A 28, GardenWall 26, MoonDial 22, Well 11, TwinTurrets 10, TopiaryKnight 10, Astronomer 9, BellTower 9, BridgeHouse 9, Dovecote 8, Gatehouse 7, BannerHall 5, CrescentKeep 3.
//   Smallest x min: TALL 29.5 m (Armour, BannerPole, MoonLamp); low 23.0 m (MoonDial, MarketCart).
#define S1D6(n) TEXT("/Game/World/ThemePark/SM_S1D6_" n)
#define S1D6_AT(set, s, side, x) Site(TEXT(set), side, x, 1000, 1000 - (s), 1.f)
Set(TEXT("WldS1D6CourtA"), { { S1D6("Gatehouse"), 0.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D6("GardenWall"), -13.2f, -1.5f, 0.f, 0.0f, 1.f }, { S1D6("GardenWall"), 13.2f, -1.5f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6CourtB"), { { S1D6("BellTower"), -11.0f, 1.0f, 0.f, 0.0f, 1.f }, { S1D6("BridgeHouse"), 2.5f, 0.0f, 0.f, 0.0f, 1.f }, { S1D6("RoundTower_B"), 14.5f, 1.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6CourtC"), { { S1D6("BannerHall"), 1.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D6("Astronomer"), -15.5f, 2.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6CourtD"), { { S1D6("TwinTurrets"), -12.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D6("GardenWall"), -2.0f, -2.0f, 0.f, 0.0f, 1.f }, { S1D6("Dovecote"), 9.0f, 1.0f, 0.f, 0.0f, 1.f }, { S1D6("RoundTower_A"), 18.5f, -1.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6CourtE"), { { S1D6("Astronomer"), 0.0f, 1.0f, 0.f, 0.0f, 1.f }, { S1D6("GardenWall"), -10.2f, -2.0f, 0.f, 0.0f, 1.f }, { S1D6("GardenWall"), 10.2f, -2.0f, 0.f, 0.0f, 1.f }, { S1D6("RoundTower_B"), -18.5f, 1.0f, 0.f, 0.0f, 1.f }, { S1D6("RoundTower_A"), 18.3f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6CourtF"), { { S1D6("Dovecote"), -9.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D6("BridgeHouse"), 6.0f, 1.0f, 0.f, 0.0f, 1.f }, { S1D6("BellTower"), 18.5f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapN1F"), { { S1D6("MoonDial"), 22.62f, -6.5f, 0.f, 0.0f, 1.f }, { S1D6("BannerPole"), 22.62f, 0.0f, 0.f, 0.0f, 1.f }, { S1D6("Armour"), 22.62f, 3.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapN1B"), { { S1D6("MoonDial"), -22.62f, -6.5f, 0.f, 0.0f, 1.f }, { S1D6("BannerPole"), -22.62f, 0.0f, 0.f, 0.0f, 1.f }, { S1D6("Armour"), -22.62f, 3.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapN2F"), { { S1D6("MarketCart"), 22.62f, -6.5f, 0.f, 0.0f, 1.f }, { S1D6("MoonLamp"), 22.62f, 0.0f, 0.f, 0.0f, 1.f }, { S1D6("BannerPole"), 22.62f, 3.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapN2B"), { { S1D6("MarketCart"), -22.62f, -6.5f, 0.f, 0.0f, 1.f }, { S1D6("MoonLamp"), -22.62f, 0.0f, 0.f, 0.0f, 1.f }, { S1D6("BannerPole"), -22.62f, 3.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapN3F"), { { S1D6("Armour"), 22.62f, 0.0f, 0.f, 0.0f, 1.f }, { S1D6("MoonLamp"), 22.62f, 3.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapN3B"), { { S1D6("Armour"), -22.62f, 0.0f, 0.f, 0.0f, 1.f }, { S1D6("MoonLamp"), -22.62f, 3.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapN4F"), { { S1D6("MoonDial"), 22.62f, -6.5f, 0.f, 0.0f, 1.f }, { S1D6("Armour"), 22.62f, 3.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapN5B"), { { S1D6("MarketCart"), -22.62f, -6.5f, 0.f, 0.0f, 1.f }, { S1D6("BannerPole"), -22.62f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapM1F"), { { S1D6("Well"), 22.62f, 9.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapM1B"), { { S1D6("Well"), -22.62f, 9.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapM2F"), { { S1D6("TopiaryKnight"), 22.62f, 9.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapM2B"), { { S1D6("TopiaryKnight"), -22.62f, 9.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapT1F"), { { S1D6("RoundTower_A"), 22.62f, 26.7f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapT1B"), { { S1D6("RoundTower_A"), -22.62f, 26.7f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapT2F"), { { S1D6("RoundTower_B"), 22.62f, 26.7f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapT2B"), { { S1D6("RoundTower_B"), -22.62f, 26.7f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapT3F"), { { S1D6("TwinTurrets"), 22.62f, 26.7f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapT3B"), { { S1D6("TwinTurrets"), -22.62f, 26.7f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapL1P"), { { S1D6("MoonDial"), 22.62f, -8.0f, 0.f, 0.0f, 1.f }, { S1D6("BannerPole"), 22.62f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapL1M"), { { S1D6("MoonDial"), -22.62f, -8.0f, 0.f, 0.0f, 1.f }, { S1D6("BannerPole"), -22.62f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapL2P"), { { S1D6("Armour"), 22.62f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapL2M"), { { S1D6("Armour"), -22.62f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapL3P"), { { S1D6("MarketCart"), 22.62f, -8.0f, 0.f, 0.0f, 1.f }, { S1D6("MoonLamp"), 22.62f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapL3M"), { { S1D6("MarketCart"), -22.62f, -8.0f, 0.f, 0.0f, 1.f }, { S1D6("MoonLamp"), -22.62f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapL4P"), { { S1D6("MoonLamp"), 22.62f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6GapL4M"), { { S1D6("MoonLamp"), -22.62f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6Keep"), { { S1D6("CrescentKeep"), 0.0f, 0.0f, 0.f, 0.0f, 1.f } });
// a lambda of its own, so that the population function stays small for the compiler (as the p3b block does)
auto WldS1D6Rules = [&]()
{
Zone(TEXT("World s1d6, Crescent Keep"), 11.94f, 16.1f);
// the courts (buildings, 57.5 to 65 m)
	S1D6_AT("WldS1D6CourtA", 264, -1, 60.5f); S1D6_AT("WldS1D6CourtC", 266, -1, 60.5f); S1D6_AT("WldS1D6CourtB", 272, -1, 60.5f);
	S1D6_AT("WldS1D6CourtA", 274, -1, 63.5f); S1D6_AT("WldS1D6CourtE", 276, -1, 60.5f); S1D6_AT("WldS1D6CourtD", 282, -1, 60.5f);
	S1D6_AT("WldS1D6CourtA", 286, -1, 63.5f); S1D6_AT("WldS1D6CourtF", 292, -1, 60.5f); S1D6_AT("WldS1D6CourtC", 294, -1, 60.5f);
	S1D6_AT("WldS1D6CourtB", 296, -1, 60.5f); S1D6_AT("WldS1D6CourtE", 302, -1, 60.5f); S1D6_AT("WldS1D6CourtD", 304, -1, 60.5f);
	S1D6_AT("WldS1D6CourtF", 306, -1, 60.5f); S1D6_AT("WldS1D6CourtC", 314, -1, 60.5f); S1D6_AT("WldS1D6CourtB", 316, -1, 60.5f);
	S1D6_AT("WldS1D6CourtA", 322, -1, 63.5f); S1D6_AT("WldS1D6CourtE", 324, -1, 60.5f); S1D6_AT("WldS1D6CourtD", 332, -1, 60.5f);
	S1D6_AT("WldS1D6CourtA", 334, -1, 63.5f); S1D6_AT("WldS1D6CourtF", 336, -1, 60.5f); S1D6_AT("WldS1D6CourtC", 342, -1, 60.5f);
	S1D6_AT("WldS1D6CourtB", 344, -1, 60.5f); S1D6_AT("WldS1D6CourtA", 346, -1, 63.5f); S1D6_AT("WldS1D6CourtA", 352, -1, 60.5f);
	S1D6_AT("WldS1D6CourtB", 274, 1, 61.0f); S1D6_AT("WldS1D6CourtF", 282, 1, 61.0f); S1D6_AT("WldS1D6CourtD", 306, 1, 61.0f);
	S1D6_AT("WldS1D6CourtC", 322, 1, 61.0f); S1D6_AT("WldS1D6CourtE", 346, 1, 61.0f);
// the gap lines, right (low 23.0 m, props 29.5 / 32.5 / 38.5 m, a thin tower 56.2 m)
	S1D6_AT("WldS1D6GapN5B", 264, 1, 29.5f); S1D6_AT("WldS1D6GapT1B", 264, 1, 29.5f); S1D6_AT("WldS1D6GapN1F", 264, 1, 29.5f);
	S1D6_AT("WldS1D6GapM1F", 264, 1, 29.5f); S1D6_AT("WldS1D6GapT2F", 264, 1, 29.5f); S1D6_AT("WldS1D6GapN2B", 268, 1, 29.5f);
	S1D6_AT("WldS1D6GapM2B", 268, 1, 29.5f); S1D6_AT("WldS1D6GapT1B", 268, 1, 29.5f); S1D6_AT("WldS1D6GapN3F", 268, 1, 29.5f);
	S1D6_AT("WldS1D6GapT2F", 268, 1, 29.5f); S1D6_AT("WldS1D6GapN5B", 272, 1, 29.5f); S1D6_AT("WldS1D6GapT3B", 272, 1, 29.5f);
	S1D6_AT("WldS1D6GapN4F", 272, 1, 29.5f); S1D6_AT("WldS1D6GapM1F", 272, 1, 29.5f); S1D6_AT("WldS1D6GapT1F", 272, 1, 29.5f);
	S1D6_AT("WldS1D6GapN5B", 276, 1, 29.5f); S1D6_AT("WldS1D6GapT2B", 276, 1, 29.5f); S1D6_AT("WldS1D6GapN1F", 276, 1, 29.5f);
	S1D6_AT("WldS1D6GapM2F", 276, 1, 29.5f); S1D6_AT("WldS1D6GapT1F", 276, 1, 29.5f); S1D6_AT("WldS1D6GapN2B", 280, 1, 29.5f);
	S1D6_AT("WldS1D6GapM1B", 280, 1, 29.5f); S1D6_AT("WldS1D6GapT2B", 280, 1, 29.5f); S1D6_AT("WldS1D6GapN3F", 280, 1, 29.5f);
	S1D6_AT("WldS1D6GapT1F", 280, 1, 29.5f); S1D6_AT("WldS1D6GapN5B", 284, 1, 29.5f); S1D6_AT("WldS1D6GapT2B", 284, 1, 29.5f);
	S1D6_AT("WldS1D6GapN1F", 284, 1, 29.5f); S1D6_AT("WldS1D6GapM2F", 284, 1, 29.5f); S1D6_AT("WldS1D6GapT3F", 284, 1, 29.5f);
	S1D6_AT("WldS1D6GapN5B", 288, 1, 29.5f); S1D6_AT("WldS1D6GapT1B", 288, 1, 29.5f); S1D6_AT("WldS1D6GapN2F", 288, 1, 29.5f);
	S1D6_AT("WldS1D6GapM1F", 288, 1, 29.5f); S1D6_AT("WldS1D6GapT2F", 288, 1, 29.5f); S1D6_AT("WldS1D6GapN3B", 292, 1, 29.5f);
	S1D6_AT("WldS1D6GapT1B", 292, 1, 29.5f); S1D6_AT("WldS1D6GapN1F", 292, 1, 29.5f); S1D6_AT("WldS1D6GapM2F", 292, 1, 29.5f);
	S1D6_AT("WldS1D6GapT2F", 292, 1, 29.5f); S1D6_AT("WldS1D6GapN5B", 296, 1, 29.5f); S1D6_AT("WldS1D6GapT1B", 296, 1, 29.5f);
	S1D6_AT("WldS1D6GapN4F", 296, 1, 29.5f); S1D6_AT("WldS1D6GapM1F", 296, 1, 29.5f); S1D6_AT("WldS1D6GapT2F", 296, 1, 29.5f);
	S1D6_AT("WldS1D6GapN5B", 300, 1, 29.5f); S1D6_AT("WldS1D6GapT3B", 300, 1, 29.5f); S1D6_AT("WldS1D6GapN2F", 300, 1, 29.5f);
	S1D6_AT("WldS1D6GapM2F", 300, 1, 29.5f); S1D6_AT("WldS1D6GapT1F", 300, 1, 29.5f); S1D6_AT("WldS1D6GapN3B", 304, 1, 29.5f);
	S1D6_AT("WldS1D6GapT2B", 304, 1, 29.5f); S1D6_AT("WldS1D6GapN1F", 304, 1, 29.5f); S1D6_AT("WldS1D6GapM1F", 304, 1, 29.5f);
	S1D6_AT("WldS1D6GapT1F", 304, 1, 29.5f); S1D6_AT("WldS1D6GapN5B", 308, 1, 29.5f); S1D6_AT("WldS1D6GapT2B", 308, 1, 29.5f);
	S1D6_AT("WldS1D6GapN4F", 308, 1, 29.5f); S1D6_AT("WldS1D6GapM2F", 308, 1, 29.5f); S1D6_AT("WldS1D6GapT1F", 308, 1, 29.5f);
	S1D6_AT("WldS1D6GapN5B", 312, 1, 29.5f); S1D6_AT("WldS1D6GapT2B", 312, 1, 29.5f); S1D6_AT("WldS1D6GapN2F", 312, 1, 29.5f);
	S1D6_AT("WldS1D6GapM1F", 312, 1, 29.5f); S1D6_AT("WldS1D6GapT3F", 312, 1, 29.5f); S1D6_AT("WldS1D6GapN3B", 316, 1, 29.5f);
	S1D6_AT("WldS1D6GapT1B", 316, 1, 29.5f); S1D6_AT("WldS1D6GapN1F", 316, 1, 29.5f); S1D6_AT("WldS1D6GapM2F", 316, 1, 29.5f);
	S1D6_AT("WldS1D6GapT2F", 316, 1, 29.5f); S1D6_AT("WldS1D6GapN5B", 320, 1, 29.5f); S1D6_AT("WldS1D6GapT1B", 320, 1, 29.5f);
	S1D6_AT("WldS1D6GapN2F", 320, 1, 29.5f); S1D6_AT("WldS1D6GapM1F", 320, 1, 29.5f); S1D6_AT("WldS1D6GapT2F", 320, 1, 29.5f);
	S1D6_AT("WldS1D6GapN5B", 324, 1, 29.5f); S1D6_AT("WldS1D6GapT1B", 324, 1, 29.5f); S1D6_AT("WldS1D6GapN3F", 324, 1, 29.5f);
	S1D6_AT("WldS1D6GapT2F", 324, 1, 29.5f); S1D6_AT("WldS1D6GapN1B", 328, 1, 29.5f); S1D6_AT("WldS1D6GapM2B", 328, 1, 29.5f);
	S1D6_AT("WldS1D6GapT3B", 328, 1, 29.5f); S1D6_AT("WldS1D6GapN2F", 328, 1, 29.5f); S1D6_AT("WldS1D6GapM1F", 328, 1, 29.5f);
	S1D6_AT("WldS1D6GapT1F", 328, 1, 29.5f); S1D6_AT("WldS1D6GapT2B", 332, 1, 29.5f); S1D6_AT("WldS1D6GapT1F", 332, 1, 29.5f);
	S1D6_AT("WldS1D6GapT2B", 336, 1, 29.5f); S1D6_AT("WldS1D6GapT1F", 336, 1, 29.5f); S1D6_AT("WldS1D6GapN3B", 340, 1, 29.5f);
	S1D6_AT("WldS1D6GapT2B", 340, 1, 29.5f); S1D6_AT("WldS1D6GapN1F", 340, 1, 29.5f); S1D6_AT("WldS1D6GapM2F", 340, 1, 29.5f);
	S1D6_AT("WldS1D6GapT3F", 340, 1, 29.5f); S1D6_AT("WldS1D6GapN5B", 344, 1, 29.5f); S1D6_AT("WldS1D6GapT1B", 344, 1, 29.5f);
	S1D6_AT("WldS1D6GapN4F", 344, 1, 29.5f); S1D6_AT("WldS1D6GapM1F", 344, 1, 29.5f); S1D6_AT("WldS1D6GapT2F", 344, 1, 29.5f);
	S1D6_AT("WldS1D6GapN5B", 348, 1, 29.5f); S1D6_AT("WldS1D6GapT1B", 348, 1, 29.5f); S1D6_AT("WldS1D6GapN2F", 348, 1, 29.5f);
	S1D6_AT("WldS1D6GapM2F", 348, 1, 29.5f); S1D6_AT("WldS1D6GapT2F", 348, 1, 29.5f); S1D6_AT("WldS1D6GapN3B", 352, 1, 29.5f);
	S1D6_AT("WldS1D6GapT1B", 352, 1, 29.5f); S1D6_AT("WldS1D6GapN1F", 352, 1, 29.5f); S1D6_AT("WldS1D6GapM1F", 352, 1, 29.5f);
	S1D6_AT("WldS1D6GapT2F", 352, 1, 29.5f);
// the gap lines, left (a prop 31.0 m, a low piece 23.0 m)
	S1D6_AT("WldS1D6GapL1P", 264, -1, 31.0f); S1D6_AT("WldS1D6GapL2M", 266, -1, 31.0f); S1D6_AT("WldS1D6GapL3P", 268, -1, 31.0f);
	S1D6_AT("WldS1D6GapL4M", 270, -1, 31.0f); S1D6_AT("WldS1D6GapL1M", 274, -1, 31.0f); S1D6_AT("WldS1D6GapL2P", 276, -1, 31.0f);
	S1D6_AT("WldS1D6GapL3M", 278, -1, 31.0f); S1D6_AT("WldS1D6GapL4P", 280, -1, 31.0f); S1D6_AT("WldS1D6GapL1M", 282, -1, 31.0f);
	S1D6_AT("WldS1D6GapL2M", 286, -1, 31.0f); S1D6_AT("WldS1D6GapL3P", 288, -1, 31.0f); S1D6_AT("WldS1D6GapL4M", 290, -1, 31.0f);
	S1D6_AT("WldS1D6GapL1P", 292, -1, 31.0f); S1D6_AT("WldS1D6GapL2M", 294, -1, 31.0f); S1D6_AT("WldS1D6GapL3M", 298, -1, 31.0f);
	S1D6_AT("WldS1D6GapL4P", 300, -1, 31.0f); S1D6_AT("WldS1D6GapL1M", 302, -1, 31.0f); S1D6_AT("WldS1D6GapL2P", 304, -1, 31.0f);
	S1D6_AT("WldS1D6GapL3M", 306, -1, 31.0f); S1D6_AT("WldS1D6GapL4M", 310, -1, 31.0f); S1D6_AT("WldS1D6GapL1P", 312, -1, 31.0f);
	S1D6_AT("WldS1D6GapL2M", 314, -1, 31.0f); S1D6_AT("WldS1D6GapL3P", 316, -1, 31.0f); S1D6_AT("WldS1D6GapL4M", 318, -1, 31.0f);
	S1D6_AT("WldS1D6GapL1M", 322, -1, 31.0f); S1D6_AT("WldS1D6GapL2P", 324, -1, 31.0f); S1D6_AT("WldS1D6GapL3M", 326, -1, 31.0f);
	S1D6_AT("WldS1D6GapL4P", 328, -1, 31.0f); S1D6_AT("WldS1D6GapL1M", 338, -1, 31.0f); S1D6_AT("WldS1D6GapL2P", 340, -1, 31.0f);
	S1D6_AT("WldS1D6GapL3M", 342, -1, 31.0f); S1D6_AT("WldS1D6GapL4M", 346, -1, 31.0f); S1D6_AT("WldS1D6GapL1P", 348, -1, 31.0f);
	S1D6_AT("WldS1D6GapL2M", 350, -1, 31.0f); S1D6_AT("WldS1D6GapL3P", 352, -1, 31.0f); S1D6_AT("WldS1D6GapL4M", 354, -1, 31.0f);
// THE CRESCENT KEEP, right, 150 m
	S1D6_AT("WldS1D6Keep", 278, 1, 150.0f); S1D6_AT("WldS1D6Keep", 314, 1, 150.0f); S1D6_AT("WldS1D6Keep", 350, 1, 150.0f);
};
WldS1D6Rules();
#undef S1D6_AT
#undef S1D6
// ---- END s1d6 ----
}

{
// ---- BEGIN s1d7 (district 7 "The Last Ride: Snowglobe Square", 16.10 to 20.50 km, ft_world_park_s1d7.py) ----
// 16 meshes (7 buildings, 8 props, 1 hero). Written by C:/ftwork/jobs/L4s1d7/mkblock.py from the slots FTDressL4.inl and
// FTPeopleL4.inl take. The zone has slots 356 to 453; this block uses EVEN slots only (s % 4 == 0 or 2), one rule a site
// (every 1000, phase 1000 - slot, the way the p3b block names its slots), each a Site: a piece stands at its slot's middle.
// What the tables already have on the even slots, and the band this block takes there (m = s % 12):
//   LEFT   m 0, 4         a stall row at 31 m, a watchers' deck at 37.5 m           -> a row of two buildings, anchor 48 m
//          m 2, 6, 8, 10  a dock, a rocket pad, a gateway or a court at 40 to 46 m  -> a row of two buildings, anchor 62 m
//   RIGHT  s % 4 == 0     a big piece of the cast at 27 to 32 m, a pond at 36 to 46 m, a high piece of p3d at 62 m
//                         -> a row of two buildings, anchor 52 m, and a forecourt of three TALL props at 40.0 m
//          s % 4 == 2     the parade lane, planets on posts, a p3b scene at 39 to 42 m -> a row of two buildings, anchor 54 m
//   Left out: slots with a show fountain at 44 to 56 m (s % 13 == 4); at 62 m the slots of a bandstand, a fountain or a
//   mascot at 54 to 70 m (s % 5 == 0 or 3, s % 7 == 4); on the right the fountain's slots (s % 5 == 3), the p3b scenes at
//   42 m (s % 16 == 4) and the watchers' tower at 48 m (m == 6).
//   KERB   three LOW unlit props at 22.8 m (behind the verge at 19 m, before the parade lane and the cast at 24.1 m and
//          out), in the slots the p3a kerb pieces leave (left m 0, 2, 6, 8; right m 2, 4, 8, 10) and only on the level
//          stretches between the banked bends: 16.10-16.27, 16.54-17.49, 17.76-18.76, 19.03-20.02, 20.29-20.50 km.
//   HERO   THE SNOW GLOBE, left, anchor 132 m (it reaches 101 to 161 m out), slots 376, 406, 438 (17.01, 18.37, 19.82 km).
// Estimate: 128 sites; 318 parts in the near sites x 0.75 (the track's share left out) = 238.5, + 3 heroes counted whole = about 242 placed.
//   sites: Cour 18, Glob 3, Kerb 50, RowA 12, RowB 12, RowC 11, RowD 11, RowE 11.
//   by mesh, before the 0.75: SnowFence 38, IceSeats 38, Sleigh 37, SkiStack 37, ChocHut 23, BellChalet 23, MittenShop 23, LampPair 14, VanePost 14, SnowmanFamily 13, IceFish 13, SkateHire 12, IceArcade 11, TobogganTower 11, PalaceGate 11, SnowGlobe 3.
// Smallest x min of a TALL mesh: 40.0 m (IceFish, LampPair, SnowmanFamily, VanePost). The four LOW meshes: 22.8 m. Glow only on meshes stood 40 m and more out.
// NOT COMPILED AND NOT SEEN IN THE GAME when written.
#define S1D7(n) TEXT("/Game/World/ThemePark/SM_S1D7_" n)
#define S1D7_AT(set, s, side, x) Site(TEXT(set), side, x, 1000, 1000 - (s), 1.f)
// { mesh, along m, out m, up m, yaw deg, scale }
Set(TEXT("WldS1D7RowA1"), { { S1D7("SkateHire"), -5.4f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("ChocHut"), 9.4f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7RowA2"), { { S1D7("ChocHut"), -9.4f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("SkateHire"), 5.4f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7RowB1"), { { S1D7("BellChalet"), -7.6f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("MittenShop"), 7.4f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7RowB2"), { { S1D7("MittenShop"), -7.4f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("BellChalet"), 7.6f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7RowC1"), { { S1D7("IceArcade"), -7.6f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("MittenShop"), 9.4f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7RowC2"), { { S1D7("MittenShop"), -9.4f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("IceArcade"), 7.6f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7RowD1"), { { S1D7("TobogganTower"), -5.4f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("ChocHut"), 9.8f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7RowD2"), { { S1D7("ChocHut"), -9.8f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("TobogganTower"), 5.4f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7RowE1"), { { S1D7("PalaceGate"), -7.4f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("BellChalet"), 9.3f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7RowE2"), { { S1D7("BellChalet"), -9.3f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("PalaceGate"), 7.4f, 0.0f, 0.f, 0.0f, 1.f } });
// fix round: on the right's slots s % 12 == 2 the chair swing of the next slot's FunCourt stands +6.3 to +24.2 m along, 41 to 59 m out:
// there a row is only its building at minus along
Set(TEXT("WldS1D7SoloA1"), { { S1D7("SkateHire"), -5.4f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7SoloB1"), { { S1D7("BellChalet"), -7.6f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7SoloD1"), { { S1D7("TobogganTower"), -5.4f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7SoloD2"), { { S1D7("ChocHut"), -9.8f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7SoloE1"), { { S1D7("PalaceGate"), -7.4f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7SoloE2"), { { S1D7("BellChalet"), -9.3f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7Kerb1"), { { S1D7("SnowFence"), -13.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("Sleigh"), 0.0f, 0.0f, 0.f, 10.0f, 1.f }, { S1D7("IceSeats"), 13.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7Kerb2"), { { S1D7("IceSeats"), -13.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("SkiStack"), 0.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("SnowFence"), 13.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7Kerb3"), { { S1D7("SkiStack"), -12.0f, 0.0f, 0.f, -8.0f, 1.f }, { S1D7("IceSeats"), 0.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("Sleigh"), 12.0f, 0.0f, 0.f, -12.0f, 1.f } });
Set(TEXT("WldS1D7Kerb4"), { { S1D7("Sleigh"), -12.0f, 0.0f, 0.f, 14.0f, 1.f }, { S1D7("SnowFence"), 0.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("SkiStack"), 12.0f, 0.0f, 0.f, 6.0f, 1.f } });
Set(TEXT("WldS1D7Court1"), { { S1D7("LampPair"), -11.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("SnowmanFamily"), 0.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("VanePost"), 11.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7Court2"), { { S1D7("VanePost"), -11.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("IceFish"), 0.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("LampPair"), 11.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7Court3"), { { S1D7("SnowmanFamily"), -11.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("LampPair"), 0.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("IceFish"), 11.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D7Court4"), { { S1D7("IceFish"), -11.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("VanePost"), 0.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("SnowmanFamily"), 11.0f, 0.0f, 0.f, 0.0f, 1.f } });
// fix round: the dressing rolls a pond 16 m across on the right's slots s % 4 == 0 (36 to 46 m out, sliding 11 m either way; chance 0.7).
// Where the roll puts it under a court or a row, that slot gets a set of its own without the pieces in the water (a prop 0.3 m in, a
// building 1.5 m in; worked out with the track's own dice by C:/ftwork/jobs/L4s1fix/edit2.py). A rule with slot 0 places nothing: it
// stays so that every later rule keeps its number, and with it its dice. Off the same way: the row of slot 360 (both buildings 5.5 m into the
// dressing's bandstand as the dice put it) and the toboggan tower of slot 410 (12 m into a bandstand and the far mascot).
Set(TEXT("WldS1D7Court2P364"), { { S1D7("VanePost"), -11.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("LampPair"), 11.0f, 0.0f, 0.f, 0.0f, 1.f } });   // slot 364 without IceFish 7.6 m
Set(TEXT("WldS1D7Court3P368"), { { S1D7("IceFish"), 11.0f, 0.0f, 0.f, 0.0f, 1.f } });   // slot 368 without SnowmanFamily 4.6 m, LampPair 5.0 m
Set(TEXT("WldS1D7RowD1P376"), { { S1D7("ChocHut"), 9.8f, 0.0f, 0.f, 0.0f, 1.f } });   // slot 376 without TobogganTower 2.3 m
Set(TEXT("WldS1D7Court4P376"), { { S1D7("VanePost"), 0.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("SnowmanFamily"), 11.0f, 0.0f, 0.f, 0.0f, 1.f } });   // slot 376 without IceFish 7.7 m
Set(TEXT("WldS1D7Court1P380"), { { S1D7("LampPair"), -11.0f, 0.0f, 0.f, 0.0f, 1.f } });   // slot 380 without SnowmanFamily 2.4 m, VanePost 6.5 m
Set(TEXT("WldS1D7Court3P392"), { { S1D7("IceFish"), 11.0f, 0.0f, 0.f, 0.0f, 1.f } });   // slot 392 without SnowmanFamily 1.6 m, LampPair 5.8 m
Set(TEXT("WldS1D7Court4P396"), { { S1D7("SnowmanFamily"), 11.0f, 0.0f, 0.f, 0.0f, 1.f } });   // slot 396 without IceFish 2.9 m, VanePost 4.5 m
Set(TEXT("WldS1D7Court1P400"), { { S1D7("VanePost"), 11.0f, 0.0f, 0.f, 0.0f, 1.f } });   // slot 400 without LampPair 7.2 m, SnowmanFamily 2.4 m
Set(TEXT("WldS1D7Court4P416"), { { S1D7("IceFish"), -11.0f, 0.0f, 0.f, 0.0f, 1.f } });   // slot 416 without VanePost 7.0 m, SnowmanFamily 1.0 m
Set(TEXT("WldS1D7Court1P424"), { { S1D7("VanePost"), 11.0f, 0.0f, 0.f, 0.0f, 1.f } });   // slot 424 without LampPair 1.8 m, SnowmanFamily 3.2 m
Set(TEXT("WldS1D7Court2P428"), { { S1D7("LampPair"), 11.0f, 0.0f, 0.f, 0.0f, 1.f } });   // slot 428 without VanePost 5.8 m, IceFish 2.1 m
Set(TEXT("WldS1D7RowB2P432"), { { S1D7("BellChalet"), 7.6f, 0.0f, 0.f, 0.0f, 1.f } });   // slot 432 without MittenShop 2.4 m
Set(TEXT("WldS1D7Court3P432"), { { S1D7("IceFish"), 11.0f, 0.0f, 0.f, 0.0f, 1.f } });   // slot 432 without SnowmanFamily 8.7 m, LampPair 1.0 m
Set(TEXT("WldS1D7Court1P444"), { { S1D7("LampPair"), -11.0f, 0.0f, 0.f, 0.0f, 1.f } });   // slot 444 without SnowmanFamily 4.8 m, VanePost 0.7 m
Set(TEXT("WldS1D7Court2P448"), { { S1D7("VanePost"), -11.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D7("LampPair"), 11.0f, 0.0f, 0.f, 0.0f, 1.f } });   // slot 448 without IceFish 7.4 m
Set(TEXT("WldS1D7Globe"), { { S1D7("SnowGlobe"), 0.0f, 0.0f, 0.f, 0.0f, 1.f } });
Zone(TEXT("World s1d7, Snowglobe Square"), 16.1f, 20.5f);
	S1D7_AT("WldS1D7RowA1", 356, -1, 62.0f); S1D7_AT("WldS1D7Kerb1", 356, -1, 22.8f); S1D7_AT("WldS1D7Kerb2", 356, 1, 22.8f);
	S1D7_AT("WldS1D7Kerb3", 358, 1, 22.8f); S1D7_AT("WldS1D7RowB1", 360, -1, 48.0f); S1D7_AT("WldS1D7RowC1", 0, 1, 52.0f);
	S1D7_AT("WldS1D7Court1", 360, 1, 40.0f); S1D7_AT("WldS1D7RowD1", 362, -1, 62.0f); S1D7_AT("WldS1D7SoloE1", 362, 1, 54.0f);
	S1D7_AT("WldS1D7RowA2", 364, -1, 48.0f); S1D7_AT("WldS1D7RowB2", 364, 1, 52.0f); S1D7_AT("WldS1D7Court2P364", 364, 1, 40.0f);
	S1D7_AT("WldS1D7RowC2", 366, -1, 62.0f); S1D7_AT("WldS1D7Kerb4", 366, -1, 22.8f); S1D7_AT("WldS1D7Kerb1", 368, -1, 22.8f);
	S1D7_AT("WldS1D7Court3P368", 368, 1, 40.0f); S1D7_AT("WldS1D7Kerb2", 368, 1, 22.8f); S1D7_AT("WldS1D7RowD2", 370, 1, 54.0f);
	S1D7_AT("WldS1D7Kerb3", 370, 1, 22.8f); S1D7_AT("WldS1D7RowE2", 372, -1, 48.0f); S1D7_AT("WldS1D7Kerb4", 372, -1, 22.8f);
	S1D7_AT("WldS1D7RowA1", 374, -1, 62.0f); S1D7_AT("WldS1D7Kerb1", 374, -1, 22.8f); S1D7_AT("WldS1D7SoloB1", 374, 1, 54.0f);
	S1D7_AT("WldS1D7Kerb2", 374, 1, 22.8f); S1D7_AT("WldS1D7RowC1", 376, -1, 48.0f); S1D7_AT("WldS1D7Globe", 376, -1, 132.0f);
	S1D7_AT("WldS1D7RowD1P376", 376, 1, 52.0f); S1D7_AT("WldS1D7Court4P376", 376, 1, 40.0f); S1D7_AT("WldS1D7Kerb3", 376, 1, 22.8f);
	S1D7_AT("WldS1D7Kerb4", 378, -1, 22.8f); S1D7_AT("WldS1D7Kerb1", 380, -1, 22.8f); S1D7_AT("WldS1D7RowE1", 380, 1, 52.0f);
	S1D7_AT("WldS1D7Court1P380", 380, 1, 40.0f); S1D7_AT("WldS1D7Kerb2", 380, 1, 22.8f); S1D7_AT("WldS1D7RowA2", 382, 1, 54.0f);
	S1D7_AT("WldS1D7Kerb3", 382, 1, 22.8f); S1D7_AT("WldS1D7RowB2", 384, -1, 48.0f); S1D7_AT("WldS1D7Kerb4", 384, -1, 22.8f);
	S1D7_AT("WldS1D7RowC2", 384, 1, 52.0f); S1D7_AT("WldS1D7Court2", 384, 1, 40.0f); S1D7_AT("WldS1D7RowD2", 386, -1, 62.0f);
	S1D7_AT("WldS1D7Kerb1", 386, -1, 22.8f); S1D7_AT("WldS1D7SoloE2", 386, 1, 54.0f); S1D7_AT("WldS1D7Kerb2", 386, 1, 22.8f);
	S1D7_AT("WldS1D7RowA1", 388, -1, 48.0f); S1D7_AT("WldS1D7RowB1", 392, -1, 62.0f); S1D7_AT("WldS1D7RowC1", 0, 1, 52.0f);
	S1D7_AT("WldS1D7Court3P392", 392, 1, 40.0f); S1D7_AT("WldS1D7RowD1", 394, -1, 62.0f); S1D7_AT("WldS1D7Kerb3", 394, 1, 22.8f);
	S1D7_AT("WldS1D7RowE1", 396, -1, 48.0f); S1D7_AT("WldS1D7Kerb4", 396, -1, 22.8f); S1D7_AT("WldS1D7RowA2", 396, 1, 52.0f);
	S1D7_AT("WldS1D7Court4P396", 396, 1, 40.0f); S1D7_AT("WldS1D7Kerb1", 398, -1, 22.8f); S1D7_AT("WldS1D7Kerb2", 398, 1, 22.8f);
	S1D7_AT("WldS1D7RowB2", 400, -1, 48.0f); S1D7_AT("WldS1D7RowC2", 400, 1, 52.0f); S1D7_AT("WldS1D7Court1P400", 400, 1, 40.0f);
	S1D7_AT("WldS1D7Kerb3", 400, 1, 22.8f); S1D7_AT("WldS1D7RowD2", 402, -1, 62.0f); S1D7_AT("WldS1D7Kerb4", 402, -1, 22.8f);
	S1D7_AT("WldS1D7RowE2", 404, -1, 62.0f); S1D7_AT("WldS1D7Kerb1", 404, -1, 22.8f); S1D7_AT("WldS1D7Kerb2", 404, 1, 22.8f);
	S1D7_AT("WldS1D7RowA1", 406, -1, 62.0f); S1D7_AT("WldS1D7Globe", 406, -1, 132.0f); S1D7_AT("WldS1D7RowB1", 406, 1, 54.0f);
	S1D7_AT("WldS1D7Kerb3", 406, 1, 22.8f); S1D7_AT("WldS1D7RowC1", 408, -1, 48.0f); S1D7_AT("WldS1D7Kerb4", 408, -1, 22.8f);
	S1D7_AT("WldS1D7Court2", 408, 1, 40.0f); S1D7_AT("WldS1D7Kerb1", 410, -1, 22.8f); S1D7_AT("WldS1D7SoloD1", 0, 1, 54.0f);
	S1D7_AT("WldS1D7Kerb2", 410, 1, 22.8f); S1D7_AT("WldS1D7RowE1", 412, -1, 48.0f); S1D7_AT("WldS1D7RowA2", 412, 1, 52.0f);
	S1D7_AT("WldS1D7Court3", 412, 1, 40.0f); S1D7_AT("WldS1D7Kerb3", 412, 1, 22.8f); S1D7_AT("WldS1D7RowB2", 414, -1, 62.0f);
	S1D7_AT("WldS1D7Kerb4", 414, -1, 22.8f); S1D7_AT("WldS1D7RowC2", 416, -1, 62.0f); S1D7_AT("WldS1D7RowD2", 416, 1, 52.0f);
	S1D7_AT("WldS1D7Court4P416", 416, 1, 40.0f); S1D7_AT("WldS1D7RowE2", 422, -1, 62.0f); S1D7_AT("WldS1D7Kerb1", 422, -1, 22.8f);
	S1D7_AT("WldS1D7SoloA1", 422, 1, 54.0f); S1D7_AT("WldS1D7Kerb2", 422, 1, 22.8f); S1D7_AT("WldS1D7RowB1", 424, -1, 48.0f);
	S1D7_AT("WldS1D7RowC1", 0, 1, 52.0f); S1D7_AT("WldS1D7Court1P424", 424, 1, 40.0f); S1D7_AT("WldS1D7Kerb3", 424, 1, 22.8f);
	S1D7_AT("WldS1D7RowD1", 426, -1, 62.0f); S1D7_AT("WldS1D7Kerb4", 426, -1, 22.8f); S1D7_AT("WldS1D7Kerb1", 428, -1, 22.8f);
	S1D7_AT("WldS1D7Court2P428", 428, 1, 40.0f); S1D7_AT("WldS1D7Kerb2", 428, 1, 22.8f); S1D7_AT("WldS1D7RowE1", 430, 1, 54.0f);
	S1D7_AT("WldS1D7Kerb3", 430, 1, 22.8f); S1D7_AT("WldS1D7RowA2", 432, -1, 48.0f); S1D7_AT("WldS1D7Kerb4", 432, -1, 22.8f);
	S1D7_AT("WldS1D7RowB2P432", 432, 1, 52.0f); S1D7_AT("WldS1D7Court3P432", 432, 1, 40.0f); S1D7_AT("WldS1D7RowC2", 434, -1, 62.0f);
	S1D7_AT("WldS1D7Kerb1", 434, -1, 22.8f); S1D7_AT("WldS1D7SoloD2", 434, 1, 54.0f); S1D7_AT("WldS1D7Kerb2", 434, 1, 22.8f);
	S1D7_AT("WldS1D7RowE2", 436, -1, 48.0f); S1D7_AT("WldS1D7Kerb3", 436, 1, 22.8f); S1D7_AT("WldS1D7Kerb4", 438, -1, 22.8f);
	S1D7_AT("WldS1D7Globe", 438, -1, 132.0f); S1D7_AT("WldS1D7Kerb1", 440, -1, 22.8f); S1D7_AT("WldS1D7RowA1", 440, 1, 52.0f);
	S1D7_AT("WldS1D7Court4", 440, 1, 40.0f); S1D7_AT("WldS1D7Kerb2", 440, 1, 22.8f); S1D7_AT("WldS1D7RowB1", 442, -1, 62.0f);
	S1D7_AT("WldS1D7RowC1", 442, 1, 54.0f); S1D7_AT("WldS1D7Kerb3", 442, 1, 22.8f); S1D7_AT("WldS1D7RowD1", 444, -1, 48.0f);
	S1D7_AT("WldS1D7RowE1", 0, 1, 52.0f); S1D7_AT("WldS1D7Court1P444", 444, 1, 40.0f); S1D7_AT("WldS1D7RowA2", 446, -1, 62.0f);
	S1D7_AT("WldS1D7RowB2", 448, -1, 48.0f); S1D7_AT("WldS1D7Court2P448", 448, 1, 40.0f); S1D7_AT("WldS1D7Kerb4", 450, -1, 22.8f);
	S1D7_AT("WldS1D7Kerb1", 452, -1, 22.8f); S1D7_AT("WldS1D7Kerb2", 452, 1, 22.8f);
#undef S1D7_AT
#undef S1D7
// ---- END s1d7 ----
}

{
// ---- BEGIN s1d8 (district 8 "The Last Ride: Giant's Garden", 20.5 km to the end, ft_world_park_s1d8.py) ----
// 16 meshes: 7 buildings (ToadHouse, PotTower, Boot, Birdhouse, Can, SeedShop, Acorn), 8 props (tall: TrowelFork, SnailKiosk, Barrow,
// Lamp, WashLine; low and unlit: ToadSeats, PegFence, Thimbles) and the hero (Teapot). NOT COMPILED, NOT SEEN IN THE GAME when written.
// Estimate (slots counted rule by rule, x 0.75 for the track's share left out): 125 Sites hold 274 pieces, x 0.75 = about 206 placed
//   (199 pieces stand outside the plan's derived loop km 20.35-20.64, 21.61-21.90, 22.87-23.16, 24.13-24.42).
//   By mesh, slots before the track leaves any out (and x 0.75): Acorn 9 (6.8), Barrow 14 (10.5), Birdhouse 13 (9.8), Boot 12 (9.0), Can 13 (9.8), Lamp 28 (21.0), PegFence 34 (25.5), PotTower 9 (6.8), SeedShop 12 (9.0), SnailKiosk 14 (10.5), Teapot 3 (2.2), Thimbles 40 (30.0), ToadHouse 10 (7.5), ToadSeats 35 (26.2), TrowelFork 14 (10.5), WashLine 14 (10.5).
//   The most of one mesh: Thimbles, 40 slots (x 0.75 = 30.0). The hero: 3.
// Every rule is a Site: a piece stands at its slot's middle plus its own offset (a Scatter slides up to 11 m into the next slot's furniture).
// Slots: all even. Bands and slots, chosen from FTDressL4.inl and FTPeopleL4.inl (read, not measured):
//   BUILDINGS, two to a slot. LEFT 63 m (fronts 55 m, backs 71 m): behind the Last Ride's sets (31 to 46 m, every even slot on the left), before the
//     main street's flats (72 to 80 m). Slots s % 20 == 2, 4, 6, 12, 14, 16: never s % 5 == 0 (bandstands, 56 to 70 m) or 3 (fountains, 54 to 68 m).
//     RIGHT nine slots with s % 4 == 2, one rule each (every 1000, phase 1000 - slot), where the people tables leave a band free:
//     51.5 m where s % 24 == 2 or 14 (p3c's high ways 61 to 63 m and p3d 64 m behind), 55 m where s % 24 == 10 (p3d 44 m in front),
//     60 m where s % 24 == 18 (the watchers' tower 48 m in front); never s % 5 == 0 or 3; none on s % 4 == 0 (p3d stands 62 m out on every one).
//   TALL PROPS, three to a slot, only on the level stretches between the banked bends (the four "verge" Zones: p3a's own km for its verge):
//     RIGHT 37 m on s % 4 == 2 (parade lane to 27.1 m and planets on posts 29 to 35 m in front, p3a 42 m behind); LEFT 37.5 m on s % 12 == 8,
//     behind a stall row (back wall 33.1 m), set off along the road so nothing stands at the slot's middle (the sky ride's line, 35 m).
//   LOW PROPS (log: tops 1.88 to 1.90 m, "low"; unlit), 23.3 m (22.4 to 24.2): behind the queue rails (left s % 12 == 2, 8) and the benches
//     (right s % 12 == 4, 10), three to a slot; either end of p3a's kerb groups (21 m; left s % 12 == 4, 10, right 0, 6), one to a slot, 14 m along.
//     Not in the hoarding's slots (left s % 6 == 0, right s % 6 == 2): nothing 1.9 m tall shows behind it.
//   HERO THE GIANT'S TEAPOT, right, 130 m (113 to 145 m), slots 462, 494, 524 (20.90, 22.35, 23.71 km): between the laps' loops, off the slots of the
//     right-hand far rules there (SM_Maintenance, SM_WoodCoaster2, SM_Funhouse, SM_Flume, SM_DropTower, the bleachers and the far crowd).
// Light: steady only. The low pieces carry none; everything else stands 37 m out or more (glow 0.08, lit windows 1.0, lamps 1.3). Nothing blinks.
// Smallest x min of a TALL mesh: 37 m (a Site's anchor; its pieces' offsets out are 0 to 0.5 m). Smallest of all: 23.3 m (the low props).
#define S1D8(n) TEXT("/Game/World/ThemePark/SM_S1D8_" n)
Set(TEXT("WldS1D8ToadSeed"), { { S1D8("ToadHouse"), -9.25f, 0.f, 0.f, 6.f, 1.f }, { S1D8("SeedShop"), 9.25f, 0.5f, 0.f, -5.f, 1.f } });
Set(TEXT("WldS1D8BootBird"), { { S1D8("Boot"), -8.f, 0.f, 0.f, -5.f, 1.f }, { S1D8("Birdhouse"), 8.f, 1.f, 0.f, 7.f, 1.f } });
Set(TEXT("WldS1D8CanAcorn"), { { S1D8("Can"), -10.f, 0.f, 0.f, 4.f, 1.f }, { S1D8("Acorn"), 10.f, 0.5f, 0.f, -8.f, 1.f } });
Set(TEXT("WldS1D8PotsToad"), { { S1D8("PotTower"), -9.f, 0.5f, 0.f, 8.f, 1.f }, { S1D8("ToadHouse"), 9.f, 0.f, 0.f, -6.f, 1.f } });
Set(TEXT("WldS1D8SeedBoot"), { { S1D8("SeedShop"), -9.25f, 0.f, 0.f, 5.f, 1.f }, { S1D8("Boot"), 9.25f, 0.5f, 0.f, 6.f, 1.f } });
Set(TEXT("WldS1D8BirdCan"), { { S1D8("Birdhouse"), -10.f, 0.5f, 0.f, -7.f, 1.f }, { S1D8("Can"), 10.f, 0.f, 0.f, -4.f, 1.f } });
Set(TEXT("WldS1D8AcornPots"), { { S1D8("Acorn"), -8.25f, 0.f, 0.f, 8.f, 1.f }, { S1D8("PotTower"), 8.25f, 0.5f, 0.f, -8.f, 1.f } });
// fix round: one building only where the next slot's chair swing stands (right, s % 12 == 2: +6.3 to +24.2 m along, 41 to 59 m out)
Set(TEXT("WldS1D8SeedOnly"), { { S1D8("SeedShop"), -9.25f, 0.f, 0.f, 5.f, 1.f } });
Set(TEXT("WldS1D8BirdOnly"), { { S1D8("Birdhouse"), -10.f, 0.5f, 0.f, -7.f, 1.f } });
Set(TEXT("WldS1D8ToolsR"), { { S1D8("WashLine"), -13.f, 0.f, 0.f, 0.f, 1.f }, { S1D8("TrowelFork"), 0.f, 0.f, 0.f, 0.f, 1.f }, { S1D8("Lamp"), 9.5f, 0.3f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8KioskR"), { { S1D8("Lamp"), -11.f, 0.3f, 0.f, 0.f, 1.f }, { S1D8("SnailKiosk"), 0.f, 0.5f, 0.f, 0.f, 1.f }, { S1D8("Barrow"), 12.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8ToolsL"), { { S1D8("WashLine"), -14.f, 0.f, 0.f, 0.f, 1.f }, { S1D8("TrowelFork"), 7.f, 0.f, 0.f, 0.f, 1.f } });   // fix round: its lamp (+15.5) stood in the Gateway's fountain (+13 to +23 m along, 29 to 39 m out)
Set(TEXT("WldS1D8KioskL"), { { S1D8("Lamp"), -17.f, 0.3f, 0.f, 0.f, 1.f }, { S1D8("SnailKiosk"), 6.5f, 0.5f, 0.f, 0.f, 1.f } });   // fix round: the barrow stood in the Gateway's crowd (-9.8 to +1.8 m along), the kiosk at +9 in its fountain (from +13)
Set(TEXT("WldS1D8VergeA"), { { S1D8("PegFence"), -13.5f, 0.f, 0.f, 0.f, 1.f }, { S1D8("ToadSeats"), 0.f, 0.f, 0.f, 0.f, 1.f }, { S1D8("Thimbles"), 12.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8VergeB"), { { S1D8("Thimbles"), 14.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8VergeC"), { { S1D8("ToadSeats"), 14.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8VergeE"), { { S1D8("PegFence"), -14.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8VergeD"), { { S1D8("Thimbles"), -14.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8Teapot"), { { S1D8("Teapot"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Zone(TEXT("World s1d8, Giant's Garden"), 20.5f, 99.f);
	// fix round: four of the six left-hand rows are OFF here (chance -1) and written slot by slot in the s1fix block without slots 466, 494, 522 and 536,
	// where the dressing's far mascot (15 m tall, 58 to 70 m out, s % 7 == 4) stood between and in the two buildings
	Site(TEXT("WldS1D8ToadSeed"), -1, 63.f, 20, 18, -1.f);	// left, s % 20 == 2: 5 slots
	Site(TEXT("WldS1D8BootBird"), -1, 63.f, 20, 16, 1.f);	// left, s % 20 == 4: 5 slots
	Site(TEXT("WldS1D8CanAcorn"), -1, 63.f, 20, 14, -1.f);	// left, s % 20 == 6: 5 slots
	Site(TEXT("WldS1D8PotsToad"), -1, 63.f, 20, 8, 1.f);	// left, s % 20 == 12: 5 slots
	Site(TEXT("WldS1D8SeedBoot"), -1, 63.f, 20, 6, -1.f);	// left, s % 20 == 14: 5 slots
	Site(TEXT("WldS1D8BirdCan"), -1, 63.f, 20, 4, -1.f);	// left, s % 20 == 16: 5 slots
	Site(TEXT("WldS1D8AcornPots"), 1, 55.f, 1000, 534, 1.f);	// right, slot 466 (21.08 km)
	Site(TEXT("WldS1D8BirdCan"), 1, 60.f, 1000, 526, 1.f);	// right, slot 474 (21.45 km)
	Site(TEXT("WldS1D8SeedOnly"), 1, 51.5f, 1000, 518, 1.f);	// right, slot 482 (21.81 km); fix round: the boot stood in the chair swing
	Site(TEXT("WldS1D8AcornPots"), 1, 51.5f, 1000, 0, 1.f);	// fix round: was slot 494 (phase 506): the acorn stood in SM_P3A_Terrace, the pot tower in the chair swing, the far mascot between. Phase 0 places nothing; the rule stays so later rules keep their numbers
	Site(TEXT("WldS1D8BirdOnly"), 1, 51.5f, 1000, 494, 1.f);	// right, slot 506 (22.89 km); fix round: the can stood in the chair swing and SM_P3A_Train
	Site(TEXT("WldS1D8SeedBoot"), 1, 55.f, 1000, 486, 1.f);	// right, slot 514 (23.25 km)
	Site(TEXT("WldS1FixPotsOnly"), 1, 60.f, 1000, 478, 1.f);	// right, slot 522 (23.62 km); fix round: the acorn stood in the far mascot (the set is in the s1fix block)
	Site(TEXT("WldS1D8BirdOnly"), 1, 51.5f, 1000, 458, 1.f);	// right, slot 542 (24.52 km); fix round: the can stood in the chair swing
	Site(TEXT("WldS1D8AcornPots"), 1, 60.f, 1000, 454, 1.f);	// right, slot 546 (24.70 km)
	Site(TEXT("WldS1D8Teapot"), 1, 130.f, 1000, 538, 1.f);	// THE GIANT'S TEAPOT, right, slot 462 (20.90 km)
	Site(TEXT("WldS1D8Teapot"), 1, 130.f, 1000, 506, 1.f);	// THE GIANT'S TEAPOT, right, slot 494 (22.35 km)
	Site(TEXT("WldS1D8Teapot"), 1, 130.f, 1000, 476, 1.f);	// THE GIANT'S TEAPOT, right, slot 524 (23.71 km)
Zone(TEXT("World s1d8, Giant's Garden, verge 1"), 20.5f, 21.29f);
	// fix round: ToolsR and KioskR are OFF in all four verges (chance -1 places nothing; the rules stay so later rules keep their numbers): pass 3's stages
	// (SM_P3B_*, 35 to 44 m out, 17 m long, sliding 11 m either way) stand on every one of these slots and 44 of the 66 props stood in one. The 22 that
	// the dice leave clear are placed one by one in the s1fix block at the end of this file.
	Site(TEXT("WldS1D8ToolsR"), 1, 37.f, 8, 6, -1.f);	// right, s % 8 == 2: behind the parade lane and the planets on posts
	Site(TEXT("WldS1D8KioskR"), 1, 37.f, 8, 2, -1.f);	// right, s % 8 == 6
	Site(TEXT("WldS1D8ToolsL"), -1, 37.5f, 24, 16, 1.f);	// left, s % 24 == 8: behind a stall row (its back wall 33.1 m)
	Site(TEXT("WldS1D8KioskL"), -1, 37.5f, 24, 4, 1.f);	// left, s % 24 == 20 (fix round: no such slot in verge 1 or verge 2, so those two rules place nothing; left in so later rules keep their numbers)
	Site(TEXT("WldS1D8VergeA"), -1, 23.3f, 12, 10, 1.f);	// left, s % 12 == 2: behind the queue rails
	Site(TEXT("WldS1D8VergeA"), -1, 23.3f, 12, 4, 1.f);	// left, s % 12 == 8
	Site(TEXT("WldS1D8VergeA"), 1, 23.3f, 12, 8, 1.f);	// right, s % 12 == 4: behind the benches
	Site(TEXT("WldS1D8VergeA"), 1, 23.3f, 12, 2, 1.f);	// right, s % 12 == 10
	Site(TEXT("WldS1D8VergeB"), -1, 23.3f, 12, 8, 1.f);	// left, s % 12 == 4: either end of a kerb group (21 m)
	Site(TEXT("WldS1D8VergeC"), -1, 23.3f, 12, 2, 1.f);	// left, s % 12 == 10
	Site(TEXT("WldS1D8VergeD"), 1, 23.3f, 12, 0, 1.f);	// right, s % 12 == 0
	Site(TEXT("WldS1D8VergeE"), 1, 23.3f, 12, 6, 1.f);	// right, s % 12 == 6
Zone(TEXT("World s1d8, Giant's Garden, verge 2"), 21.56f, 22.56f);
	Site(TEXT("WldS1D8ToolsR"), 1, 37.f, 8, 6, -1.f);
	Site(TEXT("WldS1D8KioskR"), 1, 37.f, 8, 2, -1.f);
	Site(TEXT("WldS1D8ToolsL"), -1, 37.5f, 24, 16, 1.f);
	Site(TEXT("WldS1D8KioskL"), -1, 37.5f, 24, 4, 1.f);
	Site(TEXT("WldS1D8VergeA"), -1, 23.3f, 12, 10, 1.f);
	Site(TEXT("WldS1D8VergeA"), -1, 23.3f, 12, 4, 1.f);
	Site(TEXT("WldS1D8VergeA"), 1, 23.3f, 12, 8, 1.f);
	Site(TEXT("WldS1D8VergeA"), 1, 23.3f, 12, 2, 1.f);
	Site(TEXT("WldS1D8VergeB"), -1, 23.3f, 12, 8, 1.f);
	Site(TEXT("WldS1D8VergeC"), -1, 23.3f, 12, 2, 1.f);
	Site(TEXT("WldS1D8VergeD"), 1, 23.3f, 12, 0, 1.f);
	Site(TEXT("WldS1D8VergeE"), 1, 23.3f, 12, 6, 1.f);
Zone(TEXT("World s1d8, Giant's Garden, verge 3"), 22.83f, 23.82f);
	Site(TEXT("WldS1D8ToolsR"), 1, 37.f, 8, 6, -1.f);
	Site(TEXT("WldS1D8KioskR"), 1, 37.f, 8, 2, -1.f);
	Site(TEXT("WldS1D8ToolsL"), -1, 37.5f, 24, 16, 1.f);
	Site(TEXT("WldS1D8KioskL"), -1, 37.5f, 24, 4, 1.f);
	Site(TEXT("WldS1D8VergeA"), -1, 23.3f, 12, 10, 1.f);
	Site(TEXT("WldS1D8VergeA"), -1, 23.3f, 12, 4, 1.f);
	Site(TEXT("WldS1D8VergeA"), 1, 23.3f, 12, 8, 1.f);
	Site(TEXT("WldS1D8VergeA"), 1, 23.3f, 12, 2, 1.f);
	Site(TEXT("WldS1D8VergeB"), -1, 23.3f, 12, 8, 1.f);
	Site(TEXT("WldS1D8VergeC"), -1, 23.3f, 12, 2, 1.f);
	Site(TEXT("WldS1D8VergeD"), 1, 23.3f, 12, 0, 1.f);
	Site(TEXT("WldS1D8VergeE"), 1, 23.3f, 12, 6, 1.f);
Zone(TEXT("World s1d8, Giant's Garden, verge 4"), 24.09f, 99.f);
	Site(TEXT("WldS1D8ToolsR"), 1, 37.f, 8, 6, -1.f);
	Site(TEXT("WldS1D8KioskR"), 1, 37.f, 8, 2, -1.f);
	Site(TEXT("WldS1D8ToolsL"), -1, 37.5f, 24, 16, 1.f);
	Site(TEXT("WldS1D8KioskL"), -1, 37.5f, 24, 4, 1.f);
	Site(TEXT("WldS1D8VergeA"), -1, 23.3f, 12, 10, 1.f);
	Site(TEXT("WldS1D8VergeA"), -1, 23.3f, 12, 4, 1.f);
	Site(TEXT("WldS1D8VergeA"), 1, 23.3f, 12, 8, 1.f);
	Site(TEXT("WldS1D8VergeA"), 1, 23.3f, 12, 2, 1.f);
	Site(TEXT("WldS1D8VergeB"), -1, 23.3f, 12, 8, 1.f);
	Site(TEXT("WldS1D8VergeC"), -1, 23.3f, 12, 2, 1.f);
	Site(TEXT("WldS1D8VergeD"), 1, 23.3f, 12, 0, 1.f);
	Site(TEXT("WldS1D8VergeE"), 1, 23.3f, 12, 6, 1.f);
#undef S1D8
// ---- END s1d8 ----
}

{
// ---- BEGIN s1plants (plants of all eight districts, ft_world_park_s1plants.py; this block is written by C:/ftwork/jobs/L4s1plants/place.py) ----
// 32 meshes, /Game/World/ThemePark/SM_S1PLANTS_*. Every rule is for ONE slot of the plants' slots (s % 4 == 3): every 1000, phase 1000 - s
// (so phase % 4 == 1), chance 1. S1PLANTS_AT: a Scatter with x min = x max, one plant. S1PLANTS_PAIR: a Site of a two-piece Set, two LOW
// pieces 11.5 m either side of the slot's middle. The slot and x of each rule were chosen by place.py against every rule of FTDressL4.inl
// and FTPeopleL4.inl in that slot and side (and the Stage 1 blocks that were in C:/ftwork/s1 when it ran): a plant stands in the first gap
// across the road that takes it. The level is full: most TALL plants stand 47 to 60 m out. If a dressing or people rule changes, run place.py again.
// LOW pieces (unlit, top under 2 m, bounds under 2.5 m): 23.0 m out behind the verge; the lily rafts 20.6 m out on the lagoon's valley floor.
// Nearest low bounds 18.78 m. TALL pieces (a steady glow, nothing blinks): whole bounds, scale jitter included, 24 m or more out: nearest 24.15 m
// (Papyrus, rule x 29.7); beside a banked bend's inside and at both jumps 40 m or more out; the dandelion clocks 30 m or more; no plant itself past 60.2 m (largest rule x 59.2).
// Estimate: 270 Scatter rules + 174 Site rules x 2 pieces = 618 pieces tried, x 0.75 (the track's share left out) = about 464 placed.
// By mesh (pieces tried): BeanPoles 3, BlossomCones 17, BlossomSwirl 3, Bluebells 18, BulbPlane 2, CactusTins 9, CandyTulips 48, CloverPatch 60, Cypress 19, DandelionClocks 9, FrostBirch 2, GiantBulrush 19, GiantFern 3, Hibiscus 7, HollyLow 60, LavenderBed 29, LilyRaft 60, LotusTower 26, Mangrove 2, MoonVine 22, Papyrus 19, RoseStandards 45, SeaGrassDrift 12, SilverSpirals 22, SnowFirGroup 10, SnowFirTall 6, Sunflower 11, TopiaryMoon 22, TopiaryTiers 11, TopiaryTubs 25, Willow 3, WisteriaArch 14.
// By district (pieces tried): s1d1 45, s1d2 18, s1d3 126, s1d4 62, s1d5 68, s1d6 114, s1d7 78, s1d8 107. The most of one mesh: LilyRaft, 60 tried (about 45 placed).
// A lambda a district, so that the population function stays small for the compiler (as pass 3b did).
#define S1PLANTS(n) TEXT("/Game/World/ThemePark/SM_S1PLANTS_" n)
#define S1PLANTS_AT(n, s, side, x, jit, yawj) Scatter(S1PLANTS(n), x, x, 1000, 1000 - (s), 1.f, 1.f, jit, side, yawj)
#define S1PLANTS_PAIR(set, s, side, x) Site(TEXT("WldS1PLANTSLow" set), side, x, 1000, 1000 - (s), 1.f)
Set(TEXT("WldS1PLANTSLowCactusTinsTopiaryTubs"), { { S1PLANTS("CactusTins"), -11.5f, 0.f, 0.f, 0.f, 1.f }, { S1PLANTS("TopiaryTubs"), 11.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTSLowCandyTulipsCandyTulips"), { { S1PLANTS("CandyTulips"), -11.5f, 0.f, 0.f, 0.f, 1.f }, { S1PLANTS("CandyTulips"), 11.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTSLowCloverPatchCloverPatch"), { { S1PLANTS("CloverPatch"), -11.5f, 0.f, 0.f, 0.f, 1.f }, { S1PLANTS("CloverPatch"), 11.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTSLowHollyLowHollyLow"), { { S1PLANTS("HollyLow"), -11.5f, 0.f, 0.f, 0.f, 1.f }, { S1PLANTS("HollyLow"), 11.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTSLowLavenderBedRoseStandards"), { { S1PLANTS("LavenderBed"), -11.5f, 0.f, 0.f, 0.f, 1.f }, { S1PLANTS("RoseStandards"), 11.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTSLowLilyRaftLilyRaft"), { { S1PLANTS("LilyRaft"), -11.5f, 0.f, 0.f, 0.f, 1.f }, { S1PLANTS("LilyRaft"), 11.5f, 0.f, 0.f, 180.f, 1.f } });
Set(TEXT("WldS1PLANTSLowRoseStandardsLavenderBed"), { { S1PLANTS("RoseStandards"), -11.5f, 0.f, 0.f, 0.f, 1.f }, { S1PLANTS("LavenderBed"), 11.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTSLowRoseStandardsTopiaryTubs"), { { S1PLANTS("RoseStandards"), -11.5f, 0.f, 0.f, 0.f, 1.f }, { S1PLANTS("TopiaryTubs"), 11.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTSLowSeaGrassDriftSeaGrassDrift"), { { S1PLANTS("SeaGrassDrift"), -11.5f, 0.f, 0.f, 0.f, 1.f }, { S1PLANTS("SeaGrassDrift"), 11.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTSLowTopiaryTubsCactusTins"), { { S1PLANTS("TopiaryTubs"), -11.5f, 0.f, 0.f, 0.f, 1.f }, { S1PLANTS("CactusTins"), 11.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTSLowTopiaryTubsRoseStandards"), { { S1PLANTS("TopiaryTubs"), -11.5f, 0.f, 0.f, 0.f, 1.f }, { S1PLANTS("RoseStandards"), 11.5f, 0.f, 0.f, 0.f, 1.f } });
const auto WldS1PLANTSDistrict1 = [&]()
{
Zone(TEXT("World s1plants, Front Gate and Midway"), 0.0f, 2.22f);    // 45 pieces tried: BulbPlane 2, RoseStandards 16, TopiaryTiers 11, TopiaryTubs 16
	S1PLANTS_PAIR("TopiaryTubsRoseStandards", 3, -1, 23.0f); S1PLANTS_PAIR("RoseStandardsTopiaryTubs", 3, 1, 23.0f); S1PLANTS_PAIR("RoseStandardsTopiaryTubs", 7, -1, 23.0f); S1PLANTS_PAIR("TopiaryTubsRoseStandards", 7, 1, 23.0f);
	S1PLANTS_PAIR("TopiaryTubsRoseStandards", 11, -1, 23.0f); S1PLANTS_PAIR("RoseStandardsTopiaryTubs", 11, 1, 23.0f); S1PLANTS_PAIR("RoseStandardsTopiaryTubs", 15, -1, 23.0f); S1PLANTS_PAIR("TopiaryTubsRoseStandards", 19, -1, 23.0f);
	S1PLANTS_PAIR("RoseStandardsTopiaryTubs", 19, 1, 23.0f); S1PLANTS_PAIR("RoseStandardsTopiaryTubs", 23, -1, 23.0f); S1PLANTS_PAIR("TopiaryTubsRoseStandards", 27, -1, 23.0f); S1PLANTS_PAIR("RoseStandardsTopiaryTubs", 27, 1, 23.0f);
	S1PLANTS_PAIR("RoseStandardsTopiaryTubs", 31, -1, 23.0f); S1PLANTS_PAIR("TopiaryTubsRoseStandards", 31, 1, 23.0f); S1PLANTS_PAIR("TopiaryTubsRoseStandards", 35, -1, 23.0f); S1PLANTS_PAIR("TopiaryTubsRoseStandards", 39, 1, 23.0f);
	S1PLANTS_AT("TopiaryTiers", 3, -1, 56.0f, 0.10f, 180.0f); S1PLANTS_AT("TopiaryTiers", 3, 1, 56.0f, 0.10f, 180.0f); S1PLANTS_AT("TopiaryTiers", 7, -1, 42.5f, 0.10f, 180.0f);
	S1PLANTS_AT("TopiaryTiers", 7, 1, 42.0f, 0.10f, 180.0f); S1PLANTS_AT("TopiaryTiers", 11, -1, 53.0f, 0.10f, 180.0f); S1PLANTS_AT("BulbPlane", 11, 1, 49.7f, 0.10f, 180.0f);
	S1PLANTS_AT("TopiaryTiers", 15, 1, 56.0f, 0.10f, 180.0f); S1PLANTS_AT("TopiaryTiers", 19, -1, 42.5f, 0.10f, 180.0f); S1PLANTS_AT("TopiaryTiers", 19, 1, 42.0f, 0.10f, 180.0f);
	S1PLANTS_AT("TopiaryTiers", 23, -1, 53.0f, 0.10f, 180.0f); S1PLANTS_AT("TopiaryTiers", 23, 1, 47.5f, 0.10f, 180.0f); S1PLANTS_AT("BulbPlane", 31, -1, 54.2f, 0.10f, 180.0f);
	S1PLANTS_AT("TopiaryTiers", 31, 1, 52.0f, 0.10f, 180.0f);
};
WldS1PLANTSDistrict1();
const auto WldS1PLANTSDistrict2 = [&]()
{
Zone(TEXT("World s1plants, Pirate Cove"), 2.22f, 3.21f);    // 18 pieces tried: Hibiscus 4, Mangrove 2, SeaGrassDrift 12
	S1PLANTS_PAIR("SeaGrassDriftSeaGrassDrift", 51, -1, 23.0f); S1PLANTS_PAIR("SeaGrassDriftSeaGrassDrift", 51, 1, 23.0f); S1PLANTS_PAIR("SeaGrassDriftSeaGrassDrift", 55, -1, 23.0f); S1PLANTS_PAIR("SeaGrassDriftSeaGrassDrift", 55, 1, 23.0f);
	S1PLANTS_PAIR("SeaGrassDriftSeaGrassDrift", 59, 1, 23.0f); S1PLANTS_PAIR("SeaGrassDriftSeaGrassDrift", 63, 1, 23.0f);
	S1PLANTS_AT("Mangrove", 51, -1, 50.7f, 0.10f, 180.0f); S1PLANTS_AT("Hibiscus", 51, -1, 57.8f, 0.04f, 0.f); S1PLANTS_AT("Hibiscus", 51, 1, 33.3f, 0.04f, 0.f);
	S1PLANTS_AT("Hibiscus", 55, -1, 49.3f, 0.04f, 0.f); S1PLANTS_AT("Mangrove", 55, -1, 56.7f, 0.10f, 180.0f); S1PLANTS_AT("Hibiscus", 59, 1, 33.3f, 0.04f, 0.f);
};
WldS1PLANTSDistrict2();
const auto WldS1PLANTSDistrict3 = [&]()
{
// fix round: (a) a rule with slot 0 is OFF (it places nothing and stays, so later rules keep their numbers): that plant stood, as the dice
// slide it, on the dressing's lit path (PathCross: 9.7 m wide at the middle of every odd slot on the lagoon's right, 8 to 41 m out), in a chair
// swing, a fountain or (not the water plants) a pond. (b) the tall stems that stood 28.0 to 30.3 m out stand 2 m further: their bounds reached
// 22.2 to 22.9 m and the track moved every one it kept.
Zone(TEXT("World s1plants, The Lagoon"), 3.21f, 6.9074f);    // 126 pieces tried: GiantBulrush 19, LilyRaft 60, LotusTower 26, Papyrus 19, Willow 2
	S1PLANTS_PAIR("LilyRaftLilyRaft", 75, -1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 75, 1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 79, -1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 79, 1, 20.6f);
	S1PLANTS_PAIR("LilyRaftLilyRaft", 83, -1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 83, 1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 87, -1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 87, 1, 20.6f);
	S1PLANTS_PAIR("LilyRaftLilyRaft", 91, -1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 91, 1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 95, -1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 95, 1, 20.6f);
	S1PLANTS_PAIR("LilyRaftLilyRaft", 99, -1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 99, 1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 103, -1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 103, 1, 20.6f);
	S1PLANTS_PAIR("LilyRaftLilyRaft", 107, -1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 107, 1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 111, -1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 111, 1, 20.6f);
	S1PLANTS_PAIR("LilyRaftLilyRaft", 115, -1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 115, 1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 119, -1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 119, 1, 20.6f);
	S1PLANTS_PAIR("LilyRaftLilyRaft", 123, -1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 123, 1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 127, -1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 127, 1, 20.6f);
	S1PLANTS_PAIR("LilyRaftLilyRaft", 131, -1, 20.6f); S1PLANTS_PAIR("LilyRaftLilyRaft", 131, 1, 20.6f);
	S1PLANTS_AT("Willow", 71, -1, 31.3f, 0.10f, 180.0f); S1PLANTS_AT("Willow", 0, 1, 30.3f, 0.10f, 180.0f); S1PLANTS_AT("GiantBulrush", 75, -1, 30.0f, 0.10f, 180.0f);
	S1PLANTS_AT("LotusTower", 75, -1, 36.8f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 75, 1, 32.3f, 0.10f, 180.0f); S1PLANTS_AT("Papyrus", 0, 1, 40.7f, 0.10f, 180.0f);
	S1PLANTS_AT("LotusTower", 79, -1, 31.8f, 0.10f, 180.0f); S1PLANTS_AT("Papyrus", 0, 1, 30.2f, 0.10f, 180.0f); S1PLANTS_AT("Papyrus", 83, -1, 31.7f, 0.10f, 180.0f);
	S1PLANTS_AT("GiantBulrush", 83, -1, 38.5f, 0.10f, 180.0f); S1PLANTS_AT("GiantBulrush", 83, 1, 30.5f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 0, 1, 37.3f, 0.10f, 180.0f);
	S1PLANTS_AT("GiantBulrush", 87, -1, 30.0f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 87, -1, 36.8f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 0, 1, 30.3f, 0.10f, 180.0f);
	S1PLANTS_AT("Papyrus", 0, 1, 40.7f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 91, -1, 31.8f, 0.10f, 180.0f); S1PLANTS_AT("Papyrus", 91, 1, 32.2f, 0.10f, 180.0f);
	S1PLANTS_AT("Papyrus", 95, -1, 31.7f, 0.10f, 180.0f); S1PLANTS_AT("GiantBulrush", 95, -1, 38.5f, 0.10f, 180.0f); S1PLANTS_AT("GiantBulrush", 0, 1, 28.5f, 0.10f, 180.0f);
	S1PLANTS_AT("LotusTower", 95, 1, 37.3f, 0.10f, 180.0f); S1PLANTS_AT("GiantBulrush", 99, -1, 30.0f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 99, -1, 36.8f, 0.10f, 180.0f);
	S1PLANTS_AT("LotusTower", 0, 1, 30.3f, 0.10f, 180.0f); S1PLANTS_AT("Papyrus", 0, 1, 40.7f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 103, -1, 31.8f, 0.10f, 180.0f);
	S1PLANTS_AT("Papyrus", 0, 1, 30.2f, 0.10f, 180.0f); S1PLANTS_AT("Papyrus", 107, -1, 31.7f, 0.10f, 180.0f); S1PLANTS_AT("GiantBulrush", 107, -1, 38.5f, 0.10f, 180.0f);
	S1PLANTS_AT("GiantBulrush", 107, 1, 30.5f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 0, 1, 37.3f, 0.10f, 180.0f); S1PLANTS_AT("GiantBulrush", 111, -1, 30.0f, 0.10f, 180.0f);
	S1PLANTS_AT("LotusTower", 111, -1, 36.8f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 0, 1, 30.3f, 0.10f, 180.0f); S1PLANTS_AT("Papyrus", 0, 1, 40.7f, 0.10f, 180.0f);
	S1PLANTS_AT("LotusTower", 115, -1, 31.8f, 0.10f, 180.0f); S1PLANTS_AT("Papyrus", 0, 1, 30.2f, 0.10f, 180.0f); S1PLANTS_AT("Papyrus", 119, -1, 31.7f, 0.10f, 180.0f);
	S1PLANTS_AT("GiantBulrush", 119, -1, 38.5f, 0.10f, 180.0f); S1PLANTS_AT("GiantBulrush", 0, 1, 28.5f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 119, 1, 37.3f, 0.10f, 180.0f);
	S1PLANTS_AT("GiantBulrush", 123, -1, 30.0f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 123, -1, 36.8f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 0, 1, 30.3f, 0.10f, 180.0f);
	S1PLANTS_AT("Papyrus", 0, 1, 40.7f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 127, -1, 31.8f, 0.10f, 180.0f); S1PLANTS_AT("Papyrus", 0, 1, 30.2f, 0.10f, 180.0f);
	S1PLANTS_AT("Papyrus", 131, -1, 31.7f, 0.10f, 180.0f); S1PLANTS_AT("GiantBulrush", 131, -1, 38.5f, 0.10f, 180.0f); S1PLANTS_AT("GiantBulrush", 131, 1, 30.5f, 0.10f, 180.0f);
	S1PLANTS_AT("LotusTower", 0, 1, 37.3f, 0.10f, 180.0f); S1PLANTS_AT("GiantBulrush", 135, -1, 30.0f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 135, -1, 36.8f, 0.10f, 180.0f);
	S1PLANTS_AT("LotusTower", 135, 1, 32.3f, 0.10f, 180.0f); S1PLANTS_AT("Papyrus", 0, 1, 40.7f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 139, -1, 31.8f, 0.10f, 180.0f);
	S1PLANTS_AT("Papyrus", 0, 1, 30.2f, 0.10f, 180.0f); S1PLANTS_AT("Papyrus", 143, -1, 31.7f, 0.10f, 180.0f); S1PLANTS_AT("GiantBulrush", 143, -1, 38.5f, 0.10f, 180.0f);
	S1PLANTS_AT("GiantBulrush", 0, 1, 28.5f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 143, 1, 37.3f, 0.10f, 180.0f); S1PLANTS_AT("GiantBulrush", 147, -1, 30.0f, 0.10f, 180.0f);
	S1PLANTS_AT("LotusTower", 147, -1, 36.8f, 0.10f, 180.0f); S1PLANTS_AT("LotusTower", 0, 1, 30.3f, 0.10f, 180.0f); S1PLANTS_AT("Papyrus", 147, 1, 40.7f, 0.10f, 180.0f);
};
WldS1PLANTSDistrict3();
const auto WldS1PLANTSDistrict4 = [&]()
{
Zone(TEXT("World s1plants, Rocket Land and Funhouse"), 6.9074f, 9.2974f);    // 62 pieces tried: CactusTins 9, MoonVine 22, SilverSpirals 22, TopiaryTubs 9
	S1PLANTS_PAIR("CactusTinsTopiaryTubs", 155, -1, 23.0f); S1PLANTS_PAIR("CactusTinsTopiaryTubs", 163, -1, 23.0f); S1PLANTS_PAIR("TopiaryTubsCactusTins", 163, 1, 23.0f); S1PLANTS_PAIR("CactusTinsTopiaryTubs", 167, 1, 23.0f);
	S1PLANTS_PAIR("CactusTinsTopiaryTubs", 171, -1, 23.0f); S1PLANTS_PAIR("TopiaryTubsCactusTins", 175, -1, 23.0f); S1PLANTS_PAIR("CactusTinsTopiaryTubs", 179, -1, 23.0f); S1PLANTS_PAIR("TopiaryTubsCactusTins", 179, 1, 23.0f);
	S1PLANTS_PAIR("CactusTinsTopiaryTubs", 183, 1, 23.0f);
	S1PLANTS_AT("SilverSpirals", 155, -1, 52.0f, 0.04f, 0.f); S1PLANTS_AT("MoonVine", 155, -1, 55.4f, 0.04f, 0.f); S1PLANTS_AT("MoonVine", 155, 1, 49.9f, 0.04f, 0.f);
	S1PLANTS_AT("SilverSpirals", 155, 1, 53.5f, 0.04f, 0.f); S1PLANTS_AT("MoonVine", 159, -1, 46.9f, 0.04f, 0.f); S1PLANTS_AT("SilverSpirals", 159, -1, 50.5f, 0.04f, 0.f);
	S1PLANTS_AT("SilverSpirals", 159, 1, 51.0f, 0.04f, 0.f); S1PLANTS_AT("MoonVine", 159, 1, 54.4f, 0.04f, 0.f); S1PLANTS_AT("MoonVine", 163, -1, 54.9f, 0.04f, 0.f);
	S1PLANTS_AT("MoonVine", 163, 1, 48.9f, 0.04f, 0.f); S1PLANTS_AT("SilverSpirals", 163, 1, 52.5f, 0.04f, 0.f); S1PLANTS_AT("MoonVine", 167, -1, 52.4f, 0.04f, 0.f);
	S1PLANTS_AT("SilverSpirals", 167, -1, 56.0f, 0.04f, 0.f); S1PLANTS_AT("SilverSpirals", 167, 1, 49.5f, 0.04f, 0.f); S1PLANTS_AT("MoonVine", 167, 1, 52.9f, 0.04f, 0.f);
	S1PLANTS_AT("SilverSpirals", 171, -1, 52.0f, 0.04f, 0.f); S1PLANTS_AT("MoonVine", 171, -1, 55.4f, 0.04f, 0.f); S1PLANTS_AT("MoonVine", 171, 1, 49.9f, 0.04f, 0.f);
	S1PLANTS_AT("SilverSpirals", 171, 1, 53.5f, 0.04f, 0.f); S1PLANTS_AT("MoonVine", 175, -1, 46.9f, 0.04f, 0.f); S1PLANTS_AT("SilverSpirals", 175, 1, 49.5f, 0.04f, 0.f);
	S1PLANTS_AT("MoonVine", 175, 1, 52.9f, 0.04f, 0.f); S1PLANTS_AT("SilverSpirals", 179, -1, 47.5f, 0.04f, 0.f); S1PLANTS_AT("MoonVine", 179, -1, 57.9f, 0.04f, 0.f);
	S1PLANTS_AT("MoonVine", 179, 1, 48.4f, 0.04f, 0.f); S1PLANTS_AT("SilverSpirals", 179, 1, 52.0f, 0.04f, 0.f); S1PLANTS_AT("MoonVine", 183, -1, 41.4f, 0.04f, 0.f);
	S1PLANTS_AT("SilverSpirals", 0, -1, 52.5f, 0.04f, 0.f); S1PLANTS_AT("SilverSpirals", 183, 1, 36.0f, 0.04f, 0.f); S1PLANTS_AT("MoonVine", 183, 1, 49.4f, 0.04f, 0.f);
	S1PLANTS_AT("SilverSpirals", 187, -1, 53.5f, 0.04f, 0.f); S1PLANTS_AT("MoonVine", 191, -1, 41.9f, 0.04f, 0.f); S1PLANTS_AT("SilverSpirals", 191, -1, 53.5f, 0.04f, 0.f);
	S1PLANTS_AT("SilverSpirals", 191, 1, 42.0f, 0.04f, 0.f); S1PLANTS_AT("SilverSpirals", 195, -1, 42.0f, 0.04f, 0.f); S1PLANTS_AT("MoonVine", 195, -1, 52.9f, 0.04f, 0.f);
	S1PLANTS_AT("MoonVine", 199, -1, 39.9f, 0.04f, 0.f); S1PLANTS_AT("SilverSpirals", 199, -1, 43.5f, 0.04f, 0.f); S1PLANTS_AT("SilverSpirals", 199, 1, 52.0f, 0.04f, 0.f);
	S1PLANTS_AT("MoonVine", 199, 1, 39.9f, 0.04f, 0.f); S1PLANTS_AT("SilverSpirals", 203, -1, 32.0f, 0.04f, 0.f); S1PLANTS_AT("MoonVine", 203, -1, 35.4f, 0.04f, 0.f);
	S1PLANTS_AT("MoonVine", 203, 1, 46.4f, 0.04f, 0.f); S1PLANTS_AT("SilverSpirals", 203, 1, 52.0f, 0.04f, 0.f);
};
WldS1PLANTSDistrict4();
const auto WldS1PLANTSDistrict5 = [&]()
{
Zone(TEXT("World s1plants, Sweet Street"), 9.2974f, 11.94f);    // 68 pieces tried: BlossomCones 17, BlossomSwirl 3, CandyTulips 48
	S1PLANTS_PAIR("CandyTulipsCandyTulips", 207, -1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 211, 1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 215, -1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 215, 1, 23.0f);
	S1PLANTS_PAIR("CandyTulipsCandyTulips", 219, -1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 223, 1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 227, -1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 227, 1, 23.0f);
	S1PLANTS_PAIR("CandyTulipsCandyTulips", 231, -1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 235, 1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 239, -1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 239, 1, 23.0f);
	S1PLANTS_PAIR("CandyTulipsCandyTulips", 243, -1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 243, 1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 247, -1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 247, 1, 23.0f);
	S1PLANTS_PAIR("CandyTulipsCandyTulips", 251, -1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 251, 1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 255, -1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 255, 1, 23.0f);
	S1PLANTS_PAIR("CandyTulipsCandyTulips", 259, -1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 259, 1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 263, -1, 23.0f); S1PLANTS_PAIR("CandyTulipsCandyTulips", 263, 1, 23.0f);
	S1PLANTS_AT("BlossomCones", 211, -1, 25.7f, 0.04f, 0.f); S1PLANTS_AT("BlossomCones", 211, 1, 55.2f, 0.04f, 0.f); S1PLANTS_AT("BlossomCones", 215, -1, 25.7f, 0.04f, 0.f);
	S1PLANTS_AT("BlossomCones", 215, 1, 48.2f, 0.04f, 0.f); S1PLANTS_AT("BlossomCones", 219, -1, 25.7f, 0.04f, 0.f); S1PLANTS_AT("BlossomCones", 219, 1, 59.2f, 0.04f, 0.f);
	S1PLANTS_AT("BlossomCones", 227, -1, 25.7f, 0.04f, 0.f); S1PLANTS_AT("BlossomCones", 227, 1, 48.2f, 0.04f, 0.f); S1PLANTS_AT("BlossomSwirl", 227, 1, 57.2f, 0.10f, 180.0f);
	S1PLANTS_AT("BlossomCones", 231, -1, 56.2f, 0.04f, 0.f); S1PLANTS_AT("BlossomCones", 231, 1, 59.2f, 0.04f, 0.f); S1PLANTS_AT("BlossomCones", 235, -1, 25.7f, 0.04f, 0.f);
	S1PLANTS_AT("BlossomCones", 239, -1, 25.7f, 0.04f, 0.f); S1PLANTS_AT("BlossomSwirl", 239, 1, 57.2f, 0.10f, 180.0f); S1PLANTS_AT("BlossomCones", 239, 1, 48.2f, 0.04f, 0.f);
	S1PLANTS_AT("BlossomCones", 243, 1, 31.2f, 0.04f, 0.f); S1PLANTS_AT("BlossomSwirl", 247, 1, 57.2f, 0.10f, 180.0f); S1PLANTS_AT("BlossomCones", 251, -1, 25.7f, 0.04f, 0.f);
	S1PLANTS_AT("BlossomCones", 259, -1, 25.7f, 0.04f, 0.f); S1PLANTS_AT("BlossomCones", 259, 1, 55.2f, 0.04f, 0.f);
};
WldS1PLANTSDistrict5();
const auto WldS1PLANTSDistrict6 = [&]()
{
Zone(TEXT("World s1plants, Crescent Keep"), 11.94f, 16.1f);    // 114 pieces tried: Cypress 19, LavenderBed 29, RoseStandards 29, TopiaryMoon 22, Willow 1, WisteriaArch 14
	S1PLANTS_PAIR("LavenderBedRoseStandards", 267, -1, 23.0f); S1PLANTS_PAIR("LavenderBedRoseStandards", 271, 1, 23.0f); S1PLANTS_PAIR("LavenderBedRoseStandards", 275, -1, 23.0f); S1PLANTS_PAIR("RoseStandardsLavenderBed", 275, 1, 23.0f);
	S1PLANTS_PAIR("RoseStandardsLavenderBed", 279, -1, 23.0f); S1PLANTS_PAIR("RoseStandardsLavenderBed", 283, 1, 23.0f); S1PLANTS_PAIR("RoseStandardsLavenderBed", 287, -1, 23.0f); S1PLANTS_PAIR("LavenderBedRoseStandards", 287, 1, 23.0f);
	S1PLANTS_PAIR("LavenderBedRoseStandards", 291, -1, 23.0f); S1PLANTS_PAIR("LavenderBedRoseStandards", 295, 1, 23.0f); S1PLANTS_PAIR("LavenderBedRoseStandards", 299, -1, 23.0f); S1PLANTS_PAIR("RoseStandardsLavenderBed", 299, 1, 23.0f);
	S1PLANTS_PAIR("RoseStandardsLavenderBed", 303, -1, 23.0f); S1PLANTS_PAIR("RoseStandardsLavenderBed", 307, 1, 23.0f); S1PLANTS_PAIR("RoseStandardsLavenderBed", 311, -1, 23.0f); S1PLANTS_PAIR("LavenderBedRoseStandards", 311, 1, 23.0f);
	S1PLANTS_PAIR("LavenderBedRoseStandards", 315, -1, 23.0f); S1PLANTS_PAIR("LavenderBedRoseStandards", 319, 1, 23.0f); S1PLANTS_PAIR("LavenderBedRoseStandards", 323, -1, 23.0f); S1PLANTS_PAIR("RoseStandardsLavenderBed", 323, 1, 23.0f);
	S1PLANTS_PAIR("RoseStandardsLavenderBed", 327, -1, 23.0f); S1PLANTS_PAIR("RoseStandardsLavenderBed", 331, 1, 23.0f); S1PLANTS_PAIR("LavenderBedRoseStandards", 335, 1, 23.0f); S1PLANTS_PAIR("LavenderBedRoseStandards", 339, -1, 23.0f);
	S1PLANTS_PAIR("LavenderBedRoseStandards", 343, 1, 23.0f); S1PLANTS_PAIR("LavenderBedRoseStandards", 347, -1, 23.0f); S1PLANTS_PAIR("RoseStandardsLavenderBed", 347, 1, 23.0f); S1PLANTS_PAIR("RoseStandardsLavenderBed", 351, -1, 23.0f);
	S1PLANTS_PAIR("RoseStandardsLavenderBed", 355, 1, 23.0f);
	S1PLANTS_AT("TopiaryMoon", 267, -1, 25.3f, 0.04f, 0.f); S1PLANTS_AT("Cypress", 267, -1, 57.0f, 0.04f, 0.f); S1PLANTS_AT("TopiaryMoon", 267, 1, 58.8f, 0.04f, 0.f);
	S1PLANTS_AT("TopiaryMoon", 271, -1, 25.3f, 0.04f, 0.f); S1PLANTS_AT("Cypress", 271, -1, 57.0f, 0.04f, 0.f); S1PLANTS_AT("Cypress", 271, 1, 55.5f, 0.04f, 0.f);
	S1PLANTS_AT("WisteriaArch", 271, 1, 58.8f, 0.04f, 0.f); S1PLANTS_AT("Cypress", 275, -1, 25.5f, 0.04f, 0.f); S1PLANTS_AT("WisteriaArch", 275, 1, 47.8f, 0.04f, 0.f);
	S1PLANTS_AT("WisteriaArch", 279, -1, 25.3f, 0.04f, 0.f); S1PLANTS_AT("TopiaryMoon", 279, -1, 55.8f, 0.04f, 0.f); S1PLANTS_AT("Cypress", 279, -1, 59.0f, 0.04f, 0.f);
	S1PLANTS_AT("TopiaryMoon", 279, 1, 58.8f, 0.04f, 0.f); S1PLANTS_AT("TopiaryMoon", 283, -1, 25.3f, 0.04f, 0.f); S1PLANTS_AT("TopiaryMoon", 287, -1, 54.8f, 0.04f, 0.f);
	S1PLANTS_AT("Cypress", 287, -1, 58.0f, 0.04f, 0.f); S1PLANTS_AT("Cypress", 287, 1, 48.5f, 0.04f, 0.f); S1PLANTS_AT("WisteriaArch", 287, 1, 54.8f, 0.04f, 0.f);
	S1PLANTS_AT("TopiaryMoon", 287, 1, 57.8f, 0.04f, 0.f); S1PLANTS_AT("Cypress", 291, -1, 25.5f, 0.04f, 0.f); S1PLANTS_AT("TopiaryMoon", 299, -1, 25.3f, 0.04f, 0.f);
	S1PLANTS_AT("Cypress", 0, -1, 55.0f, 0.04f, 0.f); S1PLANTS_AT("TopiaryMoon", 299, 1, 47.8f, 0.04f, 0.f); S1PLANTS_AT("Cypress", 299, 1, 55.5f, 0.04f, 0.f);
	S1PLANTS_AT("WisteriaArch", 299, 1, 58.8f, 0.04f, 0.f); S1PLANTS_AT("TopiaryMoon", 303, -1, 25.3f, 0.04f, 0.f); S1PLANTS_AT("Cypress", 307, -1, 25.5f, 0.04f, 0.f);
	S1PLANTS_AT("WisteriaArch", 307, 1, 54.8f, 0.04f, 0.f); S1PLANTS_AT("TopiaryMoon", 307, 1, 57.8f, 0.04f, 0.f); S1PLANTS_AT("WisteriaArch", 0, -1, 54.8f, 0.04f, 0.f);
	S1PLANTS_AT("TopiaryMoon", 311, -1, 57.8f, 0.04f, 0.f); S1PLANTS_AT("Willow", 311, 1, 51.8f, 0.10f, 180.0f); S1PLANTS_AT("TopiaryMoon", 311, 1, 58.8f, 0.04f, 0.f);
	S1PLANTS_AT("TopiaryMoon", 315, -1, 25.3f, 0.04f, 0.f); S1PLANTS_AT("Cypress", 319, 1, 52.5f, 0.04f, 0.f); S1PLANTS_AT("Cypress", 323, -1, 25.5f, 0.04f, 0.f);
	S1PLANTS_AT("WisteriaArch", 0, 1, 47.8f, 0.04f, 0.f); S1PLANTS_AT("WisteriaArch", 327, -1, 55.8f, 0.04f, 0.f); S1PLANTS_AT("TopiaryMoon", 327, -1, 58.8f, 0.04f, 0.f);
	S1PLANTS_AT("TopiaryMoon", 327, 1, 58.8f, 0.04f, 0.f); S1PLANTS_AT("TopiaryMoon", 0, -1, 25.3f, 0.04f, 0.f); S1PLANTS_AT("TopiaryMoon", 331, 1, 51.8f, 0.04f, 0.f);
	S1PLANTS_AT("Cypress", 331, 1, 55.0f, 0.04f, 0.f); S1PLANTS_AT("WisteriaArch", 331, 1, 58.3f, 0.04f, 0.f); S1PLANTS_AT("Cypress", 335, 1, 48.5f, 0.04f, 0.f);
	S1PLANTS_AT("Cypress", 339, -1, 25.5f, 0.04f, 0.f); S1PLANTS_AT("WisteriaArch", 339, -1, 56.8f, 0.04f, 0.f); S1PLANTS_AT("WisteriaArch", 339, 1, 58.8f, 0.04f, 0.f);
	S1PLANTS_AT("WisteriaArch", 343, -1, 25.3f, 0.04f, 0.f); S1PLANTS_AT("TopiaryMoon", 347, -1, 25.3f, 0.04f, 0.f); S1PLANTS_AT("TopiaryMoon", 347, 1, 47.8f, 0.04f, 0.f);
	S1PLANTS_AT("Cypress", 347, 1, 51.0f, 0.04f, 0.f); S1PLANTS_AT("TopiaryMoon", 351, -1, 55.8f, 0.04f, 0.f); S1PLANTS_AT("Cypress", 351, -1, 59.0f, 0.04f, 0.f);
	S1PLANTS_AT("WisteriaArch", 351, 1, 58.8f, 0.04f, 0.f); S1PLANTS_AT("Cypress", 355, -1, 25.5f, 0.04f, 0.f);
};
WldS1PLANTSDistrict6();
const auto WldS1PLANTSDistrict7 = [&]()
{
Zone(TEXT("World s1plants, Snowglobe Square"), 16.1f, 20.5f);    // 78 pieces tried: FrostBirch 2, HollyLow 60, SnowFirGroup 10, SnowFirTall 6
	S1PLANTS_PAIR("HollyLowHollyLow", 359, -1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 359, 1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 363, 1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 367, 1, 23.0f);
	S1PLANTS_PAIR("HollyLowHollyLow", 371, -1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 371, 1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 375, -1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 379, 1, 23.0f);
	S1PLANTS_PAIR("HollyLowHollyLow", 383, -1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 383, 1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 387, -1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 391, 1, 23.0f);
	S1PLANTS_PAIR("HollyLowHollyLow", 395, -1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 395, 1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 399, -1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 403, 1, 23.0f);
	S1PLANTS_PAIR("HollyLowHollyLow", 407, -1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 407, 1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 411, -1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 415, -1, 23.0f);
	S1PLANTS_PAIR("HollyLowHollyLow", 419, 1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 423, -1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 427, 1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 431, -1, 23.0f);
	S1PLANTS_PAIR("HollyLowHollyLow", 431, 1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 435, -1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 439, 1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 443, -1, 23.0f);
	S1PLANTS_PAIR("HollyLowHollyLow", 447, 1, 23.0f); S1PLANTS_PAIR("HollyLowHollyLow", 451, 1, 23.0f);
	S1PLANTS_AT("SnowFirGroup", 359, -1, 56.0f, 0.04f, 0.f); S1PLANTS_AT("FrostBirch", 359, 1, 50.7f, 0.10f, 180.0f); S1PLANTS_AT("SnowFirGroup", 359, 1, 58.0f, 0.04f, 0.f);
	S1PLANTS_AT("SnowFirTall", 367, 1, 54.3f, 0.10f, 180.0f); S1PLANTS_AT("FrostBirch", 371, 1, 50.7f, 0.10f, 180.0f); S1PLANTS_AT("SnowFirGroup", 371, 1, 58.0f, 0.04f, 0.f);
	S1PLANTS_AT("SnowFirTall", 379, 1, 54.3f, 0.10f, 180.0f); S1PLANTS_AT("SnowFirGroup", 387, -1, 58.0f, 0.04f, 0.f); S1PLANTS_AT("SnowFirTall", 391, 1, 54.3f, 0.10f, 180.0f);
	S1PLANTS_AT("SnowFirGroup", 399, -1, 57.0f, 0.04f, 0.f); S1PLANTS_AT("SnowFirGroup", 411, -1, 58.0f, 0.04f, 0.f); S1PLANTS_AT("SnowFirGroup", 0, -1, 56.0f, 0.04f, 0.f);
	S1PLANTS_AT("SnowFirGroup", 419, 1, 57.0f, 0.04f, 0.f); S1PLANTS_AT("SnowFirTall", 427, 1, 54.3f, 0.10f, 180.0f); S1PLANTS_AT("SnowFirGroup", 431, 1, 49.5f, 0.04f, 0.f);
	S1PLANTS_AT("SnowFirTall", 439, 1, 54.3f, 0.10f, 180.0f); S1PLANTS_AT("SnowFirGroup", 447, -1, 57.0f, 0.04f, 0.f); S1PLANTS_AT("SnowFirTall", 451, 1, 54.3f, 0.10f, 180.0f);
};
WldS1PLANTSDistrict7();
const auto WldS1PLANTSDistrict8 = [&]()
{
Zone(TEXT("World s1plants, Giants Garden"), 20.5f, 99.0f);    // 107 pieces tried: BeanPoles 3, Bluebells 18, CloverPatch 60, DandelionClocks 9, GiantFern 3, Hibiscus 3, Sunflower 11
	S1PLANTS_PAIR("CloverPatchCloverPatch", 455, -1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 455, 1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 459, -1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 463, 1, 23.0f);
	S1PLANTS_PAIR("CloverPatchCloverPatch", 467, -1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 467, 1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 471, -1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 475, 1, 23.0f);
	S1PLANTS_PAIR("CloverPatchCloverPatch", 479, -1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 479, 1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 483, -1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 487, 1, 23.0f);
	S1PLANTS_PAIR("CloverPatchCloverPatch", 491, -1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 491, 1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 495, -1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 499, -1, 23.0f);
	S1PLANTS_PAIR("CloverPatchCloverPatch", 503, 1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 507, -1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 511, 1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 515, -1, 23.0f);
	S1PLANTS_PAIR("CloverPatchCloverPatch", 515, 1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 519, -1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 523, 1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 527, -1, 23.0f);
	S1PLANTS_PAIR("CloverPatchCloverPatch", 531, 1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 535, 1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 539, -1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 539, 1, 23.0f);
	S1PLANTS_PAIR("CloverPatchCloverPatch", 543, -1, 23.0f); S1PLANTS_PAIR("CloverPatchCloverPatch", 547, 1, 23.0f);
	S1PLANTS_AT("Sunflower", 455, 1, 49.2f, 0.04f, 0.f); S1PLANTS_AT("Bluebells", 459, -1, 25.7f, 0.04f, 0.f); S1PLANTS_AT("Bluebells", 463, -1, 25.7f, 0.04f, 0.f);
	S1PLANTS_AT("Bluebells", 467, -1, 25.7f, 0.04f, 0.f); S1PLANTS_AT("DandelionClocks", 0, -1, 56.2f, 0.10f, 180.0f); S1PLANTS_AT("GiantFern", 467, 1, 50.9f, 0.10f, 180.0f);
	S1PLANTS_AT("DandelionClocks", 467, 1, 58.2f, 0.10f, 180.0f); S1PLANTS_AT("DandelionClocks", 471, -1, 57.2f, 0.10f, 180.0f); S1PLANTS_AT("Bluebells", 471, -1, 25.7f, 0.04f, 0.f);
	S1PLANTS_AT("Bluebells", 471, 1, 59.2f, 0.04f, 0.f); S1PLANTS_AT("Sunflower", 0, -1, 55.7f, 0.04f, 0.f); S1PLANTS_AT("Sunflower", 479, 1, 49.2f, 0.04f, 0.f);
	S1PLANTS_AT("BeanPoles", 479, 1, 55.4f, 0.10f, 180.0f); S1PLANTS_AT("Bluebells", 483, -1, 25.7f, 0.04f, 0.f); S1PLANTS_AT("Bluebells", 487, 1, 52.2f, 0.04f, 0.f);
	S1PLANTS_AT("Bluebells", 491, -1, 25.7f, 0.04f, 0.f); S1PLANTS_AT("DandelionClocks", 0, -1, 56.2f, 0.10f, 180.0f); S1PLANTS_AT("GiantFern", 491, 1, 50.9f, 0.10f, 180.0f);
	S1PLANTS_AT("DandelionClocks", 491, 1, 58.2f, 0.10f, 180.0f); S1PLANTS_AT("Sunflower", 495, -1, 26.2f, 0.04f, 0.f); S1PLANTS_AT("Bluebells", 499, -1, 25.7f, 0.04f, 0.f);
	S1PLANTS_AT("Hibiscus", 499, 1, 53.3f, 0.04f, 0.f); S1PLANTS_AT("Sunflower", 499, 1, 58.7f, 0.04f, 0.f); S1PLANTS_AT("Sunflower", 507, -1, 57.7f, 0.04f, 0.f);
	S1PLANTS_AT("Bluebells", 507, -1, 25.7f, 0.04f, 0.f); S1PLANTS_AT("Bluebells", 507, 1, 59.2f, 0.04f, 0.f); S1PLANTS_AT("Bluebells", 515, -1, 25.7f, 0.04f, 0.f);
	S1PLANTS_AT("DandelionClocks", 515, 1, 49.2f, 0.10f, 180.0f); S1PLANTS_AT("DandelionClocks", 519, -1, 57.2f, 0.10f, 180.0f); S1PLANTS_AT("Bluebells", 519, 1, 59.2f, 0.04f, 0.f);
	S1PLANTS_AT("Bluebells", 523, -1, 25.7f, 0.04f, 0.f); S1PLANTS_AT("Hibiscus", 527, -1, 56.3f, 0.04f, 0.f); S1PLANTS_AT("Bluebells", 0, -1, 25.7f, 0.04f, 0.f);
	S1PLANTS_AT("Sunflower", 527, 1, 49.2f, 0.04f, 0.f); S1PLANTS_AT("BeanPoles", 527, 1, 55.4f, 0.10f, 180.0f); S1PLANTS_AT("Sunflower", 531, -1, 57.7f, 0.04f, 0.f);
	S1PLANTS_AT("Bluebells", 535, -1, 25.7f, 0.04f, 0.f); S1PLANTS_AT("Bluebells", 539, -1, 25.7f, 0.04f, 0.f); S1PLANTS_AT("DandelionClocks", 539, -1, 56.2f, 0.10f, 180.0f);
	S1PLANTS_AT("GiantFern", 539, 1, 50.9f, 0.10f, 180.0f); S1PLANTS_AT("DandelionClocks", 539, 1, 58.2f, 0.10f, 180.0f); S1PLANTS_AT("Bluebells", 547, -1, 25.7f, 0.04f, 0.f);
	S1PLANTS_AT("Hibiscus", 547, 1, 53.3f, 0.04f, 0.f); S1PLANTS_AT("Sunflower", 547, 1, 58.7f, 0.04f, 0.f); S1PLANTS_AT("Sunflower", 0, -1, 55.7f, 0.04f, 0.f);
	S1PLANTS_AT("Sunflower", 551, 1, 49.2f, 0.04f, 0.f); S1PLANTS_AT("BeanPoles", 551, 1, 55.4f, 0.10f, 180.0f);
};
WldS1PLANTSDistrict8();
#undef S1PLANTS_PAIR
#undef S1PLANTS_AT
#undef S1PLANTS
// ---- END s1plants ----
}

{
// ---- BEGIN s1animals (the animals of the Theme Park, ft_world_park_s1animals.py) ----
// 19 meshes. WHERE: a 1.6 m barrier and a verge of hoardings hide the bare ground, so every scene brings what it
// stands on (a wall, piles, a rock, a plinth, a knoll, an ice shelf, a plank on pots, a rimmed pool) and every colour
// carries a steady glow (0.09; the birds of the sky 0.3): nothing of this block is nearer than 40.5 m, nothing blinks.
// Bands and slots used (the brief's table for s1animals):
//   40.5-45 m   s % 8 == 5 (every 8, phase 3)       walls, piles, pens: 40.5 and not 20, so nothing floats over a
//                                                   banked bend's cutting and every piece may be TALL and lit
//   47-56 m     s % 4 == 1 (every 4 phase 3; every 8 phase 3 or 7)   the scenes on plinths and knolls
//   76-104 m    s % 4 == 1 and 3 (every 4, phase 3 and 1)            the three rimmed pools (the lagoon at 2.5 times
//                                                   their size, seen from the rails 26 m up; Crescent Keep at 1.3)
//   sky         Far, 62 m or more out, yaw dice 0, scale 1: each flock flies a straight line 56 m long ALONG the
//               road and comes round again (no circle; the widest flock is 4.4 m either side of its line, so the
//               nearest wing tip is 58 m out). Drop is negative = above the land's level there: 11 to 22 m over the
//               park, 34 and 40 m over the lagoon's valley floor (8 and 14 m over the rails).
// No mesh stands both inside and outside 70 m. The lagoon's pools have a 2.6 m foot (6.5 m at their size there).
// Estimate (Scatter: slots x chance x sides x 0.75; Far x 1; slots counted by C:\ftwork\jobs\L4s1animals\est.py):
//   d1 9.0 + 9.8 + 9.8 = 28.6;  d2 4.5 + 3.8 + 3.8 + 10 = 22.1;  d3 18.0 + 11.0 + 11.0 + 20 + 14 = 74.0;
//   d4 16.5 + 4 + 3.0 + 4.5 = 28.0;  d5 10.5 + 5.2 + 10 = 25.7;  d6 18.0 + 8.2 + 10.3 + 16 + 8 = 60.5;
//   d7 18.0 + 9.8 + 9.8 + 8 = 45.6;  d8 9.0 + 19.5 + 9.0 + 16 = 53.5:  about 338 placed.
// Most of one mesh: Rabbits 36 (48 tried); Egrets and SwanFlight 30 each. Smallest x min of a TALL mesh: 40.5 m
// (PeacockWall, GeeseLine, PelicanPosts, BackLotRaccoons, Reindeer, GardenFolk; front edges at 1.3 size: Reindeer
// 36.7 m, GardenFolk 38.7 m, the others 39.2 m or more). Every mesh but SwanFlight is TALL in the make log.
#define S1ANIMALS(n) TEXT("/Game/World/ThemePark/SM_S1ANIMALS_" n)
Zone(TEXT("World s1animals, Front Gate and Midway"), 0.f, 2.22f);
	Scatter(S1ANIMALS("PeacockWall"), 40.5f, 45.f, 8, 3, 1.f, 1.3f, 0.f, 0, 6.f);          // white peacocks on a garden wall, both sides
	Scatter(S1ANIMALS("GeeseLine"), 47.f, 55.f, 4, 3, 1.f, 1.3f, 0.f, -1, 0.f);            // geese in file and their goose-herd, left
	Scatter(S1ANIMALS("Butterflies"), 47.f, 55.f, 4, 3, 1.f, 1.3f, 0.1f, 1, 20.f);         // butterflies over hollyhocks, right
Zone(TEXT("World s1animals, Pirate Cove"), 2.22f, 3.21f);
	Scatter(S1ANIMALS("PelicanPosts"), 40.5f, 45.f, 8, 3, 1.f, 1.3f, 0.1f, 0, 8.f);
	Scatter(S1ANIMALS("PelicanPosts"), 47.f, 55.f, 4, 3, 1.f, 1.3f, 0.1f, -1, 8.f);        // left: the loop comes down on the right
	Scatter(S1ANIMALS("CrabRock"), 47.f, 55.f, 4, 3, 1.f, 1.3f, 0.1f, 1, 15.f);
	Far(S1ANIMALS("Egrets"), 70.f, 110.f, 4, 0, 1.f, 1.f, 0.f, 0, -16.f, 0.f);             // 16 m over the land
Zone(TEXT("World s1animals, The Lagoon"), 3.21f, 6.9074f);
	Scatter(S1ANIMALS("Swans"), 76.f, 104.f, 4, 3, 0.6f, 2.5f, 0.1f, 0, 180.f);            // on the valley floor, lit, seen from above
	Scatter(S1ANIMALS("KoiLeap"), 76.f, 104.f, 4, 1, 0.7f, 2.5f, 0.1f, -1, 180.f);
	Scatter(S1ANIMALS("HeronsOtters"), 76.f, 104.f, 4, 1, 0.7f, 2.5f, 0.1f, 1, 10.f);
	Far(S1ANIMALS("Egrets"), 66.f, 110.f, 8, 2, 1.f, 1.f, 0.f, 0, -34.f, 0.f);             // 8 m over the rails
	Far(S1ANIMALS("SwanFlight"), 75.f, 115.f, 12, 5, 1.f, 1.f, 0.f, 0, -40.f, 0.f);        // 14 m over the rails
Zone(TEXT("World s1animals, Rocket Land and Funhouse"), 6.9074f, 8.9074f);
	Scatter(S1ANIMALS("Rabbits"), 47.f, 55.f, 4, 3, 1.f, 1.3f, 0.1f, 0, 25.f);
	Far(S1ANIMALS("Owls"), 84.f, 118.f, 24, 7, 1.f, 1.f, 0.f, 0, -13.f, 0.f);
Zone(TEXT("World s1animals, the back lot"), 8.9074f, 9.2974f);
	Scatter(S1ANIMALS("BackLotRaccoons"), 40.5f, 45.f, 8, 3, 1.f, 1.3f, 0.f, 0, 6.f);
	Scatter(S1ANIMALS("BackLotRaccoons"), 47.f, 55.f, 4, 3, 1.f, 1.3f, 0.f, 0, 6.f);
Zone(TEXT("World s1animals, Sweet Street"), 9.2974f, 11.94f);
	Scatter(S1ANIMALS("RibbonPonies"), 48.f, 56.f, 8, 7, 1.f, 1.3f, 0.f, 0, 0.f);          // s % 8 == 1
	Scatter(S1ANIMALS("GeeseLine"), 40.5f, 45.f, 24, 19, 1.f, 1.3f, 0.f, 1, 0.f);          // fix round: RIGHT, s % 24 == 5 (was left, every 8: 5 of 7 stood in pass 3's pieces; on the right the saucer park takes s % 24 == 21)
	Far(S1ANIMALS("Songbirds"), 62.f, 95.f, 12, 4, 1.f, 1.f, 0.f, 0, -11.f, 0.f);
Zone(TEXT("World s1animals, Crescent Keep"), 11.94f, 16.10f);
	Scatter(S1ANIMALS("WhiteDeer"), 47.f, 55.f, 8, 7, 1.f, 1.2f, 0.f, 1, 10.f);            // s % 8 == 1; fix round: right only (was both: 11 of 12 on the left stood in pass 3's pieces)
	Scatter(S1ANIMALS("PeacockWall"), 40.5f, 45.f, 24, 19, 1.f, 1.3f, 0.f, 1, 6.f);        // fix round: RIGHT, s % 24 == 5 (was left, every 8: 7 of 11 stood in pass 3's pieces)
	Scatter(S1ANIMALS("Swans"), 76.f, 100.f, 4, 1, 0.6f, 1.3f, 0.1f, -1, 180.f);           // the moat's swans
	Far(S1ANIMALS("SwanFlight"), 75.f, 115.f, 12, 5, 1.f, 1.f, 0.f, 0, -22.f, 0.f);
	Far(S1ANIMALS("Owls"), 84.f, 118.f, 24, 7, 1.f, 1.f, 0.f, 0, -13.f, 0.f);
Zone(TEXT("World s1animals, Snowglobe Square"), 16.10f, 20.50f);
	Scatter(S1ANIMALS("Penguins"), 47.f, 55.f, 8, 7, 1.f, 1.3f, 0.f, 0, 0.f);              // s % 8 == 1
	Scatter(S1ANIMALS("Reindeer"), 40.5f, 45.f, 8, 3, -1.f, 1.3f, 0.f, -1, 0.f);           // fix round: OFF (chance -1 places nothing; the rule stays so later rules keep their numbers): all 13 stood in pass 3's pieces
	Scatter(S1ANIMALS("Reindeer"), 47.f, 55.f, 8, 3, 1.f, 1.3f, 0.f, 1, 0.f);
	Far(S1ANIMALS("Owls"), 84.f, 118.f, 24, 7, 1.f, 1.f, 0.f, 0, -13.f, 0.f);
Zone(TEXT("World s1animals, Giant's Garden"), 20.50f, 99.f);
	Scatter(S1ANIMALS("GardenFolk"), 40.5f, 45.f, 8, 3, -1.f, 1.3f, 0.f, -1, 0.f);         // fix round: OFF here (all 12 stood in pass 3's pieces); the garden folk stand by the rule in the s1fix block
	Scatter(S1ANIMALS("Rabbits"), 47.f, 55.f, 8, 7, 1.f, 1.3f, 0.1f, 1, 25.f);             // s % 8 == 1; fix round: right only (was both: 9 of 13 on the left stood in pass 3's pieces)
	Scatter(S1ANIMALS("Butterflies"), 47.f, 55.f, 24, 3, 1.f, 1.3f, 0.1f, 1, 20.f);        // fix round: s % 24 == 21 only (was every 8): the other slots are the garden folk's now (s1fix block)
	Far(S1ANIMALS("Songbirds"), 62.f, 95.f, 12, 4, 1.f, 1.f, 0.f, 0, -11.f, 0.f);
#undef S1ANIMALS
// ---- END s1animals ----
}

{
// ---- BEGIN s1people (the people pass 3 left out, ft_world_park_s1people.py) ----
// 31 meshes, all made by the one make run (C:/ftwork/logs/L4s1people_make.log). All TALL; each brings its own raised floor (2 m and more as
// built) so its people clear the barrier and the hoarding. A steady glow of 0.08 and steady lamps (every rule stands 26.2 m out or more);
// nothing blinks. No Lit, no Bank. Slots: s % 8 == 1 only (every 8 phase 7; every 24 phase 23 / 15 / 7; every 72 phase 39). NOT COMPILED.
// What decided it (read from both tables, nothing seen): on the slots s % 8 == 1 pass 3 and the first passes already stand at 38 to 46 m on
// the LEFT of the whole Last Ride (a p3b stage in every one of them) and in Rocket Land, the funhouse, the front gate and the midway, so
// nothing is added there. Free were:
//   THE SHOW ROW, the Last Ride's RIGHT side, in the parade lane (24.9 to 27.5 m; the right's floats use only s % 4 == 2): on s % 24 == 1 and
//     17 the dressing has a stall row there (anchor 31 m) with SM_Crowd 26.7 m out, 11.6 m long (StallRow2: -1.8 to +9.8 m along; StallRow:
//     -8.8 to +2.8). Seven pieces a slot stand either side of that crowd, 26.2 m out at 1.3 (2.8 x 1.75 m as built: 3.6 m long as placed, 4 m
//     apart, 25.0 to 27.4 m out). On s % 72 == 33 only p3f's press pen stands in the lane (27.5 m, +-4.4 m along): eight pieces, four a side.
//     s % 24 == 9 otherwise is left alone (press pen and p3c's near ways). Each quarter has six pieces of its own.
//   THE BIG JUMP: slot 41 left (run-up, 42.3 m: the piece's front is 40.3 m out), slot 49 right by a Far rule (the flight; drop 1.05 like the
//     rules there). The other sides have p3f's Far pieces at 45.5 to 46.5 m.
//   PIRATE COVE: slot 57 left (41.5 m; the bend at 2.58 km; the dock stands on the right), slot 65 left by a Far rule (the loop's zone).
//   THE LAGOON: jetties on piles, deck 27 m up as placed, by Far rules 42.6 m out (40.1 to 44.9 m): right on s % 24 == 9 and 17, left on
//     s % 24 == 9. Not on s % 24 == 1 (the dressing's bandstands, 44 to 58 m, stand on s % 3 == 1).
//   THE BACK LOT: slot 201 right (41.5 m), the outside of the bend at 9.09 km.
// The protocol's slots give NO slot on the lift hill (66 to 70) and none on splashdown (149 to 152): nothing is placed there.
// Estimate (Site: places x 0.75; Far: x 1), slots counted one by one:
//   show row: Sweet Street A 2 + B 3 slots x 7 + F 1 x 8 = 43; Crescent Keep 4 x 7 + 4 x 7 + 8 = 64; Snowglobe Square 64;
//             Giant's Garden 5 x 7 + 4 x 7 + 2 x 8 = 79. Sum 250 x 0.75 = 187.5
//   gaps by Site: jump 5 + cove 5 + back lot 2 = 12 x 0.75 = 9;   by Far: jump 1 + cove 1 + lagoon 6 right + 3 left = 11.
//   About 208 placed. The most of one mesh: SM_S1PEOPLE_Gd_Ladder, 16 places (about 12). The smallest x min of a TALL mesh: 26.2 m.
#define S1PEOPLE(n) TEXT("/Game/World/ThemePark/SM_S1PEOPLE_" n)
Set(TEXT("WldS1PEOPLESweetA"), {      // Sweet Street: beside a StallRow2 crowd (s % 24 == 1)
	{ S1PEOPLE("Sw_Ices"), -15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Toffee"), -11.92f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Wagon"), -8.85f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Window"), -5.77f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Baker"), 9.54f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Bench"), 12.62f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Ices"), 15.69f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLESweetB"), {      // beside a StallRow crowd (s % 24 == 17)
	{ S1PEOPLE("Sw_Baker"), -15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Bench"), -11.88f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Window"), -8.77f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Toffee"), 4.15f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Wagon"), 7.23f, 0.f, 0.f, 0.f, 1.f },
});      // fix round: the two pieces at +10.31 and +13.38 stood in the stall row's candy cart (26.4 to 31.6 m out)
Set(TEXT("WldS1PEOPLESweetF"), {      // either side of p3f's press pen (s % 72 == 33)
	{ S1PEOPLE("Sw_Window"), -15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Wagon"), -11.92f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Toffee"), -8.85f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Bench"), -5.77f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Ices"), 5.77f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Baker"), 8.85f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Wagon"), 11.92f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sw_Toffee"), 15.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEKeepA"), {      // Crescent Keep: beside a StallRow2 crowd (s % 24 == 1)
	{ S1PEOPLE("Kp_Heralds"), -15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Kp_Juggler"), -11.92f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Kp_Dance"), -8.85f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Kp_Painter"), -5.77f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Kp_Bridge"), 9.54f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Kp_Stargazer"), 12.62f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Kp_Heralds"), 15.69f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEKeepB"), {      // beside a StallRow crowd (s % 24 == 17)
	{ S1PEOPLE("Kp_Bridge"), -15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Kp_Stargazer"), -11.88f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Kp_Painter"), -8.77f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Kp_Juggler"), 4.15f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Kp_Dance"), 7.23f, 0.f, 0.f, 0.f, 1.f },
});      // fix round: the two pieces at +10.31 and +13.38 stood in the stall row's candy cart (26.4 to 31.6 m out)
Set(TEXT("WldS1PEOPLEKeepF"), {      // either side of p3f's press pen (s % 72 == 33)
	{ S1PEOPLE("Kp_Painter"), -15.f, 0.f, 0.f, 0.f, 1.f },      // fix round: the three at -11.92, -8.85 and -5.77 stood in the press pen of slot 321 (it slides by dice: -15.2 to -6.4 m along there)
	{ S1PEOPLE("Kp_Heralds"), 5.77f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Kp_Bridge"), 8.85f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Kp_Dance"), 11.92f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Kp_Juggler"), 15.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLESnowA"), {      // Snowglobe Square: beside a StallRow2 crowd (s % 24 == 1)
	{ S1PEOPLE("Sn_Rink"), -15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Sledge"), -11.92f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Snowman"), -8.85f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Mugs"), -5.77f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Bell"), 9.54f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Skis"), 12.62f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Rink"), 15.69f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLESnowB"), {      // beside a StallRow crowd (s % 24 == 17)
	{ S1PEOPLE("Sn_Bell"), -15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Skis"), -11.88f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Mugs"), -8.77f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Sledge"), 4.15f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Snowman"), 7.23f, 0.f, 0.f, 0.f, 1.f },
});      // fix round: the two pieces at +10.31 and +13.38 stood in the stall row's candy cart (26.4 to 31.6 m out)
Set(TEXT("WldS1PEOPLESnowF"), {      // either side of p3f's press pen (s % 72 == 33)
	{ S1PEOPLE("Sn_Mugs"), -15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Snowman"), -11.92f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Sledge"), -8.85f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Skis"), -5.77f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Rink"), 5.77f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Bell"), 8.85f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Snowman"), 11.92f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Sn_Sledge"), 15.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEGardenA"), {      // Giant's Garden: beside a StallRow2 crowd (s % 24 == 1)
	{ S1PEOPLE("Gd_Ladder"), -15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Gd_TeaCup"), -11.92f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Gd_Toadstools"), -8.85f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Gd_Hose"), -5.77f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Gd_Beans"), 9.54f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Gd_Thimbles"), 12.62f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Gd_Ladder"), 15.69f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEGardenB"), {      // beside a StallRow crowd (s % 24 == 17)
	{ S1PEOPLE("Gd_Beans"), -15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Gd_Thimbles"), -11.88f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Gd_Hose"), -8.77f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Gd_TeaCup"), 4.15f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Gd_Toadstools"), 7.23f, 0.f, 0.f, 0.f, 1.f },
});      // fix round: the two pieces at +10.31 and +13.38 stood in the stall row's candy cart (26.4 to 31.6 m out)
Set(TEXT("WldS1PEOPLEGardenF"), {      // either side of p3f's press pen (s % 72 == 33)
	{ S1PEOPLE("Gd_Hose"), -15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Gd_Toadstools"), -11.92f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Gd_TeaCup"), -8.85f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Gd_Thimbles"), -5.77f, 0.f, 0.f, 0.f, 1.f },      // fix round: the two at +5.77 and +8.85 stood in the press pen of slot 465 (+5.8 to +14.6 m along there)
	{ S1PEOPLE("Gd_Toadstools"), 11.92f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Gd_TeaCup"), 15.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEJump"), {      // a terrace row on the valley side, 35 m long as placed
	{ S1PEOPLE("JumpTerrace"), -10.8f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("JumpSteps"), -5.4f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("JumpTerrace"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("JumpSteps"), 5.4f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("JumpTerrace"), 10.8f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLECove"), {      // stilt decks
	{ S1PEOPLE("CoveDeck_A"), -11.7f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("CoveDeck_B"), -5.85f, 0.f, 0.f, 0.f, 1.f },      // fix round: the middle deck (along 0) stood 1.5 m into pass 3's crow's nest (42.6 m out as the dice put it): a gap is left for it
	{ S1PEOPLE("CoveDeck_B"), 5.85f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("CoveDeck_A"), 11.7f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEBackLot"), {      // two loading docks
	{ S1PEOPLE("BackLotDock"), -7.6f, 0.f, 0.f, 0.f, 1.f },      // fix round: were -6 and +6, the second 1.7 m into SM_P3A_Prizes (to +6.0 m along)
	{ S1PEOPLE("BackLotDock"), 7.6f, 0.f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1people, the big jump's run-up"), 1.84f, 1.90f);                  // slot 41
	Site(TEXT("WldS1PEOPLEJump"), -1, 42.3f, 8, 7, 1.f, 1.3f);
Zone(TEXT("World s1people, the big jump's landing"), 2.19f, 2.22f);                 // slot 49 (the flight: a Far rule stays)
	Far(S1PEOPLE("JumpTerrace"), 44.f, 44.f, 8, 7, 1.f, 1.3f, 0.f, 1, 1.05f, 0.f);    // fix round: was 42.3 (front 40.3 m): a Far rule skips the track's own 40 m test at a landing, so 2 m more
Zone(TEXT("World s1people, Pirate Cove's bend"), 2.56f, 2.60f);                     // slot 57
	Site(TEXT("WldS1PEOPLECove"), -1, 42.2f, 8, 7, 1.f, 1.3f);    // fix round: was 41.5, 0.6 m into the watchers' terrace (its back 40.1 m)
Zone(TEXT("World s1people, the cove's loop"), 2.93f, 2.945f);                       // slot 65
	Far(S1PEOPLE("CoveDeck_A"), 42.f, 42.f, 8, 7, 1.f, 1.3f, 0.f, -1, 1.05f, 0.f);
Zone(TEXT("World s1people, the lagoon"), 3.21f, 6.7174f);                           // slots 81, 89, 105, 113, 129, 137
	Far(S1PEOPLE("LagoonJetty_A"), 42.6f, 42.6f, 24, 15, 1.f, 1.15f, 0.f, 1, 1.3f, 0.f);    // right, s % 24 == 9
	Far(S1PEOPLE("LagoonJetty_B"), 42.6f, 42.6f, 24, 7, 1.f, 1.15f, 0.f, 1, 1.3f, 0.f);     // right, s % 24 == 17
	Far(S1PEOPLE("LagoonJetty_B"), 42.6f, 42.6f, 24, 15, 1.f, 1.15f, 0.f, -1, 1.3f, 0.f);   // left, s % 24 == 9
Zone(TEXT("World s1people, the back lot's bend"), 9.08f, 9.11f);                    // slot 201
	Site(TEXT("WldS1PEOPLEBackLot"), 1, 41.5f, 8, 7, 1.f, 1.3f);
Zone(TEXT("World s1people, Sweet Street"), 9.2974f, 11.94f);
	Site(TEXT("WldS1PEOPLESweetA"), 1, 26.2f, 24, 23, 1.f, 1.3f);    // s % 24 == 1
	Site(TEXT("WldS1PEOPLESweetB"), 1, 26.2f, 24, 7, 1.f, 1.3f);     // s % 24 == 17
	Site(TEXT("WldS1PEOPLESweetF"), 1, 26.2f, 72, 39, 1.f, 1.3f);    // s % 72 == 33
Zone(TEXT("World s1people, Crescent Keep"), 11.94f, 16.1f);
	Site(TEXT("WldS1PEOPLEKeepA"), 1, 26.2f, 24, 23, 1.f, 1.3f);    // s % 24 == 1
	Site(TEXT("WldS1PEOPLEKeepB"), 1, 26.2f, 24, 7, 1.f, 1.3f);     // s % 24 == 17
	Site(TEXT("WldS1PEOPLEKeepF"), 1, 26.2f, 72, 39, 1.f, 1.3f);    // s % 72 == 33
Zone(TEXT("World s1people, Snowglobe Square"), 16.1f, 20.5f);
	Site(TEXT("WldS1PEOPLESnowA"), 1, 26.2f, 24, 23, 1.f, 1.3f);    // s % 24 == 1
	Site(TEXT("WldS1PEOPLESnowB"), 1, 26.2f, 24, 7, 1.f, 1.3f);     // s % 24 == 17
	Site(TEXT("WldS1PEOPLESnowF"), 1, 26.2f, 72, 39, 1.f, 1.3f);    // s % 72 == 33
Zone(TEXT("World s1people, Giant's Garden"), 20.5f, 99.f);
	Site(TEXT("WldS1PEOPLEGardenA"), 1, 26.2f, 24, 23, 1.f, 1.3f);    // s % 24 == 1
	Site(TEXT("WldS1PEOPLEGardenB"), 1, 26.2f, 24, 7, 1.f, 1.3f);     // s % 24 == 17
	Site(TEXT("WldS1PEOPLEGardenF"), 1, 26.2f, 72, 39, 1.f, 1.3f);    // s % 72 == 33
#undef S1PEOPLE
// ---- END s1people ----
}

{
// ---- BEGIN s1fix (the review and fix round of Stage 1; SourceArt/Integration/world/L4_park_s1.md) ----
// Rules that had to be written slot by slot after the review, at the END of the table so that no rule above changes its running number
// (and with it its dice). Every rule is one slot (every 1000, phase 1000 - slot), chance 1, a Site: no slide along the road.
// Which slots: worked out with the track's own dice against every older rule by C:/ftwork/jobs/L4s1fix (edit4.py); not seen in the game.
#define S1FIX8(n) TEXT("/Game/World/ThemePark/SM_S1D8_" n)
#define S1FIX_AT(set, s, side, x) Site(TEXT(set), side, x, 1000, 1000 - (s), 1.f)
Set(TEXT("WldS1FixWashR"), { { S1FIX8("WashLine"), -13.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1FixLampRa"), { { S1FIX8("Lamp"), 9.5f, 0.3f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1FixLampRb"), { { S1FIX8("Lamp"), -11.f, 0.3f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1FixBarrowR"), { { S1FIX8("Barrow"), 12.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1FixPotsOnly"), { { S1FIX8("PotTower"), 8.25f, 0.5f, 0.f, -8.f, 1.f } });
const auto WldS1FixGarden = [&]()      // a lambda, so that the population function stays small for the compiler (as the plants block does)
{
Zone(TEXT("World s1fix, Giant's Garden"), 20.5f, 99.f);
	// right, 37 m: the single tall props of ToolsR and KioskR that stand clear of pass 3's stage and the planets on posts in their slot
	S1FIX_AT("WldS1FixBarrowR", 454, 1, 37.f);
	S1FIX_AT("WldS1FixLampRa", 458, 1, 37.f);
	S1FIX_AT("WldS1FixBarrowR", 462, 1, 37.f);
	S1FIX_AT("WldS1FixWashR", 466, 1, 37.f);
	S1FIX_AT("WldS1FixLampRb", 470, 1, 37.f);
	S1FIX_AT("WldS1FixLampRb", 478, 1, 37.f);
	S1FIX_AT("WldS1FixLampRa", 482, 1, 37.f);
	S1FIX_AT("WldS1FixBarrowR", 486, 1, 37.f);
	S1FIX_AT("WldS1FixLampRa", 490, 1, 37.f);
	S1FIX_AT("WldS1FixLampRb", 494, 1, 37.f);
	S1FIX_AT("WldS1FixWashR", 498, 1, 37.f);
	S1FIX_AT("WldS1FixWashR", 506, 1, 37.f);
	S1FIX_AT("WldS1FixBarrowR", 510, 1, 37.f);
	S1FIX_AT("WldS1FixLampRa", 514, 1, 37.f);
	S1FIX_AT("WldS1FixBarrowR", 518, 1, 37.f);
	S1FIX_AT("WldS1FixWashR", 522, 1, 37.f);
	S1FIX_AT("WldS1FixLampRb", 526, 1, 37.f);
	S1FIX_AT("WldS1FixLampRb", 534, 1, 37.f);
	S1FIX_AT("WldS1FixLampRa", 538, 1, 37.f);
	S1FIX_AT("WldS1FixBarrowR", 542, 1, 37.f);
	S1FIX_AT("WldS1FixLampRa", 546, 1, 37.f);
	S1FIX_AT("WldS1FixLampRb", 550, 1, 37.f);
	// left, 63 m: the rows of two buildings, without the far mascot's slots
	S1FIX_AT("WldS1D8ToadSeed", 462, -1, 63.f);
	S1FIX_AT("WldS1D8ToadSeed", 482, -1, 63.f);
	S1FIX_AT("WldS1D8ToadSeed", 502, -1, 63.f);
	S1FIX_AT("WldS1D8ToadSeed", 542, -1, 63.f);
	S1FIX_AT("WldS1D8CanAcorn", 486, -1, 63.f);
	S1FIX_AT("WldS1D8CanAcorn", 506, -1, 63.f);
	S1FIX_AT("WldS1D8CanAcorn", 526, -1, 63.f);
	S1FIX_AT("WldS1D8CanAcorn", 546, -1, 63.f);
	S1FIX_AT("WldS1D8SeedBoot", 454, -1, 63.f);
	S1FIX_AT("WldS1D8SeedBoot", 474, -1, 63.f);
	S1FIX_AT("WldS1D8SeedBoot", 514, -1, 63.f);
	S1FIX_AT("WldS1D8SeedBoot", 534, -1, 63.f);
	S1FIX_AT("WldS1D8BirdCan", 456, -1, 63.f);
	S1FIX_AT("WldS1D8BirdCan", 476, -1, 63.f);
	S1FIX_AT("WldS1D8BirdCan", 496, -1, 63.f);
	S1FIX_AT("WldS1D8BirdCan", 516, -1, 63.f);
};
WldS1FixGarden();
// The near animals of the Last Ride (40.5 to 45 m), moved from the left (where pass 3 stands on those slots) to the right: the block above keeps
// s % 24 == 5; these are the same rules for s % 24 == 13 (s % 24 == 21 is the saucer park's). The garden folk stood in pass 3's pieces on all 12
// of their left-hand slots; they stand on the right here, where the butterflies stood (those keep s % 24 == 21, behind the saucer park).
#define S1FIXA(n) TEXT("/Game/World/ThemePark/SM_S1ANIMALS_" n)
Zone(TEXT("World s1fix, Sweet Street"), 9.2974f, 11.94f);
	Scatter(S1FIXA("GeeseLine"), 40.5f, 45.f, 24, 11, 1.f, 1.3f, 0.f, 1, 0.f);
Zone(TEXT("World s1fix, Crescent Keep"), 11.94f, 16.10f);
	Scatter(S1FIXA("PeacockWall"), 40.5f, 45.f, 24, 11, 1.f, 1.3f, 0.f, 1, 6.f);
Zone(TEXT("World s1fix, Giant's Garden: the garden folk"), 20.50f, 99.f);
	Scatter(S1FIXA("GardenFolk"), 40.5f, 45.f, 24, 19, 1.f, 1.3f, 0.f, 1, 0.f);
	Scatter(S1FIXA("GardenFolk"), 41.6f, 46.f, 24, 11, 1.f, 1.3f, 0.f, 1, 0.f);      // s % 24 == 13: a watchers' platform stands to 40.1 m there
#undef S1FIXA
// The district's own tall prop in the front row (the driver could not tell Sweet Street and Snowglobe Square from the rest at the kerb): one
// candy-cane row (25.2 m out: its front 24.7 m) or one vane post (26.8 m: its bounds' front 24.4 m) a slot and side, even slots only, each at
// the middle of the longest stretch along its slot that is clear by 0.5 m of every piece of both tables with the real dice (edit7.py). Sweet
// Street to 11.23 km only (before the arena's hill); Snowglobe Square only on the level stretches between its banked bends. Not seen in the game.
#define S1FIX_FRONT(mesh, s, side, x, along) Set(TEXT("WldS1FixFront" #s #side), { { TEXT("/Game/World/ThemePark/" mesh), along, 0.f, 0.f, 0.f, 1.f } }); Site(TEXT("WldS1FixFront" #s #side), side, x, 1000, 1000 - (s), 1.f)
const auto WldS1FixFront = [&]()
{
Zone(TEXT("World s1fix, Sweet Street: cane rows in the front row"), 9.2974f, 11.23f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 206, -1, 25.2f, -15.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 208, 1, 25.2f, 0.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 210, -1, 25.2f, 11.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 212, 1, 25.2f, 1.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 214, -1, 25.2f, -9.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 216, 1, 25.2f, 15.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 218, -1, 25.2f, -12.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 220, 1, 25.2f, 0.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 224, 1, 25.2f, 14.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 226, -1, 25.2f, -9.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 228, 1, 25.2f, 0.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 230, -1, 25.2f, -13.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 232, 1, 25.2f, 11.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 236, 1, 25.2f, 8.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 238, -1, 25.2f, -10.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 240, 1, 25.2f, 0.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 242, -1, 25.2f, 13.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 244, 1, 25.2f, -14.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 246, -1, 25.2f, 9.f);
	S1FIX_FRONT("SM_S1D5_CaneRow", 248, 1, 25.2f, 16.f);
Zone(TEXT("World s1fix, Snowglobe Square: vane posts in the front row"), 16.1f, 20.5f);
	S1FIX_FRONT("SM_S1D7_VanePost", 358, -1, 26.8f, 14.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 370, -1, 26.8f, -12.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 372, 1, 26.8f, -11.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 374, -1, 26.8f, 14.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 376, 1, 26.8f, 12.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 380, 1, 26.8f, 3.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 382, -1, 26.8f, -12.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 384, 1, 26.8f, -9.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 394, -1, 26.8f, -12.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 396, 1, 26.8f, -9.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 398, -1, 26.8f, 13.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 400, 1, 26.8f, -10.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 406, -1, 26.8f, -12.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 408, 1, 26.8f, 11.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 410, -1, 26.8f, 13.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 412, 1, 26.8f, -8.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 422, -1, 26.8f, 13.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 424, 1, 26.8f, -8.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 426, -1, 26.8f, -1.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 428, 1, 26.8f, -8.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 430, -1, 26.8f, 14.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 432, 1, 26.8f, -12.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 434, -1, 26.8f, 13.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 436, 1, 26.8f, 13.f);
	S1FIX_FRONT("SM_S1D7_VanePost", 440, 1, 26.8f, -11.f);
};
WldS1FixFront();
#undef S1FIX_FRONT
#undef S1FIX_AT
#undef S1FIX8
// ---- END s1fix ----
}
