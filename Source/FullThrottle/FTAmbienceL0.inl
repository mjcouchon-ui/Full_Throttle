// The world's sound, level 0: Bayshore. The words (Music, Bed, District, Mix, Pass, Spot, Up) are explained at the top of
// FTAmbience.cpp; the sounds are made by Tools/sfx_ambience.py. Districts are the dressing table's (FTDressL0.inl).
// The bay is on the LEFT of travel all the way (the boardwalk, the beach, the sea 350 m off), the town on the RIGHT:
// the sea's beds are made leaning left, the town's leaning right.
Music(-23.4f);   // M_PixelAdventure as the game plays it

Bed(TEXT("Surf"), TEXT("A_L0_Surf"));             // the shore's roar, breakers and their foam, gulls far out
Bed(TEXT("Boardwalk"), TEXT("A_L0_Boardwalk"));   // people strolling and talking, a laugh, glasses, feet on the boards
Bed(TEXT("Marina"), TEXT("A_L0_Marina"));         // water lapping at hulls, halyards tapping masts, creaks, a buoy's bell
Bed(TEXT("Fair"), TEXT("A_L0_Fair"));             // an excited crowd, the coaster's chain and its drop with screams, a barker
Bed(TEXT("Night"), TEXT("A_L0_Night"));           // crickets, wind in the dune grass, the town's hum from the right

District(TEXT("The Strip"), 0.f, 3.2f);                 Mix(TEXT("Boardwalk"), 10.f); Mix(TEXT("Surf"), 13.f);
District(TEXT("The Marina"), 3.2f, 6.4f);               Mix(TEXT("Marina"), 10.f); Mix(TEXT("Surf"), 14.f); Mix(TEXT("Boardwalk"), 18.f);
District(TEXT("Pleasure Beach"), 6.4f, 9.8f);           Mix(TEXT("Fair"), 10.f); Mix(TEXT("Surf"), 15.f); Mix(TEXT("Boardwalk"), 15.f);
District(TEXT("Hotel Row"), 9.8f, 13.2f);               Mix(TEXT("Surf"), 12.f); Mix(TEXT("Night"), 12.f);
District(TEXT("The Point"), 13.2f, 16.8f);              Mix(TEXT("Surf"), 9.f); Mix(TEXT("Night"), 11.f);
District(TEXT("Downtown waterfront"), 16.8f, 99.f);     Mix(TEXT("Boardwalk"), 12.f); Mix(TEXT("Surf"), 15.f); Mix(TEXT("Fair"), 16.f);   // the Juggernaut's fight: lower

// The big wheel's fairground on the bay side of the first banked bend (the dressing's "WheelPark", 105 m out).
Spot(TEXT("The big wheel's organ"), TEXT("A_L0_Organ"), 1.176f, -1, 105.f, 420.f, 6.f);
// The chair swing on the floor of the first jump's dip, bay side: the car flies past its crown.
Pass(TEXT("The chair swing"), TEXT("A_L0_Swing"), 1.731f, -1, 52.f, 260.f, 5.f, 1.7f, 0.6f);
// A liner out in the bay, off the Marina.
Pass(TEXT("A liner's horn"), TEXT("A_L0_ShipHorn"), 4.85f, -1, 340.f, 600.f, 7.f, 1.5f, 0.3f);
// The fourth jump's swing, town side.
Pass(TEXT("The chair swing"), TEXT("A_L0_Swing"), 8.562f, 1, 52.f, 260.f, 5.f, 1.7f, 0.6f);
// The wheel at the end of the pleasure pier.
Spot(TEXT("The pier wheel's organ"), TEXT("A_L0_Organ"), 9.184f, -1, 250.f, 520.f, 8.f);
// Off the Point, by the lighthouse.
Pass(TEXT("A ship off the Point"), TEXT("A_L0_ShipHorn"), 14.9f, -1, 340.f, 600.f, 7.f, 1.5f, 0.3f);
// Sunset Strip: "the whole town is watching" (scene 15 starts at 15.29 km).
Pass(TEXT("Sunset Strip cheers"), TEXT("A_L0_Cheer"), 15.45f, 1, 40.f, 320.f, 5.f, 1.6f, 0.4f);
// Downtown's three pier wheels (the finale).
Spot(TEXT("A pier wheel's organ"), TEXT("A_L0_Organ"), 18.685f, -1, 250.f, 480.f, 9.f);
Spot(TEXT("A pier wheel's organ"), TEXT("A_L0_Organ"), 19.681f, -1, 250.f, 480.f, 9.f);
Spot(TEXT("A pier wheel's organ"), TEXT("A_L0_Organ"), 23.662f, -1, 250.f, 480.f, 9.f);
