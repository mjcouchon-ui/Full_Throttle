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
	{ TEXT("/Game/People/Crew/SM_CrewSound_Sun"),    -13, 5.3f, 0, 0, 1 },   // fix round: 4.6 -> 5.3, the boom reached to 23.4 m
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
	Site(TEXT("PplWalkA"), -1, 22.9f, 4, 0, 1.f);
	Site(TEXT("PplWalkB"), -1, 22.9f, 4, 2, 1.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_A"), 26.4f, 26.4f, 8, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_B"), 26.4f, 26.4f, 8, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Bandstand"), 28.2f, 28.2f, 16, 9, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_GuardTower"), 27.f, 27.f, 16, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_BeachDay_A"), 44.f, 50.f, 4, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_BeachDay_B"), 66.f, 78.f, 8, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Volley_A"), 74.f, 82.f, 8, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SurfWalk"), 84.f, 96.f, 8, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Gulls"), 70.f, 160.f, 8, 6, 1.f, 1.f, 0.f, -1, 180.f);
	Site(TEXT("PplPier"), -1, 190.f, 14, 9, 1.f);   // the dressing's PierSet: the same slots
	Afloat(TEXT("/Game/People/Bayshore/SM_SurfRiders"), 430.f, 520.f, 9, 5, 1.f, 2.6f, 0.15f, -1, 20.f);
	Afloat(TEXT("/Game/People/Bayshore/SM_Dolphins"), 480.f, 620.f, 23, 16, 1.f, 3.f, 0.15f, -1, 180.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Shoppers_A"), 24.6f, 24.6f, 4, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Shoppers_A"), 24.6f, 24.6f, 4, 1, 1.f, 1.f, 0.f, 1, 0.f);
// ---- 3.2 to 6.4 km: THE MARINA. Quieter sand, busier piers: anglers, gulls over the boats, dolphins off the
// jetties. (Bay side: the arcade s%4==2, the walk's shops s%6==3, a pier s%9==5.)
Zone(TEXT("People, the Marina"), 3.2f, 6.4f);
	Site(TEXT("PplWalkB"), -1, 22.9f, 6, 0, 1.f);
	Site(TEXT("PplWalkA"), -1, 22.9f, 6, 3, 1.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_A"), 26.4f, 26.4f, 12, 11, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_B"), 26.4f, 26.4f, 12, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_GuardTower"), 27.f, 27.f, 24, 19, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_BeachDay_A"), 40.f, 46.f, 8, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Gulls"), 60.f, 200.f, 5, 4, 1.f, 1.f, 0.f, -1, 180.f);
	Site(TEXT("PplPier"), -1, 190.f, 9, 4, 1.f);   // the dressing's PierSet
	Afloat(TEXT("/Game/People/Bayshore/SM_Dolphins"), 430.f, 560.f, 17, 14, 1.f, 3.f, 0.15f, -1, 180.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Shoppers_A"), 24.6f, 24.6f, 4, 2, 1.f, 1.f, 0.f, 1, 0.f);
// ---- 6.4 to 9.8 km: PLEASURE BEACH. The beach at its busiest: a band or a terrace of people at every other
// slot, volleyball, kites, surfers. (Bay side: the arcade s%4==3, the walk's shops s%4==1, beach clubs in even
// slots, the dressing's own court 62 m out in s%4==3. Town side: front rows s%4==1 and 3, the set-back row s%4==2.)
Zone(TEXT("People, Pleasure Beach"), 6.4f, 9.8f);
	Site(TEXT("PplWalkA"), -1, 22.9f, 4, 3, 1.f);
	Site(TEXT("PplWalkB"), -1, 22.9f, 4, 1, 1.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_A"), 26.4f, 26.4f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_B"), 26.4f, 26.4f, 8, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Bandstand"), 28.2f, 28.2f, 8, 6, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_GuardTower"), 27.f, 27.f, 8, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Volley_A"), 74.f, 82.f, 8, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_BeachDay_A"), 70.f, 80.f, 8, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_BeachDay_B"), 76.f, 86.f, 4, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SurfWalk"), 88.f, 98.f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Gulls"), 70.f, 160.f, 8, 2, 1.f, 1.f, 0.f, -1, 180.f);
	Site(TEXT("PplPier"), -1, 190.f, 12, 1, 1.f);   // the dressing's PierSet
	Afloat(TEXT("/Game/People/Bayshore/SM_SurfRiders"), 430.f, 520.f, 9, 6, 1.f, 2.6f, 0.15f, -1, 20.f);
	Afloat(TEXT("/Game/People/Bayshore/SM_Dolphins"), 480.f, 620.f, 29, 24, 1.f, 3.f, 0.15f, -1, 180.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Shoppers_A"), 24.6f, 24.6f, 4, 3, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Shoppers_A"), 24.6f, 24.6f, 4, 1, 1.f, 1.f, 0.f, 1, 0.f);
