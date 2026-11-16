// The world's sound, level 7: City Center (Midtown at midnight). The words are explained at the top of FTAmbience.cpp;
// the sounds are made by Tools/sfx_ambience.py. Districts are the dressing table's (FTDressL7.inl).
Music(-23.5f);   // M_UrbanChase as the game plays it

Bed(TEXT("Traffic"), TEXT("A_L7_Traffic"));       // the avenues' wash, cars going by, a bus's brakes, a horn far off, the subway under the street
Bed(TEXT("Crowd"), TEXT("A_L7_Crowd"));           // the sidewalks at night: talk, heels, a laugh, a shout for a cab, the diner's plates
Bed(TEXT("Works"), TEXT("A_L7_Works"));           // the back streets' machinery: a compressor, garage fans, steam from the street, drips, a shutter
Bed(TEXT("Plaza"), TEXT("A_L7_Plaza"));           // open air: fountains playing into their basins, a few people crossing
Bed(TEXT("Syndicate"), TEXT("A_L7_Syndicate"));   // transformer hum, wind moaning between black walls, banners cracking, a radio

District(TEXT("Fifth and Main"), 0.f, 1.10f);           Mix(TEXT("Traffic"), 10.f); Mix(TEXT("Crowd"), 11.f);
District(TEXT("Theatre Row"), 1.10f, 2.10f);            Mix(TEXT("Crowd"), 9.f); Mix(TEXT("Traffic"), 12.f);
District(TEXT("The Garages"), 2.10f, 3.13f);            Mix(TEXT("Works"), 10.f); Mix(TEXT("Traffic"), 13.f);
District(TEXT("The Plaza"), 3.13f, 4.62f);              Mix(TEXT("Plaza"), 10.f); Mix(TEXT("Traffic"), 15.f); Mix(TEXT("Crowd"), 15.f);
District(TEXT("Wall Street"), 4.62f, 6.29f);            Mix(TEXT("Traffic"), 11.f); Mix(TEXT("Works"), 14.f);
District(TEXT("Penthouse Row"), 6.29f, 7.66f);          Mix(TEXT("Syndicate"), 13.f); Mix(TEXT("Traffic"), 14.f); Mix(TEXT("Crowd"), 16.f);
District(TEXT("Syndicate Territory"), 7.66f, 99.f);     Mix(TEXT("Syndicate"), 10.f); Mix(TEXT("Traffic"), 18.f);   // the Chairman's fight

// A siren crossing an avenue down a side street, early in Fifth and Main (slow wail, far, mostly echo).
Pass(TEXT("A siren far off"), TEXT("A_L7_Siren"), 0.55f, -1, 140.f, 450.f, 9.f, 3.5f, 0.6f);
// The elevated railway behind Theatre Row, left (the dressing's ElLine at slot 30, 96 m out): its train goes by.
Pass(TEXT("The elevated train"), TEXT("A_L7_ElTrain"), 1.357f, -1, 96.f, 380.f, 6.f, 3.2f, 0.7f);
// A picture house on the right with its premiere searchlight (W_Marquee, slot 43).
Pass(TEXT("A premiere crowd"), TEXT("A_L7_Premiere"), 1.946f, 1, 29.f, 300.f, 5.f, 1.8f, 0.4f);
// THE GARAGE JUMP goes over the elevated railway, its train running under the car.
Pass(TEXT("The train under the jump"), TEXT("A_L7_ElTrain"), 2.76f, 0, 0.f, 380.f, 4.f, 3.2f, 0.7f);
// The Plaza's clock towers strike midnight (the level is at 23:59).
Pass(TEXT("The clock tower"), TEXT("A_L7_Bell"), 3.98f, 0, 90.f, 520.f, 6.f, 0.4f, 0.3f);
// THE WALL STREET JUMP goes over a boulevard of traffic.
Pass(TEXT("The boulevard under the jump"), TEXT("A_L7_Boulevard"), 5.791f, 0, 0.f, 320.f, 5.f, 2.2f, 0.8f);
// An airship over Penthouse Row (SM_Blimp at slot 152, right, 125 m up).
Pass(TEXT("The airship"), TEXT("A_L7_Airship"), 6.877f, 1, 60.f, 520.f, 8.f, 4.5f, 0.4f); Up(125.f);
