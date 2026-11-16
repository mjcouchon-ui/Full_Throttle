// The world's sound, level 1: the Chemical Plant (Volta Chemical Works, 23:00). The words are explained at the top of
// FTAmbience.cpp; the sounds are made by Tools/sfx_ambience.py. Districts are the dressing table's (FTDressL1.inl).
Music(-23.1f);   // M_FactoryCombatLoop as the game plays it

Bed(TEXT("Plant"), TEXT("A_L1_Plant"));       // the works' own hum: transformers and big fans a few cycles apart, beating; a far clank
Bed(TEXT("Steam"), TEXT("A_L1_Steam"));       // steam: hisses that come and go, a relief valve, pipes ticking as they warm
Bed(TEXT("Pumps"), TEXT("A_L1_Pumps"));       // machinery with a beat: pumps, a compressor's chug, a conveyor's rattle
Bed(TEXT("Reactor"), TEXT("A_L1_Reactor"));   // the reactor's slow deep throb, coolant rushing, a crackle of current
Bed(TEXT("Yard"), TEXT("A_L1_Yard"));         // the open yards at night: wind round the tanks, chains, a tannoy nobody can make out

District(TEXT("Factory gate"), 0.f, 1.23f);         Mix(TEXT("Yard"), 11.f); Mix(TEXT("Plant"), 12.f);
District(TEXT("Tank farm"), 1.23f, 2.6f);           Mix(TEXT("Plant"), 11.f); Mix(TEXT("Steam"), 13.f);
District(TEXT("Process units"), 2.6f, 3.98f);       Mix(TEXT("Pumps"), 10.f); Mix(TEXT("Plant"), 12.f); Mix(TEXT("Steam"), 14.f);
District(TEXT("Reactor block"), 3.98f, 5.35f);      Mix(TEXT("Reactor"), 10.f); Mix(TEXT("Plant"), 14.f); Mix(TEXT("Steam"), 15.f);
District(TEXT("Pipe alley"), 5.35f, 6.72f);         Mix(TEXT("Steam"), 10.f); Mix(TEXT("Pumps"), 13.f);
District(TEXT("Loading yards"), 6.72f, 8.09f);      Mix(TEXT("Yard"), 10.f); Mix(TEXT("Pumps"), 14.f);
District(TEXT("Waste run"), 8.09f, 9.46f);          Mix(TEXT("Steam"), 12.f); Mix(TEXT("Yard"), 13.f);
District(TEXT("Export terminal"), 9.46f, 13.4f);    Mix(TEXT("Yard"), 12.f); Mix(TEXT("Plant"), 14.f);   // the Alchemist's fight
District(TEXT("Cooling field"), 13.4f, 17.4f);      Mix(TEXT("Steam"), 12.f); Mix(TEXT("Plant"), 14.f);
District(TEXT("Tank farm B"), 17.4f, 21.4f);        Mix(TEXT("Plant"), 12.f); Mix(TEXT("Steam"), 14.f);
District(TEXT("Reactor two"), 21.4f, 99.f);         Mix(TEXT("Reactor"), 11.f); Mix(TEXT("Plant"), 14.f);

// The shift hooter, far across the works (one long low note: not a klaxon, nothing like a warning).
Pass(TEXT("The works' hooter"), TEXT("A_L1_Hooter"), 0.62f, 1, 300.f, 600.f, 8.f, 1.2f, 0.2f);
// A relief valve letting go beside the road, in the tank farm and again in pipe alley.
Pass(TEXT("A vent lets go"), TEXT("A_L1_Vent"), 1.9f, 1, 30.f, 240.f, 5.f, 0.9f, 0.6f);
// The reactor core (SM_ReactorCore, left, 134 m): its throb swells as the car goes by.
Spot(TEXT("The reactor core"), TEXT("A_L1_Core"), 5.205f, -1, 134.f, 420.f, 5.f, 0.3f);
Pass(TEXT("A vent lets go"), TEXT("A_L1_Vent"), 5.95f, -1, 30.f, 240.f, 5.f, 0.9f, 0.6f);
// A train of tank wagons rolling through the loading yards.
Pass(TEXT("Tank wagons"), TEXT("A_L1_Tanker"), 7.3f, -1, 60.f, 380.f, 5.f, 3.0f, 0.7f);
Spot(TEXT("The second core"), TEXT("A_L1_Core"), 21.255f, -1, 150.f, 420.f, 6.f, 0.3f);
