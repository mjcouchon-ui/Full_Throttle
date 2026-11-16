// The world's sound, level 3: the Volcano (Mount Kaldera). The words are explained at the top of FTAmbience.cpp; the
// sounds are made by Tools/sfx_ambience.py. Districts are the dressing table's (FTDressL3.inl).
Music(-23.1f);   // M_InfernoLevel as the game plays it

Bed(TEXT("Rumble"), TEXT("A_L3_Rumble"));   // the mountain: a rumble that surges and sinks, far booms, stones coming down
Bed(TEXT("Lava"), TEXT("A_L3_Lava"));       // the channels: lava's low roar, its crust crackling and spitting, thick bubbles bursting
Bed(TEXT("Ash"), TEXT("A_L3_Ash"));         // falling ash: a dry hush, wind over the drifts, grit pattering
Bed(TEXT("Vents"), TEXT("A_L3_Vents"));     // fumaroles: jets of steam with a note in them, gas hissing, mud gulping
Bed(TEXT("Works"), TEXT("A_L3_Works"));     // the mine: the crusher grinding, its conveyor, pumps on the channels' banks

District(TEXT("The ash road"), 0.f, 1.17f);            Mix(TEXT("Ash"), 11.f); Mix(TEXT("Rumble"), 11.f);
District(TEXT("Lava jumps"), 1.17f, 2.5f);             Mix(TEXT("Lava"), 10.f); Mix(TEXT("Rumble"), 12.f);
District(TEXT("The caldera"), 2.5f, 3.76f);            Mix(TEXT("Rumble"), 10.f); Mix(TEXT("Vents"), 12.f); Mix(TEXT("Lava"), 15.f);
District(TEXT("Fire and brimstone"), 3.76f, 5.18f);    Mix(TEXT("Vents"), 10.f); Mix(TEXT("Lava"), 12.f);
District(TEXT("The magma run"), 5.18f, 6.73f);         Mix(TEXT("Lava"), 9.f); Mix(TEXT("Rumble"), 13.f);
District(TEXT("Obsidian pass"), 6.73f, 8.34f);         Mix(TEXT("Ash"), 11.f); Mix(TEXT("Rumble"), 12.f); Mix(TEXT("Vents"), 15.f);
District(TEXT("Engine 13's road"), 8.34f, 11.1f);      Mix(TEXT("Works"), 12.f); Mix(TEXT("Rumble"), 12.f);   // Engine 13's fight; the mine
District(TEXT("The burning slopes"), 11.1f, 13.9f);    Mix(TEXT("Lava"), 11.f); Mix(TEXT("Vents"), 13.f);
District(TEXT("The ash fields"), 13.9f, 16.7f);        Mix(TEXT("Ash"), 10.f); Mix(TEXT("Rumble"), 13.f);
District(TEXT("The far works"), 16.7f, 19.5f);         Mix(TEXT("Works"), 10.f); Mix(TEXT("Rumble"), 13.f);
District(TEXT("The inner wall"), 19.5f, 22.3f);        Mix(TEXT("Vents"), 11.f); Mix(TEXT("Lava"), 13.f);
District(TEXT("The last ash"), 22.3f, 99.f);           Mix(TEXT("Ash"), 10.f); Mix(TEXT("Rumble"), 12.f);

// The curtain of fire along the fissure on the left (0.34 to 0.56 km, 120 m out).
Spot(TEXT("The curtain of fire"), TEXT("A_L3_FireCurtain"), 0.45f, -1, 120.f, 330.f, 5.f, 0.3f);
// The cone erupts, 485 m to the right at 3.85 km; it is watched from 2.4 km on, so it is heard while it is in view.
Pass(TEXT("The eruption"), TEXT("A_L3_Eruption"), 3.3f, 1, 485.f, 800.f, 4.f, 2.0f, 0.1f);
// A vent beside the road lets go.
Pass(TEXT("A vent lets go"), TEXT("A_L3_Geyser"), 4.5f, -1, 30.f, 240.f, 5.f, 0.8f, 0.6f);
Pass(TEXT("A vent lets go"), TEXT("A_L3_Geyser"), 6.95f, 1, 30.f, 240.f, 5.f, 0.8f, 0.6f);
// The mine's railway down the right-hand side (66 m): a train of slag wagons.
Pass(TEXT("A slag train"), TEXT("A_L3_SlagTrain"), 9.6f, 1, 66.f, 380.f, 5.f, 3.0f, 0.7f);
Pass(TEXT("A slag train"), TEXT("A_L3_SlagTrain"), 17.6f, 1, 66.f, 380.f, 6.f, 3.0f, 0.7f);
