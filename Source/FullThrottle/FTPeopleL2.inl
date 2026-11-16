// Nevada (level index 2): the population layer. People, creatures and the film crew, placed by rule like the dressing
// (the words and what their numbers mean are in the header of FTTrackDress.cpp; the rule kinds in FTDress.h).
// This table is read after FTDressL2.inl, so the dressing's own placements never move.
//
// WRITTEN BY Content/Python/ft_people_nevada_table.py (plain Python): change that file and run it, not this one.
// The meshes are made by Content/Python/ft_people_nevada.py (/Game/People/Nevada/) and ft_people_crew.py (the crew).
//
// Slot by slot: every row below is one Site rule in a zone of its own (slot s lies s * 45.243 m along the road), so a
// piece stands exactly on its slot, on the side and at the distance the writer gave it, clear of what the dressing has
// there (rock cuts, rock banks, ventilation heads, mile signs; in front of the roadhouses, truck stops, road stops).
// The land lies a metre under this road behind a metre of barrier, so everybody is RAISED: every near piece is a deck,
// a bank or a podium two metres up (TALL: its front stands 24.6 m out or more), the film crew's low pieces stand on a
// scaffold riser (a set: the crew piece 2 m up on SM_CrewRiser), and in the rock cuts the cast is up on the rim (a
// 16.5 m column of the cut's own rock inside the 15 m cut, 33.1 m out). Far life (herds) was saved far=True.
// Rules that bind this layer: nothing over 2 m tall nearer than 24 m to a centre line and nothing at all nearer than
// 17 m; nothing lit; nothing that could be taken for a pickup, an enemy or an attack.
Set(TEXT("PplRoadhouse"), { { TEXT("/Game/People/Nevada/SM_RoadhouseDeck"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplDiner"), { { TEXT("/Game/People/Nevada/SM_DinerDeck"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplHitchA"), { { TEXT("/Game/People/Nevada/SM_Hitchers_A"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplHitchB"), { { TEXT("/Game/People/Nevada/SM_Hitchers_B"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplProspect"), { { TEXT("/Game/People/Nevada/SM_Prospectors"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplLizards"), { { TEXT("/Game/People/Nevada/SM_Lizards"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplRiders"), { { TEXT("/Game/People/Nevada/SM_Riders"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplStandA"), { { TEXT("/Game/People/Nevada/SM_StripStand_A"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplStandB"), { { TEXT("/Game/People/Nevada/SM_StripStand_B"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplValets"), { { TEXT("/Game/People/Nevada/SM_Valets"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplWedding"), { { TEXT("/Game/People/Nevada/SM_Wedding"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplGamblers"), { { TEXT("/Game/People/Nevada/SM_Gamblers"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplRimCoyotes"), { { TEXT("/Game/People/Nevada/SM_RimCoyotes"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplRimPosse"), { { TEXT("/Game/People/Nevada/SM_RimPosse"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplHorses"), { { TEXT("/Game/People/Nevada/SM_WildHorses"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplCattle"), { { TEXT("/Game/People/Nevada/SM_Cattle"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplCrewCam"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 0, 0, 0, 0, 1.f }, { TEXT("/Game/People/Crew/SM_CrewCamera_Desert"), 0, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplCrewClapA"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 0, 0, 0, 0, 1.f }, { TEXT("/Game/People/Crew/SM_CrewApplauseA_Desert"), 0, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplCrewClapB"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 0, 0, 0, 0, 1.f }, { TEXT("/Game/People/Crew/SM_CrewApplauseB_Desert"), 0, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplCrewStunt"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 0, 0, 0, 0, 1.f }, { TEXT("/Game/People/Crew/SM_CrewStunt_Desert"), 0, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplCrewVillage"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 0, 0, 0, 0, 1.f }, { TEXT("/Game/People/Crew/SM_CrewVillage_Desert"), 0, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplCrewDolly"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 0, 0, 0, 0, 1.f }, { TEXT("/Game/People/Crew/SM_CrewDolly_Desert"), 0, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplCrewCrane"), { { TEXT("/Game/People/Crew/SM_CrewCrane_Desert"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplCrewLights"), { { TEXT("/Game/People/Crew/SM_CrewLights_Desert"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplCrewSound"), { { TEXT("/Game/People/Crew/SM_CrewSound_Desert"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplCrewCatering"), { { TEXT("/Game/People/Crew/SM_CrewCatering_Desert"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplRoadhouseP"), { { TEXT("/Game/People/Nevada/SM_RoadhouseDeck"), 7.5f, 0, 0, 0, 1.f } });
Set(TEXT("PplDinerP"), { { TEXT("/Game/People/Nevada/SM_DinerDeck"), 7.5f, 0, 0, 0, 1.f } });
Set(TEXT("PplHitchAP"), { { TEXT("/Game/People/Nevada/SM_Hitchers_A"), 7.5f, 0, 0, 0, 1.f } });
Set(TEXT("PplHitchBP"), { { TEXT("/Game/People/Nevada/SM_Hitchers_B"), 7.5f, 0, 0, 0, 1.f } });
Set(TEXT("PplProspectP"), { { TEXT("/Game/People/Nevada/SM_Prospectors"), 7.5f, 0, 0, 0, 1.f } });
Set(TEXT("PplLizardsP"), { { TEXT("/Game/People/Nevada/SM_Lizards"), 7.5f, 0, 0, 0, 1.f } });
Set(TEXT("PplRidersP"), { { TEXT("/Game/People/Nevada/SM_Riders"), 7.5f, 0, 0, 0, 1.f } });
Set(TEXT("PplStandAP"), { { TEXT("/Game/People/Nevada/SM_StripStand_A"), 7.5f, 0, 0, 0, 1.f } });
Set(TEXT("PplStandBP"), { { TEXT("/Game/People/Nevada/SM_StripStand_B"), 7.5f, 0, 0, 0, 1.f } });
Set(TEXT("PplValetsP"), { { TEXT("/Game/People/Nevada/SM_Valets"), 7.5f, 0, 0, 0, 1.f } });
Set(TEXT("PplWeddingP"), { { TEXT("/Game/People/Nevada/SM_Wedding"), 7.5f, 0, 0, 0, 1.f } });
Set(TEXT("PplGamblersP"), { { TEXT("/Game/People/Nevada/SM_Gamblers"), 7.5f, 0, 0, 0, 1.f } });
Set(TEXT("PplCrewCamP"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 7.5f, 0, 0, 0, 1.f }, { TEXT("/Game/People/Crew/SM_CrewCamera_Desert"), 7.5f, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplCrewClapAP"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 7.5f, 0, 0, 0, 1.f }, { TEXT("/Game/People/Crew/SM_CrewApplauseA_Desert"), 7.5f, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplCrewClapBP"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 7.5f, 0, 0, 0, 1.f }, { TEXT("/Game/People/Crew/SM_CrewApplauseB_Desert"), 7.5f, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplCrewStuntP"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 7.5f, 0, 0, 0, 1.f }, { TEXT("/Game/People/Crew/SM_CrewStunt_Desert"), 7.5f, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplCrewVillageP"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 7.5f, 0, 0, 0, 1.f }, { TEXT("/Game/People/Crew/SM_CrewVillage_Desert"), 7.5f, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplCrewDollyP"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 7.5f, 0, 0, 0, 1.f }, { TEXT("/Game/People/Crew/SM_CrewDolly_Desert"), 7.5f, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplCrewCraneP"), { { TEXT("/Game/People/Crew/SM_CrewCrane_Desert"), 7.5f, 0, 0, 0, 1.f } });
Set(TEXT("PplCrewLightsP"), { { TEXT("/Game/People/Crew/SM_CrewLights_Desert"), 7.5f, 0, 0, 0, 1.f } });
Set(TEXT("PplCrewSoundP"), { { TEXT("/Game/People/Crew/SM_CrewSound_Desert"), 7.5f, 0, 0, 0, 1.f } });
Set(TEXT("PplCrewCateringP"), { { TEXT("/Game/People/Crew/SM_CrewCatering_Desert"), 7.5f, 0, 0, 0, 1.f } });

static const TCHAR* const PplSets[] = { TEXT("PplRoadhouse"), TEXT("PplDiner"), TEXT("PplHitchA"), TEXT("PplHitchB"), TEXT("PplProspect"), TEXT("PplLizards"), TEXT("PplRiders"), TEXT("PplStandA"), TEXT("PplStandB"), TEXT("PplValets"), TEXT("PplWedding"), TEXT("PplGamblers"), TEXT("PplRimCoyotes"), TEXT("PplRimPosse"), TEXT("PplHorses"), TEXT("PplCattle"), TEXT("PplCrewCam"), TEXT("PplCrewClapA"), TEXT("PplCrewClapB"), TEXT("PplCrewStunt"), TEXT("PplCrewVillage"), TEXT("PplCrewDolly"), TEXT("PplCrewCrane"), TEXT("PplCrewLights"), TEXT("PplCrewSound"), TEXT("PplCrewCatering"), TEXT("PplRoadhouseP"), TEXT("PplDinerP"), TEXT("PplHitchAP"), TEXT("PplHitchBP"), TEXT("PplProspectP"), TEXT("PplLizardsP"), TEXT("PplRidersP"), TEXT("PplStandAP"), TEXT("PplStandBP"), TEXT("PplValetsP"), TEXT("PplWeddingP"), TEXT("PplGamblersP"), TEXT("PplCrewCamP"), TEXT("PplCrewClapAP"), TEXT("PplCrewClapBP"), TEXT("PplCrewStuntP"), TEXT("PplCrewVillageP"), TEXT("PplCrewDollyP"), TEXT("PplCrewCraneP"), TEXT("PplCrewLightsP"), TEXT("PplCrewSoundP"), TEXT("PplCrewCateringP") };
struct FPplRow { int16 Slot; int8 Side; uint8 Set; float X, Scale; };
// slot, side (-1 left of travel, +1 right), set, metres from the centre line, scale
static const FPplRow PplRows[] = {
	{4,-1,39,27.6f,1.f}, {4,1,16,28.3f,1.25f}, {5,-1,45,26.9f,1.f}, {5,1,22,32.2f,1.25f}, {7,-1,47,27.1f,1.f}, {7,1,24,29.7f,1.25f}, {8,-1,43,27.6f,1.f},
	{8,1,19,28.3f,1.25f}, {9,-1,38,27.6f,1.f}, {9,-1,14,148.8f,1.f}, {9,1,18,28.3f,1.25f}, {10,1,6,42.0f,1.5f}, {11,-1,40,27.6f,1.f}, {11,1,20,28.3f,1.25f},
	{12,-1,32,28.5f,1.2f}, {12,1,15,106.8f,1.f}, {13,-1,29,28.8f,1.2f}, {13,1,1,28.3f,1.35f}, {14,1,6,42.0f,1.5f}, {15,1,14,146.9f,1.f}, {15,1,4,29.7f,1.35f},
	{16,-1,39,27.6f,1.f}, {17,-1,28,28.8f,1.2f}, {18,-1,15,100.3f,1.f}, {18,1,6,42.0f,1.5f}, {19,1,5,29.8f,1.35f}, {20,-1,32,28.5f,1.2f}, {21,-1,27,27.9f,1.2f},
	{21,-1,14,130.0f,1.f}, {21,1,3,29.3f,1.35f}, {22,1,6,42.0f,1.5f}, {23,1,4,29.7f,1.35f}, {24,-1,32,28.5f,1.2f}, {24,1,15,135.3f,1.f}, {25,-1,28,28.8f,1.2f},
	{26,1,12,33.1f,1.f}, {27,-1,12,33.1f,1.f}, {27,1,13,33.1f,1.f}, {28,-1,13,33.1f,1.f}, {29,1,12,33.1f,1.f}, {30,-1,14,140.2f,1.f}, {31,1,13,48.2f,1.f},
	{33,-1,12,48.2f,1.f}, {35,-1,14,152.9f,1.f}, {35,1,13,53.4f,1.f}, {36,-1,12,33.1f,1.f}, {36,1,13,33.1f,1.f}, {37,-1,13,33.1f,1.f}, {38,1,12,33.1f,1.f},
	{39,1,13,47.9f,1.f}, {40,-1,13,33.1f,1.f}, {41,1,12,33.1f,1.f}, {42,-1,12,33.1f,1.f}, {42,1,13,33.1f,1.f}, {44,1,12,33.1f,1.f}, {45,-1,12,33.1f,1.f},
	{45,1,13,33.1f,1.f}, {47,1,13,47.6f,1.f}, {49,-1,12,52.7f,1.f}, {50,-1,14,132.7f,1.f}, {51,1,13,50.4f,1.f}, {53,-1,12,49.1f,1.f}, {54,-1,0,28.1f,1.35f},
	{54,1,29,28.8f,1.2f}, {55,1,40,27.6f,1.f}, {56,-1,14,201.5f,1.f}, {56,-1,0,28.1f,1.35f}, {56,1,30,29.1f,1.2f}, {57,-1,5,29.8f,1.35f}, {58,-1,0,28.1f,1.35f},
	{58,1,28,28.8f,1.2f}, {59,1,32,28.5f,1.2f}, {60,-1,5,29.8f,1.35f}, {60,1,26,27.7f,1.2f}, {61,-1,3,29.3f,1.35f}, {61,1,27,27.9f,1.2f}, {62,-1,0,28.1f,1.35f},
	{62,1,29,28.8f,1.2f}, {62,1,14,202.3f,1.f}, {63,1,30,29.1f,1.2f}, {64,-1,0,28.1f,1.35f}, {64,1,30,29.1f,1.2f}, {65,-1,16,28.3f,1.25f}, {65,1,45,26.9f,1.f},
	{66,-1,17,28.3f,1.25f}, {66,1,44,30.7f,1.f}, {67,1,41,27.6f,1.f}, {68,-1,21,28.3f,1.25f}, {68,-1,14,175.8f,1.f}, {68,1,26,27.7f,1.2f}, {69,-1,1,28.3f,1.35f},
	{69,1,29,28.8f,1.2f}, {70,-1,0,28.1f,1.35f}, {70,1,29,28.8f,1.2f}, {71,1,30,29.1f,1.2f}, {72,-1,0,28.1f,1.35f}, {72,1,30,29.1f,1.2f}, {73,-1,5,29.8f,1.35f},
	{74,-1,0,28.1f,1.35f}, {74,1,28,28.8f,1.2f}, {74,1,14,215.0f,1.f}, {75,-1,18,28.3f,1.25f}, {75,1,46,28.7f,1.f}, {76,-1,5,29.8f,1.35f}, {76,1,26,27.7f,1.2f},
	{77,-1,3,29.3f,1.35f}, {77,1,27,27.9f,1.2f}, {78,-1,0,28.1f,1.35f}, {78,1,29,28.8f,1.2f}, {79,1,30,29.1f,1.2f}, {80,-1,14,229.5f,1.f}, {80,-1,0,28.1f,1.35f},
	{80,1,30,29.1f,1.2f}, {81,-1,5,29.8f,1.35f}, {81,1,45,26.9f,1.f}, {82,-1,0,28.1f,1.35f}, {82,1,28,28.8f,1.2f}, {83,1,32,28.5f,1.2f}, {84,-1,5,29.8f,1.35f},
	{84,1,26,27.7f,1.2f}, {85,-1,5,29.8f,1.35f}, {85,1,6,42.0f,1.5f}, {86,1,29,28.8f,1.2f}, {87,1,32,28.5f,1.2f}, {88,-1,4,29.7f,1.35f}, {89,-1,12,43.6f,1.f},
	{90,1,32,28.5f,1.2f}, {91,-1,15,95.3f,1.f}, {91,-1,6,42.0f,1.5f}, {91,1,31,29.3f,1.2f}, {92,-1,2,29.3f,1.35f}, {93,-1,6,29.0f,1.35f}, {94,-1,20,28.3f,1.25f},
	{94,1,30,29.1f,1.2f}, {95,1,12,40.6f,1.f}, {96,-1,23,27.4f,1.25f}, {96,1,43,27.6f,1.f}, {97,-1,16,28.3f,1.25f}, {97,1,44,30.7f,1.f}, {97,1,6,42.0f,1.5f},
	{98,-1,15,106.0f,1.f}, {98,-1,17,28.3f,1.25f}, {98,1,41,27.6f,1.f}, {99,1,32,28.5f,1.2f}, {100,-1,4,29.7f,1.35f}, {101,-1,12,47.9f,1.f}, {102,1,32,28.5f,1.2f},
	{103,-1,6,42.0f,1.5f}, {103,1,31,29.3f,1.2f}, {104,-1,2,29.3f,1.35f}, {105,-1,6,29.0f,1.35f}, {105,1,15,112.3f,1.f}, {106,-1,18,28.3f,1.25f}, {106,1,46,28.7f,1.f},
	{107,1,42,27.6f,1.f}, {107,1,12,43.0f,1.f}, {108,-1,6,29.0f,1.35f}, {108,1,41,27.6f,1.f}, {109,-1,5,29.8f,1.35f}, {109,1,6,42.0f,1.5f}, {110,1,29,28.8f,1.2f},
	{111,1,32,28.5f,1.2f}, {112,-1,4,29.7f,1.35f}, {112,1,15,113.4f,1.f}, {113,-1,12,46.8f,1.f}, {114,1,13,33.1f,1.f}, {116,1,12,33.1f,1.f}, {119,1,12,33.1f,1.f},
	{120,-1,12,33.1f,1.f}, {120,1,13,33.1f,1.f}, {121,-1,12,50.5f,1.f}, {123,1,13,53.7f,1.f}, {125,1,12,33.1f,1.f}, {126,-1,12,33.1f,1.f}, {126,1,13,33.1f,1.f},
	{127,-1,13,33.1f,1.f}, {129,-1,12,33.1f,1.f}, {130,-1,13,33.1f,1.f}, {132,-1,12,33.1f,1.f}, {133,-1,13,33.1f,1.f}, {134,1,12,33.1f,1.f}, {135,-1,12,33.1f,1.f},
	{135,1,13,33.1f,1.f}, {136,-1,13,33.1f,1.f}, {137,1,12,33.1f,1.f}, {138,-1,12,33.1f,1.f}, {138,1,13,33.1f,1.f}, {139,-1,13,33.1f,1.f}, {140,-1,31,29.3f,1.2f},
	{140,1,1,28.3f,1.35f}, {141,-1,30,29.1f,1.2f}, {141,1,1,28.3f,1.35f}, {142,1,6,29.0f,1.35f}, {143,1,2,29.3f,1.35f}, {145,-1,30,29.1f,1.2f}, {146,-1,44,30.7f,1.f},
	{146,1,3,29.3f,1.35f}, {147,1,6,29.0f,1.35f}, {148,-1,15,131.8f,1.f}, {148,-1,31,29.3f,1.2f}, {148,1,1,28.3f,1.35f}, {149,-1,27,27.9f,1.2f}, {149,1,4,29.7f,1.35f},
	{150,1,6,29.0f,1.35f}, {152,-1,27,27.9f,1.2f}, {152,1,2,29.3f,1.35f}, {153,-1,30,29.1f,1.2f}, {154,1,3,29.3f,1.35f}, {155,-1,38,27.6f,1.f}, {155,1,23,27.4f,1.25f},
	{156,-1,41,27.6f,1.f}, {156,1,22,32.2f,1.25f}, {157,-1,15,140.5f,1.f}, {157,-1,39,27.6f,1.f}, {157,1,21,28.3f,1.25f}, {159,1,18,28.3f,1.25f}, {160,-1,27,27.9f,1.2f},
	{160,1,2,29.3f,1.35f}, {161,-1,30,29.1f,1.2f}, {162,1,3,29.3f,1.35f}, {163,1,6,29.0f,1.35f}, {164,-1,31,29.3f,1.2f}, {164,1,1,28.3f,1.35f}, {166,1,15,115.7f,1.f},
	{166,1,18,28.3f,1.25f}, {167,-1,46,28.7f,1.f}, {167,1,2,29.3f,1.35f}, {168,-1,27,27.9f,1.2f}, {168,1,2,29.3f,1.35f}, {169,-1,30,29.1f,1.2f}, {170,1,3,29.3f,1.35f},
	{171,1,6,29.0f,1.35f}, {172,-1,21,28.3f,1.25f}, {172,1,36,28.9f,1.2f}, {173,-1,10,29.4f,1.35f}, {173,1,27,27.9f,1.2f}, {175,-1,11,29.2f,1.35f}, {175,1,33,27.6f,1.2f},
	{176,-1,22,32.2f,1.25f}, {176,1,38,27.6f,1.f}, {177,-1,8,43.0f,1.5f}, {178,-1,10,29.4f,1.35f}, {178,1,45,26.9f,1.f}, {179,-1,9,29.3f,1.35f}, {179,1,36,28.9f,1.2f},
	{181,-1,1,28.3f,1.35f}, {181,1,37,28.7f,1.2f}, {182,-1,8,27.9f,1.35f}, {182,1,35,28.8f,1.2f}, {184,-1,11,29.2f,1.35f}, {184,1,36,28.9f,1.2f}, {185,-1,10,29.4f,1.35f},
	{185,1,45,26.9f,1.f}, {187,-1,11,29.2f,1.35f}, {187,1,33,27.6f,1.2f}, {188,-1,9,29.3f,1.35f}, {188,1,34,27.6f,1.2f}, {189,-1,8,43.0f,1.5f}, {190,-1,10,29.4f,1.35f},
	{190,1,37,28.7f,1.2f}, {191,-1,9,29.3f,1.35f}, {191,1,40,27.6f,1.f}, {193,-1,7,27.9f,1.35f}, {193,1,37,28.7f,1.2f}, {194,-1,8,27.9f,1.35f}, {194,1,35,28.8f,1.2f},
	{196,-1,11,29.2f,1.35f}, {196,1,36,28.9f,1.2f}, {197,-1,1,28.3f,1.35f}, {197,1,35,28.8f,1.2f}, {199,-1,20,28.3f,1.25f}, {199,1,33,27.6f,1.2f}, {200,-1,9,29.3f,1.35f},
	{200,1,34,27.6f,1.2f}, {201,-1,35,28.8f,1.2f}, {201,1,11,29.2f,1.35f}, {202,-1,34,27.6f,1.2f}, {202,1,10,29.4f,1.35f}, {203,-1,35,28.8f,1.2f}, {203,1,8,27.9f,1.35f},
	{204,-1,37,28.7f,1.2f}, {204,-1,7,43.0f,1.5f}, {204,1,2,29.3f,1.35f}, {205,-1,33,27.6f,1.2f}, {205,1,1,28.3f,1.35f}, {206,-1,37,28.7f,1.2f}, {206,1,9,29.3f,1.35f},
	{207,-1,34,27.6f,1.2f}, {207,1,10,29.4f,1.35f}, {208,-1,7,43.0f,1.5f}, {208,-1,34,27.6f,1.2f}, {208,1,9,29.3f,1.35f}, {209,-1,43,27.6f,1.f}, {209,1,22,32.2f,1.25f},
	{210,-1,45,26.9f,1.f}, {210,1,16,28.3f,1.25f}, {211,-1,41,27.6f,1.f}, {211,1,17,28.3f,1.25f}, {212,-1,7,43.0f,1.5f}, {212,-1,36,28.9f,1.2f}, {212,1,19,28.3f,1.25f},
	{213,-1,27,27.9f,1.2f}, {213,1,9,29.3f,1.35f}, {214,-1,28,28.8f,1.2f}, {214,1,11,29.2f,1.35f}, {215,-1,33,27.6f,1.2f}, {215,1,1,28.3f,1.35f}, {216,-1,7,43.0f,1.5f},
	{216,-1,35,28.8f,1.2f}, {216,1,11,29.2f,1.35f}, {217,-1,36,28.9f,1.2f}, {217,1,8,27.9f,1.35f}, {218,-1,35,28.8f,1.2f}, {218,1,8,27.9f,1.35f}, {219,-1,28,28.8f,1.2f},
	{219,1,11,29.2f,1.35f}, {220,-1,27,27.9f,1.2f}, {220,-1,7,43.0f,1.5f}, {220,1,7,27.9f,1.35f}, {221,-1,35,28.8f,1.2f}, {221,1,1,28.3f,1.35f}, {222,-1,34,27.6f,1.2f},
	{222,1,10,29.4f,1.35f}, {223,-1,35,28.8f,1.2f}, {223,1,8,27.9f,1.35f}, {224,-1,39,27.6f,1.f}, {224,-1,7,43.0f,1.5f}, {224,1,2,29.3f,1.35f}, {225,-1,33,27.6f,1.2f},
	{225,1,1,28.3f,1.35f}, {226,-1,37,28.7f,1.2f}, {226,1,9,29.3f,1.35f}, {227,-1,34,27.6f,1.2f}, {227,1,10,29.4f,1.35f}, {228,-1,42,27.6f,1.f}, {228,-1,7,43.0f,1.5f},
	{228,1,25,27.7f,1.25f}, {229,-1,38,27.6f,1.f}, {229,1,22,32.2f,1.25f}, {230,-1,45,26.9f,1.f}, {230,1,18,28.3f,1.25f}, {231,-1,43,27.6f,1.f}, {231,1,24,29.7f,1.25f},
	{232,-1,39,27.6f,1.f}, {232,-1,7,43.0f,1.5f}, {232,1,19,28.3f,1.25f}, {233,-1,34,27.6f,1.2f}, {233,1,9,29.3f,1.35f}, {234,-1,28,28.8f,1.2f}, {234,1,11,29.2f,1.35f},
	{235,-1,33,27.6f,1.2f}, {235,1,1,28.3f,1.35f}, {236,-1,7,43.0f,1.5f}, {236,-1,35,28.8f,1.2f}, {236,1,11,29.2f,1.35f}, {237,-1,29,28.8f,1.2f}, {237,1,16,28.3f,1.25f},
	{238,1,15,148.4f,1.f}, {238,1,6,42.0f,1.5f}, {239,-1,32,28.5f,1.2f}, {240,-1,32,28.5f,1.2f}, {240,1,14,157.0f,1.f}, {241,-1,28,28.8f,1.2f}, {242,1,15,103.9f,1.f},
	{242,1,4,29.7f,1.35f}, {243,1,6,42.0f,1.5f}, {244,1,14,147.6f,1.f}, {244,1,6,29.0f,1.35f}, {245,-1,32,28.5f,1.2f}, {246,1,15,109.5f,1.f}, {246,1,5,29.8f,1.35f},
	{247,1,3,29.3f,1.35f}, {248,-1,6,42.0f,1.5f}, {248,1,14,139.8f,1.f}, {249,1,6,29.0f,1.35f}, {250,-1,44,30.7f,1.f}, {250,1,15,129.5f,1.f}, {250,1,6,29.0f,1.35f},
	{251,1,2,29.3f,1.35f}, {252,-1,30,29.1f,1.2f}, {252,1,14,187.4f,1.f}, {253,-1,6,42.0f,1.5f}, {254,-1,32,28.5f,1.2f}, {254,1,15,116.8f,1.f}, {255,1,6,29.0f,1.35f},
	{256,-1,31,29.3f,1.2f}, {256,1,14,152.8f,1.f}, {257,-1,29,28.8f,1.2f}, {258,1,15,126.5f,1.f}, {258,1,6,42.0f,1.5f}, {259,-1,32,28.5f,1.2f}, {260,-1,14,175.4f,1.f},
	{260,-1,32,28.5f,1.2f}, {261,-1,28,28.8f,1.2f}, {262,-1,15,117.4f,1.f}, {262,1,4,29.7f,1.35f}, {263,1,6,42.0f,1.5f}, {264,-1,14,199.8f,1.f}, {264,1,18,28.3f,1.25f},
	{265,-1,32,28.5f,1.2f}, {266,-1,15,143.4f,1.f}, {266,1,5,29.8f,1.35f}, {267,1,3,29.3f,1.35f}, {268,-1,14,122.3f,1.f}, {268,-1,6,42.0f,1.5f}, {269,1,6,29.0f,1.35f},
	{270,-1,15,110.2f,1.f}, {270,1,6,29.0f,1.35f}, {271,1,2,29.3f,1.35f}, {272,-1,14,159.7f,1.f}, {272,-1,30,29.1f,1.2f}, {273,-1,6,42.0f,1.5f}, {274,-1,15,140.1f,1.f},
	{274,-1,32,28.5f,1.2f}, {275,1,6,29.0f,1.35f}, {276,-1,43,27.6f,1.f}, {276,-1,14,119.8f,1.f}, {277,-1,29,28.8f,1.2f}, {278,-1,15,131.1f,1.f}, {278,1,6,42.0f,1.5f},
	{279,-1,32,28.5f,1.2f}, {280,-1,14,159.1f,1.f}, {280,-1,32,28.5f,1.2f}, {281,-1,28,28.8f,1.2f}, {282,-1,15,117.3f,1.f}, {282,1,4,29.7f,1.35f}, {283,1,6,42.0f,1.5f},
	{284,-1,14,176.4f,1.f}, {284,1,6,29.0f,1.35f}, {285,-1,32,28.5f,1.2f}, {286,-1,15,115.0f,1.f}, {286,1,5,29.8f,1.35f}, {287,1,3,29.3f,1.35f}, {288,-1,14,171.7f,1.f},
	{288,-1,6,42.0f,1.5f}, {289,1,23,27.4f,1.25f}, {290,-1,15,143.4f,1.f}, {290,1,6,29.0f,1.35f}, {291,1,2,29.3f,1.35f}, {292,-1,14,157.1f,1.f}, {292,-1,30,29.1f,1.2f},
	{293,-1,6,42.0f,1.5f}, {294,-1,15,134.7f,1.f}, {294,-1,32,28.5f,1.2f}, {295,1,6,29.0f,1.35f}, {296,-1,14,151.9f,1.f}, {296,-1,31,29.3f,1.2f}, {297,-1,29,28.8f,1.2f},
	{298,-1,15,124.8f,1.f}, {298,1,6,42.0f,1.5f}, {299,-1,32,28.5f,1.2f}, {300,-1,14,184.9f,1.f}, {300,-1,32,28.5f,1.2f}, {301,-1,28,28.8f,1.2f}, {302,-1,15,139.1f,1.f},
	{302,-1,42,27.6f,1.f}, {302,1,4,29.7f,1.35f}, {303,1,6,42.0f,1.5f}, {304,-1,14,168.2f,1.f}, {304,1,6,29.0f,1.35f}, {305,-1,32,28.5f,1.2f}, {306,-1,15,157.3f,1.f},
	{306,1,5,29.8f,1.35f}, {307,1,3,29.3f,1.35f}, {308,-1,14,153.0f,1.f}, {308,-1,6,42.0f,1.5f}, {309,1,6,29.0f,1.35f}, {310,-1,15,105.5f,1.f}, {310,1,6,29.0f,1.35f},
	{311,1,2,29.3f,1.35f}, {312,-1,14,167.6f,1.f}, {312,-1,30,29.1f,1.2f}, {313,-1,6,42.0f,1.5f}, {314,-1,15,150.0f,1.f}, {314,-1,32,28.5f,1.2f}, {315,1,19,28.3f,1.25f},
	{316,-1,14,151.9f,1.f}, {316,-1,31,29.3f,1.2f}, {317,-1,29,28.8f,1.2f}, {318,-1,15,139.5f,1.f}, {318,1,6,42.0f,1.5f}, {319,-1,32,28.5f,1.2f}, {320,-1,14,157.8f,1.f},
	{320,-1,32,28.5f,1.2f}, {321,1,13,65.2f,1.15f}, {322,1,6,29.0f,1.35f}, {324,1,5,29.8f,1.35f}, {325,-1,14,96.6f,1.f}, {325,-1,12,69.6f,1.15f}, {326,-1,2,29.3f,1.35f},
	{327,-1,15,98.5f,1.f}, {327,-1,13,67.0f,1.15f}, {328,-1,17,28.3f,1.25f}, {328,1,4,29.7f,1.35f}, {330,1,14,113.0f,1.f}, {330,1,6,29.0f,1.35f}, {331,1,12,66.6f,1.15f},
	{332,-1,5,29.8f,1.35f}, {333,1,13,64.7f,1.15f}, {334,1,3,29.3f,1.35f}, {335,1,14,113.9f,1.f}, {336,1,4,29.7f,1.35f}, {337,-1,12,63.0f,1.15f}, {337,1,15,115.7f,1.f},
	{338,-1,6,29.0f,1.35f}, {339,-1,13,65.7f,1.15f}, {340,-1,14,105.4f,1.f}, {340,-1,5,29.8f,1.35f}, {342,1,16,28.3f,1.25f}, {343,1,12,64.2f,1.15f}, {344,-1,4,29.7f,1.35f},
	{345,-1,14,108.6f,1.f}, {345,1,13,69.9f,1.15f}, {346,1,6,29.0f,1.35f}, {347,-1,15,100.4f,1.f}, {348,1,5,29.8f,1.35f}, {349,-1,12,69.1f,1.15f}, {350,-1,2,29.3f,1.35f},
	{350,1,14,107.1f,1.f}, {351,-1,13,68.2f,1.15f}, {352,-1,4,29.7f,1.35f}, {354,1,6,29.0f,1.35f}, {355,1,14,98.6f,1.f}, {355,1,12,69.9f,1.15f}, {356,-1,22,32.2f,1.25f},
	{357,1,15,111.1f,1.f}, {357,1,13,66.4f,1.15f}, {358,1,3,29.3f,1.35f}, {360,-1,14,108.7f,1.f}, {360,1,4,29.7f,1.35f}, {361,-1,12,63.0f,1.15f}, {362,-1,6,29.0f,1.35f},
	{363,-1,13,67.2f,1.15f}, {364,-1,5,29.8f,1.35f}, {365,-1,14,106.7f,1.f}, {366,1,3,29.3f,1.35f}, {367,-1,15,97.5f,1.f}, {367,1,12,63.8f,1.15f}, {368,-1,4,29.7f,1.35f},
	{369,1,13,63.4f,1.15f}, {370,1,14,115.7f,1.f}, {370,1,6,29.0f,1.35f}, {372,1,5,29.8f,1.35f}, {373,-1,12,66.0f,1.15f}, {374,-1,2,29.3f,1.35f}, {375,-1,13,63.5f,1.15f},
	{375,1,14,107.3f,1.f}, {376,-1,4,29.7f,1.35f}, {377,1,15,107.9f,1.f}, {378,1,6,29.0f,1.35f}, {379,1,12,65.6f,1.15f}, {380,-1,21,28.3f,1.25f}, {380,-1,14,108.7f,1.f},
	{381,1,13,65.8f,1.15f}, {382,1,3,29.3f,1.35f}, {384,1,4,29.7f,1.35f}, {385,-1,14,98.0f,1.f}, {385,-1,12,62.3f,1.15f}, {386,-1,6,29.0f,1.35f}, {387,-1,15,102.5f,1.f},
	{387,-1,13,64.9f,1.15f}, {388,-1,5,29.8f,1.35f}, {390,1,3,29.3f,1.35f}, {390,1,14,111.8f,1.f}, {391,1,12,66.4f,1.15f}, {392,-1,4,29.7f,1.35f}, {393,1,13,64.8f,1.15f},
	{394,-1,6,29.0f,1.35f}, {394,1,23,27.4f,1.25f}, {395,1,14,104.6f,1.f}, {396,1,5,29.8f,1.35f}, {397,-1,12,63.1f,1.15f}, {397,1,15,112.4f,1.f}, {398,-1,2,29.3f,1.35f},
	{399,-1,13,67.6f,1.15f}, {400,-1,14,100.1f,1.f}, {400,-1,4,29.7f,1.35f}, {402,1,6,29.0f,1.35f}, {403,1,12,68.4f,1.15f}, {404,-1,5,29.8f,1.35f}, {405,-1,14,114.8f,1.f},
	{405,1,13,68.6f,1.15f}, {406,-1,20,28.3f,1.25f}, {406,1,3,29.3f,1.35f}, {407,-1,15,105.4f,1.f}, {408,1,4,29.7f,1.35f}, {409,-1,12,49.9f,1.f}, {411,-1,12,33.1f,1.f},
	{411,1,13,33.1f,1.f}, {412,-1,13,33.1f,1.f}, {413,1,12,33.1f,1.f}, {415,-1,13,33.1f,1.f}, {416,1,12,33.1f,1.f}, {417,-1,12,33.1f,1.f}, {417,1,13,33.1f,1.f},
	{418,-1,13,33.1f,1.f}, {419,1,12,33.1f,1.f}, {420,-1,12,33.1f,1.f}, {420,1,13,33.1f,1.f}, {422,1,12,33.1f,1.f}, {423,-1,12,33.1f,1.f}, {423,1,13,33.1f,1.f},
	{424,-1,13,33.1f,1.f}, {425,1,12,33.1f,1.f}, {426,-1,12,33.1f,1.f}, {426,1,13,33.1f,1.f}, {427,-1,13,33.1f,1.f}, {429,-1,12,46.8f,1.f}, {431,1,13,49.1f,1.f},
	{433,-1,12,48.5f,1.f}, {435,1,13,52.2f,1.f}, {437,-1,12,49.7f,1.f}, {439,1,13,46.2f,1.f}, {441,-1,12,33.1f,1.f}, {441,1,13,33.1f,1.f}, {443,1,12,33.1f,1.f},
	{444,-1,12,33.1f,1.f}, {444,1,13,33.1f,1.f}, {445,-1,13,33.1f,1.f}, {446,1,12,33.1f,1.f}, {447,-1,12,33.1f,1.f}, {447,1,13,33.1f,1.f}, {448,-1,13,33.1f,1.f},
	{449,1,12,33.1f,1.f}, {450,-1,12,33.1f,1.f}, {450,1,13,33.1f,1.f}, {451,-1,13,33.1f,1.f}, {452,1,12,33.1f,1.f}, {453,-1,12,33.1f,1.f}, {453,1,13,33.1f,1.f},
	{454,-1,13,33.1f,1.f}, {455,1,12,33.1f,1.f}, {457,-1,13,33.1f,1.f}, {459,1,13,52.7f,1.f}, {461,-1,12,46.8f,1.f}, {463,1,13,52.7f,1.f}, {465,-1,12,48.6f,1.f},
	{467,1,13,51.3f,1.f}, {469,-1,12,50.1f,1.f}, {471,1,13,52.2f,1.f}, {472,-1,13,33.1f,1.f}, {473,1,12,33.1f,1.f}, {474,-1,12,33.1f,1.f}, {474,1,13,33.1f,1.f},
	{475,-1,13,33.1f,1.f}, {476,1,12,33.1f,1.f}, {477,-1,12,33.1f,1.f}, {478,-1,13,33.1f,1.f}, {479,1,12,33.1f,1.f}, {480,-1,12,33.1f,1.f}, {480,1,13,33.1f,1.f},
	{481,-1,13,33.1f,1.f}, {482,1,12,33.1f,1.f}, {483,-1,12,33.1f,1.f}, {485,1,12,33.1f,1.f}, {486,-1,12,33.1f,1.f}, {487,-1,8,27.9f,1.35f}, {487,1,37,28.7f,1.2f},
	{488,-1,7,27.9f,1.35f}, {488,1,35,28.8f,1.2f}, {489,-1,8,44.0f,1.55f}, {489,-1,10,29.4f,1.35f}, {489,1,37,28.7f,1.2f}, {490,-1,1,28.3f,1.35f}, {490,1,33,27.6f,1.2f},
	{491,-1,10,29.4f,1.35f}, {491,1,35,28.8f,1.2f}, {492,-1,11,29.2f,1.35f}, {492,1,34,27.6f,1.2f}, {493,-1,7,27.9f,1.35f}, {493,1,27,27.9f,1.2f}, {494,-1,11,29.2f,1.35f},
	{494,1,36,28.9f,1.2f}, {495,-1,7,27.9f,1.35f}, {495,1,27,27.9f,1.2f}, {496,-1,9,29.3f,1.35f}, {496,1,36,28.9f,1.2f}, {497,-1,11,29.2f,1.35f}, {497,-1,8,44.0f,1.55f},
	{497,1,45,26.9f,1.f}, {498,-1,9,29.3f,1.35f}, {498,1,33,27.6f,1.2f}, {499,-1,11,29.2f,1.35f}, {499,1,36,28.9f,1.2f}, {500,-1,1,28.3f,1.35f}, {500,1,33,27.6f,1.2f},
	{501,-1,1,28.3f,1.35f}, {501,1,36,28.9f,1.2f}, {502,-1,8,27.9f,1.35f}, {502,1,37,28.7f,1.2f}, {503,-1,9,29.3f,1.35f}, {503,1,33,27.6f,1.2f}, {504,-1,10,29.4f,1.35f},
	{504,1,37,28.7f,1.2f}, {505,-1,7,27.9f,1.35f}, {505,-1,8,44.0f,1.55f}, {505,1,27,27.9f,1.2f}, {506,-1,10,29.4f,1.35f}, {506,1,35,28.8f,1.2f}, {507,-1,8,27.9f,1.35f},
	{507,1,37,28.7f,1.2f}, {508,-1,7,27.9f,1.35f}, {508,1,35,28.8f,1.2f}, {509,-1,10,29.4f,1.35f}, {509,1,27,27.9f,1.2f}, {510,-1,20,28.3f,1.25f}, {510,1,33,27.6f,1.2f},
	{511,-1,10,29.4f,1.35f}, {511,1,35,28.8f,1.2f}, {512,-1,11,29.2f,1.35f}, {512,1,34,27.6f,1.2f}, {513,-1,7,27.9f,1.35f}, {513,-1,8,44.0f,1.55f}, {513,1,35,28.8f,1.2f},
	{514,-1,11,29.2f,1.35f}, {514,1,36,28.9f,1.2f}, {515,-1,7,27.9f,1.35f}, {515,1,27,27.9f,1.2f}, {516,-1,9,29.3f,1.35f}, {516,1,36,28.9f,1.2f}, {517,-1,1,28.3f,1.35f},
	{517,1,34,27.6f,1.2f}, {518,-1,9,29.3f,1.35f}, {518,1,33,27.6f,1.2f}, {519,-1,11,29.2f,1.35f}, {519,1,36,28.9f,1.2f}, {520,-1,1,28.3f,1.35f}, {520,1,33,27.6f,1.2f},
	{521,-1,8,44.0f,1.55f}, {521,-1,9,29.3f,1.35f}, {521,1,36,28.9f,1.2f}, {522,-1,8,27.9f,1.35f}, {522,1,37,28.7f,1.2f}, {523,-1,9,29.3f,1.35f}, {523,1,41,27.6f,1.f},
	{524,-1,10,29.4f,1.35f}, {524,1,37,28.7f,1.2f}, {525,-1,7,27.9f,1.35f}, {525,1,27,27.9f,1.2f}, {526,-1,10,29.4f,1.35f}, {526,1,35,28.8f,1.2f}, {527,-1,8,27.9f,1.35f},
	{527,1,37,28.7f,1.2f}, {528,-1,7,27.9f,1.35f}, {528,1,35,28.8f,1.2f}, {529,-1,8,44.0f,1.55f}, {529,-1,10,29.4f,1.35f}, {529,1,37,28.7f,1.2f}, {530,-1,1,28.3f,1.35f},
	{530,1,33,27.6f,1.2f}, {531,-1,10,29.4f,1.35f}, {531,1,35,28.8f,1.2f}, {532,-1,11,29.2f,1.35f}, {532,1,34,27.6f,1.2f}, {533,-1,1,28.3f,1.35f}, {533,1,35,28.8f,1.2f},
	{534,-1,11,29.2f,1.35f}, {534,1,36,28.9f,1.2f}, {535,-1,7,27.9f,1.35f}, {535,1,27,27.9f,1.2f}, {536,-1,17,28.3f,1.25f}, {536,1,36,28.9f,1.2f}, {537,-1,11,29.2f,1.35f},
	{537,-1,8,44.0f,1.55f}, {537,1,34,27.6f,1.2f}, {538,-1,9,29.3f,1.35f}, {538,1,33,27.6f,1.2f}, {539,-1,11,29.2f,1.35f}, {539,1,36,28.9f,1.2f}, {540,-1,1,28.3f,1.35f},
	{540,1,33,27.6f,1.2f}, {541,-1,9,29.3f,1.35f}, {541,1,27,27.9f,1.2f}, {542,-1,8,27.9f,1.35f}, {542,1,37,28.7f,1.2f}, {543,-1,9,29.3f,1.35f}, {543,1,33,27.6f,1.2f},
	{544,-1,10,29.4f,1.35f}, {544,1,37,28.7f,1.2f}, {545,-1,7,27.9f,1.35f}, {545,-1,8,44.0f,1.55f}, {545,1,27,27.9f,1.2f}, {546,-1,10,29.4f,1.35f}, {546,1,35,28.8f,1.2f},
	{547,-1,8,27.9f,1.35f}, {547,1,37,28.7f,1.2f}, {548,-1,7,27.9f,1.35f}, {548,1,35,28.8f,1.2f}, {549,-1,1,28.3f,1.35f}, {549,1,38,27.6f,1.f}, {550,-1,1,28.3f,1.35f},
	{550,1,33,27.6f,1.2f}, {551,-1,10,29.4f,1.35f}, {551,1,35,28.8f,1.2f}, {552,-1,11,29.2f,1.35f}, {552,1,34,27.6f,1.2f}, {553,-1,7,27.9f,1.35f}, {553,-1,8,44.0f,1.55f},
	{553,1,35,28.8f,1.2f}, {554,-1,11,29.2f,1.35f}, {554,1,36,28.9f,1.2f}, {555,-1,7,27.9f,1.35f}, {555,1,27,27.9f,1.2f}, {556,-1,9,29.3f,1.35f}, {556,1,36,28.9f,1.2f},
	{557,-1,11,29.2f,1.35f}, {557,1,27,27.9f,1.2f}, {558,-1,16,28.3f,1.25f}, {558,1,39,27.6f,1.f}, {559,-1,11,29.2f,1.35f}, {559,1,36,28.9f,1.2f}, {560,-1,18,28.3f,1.25f},
	{560,1,44,30.7f,1.f}, {561,-1,8,44.0f,1.55f}, {561,-1,9,29.3f,1.35f}, {561,1,36,28.9f,1.2f}, {562,-1,22,32.2f,1.25f}, {562,1,37,28.7f,1.2f}, {563,-1,9,29.3f,1.35f},
	{563,1,33,27.6f,1.2f}, {564,-1,10,29.4f,1.35f}, {564,1,37,28.7f,1.2f},
};
for (const FPplRow& R : PplRows)
{
	Zone(TEXT("People"), (R.Slot - 0.4f) * 0.045243f, (R.Slot + 0.4f) * 0.045243f);
	Site(PplSets[R.Set], R.Side, R.X, 1, 0, 1.f, R.Scale);
}

{
// ---- BEGIN far (ft_people_nevada_far.py) ----
// The far band and the running creatures, on the OPEN stretches only (inside the rock cuts of the dry wash, Red Rock
// Pass and the red rock return nothing beyond 45 m can be seen). Every piece is a long one laid along the road (yaw
// jitter 5 degrees), each herd on the pale swell it brings. Herds: 140 to 195 m at their own size (3.3 to 3.8 times
// life), or 215 to 275 m at 1.4 times that. Riders on their ridge: 255 to 285 m. SM_FarRunners is the one middle-
// distance piece (38 to 48 m, TALL: a 2.4 m rock ledge). A side at a time, so the two sides never show the same thing.
Zone(TEXT("People far: state line"), 0.10f, 1.15f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarMustangs"), 140.f, 195.f, 8, 2, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarPronghorns"), 140.f, 195.f, 8, 6, 1.00f, 1.00f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRidgeRiders"), 255.f, 285.f, 12, 4, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRidgeRiders"), 255.f, 285.f, 12, 10, 1.00f, 1.00f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRunners"), 38.f, 46.f, 14, 3, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRunners"), 38.f, 46.f, 14, 10, 1.00f, 1.00f, 0.08f, 1, 5.f);
Zone(TEXT("People far: the wash, between its cuts"), 1.34f, 1.60f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarCattleDrive"), 140.f, 195.f, 6, 1, 1.00f, 1.00f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRidgeRiders"), 255.f, 285.f, 6, 4, 1.00f, 1.00f, 0.08f, -1, 5.f);
Zone(TEXT("People far: the wash, its east end"), 2.07f, 2.43f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarMustangs"), 140.f, 195.f, 8, 3, 1.00f, 1.00f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarPronghorns"), 140.f, 195.f, 8, 6, 1.00f, 1.00f, 0.08f, -1, 5.f);
Zone(TEXT("People far: snake eyes"), 2.43f, 3.83f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarCattleDrive"), 215.f, 272.f, 10, 0, 1.00f, 1.40f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarWagonTrain"), 215.f, 272.f, 10, 4, 1.00f, 1.40f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarPronghorns"), 185.f, 215.f, 12, 7, 1.00f, 1.00f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarMustangs"), 185.f, 215.f, 12, 9, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRunners"), 38.f, 46.f, 20, 2, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRunners"), 38.f, 46.f, 20, 12, 1.00f, 1.00f, 0.08f, 1, 5.f);
Zone(TEXT("People far: joshua flats"), 3.83f, 5.12f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarMustangs"), 140.f, 195.f, 8, 1, 1.00f, 1.00f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarPronghorns"), 140.f, 180.f, 8, 4, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarWagonTrain"), 215.f, 272.f, 10, 8, 1.00f, 1.40f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRidgeRiders"), 258.f, 285.f, 10, 6, 1.00f, 1.00f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRunners"), 38.f, 46.f, 20, 5, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRunners"), 38.f, 46.f, 20, 15, 1.00f, 1.00f, 0.08f, 1, 5.f);
Zone(TEXT("People far: last chance gas"), 6.32f, 7.75f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarWagonTrain"), 195.f, 235.f, 10, 2, 1.00f, 1.20f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarCattleDrive"), 195.f, 245.f, 10, 6, 1.00f, 1.20f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarMustangs"), 235.f, 275.f, 16, 9, 1.00f, 1.40f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarMustangs"), 235.f, 275.f, 16, 1, 1.00f, 1.40f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRunners"), 38.f, 46.f, 20, 4, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRunners"), 38.f, 46.f, 20, 14, 1.00f, 1.00f, 0.08f, 1, 5.f);
Zone(TEXT("People far: jackpot junction"), 7.75f, 9.05f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRidgeRiders"), 262.f, 288.f, 12, 2, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRidgeRiders"), 262.f, 288.f, 12, 8, 1.00f, 1.00f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarPronghorns"), 250.f, 280.f, 16, 6, 1.00f, 1.40f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarPronghorns"), 250.f, 280.f, 16, 14, 1.00f, 1.40f, 0.08f, 1, 5.f);
Zone(TEXT("People far: city lights"), 9.05f, 10.69f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarWagonTrain"), 250.f, 280.f, 16, 3, 1.00f, 1.40f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarWagonTrain"), 250.f, 280.f, 16, 11, 1.00f, 1.40f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarMustangs"), 250.f, 280.f, 18, 8, 1.00f, 1.40f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarMustangs"), 250.f, 280.f, 18, 17, 1.00f, 1.40f, 0.08f, 1, 5.f);
Zone(TEXT("People far: salt flats"), 10.69f, 14.50f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarMustangs"), 140.f, 195.f, 12, 1, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarMustangs"), 140.f, 195.f, 12, 7, 1.00f, 1.00f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarPronghorns"), 140.f, 195.f, 14, 4, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarPronghorns"), 140.f, 195.f, 14, 11, 1.00f, 1.00f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarWagonTrain"), 215.f, 272.f, 22, 3, 1.00f, 1.40f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarWagonTrain"), 215.f, 272.f, 22, 14, 1.00f, 1.40f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarCattleDrive"), 215.f, 272.f, 20, 9, 1.00f, 1.40f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarCattleDrive"), 215.f, 272.f, 20, 19, 1.00f, 1.40f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRidgeRiders"), 262.f, 288.f, 16, 13, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRidgeRiders"), 262.f, 288.f, 16, 5, 1.00f, 1.00f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRunners"), 38.f, 46.f, 26, 2, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRunners"), 38.f, 46.f, 26, 15, 1.00f, 1.00f, 0.08f, 1, 5.f);
Zone(TEXT("People far: syndicate range"), 14.50f, 18.50f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarCattleDrive"), 185.f, 235.f, 18, 2, 1.00f, 1.20f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarCattleDrive"), 185.f, 235.f, 18, 11, 1.00f, 1.20f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRidgeRiders"), 255.f, 288.f, 14, 4, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRidgeRiders"), 255.f, 288.f, 14, 11, 1.00f, 1.00f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarMustangs"), 205.f, 265.f, 20, 5, 1.00f, 1.40f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarMustangs"), 205.f, 265.f, 20, 15, 1.00f, 1.40f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarPronghorns"), 160.f, 200.f, 22, 9, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarPronghorns"), 160.f, 200.f, 22, 20, 1.00f, 1.00f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRunners"), 44.f, 52.f, 26, 7, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRunners"), 44.f, 52.f, 26, 20, 1.00f, 1.00f, 0.08f, 1, 5.f);
Zone(TEXT("People far: red rock return, the first gap"), 19.36f, 19.92f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRidgeRiders"), 258.f, 286.f, 5, 1, 1.00f, 1.00f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarPronghorns"), 140.f, 195.f, 5, 3, 1.00f, 1.00f, 0.08f, 1, 5.f);
Zone(TEXT("People far: red rock return, the second gap"), 20.72f, 21.32f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarRidgeRiders"), 258.f, 286.f, 5, 2, 1.00f, 1.00f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarWagonTrain"), 140.f, 195.f, 5, 4, 1.00f, 1.00f, 0.08f, -1, 5.f);
Zone(TEXT("People far: vegas approach"), 22.03f, 25.60f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarWagonTrain"), 250.f, 280.f, 16, 2, 1.00f, 1.40f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarWagonTrain"), 250.f, 280.f, 16, 10, 1.00f, 1.40f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarMustangs"), 250.f, 280.f, 18, 7, 1.00f, 1.40f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarMustangs"), 250.f, 280.f, 18, 16, 1.00f, 1.40f, 0.08f, 1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarCattleDrive"), 250.f, 280.f, 22, 11, 1.00f, 1.40f, 0.08f, -1, 5.f);
	Scatter(TEXT("/Game/People/Nevada/SM_FarCattleDrive"), 250.f, 280.f, 22, 0, 1.00f, 1.40f, 0.08f, 1, 5.f);
// ---- END far ----
}

{
// ---- BEGIN watch (ft_people_nevada_watch.py) ----
// THE WATCHERS: crowds that came out to see the chase, raised and massed, 44 to 56 m out (behind the main cast and the
// film crew, clear of the dressing's own pieces), and a far crowd on a bluff 130 to 170 m out. WRITTEN BY
// Content/Python/ft_people_nevada_watch.py (python ft_people_nevada_watch.py prints this block): change that file.
// Slot by slot, like the main cast: one Site rule a row, in a zone of its own. 255 rows.
Set(TEXT("PplWatchBleachA"), { { TEXT("/Game/People/Nevada/SM_WatchBleachers_A"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplWatchBleachB"), { { TEXT("/Game/People/Nevada/SM_WatchBleachers_B"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplWatchTrucks"), { { TEXT("/Game/People/Nevada/SM_WatchTrucks"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplWatchPorch"), { { TEXT("/Game/People/Nevada/SM_WatchPorch"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplWatchBalcony"), { { TEXT("/Game/People/Nevada/SM_WatchBalcony"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplWatchBluff"), { { TEXT("/Game/People/Nevada/SM_WatchBluff"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplWatchTrucksQ"), { { TEXT("/Game/People/Nevada/SM_WatchTrucks"), 18.0f, 0, 0, 0, 1.f } });
{
	static const TCHAR* const PplWatchSets[] = { TEXT("PplWatchBleachA"), TEXT("PplWatchBleachB"), TEXT("PplWatchTrucks"), TEXT("PplWatchPorch"), TEXT("PplWatchBalcony"), TEXT("PplWatchBluff"), TEXT("PplWatchTrucksQ") };
	struct FPplWatchRow { int16 Slot; int8 Side; uint8 Set; float X, Scale; };
	static const FPplWatchRow PplWatchRows[] = {
	{4,-1,0,48.0f,1.30f}, {5,1,2,46.0f,1.30f}, {6,-1,5,142.9f,2.20f}, {8,-1,2,48.0f,1.30f}, {8,1,0,46.0f,1.30f}, {12,-1,2,46.0f,1.30f},
	{12,1,0,48.0f,1.30f}, {14,-1,5,146.6f,2.20f}, {16,-1,2,48.0f,1.30f}, {16,1,0,46.0f,1.30f}, {17,-1,2,46.0f,1.30f}, {20,-1,2,46.0f,1.30f},
	{20,1,0,48.0f,1.30f}, {22,-1,5,130.7f,2.20f}, {24,-1,2,48.0f,1.30f}, {24,1,0,46.0f,1.30f}, {25,-1,2,46.0f,1.30f}, {31,1,5,135.4f,2.20f},
	{34,1,5,144.0f,2.20f}, {46,1,5,136.6f,2.20f}, {49,-1,5,161.7f,2.20f}, {52,-1,5,135.5f,2.20f}, {54,-1,3,52.0f,1.30f}, {56,-1,6,48.0f,1.30f},
	{57,-1,0,46.0f,1.30f}, {58,1,3,52.0f,1.30f}, {59,1,0,46.0f,1.30f}, {60,1,6,48.0f,1.30f}, {62,-1,3,52.0f,1.30f}, {64,-1,6,48.0f,1.30f},
	{65,-1,0,46.0f,1.30f}, {66,1,3,52.0f,1.30f}, {67,1,0,46.0f,1.30f}, {68,1,6,48.0f,1.30f}, {70,-1,3,52.0f,1.30f}, {72,-1,6,48.0f,1.30f},
	{73,-1,0,46.0f,1.30f}, {74,1,3,52.0f,1.30f}, {75,1,0,46.0f,1.30f}, {76,1,6,48.0f,1.30f}, {78,-1,3,52.0f,1.30f}, {80,-1,6,48.0f,1.30f},
	{81,-1,0,46.0f,1.30f}, {82,1,3,52.0f,1.30f}, {83,1,0,46.0f,1.30f}, {84,1,6,48.0f,1.30f}, {86,1,5,147.3f,2.20f}, {87,-1,2,46.0f,1.30f},
	{88,1,0,48.0f,1.30f}, {90,-1,0,46.0f,1.30f}, {92,-1,5,151.1f,2.20f}, {93,1,2,46.0f,1.30f}, {94,-1,0,48.0f,1.30f}, {96,1,0,46.0f,1.30f},
	{98,1,5,149.3f,2.20f}, {99,-1,2,46.0f,1.30f}, {100,1,0,48.0f,1.30f}, {102,-1,0,46.0f,1.30f}, {104,-1,5,140.9f,2.20f}, {105,1,2,46.0f,1.30f},
	{106,-1,0,48.0f,1.30f}, {108,1,0,46.0f,1.30f}, {110,1,5,159.6f,2.20f}, {111,-1,2,46.0f,1.30f}, {112,1,0,48.0f,1.30f}, {140,-1,0,46.0f,1.30f},
	{144,1,0,46.0f,1.30f}, {145,-1,2,48.0f,1.30f}, {145,1,3,52.0f,1.30f}, {148,-1,0,46.0f,1.30f}, {152,1,0,46.0f,1.30f}, {153,-1,2,48.0f,1.30f},
	{153,1,3,52.0f,1.30f}, {156,-1,0,46.0f,1.30f}, {160,1,0,46.0f,1.30f}, {161,-1,2,48.0f,1.30f}, {161,1,3,52.0f,1.30f}, {164,-1,0,46.0f,1.30f},
	{168,1,0,46.0f,1.30f}, {169,-1,2,48.0f,1.30f}, {169,1,3,52.0f,1.30f}, {172,-1,4,52.0f,1.30f}, {172,1,1,50.0f,1.30f}, {174,1,1,46.0f,1.30f},
	{180,1,1,46.0f,1.30f}, {184,-1,4,52.0f,1.30f}, {184,1,1,50.0f,1.30f}, {186,1,1,46.0f,1.30f}, {192,1,1,46.0f,1.30f}, {193,-1,4,52.0f,1.30f},
	{193,1,1,50.0f,1.30f}, {196,-1,4,52.0f,1.30f}, {196,1,1,50.0f,1.30f}, {198,1,1,46.0f,1.30f}, {201,-1,4,56.0f,1.30f}, {201,1,1,54.0f,1.30f},
	{204,1,4,56.0f,1.30f}, {206,-1,1,54.0f,1.30f}, {206,1,4,56.0f,1.30f}, {209,-1,1,54.0f,1.30f}, {209,1,4,56.0f,1.30f}, {211,-1,1,54.0f,1.30f},
	{211,1,4,56.0f,1.30f}, {214,1,4,56.0f,1.30f}, {216,-1,4,56.0f,1.30f}, {216,1,1,54.0f,1.30f}, {219,-1,4,56.0f,1.30f}, {219,1,1,54.0f,1.30f},
	{221,-1,4,56.0f,1.30f}, {221,1,1,54.0f,1.30f}, {224,1,4,56.0f,1.30f}, {226,-1,1,54.0f,1.30f}, {226,1,4,56.0f,1.30f}, {229,-1,1,54.0f,1.30f},
	{229,1,4,56.0f,1.30f}, {231,-1,1,54.0f,1.30f}, {231,1,4,56.0f,1.30f}, {234,1,4,56.0f,1.30f}, {236,-1,4,56.0f,1.30f}, {236,1,1,54.0f,1.30f},
	{238,1,5,145.1f,2.20f}, {240,1,0,46.0f,1.30f}, {241,1,0,48.0f,1.30f}, {245,1,2,46.0f,1.30f}, {246,-1,2,48.0f,1.30f}, {246,1,5,163.4f,2.20f},
	{247,1,3,52.0f,1.30f}, {250,-1,0,46.0f,1.30f}, {251,-1,0,48.0f,1.30f}, {252,1,0,46.0f,1.30f}, {254,1,5,152.8f,2.20f}, {255,-1,2,46.0f,1.30f},
	{260,1,0,46.0f,1.30f}, {261,1,0,48.0f,1.30f}, {262,-1,2,46.0f,1.30f}, {262,-1,5,158.6f,2.20f}, {265,1,2,46.0f,1.30f}, {266,-1,2,48.0f,1.30f},
	{267,1,3,52.0f,1.30f}, {270,-1,5,154.1f,2.20f}, {271,-1,0,48.0f,1.30f}, {272,1,0,46.0f,1.30f}, {275,-1,2,46.0f,1.30f}, {276,1,2,48.0f,1.30f},
	{278,-1,5,161.9f,2.20f}, {280,1,0,46.0f,1.30f}, {281,1,0,48.0f,1.30f}, {282,-1,2,46.0f,1.30f}, {285,1,2,46.0f,1.30f}, {286,-1,2,48.0f,1.30f},
	{286,-1,5,161.7f,2.20f}, {287,1,3,52.0f,1.30f}, {290,-1,0,46.0f,1.30f}, {292,1,0,46.0f,1.30f}, {294,-1,5,135.4f,2.20f}, {295,-1,2,46.0f,1.30f},
	{296,1,2,48.0f,1.30f}, {300,1,0,46.0f,1.30f}, {301,1,0,48.0f,1.30f}, {302,-1,2,46.0f,1.30f}, {302,-1,5,166.0f,2.20f}, {306,-1,2,48.0f,1.30f},
	{307,1,3,52.0f,1.30f}, {310,-1,0,46.0f,1.30f}, {310,-1,5,161.1f,2.20f}, {311,-1,0,48.0f,1.30f}, {315,-1,2,46.0f,1.30f}, {316,1,2,48.0f,1.30f},
	{318,-1,5,145.9f,2.20f}, {320,1,0,46.0f,1.30f}, {321,-1,2,50.0f,1.30f}, {323,-1,5,167.2f,2.20f}, {323,1,2,50.0f,1.30f}, {325,1,2,50.0f,1.30f},
	{327,1,0,50.0f,1.30f}, {329,-1,0,50.0f,1.30f}, {331,-1,0,50.0f,1.30f}, {331,-1,5,169.6f,2.20f}, {333,-1,2,50.0f,1.30f}, {335,1,2,50.0f,1.30f},
	{337,1,2,50.0f,1.30f}, {339,-1,5,157.3f,2.20f}, {339,1,0,50.0f,1.30f}, {341,-1,0,50.0f,1.30f}, {343,-1,0,50.0f,1.30f}, {345,-1,2,50.0f,1.30f},
	{347,-1,5,159.4f,2.20f}, {347,1,2,50.0f,1.30f}, {349,1,2,50.0f,1.30f}, {351,1,0,50.0f,1.30f}, {353,-1,0,50.0f,1.30f}, {355,-1,0,50.0f,1.30f},
	{355,-1,5,153.4f,2.20f}, {357,-1,2,50.0f,1.30f}, {359,1,2,50.0f,1.30f}, {361,1,2,50.0f,1.30f}, {363,-1,5,150.5f,2.20f}, {363,1,0,50.0f,1.30f},
	{365,-1,0,50.0f,1.30f}, {367,-1,0,50.0f,1.30f}, {369,-1,2,50.0f,1.30f}, {371,-1,5,146.7f,2.20f}, {371,1,2,50.0f,1.30f}, {373,1,2,50.0f,1.30f},
	{375,1,0,50.0f,1.30f}, {377,-1,0,50.0f,1.30f}, {379,-1,0,50.0f,1.30f}, {379,-1,5,165.5f,2.20f}, {381,-1,2,50.0f,1.30f}, {383,1,2,50.0f,1.30f},
	{385,1,2,50.0f,1.30f}, {387,-1,5,150.5f,2.20f}, {387,1,0,50.0f,1.30f}, {389,-1,0,50.0f,1.30f}, {391,-1,0,50.0f,1.30f}, {393,-1,2,50.0f,1.30f},
	{395,-1,5,152.5f,2.20f}, {395,1,2,50.0f,1.30f}, {397,1,2,50.0f,1.30f}, {399,1,0,50.0f,1.30f}, {401,-1,0,50.0f,1.30f}, {403,-1,0,50.0f,1.30f},
	{403,-1,5,160.8f,2.20f}, {405,-1,2,50.0f,1.30f}, {407,1,2,50.0f,1.30f}, {489,1,4,52.0f,1.30f}, {491,-1,1,50.0f,1.30f}, {491,1,4,52.0f,1.30f},
	{499,-1,1,50.0f,1.30f}, {499,1,4,52.0f,1.30f}, {503,-1,1,50.0f,1.30f}, {503,1,4,52.0f,1.30f}, {511,-1,1,50.0f,1.30f}, {511,1,4,52.0f,1.30f},
	{513,1,4,52.0f,1.30f}, {519,-1,1,50.0f,1.30f}, {519,1,4,52.0f,1.30f}, {521,1,4,52.0f,1.30f}, {523,-1,1,50.0f,1.30f}, {523,1,4,52.0f,1.30f},
	{529,1,4,52.0f,1.30f}, {531,-1,1,50.0f,1.30f}, {531,1,4,52.0f,1.30f}, {539,-1,1,50.0f,1.30f}, {539,1,4,52.0f,1.30f}, {543,-1,1,50.0f,1.30f},
	{543,1,4,52.0f,1.30f}, {551,-1,1,50.0f,1.30f}, {551,1,4,52.0f,1.30f}, {553,1,4,52.0f,1.30f}, {559,-1,1,50.0f,1.30f}, {559,1,4,52.0f,1.30f},
	{561,1,4,52.0f,1.30f}, {563,-1,1,50.0f,1.30f}, {563,1,4,52.0f,1.30f},
	};
	for (const FPplWatchRow& R : PplWatchRows)
	{
		Zone(TEXT("People watch"), (R.Slot - 0.4f) * 0.045243f, (R.Slot + 0.4f) * 0.045243f);
		Site(PplWatchSets[R.Set], R.Side, R.X, 1, 0, 1.f, R.Scale);
	}
}
// ---- END watch ----
}

{
// ---- BEGIN p3a (gaps, ft_people_nevada_p3a.py) ----
// THE GAPS: the general cast of Highway 93 (people at work, at rest, passing through) on every slot and side that had
// nobody near the road. Slot by slot, like the level's own table: one Site rule a row, in a zone of its own (slot s lies
// s * 45.243 m along the road), on the slots and sides where ft_people_nevada_table.py placed no near piece (open
// districts) or no rim piece (the rock cuts). Written by the scratch script C:/ftwork/jobs/L2p3a/table.py.
//   open districts: 35.5 m out at 1.25 (37 m on the phone line's side); in the Syndicate's range, behind its rock banks
//     and ventilation heads, 60 to 63.5 m at 1.45; in the city lights and the Vegas approach (no free slot) half a slot
//     ALONG, between the cast's pieces, 34 m at 1.1 (36.5 m on the phone line's side); the same every fourth slot of
//     Snake Eyes country, Joshua Flats, Last Chance Gas and Jackpot Junction.
//   rock cuts (dry wash, Red Rock Pass, red rock return): on the rim, 33.1 m (the piece brings ft_people_nevada's
//     16.5 m column); in the open bends between the cuts the same pieces as hoodoos, 47 to 53 m.
// Every piece is TALL (banks, a deck, the rim): none nearer than 33.1 m. Nothing lit. No row beside a loop, a
// corkscrew or a jump, nor 17 to 40 m out beside the shallow banked bends (4.3, 4.4, 7.8, 8.0 km).
#define P3A(n) TEXT("/Game/People/Nevada/SM_P3A_" n)
Zone(TEXT("People p3a"), 0.253361f, 0.289555f);
Set(TEXT("PplP3ARoadCrewA"), { { P3A("RoadCrew_A"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3ARoadCrewB"), { { P3A("RoadCrew_B"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3APicnic"), { { P3A("Picnic"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3ALookout"), { { P3A("Lookout"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3AFenceMenders"), { { P3A("FenceMenders"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3ACampers"), { { P3A("Campers"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3APainters"), { { P3A("Painters"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3ATourists"), { { P3A("Tourists"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3ARimHikers"), { { P3A("RimHikers"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3ARimSurvey"), { { P3A("RimSurvey"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3ARimBighorns"), { { P3A("RimBighorns"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3ARoadCrewAH"), { { P3A("RoadCrew_A"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3ARoadCrewBH"), { { P3A("RoadCrew_B"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3APicnicH"), { { P3A("Picnic"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3ALookoutH"), { { P3A("Lookout"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3AFenceMendersH"), { { P3A("FenceMenders"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3ACampersH"), { { P3A("Campers"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3APaintersH"), { { P3A("Painters"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3ATouristsH"), { { P3A("Tourists"), 22.6f, 0, 0, 0, 1.f } });
// (the fix round took out the one explicit row, FenceMenders on slot 6 left: p3c's cyclists ride there)
{
	static const TCHAR* const PplP3ASets[] = { TEXT("PplP3ARoadCrewA"), TEXT("PplP3ARoadCrewB"), TEXT("PplP3APicnic"), TEXT("PplP3ALookout"), TEXT("PplP3AFenceMenders"), TEXT("PplP3ACampers"), TEXT("PplP3APainters"), TEXT("PplP3ATourists"), TEXT("PplP3ARimHikers"), TEXT("PplP3ARimSurvey"), TEXT("PplP3ARimBighorns"), TEXT("PplP3ARoadCrewAH"), TEXT("PplP3ARoadCrewBH"), TEXT("PplP3APicnicH"), TEXT("PplP3ALookoutH"), TEXT("PplP3AFenceMendersH"), TEXT("PplP3ACampersH"), TEXT("PplP3APaintersH"), TEXT("PplP3ATouristsH") };
	struct FPplP3ARow { int16 Slot; int8 Side; uint8 Set; float X, Scale; };
	// slot, side (-1 left of travel, +1 right), set, metres from the centre line, scale
	static const FPplP3ARow PplP3ARows[] = {
	{10,1,0,35.5f,1.25f}, {12,1,4,35.5f,1.25f}, {17,1,0,35.5f,1.25f}, {18,1,2,35.5f,1.25f}, {19,-1,7,37.f,1.25f}, {20,1,0,35.5f,1.25f}, {22,1,2,35.5f,1.25f},
	{23,-1,7,37.f,1.25f}, {25,1,4,35.5f,1.25f}, {47,-1,9,51.7f,1.f}, {49,1,10,49.5f,1.f}, {51,-1,8,52.3f,1.f}, {54,1,16,36.5f,1.1f}, {62,1,13,36.5f,1.1f},
	{78,1,12,36.5f,1.1f}, {85,1,6,37.f,1.25f}, {86,-1,3,35.5f,1.25f}, {86,1,17,36.5f,1.1f}, {87,-1,5,35.5f,1.25f}, {88,1,4,37.f,1.25f}, {89,-1,6,35.5f,1.25f},
	{90,-1,5,35.5f,1.25f}, {90,-1,14,34.f,1.1f}, {91,-1,4,35.5f,1.25f}, {93,1,3,37.f,1.25f}, {104,1,5,37.f,1.25f}, {105,1,4,37.f,1.25f}, {107,-1,6,35.5f,1.25f},
	{109,1,3,37.f,1.25f}, {110,-1,5,35.5f,1.25f}, {110,1,15,36.5f,1.1f}, {112,1,6,37.f,1.25f}, {113,-1,3,35.5f,1.25f}, {142,1,11,34.f,1.1f}, {145,1,1,35.5f,1.25f},
	{147,-1,7,37.f,1.25f}, {150,1,18,34.f,1.1f}, {151,-1,2,37.f,1.25f}, {151,1,0,35.5f,1.25f}, {158,1,11,34.f,1.1f}, {159,-1,7,37.f,1.25f}, {166,1,12,34.f,1.1f},
	{169,1,0,35.5f,1.25f}, {198,1,14,36.5f,1.1f}, {203,-1,14,36.5f,1.1f}, {207,-1,12,36.5f,1.1f}, {211,-1,14,36.5f,1.1f}, {227,-1,14,36.5f,1.1f}, {231,-1,12,36.5f,1.1f},
	{235,-1,14,36.5f,1.1f}, {238,-1,6,37.f,1.25f}, {238,1,4,35.5f,1.25f}, {239,1,5,35.5f,1.25f}, {240,1,3,35.5f,1.25f}, {243,-1,2,37.f,1.25f}, {243,1,6,35.5f,1.25f},
	{244,-1,4,37.f,1.25f}, {245,1,5,35.5f,1.25f}, {246,-1,3,37.f,1.25f}, {247,-1,0,37.f,1.25f}, {248,-1,2,37.f,1.25f}, {248,1,6,35.5f,1.25f}, {251,-1,4,37.f,1.25f},
	{252,1,5,35.5f,1.25f}, {253,-1,3,37.f,1.25f}, {253,1,0,35.5f,1.25f}, {255,-1,6,37.f,1.25f}, {257,1,4,35.5f,1.25f}, {258,-1,5,37.f,1.25f}, {258,1,3,35.5f,1.25f},
	{260,1,2,35.5f,1.25f}, {261,1,6,35.5f,1.25f}, {262,-1,4,37.f,1.25f}, {265,1,3,35.5f,1.25f}, {266,-1,0,37.f,1.25f}, {267,-1,2,37.f,1.25f}, {268,-1,6,37.f,1.25f},
	{271,-1,3,37.f,1.25f}, {273,-1,2,37.f,1.25f}, {273,1,6,35.5f,1.25f}, {274,1,4,35.5f,1.25f}, {275,-1,5,37.f,1.25f}, {278,1,2,35.5f,1.25f}, {279,1,6,35.5f,1.25f},
	{281,1,5,35.5f,1.25f}, {283,-1,0,37.f,1.25f}, {283,1,2,35.5f,1.25f}, {287,-1,5,37.f,1.25f}, {288,-1,3,37.f,1.25f}, {288,1,0,35.5f,1.25f}, {289,-1,2,37.f,1.25f},
	{292,1,4,35.5f,1.25f}, {293,-1,5,37.f,1.25f}, {294,1,0,35.5f,1.25f}, {295,-1,2,37.f,1.25f}, {296,1,6,35.5f,1.25f}, {297,1,4,35.5f,1.25f}, {300,1,3,35.5f,1.25f},
	{301,1,0,35.5f,1.25f}, {303,1,6,35.5f,1.25f}, {304,-1,4,37.f,1.25f}, {306,-1,5,37.f,1.25f}, {308,-1,0,37.f,1.25f}, {309,-1,6,37.f,1.25f}, {310,-1,4,37.f,1.25f},
	{313,-1,3,37.f,1.25f}, {314,1,2,35.5f,1.25f}, {316,1,4,35.5f,1.25f}, {318,-1,3,37.f,1.25f}, {318,1,0,35.5f,1.25f}, {320,1,2,35.5f,1.25f}, {321,-1,5,61.8f,1.45f},
	{322,-1,1,35.5f,1.25f}, {324,-1,2,60.5f,1.45f}, {325,1,7,61.3f,1.45f}, {326,1,4,63.5f,1.45f}, {327,1,3,62.1f,1.45f}, {329,1,0,63.f,1.45f}, {330,-1,5,61.f,1.45f},
	{331,-1,1,61.6f,1.45f}, {332,1,2,61.3f,1.45f}, {334,-1,7,35.5f,1.25f}, {335,-1,4,60.1f,1.45f}, {336,-1,6,61.7f,1.45f}, {337,1,0,61.3f,1.45f}, {339,1,5,62.2f,1.45f},
	{340,1,1,35.5f,1.25f}, {341,1,7,62.f,1.45f}, {342,-1,4,62.f,1.45f}, {344,1,3,61.4f,1.45f}, {345,-1,6,61.9f,1.45f}, {346,-1,0,35.5f,1.25f}, {347,-1,5,62.6f,1.45f},
	{349,1,2,63.f,1.45f}, {350,1,7,60.8f,1.45f}, {351,1,4,61.3f,1.45f}, {352,1,3,35.5f,1.25f}, {354,-1,6,61.6f,1.45f}, {355,-1,0,60.6f,1.45f}, {356,1,5,61.3f,1.45f},
	{357,-1,1,62.4f,1.45f}, {359,-1,7,60.5f,1.45f}, {360,-1,3,61.5f,1.45f}, {361,1,6,63.f,1.45f}, {362,1,0,60.8f,1.45f}, {364,1,5,35.5f,1.25f}, {365,1,2,60.5f,1.45f},
	{366,-1,7,62.5f,1.45f}, {367,-1,4,61.2f,1.45f}, {369,-1,3,62.9f,1.45f}, {371,-1,0,60.6f,1.45f}, {372,-1,1,60.8f,1.45f}, {374,1,2,60.9f,1.45f}, {375,1,7,61.7f,1.45f},
	{377,1,6,62.4f,1.45f}, {379,-1,0,61.1f,1.45f}, {380,1,5,62.3f,1.45f}, {381,-1,1,62.6f,1.45f}, {384,-1,7,60.9f,1.45f}, {385,1,4,62.1f,1.45f}, {386,1,3,60.1f,1.45f},
	{387,1,6,62.f,1.45f}, {389,1,1,61.5f,1.45f}, {390,-1,2,60.5f,1.45f}, {391,-1,7,61.4f,1.45f}, {392,1,4,62.3f,1.45f}, {395,-1,3,61.7f,1.45f}, {396,-1,0,62.5f,1.45f},
	{397,1,5,60.1f,1.45f}, {399,1,1,62.6f,1.45f}, {400,1,2,35.5f,1.25f}, {401,1,4,62.8f,1.45f}, {402,-1,3,61.f,1.45f}, {404,1,6,62.1f,1.45f}, {405,-1,0,62.5f,1.45f},
	{407,-1,5,62.7f,1.45f}, {409,1,8,49.2f,1.f}, {429,1,10,51.2f,1.f}, {431,-1,8,49.2f,1.f}, {433,1,9,52.2f,1.f}, {435,-1,10,50.7f,1.f}, {437,1,8,51.9f,1.f},
	{439,-1,9,49.f,1.f}, {459,-1,10,52.f,1.f}, {461,1,8,50.1f,1.f}, {463,-1,9,52.1f,1.f}, {465,1,10,47.7f,1.f}, {467,-1,8,48.f,1.f}, {469,1,9,51.8f,1.f},
	{471,-1,10,47.f,1.f}, {487,-1,18,34.f,1.1f}, {491,-1,17,34.f,1.1f}, {495,-1,13,34.f,1.1f}, {499,-1,14,34.f,1.1f}, {503,-1,17,34.f,1.1f}, {507,-1,18,34.f,1.1f},
	{511,-1,14,34.f,1.1f}, {515,-1,13,34.f,1.1f}, {519,-1,18,34.f,1.1f}, {523,-1,17,34.f,1.1f}, {527,-1,13,34.f,1.1f}, {531,-1,14,34.f,1.1f}, {535,-1,17,34.f,1.1f},
	{539,-1,18,34.f,1.1f}, {543,-1,14,34.f,1.1f}, {547,-1,13,34.f,1.1f}, {551,-1,18,34.f,1.1f}, {555,-1,17,34.f,1.1f}, {559,-1,13,34.f,1.1f}, {563,-1,14,34.f,1.1f},
	};
	for (const FPplP3ARow& R : PplP3ARows)
	{
		Zone(TEXT("People p3a"), (R.Slot - 0.4f) * 0.045243f, (R.Slot + 0.4f) * 0.045243f);
		Site(PplP3ASets[R.Set], R.Side, R.X, 1, 0, 1.f, R.Scale);
	}
}
#undef P3A
// ---- END p3a ----
}

{
// ---- BEGIN p3b (scenes, ft_people_nevada_p3b.py) ----
// STORY SCENES: ten groups that could only stand beside Highway 93 (a rodeo pen, the sky watchers' camp, the record
// crew on the salt, a casino revue, a barn dance, a fossil dig, a motel pool party, the chili cook-off, a prize fight,
// a rope-rescue drill on the canyon's rim). Every scene brings its own ground two metres up; all are TALL.
// Slot by slot, like the first cast (one Site rule a row, in a zone of its own): a scene stands 37 m out (38 m in the
// rock districts), 1.25 times as built (its front 32.7 m out or more), HALF A SLOT ON from its slot (the sets
// PplP3B...H: 22.6 m along), so it shows in the gap between the first cast's decks of two neighbouring slots, clear
// of the phone line (30 m), the truck stops (40 m, their slots are left out), the first cast's riders and its
// middle-distance rows. The sides take turns slot by slot. The rim rescue stands like the first cast's rim pieces:
// 33.1 m out on its slot, on rock cuts and wall bends where that cast left the rim free; alone (it is a 16.5 m hoodoo
// then) 62 m out in the open bends between the cuts and 64 m out on the Syndicate's range.
// 402 rows. Written by C:\ftwork\jobs\L2p3b\table.py (scratch, plain Python).
Set(TEXT("PplP3BRimRescue"), { { TEXT("/Game/People/Nevada/SM_P3B_RimRescue"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3BRodeoH"), { { TEXT("/Game/People/Nevada/SM_P3B_Rodeo"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3BSkyWatchH"), { { TEXT("/Game/People/Nevada/SM_P3B_SkyWatch"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3BSaltRecordH"), { { TEXT("/Game/People/Nevada/SM_P3B_SaltRecord"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3BRevueH"), { { TEXT("/Game/People/Nevada/SM_P3B_Revue"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3BHoeDownH"), { { TEXT("/Game/People/Nevada/SM_P3B_HoeDown"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3BDigH"), { { TEXT("/Game/People/Nevada/SM_P3B_Dig"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3BPoolPartyH"), { { TEXT("/Game/People/Nevada/SM_P3B_PoolParty"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3BCookOffH"), { { TEXT("/Game/People/Nevada/SM_P3B_CookOff"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3BPrizeFightH"), { { TEXT("/Game/People/Nevada/SM_P3B_PrizeFight"), 22.6f, 0, 0, 0, 1.f } });
{
	static const TCHAR* const PplP3BSets[] = { TEXT("PplP3BRodeo"), TEXT("PplP3BSkyWatch"), TEXT("PplP3BSaltRecord"), TEXT("PplP3BRevue"), TEXT("PplP3BHoeDown"), TEXT("PplP3BDig"), TEXT("PplP3BPoolParty"), TEXT("PplP3BCookOff"), TEXT("PplP3BPrizeFight"), TEXT("PplP3BRimRescue"), TEXT("PplP3BRodeoH"), TEXT("PplP3BSkyWatchH"), TEXT("PplP3BSaltRecordH"), TEXT("PplP3BRevueH"), TEXT("PplP3BHoeDownH"), TEXT("PplP3BDigH"), TEXT("PplP3BPoolPartyH"), TEXT("PplP3BCookOffH"), TEXT("PplP3BPrizeFightH") };
	struct FPplP3BRow { int16 Slot; int8 Side; uint8 Set; float X, Scale; };
	// slot, side (-1 left of travel, +1 right), set, metres from the centre line, scale
	static const FPplP3BRow PplP3BRows[] = {
	{3,1,10,37.0f,1.25f}, {4,-1,14,37.0f,1.25f}, {5,1,11,37.0f,1.25f}, {6,-1,17,37.0f,1.25f}, {7,1,10,37.0f,1.25f}, {8,-1,14,37.0f,1.25f},
	{9,-1,11,37.0f,1.25f}, {10,-1,17,37.0f,1.25f}, {11,1,10,37.0f,1.25f}, {13,-1,14,37.0f,1.25f}, {14,-1,11,37.0f,1.25f}, {15,1,17,37.0f,1.25f},
	{16,-1,10,37.0f,1.25f}, {17,-1,14,37.0f,1.25f}, {18,-1,11,37.0f,1.25f}, {19,1,17,37.0f,1.25f}, {20,1,10,37.0f,1.25f}, {22,-1,14,37.0f,1.25f},
	{23,1,11,37.0f,1.25f}, {24,1,17,37.0f,1.25f}, {30,-1,15,38.0f,1.25f}, {39,-1,9,62.0f,1.00f}, {43,-1,9,33.1f,1.00f}, {46,-1,11,38.0f,1.25f},
	{47,-1,9,62.0f,1.00f}, {48,1,15,38.0f,1.25f}, {49,1,9,62.0f,1.00f}, {49,1,11,38.0f,1.25f}, {50,-1,15,38.0f,1.25f}, {51,-1,9,62.0f,1.00f},
	{52,1,11,38.0f,1.25f}, {53,1,9,62.0f,1.00f}, {54,-1,17,37.0f,1.25f}, {55,1,14,37.0f,1.25f}, {56,-1,10,37.0f,1.25f}, {57,1,11,37.0f,1.25f},
	{58,-1,17,37.0f,1.25f}, {60,-1,14,37.0f,1.25f}, {61,-1,10,37.0f,1.25f}, {62,-1,11,37.0f,1.25f}, {63,1,17,37.0f,1.25f}, {64,-1,14,37.0f,1.25f},
	{65,1,10,37.0f,1.25f}, {66,-1,11,37.0f,1.25f}, {74,-1,17,37.0f,1.25f}, {75,1,14,37.0f,1.25f}, {76,-1,10,37.0f,1.25f}, {77,-1,11,37.0f,1.25f},
	{78,-1,17,37.0f,1.25f}, {79,1,14,37.0f,1.25f}, {80,-1,10,37.0f,1.25f}, {81,1,11,37.0f,1.25f}, {82,-1,17,37.0f,1.25f}, {85,-1,15,37.0f,1.25f},
	{86,-1,11,37.0f,1.25f}, {88,1,10,37.0f,1.25f}, {91,1,14,37.0f,1.25f}, {92,1,15,37.0f,1.25f}, {93,1,11,37.0f,1.25f}, {94,-1,10,37.0f,1.25f},
	{96,-1,14,37.0f,1.25f}, {97,-1,15,37.0f,1.25f}, {98,-1,11,37.0f,1.25f}, {104,1,10,37.0f,1.25f}, {105,1,14,37.0f,1.25f}, {106,-1,15,37.0f,1.25f},
	{109,-1,11,37.0f,1.25f}, {110,-1,10,37.0f,1.25f}, {112,1,14,37.0f,1.25f}, {115,1,9,33.1f,1.00f}, {117,1,9,33.1f,1.00f}, {118,1,9,33.1f,1.00f},
	{128,-1,9,33.1f,1.00f}, {131,-1,9,33.1f,1.00f}, {140,-1,16,37.0f,1.25f}, {141,-1,17,37.0f,1.25f}, {142,-1,10,37.0f,1.25f}, {143,1,16,37.0f,1.25f},
	{144,-1,11,37.0f,1.25f}, {145,1,16,37.0f,1.25f}, {146,-1,17,37.0f,1.25f}, {148,1,10,37.0f,1.25f}, {150,-1,16,37.0f,1.25f}, {151,1,11,37.0f,1.25f},
	{152,-1,16,37.0f,1.25f}, {153,1,17,37.0f,1.25f}, {154,-1,10,37.0f,1.25f}, {155,1,16,37.0f,1.25f}, {156,-1,11,37.0f,1.25f}, {157,-1,16,37.0f,1.25f},
	{158,-1,17,37.0f,1.25f}, {164,1,10,37.0f,1.25f}, {165,1,16,37.0f,1.25f}, {166,-1,11,37.0f,1.25f}, {167,1,16,37.0f,1.25f}, {168,-1,17,37.0f,1.25f},
	{169,1,10,37.0f,1.25f}, {170,-1,16,37.0f,1.25f}, {172,-1,13,37.0f,1.25f}, {173,-1,18,37.0f,1.25f}, {174,-1,16,37.0f,1.25f}, {175,1,13,37.0f,1.25f},
	{176,1,18,37.0f,1.25f}, {177,1,16,37.0f,1.25f}, {191,1,13,37.0f,1.25f}, {192,-1,18,37.0f,1.25f}, {193,1,16,37.0f,1.25f}, {194,-1,13,37.0f,1.25f},
	{195,1,18,37.0f,1.25f}, {196,1,16,37.0f,1.25f}, {197,1,13,37.0f,1.25f}, {198,-1,18,37.0f,1.25f}, {199,1,16,37.0f,1.25f}, {201,1,13,37.0f,1.25f},
	{202,-1,18,37.0f,1.25f}, {203,1,16,37.0f,1.25f}, {205,-1,17,37.0f,1.25f}, {206,-1,13,37.0f,1.25f}, {207,1,18,37.0f,1.25f}, {208,1,16,37.0f,1.25f},
	{209,1,17,37.0f,1.25f}, {210,-1,13,37.0f,1.25f}, {211,1,18,37.0f,1.25f}, {217,1,16,37.0f,1.25f}, {224,1,17,37.0f,1.25f}, {225,1,13,37.0f,1.25f},
	{226,-1,18,37.0f,1.25f}, {227,1,16,37.0f,1.25f}, {228,1,17,37.0f,1.25f}, {229,1,13,37.0f,1.25f}, {230,-1,18,37.0f,1.25f}, {231,1,16,37.0f,1.25f},
	{232,1,17,37.0f,1.25f}, {233,1,13,37.0f,1.25f}, {234,-1,18,37.0f,1.25f}, {235,1,16,37.0f,1.25f}, {237,-1,12,37.0f,1.25f}, {239,1,11,37.0f,1.25f},
	{240,1,12,37.0f,1.25f}, {241,1,15,37.0f,1.25f}, {242,-1,10,37.0f,1.25f}, {245,1,12,37.0f,1.25f}, {246,-1,11,37.0f,1.25f}, {247,1,12,37.0f,1.25f},
	{249,-1,15,37.0f,1.25f}, {250,-1,10,37.0f,1.25f}, {251,1,12,37.0f,1.25f}, {252,1,11,37.0f,1.25f}, {253,1,12,37.0f,1.25f}, {256,-1,15,37.0f,1.25f},
	{257,-1,10,37.0f,1.25f}, {259,1,12,37.0f,1.25f}, {260,1,11,37.0f,1.25f}, {261,1,12,37.0f,1.25f}, {262,-1,15,37.0f,1.25f}, {264,1,10,37.0f,1.25f},
	{265,1,12,37.0f,1.25f}, {266,-1,11,37.0f,1.25f}, {267,1,12,37.0f,1.25f}, {269,-1,15,37.0f,1.25f}, {270,-1,10,37.0f,1.25f}, {271,1,12,37.0f,1.25f},
	{272,1,11,37.0f,1.25f}, {273,1,12,37.0f,1.25f}, {276,-1,15,37.0f,1.25f}, {277,-1,10,37.0f,1.25f}, {279,1,12,37.0f,1.25f}, {280,1,11,37.0f,1.25f},
	{281,1,12,37.0f,1.25f}, {282,-1,15,37.0f,1.25f}, {285,1,10,37.0f,1.25f}, {286,-1,12,37.0f,1.25f}, {287,1,11,37.0f,1.25f}, {288,1,12,37.0f,1.25f},
	{289,-1,15,37.0f,1.25f}, {290,-1,10,37.0f,1.25f}, {291,1,12,37.0f,1.25f}, {292,1,11,37.0f,1.25f}, {293,1,12,37.0f,1.25f}, {296,-1,15,37.0f,1.25f},
	{297,-1,10,37.0f,1.25f}, {299,1,12,37.0f,1.25f}, {300,1,11,37.0f,1.25f}, {301,1,12,37.0f,1.25f}, {302,-1,15,37.0f,1.25f}, {305,1,10,37.0f,1.25f},
	{306,-1,12,37.0f,1.25f}, {307,1,11,37.0f,1.25f}, {309,-1,12,37.0f,1.25f}, {310,-1,15,37.0f,1.25f}, {311,1,10,37.0f,1.25f}, {312,1,12,37.0f,1.25f},
	{313,1,11,37.0f,1.25f}, {315,1,12,37.0f,1.25f}, {316,-1,15,37.0f,1.25f}, {317,-1,10,37.0f,1.25f}, {319,1,12,37.0f,1.25f}, {321,-1,15,37.0f,1.25f},
	{322,-1,12,37.0f,1.25f}, {323,1,9,64.0f,1.15f}, {323,1,11,37.0f,1.25f}, {324,-1,10,37.0f,1.25f}, {325,1,15,37.0f,1.25f}, {326,-1,12,37.0f,1.25f},
	{327,1,11,37.0f,1.25f}, {328,-1,10,37.0f,1.25f}, {329,-1,15,37.0f,1.25f}, {329,-1,9,64.0f,1.15f}, {330,-1,12,37.0f,1.25f}, {331,1,11,37.0f,1.25f},
	{332,-1,10,37.0f,1.25f}, {333,1,15,37.0f,1.25f}, {334,-1,12,37.0f,1.25f}, {335,1,9,64.0f,1.15f}, {335,1,11,37.0f,1.25f}, {336,-1,10,37.0f,1.25f},
	{337,1,15,37.0f,1.25f}, {339,1,12,37.0f,1.25f}, {340,-1,11,37.0f,1.25f}, {341,-1,9,64.0f,1.15f}, {341,1,10,37.0f,1.25f}, {342,-1,15,37.0f,1.25f},
	{343,1,12,37.0f,1.25f}, {344,-1,11,37.0f,1.25f}, {345,-1,10,37.0f,1.25f}, {346,-1,15,37.0f,1.25f}, {347,1,9,64.0f,1.15f}, {347,1,12,37.0f,1.25f},
	{348,-1,11,37.0f,1.25f}, {349,1,10,37.0f,1.25f}, {350,-1,15,37.0f,1.25f}, {351,1,12,37.0f,1.25f}, {352,-1,11,37.0f,1.25f}, {353,-1,9,64.0f,1.15f},
	{353,-1,10,37.0f,1.25f}, {354,-1,15,37.0f,1.25f}, {355,1,12,37.0f,1.25f}, {356,-1,11,37.0f,1.25f}, {357,1,10,37.0f,1.25f}, {358,-1,15,37.0f,1.25f},
	{359,1,9,64.0f,1.15f}, {359,1,12,37.0f,1.25f}, {360,-1,11,37.0f,1.25f}, {361,1,10,37.0f,1.25f}, {363,1,15,37.0f,1.25f}, {364,-1,12,37.0f,1.25f},
	{365,-1,9,64.0f,1.15f}, {365,1,11,37.0f,1.25f}, {366,-1,10,37.0f,1.25f}, {367,1,15,37.0f,1.25f}, {368,-1,12,37.0f,1.25f}, {369,-1,11,37.0f,1.25f},
	{370,-1,10,37.0f,1.25f}, {371,1,15,37.0f,1.25f}, {371,1,9,64.0f,1.15f}, {372,-1,12,37.0f,1.25f}, {373,1,11,37.0f,1.25f}, {374,-1,10,37.0f,1.25f},
	{375,1,15,37.0f,1.25f}, {376,-1,12,37.0f,1.25f}, {377,-1,9,64.0f,1.15f}, {377,-1,11,37.0f,1.25f}, {378,-1,10,37.0f,1.25f}, {379,1,15,37.0f,1.25f},
	{380,-1,12,37.0f,1.25f}, {381,1,11,37.0f,1.25f}, {382,-1,10,37.0f,1.25f}, {383,1,15,37.0f,1.25f}, {383,1,9,64.0f,1.15f}, {384,-1,12,37.0f,1.25f},
	{385,1,11,37.0f,1.25f}, {387,1,10,37.0f,1.25f}, {388,-1,15,37.0f,1.25f}, {389,-1,9,64.0f,1.15f}, {389,1,12,37.0f,1.25f}, {390,-1,11,37.0f,1.25f},
	{391,1,10,37.0f,1.25f}, {392,-1,15,37.0f,1.25f}, {393,1,12,37.0f,1.25f}, {395,1,9,64.0f,1.15f}, {395,1,11,37.0f,1.25f}, {396,-1,10,37.0f,1.25f},
	{397,1,15,37.0f,1.25f}, {398,-1,12,37.0f,1.25f}, {399,1,11,37.0f,1.25f}, {400,-1,10,37.0f,1.25f}, {401,-1,15,37.0f,1.25f}, {401,-1,9,64.0f,1.15f},
	{402,-1,12,37.0f,1.25f}, {403,1,11,37.0f,1.25f}, {404,-1,10,37.0f,1.25f}, {405,1,15,37.0f,1.25f}, {406,-1,12,37.0f,1.25f}, {407,1,9,64.0f,1.15f},
	{407,1,11,37.0f,1.25f}, {409,1,15,38.0f,1.25f}, {409,1,9,62.0f,1.00f}, {421,-1,9,33.1f,1.00f}, {428,1,11,38.0f,1.25f}, {429,1,15,38.0f,1.25f},
	{429,1,9,62.0f,1.00f}, {430,-1,11,38.0f,1.25f}, {431,-1,9,62.0f,1.00f}, {432,1,15,38.0f,1.25f}, {433,1,9,62.0f,1.00f}, {434,-1,15,38.0f,1.25f},
	{435,-1,9,62.0f,1.00f}, {437,1,15,38.0f,1.25f}, {437,1,9,62.0f,1.00f}, {439,-1,9,62.0f,1.00f}, {458,-1,15,38.0f,1.25f}, {459,-1,9,62.0f,1.00f},
	{461,1,15,38.0f,1.25f}, {461,1,9,62.0f,1.00f}, {463,-1,9,62.0f,1.00f}, {464,1,15,38.0f,1.25f}, {465,1,9,62.0f,1.00f}, {466,-1,15,38.0f,1.25f},
	{467,-1,9,62.0f,1.00f}, {469,1,15,38.0f,1.25f}, {469,1,9,62.0f,1.00f}, {471,-1,9,62.0f,1.00f}, {477,1,9,33.1f,1.00f}, {483,1,9,33.1f,1.00f},
	{486,1,9,33.1f,1.00f}, {487,1,13,37.0f,1.25f}, {488,1,18,37.0f,1.25f}, {489,1,16,37.0f,1.25f}, {490,-1,14,37.0f,1.25f}, {491,1,13,37.0f,1.25f},
	{492,-1,18,37.0f,1.25f}, {493,-1,17,37.0f,1.25f}, {494,-1,13,37.0f,1.25f}, {495,1,18,37.0f,1.25f}, {496,1,16,37.0f,1.25f}, {497,1,14,37.0f,1.25f},
	{498,-1,13,37.0f,1.25f}, {499,1,18,37.0f,1.25f}, {500,1,17,37.0f,1.25f}, {501,1,13,37.0f,1.25f}, {502,-1,18,37.0f,1.25f}, {503,1,16,37.0f,1.25f},
	{504,1,14,37.0f,1.25f}, {505,1,13,37.0f,1.25f}, {506,-1,18,37.0f,1.25f}, {507,1,17,37.0f,1.25f}, {508,-1,13,37.0f,1.25f}, {509,-1,18,37.0f,1.25f},
	{510,-1,16,37.0f,1.25f}, {511,1,14,37.0f,1.25f}, {512,1,13,37.0f,1.25f}, {513,1,18,37.0f,1.25f}, {514,-1,17,37.0f,1.25f}, {515,1,13,37.0f,1.25f},
	{516,1,18,37.0f,1.25f}, {517,1,16,37.0f,1.25f}, {518,-1,14,37.0f,1.25f}, {519,1,13,37.0f,1.25f}, {520,1,18,37.0f,1.25f}, {521,1,17,37.0f,1.25f},
	{522,-1,13,37.0f,1.25f}, {523,1,18,37.0f,1.25f}, {524,-1,16,37.0f,1.25f}, {525,-1,14,37.0f,1.25f}, {526,-1,13,37.0f,1.25f}, {527,1,18,37.0f,1.25f},
	{528,1,17,37.0f,1.25f}, {529,1,13,37.0f,1.25f}, {530,-1,18,37.0f,1.25f}, {531,1,16,37.0f,1.25f}, {532,1,14,37.0f,1.25f}, {533,1,13,37.0f,1.25f},
	{534,-1,18,37.0f,1.25f}, {535,1,17,37.0f,1.25f}, {536,1,13,37.0f,1.25f}, {537,1,18,37.0f,1.25f}, {538,-1,16,37.0f,1.25f}, {539,1,14,37.0f,1.25f},
	{540,-1,13,37.0f,1.25f}, {541,-1,18,37.0f,1.25f}, {542,-1,17,37.0f,1.25f}, {543,1,13,37.0f,1.25f}, {544,1,18,37.0f,1.25f}, {545,1,16,37.0f,1.25f},
	{546,-1,14,37.0f,1.25f}, {547,1,13,37.0f,1.25f}, {548,1,18,37.0f,1.25f}, {549,1,17,37.0f,1.25f}, {550,-1,13,37.0f,1.25f}, {551,1,18,37.0f,1.25f},
	{552,1,16,37.0f,1.25f}, {553,1,14,37.0f,1.25f}, {554,-1,13,37.0f,1.25f}, {555,1,18,37.0f,1.25f}, {556,-1,17,37.0f,1.25f}, {557,-1,13,37.0f,1.25f},
	{558,-1,18,37.0f,1.25f}, {559,1,16,37.0f,1.25f}, {560,1,14,37.0f,1.25f}, {561,1,13,37.0f,1.25f}, {562,-1,18,37.0f,1.25f}, {563,1,17,37.0f,1.25f},
	};
	for (const FPplP3BRow& R : PplP3BRows)
	{
		Zone(TEXT("People p3b"), (R.Slot - 0.4f) * 0.045243f, (R.Slot + 0.4f) * 0.045243f);
		Site(PplP3BSets[R.Set], R.Side, R.X, 1, 0, 1.f, R.Scale);
	}
}
// ---- END p3b ----
}

{
// ---- BEGIN p3c (movers, ft_people_nevada_p3c.py) ----
// THE MOVERS: everything here travels along a path of its own, laid along the road, and comes round again (or goes
// round a ring). Foot races, hikers, cyclists, a marching band and boardwalk strollers on a 2.3 m berm 36 to 37.2 m out
// (TALL pieces, 6.8 m deep: their fronts stand 32.6 m out or more). A peloton, pack trains, stagecoaches, a hand car on
// its rail bed, land yachts and round pens 76 to 112 m out. Pack trains and pelotons turned 90 degrees, heading away
// from the road, 128 m out (they reach from 80 to 177 m). Nothing crosses the road. Written by
// C:/ftwork/jobs/L2p3c/table.py, slot by slot like the table above (slot s lies s * 45.243 m along the road): one Site
// rule a row, in a zone of its own, clear of the rock cuts, the banked bends, the signs, the stops and the cast that
// stands there already. 343 rows; a row beside a loop, a jump or a corkscrew is left out by the track itself.
Zone(TEXT("People p3c"), 0.f, 0.01f);
#define P3C(n) TEXT("/Game/People/Nevada/SM_P3C_" n)
Set(TEXT("PplP3CRunA"), { { P3C("Runners_A"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3CRunB"), { { P3C("Runners_B"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3CHikers"), { { P3C("Hikers"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3CCycA"), { { P3C("Cyclists_A"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3CParade"), { { P3C("Parade"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3CStrip"), { { P3C("StripWalkers"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3CCycB"), { { P3C("Cyclists_B"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3CMule"), { { P3C("MuleTrain"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3CCoach"), { { P3C("Stagecoach"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3CHandcar"), { { P3C("Handcar"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3CYachts"), { { P3C("LandYachts"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3CPen"), { { P3C("RoundPen"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3CMuleAway"), { { P3C("MuleTrain"), 0, 0, 0, 90.f, 1.f } });
Set(TEXT("PplP3CCycAway"), { { P3C("Cyclists_B"), 0, 0, 0, 90.f, 1.f } });
#undef P3C
{
	static const TCHAR* const PplP3CSets[] = { TEXT("PplP3CRunA"), TEXT("PplP3CRunB"), TEXT("PplP3CHikers"), TEXT("PplP3CCycA"), TEXT("PplP3CParade"), TEXT("PplP3CStrip"), TEXT("PplP3CCycB"), TEXT("PplP3CMule"), TEXT("PplP3CCoach"), TEXT("PplP3CHandcar"), TEXT("PplP3CYachts"), TEXT("PplP3CPen"), TEXT("PplP3CMuleAway"), TEXT("PplP3CCycAway") };
	struct FPplP3CRow { int16 Slot; int8 Side; uint8 Set; float X, Scale; };
	// slot, side (-1 left of travel, +1 right), set, metres from the centre line, scale
	static const FPplP3CRow PplP3CRows[] = {
	{6,-1,3,36.0f,1.00f}, {6,1,12,128.0f,1.00f}, {6,1,0,36.0f,1.00f}, {8,-1,0,37.2f,1.00f}, {8,1,1,37.2f,1.00f}, {9,-1,6,76.0f,1.00f}, {9,1,7,76.0f,1.00f},
	{10,-1,1,36.6f,1.00f}, {11,-1,12,128.0f,1.00f}, {11,1,2,37.2f,1.00f}, {12,-1,3,36.0f,1.00f}, {14,-1,1,37.2f,1.00f}, {16,-1,2,36.6f,1.00f}, {16,1,0,36.6f,1.00f},
	{17,-1,11,86.0f,1.00f}, {17,1,7,76.0f,1.00f}, {18,-1,0,36.0f,1.00f}, {19,1,2,36.6f,1.00f}, {20,-1,3,37.2f,1.00f}, {20,-1,12,128.0f,1.00f}, {21,1,11,86.0f,1.00f},
	{22,-1,2,36.6f,1.00f}, {24,-1,1,36.0f,1.00f}, {24,1,3,36.0f,1.00f}, {31,-1,8,112.0f,1.30f}, {47,-1,8,112.0f,1.30f}, {47,1,8,112.0f,1.30f}, {53,-1,8,112.0f,1.30f},
	{53,1,7,112.0f,1.30f}, {55,1,1,36.6f,1.00f}, {56,1,11,86.0f,1.00f}, {57,-1,3,36.0f,1.00f}, {59,1,11,86.0f,1.00f}, {60,-1,6,80.0f,1.00f}, {61,-1,3,36.6f,1.00f},
	{63,-1,8,80.0f,1.00f}, {63,1,2,36.0f,1.00f}, {64,1,6,80.0f,1.00f}, {67,1,11,86.0f,1.00f}, {67,1,1,36.6f,1.00f}, {68,-1,6,80.0f,1.00f}, {71,-1,8,80.0f,1.00f},
	{72,1,11,86.0f,1.00f}, {73,-1,3,36.6f,1.00f}, {75,1,11,86.0f,1.00f}, {75,1,1,36.0f,1.00f}, {76,-1,6,80.0f,1.00f}, {79,-1,8,80.0f,1.00f}, {79,1,2,36.6f,1.00f},
	{80,1,6,80.0f,1.00f}, {81,-1,1,36.0f,1.00f}, {83,1,11,86.0f,1.00f}, {84,-1,6,80.0f,1.00f}, {85,-1,0,36.6f,1.00f}, {86,1,2,37.2f,1.00f}, {87,1,9,79.0f,1.00f},
	{88,-1,3,36.6f,1.00f}, {88,-1,9,86.0f,1.00f}, {89,1,2,37.2f,1.00f}, {91,1,9,91.0f,1.00f}, {92,-1,9,82.0f,1.00f}, {92,-1,0,37.2f,1.00f}, {92,1,0,37.2f,1.00f},
	{95,1,9,87.0f,1.00f}, {96,-1,7,78.0f,1.00f}, {99,-1,0,36.0f,1.00f}, {99,1,3,36.0f,1.00f}, {99,1,9,83.0f,1.00f}, {100,-1,11,90.0f,1.00f}, {102,-1,3,36.0f,1.00f},
	{102,1,0,36.0f,1.00f}, {103,1,9,79.0f,1.00f}, {104,-1,6,86.0f,1.00f}, {105,-1,0,36.0f,1.00f}, {106,1,3,36.6f,1.00f}, {107,1,7,91.0f,1.00f}, {108,-1,6,82.0f,1.00f},
	{108,-1,0,36.0f,1.00f}, {110,-1,13,128.0f,1.00f}, {110,1,2,37.2f,1.00f}, {111,-1,2,36.0f,1.00f}, {111,1,8,87.0f,1.00f}, {113,1,2,37.2f,1.00f}, {141,-1,5,36.0f,1.00f},
	{143,1,6,84.0f,1.00f}, {144,-1,5,36.0f,1.00f}, {149,-1,11,86.0f,1.00f}, {150,1,5,36.0f,1.00f}, {151,1,11,86.0f,1.00f}, {153,-1,5,36.0f,1.00f}, {153,1,5,36.0f,1.00f},
	{155,-1,8,84.0f,1.00f}, {161,1,8,84.0f,1.00f}, {162,1,5,36.0f,1.00f}, {163,-1,11,86.0f,1.00f}, {165,1,5,36.0f,1.00f}, {167,1,6,84.0f,1.00f}, {168,-1,5,36.0f,1.00f},
	{173,1,8,88.0f,1.00f}, {176,-1,6,88.0f,1.00f}, {178,-1,4,36.6f,1.00f}, {178,1,4,36.6f,1.00f}, {179,1,8,88.0f,1.00f}, {180,-1,4,36.0f,1.00f}, {180,1,4,36.0f,1.00f},
	{184,-1,4,36.6f,1.00f}, {184,1,4,36.6f,1.00f}, {185,1,6,88.0f,1.00f}, {186,-1,5,36.0f,1.00f}, {186,1,4,36.0f,1.00f}, {190,-1,5,36.6f,1.00f}, {190,1,4,36.6f,1.00f},
	{191,1,8,88.0f,1.00f}, {192,-1,4,36.0f,1.00f}, {192,1,5,36.0f,1.00f}, {196,-1,4,36.6f,1.00f}, {196,1,5,36.6f,1.00f}, {197,1,6,88.0f,1.00f}, {198,-1,5,36.0f,1.00f},
	{198,1,4,36.0f,1.00f}, {200,-1,8,88.0f,1.00f}, {206,-1,6,98.0f,1.00f}, {216,-1,8,98.0f,1.00f}, {226,-1,6,98.0f,1.00f}, {236,-1,8,98.0f,1.00f}, {237,-1,1,36.0f,1.00f},
	{237,1,1,36.0f,1.00f}, {237,1,10,81.0f,1.00f}, {241,-1,0,36.6f,1.00f}, {241,-1,10,81.0f,1.00f}, {241,1,1,36.6f,1.00f}, {243,1,10,91.0f,1.00f}, {245,-1,12,128.0f,1.00f},
	{245,-1,0,37.2f,1.00f}, {246,1,1,36.0f,1.00f}, {248,1,10,76.0f,1.00f}, {250,-1,2,36.6f,1.00f}, {250,1,0,36.6f,1.00f}, {251,-1,10,91.0f,1.00f}, {253,1,9,81.0f,1.00f},
	{254,-1,3,37.2f,1.00f}, {254,-1,12,128.0f,1.00f}, {254,1,2,37.2f,1.00f}, {258,1,10,86.0f,1.00f}, {259,-1,9,91.0f,1.00f}, {259,-1,0,36.6f,1.00f}, {259,1,2,36.6f,1.00f},
	{263,-1,12,128.0f,1.00f}, {263,1,10,91.0f,1.00f}, {264,-1,3,36.0f,1.00f}, {264,1,2,36.0f,1.00f}, {267,1,12,128.0f,1.00f}, {268,-1,10,76.0f,1.00f}, {268,1,3,36.6f,1.00f},
	{269,-1,3,37.2f,1.00f}, {272,1,1,37.2f,1.00f}, {272,1,10,76.0f,1.00f}, {273,-1,9,81.0f,1.00f}, {274,-1,1,36.6f,1.00f}, {276,1,2,36.0f,1.00f}, {276,1,12,128.0f,1.00f},
	{278,-1,3,37.2f,1.00f}, {278,-1,10,86.0f,1.00f}, {280,1,2,36.6f,1.00f}, {281,-1,12,128.0f,1.00f}, {281,1,10,81.0f,1.00f}, {282,-1,3,36.0f,1.00f}, {285,1,12,128.0f,1.00f},
	{285,1,1,36.0f,1.00f}, {286,-1,3,36.6f,1.00f}, {286,-1,10,86.0f,1.00f}, {289,1,0,36.6f,1.00f}, {290,-1,2,37.2f,1.00f}, {290,1,9,86.0f,1.00f}, {291,-1,10,91.0f,1.00f},
	{293,1,1,37.2f,1.00f}, {294,-1,3,36.0f,1.00f}, {294,1,12,128.0f,1.00f}, {296,-1,9,76.0f,1.00f}, {299,-1,2,37.2f,1.00f}, {299,-1,12,128.0f,1.00f}, {299,1,1,37.2f,1.00f},
	{299,1,10,91.0f,1.00f}, {303,-1,1,36.0f,1.00f}, {303,1,12,128.0f,1.00f}, {304,-1,10,76.0f,1.00f}, {304,1,1,36.6f,1.00f}, {307,-1,0,36.6f,1.00f}, {308,1,9,76.0f,1.00f},
	{308,1,0,37.2f,1.00f}, {309,-1,9,81.0f,1.00f}, {311,-1,2,37.2f,1.00f}, {312,1,12,128.0f,1.00f}, {313,1,3,36.6f,1.00f}, {314,-1,10,86.0f,1.00f}, {315,-1,2,36.0f,1.00f},
	{317,-1,12,128.0f,1.00f}, {317,1,1,37.2f,1.00f}, {317,1,10,81.0f,1.00f}, {320,-1,3,37.2f,1.00f}, {321,1,13,128.0f,1.00f}, {322,-1,9,84.0f,1.00f}, {322,1,0,36.6f,1.00f},
	{324,-1,0,36.0f,1.00f}, {326,-1,13,128.0f,1.00f}, {326,1,2,37.2f,1.00f}, {326,1,11,86.0f,1.00f}, {328,-1,0,36.6f,1.00f}, {330,1,2,36.0f,1.00f}, {331,-1,11,86.0f,1.00f},
	{332,-1,1,37.2f,1.00f}, {334,1,8,84.0f,1.00f}, {334,1,0,36.6f,1.00f}, {335,-1,13,128.0f,1.00f}, {336,-1,3,36.0f,1.00f}, {338,1,0,37.2f,1.00f}, {339,1,13,128.0f,1.00f},
	{340,-1,1,36.6f,1.00f}, {341,-1,9,84.0f,1.00f}, {342,1,0,36.0f,1.00f}, {344,-1,13,128.0f,1.00f}, {344,-1,1,37.2f,1.00f}, {344,1,9,84.0f,1.00f}, {346,1,0,36.6f,1.00f},
	{348,-1,2,36.0f,1.00f}, {348,1,13,128.0f,1.00f}, {350,-1,11,86.0f,1.00f}, {350,1,1,37.2f,1.00f}, {352,-1,2,36.6f,1.00f}, {353,-1,13,128.0f,1.00f}, {353,1,11,86.0f,1.00f},
	{354,1,3,36.0f,1.00f}, {358,-1,8,84.0f,1.00f}, {358,-1,1,36.6f,1.00f}, {358,1,9,84.0f,1.00f}, {358,1,2,36.6f,1.00f}, {362,-1,3,37.2f,1.00f}, {362,-1,13,128.0f,1.00f},
	{362,1,0,37.2f,1.00f}, {366,-1,1,36.0f,1.00f}, {366,1,0,36.0f,1.00f}, {366,1,10,84.0f,1.00f}, {370,-1,8,84.0f,1.00f}, {370,-1,3,36.6f,1.00f}, {372,1,2,36.0f,1.00f},
	{374,-1,3,37.2f,1.00f}, {374,1,11,86.0f,1.00f}, {376,1,0,36.6f,1.00f}, {378,-1,9,84.0f,1.00f}, {378,-1,2,36.0f,1.00f}, {380,1,3,37.2f,1.00f}, {382,-1,2,36.6f,1.00f},
	{382,1,8,84.0f,1.00f}, {384,1,3,36.0f,1.00f}, {384,1,13,128.0f,1.00f}, {386,-1,9,84.0f,1.00f}, {386,-1,1,37.2f,1.00f}, {388,1,1,36.6f,1.00f}, {389,-1,13,128.0f,1.00f},
	{390,-1,0,36.0f,1.00f}, {392,1,2,37.2f,1.00f}, {393,1,13,128.0f,1.00f}, {394,-1,3,36.6f,1.00f}, {395,-1,11,86.0f,1.00f}, {398,-1,13,128.0f,1.00f}, {398,-1,0,37.2f,1.00f},
	{398,1,2,37.2f,1.00f}, {398,1,10,84.0f,1.00f}, {402,-1,1,36.0f,1.00f}, {402,1,3,36.0f,1.00f}, {402,1,13,128.0f,1.00f}, {403,-1,7,84.0f,1.00f}, {406,-1,1,36.6f,1.00f},
	{406,1,2,36.6f,1.00f}, {407,1,11,86.0f,1.00f}, {430,-1,7,96.0f,1.00f}, {430,1,8,96.0f,1.00f}, {434,-1,7,96.0f,1.00f}, {434,1,8,96.0f,1.00f}, {438,-1,7,96.0f,1.00f},
	{438,1,8,96.0f,1.00f}, {462,-1,7,96.0f,1.00f}, {462,1,8,96.0f,1.00f}, {466,-1,7,96.0f,1.00f}, {466,1,8,96.0f,1.00f}, {470,-1,7,96.0f,1.00f}, {470,1,8,96.0f,1.00f},
	{489,1,5,36.0f,1.00f}, {491,-1,4,37.2f,1.00f}, {493,1,9,92.0f,1.00f}, {495,-1,4,36.0f,1.00f}, {495,1,5,36.0f,1.00f}, {496,-1,11,92.0f,1.00f}, {499,-1,5,36.6f,1.00f},
	{499,1,4,36.6f,1.00f}, {501,1,9,92.0f,1.00f}, {503,-1,4,37.2f,1.00f}, {503,1,5,37.2f,1.00f}, {504,-1,11,92.0f,1.00f}, {507,-1,5,36.0f,1.00f}, {507,1,4,36.0f,1.00f},
	{508,-1,9,92.0f,1.00f}, {511,-1,4,36.6f,1.00f}, {511,1,11,92.0f,1.00f}, {511,1,5,36.6f,1.00f}, {514,-1,9,92.0f,1.00f}, {515,-1,4,37.2f,1.00f}, {515,1,5,37.2f,1.00f},
	{519,-1,4,36.0f,1.00f}, {519,1,11,92.0f,1.00f}, {519,1,5,36.0f,1.00f}, {523,-1,5,36.6f,1.00f}, {523,1,9,92.0f,1.00f}, {523,1,4,36.6f,1.00f}, {526,-1,11,92.0f,1.00f},
	{527,-1,5,37.2f,1.00f}, {527,1,4,37.2f,1.00f}, {531,-1,5,36.0f,1.00f}, {531,1,4,36.0f,1.00f}, {531,1,11,92.0f,1.00f}, {534,-1,9,92.0f,1.00f}, {535,-1,5,36.6f,1.00f},
	{535,1,4,36.6f,1.00f}, {538,-1,9,92.0f,1.00f}, {539,-1,4,37.2f,1.00f}, {539,1,5,37.2f,1.00f}, {543,-1,4,36.0f,1.00f}, {543,1,11,92.0f,1.00f}, {543,1,5,36.0f,1.00f},
	{544,-1,9,92.0f,1.00f}, {547,-1,4,36.6f,1.00f}, {547,1,5,36.6f,1.00f}, {549,1,9,92.0f,1.00f}, {551,-1,5,37.2f,1.00f}, {551,1,4,37.2f,1.00f}, {555,-1,5,36.0f,1.00f},
	{555,1,4,36.0f,1.00f}, {556,-1,11,92.0f,1.00f}, {559,-1,4,36.6f,1.00f}, {559,1,8,92.0f,1.00f}, {559,1,5,36.6f,1.00f}, {563,-1,5,37.2f,1.00f}, {563,1,4,37.2f,1.00f},
	};
	for (const FPplP3CRow& R : PplP3CRows)
	{
		Zone(TEXT("People p3c"), (R.Slot - 0.4f) * 0.045243f, (R.Slot + 0.4f) * 0.045243f);
		Site(PplP3CSets[R.Set], R.Side, R.X, 1, 0, 1.f, R.Scale);
	}
}
// ---- END p3c ----
}

{
// ---- BEGIN p3d (heights, ft_people_nevada_p3d.py) ----
// Nevada, pass 3, builder p3d: people UP HIGH. Every piece brings its own structure (a scaffold, a water tower, a
// billboard, a rock pinnacle, a mesa, a motel roof, a sign frame, a trestle, a derrick, a fire lookout, a camper's
// roof) and every one of them is TALL: no rule here starts nearer than 44 m to the centre line (the band from 40 m
// in leaves the banked bends' cuttings and the jumps' run-ups alone). One side a rule, phases chosen so that two of
// these never share a slot on a side. In the rock cuts and the pass (15 m walls 30 m out) only the 21 m needle
// SM_P3D_RockSpire_B is placed. Estimate: 464 placed by the rules' own chances (before the track leaves any out);
// the most any one mesh can be placed: 57 (Lookout).
#define P3D(n) TEXT("/Game/People/Nevada/SM_P3D_" n)
Zone(TEXT("P3D heights: state line"), 0.1f, 1.17f);
	Scatter(P3D("Scaffold_A"), 46.f, 54.f, 4, 0, 0.85f, 1.2f, 0.05f, -1, 6.f);
	Scatter(P3D("WaterTower"), 46.f, 56.f, 4, 2, 0.85f, 1.2f, 0.05f, -1, 6.f);
	Scatter(P3D("Billboard"), 46.f, 54.f, 4, 1, 0.85f, 1.2f, 0.05f, 1, 6.f);
	Scatter(P3D("RVRoof"), 44.f, 50.f, 4, 0, 0.85f, 1.15f, 0.05f, 1, 6.f);
Zone(TEXT("P3D heights: dry wash, the first cut"), 1.17f, 1.335f);
	Scatter(P3D("RockSpire_B"), 44.f, 50.f, 4, 1, 0.8f, 1.f, 0.05f, -1, 6.f);
	Scatter(P3D("RockSpire_B"), 44.f, 50.f, 4, 3, 0.8f, 1.f, 0.05f, 1, 6.f);
Zone(TEXT("P3D heights: dry wash, the long cut"), 1.61f, 2.06f);
	Scatter(P3D("RockSpire_B"), 44.f, 50.f, 4, 1, 0.8f, 1.f, 0.05f, -1, 6.f);
	Scatter(P3D("RockSpire_B"), 44.f, 50.f, 4, 3, 0.8f, 1.f, 0.05f, 1, 6.f);
Zone(TEXT("P3D heights: dry wash, open"), 2.06f, 2.43f);
	Scatter(P3D("MesaLedge"), 48.f, 58.f, 2, 0, 0.85f, 1.15f, 0.05f, -1, 6.f);
	Scatter(P3D("RockSpire_A"), 46.f, 56.f, 2, 1, 0.85f, 1.15f, 0.05f, 1, 6.f);
Zone(TEXT("P3D heights: snake eyes country"), 2.43f, 3.83f);
	Scatter(P3D("WaterTower"), 46.f, 56.f, 4, 0, 0.8f, 1.2f, 0.05f, -1, 6.f);
	Scatter(P3D("Scaffold_B"), 46.f, 54.f, 4, 2, 0.8f, 1.2f, 0.05f, -1, 6.f);
	Scatter(P3D("Billboard"), 46.f, 54.f, 4, 1, 0.8f, 1.2f, 0.05f, 1, 6.f);
	Scatter(P3D("RVRoof"), 44.f, 50.f, 4, 0, 0.8f, 1.15f, 0.05f, 1, 6.f);
	Scatter(P3D("RoofParty"), 47.f, 54.f, 8, 1, 0.8f, 1.15f, 0.05f, -1, 6.f);
	Scatter(P3D("Trestle"), 48.f, 56.f, 8, 6, 0.8f, 1.1f, 0.05f, 1, 6.f);
Zone(TEXT("P3D heights: joshua flats"), 3.83f, 5.14f);
	Scatter(P3D("Lookout"), 46.f, 56.f, 4, 0, 0.8f, 1.2f, 0.05f, -1, 6.f);
	Scatter(P3D("Derrick"), 48.f, 58.f, 4, 2, 0.8f, 1.15f, 0.05f, -1, 6.f);
	Scatter(P3D("WaterTower"), 46.f, 56.f, 4, 1, 0.8f, 1.2f, 0.05f, 1, 6.f);
	Scatter(P3D("RVRoof"), 44.f, 52.f, 4, 3, 0.8f, 1.15f, 0.05f, 1, 6.f);
	Scatter(P3D("Trestle"), 50.f, 58.f, 8, 1, 0.8f, 1.1f, 0.05f, -1, 6.f);
Zone(TEXT("P3D heights: red rock pass, wall bend right"), 5.14f, 5.4f);
	Scatter(P3D("RockSpire_B"), 44.f, 50.f, 2, 1, 0.8f, 1.f, 0.05f, 1, 6.f);
Zone(TEXT("P3D heights: red rock pass, wall bend left"), 5.75f, 6.04f);
	Scatter(P3D("RockSpire_B"), 44.f, 50.f, 2, 0, 0.8f, 1.f, 0.05f, -1, 6.f);
Zone(TEXT("P3D heights: red rock pass, the east cut"), 6.04f, 6.31f);
	Scatter(P3D("RockSpire_B"), 44.f, 50.f, 2, 0, 0.8f, 1.f, 0.05f, -1, 6.f);
	Scatter(P3D("RockSpire_B"), 44.f, 50.f, 2, 1, 0.8f, 1.f, 0.05f, 1, 6.f);
Zone(TEXT("P3D heights: last chance gas"), 6.31f, 7.75f);
	Scatter(P3D("RoofParty"), 47.f, 54.f, 4, 0, 0.8f, 1.15f, 0.05f, -1, 6.f);
	Scatter(P3D("Billboard"), 46.f, 54.f, 4, 2, 0.8f, 1.2f, 0.05f, -1, 6.f);
	Scatter(P3D("WaterTower"), 47.f, 56.f, 4, 1, 0.8f, 1.2f, 0.05f, 1, 6.f);
	Scatter(P3D("Scaffold_A"), 46.f, 54.f, 8, 7, 0.8f, 1.2f, 0.05f, 1, 6.f);
Zone(TEXT("P3D heights: jackpot junction"), 7.75f, 9.05f);
	Scatter(P3D("SignRig"), 49.f, 56.f, 4, 0, 0.85f, 1.15f, 0.05f, -1, 6.f);
	Scatter(P3D("Scaffold_B"), 46.f, 50.f, 4, 2, 0.85f, 1.2f, 0.05f, -1, 6.f);
	Scatter(P3D("RoofParty"), 48.f, 55.f, 4, 0, 0.85f, 1.15f, 0.05f, 1, 6.f);
	Scatter(P3D("Billboard"), 46.f, 49.f, 4, 2, 0.85f, 1.2f, 0.05f, 1, 6.f);
Zone(TEXT("P3D heights: city lights"), 9.05f, 10.69f);
	Scatter(P3D("RoofParty"), 50.f, 57.f, 4, 0, 0.85f, 1.15f, 0.05f, -1, 6.f);
	Scatter(P3D("SignRig"), 50.f, 57.f, 4, 2, 0.85f, 1.15f, 0.05f, -1, 6.f);
	Scatter(P3D("SignRig"), 50.f, 57.f, 4, 0, 0.85f, 1.15f, 0.05f, 1, 6.f);
	Scatter(P3D("Scaffold_A"), 50.f, 56.f, 4, 2, 0.85f, 1.2f, 0.05f, 1, 6.f);
Zone(TEXT("P3D heights: salt flats"), 10.69f, 14.5f);
	Scatter(P3D("RVRoof"), 44.f, 52.f, 4, 0, 0.7f, 1.15f, 0.05f, -1, 6.f);
	Scatter(P3D("Derrick"), 48.f, 58.f, 4, 2, 0.7f, 1.15f, 0.05f, -1, 6.f);
	Scatter(P3D("Scaffold_B"), 46.f, 54.f, 4, 1, 0.7f, 1.2f, 0.05f, 1, 6.f);
	Scatter(P3D("Lookout"), 46.f, 56.f, 4, 3, 0.7f, 1.2f, 0.05f, 1, 6.f);
	Scatter(P3D("Trestle"), 50.f, 58.f, 8, 1, 0.7f, 1.1f, 0.05f, -1, 6.f);
	Scatter(P3D("Scaffold_A"), 46.f, 54.f, 8, 0, 0.7f, 1.2f, 0.05f, 1, 6.f);
	Scatter(P3D("WaterTower"), 46.f, 56.f, 8, 7, 0.7f, 1.2f, 0.05f, -1, 6.f);
	Scatter(P3D("Billboard"), 46.f, 54.f, 8, 6, 0.7f, 1.2f, 0.05f, 1, 6.f);
Zone(TEXT("P3D heights: syndicate range"), 14.5f, 18.5f);
	Scatter(P3D("Lookout"), 46.f, 56.f, 4, 1, 0.7f, 1.2f, 0.05f, -1, 6.f);
	Scatter(P3D("MesaLedge"), 48.f, 58.f, 4, 3, 0.7f, 1.15f, 0.05f, -1, 6.f);
	Scatter(P3D("RockSpire_A"), 46.f, 56.f, 4, 1, 0.7f, 1.15f, 0.05f, 1, 6.f);
	Scatter(P3D("Derrick"), 48.f, 58.f, 4, 3, 0.7f, 1.15f, 0.05f, 1, 6.f);
	Scatter(P3D("WaterTower"), 60.f, 68.f, 8, 0, 0.7f, 1.2f, 0.05f, -1, 6.f);
	Scatter(P3D("Trestle"), 60.f, 68.f, 8, 4, 0.7f, 1.1f, 0.05f, 1, 6.f);
Zone(TEXT("P3D heights: red rock return, the first cut"), 18.57f, 19.34f);
	Scatter(P3D("RockSpire_B"), 44.f, 50.f, 3, 0, 0.8f, 1.f, 0.05f, -1, 6.f);
	Scatter(P3D("RockSpire_B"), 44.f, 50.f, 3, 1, 0.8f, 1.f, 0.05f, 1, 6.f);
Zone(TEXT("P3D heights: red rock return, the second cut"), 20.02f, 20.61f);
	Scatter(P3D("RockSpire_B"), 44.f, 50.f, 3, 0, 0.8f, 1.f, 0.05f, -1, 6.f);
	Scatter(P3D("RockSpire_B"), 44.f, 50.f, 3, 1, 0.8f, 1.f, 0.05f, 1, 6.f);
Zone(TEXT("P3D heights: red rock return, the third cut"), 21.33f, 22.f);
	Scatter(P3D("RockSpire_B"), 44.f, 50.f, 3, 0, 0.8f, 1.f, 0.05f, -1, 6.f);
	Scatter(P3D("RockSpire_B"), 44.f, 50.f, 3, 1, 0.8f, 1.f, 0.05f, 1, 6.f);
Zone(TEXT("P3D heights: red rock return, the first gap"), 19.34f, 20.02f);
	Scatter(P3D("RockSpire_A"), 46.f, 56.f, 4, 1, 0.85f, 1.15f, 0.05f, -1, 6.f);
	Scatter(P3D("MesaLedge"), 48.f, 58.f, 4, 3, 0.85f, 1.15f, 0.05f, -1, 6.f);
	Scatter(P3D("MesaLedge"), 48.f, 58.f, 4, 1, 0.85f, 1.15f, 0.05f, 1, 6.f);
	Scatter(P3D("Lookout"), 46.f, 56.f, 4, 3, 0.85f, 1.2f, 0.05f, 1, 6.f);
	Scatter(P3D("Trestle"), 60.f, 68.f, 8, 0, 0.85f, 1.1f, 0.05f, -1, 6.f);
Zone(TEXT("P3D heights: red rock return, the second gap"), 20.61f, 21.33f);
	Scatter(P3D("RockSpire_A"), 46.f, 56.f, 4, 1, 0.85f, 1.15f, 0.05f, -1, 6.f);
	Scatter(P3D("MesaLedge"), 48.f, 58.f, 4, 3, 0.85f, 1.15f, 0.05f, -1, 6.f);
	Scatter(P3D("MesaLedge"), 48.f, 58.f, 4, 1, 0.85f, 1.15f, 0.05f, 1, 6.f);
	Scatter(P3D("Lookout"), 46.f, 56.f, 4, 3, 0.85f, 1.2f, 0.05f, 1, 6.f);
	Scatter(P3D("Trestle"), 60.f, 68.f, 8, 0, 0.85f, 1.1f, 0.05f, -1, 6.f);
Zone(TEXT("P3D heights: vegas approach"), 22.f, 25.6f);
	Scatter(P3D("SignRig"), 50.f, 58.f, 6, 1, 0.85f, 1.15f, 0.05f, -1, 6.f);
	Scatter(P3D("RoofParty"), 50.f, 57.f, 6, 3, 0.85f, 1.15f, 0.05f, -1, 6.f);
	Scatter(P3D("Scaffold_B"), 50.f, 56.f, 6, 5, 0.85f, 1.2f, 0.05f, -1, 6.f);
	Scatter(P3D("Scaffold_A"), 50.f, 56.f, 6, 1, 0.85f, 1.2f, 0.05f, 1, 6.f);
	Scatter(P3D("SignRig"), 50.f, 58.f, 6, 3, 0.85f, 1.15f, 0.05f, 1, 6.f);
	Scatter(P3D("Billboard"), 50.f, 56.f, 6, 5, 0.85f, 1.2f, 0.05f, 1, 6.f);
#undef P3D
// ---- END p3d ----
}

{
// ---- BEGIN p3e (far and sky, ft_people_nevada_p3e.py) ----
// Pass 3, builder e: life from 150 m to the horizon and in the air over Highway 93, on BOTH sides. Twelve meshes, every one
// TALL and saved with the always-moving master (far=True); no rule of this block places anything nearer than 160 m to the
// centre line (the widest ring, the hang gliders, reaches 33 m from its middle: 260 m less 33 m and the jitter).
// IN THE AIR (Far; the height is built into the mesh, 25 to 100 m, so the drop is the level of the land or a little above it):
//   balloons (230 m and out), hang gliders, paragliders and parachutists wheeling, a skein of snow geese and a line of powered
//   paragliders that travel along the road (yaw jitter 6 degrees: they never turn towards it). Over the big houses of Jackpot
//   Junction, the city lights and the Vegas approach everything in the air stands 230 m out or more, beyond the houses.
// ON THE LAND (Scatter, 165 to 250 m, each on the pale ground it brings): a kite meet, dirt bike riders, land yachts on the
//   salt, climbers on a 50 m rock tower, a rodeo. In each district they take the band the herds of the first round left free
//   (herds at 140-195 m: these at 198-250 m; herds at 215-288 m: these at 165-190 m).
// IN THE ROCK CUTS (dry wash, Red Rock Pass, the red rock return) the land beyond the rim cannot be seen: only balloons and
//   parachutists, lifted 110 to 120 m more (a negative drop), 160 to 200 m out, so that they stand over the rim.
// Counted slot by slot (45.243 m slots, every/phase against each zone): 444 pieces, the most of one mesh 44 (Balloons_A 36, Balloons_B 44, Climbers 32, DirtBikes 32, Geese 39, HangGliders 34, Kites 39, LandYachts 40, ParaGliders 38, Paramotors 36, Rodeo 31, Skydivers 43).
// FIX ROUND (after the two reviews): in the helicopter scenes (dry wash to its long cut, Joshua Flats, Last Chance Gas) the hang gliders
// and powered paragliders are out and the canopies stand 300 m and more out; in every open district the sky pieces of one side share
// one "every" (12) with phases that never meet; the balloons of the salt flats are lifted 40 m; Red Rock Pass has balloons on both sides
// every third slot (Balloons_A 160 m out there too); rules whose phase met no slot of their zone were given one that does. By count now: 480 pieces (Balloons_A 54, Balloons_B 58, Climbers 33, DirtBikes 31, Geese 37, HangGliders 36, Kites 37, LandYachts 40, ParaGliders 42, Paramotors 32, Rodeo 28, Skydivers 52).
#define P3E(n) TEXT("/Game/People/Nevada/SM_P3E_" n)
Zone(TEXT("People p3e far and sky: the state line"), 0.1f, 1.15f);   // open land: the whole sky and the far ground
	Far(P3E("Balloons_A"), 330.f, 520.f, 12, 9, 1.f, 1.3f, 0.12f, 1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 240.f, 330.f, 12, 9, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("HangGliders"), 260.f, 360.f, 12, 6, 1.f, 1.f, 0.1f, 1, -5.f, 180.f);
	Far(P3E("ParaGliders"), 230.f, 300.f, 12, 5, 1.f, 1.f, 0.1f, -1, 0.f, 180.f);
	Far(P3E("Skydivers"), 200.f, 290.f, 12, 3, 1.f, 1.f, 0.1f, 1, 0.f, 180.f);
	Far(P3E("Geese"), 170.f, 260.f, 12, 1, 1.f, 1.f, 0.08f, -1, 0.f, 6.f);
	Far(P3E("Paramotors"), 180.f, 250.f, 12, 0, 1.f, 1.f, 0.08f, 1, 0.f, 6.f);
	Scatter(P3E("Kites"), 205.f, 250.f, 9, 1, 1.f, 1.2f, 0.08f, -1, 8.f);
	Scatter(P3E("DirtBikes"), 205.f, 250.f, 10, 5, 1.f, 1.2f, 0.08f, 1, 5.f);
	Scatter(P3E("Rodeo"), 205.f, 250.f, 12, 9, 1.f, 1.2f, 0.08f, 1, 8.f);
Zone(TEXT("People p3e far and sky: the dry wash, its west cut"), 1.15f, 1.34f);   // a rock cut: only what flies high over the rim
	Far(P3E("Balloons_B"), 160.f, 200.f, 9, 1, 1.f, 1.f, 0.1f, -1, -120.f, 180.f);
	Far(P3E("Skydivers"), 300.f, 360.f, 9, 7, 1.f, 1.f, 0.1f, 1, -110.f, 180.f);
Zone(TEXT("People p3e far and sky: the dry wash, between its cuts"), 1.34f, 1.6f);   // open land: the whole sky and the far ground
	Far(P3E("Balloons_A"), 330.f, 520.f, 20, 9, 1.f, 1.3f, 0.12f, 1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 240.f, 330.f, 20, 9, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("ParaGliders"), 300.f, 360.f, 20, 7, 1.f, 1.f, 0.1f, -1, 0.f, 180.f);
	Far(P3E("Skydivers"), 300.f, 360.f, 20, 6, 1.f, 1.f, 0.1f, 1, 0.f, 180.f);
	Far(P3E("Geese"), 170.f, 260.f, 20, 5, 1.f, 1.f, 0.08f, -1, 0.f, 6.f);
	Scatter(P3E("Kites"), 165.f, 200.f, 5, 0, 1.f, 1.f, 0.08f, -1, 8.f);
Zone(TEXT("People p3e far and sky: the dry wash, its long cut"), 1.6f, 2.07f);   // a rock cut: only what flies high over the rim
	Far(P3E("Balloons_B"), 160.f, 200.f, 9, 0, 1.f, 1.f, 0.1f, -1, -120.f, 180.f);
	Far(P3E("Skydivers"), 300.f, 360.f, 9, 4, 1.f, 1.f, 0.1f, 1, -110.f, 180.f);
Zone(TEXT("People p3e far and sky: the dry wash, its east end"), 2.07f, 2.43f);   // open land: the whole sky and the far ground
	Far(P3E("Balloons_A"), 330.f, 520.f, 20, 13, 1.f, 1.3f, 0.12f, 1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 240.f, 330.f, 20, 13, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("HangGliders"), 260.f, 360.f, 20, 11, 1.f, 1.f, 0.1f, 1, -5.f, 180.f);
	Far(P3E("ParaGliders"), 230.f, 300.f, 20, 10, 1.f, 1.f, 0.1f, -1, 0.f, 180.f);
	Far(P3E("Skydivers"), 200.f, 290.f, 20, 9, 1.f, 1.f, 0.1f, 1, 0.f, 180.f);
	Far(P3E("Geese"), 170.f, 260.f, 20, 8, 1.f, 1.f, 0.08f, -1, 0.f, 6.f);
	Far(P3E("Paramotors"), 180.f, 250.f, 20, 7, 1.f, 1.f, 0.08f, 1, 0.f, 6.f);
	Scatter(P3E("DirtBikes"), 210.f, 250.f, 7, 2, 1.f, 1.2f, 0.08f, -1, 5.f);
	Scatter(P3E("Climbers"), 210.f, 250.f, 6, 1, 1.f, 1.2f, 0.08f, 1, 12.f);
Zone(TEXT("People p3e far and sky: Snake Eyes country"), 2.43f, 3.83f);   // open land: the whole sky and the far ground
	Far(P3E("Balloons_A"), 330.f, 520.f, 12, 6, 1.f, 1.3f, 0.12f, -1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 240.f, 330.f, 12, 6, 1.f, 1.f, 0.12f, 1, -10.f, 180.f);
	Far(P3E("HangGliders"), 260.f, 360.f, 12, 3, 1.f, 1.f, 0.1f, -1, -5.f, 180.f);
	Far(P3E("ParaGliders"), 230.f, 300.f, 12, 2, 1.f, 1.f, 0.1f, 1, 0.f, 180.f);
	Far(P3E("Skydivers"), 200.f, 290.f, 12, 0, 1.f, 1.f, 0.1f, -1, 0.f, 180.f);
	Far(P3E("Geese"), 170.f, 260.f, 12, 10, 1.f, 1.f, 0.08f, 1, 0.f, 6.f);
	Far(P3E("Paramotors"), 180.f, 250.f, 12, 9, 1.f, 1.f, 0.08f, -1, 0.f, 6.f);
	Scatter(P3E("DirtBikes"), 165.f, 185.f, 11, 0, 1.f, 1.f, 0.08f, -1, 5.f);
	Scatter(P3E("Rodeo"), 165.f, 185.f, 12, 3, 1.f, 1.f, 0.08f, 1, 8.f);
	Scatter(P3E("Kites"), 165.f, 185.f, 13, 6, 1.f, 1.f, 0.08f, 1, 8.f);
	Scatter(P3E("Climbers"), 165.f, 185.f, 14, 9, 1.f, 1.f, 0.08f, -1, 12.f);
Zone(TEXT("People p3e far and sky: Joshua Flats"), 3.83f, 5.12f);   // open land: the whole sky and the far ground
	Far(P3E("Balloons_A"), 330.f, 520.f, 12, 11, 1.f, 1.3f, 0.12f, 1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 240.f, 330.f, 12, 11, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("ParaGliders"), 300.f, 360.f, 12, 7, 1.f, 1.f, 0.1f, -1, 0.f, 180.f);
	Far(P3E("Skydivers"), 300.f, 360.f, 12, 5, 1.f, 1.f, 0.1f, 1, 0.f, 180.f);
	Far(P3E("Geese"), 170.f, 260.f, 12, 3, 1.f, 1.f, 0.08f, -1, 0.f, 6.f);
	Scatter(P3E("Kites"), 198.f, 214.f, 11, 2, 1.f, 1.1f, 0.08f, 1, 8.f);
	Scatter(P3E("DirtBikes"), 198.f, 214.f, 12, 5, 1.f, 1.1f, 0.08f, -1, 5.f);
	Scatter(P3E("LandYachts"), 198.f, 214.f, 13, 8, 1.f, 1.1f, 0.08f, 1, 5.f);
	Scatter(P3E("Climbers"), 198.f, 214.f, 12, 1, 1.f, 1.1f, 0.08f, -1, 12.f);
Zone(TEXT("People p3e far and sky: Red Rock Pass"), 5.12f, 6.32f);   // a rock cut: only what flies high over the rim
	Far(P3E("Balloons_B"), 160.f, 200.f, 6, 0, 1.f, 1.f, 0.1f, -1, -120.f, 180.f);
	Far(P3E("Balloons_A"), 160.f, 200.f, 6, 5, 1.f, 1.f, 0.1f, 1, -120.f, 180.f);
	Far(P3E("Balloons_A"), 160.f, 200.f, 6, 3, 1.f, 1.f, 0.1f, -1, -120.f, 180.f);
	Far(P3E("Skydivers"), 160.f, 200.f, 6, 2, 1.f, 1.f, 0.1f, 1, -110.f, 180.f);
Zone(TEXT("People p3e far and sky: Last Chance Gas"), 6.32f, 7.75f);   // open land: the whole sky and the far ground
	Far(P3E("Balloons_A"), 330.f, 520.f, 12, 4, 1.f, 1.3f, 0.12f, 1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 240.f, 330.f, 12, 4, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("ParaGliders"), 300.f, 360.f, 12, 0, 1.f, 1.f, 0.1f, -1, 0.f, 180.f);
	Far(P3E("Skydivers"), 300.f, 360.f, 12, 10, 1.f, 1.f, 0.1f, 1, 0.f, 180.f);
	Far(P3E("Geese"), 170.f, 260.f, 12, 8, 1.f, 1.f, 0.08f, -1, 0.f, 6.f);
	Scatter(P3E("Rodeo"), 165.f, 190.f, 11, 6, 1.f, 1.f, 0.08f, 1, 8.f);
	Scatter(P3E("DirtBikes"), 165.f, 190.f, 12, 9, 1.f, 1.f, 0.08f, -1, 5.f);
	Scatter(P3E("Kites"), 165.f, 190.f, 14, 11, 1.f, 1.f, 0.08f, -1, 8.f);
	Scatter(P3E("Climbers"), 165.f, 190.f, 13, 3, 1.f, 1.f, 0.08f, 1, 12.f);
Zone(TEXT("People p3e far and sky: Jackpot Junction"), 7.75f, 9.05f);   // the sky beyond the big houses, the far ground behind them
	Far(P3E("Balloons_A"), 360.f, 520.f, 12, 8, 1.f, 1.3f, 0.12f, -1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 250.f, 340.f, 12, 8, 1.f, 1.f, 0.12f, 1, -10.f, 180.f);
	Far(P3E("HangGliders"), 280.f, 360.f, 12, 5, 1.f, 1.f, 0.1f, -1, -5.f, 180.f);
	Far(P3E("ParaGliders"), 240.f, 310.f, 12, 4, 1.f, 1.f, 0.1f, 1, 0.f, 180.f);
	Far(P3E("Skydivers"), 230.f, 300.f, 12, 2, 1.f, 1.f, 0.1f, -1, 0.f, 180.f);
	Far(P3E("Geese"), 230.f, 300.f, 12, 0, 1.f, 1.f, 0.08f, 1, 0.f, 6.f);
	Far(P3E("Paramotors"), 235.f, 300.f, 12, 11, 1.f, 1.f, 0.08f, -1, 0.f, 6.f);
	Scatter(P3E("Rodeo"), 170.f, 240.f, 10, 9, 1.f, 1.1f, 0.08f, -1, 8.f);
	Scatter(P3E("Kites"), 170.f, 240.f, 9, 5, 1.f, 1.1f, 0.08f, 1, 8.f);
	Scatter(P3E("DirtBikes"), 170.f, 240.f, 12, 3, 1.f, 1.1f, 0.08f, 1, 5.f);
Zone(TEXT("People p3e far and sky: the city lights"), 9.05f, 10.69f);   // the sky beyond the big houses, the far ground behind them
	Far(P3E("Balloons_A"), 360.f, 520.f, 12, 3, 1.f, 1.3f, 0.12f, 1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 250.f, 340.f, 12, 3, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("HangGliders"), 280.f, 360.f, 12, 0, 1.f, 1.f, 0.1f, 1, -5.f, 180.f);
	Far(P3E("ParaGliders"), 240.f, 310.f, 12, 11, 1.f, 1.f, 0.1f, -1, 0.f, 180.f);
	Far(P3E("Skydivers"), 230.f, 300.f, 12, 9, 1.f, 1.f, 0.1f, 1, 0.f, 180.f);
	Far(P3E("Geese"), 230.f, 300.f, 12, 7, 1.f, 1.f, 0.08f, -1, 0.f, 6.f);
	Far(P3E("Paramotors"), 235.f, 300.f, 12, 6, 1.f, 1.f, 0.08f, 1, 0.f, 6.f);
	Scatter(P3E("Kites"), 170.f, 235.f, 10, 1, 1.f, 1.1f, 0.08f, -1, 8.f);
	Scatter(P3E("Rodeo"), 170.f, 235.f, 11, 3, 1.f, 1.1f, 0.08f, 1, 8.f);
	Scatter(P3E("LandYachts"), 170.f, 235.f, 12, 5, 1.f, 1.1f, 0.08f, -1, 5.f);
Zone(TEXT("People p3e far and sky: the salt flats"), 10.69f, 14.5f);   // open land: the whole sky and the far ground
	Far(P3E("Balloons_A"), 330.f, 520.f, 12, 3, 1.f, 1.3f, 0.12f, -1, -40.f, 180.f);
	Far(P3E("Balloons_B"), 240.f, 330.f, 12, 3, 1.f, 1.f, 0.12f, 1, -40.f, 180.f);
	Far(P3E("HangGliders"), 260.f, 360.f, 12, 0, 1.f, 1.f, 0.1f, -1, -5.f, 180.f);
	Far(P3E("ParaGliders"), 230.f, 300.f, 12, 11, 1.f, 1.f, 0.1f, 1, 0.f, 180.f);
	Far(P3E("Skydivers"), 200.f, 290.f, 12, 9, 1.f, 1.f, 0.1f, -1, 0.f, 180.f);
	Far(P3E("Geese"), 170.f, 260.f, 12, 7, 1.f, 1.f, 0.08f, 1, 0.f, 6.f);
	Far(P3E("Paramotors"), 180.f, 250.f, 12, 6, 1.f, 1.f, 0.08f, -1, 0.f, 6.f);
	Scatter(P3E("LandYachts"), 198.f, 214.f, 7, 2, 1.f, 1.1f, 0.08f, -1, 5.f);
	Scatter(P3E("LandYachts"), 198.f, 214.f, 8, 3, 1.f, 1.1f, 0.08f, 1, 5.f);
	Scatter(P3E("DirtBikes"), 198.f, 214.f, 17, 14, 1.f, 1.1f, 0.08f, 1, 5.f);
	Scatter(P3E("Kites"), 198.f, 214.f, 19, 16, 1.f, 1.1f, 0.08f, -1, 8.f);
	Scatter(P3E("Climbers"), 198.f, 214.f, 21, 18, 1.f, 1.1f, 0.08f, 1, 12.f);
Zone(TEXT("People p3e far and sky: the Syndicate range"), 14.5f, 18.5f);   // open land: the whole sky and the far ground
	Far(P3E("Balloons_A"), 330.f, 520.f, 12, 3, 1.f, 1.3f, 0.12f, 1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 240.f, 330.f, 12, 3, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("HangGliders"), 260.f, 360.f, 12, 0, 1.f, 1.f, 0.1f, 1, -5.f, 180.f);
	Far(P3E("ParaGliders"), 230.f, 300.f, 12, 11, 1.f, 1.f, 0.1f, -1, 0.f, 180.f);
	Far(P3E("Skydivers"), 200.f, 290.f, 12, 9, 1.f, 1.f, 0.1f, 1, 0.f, 180.f);
	Far(P3E("Geese"), 170.f, 260.f, 12, 7, 1.f, 1.f, 0.08f, -1, 0.f, 6.f);
	Far(P3E("Paramotors"), 180.f, 250.f, 12, 6, 1.f, 1.f, 0.08f, 1, 0.f, 6.f);
	Scatter(P3E("Climbers"), 165.f, 185.f, 11, 3, 1.f, 1.f, 0.08f, -1, 12.f);
	Scatter(P3E("Climbers"), 165.f, 185.f, 13, 3, 1.f, 1.f, 0.08f, 1, 12.f);
	Scatter(P3E("DirtBikes"), 165.f, 185.f, 16, 1, 1.f, 1.f, 0.08f, -1, 5.f);
	Scatter(P3E("LandYachts"), 165.f, 185.f, 14, 9, 1.f, 1.f, 0.08f, 1, 5.f);
	Scatter(P3E("Rodeo"), 165.f, 185.f, 19, 3, 1.f, 1.f, 0.08f, -1, 8.f);
Zone(TEXT("People p3e far and sky: the red rock return, its first cut"), 18.5f, 19.36f);   // a rock cut: only what flies high over the rim
	Far(P3E("Balloons_B"), 160.f, 200.f, 9, 3, 1.f, 1.f, 0.1f, -1, -120.f, 180.f);
	Far(P3E("Skydivers"), 160.f, 200.f, 9, 7, 1.f, 1.f, 0.1f, 1, -110.f, 180.f);
Zone(TEXT("People p3e far and sky: the red rock return, the first gap"), 19.36f, 19.92f);   // open land: the whole sky and the far ground
	Far(P3E("Balloons_A"), 330.f, 520.f, 12, 4, 1.f, 1.3f, 0.12f, 1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 240.f, 330.f, 12, 4, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("HangGliders"), 260.f, 360.f, 12, 1, 1.f, 1.f, 0.1f, 1, -5.f, 180.f);
	Far(P3E("ParaGliders"), 230.f, 300.f, 12, 0, 1.f, 1.f, 0.1f, -1, 0.f, 180.f);
	Far(P3E("Skydivers"), 200.f, 290.f, 12, 10, 1.f, 1.f, 0.1f, 1, 0.f, 180.f);
	Far(P3E("Geese"), 170.f, 260.f, 12, 8, 1.f, 1.f, 0.08f, -1, 0.f, 6.f);
	Far(P3E("Paramotors"), 180.f, 250.f, 12, 7, 1.f, 1.f, 0.08f, 1, 0.f, 6.f);
	Scatter(P3E("Climbers"), 205.f, 245.f, 6, 5, 1.f, 1.2f, 0.08f, -1, 12.f);
	Scatter(P3E("Kites"), 205.f, 245.f, 6, 3, 1.f, 1.2f, 0.08f, 1, 8.f);
Zone(TEXT("People p3e far and sky: the red rock return, its second cut"), 19.92f, 20.72f);   // a rock cut: only what flies high over the rim
	Far(P3E("Balloons_B"), 160.f, 200.f, 9, 0, 1.f, 1.f, 0.1f, -1, -120.f, 180.f);
	Far(P3E("Skydivers"), 160.f, 200.f, 9, 4, 1.f, 1.f, 0.1f, 1, -110.f, 180.f);
Zone(TEXT("People p3e far and sky: the red rock return, the second gap"), 20.72f, 21.32f);   // open land: the whole sky and the far ground
	Far(P3E("Balloons_A"), 330.f, 520.f, 12, 10, 1.f, 1.3f, 0.12f, 1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 240.f, 330.f, 12, 10, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("HangGliders"), 260.f, 360.f, 12, 7, 1.f, 1.f, 0.1f, 1, -5.f, 180.f);
	Far(P3E("ParaGliders"), 230.f, 300.f, 12, 6, 1.f, 1.f, 0.1f, -1, 0.f, 180.f);
	Far(P3E("Skydivers"), 200.f, 290.f, 12, 4, 1.f, 1.f, 0.1f, 1, 0.f, 180.f);
	Far(P3E("Geese"), 170.f, 260.f, 12, 2, 1.f, 1.f, 0.08f, -1, 0.f, 6.f);
	Far(P3E("Paramotors"), 180.f, 250.f, 12, 1, 1.f, 1.f, 0.08f, 1, 0.f, 6.f);
	Scatter(P3E("Climbers"), 205.f, 245.f, 6, 3, 1.f, 1.2f, 0.08f, 1, 12.f);
	Scatter(P3E("DirtBikes"), 205.f, 245.f, 7, 2, 1.f, 1.2f, 0.08f, -1, 5.f);
Zone(TEXT("People p3e far and sky: the red rock return, its last cut"), 21.32f, 22.03f);   // a rock cut: only what flies high over the rim
	Far(P3E("Balloons_B"), 160.f, 200.f, 9, 6, 1.f, 1.f, 0.1f, -1, -120.f, 180.f);
	Far(P3E("Skydivers"), 160.f, 200.f, 9, 1, 1.f, 1.f, 0.1f, 1, -110.f, 180.f);
Zone(TEXT("People p3e far and sky: the Vegas approach"), 22.03f, 25.6f);   // the sky beyond the big houses, the far ground behind them
	Far(P3E("Balloons_A"), 360.f, 520.f, 12, 5, 1.f, 1.3f, 0.12f, 1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 250.f, 340.f, 12, 5, 1.f, 1.f, 0.12f, -1, -10.f, 180.f);
	Far(P3E("HangGliders"), 280.f, 360.f, 12, 2, 1.f, 1.f, 0.1f, 1, -5.f, 180.f);
	Far(P3E("ParaGliders"), 240.f, 310.f, 12, 1, 1.f, 1.f, 0.1f, -1, 0.f, 180.f);
	Far(P3E("Skydivers"), 230.f, 300.f, 12, 11, 1.f, 1.f, 0.1f, 1, 0.f, 180.f);
	Far(P3E("Geese"), 230.f, 300.f, 12, 9, 1.f, 1.f, 0.08f, -1, 0.f, 6.f);
	Far(P3E("Paramotors"), 235.f, 300.f, 12, 8, 1.f, 1.f, 0.08f, 1, 0.f, 6.f);
	Scatter(P3E("Kites"), 170.f, 235.f, 12, 1, 1.f, 1.1f, 0.08f, -1, 8.f);
	Scatter(P3E("Kites"), 170.f, 235.f, 14, 13, 1.f, 1.1f, 0.08f, 1, 8.f);
	Scatter(P3E("Rodeo"), 170.f, 235.f, 13, 6, 1.f, 1.1f, 0.08f, -1, 8.f);
	Scatter(P3E("Rodeo"), 170.f, 235.f, 16, 1, 1.f, 1.1f, 0.08f, 1, 8.f);
	Scatter(P3E("LandYachts"), 170.f, 235.f, 15, 8, 1.f, 1.1f, 0.08f, 1, 5.f);
	Scatter(P3E("DirtBikes"), 170.f, 235.f, 17, 6, 1.f, 1.1f, 0.08f, -1, 5.f);
#undef P3E
// ---- END p3e ----
}

{
// ---- BEGIN p3f (film shoot, ft_people_nevada_p3f.py) ----
// MORE OF THE SHOOT (pass 3, builder p3f). New units, Content/Python/ft_people_nevada_p3f.py -> /Game/People/Nevada/SM_P3F_*
// (second unit A/B, the camera truck, drone teams A/B, stunt rigging A/B, the extras' holding A/B, make-up, the press
// pen A/B, the camera tower: every one TALL, its people on a riser 2.2 m up or higher, nothing lit), and more of the kit
// /Game/People/Crew/SM_Crew<Piece>_Desert (its people-on-the-ground pieces 2 m up on the level's own SM_CrewRiser, as
// the first round stood them; the crane and the lights on the ground).
// Slot by slot, like the table above (slot s lies s * 45.243 m along the road): one Site rule a row, in a zone of its own.
// WHERE: by the road, HALF A SLOT ON from the slot (the sets ...H: 22.6 m along), in the gap between the first cast's
// decks of two neighbouring slots: 26.8 to 29.3 m out (the kit's crane 31.3 m), each front 24.9 m out or more, the
// back 30.2 m out at most (in front of the half-slot scenes of p3b at 37 m and the berms of p3c at 36 m, never on the
// side of a slot where p3a or p3b stand on the half slot; never before a truck stop). On the phone line's side as
// built (between two poles, in front of the wire), on the other side 1.2 times (the kit 1.15).
// In the open bends between the rock cuts (dry wash, red rock return) only the two towers, on the slot, 41 to 41.5 m
// out behind the rock banks. Nothing in Red Rock Pass, in the rock cuts, beside a loop, a corkscrew or a jump, nor
// beside the shallow banked bends (4.3, 4.4, 7.8, 8.0 km).
// Every rule's distance is 26.8 m or more (all of these are TALL: x min >= 24 everywhere). No Lit().
// 325 rows = 325 placed pieces by this block's own count (before the track's own clearance); the most any one mesh is
// placed: 31 (SM_P3F_CamTower), the level's SM_CrewRiser 53 times. Written by C:/ftwork/jobs/L2p3f/table.py (scratch).
#define P3F(n) TEXT("/Game/People/Nevada/SM_P3F_" n)
#define P3FK(n) TEXT("/Game/People/Crew/SM_Crew" n "_Desert")
Zone(TEXT("People p3f"), 0.f, 0.01f);
Set(TEXT("PplP3FSecondUnitAH"), { { P3F("SecondUnit_A"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3FSecondUnitBH"), { { P3F("SecondUnit_B"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3FCamTruckH"), { { P3F("CamTruck"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3FDroneTeamAH"), { { P3F("DroneTeam_A"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3FDroneTeamBH"), { { P3F("DroneTeam_B"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3FStuntRigAH"), { { P3F("StuntRig_A"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3FStuntRigBH"), { { P3F("StuntRig_B"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3FExtrasAH"), { { P3F("Extras_A"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3FExtrasBH"), { { P3F("Extras_B"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3FMakeUpH"), { { P3F("MakeUp"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3FPressPenAH"), { { P3F("PressPen_A"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3FPressPenBH"), { { P3F("PressPen_B"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3FCamTowerH"), { { P3F("CamTower"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3FCamTower"), { { P3F("CamTower"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3FStuntRigB"), { { P3F("StuntRig_B"), 0, 0, 0, 0, 1.f } });
Set(TEXT("PplP3FKCameraH"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 22.6f, 0, 0, 0, 1.f }, { P3FK("Camera"), 22.6f, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplP3FKApplauseAH"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 22.6f, 0, 0, 0, 1.f }, { P3FK("ApplauseA"), 22.6f, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplP3FKApplauseBH"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 22.6f, 0, 0, 0, 1.f }, { P3FK("ApplauseB"), 22.6f, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplP3FKStuntH"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 22.6f, 0, 0, 0, 1.f }, { P3FK("Stunt"), 22.6f, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplP3FKVillageH"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 22.6f, 0, 0, 0, 1.f }, { P3FK("Village"), 22.6f, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplP3FKDollyH"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 22.6f, 0, 0, 0, 1.f }, { P3FK("Dolly"), 22.6f, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplP3FKCateringH"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 22.6f, 0, 0, 0, 1.f }, { P3FK("Catering"), 22.6f, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplP3FKSoundH"), { { TEXT("/Game/People/Nevada/SM_CrewRiser"), 22.6f, 0, 0, 0, 1.f }, { P3FK("Sound"), 22.6f, 0, 2.0f, 0, 1.f } });
Set(TEXT("PplP3FKCraneH"), { { P3FK("Crane"), 22.6f, 0, 0, 0, 1.f } });
Set(TEXT("PplP3FKLightsH"), { { P3FK("Lights"), 22.6f, 0, 0, 0, 1.f } });
{
	static const TCHAR* const PplP3FSets[] = { TEXT("PplP3FSecondUnitAH"), TEXT("PplP3FSecondUnitBH"), TEXT("PplP3FCamTruckH"), TEXT("PplP3FDroneTeamAH"), TEXT("PplP3FDroneTeamBH"), TEXT("PplP3FStuntRigAH"), TEXT("PplP3FStuntRigBH"), TEXT("PplP3FExtrasAH"), TEXT("PplP3FExtrasBH"), TEXT("PplP3FMakeUpH"), TEXT("PplP3FPressPenAH"), TEXT("PplP3FPressPenBH"), TEXT("PplP3FCamTowerH"), TEXT("PplP3FCamTower"), TEXT("PplP3FStuntRigB"), TEXT("PplP3FKCameraH"), TEXT("PplP3FKApplauseAH"), TEXT("PplP3FKApplauseBH"), TEXT("PplP3FKStuntH"), TEXT("PplP3FKVillageH"), TEXT("PplP3FKDollyH"), TEXT("PplP3FKCateringH"), TEXT("PplP3FKSoundH"), TEXT("PplP3FKCraneH"), TEXT("PplP3FKLightsH") };
	struct FPplP3FRow { int16 Slot; int8 Side; uint8 Set; float X, Scale; };
	// slot, side (-1 left of travel, +1 right), set, metres from the centre line, scale
	static const FPplP3FRow PplP3FRows[] = {
	{3,-1,0,27.1f,1.f}, {4,1,3,27.5f,1.2f}, {5,-1,16,27.6f,1.f}, {6,1,7,27.5f,1.2f}, {7,-1,5,27.1f,1.f}, {8,1,10,27.5f,1.2f}, {9,1,23,31.3f,1.1f},
	{10,1,12,27.2f,1.2f}, {11,-1,9,27.1f,1.f}, {12,-1,18,27.6f,1.f}, {14,1,1,27.5f,1.2f}, {15,-1,6,27.1f,1.f}, {16,1,8,27.5f,1.2f}, {17,1,4,27.5f,1.2f},
	{18,1,21,28.f,1.15f}, {19,-1,11,27.1f,1.f}, {21,1,2,28.4f,1.2f}, {22,1,0,27.5f,1.2f}, {23,-1,3,27.1f,1.f}, {24,-1,15,27.6f,1.f}, {48,-1,13,41.f,1.2f},
	{48,1,14,41.5f,1.2f}, {50,-1,14,41.5f,1.2f}, {50,1,13,41.f,1.2f}, {52,-1,13,41.f,1.2f}, {52,1,14,41.5f,1.2f}, {55,-1,7,27.5f,1.2f}, {56,1,5,27.1f,1.f},
	{57,-1,10,27.5f,1.2f}, {58,1,24,26.9f,1.f}, {59,1,12,26.8f,1.f}, {63,-1,9,27.5f,1.2f}, {64,1,19,27.6f,1.f}, {65,-1,1,27.5f,1.2f}, {66,1,6,27.1f,1.f},
	{75,-1,8,27.5f,1.2f}, {79,-1,4,27.5f,1.2f}, {80,1,22,28.7f,1.f}, {81,-1,11,27.5f,1.2f}, {82,1,2,27.8f,1.f}, {83,1,0,27.1f,1.f}, {84,-1,3,27.5f,1.2f},
	{85,1,17,27.6f,1.f}, {87,1,7,27.1f,1.f}, {88,-1,5,27.5f,1.2f}, {89,1,10,27.1f,1.f}, {90,1,24,26.9f,1.f}, {91,-1,12,27.2f,1.2f}, {92,-1,9,27.5f,1.2f},
	{104,-1,20,28.f,1.15f}, {105,-1,1,27.5f,1.2f}, {106,1,6,27.1f,1.f}, {107,-1,8,27.5f,1.2f}, {107,1,4,27.1f,1.f}, {108,-1,21,28.f,1.15f}, {108,1,11,27.1f,1.f},
	{109,1,2,27.8f,1.f}, {111,1,0,27.1f,1.f}, {112,-1,3,27.5f,1.2f}, {143,-1,16,27.6f,1.f}, {145,-1,5,27.1f,1.f}, {147,1,24,27.2f,1.15f}, {151,-1,9,27.1f,1.f},
	{153,-1,1,27.1f,1.f}, {155,-1,8,27.1f,1.f}, {168,1,22,29.3f,1.15f}, {170,1,2,28.4f,1.2f}, {193,-1,3,27.5f,1.2f}, {195,-1,7,27.5f,1.2f}, {200,-1,10,27.5f,1.2f},
	{202,1,12,27.2f,1.2f}, {206,1,19,28.f,1.15f}, {209,-1,6,27.1f,1.f}, {225,-1,4,27.1f,1.f}, {230,1,11,27.5f,1.2f}, {233,-1,0,27.1f,1.f}, {236,-1,17,27.6f,1.f},
	{237,1,5,27.5f,1.2f}, {238,-1,10,27.1f,1.f}, {239,-1,24,26.9f,1.f}, {240,-1,12,26.8f,1.f}, {241,-1,9,27.1f,1.f}, {242,1,20,28.f,1.15f}, {243,1,1,27.5f,1.2f},
	{244,-1,6,27.1f,1.f}, {245,-1,8,27.1f,1.f}, {246,1,4,27.5f,1.2f}, {247,-1,22,28.7f,1.f}, {248,-1,11,27.1f,1.f}, {249,1,2,28.4f,1.2f}, {250,1,0,27.5f,1.2f},
	{251,-1,3,27.1f,1.f}, {252,-1,16,27.6f,1.f}, {253,-1,7,27.1f,1.f}, {254,-1,5,27.1f,1.f}, {255,1,10,27.5f,1.2f}, {256,1,23,31.3f,1.1f}, {257,1,12,27.2f,1.2f},
	{258,-1,9,27.1f,1.f}, {259,-1,18,27.6f,1.f}, {260,-1,1,27.1f,1.f}, {261,-1,6,27.1f,1.f}, {262,1,8,27.5f,1.2f}, {263,1,4,27.5f,1.2f}, {264,-1,21,27.6f,1.f},
	{265,-1,11,27.1f,1.f}, {266,1,2,28.4f,1.2f}, {267,-1,0,27.1f,1.f}, {268,-1,3,27.1f,1.f}, {269,1,15,28.f,1.15f}, {270,1,7,27.5f,1.2f}, {271,-1,5,27.1f,1.f},
	{272,-1,10,27.1f,1.f}, {273,-1,24,26.9f,1.f}, {274,-1,12,26.8f,1.f}, {275,1,9,27.5f,1.2f}, {276,1,19,28.f,1.15f}, {277,1,1,27.5f,1.2f}, {278,-1,6,27.1f,1.f},
	{279,-1,8,27.1f,1.f}, {280,-1,4,27.1f,1.f}, {281,-1,22,28.7f,1.f}, {282,1,11,27.5f,1.2f}, {283,1,2,28.4f,1.2f}, {284,-1,0,27.1f,1.f}, {285,-1,3,27.1f,1.f},
	{286,1,17,28.f,1.15f}, {287,-1,7,27.1f,1.f}, {288,-1,5,27.1f,1.f}, {289,1,10,27.5f,1.2f}, {290,1,23,31.3f,1.1f}, {291,-1,12,26.8f,1.f}, {292,-1,9,27.1f,1.f},
	{293,-1,20,27.6f,1.f}, {294,-1,1,27.1f,1.f}, {295,1,6,27.5f,1.2f}, {296,1,8,27.5f,1.2f}, {297,1,4,27.5f,1.2f}, {298,-1,21,27.6f,1.f}, {299,-1,11,27.1f,1.f},
	{300,-1,2,27.8f,1.f}, {301,-1,0,27.1f,1.f}, {302,1,3,27.5f,1.2f}, {303,1,16,28.f,1.15f}, {304,-1,7,27.1f,1.f}, {305,-1,5,27.1f,1.f}, {306,1,10,27.5f,1.2f},
	{307,-1,24,26.9f,1.f}, {308,-1,12,26.8f,1.f}, {309,1,9,27.5f,1.2f}, {310,1,18,28.f,1.15f}, {311,-1,1,27.1f,1.f}, {312,-1,6,27.1f,1.f}, {313,-1,8,27.1f,1.f},
	{314,-1,4,27.1f,1.f}, {315,-1,22,28.7f,1.f}, {316,1,11,27.5f,1.2f}, {317,1,2,28.4f,1.2f}, {318,-1,0,27.1f,1.f}, {319,-1,3,27.1f,1.f}, {320,-1,15,27.6f,1.f},
	{321,1,7,27.5f,1.2f}, {322,1,5,27.5f,1.2f}, {323,-1,10,27.5f,1.2f}, {324,1,23,31.3f,1.1f}, {325,-1,12,27.2f,1.2f}, {326,1,9,27.5f,1.2f}, {327,-1,19,28.f,1.15f},
	{328,1,1,27.5f,1.2f}, {329,1,6,27.5f,1.2f}, {330,1,8,27.5f,1.2f}, {331,-1,4,27.5f,1.2f}, {332,1,21,28.f,1.15f}, {333,-1,11,27.5f,1.2f}, {334,1,2,28.4f,1.2f},
	{335,-1,0,27.5f,1.2f}, {336,1,3,27.5f,1.2f}, {337,-1,17,28.f,1.15f}, {338,-1,7,27.5f,1.2f}, {339,-1,5,27.5f,1.2f}, {340,1,10,27.5f,1.2f}, {341,-1,24,27.2f,1.15f},
	{342,1,12,27.2f,1.2f}, {343,-1,9,27.5f,1.2f}, {344,1,20,28.f,1.15f}, {345,1,1,27.5f,1.2f}, {346,1,6,27.5f,1.2f}, {347,-1,8,27.5f,1.2f}, {348,1,4,27.5f,1.2f},
	{349,-1,22,29.3f,1.15f}, {350,1,11,27.5f,1.2f}, {351,-1,2,28.4f,1.2f}, {352,1,0,27.5f,1.2f}, {353,1,3,27.5f,1.2f}, {354,1,16,28.f,1.15f}, {355,-1,7,27.5f,1.2f},
	{356,1,5,27.5f,1.2f}, {357,-1,10,27.5f,1.2f}, {358,1,23,31.3f,1.1f}, {359,-1,12,27.2f,1.2f}, {360,1,9,27.5f,1.2f}, {361,-1,18,28.f,1.15f}, {362,-1,1,27.5f,1.2f},
	{363,-1,6,27.5f,1.2f}, {364,1,8,27.5f,1.2f}, {365,-1,4,27.5f,1.2f}, {366,1,21,28.f,1.15f}, {367,-1,11,27.5f,1.2f}, {368,1,2,28.4f,1.2f}, {369,1,0,27.5f,1.2f},
	{370,1,3,27.5f,1.2f}, {371,-1,15,28.f,1.15f}, {372,1,7,27.5f,1.2f}, {373,-1,5,27.5f,1.2f}, {374,1,10,27.5f,1.2f}, {375,-1,24,27.2f,1.15f}, {376,1,12,27.2f,1.2f},
	{377,1,9,27.5f,1.2f}, {378,1,19,28.f,1.15f}, {379,-1,1,27.5f,1.2f}, {380,1,6,27.5f,1.2f}, {381,-1,8,27.5f,1.2f}, {382,1,4,27.5f,1.2f}, {383,-1,22,29.3f,1.15f},
	{384,1,11,27.5f,1.2f}, {385,-1,2,28.4f,1.2f}, {386,-1,0,27.5f,1.2f}, {387,-1,3,27.5f,1.2f}, {388,1,17,28.f,1.15f}, {389,-1,7,27.5f,1.2f}, {390,1,5,27.5f,1.2f},
	{391,-1,10,27.5f,1.2f}, {392,1,23,31.3f,1.1f}, {393,-1,12,27.2f,1.2f}, {394,-1,9,27.5f,1.2f}, {395,-1,20,28.f,1.15f}, {396,1,1,27.5f,1.2f}, {397,-1,6,27.5f,1.2f},
	{398,1,8,27.5f,1.2f}, {399,-1,4,27.5f,1.2f}, {400,1,21,28.f,1.15f}, {401,1,11,27.5f,1.2f}, {402,1,2,28.4f,1.2f}, {403,-1,0,27.5f,1.2f}, {404,1,3,27.5f,1.2f},
	{405,-1,16,28.f,1.15f}, {406,1,7,27.5f,1.2f}, {407,-1,5,27.5f,1.2f}, {430,-1,14,41.5f,1.2f}, {430,1,13,41.f,1.2f}, {432,-1,13,41.f,1.2f}, {432,1,14,41.5f,1.2f},
	{434,-1,14,41.5f,1.2f}, {434,1,13,41.f,1.2f}, {436,-1,13,41.f,1.2f}, {436,1,14,41.5f,1.2f}, {438,-1,14,41.5f,1.2f}, {438,1,13,41.f,1.2f}, {460,-1,13,41.f,1.2f},
	{460,1,14,41.5f,1.2f}, {462,-1,14,41.5f,1.2f}, {462,1,13,41.f,1.2f}, {464,-1,13,41.f,1.2f}, {464,1,14,41.5f,1.2f}, {466,-1,14,41.5f,1.2f}, {466,1,13,41.f,1.2f},
	{468,-1,13,41.f,1.2f}, {468,1,14,41.5f,1.2f}, {488,-1,10,27.5f,1.2f}, {489,-1,24,27.2f,1.15f}, {490,1,12,26.8f,1.f}, {494,1,9,27.1f,1.f}, {496,-1,18,28.f,1.15f},
	{497,-1,1,27.5f,1.2f}, {498,1,6,27.1f,1.f}, {502,1,8,27.1f,1.f}, {504,-1,4,27.5f,1.2f}, {505,-1,22,29.3f,1.15f}, {506,1,11,27.1f,1.f}, {510,1,2,27.8f,1.f},
	{512,-1,0,27.5f,1.2f}, {513,-1,3,27.5f,1.2f}, {514,1,15,27.6f,1.f}, {518,1,7,27.1f,1.f}, {520,-1,5,27.5f,1.2f}, {521,-1,10,27.5f,1.2f}, {522,1,24,26.9f,1.f},
	{526,1,12,26.8f,1.f}, {528,-1,9,27.5f,1.2f}, {529,-1,19,28.f,1.15f}, {530,1,1,27.1f,1.f}, {534,1,6,27.1f,1.f}, {536,-1,8,27.5f,1.2f}, {537,-1,4,27.5f,1.2f},
	{538,1,21,27.6f,1.f}, {542,1,11,27.1f,1.f}, {544,-1,2,28.4f,1.2f}, {545,-1,0,27.5f,1.2f}, {546,1,3,27.1f,1.f}, {550,1,17,27.6f,1.f}, {552,-1,7,27.5f,1.2f},
	{553,-1,5,27.5f,1.2f}, {554,1,10,27.1f,1.f}, {558,1,24,26.9f,1.f}, {560,-1,12,27.2f,1.2f}, {561,-1,9,27.5f,1.2f}, {562,1,20,27.6f,1.f},
	};
	for (const FPplP3FRow& R : PplP3FRows)
	{
		Zone(TEXT("People p3f"), (R.Slot - 0.4f) * 0.045243f, (R.Slot + 0.4f) * 0.045243f);
		Site(PplP3FSets[R.Set], R.Side, R.X, 1, 0, 1.f, R.Scale);
	}
}
#undef P3F
#undef P3FK
// ---- END p3f ----
}

{
// ---- BEGIN s1d1 (district 1 "State Line and the Dry Wash", 0 to 2.43 km, ft_world_nevada_s1d1.py) ----
// 20 meshes (9 buildings, 10 props, the hero SM_S1D1_LineRider), made by the one make run (C:/ftwork/logs/L2s1d1_make.log).
// A border railhead and ranch town in whitewash and red, both sides, on even slots only (s % 4 == 0 or 2), nothing outside
// 0 to 2.43 km. Slot by slot, like the level's own population table: one zone a slot (slot s lies s * 45.243 m along the
// road), Site rules of every 1, chance 1, scale 1, so each yard stands exactly on its slot, on the side and at the distance
// chosen against what both tables already have there. Written by C:/ftwork/jobs/L2s1d1/table.py.
// Estimate: 74 sites, 251 piece-places counted part by part (249 + the hero's 2); by the brief's rule x 0.75 = about 187, + 2 = about 189 placed.
//   The most of one mesh: SM_S1D1_Corral, 33 places before the 0.75. The hero: 2 (slots 16 and 50, right, 110 m).
//   By mesh: Barn 6, Border 3, Corral 33, Depot 5, HayStack 28, HitchRail 30, Hotel 5, LampSign 23, LineRider 2, Livery 5, Marker 8, RimCabin 7, RimTank 7, School 5, Semaphore 8, Store 5, Trough 25, Wagon 20, WashLine 10, YardPump 16.
// Bands and slots used (x = metres from the centre line; a yard's pieces stand at the anchor + their own "out"):
//   state line, slots 4 to 24   yards: left 64 m (pieces 58.5 to 69.5: behind the pass 3 heights, 46 to 56 m), right 60 m on
//                               s % 4 == 0 (behind the watchers at 46 / 48 and the RV roofs at 44 to 50), right 51 m on s % 4 == 2
//                               (free from 46 m there; slot 18: 56 m, behind the far runners' ledge). LOW verge sets 22.5 m,
//                               both sides, between the ranch fence (18.9) and the first cast's fronts (24.6).
//   state line, slot 2          the border station on both sides, 40 m left (the phone line stands 30 m left) and 36 m right.
//   the wash's rock cuts        (slots 26, 28, 36, 38, 40, 42, 44) only the two rim pieces, 46 m, each on a 20 m column of the
//                               cut's own rock (the cut is 15 m tall, 25 to 35 m out); the pass 3 needles stand on the odd slots.
//   the wash, open              slots 30, 34: yards 50 m both sides (44.5 m and more: outside the 40 m a jump's run-up and
//                               landing keep clear); slots 48, 50, 52: left 66.5 m (behind the mesa ledges, 48 to 58), right 53 m.
//                               Slots 32 and 46 are left empty: the wash's bed crosses there (52 m, both sides).
//   the hero                    110 m right, slots 16 (0.72 km) and 50 (2.26 km); it never comes within 70 m (no shadow).
// No banked bend in this district. Every piece of the block but the hero stands nearer than 70 m (the furthest 69.5 m).
// TALL meshes: the smallest x min is 32.0 m (the border station's yard at slot 2, right). Glow (steady, never blinking)
// only on TALL meshes. LOW meshes (SM_S1D1_HitchRail, Trough, HayStack, Corral: tops 1.3 to 1.9 m, no glow, no movement):
// x min 22.5 m.
// FIX ROUND (after Stage 1's review): the yards of 8L and 10L stand 60 m out, not 64 (they reached 2.2 m into the cyclists' lane of slot 9).
#define S1D1(n) TEXT("/Game/World/Nevada/SM_S1D1_" n)
Set(TEXT("WldS1D1Border"), {      // the border inspection station: the state line marker, a lamp post, a hitching rail, a trough, a signal
	{ S1D1("Border"), 0.0f, 1.5f, 0.f, 0.0f, 1.f },
	{ S1D1("Marker"), -12.5f, -2.5f, 0.f, 0.0f, 1.f },
	{ S1D1("LampSign"), 10.5f, -4.0f, 0.f, 0.0f, 1.f },
	{ S1D1("HitchRail"), 13.5f, 1.0f, 0.f, 0.0f, 1.f },
	{ S1D1("Trough"), -12.5f, 2.5f, 0.f, 0.0f, 1.f },
	{ S1D1("Semaphore"), 15.5f, -3.5f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1Depot"), {      // the railhead depot: its signal mast, a wagon waiting, a lamp post, hay for the stock cars
	{ S1D1("Depot"), 0.0f, 3.5f, 0.f, 0.0f, 1.f },
	{ S1D1("Semaphore"), -12.5f, -2.5f, 0.f, 0.0f, 1.f },
	{ S1D1("Wagon"), 13.5f, -3.0f, 0.f, 0.0f, 1.f },
	{ S1D1("LampSign"), -11.0f, 2.5f, 0.f, 0.0f, 1.f },
	{ S1D1("HayStack"), 12.5f, 2.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1Store"), {      // the general store: a hitching rail before the porch, a wagon, a trough, a lamp post, washing behind
	{ S1D1("Store"), 0.0f, 2.5f, 0.f, 0.0f, 1.f },
	{ S1D1("HitchRail"), 0.5f, -5.5f, 0.f, 0.0f, 1.f },
	{ S1D1("Wagon"), 11.5f, -2.0f, 0.f, 0.0f, 1.f },
	{ S1D1("Trough"), -11.0f, -3.0f, 0.f, 0.0f, 1.f },
	{ S1D1("LampSign"), 7.5f, -5.5f, 0.f, 0.0f, 1.f },
	{ S1D1("WashLine"), -11.5f, 3.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1Barn"), {      // the ranch barn: the yard windpump, hay, a length of corral, a trough
	{ S1D1("Barn"), 0.0f, 5.5f, 0.f, 0.0f, 1.f },
	{ S1D1("YardPump"), -12.5f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D1("HayStack"), -6.0f, -4.5f, 0.f, 0.0f, 1.f },
	{ S1D1("Corral"), 9.5f, -4.5f, 0.f, 0.0f, 1.f },
	{ S1D1("Trough"), 13.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1School"), {      // the schoolhouse: a fence each side of its gate, a lamp post, a windpump, a hitching rail
	{ S1D1("School"), 0.0f, 2.5f, 0.f, 0.0f, 1.f },
	{ S1D1("Corral"), -9.5f, -4.5f, 0.f, 0.0f, 1.f },
	{ S1D1("Corral"), 9.5f, -4.5f, 0.f, 0.0f, 1.f },
	{ S1D1("LampSign"), 4.0f, -4.8f, 0.f, 0.0f, 1.f },
	{ S1D1("YardPump"), 12.0f, 3.0f, 0.f, 0.0f, 1.f },
	{ S1D1("HitchRail"), -11.5f, 1.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1Hotel"), {      // the line hotel: a lamp post, a hitching rail, a wagon, washing, the state line marker
	{ S1D1("Hotel"), 0.0f, 2.5f, 0.f, 0.0f, 1.f },
	{ S1D1("LampSign"), -8.5f, -4.0f, 0.f, 0.0f, 1.f },
	{ S1D1("HitchRail"), 1.0f, -5.5f, 0.f, 0.0f, 1.f },
	{ S1D1("Wagon"), -12.5f, -0.5f, 0.f, 0.0f, 1.f },
	{ S1D1("WashLine"), 11.5f, 3.0f, 0.f, 0.0f, 1.f },
	{ S1D1("Marker"), 11.5f, -3.5f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1Livery"), {      // the livery and smithy: a corral, hay, a trough before the door, a windpump, a wagon
	{ S1D1("Livery"), 0.0f, 2.0f, 0.f, 0.0f, 1.f },
	{ S1D1("Corral"), -11.5f, -3.0f, 0.f, 0.0f, 1.f },
	{ S1D1("HayStack"), 9.0f, -4.5f, 0.f, 0.0f, 1.f },
	{ S1D1("Trough"), -4.0f, -5.5f, 0.f, 0.0f, 1.f },
	{ S1D1("YardPump"), -13.0f, 3.5f, 0.f, 0.0f, 1.f },
	{ S1D1("Wagon"), 13.0f, 0.5f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1VergeA"), {      // LOW, by the ranch fence: a hitching rail and a hay stack
	{ S1D1("HitchRail"), -7.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D1("HayStack"), 6.0f, 0.3f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1VergeB"), {      // LOW, by the ranch fence: a length of corral
	{ S1D1("Corral"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1VergeC"), {      // LOW, by the ranch fence: a length of corral and a trough
	{ S1D1("Corral"), -4.5f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D1("Trough"), 5.5f, 0.3f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1RimCabin"), {      // the rim signal cabin on its 20 m column, over a rock cut
	{ S1D1("RimCabin"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1RimTank"), {      // the rim tank and its windpump on a 20 m column, over a rock cut
	{ S1D1("RimTank"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D1Rider"), {      // THE LINE RIDER, the hero: turned 45 degrees to the driver who comes at it (right side)
	{ S1D1("LineRider"), 0.0f, 0.0f, 0.f, -45.0f, 1.f },
});
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 2"), 0.072389f, 0.108583f);
	Site(TEXT("WldS1D1Border"), -1, 40.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1Border"), 1, 36.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeC"), -1, 22.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeB"), 1, 22.5f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 4"), 0.162875f, 0.199069f);
	Site(TEXT("WldS1D1Depot"), -1, 64.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1Livery"), 1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeA"), -1, 22.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeA"), 1, 22.5f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 6"), 0.253361f, 0.289555f);
	Site(TEXT("WldS1D1Barn"), -1, 64.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1School"), 1, 51.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeC"), -1, 22.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeB"), 1, 22.5f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 8"), 0.343847f, 0.380041f);
	Site(TEXT("WldS1D1Store"), -1, 60.0f, 1, 0, 1.f);   // fix round: 64 m reached 2.2 m into the cyclists' lane (slot 9, 76 m)
	Site(TEXT("WldS1D1Hotel"), 1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeA"), -1, 22.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeA"), 1, 22.5f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 10"), 0.434333f, 0.470527f);
	Site(TEXT("WldS1D1School"), -1, 60.0f, 1, 0, 1.f);   // fix round: as slot 8
	Site(TEXT("WldS1D1Depot"), 1, 51.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeC"), -1, 22.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeB"), 1, 22.5f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 12"), 0.524819f, 0.561013f);
	Site(TEXT("WldS1D1Livery"), -1, 64.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1Barn"), 1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeA"), -1, 22.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeA"), 1, 22.5f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 14"), 0.615305f, 0.651499f);
	Site(TEXT("WldS1D1Hotel"), -1, 64.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1Store"), 1, 51.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeC"), -1, 22.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeB"), 1, 22.5f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 16"), 0.705791f, 0.741985f);
	Site(TEXT("WldS1D1Depot"), -1, 64.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1Livery"), 1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1Rider"), 1, 110.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeA"), -1, 22.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeA"), 1, 22.5f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 18"), 0.796277f, 0.832471f);
	Site(TEXT("WldS1D1Barn"), -1, 64.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1School"), 1, 56.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeC"), -1, 22.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeB"), 1, 22.5f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 20"), 0.886763f, 0.922957f);
	Site(TEXT("WldS1D1Border"), -1, 64.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1Hotel"), 1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeA"), -1, 22.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeA"), 1, 22.5f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 22"), 0.977249f, 1.013443f);
	Site(TEXT("WldS1D1Store"), -1, 64.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1Depot"), 1, 51.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeC"), -1, 22.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeB"), 1, 22.5f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 24"), 1.067735f, 1.103929f);
	Site(TEXT("WldS1D1Livery"), -1, 64.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1Barn"), 1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeA"), -1, 22.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D1VergeA"), 1, 22.5f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 26"), 1.158221f, 1.194415f);
	Site(TEXT("WldS1D1RimCabin"), -1, 46.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1RimTank"), 1, 46.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 28"), 1.248707f, 1.284901f);
	Site(TEXT("WldS1D1RimTank"), -1, 46.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1RimCabin"), 1, 46.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 36"), 1.610651f, 1.646845f);
	Site(TEXT("WldS1D1RimCabin"), -1, 46.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1RimTank"), 1, 46.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 38"), 1.701137f, 1.737331f);
	Site(TEXT("WldS1D1RimTank"), -1, 46.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1RimCabin"), 1, 46.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 40"), 1.791623f, 1.827817f);
	Site(TEXT("WldS1D1RimCabin"), -1, 46.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1RimTank"), 1, 46.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 42"), 1.882109f, 1.918303f);
	Site(TEXT("WldS1D1RimTank"), -1, 46.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1RimCabin"), 1, 46.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 44"), 1.972595f, 2.008789f);
	Site(TEXT("WldS1D1RimCabin"), -1, 46.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1RimTank"), 1, 46.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 30"), 1.339193f, 1.375387f);
	Site(TEXT("WldS1D1Barn"), -1, 50.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1School"), 1, 50.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 34"), 1.520165f, 1.556359f);
	Site(TEXT("WldS1D1Livery"), -1, 50.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1Depot"), 1, 50.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 48"), 2.153567f, 2.189761f);
	Site(TEXT("WldS1D1Hotel"), -1, 66.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D1Store"), 1, 53.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 50"), 2.244053f, 2.280247f);
	Site(TEXT("WldS1D1School"), -1, 66.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D1Barn"), 1, 53.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D1Rider"), 1, 110.0f, 1, 0, 1.f);
Zone(TEXT("World s1d1, State Line and the Dry Wash, slot 52"), 2.334539f, 2.370733f);
	Site(TEXT("WldS1D1Store"), -1, 66.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D1Hotel"), 1, 53.0f, 1, 0, 1.f);
#undef S1D1
// ---- END s1d1 ----
}

{
// ---- BEGIN s1d2 (district 2 "Snake Eyes Country and Joshua Flats", ft_world_nevada_s1d2.py) ----
// 17 meshes: 7 buildings, 9 props (3 low, 6 tall), the hero. Every rule is a certain Site (chance 1, no jitter) in a Zone of ONE slot,
// so the count is the sum of the Sets' parts: 79 Sites in 27 slots = 220 placed if the track leaves none out (by the brief's share
// of 0.75: about 165). Most of one mesh: ScrapSign 26. The Tin Rattler: 2 (slots 78 and 106, left: 3.53 and 4.80 km).
// Slots: even ones only, 54 to 112 (2.443 to 5.067 km), without 70, 72 (the loop) and 102 (the jump).
//   compounds  two buildings 38 m of road wide and three yard props: anchor 66 m on the left, 64 m on the right (the buildings'
//              middles; their fronts 57.5 m and more; yard props 55 to 61 m). Snake Eyes country (54 to 84): on the side where the
//              level's own BikerDen (70 m) or Scrapyard (62 m) set is NOT. Joshua Flats (86 to 112): both sides on s % 4 == 0
//              (not 96 left: the mule train), one side in turn on s % 4 == 2.
//   tall props 39.5 m out, 11 m along from the slot's middle, only where the people table has no row there (computed from its rows).
//   low props  22.5 m out, every other slot, on the side away from the compound. No tall or low prop in 94 to 104 (the shallow
//              banked bends at 4.3 and 4.4 km, the jump at 4.6 km): there only the compounds, 55 m and more out.
// TALL pieces: the nearest stands 39.5 m out. LOW ones (HubcapFence, DrumPlanters, MailSnake): 22.5 m. Glow (steady, never
// blinking): lamp panes on the seven buildings (55 m and more), TyreTower, ScrapSign, InsulatorStack (39.5 m and more), the Rattler (112 m).
// FIX ROUND (after Stage 1's review): the Tin Rattler stands 100 m out, 1.3 times as built (was 130 and 132 m: not found from the road);
// the car stack's "Mid" set stands 41.5 m out, not 39.5 (three flattened hulks: nothing like a rack of vehicles within 40 m of the road).
#define S1D2(n) TEXT("/Game/World/Nevada/SM_S1D2_" n)
Set(TEXT("WldS1D2Gate"), {      // the first slot, left: the house of flattened cars between a sign and a whirligig
	{ S1D2("CarWallHouse"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("ScrapSign"), -10.5f, -6.f, 0.f, 60.f, 1.f },
	{ S1D2("Whirligig"), 10.6f, -6.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2CrushWall"), {      // CrusherShed and CarWallHouse, their yard in front
	{ S1D2("CrusherShed"), -11.8f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("CarWallHouse"), 12.9f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("CarStack"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("Whirligig"), 1.9f, -5.f, 0.f, 0.f, 1.f },
	{ S1D2("TyreTower"), 15.f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2QuonTyreR"), {      // QuonsetGarage and TyreWorkshop, their yard in front
	{ S1D2("QuonsetGarage"), -10.8f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("TyreWorkshop"), 13.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("TyreTower"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("ScrapSign"), 1.5f, -5.f, 0.f, -60.f, 1.f },
	{ S1D2("CarStack"), 15.f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2CrushQuonL"), {      // CrusherShed and QuonsetGarage, their yard in front
	{ S1D2("CrusherShed"), -11.8f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("QuonsetGarage"), 12.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("CarStack"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("TyreTower"), 0.1f, -5.f, 0.f, 0.f, 1.f },
	{ S1D2("ScrapSign"), 15.f, -9.f, 0.f, 60.f, 1.f },
});
Set(TEXT("WldS1D2TyreCrushL"), {      // TyreWorkshop and CrusherShed, their yard in front
	{ S1D2("TyreWorkshop"), -12.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("CrusherShed"), 10.8f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("Whirligig"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("ScrapSign"), -1.5f, -5.f, 0.f, 60.f, 1.f },
	{ S1D2("CarStack"), 15.f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2TankWallR"), {      // TankHouse and CarWallHouse, their yard in front
	{ S1D2("TankHouse"), -15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("CarWallHouse"), 12.9f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("WashLine"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("TyreTower"), -0.f, -5.f, 0.f, 0.f, 1.f },
	{ S1D2("ScrapSign"), 15.f, -9.f, 0.f, -60.f, 1.f },
});
Set(TEXT("WldS1D2BinTankR"), {      // GrainBinBar and TankHouse, their yard in front
	{ S1D2("GrainBinBar"), -12.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("TankHouse"), 11.6f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("ScrapSign"), -15.f, -9.f, 0.f, -60.f, 1.f },
	{ S1D2("WashLine"), 0.8f, -5.f, 0.f, 0.f, 1.f },
	{ S1D2("Whirligig"), 15.f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2WallBin"), {      // CarWallHouse and GrainBinBar, their yard in front
	{ S1D2("CarWallHouse"), -13.6f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("GrainBinBar"), 12.8f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("WashLine"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("CarStack"), -0.8f, -5.f, 0.f, 0.f, 1.f },
	{ S1D2("Whirligig"), 15.f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2QuonTyreL"), {      // QuonsetGarage and TyreWorkshop, their yard in front
	{ S1D2("QuonsetGarage"), -10.8f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("TyreWorkshop"), 13.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("TyreTower"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("ScrapSign"), 1.5f, -5.f, 0.f, 60.f, 1.f },
	{ S1D2("CarStack"), 15.f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2TyreCrushR"), {      // TyreWorkshop and CrusherShed, their yard in front
	{ S1D2("TyreWorkshop"), -12.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("CrusherShed"), 10.8f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("Whirligig"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("ScrapSign"), -1.5f, -5.f, 0.f, -60.f, 1.f },
	{ S1D2("CarStack"), 15.f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2TankWallL"), {      // TankHouse and CarWallHouse, their yard in front
	{ S1D2("TankHouse"), -15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("CarWallHouse"), 12.9f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("WashLine"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("TyreTower"), -0.f, -5.f, 0.f, 0.f, 1.f },
	{ S1D2("ScrapSign"), 15.f, -9.f, 0.f, 60.f, 1.f },
});
Set(TEXT("WldS1D2LineTank"), {      // LinemanHouse and TankHouse, their yard in front
	{ S1D2("LinemanHouse"), -12.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("TankHouse"), 11.6f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("InsulatorStack"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("WashLine"), 0.9f, -5.f, 0.f, 0.f, 1.f },
	{ S1D2("Whirligig"), 15.f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2BinQuonL"), {      // GrainBinBar and QuonsetGarage, their yard in front
	{ S1D2("GrainBinBar"), -12.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("QuonsetGarage"), 12.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("ScrapSign"), -15.f, -9.f, 0.f, 60.f, 1.f },
	{ S1D2("TyreTower"), -1.1f, -5.f, 0.f, 0.f, 1.f },
	{ S1D2("CarStack"), 15.f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2TyreLine"), {      // TyreWorkshop and LinemanHouse, their yard in front
	{ S1D2("TyreWorkshop"), -12.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("LinemanHouse"), 12.8f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("TyreTower"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("Whirligig"), -0.4f, -5.f, 0.f, 0.f, 1.f },
	{ S1D2("InsulatorStack"), 15.f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2TankBinR"), {      // TankHouse and GrainBinBar, their yard in front
	{ S1D2("TankHouse"), -15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("GrainBinBar"), 12.8f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("WashLine"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("ScrapSign"), -0.8f, -5.f, 0.f, -60.f, 1.f },
	{ S1D2("Whirligig"), 15.f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2LineWall"), {      // LinemanHouse and CarWallHouse, their yard in front
	{ S1D2("LinemanHouse"), -12.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("CarWallHouse"), 12.9f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("InsulatorStack"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("CarStack"), 0.9f, -5.f, 0.f, 0.f, 1.f },
	{ S1D2("WashLine"), 15.f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2QuonTankR"), {      // QuonsetGarage and TankHouse, their yard in front
	{ S1D2("QuonsetGarage"), -10.8f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("TankHouse"), 11.6f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("CarStack"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("ScrapSign"), 1.9f, -5.f, 0.f, -60.f, 1.f },
	{ S1D2("WashLine"), 15.f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2LineBinL"), {      // LinemanHouse and GrainBinBar, their yard in front
	{ S1D2("LinemanHouse"), -12.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("GrainBinBar"), 12.8f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("WashLine"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("TyreTower"), 0.1f, -5.f, 0.f, 0.f, 1.f },
	{ S1D2("ScrapSign"), 15.f, -9.f, 0.f, 60.f, 1.f },
});
Set(TEXT("WldS1D2TyreQuon"), {      // TyreWorkshop and QuonsetGarage, their yard in front
	{ S1D2("TyreWorkshop"), -12.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("QuonsetGarage"), 12.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("Whirligig"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("CarStack"), -1.5f, -5.f, 0.f, 0.f, 1.f },
	{ S1D2("TyreTower"), 15.f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2WallLineR"), {      // CarWallHouse and LinemanHouse, their yard in front
	{ S1D2("CarWallHouse"), -13.6f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("LinemanHouse"), 12.8f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("ScrapSign"), -15.f, -9.f, 0.f, -60.f, 1.f },
	{ S1D2("Whirligig"), -0.9f, -5.f, 0.f, 0.f, 1.f },
	{ S1D2("WashLine"), 15.f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2TankCrush"), {      // TankHouse and CrusherShed, their yard in front
	{ S1D2("TankHouse"), -15.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("CrusherShed"), 10.8f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("TyreTower"), -15.f, -9.f, 0.f, 0.f, 1.f },
	{ S1D2("WashLine"), -2.f, -5.f, 0.f, 0.f, 1.f },
	{ S1D2("CarStack"), 15.f, -9.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2Rattler"), {      // THE TIN RATTLER (fix round: 1.3 times as built and 100 m out, was 130 and 132 m: the driver could not find it)
	{ S1D2("TinRattler"), 0.f, 0.f, 0.f, 0.f, 1.3f },
});
Set(TEXT("WldS1D2MidTowerA"), {      // 39.5 m out, -11 m along
	{ S1D2("TyreTower"), -11.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2MidStackA"), {      // 39.5 m out, -11 m along
	{ S1D2("CarStack"), -11.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2MidWashB"), {      // 39.5 m out, 11 m along
	{ S1D2("WashLine"), 11.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2MidSignAL"), {      // 39.5 m out, -11 m along
	{ S1D2("ScrapSign"), -11.f, 0.f, 0.f, 60.f, 1.f },
});
Set(TEXT("WldS1D2MidWhirlB"), {      // 39.5 m out, 11 m along
	{ S1D2("Whirligig"), 11.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2MidTowerB"), {      // 39.5 m out, 11 m along
	{ S1D2("TyreTower"), 11.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2MidWashA"), {      // 39.5 m out, -11 m along
	{ S1D2("WashLine"), -11.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2MidSignAR"), {      // 39.5 m out, -11 m along
	{ S1D2("ScrapSign"), -11.f, 0.f, 0.f, -60.f, 1.f },
});
Set(TEXT("WldS1D2MidWhirlA"), {      // 39.5 m out, -11 m along
	{ S1D2("Whirligig"), -11.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2MidSignBR"), {      // 39.5 m out, 11 m along
	{ S1D2("ScrapSign"), 11.f, 0.f, 0.f, -60.f, 1.f },
});
Set(TEXT("WldS1D2MidInsulA"), {      // 39.5 m out, -11 m along
	{ S1D2("InsulatorStack"), -11.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2LowA"), {      // low, 22.5 m out
	{ S1D2("HubcapFence"), -11.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("DrumPlanters"), 9.f, 0.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2LowB"), {      // low, 22.5 m out
	{ S1D2("MailSnake"), -10.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D2("HubcapFence"), 10.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D2LowC"), {      // low, 22.5 m out
	{ S1D2("DrumPlanters"), -10.f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D2("MailSnake"), 9.f, 0.f, 0.f, 0.f, 1.f },
});
{
	static const TCHAR* const S1D2Sets[] = { TEXT("WldS1D2Gate"), TEXT("WldS1D2CrushWall"), TEXT("WldS1D2QuonTyreR"), TEXT("WldS1D2CrushQuonL"), TEXT("WldS1D2TyreCrushL"), TEXT("WldS1D2TankWallR"), TEXT("WldS1D2BinTankR"), TEXT("WldS1D2WallBin"), TEXT("WldS1D2QuonTyreL"), TEXT("WldS1D2TyreCrushR"), TEXT("WldS1D2TankWallL"), TEXT("WldS1D2LineTank"), TEXT("WldS1D2BinQuonL"), TEXT("WldS1D2TyreLine"), TEXT("WldS1D2TankBinR"), TEXT("WldS1D2LineWall"), TEXT("WldS1D2QuonTankR"), TEXT("WldS1D2LineBinL"), TEXT("WldS1D2TyreQuon"), TEXT("WldS1D2WallLineR"), TEXT("WldS1D2TankCrush"), TEXT("WldS1D2Rattler"), TEXT("WldS1D2MidTowerA"), TEXT("WldS1D2MidStackA"), TEXT("WldS1D2MidWashB"), TEXT("WldS1D2MidSignAL"), TEXT("WldS1D2MidWhirlB"), TEXT("WldS1D2MidTowerB"), TEXT("WldS1D2MidWashA"), TEXT("WldS1D2MidSignAR"), TEXT("WldS1D2MidWhirlA"), TEXT("WldS1D2MidSignBR"), TEXT("WldS1D2MidInsulA"), TEXT("WldS1D2LowA"), TEXT("WldS1D2LowB"), TEXT("WldS1D2LowC") };
	struct FS1D2Row { int16 Slot; int8 Side; uint8 Set; float X; };
	// slot, side (-1 left of travel, +1 right), set, metres from the centre line
	static const FS1D2Row S1D2Rows[] = {
	{54,-1,22,39.5f}, {54,-1,0,66.f}, {54,1,22,39.5f}, {56,-1,33,22.5f}, {56,-1,23,41.5f}, {56,1,24,39.5f}, {56,1,1,64.f}, {58,-1,25,39.5f},
	{58,1,26,39.5f}, {58,1,2,64.f}, {60,-1,27,39.5f}, {60,-1,3,66.f}, {60,1,34,22.5f}, {60,1,23,41.5f}, {62,-1,28,39.5f}, {62,-1,4,66.f},
	{62,1,29,39.5f}, {64,-1,35,22.5f}, {64,-1,30,39.5f}, {64,1,27,39.5f}, {64,1,5,64.f}, {66,-1,23,41.5f}, {66,1,24,39.5f}, {66,1,6,64.f},
	{68,-1,25,39.5f}, {68,-1,1,66.f}, {68,1,33,22.5f}, {68,1,30,39.5f}, {74,-1,22,39.5f}, {74,1,23,41.5f}, {74,1,7,64.f}, {76,-1,24,39.5f},
	{76,-1,8,66.f}, {76,1,34,22.5f}, {76,1,31,39.5f}, {78,-1,30,39.5f}, {78,-1,3,66.f}, {78,-1,21,100.f}, {78,1,22,39.5f}, {80,-1,35,22.5f},
	{80,-1,23,41.5f}, {80,1,24,39.5f}, {80,1,9,64.f}, {82,-1,25,39.5f}, {82,1,26,39.5f}, {82,1,6,64.f}, {84,-1,27,39.5f}, {84,-1,10,66.f},
	{84,1,33,22.5f}, {84,1,23,41.5f}, {86,-1,28,39.5f}, {86,-1,11,66.f}, {88,-1,12,66.f}, {88,1,34,22.5f}, {88,1,13,64.f}, {90,1,32,39.5f},
	{90,1,14,64.f}, {92,-1,35,22.5f}, {92,-1,15,66.f}, {92,1,16,64.f}, {94,-1,17,66.f}, {96,1,18,64.f}, {98,1,19,64.f}, {100,-1,20,66.f},
	{100,1,11,64.f}, {104,-1,12,66.f}, {104,1,13,64.f}, {106,-1,30,39.5f}, {106,-1,21,100.f}, {106,1,14,64.f}, {108,-1,15,66.f}, {108,1,33,22.5f},
	{108,1,22,39.5f}, {108,1,16,64.f}, {110,-1,17,66.f}, {112,-1,34,22.5f}, {112,-1,23,41.5f}, {112,-1,18,66.f}, {112,1,19,64.f},
	};
	for (const FS1D2Row& R : S1D2Rows)
	{
		Zone(TEXT("World s1d2, Snake Eyes Country and Joshua Flats"), FMath::Max(2.43f, (R.Slot - 0.4f) * 0.045243f), (R.Slot + 0.4f) * 0.045243f);
		Site(S1D2Sets[R.Set], R.Side, R.X, 1, 0, 1.f);
	}
}
#undef S1D2
// ---- END s1d2 ----
}

{
// ---- BEGIN s1d3 (district 3 "Red Rock Pass and Last Chance Gas", 5.14 to 7.75 km, ft_world_nevada_s1d3.py) ----
// 19 meshes, painted adobe: 8 buildings, 10 props (3 low), 1 hero (THE BIG PUMP, once: right side, 95 m out, slot 152 = 6.88 km).
// THE PASS (5.14-6.31 km): only the two rim pieces (adobe on a 17 m rock column, 25.9 and 26.5 m tall), by Scatter on
//   s % 4 == 2 (CliffHouse) and s % 4 == 0 (CliffTower), on the cut side only: right 48-56 m through the right-hand
//   wall bend and its cut (slots 114-120), left 52-60 m through the left-hand wall bend (126-132), both sides 52-60 m
//   along the east cut (134-138). Nothing on the cliff side, nothing beside the corkscrew (122, 124).
// LAST CHANCE GAS (6.31-7.75 km): one Site a side on every even slot 140-170, each slot a zone of its own (the
//   people table does the same: slot s runs (s - 0.4) to (s + 0.4) x 0.045243 km), so a set stands exactly on its slot.
//   The street: buildings 41.8 m out (40.4 m where the watch bleachers stand 46 m out: L 140 148 156 164, R 144 152
//   160 168), behind the near people rows (27-33 m), the strip walkers (36 m) and the neon signs (35 m), in front of
//   the motel courts and road stops (45 m and beyond) and of the pass 3 heights (46-56 m). Tall props stand beside
//   the building: 39.6-39.8 m out on its +along side, 42.0-43.0 m on its -along side (the half-slot scenes of p3b
//   reach 41 m there). Low pieces (top 1.8 m) stand 36.8-38.7 m out, in front of the building.
// Estimate: the pass, Scatter: 14 slot-sides x 0.75 = 10.5; Last Chance Gas, Site: 32 sets holding 216 pieces x 0.75 = 162.0;
//   the hero 1 x 0.75 = 0.8: about 173 placed (231 written, before the track's share is taken off). Most of one mesh: GardenWall, 29.
// Smallest x of a TALL mesh: 39.6 m. Smallest x of any mesh (a low one): 36.8 m. Glow: steady only. Slots: even only.
// FIX ROUND (after Stage 1's review): 146R, 156L and 166R are left open (the first cast's far runners' ledge, 38 to 46 m out, ran 1.7 to
// 2 m into the building), slot 170 on both sides and 152L too (the level's own sign frame stands in the footprint; 152L's rule is kept
// with chance -1 so that no later rule's random stream moves). THE BIG PUMP is 1.5 times as built: it did not read behind the kit's sign.
#define S1D3(n) TEXT("/Game/World/Nevada/SM_S1D3_" n)
Set(TEXT("WldS1D3Trading"), {
	{ S1D3("TradingPost"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("WaterJar"), -9.4f, 1.f, 0.f, 0.f, 1.f },
	{ S1D3("DryingRack"), -12.8f, 1.2f, 0.f, 0.f, 1.f },
	{ S1D3("PotStall"), 10.3f, -2.f, 0.f, 0.f, 1.f },
	{ S1D3("LampPost"), 13.9f, -2.2f, 0.f, 0.f, 1.f },
	{ S1D3("GardenWall"), -3.4f, -4.6f, 0.f, 0.f, 1.f },
	{ S1D3("ClayOven"), 5.4f, -4.6f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3Cantina"), {
	{ S1D3("Cantina"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("BrushShade"), -8.4f, 1.f, 0.f, 0.f, 1.f },
	{ S1D3("BlanketLine"), -13.7f, 1.2f, 0.f, 0.f, 1.f },
	{ S1D3("AirTower"), 7.8f, -2.f, 0.f, 0.f, 1.f },
	{ S1D3("PotStall"), 11.2f, -2.f, 0.f, 0.f, 1.f },
	{ S1D3("RadiatorStand"), 3.4f, -4.4f, 0.f, 0.f, 1.f },
	{ S1D3("GardenWall"), -2.7f, -5.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3Gate"), {
	{ S1D3("MotorCourt"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("LampPost"), -10.4f, 1.f, 0.f, 0.f, 1.f },
	{ S1D3("WaterJar"), -13.f, 1.f, 0.f, 0.f, 1.f },
	{ S1D3("DryingRack"), 11.f, -2.f, 0.f, 0.f, 1.f },
	{ S1D3("AirTower"), 14.2f, -2.f, 0.f, 0.f, 1.f },
	{ S1D3("GardenWall"), -7.1f, -3.1f, 0.f, 0.f, 1.f },
	{ S1D3("GardenWall"), 7.1f, -3.1f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3Station"), {
	{ S1D3("FillingStation"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("AirTower"), -8.6f, 1.f, 0.f, 0.f, 1.f },
	{ S1D3("BrushShade"), -12.4f, 1.f, 0.f, 0.f, 1.f },
	{ S1D3("WaterJar"), 8.9f, -2.f, 0.f, 0.f, 1.f },
	{ S1D3("BlanketLine"), 13.3f, -2.f, 0.f, 0.f, 1.f },
	{ S1D3("RadiatorStand"), -3.f, -4.6f, 0.f, 0.f, 1.f },
	{ S1D3("GardenWall"), 4.4f, -3.6f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3Repair"), {
	{ S1D3("RepairShop"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("PotStall"), -8.3f, 1.f, 0.f, 0.f, 1.f },
	{ S1D3("LampPost"), -12.6f, 1.f, 0.f, 0.f, 1.f },
	{ S1D3("BrushShade"), 9.f, -2.f, 0.f, 0.f, 1.f },
	{ S1D3("WaterJar"), 12.9f, -2.f, 0.f, 0.f, 1.f },
	{ S1D3("RadiatorStand"), 2.6f, -4.4f, 0.f, 0.f, 1.f },
	{ S1D3("ClayOven"), 7.4f, -4.6f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3PotsA"), {
	{ S1D3("PotShop"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("DryingRack"), -5.6f, 1.6f, 0.f, 0.f, 1.f },
	{ S1D3("LampPost"), -9.2f, 1.6f, 0.f, 0.f, 1.f },
	{ S1D3("PotStall"), 5.6f, -0.8f, 0.f, 0.f, 1.f },
	{ S1D3("WaterJar"), 9.2f, -0.6f, 0.f, 0.f, 1.f },
	{ S1D3("GardenWall"), 0.f, -3.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3PotsB"), {
	{ S1D3("PotShop"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("AirTower"), -4.8f, 1.6f, 0.f, 0.f, 1.f },
	{ S1D3("BlanketLine"), -9.6f, 1.9f, 0.f, 0.f, 1.f },
	{ S1D3("BrushShade"), 5.9f, -0.6f, 0.f, 0.f, 1.f },
	{ S1D3("LampPost"), 9.8f, -0.8f, 0.f, 0.f, 1.f },
	{ S1D3("RadiatorStand"), 0.4f, -3.6f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3BigPump"), {
	{ S1D3("BigPump"), 0.f, 0.f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1d3, Red Rock Pass: the right-hand wall bend, inside"), 5.14f, 5.45f);
	Scatter(S1D3("CliffHouse"), 48.f, 56.f, 4, 2, 1.f, 1.f, 0.05f, 1, 10.f);
	Scatter(S1D3("CliffTower"), 48.f, 56.f, 4, 0, 1.f, 1.f, 0.05f, 1, 10.f);
Zone(TEXT("World s1d3, Red Rock Pass: the left-hand wall bend, inside"), 5.68f, 6.04f);
	Scatter(S1D3("CliffHouse"), 52.f, 60.f, 4, 2, 1.f, 1.f, 0.05f, -1, 10.f);
	Scatter(S1D3("CliffTower"), 52.f, 60.f, 4, 0, 1.f, 1.f, 0.05f, -1, 10.f);
Zone(TEXT("World s1d3, Red Rock Pass: the east cut"), 6.04f, 6.31f);
	Scatter(S1D3("CliffHouse"), 52.f, 60.f, 4, 2, 1.f, 1.f, 0.05f, 0, 10.f);
	Scatter(S1D3("CliffTower"), 52.f, 60.f, 4, 0, 1.f, 1.f, 0.05f, 0, 10.f);
Zone(TEXT("World s1d3, Last Chance Gas 140"), 6.3159f, 6.3521f);
	Site(TEXT("WldS1D3PotsA"), -1, 40.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D3Station"), 1, 41.8f, 1, 0, 1.f);
Zone(TEXT("World s1d3, Last Chance Gas 142"), 6.4064f, 6.4426f);
	Site(TEXT("WldS1D3Trading"), -1, 41.8f, 1, 0, 1.f);
	Site(TEXT("WldS1D3Repair"), 1, 41.8f, 1, 0, 1.f);
Zone(TEXT("World s1d3, Last Chance Gas 144"), 6.4969f, 6.5331f);
	Site(TEXT("WldS1D3Gate"), -1, 41.8f, 1, 0, 1.f);
	Site(TEXT("WldS1D3PotsB"), 1, 40.4f, 1, 0, 1.f);
Zone(TEXT("World s1d3, Last Chance Gas 146"), 6.5874f, 6.6236f);
	Site(TEXT("WldS1D3Cantina"), -1, 41.8f, 1, 0, 1.f);
	// (fix round) right side left open: the far runners' ledge of the first cast stands 38 to 46 m out here
Zone(TEXT("World s1d3, Last Chance Gas 148"), 6.6779f, 6.7141f);
	Site(TEXT("WldS1D3PotsB"), -1, 40.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D3Cantina"), 1, 41.8f, 1, 0, 1.f);
Zone(TEXT("World s1d3, Last Chance Gas 150"), 6.7684f, 6.8045f);
	Site(TEXT("WldS1D3Station"), -1, 41.8f, 1, 0, 1.f);
	Site(TEXT("WldS1D3Gate"), 1, 41.8f, 1, 0, 1.f);
Zone(TEXT("World s1d3, Last Chance Gas 152"), 6.8588f, 6.895f);
	Site(TEXT("WldS1D3Repair"), -1, 41.8f, 1, 0, -1.f);   // fix round: chance -1 = never placed (the rule keeps its place in the table): the level's own sign frame stands 41 m out in this footprint
	Site(TEXT("WldS1D3PotsA"), 1, 40.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D3BigPump"), 1, 95.f, 1, 0, 1.f, 1.5f);   // fix round: 1.5 times as built (the driver: the kit's sign towered over it)
Zone(TEXT("World s1d3, Last Chance Gas 154"), 6.9493f, 6.9855f);
	Site(TEXT("WldS1D3Trading"), -1, 41.8f, 1, 0, 1.f);
	Site(TEXT("WldS1D3Station"), 1, 41.8f, 1, 0, 1.f);
Zone(TEXT("World s1d3, Last Chance Gas 156"), 7.0398f, 7.076f);
	// (fix round) left side left open: the far runners' ledge stands here
	Site(TEXT("WldS1D3Repair"), 1, 41.8f, 1, 0, 1.f);
Zone(TEXT("World s1d3, Last Chance Gas 158"), 7.1303f, 7.1665f);
	Site(TEXT("WldS1D3Cantina"), -1, 41.8f, 1, 0, 1.f);
	Site(TEXT("WldS1D3Trading"), 1, 41.8f, 1, 0, 1.f);
Zone(TEXT("World s1d3, Last Chance Gas 160"), 7.2208f, 7.257f);
	Site(TEXT("WldS1D3Station"), -1, 41.8f, 1, 0, 1.f);
	Site(TEXT("WldS1D3PotsB"), 1, 40.4f, 1, 0, 1.f);
Zone(TEXT("World s1d3, Last Chance Gas 162"), 7.3113f, 7.3475f);
	Site(TEXT("WldS1D3Repair"), -1, 41.8f, 1, 0, 1.f);
	Site(TEXT("WldS1D3Gate"), 1, 41.8f, 1, 0, 1.f);
Zone(TEXT("World s1d3, Last Chance Gas 164"), 7.4018f, 7.4379f);
	Site(TEXT("WldS1D3PotsB"), -1, 40.4f, 1, 0, 1.f);
	Site(TEXT("WldS1D3Cantina"), 1, 41.8f, 1, 0, 1.f);
Zone(TEXT("World s1d3, Last Chance Gas 166"), 7.4922f, 7.5284f);
	Site(TEXT("WldS1D3Trading"), -1, 41.8f, 1, 0, 1.f);
	// (fix round) right side left open: the far runners' ledge stands here
Zone(TEXT("World s1d3, Last Chance Gas 168"), 7.5827f, 7.6189f);
	Site(TEXT("WldS1D3Gate"), -1, 41.8f, 1, 0, 1.f);
	Site(TEXT("WldS1D3PotsA"), 1, 40.4f, 1, 0, 1.f);
// (fix round) slot 170 is left open on both sides: the level's own sign frame stands 40.4 m out on this slot
#undef S1D3
// ---- END s1d3 ----
}

{
// ---- BEGIN s1d4 (district 4 "Jackpot Junction and the City Lights", ft_world_nevada_s1d4.py) ----
// 16 meshes: 7 buildings, 8 props (6 tall, 2 low), 1 hero (SM_S1D4_LuckyHorseshoe). 7.75 to 10.69 km: slots 172 to 236, the EVEN slots
// only (s % 4 == 0 or 2). Written by C:/ftwork/jobs/L2s1d4/gen_block.py: one Zone a slot, every piece a Site rule with chance 1, so it
// stands exactly on its slot, side and distance (as the rows of this table do): this stretch is full and only named places are free.
// Estimate: 279 Site parts written. The brief's arithmetic (x 0.75 for what the track leaves out) = about 209 placed. By the slots
// themselves: 43 of the parts stand on slots 180-184, 188, 190, 220, 222 (the loops and the corkscrew, where the track leaves pieces
// out; which slots exactly was not checked) = 236 placed if all of those go. Most of any one mesh: StarLamp 40. Hero: 2.
// Where, and why there (read from FTDressL2.inl, FTPeopleL2.inl and the make logs' sizes; NOT seen in the game):
//   67 m out (60.8 to 73.2 m): THE BLOCKS. A building, a star lamp and an atom pylon or tube cactus on one flank, an arrow sign and fin
//     pylons or a phone kiosk on the other (along -16.5 to +18 m). Only on the slot-sides where that band holds no set of the first
//     pass: not where Jackpot (74 / 80 m) or RoadStop (66 m) stands or reaches in from the odd slot beside, not on the chapels' slots
//     (52-90 m: Jackpot Junction s % 4 == 2, City Lights s % 5 == 2), the motel courts' (left, s % 10 == 4), the fountain's (204) or
//     the turning hoardings'. Left: 172, 176, 196, 200, 206, 208, 216, 218, 226, 228, 236. Right: 172, 192, 200, 210, 214, 224, 230, 234.
//     Not dodged (a Scatter rule, its place is random): SM_Trailer 60-120 m on slots 172 and 196 (chance 0.7).
//   29.5 m out (26.55 to 32.45 m): SodaBar, left of slots 180, 186, 192, 198, where the near row is empty (no phone line on that side).
//   35.5, 38 or 42 m out: one tall slim prop at each END of a slot (along -15.5 and +15.5 m) where no deck or stand of the population
//     reaches (1.2 m clear of every row's footprint, the neighbours' included). The old hoardings (SM_SignFrame, 32-48 m) slide 11.3 m
//     either way about a slot's middle and are 1.5 m thick: the ends are clear of them. Slots 172-178 (banked bends): 42 m only.
//   22 m out (20.9 to 22.9 m): one LOW prop a slot-side from slot 180, between the kerb / ranch fence (to 20.2 m) and the decks (24.6 m).
//   100 m out: THE LUCKY HORSESHOE, slot 194 left (8.78 km) and slot 232 right (10.50 km); plinth 93.2 to 105 m (chapels end at 93.8 m).
// Every TALL mesh stands 24 m or more out (the nearest: SodaBar, anchor 29.5 m, front edge 26.55 m). Light: steady only, and only on meshes
// whose every rule is 26.55 m or more out; KidneyPlanter and RopePodium (22 m) carry none. Only the hero stands outside 70 m.
#define S1D4(n) TEXT("/Game/World/Nevada/SM_S1D4_" n)
Set(TEXT("WldS1D4BlkCoffeeShopA"), {                               // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D4("CoffeeShop"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("StarLamp"), -11.5f, -4.f, 0.f, 0.f, 1.f },
	{ S1D4("AtomPylon"), -16.5f, 1.f, 0.f, 0.f, 1.f },
	{ S1D4("ArrowSign"), 12.5f, -3.5f, 0.f, 0.f, 1.f },
	{ S1D4("FinPylons"), 18.f, 1.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4BlkDrumLoungeB"), {                               // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D4("DrumLounge"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("StarLamp"), -11.5f, -4.f, 0.f, 0.f, 1.f },
	{ S1D4("TubeCactus"), -16.5f, 1.f, 0.f, 0.f, 1.f },
	{ S1D4("ArrowSign"), 12.5f, -3.5f, 0.f, 0.f, 1.f },
	{ S1D4("PhoneKiosk"), 18.f, 1.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4BlkMotelOfficeA"), {                               // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D4("MotelOffice"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("StarLamp"), -11.5f, -4.f, 0.f, 0.f, 1.f },
	{ S1D4("AtomPylon"), -16.5f, 1.f, 0.f, 0.f, 1.f },
	{ S1D4("ArrowSign"), 12.5f, -3.5f, 0.f, 0.f, 1.f },
	{ S1D4("FinPylons"), 18.f, 1.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4BlkBowlingAlleyB"), {                               // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D4("BowlingAlley"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("StarLamp"), -11.5f, -4.f, 0.f, 0.f, 1.f },
	{ S1D4("TubeCactus"), -16.5f, 1.f, 0.f, 0.f, 1.f },
	{ S1D4("ArrowSign"), 12.5f, -3.5f, 0.f, 0.f, 1.f },
	{ S1D4("PhoneKiosk"), 18.f, 1.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4BlkArcadeHallA"), {                               // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D4("ArcadeHall"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("StarLamp"), -11.5f, -4.f, 0.f, 0.f, 1.f },
	{ S1D4("AtomPylon"), -16.5f, 1.f, 0.f, 0.f, 1.f },
	{ S1D4("ArrowSign"), 12.5f, -3.5f, 0.f, 0.f, 1.f },
	{ S1D4("FinPylons"), 18.f, 1.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4BlkDriveInB"), {                               // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D4("DriveIn"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("StarLamp"), -11.5f, -4.f, 0.f, 0.f, 1.f },
	{ S1D4("TubeCactus"), -16.5f, 1.f, 0.f, 0.f, 1.f },
	{ S1D4("ArrowSign"), 12.5f, -3.5f, 0.f, 0.f, 1.f },
	{ S1D4("PhoneKiosk"), 18.f, 1.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4BlkMotelOfficeB"), {                               // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D4("MotelOffice"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("StarLamp"), -11.5f, -4.f, 0.f, 0.f, 1.f },
	{ S1D4("TubeCactus"), -16.5f, 1.f, 0.f, 0.f, 1.f },
	{ S1D4("ArrowSign"), 12.5f, -3.5f, 0.f, 0.f, 1.f },
	{ S1D4("PhoneKiosk"), 18.f, 1.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4BlkDrumLoungeA"), {                               // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D4("DrumLounge"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("StarLamp"), -11.5f, -4.f, 0.f, 0.f, 1.f },
	{ S1D4("AtomPylon"), -16.5f, 1.f, 0.f, 0.f, 1.f },
	{ S1D4("ArrowSign"), 12.5f, -3.5f, 0.f, 0.f, 1.f },
	{ S1D4("FinPylons"), 18.f, 1.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4BlkDriveInA"), {                               // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D4("DriveIn"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("StarLamp"), -11.5f, -4.f, 0.f, 0.f, 1.f },
	{ S1D4("AtomPylon"), -16.5f, 1.f, 0.f, 0.f, 1.f },
	{ S1D4("ArrowSign"), 12.5f, -3.5f, 0.f, 0.f, 1.f },
	{ S1D4("FinPylons"), 18.f, 1.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Horseshoe"), { { S1D4("LuckyHorseshoe"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D4SodaBarP0"), { { S1D4("SodaBar"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D4FinPylonsP155"), { { S1D4("FinPylons"), 15.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D4StarLampM155"), { { S1D4("StarLamp"), -15.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D4StarLampP155"), { { S1D4("StarLamp"), 15.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D4ArrowSignM155"), { { S1D4("ArrowSign"), -15.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D4TubeCactusP155"), { { S1D4("TubeCactus"), 15.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D4ArrowSignP155"), { { S1D4("ArrowSign"), 15.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D4TubeCactusM155"), { { S1D4("TubeCactus"), -15.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D4FinPylonsM155"), { { S1D4("FinPylons"), -15.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D4PhoneKioskM155"), { { S1D4("PhoneKiosk"), -15.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D4PhoneKioskP155"), { { S1D4("PhoneKiosk"), 15.5f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D4KidneyPlanterM80"), { { S1D4("KidneyPlanter"), -8.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D4RopePodiumP80"), { { S1D4("RopePodium"), 8.f, 0.f, 0.f, 0.f, 1.f } });
Zone(TEXT("World s1d4, slot 172"), 7.762f, 7.802f);
	Site(TEXT("WldS1D4BlkCoffeeShopA"), -1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4BlkDriveInB"), 1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampM155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampP155"), 1, 42.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 174"), 7.852f, 7.892f);
	Site(TEXT("WldS1D4ArrowSignM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusP155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampM155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampP155"), 1, 42.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 176"), 7.943f, 7.983f);
	Site(TEXT("WldS1D4BlkDrumLoungeB"), -1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignP155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusM155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), 1, 42.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 178"), 8.033f, 8.073f);
	Site(TEXT("WldS1D4ArrowSignM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignP155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), 1, 42.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 180"), 8.124f, 8.164f);
	Site(TEXT("WldS1D4SodaBarP0"), -1, 29.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampP155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampM155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusP155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 182"), 8.214f, 8.254f);
	Site(TEXT("WldS1D4TubeCactusM155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4PhoneKioskM155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 184"), 8.305f, 8.345f);
	Site(TEXT("WldS1D4ArrowSignM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsM155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignP155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 186"), 8.395f, 8.435f);
	Site(TEXT("WldS1D4SodaBarP0"), -1, 29.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignP155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusM155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampP155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 188"), 8.486f, 8.526f);
	Site(TEXT("WldS1D4PhoneKioskM155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignM155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 190"), 8.576f, 8.616f);
	Site(TEXT("WldS1D4FinPylonsM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampP155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignM155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignP155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 192"), 8.667f, 8.707f);
	Site(TEXT("WldS1D4BlkArcadeHallA"), 1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4SodaBarP0"), -1, 29.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusM155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampP155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 194"), 8.757f, 8.797f);
	Site(TEXT("WldS1D4Horseshoe"), -1, 100.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignM155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4PhoneKioskP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 196"), 8.848f, 8.888f);
	Site(TEXT("WldS1D4BlkMotelOfficeA"), -1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignP155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 198"), 8.938f, 8.978f);
	Site(TEXT("WldS1D4SodaBarP0"), -1, 29.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampP155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusM155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusP155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 200"), 9.029f, 9.069f);
	Site(TEXT("WldS1D4BlkBowlingAlleyB"), -1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4BlkMotelOfficeB"), 1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsM155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4PhoneKioskP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 202"), 9.119f, 9.159f);
	Site(TEXT("WldS1D4TubeCactusM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignM155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 204"), 9.210f, 9.250f);
	Site(TEXT("WldS1D4ArrowSignM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4PhoneKioskP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 206"), 9.300f, 9.340f);
	Site(TEXT("WldS1D4BlkArcadeHallA"), -1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusM155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 208"), 9.391f, 9.431f);
	Site(TEXT("WldS1D4BlkDriveInB"), -1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4PhoneKioskM155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 210"), 9.481f, 9.521f);
	Site(TEXT("WldS1D4BlkDrumLoungeA"), 1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4PhoneKioskP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignM155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 212"), 9.572f, 9.612f);
	Site(TEXT("WldS1D4TubeCactusM155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 214"), 9.662f, 9.702f);
	Site(TEXT("WldS1D4BlkDriveInB"), 1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignM155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsM155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4PhoneKioskP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 216"), 9.752f, 9.792f);
	Site(TEXT("WldS1D4BlkCoffeeShopA"), -1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampM155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4ArrowSignP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusM155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 218"), 9.843f, 9.883f);
	Site(TEXT("WldS1D4BlkDrumLoungeB"), -1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4PhoneKioskM155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4StarLampP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 220"), 9.933f, 9.973f);
	Site(TEXT("WldS1D4FinPylonsM155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4PhoneKioskP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusM155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 222"), 10.024f, 10.064f);
	Site(TEXT("WldS1D4TubeCactusM155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4PhoneKioskM155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 224"), 10.114f, 10.154f);
	Site(TEXT("WldS1D4BlkCoffeeShopA"), 1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusM155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsM155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4PhoneKioskP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 226"), 10.205f, 10.245f);
	Site(TEXT("WldS1D4BlkMotelOfficeA"), -1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusM155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 228"), 10.295f, 10.335f);
	Site(TEXT("WldS1D4BlkBowlingAlleyB"), -1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4PhoneKioskM155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 230"), 10.386f, 10.426f);
	Site(TEXT("WldS1D4BlkBowlingAlleyB"), 1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4PhoneKioskP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4TubeCactusM155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 232"), 10.476f, 10.516f);
	Site(TEXT("WldS1D4Horseshoe"), 1, 100.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsM155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 234"), 10.567f, 10.607f);
	Site(TEXT("WldS1D4BlkDriveInA"), 1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsM155"), 1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4PhoneKioskP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), 1, 22.f, 1, 0, 1.f);
Zone(TEXT("World s1d4, slot 236"), 10.657f, 10.690f);
	Site(TEXT("WldS1D4BlkArcadeHallA"), -1, 67.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4FinPylonsM155"), -1, 42.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4PhoneKioskP155"), -1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4PhoneKioskP155"), 1, 38.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4KidneyPlanterM80"), -1, 22.f, 1, 0, 1.f);
	Site(TEXT("WldS1D4RopePodiumP80"), 1, 22.f, 1, 0, 1.f);
#undef S1D4
// ---- END s1d4 ----
}

{
// ---- BEGIN s1d5 (district 5 "Salt Flats", ft_world_nevada_s1d5.py) ----
// 18 meshes, /Game/World/Nevada/SM_S1D5_*: 8 buildings (TimingTower, PitShelter, ScrutineerShed, CaravanDiner, CommentaryBox,
// CaravanCamp, EvaporatorHouse, SaltTower), 9 props (SaltCone, PennantMasts, Windsock, ResultsBoard, SpeakerPole, SunShade,
// FloodMast: tall; MarkerDrums, TyreRack: low), the hero (BigStopwatch). A speed-week camp and a salt works.
// The zone has slots 237 to 320; this block uses the even ones only (every 8 or 16 with an even phase: s % 4 == 0 or 2).
// FIX ROUND (after Stage 1's review): the block is now written SLOT BY SLOT by C:/ftwork/jobs/L2s1fix/d5_table.py, one zone a slot
// and one-piece sets, every rule a Site of every 1, chance 1. Reason: pass 3b's half-slot scenes stand 28.25 m along from
// their slot (22.6 x their 1.25 scale), not 22.6, so the first camp row stood in them (61 pieces a metre or more inside an
// older piece). Each piece below was tested, primitive box against primitive box, against every older piece of both tables
// and is written only where it overlaps none by 0.3 m or more; 48 places are left open.
// WHERE:
//   CAMP ROW   the six camp buildings and the salt cone, 37 m out (34 or 35.5 m where the first cast's riders stand 42 m out),
//              16 to 25 m along from the slot's middle, on the half of the slot the scenes leave free.
//   KERB ROW   26.5 to 27.5 m out: the tall props. The thin ones (pennant masts, windsock, loudspeakers, floodlight) stand on
//              every second slot of their class only (the driver's review: the near band read as one tangle at speed).
//              The two low props 21.5 m out (the ranch fence stands 18.9 m out).
//   WORKS      the evaporator house and the salt tower on the slot's middle, 66 m out (62 or 69 m beside the land yachts' lane).
//   HERO       THE BIG STOPWATCH, right, twice: slot 252 (11.40 km) 148 m out and slot 308 (13.93 km) 140 m out, turned 30
//              degrees to face the cars coming up the road. Unchanged.
// Light: steady only (M_People cannot blink); every lit mesh stands 26.5 m or more out. No figures, no animals, no words.
// Written: 187 pieces + the hero's 2 = 189 (was 264). Most of one mesh: CommentaryBox 26. Smallest x of a TALL mesh: 26.5 m.
#define S1D5(n) TEXT("/Game/World/Nevada/SM_S1D5_" n)
Set(TEXT("WldS1D5CommentaryBox1"), { { S1D5("CommentaryBox"), -22.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5SaltCone1"), { { S1D5("SaltCone"), 22.0f, 0.f, 0.f, 40.0f, 1.f } });
Set(TEXT("WldS1D5CaravanDiner1"), { { S1D5("CaravanDiner"), 22.0f, 0.f, 0.f, 5.0f, 1.f } });
Set(TEXT("WldS1D5CommentaryBox2"), { { S1D5("CommentaryBox"), 16.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5PennantMasts1"), { { S1D5("PennantMasts"), 22.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5SaltTower1"), { { S1D5("SaltTower"), 0.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5PitShelter1"), { { S1D5("PitShelter"), 22.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5SaltCone2"), { { S1D5("SaltCone"), -22.0f, 0.f, 0.f, -25.0f, 1.f } });
Set(TEXT("WldS1D5ResultsBoard1"), { { S1D5("ResultsBoard"), -22.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5SunShade1"), { { S1D5("SunShade"), -22.0f, 0.f, 0.f, 8.0f, 1.f } });
Set(TEXT("WldS1D5Windsock1"), { { S1D5("Windsock"), 18.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5CaravanCamp1"), { { S1D5("CaravanCamp"), -22.0f, 0.f, 0.f, -6.0f, 1.f } });
Set(TEXT("WldS1D5EvaporatorHouse1"), { { S1D5("EvaporatorHouse"), 0.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5ScrutineerShed1"), { { S1D5("ScrutineerShed"), 16.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5TyreRack1"), { { S1D5("TyreRack"), -6.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5TimingTower1"), { { S1D5("TimingTower"), 22.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5ScrutineerShed2"), { { S1D5("ScrutineerShed"), -22.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5MarkerDrums1"), { { S1D5("MarkerDrums"), 0.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5SpeakerPole1"), { { S1D5("SpeakerPole"), -15.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5CaravanDiner2"), { { S1D5("CaravanDiner"), 16.0f, 0.f, 0.f, 5.0f, 1.f } });
Set(TEXT("WldS1D5FloodMast1"), { { S1D5("FloodMast"), -15.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5CaravanCamp2"), { { S1D5("CaravanCamp"), 22.0f, 0.f, 0.f, 6.0f, 1.f } });
Set(TEXT("WldS1D5CaravanDiner3"), { { S1D5("CaravanDiner"), -22.0f, 0.f, 0.f, -5.0f, 1.f } });
Set(TEXT("WldS1D5TimingTower2"), { { S1D5("TimingTower"), -22.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5CommentaryBox3"), { { S1D5("CommentaryBox"), 22.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5PennantMasts2"), { { S1D5("PennantMasts"), -22.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5PitShelter2"), { { S1D5("PitShelter"), 25.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5ScrutineerShed3"), { { S1D5("ScrutineerShed"), 22.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5PitShelter3"), { { S1D5("PitShelter"), -22.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5CaravanCamp3"), { { S1D5("CaravanCamp"), -25.0f, 0.f, 0.f, -6.0f, 1.f } });
Set(TEXT("WldS1D5CaravanDiner4"), { { S1D5("CaravanDiner"), 22.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5FloodMast2"), { { S1D5("FloodMast"), -18.0f, 0.f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D5Watch"),   { { S1D5("BigStopwatch"), 0.f, 0.f, 0.f, -30.f, 1.f } });
Zone(TEXT("World s1d5, Salt Flats, slot 238"), 10.749737f, 10.785931f);
	Site(TEXT("WldS1D5CommentaryBox1"), 1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltCone1"), 1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanDiner1"), -1, 37.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 240"), 10.840223f, 10.876417f);
	Site(TEXT("WldS1D5CommentaryBox2"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PennantMasts1"), 1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltTower1"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PitShelter1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltCone2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ResultsBoard1"), -1, 26.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 242"), 10.930709f, 10.966903f);
	Site(TEXT("WldS1D5SunShade1"), 1, 27.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5Windsock1"), 1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CommentaryBox2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanCamp1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5EvaporatorHouse1"), -1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 244"), 11.021195f, 11.057389f);
	Site(TEXT("WldS1D5CommentaryBox1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ScrutineerShed1"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TyreRack1"), 1, 21.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TimingTower1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ScrutineerShed2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5MarkerDrums1"), -1, 21.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 246"), 11.111681f, 11.147875f);
	Site(TEXT("WldS1D5SaltCone1"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SpeakerPole1"), 1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanDiner2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5FloodMast1"), -1, 26.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 248"), 11.202167f, 11.238361f);
	Site(TEXT("WldS1D5CaravanCamp2"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PitShelter1"), -1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ResultsBoard1"), -1, 26.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 250"), 11.292653f, 11.328847f);
	Site(TEXT("WldS1D5CaravanDiner3"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SunShade1"), 1, 27.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanCamp1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltTower1"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 252"), 11.383139f, 11.419333f);
	Site(TEXT("WldS1D5CommentaryBox2"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TyreRack1"), 1, 21.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TimingTower1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ScrutineerShed2"), -1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5MarkerDrums1"), -1, 21.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 254"), 11.473625f, 11.509819f);
	Site(TEXT("WldS1D5SaltCone1"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanDiner1"), -1, 37.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 256"), 11.564111f, 11.600305f);
	Site(TEXT("WldS1D5TimingTower2"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanCamp2"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PennantMasts1"), 1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltTower1"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CommentaryBox2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ResultsBoard1"), -1, 26.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 258"), 11.654597f, 11.690791f);
	Site(TEXT("WldS1D5CaravanDiner3"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SunShade1"), 1, 27.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5Windsock1"), 1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CommentaryBox3"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanCamp1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5EvaporatorHouse1"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 260"), 11.745083f, 11.781277f);
	Site(TEXT("WldS1D5CommentaryBox2"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TyreRack1"), 1, 21.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TimingTower1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ScrutineerShed2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PennantMasts2"), -1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5MarkerDrums1"), -1, 21.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 262"), 11.835569f, 11.871763f);
	Site(TEXT("WldS1D5SaltCone1"), 1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SpeakerPole1"), 1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanDiner2"), -1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5FloodMast1"), -1, 26.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 264"), 11.926055f, 11.962249f);
	Site(TEXT("WldS1D5TimingTower2"), 1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5EvaporatorHouse1"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PitShelter2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltCone2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ResultsBoard1"), -1, 26.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 266"), 12.016541f, 12.052735f);
	Site(TEXT("WldS1D5SunShade1"), 1, 27.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CommentaryBox2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanCamp1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltTower1"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 268"), 12.107027f, 12.143221f);
	Site(TEXT("WldS1D5ScrutineerShed3"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TyreRack1"), 1, 21.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TimingTower1"), -1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5MarkerDrums1"), -1, 21.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 270"), 12.197513f, 12.233707f);
	Site(TEXT("WldS1D5CommentaryBox1"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltCone1"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CommentaryBox2"), -1, 37.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 272"), 12.287999f, 12.324193f);
	Site(TEXT("WldS1D5PennantMasts1"), 1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PitShelter1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltCone2"), -1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ResultsBoard1"), -1, 26.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 274"), 12.378485f, 12.414679f);
	Site(TEXT("WldS1D5SunShade1"), 1, 27.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5Windsock1"), 1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CommentaryBox3"), -1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanCamp1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5EvaporatorHouse1"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 276"), 12.468971f, 12.505165f);
	Site(TEXT("WldS1D5PitShelter3"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ScrutineerShed3"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TyreRack1"), 1, 21.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CommentaryBox2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PennantMasts2"), -1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5MarkerDrums1"), -1, 21.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 278"), 12.559457f, 12.595651f);
	Site(TEXT("WldS1D5CommentaryBox1"), 1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltCone1"), 1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SpeakerPole1"), 1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanDiner1"), -1, 37.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 280"), 12.649943f, 12.686137f);
	Site(TEXT("WldS1D5EvaporatorHouse1"), 1, 62.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PitShelter1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltCone2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ResultsBoard1"), -1, 26.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 282"), 12.740429f, 12.776623f);
	Site(TEXT("WldS1D5SunShade1"), 1, 27.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanCamp3"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltTower1"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 284"), 12.830915f, 12.867109f);
	Site(TEXT("WldS1D5CommentaryBox1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ScrutineerShed3"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TyreRack1"), 1, 21.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanDiner4"), -1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ScrutineerShed2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5MarkerDrums1"), -1, 21.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 286"), 12.921401f, 12.957595f);
	Site(TEXT("WldS1D5SaltCone1"), 1, 37.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 288"), 13.011887f, 13.048081f);
	Site(TEXT("WldS1D5CommentaryBox2"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PennantMasts1"), 1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltTower1"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PitShelter1"), -1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ResultsBoard1"), -1, 26.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 290"), 13.102373f, 13.138567f);
	Site(TEXT("WldS1D5CaravanDiner3"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SunShade1"), 1, 27.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5Windsock1"), 1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanCamp1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5EvaporatorHouse1"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 292"), 13.192859f, 13.229053f);
	Site(TEXT("WldS1D5CommentaryBox2"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TyreRack1"), 1, 21.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TimingTower1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ScrutineerShed2"), -1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PennantMasts2"), -1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5MarkerDrums1"), -1, 21.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 294"), 13.283345f, 13.319539f);
	Site(TEXT("WldS1D5SaltCone1"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SpeakerPole1"), 1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanDiner1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5FloodMast2"), -1, 26.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 296"), 13.373831f, 13.410025f);
	Site(TEXT("WldS1D5TimingTower2"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanCamp2"), 1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5EvaporatorHouse1"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CommentaryBox2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ResultsBoard1"), -1, 26.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 298"), 13.464317f, 13.500511f);
	Site(TEXT("WldS1D5CaravanDiner3"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SunShade1"), 1, 27.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CommentaryBox3"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanCamp1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltTower1"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 300"), 13.554803f, 13.590997f);
	Site(TEXT("WldS1D5CommentaryBox2"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TyreRack1"), 1, 21.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TimingTower1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ScrutineerShed2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5MarkerDrums1"), -1, 21.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 302"), 13.645289f, 13.681483f);
	Site(TEXT("WldS1D5SaltCone1"), 1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanDiner2"), -1, 34.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 304"), 13.735775f, 13.771969f);
	Site(TEXT("WldS1D5TimingTower2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanCamp2"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltTower1"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PitShelter1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltCone2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ResultsBoard1"), -1, 26.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 306"), 13.826261f, 13.862455f);
	Site(TEXT("WldS1D5SunShade1"), 1, 27.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5Windsock1"), 1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CommentaryBox2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanCamp1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5EvaporatorHouse1"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 308"), 13.916747f, 13.952941f);
	Site(TEXT("WldS1D5ScrutineerShed3"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TyreRack1"), 1, 21.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TimingTower1"), -1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PennantMasts2"), -1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5MarkerDrums1"), -1, 21.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 310"), 14.007233f, 14.043427f);
	Site(TEXT("WldS1D5CommentaryBox1"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltCone1"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanDiner2"), -1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5FloodMast1"), -1, 26.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 312"), 14.097719f, 14.133913f);
	Site(TEXT("WldS1D5CommentaryBox2"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5EvaporatorHouse1"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PitShelter1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltCone2"), -1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ResultsBoard1"), -1, 26.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 314"), 14.188205f, 14.224399f);
	Site(TEXT("WldS1D5SunShade1"), 1, 27.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CommentaryBox3"), -1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanCamp1"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltTower1"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 316"), 14.278691f, 14.314885f);
	Site(TEXT("WldS1D5ScrutineerShed3"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5TyreRack1"), 1, 21.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CommentaryBox2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5MarkerDrums1"), -1, 21.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 318"), 14.369177f, 14.405371f);
	Site(TEXT("WldS1D5CommentaryBox1"), 1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltCone1"), 1, 34.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5CaravanDiner1"), -1, 37.0f, 1, 0, 1.f);
Zone(TEXT("World s1d5, Salt Flats, slot 320"), 14.459663f, 14.495857f);
	Site(TEXT("WldS1D5CaravanCamp2"), 1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PennantMasts1"), 1, 26.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D5SaltTower1"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5PitShelter2"), -1, 37.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D5ResultsBoard1"), -1, 26.5f, 1, 0, 1.f);
Zone(TEXT("World s1d5, the Big Stopwatch 11.40"), 11.39f, 11.41f);                    // slot 252
	Site(TEXT("WldS1D5Watch"), 1, 148.f, 4, 0, 1.f);
Zone(TEXT("World s1d5, the Big Stopwatch 13.93"), 13.925f, 13.945f);                  // slot 308
	Site(TEXT("WldS1D5Watch"), 1, 140.f, 4, 0, 1.f);
#undef S1D5
// ---- END s1d5 ----
}

{
// ---- BEGIN s1d6 (district 6 "Syndicate Range", ft_world_nevada_s1d6.py) ----
// 18 meshes: 7 buildings, 10 props (3 low), the hero THE BIG EAR. 14.5 to 18.5 km: slots 321 to 408, even slots only
// (11 slots of each of s % 8 == 0, 2, 4, 6). Everything stands by Site (a Set on its slot, nothing slides along) but the low props.
//   60 to 70 m (anchor 66 m): six compounds of two or three pieces, one to a side and a slot class, on the classes the
//     pass 3 water tower (left, s % 8 == 0, 60-68 m) and trestle (right, s % 8 == 4, 60-68 m) do not use. The buildings stand
//     10 to 12 m along from the slot's middle, which stays open (pass 3a has a piece 60 to 63.5 m out on some of these slots).
//   (as first written; see FIX ROUND below) 26 to 27 m (anchor 26 m): slim tall props in pairs, 14 m before and after the slot's middle (the cast's decks stand ON the
//     middle, 27 to 37 m out), every even slot, both sides, chance 0.6.
//   21 to 23.5 m: the three low props (Scatter). The ranch fence is 18.9 m out.
//   The hero: right side, 112 m out, on slots 346 (15.65 km) and 394 (17.83 km): chosen s % 12 == 10 so that the level's
//     hangar set (s % 4 == 0), radar posts and spires (s % 3 == 2), ridge dunes (s % 3 == 0) and, on these two, the mesa
//     stubs (s % 5 == 2) and the cast's far pieces have no rule there.
// FIX ROUND (after Stage 1's review), written by C:/ftwork/jobs/L2s1fix/d6_table.py: the compounds and the kerb props now stand
//   SLOT BY SLOT (one zone a slot, Site of every 1, chance 1), each tested box against box against every older piece of both
//   tables and written only where it overlaps none by 0.3 m or more. A compound's anchor is 66 m, or 69.5 or 63 m where pass
//   3a's group or the round pen's fence stands; 5 compounds are left out. The kerb props are single pieces now: the thin
//   ones (camera mast, warning board, pylons) on a quarter of their slots, the cowls and the target on half (the driver's
//   review: the near band read as one dark tangle). THE COWLS ARE TURNED ROUND (yaw 180): their two dark mouths pointed
//   across the road and read as a twin gun; now the road sees their backs and the white vane.
//   The hero is 1.3 times as built (it read only as a small white dish): slot 346 100 m out (was 112), slot 394 124 m out
//   (112 m there stood in the lane of the first cast's wild horses, which run past from slot 395).
// Written: 186 Site pieces + the low Scatters (about 44 x 0.75) + the hero's 2. Most of one mesh: WindSock 22.
//   Smallest x of a TALL mesh: 26 m (the kerb props; the turned cowls reach in to 24.97 m).
#define S1D6(n) TEXT("/Game/World/Nevada/SM_S1D6_" n)
Set(TEXT("WldS1D6Radar"), {                        // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D6("RadomeTower"), -11.5f,  1.0f, 0.f,  0.f, 1.f },
	{ S1D6("AerialHut"),    10.5f,  0.0f, 0.f,  0.f, 1.f },
	{ S1D6("WindSock"),     -1.0f, -1.0f, 0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D6Depot"), {
	{ S1D6("Magazines"),    -3.0f,  3.5f, 0.f,  0.f, 1.f },
	{ S1D6("WindSock"),     15.0f, -1.0f, 0.f, 20.f, 1.f },
});
Set(TEXT("WldS1D6Command"), {
	{ S1D6("CommandBunker"), -10.0f, 1.0f, 0.f, 0.f, 1.f },
	{ S1D6("TrackHouse"),     10.5f, 1.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Stand"), {
	{ S1D6("TestStand"),    -7.5f,  1.5f, 0.f,  0.f, 1.f },
	{ S1D6("BlastWall"),    11.5f, -2.0f, 0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D6Gate"), {
	{ S1D6("GateTower"),   -10.0f,  0.0f, 0.f,  0.f, 1.f },
	{ S1D6("BlastWall"),     8.0f,  1.5f, 0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D6Dome"), {
	{ S1D6("CommandBunker"), -10.5f, 1.0f, 0.f, 0.f, 1.f },
	{ S1D6("RadomeTower"),    10.0f, 1.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6BigEar"), {
	{ S1D6("BigEar"),        0.0f,  0.0f, 0.f,  0.f, 1.f },
});
Set(TEXT("WldS1D6WarnBoard1"), { { S1D6("WarnBoard"), -14.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6CameraMast1"), { { S1D6("CameraMast"), -14.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6Pylons1"), { { S1D6("Pylons"), -14.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6Cowls1"), { { S1D6("Cowls"), 14.0f, 0.5f, 0.f, 180.0f, 1.f } });
Set(TEXT("WldS1D6CalTarget1"), { { S1D6("CalTarget"), -17.0f, 0.5f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6CalTarget2"), { { S1D6("CalTarget"), -14.0f, 0.5f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6WarnBoard2"), { { S1D6("WarnBoard"), 14.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6CameraMast2"), { { S1D6("CameraMast"), 14.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6Pylons2"), { { S1D6("Pylons"), 14.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D6CalTarget3"), { { S1D6("CalTarget"), -11.0f, 0.5f, 0.f, 0.0f, 1.f } });
Zone(TEXT("World s1d6, Syndicate Range, slot 322"), 14.550149f, 14.586343f);
	Site(TEXT("WldS1D6Gate"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6WarnBoard1"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Radar"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 324"), 14.640635f, 14.676829f);
	Site(TEXT("WldS1D6CameraMast1"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Depot"), -1, 69.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Pylons1"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 326"), 14.731121f, 14.767315f);
	Site(TEXT("WldS1D6Command"), -1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6WarnBoard1"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 328"), 14.821607f, 14.857801f);
	Site(TEXT("WldS1D6Stand"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Cowls1"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 330"), 14.912093f, 14.948287f);
	Site(TEXT("WldS1D6Gate"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Radar"), -1, 69.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CalTarget1"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 332"), 15.002579f, 15.038773f);
	Site(TEXT("WldS1D6Depot"), -1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Cowls1"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 334"), 15.093065f, 15.129259f);
	Site(TEXT("WldS1D6Dome"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CalTarget2"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Command"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 336"), 15.183551f, 15.219745f);
	Site(TEXT("WldS1D6Stand"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6WarnBoard2"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 338"), 15.274037f, 15.310231f);
	Site(TEXT("WldS1D6Gate"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CameraMast2"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Radar"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 340"), 15.364523f, 15.400717f);
	Site(TEXT("WldS1D6WarnBoard2"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Depot"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 342"), 15.455009f, 15.491203f);
	Site(TEXT("WldS1D6Dome"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Pylons2"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CameraMast2"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 344"), 15.545495f, 15.581689f);
	Site(TEXT("WldS1D6Stand"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Cowls1"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 346"), 15.635981f, 15.672175f);
	Site(TEXT("WldS1D6Gate"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Radar"), -1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CalTarget2"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 348"), 15.726467f, 15.762661f);
	Site(TEXT("WldS1D6Depot"), -1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Cowls1"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 350"), 15.816953f, 15.853147f);
	Site(TEXT("WldS1D6Dome"), 1, 69.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CalTarget2"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Command"), -1, 63.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 352"), 15.907439f, 15.943633f);
	Site(TEXT("WldS1D6Stand"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Pylons1"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CameraMast1"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 354"), 15.997925f, 16.034119f);
	Site(TEXT("WldS1D6Gate"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6WarnBoard1"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Radar"), -1, 69.5f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 356"), 16.088411f, 16.124605f);
	Site(TEXT("WldS1D6CameraMast1"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Depot"), -1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Pylons1"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 358"), 16.178897f, 16.215091f);
	Site(TEXT("WldS1D6Dome"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Command"), -1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6WarnBoard1"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 360"), 16.269383f, 16.305577f);
	Site(TEXT("WldS1D6Stand"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Cowls1"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 362"), 16.359869f, 16.396063f);
	Site(TEXT("WldS1D6Gate"), 1, 69.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Radar"), -1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CalTarget3"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 364"), 16.450355f, 16.486549f);
	Site(TEXT("WldS1D6Depot"), -1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Cowls1"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 366"), 16.540841f, 16.577035f);
	Site(TEXT("WldS1D6Dome"), 1, 63.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CalTarget2"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 368"), 16.631327f, 16.667521f);
	Site(TEXT("WldS1D6Stand"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6WarnBoard2"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 370"), 16.721813f, 16.758007f);
	Site(TEXT("WldS1D6Gate"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CameraMast2"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Radar"), -1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Pylons2"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 372"), 16.812299f, 16.848493f);
	Site(TEXT("WldS1D6WarnBoard2"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Depot"), -1, 69.5f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 374"), 16.902785f, 16.938979f);
	Site(TEXT("WldS1D6Pylons2"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Command"), -1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CameraMast2"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 376"), 16.993271f, 17.029465f);
	Site(TEXT("WldS1D6Stand"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Cowls1"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 378"), 17.083757f, 17.119951f);
	Site(TEXT("WldS1D6Gate"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Radar"), -1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CalTarget3"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 380"), 17.174243f, 17.210437f);
	Site(TEXT("WldS1D6Depot"), -1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Cowls1"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 382"), 17.264729f, 17.300923f);
	Site(TEXT("WldS1D6Dome"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CalTarget2"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Command"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 384"), 17.355215f, 17.391409f);
	Site(TEXT("WldS1D6Stand"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Pylons1"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CameraMast1"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 386"), 17.445701f, 17.481895f);
	Site(TEXT("WldS1D6Gate"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6WarnBoard1"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Radar"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 388"), 17.536187f, 17.572381f);
	Site(TEXT("WldS1D6CameraMast1"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Depot"), -1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Pylons1"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 390"), 17.626673f, 17.662867f);
	Site(TEXT("WldS1D6Dome"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Command"), -1, 69.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D6WarnBoard1"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 392"), 17.717159f, 17.753353f);
	Site(TEXT("WldS1D6Cowls1"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 394"), 17.807645f, 17.843839f);
	Site(TEXT("WldS1D6Gate"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Radar"), -1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CalTarget2"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 396"), 17.898131f, 17.934325f);
	Site(TEXT("WldS1D6Depot"), -1, 69.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Cowls1"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 398"), 17.988617f, 18.024811f);
	Site(TEXT("WldS1D6Dome"), 1, 63.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CalTarget2"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Command"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 400"), 18.079103f, 18.115297f);
	Site(TEXT("WldS1D6Stand"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6WarnBoard2"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 402"), 18.169589f, 18.205783f);
	Site(TEXT("WldS1D6Gate"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CameraMast2"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Radar"), -1, 69.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Pylons2"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 404"), 18.260075f, 18.296269f);
	Site(TEXT("WldS1D6WarnBoard2"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Depot"), -1, 66.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 406"), 18.350561f, 18.386755f);
	Site(TEXT("WldS1D6Dome"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Pylons2"), 1, 26.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Command"), -1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6CameraMast2"), -1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range, slot 408"), 18.441047f, 18.477241f);
	Site(TEXT("WldS1D6Stand"), 1, 66.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D6Cowls1"), 1, 26.0f, 1, 0, 1.f);
Zone(TEXT("World s1d6, Syndicate Range"), 14.5f, 18.5f);
	Scatter(S1D6("TankTraps"),  21.f, 23.f,  4, 2, 0.5f, 1.f, 0.f,  0, 0.f);     // low, both sides, s % 4 == 2
	Scatter(S1D6("CableDrums"), 21.f, 23.5f, 4, 0, 0.5f, 1.f, 0.f, -1, 8.f);     // low, left,  s % 4 == 0
	Scatter(S1D6("Hatch"),      21.f, 23.5f, 4, 0, 0.5f, 1.f, 0.f,  1, 8.f);     // low, right, s % 4 == 0
Zone(TEXT("World s1d6, the Big Ear, first"), 15.632f, 15.676f);                  // slot 346 only
	Site(TEXT("WldS1D6BigEar"), 1, 100.f, 1, 0, 1.f, 1.3f);
Zone(TEXT("World s1d6, the Big Ear, second"), 17.804f, 17.848f);                 // slot 394 only
	Site(TEXT("WldS1D6BigEar"), 1, 124.f, 1, 0, 1.f, 1.3f);
#undef S1D6
// ---- END s1d6 ----
}

{
// ---- BEGIN s1d7 (district 7 "Red Rock Return", 18.5 to 22 km, ft_world_nevada_s1d7.py) ----
// 18 meshes: 8 buildings, 9 props, 1 hero (THE TWIN-WHEEL HEADFRAME). Every name below is one the make log printed (C:/ftwork/logs/L2s1d7_make.log).
// The district is slots 409 to 486; this block uses its EVEN slots only (s % 4 == 0 or 2), each Site in a ONE-SLOT Zone, chance 1 (the
// people table's own way), because what stands there already differs slot by slot. Read from FTDressL2.inl and FTPeopleL2.inl:
//   the three rock cuts (SM_RockCut 25 to 35 m out, 15 m tall: slots 411-413, 415-427, 441, 443-455, 457, 472-483, 485-486);
//   the second wall behind it (SM_RockWall2 at 0.87: 29 to 44 m out, 31 m tall, chance 0.8: left when s % 3 == 2, right when s % 3 == 1);
//   the rim people 33.1 m out; the pass 3 needle 44 to 50 m (left s % 3 == 0, right s % 3 == 2); hoodoos 44 to 90 m on every even slot;
//   in the two gaps (428-440, 458-471): camera towers and stunt rigs 38.7 to 44.1 m out in the middle of every even slot, the dig and the
//   sky watch 34 to 42 m out in the far half of 428R 430L 432R 434L 458L 464R 466L, the pack train (left) and the stagecoach (right) on a
//   trail 86.7 to 105.3 m out, a trestle 60 to 68 m left in 432 440 464, the Canyon set from 68 m on s % 4 == 0, rock banks 24.7 to 38.8 m
//   everywhere; a wash bed 20 to 84 m out across slots 414 428 442 456 470 484 (in the cuts those four are openings: no wall, no rim people).
// So: GAPS: buildings 20 m either side of the slot's middle (between the scatter of this slot and the next), fronts 43.6 m out or more; tall
// props 26 to 39.5 m; low props 20 to 21 m. WASH slots: the Wash set. CUTS: one side a slot, the side without the second wall, 41 m out, on
// columns of 17 m. Not dodged, and saying so: the rock banks (2.6 m tall, chance 0.7, 24.7 to 38.8 m: the tall props stand among them) and the
// hoodoos' random places (44 to 90 m: a building 20 m from the slot's middle is outside most of their 11 m slide, not all of it).
// Count (exact: every Site is written out, chance 1; nothing is taken off for the track's share):
//   CampA   8 sites x 5 parts =  40   432R 434L 436R 440R 458L 462L 464R 468L
//   CampB   9 sites x 5 parts =  45   410R 430L 434R 438L 440L 460R 464L 466L 468R
//   CampC   9 sites x 5 parts =  45   410L 430R 432L 436L 438R 458R 460L 462R 466R
//   Wash   12 sites x 6 parts =  72   414L 414R 428L 428R 442L 442R 456L 456R 470L 470R 484L 484R
//   RimA    7 sites x 2 parts =  14   412L 424L 454L 478L 426R 450R 480R
//   RimB    7 sites x 2 parts =  14   418L 448L 472L 420R 444R 474R 486R
//   RimF    3 sites x 1 parts =   3   416R 446R 476R
//   RimH    3 sites x 1 parts =   3   422R 452R 482R
//   Hero    2 sites x 1 parts =   2   432R 464R
//   SUM 238 placed in 60 sites.
//   Per mesh: AssayTower 21, Tipple 21, Flume 21, TramTower 21, SluiceBox 21, OreCart 20, BoardingRim 17, RimFrame 17, AditPortal 9, HoistHouse 9, PowderHouse 9, BellFrame 9, Windlass 9, OreMill 8, BunkHouse 8, StampBattery 8, WashLine 8, TwinHeadframe 2.
// Smallest x of a TALL mesh: 26.0 m (BellFrame in CampB). Smallest x of a low mesh: 20.0 m (OreCart in Wash).
// Glow: steady lamp rooms and lanterns only, on TALL meshes (the nearest 26.0 m out: BellFrame); the two low meshes have none. Nothing blinks.
// No mesh stands both inside and outside 70 m (the hero is the only one outside).
// FIX ROUND (after Stage 1's review): CampC's flume stands 4 m along, not 16 (it stood 1.2 m in pass 3b's dig, which is 28.25 m along from
// its slot, not 22.6); 470R is left open (its tipple stood in the dig); the last zone ends at 22.0 km. Counts above: 232 placed in 59 sites.
#define S1D7(n) TEXT("/Game/World/Nevada/SM_S1D7_" n)
// the mill camp: the stepped mill and the boarding house either side of the slot, the stamps and wash day in front, carts by the road
Set(TEXT("WldS1D7CampA"), {                       // anchor 40 m   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("OreMill"), 20.0f, 14.0f, 0.f, 0.0f, 1.f },   // 54.0 m out; TALL, top 24.0 m
	{ S1D7("BunkHouse"), -20.0f, 10.5f, 0.f, 0.0f, 1.f },   // 50.5 m out; TALL, top 15.4 m
	{ S1D7("StampBattery"), -12.0f, -12.8f, 0.f, 0.0f, 1.f },   // 27.2 m out; TALL, top 7.5 m
	{ S1D7("WashLine"), -20.0f, -0.5f, 0.f, 0.0f, 1.f },   // 39.5 m out; TALL, top 2.8 m
	{ S1D7("OreCart"), 4.0f, -19.5f, 0.f, 0.0f, 1.f },   // 20.5 m out; low, top 1.5 m
});
// the hoist camp: the hoist house and the powder house, the shift bell and a prospect shaft in front, a sluice by the road
Set(TEXT("WldS1D7CampB"), {                       // anchor 40 m   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("HoistHouse"), 20.0f, 11.5f, 0.f, 0.0f, 1.f },   // 51.5 m out; TALL, top 25.1 m
	{ S1D7("PowderHouse"), -20.0f, 9.5f, 0.f, 0.0f, 1.f },   // 49.5 m out; TALL, top 9.0 m
	{ S1D7("BellFrame"), -13.0f, -14.0f, 0.f, 0.0f, 1.f },   // 26.0 m out; TALL, top 6.0 m
	{ S1D7("Windlass"), 13.0f, -13.5f, 0.f, 0.0f, 1.f },   // 26.5 m out; TALL, top 3.6 m
	{ S1D7("SluiceBox"), -3.0f, -19.0f, 0.f, 0.0f, 1.f },   // 21.0 m out; low, top 1.8 m
});
// the assay camp: the assay tower and a tipple, an adit, a flume running away from the road, a tramway tower
Set(TEXT("WldS1D7CampC"), {                       // anchor 40 m   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("AssayTower"), 20.0f, 9.0f, 0.f, 0.0f, 1.f },   // 49.0 m out; TALL, top 21.9 m
	{ S1D7("Tipple"), -20.0f, 10.5f, 0.f, 0.0f, 1.f },   // 50.5 m out; TALL, top 14.0 m
	{ S1D7("AditPortal"), 13.0f, -11.0f, 0.f, 0.0f, 1.f },   // 29.0 m out; TALL, top 5.2 m
	{ S1D7("Flume"), -4.0f, -6.0f, 0.f, 90.0f, 1.f },   // 34.0 m out; TALL, top 7.0 m (fix round: along -16 stood 1.2 m in pass 3b's dig, which is 28.25 m along, not 22.6)
	{ S1D7("TramTower"), 21.0f, -12.5f, 0.f, 0.0f, 1.f },   // 27.5 m out; TALL, top 14.7 m
});
// where a wash crosses (its bed lies 20 to 84 m out, 11 m either side of the slot's middle): a tipple and the assay tower on its two banks, a flume carried over it, a tramway tower, a sluice and carts at its mouth
Set(TEXT("WldS1D7Wash"), {                       // anchor 40 m   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("Tipple"), -14.8f, 3.5f, 0.f, 0.0f, 1.f },   // 43.5 m out; TALL, top 14.0 m
	{ S1D7("AssayTower"), 15.0f, 2.0f, 0.f, 0.0f, 1.f },   // 42.0 m out; TALL, top 21.9 m
	{ S1D7("TramTower"), -14.5f, -12.5f, 0.f, 0.0f, 1.f },   // 27.5 m out; TALL, top 14.7 m
	{ S1D7("Flume"), 0.0f, -9.0f, 0.f, 0.0f, 1.f },   // 31.0 m out; TALL, top 7.0 m
	{ S1D7("SluiceBox"), -16.0f, -19.0f, 0.f, 0.0f, 1.f },   // 21.0 m out; low, top 1.8 m
	{ S1D7("OreCart"), 16.0f, -20.0f, 0.f, 0.0f, 1.f },   // 20.0 m out; low, top 1.5 m
});
// the cuts: a boarding house and a prospect headframe, each on its own 17 m column, behind the rim (the cut is 25 to 35 m out)
Set(TEXT("WldS1D7RimA"), {                       // anchor 41 m   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("BoardingRim"), -9.5f, 0.0f, 0.f, 0.0f, 1.f },   // 41.0 m out; TALL, top 28.0 m
	{ S1D7("RimFrame"), 10.0f, 0.0f, 0.f, 0.0f, 1.f },   // 41.0 m out; TALL, top 33.2 m
});
// the same, the other way round
Set(TEXT("WldS1D7RimB"), {                       // anchor 41 m   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("RimFrame"), -9.5f, 0.0f, 0.f, 0.0f, 1.f },   // 41.0 m out; TALL, top 33.2 m
	{ S1D7("BoardingRim"), 10.0f, 0.0f, 0.f, 0.0f, 1.f },   // 41.0 m out; TALL, top 28.0 m
});
// the cuts, slots with a needle 44 to 50 m out: one headframe at the slot's far end
Set(TEXT("WldS1D7RimF"), {                       // anchor 41 m   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("RimFrame"), 19.8f, 0.0f, 0.f, 0.0f, 1.f },   // 41.0 m out; TALL, top 33.2 m
});
// the same, a boarding house
Set(TEXT("WldS1D7RimH"), {                       // anchor 41 m   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("BoardingRim"), 19.8f, 0.0f, 0.f, 0.0f, 1.f },   // 41.0 m out; TALL, top 28.0 m
});
// THE TWIN-WHEEL HEADFRAME, beyond the stagecoach trail (86.7 to 105.3 m out on this side)
Set(TEXT("WldS1D7Hero"), {                       // anchor 112 m   { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("TwinHeadframe"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },   // 112.0 m out; TALL, top 45.2 m
});
// One Site in one slot: slot S is the Zone from (S - 0.5) to (S + 0.5) slots of 45.243 m; every 4, the phase that picks S.
auto S1D7At = [&](int32 S, const TCHAR* SetName, int32 Side, float X)
{
	Zone(TEXT("World s1d7, Red Rock Return"), (S - 0.5f) * 0.045243f, FMath::Min((S + 0.5f) * 0.045243f, 22.f));   // fix round: slot 486's zone ended at 22.0107 km
	Site(SetName, Side, X, 4, (4 - S % 4) % 4, 1.f);
};
// CampA: s % 4 == 0 in 432R 436R 440R 464R 468L; s % 4 == 2 in 434L 458L 462L
S1D7At(432, TEXT("WldS1D7CampA"), 1, 40.0f); S1D7At(434, TEXT("WldS1D7CampA"), -1, 40.0f); S1D7At(436, TEXT("WldS1D7CampA"), 1, 40.0f);
S1D7At(440, TEXT("WldS1D7CampA"), 1, 40.0f); S1D7At(458, TEXT("WldS1D7CampA"), -1, 40.0f); S1D7At(462, TEXT("WldS1D7CampA"), -1, 40.0f);
S1D7At(464, TEXT("WldS1D7CampA"), 1, 40.0f); S1D7At(468, TEXT("WldS1D7CampA"), -1, 40.0f);
// CampB: s % 4 == 0 in 440L 460R 464L 468R; s % 4 == 2 in 410R 430L 434R 438L 466L
S1D7At(410, TEXT("WldS1D7CampB"), 1, 40.0f); S1D7At(430, TEXT("WldS1D7CampB"), -1, 40.0f); S1D7At(434, TEXT("WldS1D7CampB"), 1, 40.0f);
S1D7At(438, TEXT("WldS1D7CampB"), -1, 40.0f); S1D7At(440, TEXT("WldS1D7CampB"), -1, 40.0f); S1D7At(460, TEXT("WldS1D7CampB"), 1, 40.0f);
S1D7At(464, TEXT("WldS1D7CampB"), -1, 40.0f); S1D7At(466, TEXT("WldS1D7CampB"), -1, 40.0f); S1D7At(468, TEXT("WldS1D7CampB"), 1, 40.0f);
// CampC: s % 4 == 0 in 432L 436L 460L; s % 4 == 2 in 410L 430R 438R 458R 462R 466R
S1D7At(410, TEXT("WldS1D7CampC"), -1, 40.0f); S1D7At(430, TEXT("WldS1D7CampC"), 1, 40.0f); S1D7At(432, TEXT("WldS1D7CampC"), -1, 40.0f);
S1D7At(436, TEXT("WldS1D7CampC"), -1, 40.0f); S1D7At(438, TEXT("WldS1D7CampC"), 1, 40.0f); S1D7At(458, TEXT("WldS1D7CampC"), 1, 40.0f);
S1D7At(460, TEXT("WldS1D7CampC"), -1, 40.0f); S1D7At(462, TEXT("WldS1D7CampC"), 1, 40.0f); S1D7At(466, TEXT("WldS1D7CampC"), 1, 40.0f);
// Wash: s % 4 == 0 in 428L 428R 456L 456R 484L 484R; s % 4 == 2 in 414L 414R 442L 442R 470L 470R
S1D7At(414, TEXT("WldS1D7Wash"), -1, 40.0f); S1D7At(414, TEXT("WldS1D7Wash"), 1, 40.0f); S1D7At(428, TEXT("WldS1D7Wash"), -1, 40.0f);
S1D7At(428, TEXT("WldS1D7Wash"), 1, 40.0f); S1D7At(442, TEXT("WldS1D7Wash"), -1, 40.0f); S1D7At(442, TEXT("WldS1D7Wash"), 1, 40.0f);
S1D7At(456, TEXT("WldS1D7Wash"), -1, 40.0f); S1D7At(456, TEXT("WldS1D7Wash"), 1, 40.0f); S1D7At(470, TEXT("WldS1D7Wash"), -1, 40.0f);
/* fix round: 470R left open, its tipple stood in pass 3b's dig */ S1D7At(484, TEXT("WldS1D7Wash"), -1, 40.0f); S1D7At(484, TEXT("WldS1D7Wash"), 1, 40.0f);
// RimA: s % 4 == 0 in 412L 424L 480R; s % 4 == 2 in 454L 478L 426R 450R
S1D7At(412, TEXT("WldS1D7RimA"), -1, 41.0f); S1D7At(424, TEXT("WldS1D7RimA"), -1, 41.0f); S1D7At(454, TEXT("WldS1D7RimA"), -1, 41.0f);
S1D7At(478, TEXT("WldS1D7RimA"), -1, 41.0f); S1D7At(426, TEXT("WldS1D7RimA"), 1, 41.0f); S1D7At(450, TEXT("WldS1D7RimA"), 1, 41.0f);
S1D7At(480, TEXT("WldS1D7RimA"), 1, 41.0f);
// RimB: s % 4 == 0 in 448L 472L 420R 444R; s % 4 == 2 in 418L 474R 486R
S1D7At(418, TEXT("WldS1D7RimB"), -1, 41.0f); S1D7At(448, TEXT("WldS1D7RimB"), -1, 41.0f); S1D7At(472, TEXT("WldS1D7RimB"), -1, 41.0f);
S1D7At(420, TEXT("WldS1D7RimB"), 1, 41.0f); S1D7At(444, TEXT("WldS1D7RimB"), 1, 41.0f); S1D7At(474, TEXT("WldS1D7RimB"), 1, 41.0f);
S1D7At(486, TEXT("WldS1D7RimB"), 1, 41.0f);
// RimF: s % 4 == 0 in 416R 476R; s % 4 == 2 in 446R
S1D7At(416, TEXT("WldS1D7RimF"), 1, 41.0f); S1D7At(446, TEXT("WldS1D7RimF"), 1, 41.0f); S1D7At(476, TEXT("WldS1D7RimF"), 1, 41.0f);
// RimH: s % 4 == 0 in 452R; s % 4 == 2 in 422R 482R
S1D7At(422, TEXT("WldS1D7RimH"), 1, 41.0f); S1D7At(452, TEXT("WldS1D7RimH"), 1, 41.0f); S1D7At(482, TEXT("WldS1D7RimH"), 1, 41.0f);
// Hero: s % 4 == 0 in 432R 464R; s % 4 == 2 in none
S1D7At(432, TEXT("WldS1D7Hero"), 1, 112.0f); S1D7At(464, TEXT("WldS1D7Hero"), 1, 112.0f);
#undef S1D7
// ---- END s1d7 ----
}

{
// ---- BEGIN s1d8 (district 8 "Vegas Approach", ft_world_nevada_s1d8.py) ----
// 16 meshes: 7 buildings (CarriagePorch, HotelTower, WinterGarden, Arcade, Theatre on the s % 4 == 0 slots; ClockPavilion, Belvedere on s % 4 == 2),
// 8 props (LampStandard, Topiary, UrnBalustrade, FlagCourt, GateArch, VictoryColumn TALL; PoolEdge, RopeLine low), the hero (CrownTower, twice).
// Steady light only (glow), on pieces stood 35.5 m out and more. NOT COMPILED, NOT SEEN IN THE GAME when written. Written by C:/ftwork/jobs/L2s1d8/block.py.
// One Zone a slot (the 39 even slots of the district, 488 to 564: 22.08 to 25.52 km), Site rules only (every 1, chance 1): a Site stands on the middle of
// its slot and does not slide, so every piece keeps the place it was given.
// Estimate: 170 rules, 283 pieces before the track leaves any out; x 0.75 (the share the brief gives) = about 212 placed.
//   by mesh, before the 0.75: LampStandard 40, UrnBalustrade 40, Topiary 38, FlagCourt 36, PoolEdge 20, VictoryColumn 20, RopeLine 19, GateArch 17, Belvedere 12, ClockPavilion 12, CarriagePorch 6, WinterGarden 6, Arcade 5, HotelTower 5, Theatre 5, CrownTower 2
// Where, against FTDressL2.inl and FTPeopleL2.inl (read from the tables, none of it seen in the game):
//   LOW  22.3 m (21.2 to 23.4): behind the kerb wall and the ranch fence (19 m), in front of the decks of the cast (fronts 24.6 m). PoolEdge left s % 8 == 0,
//        right s % 8 == 4; RopeLine left s % 8 == 2, right s % 8 == 6.
//   NEAR 35.5 m, 18.5 m along either way from the slot (15 to 22 m): the sign towers and sign frames (32 to 54 m) slide 11.3 m at most and stand across the
//        road; the cast (27 to 30 m) and p3a (37 m) stand on the slot; p3f (to 30.2 m) and the half-slot scenes of p3a (34 m, on the left after the
//        s % 4 == 2 slots: nothing of this block on that end) stand 22.6 m along. s % 4 == 0: a lamp standard and an urn balustrade a side;
//        s % 4 == 2: topiary, and a flag court on the right.
//   DEEP s % 4 == 0, anchor 62 m, fronts 55 m: a house and two forecourt pieces 50 m out. Left out where the tables stand a set: left s % 10 == 0 (RoadStop)
//        and s % 10 == 2 (Jackpot), right s % 10 == 4 (motel courts) and slot 552 (the hero). Right slots 532, 540, 548 (the old road, 76 to 78 m): only the
//        winter garden and the arcade (backs 69.5 and 66.5 m). The porch, the hotel and the theatre reach 70.7 to 73.4 m at the back.
//        s % 4 == 2 (chapels, 52 to 90 m, slide over the middle of the slot, 14.8 m either way): only the two slender towers, 60 m out, 19.2 m along.
//   HERO THE CROWN TOWER, right, 92 m out, slots 504 and 552 (22.80 and 24.97 km): no RoadStop, Jackpot, trailer (s % 3 == 2), garage, fountain, board, coaster
//        or p3c row on the right of either; the motel court of slot 504 stands 60 to 66 m; the grand casinos (105 m and on) stand a slot away (503, 551).
//   NOT dodged: SM_Trailer (60 to 140 m, s % 6 == 2, chance 0.8) and SM_DustBlow (50 to 70 m, s % 8 == 2) can fall on a deep house; sage is everywhere.
// Smallest x of a TALL piece: 35.5 m (anchor 35.5, out 0). The two low pieces: 22.3 m.
// FIX ROUND (after Stage 1's review), by C:/ftwork/jobs/L2s1fix/d8_fix.py: the NEAR props are one-piece sets now, each placed where it
//   overlaps no older piece by 0.1 m or more and stands a metre clear of the block's other near props (first choice 15.5 m along, then 18.5, 12.5, 21, then the slot's other half): pass 3b's scenes
//   stand 28.25 m along from their slot, not 22.6, and reach back to 9 m from the next slot's middle. Pieces with no free place are left out.
//   The towers of 498R, 518R and 558R use the +19.2 sets: the -19.2 ones stood on the Jackpot set's dice pylon (60 m out, 26 m past 497, 517, 557).
#define S1D8(n) TEXT("/Game/World/Nevada/SM_S1D8_" n)
Set(TEXT("WldS1D8LampStandard1"), { { S1D8("LampStandard"), 18.5f, 0.0f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8UrnBalustrade1"), { { S1D8("UrnBalustrade"), -21.0f, 0.0f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8LampStandard2"), { { S1D8("LampStandard"), 9.5f, 0.0f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8Topiary1"), { { S1D8("Topiary"), 9.5f, 0.0f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8LampStandard3"), { { S1D8("LampStandard"), 21.0f, 0.0f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8UrnBalustrade2"), { { S1D8("UrnBalustrade"), 9.5f, 0.0f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8Topiary2"), { { S1D8("Topiary"), -21.0f, 0.0f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8FlagCourt1"), { { S1D8("FlagCourt"), 21.0f, 0.0f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8LowPoolL"), { { S1D8("PoolEdge"), -6.0f, 0.0f, 0.f, 0.f, 1.f } });	// anchor 22.3: 21.2 to 23.4 m out
Set(TEXT("WldS1D8LowPoolR"), { { S1D8("PoolEdge"), 6.0f, 0.0f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8LowRopeL"), { { S1D8("RopeLine"), -6.0f, 0.0f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8LowRopeR"), { { S1D8("RopeLine"), 6.0f, 0.0f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8Porch"), { { S1D8("CarriagePorch"), 0.0f, 4.0f, 0.f, 0.f, 1.f }, { S1D8("FlagCourt"), -18.5f, -12.0f, 0.f, 0.f, 1.f }, { S1D8("FlagCourt"), 18.5f, -12.0f, 0.f, 0.f, 1.f } });	// anchor 62: the house 55.0 to 73.4 m out, the forecourt pieces 50 m
Set(TEXT("WldS1D8Hotel"), { { S1D8("HotelTower"), 0.0f, 3.4f, 0.f, 0.f, 1.f }, { S1D8("VictoryColumn"), -18.5f, -12.0f, 0.f, 0.f, 1.f }, { S1D8("VictoryColumn"), 18.5f, -12.0f, 0.f, 0.f, 1.f } });	// 55.0 to 72.9
Set(TEXT("WldS1D8Garden"), { { S1D8("WinterGarden"), 0.0f, 1.0f, 0.f, 0.f, 1.f }, { S1D8("GateArch"), -18.5f, -12.0f, 0.f, 0.f, 1.f }, { S1D8("GateArch"), 18.5f, -12.0f, 0.f, 0.f, 1.f } });	// 55.0 to 69.5
Set(TEXT("WldS1D8Arcade"), { { S1D8("Arcade"), 0.0f, 0.0f, 0.f, 0.f, 1.f }, { S1D8("VictoryColumn"), -18.5f, -12.0f, 0.f, 0.f, 1.f }, { S1D8("GateArch"), 18.5f, -12.0f, 0.f, 0.f, 1.f } });	// 57.1 to 66.5
Set(TEXT("WldS1D8Theatre"), { { S1D8("Theatre"), 0.0f, 4.0f, 0.f, 0.f, 1.f }, { S1D8("FlagCourt"), -18.5f, -12.0f, 0.f, 0.f, 1.f }, { S1D8("VictoryColumn"), 18.5f, -12.0f, 0.f, 0.f, 1.f } });	// 55.0 to 70.7
Set(TEXT("WldS1D8ClockP"), { { S1D8("ClockPavilion"), 19.2f, 0.0f, 0.f, 0.f, 1.f } });	// anchor 60: 55.6 to 63.8 m out, 15.4 to 23.0 m along from the slot
Set(TEXT("WldS1D8ClockM"), { { S1D8("ClockPavilion"), -19.2f, 0.0f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8BelvP"), { { S1D8("Belvedere"), 19.2f, 0.0f, 0.f, 0.f, 1.f } });	// anchor 60: 55.3 to 64.1 m out, 15.1 to 23.3 m along
Set(TEXT("WldS1D8BelvM"), { { S1D8("Belvedere"), -19.2f, 0.0f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D8Crown"), { { S1D8("CrownTower"), 0.0f, 0.0f, 0.f, 0.f, 1.f } });	// anchor 92: 82.4 to 101.5 m out
Zone(TEXT("World s1d8, Vegas Approach"), 22.060487f, 22.096681f);	// slot 488, 22.08 km
	Site(TEXT("WldS1D8LampStandard1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Porch"), -1, 62.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Theatre"), 1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 22.150973f, 22.187167f);	// slot 490, 22.17 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ClockP"), 1, 60.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 22.241459f, 22.277653f);	// slot 492, 22.26 km
	Site(TEXT("WldS1D8LampStandard2"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard3"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Porch"), 1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 22.331945f, 22.368139f);	// slot 494, 22.35 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8FlagCourt1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BelvP"), -1, 60.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 22.422431f, 22.458625f);	// slot 496, 22.44 km
	Site(TEXT("WldS1D8LampStandard1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Hotel"), -1, 62.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Hotel"), 1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 22.512917f, 22.549111f);	// slot 498, 22.53 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ClockM"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BelvP"), 1, 60.0f, 1, 0, 1.f);	// fix round: the M set stood on the Jackpot set's dice pylon
Zone(TEXT("World s1d8, Vegas Approach"), 22.603403f, 22.639597f);	// slot 500, 22.62 km
	Site(TEXT("WldS1D8LampStandard1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Garden"), 1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 22.693889f, 22.730083f);	// slot 502, 22.71 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ClockP"), 1, 60.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 22.784375f, 22.820569f);	// slot 504, 22.80 km
	Site(TEXT("WldS1D8LampStandard1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Garden"), -1, 62.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Crown"), 1, 92.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 22.874861f, 22.911055f);	// slot 506, 22.89 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BelvP"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ClockM"), 1, 60.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 22.965347f, 23.001541f);	// slot 508, 22.98 km
	Site(TEXT("WldS1D8LampStandard2"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard3"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Arcade"), -1, 62.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Arcade"), 1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 23.055833f, 23.092027f);	// slot 510, 23.07 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8FlagCourt1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BelvM"), 1, 60.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 23.146319f, 23.182513f);	// slot 512, 23.16 km
	Site(TEXT("WldS1D8LampStandard1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Theatre"), 1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 23.236805f, 23.272999f);	// slot 514, 23.25 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ClockP"), -1, 60.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 23.327291f, 23.363485f);	// slot 516, 23.35 km
	Site(TEXT("WldS1D8LampStandard1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Theatre"), -1, 62.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Porch"), 1, 62.0f, 1, 0, -1.f);   // fix round: chance -1 = never placed (the rule keeps its place in the table): the older set's chapel stands 72 m out, 11 m along, in this footprint
Zone(TEXT("World s1d8, Vegas Approach"), 23.417777f, 23.453971f);	// slot 518, 23.44 km
	Site(TEXT("WldS1D8Topiary1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BelvP"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ClockP"), 1, 60.0f, 1, 0, 1.f);	// fix round: the M set stood on the Jackpot set's dice pylon
Zone(TEXT("World s1d8, Vegas Approach"), 23.508263f, 23.544457f);	// slot 520, 23.53 km
	Site(TEXT("WldS1D8LampStandard1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Hotel"), 1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 23.598749f, 23.634943f);	// slot 522, 23.62 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BelvM"), 1, 60.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 23.689235f, 23.725429f);	// slot 524, 23.71 km
	Site(TEXT("WldS1D8LampStandard2"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard3"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Porch"), -1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 23.779721f, 23.815915f);	// slot 526, 23.80 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8FlagCourt1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ClockP"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BelvP"), 1, 60.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 23.870207f, 23.906401f);	// slot 528, 23.89 km
	Site(TEXT("WldS1D8LampStandard1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Hotel"), -1, 62.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Garden"), 1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 23.960693f, 23.996887f);	// slot 530, 23.98 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeL"), -1, 22.3f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 24.051179f, 24.087373f);	// slot 532, 24.07 km
	Site(TEXT("WldS1D8LampStandard1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Garden"), 1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 24.141665f, 24.177859f);	// slot 534, 24.16 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ClockM"), -1, 60.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 24.232151f, 24.268345f);	// slot 536, 24.25 km
	Site(TEXT("WldS1D8LampStandard1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Garden"), -1, 62.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Arcade"), 1, 62.0f, 1, 0, -1.f);   // fix round: chance -1 = never placed (the rule keeps its place in the table): an older trailer stands 65 m out in this footprint
Zone(TEXT("World s1d8, Vegas Approach"), 24.322637f, 24.358831f);	// slot 538, 24.34 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BelvM"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ClockP"), 1, 60.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 24.413123f, 24.449317f);	// slot 540, 24.43 km
	Site(TEXT("WldS1D8LampStandard2"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard3"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Arcade"), 1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 24.503609f, 24.539803f);	// slot 542, 24.52 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8FlagCourt1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BelvP"), 1, 60.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 24.594095f, 24.630289f);	// slot 544, 24.61 km
	Site(TEXT("WldS1D8LampStandard1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Arcade"), -1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 24.684581f, 24.720775f);	// slot 546, 24.70 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ClockM"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BelvM"), 1, 60.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 24.775067f, 24.811261f);	// slot 548, 24.79 km
	Site(TEXT("WldS1D8LampStandard1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Theatre"), -1, 62.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Garden"), 1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 24.865553f, 24.901747f);	// slot 550, 24.88 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ClockP"), 1, 60.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 24.956039f, 24.992233f);	// slot 552, 24.97 km
	Site(TEXT("WldS1D8LampStandard1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Crown"), 1, 92.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 25.046525f, 25.082719f);	// slot 554, 25.06 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BelvP"), -1, 60.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 25.137011f, 25.173205f);	// slot 556, 25.16 km
	Site(TEXT("WldS1D8LampStandard2"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard3"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Porch"), -1, 62.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Theatre"), 1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 25.227497f, 25.263691f);	// slot 558, 25.25 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8FlagCourt1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8ClockM"), -1, 60.0f, 1, 0, 1.f);
	Site(TEXT("WldS1D8BelvP"), 1, 60.0f, 1, 0, 1.f);	// fix round: the M set stood on the Jackpot set's dice pylon
Zone(TEXT("World s1d8, Vegas Approach"), 25.317983f, 25.354177f);	// slot 560, 25.34 km
	Site(TEXT("WldS1D8LampStandard1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolL"), -1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Porch"), 1, 62.0f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 25.408469f, 25.444663f);	// slot 562, 25.43 km
	Site(TEXT("WldS1D8Topiary1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Topiary1"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowRopeL"), -1, 22.3f, 1, 0, 1.f);
Zone(TEXT("World s1d8, Vegas Approach"), 25.498955f, 25.535149f);	// slot 564, 25.52 km
	Site(TEXT("WldS1D8LampStandard1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade1"), -1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LampStandard3"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8UrnBalustrade2"), 1, 35.5f, 1, 0, 1.f);
	Site(TEXT("WldS1D8LowPoolR"), 1, 22.3f, 1, 0, 1.f);
	Site(TEXT("WldS1D8Hotel"), -1, 62.0f, 1, 0, 1.f);
#undef S1D8
// ---- END s1d8 ----
}

{
// ---- BEGIN s1plants (the planting of the eight districts, ft_world_nevada_s1plants.py) ----
// Written by C:/ftwork/jobs/L2s1plants/make_inl.py. 21 meshes, all Scatter, chance 1, slots s % 4 == 3 only (every 4 phase 1; every 8 phase 5 =
// s % 8 == 3; every 8 phase 1 = s % 8 == 7). Counted slot by slot: 616 rule-slots (a rule on both sides counts 2) = 616 pieces before the track
// leaves any out; x 0.75 (the share the track leaves out) = about 462 placed. Rule-slots by district: s1d1 53, s1d2 74, s1d3 63, s1d4 93, s1d5 73, s1d6 88, s1d7 56, s1d8 116.
// The most of one mesh, before / after x 0.75: Century 49 / 37, RimPines 46 / 34, FanPalm 45 / 34, Creosote 44 / 33.
// Bands (m from the centre line): low 20.5-23.0 (behind the fence and the kerb at 19, before the near rows that stand from 24.6); TALL 31.5-34.5
// (between the near rows and the second rows at 36-37); 36.3-38.8 (d1 and d7); 40.5-45.0 (40.5-42.3 and 43.2-45.0 in Vegas approach);
// 48.0-52.0 (Jackpot Junction and the City Lights, behind the sign towers at 39-46); 56.5-60.0 (56.5-58.5 in Syndicate range, 59.0-60.0 in
// Vegas approach). In the rock cuts (the zone plan's list, 30 m of margin each end) and all through Red Rock Pass only the rim piece stands,
// 40.5-43.5 m out: both sides in a cut, the INSIDE of each wall bend (right of the first, left of the second). Beside the shallow banked bends
// (4.3, 4.4, 7.8, 8.0 km) a zone of its own holds only the rules that start 40 m out or more.
// FIX ROUND (after Stage 1's review):
//   - the rim pines stood in pass 3's needle (44 to 50 m out) wherever both had the slot: their rules now leave the needle's slots and sides.
//   - Barrel and Rabbitbrush (green metre-sized domes) no longer stand 20.5 to 23 m out: Bunchgrass stands there (the repair pickup is green).
//   - every rule with a piece 0.3 m or more inside an older piece was tried 1.5 to 12 m nearer and further (C:/ftwork/jobs/L2s1fix/opt.py,
//     box against box, the same random draws) and keeps the band with the least overlap: the lines marked "fix round: was".
#define S1PLANTS(n) TEXT("/Game/World/Nevada/SM_S1PLANTS_" n)
Zone(TEXT("World s1plants, State Line and the Dry Wash"), 0.0000f, 1.1085f);   // slots 0 to 24
Zone(TEXT("World s1plants, State Line and the Dry Wash"), 0.1176f, 0.6967f);   // fix round: this rule alone, slots 3 to 15, clear of the older pieces
	Scatter(S1PLANTS("Yucca"), 31.5f, 34.5f, 4, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
Zone(TEXT("World s1plants, State Line and the Dry Wash"), 0.0000f, 1.1085f);   // (the zone goes on)
	Scatter(S1PLANTS("DesertWillow"), 44.3f, 46.8f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);   // fix round: was 36.3 to 38.8 m
	Scatter(S1PLANTS("Cottonwood"), 36.3f, 38.8f, 8, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Bunchgrass"), 20.5f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, -1, 180.0f);
	Scatter(S1PLANTS("Wildflowers"), 43.5f, 48.0f, 8, 5, 1.f, 1.3f, 0.10f, -1, 20.0f);   // fix round: was 40.5 to 45.0 m
	Scatter(S1PLANTS("Cholla"), 39.5f, 42.5f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);   // fix round: was 31.5 to 34.5 m
	Scatter(S1PLANTS("DesertWillow"), 33.0f, 36.0f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);   // fix round: was 31.5 to 34.5 m
	Scatter(S1PLANTS("Wildflowers"), 40.5f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, 1, 20.0f);
	Scatter(S1PLANTS("Beavertail"), 20.5f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, 1, 180.0f);
	Scatter(S1PLANTS("Yucca"), 40.5f, 45.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, State Line and the Dry Wash, a rock cut"), 1.1537f, 1.3799f);   // slots 26 to 30
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 4, 1, 1.f, 1.0f, 0.06f, 1, 12.0f);   // fix round: right only, pass 3's needle stands on every one of these slots on the left
Zone(TEXT("World s1plants, State Line and the Dry Wash"), 1.3799f, 1.5609f);   // slots 31 to 34
	Scatter(S1PLANTS("Cottonwood"), 37.5f, 42.0f, 4, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);   // fix round: was 40.5 to 45.0 m
	Scatter(S1PLANTS("DesertWillow"), 36.3f, 38.8f, 8, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Cholla"), 40.5f, 45.0f, 8, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Bunchgrass"), 20.5f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, 1, 180.0f);
Zone(TEXT("World s1plants, State Line and the Dry Wash, a rock cut"), 1.5609f, 2.1038f);   // slots 35 to 46
	Scatter(S1PLANTS("RimPines"), 36.0f, 39.0f, 4, 1, 1.f, 1.0f, 0.06f, 1, 12.0f);   // fix round: right only, pass 3's needle stands on every one of these slots on the left   // fix round: was 40.5 to 43.5 m
Zone(TEXT("World s1plants, State Line and the Dry Wash"), 2.1038f, 2.4205f);   // slots 47 to 53
	// fix round: rule kept for its place in the table, no slot of it was free
	Scatter(S1PLANTS("Cottonwood"), 40.5f, 45.0f, 4, 1, -1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Cottonwood"), 36.3f, 38.8f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("DesertWillow"), 36.3f, 38.8f, 8, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Yucca"), 40.5f, 45.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Cholla"), 40.5f, 45.0f, 8, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Beavertail"), 20.5f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, -1, 180.0f);
	Scatter(S1PLANTS("Bunchgrass"), 20.5f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, 1, 180.0f);
Zone(TEXT("World s1plants, Snake Eyes Country and Joshua Flats"), 2.4205f, 3.8230f);   // slots 54 to 84
	Scatter(S1PLANTS("Mesquite"), 40.5f, 45.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
Zone(TEXT("World s1plants, Snake Eyes Country and Joshua Flats"), 3.1942f, 3.5923f);   // fix round: this rule alone, slots 71 to 79, clear of the older pieces
	Scatter(S1PLANTS("Ocotillo"), 52.5f, 57.0f, 8, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);   // fix round: was 40.5 to 45.0 m
Zone(TEXT("World s1plants, Snake Eyes Country and Joshua Flats"), 2.4703f, 3.2304f);   // fix round: this rule alone, slots 55 to 71, clear of the older pieces
	Scatter(S1PLANTS("Mesquite"), 56.5f, 60.0f, 16, 9, 1.f, 1.0f, 0.12f, -1, 180.0f);
Zone(TEXT("World s1plants, Snake Eyes Country and Joshua Flats"), 2.4205f, 3.8230f);   // (the zone goes on)
	Scatter(S1PLANTS("Creosote"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Bunchgrass"), 20.5f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, -1, 180.0f);
	Scatter(S1PLANTS("Beavertail"), 20.5f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, -1, 180.0f);
	Scatter(S1PLANTS("Ocotillo"), 43.5f, 46.5f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);   // fix round: was 31.5 to 34.5 m
	Scatter(S1PLANTS("Yucca"), 41.5f, 44.5f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);   // fix round: was 31.5 to 34.5 m
	Scatter(S1PLANTS("Creosote"), 40.5f, 45.0f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Cholla"), 40.5f, 45.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Mesquite"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Creosote"), 50.5f, 54.0f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);   // fix round: was 56.5 to 60.0 m
Zone(TEXT("World s1plants, Snake Eyes Country and Joshua Flats"), 3.8230f, 4.2302f);   // slots 85 to 93
Zone(TEXT("World s1plants, Snake Eyes Country and Joshua Flats"), 3.9180f, 3.9542f);   // fix round: this rule alone, slots 87 to 87, clear of the older pieces
	Scatter(S1PLANTS("Yucca"), 39.5f, 42.5f, 4, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);   // fix round: was 31.5 to 34.5 m
Zone(TEXT("World s1plants, Snake Eyes Country and Joshua Flats"), 3.8230f, 4.2302f);   // (the zone goes on)
	Scatter(S1PLANTS("Cholla"), 31.5f, 34.5f, 4, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Yucca"), 40.5f, 45.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Cholla"), 40.5f, 45.0f, 8, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Yucca"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Cholla"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
Zone(TEXT("World s1plants, Snake Eyes Country and Joshua Flats, a banked bend"), 4.2302f, 4.4564f);   // slots 94 to 98
	Scatter(S1PLANTS("Cholla"), 46.5f, 51.0f, 8, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);   // fix round: was 40.5 to 45.0 m
	Scatter(S1PLANTS("Yucca"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, Snake Eyes Country and Joshua Flats"), 4.4564f, 5.1351f);   // slots 99 to 113
Zone(TEXT("World s1plants, Snake Eyes Country and Joshua Flats"), 4.8229f, 5.0401f);   // fix round: this rule alone, slots 107 to 111, clear of the older pieces
	Scatter(S1PLANTS("Yucca"), 41.5f, 44.5f, 4, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);   // fix round: was 31.5 to 34.5 m
Zone(TEXT("World s1plants, Snake Eyes Country and Joshua Flats"), 4.4564f, 5.1351f);   // (the zone goes on)
	Scatter(S1PLANTS("Cholla"), 39.5f, 42.5f, 4, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);   // fix round: was 31.5 to 34.5 m
	Scatter(S1PLANTS("Yucca"), 37.5f, 42.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);   // fix round: was 40.5 to 45.0 m
	Scatter(S1PLANTS("Cholla"), 48.5f, 53.0f, 8, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);   // fix round: was 40.5 to 45.0 m
	Scatter(S1PLANTS("Yucca"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Cholla"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
	// fix round: no rim pines here, pass 3's needle stands on every odd slot of this bend's inside
Zone(TEXT("World s1plants, Red Rock Pass and Last Chance Gas, a rock cut"), 5.4970f, 5.6328f);   // slots 122 to 124
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 4, 1, 1.f, 1.0f, 0.06f, 0, 12.0f);
Zone(TEXT("World s1plants, Red Rock Pass and Last Chance Gas, the second wall bend"), 5.6328f, 6.0852f);   // slots 125 to 134
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 4, 1, 1.f, 1.0f, 0.06f, -1, 12.0f);
Zone(TEXT("World s1plants, Red Rock Pass and Last Chance Gas, a rock cut"), 6.0852f, 6.3114f);   // slots 135 to 139
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 4, 1, 1.f, 1.0f, 0.06f, -1, 12.0f);   // fix round: left only, the needle stands on the odd slots on the right
Zone(TEXT("World s1plants, Red Rock Pass and Last Chance Gas"), 6.3566f, 7.7139f);   // slots 141 to 170
	Scatter(S1PLANTS("Century"), 41.5f, 44.5f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);   // fix round: was 31.5 to 34.5 m
	Scatter(S1PLANTS("Cholla"), 31.5f, 34.5f, 8, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Cottonwood"), 48.5f, 53.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);   // fix round: was 40.5 to 45.0 m
	Scatter(S1PLANTS("Mesquite"), 50.5f, 55.0f, 8, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);   // fix round: was 40.5 to 45.0 m
	Scatter(S1PLANTS("Pinyon"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("DesertWillow"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Bunchgrass"), 20.5f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, -1, 180.0f);
	Scatter(S1PLANTS("Yucca"), 31.5f, 34.5f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, Red Rock Pass and Last Chance Gas"), 7.1755f, 7.5737f);   // fix round: this rule alone, slots 159 to 167, clear of the older pieces
	Scatter(S1PLANTS("Century"), 33.0f, 36.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);   // fix round: was 31.5 to 34.5 m
Zone(TEXT("World s1plants, Red Rock Pass and Last Chance Gas"), 6.3566f, 7.7139f);   // (the zone goes on)
	Scatter(S1PLANTS("DesertWillow"), 40.5f, 45.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Pinyon"), 42.0f, 46.5f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);   // fix round: was 40.5 to 45.0 m
Zone(TEXT("World s1plants, Red Rock Pass and Last Chance Gas"), 6.8136f, 7.5737f);   // fix round: this rule alone, slots 151 to 167, clear of the older pieces
	Scatter(S1PLANTS("Cottonwood"), 56.5f, 60.0f, 16, 9, 1.f, 1.0f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, Red Rock Pass and Last Chance Gas"), 6.3566f, 7.7139f);   // (the zone goes on)
	Scatter(S1PLANTS("Mesquite"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, Red Rock Pass and Last Chance Gas"), 6.6326f, 7.0308f);   // fix round: this rule alone, slots 147 to 155, clear of the older pieces
	Scatter(S1PLANTS("Bunchgrass"), 20.5f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, 1, 180.0f);
Zone(TEXT("World s1plants, Red Rock Pass and Last Chance Gas, a banked bend"), 7.7139f, 7.7592f);   // slots 171 to 171
	Scatter(S1PLANTS("Cottonwood"), 42.0f, 46.5f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);   // fix round: was 40.5 to 45.0 m
	Scatter(S1PLANTS("Pinyon"), 56.5f, 60.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("DesertWillow"), 40.5f, 45.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Mesquite"), 44.5f, 48.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);   // fix round: was 56.5 to 60.0 m
Zone(TEXT("World s1plants, Jackpot Junction and the City Lights, a banked bend"), 7.7592f, 8.0759f);   // slots 172 to 178
	Scatter(S1PLANTS("DatePalms"), 48.0f, 52.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
	Scatter(S1PLANTS("FanPalm"), 48.0f, 52.0f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Century"), 56.5f, 60.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Jackpot Junction and the City Lights"), 8.0759f, 10.7000f);   // slots 179 to 236
	Scatter(S1PLANTS("FanPalm"), 48.0f, 52.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("DatePalms"), 48.0f, 52.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
	Scatter(S1PLANTS("Century"), 31.5f, 34.5f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("JuniperFlames"), 31.5f, 34.5f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
	Scatter(S1PLANTS("BloomHedge"), 20.5f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("DatePalms"), 48.5f, 52.0f, 8, 5, 1.f, 1.0f, 0.05f, -1, 5.0f);   // fix round: was 56.5 to 60.0 m
	Scatter(S1PLANTS("DatePalms"), 48.0f, 52.0f, 8, 5, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("FanPalm"), 48.0f, 52.0f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("JuniperFlames"), 33.0f, 36.0f, 8, 5, 1.f, 1.0f, 0.05f, 1, 5.0f);   // fix round: was 31.5 to 34.5 m
	Scatter(S1PLANTS("Century"), 31.5f, 34.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("BloomHedge"), 20.5f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("Century"), 44.5f, 48.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);   // fix round: was 56.5 to 60.0 m
Zone(TEXT("World s1plants, Salt Flats"), 10.7000f, 14.5004f);   // slots 237 to 320
	Scatter(S1PLANTS("Tamarisk"), 48.5f, 53.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);   // fix round: was 40.5 to 45.0 m
	Scatter(S1PLANTS("Saltbush"), 52.5f, 57.0f, 8, 1, 1.f, 1.3f, 0.10f, -1, 180.0f);   // fix round: was 40.5 to 45.0 m
	Scatter(S1PLANTS("Bunchgrass"), 21.5f, 24.5f, 8, 5, 1.f, 1.2f, 0.10f, -1, 25.0f);   // fix round: was 31.5 to 34.5 m
	Scatter(S1PLANTS("Creosote"), 45.0f, 49.5f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);   // fix round: was 40.5 to 45.0 m
Zone(TEXT("World s1plants, Salt Flats"), 11.8808f, 14.0887f);   // fix round: this rule alone, slots 263 to 311, clear of the older pieces
	Scatter(S1PLANTS("Bunchgrass"), 45.0f, 49.5f, 16, 9, 1.f, 1.3f, 0.10f, 1, 25.0f);   // fix round: was 40.5 to 45.0 m
Zone(TEXT("World s1plants, Salt Flats"), 12.4237f, 14.2696f);   // fix round: this rule alone, slots 275 to 315, clear of the older pieces
	Scatter(S1PLANTS("Saltbush"), 56.5f, 60.0f, 8, 5, 1.f, 1.4f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Salt Flats"), 11.5189f, 11.9170f);   // fix round: this rule alone, slots 255 to 263, clear of the older pieces
	Scatter(S1PLANTS("Tamarisk"), 48.5f, 52.0f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);   // fix round: was 56.5 to 60.0 m
Zone(TEXT("World s1plants, Syndicate Range"), 14.5004f, 18.4818f);   // slots 321 to 408
	Scatter(S1PLANTS("Ocotillo"), 40.5f, 45.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
Zone(TEXT("World s1plants, Syndicate Range"), 15.8622f, 18.4320f);   // fix round: this rule alone, slots 351 to 407, clear of the older pieces
	Scatter(S1PLANTS("Creosote"), 40.5f, 45.0f, 8, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
Zone(TEXT("World s1plants, Syndicate Range"), 14.5004f, 18.4818f);   // (the zone goes on)
	Scatter(S1PLANTS("Rabbitbrush"), 52.0f, 54.0f, 8, 5, 1.f, 1.3f, 0.10f, -1, 180.0f);   // fix round: was 56.5 to 58.5 m
Zone(TEXT("World s1plants, Syndicate Range"), 15.8622f, 16.9842f);   // fix round: this rule alone, slots 351 to 375, clear of the older pieces
	Scatter(S1PLANTS("Bunchgrass"), 46.5f, 48.5f, 8, 1, 1.f, 1.3f, 0.10f, -1, 25.0f);   // fix round: was 56.5 to 58.5 m
Zone(TEXT("World s1plants, Syndicate Range"), 14.5004f, 18.4818f);   // (the zone goes on)
	Scatter(S1PLANTS("Creosote"), 40.5f, 45.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, Syndicate Range"), 15.1383f, 18.4320f);   // fix round: this rule alone, slots 335 to 407, clear of the older pieces
	Scatter(S1PLANTS("Ocotillo"), 42.0f, 46.5f, 24, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);   // fix round: was 40.5 to 45.0 m
Zone(TEXT("World s1plants, Syndicate Range"), 14.7764f, 18.0701f);   // fix round: this rule alone, slots 327 to 399, clear of the older pieces
	Scatter(S1PLANTS("Saltbush"), 44.5f, 46.5f, 8, 1, 1.f, 1.3f, 0.10f, 1, 180.0f);   // fix round: was 56.5 to 58.5 m
Zone(TEXT("World s1plants, Syndicate Range"), 14.5954f, 16.4413f);   // fix round: this rule alone, slots 323 to 363, clear of the older pieces
	Scatter(S1PLANTS("Rabbitbrush"), 44.5f, 46.5f, 8, 5, 1.f, 1.3f, 0.10f, 1, 180.0f);   // fix round: was 56.5 to 58.5 m
Zone(TEXT("World s1plants, Red Rock Return, a rock cut"), 18.5270f, 19.3866f);   // slots 410 to 428
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 12, 5, 1.f, 1.0f, 0.06f, -1, 12.0f);   // fix round: left s % 12 == 7 and 11, right s % 12 == 3 and 7: off the needle's slots (left s % 3 == 0, right s % 3 == 2)
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 12, 1, 1.f, 1.0f, 0.06f, -1, 12.0f);
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 12, 9, 1.f, 1.0f, 0.06f, 1, 12.0f);
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 12, 5, 1.f, 1.0f, 0.06f, 1, 12.0f);
Zone(TEXT("World s1plants, Red Rock Return"), 19.3866f, 19.8843f);   // slots 429 to 439
	Scatter(S1PLANTS("Pinyon"), 36.3f, 38.8f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("DesertWillow"), 36.3f, 38.8f, 8, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Barrel"), 40.5f, 45.0f, 8, 5, 1.f, 1.3f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Rabbitbrush"), 40.5f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Wildflowers"), 55.0f, 58.5f, 4, 1, 1.f, 1.4f, 0.10f, -1, 20.0f);   // fix round: was 56.5 to 60.0 m
	Scatter(S1PLANTS("DesertWillow"), 36.3f, 38.8f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Pinyon"), 36.3f, 38.8f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Beavertail"), 40.5f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Wildflowers"), 40.5f, 45.0f, 8, 5, 1.f, 1.3f, 0.10f, 1, 20.0f);
Zone(TEXT("World s1plants, Red Rock Return, a rock cut"), 19.8843f, 20.7439f);   // slots 440 to 458
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 12, 5, 1.f, 1.0f, 0.06f, -1, 12.0f);   // fix round: left s % 12 == 7 and 11, right s % 12 == 3 and 7: off the needle's slots (left s % 3 == 0, right s % 3 == 2)
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 12, 1, 1.f, 1.0f, 0.06f, -1, 12.0f);
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 12, 9, 1.f, 1.0f, 0.06f, 1, 12.0f);
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 12, 5, 1.f, 1.0f, 0.06f, 1, 12.0f);
Zone(TEXT("World s1plants, Red Rock Return"), 20.7439f, 21.2868f);   // slots 459 to 470
	Scatter(S1PLANTS("Pinyon"), 36.3f, 38.8f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("DesertWillow"), 36.3f, 38.8f, 8, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Barrel"), 40.5f, 45.0f, 8, 5, 1.f, 1.3f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Rabbitbrush"), 40.5f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Wildflowers"), 55.0f, 58.5f, 4, 1, 1.f, 1.4f, 0.10f, -1, 20.0f);   // fix round: was 56.5 to 60.0 m
	Scatter(S1PLANTS("DesertWillow"), 36.3f, 38.8f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Pinyon"), 36.3f, 38.8f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("Beavertail"), 40.5f, 45.0f, 8, 1, 1.f, 1.3f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Wildflowers"), 40.5f, 45.0f, 8, 5, 1.f, 1.3f, 0.10f, 1, 20.0f);
Zone(TEXT("World s1plants, Red Rock Return, a rock cut"), 21.2868f, 22.0107f);   // slots 471 to 486
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 12, 5, 1.f, 1.0f, 0.06f, -1, 12.0f);   // fix round: left s % 12 == 7 and 11, right s % 12 == 3 and 7: off the needle's slots (left s % 3 == 0, right s % 3 == 2)
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 12, 1, 1.f, 1.0f, 0.06f, -1, 12.0f);
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 12, 9, 1.f, 1.0f, 0.06f, 1, 12.0f);
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 12, 5, 1.f, 1.0f, 0.06f, 1, 12.0f);
Zone(TEXT("World s1plants, Vegas Approach, a rock cut"), 22.0107f, 22.0560f);   // slots 487 to 487
	Scatter(S1PLANTS("RimPines"), 40.5f, 43.5f, 4, 1, 1.f, 1.0f, 0.06f, 0, 12.0f);
Zone(TEXT("World s1plants, Vegas Approach"), 22.0560f, 99.f);   // slots 488 to 565
	Scatter(S1PLANTS("Century"), 42.0f, 43.8f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);   // fix round: was 40.5 to 42.3 m
	Scatter(S1PLANTS("JuniperFlames"), 40.5f, 42.3f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
	Scatter(S1PLANTS("FanPalm"), 44.7f, 46.5f, 8, 5, 1.f, 1.0f, 0.12f, -1, 180.0f);   // fix round: was 43.2 to 45.0 m
	Scatter(S1PLANTS("DatePalms"), 43.2f, 45.0f, 8, 1, 1.f, 1.0f, 0.05f, -1, 5.0f);
	Scatter(S1PLANTS("BloomHedge"), 20.5f, 23.0f, 8, 1, 1.f, 1.0f, 0.00f, -1, 4.0f);
	Scatter(S1PLANTS("JuniperFlames"), 40.5f, 42.3f, 8, 5, 1.f, 1.0f, 0.05f, 1, 5.0f);
	Scatter(S1PLANTS("BloomHedge"), 42.0f, 43.8f, 8, 1, 1.f, 1.2f, 0.00f, 1, 4.0f);   // fix round: was 40.5 to 42.3 m
	Scatter(S1PLANTS("DatePalms"), 43.2f, 45.0f, 8, 5, 1.f, 1.0f, 0.05f, 1, 5.0f);
Zone(TEXT("World s1plants, Vegas Approach"), 22.3772f, 24.5850f);   // fix round: this rule alone, slots 495 to 543, clear of the older pieces
	Scatter(S1PLANTS("FanPalm"), 31.2f, 33.0f, 8, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);   // fix round: was 43.2 to 45.0 m
Zone(TEXT("World s1plants, Vegas Approach"), 22.0560f, 99.f);   // (the zone goes on)
	Scatter(S1PLANTS("Century"), 41.5f, 44.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);   // fix round: was 31.5 to 34.5 m
	Scatter(S1PLANTS("BloomHedge"), 20.5f, 23.0f, 8, 5, 1.f, 1.0f, 0.00f, 1, 4.0f);
	Scatter(S1PLANTS("FanPalm"), 59.0f, 60.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
#undef S1PLANTS
// ---- END s1plants ----
}

{
// ---- BEGIN s1animals (the animals of the eight districts, ft_world_nevada_s1animals.py) ----
// 19 meshes, /Game/World/Nevada/SM_S1ANIMALS_*. Estimate (every rule counted slot by slot against its zone; Scatter x 0.75, Far x 1):
// about 298 placed. By mesh: BurroShelf 31, Jackrabbits 29, PelicanLine 21, Peacocks 20, FoxDen 20, BrinePool 17, QuailWall 15, MuleDeer 15, CraneDance 15, Owls 15, SheepFlock 14, Roadrunners 14, PrairieDogs 13, SwanLagoon 13, FlamingoPool 12, HenCoop 11, Tortoises 11, DuckTank 9, Dovecote 6.
// Before the 0.75 the rules name 385 places; the most of one mesh is BurroShelf, 41 places (about 31 placed).
// Bands and slots (the brief's): 20 to 45 m in s%8==5 (only two rules: Roadrunners 41.5-44.5 m in Joshua Flats, Peacocks 42.5-44.5 m on the
// Vegas approach, both TALL and both in the one gap the two tables leave there, 41 to 46 m); 46 to 120 m in s%4==1 and, from 61 m, s%4==3.
// The sky (Far, a negative drop): PelicanLine 95 to 145 m out, Owls 100 to 135 m out; both travel ALONG the road and never circle.
// No mesh stands both inside and outside 70 m. Inside (58 to 67 m): DuckTank, HenCoop, Tortoises, Jackrabbits, Roadrunners, QuailWall,
// Peacocks, SwanLagoon, FlamingoPool. Outside (76 m and more): BurroShelf, SheepFlock, MuleDeer, BrinePool, CraneDance, FoxDen, PrairieDogs, Dovecote.
// Nothing in the rock cuts but the owls (the land beyond the rim cannot be seen); nothing in Red Rock Pass; no sky piece in the dry wash,
// Red Rock Pass, Last Chance Gas, Jackpot Junction or the city lights (the helicopter scenes; the zone plan). Every rule was checked slot by
// slot against both tables (C:/ftwork/jobs/L2s1animals/occ.py and block.py): no piece meets a Site, a row of the population or a Scatter rule
// narrower than 30 m; sage, scrub patches, dust, Joshua trees and the wide rock scatters were not dodged.
// FIX ROUND (after Stage 1's review): pieces stood on pass 3c's trails (pack trains, stagecoaches, land yachts, the hand car, cyclists:
// lanes 97 m long, 76 to 128 m out) and on two older sets. The Red Rock Return gaps' deer and burros now stand 75 to 77 m out, short of the
// trails; the Last Chance Gas roadrunners are on the right; every other rule with a piece 0.3 m or more inside an older piece (a travelling
// piece's whole lane counted) was tried nearer and further by C:/ftwork/jobs/L2s1fix/opt.py and keeps the band with the least overlap
// (lines marked "fix round: was"); a mesh that stood inside 70 m still does, one that stood outside still does.
#define S1ANIMALS(n) TEXT("/Game/World/Nevada/SM_S1ANIMALS_" n)
Zone(TEXT("World s1animals, State Line"), 0.10f, 1.15f);
	Scatter(S1ANIMALS("SheepFlock"), 82.f, 110.f, 8, 3, 1.f, 1.4f, 0.06f, -1, 5.f);   // a flock with its dog and its shepherd on a swell, left: s%8==5
	Scatter(S1ANIMALS("BurroShelf"), 82.f, 110.f, 8, 7, 1.f, 1.4f, 0.06f, 1, 4.f);   // wild burros on a rock shelf, right: s%8==1
Zone(TEXT("World s1animals, State Line"), 1.1130f, 1.1492f);   // fix round: this rule alone, slots 25 to 25, clear of the older pieces
	Scatter(S1ANIMALS("DuckTank"), 58.5f, 66.f, 8, 7, 1.f, 1.3f, 0.05f, 0, 20.f);   // a stock tank with ducks, behind the ranch fence: s%8==1
Zone(TEXT("World s1animals, State Line"), 0.10f, 1.15f);   // (the zone goes on)
	Scatter(S1ANIMALS("HenCoop"), 59.f, 66.f, 8, 3, 1.f, 1.3f, 0.05f, 0, 8.f);   // a hen house in its run: s%8==5
	Scatter(S1ANIMALS("Tortoises"), 62.f, 67.f, 8, 1, 1.f, 1.4f, 0.08f, -1, 20.f);   // tortoises on slabs: s%8==7, the side the StateLine set leaves free
	Scatter(S1ANIMALS("Tortoises"), 62.f, 67.f, 8, 5, 1.f, 1.4f, 0.08f, 1, 20.f);   // s%8==3
Zone(TEXT("World s1animals, State Line"), 0.6605f, 1.0587f);   // fix round: this rule alone, slots 15 to 23, clear of the older pieces
	Scatter(S1ANIMALS("PrairieDogs"), 84.f, 106.f, 4, 1, 1.f, 1.5f, 0.08f, 0, 20.f);   // a prairie dog town on the open flat: s%4==3, beyond the StateLine set (to 72 m)
Zone(TEXT("World s1animals, the Dry Wash, its open east end"), 2.06f, 2.43f);
	Scatter(S1ANIMALS("BurroShelf"), 78.f, 106.f, 4, 3, 1.f, 1.4f, 0.06f, 0, 4.f);   // burros where the wash opens (the rock banks end 70 m out)
Zone(TEXT("World s1animals, Snake Eyes Country"), 2.43f, 3.83f);
Zone(TEXT("World s1animals, Snake Eyes Country"), 3.1037f, 3.1399f);   // fix round: this rule alone, slots 69 to 69, clear of the older pieces
	Scatter(S1ANIMALS("BurroShelf"), 78.f, 108.f, 8, 3, 1.f, 1.4f, 0.06f, 0, 4.f);   // s%8==5
Zone(TEXT("World s1animals, Snake Eyes Country"), 2.43f, 3.83f);   // (the zone goes on)
	Scatter(S1ANIMALS("HenCoop"), 59.f, 66.f, 8, 7, 1.f, 1.3f, 0.05f, 0, 8.f);   // hens behind the sheds: s%8==1
Zone(TEXT("World s1animals, Snake Eyes Country"), 2.7417f, 2.7779f);   // fix round: this rule alone, slots 61 to 61, clear of the older pieces
	Scatter(S1ANIMALS("DuckTank"), 58.5f, 66.f, 8, 3, 1.f, 1.3f, 0.05f, 0, 20.f);   // s%8==5
Zone(TEXT("World s1animals, Snake Eyes Country"), 3.1942f, 3.5923f);   // fix round: this rule alone, slots 71 to 79, clear of the older pieces
	Scatter(S1ANIMALS("Jackrabbits"), 62.f, 67.f, 8, 1, 1.f, 1.4f, 0.08f, 0, 30.f);   // jackrabbits on a boulder: s%8==7 (the dens and yards stand on even slots)
Zone(TEXT("World s1animals, Joshua Flats, by the road (west of the jump)"), 3.83f, 4.40f);
	Scatter(S1ANIMALS("Roadrunners"), 41.5f, 44.5f, 8, 3, 1.f, 1.f, 0.f, -1, 3.f);   // TALL (the ledge is 2.1 m): roadrunners sprinting a ledge, left, in the gap 41 to 46 m: s%8==5
Zone(TEXT("World s1animals, Joshua Flats, by the road (west of the jump)"), 4.1895f, 4.2257f);   // fix round: this rule alone, slots 93 to 93, clear of the older pieces
	Scatter(S1ANIMALS("Roadrunners"), 58.f, 66.f, 8, 3, 1.f, 1.3f, 0.05f, 1, 3.f);   // and on the right
Zone(TEXT("World s1animals, Joshua Flats, by the road (east of the jump)"), 4.80f, 5.14f);
	Scatter(S1ANIMALS("Roadrunners"), 41.5f, 44.5f, 8, 3, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(S1ANIMALS("Roadrunners"), 58.f, 66.f, 8, 3, 1.f, 1.3f, 0.05f, 1, 3.f);
Zone(TEXT("World s1animals, Joshua Flats"), 3.83f, 5.14f);
	Scatter(S1ANIMALS("Roadrunners"), 58.f, 66.f, 8, 7, 1.f, 1.3f, 0.05f, 0, 3.f);   // s%8==1
Zone(TEXT("World s1animals, Joshua Flats"), 3.9180f, 4.3162f);   // fix round: this rule alone, slots 87 to 95, clear of the older pieces
	Scatter(S1ANIMALS("Jackrabbits"), 64.f, 67.f, 8, 1, 1.f, 1.4f, 0.08f, 0, 30.f);   // s%8==7
Zone(TEXT("World s1animals, Joshua Flats"), 4.0085f, 5.1306f);   // fix round: this rule alone, slots 89 to 113, clear of the older pieces
	Scatter(S1ANIMALS("BurroShelf"), 72.0f, 96.0f, 24, 7, 1.f, 1.4f, 0.06f, 0, 4.f);   // short of the power line (115 m right)   // fix round: was 78. to 102. m
Zone(TEXT("World s1animals, Last Chance Gas"), 6.31f, 7.75f);
	Scatter(S1ANIMALS("QuailWall"), 60.f, 66.f, 8, 3, 1.f, 1.3f, 0.05f, 0, 6.f);   // a quail covey on a garden wall: s%8==5
	Scatter(S1ANIMALS("Tortoises"), 61.f, 66.f, 8, 7, 1.f, 1.4f, 0.08f, 0, 20.f);   // s%8==1
Zone(TEXT("World s1animals, Last Chance Gas"), 6.5421f, 6.9403f);   // fix round: this rule alone, slots 145 to 153, clear of the older pieces
	Scatter(S1ANIMALS("MuleDeer"), 88.0f, 110.0f, 8, 7, 1.f, 1.4f, 0.06f, 0, 5.f);   // mule deer on a rise, beyond the stops and courts: s%8==1   // fix round: was 94. to 116. m
Zone(TEXT("World s1animals, Last Chance Gas"), 7.0851f, 7.4832f);   // fix round: this rule alone, slots 157 to 165, clear of the older pieces
	Scatter(S1ANIMALS("BurroShelf"), 89.5f, 111.5f, 8, 3, 1.f, 1.4f, 0.06f, 0, 4.f);   // burros at their trough: s%8==5   // fix round: was 94. to 116. m
Zone(TEXT("World s1animals, Last Chance Gas"), 6.31f, 7.75f);   // (the zone goes on)
	Scatter(S1ANIMALS("Roadrunners"), 62.f, 67.f, 8, 1, 1.f, 1.3f, 0.05f, 1, 3.f);   // s%8==7, RIGHT (fix round: on the left the RoadStop set's gas station stood in the ledge at slots 143 and 151)
Zone(TEXT("World s1animals, Jackpot Junction and the City Lights, left"), 7.75f, 9.60f);
	Scatter(S1ANIMALS("Peacocks"), 63.5f, 68.5f, 8, 3, 1.f, 1.2f, 0.04f, -1, 5.f);   // peacocks on a balustrade: s%8==5   // fix round: was 62. to 67. m
Zone(TEXT("World s1animals, Jackpot Junction and the City Lights, left"), 7.9899f, 8.7500f);   // fix round: this rule alone, slots 177 to 193, clear of the older pieces
	Scatter(S1ANIMALS("SwanLagoon"), 62.0f, 64.5f, 8, 7, 1.f, 1.f, 0.f, -1, 4.f);   // swans on a raised lagoon: s%8==1   // fix round: was 63.5 to 66. m
Zone(TEXT("World s1animals, the City Lights, left"), 9.75f, 10.45f);
	Scatter(S1ANIMALS("Peacocks"), 62.f, 67.f, 8, 3, 1.f, 1.2f, 0.04f, -1, 5.f);
	Scatter(S1ANIMALS("SwanLagoon"), 66.5f, 69.0f, 8, 7, 1.f, 1.f, 0.f, -1, 4.f);   // fix round: was 63.5 to 66. m
Zone(TEXT("World s1animals, Jackpot Junction and the City Lights, quail"), 7.75f, 10.00f);
Zone(TEXT("World s1animals, Jackpot Junction and the City Lights, quail"), 8.2614f, 9.7453f);   // fix round: this rule alone, slots 183 to 215, clear of the older pieces
	Scatter(S1ANIMALS("QuailWall"), 62.f, 67.f, 8, 1, 1.f, 1.3f, 0.05f, -1, 6.f);   // s%8==7
Zone(TEXT("World s1animals, Jackpot Junction, flamingos"), 7.75f, 8.70f);
	Scatter(S1ANIMALS("FlamingoPool"), 64.f, 66.5f, 8, 5, 1.f, 1.1f, 0.f, 1, 10.f);   // flamingos in a raised pool, right: s%8==3
Zone(TEXT("World s1animals, the City Lights, flamingos"), 9.10f, 10.40f);
	Scatter(S1ANIMALS("FlamingoPool"), 64.f, 66.5f, 8, 5, 1.f, 1.1f, 0.f, 1, 10.f);
Zone(TEXT("World s1animals, Salt Flats"), 10.69f, 14.50f);
Zone(TEXT("World s1animals, Salt Flats"), 12.5142f, 13.9982f);   // fix round: this rule alone, slots 277 to 309, clear of the older pieces
	Scatter(S1ANIMALS("Jackrabbits"), 60.5f, 65.5f, 8, 3, 1.f, 1.4f, 0.08f, 0, 30.f);   // s%8==5   // fix round: was 62. to 67. m
Zone(TEXT("World s1animals, Salt Flats"), 11.4284f, 12.1885f);   // fix round: this rule alone, slots 253 to 269, clear of the older pieces
	Scatter(S1ANIMALS("BrinePool"), 82.0f, 108.0f, 8, 3, 1.f, 1.4f, 0.06f, -1, 25.f);   // a brine pool with avocets and pelicans: s%8==5   // fix round: was 90. to 116. m
Zone(TEXT("World s1animals, Salt Flats"), 10.7045f, 12.5504f);   // fix round: this rule alone, slots 237 to 277, clear of the older pieces
	Scatter(S1ANIMALS("BrinePool"), 98.0f, 110.0f, 8, 3, 1.f, 1.4f, 0.06f, 1, 25.f);   // right: short of the power line (towers 120 m out)   // fix round: was 92. to 104. m
Zone(TEXT("World s1animals, Salt Flats"), 10.8855f, 12.3694f);   // fix round: this rule alone, slots 241 to 273, clear of the older pieces
	Scatter(S1ANIMALS("CraneDance"), 92.f, 116.f, 8, 7, 1.f, 1.4f, 0.06f, -1, 25.f);   // sandhill cranes dancing on the salt: s%8==1
Zone(TEXT("World s1animals, Salt Flats"), 10.69f, 14.50f);   // (the zone goes on)
	Scatter(S1ANIMALS("CraneDance"), 95.0f, 107.0f, 8, 7, 1.f, 1.4f, 0.06f, 1, 25.f);   // fix round: was 92. to 104. m
Zone(TEXT("World s1animals, Salt Flats"), 10.7950f, 13.3648f);   // fix round: this rule alone, slots 239 to 295, clear of the older pieces
	Scatter(S1ANIMALS("FoxDen"), 109.0f, 115.0f, 8, 1, 1.f, 1.5f, 0.06f, 1, 6.f);   // a kit fox den: s%8==7, right, short of the power line   // fix round: was 99. to 105. m
Zone(TEXT("World s1animals, Salt Flats, left, west of the monument"), 10.69f, 11.80f);
	Scatter(S1ANIMALS("FoxDen"), 98.0f, 113.0f, 8, 1, 1.f, 1.5f, 0.06f, -1, 6.f);   // left: beyond the railway (80 m)   // fix round: was 101. to 116. m
	Far(S1ANIMALS("PelicanLine"), 95.f, 135.f, 12, 5, 1.f, 1.6f, 0.06f, -1, -30.f, 6.f);   // white pelicans in a line along the road, 34 m up, left (the power line is on the right): their path ends 80 m each way
Zone(TEXT("World s1animals, Salt Flats, left, east of the monument"), 12.05f, 14.50f);
	Scatter(S1ANIMALS("FoxDen"), 101.f, 116.f, 8, 1, 1.f, 1.5f, 0.06f, -1, 6.f);
	Far(S1ANIMALS("PelicanLine"), 95.f, 135.f, 12, 5, 1.f, 1.6f, 0.06f, -1, -30.f, 6.f);
Zone(TEXT("World s1animals, Syndicate Range"), 14.50f, 18.50f);
Zone(TEXT("World s1animals, Syndicate Range"), 14.6859f, 15.8079f);   // fix round: this rule alone, slots 325 to 349, clear of the older pieces
	Scatter(S1ANIMALS("SheepFlock"), 92.0f, 116.0f, 8, 3, 1.f, 1.4f, 0.06f, 1, 5.f);   // a flock out on the range, right: s%8==5   // fix round: was 80. to 104. m
Zone(TEXT("World s1animals, Syndicate Range"), 14.50f, 18.50f);   // (the zone goes on)
	Far(S1ANIMALS("PelicanLine"), 105.f, 145.f, 12, 2, 1.f, 1.6f, 0.06f, 1, -44.f, 6.f);   // 48 m up, over the hoodoos and the radar posts (their heights were not read)
	Far(S1ANIMALS("PelicanLine"), 105.f, 145.f, 16, 8, 1.f, 1.6f, 0.06f, -1, -44.f, 6.f);
Zone(TEXT("World s1animals, Syndicate Range, its west"), 14.50f, 17.20f);
Zone(TEXT("World s1animals, Syndicate Range, its west"), 14.5049f, 15.2650f);   // fix round: this rule alone, slots 321 to 337, clear of the older pieces
	Scatter(S1ANIMALS("FoxDen"), 86.0f, 106.0f, 16, 15, 1.f, 1.5f, 0.06f, -1, 6.f);   // s%16==1, left   // fix round: was 80. to 100. m
Zone(TEXT("World s1animals, Syndicate Range, its east"), 17.20f, 18.50f);
	Scatter(S1ANIMALS("FoxDen"), 80.f, 100.f, 16, 15, 1.f, 1.5f, 0.06f, 1, 6.f);   // s%16==1, right
Zone(TEXT("World s1animals, Syndicate Range, the flock on the left"), 16.00f, 18.50f);
Zone(TEXT("World s1animals, Syndicate Range, the flock on the left"), 16.1337f, 16.8937f);   // fix round: this rule alone, slots 357 to 373, clear of the older pieces
	Scatter(S1ANIMALS("SheepFlock"), 96.0f, 120.0f, 16, 11, 1.f, 1.4f, 0.06f, -1, 5.f);   // s%16==5, left (clear of the wild horses and the hand car)   // fix round: was 80. to 104. m
Zone(TEXT("World s1animals, Syndicate Range, prairie dogs"), 14.50f, 18.30f);
Zone(TEXT("World s1animals, Syndicate Range, prairie dogs"), 16.9480f, 17.7081f);   // fix round: this rule alone, slots 375 to 391, clear of the older pieces
	Scatter(S1ANIMALS("PrairieDogs"), 86.0f, 106.0f, 16, 9, 1.f, 1.5f, 0.08f, 1, 20.f);   // s%16==7, right   // fix round: was 80. to 100. m
Zone(TEXT("World s1animals, Red Rock Return, the first gap"), 19.36f, 19.92f);
	Scatter(S1ANIMALS("MuleDeer"), 72.0f, 74.0f, 4, 3, 1.f, 1.4f, 0.06f, 0, 5.f);   // fix round: was 76 to 112 m, on the stagecoach and pack-train trails (86.7 to 105.3 m); beyond them stand the second rock wall and the mesa stubs (110 m on)   // s%4==1   // fix round: was 75. to 77. m
	Scatter(S1ANIMALS("BurroShelf"), 73.5f, 75.5f, 4, 1, 1.f, 1.4f, 0.06f, 0, 4.f);   // fix round: was 76 to 112 m   // pack burros: s%4==3   // fix round: was 75. to 77. m
	Scatter(S1ANIMALS("QuailWall"), 62.5f, 67.f, 4, 3, 1.f, 1.3f, 0.05f, -1, 6.f);
Zone(TEXT("World s1animals, Red Rock Return, the second gap"), 20.72f, 21.30f);
	Scatter(S1ANIMALS("MuleDeer"), 75.f, 77.f, 4, 3, 1.f, 1.4f, 0.06f, 0, 5.f);   // fix round: was 76 to 112 m, on the stagecoach and pack-train trails (86.7 to 105.3 m); beyond them stand the second rock wall and the mesa stubs (110 m on)
	Scatter(S1ANIMALS("BurroShelf"), 75.f, 77.f, 4, 1, 1.f, 1.4f, 0.06f, 0, 4.f);   // fix round: was 76 to 112 m
	Scatter(S1ANIMALS("QuailWall"), 62.5f, 67.f, 4, 3, 1.f, 1.3f, 0.05f, -1, 6.f);
Zone(TEXT("World s1animals, Red Rock Return, the sky, right"), 18.50f, 22.00f);
	Far(S1ANIMALS("Owls"), 100.f, 135.f, 8, 3, 1.f, 2.5f, 0.06f, 1, -62.f, 5.f);   // pale barn owls quartering along the road 63 to 73 m up, over the 15 m rim and the 36 m canyon walls: their path ends 75 m each way
Zone(TEXT("World s1animals, Red Rock Return, the sky, left, first cut"), 18.55f, 19.60f);
	Far(S1ANIMALS("Owls"), 100.f, 135.f, 8, 7, 1.f, 2.5f, 0.06f, -1, -62.f, 5.f);   // left only clear of the two monuments (126 m tall, 108 m out at 19.9 and 21.3 km)
Zone(TEXT("World s1animals, Red Rock Return, the sky, left, second cut"), 20.20f, 21.00f);
	Far(S1ANIMALS("Owls"), 100.f, 135.f, 8, 7, 1.f, 2.5f, 0.06f, -1, -62.f, 5.f);
Zone(TEXT("World s1animals, Vegas Approach"), 22.03f, 25.60f);
	Scatter(S1ANIMALS("Peacocks"), 42.5f, 44.5f, 16, 3, 1.f, 1.f, 0.f, -1, 4.f);   // TALL (the rail is 1.8 m, the fan 4.2 m): in the gap 41 to 46 m, left: s%16==13
	Scatter(S1ANIMALS("Peacocks"), 42.5f, 44.5f, 16, 11, 1.f, 1.f, 0.f, 1, 4.f);   // right: s%16==5
	Scatter(S1ANIMALS("Peacocks"), 62.f, 67.f, 8, 1, 1.f, 1.2f, 0.04f, -1, 5.f);   // s%8==7
	Scatter(S1ANIMALS("SwanLagoon"), 63.f, 66.f, 8, 7, 1.f, 1.f, 0.f, -1, 4.f);   // s%8==1
Zone(TEXT("World s1animals, Vegas Approach"), 23.0106f, 25.5804f);   // fix round: this rule alone, slots 509 to 565, clear of the older pieces
	Scatter(S1ANIMALS("FlamingoPool"), 62.0f, 64.5f, 8, 3, 1.f, 1.1f, 0.f, -1, 10.f);   // s%8==5   // fix round: was 63.5 to 66. m
Zone(TEXT("World s1animals, Vegas Approach, doves, before the wheel"), 22.03f, 23.90f);
Zone(TEXT("World s1animals, Vegas Approach, doves, before the wheel"), 22.5582f, 23.6802f);   // fix round: this rule alone, slots 499 to 523, clear of the older pieces
	Scatter(S1ANIMALS("Dovecote"), 101.0f, 107.0f, 8, 5, 1.f, 1.2f, 0.f, -1, 180.f);   // a dovecote, its doves wheeling 10 m round it at the widest: s%8==3, left   // fix round: was 93. to 99. m
Zone(TEXT("World s1animals, Vegas Approach, doves, after the wheel"), 24.20f, 25.00f);
Zone(TEXT("World s1animals, Vegas Approach, doves, after the wheel"), 24.7298f, 24.7660f);   // fix round: this rule alone, slots 547 to 547, clear of the older pieces
	Scatter(S1ANIMALS("Dovecote"), 88.5f, 94.5f, 8, 5, 1.f, 1.2f, 0.f, -1, 180.f);   // fix round: was 93. to 99. m
Zone(TEXT("World s1animals, Vegas Approach, doves, the last"), 25.20f, 25.60f);
	Scatter(S1ANIMALS("Dovecote"), 93.f, 99.f, 8, 5, 1.f, 1.2f, 0.f, -1, 180.f);
#undef S1ANIMALS
// ---- END s1animals ----
}

{
// ---- BEGIN s1people (the people pass 3 left out, ft_world_nevada_s1people.py) ----
// 23 meshes, all printed by the one make run (C:/ftwork/logs/L2s1people_make.log: built 23, failed 0). All TALL. Each brings what its
// people stand on (a stepped stand, a veranda, a freight wagon, a scrap deck, an adobe roof, a stalk deck, a catwalk, a bunker roof,
// galleries, a marble stair: floors 2.35 to 6.6 m up; in the rock cuts a 23 m column of the cut's own rock, people at twice life size).
// Steady glow 0.6 on five meshes (PorchBand's lamp, Bellhops' starburst, Catwalk's strip, BunkerRoof's slit, GrandStair's globes); every
// rule that places them stands 40.6 m out or more. Nothing blinks, travels or flies. No Lit, no Bank, no Far. NOT COMPILED, NOT SEEN.
// Slots: s % 8 == 1 only (every 8, phase 7), each rule in a one-slot Zone of its own, so a set stands on the slot and side chosen for it.
// What decided the places (read from both tables with C:/ftwork/jobs/L2s1people/occ.py and sites.py; nothing seen in the game):
//   open land: a row of six 4.5 m pieces, 5.2 m apart along the road, every second one 1.2 m further out, anchor 40.6 m (fronts 38.8 m),
//     on the slots and sides where no row of the people table stands on the slot between 33.5 and 47.5 m and the dressing has no sign
//     tower, sign frame or FarRunners rule there. Beside the banked bends at 4.4 and 8.0 km and behind the neon signs of Snake Eyes
//     country: anchor 42 m (fronts 40.2 m). Syndicate range: 43 m (its rock banks reach 40 m; the pieces' floors are 3.1 to 4.2 m up).
//     Vegas approach, right side only: 43.2 m, behind p3b's half-slot scenes (their backs 41.1 m). The start line (slot 1, both sides): 36 m.
//     p3b's scenes stand half a slot on (22.6 m along, 37 m out, to 17.4 m long as placed): where the slot before has one on this side the
//     row is moved on (sets ...P: -10 to +16 m along); where this slot and the one before both have one, five pieces between them (...B).
//   rock cuts (1.85, 6.20, 18.87 to 21.76 km): two columns 10 m either side of the slot, 38.5 m out, behind the first cast's rim piece
//     (33.1 m, 16.5 m column), on the sides that have no 31 m wall (CutLineB) on this slot.
//   the jump at 1.5 km: one high stand, slot 33 right, 42.5 m (front 40.6 m: outside the 40 m the track keeps clear).
// Left out: both wall bends of Red Rock Pass (outside: the 76 m cliff 35.5 m out; nothing may stand 17 to 40 m beside a banked bend),
// the corkscrews, the loops, the jumps at 4.6, 7.3 and 9.7 km, the City Lights (sign towers 39 to 46 m on every slot), the left of the
// Vegas approach (a stand at 44 m on every one of these slots), and every slot-side that already has a row 33.5 to 47.5 m out.
// Estimate (Site: parts x 0.75): 37 sets of six + 5 sets of five = 247, 11 rim sets of two = 22, the jump stand 1: 270 parts x 0.75 =
// about 203 placed. The most of one mesh: 20 places (Catwalk, BunkerRoof, Surveyors: about 15 placed). The smallest x min of a TALL mesh:
// 36.0 m (the start line; its pieces' fronts 34.2 m).
// FIX ROUND (after Stage 1's review): pass 3b's scenes stand 28.25 m along from their slot (22.6 x their 1.25 scale), not 22.6 as
// read above. Eight rows that stood 0.3 to 2.3 m inside one (or inside a pass 3d water tower) were moved out or in, each tested box against
// box against every older piece: 145L 169L 249L 281L 433L 465L to 42 m, 297L to 39.5 m, the rim pair of 457L to 44 m.
#define S1PEOPLE(n) TEXT("/Game/World/Nevada/SM_S1PEOPLE_" n)
Set(TEXT("WldS1PEOPLEStartAN"), {
	{ S1PEOPLE("SendOff"), -16.6f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("MailWagon"), -11.4f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("SendOff"), -6.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PorchRail"), -1.0f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("SendOff"), 4.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("MailWagon"), 9.4f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEStartBN"), {
	{ S1PEOPLE("SendOff"), -16.6f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("MailWagon"), -11.4f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("SendOff"), -6.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PorchRail"), -1.0f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("SendOff"), 4.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("MailWagon"), 9.4f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLELineAB"), {
	{ S1PEOPLE("PorchRail"), -10.4f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("MailWagon"), -5.2f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PorchRail"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("SendOff"), 5.2f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PorchRail"), 10.4f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLELineBN"), {
	{ S1PEOPLE("PorchRail"), -16.6f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("SendOff"), -11.4f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("MailWagon"), -6.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PorchRail"), -1.0f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("MailWagon"), 4.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PorchRail"), 9.4f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLESnakeAN"), {
	{ S1PEOPLE("DrumTable"), -16.6f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PorchBand"), -11.4f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Linemen"), -6.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("DrumTable"), -1.0f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PorchBand"), 4.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Linemen"), 9.4f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLESnakeBN"), {
	{ S1PEOPLE("Linemen"), -16.6f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("DrumTable"), -11.4f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PorchBand"), -6.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Linemen"), -1.0f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("DrumTable"), 4.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PorchBand"), 9.4f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLESnakeBB"), {
	{ S1PEOPLE("Linemen"), -10.4f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("DrumTable"), -5.2f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PorchBand"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Linemen"), 5.2f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("DrumTable"), 10.4f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEGasAP"), {
	{ S1PEOPLE("AdobeTerrace"), -10.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PotStall"), -4.8f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("AdobeTerrace"), 0.4f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PotStall"), 5.6f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("AdobeTerrace"), 10.8f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PotStall"), 16.0f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEGasBP"), {
	{ S1PEOPLE("PotStall"), -10.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("AdobeTerrace"), -4.8f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PotStall"), 0.4f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("AdobeTerrace"), 5.6f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PotStall"), 10.8f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("AdobeTerrace"), 16.0f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEJackpotAB"), {
	{ S1PEOPLE("Bellhops"), -10.4f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("SnapShooters"), -5.2f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Bellhops"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("SnapShooters"), 5.2f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Bellhops"), 10.4f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEJackpotBP"), {
	{ S1PEOPLE("SnapShooters"), -10.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Bellhops"), -4.8f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("SnapShooters"), 0.4f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Bellhops"), 5.6f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("SnapShooters"), 10.8f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Bellhops"), 16.0f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEJackpotAN"), {
	{ S1PEOPLE("Bellhops"), -16.6f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("SnapShooters"), -11.4f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Bellhops"), -6.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("SnapShooters"), -1.0f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Bellhops"), 4.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("SnapShooters"), 9.4f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLESaltAN"), {
	{ S1PEOPLE("TimingStand"), -16.6f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("UmbrellaWagon"), -11.4f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PitDeck"), -6.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("TimingStand"), -1.0f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("UmbrellaWagon"), 4.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PitDeck"), 9.4f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLESaltBN"), {
	{ S1PEOPLE("PitDeck"), -16.6f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("TimingStand"), -11.4f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("UmbrellaWagon"), -6.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PitDeck"), -1.0f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("TimingStand"), 4.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("UmbrellaWagon"), 9.4f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLESaltAB"), {
	{ S1PEOPLE("TimingStand"), -10.4f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("UmbrellaWagon"), -5.2f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PitDeck"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("TimingStand"), 5.2f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("UmbrellaWagon"), 10.4f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLESaltBB"), {
	{ S1PEOPLE("PitDeck"), -10.4f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("TimingStand"), -5.2f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("UmbrellaWagon"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PitDeck"), 5.2f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("TimingStand"), 10.4f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLERangeAN"), {
	{ S1PEOPLE("Catwalk"), -16.6f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("BunkerRoof"), -11.4f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Surveyors"), -6.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Catwalk"), -1.0f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("BunkerRoof"), 4.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Surveyors"), 9.4f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLERangeBN"), {
	{ S1PEOPLE("Surveyors"), -16.6f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Catwalk"), -11.4f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("BunkerRoof"), -6.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Surveyors"), -1.0f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Catwalk"), 4.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("BunkerRoof"), 9.4f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLERangeAP"), {
	{ S1PEOPLE("Catwalk"), -10.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("BunkerRoof"), -4.8f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Surveyors"), 0.4f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Catwalk"), 5.6f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("BunkerRoof"), 10.8f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Surveyors"), 16.0f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLERangeBP"), {
	{ S1PEOPLE("Surveyors"), -10.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Catwalk"), -4.8f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("BunkerRoof"), 0.4f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Surveyors"), 5.6f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Catwalk"), 10.8f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("BunkerRoof"), 16.0f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEMineAN"), {
	{ S1PEOPLE("MineGallery"), -16.6f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("HoistDeck"), -11.4f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("MineGallery"), -6.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("HoistDeck"), -1.0f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("MineGallery"), 4.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("HoistDeck"), 9.4f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEMineBN"), {
	{ S1PEOPLE("HoistDeck"), -16.6f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("MineGallery"), -11.4f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("HoistDeck"), -6.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("MineGallery"), -1.0f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("HoistDeck"), 4.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("MineGallery"), 9.4f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEMineBP"), {
	{ S1PEOPLE("HoistDeck"), -10.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("MineGallery"), -4.8f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("HoistDeck"), 0.4f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("MineGallery"), 5.6f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("HoistDeck"), 10.8f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("MineGallery"), 16.0f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEVegasAN"), {
	{ S1PEOPLE("GrandStair"), -16.6f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Quartet"), -11.4f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("GrandStair"), -6.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Quartet"), -1.0f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("GrandStair"), 4.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Quartet"), 9.4f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEVegasBN"), {
	{ S1PEOPLE("Quartet"), -16.6f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("GrandStair"), -11.4f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Quartet"), -6.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("GrandStair"), -1.0f, 1.2f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Quartet"), 4.2f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("GrandStair"), 9.4f, 1.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLERim"), {
	{ S1PEOPLE("RimFlags"), -10.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("RimParasols"), 10.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLEJump"), {
	{ S1PEOPLE("JumpStand"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1people, the start line, slot 1"), 0.0271f, 0.0633f);
	Site(TEXT("WldS1PEOPLEStartAN"), -1, 36.0f, 8, 7, 1.f);    // 0.05 km, left, 6 in the set
Zone(TEXT("World s1people, the start line, slot 1"), 0.0271f, 0.0633f);
	Site(TEXT("WldS1PEOPLEStartBN"), 1, 36.0f, 8, 7, 1.f);    // 0.05 km, right, 6 in the set
Zone(TEXT("World s1people, state line, slot 9"), 0.3891f, 0.4253f);
	Site(TEXT("WldS1PEOPLELineAB"), -1, 40.6f, 8, 7, 1.f);    // 0.41 km, left, 5 in the set
Zone(TEXT("World s1people, state line, slot 9"), 0.3891f, 0.4253f);
	Site(TEXT("WldS1PEOPLELineBN"), 1, 40.6f, 8, 7, 1.f);    // 0.41 km, right, 6 in the set
Zone(TEXT("World s1people, the jump at 1.5 km, slot 33"), 1.4749f, 1.5111f);
	Site(TEXT("WldS1PEOPLEJump"), 1, 42.5f, 8, 7, 1.f);    // 1.49 km, right, 1 in the set
Zone(TEXT("World s1people, a rock cut, over the rim, slot 41"), 1.8369f, 1.8731f);
	Site(TEXT("WldS1PEOPLERim"), 1, 38.5f, 8, 7, 1.f);    // 1.85 km, right, 2 in the set
Zone(TEXT("World s1people, Snake Eyes country, slot 57"), 2.5608f, 2.5969f);
	Site(TEXT("WldS1PEOPLESnakeAN"), 1, 42.0f, 8, 7, 1.f);    // 2.58 km, right, 6 in the set
Zone(TEXT("World s1people, Snake Eyes country, slot 65"), 2.9227f, 2.9589f);
	Site(TEXT("WldS1PEOPLESnakeBN"), 1, 42.0f, 8, 7, 1.f);    // 2.94 km, right, 6 in the set
Zone(TEXT("World s1people, Snake Eyes country, slot 81"), 3.6466f, 3.6828f);
	Site(TEXT("WldS1PEOPLESnakeAN"), 1, 42.0f, 8, 7, 1.f);    // 3.66 km, right, 6 in the set
Zone(TEXT("World s1people, Joshua Flats, slot 97"), 4.3705f, 4.4067f);
	Site(TEXT("WldS1PEOPLESnakeBB"), -1, 42.0f, 8, 7, 1.f);    // 4.39 km, left, 5 in the set
Zone(TEXT("World s1people, a rock cut, over the rim, slot 137"), 6.1802f, 6.2164f);
	Site(TEXT("WldS1PEOPLERim"), 1, 38.5f, 8, 7, 1.f);    // 6.20 km, right, 2 in the set
Zone(TEXT("World s1people, Last Chance Gas, slot 145"), 6.5421f, 6.5783f);
	Site(TEXT("WldS1PEOPLEGasAP"), -1, 42.0f, 8, 7, 1.f);    // 6.56 km, left, 6 in the set; fix round: was 40.6 m, in an older piece
Zone(TEXT("World s1people, Last Chance Gas, slot 169"), 7.628f, 7.6642f);
	Site(TEXT("WldS1PEOPLEGasBP"), -1, 42.0f, 8, 7, 1.f);    // 7.65 km, left, 6 in the set; fix round: was 40.6 m, in an older piece
Zone(TEXT("World s1people, Jackpot Junction, slot 177"), 7.9899f, 8.0261f);
	Site(TEXT("WldS1PEOPLEJackpotAB"), 1, 42.0f, 8, 7, 1.f);    // 8.01 km, right, 5 in the set
Zone(TEXT("World s1people, Jackpot Junction, slot 193"), 8.7138f, 8.75f);
	Site(TEXT("WldS1PEOPLEJackpotBP"), -1, 40.6f, 8, 7, 1.f);    // 8.73 km, left, 6 in the set
Zone(TEXT("World s1people, Jackpot Junction, slot 193"), 8.7138f, 8.75f);
	Site(TEXT("WldS1PEOPLEJackpotAN"), 1, 40.6f, 8, 7, 1.f);    // 8.73 km, right, 6 in the set
Zone(TEXT("World s1people, salt flats, slot 249"), 11.2474f, 11.2836f);
	Site(TEXT("WldS1PEOPLESaltAN"), -1, 42.0f, 8, 7, 1.f);    // 11.27 km, left, 6 in the set; fix round: was 40.6 m, in an older piece
Zone(TEXT("World s1people, salt flats, slot 249"), 11.2474f, 11.2836f);
	Site(TEXT("WldS1PEOPLESaltBN"), 1, 40.6f, 8, 7, 1.f);    // 11.27 km, right, 6 in the set
Zone(TEXT("World s1people, salt flats, slot 257"), 11.6094f, 11.6455f);
	Site(TEXT("WldS1PEOPLESaltAB"), -1, 40.6f, 8, 7, 1.f);    // 11.63 km, left, 5 in the set
Zone(TEXT("World s1people, salt flats, slot 265"), 11.9713f, 12.0075f);
	Site(TEXT("WldS1PEOPLESaltBN"), -1, 40.6f, 8, 7, 1.f);    // 11.99 km, left, 6 in the set
Zone(TEXT("World s1people, salt flats, slot 281"), 12.6952f, 12.7314f);
	Site(TEXT("WldS1PEOPLESaltAN"), -1, 42.0f, 8, 7, 1.f);    // 12.71 km, left, 6 in the set; fix round: was 40.6 m, in an older piece
Zone(TEXT("World s1people, salt flats, slot 297"), 13.4191f, 13.4553f);
	Site(TEXT("WldS1PEOPLESaltBB"), -1, 39.5f, 8, 7, 1.f);    // 13.44 km, left, 5 in the set; fix round: was 40.6 m, in an older piece
Zone(TEXT("World s1people, salt flats, slot 305"), 13.781f, 13.8172f);
	Site(TEXT("WldS1PEOPLESaltAN"), -1, 40.6f, 8, 7, 1.f);    // 13.80 km, left, 6 in the set
Zone(TEXT("World s1people, salt flats, slot 305"), 13.781f, 13.8172f);
	Site(TEXT("WldS1PEOPLESaltBN"), 1, 40.6f, 8, 7, 1.f);    // 13.80 km, right, 6 in the set
Zone(TEXT("World s1people, Syndicate range, slot 321"), 14.5049f, 14.5411f);
	Site(TEXT("WldS1PEOPLERangeAN"), 1, 43.0f, 8, 7, 1.f);    // 14.52 km, right, 6 in the set
Zone(TEXT("World s1people, Syndicate range, slot 329"), 14.8668f, 14.903f);
	Site(TEXT("WldS1PEOPLERangeBN"), 1, 43.0f, 8, 7, 1.f);    // 14.88 km, right, 6 in the set
Zone(TEXT("World s1people, Syndicate range, slot 337"), 15.2288f, 15.265f);
	Site(TEXT("WldS1PEOPLERangeAP"), -1, 43.0f, 8, 7, 1.f);    // 15.25 km, left, 6 in the set
Zone(TEXT("World s1people, Syndicate range, slot 345"), 15.5907f, 15.6269f);
	Site(TEXT("WldS1PEOPLERangeBN"), 1, 43.0f, 8, 7, 1.f);    // 15.61 km, right, 6 in the set
Zone(TEXT("World s1people, Syndicate range, slot 353"), 15.9527f, 15.9889f);
	Site(TEXT("WldS1PEOPLERangeAN"), 1, 43.0f, 8, 7, 1.f);    // 15.97 km, right, 6 in the set
Zone(TEXT("World s1people, Syndicate range, slot 361"), 16.3146f, 16.3508f);
	Site(TEXT("WldS1PEOPLERangeBP"), -1, 43.0f, 8, 7, 1.f);    // 16.33 km, left, 6 in the set
Zone(TEXT("World s1people, Syndicate range, slot 369"), 16.6766f, 16.7128f);
	Site(TEXT("WldS1PEOPLERangeAN"), 1, 43.0f, 8, 7, 1.f);    // 16.69 km, right, 6 in the set
Zone(TEXT("World s1people, Syndicate range, slot 377"), 17.0385f, 17.0747f);
	Site(TEXT("WldS1PEOPLERangeBN"), 1, 43.0f, 8, 7, 1.f);    // 17.06 km, right, 6 in the set
Zone(TEXT("World s1people, Syndicate range, slot 385"), 17.4005f, 17.4367f);
	Site(TEXT("WldS1PEOPLERangeAP"), -1, 43.0f, 8, 7, 1.f);    // 17.42 km, left, 6 in the set
Zone(TEXT("World s1people, Syndicate range, slot 401"), 18.1243f, 18.1605f);
	Site(TEXT("WldS1PEOPLERangeBN"), 1, 43.0f, 8, 7, 1.f);    // 18.14 km, right, 6 in the set
Zone(TEXT("World s1people, red rock return, before the cut, slot 409"), 18.4863f, 18.5225f);
	Site(TEXT("WldS1PEOPLEMineAN"), -1, 40.6f, 8, 7, 1.f);    // 18.50 km, left, 6 in the set
Zone(TEXT("World s1people, red rock return, before the cut, slot 409"), 18.4863f, 18.5225f);
	Site(TEXT("WldS1PEOPLEMineBN"), 1, 40.6f, 8, 7, 1.f);    // 18.50 km, right, 6 in the set
Zone(TEXT("World s1people, a rock cut, over the rim, slot 417"), 18.8482f, 18.8844f);
	Site(TEXT("WldS1PEOPLERim"), -1, 38.5f, 8, 7, 1.f);    // 18.87 km, left, 2 in the set
Zone(TEXT("World s1people, a rock cut, over the rim, slot 417"), 18.8482f, 18.8844f);
	Site(TEXT("WldS1PEOPLERim"), 1, 38.5f, 8, 7, 1.f);    // 18.87 km, right, 2 in the set
Zone(TEXT("World s1people, a rock cut, over the rim, slot 425"), 19.2102f, 19.2464f);
	Site(TEXT("WldS1PEOPLERim"), 1, 38.5f, 8, 7, 1.f);    // 19.23 km, right, 2 in the set
Zone(TEXT("World s1people, red rock return, first gap, slot 433"), 19.5721f, 19.6083f);
	Site(TEXT("WldS1PEOPLEMineAN"), -1, 42.0f, 8, 7, 1.f);    // 19.59 km, left, 6 in the set; fix round: was 40.6 m, in an older piece
Zone(TEXT("World s1people, red rock return, first gap, slot 433"), 19.5721f, 19.6083f);
	Site(TEXT("WldS1PEOPLEMineBP"), 1, 40.6f, 8, 7, 1.f);    // 19.59 km, right, 6 in the set
Zone(TEXT("World s1people, a rock cut, over the rim, slot 441"), 19.9341f, 19.9703f);
	Site(TEXT("WldS1PEOPLERim"), -1, 38.5f, 8, 7, 1.f);    // 19.95 km, left, 2 in the set
Zone(TEXT("World s1people, a rock cut, over the rim, slot 441"), 19.9341f, 19.9703f);
	Site(TEXT("WldS1PEOPLERim"), 1, 38.5f, 8, 7, 1.f);    // 19.95 km, right, 2 in the set
Zone(TEXT("World s1people, a rock cut, over the rim, slot 449"), 20.296f, 20.3322f);
	Site(TEXT("WldS1PEOPLERim"), 1, 38.5f, 8, 7, 1.f);    // 20.31 km, right, 2 in the set
Zone(TEXT("World s1people, a rock cut, over the rim, slot 457"), 20.658f, 20.6941f);
	Site(TEXT("WldS1PEOPLERim"), -1, 44.0f, 8, 7, 1.f);    // 20.68 km, left, 2 in the set; fix round: was 38.5 m, in an older piece
Zone(TEXT("World s1people, red rock return, second gap, slot 465"), 21.0199f, 21.0561f);
	Site(TEXT("WldS1PEOPLEMineAN"), -1, 42.0f, 8, 7, 1.f);    // 21.04 km, left, 6 in the set; fix round: was 40.6 m, in an older piece
Zone(TEXT("World s1people, red rock return, second gap, slot 465"), 21.0199f, 21.0561f);
	Site(TEXT("WldS1PEOPLEMineBP"), 1, 40.6f, 8, 7, 1.f);    // 21.04 km, right, 6 in the set
Zone(TEXT("World s1people, a rock cut, over the rim, slot 473"), 21.3818f, 21.418f);
	Site(TEXT("WldS1PEOPLERim"), 1, 38.5f, 8, 7, 1.f);    // 21.40 km, right, 2 in the set
Zone(TEXT("World s1people, a rock cut, over the rim, slot 481"), 21.7438f, 21.78f);
	Site(TEXT("WldS1PEOPLERim"), -1, 38.5f, 8, 7, 1.f);    // 21.76 km, left, 2 in the set
Zone(TEXT("World s1people, Vegas approach, slot 497"), 22.4677f, 22.5039f);
	Site(TEXT("WldS1PEOPLEVegasAN"), 1, 43.2f, 8, 7, 1.f);    // 22.49 km, right, 6 in the set
Zone(TEXT("World s1people, Vegas approach, slot 513"), 23.1916f, 23.2278f);
	Site(TEXT("WldS1PEOPLEVegasBN"), 1, 43.2f, 8, 7, 1.f);    // 23.21 km, right, 6 in the set
Zone(TEXT("World s1people, Vegas approach, slot 529"), 23.9154f, 23.9516f);
	Site(TEXT("WldS1PEOPLEVegasAN"), 1, 43.2f, 8, 7, 1.f);    // 23.93 km, right, 6 in the set
Zone(TEXT("World s1people, Vegas approach, slot 545"), 24.6393f, 24.6755f);
	Site(TEXT("WldS1PEOPLEVegasBN"), 1, 43.2f, 8, 7, 1.f);    // 24.66 km, right, 6 in the set
Zone(TEXT("World s1people, Vegas approach, slot 561"), 25.3632f, 25.3994f);
	Site(TEXT("WldS1PEOPLEVegasAN"), 1, 43.2f, 8, 7, 1.f);    // 25.38 km, right, 6 in the set
#undef S1PEOPLE
// ---- END s1people ----
}
