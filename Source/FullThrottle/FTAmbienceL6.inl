// The world's sound, level 6: Under the Sea (the road to Nereida). The words are explained at the top of
// FTAmbience.cpp; the sounds are made by Tools/sfx_ambience.py. Districts are the dressing table's (FTDressL6.inl).
// Everything down here is heard through water: dull, slow, and a long way off.
Music(-24.2f);   // M_ToTheCityInTheSea as the game plays it

Bed(TEXT("Deep"), TEXT("A_L6_Deep"));         // the deep itself: pressure's hum, slow surges, the far thud of something large
Bed(TEXT("Bubbles"), TEXT("A_L6_Bubbles"));   // the reef: streams of bubbles rising, the crackle of shrimp, fish flicking past
Bed(TEXT("Kelp"), TEXT("A_L6_Kelp"));         // the forest: the swell pulling the kelp to and fro, stalks rubbing, a current's wash
Bed(TEXT("Station"), TEXT("A_L6_Station"));   // the station: pumps heard through the hull, an airlock's hiss, sonar far off, a radio
Bed(TEXT("Hull"), TEXT("A_L6_Hull"));         // the salvage field: hulls creaking and groaning, a chain, plates knocking in the current

District(TEXT("The reef"), 0.f, 1.7f);                    Mix(TEXT("Bubbles"), 10.f); Mix(TEXT("Deep"), 12.f);
District(TEXT("The kelp forest"), 1.7f, 3.4f);            Mix(TEXT("Kelp"), 10.f); Mix(TEXT("Deep"), 12.f); Mix(TEXT("Bubbles"), 16.f);
District(TEXT("The research station"), 3.4f, 5.0f);       Mix(TEXT("Station"), 10.f); Mix(TEXT("Deep"), 13.f);
District(TEXT("The salvage field"), 5.0f, 6.6f);          Mix(TEXT("Hull"), 10.f); Mix(TEXT("Deep"), 12.f);
District(TEXT("The approach to Nereida"), 6.6f, 7.94f);   Mix(TEXT("Deep"), 10.f); Mix(TEXT("Kelp"), 14.f); Mix(TEXT("Station"), 15.f);
District(TEXT("Nereida"), 7.94f, 99.f);                   Mix(TEXT("Deep"), 11.f); Mix(TEXT("Station"), 13.f); Mix(TEXT("Bubbles"), 15.f);   // the Great White's fight

// The fish tornado on the left (51 m): thousands of fish turning as one.
Pass(TEXT("The fish tornado"), TEXT("A_L6_Swirl"), 0.272f, -1, 51.f, 260.f, 6.f, 1.8f, 0.5f);
// A whale calling, far out in the kelp.
Pass(TEXT("A whale far off"), TEXT("A_L6_Whale"), 2.5f, -1, 300.f, 600.f, 7.f, 2.0f, 0.1f);
// The station's sonar: one ping and what the sea sends back of it.
Pass(TEXT("The station's sonar"), TEXT("A_L6_Sonar"), 4.2f, 1, 120.f, 420.f, 8.f, 0.3f, 0.3f);
// The salvage ship hanging over the field on the left (SM_HullAbove, 120 m): her hull works.
Pass(TEXT("The salvage ship"), TEXT("A_L6_Salvage"), 5.203f, -1, 120.f, 400.f, 5.f, 2.0f, 0.3f); Up(25.f);
// The whales over the gate of Nereida (SM_WhalePass, 38 m over the road).
Pass(TEXT("The whales over the gate"), TEXT("A_L6_Whale"), 8.189f, 0, 0.f, 420.f, 5.f, 2.0f, 0.2f); Up(38.f);
