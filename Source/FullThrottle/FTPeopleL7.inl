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
Zone(TEXT("People far: test"), 0.f, 99.f);
	Far(TEXT("/Game/People/CityCenter/SM_FarGeese"), 38.f, 44.f, 5, 0, 1.f, 1.f, 0.f, 0, -105.f, 6.f);
	Far(TEXT("/Game/People/CityCenter/SM_FarGliders"), 34.f, 42.f, 5, 2, 1.f, 1.5f, 0.f, 0, -95.f, 6.f);
	Far(TEXT("/Game/People/CityCenter/SM_FarTowerParty"), 110.f, 150.f, 4, 1, 1.f, 2.2f, 0.f, 0, 0.7f, 10.f);
	Far(TEXT("/Game/People/CityCenter/SM_FarTowerLoft"), 62.f, 84.f, 4, 3, 1.f, 2.0f, 0.f, 0, 0.7f, 10.f);
	Far(TEXT("/Game/People/CityCenter/SM_FarConcert"), 66.f, 90.f, 6, 2, 1.f, 1.6f, 0.f, 0, 0.7f, 8.f);
	Far(TEXT("/Game/People/CityCenter/SM_FarSkyRun_A"), 80.f, 110.f, 6, 4, 1.f, 1.6f, 0.f, 0, 0.7f, 8.f);
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
// alleys: every other slot, side by side in turn (G_Alley 0, 2, 0); side streets: left (4,1), right (4,3); the dear
// doors: left (4,1), right (4,2)
	Site(TEXT("PplWatchGap"), 0, 29.f, 4, 0, 1.f);
	Site(TEXT("PplWatchSteps"), 0, 29.f, 4, 2, 0.7f);
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
