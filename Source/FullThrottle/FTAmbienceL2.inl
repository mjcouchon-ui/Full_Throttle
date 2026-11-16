// The world's sound, level 2: the Nevada Desert. The words are explained at the top of FTAmbience.cpp; the sounds are
// made by Tools/sfx_ambience.py. Districts are the dressing table's (FTDressL2.inl).
Music(-23.1f);   // M_DesertFrontier as the game plays it

Bed(TEXT("Wind"), TEXT("A_L2_Wind"));         // desert wind in gusts, sand hissing over the ground, tumbleweed and wire
Bed(TEXT("Cicadas"), TEXT("A_L2_Cicadas"));   // cicadas' rasp swelling and dying away, a few crickets, dry brush ticking
Bed(TEXT("Highway"), TEXT("A_L2_Highway"));   // a far highway: the steady drone of tyres, a truck now and then, a jake brake
Bed(TEXT("Neon"), TEXT("A_L2_Neon"));         // the strip: neon buzzing and flickering, slot machines chattering (reels, bells, coins)
Bed(TEXT("Canyon"), TEXT("A_L2_Canyon"));     // red rock: wind moaning in the pass, stones trickling, every sound given back

District(TEXT("State line"), 0.f, 1.17f);             Mix(TEXT("Wind"), 11.f); Mix(TEXT("Highway"), 12.f);
District(TEXT("Dry wash"), 1.17f, 2.43f);             Mix(TEXT("Wind"), 10.f); Mix(TEXT("Cicadas"), 12.f);
District(TEXT("Snake Eyes country"), 2.43f, 3.83f);   Mix(TEXT("Cicadas"), 10.f); Mix(TEXT("Wind"), 13.f);
District(TEXT("Joshua Flats"), 3.83f, 5.14f);         Mix(TEXT("Wind"), 11.f); Mix(TEXT("Cicadas"), 12.f);
District(TEXT("Red Rock Pass"), 5.14f, 6.31f);        Mix(TEXT("Canyon"), 10.f); Mix(TEXT("Wind"), 14.f);
District(TEXT("Last Chance Gas"), 6.31f, 7.75f);      Mix(TEXT("Highway"), 11.f); Mix(TEXT("Neon"), 14.f);
District(TEXT("Jackpot Junction"), 7.75f, 9.05f);     Mix(TEXT("Neon"), 10.f); Mix(TEXT("Highway"), 13.f);
District(TEXT("City lights"), 9.05f, 10.69f);         Mix(TEXT("Neon"), 10.f); Mix(TEXT("Highway"), 12.f);
District(TEXT("Salt flats"), 10.69f, 14.5f);          Mix(TEXT("Wind"), 10.f); Mix(TEXT("Highway"), 16.f);   // the Big Dealer's fight
District(TEXT("Syndicate range"), 14.5f, 18.5f);      Mix(TEXT("Wind"), 11.f); Mix(TEXT("Canyon"), 14.f);
District(TEXT("Red rock return"), 18.5f, 22.f);       Mix(TEXT("Canyon"), 11.f); Mix(TEXT("Wind"), 13.f);
District(TEXT("Vegas approach"), 22.f, 99.f);         Mix(TEXT("Neon"), 11.f); Mix(TEXT("Highway"), 12.f);

// A big rig on the highway beside the state line, going the other way.
Pass(TEXT("A truck goes by"), TEXT("A_L2_Truck"), 0.7f, -1, 40.f, 320.f, 5.f, 2.2f, 1.f);
// A hawk over the pass, its cry coming back off the rock.
Pass(TEXT("A hawk over the pass"), TEXT("A_L2_Hawk"), 5.6f, -1, 120.f, 380.f, 6.f, 0.8f, 0.2f);
// Last Chance Gas: a car rolls over the forecourt's hose (ding ding) and the pump runs.
Pass(TEXT("The gas station's bell"), TEXT("A_L2_GasBell"), 6.9f, 1, 35.f, 260.f, 6.f, 0.7f, 0.5f);
// Jackpot Junction: somebody's machine pays out.
Pass(TEXT("A jackpot"), TEXT("A_L2_Jackpot"), 8.3f, 1, 40.f, 300.f, 5.f, 1.4f, 0.4f);
Pass(TEXT("A truck goes by"), TEXT("A_L2_Truck"), 9.8f, -1, 40.f, 320.f, 5.f, 2.2f, 1.f);