// ---- 9.8 to 13.2 km: HOTEL ROW. Joggers on the walk, the hotels' guests on the balconies, surfers. (Bay side:
// the arcade s%4==1, the walk's shops s%4==3, club decks and lidos 46 to 54 m out in even slots.)
Zone(TEXT("People, Hotel Row"), 9.8f, 13.2f);
	Site(TEXT("PplWalkB"), -1, 22.9f, 6, 5, 1.f);
	Site(TEXT("PplWalkA"), -1, 22.9f, 6, 2, 1.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_A"), 26.4f, 26.4f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_B"), 26.4f, 26.4f, 16, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_GuardTower"), 27.f, 27.f, 16, 12, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_BeachDay_A"), 44.f, 52.f, 8, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_BeachDay_B"), 44.f, 52.f, 8, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SurfWalk"), 70.f, 90.f, 8, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Gulls"), 70.f, 160.f, 8, 1, 1.f, 1.f, 0.f, -1, 180.f);
	Afloat(TEXT("/Game/People/Bayshore/SM_SurfRiders"), 430.f, 520.f, 8, 6, 1.f, 2.6f, 0.15f, -1, 20.f);
	Afloat(TEXT("/Game/People/Bayshore/SM_Dolphins"), 480.f, 620.f, 31, 22, 1.f, 3.f, 0.15f, -1, 180.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Shoppers_A"), 24.6f, 24.6f, 4, 2, 1.f, 1.f, 0.f, 1, 0.f);
// ---- 13.2 to 16.8 km: THE POINT. The town thins out: gulls, surfers, dolphins off the rocks, a few families.
// (The boardwalk stands in every third slot here: s%3==0.)
Zone(TEXT("People, the Point"), 13.2f, 16.8f);
	Site(TEXT("PplWalkB"), -1, 22.9f, 6, 0, 1.f);
	Site(TEXT("PplWalkA"), -1, 22.9f, 6, 3, 1.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_A"), 26.4f, 26.4f, 16, 15, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_GuardTower"), 27.f, 27.f, 16, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_BeachDay_A"), 34.f, 48.f, 8, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SurfWalk"), 40.f, 56.f, 8, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Gulls"), 40.f, 200.f, 4, 2, 1.f, 1.f, 0.f, -1, 180.f);
	Afloat(TEXT("/Game/People/Bayshore/SM_SurfRiders"), 420.f, 520.f, 6, 5, 1.f, 2.6f, 0.15f, -1, 20.f);
	Afloat(TEXT("/Game/People/Bayshore/SM_Dolphins"), 440.f, 600.f, 13, 7, 1.f, 3.f, 0.15f, -1, 180.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Shoppers_A"), 24.6f, 24.6f, 8, 5, 1.f, 1.f, 0.f, 1, 0.f);
// ---- 16.8 km to the end: THE FINALE, downtown's waterfront. Everything at once. (As the Strip; piers s%11==6.)
Zone(TEXT("People, Downtown waterfront"), 16.8f, 99.f);
	Site(TEXT("PplWalkA"), -1, 22.9f, 4, 0, 1.f);
	Site(TEXT("PplWalkB"), -1, 22.9f, 4, 2, 1.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_A"), 26.4f, 26.4f, 8, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SunDeck_B"), 26.4f, 26.4f, 8, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Bandstand"), 28.2f, 28.2f, 16, 13, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_GuardTower"), 27.f, 27.f, 16, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_BeachDay_A"), 70.f, 80.f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_BeachDay_B"), 70.f, 80.f, 8, 4, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Volley_A"), 72.f, 82.f, 16, 10, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_SurfWalk"), 84.f, 96.f, 16, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Gulls"), 70.f, 160.f, 8, 5, 1.f, 1.f, 0.f, -1, 180.f);
	Site(TEXT("PplPier"), -1, 190.f, 11, 5, 1.f);   // the dressing's PierSet
	Afloat(TEXT("/Game/People/Bayshore/SM_SurfRiders"), 430.f, 520.f, 9, 7, 1.f, 2.6f, 0.15f, -1, 20.f);
	Afloat(TEXT("/Game/People/Bayshore/SM_Dolphins"), 480.f, 620.f, 19, 15, 1.f, 3.f, 0.15f, -1, 180.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_Shoppers_A"), 24.6f, 24.6f, 4, 2, 1.f, 1.f, 0.f, 1, 0.f);

// ---- The film crew (/Game/People/Crew/, wardrobe Sun). The level is a shoot: a unit by the start, the stunt team
// before every jump, a camera and its crane before every loop, a camera before every corkscrew, crew applauding on
// the boardwalk after them, a unit before the boss, and a camera every two kilometres between. One slot a district.
Zone(TEXT("People, the crew at the start"), 0.15835f, 0.20359f);
	Site(TEXT("PplShootA"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, the crew at the start"), 0.33932f, 0.38457f);
	Site(TEXT("PplShootB"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, the crew at the start"), 0.02262f, 0.06786f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, the crew at the start"), 0.06786f, 0.11311f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewApplauseB_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, the crew at the start"), 0.11311f, 0.15835f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewVillage_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, the crew at the start"), 0.24884f, 0.29408f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewApplauseA_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, the crew at the start"), 0.02262f, 0.06786f);
	Site(TEXT("PplCrewWalkA"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, the crew at the start"), 0.11311f, 0.15835f);
	Site(TEXT("PplCrewWalkC"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, the crew at the start"), 0.20359f, 0.24884f);
	Site(TEXT("PplCrewWalkB"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, the stunt team before a jump"), 1.60613f, 1.65137f);
	Site(TEXT("PplShootC"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, a camera on the jump"), 1.56088f, 1.60613f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew past the landing"), 1.87758f, 1.92283f);
	Site(TEXT("PplCrewWalkB"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, the stunt team before a jump"), 3.23487f, 3.28012f);
	Site(TEXT("PplShootC"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, a camera on the jump"), 3.18963f, 3.23487f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew past the landing"), 3.64206f, 3.68730f);
	Site(TEXT("PplCrewWalkA"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, the stunt team before a jump"), 5.22557f, 5.27081f);
	Site(TEXT("PplShootC"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, a camera on the jump"), 5.18032f, 5.22557f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew past the landing"), 5.54227f, 5.58751f);
	Site(TEXT("PplCrewWalkB"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, the stunt team before a jump"), 8.30209f, 8.34733f);
	Site(TEXT("PplShootC"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, a camera on the jump"), 8.25685f, 8.30209f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew past the landing"), 8.70928f, 8.75452f);
	Site(TEXT("PplCrewWalkA"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, the stunt team before a jump"), 11.37861f, 11.42386f);
	Site(TEXT("PplShootC"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, a camera on the jump"), 11.33337f, 11.37861f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew past the landing"), 11.74056f, 11.78580f);
	Site(TEXT("PplCrewWalkB"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, the stunt team before a jump"), 14.45514f, 14.50038f);
	Site(TEXT("PplShootC"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, a camera on the jump"), 14.40990f, 14.45514f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew past the landing"), 14.77184f, 14.81708f);
	Site(TEXT("PplCrewWalkA"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, a unit before a loop"), 1.78710f, 1.83234f);
	Site(TEXT("PplShootA"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, a unit before a loop"), 4.13973f, 4.18498f);
	Site(TEXT("PplShootB"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, crew before a loop"), 4.18498f, 4.23022f);
	Site(TEXT("PplCrewWalkB"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, a unit before a loop"), 6.85431f, 6.89956f);
	Site(TEXT("PplShootA"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, a unit before a loop"), 10.29278f, 10.33803f);
	Site(TEXT("PplShootB"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, a unit before a loop"), 12.64542f, 12.69066f);
	Site(TEXT("PplShootA"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, crew before a loop"), 12.69066f, 12.73590f);
	Site(TEXT("PplCrewWalkA"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, a unit before a loop"), 14.63611f, 14.68135f);
	Site(TEXT("PplShootB"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, a unit before a loop"), 16.08389f, 16.12913f);
	Site(TEXT("PplShootA"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, crew before a loop"), 16.21962f, 16.26486f);
	Site(TEXT("PplCrewWalkA"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, a unit before a loop"), 17.16972f, 17.21496f);
	Site(TEXT("PplShootB"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, crew before a loop"), 17.21496f, 17.26020f);
	Site(TEXT("PplCrewWalkB"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, a unit before a loop"), 18.61749f, 18.66274f);
	Site(TEXT("PplShootA"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, crew before a loop"), 18.66274f, 18.70798f);
	Site(TEXT("PplCrewWalkA"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, a unit before a loop"), 20.06527f, 20.11051f);
	Site(TEXT("PplShootB"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, a unit before a loop"), 21.51305f, 21.55829f);
	Site(TEXT("PplShootA"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, a unit before a loop"), 22.96082f, 23.00607f);
	Site(TEXT("PplShootB"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, a unit before a loop"), 24.40860f, 24.45384f);
	Site(TEXT("PplShootA"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, a camera on a corkscrew"), 2.82769f, 2.87293f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew before a corkscrew"), 2.78244f, 2.82769f);
	Site(TEXT("PplCrewWalkB"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, a camera on a corkscrew"), 7.39723f, 7.44247f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewDolly_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew before a corkscrew"), 7.35199f, 7.39723f);
	Site(TEXT("PplCrewWalkA"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, a camera on a corkscrew"), 10.06657f, 10.11181f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew before a corkscrew"), 10.02132f, 10.06657f);
	Site(TEXT("PplCrewWalkB"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, a camera on a corkscrew"), 13.05261f, 13.09785f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewDolly_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew before a corkscrew"), 13.00736f, 13.05261f);
	Site(TEXT("PplCrewWalkA"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, a camera on a corkscrew"), 15.58621f, 15.63146f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew before a corkscrew"), 15.54097f, 15.58621f);
	Site(TEXT("PplCrewWalkB"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, a camera on a corkscrew"), 17.89361f, 17.93885f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewDolly_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew before a corkscrew"), 17.84836f, 17.89361f);
	Site(TEXT("PplCrewWalkA"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, a camera on a corkscrew"), 19.29614f, 19.34138f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew before a corkscrew"), 19.25090f, 19.29614f);
	Site(TEXT("PplCrewWalkB"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, a camera on a corkscrew"), 20.74392f, 20.78916f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewDolly_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew before a corkscrew"), 20.69867f, 20.74392f);
	Site(TEXT("PplCrewWalkA"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, a camera on a corkscrew"), 22.19169f, 22.23693f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew before a corkscrew"), 22.14645f, 22.19169f);
	Site(TEXT("PplCrewWalkB"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, a camera on a corkscrew"), 23.63947f, 23.68471f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewDolly_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew before a corkscrew"), 23.59422f, 23.63947f);
	Site(TEXT("PplCrewWalkA"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, a camera on a corkscrew"), 25.08724f, 25.13249f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, crew before a corkscrew"), 25.04200f, 25.08724f);
	Site(TEXT("PplCrewWalkB"), -1, 22.6f, 1, 0, 1.f);
Zone(TEXT("People, the unit before the boss"), 24.04665f, 24.09190f);
	Site(TEXT("PplShootB"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, the unit before the boss"), 24.22763f, 24.27287f);
	Site(TEXT("PplShootC"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, the unit before the boss"), 24.77054f, 24.81579f);
	Site(TEXT("PplShootA"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, the unit before the boss"), 24.95151f, 24.99676f);
	Site(TEXT("PplShootB"), 1, 21.9f, 1, 0, 1.f);
Zone(TEXT("People, the unit before the boss"), 24.13714f, 24.18238f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, the unit before the boss"), 24.31811f, 24.36336f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewApplauseA_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, the unit before the boss"), 24.68006f, 24.72530f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewApplauseB_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, the unit before the boss"), 24.86103f, 24.90627f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, the unit before the boss"), 25.04200f, 25.08724f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewVillage_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, a camera along the way"), 0.97272f, 1.01797f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, a camera along the way"), 2.96342f, 3.00866f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewApplauseA_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, a camera along the way"), 4.95411f, 4.99935f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, a camera along the way"), 6.94480f, 6.99004f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewApplauseA_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, a camera along the way"), 8.93549f, 8.98074f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, a camera along the way"), 10.92618f, 10.97143f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewApplauseA_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, a camera along the way"), 12.91688f, 12.96212f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, a camera along the way"), 14.90757f, 14.95281f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewApplauseA_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, a camera along the way"), 16.89826f, 16.94350f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, a camera along the way"), 18.88895f, 18.93420f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewApplauseA_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, a camera along the way"), 20.87964f, 20.92489f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewCamera_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People, a camera along the way"), 22.87034f, 22.91558f);
	Scatter(TEXT("/Game/People/Crew/SM_CrewApplauseA_Sun"), 21.9f, 21.9f, 1, 0, 1.f, 1.f, 0.f, 1, 0.f);

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
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 0.38457f, 0.47505f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 1.01797f, 1.24418f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 1.87758f, 1.92283f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 2.19429f, 2.42050f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 2.78244f, 2.87293f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 3.00866f, 3.05390f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 0.20359f, 0.29408f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 0.33932f, 0.38457f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 0.47505f, 0.52029f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 0.61078f, 0.65602f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 1.33467f, 1.37991f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 1.47040f, 1.51564f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 1.60613f, 1.65137f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 1.78710f, 1.87758f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 2.42050f, 2.46574f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 2.55623f, 2.60147f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 0.15835f, 0.20359f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 0.29408f, 0.33932f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 0.52029f, 0.61078f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 0.70127f, 0.79175f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 0.83700f, 0.92748f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 0.97272f, 1.01797f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 1.24418f, 1.33467f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 1.37991f, 1.47040f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 2.60147f, 2.78244f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 3.14439f, 3.18963f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 3.32536f, 3.37060f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 3.68730f, 3.95876f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 4.18498f, 4.23022f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 4.54692f, 4.90887f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 5.58751f, 6.22091f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 3.64206f, 3.68730f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 3.95876f, 4.00401f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 4.04925f, 4.09449f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 4.13973f, 4.18498f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 4.90887f, 4.95411f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 5.49702f, 5.58751f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 6.31140f, 6.40188f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 3.18963f, 3.23487f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 3.37060f, 3.41585f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 3.59682f, 3.64206f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 4.00401f, 4.04925f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 4.09449f, 4.13973f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 4.95411f, 5.22557f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 5.27081f, 5.31605f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 6.22091f, 6.31140f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 6.53761f, 6.89956f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 7.21626f, 7.26150f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 7.98539f, 8.34733f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 8.70928f, 8.75452f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 8.98074f, 9.25219f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 9.65938f, 9.79511f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 6.40188f, 6.53761f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 7.62345f, 7.66869f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 7.84966f, 7.89490f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 7.94015f, 7.98539f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 8.43782f, 8.48306f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 8.61879f, 8.70928f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 9.25219f, 9.29744f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 7.26150f, 7.30674f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 7.39723f, 7.44247f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 7.71393f, 7.84966f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 7.89490f, 7.94015f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 8.75452f, 8.98074f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 9.43317f, 9.65938f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 3, 1.f);
Zone(TEXT("People, balconies"), 9.79511f, 9.84035f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 10.02132f, 10.11181f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 10.29278f, 10.33803f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 10.65473f, 10.74521f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 10.97143f, 11.28813f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 11.78580f, 11.87629f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 12.05726f, 12.46445f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 12.69066f, 12.73590f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 13.05261f, 13.09785f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 11.28813f, 11.33337f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 11.42386f, 11.51434f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 12.50969f, 12.60018f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 9.84035f, 9.88560f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 9.97608f, 10.02132f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 10.74521f, 10.97143f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 11.69532f, 11.78580f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 11.87629f, 12.05726f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 12.46445f, 12.50969f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 12.60018f, 12.69066f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
Zone(TEXT("People, balconies"), 13.36931f, 13.77649f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 14.00271f, 14.40990f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 14.77184f, 14.81708f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 15.08854f, 15.63146f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 15.99340f, 16.26486f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 16.53632f, 16.76253f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 13.77649f, 14.00271f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 14.50038f, 14.54562f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 14.68135f, 14.77184f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 16.76253f, 16.80777f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 13.32406f, 13.36931f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 15.81243f, 15.99340f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 17.03399f, 17.26020f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 17.66739f, 17.93885f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 18.43652f, 18.70798f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 19.06992f, 19.34138f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 19.88430f, 20.11051f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 20.51770f, 20.78916f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 21.33207f, 21.55829f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 21.96548f, 22.23693f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 22.77985f, 23.00607f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 23.41325f, 23.68471f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 24.22763f, 24.45384f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 24.86103f, 25.13249f);
	Site(TEXT("PplBalc1"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 16.98875f, 17.03399f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 17.62215f, 17.66739f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 18.21031f, 18.25555f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 19.52235f, 19.56760f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 23.27752f, 23.32277f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 24.18238f, 24.22763f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 24.81579f, 24.86103f);
	Site(TEXT("PplBalc1B"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2B"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 16.80777f, 16.98875f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 18.25555f, 18.43652f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 19.65808f, 19.88430f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 20.97013f, 21.01537f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 21.10586f, 21.33207f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 22.41791f, 22.46315f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 22.50839f, 22.77985f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 23.36801f, 23.41325f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 23.95617f, 24.18238f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);
Zone(TEXT("People, balconies"), 24.72530f, 24.77054f);
	Site(TEXT("PplBalc1C"), 1, 27.5f, 4, 2, 1.f);
	Site(TEXT("PplBalc2C"), 1, 27.5f, 4, 1, 1.f);

{
// ---- BEGIN far (ft_people_bay_far.py) ----
// Far life and creatures: everything living from 150 m to the horizon on the bay side (the beach falls away from the
// road to the sea 340 m out, so all of it is in view over the barrier), and gulls over the town's roofs. All six
// meshes are drawn big and placed larger still (the scale in each rule), and saved with the always-moving master:
// from the road 1 m at 200 m is 5 pixels of a 1920 frame. All but the dolphins are TALL: the flock's nearest bird
// is 26 m from the centre line (65 m less its ring, 39 m at the largest), the kites and the crowd stand 70 m out and more.
// The travelling pieces (pelicans, dolphins, surf) run along their own X, so they are turned by a few degrees only.
#define FARP(n) TEXT("/Game/People/Bayshore/SM_Far" #n)
Zone(TEXT("People far: the Strip"), 0.f, 3.2f);
	Scatter(FARP(Flock),    66, 160, 11, 1, 1.f, 1.4f,  0.15f, -1, 180.f);
	Scatter(FARP(Crowd),   110, 180,  9, 3, 1.f, 1.4f,  0.1f,  -1, 8.f);
	Scatter(FARP(Kites),    70, 180, 16, 6, 1.f, 1.3f,  0.1f,  -1, 10.f);
	Afloat(FARP(Surf),     380, 390, 16, 2, 1.f, 1.5f,  0.f,   -1, 6.f);
	Afloat(FARP(Dolphins), 400, 460, 30, 8, 1.f, 1.3f,  0.1f,  -1, 8.f);
	Afloat(FARP(Pelicans), 350, 420, 32, 13, 1.f, 1.5f, 0.1f,  -1, 8.f);
Zone(TEXT("People far: the Marina"), 3.2f, 6.4f);          // quieter sand: birds over the boats, dolphins off the jetties
	Scatter(FARP(Flock),    66, 160,  9, 2, 1.f, 1.4f,  0.15f, -1, 180.f);
	Afloat(FARP(Pelicans), 350, 430, 16, 5, 1.f, 1.5f,  0.1f,  -1, 8.f);
	Afloat(FARP(Dolphins), 400, 470, 15, 9, 1.f, 1.3f,  0.1f,  -1, 8.f);
	Scatter(FARP(Crowd),   110, 180, 26, 12, 1.f, 1.4f, 0.1f,  -1, 8.f);
	Scatter(FARP(Kites),    70, 180, 34, 20, 1.f, 1.3f, 0.1f,  -1, 10.f);
Zone(TEXT("People far: Pleasure Beach"), 6.4f, 9.8f);      // the beach at its busiest
	Scatter(FARP(Crowd),   110, 180,  7, 1, 1.f, 1.4f,  0.1f,  -1, 8.f);
	Scatter(FARP(Kites),    70, 180, 11, 4, 1.f, 1.3f,  0.1f,  -1, 10.f);
	Afloat(FARP(Surf),     380, 390, 13, 6, 1.f, 1.5f,  0.f,   -1, 6.f);
	Scatter(FARP(Flock),    66, 160, 17, 9, 1.f, 1.4f,  0.15f, -1, 180.f);
	Afloat(FARP(Dolphins), 400, 460, 38, 20, 1.f, 1.3f, 0.1f,  -1, 8.f);
	Afloat(FARP(Pelicans), 350, 420, 38, 30, 1.f, 1.5f, 0.1f,  -1, 8.f);
Zone(TEXT("People far: Hotel Row"), 9.8f, 13.2f);          // surfers; gulls over the hotels' roofs
	Afloat(FARP(Surf),     380, 390, 11, 3, 1.f, 1.5f,  0.f,   -1, 6.f);
	Scatter(FARP(Crowd),   110, 180, 15, 7, 1.f, 1.4f,  0.1f,  -1, 8.f);
	Scatter(FARP(Flock),    66, 160, 15, 11, 1.f, 1.4f, 0.15f, -1, 180.f);
	Scatter(FARP(Kites),    70, 180, 22, 14, 1.f, 1.3f, 0.1f,  -1, 10.f);
	Afloat(FARP(Pelicans), 350, 420, 28, 9, 1.f, 1.5f,  0.1f,  -1, 8.f);
	Afloat(FARP(Dolphins), 400, 460, 28, 23, 1.f, 1.3f, 0.1f,  -1, 8.f);
	Far(FARP(Flock),       140, 260, 14, 5, 1.f, 1.6f, 0.1f,   1, -25.f, 180.f);
Zone(TEXT("People far: the Point"), 13.2f, 16.8f);         // the town thins out: birds and dolphins
	Scatter(FARP(Flock),    66, 160,  8, 0, 1.f, 1.4f,  0.15f, -1, 180.f);
	Afloat(FARP(Dolphins), 400, 470, 11, 4, 1.f, 1.3f,  0.1f,  -1, 8.f);
	Afloat(FARP(Pelicans), 350, 430, 13, 7, 1.f, 1.5f,  0.1f,  -1, 8.f);
	Afloat(FARP(Surf),     380, 390, 19, 10, 1.f, 1.5f, 0.f,   -1, 6.f);
	Scatter(FARP(Kites),    70, 180, 30, 16, 1.f, 1.3f, 0.1f,  -1, 10.f);
	Scatter(FARP(Crowd),   110, 180, 36, 25, 1.f, 1.4f, 0.1f,  -1, 8.f);
Zone(TEXT("People far: Downtown waterfront"), 16.8f, 99.f); // everything at once
	Scatter(FARP(Crowd),   110, 180, 11, 2, 1.f, 1.4f,  0.1f,  -1, 8.f);
	Scatter(FARP(Flock),    66, 160, 13, 5, 1.f, 1.4f,  0.15f, -1, 180.f);
	Scatter(FARP(Kites),    70, 180, 19, 8, 1.f, 1.3f,  0.1f,  -1, 10.f);
	Afloat(FARP(Surf),     380, 390, 17, 11, 1.f, 1.5f, 0.f,   -1, 6.f);
	Afloat(FARP(Dolphins), 400, 460, 23, 14, 1.f, 1.3f, 0.1f,  -1, 8.f);
	Afloat(FARP(Pelicans), 350, 420, 23, 3, 1.f, 1.5f,  0.1f,  -1, 8.f);
	Far(FARP(Flock),       140, 260, 20, 9, 1.f, 1.6f, 0.1f,   1, -25.f, 180.f);
#undef FARP
// ---- END far ----
}

{
// ---- BEGIN watch (ft_people_bay_watch.py) ----
// The watchers: the town out to see the chase, on stands and platforms of its own. All six meshes are TALL (7.7 to 13 m):
// 28 m and more from the centre line. Slots are named as s % every == residue in the notes (the table's word is a phase).
// Bay side: a grandstand stands 30 m out, right behind the boardwalk, 15 m along from the slot's middle (the boardwalk's own
// canopy covers the middle 14 m), in slots that have no arcade (33.5 m) and no boardwalk shops (32 m); a scaffold platform
// stands 19 m along, at the end of an arcade's slot. Town side: the hotel terrace bridges the mouth of the side street
// (s % 4 == 0, no shop row there), 38 m out: behind the promenade (26 to 34 m, its clipped trees at 32.9 m; at 34 m the
// trees stood inside the terrace) and behind where the film crew sets up; a platform beside the other side streets, on
// the promenade's paving, 19 m along from the slot's middle (clear of the street and of the trees at 6.2 and 11.8 m).
Set(TEXT("PplWatchStandA"), { { TEXT("/Game/People/Bayshore/SM_WatchStand_A"), 15, 0, 0, 0, 1 } });
Set(TEXT("PplWatchStandB"), { { TEXT("/Game/People/Bayshore/SM_WatchStand_B"), 15, 0, 0, 0, 1 } });
Set(TEXT("PplWatchDeck"), { { TEXT("/Game/People/Bayshore/SM_WatchDeck"), -16, 0, 0, 0, 1 } });
Set(TEXT("PplWatchTowerA"), { { TEXT("/Game/People/Bayshore/SM_WatchTower_A"), 19, 0, 0, 0, 1 } });
Set(TEXT("PplWatchTowerB"), { { TEXT("/Game/People/Bayshore/SM_WatchTower_B"), -19, 0, 0, 0, 1 } });
Set(TEXT("PplWatchTerrace"), { { TEXT("/Game/People/Bayshore/SM_WatchTerrace"), 0, 0, 0, 0, 1 } });
Zone(TEXT("People watch: the Strip"), 0.f, 3.2f);
	Site(TEXT("PplWatchStandA"), -1, 30.f, 8, 7, 1.f);   // s % 8 == 1: bay (arcade s%4==2, shops s%4==0, the main cast's sun deck s%8==7)
	Site(TEXT("PplWatchStandB"), -1, 30.f, 8, 3, 1.f);   // s % 8 == 5
	Site(TEXT("PplWatchDeck"), -1, 30.5f, 16, 13, 1.f);   // s % 16 == 3: bay, before a beach club
	Site(TEXT("PplWatchTowerB"), -1, 28.f, 16, 10, 1.f);   // s % 16 == 6: bay, the end of an arcade's slot
	Site(TEXT("PplWatchTerrace"), 1, 38.f, 8, 0, 1.f);   // s % 8 == 0: town, over a side street, behind the promenade
	Site(TEXT("PplWatchTowerA"), 1, 31.f, 8, 4, 1.f);   // s % 8 == 4: town, the other side streets
Zone(TEXT("People watch: the Marina"), 3.2f, 6.4f);
	Site(TEXT("PplWatchStandA"), -1, 30.f, 8, 0, 1.f);   // s % 8 == 0: bay (arcade s%4==2, shops s%6==3)
	Site(TEXT("PplWatchStandB"), -1, 30.f, 8, 4, 1.f);   // s % 8 == 4
	Site(TEXT("PplWatchTowerB"), -1, 28.f, 16, 10, 1.f);   // s % 16 == 6
	Site(TEXT("PplWatchTerrace"), 1, 38.f, 8, 0, 1.f);   // s % 8 == 0: town, over a side street, behind the promenade
	Site(TEXT("PplWatchTowerA"), 1, 31.f, 8, 4, 1.f);   // s % 8 == 4: town, the other side streets
Zone(TEXT("People watch: Pleasure Beach"), 6.4f, 9.8f);
	Site(TEXT("PplWatchStandA"), -1, 30.f, 8, 6, 1.f);   // s % 8 == 2: bay (arcades and shops in odd slots, the main cast's sun deck s%8==0)
	Site(TEXT("PplWatchStandB"), -1, 30.f, 8, 2, 1.f);   // s % 8 == 6
	Site(TEXT("PplWatchDeck"), -1, 30.5f, 16, 12, 1.f);   // s % 16 == 4
	Site(TEXT("PplWatchTowerA"), -1, 28.f, 16, 4, 1.f);   // s % 16 == 12
	Site(TEXT("PplWatchTerrace"), 1, 38.f, 8, 0, 1.f);   // s % 8 == 0: town, over a side street, behind the promenade
	Site(TEXT("PplWatchTowerA"), 1, 31.f, 8, 4, 1.f);   // s % 8 == 4: town, the other side streets
Zone(TEXT("People watch: Hotel Row"), 9.8f, 13.2f);
	Site(TEXT("PplWatchStandA"), -1, 30.f, 8, 6, 1.f);   // s % 8 == 2: bay (arcades and shops in odd slots, the main cast's sun deck s%8==0)
	Site(TEXT("PplWatchStandB"), -1, 30.f, 8, 2, 1.f);   // s % 8 == 6
	Site(TEXT("PplWatchDeck"), -1, 30.5f, 16, 12, 1.f);   // s % 16 == 4
	Site(TEXT("PplWatchTowerA"), -1, 28.f, 16, 4, 1.f);   // s % 16 == 12
	Site(TEXT("PplWatchTerrace"), 1, 38.f, 8, 0, 1.f);   // s % 8 == 0: town, over a side street, behind the promenade
	Site(TEXT("PplWatchTowerA"), 1, 31.f, 8, 4, 1.f);   // s % 8 == 4: town, the other side streets
Zone(TEXT("People watch: the Point"), 13.2f, 16.8f);     // quiet: a stand now and then, platforms by the villas
	Site(TEXT("PplWatchStandA"), -1, 30.f, 16, 14, 1.f);   // s % 16 == 2
	Site(TEXT("PplWatchTowerA"), -1, 28.f, 8, 3, 1.f);   // s % 8 == 5
	Site(TEXT("PplWatchTowerB"), 1, 31.f, 8, 7, 1.f);   // s % 8 == 1: town, on the promenade
Zone(TEXT("People watch: Downtown waterfront"), 16.8f, 99.f);
	Site(TEXT("PplWatchStandA"), -1, 30.f, 8, 7, 1.f);   // s % 8 == 1: bay (arcade s%4==2, shops s%4==0, the main cast's sun deck s%8==7)
	Site(TEXT("PplWatchStandB"), -1, 30.f, 8, 3, 1.f);   // s % 8 == 5
	Site(TEXT("PplWatchDeck"), -1, 30.5f, 16, 13, 1.f);   // s % 16 == 3: bay, before a beach club
	Site(TEXT("PplWatchTowerB"), -1, 28.f, 16, 10, 1.f);   // s % 16 == 6: bay, the end of an arcade's slot
	Site(TEXT("PplWatchTerrace"), 1, 38.f, 8, 0, 1.f);   // s % 8 == 0: town, over a side street, behind the promenade
	Site(TEXT("PplWatchTowerA"), 1, 31.f, 8, 4, 1.f);   // s % 8 == 4: town, the other side streets
// ---- END watch ----
}

{
// ---- BEGIN p3a (gaps, ft_people_bay_p3a.py) ----
// Bayshore, pass 3, builder p3a: the general cast in the places the first cast left empty. Thirteen meshes,
// /Game/People/Bayshore/SM_P3A_*. Slots are named as s % every == residue in the notes; the table's word is a phase,
// (s + phase) % every == 0. Estimated pieces (a level is about 557 slots; before the track leaves out loops,
// corkscrews and jumps): the walk's ends 218, the pavement 111, the side streets 119, the sand 82: about 530 tried,
// about 390 placed if the track leaves out a quarter. No mesh more than 56 times.
//
//   The walk's ends (bay side, LOW): WalkRest_A/B, WalkWork_A/B stand ON the boardwalk's deck (up 0.48 m, as the
//     first cast's strollers), 18.3 m along from the slot's middle, 21.3 m out (20.8 to 21.8 m). The dressing's beach
//     edge fills 18.6 to 22.2 m for 14.8 m either side of the middle, the banner masts stand at 19 m along and 20.5 m
//     out, the strollers' first lane is at 22.2 m, the crew on the walk is in the slot's middle. On the Point the
//     boardwalk stands in every third slot (s % 3 == 0): the rules there use those slots only.
//   The pavement (town side, LOW): Pave_A/B, 15 m along from the slot's middle (the film crew sets up in the middle,
//     out to 6 m), 21.3 m out (20.6 to 22.0 m): behind the kerb's cars and stalls (to 19.8 m), in front of the
//     shoppers (from 23.6 m). Never in a side-street slot (s % 4 == 0): the residues used are 1, 6, 11, 14 of 20.
//   The side streets (town side, 37 to 59 m): SideSt_A/B/C, turned 90 degrees so that the walkers go up and down the
//     street's two pavements (7.5 m either side of its middle; the carriageway and its kerbs reach 5.1 m), in the
//     side-street slots that have no hotel terrace over them (s % 8 == 4). Not on the Point (its streets are s % 6 == 3).
//   The sand (bay side, 37 to 48 m): Sand_A/B/C/D in the slots with nothing tall in front of them: the Marina's
//     s % 6 == 5, the Point's even slots (not s % 16 == 2, a grandstand), downtown's s % 16 == 7, 11 and 15.
#define P3A(n) TEXT("/Game/People/Bayshore/SM_P3A_" #n)
Set(TEXT("PplP3ARestA1"), { { P3A(WalkRest_A),  18.3f, 0.f, 0.48f, 0.f, 1.f } });
Set(TEXT("PplP3ARestA2"), { { P3A(WalkRest_A), -18.3f, 0.f, 0.48f, 0.f, 1.f } });
Set(TEXT("PplP3ARestB1"), { { P3A(WalkRest_B),  18.3f, 0.f, 0.48f, 0.f, 1.f } });
Set(TEXT("PplP3ARestB2"), { { P3A(WalkRest_B), -18.3f, 0.f, 0.48f, 0.f, 1.f } });
Set(TEXT("PplP3AWorkA1"), { { P3A(WalkWork_A),  18.3f, 0.f, 0.48f, 0.f, 1.f } });
Set(TEXT("PplP3AWorkA2"), { { P3A(WalkWork_A), -18.3f, 0.f, 0.48f, 0.f, 1.f } });
Set(TEXT("PplP3AWorkB1"), { { P3A(WalkWork_B),  18.3f, 0.f, 0.48f, 0.f, 1.f } });
Set(TEXT("PplP3AWorkB2"), { { P3A(WalkWork_B), -18.3f, 0.f, 0.48f, 0.f, 1.f } });
Set(TEXT("PplP3APaveA1"), { { P3A(Pave_A),  15.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3APaveA2"), { { P3A(Pave_A), -15.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3APaveB1"), { { P3A(Pave_B),  15.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3APaveB2"), { { P3A(Pave_B), -15.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("PplP3AStreetAB"), {
	{ P3A(SideSt_A),  7.5f, 0.f, 0.f,  90.f, 1.f },
	{ P3A(SideSt_B), -7.5f, 0.f, 0.f, -90.f, 1.f },
});
Set(TEXT("PplP3AStreetBC"), {
	{ P3A(SideSt_B),  7.5f, 0.f, 0.f,  90.f, 1.f },
	{ P3A(SideSt_C), -7.5f, 0.f, 0.f, -90.f, 1.f },
});
Set(TEXT("PplP3AStreetCA"), {
	{ P3A(SideSt_C),  7.5f, 0.f, 0.f,  90.f, 1.f },
	{ P3A(SideSt_A), -7.5f, 0.f, 0.f, -90.f, 1.f },
});

// ---- 0 to 13.2 km: the Strip, the Marina, Pleasure Beach, Hotel Row (slots 1 to 291). The boardwalk is unbroken.
Zone(TEXT("People p3a: the walk and the streets, Strip to Hotel Row"), 0.f, 13.2f);
	Site(TEXT("PplP3ARestA1"), -1, 21.3f, 20, 0, 0.f);     // s % 20 == 0   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3ARestB2"), -1, 21.3f, 20, 18, 0.f);    // s % 20 == 2   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3AWorkA1"), -1, 21.3f, 20, 15, 0.f);    // s % 20 == 5   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3AWorkB2"), -1, 21.3f, 20, 13, 0.f);    // s % 20 == 7   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3ARestB1"), -1, 21.3f, 20, 10, 0.f);    // s % 20 == 10   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3ARestA2"), -1, 21.3f, 20, 8, 0.f);     // s % 20 == 12   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3AWorkB1"), -1, 21.3f, 20, 5, 0.f);     // s % 20 == 15   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3AWorkA2"), -1, 21.3f, 20, 3, 0.f);     // s % 20 == 17   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3AStreetAB"), 1, 48.f, 24, 20, 1.f);    // s % 24 == 4
	Site(TEXT("PplP3AStreetBC"), 1, 48.f, 24, 12, 1.f);    // s % 24 == 12
	Site(TEXT("PplP3AStreetCA"), 1, 48.f, 24, 4, 1.f);     // s % 24 == 20
// ---- 13.2 to 16.8 km: the Point (slots 292 to 371). The boardwalk stands in s % 3 == 0 only; open sand between.
Zone(TEXT("People p3a: the Point"), 13.2f, 16.8f);
	Site(TEXT("PplP3ARestA1"), -1, 21.3f, 12, 0, 0.f);     // s % 12 == 0   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3AWorkA2"), -1, 21.3f, 12, 9, 0.f);     // s % 12 == 3   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3ARestB1"), -1, 21.3f, 12, 6, 0.f);     // s % 12 == 6   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3AWorkB2"), -1, 21.3f, 12, 3, 0.f);     // s % 12 == 9   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(P3A(Sand_A), 38.f, 48.f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);     // s % 8 == 0
	Scatter(P3A(Sand_D), 38.f, 48.f, 8, 2, 1.f, 1.f, 0.f, -1, 0.f);     // s % 8 == 6
	Scatter(P3A(Sand_C), 38.f, 48.f, 16, 12, 1.f, 1.f, 0.f, -1, 0.f);   // s % 16 == 4
	Scatter(P3A(Sand_B), 38.f, 48.f, 16, 4, 1.f, 1.f, 0.f, -1, 0.f);    // s % 16 == 12
	Scatter(P3A(Sand_B), 38.f, 48.f, 16, 6, 1.f, 1.f, 0.f, -1, 0.f);    // s % 16 == 10
// ---- 16.8 km to the end: downtown's waterfront (slots 372 on).
Zone(TEXT("People p3a: the walk, the streets and the sand, Downtown waterfront"), 16.8f, 99.f);
	Site(TEXT("PplP3ARestA1"), -1, 21.3f, 20, 0, 0.f);     // s % 20 == 0   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3ARestB2"), -1, 21.3f, 20, 18, 0.f);    // s % 20 == 2   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3AWorkA1"), -1, 21.3f, 20, 15, 0.f);    // s % 20 == 5   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3AWorkB2"), -1, 21.3f, 20, 13, 0.f);    // s % 20 == 7   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3ARestB1"), -1, 21.3f, 20, 10, 0.f);    // s % 20 == 10   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3ARestA2"), -1, 21.3f, 20, 8, 0.f);     // s % 20 == 12   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3AWorkB1"), -1, 21.3f, 20, 5, 0.f);     // s % 20 == 15   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3AWorkA2"), -1, 21.3f, 20, 3, 0.f);     // s % 20 == 17   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3AStreetAB"), 1, 48.f, 24, 20, 1.f);    // s % 24 == 4
	Site(TEXT("PplP3AStreetBC"), 1, 48.f, 24, 12, 1.f);    // s % 24 == 12
	Site(TEXT("PplP3AStreetCA"), 1, 48.f, 24, 4, 1.f);     // s % 24 == 20
	Scatter(P3A(Sand_A), 39.f, 42.f, 32, 25, 1.f, 1.f, 0.f, -1, 0.f);   // s % 32 == 7
	Scatter(P3A(Sand_C), 39.f, 42.f, 32, 9, 1.f, 1.f, 0.f, -1, 0.f);    // s % 32 == 23
	Scatter(P3A(Sand_B), 39.f, 42.f, 32, 17, 1.f, 1.f, 0.f, -1, 0.f);   // s % 32 == 15
	Scatter(P3A(Sand_D), 39.f, 42.f, 32, 1, 1.f, 1.f, 0.f, -1, 0.f);    // s % 32 == 31
	Scatter(P3A(Sand_A), 39.f, 42.f, 32, 21, 1.f, 1.f, 0.f, -1, 0.f);   // s % 32 == 11
	Scatter(P3A(Sand_D), 39.f, 42.f, 32, 5, 1.f, 1.f, 0.f, -1, 0.f);    // s % 32 == 27
// ---- 3.2 to 6.4 km: the Marina's sand (slots 71 to 141): s % 6 == 5 has no bar, no boardwalk shop, no arcade, no stand.
Zone(TEXT("People p3a: the Marina's sand"), 3.2f, 6.4f);
	Scatter(P3A(Sand_A), 37.f, 45.f, 24, 19, 1.f, 1.f, 0.f, -1, 0.f);   // s % 24 == 5
	Scatter(P3A(Sand_B), 37.f, 45.f, 24, 13, 1.f, 1.f, 0.f, -1, 0.f);   // s % 24 == 11
	Scatter(P3A(Sand_C), 37.f, 45.f, 24, 7, 1.f, 1.f, 0.f, -1, 0.f);    // s % 24 == 17
	Scatter(P3A(Sand_D), 37.f, 45.f, 24, 1, 1.f, 1.f, 0.f, -1, 0.f);    // s % 24 == 23
// ---- The whole road: the town's pavement (never s % 4 == 0, the side streets).
Zone(TEXT("People p3a: the town's pavement"), 0.f, 99.f);
	Site(TEXT("PplP3APaveA1"), 1, 21.3f, 20, 19, 0.f);     // s % 20 == 1   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3APaveB1"), 1, 21.3f, 20, 14, 0.f);     // s % 20 == 6   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3APaveA2"), 1, 21.3f, 20, 9, 0.f);      // s % 20 == 11   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3APaveB2"), 1, 21.3f, 20, 6, 0.f);      // s % 20 == 14   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
#undef P3A
// ---- END p3a ----
}

{
// ---- BEGIN p3b (scenes, ft_people_bay_p3b.py) ----
// Story groups that could only be at Bayshore: one mesh a scene, 8 to 14 figures and the props that tell it. All twelve are TALL
// (tops 3.15 to 7.0 m as built): the nearest edge of any of them is 26.7 m from the centre line.
//   the beach (bay side): Wedding, SandCastle, SurfBoat (the lifeguards' drill), FishMarket, Luau, SurfContest, Yoga, TugOfWar.
//     Each is at most 13.9 m along the road and 8.3 m deep, and stands 42.5 m out on the sand (38.3 to 46.7 m: behind the
//     arcade's deck, which ends 38.3 m out), 8 m back along the slot (the watchers' grandstands stand 15 m on, 30 m out: the
//     scene shows beside one, not behind it). Only on level and gently sloping ground (the slots of the dressing's Street(...)
//     lines whose rows stand at 0 and 0.45 m: the scenes stand on the sand without a footing); on the steeper terraces (rows
//     at 0.95 m) only the yoga class, whose deck has a footing.
//   the town (town side): Carnival_A, Carnival_B, StreetShow, Mural, each on a stage of its own 12.4 x 5.6 m (0.6 to 0.8 m high,
//     its footing 2.2 m down), 29.5 m out (26.7 to 32.3 m) on the corner of a side street: slots s%4==0, 15 m back along the
//     slot (the street itself is 5.1 m either way; the front rows of the next slots end 20 m either way; the watchers' platform
//     stands 19 m ON in s%8==4, their terrace 38 m out over the street's mouth in s%8==0). In s%8==0 a second scene stands on
//     the other corner, 15 m on. Left out of the slots where the film crew's units (PplShootA/B/C) stand, and of the first eight.
// Slots are named as s % 16 == residue in the notes; the table's word is a phase ((s + phase) % 16 == 0). Every rule is certain
// (chance 1). Written by C:/ftwork/jobs/L0p3b/make_inl.py from the Street(...) lines of FTDressL0.inl.
Zone(TEXT("People p3b, scenes"), 0.f, 0.f);       // (the block starts with a Zone; it has no rules: every rule is in a district below)
#define P3B(n) TEXT("/Game/People/Bayshore/SM_P3B_" n)
Set(TEXT("PplP3BWeddingL"), { { P3B("Wedding"), -8, 0, 0, 0, 1 } });
Set(TEXT("PplP3BSandCastleL"), { { P3B("SandCastle"), -8, 0, 0, 0, 1 } });
Set(TEXT("PplP3BSandCastleR"), { { P3B("SandCastle"), 9, 0, 0, 0, 1 } });
Set(TEXT("PplP3BSurfBoatL"), { { P3B("SurfBoat"), -8, 0, 0, 0, 1 } });
Set(TEXT("PplP3BFishMarketL"), { { P3B("FishMarket"), -8, 0, 0, 0, 1 } });
Set(TEXT("PplP3BFishMarketR"), { { P3B("FishMarket"), 9, 0, 0, 0, 1 } });
Set(TEXT("PplP3BLuauL"), { { P3B("Luau"), -8, 0, 0, 0, 1 } });
Set(TEXT("PplP3BSurfContestL"), { { P3B("SurfContest"), -8, 0, 0, 0, 1 } });
Set(TEXT("PplP3BSurfContestR"), { { P3B("SurfContest"), 9, 0, 0, 0, 1 } });
Set(TEXT("PplP3BYogaL"), { { P3B("Yoga"), -8, 0, 0, 0, 1 } });
Set(TEXT("PplP3BYogaR"), { { P3B("Yoga"), 9, 0, 0, 0, 1 } });
Set(TEXT("PplP3BTugOfWarL"), { { P3B("TugOfWar"), -8, 0, 0, 0, 1 } });
Set(TEXT("PplP3BTugOfWarR"), { { P3B("TugOfWar"), 9, 0, 0, 0, 1 } });
Set(TEXT("PplP3BCarnivalAL"), { { P3B("Carnival_A"), -15, 0, 0, 0, 1 } });
Set(TEXT("PplP3BCarnivalAR"), { { P3B("Carnival_A"), 15, 0, 0, 0, 1 } });
Set(TEXT("PplP3BStreetShowL"), { { P3B("StreetShow"), -15, 0, 0, 0, 1 } });
Set(TEXT("PplP3BStreetShowR"), { { P3B("StreetShow"), 15, 0, 0, 0, 1 } });
Set(TEXT("PplP3BCarnivalBL"), { { P3B("Carnival_B"), -15, 0, 0, 0, 1 } });
Set(TEXT("PplP3BCarnivalBR"), { { P3B("Carnival_B"), 15, 0, 0, 0, 1 } });
Set(TEXT("PplP3BMuralL"), { { P3B("Mural"), -15, 0, 0, 0, 1 } });
Set(TEXT("PplP3BMuralR"), { { P3B("Mural"), 15, 0, 0, 0, 1 } });
auto P3BKm = [](float S) { return S * 0.045243f; };                // a slot is 45.243 m (the dressing's own SlotKm)
// ---- the Strip (slots 1 to 70). Bay: odd slots (the arcade is in s%4==2, the walk shops in s%4==0); s%16==3 has the watchers' deck 16 m back along the slot, so the scene stands 9 m on.
auto P3BStripBay = [&](float A, float B)
{
	Zone(TEXT("People p3b, the Strip: the beach"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BWeddingL"), -1, 42.5f, 16, 15, 1.f);   // s % 16 == 1
	Site(TEXT("PplP3BSandCastleR"), -1, 42.5f, 16, 13, 1.f);   // s % 16 == 3
	Site(TEXT("PplP3BSurfBoatL"), -1, 42.5f, 16, 11, 1.f);   // s % 16 == 5
	Site(TEXT("PplP3BFishMarketL"), -1, 42.5f, 16, 9, 1.f);   // s % 16 == 7
	Site(TEXT("PplP3BLuauL"), -1, 42.5f, 16, 7, 1.f);   // s % 16 == 9
	Site(TEXT("PplP3BSurfContestL"), -1, 42.5f, 16, 5, 1.f);   // s % 16 == 11
	Site(TEXT("PplP3BYogaL"), -1, 42.5f, 16, 3, 1.f);   // s % 16 == 13
	Site(TEXT("PplP3BTugOfWarL"), -1, 42.5f, 16, 1, 1.f);   // s % 16 == 15
};
auto P3BStripTerrace = [&](float A, float B)      // steeper ground: only the yoga deck
{
	Zone(TEXT("People p3b, the Strip: the beach, terraces"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BYogaR"), -1, 42.5f, 16, 13, 1.f);   // s % 16 == 3
	Site(TEXT("PplP3BYogaL"), -1, 42.5f, 16, 5, 1.f);   // s % 16 == 11
};
auto P3BStripTown = [&](float A, float B)
{
	Zone(TEXT("People p3b, the Strip: street corners"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BCarnivalAL"), 1, 29.5f, 16, 0, 1.f);   // s % 16 == 0
	Site(TEXT("PplP3BCarnivalBR"), 1, 29.5f, 16, 0, 1.f);   // s % 16 == 0: the other corner
	Site(TEXT("PplP3BStreetShowL"), 1, 29.5f, 16, 12, 1.f);   // s % 16 == 4
	Site(TEXT("PplP3BCarnivalBL"), 1, 29.5f, 16, 8, 1.f);   // s % 16 == 8
	Site(TEXT("PplP3BCarnivalAR"), 1, 29.5f, 16, 8, 1.f);   // s % 16 == 8: the other corner
	Site(TEXT("PplP3BMuralL"), 1, 29.5f, 16, 4, 1.f);   // s % 16 == 12
};
// ---- the Marina (slots 71 to 141). Bay: not s%4==2 (the arcade) and not s%8==4 (the first cast's beach day, 40 to 46 m); the fish market three times in sixteen: this is the harbour.
auto P3BMarinaBay = [&](float A, float B)
{
	Zone(TEXT("People p3b, the Marina: the beach"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BFishMarketL"), -1, 42.5f, 16, 0, 1.f);   // s % 16 == 0
	Site(TEXT("PplP3BWeddingL"), -1, 42.5f, 16, 15, 1.f);   // s % 16 == 1
	Site(TEXT("PplP3BSurfBoatL"), -1, 42.5f, 16, 13, 1.f);   // s % 16 == 3
	Site(TEXT("PplP3BFishMarketL"), -1, 42.5f, 16, 11, 1.f);   // s % 16 == 5
	Site(TEXT("PplP3BYogaL"), -1, 42.5f, 16, 9, 1.f);   // s % 16 == 7
	Site(TEXT("PplP3BSandCastleL"), -1, 42.5f, 16, 8, 1.f);   // s % 16 == 8
	Site(TEXT("PplP3BFishMarketL"), -1, 42.5f, 16, 7, 1.f);   // s % 16 == 9
	Site(TEXT("PplP3BTugOfWarL"), -1, 42.5f, 16, 5, 1.f);   // s % 16 == 11
	Site(TEXT("PplP3BSurfContestL"), -1, 42.5f, 16, 3, 1.f);   // s % 16 == 13
	Site(TEXT("PplP3BLuauL"), -1, 42.5f, 16, 1, 1.f);   // s % 16 == 15
};
auto P3BMarinaTerrace = [&](float A, float B)      // steeper ground: only the yoga deck
{
	Zone(TEXT("People p3b, the Marina: the beach, terraces"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BYogaL"), -1, 42.5f, 16, 15, 1.f);   // s % 16 == 1
	Site(TEXT("PplP3BYogaL"), -1, 42.5f, 16, 7, 1.f);   // s % 16 == 9
};
auto P3BMarinaTown = [&](float A, float B)
{
	Zone(TEXT("People p3b, the Marina: street corners"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BStreetShowL"), 1, 29.5f, 16, 0, 1.f);   // s % 16 == 0
	Site(TEXT("PplP3BMuralR"), 1, 29.5f, 16, 0, 1.f);   // s % 16 == 0: the other corner
	Site(TEXT("PplP3BCarnivalBL"), 1, 29.5f, 16, 12, 1.f);   // s % 16 == 4
	Site(TEXT("PplP3BMuralL"), 1, 29.5f, 16, 8, 1.f);   // s % 16 == 8
	Site(TEXT("PplP3BStreetShowR"), 1, 29.5f, 16, 8, 1.f);   // s % 16 == 8: the other corner
	Site(TEXT("PplP3BCarnivalAL"), 1, 29.5f, 16, 4, 1.f);   // s % 16 == 12
};
// ---- Pleasure Beach (slots 142 to 216). Bay: even slots (arcades and walk shops are in the odd ones); s%16==4 has the watchers' deck, so the scene stands 9 m on.
auto P3BPleasureBay = [&](float A, float B)
{
	Zone(TEXT("People p3b, Pleasure Beach: the beach"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BFishMarketL"), -1, 42.5f, 16, 0, 1.f);   // s % 16 == 0
	Site(TEXT("PplP3BLuauL"), -1, 42.5f, 16, 14, 1.f);   // s % 16 == 2
	Site(TEXT("PplP3BSurfContestR"), -1, 42.5f, 16, 12, 1.f);   // s % 16 == 4
	Site(TEXT("PplP3BYogaL"), -1, 42.5f, 16, 10, 1.f);   // s % 16 == 6
	Site(TEXT("PplP3BTugOfWarL"), -1, 42.5f, 16, 8, 1.f);   // s % 16 == 8
	Site(TEXT("PplP3BWeddingL"), -1, 42.5f, 16, 6, 1.f);   // s % 16 == 10
	Site(TEXT("PplP3BSandCastleL"), -1, 42.5f, 16, 4, 1.f);   // s % 16 == 12
	Site(TEXT("PplP3BSurfBoatL"), -1, 42.5f, 16, 2, 1.f);   // s % 16 == 14
};
auto P3BPleasureTerrace = [&](float A, float B)      // steeper ground: only the yoga deck
{
	Zone(TEXT("People p3b, Pleasure Beach: the beach, terraces"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BYogaL"), -1, 42.5f, 16, 14, 1.f);   // s % 16 == 2
	Site(TEXT("PplP3BYogaL"), -1, 42.5f, 16, 6, 1.f);   // s % 16 == 10
};
auto P3BPleasureTown = [&](float A, float B)
{
	Zone(TEXT("People p3b, Pleasure Beach: street corners"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BCarnivalBL"), 1, 29.5f, 16, 0, 1.f);   // s % 16 == 0
	Site(TEXT("PplP3BCarnivalAR"), 1, 29.5f, 16, 0, 1.f);   // s % 16 == 0: the other corner
	Site(TEXT("PplP3BMuralL"), 1, 29.5f, 16, 12, 1.f);   // s % 16 == 4
	Site(TEXT("PplP3BCarnivalAL"), 1, 29.5f, 16, 8, 1.f);   // s % 16 == 8
	Site(TEXT("PplP3BCarnivalBR"), 1, 29.5f, 16, 8, 1.f);   // s % 16 == 8: the other corner
	Site(TEXT("PplP3BStreetShowL"), 1, 29.5f, 16, 4, 1.f);   // s % 16 == 12
};
// ---- Hotel Row (slots 217 to 291). Bay: even slots (arcades and walk shops are in the odd ones); s%16==4 has the watchers' deck.
auto P3BHotelBay = [&](float A, float B)
{
	Zone(TEXT("People p3b, Hotel Row: the beach"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BSurfContestL"), -1, 42.5f, 16, 0, 1.f);   // s % 16 == 0
	Site(TEXT("PplP3BYogaL"), -1, 42.5f, 16, 14, 1.f);   // s % 16 == 2
	Site(TEXT("PplP3BTugOfWarR"), -1, 42.5f, 16, 12, 1.f);   // s % 16 == 4
	Site(TEXT("PplP3BWeddingL"), -1, 42.5f, 16, 10, 1.f);   // s % 16 == 6
	Site(TEXT("PplP3BSandCastleL"), -1, 42.5f, 16, 8, 1.f);   // s % 16 == 8
	Site(TEXT("PplP3BSurfBoatL"), -1, 42.5f, 16, 6, 1.f);   // s % 16 == 10
	Site(TEXT("PplP3BFishMarketL"), -1, 42.5f, 16, 4, 1.f);   // s % 16 == 12
	Site(TEXT("PplP3BLuauL"), -1, 42.5f, 16, 2, 1.f);   // s % 16 == 14
};
auto P3BHotelTerrace = [&](float A, float B)      // steeper ground: only the yoga deck
{
	Zone(TEXT("People p3b, Hotel Row: the beach, terraces"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BYogaL"), -1, 42.5f, 16, 14, 1.f);   // s % 16 == 2
	Site(TEXT("PplP3BYogaL"), -1, 42.5f, 16, 6, 1.f);   // s % 16 == 10
};
auto P3BHotelTown = [&](float A, float B)
{
	Zone(TEXT("People p3b, Hotel Row: street corners"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BMuralL"), 1, 29.5f, 16, 0, 1.f);   // s % 16 == 0
	Site(TEXT("PplP3BStreetShowR"), 1, 29.5f, 16, 0, 1.f);   // s % 16 == 0: the other corner
	Site(TEXT("PplP3BCarnivalAL"), 1, 29.5f, 16, 12, 1.f);   // s % 16 == 4
	Site(TEXT("PplP3BStreetShowL"), 1, 29.5f, 16, 8, 1.f);   // s % 16 == 8
	Site(TEXT("PplP3BMuralR"), 1, 29.5f, 16, 8, 1.f);   // s % 16 == 8: the other corner
	Site(TEXT("PplP3BCarnivalBL"), 1, 29.5f, 16, 4, 1.f);   // s % 16 == 12
};
// ---- the Point (slots 292 to 371). Bay: not s%8==3 (the first cast's beach day, 34 to 48 m), not s%8==7 (its surfers, 40 to 56 m), not s%16==2 (the watchers' grandstand).
auto P3BPointBay = [&](float A, float B)
{
	Zone(TEXT("People p3b, the Point: the beach"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BYogaL"), -1, 42.5f, 16, 0, 1.f);   // s % 16 == 0
	Site(TEXT("PplP3BTugOfWarL"), -1, 42.5f, 16, 15, 1.f);   // s % 16 == 1
	Site(TEXT("PplP3BWeddingL"), -1, 42.5f, 16, 12, 1.f);   // s % 16 == 4
	Site(TEXT("PplP3BSandCastleL"), -1, 42.5f, 16, 11, 1.f);   // s % 16 == 5
	Site(TEXT("PplP3BSurfBoatL"), -1, 42.5f, 16, 8, 1.f);   // s % 16 == 8
	Site(TEXT("PplP3BFishMarketL"), -1, 42.5f, 16, 7, 1.f);   // s % 16 == 9
	Site(TEXT("PplP3BLuauL"), -1, 42.5f, 16, 4, 1.f);   // s % 16 == 12
	Site(TEXT("PplP3BSurfContestL"), -1, 42.5f, 16, 3, 1.f);   // s % 16 == 13
	Site(TEXT("PplP3BSurfBoatL"), -1, 42.5f, 16, 10, 1.f);   // s % 16 == 6
	Site(TEXT("PplP3BSandCastleL"), -1, 42.5f, 16, 6, 1.f);   // s % 16 == 10
	Site(TEXT("PplP3BLuauL"), -1, 42.5f, 16, 2, 1.f);   // s % 16 == 14
};
auto P3BPointTerrace = [&](float A, float B)      // steeper ground: only the yoga deck
{
	Zone(TEXT("People p3b, the Point: the beach, terraces"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BYogaL"), -1, 42.5f, 16, 15, 1.f);   // s % 16 == 1
	Site(TEXT("PplP3BYogaL"), -1, 42.5f, 16, 4, 1.f);   // s % 16 == 12
};
auto P3BPointTown = [&](float A, float B)
{
	Zone(TEXT("People p3b, the Point: street corners"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BCarnivalAL"), 1, 29.5f, 16, 0, 1.f);   // s % 16 == 0
	Site(TEXT("PplP3BCarnivalBR"), 1, 29.5f, 16, 0, 1.f);   // s % 16 == 0: the other corner
	Site(TEXT("PplP3BStreetShowL"), 1, 29.5f, 16, 12, 1.f);   // s % 16 == 4
	Site(TEXT("PplP3BCarnivalBL"), 1, 29.5f, 16, 8, 1.f);   // s % 16 == 8
	Site(TEXT("PplP3BCarnivalAR"), 1, 29.5f, 16, 8, 1.f);   // s % 16 == 8: the other corner
	Site(TEXT("PplP3BMuralL"), 1, 29.5f, 16, 4, 1.f);   // s % 16 == 12
};
// ---- Downtown waterfront (slots 372 to 555). Bay: as the Strip.
auto P3BDownBay = [&](float A, float B)
{
	Zone(TEXT("People p3b, Downtown waterfront: the beach"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BSurfBoatL"), -1, 42.5f, 16, 15, 1.f);   // s % 16 == 1
	Site(TEXT("PplP3BFishMarketR"), -1, 42.5f, 16, 13, 1.f);   // s % 16 == 3
	Site(TEXT("PplP3BLuauL"), -1, 42.5f, 16, 11, 1.f);   // s % 16 == 5
	Site(TEXT("PplP3BSurfContestL"), -1, 42.5f, 16, 9, 1.f);   // s % 16 == 7
	Site(TEXT("PplP3BYogaL"), -1, 42.5f, 16, 7, 1.f);   // s % 16 == 9
	Site(TEXT("PplP3BTugOfWarL"), -1, 42.5f, 16, 5, 1.f);   // s % 16 == 11
	Site(TEXT("PplP3BWeddingL"), -1, 42.5f, 16, 3, 1.f);   // s % 16 == 13
	Site(TEXT("PplP3BSandCastleL"), -1, 42.5f, 16, 1, 1.f);   // s % 16 == 15
};
auto P3BDownTerrace = [&](float A, float B)      // steeper ground: only the yoga deck
{
	Zone(TEXT("People p3b, Downtown waterfront: the beach, terraces"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BYogaR"), -1, 42.5f, 16, 13, 1.f);   // s % 16 == 3
	Site(TEXT("PplP3BYogaL"), -1, 42.5f, 16, 5, 1.f);   // s % 16 == 11
};
auto P3BDownTown = [&](float A, float B)
{
	Zone(TEXT("People p3b, Downtown waterfront: street corners"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BStreetShowL"), 1, 29.5f, 16, 0, 1.f);   // s % 16 == 0
	Site(TEXT("PplP3BMuralR"), 1, 29.5f, 16, 0, 1.f);   // s % 16 == 0: the other corner
	Site(TEXT("PplP3BCarnivalBL"), 1, 29.5f, 16, 12, 1.f);   // s % 16 == 4
	Site(TEXT("PplP3BMuralL"), 1, 29.5f, 16, 8, 1.f);   // s % 16 == 8
	Site(TEXT("PplP3BStreetShowR"), 1, 29.5f, 16, 8, 1.f);   // s % 16 == 8: the other corner
	Site(TEXT("PplP3BCarnivalAL"), 1, 29.5f, 16, 4, 1.f);   // s % 16 == 12
};
auto P3BDownLevel = [&](float A, float B)      // level ground only: the walk shops (s%4==0) stand where it is not quite level
{
	Zone(TEXT("People p3b, Downtown waterfront: the beach, level ground"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Site(TEXT("PplP3BSurfBoatL"), -1, 42.5f, 16, 0, 1.f);   // s % 16 == 0
	Site(TEXT("PplP3BWeddingL"), -1, 42.5f, 16, 12, 1.f);   // s % 16 == 4
	Site(TEXT("PplP3BSandCastleL"), -1, 42.5f, 16, 8, 1.f);   // s % 16 == 8
	Site(TEXT("PplP3BLuauL"), -1, 42.5f, 16, 4, 1.f);   // s % 16 == 12
};
// ---- where: the runs of slots (the same slot numbers as the Street(...) lines of FTDressL0.inl)
P3BStripBay(1, 3); P3BStripBay(5, 6); P3BStripBay(8, 11); P3BStripBay(14, 14); P3BStripBay(23, 27); P3BStripBay(30, 30); P3BStripBay(33, 33);
P3BStripBay(36, 36); P3BStripBay(40, 42); P3BStripBay(49, 54); P3BStripBay(57, 57); P3BStripBay(62, 63); P3BStripBay(67, 67); P3BStripTerrace(4, 4);
P3BStripTerrace(7, 7); P3BStripTerrace(12, 13); P3BStripTerrace(16, 17); P3BStripTerrace(19, 20); P3BStripTerrace(22, 22); P3BStripTerrace(28, 29);
P3BStripTerrace(31, 32); P3BStripTerrace(58, 61); P3BStripTerrace(70, 70); P3BStripTown(9, 14); P3BStripTown(16, 17); P3BStripTown(19, 20);
P3BStripTown(22, 33); P3BStripTown(41, 42); P3BStripTown(49, 54); P3BStripTown(57, 63); P3BStripTown(67, 67); P3BStripTown(70, 70);
P3BMarinaBay(74, 74); P3BMarinaBay(81, 88); P3BMarinaBay(90, 90); P3BMarinaBay(92, 93); P3BMarinaBay(101, 109); P3BMarinaBay(122, 137);
P3BMarinaBay(140, 141); P3BMarinaTerrace(71, 71); P3BMarinaTerrace(75, 75); P3BMarinaTerrace(80, 80); P3BMarinaTerrace(89, 89);
P3BMarinaTerrace(91, 91); P3BMarinaTerrace(110, 115); P3BMarinaTerrace(117, 117); P3BMarinaTerrace(138, 139); P3BMarinaTown(71, 71);
P3BMarinaTown(74, 75); P3BMarinaTown(80, 91); P3BMarinaTown(93, 93); P3BMarinaTown(101, 115); P3BMarinaTown(117, 117); P3BMarinaTown(122, 141);
P3BPleasureBay(142, 152); P3BPleasureBay(160, 160); P3BPleasureBay(169, 169); P3BPleasureBay(174, 174); P3BPleasureBay(176, 184);
P3BPleasureBay(187, 187); P3BPleasureBay(191, 193); P3BPleasureBay(199, 205); P3BPleasureBay(214, 216); P3BPleasureTerrace(161, 161);
P3BPleasureTerrace(164, 164); P3BPleasureTerrace(171, 173); P3BPleasureTerrace(175, 175); P3BPleasureTerrace(194, 198); P3BPleasureTerrace(209, 213);
P3BPleasureTown(142, 151); P3BPleasureTown(160, 161); P3BPleasureTown(164, 164); P3BPleasureTown(169, 169); P3BPleasureTown(171, 183);
P3BPleasureTown(187, 187); P3BPleasureTown(191, 205); P3BPleasureTown(209, 216); P3BHotelBay(217, 217); P3BHotelBay(222, 223); P3BHotelBay(228, 228);
P3BHotelBay(236, 237); P3BHotelBay(243, 250); P3BHotelBay(253, 254); P3BHotelBay(261, 262); P3BHotelBay(267, 275); P3BHotelBay(277, 278);
P3BHotelBay(281, 281); P3BHotelBay(289, 289); P3BHotelTerrace(218, 218); P3BHotelTerrace(221, 221); P3BHotelTerrace(238, 242);
P3BHotelTerrace(259, 260); P3BHotelTerrace(263, 266); P3BHotelTerrace(276, 276); P3BHotelTerrace(279, 280); P3BHotelTown(217, 218);
P3BHotelTown(221, 223); P3BHotelTown(236, 250); P3BHotelTown(253, 254); P3BHotelTown(259, 279); P3BHotelTown(281, 281); P3BHotelTown(289, 289);
P3BPointBay(296, 318); P3BPointBay(321, 321); P3BPointBay(325, 327); P3BPointBay(334, 345); P3BPointBay(354, 359); P3BPointBay(366, 371);
P3BPointTerrace(295, 295); P3BPointTerrace(350, 353); P3BPointTown(295, 318); P3BPointTown(321, 321); P3BPointTown(325, 327); P3BPointTown(334, 345);
P3BPointTown(350, 355); P3BPointTown(357, 359); P3BPointTown(366, 371); P3BDownLevel(377, 381); P3BDownLevel(391, 396); P3BDownLevel(408, 413);
P3BDownLevel(422, 427); P3BDownLevel(440, 444); P3BDownLevel(454, 459); P3BDownLevel(472, 476); P3BDownLevel(486, 491); P3BDownLevel(504, 508);
P3BDownLevel(518, 523); P3BDownLevel(536, 540); P3BDownLevel(550, 555); P3BDownBay(376, 381); P3BDownBay(390, 396); P3BDownBay(403, 403);
P3BDownBay(408, 413); P3BDownBay(422, 427); P3BDownBay(432, 432); P3BDownBay(440, 444); P3BDownBay(454, 459); P3BDownBay(472, 476);
P3BDownBay(486, 491); P3BDownBay(504, 508); P3BDownBay(515, 515); P3BDownBay(518, 523); P3BDownBay(535, 540); P3BDownBay(549, 555);
P3BDownTerrace(372, 375); P3BDownTerrace(404, 407); P3BDownTerrace(435, 439); P3BDownTerrace(464, 464); P3BDownTerrace(467, 471);
P3BDownTerrace(496, 496); P3BDownTerrace(498, 503); P3BDownTerrace(517, 517); P3BDownTerrace(530, 534); P3BDownTerrace(547, 547);
P3BDownTown(372, 379); P3BDownTown(381, 381); P3BDownTown(390, 396); P3BDownTown(403, 411); P3BDownTown(413, 413); P3BDownTown(422, 427);
P3BDownTown(432, 432); P3BDownTown(435, 443); P3BDownTown(454, 459); P3BDownTown(464, 464); P3BDownTown(467, 475); P3BDownTown(486, 491);
P3BDownTown(496, 496); P3BDownTown(498, 507); P3BDownTown(515, 515); P3BDownTown(517, 523); P3BDownTown(530, 531); P3BDownTown(533, 535);
P3BDownTown(537, 539); P3BDownTown(547, 547); P3BDownTown(549, 551); P3BDownTown(553, 555);
#undef P3B
// ---- END p3b ----
}

{
// ---- BEGIN p3c (movers, ft_people_bay_p3c.py) ----
// Everything that TRAVELS: each piece carries its own line along the road (the piece's X) or its own rings, and its rule never
// turns it towards the road (yaw jitter 0 by the road, 4 degrees on the lower beach, 3 on the water). Twelve meshes.
//   LOW (the log: top 1.77 to 1.92 m as built): SM_P3C_Wheels_A/B on the boardwalk's deck (a Set stands them 0.48 m up, the
//     deck; 21.7 m out: between the beach edge's windbreaks at 21.0 and the strollers' lanes from 22.2), in the slots the first
//     cast's strollers leave free (at the Point every boardwalk slot has strollers: the lanes differ); SM_P3C_Wheels_C/D on the
//     town's pavement (22.4 m: between the kerb cars and the shoppers under the awnings at 24.6), never in a side street's slot.
//     Their lines are 38 and 36 m: inside their own 45 m slot. SM_P3C_Ponies is low too (1.93 m) and stands 106 m out.
//   TALL (2.07 to 5.2 m), all 40 m out and more: SM_P3C_BeachPath_A/B along the top of the beach (40 m, a 44 m line),
//     SM_P3C_Circles (rings of up to 9 m about a point 57 to 63 m out: nothing of it nearer than 48 m), and on the lower beach
//     where the dressing found the land level (its ...Level(A, B) slots, runs of two and more): SM_P3C_Riders (98 to 102 m),
//     SM_P3C_Ponies (106 to 109 m), and in the inner slots of runs of five and more SM_P3C_SandTrain (114 to 119 m) and
//     SM_P3C_BeachRace (126 to 136 m): as placed their lines are 117, 78, 140 and 165 m.
//   Afloat: SM_P3C_Paddlers, 395 to 420 m out, past the surf line (a 240 m line, 384 m as placed).
// Slots are named as s % every == residue in the notes (the table's word is a phase).
Set(TEXT("PplP3CWalkA"), { { TEXT("/Game/People/Bayshore/SM_P3C_Wheels_A"), 0, 0, 0.48f, 0, 1 } });
Set(TEXT("PplP3CWalkB"), { { TEXT("/Game/People/Bayshore/SM_P3C_Wheels_B"), 0, 0, 0.48f, 0, 1 } });
Zone(TEXT("People p3c: the Strip"), 0.f, 3.2f);
	Site(TEXT("PplP3CWalkA"), -1, 21.7f, 12, 11, 0.f);   // s % 12 == 1: on the boardwalk   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3CWalkB"), -1, 21.7f, 12, 5, 0.f);   // s % 12 == 7   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Wheels_C"), 22.4f, 22.4f, 12, 10, 0.f, 1.f, 0.f, 1, 0.f);   // s % 12 == 2: the town's pavement (never a side street's slot)   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Wheels_D"), 22.4f, 22.4f, 12, 5, 0.f, 1.f, 0.f, 1, 0.f);   // s % 12 == 7   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachPath_A"), 40.f, 40.f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);   // s % 12 == 5: the top of the beach
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachPath_B"), 40.f, 40.f, 12, 3, 1.f, 1.f, 0.f, -1, 0.f);   // s % 12 == 9
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Circles"), 57.f, 63.f, 14, 11, 1.f, 1.f, 0.f, -1, 180.f);   // rings on the sand
	Afloat(TEXT("/Game/People/Bayshore/SM_P3C_Paddlers"), 395.f, 420.f, 20, 16, 1.f, 1.6f, 0.f, -1, 3.f);   // along the shore, past the surf line
Zone(TEXT("People p3c: the Marina"), 3.2f, 6.4f);
	Site(TEXT("PplP3CWalkA"), -1, 21.7f, 12, 11, 0.f);   // s % 12 == 1: on the boardwalk   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3CWalkB"), -1, 21.7f, 12, 5, 0.f);   // s % 12 == 7   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Wheels_C"), 22.4f, 22.4f, 12, 10, 0.f, 1.f, 0.f, 1, 0.f);   // s % 12 == 2: the town's pavement (never a side street's slot)   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Wheels_D"), 22.4f, 22.4f, 12, 5, 0.f, 1.f, 0.f, 1, 0.f);   // s % 12 == 7   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachPath_A"), 40.f, 40.f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);   // s % 12 == 5: the top of the beach
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachPath_B"), 40.f, 40.f, 12, 3, 1.f, 1.f, 0.f, -1, 0.f);   // s % 12 == 9
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Circles"), 57.f, 63.f, 14, 5, 1.f, 1.f, 0.f, -1, 180.f);   // rings on the sand
	Afloat(TEXT("/Game/People/Bayshore/SM_P3C_Paddlers"), 395.f, 420.f, 20, 9, 1.f, 1.6f, 0.f, -1, 3.f);   // along the shore, past the surf line
Zone(TEXT("People p3c: Pleasure Beach"), 6.4f, 9.8f);
	Site(TEXT("PplP3CWalkA"), -1, 21.7f, 12, 10, 0.f);   // s % 12 == 2: on the boardwalk   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3CWalkB"), -1, 21.7f, 12, 4, 0.f);   // s % 12 == 8   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Wheels_C"), 22.4f, 22.4f, 12, 10, 0.f, 1.f, 0.f, 1, 0.f);   // s % 12 == 2: the town's pavement (never a side street's slot)   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Wheels_D"), 22.4f, 22.4f, 12, 5, 0.f, 1.f, 0.f, 1, 0.f);   // s % 12 == 7   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachPath_A"), 40.f, 40.f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);   // s % 12 == 5: the top of the beach
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachPath_B"), 40.f, 40.f, 12, 3, 1.f, 1.f, 0.f, -1, 0.f);   // s % 12 == 9
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Circles"), 57.f, 63.f, 14, 9, 1.f, 1.f, 0.f, -1, 180.f);   // rings on the sand
	Afloat(TEXT("/Game/People/Bayshore/SM_P3C_Paddlers"), 395.f, 420.f, 20, 3, 1.f, 1.6f, 0.f, -1, 3.f);   // along the shore, past the surf line
Zone(TEXT("People p3c: Hotel Row"), 9.8f, 13.2f);
	Site(TEXT("PplP3CWalkA"), -1, 21.7f, 12, 10, 0.f);   // s % 12 == 2: on the boardwalk   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3CWalkB"), -1, 21.7f, 12, 4, 0.f);   // s % 12 == 8   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Wheels_C"), 22.4f, 22.4f, 12, 10, 0.f, 1.f, 0.f, 1, 0.f);   // s % 12 == 2: the town's pavement (never a side street's slot)   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Wheels_D"), 22.4f, 22.4f, 12, 5, 0.f, 1.f, 0.f, 1, 0.f);   // s % 12 == 7   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachPath_A"), 40.f, 40.f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);   // s % 12 == 5: the top of the beach
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachPath_B"), 40.f, 40.f, 12, 3, 1.f, 1.f, 0.f, -1, 0.f);   // s % 12 == 9
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Circles"), 57.f, 63.f, 14, 3, 1.f, 1.f, 0.f, -1, 180.f);   // rings on the sand
	Afloat(TEXT("/Game/People/Bayshore/SM_P3C_Paddlers"), 395.f, 420.f, 20, 18, 1.f, 1.6f, 0.f, -1, 3.f);   // along the shore, past the surf line
Zone(TEXT("People p3c: the Point"), 13.2f, 16.8f);
	Site(TEXT("PplP3CWalkA"), -1, 21.7f, 12, 0, 0.f);   // s % 12 == 0: on the boardwalk   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3CWalkB"), -1, 21.7f, 12, 6, 0.f);   // s % 12 == 6   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Wheels_C"), 22.4f, 22.4f, 12, 10, 0.f, 1.f, 0.f, 1, 0.f);   // s % 12 == 2: the town's pavement (never a side street's slot)   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Wheels_D"), 22.4f, 22.4f, 12, 5, 0.f, 1.f, 0.f, 1, 0.f);   // s % 12 == 7   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachPath_A"), 40.f, 40.f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);   // s % 12 == 5: the top of the beach
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachPath_B"), 40.f, 40.f, 12, 3, 1.f, 1.f, 0.f, -1, 0.f);   // s % 12 == 9
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Circles"), 57.f, 63.f, 14, 12, 1.f, 1.f, 0.f, -1, 180.f);   // rings on the sand
	Afloat(TEXT("/Game/People/Bayshore/SM_P3C_Paddlers"), 395.f, 420.f, 20, 12, 1.f, 1.6f, 0.f, -1, 3.f);   // along the shore, past the surf line
Zone(TEXT("People p3c: Downtown waterfront"), 16.8f, 99.f);
	Site(TEXT("PplP3CWalkA"), -1, 21.7f, 12, 11, 0.f);   // s % 12 == 1: on the boardwalk   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Site(TEXT("PplP3CWalkB"), -1, 21.7f, 12, 5, 0.f);   // s % 12 == 7   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Wheels_C"), 22.4f, 22.4f, 12, 10, 0.f, 1.f, 0.f, 1, 0.f);   // s % 12 == 2: the town's pavement (never a side street's slot)   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Wheels_D"), 22.4f, 22.4f, 12, 5, 0.f, 1.f, 0.f, 1, 0.f);   // s % 12 == 7   // fix round: off here (chance 0.f keeps the rule's place), placed again on level ground at the end of the table
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachPath_A"), 40.f, 40.f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);   // s % 12 == 5: the top of the beach
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachPath_B"), 40.f, 40.f, 12, 3, 1.f, 1.f, 0.f, -1, 0.f);   // s % 12 == 9
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Circles"), 57.f, 63.f, 14, 7, 1.f, 1.f, 0.f, -1, 180.f);   // rings on the sand
	Afloat(TEXT("/Game/People/Bayshore/SM_P3C_Paddlers"), 395.f, 420.f, 20, 6, 1.f, 1.6f, 0.f, -1, 3.f);   // along the shore, past the surf line
// ---- The lower beach, on the stretches the dressing found level (slots A to B of its ...Level lines).
Zone(TEXT("People p3c: level beach, slots 1 to 3"), 0.02262f, 0.15835f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 9 to 10"), 0.38457f, 0.47505f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 23 to 27"), 1.01797f, 1.24418f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 23 to 27"), 1.06321f, 1.19894f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 49 to 53"), 2.19429f, 2.42050f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 49 to 53"), 2.23953f, 2.37526f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 62 to 63"), 2.78244f, 2.87293f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 82 to 87"), 3.68730f, 3.95876f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 82 to 87"), 3.73255f, 3.91352f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 101 to 108"), 4.54692f, 4.90887f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 101 to 108"), 4.59216f, 4.86362f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 124 to 137"), 5.58751f, 6.22091f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 124 to 137"), 5.63275f, 6.17567f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 145 to 152"), 6.53761f, 6.89956f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 145 to 152"), 6.58286f, 6.85431f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 177 to 184"), 7.98539f, 8.34733f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 177 to 184"), 8.03063f, 8.30209f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 199 to 204"), 8.98074f, 9.25219f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 199 to 204"), 9.02598f, 9.20695f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 214 to 216"), 9.65938f, 9.79511f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 222 to 223"), 10.02132f, 10.11181f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 236 to 237"), 10.65473f, 10.74521f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 243 to 249"), 10.97143f, 11.28813f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 243 to 249"), 11.01667f, 11.24289f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 261 to 262"), 11.78580f, 11.87629f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 267 to 275"), 12.05726f, 12.46445f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 267 to 275"), 12.10250f, 12.41920f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 296 to 304"), 13.36931f, 13.77649f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 296 to 304"), 13.41455f, 13.73125f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 310 to 318"), 14.00271f, 14.40990f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 310 to 318"), 14.04795f, 14.36465f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 334 to 345"), 15.08854f, 15.63146f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 334 to 345"), 15.13378f, 15.58621f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 354 to 359"), 15.99340f, 16.26486f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 354 to 359"), 16.03864f, 16.21962f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 366 to 370"), 16.53632f, 16.76253f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 366 to 370"), 16.58156f, 16.71729f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 377 to 381"), 17.03399f, 17.26020f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 377 to 381"), 17.07923f, 17.21496f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 391 to 396"), 17.66739f, 17.93885f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 391 to 396"), 17.71263f, 17.89361f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 408 to 413"), 18.43652f, 18.70798f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 408 to 413"), 18.48177f, 18.66274f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 422 to 427"), 19.06992f, 19.34138f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 422 to 427"), 19.11517f, 19.29614f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 440 to 444"), 19.88430f, 20.11051f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 440 to 444"), 19.92954f, 20.06527f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 454 to 459"), 20.51770f, 20.78916f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 454 to 459"), 20.56294f, 20.74392f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 472 to 476"), 21.33207f, 21.55829f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 472 to 476"), 21.37732f, 21.51305f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 486 to 491"), 21.96548f, 22.23693f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 486 to 491"), 22.01072f, 22.19169f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 504 to 508"), 22.77985f, 23.00607f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 504 to 508"), 22.82509f, 22.96082f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 518 to 523"), 23.41325f, 23.68471f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 518 to 523"), 23.45850f, 23.63947f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 536 to 540"), 24.22763f, 24.45384f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 536 to 540"), 24.27287f, 24.40860f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, slots 550 to 555"), 24.86103f, 25.13249f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Riders"), 98.f, 102.f, 5, 4, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_Ponies"), 106.f, 109.f, 7, 4, 1.f, 1.3f, 0.f, -1, 4.f);
Zone(TEXT("People p3c: level beach, the inner slots of 550 to 555"), 24.90627f, 25.08724f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_SandTrain"), 114.f, 119.f, 4, 2, 1.f, 1.4f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/Bayshore/SM_P3C_BeachRace"), 126.f, 136.f, 5, 1, 1.f, 1.5f, 0.f, -1, 4.f);
// ---- END p3c ----
}

{
// ---- BEGIN p3d (heights, ft_people_bay_p3d.py) ----
// People UP HIGH, each piece with its own structure (roofs, a footbridge, towers, a billboard's cradle, a lift, a
// climbing wall, a rail of pedal cars, balconies). All eleven meshes are TALL (their log lines say so: tops 8.0 to 16.2 m):
// every rule's x min is 58 m or more, so nothing of this block is nearer than 58 m to the centre line. No Sets, no
// lights. Slots are named in the notes as s % every == residue (the table's phase is (every - residue) % every).
//   Bay side (-1), 64 to 74 m out, on the beach behind the boardwalk's stands: three (the Marina: four) slots in eight.
//   Town side (+1), 58 to 70 m out, between the palm belt's cafes and markets (40 to 54 m) and the shop rows (76 m on),
//   only in the slots where the dressing has neither a side street nor a pool, court or plaza at that distance:
//   the Strip and Downtown s % 4 == 3, the Marina and Pleasure Beach s % 4 == 1, Hotel Row s % 4 == 2, the Point
//   s % 6 == 1 and 5. The long pieces (FootBridge 30 m, SkyBikes 33 m) run along the road; their walkers and cars
//   travel the piece's own X and never leave it.
// Estimate (550 x fraction of road x chance / every, a rule): bay 197, town 133, about 330 placed before the track
// leaves out what stands beside its loops, jumps and corkscrews. The most of one mesh: RoofParty_A, about 39.
#define P3D(n) TEXT("/Game/People/Bayshore/SM_P3D_" n)
Zone(TEXT("People p3d, heights, the Strip"), 0.f, 3.2f);
	Scatter(P3D("SkyBikes"),     66.f, 72.f,  8,  6, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 8 == 2
	Scatter(P3D("JudgeTower"),   66.f, 72.f,  8,  2, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 8 == 6
	Scatter(P3D("ClimbWall"),    68.f, 72.f,  8,  1, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 8 == 7
	Scatter(P3D("RoofParty_A"),  60.f, 68.f, 16, 13, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 3
	Scatter(P3D("BalconyBlock"), 60.f, 68.f, 16,  9, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 7
	Scatter(P3D("SignPainters"), 60.f, 68.f, 16,  5, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 11
	Scatter(P3D("FootBridge_B"), 60.f, 68.f, 16,  1, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 15
Zone(TEXT("People p3d, heights, the Marina"), 3.2f, 6.4f);
	Scatter(P3D("FootBridge_A"), 64.f, 70.f,  8,  6, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 8 == 2
	Scatter(P3D("Lookout"),      64.f, 72.f,  8,  2, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 8 == 6
	Scatter(P3D("JudgeTower"),   64.f, 72.f,  8,  1, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 8 == 7
	Scatter(P3D("ScissorLift"),  64.f, 70.f,  8,  5, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 8 == 3
	Scatter(P3D("RoofParty_B"),  58.f, 64.f, 16, 15, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 1
	Scatter(P3D("ScissorLift"),  58.f, 64.f, 16, 11, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 5
	Scatter(P3D("BalconyBlock"), 58.f, 64.f, 16,  7, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 9
	Scatter(P3D("SignPainters"), 58.f, 64.f, 16,  3, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 13
Zone(TEXT("People p3d, heights, Pleasure Beach"), 6.4f, 9.8f);
	Scatter(P3D("SkyBikes"),     68.f, 74.f,  8,  6, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 8 == 2
	Scatter(P3D("ClimbWall"),    68.f, 74.f,  8,  4, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 8 == 4
	Scatter(P3D("FootBridge_A"), 68.f, 74.f,  8,  2, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 8 == 6
	Scatter(P3D("RoofParty_A"),  60.f, 68.f, 16, 15, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 1
	Scatter(P3D("RoofParty_B"),  60.f, 68.f, 16, 11, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 5
	Scatter(P3D("ScissorLift"),  60.f, 68.f, 16,  7, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 9
	Scatter(P3D("FootBridge_B"), 60.f, 68.f, 16,  3, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 13
Zone(TEXT("People p3d, heights, Hotel Row"), 9.8f, 13.2f);
	Scatter(P3D("JudgeTower"),   66.f, 74.f,  8,  7, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 8 == 1
	Scatter(P3D("SkyBikes"),     68.f, 74.f,  8,  3, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 8 == 5
	Scatter(P3D("RoofParty_A"),  66.f, 74.f,  8,  1, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 8 == 7: a beach house
	Scatter(P3D("BalconyBlock"), 58.f, 68.f, 16, 14, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 2
	Scatter(P3D("SignPainters"), 58.f, 68.f, 16, 10, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 6
	Scatter(P3D("BalconyBlock"), 58.f, 68.f, 16,  6, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 10
	Scatter(P3D("ScissorLift"),  58.f, 68.f, 16,  2, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 14
Zone(TEXT("People p3d, heights, the Point"), 13.2f, 16.8f);
	Scatter(P3D("Lookout"),      60.f, 74.f,  8,  7, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 8 == 1
	Scatter(P3D("Lookout"),      60.f, 74.f,  8,  4, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 8 == 4
	Scatter(P3D("ClimbWall"),    64.f, 74.f,  8,  2, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 8 == 6
	Scatter(P3D("RoofParty_A"),  62.f, 70.f,  6,  5, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 6 == 1
	Scatter(P3D("RoofParty_B"),  62.f, 70.f,  6,  1, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 6 == 5
Zone(TEXT("People p3d, heights, Downtown waterfront"), 16.8f, 99.f);
	Scatter(P3D("SkyBikes"),     66.f, 72.f, 16, 14, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 16 == 2
	Scatter(P3D("JudgeTower"),   66.f, 72.f, 16,  6, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 16 == 10
	Scatter(P3D("ClimbWall"),    66.f, 72.f, 16, 10, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 16 == 6
	Scatter(P3D("FootBridge_A"), 66.f, 72.f, 16,  2, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 16 == 14
	Scatter(P3D("Lookout"),      68.f, 72.f, 16,  9, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 16 == 7
	Scatter(P3D("RoofParty_B"),  68.f, 72.f, 16,  1, 0.9f, 1.f, 0.f, -1, 0.f);   // bay, s % 16 == 15: a beach house
	Scatter(P3D("BalconyBlock"), 60.f, 68.f, 16, 13, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 3
	Scatter(P3D("SignPainters"), 60.f, 68.f, 16,  9, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 7
	Scatter(P3D("RoofParty_A"),  60.f, 68.f, 16,  5, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 11
	Scatter(P3D("FootBridge_B"), 60.f, 68.f, 16,  1, 0.9f, 1.f, 0.f,  1, 0.f);   // town, s % 16 == 15
#undef P3D
// ---- END p3d ----
}

{
// ---- BEGIN p3e (far and sky, ft_people_bay_p3e.py) ----
// Pass 3, builder e: life from 185 m to the horizon and in the air, on BOTH sides. Twelve meshes, all TALL and all saved with
// the always-moving master (far=True); nothing of this block stands nearer than 150 m to the centre line (the widest ring,
// Gliders_A's, reaches 39 m from its middle, 43 m at the largest scale: 200 m less 43 m). Bay side (left): a football game and riders on the lower beach
// (Scatter 185-290 m, beyond the first rounds' crowd at 110-180 m), paragliders and parachutes over the sand and the surf (Far, the
// base 8 and 15 m ABOVE the road: a negative drop), rowers, jet skis, flamingos and kite surfers on the water in lanes of their
// own (Afloat 362-600 m), balloons far out (Afloat 560-900 m). Town side (right): balloons, hang gliders and round parachutes in
// the air over the gap between the second row of hotels and the skyline (Far 330-370 m, the base 45 to 55 m above the road).
// The travelling pieces (Flamingos, KiteSurf, Rowers, Riders) run along their own X, so they are turned by a few degrees only.
#define P3E(n) TEXT("/Game/People/Bayshore/SM_P3E_" #n)
Zone(TEXT("People p3e far and sky: the Strip"), 0.f, 3.2f);   // balloons over the town, gliders over the busy sand, a game on the lower beach
	Scatter(P3E(BeachGame), 185.f, 235.f, 11, 2, 1.f, 1.2f, 0.08f, -1, 8.f);
	Afloat(P3E(KiteSurf), 520.f, 600.f, 12, 1, 1.f, 1.5f, 0.1f, -1, 6.f);
	Far(P3E(Gliders_A), 200.f, 285.f, 13, 5, 1.f, 1.f, 0.1f, -1, -8.f, 180.f);
	Afloat(P3E(JetSkis), 450.f, 510.f, 15, 10, 1.f, 1.4f, 0.1f, -1, 180.f);
	Far(P3E(Balloons_B), 335.f, 365.f, 15, 4, 1.f, 1.f, 0.12f, 1, -45.f, 180.f);
	Afloat(P3E(Balloons_A), 560.f, 900.f, 17, 3, 1.f, 1.5f, 0.12f, -1, 180.f);
	Far(P3E(Chutes_B), 330.f, 370.f, 18, 13, 0.f, 1.f, 0.1f, 1, -55.f, 180.f);   // fix round: off. Parachutists read as the helicopter's dropped men (gunships from 3.7 km on)
	Far(P3E(Gliders_B), 335.f, 350.f, 19, 9, 1.f, 1.f, 0.08f, 1, -55.f, 180.f);
	Scatter(P3E(Riders), 240.f, 290.f, 20, 6, 1.f, 1.3f, 0.08f, -1, 6.f);
	Far(P3E(Chutes_A), 300.f, 420.f, 21, 8, 0.f, 1.2f, 0.1f, -1, -15.f, 180.f);   // fix round: off. Parachutists read as the helicopter's dropped men (gunships from 3.7 km on)
	Afloat(P3E(Rowers), 362.f, 376.f, 25, 15, 1.f, 1.4f, 0.f, -1, 5.f);
	Afloat(P3E(Flamingos), 440.f, 520.f, 29, 12, 1.f, 1.3f, 0.1f, -1, 6.f);
Zone(TEXT("People p3e far and sky: the Marina"), 3.2f, 6.4f);   // rowing crews off the jetties, flamingos over the boats
	Afloat(P3E(Rowers), 362.f, 376.f, 10, 2, 1.f, 1.4f, 0.f, -1, 5.f);
	Afloat(P3E(Flamingos), 440.f, 520.f, 11, 6, 1.f, 1.3f, 0.1f, -1, 6.f);
	Scatter(P3E(Riders), 240.f, 290.f, 14, 9, 1.f, 1.3f, 0.08f, -1, 6.f);
	Far(P3E(Gliders_B), 335.f, 350.f, 15, 12, 1.f, 1.f, 0.08f, 1, -55.f, 180.f);
	Far(P3E(Balloons_B), 335.f, 365.f, 18, 7, 1.f, 1.f, 0.12f, 1, -45.f, 180.f);
	Far(P3E(Chutes_B), 330.f, 370.f, 20, 3, 0.f, 1.f, 0.1f, 1, -55.f, 180.f);   // fix round: off. Parachutists read as the helicopter's dropped men (gunships from 3.7 km on)
	Afloat(P3E(JetSkis), 450.f, 510.f, 22, 8, 1.f, 1.4f, 0.1f, -1, 180.f);
	Afloat(P3E(Balloons_A), 560.f, 900.f, 23, 11, 1.f, 1.5f, 0.12f, -1, 180.f);
	Far(P3E(Chutes_A), 300.f, 420.f, 25, 4, 0.f, 1.2f, 0.1f, -1, -15.f, 180.f);   // fix round: off. Parachutists read as the helicopter's dropped men (gunships from 3.7 km on)
	Scatter(P3E(BeachGame), 185.f, 235.f, 27, 20, 1.f, 1.2f, 0.08f, -1, 8.f);
	Far(P3E(Gliders_A), 200.f, 285.f, 29, 14, 0.f, 1.f, 0.1f, -1, -8.f, 180.f);   // fix round: off (every second district: a sky full of canopies read as an attack coming in)
	Afloat(P3E(KiteSurf), 520.f, 600.f, 31, 17, 1.f, 1.5f, 0.1f, -1, 6.f);
Zone(TEXT("People p3e far and sky: Pleasure Beach"), 6.4f, 9.8f);   // the sky at its fullest: gliders, parachutes, kites on the water
	Scatter(P3E(BeachGame), 185.f, 235.f, 10, 5, 1.f, 1.2f, 0.08f, -1, 8.f);
	Far(P3E(Gliders_A), 200.f, 285.f, 11, 1, 1.f, 1.f, 0.1f, -1, -8.f, 180.f);
	Afloat(P3E(JetSkis), 450.f, 510.f, 12, 3, 1.f, 1.4f, 0.1f, -1, 180.f);
	Afloat(P3E(KiteSurf), 520.f, 600.f, 13, 4, 1.f, 1.5f, 0.1f, -1, 6.f);
	Far(P3E(Chutes_A), 300.f, 420.f, 14, 6, 0.f, 1.2f, 0.1f, -1, -15.f, 180.f);   // fix round: off. Parachutists read as the helicopter's dropped men (gunships from 3.7 km on)
	Far(P3E(Chutes_B), 330.f, 370.f, 14, 8, 0.f, 1.f, 0.1f, 1, -55.f, 180.f);   // fix round: off. Parachutists read as the helicopter's dropped men (gunships from 3.7 km on)
	Afloat(P3E(Balloons_A), 560.f, 900.f, 15, 7, 1.f, 1.5f, 0.12f, -1, 180.f);
	Far(P3E(Balloons_B), 335.f, 365.f, 16, 10, 1.f, 1.f, 0.12f, 1, -45.f, 180.f);
	Scatter(P3E(Riders), 240.f, 290.f, 19, 12, 1.f, 1.3f, 0.08f, -1, 6.f);
	Far(P3E(Gliders_B), 335.f, 350.f, 21, 2, 1.f, 1.f, 0.08f, 1, -55.f, 180.f);
	Afloat(P3E(Rowers), 362.f, 376.f, 23, 9, 1.f, 1.4f, 0.f, -1, 5.f);
	Afloat(P3E(Flamingos), 440.f, 520.f, 31, 19, 1.f, 1.3f, 0.1f, -1, 6.f);
Zone(TEXT("People p3e far and sky: Hotel Row"), 9.8f, 13.2f);   // balloons and hang gliders over the hotels, jet skis off the beach
	Far(P3E(Balloons_B), 335.f, 365.f, 12, 1, 1.f, 1.f, 0.12f, 1, -45.f, 180.f);
	Scatter(P3E(BeachGame), 185.f, 235.f, 14, 11, 1.f, 1.2f, 0.08f, -1, 8.f);
	Far(P3E(Gliders_B), 335.f, 350.f, 14, 6, 1.f, 1.f, 0.08f, 1, -55.f, 180.f);
	Afloat(P3E(KiteSurf), 520.f, 600.f, 15, 10, 1.f, 1.5f, 0.1f, -1, 6.f);
	Far(P3E(Chutes_B), 330.f, 370.f, 16, 12, 0.f, 1.f, 0.1f, 1, -55.f, 180.f);   // fix round: off. Parachutists read as the helicopter's dropped men (gunships from 3.7 km on)
	Afloat(P3E(JetSkis), 450.f, 510.f, 17, 7, 1.f, 1.4f, 0.1f, -1, 180.f);
	Scatter(P3E(Riders), 240.f, 290.f, 17, 3, 1.f, 1.3f, 0.08f, -1, 6.f);
	Far(P3E(Chutes_A), 300.f, 420.f, 19, 13, 0.f, 1.2f, 0.1f, -1, -15.f, 180.f);   // fix round: off. Parachutists read as the helicopter's dropped men (gunships from 3.7 km on)
	Afloat(P3E(Rowers), 362.f, 376.f, 19, 16, 1.f, 1.4f, 0.f, -1, 5.f);
	Far(P3E(Gliders_A), 200.f, 285.f, 21, 9, 0.f, 1.f, 0.1f, -1, -8.f, 180.f);   // fix round: off (every second district: a sky full of canopies read as an attack coming in)
	Afloat(P3E(Balloons_A), 560.f, 900.f, 23, 2, 1.f, 1.5f, 0.12f, -1, 180.f);
	Afloat(P3E(Flamingos), 440.f, 520.f, 25, 5, 1.f, 1.3f, 0.1f, -1, 6.f);
Zone(TEXT("People p3e far and sky: the Point"), 13.2f, 16.8f);   // quieter: riders on the lower beach, flamingos, rowers
	Scatter(P3E(Riders), 240.f, 290.f, 9, 4, 1.f, 1.3f, 0.08f, -1, 6.f);
	Afloat(P3E(Flamingos), 440.f, 520.f, 10, 1, 1.f, 1.3f, 0.1f, -1, 6.f);
	Far(P3E(Balloons_B), 335.f, 365.f, 11, 5, 1.f, 1.f, 0.12f, 1, -45.f, 180.f);
	Afloat(P3E(Rowers), 362.f, 376.f, 13, 6, 1.f, 1.4f, 0.f, -1, 5.f);
	Far(P3E(Gliders_B), 335.f, 350.f, 13, 8, 1.f, 1.f, 0.08f, 1, -55.f, 180.f);
	Far(P3E(Chutes_B), 330.f, 370.f, 15, 10, 0.f, 1.f, 0.1f, 1, -55.f, 180.f);   // fix round: off. Parachutists read as the helicopter's dropped men (gunships from 3.7 km on)
	Far(P3E(Gliders_A), 200.f, 285.f, 17, 3, 0.f, 1.f, 0.1f, -1, -8.f, 180.f);   // fix round: off (every second district: a sky full of canopies read as an attack coming in)
	Afloat(P3E(Balloons_A), 560.f, 900.f, 19, 9, 1.f, 1.5f, 0.12f, -1, 180.f);
	Afloat(P3E(JetSkis), 450.f, 510.f, 24, 20, 1.f, 1.4f, 0.1f, -1, 180.f);
	Far(P3E(Chutes_A), 300.f, 420.f, 26, 15, 0.f, 1.2f, 0.1f, -1, -15.f, 180.f);   // fix round: off. Parachutists read as the helicopter's dropped men (gunships from 3.7 km on)
	Afloat(P3E(KiteSurf), 520.f, 600.f, 27, 12, 1.f, 1.5f, 0.1f, -1, 6.f);
	Scatter(P3E(BeachGame), 185.f, 235.f, 33, 17, 1.f, 1.2f, 0.08f, -1, 8.f);
Zone(TEXT("People p3e far and sky: Downtown waterfront"), 16.8f, 99.f);   // everything at once
	Far(P3E(Balloons_B), 335.f, 365.f, 14, 6, 1.f, 1.f, 0.12f, 1, -45.f, 180.f);
	Scatter(P3E(BeachGame), 185.f, 235.f, 15, 3, 1.f, 1.2f, 0.08f, -1, 8.f);
	Far(P3E(Chutes_B), 330.f, 370.f, 15, 1, 0.f, 1.f, 0.1f, 1, -55.f, 180.f);   // fix round: off. Parachutists read as the helicopter's dropped men (gunships from 3.7 km on)
	Far(P3E(Gliders_A), 200.f, 285.f, 16, 11, 1.f, 1.f, 0.1f, -1, -8.f, 180.f);
	Far(P3E(Chutes_A), 300.f, 420.f, 16, 2, 0.f, 1.2f, 0.1f, -1, -15.f, 180.f);   // fix round: off. Parachutists read as the helicopter's dropped men (gunships from 3.7 km on)
	Scatter(P3E(Riders), 240.f, 290.f, 16, 13, 1.f, 1.3f, 0.08f, -1, 6.f);
	Afloat(P3E(Balloons_A), 560.f, 900.f, 17, 5, 1.f, 1.5f, 0.12f, -1, 180.f);
	Afloat(P3E(JetSkis), 450.f, 510.f, 17, 9, 1.f, 1.4f, 0.1f, -1, 180.f);
	Far(P3E(Gliders_B), 335.f, 350.f, 17, 10, 1.f, 1.f, 0.08f, 1, -55.f, 180.f);
	Afloat(P3E(KiteSurf), 520.f, 600.f, 18, 7, 1.f, 1.5f, 0.1f, -1, 6.f);
	Afloat(P3E(Rowers), 362.f, 376.f, 21, 18, 1.f, 1.4f, 0.f, -1, 5.f);
	Afloat(P3E(Flamingos), 440.f, 520.f, 23, 14, 1.f, 1.3f, 0.1f, -1, 6.f);
#undef P3E
// ---- END p3e ----
}

{
// ---- BEGIN p3f (film shoot, ft_people_bay_p3f.py) ----
// More of the shoot: second units, drone teams, a wire rig, the extras, make-up, the press, grips (ten meshes SM_P3F_*,
// each raised on its own deck or riser: every one is TALL, so no rule here is nearer than 24 m) and more of the crew
// kit in wardrobe Sun. Written by C:/ftwork/jobs/L0p3f/make_inl.py: one Site a 45 m slot, one district a slot (as the
// first pass wrote its own crew), so the count is exact and no slot of the first pass's PplShoot units is used twice.
//   town side (+1)  anchor 26 m, in the side-street slots (s%4==0: no shop row at 27.5 or 33 m there), the pieces 13 m
//                   either side of the street's mouth (the street is 10.2 m wide); the P3F pieces 28.5 m out and more;
//                   not in a slot of PplShootA/B/C, not on steep land (kiosks stand at 31.6 m there)
//   bay side (-1)   anchor 31 m, on the sand behind the boardwalk, in slots that have neither the arcade nor the walk's
//                   shops nor the bandstand: Strip s%8==5, Marina s%12==1 or 7, Pleasure Beach s%8==2 (behind the guard
//                   tower), Hotel Row and the Point s%4==2, Downtown s%8==1
// No slot of a loop or a corkscrew, none within three slots of a jump. Nothing glows but the kit's own steady lamps
// (SM_CrewLights, 33 m out and more); nothing blinks; the drones hover or circle 29 m out and more, 5.6 m up and more.
#define P3F(n) TEXT("/Game/People/Bayshore/SM_P3F_" n)
#define P3FCREW(n) TEXT("/Game/People/Crew/SM_Crew" n "_Sun")
Set(TEXT("PplP3FTownUnit"), {      // a camera tower, the bounce frame, lights behind
	{ P3F("SecondUnit_A"), -13, 2.5f, 0, 0, 1 },
	{ P3F("Reflector"), 13, 2.5f, 0, 0, 1 },
	{ P3FCREW("Lights"), 13, 7.5f, 0, 0, 1 },
});
Set(TEXT("PplP3FTownDrone"), {      // a drone over its pad, extras waiting, a camera at the tape
	{ P3F("DroneTeam_A"), -13, 5.5f, 0, 0, 1 },
	{ P3F("Extras_B"), 13, 2.5f, 0, 0, 1 },
	{ P3FCREW("Camera"), -13, 0, 0, 0, 1 },
});
Set(TEXT("PplP3FTownStunt"), {      // the wire rig, the make-up tent, crew applauding
	{ P3F("StuntRig"), -13.5f, 3, 0, 0, 1 },
	{ P3F("MakeUp"), 13, 3.5f, 0, 0, 1 },
	{ P3FCREW("ApplauseB"), 13, 0, 0, 0, 1 },
});
Set(TEXT("PplP3FTownPress"), {      // the press pen and a long-lens position
	{ P3F("PressPen"), -13, 2.5f, 0, 0, 1 },
	{ P3F("SecondUnit_B"), 13, 2.5f, 0, 0, 1 },
});
Set(TEXT("PplP3FTownExtras"), {      // the riser of extras, catering, the village
	{ P3F("Extras_A"), -13, 2.5f, 0, 0, 1 },
	{ P3FCREW("Catering"), 13, 4.5f, 0, 0, 1 },
	{ P3FCREW("Village"), 13, 0, 0, 0, 1 },
});
Set(TEXT("PplP3FTownRing"), {      // two drones round their mast, the crane, crew applauding
	{ P3F("DroneTeam_B"), 13, 6.5f, 0, 0, 1 },
	{ P3FCREW("Crane"), -13, 6, 0, 0, 1 },
	{ P3FCREW("ApplauseA"), 13, 0, 0, 0, 1 },
});
Set(TEXT("PplP3FBayUnit"), {      // a camera tower and the crane on the sand
	{ P3F("SecondUnit_A"), -9, 1.5f, 0, 0, 1 },
	{ P3FCREW("Crane"), 9, 4, 0, 0, 1 },
});
Set(TEXT("PplP3FBayRing"), {      // two drones round their mast, the riser of extras
	{ P3F("DroneTeam_B"), 9, 3.5f, 0, 0, 1 },
	{ P3F("Extras_A"), -9, 1.5f, 0, 0, 1 },
});
Set(TEXT("PplP3FBayStunt"), {      // the wire rig between lights and sound
	{ P3F("StuntRig"), 0, 1.5f, 0, 0, 1 },
	{ P3FCREW("Lights"), -15, 2, 0, 0, 1 },
	{ P3FCREW("Sound"), 15, 2, 0, 0, 1 },
});
Set(TEXT("PplP3FBayMakeUp"), {      // the make-up tent and the deck of extras
	{ P3F("MakeUp"), -8, 1.5f, 0, 0, 1 },
	{ P3F("Extras_B"), 9, 1.5f, 0, 0, 1 },
});
Set(TEXT("PplP3FBayPress"), {      // the press pen and the bounce frame
	{ P3F("PressPen"), -9, 1.5f, 0, 0, 1 },
	{ P3F("Reflector"), 9, 1.5f, 0, 0, 1 },
});
Set(TEXT("PplP3FBayLens"), {      // a long-lens position and a drone over its pad
	{ P3F("SecondUnit_B"), -9, 1.5f, 0, 0, 1 },
	{ P3F("DroneTeam_A"), 9, 3, 0, 0, 1 },
});
Zone(TEXT("People p3f, the shoot, the Strip"), 0.20359f, 0.24884f);
	Site(TEXT("PplP3FBayUnit"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Strip"), 0.52029f, 0.56554f);
	Site(TEXT("PplP3FTownUnit"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Strip"), 0.56554f, 0.61078f);
	Site(TEXT("PplP3FBayRing"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Strip"), 0.70127f, 0.74651f);
	Site(TEXT("PplP3FTownDrone"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Strip"), 0.88224f, 0.92748f);
	Site(TEXT("PplP3FTownStunt"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Strip"), 0.92748f, 0.97272f);
	Site(TEXT("PplP3FBayStunt"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Strip"), 1.06321f, 1.10845f);
	Site(TEXT("PplP3FTownPress"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Strip"), 1.24418f, 1.28943f);
	Site(TEXT("PplP3FTownExtras"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Strip"), 1.28943f, 1.33467f);
	Site(TEXT("PplP3FBayMakeUp"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Strip"), 1.42515f, 1.4704f);
	Site(TEXT("PplP3FTownRing"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Strip"), 2.33001f, 2.37526f);
	Site(TEXT("PplP3FTownUnit"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Strip"), 2.37526f, 2.4205f);
	Site(TEXT("PplP3FBayPress"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Strip"), 2.69196f, 2.7372f);
	Site(TEXT("PplP3FTownDrone"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Strip"), 2.7372f, 2.78244f);
	Site(TEXT("PplP3FBayLens"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Strip"), 3.09915f, 3.14439f);
	Site(TEXT("PplP3FBayUnit"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Marina"), 3.28012f, 3.32536f);
	Site(TEXT("PplP3FBayRing"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Marina"), 3.77779f, 3.82303f);
	Site(TEXT("PplP3FTownStunt"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Marina"), 3.82303f, 3.86828f);
	Site(TEXT("PplP3FBayStunt"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Marina"), 3.95876f, 4.00401f);
	Site(TEXT("PplP3FTownPress"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Marina"), 4.63741f, 4.68265f);
	Site(TEXT("PplP3FBayMakeUp"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Marina"), 4.68265f, 4.72789f);
	Site(TEXT("PplP3FTownExtras"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Marina"), 4.86362f, 4.90887f);
	Site(TEXT("PplP3FTownRing"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Marina"), 4.90887f, 4.95411f);
	Site(TEXT("PplP3FBayPress"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Marina"), 5.04459f, 5.08984f);
	Site(TEXT("PplP3FTownUnit"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Marina"), 5.58751f, 5.63275f);
	Site(TEXT("PplP3FTownDrone"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Marina"), 5.72324f, 5.76848f);
	Site(TEXT("PplP3FBayLens"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Marina"), 5.76848f, 5.81373f);
	Site(TEXT("PplP3FTownStunt"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Marina"), 5.94945f, 5.9947f);
	Site(TEXT("PplP3FTownPress"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Marina"), 5.9947f, 6.03994f);
	Site(TEXT("PplP3FBayUnit"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Marina"), 6.13043f, 6.17567f);
	Site(TEXT("PplP3FTownExtras"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Marina"), 6.3114f, 6.35664f);
	Site(TEXT("PplP3FTownRing"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 6.49237f, 6.53761f);
	Site(TEXT("PplP3FTownUnit"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 6.58286f, 6.6281f);
	Site(TEXT("PplP3FBayRing"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 6.67334f, 6.71859f);
	Site(TEXT("PplP3FTownDrone"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 7.21626f, 7.2615f);
	Site(TEXT("PplP3FTownStunt"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 7.30674f, 7.35199f);
	Site(TEXT("PplP3FBayStunt"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 7.39723f, 7.44247f);
	Site(TEXT("PplP3FTownPress"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 7.66869f, 7.71393f);
	Site(TEXT("PplP3FBayMakeUp"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 7.75917f, 7.80442f);
	Site(TEXT("PplP3FTownExtras"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 7.94015f, 7.98539f);
	Site(TEXT("PplP3FTownRing"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 8.03063f, 8.07588f);
	Site(TEXT("PplP3FBayPress"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 8.12112f, 8.16636f);
	Site(TEXT("PplP3FTownUnit"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 8.75452f, 8.79976f);
	Site(TEXT("PplP3FBayLens"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 8.84501f, 8.89025f);
	Site(TEXT("PplP3FTownDrone"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 9.02598f, 9.07122f);
	Site(TEXT("PplP3FTownStunt"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 9.11646f, 9.16171f);
	Site(TEXT("PplP3FBayUnit"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 9.20695f, 9.25219f);
	Site(TEXT("PplP3FTownPress"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 9.47841f, 9.52365f);
	Site(TEXT("PplP3FBayRing"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 9.56889f, 9.61414f);
	Site(TEXT("PplP3FTownExtras"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Pleasure Beach"), 9.74987f, 9.79511f);
	Site(TEXT("PplP3FTownRing"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 9.84035f, 9.8856f);
	Site(TEXT("PplP3FBayStunt"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 10.02132f, 10.06657f);
	Site(TEXT("PplP3FBayMakeUp"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 10.65473f, 10.69997f);
	Site(TEXT("PplP3FTownUnit"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 10.74521f, 10.79046f);
	Site(TEXT("PplP3FBayPress"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 10.8357f, 10.88094f);
	Site(TEXT("PplP3FTownDrone"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 10.92618f, 10.97143f);
	Site(TEXT("PplP3FBayLens"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 11.01667f, 11.06191f);
	Site(TEXT("PplP3FTownStunt"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 11.10716f, 11.1524f);
	Site(TEXT("PplP3FBayUnit"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 11.19764f, 11.24289f);
	Site(TEXT("PplP3FTownPress"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 11.28813f, 11.33337f);
	Site(TEXT("PplP3FBayRing"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 11.83104f, 11.87629f);
	Site(TEXT("PplP3FBayStunt"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 11.92153f, 11.96677f);
	Site(TEXT("PplP3FTownExtras"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 12.01202f, 12.05726f);
	Site(TEXT("PplP3FBayMakeUp"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 12.1025f, 12.14775f);
	Site(TEXT("PplP3FTownRing"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 12.19299f, 12.23823f);
	Site(TEXT("PplP3FBayPress"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 12.28347f, 12.32872f);
	Site(TEXT("PplP3FTownUnit"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 12.37396f, 12.4192f);
	Site(TEXT("PplP3FBayLens"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 12.46445f, 12.50969f);
	Site(TEXT("PplP3FTownDrone"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Hotel Row"), 12.55493f, 12.60018f);
	Site(TEXT("PplP3FBayUnit"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 13.27882f, 13.32406f);
	Site(TEXT("PplP3FBayRing"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 13.36931f, 13.41455f);
	Site(TEXT("PplP3FTownStunt"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 13.45979f, 13.50504f);
	Site(TEXT("PplP3FBayStunt"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 13.55028f, 13.59552f);
	Site(TEXT("PplP3FTownPress"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 13.64076f, 13.68601f);
	Site(TEXT("PplP3FBayMakeUp"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 13.73125f, 13.77649f);
	Site(TEXT("PplP3FTownExtras"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 13.82174f, 13.86698f);
	Site(TEXT("PplP3FBayPress"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 13.91222f, 13.95747f);
	Site(TEXT("PplP3FTownRing"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 14.00271f, 14.04795f);
	Site(TEXT("PplP3FBayLens"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 14.09319f, 14.13844f);
	Site(TEXT("PplP3FTownUnit"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 14.18368f, 14.22892f);
	Site(TEXT("PplP3FBayUnit"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 14.27417f, 14.31941f);
	Site(TEXT("PplP3FTownDrone"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 14.36465f, 14.4099f);
	Site(TEXT("PplP3FBayRing"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 15.08854f, 15.13378f);
	Site(TEXT("PplP3FBayStunt"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 15.17903f, 15.22427f);
	Site(TEXT("PplP3FTownStunt"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 15.26951f, 15.31476f);
	Site(TEXT("PplP3FBayMakeUp"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 15.36f, 15.40524f);
	Site(TEXT("PplP3FTownPress"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 15.45048f, 15.49573f);
	Site(TEXT("PplP3FBayPress"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 15.54097f, 15.58621f);
	Site(TEXT("PplP3FTownExtras"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 15.81243f, 15.85767f);
	Site(TEXT("PplP3FBayLens"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 15.90291f, 15.94816f);
	Site(TEXT("PplP3FTownRing"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 15.9934f, 16.03864f);
	Site(TEXT("PplP3FBayUnit"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 16.17437f, 16.21962f);
	Site(TEXT("PplP3FBayRing"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 16.53632f, 16.58156f);
	Site(TEXT("PplP3FBayStunt"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 16.6268f, 16.67205f);
	Site(TEXT("PplP3FTownUnit"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, the Point"), 16.71729f, 16.76253f);
	Site(TEXT("PplP3FBayMakeUp"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 16.80777f, 16.85302f);
	Site(TEXT("PplP3FTownDrone"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 16.98875f, 17.03399f);
	Site(TEXT("PplP3FTownStunt"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 17.03399f, 17.07923f);
	Site(TEXT("PplP3FBayPress"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 17.71263f, 17.75788f);
	Site(TEXT("PplP3FTownPress"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 17.75788f, 17.80312f);
	Site(TEXT("PplP3FBayLens"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 17.89361f, 17.93885f);
	Site(TEXT("PplP3FTownExtras"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 18.11982f, 18.16506f);
	Site(TEXT("PplP3FBayUnit"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 18.25555f, 18.30079f);
	Site(TEXT("PplP3FTownRing"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 18.43652f, 18.48177f);
	Site(TEXT("PplP3FTownUnit"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 18.48177f, 18.52701f);
	Site(TEXT("PplP3FBayRing"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 19.16041f, 19.20565f);
	Site(TEXT("PplP3FTownDrone"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 19.20565f, 19.2509f);
	Site(TEXT("PplP3FBayStunt"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 19.52235f, 19.5676f);
	Site(TEXT("PplP3FTownStunt"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 19.5676f, 19.61284f);
	Site(TEXT("PplP3FBayMakeUp"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 19.70333f, 19.74857f);
	Site(TEXT("PplP3FTownPress"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 19.8843f, 19.92954f);
	Site(TEXT("PplP3FTownExtras"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 19.92954f, 19.97478f);
	Site(TEXT("PplP3FBayPress"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 20.60819f, 20.65343f);
	Site(TEXT("PplP3FTownRing"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 20.65343f, 20.69867f);
	Site(TEXT("PplP3FBayLens"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 20.97013f, 21.01537f);
	Site(TEXT("PplP3FTownUnit"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 21.01537f, 21.06062f);
	Site(TEXT("PplP3FBayUnit"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 21.1511f, 21.19635f);
	Site(TEXT("PplP3FTownDrone"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 21.33207f, 21.37732f);
	Site(TEXT("PplP3FTownStunt"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 21.37732f, 21.42256f);
	Site(TEXT("PplP3FBayRing"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 22.05596f, 22.10121f);
	Site(TEXT("PplP3FTownPress"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 22.10121f, 22.14645f);
	Site(TEXT("PplP3FBayStunt"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 22.41791f, 22.46315f);
	Site(TEXT("PplP3FTownExtras"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 22.46315f, 22.50839f);
	Site(TEXT("PplP3FBayMakeUp"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 22.59888f, 22.64412f);
	Site(TEXT("PplP3FTownRing"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 22.77985f, 22.82509f);
	Site(TEXT("PplP3FTownUnit"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 22.82509f, 22.87034f);
	Site(TEXT("PplP3FBayPress"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 23.50374f, 23.54898f);
	Site(TEXT("PplP3FTownDrone"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 23.54898f, 23.59422f);
	Site(TEXT("PplP3FBayLens"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 23.91093f, 23.95617f);
	Site(TEXT("PplP3FBayUnit"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 24.27287f, 24.31811f);
	Site(TEXT("PplP3FBayRing"), -1, 31.0f, 1, 0, 1.f);
Zone(TEXT("People p3f, the shoot, Downtown waterfront"), 24.99676f, 25.042f);
	Site(TEXT("PplP3FBayStunt"), -1, 31.0f, 1, 0, 1.f);
#undef P3F
#undef P3FCREW
// ---- END p3f ----
}

{
// ---- BEGIN p3 fix round (the review of pass 3; this block is hand-kept, no generator) ----
// The low pieces of p3a and p3c that stand right behind the barrier (the boardwalk at 21.3 and 21.7 m, the town's pavement
// at 21.3 and 22.4 m) were written for the whole road. On the hills the track moved 109 of them out past 24 m (a level
// piece on falling land stands over 2 m above the road at one end): off the boardwalk's deck, in among the shoppers and
// the shop fronts. Their rules above are off (chance 0.f, so every later rule keeps its place and its random numbers) and
// stand here again, with the same Sets, distances and slot residues, only in the slots where the dressing has its level and
// gentle street rows (the Street(...) lines of FTDressL0.inl whose row set is ShopRow or ShopRowB: the land rises or falls
// no more than 0.9 m within 20 m). Not on the slopes, the steep slots or the hills, nor beside a loop, a corkscrew or a jump.
#define P3XC(n) TEXT("/Game/People/Bayshore/SM_P3C_Wheels_" n)
auto P3XKm = [](float S) { return S * 0.045243f; };                // a slot is 45.243 m
auto P3XNearA = [&](float A, float B)      // the Strip, the Marina, Downtown waterfront
{
	Zone(TEXT("People p3 fix: by the barrier, level ground"), P3XKm(A - 0.5f), P3XKm(B + 0.5f));
	Site(TEXT("PplP3ARestA1"), -1, 21.3f, 20, 0, 1.f);
	Site(TEXT("PplP3ARestB2"), -1, 21.3f, 20, 18, 1.f);
	Site(TEXT("PplP3AWorkA1"), -1, 21.3f, 20, 15, 1.f);
	Site(TEXT("PplP3AWorkB2"), -1, 21.3f, 20, 13, 1.f);
	Site(TEXT("PplP3ARestB1"), -1, 21.3f, 20, 10, 1.f);
	Site(TEXT("PplP3ARestA2"), -1, 21.3f, 20, 8, 1.f);
	Site(TEXT("PplP3AWorkB1"), -1, 21.3f, 20, 5, 1.f);
	Site(TEXT("PplP3AWorkA2"), -1, 21.3f, 20, 3, 1.f);
	Site(TEXT("PplP3CWalkA"), -1, 21.7f, 12, 11, 1.f);
	Site(TEXT("PplP3CWalkB"), -1, 21.7f, 12, 5, 1.f);
	Site(TEXT("PplP3APaveA1"), 1, 21.3f, 20, 19, 1.f);
	Site(TEXT("PplP3APaveB1"), 1, 21.3f, 20, 14, 1.f);
	Site(TEXT("PplP3APaveA2"), 1, 21.3f, 20, 9, 1.f);
	Site(TEXT("PplP3APaveB2"), 1, 21.3f, 20, 6, 1.f);
	Scatter(P3XC("C"), 22.4f, 22.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3XC("D"), 22.4f, 22.4f, 12, 5, 1.f, 1.f, 0.f, 1, 0.f);
};
auto P3XNearB = [&](float A, float B)      // Pleasure Beach, Hotel Row (the cyclists' slots differ)
{
	Zone(TEXT("People p3 fix: by the barrier, level ground"), P3XKm(A - 0.5f), P3XKm(B + 0.5f));
	Site(TEXT("PplP3ARestA1"), -1, 21.3f, 20, 0, 1.f);
	Site(TEXT("PplP3ARestB2"), -1, 21.3f, 20, 18, 1.f);
	Site(TEXT("PplP3AWorkA1"), -1, 21.3f, 20, 15, 1.f);
	Site(TEXT("PplP3AWorkB2"), -1, 21.3f, 20, 13, 1.f);
	Site(TEXT("PplP3ARestB1"), -1, 21.3f, 20, 10, 1.f);
	Site(TEXT("PplP3ARestA2"), -1, 21.3f, 20, 8, 1.f);
	Site(TEXT("PplP3AWorkB1"), -1, 21.3f, 20, 5, 1.f);
	Site(TEXT("PplP3AWorkA2"), -1, 21.3f, 20, 3, 1.f);
	Site(TEXT("PplP3CWalkA"), -1, 21.7f, 12, 10, 1.f);
	Site(TEXT("PplP3CWalkB"), -1, 21.7f, 12, 4, 1.f);
	Site(TEXT("PplP3APaveA1"), 1, 21.3f, 20, 19, 1.f);
	Site(TEXT("PplP3APaveB1"), 1, 21.3f, 20, 14, 1.f);
	Site(TEXT("PplP3APaveA2"), 1, 21.3f, 20, 9, 1.f);
	Site(TEXT("PplP3APaveB2"), 1, 21.3f, 20, 6, 1.f);
	Scatter(P3XC("C"), 22.4f, 22.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3XC("D"), 22.4f, 22.4f, 12, 5, 1.f, 1.f, 0.f, 1, 0.f);
};
auto P3XNearP = [&](float A, float B)      // the Point (the boardwalk stands in s % 3 == 0 only)
{
	Zone(TEXT("People p3 fix: by the barrier, level ground"), P3XKm(A - 0.5f), P3XKm(B + 0.5f));
	Site(TEXT("PplP3ARestA1"), -1, 21.3f, 12, 0, 1.f);
	Site(TEXT("PplP3AWorkA2"), -1, 21.3f, 12, 9, 1.f);
	Site(TEXT("PplP3ARestB1"), -1, 21.3f, 12, 6, 1.f);
	Site(TEXT("PplP3AWorkB2"), -1, 21.3f, 12, 3, 1.f);
	Site(TEXT("PplP3CWalkA"), -1, 21.7f, 12, 0, 1.f);
	Site(TEXT("PplP3CWalkB"), -1, 21.7f, 12, 6, 1.f);
	Site(TEXT("PplP3APaveA1"), 1, 21.3f, 20, 19, 1.f);
	Site(TEXT("PplP3APaveB1"), 1, 21.3f, 20, 14, 1.f);
	Site(TEXT("PplP3APaveA2"), 1, 21.3f, 20, 9, 1.f);
	Site(TEXT("PplP3APaveB2"), 1, 21.3f, 20, 6, 1.f);
	Scatter(P3XC("C"), 22.4f, 22.4f, 12, 10, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3XC("D"), 22.4f, 22.4f, 12, 5, 1.f, 1.f, 0.f, 1, 0.f);
};
P3XNearA(1, 3); P3XNearA(5, 6); P3XNearA(8, 11); P3XNearA(14, 14); P3XNearA(23, 27); P3XNearA(30, 30); P3XNearA(33, 33); P3XNearA(36, 36);
P3XNearA(40, 42); P3XNearA(49, 54); P3XNearA(57, 57); P3XNearA(62, 63); P3XNearA(67, 67); P3XNearA(74, 74); P3XNearA(81, 88); P3XNearA(90, 90);
P3XNearA(92, 93); P3XNearA(101, 109); P3XNearA(122, 137); P3XNearA(140, 141); P3XNearB(142, 152); P3XNearB(160, 160); P3XNearB(169, 169);
P3XNearB(174, 174); P3XNearB(176, 184); P3XNearB(187, 187); P3XNearB(191, 193); P3XNearB(199, 205); P3XNearB(214, 217); P3XNearB(222, 223);
P3XNearB(228, 228); P3XNearB(236, 237); P3XNearB(243, 250); P3XNearB(253, 254); P3XNearB(261, 262); P3XNearB(267, 275); P3XNearB(277, 278);
P3XNearB(281, 281); P3XNearB(289, 289); P3XNearP(296, 318); P3XNearP(321, 321); P3XNearP(325, 327); P3XNearP(334, 345); P3XNearP(354, 359);
P3XNearP(366, 371); P3XNearA(376, 381); P3XNearA(390, 396); P3XNearA(403, 403); P3XNearA(408, 413); P3XNearA(422, 427); P3XNearA(432, 432);
P3XNearA(440, 444); P3XNearA(454, 459); P3XNearA(472, 476); P3XNearA(486, 491); P3XNearA(504, 508); P3XNearA(515, 515); P3XNearA(518, 523);
P3XNearA(535, 540); P3XNearA(549, 555);
#undef P3XC
// ---- END p3 fix round ----
}
