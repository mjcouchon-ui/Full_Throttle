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
{
// ---- BEGIN s1d1 (district 1 "Fifth and Main", ft_world_city_s1d1.py) ----
// 18 meshes (7 buildings, 6 tall props, 4 low props, 1 hero), all /Game/World/CityCenter/SM_S1D1_*. Only even slots
// (every 4, 8 or 12; phase % 4 == 0 or 2), only 0 to 1.10 km. Nothing blinks; light is steady and only on TALL pieces.
// Estimate (slots = 22.1 x km; x 0.75 for what the track leaves out; x parts in the set):
//   blocks       4 Sites, every 4, 0 to 1.10 km (24.3 slots):  4 x 24.3/4 x 0.75 x 4 parts            = 72.9
//   alleys A     1 Site,  every 4, 0 to 0.85 km (18.8 slots):  18.8/4 x 0.75 x 4 parts                = 14.1
//   alleys B1+B2 2 Sites, every 8:                             2 x 18.8/8 x 0.75 x 6 parts            = 21.2
//   corners      2 Sites, every 4:                             2 x 18.8/4 x 0.75 x 2 parts            = 14.1
//   kerb, low    12 Sites, every 12:                           18.8/12 x 0.75 x (4x4 + 4x4 + 4x3)     = 51.7
//   hero         2 one-slot Sites:                             2 x 0.75                               =  1.5
//   sum about 175 placed (if the track leaves nothing out: 250). Most of one mesh: WalkUp about 22 (31 at the most).
// Bands and slots used, and what stands there already (read from FTDressL7.inl and FTPeopleL7.inl, not seen in the game):
//   60 to 66 m  BUILDINGS as blocks of four (anchor 62 m), both sides, every even slot: crowns 63 to 85 m tall behind
//               the first row of towers (the second row of towers stands 51 to 82 m out: not checked what hides what)
//   41 m        a BUILDING at 0.66 to 0.72 of its size behind each LEFT alley (G_Alley: left, the even slots), in the 13 m
//               of air between two towers, over the roof of SM_Infill1 (34 m)
//   25.8 to 29.9 m  TALL PROPS in and over the LEFT alleys: a pennant-and-washing line whose poles stand inside the
//               side walls of the alley (lines 10.6 to 13.9 m up; the deck of SM_ElOver is 24 m up), picture signs on masts
//               inside those walls (their show 12 to 16.5 m up), and in the alleys of slots s % 4 == 2 (no SM_WatchSteps
//               there) a water tank on legs, a shoe-shine stand and the clock on a barber pole
//   29.4 m      TALL PROPS on the RIGHT, on the pale kerbs at the mouths of the side streets (G_Street: right, the even
//               slots), 0.56 m off the end wall of each podium: the roadway itself (11 m) is left clear
//   21.0 m      LOW PROPS behind the parked cars of the kerb (anchor 17.6 m like the kerb sets; 20.2 to 22.0 m out), by
//               every-12 rules that pick the kerb set of the slot: K1 = Kerb1 (cars at -10 m along), K2 = Kerb2 (cars at
//               +8), KC = KerbCafe (cars at +12). Left: s % 12 = 0, 6 Kerb1; 2, 8 KerbCafe; 4, 10 Kerb2. Right: 0, 6 Kerb2;
//               2, 8 Kerb1; 4, 10 KerbCafe. If a kerb rule is changed in FTDressL7.inl (every, phase, side), change these.
//   140 m       THE PERCOLATOR at 1.25 (140 m tall): left at slot 20 (0.905 km, outside the first hairpin), right at slot 8
// Near things (under 40 m) stop at 0.85 km: the two hairpins after it are banked and near pieces float over the cutting.
#define S1D1(n) TEXT("/Game/World/CityCenter/SM_S1D1_" n)
Set(TEXT("WldS1D1BlockA"), { { S1D1("CornerStore"), -16.5f, -2.f, 0.f, 0.f, 1.f }, { S1D1("WalkUp"), -5.5f, 3.f, 0.f, 0.f, 1.f },
	{ S1D1("Bakery"), 5.5f, 0.f, 0.f, 0.f, 1.f }, { S1D1("HatHouse"), 16.5f, 4.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1BlockB"), { { S1D1("GableHouse"), -16.5f, 0.f, 0.f, 0.f, 1.f }, { S1D1("ToyEmporium"), -5.5f, -2.f, 0.f, 0.f, 1.f },
	{ S1D1("ArcadeHall"), 5.5f, 4.f, 0.f, 0.f, 1.f }, { S1D1("WalkUp"), 16.5f, 1.f, 0.f, 0.f, 0.92f } });
// fix round: blocks of three. The fourth house stood across the far end of a side street (left: the street at -22.6 along; right: at +22.6)
Set(TEXT("WldS1D1BlockAL"), { { S1D1("WalkUp"), -5.5f, 3.f, 0.f, 0.f, 1.f }, { S1D1("Bakery"), 5.5f, 0.f, 0.f, 0.f, 1.f }, { S1D1("HatHouse"), 16.5f, 4.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1BlockBL"), { { S1D1("ToyEmporium"), -5.5f, -2.f, 0.f, 0.f, 1.f }, { S1D1("ArcadeHall"), 5.5f, 4.f, 0.f, 0.f, 1.f }, { S1D1("WalkUp"), 16.5f, 1.f, 0.f, 0.f, 0.92f } });
Set(TEXT("WldS1D1BlockAR"), { { S1D1("CornerStore"), -16.5f, -2.f, 0.f, 0.f, 1.f }, { S1D1("WalkUp"), -5.5f, 3.f, 0.f, 0.f, 1.f }, { S1D1("Bakery"), 5.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1BlockBR"), { { S1D1("GableHouse"), -16.5f, 0.f, 0.f, 0.f, 1.f }, { S1D1("ToyEmporium"), -5.5f, -2.f, 0.f, 0.f, 1.f }, { S1D1("ArcadeHall"), 5.5f, 4.f, 0.f, 0.f, 1.f } });
// the left alleys (anchor 29 m, the same as the podiums; the middle of the alley is 22.6 m along)
Set(TEXT("WldS1D1AlleyA"), { { S1D1("Bunting"), 22.6f, -3.2f, 0.f, 0.f, 1.f }, { S1D1("CupSign"), 16.6f, -1.f, 0.f, 0.f, 1.f },
	{ S1D1("HatSign"), 28.6f, -1.f, 0.f, 0.f, 1.f }, { S1D1("WalkUp"), 22.6f, 12.f, 0.f, 0.f, 0.7f } });
Set(TEXT("WldS1D1AlleyB1"), { { S1D1("Bunting"), 22.6f, -3.2f, 0.f, 0.f, 1.f }, { S1D1("WaterTank"), 22.6f, 0.9f, 0.f, 0.f, 1.f },
	{ S1D1("ShoeShine"), 20.3f, -2.6f, 0.f, 0.f, 1.f }, { S1D1("BarberClock"), 25.2f, -2.9f, 0.f, 0.f, 1.f },
	{ S1D1("HatSign"), 16.6f, -1.f, 0.f, 0.f, 1.f }, { S1D1("GableHouse"), 22.6f, 12.f, 0.f, 0.f, 0.66f } });
Set(TEXT("WldS1D1AlleyB2"), { { S1D1("Bunting"), 22.6f, -3.2f, 0.f, 0.f, 1.f }, { S1D1("WaterTank"), 22.6f, 0.9f, 0.f, 0.f, 1.f },
	{ S1D1("ShoeShine"), 20.3f, -2.6f, 0.f, 0.f, 1.f }, { S1D1("BarberClock"), 25.2f, -2.9f, 0.f, 0.f, 1.f },
	{ S1D1("CupSign"), 28.6f, -1.f, 0.f, 0.f, 1.f }, { S1D1("Bakery"), 22.6f, 12.f, 0.f, 0.f, 0.72f } });
// the corners of the right side streets (anchor 29 m; the pale kerbs of the street are 16.3 to 17.1 and 28.1 to 28.9 m along)
Set(TEXT("WldS1D1CornerA"), { { S1D1("BarberClock"), 16.56f, 0.4f, 0.f, 0.f, 1.f }, { S1D1("CupSign"), 28.68f, 0.4f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1CornerB"), { { S1D1("HatSign"), 16.56f, 0.4f, 0.f, 0.f, 1.f }, { S1D1("BarberClock"), 28.68f, 0.4f, 0.f, 0.f, 1.f } });
// low props behind the parked cars (anchor 17.6 m)
Set(TEXT("WldS1D1KerbK1"), { { S1D1("Tricycles"), -16.2f, 3.4f, 0.f, 0.f, 1.f }, { S1D1("FruitStall"), -11.8f, 3.4f, 0.f, 0.f, 1.f },
	{ S1D1("Boards"), -7.6f, 3.4f, 0.f, 0.f, 1.f }, { S1D1("HatboxCart"), -3.9f, 3.4f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1KerbK2"), { { S1D1("Tricycles"), 1.8f, 3.4f, 0.f, 0.f, 1.f }, { S1D1("FruitStall"), 6.2f, 3.4f, 0.f, 0.f, 1.f },
	{ S1D1("Boards"), 10.4f, 3.4f, 0.f, 0.f, 1.f }, { S1D1("HatboxCart"), 14.1f, 3.4f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1KerbKC"), { { S1D1("Tricycles"), 5.4f, 3.4f, 0.f, 0.f, 1.f }, { S1D1("FruitStall"), 9.8f, 3.4f, 0.f, 0.f, 1.f },
	{ S1D1("Boards"), 13.9f, 3.4f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D1Percolator"), { { S1D1("Percolator"), 0.f, 0.f, 0.f, 0.f, 1.25f } });
Zone(TEXT("World s1d1, Fifth and Main"), 0.f, 1.1f);
	Site(TEXT("WldS1D1BlockAL"), -1, 62.f, 4, 0, 1.f);
	Site(TEXT("WldS1D1BlockBL"), -1, 62.f, 4, 2, 1.f);
	Site(TEXT("WldS1D1BlockBR"), 1, 62.f, 4, 0, 1.f);
	Site(TEXT("WldS1D1BlockAR"), 1, 62.f, 4, 2, 1.f);
Zone(TEXT("World s1d1, Fifth and Main, the street"), 0.f, 0.85f);
	Site(TEXT("WldS1D1AlleyA"), -1, 29.f, 4, 0, 1.f);
	Site(TEXT("WldS1D1AlleyB1"), -1, 29.f, 8, 2, 1.f);
	Site(TEXT("WldS1D1AlleyB2"), -1, 29.f, 8, 6, 1.f);
	Site(TEXT("WldS1D1CornerA"), 1, 29.f, 4, 0, 1.f);
	Site(TEXT("WldS1D1CornerB"), 1, 29.f, 4, 2, 1.f);
	Site(TEXT("WldS1D1KerbK1"), -1, 17.6f, 12, 0, 1.f);
	Site(TEXT("WldS1D1KerbK1"), -1, 17.6f, 12, 6, 1.f);
	Site(TEXT("WldS1D1KerbKC"), -1, 17.6f, 12, 10, 1.f);
	Site(TEXT("WldS1D1KerbKC"), -1, 17.6f, 12, 4, 1.f);
	Site(TEXT("WldS1D1KerbK2"), -1, 17.6f, 12, 8, 1.f);
	Site(TEXT("WldS1D1KerbK2"), -1, 17.6f, 12, 2, 1.f);
	Site(TEXT("WldS1D1KerbK2"), 1, 17.6f, 12, 0, 1.f);
	Site(TEXT("WldS1D1KerbK2"), 1, 17.6f, 12, 6, 1.f);
	Site(TEXT("WldS1D1KerbK1"), 1, 17.6f, 12, 10, 1.f);
	Site(TEXT("WldS1D1KerbK1"), 1, 17.6f, 12, 4, 1.f);
	Site(TEXT("WldS1D1KerbKC"), 1, 17.6f, 12, 8, 1.f);
	Site(TEXT("WldS1D1KerbKC"), 1, 17.6f, 12, 2, 1.f);
Zone(TEXT("World s1d1, the Percolator"), 0.9018f, 0.9078f);
	Site(TEXT("WldS1D1Percolator"), -1, 140.f, 4, 0, 1.f);
Zone(TEXT("World s1d1, the Percolator, second"), 0.3589f, 0.3649f);
	Site(TEXT("WldS1D1Percolator"), 1, 140.f, 4, 0, 1.f);
#undef S1D1
// ---- END s1d1 ----
}
{
// ---- BEGIN s1d2 (district 2 "Theatre Row", ft_world_city_s1d2.py) ----
// 16 meshes (6 buildings, 3 tall props, 1 roof prop, 5 low props, 1 hero), all /Game/World/CityCenter/SM_S1D2_*. Only even
// slots (every 4, 12 or 24; phase % 4 == 0 or 2), only 1.10 to 2.10 km (slots 25 to 46: the even ones are 26 ... 46).
// Nothing blinks; light is steady and only on TALL pieces (the nearest 28.6 m out, on a roof).
// Estimate, counted by the slots each rule really takes in 25..46 (Site x 0.75 for what the track leaves out, Far x 1):
//   gap columns   14 Sites, 25 rule slots x 5 parts = 125  x 0.75 = 93.75
//   kerb, low      7 Sites, 13 rule slots: 9 x 5 parts + 4 x 4 parts = 61  x 0.75 = 45.75
//   roof           4 Sites, 7 rule slots x 1 part = 7  x 0.75 = 5.25
//   hero           1 Site, 1 slot x 0.75 = 0.75
//   corkscrew      4 Far rules, 1 slot x 2 sides each = 8
//   sum about 153 placed (202 if the track leaves nothing out). By the brief's 22.1 slots a km instead: about 148.
//   Most of one mesh: CostumeHouse and FlyTower 21 each (rule slots, nothing left out). Hero: 1.
//   NOT CHECKED, and likely fewer: the road runs through tower blocks at 1.17 to 1.36 and 1.85 to 2.10 km, where the track
//   leaves out everything nearer than 35.7 m (the kerb and roof rules of slots 26, 28, 30, 42, 44, 46), and slots 32, 34 are
//   in the corkscrew (no Scatter or Site at all: the four Far rules stand there instead). The gap columns (37.6 m and more
//   out) stay in the tower blocks.
// Bands and slots used, and what stands there already (read from FTDressL7.inl and FTPeopleL7.inl, not seen in the game):
//   37.6 to 69.9 m  GAP COLUMNS (anchor 29 m, the podiums' own): in the 13 m between two towers, 22.6 m along from the
//               slot's anchor, behind the 34 m stage flat that closes the gap (SM_Infill2 26.8 m out, or SM_Alley with
//               SM_Infill1 33.9 m out). Front to back: a mast 37.6 m out (TALL PROP, 51 to 55 m), then four BUILDINGS
//               44 to 69.9 m out, each taller than the one before it (57 to 61 m, 76, 91 to 92, 107 to 111 m; the last two are
//               scaled 0.94 to 1.2), so their crowns show one over the other above the flat. "Col?" = +22.6 m along, the
//               slot's own gap; "Col?n" = -22.6 m, the gap of the next slot on. Only gaps that hold NO side street
//               (G_Street: left gaps of slots 26, 29, 32, 35, 38, 41, 44; right gaps of 25, 28, 31, 34, 37, 40, 43, 46),
//               and none on the right in slots 36 and 38 (the inside of the banked bend 1.60 to 1.74 km).
//   28.6 m      ROOF PROP on the left picture houses' roofs (W_Marquee: left, every even slot; up 11.82 m), 13.4 m back
//               along the roof from the anchor: the slots where no people piece stands there (not 30, 42: SM_P3D_HighWire_A;
//               not 34, 46: SM_P3F_Extras_A). Free of SM_RoofParty_A (-9.2 to 1.2 m along) by 2 m.
//   21.4 m      LOW PROPS behind the parked cars (anchor 17.6 m like the kerb sets; 20.2 to 22.6 m out), by every-12 rules
//               that pick the slot's kerb set: K1 = Kerb1 (cars -10 m along), K3 = Kerb3 (cars +7), Cf = KerbCafe (cars +12).
//               Left: Kerb1 only (s % 12 == 4, 10); the other left slots have SM_Queue_A behind the cars. Right: Kerb1 s % 12
//               == 6, Kerb3 s % 12 == 2 (slot 26 only), 8, KerbCafe s % 12 == 4, 10; not 36 and 38 (the cutting inside the bend).
//               Clear of SM_Commuters (23.72 m), SM_P3A_Strip_* (23.05 m on) and the pockets (17.5 m along and on).
//   172 m       HERO, left, slot 38 (1.719 km): outside of the right-hand bend, turned 20 degrees like the dressing's
//               "BigFilmL" was. The far blocks (96 to 150 m) stand in front of its foot.
//   62.4, 70 m  In the corkscrew (1.443 to 1.588 km) two buildings a side in each of slots 32 and 34, by Far rules like the
//               dressing's own wall there (drop 0.6 m), behind its SM_MidBlock (47 to 51 m).
// TALL (log): all but the five kerb props. Smallest x min of a TALL mesh: 28.6 m (MoonFlats, on a roof); on the ground 37.6 m.
#define S1D2(n) TEXT("/Game/World/CityCenter/SM_S1D2_" n)
// gap columns: { mesh, along m, out m (from the 29 m anchor), up m, yaw deg, scale }
Set(TEXT("WldS1D2ColA"),  { { S1D2("MaskMast"),    22.6f, 8.6f, 0.f, 0.f, 1.f }, { S1D2("PuppetHouse"),    22.6f, 15.f, 0.f, 0.f, 1.f }, { S1D2("CostumeHouse"),  22.6f, 22.4f, 0.f, 0.f, 1.f }, { S1D2("FlyTower"),      22.6f, 30.3f, 0.f, 0.f, 0.94f }, { S1D2("MusicHall"),  22.6f, 38.6f, 0.f, 0.f, 1.13f } });
Set(TEXT("WldS1D2ColB"),  { { S1D2("Chandelier"),  22.6f, 8.6f, 0.f, 0.f, 1.f }, { S1D2("RehearsalLoft"),  22.6f, 15.f, 0.f, 0.f, 1.f }, { S1D2("DanceDrum"),     22.6f, 22.7f, 0.f, 0.f, 1.f }, { S1D2("MusicHall"),     22.6f, 30.7f, 0.f, 0.f, 0.94f }, { S1D2("FlyTower"),   22.6f, 40.6f, 0.f, 0.f, 1.13f } });
Set(TEXT("WldS1D2ColC"),  { { S1D2("TicketMast"),  22.6f, 8.6f, 0.f, 0.f, 1.f }, { S1D2("PuppetHouse"),    22.6f, 15.f, 0.f, 0.f, 1.f }, { S1D2("DanceDrum"),     22.6f, 22.7f, 0.f, 0.f, 1.f }, { S1D2("CostumeHouse"),  22.6f, 31.4f, 0.f, 0.f, 1.2f },  { S1D2("MusicHall"),  22.6f, 39.8f, 0.f, 0.f, 1.13f } });
Set(TEXT("WldS1D2ColD"),  { { S1D2("MaskMast"),    22.6f, 8.6f, 0.f, 0.f, 1.f }, { S1D2("RehearsalLoft"),  22.6f, 15.f, 0.f, 0.f, 1.f }, { S1D2("CostumeHouse"),  22.6f, 22.4f, 0.f, 0.f, 1.f }, { S1D2("DanceDrum"),     22.6f, 30.5f, 0.f, 0.f, 1.2f },  { S1D2("FlyTower"),   22.6f, 40.9f, 0.f, 0.f, 1.1f } });
Set(TEXT("WldS1D2ColAn"), { { S1D2("MaskMast"),   -22.6f, 8.6f, 0.f, 0.f, 1.f }, { S1D2("PuppetHouse"),   -22.6f, 15.f, 0.f, 0.f, 1.f }, { S1D2("CostumeHouse"), -22.6f, 22.4f, 0.f, 0.f, 1.f }, { S1D2("FlyTower"),     -22.6f, 30.3f, 0.f, 0.f, 0.94f }, { S1D2("MusicHall"), -22.6f, 38.6f, 0.f, 0.f, 1.13f } });
Set(TEXT("WldS1D2ColBn"), { { S1D2("Chandelier"), -22.6f, 8.6f, 0.f, 0.f, 1.f }, { S1D2("RehearsalLoft"), -22.6f, 15.f, 0.f, 0.f, 1.f }, { S1D2("DanceDrum"),    -22.6f, 22.7f, 0.f, 0.f, 1.f }, { S1D2("MusicHall"),    -22.6f, 30.7f, 0.f, 0.f, 0.94f }, { S1D2("FlyTower"),  -22.6f, 40.6f, 0.f, 0.f, 1.13f } });
Set(TEXT("WldS1D2ColCn"), { { S1D2("TicketMast"), -22.6f, 8.6f, 0.f, 0.f, 1.f }, { S1D2("PuppetHouse"),   -22.6f, 15.f, 0.f, 0.f, 1.f }, { S1D2("DanceDrum"),    -22.6f, 22.7f, 0.f, 0.f, 1.f }, { S1D2("CostumeHouse"), -22.6f, 31.4f, 0.f, 0.f, 1.2f },  { S1D2("MusicHall"), -22.6f, 39.8f, 0.f, 0.f, 1.13f } });
Set(TEXT("WldS1D2ColDn"), { { S1D2("MaskMast"),   -22.6f, 8.6f, 0.f, 0.f, 1.f }, { S1D2("RehearsalLoft"), -22.6f, 15.f, 0.f, 0.f, 1.f }, { S1D2("CostumeHouse"), -22.6f, 22.4f, 0.f, 0.f, 1.f }, { S1D2("DanceDrum"),    -22.6f, 30.5f, 0.f, 0.f, 1.2f },  { S1D2("FlyTower"),  -22.6f, 40.9f, 0.f, 0.f, 1.1f } });
// the kerb, behind the parked cars (anchor 17.6 m: 21.4 m out), low and unlit
Set(TEXT("WldS1D2KerbK1"), { { S1D2("VelvetBenches"), -18.4f, 3.8f, 0.f, 0.f, 1.f }, { S1D2("PosterDrums"), -14.1f, 3.8f, 0.f, 0.f, 1.f }, { S1D2("StageTrunks"), -10.4f, 3.8f, 0.f, 0.f, 1.f }, { S1D2("SpotPlanters"), -6.f, 3.8f, 0.f, 0.f, 1.f }, { S1D2("HatBoxKiosk"), -2.2f, 3.8f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D2KerbK1L"), { { S1D2("PosterDrums"), -14.1f, 3.8f, 0.f, 0.f, 1.f }, { S1D2("StageTrunks"), -10.4f, 3.8f, 0.f, 0.f, 1.f }, { S1D2("SpotPlanters"), -6.f, 3.8f, 0.f, 0.f, 1.f }, { S1D2("HatBoxKiosk"), -2.2f, 3.8f, 0.f, 0.f, 1.f } });   // fix round: left, no benches (they reached into the side street's mouth)
Set(TEXT("WldS1D2KerbK3"), { { S1D2("HatBoxKiosk"), -3.f, 3.8f, 0.f, 0.f, 1.f }, { S1D2("SpotPlanters"), 0.8f, 3.8f, 0.f, 0.f, 1.f }, { S1D2("StageTrunks"), 5.2f, 3.8f, 0.f, 0.f, 1.f }, { S1D2("PosterDrums"), 9.f, 3.8f, 0.f, 0.f, 1.f }, { S1D2("VelvetBenches"), 13.6f, 3.8f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D2KerbCf"), { { S1D2("HatBoxKiosk"), 3.2f, 3.8f, 0.f, 0.f, 1.f }, { S1D2("SpotPlanters"), 7.f, 3.8f, 0.f, 0.f, 1.f }, { S1D2("StageTrunks"), 11.2f, 3.8f, 0.f, 0.f, 1.f }, { S1D2("PosterDrums"), 15.f, 3.8f, 0.f, 0.f, 1.f } });
// a picture house's roof (anchor 29 m, roof 11.82 m up), and the hero
Set(TEXT("WldS1D2Roof"), { { S1D2("MoonFlats"), -13.4f, -0.4f, 11.82f, 0.f, 1.f } });
Set(TEXT("WldS1D2Hero"), { { S1D2("GrandCurtain"), 0.f, 0.f, 0.f, 20.f, 1.f } });
Zone(TEXT("World s1d2, Theatre Row"), 1.10f, 2.10f);
	Site(TEXT("WldS1D2ColA"), -1, 29.f, 12, 8, 1.f);       // left, the slot's own gap: slots 28, 40
	Site(TEXT("WldS1D2ColB"), -1, 29.f, 12, 6, 1.f);       // 30, 42
	Site(TEXT("WldS1D2ColC"), -1, 29.f, 12, 2, 1.f);       // 34, 46
	Site(TEXT("WldS1D2ColD"), -1, 29.f, 12, 0, 1.f);       // 36
	Site(TEXT("WldS1D2ColCn"), -1, 29.f, 12, 10, 1.f);     // left, the next gap on: slots 26, 38
	Site(TEXT("WldS1D2ColDn"), -1, 29.f, 12, 6, 1.f);      // 30, 42
	Site(TEXT("WldS1D2ColAn"), -1, 29.f, 12, 4, 1.f);      // 32, 44
	Site(TEXT("WldS1D2ColBn"), -1, 29.f, 12, 0, 1.f);      // 36
	Site(TEXT("WldS1D2ColB"), 1, 29.f, 24, 22, 1.f);       // right, the slot's own gap: slot 26 (not 38: inside the bend)
	Site(TEXT("WldS1D2ColC"), 1, 29.f, 12, 6, 1.f);        // 30, 42
	Site(TEXT("WldS1D2ColD"), 1, 29.f, 12, 4, 1.f);        // 32, 44
	Site(TEXT("WldS1D2ColDn"), 1, 29.f, 12, 8, 1.f);       // right, the gap before: slots 28, 40
	Site(TEXT("WldS1D2ColAn"), 1, 29.f, 12, 6, 1.f);       // 30, 42
	Site(TEXT("WldS1D2ColBn"), 1, 29.f, 12, 2, 1.f);       // 34, 46
	Site(TEXT("WldS1D2KerbK1L"), -1, 17.6f, 12, 8, 1.f);    // left Kerb1: slots 28, 40
	Site(TEXT("WldS1D2KerbK1L"), -1, 17.6f, 12, 2, 1.f);    // 34, 46
	Site(TEXT("WldS1D2KerbK1"), 1, 17.6f, 12, 6, 1.f);     // right Kerb1: 30, 42 (not 36: the cutting)
	Site(TEXT("WldS1D2KerbK3"), 1, 17.6f, 24, 22, 1.f);    // right Kerb3: 26 (not 38: the cutting)
	Site(TEXT("WldS1D2KerbK3"), 1, 17.6f, 12, 4, 1.f);     // 32, 44
	Site(TEXT("WldS1D2KerbCf"), 1, 17.6f, 12, 8, 1.f);     // right KerbCafe: 28, 40
	Site(TEXT("WldS1D2KerbCf"), 1, 17.6f, 12, 2, 1.f);     // 34, 46
	Site(TEXT("WldS1D2Roof"), -1, 29.f, 12, 10, 1.f);      // left picture houses: slots 26, 38
	Site(TEXT("WldS1D2Roof"), -1, 29.f, 12, 8, 1.f);       // 28, 40
	Site(TEXT("WldS1D2Roof"), -1, 29.f, 12, 4, 1.f);       // 32, 44
	Site(TEXT("WldS1D2Roof"), -1, 29.f, 12, 0, 1.f);       // 36
Zone(TEXT("World s1d2, the Grand Curtain"), 1.70f, 1.74f);                             // one slot: 38 (1.719 km)
	Site(TEXT("WldS1D2Hero"), -1, 172.f, 4, 2, 1.f);
Zone(TEXT("World s1d2, Theatre Row corkscrew"), 1.443f, 1.588f);                       // slots 32 to 35; these four rules last (Far)
	Far(S1D2("CostumeHouse"), 62.6f, 62.6f, 4, 0, 1.f, 1.f, 0.f, 0, 0.6f, 3.f);        // slot 32, both sides
	Far(S1D2("MusicHall"), 70.f, 70.f, 4, 0, 1.f, 1.f, 0.f, 0, 0.6f, 3.f);
	Far(S1D2("RehearsalLoft"), 62.4f, 62.4f, 4, 2, 1.f, 1.f, 0.f, 0, 0.6f, 3.f);       // slot 34, both sides
	Far(S1D2("FlyTower"), 70.f, 70.f, 4, 2, 1.f, 1.f, 0.f, 0, 0.6f, 3.f);
#undef S1D2
// ---- END s1d2 ----
}
{
// ---- BEGIN s1d3 (district 3 "The Garages", 2.10 to 3.13 km, ft_world_city_s1d3.py) ----
// 20 meshes: 1 hero (CorkscrewStack), 7 buildings (DeckGarage, BodyShop, TyreDepot on the left; CarWash, ElevatorTower, PaintShop,
// RampDrum on the right), 12 props (PayBooth, TyreStack, BollardRow, HoistFrame, OilDrums, HeightGantry, BaySign, ToolCart, ArrowSign,
// DeckLamps, CarStack, CarVane).
// WHERE AND WHY (read from FTDressL7.inl, FTPeopleL7.inl and the level's own prop rules in FTTrack.cpp; nothing seen in the game):
//   the kerb, the pockets, the podiums and their roofs are full in every slot of this district, the level's towers stand 33 to 42 m out
//   in every slot, SM_Tower3 (left, 51 to 64 m) only in ODD slots and SM_Tower4 (right, 51 to 82 m) in EVEN slots. Stage 1's district
//   slots are the even ones. So: LEFT lots stand behind the first row where no SM_Tower3 stands: buildings at 62 m (54 to 70), a yard of
//   props behind them (75 to 84 m). RIGHT lots stand behind SM_Tower4 and before SM_MidBlock (128 m +): buildings at 108 m (100 to 116),
//   a yard of props in front (87 to 96 m). The right-hand buildings are OUTSIDE the brief's 26 to 70 m band (it is full there), and no
//   prop stands in the 17 to 40 m bands (full): said in the hand-over.
//   Every rule is a Site, every 12, so each slot is laid out by hand round what the tables put there:
//     s % 12 == 0, 8  (48, 56, 68; 60 is in the jump)  the elevated line runs out from 66 m on the LEFT at along 0: lot A / lot RA
//     s % 12 == 4     (52, 64)  a tower crane 76 to 92 m out, both sides (jib 87 m up): two buildings under 66 m, no yard
//     s % 12 == 2, 6  (50, 54, 66; 62 is in the jump)  the elevated line on the RIGHT at along 0: lot C / lot RC
//     s % 12 == 10    (58)  a crane, and the line on the right
//   No mesh stands both inside and outside 70 m: the three left buildings only at 62 m, everything else 75 m and more out.
//   THE CORKSCREW STACK: right, 148 m out, slot 50 (2.262 km, the outside of the left-hand hairpin 2.21 to 2.33), set 32 m back along
//   the road to clear the elevated line; and left, 190 m out, slot 54 (2.443 km, the outside of the right-hand hairpin: the dressing's
//   own "Street's end 2.44" slot). Both outside 2.52 to 2.96 km.
// Estimate (22.1 x 1.03 = 22.8 slots; a Site every 12: 22.8 / 12 x 0.75 = 1.42 placements a rule, x parts):
//   lots A, C, RA, RC: 2 rules x 1.42 x 15 parts = 42.7 each = 170.8; lots B, D, RB: 1.42 x 2 = 2.8 each = 8.5; lot RD: 1.42 x 4 = 5.7;
//   hero: 2 one-slot zones x 0.75 = 1.5. About 186 placed. (By the slot list: 4 x 45 + 4 + 2 + 4 + 4 + 2 = 196 before the track leaves any out.)
//   Most of one mesh: BollardRow, 2 in each of the four big lots: 8 rules x 1.42 x 2 = 23 (24 by the slot list). Hero: 2.
#define S1D3(n) TEXT("/Game/World/CityCenter/SM_S1D3_" n)
// LEFT lots, anchor 62 m. { mesh, along m, out m, up m, yaw deg, scale }
Set(TEXT("WldS1D3LotA"), {                         // the elevated line at along 0 from 66 m: the yard keeps 6 m clear of it either side
	{ S1D3("DeckGarage"),    12.5f,  0.f,   0.f, 0.f, 1.f },
	{ S1D3("TyreDepot"),    -13.f,   0.f,   0.f, 0.f, 1.f },
	{ S1D3("HeightGantry"), -16.f,  14.5f,  0.f, 0.f, 1.f },
	{ S1D3("PayBooth"),     -10.f,  14.5f,  0.f, 0.f, 1.f },
	{ S1D3("DeckLamps"),     13.f,  14.5f,  0.f, 0.f, 1.f },
	{ S1D3("ArrowSign"),     20.f,  14.5f,  0.f, 0.f, 1.f },
	{ S1D3("BaySign"),       -7.f,  17.5f,  0.f, 0.f, 1.f },
	{ S1D3("BollardRow"),   -13.5f, 17.3f,  0.f, 0.f, 1.f },
	{ S1D3("OilDrums"),       8.f,  17.3f,  0.f, 0.f, 1.f },
	{ S1D3("BollardRow"),    15.f,  17.3f,  0.f, 0.f, 1.f },
	{ S1D3("CarStack"),     -16.f,  20.5f,  0.f, 0.f, 1.f },
	{ S1D3("TyreStack"),     -8.5f, 20.f,   0.f, 0.f, 1.f },
	{ S1D3("HoistFrame"),     9.5f, 20.f,   0.f, 0.f, 1.f },
	{ S1D3("ToolCart"),      15.5f, 20.f,   0.f, 0.f, 1.f },
	{ S1D3("CarVane"),       20.5f, 20.5f,  0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3LotB"), {                         // under a crane: two buildings below 66 m, no yard
	{ S1D3("BodyShop"),      11.f,   0.f,   0.f, 0.f, 1.f },
	{ S1D3("TyreDepot"),    -12.f,   0.f,   0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3LotC"), {
	{ S1D3("DeckGarage"),   -11.f,   0.f,   0.f, 0.f, 1.f },
	{ S1D3("BodyShop"),      11.f,   0.f,   0.f, 0.f, 1.f },
	{ S1D3("HeightGantry"), -16.f,  14.5f,  0.f, 0.f, 1.f },
	{ S1D3("PayBooth"),      -7.5f, 14.5f,  0.f, 0.f, 1.f },
	{ S1D3("ArrowSign"),     -1.f,  14.5f,  0.f, 0.f, 1.f },
	{ S1D3("BollardRow"),     5.5f, 14.5f,  0.f, 0.f, 1.f },
	{ S1D3("DeckLamps"),     15.f,  14.5f,  0.f, 0.f, 1.f },
	{ S1D3("BaySign"),       -1.5f, 17.3f,  0.f, 0.f, 1.f },
	{ S1D3("CarStack"),     -16.f,  20.5f,  0.f, 0.f, 1.f },
	{ S1D3("TyreStack"),     -8.5f, 20.f,   0.f, 0.f, 1.f },
	{ S1D3("OilDrums"),      -4.f,  20.f,   0.f, 0.f, 1.f },
	{ S1D3("HoistFrame"),     2.f,  20.f,   0.f, 0.f, 1.f },
	{ S1D3("ToolCart"),       8.f,  20.f,   0.f, 0.f, 1.f },
	{ S1D3("BollardRow"),    14.5f, 20.f,   0.f, 0.f, 1.f },
	{ S1D3("CarVane"),       20.f,  19.f,   0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3LotD"), {                         // under a crane
	{ S1D3("BodyShop"),     -12.f,   0.f,   0.f, 0.f, 1.f },
	{ S1D3("TyreDepot"),     12.f,   0.f,   0.f, 0.f, 1.f },
});
// RIGHT lots, anchor 108 m: the yard in front of the buildings (out -13.5 to -20.5: 87 to 95 m)
Set(TEXT("WldS1D3LotRA"), {
	{ S1D3("CarWash"),         8.5f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("ElevatorTower"), -14.5f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("HeightGantry"),  -16.f,  -13.5f, 0.f, 0.f, 1.f },
	{ S1D3("PayBooth"),       -7.5f, -13.5f, 0.f, 0.f, 1.f },
	{ S1D3("ArrowSign"),      -1.f,  -13.5f, 0.f, 0.f, 1.f },
	{ S1D3("BollardRow"),      5.5f, -13.5f, 0.f, 0.f, 1.f },
	{ S1D3("DeckLamps"),      15.f,  -13.5f, 0.f, 0.f, 1.f },
	{ S1D3("BaySign"),        -1.5f, -16.3f, 0.f, 0.f, 1.f },
	{ S1D3("CarStack"),      -16.f,  -19.5f, 0.f, 0.f, 1.f },
	{ S1D3("TyreStack"),      -8.5f, -19.f,  0.f, 0.f, 1.f },
	{ S1D3("OilDrums"),       -4.f,  -19.f,  0.f, 0.f, 1.f },
	{ S1D3("HoistFrame"),      2.f,  -19.f,  0.f, 0.f, 1.f },
	{ S1D3("ToolCart"),        8.f,  -19.f,  0.f, 0.f, 1.f },
	{ S1D3("BollardRow"),     14.5f, -19.f,  0.f, 0.f, 1.f },
	{ S1D3("CarVane"),        20.f,  -18.f,  0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3LotRB"), {                        // under a crane: two buildings below 66 m, no yard
	{ S1D3("PaintShop"),      -9.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("RampDrum"),       14.f,   0.f,  0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3LotRC"), {                        // the elevated line at along 0 from 66 m: everything keeps 6 m clear of it either side
	{ S1D3("ElevatorTower"),  13.5f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("RampDrum"),      -16.f,    0.f,  0.f, 0.f, 1.f },
	{ S1D3("HeightGantry"),  -16.f,  -13.5f, 0.f, 0.f, 1.f },
	{ S1D3("PayBooth"),      -10.f,  -13.5f, 0.f, 0.f, 1.f },
	{ S1D3("DeckLamps"),      13.f,  -13.5f, 0.f, 0.f, 1.f },
	{ S1D3("ArrowSign"),      20.f,  -13.5f, 0.f, 0.f, 1.f },
	{ S1D3("BaySign"),        -7.f,  -16.5f, 0.f, 0.f, 1.f },
	{ S1D3("BollardRow"),    -13.5f, -16.3f, 0.f, 0.f, 1.f },
	{ S1D3("OilDrums"),        8.f,  -16.3f, 0.f, 0.f, 1.f },
	{ S1D3("BollardRow"),     15.f,  -16.3f, 0.f, 0.f, 1.f },
	{ S1D3("CarStack"),      -16.f,  -19.5f, 0.f, 0.f, 1.f },
	{ S1D3("TyreStack"),      -8.5f, -19.f,  0.f, 0.f, 1.f },
	{ S1D3("HoistFrame"),      9.5f, -19.f,  0.f, 0.f, 1.f },
	{ S1D3("ToolCart"),       15.5f, -19.f,  0.f, 0.f, 1.f },
	{ S1D3("CarVane"),        20.5f, -19.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3LotRD"), {                        // under a crane, the elevated line at along 0
	{ S1D3("RampDrum"),       16.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("DeckLamps"),     -12.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("ArrowSign"),     -20.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("CarVane"),        -7.f,   3.f,  0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3HeroR"), { { S1D3("CorkscrewStack"), -32.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D3HeroL"), { { S1D3("CorkscrewStack"),   0.f, 0.f, 0.f, 0.f, 1.f } });
Zone(TEXT("World s1d3, The Garages"), 2.10f, 3.13f);
	Site(TEXT("WldS1D3LotA"), -1, 62.f, 12, 0, 1.f);        // s % 12 == 0: slot 48 (60 is in the jump)
	Site(TEXT("WldS1D3LotA"), -1, 62.f, 12, 4, 1.f);        // s % 12 == 8: slots 56, 68
	Site(TEXT("WldS1D3LotB"), -1, 62.f, 12, 8, 1.f);        // s % 12 == 4: slots 52, 64 (cranes)
	Site(TEXT("WldS1D3LotC"), -1, 62.f, 12, 10, 1.f);       // s % 12 == 2: slot 50 (62 is in the jump)
	Site(TEXT("WldS1D3LotC"), -1, 62.f, 12, 6, 1.f);        // s % 12 == 6: slots 54, 66
	Site(TEXT("WldS1D3LotD"), -1, 62.f, 12, 2, 1.f);        // s % 12 == 10: slot 58 (crane)
	Site(TEXT("WldS1D3LotRA"), 1, 108.f, 12, 0, 1.f);
	Site(TEXT("WldS1D3LotRA"), 1, 108.f, 12, 4, 1.f);
	Site(TEXT("WldS1D3LotRB"), 1, 108.f, 12, 8, 1.f);
	Site(TEXT("WldS1D3LotRC"), 1, 108.f, 12, 10, 1.f);
	Site(TEXT("WldS1D3LotRC"), 1, 108.f, 12, 6, 1.f);
	Site(TEXT("WldS1D3LotRD"), 1, 108.f, 12, 2, 1.f);
Zone(TEXT("World s1d3, the Corkscrew Stack 2.26"), 2.24f, 2.28f);       // slot 50 only
	Site(TEXT("WldS1D3HeroR"), 1, 148.f, 4, 2, 1.f);
Zone(TEXT("World s1d3, the Corkscrew Stack 2.44"), 2.43f, 2.46f);       // slot 54 only
	Site(TEXT("WldS1D3HeroL"), -1, 190.f, 4, 2, 1.f);
#undef S1D3
// ---- END s1d3 ----
}
{
// ---- BEGIN s1d4 (district 4 "The Plaza", ft_world_city_s1d4.py) ----
// 16 meshes: 7 buildings, 8 props, 1 hero (SM_S1D4_Orrery). One Zone, 3.13 to 4.62 km: slots 70 to 102 (33 slots).
// Estimate (the brief's arithmetic): 8 Site rules of every 8 (4 on the right with 6 parts, 4 on the left with 11 parts):
//   33 / 8 x 1 x 0.75 = 3.09 a rule and part; x (4 x 6 + 4 x 11 = 68 parts) = 210; the hero 2 rules of every 32
//   (slot 80 right, slot 98 left) = 2. About 212 placed. By the slots themselves: even slots 70..102 = 17, less the loop's
//   72, 74, 76 = 14 slots x 17 parts = 238, + 2. Most of any one mesh: BenchRing 34 (by slots; 31 by the arithmetic).
// Slots: s % 8 == 0, 2, 4, 6 (every 8, phase 0, 6, 4, 2): all s % 4 == 0 or 2. Every part 41.9 m or more out.
// Where, and why there (from FTDressL7.inl, FTPeopleL7.inl, ft_dress_metro.py; NOT seen in the game):
//   the kerb (17 to 24 m), the podiums and the level's towers (23 to 53 m) are full in every slot, so nothing of this
//   block stands nearer than 41.9 m. On the Plaza's EVEN slots:
//   RIGHT (the open blocks: no podium; G_Monument right (2,0): the monument 22.6 m along, 33.5 m out, its flagpoles to 41.1 m):
//     THE POCKET behind the monument, 41.9 to 53 m out between two towers: a pavilion 11 m wide, two lamp clusters
//     before it, a sundial and a bench ring or boat rack in the 5 m either side of it;
//     and 87 m out on the same line, past the elevated line across the side streets' ends (77 m out, deck 9.75 to
//     12 m up, no pier within 11 m of this line): a domed building seen through the gap, under and over the deck.
//   LEFT (podiums on the even slots; G_Street left (2,0): the side street at +22.6; the monument of the odd slot before
//     is at -22.6 of the even slot: on the left +along runs back down the road):
//     the same POCKET behind that monument (along -22.6); THE STREET'S END 87 m out on the side street's own line
//     (along +22.6: the street's floor ends 69 m out, the viaduct stands at 77 m), a fountain and a pergola walk beside it;
//     THE LOT behind the tower, 56 to 70 m out (no second-row tower on the left's even slots: SM_Tower3 is (2,1)):
//     the model-yacht pond, its balustrade, the boat hire's rack, benches round a tree. FIX ROUND: the lot is taken out: SM_Tower4 (2,0) is
//     placed on BOTH sides (FTTrack.cpp), so a second-row tower does stand on the left's even slots, 51 to 82 m out. YachtPond and
//     UrnBalustrade are no longer placed.
// No mesh stands both inside and outside 70 m (the shadow rule): PalmHouse, Library, Belvedere, FountainJets, Pergola
// and Orrery only past 70 m; the rest only inside it.
#define S1D4(n) TEXT("/Game/World/CityCenter/SM_S1D4_" n)
// right: anchor 29 m. The pocket (along 22.6), the building past the elevated line (out 58: 87 m)
Set(TEXT("WldS1D4R0"), {                               // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D4("Bandstand"),      22.6f, 19.f,   0.f, 0.f, 1.f },
	{ S1D4("GlobeLamps"),     16.9f, 12.9f,  0.f, 0.f, 1.f },
	{ S1D4("GlobeLamps"),     28.3f, 12.9f,  0.f, 0.f, 1.f },
	{ S1D4("Sundial"),        14.4f, 19.5f,  0.f, 0.f, 1.f },
	{ S1D4("BenchRing"),      30.8f, 19.5f,  0.f, 0.f, 1.f },
	{ S1D4("PalmHouse"),      22.6f, 58.f,   0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4R2"), {
	{ S1D4("TeaPavilion"),    22.6f, 19.f,   0.f, 0.f, 1.f },
	{ S1D4("GlobeLamps"),     16.9f, 12.9f,  0.f, 0.f, 1.f },
	{ S1D4("GlobeLamps"),     28.3f, 12.9f,  0.f, 0.f, 1.f },
	{ S1D4("Sundial"),        14.4f, 19.5f,  0.f, 0.f, 1.f },
	{ S1D4("BoatRack"),       30.8f, 19.5f,  0.f, 0.f, 1.f },
	{ S1D4("Library"),        22.6f, 58.f,   0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4R4"), {
	{ S1D4("CarouselHouse"),  22.6f, 19.f,   0.f, 0.f, 1.f },
	{ S1D4("GlobeLamps"),     16.9f, 12.9f,  0.f, 0.f, 1.f },
	{ S1D4("GlobeLamps"),     28.3f, 12.9f,  0.f, 0.f, 1.f },
	{ S1D4("Sundial"),        14.4f, 19.5f,  0.f, 0.f, 1.f },
	{ S1D4("BenchRing"),      30.8f, 19.5f,  0.f, 0.f, 1.f },
	{ S1D4("Belvedere"),      22.6f, 58.f,   0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4R6"), {
	{ S1D4("FountainCourt"),  22.6f, 19.f,   0.f, 0.f, 1.f },
	{ S1D4("GlobeLamps"),     16.9f, 12.9f,  0.f, 0.f, 1.f },
	{ S1D4("GlobeLamps"),     28.3f, 12.9f,  0.f, 0.f, 1.f },
	{ S1D4("Sundial"),        14.4f, 19.5f,  0.f, 0.f, 1.f },
	{ S1D4("BoatRack"),       30.8f, 19.5f,  0.f, 0.f, 1.f },
	{ S1D4("PalmHouse"),      22.6f, 58.f,   0.f, 0.f, 1.f },
});
// left: anchor 29 m. The pocket (along -22.6), the street's end (along +22.6, out 58: 87 m), the lot (along -3, 56 to 70 m)
Set(TEXT("WldS1D4L0"), {
	{ S1D4("CarouselHouse"), -22.6f, 19.f,   0.f, 0.f, 1.f },
	{ S1D4("BenchRing"),     -30.8f, 19.5f,  0.f, 0.f, 1.f },
	{ S1D4("BoatRack"),      -14.4f, 19.5f,  0.f, 0.f, 1.f },
	{ S1D4("Library"),        22.6f, 58.f,   0.f, 0.f, 1.f },
	{ S1D4("FountainJets"),   14.f,  55.f,   0.f, 0.f, 1.f },
	{ S1D4("Pergola"),        31.9f, 58.f,   0.f, 90.f, 1.f },   // the walk runs away from the road, beside the building
});
Set(TEXT("WldS1D4L2"), {
	{ S1D4("FountainCourt"), -22.6f, 19.f,   0.f, 0.f, 1.f },
	{ S1D4("BenchRing"),     -30.8f, 19.5f,  0.f, 0.f, 1.f },
	{ S1D4("Sundial"),       -14.4f, 19.5f,  0.f, 0.f, 1.f },
	{ S1D4("Belvedere"),      22.6f, 58.f,   0.f, 0.f, 1.f },
	{ S1D4("FountainJets"),   14.f,  55.f,   0.f, 0.f, 1.f },
	{ S1D4("Pergola"),        31.9f, 58.f,   0.f, 90.f, 1.f },
});
Set(TEXT("WldS1D4L4"), {
	{ S1D4("Bandstand"),     -22.6f, 19.f,   0.f, 0.f, 1.f },
	{ S1D4("BenchRing"),     -30.8f, 19.5f,  0.f, 0.f, 1.f },
	{ S1D4("BoatRack"),      -14.4f, 19.5f,  0.f, 0.f, 1.f },
	{ S1D4("PalmHouse"),      22.6f, 58.f,   0.f, 0.f, 1.f },
	{ S1D4("FountainJets"),   14.f,  55.f,   0.f, 0.f, 1.f },
	{ S1D4("Pergola"),        31.9f, 58.f,   0.f, 90.f, 1.f },
});
Set(TEXT("WldS1D4L6"), {
	{ S1D4("TeaPavilion"),   -22.6f, 19.f,   0.f, 0.f, 1.f },
	{ S1D4("BenchRing"),     -30.8f, 19.5f,  0.f, 0.f, 1.f },
	{ S1D4("Sundial"),       -14.4f, 19.5f,  0.f, 0.f, 1.f },
	{ S1D4("Library"),        22.6f, 58.f,   0.f, 0.f, 1.f },
	{ S1D4("FountainJets"),   14.f,  55.f,   0.f, 0.f, 1.f },
	{ S1D4("Pergola"),        31.9f, 58.f,   0.f, 90.f, 1.f },
});
Set(TEXT("WldS1D4Orrery"), { { S1D4("Orrery"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Zone(TEXT("World s1d4, The Plaza"), 3.13f, 4.62f);
	Site(TEXT("WldS1D4R0"), 1, 29.f, 8, 0, 1.f);                 // right, s % 8 == 0: 80, 88, 96 (72 is in the loop)
	Site(TEXT("WldS1D4R2"), 1, 29.f, 8, 6, 1.f);                 // right, s % 8 == 2: 82, 90, 98 (74: the loop)
	Site(TEXT("WldS1D4R4"), 1, 29.f, 8, 4, 1.f);                 // right, s % 8 == 4: 84, 92, 100 (76: the loop)
	Site(TEXT("WldS1D4R6"), 1, 29.f, 8, 2, 1.f);                 // right, s % 8 == 6: 70, 78, 86, 94, 102
	Site(TEXT("WldS1D4L0"), -1, 29.f, 8, 0, 1.f);                // left, the same slots; the pavilion never the one opposite
	Site(TEXT("WldS1D4L2"), -1, 29.f, 8, 6, 1.f);
	Site(TEXT("WldS1D4L4"), -1, 29.f, 8, 4, 1.f);
	Site(TEXT("WldS1D4Orrery"), 1, 176.f, 32, 16, 1.f);          // THE ORRERY, right, slot 80 (3.62 km): outside the left-hand hairpin 3.56 to 3.70
	Site(TEXT("WldS1D4Orrery"), -1, 176.f, 32, 30, 1.f);         // and left, slot 98 (4.43 km): after the right-hand hairpin 4.28 to 4.40
Zone(TEXT("World s1d4, The Plaza, left pockets s % 8 == 6"), 3.13f, 4.59f);   // fix round: not slot 102 (Wall Street's side street starts behind it)
	Site(TEXT("WldS1D4L6"), -1, 29.f, 8, 2, 1.f);
#undef S1D4
// ---- END s1d4 ----
}
{
// ---- BEGIN s1d5 (district 5 "Wall Street", ft_world_city_s1d5.py) ----
// 20 meshes, /Game/World/CityCenter/SM_S1D5_*: 8 buildings (PostTower, CountingTower, VaultDrum, LanternTower, ExchangeTower,
// CoinPressHall, ClerksBlock, ZigguratMinor), 11 props (CopperCupola, VentCowls, TeaCanopy, BellArch, ParapetUrns, PennantMast,
// UrnRow, TapeBench, LedgerCart, PineappleSteps, StreetClock), the hero (GreatScales).
// The zone has slots 103 to 139; this block uses the even ones only (every 4 or 8 with an even phase: s % 4 == 0 or 2).
// WHERE, and why (read from FTDressL7.inl, FTPeopleL7.inl, FTTrack.cpp and the level's section list; nothing seen in the game):
//   GAP TOWERS  A slender tower (9 m and less across, 73 to 87 m tall) on the gap's own line: 22.6 m along from the slot's
//               middle, 43 m out, behind the alley's or the infill's 34 m flat. The level's first-row towers stand within 11.3 m
//               of a slot's middle and are about 20 m across: that line is the one they never reach. Even slots hold no side
//               street here (those are left s % 4 == 3, right s % 4 == 1). The vault's open door reaches to 32.3 m out, 52 m up.
//   ROWS        Two stepped towers a slot and side, 54 to 57 m and 66.5 to 69.5 m out, 92 to 104 m tall, among the level's second
//               row (51 to 82 m out, 46 to about 110 m): they read by their tops. In the corkscrew, where the track leaves
//               Scatter and Site out, four Far rules at the sidewalk's own depth carry both rows on (as the dressing does there).
//   ROOFS       On the FREE END of a podium's roof, clear of every first-pass, watch, crew, p3b, p3c, p3d and p3f piece there:
//               left, the bank (every even slot; roof 10.4 m): a cupola or three vent cowls 31.4 m out at -14.3 m along, a
//               parapet of urns on the cornice (26.05 m out, -15.8 to -3.2 m along; s % 4 == 2 also +3.2 to +15.8, where only
//               the roof runners pass behind it) and, s % 4 == 0, stone steps at -15.2 m;
//               right, s % 4 == 2, the dear doors (9.6 m): a tea canopy 31.6 m out at +13.5, a parapet of urns on the cornice
//               (26.05 m out, -14.3 to -1.7 m along); right, s % 4 == 0, the yard (6.64 m): a bell arch at -13.8 and a pennant
//               mast at +15.3, 30.7 m out, a row of urns at the roof's front edge (29.3 m out, -10 to -3 m). Not on the INSIDE of the four banked
//               bends (left slots 111, 112, 137 to 139; right 108 to 110, 134 to 136): the land is cut down there.
//               If a W_Banks, W_Pent or W_Yards rule's every, phase or side is changed in FTDressL7.inl, change these too.
//   KERB        Low and unlit, behind the parked cars (19.9 to 22.5 m out; the cars' meters end at 19.1 m, the commuters' strip
//               begins at 23.0 m), only at the four level, straight even slots 114, 120, 124 and 132; a street clock 25.0 m
//               out before the left bank's wing at the same slots (the brokers stand to 11.3 m along, the clock at 14.2).
//   HERO        THE GREAT SCALES twice, left, 165 m out (it is 74 m across: 128 to 202 m): slot 110 (4.977 km, the end of the
//               right-hand bend) and slot 136 (6.153 km, the end of the next right-hand bend), turned 38 and 10 degrees to face
//               the street that runs up to each bend. From the section list the nearest other stretch of road is 202 and 171 m
//               from those two spots.
// Light: steady only (M_People cannot blink); every lit mesh stands 25 m or more out (the clock; the next 30 m). Movement (vertex shader only): the hero's
// beam and pans, four weather vanes, two pennants, a vent cowl, a valance, a bell and its clapper. No figures, no animals, no words.
// Estimate (the brief's: slots x chance x parts x 0.75 for Scatter and Site, x 1 for Far), counted slot by slot (count.py):
//   gap towers 8 rules, 36 slot-uses: 27.0;   rows 16 rules, 72 slot-uses: 54.0;   corkscrew (4 Far rules, both sides): 8.0;
//   left roofs 16 slots x 3 parts: 36.0;   right roofs 12 slots (6 yards x 3 parts, 6 dear doors x 2): 22.5;
//   kerb 4 slots x 5 parts: 15.0;   hero 2 slots: 1.5.   Sum: 164 placed.
//   Read from the track's code, not counted in the game: of the 208 Scatter and Site pieces listed, 46 fall in slots the track
//   leaves out by its flags (the corkscrew 104 and 106; roofs in the tower block 116 and 118 and at the jump's kicker 126;
//   everything at 128, the jump's gap), so at most 162 of them and the 8 Far pieces (170) can stand, before the track's road-clearance test.
//   Most of one mesh: ParapetUrns 22.5 (30 listed), ZigguratMinor 22.2. Smallest x min of a TALL mesh: StreetClock 25.0 m.
#define S1D5(n) TEXT("/Game/World/CityCenter/SM_S1D5_" n)
Set(TEXT("WldS1D5GapPost"),    { { S1D5("PostTower"),     22.6f, 14.f, 0.f, 0.f, 1.f } });       // { mesh, along m, out m, up m, yaw deg, scale }
Set(TEXT("WldS1D5GapCount"),   { { S1D5("CountingTower"), 22.6f, 14.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D5GapVault"),   { { S1D5("VaultDrum"),     22.6f, 14.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D5GapLantern"), { { S1D5("LanternTower"),  22.6f, 14.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D5BankRoofA"),  { { S1D5("CopperCupola"), -14.3f, 2.4f, 10.4f, 0.f, 1.f }, { S1D5("ParapetUrns"), -9.5f, -2.95f, 10.4f, 0.f, 1.f },
                                 { S1D5("PineappleSteps"), -15.2f, -1.f, 10.4f, 90.f, 1.f } });
Set(TEXT("WldS1D5BankRoofB"),  { { S1D5("VentCowls"), -14.3f, 2.4f, 10.4f, 0.f, 1.f }, { S1D5("ParapetUrns"), -9.5f, -2.95f, 10.4f, 0.f, 1.f },
                                 { S1D5("ParapetUrns"), 9.5f, -2.95f, 10.4f, 0.f, 1.f } });
Set(TEXT("WldS1D5PentRoof"),   { { S1D5("TeaCanopy"), 13.5f, 2.6f, 9.6f, 0.f, 1.f }, { S1D5("ParapetUrns"), -8.f, -2.95f, 9.6f, 0.f, 1.f } });
Set(TEXT("WldS1D5YardRoof"),   { { S1D5("BellArch"), -13.8f, 1.7f, 6.64f, 0.f, 1.f }, { S1D5("PennantMast"), 15.3f, 1.7f, 6.64f, 0.f, 1.f },
                                 { S1D5("UrnRow"), -6.5f, 0.3f, 6.64f, 0.f, 1.f } });
Set(TEXT("WldS1D5KerbL"),      { { S1D5("TapeBench"), 3.5f, 3.6f, 0.f, 0.f, 1.f }, { S1D5("UrnRow"), 11.5f, 3.6f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D5KerbR"),      { { S1D5("LedgerCart"), -5.f, 3.6f, 0.f, 0.f, 1.f }, { S1D5("PineappleSteps"), -13.f, 3.6f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D5Clock"),      { { S1D5("StreetClock"), 14.2f, -4.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D5ScalesA"),    { { S1D5("GreatScales"), 0.f, 0.f, 0.f, 38.f, 1.f } });
Set(TEXT("WldS1D5ScalesB"),    { { S1D5("GreatScales"), 0.f, 0.f, 0.f, 10.f, 1.f } });
Zone(TEXT("World s1d5, Wall Street"), 4.62f, 6.29f);
	// gap towers: anchor 29 m like the dressing's gap sets, the piece 43 m out; each tower once on each side
	Site(TEXT("WldS1D5GapPost"),    -1, 29.f, 8, 0, 1.f);        // left  s % 8 == 0
	Site(TEXT("WldS1D5GapCount"),   -1, 29.f, 8, 6, 1.f);        // left  s % 8 == 2
	Site(TEXT("WldS1D5GapVault"),   -1, 29.f, 8, 4, 1.f);        // left  s % 8 == 4
	Site(TEXT("WldS1D5GapLantern"), -1, 29.f, 8, 2, 1.f);        // left  s % 8 == 6
	Site(TEXT("WldS1D5GapLantern"),  1, 29.f, 8, 0, 1.f);        // right s % 8 == 0
	Site(TEXT("WldS1D5GapVault"),    1, 29.f, 8, 6, 1.f);        // right s % 8 == 2
	Site(TEXT("WldS1D5GapPost"),     1, 29.f, 8, 4, 1.f);        // right s % 8 == 4
	Site(TEXT("WldS1D5GapCount"),    1, 29.f, 8, 2, 1.f);        // right s % 8 == 6
	// the rows: front 54 to 57 m, back 66.5 to 69.5 m; never the same tower twice in one slot and side
	Scatter(S1D5("ExchangeTower"), 54.f, 57.f, 8, 0, 1.f, 1.f, 0.06f, -1, 4.f);       // left front
	Scatter(S1D5("ClerksBlock"),   54.f, 57.f, 8, 6, 1.f, 1.f, 0.06f, -1, 4.f);
	Scatter(S1D5("ZigguratMinor"), 54.f, 57.f, 8, 4, 1.f, 1.f, 0.06f, -1, 4.f);
	Scatter(S1D5("CoinPressHall"), 54.f, 57.f, 8, 2, 1.f, 1.f, 0.06f, -1, 4.f);
	Scatter(S1D5("ClerksBlock"),   66.5f, 69.5f, 8, 0, 1.f, 1.f, 0.06f, -1, 4.f);     // left back
	Scatter(S1D5("ZigguratMinor"), 66.5f, 69.5f, 8, 6, 1.f, 1.f, 0.06f, -1, 4.f);
	Scatter(S1D5("CoinPressHall"), 66.5f, 69.5f, 8, 4, 1.f, 1.f, 0.06f, -1, 4.f);
	Scatter(S1D5("ZigguratMinor"), 66.5f, 69.5f, 8, 2, 1.f, 1.f, 0.06f, -1, 4.f);
	Scatter(S1D5("ZigguratMinor"), 54.f, 57.f, 8, 0, 1.f, 1.f, 0.06f, 1, 4.f);        // right front
	Scatter(S1D5("ExchangeTower"), 54.f, 57.f, 8, 6, 1.f, 1.f, 0.06f, 1, 4.f);
	Scatter(S1D5("ClerksBlock"),   54.f, 57.f, 8, 4, 1.f, 1.f, 0.06f, 1, 4.f);
	Scatter(S1D5("ClerksBlock"),   54.f, 57.f, 8, 2, 1.f, 1.f, 0.06f, 1, 4.f);
	Scatter(S1D5("CoinPressHall"), 66.5f, 69.5f, 8, 0, 1.f, 1.f, 0.06f, 1, 4.f);      // right back
	Scatter(S1D5("ZigguratMinor"), 66.5f, 69.5f, 8, 6, 1.f, 1.f, 0.06f, 1, 4.f);
	Scatter(S1D5("ExchangeTower"), 66.5f, 69.5f, 8, 4, 1.f, 1.f, 0.06f, 1, 4.f);
	Scatter(S1D5("ZigguratMinor"), 66.5f, 69.5f, 8, 2, 1.f, 1.f, 0.06f, 1, 4.f);
Zone(TEXT("World s1d5, Wall Street corkscrew"), 4.701f, 4.846f);                      // slots 104 and 106: Far, 0.6 m under the road (the sidewalk)
	Far(S1D5("ExchangeTower"), 54.f, 57.f, 4, 0, 1.f, 1.f, 0.06f, 0, 0.6f, 4.f);
	Far(S1D5("ZigguratMinor"), 66.5f, 69.5f, 4, 0, 1.f, 1.f, 0.06f, 0, 0.6f, 4.f);
	Far(S1D5("CoinPressHall"), 54.f, 57.f, 4, 2, 1.f, 1.f, 0.06f, 0, 0.6f, 4.f);
	Far(S1D5("ClerksBlock"),   66.5f, 69.5f, 4, 2, 1.f, 1.f, 0.06f, 0, 0.6f, 4.f);
Zone(TEXT("World s1d5, bank roofs to the first bends"), 4.62f, 4.99f);               // left: slots 108, 110 (104, 106: the corkscrew)
	Site(TEXT("WldS1D5BankRoofA"), -1, 29.f, 4, 0, 1.f);
	Site(TEXT("WldS1D5BankRoofB"), -1, 29.f, 4, 2, 1.f);
Zone(TEXT("World s1d5, bank roofs on"), 5.09f, 6.16f);                                // left: even slots 114 to 136
	Site(TEXT("WldS1D5BankRoofA"), -1, 29.f, 4, 0, 1.f);
	Site(TEXT("WldS1D5BankRoofB"), -1, 29.f, 4, 2, 1.f);
Zone(TEXT("World s1d5, right roofs"), 4.99f, 6.03f);                                  // right: even slots 112 to 132
	Site(TEXT("WldS1D5YardRoof"), 1, 29.f, 4, 0, 1.f);
	Site(TEXT("WldS1D5PentRoof"), 1, 29.f, 4, 2, 1.f);
Zone(TEXT("World s1d5, right roofs, the last bend"), 6.16f, 6.29f);                   // right: slot 138 (the dear doors)
	Site(TEXT("WldS1D5PentRoof"), 1, 29.f, 4, 2, 1.f);
Zone(TEXT("World s1d5, kerb 5.16"), 5.14f, 5.17f);                                    // slot 114
	Site(TEXT("WldS1D5KerbL"), -1, 17.6f, 4, 2, 1.f);
	Site(TEXT("WldS1D5KerbR"),  1, 17.6f, 4, 2, 1.f);
	Site(TEXT("WldS1D5Clock"), -1, 29.f, 4, 2, 1.f);
Zone(TEXT("World s1d5, kerb 5.43 and 5.61"), 5.42f, 5.62f);                           // slots 120 and 124
	Site(TEXT("WldS1D5KerbL"), -1, 17.6f, 4, 0, 1.f);
	Site(TEXT("WldS1D5KerbR"),  1, 17.6f, 4, 0, 1.f);
	Site(TEXT("WldS1D5Clock"), -1, 29.f, 4, 0, 1.f);
Zone(TEXT("World s1d5, kerb 5.97"), 5.96f, 5.98f);                                    // slot 132
	Site(TEXT("WldS1D5KerbL"), -1, 17.6f, 4, 0, 1.f);
	Site(TEXT("WldS1D5KerbR"),  1, 17.6f, 4, 0, 1.f);
	Site(TEXT("WldS1D5Clock"), -1, 29.f, 4, 0, 1.f);
Zone(TEXT("World s1d5, the Great Scales 4.98"), 4.97f, 4.99f);                        // slot 110
	Site(TEXT("WldS1D5ScalesA"), -1, 165.f, 4, 2, 1.f);
Zone(TEXT("World s1d5, the Great Scales 6.15"), 6.14f, 6.16f);                        // slot 136
	Site(TEXT("WldS1D5ScalesB"), -1, 165.f, 4, 0, 1.f);
#undef S1D5
// ---- END s1d5 ----
}
{
// ---- BEGIN s1d6 (district 6 "Penthouse Row", ft_world_city_s1d6.py) ----
// 16 meshes, /Game/World/CityCenter/SM_S1D6_*: 7 buildings (TerraceTower, FinTower, LanternTower, PoolBlock, WinterGarden, HatBox,
// Observatory), 8 props (Canopy, Luggage, Loungers, Piano, CakeFountain, LiftKiosk, PlungePool, Sunfin), the hero (Cascade).
// The zone has slots 140 to 169; this block uses the even ones only (every 4, 12, 24 or 32 with an even phase: s % 4 == 0 or 2).
// WHERE, and why (read from FTDressL7.inl, FTPeopleL7.inl and FTTrack.cpp; nothing seen in the game):
//   ROWS     Three buildings one behind the other on the gap's own line (22.6 m along from the slot's middle, where the dressing
//            stands its alley, banner, monument or infill): 46, 57.5 and 68 m out, 55 to 119 m tall, so that each stands over the
//            one before it and over the 34 m infill. The level's own towers stand at the slots' middles (33-42 m and 51-82 m out,
//            sliding 11 m either way), so the gap's line is the freest ground behind the street wall. Right: every even slot.
//            Left: not where the gap is a side street (left s % 6 == 4, i.e. s % 12 == 4 or 10: cars drive it, 19 to 69 m out).
//   PAIRS    A slim fin tower and a slim lantern tower at each even slot's middle, both sides, 61 and 67 m out, 88 to 119 m tall:
//            they stand in or beside the level's second row of towers (46 to 109 m) and read as white penthouse tops over them.
//   COURT    In the gaps that are open at street level (right s % 6 == 4: the dressing leaves it empty; left s % 6 == 0: behind
//            the Syndicate's banners): a canopy and its carpet 28.6 m out, a luggage trolley 27.2 m, a lift kiosk 32 m, a
//            cake-stand fountain 35.2 m. All nearer than 35.7 m, so the track leaves the whole court out in a tower block.
//            Not at slot 160 (the inside of the right-hand hairpin: the land is cut down there): that slot has rows only.
//   TERRACES On the dear doors' roof terraces (W_Pent, 9.6 m up, like the p3d sets), at the roof's FRONT edge (26.1 to 28.6 m
//            out), only on the stretches of roof no first-pass, watch, crew, p3b, p3c, p3d or p3f rule uses:
//            right s % 6 == 2 (only a lookout at +12.4): pool -12.2, loungers -5.4, piano +1.9, sun fin +6.1;
//            left  s % 6 == 0 (rally -5.05, lookout -12.5): loungers +3.0, sun fin +8.3, pool +11.6;
//            left  s % 12 == 8 (rally, watchers, a crow's nest at +12.4): pool -12.2 (not s % 12 == 2: slot 158 is inside a hairpin);
//            right s % 6 == 4 (rally, watchers, a crow's nest at -12.5): piano +11.7 (not slot 160: inside a hairpin).
//            If a W_Pent rule's every, phase or side is changed in FTDressL7.inl, change these too, or a pool floats.
//   HERO     THE CASCADE twice: right at slot 158 (7.17 km: outside of the left-hand hairpin, dead ahead of the street), and
//            left at slot 146 (6.63 km: before the loop), each 22.6 m along, 160 m out (it is 40 m across: 140 to 180 m).
// Light: steady only (M_People cannot blink); every lit mesh stands 26 m or more out. Movement: a pennant, a bow, vines, palm
// crowns, a valance, a fountain's plume, a swim ring, the sun fin's slow turn; all in the vertex shader. No figures, no animals.
// Estimate (the brief's: slots / every x chance x parts x 0.75, 30 slots):
//   rows 10 rules, every 12, 3 parts: 10 x 2.5 x 0.75 x 3 = 56.3;  court 3 rules every 12 and 1 every 24, 4 parts: 8.75 x 0.75 x 4 = 26.3;
//   pairs 4 rules, every 4, 2 parts: 4 x 7.5 x 0.75 x 2 = 45;  terraces 15 + 11.3 + 1.9 + 2.8 = 30.9;  hero 2 (two one-slot rules).
//   Sum: about 160 placed. The most of one mesh: FinTower and LanternTower, 22.5 each by this sum (24 by the slot list below).
// By the slot list instead (even slots 140 to 168; 150, 152, 154 are in the loop: nothing; 142 to 146 and 162 to 166 are in the
// two tower blocks: nothing nearer than 35.7 m): rows 60, pairs 48, court 12, terraces 16, hero 2 = about 138, before the track's
// own clearance test. Not counted in the game.
#define S1D6(n) TEXT("/Game/World/CityCenter/SM_S1D6_" n)
// the rows (anchor 46 m): front, middle, back
Set(TEXT("WldS1D6RowA"), { { S1D6("HatBox"),    22.6f, 0.f, 0.f, 0.f, 1.15f }, { S1D6("WinterGarden"), 22.6f, 11.5f, 0.f, 0.f, 1.15f }, { S1D6("TerraceTower"), 22.6f, 22.f, 0.f, 0.f, 1.25f } });
Set(TEXT("WldS1D6RowB"), { { S1D6("PoolBlock"), 22.6f, 0.f, 0.f, 0.f, 1.f },   { S1D6("Observatory"),  22.6f, 11.5f, 0.f, 0.f, 1.15f }, { S1D6("WinterGarden"), 22.6f, 22.f, 0.f, 0.f, 1.45f } });
Set(TEXT("WldS1D6RowC"), { { S1D6("PoolBlock"), 22.6f, 0.f, 0.f, 0.f, 1.05f }, { S1D6("TerraceTower"), 22.6f, 11.5f, 0.f, 0.f, 1.f },   { S1D6("Observatory"),  22.6f, 22.f, 0.f, 0.f, 1.4f } });
Set(TEXT("WldS1D6RowD"), { { S1D6("HatBox"),    22.6f, 0.f, 0.f, 0.f, 1.2f },  { S1D6("Observatory"),  22.6f, 11.5f, 0.f, 0.f, 1.15f }, { S1D6("TerraceTower"), 22.6f, 22.f, 0.f, 0.f, 1.25f } });
Set(TEXT("WldS1D6RowE"), { { S1D6("PoolBlock"), 22.6f, 0.f, 0.f, 0.f, 1.f },   { S1D6("WinterGarden"), 22.6f, 11.5f, 0.f, 0.f, 1.15f }, { S1D6("TerraceTower"), 22.6f, 22.f, 0.f, 0.f, 1.2f } });
// the court in an open gap (anchor 46 m: canopy 28.6, luggage 27.2, kiosk 32, fountain 35.2 m out)
Set(TEXT("WldS1D6Court"), { { S1D6("Canopy"), 22.6f, -17.4f, 0.f, 0.f, 1.f }, { S1D6("Luggage"), 19.f, -18.8f, 0.f, 0.f, 1.f }, { S1D6("LiftKiosk"), 26.4f, -14.f, 0.f, 0.f, 1.f }, { S1D6("CakeFountain"), 21.8f, -10.8f, 0.f, 0.f, 1.f } });
// the slim pairs at a slot's middle (anchor 61 m)
Set(TEXT("WldS1D6PairA"), { { S1D6("FinTower"),     -7.f, 0.f, 0.f, 0.f, 1.f },   { S1D6("LanternTower"), 8.f, 6.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D6PairB"), { { S1D6("LanternTower"), -7.f, 0.f, 0.f, 0.f, 0.9f }, { S1D6("FinTower"),     8.f, 6.f, 0.f, 0.f, 1.08f } });
// the dear doors' roof terraces (anchor 29 m, the podiums' own; up 9.6 m: the terrace)
Set(TEXT("WldS1D6TerrR2"), { { S1D6("PlungePool"), -12.2f, -2.f, 9.6f, 0.f, 1.f }, { S1D6("Loungers"), -5.4f, -1.8f, 9.6f, 0.f, 1.f }, { S1D6("Piano"), 1.9f, -2.f, 9.6f, 0.f, 1.f }, { S1D6("Sunfin"), 6.1f, -1.7f, 9.6f, 0.f, 1.f } });
Set(TEXT("WldS1D6TerrL0"), { { S1D6("Loungers"), 3.f, -1.8f, 9.6f, 0.f, 1.f }, { S1D6("Sunfin"), 8.3f, -2.3f, 9.6f, 0.f, 1.f }, { S1D6("PlungePool"), 11.6f, -2.f, 9.6f, 0.f, 1.f } });
Set(TEXT("WldS1D6TerrL2"), { { S1D6("PlungePool"), -12.2f, -2.f, 9.6f, 0.f, 1.f } });
Set(TEXT("WldS1D6TerrR4"), { { S1D6("Piano"), 11.7f, -2.f, 9.6f, 0.f, 1.f } });
// the hero
Set(TEXT("WldS1D6Cascade"), { { S1D6("Cascade"), 22.6f, 0.f, 0.f, 0.f, 1.f } });
Zone(TEXT("World s1d6, Penthouse Row"), 6.29f, 7.66f);
	// rows, right: s % 12 == 0, 6, 2, 8 (the gap is a glass infill), 4, 10 (the gap is empty)
	Site(TEXT("WldS1D6RowA"), 1, 46.f, 12, 0, 1.f);
	Site(TEXT("WldS1D6RowB"), 1, 46.f, 12, 6, 1.f);
	Site(TEXT("WldS1D6RowC"), 1, 46.f, 12, 10, 1.f);
	Site(TEXT("WldS1D6RowB"), 1, 46.f, 12, 4, 1.f);
	Site(TEXT("WldS1D6RowD"), 1, 46.f, 12, 8, 1.f);
	Site(TEXT("WldS1D6RowE"), 1, 46.f, 12, 2, 1.f);
	// rows, left: s % 12 == 0, 6 (banners), 2, 8 (a monument: it reaches 41 m out)
	Site(TEXT("WldS1D6RowE"), -1, 46.f, 12, 0, 1.f);
	Site(TEXT("WldS1D6RowD"), -1, 46.f, 12, 6, 1.f);
	Site(TEXT("WldS1D6RowA"), -1, 46.f, 12, 10, 1.f);
	Site(TEXT("WldS1D6RowC"), -1, 46.f, 12, 4, 1.f);
	// the court: right s % 12 == 10 and s % 24 == 4 (slot 148, not 160); left s % 12 == 0 and 6
	Site(TEXT("WldS1D6Court"), 1, 46.f, 12, 2, 1.f);
	Site(TEXT("WldS1D6Court"), 1, 46.f, 24, 20, 1.f);
	Site(TEXT("WldS1D6Court"), -1, 46.f, 12, 0, 1.f);
	Site(TEXT("WldS1D6Court"), -1, 46.f, 12, 6, 1.f);
	// the slim pairs: every even slot, both sides
	Site(TEXT("WldS1D6PairA"), 1, 61.f, 4, 0, 1.f, 1.f, 0.06f);
	Site(TEXT("WldS1D6PairB"), 1, 61.f, 4, 2, 1.f, 1.f, 0.06f);
	Site(TEXT("WldS1D6PairB"), -1, 61.f, 4, 0, 1.f, 1.f, 0.06f);
	Site(TEXT("WldS1D6PairA"), -1, 61.f, 4, 2, 1.f, 1.f, 0.06f);
	// the roof terraces
	Site(TEXT("WldS1D6TerrR2"), 1, 29.f, 12, 10, 1.f);
	Site(TEXT("WldS1D6TerrR2"), 1, 29.f, 12, 4, 1.f);
	Site(TEXT("WldS1D6TerrL0"), -1, 29.f, 12, 0, 1.f);
	Site(TEXT("WldS1D6TerrL0"), -1, 29.f, 12, 6, 1.f);
	Site(TEXT("WldS1D6TerrL2"), -1, 29.f, 12, 4, 1.f);
	Site(TEXT("WldS1D6TerrR4"), 1, 29.f, 12, 2, 1.f);
	Site(TEXT("WldS1D6TerrR4"), 1, 29.f, 24, 20, 1.f);
	// THE CASCADE: right at slot 158, left at slot 146
	Site(TEXT("WldS1D6Cascade"), 1, 160.f, 32, 2, 1.f);
	Site(TEXT("WldS1D6Cascade"), -1, 160.f, 32, 14, 1.f);
#undef S1D6
// ---- END s1d6 ----
}
{
// ---- BEGIN s1d7 (district 7 "Syndicate Territory, the arena", 7.66 to 13.56 km, ft_world_city_s1d7.py) ----
// 16 meshes (7 buildings, 8 props, 1 hero), all STILL (the district's house rule); steady light only on pieces stood 27.4 m and more out.
// The zone has slots 170 to 299 (130). This block uses EVEN slots only (s % 4 == 0 or 2), where the dressing has, in every one:
//   LEFT   a black wall (W_Syn (2,0): 32 m long, 7 m deep, roof 9.0 m, its raised middle 11 m long to 11.2 m), KerbSyn at the kerb
//   RIGHT  dear doors, a yard or shops (their roofs are full of the people passes: left alone), KerbSyn2 at the kerb
//   GAPS   (22.6 m along from the anchor) left: banners s % 12 == 4, 10; checkpoint s % 12 == 0, 6; watch tower s % 12 == 2, 8 (left alone)
//          right: banners s % 12 == 0, 6; checkpoint s % 12 == 2, 8; watch tower s % 12 == 4, 10 (left alone)
// Bands used:
//   KERB   21.8 m out (anchor 17.6 + 4.2), behind the barrier rows (the people table: free from 20.6 m), at a block's two ends (along
//          -10.5 and +9.5), only in slots s % 4 == 0 (no "Md" piece of pass 3 stands at a block's middle there on either side)
//   ROOF   of the left black wall (anchor 29, up 9.0; the pennants up 11.2 on its raised middle). The men of PplSynWatch stand at
//          along +9 in s % 8 == 0: that slot's set (RoofC) keeps the +9 end empty
//   GAP    the toll house 33.4 m out behind the banners (they flank its gate), a sentry box or a lantern 27.4 m out beside it;
//          the ledger archive 33.2 m out behind a checkpoint (the checkpoint ends 29.4 m out, the archive's front is 30.1 m out)
//   TOWERS 62 to 68 m out, both sides, over the first row;  HERO 178 m out, left, slots 182, 242, 278 (s % 8 == 2 or 6: the pass 3
//          far pieces at 170 to 255 m stand on the left in s % 8 == 0 only)
// Estimate (sites x parts x chance x 0.75 left out by the track; the three hero sites counted whole):
//   kerb    4 rules x 16 slots x 2 parts x 0.45 x 0.75                                   = 43.2
//   roofs   (RoofA 16 x 3 + RoofB 17 x 2 + RoofC 16 x 2 + RoofD 16 x 2 = 146 parts) x 0.65 x 0.75 = 71.2
//   toll    (left 11 x 3 + 11 x 2, right 10 x 2 + 11 x 2 = 97 parts) x 0.75              = 72.75
//   archive (10 + 11 + 11 + 11 = 43 sites) x 0.75                                        = 32.25
//   towers  CountingTower (8 + 9 slots) x 2 sides x 0.75 = 25.5; KeepTower 8 slots x 2 sides x 0.75 = 12
//   hero    3
//   SUM about 260. Most of one mesh: LedgerArchive 32.25, TollHouse 32.25, CountingTower 25.5, SentryBox 24.75.
// Smallest x min of a TALL mesh: 27.4 m (SentryBox, LanternCluster: anchor 29 - 1.6). The four low meshes: 21.8 m.
#define S1D7(n) TEXT("/Game/World/CityCenter/SM_S1D7_" n)
// the kerb, behind the barrier rows (anchor 17.6)       { mesh, along m, out m, up m, yaw deg, scale }
Set(TEXT("WldS1D7KerbA"), { { S1D7("BollardRow"), -10.5f, 4.2f, 0.f, 0.f, 1.f }, { S1D7("StrongboxCart"), 9.5f, 4.2f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D7KerbB"), { { S1D7("LedgerTable"), -10.f, 4.2f, 0.f, 0.f, 1.f }, { S1D7("MountingBlock"), 9.f, 4.2f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D7KerbC"), { { S1D7("MountingBlock"), -10.f, 4.2f, 0.f, 0.f, 1.f }, { S1D7("BollardRow"), 9.5f, 4.2f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D7KerbD"), { { S1D7("StrongboxCart"), -10.f, 4.2f, 0.f, 0.f, 1.f }, { S1D7("LedgerTable"), 9.5f, 4.2f, 0.f, 0.f, 1.f } });
// the black wall's roof (anchor 29: the wall's own; 9.0 m up, the raised middle 11.2 m)
Set(TEXT("WldS1D7RoofA"), { { S1D7("Strongroom"), -10.2f, 0.f, 9.f, 0.f, 1.f }, { S1D7("LanternCluster"), 10.2f, -1.5f, 9.f, 0.f, 1.f }, { S1D7("PennantRow"), 0.f, 0.f, 11.2f, 0.f, 1.f } });
Set(TEXT("WldS1D7RoofB"), { { S1D7("Mint"), 10.2f, 0.f, 9.f, 0.f, 1.f }, { S1D7("RoofVents"), -10.3f, 0.f, 9.f, 0.f, 1.f } });
Set(TEXT("WldS1D7RoofC"), { { S1D7("ClerksBastion"), -10.2f, 0.f, 9.f, 0.f, 1.f }, { S1D7("PennantRow"), 0.f, 0.f, 11.2f, 0.f, 1.f } });
Set(TEXT("WldS1D7RoofD"), { { S1D7("Strongroom"), 10.2f, 0.f, 9.f, 0.f, 1.f }, { S1D7("RoofVents"), -10.3f, 0.f, 9.f, 0.f, 1.f } });
// the gaps (anchor 29; a gap's middle is 22.6 m along)
Set(TEXT("WldS1D7TollA"), { { S1D7("TollHouse"), 22.6f, 4.4f, 0.f, 0.f, 1.f }, { S1D7("SentryBox"), 17.9f, -1.6f, 0.f, 0.f, 1.f }, { S1D7("SentryBox"), 27.3f, -1.6f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D7TollB"), { { S1D7("TollHouse"), 22.6f, 4.4f, 0.f, 0.f, 1.f }, { S1D7("LanternCluster"), 18.f, -1.6f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D7TollC"), { { S1D7("TollHouse"), 22.6f, 4.4f, 0.f, 0.f, 1.f }, { S1D7("SentryBox"), 27.3f, -1.6f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D7Archive"), { { S1D7("LedgerArchive"), 22.6f, 4.2f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D7Vault"), { { S1D7("Vault"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Zone(TEXT("World s1d7, Syndicate Territory, the arena"), 7.66f, 13.56f);
	Site(TEXT("WldS1D7KerbA"), -1, 17.6f, 8, 0, 0.45f);                           // left, s % 8 == 0
	Site(TEXT("WldS1D7KerbB"), -1, 17.6f, 8, 4, 0.45f);                           // left, s % 8 == 4
	Site(TEXT("WldS1D7KerbC"), 1, 17.6f, 8, 4, 0.45f);                            // right, s % 8 == 4
	Site(TEXT("WldS1D7KerbD"), 1, 17.6f, 8, 0, 0.45f);                            // right, s % 8 == 0
	Site(TEXT("WldS1D7RoofC"), -1, 29.f, 8, 0, 0.65f);                            // left roofs, s % 8 == 0 (the watch stands at +9)
	Site(TEXT("WldS1D7RoofA"), -1, 29.f, 8, 4, 0.65f);                            // s % 8 == 4
	Site(TEXT("WldS1D7RoofB"), -1, 29.f, 8, 6, 0.65f);                            // s % 8 == 2
	Site(TEXT("WldS1D7RoofD"), -1, 29.f, 8, 2, 0.65f);                            // s % 8 == 6
	Site(TEXT("WldS1D7TollA"), -1, 29.f, 12, 8, 1.f);                             // left banner gaps, s % 12 == 4
	Site(TEXT("WldS1D7TollB"), -1, 29.f, 12, 2, 1.f);                             // s % 12 == 10
	Site(TEXT("WldS1D7TollB"), 1, 29.f, 12, 0, 1.f);                              // right banner gaps, s % 12 == 0
	Site(TEXT("WldS1D7TollC"), 1, 29.f, 24, 6, 1.f);                              // s % 24 == 18 (fix round: every 12 put TollHouse in 43 slots; the cap is 40)
	Site(TEXT("WldS1D7Archive"), -1, 29.f, 12, 0, 1.f);                           // left checkpoint gaps, s % 12 == 0
	Site(TEXT("WldS1D7Archive"), -1, 29.f, 12, 6, 1.f);                           // s % 12 == 6
	Site(TEXT("WldS1D7Archive"), 1, 29.f, 12, 10, 1.f);                           // right checkpoint gaps, s % 12 == 2
	Site(TEXT("WldS1D7Archive"), 1, 29.f, 24, 4, 1.f);                            // s % 24 == 20 (fix round: LedgerArchive was placed 41 times; the cap is 40)
	Scatter(S1D7("CountingTower"), 62.f, 68.f, 16, 0, 1.f, 1.f, 0.05f, 0, 4.f);    // both sides, s % 16 == 0
	Scatter(S1D7("CountingTower"), 62.f, 68.f, 16, 6, 1.f, 1.f, 0.05f, 0, 4.f);    // s % 16 == 10
	Scatter(S1D7("KeepTower"), 62.f, 68.f, 16, 12, 1.f, 1.1f, 0.05f, 0, 4.f);      // s % 16 == 4 (none in s % 16 == 2 or 6: the Vault's slots)
// the hero, left, 178 m out (the cube's door 122 m out): one slot each: 210 (9.50 km), 242 (10.95 km), 274 (12.40 km)
Zone(TEXT("World s1d7, the Vault 9.50"), 9.4974f, 9.5034f);   // fix round: slot 210 (182 shared its spot with older far pieces)
	Site(TEXT("WldS1D7Vault"), -1, 178.f, 1, 0, 1.f);
Zone(TEXT("World s1d7, the Vault 10.95"), 10.9451f, 10.9511f);
	Site(TEXT("WldS1D7Vault"), -1, 178.f, 1, 0, 1.f);
Zone(TEXT("World s1d7, the Vault 12.40"), 12.3928f, 12.3988f);   // fix round: slot 274 (278 shared its spot with the helicopter and a far party)
	Site(TEXT("WldS1D7Vault"), -1, 178.f, 1, 0, 1.f);
#undef S1D7
// ---- END s1d7 ----
}
{
// ---- BEGIN s1d8 (district 8 "Syndicate Territory, the circuit": the Chairman's works, ft_world_city_s1d8.py) ----
// 14 meshes: 6 buildings (Gasholder, WaterTank, TramShed, SortingHall in the gaps; SignalCabin, SwitchHouse on the black wall's roof),
// 7 props (LampMast, HandCrane, VentStack on that roof; BufferStop, CableDrums, PipeRack, YardGate low, behind the barrier rows), the hero (Hourglass).
// Every piece is STILL (no moving part); steady light only, and only on the pieces stood 24 m and more out. NOT COMPILED, NOT SEEN IN THE GAME when written.
// Estimate (the brief's arithmetic: slots / every x 0.75; the zone is 11.88 km = 262.5 slots; a low zone is 0.274 km = 6.06 slots, 14 of them):
//   gaps 4 rules of every 12 x 16.4 + 4 of every 24 x 8.2 = 98.4; roofs 6 rules of every 16 x 12.3 + 1 of every 8 x 24.6 = 98.4; low 14 zones x 6 rules x 0.57 = 47.7; hero 3. Sum about 248 placed.
//   Most of one mesh: 24.6 (the four gap buildings, HandCrane, VentStack, LampMast): 33 slots each before the track leaves any out.
// Slots: all even (every 8, 12, 16 or 24 with an even phase: s % 4 == 0 or 2; the hero's three one-slot Zones hold slots 324, 380 and 490). Where each rule stands, from FTDressL7.inl and FTPeopleL7.inl:
//   GAPS   the 13 m between two podiums, 22.6 m along (G_Banner, G_Check: the same along). Left banners are s % 3 == 1, left checkpoints s % 3 == 0;
//          right banners s % 3 == 0, right checkpoints s % 3 == 2. The banners stand 25.5 m out, the checkpoint to 29.4 m: a piece behind the banners
//          has its front 27.3 m out or more, one behind a checkpoint 30.7 m or more. The gaps with a watch tower (G_Search) are left alone.
//   ROOFS  the Syndicate's black wall (W_Syn left (2,0): every even slot on the left; roof 9 m). A: 11.3 m along one way, B: the other; fronts 26 m out.
//          PplSynWatch (left (8,0), 9 m along, the B end) is the only thing the tables stand on that roof: B is never used where s % 8 == 0.
//          On the right the even slots are dear doors, the yard and the shops, whose roofs the people passes share by turns: nothing of this block there.
//   LOW    the band behind the barrier rows, 20.75 to 23.05 m out, at a block's middle (the P3A "Md" sets' place: -1 m along, 4.3 m out of 17.6;
//          those stand in odd slots only). ONLY on each lap's long straight (one Zone a lap, from the section list in FTData.cpp: the circuit starts
//          at 13.7237 km and a lap is 0.83631 km; its straight runs from 0.548 to 0.836 of it), so nothing low floats over a hairpin's cutting.
//   HERO   THE HOURGLASS, left, 190 m out at 1.25 (150 m tall), three one-slot Zones: slots 324, 380, 490 (14.66, 17.19, 22.17 km), each at the end of
//          a lap's right-hand hairpin, on its outside: dead ahead of the long straight that runs up to it. Clear of the left far rules' slots
//          (P3E 170 to 255 m: s % 64 == 0, 16, 24, 32, 48, 56; the far towers s % 48 == 15 and 39) by 3 slots or more.
#define S1D8(n) TEXT("/Game/World/CityCenter/SM_S1D8_" n)
Set(TEXT("WldS1D8GapGas"), { { S1D8("Gasholder"), 22.6f, 4.8f, 0.0f, 0.f, 1.f } });	// front 27.35 m out, back 39.4
Set(TEXT("WldS1D8GapTank"), { { S1D8("WaterTank"), 22.6f, 6.0f, 0.0f, 0.f, 1.f } });	// front 30.8 m out, back 39.7
Set(TEXT("WldS1D8GapShed"), { { S1D8("TramShed"), 22.6f, 4.7f, 0.0f, 0.f, 1.f } });	// front (its rails) 27.4 m out, back 38.3
Set(TEXT("WldS1D8GapSort"), { { S1D8("SortingHall"), 22.6f, 5.4f, 0.0f, 0.f, 1.f } });	// front 30.7 m out, back 38.0
Set(TEXT("WldS1D8RoofCabinA"), { { S1D8("SignalCabin"), -11.3f, -0.7f, 9.0f, 0.f, 1.f } });	// anchor 29: 26.0 to 30.6 m out, on the roof
Set(TEXT("WldS1D8RoofSwitchA"), { { S1D8("SwitchHouse"), -11.3f, -0.7f, 9.0f, 0.f, 1.f } });
Set(TEXT("WldS1D8RoofCraneA"), { { S1D8("HandCrane"), -11.3f, -0.7f, 9.0f, 0.f, 1.f } });
Set(TEXT("WldS1D8RoofCraneB"), { { S1D8("HandCrane"), 11.3f, -0.7f, 9.0f, 0.f, 1.f } });
Set(TEXT("WldS1D8RoofVentA"), { { S1D8("VentStack"), -11.3f, -0.7f, 9.0f, 0.f, 1.f } });
Set(TEXT("WldS1D8RoofVentB"), { { S1D8("VentStack"), 11.3f, -0.7f, 9.0f, 0.f, 1.f } });
Set(TEXT("WldS1D8RoofLampB"), { { S1D8("LampMast"), 11.3f, -0.7f, 9.0f, 0.f, 1.f } });
Set(TEXT("WldS1D8LowBuffer"), { { S1D8("BufferStop"), -1.0f, 4.3f, 0.0f, 0.f, 1.f } });	// anchor 17.6: 21.9 m out
Set(TEXT("WldS1D8LowDrums"), { { S1D8("CableDrums"), -1.0f, 4.3f, 0.0f, 0.f, 1.f } });
Set(TEXT("WldS1D8LowPipes"), { { S1D8("PipeRack"), -1.0f, 4.3f, 0.0f, 0.f, 1.f } });
Set(TEXT("WldS1D8LowGate"), { { S1D8("YardGate"), -1.0f, 4.3f, 0.0f, 0.f, 1.f } });
Set(TEXT("WldS1D8Hero"), { { S1D8("Hourglass"), 0.0f, 0.0f, 0.0f, 0.f, 1.f } });
Zone(TEXT("World s1d8, Syndicate Territory, the circuit"), 13.56f, 99.f);
	Site(TEXT("WldS1D8GapGas"), 1, 29.f, 12, 0, 1.f);	// right, the banner gap (G_Banner right (3,0)), s % 12 == 0
	Site(TEXT("WldS1D8GapGas"), -1, 29.f, 24, 8, 1.f);	// left, the banner gap (G_Banner left (3,2)), s % 24 == 16
	Site(TEXT("WldS1D8GapShed"), -1, 29.f, 24, 20, 1.f);	// left, the banner gap, s % 24 == 4
	Site(TEXT("WldS1D8GapTank"), 1, 29.f, 12, 10, 1.f);	// right, behind the checkpoint (G_Check right (3,1)), s % 12 == 2
	Site(TEXT("WldS1D8GapTank"), -1, 29.f, 24, 6, 1.f);	// left, behind the checkpoint (G_Check left (3,0)), s % 24 == 18
	Site(TEXT("WldS1D8GapSort"), -1, 29.f, 24, 18, 1.f);	// left, behind the checkpoint, s % 24 == 6
	Site(TEXT("WldS1D8GapShed"), 1, 29.f, 12, 6, 1.f);	// right, the banner gap, s % 12 == 6
	Site(TEXT("WldS1D8GapSort"), 1, 29.f, 12, 4, 1.f);	// right, behind the checkpoint, s % 12 == 8
	Site(TEXT("WldS1D8RoofCabinA"), -1, 29.f, 16, 0, 1.f);	// left, on the black wall (W_Syn left (2,0)), s % 16 == 0
	Site(TEXT("WldS1D8RoofSwitchA"), -1, 29.f, 16, 8, 1.f);	// s % 16 == 8
	Site(TEXT("WldS1D8RoofCraneA"), -1, 29.f, 16, 12, 1.f);	// s % 16 == 4
	Site(TEXT("WldS1D8RoofVentA"), -1, 29.f, 16, 4, 1.f);	// s % 16 == 12
	Site(TEXT("WldS1D8RoofLampB"), -1, 29.f, 8, 6, 1.f);	// the wall's other end, s % 8 == 2 (never s % 8 == 0: PplSynWatch stands there)
	Site(TEXT("WldS1D8RoofVentB"), -1, 29.f, 16, 10, 1.f);	// s % 16 == 6
	Site(TEXT("WldS1D8RoofCraneB"), -1, 29.f, 16, 2, 1.f);	// s % 16 == 14
Zone(TEXT("World s1d8, the Hourglass 14.66"), 14.6548f, 14.6608f);
	Site(TEXT("WldS1D8Hero"), -1, 190.f, 1, 0, 1.f, 1.25f);
Zone(TEXT("World s1d8, the Hourglass 17.19"), 17.1882f, 17.1942f);
	Site(TEXT("WldS1D8Hero"), -1, 190.f, 1, 0, 1.f, 1.25f);
Zone(TEXT("World s1d8, the Hourglass 22.17"), 22.1646f, 22.1706f);
	Site(TEXT("WldS1D8Hero"), -1, 190.f, 1, 0, 1.f, 1.25f);
Zone(TEXT("World s1d8, the yard, lap 1"), 14.2837f, 14.5577f);
	Site(TEXT("WldS1D8LowBuffer"), -1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowPipes"), -1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowGate"), -1, 17.6f, 8, 6, 1.f);
	Site(TEXT("WldS1D8LowDrums"), 1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowGate"), 1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowPipes"), 1, 17.6f, 8, 2, 1.f);
Zone(TEXT("World s1d8, the yard, lap 2"), 15.1200f, 15.3940f);
	Site(TEXT("WldS1D8LowBuffer"), -1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowPipes"), -1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowGate"), -1, 17.6f, 8, 6, 1.f);
	Site(TEXT("WldS1D8LowDrums"), 1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowGate"), 1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowPipes"), 1, 17.6f, 8, 2, 1.f);
Zone(TEXT("World s1d8, the yard, lap 3"), 15.9563f, 16.2303f);
	Site(TEXT("WldS1D8LowBuffer"), -1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowPipes"), -1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowGate"), -1, 17.6f, 8, 6, 1.f);
	Site(TEXT("WldS1D8LowDrums"), 1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowGate"), 1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowPipes"), 1, 17.6f, 8, 2, 1.f);
Zone(TEXT("World s1d8, the yard, lap 4"), 16.7926f, 17.0666f);
	Site(TEXT("WldS1D8LowBuffer"), -1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowPipes"), -1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowGate"), -1, 17.6f, 8, 6, 1.f);
	Site(TEXT("WldS1D8LowDrums"), 1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowGate"), 1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowPipes"), 1, 17.6f, 8, 2, 1.f);
Zone(TEXT("World s1d8, the yard, lap 5"), 17.6289f, 17.9029f);
	Site(TEXT("WldS1D8LowBuffer"), -1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowPipes"), -1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowGate"), -1, 17.6f, 8, 6, 1.f);
	Site(TEXT("WldS1D8LowDrums"), 1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowGate"), 1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowPipes"), 1, 17.6f, 8, 2, 1.f);
Zone(TEXT("World s1d8, the yard, lap 6"), 18.4653f, 18.7393f);
	Site(TEXT("WldS1D8LowBuffer"), -1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowPipes"), -1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowGate"), -1, 17.6f, 8, 6, 1.f);
	Site(TEXT("WldS1D8LowDrums"), 1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowGate"), 1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowPipes"), 1, 17.6f, 8, 2, 1.f);
Zone(TEXT("World s1d8, the yard, lap 7"), 19.3016f, 19.5756f);
	Site(TEXT("WldS1D8LowBuffer"), -1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowPipes"), -1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowGate"), -1, 17.6f, 8, 6, 1.f);
	Site(TEXT("WldS1D8LowDrums"), 1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowGate"), 1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowPipes"), 1, 17.6f, 8, 2, 1.f);
Zone(TEXT("World s1d8, the yard, lap 8"), 20.1379f, 20.4119f);
	Site(TEXT("WldS1D8LowBuffer"), -1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowPipes"), -1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowGate"), -1, 17.6f, 8, 6, 1.f);
	Site(TEXT("WldS1D8LowDrums"), 1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowGate"), 1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowPipes"), 1, 17.6f, 8, 2, 1.f);
Zone(TEXT("World s1d8, the yard, lap 9"), 20.9742f, 21.2482f);
	Site(TEXT("WldS1D8LowBuffer"), -1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowPipes"), -1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowGate"), -1, 17.6f, 8, 6, 1.f);
	Site(TEXT("WldS1D8LowDrums"), 1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowGate"), 1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowPipes"), 1, 17.6f, 8, 2, 1.f);
Zone(TEXT("World s1d8, the yard, lap 10"), 21.8105f, 22.0845f);
	Site(TEXT("WldS1D8LowBuffer"), -1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowPipes"), -1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowGate"), -1, 17.6f, 8, 6, 1.f);
	Site(TEXT("WldS1D8LowDrums"), 1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowGate"), 1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowPipes"), 1, 17.6f, 8, 2, 1.f);
Zone(TEXT("World s1d8, the yard, lap 11"), 22.6468f, 22.9208f);
	Site(TEXT("WldS1D8LowBuffer"), -1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowPipes"), -1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowGate"), -1, 17.6f, 8, 6, 1.f);
	Site(TEXT("WldS1D8LowDrums"), 1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowGate"), 1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowPipes"), 1, 17.6f, 8, 2, 1.f);
Zone(TEXT("World s1d8, the yard, lap 12"), 23.4831f, 23.7571f);
	Site(TEXT("WldS1D8LowBuffer"), -1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowPipes"), -1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowGate"), -1, 17.6f, 8, 6, 1.f);
	Site(TEXT("WldS1D8LowDrums"), 1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowGate"), 1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowPipes"), 1, 17.6f, 8, 2, 1.f);
Zone(TEXT("World s1d8, the yard, lap 13"), 24.3194f, 24.5934f);
	Site(TEXT("WldS1D8LowBuffer"), -1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowPipes"), -1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowGate"), -1, 17.6f, 8, 6, 1.f);
	Site(TEXT("WldS1D8LowDrums"), 1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowGate"), 1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowPipes"), 1, 17.6f, 8, 2, 1.f);
Zone(TEXT("World s1d8, the yard, lap 14"), 25.1557f, 25.4297f);
	Site(TEXT("WldS1D8LowBuffer"), -1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowPipes"), -1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowGate"), -1, 17.6f, 8, 6, 1.f);
	Site(TEXT("WldS1D8LowDrums"), 1, 17.6f, 8, 0, 1.f);
	Site(TEXT("WldS1D8LowGate"), 1, 17.6f, 8, 4, 1.f);
	Site(TEXT("WldS1D8LowPipes"), 1, 17.6f, 8, 2, 1.f);
#undef S1D8
// ---- END s1d8 ----
}
{
// ---- BEGIN s1plants (plants of all eight districts, ft_world_city_s1plants.py; this block is written by C:/ftwork/jobs/L7s1plants/place.py) ----
// 24 meshes, /Game/World/CityCenter/SM_S1PLANTS_*. Every rule is a Site with chance 1 in the plants' slots (s % 4 == 3: every a multiple of 4, phase % 4 == 1).
// KERB (anchor 17.6 m): low pieces behind the parked cars, 20.85 to 22.35 m out (21.15 to 22.65 m in Syndicate Territory, behind its barrier rows).
// ROOF (anchor 29 m): tall pieces in their own tubs at the front edge of a podium's roof, 26.7 to 29.7 m out, up = that podium's roof.
// OPEN (anchor 29 m): the willow and the magnolia on the ground of the Plaza's open blocks, 27.5 and 27.2 m out.
// Each rule was checked against every Site of FTDressL7.inl and FTPeopleL7.inl in its slots and their neighbours (place.py): a rule has
// the every and phase it has because of what stands there already. If a podium, kerb or people rule changes, run place.py again.
// Estimate: 578 rule slots x 0.75 (the track's share left out) = about 434 placed. By mesh (rule slots): BambooScreen 2, BirchPair 22, BoxHedge 30, Bramble 43, CamelliaTubs 16, Cherry 3, CypressUrns 2, GeraniumRun 23, HangTrough 20, IvyTrellis 23, LaurelUrns 16, LimeRow 33, Magnolia 6, PalmTub 3, PlaneBox 1, RoseBed 76, SumacLow 63, SumacTall 25, ThistlePampas 44, TomatoRoof 1, TulipBed 15, Willow 12, YewDrums 66, YewObelisk 33.
// By district (rule slots): d1 25, d2 18, d3 25, d4 47, d5 32, d6 38, d7 198, d8 195. Smallest x of a TALL mesh: 26.7 m (a roof); of a low one: 20.85 m.
#define S1PLANTS(n) TEXT("/Game/World/CityCenter/SM_S1PLANTS_" n)
Set(TEXT("WldS1PLANTS001"), { { S1PLANTS("GeraniumRun"), -14.8f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS002"), { { S1PLANTS("GeraniumRun"), -7.2f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS003"), { { S1PLANTS("GeraniumRun"), -2.4f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS004"), { { S1PLANTS("GeraniumRun"), 5.2f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS005"), { { S1PLANTS("GeraniumRun"), 5.4f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS006"), { { S1PLANTS("GeraniumRun"), 12.8f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS007"), { { S1PLANTS("GeraniumRun"), 13.0f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS008"), { { S1PLANTS("PlaneBox"), -0.2f, -2.30f, 8.88f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS009"), { { S1PLANTS("TomatoRoof"), -0.2f, -2.30f, 5.48f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS010"), { { S1PLANTS("BambooScreen"), -6.0f, -2.30f, 11.82f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS011"), { { S1PLANTS("CamelliaTubs"), -14.8f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS012"), { { S1PLANTS("CamelliaTubs"), -7.2f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS013"), { { S1PLANTS("CamelliaTubs"), 1.0f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS014"), { { S1PLANTS("CamelliaTubs"), 5.4f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS015"), { { S1PLANTS("CamelliaTubs"), 8.6f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS016"), { { S1PLANTS("CamelliaTubs"), 13.0f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS017"), { { S1PLANTS("Cherry"), -0.1f, -2.30f, 11.82f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS018"), { { S1PLANTS("IvyTrellis"), 0.7f, 0.70f, 6.64f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS019"), { { S1PLANTS("SumacLow"), 5.6f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS020"), { { S1PLANTS("SumacLow"), 13.2f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS021"), { { S1PLANTS("SumacLow"), -14.8f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS022"), { { S1PLANTS("SumacLow"), -7.2f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS023"), { { S1PLANTS("SumacTall"), -0.2f, 0.70f, 6.64f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS024"), { { S1PLANTS("SumacTall"), 0.3f, 0.70f, 6.64f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS025"), { { S1PLANTS("BoxHedge"), -14.8f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS026"), { { S1PLANTS("BoxHedge"), 5.2f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS027"), { { S1PLANTS("BoxHedge"), 5.4f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS028"), { { S1PLANTS("BoxHedge"), -2.4f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS029"), { { S1PLANTS("BoxHedge"), 12.8f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS030"), { { S1PLANTS("Magnolia"), 0.0f, -1.80f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS031"), { { S1PLANTS("TulipBed"), -7.2f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS032"), { { S1PLANTS("TulipBed"), -2.4f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS033"), { { S1PLANTS("TulipBed"), 12.8f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS034"), { { S1PLANTS("TulipBed"), 13.0f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS035"), { { S1PLANTS("TulipBed"), 5.2f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS036"), { { S1PLANTS("Willow"), -9.5f, -1.50f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS037"), { { S1PLANTS("Willow"), 9.5f, -1.50f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS038"), { { S1PLANTS("BoxHedge"), -7.2f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS039"), { { S1PLANTS("BoxHedge"), 8.6f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS040"), { { S1PLANTS("LaurelUrns"), -14.8f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS041"), { { S1PLANTS("LaurelUrns"), 1.0f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS042"), { { S1PLANTS("BambooScreen"), 12.0f, -1.80f, 10.36f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS043"), { { S1PLANTS("Cherry"), 5.8f, -2.10f, 9.50f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS044"), { { S1PLANTS("CypressUrns"), 6.3f, -1.80f, 10.36f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS045"), { { S1PLANTS("CypressUrns"), -6.2f, -2.30f, 11.82f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS046"), { { S1PLANTS("HangTrough"), -14.8f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS047"), { { S1PLANTS("HangTrough"), -7.2f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS048"), { { S1PLANTS("HangTrough"), 0.4f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS049"), { { S1PLANTS("PalmTub"), -1.0f, -2.30f, 11.82f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS050"), { { S1PLANTS("PalmTub"), 5.8f, -2.10f, 9.50f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS051"), { { S1PLANTS("RoseBed"), -14.8f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS052"), { { S1PLANTS("RoseBed"), -7.2f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS053"), { { S1PLANTS("RoseBed"), 0.4f, 4.00f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS054"), { { S1PLANTS("LimeRow"), 9.0f, -2.00f, 9.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS055"), { { S1PLANTS("RoseBed"), -14.8f, 4.30f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS056"), { { S1PLANTS("YewDrums"), -7.2f, 4.30f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS057"), { { S1PLANTS("YewDrums"), 5.8f, 4.30f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS058"), { { S1PLANTS("YewDrums"), 6.0f, 4.30f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS059"), { { S1PLANTS("YewObelisk"), -9.2f, -2.00f, 9.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS060"), { { S1PLANTS("BirchPair"), -9.1f, -2.00f, 9.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS061"), { { S1PLANTS("Bramble"), -14.8f, 4.30f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS062"), { { S1PLANTS("IvyTrellis"), -8.8f, -2.00f, 9.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS063"), { { S1PLANTS("SumacLow"), -14.8f, 4.30f, 0.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS064"), { { S1PLANTS("SumacTall"), -9.2f, -2.00f, 9.00f, 0.f, 1.f } });
Set(TEXT("WldS1PLANTS065"), { { S1PLANTS("ThistlePampas"), -14.8f, 4.30f, 0.00f, 0.f, 1.f } });
Zone(TEXT("World s1plants, Fifth and Main"), 0.00f, 1.10f);
	Site(TEXT("WldS1PLANTS001"), -1, 17.6f, 12, 9, 1.f);   // GeraniumRun, 2
	Site(TEXT("WldS1PLANTS002"), -1, 17.6f, 12, 9, 1.f);   // GeraniumRun, 2
	Site(TEXT("WldS1PLANTS003"), -1, 17.6f, 12, 5, 1.f);   // GeraniumRun, 2
	Site(TEXT("WldS1PLANTS004"), -1, 17.6f, 12, 5, 1.f);   // GeraniumRun, 2
	Site(TEXT("WldS1PLANTS005"), -1, 17.6f, 16, 5, 1.f);   // GeraniumRun, 1
	Site(TEXT("WldS1PLANTS006"), -1, 17.6f, 12, 5, 1.f);   // GeraniumRun, 2
	Site(TEXT("WldS1PLANTS007"), -1, 17.6f, 16, 5, 1.f);   // GeraniumRun, 1
	Site(TEXT("WldS1PLANTS001"), 1, 17.6f, 12, 1, 1.f);   // GeraniumRun, 2
	Site(TEXT("WldS1PLANTS002"), 1, 17.6f, 12, 1, 1.f);   // GeraniumRun, 2
	Site(TEXT("WldS1PLANTS003"), 1, 17.6f, 12, 9, 1.f);   // GeraniumRun, 2
	Site(TEXT("WldS1PLANTS004"), 1, 17.6f, 12, 9, 1.f);   // GeraniumRun, 2
	Site(TEXT("WldS1PLANTS005"), 1, 17.6f, 24, 17, 1.f);   // GeraniumRun, 1
	Site(TEXT("WldS1PLANTS006"), 1, 17.6f, 16, 1, 1.f);   // GeraniumRun, 1
	Site(TEXT("WldS1PLANTS007"), 1, 17.6f, 24, 17, 1.f);   // GeraniumRun, 1
	Site(TEXT("WldS1PLANTS008"), -1, 29.0f, 16, 1, 1.f);   // PlaneBox, 1
	Site(TEXT("WldS1PLANTS009"), 1, 29.0f, 16, 1, 1.f);   // TomatoRoof, 1
Zone(TEXT("World s1plants, Theatre Row"), 1.10f, 2.10f);
	Site(TEXT("WldS1PLANTS010"), 1, 29.0f, 16, 1, 1.f);   // BambooScreen, 1
	Site(TEXT("WldS1PLANTS011"), -1, 17.6f, 12, 5, 1.f);   // CamelliaTubs, 2
	Site(TEXT("WldS1PLANTS012"), -1, 17.6f, 12, 5, 1.f);   // CamelliaTubs, 2
	Site(TEXT("WldS1PLANTS013"), -1, 17.6f, 12, 9, 1.f);   // CamelliaTubs, 2
	Site(TEXT("WldS1PLANTS014"), -1, 17.6f, 12, 1, 1.f);   // CamelliaTubs, 1
	Site(TEXT("WldS1PLANTS015"), -1, 17.6f, 12, 9, 1.f);   // CamelliaTubs, 2
	Site(TEXT("WldS1PLANTS016"), -1, 17.6f, 12, 1, 1.f);   // CamelliaTubs, 1
	Site(TEXT("WldS1PLANTS011"), 1, 17.6f, 12, 9, 1.f);   // CamelliaTubs, 2
	Site(TEXT("WldS1PLANTS012"), 1, 17.6f, 12, 9, 1.f);   // CamelliaTubs, 2
	Site(TEXT("WldS1PLANTS014"), 1, 17.6f, 16, 1, 1.f);   // CamelliaTubs, 1
	Site(TEXT("WldS1PLANTS016"), 1, 17.6f, 16, 1, 1.f);   // CamelliaTubs, 1
	Site(TEXT("WldS1PLANTS017"), 1, 29.0f, 16, 1, 1.f);   // Cherry, 1
Zone(TEXT("World s1plants, The Garages"), 2.10f, 3.13f);
	Site(TEXT("WldS1PLANTS018"), -1, 29.0f, 16, 9, 1.f);   // IvyTrellis, 1
	Site(TEXT("WldS1PLANTS018"), -1, 29.0f, 24, 9, 1.f);   // IvyTrellis, 1
	Site(TEXT("WldS1PLANTS019"), -1, 17.6f, 8, 1, 1.f);   // SumacLow, 3
	Site(TEXT("WldS1PLANTS019"), -1, 17.6f, 16, 5, 1.f);   // SumacLow, 1
	Site(TEXT("WldS1PLANTS019"), -1, 17.6f, 24, 5, 1.f);   // SumacLow, 1
	Site(TEXT("WldS1PLANTS020"), -1, 17.6f, 8, 1, 1.f);   // SumacLow, 3
	Site(TEXT("WldS1PLANTS020"), -1, 17.6f, 16, 5, 1.f);   // SumacLow, 1
	Site(TEXT("WldS1PLANTS020"), -1, 17.6f, 24, 5, 1.f);   // SumacLow, 1
	Site(TEXT("WldS1PLANTS021"), 1, 17.6f, 8, 1, 1.f);   // SumacLow, 3
	Site(TEXT("WldS1PLANTS021"), 1, 17.6f, 16, 5, 1.f);   // SumacLow, 1
	Site(TEXT("WldS1PLANTS021"), 1, 17.6f, 24, 5, 1.f);   // SumacLow, 1
	Site(TEXT("WldS1PLANTS022"), 1, 17.6f, 8, 1, 1.f);   // SumacLow, 3
	Site(TEXT("WldS1PLANTS022"), 1, 17.6f, 16, 5, 1.f);   // SumacLow, 1
	Site(TEXT("WldS1PLANTS022"), 1, 17.6f, 24, 5, 1.f);   // SumacLow, 1
	Site(TEXT("WldS1PLANTS023"), -1, 29.0f, 16, 5, 1.f);   // SumacTall, 1
	Site(TEXT("WldS1PLANTS023"), -1, 29.0f, 24, 5, 1.f);   // SumacTall, 1
	Site(TEXT("WldS1PLANTS024"), -1, 29.0f, 24, 1, 1.f);   // SumacTall, 1
Zone(TEXT("World s1plants, The Plaza"), 3.13f, 4.62f);
	Site(TEXT("WldS1PLANTS025"), -1, 17.6f, 24, 9, 1.f);   // BoxHedge, 1
	Site(TEXT("WldS1PLANTS025"), -1, 17.6f, 32, 21, 1.f);   // BoxHedge, 1
	Site(TEXT("WldS1PLANTS026"), -1, 17.6f, 12, 1, 1.f);   // BoxHedge, 3
	Site(TEXT("WldS1PLANTS027"), -1, 17.6f, 24, 5, 1.f);   // BoxHedge, 1
	Site(TEXT("WldS1PLANTS025"), 1, 17.6f, 32, 25, 1.f);   // BoxHedge, 1
	Site(TEXT("WldS1PLANTS028"), 1, 17.6f, 12, 5, 1.f);   // BoxHedge, 2
	Site(TEXT("WldS1PLANTS027"), 1, 17.6f, 12, 9, 1.f);   // BoxHedge, 3
	Site(TEXT("WldS1PLANTS029"), 1, 17.6f, 12, 5, 1.f);   // BoxHedge, 2
	Site(TEXT("WldS1PLANTS030"), -1, 29.0f, 12, 1, 1.f);   // Magnolia, 3
	Site(TEXT("WldS1PLANTS030"), -1, 29.0f, 16, 5, 1.f);   // Magnolia, 2
	Site(TEXT("WldS1PLANTS030"), -1, 29.0f, 24, 9, 1.f);   // Magnolia, 1
	Site(TEXT("WldS1PLANTS031"), -1, 17.6f, 24, 9, 1.f);   // TulipBed, 1
	Site(TEXT("WldS1PLANTS031"), -1, 17.6f, 32, 21, 1.f);   // TulipBed, 1
	Site(TEXT("WldS1PLANTS032"), -1, 17.6f, 12, 1, 1.f);   // TulipBed, 3
	Site(TEXT("WldS1PLANTS033"), -1, 17.6f, 12, 1, 1.f);   // TulipBed, 3
	Site(TEXT("WldS1PLANTS034"), -1, 17.6f, 24, 5, 1.f);   // TulipBed, 1
	Site(TEXT("WldS1PLANTS031"), 1, 17.6f, 32, 25, 1.f);   // TulipBed, 1
	Site(TEXT("WldS1PLANTS035"), 1, 17.6f, 12, 5, 1.f);   // TulipBed, 2
	Site(TEXT("WldS1PLANTS034"), 1, 17.6f, 12, 9, 1.f);   // TulipBed, 3
	Site(TEXT("WldS1PLANTS036"), -1, 29.0f, 12, 1, 1.f);   // Willow, 3
	Site(TEXT("WldS1PLANTS036"), -1, 29.0f, 16, 5, 1.f);   // Willow, 2
	Site(TEXT("WldS1PLANTS036"), -1, 29.0f, 24, 9, 1.f);   // Willow, 1
	Site(TEXT("WldS1PLANTS037"), -1, 29.0f, 12, 1, 1.f);   // Willow, 3
	Site(TEXT("WldS1PLANTS037"), -1, 29.0f, 16, 5, 1.f);   // Willow, 2
	Site(TEXT("WldS1PLANTS037"), -1, 29.0f, 24, 9, 1.f);   // Willow, 1
Zone(TEXT("World s1plants, Wall Street"), 4.62f, 6.29f);
	Site(TEXT("WldS1PLANTS038"), -1, 17.6f, 12, 1, 1.f);   // BoxHedge, 3
	Site(TEXT("WldS1PLANTS038"), -1, 17.6f, 24, 21, 1.f);   // BoxHedge, 1
	Site(TEXT("WldS1PLANTS038"), -1, 17.6f, 24, 17, 1.f);   // BoxHedge, 2
	Site(TEXT("WldS1PLANTS038"), -1, 17.6f, 32, 13, 1.f);   // BoxHedge, 1
	Site(TEXT("WldS1PLANTS038"), -1, 17.6f, 36, 9, 1.f);   // BoxHedge, 1
	Site(TEXT("WldS1PLANTS039"), 1, 17.6f, 12, 5, 1.f);   // BoxHedge, 4
	Site(TEXT("WldS1PLANTS039"), 1, 17.6f, 24, 21, 1.f);   // BoxHedge, 1
	Site(TEXT("WldS1PLANTS039"), 1, 17.6f, 24, 1, 1.f);   // BoxHedge, 1
	Site(TEXT("WldS1PLANTS039"), 1, 17.6f, 32, 29, 1.f);   // BoxHedge, 1
	Site(TEXT("WldS1PLANTS039"), 1, 17.6f, 32, 17, 1.f);   // BoxHedge, 1
	Site(TEXT("WldS1PLANTS040"), -1, 17.6f, 12, 1, 1.f);   // LaurelUrns, 3
	Site(TEXT("WldS1PLANTS040"), -1, 17.6f, 24, 21, 1.f);   // LaurelUrns, 1
	Site(TEXT("WldS1PLANTS040"), -1, 17.6f, 24, 17, 1.f);   // LaurelUrns, 2
	Site(TEXT("WldS1PLANTS040"), -1, 17.6f, 32, 13, 1.f);   // LaurelUrns, 1
	Site(TEXT("WldS1PLANTS040"), -1, 17.6f, 36, 9, 1.f);   // LaurelUrns, 1
	Site(TEXT("WldS1PLANTS041"), 1, 17.6f, 12, 5, 1.f);   // LaurelUrns, 4
	Site(TEXT("WldS1PLANTS041"), 1, 17.6f, 24, 21, 1.f);   // LaurelUrns, 1
	Site(TEXT("WldS1PLANTS041"), 1, 17.6f, 24, 1, 1.f);   // LaurelUrns, 1
	Site(TEXT("WldS1PLANTS041"), 1, 17.6f, 32, 29, 1.f);   // LaurelUrns, 1
	Site(TEXT("WldS1PLANTS041"), 1, 17.6f, 32, 17, 1.f);   // LaurelUrns, 1
Zone(TEXT("World s1plants, Penthouse Row"), 6.29f, 7.66f);
	Site(TEXT("WldS1PLANTS042"), -1, 29.0f, 24, 5, 1.f);   // BambooScreen, 1
	Site(TEXT("WldS1PLANTS043"), -1, 29.0f, 24, 21, 1.f);   // Cherry, 1
	Site(TEXT("WldS1PLANTS043"), 1, 29.0f, 32, 25, 1.f);   // Cherry, 1
	Site(TEXT("WldS1PLANTS044"), -1, 29.0f, 24, 5, 1.f);   // CypressUrns, 1
	Site(TEXT("WldS1PLANTS045"), 1, 29.0f, 24, 21, 1.f);   // CypressUrns, 1
	Site(TEXT("WldS1PLANTS046"), -1, 17.6f, 12, 1, 1.f);   // HangTrough, 3
	Site(TEXT("WldS1PLANTS046"), -1, 17.6f, 24, 17, 1.f);   // HangTrough, 1
	Site(TEXT("WldS1PLANTS047"), -1, 17.6f, 8, 5, 1.f);   // HangTrough, 3
	Site(TEXT("WldS1PLANTS047"), -1, 17.6f, 32, 17, 1.f);   // HangTrough, 1
	Site(TEXT("WldS1PLANTS048"), -1, 17.6f, 12, 5, 1.f);   // HangTrough, 2
	Site(TEXT("WldS1PLANTS048"), -1, 17.6f, 24, 21, 1.f);   // HangTrough, 1
	Site(TEXT("WldS1PLANTS048"), -1, 17.6f, 32, 25, 1.f);   // HangTrough, 1
	Site(TEXT("WldS1PLANTS046"), 1, 17.6f, 12, 1, 1.f);   // HangTrough, 3
	Site(TEXT("WldS1PLANTS046"), 1, 17.6f, 24, 17, 1.f);   // HangTrough, 1
	Site(TEXT("WldS1PLANTS047"), 1, 17.6f, 8, 5, 1.f);   // HangTrough, 3
	Site(TEXT("WldS1PLANTS047"), 1, 17.6f, 32, 17, 1.f);   // HangTrough, 1
	Site(TEXT("WldS1PLANTS049"), 1, 29.0f, 24, 21, 1.f);   // PalmTub, 1
	Site(TEXT("WldS1PLANTS050"), 1, 29.0f, 16, 5, 1.f);   // PalmTub, 1
	Site(TEXT("WldS1PLANTS050"), 1, 29.0f, 32, 17, 1.f);   // PalmTub, 1
	Site(TEXT("WldS1PLANTS051"), -1, 17.6f, 16, 13, 1.f);   // RoseBed, 2
	Site(TEXT("WldS1PLANTS052"), -1, 17.6f, 16, 9, 1.f);   // RoseBed, 2
	Site(TEXT("WldS1PLANTS053"), -1, 17.6f, 16, 5, 1.f);   // RoseBed, 1
	Site(TEXT("WldS1PLANTS053"), -1, 17.6f, 32, 17, 1.f);   // RoseBed, 1
	Site(TEXT("WldS1PLANTS051"), 1, 17.6f, 16, 13, 1.f);   // RoseBed, 2
	Site(TEXT("WldS1PLANTS052"), 1, 17.6f, 16, 9, 1.f);   // RoseBed, 2
Zone(TEXT("World s1plants, Syndicate Territory, the arena"), 7.66f, 13.56f);
	Site(TEXT("WldS1PLANTS054"), 1, 29.0f, 4, 1, 1.f);   // LimeRow, 33
	Site(TEXT("WldS1PLANTS055"), -1, 17.6f, 4, 1, 1.f);   // RoseBed, 33
	Site(TEXT("WldS1PLANTS055"), 1, 17.6f, 4, 1, 1.f);   // RoseBed, 33
	Site(TEXT("WldS1PLANTS056"), -1, 17.6f, 16, 13, 1.f);   // YewDrums, 8
	Site(TEXT("WldS1PLANTS057"), -1, 17.6f, 8, 1, 1.f);   // YewDrums, 16
	Site(TEXT("WldS1PLANTS058"), -1, 17.6f, 16, 5, 1.f);   // YewDrums, 9
	Site(TEXT("WldS1PLANTS056"), 1, 17.6f, 8, 5, 1.f);   // YewDrums, 17
	Site(TEXT("WldS1PLANTS058"), 1, 17.6f, 8, 1, 1.f);   // YewDrums, 16
	Site(TEXT("WldS1PLANTS059"), 1, 29.0f, 4, 1, 1.f);   // YewObelisk, 33
Zone(TEXT("World s1plants, Syndicate Territory, the circuit"), 13.56f, 99.f);
	Site(TEXT("WldS1PLANTS060"), 1, 29.0f, 12, 9, 1.f);   // BirchPair, 22
	Site(TEXT("WldS1PLANTS061"), -1, 17.6f, 12, 1, 1.f);   // Bramble, 21
	Site(TEXT("WldS1PLANTS061"), 1, 17.6f, 12, 9, 1.f);   // Bramble, 22
	Site(TEXT("WldS1PLANTS062"), 1, 29.0f, 12, 1, 1.f);   // IvyTrellis, 21
	Site(TEXT("WldS1PLANTS063"), -1, 17.6f, 12, 5, 1.f);   // SumacLow, 22
	Site(TEXT("WldS1PLANTS063"), 1, 17.6f, 12, 1, 1.f);   // SumacLow, 21
	Site(TEXT("WldS1PLANTS064"), 1, 29.0f, 12, 5, 1.f);   // SumacTall, 22
	Site(TEXT("WldS1PLANTS065"), -1, 17.6f, 12, 9, 1.f);   // ThistlePampas, 22
	Site(TEXT("WldS1PLANTS065"), 1, 17.6f, 12, 5, 1.f);   // ThistlePampas, 22
#undef S1PLANTS
// ---- END s1plants ----
}
{
// ---- BEGIN s1animals (the animals of City Center, ft_world_city_s1animals.py) ----
// 18 meshes. WHERE: this avenue is a canyon (kerb and pockets full, podiums and towers from 25.5 m out), so the ground
// animals stand on the FRONT EDGE of the podium roofs (a strip 0.9 m deep behind the cornice that no rule of either
// table uses: 26.0 to 26.7 m out; "up" = that roof), on the service yard's loading dock, on the water round the Plaza's
// monument and on the ground behind it (42 m). Every near rule is a Site at 29 m (the podiums' own anchor) in slots
// s % 8 == 5 only (phase 3 with every 8; with every 24 the phases 3, 11, 19), copying which podium the dressing stands
// there: if a podium rule's every, phase or side changes in FTDressL7.inl, change it here. The doves are Far rules,
// 100 to 140 m out, 135 m over the road's level (over the gliders' 110 m), ring 16 and 12.5 m: x min - ring = 84 m.
// Estimate (Site: slots x chance x 0.75 x parts; Far x 1), slots s % 8 == 5 a district: 3, 3, 3, 4, 4, 4, 16, 33:
//   d1 6 sites x 2 = 12; d2 6 x 2 = 12; d3 3 x 2 + 3 x 3 = 15; d4 4 x 3 + 4 x 1 = 16; d5 4 x 1 + 4 x 2 = 12;
//   d6 5 x 3 + 2 + 2 + 1 = 20; d7 16 x 2 + 16 = 48; d8 33 x 2 + 33 = 99: 234 x 0.75 = 175.5;
//   doves (7 + 7) + (11 + 11) + (12 + 12) = 60 x 0.9 = 54. About 230 placed. Most of one mesh: SynDogs 48 tried (36).
// Smallest x of a TALL mesh: Gulls 27.55 m (29 - 1.45), DrayHorse 42.2 m, Squirrels 42.4 m.
#define S1ANIMALS(n) TEXT("/Game/World/CityCenter/SM_S1ANIMALS_" n)
// shop roofs (5.48 m; the diner's 4.44 m): Shops1 its low end bay, Shops2 its low middle bay, Shops3 the diner's roof
Set(TEXT("WldS1ANIMALSRoofS1"), { { S1ANIMALS("SparrowWire"), -11.5f, -3.0f, 5.48f, 0.f, 1.f }, { S1ANIMALS("ShopCat"), -7.0f, -3.0f, 5.48f, 0.f, 1.f } });
Set(TEXT("WldS1ANIMALSRoofS2"), { { S1ANIMALS("SparrowWire"), -2.4f, -3.0f, 5.48f, 0.f, 1.f }, { S1ANIMALS("ShopCat"), 3.4f, -3.0f, 5.48f, 0.f, 1.f } });
Set(TEXT("WldS1ANIMALSRoofS3"), { { S1ANIMALS("SparrowWire"), -11.5f, -3.0f, 4.44f, 0.f, 1.f }, { S1ANIMALS("ShopCat"), -6.5f, -3.0f, 4.44f, 0.f, 1.f } });
Set(TEXT("WldS1ANIMALSRoofMq"), { { S1ANIMALS("SparrowWire"), -13.0f, -3.0f, 11.82f, 0.f, 1.f }, { S1ANIMALS("ShopCat"), 1.5f, -3.0f, 11.82f, 0.f, 1.f } });
// the Garages: the yard's loading dock (1.1 m) and its roof's edge (6.64 m); the garage's top deck (12.37 m, its front 27 m out)
Set(TEXT("WldS1ANIMALSYard"), { { S1ANIMALS("RaccoonDog"), -6.5f, -0.8f, 1.1f, 0.f, 1.f }, { S1ANIMALS("ShopCat"), -1.5f, 0.45f, 6.64f, 0.f, 1.f } });
Set(TEXT("WldS1ANIMALSDeck"), { { S1ANIMALS("Gulls"), -8.5f, -1.45f, 12.37f, 0.f, 1.f }, { S1ANIMALS("Gulls"), 7.5f, -1.45f, 12.37f, 0.f, 1.f }, { S1ANIMALS("ShopCat"), 13.5f, -1.45f, 12.37f, 0.f, 1.f } });
// the Plaza: on SM_Monument's own ring of water (G_Monument: 22.6 m along, 33.5 m out, water 0.5 m up) and behind it
Set(TEXT("WldS1ANIMALSMon"), { { S1ANIMALS("DucksSwans"), 22.6f, 4.5f, 0.5f, 0.f, 1.f }, { S1ANIMALS("DrayHorse"), 19.6f, 13.2f, 0.f, 0.f, 1.f }, { S1ANIMALS("Squirrels"), 26.6f, 13.4f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1ANIMALSMonBack"), { { S1ANIMALS("DrayHorse"), 19.6f, 13.2f, 0.f, 0.f, 1.f } });
// the dear doors' roof terrace (9.6 m, its front 26 m out), between its parasols and clipped trees
Set(TEXT("WldS1ANIMALSKoi"), { { S1ANIMALS("KoiTrough"), 3.2f, -2.5f, 9.6f, 0.f, 1.f } });
Set(TEXT("WldS1ANIMALSTerr"), { { S1ANIMALS("Flamingos"), 3.2f, -2.5f, 9.6f, 0.f, 1.f }, { S1ANIMALS("Peacock"), -6.6f, -2.5f, 9.6f, 0.f, 1.f }, { S1ANIMALS("Greyhounds"), 11.2f, -2.5f, 9.6f, 0.f, 1.f } });
// the bank's cornice (10.36 m, its front 26 m out), clear of the pediment; a picture house's roof (11.82 m)
Set(TEXT("WldS1ANIMALSOwlS2"), { { S1ANIMALS("Owls"), -2.4f, -2.9f, 5.48f, 0.f, 1.f } });
Set(TEXT("WldS1ANIMALSOwlBk"), { { S1ANIMALS("Owls"), -13.0f, -2.9f, 10.36f, 0.f, 1.f }, { S1ANIMALS("Owls"), 13.0f, -2.9f, 10.36f, 0.f, 1.f } });
Set(TEXT("WldS1ANIMALSBkPent"), { { S1ANIMALS("Peacock"), -13.0f, -2.5f, 10.36f, 0.f, 1.f }, { S1ANIMALS("KoiTrough"), 13.0f, -2.5f, 10.36f, 0.f, 1.f } });
Set(TEXT("WldS1ANIMALSMqPea"), { { S1ANIMALS("Peacock"), 13.5f, -3.0f, 11.82f, 0.f, 1.f } });
// Syndicate Territory: the bank's roof on the left, the black wall's (9 m, behind its spikes) on the right. Tails only.
Set(TEXT("WldS1ANIMALSSynBk"), { { S1ANIMALS("SynDogs"), -13.0f, -2.3f, 10.36f, 0.f, 1.f }, { S1ANIMALS("SynDogs"), 13.0f, -2.3f, 10.36f, 0.f, 1.f } });
Set(TEXT("WldS1ANIMALSSynWall"), { { S1ANIMALS("SynDogs"), 10.0f, -2.4f, 9.0f, 0.f, 1.f } });
Set(TEXT("WldS1ANIMALSYardBk"), { { S1ANIMALS("FoxPipe"), -13.0f, -2.5f, 10.36f, 0.f, 1.f }, { S1ANIMALS("Heron"), 12.5f, -2.5f, 10.36f, 0.f, 1.f } });
Set(TEXT("WldS1ANIMALSYardWall"), { { S1ANIMALS("YardCats"), 10.0f, -2.6f, 9.0f, 0.f, 1.f } });

Zone(TEXT("World s1animals, Fifth and Main"), 0.f, 1.10f);                  // slots 5, 13, 21: a different shop row each
	Site(TEXT("WldS1ANIMALSRoofS3"), -1, 29.f, 24, 19, 1.f);                // slot 5 left: W_Shops3
	Site(TEXT("WldS1ANIMALSRoofS1"), 1, 29.f, 24, 19, 1.f);                 // slot 5 right: W_Shops1
	Site(TEXT("WldS1ANIMALSRoofS2"), -1, 29.f, 24, 11, 1.f);                // slot 13 left: W_Shops2s
	Site(TEXT("WldS1ANIMALSRoofS3"), 1, 29.f, 24, 11, 1.f);                 // slot 13 right: W_Shops3
	Site(TEXT("WldS1ANIMALSRoofS1"), -1, 29.f, 24, 3, 1.f);                 // slot 21 left: W_Shops1L
	Site(TEXT("WldS1ANIMALSRoofS2"), 1, 29.f, 24, 3, 1.f);                  // slot 21 right: W_Shops2s
Zone(TEXT("World s1animals, Theatre Row"), 1.10f, 2.10f);                   // slots 29, 37, 45: left W_Shops2s, right W_Marquee
	Site(TEXT("WldS1ANIMALSRoofS2"), -1, 29.f, 8, 3, 1.f);
	Site(TEXT("WldS1ANIMALSRoofMq"), 1, 29.f, 8, 3, 1.f);
	Far(S1ANIMALS("Doves_A"), 100.f, 140.f, 3, 0, 0.9f, 1.f, 0.1f, -1, -135.f, 180.f);
	Far(S1ANIMALS("Doves_B"), 100.f, 140.f, 3, 1, 0.9f, 1.f, 0.1f, 1, -135.f, 180.f);
Zone(TEXT("World s1animals, The Garages"), 2.10f, 3.13f);                   // slots 53, 61, 69: left W_Yards, right W_Garage
	Site(TEXT("WldS1ANIMALSYard"), -1, 29.f, 8, 3, 1.f);
	Site(TEXT("WldS1ANIMALSDeck"), 1, 29.f, 8, 3, 1.f);
Zone(TEXT("World s1animals, The Plaza"), 3.13f, 4.62f);                     // slots 77, 85, 93, 101: left G_Monument, right W_Pent
	Site(TEXT("WldS1ANIMALSMon"), -1, 29.f, 8, 3, 1.f);
	Site(TEXT("WldS1ANIMALSKoi"), 1, 29.f, 8, 3, 1.f);
	Far(S1ANIMALS("Doves_A"), 100.f, 140.f, 3, 0, 0.9f, 1.f, 0.1f, -1, -135.f, 180.f);
	Far(S1ANIMALS("Doves_B"), 100.f, 140.f, 3, 1, 0.9f, 1.f, 0.1f, 1, -135.f, 180.f);
Zone(TEXT("World s1animals, Wall Street"), 4.62f, 6.29f);                   // slots 109, 117, 125, 133: left W_Shops2s, right W_Banks
	Site(TEXT("WldS1ANIMALSOwlS2"), -1, 29.f, 8, 3, 1.f);
	Site(TEXT("WldS1ANIMALSOwlBk"), 1, 29.f, 8, 3, 1.f);
	Far(S1ANIMALS("Doves_A"), 100.f, 140.f, 3, 0, 0.9f, 1.f, 0.1f, -1, -135.f, 180.f);
	Far(S1ANIMALS("Doves_B"), 100.f, 140.f, 3, 1, 0.9f, 1.f, 0.1f, 1, -135.f, 180.f);
Zone(TEXT("World s1animals, Penthouse Row"), 6.29f, 7.66f);                 // slots 141, 149, 157, 165
	Site(TEXT("WldS1ANIMALSTerr"), -1, 29.f, 24, 3, 1.f);                   // 141, 165 left: W_Pent
	Site(TEXT("WldS1ANIMALSTerr"), -1, 29.f, 24, 19, 1.f);                  // 149 left: W_Pent
	Site(TEXT("WldS1ANIMALSBkPent"), -1, 29.f, 24, 11, 1.f);                // 157 left: W_Banks
	Site(TEXT("WldS1ANIMALSMqPea"), 1, 29.f, 24, 3, 1.f);                   // 141, 165 right: W_Marquee
	Site(TEXT("WldS1ANIMALSTerr"), 1, 29.f, 24, 19, 1.f);                   // 149 right: W_Pent
	Site(TEXT("WldS1ANIMALSTerr"), 1, 29.f, 24, 11, 1.f);                   // 157 right: W_Pent
	Site(TEXT("WldS1ANIMALSMonBack"), 1, 29.f, 24, 19, 1.f);                // 149 right: behind G_Monument
Zone(TEXT("World s1animals, Syndicate Territory, the arena"), 7.66f, 13.56f);   // left W_Bank, right W_Syn
	Site(TEXT("WldS1ANIMALSSynBk"), -1, 29.f, 8, 3, 1.f);
	Site(TEXT("WldS1ANIMALSSynWall"), 1, 29.f, 8, 3, 1.f);
Zone(TEXT("World s1animals, Syndicate Territory, the circuit"), 13.56f, 99.f);  // left W_Bank, right W_Syn
	Site(TEXT("WldS1ANIMALSYardBk"), -1, 29.f, 8, 3, 1.f);
	Site(TEXT("WldS1ANIMALSYardWall"), 1, 29.f, 8, 3, 1.f);
#undef S1ANIMALS
// ---- END s1animals ----
}
{
// ---- BEGIN s1people (people raised on their own structures, ft_world_city_s1people.py) ----
// 16 meshes, every one TALL (the two balconies are logged "low" because their deck is z = 0: they hang 6 m up, 24.85 m out).
// Every rule is a Site with anchor 29 m (the podiums' own), in slots s % 8 == 1 only (every 8, 16, 24; phase % 8 == 7).
// Smallest distance of any piece's origin: 24.85 m (the balconies), 25.0 m on the ground (the wall line). Nothing lit in
// Syndicate Territory; steady lamps only on the four Plaza pieces (26.7 and 43 m) and under the TuningBalcony's rail (24.85 m).
// Where things stand, from the two tables (not seen in the game):
//   Syndicate Territory, RIGHT, s % 8 == 1: the podium is the black wall (W_Syn, front 25.5 m), its roof and its gap carry no
//     population piece. "SynWall" = five pieces in a line 24.5 to 25.5 m out against the wall (behind p3a's walkway at 23.0 to 24.5).
//     The gap (22.6 m along) holds a banner pair (s % 24 == 9), a watch tower (s % 24 == 1) or a checkpoint (s % 24 == 17):
//     a stand between the banner poles (24.3 to 27.0 m; the cloths hang 4.15 m either side of the gap's middle, the stand's
//     middle is 1 m on), a stand before the tower's legs (24.3 to 26.8; legs from 27.4), a queue before the checkpoint (24.3 to 25.9;
//     its sandbags from 26.2). LEFT the same three by the left side's own gaps (check 9, search 17, banner 1).
//   The Plaza, LEFT, s % 8 == 1: an open block (no podium) and a monument gap (the mayor before it at 25.9 m). Three terraces
//     25.2 to 28.2 m out along the block; a choir on tall steps 43 m out behind the monument (41.1 to 45.4).
//     Terraces only from 3.84 km (before that the left is the inside of a banked bend, or the loop).
//   The Garages: a raised car lift in an alley's mouth (25.5 to 28.2 m; bins of the alley from 28.1). Right 2.10 to 2.52 and
//     2.81 to 3.04, left 2.81 to 3.04 (at 2.24 km the left is the inside of a banked bend; 2.52 to 2.81 is the jump).
//   Glass infills (G_Fill, face 25.5 m) on the LEFT in s % 8 == 1: a balcony 6 m up on the face.
// Estimate (brief's arithmetic, slots / every x chance 1 x 0.75 x parts):
//   arena 130.4 slots: wall 16.3 x 5 = 81.5, right gaps 3 x 5.43 = 16.3, left gaps 16.3            = 114.1
//   circuit 262.6 slots: wall (every 16) 16.4 x 5 = 82.1, right gaps 3 x 10.94 = 32.8, left 32.8   = 147.7
//   Plaza terraces 15.3 slots / 8 x 3 = 5.7, choir 23.0 / 8 = 2.9; Garages 1.2 + 0.6 + 0.6 = 2.4;
//   balconies 0.7 + 1.3 + 1.7 + 0.9 = 4.6                                                          =  15.6
//   sum 277.4 x 0.75 = about 208 placed. Most of one mesh: StandLadders and PassQueue 32.7 x 0.75 = 24.6 each.
#define S1PEOPLE(n) TEXT("/Game/World/CityCenter/SM_S1PEOPLE_" n)
Set(TEXT("WldS1PEOPLESynWall"), {                          // { mesh, along m, out m, up m, yaw deg, scale }: 25.0 m out
	{ S1PEOPLE("UmpireChair"),  -14.2f, -4.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("WallSitters"),   -9.9f, -4.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("NewsStack"),      6.6f, -4.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PressRiser"),    10.6f, -4.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PlinthBarrel"),  15.4f, -4.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEGapBanner"),     { { S1PEOPLE("StandCrates"),  23.6f, -3.4f,  0.f, 0.f, 1.f } });   // 25.6 m out
Set(TEXT("WldS1PEOPLEGapBannerYard"), { { S1PEOPLE("GirderLunch"),  23.6f, -3.4f,  0.f, 0.f, 1.f } });
Set(TEXT("WldS1PEOPLEGapSearch"),     { { S1PEOPLE("StandLadders"), 22.6f, -3.45f, 0.f, 0.f, 1.f } });   // 25.55 m out
Set(TEXT("WldS1PEOPLEGapCheck"),      { { S1PEOPLE("PassQueue"),    21.9f, -3.9f,  0.f, 0.f, 1.f } });   // 25.1 m out
Set(TEXT("WldS1PEOPLEPlazaOpen"), {                        // 26.7 m out
	{ S1PEOPLE("StoryTerrace"), -10.6f, -2.3f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("TaiChi"),         0.0f, -2.3f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("EaselPergola"),  10.6f, -2.3f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEPlazaChoir"), { { S1PEOPLE("ChoirSteps"),    22.6f, 14.0f,  0.f, 0.f, 1.f } });     // 43 m out
Set(TEXT("WldS1PEOPLEGarLift"),    { { S1PEOPLE("LiftLunch"),     22.6f, -2.2f,  0.f, 0.f, 1.f } });     // 26.8 m out
Set(TEXT("WldS1PEOPLETuning"),     { { S1PEOPLE("TuningBalcony"), 22.6f, -4.15f, 6.f, 0.f, 1.f } });     // 24.85 m out, 6 m up
Set(TEXT("WldS1PEOPLEClerks"),     { { S1PEOPLE("ClerksLedge"),   22.6f, -4.15f, 6.f, 0.f, 1.f } });
Zone(TEXT("World s1people: Theatre Row (1)"), 1.10f, 1.34f);
	Site(TEXT("WldS1PEOPLETuning"), -1, 29.f, 8, 7, 1.f);
Zone(TEXT("World s1people: The Garages, to the run-up"), 2.10f, 2.52f);
	Site(TEXT("WldS1PEOPLEGarLift"), 1, 29.f, 8, 7, 1.f);
Zone(TEXT("World s1people: The Garages, after the jump"), 2.81f, 3.04f);
	Site(TEXT("WldS1PEOPLEGarLift"), 1, 29.f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLEGarLift"), -1, 29.f, 8, 7, 1.f);
Zone(TEXT("World s1people: The Plaza, the choir"), 3.49f, 4.53f);
	Site(TEXT("WldS1PEOPLEPlazaChoir"), -1, 29.f, 8, 7, 1.f);
Zone(TEXT("World s1people: The Plaza, the open blocks"), 3.84f, 4.53f);
	Site(TEXT("WldS1PEOPLEPlazaOpen"), -1, 29.f, 8, 7, 1.f);
Zone(TEXT("World s1people: Wall Street (1)"), 4.846f, 5.46f);
	Site(TEXT("WldS1PEOPLEClerks"), -1, 29.f, 8, 7, 1.f);
Zone(TEXT("World s1people: Wall Street (2)"), 5.95f, 6.29f);
	Site(TEXT("WldS1PEOPLEClerks"), -1, 29.f, 8, 7, 1.f);
Zone(TEXT("World s1people: Penthouse Row (2)"), 7.00f, 7.48f);
	Site(TEXT("WldS1PEOPLETuning"), -1, 29.f, 8, 7, 1.f);
Zone(TEXT("World s1people: Syndicate Territory, the arena"), 7.66f, 13.56f);
	Site(TEXT("WldS1PEOPLESynWall"), 1, 29.f, 8, 7, 1.f);            // right, every s % 8 == 1
	Site(TEXT("WldS1PEOPLEGapBanner"), 1, 29.f, 24, 15, 1.f);        // right, s % 24 == 9: G_Banner
	Site(TEXT("WldS1PEOPLEGapSearch"), 1, 29.f, 24, 23, 1.f);        // right, s % 24 == 1: G_Search
	Site(TEXT("WldS1PEOPLEGapCheck"), 1, 29.f, 24, 7, 1.f);          // right, s % 24 == 17: G_Check
	Site(TEXT("WldS1PEOPLEGapCheck"), -1, 29.f, 24, 15, 1.f);        // left, s % 24 == 9: G_Check
	Site(TEXT("WldS1PEOPLEGapSearch"), -1, 29.f, 24, 7, 1.f);        // left, s % 24 == 17: G_Search
	Site(TEXT("WldS1PEOPLEGapBanner"), -1, 29.f, 24, 23, 1.f);       // left, s % 24 == 1: G_Banner
Zone(TEXT("World s1people: Syndicate Territory, the circuit"), 13.56f, 99.f);
	Site(TEXT("WldS1PEOPLESynWall"), 1, 29.f, 16, 15, 1.f);          // right, s % 16 == 1
	Site(TEXT("WldS1PEOPLEGapBannerYard"), 1, 29.f, 24, 15, 1.f);
	Site(TEXT("WldS1PEOPLEGapSearch"), 1, 29.f, 24, 23, 1.f);
	Site(TEXT("WldS1PEOPLEGapCheck"), 1, 29.f, 24, 7, 1.f);
	Site(TEXT("WldS1PEOPLEGapCheck"), -1, 29.f, 24, 15, 1.f);
	Site(TEXT("WldS1PEOPLEGapSearch"), -1, 29.f, 24, 7, 1.f);
	Site(TEXT("WldS1PEOPLEGapBannerYard"), -1, 29.f, 24, 23, 1.f);
#undef S1PEOPLE
// ---- END s1people ----
}
