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
