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

// ==== Stage 1 world (final pass): ten blocks, each in its own braces, appended after pass 3 so no earlier rule re-rolls ====
{
// ---- BEGIN s1d1 (district 1 "The Strip", 0 to 3.2 km, ft_world_bay_s1d1.py) ----
// 19 meshes (9 buildings, 9 props, the hero SM_S1D1_Sunburst), all made by the one make run (C:/ftwork/logs/L0s1d1_make.log).
// Streamline deco on both sides of the road, in the even slots only (s % 4 == 0 or 2), nothing outside 0 to 3.2 km.
// Estimate: the rules below use 254 piece-places counted slot by slot (252 + the hero's 2):
//   back rows 9 + 9 slots x 3 = 54; corners 9 + 9 slots x 4 = 72; street mouths 11 slots x (4 + 2) = 66 and 12 scattered;
//   the open boardwalk 8 + 4 slots x 4 = 48; 252 x 0.75 (the track's share left out) = 189, + 2 Sunbursts = about 191 placed.
//   The most of one mesh: SM_S1D1_Bench, 36 places before the 0.75 (about 27). Every chance is 1.f (a chance under 1 leaves holes in runs).
// Bands and slots used (x = metres from the centre line; a Set's pieces are listed with their own distance):
//   town s % 8 == 0 / 4   corners of the side street: buildings 41.5 m (36 to 46.5), 16 m along each way from the street's
//                         middle (the street is 10.2 m wide; the watchers' terrace bridges it at 38 m); clock, arrow 35 m; fin totem 39.5 m
//   town s % 8 == 2 / 6   back row 69 m (64 to 74): behind the pool deck (41 to 63), before the shop block (78); fin totems 66, fountain 65.5
//   town s % 4 == 0       LOW bollards and benches 22.5 / 23.7 m, lamp pylons 25 m, at the street's mouth: only from slot 10 on and
//                         not slot 36 (the film crew stands 21.9 m out in slots 1 to 8 and 36)
//   town s % 8 == 2 / 6   LOW bollards / a bench 21 to 21.8 m (between the kerb's cars, 18.6, and the shoppers, 24.6)
//   bay  s % 4 == 0 / 2   31 m anchor (pieces 26.8 to 42 m), ONLY in the slots where the third pass stands no arcade (33.5 m)
//                         and no boardwalk shops (32 m): 4, 12, 16, 18, 20, 28, 32, 34, 58, 60, 68, 70
//   bay  slots 8 and 52   the Sunburst, 150 m (its benches from 132.6 m, its back 157.5 m)
// Banked bends (slots 22 to 27 and 50 to 57): nothing of this block nearer than 40 m (corners at 47 m; the back rows; the hero).
// Glow (steady, never blinking): only on TALL meshes; the smallest x min of a TALL mesh is 25 m (SM_S1D1_PylonA).
// LOW meshes: SM_S1D1_Bollards (top 1.05 m), SM_S1D1_Bench (top 1.09 m), no glow, no movement; x min 21 m.
#define S1D1(n) TEXT("/Game/World/Bayshore/SM_S1D1_" n)
Set(TEXT("WldS1D1Sun"), {      // the hero, alone on the sand
	{ S1D1("Sunburst"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D1CornerA"), {      // a side street's two corners: the diner and the record shop, an arrow and a clock in front
	{ S1D1("Diner"), -16.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("RecordShop"), 16.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("NeonArrow"), -9.2f, -6.5f, 0.f, 0.f, 1.f },   // fix round: was -11.0, in the film crane of slot 32 (11.1 to 14.9 m along); the watchers' terrace ends 7.6 m along
	{ S1D1("Clock"), 11.0f, -6.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D1CornerB"), {      // the other corners: the milk bar and the dance hall, a fin totem at the street's mouth, a clock
	{ S1D1("MilkBar"), -16.3f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("DanceHall"), 16.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("PylonB"), -10.5f, -6.3f, 0.f, 0.f, 1.f },   // fix round: was -7.8 / -2.0, in the side street walkers' lane (6.6 to 8.4 m, 39 to 57 m out); now before the milk bar's front
	{ S1D1("Clock"), 11.0f, -6.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D1BackA"), {      // behind the pool decks: the bowling alley between two fin totems
	{ S1D1("Bowling"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("PylonB"), -14.0f, -3.0f, 0.f, 0.f, 1.f },
	{ S1D1("PylonB"), 14.0f, -3.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D1BackB"), {      // behind the pool decks: the picture house and the flats, a fountain in the court between
	{ S1D1("PictureHouse"), -10.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("Flats"), 10.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("Fountain"), 0.6f, -3.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D1MouthLow"), {      // LOW, the pavement each side of a side street's mouth: bollards and a bench
	{ S1D1("Bollards"), -9.5f, 1.2f, 0.f, 0.f, 1.f },   // fix round: out 1.2 (23.7 m, the benches' line): at 22.5 m they stood in the film crew of slot 40 (20.8 to 23.1 m)
	{ S1D1("Bollards"), 9.5f, 1.2f, 0.f, 0.f, 1.f },
	{ S1D1("Bench"), -16.5f, 1.2f, 0.f, 0.f, 1.f },
	{ S1D1("Bench"), 16.5f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D1MouthTall"), {      // two lamp pylons at the street's mouth
	{ S1D1("PylonA"), -6.6f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("PylonA"), 6.6f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D1BeachA"), {      // behind the boardwalk: the ice-cream parlour, deckchairs for hire, the jukebox kiosk, a bench
	{ S1D1("Parlour"), 0.0f, 6.0f, 0.f, 0.f, 1.f },
	{ S1D1("DeckHut"), -11.0f, -2.0f, 0.f, 0.f, 1.f },
	{ S1D1("JukeKiosk"), 11.0f, -2.2f, 0.f, 0.f, 1.f },
	{ S1D1("Bench"), 0.0f, -4.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D1BeachB"), {      // behind the boardwalk: the bathing pavilion, an arrow, a fin totem, deckchairs
	{ S1D1("BathHouse"), 0.0f, 5.5f, 0.f, 0.f, 1.f },
	{ S1D1("NeonArrow"), 11.0f, -2.0f, 0.f, 0.f, 1.f },
	{ S1D1("PylonB"), -11.0f, -2.0f, 0.f, 0.f, 1.f },
	{ S1D1("DeckHut"), 1.0f, -3.2f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1d1, The Strip"), 0.0f, 3.2f);
	Site(TEXT("WldS1D1Sun"), -1, 150.0f, 44, 36, 1.f);   // bay, slots 8 and 52 (0.36 and 2.35 km): no groyne (s % 4 == 2), no pier, no lifeguard tower in them
	Site(TEXT("WldS1D1BackA"), 1, 69.0f, 8, 6, 1.f);   // town, s % 8 == 2: 64 to 74 m, behind the pool deck (41 to 63), before the shop block (78)
	Site(TEXT("WldS1D1BackB"), 1, 69.0f, 8, 2, 1.f);   // town, s % 8 == 6
Zone(TEXT("World s1d1, The Strip, corners, slots 0 to 21"), 0.0f, 0.9718f);
	Site(TEXT("WldS1D1CornerA"), 1, 41.5f, 8, 0, 1.f);   // town, s % 8 == 0: the side street's slot; buildings 36 to 46.5 m, behind the promenade (26 to 34)
	Site(TEXT("WldS1D1CornerB"), 1, 41.5f, 8, 4, 1.f);   // town, s % 8 == 4
Zone(TEXT("World s1d1, The Strip, corners, slots 28 to 49"), 1.2451f, 2.2386f);
	Site(TEXT("WldS1D1CornerA"), 1, 41.5f, 8, 0, 1.f);   // town, s % 8 == 0: the side street's slot; buildings 36 to 46.5 m, behind the promenade (26 to 34)
	Site(TEXT("WldS1D1CornerB"), 1, 41.5f, 8, 4, 1.f);   // town, s % 8 == 4
Zone(TEXT("World s1d1, The Strip, corners, slots 58 to 70"), 2.6024f, 3.1887f);
	Site(TEXT("WldS1D1CornerA"), 1, 41.5f, 8, 0, 1.f);   // town, s % 8 == 0: the side street's slot; buildings 36 to 46.5 m, behind the promenade (26 to 34)
	Site(TEXT("WldS1D1CornerB"), 1, 41.5f, 8, 4, 1.f);   // town, s % 8 == 4
Zone(TEXT("World s1d1, The Strip, corners on a banked bend, slots 22 to 27"), 0.9736f, 1.2433f);
	Site(TEXT("WldS1D1CornerA"), 1, 47.0f, 8, 0, 1.f);   // 47 m: the nearest piece of the set stands 40.5 m out
	Site(TEXT("WldS1D1CornerB"), 1, 47.0f, 8, 4, 1.f);
Zone(TEXT("World s1d1, The Strip, corners on a banked bend, slots 50 to 57"), 2.2404f, 2.6006f);
	Site(TEXT("WldS1D1CornerA"), 1, 47.0f, 8, 0, 1.f);   // 47 m: the nearest piece of the set stands 40.5 m out
	Site(TEXT("WldS1D1CornerB"), 1, 47.0f, 8, 4, 1.f);
Zone(TEXT("World s1d1, The Strip, street mouths, slots 10 to 21"), 0.4307f, 0.9718f);
	Site(TEXT("WldS1D1MouthLow"), 1, 22.5f, 4, 0, 1.f);   // town, s % 4 == 0, LOW: between the kerb's cars (18.6) and the promenade (26)
	Site(TEXT("WldS1D1MouthTall"), 1, 25.0f, 4, 0, 1.f);   // TALL: 25 m
	Scatter(S1D1("Bench"), 21.0f, 21.8f, 8, 2, 1.f, 1.f, 0.f, 1, 0.f);   // town, s % 8 == 6, LOW
	Scatter(S1D1("Bollards"), 21.0f, 21.8f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);   // town, s % 8 == 2, LOW
Zone(TEXT("World s1d1, The Strip, street mouths, slots 28 to 35"), 1.2451f, 1.6052f);
	Site(TEXT("WldS1D1MouthLow"), 1, 22.5f, 4, 0, 1.f);   // town, s % 4 == 0, LOW: between the kerb's cars (18.6) and the promenade (26)
	Site(TEXT("WldS1D1MouthTall"), 1, 25.0f, 4, 0, 1.f);   // TALL: 25 m
	Scatter(S1D1("Bench"), 21.0f, 21.8f, 8, 2, 1.f, 1.f, 0.f, 1, 0.f);   // town, s % 8 == 6, LOW
	Scatter(S1D1("Bollards"), 21.0f, 21.8f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);   // town, s % 8 == 2, LOW
Zone(TEXT("World s1d1, The Strip, street mouths, slots 37 to 49"), 1.6523f, 2.2386f);
	Site(TEXT("WldS1D1MouthLow"), 1, 22.5f, 4, 0, 1.f);   // town, s % 4 == 0, LOW: between the kerb's cars (18.6) and the promenade (26)
	Site(TEXT("WldS1D1MouthTall"), 1, 25.0f, 4, 0, 1.f);   // TALL: 25 m
	Scatter(S1D1("Bench"), 21.0f, 21.8f, 8, 2, 1.f, 1.f, 0.f, 1, 0.f);   // town, s % 8 == 6, LOW
	Scatter(S1D1("Bollards"), 21.0f, 21.8f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);   // town, s % 8 == 2, LOW
Zone(TEXT("World s1d1, The Strip, street mouths, slots 58 to 70"), 2.6024f, 3.1887f);
	Site(TEXT("WldS1D1MouthLow"), 1, 22.5f, 4, 0, 1.f);   // town, s % 4 == 0, LOW: between the kerb's cars (18.6) and the promenade (26)
	Site(TEXT("WldS1D1MouthTall"), 1, 25.0f, 4, 0, 1.f);   // TALL: 25 m
	Scatter(S1D1("Bench"), 21.0f, 21.8f, 8, 2, 1.f, 1.f, 0.f, 1, 0.f);   // town, s % 8 == 6, LOW
	Scatter(S1D1("Bollards"), 21.0f, 21.8f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);   // town, s % 8 == 2, LOW
Zone(TEXT("World s1d1, The Strip, the open boardwalk, slots 4 to 4"), 0.1593f, 0.2027f);
	Site(TEXT("WldS1D1BeachA"), -1, 31.0f, 4, 0, 1.f);   // bay, s % 4 == 0
	Site(TEXT("WldS1D1BeachB"), -1, 31.0f, 4, 2, 1.f);   // bay, s % 4 == 2
Zone(TEXT("World s1d1, The Strip, the open boardwalk, slots 12 to 13"), 0.5212f, 0.6099f);
	Site(TEXT("WldS1D1BeachA"), -1, 31.0f, 4, 0, 1.f);   // bay, s % 4 == 0
	Site(TEXT("WldS1D1BeachB"), -1, 31.0f, 4, 2, 1.f);   // bay, s % 4 == 2
Zone(TEXT("World s1d1, The Strip, the open boardwalk, slots 15 to 21"), 0.6569f, 0.9718f);
	Site(TEXT("WldS1D1BeachA"), -1, 31.0f, 4, 0, 1.f);   // bay, s % 4 == 0
	Site(TEXT("WldS1D1BeachB"), -1, 31.0f, 4, 2, 1.f);   // bay, s % 4 == 2
Zone(TEXT("World s1d1, The Strip, the open boardwalk, slots 28 to 29"), 1.2451f, 1.3338f);
	Site(TEXT("WldS1D1BeachA"), -1, 31.0f, 4, 0, 1.f);   // bay, s % 4 == 0
	Site(TEXT("WldS1D1BeachB"), -1, 31.0f, 4, 2, 1.f);   // bay, s % 4 == 2
Zone(TEXT("World s1d1, The Strip, the open boardwalk, slots 31 to 35"), 1.3808f, 1.6052f);
	Site(TEXT("WldS1D1BeachA"), -1, 31.0f, 4, 0, 1.f);   // bay, s % 4 == 0
	Site(TEXT("WldS1D1BeachB"), -1, 31.0f, 4, 2, 1.f);   // bay, s % 4 == 2
Zone(TEXT("World s1d1, The Strip, the open boardwalk, slots 58 to 61"), 2.6024f, 2.7815f);
	Site(TEXT("WldS1D1BeachA"), -1, 31.0f, 4, 0, 1.f);   // bay, s % 4 == 0
	Site(TEXT("WldS1D1BeachB"), -1, 31.0f, 4, 2, 1.f);   // bay, s % 4 == 2
Zone(TEXT("World s1d1, The Strip, the open boardwalk, slots 68 to 70"), 3.0548f, 3.1887f);
	Site(TEXT("WldS1D1BeachA"), -1, 31.0f, 4, 0, 1.f);   // bay, s % 4 == 0
	Site(TEXT("WldS1D1BeachB"), -1, 31.0f, 4, 2, 1.f);   // bay, s % 4 == 2
#undef S1D1
// ---- END s1d1 ----
}
{
// ---- BEGIN s1d2 (district 2 "The Marina", ft_world_bay_s1d2.py) ----
// 15 meshes: 7 buildings, 7 props, the hero. Every rule is a certain Site (chance 1, no jitter) in a Zone of ONE slot, so the
// count is the sum of the Sets' parts: 46 Sites in 25 slots = 241 placed if the track leaves none out (by the brief's share of
// 0.75: about 181). Most of one mesh: RopeFence 34. The Clipper: 2 (slots 82 and 128: 3.71 and 5.79 km).
// Slots: even ones only, 71 to 141, without the jumps' and the loop's (75-81, 94-100, 117-123).
//   s % 4 == 0  town side: a Set of three buildings and four props on the two corners of the side street (anchor 54 m: buildings
//               44.9 to 69.4 m, nothing within 6 m of the street's middle; props 42 m, edges 40.1 m). Bay side: two buildings and their yard on
//               the sand (anchor 56 m: 46.4 to 61 m; not in 104, 124, 128, 136: the level ground's boat row or lido), and the
//               quay's things behind the boardwalk (anchor 31 m: 26.2 to 37.8 m; not in 84, 88, 104: banked bends).
//   s % 4 == 2  bay side only: one building and its yard 44.5 m out (40.2 to 48.4 m), behind the arcade's slot.
// TALL pieces: the nearest rule is the quay Set's anchor (anchor x + out = 30 m; its edge 28.1 m). LOW ones (Dinghies,
// LobsterPots, RopeFence) from 26.5 m. Glow (steady): ClubHouse (47.5 m and more) and Clipper (152 m) only.
#define S1D2(n) TEXT("/Game/World/Bayshore/SM_S1D2_" n)
Set(TEXT("WldS1D2TownA"), {      // town, a side street's two corners: the harbour office and the chandlery, the pilot's cottage behind; the quay's things 42 m out
	{ S1D2("HarbourOffice"), -15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("Chandlery"), 15.5f, 0.f, 0.f, 0.f, 1.f },   // fix round: every piece of the three town Sets now begins 8.5 m and more from the street's middle (the walkers' lanes end at 8.4)
	{ S1D2("PilotCottage"), -15.f, 11.5f, 0.f, 0.f, 1.f },
	{ S1D2("SignalMast"), 12.8f, -12.f, 0.f, 0.f, 1.f },
	{ S1D2("RopeFence"), -13.f, -12.f, 0.f, 0.f, 1.f },
	{ S1D2("LobsterPots"), 19.8f, -12.f, 0.f, 0.f, 1.f },
	{ S1D2("Anchor"), -21.f, -12.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2TownB"), {      // town: the club house and the sail loft, the smokehouse behind
	{ S1D2("ClubHouse"), -16.4f, 0.f, 0.f, 0.f, 1.f },   // fix round: as WldS1D2TownA
	{ S1D2("SailLoft"), 14.9f, 1.f, 0.f, 0.f, 1.f },
	{ S1D2("Smokehouse"), 14.5f, 11.5f, 0.f, 0.f, 1.f },
	{ S1D2("ShipsBell"), -10.2f, -12.f, 0.f, 0.f, 1.f },
	{ S1D2("DryDinghy"), 11.2f, -12.f, 0.f, 0.f, 1.f },
	{ S1D2("RopeFence"), 18.5f, -12.f, 0.f, 0.f, 1.f },
	{ S1D2("Dinghies"), -15.8f, -12.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2TownC"), {      // town: the boat shed and the harbour office, the chandlery behind
	{ S1D2("BoatShed"), -16.5f, 1.f, 0.f, 0.f, 1.f },   // fix round: as WldS1D2TownA
	{ S1D2("HarbourOffice"), 15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("Chandlery"), 14.5f, 11.5f, 0.f, 0.f, 1.f },
	{ S1D2("Anchor"), 10.6f, -12.f, 0.f, 0.f, 1.f },
	{ S1D2("LobsterPots"), -11.0f, -12.f, 0.f, 0.f, 1.f },
	{ S1D2("Dinghies"), 17.f, -12.f, 0.f, 0.f, 1.f },
	{ S1D2("ShipsBell"), -20.f, -12.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2BayA"), {      // bay, on the sand 56 m out: the boat shed and the club house
	{ S1D2("BoatShed"), -12.5f, 2.f, 0.f, 0.f, 1.f },
	{ S1D2("ClubHouse"), 7.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("DryDinghy"), 3.f, -8.5f, 0.f, 0.f, 1.f },
	{ S1D2("LobsterPots"), 10.f, -8.5f, 0.f, 0.f, 1.f },
	{ S1D2("RopeFence"), 17.5f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2BayB"), {      // bay: the pilot's cottage and the sail loft
	{ S1D2("PilotCottage"), -10.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("SailLoft"), 10.5f, 1.f, 0.f, 0.f, 1.f },
	{ S1D2("Anchor"), 2.f, -8.f, 0.f, 0.f, 1.f },
	{ S1D2("Dinghies"), -12.f, -8.f, 0.f, 0.f, 1.f },
	{ S1D2("RopeFence"), 12.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("ShipsBell"), 20.5f, -7.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2BayC"), {      // bay: the club house and the harbour office
	{ S1D2("ClubHouse"), -11.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("HarbourOffice"), 9.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("LobsterPots"), -16.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("DryDinghy"), -6.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("Dinghies"), 6.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("RopeFence"), 16.f, -9.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2YardD"), {      // bay, 44.5 m out behind the arcade's slot: the smokehouse and its yard
	{ S1D2("Smokehouse"), -9.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("LobsterPots"), 3.f, -1.f, 0.f, 0.f, 1.f },
	{ S1D2("ShipsBell"), 8.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("RopeFence"), 16.f, -2.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2YardE"), {      // bay: the pilot's cottage and its yard
	{ S1D2("PilotCottage"), 8.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("DryDinghy"), -4.f, -1.f, 0.f, 0.f, 1.f },
	{ S1D2("Anchor"), -10.5f, -1.f, 0.f, 0.f, 1.f },
	{ S1D2("Dinghies"), -17.f, -1.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2Quay"), {      // bay, on the sand right behind the boardwalk (26.2 to 37.8 m), on the side away from the watchers' stand
	{ S1D2("SignalMast"), -11.f, 1.f, 0.f, 0.f, 1.f },
	{ S1D2("Anchor"), -3.5f, -1.f, 0.f, 0.f, 1.f },
	{ S1D2("RopeFence"), -13.f, -4.5f, 0.f, 0.f, 1.f },
	{ S1D2("LobsterPots"), -19.f, 1.5f, 0.f, 0.f, 1.f },
	{ S1D2("Dinghies"), -6.f, 6.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2Clipper"), {      // THE CLIPPER, bay side, 152 m out
	{ S1D2("Clipper"), 0.f, 0.f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1d2, the Marina, slot 72"), 3.23487f, 3.28012f);
	Site(TEXT("WldS1D2TownA"), 1, 54.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2BayA"), -1, 56.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2Quay"), -1, 31.f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 74"), 3.32536f, 3.3706f);
	Site(TEXT("WldS1D2YardD"), -1, 44.5f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 82"), 3.6873f, 3.73255f);
	Site(TEXT("WldS1D2YardD"), -1, 44.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D2Clipper"), -1, 152.f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 84"), 3.77779f, 3.82303f);
	Site(TEXT("WldS1D2TownA"), 1, 54.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2BayA"), -1, 56.f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 86"), 3.86828f, 3.91352f);
	Site(TEXT("WldS1D2YardE"), -1, 44.5f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 88"), 3.95876f, 4.00401f);
	Site(TEXT("WldS1D2TownB"), 1, 54.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2BayB"), -1, 56.f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 90"), 4.04925f, 4.09449f);
	Site(TEXT("WldS1D2YardD"), -1, 44.5f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 92"), 4.13973f, 4.18498f);
	Site(TEXT("WldS1D2TownC"), 1, 54.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2BayC"), -1, 56.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2Quay"), -1, 31.f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 102"), 4.59216f, 4.63741f);
	Site(TEXT("WldS1D2YardE"), -1, 44.5f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 104"), 4.68265f, 4.72789f);
	Site(TEXT("WldS1D2TownC"), 1, 54.f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 106"), 4.77314f, 4.81838f);
	Site(TEXT("WldS1D2YardD"), -1, 44.5f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 108"), 4.86362f, 4.90887f);
	Site(TEXT("WldS1D2TownA"), 1, 54.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2BayA"), -1, 56.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2Quay"), -1, 31.f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 110"), 4.95411f, 4.99935f);
	Site(TEXT("WldS1D2YardE"), -1, 44.5f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 112"), 5.04459f, 5.08984f);
	Site(TEXT("WldS1D2TownB"), 1, 54.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2BayB"), -1, 56.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2Quay"), -1, 31.f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 114"), 5.13508f, 5.18032f);
	Site(TEXT("WldS1D2YardD"), -1, 44.5f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 116"), 5.22557f, 5.27081f);
	Site(TEXT("WldS1D2TownC"), 1, 54.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2BayC"), -1, 56.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2Quay"), -1, 31.f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 124"), 5.58751f, 5.63275f);
	Site(TEXT("WldS1D2TownB"), 1, 54.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2Quay"), -1, 31.f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 126"), 5.678f, 5.72324f);
	Site(TEXT("WldS1D2YardE"), -1, 44.5f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 128"), 5.76848f, 5.81373f);
	Site(TEXT("WldS1D2TownC"), 1, 54.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2Quay"), -1, 31.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2Clipper"), -1, 152.f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 130"), 5.85897f, 5.90421f);
	Site(TEXT("WldS1D2YardD"), -1, 44.5f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 132"), 5.94945f, 5.9947f);
	Site(TEXT("WldS1D2TownA"), 1, 54.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2BayA"), -1, 56.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2Quay"), -1, 31.f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 134"), 6.03994f, 6.08518f);
	Site(TEXT("WldS1D2YardE"), -1, 44.5f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 136"), 6.13043f, 6.17567f);
	Site(TEXT("WldS1D2TownB"), 1, 54.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2Quay"), -1, 31.f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 138"), 6.22091f, 6.26616f);
	Site(TEXT("WldS1D2YardD"), -1, 44.5f, 1, 0, 1.f);
Zone(TEXT("World s1d2, the Marina, slot 140"), 6.3114f, 6.35664f);
	Site(TEXT("WldS1D2TownC"), 1, 54.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2BayC"), -1, 56.f, 1, 0, 1.f);
	Site(TEXT("WldS1D2Quay"), -1, 31.f, 1, 0, 1.f);
#undef S1D2
// ---- END s1d2 ----
}
{
// ---- BEGIN s1d3 (district 3 "Pleasure Beach", 6.4 to 9.8 km, ft_world_bay_s1d3.py) ----
// The fairground: 16 meshes /Game/World/Bayshore/SM_S1D3_* (8 buildings, 7 props, 1 hero), all ft_people Scenes, one section each.
// The zone has slots 142 to 216 (75). This block uses EVEN slots only (s % 4 == 0 or 2). What stands there already, and what is used:
//   TOWN  s % 4 == 0: a side street down the slot's middle (5.1 m either way, 18 to 162 m out), the film crew and the stages at its
//         mouth (21.9 to 32.3 m), the watchers' terrace 38 m out over it (s % 8 == 0), walkers on its pavements 7.5 m either side,
//         37 to 59 m (s % 8 == 4). USED: two buildings flanking the street, 16.5 m either side of its middle, 57 m out (50 to 64 m);
//         in s % 8 == 4 two ribbon masts 10 m either side, 36.5 m out (behind the promenade, 26 to 34 m, and its trees at 32.9 m).
//   TOWN  s % 4 == 2: planters at the kerb (16.7 to 19.2 m, 11 m either way), a drinks kiosk 14 m back / 25.5 m out, flags 6 to 18 m
//         on / 27.5 m out, the shop row 29.3 to 39 m, the pool deck 41 to 63 m, the shop block from 77.5 m. USED: the kerb beyond the
//         planters' ends (18.5 m either way, 18.8 m out: low pieces), the pavement in front of the shop row (27 m out: tall props at
//         -20.5, -5 and +20.4 m along), and the strip behind the pool (69.5 m out: 63 to 77 m).
//   BAY   even slots: the boardwalk to 24.1 m, the first cast and the watchers 26 to 33 m, the beach club 34 m (its path 6 m on, its
//         huts 14 m back / 48 m out, sunbeds 15 m on / 54 m out), the scenes 38.3 to 46.7 m, the chair lift 44 m, the heights
//         pieces 68 to 74 m in s % 8 == 2, 4, 6. USED: 15 m either side of the middle, 61.5 m out (57 to 66 m), and 63.5 m out in
//         s % 8 == 0 (56 to 70 m: no heights piece there).
//   HERO  THE BIG SUNDAE, bay side, 125 m out (111.5 to 138.5 m), in slots 152, 176 and 200 (s % 8 == 0: the funfair's coaster and
//         carousels are anchored in s % 8 == 3 and reach two slots back and one on, the groynes are in s % 8 == 6, the piers in
//         s % 12 == 11; the far crowd s % 7 == 6, the kites s % 11 == 7 and the beach race / sand train lines end 20 m short).
// Beside the steep banked bend (7.99 to 8.23 km, slots 177 to 182) nothing of this block stands nearer than 40 m: the rules for
// the kerb, the pavement and the ribbon masts are written twice, for 6.4 to 7.97 km and for 8.25 to 9.8 km.
// Light: steady glow only on FunHouse, DodgemPavilion, PennyPalace, CandyFloss (57 m out and more) and the Sundae (125 m). Nothing blinks.
// Estimate (sites x parts x 0.75 left out by the track; every chance is 1; the three hero sites counted whole):
//   flanking the side streets  GateA 10 slots x 2 + GateB 9 x 2             = 38 x 0.75 = 28.5
//   behind the pools           PoolBackA 10 x 2 + PoolBackB 9 x 2           = 38 x 0.75 = 28.5
//   bay, 61.5 and 63.5 m       BayBig 10 x 2 + BayKioskA 9 x 2 + BayKioskB 10 x 2 = 58 x 0.75 = 43.5
//   pavement (two zones)       StreetA 9 x 3 + StreetB 8 x 3                = 51 x 0.75 = 38.25
//   ribbon masts (two zones)   StreetGate 8 x 2                             = 16 x 0.75 = 12
//   kerb (two zones)           Kerb 17 x 2                                  = 34 x 0.75 = 25.5
//   hero                                                                                  3
//   SUM about 179 placed. Most of one mesh: PuppetTheatre about 22 (29 before the track's share), CandyFloss about 21.
//   Smallest x min of a TALL mesh: 27 m (the pavement props); of a low mesh: 18.8 m.
#define S1D3(n) TEXT("/Game/World/Bayshore/SM_S1D3_" n)
Set(TEXT("WldS1D3GateA"), {                        // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D3("FunHouse"),       -16.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("MirrorHall"),      16.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3GateB"), {
	{ S1D3("DodgemPavilion"), -16.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("PennyPalace"),     16.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3PoolBackA"), {
	{ S1D3("DuckGallery"),    -11.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("PuppetTheatre"),   10.f,  0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3PoolBackB"), {
	{ S1D3("BigTop"),         -11.f,  1.f, 0.f, 0.f, 1.f },
	{ S1D3("CandyFloss"),      10.f,  0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3BayBig"), {
	{ S1D3("BigTop"),         -15.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D3("PennyPalace"),     15.f,  0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3BayKioskA"), {
	{ S1D3("CandyFloss"),     -15.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D3("PuppetTheatre"),   15.f,  0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3BayKioskB"), {
	{ S1D3("PuppetTheatre"),  -15.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D3("CandyFloss"),      15.f,  0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3StreetA"), {                      // TALL props on the pavement, in front of the set-back shop row
	{ S1D3("TicketDrum"),     -20.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("StrengthTower"),   -5.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D3("Lollipops"),       20.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3StreetB"), {
	{ S1D3("Lollipops"),      -20.4f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("PrizeStall"),      -5.f,  0.f, 0.f, 0.f, 1.f },
	// fix round: the ribbon mast that stood here (20.5 m along) is gone: behind the rocking horses at the kerb the two read as one heap (the driver, 7.71 km)
});
Set(TEXT("WldS1D3StreetGate"), {                   // a ribbon mast either side of a side street
	{ S1D3("BuntingMast"),    -10.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D3("BuntingMast"),     10.f,  0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3Kerb"), {                         // LOW pieces at the kerb, beyond the planters' two ends
	{ S1D3("MirrorRow"),      -18.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("RockingHorses"),   18.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3Sundae"), { { S1D3("BigSundae"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Zone(TEXT("World s1d3, Pleasure Beach"), 6.4f, 9.8f);
	Site(TEXT("WldS1D3GateA"), 1, 57.f, 8, 0, 1.f);              // town, s % 8 == 0: the fun house and the hall of mirrors
	Site(TEXT("WldS1D3GateB"), 1, 57.f, 8, 4, 1.f);              // town, s % 8 == 4: the dodgems and the penny arcade
	Site(TEXT("WldS1D3PoolBackA"), 1, 69.5f, 8, 2, 1.f);         // town, s % 8 == 6: behind the pool deck
	Site(TEXT("WldS1D3PoolBackB"), 1, 69.5f, 8, 6, 1.f);         // town, s % 8 == 2
	Site(TEXT("WldS1D3BayBig"), -1, 63.5f, 8, 0, 1.f);           // bay, s % 8 == 0: the big top and a penny arcade on the sand
	Site(TEXT("WldS1D3BayKioskA"), -1, 61.5f, 8, 4, 1.f);        // bay, s % 8 == 4
	Site(TEXT("WldS1D3BayKioskB"), -1, 61.5f, 8, 2, 1.f);        // bay, s % 8 == 6
Zone(TEXT("World s1d3, Pleasure Beach, by the road, before the bend"), 6.4f, 7.97f);     // slots 142 to 176
	Site(TEXT("WldS1D3StreetA"), 1, 27.f, 8, 2, 1.f);            // town, s % 8 == 6
	Site(TEXT("WldS1D3StreetB"), 1, 27.f, 8, 6, 1.f);            // town, s % 8 == 2
	Site(TEXT("WldS1D3StreetGate"), 1, 36.5f, 8, 4, 1.f);        // town, s % 8 == 4
	Site(TEXT("WldS1D3Kerb"), 1, 18.8f, 4, 2, 1.f);              // town, s % 4 == 2
Zone(TEXT("World s1d3, Pleasure Beach, by the road, after the bend"), 8.25f, 9.21f);     // slots 183 to 203 (fix round: was to 9.8; the cliff road's banked esses begin at 9.25 km)
	Site(TEXT("WldS1D3StreetA"), 1, 27.f, 8, 2, 1.f);
	Site(TEXT("WldS1D3StreetB"), 1, 27.f, 8, 6, 1.f);
	Site(TEXT("WldS1D3StreetGate"), 1, 36.5f, 8, 4, 1.f);
	Site(TEXT("WldS1D3Kerb"), 1, 18.8f, 4, 2, 1.f);
Zone(TEXT("World s1d3, the Big Sundae"), 6.85431f, 6.89956f);    // slot 152, 6.88 km: level, the last slot before the loop
	Site(TEXT("WldS1D3Sundae"), -1, 125.f, 1, 0, 1.f);
Zone(TEXT("World s1d3, the Big Sundae"), 7.94015f, 7.98539f);    // slot 176, 7.96 km: where the road turns towards the bay
	Site(TEXT("WldS1D3Sundae"), -1, 125.f, 1, 0, 1.f);
Zone(TEXT("World s1d3, the Big Sundae"), 9.02598f, 9.07122f);    // slot 200, 9.05 km: level, past the jump
	Site(TEXT("WldS1D3Sundae"), -1, 125.f, 1, 0, 1.f);
#undef S1D3
// ---- END s1d3 ----
}
{
// ---- BEGIN s1d4 (district 4 "Hotel Row", 9.8 to 13.2 km, ft_world_bay_s1d4.py) ----
// 20 meshes: 11 buildings, 8 props, 1 hero (THE SHELL). Mint palaces with gold domes, everything in arches.
// Estimate (the brief's: slots / every x chance x 0.75 x parts), about 203 placed:
//   whole district, 75.1 slots, seven Sites every 8, 12 building parts:      75.1 / 8 x 0.75 x 12 = 84.5
//   off the bends and the crew's slots, 62 slots (S1D4Near, five stretches):
//     WldS1D4Kerb every 4, 4 parts: 62 / 4 x 0.75 x 4 = 46.5;  WldS1D4CourtA and CourtB every 8, 4 parts each: 46.5;
//     WldS1D4LawnA and LawnB every 8, 2 parts each: 23.3                                                  = 116.3
//   the Shell: 2 (slots 236 and 266 (268 before the fix round), one certain Site each)
//   Most of one mesh: Balustrade 23.3, Lamp 23.3, Gatehouse 14.1; every building 7.0; the Shell 2.
// Slots and bands used (only even slots; never s % 4 == 1 or 3):
//   town, s % 4 == 0 (the side street's slot; the street lies 4.8 m either side of the slot's middle, so every piece
//     stands 6 m and more along from it): low props 22.3 to 25.8 m (between the kerb's cars and the promenade);
//     lamps 36 m, tall props 35.9 to 42.5 m (behind the promenade, either side of PplWatchTerrace / the street);
//     gatehouses 42.9 to 48.9 m (s % 8 == 4); buildings 54.4 to 68.4 m (two flank the street).
//   town, s % 4 == 2 (the cafe's slot): two hotels 39.4 to 55.3 m, either side of the cafe (which is 7.7 m each way).
//   bay, s % 4 == 0: one beach building 60.2 to 71.1 m (behind SM_BeachClub's deck, which ends at 56 m).
//   bay, s % 4 == 2: tall props 36.1 to 43.3 m (between the watch stands at 30 m and the lido at 44 m).
//   the Shell: bay side, anchor 142 m (110.7 to 157.5 m), slots 236 and 266 (268 before the fix round): no P3C sand train (s % 4 == 2), beach
//     race (s % 5 == 4), FARP crowd (s % 15 == 8), flock (s % 15 == 4) or kites (s % 22 == 8) and none of the
//     level's own Ferris wheels (s % 23 == 16) or lighthouses (s % 29 == 16) in them. Not the town side: the
//     level's own hotels fill 100 to 155 m there in every slot (FTTrack.cpp, 1100 to 1700 units, chance 0.7).
// Smallest x min of a TALL mesh: 36 m (Lamp). Glow (steady) only on meshes whose every placement is 36 m and more out.
#define S1D4(n) TEXT("/Game/World/Bayshore/SM_S1D4_" n)
Set(TEXT("WldS1D4GrandA"), {                       // { mesh, along m, out m, up m, yaw deg, scale }: the street runs between them
	{ S1D4("Ballroom"),   -14.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("Casino"),      15.5f, 0.f, 0.f, 0.f, 1.f },   // its turret on the street's corner
});
Set(TEXT("WldS1D4GrandB"), {
	{ S1D4("PalmCourt"),  -14.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("RoofGarden"),  14.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Lodge"), {                        // a gatehouse either side of the street's mouth
	{ S1D4("Gatehouse"),  -14.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("Gatehouse"),   14.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4HotelsA"), {                      // either side of the cafe, over the shopfronts' roofs
	{ S1D4("GrandHotel"), -15.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("Belvedere"),   15.2f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4HotelsB"), {
	{ S1D4("Loggia"),     -15.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("Rotunda"),     15.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Spa"),  { { S1D4("SpaHouse"),     0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D4Bath"), { { S1D4("BathPavilion"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D4Kerb"), {                         // LOW, anchor 22.6 m: on the pavement at the street's mouth
	{ S1D4("Balustrade"), -17.5f, 0.f,  0.f, 0.f, 1.f },   // fix round: the balustrades 2.5 m further apart, the tables and the trolley in their line (22.6 m):
	{ S1D4("Balustrade"),  17.5f, 0.f,  0.f, 0.f, 1.f },   // at 24.5 m they stood under the film crew's units of slots 240, 264, 268, 276 (26 m, from 24.55 m)
	{ S1D4("Trolley"),      9.f,  0.f,  0.f, 0.f, 1.f },
	{ S1D4("TeaTables"),   -9.5f, 0.f,  0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4CourtA"), {                       // TALL, anchor 36 m (the smallest out is 0)
	{ S1D4("Lamp"),        -8.5f, 0.f,  0.f, 0.f, 1.f },
	{ S1D4("Lamp"),         8.5f, 0.f,  0.f, 0.f, 1.f },
	{ S1D4("Fountain"),   -15.f,  3.2f, 0.f, 0.f, 1.f },
	{ S1D4("Bandstand"),   15.5f, 3.4f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4CourtB"), {
	{ S1D4("Lamp"),        -8.5f, 0.f,  0.f, 0.f, 1.f },
	{ S1D4("Lamp"),         8.5f, 0.f,  0.f, 0.f, 1.f },
	{ S1D4("Valet"),      -13.5f, 3.f,  0.f, 0.f, 1.f },
	{ S1D4("Flagpoles"),   15.5f, 2.f,  0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4LawnA"), {                        // TALL, anchor 40 m, bay side
	{ S1D4("Bandstand"),  -19.f,  0.f,  0.f, 0.f, 1.f },   // fix round: was -16, in the third pass's beach scenes (42.5 m out, 14.7 to 1.3 m before the middle)
	{ S1D4("Flagpoles"),   15.f,  0.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4LawnB"), {
	{ S1D4("Fountain"),   -19.f,  0.f,  0.f, 0.f, 1.f },   // fix round: as WldS1D4LawnA
	{ S1D4("Valet"),       16.f,  0.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Shell"), { { S1D4("Shell"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Zone(TEXT("World s1d4, Hotel Row"), 9.8f, 13.2f);                               // everything that stands 40 m and more out
	Site(TEXT("WldS1D4GrandA"),  1, 62.f,  8, 0, 1.f);                              // town, s % 8 == 0
	Site(TEXT("WldS1D4GrandB"),  1, 62.f,  8, 4, 1.f);                              // town, s % 8 == 4
	Site(TEXT("WldS1D4Lodge"),   1, 46.f,  8, 4, 1.f);                              // town, s % 8 == 4
	Site(TEXT("WldS1D4HotelsA"), 1, 48.5f, 8, 6, 1.f);                              // town, s % 8 == 2
	Site(TEXT("WldS1D4HotelsB"), 1, 48.5f, 8, 2, 1.f);                              // town, s % 8 == 6
	Site(TEXT("WldS1D4Spa"),    -1, 66.f,  8, 0, 1.f);                              // bay, s % 8 == 0
	Site(TEXT("WldS1D4Bath"),   -1, 66.f,  8, 4, 1.f);                              // bay, s % 8 == 4
// What stands nearer than 40 m: not beside the banked bends (the cliff road's esses to 10.02 km, the steep bend
// 10.98 to 11.21 km: slots 217 to 221 and 243 to 247) and not in the film crew's town slots 228, 252 and 280
// (PplShootB / C / A at 21.9 m). The numbers are slot edges, (s - 0.5) x 0.045243 km, as the table writes them.
auto S1D4Near = [&](float FromKm, float ToKm)
{
	Zone(TEXT("World s1d4, Hotel Row, by the road"), FromKm, ToKm);
	Site(TEXT("WldS1D4Kerb"),    1, 22.6f, 4, 0, 1.f);                              // town, s % 4 == 0: low
	Site(TEXT("WldS1D4CourtA"),  1, 36.f,  8, 0, 1.f);                              // town, s % 8 == 0
	Site(TEXT("WldS1D4CourtB"),  1, 36.f,  8, 4, 1.f);                              // town, s % 8 == 4
	Site(TEXT("WldS1D4LawnA"),  -1, 40.f,  8, 6, 1.f);                              // bay, s % 8 == 2
	Site(TEXT("WldS1D4LawnB"),  -1, 40.f,  8, 2, 1.f);                              // bay, s % 8 == 6
};
S1D4Near(10.02132f, 10.29278f);     // slots 222 to 227
S1D4Near(10.33803f, 10.97143f);     // slots 229 to 242
S1D4Near(11.19764f, 11.37861f);     // slots 248 to 251
S1D4Near(11.42386f, 12.64542f);     // slots 253 to 279
S1D4Near(12.69066f, 13.2f);         // slots 281 to 291
Zone(TEXT("World s1d4, the Shell"), 10.65473f, 10.69997f);                      // slot 236
	Site(TEXT("WldS1D4Shell"), -1, 142.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, the Shell"), 12.01202f, 12.05726f);                      // slot 266 (fix round: was 268, where the beach race of slot 269 ran through it)
	Site(TEXT("WldS1D4Shell"), -1, 142.f, 1, 0, 1.f);
#undef S1D4
// ---- END s1d4 ----
}
{
// ---- BEGIN s1d5 (district 5 "The Point", ft_world_bay_s1d5.py) ----
// 19 meshes (8 buildings, 10 props, the hero). Estimate by the brief's formula (slots / every x chance x 0.75 x parts):
//   whole district (80 slots): 4 back-row houses x 80/12 x 0.75 = 20; Bluff 4 parts x 80/8 x 0.75 = 30; CubeHouse 80/8 x 0.75 = 7.5;
//   Cairn 80/4 x 0.8 x 0.75 = 12; by the road (4 zones, 59 slots, every 8): (5 Sets x 3 parts + 13 Scatters) = 28 pieces x 59/8 x 0.75 = 154.9;
//   Arch Rock 3 (three zones of one slot)  =  about 227 placed. Counted slot by slot without the set pieces' slots: about 199.
//   Most of one mesh (formula): WashLine 24, Dovecote 22, Cairn 20. Smallest x min of a TALL mesh: 27 m (FingerPost).
// Bands and slots: even slots only (s % 4 == 0 or 2). Town: LOW 24.0-24.4 m; wall line 36-38 m (behind the film crew at 26-34 m and the
//   shop fronts at 27.5 m); back row 69-70 m in s % 12 == 2, 4, 8, 10 (behind the pools and pool decks, not the courts). Bay: LOW 25.0-27.0 m;
//   Sets at 30 m in s % 4 == 0 (the shoot stands at 31 m in s % 4 == 2); 50-69 m. Arch Rock at 156 m in slots 312, 340, 368.
#define S1D5(n) TEXT("/Game/World/Bayshore/SM_S1D5_" n)
Set(TEXT("WldS1D5ArchRock"), {      // the hero, alone
	{ S1D5("ArchRock"), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5Bluff"), {      // bay, 52 to 69 m: houses stepping down to the beach
	{ S1D5("StepRow"), -4.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("Pavilion"), 13.0f, 8.5f, 0.0f, 0.0f, 1.0f },
	{ S1D5("Cairn"), -17.0f, 9.0f, 0.0f, 25.0f, 1.0f },
	{ S1D5("WashLine"), 9.0f, -6.0f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5WallA"), {      // town, 38 m: the garden wall and its gate, behind the film crew
	{ S1D5("GateHouse"), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("Dovecote"), -13.5f, 0.5f, 0.0f, 0.0f, 1.0f },
	{ S1D5("WashLine"), 14.5f, 0.5f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5WallB"), {      // town, 38 m: the stepped row
	{ S1D5("StepRow"), -4.0f, 1.9f, 0.0f, 0.0f, 1.0f },   // fix round: was out -0.3, in the film crane of slot 308 (to 36.2 m)
	{ S1D5("CubeHouse"), 12.5f, 0.2f, 0.0f, 0.0f, 1.0f },
	{ S1D5("FingerPost"), 6.8f, -3.2f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5Yard"), {      // town, 36 m: a yard behind the shop fronts, at the near end of the row
	{ S1D5("CubeHouse"), -14.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("WashLine"), -4.0f, 1.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("Dovecote"), 3.5f, 0.5f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5ShoreA"), {      // bay, 28 to 38 m: the pavilion over the beach
	{ S1D5("Pavilion"), 0.0f, 3.5f, 0.0f, 0.0f, 1.0f },
	{ S1D5("Dovecote"), -12.0f, 3.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("WashLine"), 12.0f, 4.0f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5ShoreB"), {      // bay, 29 to 33 m: a walled garden on the shore
	{ S1D5("GateHouse"), 0.0f, 2.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("Dovecote"), 13.5f, 2.5f, 0.0f, 0.0f, 1.0f },
	{ S1D5("FingerPost"), -5.0f, -0.9f, 0.0f, 0.0f, 1.0f },
});
// 40 m and more from the road: the whole district, its banked bends too (13.38-13.64, 14.00-14.41, 15.29-15.53 km)
Zone(TEXT("World s1d5, The Point"), 13.2f, 16.8f);
	Scatter(S1D5("Windmill"), 70.0f, 70.0f, 12, 10, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, behind the small pools, s % 12 == 2
	Scatter(S1D5("TowerHouse"), 69.0f, 70.0f, 12, 4, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, behind the small pools, s % 12 == 8
	Scatter(S1D5("DomeVilla"), 69.0f, 70.0f, 12, 8, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, behind the pool decks, s % 12 == 4
	Scatter(S1D5("TerraceVilla"), 69.0f, 70.0f, 12, 2, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, behind the pool decks, s % 12 == 10
	Site(TEXT("WldS1D5Bluff"), -1, 56.0f, 8, 0, 1.0f);   // bay, s % 8 == 0
	Scatter(S1D5("CubeHouse"), 51.0f, 54.0f, 8, 4, 1.0f, 1.0f, 0.0f, -1, 6.0f);   // bay, above the sand, s % 8 == 4
	Scatter(S1D5("Cairn"), 50.0f, 58.0f, 4, 2, 0.8f, 1.0f, 0.12f, -1, 180.0f);   // bay, on the beach, s % 4 == 2
// nearer than 40 m: slots 292 to 295 only (beside a banked bend the land 17 to 40 m out is a cutting)
Zone(TEXT("World s1d5, The Point, by the road 1"), 13.2f, 13.36931f);
	Site(TEXT("WldS1D5WallA"), 1, 38.0f, 8, 0, 1.0f);   // town, s % 8 == 0
	Site(TEXT("WldS1D5WallB"), 1, 38.0f, 8, 4, 1.0f);   // town, s % 8 == 4
	Scatter(S1D5("TowerHouse"), 35.5f, 36.5f, 8, 6, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, its top over the shop fronts, s % 8 == 2
	Site(TEXT("WldS1D5Yard"), 1, 36.0f, 8, 2, 1.0f);   // town, s % 8 == 6
	Scatter(S1D5("JarRow"), 24.0f, 24.4f, 8, 0, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, LOW, the back edge of the pavement, s % 8 == 0
	Scatter(S1D5("Exedra"), 24.2f, 24.4f, 8, 4, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, LOW, s % 8 == 4
	Scatter(S1D5("Sundial"), 24.0f, 24.4f, 8, 6, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, LOW, s % 8 == 2
	Scatter(S1D5("Well"), 24.0f, 24.4f, 8, 2, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, LOW, s % 8 == 6
	Site(TEXT("WldS1D5ShoreA"), -1, 30.0f, 8, 0, 1.0f);   // bay, s % 8 == 0
	Site(TEXT("WldS1D5ShoreB"), -1, 30.0f, 8, 4, 1.0f);   // bay, s % 8 == 4
	Scatter(S1D5("Scope"), 25.0f, 25.2f, 8, 0, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, past the edge of the boardwalk (24.1 m), s % 8 == 0
	Scatter(S1D5("DryWall"), 26.6f, 26.8f, 8, 0, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 0
	Scatter(S1D5("JarRow"), 25.0f, 25.3f, 8, 4, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 4
	Scatter(S1D5("Exedra"), 26.8f, 27.0f, 8, 4, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 4
	Scatter(S1D5("Well"), 25.1f, 25.5f, 8, 6, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 2
	Scatter(S1D5("FingerPost"), 27.0f, 28.0f, 8, 6, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, TALL (3.6 m), s % 8 == 2
	Scatter(S1D5("Sundial"), 25.1f, 25.5f, 8, 2, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 6
	Scatter(S1D5("DryWall"), 26.6f, 26.8f, 8, 2, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 6
// nearer than 40 m: slots 302 to 309 only (beside a banked bend the land 17 to 40 m out is a cutting)
Zone(TEXT("World s1d5, The Point, by the road 2"), 13.64076f, 14.00271f);
	Site(TEXT("WldS1D5WallA"), 1, 38.0f, 8, 0, 1.0f);   // town, s % 8 == 0
	Site(TEXT("WldS1D5WallB"), 1, 38.0f, 8, 4, 1.0f);   // town, s % 8 == 4
	Scatter(S1D5("TowerHouse"), 35.5f, 36.5f, 8, 6, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, its top over the shop fronts, s % 8 == 2
	Site(TEXT("WldS1D5Yard"), 1, 36.0f, 8, 2, 1.0f);   // town, s % 8 == 6
	Scatter(S1D5("JarRow"), 24.0f, 24.4f, 8, 0, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, LOW, the back edge of the pavement, s % 8 == 0
	Scatter(S1D5("Exedra"), 24.2f, 24.4f, 8, 4, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, LOW, s % 8 == 4
	Scatter(S1D5("Sundial"), 24.0f, 24.4f, 8, 6, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, LOW, s % 8 == 2
	Scatter(S1D5("Well"), 24.0f, 24.4f, 8, 2, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, LOW, s % 8 == 6
	Site(TEXT("WldS1D5ShoreA"), -1, 30.0f, 8, 0, 1.0f);   // bay, s % 8 == 0
	Site(TEXT("WldS1D5ShoreB"), -1, 30.0f, 8, 4, 1.0f);   // bay, s % 8 == 4
	Scatter(S1D5("Scope"), 25.0f, 25.2f, 8, 0, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, past the edge of the boardwalk (24.1 m), s % 8 == 0
	Scatter(S1D5("DryWall"), 26.6f, 26.8f, 8, 0, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 0
	Scatter(S1D5("JarRow"), 25.0f, 25.3f, 8, 4, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 4
	Scatter(S1D5("Exedra"), 26.8f, 27.0f, 8, 4, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 4
	Scatter(S1D5("Well"), 25.1f, 25.5f, 8, 6, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 2
	Scatter(S1D5("FingerPost"), 27.0f, 28.0f, 8, 6, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, TALL (3.6 m), s % 8 == 2
	Scatter(S1D5("Sundial"), 25.1f, 25.5f, 8, 2, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 6
	Scatter(S1D5("DryWall"), 26.6f, 26.8f, 8, 2, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 6
// nearer than 40 m: slots 319 to 337 only (beside a banked bend the land 17 to 40 m out is a cutting)
Zone(TEXT("World s1d5, The Point, by the road 3"), 14.4099f, 15.26951f);
	Site(TEXT("WldS1D5WallA"), 1, 38.0f, 8, 0, 1.0f);   // town, s % 8 == 0
	Site(TEXT("WldS1D5WallB"), 1, 38.0f, 8, 4, 1.0f);   // town, s % 8 == 4
	Scatter(S1D5("TowerHouse"), 35.5f, 36.5f, 8, 6, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, its top over the shop fronts, s % 8 == 2
	Site(TEXT("WldS1D5Yard"), 1, 36.0f, 8, 2, 1.0f);   // town, s % 8 == 6
	Scatter(S1D5("JarRow"), 24.0f, 24.4f, 8, 0, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, LOW, the back edge of the pavement, s % 8 == 0
	Scatter(S1D5("Exedra"), 24.2f, 24.4f, 8, 4, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, LOW, s % 8 == 4
	Scatter(S1D5("Sundial"), 24.0f, 24.4f, 8, 6, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, LOW, s % 8 == 2
	Scatter(S1D5("Well"), 24.0f, 24.4f, 8, 2, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, LOW, s % 8 == 6
	Site(TEXT("WldS1D5ShoreA"), -1, 30.0f, 8, 0, 1.0f);   // bay, s % 8 == 0
	Site(TEXT("WldS1D5ShoreB"), -1, 30.0f, 8, 4, 1.0f);   // bay, s % 8 == 4
	Scatter(S1D5("Scope"), 25.0f, 25.2f, 8, 0, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, past the edge of the boardwalk (24.1 m), s % 8 == 0
	Scatter(S1D5("DryWall"), 26.6f, 26.8f, 8, 0, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 0
	Scatter(S1D5("JarRow"), 25.0f, 25.3f, 8, 4, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 4
	Scatter(S1D5("Exedra"), 26.8f, 27.0f, 8, 4, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 4
	Scatter(S1D5("Well"), 25.1f, 25.5f, 8, 6, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 2
	Scatter(S1D5("FingerPost"), 27.0f, 28.0f, 8, 6, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, TALL (3.6 m), s % 8 == 2
	Scatter(S1D5("Sundial"), 25.1f, 25.5f, 8, 2, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 6
	Scatter(S1D5("DryWall"), 26.6f, 26.8f, 8, 2, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 6
// nearer than 40 m: slots 344 to 371 only (beside a banked bend the land 17 to 40 m out is a cutting)
Zone(TEXT("World s1d5, The Point, by the road 4"), 15.54097f, 16.8f);
	Site(TEXT("WldS1D5WallA"), 1, 38.0f, 8, 0, 1.0f);   // town, s % 8 == 0
	Site(TEXT("WldS1D5WallB"), 1, 38.0f, 8, 4, 1.0f);   // town, s % 8 == 4
	Scatter(S1D5("TowerHouse"), 35.5f, 36.5f, 8, 6, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, its top over the shop fronts, s % 8 == 2
	Site(TEXT("WldS1D5Yard"), 1, 36.0f, 8, 2, 1.0f);   // town, s % 8 == 6
	Scatter(S1D5("JarRow"), 24.0f, 24.4f, 8, 0, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, LOW, the back edge of the pavement, s % 8 == 0
	Scatter(S1D5("Exedra"), 24.2f, 24.4f, 8, 4, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, LOW, s % 8 == 4
	Scatter(S1D5("Sundial"), 24.0f, 24.4f, 8, 6, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, LOW, s % 8 == 2
	Scatter(S1D5("Well"), 24.0f, 24.4f, 8, 2, 1.0f, 1.0f, 0.0f, 1, 0.0f);   // town, LOW, s % 8 == 6
	Site(TEXT("WldS1D5ShoreA"), -1, 30.0f, 8, 0, 1.0f);   // bay, s % 8 == 0
	Site(TEXT("WldS1D5ShoreB"), -1, 30.0f, 8, 4, 1.0f);   // bay, s % 8 == 4
	Scatter(S1D5("Scope"), 25.0f, 25.2f, 8, 0, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, past the edge of the boardwalk (24.1 m), s % 8 == 0
	Scatter(S1D5("DryWall"), 26.6f, 26.8f, 8, 0, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 0
	Scatter(S1D5("JarRow"), 25.0f, 25.3f, 8, 4, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 4
	Scatter(S1D5("Exedra"), 26.8f, 27.0f, 8, 4, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 4
	Scatter(S1D5("Well"), 25.1f, 25.5f, 8, 6, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 2
	Scatter(S1D5("FingerPost"), 27.0f, 28.0f, 8, 6, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, TALL (3.6 m), s % 8 == 2
	Scatter(S1D5("Sundial"), 25.1f, 25.5f, 8, 2, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 6
	Scatter(S1D5("DryWall"), 26.6f, 26.8f, 8, 2, 1.0f, 1.0f, 0.0f, -1, 0.0f);   // bay, LOW, s % 8 == 6
Zone(TEXT("World s1d5, Arch Rock, slot 312"), 14.09319f, 14.13844f);   // 14.12 km
	Site(TEXT("WldS1D5ArchRock"), -1, 156.0f, 4, 0, 1.0f);   // bay, 156 m: past the beach race (126 to 136 m), not a groyne slot (205 m, s % 5 == 4)
Zone(TEXT("World s1d5, Arch Rock, slot 340"), 15.36f, 15.40524f);   // 15.38 km
	Site(TEXT("WldS1D5ArchRock"), -1, 156.0f, 4, 0, 1.0f);   // bay, 156 m: past the beach race (126 to 136 m), not a groyne slot (205 m, s % 5 == 4)
Zone(TEXT("World s1d5, Arch Rock, slot 368"), 16.6268f, 16.67205f);   // 16.65 km
	Site(TEXT("WldS1D5ArchRock"), -1, 156.0f, 4, 0, 1.0f);   // bay, 156 m: past the beach race (126 to 136 m), not a groyne slot (205 m, s % 5 == 4)
#undef S1D5
// ---- END s1d5 ----
}
{
// ---- BEGIN s1d6 (district 6 "Downtown waterfront", ft_world_bay_s1d6.py) ----
// 21 meshes: 11 buildings and a skybridge, 8 props (3 low), the hero THE PICTURE PALACE. 16.8 to 21.1 km: slots 372 to 466.
// One Zone a slot (every 4, phase 0 or 2: even slots only), so that each Set stands in a slot chosen by reading both tables:
//   town, s%4==0 (a side street, no shop row): four buildings flank the street, fronts 40.8 m out and more, the back row to 71 m,
//     a skybridge over the street in every second one, three props 37.4 to 40.4 m out (not on a banked bend);
//   town, s%4==2, not on a banked bend: two low-rise blocks 38.4 m out behind the shop row; low props on the pavement at 21.6 m
//     (hill slots only, short pieces: every level slot here is a banked bend or holds the film crew);
//   bay, s%12==6 or 10: a pavilion and two props 44 m out (45.5 on a banked bend); bay, s%12==0 or 4 without bar or court: the
//     winter garden and two props 57 m out; bay, slots 374, 404, 440 (one a lap of the boss circuit): the hero, 135 m out.
// No slot of a loop or a corkscrew is used (382-387, 397-400, 414-419, 428-431, 445-451, 460-463).
// Estimate: every rule is certain (chance 1) and stands in a named slot outside the set pieces, so the count is the sum of the
// Sets' parts: BayD 30 + BayE 24 + Canyon 117 + Hero 3 + Low 12 + Near 31 = 217 placed (by the brief's formula, x 0.75 for the track's share: about 163).
// The most of one mesh: NeonPalm 22, PosterDrum 16, StreetClock 14. Smallest x of a TALL mesh: 38 m (TaxiRank, anchor 44 - 6; its cabs reach 34.4 m).
#define S1D6(n) TEXT("/Game/World/Bayshore/SM_S1D6_" n)
Set(TEXT("WldS1D6Canyon0"), {
	{ S1D6("NightClub"), -18.0f, 7.0f, 0.f, 0.f, 1.f },
	{ S1D6("Theatre"), 18.0f, 7.0f, 0.f, 0.f, 1.f },
	{ S1D6("GlassOffice"), -18.0f, 21.0f, 0.f, 0.f, 1.f },
	{ S1D6("StepTower"), 18.0f, 21.9f, 0.f, 0.f, 1.f },   // fix round: was 21.0, 0.9 m inside the theatre's back
	{ S1D6("SkyBridge"), 0.0f, 21.0f, 0.f, 0.f, 1.f },
	{ S1D6("NeonPalm"), -12.5f, -5.5f, 0.f, 0.f, 1.f },
	{ S1D6("StreetClock"), 12.0f, -5.5f, 0.f, 0.f, 1.f },
	{ S1D6("TaxiRank"), 19.5f, -6.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Canyon1"), {
	{ S1D6("DeptStore"), -18.0f, 7.0f, 0.f, 0.f, 1.f },
	{ S1D6("CornerTower"), 18.0f, 7.0f, 0.f, 0.f, 1.f },
	{ S1D6("CarDeck"), -18.0f, 21.0f, 0.f, 0.f, 1.f },
	{ S1D6("GlassOffice"), 18.0f, 21.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Canyon2"), {
	{ S1D6("Theatre"), -18.0f, 7.0f, 0.f, 0.f, 1.f },
	{ S1D6("NightClub"), 18.0f, 7.0f, 0.f, 0.f, 1.f },
	{ S1D6("StepTower"), -18.0f, 21.9f, 0.f, 0.f, 1.f },   // fix round: was 21.0, 0.9 m inside the theatre's back
	{ S1D6("CarDeck"), 18.0f, 21.0f, 0.f, 0.f, 1.f },
	{ S1D6("SkyBridge"), 0.0f, 21.0f, 0.f, 0.f, 1.f },
	{ S1D6("NeonPalm"), 12.5f, -5.5f, 0.f, 0.f, 1.f },
	{ S1D6("TaxiRank"), -19.0f, -6.0f, 0.f, 0.f, 1.f },
	{ S1D6("PosterDrum"), -12.0f, -5.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Canyon3"), {
	{ S1D6("DeptStore"), -18.0f, 7.0f, 0.f, 0.f, 1.f },
	{ S1D6("CornerTower"), 18.0f, 7.0f, 0.f, 0.f, 1.f },
	{ S1D6("GlassOffice"), -18.0f, 21.0f, 0.f, 0.f, 1.f },
	{ S1D6("StepTower"), 18.0f, 21.0f, 0.f, 0.f, 1.f },
	{ S1D6("NeonPalm"), -12.5f, -5.5f, 0.f, 0.f, 1.f },
	{ S1D6("NewsStand"), 13.0f, -5.5f, 0.f, 0.f, 1.f },
	{ S1D6("StreetClock"), 21.0f, -5.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Canyon4"), {
	{ S1D6("NightClub"), -18.0f, 7.0f, 0.f, 0.f, 1.f },
	{ S1D6("DeptStore"), 18.0f, 7.0f, 0.f, 0.f, 1.f },
	{ S1D6("CarDeck"), -18.0f, 21.0f, 0.f, 0.f, 1.f },
	{ S1D6("GlassOffice"), 18.0f, 21.0f, 0.f, 0.f, 1.f },
	{ S1D6("SkyBridge"), 0.0f, 21.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Canyon5"), {
	{ S1D6("Theatre"), -18.0f, 7.0f, 0.f, 0.f, 1.f },
	{ S1D6("CornerTower"), 18.0f, 7.0f, 0.f, 0.f, 1.f },
	{ S1D6("StepTower"), -18.0f, 21.9f, 0.f, 0.f, 1.f },   // fix round: was 21.0, 0.9 m inside the theatre's back
	{ S1D6("CarDeck"), 18.0f, 21.0f, 0.f, 0.f, 1.f },
	{ S1D6("NeonPalm"), 12.5f, -5.5f, 0.f, 0.f, 1.f },
	{ S1D6("PosterDrum"), -12.0f, -5.5f, 0.f, 0.f, 1.f },
	{ S1D6("NewsStand"), -19.5f, -5.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Low0"), {
	{ S1D6("WalkUp"), -10.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("NeonBar"), 10.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Low1"), {
	{ S1D6("NeonBar"), -9.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("WalkUp"), 10.5f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Near3"), {
	{ S1D6("RopeCarpet"), -14.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("NewsRacks"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("PremiereWall"), 14.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6NearA"), {
	{ S1D6("RopeCarpet"), -13.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("NewsRacks"), 13.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6NearB"), {
	{ S1D6("NewsRacks"), -13.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("PremiereWall"), 13.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6NearC"), {
	{ S1D6("PremiereWall"), -13.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("RopeCarpet"), 13.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6BayD"), {
	{ S1D6("Pavilion"), 0.0f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D6("NeonPalm"), -13.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("PosterDrum"), 13.5f, -1.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6BayE"), {
	{ S1D6("WinterGarden"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("StreetClock"), -14.0f, -5.5f, 0.f, 0.f, 1.f },
	{ S1D6("NewsStand"), 14.0f, -5.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Hero"), {      // fix round: its three Sites place it at 1.3 times its size (60 m tall, 121 to 149 m out): from the road it was a small dark tower
	{ S1D6("PicturePalace"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1d6, Downtown waterfront, slot 372"), 16.80777f, 16.85302f);
	Site(TEXT("WldS1D6Canyon0"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more
	Site(TEXT("WldS1D6NearA"), 1, 21.6f, 4, 0, 1.f);   // town, the pavement either side of the side street
	Site(TEXT("WldS1D6BayE"), -1, 57.0f, 4, 0, 1.f);   // bay, the upper beach: no beach club (s%3==2), bar (s%5==3) or court (s%7==4) in this slot
Zone(TEXT("World s1d6, Downtown waterfront, slot 374"), 16.89826f, 16.9435f);
	Site(TEXT("WldS1D6Low0"), 1, 38.4f, 4, 2, 1.f);   // town, behind the shopfronts (27.5 m) and the promenade
	Site(TEXT("WldS1D6Hero"), -1, 135.0f, 4, 2, 1.f, 1.3f);   // bay, THE PICTURE PALACE (the town side 110 to 170 m holds the level's own hotels: FTTrack.cpp)
Zone(TEXT("World s1d6, Downtown waterfront, slot 376"), 16.98875f, 17.03399f);
	Site(TEXT("WldS1D6Canyon1"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more (a banked bend: no props inside 40 m)
	Site(TEXT("WldS1D6BayE"), -1, 57.0f, 4, 0, 1.f);   // bay, the upper beach: no beach club (s%3==2), bar (s%5==3) or court (s%7==4) in this slot
Zone(TEXT("World s1d6, Downtown waterfront, slot 378"), 17.07923f, 17.12448f);
	Site(TEXT("WldS1D6BayD"), -1, 45.5f, 4, 2, -1.f);   // OFF (fix round: chance -1): it stood in the chair lift, and a beach bar stands 54 m out here. The rule keeps its place: rules are seeded by their place in the table
Zone(TEXT("World s1d6, Downtown waterfront, slot 380"), 17.16972f, 17.21496f);
	Site(TEXT("WldS1D6Canyon2"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more
Zone(TEXT("World s1d6, Downtown waterfront, slot 388"), 17.53166f, 17.57691f);
	Site(TEXT("WldS1D6Canyon3"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more
	Site(TEXT("WldS1D6NearB"), 1, 21.6f, 4, 0, 1.f);   // town, the pavement either side of the side street
Zone(TEXT("World s1d6, Downtown waterfront, slot 390"), 17.62215f, 17.66739f);
	Site(TEXT("WldS1D6BayD"), -1, 57.0f, 4, 2, 1.f);   // bay, the upper beach (fix round: was 45.5 m, in the chair lift: its cables run 41.5 and 46.5 m out)
Zone(TEXT("World s1d6, Downtown waterfront, slot 392"), 17.71263f, 17.75788f);
	Site(TEXT("WldS1D6Canyon4"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more (a banked bend: no props inside 40 m)
Zone(TEXT("World s1d6, Downtown waterfront, slot 394"), 17.80312f, 17.84836f);
	Site(TEXT("WldS1D6BayD"), -1, 57.0f, 4, 2, 1.f);   // bay, the upper beach (fix round: was 45.5 m, in the chair lift: its cables run 41.5 and 46.5 m out)
Zone(TEXT("World s1d6, Downtown waterfront, slot 396"), 17.89361f, 17.93885f);
	Site(TEXT("WldS1D6Canyon5"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more
Zone(TEXT("World s1d6, Downtown waterfront, slot 402"), 18.16506f, 18.21031f);
	Site(TEXT("WldS1D6Low1"), 1, 38.4f, 4, 2, 1.f);   // town, behind the shopfronts (27.5 m) and the promenade
	Site(TEXT("WldS1D6Near3"), 1, 21.6f, 4, 2, 1.f);   // town, the pavement between the kerb (18.6 m) and the shoppers (24.6 m)
	Site(TEXT("WldS1D6BayD"), -1, 44.0f, 4, 2, 1.f);   // bay, behind the arcade (its deck ends 38.3 m out)
Zone(TEXT("World s1d6, Downtown waterfront, slot 404"), 18.25555f, 18.30079f);
	Site(TEXT("WldS1D6Canyon0"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more
	Site(TEXT("WldS1D6NearC"), 1, 21.6f, 4, 0, 1.f);   // town, the pavement either side of the side street
	Site(TEXT("WldS1D6Hero"), -1, 135.0f, 4, 0, 1.f, 1.3f);   // bay, THE PICTURE PALACE (the town side 110 to 170 m holds the level's own hotels: FTTrack.cpp)
Zone(TEXT("World s1d6, Downtown waterfront, slot 406"), 18.34604f, 18.39128f);
	Site(TEXT("WldS1D6Low0"), 1, 38.4f, 4, 2, 1.f);   // town, behind the shopfronts (27.5 m) and the promenade
	Site(TEXT("WldS1D6Near3"), 1, 21.6f, 4, 2, 1.f);   // town, the pavement between the kerb (18.6 m) and the shoppers (24.6 m)
	Site(TEXT("WldS1D6BayD"), -1, 44.0f, 4, 2, 1.f);   // bay, behind the arcade (its deck ends 38.3 m out)
Zone(TEXT("World s1d6, Downtown waterfront, slot 408"), 18.43652f, 18.48177f);
	Site(TEXT("WldS1D6Canyon1"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more (a banked bend: no props inside 40 m)
Zone(TEXT("World s1d6, Downtown waterfront, slot 412"), 18.61749f, 18.66274f);
	Site(TEXT("WldS1D6Canyon2"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more
	Site(TEXT("WldS1D6BayE"), -1, 57.0f, 4, 0, 1.f);   // bay, the upper beach: no beach club (s%3==2), bar (s%5==3) or court (s%7==4) in this slot
Zone(TEXT("World s1d6, Downtown waterfront, slot 420"), 18.97944f, 19.02468f);
	Site(TEXT("WldS1D6Canyon3"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more
	Site(TEXT("WldS1D6NearA"), 1, 21.6f, 4, 0, 1.f);   // town, the pavement either side of the side street
	Site(TEXT("WldS1D6BayE"), -1, 57.0f, 4, 0, 1.f);   // bay, the upper beach: no beach club (s%3==2), bar (s%5==3) or court (s%7==4) in this slot
Zone(TEXT("World s1d6, Downtown waterfront, slot 424"), 19.16041f, 19.20565f);
	Site(TEXT("WldS1D6Canyon4"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more (a banked bend: no props inside 40 m)
Zone(TEXT("World s1d6, Downtown waterfront, slot 426"), 19.2509f, 19.29614f);
	Site(TEXT("WldS1D6BayD"), -1, 57.0f, 4, 2, 1.f);   // bay, the upper beach (fix round: was 45.5 m, in the chair lift: its cables run 41.5 and 46.5 m out)
Zone(TEXT("World s1d6, Downtown waterfront, slot 432"), 19.52235f, 19.5676f);
	Site(TEXT("WldS1D6Canyon5"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more
	Site(TEXT("WldS1D6NearB"), 1, 21.6f, 4, 0, 1.f);   // town, the pavement either side of the side street
	Site(TEXT("WldS1D6BayE"), -1, 57.0f, 4, 0, 1.f);   // bay, the upper beach: no beach club (s%3==2), bar (s%5==3) or court (s%7==4) in this slot
Zone(TEXT("World s1d6, Downtown waterfront, slot 434"), 19.61284f, 19.65808f);
	Site(TEXT("WldS1D6Low1"), 1, 38.4f, 4, 2, 1.f);   // town, behind the shopfronts (27.5 m) and the promenade
	Site(TEXT("WldS1D6Near3"), 1, 21.6f, 4, 2, 1.f);   // town, the pavement between the kerb (18.6 m) and the shoppers (24.6 m)
Zone(TEXT("World s1d6, Downtown waterfront, slot 436"), 19.70333f, 19.74857f);
	Site(TEXT("WldS1D6Canyon0"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more
	Site(TEXT("WldS1D6NearC"), 1, 21.6f, 4, 0, 1.f);   // town, the pavement either side of the side street
	Site(TEXT("WldS1D6BayE"), -1, 57.0f, 4, 0, 1.f);   // bay, the upper beach: no beach club (s%3==2), bar (s%5==3) or court (s%7==4) in this slot
Zone(TEXT("World s1d6, Downtown waterfront, slot 438"), 19.79381f, 19.83906f);
	Site(TEXT("WldS1D6Low0"), 1, 38.4f, 4, 2, 1.f);   // town, behind the shopfronts (27.5 m) and the promenade
	Site(TEXT("WldS1D6Near3"), 1, 21.6f, 4, 2, 1.f);   // town, the pavement between the kerb (18.6 m) and the shoppers (24.6 m)
	Site(TEXT("WldS1D6BayD"), -1, 44.0f, 4, 2, 1.f);   // bay, behind the arcade (its deck ends 38.3 m out)
Zone(TEXT("World s1d6, Downtown waterfront, slot 440"), 19.8843f, 19.92954f);
	Site(TEXT("WldS1D6Canyon1"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more (a banked bend: no props inside 40 m)
	Site(TEXT("WldS1D6Hero"), -1, 135.0f, 4, 0, 1.f, 1.3f);   // bay, THE PICTURE PALACE (the town side 110 to 170 m holds the level's own hotels: FTTrack.cpp)
Zone(TEXT("World s1d6, Downtown waterfront, slot 442"), 19.97478f, 20.02003f);
	Site(TEXT("WldS1D6BayD"), -1, 45.5f, 4, 2, 1.f);   // bay, behind the arcade (its deck ends 38.3 m out) (a banked bend: 40 m and more)
Zone(TEXT("World s1d6, Downtown waterfront, slot 444"), 20.06527f, 20.11051f);
	Site(TEXT("WldS1D6Canyon2"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more
	Site(TEXT("WldS1D6BayE"), -1, 57.0f, 4, 0, 1.f);   // bay, the upper beach: no beach club (s%3==2), bar (s%5==3) or court (s%7==4) in this slot
Zone(TEXT("World s1d6, Downtown waterfront, slot 452"), 20.42721f, 20.47246f);
	Site(TEXT("WldS1D6Canyon3"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more
	Site(TEXT("WldS1D6NearA"), 1, 21.6f, 4, 0, 1.f);   // town, the pavement either side of the side street
Zone(TEXT("World s1d6, Downtown waterfront, slot 454"), 20.5177f, 20.56294f);
	Site(TEXT("WldS1D6BayD"), -1, 45.5f, 4, 2, 1.f);   // bay, behind the arcade (its deck ends 38.3 m out) (a banked bend: 40 m and more)
Zone(TEXT("World s1d6, Downtown waterfront, slot 456"), 20.60819f, 20.65343f);
	Site(TEXT("WldS1D6Canyon4"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more (a banked bend: no props inside 40 m)
	Site(TEXT("WldS1D6BayE"), -1, 57.0f, 4, 0, 1.f);   // bay, the upper beach: no beach club (s%3==2), bar (s%5==3) or court (s%7==4) in this slot
Zone(TEXT("World s1d6, Downtown waterfront, slot 464"), 20.97013f, 21.01537f);
	Site(TEXT("WldS1D6Canyon5"), 1, 44.0f, 4, 0, 1.f);   // town, a side street: fronts 40.8 m out and more
	Site(TEXT("WldS1D6NearB"), 1, 21.6f, 4, 0, 1.f);   // town, the pavement either side of the side street
Zone(TEXT("World s1d6, Downtown waterfront, slot 466"), 21.06062f, 21.1f);
	Site(TEXT("WldS1D6Low1"), 1, 38.4f, 4, 2, 1.f);   // town, behind the shopfronts (27.5 m) and the promenade
	Site(TEXT("WldS1D6Near3"), 1, 21.6f, 4, 2, 1.f);   // town, the pavement between the kerb (18.6 m) and the shoppers (24.6 m)
	Site(TEXT("WldS1D6BayD"), -1, 44.0f, 4, 2, 1.f);   // bay, behind the arcade (its deck ends 38.3 m out)
#undef S1D6
// ---- END s1d6 ----
}
{
// ---- BEGIN s1d7 (district 7 "The Old Harbour", 21.1 km to the end, ft_world_bay_s1d7.py) ----
// 16 meshes: 7 buildings, 8 props, 1 hero (THE BIG FISH). Every name below is one the make log printed (C:/ftwork/logs/L0s1d7_make.log).
// The district is slots 467 to 561. Loops (477-483, 509-514, 541-546) and corkscrews (492-495, 524-527, 556-559) take 31 of them (read from
// the slot lists of the dressing and the zone plan: not checked in the game), and in
// the 32 even slots that are left the dressing and the people passes already stand in nearly every band, a different thing in each slot.
// So this block does not sprinkle: every Site is in a ONE-SLOT Zone (the tables' own way: "People p3f", "A pier wheel"), chance 1, in a
// slot whose own furniture was read from FTDressL0.inl and FTPeopleL0.inl. Slots used: EVEN only (s % 4 == 0 or 2; every 4, phase 0 or 2).
// What was dodged, per slot: the pier (s % 11 == 6: the QuayP set leaves its middle open), the beach club (s % 3 == 2, 37 to 66 m: no bay
// set there), the tiki bar (s % 5 == 3, 49 to 62 m) and the volleyball (s % 7 == 4, 53 to 68 m: no Yard set there; the Quay sets of slots
// 498 and 538 end 0.4 m past the surf rack of the tiki bar at 49 m: left so), the arcade (s % 4 == 2,
// to 38.3 m) and the boardwalk shops (s % 4 == 0, to 37 m) on level and gentle ground, the watch tower 28 m out (s % 16 == 6), the pool
// deck (town, 42.1 m and out, level s % 4 == 2), the side street and its people (town s % 4 == 0: 9 m kept clear either side of its
// middle), the cuttings inside the banked bends (bay side slots 472-475, 504-507, 536-539: nothing nearer than 40 m; town side slots
// 486-490, 518-522, 550-554: no Back set). The low band by the road (17 to 26 m) is left alone: in every even slot it is the boardwalk
// and the beach edge (bay) or parked cars, shoppers and the film crew (town).
// Bands: Wharf 25.8 to 31 m (bay) | Back 33.3 to 41.5 m (town) | Quay 40.8 to 51.2 m (bay) | Yard 47.1 to 60 m (bay) | Lane 51.8 to 61.5 m
// (town) | the Big Fish 140 m (bay), slots 476, 518, 552 (no pier, groyne, funfair, crowd or beach race in them).
// Count (exact: every Site is written out, chance 1; nothing is taken off for the track's share):
//   Wharf    6 slots x 3 parts =  18   slots 468 484 498 516 528 532
//   QuayA    4 slots x 6 parts =  24   slots 474 498 522 550
//   QuayB    3 slots x 6 parts =  18   slots 486 502 538
//   QuayP    2 slots x 4 parts =   8   slots 490 534
//   YardA    5 slots x 6 parts =  30   slots 472 496 516 532 552
//   YardB    4 slots x 6 parts =  24   slots 484 504 520 540
//   LaneA    9 slots x 5 parts =  45   slots 468 476 488 500 508 520 532 540 552
//   LaneB    9 slots x 5 parts =  45   slots 472 484 496 504 516 528 536 548 560
//   BackA    4 slots x 4 parts =  16   slots 470 498 506 534
//   BackB    4 slots x 4 parts =  16   slots 474 502 530 538
//   BigFish  3 slots x 1 parts =   3   slots 476 518 552
//   SUM 247 placed in 53 sites (x 0.75, if the track leaves its usual share out: about 185).
//   Per mesh: SailLoft 30, NetMast 26, QuayCrane 24, Smokery 23, NetLoft 20, TwinGables 18, HarbourInn 18, IceHouse 16, Capstan 16, FishBoxes 10, BoatShed 9, QuayWall 9, Bollards 9, DryRack 8, NetBoat 8, BigFish 3.
// Smallest x of a TALL mesh: 29.0 m (NetMast in Wharf). Smallest x of a low mesh: 25.8 m (FishBoxes in Wharf). Glow: only the Big Fish (steady, 140 m).
#define S1D7(n) TEXT("/Game/World/Bayshore/SM_S1D7_" n)
// bay, the upper beach behind the arcade (it ends 38.3 m out): a row of lofts with a 9.6 m gap in the middle, racks and boxes in the gap
Set(TEXT("WldS1D7QuayA"), {                       // anchor 45.5 m, bay side   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("Smokery"), -10.2f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("NetMast"), -17.6f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D7("NetLoft"), 8.4f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("SailLoft"), 15.7f, 0.3f, 0.f, 0.0f, 1.f },
	{ S1D7("DryRack"), 0.0f, -1.5f, 0.f, 90.0f, 1.f },
	{ S1D7("FishBoxes"), 3.3f, -0.5f, 0.f, 90.0f, 1.f },
});
// the same row, other houses; a boat and a capstan in the gap
Set(TEXT("WldS1D7QuayB"), {                       // anchor 45.5 m, bay side   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("IceHouse"), -8.8f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("NetLoft"), -17.0f, 0.3f, 0.f, 0.0f, 1.f },
	{ S1D7("TwinGables"), 9.8f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("QuayCrane"), 17.3f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D7("NetBoat"), 0.0f, -1.2f, 0.f, 90.0f, 1.f },
	{ S1D7("Capstan"), 0.0f, 4.0f, 0.f, 0.0f, 1.f },
});
// the same row in a PIER slot (s % 11 == 6): the gap is left empty, the pier and its gateway (7.6 m wide, from 40 m out) run through it
Set(TEXT("WldS1D7QuayP"), {                       // anchor 45.5 m, bay side   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("Smokery"), -10.2f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("NetMast"), -17.6f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D7("TwinGables"), 9.8f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("QuayCrane"), 17.3f, -1.0f, 0.f, 0.0f, 1.f },
});
// bay, the boat builder's yard, further down the beach (the pass 3 beach scenes stand 42.5 m out in these slots)
Set(TEXT("WldS1D7YardA"), {                       // anchor 56 m, bay side   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("BoatShed"), 10.2f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("NetLoft"), 18.3f, 0.3f, 0.f, 0.0f, 1.f },
	{ S1D7("HarbourInn"), -9.6f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("QuayWall"), -10.0f, -6.3f, 0.f, 0.0f, 1.f },
	{ S1D7("NetBoat"), 9.5f, -7.0f, 0.f, -12.0f, 1.f },
	{ S1D7("NetMast"), -17.6f, -1.5f, 0.f, 0.0f, 1.f },
});
// the yard the other way round, a drying ground in the middle
Set(TEXT("WldS1D7YardB"), {                       // anchor 56 m, bay side   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("BoatShed"), -10.2f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("Smokery"), 10.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("QuayCrane"), -17.6f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D7("QuayWall"), 10.5f, -6.3f, 0.f, 0.0f, 1.f },
	{ S1D7("Capstan"), -10.0f, -7.2f, 0.f, 0.0f, 1.f },
	{ S1D7("DryRack"), 0.0f, -6.5f, 0.f, 0.0f, 1.f },
});
// bay, right behind the boardwalk (it ends 24.1 m out) where no arcade or shop stands on it: slopes and steep slots only
Set(TEXT("WldS1D7Wharf"), {                       // anchor 29 m, bay side   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("NetMast"), -9.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("QuayCrane"), 9.0f, 1.0f, 0.f, 0.0f, 1.f },
	{ S1D7("FishBoxes"), 0.5f, -3.2f, 0.f, 0.0f, 1.f },
});
// town, the two corners of a side street, 52 to 62 m back (the street is 10.2 m wide with its kerbs: nothing nearer than 9 m to its middle)
Set(TEXT("WldS1D7LaneA"), {                       // anchor 58 m, town side   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("HarbourInn"), -13.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("SailLoft"), -20.0f, 0.4f, 0.f, 0.0f, 1.f },
	{ S1D7("TwinGables"), 13.8f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("QuayCrane"), 20.6f, -0.5f, 0.f, 0.0f, 1.f },
	{ S1D7("Bollards"), 13.5f, -5.6f, 0.f, 0.0f, 1.f },
});
// the same corners, other houses
Set(TEXT("WldS1D7LaneB"), {                       // anchor 58 m, town side   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("Smokery"), -14.4f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("NetMast"), -20.9f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D7("IceHouse"), 13.3f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("SailLoft"), 20.0f, 0.4f, 0.f, 0.0f, 1.f },
	{ S1D7("Capstan"), -12.5f, -6.2f, 0.f, 0.0f, 1.f },
});
// town, a row of lofts behind the first shop row (Front1: false fronts 6 to 8.6 m tall, 27.5 m out): gables, hoists and chimneys over its parapets
Set(TEXT("WldS1D7BackA"), {                       // anchor 38 m, town side   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("SailLoft"), -16.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("NetLoft"), -9.4f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("HarbourInn"), 1.5f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("IceHouse"), 10.5f, 0.0f, 0.f, 0.0f, 1.f },
});
// the same row, other houses
Set(TEXT("WldS1D7BackB"), {                       // anchor 38 m, town side   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("TwinGables"), -13.5f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("SailLoft"), -5.6f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("Smokery"), 2.7f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D7("NetLoft"), 12.2f, 0.0f, 0.f, 0.0f, 1.f },
});
// THE BIG FISH, bay side, 140 m out (placed at 1.05: 50.3 m long, 21.2 m tall)
Set(TEXT("WldS1D7BigFish"), {                       // anchor 152 m, bay side   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("BigFish"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
});
// One Site in one slot: slot S is the Zone from (S - 0.5) to (S + 0.5) slots of 45.243 m; every 4, the phase that picks S.
auto S1D7At = [&](int32 S, const TCHAR* SetName, int32 Side, float X, float Sc)
{
	Zone(TEXT("World s1d7, the Old Harbour"), (S - 0.5f) * 0.045243f, (S + 0.5f) * 0.045243f);
	Site(SetName, Side, X, 4, (4 - S % 4) % 4, 1.f, Sc);
};
// Wharf: s % 4 == 0 in 468 484 516 528 532; s % 4 == 2 in 498
S1D7At(468, TEXT("WldS1D7Wharf"), -1, 29.0f, 1.f); S1D7At(484, TEXT("WldS1D7Wharf"), -1, 29.0f, 1.f); S1D7At(498, TEXT("WldS1D7Wharf"), -1, 29.0f, 1.f);
S1D7At(516, TEXT("WldS1D7Wharf"), -1, 29.0f, 1.f); S1D7At(528, TEXT("WldS1D7Wharf"), -1, 29.0f, 1.f); S1D7At(532, TEXT("WldS1D7Wharf"), -1, 29.0f, 1.f);
// QuayA: s % 4 == 0 in none; s % 4 == 2 in 474 498 522 550
S1D7At(474, TEXT("WldS1D7QuayA"), -1, 45.5f, 1.f); S1D7At(498, TEXT("WldS1D7QuayA"), -1, 45.5f, 1.f); S1D7At(522, TEXT("WldS1D7QuayA"), -1, 45.5f, 1.f);
S1D7At(550, TEXT("WldS1D7QuayA"), -1, 45.5f, 1.f);
// QuayB: s % 4 == 0 in none; s % 4 == 2 in 486 502 538
S1D7At(486, TEXT("WldS1D7QuayB"), -1, 45.5f, 1.f); S1D7At(502, TEXT("WldS1D7QuayB"), -1, 45.5f, 1.f); S1D7At(538, TEXT("WldS1D7QuayB"), -1, 45.5f, 1.f);
// QuayP: s % 4 == 0 in none; s % 4 == 2 in 490 534
S1D7At(490, TEXT("WldS1D7QuayP"), -1, 45.5f, 1.f); S1D7At(534, TEXT("WldS1D7QuayP"), -1, 45.5f, 1.f);
// YardA: s % 4 == 0 in 472 496 516 532 552; s % 4 == 2 in none
S1D7At(472, TEXT("WldS1D7YardA"), -1, 56.0f, 1.f); S1D7At(496, TEXT("WldS1D7YardA"), -1, 56.0f, 1.f); S1D7At(516, TEXT("WldS1D7YardA"), -1, 56.0f, 1.f);
S1D7At(532, TEXT("WldS1D7YardA"), -1, 56.0f, 1.f); S1D7At(552, TEXT("WldS1D7YardA"), -1, 56.0f, 1.f);
// YardB: s % 4 == 0 in 484 504 520 540; s % 4 == 2 in none
S1D7At(484, TEXT("WldS1D7YardB"), -1, 56.0f, 1.f); S1D7At(504, TEXT("WldS1D7YardB"), -1, 56.0f, 1.f); S1D7At(520, TEXT("WldS1D7YardB"), -1, 56.0f, 1.f);
S1D7At(540, TEXT("WldS1D7YardB"), -1, 56.0f, 1.f);
// LaneA: s % 4 == 0 in 468 476 488 500 508 520 532 540 552; s % 4 == 2 in none
S1D7At(468, TEXT("WldS1D7LaneA"), 1, 58.0f, 1.f); S1D7At(476, TEXT("WldS1D7LaneA"), 1, 58.0f, 1.f); S1D7At(488, TEXT("WldS1D7LaneA"), 1, 58.0f, 1.f);
S1D7At(500, TEXT("WldS1D7LaneA"), 1, 58.0f, 1.f); S1D7At(508, TEXT("WldS1D7LaneA"), 1, 58.0f, 1.f); S1D7At(520, TEXT("WldS1D7LaneA"), 1, 58.0f, 1.f);
S1D7At(532, TEXT("WldS1D7LaneA"), 1, 58.0f, 1.f); S1D7At(540, TEXT("WldS1D7LaneA"), 1, 58.0f, 1.f); S1D7At(552, TEXT("WldS1D7LaneA"), 1, 58.0f, 1.f);
// LaneB: s % 4 == 0 in 472 484 496 504 516 528 536 548 560; s % 4 == 2 in none
S1D7At(472, TEXT("WldS1D7LaneB"), 1, 58.0f, 1.f); S1D7At(484, TEXT("WldS1D7LaneB"), 1, 58.0f, 1.f); S1D7At(496, TEXT("WldS1D7LaneB"), 1, 58.0f, 1.f);
S1D7At(504, TEXT("WldS1D7LaneB"), 1, 58.0f, 1.f); S1D7At(516, TEXT("WldS1D7LaneB"), 1, 58.0f, 1.f); S1D7At(528, TEXT("WldS1D7LaneB"), 1, 58.0f, 1.f);
S1D7At(536, TEXT("WldS1D7LaneB"), 1, 58.0f, 1.f); S1D7At(548, TEXT("WldS1D7LaneB"), 1, 58.0f, 1.f); S1D7At(560, TEXT("WldS1D7LaneB"), 1, 58.0f, 1.f);
// BackA: s % 4 == 0 in none; s % 4 == 2 in 470 498 506 534
S1D7At(470, TEXT("WldS1D7BackA"), 1, 38.0f, 1.f); S1D7At(498, TEXT("WldS1D7BackA"), 1, 38.0f, 1.f); S1D7At(506, TEXT("WldS1D7BackA"), 1, 38.0f, 1.f);
S1D7At(534, TEXT("WldS1D7BackA"), 1, 38.0f, 1.f);
// BackB: s % 4 == 0 in none; s % 4 == 2 in 474 502 530 538
S1D7At(474, TEXT("WldS1D7BackB"), 1, 38.0f, 1.f); S1D7At(502, TEXT("WldS1D7BackB"), 1, 38.0f, 1.f); S1D7At(530, TEXT("WldS1D7BackB"), 1, 38.0f, 1.f);
S1D7At(538, TEXT("WldS1D7BackB"), 1, 38.0f, 1.f);
// BigFish: s % 4 == 0 in 476 552; s % 4 == 2 in 518
S1D7At(476, TEXT("WldS1D7BigFish"), -1, 152.0f, 1.05f); S1D7At(518, TEXT("WldS1D7BigFish"), -1, 152.0f, 1.05f); S1D7At(552, TEXT("WldS1D7BigFish"), -1, 152.0f, 1.05f);   // fix round: was 140 m, in the beach race's line (118 to 144 m); now 144.5 to 159.5 m
#undef S1D7
// ---- END s1d7 ----
}
{
// ---- BEGIN s1plants (the planting of the seven districts, ft_world_bay_s1plants.py) ----
// Written by C:/ftwork/jobs/L0s1plants/make_inl.py. 22 meshes, all Scatter, chance 1, one side a rule, slots s % 4 == 3 only (every 4 phase 1;
// every 8 phase 5 = s % 8 == 3; every 8 phase 1 = s % 8 == 7). Counted slot by slot: 719 rule-slots = 719 pieces before the track leaves any out;
// x 0.75 (the share the track leaves out) = about 539 placed. Rule-slots by district: s1d1 81, s1d2 101, s1d3 93, s1d4 105, s1d5 111, s1d6 103, s1d7 125.
// The most of one mesh, before / after x 0.75: CocoPalm 53 / 40, FlameTree 51 / 38, FanPalm 48 / 36, RoyalPalms 43 / 32.
// Bands (m from the centre line). Town side: low 20.8-23.0 (the pavement); TALL 34.5-36.5 and 37.0-38.8 (behind the 27.5 m fronts and the
// promenade, before the cafes and markets at 42); the great fig 37.5-39.5; TALL 56.5-60.0. Bay side: low 25.5-27.5, low and reeds 28.5-31.0,
// carpets 32.0-34.5 (the Point, the Old Harbour); Hotel Row low 40.0-42.5 and 43.5-46.5 (its boardwalk shops reach 38 m); TALL 40.0-47.0 and 49.0-58.0.
// Beside a banked bend (the zone plan's list, 30 m of margin each end) a zone of its own holds only the rules that start 40 m out or more.
#define S1PLANTS(n) TEXT("/Game/World/Bayshore/SM_S1PLANTS_" n)
Zone(TEXT("World s1plants, The Strip"), 0.0000f, 0.9727f);   // slots 0 to 21
	Scatter(S1PLANTS("RoyalPalms"), 37.0f, 38.8f, 8, 5, 1.f, 1.0f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("FlameTree"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("FanPalm"), 34.5f, 36.5f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Hibiscus"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("CarpetSun"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("FlameTree"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("FanPalm"), 40.0f, 47.0f, 4, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("FlameTree"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Hibiscus"), 25.5f, 27.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("CarpetSun"), 28.5f, 31.0f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, The Strip, a banked bend"), 0.9727f, 1.2442f);   // slots 22 to 27
	Scatter(S1PLANTS("FlameTree"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("FanPalm"), 40.0f, 47.0f, 4, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("FlameTree"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The Strip"), 1.2442f, 2.2395f);   // slots 28 to 49
	Scatter(S1PLANTS("RoyalPalms"), 37.0f, 38.8f, 8, 5, 1.f, 1.0f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("FlameTree"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("FanPalm"), 34.5f, 36.5f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Hibiscus"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("CarpetSun"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("FlameTree"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("FanPalm"), 40.0f, 47.0f, 4, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("FlameTree"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Hibiscus"), 25.5f, 27.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("CarpetSun"), 28.5f, 31.0f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, The Strip, a banked bend"), 2.2395f, 2.6015f);   // slots 50 to 57
	Scatter(S1PLANTS("FlameTree"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("FanPalm"), 40.0f, 47.0f, 4, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("FlameTree"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The Strip"), 2.6015f, 3.1896f);   // slots 58 to 70
	Scatter(S1PLANTS("RoyalPalms"), 37.0f, 38.8f, 8, 5, 1.f, 1.0f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("FlameTree"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("FanPalm"), 34.5f, 36.5f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Hibiscus"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("CarpetSun"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("FlameTree"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("FanPalm"), 40.0f, 47.0f, 4, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("FlameTree"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Hibiscus"), 25.5f, 27.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("CarpetSun"), 28.5f, 31.0f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, The Marina"), 3.1896f, 3.7325f);   // slots 71 to 82
	Scatter(S1PLANTS("FanPalm"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 37.0f, 38.8f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.06f, 1, 180.0f);
	Scatter(S1PLANTS("CarpetWave"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("FanPalm"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("FanPalm"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 4, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Reeds"), 28.5f, 31.0f, 4, 1, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 25.5f, 27.5f, 8, 5, 1.f, 1.0f, 0.06f, -1, 180.0f);
	Scatter(S1PLANTS("CarpetWave"), 25.5f, 27.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, The Marina, a banked bend"), 3.7325f, 4.0040f);   // slots 83 to 88
	Scatter(S1PLANTS("FanPalm"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("FanPalm"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 4, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The Marina"), 4.0040f, 4.5017f);   // slots 89 to 99
	Scatter(S1PLANTS("FanPalm"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 37.0f, 38.8f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.06f, 1, 180.0f);
	Scatter(S1PLANTS("CarpetWave"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("FanPalm"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("FanPalm"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 4, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Reeds"), 28.5f, 31.0f, 4, 1, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 25.5f, 27.5f, 8, 5, 1.f, 1.0f, 0.06f, -1, 180.0f);
	Scatter(S1PLANTS("CarpetWave"), 25.5f, 27.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, The Marina, a banked bend"), 4.5017f, 4.7731f);   // slots 100 to 105
	Scatter(S1PLANTS("CocoPalm"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 4, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The Marina"), 4.7731f, 5.5875f);   // slots 106 to 123
	Scatter(S1PLANTS("FanPalm"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 37.0f, 38.8f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.06f, 1, 180.0f);
	Scatter(S1PLANTS("CarpetWave"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("FanPalm"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("FanPalm"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 4, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Reeds"), 28.5f, 31.0f, 4, 1, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 25.5f, 27.5f, 8, 5, 1.f, 1.0f, 0.06f, -1, 180.0f);
	Scatter(S1PLANTS("CarpetWave"), 25.5f, 27.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, The Marina, a banked bend"), 5.5875f, 5.8590f);   // slots 124 to 129
	Scatter(S1PLANTS("CocoPalm"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 4, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The Marina"), 5.8590f, 6.4019f);   // slots 130 to 141
	Scatter(S1PLANTS("FanPalm"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 37.0f, 38.8f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.06f, 1, 180.0f);
	Scatter(S1PLANTS("CarpetWave"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("FanPalm"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("FanPalm"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 4, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Reeds"), 28.5f, 31.0f, 4, 1, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 25.5f, 27.5f, 8, 5, 1.f, 1.0f, 0.06f, -1, 180.0f);
	Scatter(S1PLANTS("CarpetWave"), 25.5f, 27.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, Pleasure Beach"), 6.4019f, 7.9401f);   // slots 142 to 175
	Scatter(S1PLANTS("FlameTree"), 34.5f, 36.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Agave"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.05f, 1, 180.0f);
	Scatter(S1PLANTS("CarpetWedge"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("FlameTree"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 40.0f, 47.0f, 4, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("FlameTree"), 49.0f, 58.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Agave"), 25.5f, 27.5f, 8, 5, 1.f, 1.0f, 0.05f, -1, 180.0f);
	Scatter(S1PLANTS("CarpetWedge"), 28.5f, 31.0f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, Pleasure Beach, a banked bend"), 7.9401f, 8.2568f);   // slots 176 to 182
	Scatter(S1PLANTS("CocoPalm"), 40.0f, 47.0f, 4, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, Pleasure Beach"), 8.2568f, 9.2070f);   // slots 183 to 203
	Scatter(S1PLANTS("FlameTree"), 34.5f, 36.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Agave"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.05f, 1, 180.0f);
	Scatter(S1PLANTS("CarpetWedge"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("FlameTree"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 40.0f, 47.0f, 4, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("FlameTree"), 49.0f, 58.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Agave"), 25.5f, 27.5f, 8, 5, 1.f, 1.0f, 0.05f, -1, 180.0f);
	Scatter(S1PLANTS("CarpetWedge"), 28.5f, 31.0f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, Pleasure Beach, a banked bend"), 9.2070f, 9.7951f);   // slots 204 to 216
	Scatter(S1PLANTS("FlameTree"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 40.0f, 47.0f, 4, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("FlameTree"), 49.0f, 58.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("CocoPalm"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, Hotel Row, a banked bend"), 9.7951f, 10.0666f);   // slots 217 to 222
	Scatter(S1PLANTS("BirdOfParadise"), 40.0f, 42.5f, 4, 1, 1.f, 1.0f, 0.05f, -1, 180.0f);
	Scatter(S1PLANTS("Hibiscus"), 43.5f, 46.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("Pergola"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.00f, -1, 6.0f);
Zone(TEXT("World s1plants, Hotel Row"), 10.0666f, 10.9714f);   // slots 223 to 242
	Scatter(S1PLANTS("RoyalPalms"), 37.0f, 38.8f, 4, 1, 1.f, 1.0f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("Topiary"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("Pergola"), 34.5f, 36.5f, 8, 5, 1.f, 1.0f, 0.00f, 1, 6.0f);
	Scatter(S1PLANTS("BirdOfParadise"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.05f, 1, 180.0f);
	Scatter(S1PLANTS("Hibiscus"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("Pergola"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.00f, 1, 6.0f);
	Scatter(S1PLANTS("BirdOfParadise"), 40.0f, 42.5f, 4, 1, 1.f, 1.0f, 0.05f, -1, 180.0f);
	Scatter(S1PLANTS("CarpetSun"), 43.5f, 46.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("Hibiscus"), 43.5f, 46.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("Pergola"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.00f, -1, 6.0f);
Zone(TEXT("World s1plants, Hotel Row, a banked bend"), 10.9714f, 11.2429f);   // slots 243 to 248
	Scatter(S1PLANTS("Pergola"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.00f, 1, 6.0f);
	Scatter(S1PLANTS("BirdOfParadise"), 40.0f, 42.5f, 4, 1, 1.f, 1.0f, 0.05f, -1, 180.0f);
	Scatter(S1PLANTS("CarpetSun"), 43.5f, 46.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("Hibiscus"), 43.5f, 46.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("Pergola"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.00f, -1, 6.0f);
Zone(TEXT("World s1plants, Hotel Row"), 11.2429f, 13.1883f);   // slots 249 to 291
	Scatter(S1PLANTS("RoyalPalms"), 37.0f, 38.8f, 4, 1, 1.f, 1.0f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("Topiary"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("Pergola"), 34.5f, 36.5f, 8, 5, 1.f, 1.0f, 0.00f, 1, 6.0f);
	Scatter(S1PLANTS("BirdOfParadise"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.05f, 1, 180.0f);
	Scatter(S1PLANTS("Hibiscus"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("Pergola"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.00f, 1, 6.0f);
	Scatter(S1PLANTS("BirdOfParadise"), 40.0f, 42.5f, 4, 1, 1.f, 1.0f, 0.05f, -1, 180.0f);
	Scatter(S1PLANTS("CarpetSun"), 43.5f, 46.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("Hibiscus"), 43.5f, 46.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("Pergola"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.00f, -1, 6.0f);
Zone(TEXT("World s1plants, The Point, a banked bend"), 13.3241f, 13.6860f);   // slots 295 to 302
	Scatter(S1PLANTS("UmbrellaPine"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("UmbrellaPine"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Agave"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.05f, -1, 180.0f);
	Scatter(S1PLANTS("UmbrellaPine"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The Point"), 13.6860f, 13.9575f);   // slots 303 to 308
	Scatter(S1PLANTS("UmbrellaPine"), 37.0f, 38.8f, 12, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);   // fix round: s % 12 == 7 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("Pergola"), 34.5f, 36.5f, 24, 17, 1.f, 1.0f, 0.00f, 1, 6.0f);   // fix round: s % 24 == 7 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("QuayTree"), 34.5f, 36.5f, 24, 5, 1.f, 1.0f, 0.10f, 1, 15.0f);   // fix round: s % 24 == 19 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("PricklyPear"), 20.8f, 23.0f, 24, 17, 1.f, 1.0f, 0.05f, 1, 180.0f);   // fix round: s % 24 == 7 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("Agave"), 20.8f, 23.0f, 24, 5, 1.f, 1.0f, 0.05f, 1, 180.0f);   // fix round: s % 24 == 19 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("UmbrellaPine"), 56.5f, 60.0f, 24, 17, 1.f, 1.0f, 0.10f, 1, 180.0f);   // fix round: s % 24 == 7 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("UmbrellaPine"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Agave"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.05f, -1, 180.0f);
	Scatter(S1PLANTS("UmbrellaPine"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("PricklyPear"), 25.5f, 27.5f, 4, 1, 1.f, 1.0f, 0.05f, -1, 180.0f);
	Scatter(S1PLANTS("Reeds"), 28.5f, 31.0f, 8, 1, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 28.5f, 31.0f, 8, 5, 1.f, 1.0f, 0.06f, -1, 180.0f);
	Scatter(S1PLANTS("CarpetWave"), 32.0f, 34.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, The Point, a banked bend"), 13.9575f, 14.4551f);   // slots 309 to 319
	Scatter(S1PLANTS("UmbrellaPine"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("UmbrellaPine"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Agave"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.05f, -1, 180.0f);
	Scatter(S1PLANTS("UmbrellaPine"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The Point"), 14.4551f, 15.2695f);   // slots 320 to 337
	Scatter(S1PLANTS("UmbrellaPine"), 37.0f, 38.8f, 12, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);   // fix round: s % 12 == 11 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("Pergola"), 34.5f, 36.5f, 24, 1, 1.f, 1.0f, 0.00f, 1, 6.0f);   // fix round: s % 24 == 23 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("QuayTree"), 34.5f, 36.5f, 24, 13, 1.f, 1.0f, 0.10f, 1, 15.0f);   // fix round: s % 24 == 11 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("PricklyPear"), 20.8f, 23.0f, 24, 1, 1.f, 1.0f, 0.05f, 1, 180.0f);   // fix round: s % 24 == 23 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("Agave"), 20.8f, 23.0f, 24, 13, 1.f, 1.0f, 0.05f, 1, 180.0f);   // fix round: s % 24 == 11 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("UmbrellaPine"), 56.5f, 60.0f, 24, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);   // fix round: s % 24 == 23 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("UmbrellaPine"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Agave"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.05f, -1, 180.0f);
	Scatter(S1PLANTS("UmbrellaPine"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("PricklyPear"), 25.5f, 27.5f, 4, 1, 1.f, 1.0f, 0.05f, -1, 180.0f);
	Scatter(S1PLANTS("Reeds"), 28.5f, 31.0f, 8, 1, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 28.5f, 31.0f, 8, 5, 1.f, 1.0f, 0.06f, -1, 180.0f);
	Scatter(S1PLANTS("CarpetWave"), 32.0f, 34.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, The Point, a banked bend"), 15.2695f, 15.5410f);   // slots 338 to 343
	Scatter(S1PLANTS("UmbrellaPine"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("UmbrellaPine"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Agave"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.05f, -1, 180.0f);
	Scatter(S1PLANTS("UmbrellaPine"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The Point"), 15.5410f, 16.8078f);   // slots 344 to 371
	Scatter(S1PLANTS("UmbrellaPine"), 37.0f, 38.8f, 12, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);   // fix round: s % 12 == 11 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("Pergola"), 34.5f, 36.5f, 24, 17, 1.f, 1.0f, 0.00f, 1, 6.0f);   // fix round: s % 24 == 7 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("QuayTree"), 34.5f, 36.5f, 24, 13, 1.f, 1.0f, 0.10f, 1, 15.0f);   // fix round: s % 24 == 11 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("PricklyPear"), 20.8f, 23.0f, 24, 17, 1.f, 1.0f, 0.05f, 1, 180.0f);   // fix round: s % 24 == 7 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("Agave"), 20.8f, 23.0f, 24, 13, 1.f, 1.0f, 0.05f, 1, 180.0f);   // fix round: s % 24 == 11 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("UmbrellaPine"), 56.5f, 60.0f, 24, 17, 1.f, 1.0f, 0.10f, 1, 180.0f);   // fix round: s % 24 == 7 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1PLANTS("UmbrellaPine"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Agave"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.05f, -1, 180.0f);
	Scatter(S1PLANTS("UmbrellaPine"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("PricklyPear"), 25.5f, 27.5f, 4, 1, 1.f, 1.0f, 0.05f, -1, 180.0f);
	Scatter(S1PLANTS("Reeds"), 28.5f, 31.0f, 8, 1, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 28.5f, 31.0f, 8, 5, 1.f, 1.0f, 0.06f, -1, 180.0f);
	Scatter(S1PLANTS("CarpetWave"), 32.0f, 34.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, Downtown waterfront"), 16.8078f, 16.9887f);   // slots 372 to 375
	Scatter(S1PLANTS("FigBalls"), 34.5f, 36.5f, 4, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("RoyalPalms"), 37.0f, 38.8f, 8, 1, 1.f, 1.0f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("CarpetNeon"), 20.8f, 23.0f, 4, 1, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("Topiary"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("FigBalls"), 28.5f, 31.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
	Scatter(S1PLANTS("Topiary"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
Zone(TEXT("World s1plants, Downtown waterfront, a banked bend"), 16.9887f, 17.1697f);   // slots 376 to 379
	Scatter(S1PLANTS("RoyalPalms"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.08f, -1, 180.0f);
Zone(TEXT("World s1plants, Downtown waterfront"), 17.1697f, 17.6221f);   // slots 380 to 389
	Scatter(S1PLANTS("FigBalls"), 34.5f, 36.5f, 4, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("RoyalPalms"), 37.0f, 38.8f, 8, 1, 1.f, 1.0f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("Topiary"), 37.0f, 38.8f, 8, 5, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("CarpetNeon"), 20.8f, 23.0f, 4, 1, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("Topiary"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("FigBalls"), 28.5f, 31.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
	Scatter(S1PLANTS("CarpetNeon"), 28.5f, 31.0f, 8, 5, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("RoyalPalms"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("Topiary"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
Zone(TEXT("World s1plants, Downtown waterfront, a banked bend"), 17.6221f, 17.8484f);   // slots 390 to 394
	Scatter(S1PLANTS("Topiary"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("Topiary"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
Zone(TEXT("World s1plants, Downtown waterfront"), 17.8484f, 18.4365f);   // slots 395 to 407
	Scatter(S1PLANTS("FigBalls"), 34.5f, 36.5f, 4, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("RoyalPalms"), 37.0f, 38.8f, 8, 1, 1.f, 1.0f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("Topiary"), 37.0f, 38.8f, 8, 5, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("CarpetNeon"), 20.8f, 23.0f, 4, 1, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("Topiary"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("FigBalls"), 28.5f, 31.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
	Scatter(S1PLANTS("CarpetNeon"), 28.5f, 31.0f, 8, 5, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("RoyalPalms"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("Topiary"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
Zone(TEXT("World s1plants, Downtown waterfront, a banked bend"), 18.4365f, 18.6175f);   // slots 408 to 411
	Scatter(S1PLANTS("RoyalPalms"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.08f, -1, 180.0f);
Zone(TEXT("World s1plants, Downtown waterfront"), 18.6175f, 19.0699f);   // slots 412 to 421
	Scatter(S1PLANTS("FigBalls"), 34.5f, 36.5f, 4, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("RoyalPalms"), 37.0f, 38.8f, 8, 1, 1.f, 1.0f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("Topiary"), 37.0f, 38.8f, 8, 5, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("CarpetNeon"), 20.8f, 23.0f, 4, 1, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("Topiary"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("FigBalls"), 28.5f, 31.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
	Scatter(S1PLANTS("CarpetNeon"), 28.5f, 31.0f, 8, 5, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("RoyalPalms"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("Topiary"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
Zone(TEXT("World s1plants, Downtown waterfront, a banked bend"), 19.0699f, 19.2961f);   // slots 422 to 426
	Scatter(S1PLANTS("Topiary"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("Topiary"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
Zone(TEXT("World s1plants, Downtown waterfront"), 19.2961f, 19.8843f);   // slots 427 to 439
	Scatter(S1PLANTS("FigBalls"), 34.5f, 36.5f, 4, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("RoyalPalms"), 37.0f, 38.8f, 8, 1, 1.f, 1.0f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("Topiary"), 37.0f, 38.8f, 8, 5, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("CarpetNeon"), 20.8f, 23.0f, 4, 1, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("Topiary"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("FigBalls"), 28.5f, 31.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
	Scatter(S1PLANTS("CarpetNeon"), 28.5f, 31.0f, 8, 5, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("RoyalPalms"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("Topiary"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
Zone(TEXT("World s1plants, Downtown waterfront, a banked bend"), 19.8843f, 20.0653f);   // slots 440 to 443
	Scatter(S1PLANTS("RoyalPalms"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.08f, -1, 180.0f);
Zone(TEXT("World s1plants, Downtown waterfront"), 20.0653f, 20.5177f);   // slots 444 to 453
	Scatter(S1PLANTS("FigBalls"), 34.5f, 36.5f, 4, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("RoyalPalms"), 37.0f, 38.8f, 8, 1, 1.f, 1.0f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("Topiary"), 37.0f, 38.8f, 8, 5, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("CarpetNeon"), 20.8f, 23.0f, 4, 1, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("Topiary"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("FigBalls"), 28.5f, 31.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
	Scatter(S1PLANTS("CarpetNeon"), 28.5f, 31.0f, 8, 5, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("RoyalPalms"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("Topiary"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
Zone(TEXT("World s1plants, Downtown waterfront, a banked bend"), 20.5177f, 20.7439f);   // slots 454 to 458
	Scatter(S1PLANTS("Topiary"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("Topiary"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
Zone(TEXT("World s1plants, Downtown waterfront"), 20.7439f, 21.1059f);   // slots 459 to 466
	Scatter(S1PLANTS("FigBalls"), 34.5f, 36.5f, 4, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("RoyalPalms"), 37.0f, 38.8f, 8, 1, 1.f, 1.0f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("Topiary"), 37.0f, 38.8f, 8, 5, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("CarpetNeon"), 20.8f, 23.0f, 4, 1, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("Topiary"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("FigBalls"), 28.5f, 31.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
	Scatter(S1PLANTS("CarpetNeon"), 28.5f, 31.0f, 8, 5, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("RoyalPalms"), 40.0f, 47.0f, 8, 5, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("Topiary"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
Zone(TEXT("World s1plants, The Old Harbour"), 21.1059f, 21.3321f);   // slots 467 to 471
	Scatter(S1PLANTS("BigFig"), 37.5f, 39.5f, 8, 5, 1.f, 1.0f, 0.04f, 1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Geraniums"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, 1, 8.0f);
	Scatter(S1PLANTS("SeaGrape"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.06f, 1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Mangrove"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 49.0f, 58.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 15.0f);
	Scatter(S1PLANTS("Reeds"), 28.5f, 31.0f, 4, 1, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 25.5f, 27.5f, 8, 1, 1.f, 1.0f, 0.06f, -1, 180.0f);
	Scatter(S1PLANTS("Geraniums"), 25.5f, 27.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 8.0f);
	Scatter(S1PLANTS("CarpetWedge"), 32.0f, 34.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, The Old Harbour, a banked bend"), 21.3321f, 21.5130f);   // slots 472 to 475
	Scatter(S1PLANTS("QuayTree"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The Old Harbour"), 21.5130f, 21.9655f);   // slots 476 to 485
	Scatter(S1PLANTS("BigFig"), 37.5f, 39.5f, 8, 5, 1.f, 1.0f, 0.04f, 1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Geraniums"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, 1, 8.0f);
	Scatter(S1PLANTS("SeaGrape"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.06f, 1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Mangrove"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 49.0f, 58.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 15.0f);
	Scatter(S1PLANTS("Reeds"), 28.5f, 31.0f, 4, 1, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 25.5f, 27.5f, 8, 1, 1.f, 1.0f, 0.06f, -1, 180.0f);
	Scatter(S1PLANTS("Geraniums"), 25.5f, 27.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 8.0f);
	Scatter(S1PLANTS("CarpetWedge"), 32.0f, 34.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, The Old Harbour, a banked bend"), 21.9655f, 22.1917f);   // slots 486 to 490
	Scatter(S1PLANTS("Mangrove"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 49.0f, 58.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 15.0f);
Zone(TEXT("World s1plants, The Old Harbour"), 22.1917f, 22.7799f);   // slots 491 to 503
	Scatter(S1PLANTS("BigFig"), 37.5f, 39.5f, 8, 5, 1.f, 1.0f, 0.04f, 1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Geraniums"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, 1, 8.0f);
	Scatter(S1PLANTS("SeaGrape"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.06f, 1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Mangrove"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 49.0f, 58.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 15.0f);
	Scatter(S1PLANTS("Reeds"), 28.5f, 31.0f, 4, 1, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 25.5f, 27.5f, 8, 1, 1.f, 1.0f, 0.06f, -1, 180.0f);
	Scatter(S1PLANTS("Geraniums"), 25.5f, 27.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 8.0f);
	Scatter(S1PLANTS("CarpetWedge"), 32.0f, 34.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, The Old Harbour, a banked bend"), 22.7799f, 22.9608f);   // slots 504 to 507
	Scatter(S1PLANTS("QuayTree"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The Old Harbour"), 22.9608f, 23.4133f);   // slots 508 to 517
	Scatter(S1PLANTS("BigFig"), 37.5f, 39.5f, 8, 5, 1.f, 1.0f, 0.04f, 1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Geraniums"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, 1, 8.0f);
	Scatter(S1PLANTS("SeaGrape"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.06f, 1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Mangrove"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 49.0f, 58.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 15.0f);
	Scatter(S1PLANTS("Reeds"), 28.5f, 31.0f, 4, 1, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 25.5f, 27.5f, 8, 1, 1.f, 1.0f, 0.06f, -1, 180.0f);
	Scatter(S1PLANTS("Geraniums"), 25.5f, 27.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 8.0f);
	Scatter(S1PLANTS("CarpetWedge"), 32.0f, 34.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, The Old Harbour, a banked bend"), 23.4133f, 23.6395f);   // slots 518 to 522
	Scatter(S1PLANTS("Mangrove"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 49.0f, 58.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 15.0f);
Zone(TEXT("World s1plants, The Old Harbour"), 23.6395f, 24.2276f);   // slots 523 to 535
	Scatter(S1PLANTS("BigFig"), 37.5f, 39.5f, 8, 5, 1.f, 1.0f, 0.04f, 1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Geraniums"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, 1, 8.0f);
	Scatter(S1PLANTS("SeaGrape"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.06f, 1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Mangrove"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 49.0f, 58.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 15.0f);
	Scatter(S1PLANTS("Reeds"), 28.5f, 31.0f, 4, 1, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 25.5f, 27.5f, 8, 1, 1.f, 1.0f, 0.06f, -1, 180.0f);
	Scatter(S1PLANTS("Geraniums"), 25.5f, 27.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 8.0f);
	Scatter(S1PLANTS("CarpetWedge"), 32.0f, 34.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, The Old Harbour, a banked bend"), 24.2276f, 24.4086f);   // slots 536 to 539
	Scatter(S1PLANTS("QuayTree"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
Zone(TEXT("World s1plants, The Old Harbour"), 24.4086f, 24.8610f);   // slots 540 to 549
	Scatter(S1PLANTS("BigFig"), 37.5f, 39.5f, 8, 5, 1.f, 1.0f, 0.04f, 1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Geraniums"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, 1, 8.0f);
	Scatter(S1PLANTS("SeaGrape"), 20.8f, 23.0f, 8, 5, 1.f, 1.0f, 0.06f, 1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Mangrove"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 49.0f, 58.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 15.0f);
	Scatter(S1PLANTS("Reeds"), 28.5f, 31.0f, 4, 1, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 25.5f, 27.5f, 8, 1, 1.f, 1.0f, 0.06f, -1, 180.0f);
	Scatter(S1PLANTS("Geraniums"), 25.5f, 27.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 8.0f);
	Scatter(S1PLANTS("CarpetWedge"), 32.0f, 34.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
Zone(TEXT("World s1plants, The Old Harbour, a banked bend"), 24.8610f, 25.1325f);   // slots 550 to 555
	Scatter(S1PLANTS("QuayTree"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Mangrove"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Mangrove"), 49.0f, 58.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 49.0f, 58.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 15.0f);
Zone(TEXT("World s1plants, The Old Harbour"), 25.1325f, 99.f);   // slots 556 to 560
	Scatter(S1PLANTS("QuayTree"), 34.5f, 36.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 15.0f);
	Scatter(S1PLANTS("Geraniums"), 20.8f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, 1, 8.0f);
	Scatter(S1PLANTS("Mangrove"), 40.0f, 47.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("QuayTree"), 49.0f, 58.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 15.0f);
	Scatter(S1PLANTS("Reeds"), 28.5f, 31.0f, 4, 1, 1.f, 1.0f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaGrape"), 25.5f, 27.5f, 8, 1, 1.f, 1.0f, 0.06f, -1, 180.0f);
	Scatter(S1PLANTS("CarpetWedge"), 32.0f, 34.5f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
#undef S1PLANTS
// ---- END s1plants ----
}
{
// ---- BEGIN s1animals (the animals of the seven districts, ft_world_bay_s1animals.py) ----
// 20 meshes, /Game/World/Bayshore/SM_S1ANIMALS_*. Estimate (slots counted one by one; Scatter x 0.75, Far and Afloat x 1): about 264 placed.
// By mesh: PelicanPosts 35, FlyingFish 28, TernRing 20, Turtles 20, BeachDog 17, Spoonbills 16, SealRock 16, HeronReeds 14, Sandpipers 14, GoatWall 12, CatsPots 9, Crabs 9, Tortoise 9, Donkey 9, Doves 8, Peacocks 8, SwanPool 8, MacawPerch 6, CarriageHorse 5, Whale 2.
// Bands and slots (the brief's): ground 20 to 45 m in s%8==5 (phase 3 of 8; 3 or 11 of 16); 46 to 120 m in s%4==1 (phase 3 of 4, 7 of 8)
// and, from 61 m, s%4==3 (phase 1 of 4; 1 or 5 of 8); the sea (Afloat, bay side) 360 to 430 m and the whale 650 to 800 m; the sky (Far, a
// negative drop, bay side) 125 to 250 m out. TALL meshes: CarriageHorse (x min 29), GoatWall (26), SealRock (100); the other ground meshes are low.
// Every rule nearer than 40 m is written once a stretch BETWEEN the banked bends (the zone plan's list, each bend widened by 30 m).
// No mesh stands both inside and outside 70 m. The travelling pieces (Sandpipers, BeachDog) run along the beach 72 m and more out.
#define S1ANIMALS(n) TEXT("/Game/World/Bayshore/SM_S1ANIMALS_" n)
Set(TEXT("WldS1ANIMALSPelicans"), { { S1ANIMALS("PelicanPosts"), -12.f, 0.f, 0.f, 0.f, 1.f } });   // fix round: the Old Harbour's quay pelicans, placed by Site
// -- district 1, The Strip. Nearer than 40 m: not beside the banked bends (3 stretches).
Zone(TEXT("World s1animals, The Strip, by the road"), 0.f, 0.98f);
	Scatter(S1ANIMALS("MacawPerch"), 31.f, 36.f, 8, 3, 1.f, 1.f, 0.f, -1, 20.f);   // macaws on the sand by the walk (bay side: sun decks 26.4 m, the beach club 34 m in s%4==3, the bar 52 m)
Zone(TEXT("World s1animals, The Strip, by the road"), 1.26f, 2.22f);
	Scatter(S1ANIMALS("MacawPerch"), 31.f, 36.f, 8, 3, 1.f, 1.f, 0.f, -1, 20.f);
Zone(TEXT("World s1animals, The Strip, by the road"), 2.61f, 3.2f);
	Scatter(S1ANIMALS("MacawPerch"), 31.f, 36.f, 8, 3, 1.f, 1.f, 0.f, -1, 20.f);
Zone(TEXT("World s1animals, The Strip"), 0.f, 3.2f);
	Scatter(S1ANIMALS("PelicanPosts"), 61.f, 65.f, 8, 1, 1.f, 1.f, 0.f, -1, 8.f);   // pelicans further down the beach: s%8==7
	Scatter(S1ANIMALS("BeachDog"), 86.f, 94.f, 8, 1, 1.f, 1.2f, 0.f, -1, 4.f);   // a dog after the terns, along the beach: s%8==7
	Far(S1ANIMALS("TernRing"), 125.f, 150.f, 16, 2, 1.f, 1.f, 0.f, -1, -40.f, 180.f);   // terns wheeling 40 m over the road's level; the ring's radius is under 20 m: 125 - 20 = 105 m from the centre line at the nearest
// -- district 2, The Marina. Nearer than 40 m: not beside the banked bends (4 stretches).
Zone(TEXT("World s1animals, The Marina, by the road"), 3.2f, 3.71f);
	Scatter(S1ANIMALS("PelicanPosts"), 29.f, 32.f, 8, 3, 1.f, 1.f, 0.f, -1, 8.f);   // pelicans on mooring posts behind the walk
	Scatter(S1ANIMALS("CatsPots"), 37.f, 42.f, 8, 3, 1.f, 1.f, 0.f, -1, 25.f);   // cats on the lobster pots
Zone(TEXT("World s1animals, The Marina, by the road"), 4.01f, 4.49f);
	Scatter(S1ANIMALS("PelicanPosts"), 29.f, 32.f, 8, 3, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(S1ANIMALS("CatsPots"), 37.f, 42.f, 8, 3, 1.f, 1.f, 0.f, -1, 25.f);
Zone(TEXT("World s1animals, The Marina, by the road"), 4.76f, 5.6f);
	Scatter(S1ANIMALS("PelicanPosts"), 29.f, 32.f, 8, 3, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(S1ANIMALS("CatsPots"), 37.f, 42.f, 8, 3, 1.f, 1.f, 0.f, -1, 25.f);
Zone(TEXT("World s1animals, The Marina, by the road"), 5.88f, 6.4f);
	Scatter(S1ANIMALS("PelicanPosts"), 29.f, 32.f, 8, 3, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(S1ANIMALS("CatsPots"), 37.f, 42.f, 8, 3, 1.f, 1.f, 0.f, -1, 25.f);
Zone(TEXT("World s1animals, The Marina"), 3.2f, 6.4f);
	Scatter(S1ANIMALS("PelicanPosts"), 61.f, 66.f, 8, 5, 1.f, 1.f, 0.f, -1, 8.f);   // s%8==3
	Scatter(S1ANIMALS("HeronReeds"), 48.f, 58.f, 8, 7, 1.f, 1.f, 0.1f, -1, 40.f);   // egrets in a reed bed: s%8==1
	Afloat(S1ANIMALS("Turtles"), 360.f, 420.f, 7, 2, 1.f, 1.6f, 0.15f, -1, 180.f);   // sea turtles off the jetties
	Afloat(S1ANIMALS("FlyingFish"), 370.f, 430.f, 8, 1, 1.f, 1.5f, 0.1f, -1, 10.f);   // flying fish skipping along the shore
	Far(S1ANIMALS("Spoonbills"), 150.f, 250.f, 12, 3, 1.f, 1.f, 0.1f, -1, -22.f, 8.f);   // a line of pink spoonbills 22 m up, flying the road's way
	Far(S1ANIMALS("TernRing"), 125.f, 150.f, 16, 5, 1.f, 1.f, 0.f, -1, -40.f, 180.f);
// -- district 3, Pleasure Beach. Nearer than 40 m: not beside the banked bends (2 stretches).
Zone(TEXT("World s1animals, Pleasure Beach, by the road"), 6.4f, 7.96f);
	Scatter(S1ANIMALS("Crabs"), 36.f, 44.f, 8, 3, 1.f, 1.1f, 0.1f, -1, 40.f);   // crabs round a sandcastle (the beach clubs stand in even slots)
Zone(TEXT("World s1animals, Pleasure Beach, by the road"), 8.26f, 9.22f);
	Scatter(S1ANIMALS("Crabs"), 36.f, 44.f, 8, 3, 1.f, 1.1f, 0.1f, -1, 40.f);
Zone(TEXT("World s1animals, Pleasure Beach"), 6.4f, 9.8f);
	Scatter(S1ANIMALS("Sandpipers"), 88.f, 96.f, 8, 3, 1.f, 1.f, 0.f, -1, 4.f);   // sandpipers running the wet sand: s%8==5
	Scatter(S1ANIMALS("BeachDog"), 100.f, 105.f, 8, 5, 1.f, 1.2f, 0.f, -1, 4.f);   // s%8==3
	Afloat(S1ANIMALS("FlyingFish"), 370.f, 430.f, 12, 4, 1.f, 1.5f, 0.1f, -1, 10.f);
	Far(S1ANIMALS("Spoonbills"), 150.f, 250.f, 8, 1, 1.f, 1.f, 0.1f, -1, -22.f, 8.f);
// -- district 4, Hotel Row. Nearer than 40 m: not beside the banked bends (2 stretches).
Zone(TEXT("World s1animals, Hotel Row, by the road"), 10.05f, 10.95f);
	Scatter(S1ANIMALS("CarriageHorse"), 29.f, 32.f, 8, 3, 1.f, 1.f, 0.f, 1, 0.f);   // TALL (2.2 m): the carriage waits on the town side
	Scatter(S1ANIMALS("Doves"), 34.f, 40.f, 16, 3, 1.f, 1.f, 0.f, 1, 30.f);   // doves round a bird bath, town side: s%16==13
	Scatter(S1ANIMALS("Peacocks"), 34.f, 40.f, 16, 11, 1.f, 1.f, 0.f, 1, 10.f);   // peacocks by a balustrade, town side: s%16==5
	Scatter(S1ANIMALS("Peacocks"), 30.f, 38.f, 8, 3, 1.f, 1.f, 0.f, -1, 10.f);   // and on the beach under the hotels
Zone(TEXT("World s1animals, Hotel Row, by the road"), 11.24f, 13.2f);
	Scatter(S1ANIMALS("CarriageHorse"), 29.f, 32.f, 8, 3, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(S1ANIMALS("Doves"), 34.f, 40.f, 16, 3, 1.f, 1.f, 0.f, 1, 30.f);
	Scatter(S1ANIMALS("Peacocks"), 34.f, 40.f, 16, 11, 1.f, 1.f, 0.f, 1, 10.f);
	Scatter(S1ANIMALS("Peacocks"), 30.f, 38.f, 8, 3, 1.f, 1.f, 0.f, -1, 10.f);
Zone(TEXT("World s1animals, Hotel Row"), 9.8f, 13.2f);
	Scatter(S1ANIMALS("SwanPool"), 48.f, 56.f, 8, 7, 1.f, 1.f, 0.f, -1, 180.f);   // swans on a fountain pool between the lidos (those stand in even slots)
// -- district 5, The Point. Nearer than 40 m: not beside the banked bends (4 stretches).
Zone(TEXT("World s1animals, The Point, by the road"), 13.2f, 13.34f);
	Scatter(S1ANIMALS("GoatWall"), 26.f, 34.f, 24, 19, 1.f, 1.f, 0.f, 1, 0.f);   // fix round: s % 24 == 5 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1ANIMALS("Tortoise"), 21.f, 24.f, 24, 19, 1.f, 1.f, 0.1f, 1, 30.f);   // fix round: s % 24 == 5 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1ANIMALS("Tortoise"), 30.f, 36.f, 8, 3, 1.f, 1.f, 0.1f, -1, 60.f);   // and in the dunes
Zone(TEXT("World s1animals, The Point, by the road"), 13.67f, 13.97f);
	Scatter(S1ANIMALS("GoatWall"), 26.f, 34.f, 24, 19, 1.f, 1.f, 0.f, 1, 0.f);   // fix round: s % 24 == 5 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1ANIMALS("Tortoise"), 21.f, 24.f, 24, 19, 1.f, 1.f, 0.1f, 1, 30.f);   // fix round: s % 24 == 5 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1ANIMALS("Tortoise"), 30.f, 36.f, 8, 3, 1.f, 1.f, 0.1f, -1, 60.f);
Zone(TEXT("World s1animals, The Point, by the road"), 14.44f, 15.26f);
	Scatter(S1ANIMALS("GoatWall"), 26.f, 34.f, 24, 11, 1.f, 1.f, 0.f, 1, 0.f);   // fix round: s % 24 == 13 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1ANIMALS("Tortoise"), 21.f, 24.f, 24, 11, 1.f, 1.f, 0.1f, 1, 30.f);   // fix round: s % 24 == 13 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1ANIMALS("Tortoise"), 30.f, 36.f, 8, 3, 1.f, 1.f, 0.1f, -1, 60.f);
Zone(TEXT("World s1animals, The Point, by the road"), 15.55f, 16.8f);
	Scatter(S1ANIMALS("GoatWall"), 26.f, 34.f, 24, 19, 1.f, 1.f, 0.f, 1, 0.f);   // fix round: s % 24 == 5 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1ANIMALS("Tortoise"), 21.f, 24.f, 24, 19, 1.f, 1.f, 0.1f, 1, 30.f);   // fix round: s % 24 == 5 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1ANIMALS("Tortoise"), 30.f, 36.f, 8, 3, 1.f, 1.f, 0.1f, -1, 60.f);
Zone(TEXT("World s1animals, The Point"), 13.2f, 16.8f);
	Scatter(S1ANIMALS("GoatWall"), 46.f, 58.f, 24, 23, 1.f, 1.f, 0.f, 1, 0.f);   // fix round: s % 24 == 1 only (not the side streets' slots, s % 6 == 3)
	Scatter(S1ANIMALS("HeronReeds"), 48.5f, 58.f, 8, 7, 1.f, 1.f, 0.1f, -1, 40.f);   // egrets in the dune reeds: s%8==1 (fix round: from 48.5 m, clear of the painters' terrace at 43 m)
	Scatter(S1ANIMALS("Sandpipers"), 72.f, 100.f, 8, 1, 1.f, 1.f, 0.f, -1, 4.f);   // s%8==7
	Scatter(S1ANIMALS("SealRock"), 100.f, 118.f, 8, 3, 1.f, 1.f, 0.1f, -1, 60.f);   // TALL (2.5 m): seals hauled out among the breakwater rocks
	Afloat(S1ANIMALS("Whale"), 650.f, 800.f, 40, 10, 1.f, 1.6f, 0.f, -1, 25.f);   // the whale and her calf: slots 310 and 350 (14.0 and 15.8 km)
	Far(S1ANIMALS("TernRing"), 125.f, 150.f, 16, 0, 1.f, 1.f, 0.f, -1, -40.f, 180.f);
// -- district 6, Downtown waterfront. Nearer than 40 m: not beside the banked bends (7 stretches).
Zone(TEXT("World s1animals, Downtown waterfront, by the road"), 16.8f, 16.97f);
	Scatter(S1ANIMALS("Doves"), 37.f, 40.f, 16, 3, 1.f, 1.f, 0.f, 1, 30.f);   // pigeons and doves between the shop row and the palms
Zone(TEXT("World s1animals, Downtown waterfront, by the road"), 17.19f, 17.61f);
	Scatter(S1ANIMALS("Doves"), 37.f, 40.f, 16, 3, 1.f, 1.f, 0.f, 1, 30.f);
Zone(TEXT("World s1animals, Downtown waterfront, by the road"), 17.87f, 18.418f);
	Scatter(S1ANIMALS("Doves"), 37.f, 40.f, 16, 3, 1.f, 1.f, 0.f, 1, 30.f);
Zone(TEXT("World s1animals, Downtown waterfront, by the road"), 18.638f, 19.058f);
	Scatter(S1ANIMALS("Doves"), 37.f, 40.f, 16, 3, 1.f, 1.f, 0.f, 1, 30.f);
Zone(TEXT("World s1animals, Downtown waterfront, by the road"), 19.318f, 19.866f);
	Scatter(S1ANIMALS("Doves"), 37.f, 40.f, 16, 3, 1.f, 1.f, 0.f, 1, 30.f);
Zone(TEXT("World s1animals, Downtown waterfront, by the road"), 20.086f, 20.506f);
	Scatter(S1ANIMALS("Doves"), 37.f, 40.f, 16, 3, 1.f, 1.f, 0.f, 1, 30.f);
Zone(TEXT("World s1animals, Downtown waterfront, by the road"), 20.766f, 21.1f);
	Scatter(S1ANIMALS("Doves"), 37.f, 40.f, 16, 3, 1.f, 1.f, 0.f, 1, 30.f);
Zone(TEXT("World s1animals, Downtown waterfront"), 16.8f, 21.1f);
	Scatter(S1ANIMALS("BeachDog"), 100.f, 105.f, 16, 1, 1.f, 1.2f, 0.f, -1, 4.f);
	Afloat(S1ANIMALS("FlyingFish"), 370.f, 430.f, 16, 5, 1.f, 1.5f, 0.1f, -1, 10.f);
// -- district 7, The Old Harbour. Nearer than 40 m: not beside the banked bends (7 stretches).
Zone(TEXT("World s1animals, The Old Harbour, by the road"), 21.1f, 21.314f);
	Site(TEXT("WldS1ANIMALSPelicans"), -1, 30.f, 8, 3, 1.f);   // pelicans waiting on the quay posts (fix round: a Site, 12 m before the middle; scattered, 1 in 3 stood in the watchers' stand, 7.3 to 22.7 m after it)
	Scatter(S1ANIMALS("CatsPots"), 36.f, 42.f, 16, 3, 1.f, 1.f, 0.f, -1, 25.f);   // cats on the pots: s%16==13
	Scatter(S1ANIMALS("Crabs"), 36.f, 44.f, 16, 11, 1.f, 1.1f, 0.1f, -1, 40.f);   // crabs: s%16==5
	Scatter(S1ANIMALS("Donkey"), 37.f, 40.f, 8, 3, 1.f, 1.f, 0.f, 1, 0.f);   // the pannier donkey and her keeper, town side
Zone(TEXT("World s1animals, The Old Harbour, by the road"), 21.534f, 21.954f);
	Site(TEXT("WldS1ANIMALSPelicans"), -1, 30.f, 8, 3, 1.f);
	Scatter(S1ANIMALS("CatsPots"), 36.f, 42.f, 16, 3, 1.f, 1.f, 0.f, -1, 25.f);
	Scatter(S1ANIMALS("Crabs"), 36.f, 44.f, 16, 11, 1.f, 1.1f, 0.1f, -1, 40.f);
	Scatter(S1ANIMALS("Donkey"), 37.f, 40.f, 8, 3, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("World s1animals, The Old Harbour, by the road"), 22.214f, 22.762f);
	Site(TEXT("WldS1ANIMALSPelicans"), -1, 30.f, 8, 3, 1.f);
	Scatter(S1ANIMALS("CatsPots"), 36.f, 42.f, 16, 3, 1.f, 1.f, 0.f, -1, 25.f);
	Scatter(S1ANIMALS("Crabs"), 36.f, 44.f, 16, 11, 1.f, 1.1f, 0.1f, -1, 40.f);
	Scatter(S1ANIMALS("Donkey"), 37.f, 40.f, 8, 3, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("World s1animals, The Old Harbour, by the road"), 22.982f, 23.402f);
	Site(TEXT("WldS1ANIMALSPelicans"), -1, 30.f, 8, 3, 1.f);
	Scatter(S1ANIMALS("CatsPots"), 36.f, 42.f, 16, 3, 1.f, 1.f, 0.f, -1, 25.f);
	Scatter(S1ANIMALS("Crabs"), 36.f, 44.f, 16, 11, 1.f, 1.1f, 0.1f, -1, 40.f);
	Scatter(S1ANIMALS("Donkey"), 37.f, 40.f, 8, 3, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("World s1animals, The Old Harbour, by the road"), 23.662f, 24.21f);
	Site(TEXT("WldS1ANIMALSPelicans"), -1, 30.f, 8, 3, 1.f);
	Scatter(S1ANIMALS("CatsPots"), 36.f, 42.f, 16, 3, 1.f, 1.f, 0.f, -1, 25.f);
	Scatter(S1ANIMALS("Crabs"), 36.f, 44.f, 16, 11, 1.f, 1.1f, 0.1f, -1, 40.f);
	Scatter(S1ANIMALS("Donkey"), 37.f, 40.f, 8, 3, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("World s1animals, The Old Harbour, by the road"), 24.43f, 24.85f);
	Site(TEXT("WldS1ANIMALSPelicans"), -1, 30.f, 8, 3, 1.f);
	Scatter(S1ANIMALS("CatsPots"), 36.f, 42.f, 16, 3, 1.f, 1.f, 0.f, -1, 25.f);
	Scatter(S1ANIMALS("Crabs"), 36.f, 44.f, 16, 11, 1.f, 1.1f, 0.1f, -1, 40.f);
	Scatter(S1ANIMALS("Donkey"), 37.f, 40.f, 8, 3, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("World s1animals, The Old Harbour, by the road"), 25.11f, 99.f);
	Site(TEXT("WldS1ANIMALSPelicans"), -1, 30.f, 8, 3, 1.f);
	Scatter(S1ANIMALS("CatsPots"), 36.f, 42.f, 16, 3, 1.f, 1.f, 0.f, -1, 25.f);
	Scatter(S1ANIMALS("Crabs"), 36.f, 44.f, 16, 11, 1.f, 1.1f, 0.1f, -1, 40.f);
	Scatter(S1ANIMALS("Donkey"), 37.f, 40.f, 8, 3, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("World s1animals, The Old Harbour"), 21.1f, 99.f);
	Scatter(S1ANIMALS("PelicanPosts"), 61.f, 66.f, 8, 1, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(S1ANIMALS("SealRock"), 104.f, 112.f, 8, 7, 1.f, 1.f, 0.1f, -1, 60.f);   // a harbour seal rock: s%8==1
	Afloat(S1ANIMALS("Turtles"), 360.f, 420.f, 10, 3, 1.f, 1.6f, 0.15f, -1, 180.f);
	Afloat(S1ANIMALS("FlyingFish"), 370.f, 430.f, 12, 2, 1.f, 1.5f, 0.1f, -1, 10.f);
	Far(S1ANIMALS("TernRing"), 125.f, 150.f, 16, 7, 1.f, 1.f, 0.f, -1, -40.f, 180.f);
#undef S1ANIMALS
// ---- END s1animals ----
}
{
// ---- BEGIN s1people (the people pass 3 left out, ft_world_bay_s1people.py) ----
// 19 meshes, all made by the one make run (C:/ftwork/logs/L0s1people_make.log: built 19 failed 0). All TALL (tops 3.9 to 7.4 m),
// each on its own raised floor (1.5 to 2.6 m over the land, 3 m of footing), 7.1 m long at most. No glow. No Lit, no Bank.
// Slots: s % 8 == 1 only (every 8 phase 7; every 24 phase 23 / 15 / 7 where a slot of three is left out).
// Distance: every Site is anchored 43 m from the centre line, both sides, every piece at out 0 (pieces reach from 40.0 to 46.0 m
// at the most: the round podium and the round dais; the others 40.6 to 45.3): behind the banked bends' cuttings (40 m), before the animals' band (46 m).
// Along the slot: town side 17.5 m before and after the slot's middle (the cafe, 44 m, and the market, 42 m, fill the middle 23 m; the
// hills' walkway the middle 8 m). Bay side 19.5 m before the middle (the third pass's beach scenes stand 42.5 m out from 14.7 to 1.3 m
// before it; the watchers' stand 30 m out from 8 m after it), a second piece 4.5 m after it, a third 19 m after it where no stand is.
// Left out: the Point's town side in s % 24 == 17 (the dressing scatters SM_Pool 40 to 60 m out in s % 3 == 2); the second bay piece in
// Downtown and the Old Harbour in s % 24 == 17 (the beach club's path, 37 m out, 6 m after the middle, in s % 3 == 2).
// Estimate, counted slot by slot (slots with s % 8 == 1: Strip 9, Marina 9, Pleasure Beach 9, Hotel Row 10, Point 10, Downtown 12, Old Harbour 11):
//   Strip 9x2 + 9x2 = 36; Marina 9x2 + 9x3 = 45; Pleasure Beach 9x2 + 9x3 = 45; Hotel Row 10x2 + 10x2 = 40; Point 7x2 + 10x3 = 44;
//   Downtown 12x2 + 8x2 + 4x1 = 44; Old Harbour 11x2 + 7x2 + 4x1 = 40. Sum 294 piece-places x 0.75 (the track's share left out) = about 220 placed.
//   The most of one mesh: SM_S1PEOPLE_PhotoRiser, 24 places before the 0.75 (about 18). The smallest x min of a TALL mesh: 43 m.
#define S1PEOPLE(n) TEXT("/Game/World/Bayshore/SM_S1PEOPLE_" n)
Set(TEXT("WldS1PEOPLEStripTown"), {      // dancing round the jukebox podium; waving from the diner steps
	{ S1PEOPLE("JukePodium"), -17.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("DinerSteps"),  17.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEStripBay"), {       // a parade wagon drawn up on the sand; a second podium
	{ S1PEOPLE("Wagon_A"),    -19.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("JukePodium"),   4.5f, 1.2f, 0.f, 0.f, 1.f },   // fix round: out 1.2, clear of the beach path's walkers (to 41.3 m)
});
Set(TEXT("WldS1PEOPLEMarinaTown"), {     // net menders on a quay deck; a boarded veranda
	{ S1PEOPLE("NetDeck"),    -17.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Veranda_A"),   17.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEMarinaBay"), {      // a dinghy painted from a plank; the menders; the veranda
	{ S1PEOPLE("HullPainters"), -19.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("NetDeck"),        4.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Veranda_A"),     19.0f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEPleasureTown"), {   // strongman, juggler and ringmaster on show plinths; a parade wagon
	{ S1PEOPLE("ShowPlinths"), -17.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Wagon_B"),      17.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEPleasureBay"), {    // stilt walkers over a raised bench; the plinths; the wagon
	{ S1PEOPLE("StiltWalkers"), -14.0f, 0.f, 0.f, 0.f, 1.f },   // fix round: were -19.5 and 19.0, round the chair lift's masts (20.4 m either way, 44 m out)
	{ S1PEOPLE("ShowPlinths"),    4.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Wagon_B"),       13.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEHotelTown"), {      // guests on a balustraded terrace; a string trio on a dais
	{ S1PEOPLE("BalusTerrace"), -17.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("StringTrio"),    17.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEHotelBay"), {
	{ S1PEOPLE("StringTrio"),   -19.5f, 1.2f, 0.f, 0.f, 1.f },   // fix round: out 1.2, as the Strip's podium
	{ S1PEOPLE("BalusTerrace"),   4.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEPointTown"), {      // painters at easels on a garden-wall terrace; a picnic over a dry-stone wall
	{ S1PEOPLE("PaintersTerrace"), -17.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PicnicWall"),       17.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEPointBay"), {
	{ S1PEOPLE("PicnicWall"),      -19.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PaintersTerrace"),   4.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Veranda_B"),        19.0f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEDownTown"), {       // a premiere crowd on steps behind rope posts; photographers on a riser
	{ S1PEOPLE("PremiereSteps"), -15.0f, 0.f, 0.f, 0.f, 1.f },   // fix round: was -17.5, in the corner of the department store of the slot before
	{ S1PEOPLE("PhotoRiser"),     17.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEDownBay"), {
	{ S1PEOPLE("PhotoRiser"),    -19.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PremiereSteps"),   4.5f, 0.6f, 0.f, 0.f, 1.f },   // fix round: out 0.6, as the Strip's podium
});
Set(TEXT("WldS1PEOPLEDownBayOne"), {
	{ S1PEOPLE("PhotoRiser"),    -19.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEHarbourTown"), {    // fishwives at raised stalls; an accordion player on a plinth, dancers on a stage
	{ S1PEOPLE("FishStalls"), -17.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Accordion"),   17.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEHarbourBay"), {     // four hauling a net over a quay wall; the stalls
	{ S1PEOPLE("NetHaulers"), -19.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("FishStalls"),   4.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEHarbourBayOne"), {
	{ S1PEOPLE("NetHaulers"), -19.5f, 0.f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1people, the Strip"), 0.f, 3.2f);
	Site(TEXT("WldS1PEOPLEStripTown"), 1, 43.f, 8, 7, 1.f);                 // s % 8 == 1
	Site(TEXT("WldS1PEOPLEStripBay"), -1, 43.f, 8, 7, 1.f);
Zone(TEXT("World s1people, the Marina"), 3.2f, 6.4f);
	Site(TEXT("WldS1PEOPLEMarinaTown"), 1, 43.f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLEMarinaBay"), -1, 43.f, 8, 7, 1.f);
Zone(TEXT("World s1people, Pleasure Beach"), 6.4f, 9.8f);
	Site(TEXT("WldS1PEOPLEPleasureTown"), 1, 43.f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLEPleasureBay"), -1, 43.f, 8, 7, 1.f);
Zone(TEXT("World s1people, Hotel Row"), 9.8f, 13.2f);
	Site(TEXT("WldS1PEOPLEHotelTown"), 1, 43.f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLEHotelBay"), -1, 43.f, 8, 7, 1.f);
Zone(TEXT("World s1people, the Point"), 13.2f, 16.8f);
	Site(TEXT("WldS1PEOPLEPointTown"), 1, 43.f, 24, 23, 1.f);               // s % 24 == 1
	Site(TEXT("WldS1PEOPLEPointTown"), 1, 43.f, 24, 15, 1.f);               // s % 24 == 9 (17: the dressing's pools)
	Site(TEXT("WldS1PEOPLEPointBay"), -1, 43.f, 8, 7, 1.f);
Zone(TEXT("World s1people, Downtown waterfront"), 16.8f, 21.1f);
	Site(TEXT("WldS1PEOPLEDownTown"), 1, 43.f, 8, 7, 1.f);
// Fix round: the bay side is written a stretch at a time, WITHOUT slots 377, 393, 409 and 425: there the chair lift's mast (44 m out,
// 20.4 m before the middle) stood in the photographers' riser, the beach scene fills the gap beside it, and the steps reached the chairs.
Zone(TEXT("World s1people, Downtown waterfront, bay, slots 378 to 392"), 17.07923f, 17.75788f);
	Site(TEXT("WldS1PEOPLEDownBay"), -1, 43.f, 24, 23, 1.f);                // s % 24 == 1: slot 385
Zone(TEXT("World s1people, Downtown waterfront, bay, slots 394 to 408"), 17.80312f, 18.48177f);
	Site(TEXT("WldS1PEOPLEDownBayOne"), -1, 43.f, 24, 7, 1.f);              // s % 24 == 17: slot 401 (the beach club's path)
Zone(TEXT("World s1people, Downtown waterfront, bay, slots 410 to 424"), 18.52701f, 19.20565f);
	Site(TEXT("WldS1PEOPLEDownBay"), -1, 43.f, 24, 15, 1.f);                // s % 24 == 9: slot 417
Zone(TEXT("World s1people, Downtown waterfront, bay, slots 426 to 466"), 19.25090f, 21.1f);
	Site(TEXT("WldS1PEOPLEDownBay"), -1, 43.f, 24, 23, 1.f);                // s % 24 == 1
	Site(TEXT("WldS1PEOPLEDownBay"), -1, 43.f, 24, 15, 1.f);                // s % 24 == 9
	Site(TEXT("WldS1PEOPLEDownBayOne"), -1, 43.f, 24, 7, 1.f);              // s % 24 == 17: the beach club's path
Zone(TEXT("World s1people, the Old Harbour"), 21.1f, 99.f);
	Site(TEXT("WldS1PEOPLEHarbourTown"), 1, 43.f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLEHarbourBay"), -1, 43.f, 24, 23, 1.f);
	Site(TEXT("WldS1PEOPLEHarbourBay"), -1, 43.f, 24, 15, 1.f);
	Site(TEXT("WldS1PEOPLEHarbourBayOne"), -1, 43.f, 24, 7, 1.f);
#undef S1PEOPLE
// ---- END s1people ----
}
{
// ---- BEGIN s1fix (the one fix round after Stage 1's review; SourceArt/Integration/world/L0_bay_s1.md, "Review and fix round") ----
// In the Point the side streets and their cars lie in the slots s % 6 == 3 (FTDressL0.inl). The town-side plant and animal rules
// of s1plants and s1animals stood a piece in them about 1 time in 2; each of those rules now keeps one slot in three (written at
// its old place, so its pieces stand where they stood) and the rules below add the other slot that is no side street.
// They stand at the very end because a rule's random numbers come from its place in the table: nothing before them moves.
#define S1PLANTS(n) TEXT("/Game/World/Bayshore/SM_S1PLANTS_" n)
#define S1ANIMALS(n) TEXT("/Game/World/Bayshore/SM_S1ANIMALS_" n)
Zone(TEXT("World s1plants, The Point, fix round"), 14.4551f, 15.2695f);
	Scatter(S1PLANTS("UmbrellaPine"), 37.0f, 38.8f, 12, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);   // s % 12 == 7
	Scatter(S1PLANTS("QuayTree"), 34.5f, 36.5f, 24, 5, 1.f, 1.0f, 0.10f, 1, 15.0f);   // s % 24 == 19
	Scatter(S1PLANTS("Agave"), 20.8f, 23.0f, 24, 5, 1.f, 1.0f, 0.05f, 1, 180.0f);   // s % 24 == 19
Zone(TEXT("World s1plants, The Point, fix round"), 15.5410f, 16.8078f);
	Scatter(S1PLANTS("UmbrellaPine"), 37.0f, 38.8f, 12, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);   // s % 12 == 7
	Scatter(S1PLANTS("Pergola"), 34.5f, 36.5f, 24, 1, 1.f, 1.0f, 0.00f, 1, 6.0f);   // s % 24 == 23
	Scatter(S1PLANTS("QuayTree"), 34.5f, 36.5f, 24, 5, 1.f, 1.0f, 0.10f, 1, 15.0f);   // s % 24 == 19
	Scatter(S1PLANTS("PricklyPear"), 20.8f, 23.0f, 24, 1, 1.f, 1.0f, 0.05f, 1, 180.0f);   // s % 24 == 23
	Scatter(S1PLANTS("Agave"), 20.8f, 23.0f, 24, 5, 1.f, 1.0f, 0.05f, 1, 180.0f);   // s % 24 == 19
	Scatter(S1PLANTS("UmbrellaPine"), 56.5f, 60.0f, 24, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);   // s % 24 == 23
Zone(TEXT("World s1animals, The Point, by the road, fix round"), 15.55f, 16.8f);
	Scatter(S1ANIMALS("GoatWall"), 26.f, 34.f, 24, 11, 1.f, 1.f, 0.f, 1, 0.f);   // s % 24 == 13
	Scatter(S1ANIMALS("Tortoise"), 21.f, 24.f, 24, 11, 1.f, 1.f, 0.1f, 1, 30.f);   // s % 24 == 13
Zone(TEXT("World s1animals, The Point, fix round"), 13.2f, 16.8f);
	Scatter(S1ANIMALS("GoatWall"), 46.f, 58.f, 24, 7, 1.f, 1.f, 0.f, 1, 0.f);   // s % 24 == 17
#undef S1PLANTS
#undef S1ANIMALS
// ---- END s1fix ----
}
}
