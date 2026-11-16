// Under the Sea (level index 6): the population layer. People, creatures and film crews, placed by rule like the dressing
// (the words and what their numbers mean are in the header of FTTrackDress.cpp; the rule kinds in FTDress.h).
// This table is read after FTDressL6.inl: start with a Zone line, and every rule here comes after every dressing
// rule, so the dressing's own placements never move. A mesh name without a leading slash is looked up in
// /Game/Maps/Abyss/Dress/; give a full path for the population's own meshes, e.g. TEXT("/Game/People/SM_CrowdCheerA").
// Rules that bind this layer: nothing over 2 m tall nearer than 24 m to a centre line and nothing at all nearer than
// 17 m; nothing lit that blinks nearer than 30 m; nothing that could be taken for a pickup, an enemy or an attack.
//
// The cast (Content/Python/ft_people_sea.py, /Game/People/UnderTheSea/) and the crew in the Sea wardrobe
// (/Game/People/Crew/SM_Crew<Piece>_Sea). What the frames taught: the sand lies 0.9 m under the road behind a metre of
// barrier and the reef is a riot of small shapes, so people on the sand are lost. This level's people are IN THE WATER,
// 4 to 15 m up where the background is plain blue: swimming, riding, on stands and scaffolds; and they are drawn large
// (the Scale below is part of the design: at 1.0 a diver is a speck from the road).
//   by the barrier (17.6-20 m, LOW)   SM_CrabRow_A/B, the crew's low pieces
//   24 to 60 m                        SM_Watchers_A/B (a stand), SM_AquaWeld (a scaffold), SM_AquaLift (a lift under
//                                     bags), SM_DiveTeam_A/B, SM_SeahorsePatrol, SM_Turtles, SM_SeaCamSled (crew)
//   60 to 130 m                       SM_Octopus, SM_Dolphins, SM_Turtles
//   150 m to the trench walls         SM_JellyGiants, SM_FarConvoy
// Pieces that travel along the road (teams, patrol, turtles, dolphins, convoy) take no yaw jitter: their line is the road's.
// A slot is 45.243 m; the districts are the dressing's. The finale (GREAT WHITE) starts at slot 175 and is fought through
// Nereida, so Nereida's own people are the crowd of the fight: stands on both sides all the way.
#define PP(x) TEXT("/Game/People/UnderTheSea/" x)
#define CREW(x) TEXT("/Game/People/Crew/SM_Crew" x "_Sea")
auto PSlotKm = [](float S) { return S * 0.045243f; };

// ---- the shoot's base camp at the start (slots 1 to 6): every rule here takes one slot (every 6, its own phase)
Zone(TEXT("People: the start"), 0.0f, 0.28f);
	Scatter(CREW("ApplauseA"), 17.8f, 18.6f, 6, 5, 1.f, 1.f, 0.f, 1, 0.f);        // slot 1, right
	Scatter(CREW("Camera"), 18.f, 19.f, 6, 4, 1.f, 1.f, 0.f, 1, 0.f);             // slot 2, right
	Scatter(PP("SM_SeaCamSled"), 26.f, 28.f, 6, 4, 1.f, 1.3f, 0.f, -1, 0.f);       // slot 2, left
	Scatter(CREW("Crane"), 25.5f, 27.f, 6, 3, 1.f, 1.f, 0.f, -1, 0.f);            // slot 3, left
	Scatter(CREW("Village"), 18.f, 19.f, 6, 3, 1.f, 1.f, 0.f, 1, 0.f);            // slot 3, right
	Scatter(CREW("ApplauseB"), 17.8f, 18.6f, 6, 2, 1.f, 1.f, 0.f, -1, 0.f);       // slot 4, left
	Scatter(CREW("Lights"), 26.f, 28.f, 6, 2, 1.f, 1.f, 0.f, 1, 0.f);             // slot 4, right
	Scatter(CREW("Stunt"), 18.f, 19.f, 6, 1, 1.f, 1.f, 0.f, -1, 0.f);             // slot 5, left
	Scatter(CREW("Catering"), 27.f, 30.f, 6, 1, 1.f, 1.f, 0.f, 1, 0.f);           // slot 5, right
	Scatter(PP("SM_Watchers_A"), 26.f, 28.f, 6, 5, 1.f, 1.3f, 0.f, -1, 4.f);       // slot 1, left: the first thing the player sees
	Scatter(PP("SM_Watchers_B"), 26.f, 28.f, 6, 0, 1.f, 1.3f, 0.f, 1, 4.f);        // slot 6, right

// ---- 0 to 1.7 km: THE REEF. Sport divers over the coral, turtles, the octopus, crabs on the ledges, dolphins further out.
Zone(TEXT("People: the reef"), 0.0f, 1.7f);
	Scatter(PP("SM_CrabRow_A"), 17.6f, 19.2f, 5, 0, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(PP("SM_CrabRow_B"), 17.6f, 19.2f, 5, 1, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(PP("SM_DiveTeam_A"), 27.f, 34.f, 5, 2, 1.f, 1.5f, 0.1f, 1, 0.f);
	Scatter(PP("SM_DiveTeam_B"), 27.f, 34.f, 5, 0, 1.f, 1.5f, 0.1f, -1, 0.f);
	Scatter(PP("SM_Turtles"), 34.f, 56.f, 7, 1, 1.f, 1.5f, 0.15f, 1, 0.f);
	Scatter(PP("SM_Turtles"), 34.f, 56.f, 7, 3, 1.f, 1.5f, 0.15f, -1, 0.f);
	Scatter(PP("SM_Octopus"), 46.f, 70.f, 14, 3, 1.f, 1.6f, 0.1f, -1, 20.f);
	Scatter(PP("SM_Octopus"), 46.f, 70.f, 14, 7, 1.f, 1.6f, 0.1f, 1, 20.f);
	Scatter(PP("SM_Dolphins"), 60.f, 110.f, 8, 2, 1.f, 1.7f, 0.15f, 0, 0.f);
	Scatter(PP("SM_Watchers_A"), 26.f, 29.f, 12, 4, 1.f, 1.3f, 0.f, 1, 4.f);
	Scatter(PP("SM_Watchers_B"), 26.f, 29.f, 12, 1, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(PP("SM_JellyGiants"), 150.f, 260.f, 7, 1, 1.f, 1.f, 0.2f, 0, 180.f);
	Scatter(PP("SM_FarConvoy"), 150.f, 230.f, 15, 5, 1.f, 1.f, 0.f, 0, 0.f);

// ---- 1.7 to 3.4 km: THE KELP FOREST. Nereida's mounted patrol rides the forest's edge; photographers; turtles.
Zone(TEXT("People: the kelp forest"), 1.7f, 3.4f);
	Scatter(PP("SM_SeahorsePatrol"), 25.5f, 27.f, 5, 0, 1.f, 1.5f, 0.1f, 1, 0.f);
	Scatter(PP("SM_SeahorsePatrol"), 25.5f, 27.f, 5, 1, 1.f, 1.5f, 0.1f, -1, 0.f);
	Scatter(PP("SM_DiveTeam_B"), 27.f, 36.f, 7, 2, 1.f, 1.5f, 0.1f, 1, 0.f);
	Scatter(PP("SM_DiveTeam_A"), 27.f, 36.f, 7, 0, 1.f, 1.5f, 0.1f, -1, 0.f);
	Scatter(PP("SM_Turtles"), 30.f, 50.f, 8, 3, 1.f, 1.5f, 0.15f, 0, 0.f);
	Scatter(PP("SM_CrabRow_B"), 17.6f, 19.2f, 7, 1, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(PP("SM_CrabRow_A"), 17.6f, 19.2f, 7, 3, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(PP("SM_Octopus"), 44.f, 64.f, 15, 5, 1.f, 1.6f, 0.1f, 0, 20.f);
	Scatter(PP("SM_Watchers_B"), 26.f, 29.f, 12, 3, 1.f, 1.3f, 0.f, 1, 4.f);
	Scatter(PP("SM_Watchers_A"), 26.f, 29.f, 12, 6, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(PP("SM_Dolphins"), 70.f, 120.f, 10, 1, 1.f, 1.8f, 0.15f, 0, 0.f);
	Scatter(PP("SM_JellyGiants"), 150.f, 270.f, 8, 2, 1.f, 1.1f, 0.2f, 0, 180.f);

// ---- A note on slots for the four districts below. The dressing's pressure habitats (SM_PressureHab: 11 m tall, a
// tank beside the sphere, turned any way) stand at 33-38 m and reach in to 25 m. They take known slots (station, left:
// odd slots; salvage: s%6 == 4; approach: s%3 == 2; Nereida, right: s%6 == 3), so every rule here that stands nearer than
// 31 m takes an "every" that is a multiple of theirs and a phase that never meets them. (s + phase) % every == 0.

// ---- 3.4 to 5.0 km: THE RESEARCH STATION. Aquanauts at work on scaffolds by the pipeline and the tube, shifts watching
// from stands, cargo going up under lift bags, the patrol.
Zone(TEXT("People: the research station"), 3.4f, 5.0f);
	Scatter(PP("SM_AquaWeld"), 25.5f, 27.f, 4, 0, 1.f, 1.35f, 0.f, -1, 0.f);        // left, behind the pipeline (even slots)
	Scatter(PP("SM_Watchers_B"), 25.5f, 27.f, 8, 2, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(PP("SM_SeahorsePatrol"), 25.5f, 27.f, 8, 6, 1.f, 1.5f, 0.f, -1, 0.f);
	Scatter(PP("SM_Watchers_A"), 25.5f, 27.f, 6, 1, 1.f, 1.3f, 0.f, 1, 4.f);        // right, in front of the travel tube
	Scatter(PP("SM_DiveTeam_B"), 25.5f, 27.5f, 6, 4, 1.f, 1.5f, 0.f, 1, 0.f);
	Scatter(PP("SM_AquaWeld"), 40.f, 42.f, 6, 0, 1.f, 1.5f, 0.f, 1, 10.f);          // right, behind the tube
	Scatter(PP("SM_DiveTeam_A"), 40.f, 42.f, 9, 7, 1.f, 1.7f, 0.f, 1, 0.f);
	Scatter(PP("SM_AquaLift"), 44.f, 60.f, 7, 1, 1.f, 1.6f, 0.1f, -1, 40.f);
	Scatter(PP("SM_AquaLift"), 54.f, 66.f, 8, 4, 1.f, 1.6f, 0.1f, 1, 40.f);
	Scatter(PP("SM_Dolphins"), 70.f, 120.f, 12, 3, 1.f, 1.8f, 0.15f, 0, 0.f);
	Scatter(PP("SM_FarConvoy"), 140.f, 220.f, 8, 1, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(PP("SM_JellyGiants"), 160.f, 270.f, 10, 4, 1.f, 1.f, 0.2f, 0, 180.f);

// ---- 5.0 to 6.6 km: THE SALVAGE FIELD. Lifts everywhere, salvage divers with cameras, the octopus among the wrecks.
Zone(TEXT("People: the salvage field"), 5.0f, 6.6f);
	Scatter(PP("SM_AquaLift"), 40.f, 58.f, 3, 0, 1.f, 1.6f, 0.1f, -1, 40.f);
	Scatter(PP("SM_AquaLift"), 40.f, 58.f, 3, 1, 1.f, 1.6f, 0.1f, 1, 40.f);
	Scatter(PP("SM_DiveTeam_B"), 25.5f, 27.5f, 6, 0, 1.f, 1.5f, 0.f, -1, 0.f);
	Scatter(PP("SM_DiveTeam_A"), 25.5f, 27.5f, 6, 3, 1.f, 1.5f, 0.f, 1, 0.f);
	Scatter(PP("SM_AquaWeld"), 25.5f, 27.f, 9, 4, 1.f, 1.35f, 0.f, 0, 10.f);
	Scatter(PP("SM_Watchers_A"), 25.5f, 27.f, 12, 6, 1.f, 1.3f, 0.f, 0, 4.f);
	Scatter(PP("SM_Octopus"), 44.f, 60.f, 10, 2, 1.f, 1.7f, 0.1f, -1, 20.f);
	Scatter(PP("SM_Octopus"), 44.f, 60.f, 10, 7, 1.f, 1.7f, 0.1f, 1, 20.f);
	Scatter(PP("SM_Turtles"), 40.f, 60.f, 10, 4, 1.f, 1.5f, 0.15f, 0, 0.f);
	Scatter(PP("SM_CrabRow_A"), 17.6f, 19.2f, 8, 1, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(PP("SM_FarConvoy"), 140.f, 220.f, 7, 0, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(PP("SM_Dolphins"), 80.f, 130.f, 12, 6, 1.f, 1.8f, 0.15f, 0, 0.f);

// ---- 6.6 to 7.94 km: THE APPROACH. Nereida's outworks: the patrol in strength, the first big crowds, dolphins and
// giant jellyfish over the vents.
Zone(TEXT("People: the approach"), 6.6f, 7.94f);
	Scatter(PP("SM_SeahorsePatrol"), 25.5f, 27.f, 3, 0, 1.f, 1.5f, 0.f, 1, 0.f);
	Scatter(PP("SM_SeahorsePatrol"), 25.5f, 27.f, 3, 2, 1.f, 1.5f, 0.f, -1, 0.f);
	Scatter(PP("SM_Watchers_A"), 25.5f, 27.f, 6, 5, 1.f, 1.3f, 0.f, 1, 4.f);
	Scatter(PP("SM_Watchers_B"), 25.5f, 27.f, 6, 3, 1.f, 1.3f, 0.f, -1, 4.f);
	Scatter(PP("SM_Dolphins"), 50.f, 100.f, 7, 3, 1.f, 1.7f, 0.15f, 0, 0.f);
	Scatter(PP("SM_AquaWeld"), 46.f, 52.f, 9, 4, 1.f, 1.5f, 0.f, 0, 10.f);
	Scatter(PP("SM_DiveTeam_A"), 46.f, 54.f, 9, 0, 1.f, 1.7f, 0.f, 0, 0.f);
	Scatter(PP("SM_JellyGiants"), 140.f, 260.f, 5, 0, 1.f, 1.1f, 0.2f, 0, 180.f);
	Scatter(PP("SM_FarConvoy"), 140.f, 220.f, 10, 5, 1.f, 1.f, 0.f, 0, 0.f);

// ---- 7.94 km to the end: NEREIDA, and the fight with the boss. The city is out on its stands on both sides; the
// patrol rides; work goes on at the pipeline (right) and the tube (left); everything else of the level passes through.
// Right: the pipeline on the verge, habitats at 33-38 m in slots s%6 == 3. Left: the tube at 33 m (4 to 6.5 m up),
// habitats at 44-50 m in slots s%6 == 1.
Zone(TEXT("People: Nereida"), 7.94f, 99.f);
	Scatter(PP("SM_Watchers_B"), 25.5f, 27.f, 12, 0, 1.f, 1.3f, 0.f, 1, 4.f);          // right: s%12 == 0
	Scatter(PP("SM_SeahorsePatrol"), 25.5f, 27.f, 18, 16, 1.f, 1.5f, 0.f, 1, 0.f);     // s%18 == 2
	Scatter(PP("SM_AquaWeld"), 25.5f, 27.f, 18, 10, 1.f, 1.35f, 0.f, 1, 0.f);          // s%18 == 8, behind the pipeline
	Scatter(PP("SM_DiveTeam_A"), 25.5f, 27.5f, 18, 4, 1.f, 1.5f, 0.f, 1, 0.f);         // s%18 == 14
	Scatter(PP("SM_Watchers_A"), 40.f, 43.f, 18, 13, 1.f, 1.7f, 0.f, 1, 4.f);          // s%18 == 5: further back, higher and larger
	Scatter(PP("SM_Watchers_A"), 25.5f, 27.f, 12, 6, 1.f, 1.3f, 0.f, -1, 4.f);         // left: s%12 == 6
	Scatter(PP("SM_SeahorsePatrol"), 25.5f, 27.f, 18, 8, 1.f, 1.5f, 0.f, -1, 0.f);     // s%18 == 10
	Scatter(PP("SM_DiveTeam_B"), 25.5f, 27.5f, 18, 2, 1.f, 1.5f, 0.f, -1, 0.f);        // s%18 == 16
	Scatter(PP("SM_AquaWeld"), 40.f, 42.f, 18, 15, 1.f, 1.5f, 0.f, -1, 10.f);          // s%18 == 3, behind the tube
	Scatter(PP("SM_Watchers_B"), 40.f, 43.f, 18, 9, 1.f, 1.7f, 0.f, -1, 4.f);          // s%18 == 9
	Scatter(PP("SM_AquaLift"), 42.f, 50.f, 20, 8, 1.f, 1.6f, 0.1f, 0, 40.f);
	Scatter(PP("SM_Turtles"), 40.f, 48.f, 27, 5, 1.f, 1.6f, 0.15f, 0, 0.f);
	Scatter(PP("SM_Dolphins"), 70.f, 120.f, 24, 10, 1.f, 1.8f, 0.15f, 0, 0.f);
	Scatter(PP("SM_Octopus"), 66.f, 76.f, 54, 13, 1.f, 1.8f, 0.1f, 0, 20.f);
	Scatter(PP("SM_CrabRow_A"), 17.6f, 19.2f, 20, 6, 1.f, 1.f, 0.f, -1, 0.f);        // the left verge (the pipeline has the right)
	Scatter(PP("SM_CrabRow_B"), 17.6f, 19.2f, 20, 16, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(PP("SM_JellyGiants"), 150.f, 270.f, 22, 3, 1.f, 1.1f, 0.2f, 0, 180.f);
	Scatter(PP("SM_FarConvoy"), 140.f, 230.f, 26, 8, 1.f, 1.f, 0.f, 0, 0.f);
	// the crew along the fight
	Scatter(CREW("ApplauseA"), 17.8f, 18.6f, 48, 5, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("ApplauseB"), 17.8f, 18.6f, 48, 29, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(PP("SM_SeaCamSled"), 25.5f, 27.f, 36, 23, 1.f, 1.3f, 0.f, -1, 0.f);        // left: s%36 == 13
	Scatter(PP("SM_SeaCamSled"), 25.5f, 27.f, 36, 25, 1.f, 1.3f, 0.f, 1, 0.f);         // right: s%36 == 11
	Scatter(CREW("Crane"), 25.5f, 26.5f, 72, 43, 1.f, 1.f, 0.f, -1, 0.f);
	Scatter(CREW("Lights"), 25.5f, 26.5f, 72, 7, 1.f, 1.f, 0.f, -1, 0.f);

// ---- the crew along the way: before the jumps (slots 57 and 122-123), before and after the loops and corkscrews
// (62-65, 71-76, 128-131, 148-154), and before the boss (the finale starts at slot 175; 167-172 is a tunnel).
auto CrewAt = [&](float S, int32 Side, const TCHAR* Low, const TCHAR* Tall)
{
	Zone(TEXT("People: the crew"), PSlotKm(S - 0.5f), PSlotKm(S + 0.4f));
	Scatter(Low, 17.8f, 18.8f, 1, 0, 1.f, 1.f, 0.f, Side, 0.f);
	Scatter(Tall, 25.5f, 27.f, 1, 0, 1.f, 1.f, 0.f, -Side, 0.f);
	Scatter(PP("SM_SeaCamSled"), 25.5f, 27.f, 1, 0, 1.f, 1.3f, 0.f, Side, 0.f);
};
CrewAt(10, 1, CREW("Dolly"), CREW("Sound"));
CrewAt(29, -1, CREW("ApplauseA"), CREW("Crane"));
CrewAt(54, 1, CREW("Camera"), CREW("Crane"));          // the jump at 57
CrewAt(55, -1, CREW("Stunt"), CREW("Lights"));
CrewAt(60, 1, CREW("ApplauseB"), CREW("Sound"));        // the loop at 62
CrewAt(68, -1, CREW("Camera"), CREW("Crane"));          // the corkscrew at 71
CrewAt(90, 1, CREW("Dolly"), CREW("Catering"));
CrewAt(119, -1, CREW("Camera"), CREW("Crane"));         // the jump at 122
CrewAt(120, 1, CREW("Stunt"), CREW("Lights"));
CrewAt(126, 1, CREW("ApplauseA"), CREW("Sound"));       // the loop at 128
CrewAt(135, -1, CREW("Village"), CREW("Catering"));
CrewAt(147, 1, CREW("Camera"), CREW("Crane"));          // the set piece at 148
CrewAt(160, -1, CREW("Dolly"), CREW("Lights"));
CrewAt(173, 1, CREW("Camera"), CREW("Crane"));          // before the boss
CrewAt(174, -1, CREW("ApplauseA"), CREW("Lights"));
CrewAt(175, 1, CREW("Village"), CREW("Sound"));
CrewAt(176, -1, CREW("ApplauseB"), CREW("Catering"));
CrewAt(177, 1, CREW("Stunt"), CREW("Crane"));

// ---- the glass tunnels (Scatter rules take no slot of a tunnel, a loop, a corkscrew or a jump's gap: Far rules do,
// with the piece stood on the sand by its drop, as the dressing does). Seen through the glass: swimmers keeping pace
// 30 m out, a stand of watchers, turtles and dolphins beyond. The dressing's own tunnel pieces stand at 34-46 m (low),
// 50-72 and 76-104 m.
auto PplPipe = [&](float A, float B, int32 Ph)
{
	Zone(TEXT("People: by a glass tunnel"), PSlotKm(A - 0.5f), PSlotKm(B + 0.5f));
	Far(PP("SM_DiveTeam_A"), 28.f, 32.f, 4, Ph, 1.f, 1.6f, 0.f, 1, 0.9f, 0.f);
	Far(PP("SM_DiveTeam_B"), 28.f, 32.f, 4, Ph + 2, 1.f, 1.6f, 0.f, -1, 0.9f, 0.f);
	Far(PP("SM_Watchers_A"), 27.5f, 29.f, 5, Ph, 1.f, 1.4f, 0.f, -1, 0.9f, 3.f);
	Far(PP("SM_Watchers_B"), 27.5f, 29.f, 5, Ph + 1, 1.f, 1.4f, 0.f, 1, 0.9f, 3.f);
	Far(PP("SM_Turtles"), 40.f, 60.f, 6, Ph + 2, 1.f, 1.6f, 0.1f, 0, 0.95f, 0.f);
	Far(PP("SM_Dolphins"), 60.f, 100.f, 7, Ph, 1.f, 1.8f, 0.1f, 0, 1.f, 0.f);
};
PplPipe(15, 20, 0); PplPipe(37, 43, 1); PplPipe(79, 83, 0); PplPipe(101, 108, 1); PplPipe(140, 146, 0); PplPipe(167, 172, 1);
PplPipe(310, 314, 0); PplPipe(331, 335, 1); PplPipe(352, 356, 0); PplPipe(373, 377, 1); PplPipe(394, 398, 0); PplPipe(414, 419, 1);
PplPipe(435, 440, 0); PplPipe(456, 461, 1); PplPipe(477, 482, 0); PplPipe(498, 503, 1); PplPipe(519, 523, 0); PplPipe(540, 544, 1);
// under a loop, a corkscrew, a jump: nobody near. Far out and high: dolphins, the giant jellyfish.
auto PplSet = [&](float A, float B)
{
	Zone(TEXT("People: far from a set piece"), PSlotKm(A - 0.5f), PSlotKm(B + 0.5f));
	Far(PP("SM_Dolphins"), 80.f, 130.f, 3, 0, 1.f, 2.f, 0.1f, 0, 1.f, 0.f);
	Far(PP("SM_JellyGiants"), 150.f, 250.f, 3, 1, 1.f, 1.1f, 0.2f, 0, -6.f, 180.f);
};
PplSet(57, 57); PplSet(62, 65); PplSet(71, 76); PplSet(122, 123); PplSet(128, 131); PplSet(148, 154);
#undef PP
#undef CREW

{
// ---- BEGIN far (ft_people_sea_far.py) ----
// Far life and creatures (the extra worker, focus "far"): what lives from 150 m out to the trench walls. Six meshes,
// /Game/People/UnderTheSea/SM_Far*, all saved with the always-moving master. Learned in frames: the chase camera looks
// ahead, so a piece 180 m to the side is seen from 300 m and more, through the haze: every piece is placed at about
// twice the size it was built (the scale below), carries a little steady light of its own colour, and swims 20 to
// 60 m over the sand, against the water. The travelling ones go at most 80 m either way along the road and are turned
// 6 degrees at most, so none comes nearer than about 150 m. Every rule has a chance of 1 and its own step (see the
// dressing's note on chances), one side to a rule, so the two sides are never the same.
// Scale: squid 2.2 (75 m of animal), dome works 1.5, nautilus 2 (shells 10 to 16 m), crabs 2 (225 m and more out, on the trench wall,
// which stands 8 to 13 m over the road there), sea dragon 1.8 (a ribbon of 110 m), scooter wing 1.8.
Zone(TEXT("People far: the reef"), 0.0f, 1.7f);                  // nautilus shoals over the reef, the first squid, a sea dragon
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarNautilus"), 150.f, 200.f, 7, 1, 1.f, 2.0f, 0.f, -1, 20.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarNautilus"), 150.f, 200.f, 7, 4, 1.f, 2.0f, 0.f, 1, 20.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarSquid"), 175.f, 215.f, 13, 3, 1.f, 2.2f, 0.f, -1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarSquid"), 175.f, 215.f, 13, 9, 1.f, 2.2f, 0.f, 1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarSeaDragon"), 170.f, 210.f, 15, 6, 1.f, 1.8f, 0.f, 1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarSeaDragon"), 170.f, 210.f, 15, 13, 1.f, 1.8f, 0.f, -1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarCrabMarch"), 225.f, 280.f, 11, 8, 1.f, 2.0f, 0.f, -1, 6.f);
Zone(TEXT("People far: the kelp forest"), 1.7f, 3.4f);           // crabs on the march along the walls, dragons over the weed
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarCrabMarch"), 225.f, 280.f, 7, 0, 1.f, 2.0f, 0.f, 1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarCrabMarch"), 225.f, 280.f, 7, 3, 1.f, 2.0f, 0.f, -1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarSeaDragon"), 170.f, 210.f, 9, 2, 1.f, 1.8f, 0.f, -1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarSeaDragon"), 170.f, 210.f, 9, 6, 1.f, 1.8f, 0.f, 1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarSquid"), 175.f, 215.f, 17, 5, 1.f, 2.2f, 0.f, 1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarNautilus"), 150.f, 200.f, 13, 9, 1.f, 2.0f, 0.f, -1, 20.f);
Zone(TEXT("People far: the research station"), 3.4f, 5.0f);     // Nereida builds: domes going up, flights of scooters
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarDomeWorks"), 170.f, 225.f, 8, 0, 1.f, 1.5f, 0.f, -1, 10.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarDomeWorks"), 170.f, 225.f, 8, 4, 1.f, 1.5f, 0.f, 1, 10.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarScooterWing"), 170.f, 210.f, 7, 2, 1.f, 1.8f, 0.f, 1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarScooterWing"), 170.f, 210.f, 7, 5, 1.f, 1.8f, 0.f, -1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarNautilus"), 150.f, 200.f, 13, 6, 1.f, 2.0f, 0.f, 1, 20.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarSeaDragon"), 170.f, 210.f, 17, 11, 1.f, 1.8f, 0.f, -1, 6.f);
Zone(TEXT("People far: the salvage field"), 5.0f, 6.6f);        // the squid hunts over the dead ships; crabs pick the wrecks
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarSquid"), 175.f, 215.f, 9, 1, 1.f, 2.2f, 0.f, -1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarSquid"), 175.f, 215.f, 9, 5, 1.f, 2.2f, 0.f, 1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarCrabMarch"), 225.f, 280.f, 8, 3, 1.f, 2.0f, 0.f, 1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarCrabMarch"), 225.f, 280.f, 8, 7, 1.f, 2.0f, 0.f, -1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarNautilus"), 150.f, 200.f, 11, 2, 1.f, 2.0f, 0.f, -1, 20.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarScooterWing"), 170.f, 210.f, 13, 8, 1.f, 1.8f, 0.f, 1, 6.f);
Zone(TEXT("People far: the approach"), 6.6f, 7.94f);            // the city's traffic: scooter wings, a dome, a dragon
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarScooterWing"), 170.f, 210.f, 7, 0, 1.f, 1.8f, 0.f, -1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarScooterWing"), 170.f, 210.f, 7, 3, 1.f, 1.8f, 0.f, 1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarDomeWorks"), 170.f, 225.f, 9, 2, 1.f, 1.5f, 0.f, 1, 10.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarDomeWorks"), 170.f, 225.f, 9, 6, 1.f, 1.5f, 0.f, -1, 10.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarSeaDragon"), 170.f, 210.f, 11, 5, 1.f, 1.8f, 0.f, 1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarSquid"), 175.f, 215.f, 15, 8, 1.f, 2.2f, 0.f, -1, 6.f);
Zone(TEXT("People far: Nereida"), 7.94f, 99.f);                // 17 km of outworks: everything, in turn, thinly
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarDomeWorks"), 170.f, 225.f, 26, 3, 1.f, 1.5f, 0.f, -1, 10.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarDomeWorks"), 170.f, 225.f, 26, 16, 1.f, 1.5f, 0.f, 1, 10.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarScooterWing"), 170.f, 210.f, 23, 7, 1.f, 1.8f, 0.f, 1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarScooterWing"), 170.f, 210.f, 23, 18, 1.f, 1.8f, 0.f, -1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarCrabMarch"), 225.f, 280.f, 29, 11, 1.f, 2.0f, 0.f, -1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarCrabMarch"), 225.f, 280.f, 29, 25, 1.f, 2.0f, 0.f, 1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarSquid"), 175.f, 215.f, 41, 14, 1.f, 2.2f, 0.f, 1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarSquid"), 175.f, 215.f, 41, 34, 1.f, 2.2f, 0.f, -1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarNautilus"), 150.f, 200.f, 31, 9, 1.f, 2.0f, 0.f, 1, 20.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarNautilus"), 150.f, 200.f, 31, 24, 1.f, 2.0f, 0.f, -1, 20.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarSeaDragon"), 170.f, 210.f, 37, 5, 1.f, 1.8f, 0.f, -1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_FarSeaDragon"), 170.f, 210.f, 37, 23, 1.f, 1.8f, 0.f, 1, 6.f);
// ---- END far ----
}

