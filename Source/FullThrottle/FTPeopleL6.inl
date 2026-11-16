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
