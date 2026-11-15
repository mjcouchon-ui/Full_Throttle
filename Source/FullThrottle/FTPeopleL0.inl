// Bayshore (level index 0): the population layer. People, creatures and film crews, placed by rule like the dressing
// (the words and what their numbers mean are in the header of FTTrackDress.cpp; the rule kinds in FTDress.h).
// WRITTEN BY Content/Python/ft_people_bay_table.py: change that file and run it, never this one. Its header says why.
// The meshes: ft_people_bay.py and ft_people_bay_beach.py (/Game/People/Bayshore/), and the film crew in wardrobe
// Sun (ft_people_crew.py, /Game/People/Crew/). This table is read after FTDressL0.inl, so no dressing piece moves.
// The sight rule as the track applies it: a piece whose top stands 2 m over the road is pushed out to 24 m.

// ---- Sets. The boardwalk's deck is 0.48 m over the sand (ft_dress_bay.boardwalk): what walks on it is drawn from
// z = 0 and stood up by that much. The pier's people are drawn in the pier's own space: same anchor, no offsets.
Set(TEXT("PplWalkA"), { { TEXT("/Game/People/Bayshore/SM_Strollers_A"), 0, 0, 0.48f, 0, 1 } });
Set(TEXT("PplWalkB"), { { TEXT("/Game/People/Bayshore/SM_Strollers_B"), 0, 0, 0.48f, 0, 1 } });
Set(TEXT("PplPier"), { { TEXT("/Game/People/Bayshore/SM_Anglers"), 0, 0, 0, 0, 1 } });
// The film crew (wardrobe Sun). The town-side sets are anchored 21.9 m out, on the pavement between the kerb and
// the shopfronts, and go in side-street slots only (no shop row at 27.5 m there: the tall pieces stand in the
// street's mouth, 26 m out and more). The bay-side ones stand on the boardwalk's deck.
Set(TEXT("PplShootA"), {      // a camera position and the crane behind it
	{ TEXT("/Game/People/Crew/SM_CrewCamera_Sun"),   -13, 0,    0, 0, 1 },
	{ TEXT("/Game/People/Crew/SM_CrewCrane_Sun"),      1, 8.4f, 0, 0, 1 },
	{ TEXT("/Game/People/Crew/SM_CrewLights_Sun"),   -12, 5.2f, 0, 0, 1 },
	{ TEXT("/Game/People/Crew/SM_CrewApplauseA_Sun"), 13, 0,    0, 0, 1 },
});
Set(TEXT("PplShootB"), {      // the dolly on its rails, the director's village, sound, catering
	{ TEXT("/Game/People/Crew/SM_CrewDolly_Sun"),      0, 0,    0, 0, 1 },
	{ TEXT("/Game/People/Crew/SM_CrewVillage_Sun"),   14, 0.4f, 0, 0, 1 },
	{ TEXT("/Game/People/Crew/SM_CrewSound_Sun"),    -13, 4.6f, 0, 0, 1 },
	{ TEXT("/Game/People/Crew/SM_CrewCatering_Sun"),   3, 7.8f, 0, 0, 1 },
});
Set(TEXT("PplShootC"), {      // the stunt team before a jump
	{ TEXT("/Game/People/Crew/SM_CrewStunt_Sun"),      0, 0.3f, 0, 0, 1 },
	{ TEXT("/Game/People/Crew/SM_CrewApplauseB_Sun"), 13, 0,    0, 0, 1 },
	{ TEXT("/Game/People/Crew/SM_CrewLights_Sun"),   -12, 5.2f, 0, 0, 1 },
});
Set(TEXT("PplCrewWalkA"), { { TEXT("/Game/People/Crew/SM_CrewApplauseA_Sun"), 0, 0, 0.48f, 0, 1 } });
Set(TEXT("PplCrewWalkB"), { { TEXT("/Game/People/Crew/SM_CrewApplauseB_Sun"), 0, 0, 0.48f, 0, 1 } });
Set(TEXT("PplCrewWalkC"), { { TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 0, 0, 0.48f, 0, 1 } });