{
// ---- BEGIN watch (ft_people_sea_watch.py) ----
// THE WATCHERS: Nereida has come out to see the chase. Six meshes, each a crowd brought with what it stands on
// (the dressing's domes and hulls have no deck a figure could stand on): SM_WatchBalcony_A / _B (a habitat drum on a
// pylon, a balcony all round it), SM_WatchAirlock (an airlock tower, two landing platforms), SM_WatchDome (a viewing
// dome, a crowd on its rim), SM_WatchWall (divers hanging in the water in three rows with a long banner) and
// SM_WatchFarGallery (a 42 m gallery of giants for the trench walls, always moving).
// All are TALL: their middles stand 33 m out or more, so that nothing of them over 2 m is nearer than 24 m (a balcony
// is 5.4 m in radius, the dome's rim 7 m) and the 17 to 27 m band stays free for the street cast and the film crew.
// The one exception is the wall of divers (2.6 m deep): 26 to 28 m in the kelp forest, in front of the weed, and on
// the travel tube's side of the station and of Nereida, in front of the tube (which stands at 33 m, 5 m up, and hides
// a deck behind it to 8 m up: seen in frames, so nothing of this block stands behind the tube).
// Phases are chosen against the dressing's own rules (the pressure habitats at 33 to 38 m, the vents at 40 to 44 m,
// the habitats' airlock tubes): see ft_people_sea_watch.py's header for what each piece is.
Zone(TEXT("People watch: the reef"), 0.0f, 1.7f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchBalcony_A"), 33.f, 36.f, 10, 1, 1.f, 1.05f, 0.08f, -1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchDome"), 34.f, 37.f, 10, 5, 1.f, 1.f, 0.08f, -1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchWall"), 42.f, 48.f, 10, 8, 1.f, 1.15f, 0.1f, -1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchAirlock"), 33.f, 36.f, 10, 2, 1.f, 1.05f, 0.08f, 1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchBalcony_B"), 33.f, 36.f, 10, 7, 1.f, 1.05f, 0.08f, 1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchWall"), 42.f, 48.f, 10, 4, 1.f, 1.15f, 0.1f, 1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchFarGallery"), 120.f, 170.f, 10, 4, 1.f, 1.f, 0.1f, -1, 10.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchFarGallery"), 120.f, 170.f, 10, 9, 1.f, 1.f, 0.1f, 1, 10.f);
Zone(TEXT("People watch: the kelp forest"), 1.7f, 3.4f);     // the weed stands from 27 m: the divers hang in front of it
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchWall"), 26.f, 27.5f, 6, 0, 1.f, 1.f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchWall"), 26.f, 27.5f, 6, 3, 1.f, 1.f, 0.f, 1, 4.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchAirlock"), 33.f, 35.f, 8, 3, 1.f, 1.1f, 0.05f, -1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchBalcony_B"), 33.f, 35.f, 8, 7, 1.f, 1.1f, 0.05f, 1, 6.f);
Zone(TEXT("People watch: the research station"), 3.4f, 5.0f); // left: behind the pipeline, between the pressure habitats (odd slots); right: behind the tube
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchBalcony_A"), 33.f, 36.f, 8, 2, 1.f, 1.05f, 0.08f, -1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchDome"), 34.f, 37.f, 8, 6, 1.f, 1.f, 0.08f, -1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchWall"), 26.f, 27.5f, 6, 1, 1.f, 1.f, 0.f, 1, 4.f);      // in front of the tube (behind it a deck is hidden to 8 m up)
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchFarGallery"), 130.f, 180.f, 10, 2, 1.f, 1.f, 0.1f, -1, 10.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchFarGallery"), 130.f, 180.f, 10, 7, 1.f, 1.f, 0.1f, 1, 10.f);
Zone(TEXT("People watch: the salvage field"), 5.0f, 6.6f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchBalcony_B"), 33.f, 37.f, 12, 0, 1.f, 1.05f, 0.08f, -1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchAirlock"), 33.f, 37.f, 12, 6, 1.f, 1.05f, 0.08f, -1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchWall"), 44.f, 50.f, 8, 5, 1.f, 1.2f, 0.1f, -1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchDome"), 34.f, 38.f, 12, 3, 1.f, 1.f, 0.08f, 1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchBalcony_A"), 33.f, 37.f, 12, 9, 1.f, 1.05f, 0.08f, 1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchWall"), 44.f, 50.f, 8, 1, 1.f, 1.2f, 0.1f, 1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchFarGallery"), 130.f, 180.f, 10, 3, 1.f, 1.f, 0.1f, -1, 10.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchFarGallery"), 130.f, 180.f, 10, 8, 1.f, 1.f, 0.1f, 1, 10.f);
Zone(TEXT("People watch: the approach"), 6.6f, 7.94f);       // the vents stand at 40 to 44 m (left: even slots, right: odd), a pressure habitat every third slot
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchBalcony_A"), 33.f, 36.f, 6, 5, 1.f, 1.05f, 0.08f, -1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchDome"), 34.f, 37.f, 6, 2, 1.f, 1.f, 0.08f, 1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchAirlock"), 33.f, 36.f, 6, 3, 1.f, 1.05f, 0.08f, -1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchBalcony_B"), 33.f, 36.f, 6, 0, 1.f, 1.05f, 0.08f, 1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchFarGallery"), 120.f, 170.f, 10, 5, 1.f, 1.f, 0.1f, 0, 10.f);
Zone(TEXT("People watch: Nereida"), 7.94f, 99.f);            // right: the pipeline's side (pressure habitats at slots 3 of 6); left: the travel tube's
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchBalcony_A"), 33.f, 37.f, 16, 0, 1.f, 1.05f, 0.08f, 1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchAirlock"), 33.f, 37.f, 16, 4, 1.f, 1.05f, 0.08f, 1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchDome"), 34.f, 38.f, 16, 8, 1.f, 1.f, 0.08f, 1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchBalcony_B"), 33.f, 37.f, 16, 12, 1.f, 1.05f, 0.08f, 1, 8.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchWall"), 42.f, 47.f, 12, 2, 1.f, 1.2f, 0.1f, 1, 6.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchWall"), 26.f, 27.5f, 8, 1, 1.f, 1.f, 0.f, -1, 4.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchFarGallery"), 125.f, 190.f, 14, 6, 1.f, 1.f, 0.1f, -1, 10.f);
	Scatter(TEXT("/Game/People/UnderTheSea/SM_WatchFarGallery"), 125.f, 190.f, 14, 13, 1.f, 1.f, 0.1f, 1, 10.f);
