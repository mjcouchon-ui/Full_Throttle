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
	Scatter(FARP(Flock),    65, 160, 11, 1, 1.f, 1.4f,  0.15f, -1, 180.f);
	Scatter(FARP(Crowd),   110, 180,  9, 3, 1.f, 1.25f,  0.1f,  -1, 8.f);
	Scatter(FARP(Kites),    70, 180, 16, 6, 1.f, 1.3f,  0.1f,  -1, 10.f);
	Afloat(FARP(Surf),     380, 390, 16, 2, 1.f, 1.5f,  0.f,   -1, 6.f);
	Afloat(FARP(Dolphins), 400, 460, 30, 8, 1.f, 1.3f,  0.1f,  -1, 8.f);
	Afloat(FARP(Pelicans), 350, 420, 32, 13, 1.f, 1.5f, 0.1f,  -1, 8.f);
Zone(TEXT("People far: the Marina"), 3.2f, 6.4f);          // quieter sand: birds over the boats, dolphins off the jetties
	Scatter(FARP(Flock),    65, 160,  9, 2, 1.f, 1.4f,  0.15f, -1, 180.f);
	Afloat(FARP(Pelicans), 350, 430, 16, 5, 1.f, 1.5f,  0.1f,  -1, 8.f);
	Afloat(FARP(Dolphins), 400, 470, 15, 9, 1.f, 1.3f,  0.1f,  -1, 8.f);
	Scatter(FARP(Crowd),   110, 180, 26, 12, 1.f, 1.25f, 0.1f,  -1, 8.f);
	Scatter(FARP(Kites),    70, 180, 34, 20, 1.f, 1.3f, 0.1f,  -1, 10.f);
Zone(TEXT("People far: Pleasure Beach"), 6.4f, 9.8f);      // the beach at its busiest
	Scatter(FARP(Crowd),   110, 180,  7, 1, 1.f, 1.25f,  0.1f,  -1, 8.f);
	Scatter(FARP(Kites),    70, 180, 11, 4, 1.f, 1.3f,  0.1f,  -1, 10.f);
	Afloat(FARP(Surf),     380, 390, 13, 6, 1.f, 1.5f,  0.f,   -1, 6.f);
	Scatter(FARP(Flock),    65, 160, 17, 9, 1.f, 1.4f,  0.15f, -1, 180.f);
	Afloat(FARP(Dolphins), 400, 460, 38, 20, 1.f, 1.3f, 0.1f,  -1, 8.f);
	Afloat(FARP(Pelicans), 350, 420, 38, 30, 1.f, 1.5f, 0.1f,  -1, 8.f);
Zone(TEXT("People far: Hotel Row"), 9.8f, 13.2f);          // surfers; gulls over the hotels' roofs
	Afloat(FARP(Surf),     380, 390, 11, 3, 1.f, 1.5f,  0.f,   -1, 6.f);
	Scatter(FARP(Crowd),   110, 180, 15, 7, 1.f, 1.25f,  0.1f,  -1, 8.f);
	Scatter(FARP(Flock),    65, 160, 15, 11, 1.f, 1.4f, 0.15f, -1, 180.f);
	Scatter(FARP(Kites),    70, 180, 22, 14, 1.f, 1.3f, 0.1f,  -1, 10.f);
	Afloat(FARP(Pelicans), 350, 420, 28, 9, 1.f, 1.5f,  0.1f,  -1, 8.f);
	Afloat(FARP(Dolphins), 400, 460, 28, 23, 1.f, 1.3f, 0.1f,  -1, 8.f);
	Far(FARP(Flock),       140, 260, 14, 5, 1.f, 1.6f, 0.1f,   1, -25.f, 180.f);
Zone(TEXT("People far: the Point"), 13.2f, 16.8f);         // the town thins out: birds and dolphins
	Scatter(FARP(Flock),    65, 160,  8, 0, 1.f, 1.4f,  0.15f, -1, 180.f);
	Afloat(FARP(Dolphins), 400, 470, 11, 4, 1.f, 1.3f,  0.1f,  -1, 8.f);
	Afloat(FARP(Pelicans), 350, 430, 13, 7, 1.f, 1.5f,  0.1f,  -1, 8.f);
	Afloat(FARP(Surf),     380, 390, 19, 10, 1.f, 1.5f, 0.f,   -1, 6.f);
	Scatter(FARP(Kites),    70, 180, 30, 16, 1.f, 1.3f, 0.1f,  -1, 10.f);
	Scatter(FARP(Crowd),   110, 180, 36, 25, 1.f, 1.25f, 0.1f,  -1, 8.f);
