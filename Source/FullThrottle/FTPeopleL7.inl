// City Center (level index 7): the population layer. People, creatures and film crews, placed by rule like the dressing
// (the words and what their numbers mean are in the header of FTTrackDress.cpp; the rule kinds in FTDress.h).
// This table is read after FTDressL7.inl: start with a Zone line, and every rule here comes after every dressing
// rule, so the dressing's own placements never move. A mesh name without a leading slash is looked up in
// /Game/Maps/Metro/Dress/; give a full path for the population's own meshes, e.g. TEXT("/Game/People/SM_CrowdCheerA").
// Rules that bind this layer: nothing over 2 m tall nearer than 24 m to a centre line and nothing at all nearer than
// 17 m; nothing lit that blinks nearer than 30 m; nothing that could be taken for a pickup, an enemy or an attack.

// WHO IS HERE (the meshes are made by Content/Python/ft_people_city.py; its header says how the street lies):
//   the city has come out to watch the last scene being shot. Onlookers behind police barricades, marches behind long
//   striped banners (one held at the kerb by a police line), commuters walking home past the lit windows, a pretzel cart
//   and its queue, the queue for the late show behind a red rope, an old hand feeding pigeons; parties and rallies on
//   the podiums' roofs under strings of lights; window cleaners in a cradle on the glass infills; a rally on the plaza
//   before each monument; pigeons wheeling over the side streets and, big, over the far roofs; and from Penthouse Row
//   on the Chairman's own men in black and gold, at the kerb and up on his black walls. The film crew is at the start,
//   before every corkscrew, jump and loop and inside them (Far rules), before the boss's arena, and thinly through it.
//
// WHERE THINGS MAY STAND, and why every rule below has the numbers it has:
//   POCKETS  The kerb (17 to 22.6 m) is full of the dressing's cars, benches, shelters, cafes and works in every slot.
//            Free in EVERY slot is a pocket in front of the gap between two podiums: 17.5 to 23.7 m along from the
//            slot's anchor, 19.4 to 23.1 m out (20.6 m and more in Syndicate Territory, behind its barrier rows). The
//            "Ppl..." pocket sets stand one low piece there (anchor 17.6 m like the dressing's kerb sets). A pocket
//            rule copies the every-and-phase of the dressing's GAP rule it stands before (G_Alley, G_Fill, G_Monument,
//            G_Banner ...) and leaves out the gaps that hold a side street (G_Street: cars drive there).
//   THE STRIP  23.5 to 24.1 m out is free the whole length of every block: SM_Commuters walk it (Scatter at 23.72 m).
//              The picture-house queue stands 22.9 m out, 2 to 14 m along from a W_Marquee's anchor.
//   ROOFS    Tall pieces stand ON the dressing: a "PplRoof..." set copies the every-and-phase of the podium rule
//            (W_Shops2s, W_Shops1, W_Marquee, W_Garage, W_Yards, W_Pent, W_Syn) and its "up" is that podium's roof
//            (5.48 m shops, 11.82 m picture house, 12.37 m garage, 6.64 m yard, 9.5 m dear doors, 9 m the black wall).
//            If a podium rule's every, phase or side is changed in FTDressL7.inl, change it here too, or a party floats.
//   GAPS     SM_WindowCleaners_A stands at SM_Infill2 (G_Fill's own offsets), SM_Rally_A on the plaza before
//            SM_Monument (24.4 to 27.3 m out), the small flock over a side street.
//   LIGHT    Only pieces stood 24 m and more out carry light (steady: strings of bulbs, a lit floor, a faint light on
//            the figures). Nothing here blinks. Nothing lit is nearer than 24 m.
// Counts (from the game's FTCLEAR line) are in SourceArt/Integration/people/L7_city.md.