// ---- 0 to 3.2 km: THE STRIP. The town is out to watch. (The dressing, bay side: an arcade on the walk in slots
// s%4==2, the walk's shops in s%4==0, a beach club 34 m out in s%4==3, a bar 52 m out in s%4==1. Town side: side
// street s%4==0, the set-back shop row and a kiosk corner s%4==1, the two front rows s%4==2 and 3.)
Zone(TEXT("People, the Strip"), 0.f, 3.2f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_A"), 26.4f, 26.4f, 8, 7, 1.f, 1.f, 0.f, -1, 0.f);
// ---- 3.2 to 6.4 km: THE MARINA. Quieter sand, busier piers: anglers, gulls over the boats, dolphins off the
// jetties. (Bay side: the arcade s%4==2, the walk's shops s%6==3, a pier s%9==5.)
Zone(TEXT("People, the Marina"), 3.2f, 6.4f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_A"), 26.4f, 26.4f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
// ---- 6.4 to 9.8 km: PLEASURE BEACH. The beach at its busiest: a band or a terrace of people at every other
// slot, volleyball, kites, surfers. (Bay side: the arcade s%4==3, the walk's shops s%4==1, beach clubs in even
// slots, the dressing's own court 62 m out in s%4==3. Town side: front rows s%4==1 and 3, the set-back row s%4==2.)
Zone(TEXT("People, Pleasure Beach"), 6.4f, 9.8f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_A"), 26.4f, 26.4f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);
// ---- 9.8 to 13.2 km: HOTEL ROW. Joggers on the walk, the hotels' guests on the balconies, surfers. (Bay side:
// the arcade s%4==1, the walk's shops s%4==3, club decks and lidos 46 to 54 m out in even slots.)
Zone(TEXT("People, Hotel Row"), 9.8f, 13.2f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_A"), 26.4f, 26.4f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);
// ---- 13.2 to 16.8 km: THE POINT. The town thins out: gulls, surfers, dolphins off the rocks, a few families.
// (The boardwalk stands in every third slot here: s%3==0.)
Zone(TEXT("People, the Point"), 13.2f, 16.8f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_A"), 26.4f, 26.4f, 16, 15, 1.f, 1.f, 0.f, -1, 0.f);
// ---- 16.8 km to the end: THE FINALE, downtown's waterfront. Everything at once. (As the Strip; piers s%11==6.)
Zone(TEXT("People, Downtown waterfront"), 16.8f, 99.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_A"), 26.4f, 26.4f, 8, 7, 1.f, 1.f, 0.f, -1, 0.f);

// ---- The film crew (/Game/People/Crew/, wardrobe Sun). The level is a shoot: a unit by the start, the stunt team
// before every jump, a camera and its crane before every loop, a camera before every corkscrew, crew applauding on
// the boardwalk after them, a unit before the boss, and a camera every two kilometres between. One slot a district.
Zone(TEXT("People, the crew at the start"), 0.15835f, 0.20359f);
Zone(TEXT("People, the crew at the start"), 0.33932f, 0.38457f);
Zone(TEXT("People, the crew at the start"), 0.02262f, 0.06786f);
Zone(TEXT("People, the crew at the start"), 0.06786f, 0.11311f);
Zone(TEXT("People, the crew at the start"), 0.11311f, 0.15835f);
Zone(TEXT("People, the crew at the start"), 0.24884f, 0.29408f);
Zone(TEXT("People, the crew at the start"), 0.02262f, 0.06786f);
Zone(TEXT("People, the crew at the start"), 0.11311f, 0.15835f);
Zone(TEXT("People, the crew at the start"), 0.20359f, 0.24884f);
Zone(TEXT("People, the stunt team before a jump"), 1.60613f, 1.65137f);
Zone(TEXT("People, a camera on the jump"), 1.56088f, 1.60613f);
Zone(TEXT("People, crew past the landing"), 1.87758f, 1.92283f);
Zone(TEXT("People, the stunt team before a jump"), 3.23487f, 3.28012f);
Zone(TEXT("People, a camera on the jump"), 3.18963f, 3.23487f);
Zone(TEXT("People, crew past the landing"), 3.64206f, 3.68730f);
Zone(TEXT("People, the stunt team before a jump"), 5.22557f, 5.27081f);
Zone(TEXT("People, a camera on the jump"), 5.18032f, 5.22557f);
Zone(TEXT("People, crew past the landing"), 5.54227f, 5.58751f);
Zone(TEXT("People, the stunt team before a jump"), 8.30209f, 8.34733f);
Zone(TEXT("People, a camera on the jump"), 8.25685f, 8.30209f);
Zone(TEXT("People, crew past the landing"), 8.70928f, 8.75452f);
Zone(TEXT("People, the stunt team before a jump"), 11.37861f, 11.42386f);
Zone(TEXT("People, a camera on the jump"), 11.33337f, 11.37861f);
Zone(TEXT("People, crew past the landing"), 11.74056f, 11.78580f);
Zone(TEXT("People, the stunt team before a jump"), 14.45514f, 14.50038f);
Zone(TEXT("People, a camera on the jump"), 14.40990f, 14.45514f);
Zone(TEXT("People, crew past the landing"), 14.77184f, 14.81708f);
Zone(TEXT("People, a unit before a loop"), 1.78710f, 1.83234f);
Zone(TEXT("People, a unit before a loop"), 4.13973f, 4.18498f);
Zone(TEXT("People, crew before a loop"), 4.18498f, 4.23022f);
Zone(TEXT("People, a unit before a loop"), 6.85431f, 6.89956f);
Zone(TEXT("People, a unit before a loop"), 10.29278f, 10.33803f);
Zone(TEXT("People, a unit before a loop"), 12.64542f, 12.69066f);
Zone(TEXT("People, crew before a loop"), 12.69066f, 12.73590f);
Zone(TEXT("People, a unit before a loop"), 14.63611f, 14.68135f);
Zone(TEXT("People, a unit before a loop"), 16.08389f, 16.12913f);
Zone(TEXT("People, crew before a loop"), 16.21962f, 16.26486f);
Zone(TEXT("People, a unit before a loop"), 17.16972f, 17.21496f);
Zone(TEXT("People, crew before a loop"), 17.21496f, 17.26020f);
Zone(TEXT("People, a unit before a loop"), 18.61749f, 18.66274f);
Zone(TEXT("People, crew before a loop"), 18.66274f, 18.70798f);
Zone(TEXT("People, a unit before a loop"), 20.06527f, 20.11051f);
Zone(TEXT("People, a unit before a loop"), 21.51305f, 21.55829f);
Zone(TEXT("People, a unit before a loop"), 22.96082f, 23.00607f);
Zone(TEXT("People, a unit before a loop"), 24.40860f, 24.45384f);
Zone(TEXT("People, a camera on a corkscrew"), 2.82769f, 2.87293f);
Zone(TEXT("People, crew before a corkscrew"), 2.78244f, 2.82769f);
Zone(TEXT("People, a camera on a corkscrew"), 7.39723f, 7.44247f);
Zone(TEXT("People, crew before a corkscrew"), 7.35199f, 7.39723f);
Zone(TEXT("People, a camera on a corkscrew"), 10.06657f, 10.11181f);
Zone(TEXT("People, crew before a corkscrew"), 10.02132f, 10.06657f);
Zone(TEXT("People, a camera on a corkscrew"), 13.05261f, 13.09785f);
Zone(TEXT("People, crew before a corkscrew"), 13.00736f, 13.05261f);
Zone(TEXT("People, a camera on a corkscrew"), 15.58621f, 15.63146f);
Zone(TEXT("People, crew before a corkscrew"), 15.54097f, 15.58621f);
Zone(TEXT("People, a camera on a corkscrew"), 17.89361f, 17.93885f);
Zone(TEXT("People, crew before a corkscrew"), 17.84836f, 17.89361f);
Zone(TEXT("People, a camera on a corkscrew"), 19.29614f, 19.34138f);
Zone(TEXT("People, crew before a corkscrew"), 19.25090f, 19.29614f);
Zone(TEXT("People, a camera on a corkscrew"), 20.74392f, 20.78916f);
Zone(TEXT("People, crew before a corkscrew"), 20.69867f, 20.74392f);
Zone(TEXT("People, a camera on a corkscrew"), 22.19169f, 22.23693f);
Zone(TEXT("People, crew before a corkscrew"), 22.14645f, 22.19169f);
Zone(TEXT("People, a camera on a corkscrew"), 23.63947f, 23.68471f);
Zone(TEXT("People, crew before a corkscrew"), 23.59422f, 23.63947f);
Zone(TEXT("People, a camera on a corkscrew"), 25.08724f, 25.13249f);
Zone(TEXT("People, crew before a corkscrew"), 25.04200f, 25.08724f);
Zone(TEXT("People, the unit before the boss"), 24.04665f, 24.09190f);
Zone(TEXT("People, the unit before the boss"), 24.22763f, 24.27287f);
Zone(TEXT("People, the unit before the boss"), 24.77054f, 24.81579f);
Zone(TEXT("People, the unit before the boss"), 24.95151f, 24.99676f);
Zone(TEXT("People, the unit before the boss"), 24.13714f, 24.18238f);
Zone(TEXT("People, the unit before the boss"), 24.31811f, 24.36336f);
Zone(TEXT("People, the unit before the boss"), 24.68006f, 24.72530f);
Zone(TEXT("People, the unit before the boss"), 24.86103f, 24.90627f);
Zone(TEXT("People, the unit before the boss"), 25.04200f, 25.08724f);
Zone(TEXT("People, a camera along the way"), 0.97272f, 1.01797f);
Zone(TEXT("People, a camera along the way"), 2.96342f, 3.00866f);
Zone(TEXT("People, a camera along the way"), 4.95411f, 4.99935f);
Zone(TEXT("People, a camera along the way"), 6.94480f, 6.99004f);
Zone(TEXT("People, a camera along the way"), 8.93549f, 8.98074f);
Zone(TEXT("People, a camera along the way"), 10.92618f, 10.97143f);
Zone(TEXT("People, a camera along the way"), 12.91688f, 12.96212f);
Zone(TEXT("People, a camera along the way"), 14.90757f, 14.95281f);
Zone(TEXT("People, a camera along the way"), 16.89826f, 16.94350f);
Zone(TEXT("People, a camera along the way"), 18.88895f, 18.93420f);
Zone(TEXT("People, a camera along the way"), 20.87964f, 20.92489f);
Zone(TEXT("People, a camera along the way"), 22.87034f, 22.91558f);

// ---- The balconies, on the walls of the dressing's two front shop rows (27.5 m): one district for each of its
// Street(...) lines, the row's own slots, phase and height. Front1 has the surf shop's upper windows 5.8 m back
// along the row; Front2 the sweet shop's 4.6 m on. Front2's are left out from the Marina to Hotel Row (a
// balcony at every row read as a pattern).
Set(TEXT("PplBalc1"), { { TEXT("/Game/People/Bayshore/SM_Balcony_A"), -5.8f, 0, 0.f, 0, 1 } });
Set(TEXT("PplBalc2"), { { TEXT("/Game/People/Bayshore/SM_Balcony_A"), 4.6f, 0, 0.f, 0, 1 } });
Set(TEXT("PplBalc1B"), { { TEXT("/Game/People/Bayshore/SM_Balcony_A"), -5.8f, 0, 0.45f, 0, 1 } });
Set(TEXT("PplBalc2B"), { { TEXT("/Game/People/Bayshore/SM_Balcony_A"), 4.6f, 0, 0.45f, 0, 1 } });
Set(TEXT("PplBalc1C"), { { TEXT("/Game/People/Bayshore/SM_Balcony_A"), -5.8f, 0, 0.95f, 0, 1 } });
Set(TEXT("PplBalc2C"), { { TEXT("/Game/People/Bayshore/SM_Balcony_A"), 4.6f, 0, 0.95f, 0, 1 } });
Zone(TEXT("People, balconies"), 0.02262f, 0.15835f);
Zone(TEXT("People, balconies"), 0.38457f, 0.47505f);
Zone(TEXT("People, balconies"), 1.01797f, 1.24418f);
Zone(TEXT("People, balconies"), 1.87758f, 1.92283f);
Zone(TEXT("People, balconies"), 2.19429f, 2.42050f);
Zone(TEXT("People, balconies"), 2.78244f, 2.87293f);
Zone(TEXT("People, balconies"), 3.00866f, 3.05390f);
Zone(TEXT("People, balconies"), 0.20359f, 0.29408f);
Zone(TEXT("People, balconies"), 0.33932f, 0.38457f);
Zone(TEXT("People, balconies"), 0.47505f, 0.52029f);
Zone(TEXT("People, balconies"), 0.61078f, 0.65602f);
Zone(TEXT("People, balconies"), 1.33467f, 1.37991f);
Zone(TEXT("People, balconies"), 1.47040f, 1.51564f);
Zone(TEXT("People, balconies"), 1.60613f, 1.65137f);
Zone(TEXT("People, balconies"), 1.78710f, 1.87758f);
Zone(TEXT("People, balconies"), 2.42050f, 2.46574f);
Zone(TEXT("People, balconies"), 2.55623f, 2.60147f);
Zone(TEXT("People, balconies"), 0.15835f, 0.20359f);
Zone(TEXT("People, balconies"), 0.29408f, 0.33932f);
Zone(TEXT("People, balconies"), 0.52029f, 0.61078f);
Zone(TEXT("People, balconies"), 0.70127f, 0.79175f);
Zone(TEXT("People, balconies"), 0.83700f, 0.92748f);
Zone(TEXT("People, balconies"), 0.97272f, 1.01797f);
Zone(TEXT("People, balconies"), 1.24418f, 1.33467f);
Zone(TEXT("People, balconies"), 1.37991f, 1.47040f);
Zone(TEXT("People, balconies"), 2.60147f, 2.78244f);
Zone(TEXT("People, balconies"), 3.14439f, 3.18963f);
Zone(TEXT("People, balconies"), 3.32536f, 3.37060f);
Zone(TEXT("People, balconies"), 3.68730f, 3.95876f);
Zone(TEXT("People, balconies"), 4.18498f, 4.23022f);
Zone(TEXT("People, balconies"), 4.54692f, 4.90887f);
Zone(TEXT("People, balconies"), 5.58751f, 6.22091f);
Zone(TEXT("People, balconies"), 3.64206f, 3.68730f);
Zone(TEXT("People, balconies"), 3.95876f, 4.00401f);
Zone(TEXT("People, balconies"), 4.04925f, 4.09449f);
Zone(TEXT("People, balconies"), 4.13973f, 4.18498f);
Zone(TEXT("People, balconies"), 4.90887f, 4.95411f);
Zone(TEXT("People, balconies"), 5.49702f, 5.58751f);
Zone(TEXT("People, balconies"), 6.31140f, 6.40188f);
Zone(TEXT("People, balconies"), 3.18963f, 3.23487f);
Zone(TEXT("People, balconies"), 3.37060f, 3.41585f);
Zone(TEXT("People, balconies"), 3.59682f, 3.64206f);
Zone(TEXT("People, balconies"), 4.00401f, 4.04925f);
Zone(TEXT("People, balconies"), 4.09449f, 4.13973f);
Zone(TEXT("People, balconies"), 4.95411f, 5.22557f);
Zone(TEXT("People, balconies"), 5.27081f, 5.31605f);
Zone(TEXT("People, balconies"), 6.22091f, 6.31140f);
Zone(TEXT("People, balconies"), 6.53761f, 6.89956f);
Zone(TEXT("People, balconies"), 7.21626f, 7.26150f);
Zone(TEXT("People, balconies"), 7.98539f, 8.34733f);
Zone(TEXT("People, balconies"), 8.70928f, 8.75452f);
Zone(TEXT("People, balconies"), 8.98074f, 9.25219f);
Zone(TEXT("People, balconies"), 9.65938f, 9.79511f);
Zone(TEXT("People, balconies"), 6.40188f, 6.53761f);
Zone(TEXT("People, balconies"), 7.62345f, 7.66869f);
Zone(TEXT("People, balconies"), 7.84966f, 7.89490f);
Zone(TEXT("People, balconies"), 7.94015f, 7.98539f);
Zone(TEXT("People, balconies"), 8.43782f, 8.48306f);
Zone(TEXT("People, balconies"), 8.61879f, 8.70928f);
Zone(TEXT("People, balconies"), 9.25219f, 9.29744f);
Zone(TEXT("People, balconies"), 7.26150f, 7.30674f);
Zone(TEXT("People, balconies"), 7.39723f, 7.44247f);
Zone(TEXT("People, balconies"), 7.71393f, 7.84966f);
Zone(TEXT("People, balconies"), 7.89490f, 7.94015f);
Zone(TEXT("People, balconies"), 8.75452f, 8.98074f);
Zone(TEXT("People, balconies"), 9.43317f, 9.65938f);
Zone(TEXT("People, balconies"), 9.79511f, 9.84035f);
Zone(TEXT("People, balconies"), 10.02132f, 10.11181f);
Zone(TEXT("People, balconies"), 10.29278f, 10.33803f);
Zone(TEXT("People, balconies"), 10.65473f, 10.74521f);
Zone(TEXT("People, balconies"), 10.97143f, 11.28813f);
Zone(TEXT("People, balconies"), 11.78580f, 11.87629f);
Zone(TEXT("People, balconies"), 12.05726f, 12.46445f);
Zone(TEXT("People, balconies"), 12.69066f, 12.73590f);
Zone(TEXT("People, balconies"), 13.05261f, 13.09785f);
Zone(TEXT("People, balconies"), 11.28813f, 11.33337f);
Zone(TEXT("People, balconies"), 11.42386f, 11.51434f);
Zone(TEXT("People, balconies"), 12.50969f, 12.60018f);
Zone(TEXT("People, balconies"), 9.84035f, 9.88560f);
Zone(TEXT("People, balconies"), 9.97608f, 10.02132f);
Zone(TEXT("People, balconies"), 10.74521f, 10.97143f);
Zone(TEXT("People, balconies"), 11.69532f, 11.78580f);
Zone(TEXT("People, balconies"), 11.87629f, 12.05726f);
Zone(TEXT("People, balconies"), 12.46445f, 12.50969f);
Zone(TEXT("People, balconies"), 12.60018f, 12.69066f);
Zone(TEXT("People, balconies"), 13.36931f, 13.77649f);
Zone(TEXT("People, balconies"), 14.00271f, 14.40990f);
Zone(TEXT("People, balconies"), 14.77184f, 14.81708f);
Zone(TEXT("People, balconies"), 15.08854f, 15.63146f);
Zone(TEXT("People, balconies"), 15.99340f, 16.26486f);
Zone(TEXT("People, balconies"), 16.53632f, 16.76253f);
Zone(TEXT("People, balconies"), 13.77649f, 14.00271f);
Zone(TEXT("People, balconies"), 14.50038f, 14.54562f);
Zone(TEXT("People, balconies"), 14.68135f, 14.77184f);
Zone(TEXT("People, balconies"), 16.76253f, 16.80777f);
Zone(TEXT("People, balconies"), 13.32406f, 13.36931f);
Zone(TEXT("People, balconies"), 15.81243f, 15.99340f);
Zone(TEXT("People, balconies"), 17.03399f, 17.26020f);
Zone(TEXT("People, balconies"), 17.66739f, 17.93885f);
Zone(TEXT("People, balconies"), 18.43652f, 18.70798f);
Zone(TEXT("People, balconies"), 19.06992f, 19.34138f);
Zone(TEXT("People, balconies"), 19.88430f, 20.11051f);
Zone(TEXT("People, balconies"), 20.51770f, 20.78916f);
Zone(TEXT("People, balconies"), 21.33207f, 21.55829f);
Zone(TEXT("People, balconies"), 21.96548f, 22.23693f);
Zone(TEXT("People, balconies"), 22.77985f, 23.00607f);
Zone(TEXT("People, balconies"), 23.41325f, 23.68471f);
Zone(TEXT("People, balconies"), 24.22763f, 24.45384f);
Zone(TEXT("People, balconies"), 24.86103f, 25.13249f);
Zone(TEXT("People, balconies"), 16.98875f, 17.03399f);
Zone(TEXT("People, balconies"), 17.62215f, 17.66739f);
Zone(TEXT("People, balconies"), 18.21031f, 18.25555f);
Zone(TEXT("People, balconies"), 19.52235f, 19.56760f);
Zone(TEXT("People, balconies"), 23.27752f, 23.32277f);
Zone(TEXT("People, balconies"), 24.18238f, 24.22763f);
Zone(TEXT("People, balconies"), 24.81579f, 24.86103f);
Zone(TEXT("People, balconies"), 16.80777f, 16.98875f);
Zone(TEXT("People, balconies"), 18.25555f, 18.43652f);
Zone(TEXT("People, balconies"), 19.65808f, 19.88430f);
Zone(TEXT("People, balconies"), 20.97013f, 21.01537f);
Zone(TEXT("People, balconies"), 21.10586f, 21.33207f);
Zone(TEXT("People, balconies"), 22.41791f, 22.46315f);
Zone(TEXT("People, balconies"), 22.50839f, 22.77985f);
Zone(TEXT("People, balconies"), 23.36801f, 23.41325f);
Zone(TEXT("People, balconies"), 23.95617f, 24.18238f);
Zone(TEXT("People, balconies"), 24.72530f, 24.77054f);
