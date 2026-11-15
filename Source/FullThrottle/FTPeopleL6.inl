// Under the Sea (level index 6): the population layer. People, creatures and film crews, placed by rule like the dressing
// (the words and what their numbers mean are in the header of FTTrackDress.cpp; the rule kinds in FTDress.h).
// This table is read after FTDressL6.inl: start with a Zone line, and every rule here comes after every dressing
// rule, so the dressing's own placements never move. A mesh name without a leading slash is looked up in
// /Game/Maps/Abyss/Dress/; give a full path for the population's own meshes, e.g. TEXT("/Game/People/SM_CrowdCheerA").
// Rules that bind this layer: nothing over 2 m tall nearer than 24 m to a centre line and nothing at all nearer than
// 17 m; nothing lit that blinks nearer than 30 m; nothing that could be taken for a pickup, an enemy or an attack.

Zone(TEXT("People: test"), 0.0f, 0.55f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_Octopus"), 50.f, 50.5f, 12, 10, 1.f, 1.6f, 0.f, 1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_AquaWeld"), 26.f, 26.5f, 12, 10, 1.f, 1.35f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_SeaCamSled"), 26.f, 26.5f, 12, 9, 1.f, 1.3f, 0.f, 1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_Turtles"), 40.f, 40.5f, 12, 9, 1.f, 1.5f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_AquaLift"), 44.f, 44.5f, 12, 8, 1.f, 1.6f, 0.f, 1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_Octopus"), 60.f, 60.5f, 12, 8, 1.f, 1.7f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_JellyGiants"), 200.f, 200.5f, 12, 7, 1.f, 1.1f, 0.f, 1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarConvoy"), 180.f, 180.5f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarConvoy"), 170.f, 170.5f, 12, 6, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_JellyGiants"), 220.f, 220.5f, 12, 6, 1.f, 1.1f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_AquaWeld"), 26.f, 26.5f, 12, 5, 1.f, 1.35f, 0.f, 1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_Dolphins"), 80.f, 80.5f, 12, 5, 1.f, 1.8f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_Turtles"), 40.f, 40.5f, 12, 4, 1.f, 1.5f, 0.f, 1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_SeaCamSled"), 26.f, 26.5f, 12, 4, 1.f, 1.3f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_Dolphins"), 70.f, 70.5f, 12, 3, 1.f, 1.8f, 0.f, 1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_AquaLift"), 44.f, 44.5f, 12, 3, 1.f, 1.6f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_Octopus"), 48.f, 48.5f, 12, 2, 1.f, 1.6f, 0.f, 1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_DiveTeam_B"), 27.f, 27.5f, 12, 2, 1.f, 1.5f, 0.f, -1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_JellyGiants"), 160.f, 160.5f, 12, 1, 1.f, 1.1f, 0.f, 1, 0.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarConvoy"), 150.f, 150.5f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);