Zone(TEXT("People far: Downtown waterfront"), 16.8f, 99.f); // everything at once
	Scatter(FARP(Crowd),   110, 180, 11, 2, 1.f, 1.25f,  0.1f,  -1, 8.f);
	Scatter(FARP(Flock),    65, 160, 13, 5, 1.f, 1.4f,  0.15f, -1, 180.f);
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
// (s % 4 == 0, no shop row there), 34 m out, behind where the film crew sets up; a platform in the other side streets.
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
	Site(TEXT("PplWatchTerrace"), 1, 34.f, 8, 0, 1.f);   // s % 8 == 0: town, a side street's mouth
	Site(TEXT("PplWatchTowerA"), 1, 31.f, 8, 4, 1.f);   // s % 8 == 4: town, the other side streets
Zone(TEXT("People watch: the Marina"), 3.2f, 6.4f);
	Site(TEXT("PplWatchStandA"), -1, 30.f, 8, 0, 1.f);   // s % 8 == 0: bay (arcade s%4==2, shops s%6==3)
	Site(TEXT("PplWatchStandB"), -1, 30.f, 8, 4, 1.f);   // s % 8 == 4
	Site(TEXT("PplWatchTowerB"), -1, 28.f, 16, 10, 1.f);   // s % 16 == 6
	Site(TEXT("PplWatchTerrace"), 1, 34.f, 8, 0, 1.f);   // s % 8 == 0: town, a side street's mouth
	Site(TEXT("PplWatchTowerA"), 1, 31.f, 8, 4, 1.f);   // s % 8 == 4: town, the other side streets
Zone(TEXT("People watch: Pleasure Beach"), 6.4f, 9.8f);
	Site(TEXT("PplWatchStandA"), -1, 30.f, 8, 6, 1.f);   // s % 8 == 2: bay (arcades and shops in odd slots, the main cast's sun deck s%8==0)
	Site(TEXT("PplWatchStandB"), -1, 30.f, 8, 2, 1.f);   // s % 8 == 6
	Site(TEXT("PplWatchDeck"), -1, 30.5f, 16, 12, 1.f);   // s % 16 == 4
	Site(TEXT("PplWatchTowerA"), -1, 28.f, 16, 4, 1.f);   // s % 16 == 12
	Site(TEXT("PplWatchTerrace"), 1, 34.f, 8, 0, 1.f);   // s % 8 == 0: town, a side street's mouth
	Site(TEXT("PplWatchTowerA"), 1, 31.f, 8, 4, 1.f);   // s % 8 == 4: town, the other side streets
Zone(TEXT("People watch: Hotel Row"), 9.8f, 13.2f);
	Site(TEXT("PplWatchStandA"), -1, 30.f, 8, 6, 1.f);   // s % 8 == 2: bay (arcades and shops in odd slots, the main cast's sun deck s%8==0)
	Site(TEXT("PplWatchStandB"), -1, 30.f, 8, 2, 1.f);   // s % 8 == 6
	Site(TEXT("PplWatchDeck"), -1, 30.5f, 16, 12, 1.f);   // s % 16 == 4
	Site(TEXT("PplWatchTowerA"), -1, 28.f, 16, 4, 1.f);   // s % 16 == 12
	Site(TEXT("PplWatchTerrace"), 1, 34.f, 8, 0, 1.f);   // s % 8 == 0: town, a side street's mouth
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
	Site(TEXT("PplWatchTerrace"), 1, 34.f, 8, 0, 1.f);   // s % 8 == 0: town, a side street's mouth
	Site(TEXT("PplWatchTowerA"), 1, 31.f, 8, 4, 1.f);   // s % 8 == 4: town, the other side streets
// ---- END watch ----
}