// ---- END watch ----
}

{
// ---- BEGIN p3a (gaps, ft_people_sea_p3a.py) ----
// FIX ROUND (after the two reviews of pass 3, SourceArt/Integration/people/L6_sea_p3.md): the water rules at 30 to 34 m stood in the slots and the band of p3c's movers and
// p3f's units (salvage field, approach, Nereida's right) and behind the pieces in front of the travel tube (Nereida's left). They were moved out or to free slots, thinned, or
// taken out: every changed line says "fix round". A rule taken out is left as a comment (the rules after it draw other random numbers than before: only this block's and later ones).
// THE GAPS (pass 3, builder p3a). Fourteen meshes, /Game/People/UnderTheSea/SM_P3A_*: the general cast of the road to
// Nereida, put where the first two passes left nobody. Where that was: (1) Nereida's 17 km, where the street cast came
// every 12th to 18th slot a side and the left verge had two crab rows in 20 slots; (2) the water behind the travel tube
// (left) and behind the pipeline past the balconies (right); (3) the verge of the first 8 km between the crabs and the
// crew; (4) more company by the glass tunnels. By the log of the one editor run (top as built):
//   LOW, under 2 m, placed at scale 1 only, 21.6 to 23.6 m (behind the grass, the vessels and the crabs at 17.5 to 21 m;
//   the pieces are 1.4 to 2.2 m deep, so nothing of them is nearer than 20.4 m), never on the pipeline's side:
//     VergeGang_A 1.82 m, VergeGang_B 1.76, WallSitters 1.81, Gardeners 1.76, Surveyors 1.75, EelGarden 1.85, LobsterMarch 1.82
//   TALL, never nearer than 30 m (x min of every rule), scaled 1.3 to 1.7 (a diver at 1.0 is a speck from this road):
//     Commuters_A 10.8 m, Commuters_B 11.1, SchoolClass 9.0, KelpFarmers 10.8, MastCrew 11.1 (a swimmer circles it 4.3 m
//     out: its rules start at 34 m), RestBuoy 9.6, Catwalk 5.2
// No glow and nothing that blinks in any of them. Travel is along the piece's X, the road's own line: those rules take
// no yaw jitter. Every rule has a chance of 1 (side 0 = one on each side), with phases chosen against the dressing (pressure vessels on
// the verge, pressure habitats at 33 to 38 and 44 to 50 m, watch towers at 52 and 56 m) and against the first two passes
// (crabs and crew at 17.6 to 19, the street cast at 25.5 to 28, the balconies at 33 to 37): (slot + phase) % every == 0.
// Estimated placed: 649 pieces (counted slot by slot without the tunnels' and set pieces' slots; what the road
// test, run-ups and landings leave out is not known here). No mesh more than 60 times.
#define P3A(n) TEXT("/Game/People/UnderTheSea/SM_P3A_" n)
Zone(TEXT("People p3a: the reef"), 0.f, 1.7f);   // even slots on the verge (the sea whips have the odd ones); the water at 36 to 58 m
	Scatter(P3A("VergeGang_A"), 21.6f, 23.6f, 6, 0, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3A("VergeGang_B"), 21.6f, 23.6f, 6, 2, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("EelGarden"), 21.6f, 23.6f, 6, 4, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3A("EelGarden"), 21.6f, 23.6f, 6, 0, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("Gardeners"), 21.6f, 23.6f, 6, 4, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("Gardeners"), 21.6f, 23.6f, 6, 2, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3A("Commuters_A"), 46.f, 56.f, 7, 2, 1.f, 1.5f, 0.1f, 1, 0.f);
	Scatter(P3A("Commuters_B"), 46.f, 56.f, 7, 5, 1.f, 1.5f, 0.1f, -1, 0.f);
	Scatter(P3A("SchoolClass"), 37.f, 41.f, 9, 3, 1.f, 1.4f, 0.f, 1, 0.f);
	Scatter(P3A("RestBuoy"), 48.f, 58.f, 8, 7, 1.f, 1.4f, 0.1f, 1, 20.f);
	Scatter(P3A("RestBuoy"), 48.f, 58.f, 8, 3, 1.f, 1.4f, 0.1f, -1, 20.f);
Zone(TEXT("People p3a: the kelp forest"), 1.7f, 3.4f);   // the weed stands from 27 m: the verge, and farmers at the weed's edge
	Scatter(P3A("LobsterMarch"), 21.6f, 23.6f, 6, 0, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3A("LobsterMarch"), 21.6f, 23.6f, 6, 2, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("EelGarden"), 21.6f, 23.6f, 6, 0, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("VergeGang_B"), 21.6f, 23.6f, 6, 2, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3A("WallSitters"), 21.6f, 23.6f, 6, 3, 1.f, 1.f, 0.f, 0, 3.f);
	Scatter(P3A("KelpFarmers"), 30.f, 34.f, 5, 1, 1.f, 1.3f, 0.f, -1, 10.f);
	Scatter(P3A("KelpFarmers"), 30.f, 34.f, 5, 3, 1.f, 1.3f, 0.f, 1, 10.f);
Zone(TEXT("People p3a: the research station"), 3.4f, 5.f);   // right verge, odd slots (pressure vessels on the even ones; the pipeline has the left); left water behind the pipeline; right water over and behind the tube
	Scatter(P3A("VergeGang_A"), 21.6f, 23.6f, 4, 1, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3A("Surveyors"), 21.6f, 23.6f, 4, 3, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3A("MastCrew"), 44.f, 50.f, 4, 2, 1.f, 1.35f, 0.f, -1, 20.f);
	// (fix round) taken out, every one of its slots holds a p3c mover in the same band: Scatter(P3A("Catwalk"), 30.f, 32.f, 8, 4, 1.f, 1.3f, 0.f, -1, 0.f);
	Scatter(P3A("Commuters_B"), 56.f, 62.f, 8, 0, 1.f, 1.6f, 0.1f, 1, 0.f);   // over the tube
	Scatter(P3A("RestBuoy"), 44.f, 47.f, 6, 0, 1.f, 1.5f, 0.f, 1, 20.f);   // the deck 8 m up: over the tube
Zone(TEXT("People p3a: the salvage field"), 5.f, 6.6f);   // the verge between the pressure vessels (slots 3 of 4); the water at 30 to 38 m between the habitats (slots 4 of 6)
	Scatter(P3A("Surveyors"), 21.6f, 23.6f, 4, 0, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("Surveyors"), 21.6f, 23.6f, 4, 2, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3A("VergeGang_B"), 21.6f, 23.6f, 4, 3, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("WallSitters"), 21.6f, 23.6f, 4, 3, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3A("LobsterMarch"), 21.6f, 23.6f, 8, 6, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("LobsterMarch"), 21.6f, 23.6f, 8, 4, 1.f, 1.f, 0.f, 1, 3.f);
	Scatter(P3A("Commuters_A"), 45.f, 48.f, 12, 11, 1.f, 1.4f, 0.f, -1, 0.f);   // fix round: was 30-34 m every 6, inside the scooter files and the stunt Set; now behind them, s % 12 == 1
	Scatter(P3A("Commuters_B"), 45.f, 48.f, 12, 11, 1.f, 1.4f, 0.f, 1, 0.f);   // fix round: was 30-34 m every 6, inside the cyclists, the race and the p3f units; now behind the walkway, s % 12 == 1
	Scatter(P3A("MastCrew"), 36.f, 38.f, 12, 1, 1.f, 1.35f, 0.f, 1, 20.f);   // fix round: was 34 m every 6; s % 12 == 11 only (the wagon and the stunt rig have s % 12 == 5)
	// (fix round) taken out, every one of its slots holds p3c's cyclists or walkway in the same band: Scatter(P3A("Catwalk"), 30.f, 32.f, 6, 3, 1.f, 1.3f, 0.f, -1, 0.f);
Zone(TEXT("People p3a: the approach"), 6.6f, 7.94f);   // the verge between the pressure vessels (every third slot)
	Scatter(P3A("VergeGang_A"), 21.6f, 23.6f, 6, 2, 1.f, 1.f, 0.f, 0, 3.f);
	Scatter(P3A("EelGarden"), 21.6f, 23.6f, 6, 1, 1.f, 1.f, 0.f, 0, 3.f);
	Scatter(P3A("Gardeners"), 21.6f, 23.6f, 6, 4, 1.f, 1.f, 0.f, 0, 3.f);
	Scatter(P3A("Commuters_A"), 45.f, 48.f, 12, 5, 1.f, 1.4f, 0.f, -1, 0.f);   // fix round: was 30-33 m every 6, inside the walkway, the wagon and the extras; now s % 12 == 7, behind the balcony
	Scatter(P3A("Commuters_B"), 45.f, 48.f, 12, 0, 1.f, 1.4f, 0.f, 1, 0.f);   // fix round: was 30-33 m every 6, inside the scooter file; now s % 12 == 0, behind the balcony
Zone(TEXT("People p3a: Nereida"), 7.94f, 99.f);   // LEFT verge: slots 1,2,3,5,6,9,10,11,13,14 of 16 (pressure vessels on 0,4,8,12). After the fix round: RIGHT, behind the pipeline: 33.5 to 36 m in slot 10 of 12, 30 to 34 m in slot 11 of 12, a catwalk in slot 2 of 60; 47 to 50 m over and behind the pressure habitats (slots 3 and 9 of 12); 46.5 to 48 m in slot 6 of 24. LEFT, behind the tube, only where nothing stands in front of it: 42 to 43.5 m in slots 0, 8 and 12 of 24 (before the watch towers), 42 to 48 m in slots 2 and 26 of 48
	Scatter(P3A("VergeGang_A"), 21.6f, 23.6f, 16, 15, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("VergeGang_A"), 21.6f, 23.6f, 16, 2, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("VergeGang_B"), 21.6f, 23.6f, 16, 14, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("VergeGang_B"), 21.6f, 23.6f, 16, 7, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("WallSitters"), 21.6f, 23.6f, 16, 10, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("WallSitters"), 21.6f, 23.6f, 16, 3, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("Surveyors"), 21.6f, 23.6f, 16, 6, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("Gardeners"), 21.6f, 23.6f, 16, 13, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("EelGarden"), 21.6f, 23.6f, 16, 5, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("LobsterMarch"), 21.6f, 23.6f, 16, 11, 1.f, 1.f, 0.f, -1, 3.f);
	Scatter(P3A("Commuters_A"), 33.5f, 36.f, 12, 2, 1.f, 1.4f, 0.f, 1, 0.f);   // fix round: was s % 12 == 1 (a p3f unit and a p3b scene in every one); now s % 12 == 10
	Scatter(P3A("Catwalk"), 30.f, 30.5f, 60, 58, 1.f, 1.3f, 0.f, 1, 0.f);   // fix round: was s % 12 == 2 (a p3c mover in four of five); now s % 60 == 2 only
	Scatter(P3A("SchoolClass"), 47.f, 50.f, 12, 3, 1.f, 1.4f, 0.f, 1, 0.f);   // fix round: was 30-34 m, s % 12 == 5 (a p3f kit rostrum in every one); now s % 12 == 9, over and behind the pressure habitat
	Scatter(P3A("Commuters_B"), 47.f, 50.f, 12, 9, 1.f, 1.4f, 0.f, 1, 0.f);   // fix round: was 30-34 m, s % 12 == 7 (a p3f unit and a p3b scene in every one); now s % 12 == 3, over and behind the pressure habitat
	Scatter(P3A("KelpFarmers"), 30.f, 34.f, 12, 1, 1.f, 1.3f, 0.f, 1, 10.f);
	Scatter(P3A("RestBuoy"), 46.5f, 48.f, 24, 18, 1.f, 1.4f, 0.f, 1, 20.f);   // fix round: was 42-44 m, inside the p3b scene of the same slot (s % 24 == 6)
	Scatter(P3A("MastCrew"), 42.f, 48.f, 48, 46, 1.f, 1.4f, 0.f, -1, 20.f);   // fix round: was s % 12 == 3 (a p3b scene or a kit rostrum in front of the tube in most); now s % 48 == 2: nothing in front of the tube there
	// (fix round) taken out, left of Nereida: no slot left behind the tube without a piece in front of it: Scatter(P3A("KelpFarmers"), 42.f, 48.f, 24, 19, 1.f, 1.4f, 0.f, -1, 10.f);
	Scatter(P3A("RestBuoy"), 42.f, 43.5f, 24, 0, 1.f, 1.45f, 0.f, -1, 20.f);   // fix round: was s % 24 == 9 (a p3f unit in front of the tube in every one); now s % 24 == 0, before the watch tower, nothing in front of the tube
	Scatter(P3A("Commuters_A"), 42.f, 43.5f, 24, 12, 1.f, 1.6f, 0.f, -1, 0.f);   // fix round: was s % 24 == 11 (a p3b scene in front of the tube); now s % 24 == 12
	Scatter(P3A("Commuters_B"), 42.f, 43.5f, 24, 16, 1.f, 1.6f, 0.f, -1, 0.f);   // fix round: was s % 24 == 23 (the first pass in front of the tube); now s % 24 == 8
	Scatter(P3A("SchoolClass"), 44.f, 48.f, 48, 22, 1.f, 1.7f, 0.f, -1, 0.f);   // fix round: s % 48 == 26 (was every 24)
// The glass tunnels (Scatter rules take none of their slots): Far rules used as land rules, as the dressing and the first
// pass do, the piece stood on the sand by its drop. Seen through the glass, 30 m and more out, over the tunnel dressing's low pieces.
auto P3ATunnel = [&](float A, float B)
{
	Zone(TEXT("People p3a: by a glass tunnel"), (A - 0.5f) * 0.045243f, (B + 0.5f) * 0.045243f);
	Far(P3A("Catwalk"), 34.f, 38.f, 5, 1, 1.f, 1.4f, 0.f, 1, 0.94f, 0.f);
	Far(P3A("MastCrew"), 36.f, 42.f, 8, 2, 1.f, 1.4f, 0.f, -1, 0.95f, 20.f);
	Far(P3A("SchoolClass"), 30.f, 33.f, 9, 4, 1.f, 1.5f, 0.f, -1, 0.9f, 0.f);
	Far(P3A("RestBuoy"), 44.f, 52.f, 8, 5, 1.f, 1.5f, 0.f, 1, 0.98f, 20.f);
};
P3ATunnel(15, 20); P3ATunnel(37, 43); P3ATunnel(79, 83); P3ATunnel(101, 108); P3ATunnel(140, 146); P3ATunnel(167, 172);
P3ATunnel(310, 314); P3ATunnel(331, 335); P3ATunnel(352, 356); P3ATunnel(373, 377); P3ATunnel(394, 398); P3ATunnel(414, 419);
P3ATunnel(435, 440); P3ATunnel(456, 461); P3ATunnel(477, 482); P3ATunnel(498, 503); P3ATunnel(519, 523); P3ATunnel(540, 544);
#undef P3A
// ---- END p3a ----

}

