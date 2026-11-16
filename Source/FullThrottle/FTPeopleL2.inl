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
