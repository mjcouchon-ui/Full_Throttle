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
	FarAt(FARP("Ducks"), -1, 80, 150, Ducks, 3, DuckDrop, 2.f, 15.f);
	FarAt(FARP("Ducks"), 1, 80, 150, Ducks, 3 + Ducks / 2, DuckDrop, 2.f, 15.f);
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