{
// ---- BEGIN p3b (scenes, ft_people_sea_p3b.py) ----
// FIX ROUND: Nereida's right starts at 37.5 m (was 36.5: the scene's front reached into the p3f unit of the same slot); Nereida's left, in front of the tube, is every 96 slots
// (was 48) and off the slots of p3f's kit rostrums (s % 12 == 9): s % 96 in {3, 19, 27, 35, 43, 51, 59, 67, 83, 91}.
// Story groups that could only be on the road to Nereida: one mesh a scene, 8 to 13 figures and the props that tell it
// (/Game/People/UnderTheSea/SM_P3B_*): Wedding (under a coral arch), PearlMarket, TreasureHaul (a chest of gold under a hoist),
// SeahorseRace (round the judges' tower), KelpHarvest, SchoolTrip, StatueDig (a sea king's stone head), BubbleBand (a clam-shell
// stage), RescueDrill (a stretcher team and the capsule), SubLaunch. Each brings its own deck or rock 2.4 m and more over the sand.
// All ten are TALL (tops 7.6 to 12.0 m as built): no rule here has an x min under 28 m. A piece is at most 6.4 m deep as built
// (3.4 m either side of its middle; what circles in it stays inside that), so:
//   28.0 to 28.2 m, scale 1    in front of the weed (the kelp forest), of the travel tube (the station's right, Nereida's left:
//                              the tube's face is 31.6 m out) and of the vents (the approach): 24.6 to 31.6 m
//   38 to 41 m, scale 1.2      the reef and the salvage field, behind the 25 to 37 m cast: 33.9 to 45.1 m
//   36.5 to 38.5 m, scale 1.2  Nereida's right (the pipeline's side): 32.4 to 42.6 m
//   32 to 33 m, scale 1.1      beside the glass tunnels (Far rules used as land rules, the dressing's way: 28.2 m and more)
// The first 8 km are placed slot by slot (one district of one slot each, as the crew's CrewAt does): the slots are chosen against
// the rules of FTPeopleL6.inl and FTDressL6.inl in the same band (the watchers' balconies, domes and walls, the pressure
// habitats, the lifts, the patrol, the crew's slots, the slots by a jump, a loop or a corkscrew). The research station's left
// has no free slot in this band and gets none. Nereida is by rule, steps of 48 slots: on the right the slots s % 24 in
// {1,2,6,7,13,14,18,19} (never s % 4 == 0: the watchers' balconies; never s % 6 == 3: the pressure habitats; never s % 12 == 10:
// the wall of divers), on the left s % 24 in {3,9,11,19,21} (odd: the patrol and the dive team ride even slots; not the stands',
// the wall's, the sled's, the crane's or the lights' slots). Every rule is certain (chance 1). Written by
// C:/ftwork/jobs/L6p3b/make_inl.py. Estimated pieces placed: 284 (no mesh more than 34 times).
Zone(TEXT("People p3b, scenes"), 0.f, 0.f);       // (the block starts with a Zone; it has no rules: every rule is in a district below)
#define P3B(n) TEXT("/Game/People/UnderTheSea/SM_P3B_" n)
auto P3BKm = [](float S) { return S * 0.045243f; };                // a slot is 45.243 m
auto P3BAt = [&](float S, int32 Side, const TCHAR* Mesh, float X0, float X1, float Sc)
{
	Zone(TEXT("People p3b: a scene"), P3BKm(S - 0.5f), P3BKm(S + 0.4f));
	Scatter(Mesh, X0, X1, 1, 0, 1.f, Sc, 0.f, Side, 3.f);
};
// ---- the reef (slots 1 to 37): 21 scenes
P3BAt(3, -1, P3B("Wedding"), 38.0f, 41.0f, 1.20f); P3BAt(4, 1, P3B("PearlMarket"), 38.0f, 41.0f, 1.20f);
P3BAt(7, -1, P3B("SeahorseRace"), 38.0f, 41.0f, 1.20f); P3BAt(9, 1, P3B("BubbleBand"), 38.0f, 41.0f, 1.20f);
P3BAt(11, -1, P3B("SchoolTrip"), 38.0f, 41.0f, 1.20f); P3BAt(12, 1, P3B("Wedding"), 38.0f, 41.0f, 1.20f);
P3BAt(13, -1, P3B("PearlMarket"), 38.0f, 41.0f, 1.20f); P3BAt(14, 1, P3B("SeahorseRace"), 38.0f, 41.0f, 1.20f);
P3BAt(21, -1, P3B("BubbleBand"), 38.0f, 41.0f, 1.20f); P3BAt(22, 1, P3B("SchoolTrip"), 38.0f, 41.0f, 1.20f);
P3BAt(23, -1, P3B("Wedding"), 38.0f, 41.0f, 1.20f); P3BAt(24, 1, P3B("PearlMarket"), 38.0f, 41.0f, 1.20f);
P3BAt(26, -1, P3B("SeahorseRace"), 38.0f, 41.0f, 1.20f); P3BAt(27, 1, P3B("BubbleBand"), 38.0f, 41.0f, 1.20f);
P3BAt(28, -1, P3B("SchoolTrip"), 38.0f, 41.0f, 1.20f); P3BAt(30, 1, P3B("Wedding"), 38.0f, 41.0f, 1.20f);
P3BAt(31, -1, P3B("PearlMarket"), 38.0f, 41.0f, 1.20f); P3BAt(32, 1, P3B("SeahorseRace"), 38.0f, 41.0f, 1.20f);
P3BAt(33, -1, P3B("BubbleBand"), 38.0f, 41.0f, 1.20f); P3BAt(34, 1, P3B("SchoolTrip"), 38.0f, 41.0f, 1.20f);
P3BAt(36, -1, P3B("Wedding"), 38.0f, 41.0f, 1.20f);
// ---- the kelp forest (slots 38 to 75): 10 scenes
P3BAt(44, 1, P3B("KelpHarvest"), 28.0f, 28.2f, 1.00f); P3BAt(45, -1, P3B("SeahorseRace"), 28.0f, 28.2f, 1.00f);
P3BAt(47, 1, P3B("SchoolTrip"), 28.0f, 28.2f, 1.00f); P3BAt(50, -1, P3B("KelpHarvest"), 28.0f, 28.2f, 1.00f);
P3BAt(52, 1, P3B("Wedding"), 28.0f, 28.2f, 1.00f); P3BAt(53, -1, P3B("KelpHarvest"), 28.0f, 28.2f, 1.00f);
P3BAt(61, -1, P3B("SeahorseRace"), 28.0f, 28.2f, 1.00f); P3BAt(66, 1, P3B("SchoolTrip"), 28.0f, 28.2f, 1.00f);
P3BAt(67, -1, P3B("KelpHarvest"), 28.0f, 28.2f, 1.00f); P3BAt(70, -1, P3B("Wedding"), 28.0f, 28.2f, 1.00f);
// ---- the research station (slots 76 to 110): 8 scenes
P3BAt(78, 1, P3B("RescueDrill"), 28.0f, 28.2f, 1.00f); P3BAt(84, 1, P3B("SubLaunch"), 28.0f, 28.2f, 1.00f);
P3BAt(87, 1, P3B("SchoolTrip"), 28.0f, 28.2f, 1.00f); P3BAt(91, 1, P3B("PearlMarket"), 28.0f, 28.2f, 1.00f);
P3BAt(93, 1, P3B("RescueDrill"), 28.0f, 28.2f, 1.00f); P3BAt(96, 1, P3B("SubLaunch"), 28.0f, 28.2f, 1.00f);
P3BAt(99, 1, P3B("SchoolTrip"), 28.0f, 28.2f, 1.00f); P3BAt(109, 1, P3B("PearlMarket"), 28.0f, 28.2f, 1.00f);
// ---- the salvage field (slots 111 to 146): 8 scenes
P3BAt(113, -1, P3B("TreasureHaul"), 38.0f, 41.0f, 1.20f); P3BAt(114, 1, P3B("StatueDig"), 38.0f, 41.0f, 1.20f);
P3BAt(116, -1, P3B("RescueDrill"), 38.0f, 41.0f, 1.20f); P3BAt(127, -1, P3B("TreasureHaul"), 38.0f, 41.0f, 1.20f);
P3BAt(132, 1, P3B("StatueDig"), 38.0f, 41.0f, 1.20f); P3BAt(133, -1, P3B("RescueDrill"), 38.0f, 41.0f, 1.20f);
P3BAt(137, -1, P3B("TreasureHaul"), 38.0f, 41.0f, 1.20f); P3BAt(138, 1, P3B("StatueDig"), 38.0f, 41.0f, 1.20f);
// ---- the approach (slots 147 to 175): 3 scenes
P3BAt(156, -1, P3B("BubbleBand"), 28.0f, 28.2f, 1.00f); P3BAt(162, -1, P3B("SubLaunch"), 28.0f, 28.2f, 1.00f);
P3BAt(166, 1, P3B("PearlMarket"), 28.0f, 28.2f, 1.00f);
// ---- Nereida (slot 180 to the end; 179 is the gate)
Zone(TEXT("People p3b: Nereida"), P3BKm(179.5f), 99.f);
	Scatter(P3B("SubLaunch"), 37.5f, 38.5f, 48, 47, 1.f, 1.2f, 0.f, 1, 3.f);          // right: s % 48 == 1
	Scatter(P3B("PearlMarket"), 37.5f, 38.5f, 48, 46, 1.f, 1.2f, 0.f, 1, 3.f);          // right: s % 48 == 2
	Scatter(P3B("StatueDig"), 37.5f, 38.5f, 48, 42, 1.f, 1.2f, 0.f, 1, 3.f);          // right: s % 48 == 6
	Scatter(P3B("BubbleBand"), 37.5f, 38.5f, 48, 41, 1.f, 1.2f, 0.f, 1, 3.f);          // right: s % 48 == 7
	Scatter(P3B("RescueDrill"), 37.5f, 38.5f, 48, 35, 1.f, 1.2f, 0.f, 1, 3.f);          // right: s % 48 == 13
	Scatter(P3B("Wedding"), 37.5f, 38.5f, 48, 34, 1.f, 1.2f, 0.f, 1, 3.f);          // right: s % 48 == 14
	Scatter(P3B("SchoolTrip"), 37.5f, 38.5f, 48, 30, 1.f, 1.2f, 0.f, 1, 3.f);          // right: s % 48 == 18
	Scatter(P3B("TreasureHaul"), 37.5f, 38.5f, 48, 29, 1.f, 1.2f, 0.f, 1, 3.f);          // right: s % 48 == 19
	Scatter(P3B("SeahorseRace"), 37.5f, 38.5f, 48, 23, 1.f, 1.2f, 0.f, 1, 3.f);          // right: s % 48 == 25
	Scatter(P3B("KelpHarvest"), 37.5f, 38.5f, 48, 22, 1.f, 1.2f, 0.f, 1, 3.f);          // right: s % 48 == 26
	Scatter(P3B("PearlMarket"), 37.5f, 38.5f, 48, 18, 1.f, 1.2f, 0.f, 1, 3.f);          // right: s % 48 == 30
	Scatter(P3B("SubLaunch"), 37.5f, 38.5f, 48, 17, 1.f, 1.2f, 0.f, 1, 3.f);          // right: s % 48 == 31
	Scatter(P3B("BubbleBand"), 37.5f, 38.5f, 48, 11, 1.f, 1.2f, 0.f, 1, 3.f);          // right: s % 48 == 37
	Scatter(P3B("RescueDrill"), 37.5f, 38.5f, 48, 10, 1.f, 1.2f, 0.f, 1, 3.f);          // right: s % 48 == 38
	Scatter(P3B("Wedding"), 37.5f, 38.5f, 48, 6, 1.f, 1.2f, 0.f, 1, 3.f);          // right: s % 48 == 42
	Scatter(P3B("StatueDig"), 37.5f, 38.5f, 48, 5, 1.f, 1.2f, 0.f, 1, 3.f);          // right: s % 48 == 43
	Scatter(P3B("SchoolTrip"), 28.0f, 28.2f, 96, 45, 1.f, 1.f, 0.f, -1, 2.f);   // left, before the tube: s % 96 == 51 (fix round: every 96, was 48)
	Scatter(P3B("RescueDrill"), 28.0f, 28.2f, 96, 93, 1.f, 1.f, 0.f, -1, 2.f);   // left, before the tube: s % 96 == 3 (fix round: every 96, was 48)
	Scatter(P3B("KelpHarvest"), 28.0f, 28.2f, 96, 37, 1.f, 1.f, 0.f, -1, 2.f);   // left, before the tube: s % 96 == 59 (fix round: every 96, was 48)
	Scatter(P3B("Wedding"), 28.0f, 28.2f, 96, 77, 1.f, 1.f, 0.f, -1, 2.f);   // left, before the tube: s % 96 == 19 (fix round: every 96, was 48)
	Scatter(P3B("TreasureHaul"), 28.0f, 28.2f, 96, 29, 1.f, 1.f, 0.f, -1, 2.f);   // left, before the tube: s % 96 == 67 (fix round: every 96, was 48)
	Scatter(P3B("SeahorseRace"), 28.0f, 28.2f, 96, 69, 1.f, 1.f, 0.f, -1, 2.f);   // left, before the tube: s % 96 == 27 (fix round: every 96, was 48)
	Scatter(P3B("PearlMarket"), 28.0f, 28.2f, 96, 13, 1.f, 1.f, 0.f, -1, 2.f);   // left, before the tube: s % 96 == 83 (fix round: every 96, was 48)
	Scatter(P3B("StatueDig"), 28.0f, 28.2f, 96, 61, 1.f, 1.f, 0.f, -1, 2.f);   // left, before the tube: s % 96 == 35 (fix round: every 96, was 48)
	Scatter(P3B("SubLaunch"), 28.0f, 28.2f, 96, 5, 1.f, 1.f, 0.f, -1, 2.f);   // left, before the tube: s % 96 == 91 (fix round: every 96, was 48)
	Scatter(P3B("BubbleBand"), 28.0f, 28.2f, 96, 53, 1.f, 1.f, 0.f, -1, 2.f);   // left, before the tube: s % 96 == 43 (fix round: every 96, was 48)
// ---- beside the glass tunnels: a scene every third slot on each side, seen through the glass
auto P3BPipe = [&](float A, float B, const TCHAR* MeshL, int32 PhL, const TCHAR* MeshR, int32 PhR)
{
	Zone(TEXT("People p3b: by a glass tunnel"), P3BKm(A - 0.5f), P3BKm(B + 0.5f));
	Far(MeshL, 32.f, 33.f, 3, PhL, 1.f, 1.1f, 0.f, -1, 0.9f, 3.f);
	Far(MeshR, 32.f, 33.f, 3, PhR, 1.f, 1.1f, 0.f, 1, 0.9f, 3.f);
};
P3BPipe(15, 20, P3B("Wedding"), 0, P3B("SchoolTrip"), 2); P3BPipe(37, 43, P3B("TreasureHaul"), 1, P3B("BubbleBand"), 0);
P3BPipe(79, 83, P3B("KelpHarvest"), 2, P3B("SubLaunch"), 1); P3BPipe(101, 108, P3B("StatueDig"), 0, P3B("PearlMarket"), 2);
P3BPipe(140, 146, P3B("RescueDrill"), 1, P3B("SeahorseRace"), 0); P3BPipe(167, 172, P3B("Wedding"), 2, P3B("SchoolTrip"), 1);
P3BPipe(310, 314, P3B("TreasureHaul"), 0, P3B("BubbleBand"), 2); P3BPipe(331, 335, P3B("KelpHarvest"), 1, P3B("SubLaunch"), 0);
P3BPipe(352, 356, P3B("StatueDig"), 2, P3B("PearlMarket"), 1); P3BPipe(373, 377, P3B("RescueDrill"), 0, P3B("SeahorseRace"), 2);
P3BPipe(394, 398, P3B("Wedding"), 1, P3B("SchoolTrip"), 0); P3BPipe(414, 419, P3B("TreasureHaul"), 2, P3B("BubbleBand"), 1);
P3BPipe(435, 440, P3B("KelpHarvest"), 0, P3B("SubLaunch"), 2); P3BPipe(456, 461, P3B("StatueDig"), 1, P3B("PearlMarket"), 0);
P3BPipe(477, 482, P3B("RescueDrill"), 2, P3B("SeahorseRace"), 1); P3BPipe(498, 503, P3B("Wedding"), 0, P3B("SchoolTrip"), 2);
P3BPipe(519, 523, P3B("TreasureHaul"), 1, P3B("BubbleBand"), 0); P3BPipe(540, 544, P3B("KelpHarvest"), 2, P3B("SubLaunch"), 1);
#undef P3B
// ---- END p3b ----

}

