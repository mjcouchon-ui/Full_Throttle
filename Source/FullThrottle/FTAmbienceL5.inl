// The world's sound, level 5: Ice Mountain (Frostbite Peak). The words are explained at the top of FTAmbience.cpp; the
// sounds are made by Tools/sfx_ambience.py. Districts are the dressing table's (FTDressL5.inl).
Music(-23.4f);   // M_BlizzardRun as the game plays it

Bed(TEXT("Wind"), TEXT("A_L5_Wind"));           // mountain wind: gusts over the ridge, a whistle in the pines, a flag's rope
Bed(TEXT("Snow"), TEXT("A_L5_Snow"));           // snow's hush: the muffled quiet of a slope, pines sighing, snow sliding off a branch
Bed(TEXT("Lift"), TEXT("A_L5_Lift"));           // lift machinery: the cable's hum, chairs clicking over the sheaves, the motor house
Bed(TEXT("Blizzard"), TEXT("A_L5_Blizzard"));   // the whiteout: wind roaring, ice grains hissing past, everything else gone
Bed(TEXT("Glacier"), TEXT("A_L5_Glacier"));     // the ice: deep cracks and groans, meltwater under it, wind over the crevasses

District(TEXT("First tracks"), 0.f, 1.19f);                  Mix(TEXT("Snow"), 11.f); Mix(TEXT("Wind"), 11.f); Mix(TEXT("Lift"), 15.f);
District(TEXT("The half pipe"), 1.19f, 2.43f);               Mix(TEXT("Snow"), 11.f); Mix(TEXT("Lift"), 12.f);
District(TEXT("Avalanche chute"), 2.43f, 2.96f);             Mix(TEXT("Wind"), 10.f); Mix(TEXT("Snow"), 14.f);
District(TEXT("The super pipe"), 2.96f, 4.02f);              Mix(TEXT("Snow"), 11.f); Mix(TEXT("Lift"), 12.f);
District(TEXT("Black diamond"), 4.02f, 5.65f);               Mix(TEXT("Wind"), 10.f); Mix(TEXT("Glacier"), 13.f);
District(TEXT("The whiteout"), 5.65f, 7.3f);                 Mix(TEXT("Blizzard"), 9.f); Mix(TEXT("Wind"), 14.f);
District(TEXT("The valley run"), 7.3f, 8.94f);               Mix(TEXT("Snow"), 10.f); Mix(TEXT("Lift"), 13.f);
District(TEXT("The Abominable's glacier"), 8.94f, 11.6f);    Mix(TEXT("Glacier"), 10.f); Mix(TEXT("Wind"), 13.f);   // the Abominable's fight
District(TEXT("The white ridge"), 11.6f, 14.3f);             Mix(TEXT("Wind"), 10.f); Mix(TEXT("Blizzard"), 14.f);
District(TEXT("The resort"), 14.3f, 17.f);                   Mix(TEXT("Lift"), 10.f); Mix(TEXT("Snow"), 12.f);
District(TEXT("The ice again"), 17.f, 19.7f);                Mix(TEXT("Glacier"), 10.f); Mix(TEXT("Wind"), 13.f);
District(TEXT("The high ridge"), 19.7f, 22.4f);              Mix(TEXT("Wind"), 10.f); Mix(TEXT("Blizzard"), 14.f);
District(TEXT("The last lifts"), 22.4f, 99.f);               Mix(TEXT("Lift"), 10.f); Mix(TEXT("Snow"), 12.f);

// The gondolas that cross over the road (the dressing's GondolaCross): the hanger rattling over the tower's sheaves.
Pass(TEXT("The gondola over the top"), TEXT("A_L5_Gondola"), 0.225f, 1, 30.f, 300.f, 5.f, 2.2f, 0.7f); Up(25.f);
// The half pipe's stands: cowbells and a cheer.
Pass(TEXT("The half pipe's stands"), TEXT("A_L5_Cowbells"), 1.7f, 1, 30.f, 300.f, 5.f, 1.6f, 0.4f);
// Something lets go high up in the chute: a slide, far off to the left.
Pass(TEXT("A slide in the chute"), TEXT("A_L5_Avalanche"), 2.72f, -1, 300.f, 600.f, 5.f, 2.5f, 0.1f);
Pass(TEXT("The super pipe's stands"), TEXT("A_L5_Cowbells"), 3.1f, -1, 30.f, 260.f, 5.f, 1.6f, 0.4f);
Pass(TEXT("The gondola over the glacier"), TEXT("A_L5_Gondola"), 3.527f, 1, 30.f, 300.f, 5.f, 2.2f, 0.7f); Up(25.f);
Pass(TEXT("The gondola over the valley"), TEXT("A_L5_Gondola"), 8.325f, 1, 30.f, 300.f, 5.f, 2.2f, 0.7f); Up(25.f);