// ---- the sets. Pocket sets: anchor 17.6 m. Roof, gap and queue sets: anchor 29 m (the podiums' own).
Set(TEXT("PplOnA"), { { TEXT("/Game/People/CityCenter/SM_Onlookers_A"), 20.6f, 3.9f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplOnB"), { { TEXT("/Game/People/CityCenter/SM_Onlookers_B"), 20.6f, 3.9f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplMarA"), { { TEXT("/Game/People/CityCenter/SM_March_A"), 20.6f, 3.9f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplMarB"), { { TEXT("/Game/People/CityCenter/SM_March_B"), 20.6f, 4.3f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplVend"), { { TEXT("/Game/People/CityCenter/SM_Vendor_A"), 20.6f, 4.0f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplPig"), { { TEXT("/Game/People/CityCenter/SM_Pigeons_A"), 20.6f, 4.0f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplSyn"), { { TEXT("/Game/People/CityCenter/SM_Syndicate_A"), 20.6f, 4.4f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplOnAs"), { { TEXT("/Game/People/CityCenter/SM_Onlookers_A"), 20.6f, 4.7f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplOnBs"), { { TEXT("/Game/People/CityCenter/SM_Onlookers_B"), 20.6f, 4.7f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplCrewCam"), { { TEXT("/Game/People/Crew/SM_CrewCamera_City"), 20.6f, 4.2f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplCrewAppA"), { { TEXT("/Game/People/Crew/SM_CrewApplauseA_City"), 20.0f, 3.8f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplCrewAppB"), { { TEXT("/Game/People/Crew/SM_CrewApplauseB_City"), 20.0f, 3.8f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplCrewStunt"), { { TEXT("/Game/People/Crew/SM_CrewStunt_City"), 20.0f, 3.9f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplCrewVil"), { { TEXT("/Game/People/Crew/SM_CrewVillage_City"), 20.6f, 4.1f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplCrewAppAs"), { { TEXT("/Game/People/Crew/SM_CrewApplauseA_City"), 20.0f, 4.4f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplQueue"), { { TEXT("/Game/People/CityCenter/SM_Queue_A"), 8.0f, -6.1f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplRoofMidA"), { { TEXT("/Game/People/CityCenter/SM_RoofParty_A"), 0.0f, 0.0f, 5.48f, 0.0f, 1.0f } });
Set(TEXT("PplRoofMidB"), { { TEXT("/Game/People/CityCenter/SM_RoofParty_B"), 0.0f, 0.0f, 5.48f, 0.0f, 1.0f } });
Set(TEXT("PplRoofMidR"), { { TEXT("/Game/People/CityCenter/SM_RoofRally_A"), 0.0f, 0.0f, 5.48f, 0.0f, 1.0f } });
Set(TEXT("PplRoofS1"), { { TEXT("/Game/People/CityCenter/SM_RoofRally_A"), 10.67f, 0.0f, 5.48f, 0.0f, 1.0f } });
Set(TEXT("PplRoofS1B"), { { TEXT("/Game/People/CityCenter/SM_RoofParty_B"), 10.67f, 0.0f, 5.48f, 0.0f, 1.0f } });
Set(TEXT("PplRoofMarq"), { { TEXT("/Game/People/CityCenter/SM_RoofParty_A"), -4.0f, 0.0f, 11.82f, 0.0f, 1.0f } });
Set(TEXT("PplRoofGar"), { { TEXT("/Game/People/CityCenter/SM_RoofParty_A"), 0.0f, 1.5f, 12.37f, 0.0f, 1.0f } });
Set(TEXT("PplRoofGarR"), { { TEXT("/Game/People/CityCenter/SM_RoofRally_A"), 0.0f, 1.5f, 12.37f, 0.0f, 1.0f } });
Set(TEXT("PplRoofYard"), { { TEXT("/Game/People/CityCenter/SM_RoofParty_B"), -7.0f, 3.4f, 6.64f, 0.0f, 1.0f } });
Set(TEXT("PplRoofPent"), { { TEXT("/Game/People/CityCenter/SM_RoofRally_A"), -5.05f, 0.5f, 9.5f, 0.0f, 1.0f } });
Set(TEXT("PplSynWatch"), { { TEXT("/Game/People/CityCenter/SM_Syndicate_A"), 9.0f, -2.1f, 9.0f, 0.0f, 1.0f } });
Set(TEXT("PplSynWatch2"), { { TEXT("/Game/People/CityCenter/SM_Syndicate_A"), -9.0f, -2.1f, 9.0f, 0.0f, 1.0f } });
Set(TEXT("PplClean"), { { TEXT("/Game/People/CityCenter/SM_WindowCleaners_A"), 22.6f, -2.2f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplRally"), { { TEXT("/Game/People/CityCenter/SM_Rally_A"), 22.6f, -3.1f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplFlockGap"), { { TEXT("/Game/People/CityCenter/SM_PigeonFlock"), 22.6f, 0.5f, 15.0f, 0.0f, 0.36f } });
Set(TEXT("PplCrewCraneRoof"), { { TEXT("/Game/People/Crew/SM_CrewCrane_City"), 0.0f, 1.0f, 5.48f, 0.0f, 1.0f } });
Set(TEXT("PplCrewLightsRoof"), { { TEXT("/Game/People/Crew/SM_CrewLights_City"), 0.0f, -2.0f, 5.48f, 0.0f, 1.0f } });
Set(TEXT("PplCrewCraneRoofP"), { { TEXT("/Game/People/Crew/SM_CrewCrane_City"), -5.05f, 1.5f, 9.5f, 0.0f, 1.0f } });
Set(TEXT("PplCrewCater"), { { TEXT("/Game/People/Crew/SM_CrewCatering_City"), 22.6f, -3.4f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplCrewSound"), { { TEXT("/Game/People/Crew/SM_CrewSound_City"), 22.6f, -3.6f, 0.0f, 0.0f, 1.0f } });

// ================================================================ THE START: the unit, on both sides of the first 300 m
// (slots 1 to 6. Left alleys are the even slots, right the odd; W_Shops2s roofs: left slots 1 and 4, right 3 and 6)
Zone(TEXT("Crew: the start"), 0.0f, 0.3f);
	Site(TEXT("PplCrewCam"), -1, 17.6f, 6, 4, 1.0f);
	Site(TEXT("PplCrewAppA"), -1, 17.6f, 6, 2, 1.0f);
	Site(TEXT("PplCrewStunt"), -1, 17.6f, 6, 0, 1.0f);
	Site(TEXT("PplCrewVil"), 1, 17.6f, 6, 5, 1.0f);
	Site(TEXT("PplCrewAppB"), 1, 17.6f, 6, 3, 1.0f);
	Site(TEXT("PplCrewCam"), 1, 17.6f, 6, 1, 1.0f);
	Site(TEXT("PplCrewLightsRoof"), -1, 29.0f, 6, 5, 1.0f);
	Site(TEXT("PplCrewCraneRoof"), -1, 29.0f, 6, 2, 1.0f);
	Site(TEXT("PplCrewCraneRoof"), 1, 29.0f, 6, 3, 1.0f);
	Site(TEXT("PplCrewLightsRoof"), 1, 29.0f, 6, 0, 1.0f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 2, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 2, 1, 1.0f, 1.f, 0.f, 1, 0.f);

// ================================================================ THE SIX DISTRICTS (FTDressL7.inl's own), each cut round the crew's windows:
// the two slots before every corkscrew, jump and loop belong to the crew, and so do the last four before the Chairman.
Zone(TEXT("People: Fifth and Main"), 0.3f, 1.1f);
// pockets before the alleys (left: even slots; right: odd). Roofs: W_Shops2s left (3,2) right (3,0); W_Shops1 right roof, right (3,1)
	Site(TEXT("PplOnA"), -1, 17.6f, 8, 0, 1.0f);
	Site(TEXT("PplVend"), -1, 17.6f, 8, 6, 1.0f);
	Site(TEXT("PplPig"), -1, 17.6f, 8, 4, 1.0f);
	Site(TEXT("PplOnB"), -1, 17.6f, 8, 2, 1.0f);
	Site(TEXT("PplVend"), 1, 17.6f, 8, 7, 1.0f);
	Site(TEXT("PplOnB"), 1, 17.6f, 8, 5, 1.0f);
	Site(TEXT("PplMarA"), 1, 17.6f, 8, 3, 1.0f);
	Site(TEXT("PplOnA"), 1, 17.6f, 8, 1, 1.0f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 2, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 2, 1, 1.0f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplRoofMidA"), -1, 29.0f, 6, 2, 1.0f);
	Site(TEXT("PplRoofMidR"), -1, 29.0f, 6, 5, 1.0f);
	Site(TEXT("PplRoofMidB"), 1, 29.0f, 6, 0, 1.0f);
	Site(TEXT("PplRoofS1"), 1, 29.0f, 6, 4, 1.0f);
	Site(TEXT("PplFlockGap"), -1, 29.0f, 4, 1, 1.0f);
	Site(TEXT("PplFlockGap"), 1, 29.0f, 4, 0, 1.0f);
	Far(TEXT("/Game/People/CityCenter/SM_PigeonFlock"), 110.0f, 190.0f, 10, 2, 0.85f, 2.2f, 0.2f, 1, -55.0f, 180.0f);
Zone(TEXT("People: Theatre Row (1)"), 1.1f, 1.34f);
// the queue at every picture house (W_Marquee left (2,0), right (2,1)); pockets: left every gap but s%3==2, right but s%3==1;
// a party on every other picture house, the cradle on every other glass infill (G_Fill left (3,2), right (3,1))
	Site(TEXT("PplQueue"), -1, 29.0f, 2, 0, 1.0f);
	Site(TEXT("PplQueue"), 1, 29.0f, 2, 1, 1.0f);
	Site(TEXT("PplOnA"), -1, 17.6f, 6, 0, 1.0f);
	Site(TEXT("PplMarB"), -1, 17.6f, 6, 3, 1.0f);
	Site(TEXT("PplVend"), -1, 17.6f, 6, 2, 1.0f);
	Site(TEXT("PplOnB"), -1, 17.6f, 6, 5, 1.0f);
	Site(TEXT("PplMarA"), 1, 17.6f, 6, 0, 1.0f);
	Site(TEXT("PplOnB"), 1, 17.6f, 6, 3, 1.0f);
	Site(TEXT("PplOnA"), 1, 17.6f, 6, 1, 1.0f);
	Site(TEXT("PplPig"), 1, 17.6f, 6, 4, 1.0f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 2, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 2, 1, 1.0f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplRoofMarq"), -1, 29.0f, 4, 0, 1.0f);
	Site(TEXT("PplRoofMarq"), 1, 29.0f, 4, 1, 1.0f);
	Site(TEXT("PplRoofMidR"), -1, 29.0f, 4, 3, 1.0f);
	Site(TEXT("PplRoofS1B"), 1, 29.0f, 4, 0, 1.0f);
	Site(TEXT("PplClean"), -1, 29.0f, 6, 2, 1.0f);
	Site(TEXT("PplClean"), 1, 29.0f, 6, 1, 1.0f);
	Site(TEXT("PplFlockGap"), -1, 29.0f, 6, 1, 1.0f);
	Site(TEXT("PplFlockGap"), 1, 29.0f, 6, 2, 1.0f);
	Far(TEXT("/Game/People/CityCenter/SM_PigeonFlock"), 110.0f, 190.0f, 10, 0, 0.85f, 2.2f, 0.2f, 1, -55.0f, 180.0f);
Zone(TEXT("Crew: Theatre Row 1.34 km"), 1.34f, 1.443f);
	Site(TEXT("PplCrewCam"), -1, 17.6f, 3, 0, 1.0f);
	Site(TEXT("PplCrewAppA"), -1, 17.6f, 3, 2, 1.0f);
	Site(TEXT("PplCrewStunt"), 1, 17.6f, 3, 0, 1.0f);
	Site(TEXT("PplCrewAppB"), 1, 17.6f, 3, 1, 1.0f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 3, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 3, 1, 1.0f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People: Theatre Row (2)"), 1.443f, 2.1f);
// the queue at every picture house (W_Marquee left (2,0), right (2,1)); pockets: left every gap but s%3==2, right but s%3==1;
// a party on every other picture house, the cradle on every other glass infill (G_Fill left (3,2), right (3,1))
	Site(TEXT("PplQueue"), -1, 29.0f, 2, 0, 1.0f);
	Site(TEXT("PplQueue"), 1, 29.0f, 2, 1, 1.0f);
	Site(TEXT("PplOnA"), -1, 17.6f, 6, 0, 1.0f);
	Site(TEXT("PplMarB"), -1, 17.6f, 6, 3, 1.0f);
	Site(TEXT("PplVend"), -1, 17.6f, 6, 2, 1.0f);
	Site(TEXT("PplOnB"), -1, 17.6f, 6, 5, 1.0f);
	Site(TEXT("PplMarA"), 1, 17.6f, 6, 0, 1.0f);
	Site(TEXT("PplOnB"), 1, 17.6f, 6, 3, 1.0f);
	Site(TEXT("PplOnA"), 1, 17.6f, 6, 1, 1.0f);
	Site(TEXT("PplPig"), 1, 17.6f, 6, 4, 1.0f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 2, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 2, 1, 1.0f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplRoofMarq"), -1, 29.0f, 4, 0, 1.0f);
	Site(TEXT("PplRoofMarq"), 1, 29.0f, 4, 1, 1.0f);
	Site(TEXT("PplRoofMidR"), -1, 29.0f, 4, 3, 1.0f);
	Site(TEXT("PplRoofS1B"), 1, 29.0f, 4, 0, 1.0f);
	Site(TEXT("PplClean"), -1, 29.0f, 6, 2, 1.0f);
	Site(TEXT("PplClean"), 1, 29.0f, 6, 1, 1.0f);
	Site(TEXT("PplFlockGap"), -1, 29.0f, 6, 1, 1.0f);
	Site(TEXT("PplFlockGap"), 1, 29.0f, 6, 2, 1.0f);
	Far(TEXT("/Game/People/CityCenter/SM_PigeonFlock"), 110.0f, 190.0f, 10, 0, 0.85f, 2.2f, 0.2f, 1, -55.0f, 180.0f);
Zone(TEXT("People: The Garages (1)"), 2.1f, 2.56f);
// every gap an alley: a pocket in every other slot; parties on the garages' top decks (W_Garage left (2,0), right (2,1)) and
// the yards' roofs (W_Yards left (2,1), right (2,0))
	Site(TEXT("PplMarB"), -1, 17.6f, 4, 0, 1.0f);
	Site(TEXT("PplPig"), -1, 17.6f, 4, 2, 1.0f);
	Site(TEXT("PplOnA"), 1, 17.6f, 4, 1, 1.0f);
	Site(TEXT("PplVend"), 1, 17.6f, 4, 3, 1.0f);
	Site(TEXT("PplOnB"), -1, 17.6f, 4, 1, 0.6f);
	Site(TEXT("PplOnB"), 1, 17.6f, 4, 2, 0.6f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 3, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 3, 1, 1.0f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplRoofGar"), -1, 29.0f, 4, 0, 1.0f);
	Site(TEXT("PplRoofGarR"), 1, 29.0f, 4, 1, 1.0f);
	Site(TEXT("PplRoofYard"), -1, 29.0f, 4, 3, 1.0f);
	Site(TEXT("PplRoofYard"), 1, 29.0f, 4, 2, 1.0f);
	Site(TEXT("PplFlockGap"), -1, 29.0f, 6, 1, 1.0f);
	Site(TEXT("PplFlockGap"), 1, 29.0f, 6, 4, 1.0f);
	Far(TEXT("/Game/People/CityCenter/SM_PigeonFlock"), 110.0f, 190.0f, 8, 1, 0.85f, 2.2f, 0.2f, -1, -55.0f, 180.0f);
Zone(TEXT("Crew: The Garages 2.56 km"), 2.56f, 2.665f);
	Site(TEXT("PplCrewCam"), -1, 17.6f, 2, 0, 1.0f);
	Site(TEXT("PplCrewAppA"), -1, 17.6f, 2, 1, 1.0f);
	Site(TEXT("PplCrewStunt"), 1, 17.6f, 2, 0, 1.0f);
	Site(TEXT("PplCrewAppB"), 1, 17.6f, 2, 1, 1.0f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 3, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 3, 1, 1.0f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People: The Garages (2)"), 2.665f, 3.04f);
// every gap an alley: a pocket in every other slot; parties on the garages' top decks (W_Garage left (2,0), right (2,1)) and
// the yards' roofs (W_Yards left (2,1), right (2,0))
	Site(TEXT("PplMarB"), -1, 17.6f, 4, 0, 1.0f);
	Site(TEXT("PplPig"), -1, 17.6f, 4, 2, 1.0f);
	Site(TEXT("PplOnA"), 1, 17.6f, 4, 1, 1.0f);
	Site(TEXT("PplVend"), 1, 17.6f, 4, 3, 1.0f);
	Site(TEXT("PplOnB"), -1, 17.6f, 4, 1, 0.6f);
	Site(TEXT("PplOnB"), 1, 17.6f, 4, 2, 0.6f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 3, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 3, 1, 1.0f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplRoofGar"), -1, 29.0f, 4, 0, 1.0f);
	Site(TEXT("PplRoofGarR"), 1, 29.0f, 4, 1, 1.0f);
	Site(TEXT("PplRoofYard"), -1, 29.0f, 4, 3, 1.0f);
	Site(TEXT("PplRoofYard"), 1, 29.0f, 4, 2, 1.0f);
	Site(TEXT("PplFlockGap"), -1, 29.0f, 6, 1, 1.0f);
	Site(TEXT("PplFlockGap"), 1, 29.0f, 6, 4, 1.0f);
	Far(TEXT("/Game/People/CityCenter/SM_PigeonFlock"), 110.0f, 190.0f, 8, 1, 0.85f, 2.2f, 0.2f, -1, -55.0f, 180.0f);
Zone(TEXT("Crew: The Garages 3.04 km"), 3.04f, 3.13f);
	Site(TEXT("PplCrewCam"), -1, 17.6f, 2, 0, 1.0f);
	Site(TEXT("PplCrewAppA"), -1, 17.6f, 2, 1, 1.0f);
	Site(TEXT("PplCrewStunt"), 1, 17.6f, 2, 0, 1.0f);
	Site(TEXT("PplCrewAppB"), 1, 17.6f, 2, 1, 1.0f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 3, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 3, 1, 1.0f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("Crew: The Plaza 3.13 km"), 3.13f, 3.208f);
	Site(TEXT("PplCrewCam"), -1, 17.6f, 2, 1, 1.0f);
	Site(TEXT("PplCrewCater"), -1, 29.0f, 2, 1, 1.0f);
	Site(TEXT("PplCrewAppB"), 1, 17.6f, 2, 0, 1.0f);
	Site(TEXT("PplCrewSound"), 1, 29.0f, 2, 0, 1.0f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 3, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 3, 1, 1.0f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People: The Plaza (1)"), 3.208f, 4.53f);
// the monuments (G_Monument left (2,1), right (2,0)): a rally before every other one, the march coming to it in the pocket;
// roofs: W_Shops1 left (4,0), W_Pent left (4,2), W_Shops2s right (4,1), W_Pent right (4,3)
	Site(TEXT("PplRally"), -1, 29.0f, 4, 1, 1.0f);
	Site(TEXT("PplMarA"), -1, 17.6f, 4, 1, 1.0f);
	Site(TEXT("PplPig"), -1, 17.6f, 4, 3, 1.0f);
	Site(TEXT("PplRally"), 1, 29.0f, 4, 0, 1.0f);
	Site(TEXT("PplMarB"), 1, 17.6f, 4, 0, 1.0f);
	Site(TEXT("PplVend"), 1, 17.6f, 8, 2, 1.0f);
	Site(TEXT("PplOnB"), 1, 17.6f, 8, 6, 1.0f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 2, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 2, 1, 1.0f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplRoofS1"), -1, 29.0f, 8, 0, 1.0f);
	Site(TEXT("PplRoofS1B"), -1, 29.0f, 8, 4, 1.0f);
	Site(TEXT("PplRoofPent"), -1, 29.0f, 4, 2, 1.0f);
	Site(TEXT("PplRoofMidA"), 1, 29.0f, 8, 1, 1.0f);
	Site(TEXT("PplRoofMidB"), 1, 29.0f, 8, 5, 1.0f);
	Site(TEXT("PplRoofPent"), 1, 29.0f, 4, 3, 1.0f);
	Site(TEXT("PplFlockGap"), -1, 29.0f, 4, 0, 1.0f);
	Site(TEXT("PplFlockGap"), 1, 29.0f, 4, 1, 1.0f);
	Far(TEXT("/Game/People/CityCenter/SM_PigeonFlock"), 110.0f, 190.0f, 8, 3, 0.85f, 2.2f, 0.2f, -1, -55.0f, 180.0f);
Zone(TEXT("Crew: The Plaza 4.53 km"), 4.53f, 4.62f);
	Site(TEXT("PplCrewCam"), -1, 17.6f, 2, 1, 1.0f);
	Site(TEXT("PplCrewCater"), -1, 29.0f, 2, 1, 1.0f);
	Site(TEXT("PplCrewAppB"), 1, 17.6f, 2, 0, 1.0f);
	Site(TEXT("PplCrewSound"), 1, 29.0f, 2, 0, 1.0f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 3, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 3, 1, 1.0f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("Crew: Wall Street 4.62 km"), 4.62f, 4.701f);
	Site(TEXT("PplCrewCam"), -1, 17.6f, 4, 0, 1.0f);
	Site(TEXT("PplCrewAppA"), -1, 17.6f, 4, 3, 1.0f);
	Site(TEXT("PplCrewVil"), -1, 17.6f, 4, 2, 1.0f);
	Site(TEXT("PplCrewAppB"), 1, 17.6f, 4, 0, 1.0f);
	Site(TEXT("PplCrewStunt"), 1, 17.6f, 4, 1, 1.0f);
	Site(TEXT("PplCrewCam"), 1, 17.6f, 4, 2, 1.0f);
	Site(TEXT("PplCrewCraneRoofP"), -1, 29.0f, 4, 1, 1.0f);
	Site(TEXT("PplCrewCraneRoofP"), 1, 29.0f, 4, 2, 1.0f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 3, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 3, 1, 1.0f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People: Wall Street (1)"), 4.701f, 5.59f);
// the money district: more commuters. Pockets: left s%4 = 0 (alley), 1 and 2 (closed gaps); right 0, 3 (closed), 2 (alley).
// The cradle on the closed gaps (G_Fill left (4,2), right (4,0)); roofs: W_Pent left (4,1) right (4,2), W_Shops2s left (4,3), W_Yards right (4,0)
	Site(TEXT("PplOnB"), -1, 17.6f, 4, 0, 1.0f);
	Site(TEXT("PplVend"), -1, 17.6f, 4, 3, 1.0f);
	Site(TEXT("PplMarB"), -1, 17.6f, 8, 2, 1.0f);
	Site(TEXT("PplPig"), -1, 17.6f, 8, 6, 1.0f);
	Site(TEXT("PplOnA"), 1, 17.6f, 4, 0, 1.0f);
	Site(TEXT("PplMarA"), 1, 17.6f, 8, 1, 1.0f);
	Site(TEXT("PplVend"), 1, 17.6f, 8, 5, 1.0f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 2, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 2, 1, 1.0f, 1.f, 0.f, 1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 4, 3, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 4, 2, 1.0f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplClean"), -1, 29.0f, 4, 2, 1.0f);
	Site(TEXT("PplClean"), 1, 29.0f, 4, 0, 1.0f);
	Site(TEXT("PplRoofPent"), -1, 29.0f, 4, 1, 1.0f);
	Site(TEXT("PplRoofPent"), 1, 29.0f, 4, 2, 1.0f);
	Site(TEXT("PplRoofMidA"), -1, 29.0f, 4, 3, 1.0f);
	Site(TEXT("PplRoofYard"), 1, 29.0f, 4, 0, 1.0f);
	Site(TEXT("PplFlockGap"), -1, 29.0f, 4, 1, 1.0f);
	Site(TEXT("PplFlockGap"), 1, 29.0f, 4, 3, 1.0f);
	Far(TEXT("/Game/People/CityCenter/SM_PigeonFlock"), 110.0f, 190.0f, 10, 4, 0.85f, 2.2f, 0.2f, 1, -55.0f, 180.0f);
Zone(TEXT("Crew: Wall Street 5.59 km"), 5.59f, 5.696f);
	Site(TEXT("PplCrewCam"), -1, 17.6f, 4, 0, 1.0f);
	Site(TEXT("PplCrewAppA"), -1, 17.6f, 4, 3, 1.0f);
	Site(TEXT("PplCrewVil"), -1, 17.6f, 4, 2, 1.0f);
	Site(TEXT("PplCrewAppB"), 1, 17.6f, 4, 0, 1.0f);
	Site(TEXT("PplCrewStunt"), 1, 17.6f, 4, 1, 1.0f);
	Site(TEXT("PplCrewCam"), 1, 17.6f, 4, 2, 1.0f);
	Site(TEXT("PplCrewCraneRoofP"), -1, 29.0f, 4, 1, 1.0f);
	Site(TEXT("PplCrewCraneRoofP"), 1, 29.0f, 4, 2, 1.0f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 3, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 3, 1, 1.0f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People: Wall Street (2)"), 5.696f, 6.29f);
// the money district: more commuters. Pockets: left s%4 = 0 (alley), 1 and 2 (closed gaps); right 0, 3 (closed), 2 (alley).
// The cradle on the closed gaps (G_Fill left (4,2), right (4,0)); roofs: W_Pent left (4,1) right (4,2), W_Shops2s left (4,3), W_Yards right (4,0)
	Site(TEXT("PplOnB"), -1, 17.6f, 4, 0, 1.0f);
	Site(TEXT("PplVend"), -1, 17.6f, 4, 3, 1.0f);
	Site(TEXT("PplMarB"), -1, 17.6f, 8, 2, 1.0f);
	Site(TEXT("PplPig"), -1, 17.6f, 8, 6, 1.0f);
	Site(TEXT("PplOnA"), 1, 17.6f, 4, 0, 1.0f);
	Site(TEXT("PplMarA"), 1, 17.6f, 8, 1, 1.0f);
	Site(TEXT("PplVend"), 1, 17.6f, 8, 5, 1.0f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 2, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 2, 1, 1.0f, 1.f, 0.f, 1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 4, 3, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 4, 2, 1.0f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplClean"), -1, 29.0f, 4, 2, 1.0f);
	Site(TEXT("PplClean"), 1, 29.0f, 4, 0, 1.0f);
	Site(TEXT("PplRoofPent"), -1, 29.0f, 4, 1, 1.0f);
	Site(TEXT("PplRoofPent"), 1, 29.0f, 4, 2, 1.0f);
	Site(TEXT("PplRoofMidA"), -1, 29.0f, 4, 3, 1.0f);
	Site(TEXT("PplRoofYard"), 1, 29.0f, 4, 0, 1.0f);
	Site(TEXT("PplFlockGap"), -1, 29.0f, 4, 1, 1.0f);
	Site(TEXT("PplFlockGap"), 1, 29.0f, 4, 3, 1.0f);
	Far(TEXT("/Game/People/CityCenter/SM_PigeonFlock"), 110.0f, 190.0f, 10, 4, 0.85f, 2.2f, 0.2f, 1, -55.0f, 180.0f);
Zone(TEXT("People: Penthouse Row (1)"), 6.29f, 6.63f);
// the Syndicate shows itself: its men in the pockets before its banners (G_Banner: left s%6 = 0, right 3). A rally before
// each monument (left s%6 = 2, right 5), the cradle on the closed gaps (left 3, right 0), the queue at the picture house
// (W_Marquee right (3,0)); rallies on the dear doors' roof terraces (W_Pent left (3,0),(3,1), right (3,1),(3,2))
	Site(TEXT("PplSyn"), -1, 17.6f, 6, 0, 1.0f);
	Site(TEXT("PplOnA"), -1, 17.6f, 6, 5, 1.0f);
	Site(TEXT("PplMarA"), -1, 17.6f, 6, 4, 1.0f);
	Site(TEXT("PplRally"), -1, 29.0f, 6, 4, 1.0f);
	Site(TEXT("PplOnB"), -1, 17.6f, 6, 3, 1.0f);
	Site(TEXT("PplClean"), -1, 29.0f, 6, 3, 1.0f);
	Site(TEXT("PplVend"), -1, 17.6f, 6, 1, 1.0f);
	Site(TEXT("PplSyn"), 1, 17.6f, 6, 3, 1.0f);
	Site(TEXT("PplOnB"), 1, 17.6f, 6, 2, 1.0f);
	Site(TEXT("PplMarB"), 1, 17.6f, 6, 1, 1.0f);
	Site(TEXT("PplRally"), 1, 29.0f, 6, 1, 1.0f);
	Site(TEXT("PplOnA"), 1, 17.6f, 6, 0, 1.0f);
	Site(TEXT("PplClean"), 1, 29.0f, 6, 0, 1.0f);
	Site(TEXT("PplPig"), 1, 17.6f, 6, 4, 1.0f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 3, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 3, 1, 1.0f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplRoofPent"), -1, 29.0f, 6, 0, 1.0f);
	Site(TEXT("PplRoofPent"), -1, 29.0f, 6, 4, 1.0f);
	Site(TEXT("PplRoofPent"), 1, 29.0f, 6, 1, 1.0f);
	Site(TEXT("PplRoofPent"), 1, 29.0f, 6, 2, 1.0f);
	Site(TEXT("PplQueue"), 1, 29.0f, 6, 0, 1.0f);
	Site(TEXT("PplRoofMarq"), 1, 29.0f, 6, 3, 1.0f);
	Far(TEXT("/Game/People/CityCenter/SM_PigeonFlock"), 110.0f, 190.0f, 10, 1, 0.85f, 2.2f, 0.2f, -1, -55.0f, 180.0f);
Zone(TEXT("Crew: Penthouse Row 6.63 km"), 6.63f, 6.737f);
	Site(TEXT("PplCrewCam"), -1, 17.6f, 6, 0, 1.0f);
	Site(TEXT("PplCrewAppA"), -1, 17.6f, 6, 5, 1.0f);
	Site(TEXT("PplCrewVil"), -1, 17.6f, 6, 4, 1.0f);
	Site(TEXT("PplCrewStunt"), -1, 17.6f, 6, 3, 1.0f);
	Site(TEXT("PplCrewAppB"), -1, 17.6f, 6, 1, 1.0f);
	Site(TEXT("PplCrewAppA"), 1, 17.6f, 6, 3, 1.0f);
	Site(TEXT("PplCrewCam"), 1, 17.6f, 6, 2, 1.0f);
	Site(TEXT("PplCrewStunt"), 1, 17.6f, 6, 1, 1.0f);
	Site(TEXT("PplCrewAppB"), 1, 17.6f, 6, 0, 1.0f);
	Site(TEXT("PplCrewVil"), 1, 17.6f, 6, 4, 1.0f);
	Site(TEXT("PplCrewCraneRoofP"), -1, 29.0f, 3, 0, 1.0f);
	Site(TEXT("PplCrewCraneRoofP"), 1, 29.0f, 3, 1, 1.0f);
	Site(TEXT("PplCrewCater"), -1, 29.0f, 6, 4, 1.0f);
	Site(TEXT("PplCrewSound"), 1, 29.0f, 6, 1, 1.0f);
Zone(TEXT("People: Penthouse Row (2)"), 6.737f, 7.48f);
// the Syndicate shows itself: its men in the pockets before its banners (G_Banner: left s%6 = 0, right 3). A rally before
// each monument (left s%6 = 2, right 5), the cradle on the closed gaps (left 3, right 0), the queue at the picture house
// (W_Marquee right (3,0)); rallies on the dear doors' roof terraces (W_Pent left (3,0),(3,1), right (3,1),(3,2))
	Site(TEXT("PplSyn"), -1, 17.6f, 6, 0, 1.0f);
	Site(TEXT("PplOnA"), -1, 17.6f, 6, 5, 1.0f);
	Site(TEXT("PplMarA"), -1, 17.6f, 6, 4, 1.0f);
	Site(TEXT("PplRally"), -1, 29.0f, 6, 4, 1.0f);
	Site(TEXT("PplOnB"), -1, 17.6f, 6, 3, 1.0f);
	Site(TEXT("PplClean"), -1, 29.0f, 6, 3, 1.0f);
	Site(TEXT("PplVend"), -1, 17.6f, 6, 1, 1.0f);
	Site(TEXT("PplSyn"), 1, 17.6f, 6, 3, 1.0f);
	Site(TEXT("PplOnB"), 1, 17.6f, 6, 2, 1.0f);
	Site(TEXT("PplMarB"), 1, 17.6f, 6, 1, 1.0f);
	Site(TEXT("PplRally"), 1, 29.0f, 6, 1, 1.0f);
	Site(TEXT("PplOnA"), 1, 17.6f, 6, 0, 1.0f);
	Site(TEXT("PplClean"), 1, 29.0f, 6, 0, 1.0f);
	Site(TEXT("PplPig"), 1, 17.6f, 6, 4, 1.0f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_A"), 23.72f, 23.72f, 3, 0, 1.0f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/CityCenter/SM_Commuters_B"), 23.72f, 23.72f, 3, 1, 1.0f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplRoofPent"), -1, 29.0f, 6, 0, 1.0f);
	Site(TEXT("PplRoofPent"), -1, 29.0f, 6, 4, 1.0f);
	Site(TEXT("PplRoofPent"), 1, 29.0f, 6, 1, 1.0f);
	Site(TEXT("PplRoofPent"), 1, 29.0f, 6, 2, 1.0f);
	Site(TEXT("PplQueue"), 1, 29.0f, 6, 0, 1.0f);
	Site(TEXT("PplRoofMarq"), 1, 29.0f, 6, 3, 1.0f);
	Far(TEXT("/Game/People/CityCenter/SM_PigeonFlock"), 110.0f, 190.0f, 10, 1, 0.85f, 2.2f, 0.2f, -1, -55.0f, 180.0f);
Zone(TEXT("Crew: Penthouse Row 7.48 km"), 7.48f, 7.66f);
	Site(TEXT("PplCrewCam"), -1, 17.6f, 6, 0, 1.0f);
	Site(TEXT("PplCrewAppA"), -1, 17.6f, 6, 5, 1.0f);
	Site(TEXT("PplCrewVil"), -1, 17.6f, 6, 4, 1.0f);
	Site(TEXT("PplCrewStunt"), -1, 17.6f, 6, 3, 1.0f);
	Site(TEXT("PplCrewAppB"), -1, 17.6f, 6, 1, 1.0f);
	Site(TEXT("PplCrewAppA"), 1, 17.6f, 6, 3, 1.0f);
	Site(TEXT("PplCrewCam"), 1, 17.6f, 6, 2, 1.0f);
	Site(TEXT("PplCrewStunt"), 1, 17.6f, 6, 1, 1.0f);
	Site(TEXT("PplCrewAppB"), 1, 17.6f, 6, 0, 1.0f);
	Site(TEXT("PplCrewVil"), 1, 17.6f, 6, 4, 1.0f);
	Site(TEXT("PplCrewCraneRoofP"), -1, 29.0f, 3, 0, 1.0f);
	Site(TEXT("PplCrewCraneRoofP"), 1, 29.0f, 3, 1, 1.0f);
	Site(TEXT("PplCrewCater"), -1, 29.0f, 6, 4, 1.0f);
	Site(TEXT("PplCrewSound"), 1, 29.0f, 6, 1, 1.0f);

// ================================================================ INSIDE THE SET PIECES (near rules are left out there by the track: Far rules)
Zone(TEXT("Crew: Theatre Row corkscrew"), 1.443f, 1.588f);
	Far(TEXT("/Game/People/Crew/SM_CrewApplauseA_City"), 21.5f, 22.5f, 2, 0, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewApplauseB_City"), 21.5f, 22.5f, 2, 1, 1.0f, 1.0f, 0.0f, 1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewCamera_City"), 21.5f, 22.5f, 2, 1, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewDolly_City"), 21.5f, 22.5f, 2, 0, 1.0f, 1.0f, 0.0f, 1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/CityCenter/SM_Onlookers_A"), 24.0f, 25.0f, 2, 0, 1.0f, 1.0f, 0.0f, 1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/CityCenter/SM_Onlookers_B"), 24.5f, 25.5f, 2, 1, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
Zone(TEXT("Crew: Garage jump"), 2.665f, 2.81f);
	Far(TEXT("/Game/People/Crew/SM_CrewCrane_City"), 44.0f, 47.0f, 2, 1, 1.0f, 1.0f, 0.0f, -1, 0.62f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewLights_City"), 44.0f, 47.0f, 2, 0, 1.0f, 1.0f, 0.0f, 1, 0.62f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewStunt_City"), 42.0f, 44.0f, 2, 0, 1.0f, 1.0f, 0.0f, -1, 0.62f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewApplauseA_City"), 42.0f, 44.0f, 2, 1, 1.0f, 1.0f, 0.0f, 1, 0.62f, 0.0f);
Zone(TEXT("Crew: Plaza loop"), 3.208f, 3.488f);
	Far(TEXT("/Game/People/Crew/SM_CrewApplauseA_City"), 26.0f, 28.0f, 2, 0, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewDolly_City"), 30.0f, 31.0f, 2, 1, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewStunt_City"), 26.0f, 28.0f, 3, 0, 1.0f, 1.0f, 0.0f, 1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewCrane_City"), 46.0f, 50.0f, 3, 1, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewLights_City"), 46.0f, 50.0f, 3, 2, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/CityCenter/SM_Onlookers_A"), 34.0f, 36.0f, 2, 1, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/CityCenter/SM_Rally_A"), 38.0f, 41.0f, 3, 0, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
Zone(TEXT("Crew: Wall Street corkscrew"), 4.701f, 4.846f);
	Far(TEXT("/Game/People/Crew/SM_CrewApplauseA_City"), 21.5f, 22.5f, 2, 0, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewApplauseB_City"), 21.5f, 22.5f, 2, 1, 1.0f, 1.0f, 0.0f, 1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewCamera_City"), 21.5f, 22.5f, 2, 1, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewDolly_City"), 21.5f, 22.5f, 2, 0, 1.0f, 1.0f, 0.0f, 1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/CityCenter/SM_Onlookers_A"), 24.0f, 25.0f, 2, 0, 1.0f, 1.0f, 0.0f, 1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/CityCenter/SM_Onlookers_B"), 24.5f, 25.5f, 2, 1, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
Zone(TEXT("Crew: Wall Street jump"), 5.696f, 5.841f);
	Far(TEXT("/Game/People/Crew/SM_CrewCrane_City"), 44.0f, 47.0f, 2, 1, 1.0f, 1.0f, 0.0f, -1, 0.62f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewLights_City"), 44.0f, 47.0f, 2, 0, 1.0f, 1.0f, 0.0f, 1, 0.62f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewStunt_City"), 42.0f, 44.0f, 2, 0, 1.0f, 1.0f, 0.0f, -1, 0.62f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewApplauseA_City"), 42.0f, 44.0f, 2, 1, 1.0f, 1.0f, 0.0f, 1, 0.62f, 0.0f);
Zone(TEXT("Crew: Penthouse loop"), 6.737f, 6.972f);
	Far(TEXT("/Game/People/Crew/SM_CrewApplauseA_City"), 26.0f, 28.0f, 2, 0, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewDolly_City"), 30.0f, 31.0f, 2, 1, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewStunt_City"), 26.0f, 28.0f, 3, 0, 1.0f, 1.0f, 0.0f, 1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewCrane_City"), 46.0f, 50.0f, 3, 1, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/Crew/SM_CrewLights_City"), 46.0f, 50.0f, 3, 2, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/CityCenter/SM_Onlookers_A"), 34.0f, 36.0f, 2, 1, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
	Far(TEXT("/Game/People/CityCenter/SM_Rally_A"), 38.0f, 41.0f, 3, 0, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);

// ================================================================ SYNDICATE TERRITORY, the finale (7.66 km to the end: the arena, then the circuit).
// The Chairman's men in every fourth pocket and on every fourth black wall (W_Syn left (2,0), right (2,1)); the city on the roofs
// it still holds: rallies on the dear doors (W_Pent left (4,1), right (4,2)), parties on the garage (left (8,7)), the shops (right (8,4))
// and the yard (right (8,0)); onlookers and the crew's applause behind the barrier rows; flocks over the far roofs. Nothing lit here
// blinks or turns. Thinner on the circuit (13.56 km on), which the boss fight seldom reaches.
Zone(TEXT("People: Syndicate Territory, the arena"), 7.66f, 13.56f);
	Site(TEXT("PplSyn"), -1, 17.6f, 4, 0, 0.9f);
	Site(TEXT("PplSyn"), 1, 17.6f, 4, 2, 0.9f);
	Site(TEXT("PplOnAs"), -1, 17.6f, 4, 1, 0.72f);
	Site(TEXT("PplOnBs"), 1, 17.6f, 4, 3, 0.72f);
	Site(TEXT("PplCrewAppAs"), -1, 17.6f, 16, 5, 0.72f);
	Site(TEXT("PplCrewAppAs"), 1, 17.6f, 16, 13, 0.72f);
	Site(TEXT("PplSynWatch"), -1, 29.0f, 8, 0, 0.9f);
	Site(TEXT("PplSynWatch2"), 1, 29.0f, 8, 3, 0.9f);
	Site(TEXT("PplRoofPent"), -1, 29.0f, 8, 1, 0.9f);
	Site(TEXT("PplRoofPent"), 1, 29.0f, 8, 2, 0.9f);
	Site(TEXT("PplRoofGar"), -1, 29.0f, 8, 7, 0.9f);
	Site(TEXT("PplRoofMidA"), 1, 29.0f, 8, 4, 0.9f);
	Site(TEXT("PplRoofYard"), 1, 29.0f, 8, 0, 0.9f);
	Far(TEXT("/Game/People/CityCenter/SM_PigeonFlock"), 110.0f, 190.0f, 9, 3, 0.9f, 2.2f, 0.2f, -1, -55.0f, 180.0f);
	Far(TEXT("/Game/People/CityCenter/SM_PigeonFlock"), 110.0f, 190.0f, 9, 7, 0.9f, 2.2f, 0.2f, 1, -55.0f, 180.0f);
Zone(TEXT("People: Syndicate Territory, the circuit"), 13.56f, 99.0f);
	Site(TEXT("PplSyn"), -1, 17.6f, 4, 0, 0.25f);
	Site(TEXT("PplSyn"), 1, 17.6f, 4, 2, 0.25f);
	Site(TEXT("PplOnAs"), -1, 17.6f, 4, 1, 0.2f);
	Site(TEXT("PplOnBs"), 1, 17.6f, 4, 3, 0.2f);
	Site(TEXT("PplCrewAppAs"), -1, 17.6f, 16, 5, 0.2f);
	Site(TEXT("PplCrewAppAs"), 1, 17.6f, 16, 13, 0.2f);
	Site(TEXT("PplSynWatch"), -1, 29.0f, 8, 0, 0.25f);
	Site(TEXT("PplSynWatch2"), 1, 29.0f, 8, 3, 0.25f);
	Site(TEXT("PplRoofPent"), -1, 29.0f, 8, 1, 0.25f);
	Site(TEXT("PplRoofPent"), 1, 29.0f, 8, 2, 0.25f);
	Site(TEXT("PplRoofGar"), -1, 29.0f, 8, 7, 0.25f);
	Site(TEXT("PplRoofMidA"), 1, 29.0f, 8, 4, 0.25f);
	Site(TEXT("PplRoofYard"), 1, 29.0f, 8, 0, 0.25f);
	Far(TEXT("/Game/People/CityCenter/SM_PigeonFlock"), 110.0f, 190.0f, 9, 3, 0.25f, 2.2f, 0.2f, -1, -55.0f, 180.0f);
	Far(TEXT("/Game/People/CityCenter/SM_PigeonFlock"), 110.0f, 190.0f, 9, 7, 0.25f, 2.2f, 0.2f, 1, -55.0f, 180.0f);

{
// ---- BEGIN far (ft_people_city_far.py) ----
// FAR LIFE AND CREATURES (the extra worker, focus "far"). Six meshes, /Game/People/CityCenter/SM_Far*, all saved with
// the always-moving master. What the test drives showed, and so why the numbers are what they are:
//   - the avenue is a canyon: a 46 to 82 m tower stands 36 to 46 m out in EVERY slot on both sides, 70 to 98 m ones
//     behind them. From the street nothing living beyond the first row is seen at any size. It IS seen wherever the
//     road climbs (the banked walls, the roofs, the corkscrews, the jumps, the loops: the road goes up to 72 m), and
//     this road climbs in every scene. So far life here stands on stands of its own (a dark shaft with lit windows,
//     brought by the piece), 100 m and more out, at 2 to 2.4 times its built size: a crown 110 to 130 m up, dancers
//     10 m tall. Far rules, so they stay through the loops, corkscrews and jumps, where they read best;
//   - 100 m and more also keeps every lit, tall thing of ours outside the loops' own rule (66 m from the loop's ground
//     line; the loop comes down 33 m to the right of where it went up);
//   - the sky over the first row of towers is the other place that is seen: a skein of geese (drop -105: they fly 105
//     to 111 m up, 33 m over the top of the highest loop) and a flight of hang-gliders (95 to 107 m up). Both fly the
//     road's own line where they stand, 130 to 150 m each way, and come round again. The gliders are kept OUT of the
//     loops, corkscrews and jumps and 200 m either side of them (on the test drive a glider hung close beside the
//     top of the plaza loop, 18 m of bright wing: nothing of that size may hang where the player is looking for the
//     road), and out of Syndicate Territory (nothing new moves in the sky the Chairman's shots fly through).
// Steady light only; nothing blinks. Nothing here is nearer than 36 m to a centre line, and that only 95 m up.
// About 240 pieces (the count from the game's FTDRESS line is in SourceArt/Integration/people/L7_city_far.md).
Zone(TEXT("People far: the six districts"), 0.f, 7.66f);
	Far(TEXT("/Game/People/CityCenter/SM_FarTowerParty"), 110.0f, 150.0f, 8, 1, 1.f, 2.20f, 0.06f, -1, 0.7f, 10.0f);
	Far(TEXT("/Game/People/CityCenter/SM_FarTowerParty"), 110.0f, 150.0f, 8, 5, 1.f, 2.20f, 0.06f, 1, 0.7f, 10.0f);
	Far(TEXT("/Game/People/CityCenter/SM_FarTowerLoft"), 100.0f, 125.0f, 8, 5, 1.f, 2.30f, 0.06f, -1, 0.7f, 10.0f);
	Far(TEXT("/Game/People/CityCenter/SM_FarTowerLoft"), 100.0f, 125.0f, 8, 1, 1.f, 2.30f, 0.06f, 1, 0.7f, 10.0f);
	Far(TEXT("/Game/People/CityCenter/SM_FarConcert"), 100.0f, 130.0f, 16, 3, 1.f, 2.40f, 0.06f, -1, 0.7f, 8.0f);
	Far(TEXT("/Game/People/CityCenter/SM_FarConcert"), 100.0f, 130.0f, 16, 11, 1.f, 2.40f, 0.06f, 1, 0.7f, 8.0f);
	Far(TEXT("/Game/People/CityCenter/SM_FarSkyRun_A"), 100.0f, 140.0f, 16, 11, 1.f, 2.00f, 0.06f, -1, 0.7f, 8.0f);
	Far(TEXT("/Game/People/CityCenter/SM_FarSkyRun_A"), 100.0f, 140.0f, 16, 3, 1.f, 2.00f, 0.06f, 1, 0.7f, 8.0f);
	Far(TEXT("/Game/People/CityCenter/SM_FarGeese"), 40.0f, 46.0f, 12, 7, 1.f, 1.00f, 0.00f, 0, -105.0f, 6.0f);
Zone(TEXT("People far: Syndicate Territory"), 7.66f, 99.f);
	Far(TEXT("/Game/People/CityCenter/SM_FarTowerParty"), 110.0f, 150.0f, 48, 9, 1.f, 2.20f, 0.06f, 0, 0.7f, 10.0f);
	Far(TEXT("/Game/People/CityCenter/SM_FarTowerLoft"), 100.0f, 125.0f, 48, 33, 1.f, 2.30f, 0.06f, 0, 0.7f, 10.0f);
	Far(TEXT("/Game/People/CityCenter/SM_FarGeese"), 40.0f, 46.0f, 48, 21, 1.f, 1.00f, 0.00f, 0, -105.0f, 6.0f);
// the hang-gliders: only the level stretches between the corkscrews, jumps and loops
Zone(TEXT("People far: gliders 0.05"), 0.05f, 1.24f);
	Far(TEXT("/Game/People/CityCenter/SM_FarGliders"), 36.0f, 44.0f, 5, 2, 1.f, 1.20f, 0.00f, 0, -95.0f, 6.0f);
Zone(TEXT("People far: gliders 1.79"), 1.79f, 2.46f);
	Far(TEXT("/Game/People/CityCenter/SM_FarGliders"), 36.0f, 44.0f, 5, 2, 1.f, 1.20f, 0.00f, 0, -95.0f, 6.0f);
Zone(TEXT("People far: gliders 3.69"), 3.69f, 4.50f);
	Far(TEXT("/Game/People/CityCenter/SM_FarGliders"), 36.0f, 44.0f, 5, 2, 1.f, 1.20f, 0.00f, 0, -95.0f, 6.0f);
Zone(TEXT("People far: gliders 5.05"), 5.05f, 5.49f);
	Far(TEXT("/Game/People/CityCenter/SM_FarGliders"), 36.0f, 44.0f, 5, 2, 1.f, 1.20f, 0.00f, 0, -95.0f, 6.0f);
Zone(TEXT("People far: gliders 6.05"), 6.05f, 6.53f);
	Far(TEXT("/Game/People/CityCenter/SM_FarGliders"), 36.0f, 44.0f, 5, 2, 1.f, 1.20f, 0.00f, 0, -95.0f, 6.0f);
Zone(TEXT("People far: gliders 7.18"), 7.18f, 7.60f);
	Far(TEXT("/Game/People/CityCenter/SM_FarGliders"), 36.0f, 44.0f, 5, 2, 1.f, 1.20f, 0.00f, 0, -95.0f, 6.0f);
// ---- END far ----
}

{
// ---- BEGIN watch (ft_people_city_watch.py) ----
// THE WATCHERS: the city, climbed onto everything to see the chase. Every piece here is tall, lit (steadily) and 24 m
// or more from the road, and stands where the main cast does not: a footbridge over a side street's mouth (the
// G_Street gaps), a scaffold stand in an alley's mouth and a scaffold up the brick building behind it (the G_Alley
// gaps), the open decks of the garages (W_Garage) and the free half of the dear doors' roof terraces (W_Pent: the
// main cast's rally and the crew's crane stand 5.05 m the other way from the middle). A rule here copies the side,
// every and phase of the dressing rule it stands at (FTDressL7.inl): change one there, change it here.
// The dressing's alleys have a chance of 0.8 to 0.9 and their dice are their own: one stand or scaffold in ten stands
// in an empty gap (both are scaffolds on their own legs, so that reads).
Set(TEXT("PplWatchBridgeA"), { { TEXT("/Game/People/CityCenter/SM_WatchBridge_A"), 22.6f, -2.4f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplWatchBridgeB"), { { TEXT("/Game/People/CityCenter/SM_WatchBridge_B"), 22.6f, -2.4f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplWatchSteps"),   { { TEXT("/Game/People/CityCenter/SM_WatchSteps_A"), 22.6f, -1.9f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplWatchScaf"),    { { TEXT("/Game/People/CityCenter/SM_WatchBalcony_A"), 22.6f, 4.9f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplWatchGap"),     { { TEXT("/Game/People/CityCenter/SM_WatchSteps_A"), 22.6f, -1.9f, 0.0f, 0.0f, 1.0f }, { TEXT("/Game/People/CityCenter/SM_WatchBalcony_A"), 22.6f, 4.9f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplWatchDeck"),    { { TEXT("/Game/People/CityCenter/SM_WatchDeck_A"), 0.0f, 2.5f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplWatchRoof"),    { { TEXT("/Game/People/CityCenter/SM_WatchRoof_A"), 5.05f, 0.5f, 9.6f, 0.0f, 1.0f } });

Zone(TEXT("People watch: Fifth and Main"), 0.f, 1.10f);
// side streets: left the odd slots, right the even; alleys: left the even slots, right the odd
	Site(TEXT("PplWatchBridgeA"), -1, 29.f, 4, 1, 1.f);
	Site(TEXT("PplWatchBridgeB"), -1, 29.f, 4, 3, 0.5f);
	Site(TEXT("PplWatchBridgeB"), 1, 29.f, 4, 0, 1.f);
	Site(TEXT("PplWatchBridgeA"), 1, 29.f, 4, 2, 0.5f);
	Site(TEXT("PplWatchGap"), -1, 29.f, 4, 0, 1.f);
	Site(TEXT("PplWatchScaf"), -1, 29.f, 4, 2, 0.6f);
	Site(TEXT("PplWatchSteps"), 1, 29.f, 4, 1, 1.f);
	Site(TEXT("PplWatchGap"), 1, 29.f, 4, 3, 0.6f);
Zone(TEXT("People watch: Theatre Row"), 1.10f, 2.10f);
// alleys: every third slot, side by side in turn (G_Alley 0, 3, 0); side streets: left (3,1), right (3,2)
	Site(TEXT("PplWatchGap"), 0, 29.f, 3, 0, 1.f);
	Site(TEXT("PplWatchBridgeA"), -1, 29.f, 3, 1, 1.f);
	Site(TEXT("PplWatchBridgeB"), 1, 29.f, 3, 2, 1.f);
Zone(TEXT("People watch: The Garages"), 2.10f, 3.13f);
// a garage every other block (left (2,0), right (2,1)); an alley in every gap
	Site(TEXT("PplWatchDeck"), -1, 29.f, 2, 0, 1.f);
	Site(TEXT("PplWatchDeck"), 1, 29.f, 2, 1, 1.f);
	Site(TEXT("PplWatchSteps"), -1, 29.f, 4, 1, 1.f);
	Site(TEXT("PplWatchScaf"), -1, 29.f, 4, 3, 1.f);
	Site(TEXT("PplWatchSteps"), 1, 29.f, 4, 2, 1.f);
	Site(TEXT("PplWatchScaf"), 1, 29.f, 4, 0, 1.f);
Zone(TEXT("People watch: The Plaza"), 3.13f, 4.62f);
// side streets: left (2,0), right (2,1); the dear doors: left (4,2), right (4,3)
	Site(TEXT("PplWatchBridgeA"), -1, 29.f, 4, 0, 1.f);
	Site(TEXT("PplWatchBridgeB"), -1, 29.f, 4, 2, 0.5f);
	Site(TEXT("PplWatchBridgeB"), 1, 29.f, 4, 1, 1.f);
	Site(TEXT("PplWatchBridgeA"), 1, 29.f, 4, 3, 0.5f);
	Site(TEXT("PplWatchRoof"), -1, 29.f, 4, 2, 1.f);
	Site(TEXT("PplWatchRoof"), 1, 29.f, 4, 3, 1.f);
Zone(TEXT("People watch: Wall Street"), 4.62f, 6.29f);
// alleys: every other slot, side by side in turn (G_Alley 0, 2, 0): a side-0 Site takes its side from (slot / every),
// so the alleys are LEFT on the slots (4,0) and RIGHT on (4,2), and a rule with another "every" must name its side (a
// side-0 rule every 4 put half of these into the glass infills across the street: fixed in review). Side streets: left
// (4,1), right (4,3); the dear doors: left (4,1), right (4,2)
	Site(TEXT("PplWatchGap"), -1, 29.f, 4, 0, 1.f);
	Site(TEXT("PplWatchSteps"), 1, 29.f, 4, 2, 0.7f);
	Site(TEXT("PplWatchBridgeA"), -1, 29.f, 4, 1, 1.f);
	Site(TEXT("PplWatchBridgeB"), 1, 29.f, 4, 3, 1.f);
	Site(TEXT("PplWatchRoof"), -1, 29.f, 8, 1, 1.f);
	Site(TEXT("PplWatchRoof"), 1, 29.f, 8, 2, 1.f);
Zone(TEXT("People watch: Penthouse Row"), 6.29f, 7.66f);
// side streets: left (6,2), right (6,5); the dear doors: left (3,0) and (3,1), right (3,1) and (3,2)
	Site(TEXT("PplWatchBridgeB"), -1, 29.f, 6, 2, 1.f);
	Site(TEXT("PplWatchBridgeA"), 1, 29.f, 6, 5, 1.f);
	Site(TEXT("PplWatchRoof"), -1, 29.f, 3, 1, 0.8f);
	Site(TEXT("PplWatchRoof"), 1, 29.f, 3, 2, 0.8f);
Zone(TEXT("People watch: Syndicate Territory, the arena"), 7.66f, 13.56f);
// the roofs the city still holds: the dear doors (left (4,1), right (4,2)) and the one garage (left (8,7))
	Site(TEXT("PplWatchRoof"), -1, 29.f, 4, 1, 0.5f);
	Site(TEXT("PplWatchRoof"), 1, 29.f, 4, 2, 0.5f);
	Site(TEXT("PplWatchDeck"), -1, 29.f, 8, 7, 1.f);
Zone(TEXT("People watch: Syndicate Territory, the circuit"), 13.56f, 99.f);
	Site(TEXT("PplWatchRoof"), -1, 29.f, 4, 1, 0.2f);
	Site(TEXT("PplWatchRoof"), 1, 29.f, 4, 2, 0.2f);
	Site(TEXT("PplWatchDeck"), -1, 29.f, 8, 7, 0.4f);
// ---- END watch ----
}

{
// ---- BEGIN p3a (gaps, ft_people_city_p3a.py) ----
Zone(TEXT("P3A gaps: Fifth and Main"), 0.3f, 1.1f);
// THE GAPS (pass 3, builder p3a). Sixteen meshes, /Game/People/CityCenter/SM_P3A_*: all LOW (the generator's log: tops
// 1.84 to 1.94 m as built), unlit, nothing that blinks. Every rule is a Site with its side named: nothing is jittered
// along the road. Where the road had nobody (read from the table above and from FTDressL7.inl):
//   - SYNDICATE TERRITORY (7.66 km to the end, 70 % of the road): half its pockets, the middle of every block, and the
//     whole strip. Behind the Syndicate's barrier rows (they end 20.4 m out) the pavement is free from 20.6 m to the
//     podiums' fixtures at 24.5 m, the whole length of a block;
//   - the six districts before it: the strip under the awnings in the slots between the commuters' pieces, and the
//     few pockets left (The Garages: left phase 3 and right phase 0 of 4; Wall Street: right phase 2 of 4).
// The sets (a rule takes slot s when (s + phase) % every == 0: two rules with the same every and another phase never meet):
//   Pk  a pocket of the six districts: anchor 17.6 m, 20.6 m along, 3.9 m out. The piece (6.0 by 2.4 m) lies 20.3 to 22.7 m out
//   Sy  a pocket of Syndicate Territory: 4.3 m out, so 20.7 to 23.1 m, behind the barrier rows
//   Md  the middle of a block in Syndicate Territory: 1 m back from the anchor, 20.7 to 23.1 m out
//   St  the strip: anchor 23.72 m, the middle of the block. 20 m of duckboard walkway 23.0 to 24.5 m out at the most (its
//       boards 23.2 to 24.2 m), its people walking it along the road and coming round again. A commuters' piece (30 m, a
//       Scatter jittered 11 m) in the next slot ends 18.7 m from this one's anchor at the nearest; the walkway is 21.4 m long.
// Nothing here is nearer than 20.3 m to a centre line. People: the press behind a rail, neighbours painting colour over a
// black hoarding, a two-tier stand of pallets, buskers, chess tables, a flower stall, a household moving out, the street
// cleaners; on the strip errands, a fun run, a marching band, and households leaving Syndicate Territory.
Set(TEXT("PplP3ASyPressA"), { { TEXT("/Game/People/CityCenter/SM_P3A_Press_A"), 20.6f, 4.3f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3ASyPressB"), { { TEXT("/Game/People/CityCenter/SM_P3A_Press_B"), 20.6f, 4.3f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3ASyMuralA"), { { TEXT("/Game/People/CityCenter/SM_P3A_Mural_A"), 20.6f, 4.3f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3ASyMuralB"), { { TEXT("/Game/People/CityCenter/SM_P3A_Mural_B"), 20.6f, 4.3f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3ASyBleacherA"), { { TEXT("/Game/People/CityCenter/SM_P3A_Bleacher_A"), 20.6f, 4.3f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3ASyBleacherB"), { { TEXT("/Game/People/CityCenter/SM_P3A_Bleacher_B"), 20.6f, 4.3f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3APkBuskersA"), { { TEXT("/Game/People/CityCenter/SM_P3A_Buskers_A"), 20.6f, 3.9f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3ASyBuskersA"), { { TEXT("/Game/People/CityCenter/SM_P3A_Buskers_A"), 20.6f, 4.3f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3ASyBuskersB"), { { TEXT("/Game/People/CityCenter/SM_P3A_Buskers_B"), 20.6f, 4.3f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3APkChessA"), { { TEXT("/Game/People/CityCenter/SM_P3A_Chess_A"), 20.6f, 3.9f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3AMdChessA"), { { TEXT("/Game/People/CityCenter/SM_P3A_Chess_A"), -1.0f, 4.3f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3APkFlowersA"), { { TEXT("/Game/People/CityCenter/SM_P3A_Flowers_A"), 20.6f, 3.9f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3ASyFlowersA"), { { TEXT("/Game/People/CityCenter/SM_P3A_Flowers_A"), 20.6f, 4.3f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3AMdFlowersA"), { { TEXT("/Game/People/CityCenter/SM_P3A_Flowers_A"), -1.0f, 4.3f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3AMdPortersA"), { { TEXT("/Game/People/CityCenter/SM_P3A_Porters_A"), -1.0f, 4.3f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3APkSweepersA"), { { TEXT("/Game/People/CityCenter/SM_P3A_Sweepers_A"), 20.6f, 3.9f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3AMdSweepersA"), { { TEXT("/Game/People/CityCenter/SM_P3A_Sweepers_A"), -1.0f, 4.3f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3AMdPressB"), { { TEXT("/Game/People/CityCenter/SM_P3A_Press_B"), -1.0f, 4.3f, 0.0f, 0.0f, 1.0f } });	// review round: the raised pieces read best from the road
Set(TEXT("PplP3AMdBleacherA"), { { TEXT("/Game/People/CityCenter/SM_P3A_Bleacher_A"), -1.0f, 4.3f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3AStStripA"), { { TEXT("/Game/People/CityCenter/SM_P3A_Strip_A"), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3AStStripB"), { { TEXT("/Game/People/CityCenter/SM_P3A_Strip_B"), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3AStStripC"), { { TEXT("/Game/People/CityCenter/SM_P3A_Strip_C"), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3AStStripD"), { { TEXT("/Game/People/CityCenter/SM_P3A_Strip_D"), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } });
// the strip, in the slots the commuters leave (they walk left (2,0), right (2,1))
	Site(TEXT("PplP3AStStripD"), -1, 23.72f, 4, 1, 1.0f);
	Site(TEXT("PplP3AStStripB"), -1, 23.72f, 4, 3, 0.6f);
	Site(TEXT("PplP3AStStripA"), 1, 23.72f, 4, 2, 1.0f);
	Site(TEXT("PplP3AStStripB"), 1, 23.72f, 4, 0, 0.6f);
Zone(TEXT("P3A gaps: Theatre Row (1)"), 1.1f, 1.34f);
// the strip between the commuters; never a picture house slot on its own side (the queue stands there: left (2,0), right (2,1))
	Site(TEXT("PplP3AStStripA"), -1, 23.72f, 4, 3, 1.0f);
	Site(TEXT("PplP3AStStripB"), -1, 23.72f, 4, 1, 0.5f);
	Site(TEXT("PplP3AStStripD"), 1, 23.72f, 4, 0, 1.0f);
	Site(TEXT("PplP3AStStripB"), 1, 23.72f, 4, 2, 0.5f);
Zone(TEXT("P3A gaps: Theatre Row (2)"), 1.443f, 2.1f);
	Site(TEXT("PplP3AStStripA"), -1, 23.72f, 4, 3, 1.0f);
	Site(TEXT("PplP3AStStripB"), -1, 23.72f, 4, 1, 0.5f);
	Site(TEXT("PplP3AStStripD"), 1, 23.72f, 4, 0, 1.0f);
	Site(TEXT("PplP3AStStripB"), 1, 23.72f, 4, 2, 0.5f);
Zone(TEXT("P3A gaps: The Garages (1)"), 2.1f, 2.56f);
// commuters walk left (3,0), right (3,1). The pockets the first pass left: left phase 3 of 4, right phase 0 of 4 (every gap is an alley)
	Site(TEXT("PplP3AStStripB"), -1, 23.72f, 3, 1, 1.0f);
	Site(TEXT("PplP3AStStripA"), 1, 23.72f, 3, 0, 1.0f);
	Site(TEXT("PplP3APkChessA"), -1, 17.6f, 8, 3, 1.0f);
	Site(TEXT("PplP3APkBuskersA"), -1, 17.6f, 8, 7, 1.0f);
	Site(TEXT("PplP3APkSweepersA"), 1, 17.6f, 8, 0, 1.0f);
	Site(TEXT("PplP3APkBuskersA"), 1, 17.6f, 8, 4, 1.0f);
Zone(TEXT("P3A gaps: The Garages (2)"), 2.665f, 3.04f);
	Site(TEXT("PplP3AStStripB"), -1, 23.72f, 3, 1, 1.0f);
	Site(TEXT("PplP3AStStripA"), 1, 23.72f, 3, 0, 1.0f);
	Site(TEXT("PplP3APkChessA"), -1, 17.6f, 8, 3, 1.0f);
	Site(TEXT("PplP3APkBuskersA"), -1, 17.6f, 8, 7, 1.0f);
	Site(TEXT("PplP3APkSweepersA"), 1, 17.6f, 8, 0, 1.0f);
	Site(TEXT("PplP3APkBuskersA"), 1, 17.6f, 8, 4, 1.0f);
Zone(TEXT("P3A gaps: The Plaza"), 3.208f, 4.53f);
// the strip between the commuters (left (2,0), right (2,1)); every pocket here is taken
	Site(TEXT("PplP3AStStripD"), -1, 23.72f, 4, 1, 1.0f);
	Site(TEXT("PplP3AStStripA"), -1, 23.72f, 4, 3, 0.6f);
	Site(TEXT("PplP3AStStripB"), 1, 23.72f, 4, 2, 1.0f);
	Site(TEXT("PplP3AStStripD"), 1, 23.72f, 4, 0, 0.6f);
Zone(TEXT("P3A gaps: Wall Street (1)"), 4.701f, 5.59f);
// commuters walk left (2,0) and (4,3), right (2,1) and (4,2): one free slot in four a side. The pocket before the right-hand alleys (phase 2 of 4) was empty
	Site(TEXT("PplP3AStStripA"), -1, 23.72f, 4, 1, 1.0f);
	Site(TEXT("PplP3AStStripB"), 1, 23.72f, 4, 0, 1.0f);
	Site(TEXT("PplP3APkFlowersA"), 1, 17.6f, 8, 2, 1.0f);
	Site(TEXT("PplP3APkChessA"), 1, 17.6f, 8, 6, 1.0f);
Zone(TEXT("P3A gaps: Wall Street (2)"), 5.696f, 6.29f);
	Site(TEXT("PplP3AStStripA"), -1, 23.72f, 4, 1, 1.0f);
	Site(TEXT("PplP3AStStripB"), 1, 23.72f, 4, 0, 1.0f);
	Site(TEXT("PplP3APkFlowersA"), 1, 17.6f, 8, 2, 1.0f);
	Site(TEXT("PplP3APkChessA"), 1, 17.6f, 8, 6, 1.0f);
Zone(TEXT("P3A gaps: Penthouse Row (1)"), 6.29f, 6.63f);
// commuters walk left (3,0), right (3,1); the picture house and its queue are right (3,0)
	Site(TEXT("PplP3AStStripA"), -1, 23.72f, 3, 1, 0.7f);
	Site(TEXT("PplP3AStStripD"), 1, 23.72f, 3, 2, 0.7f);
Zone(TEXT("P3A gaps: Penthouse Row (2)"), 6.737f, 7.48f);
	Site(TEXT("PplP3AStStripA"), -1, 23.72f, 3, 1, 0.7f);
	Site(TEXT("PplP3AStStripD"), 1, 23.72f, 3, 2, 0.7f);
Zone(TEXT("P3A gaps: Syndicate Territory, the arena"), 7.66f, 13.56f);
// the first pass holds the left pockets (4,0) and (4,1), the right (4,2) and (4,3), the crew left (16,5) and right (16,13): these are the others. Md: the middle of a block. St: the strip, which nobody walked
	Site(TEXT("PplP3ASyPressA"), -1, 17.6f, 8, 2, 0.7f);
	Site(TEXT("PplP3ASyMuralA"), -1, 17.6f, 8, 6, 0.7f);
	Site(TEXT("PplP3ASyBleacherA"), -1, 17.6f, 8, 3, 0.7f);
	Site(TEXT("PplP3ASyBuskersB"), -1, 17.6f, 8, 7, 0.7f);
	Site(TEXT("PplP3ASyBleacherB"), 1, 17.6f, 8, 0, 0.7f);
	Site(TEXT("PplP3ASyPressB"), 1, 17.6f, 8, 4, 0.7f);
	Site(TEXT("PplP3ASyMuralB"), 1, 17.6f, 16, 1, 0.7f);
	Site(TEXT("PplP3ASyFlowersA"), 1, 17.6f, 16, 9, 0.7f);
	Site(TEXT("PplP3AMdPortersA"), -1, 17.6f, 8, 1, 0.7f);
	Site(TEXT("PplP3AMdSweepersA"), 1, 17.6f, 8, 3, 0.7f);
	Site(TEXT("PplP3AMdChessA"), -1, 17.6f, 16, 5, 0.7f);
	Site(TEXT("PplP3AMdPortersA"), 1, 17.6f, 16, 14, 0.7f);
	Site(TEXT("PplP3AStStripC"), -1, 23.72f, 8, 0, 0.7f);
	Site(TEXT("PplP3AStStripC"), 1, 23.72f, 8, 5, 0.7f);
	Site(TEXT("PplP3AStStripB"), -1, 23.72f, 16, 12, 0.7f);
	Site(TEXT("PplP3AStStripA"), 1, 23.72f, 16, 7, 0.7f);
// REVIEW AND FIX ROUND (16 Nov 2026, see SourceArt/Integration/people/L7_city_p3.md): the driver found the right-hand pavement of the arena empty at street level for long stretches.
// Three more rules on the right, in slots where no Md or St piece of this block stands: the press stand and the pallet stand at a block's middle, one more walkway.
	Site(TEXT("PplP3AMdPressB"), 1, 17.6f, 8, 1, 0.7f);
	Site(TEXT("PplP3AMdBleacherA"), 1, 17.6f, 8, 2, 0.7f);
	Site(TEXT("PplP3AStStripD"), 1, 23.72f, 16, 15, 0.7f);
Zone(TEXT("P3A gaps: Syndicate Territory, the circuit"), 13.56f, 99.0f);
// the same places, one slot in two a side, at 0.55: the first pass leaves three pockets in four empty here
	Site(TEXT("PplP3ASyBleacherA"), -1, 17.6f, 16, 2, 0.55f);
	Site(TEXT("PplP3ASyPressA"), -1, 17.6f, 16, 10, 0.55f);
	Site(TEXT("PplP3ASyMuralA"), -1, 17.6f, 16, 7, 0.55f);
	Site(TEXT("PplP3ASyBuskersB"), -1, 17.6f, 16, 15, 0.55f);
	Site(TEXT("PplP3ASyPressB"), 1, 17.6f, 16, 0, 0.55f);
	Site(TEXT("PplP3ASyBleacherB"), 1, 17.6f, 16, 8, 0.55f);
	Site(TEXT("PplP3ASyMuralB"), 1, 17.6f, 16, 4, 0.55f);
	Site(TEXT("PplP3ASyBuskersA"), 1, 17.6f, 16, 12, 0.55f);
	Site(TEXT("PplP3AMdPortersA"), -1, 17.6f, 16, 5, 0.55f);
	Site(TEXT("PplP3AMdSweepersA"), 1, 17.6f, 16, 11, 0.55f);
	Site(TEXT("PplP3AMdFlowersA"), -1, 17.6f, 16, 13, 0.55f);
	Site(TEXT("PplP3AMdChessA"), 1, 17.6f, 16, 3, 0.55f);
	Site(TEXT("PplP3AStStripC"), -1, 23.72f, 16, 9, 0.55f);
	Site(TEXT("PplP3AStStripC"), 1, 23.72f, 16, 1, 0.55f);
	Site(TEXT("PplP3AStStripD"), -1, 23.72f, 16, 1, 0.55f);
	Site(TEXT("PplP3AStStripB"), 1, 23.72f, 16, 9, 0.55f);
// ---- END p3a ----
}

{
// ---- BEGIN p3b (scenes, ft_people_city_p3b.py) ----
// REVIEW AND FIX ROUND (16 Nov 2026, see SourceArt/Integration/people/L7_city_p3.md): p3b, p3c, p3d and p3f had each copied the podium rules without seeing the others, so two or three
// pieces stood in one roof spot (and the mayor inside the lantern bridge). Where rules of two blocks met on a side, in a slot and within 6.5 m along,
// 3 m out and 3 m up, the slots were shared out by turns (C:/ftwork/jobs/L7p3fix/resolve.py): such a rule now has a multiple of its podium's every
// (twice: two took turns; three times: three), sometimes as two or four lines. One roof spot, one piece. The lines changed say what they were.
// TWELVE STORY GROUPS that could only be in this city at midnight, one mesh a scene (8 to 15 figures and the props that tell it):
// a wedding, an open-air picture show, a jazz band, a boxing gym, a pigeon fancier's loft, sign hangers mending a neon sign, a
// fashion show, a basketball court, a farm, a karate class (those ten on roofs), the brokers out on a bank's steps at the closing
// bell, and the mayor cutting a ribbon on the plaza. All twelve are TALL (tops 2.92 to 4.8 m as built) and lit steadily; nothing
// blinks; every one stands 24 m or more from the centre line (the brokers' and the mayor's fronts 24.8 m, the roofs 25.5 m).
// WHERE, and why the numbers are what they are. The avenue is a canyon and all its ground is the dressing's: the scenes stand UP,
// on podium roofs that carry nobody yet. A rule copies the side, every and phase of the dressing's podium rule it stands on
// (FTDressL7.inl; split in two or four where the scenes take turns): change one there, change it here, or a scene floats.
// A roof scene is 7.6 m along the street and 2.7 m deep at the roof's front edge; a set's "along" picks the free part of the roof:
//   S2L / S2R  the two tall shops of SM_Shops2 (8.88 m; the main cast's party is on the low middle shop, 5.48 m)
//   S1M        the tall middle shop of SM_Shops1 (8.88 m; the main cast's pieces are on its low ends)
//   ARC        the arcade of SM_Shops3 (8.88 m)
//   MQA / MQB  a picture house's roof (11.82 m): MQA beside the main cast's party (which is at -4 m), MQB where there is none
//   BNK        a bank's roof (10.36 m), left of its CASH sign
//   YDA / YDB  a service yard's shed (6.64 m): YDB is the end the main cast's party leaves free
//   STP        a bank's steps, between the columns (0.6 m up)      MON  the plaza before a monument (the rally's place)
// On the left in Fifth and Main the middle-of-podium spots are used only where no pier of the elevated line stands in the podium.
// ESTIMATE (slots counted by C:/ftwork/jobs/L7p3b/make_inl.py: 45 m slots to 25 km, the six corkscrews, jumps and loops left
// out, chances as expected values): 382 placed. By mesh: RoofWedding 25, RoofCinema 26, RoofJazz 25, RoofBoxing 32, PigeonLoft 38, NeonCrew 41, RoofCatwalk 30, RoofHoops 38, RoofFarm 31, RoofDojo 31, Brokers 52, Mayor 14.
// Not seen in the game. Where the other p3 builders stand on these roofs was not known when this was written.
Zone(TEXT("People p3b, scenes"), 0.f, 0.f);       // (the block starts with a Zone; it has no rules: every rule is in a district below)
#define P3B(n) TEXT("/Game/People/CityCenter/SM_P3B_" n)
Set(TEXT("PplP3BRoofJazzS2L"), { { P3B("RoofJazz"), -10.0f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofHoopsS2L"), { { P3B("RoofHoops"), -10.0f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BPigeonLoftS2R"), { { P3B("PigeonLoft"), 11.33f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofWeddingS2R"), { { P3B("RoofWedding"), 11.33f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofFarmS2L"), { { P3B("RoofFarm"), -10.0f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofDojoS2L"), { { P3B("RoofDojo"), -10.0f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofBoxingS2R"), { { P3B("RoofBoxing"), 11.33f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BNeonCrewS2R"), { { P3B("NeonCrew"), 11.33f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofCatwalkS1M"), { { P3B("RoofCatwalk"), 0.67f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofCinemaS1M"), { { P3B("RoofCinema"), 0.67f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofHoopsARC"), { { P3B("RoofHoops"), 2.0f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofJazzARC"), { { P3B("RoofJazz"), 2.0f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofWeddingS1M"), { { P3B("RoofWedding"), 0.67f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofFarmARC"), { { P3B("RoofFarm"), 2.0f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofDojoS1M"), { { P3B("RoofDojo"), 0.67f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofBoxingARC"), { { P3B("RoofBoxing"), 2.0f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofCinemaMQA"), { { P3B("RoofCinema"), 5.6f, 0.0f, 11.82f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofCatwalkMQB"), { { P3B("RoofCatwalk"), -4.0f, 0.0f, 11.82f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofJazzMQA"), { { P3B("RoofJazz"), 5.6f, 0.0f, 11.82f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofCinemaMQB"), { { P3B("RoofCinema"), -4.0f, 0.0f, 11.82f, 0.0f, 1.0f } });
Set(TEXT("PplP3BNeonCrewARC"), { { P3B("NeonCrew"), 2.0f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofCatwalkARC"), { { P3B("RoofCatwalk"), 2.0f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BNeonCrewS1M"), { { P3B("NeonCrew"), 0.67f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofBoxingYDB"), { { P3B("RoofBoxing"), 9.5f, 3.4f, 6.64f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofHoopsYDA"), { { P3B("RoofHoops"), -7.0f, 3.4f, 6.64f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofDojoYDB"), { { P3B("RoofDojo"), 9.5f, 3.4f, 6.64f, 0.0f, 1.0f } });
Set(TEXT("PplP3BPigeonLoftYDB"), { { P3B("PigeonLoft"), 9.5f, 3.4f, 6.64f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofBoxingYDA"), { { P3B("RoofBoxing"), -7.0f, 3.4f, 6.64f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofFarmYDB"), { { P3B("RoofFarm"), 9.5f, 3.4f, 6.64f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofFarmS1M"), { { P3B("RoofFarm"), 0.67f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofCatwalkS2L"), { { P3B("RoofCatwalk"), -10.0f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BPigeonLoftS2L"), { { P3B("PigeonLoft"), -10.0f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofDojoS2R"), { { P3B("RoofDojo"), 11.33f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofHoopsS2R"), { { P3B("RoofHoops"), 11.33f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BMayorMON"), { { P3B("Mayor"), 22.6f, -3.1f, 0.0f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofWeddingBNK"), { { P3B("RoofWedding"), -8.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofCatwalkBNK"), { { P3B("RoofCatwalk"), -8.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofJazzBNK"), { { P3B("RoofJazz"), -8.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofCinemaBNK"), { { P3B("RoofCinema"), -8.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3BNeonCrewBNK"), { { P3B("NeonCrew"), -8.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofBoxingS2L"), { { P3B("RoofBoxing"), -10.0f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofFarmS2R"), { { P3B("RoofFarm"), 11.33f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofHoopsYDB"), { { P3B("RoofHoops"), 9.5f, 3.4f, 6.64f, 0.0f, 1.0f } });
Set(TEXT("PplP3BBrokersSTP"), { { P3B("Brokers"), 0.0f, -3.6f, 0.6f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofCatwalkMQA"), { { P3B("RoofCatwalk"), 5.6f, 0.0f, 11.82f, 0.0f, 1.0f } });
Set(TEXT("PplP3BRoofFarmBNK"), { { P3B("RoofFarm"), -8.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3BPigeonLoftBNK"), { { P3B("PigeonLoft"), -8.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3BNeonCrewS2L"), { { P3B("NeonCrew"), -10.0f, 0.0f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3BNeonCrewYDB"), { { P3B("NeonCrew"), 9.5f, 3.4f, 6.64f, 0.0f, 1.0f } });
Zone(TEXT("People p3b: Fifth and Main"), 0.0f, 1.1f);
// the tall shops at both ends of every BAR HATS LOANS row (W_Shops2s: left (3,2), right (3,0)), the tall toy shop on the right (W_Shops1 (3,1)), the arcade on the right (W_Shops3 (3,2))
	Site(TEXT("PplP3BRoofJazzS2L"), -1, 29.0f, 18, 14, 1.0f);	// was (6,2): shares its roof spot by turns
	Site(TEXT("PplP3BRoofHoopsS2L"), -1, 29.0f, 18, 17, 1.0f);	// was (6,5): shares its roof spot by turns
	Site(TEXT("PplP3BPigeonLoftS2R"), -1, 29.0f, 6, 2, 1.0f);
	Site(TEXT("PplP3BRoofWeddingS2R"), -1, 29.0f, 12, 11, 1.0f);	// was (6,5): shares its roof spot by turns
	Site(TEXT("PplP3BRoofFarmS2L"), 1, 29.0f, 18, 0, 1.0f);	// was (6,0): shares its roof spot by turns
	Site(TEXT("PplP3BRoofDojoS2L"), 1, 29.0f, 12, 9, 1.0f);	// was (6,3): shares its roof spot by turns
	Site(TEXT("PplP3BRoofBoxingS2R"), 1, 29.0f, 6, 0, 1.0f);
	Site(TEXT("PplP3BNeonCrewS2R"), 1, 29.0f, 18, 15, 1.0f);	// was (6,3): shares its roof spot by turns
	Site(TEXT("PplP3BRoofCatwalkS1M"), 1, 29.0f, 12, 7, 1.0f);	// was (6,1): shares its roof spot by turns
	Site(TEXT("PplP3BRoofCinemaS1M"), 1, 29.0f, 12, 10, 1.0f);	// was (6,4): shares its roof spot by turns
	Site(TEXT("PplP3BRoofHoopsARC"), 1, 29.0f, 12, 8, 1.0f);	// was (6,2): shares its roof spot by turns
	Site(TEXT("PplP3BRoofJazzARC"), 1, 29.0f, 12, 11, 1.0f);	// was (6,5): shares its roof spot by turns
Zone(TEXT("People p3b: Fifth and Main, left, to 0.62 km"), 0.0f, 0.62f);
// the left side's toy shop (W_Shops1L (3,0)) and arcade (W_Shops3 (3,1)), only in the slots where no pier of the elevated line (ElOverRun: the even slots here) comes down through the podium's middle
	Site(TEXT("PplP3BRoofWeddingS1M"), -1, 29.0f, 12, 9, 1.0f);	// was (6,3): shares its roof spot by turns
	Site(TEXT("PplP3BRoofFarmARC"), -1, 29.0f, 12, 7, 1.0f);	// was (6,1): shares its roof spot by turns
Zone(TEXT("People p3b: Fifth and Main, left, from 0.90 km"), 0.9f, 1.1f);
// the same, past the line's last pier
	Site(TEXT("PplP3BRoofDojoS1M"), -1, 29.0f, 12, 0, 1.0f);	// was (6,0): shares its roof spot by turns
	Site(TEXT("PplP3BRoofBoxingARC"), -1, 29.0f, 12, 10, 1.0f);	// was (3,1): shares its roof spot by turns
	Site(TEXT("PplP3BRoofBoxingARC"), -1, 29.0f, 12, 7, 1.0f);
Zone(TEXT("People p3b: Theatre Row"), 1.1f, 2.1f);
// beside the party on every other picture house (W_Marquee left (4,0), right (4,1)), alone on the others (left (4,2), right (4,3)); the arcades (W_Shops3 left (4,1), right (4,2)), the tall shops (W_Shops2s left (4,3), W_Shops1 right (4,0))
	Site(TEXT("PplP3BRoofCinemaMQA"), -1, 29.0f, 12, 0, 1.0f);	// was (4,0): shares its roof spot by turns
	Site(TEXT("PplP3BRoofCatwalkMQB"), -1, 29.0f, 12, 10, 1.0f);	// was (4,2): shares its roof spot by turns
	Site(TEXT("PplP3BRoofJazzMQA"), 1, 29.0f, 12, 9, 1.0f);	// was (4,1): shares its roof spot by turns
	Site(TEXT("PplP3BRoofCinemaMQB"), 1, 29.0f, 12, 11, 1.0f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3BNeonCrewARC"), -1, 29.0f, 8, 5, 1.0f);	// was (4,1): shares its roof spot by turns
	Site(TEXT("PplP3BRoofJazzS2L"), -1, 29.0f, 12, 11, 1.0f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3BRoofWeddingS2R"), -1, 29.0f, 8, 7, 1.0f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3BRoofCatwalkARC"), 1, 29.0f, 8, 6, 1.0f);	// was (4,2): shares its roof spot by turns
	Site(TEXT("PplP3BNeonCrewS1M"), 1, 29.0f, 8, 0, 1.0f);	// was (4,0): shares its roof spot by turns
Zone(TEXT("People p3b: The Garages"), 2.1f, 3.13f);
// the service yards' sheds (W_Yards left (2,1), right (2,0)): both ends of the empty ones (left (4,1), right (4,0)), the free end of those with a party (left (4,3), right (4,2))
	Site(TEXT("PplP3BRoofBoxingYDB"), -1, 29.0f, 12, 11, 1.0f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3BRoofHoopsYDA"), -1, 29.0f, 8, 5, 1.0f);	// was (4,1): shares its roof spot by turns
	Site(TEXT("PplP3BRoofDojoYDB"), -1, 29.0f, 12, 9, 1.0f);	// was (4,1): shares its roof spot by turns
	Site(TEXT("PplP3BPigeonLoftYDB"), 1, 29.0f, 12, 10, 1.0f);	// was (4,2): shares its roof spot by turns
	Site(TEXT("PplP3BRoofBoxingYDA"), 1, 29.0f, 8, 0, 1.0f);	// was (4,0): shares its roof spot by turns
	Site(TEXT("PplP3BRoofFarmYDB"), 1, 29.0f, 12, 0, 1.0f);	// was (4,0): shares its roof spot by turns
Zone(TEXT("People p3b: The Plaza"), 3.13f, 4.62f);
// the tall toy shop (W_Shops1L left (4,0)), the tall shops (W_Shops2s right (4,1)), and the mayor before every monument that has no rally (G_Monument left (2,1) less the rally's (4,1); right (2,0) less (4,0))
	Site(TEXT("PplP3BRoofWeddingS1M"), -1, 29.0f, 16, 0, 1.0f);	// was (8,0): shares its roof spot by turns
	Site(TEXT("PplP3BRoofFarmS1M"), -1, 29.0f, 16, 12, 1.0f);	// was (8,4): shares its roof spot by turns
	Site(TEXT("PplP3BRoofCatwalkS2L"), 1, 29.0f, 24, 17, 1.0f);	// was (8,1): shares its roof spot by turns
	Site(TEXT("PplP3BPigeonLoftS2L"), 1, 29.0f, 24, 21, 1.0f);	// was (8,5): shares its roof spot by turns
	Site(TEXT("PplP3BRoofDojoS2R"), 1, 29.0f, 24, 17, 1.0f);	// was (8,1): shares its roof spot by turns
	Site(TEXT("PplP3BRoofHoopsS2R"), 1, 29.0f, 24, 21, 1.0f);	// was (8,5): shares its roof spot by turns
	Site(TEXT("PplP3BMayorMON"), -1, 29.0f, 8, 7, 1.0f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3BMayorMON"), 1, 29.0f, 8, 6, 1.0f);	// was (4,2): shares its roof spot by turns
Zone(TEXT("People p3b: Wall Street"), 4.62f, 6.29f);
// every bank's roof (W_Banks left (2,0), right (2,1)) and its steps, the tall shops (W_Shops2s left (4,3)), the free end of the yard's shed (W_Yards right (4,0))
	Site(TEXT("PplP3BRoofWeddingBNK"), -1, 29.0f, 24, 0, 1.0f);	// was (8,0): shares its roof spot by turns
	Site(TEXT("PplP3BRoofCatwalkBNK"), -1, 29.0f, 24, 18, 1.0f);	// was (8,2): shares its roof spot by turns
	Site(TEXT("PplP3BRoofJazzBNK"), -1, 29.0f, 24, 20, 1.0f);	// was (8,4): shares its roof spot by turns
	Site(TEXT("PplP3BRoofCinemaBNK"), -1, 29.0f, 24, 22, 1.0f);	// was (8,6): shares its roof spot by turns
	Site(TEXT("PplP3BRoofCinemaBNK"), 1, 29.0f, 24, 17, 1.0f);	// was (8,1): shares its roof spot by turns
	Site(TEXT("PplP3BNeonCrewBNK"), 1, 29.0f, 24, 19, 1.0f);	// was (8,3): shares its roof spot by turns
	Site(TEXT("PplP3BRoofCatwalkBNK"), 1, 29.0f, 24, 21, 1.0f);	// was (8,5): shares its roof spot by turns
	Site(TEXT("PplP3BRoofWeddingBNK"), 1, 29.0f, 24, 23, 1.0f);	// was (8,7): shares its roof spot by turns
	Site(TEXT("PplP3BRoofBoxingS2L"), -1, 29.0f, 24, 19, 1.0f);	// was (8,3): shares its roof spot by turns
	Site(TEXT("PplP3BRoofDojoS2L"), -1, 29.0f, 24, 23, 1.0f);	// was (8,7): shares its roof spot by turns
	Site(TEXT("PplP3BRoofFarmS2R"), -1, 29.0f, 16, 11, 1.0f);	// was (8,3): shares its roof spot by turns
	Site(TEXT("PplP3BPigeonLoftS2R"), -1, 29.0f, 16, 15, 1.0f);	// was (8,7): shares its roof spot by turns
	Site(TEXT("PplP3BRoofHoopsYDB"), 1, 29.0f, 12, 0, 1.0f);	// was (4,0): shares its roof spot by turns
	Site(TEXT("PplP3BBrokersSTP"), -1, 29.0f, 2, 0, 0.75f);
	Site(TEXT("PplP3BBrokersSTP"), 1, 29.0f, 2, 1, 0.75f);
Zone(TEXT("People p3b: Penthouse Row"), 6.29f, 7.66f);
// the bank (W_Banks left (3,2)): roof and steps; the picture house (W_Marquee right (3,0)): beside its party (6,3), alone (6,0)
	Site(TEXT("PplP3BRoofJazzBNK"), -1, 29.0f, 18, 14, 1.0f);	// was (6,2): shares its roof spot by turns
	Site(TEXT("PplP3BRoofWeddingBNK"), -1, 29.0f, 18, 17, 1.0f);	// was (6,5): shares its roof spot by turns
	Site(TEXT("PplP3BBrokersSTP"), -1, 29.0f, 3, 2, 1.0f);
	Site(TEXT("PplP3BRoofCatwalkMQA"), 1, 29.0f, 18, 15, 1.0f);	// was (6,3): shares its roof spot by turns
	Site(TEXT("PplP3BRoofCinemaMQB"), 1, 29.0f, 18, 0, 1.0f);	// was (6,0): shares its roof spot by turns
Zone(TEXT("People p3b: Syndicate Territory, the arena"), 7.66f, 13.56f);
// the roofs the city still holds: the bank (W_Bank left (8,3)) and its steps, the tall shops (W_Shops2 right (8,4)), the free end of the yard's shed (W_Yard right (8,0)). Steady light only
	Site(TEXT("PplP3BRoofFarmBNK"), -1, 29.0f, 48, 35, 0.9f);	// was (16,3): shares its roof spot by turns
	Site(TEXT("PplP3BPigeonLoftBNK"), -1, 29.0f, 48, 43, 0.9f);	// was (16,11): shares its roof spot by turns
	Site(TEXT("PplP3BBrokersSTP"), -1, 29.0f, 8, 3, 0.7f);
	Site(TEXT("PplP3BRoofDojoS2L"), 1, 29.0f, 48, 36, 0.9f);	// was (16,4): shares its roof spot by turns
	Site(TEXT("PplP3BNeonCrewS2L"), 1, 29.0f, 48, 44, 0.9f);	// was (16,12): shares its roof spot by turns
	Site(TEXT("PplP3BRoofHoopsS2R"), 1, 29.0f, 48, 36, 0.9f);	// was (16,4): shares its roof spot by turns
	Site(TEXT("PplP3BRoofBoxingS2R"), 1, 29.0f, 48, 44, 0.9f);	// was (16,12): shares its roof spot by turns
	Site(TEXT("PplP3BPigeonLoftYDB"), 1, 29.0f, 48, 0, 0.9f);	// was (16,0): shares its roof spot by turns
	Site(TEXT("PplP3BNeonCrewYDB"), 1, 29.0f, 48, 40, 0.9f);	// was (16,8): shares its roof spot by turns
Zone(TEXT("People p3b: Syndicate Territory, the circuit"), 13.56f, 99.0f);
// the same, thinly (the boss fight seldom reaches it)
	Site(TEXT("PplP3BRoofFarmBNK"), -1, 29.0f, 48, 35, 0.3f);	// was (16,3): shares its roof spot by turns
	Site(TEXT("PplP3BPigeonLoftBNK"), -1, 29.0f, 48, 43, 0.3f);	// was (16,11): shares its roof spot by turns
	Site(TEXT("PplP3BBrokersSTP"), -1, 29.0f, 8, 3, 0.3f);
	Site(TEXT("PplP3BRoofDojoS2L"), 1, 29.0f, 48, 36, 0.3f);	// was (16,4): shares its roof spot by turns
	Site(TEXT("PplP3BNeonCrewS2L"), 1, 29.0f, 48, 44, 0.3f);	// was (16,12): shares its roof spot by turns
	Site(TEXT("PplP3BRoofHoopsS2R"), 1, 29.0f, 48, 36, 0.3f);	// was (16,4): shares its roof spot by turns
	Site(TEXT("PplP3BRoofBoxingS2R"), 1, 29.0f, 48, 44, 0.3f);	// was (16,12): shares its roof spot by turns
	Site(TEXT("PplP3BPigeonLoftYDB"), 1, 29.0f, 48, 0, 0.3f);	// was (16,0): shares its roof spot by turns
	Site(TEXT("PplP3BNeonCrewYDB"), 1, 29.0f, 48, 40, 0.3f);	// was (16,8): shares its roof spot by turns
#undef P3B
// ---- END p3b ----
}

{
// ---- BEGIN p3c (movers, ft_people_city_p3c.py) ----
// REVIEW AND FIX ROUND (16 Nov 2026, see SourceArt/Integration/people/L7_city_p3.md): p3b, p3c, p3d and p3f had each copied the podium rules without seeing the others, so two or three
// pieces stood in one roof spot (and the mayor inside the lantern bridge). Where rules of two blocks met on a side, in a slot and within 6.5 m along,
// 3 m out and 3 m up, the slots were shared out by turns (C:/ftwork/jobs/L7p3fix/resolve.py): such a rule now has a multiple of its podium's every
// (twice: two took turns; three times: three), sometimes as two or four lines. One roof spot, one piece. The lines changed say what they were.
// MarchMove_A and Mounted_A (the two TALL lane pieces) stay at 24.42 m. 24.9 m was tried for their fronts' sake (0.81 and 0.53 m before the line): the
// track's own sight test then pushed every MarchMove_A out (31 of 31, against 5 of 29 at 24.42 m), towards the shop fronts at 25.5 m. Put back.
// Everything that TRAVELS in the city: fourteen meshes, /Game/People/CityCenter/SM_P3C_*. Each piece carries its own line
// (the piece's X) and comes round again; no rule here turns a piece (yaw jitter 0), so a line runs along the road, or, in a
// side street's Set (yaw 90: the piece's +X then points away from the road), away from it. Nothing crosses the road.
// Written from FTDressL7.inl and ft_dress_metro.py; none of it has been looked at in the game.
//   THE DOOR LANE, 24.42 m out (Scatter, at the slot's anchor: the middle of a podium, 30 m of line inside its 32 m).
//     The first cast's commuters walk 23.5 to 24.1 m; the podiums' fronts are 25.5 m out, their awnings (3 m up) reach
//     24.44 m, the dear doors' tree tubs begin at 24.75 m, a bank's columns at 24.48 m. The walkers' bodies keep to 24.06
//     to 24.78 m; with arms, flags and elbows the pieces are 1.0 to 1.4 m deep as built (the log), Skaters_A 1.8 m (arms out).
//     Only in slots that HAVE a podium (the Plaza's open blocks are left alone), never before a bank (its columns) and
//     never before a garage (its ramp comes down to 24.5 m): in The Garages only the service yards' slots. Skaters_A and
//     MarchMove_A (the widest) only before shops, a picture house or a yard, not before the dear doors' trees.
//     LOW (the log: top 1.67 to 1.86 m as built): Runners_A/B, Skaters_A, Porters_A, School_A, DogWalk_A, Patrol_A/B.
//     TALL (the log: 2.29 and 2.01 m), unlit, 24.42 m is past 24 m: Mounted_A (the city's mounted police), MarchMove_A.
//     Patrol_A/B are the Chairman's men (the first cast's own gold and black) and walk only before his black walls
//     (W_Syn: left the even slots, right the odd) in Syndicate Territory; the police ride only before the finale.
//   SIDE STREETS (Site, the every-and-phase-and-side of the dressing's G_Street rule; if one changes there, change it here):
//     SideStreet_A/B stand 37 to 68 m out (the dressing's own traffic starts 37 m out for the tightest bend's sake) on
//     SM_FloorCross: walkers on its kerbs both ways, cyclists and a pedal cab riding away between the traffic and the
//     parked cars. TALL by the log (2.22 and 2.21 m: they stand on tarmac 28 cm up). Faint steady light (37 m out and more).
//   BANK ROOFS (Site, the every-and-phase-and-side of W_Banks): RoofRun_A/B, free-runners leaping along the roof's front
//     edge, 26.8 m out, 10.4 m up (SM_BankFront's roof: no first-cast piece stands there; 1.64 and 1.72 m as built, and
//     they leap). Faint steady light. Not in Syndicate Territory.
// Nothing here blinks. Nothing is nearer than 24.06 m. No Lit().
// Estimate: about 560 pieces by the rules' arithmetic (550 slots of 45 m), about 490 if the corkscrews, jumps and loops take
// an eighth; no mesh more than about 52 times (Porters_A 51, the two patrols and the two side-street pieces 50 each).
#define P3C(n) TEXT("/Game/People/CityCenter/SM_P3C_" n)
Set(TEXT("PplP3CSideA"), { { P3C("SideStreet_A"), 22.6f, 23.5f, 0.0f, 90.0f, 1.0f } });
Set(TEXT("PplP3CSideB"), { { P3C("SideStreet_B"), 22.6f, 23.5f, 0.0f, 90.0f, 1.0f } });
Set(TEXT("PplP3CRoofA"), { { P3C("RoofRun_A"), 0.0f, -2.2f, 10.4f, 0.0f, 1.0f } });
Set(TEXT("PplP3CRoofB"), { { P3C("RoofRun_B"), 0.0f, -2.2f, 10.4f, 0.0f, 1.0f } });

Zone(TEXT("People p3c: Fifth and Main"), 0.f, 1.10f);
// a podium in every slot; side streets: left the odd slots, right the even
	Scatter(P3C("Runners_A"), 24.42f, 24.42f, 4, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("School_A"), 24.42f, 24.42f, 4, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("DogWalk_A"), 24.42f, 24.42f, 4, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("Skaters_A"), 24.42f, 24.42f, 4, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplP3CSideA"), -1, 29.f, 2, 1, 1.f);
	Site(TEXT("PplP3CSideB"), 1, 29.f, 2, 0, 1.f);
Zone(TEXT("People p3c: Theatre Row"), 1.10f, 2.10f);
// a podium in every slot; side streets: left (3,1), right (3,2)
	Scatter(P3C("Skaters_A"), 24.42f, 24.42f, 4, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("MarchMove_A"), 24.42f, 24.42f, 4, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("Runners_B"), 24.42f, 24.42f, 4, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("Porters_A"), 24.42f, 24.42f, 4, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplP3CSideA"), -1, 29.f, 3, 1, 1.f);
	Site(TEXT("PplP3CSideB"), 1, 29.f, 3, 2, 1.f);
Zone(TEXT("People p3c: The Garages"), 2.10f, 3.13f);
// only before the service yards (left the odd slots, right the even): a garage's ramp comes down into the lane
	Scatter(P3C("Porters_A"), 24.42f, 24.42f, 4, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("Runners_A"), 24.42f, 24.42f, 4, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("DogWalk_A"), 24.42f, 24.42f, 4, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("Skaters_A"), 24.42f, 24.42f, 4, 2, 1.f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3c: The Plaza"), 3.13f, 4.62f);
// podiums: left the even slots, right the odd (the other blocks are open); side streets: left (2,0), right (2,1)
	Scatter(P3C("Mounted_A"), 24.42f, 24.42f, 4, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("School_A"), 24.42f, 24.42f, 4, 2, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("MarchMove_A"), 24.42f, 24.42f, 4, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("Runners_B"), 24.42f, 24.42f, 4, 3, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplP3CSideA"), -1, 29.f, 2, 0, 1.f);
	Site(TEXT("PplP3CSideB"), 1, 29.f, 2, 1, 1.f);
Zone(TEXT("People p3c: Wall Street"), 4.62f, 6.29f);
// left: banks (2,0), dear doors (4,1), shops (4,3); right: banks (2,1), dear doors (4,2), yards (4,0); side streets: left (4,1), right (4,3)
// the door lane only before the dear doors, the shops and the yards (never the banks' slots: left even, right odd)
	Scatter(P3C("Porters_A"), 24.42f, 24.42f, 4, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("Mounted_A"), 24.42f, 24.42f, 4, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("School_A"), 24.42f, 24.42f, 4, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("Mounted_A"), 24.42f, 24.42f, 4, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplP3CSideA"), -1, 29.f, 4, 1, 1.f);
	Site(TEXT("PplP3CSideB"), 1, 29.f, 4, 3, 1.f);
	Site(TEXT("PplP3CRoofA"), -1, 29.f, 4, 2, 1.f);	// was (2,0): shares its roof spot by turns
	Site(TEXT("PplP3CRoofA"), -1, 29.f, 12, 8, 1.f);
	Site(TEXT("PplP3CRoofB"), 1, 29.f, 8, 3, 1.f);	// was (2,1): shares its roof spot by turns
	Site(TEXT("PplP3CRoofB"), 1, 29.f, 8, 1, 1.f);
Zone(TEXT("People p3c: Penthouse Row"), 6.29f, 7.66f);
// left: dear doors (3,0) and (3,1), a bank (3,2); right: a picture house (3,0), dear doors (3,1) and (3,2); side streets: left (6,2), right (6,5)
	Scatter(P3C("Runners_A"), 24.42f, 24.42f, 3, 0, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("DogWalk_A"), 24.42f, 24.42f, 3, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("MarchMove_A"), 24.42f, 24.42f, 3, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("Runners_B"), 24.42f, 24.42f, 3, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("Porters_A"), 24.42f, 24.42f, 3, 2, 1.f, 1.f, 0.f, 1, 0.f);
	Site(TEXT("PplP3CSideA"), -1, 29.f, 6, 2, 1.f);
	Site(TEXT("PplP3CSideB"), 1, 29.f, 6, 5, 1.f);
	Site(TEXT("PplP3CRoofA"), -1, 29.f, 6, 2, 1.f);	// was (3,2): shares its roof spot by turns
	Site(TEXT("PplP3CRoofA"), -1, 29.f, 18, 11, 1.f);
Zone(TEXT("People p3c: Syndicate Territory, the arena"), 7.66f, 13.56f);
// left: black walls on the even slots, dear doors (4,1), a bank (8,3) and a garage (8,7), both left alone; right: black walls
// on the odd slots, dear doors (4,2), a yard (8,0), shops (8,4)
	Scatter(P3C("Patrol_A"), 24.42f, 24.42f, 4, 2, 0.75f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("Patrol_B"), 24.42f, 24.42f, 4, 1, 0.75f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("Porters_A"), 24.42f, 24.42f, 8, 1, 0.7f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("Runners_A"), 24.42f, 24.42f, 8, 5, 0.7f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("MarchMove_A"), 24.42f, 24.42f, 8, 0, 0.7f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("DogWalk_A"), 24.42f, 24.42f, 8, 2, 0.7f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("Runners_B"), 24.42f, 24.42f, 8, 6, 0.7f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("Skaters_A"), 24.42f, 24.42f, 8, 4, 0.7f, 1.f, 0.f, 1, 0.f);
Zone(TEXT("People p3c: Syndicate Territory, the circuit"), 13.56f, 99.f);
	Scatter(P3C("Patrol_A"), 24.42f, 24.42f, 4, 2, 0.4f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("Patrol_B"), 24.42f, 24.42f, 4, 1, 0.4f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("Porters_A"), 24.42f, 24.42f, 8, 1, 0.3f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("Runners_A"), 24.42f, 24.42f, 8, 5, 0.3f, 1.f, 0.f, -1, 0.f);
	Scatter(P3C("MarchMove_A"), 24.42f, 24.42f, 8, 0, 0.3f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("DogWalk_A"), 24.42f, 24.42f, 8, 2, 0.3f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("Runners_B"), 24.42f, 24.42f, 8, 6, 0.3f, 1.f, 0.f, 1, 0.f);
	Scatter(P3C("Skaters_A"), 24.42f, 24.42f, 8, 4, 0.3f, 1.f, 0.f, 1, 0.f);
#undef P3C
// ---- END p3c ----
}

{
// ---- BEGIN p3d (heights, ft_people_city_p3d.py) ----
// REVIEW AND FIX ROUND (16 Nov 2026, see SourceArt/Integration/people/L7_city_p3.md): p3b, p3c, p3d and p3f had each copied the podium rules without seeing the others, so two or three
// pieces stood in one roof spot (and the mayor inside the lantern bridge). Where rules of two blocks met on a side, in a slot and within 6.5 m along,
// 3 m out and 3 m up, the slots were shared out by turns (C:/ftwork/jobs/L7p3fix/resolve.py): such a rule now has a multiple of its podium's every
// (twice: two took turns; three times: three), sometimes as two or four lines. One roof spot, one piece. The lines changed say what they were.
// PEOPLE UP HIGH (pass 3, builder p3d). Twelve meshes, /Game/People/CityCenter/SM_P3D_*, every one TALL (its top 4.2 to
// 36.6 m as built), lit steadily (nothing blinks) and stood on the street wall or over a plaza, never on the ground:
// the avenue is a canyon, with a podium or a closed gap in every slot and a tower behind it, so nothing here is a Scatter.
// Every rule is a Site at the podiums' own anchor (29 m) whose set gives the place on the roof:
//   roof pieces keep to the roof's edge, 24.8 to 27.8 m from the centre line (the nearest thing of any of them, grown
//   bounds and all, is 24.2 m out). "up" is the roof: 8.88 m the TALL shops (SM_Shops1's middle shop, SM_Shops2's two
//   end shops, SM_Shops3's arcade), 11.82 m the picture house, 10.38 m the bank, 6.64 m the service yard (3.4 m further
//   out, like the main cast's own yard set), 9.6 m the dear doors' terrace, whose two ENDS are used (12.5 m either side of
//   its middle: the main cast's rally, the crew's crane and the watchers' roof crowd have its middle);
//   SM_P3D_Abseil_A stands at SM_Infill1 (G_Alley's own offsets: 33.9 m out; it brings two masts, so it also stands where the
//   alley's dice left the gap empty); SM_P3D_SkyWalk_A stands across a monument's plaza, 25.3 to 27.9 m out.
// A rule copies the side, every and phase of the dressing rule it stands on (FTDressL7.inl), or a multiple of its every:
// if a podium rule is changed there, change it here. Nothing of the main cast, the watchers or the crew is on these
// places (their rules were read: FTPeopleL7.inl). The dressing's blade signs (Scatter, 26 m, from 11.5 m up) stand
// before the middle of most podiums: the pieces put at a roof's middle (bleachers, the band) are lower than a blade's foot.
// Nothing on the Syndicate's black walls; no rule in the corkscrews, jumps and loops (the track leaves Sites out there).
// Estimate: about 521 pieces (Abseil_A 34, Band_A 47, Billboard_A 52, Bleacher_A 51, Bleacher_B 48, CrowsNest_A 50, HighWire_A 33, Lookout_A 51, Lookout_B 46, Lookout_C 48, SkyWalk_A 14, Ticker_A 48),
// counted as slots / every * chance with 24, 19, 20, 27, 30, 25, 131 and 122 slots for the eight districts below (the
// set-piece stretches taken off; the road taken as 19.1 km). Not counted in the game.
#define P3D(n) TEXT("/Game/People/CityCenter/SM_P3D_" n)
Set(TEXT("PplP3DAbsABS"), { { P3D("Abseil_A"), 22.6f, 4.9f, 0.0f, 0.f, 1.f } });
Set(TEXT("PplP3DBandMQb"), { { P3D("Band_A"), 4.3f, 0.0f, 11.82f, 0.f, 1.f } });
Set(TEXT("PplP3DBandS1M"), { { P3D("Band_A"), 0.6f, 0.0f, 8.88f, 0.f, 1.f } });
Set(TEXT("PplP3DBandS2L"), { { P3D("Band_A"), -10.0f, 0.0f, 8.88f, 0.f, 1.f } });
Set(TEXT("PplP3DBandS2R"), { { P3D("Band_A"), 11.0f, 0.0f, 8.88f, 0.f, 1.f } });
Set(TEXT("PplP3DBandS3A"), { { P3D("Band_A"), 2.3f, 0.0f, 8.88f, 0.f, 1.f } });
Set(TEXT("PplP3DBandYDb"), { { P3D("Band_A"), 9.0f, 3.4f, 6.64f, 0.f, 1.f } });
Set(TEXT("PplP3DBillMQa"), { { P3D("Billboard_A"), -7.0f, 0.0f, 11.82f, 0.f, 1.f } });
Set(TEXT("PplP3DBillS2L"), { { P3D("Billboard_A"), -10.0f, 0.0f, 8.88f, 0.f, 1.f } });
Set(TEXT("PplP3DBillYDa"), { { P3D("Billboard_A"), -7.0f, 3.4f, 6.64f, 0.f, 1.f } });
Set(TEXT("PplP3DBillYDc"), { { P3D("Billboard_A"), 10.5f, 3.4f, 6.64f, 0.f, 1.f } });
Set(TEXT("PplP3DBleaABK"), { { P3D("Bleacher_A"), -7.2f, 0.0f, 10.38f, 0.f, 1.f } });
Set(TEXT("PplP3DBleaAMQa"), { { P3D("Bleacher_A"), -7.0f, 0.0f, 11.82f, 0.f, 1.f } });
Set(TEXT("PplP3DBleaAMQb"), { { P3D("Bleacher_A"), 4.3f, 0.0f, 11.82f, 0.f, 1.f } });
Set(TEXT("PplP3DBleaAS1M"), { { P3D("Bleacher_A"), 0.6f, 0.0f, 8.88f, 0.f, 1.f } });
Set(TEXT("PplP3DBleaAS2R"), { { P3D("Bleacher_A"), 11.0f, 0.0f, 8.88f, 0.f, 1.f } });
Set(TEXT("PplP3DBleaAS3A"), { { P3D("Bleacher_A"), 2.3f, 0.0f, 8.88f, 0.f, 1.f } });
Set(TEXT("PplP3DBleaAYDa"), { { P3D("Bleacher_A"), -7.0f, 3.4f, 6.64f, 0.f, 1.f } });
Set(TEXT("PplP3DBleaAYDc"), { { P3D("Bleacher_A"), 10.5f, 3.4f, 6.64f, 0.f, 1.f } });
Set(TEXT("PplP3DBleaBMQb"), { { P3D("Bleacher_B"), 4.3f, 0.0f, 11.82f, 0.f, 1.f } });
Set(TEXT("PplP3DBleaBMQd"), { { P3D("Bleacher_B"), 5.5f, 0.0f, 11.82f, 0.f, 1.f } });
Set(TEXT("PplP3DBleaBS1M"), { { P3D("Bleacher_B"), 0.6f, 0.0f, 8.88f, 0.f, 1.f } });
Set(TEXT("PplP3DBleaBS2R"), { { P3D("Bleacher_B"), 11.0f, 0.0f, 8.88f, 0.f, 1.f } });
Set(TEXT("PplP3DBleaBS3A"), { { P3D("Bleacher_B"), 2.3f, 0.0f, 8.88f, 0.f, 1.f } });
Set(TEXT("PplP3DBleaBYDb"), { { P3D("Bleacher_B"), 9.0f, 3.4f, 6.64f, 0.f, 1.f } });
Set(TEXT("PplP3DBleaBYDc"), { { P3D("Bleacher_B"), 10.5f, 3.4f, 6.64f, 0.f, 1.f } });
Set(TEXT("PplP3DLookAMQc"), { { P3D("Lookout_A"), 6.6f, 0.0f, 11.82f, 0.f, 1.f } });
Set(TEXT("PplP3DLookAPEl"), { { P3D("Lookout_A"), -12.5f, 0.0f, 9.6f, 0.f, 1.f } });
Set(TEXT("PplP3DLookAPEr"), { { P3D("Lookout_A"), 12.4f, 0.0f, 9.6f, 0.f, 1.f } });
Set(TEXT("PplP3DLookAS2R"), { { P3D("Lookout_A"), 11.0f, 0.0f, 8.88f, 0.f, 1.f } });
Set(TEXT("PplP3DLookAYDc"), { { P3D("Lookout_A"), 10.5f, 3.4f, 6.64f, 0.f, 1.f } });
Set(TEXT("PplP3DLookBPEl"), { { P3D("Lookout_B"), -12.5f, 0.0f, 9.6f, 0.f, 1.f } });
Set(TEXT("PplP3DLookBPEr"), { { P3D("Lookout_B"), 12.4f, 0.0f, 9.6f, 0.f, 1.f } });
Set(TEXT("PplP3DLookBS2R"), { { P3D("Lookout_B"), 11.0f, 0.0f, 8.88f, 0.f, 1.f } });
Set(TEXT("PplP3DLookCMQc"), { { P3D("Lookout_C"), 6.6f, 0.0f, 11.82f, 0.f, 1.f } });
Set(TEXT("PplP3DLookCPEl"), { { P3D("Lookout_C"), -12.5f, 0.0f, 9.6f, 0.f, 1.f } });
Set(TEXT("PplP3DLookCPEr"), { { P3D("Lookout_C"), 12.4f, 0.0f, 9.6f, 0.f, 1.f } });
Set(TEXT("PplP3DNestPEl"), { { P3D("CrowsNest_A"), -12.5f, 0.0f, 9.6f, 0.f, 1.f } });
Set(TEXT("PplP3DNestPEr"), { { P3D("CrowsNest_A"), 12.4f, 0.0f, 9.6f, 0.f, 1.f } });
Set(TEXT("PplP3DSkySKY"), { { P3D("SkyWalk_A"), 22.6f, -2.4f, 0.0f, 0.f, 1.f } });
Set(TEXT("PplP3DTickBK"), { { P3D("Ticker_A"), -7.2f, 0.0f, 10.38f, 0.f, 1.f } });
Set(TEXT("PplP3DWireMQa"), { { P3D("HighWire_A"), -7.0f, 0.0f, 11.82f, 0.f, 1.f } });
Set(TEXT("PplP3DWireS2L"), { { P3D("HighWire_A"), -10.0f, 0.0f, 8.88f, 0.f, 1.f } });
Set(TEXT("PplP3DWireS2R"), { { P3D("HighWire_A"), 11.0f, 0.0f, 8.88f, 0.f, 1.f } });

Zone(TEXT("People p3d: Fifth and Main"), 0.000f, 1.10f);
// tall shop roofs (a blade sign stands over the middle of most podiums from 11.5 m up: only the bleachers and the band, which are lower, stand there). Left: W_Shops1L (3,0) its middle shop, W_Shops3 (3,1) its arcade, W_Shops2s (3,2) its two end shops.
// Right: W_Shops2s (3,0), W_Shops1 (3,1), W_Shops3 (3,2). The brick building behind the right-hand alleys the watchers left bare (4,1).
	Site(TEXT("PplP3DBleaBS1M"), -1, 29.f, 12, 6, 1.00f);	// was (6,0): shares its roof spot by turns
	Site(TEXT("PplP3DBandS1M"), -1, 29.f, 12, 3, 1.00f);	// was (6,3): shares its roof spot by turns
	Site(TEXT("PplP3DBleaAS3A"), -1, 29.f, 12, 1, 1.00f);	// was (6,1): shares its roof spot by turns
	Site(TEXT("PplP3DBandS3A"), -1, 29.f, 12, 4, 1.00f);	// was (6,4): shares its roof spot by turns
	Site(TEXT("PplP3DBillS2L"), -1, 29.f, 18, 8, 1.00f);	// was (6,2): shares its roof spot by turns
	Site(TEXT("PplP3DWireS2L"), -1, 29.f, 18, 11, 1.00f);	// was (6,5): shares its roof spot by turns
	Site(TEXT("PplP3DLookAS2R"), -1, 29.f, 12, 5, 1.00f);	// was (6,5): shares its roof spot by turns
	Site(TEXT("PplP3DWireS2L"), 1, 29.f, 18, 12, 1.00f);	// was (6,0): shares its roof spot by turns
	Site(TEXT("PplP3DBillS2L"), 1, 29.f, 12, 3, 1.00f);	// was (6,3): shares its roof spot by turns
	Site(TEXT("PplP3DLookBS2R"), 1, 29.f, 18, 9, 1.00f);	// was (6,3): shares its roof spot by turns
	Site(TEXT("PplP3DBleaAS1M"), 1, 29.f, 12, 1, 1.00f);	// was (6,1): shares its roof spot by turns
	Site(TEXT("PplP3DBandS1M"), 1, 29.f, 12, 4, 1.00f);	// was (6,4): shares its roof spot by turns
	Site(TEXT("PplP3DBleaBS3A"), 1, 29.f, 12, 2, 1.00f);	// was (6,2): shares its roof spot by turns
	Site(TEXT("PplP3DBleaAS3A"), 1, 29.f, 12, 5, 1.00f);	// was (6,5): shares its roof spot by turns
	Site(TEXT("PplP3DAbsABS"), 1, 29.f, 4, 1, 1.00f);
Zone(TEXT("People p3d: Theatre Row"), 1.100f, 2.10f);
// the picture houses without a roof party (W_Marquee left (4,2), right (4,3)) and, beside the party, the free end of the others
// (left (4,0), right (4,1)); W_Shops3 left (4,1), right (4,2); W_Shops2s left (4,3); W_Shops1 right (4,0)
	Site(TEXT("PplP3DWireMQa"), -1, 29.f, 12, 6, 1.00f);	// was (4,2): shares its roof spot by turns
	Site(TEXT("PplP3DBleaBMQb"), -1, 29.f, 8, 6, 1.00f);	// was (4,2): shares its roof spot by turns
	Site(TEXT("PplP3DLookAMQc"), -1, 29.f, 12, 8, 1.00f);	// was (4,0): shares its roof spot by turns
	Site(TEXT("PplP3DBandS3A"), -1, 29.f, 8, 1, 1.00f);	// was (4,1): shares its roof spot by turns
	Site(TEXT("PplP3DBillS2L"), -1, 29.f, 12, 7, 1.00f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3DLookBS2R"), -1, 29.f, 8, 3, 1.00f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3DBillMQa"), 1, 29.f, 12, 7, 1.00f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3DBleaAMQb"), 1, 29.f, 8, 7, 1.00f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3DLookCMQc"), 1, 29.f, 12, 5, 1.00f);	// was (4,1): shares its roof spot by turns
	Site(TEXT("PplP3DBleaBS3A"), 1, 29.f, 8, 2, 1.00f);	// was (4,2): shares its roof spot by turns
	Site(TEXT("PplP3DBandS1M"), 1, 29.f, 8, 4, 1.00f);	// was (4,0): shares its roof spot by turns
Zone(TEXT("People p3d: The Garages"), 2.100f, 3.13f);
// the service yards (W_Yards left (2,1), right (2,0)): the whole roof of those without a party (left (4,1), right (4,0)), the far end
// of the others; the brick building behind the alleys that have no stand and no scaffold (left (4,0) and (4,2), right (4,1) and (4,3))
	Site(TEXT("PplP3DBleaAYDa"), -1, 29.f, 8, 1, 1.00f);	// was (4,1): shares its roof spot by turns
	Site(TEXT("PplP3DBandYDb"), -1, 29.f, 12, 5, 1.00f);	// was (4,1): shares its roof spot by turns
	Site(TEXT("PplP3DBleaBYDc"), -1, 29.f, 12, 7, 1.00f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3DBillYDa"), 1, 29.f, 8, 4, 1.00f);	// was (4,0): shares its roof spot by turns
	Site(TEXT("PplP3DBleaBYDb"), 1, 29.f, 12, 8, 1.00f);	// was (4,0): shares its roof spot by turns
	Site(TEXT("PplP3DLookAYDc"), 1, 29.f, 12, 6, 1.00f);	// was (4,2): shares its roof spot by turns
	Site(TEXT("PplP3DAbsABS"), -1, 29.f, 4, 0, 1.00f);
	Site(TEXT("PplP3DAbsABS"), -1, 29.f, 4, 2, 1.00f);
	Site(TEXT("PplP3DAbsABS"), 1, 29.f, 4, 1, 1.00f);
	Site(TEXT("PplP3DAbsABS"), 1, 29.f, 4, 3, 1.00f);
Zone(TEXT("People p3d: The Plaza"), 3.130f, 4.62f);
// W_Shops1L left (4,0), W_Pent left (4,2) and right (4,3) (its two ends), W_Shops2s right (4,1); a lantern bridge over the plaza of
// every monument that has no rally before it (G_Monument left (4,3), right (4,2))
	Site(TEXT("PplP3DBleaAS1M"), -1, 29.f, 16, 8, 1.00f);	// was (8,0): shares its roof spot by turns
	Site(TEXT("PplP3DBandS1M"), -1, 29.f, 16, 4, 1.00f);	// was (8,4): shares its roof spot by turns
	Site(TEXT("PplP3DLookAPEl"), -1, 29.f, 4, 2, 1.00f);
	Site(TEXT("PplP3DNestPEr"), -1, 29.f, 8, 2, 1.00f);
	Site(TEXT("PplP3DWireS2L"), 1, 29.f, 24, 9, 1.00f);	// was (8,1): shares its roof spot by turns
	Site(TEXT("PplP3DBillS2L"), 1, 29.f, 24, 13, 1.00f);	// was (8,5): shares its roof spot by turns
	Site(TEXT("PplP3DBleaBS2R"), 1, 29.f, 12, 1, 1.00f);	// was (4,1): shares its roof spot by turns
	Site(TEXT("PplP3DLookBPEl"), 1, 29.f, 4, 3, 1.00f);
	Site(TEXT("PplP3DNestPEr"), 1, 29.f, 8, 7, 1.00f);
	Site(TEXT("PplP3DSkySKY"), -1, 29.f, 8, 3, 1.00f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3DSkySKY"), 1, 29.f, 8, 2, 1.00f);	// was (4,2): shares its roof spot by turns
Zone(TEXT("People p3d: Wall Street"), 4.620f, 6.29f);
// every bank (W_Banks left (2,0), right (2,1)); the ends of the dear doors (W_Pent left (4,1), right (4,2)); W_Shops2s left (4,3);
// the far end of the yards (W_Yards right (4,0)); the brick building behind the right-hand alleys (4,2)
	Site(TEXT("PplP3DTickBK"), -1, 29.f, 24, 16, 1.00f);	// was (2,0): shares its roof spot by turns
	Site(TEXT("PplP3DTickBK"), -1, 29.f, 24, 10, 1.00f);
	Site(TEXT("PplP3DTickBK"), -1, 29.f, 24, 8, 1.00f);
	Site(TEXT("PplP3DTickBK"), -1, 29.f, 24, 2, 1.00f);
	Site(TEXT("PplP3DTickBK"), 1, 29.f, 24, 15, 1.00f);	// was (2,1): shares its roof spot by turns
	Site(TEXT("PplP3DTickBK"), 1, 29.f, 24, 9, 1.00f);
	Site(TEXT("PplP3DTickBK"), 1, 29.f, 24, 7, 1.00f);
	Site(TEXT("PplP3DTickBK"), 1, 29.f, 24, 1, 1.00f);
	Site(TEXT("PplP3DLookCPEl"), -1, 29.f, 4, 1, 1.00f);
	Site(TEXT("PplP3DLookBPEr"), -1, 29.f, 8, 5, 1.00f);
	Site(TEXT("PplP3DNestPEl"), 1, 29.f, 4, 2, 1.00f);
	Site(TEXT("PplP3DLookAPEr"), 1, 29.f, 8, 2, 1.00f);
	Site(TEXT("PplP3DWireS2L"), -1, 29.f, 24, 15, 1.00f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3DWireS2L"), -1, 29.f, 24, 7, 1.00f);
	Site(TEXT("PplP3DBleaAS2R"), -1, 29.f, 16, 3, 1.00f);	// was (8,3): shares its roof spot by turns
	Site(TEXT("PplP3DBandS2R"), -1, 29.f, 16, 7, 1.00f);	// was (8,7): shares its roof spot by turns
	Site(TEXT("PplP3DBillYDc"), 1, 29.f, 12, 8, 1.00f);	// was (4,0): shares its roof spot by turns
	Site(TEXT("PplP3DAbsABS"), 1, 29.f, 4, 2, 1.00f);
Zone(TEXT("People p3d: Penthouse Row"), 6.290f, 7.66f);
// the ends of the dear doors (W_Pent left (3,0) and (3,1), right (3,1) and (3,2)), the bank (left (3,2)), the picture house (right (3,0):
// free (6,0), a party on (6,3))
	Site(TEXT("PplP3DLookAPEl"), -1, 29.f, 6, 0, 1.00f);
	Site(TEXT("PplP3DLookBPEr"), -1, 29.f, 6, 3, 1.00f);
	Site(TEXT("PplP3DLookBPEl"), -1, 29.f, 6, 1, 1.00f);
	Site(TEXT("PplP3DNestPEr"), -1, 29.f, 6, 4, 1.00f);
	Site(TEXT("PplP3DTickBK"), -1, 29.f, 9, 2, 1.00f);	// was (3,2): shares its roof spot by turns
	Site(TEXT("PplP3DLookCPEl"), 1, 29.f, 6, 1, 1.00f);
	Site(TEXT("PplP3DLookAPEr"), 1, 29.f, 6, 4, 1.00f);
	Site(TEXT("PplP3DNestPEl"), 1, 29.f, 6, 2, 1.00f);
	Site(TEXT("PplP3DLookCPEr"), 1, 29.f, 6, 5, 1.00f);
	Site(TEXT("PplP3DBleaAMQa"), 1, 29.f, 18, 12, 1.00f);	// was (6,0): shares its roof spot by turns
	Site(TEXT("PplP3DBandMQb"), 1, 29.f, 12, 0, 1.00f);	// was (6,0): shares its roof spot by turns
	Site(TEXT("PplP3DBleaBMQd"), 1, 29.f, 18, 9, 1.00f);	// was (6,3): shares its roof spot by turns
Zone(TEXT("People p3d: Syndicate Territory, the arena"), 7.660f, 13.56f);
// the roofs the city still holds: the ends of the dear doors (W_Pent left (4,1), right (4,2)), the bank (W_Bank left (8,3)),
// the far end of the yard (W_Yard right (8,0)), the end shops of the one shop row (W_Shops2 right (8,4)). Nothing on the black walls.
	Site(TEXT("PplP3DNestPEl"), -1, 29.f, 8, 1, 0.80f);
	Site(TEXT("PplP3DLookCPEr"), -1, 29.f, 16, 1, 0.80f);
	Site(TEXT("PplP3DLookCPEl"), -1, 29.f, 16, 5, 0.80f);
	Site(TEXT("PplP3DLookAPEl"), -1, 29.f, 16, 13, 0.80f);
	Site(TEXT("PplP3DLookBPEr"), -1, 29.f, 16, 9, 0.80f);
	Site(TEXT("PplP3DTickBK"), -1, 29.f, 48, 19, 0.80f);	// was (16,3): shares its roof spot by turns
	Site(TEXT("PplP3DBleaABK"), -1, 29.f, 48, 27, 0.80f);	// was (16,11): shares its roof spot by turns
	Site(TEXT("PplP3DLookBPEl"), 1, 29.f, 16, 2, 0.80f);
	Site(TEXT("PplP3DLookAPEl"), 1, 29.f, 16, 10, 0.80f);
	Site(TEXT("PplP3DNestPEr"), 1, 29.f, 16, 6, 0.80f);
	Site(TEXT("PplP3DLookCPEl"), 1, 29.f, 16, 14, 0.80f);
	Site(TEXT("PplP3DBleaAYDc"), 1, 29.f, 48, 32, 0.80f);	// was (16,0): shares its roof spot by turns
	Site(TEXT("PplP3DBillYDc"), 1, 29.f, 48, 24, 0.80f);	// was (16,8): shares its roof spot by turns
	Site(TEXT("PplP3DBandS2L"), 1, 29.f, 48, 20, 0.80f);	// was (16,4): shares its roof spot by turns
	Site(TEXT("PplP3DBillS2L"), 1, 29.f, 48, 28, 0.80f);	// was (16,12): shares its roof spot by turns
	Site(TEXT("PplP3DBleaBS2R"), 1, 29.f, 48, 20, 0.80f);	// was (16,4): shares its roof spot by turns
	Site(TEXT("PplP3DWireS2R"), 1, 29.f, 48, 28, 0.80f);	// was (16,12): shares its roof spot by turns
Zone(TEXT("People p3d: Syndicate Territory, the circuit"), 13.560f, 99.00f);
// the same, thinner
	Site(TEXT("PplP3DNestPEl"), -1, 29.f, 8, 1, 0.35f);
	Site(TEXT("PplP3DLookCPEr"), -1, 29.f, 16, 1, 0.35f);
	Site(TEXT("PplP3DLookCPEl"), -1, 29.f, 16, 5, 0.35f);
	Site(TEXT("PplP3DLookAPEl"), -1, 29.f, 16, 13, 0.35f);
	Site(TEXT("PplP3DLookBPEr"), -1, 29.f, 16, 9, 0.35f);
	Site(TEXT("PplP3DTickBK"), -1, 29.f, 48, 19, 0.35f);	// was (16,3): shares its roof spot by turns
	Site(TEXT("PplP3DBleaABK"), -1, 29.f, 48, 27, 0.35f);	// was (16,11): shares its roof spot by turns
	Site(TEXT("PplP3DLookBPEl"), 1, 29.f, 16, 2, 0.35f);
	Site(TEXT("PplP3DLookAPEl"), 1, 29.f, 16, 10, 0.35f);
	Site(TEXT("PplP3DNestPEr"), 1, 29.f, 16, 6, 0.35f);
	Site(TEXT("PplP3DLookCPEl"), 1, 29.f, 16, 14, 0.35f);
	Site(TEXT("PplP3DBleaAYDc"), 1, 29.f, 48, 32, 0.35f);	// was (16,0): shares its roof spot by turns
	Site(TEXT("PplP3DBillYDc"), 1, 29.f, 48, 24, 0.35f);	// was (16,8): shares its roof spot by turns
	Site(TEXT("PplP3DBandS2L"), 1, 29.f, 48, 20, 0.35f);	// was (16,4): shares its roof spot by turns
	Site(TEXT("PplP3DBillS2L"), 1, 29.f, 48, 28, 0.35f);	// was (16,12): shares its roof spot by turns
	Site(TEXT("PplP3DBleaBS2R"), 1, 29.f, 48, 20, 0.35f);	// was (16,4): shares its roof spot by turns
	Site(TEXT("PplP3DWireS2R"), 1, 29.f, 48, 28, 0.35f);	// was (16,12): shares its roof spot by turns
#undef P3D
// ---- END p3d ----
}

{
// ---- BEGIN p3e (far and sky, ft_people_city_p3e.py) ----
// Pass 3, builder e: life from 170 m out to the horizon and in the air over the far roofs, on BOTH sides, the whole road.
// Thirteen meshes, /Game/People/CityCenter/SM_P3E_*, all TALL and all saved with the always-moving master (far=True).
// Every rule is a Far rule (so the pieces stay through the corkscrews, jumps and loops, where the far city is seen best)
// and no rule's nearest distance is under 170 m: nothing of this block stands or flies nearer than 150 m to a centre line
// (the widest reach towards the road is a starling ring's, 43 m from its middle at 195 m).
//   STANDS (drop 0.7 m, as the first far kit's: the sidewalk's own depth). Each brings its own dark shaft with lit windows
//   and carries its people on the crown, 52 to 64 m up as built, 2 to 2.3 times that as stood: a wire-walker between two
//   towers (HighWire), kite flyers (KiteRoof), an open-air picture show (RoofCinema), climbers on ropes down a tower's
//   face (Abseil), a grandstand (FarStand), night-tour buses on a high road (Viaduct). Near band 170 to 210 m, in slots of
//   their own 180 m apart on each side; the two long ones (HighWire, Viaduct) behind them at 215 to 255 m.
//   SKY (a negative drop: the base hangs 95 to 105 m OVER the road's level, 23 m and more over the top of the highest
//   loop): paper lanterns (Lanterns_A/B), hot-air balloons lit from inside (Balloons_A/B), starlings in rings (Murmur_A)
//   and in a travelling ribbon (Murmur_B), a dragon kite (DragonKite). The ones that travel fly their own X, which the
//   table lays along the road where they hang (turned by 5 or 6 degrees at most), 100 m up and 185 m and more out.
//   Lanterns are left out of the Chairman's arena (7.66 to 13.56 km): no small lights drift in the sky his shots fly in.
// Steady light only; nothing blinks; nothing black and gold. The first far kit (100 to 150 m, and geese and gliders at
// 36 to 46 m) and the dressing's blimps (30 to 90 m) and helicopters (180 to 260 m, 110 to 120 m up) keep their places.
// Estimate (550 slots: 170 in the six districts, 131 in the arena, about 254 on the circuit): a stand mesh
// 2 x (10.6 + 4.1 + 4.0) = 37, six of them 224; a sky mesh 2 x (7.1 + 3.3 + 3.2) = 27 (lanterns 2 x (7.1 + 3.2) = 21),
// seven of them 177: about 400 pieces, no mesh more than 40 times.
#define P3E(n) TEXT("/Game/People/CityCenter/SM_P3E_" #n)
Zone(TEXT("People p3e far and sky: the six districts"), 0.f, 7.66f);
	Far(P3E(KiteRoof), 172.f, 200.f, 16, 0, 1.f, 2.3f, 0.06f, -1, 0.7f, 8.f);
	Far(P3E(RoofCinema), 180.f, 210.f, 16, 4, 1.f, 2.2f, 0.06f, -1, 0.7f, 8.f);
	Far(P3E(Abseil), 170.f, 198.f, 16, 8, 1.f, 2.3f, 0.06f, -1, 0.7f, 8.f);
	Far(P3E(FarStand), 176.f, 206.f, 16, 12, 1.f, 2.2f, 0.06f, -1, 0.7f, 8.f);
	Far(P3E(HighWire), 215.f, 250.f, 16, 2, 1.f, 2.2f, 0.05f, -1, 0.7f, 6.f);
	Far(P3E(Viaduct), 220.f, 255.f, 16, 10, 1.f, 2.f, 0.05f, -1, 0.7f, 5.f);
	Far(P3E(KiteRoof), 172.f, 200.f, 16, 6, 1.f, 2.3f, 0.06f, 1, 0.7f, 8.f);
	Far(P3E(RoofCinema), 180.f, 210.f, 16, 10, 1.f, 2.2f, 0.06f, 1, 0.7f, 8.f);
	Far(P3E(Abseil), 170.f, 198.f, 16, 14, 1.f, 2.3f, 0.06f, 1, 0.7f, 8.f);
	Far(P3E(FarStand), 176.f, 206.f, 16, 2, 1.f, 2.2f, 0.06f, 1, 0.7f, 8.f);
	Far(P3E(HighWire), 215.f, 250.f, 16, 12, 1.f, 2.2f, 0.05f, 1, 0.7f, 6.f);
	Far(P3E(Viaduct), 220.f, 255.f, 16, 4, 1.f, 2.f, 0.05f, 1, 0.7f, 5.f);
	Far(P3E(Murmur_A), 195.f, 240.f, 24, 1, 1.f, 1.6f, 0.1f, -1, -100.f, 180.f);
	Far(P3E(Lanterns_A), 190.f, 260.f, 24, 5, 1.f, 1.6f, 0.1f, -1, -95.f, 6.f);
	Far(P3E(Balloons_A), 215.f, 290.f, 24, 9, 1.f, 1.5f, 0.1f, -1, -100.f, 5.f);
	Far(P3E(Murmur_B), 185.f, 230.f, 24, 13, 1.f, 1.6f, 0.1f, -1, -100.f, 6.f);
	Far(P3E(Lanterns_B), 190.f, 260.f, 24, 17, 1.f, 1.6f, 0.1f, -1, -95.f, 6.f);
	Far(P3E(Balloons_B), 215.f, 290.f, 24, 21, 1.f, 1.5f, 0.1f, -1, -105.f, 5.f);
	Far(P3E(DragonKite), 180.f, 230.f, 24, 3, 1.f, 1.6f, 0.1f, -1, -95.f, 25.f);
	Far(P3E(Murmur_A), 195.f, 240.f, 24, 13, 1.f, 1.6f, 0.1f, 1, -100.f, 180.f);
	Far(P3E(Lanterns_A), 190.f, 260.f, 24, 17, 1.f, 1.6f, 0.1f, 1, -95.f, 6.f);
	Far(P3E(Balloons_A), 215.f, 290.f, 24, 21, 1.f, 1.5f, 0.1f, 1, -100.f, 5.f);
	Far(P3E(Murmur_B), 185.f, 230.f, 24, 1, 1.f, 1.6f, 0.1f, 1, -100.f, 6.f);
	Far(P3E(Lanterns_B), 190.f, 260.f, 24, 5, 1.f, 1.6f, 0.1f, 1, -95.f, 6.f);
	Far(P3E(Balloons_B), 215.f, 290.f, 24, 9, 1.f, 1.5f, 0.1f, 1, -105.f, 5.f);
	Far(P3E(DragonKite), 180.f, 230.f, 24, 15, 1.f, 1.6f, 0.1f, 1, -95.f, 25.f);
Zone(TEXT("People p3e far and sky: Syndicate Territory, the arena"), 7.66f, 13.56f);
	Far(P3E(KiteRoof), 172.f, 200.f, 32, 0, 1.f, 2.3f, 0.06f, -1, 0.7f, 8.f);
	Far(P3E(RoofCinema), 180.f, 210.f, 32, 8, 1.f, 2.2f, 0.06f, -1, 0.7f, 8.f);
	Far(P3E(Abseil), 170.f, 198.f, 32, 16, 1.f, 2.3f, 0.06f, -1, 0.7f, 8.f);
	Far(P3E(FarStand), 176.f, 206.f, 32, 24, 1.f, 2.2f, 0.06f, -1, 0.7f, 8.f);
	Far(P3E(HighWire), 215.f, 250.f, 32, 4, 1.f, 2.2f, 0.05f, -1, 0.7f, 6.f);
	Far(P3E(Viaduct), 220.f, 255.f, 32, 20, 1.f, 2.f, 0.05f, -1, 0.7f, 5.f);
	Far(P3E(KiteRoof), 172.f, 200.f, 32, 12, 1.f, 2.3f, 0.06f, 1, 0.7f, 8.f);
	Far(P3E(RoofCinema), 180.f, 210.f, 32, 20, 1.f, 2.2f, 0.06f, 1, 0.7f, 8.f);
	Far(P3E(Abseil), 170.f, 198.f, 32, 28, 1.f, 2.3f, 0.06f, 1, 0.7f, 8.f);
	Far(P3E(FarStand), 176.f, 206.f, 32, 4, 1.f, 2.2f, 0.06f, 1, 0.7f, 8.f);
	Far(P3E(HighWire), 215.f, 250.f, 32, 24, 1.f, 2.2f, 0.05f, 1, 0.7f, 6.f);
	Far(P3E(Viaduct), 220.f, 255.f, 32, 8, 1.f, 2.f, 0.05f, 1, 0.7f, 5.f);
	Far(P3E(Murmur_A), 195.f, 240.f, 40, 1, 1.f, 1.6f, 0.1f, -1, -100.f, 180.f);
	Far(P3E(Balloons_A), 215.f, 290.f, 40, 9, 1.f, 1.5f, 0.1f, -1, -100.f, 5.f);
	Far(P3E(Murmur_B), 185.f, 230.f, 40, 17, 1.f, 1.6f, 0.1f, -1, -100.f, 6.f);
	Far(P3E(Balloons_B), 215.f, 290.f, 40, 25, 1.f, 1.5f, 0.1f, -1, -105.f, 5.f);
	Far(P3E(DragonKite), 180.f, 230.f, 40, 33, 1.f, 1.6f, 0.1f, -1, -95.f, 25.f);
	Far(P3E(Murmur_A), 195.f, 240.f, 40, 21, 1.f, 1.6f, 0.1f, 1, -100.f, 180.f);
	Far(P3E(Balloons_A), 215.f, 290.f, 40, 29, 1.f, 1.5f, 0.1f, 1, -100.f, 5.f);
	Far(P3E(Murmur_B), 185.f, 230.f, 40, 37, 1.f, 1.6f, 0.1f, 1, -100.f, 6.f);
	Far(P3E(Balloons_B), 215.f, 290.f, 40, 5, 1.f, 1.5f, 0.1f, 1, -105.f, 5.f);
	Far(P3E(DragonKite), 180.f, 230.f, 40, 13, 1.f, 1.6f, 0.1f, 1, -95.f, 25.f);
Zone(TEXT("People p3e far and sky: Syndicate Territory, the circuit"), 13.56f, 99.f);
	Far(P3E(KiteRoof), 172.f, 200.f, 64, 0, 1.f, 2.3f, 0.06f, -1, 0.7f, 8.f);
	Far(P3E(RoofCinema), 180.f, 210.f, 64, 16, 1.f, 2.2f, 0.06f, -1, 0.7f, 8.f);
	Far(P3E(Abseil), 170.f, 198.f, 64, 32, 1.f, 2.3f, 0.06f, -1, 0.7f, 8.f);
	Far(P3E(FarStand), 176.f, 206.f, 64, 48, 1.f, 2.2f, 0.06f, -1, 0.7f, 8.f);
	Far(P3E(HighWire), 215.f, 250.f, 64, 8, 1.f, 2.2f, 0.05f, -1, 0.7f, 6.f);
	Far(P3E(Viaduct), 220.f, 255.f, 64, 40, 1.f, 2.f, 0.05f, -1, 0.7f, 5.f);
	Far(P3E(KiteRoof), 172.f, 200.f, 64, 24, 1.f, 2.3f, 0.06f, 1, 0.7f, 8.f);
	Far(P3E(RoofCinema), 180.f, 210.f, 64, 40, 1.f, 2.2f, 0.06f, 1, 0.7f, 8.f);
	Far(P3E(Abseil), 170.f, 198.f, 64, 56, 1.f, 2.3f, 0.06f, 1, 0.7f, 8.f);
	Far(P3E(FarStand), 176.f, 206.f, 64, 8, 1.f, 2.2f, 0.06f, 1, 0.7f, 8.f);
	Far(P3E(HighWire), 215.f, 250.f, 64, 48, 1.f, 2.2f, 0.05f, 1, 0.7f, 6.f);
	Far(P3E(Viaduct), 220.f, 255.f, 64, 16, 1.f, 2.f, 0.05f, 1, 0.7f, 5.f);
	Far(P3E(Murmur_A), 195.f, 240.f, 80, 1, 1.f, 1.6f, 0.1f, -1, -100.f, 180.f);
	Far(P3E(Lanterns_A), 190.f, 260.f, 80, 11, 1.f, 1.6f, 0.1f, -1, -95.f, 6.f);
	Far(P3E(Balloons_A), 215.f, 290.f, 80, 21, 1.f, 1.5f, 0.1f, -1, -100.f, 5.f);
	Far(P3E(Murmur_B), 185.f, 230.f, 80, 31, 1.f, 1.6f, 0.1f, -1, -100.f, 6.f);
	Far(P3E(Lanterns_B), 190.f, 260.f, 80, 41, 1.f, 1.6f, 0.1f, -1, -95.f, 6.f);
	Far(P3E(Balloons_B), 215.f, 290.f, 80, 51, 1.f, 1.5f, 0.1f, -1, -105.f, 5.f);
	Far(P3E(DragonKite), 180.f, 230.f, 80, 61, 1.f, 1.6f, 0.1f, -1, -95.f, 25.f);
	Far(P3E(Murmur_A), 195.f, 240.f, 80, 41, 1.f, 1.6f, 0.1f, 1, -100.f, 180.f);
	Far(P3E(Lanterns_A), 190.f, 260.f, 80, 51, 1.f, 1.6f, 0.1f, 1, -95.f, 6.f);
	Far(P3E(Balloons_A), 215.f, 290.f, 80, 61, 1.f, 1.5f, 0.1f, 1, -100.f, 5.f);
	Far(P3E(Murmur_B), 185.f, 230.f, 80, 71, 1.f, 1.6f, 0.1f, 1, -100.f, 6.f);
	Far(P3E(Lanterns_B), 190.f, 260.f, 80, 1, 1.f, 1.6f, 0.1f, 1, -95.f, 6.f);
	Far(P3E(Balloons_B), 215.f, 290.f, 80, 11, 1.f, 1.5f, 0.1f, 1, -105.f, 5.f);
	Far(P3E(DragonKite), 180.f, 230.f, 80, 21, 1.f, 1.6f, 0.1f, 1, -95.f, 25.f);
#undef P3E
// ---- END p3e ----
}

{
// ---- BEGIN p3f (film shoot, ft_people_city_p3f.py) ----
// REVIEW AND FIX ROUND (16 Nov 2026, see SourceArt/Integration/people/L7_city_p3.md): p3b, p3c, p3d and p3f had each copied the podium rules without seeing the others, so two or three
// pieces stood in one roof spot (and the mayor inside the lantern bridge). Where rules of two blocks met on a side, in a slot and within 6.5 m along,
// 3 m out and 3 m up, the slots were shared out by turns (C:/ftwork/jobs/L7p3fix/resolve.py): such a rule now has a multiple of its podium's every
// (twice: two took turns; three times: three), sometimes as two or four lines. One roof spot, one piece. The lines changed say what they were.
// MORE OF THE SHOOT: the unit has taken the roofs. Eleven meshes SM_P3F_* (second units, a jib, drone teams, a wire rig,
// the extras before a green screen and in their holding area, the make-up tent, the press pen, playback) and more of the
// crew kit in wardrobe City (Camera, ApplauseA/B, Village, Stunt, Lights, Sound, Catering, Crane). Written by
// C:/ftwork/jobs/L7p3f/make_inl.py.
// WHERE, AND WHY THERE: in this level the band 24 to 34 m is the street wall itself (a 32 m podium in every slot, its
// front 25.5 m out), the kerb pockets belong to the first pass and to the low pieces of others, and every gap holds an
// alley, a street, a monument or a checkpoint. So the shoot stands ON the podiums, like the first pass's roof parties:
// a one-piece set copies the side, every and phase of the dressing's podium rule (FTDressL7.inl: change one there,
// change it here) and its "up" is that roof. Every piece is drawn with the roof's edge at y = -350 and its tape at
// y = -335, so it stands along the FRONT edge, where it is seen from the street. The spots used are the ones the
// dressing's roof signs, the first pass (parties at a roof's middle, +10.67 on a W_Shops1, -4 on a W_Marquee, -7 on a
// yard, the dear doors' terraces) and the watchers leave free:
//   SM_Shops1  the low shop at either end (-10.67 / +10.67 m along, roof 5.48 m)     SM_Shops3  the pawn shop (+11.5, 5.88 m)
//   SM_Shops2  its two tall shops (-11.2 / +11.2, 8.88 m; a water tank stands behind: shallow kit pieces only)
//   SM_Marquee +8 (and -8 where the first pass has no party), 11.82 m     SM_GarageRamp  the top deck, -10.5 / 0 / +10.5, 12.37 m
//   SM_ServiceYard  +10 on the roof strip at the back (out 3.4), 6.64 m     SM_BankFront  -8 / 0 / +8, 10.36 m (nobody was up there)
// Not on the dear doors (full), not on the Syndicate's black walls, no drones in Syndicate Territory.
// RULES: every Site here is at 29 m (the podiums' own anchor): the nearest edge of any piece is the roof's edge, 25.5 m
// from the centre line and 5.5 m or more up. Steady light only (lamps, monitors, a green screen, a mirror's bulbs):
// nothing blinks. The drones hover or circle a mast over their own roof (26 m out and more). Inside the two loops only
// SM_P3F_LowUnit (1.88 m as built, unlit), on the left (the loop comes down on the right), 32 to 33 m out by a Far rule
// like the first pass's dolly. Nothing of ours in the corkscrews and at the jumps (the first pass fills both).
// ESTIMATE: about 371 placed (a rule every E with chance c over a district of n slots = n * c / E; the slots of the
// corkscrews, jumps and loops taken off; Syndicate Territory taken as 380 slots). By mesh: ApplauseB 32, PressPen 27, Playback 27, StuntRig 27, Extras_B 26, Camera 25, Extras_A 25, SecondUnit_B 23, ApplauseA 22, SecondUnit_A 20, Crane 20, MakeUp 20, Village 19, Lights 16, DroneTeam_B 14, DroneTeam_A 11, LowUnit 6, Sound 5, Stunt 5, Catering 3.
// NOT SEEN IN THE GAME, NOT COMPILED (pass 3: the integrator does both).
#define P3F(n) TEXT("/Game/People/CityCenter/SM_P3F_" n)
#define P3FCREW(n) TEXT("/Game/People/Crew/SM_Crew" n "_City")
Set(TEXT("PplP3FSecAS1n"), { { P3F("SecondUnit_A"), -10.67f, 0.0f, 5.48f, 0.0f, 1.0f } });
Set(TEXT("PplP3FPressS1n"), { { P3F("PressPen"), -10.67f, 0.0f, 5.48f, 0.0f, 1.0f } });
Set(TEXT("PplP3FPlayS1p"), { { P3F("Playback"), 10.67f, 0.0f, 5.48f, 0.0f, 1.0f } });
Set(TEXT("PplP3FAppBS1p"), { { P3FCREW("ApplauseB"), 10.67f, -2.65f, 5.48f, 0.0f, 1.0f } });
Set(TEXT("PplP3FDrAS3"), { { P3F("DroneTeam_A"), 11.5f, 0.0f, 5.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3FVilS3"), { { P3FCREW("Village"), 11.5f, -1.65f, 5.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3FCamS2n"), { { P3FCREW("Camera"), -11.2f, -1.85f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3FAppAS2n"), { { P3FCREW("ApplauseA"), -11.2f, -2.65f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3FCamS2p"), { { P3FCREW("Camera"), 11.2f, -1.85f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3FRigS1n"), { { P3F("StuntRig"), -10.67f, 0.0f, 5.48f, 0.0f, 1.0f } });
Set(TEXT("PplP3FSecBS1n"), { { P3F("SecondUnit_B"), -10.67f, 0.0f, 5.48f, 0.0f, 1.0f } });
Set(TEXT("PplP3FExAS1p"), { { P3F("Extras_A"), 10.67f, 0.0f, 5.48f, 0.0f, 1.0f } });
Set(TEXT("PplP3FExBS3"), { { P3F("Extras_B"), 11.5f, 0.0f, 5.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3FDrBS3"), { { P3F("DroneTeam_B"), 11.5f, 0.0f, 5.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3FSecAMQp"), { { P3F("SecondUnit_A"), 8.0f, 0.0f, 11.82f, 0.0f, 1.0f } });
Set(TEXT("PplP3FCraneMQp"), { { P3FCREW("Crane"), 8.0f, 1.5f, 11.82f, 0.0f, 1.0f } });
Set(TEXT("PplP3FExAMQn"), { { P3F("Extras_A"), -8.0f, 0.0f, 11.82f, 0.0f, 1.0f } });
Set(TEXT("PplP3FAppBS2n"), { { P3FCREW("ApplauseB"), -11.2f, -2.65f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3FPlayMQp"), { { P3F("Playback"), 8.0f, 0.0f, 11.82f, 0.0f, 1.0f } });
Set(TEXT("PplP3FRigMQp"), { { P3F("StuntRig"), 8.0f, 0.0f, 11.82f, 0.0f, 1.0f } });
Set(TEXT("PplP3FMkUpMQn"), { { P3F("MakeUp"), -8.0f, 0.0f, 11.82f, 0.0f, 1.0f } });
Set(TEXT("PplP3FPressS3"), { { P3F("PressPen"), 11.5f, 0.0f, 5.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3FCamS1n"), { { P3FCREW("Camera"), -10.67f, -1.85f, 5.48f, 0.0f, 1.0f } });
Set(TEXT("PplP3FDrAGAn"), { { P3F("DroneTeam_A"), -10.5f, 1.5f, 12.37f, 0.0f, 1.0f } });
Set(TEXT("PplP3FSecBGAn"), { { P3F("SecondUnit_B"), -10.5f, 1.5f, 12.37f, 0.0f, 1.0f } });
Set(TEXT("PplP3FLightsGAp"), { { P3FCREW("Lights"), 10.5f, -0.5f, 12.37f, 0.0f, 1.0f } });
Set(TEXT("PplP3FPressGAp"), { { P3F("PressPen"), 10.5f, 1.5f, 12.37f, 0.0f, 1.0f } });
Set(TEXT("PplP3FRigGA0"), { { P3F("StuntRig"), 0.0f, 1.5f, 12.37f, 0.0f, 1.0f } });
Set(TEXT("PplP3FExBYD"), { { P3F("Extras_B"), 10.0f, 3.4f, 6.64f, 0.0f, 1.0f } });
Set(TEXT("PplP3FAppBYD"), { { P3FCREW("ApplauseB"), 10.0f, 0.75f, 6.64f, 0.0f, 1.0f } });
Set(TEXT("PplP3FMkUpYD"), { { P3F("MakeUp"), 10.0f, 3.4f, 6.64f, 0.0f, 1.0f } });
Set(TEXT("PplP3FPlayYD"), { { P3F("Playback"), 10.0f, 3.4f, 6.64f, 0.0f, 1.0f } });
Set(TEXT("PplP3FExAGAn"), { { P3F("Extras_A"), -10.5f, 1.5f, 12.37f, 0.0f, 1.0f } });
Set(TEXT("PplP3FCraneGAn"), { { P3FCREW("Crane"), -10.5f, 3.0f, 12.37f, 0.0f, 1.0f } });
Set(TEXT("PplP3FSecAGAp"), { { P3F("SecondUnit_A"), 10.5f, 1.5f, 12.37f, 0.0f, 1.0f } });
Set(TEXT("PplP3FDrBGAp"), { { P3F("DroneTeam_B"), 10.5f, 1.5f, 12.37f, 0.0f, 1.0f } });
Set(TEXT("PplP3FCaterGA0"), { { P3FCREW("Catering"), 0.0f, -0.5f, 12.37f, 0.0f, 1.0f } });
Set(TEXT("PplP3FExAS1n"), { { P3F("Extras_A"), -10.67f, 0.0f, 5.48f, 0.0f, 1.0f } });
Set(TEXT("PplP3FVilS2p"), { { P3FCREW("Village"), 11.2f, -1.65f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3FAppBS2p"), { { P3FCREW("ApplauseB"), 11.2f, -2.65f, 8.88f, 0.0f, 1.0f } });
Set(TEXT("PplP3FPlayBKn"), { { P3F("Playback"), -8.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FDrABK0"), { { P3F("DroneTeam_A"), 0.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FAppABKp"), { { P3FCREW("ApplauseA"), 8.0f, -2.15f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FExBBKn"), { { P3F("Extras_B"), -8.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FCraneBK0"), { { P3FCREW("Crane"), 0.0f, 2.0f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FSoundBKp"), { { P3FCREW("Sound"), 8.0f, 0.2f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FPressBKn"), { { P3F("PressPen"), -8.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FLightsBK0"), { { P3FCREW("Lights"), 0.0f, -1.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FStuntBKp"), { { P3FCREW("Stunt"), 8.0f, -0.95f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FRigBKn"), { { P3F("StuntRig"), -8.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FMkUpBK0"), { { P3F("MakeUp"), 0.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FVilBKp"), { { P3FCREW("Village"), 8.0f, -1.15f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FSecBYD"), { { P3F("SecondUnit_B"), 10.0f, 3.4f, 6.64f, 0.0f, 1.0f } });
Set(TEXT("PplP3FExABKn"), { { P3F("Extras_A"), -8.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FAppBBKp"), { { P3FCREW("ApplauseB"), 8.0f, -2.15f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FSecBBKn"), { { P3F("SecondUnit_B"), -8.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FDrBBK0"), { { P3F("DroneTeam_B"), 0.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FPressMQp"), { { P3F("PressPen"), 8.0f, 0.0f, 11.82f, 0.0f, 1.0f } });
Set(TEXT("PplP3FRigMQn"), { { P3F("StuntRig"), -8.0f, 0.0f, 11.82f, 0.0f, 1.0f } });
Set(TEXT("PplP3FSecABKn"), { { P3F("SecondUnit_A"), -8.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FExBBKp"), { { P3F("Extras_B"), 8.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FRigBKp"), { { P3F("StuntRig"), 8.0f, 0.5f, 10.36f, 0.0f, 1.0f } });
Set(TEXT("PplP3FPlayGAp"), { { P3F("Playback"), 10.5f, 1.5f, 12.37f, 0.0f, 1.0f } });
Set(TEXT("PplP3FLightsGAn"), { { P3FCREW("Lights"), -10.5f, -0.5f, 12.37f, 0.0f, 1.0f } });
Set(TEXT("PplP3FSecBGAp"), { { P3F("SecondUnit_B"), 10.5f, 1.5f, 12.37f, 0.0f, 1.0f } });
Zone(TEXT("People p3f: Fifth and Main"), 0.0f, 1.1f);
// left: W_Shops1L (3,0), W_Shops3 (3,1), W_Shops2s (3,2); right: W_Shops2s (3,0), W_Shops1 (3,1), W_Shops3 (3,2)
	Site(TEXT("PplP3FSecAS1n"), -1, 29.0f, 6, 0, 0.7f);
	Site(TEXT("PplP3FPressS1n"), -1, 29.0f, 6, 3, 0.7f);
	Site(TEXT("PplP3FPlayS1p"), -1, 29.0f, 6, 0, 0.7f);
	Site(TEXT("PplP3FAppBS1p"), -1, 29.0f, 6, 3, 0.7f);
	Site(TEXT("PplP3FDrAS3"), -1, 29.0f, 6, 1, 0.7f);
	Site(TEXT("PplP3FVilS3"), -1, 29.0f, 6, 4, 0.7f);
	Site(TEXT("PplP3FCamS2n"), -1, 29.0f, 18, 2, 0.7f);	// was (6,2): shares its roof spot by turns
	Site(TEXT("PplP3FAppAS2n"), -1, 29.0f, 18, 5, 0.7f);	// was (6,5): shares its roof spot by turns
	Site(TEXT("PplP3FAppAS2n"), 1, 29.0f, 18, 6, 0.7f);	// was (6,0): shares its roof spot by turns
	Site(TEXT("PplP3FCamS2p"), 1, 29.0f, 18, 3, 0.7f);	// was (6,3): shares its roof spot by turns
	Site(TEXT("PplP3FRigS1n"), 1, 29.0f, 6, 1, 0.7f);
	Site(TEXT("PplP3FSecBS1n"), 1, 29.0f, 6, 4, 0.7f);
	Site(TEXT("PplP3FExAS1p"), 1, 29.0f, 6, 1, 0.7f);
	Site(TEXT("PplP3FExBS3"), 1, 29.0f, 6, 2, 0.7f);
	Site(TEXT("PplP3FDrBS3"), 1, 29.0f, 6, 5, 0.7f);
Zone(TEXT("People p3f: Theatre Row"), 1.1f, 2.1f);
// left: W_Marquee (2,0) with the first pass party on (4,0), W_Shops3 (4,1), W_Shops2s (4,3); right: W_Marquee (2,1), party on (4,1), W_Shops3 (4,2), W_Shops1 (4,0)
	Site(TEXT("PplP3FSecAMQp"), -1, 29.0f, 12, 4, 0.8f);	// was (4,0): shares its roof spot by turns
	Site(TEXT("PplP3FCraneMQp"), -1, 29.0f, 8, 2, 0.8f);	// was (4,2): shares its roof spot by turns
	Site(TEXT("PplP3FExAMQn"), -1, 29.0f, 12, 2, 0.8f);	// was (4,2): shares its roof spot by turns
	Site(TEXT("PplP3FDrBS3"), -1, 29.0f, 4, 1, 0.8f);
	Site(TEXT("PplP3FAppBS2n"), -1, 29.0f, 12, 3, 0.8f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3FPlayMQp"), 1, 29.0f, 12, 1, 0.8f);	// was (4,1): shares its roof spot by turns
	Site(TEXT("PplP3FRigMQp"), 1, 29.0f, 8, 3, 0.8f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3FMkUpMQn"), 1, 29.0f, 12, 3, 0.8f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3FPressS3"), 1, 29.0f, 4, 2, 0.8f);
	Site(TEXT("PplP3FCamS1n"), 1, 29.0f, 4, 0, 0.8f);
Zone(TEXT("People p3f: The Garages"), 2.1f, 3.13f);
// left: W_Garage (2,0), a party at its middle on (4,0), W_Yards (2,1); right: W_Yards (2,0), W_Garage (2,1), a party on (4,1)
	Site(TEXT("PplP3FDrAGAn"), -1, 29.0f, 4, 0, 0.7f);
	Site(TEXT("PplP3FSecBGAn"), -1, 29.0f, 4, 2, 0.7f);
	Site(TEXT("PplP3FLightsGAp"), -1, 29.0f, 4, 0, 0.7f);
	Site(TEXT("PplP3FPressGAp"), -1, 29.0f, 4, 2, 0.7f);
	Site(TEXT("PplP3FRigGA0"), -1, 29.0f, 4, 2, 0.7f);
	Site(TEXT("PplP3FExBYD"), -1, 29.0f, 12, 1, 0.7f);	// was (4,1): shares its roof spot by turns
	Site(TEXT("PplP3FAppBYD"), -1, 29.0f, 12, 3, 0.7f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3FMkUpYD"), 1, 29.0f, 12, 4, 0.7f);	// was (4,0): shares its roof spot by turns
	Site(TEXT("PplP3FPlayYD"), 1, 29.0f, 12, 2, 0.7f);	// was (4,2): shares its roof spot by turns
	Site(TEXT("PplP3FExAGAn"), 1, 29.0f, 4, 1, 0.7f);
	Site(TEXT("PplP3FCraneGAn"), 1, 29.0f, 4, 3, 0.7f);
	Site(TEXT("PplP3FSecAGAp"), 1, 29.0f, 4, 1, 0.7f);
	Site(TEXT("PplP3FDrBGAp"), 1, 29.0f, 4, 3, 0.7f);
	Site(TEXT("PplP3FCaterGA0"), 1, 29.0f, 4, 3, 0.7f);
Zone(TEXT("People p3f: The Plaza"), 3.13f, 4.62f);
// left: W_Shops1L (4,0); right: W_Shops2s (4,1). (The dear doors and their terraces are full: the first pass and the watchers.)
	Site(TEXT("PplP3FSecAS1n"), -1, 29.0f, 8, 0, 1.0f);
	Site(TEXT("PplP3FExAS1n"), -1, 29.0f, 8, 4, 1.0f);
	Site(TEXT("PplP3FAppAS2n"), 1, 29.0f, 24, 1, 1.0f);	// was (8,1): shares its roof spot by turns
	Site(TEXT("PplP3FCamS2n"), 1, 29.0f, 24, 5, 1.0f);	// was (8,5): shares its roof spot by turns
	Site(TEXT("PplP3FVilS2p"), 1, 29.0f, 24, 9, 1.0f);	// was (8,1): shares its roof spot by turns
	Site(TEXT("PplP3FAppBS2p"), 1, 29.0f, 24, 5, 1.0f);	// was (8,5): shares its roof spot by turns
Zone(TEXT("People p3f: Wall Street"), 4.62f, 6.29f);
// the roofs of the banks, which nobody stood on: left W_Banks (2,0), right (2,1); left W_Shops2s (4,3); right W_Yards (4,0)
	Site(TEXT("PplP3FPlayBKn"), -1, 29.0f, 24, 12, 0.7f);	// was (4,0): shares its roof spot by turns
	Site(TEXT("PplP3FPlayBKn"), -1, 29.0f, 24, 4, 0.7f);
	Site(TEXT("PplP3FDrABK0"), -1, 29.0f, 12, 0, 0.7f);	// was (4,0): shares its roof spot by turns
	Site(TEXT("PplP3FDrABK0"), -1, 29.0f, 12, 4, 0.7f);
	Site(TEXT("PplP3FAppABKp"), -1, 29.0f, 4, 0, 0.7f);
	Site(TEXT("PplP3FExBBKn"), -1, 29.0f, 24, 14, 0.7f);	// was (4,2): shares its roof spot by turns
	Site(TEXT("PplP3FExBBKn"), -1, 29.0f, 24, 6, 0.7f);
	Site(TEXT("PplP3FCraneBK0"), -1, 29.0f, 4, 2, 0.7f);
	Site(TEXT("PplP3FSoundBKp"), -1, 29.0f, 4, 2, 0.7f);
	Site(TEXT("PplP3FCamS2n"), -1, 29.0f, 24, 11, 0.7f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3FCamS2n"), -1, 29.0f, 24, 3, 0.7f);
	Site(TEXT("PplP3FPressBKn"), 1, 29.0f, 24, 13, 0.7f);	// was (4,1): shares its roof spot by turns
	Site(TEXT("PplP3FPressBKn"), 1, 29.0f, 24, 5, 0.7f);
	Site(TEXT("PplP3FLightsBK0"), 1, 29.0f, 8, 5, 0.7f);	// was (4,1): shares its roof spot by turns
	Site(TEXT("PplP3FStuntBKp"), 1, 29.0f, 4, 1, 0.7f);
	Site(TEXT("PplP3FRigBKn"), 1, 29.0f, 24, 11, 0.7f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3FRigBKn"), 1, 29.0f, 24, 3, 0.7f);
	Site(TEXT("PplP3FMkUpBK0"), 1, 29.0f, 8, 7, 0.7f);	// was (4,3): shares its roof spot by turns
	Site(TEXT("PplP3FVilBKp"), 1, 29.0f, 4, 3, 0.7f);
	Site(TEXT("PplP3FSecBYD"), 1, 29.0f, 12, 4, 0.7f);	// was (4,0): shares its roof spot by turns
Zone(TEXT("People p3f: Penthouse Row"), 6.29f, 7.66f);
// left: W_Banks (3,2); right: W_Marquee (3,0), the first pass party on (6,3)
	Site(TEXT("PplP3FExABKn"), -1, 29.0f, 18, 8, 1.0f);	// was (6,2): shares its roof spot by turns
	Site(TEXT("PplP3FAppBBKp"), -1, 29.0f, 6, 2, 1.0f);
	Site(TEXT("PplP3FSecBBKn"), -1, 29.0f, 18, 5, 1.0f);	// was (6,5): shares its roof spot by turns
	Site(TEXT("PplP3FDrBBK0"), -1, 29.0f, 18, 17, 1.0f);	// was (6,5): shares its roof spot by turns
	Site(TEXT("PplP3FDrBBK0"), -1, 29.0f, 18, 5, 1.0f);
	Site(TEXT("PplP3FPressMQp"), 1, 29.0f, 12, 6, 1.0f);	// was (6,0): shares its roof spot by turns
	Site(TEXT("PplP3FRigMQn"), 1, 29.0f, 18, 6, 1.0f);	// was (6,0): shares its roof spot by turns
	Site(TEXT("PplP3FPlayMQp"), 1, 29.0f, 18, 3, 1.0f);	// was (6,3): shares its roof spot by turns
Zone(TEXT("People p3f: Syndicate Territory, the arena"), 7.66f, 13.56f);
// left: W_Bank (8,3), W_Garage (8,7) with the first pass party at its middle; right: W_Yard (8,0), W_Shops2 (8,4)
	Site(TEXT("PplP3FSecABKn"), -1, 29.0f, 48, 3, 0.6f);	// was (16,3): shares its roof spot by turns
	Site(TEXT("PplP3FMkUpBK0"), -1, 29.0f, 16, 3, 0.6f);
	Site(TEXT("PplP3FExBBKp"), -1, 29.0f, 16, 3, 0.6f);
	Site(TEXT("PplP3FPressBKn"), -1, 29.0f, 48, 11, 0.6f);	// was (16,11): shares its roof spot by turns
	Site(TEXT("PplP3FCraneBK0"), -1, 29.0f, 16, 11, 0.6f);
	Site(TEXT("PplP3FRigBKp"), -1, 29.0f, 16, 11, 0.6f);
	Site(TEXT("PplP3FExAGAn"), -1, 29.0f, 16, 7, 0.6f);
	Site(TEXT("PplP3FPlayGAp"), -1, 29.0f, 16, 7, 0.6f);
	Site(TEXT("PplP3FLightsGAn"), -1, 29.0f, 16, 15, 0.6f);
	Site(TEXT("PplP3FSecBGAp"), -1, 29.0f, 16, 15, 0.6f);
	Site(TEXT("PplP3FAppBYD"), 1, 29.0f, 48, 16, 0.6f);	// was (16,0): shares its roof spot by turns
	Site(TEXT("PplP3FExBYD"), 1, 29.0f, 48, 8, 0.6f);	// was (16,8): shares its roof spot by turns
	Site(TEXT("PplP3FCamS2n"), 1, 29.0f, 48, 4, 0.6f);	// was (16,4): shares its roof spot by turns
	Site(TEXT("PplP3FVilS2p"), 1, 29.0f, 48, 4, 0.6f);	// was (16,4): shares its roof spot by turns
	Site(TEXT("PplP3FAppAS2n"), 1, 29.0f, 48, 12, 0.6f);	// was (16,12): shares its roof spot by turns
	Site(TEXT("PplP3FAppBS2p"), 1, 29.0f, 48, 12, 0.6f);	// was (16,12): shares its roof spot by turns
Zone(TEXT("People p3f: Syndicate Territory, the circuit"), 13.56f, 99.0f);
// left: W_Bank (8,3), W_Garage (8,7) with the first pass party at its middle; right: W_Yard (8,0), W_Shops2 (8,4)
	Site(TEXT("PplP3FSecABKn"), -1, 29.0f, 48, 3, 0.15f);	// was (16,3): shares its roof spot by turns
	Site(TEXT("PplP3FMkUpBK0"), -1, 29.0f, 16, 3, 0.15f);
	Site(TEXT("PplP3FExBBKp"), -1, 29.0f, 16, 3, 0.15f);
	Site(TEXT("PplP3FPressBKn"), -1, 29.0f, 48, 11, 0.15f);	// was (16,11): shares its roof spot by turns
	Site(TEXT("PplP3FCraneBK0"), -1, 29.0f, 16, 11, 0.15f);
	Site(TEXT("PplP3FRigBKp"), -1, 29.0f, 16, 11, 0.15f);
	Site(TEXT("PplP3FExAGAn"), -1, 29.0f, 16, 7, 0.15f);
	Site(TEXT("PplP3FPlayGAp"), -1, 29.0f, 16, 7, 0.15f);
	Site(TEXT("PplP3FLightsGAn"), -1, 29.0f, 16, 15, 0.15f);
	Site(TEXT("PplP3FSecBGAp"), -1, 29.0f, 16, 15, 0.15f);
	Site(TEXT("PplP3FAppBYD"), 1, 29.0f, 48, 16, 0.15f);	// was (16,0): shares its roof spot by turns
	Site(TEXT("PplP3FExBYD"), 1, 29.0f, 48, 8, 0.15f);	// was (16,8): shares its roof spot by turns
	Site(TEXT("PplP3FCamS2n"), 1, 29.0f, 48, 4, 0.15f);	// was (16,4): shares its roof spot by turns
	Site(TEXT("PplP3FVilS2p"), 1, 29.0f, 48, 4, 0.15f);	// was (16,4): shares its roof spot by turns
	Site(TEXT("PplP3FAppAS2n"), 1, 29.0f, 48, 12, 0.15f);	// was (16,12): shares its roof spot by turns
	Site(TEXT("PplP3FAppBS2p"), 1, 29.0f, 48, 12, 0.15f);	// was (16,12): shares its roof spot by turns
Zone(TEXT("People p3f: Plaza loop"), 3.208f, 3.488f);
	Far(P3F("LowUnit"), 32.0f, 33.0f, 2, 0, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
Zone(TEXT("People p3f: Penthouse loop"), 6.737f, 6.972f);
	Far(P3F("LowUnit"), 32.0f, 33.0f, 2, 0, 1.0f, 1.0f, 0.0f, -1, 0.6f, 0.0f);
#undef P3F
#undef P3FCREW
// ---- END p3f ----
}