{
// ---- BEGIN p3c (movers, ft_people_sea_p3c.py) ----
// FIX ROUND: the turtle wagon 1.8 m further out and the kelp forest's files 0.2 to 0.6 m (their saved bounds stood inside 24 m and the track pushed them); the scooter files of the
// reef, the kelp forest and the station's right moved off the slots of p3f's units; two rules in front of the station's tube taken out. Lines marked "fix round".
// THE MOVERS (pass 3, builder p3c): everything here TRAVELS along a path of its own that is laid along the road and comes
// round again, or goes round a ring. Eleven meshes, /Game/People/UnderTheSea/SM_P3C_*, all in the water 3 to 16 m over the
// sand and so all TALL: no rule here has an x min under 26 m, and each stands far enough out that nothing of it is
// nearer than 24 m (half depths as placed: scooters 1.4 m, bikes 1.4, sleds 1.9, the race 2.3, the catwalk 1.9, the
// wagon's turtle 3.9). Nothing crosses the road: a travel path is the piece's own X (no yaw jitter anywhere), and the
// rings (17.6 and 19.5 m across as built) stand 76 m and more out. Nothing glows, nothing blinks.
//   26 to 31 m    SM_P3C_Scooters_A/_B (files towed by scooters), SM_P3C_AquaBikes (the cyclists), SM_P3C_SledTrain,
//                 SM_P3C_TurtleWagon, SM_P3C_Walkway (a catwalk between two airlock booths), SM_P3C_FinRace
//   58 to 132 m   SM_P3C_ManateeHerd, SM_P3C_CableCar (52 m between its pylons), SM_P3C_RidingRing, SM_P3C_BuoyLaps
// The near band is 28.8 to 30.5 m: behind the first passes' cast (25.5 to 27.5 m), in front of the balconies and the
// habitats (33 m and out). Every rule has a chance of 1, one side, and slots of its own: (s + phase) % every == 0; on one
// side of one district no two near rules (and no two middle rules) share a slot. The slots are chosen against the
// pressure habitats as the table above does. The scale is part of the design, as above.
#define P3C(n) TEXT("/Game/People/UnderTheSea/SM_P3C_" n)
Zone(TEXT("People p3c: the reef"), 0.3f, 1.7f);          // scooter files, cyclists, wagons, sled trains and catwalks behind the dive teams; herds, the cable car and the rings out on the sand
	Scatter(P3C("Scooters_A"), 28.8f, 30.5f, 6, 1, 1.f, 1.4f, 0.f, 1, 0.f);   // s % 6 == 5 (fix round: was 0, the slots of p3f's make-up tent and extras)
	Scatter(P3C("AquaBikes"), 28.8f, 30.5f, 12, 10, 1.f, 1.5f, 0.f, 1, 0.f);          // s % 12 == 2
	Scatter(P3C("TurtleWagon"), 30.6f, 32.3f, 12, 8, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 12 == 4 (fix round: 1.8 m further out, its bounds stood inside 24 m)
	Scatter(P3C("Walkway"), 28.8f, 30.5f, 12, 2, 1.f, 1.25f, 0.f, 1, 0.f);          // s % 12 == 10
	Scatter(P3C("FinRace"), 28.8f, 30.5f, 12, 11, 1.f, 1.25f, 0.f, 1, 0.f);          // s % 12 == 1
	Scatter(P3C("SledTrain"), 28.8f, 30.5f, 12, 5, 1.f, 1.4f, 0.f, 1, 0.f);          // s % 12 == 7
	Scatter(P3C("Scooters_B"), 28.8f, 30.5f, 12, 4, 1.f, 1.4f, 0.f, -1, 0.f);   // s % 12 == 8 (fix round: was s % 6 == 3, the slots of p3f's press pen and make-up tent)
	Scatter(P3C("SledTrain"), 28.8f, 30.5f, 12, 7, 1.f, 1.4f, 0.f, -1, 0.f);          // s % 12 == 5
	Scatter(P3C("Walkway"), 28.8f, 30.5f, 12, 1, 1.f, 1.25f, 0.f, -1, 0.f);          // s % 12 == 11
	Scatter(P3C("AquaBikes"), 28.8f, 30.5f, 12, 11, 1.f, 1.5f, 0.f, -1, 0.f);          // s % 12 == 1
	Scatter(P3C("FinRace"), 28.8f, 30.5f, 12, 5, 1.f, 1.25f, 0.f, -1, 0.f);          // s % 12 == 7
	Scatter(P3C("TurtleWagon"), 30.6f, 32.3f, 12, 8, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 12 == 4 (fix round: 1.8 m further out, its bounds stood inside 24 m)
	Scatter(P3C("ManateeHerd"), 64.f, 100.f, 12, 10, 1.f, 1.5f, 0.f, 1, 0.f);          // s % 12 == 2
	Scatter(P3C("ManateeHerd"), 64.f, 100.f, 12, 4, 1.f, 1.5f, 0.f, -1, 0.f);          // s % 12 == 8
	Scatter(P3C("CableCar"), 70.f, 100.f, 12, 7, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 12 == 5
	Scatter(P3C("CableCar"), 70.f, 100.f, 12, 1, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 12 == 11
	Scatter(P3C("RidingRing"), 76.f, 96.f, 12, 4, 1.f, 1.4f, 0.f, 1, 0.f);          // s % 12 == 8
	Scatter(P3C("RidingRing"), 76.f, 96.f, 12, 10, 1.f, 1.4f, 0.f, -1, 0.f);          // s % 12 == 2
	Scatter(P3C("BuoyLaps"), 76.f, 100.f, 12, 1, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 12 == 11
	Scatter(P3C("BuoyLaps"), 76.f, 100.f, 12, 7, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 12 == 5
Zone(TEXT("People p3c: the kelp forest"), 1.7f, 3.4f);          // the weed stands from 27 m: only thin files, in front of it (never the slots of the wall of divers); the rest between the stands and the far weed
	Scatter(P3C("Scooters_B"), 26.8f, 27.4f, 6, 5, 1.f, 1.3f, 0.f, -1, 0.f);   // s % 6 == 1 (fix round: was 2, the slots of p3f's units; 0.6 m further out)
	Scatter(P3C("AquaBikes"), 26.4f, 27.0f, 12, 8, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 12 == 4
	Scatter(P3C("Scooters_A"), 26.8f, 27.4f, 6, 4, 1.f, 1.3f, 0.f, 1, 0.f);   // s % 6 == 2 (fix round: was 1, the slots of p3f's units; 0.6 m further out)
	Scatter(P3C("AquaBikes"), 26.4f, 27.0f, 12, 7, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 12 == 5
	Scatter(P3C("ManateeHerd"), 76.f, 100.f, 12, 10, 1.f, 1.5f, 0.f, 1, 0.f);          // s % 12 == 2
	Scatter(P3C("ManateeHerd"), 76.f, 100.f, 12, 4, 1.f, 1.5f, 0.f, -1, 0.f);          // s % 12 == 8
	Scatter(P3C("CableCar"), 80.f, 96.f, 12, 7, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 12 == 5
	Scatter(P3C("CableCar"), 80.f, 96.f, 12, 1, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 12 == 11
	Scatter(P3C("RidingRing"), 84.f, 92.f, 12, 4, 1.f, 1.4f, 0.f, 1, 0.f);          // s % 12 == 8
	Scatter(P3C("RidingRing"), 84.f, 92.f, 12, 10, 1.f, 1.4f, 0.f, -1, 0.f);          // s % 12 == 2
	Scatter(P3C("BuoyLaps"), 84.f, 92.f, 12, 1, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 12 == 11
	Scatter(P3C("BuoyLaps"), 84.f, 92.f, 12, 7, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 12 == 5
Zone(TEXT("People p3c: the research station"), 3.4f, 5.f);          // left: even slots only (a pressure habitat stands in every odd one); right: in front of the travel tube, and no wagon there (its turtle is 7 m across)
	Scatter(P3C("SledTrain"), 28.8f, 30.5f, 12, 10, 1.f, 1.4f, 0.f, -1, 0.f);          // s % 12 == 2
	Scatter(P3C("Scooters_B"), 28.8f, 30.5f, 6, 2, 1.f, 1.4f, 0.f, -1, 0.f);          // s % 6 == 4
	Scatter(P3C("Walkway"), 28.8f, 30.5f, 12, 6, 1.f, 1.25f, 0.f, -1, 0.f);          // s % 12 == 6
	Scatter(P3C("TurtleWagon"), 30.6f, 32.3f, 12, 4, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 12 == 8 (fix round: 1.8 m further out, its bounds stood inside 24 m)
	Scatter(P3C("AquaBikes"), 28.8f, 30.5f, 12, 0, 1.f, 1.5f, 0.f, -1, 0.f);          // s % 12 == 0
	Scatter(P3C("Scooters_A"), 27.8f, 29.2f, 6, 2, 1.f, 1.4f, 0.f, 1, 0.f);   // s % 6 == 4 (fix round: was 0, the slots of p3f's second unit and press pen)
	Scatter(P3C("Walkway"), 27.8f, 29.2f, 12, 11, 1.f, 1.25f, 0.f, 1, 0.f);          // s % 12 == 1
	Scatter(P3C("SledTrain"), 27.8f, 29.2f, 12, 5, 1.f, 1.4f, 0.f, 1, 0.f);          // s % 12 == 7
	// (fix round) taken out, its slot holds p3f's drone team and a p3b scene in front of the tube: Scatter(P3C("AquaBikes"), 27.8f, 29.2f, 12, 9, 1.f, 1.5f, 0.f, 1, 0.f);          // s % 12 == 3
	// (fix round) taken out, its slot holds p3f's cable camera and a p3b scene in front of the tube: Scatter(P3C("FinRace"), 27.8f, 29.2f, 12, 3, 1.f, 1.25f, 0.f, 1, 0.f);          // s % 12 == 9
	Scatter(P3C("ManateeHerd"), 58.f, 70.f, 12, 10, 1.f, 1.5f, 0.f, 1, 0.f);          // s % 12 == 2
	Scatter(P3C("ManateeHerd"), 58.f, 70.f, 12, 4, 1.f, 1.5f, 0.f, -1, 0.f);          // s % 12 == 8
	Scatter(P3C("CableCar"), 102.f, 130.f, 12, 7, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 12 == 5
	Scatter(P3C("CableCar"), 102.f, 130.f, 12, 1, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 12 == 11
	Scatter(P3C("RidingRing"), 104.f, 126.f, 12, 4, 1.f, 1.4f, 0.f, 1, 0.f);          // s % 12 == 8
	Scatter(P3C("RidingRing"), 104.f, 126.f, 12, 10, 1.f, 1.4f, 0.f, -1, 0.f);          // s % 12 == 2
	Scatter(P3C("BuoyLaps"), 104.f, 130.f, 12, 1, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 12 == 11
	Scatter(P3C("BuoyLaps"), 104.f, 130.f, 12, 7, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 12 == 5
Zone(TEXT("People p3c: the salvage field"), 5.f, 6.6f);          // no near rule takes a slot s % 6 == 4 (the pressure habitats)
	Scatter(P3C("Scooters_B"), 28.8f, 30.5f, 6, 5, 1.f, 1.4f, 0.f, -1, 0.f);          // s % 6 == 1
	Scatter(P3C("SledTrain"), 28.8f, 30.5f, 12, 10, 1.f, 1.4f, 0.f, -1, 0.f);          // s % 12 == 2
	Scatter(P3C("TurtleWagon"), 30.6f, 32.3f, 12, 4, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 12 == 8 (fix round: 1.8 m further out, its bounds stood inside 24 m)
	Scatter(P3C("AquaBikes"), 28.8f, 30.5f, 12, 9, 1.f, 1.5f, 0.f, -1, 0.f);          // s % 12 == 3
	Scatter(P3C("Walkway"), 28.8f, 30.5f, 12, 3, 1.f, 1.25f, 0.f, -1, 0.f);          // s % 12 == 9
	Scatter(P3C("FinRace"), 28.8f, 30.5f, 12, 7, 1.f, 1.25f, 0.f, -1, 0.f);          // s % 12 == 5
	Scatter(P3C("Scooters_A"), 28.8f, 30.5f, 6, 0, 1.f, 1.4f, 0.f, 1, 0.f);          // s % 6 == 0
	Scatter(P3C("Walkway"), 28.8f, 30.5f, 12, 11, 1.f, 1.25f, 0.f, 1, 0.f);          // s % 12 == 1
	Scatter(P3C("SledTrain"), 28.8f, 30.5f, 12, 5, 1.f, 1.4f, 0.f, 1, 0.f);          // s % 12 == 7
	Scatter(P3C("AquaBikes"), 28.8f, 30.5f, 12, 10, 1.f, 1.5f, 0.f, 1, 0.f);          // s % 12 == 2
	Scatter(P3C("FinRace"), 28.8f, 30.5f, 12, 4, 1.f, 1.25f, 0.f, 1, 0.f);          // s % 12 == 8
	Scatter(P3C("TurtleWagon"), 30.6f, 32.3f, 12, 7, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 12 == 5 (fix round: 1.8 m further out, its bounds stood inside 24 m)
	Scatter(P3C("ManateeHerd"), 62.f, 100.f, 12, 10, 1.f, 1.5f, 0.f, 1, 0.f);          // s % 12 == 2
	Scatter(P3C("ManateeHerd"), 62.f, 100.f, 12, 4, 1.f, 1.5f, 0.f, -1, 0.f);          // s % 12 == 8
	Scatter(P3C("CableCar"), 70.f, 110.f, 12, 7, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 12 == 5
	Scatter(P3C("CableCar"), 70.f, 110.f, 12, 1, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 12 == 11
	Scatter(P3C("RidingRing"), 76.f, 104.f, 12, 4, 1.f, 1.4f, 0.f, 1, 0.f);          // s % 12 == 8
	Scatter(P3C("RidingRing"), 76.f, 104.f, 12, 10, 1.f, 1.4f, 0.f, -1, 0.f);          // s % 12 == 2
	Scatter(P3C("BuoyLaps"), 76.f, 110.f, 12, 1, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 12 == 11
	Scatter(P3C("BuoyLaps"), 76.f, 110.f, 12, 7, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 12 == 5
Zone(TEXT("People p3c: the approach"), 6.6f, 7.94f);          // no near rule takes a slot s % 3 == 2 (the pressure habitats)
	Scatter(P3C("Scooters_B"), 28.8f, 30.5f, 6, 0, 1.f, 1.4f, 0.f, -1, 0.f);          // s % 6 == 0
	Scatter(P3C("AquaBikes"), 28.8f, 30.5f, 12, 9, 1.f, 1.5f, 0.f, -1, 0.f);          // s % 12 == 3
	Scatter(P3C("SledTrain"), 28.8f, 30.5f, 12, 3, 1.f, 1.4f, 0.f, -1, 0.f);          // s % 12 == 9
	Scatter(P3C("Walkway"), 28.8f, 30.5f, 12, 8, 1.f, 1.25f, 0.f, -1, 0.f);          // s % 12 == 4
	Scatter(P3C("TurtleWagon"), 30.6f, 32.3f, 12, 2, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 12 == 10 (fix round: 1.8 m further out, its bounds stood inside 24 m)
	Scatter(P3C("FinRace"), 28.8f, 30.5f, 12, 11, 1.f, 1.25f, 0.f, -1, 0.f);          // s % 12 == 1
	Scatter(P3C("Scooters_A"), 28.8f, 30.5f, 6, 5, 1.f, 1.4f, 0.f, 1, 0.f);          // s % 6 == 1
	Scatter(P3C("FinRace"), 28.8f, 30.5f, 12, 8, 1.f, 1.25f, 0.f, 1, 0.f);          // s % 12 == 4
	Scatter(P3C("SledTrain"), 28.8f, 30.5f, 12, 2, 1.f, 1.4f, 0.f, 1, 0.f);          // s % 12 == 10
	Scatter(P3C("TurtleWagon"), 30.6f, 32.3f, 12, 9, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 12 == 3 (fix round: 1.8 m further out, its bounds stood inside 24 m)
	Scatter(P3C("Walkway"), 28.8f, 30.5f, 12, 3, 1.f, 1.25f, 0.f, 1, 0.f);          // s % 12 == 9
	Scatter(P3C("AquaBikes"), 28.8f, 30.5f, 12, 0, 1.f, 1.5f, 0.f, 1, 0.f);          // s % 12 == 0
	Scatter(P3C("ManateeHerd"), 62.f, 100.f, 12, 10, 1.f, 1.5f, 0.f, 1, 0.f);          // s % 12 == 2
	Scatter(P3C("ManateeHerd"), 62.f, 100.f, 12, 4, 1.f, 1.5f, 0.f, -1, 0.f);          // s % 12 == 8
	Scatter(P3C("CableCar"), 70.f, 100.f, 12, 7, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 12 == 5
	Scatter(P3C("CableCar"), 70.f, 100.f, 12, 1, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 12 == 11
	Scatter(P3C("RidingRing"), 76.f, 96.f, 12, 4, 1.f, 1.4f, 0.f, 1, 0.f);          // s % 12 == 8
	Scatter(P3C("RidingRing"), 76.f, 96.f, 12, 10, 1.f, 1.4f, 0.f, -1, 0.f);          // s % 12 == 2
	Scatter(P3C("BuoyLaps"), 76.f, 100.f, 12, 1, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 12 == 11
	Scatter(P3C("BuoyLaps"), 76.f, 100.f, 12, 7, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 12 == 5
Zone(TEXT("People p3c: Nereida"), 7.94f, 99.f);          // right: never a slot s % 6 == 3 (the pressure habitats); left: in front of the travel tube, no wagon; herds between the watch towers and the mini-subs, the rest beyond them
	Scatter(P3C("Scooters_A"), 28.8f, 30.5f, 30, 0, 1.f, 1.4f, 0.f, 1, 0.f);          // s % 30 == 0
	Scatter(P3C("AquaBikes"), 28.8f, 30.5f, 30, 26, 1.f, 1.5f, 0.f, 1, 0.f);          // s % 30 == 4
	Scatter(P3C("SledTrain"), 28.8f, 30.5f, 30, 22, 1.f, 1.4f, 0.f, 1, 0.f);          // s % 30 == 8
	Scatter(P3C("Scooters_B"), 28.8f, 30.5f, 60, 50, 1.f, 1.4f, 0.f, 1, 0.f);          // s % 60 == 10
	Scatter(P3C("TurtleWagon"), 30.6f, 32.3f, 30, 16, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 30 == 14 (fix round: was every 15, half of them in the slots of p3f's kit rostrums and p3a's kelp farmers; 1.8 m further out, its bounds stood inside 24 m)
	Scatter(P3C("Walkway"), 28.8f, 30.5f, 30, 10, 1.f, 1.25f, 0.f, 1, 0.f);          // s % 30 == 20
	Scatter(P3C("FinRace"), 28.8f, 30.5f, 30, 4, 1.f, 1.25f, 0.f, 1, 0.f);          // s % 30 == 26
	Scatter(P3C("Scooters_B"), 27.8f, 29.2f, 30, 29, 1.f, 1.4f, 0.f, -1, 0.f);          // s % 30 == 1
	Scatter(P3C("FinRace"), 27.8f, 29.2f, 30, 25, 1.f, 1.25f, 0.f, -1, 0.f);          // s % 30 == 5
	Scatter(P3C("AquaBikes"), 27.8f, 29.2f, 45, 36, 1.f, 1.5f, 0.f, -1, 0.f);          // s % 45 == 9
	Scatter(P3C("Walkway"), 27.8f, 29.2f, 30, 17, 1.f, 1.25f, 0.f, -1, 0.f);          // s % 30 == 13
	Scatter(P3C("SledTrain"), 27.8f, 29.2f, 30, 13, 1.f, 1.4f, 0.f, -1, 0.f);          // s % 30 == 17
	Scatter(P3C("Scooters_A"), 27.8f, 29.2f, 60, 35, 1.f, 1.4f, 0.f, -1, 0.f);          // s % 60 == 25
	Scatter(P3C("ManateeHerd"), 64.f, 80.f, 36, 34, 1.f, 1.5f, 0.f, 1, 0.f);          // s % 36 == 2
	Scatter(P3C("ManateeHerd"), 64.f, 80.f, 36, 16, 1.f, 1.5f, 0.f, -1, 0.f);          // s % 36 == 20
	Scatter(P3C("CableCar"), 102.f, 132.f, 28, 19, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 28 == 9
	Scatter(P3C("CableCar"), 102.f, 132.f, 28, 5, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 28 == 23
	Scatter(P3C("RidingRing"), 104.f, 130.f, 28, 8, 1.f, 1.4f, 0.f, 1, 0.f);          // s % 28 == 20
	Scatter(P3C("RidingRing"), 104.f, 130.f, 28, 22, 1.f, 1.4f, 0.f, -1, 0.f);          // s % 28 == 6
	Scatter(P3C("BuoyLaps"), 104.f, 132.f, 28, 1, 1.f, 1.3f, 0.f, 1, 0.f);          // s % 28 == 27
	Scatter(P3C("BuoyLaps"), 104.f, 132.f, 28, 15, 1.f, 1.3f, 0.f, -1, 0.f);          // s % 28 == 13
// ---- by the glass tunnels (Scatter rules take no slot of a tunnel: Far rules do, the piece stood on the sand by its drop,
// as the table above does). Seen through the glass: files of scooters and cyclists and a sled train 34 to 37 m out,
// behind the swimmers of the first pass (28 to 32 m); a herd of sea cows beyond.
auto PplP3CPipe = [&](float A, float B, int32 Ph)
{
	Zone(TEXT("People p3c: by a glass tunnel"), (A - 0.5f) * 0.045243f, (B + 0.5f) * 0.045243f);
	Far(P3C("Scooters_A"), 34.f, 37.f, 12, Ph + 0, 1.f, 1.5f, 0.f, 1, 0.9f, 0.f);
	Far(P3C("Scooters_B"), 34.f, 37.f, 12, Ph + 6, 1.f, 1.5f, 0.f, -1, 0.9f, 0.f);
	Far(P3C("AquaBikes"), 34.f, 37.f, 12, Ph + 1, 1.f, 1.5f, 0.f, -1, 0.9f, 0.f);
	Far(P3C("SledTrain"), 34.f, 37.f, 8, Ph + 5, 1.f, 1.4f, 0.f, 1, 0.9f, 0.f);
	Far(P3C("ManateeHerd"), 66.f, 90.f, 24, Ph + 2, 1.f, 1.6f, 0.f, 1, 1.f, 0.f);
	Far(P3C("ManateeHerd"), 66.f, 90.f, 24, Ph + 14, 1.f, 1.6f, 0.f, -1, 1.f, 0.f);
};
PplP3CPipe(15, 20, 0); PplP3CPipe(37, 43, 1); PplP3CPipe(79, 83, 0); PplP3CPipe(101, 108, 1); PplP3CPipe(140, 146, 0); PplP3CPipe(167, 172, 1); PplP3CPipe(310, 314, 0); PplP3CPipe(331, 335, 1); PplP3CPipe(352, 356, 0);
PplP3CPipe(373, 377, 1); PplP3CPipe(394, 398, 0); PplP3CPipe(414, 419, 1); PplP3CPipe(435, 440, 0); PplP3CPipe(456, 461, 1); PplP3CPipe(477, 482, 0); PplP3CPipe(498, 503, 1); PplP3CPipe(519, 523, 0); PplP3CPipe(540, 544, 1);
#undef P3C
// ---- END p3c ----

}

{
// ---- BEGIN p3d (heights, ft_people_sea_p3d.py) ----
// PEOPLE UP HIGH (pass 3, builder d). Ten meshes, /Game/People/UnderTheSea/SM_P3D_*, each a crowd brought with what it
// stands on: Gantry_A / _B (a bridge between two towers, 7.2 m up), Mast (a lookout tub at 9.2 m), Buoy (a deck under a
// float, 6.4 m), Spire (a rock spire, ledges at 3.8, 6.8, 9.8 m), WreckMast (divers along a wreck's yards), Crane (a
// salvage crane, two riding its load), Terraces (a stepped block, three roofs), RopeBridge (a plank bridge between two
// stacks, 6.6 m), Lift (a cage riding between two landings). All TALL: nothing of this block stands nearer than 46 m.
// Steady light only (windows and lamps, 46 m out and more); nothing blinks.
// Where: 46 to 70 m, behind the street cast, the crew and the watchers' stands (17 to 48 m), in the slots the
// dressing's own sites at these distances leave free (worked out per district from FTDressL6.inl; (s + phase) % every == 0):
//   reef      left odd slots (the reef gardens stand at 68 m in the even ones), right even slots
//   kelp      46 to 50 m, in front of the kelp stands' middles
//   station   left s%4 == 2 and right s%4 == 0 at 50 to 56 m (habitats, trenches and aprons have the others); 66 to 70 m in s%4 == 3 / 1
//   salvage   left s%3 != 0, right s%3 != 2 (the salvage frames and their umbilicals have those)
//   approach  left s%4 == 1 and 2, right even slots (the vents stand at 40 to 44 m, the guard towers at 66 and 70 m)
//   Nereida   right s%12 in {4, 7, 8}, left s%12 in {1, 5, 7, 10, 11} (guards, habitats, kelp stands, aprons have the rest);
//             every 60 with five meshes in turn, so that a mesh comes back on a side only every 2.7 km
// By the glass tunnels (Far rules, stood on the sand by their drop like the table's own tunnel people): 46 to 50 m.
#define P3D(n) TEXT("/Game/People/UnderTheSea/SM_P3D_" n)
Zone(TEXT("People p3d: the reef"), 0.0f, 1.7f);   // spires, plank bridges, float decks, lookouts over the coral
	Scatter(P3D("Spire"), 48.0f, 54.0f, 8, 7, 1.f, 1.2f, 0.06f, -1, 6.0f);
	Scatter(P3D("RopeBridge"), 48.0f, 54.0f, 8, 5, 1.f, 1.2f, 0.06f, -1, 6.0f);
	Scatter(P3D("Buoy"), 48.0f, 54.0f, 8, 3, 1.f, 1.2f, 0.06f, -1, 6.0f);
	Scatter(P3D("Mast"), 48.0f, 54.0f, 8, 1, 1.f, 1.2f, 0.06f, -1, 6.0f);
	Scatter(P3D("RopeBridge"), 50.0f, 56.0f, 8, 0, 1.f, 1.2f, 0.06f, 1, 6.0f);
	Scatter(P3D("Buoy"), 50.0f, 56.0f, 8, 6, 1.f, 1.2f, 0.06f, 1, 6.0f);
	Scatter(P3D("Spire"), 50.0f, 56.0f, 8, 4, 1.f, 1.2f, 0.06f, 1, 6.0f);
	Scatter(P3D("Gantry_A"), 50.0f, 56.0f, 8, 2, 1.f, 1.2f, 0.06f, 1, 6.0f);
Zone(TEXT("People p3d: the kelp forest"), 1.7f, 3.4f);   // masts and bridges standing out of the weed
	Scatter(P3D("Mast"), 46.0f, 50.0f, 8, 0, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("RopeBridge"), 46.0f, 50.0f, 8, 5, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("WreckMast"), 46.0f, 50.0f, 8, 2, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Buoy"), 46.0f, 50.0f, 8, 7, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("Mast"), 46.0f, 50.0f, 8, 4, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("Spire"), 46.0f, 50.0f, 8, 1, 1.f, 1.3f, 0.06f, 1, 6.0f);
Zone(TEXT("People p3d: the research station"), 3.4f, 5.0f);   // the station at work: gantries, lifts, cranes, roof terraces
	Scatter(P3D("Gantry_A"), 50.0f, 54.0f, 8, 6, 1.f, 1.2f, 0.06f, -1, 6.0f);
	Scatter(P3D("Lift"), 50.0f, 54.0f, 8, 2, 1.f, 1.2f, 0.06f, -1, 6.0f);
	Scatter(P3D("Terraces"), 52.0f, 56.0f, 8, 0, 1.f, 1.2f, 0.06f, 1, 6.0f);
	Scatter(P3D("Gantry_B"), 52.0f, 56.0f, 8, 4, 1.f, 1.2f, 0.06f, 1, 6.0f);
	Scatter(P3D("Crane"), 66.0f, 70.0f, 8, 5, 1.f, 1.4f, 0.06f, -1, 6.0f);
	Scatter(P3D("Terraces"), 66.0f, 70.0f, 8, 1, 1.f, 1.4f, 0.06f, -1, 6.0f);
	Scatter(P3D("Lift"), 66.0f, 70.0f, 8, 7, 1.f, 1.4f, 0.06f, 1, 6.0f);
	Scatter(P3D("Crane"), 66.0f, 70.0f, 8, 3, 1.f, 1.4f, 0.06f, 1, 6.0f);
Zone(TEXT("People p3d: the salvage field"), 5.0f, 6.6f);   // the wrecks: masts with divers on the yards, cranes, lifts
	Scatter(P3D("WreckMast"), 52.0f, 58.0f, 6, 5, 1.f, 1.25f, 0.06f, -1, 6.0f);
	Scatter(P3D("Crane"), 52.0f, 58.0f, 6, 4, 1.f, 1.25f, 0.06f, -1, 6.0f);
	Scatter(P3D("Gantry_B"), 52.0f, 58.0f, 6, 2, 1.f, 1.25f, 0.06f, -1, 6.0f);
	Scatter(P3D("Crane"), 52.0f, 58.0f, 6, 0, 1.f, 1.25f, 0.06f, 1, 6.0f);
	Scatter(P3D("WreckMast"), 52.0f, 58.0f, 6, 3, 1.f, 1.25f, 0.06f, 1, 6.0f);
	Scatter(P3D("Lift"), 52.0f, 58.0f, 6, 2, 1.f, 1.25f, 0.06f, 1, 6.0f);
Zone(TEXT("People p3d: the approach"), 6.6f, 7.94f);   // the outworks of Nereida: terraces, float decks, lifts
	Scatter(P3D("Terraces"), 54.0f, 60.0f, 8, 7, 1.f, 1.25f, 0.06f, -1, 6.0f);
	Scatter(P3D("Buoy"), 54.0f, 60.0f, 8, 3, 1.f, 1.25f, 0.06f, -1, 6.0f);
	Scatter(P3D("Lift"), 58.0f, 62.0f, 8, 6, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Gantry_B"), 58.0f, 62.0f, 8, 2, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Gantry_A"), 54.0f, 60.0f, 8, 0, 1.f, 1.25f, 0.06f, 1, 6.0f);
	Scatter(P3D("Terraces"), 54.0f, 60.0f, 8, 4, 1.f, 1.25f, 0.06f, 1, 6.0f);
	Scatter(P3D("Spire"), 54.0f, 60.0f, 8, 6, 1.f, 1.25f, 0.06f, 1, 6.0f);
	Scatter(P3D("Mast"), 54.0f, 60.0f, 8, 2, 1.f, 1.25f, 0.06f, 1, 6.0f);
Zone(TEXT("People p3d: Nereida"), 7.94f, 99.0f);   // the city has climbed onto everything it has: both sides, all the way through the fight
	Scatter(P3D("Terraces"), 52.0f, 58.0f, 60, 56, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("Lift"), 52.0f, 58.0f, 60, 44, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("Crane"), 52.0f, 58.0f, 60, 32, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("Gantry_B"), 52.0f, 58.0f, 60, 20, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("RopeBridge"), 52.0f, 58.0f, 60, 8, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("Buoy"), 52.0f, 58.0f, 60, 53, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("Mast"), 52.0f, 58.0f, 60, 41, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("Spire"), 52.0f, 58.0f, 60, 29, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("WreckMast"), 52.0f, 58.0f, 60, 17, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("Gantry_A"), 52.0f, 58.0f, 60, 5, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("Gantry_B"), 52.0f, 58.0f, 60, 52, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("RopeBridge"), 52.0f, 58.0f, 60, 40, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("Terraces"), 52.0f, 58.0f, 60, 28, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("Lift"), 52.0f, 58.0f, 60, 16, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("Crane"), 52.0f, 58.0f, 60, 4, 1.f, 1.3f, 0.06f, 1, 6.0f);
	Scatter(P3D("Terraces"), 54.0f, 60.0f, 60, 59, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Lift"), 54.0f, 60.0f, 60, 47, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Crane"), 54.0f, 60.0f, 60, 35, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Gantry_B"), 54.0f, 60.0f, 60, 23, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("RopeBridge"), 54.0f, 60.0f, 60, 11, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Buoy"), 54.0f, 60.0f, 60, 55, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Mast"), 54.0f, 60.0f, 60, 43, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Spire"), 54.0f, 60.0f, 60, 31, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("WreckMast"), 54.0f, 60.0f, 60, 19, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Gantry_A"), 54.0f, 60.0f, 60, 7, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Gantry_B"), 54.0f, 60.0f, 60, 53, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("RopeBridge"), 54.0f, 60.0f, 60, 41, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Terraces"), 54.0f, 60.0f, 60, 29, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Lift"), 54.0f, 60.0f, 60, 17, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Crane"), 54.0f, 60.0f, 60, 5, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("WreckMast"), 54.0f, 60.0f, 60, 50, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Gantry_A"), 54.0f, 60.0f, 60, 38, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Buoy"), 54.0f, 60.0f, 60, 26, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Mast"), 54.0f, 60.0f, 60, 14, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Spire"), 54.0f, 60.0f, 60, 2, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Lift"), 54.0f, 60.0f, 60, 49, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Crane"), 54.0f, 60.0f, 60, 37, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Gantry_B"), 54.0f, 60.0f, 60, 25, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("RopeBridge"), 54.0f, 60.0f, 60, 13, 1.f, 1.3f, 0.06f, -1, 6.0f);
	Scatter(P3D("Terraces"), 54.0f, 60.0f, 60, 1, 1.f, 1.3f, 0.06f, -1, 6.0f);
// ---- by the glass tunnels: seen through the glass, high over the low pieces in front of them
auto PplP3DPipe = [&](float A, float B, const TCHAR* L0, const TCHAR* L1, const TCHAR* R0, const TCHAR* R1)
{
	Zone(TEXT("People p3d: by a glass tunnel"), (A - 0.5f) * 0.045243f, (B + 0.5f) * 0.045243f);
	Far(L0, 46.f, 50.f, 6, 0, 1.f, 1.3f, 0.06f, -1, 0.9f, 6.f);
	Far(L1, 46.f, 50.f, 6, 3, 1.f, 1.3f, 0.06f, -1, 0.9f, 6.f);
	Far(R0, 46.f, 50.f, 6, 1, 1.f, 1.3f, 0.06f, 1, 0.9f, 6.f);
	Far(R1, 46.f, 50.f, 6, 4, 1.f, 1.3f, 0.06f, 1, 0.9f, 6.f);
};
PplP3DPipe(15.f, 20.f, P3D("Spire"), P3D("Buoy"), P3D("RopeBridge"), P3D("Mast")); PplP3DPipe(37.f, 43.f, P3D("Mast"), P3D("WreckMast"), P3D("Spire"), P3D("RopeBridge"));
PplP3DPipe(79.f, 83.f, P3D("Gantry_A"), P3D("Lift"), P3D("Terraces"), P3D("Crane")); PplP3DPipe(101.f, 108.f, P3D("Crane"), P3D("Gantry_B"), P3D("Lift"), P3D("Gantry_A"));
PplP3DPipe(140.f, 146.f, P3D("WreckMast"), P3D("Crane"), P3D("Buoy"), P3D("Gantry_B")); PplP3DPipe(167.f, 172.f, P3D("Terraces"), P3D("Spire"), P3D("Gantry_B"), P3D("Buoy"));
PplP3DPipe(310.f, 314.f, P3D("Gantry_A"), P3D("Lift"), P3D("Terraces"), P3D("Crane")); PplP3DPipe(331.f, 335.f, P3D("Mast"), P3D("WreckMast"), P3D("Spire"), P3D("RopeBridge"));
PplP3DPipe(352.f, 356.f, P3D("Spire"), P3D("Buoy"), P3D("RopeBridge"), P3D("Mast")); PplP3DPipe(373.f, 377.f, P3D("Terraces"), P3D("Spire"), P3D("Gantry_B"), P3D("Buoy"));
PplP3DPipe(394.f, 398.f, P3D("WreckMast"), P3D("Crane"), P3D("Buoy"), P3D("Gantry_B")); PplP3DPipe(414.f, 419.f, P3D("Crane"), P3D("Gantry_B"), P3D("Lift"), P3D("Gantry_A"));
PplP3DPipe(435.f, 440.f, P3D("Gantry_A"), P3D("Lift"), P3D("Terraces"), P3D("Crane")); PplP3DPipe(456.f, 461.f, P3D("Mast"), P3D("WreckMast"), P3D("Spire"), P3D("RopeBridge"));
PplP3DPipe(477.f, 482.f, P3D("Spire"), P3D("Buoy"), P3D("RopeBridge"), P3D("Mast")); PplP3DPipe(498.f, 503.f, P3D("Terraces"), P3D("Spire"), P3D("Gantry_B"), P3D("Buoy"));
PplP3DPipe(519.f, 523.f, P3D("WreckMast"), P3D("Crane"), P3D("Buoy"), P3D("Gantry_B")); PplP3DPipe(540.f, 544.f, P3D("Crane"), P3D("Gantry_B"), P3D("Lift"), P3D("Gantry_A"));
#undef P3D
// ---- END p3d ----

}

{
// ---- BEGIN p3e (far and sky, ft_people_sea_p3e.py) ----
// Pass 3, builder e: life from 150 m out to the trench walls and in the open water high over them (this level's sky), on
// BOTH sides, the whole road. Eleven meshes, /Game/People/UnderTheSea/SM_P3E_*, every one TALL, saved with the always-moving
// master (far=True) and carrying a little steady light of its own colour against the haze (nothing blinks).
// No rule of this block places anything nearer than 160 m to the centre line; with what a piece reaches from its middle
// (a wheel of gliders 51 m, a travelling piece turned 6 degrees at most) nothing of it comes nearer than about 150 m.
// IN THE WATER (Far: z = 0 of the piece is the road's own level less the drop, wherever the land lies, so these also stand
//   beside the glass tunnels, the loops and the jumps; what they carry starts 15 m and more over that, clear of the trench
//   walls): balloons and balloon bunches wheeling, gliders wheeling, a flock of diving birds wheeling, the formation
//   swimmers' stars (turned 15 degrees at most: they face the road), the herders with their shoal and the water bus
//   (they travel 45 m either way along the road, turned 6 degrees at most: they never head for it).
// ON THE LAND (Scatter, 160 to 250 m, on the trench walls' lower slopes; not beside tunnels and set pieces): the dive
//   lines with their teams, the kite flyers' pylon.
// ON THE HORIZON (Far, 305 to 380 m, past the land's edge, its foot 15 m into the ground): the ridge and its climbers.
// The first far kit (SM_Far*, 150 to 280 m, placed at twice its size, 40 to 120 m up) and the giant jellyfish share
// these distances: this block's pieces are other things at other heights, with a chance of 1 and one side to a rule.
// Scale is part of the design (figures built 2.4 to 4.5 times life, placed at 1.4 to 1.8 times that).
// Counted slot by slot (45.243 m slots, 552 of them, every/phase against each zone; Scatter rules without the slots of
// tunnels and set pieces): 322 pieces (the reef 34, the kelp forest 29, the research station 30, the salvage field 28, the approach 24, Nereida 177); left 161, right 161; the most of one mesh 32
// (Balloons_A 30, Balloons_B 31, DiveLine_A 24, DiveLine_B 24, DivingBirds 32, FishHerd 31, Gliders 32, Kites 23, RidgeClimbers 32, StarRing 31, WaterBus 32).
#define P3E(n) TEXT("/Game/People/UnderTheSea/SM_P3E_" n)
Zone(TEXT("People p3e far and sky: the reef"), 0.f, 1.7f);   // dive teams on their lines, kites and balloons over the coral (slots 0 to 37: 34 pieces)
	Scatter(P3E("DiveLine_A"), 170.f, 250.f, 12, 0, 1.f, 1.4f, 0.1f, 1, 20.f);
	Far(P3E("Balloons_A"), 240.f, 400.f, 12, 7, 1.f, 1.8f, 0.12f, -1, -5.f, 180.f);
	Far(P3E("WaterBus"), 165.f, 225.f, 12, 2, 1.f, 1.6f, 0.08f, 1, 0.f, 6.f);
	Far(P3E("Gliders"), 205.f, 290.f, 12, 9, 1.f, 1.6f, 0.1f, -1, 0.f, 180.f);
	Scatter(P3E("Kites"), 160.f, 230.f, 12, 4, 1.f, 1.5f, 0.08f, 1, 10.f);
	Far(P3E("RidgeClimbers"), 305.f, 380.f, 12, 11, 1.f, 1.6f, 0.12f, -1, 4.f, 12.f);
	Far(P3E("StarRing"), 180.f, 260.f, 12, 6, 1.f, 1.6f, 0.1f, 1, 0.f, 15.f);
	Scatter(P3E("DiveLine_B"), 170.f, 250.f, 12, 1, 1.f, 1.4f, 0.1f, -1, 20.f);
	Far(P3E("FishHerd"), 175.f, 240.f, 12, 8, 1.f, 1.7f, 0.08f, 1, 0.f, 6.f);
	Far(P3E("Balloons_B"), 200.f, 330.f, 12, 3, 1.f, 1.6f, 0.12f, -1, 0.f, 180.f);
	Far(P3E("DivingBirds"), 200.f, 280.f, 12, 10, 1.f, 1.6f, 0.1f, 1, -5.f, 180.f);
Zone(TEXT("People p3e far and sky: the kelp forest"), 1.7f, 3.4f);   // gliders and birds over the weed, the water bus on its rounds (slots 38 to 75: 29 pieces)
	Scatter(P3E("DiveLine_A"), 170.f, 250.f, 12, 5, 1.f, 1.4f, 0.1f, -1, 20.f);
	Far(P3E("Balloons_A"), 240.f, 400.f, 12, 0, 1.f, 1.8f, 0.12f, 1, -5.f, 180.f);
	Far(P3E("WaterBus"), 165.f, 225.f, 12, 7, 1.f, 1.6f, 0.08f, -1, 0.f, 6.f);
	Far(P3E("Gliders"), 205.f, 290.f, 12, 2, 1.f, 1.6f, 0.1f, 1, 0.f, 180.f);
	Scatter(P3E("Kites"), 160.f, 230.f, 12, 9, 1.f, 1.5f, 0.08f, -1, 10.f);
	Far(P3E("RidgeClimbers"), 305.f, 380.f, 12, 4, 1.f, 1.6f, 0.12f, 1, 4.f, 12.f);
	Far(P3E("StarRing"), 180.f, 260.f, 12, 11, 1.f, 1.6f, 0.1f, -1, 0.f, 15.f);
	Scatter(P3E("DiveLine_B"), 170.f, 250.f, 12, 6, 1.f, 1.4f, 0.1f, 1, 20.f);
	Far(P3E("FishHerd"), 175.f, 240.f, 12, 1, 1.f, 1.7f, 0.08f, -1, 0.f, 6.f);
	Far(P3E("Balloons_B"), 200.f, 330.f, 12, 8, 1.f, 1.6f, 0.12f, 1, 0.f, 180.f);
	Far(P3E("DivingBirds"), 200.f, 280.f, 12, 3, 1.f, 1.6f, 0.1f, -1, -5.f, 180.f);
Zone(TEXT("People p3e far and sky: the research station"), 3.4f, 5.f);   // the station at play: stars, kites, the herders (slots 76 to 110: 30 pieces)
	Scatter(P3E("DiveLine_A"), 170.f, 250.f, 12, 10, 1.f, 1.4f, 0.1f, 1, 20.f);
	Far(P3E("Balloons_A"), 240.f, 400.f, 12, 5, 1.f, 1.8f, 0.12f, -1, -5.f, 180.f);
	Far(P3E("WaterBus"), 165.f, 225.f, 12, 0, 1.f, 1.6f, 0.08f, 1, 0.f, 6.f);
	Far(P3E("Gliders"), 205.f, 290.f, 12, 7, 1.f, 1.6f, 0.1f, -1, 0.f, 180.f);
	Scatter(P3E("Kites"), 160.f, 230.f, 12, 2, 1.f, 1.5f, 0.08f, 1, 10.f);
	Far(P3E("RidgeClimbers"), 305.f, 380.f, 12, 9, 1.f, 1.6f, 0.12f, -1, 4.f, 12.f);
	Far(P3E("StarRing"), 180.f, 260.f, 12, 4, 1.f, 1.6f, 0.1f, 1, 0.f, 15.f);
	Scatter(P3E("DiveLine_B"), 170.f, 250.f, 12, 11, 1.f, 1.4f, 0.1f, -1, 20.f);
	Far(P3E("FishHerd"), 175.f, 240.f, 12, 6, 1.f, 1.7f, 0.08f, 1, 0.f, 6.f);
	Far(P3E("Balloons_B"), 200.f, 330.f, 12, 1, 1.f, 1.6f, 0.12f, -1, 0.f, 180.f);
	Far(P3E("DivingBirds"), 200.f, 280.f, 12, 8, 1.f, 1.6f, 0.1f, 1, -5.f, 180.f);
Zone(TEXT("People p3e far and sky: the salvage field"), 5.f, 6.6f);   // over the dead ships: lines down to the wrecks, balloons, birds (slots 111 to 145: 28 pieces)
	Scatter(P3E("DiveLine_A"), 170.f, 250.f, 12, 3, 1.f, 1.4f, 0.1f, -1, 20.f);
	Far(P3E("Balloons_A"), 240.f, 400.f, 12, 10, 1.f, 1.8f, 0.12f, 1, -5.f, 180.f);
	Far(P3E("WaterBus"), 165.f, 225.f, 12, 5, 1.f, 1.6f, 0.08f, -1, 0.f, 6.f);
	Far(P3E("Gliders"), 205.f, 290.f, 12, 0, 1.f, 1.6f, 0.1f, 1, 0.f, 180.f);
	Scatter(P3E("Kites"), 160.f, 230.f, 12, 7, 1.f, 1.5f, 0.08f, -1, 10.f);
	Far(P3E("RidgeClimbers"), 305.f, 380.f, 12, 2, 1.f, 1.6f, 0.12f, 1, 4.f, 12.f);
	Far(P3E("StarRing"), 180.f, 260.f, 12, 9, 1.f, 1.6f, 0.1f, -1, 0.f, 15.f);
	Scatter(P3E("DiveLine_B"), 170.f, 250.f, 12, 4, 1.f, 1.4f, 0.1f, 1, 20.f);
	Far(P3E("FishHerd"), 175.f, 240.f, 12, 11, 1.f, 1.7f, 0.08f, -1, 0.f, 6.f);
	Far(P3E("Balloons_B"), 200.f, 330.f, 12, 6, 1.f, 1.6f, 0.12f, 1, 0.f, 180.f);
	Far(P3E("DivingBirds"), 200.f, 280.f, 12, 1, 1.f, 1.6f, 0.1f, -1, -5.f, 180.f);
Zone(TEXT("People p3e far and sky: the approach"), 6.6f, 7.94f);   // the city's traffic thickens: buses, herds, the formation teams (slots 146 to 175: 24 pieces)
	Scatter(P3E("DiveLine_A"), 170.f, 250.f, 12, 8, 1.f, 1.4f, 0.1f, 1, 20.f);
	Far(P3E("Balloons_A"), 240.f, 400.f, 12, 3, 1.f, 1.8f, 0.12f, -1, -5.f, 180.f);
	Far(P3E("WaterBus"), 165.f, 225.f, 12, 10, 1.f, 1.6f, 0.08f, 1, 0.f, 6.f);
	Far(P3E("Gliders"), 205.f, 290.f, 12, 5, 1.f, 1.6f, 0.1f, -1, 0.f, 180.f);
	Scatter(P3E("Kites"), 160.f, 230.f, 12, 0, 1.f, 1.5f, 0.08f, 1, 10.f);
	Far(P3E("RidgeClimbers"), 305.f, 380.f, 12, 7, 1.f, 1.6f, 0.12f, -1, 4.f, 12.f);
	Far(P3E("StarRing"), 180.f, 260.f, 12, 2, 1.f, 1.6f, 0.1f, 1, 0.f, 15.f);
	Scatter(P3E("DiveLine_B"), 170.f, 250.f, 12, 9, 1.f, 1.4f, 0.1f, -1, 20.f);
	Far(P3E("FishHerd"), 175.f, 240.f, 12, 4, 1.f, 1.7f, 0.08f, 1, 0.f, 6.f);
	Far(P3E("Balloons_B"), 200.f, 330.f, 12, 11, 1.f, 1.6f, 0.12f, -1, 0.f, 180.f);
	Far(P3E("DivingBirds"), 200.f, 280.f, 12, 6, 1.f, 1.6f, 0.1f, 1, -5.f, 180.f);
Zone(TEXT("People p3e far and sky: Nereida"), 7.94f, 99.f);   // the whole sky of the city, both sides, all the way through the fight (slots 176 to 551: 177 pieces)
	Scatter(P3E("DiveLine_A"), 170.f, 250.f, 44, 0, 1.f, 1.4f, 0.1f, -1, 20.f);
	Far(P3E("Balloons_A"), 240.f, 400.f, 44, 4, 1.f, 1.8f, 0.12f, -1, -5.f, 180.f);
	Far(P3E("WaterBus"), 165.f, 225.f, 44, 8, 1.f, 1.6f, 0.08f, -1, 0.f, 6.f);
	Far(P3E("Gliders"), 205.f, 290.f, 44, 12, 1.f, 1.6f, 0.1f, -1, 0.f, 180.f);
	Scatter(P3E("Kites"), 160.f, 230.f, 44, 16, 1.f, 1.5f, 0.08f, -1, 10.f);
	Far(P3E("RidgeClimbers"), 305.f, 380.f, 44, 20, 1.f, 1.6f, 0.12f, -1, 4.f, 12.f);
	Far(P3E("StarRing"), 180.f, 260.f, 44, 24, 1.f, 1.6f, 0.1f, -1, 0.f, 15.f);
	Scatter(P3E("DiveLine_B"), 170.f, 250.f, 44, 28, 1.f, 1.4f, 0.1f, -1, 20.f);
	Far(P3E("FishHerd"), 175.f, 240.f, 44, 32, 1.f, 1.7f, 0.08f, -1, 0.f, 6.f);
	Far(P3E("Balloons_B"), 200.f, 330.f, 44, 36, 1.f, 1.6f, 0.12f, -1, 0.f, 180.f);
	Far(P3E("DivingBirds"), 200.f, 280.f, 44, 40, 1.f, 1.6f, 0.1f, -1, -5.f, 180.f);
	Scatter(P3E("DiveLine_A"), 170.f, 250.f, 44, 24, 1.f, 1.4f, 0.1f, 1, 20.f);
	Far(P3E("Balloons_A"), 240.f, 400.f, 44, 28, 1.f, 1.8f, 0.12f, 1, -5.f, 180.f);
	Far(P3E("WaterBus"), 165.f, 225.f, 44, 32, 1.f, 1.6f, 0.08f, 1, 0.f, 6.f);
	Far(P3E("Gliders"), 205.f, 290.f, 44, 36, 1.f, 1.6f, 0.1f, 1, 0.f, 180.f);
	Scatter(P3E("Kites"), 160.f, 230.f, 44, 40, 1.f, 1.5f, 0.08f, 1, 10.f);
	Far(P3E("RidgeClimbers"), 305.f, 380.f, 44, 0, 1.f, 1.6f, 0.12f, 1, 4.f, 12.f);
	Far(P3E("StarRing"), 180.f, 260.f, 44, 4, 1.f, 1.6f, 0.1f, 1, 0.f, 15.f);
	Scatter(P3E("DiveLine_B"), 170.f, 250.f, 44, 8, 1.f, 1.4f, 0.1f, 1, 20.f);
	Far(P3E("FishHerd"), 175.f, 240.f, 44, 12, 1.f, 1.7f, 0.08f, 1, 0.f, 6.f);
	Far(P3E("Balloons_B"), 200.f, 330.f, 44, 16, 1.f, 1.6f, 0.12f, 1, 0.f, 180.f);
	Far(P3E("DivingBirds"), 200.f, 280.f, 44, 20, 1.f, 1.6f, 0.1f, 1, -5.f, 180.f);
#undef P3E
// ---- END p3e ----

}

{
// ---- BEGIN p3f (film shoot, ft_people_sea_p3f.py) ----
// FIX ROUND: Nereida's left, in front of the tube: the six units every 72 slots (was 36), s % 72 in {14, 20, 38, 44, 62, 68}; the sound rostrum of the kelp forest at 28 m.
// MORE OF THE SHOOT (pass 3, builder p3f; meshes /Game/People/UnderTheSea/SM_P3F_..., made by ft_people_sea_p3f.py).
// The main table has the main unit at the start, at the jumps and loops and along the fight. This block is the rest of a
// big shoot, spread over the whole road and both sides: second units, the camera-drone teams (under water a drone is a
// small yellow submersible: one hovers over its pad, one circles the mast behind the deck), the stunt rigging, the
// extras' holding area, the make-up tent, the press pens, a cable camera, a balloon light. Each unit comes with the
// rostrum it stands on (decks 2.4 to 2.6 m over the sand, figures a quarter over life size: the sand lies 0.9 m under
// the road behind a metre of barrier). It also stands the SHARED crew kit (/Game/People/Crew/SM_Crew<Piece>_Sea) up
// where it can be seen: a Set is a bare rostrum of this kit (SM_P3F_Deck_A / _B, 15 m by 5.4 m) with one kit scene on
// it, 2.5 m up, at 1.15 times its size.
// EVERY mesh of this block is TALL (the log: tops 6.3 to 12.4 m; a kit scene on its deck is over 2 m too), so every
// rule stands 27 m out or more, and with the piece's own depth and scale nothing of it is nearer than 24 m:
//   27.0-28 m, scale 1      in front of the travel tube (the station's right side, Nereida's left: the tube stands
//                           at 33 m) and in front of the weed in the kelp forest: bounds from 24.2 m
//   29.5-31 m, scale 1.1    everywhere else: bounds from 26.4 m (the kit's crane on its deck 31.5 m: its arm to 24.3 m)
//   37.5-45 m               behind the main unit at the start, the station's left side (on the slots between its pressure
//                           habitats, balconies and domes) and beside the two jumps
//   30.5-32 m, Far rules    by the glass tunnels, stood on the sand by the drop (0.9 m), as the main table does
//   44-46 m, Far rules      beside the loops and corkscrews
// Light: steady only (the balloon light, the make-up mirror's lamps, the kit's lighting stands), all 24 m out or more;
// nothing blinks. No yaw jitter and no place jitter: the fronts face the road.
// Slots: a rule's every and phase were chosen against the main table's own rules in the 24 to 37 m band (dive teams,
// patrols, the stands, the watch block's balconies, domes and airlocks) and the dressing's pressure habitats, district
// by district and side by side (C:/ftwork/jobs/L6p3f/gen_inl.py worked them out; the slots a rule takes are in its
// comment: s % every). Where a district has no free slot (the approach) a rule takes a slot of the mounted patrol and
// stands behind its line (30.5 m and out; the patrol rides at 25.5 to 27 m). Not checked against the other pass-3 builders' blocks, nor in the game.
#define P3F(n) TEXT("/Game/People/UnderTheSea/SM_P3F_" n)
#define P3FK(n) TEXT("/Game/People/Crew/SM_Crew" n "_Sea")

// ---- the start (slots 1 to 6; the main unit has 25.5 to 30 m on most of them): the press and the make-up tent behind the
// applauding crew and the camera on the right, the extras and a second unit on the left, a balloon light behind the crane.
Zone(TEXT("People p3f: the start"), 0.0f, 0.28f);
	Scatter(P3F("PressPen_A"), 29.5f, 30.5f, 6, 5, 1.f, 1.1f, 0.f, 1, 0.f);   // slot 1, right
	Scatter(P3F("MakeUp"), 29.5f, 30.5f, 6, 4, 1.f, 1.1f, 0.f, 1, 0.f);   // slot 2, right
	Scatter(P3F("LightBalloon"), 37.5f, 38.5f, 6, 3, 1.f, 1.2f, 0.f, -1, 0.f);   // slot 3, left, behind the crane
	Scatter(P3F("Extras_A"), 29.5f, 30.5f, 6, 2, 1.f, 1.1f, 0.f, -1, 0.f);   // slot 4, left
	Scatter(P3F("Extras_B"), 37.5f, 38.5f, 6, 1, 1.f, 1.2f, 0.f, 1, 0.f);   // slot 5, right, behind the catering
	Scatter(P3F("SecondUnit_A"), 29.5f, 30.5f, 7, 1, 1.f, 1.1f, 0.f, -1, 0.f);   // slot 6, left

// ---- the shared kit, each scene on a rostrum of its own (deck top 2.5 m; the kit scene 1.15 times its size)
Set(TEXT("PplP3FCam"), { { P3F("Deck_A"), 0.f, 0.f, 0.f, 0.f, 1.f }, { P3FK("Camera"), 0.f, 0.f, 2.5f, 0.f, 1.15f } });
Set(TEXT("PplP3FClapA"), { { P3F("Deck_A"), 0.f, 0.f, 0.f, 0.f, 1.f }, { P3FK("ApplauseA"), 0.f, 0.f, 2.5f, 0.f, 1.15f } });
Set(TEXT("PplP3FStunt"), { { P3F("Deck_A"), 0.f, 0.f, 0.f, 0.f, 1.f }, { P3FK("Stunt"), 0.f, 0.f, 2.5f, 0.f, 1.15f } });
Set(TEXT("PplP3FCrane"), { { P3F("Deck_A"), 0.f, 0.f, 0.f, 0.f, 1.f }, { P3FK("Crane"), 0.f, 0.f, 2.5f, 0.f, 1.15f } });
Set(TEXT("PplP3FSound"), { { P3F("Deck_A"), 0.f, 0.f, 0.f, 0.f, 1.f }, { P3FK("Sound"), 0.f, 0.f, 2.5f, 0.f, 1.15f } });
Set(TEXT("PplP3FVillage"), { { P3F("Deck_B"), 0.f, 0.f, 0.f, 0.f, 1.f }, { P3FK("Village"), 0.f, 0.f, 2.5f, 0.f, 1.15f } });
Set(TEXT("PplP3FClapB"), { { P3F("Deck_B"), 0.f, 0.f, 0.f, 0.f, 1.f }, { P3FK("ApplauseB"), 0.f, 0.f, 2.5f, 0.f, 1.15f } });
Set(TEXT("PplP3FDolly"), { { P3F("Deck_B"), 0.f, 0.f, 0.f, 0.f, 1.f }, { P3FK("Dolly"), 0.f, 0.f, 2.5f, 0.f, 1.15f } });
Set(TEXT("PplP3FLights"), { { P3F("Deck_B"), 0.f, 0.f, 0.f, 0.f, 1.f }, { P3FK("Lights"), 0.f, 0.f, 2.5f, 0.f, 1.15f } });
Set(TEXT("PplP3FCatering"), { { P3F("Deck_B"), 0.f, 0.f, 0.f, 0.f, 1.f }, { P3FK("Catering"), 0.f, 0.f, 2.5f, 0.f, 1.15f } });

// ---- the reef: the extras come in, make-up, the press, the first units
Zone(TEXT("People p3f: the reef"), 0.294f, 1.7f);
	Scatter(P3F("Extras_A"), 29.5f, 31.f, 12, 0, 1.f, 1.1f, 0.f, -1, 0.f);   // left: s % 12 == 0
	Scatter(P3F("PressPen_B"), 29.5f, 31.f, 12, 9, 1.f, 1.1f, 0.f, -1, 0.f);   // left: s % 12 == 3
	Scatter(P3F("SecondUnit_A"), 29.5f, 31.f, 12, 6, 1.f, 1.1f, 0.f, -1, 0.f);   // left: s % 12 == 6
	Scatter(P3F("MakeUp"), 29.5f, 31.f, 12, 3, 1.f, 1.1f, 0.f, -1, 0.f);   // left: s % 12 == 9
	Site(TEXT("PplP3FCam"), -1, 30.f, 12, 10, 1.f);   // left: s % 12 == 2
	Scatter(P3F("MakeUp"), 29.5f, 31.f, 12, 0, 1.f, 1.1f, 0.f, 1, 0.f);   // right: s % 12 == 0
	Scatter(P3F("DroneTeam_A"), 29.5f, 31.f, 12, 9, 1.f, 1.1f, 0.f, 1, 0.f);   // right: s % 12 == 3
	Scatter(P3F("Extras_B"), 29.5f, 31.f, 12, 6, 1.f, 1.1f, 0.f, 1, 0.f);   // right: s % 12 == 6
	Scatter(P3F("PressPen_A"), 29.5f, 31.f, 12, 3, 1.f, 1.1f, 0.f, 1, 0.f);   // right: s % 12 == 9
	Site(TEXT("PplP3FDolly"), 1, 30.f, 12, 2, 1.f);   // right: s % 12 == 10

// ---- the kelp forest: light for the dark of the weed, drones over it, the cable camera (all in front of the weed)
Zone(TEXT("People p3f: the kelp forest"), 1.7f, 3.4f);
	Scatter(P3F("LightBalloon"), 27.f, 28.f, 12, 10, 1.f, 1.f, 0.f, -1, 0.f);   // left: s % 12 == 2
	Scatter(P3F("DroneTeam_B"), 27.f, 28.f, 12, 7, 1.f, 1.f, 0.f, -1, 0.f);   // left: s % 12 == 5
	Scatter(P3F("CableCam"), 27.f, 28.f, 12, 4, 1.f, 1.f, 0.f, -1, 0.f);   // left: s % 12 == 8
	Scatter(P3F("SecondUnit_B"), 27.f, 28.f, 12, 1, 1.f, 1.f, 0.f, -1, 0.f);   // left: s % 12 == 11
	Site(TEXT("PplP3FLights"), -1, 27.5f, 12, 9, 1.f);   // left: s % 12 == 3
	Scatter(P3F("CableCam"), 27.f, 28.f, 12, 11, 1.f, 1.f, 0.f, 1, 0.f);   // right: s % 12 == 1
	Scatter(P3F("LightBalloon"), 27.f, 28.f, 12, 8, 1.f, 1.f, 0.f, 1, 0.f);   // right: s % 12 == 4
	Scatter(P3F("SecondUnit_A"), 27.f, 28.f, 12, 5, 1.f, 1.f, 0.f, 1, 0.f);   // right: s % 12 == 7
	Scatter(P3F("DroneTeam_A"), 27.f, 28.f, 12, 2, 1.f, 1.f, 0.f, 1, 0.f);   // right: s % 12 == 10
	Site(TEXT("PplP3FSound"), 1, 28.f, 12, 6, 1.f);   // right: s % 12 == 6 (fix round: 0.5 m further out)

// ---- the research station: right, in front of the travel tube; left, behind the pipeline, between the pressure habitats
Zone(TEXT("People p3f: the research station"), 3.4f, 5.f);
	Scatter(P3F("StuntRig"), 38.5f, 39.5f, 12, 0, 1.f, 1.15f, 0.f, -1, 0.f);   // left: s % 12 == 0
	Scatter(P3F("PressPen_A"), 38.5f, 39.5f, 12, 8, 1.f, 1.15f, 0.f, -1, 0.f);   // left: s % 12 == 4
	Scatter(P3F("Extras_A"), 38.5f, 39.5f, 12, 4, 1.f, 1.15f, 0.f, -1, 0.f);   // left: s % 12 == 8
	Scatter(P3F("SecondUnit_B"), 27.f, 27.5f, 12, 0, 1.f, 1.f, 0.f, 1, 0.f);   // right: s % 12 == 0
	Scatter(P3F("DroneTeam_B"), 27.f, 27.5f, 12, 9, 1.f, 1.f, 0.f, 1, 0.f);   // right: s % 12 == 3
	Scatter(P3F("PressPen_B"), 27.f, 27.5f, 12, 6, 1.f, 1.f, 0.f, 1, 0.f);   // right: s % 12 == 6
	Scatter(P3F("CableCam"), 27.f, 27.5f, 12, 3, 1.f, 1.f, 0.f, 1, 0.f);   // right: s % 12 == 9
	Site(TEXT("PplP3FVillage"), 1, 27.5f, 12, 11, 1.f);   // right: s % 12 == 1

// ---- the salvage field: the stunt unit works the wrecks
Zone(TEXT("People p3f: the salvage field"), 5.f, 6.6f);
	Scatter(P3F("StuntRig"), 29.5f, 31.f, 12, 10, 1.f, 1.1f, 0.f, -1, 0.f);   // left: s % 12 == 2
	Scatter(P3F("SecondUnit_A"), 29.5f, 31.f, 12, 7, 1.f, 1.1f, 0.f, -1, 0.f);   // left: s % 12 == 5
	Scatter(P3F("Extras_B"), 29.5f, 31.f, 12, 4, 1.f, 1.1f, 0.f, -1, 0.f);   // left: s % 12 == 8
	Scatter(P3F("LightBalloon"), 29.5f, 31.f, 12, 1, 1.f, 1.1f, 0.f, -1, 0.f);   // left: s % 12 == 11
	Site(TEXT("PplP3FStunt"), -1, 30.f, 12, 5, 1.f);   // left: s % 12 == 7
	Scatter(P3F("DroneTeam_A"), 29.5f, 31.f, 12, 10, 1.f, 1.1f, 0.f, 1, 0.f);   // right: s % 12 == 2
	Scatter(P3F("StuntRig"), 29.5f, 31.f, 12, 7, 1.f, 1.1f, 0.f, 1, 0.f);   // right: s % 12 == 5
	Scatter(P3F("MakeUp"), 29.5f, 31.f, 12, 4, 1.f, 1.1f, 0.f, 1, 0.f);   // right: s % 12 == 8
	Scatter(P3F("SecondUnit_B"), 29.5f, 31.f, 12, 1, 1.f, 1.1f, 0.f, 1, 0.f);   // right: s % 12 == 11
	Site(TEXT("PplP3FCrane"), 1, 31.5f, 12, 5, 1.f);   // right: s % 12 == 7

// ---- the approach: the press waits for the city
Zone(TEXT("People p3f: the approach"), 6.6f, 7.94f);
	Scatter(P3F("PressPen_A"), 30.5f, 32.f, 6, 0, 1.f, 1.1f, 0.f, -1, 0.f);   // left: s % 6 == 0
	Scatter(P3F("Extras_A"), 30.5f, 32.f, 6, 2, 1.f, 1.1f, 0.f, -1, 0.f);   // left: s % 6 == 4
	Scatter(P3F("PressPen_B"), 30.5f, 32.f, 6, 3, 1.f, 1.1f, 0.f, 1, 0.f);   // right: s % 6 == 3

// ---- Nereida and the fight: the whole shoot, in turn, on both sides (left: in front of the travel tube)
Zone(TEXT("People p3f: Nereida and the fight"), 7.94f, 99.f);
	Scatter(P3F("SecondUnit_A"), 27.f, 27.5f, 72, 34, 1.f, 1.f, 0.f, -1, 0.f);   // left: s % 72 == 38 (fix round: every 72, was 36)
	Scatter(P3F("Extras_B"), 27.f, 27.5f, 72, 28, 1.f, 1.f, 0.f, -1, 0.f);   // left: s % 72 == 44 (fix round: every 72, was 36)
	Scatter(P3F("DroneTeam_A"), 27.f, 27.5f, 72, 58, 1.f, 1.f, 0.f, -1, 0.f);   // left: s % 72 == 14 (fix round: every 72, was 36)
	Scatter(P3F("PressPen_A"), 27.f, 27.5f, 72, 52, 1.f, 1.f, 0.f, -1, 0.f);   // left: s % 72 == 20 (fix round: every 72, was 36)
	Scatter(P3F("LightBalloon"), 27.f, 27.5f, 72, 10, 1.f, 1.f, 0.f, -1, 0.f);   // left: s % 72 == 62 (fix round: every 72, was 36)
	Scatter(P3F("MakeUp"), 27.f, 27.5f, 72, 4, 1.f, 1.f, 0.f, -1, 0.f);   // left: s % 72 == 68 (fix round: every 72, was 36)
	Site(TEXT("PplP3FClapA"), -1, 27.5f, 72, 63, 1.f);   // left: s % 72 == 9
	Site(TEXT("PplP3FStunt"), -1, 27.5f, 72, 51, 1.f);   // left: s % 72 == 21
	Site(TEXT("PplP3FVillage"), -1, 27.5f, 72, 39, 1.f);   // left: s % 72 == 33
	Site(TEXT("PplP3FCam"), -1, 27.5f, 72, 27, 1.f);   // left: s % 72 == 45
	Site(TEXT("PplP3FLights"), -1, 27.5f, 72, 15, 1.f);   // left: s % 72 == 57
	Site(TEXT("PplP3FClapB"), -1, 27.5f, 72, 3, 1.f);   // left: s % 72 == 69
	Scatter(P3F("StuntRig"), 29.5f, 31.f, 36, 35, 1.f, 1.1f, 0.f, 1, 0.f);   // right: s % 36 == 1
	Scatter(P3F("PressPen_B"), 29.5f, 31.f, 36, 29, 1.f, 1.1f, 0.f, 1, 0.f);   // right: s % 36 == 7
	Scatter(P3F("CableCam"), 29.5f, 31.f, 36, 23, 1.f, 1.1f, 0.f, 1, 0.f);   // right: s % 36 == 13
	Scatter(P3F("Extras_A"), 29.5f, 31.f, 36, 17, 1.f, 1.1f, 0.f, 1, 0.f);   // right: s % 36 == 19
	Scatter(P3F("SecondUnit_B"), 29.5f, 31.f, 36, 11, 1.f, 1.1f, 0.f, 1, 0.f);   // right: s % 36 == 25
	Scatter(P3F("DroneTeam_B"), 29.5f, 31.f, 36, 5, 1.f, 1.1f, 0.f, 1, 0.f);   // right: s % 36 == 31
	Site(TEXT("PplP3FCrane"), 1, 31.5f, 72, 67, 1.f);   // right: s % 72 == 5
	Site(TEXT("PplP3FCatering"), 1, 30.f, 72, 55, 1.f);   // right: s % 72 == 17
	Site(TEXT("PplP3FDolly"), 1, 30.f, 72, 43, 1.f);   // right: s % 72 == 29
	Site(TEXT("PplP3FSound"), 1, 30.f, 72, 31, 1.f);   // right: s % 72 == 41
	Site(TEXT("PplP3FCam"), 1, 30.f, 72, 19, 1.f);   // right: s % 72 == 53
	Site(TEXT("PplP3FLights"), 1, 30.f, 72, 7, 1.f);   // right: s % 72 == 65

// ---- beside the two jumps (the gaps are slots 57 and 122-123): the stunt unit and the cameras 44 m out, two and three slots
// before the gap and two after it (the track keeps the run-up and the landing clear within 40 m by itself; the sand dips
// under the gap, so nothing of this block stands in the slots next to it)
const auto PplP3FAt = [&](float S, const TCHAR* Mesh, int32 Side)
{
	Zone(TEXT("People p3f: by a jump"), (S - 0.5f) * 0.045243f, (S + 0.4f) * 0.045243f);
	Scatter(Mesh, 44.f, 45.f, 1, 0, 1.f, 1.1f, 0.f, Side, 0.f);
};
PplP3FAt(54.f, P3F("CableCam"), 1); PplP3FAt(54.f, P3F("PressPen_B"), -1); PplP3FAt(55.f, P3F("StuntRig"), -1);
PplP3FAt(55.f, P3F("SecondUnit_B"), 1); PplP3FAt(59.f, P3F("DroneTeam_A"), 1); PplP3FAt(59.f, P3F("Extras_A"), -1);
PplP3FAt(119.f, P3F("SecondUnit_A"), -1); PplP3FAt(119.f, P3F("PressPen_A"), 1); PplP3FAt(120.f, P3F("StuntRig"), 1);
PplP3FAt(120.f, P3F("DroneTeam_B"), -1); PplP3FAt(125.f, P3F("CableCam"), -1); PplP3FAt(125.f, P3F("LightBalloon"), 1);

// ---- by the glass tunnels (Scatter rules take no slot of a tunnel: Far rules do, the piece stood on the sand by its
// drop). Two units a side, every third slot, seen through the glass; the phases are each tunnel's own, off the main
// table's swimmers and stands there.
const auto PplP3FPipe = [&](float A, float B, const TCHAR* L0, const TCHAR* L1, int32 PhL, const TCHAR* R0, const TCHAR* R1, int32 PhR)
{
	Zone(TEXT("People p3f: by a glass tunnel"), (A - 0.5f) * 0.045243f, (B + 0.5f) * 0.045243f);
	Far(L0, 30.5f, 32.f, 6, PhL, 1.f, 1.1f, 0.f, -1, 0.9f, 0.f);
	Far(L1, 30.5f, 32.f, 6, PhL + 3, 1.f, 1.1f, 0.f, -1, 0.9f, 0.f);
	Far(R0, 30.5f, 32.f, 6, PhR, 1.f, 1.1f, 0.f, 1, 0.9f, 0.f);
	Far(R1, 30.5f, 32.f, 6, PhR + 3, 1.f, 1.1f, 0.f, 1, 0.9f, 0.f);
};
PplP3FPipe(15.f, 20.f, P3F("SecondUnit_B"), P3F("Extras_B"), 2, P3F("LightBalloon"), P3F("StuntRig"), 0);
PplP3FPipe(37.f, 43.f, P3F("Extras_A"), P3F("CableCam"), 2, P3F("DroneTeam_B"), P3F("PressPen_A"), 2);
PplP3FPipe(79.f, 83.f, P3F("PressPen_B"), P3F("DroneTeam_A"), 0, P3F("MakeUp"), P3F("SecondUnit_A"), 0);
PplP3FPipe(101.f, 108.f, P3F("SecondUnit_B"), P3F("Extras_B"), 2, P3F("LightBalloon"), P3F("StuntRig"), 0);
PplP3FPipe(140.f, 146.f, P3F("Extras_A"), P3F("CableCam"), 0, P3F("DroneTeam_B"), P3F("PressPen_A"), 2);
PplP3FPipe(167.f, 172.f, P3F("PressPen_B"), P3F("DroneTeam_A"), 0, P3F("MakeUp"), P3F("SecondUnit_A"), 2);
PplP3FPipe(310.f, 314.f, P3F("SecondUnit_B"), P3F("Extras_B"), 0, P3F("LightBalloon"), P3F("StuntRig"), 2);
PplP3FPipe(331.f, 335.f, P3F("Extras_A"), P3F("CableCam"), 1, P3F("DroneTeam_B"), P3F("PressPen_A"), 1);
PplP3FPipe(352.f, 356.f, P3F("PressPen_B"), P3F("DroneTeam_A"), 1, P3F("MakeUp"), P3F("SecondUnit_A"), 1);
PplP3FPipe(373.f, 377.f, P3F("SecondUnit_B"), P3F("Extras_B"), 0, P3F("LightBalloon"), P3F("StuntRig"), 1);
PplP3FPipe(394.f, 398.f, P3F("Extras_A"), P3F("CableCam"), 0, P3F("DroneTeam_B"), P3F("PressPen_A"), 1);
PplP3FPipe(414.f, 419.f, P3F("PressPen_B"), P3F("DroneTeam_A"), 2, P3F("MakeUp"), P3F("SecondUnit_A"), 0);
PplP3FPipe(435.f, 440.f, P3F("SecondUnit_B"), P3F("Extras_B"), 2, P3F("LightBalloon"), P3F("StuntRig"), 0);
PplP3FPipe(456.f, 461.f, P3F("Extras_A"), P3F("CableCam"), 0, P3F("DroneTeam_B"), P3F("PressPen_A"), 2);
PplP3FPipe(477.f, 482.f, P3F("PressPen_B"), P3F("DroneTeam_A"), 0, P3F("MakeUp"), P3F("SecondUnit_A"), 2);
PplP3FPipe(498.f, 503.f, P3F("SecondUnit_B"), P3F("Extras_B"), 1, P3F("LightBalloon"), P3F("StuntRig"), 0);
PplP3FPipe(519.f, 523.f, P3F("Extras_A"), P3F("CableCam"), 1, P3F("DroneTeam_B"), P3F("PressPen_A"), 1);
PplP3FPipe(540.f, 544.f, P3F("PressPen_B"), P3F("DroneTeam_A"), 0, P3F("MakeUp"), P3F("SecondUnit_A"), 2);

// ---- beside the loops and corkscrews (62-65, 71-76, 128-131, 148-154): the cameras that shoot them, 44 to 46 m out
const auto PplP3FLoop = [&](float A, float B, const TCHAR* L, const TCHAR* R)
{
	Zone(TEXT("People p3f: beside a set piece"), (A - 0.5f) * 0.045243f, (B + 0.5f) * 0.045243f);
	Far(L, 44.f, 46.f, 3, 0, 1.f, 1.1f, 0.f, -1, 0.95f, 0.f);
	Far(R, 44.f, 46.f, 3, 1, 1.f, 1.1f, 0.f, 1, 0.95f, 0.f);
};
PplP3FLoop(62.f, 65.f, P3F("CableCam"), P3F("SecondUnit_B")); PplP3FLoop(71.f, 76.f, P3F("DroneTeam_B"), P3F("StuntRig"));
PplP3FLoop(128.f, 131.f, P3F("SecondUnit_A"), P3F("CableCam")); PplP3FLoop(148.f, 154.f, P3F("StuntRig"), P3F("DroneTeam_A"));
#undef P3F
#undef P3FK
// ---- END p3f ----

}
