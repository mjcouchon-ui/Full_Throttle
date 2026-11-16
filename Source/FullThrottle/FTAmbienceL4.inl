// The world's sound, level 4: the Theme Park. The words are explained at the top of FTAmbience.cpp; the sounds are made
// by Tools/sfx_ambience.py. Districts are the dressing table's (FTDressL4.inl). The level's music is a carnival tune
// already, so nothing here plays a tune: the park is crowds, machines, water and screams.
Music(-24.0f);   // M_CarnivalChaos as the game plays it

Bed(TEXT("Crowd"), TEXT("A_L4_Crowd"));         // a park full of people: families, children shrieking, a balloon, a stall's bell
Bed(TEXT("Rides"), TEXT("A_L4_Rides"));         // the rides: chains and ratchets, a train's rumble and its screams, air brakes, motors
Bed(TEXT("Lagoon"), TEXT("A_L4_Lagoon"));       // water: lapping under the rails, fountains, a boat's splash, ropes creaking
Bed(TEXT("Rocket"), TEXT("A_L4_Rocket"));       // Rocket Land: the hum of the machines, pneumatics, a countdown nobody can make out
Bed(TEXT("Funhouse"), TEXT("A_L4_Funhouse"));   // boards creaking, a bellows wheezing, barrels rolling, somebody cackling inside

District(TEXT("Front Gate"), 0.f, 1.15f);             Mix(TEXT("Crowd"), 10.f); Mix(TEXT("Rides"), 13.f);
District(TEXT("The Midway"), 1.15f, 1.84f);           Mix(TEXT("Crowd"), 10.f); Mix(TEXT("Rides"), 11.f);
District(TEXT("The big jump"), 1.84f, 2.22f);         Mix(TEXT("Rides"), 11.f); Mix(TEXT("Crowd"), 13.f);
District(TEXT("Pirate Cove"), 2.22f, 2.98f);          Mix(TEXT("Lagoon"), 11.f); Mix(TEXT("Crowd"), 13.f);
District(TEXT("The lift hill"), 2.98f, 3.21f);        Mix(TEXT("Rides"), 10.f); Mix(TEXT("Lagoon"), 14.f);
District(TEXT("The Lagoon"), 3.21f, 6.72f);           Mix(TEXT("Lagoon"), 10.f); Mix(TEXT("Rides"), 14.f);
District(TEXT("Splashdown"), 6.72f, 6.91f);           Mix(TEXT("Lagoon"), 10.f); Mix(TEXT("Crowd"), 12.f);
District(TEXT("Rocket Land"), 6.91f, 8.12f);          Mix(TEXT("Rocket"), 10.f); Mix(TEXT("Crowd"), 13.f);
District(TEXT("The Funhouse"), 8.12f, 8.44f);         Mix(TEXT("Funhouse"), 9.f); Mix(TEXT("Crowd"), 15.f);
District(TEXT("The funhouse jump"), 8.44f, 8.91f);    Mix(TEXT("Funhouse"), 11.f); Mix(TEXT("Rides"), 13.f);
District(TEXT("The back lot"), 8.91f, 9.30f);         Mix(TEXT("Rides"), 14.f); Mix(TEXT("Funhouse"), 14.f);
District(TEXT("The Last Ride"), 9.30f, 99.f);         Mix(TEXT("Rides"), 12.f); Mix(TEXT("Crowd"), 13.f);   // Mr. Chuckles' fight

// A coaster's train comes down over the Midway with its carful of screams.
Pass(TEXT("A coaster goes over"), TEXT("A_L4_Coaster"), 1.52f, -1, 40.f, 320.f, 5.f, 2.4f, 0.8f); Up(20.f);
// Pirate Cove: the galleon's bell, and her timbers.
Pass(TEXT("The galleon's bell"), TEXT("A_L4_ShipBell"), 2.5f, 1, 50.f, 300.f, 6.f, 0.6f, 0.4f);
// Splashdown: a boat hits the water, and everybody watching is soaked.
Pass(TEXT("Splashdown"), TEXT("A_L4_Splash"), 6.81f, 1, 40.f, 280.f, 4.f, 1.3f, 0.5f);
// Rocket Land: a ride goes up.
Pass(TEXT("A rocket ride goes up"), TEXT("A_L4_Launch"), 7.62f, -1, 60.f, 340.f, 5.f, 2.6f, 0.4f);
// The Funhouse: the laughing clown over its door.
Pass(TEXT("The laughing clown"), TEXT("A_L4_Laugh"), 8.28f, 1, 30.f, 260.f, 5.f, 1.5f, 0.4f);
