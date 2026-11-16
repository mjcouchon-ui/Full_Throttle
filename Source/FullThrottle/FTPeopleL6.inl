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

// ==== STAGE 1 (the world): eleven builders' blocks, each in its own { }, pasted at the end so no earlier rule re-rolls ====

{
// ---- BEGIN s1d1 (district 1 "The reef", ft_world_sea_s1d1.py) ----
// A VILLAGE GROWN OUT OF CORAL, 0 to 1.7 km. 18 meshes, all /Game/World/UnderTheSea/SM_S1D1_*: 7 buildings (UrchinHut,
// LimpetStore, ConchHouse, ScallopMarket, BarnacleTower, ClamBandShell, BrainBath), 4 low props (CowrieStools,
// BasketStack, PearlFence, ConchPlanters), 6 tall props (CoralLamp, ShellSign, ShellChimes, SpongeRack, ShellWell,
// WashLine), 1 hero (GreatConch, 45 m). Only Sites (every rule certain: chance 1), only even slots (every 4, 12, 16, 20
// or 44; phase % 4 == 0 or 2), only inside 0 to 1.7 km. Nothing blinks; steady light only on pieces 25.9 m out and
// more (lamp globes magenta and lavender; windows cream, 48 m out and more). No words: picture signs.
// Estimate (slots = 22.1 x km; x 0.75 for what the track leaves out; x parts in the set):
//   village, right   8 Sites, every 16, 0 to 1.7 km (37.57 slots):    8 x 37.57/16 x 0.75 x 3      = 42.3
//   village, left    7 Sites, every 20:                               7 x 37.57/20 x 0.75 x 3      = 29.6
//   the Great Conch  2 Sites, every 44 (one slot each):               2 x 37.57/44 x 0.75          =  1.3
//   the verge        4 Sites, every 4, 0.30 to 1.7 km (30.94 slots):  4 x 30.94/4 x 0.75 x 3       = 69.6
//   the lane         LaneA 2 Sites, every 4, 0.48 to 1.7 km (26.96):  2 x 26.96/4 x 0.75 x 3       = 30.3
//                    LaneB 1 Site every 4 and 2 every 12:             (26.96/4 + 2 x 26.96/12) x 0.75 x 4 = 33.7
//   sum about 207 placed. Counted slot by slot (even slots 0 to 36 without the glass tunnel's 16, 18, 20; if the track
//   leaves nothing else out): 224. Most of one mesh: PearlFence and ConchPlanters 24 each (estimate 23.2), UrchinHut 21,
//   CoralLamp 19, LimpetStore 16; every other building 7 to 12; the hero twice.
// Bands and slots used, and what stands there already (read from FTDressL6.inl, FTPeopleL6.inl and the sizes in the
// make logs; NOT seen in the game):
//   20.3 m   THE VERGE, LOW (tops 0.9 to 1.5 m, each piece 1 m deep or less, three a slot at fixed places along): behind
//            the crab rows (17.6 to 19.2 m, 2.2 m deep, right s % 5 == 0, left s % 5 == 4: a row can touch a piece at
//            slots 10 and 30 right, 14, 24 and 34 left; not avoided) and before the verge gangs, eel gardens and
//            gardeners (21.6 to 23.6 m, 2 m deep). From slot 8 on: slots 1 to 6 are the film crew's base camp.
//   25.9 m   THE LANE, TALL (3.06 to 5.03 m, the smallest x min of a TALL piece of this block; bounds 1.1 m deep each way at
//            the most, movement included: front 24.8 m). Behind the verge gangs (backs to 24.6 m), before the scooter
//            files, decks and wagons (28.8 m on; fronts from 27.3 m; the turtle wagon's front can be 26.7 m at slot 28
//            right: not avoided). From slot 12 on (the crew's crane and sled stand at 25.5 to 27 m in slot 10). Right:
//            never s % 12 == 8 (SM_Watchers_A, 26 to 29 m): the right lane takes s % 4 == 2 and s % 12 == 0 and 4.
//   52 m     THE VILLAGE, LEFT: three buildings a slot (anchor 52 m, pieces 51.5 to 54.5 m, fronts from 46 m). Even slots:
//            SM_P3D_* stands at 48 to 54 m in the odd ones. Never s % 10 == 2 (SM_WatchWall, 42 to 48 m): seven rules of
//            every 20. Behind the scenes of p3b (38 to 41 m, backs to 44.9 m); the reef garden's corals (54 to 59 m in
//            the even slots) grow among the huts.
//   65 m     THE VILLAGE, RIGHT, 1.2 times the size (anchor 65 m, pieces 64.4 to 68 m): behind SM_P3D_* (50 to 56 m in
//            every even slot, 11 to 19 m tall; its back can reach 61.8 m, a bath house's pool 58 m: not avoided), before
//            nothing: the right reef gardens (72 m) stand in the odd slots.
//   135 / 128 m  THE GREAT CONCH, right: slot 12 (0.543 km) and slot 34 (1.538 km). Not slot 32 (a reef arch, s % 3 == 2,
//            and the shark ring, s % 9 == 5, at 90 to 190 m). Reef shelves (110 to 230 m, every slot) lie about its foot.
// Fish rings, jellyfish, turtles, dive teams and commuters swim through all of this, 4 to 15 m up: not avoided.
// The glass tunnel (slots 15 to 20) takes no Site: the village has a gap there. No Far rule is used.
#define S1D1(n) TEXT("/Game/World/UnderTheSea/SM_S1D1_" n)
// the village: three buildings a slot { mesh, along m, out m, up m, yaw deg, scale }
Set(TEXT("WldS1D1YardA"), { { S1D1("UrchinHut"), -15.5f, 0.0f, 0.f, 14.0f, 1.f }, { S1D1("ScallopMarket"), 0.0f, 0.5f, 0.f, 0.0f, 1.f }, { S1D1("LimpetStore"), 15.0f, -0.5f, 0.f, -12.0f, 1.f } });
Set(TEXT("WldS1D1YardB"), { { S1D1("ConchHouse"), -14.0f, 0.5f, 0.f, 8.0f, 1.f }, { S1D1("UrchinHut"), 1.0f, 2.5f, 0.f, -20.0f, 1.f }, { S1D1("BarnacleTower"), 14.5f, 1.0f, 0.f, 25.0f, 1.f } });
Set(TEXT("WldS1D1YardC"), { { S1D1("LimpetStore"), -15.0f, 1.0f, 0.f, 10.0f, 1.f }, { S1D1("BrainBath"), 0.0f, 2.5f, 0.f, 0.0f, 1.f }, { S1D1("UrchinHut"), 14.0f, 0.5f, 0.f, -16.0f, 1.f } });
Set(TEXT("WldS1D1YardD"), { { S1D1("BarnacleTower"), -14.0f, 1.5f, 0.f, -30.0f, 1.f }, { S1D1("ClamBandShell"), 1.0f, 1.0f, 0.f, 0.0f, 1.f }, { S1D1("LimpetStore"), 15.5f, 2.0f, 0.f, -8.0f, 1.f } });
Set(TEXT("WldS1D1YardE"), { { S1D1("UrchinHut"), -15.0f, 1.5f, 0.f, 22.0f, 1.f }, { S1D1("ConchHouse"), -0.5f, 0.5f, 0.f, -10.0f, 1.f }, { S1D1("BrainBath"), 13.5f, 2.5f, 0.f, 4.0f, 1.f } });
Set(TEXT("WldS1D1YardF"), { { S1D1("ScallopMarket"), -11.5f, 0.5f, 0.f, 5.0f, 1.f }, { S1D1("UrchinHut"), 2.5f, 2.5f, 0.f, -24.0f, 1.f }, { S1D1("BarnacleTower"), 14.5f, 0.5f, 0.f, 50.0f, 1.f } });
Set(TEXT("WldS1D1YardG"), { { S1D1("ClamBandShell"), -12.0f, 1.0f, 0.f, 4.0f, 1.f }, { S1D1("LimpetStore"), 1.0f, 2.0f, 0.f, 14.0f, 1.f }, { S1D1("ConchHouse"), 13.0f, 0.5f, 0.f, -6.0f, 1.f } });
// the lane, TALL (anchor 25.9 m)
Set(TEXT("WldS1D1LaneA"), { { S1D1("CoralLamp"), -15.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D1("ShellSign"), 0.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D1("ShellChimes"), 15.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D1LaneB"), { { S1D1("SpongeRack"), -15.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D1("ShellWell"), -5.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D1("WashLine"), 6.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D1("CoralLamp"), 17.0f, 0.0f, 0.f, 0.0f, 1.f } });
// the verge, LOW (anchor 20.3 m)
Set(TEXT("WldS1D1VergeA"), { { S1D1("PearlFence"), -13.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D1("CowrieStools"), -3.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D1("ConchPlanters"), 7.0f, 0.0f, 0.f, 0.0f, 1.f } });
Set(TEXT("WldS1D1VergeB"), { { S1D1("ConchPlanters"), -12.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D1("BasketStack"), -3.0f, 0.0f, 0.f, 0.0f, 1.f }, { S1D1("PearlFence"), 8.0f, 0.0f, 0.f, 0.0f, 1.f } });
// the hero
Set(TEXT("WldS1D1Conch"), { { S1D1("GreatConch"), 0.0f, 0.0f, 0.f, 0.0f, 1.f } });
Zone(TEXT("World s1d1, The reef: the village"), 0.0f, 1.7f);
	Site(TEXT("WldS1D1YardA"), 1, 65.0f, 16, 0, 1.f, 1.2f); // right, s % 16 == 0: slots 0, 32
	Site(TEXT("WldS1D1YardB"), 1, 65.0f, 16, 8, 1.f, 1.2f); // right, s % 16 == 8: slots 8, 24
	Site(TEXT("WldS1D1YardC"), 1, 65.0f, 16, 14, 1.f, 1.2f); // right, s % 16 == 2: slots 2, 34
	Site(TEXT("WldS1D1YardD"), 1, 65.0f, 16, 6, 1.f, 1.2f); // right, s % 16 == 10: slots 10, 26
	Site(TEXT("WldS1D1YardE"), 1, 65.0f, 16, 12, 1.f, 1.2f); // right, s % 16 == 4: slots 4, 36
	Site(TEXT("WldS1D1YardF"), 1, 65.0f, 16, 4, 1.f, 1.2f); // right, s % 16 == 12: slots 12, 28
	Site(TEXT("WldS1D1YardG"), 1, 65.0f, 16, 10, 1.f, 1.2f); // right, s % 16 == 6: slots 6, 22
	Site(TEXT("WldS1D1YardA"), 1, 65.0f, 16, 2, 1.f, 1.2f); // right, s % 16 == 14: slots 14, 30
	Site(TEXT("WldS1D1YardG"), -1, 52.0f, 20, 0, 1.f);       // left, s % 20 == 0: slots 0
	Site(TEXT("WldS1D1YardC"), -1, 52.0f, 20, 16, 1.f);       // left, s % 20 == 4: slots 4, 24
	Site(TEXT("WldS1D1YardD"), -1, 52.0f, 20, 12, 1.f);       // left, s % 20 == 8: slots 8, 28
	Site(TEXT("WldS1D1YardA"), -1, 52.0f, 20, 4, 1.f);       // left, s % 20 == 16: slots 36
	Site(TEXT("WldS1D1YardE"), -1, 52.0f, 20, 14, 1.f);       // left, s % 20 == 6: slots 6, 26
	Site(TEXT("WldS1D1YardF"), -1, 52.0f, 20, 10, 1.f);       // left, s % 20 == 10: slots 10, 30
	Site(TEXT("WldS1D1YardB"), -1, 52.0f, 20, 6, 1.f);       // left, s % 20 == 14: slots 14, 34
	Site(TEXT("WldS1D1Conch"), 1, 135.0f, 44, 32, 1.f);       // right, s % 44 == 12: slots 12
	Site(TEXT("WldS1D1Conch"), 1, 128.0f, 44, 10, 1.f);       // right, s % 44 == 34: slots 34
Zone(TEXT("World s1d1, The reef: the verge"), 0.3f, 0.97f);   // fix round: to slot 21 only: slots 22-29 are the banked S-bend (nothing 17 to 40 m there); slots 30-36 stand in the s1fix block at the end of this file
	Site(TEXT("WldS1D1VergeA"), -1, 20.3f, 4, 0, 1.f);      // left, s % 4 == 0: slots 8, 12, 24, 28, 32, 36
	Site(TEXT("WldS1D1VergeB"), 1, 20.3f, 4, 0, 1.f);      // right, s % 4 == 0: slots 8, 12, 24, 28, 32, 36
	Site(TEXT("WldS1D1VergeB"), -1, 20.3f, 4, 2, 1.f);      // left, s % 4 == 2: slots 10, 14, 22, 26, 30, 34
	Site(TEXT("WldS1D1VergeA"), 1, 20.3f, 4, 2, 1.f);      // right, s % 4 == 2: slots 10, 14, 22, 26, 30, 34
Zone(TEXT("World s1d1, The reef: the lane"), 0.48f, 0.97f);   // fix round: as the verge
	Site(TEXT("WldS1D1LaneA"), -1, 25.9f, 4, 0, 1.f);       // left, s % 4 == 0: slots 12, 24, 28, 32, 36
	Site(TEXT("WldS1D1LaneB"), -1, 25.9f, 4, 2, 1.f);       // left, s % 4 == 2: slots 14, 22, 26, 30, 34
	Site(TEXT("WldS1D1LaneA"), 1, 25.9f, 4, 2, 1.f);       // right, s % 4 == 2: slots 14, 22, 26, 30, 34
	Site(TEXT("WldS1D1LaneB"), 1, 25.9f, 12, 0, 1.f);       // right, s % 12 == 0: slots 12, 24, 36
	Site(TEXT("WldS1D1LaneB"), 1, 25.9f, 12, 8, 1.f);       // right, s % 12 == 4: slots 28
#undef S1D1
// ---- END s1d1 ----
}

{
// ---- BEGIN s1d2 (district 2 "The kelp forest", 1.7 to 3.4 km, ft_world_sea_s1d2.py) ----
// The kelp farmers' stilt village: 17 meshes (7 buildings, 9 props, the hero THE REEL), /Game/World/UnderTheSea/SM_S1D2_*.
// Slots: the district is slots 38 to 75. Scatter and Site rules stand only in 44-56, 58-61 and 66-70 (38-43 is a glass
// tunnel, 57 the jump's gap, 62-65 the corkscrew, 71-75 the loop); this block takes the even ones: 44 46 48 50 52 54 56
// 58 60 66 68 70 (s % 4 == 0 or 2; every 8 with phase 0/6/4/2 = s % 8 == 0/2/4/6).
// Bands: LOW props 18.9 to 19.2 m (only beside the rise and the last mild bend: nothing low or near on the banked
// S-bend, slots 44-52); TALL props 40 m (fix round; were 37.5 to 39; behind the divers and farmers at 26-36 m, before the masts at 46-50 m);
// buildings 44.5 m (only in slots where pass 3's masts do not stand: left s % 8 == 2, 4; right s % 8 == 0, 6) and
// 63 to 65 m; a rope bridge at 55 m; the hero 145 m out on the left in slots 46 (2.08 km) and 68 (3.08 km).
// Every roof stands 40 m or more out. Glow (float lanterns, windows): pale glass and cream, steady, nearest 37 m.
// Estimate by the brief's sum: farms 37.6 slots / 8 x 0.75 = 3.52 a rule x 50 parts (two sides x 6+6+7+6) = 176;
// the hero 2; verge 8.84 / 4 x 0.75 x 2 parts x 4 rules = 13.3 and 4.86 / 4 x 0.75 x 2 x 4 = 7.3; Far 14 slots / 4 x 8
// rules = 28: about 227. Counted by the slots themselves: 24 farms = 150, verge 28, Far 28, the hero 2 = 208.
// The most of one mesh: 18 by slots (21 by the sum): FloatLantern, DryingRack, BaleStack. The hero: 2.
// The LAST THREE Zones use Far as a land rule, the way this level's own tables dress its tunnels and set pieces
// (drop = the sand's depth under the road there): buildings only, 44.5 m and more out. They come last so that taking
// them out moves nothing else.
#define S1D2(n) TEXT("/Game/World/UnderTheSea/SM_S1D2_" n)
// fix round: every prop of the four farms stood 37.5 to 39 m out, also beside the banked S-bend (slots 45-52), where 17 to 40 m floats
// over the cutting: they stand 40 m out now (out -2 from the 42 m anchor).
Set(TEXT("WldS1D2FarmA"), {                         // { mesh, along m, out m, up m, yaw deg, scale }: anchor 42 m. No house in front (the masts' slots)
	{ S1D2("StiltHouse"),  -16.f,  22.f,  0.f,   5.f, 1.f },
	{ S1D2("DryingBarn"),    5.f,  23.f,  0.f,   0.f, 1.f },
	{ S1D2("RopeBridge"),   -3.f,  13.f,  0.f,   0.f, 1.f },
	{ S1D2("FloatLantern"), -14.f, -2.f, 0.f,   0.f, 1.f },
	{ S1D2("DryingRack"),   -4.f,  -2.f, 0.f,   4.f, 1.f },
	{ S1D2("BaleStack"),     8.f,  -2.f,  0.f, -12.f, 1.f },
});
Set(TEXT("WldS1D2FarmB"), {                         // the cutters' lodging in front, the rope walk behind
	{ S1D2("TallHouse"),     9.f,   2.5f, 0.f,   0.f, 1.f },
	{ S1D2("RopeWalk"),      0.f,  23.f,  0.f,   0.f, 1.f },
	{ S1D2("BellPost"),      0.f,  -2.f, 0.f,   0.f, 1.f },
	{ S1D2("ScytheRack"),   -8.f,  -2.f,  0.f,  -5.f, 1.f },
	{ S1D2("FloatLantern"), -16.f, -2.f, 0.f, 180.f, 1.f },
	{ S1D2("DryingRack"),   18.5f, -2.f, 0.f,  -3.f, 1.f },
});
Set(TEXT("WldS1D2FarmC"), {                         // the press house in front; the float store, a bridge and a house behind
	{ S1D2("PressHouse"),   -9.f,   2.5f, 0.f,   0.f, 1.f },
	{ S1D2("FloatStore"),  -13.f,  22.f,  0.f,   0.f, 1.f },
	{ S1D2("RopeBridge"),    4.f,  22.5f, 0.f,   0.f, 1.f },
	{ S1D2("StiltHouse"),   18.5f, 22.f,  0.f,  -6.f, 1.f },
	{ S1D2("BaleStack"),     0.f,  -2.f,  0.f,  15.f, 1.f },
	{ S1D2("DryingRack"),    8.f,  -2.f, 0.f,   0.f, 1.f },
	{ S1D2("BellPost"),     16.f,  -2.f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D2FarmD"), {                         // no house in front (the masts' slots): the look-out, a lodging and a barn behind
	{ S1D2("MastHouse"),   -17.f,  21.f,  0.f,   0.f, 1.f },
	{ S1D2("TallHouse"),    -6.f,  22.f,  0.f,   4.f, 1.f },
	{ S1D2("DryingBarn"),   11.f,  23.f,  0.f,   0.f, 1.f },
	{ S1D2("ScytheRack"),    3.f,  -2.f,  0.f,   6.f, 1.f },
	{ S1D2("FloatLantern"), -6.f,  -2.f, 0.f,   0.f, 1.f },
	{ S1D2("BaleStack"),   -14.f,  -2.f,  0.f,   8.f, 1.f },
});
Set(TEXT("WldS1D2VergeA"), {                        // LOW, anchor 18.9 m
	{ S1D2("RopeSledge"),   -8.f,   0.f,  0.f,   0.f, 1.f },
	{ S1D2("SheafRow"),      7.f,   0.3f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D2VergeB"), {
	{ S1D2("PlankWalk"),    -7.f,   0.f,  0.f,   0.f, 1.f },
	{ S1D2("RopeSledge"),    8.f,   0.2f, 0.f,   8.f, 1.f },
});
Set(TEXT("WldS1D2VergeC"), {
	{ S1D2("SheafRow"),     -8.f,   0.2f, 0.f,   0.f, 1.f },
	{ S1D2("PlankWalk"),     7.f,   0.f,  0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D2Reel"), { { S1D2("TheReel"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Zone(TEXT("World s1d2, the kelp forest: the farms"), 1.7f, 3.4f);
	Site(TEXT("WldS1D2FarmA"), -1, 42.f, 8, 0, 1.f);        // left, s % 8 == 0: 48 56
	Site(TEXT("WldS1D2FarmB"), -1, 42.f, 8, 6, 1.f);        // left, s % 8 == 2: 50 58 66
	Site(TEXT("WldS1D2FarmC"), -1, 42.f, 8, 4, 1.f);        // left, s % 8 == 4: 44 52 60 68
	Site(TEXT("WldS1D2FarmD"), -1, 42.f, 8, 2, 1.f);        // left, s % 8 == 6: 46 54 70
	Site(TEXT("WldS1D2FarmC"), 1, 42.f, 8, 0, 1.f);         // right, s % 8 == 0
	Site(TEXT("WldS1D2FarmD"), 1, 42.f, 8, 6, 1.f);         // right, s % 8 == 2
	Site(TEXT("WldS1D2FarmA"), 1, 42.f, 8, 4, 1.f);         // right, s % 8 == 4
	Site(TEXT("WldS1D2FarmB"), 1, 42.f, 8, 2, 1.f);         // right, s % 8 == 6
	Site(TEXT("WldS1D2Reel"), -1, 145.f, 44, 42, 1.f);      // THE REEL, left, slot 46 (2.08 km, the S-bend)
	Site(TEXT("WldS1D2Reel"), -1, 145.f, 44, 20, 1.f);      // THE REEL, left, slot 68 (3.08 km, before the loop)
Zone(TEXT("World s1d2, the kelp forest: the verge by the rise"), 2.38f, 2.78f);          // slots 53 to 61
	Site(TEXT("WldS1D2VergeA"), -1, 18.9f, 4, 0, 1.f);
	Site(TEXT("WldS1D2VergeB"), -1, 18.9f, 4, 2, 1.f);
	Site(TEXT("WldS1D2VergeC"), 1, 18.9f, 4, 0, 1.f);
	Site(TEXT("WldS1D2VergeA"), 1, 18.9f, 4, 2, 1.f);
Zone(TEXT("World s1d2, the kelp forest: the verge before the loop"), 2.97f, 3.19f);     // slots 66 to 70
	Site(TEXT("WldS1D2VergeA"), -1, 18.9f, 4, 0, 1.f);
	Site(TEXT("WldS1D2VergeB"), -1, 18.9f, 4, 2, 1.f);
	Site(TEXT("WldS1D2VergeC"), 1, 18.9f, 4, 0, 1.f);
	Site(TEXT("WldS1D2VergeA"), 1, 18.9f, 4, 2, 1.f);
Zone(TEXT("World s1d2, the kelp forest: by the glass tunnel"), 1.7f, 1.965f);           // slots 38 to 43: Far as a land rule
	Far(S1D2("MastHouse"), 44.5f, 46.f, 4, 0, 1.f, 1.f, 0.f, -1, 0.95f, 4.f);
	Far(S1D2("RopeWalk"), 63.f, 65.f, 4, 0, 1.f, 1.f, 0.f, -1, 1.02f, 3.f);
	Far(S1D2("PressHouse"), 44.5f, 46.f, 4, 2, 1.f, 1.f, 0.f, -1, 0.95f, 4.f);
	Far(S1D2("FloatStore"), 63.f, 65.f, 4, 2, 1.f, 1.f, 0.f, -1, 1.02f, 4.f);
	Far(S1D2("FloatStore"), 44.5f, 46.f, 4, 0, 1.f, 1.f, 0.f, 1, 0.95f, 4.f);
	Far(S1D2("PressHouse"), 63.f, 65.f, 4, 0, 1.f, 1.f, 0.f, 1, 1.02f, 4.f);
	Far(S1D2("RopeWalk"), 44.5f, 46.f, 4, 2, 1.f, 1.f, 0.f, 1, 0.95f, 3.f);
	Far(S1D2("MastHouse"), 63.f, 65.f, 4, 2, 1.f, 1.f, 0.f, 1, 1.02f, 4.f);
Zone(TEXT("World s1d2, the kelp forest: under the corkscrew"), 2.79f, 2.96f);           // slots 62 to 65: Far as a land rule
	Far(S1D2("MastHouse"), 44.5f, 46.f, 4, 0, 1.f, 1.f, 0.f, -1, 1.f, 4.f);
	Far(S1D2("RopeWalk"), 63.f, 65.f, 4, 0, 1.f, 1.f, 0.f, -1, 1.f, 3.f);
	Far(S1D2("PressHouse"), 44.5f, 46.f, 4, 2, 1.f, 1.f, 0.f, -1, 1.f, 4.f);
	Far(S1D2("FloatStore"), 63.f, 65.f, 4, 2, 1.f, 1.f, 0.f, -1, 1.f, 4.f);
	Far(S1D2("FloatStore"), 44.5f, 46.f, 4, 0, 1.f, 1.f, 0.f, 1, 1.f, 4.f);
	Far(S1D2("PressHouse"), 63.f, 65.f, 4, 0, 1.f, 1.f, 0.f, 1, 1.f, 4.f);
	Far(S1D2("RopeWalk"), 44.5f, 46.f, 4, 2, 1.f, 1.f, 0.f, 1, 1.f, 3.f);
	Far(S1D2("MastHouse"), 63.f, 65.f, 4, 2, 1.f, 1.f, 0.f, 1, 1.f, 4.f);
Zone(TEXT("World s1d2, the kelp forest: under the loop"), 3.2f, 3.4f);                  // slots 71 to 75: Far as a land rule
	Far(S1D2("MastHouse"), 44.5f, 46.f, 4, 0, 1.f, 1.f, 0.f, -1, 1.f, 4.f);
	Far(S1D2("RopeWalk"), 63.f, 65.f, 4, 0, 1.f, 1.f, 0.f, -1, 1.f, 3.f);
	Far(S1D2("PressHouse"), 44.5f, 46.f, 4, 2, 1.f, 1.f, 0.f, -1, 1.f, 4.f);
	Far(S1D2("FloatStore"), 63.f, 65.f, 4, 2, 1.f, 1.f, 0.f, -1, 1.f, 4.f);
	Far(S1D2("FloatStore"), 44.5f, 46.f, 4, 0, 1.f, 1.f, 0.f, 1, 1.f, 4.f);
	Far(S1D2("PressHouse"), 63.f, 65.f, 4, 0, 1.f, 1.f, 0.f, 1, 1.f, 4.f);
	Far(S1D2("RopeWalk"), 44.5f, 46.f, 4, 2, 1.f, 1.f, 0.f, 1, 1.f, 3.f);
	Far(S1D2("MastHouse"), 63.f, 65.f, 4, 2, 1.f, 1.f, 0.f, 1, 1.f, 4.f);
#undef S1D2
// ---- END s1d2 ----
}

{
// ---- BEGIN s1d3 (district 3 "The research station", ft_world_sea_s1d3.py) ----
// 17 meshes (7 buildings, 9 props, 1 hero), /Game/World/UnderTheSea/SM_S1D3_*. Only inside 3.4 to 5.0 km, only even slots.
// Estimate, counted slot by slot (the land slots of the district are 77-78, 84-100, 109-110: the loop takes 76, the glass
// tunnels 79-83 and 101-108): 4x8 + 2x8 + 3x7 + 2x8 + 5x5 + 6x1 + 1x3 + 1x1 + 1x3 + 1x3 + 1x3 + 1x3 + 3x3 + 2x1 + 2x3 + 3x3 + 2x3 + 3x3 + 1x1 + 1x1 = 175 placed
// (slots x parts, one term a rule, in the order below). By the brief's formula (22.1 slots a km / every x 0.75 x parts): about 218.
// Most of one mesh: FloodMast 24, CurrentVane 22, AntennaFarm 20. Smallest x of a TALL mesh: SignPost 34.5 m; of a mesh with glow: FloodMast 37.4 m.
// Bands and slots: verge 22.5 m (left) and 23.8 m (right), low pieces only, and 34.5 to 41.5 m tall props: only outside the steep
// banked S-bend (slots 85-93), so two sub-districts (slots 77-84 and 94-110), but the vanes at 40.5 m (the bend's rule: 40 m);
// 60 to 70 m the campus yards, every even land slot (left s % 8 == 6 / 2 and s % 4 == 0; right s % 8 == 4 / 0); the Big Ear
// 125 m out at slot 86 (right) and slot 100 (left; fix round, was 94).
#define S1D3(n) TEXT("/Game/World/UnderTheSea/SM_S1D3_" n)
// fix round: the greenhouse stood on the kit's marker post (13.4 to 16.3 m along): 6 m further along, the aerials before the post
Set(TEXT("WldS1D3LabCourt"), {         // the laboratory court: the lab between the kit's two marker posts, deck house and greenhouse either side
	{ S1D3("DrumLab"), 0.f, 3.5f, 0.f, 0.f, 1.f },
	{ S1D3("DeckHouse"), -16.5f, 3.9f, 0.f, 0.f, 1.f },
	{ S1D3("Greenhouse"), 23.f, 3.9f, 0.f, 0.f, 1.f },
	{ S1D3("FloodMast"), -7.5f, -4.6f, 0.f, 0.f, 1.f },
	{ S1D3("SignPost"), 7.f, -5.f, 0.f, 12.f, 1.f },
	{ S1D3("CurrentVane"), -21.5f, -4.5f, 0.f, 0.f, 1.f },
	{ S1D3("AntennaFarm"), 11.f, -4.6f, 0.f, -10.f, 1.f },
	{ S1D3("Lockers"), -2.5f, -6.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3TankYard"), {         // the tank yard: the specimen tank, the current tower, sample racks
	{ S1D3("TankHall"), 2.f, 3.5f, 0.f, 0.f, 1.f },
	{ S1D3("CurrentTower"), -17.f, 3.f, 0.f, 0.f, 1.f },
	{ S1D3("FloodMast"), -3.f, -4.6f, 0.f, 0.f, 1.f },
	{ S1D3("SignPost"), 6.f, -5.f, 0.f, -10.f, 1.f },
	{ S1D3("AntennaFarm"), 20.5f, -4.6f, 0.f, 8.f, 1.f },
	{ S1D3("CurrentVane"), -21.5f, -4.5f, 0.f, 0.f, 1.f },
	{ S1D3("SampleRack"), -12.f, -6.f, 0.f, 5.f, 1.f },
	{ S1D3("Manifold"), 11.f, -6.f, 0.f, -4.f, 1.f },
});
Set(TEXT("WldS1D3DormRing"), {         // the dormitory ring, a lab and the current tower
	{ S1D3("RingDorm"), -1.f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D3("DrumLab"), 16.f, 3.2f, 0.f, 0.f, 1.f },
	{ S1D3("CurrentTower"), -16.5f, 2.5f, 0.f, 0.f, 1.f },
	{ S1D3("FloodMast"), 7.5f, -5.5f, 0.f, 0.f, 1.f },
	{ S1D3("SignPost"), -6.5f, -7.f, 0.f, 10.f, 1.f },
	{ S1D3("AntennaFarm"), -21.f, -4.5f, 0.f, 12.f, 1.f },
	{ S1D3("CurrentVane"), 21.5f, -5.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3WorkYard"), {         // the work yard: the workshop and its gantry, a greenhouse, a deck house
	{ S1D3("Workshop"), -3.f, 2.f, 0.f, 0.f, 1.f },
	{ S1D3("Greenhouse"), 23.f, 3.f, 0.f, 0.f, 1.f },
	{ S1D3("DeckHouse"), -17.f, 3.5f, 0.f, 0.f, 1.f },
	{ S1D3("FloodMast"), 9.f, -6.f, 0.f, 0.f, 1.f },
	{ S1D3("SignPost"), -6.5f, -7.f, 0.f, -8.f, 1.f },
	{ S1D3("AntennaFarm"), 14.f, -7.5f, 0.f, -12.f, 1.f },
	{ S1D3("CurrentVane"), -21.5f, -5.f, 0.f, 0.f, 1.f },
	{ S1D3("Lockers"), 4.f, -3.4f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3DormRow"), {          // behind the kit's habitat domes: a dormitory ring and a lab
	{ S1D3("RingDorm"), -10.f, 3.f, 0.f, 0.f, 1.f },
	{ S1D3("DrumLab"), 7.5f, 3.5f, 0.f, 0.f, 1.f },
	{ S1D3("FloodMast"), -1.2f, -3.5f, 0.f, 0.f, 1.f },
	{ S1D3("AntennaFarm"), -21.f, -3.f, 0.f, 10.f, 1.f },
	{ S1D3("CurrentVane"), 14.5f, -3.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3TubeSide"), {         // behind the travel tube: two floodlight masts and the aerials
	{ S1D3("FloodMast"), -14.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("AntennaFarm"), 0.f, 0.3f, 0.f, 0.f, 1.f },
	{ S1D3("FloodMast"), 15.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3VergeRA"), {          // right verge, low: survey gear
	{ S1D3("Tripod"), -12.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("SampleRack"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("Manifold"), 12.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3VergeRB"), {          // right verge, low: lockers and a cable drum
	{ S1D3("Lockers"), -11.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("CableDrum"), 1.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("Tripod"), 12.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3VergeLA"), {          // left verge, low, behind the pipeline: its valves and cable
	{ S1D3("Manifold"), -10.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("CableDrum"), 2.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("SampleRack"), 13.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3VergeLB"), {          // left verge, low, behind the pipeline
	{ S1D3("Tripod"), -12.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("Lockers"), 0.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D3("Manifold"), 12.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3BigEar"), {           // THE BIG EAR
	{ S1D3("BigEar"), 0.f, 0.f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1d3, the research station"), 3.4f, 5.0f);
	Site(TEXT("WldS1D3LabCourt"), -1, 66.f, 8, 2, 1.f);                          // slots 78 86 94 110: left, s % 8 == 6: behind the gantries (50-54 m), in front of the outpost (hull from 79.7 m)
	Site(TEXT("WldS1D3TankYard"), -1, 66.f, 8, 6, 1.f);                          // slots 90 98: left, s % 8 == 2
	Site(TEXT("WldS1D3DormRing"), 1, 66.f, 8, 4, 1.f);                           // slots 84 92 100: right, s % 8 == 4: behind the terraces and gantries (52-56 m), in front of the outpost (hull from 75.7 m)
	Site(TEXT("WldS1D3WorkYard"), 1, 66.f, 8, 0, 1.f);                           // slots 88 96: right, s % 8 == 0
	Site(TEXT("WldS1D3DormRow"), -1, 66.f, 4, 0, 1.f);                           // slots 84 88 92 96 100: left, s % 4 == 0: behind the habitat domes (to 60.6 m; the small dome from 18 m along)
	Scatter(S1D3("CurrentVane"), 40.5f, 41.5f, 4, 2, 1.f, 1.f, 0.f, -1, 20.f);   // slots 78 86 90 94 98 110: left, s % 4 == 2: between the watch balconies (33-37 m) and the mast crews (44-50 m)
Zone(TEXT("World s1d3, the station before the steep bend"), 3.4611f, 3.8230f);
	Site(TEXT("WldS1D3TubeSide"), 1, 37.4f, 4, 2, 1.f);                          // slots 78: right, s % 4 == 2: behind the travel tube (its back 34.4 m)
	Scatter(S1D3("SignPost"), 34.5f, 35.5f, 4, 0, 1.f, 1.f, 0.f, -1, 15.f);      // slots 84: left, s % 4 == 0: between the movers (to 30.5 m) and the film unit (38.5 m)
	Site(TEXT("WldS1D3VergeRA"), 1, 23.8f, 4, 0, 1.f);                           // slots 84: right verge, low, behind the pressure vessels (18.3-21 m)
	Site(TEXT("WldS1D3VergeRB"), 1, 23.8f, 4, 2, 1.f);                           // slots 78
	Site(TEXT("WldS1D3VergeLA"), -1, 22.5f, 4, 0, 1.f);                          // slots 84: left verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D3VergeLB"), -1, 22.5f, 4, 2, 1.f);                          // slots 78
Zone(TEXT("World s1d3, the station after the steep bend"), 4.2302f, 4.95f);   // fix round: to slot 109: at slot 110 the next banked S-bend (bank .35) begins
	Site(TEXT("WldS1D3TubeSide"), 1, 37.4f, 4, 2, 1.f);                          // slots 94 98 110
	Scatter(S1D3("SignPost"), 34.5f, 35.5f, 4, 0, 1.f, 1.f, 0.f, -1, 15.f);      // slots 96 100
	Site(TEXT("WldS1D3VergeRA"), 1, 23.8f, 4, 0, 1.f);                           // slots 96 100
	Site(TEXT("WldS1D3VergeRB"), 1, 23.8f, 4, 2, 1.f);                           // slots 94 98 110
	Site(TEXT("WldS1D3VergeLA"), -1, 22.5f, 4, 0, 1.f);                          // slots 96 100
	Site(TEXT("WldS1D3VergeLB"), -1, 22.5f, 4, 2, 1.f);                          // slots 94 98 110
Zone(TEXT("World s1d3, the Big Ear, right"), 3.8683f, 3.9135f);
	Site(TEXT("WldS1D3BigEar"), 1, 125.f, 4, 2, 1.f);                            // slots 86: slot 86 (3.89 km), right
Zone(TEXT("World s1d3, the Big Ear, left"), 4.5017f, 4.5424f);   // fix round: slot 100 (4.52 km), on the straight before the tunnel; at slot 94 it stood at the exit of the steepest S-bend and no frame showed it
	Site(TEXT("WldS1D3BigEar"), -1, 125.f, 4, 0, 1.f);                           // slot 100 (4.52 km), left
#undef S1D3
// ---- END s1d3 ----
}

{
// ---- BEGIN s1d4 (district 4 "The salvage field and the approach", ft_world_sea_s1d4.py) ----
// THE WRECKERS TOWN, 5.0 to 7.94 km (slots 111 to 175; this block uses the even ones, both sides). 16 meshes:
// 7 buildings, 8 props, 1 hero. Every rule has every 64, chance 1 and takes ONE slot (phase = (64 - slot % 64) % 64;
// each Zone is under 64 slots long), so each line is one Set or one piece; the comment after it names slot and side.
// Placed, before the track makes its own tests: 187 by 51 Site rules + 49 by Far rules = 236.
// Estimate by the rule of the brief: Site 187 x 0.75 = 140.2, Far 49 x 1 = 49: about 189 placed.
// By mesh: HullHouse 9, FunnelTower 15, BridgeInn 11, JibWorkshop 5, LifeboatRow 7, ForgeRock 15, Tenement 11, ChainFence 19, PortholeLamp 39, PropMonument 18, WheelCapstan 15, AwningStall 20, WinchNet 16, SignalMast 22, BoilerPlanter 12, StandingStern 2.
// Buildings 73, props 161, hero 2. Most of any one mesh: 39 (PortholeLamp).
// WHY so far out: of the 33 even slots of the district only 18 are land (the rest: the gap of the jump, the
// corkscrew, two glass tunnels, the loop). Of those 18, 6 lie in the two banked S-bends of the field (112 to 116,
// 132 to 136), 5 in the banked bend of the approach (156 to 164) and 3 at the run-up and landing of the jump (118,
// 120, 124), where the brief says 40 m or more (the cutting; the track clears 40 m at a jump). So the town stands
// 40.8 to 70 m out, tall props in their yards with the houses; only slots 126, 138, 166 and 174 (off the bends by
// the section table of the plan) carry the street by the road (low 20.2 / 22.6 m; tall 30.0 to 38.6 m). In the 13
// even slots of the corkscrew, the tunnels and the loop the track leaves Scatter and Site out: there single pieces
// stand by Far rules, as the tunnel districts of the dressing do (drop = the sand: 0.96 m at 41 to 44 m, 1.0 at
// 47.5 and 54 m, 1.02 at 64 m; read from the dressing table, not measured).
// Bands: Sets WldS1D4HullYard/Stack/Workshop/Boats/Inn anchor 62 (65 in slot 120): props 42.5 to 49, houses 65 to 67.
// WldS1D4CampA/B anchor 44 (40.8 to 45.5): where the Salvage set (48 to 88 m) or a herd has the ground behind.
// WldS1D4RockR1..3 anchor 47 (right: 42.5 to 49 and 63 to 67.5); WldS1D4RockBackA..C anchor 66 (64 to 67.5).
// Hero: slot 134 right at 142 m, slot 160 left at 108 m. Smallest x min of a TALL mesh: 30.0 m (PortholeLamp, Set WldS1D4Kerb1 in slots 126 and 138 left);
// of a low mesh: 20.2 m (BoilerPlanter). Steady glow only: PortholeLamp (nearest 30.0 m), houses and hero; nothing blinks.
#define S1D4(n) TEXT("/Game/World/UnderTheSea/SM_S1D4_" n)
Set(TEXT("WldS1D4HullYard"), {
	{ S1D4("HullHouse"), 0.f, 4.f, 0.f, 0.f, 1.f },
	{ S1D4("WinchNet"), 15.f, -15.5f, 0.f, 0.f, 1.f },
	{ S1D4("PortholeLamp"), -11.f, -17.f, 0.f, 0.f, 1.f },
	{ S1D4("BoilerPlanter"), -1.f, -18.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Stack"), {
	{ S1D4("Tenement"), 0.f, 4.5f, 0.f, 0.f, 1.f },
	{ S1D4("AwningStall"), 11.f, -16.f, 0.f, 0.f, 1.f },
	{ S1D4("SignalMast"), -12.f, -15.f, 0.f, 0.f, 1.f },
	{ S1D4("ChainFence"), -1.f, -19.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Workshop"), {
	{ S1D4("JibWorkshop"), -4.f, 4.f, 0.f, 0.f, 1.f },
	{ S1D4("PropMonument"), 13.f, -16.f, 0.f, 0.f, 1.f },
	{ S1D4("WheelCapstan"), -9.f, -17.f, 0.f, 0.f, 1.f },
	{ S1D4("PortholeLamp"), 2.f, -18.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Boats"), {
	{ S1D4("LifeboatRow"), -6.f, 3.f, 0.f, 0.f, 1.f },
	{ S1D4("FunnelTower"), 14.f, 4.f, 0.f, 0.f, 1.f },
	{ S1D4("AwningStall"), -8.f, -16.f, 0.f, 0.f, 1.f },
	{ S1D4("PortholeLamp"), 6.f, -17.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Inn"), {
	{ S1D4("BridgeInn"), 2.f, 3.5f, 0.f, 0.f, 1.f },
	{ S1D4("FunnelTower"), -14.f, 4.f, 0.f, 0.f, 1.f },
	{ S1D4("SignalMast"), 13.f, -15.f, 0.f, 0.f, 1.f },
	{ S1D4("ChainFence"), -3.f, -19.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4CampA"), {
	{ S1D4("AwningStall"), 0.f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D4("PortholeLamp"), 9.f, 1.f, 0.f, 0.f, 1.f },
	{ S1D4("WheelCapstan"), -9.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("ChainFence"), 1.f, -3.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4CampB"), {
	{ S1D4("WinchNet"), 0.f, 1.5f, 0.f, 0.f, 1.f },
	{ S1D4("SignalMast"), 10.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("BoilerPlanter"), -9.f, -1.f, 0.f, 0.f, 1.f },
	{ S1D4("PropMonument"), -16.f, 1.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4RockR1"), {
	{ S1D4("ForgeRock"), 0.f, 1.5f, 0.f, 0.f, 1.f },
	{ S1D4("PortholeLamp"), 11.f, -3.f, 0.f, 0.f, 1.f },
	{ S1D4("WheelCapstan"), -11.f, -2.5f, 0.f, 0.f, 1.f },
	{ S1D4("BridgeInn"), 4.f, 20.f, 0.f, 0.f, 1.f },
	{ S1D4("SignalMast"), -12.f, 19.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4RockR2"), {
	{ S1D4("FunnelTower"), 0.f, 2.f, 0.f, 0.f, 1.f },
	{ S1D4("AwningStall"), -11.f, -1.f, 0.f, 0.f, 1.f },
	{ S1D4("ChainFence"), 10.f, -4.5f, 0.f, 0.f, 1.f },
	{ S1D4("HullHouse"), -2.f, 20.f, 0.f, 0.f, 1.f },
	{ S1D4("WinchNet"), 15.f, 18.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4RockR3"), {
	{ S1D4("ForgeRock"), 0.f, 1.5f, 0.f, 0.f, 1.f },
	{ S1D4("BoilerPlanter"), 11.f, -3.5f, 0.f, 0.f, 1.f },
	{ S1D4("PropMonument"), -12.f, -2.f, 0.f, 0.f, 1.f },
	{ S1D4("Tenement"), 0.f, 20.5f, 0.f, 0.f, 1.f },
	{ S1D4("PortholeLamp"), -12.f, 16.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4RockBackA"), {
	{ S1D4("SignalMast"), -19.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("PortholeLamp"), -12.5f, -1.5f, 0.f, 0.f, 1.f },
	{ S1D4("ForgeRock"), -3.f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D4("WheelCapstan"), 7.f, -2.f, 0.f, 0.f, 1.f },
	{ S1D4("FunnelTower"), 16.f, 1.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4RockBackB"), {
	{ S1D4("PropMonument"), -18.f, -1.f, 0.f, 0.f, 1.f },
	{ S1D4("ForgeRock"), -8.f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D4("BridgeInn"), 5.f, 1.f, 0.f, 0.f, 1.f },
	{ S1D4("AwningStall"), 14.5f, -1.f, 0.f, 0.f, 1.f },
	{ S1D4("PortholeLamp"), 19.5f, -1.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4RockBackC"), {
	{ S1D4("WinchNet"), -19.f, -1.f, 0.f, 0.f, 1.f },
	{ S1D4("Tenement"), -8.5f, 1.5f, 0.f, 0.f, 1.f },
	{ S1D4("LifeboatRow"), 9.5f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("PortholeLamp"), 21.f, -1.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Verge1"), {
	{ S1D4("ChainFence"), -8.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("WheelCapstan"), 9.f, 0.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Verge2"), {
	{ S1D4("BoilerPlanter"), -8.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("ChainFence"), 8.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Kerb1"), {
	{ S1D4("PortholeLamp"), -11.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("AwningStall"), 1.f, 0.4f, 0.f, 0.f, 1.f },
	{ S1D4("PortholeLamp"), 13.f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Kerb2"), {
	{ S1D4("SignalMast"), -10.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("PortholeLamp"), 2.f, 0.f, 0.f, 0.f, 1.f },
	{ S1D4("PropMonument"), 13.f, 0.3f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D4Stern"), {
	{ S1D4("StandingStern"), 0.f, 0.f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1d4, the salvage field: the wreckers town"), 5.0f, 6.6f);
	// the town on the land slots (112 to 126, 132 to 138): a yard a side a slot, 40.8 m and more out
	Site(TEXT("WldS1D4HullYard"), -1, 62.f, 64, 16, 1.f);   // slot 112 L
	Site(TEXT("WldS1D4Inn"), 1, 62.f, 64, 16, 1.f);   // slot 112 R
	Site(TEXT("WldS1D4CampA"), -1, 44.f, 64, 14, 1.f);   // slot 114 L: the Salvage set and the old wreck have 48 to 88 m here
	Site(TEXT("WldS1D4Stack"), 1, 62.f, 64, 14, 1.f);   // slot 114 R
	Site(TEXT("WldS1D4CampB"), -1, 44.f, 64, 12, 1.f);   // slot 116 L: the manatee herd has 62 to 100 m here
	Site(TEXT("WldS1D4CampB"), 1, 44.f, 64, 12, 1.f);   // slot 116 R: the Salvage set has 48 to 88 m here
	Site(TEXT("WldS1D4Boats"), -1, 62.f, 64, 10, 1.f);   // slot 118 L
	Site(TEXT("WldS1D4Workshop"), 1, 62.f, 64, 10, 1.f);   // slot 118 R
	Site(TEXT("WldS1D4HullYard"), 1, 65.f, 64, 8, 1.f);   // slot 120 R: 3 m further out: a film unit stands at 44 to 45 m by the jump (and so nothing of this block on the left here)
	Site(TEXT("WldS1D4Stack"), -1, 62.f, 64, 4, 1.f);   // slot 124 L
	Site(TEXT("WldS1D4Boats"), 1, 62.f, 64, 4, 1.f);   // slot 124 R
	Site(TEXT("WldS1D4CampB"), -1, 44.f, 64, 2, 1.f);   // slot 126 L
	Site(TEXT("WldS1D4Stack"), 1, 62.f, 64, 2, 1.f);   // slot 126 R
	Site(TEXT("WldS1D4CampA"), -1, 44.f, 64, 60, 1.f);   // slot 132 L
	Site(TEXT("WldS1D4Inn"), 1, 62.f, 64, 60, 1.f);   // slot 132 R
	Site(TEXT("WldS1D4Workshop"), -1, 62.f, 64, 58, 1.f);   // slot 134 L
	Site(TEXT("WldS1D4CampA"), 1, 44.f, 64, 58, 1.f);   // slot 134 R
	Site(TEXT("WldS1D4Inn"), -1, 62.f, 64, 56, 1.f);   // slot 136 L
	Site(TEXT("WldS1D4HullYard"), 1, 62.f, 64, 56, 1.f);   // slot 136 R
	Site(TEXT("WldS1D4CampB"), -1, 44.f, 64, 54, 1.f);   // slot 138 L
	Site(TEXT("WldS1D4Workshop"), 1, 62.f, 64, 54, 1.f);   // slot 138 R
	// the two level slots of the field (126 by the corkscrew, 138 before the tunnel): the street by the road
	Site(TEXT("WldS1D4Verge1"), -1, 20.2f, 64, 2, 1.f);   // slot 126 L
	Site(TEXT("WldS1D4Verge2"), 1, 20.2f, 64, 2, 1.f);   // slot 126 R
	Site(TEXT("WldS1D4Kerb1"), -1, 30.f, 64, 2, 1.f);   // slot 126 L
	Site(TEXT("WldS1D4Kerb2"), 1, 34.5f, 64, 2, 1.f);   // slot 126 R
	Site(TEXT("WldS1D4Verge2"), -1, 20.2f, 64, 54, 1.f);   // slot 138 L
	Site(TEXT("WldS1D4Verge1"), 1, 20.2f, 64, 54, 1.f);   // slot 138 R
	Site(TEXT("WldS1D4Kerb1"), -1, 30.f, 64, 54, 1.f);   // slot 138 L
	Site(TEXT("WldS1D4Kerb2"), 1, 34.5f, 64, 54, 1.f);   // slot 138 R
	// the hero
	Site(TEXT("WldS1D4Stern"), 1, 142.f, 64, 58, 1.f);   // slot 134 R: THE STANDING STERN, right, 6.08 km
	// under the corkscrew (128, 130) and beside the glass tunnel (140 to 144): Far rules as land rules, the drop is the sand
	Far(S1D4("HullHouse"), 54.f, 54.f, 64, 0, 1.f, 1.f, 0.f, -1, 1.f, 4.f);   // slot 128 L
	Far(S1D4("PortholeLamp"), 41.f, 41.f, 64, 0, 1.f, 1.f, 0.f, -1, 0.96f, 4.f);   // slot 128 L
	Far(S1D4("Tenement"), 54.f, 54.f, 64, 0, 1.f, 1.f, 0.f, 1, 1.f, 4.f);   // slot 128 R
	Far(S1D4("JibWorkshop"), 54.f, 54.f, 64, 62, 1.f, 1.f, 0.f, -1, 1.f, 4.f);   // slot 130 L
	Far(S1D4("WinchNet"), 41.f, 41.f, 64, 62, 1.f, 1.f, 0.f, -1, 0.96f, 4.f);   // slot 130 L
	Far(S1D4("LifeboatRow"), 54.f, 54.f, 64, 62, 1.f, 1.f, 0.f, 1, 1.f, 4.f);   // slot 130 R
	Far(S1D4("SignalMast"), 41.f, 41.f, 64, 62, 1.f, 1.f, 0.f, 1, 0.96f, 4.f);   // slot 130 R
	Far(S1D4("BridgeInn"), 47.5f, 47.5f, 64, 52, 1.f, 1.f, 0.f, -1, 1.f, 4.f);   // slot 140 L
	Far(S1D4("PropMonument"), 41.5f, 41.5f, 64, 52, 1.f, 1.f, 0.f, -1, 0.96f, 4.f);   // slot 140 L
	Far(S1D4("HullHouse"), 64.f, 64.f, 64, 52, 1.f, 1.f, 0.f, 1, 1.02f, 4.f);   // slot 140 R
	Far(S1D4("AwningStall"), 41.5f, 41.5f, 64, 52, 1.f, 1.f, 0.f, 1, 0.96f, 4.f);   // slot 140 R
	Far(S1D4("FunnelTower"), 47.5f, 47.5f, 64, 50, 1.f, 1.f, 0.f, -1, 1.f, 4.f);   // slot 142 L
	Far(S1D4("AwningStall"), 43.5f, 43.5f, 64, 50, 1.f, 1.f, 0.f, -1, 0.96f, 4.f);   // slot 142 L
	Far(S1D4("JibWorkshop"), 47.5f, 47.5f, 64, 50, 1.f, 1.f, 0.f, 1, 1.f, 4.f);   // slot 142 R
	Far(S1D4("PortholeLamp"), 41.5f, 41.5f, 64, 50, 1.f, 1.f, 0.f, 1, 0.96f, 4.f);   // slot 142 R
	Far(S1D4("Tenement"), 64.f, 64.f, 64, 48, 1.f, 1.f, 0.f, -1, 1.02f, 4.f);   // slot 144 L
	Far(S1D4("SignalMast"), 41.5f, 41.5f, 64, 48, 1.f, 1.f, 0.f, -1, 0.96f, 4.f);   // slot 144 L
	Far(S1D4("BridgeInn"), 47.5f, 47.5f, 64, 48, 1.f, 1.f, 0.f, 1, 1.f, 4.f);   // slot 144 R
	Far(S1D4("WinchNet"), 41.5f, 41.5f, 64, 48, 1.f, 1.f, 0.f, 1, 0.96f, 4.f);   // slot 144 R
Zone(TEXT("World s1d4, the approach: the town on the chimney rock"), 6.6f, 7.94f);
	// the land slots (156 to 166, 174)
	Site(TEXT("WldS1D4RockBackB"), -1, 66.f, 64, 36, 1.f);   // slot 156 L
	Site(TEXT("WldS1D4RockBackA"), 1, 66.f, 64, 36, 1.f);   // slot 156 R
	Site(TEXT("WldS1D4RockBackC"), -1, 66.f, 64, 34, 1.f);   // slot 158 L
	Site(TEXT("WldS1D4RockBackB"), 1, 66.f, 64, 34, 1.f);   // slot 158 R
	Site(TEXT("WldS1D4RockBackA"), -1, 66.f, 64, 32, 1.f);   // slot 160 L
	Site(TEXT("WldS1D4RockR1"), 1, 47.f, 64, 32, 1.f);   // slot 160 R
	Site(TEXT("WldS1D4RockBackA"), -1, 66.f, 64, 30, 1.f);   // slot 162 L
	Site(TEXT("WldS1D4RockBackC"), 1, 66.f, 64, 30, 1.f);   // slot 162 R
	Site(TEXT("WldS1D4RockBackB"), -1, 66.f, 64, 28, 1.f);   // slot 164 L
	Site(TEXT("WldS1D4RockR2"), 1, 47.f, 64, 28, 1.f);   // slot 164 R
	Site(TEXT("WldS1D4RockBackC"), -1, 66.f, 64, 26, 1.f);   // slot 166 L
	Site(TEXT("WldS1D4RockR3"), 1, 47.f, 64, 26, 1.f);   // slot 166 R
	Site(TEXT("WldS1D4RockBackA"), -1, 66.f, 64, 18, 1.f);   // slot 174 L
	Site(TEXT("WldS1D4RockR2"), 1, 47.f, 64, 18, 1.f);   // slot 174 R
	// the two level slots of the approach (166, 174): the street by the road
	Site(TEXT("WldS1D4Verge1"), -1, 20.2f, 64, 26, 1.f);   // slot 166 L
	Site(TEXT("WldS1D4Verge2"), 1, 20.2f, 64, 26, 1.f);   // slot 166 R
	Site(TEXT("WldS1D4Kerb1"), 1, 38.6f, 64, 26, 1.f);   // slot 166 R
	Site(TEXT("WldS1D4Verge2"), -1, 22.6f, 64, 18, 1.f);   // slot 174 L
	Site(TEXT("WldS1D4Verge1"), 1, 22.6f, 64, 18, 1.f);   // slot 174 R
	Site(TEXT("WldS1D4Kerb2"), 1, 30.5f, 64, 18, 1.f);   // slot 174 R
	// the hero
	Site(TEXT("WldS1D4Stern"), -1, 108.f, 64, 32, 1.f);   // slot 160 L: THE STANDING STERN, left, 7.26 km (108 m: station hubs are scattered from 120 m in every even slot here)
	// beside the tunnels (146, 168 to 172) and under the loop (148 to 154): Far rules as land rules
	Far(S1D4("LifeboatRow"), 47.5f, 47.5f, 64, 46, 1.f, 1.f, 0.f, -1, 1.f, 4.f);   // slot 146 L
	Far(S1D4("PortholeLamp"), 41.5f, 41.5f, 64, 46, 1.f, 1.f, 0.f, -1, 0.96f, 4.f);   // slot 146 L
	Far(S1D4("FunnelTower"), 64.f, 64.f, 64, 46, 1.f, 1.f, 0.f, 1, 1.02f, 4.f);   // slot 146 R
	Far(S1D4("PropMonument"), 41.5f, 41.5f, 64, 46, 1.f, 1.f, 0.f, 1, 0.96f, 4.f);   // slot 146 R
	Far(S1D4("ForgeRock"), 54.f, 54.f, 64, 44, 1.f, 1.f, 0.f, -1, 1.f, 4.f);   // slot 148 L
	Far(S1D4("PortholeLamp"), 41.f, 41.f, 64, 44, 1.f, 1.f, 0.f, -1, 0.96f, 4.f);   // slot 148 L
	Far(S1D4("FunnelTower"), 54.f, 54.f, 64, 44, 1.f, 1.f, 0.f, 1, 1.f, 4.f);   // slot 148 R
	Far(S1D4("SignalMast"), 41.f, 41.f, 64, 44, 1.f, 1.f, 0.f, 1, 0.96f, 4.f);   // slot 148 R
	Far(S1D4("HullHouse"), 54.f, 54.f, 64, 42, 1.f, 1.f, 0.f, -1, 1.f, 4.f);   // slot 150 L
	Far(S1D4("ForgeRock"), 54.f, 54.f, 64, 42, 1.f, 1.f, 0.f, 1, 1.f, 4.f);   // slot 150 R
	Far(S1D4("AwningStall"), 41.f, 41.f, 64, 42, 1.f, 1.f, 0.f, 1, 0.96f, 4.f);   // slot 150 R
	Far(S1D4("BridgeInn"), 54.f, 54.f, 64, 40, 1.f, 1.f, 0.f, -1, 1.f, 4.f);   // slot 152 L
	Far(S1D4("WinchNet"), 41.f, 41.f, 64, 40, 1.f, 1.f, 0.f, -1, 0.96f, 4.f);   // slot 152 L
	Far(S1D4("Tenement"), 54.f, 54.f, 64, 40, 1.f, 1.f, 0.f, 1, 1.f, 4.f);   // slot 152 R
	Far(S1D4("FunnelTower"), 54.f, 54.f, 64, 38, 1.f, 1.f, 0.f, -1, 1.f, 4.f);   // slot 154 L
	Far(S1D4("PropMonument"), 41.f, 41.f, 64, 38, 1.f, 1.f, 0.f, -1, 0.96f, 4.f);   // slot 154 L
	Far(S1D4("ForgeRock"), 54.f, 54.f, 64, 38, 1.f, 1.f, 0.f, 1, 1.f, 4.f);   // slot 154 R
	Far(S1D4("PortholeLamp"), 41.f, 41.f, 64, 38, 1.f, 1.f, 0.f, 1, 0.96f, 4.f);   // slot 154 R
	Far(S1D4("ForgeRock"), 64.f, 64.f, 64, 24, 1.f, 1.f, 0.f, -1, 1.02f, 4.f);   // slot 168 L
	Far(S1D4("SignalMast"), 41.5f, 41.5f, 64, 24, 1.f, 1.f, 0.f, -1, 0.96f, 4.f);   // slot 168 L
	Far(S1D4("BridgeInn"), 47.5f, 47.5f, 64, 24, 1.f, 1.f, 0.f, 1, 1.f, 4.f);   // slot 168 R
	Far(S1D4("PortholeLamp"), 41.5f, 41.5f, 64, 24, 1.f, 1.f, 0.f, 1, 0.96f, 4.f);   // slot 168 R
	Far(S1D4("Tenement"), 47.5f, 47.5f, 64, 22, 1.f, 1.f, 0.f, -1, 1.f, 4.f);   // slot 170 L
	Far(S1D4("AwningStall"), 41.5f, 41.5f, 64, 22, 1.f, 1.f, 0.f, -1, 0.96f, 4.f);   // slot 170 L
	Far(S1D4("ForgeRock"), 64.f, 64.f, 64, 22, 1.f, 1.f, 0.f, 1, 1.02f, 4.f);   // slot 170 R
	Far(S1D4("WinchNet"), 41.5f, 41.5f, 64, 22, 1.f, 1.f, 0.f, 1, 0.96f, 4.f);   // slot 170 R
	Far(S1D4("ForgeRock"), 47.5f, 47.5f, 64, 20, 1.f, 1.f, 0.f, -1, 1.f, 4.f);   // slot 172 L
	Far(S1D4("PortholeLamp"), 41.5f, 41.5f, 64, 20, 1.f, 1.f, 0.f, -1, 0.96f, 4.f);   // slot 172 L
	Far(S1D4("HullHouse"), 47.5f, 47.5f, 64, 20, 1.f, 1.f, 0.f, 1, 1.f, 4.f);   // slot 172 R
	Far(S1D4("PropMonument"), 41.5f, 41.5f, 64, 20, 1.f, 1.f, 0.f, 1, 0.96f, 4.f);   // slot 172 R
#undef S1D4
// ---- END s1d4 ----
}

{
// ---- BEGIN s1d5 (district 5 "Nereida: the Pearl Quarter", ft_world_sea_s1d5.py) ----
// 16 meshes (7 buildings, 8 props, the hero THE GREAT PEARL). Even slots only (s % 4 == 0 or 2), 7.94 to 12.2 km (slots 176 to 269: 94.1).
// Estimate (slots / every x chance x 0.75; the Site: its 2 slots): buildings right 4 x 5.88 + 2 x 2.94 + 5.88 = 35.3, left 5.88 + 6 x 2.94
//   = 23.5; low props 4 x 8.83 + 14.12 = 49.4; tall props 17.65 + 3 x 5.88 + 4 x 8.83 = 70.6; the hero 2. Sum: about 181 placed.
//   Most of one mesh if no slot is left out: PearlLamp 24 + 11 = 35 (estimate 26.5). Smallest x min of a TALL mesh: 24.2 m (PearlLamp, right).
// Bands and slots (R = right, the pipeline's side; L = left, the travel tube's side, the tube at 33 m hides the first 6.6 m behind it):
//   No building in a slot s % 6 == 0 (the kelp stands at 62 m reach in to 40 m). Never both sides of 70 m for one mesh (all 70 m and nearer; the hero 165 m only).
//   R 21.8-24 low props behind the pipeline (s % 8 == 2, 6)   R 24.2-25 PearlLamp (s % 4 == 0)   R 31.3-32.5 Seahorse (s % 12 == 2), BannerMast (s % 12 == 6)
//   R 43.5-46 ShopArcade (s % 12 == 4), ArchGate (s % 24 == 8), TownHouse (s % 24 == 20): between the watch pieces at 33-38 m and pass 3's pieces at 52-58 m
//   R 68-70 Hall (s % 12 == 2), BathHouse (s % 12 == 10): behind the watch towers at 56 m; StepTemple (s % 12 == 4), Treasury (s % 12 == 8)
//   L 22-23.6 low props behind the pressure vessels (s % 8 == 0, 4)   L 24.2-24.8 PlanterRail (s % 4 == 2)   L 25-25.4 BannerMast (s % 12 == 8)
//   L 28.2-30 in front of the tube: Fountain (s % 8 == 0), AmphoraStall (s % 8 == 4), Seahorse (s % 8 == 2), PearlLamp (s % 8 == 6)
//   L 39.5-42 behind the tube: ArchGate (s % 12 == 4), ShopArcade (s % 24 == 10), TownHouse (s % 24 == 22)
//   L 66-70 Hall (s % 24 == 4), BathHouse (s % 24 == 16): behind the watch towers at 52 m; StepTemple (s % 24 == 10), Treasury (s % 24 == 22)
//   THE GREAT PEARL: left, 165 m, slots 186 (8.42 km) and 242 (10.95 km) (fix round; was right, 198 and 246). Glow (steady) only on pieces of rules with x min 24 m or more.
#define S1D5(n) TEXT("/Game/World/UnderTheSea/SM_S1D5_" n)
Set(TEXT("WldS1D5GreatPearl"), {                    // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D5("GreatPearl"), 0.f, 0.f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1d5, Nereida: the Pearl Quarter"), 7.94f, 12.2f);
	// ---- buildings, right
	Scatter(S1D5("ShopArcade"), 43.5f, 46.f, 12, 8, 1.f, 1.f, 0.f, 1, 3.f);            // s % 12 == 4
	Scatter(S1D5("ArchGate"), 43.5f, 46.f, 24, 16, 1.f, 1.f, 0.f, 1, 3.f);             // s % 24 == 8
	Scatter(S1D5("TownHouse"), 44.f, 45.5f, 24, 4, 1.f, 1.f, 0.f, 1, 3.f);             // s % 24 == 20
	Scatter(S1D5("Hall"), 69.f, 70.f, 12, 10, 1.f, 1.15f, 0.f, 1, 3.f);                // s % 12 == 2
	Scatter(S1D5("BathHouse"), 69.f, 70.f, 12, 2, 1.f, 1.15f, 0.f, 1, 3.f);            // s % 12 == 10
	Scatter(S1D5("StepTemple"), 69.3f, 70.f, 12, 8, 1.f, 1.05f, 0.f, 1, 3.f);          // s % 12 == 4
	Scatter(S1D5("Treasury"), 68.f, 69.5f, 12, 4, 1.f, 1.2f, 0.f, 1, 20.f);            // s % 12 == 8
	// ---- buildings, left
	Scatter(S1D5("ArchGate"), 39.5f, 41.f, 12, 8, 1.f, 1.f, 0.f, -1, 3.f);             // s % 12 == 4
	Scatter(S1D5("ShopArcade"), 40.5f, 41.5f, 24, 14, 1.f, 1.f, 0.f, -1, 3.f);         // s % 24 == 10
	Scatter(S1D5("TownHouse"), 41.f, 42.f, 24, 2, 1.f, 1.1f, 0.f, -1, 3.f);            // s % 24 == 22
	Scatter(S1D5("Hall"), 66.f, 69.f, 24, 20, 1.f, 1.15f, 0.f, -1, 3.f);               // s % 24 == 4
	Scatter(S1D5("BathHouse"), 66.f, 69.f, 24, 8, 1.f, 1.15f, 0.f, -1, 3.f);           // s % 24 == 16
	Scatter(S1D5("StepTemple"), 69.3f, 70.f, 24, 14, 1.f, 1.05f, 0.f, -1, 3.f);        // s % 24 == 10
	Scatter(S1D5("Treasury"), 68.f, 69.5f, 24, 2, 1.f, 1.2f, 0.f, -1, 20.f);           // s % 24 == 22
	// ---- low props (the log says low)
	Scatter(S1D5("BrokenColumn"), 21.8f, 24.f, 8, 6, 1.f, 1.f, 0.f, 1, 25.f);          // right, s % 8 == 2
	Scatter(S1D5("BenchUrn"), 21.8f, 24.f, 8, 2, 1.f, 1.f, 0.f, 1, 4.f);               // right, s % 8 == 6
	Scatter(S1D5("BrokenColumn"), 22.f, 23.6f, 8, 0, 1.f, 1.f, 0.f, -1, 25.f);         // left, s % 8 == 0
	Scatter(S1D5("BenchUrn"), 22.f, 23.6f, 8, 4, 1.f, 1.f, 0.f, -1, 4.f);              // left, s % 8 == 4
	Scatter(S1D5("PlanterRail"), 24.2f, 24.8f, 4, 2, 0.8f, 1.f, 0.f, -1, 0.f);         // left, s % 4 == 2
	// ---- tall props (24 m and more)
	Scatter(S1D5("PearlLamp"), 24.6f, 25.4f, 4, 0, 1.f, 1.f, 0.f, 1, 0.f);             // right, s % 4 == 0 (fix round: from 24.6, so the track no longer pushes it out and its glass is 24 m out)
	Scatter(S1D5("Seahorse"), 28.5f, 29.5f, 12, 8, 1.f, 1.f, 0.f, 1, 0.f);             // right, s % 12 == 4 (fix round: at 31.3 m in s % 12 == 2 it stood in the lane of pass 3's wagons and sled trains)
	Scatter(S1D5("BannerMast"), 31.3f, 32.5f, 12, 6, 1.f, 1.f, 0.f, 1, 0.f);           // right, s % 12 == 6
	Scatter(S1D5("Fountain"), 28.2f, 28.6f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);             // left, s % 8 == 0
	Scatter(S1D5("AmphoraStall"), 28.6f, 29.4f, 8, 4, 1.f, 1.f, 0.f, -1, 4.f);         // left, s % 8 == 4
	Scatter(S1D5("Seahorse"), 28.6f, 29.6f, 8, 6, 1.f, 1.f, 0.f, -1, 0.f);             // left, s % 8 == 2
	Scatter(S1D5("PearlLamp"), 28.6f, 30.f, 8, 2, 1.f, 1.f, 0.f, -1, 0.f);             // left, s % 8 == 6
	Scatter(S1D5("BannerMast"), 25.3f, 25.7f, 12, 4, 1.f, 1.f, 0.f, -1, 0.f);         // left, s % 12 == 8 (fix round: from 25.3)
	// ---- the hero landmark: slots 186 (8.42 km) and 242 (10.95 km) only ((s + 38) % 56 == 0), LEFT. Fix round: on the right (slots 198, 246)
	// it came into view under the HUD's panel (top right), and older far pieces stood in its footprint; the top left of the frame is free.
	Site(TEXT("WldS1D5GreatPearl"), -1, 165.f, 56, 38, 1.f);
#undef S1D5
// ---- END s1d5 ----
}

{
// ---- BEGIN s1d6 (district 6 "Nereida: the Bubble Works", ft_world_sea_s1d6.py) ----
// 20 meshes: 8 buildings, 11 props (7 tall, 4 low), the hero THE BUBBLE TREE. 12.2 to 16.1 km: slots 270 to 355.
// One Zone a slot (every 4, phase 0 or 2: even slots only), each Site in a slot chosen by reading both tables of the level:
//   no slot of a glass tunnel (310-314, 331-335, 352-356); on the banked bends of the circuit (slots 301-309, 322-330,
//   343-351, worked out from FTData.cpp; which side is the inside was not checked) nothing inside 40 m, either side.
//   RIGHT: low props 23.2 m behind the pipeline; tall props 26.2 m where the 25.5-27.5 m band is free; a building and two
//   props 46.5 m (s%4==0: no watch tower there); two buildings and a prop 67.5 m. LEFT: low props 23.4 m (s%4==0); tall props
//   26.2 m in front of the tube (s%4==2); tall props 40.8 m behind it; a building and two props 46.5 m (s%4==2); two buildings
//   and a prop 68 m; the hero 155 m in slots 290 (13.12 km) and 338 (15.29 km).
// Estimate: every rule is certain (chance 1) and stands in a named slot outside the tunnels, so the count is the sum of the
// Sets' parts over 99 rules in 32 slots = 261 placed (by the brief's formula, x 0.75 for the track's share: about 196).
// The most of one mesh: MouldBench 24, PipeVent 23, SandSledge 23. Smallest x of a TALL mesh: 26.2 m (the tall props of the Near sets).
#define S1D6(n) TEXT("/Game/World/UnderTheSea/SM_S1D6_" n)
Set(TEXT("WldS1D6Verge0"), {
	{ S1D6("MouldBench"), -14.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("PipeVent"), 0.0f, 0.2f, 0.f, 0.f, 1.f },
	{ S1D6("SandSledge"), 14.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Near0"), {
	{ S1D6("BubbleLamp"), -11.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("FloatRack"), 11.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Front0"), {
	{ S1D6("StemVilla"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("ChimneyStack"), -16.5f, -3.0f, 0.f, 0.f, 1.f },
	{ S1D6("SeahorseVane"), 16.5f, -3.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Verge1"), {
	{ S1D6("GazingGlobes"), -14.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("MouldBench"), 0.0f, 0.2f, 0.f, 0.f, 1.f },
	{ S1D6("PipeVent"), 14.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Back0"), {
	{ S1D6("GrapeTenement"), -12.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("FurnaceHall"), 12.5f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D6("SignPost"), 0.0f, -6.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6VergeL0"), {
	{ S1D6("SandSledge"), -8.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("GazingGlobes"), 8.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Near1"), {
	{ S1D6("GlassStall"), -11.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("BottleTree"), 11.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Front1"), {
	{ S1D6("ThreeBubble"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("ChimneyStack"), -16.5f, -3.0f, 0.f, 0.f, 1.f },
	{ S1D6("SeahorseVane"), 16.5f, -3.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Front2"), {
	{ S1D6("BottleShop"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("SignPost"), -16.5f, -3.0f, 0.f, 0.f, 1.f },
	{ S1D6("GlassStall"), 16.5f, -3.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Near2"), {
	{ S1D6("BottleTree"), -11.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("BubbleLamp"), 11.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Front3"), {
	{ S1D6("DomeRow"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("FloatRack"), -16.5f, -3.0f, 0.f, 0.f, 1.f },
	{ S1D6("SignPost"), 16.5f, -3.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Back1"), {
	{ S1D6("BlowpipeTower"), -12.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("GrapeTenement"), 12.5f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D6("GlassStall"), 0.0f, -6.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Over0"), {
	{ S1D6("ChimneyStack"), -12.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("SeahorseVane"), 12.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Back2"), {
	{ S1D6("BlowpipeTower"), -12.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("FurnaceHall"), 12.5f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D6("ChimneyStack"), 0.0f, -6.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Front4"), {
	{ S1D6("PumpHouse"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("FloatRack"), -16.5f, -3.0f, 0.f, 0.f, 1.f },
	{ S1D6("SignPost"), 16.5f, -3.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Back3"), {
	{ S1D6("StemVilla"), -12.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("BottleShop"), 12.5f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D6("GlassStall"), 0.0f, -6.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Over1"), {
	{ S1D6("SeahorseVane"), -12.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("ChimneyStack"), 12.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Back4"), {
	{ S1D6("ThreeBubble"), -12.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("GrapeTenement"), 12.5f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D6("FloatRack"), -9.0f, -7.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Front5"), {
	{ S1D6("PumpHouse"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("SeahorseVane"), -16.5f, -3.0f, 0.f, 0.f, 1.f },
	{ S1D6("ChimneyStack"), 16.5f, -3.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Back5"), {
	{ S1D6("BlowpipeTower"), -12.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("FurnaceHall"), 12.5f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D6("SeahorseVane"), 0.0f, -6.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Near3"), {
	{ S1D6("SignPost"), -11.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("GlassStall"), 11.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Hero0"), {
	{ S1D6("BubbleTree"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Near4"), {
	{ S1D6("FloatRack"), -11.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("SignPost"), 11.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Back6"), {
	{ S1D6("BottleShop"), -12.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("ThreeBubble"), 12.5f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D6("GlassStall"), 0.0f, -6.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Back7"), {
	{ S1D6("GrapeTenement"), -12.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("FurnaceHall"), 12.5f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D6("FloatRack"), -9.0f, -7.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Back8"), {
	{ S1D6("BlowpipeTower"), -12.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("StemVilla"), 12.5f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D6("ChimneyStack"), 0.0f, -6.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Front6"), {
	{ S1D6("DomeRow"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("SignPost"), -16.5f, -3.0f, 0.f, 0.f, 1.f },
	{ S1D6("GlassStall"), 16.5f, -3.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6FrontT0"), {
	{ S1D6("ThreeBubble"), -11.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("SeahorseVane"), 9.5f, -3.0f, 0.f, 0.f, 1.f },
	{ S1D6("BubbleLamp"), 18.5f, -3.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Front7"), {
	{ S1D6("DomeRow"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("BottleTree"), -16.5f, -3.0f, 0.f, 0.f, 1.f },
	{ S1D6("FloatRack"), 16.5f, -3.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Back9"), {
	{ S1D6("BottleShop"), -12.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("GrapeTenement"), 12.5f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D6("SignPost"), 0.0f, -6.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Back10"), {
	{ S1D6("BlowpipeTower"), -12.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("FurnaceHall"), 12.5f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D6("BubbleLamp"), 0.0f, -6.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Front8"), {
	{ S1D6("PumpHouse"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("ChimneyStack"), -16.5f, -3.0f, 0.f, 0.f, 1.f },
	{ S1D6("SeahorseVane"), 16.5f, -3.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Near5"), {
	{ S1D6("BottleTree"), -11.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("FloatRack"), 11.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Back11"), {
	{ S1D6("ThreeBubble"), -12.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("GrapeTenement"), 12.5f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D6("BottleTree"), 0.0f, -6.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6VergeL1"), {
	{ S1D6("GazingGlobes"), -8.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("MouldBench"), 8.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Over2"), {
	{ S1D6("BubbleLamp"), -12.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("ChimneyStack"), 12.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Verge2"), {
	{ S1D6("PipeVent"), -14.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("SandSledge"), 0.0f, 0.2f, 0.f, 0.f, 1.f },
	{ S1D6("GazingGlobes"), 14.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Front9"), {
	{ S1D6("PumpHouse"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("GlassStall"), -16.5f, -3.0f, 0.f, 0.f, 1.f },
	{ S1D6("BottleTree"), 16.5f, -3.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Back12"), {
	{ S1D6("StemVilla"), -12.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("BottleShop"), 12.5f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D6("BubbleLamp"), 0.0f, -6.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6FrontT1"), {
	{ S1D6("StemVilla"), -11.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("ChimneyStack"), 9.5f, -3.0f, 0.f, 0.f, 1.f },
	{ S1D6("SeahorseVane"), 18.5f, -3.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Front10"), {
	{ S1D6("PumpHouse"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("BubbleLamp"), -16.5f, -3.0f, 0.f, 0.f, 1.f },
	{ S1D6("ChimneyStack"), 16.5f, -3.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6VergeL2"), {
	{ S1D6("MouldBench"), -8.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("PipeVent"), 8.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Verge3"), {
	{ S1D6("SandSledge"), -14.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("GazingGlobes"), 0.0f, 0.2f, 0.f, 0.f, 1.f },
	{ S1D6("MouldBench"), 14.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Back13"), {
	{ S1D6("BottleShop"), -12.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("ThreeBubble"), 12.5f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D6("BottleTree"), -9.0f, -7.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Back14"), {
	{ S1D6("GrapeTenement"), -12.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("FurnaceHall"), 12.5f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D6("GlassStall"), 0.0f, -6.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Over3"), {
	{ S1D6("SeahorseVane"), -12.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("BubbleLamp"), 12.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Front11"), {
	{ S1D6("PumpHouse"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D6("SeahorseVane"), -16.5f, -3.0f, 0.f, 0.f, 1.f },
	{ S1D6("BubbleLamp"), 16.5f, -3.0f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1d6, the Bubble Works, slot 270"), 12.20000f, 12.23597f);
	Site(TEXT("WldS1D6Verge0"), 1, 23.2f, 4, 2, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Near0"), 1, 26.2f, 4, 2, 1.f);   // right, 26.2 m: the 25.5-27.5 m band is free in this slot (stands s%12==0; patrol, weld, dive team s%18==2,8,14)
	Site(TEXT("WldS1D6Front0"), -1, 46.5f, 4, 2, 1.f);   // left, 46.5 m, behind the tube: no watch tower (s%4==0), no pressure habitat (odd slots)
Zone(TEXT("World s1d6, the Bubble Works, slot 272"), 12.28574f, 12.32646f);
	Site(TEXT("WldS1D6Verge1"), 1, 23.2f, 4, 0, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Back0"), 1, 67.5f, 4, 0, 1.f);   // right, 67.5 m: no kelp stand (s%3==0), no habitat (odd slots), no salvage (s%18==4)
	Site(TEXT("WldS1D6VergeL0"), -1, 23.4f, 4, 0, 1.f);   // left verge, low, behind the low pods (18.3-21 m); the verge gangs have s%4==2
Zone(TEXT("World s1d6, the Bubble Works, slot 274"), 12.37622f, 12.41694f);
	Site(TEXT("WldS1D6Verge0"), 1, 23.2f, 4, 2, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Near1"), 1, 26.2f, 4, 2, 1.f);   // right, 26.2 m: the 25.5-27.5 m band is free in this slot (stands s%12==0; patrol, weld, dive team s%18==2,8,14)
	Site(TEXT("WldS1D6Near0"), -1, 26.2f, 4, 2, 1.f);   // left, 26.2 m, in front of the tube: no stand, patrol, dive team or film unit in this slot
	Site(TEXT("WldS1D6Front1"), -1, 46.5f, 4, 2, 1.f);   // left, 46.5 m, behind the tube: no watch tower (s%4==0), no pressure habitat (odd slots)
Zone(TEXT("World s1d6, the Bubble Works, slot 276"), 12.46671f, 12.50743f);
	Site(TEXT("WldS1D6Verge1"), 1, 23.2f, 4, 0, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Front2"), 1, 46.5f, 4, 0, 1.f);   // right, 46.5 m: behind the watch balconies (33-38 m); no watch tower on the right in s%4==0
	Site(TEXT("WldS1D6VergeL0"), -1, 23.4f, 4, 0, 1.f);   // left verge, low, behind the low pods (18.3-21 m); the verge gangs have s%4==2
Zone(TEXT("World s1d6, the Bubble Works, slot 278"), 12.55719f, 12.59791f);
	Site(TEXT("WldS1D6Verge0"), 1, 23.2f, 4, 2, 1.f);   // right verge, low, behind the pipeline (19.5 m)
Zone(TEXT("World s1d6, the Bubble Works, slot 280"), 12.64768f, 12.68840f);
	Site(TEXT("WldS1D6Verge1"), 1, 23.2f, 4, 0, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Near2"), 1, 26.2f, 4, 0, 1.f);   // right, 26.2 m: the 25.5-27.5 m band is free in this slot (stands s%12==0; patrol, weld, dive team s%18==2,8,14)
	Site(TEXT("WldS1D6Front3"), 1, 46.5f, 4, 0, 1.f);   // right, 46.5 m: behind the watch balconies (33-38 m); no watch tower on the right in s%4==0
	Site(TEXT("WldS1D6Back1"), 1, 67.5f, 4, 0, 1.f);   // right, 67.5 m: no kelp stand (s%3==0), no habitat (odd slots), no salvage (s%18==4)
	Site(TEXT("WldS1D6VergeL0"), -1, 23.4f, 4, 0, 1.f);   // left verge, low, behind the low pods (18.3-21 m); the verge gangs have s%4==2
	Site(TEXT("WldS1D6Over0"), -1, 40.8f, 4, 0, 1.f);   // left, 40.8 m, behind the tube, tall enough to show over it; no swimmers at 42-43.5 m in this slot
	Site(TEXT("WldS1D6Back2"), -1, 68.0f, 4, 0, 1.f);   // left, 68 m: no kelp stand (s%3==0), no habitat (s%6==2), no salvage (s%18==4)
Zone(TEXT("World s1d6, the Bubble Works, slot 282"), 12.73817f, 12.77889f);
	Site(TEXT("WldS1D6Verge0"), 1, 23.2f, 4, 2, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Near2"), 1, 26.2f, 4, 2, 1.f);   // right, 26.2 m: the 25.5-27.5 m band is free in this slot (stands s%12==0; patrol, weld, dive team s%18==2,8,14)
Zone(TEXT("World s1d6, the Bubble Works, slot 284"), 12.82865f, 12.86937f);
	Site(TEXT("WldS1D6Verge1"), 1, 23.2f, 4, 0, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Front4"), 1, 46.5f, 4, 0, 1.f);   // right, 46.5 m: behind the watch balconies (33-38 m); no watch tower on the right in s%4==0
	Site(TEXT("WldS1D6Back3"), 1, 67.5f, 4, 0, 1.f);   // right, 67.5 m: no kelp stand (s%3==0), no habitat (odd slots), no salvage (s%18==4)
	Site(TEXT("WldS1D6VergeL0"), -1, 23.4f, 4, 0, 1.f);   // left verge, low, behind the low pods (18.3-21 m); the verge gangs have s%4==2
	Site(TEXT("WldS1D6Over1"), -1, 40.8f, 4, 0, 1.f);   // left, 40.8 m, behind the tube, tall enough to show over it; no swimmers at 42-43.5 m in this slot
Zone(TEXT("World s1d6, the Bubble Works, slot 286"), 12.91914f, 12.95986f);
	Site(TEXT("WldS1D6Verge0"), 1, 23.2f, 4, 2, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Near2"), 1, 26.2f, 4, 2, 1.f);   // right, 26.2 m: the 25.5-27.5 m band is free in this slot (stands s%12==0; patrol, weld, dive team s%18==2,8,14)
	Site(TEXT("WldS1D6Back4"), 1, 67.5f, 4, 2, 1.f);   // right, 67.5 m: no kelp stand (s%3==0), no habitat (odd slots), no salvage (s%18==4)
	Site(TEXT("WldS1D6Front5"), -1, 46.5f, 4, 2, 1.f);   // left, 46.5 m, behind the tube: no watch tower (s%4==0), no pressure habitat (odd slots)
	Site(TEXT("WldS1D6Back5"), -1, 68.0f, 4, 2, 1.f);   // left, 68 m: no kelp stand (s%3==0), no habitat (s%6==2), no salvage (s%18==4)
Zone(TEXT("World s1d6, the Bubble Works, slot 288"), 13.00962f, 13.05034f);
	Site(TEXT("WldS1D6Verge1"), 1, 23.2f, 4, 0, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6VergeL0"), -1, 23.4f, 4, 0, 1.f);   // left verge, low, behind the low pods (18.3-21 m); the verge gangs have s%4==2
Zone(TEXT("World s1d6, the Bubble Works, slot 290"), 13.10011f, 13.14083f);
	Site(TEXT("WldS1D6Verge0"), 1, 23.2f, 4, 2, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Near3"), -1, 26.2f, 4, 2, 1.f);   // left, 26.2 m, in front of the tube: no stand, patrol, dive team or film unit in this slot
Zone(TEXT("World s1d6, the Bubble Tree at 12.4 km"), 273.5f * 0.045243f, 274.4f * 0.045243f);   // fix round: slot 274. At slot 290 (13.12 km) it stood where the finale ends (13.13 to 13.17 km in every run) and was never on screen
	Site(TEXT("WldS1D6Hero0"), -1, 155.0f, 4, 2, 1.f);   // left, THE BUBBLE TREE, 155 m: nothing of the older tables inside its footprint in slot 274 by the replay
Zone(TEXT("World s1d6, the Bubble Works, slot 292"), 13.19060f, 13.23132f);
	Site(TEXT("WldS1D6Verge1"), 1, 23.2f, 4, 0, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Near2"), 1, 26.2f, 4, 0, 1.f);   // right, 26.2 m: the 25.5-27.5 m band is free in this slot (stands s%12==0; patrol, weld, dive team s%18==2,8,14)
	Site(TEXT("WldS1D6VergeL0"), -1, 23.4f, 4, 0, 1.f);   // left verge, low, behind the low pods (18.3-21 m); the verge gangs have s%4==2
Zone(TEXT("World s1d6, the Bubble Works, slot 294"), 13.28108f, 13.32180f);
	Site(TEXT("WldS1D6Verge0"), 1, 23.2f, 4, 2, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Near4"), 1, 26.2f, 4, 2, 1.f);   // right, 26.2 m: the 25.5-27.5 m band is free in this slot (stands s%12==0; patrol, weld, dive team s%18==2,8,14)
	Site(TEXT("WldS1D6Front0"), -1, 46.5f, 4, 2, 1.f);   // left, 46.5 m, behind the tube: no watch tower (s%4==0), no pressure habitat (odd slots)
Zone(TEXT("World s1d6, the Bubble Works, slot 296"), 13.37157f, 13.41229f);
	Site(TEXT("WldS1D6Verge1"), 1, 23.2f, 4, 0, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Back6"), 1, 67.5f, 4, 0, 1.f);   // right, 67.5 m: no kelp stand (s%3==0), no habitat (odd slots), no salvage (s%18==4)
	Site(TEXT("WldS1D6VergeL0"), -1, 23.4f, 4, 0, 1.f);   // left verge, low, behind the low pods (18.3-21 m); the verge gangs have s%4==2
Zone(TEXT("World s1d6, the Bubble Works, slot 298"), 13.46205f, 13.50277f);
	Site(TEXT("WldS1D6Verge0"), 1, 23.2f, 4, 2, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Near2"), 1, 26.2f, 4, 2, 1.f);   // right, 26.2 m: the 25.5-27.5 m band is free in this slot (stands s%12==0; patrol, weld, dive team s%18==2,8,14)
	Site(TEXT("WldS1D6Back7"), 1, 67.5f, 4, 2, 1.f);   // right, 67.5 m: no kelp stand (s%3==0), no habitat (odd slots), no salvage (s%18==4)
	Site(TEXT("WldS1D6Back8"), -1, 68.0f, 4, 2, 1.f);   // left, 68 m: no kelp stand (s%3==0), no habitat (s%6==2), no salvage (s%18==4)
Zone(TEXT("World s1d6, the Bubble Works, slot 300"), 13.55254f, 13.59326f);
	Site(TEXT("WldS1D6Verge1"), 1, 23.2f, 4, 0, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Front6"), 1, 46.5f, 4, 0, 1.f);   // right, 46.5 m: behind the watch balconies (33-38 m); no watch tower on the right in s%4==0
	Site(TEXT("WldS1D6VergeL0"), -1, 23.4f, 4, 0, 1.f);   // left verge, low, behind the low pods (18.3-21 m); the verge gangs have s%4==2
Zone(TEXT("World s1d6, the Bubble Works, slot 302"), 13.64303f, 13.68375f);   // a banked bend: nothing inside 40 m
	Site(TEXT("WldS1D6FrontT0"), -1, 46.5f, 4, 2, 1.f);   // left, 46.5 m, behind the tube, beside the cable trench of this slot (s%6==2) and clear of the habitat's marker post
Zone(TEXT("World s1d6, the Bubble Works, slot 304"), 13.73351f, 13.77423f);   // a banked bend: nothing inside 40 m
	Site(TEXT("WldS1D6Front7"), 1, 46.5f, 4, 0, 1.f);   // right, 46.5 m: behind the watch balconies (33-38 m); no watch tower on the right in s%4==0
	Site(TEXT("WldS1D6Back9"), 1, 67.5f, 4, 0, 1.f);   // right, 67.5 m: no kelp stand (s%3==0), no habitat (odd slots), no salvage (s%18==4)
	Site(TEXT("WldS1D6Over0"), -1, 40.8f, 4, 0, 1.f);   // left, 40.8 m, behind the tube, tall enough to show over it; no swimmers at 42-43.5 m in this slot
	Site(TEXT("WldS1D6Back10"), -1, 68.0f, 4, 0, 1.f);   // left, 68 m: no kelp stand (s%3==0), no habitat (s%6==2), no salvage (s%18==4)
Zone(TEXT("World s1d6, the Bubble Works, slot 306"), 13.82400f, 13.86472f);   // a banked bend: nothing inside 40 m
	Site(TEXT("WldS1D6Front8"), -1, 46.5f, 4, 2, 1.f);   // left, 46.5 m, behind the tube: no watch tower (s%4==0), no pressure habitat (odd slots)
Zone(TEXT("World s1d6, the Bubble Works, slot 308"), 13.91448f, 13.95520f);   // a banked bend: nothing inside 40 m
	Site(TEXT("WldS1D6Back3"), 1, 67.5f, 4, 0, 1.f);   // right, 67.5 m: no kelp stand (s%3==0), no habitat (odd slots), no salvage (s%18==4)
Zone(TEXT("World s1d6, the Bubble Works, slot 316"), 14.27643f, 14.31715f);
	Site(TEXT("WldS1D6Verge0"), 1, 23.2f, 4, 0, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Near5"), 1, 26.2f, 4, 0, 1.f);   // right, 26.2 m: the 25.5-27.5 m band is free in this slot (stands s%12==0; patrol, weld, dive team s%18==2,8,14)
	Site(TEXT("WldS1D6Front6"), 1, 46.5f, 4, 0, 1.f);   // right, 46.5 m: behind the watch balconies (33-38 m); no watch tower on the right in s%4==0
	Site(TEXT("WldS1D6Back11"), 1, 67.5f, 4, 0, 1.f);   // right, 67.5 m: no kelp stand (s%3==0), no habitat (odd slots), no salvage (s%18==4)
	Site(TEXT("WldS1D6VergeL1"), -1, 23.4f, 4, 0, 1.f);   // left verge, low, behind the low pods (18.3-21 m); the verge gangs have s%4==2
	Site(TEXT("WldS1D6Over2"), -1, 40.8f, 4, 0, 1.f);   // left, 40.8 m, behind the tube, tall enough to show over it; no swimmers at 42-43.5 m in this slot
	Site(TEXT("WldS1D6Back5"), -1, 68.0f, 4, 0, 1.f);   // left, 68 m: no kelp stand (s%3==0), no habitat (s%6==2), no salvage (s%18==4)
Zone(TEXT("World s1d6, the Bubble Works, slot 318"), 14.36691f, 14.40763f);
	Site(TEXT("WldS1D6Verge2"), 1, 23.2f, 4, 2, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Near4"), 1, 26.2f, 4, 2, 1.f);   // right, 26.2 m: the 25.5-27.5 m band is free in this slot (stands s%12==0; patrol, weld, dive team s%18==2,8,14)
Zone(TEXT("World s1d6, the Bubble Works, slot 320"), 14.45740f, 14.49812f);
	Site(TEXT("WldS1D6Verge0"), 1, 23.2f, 4, 0, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Front9"), 1, 46.5f, 4, 0, 1.f);   // right, 46.5 m: behind the watch balconies (33-38 m); no watch tower on the right in s%4==0
	Site(TEXT("WldS1D6Back12"), 1, 67.5f, 4, 0, 1.f);   // right, 67.5 m: no kelp stand (s%3==0), no habitat (odd slots), no salvage (s%18==4)
	Site(TEXT("WldS1D6VergeL1"), -1, 23.4f, 4, 0, 1.f);   // left verge, low, behind the low pods (18.3-21 m); the verge gangs have s%4==2
Zone(TEXT("World s1d6, the Bubble Works, slot 322"), 14.54789f, 14.58861f);   // a banked bend: nothing inside 40 m
	Site(TEXT("WldS1D6Back4"), 1, 67.5f, 4, 2, 1.f);   // right, 67.5 m: no kelp stand (s%3==0), no habitat (odd slots), no salvage (s%18==4)
	Site(TEXT("WldS1D6Front8"), -1, 46.5f, 4, 2, 1.f);   // left, 46.5 m, behind the tube: no watch tower (s%4==0), no pressure habitat (odd slots)
	Site(TEXT("WldS1D6Back10"), -1, 68.0f, 4, 2, 1.f);   // left, 68 m: no kelp stand (s%3==0), no habitat (s%6==2), no salvage (s%18==4)
Zone(TEXT("World s1d6, the Bubble Works, slot 326"), 14.72886f, 14.76958f);   // a banked bend: nothing inside 40 m
	Site(TEXT("WldS1D6FrontT1"), -1, 46.5f, 4, 2, 1.f);   // left, 46.5 m, behind the tube, beside the cable trench of this slot (s%6==2) and clear of the habitat's marker post
Zone(TEXT("World s1d6, the Bubble Works, slot 330"), 14.90983f, 14.95055f);   // a banked bend: nothing inside 40 m
	Site(TEXT("WldS1D6Front10"), -1, 46.5f, 4, 2, 1.f);   // left, 46.5 m, behind the tube: no watch tower (s%4==0), no pressure habitat (odd slots)
Zone(TEXT("World s1d6, the Bubble Works, slot 336"), 15.18129f, 15.22201f);
	Site(TEXT("WldS1D6Verge2"), 1, 23.2f, 4, 0, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Front6"), 1, 46.5f, 4, 0, 1.f);   // right, 46.5 m: behind the watch balconies (33-38 m); no watch tower on the right in s%4==0
	Site(TEXT("WldS1D6VergeL2"), -1, 23.4f, 4, 0, 1.f);   // left verge, low, behind the low pods (18.3-21 m); the verge gangs have s%4==2
Zone(TEXT("World s1d6, the Bubble Works, slot 338"), 15.27177f, 15.31249f);
	Site(TEXT("WldS1D6Verge3"), 1, 23.2f, 4, 2, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Back13"), 1, 67.5f, 4, 2, 1.f);   // right, 67.5 m: no kelp stand (s%3==0), no habitat (odd slots), no salvage (s%18==4)
	Site(TEXT("WldS1D6Near4"), -1, 26.2f, 4, 2, 1.f);   // left, 26.2 m, in front of the tube: no stand, patrol, dive team or film unit in this slot
	Site(TEXT("WldS1D6Hero0"), -1, 155.0f, 4, 2, 1.f);   // left, THE BUBBLE TREE, 155 m: no far gallery (s%14==8), far watch tower (s%3==1), convoy, giants or shark ring in this slot
Zone(TEXT("World s1d6, the Bubble Works, slot 340"), 15.36226f, 15.40298f);
	Site(TEXT("WldS1D6Verge2"), 1, 23.2f, 4, 0, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Near1"), 1, 26.2f, 4, 0, 1.f);   // right, 26.2 m: the 25.5-27.5 m band is free in this slot (stands s%12==0; patrol, weld, dive team s%18==2,8,14)
	Site(TEXT("WldS1D6Front3"), 1, 46.5f, 4, 0, 1.f);   // right, 46.5 m: behind the watch balconies (33-38 m); no watch tower on the right in s%4==0
	Site(TEXT("WldS1D6Back14"), 1, 67.5f, 4, 0, 1.f);   // right, 67.5 m: no kelp stand (s%3==0), no habitat (odd slots), no salvage (s%18==4)
	Site(TEXT("WldS1D6VergeL2"), -1, 23.4f, 4, 0, 1.f);   // left verge, low, behind the low pods (18.3-21 m); the verge gangs have s%4==2
	Site(TEXT("WldS1D6Over3"), -1, 40.8f, 4, 0, 1.f);   // left, 40.8 m, behind the tube, tall enough to show over it; no swimmers at 42-43.5 m in this slot
	Site(TEXT("WldS1D6Back8"), -1, 68.0f, 4, 0, 1.f);   // left, 68 m: no kelp stand (s%3==0), no habitat (s%6==2), no salvage (s%18==4)
Zone(TEXT("World s1d6, the Bubble Works, slot 342"), 15.45275f, 15.49347f);
	Site(TEXT("WldS1D6Verge3"), 1, 23.2f, 4, 2, 1.f);   // right verge, low, behind the pipeline (19.5 m)
	Site(TEXT("WldS1D6Near5"), 1, 26.2f, 4, 2, 1.f);   // right, 26.2 m: the 25.5-27.5 m band is free in this slot (stands s%12==0; patrol, weld, dive team s%18==2,8,14)
	Site(TEXT("WldS1D6Front11"), -1, 46.5f, 4, 2, 1.f);   // left, 46.5 m, behind the tube: no watch tower (s%4==0), no pressure habitat (odd slots)
Zone(TEXT("World s1d6, the Bubble Works, slot 344"), 15.54323f, 15.58395f);   // a banked bend: nothing inside 40 m
	Site(TEXT("WldS1D6Front6"), 1, 46.5f, 4, 0, 1.f);   // right, 46.5 m: behind the watch balconies (33-38 m); no watch tower on the right in s%4==0
	Site(TEXT("WldS1D6Back13"), 1, 67.5f, 4, 0, 1.f);   // right, 67.5 m: no kelp stand (s%3==0), no habitat (odd slots), no salvage (s%18==4)
Zone(TEXT("World s1d6, the Bubble Works, slot 350"), 15.81469f, 15.85541f);   // a banked bend: nothing inside 40 m
	Site(TEXT("WldS1D6FrontT1"), -1, 46.5f, 4, 2, 1.f);   // left, 46.5 m, behind the tube, beside the cable trench of this slot (s%6==2) and clear of the habitat's marker post
#undef S1D6
// ---- END s1d6 ----
}

{
// ---- BEGIN s1d7 (district 7 "Nereida: Lamp Row", ft_world_sea_s1d7.py) ----
// 17 meshes (8 buildings, 8 props, 1 hero), all between 16.1 and 20.6 km (slots 356 to 455), only even slots.
// The district's land slots, counted from the table's own tunnel slots (356, 373-377, 394-398, 414-419, 435-440 are
// glass tunnel): 38 even land slots, 19 with s % 4 == 0 and 19 with s % 4 == 2.
// The circuit's banked S-bend (bank .45) takes about slots 363-372, 384-393, 405-413, 426-434, 447-455 (worked out
// from FTData's circuit sections against the tunnel slots; not seen in the game). Beside a banked bend nothing may
// stand 17 to 40 m out, so: every BUILDING rule stands 40 m or more out (nearest part of any Set: 40.2 m) and runs the
// whole district; every NEAR rule (17 to 26 m) is written only for the five straights (slots 357-362, 378-383, 399-404,
// 420-425, 441-446: 15 even slots, 7 with s % 4 == 0 and 8 with s % 4 == 2).
// Estimate, by slots counted (not by the 0.75 rule: the tunnels are already taken out):
//   buildings, right: KeepersYard 9 slots x 3 + ChartYard 10 x 2 + BuoyYard 6 x 3 + BellYard 6 x 3 + PilotRow 7 x 2 = 97
//   buildings, left:  TwinRow 9 x 2 + TallPair 10 x 2 + BellRow 10 x 2 + PilotYard 9 x 3 = 85
//   near, straights:  QuayR 7 x 2 + LampWalkR 8 x 2 + LampWalkL 7 x 2 + QuayL 8 x 2 = 60
//   hero: 2                                                              sum = about 244 placed, less what the track leaves out
// Most of one mesh: LampTower_B 38 (LampTower_A 35; then 17 or fewer). Hero: 2 (slots 386 and 428, right, 185 m).
// Bands and slots used. RIGHT: 22.8 m low (s % 4 == 0, behind the pipeline at 19.5); 24.8 m tall and slim (s % 4 == 2);
// 46 m (s % 4 == 0, behind the watch pieces at 33-38); 47 m (s % 12 == 2 and 6, before the watch tower at 56);
// 66 m (s % 12 == 10, where the watch wall has 42-47). LEFT: 20.4 m low (s % 4 == 2, before the verge gangs at 21.6);
// 24.8 m tall and slim (s % 4 == 0, behind the pressure vessels at 18.3-21); 46 m (s % 4 == 2, behind the tube: only
// pieces 13 m and taller stand there); 66 m (s % 4 == 0, behind the watch tower at 52: towers only).
// Smallest x min of a TALL mesh: 24.8 m. Glow (steady, lamp cream only): nearest 24.8 m (BracketLamp).
#define S1D7(n) TEXT("/Game/World/UnderTheSea/SM_S1D7_" n)
// fix round: in BuoyYard, BellYard, BellRow, PilotYard and TwinRow the parts were moved ALONG so that none stands on the kit's marker posts
// (47 m out, 7 to 11 m either side of the slot's middle) or in its habitat dome (64 m, 7.7 m either side).
Set(TEXT("WldS1D7KeepersYard"), {                  // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D7("LampTower_A"),     0.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D7("KeeperCottage"), -13.f,  -2.f, 0.f,   0.f, 1.f },
	{ S1D7("BracketLamp"),     9.f,  -5.f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D7ChartYard"), {
	{ S1D7("ChartHouse"),     -7.f,  0.5f, 0.f,   0.f, 1.f },
	{ S1D7("LampTower_B"),     9.f,   2.f, 0.f, -20.f, 1.f },
});
Set(TEXT("WldS1D7BuoyYard"), {
	{ S1D7("BuoyShed"),        0.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D7("BuoySide"),       13.f,  -5.f, 0.f,  25.f, 1.f },
	{ S1D7("LampTower_A"), -18.5f,   2.f, 0.f,  30.f, 1.f },
});
Set(TEXT("WldS1D7BellYard"), {
	{ S1D7("BellTower"),       0.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D7("GalleryWalk"),   -17.f,  -3.f, 0.f,   0.f, 1.f },
	{ S1D7("KeeperCottage"),  17.f,   0.f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D7PilotRow"), {
	{ S1D7("TwinTowers"),      0.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D7("PilotHouse"),    -14.f,  -3.f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D7TwinRow"), {
	{ S1D7("TwinTowers"),     15.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D7("LampTower_B"),   -15.f,   1.f, 0.f,  15.f, 1.f },
});
Set(TEXT("WldS1D7TallPair"), {
	{ S1D7("LampTower_A"),    -8.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D7("LampTower_B"),     8.f,   2.f, 0.f, -25.f, 1.f },
});
Set(TEXT("WldS1D7BellRow"), {
	{ S1D7("BellTower"),       0.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D7("LampTower_A"),   17.5f,   2.f, 0.f, -30.f, 1.f },
});
Set(TEXT("WldS1D7PilotYard"), {
	{ S1D7("PilotHouse"),      1.f,   1.f, 0.f,   0.f, 1.f },
	{ S1D7("LampTower_B"),   -10.f,   2.f, 0.f,   0.f, 1.f },
	{ S1D7("SignalMast"),     17.f,  -4.f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D7QuayR"), {                        // low
	{ S1D7("BollardRow"),     -7.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D7("OilJarRack"),      5.f,  0.4f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D7QuayL"), {                        // low
	{ S1D7("LadderBench"),    -6.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D7("BollardRow"),      6.f,   0.f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D7LampWalkR"), {                    // tall, slim (0.6 m and 1.3 m deep)
	{ S1D7("BracketLamp"),    -8.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D7("Spyglass"),        5.f,  0.3f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D7LampWalkL"), {                    // tall, slim. Fix round: the buoy on its side (a banded drum with a cone, lying along the road 25.4 m out: it could be taken for a torpedo) is a spyglass here; the buoy stays only in the buoy yard, 42 m out
	{ S1D7("SignalMast"),     -6.f,   0.f, 0.f,   0.f, 1.f },
	{ S1D7("Spyglass"),        7.f,  0.6f, 0.f,   0.f, 1.f },
});
Set(TEXT("WldS1D7GreatLamp"), { { S1D7("GreatLamp"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Zone(TEXT("World s1d7, Lamp Row"), 16.1f, 20.6f);                // buildings: every anchor 46 m or more out, the whole district
	Site(TEXT("WldS1D7KeepersYard"), 1, 46.f, 8, 0, 1.f);           // right, s % 8 == 0
	Site(TEXT("WldS1D7ChartYard"), 1, 46.f, 8, 4, 1.f);             // right, s % 8 == 4
	Site(TEXT("WldS1D7BuoyYard"), 1, 47.f, 12, 10, 1.f);            // right, s % 12 == 2
	Site(TEXT("WldS1D7BellYard"), 1, 47.f, 12, 6, 1.f);             // right, s % 12 == 6
	Site(TEXT("WldS1D7PilotRow"), 1, 66.f, 12, 2, 1.f);             // right, s % 12 == 10
	Site(TEXT("WldS1D7TwinRow"), -1, 66.f, 8, 0, 1.f);              // left, s % 8 == 0
	Site(TEXT("WldS1D7TallPair"), -1, 66.f, 8, 4, 1.f);             // left, s % 8 == 4
	Site(TEXT("WldS1D7BellRow"), -1, 46.f, 8, 6, 1.f);              // left, s % 8 == 2
	Site(TEXT("WldS1D7PilotYard"), -1, 46.f, 8, 2, 1.f);            // left, s % 8 == 6
auto S1D7Straight = [&](float A, float B)                         // the near rules: only where the road is not banked
{
	Zone(TEXT("World s1d7, Lamp Row, a straight"), (A - 0.5f) * 0.045243f, (B + 0.5f) * 0.045243f);
	Site(TEXT("WldS1D7QuayR"), 1, 22.8f, 4, 0, 1.f);                // right, s % 4 == 0, low
	Site(TEXT("WldS1D7LampWalkR"), 1, 24.8f, 4, 2, 1.f);            // right, s % 4 == 2, tall
	Site(TEXT("WldS1D7LampWalkL"), -1, 24.8f, 4, 0, 1.f);           // left, s % 4 == 0, tall
	Site(TEXT("WldS1D7QuayL"), -1, 20.4f, 4, 2, 1.f);               // left, s % 4 == 2, low
};
S1D7Straight(357.f, 362.f); S1D7Straight(378.f, 383.f); S1D7Straight(399.f, 404.f); S1D7Straight(420.f, 425.f); S1D7Straight(441.f, 446.f);
Zone(TEXT("World s1d7, the Great Lamp at 17.5 km"), 385.5f * 0.045243f, 386.4f * 0.045243f);   // slot 386 (17.46 km)
	Site(TEXT("WldS1D7GreatLamp"), 1, 185.f, 1, 0, 1.f);
Zone(TEXT("World s1d7, the Great Lamp at 19.4 km"), 427.5f * 0.045243f, 428.4f * 0.045243f);   // slot 428 (19.36 km). Fix round: at slot 432 a station hub and a dive line of the older tables stood inside it
	Site(TEXT("WldS1D7GreatLamp"), 1, 185.f, 1, 0, 1.f);
#undef S1D7
// ---- END s1d7 ----
}

{
// ---- BEGIN s1d8 (district 8 "Nereida: the Crown", ft_world_sea_s1d8.py) ----
// 15 meshes: 7 buildings, 7 props, the hero (THE CROWN). The palace gardens, 20.6 km to the end: slots 456 to 550
// (95 slots; 28 of them glass tunnel: 456-461, 477-482, 498-503, 519-523, 540-544). Every rule has chance 1.
// Estimate (slots / every x 0.75, x parts of the Set): 95/8 x 0.75 = 8.9; 95/12 = 5.9; 95/16 = 4.45; 95/24 = 3.0.
//   buildings  RowA 8.9 x 2 + RowB 4.45 x 2 + RowC left 4.45 x 2 + RowC right 8.9 x 2 + RowD 8.9 x 2 = 71.2
//              TerraceBlock 5.9 + 5.9, ArchBridge 5.9 + 5.9, MusicRotunda 5.9 = 29.5
//   props      HedgeGate 8.9 + 8.9, Parterre 4.45 + 8.9, PennantMast 8.9 + 8.9, SeahorseTopiary 4.45 + 3.0 + 4.45,
//              BubbleFountain 4.45 + 3.0 + 4.45, Walk (LampTree + 2 Urn) 5.9 x 3 + 8.9 x 3 = 117.2
//   hero       95/60 x 0.75 = 1.2 by the formula; the two slots it takes are 470 (21.26 km) and 530 (23.98 km)
//   = about 219 placed. Most of one mesh: Urn 29.6. Smallest x min of a TALL mesh: 24.6 (SeahorseTopiary).
// Bands and slots used (s % 4 == 0 or 2 only):
//   LEFT   22.6-24.4 HedgeGate (s%8==0); 24.5-25.2 Parterre (s%16==4); 24.8-25.8 BubbleFountain (s%16==12);
//          26.5-27.8 PennantMast (s%8==0); 28.4-29.2 SeahorseTopiary (s%16==4): the pressure vessels have the verge
//          to 22 m in these slots and the travel tube stands from 30.8 m;
//          25.2 Walk (s%12==2), 25.6-26.2 BubbleFountain (s%24==10), 24.6-25.6 SeahorseTopiary (s%24==22): the slots
//          s%12==6 are left to the stand of watchers at 25.5-27 m;
//          43 the palace rows behind the tube (s%4==2: RowA s%8==2, RowB s%16==6, RowC s%16==14);
//          66-68 TerraceBlock (s%12==4), 66-70 ArchBridge (s%12==10).
//   RIGHT  21.8-23.4 HedgeGate (s%8==6); 23.2-24.2 Parterre (s%8==2); 24.8 Walk (s%8==4); 25.0-26.0 BubbleFountain
//          (s%16==0); 24.6-25.4 SeahorseTopiary (s%16==8); 26.6-28.0 PennantMast (s%8==6): behind the pipeline (18.5-20.5 m);
//          45.5 the palace rows (s%4==0: RowC s%8==0, RowD s%8==4), behind the watch balconies at 33-38 m;
//          64-67 TerraceBlock (s%12==0), 66-70 ArchBridge (s%12==2), 67-70 MusicRotunda (s%12==6): behind the watch towers at 56 m.
//   HERO   185 m, left, s%54==42 (slots 474 and 528; fix round).
// FIX ROUND: the walks by the road (17 to 30 m) ran the whole district, also beside the circuit's banked bends (slots 468-475, 489-496, 510-517,
// 531-538), where 17 to 40 m floats over the cutting. They are written for the straights only now, as s1d7 does: the first straight (slots 462-467)
// here, with the old rules' places in the table so that nothing after them re-rolls; the other four straights in the s1fix block at the end of the file.
// BubbleFountain from 26.5 m, LampTree and SeahorseTopiary from 25 m: the track pushed them out before (bounds over the road's 24 m).
#define S1D8(n) TEXT("/Game/World/UnderTheSea/SM_S1D8_" n)
Set(TEXT("WldS1D8Crown"), {                        // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D8("Crown"),              0.f,  0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8Walk"), {                         // a gold lamp tree between two urns
	{ S1D8("LampTree"),           0.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D8("Urn"),               -8.f, -0.4f, 0.f, 0.f, 1.f },
	{ S1D8("Urn"),                8.f, -0.4f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8RowA"), {                         // the onion-dome pavilion and a crystal spire house
	{ S1D8("OnionPavilion"),     -9.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D8("CrystalSpireHouse"),  2.f,  3.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8RowB"), {                         // the music rotunda and the gate lodge
	{ S1D8("MusicRotunda"),     -10.f,  1.f, 0.f, 0.f, 1.f },
	{ S1D8("GateLodge"),          9.f,  0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8RowC"), {                         // the orangery and a crystal spire house
	{ S1D8("Orangery"),          -8.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D8("CrystalSpireHouse"), 14.f,  2.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8RowD"), {                         // the gate lodge and the onion-dome pavilion
	{ S1D8("GateLodge"),         -9.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D8("OnionPavilion"),      9.f,  1.f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1d8, Nereida: the Crown"), 20.6f, 99.f);
	Site(TEXT("WldS1D8Crown"), -1, 185.f, 54, 12, 1.f);                                  // the hero: slots 474 (21.45 km) and 528 (23.89 km), left. Fix round: at slot 470 a station hub and a far gallery stood inside it
	// the palace rows
	Site(TEXT("WldS1D8RowA"), -1, 43.f, 8, 6, 1.f);                                      // left, s % 8 == 2
	Site(TEXT("WldS1D8RowB"), -1, 43.f, 16, 10, 1.f);                                    // left, s % 16 == 6
	Site(TEXT("WldS1D8RowC"), -1, 43.f, 16, 2, 1.f);                                     // left, s % 16 == 14
	Site(TEXT("WldS1D8RowC"), 1, 45.5f, 8, 0, 1.f);                                      // right, s % 8 == 0
	Site(TEXT("WldS1D8RowD"), 1, 45.5f, 8, 4, 1.f);                                      // right, s % 8 == 4
	// the gardens behind them
	Scatter(S1D8("TerraceBlock"), 66.f, 68.f, 12, 8, 1.f, 1.f, 0.f, -1, 3.f);            // left, s % 12 == 4
	Scatter(S1D8("TerraceBlock"), 64.f, 67.f, 12, 0, 1.f, 1.f, 0.f, 1, 3.f);             // right, s % 12 == 0
	Scatter(S1D8("ArchBridge"), 66.f, 70.f, 12, 2, 1.f, 1.f, 0.f, -1, 2.f);              // left, s % 12 == 10
	Scatter(S1D8("ArchBridge"), 66.f, 70.f, 12, 10, 1.f, 1.f, 0.f, 1, 2.f);              // right, s % 12 == 2
	Scatter(S1D8("MusicRotunda"), 67.f, 70.f, 12, 6, 1.f, 1.f, 0.f, 1, 10.f);            // right, s % 12 == 6
Zone(TEXT("World s1d8, the Crown: the walks, first straight"), 461.5f * 0.045243f, 467.5f * 0.045243f);   // slots 462 to 467
	// the walks by the road, left
	Scatter(S1D8("HedgeGate"), 22.6f, 24.4f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);              // low, s % 8 == 0
	Scatter(S1D8("PennantMast"), 26.5f, 27.8f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);            // behind the hedge
	Scatter(S1D8("Parterre"), 24.5f, 25.2f, 16, 12, 1.f, 1.f, 0.f, -1, 0.f);             // low, s % 16 == 4
	Scatter(S1D8("SeahorseTopiary"), 28.4f, 29.2f, 16, 12, 1.f, 1.f, 0.f, -1, 6.f);      // behind the parterre
	Scatter(S1D8("BubbleFountain"), 26.5f, 27.3f, 16, 4, 1.f, 1.f, 0.f, -1, 0.f);        // s % 16 == 12
	Site(TEXT("WldS1D8Walk"), -1, 25.2f, 12, 10, 1.f);                                   // s % 12 == 2
	Scatter(S1D8("BubbleFountain"), 26.5f, 27.f, 24, 14, 1.f, 1.f, 0.f, -1, 0.f);       // s % 24 == 10
	Scatter(S1D8("SeahorseTopiary"), 25.f, 25.8f, 24, 2, 1.f, 1.f, 0.f, -1, 6.f);       // s % 24 == 22
	// the walks by the road, right
	Scatter(S1D8("HedgeGate"), 21.8f, 23.4f, 8, 2, 1.f, 1.f, 0.f, 1, 0.f);               // low, s % 8 == 6
	Scatter(S1D8("PennantMast"), 26.6f, 28.f, 8, 2, 1.f, 1.f, 0.f, 1, 0.f);              // behind the hedge
	Scatter(S1D8("Parterre"), 23.2f, 24.2f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);                // low, s % 8 == 2
	Site(TEXT("WldS1D8Walk"), 1, 25.f, 8, 4, 1.f);                                       // s % 8 == 4
	Scatter(S1D8("BubbleFountain"), 26.5f, 27.3f, 16, 0, 1.f, 1.f, 0.f, 1, 0.f);           // s % 16 == 0
	Scatter(S1D8("SeahorseTopiary"), 25.f, 25.8f, 16, 8, 1.f, 1.f, 0.f, 1, 6.f);        // s % 16 == 8
#undef S1D8
// ---- END s1d8 ----
}

{
// ---- BEGIN s1plants (what grows on the seabed, district by district, ft_world_sea_s1plants.py) ----
// Written by C:/ftwork/jobs/L6s1plants/make_inl.py. 16 meshes, 488 rule lines in 101 one-slot Zones; every rule: chance 1, one side, every 8, on slots
// s % 4 == 3 only (phase 5 = s % 8 == 3, phase 1 = s % 8 == 7), a band 1 m wide. Counted slot by slot: 488 rule-slots = 488 pieces before the track
// leaves any out; x 0.75 (the brief's share) = about 366 placed. No rule is written on a slot of a glass tunnel, a loop, a corkscrew or a
// jump's gap (the table's own lists), nor on slot 179 (the gate), so the true share may be higher.
// By district: The reef 72, The kelp forest 55, The research station 10, The salvage field 38, The approach 28, Nereida, the Pearl Quarter 87, Nereida, the Bubble Works 65, Nereida, Lamp Row 74, Nereida, the Crown 59.
// By mesh (before the track's share; the most is 60): StarMoss 60, GardenUrn 48, RibbonHedge 42, SeaFans 34, SeaPalm 33, TableCoral 32, TubeWorms 32, BrainCoral 31, SeaLettuce 31, SeaLily 31, LanternWeed 28, SeaPens 24, SpiralWhip 18, TubeSponges 17, PearlVine 17, WreckRib 10.
// Bands (m from the centre line; a rule's x min less the piece's own reach towards the road, scale and jitter in):
//   LOW as placed (top 2 m and less): x min from 19.0, nothing of a piece nearer than 17.4 m.
//   TALL as placed: smallest x min 27.0 (SeaFans), nothing of a piece nearer than 24.2 m. Glow (LanternWeed only, steady): smallest x min 42.5.
//   Beside banked bends (bank .2 and more), rises, jumps' run-ups and landings and the slots next to a tunnel, loop or corkscrew (237 rule-slots):
//   x min 42.5 and more (the piece stands 40 m out and more). Largest x max 60.0.
// A rule-slot is left out where a Scatter or Site rule of FTDressL6.inl or FTPeopleL6.inl stands on the same slot and side within its own
// margin of the band (pressure habitats 8 m, domes 9, vents 7, aprons 9, the tube 2: its face is 31.6 m out; stands, decks, masts and rostrums 2 x
// their scale); reef, weed, pavement, fish and rays overhead are not dodged; plants under 2.75 m as placed may stand under the movers, the wall
// of divers and the patrol (in the water 3 m up and more by their blocks' notes), plants under 3.6 m under the commuters, classes and kelp
// farmers (the cast swims 4 m up and more by the table's header; not seen). On the travel tube's side nothing under 14 m stands behind the
// tube (it hides 8 m): there only sea lilies, sea palms and lantern weed at
// 1.8 to 2.6 times. 441 rule-slots found no room and are not written.
#define S1PLANTS(n) TEXT("/Game/World/UnderTheSea/SM_S1PLANTS_" n)
Zone(TEXT("World s1plants, The reef, slot 3"), 0.1131f, 0.1584f);   // 0.14 km
	Scatter(S1PLANTS("SeaFans"), 30.5f, 31.5f, 8, 5, 1.f, 1.00f, 0.08f, -1, 10.0f);
	Scatter(S1PLANTS("TubeSponges"), 32.0f, 33.0f, 8, 5, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TableCoral"), 58.0f, 59.0f, 8, 5, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("BrainCoral"), 23.5f, 24.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 25.0f, 26.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 27.0f, 28.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TubeSponges"), 28.5f, 29.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TableCoral"), 40.0f, 41.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 41.5f, 42.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("BrainCoral"), 43.0f, 44.0f, 8, 5, 1.f, 1.40f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeSponges"), 44.5f, 45.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TableCoral"), 46.0f, 47.0f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The reef, slot 7"), 0.2941f, 0.3393f);   // 0.32 km
	Scatter(S1PLANTS("TableCoral"), 58.0f, 59.0f, 8, 1, 1.f, 1.30f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TableCoral"), 43.5f, 44.5f, 8, 1, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 45.0f, 46.0f, 8, 1, 1.f, 1.50f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("BrainCoral"), 46.5f, 47.5f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 48.0f, 49.0f, 8, 1, 1.f, 1.70f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeSponges"), 49.5f, 50.5f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 51.0f, 52.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 52.5f, 53.5f, 8, 1, 1.f, 1.10f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The reef, slot 11"), 0.4751f, 0.5203f);   // 0.50 km
	Scatter(S1PLANTS("SeaFans"), 58.0f, 59.0f, 8, 5, 1.f, 1.50f, 0.08f, -1, 10.0f);
	Scatter(S1PLANTS("SeaFans"), 43.5f, 44.5f, 8, 5, 1.f, 1.50f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TubeSponges"), 45.0f, 46.0f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 46.5f, 47.5f, 8, 5, 1.f, 1.70f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TableCoral"), 48.0f, 49.0f, 8, 5, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("BrainCoral"), 49.5f, 50.5f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeSponges"), 51.0f, 52.0f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 52.5f, 53.5f, 8, 5, 1.f, 1.20f, 0.08f, 1, 10.0f);
Zone(TEXT("World s1plants, The reef, slot 23"), 1.0180f, 1.0632f);   // 1.04 km
	Scatter(S1PLANTS("BrainCoral"), 57.5f, 58.5f, 8, 1, 1.f, 1.50f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 59.0f, 60.0f, 8, 1, 1.f, 1.70f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TableCoral"), 43.5f, 44.5f, 8, 1, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 45.0f, 46.0f, 8, 1, 1.f, 1.50f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("BrainCoral"), 46.5f, 47.5f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 48.0f, 49.0f, 8, 1, 1.f, 1.70f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeSponges"), 49.5f, 50.5f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 51.0f, 52.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 52.5f, 53.5f, 8, 1, 1.f, 1.10f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The reef, slot 27"), 1.1989f, 1.2442f);   // 1.22 km
	Scatter(S1PLANTS("StarMoss"), 43.5f, 44.5f, 8, 5, 1.f, 1.70f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 58.0f, 59.0f, 8, 5, 1.f, 1.50f, 0.08f, -1, 10.0f);
	Scatter(S1PLANTS("SeaFans"), 45.0f, 46.0f, 8, 5, 1.f, 1.50f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TubeSponges"), 46.5f, 47.5f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 48.0f, 49.0f, 8, 5, 1.f, 1.70f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TableCoral"), 49.5f, 50.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("BrainCoral"), 51.0f, 52.0f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeSponges"), 52.5f, 53.5f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 54.0f, 55.0f, 8, 5, 1.f, 1.20f, 0.08f, 1, 10.0f);
Zone(TEXT("World s1plants, The reef, slot 31"), 1.3799f, 1.4252f);   // 1.40 km
	Scatter(S1PLANTS("StarMoss"), 25.0f, 26.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("BrainCoral"), 26.5f, 27.5f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TubeSponges"), 28.5f, 29.5f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 58.0f, 59.0f, 8, 1, 1.f, 1.20f, 0.08f, -1, 10.0f);
	Scatter(S1PLANTS("StarMoss"), 25.0f, 26.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("BrainCoral"), 26.5f, 27.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeSponges"), 28.5f, 29.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TableCoral"), 35.0f, 36.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 36.5f, 37.5f, 8, 1, 1.f, 1.20f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 40.0f, 41.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeSponges"), 41.5f, 42.5f, 8, 1, 1.f, 1.40f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 43.0f, 44.0f, 8, 1, 1.f, 1.40f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("BrainCoral"), 44.5f, 45.5f, 8, 1, 1.f, 1.40f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The reef, slot 35"), 1.5609f, 1.6061f);   // 1.58 km
	Scatter(S1PLANTS("SeaFans"), 40.5f, 41.5f, 8, 5, 1.f, 1.00f, 0.08f, -1, 10.0f);
	Scatter(S1PLANTS("TubeSponges"), 42.0f, 43.0f, 8, 5, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("BrainCoral"), 43.5f, 44.5f, 8, 5, 1.f, 1.40f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TableCoral"), 58.0f, 59.0f, 8, 5, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("BrainCoral"), 23.5f, 24.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 25.0f, 26.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeSponges"), 28.5f, 29.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 35.0f, 36.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 36.5f, 37.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 40.0f, 41.0f, 8, 5, 1.f, 1.30f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("BrainCoral"), 41.5f, 42.5f, 8, 5, 1.f, 1.40f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeSponges"), 43.0f, 44.0f, 8, 5, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TableCoral"), 44.5f, 45.5f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The kelp forest, slot 47"), 2.1038f, 2.1490f);   // 2.13 km
	Scatter(S1PLANTS("SeaPalm"), 43.5f, 44.5f, 8, 1, 1.f, 1.50f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 45.0f, 46.0f, 8, 1, 1.f, 1.50f, 0.08f, -1, 8.0f);
	Scatter(S1PLANTS("SeaLettuce"), 46.5f, 47.5f, 8, 1, 1.f, 1.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 48.0f, 49.0f, 8, 1, 1.f, 1.20f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 49.5f, 50.5f, 8, 1, 1.f, 1.70f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 51.0f, 52.0f, 8, 1, 1.f, 1.30f, 0.08f, -1, 8.0f);
	Scatter(S1PLANTS("SeaLettuce"), 52.5f, 53.5f, 8, 1, 1.f, 1.40f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 54.5f, 55.5f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 56.0f, 57.0f, 8, 1, 1.f, 1.50f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("SeaLettuce"), 57.5f, 58.5f, 8, 1, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 59.0f, 60.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The kelp forest, slot 51"), 2.2848f, 2.3300f);   // 2.31 km
	Scatter(S1PLANTS("SeaPalm"), 54.5f, 55.5f, 8, 5, 1.f, 1.40f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 56.0f, 57.0f, 8, 5, 1.f, 1.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 57.5f, 58.5f, 8, 5, 1.f, 1.50f, 0.08f, -1, 8.0f);
	Scatter(S1PLANTS("StarMoss"), 59.0f, 60.0f, 8, 5, 1.f, 1.70f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 43.5f, 44.5f, 8, 5, 1.f, 1.40f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 45.0f, 46.0f, 8, 5, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 46.5f, 47.5f, 8, 5, 1.f, 1.50f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("StarMoss"), 48.0f, 49.0f, 8, 5, 1.f, 1.70f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 49.5f, 50.5f, 8, 5, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 51.0f, 52.0f, 8, 5, 1.f, 1.40f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 52.5f, 53.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, The kelp forest, slot 55"), 2.4657f, 2.5110f);   // 2.49 km
	Scatter(S1PLANTS("SeaPalm"), 49.0f, 50.0f, 8, 1, 1.f, 1.50f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 50.5f, 51.5f, 8, 1, 1.f, 1.50f, 0.08f, -1, 8.0f);
	Scatter(S1PLANTS("SeaLettuce"), 52.0f, 53.0f, 8, 1, 1.f, 1.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 53.5f, 54.5f, 8, 1, 1.f, 1.20f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 55.0f, 56.0f, 8, 1, 1.f, 1.70f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 56.5f, 57.5f, 8, 1, 1.f, 1.30f, 0.08f, -1, 8.0f);
	Scatter(S1PLANTS("SeaLettuce"), 58.0f, 59.0f, 8, 1, 1.f, 1.40f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 54.5f, 55.5f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 56.0f, 57.0f, 8, 1, 1.f, 1.50f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("SeaLettuce"), 57.5f, 58.5f, 8, 1, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 59.0f, 60.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The kelp forest, slot 59"), 2.6467f, 2.6920f);   // 2.67 km
	Scatter(S1PLANTS("SeaPalm"), 54.5f, 55.5f, 8, 5, 1.f, 1.40f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 56.0f, 57.0f, 8, 5, 1.f, 1.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 57.5f, 58.5f, 8, 5, 1.f, 1.50f, 0.08f, -1, 8.0f);
	Scatter(S1PLANTS("StarMoss"), 59.0f, 60.0f, 8, 5, 1.f, 1.70f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 49.0f, 50.0f, 8, 5, 1.f, 1.40f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 50.5f, 51.5f, 8, 5, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 52.0f, 53.0f, 8, 5, 1.f, 1.50f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("StarMoss"), 53.5f, 54.5f, 8, 5, 1.f, 1.70f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 55.0f, 56.0f, 8, 5, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 56.5f, 57.5f, 8, 5, 1.f, 1.40f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 58.0f, 59.0f, 8, 5, 1.f, 1.30f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, The kelp forest, slot 67"), 3.0087f, 3.0539f);   // 3.03 km
	Scatter(S1PLANTS("SeaPalm"), 54.5f, 55.5f, 8, 5, 1.f, 1.50f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 56.0f, 57.0f, 8, 5, 1.f, 1.50f, 0.08f, -1, 8.0f);
	Scatter(S1PLANTS("SeaLettuce"), 57.5f, 58.5f, 8, 5, 1.f, 1.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 59.0f, 60.0f, 8, 5, 1.f, 1.20f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 43.5f, 44.5f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 45.0f, 46.0f, 8, 5, 1.f, 1.50f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("SeaLettuce"), 46.5f, 47.5f, 8, 5, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 48.0f, 49.0f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 49.5f, 50.5f, 8, 5, 1.f, 1.70f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 51.0f, 52.0f, 8, 5, 1.f, 1.30f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("SeaLettuce"), 52.5f, 53.5f, 8, 5, 1.f, 1.40f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The research station, slot 87"), 3.9135f, 3.9588f);   // 3.94 km
	Scatter(S1PLANTS("SpiralWhip"), 43.5f, 44.5f, 8, 1, 1.f, 2.40f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The research station, slot 95"), 4.2755f, 4.3207f);   // 4.30 km
	Scatter(S1PLANTS("StarMoss"), 22.0f, 23.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TubeSponges"), 47.5f, 48.5f, 8, 1, 1.f, 1.30f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaPens"), 49.0f, 50.0f, 8, 1, 1.f, 1.50f, 0.08f, -1, 8.0f);
	Scatter(S1PLANTS("SpiralWhip"), 50.5f, 51.5f, 8, 1, 1.f, 1.30f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaPens"), 52.0f, 53.0f, 8, 1, 1.f, 1.20f, 0.08f, -1, 8.0f);
	Scatter(S1PLANTS("TubeSponges"), 53.5f, 54.5f, 8, 1, 1.f, 1.20f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SpiralWhip"), 40.0f, 41.0f, 8, 1, 1.f, 2.40f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The research station, slot 99"), 4.4564f, 4.5017f);   // 4.48 km
	Scatter(S1PLANTS("StarMoss"), 22.0f, 23.0f, 8, 5, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SpiralWhip"), 40.0f, 41.0f, 8, 5, 1.f, 2.40f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The salvage field, slot 111"), 4.9994f, 5.0446f);   // 5.02 km
	Scatter(S1PLANTS("WreckRib"), 52.5f, 53.5f, 8, 1, 1.f, 1.00f, 0.08f, -1, 10.0f);
	Scatter(S1PLANTS("SeaLettuce"), 43.5f, 44.5f, 8, 1, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 45.0f, 46.0f, 8, 1, 1.f, 1.70f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 46.5f, 47.5f, 8, 1, 1.f, 1.30f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The salvage field, slot 115"), 5.1803f, 5.2256f);   // 5.20 km
	Scatter(S1PLANTS("StarMoss"), 43.5f, 44.5f, 8, 5, 1.f, 1.70f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 45.0f, 46.0f, 8, 5, 1.f, 1.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SpiralWhip"), 43.5f, 44.5f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("WreckRib"), 45.0f, 46.0f, 8, 5, 1.f, 1.10f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("StarMoss"), 46.5f, 47.5f, 8, 5, 1.f, 1.70f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 48.0f, 49.0f, 8, 5, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("WreckRib"), 49.5f, 50.5f, 8, 5, 1.f, 0.90f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("SpiralWhip"), 51.0f, 52.0f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The salvage field, slot 119"), 5.3613f, 5.4065f);   // 5.38 km
	Scatter(S1PLANTS("WreckRib"), 49.0f, 50.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 10.0f);
	Scatter(S1PLANTS("SpiralWhip"), 50.5f, 51.5f, 8, 1, 1.f, 1.40f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 52.0f, 53.0f, 8, 1, 1.f, 1.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 53.5f, 54.5f, 8, 1, 1.f, 1.70f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SpiralWhip"), 55.0f, 56.0f, 8, 1, 1.f, 1.10f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 56.5f, 57.5f, 8, 1, 1.f, 1.30f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("WreckRib"), 56.5f, 57.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 10.0f);
Zone(TEXT("World s1plants, The salvage field, slot 127"), 5.7232f, 5.7685f);   // 5.75 km
	Scatter(S1PLANTS("SpiralWhip"), 45.0f, 46.0f, 8, 1, 1.f, 1.50f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("WreckRib"), 46.5f, 47.5f, 8, 1, 1.f, 1.10f, 0.08f, -1, 10.0f);
	Scatter(S1PLANTS("StarMoss"), 43.5f, 44.5f, 8, 1, 1.f, 1.70f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 45.0f, 46.0f, 8, 1, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SpiralWhip"), 54.0f, 55.0f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("WreckRib"), 55.5f, 56.5f, 8, 1, 1.f, 1.10f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("WreckRib"), 57.0f, 58.0f, 8, 1, 1.f, 0.90f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("SpiralWhip"), 58.5f, 59.5f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The salvage field, slot 135"), 6.0852f, 6.1304f);   // 6.11 km
	Scatter(S1PLANTS("SeaLettuce"), 43.5f, 44.5f, 8, 1, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 45.0f, 46.0f, 8, 1, 1.f, 1.70f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 46.5f, 47.5f, 8, 1, 1.f, 1.30f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The salvage field, slot 139"), 6.2662f, 6.3114f);   // 6.29 km
	Scatter(S1PLANTS("StarMoss"), 43.5f, 44.5f, 8, 5, 1.f, 1.70f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 45.0f, 46.0f, 8, 5, 1.f, 1.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SpiralWhip"), 43.5f, 44.5f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("WreckRib"), 45.0f, 46.0f, 8, 5, 1.f, 1.10f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("StarMoss"), 46.5f, 47.5f, 8, 5, 1.f, 1.70f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 48.0f, 49.0f, 8, 5, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("WreckRib"), 49.5f, 50.5f, 8, 5, 1.f, 0.90f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("SpiralWhip"), 51.0f, 52.0f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The approach, slot 147"), 6.6281f, 6.6733f);   // 6.65 km
	Scatter(S1PLANTS("TubeWorms"), 43.5f, 44.5f, 8, 5, 1.f, 1.50f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SpiralWhip"), 45.0f, 46.0f, 8, 5, 1.f, 1.40f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 46.5f, 47.5f, 8, 5, 1.f, 1.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 48.0f, 49.0f, 8, 5, 1.f, 1.70f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 49.5f, 50.5f, 8, 5, 1.f, 1.20f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SpiralWhip"), 51.0f, 52.0f, 8, 5, 1.f, 1.20f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 58.5f, 59.5f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The approach, slot 155"), 6.9900f, 7.0353f);   // 7.01 km
	Scatter(S1PLANTS("TubeWorms"), 47.5f, 48.5f, 8, 5, 1.f, 1.40f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SpiralWhip"), 49.0f, 50.0f, 8, 5, 1.f, 1.30f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 50.5f, 51.5f, 8, 5, 1.f, 1.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 52.0f, 53.0f, 8, 5, 1.f, 1.20f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 58.5f, 59.5f, 8, 5, 1.f, 1.40f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The approach, slot 159"), 7.1710f, 7.2163f);   // 7.19 km
	Scatter(S1PLANTS("TubeWorms"), 43.5f, 44.5f, 8, 1, 1.f, 1.50f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SpiralWhip"), 45.0f, 46.0f, 8, 1, 1.f, 1.40f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 46.5f, 47.5f, 8, 1, 1.f, 1.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 48.0f, 49.0f, 8, 1, 1.f, 1.70f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 49.5f, 50.5f, 8, 1, 1.f, 1.20f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SpiralWhip"), 51.0f, 52.0f, 8, 1, 1.f, 1.20f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 58.5f, 59.5f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The approach, slot 163"), 7.3520f, 7.3972f);   // 7.37 km
	Scatter(S1PLANTS("SeaLettuce"), 43.5f, 44.5f, 8, 5, 1.f, 1.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 45.0f, 46.0f, 8, 5, 1.f, 1.70f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 52.5f, 53.5f, 8, 5, 1.f, 1.50f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 58.5f, 59.5f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, The approach, slot 175"), 7.8949f, 7.9401f);   // 7.92 km
	Scatter(S1PLANTS("StarMoss"), 21.5f, 22.5f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 40.0f, 41.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaLettuce"), 41.5f, 42.5f, 8, 1, 1.f, 1.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 52.5f, 53.5f, 8, 1, 1.f, 1.40f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 58.5f, 59.5f, 8, 1, 1.f, 1.40f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 183"), 8.2568f, 8.3021f);   // 8.28 km
	Scatter(S1PLANTS("StarMoss"), 21.5f, 22.5f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 27.0f, 28.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 22.0f, 23.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 47.5f, 48.5f, 8, 1, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 54.5f, 55.5f, 8, 1, 1.f, 1.20f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 56.0f, 57.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 187"), 8.4378f, 8.4831f);   // 8.46 km
	Scatter(S1PLANTS("GardenUrn"), 42.0f, 43.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 43.5f, 44.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 45.0f, 46.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("PearlVine"), 46.5f, 47.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 6.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 191"), 8.6188f, 8.6640f);   // 8.64 km
	Scatter(S1PLANTS("GardenUrn"), 27.0f, 28.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 34.0f, 35.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 195"), 8.7998f, 8.8450f);   // 8.82 km
	Scatter(S1PLANTS("StarMoss"), 22.0f, 23.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 47.5f, 48.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 54.5f, 55.5f, 8, 5, 1.f, 1.20f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 56.0f, 57.0f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 199"), 8.9807f, 9.0260f);   // 9.00 km
	Scatter(S1PLANTS("GardenUrn"), 27.0f, 28.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 42.0f, 43.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 43.5f, 44.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 45.0f, 46.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("PearlVine"), 46.5f, 47.5f, 8, 1, 1.f, 1.30f, 0.08f, 1, 6.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 203"), 9.1617f, 9.2070f);   // 9.18 km
	Scatter(S1PLANTS("SeaFans"), 27.5f, 28.5f, 8, 5, 1.f, 1.00f, 0.08f, -1, 10.0f);
	Scatter(S1PLANTS("GardenUrn"), 29.0f, 30.0f, 8, 5, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 22.0f, 23.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 34.0f, 35.0f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 207"), 9.3427f, 9.3879f);   // 9.37 km
	Scatter(S1PLANTS("StarMoss"), 21.5f, 22.5f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 27.0f, 28.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 22.0f, 23.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 47.5f, 48.5f, 8, 1, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 54.5f, 55.5f, 8, 1, 1.f, 1.20f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 56.0f, 57.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 211"), 9.5237f, 9.5689f);   // 9.55 km
	Scatter(S1PLANTS("GardenUrn"), 42.0f, 43.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 43.5f, 44.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 45.0f, 46.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("PearlVine"), 46.5f, 47.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 6.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 215"), 9.7046f, 9.7499f);   // 9.73 km
	Scatter(S1PLANTS("GardenUrn"), 27.0f, 28.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 22.0f, 23.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 34.0f, 35.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TableCoral"), 38.5f, 39.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 219"), 9.8856f, 9.9308f);   // 9.91 km
	Scatter(S1PLANTS("StarMoss"), 22.0f, 23.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 58.5f, 59.5f, 8, 5, 1.f, 1.20f, 0.08f, 1, 10.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 223"), 10.0666f, 10.1118f);   // 10.09 km
	Scatter(S1PLANTS("GardenUrn"), 42.0f, 43.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 43.5f, 44.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 45.0f, 46.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("PearlVine"), 46.5f, 47.5f, 8, 1, 1.f, 1.30f, 0.08f, 1, 6.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 227"), 10.2475f, 10.2928f);   // 10.27 km
	Scatter(S1PLANTS("GardenUrn"), 34.0f, 35.0f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TableCoral"), 38.5f, 39.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 231"), 10.4285f, 10.4738f);   // 10.45 km
	Scatter(S1PLANTS("StarMoss"), 21.5f, 22.5f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 27.0f, 28.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 22.0f, 23.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 47.5f, 48.5f, 8, 1, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 54.5f, 55.5f, 8, 1, 1.f, 1.20f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 56.0f, 57.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 235"), 10.6095f, 10.6547f);   // 10.63 km
	Scatter(S1PLANTS("GardenUrn"), 42.0f, 43.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 43.5f, 44.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 45.0f, 46.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("PearlVine"), 46.5f, 47.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 6.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 239"), 10.7905f, 10.8357f);   // 10.81 km
	Scatter(S1PLANTS("GardenUrn"), 27.0f, 28.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 22.0f, 23.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 34.0f, 35.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 243"), 10.9714f, 11.0167f);   // 10.99 km
	Scatter(S1PLANTS("StarMoss"), 22.0f, 23.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 47.5f, 48.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 54.5f, 55.5f, 8, 5, 1.f, 1.20f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 56.0f, 57.0f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 247"), 11.1524f, 11.1976f);   // 11.18 km
	Scatter(S1PLANTS("GardenUrn"), 27.0f, 28.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 251"), 11.3334f, 11.3786f);   // 11.36 km
	Scatter(S1PLANTS("StarMoss"), 22.0f, 23.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 34.0f, 35.0f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TableCoral"), 38.5f, 39.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 255"), 11.5143f, 11.5596f);   // 11.54 km
	Scatter(S1PLANTS("StarMoss"), 21.5f, 22.5f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 27.0f, 28.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 22.0f, 23.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 47.5f, 48.5f, 8, 1, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 54.5f, 55.5f, 8, 1, 1.f, 1.20f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 56.0f, 57.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 259"), 11.6953f, 11.7406f);   // 11.72 km
	Scatter(S1PLANTS("GardenUrn"), 42.0f, 43.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 43.5f, 44.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 45.0f, 46.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("PearlVine"), 46.5f, 47.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 6.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 263"), 11.8763f, 11.9215f);   // 11.90 km
	Scatter(S1PLANTS("GardenUrn"), 27.0f, 28.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 34.0f, 35.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TableCoral"), 38.5f, 39.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Pearl Quarter, slot 267"), 12.0573f, 12.1025f);   // 12.08 km
	Scatter(S1PLANTS("GardenUrn"), 27.0f, 28.0f, 8, 5, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 28.5f, 29.5f, 8, 5, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 22.0f, 23.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 47.5f, 48.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaFans"), 54.5f, 55.5f, 8, 5, 1.f, 1.20f, 0.08f, 1, 10.0f);
	Scatter(S1PLANTS("TableCoral"), 56.0f, 57.0f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 271"), 12.2382f, 12.2835f);   // 12.26 km
	Scatter(S1PLANTS("StarMoss"), 23.5f, 24.5f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 23.5f, 24.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 42.5f, 43.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 44.0f, 45.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 45.5f, 46.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 275"), 12.4192f, 12.4644f);   // 12.44 km
	Scatter(S1PLANTS("TubeWorms"), 27.5f, 28.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 279"), 12.6002f, 12.6454f);   // 12.62 km
	Scatter(S1PLANTS("StarMoss"), 21.5f, 22.5f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 22.0f, 23.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 54.5f, 55.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 56.0f, 57.0f, 8, 1, 1.f, 1.40f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 57.5f, 58.5f, 8, 1, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 59.0f, 60.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 283"), 12.7811f, 12.8264f);   // 12.80 km
	Scatter(S1PLANTS("TubeWorms"), 42.5f, 43.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 44.0f, 45.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 45.5f, 46.5f, 8, 5, 1.f, 1.10f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 287"), 12.9621f, 13.0074f);   // 12.98 km
	Scatter(S1PLANTS("StarMoss"), 23.0f, 24.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 40.0f, 41.0f, 8, 1, 1.f, 1.80f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 22.0f, 23.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 27.5f, 28.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 38.5f, 39.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 291"), 13.1431f, 13.1883f);   // 13.17 km
	Scatter(S1PLANTS("SeaLily"), 54.5f, 55.5f, 8, 5, 1.f, 1.10f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 56.0f, 57.0f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 57.5f, 58.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 59.0f, 60.0f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 295"), 13.3241f, 13.3693f);   // 13.35 km
	Scatter(S1PLANTS("StarMoss"), 23.5f, 24.5f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 23.5f, 24.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 42.5f, 43.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 44.0f, 45.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 45.5f, 46.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 299"), 13.5050f, 13.5503f);   // 13.53 km
	Scatter(S1PLANTS("TubeWorms"), 27.5f, 28.5f, 8, 5, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 40.0f, 41.0f, 8, 5, 1.f, 1.80f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 31.5f, 32.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 38.5f, 39.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 303"), 13.6860f, 13.7313f);   // 13.71 km
	Scatter(S1PLANTS("SeaLily"), 58.5f, 59.5f, 8, 1, 1.f, 1.30f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 307"), 13.8670f, 13.9122f);   // 13.89 km
	Scatter(S1PLANTS("SeaPalm"), 43.5f, 44.5f, 8, 5, 1.f, 1.40f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 45.0f, 46.0f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 46.5f, 47.5f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 315"), 14.2289f, 14.2742f);   // 14.25 km
	Scatter(S1PLANTS("StarMoss"), 47.0f, 48.0f, 8, 5, 1.f, 1.70f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 54.5f, 55.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 56.0f, 57.0f, 8, 5, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 57.5f, 58.5f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 59.0f, 60.0f, 8, 5, 1.f, 1.10f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 319"), 14.4099f, 14.4551f);   // 14.43 km
	Scatter(S1PLANTS("StarMoss"), 23.5f, 24.5f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("StarMoss"), 23.5f, 24.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 42.5f, 43.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 44.0f, 45.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 45.5f, 46.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 323"), 14.5909f, 14.6361f);   // 14.61 km
	Scatter(S1PLANTS("SeaPalm"), 43.5f, 44.5f, 8, 5, 1.f, 1.80f, 0.08f, -1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 327"), 14.7718f, 14.8171f);   // 14.79 km
	Scatter(S1PLANTS("StarMoss"), 47.0f, 48.0f, 8, 1, 1.f, 1.70f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 54.5f, 55.5f, 8, 1, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 56.0f, 57.0f, 8, 1, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 57.5f, 58.5f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 59.0f, 60.0f, 8, 1, 1.f, 1.10f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 339"), 15.3148f, 15.3600f);   // 15.34 km
	Scatter(S1PLANTS("SeaLily"), 54.5f, 55.5f, 8, 5, 1.f, 1.10f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 56.0f, 57.0f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPalm"), 57.5f, 58.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 59.0f, 60.0f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 343"), 15.4957f, 15.5410f);   // 15.52 km
	Scatter(S1PLANTS("SeaPalm"), 43.5f, 44.5f, 8, 1, 1.f, 1.40f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 45.0f, 46.0f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 46.5f, 47.5f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 347"), 15.6767f, 15.7219f);   // 15.70 km
	Scatter(S1PLANTS("SeaLily"), 43.5f, 44.5f, 8, 5, 1.f, 1.80f, 0.08f, -1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Bubble Works, slot 351"), 15.8577f, 15.9029f);   // 15.88 km
	Scatter(S1PLANTS("SeaPalm"), 54.5f, 55.5f, 8, 1, 1.f, 1.40f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 56.0f, 57.0f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 57.5f, 58.5f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("TubeWorms"), 59.0f, 60.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 359"), 16.2196f, 16.2649f);   // 16.24 km
	Scatter(S1PLANTS("RibbonHedge"), 23.0f, 24.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 8.0f);
	Scatter(S1PLANTS("SeaPens"), 27.0f, 28.0f, 8, 1, -1.f, 1.00f, 0.08f, -1, 8.0f);   // fix round: chance -1 = never placed (it stood on the watchers' wall at 26 to 27.5 m); the line stays so that no later rule re-rolls
	Scatter(S1PLANTS("RibbonHedge"), 22.0f, 23.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("SeaPens"), 27.0f, 28.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("RibbonHedge"), 34.0f, 35.0f, 8, 1, 1.f, 1.40f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 363"), 16.4006f, 16.4458f);   // 16.42 km
	Scatter(S1PLANTS("RibbonHedge"), 47.0f, 48.0f, 8, 5, 1.f, 1.60f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("SeaPens"), 48.5f, 49.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("LanternWeed"), 54.5f, 55.5f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPens"), 56.0f, 57.0f, 8, 5, 1.f, 1.60f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("SeaLily"), 57.5f, 58.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 367"), 16.5816f, 16.6268f);   // 16.60 km
	Scatter(S1PLANTS("SeaPens"), 43.5f, 44.5f, 8, 1, 1.f, 1.60f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("LanternWeed"), 45.0f, 46.0f, 8, 1, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 46.5f, 47.5f, 8, 1, 1.f, 1.60f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 371"), 16.7625f, 16.8078f);   // 16.79 km
	Scatter(S1PLANTS("SeaLily"), 43.5f, 44.5f, 8, 5, 1.f, 1.80f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 45.0f, 46.0f, 8, 5, 1.f, 1.60f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 379"), 17.1245f, 17.1697f);   // 17.15 km
	Scatter(S1PLANTS("RibbonHedge"), 23.5f, 24.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("LanternWeed"), 42.5f, 43.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPens"), 44.0f, 45.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("SeaLily"), 45.5f, 46.5f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 47.0f, 48.0f, 8, 5, 1.f, 1.60f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 383"), 17.3054f, 17.3507f);   // 17.33 km
	Scatter(S1PLANTS("RibbonHedge"), 23.0f, 24.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 8.0f);
	Scatter(S1PLANTS("SeaPens"), 27.0f, 28.0f, 8, 1, -1.f, 1.00f, 0.08f, -1, 8.0f);   // fix round: chance -1 = never placed (it stood on the watchers' wall at 26 to 27.5 m); the line stays so that no later rule re-rolls
	Scatter(S1PLANTS("SeaLily"), 40.0f, 41.0f, 8, 1, 1.f, 1.80f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 22.0f, 23.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("SeaPens"), 27.0f, 28.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("RibbonHedge"), 34.0f, 35.0f, 8, 1, 1.f, 1.40f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 387"), 17.4864f, 17.5317f);   // 17.51 km
	Scatter(S1PLANTS("SeaPens"), 58.5f, 59.5f, 8, 5, 1.f, 1.60f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 391"), 17.6674f, 17.7126f);   // 17.69 km
	Scatter(S1PLANTS("LanternWeed"), 43.5f, 44.5f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPens"), 45.0f, 46.0f, 8, 1, 1.f, 1.60f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("RibbonHedge"), 46.5f, 47.5f, 8, 1, 1.f, 1.60f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 399"), 18.0293f, 18.0746f);   // 18.05 km
	Scatter(S1PLANTS("RibbonHedge"), 47.0f, 48.0f, 8, 1, 1.f, 1.60f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("SeaPens"), 54.5f, 55.5f, 8, 1, 1.f, 1.60f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("LanternWeed"), 56.0f, 57.0f, 8, 1, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 57.5f, 58.5f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("LanternWeed"), 59.0f, 60.0f, 8, 1, 1.f, 1.30f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 403"), 18.2103f, 18.2556f);   // 18.23 km
	Scatter(S1PLANTS("RibbonHedge"), 23.5f, 24.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("LanternWeed"), 42.5f, 43.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPens"), 44.0f, 45.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("SeaLily"), 45.5f, 46.5f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 47.0f, 48.0f, 8, 5, 1.f, 1.60f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 407"), 18.3913f, 18.4365f);   // 18.41 km
	Scatter(S1PLANTS("SeaLily"), 43.5f, 44.5f, 8, 1, 1.f, 1.80f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 45.0f, 46.0f, 8, 1, 1.f, 1.60f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 411"), 18.5723f, 18.6175f);   // 18.59 km
	Scatter(S1PLANTS("RibbonHedge"), 47.0f, 48.0f, 8, 5, 1.f, 1.60f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("SeaPens"), 54.5f, 55.5f, 8, 5, 1.f, 1.60f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("LanternWeed"), 56.0f, 57.0f, 8, 5, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaLily"), 57.5f, 58.5f, 8, 5, 1.f, 1.20f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("LanternWeed"), 59.0f, 60.0f, 8, 5, 1.f, 1.30f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 423"), 19.1152f, 19.1604f);   // 19.14 km
	Scatter(S1PLANTS("RibbonHedge"), 19.0f, 20.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 8.0f);
	Scatter(S1PLANTS("SeaPens"), 27.0f, 28.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 8.0f);
	Scatter(S1PLANTS("RibbonHedge"), 22.0f, 23.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("RibbonHedge"), 47.0f, 48.0f, 8, 1, 1.f, 1.40f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("SeaLily"), 54.5f, 55.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("LanternWeed"), 56.0f, 57.0f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPens"), 57.5f, 58.5f, 8, 1, 1.f, 1.50f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 427"), 19.2961f, 19.3414f);   // 19.32 km
	Scatter(S1PLANTS("LanternWeed"), 43.5f, 44.5f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPens"), 45.0f, 46.0f, 8, 5, 1.f, 1.60f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("RibbonHedge"), 46.5f, 47.5f, 8, 5, 1.f, 1.60f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 431"), 19.4771f, 19.5224f);   // 19.50 km
	Scatter(S1PLANTS("LanternWeed"), 43.5f, 44.5f, 8, 1, 1.f, 2.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 45.0f, 46.0f, 8, 1, 1.f, 1.60f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 443"), 20.0200f, 20.0653f);   // 20.04 km
	Scatter(S1PLANTS("SeaPens"), 31.0f, 32.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 447"), 20.2010f, 20.2462f);   // 20.22 km
	Scatter(S1PLANTS("RibbonHedge"), 47.0f, 48.0f, 8, 1, 1.f, 1.60f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("SeaPens"), 48.5f, 49.5f, 8, 1, 1.f, 1.30f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("LanternWeed"), 54.5f, 55.5f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("SeaPens"), 56.0f, 57.0f, 8, 1, 1.f, 1.60f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("SeaLily"), 57.5f, 58.5f, 8, 1, 1.f, 1.30f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 451"), 20.3820f, 20.4272f);   // 20.40 km
	Scatter(S1PLANTS("SeaPens"), 43.5f, 44.5f, 8, 5, 1.f, 1.60f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("LanternWeed"), 45.0f, 46.0f, 8, 5, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 46.5f, 47.5f, 8, 5, 1.f, 1.60f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, Nereida, Lamp Row, slot 455"), 20.5629f, 20.6082f);   // 20.59 km
	Scatter(S1PLANTS("RibbonHedge"), 23.0f, 24.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 8.0f);
	Scatter(S1PLANTS("SeaPens"), 27.0f, 28.0f, 8, 1, -1.f, 1.00f, 0.08f, -1, 8.0f);   // fix round: chance -1 = never placed (it stood on the watchers' wall at 26 to 27.5 m); the line stays so that no later rule re-rolls
	Scatter(S1PLANTS("SeaLily"), 40.0f, 41.0f, 8, 1, 1.f, 1.80f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("RibbonHedge"), 22.0f, 23.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("SeaPens"), 27.0f, 28.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("RibbonHedge"), 34.0f, 35.0f, 8, 1, 1.f, 1.40f, 0.08f, 1, 8.0f);
Zone(TEXT("World s1plants, Nereida, the Crown, slot 463"), 20.9249f, 20.9701f);   // 20.95 km
	Scatter(S1PLANTS("BrainCoral"), 23.5f, 24.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("PearlVine"), 42.5f, 43.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 6.0f);
	Scatter(S1PLANTS("LanternWeed"), 44.0f, 45.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 45.5f, 46.5f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("BrainCoral"), 47.0f, 48.0f, 8, 1, 1.f, 1.40f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Crown, slot 467"), 21.1059f, 21.1511f);   // 21.13 km
	Scatter(S1PLANTS("LanternWeed"), 43.5f, 44.5f, 8, 5, 1.f, 2.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("BrainCoral"), 45.0f, 46.0f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Crown, slot 471"), 21.2868f, 21.3321f);   // 21.31 km
	Scatter(S1PLANTS("GardenUrn"), 58.5f, 59.5f, 8, 1, 1.f, 1.60f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Crown, slot 475"), 21.4678f, 21.5130f);   // 21.49 km
	Scatter(S1PLANTS("PearlVine"), 42.5f, 43.5f, 8, 5, 1.f, 1.40f, 0.08f, 1, 6.0f);
	Scatter(S1PLANTS("LanternWeed"), 44.0f, 45.0f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 45.5f, 46.5f, 8, 5, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("BrainCoral"), 47.0f, 48.0f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Crown, slot 483"), 21.8297f, 21.8750f);   // 21.85 km
	Scatter(S1PLANTS("BrainCoral"), 22.0f, 23.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("PearlVine"), 54.5f, 55.5f, 8, 5, 1.f, 1.20f, 0.08f, 1, 6.0f);
	Scatter(S1PLANTS("LanternWeed"), 56.0f, 57.0f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 57.5f, 58.5f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Crown, slot 487"), 22.0107f, 22.0560f);   // 22.03 km
	Scatter(S1PLANTS("BrainCoral"), 23.5f, 24.5f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 27.0f, 28.0f, 8, 1, -1.f, 1.00f, 0.08f, -1, 180.0f);   // fix round: chance -1 = never placed (it stood on the watchers' wall at 26 to 27.5 m); the line stays so that no later rule re-rolls
	Scatter(S1PLANTS("BrainCoral"), 23.5f, 24.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("PearlVine"), 42.5f, 43.5f, 8, 1, 1.f, 1.00f, 0.08f, 1, 6.0f);
	Scatter(S1PLANTS("LanternWeed"), 44.0f, 45.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 45.5f, 46.5f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("BrainCoral"), 47.0f, 48.0f, 8, 1, 1.f, 1.40f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Crown, slot 491"), 22.1917f, 22.2369f);   // 22.21 km
	Scatter(S1PLANTS("LanternWeed"), 43.5f, 44.5f, 8, 5, 1.f, 2.60f, 0.08f, -1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Crown, slot 495"), 22.3727f, 22.4179f);   // 22.40 km
	Scatter(S1PLANTS("BrainCoral"), 47.0f, 48.0f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("PearlVine"), 54.5f, 55.5f, 8, 1, 1.f, 1.40f, 0.08f, 1, 6.0f);
	Scatter(S1PLANTS("LanternWeed"), 56.0f, 57.0f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 57.5f, 58.5f, 8, 1, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("PearlVine"), 59.0f, 60.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 6.0f);
Zone(TEXT("World s1plants, Nereida, the Crown, slot 507"), 22.9156f, 22.9608f);   // 22.94 km
	Scatter(S1PLANTS("BrainCoral"), 22.0f, 23.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("PearlVine"), 54.5f, 55.5f, 8, 5, 1.f, 1.20f, 0.08f, 1, 6.0f);
	Scatter(S1PLANTS("LanternWeed"), 56.0f, 57.0f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 57.5f, 58.5f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Crown, slot 511"), 23.0966f, 23.1418f);   // 23.12 km
	Scatter(S1PLANTS("GardenUrn"), 43.5f, 44.5f, 8, 1, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("BrainCoral"), 45.0f, 46.0f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("PearlVine"), 46.5f, 47.5f, 8, 1, 1.f, 1.40f, 0.08f, 1, 6.0f);
Zone(TEXT("World s1plants, Nereida, the Crown, slot 515"), 23.2775f, 23.3228f);   // 23.30 km
	Scatter(S1PLANTS("LanternWeed"), 43.5f, 44.5f, 8, 5, 1.f, 2.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("BrainCoral"), 45.0f, 46.0f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Crown, slot 527"), 23.8204f, 23.8657f);   // 23.84 km
	Scatter(S1PLANTS("BrainCoral"), 23.0f, 24.0f, 8, 1, 1.f, 1.00f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 27.0f, 28.0f, 8, 1, -1.f, 1.00f, 0.08f, -1, 180.0f);   // fix round: chance -1 = never placed (it stood on the watchers' wall at 26 to 27.5 m); the line stays so that no later rule re-rolls
	Scatter(S1PLANTS("BrainCoral"), 22.0f, 23.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 27.0f, 28.0f, 8, 1, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 34.0f, 35.0f, 8, 1, 1.f, 1.20f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Crown, slot 531"), 24.0014f, 24.0467f);   // 24.02 km
	Scatter(S1PLANTS("BrainCoral"), 47.0f, 48.0f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 48.5f, 49.5f, 8, 5, 1.f, 1.30f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 54.5f, 55.5f, 8, 5, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("PearlVine"), 56.0f, 57.0f, 8, 5, 1.f, 1.40f, 0.08f, 1, 6.0f);
	Scatter(S1PLANTS("LanternWeed"), 57.5f, 58.5f, 8, 5, 1.f, 1.60f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Crown, slot 535"), 24.1824f, 24.2276f);   // 24.21 km
	Scatter(S1PLANTS("PearlVine"), 42.5f, 43.5f, 8, 1, 1.f, 1.40f, 0.08f, 1, 6.0f);
	Scatter(S1PLANTS("LanternWeed"), 44.0f, 45.0f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 45.5f, 46.5f, 8, 1, 1.f, 1.60f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("BrainCoral"), 47.0f, 48.0f, 8, 1, 1.f, 1.50f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Crown, slot 539"), 24.3634f, 24.4086f);   // 24.39 km
	Scatter(S1PLANTS("LanternWeed"), 43.5f, 44.5f, 8, 5, 1.f, 2.60f, 0.08f, -1, 180.0f);
	Scatter(S1PLANTS("BrainCoral"), 45.0f, 46.0f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
Zone(TEXT("World s1plants, Nereida, the Crown, slot 547"), 24.7253f, 24.7705f);   // 24.75 km
	Scatter(S1PLANTS("BrainCoral"), 23.5f, 24.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("GardenUrn"), 42.0f, 43.0f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("LanternWeed"), 43.5f, 44.5f, 8, 5, 1.f, 1.00f, 0.08f, 1, 180.0f);
	Scatter(S1PLANTS("PearlVine"), 45.0f, 46.0f, 8, 5, 1.f, 1.20f, 0.08f, 1, 6.0f);
	Scatter(S1PLANTS("LanternWeed"), 46.5f, 47.5f, 8, 5, 1.f, 1.50f, 0.08f, 1, 180.0f);
#undef S1PLANTS
// ---- END s1plants ----
}

{
// ---- BEGIN s1animals (the animals of all eight districts, ft_world_sea_s1animals.py) ----
// 16 meshes. Estimate: ground 245 Scatter lines (one piece each, chance 1) x 0.75 = 184; far water 41 Far pieces x 1 = 41; about 225 placed.
// Written (before the 0.75): BaitBall 14, Cuttlefish 31, Flounders 9, HermitCrabs 18, Lionfish 23, ParrotFish 18, PigKeeper 7, Puffers 34, SeaAngels 17, SeaPigs 6, SeaSlugs 17, Seals 40, Shrimps 8, SpiderCrabs 17, Starfish 17, Sunfish 10.
// Bands and slots used: ground 35 to 44 m on s % 8 == 5 (banked or not known: from 40.5 m); 46 to 67 m on s % 4 == 1 (61 to 67 m also
// s % 4 == 3); 72 to 116 m on odd slots (SpiderCrabs, Cuttlefish, Puffers, Lionfish, Seals: those five never nearer than 72 m, the
// other eight never past 67 m, so no mesh stands on both sides of the 70 m shadow line). Every piece here is placed as TALL
// (nothing nearer than 35 m). One Zone a slot, every 1, chance 1: each line was tried against both tables by a scratch tool
// (C:/ftwork/jobs/L6s1animals/plan.py reads the two .inl files: same slot and side, clear of every population piece, of every
// built or tall dressing piece in a band up to 16 m wide and of every part of a Site; low reef, coral gardens, grass and the
// flat pavements count as ground to stand among; a dressing piece scattered over a band wider than 16 m cannot be dodged by
// rule). No tunnel, loop, corkscrew or jump slot (run-up and landing slots 52-60, 118-125 left out too), nor the slot either side.
// Far water: Far with a negative drop, 62 m and more out, in slots where nothing of the tables swims or stands within 12 m across;
// SeaAngels and Sunfish go along the road (yaw jitter 0); BaitBall and Seals turn about their own middle only.
#define S1ANIMALS(n) TEXT("/Game/World/UnderTheSea/SM_S1ANIMALS_" n)
Zone(TEXT("World s1animals, slot 1"), 0.02307f, 0.06741f);
	Scatter(S1ANIMALS("ParrotFish"), 65.5f, 66.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("HermitCrabs"), 60.0f, 61.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 3"), 0.11356f, 0.1579f);
	Scatter(S1ANIMALS("Starfish"), 61.0f, 62.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("ParrotFish"), 61.5f, 62.5f, 1, 0, 1.f, 1.45f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 5"), 0.20405f, 0.24838f);
	Scatter(S1ANIMALS("HermitCrabs"), 62.0f, 63.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Starfish"), 64.0f, 65.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 7"), 0.29453f, 0.33887f);
	Scatter(S1ANIMALS("ParrotFish"), 61.0f, 62.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("HermitCrabs"), 61.0f, 62.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 9"), 0.38502f, 0.42936f);
	Scatter(S1ANIMALS("Starfish"), 64.0f, 65.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("ParrotFish"), 63.5f, 64.5f, 1, 0, 1.f, 1.45f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 11"), 0.4755f, 0.51984f);
	Scatter(S1ANIMALS("HermitCrabs"), 61.0f, 62.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Starfish"), 61.0f, 62.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 13"), 0.56599f, 0.61033f);
	Scatter(S1ANIMALS("HermitCrabs"), 42.0f, 43.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("ParrotFish"), 54.5f, 55.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Starfish"), 64.0f, 65.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 23"), 1.01842f, 1.06276f);
	Scatter(S1ANIMALS("ParrotFish"), 61.0f, 62.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("HermitCrabs"), 62.0f, 63.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 25"), 1.10891f, 1.15324f);
	Scatter(S1ANIMALS("Starfish"), 64.0f, 65.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("ParrotFish"), 61.5f, 62.5f, 1, 0, 1.f, 1.45f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 27"), 1.19939f, 1.24373f);
	Scatter(S1ANIMALS("HermitCrabs"), 61.0f, 62.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Starfish"), 61.0f, 62.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 29"), 1.28988f, 1.33422f);
	Scatter(S1ANIMALS("HermitCrabs"), 40.5f, 41.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("ParrotFish"), 53.0f, 54.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Starfish"), 64.0f, 65.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 31"), 1.38036f, 1.4247f);
	Scatter(S1ANIMALS("ParrotFish"), 61.0f, 62.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("HermitCrabs"), 61.0f, 62.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 33"), 1.47085f, 1.51519f);
	Scatter(S1ANIMALS("Starfish"), 64.0f, 65.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("ParrotFish"), 61.5f, 62.5f, 1, 0, 1.f, 1.45f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 35"), 1.56134f, 1.60567f);
	Scatter(S1ANIMALS("HermitCrabs"), 61.0f, 62.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Starfish"), 61.0f, 62.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 40"), 1.78755f, 1.83189f);
	Far(S1ANIMALS("BaitBall"), 100.0f, 125.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -25.0f, 180.0f);
Zone(TEXT("World s1animals, slot 45"), 2.01377f, 2.0581f);
	Scatter(S1ANIMALS("Cuttlefish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Cuttlefish"), 76.5f, 78.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 0.0f);
	Far(S1ANIMALS("BaitBall"), 100.0f, 125.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -25.0f, 180.0f);
Zone(TEXT("World s1animals, slot 47"), 2.10425f, 2.14859f);
	Scatter(S1ANIMALS("Cuttlefish"), 98.5f, 100.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Cuttlefish"), 102.5f, 104.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 49"), 2.19474f, 2.23908f);
	Scatter(S1ANIMALS("Cuttlefish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Cuttlefish"), 76.5f, 78.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 51"), 2.28522f, 2.32956f);
	Far(S1ANIMALS("BaitBall"), 100.0f, 125.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -25.0f, 180.0f);
Zone(TEXT("World s1animals, slot 61"), 2.73765f, 2.78199f);
	Far(S1ANIMALS("BaitBall"), 100.0f, 125.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -25.0f, 180.0f);
Zone(TEXT("World s1animals, slot 67"), 3.00911f, 3.05345f);
	Scatter(S1ANIMALS("Cuttlefish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Cuttlefish"), 76.5f, 78.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 0.0f);
	Far(S1ANIMALS("BaitBall"), 100.0f, 125.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -25.0f, 180.0f);
Zone(TEXT("World s1animals, slot 69"), 3.0996f, 3.14394f);
	Scatter(S1ANIMALS("SeaSlugs"), 35.0f, 36.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Cuttlefish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Cuttlefish"), 76.5f, 78.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 73"), 3.28057f, 3.32491f);
	Far(S1ANIMALS("BaitBall"), 100.0f, 125.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -25.0f, 180.0f);
Zone(TEXT("World s1animals, slot 85"), 3.82349f, 3.86782f);
	Scatter(S1ANIMALS("Puffers"), 102.5f, 104.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Puffers"), 106.5f, 108.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 87"), 3.91397f, 3.95831f);
	Scatter(S1ANIMALS("Puffers"), 102.5f, 104.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Puffers"), 106.5f, 108.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 89"), 4.00446f, 4.0488f);
	Scatter(S1ANIMALS("Puffers"), 102.5f, 104.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Puffers"), 106.5f, 108.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 91"), 4.09494f, 4.13928f);
	Scatter(S1ANIMALS("Puffers"), 102.5f, 104.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Puffers"), 106.5f, 108.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 93"), 4.18543f, 4.22977f);
	Scatter(S1ANIMALS("Puffers"), 102.5f, 104.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Puffers"), 106.5f, 108.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 95"), 4.27592f, 4.32025f);
	Scatter(S1ANIMALS("Puffers"), 102.5f, 104.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Puffers"), 106.5f, 108.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 97"), 4.3664f, 4.41074f);
	Scatter(S1ANIMALS("Puffers"), 102.5f, 104.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Puffers"), 106.5f, 108.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 99"), 4.45689f, 4.50123f);
	Scatter(S1ANIMALS("Puffers"), 102.5f, 104.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Puffers"), 106.5f, 108.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 111"), 4.9998f, 5.04414f);
	Scatter(S1ANIMALS("SpiderCrabs"), 78.0f, 80.0f, 1, 0, 1.f, 1.4f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("SpiderCrabs"), 108.0f, 110.0f, 1, 0, 1.f, 1.4f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 112"), 5.04505f, 5.08939f);
	Far(S1ANIMALS("BaitBall"), 100.0f, 125.0f, 1, 0, 1.f, 1.3f, 0.f, -1, -25.0f, 180.0f);
Zone(TEXT("World s1animals, slot 113"), 5.09029f, 5.13463f);
	Scatter(S1ANIMALS("SpiderCrabs"), 98.0f, 100.0f, 1, 0, 1.f, 1.4f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 115"), 5.18078f, 5.22511f);
	Scatter(S1ANIMALS("SpiderCrabs"), 94.0f, 96.0f, 1, 0, 1.f, 1.4f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 117"), 5.27126f, 5.3156f);
	Far(S1ANIMALS("BaitBall"), 100.0f, 125.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -25.0f, 180.0f);
Zone(TEXT("World s1animals, slot 131"), 5.90466f, 5.949f);
	Far(S1ANIMALS("BaitBall"), 100.0f, 125.0f, 1, 0, 1.f, 1.3f, 0.f, -1, -25.0f, 180.0f);
Zone(TEXT("World s1animals, slot 133"), 5.99515f, 6.03949f);
	Scatter(S1ANIMALS("SpiderCrabs"), 98.0f, 100.0f, 1, 0, 1.f, 1.4f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 135"), 6.08564f, 6.12997f);
	Scatter(S1ANIMALS("SpiderCrabs"), 94.0f, 96.0f, 1, 0, 1.f, 1.4f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 136"), 6.13088f, 6.17522f);
	Far(S1ANIMALS("BaitBall"), 100.0f, 125.0f, 1, 0, 1.f, 1.3f, 0.f, -1, -25.0f, 180.0f);
Zone(TEXT("World s1animals, slot 142"), 6.40234f, 6.44668f);
	Far(S1ANIMALS("BaitBall"), 100.0f, 125.0f, 1, 0, 1.f, 1.3f, 0.f, -1, -25.0f, 180.0f);
Zone(TEXT("World s1animals, slot 149"), 6.71904f, 6.76338f);
	Far(S1ANIMALS("BaitBall"), 100.0f, 125.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -25.0f, 180.0f);
Zone(TEXT("World s1animals, slot 157"), 7.08098f, 7.12532f);
	Scatter(S1ANIMALS("SpiderCrabs"), 80.0f, 82.0f, 1, 0, 1.f, 1.4f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("SeaPigs"), 42.0f, 43.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
	Scatter(S1ANIMALS("SpiderCrabs"), 72.0f, 74.0f, 1, 0, 1.f, 1.4f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 159"), 7.17147f, 7.21581f);
	Scatter(S1ANIMALS("PigKeeper"), 61.0f, 62.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("SpiderCrabs"), 74.0f, 76.0f, 1, 0, 1.f, 1.4f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("SpiderCrabs"), 76.0f, 78.0f, 1, 0, 1.f, 1.4f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 161"), 7.26195f, 7.30629f);
	Scatter(S1ANIMALS("SpiderCrabs"), 80.0f, 82.0f, 1, 0, 1.f, 1.4f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("PigKeeper"), 46.0f, 47.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 6.0f);
	Scatter(S1ANIMALS("SpiderCrabs"), 72.0f, 74.0f, 1, 0, 1.f, 1.4f, 0.f, -1, 0.0f);
	Far(S1ANIMALS("BaitBall"), 100.0f, 125.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -25.0f, 180.0f);
Zone(TEXT("World s1animals, slot 163"), 7.35244f, 7.39678f);
	Scatter(S1ANIMALS("SeaPigs"), 61.0f, 62.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("SpiderCrabs"), 74.0f, 76.0f, 1, 0, 1.f, 1.4f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("SpiderCrabs"), 76.0f, 78.0f, 1, 0, 1.f, 1.4f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 165"), 7.44293f, 7.48726f);
	Scatter(S1ANIMALS("SpiderCrabs"), 80.0f, 82.0f, 1, 0, 1.f, 1.4f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("SeaPigs"), 42.0f, 43.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
	Scatter(S1ANIMALS("SpiderCrabs"), 72.0f, 74.0f, 1, 0, 1.f, 1.4f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 173"), 7.80487f, 7.84921f);
	Far(S1ANIMALS("BaitBall"), 100.0f, 125.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -25.0f, 180.0f);
Zone(TEXT("World s1animals, slot 175"), 7.89536f, 7.93969f);
	Scatter(S1ANIMALS("PigKeeper"), 61.0f, 62.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("SpiderCrabs"), 84.0f, 86.0f, 1, 0, 1.f, 1.4f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 177"), 7.98584f, 8.03018f);
	Scatter(S1ANIMALS("HermitCrabs"), 55.0f, 56.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 0.0f);
	Scatter(S1ANIMALS("Seals"), 86.5f, 88.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 179"), 8.07633f, 8.12067f);
	Scatter(S1ANIMALS("Seals"), 82.5f, 84.5f, 1, 0, 1.f, 1.2f, 0.f, 1, 0.0f);
	Far(S1ANIMALS("Sunfish"), 112.0f, 130.0f, 1, 0, 1.f, 1.6f, 0.f, 1, -22.0f, 0.0f);
Zone(TEXT("World s1animals, slot 181"), 8.16681f, 8.21115f);
	Scatter(S1ANIMALS("Starfish"), 37.0f, 38.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("ParrotFish"), 59.5f, 60.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 10.0f);
Zone(TEXT("World s1animals, slot 185"), 8.34779f, 8.39212f);
	Scatter(S1ANIMALS("Seals"), 82.5f, 84.5f, 1, 0, 1.f, 1.2f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 187"), 8.43827f, 8.48261f);
	Scatter(S1ANIMALS("ParrotFish"), 65.5f, 66.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Seals"), 86.5f, 88.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 189"), 8.52876f, 8.5731f);
	Scatter(S1ANIMALS("HermitCrabs"), 55.0f, 56.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 191"), 8.61924f, 8.66358f);
	Scatter(S1ANIMALS("Seals"), 82.5f, 84.5f, 1, 0, 1.f, 1.2f, 0.f, 1, 0.0f);
	Far(S1ANIMALS("Sunfish"), 112.0f, 130.0f, 1, 0, 1.f, 1.6f, 0.f, 1, -22.0f, 0.0f);
Zone(TEXT("World s1animals, slot 193"), 8.70973f, 8.75407f);
	Scatter(S1ANIMALS("Seals"), 86.5f, 88.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 195"), 8.80022f, 8.84455f);
	Scatter(S1ANIMALS("Seals"), 86.5f, 88.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 197"), 8.8907f, 8.93504f);
	Scatter(S1ANIMALS("Seals"), 82.5f, 84.5f, 1, 0, 1.f, 1.2f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Starfish"), 41.0f, 42.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 199"), 8.98119f, 9.02553f);
	Scatter(S1ANIMALS("ParrotFish"), 65.5f, 66.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 10.0f);
Zone(TEXT("World s1animals, slot 201"), 9.07167f, 9.11601f);
	Scatter(S1ANIMALS("Seals"), 102.5f, 104.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 203"), 9.16216f, 9.2065f);
	Scatter(S1ANIMALS("Seals"), 86.5f, 88.5f, 1, 0, 1.f, 1.2f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 205"), 9.25265f, 9.29698f);
	Scatter(S1ANIMALS("HermitCrabs"), 60.0f, 61.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 209"), 9.43362f, 9.47796f);
	Scatter(S1ANIMALS("Seals"), 82.5f, 84.5f, 1, 0, 1.f, 1.2f, 0.f, 1, 0.0f);
	Far(S1ANIMALS("Sunfish"), 112.0f, 130.0f, 1, 0, 1.f, 1.6f, 0.f, 1, -22.0f, 0.0f);
Zone(TEXT("World s1animals, slot 211"), 9.5241f, 9.56844f);
	Scatter(S1ANIMALS("Seals"), 86.5f, 88.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 213"), 9.61459f, 9.65893f);
	Scatter(S1ANIMALS("HermitCrabs"), 55.0f, 56.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 215"), 9.70508f, 9.74941f);
	Scatter(S1ANIMALS("Seals"), 82.5f, 84.5f, 1, 0, 1.f, 1.2f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 217"), 9.79556f, 9.8399f);
	Scatter(S1ANIMALS("ParrotFish"), 59.5f, 60.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Seals"), 86.5f, 88.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 219"), 9.88605f, 9.93039f);
	Scatter(S1ANIMALS("Seals"), 102.5f, 104.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 221"), 9.97653f, 10.02087f);
	Scatter(S1ANIMALS("Seals"), 82.5f, 84.5f, 1, 0, 1.f, 1.2f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Starfish"), 41.0f, 42.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 223"), 10.06702f, 10.11136f);
	Scatter(S1ANIMALS("ParrotFish"), 65.5f, 66.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 10.0f);
Zone(TEXT("World s1animals, slot 225"), 10.15751f, 10.20184f);
	Scatter(S1ANIMALS("HermitCrabs"), 55.0f, 56.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 227"), 10.24799f, 10.29233f);
	Far(S1ANIMALS("Sunfish"), 112.0f, 130.0f, 1, 0, 1.f, 1.6f, 0.f, 1, -22.0f, 0.0f);
Zone(TEXT("World s1animals, slot 229"), 10.33848f, 10.38282f);
	Scatter(S1ANIMALS("Starfish"), 37.0f, 38.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
Zone(TEXT("World s1animals, slot 233"), 10.51945f, 10.56379f);
	Far(S1ANIMALS("Sunfish"), 112.0f, 130.0f, 1, 0, 1.f, 1.6f, 0.f, 1, -22.0f, 0.0f);
Zone(TEXT("World s1animals, slot 235"), 10.60994f, 10.65427f);
	Scatter(S1ANIMALS("ParrotFish"), 65.5f, 66.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 10.0f);
Zone(TEXT("World s1animals, slot 241"), 10.88139f, 10.92573f);
	Scatter(S1ANIMALS("HermitCrabs"), 58.0f, 59.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 245"), 11.06237f, 11.1067f);
	Scatter(S1ANIMALS("Starfish"), 41.0f, 42.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 249"), 11.24334f, 11.28768f);
	Scatter(S1ANIMALS("HermitCrabs"), 55.0f, 56.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 253"), 11.42431f, 11.46865f);
	Scatter(S1ANIMALS("ParrotFish"), 59.5f, 60.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 10.0f);
Zone(TEXT("World s1animals, slot 259"), 11.69577f, 11.74011f);
	Scatter(S1ANIMALS("ParrotFish"), 65.5f, 66.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 10.0f);
Zone(TEXT("World s1animals, slot 261"), 11.78625f, 11.83059f);
	Scatter(S1ANIMALS("HermitCrabs"), 55.0f, 56.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 263"), 11.87674f, 11.92108f);
	Far(S1ANIMALS("Sunfish"), 112.0f, 130.0f, 1, 0, 1.f, 1.6f, 0.f, 1, -22.0f, 0.0f);
Zone(TEXT("World s1animals, slot 269"), 12.1482f, 12.19254f);
	Scatter(S1ANIMALS("Starfish"), 41.0f, 42.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 271"), 12.23868f, 12.28302f);
	Scatter(S1ANIMALS("Shrimps"), 64.5f, 65.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 8.0f);
Zone(TEXT("World s1animals, slot 273"), 12.32917f, 12.37351f);
	Scatter(S1ANIMALS("Cuttlefish"), 98.5f, 100.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 274"), 12.37441f, 12.41875f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, -1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 275"), 12.41966f, 12.46399f);
	Scatter(S1ANIMALS("Cuttlefish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 277"), 12.51014f, 12.55448f);
	Scatter(S1ANIMALS("Shrimps"), 37.0f, 38.0f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Shrimps"), 58.5f, 59.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 8.0f);
Zone(TEXT("World s1animals, slot 281"), 12.69111f, 12.73545f);
	Scatter(S1ANIMALS("Cuttlefish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 283"), 12.7816f, 12.82594f);
	Scatter(S1ANIMALS("Cuttlefish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 0.0f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 287"), 12.96257f, 13.00691f);
	Scatter(S1ANIMALS("Cuttlefish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 289"), 13.05306f, 13.0974f);
	Scatter(S1ANIMALS("PigKeeper"), 60.5f, 61.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Cuttlefish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 0.0f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 291"), 13.14354f, 13.18788f);
	Scatter(S1ANIMALS("Cuttlefish"), 98.5f, 100.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 293"), 13.23403f, 13.27837f);
	Scatter(S1ANIMALS("Cuttlefish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Shrimps"), 41.0f, 42.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 295"), 13.32452f, 13.36885f);
	Scatter(S1ANIMALS("SeaPigs"), 64.5f, 65.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 297"), 13.415f, 13.45934f);
	Scatter(S1ANIMALS("Cuttlefish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 298"), 13.46024f, 13.50458f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, -1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 299"), 13.50549f, 13.54983f);
	Scatter(S1ANIMALS("Cuttlefish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 305"), 13.77695f, 13.82128f);
	Scatter(S1ANIMALS("Cuttlefish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 307"), 13.86743f, 13.91177f);
	Scatter(S1ANIMALS("PigKeeper"), 64.5f, 65.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
	Scatter(S1ANIMALS("Cuttlefish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 0.0f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 317"), 14.31986f, 14.3642f);
	Scatter(S1ANIMALS("Cuttlefish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Shrimps"), 41.0f, 42.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 321"), 14.50083f, 14.54517f);
	Scatter(S1ANIMALS("Cuttlefish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 322"), 14.54608f, 14.59042f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, -1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 323"), 14.59132f, 14.63566f);
	Scatter(S1ANIMALS("Cuttlefish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 325"), 14.68181f, 14.72614f);
	Scatter(S1ANIMALS("SeaPigs"), 40.5f, 41.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("PigKeeper"), 58.5f, 59.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
Zone(TEXT("World s1animals, slot 329"), 14.86278f, 14.90712f);
	Scatter(S1ANIMALS("Cuttlefish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 337"), 15.22472f, 15.26906f);
	Scatter(S1ANIMALS("Cuttlefish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 0.0f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 339"), 15.31521f, 15.35955f);
	Scatter(S1ANIMALS("Cuttlefish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 341"), 15.40569f, 15.45003f);
	Scatter(S1ANIMALS("Cuttlefish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Shrimps"), 41.0f, 42.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 8.0f);
Zone(TEXT("World s1animals, slot 343"), 15.49618f, 15.54052f);
	Scatter(S1ANIMALS("SeaPigs"), 64.5f, 65.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 345"), 15.58667f, 15.631f);
	Scatter(S1ANIMALS("Cuttlefish"), 98.5f, 100.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 346"), 15.63191f, 15.67625f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, -1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 347"), 15.67715f, 15.72149f);
	Scatter(S1ANIMALS("Cuttlefish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 349"), 15.76764f, 15.81198f);
	Scatter(S1ANIMALS("PigKeeper"), 58.5f, 59.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 6.0f);
Zone(TEXT("World s1animals, slot 359"), 16.22007f, 16.26441f);
	Scatter(S1ANIMALS("Lionfish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Far(S1ANIMALS("Sunfish"), 112.0f, 130.0f, 1, 0, 1.f, 1.6f, 0.f, 1, -22.0f, 0.0f);
Zone(TEXT("World s1animals, slot 361"), 16.31055f, 16.35489f);
	Scatter(S1ANIMALS("Flounders"), 58.5f, 59.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Lionfish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Seals"), 99.0f, 101.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 363"), 16.40104f, 16.44538f);
	Scatter(S1ANIMALS("Lionfish"), 98.5f, 100.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 365"), 16.49153f, 16.53586f);
	Scatter(S1ANIMALS("Lionfish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Flounders"), 41.0f, 42.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 367"), 16.58201f, 16.62635f);
	Scatter(S1ANIMALS("Flounders"), 64.5f, 65.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Seals"), 86.5f, 88.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 369"), 16.6725f, 16.71684f);
	Scatter(S1ANIMALS("Lionfish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Seals"), 99.0f, 101.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 371"), 16.76298f, 16.80732f);
	Scatter(S1ANIMALS("Lionfish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Seals"), 95.0f, 97.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 0.0f);
	Far(S1ANIMALS("Sunfish"), 112.0f, 130.0f, 1, 0, 1.f, 1.6f, 0.f, 1, -22.0f, 0.0f);
Zone(TEXT("World s1animals, slot 379"), 17.12493f, 17.16927f);
	Scatter(S1ANIMALS("Flounders"), 64.5f, 65.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Lionfish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Seals"), 99.0f, 101.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 381"), 17.21541f, 17.25975f);
	Scatter(S1ANIMALS("Seals"), 102.5f, 104.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 383"), 17.3059f, 17.35024f);
	Scatter(S1ANIMALS("Lionfish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
Zone(TEXT("World s1animals, slot 385"), 17.39639f, 17.44072f);
	Scatter(S1ANIMALS("Flounders"), 58.5f, 59.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Lionfish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Seals"), 99.0f, 101.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 387"), 17.48687f, 17.53121f);
	Scatter(S1ANIMALS("Lionfish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Seals"), 99.0f, 101.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 389"), 17.57736f, 17.6217f);
	Scatter(S1ANIMALS("Lionfish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Flounders"), 41.0f, 42.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 391"), 17.66784f, 17.71218f);
	Scatter(S1ANIMALS("Seals"), 86.5f, 88.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 401"), 18.12027f, 18.16461f);
	Scatter(S1ANIMALS("Lionfish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Seals"), 95.0f, 97.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 403"), 18.21076f, 18.2551f);
	Scatter(S1ANIMALS("Flounders"), 64.5f, 65.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Lionfish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Seals"), 99.0f, 101.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 405"), 18.30125f, 18.34558f);
	Scatter(S1ANIMALS("Seals"), 86.5f, 88.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 407"), 18.39173f, 18.43607f);
	Scatter(S1ANIMALS("Lionfish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
Zone(TEXT("World s1animals, slot 409"), 18.48222f, 18.52656f);
	Scatter(S1ANIMALS("Lionfish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 411"), 18.5727f, 18.61704f);
	Scatter(S1ANIMALS("Lionfish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 421"), 19.02513f, 19.06947f);
	Scatter(S1ANIMALS("Shrimps"), 40.5f, 41.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Flounders"), 58.5f, 59.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 425"), 19.20611f, 19.25044f);
	Scatter(S1ANIMALS("Lionfish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Far(S1ANIMALS("Sunfish"), 112.0f, 130.0f, 1, 0, 1.f, 1.6f, 0.f, 1, -22.0f, 0.0f);
Zone(TEXT("World s1animals, slot 427"), 19.29659f, 19.34093f);
	Scatter(S1ANIMALS("Lionfish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 431"), 19.47756f, 19.5219f);
	Scatter(S1ANIMALS("Lionfish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
Zone(TEXT("World s1animals, slot 433"), 19.56805f, 19.61239f);
	Scatter(S1ANIMALS("Shrimps"), 58.5f, 59.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Lionfish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 443"), 20.02048f, 20.06482f);
	Scatter(S1ANIMALS("Lionfish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Far(S1ANIMALS("Sunfish"), 112.0f, 130.0f, 1, 0, 1.f, 1.6f, 0.f, 1, -22.0f, 0.0f);
Zone(TEXT("World s1animals, slot 449"), 20.29194f, 20.33628f);
	Scatter(S1ANIMALS("Lionfish"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
Zone(TEXT("World s1animals, slot 451"), 20.38242f, 20.42676f);
	Scatter(S1ANIMALS("Flounders"), 64.5f, 65.5f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Lionfish"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 463"), 20.92534f, 20.96968f);
	Scatter(S1ANIMALS("Seals"), 86.5f, 88.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 465"), 21.01583f, 21.06016f);
	Scatter(S1ANIMALS("SeaSlugs"), 55.0f, 56.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 0.0f);
	Scatter(S1ANIMALS("Puffers"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Seals"), 99.0f, 101.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 466"), 21.06107f, 21.10541f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, -1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 467"), 21.10631f, 21.15065f);
	Scatter(S1ANIMALS("Puffers"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Seals"), 95.0f, 97.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 469"), 21.1968f, 21.24114f);
	Scatter(S1ANIMALS("SeaSlugs"), 40.5f, 41.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("SeaSlugs"), 58.0f, 59.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Seals"), 86.5f, 88.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 471"), 21.28728f, 21.33162f);
	Scatter(S1ANIMALS("Seals"), 102.5f, 104.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 473"), 21.37777f, 21.42211f);
	Scatter(S1ANIMALS("Puffers"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Seals"), 99.0f, 101.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 475"), 21.46826f, 21.51259f);
	Scatter(S1ANIMALS("SeaSlugs"), 64.0f, 65.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Puffers"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Seals"), 99.0f, 101.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 485"), 21.92069f, 21.96502f);
	Scatter(S1ANIMALS("Puffers"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("SeaSlugs"), 41.0f, 42.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 487"), 22.01117f, 22.05551f);
	Scatter(S1ANIMALS("SeaSlugs"), 64.0f, 65.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Seals"), 86.5f, 88.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 489"), 22.10166f, 22.146f);
	Scatter(S1ANIMALS("Puffers"), 98.5f, 100.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 490"), 22.1469f, 22.19124f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, -1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 491"), 22.19214f, 22.23648f);
	Scatter(S1ANIMALS("Puffers"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("Seals"), 95.0f, 97.0f, 1, 0, 1.f, 1.2f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 493"), 22.28263f, 22.32697f);
	Scatter(S1ANIMALS("SeaSlugs"), 58.0f, 59.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Seals"), 86.5f, 88.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 495"), 22.37312f, 22.41745f);
	Scatter(S1ANIMALS("Seals"), 86.5f, 88.5f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 505"), 22.82555f, 22.86988f);
	Scatter(S1ANIMALS("SeaSlugs"), 58.0f, 59.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Puffers"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
	Scatter(S1ANIMALS("Seals"), 99.0f, 101.0f, 1, 0, 1.f, 1.2f, 0.f, -1, 0.0f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 507"), 22.91603f, 22.96037f);
	Scatter(S1ANIMALS("Puffers"), 98.5f, 100.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 509"), 23.00652f, 23.05086f);
	Scatter(S1ANIMALS("Puffers"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("SeaSlugs"), 41.0f, 42.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 511"), 23.097f, 23.14134f);
	Scatter(S1ANIMALS("SeaSlugs"), 64.0f, 65.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 513"), 23.18749f, 23.23183f);
	Scatter(S1ANIMALS("SeaSlugs"), 55.0f, 56.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 0.0f);
	Scatter(S1ANIMALS("Puffers"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 514"), 23.23273f, 23.27707f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, -1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 515"), 23.27798f, 23.32231f);
	Scatter(S1ANIMALS("Puffers"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
Zone(TEXT("World s1animals, slot 517"), 23.36846f, 23.4128f);
	Scatter(S1ANIMALS("SeaSlugs"), 40.5f, 41.5f, 1, 0, 1.f, 1.3f, 0.f, 1, 0.0f);
Zone(TEXT("World s1animals, slot 524"), 23.68516f, 23.7295f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 527"), 23.82089f, 23.86523f);
	Scatter(S1ANIMALS("Puffers"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
Zone(TEXT("World s1animals, slot 529"), 23.91138f, 23.95572f);
	Scatter(S1ANIMALS("SeaSlugs"), 58.0f, 59.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 0.0f);
	Scatter(S1ANIMALS("Puffers"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 531"), 24.00186f, 24.0462f);
	Scatter(S1ANIMALS("Puffers"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 533"), 24.09235f, 24.13669f);
	Scatter(S1ANIMALS("Puffers"), 78.5f, 80.5f, 1, 0, 1.f, 1.6f, 0.f, 1, 10.0f);
	Scatter(S1ANIMALS("SeaSlugs"), 41.0f, 42.0f, 1, 0, 1.f, 1.3f, 0.f, -1, 0.0f);
Zone(TEXT("World s1animals, slot 537"), 24.27332f, 24.31766f);
	Scatter(S1ANIMALS("SeaSlugs"), 55.0f, 56.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 0.0f);
	Scatter(S1ANIMALS("Puffers"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
Zone(TEXT("World s1animals, slot 538"), 24.31856f, 24.3629f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, -1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 547"), 24.72575f, 24.77009f);
	Scatter(S1ANIMALS("Starfish"), 64.0f, 65.0f, 1, 0, 1.f, 1.45f, 0.f, 1, 8.0f);
	Scatter(S1ANIMALS("Puffers"), 82.5f, 84.5f, 1, 0, 1.f, 1.6f, 0.f, -1, 10.0f);
	Far(S1ANIMALS("SeaAngels"), 62.0f, 72.0f, 1, 0, 1.f, 1.3f, 0.f, 1, -30.0f, 0.0f);
Zone(TEXT("World s1animals, slot 549"), 24.81624f, 24.86058f);
	Scatter(S1ANIMALS("SeaSlugs"), 55.0f, 56.0f, 1, 0, 1.f, 1.45f, 0.f, -1, 0.0f);
#undef S1ANIMALS
// ---- END s1animals ----
}

{
// ---- BEGIN s1people (the people pass 3 left out: raised groups for the banked bends, the far sides, the tube's side and the empty stretches, ft_world_sea_s1people.py) ----
// 26 meshes, all made by the one make run (C:/ftwork/logs/L6s1people_make.log: 26 built, 0 failed). All TALL. Each brings its own raised floor, footed 3 m
// into the sand: P pieces floors 2.3 to 3.4 m as built, M pieces a second floor 5.8 to 6.2 m, T pieces a floor 6.2 m (the kelp cutters' mast: a nest 13.8 m).
// Placed at 1.4 (the cast's own size): the high floors stand 8.1 m and more over the sand, over the travel tube (hides 3.9 to 7.5 m at 42 m) and the weed's foot.
// Steady glow 0.08 on still parts only (lamps, lit ports, pearls: cream, pink, rose, magenta; none amber or mint); nothing blinks. No Lit, no Bank, no Far.
// Slots: s % 8 == 1 only: every Site is "every 8, phase 7" in a Zone that holds exactly one slot (slot s: (s - 0.45) to (s + 0.45) x 0.045243 km).
// Distances (anchor; every piece's box with its bounds' growth is 2.38 m or less either way, the deepest mesh 3.0 m as built):
//   26.7 m (box 24.3 to 29.1) on level sand where the replay shows it clear; 27 to 34.5 m where something stands nearer; 38.1 m (35.7 to 40.5) behind the
//   travel tube on level sand: only M and T pieces there; 42.5 m (40.1 to 44.9) beside every banked bend (the plan's list; the circuit's bends worked out from
//   FTData.cpp's Circuit sections against the table's tunnel slots, one slot of margin), at the jump's run-up (slot 121) and by a tunnel's mouth.
// Yaw 0 (front to the road), up 0 (the 3 m footing takes the slope). Sets are 2 to 4 pieces 11 to 16 m apart along the road.
// Where: a scratch replay of both tables (C:/ftwork/jobs/L6s1people/plan.py on the pass 3 fixer's occ.py / slotmap.py, every chance taken as 1, worst-case
// depths) gave each slot side the nearest anchor whose box overlaps no piece with a narrow range by more than 0.7 m; pieces with a range of 10 m or more
// (turtles, lift bags, the dressing's scattered coral, kelp and crates) are not dodged. 15 sites keep an overlap of 1.0 to 2.0 m by the replay (marked
// "may touch"). Left alone: the slots in tunnels, loops, corkscrews and jump gaps (17, 41, 57, 65, 73, 81, 105, 129, 145, 153, 169, 313, 353, 377, 417, 457,
// 481, 521), and 35 slot sides with no clear place (9 R, 25 L, 33 L, 49 R, 113 L, 137 L, 161 R, 193 R, 201 L, 217 R, 225 L, 241 R, 265 L, 265 R, 273 L, 289 R, 297 L, 329 R, 337 R, 345 L, 361 R, 369 L, 385 L, 385 R, 409 L, 409 R, 433 L, 433 R, 441 L, 473 R, 489 L, 505 R, 513 L, 529 R, 545 R).
// Estimate: 67 Sites, 224 parts tried (46 at 26.7 m, 30 at 27 to 34.5 m, 68 behind the tube at 38.1 or 42.5 m, 80 more at 42.5 m). By the brief's rule
// x 0.75 = about 168 placed; the left-out slots are already taken off, so up to 224.
// The most of one mesh: SM_S1PEOPLE_D5_Guards, 17 tried. The smallest x min of a TALL mesh: 26.7 m (its box from 24.3 m). NOT COMPILED, not seen in the game.
#define S1PEOPLE(n) TEXT("/Game/World/UnderTheSea/SM_S1PEOPLE_" n)
Set(TEXT("WldS1PEOPLE001L"), {
	{ S1PEOPLE("D1_Gardeners"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D1_PearlStair"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D1_Conch"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE001R"), {
	{ S1PEOPLE("D1_PearlStair"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D1_Conch"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D1_Gardeners"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE009L"), {
	{ S1PEOPLE("D1_Conch"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D1_Gardeners"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D1_PearlStair"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE025R"), {
	{ S1PEOPLE("D1_Gardeners"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D1_PearlStair"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D1_Conch"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE033R"), {
	{ S1PEOPLE("D1_PearlStair"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D1_Conch"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D1_Gardeners"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE049L"), {
	{ S1PEOPLE("D2_StiltDeck"), -8.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D2_CuttersMast"), 8.0f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE089L"), {
	{ S1PEOPLE("D3_DiverClass"), -8.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D3_RoofDeck"), 8.0f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE089R"), {
	{ S1PEOPLE("D3_RoofDeck"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE097L"), {
	{ S1PEOPLE("D3_RoofDeck"), -8.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D3_DiverClass"), 8.0f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE097R"), {
	{ S1PEOPLE("D3_RoofDeck"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE113R"), {
	{ S1PEOPLE("D4_HullDeck"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D4_Forge"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D4_BridgeHouse"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE121L"), {
	{ S1PEOPLE("D4_Forge"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D4_BridgeHouse"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D4_HullDeck"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE121R"), {
	{ S1PEOPLE("D4_BridgeHouse"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D4_HullDeck"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D4_Forge"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE137R"), {
	{ S1PEOPLE("D4_HullDeck"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D4_Forge"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D4_BridgeHouse"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE161L"), {
	{ S1PEOPLE("D4_Forge"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D4_BridgeHouse"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D4_HullDeck"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE177L"), {
	{ S1PEOPLE("D5_Guards"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Arcade"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_GrandStair"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE177R"), {
	{ S1PEOPLE("D5_TempleSteps"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Guards"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Arcade"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_GrandStair"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE185L"), {
	{ S1PEOPLE("D5_Arcade"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_GrandStair"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Guards"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE185R"), {
	{ S1PEOPLE("D5_Guards"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Arcade"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_GrandStair"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_TempleSteps"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE193L"), {
	{ S1PEOPLE("D5_GrandStair"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Guards"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Arcade"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE201R"), {
	{ S1PEOPLE("D5_Arcade"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_GrandStair"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_TempleSteps"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Guards"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE209L"), {
	{ S1PEOPLE("D5_Guards"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Arcade"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_GrandStair"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE209R"), {
	{ S1PEOPLE("D5_GrandStair"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_TempleSteps"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Guards"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Arcade"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE217L"), {
	{ S1PEOPLE("D5_TempleSteps"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Guards"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Arcade"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_GrandStair"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE225R"), {
	{ S1PEOPLE("D5_Guards"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Arcade"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_GrandStair"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_TempleSteps"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE233L"), {
	{ S1PEOPLE("D5_Arcade"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_GrandStair"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_TempleSteps"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Guards"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE233R"), {
	{ S1PEOPLE("D5_GrandStair"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_TempleSteps"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Guards"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Arcade"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE241L"), {
	{ S1PEOPLE("D5_Arcade"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_GrandStair"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Guards"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE249L"), {
	{ S1PEOPLE("D5_GrandStair"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Guards"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Arcade"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE249R"), {
	{ S1PEOPLE("D5_TempleSteps"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Guards"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Arcade"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_GrandStair"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE257L"), {
	{ S1PEOPLE("D5_Guards"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Arcade"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_GrandStair"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE257R"), {
	{ S1PEOPLE("D5_Guards"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_Arcade"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_GrandStair"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D5_TempleSteps"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE273R"), {
	{ S1PEOPLE("D6_BlowDeck"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_FloatStairs"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_BubbleTerrace"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_Blowpipe"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE281L"), {
	{ S1PEOPLE("D6_FloatStairs"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_BubbleTerrace"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_Blowpipe"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE281R"), {
	{ S1PEOPLE("D6_FloatStairs"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_BubbleTerrace"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_Blowpipe"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_BlowDeck"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE289L"), {
	{ S1PEOPLE("D6_BubbleTerrace"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_Blowpipe"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_BlowDeck"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_FloatStairs"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE297R"), {
	{ S1PEOPLE("D6_Blowpipe"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_BlowDeck"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_FloatStairs"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_BubbleTerrace"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE305L"), {
	{ S1PEOPLE("D6_BubbleTerrace"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_Blowpipe"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_FloatStairs"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE305R"), {
	{ S1PEOPLE("D6_BlowDeck"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_FloatStairs"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_BubbleTerrace"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_Blowpipe"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE321L"), {
	{ S1PEOPLE("D6_Blowpipe"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_FloatStairs"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_BubbleTerrace"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE321R"), {
	{ S1PEOPLE("D6_FloatStairs"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_BubbleTerrace"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_Blowpipe"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_BlowDeck"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE329L"), {
	{ S1PEOPLE("D6_FloatStairs"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_BubbleTerrace"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_Blowpipe"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE337L"), {
	{ S1PEOPLE("D6_BubbleTerrace"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_Blowpipe"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_FloatStairs"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE345R"), {
	{ S1PEOPLE("D6_BubbleTerrace"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_Blowpipe"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_BlowDeck"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D6_FloatStairs"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE361L"), {
	{ S1PEOPLE("D7_BellTower"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_Keepers"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_PilotHouse"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE369R"), {
	{ S1PEOPLE("D7_Pilots"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_BellTower"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_Keepers"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_PilotHouse"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE393L"), {
	{ S1PEOPLE("D7_Keepers"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_PilotHouse"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_BellTower"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE393R"), {
	{ S1PEOPLE("D7_BellTower"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_Keepers"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_PilotHouse"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_Pilots"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE401L"), {
	{ S1PEOPLE("D7_Keepers"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_PilotHouse"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_Pilots"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_BellTower"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE401R"), {
	{ S1PEOPLE("D7_PilotHouse"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_Pilots"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_BellTower"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_Keepers"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE425L"), {
	{ S1PEOPLE("D7_PilotHouse"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_BellTower"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_Keepers"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE425R"), {
	{ S1PEOPLE("D7_Pilots"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_BellTower"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_Keepers"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_PilotHouse"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE441R"), {
	{ S1PEOPLE("D7_BellTower"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_Keepers"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_PilotHouse"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_Pilots"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE449L"), {
	{ S1PEOPLE("D7_BellTower"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_Keepers"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_PilotHouse"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE449R"), {
	{ S1PEOPLE("D7_Keepers"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_PilotHouse"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_Pilots"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D7_BellTower"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE465L"), {
	{ S1PEOPLE("D8_Rotunda"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Turret"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_ArchBridge"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE465R"), {
	{ S1PEOPLE("D8_Terrace"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Rotunda"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Turret"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_ArchBridge"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE473L"), {
	{ S1PEOPLE("D8_Turret"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_ArchBridge"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Rotunda"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE489R"), {
	{ S1PEOPLE("D8_Rotunda"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Turret"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_ArchBridge"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Terrace"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE497L"), {
	{ S1PEOPLE("D8_ArchBridge"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Rotunda"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Turret"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE497R"), {
	{ S1PEOPLE("D8_Turret"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_ArchBridge"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Terrace"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Rotunda"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE505L"), {
	{ S1PEOPLE("D8_Rotunda"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Turret"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_ArchBridge"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE513R"), {
	{ S1PEOPLE("D8_ArchBridge"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Terrace"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Rotunda"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Turret"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE529L"), {
	{ S1PEOPLE("D8_Terrace"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Rotunda"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Turret"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_ArchBridge"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE537L"), {
	{ S1PEOPLE("D8_Turret"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_ArchBridge"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Rotunda"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE537R"), {
	{ S1PEOPLE("D8_Rotunda"), -16.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Turret"), -5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_ArchBridge"), 5.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Terrace"), 16.5f, 0.f, 0.f, 0.f, 1.4f },
});
Set(TEXT("WldS1PEOPLE545L"), {
	{ S1PEOPLE("D8_ArchBridge"), -13.5f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Rotunda"), 0.0f, 0.f, 0.f, 0.f, 1.4f },
	{ S1PEOPLE("D8_Turret"), 13.5f, 0.f, 0.f, 0.f, 1.4f },
});
Zone(TEXT("World s1people, slot 1, The reef"), 0.0249f, 0.0656f);     // 0.05 km, level
	Site(TEXT("WldS1PEOPLE001L"), -1, 32.1f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE001R"), 1, 34.5f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 9, The reef"), 0.3868f, 0.4275f);     // 0.41 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE009L"), -1, 42.5f, 8, 7, 1.f);     // may touch P3D_Spire (2.0 m by the replay)
Zone(TEXT("World s1people, slot 25, The reef"), 1.1107f, 1.1514f);     // 1.13 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE025R"), 1, 42.5f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 33, The reef"), 1.4727f, 1.5134f);     // 1.49 km, level
	Site(TEXT("WldS1PEOPLE033R"), 1, 42.5f, 8, 7, 1.f);     // may touch P3A_SchoolClass (1.8 m by the replay)
Zone(TEXT("World s1people, slot 49, The kelp forest"), 2.1965f, 2.2373f);     // 2.22 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE049L"), -1, 42.5f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 89, The research station"), 4.0063f, 4.0470f);     // 4.03 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE089L"), -1, 45.f, 8, 7, 1.f);     // may touch PressureHab (1.9 m by the replay); fix round: 2.5 m further out
	Site(TEXT("WldS1PEOPLE089R"), 1, 42.5f, 8, 7, 1.f);     // behind the tube
Zone(TEXT("World s1people, slot 97, The research station"), 4.3682f, 4.4089f);     // 4.39 km, level
	Site(TEXT("WldS1PEOPLE097L"), -1, 26.7f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE097R"), 1, 38.1f, 8, 7, 1.f);     // behind the tube
Zone(TEXT("World s1people, slot 113, The salvage field and the approach"), 5.0921f, 5.1328f);     // 5.11 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE113R"), 1, 42.5f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 121, The salvage field and the approach"), 5.4540f, 5.4948f);     // 5.47 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE121L"), -1, 42.5f, 8, 7, 1.f);     // may touch P3A_Commuters_A (1.4 m by the replay)
	Site(TEXT("WldS1PEOPLE121R"), 1, 42.5f, 8, 7, 1.f);     // may touch P3A_Commuters_B (1.0 m by the replay)
Zone(TEXT("World s1people, slot 137, The salvage field and the approach"), 6.1779f, 6.2187f);     // 6.20 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE137R"), 1, 42.5f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 161, The salvage field and the approach"), 7.2638f, 7.3045f);     // 7.28 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE161L"), -1, 42.5f, 8, 7, 1.f);     // may touch PressureHab (1.9 m by the replay); not moved in the fix round: the pig keeper of s1animals stands behind it
Zone(TEXT("World s1people, slot 177, Nereida, the Pearl Quarter"), 7.9877f, 8.0284f);     // 8.01 km, level
	Site(TEXT("WldS1PEOPLE177L"), -1, 38.1f, 8, 7, 1.f);     // behind the tube
	Site(TEXT("WldS1PEOPLE177R"), 1, 45.f, 8, 7, 1.f);     // may touch PressureHab (1.9 m by the replay); fix round: 2.5 m further out
Zone(TEXT("World s1people, slot 185, Nereida, the Pearl Quarter"), 8.3496f, 8.3903f);     // 8.37 km, level
	Site(TEXT("WldS1PEOPLE185L"), -1, 38.1f, 8, 7, 1.f);     // behind the tube
	Site(TEXT("WldS1PEOPLE185R"), 1, 34.5f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 193, Nereida, the Pearl Quarter"), 8.7115f, 8.7523f);     // 8.73 km, level
	Site(TEXT("WldS1PEOPLE193L"), -1, 38.1f, 8, 7, 1.f);     // behind the tube
Zone(TEXT("World s1people, slot 201, Nereida, the Pearl Quarter"), 9.0735f, 9.1142f);     // 9.09 km, level
	Site(TEXT("WldS1PEOPLE201R"), 1, 26.7f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 209, Nereida, the Pearl Quarter"), 9.4354f, 9.4761f);     // 9.46 km, level
	Site(TEXT("WldS1PEOPLE209L"), -1, 38.1f, 8, 7, 1.f);     // behind the tube
	Site(TEXT("WldS1PEOPLE209R"), 1, 34.5f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 217, Nereida, the Pearl Quarter"), 9.7974f, 9.8381f);     // 9.82 km, level
	Site(TEXT("WldS1PEOPLE217L"), -1, 26.7f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 225, Nereida, the Pearl Quarter"), 10.1593f, 10.2000f);     // 10.18 km, level
	Site(TEXT("WldS1PEOPLE225R"), 1, 26.7f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 233, Nereida, the Pearl Quarter"), 10.5213f, 10.5620f);     // 10.54 km, level
	Site(TEXT("WldS1PEOPLE233L"), -1, 26.7f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE233R"), 1, 34.5f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 241, Nereida, the Pearl Quarter"), 10.8832f, 10.9239f);     // 10.90 km, level
	Site(TEXT("WldS1PEOPLE241L"), -1, 38.1f, 8, 7, 1.f);     // behind the tube
Zone(TEXT("World s1people, slot 249, Nereida, the Pearl Quarter"), 11.2451f, 11.2859f);     // 11.27 km, level
	Site(TEXT("WldS1PEOPLE249L"), -1, 38.1f, 8, 7, 1.f);     // behind the tube
	Site(TEXT("WldS1PEOPLE249R"), 1, 26.7f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 257, Nereida, the Pearl Quarter"), 11.6071f, 11.6478f);     // 11.63 km, level
	Site(TEXT("WldS1PEOPLE257L"), -1, 38.1f, 8, 7, 1.f);     // behind the tube
	Site(TEXT("WldS1PEOPLE257R"), 1, 34.5f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 273, Nereida, the Bubble Works"), 12.3310f, 12.3717f);     // 12.35 km, level
	Site(TEXT("WldS1PEOPLE273R"), 1, 26.7f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 281, Nereida, the Bubble Works"), 12.6929f, 12.7336f);     // 12.71 km, level
	Site(TEXT("WldS1PEOPLE281L"), -1, 38.1f, 8, 7, 1.f);     // behind the tube
	Site(TEXT("WldS1PEOPLE281R"), 1, 34.5f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 289, Nereida, the Bubble Works"), 13.0549f, 13.0956f);     // 13.08 km, level
	Site(TEXT("WldS1PEOPLE289L"), -1, 26.7f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 297, Nereida, the Bubble Works"), 13.4168f, 13.4575f);     // 13.44 km, level
	Site(TEXT("WldS1PEOPLE297R"), 1, 26.7f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 305, Nereida, the Bubble Works"), 13.7788f, 13.8195f);     // 13.80 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE305L"), -1, 42.5f, 8, 7, 1.f);     // behind the tube
	Site(TEXT("WldS1PEOPLE305R"), 1, 42.5f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 321, Nereida, the Bubble Works"), 14.5026f, 14.5434f);     // 14.52 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE321L"), -1, 42.5f, 8, 7, 1.f);     // behind the tube
	Site(TEXT("WldS1PEOPLE321R"), 1, 45.f, 8, 7, 1.f);     // may touch PressureHab (1.9 m by the replay); fix round: 2.5 m further out
Zone(TEXT("World s1people, slot 329, Nereida, the Bubble Works"), 14.8646f, 14.9053f);     // 14.88 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE329L"), -1, 42.5f, 8, 7, 1.f);     // behind the tube
Zone(TEXT("World s1people, slot 337, Nereida, the Bubble Works"), 15.2265f, 15.2673f);     // 15.25 km, level
	Site(TEXT("WldS1PEOPLE337L"), -1, 38.1f, 8, 7, 1.f);     // behind the tube
Zone(TEXT("World s1people, slot 345, Nereida, the Bubble Works"), 15.5885f, 15.6292f);     // 15.61 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE345R"), 1, 45.f, 8, 7, 1.f);     // may touch PressureHab (1.9 m by the replay); fix round: 2.5 m further out
Zone(TEXT("World s1people, slot 361, Nereida, Lamp Row"), 16.3124f, 16.3531f);     // 16.33 km, level
	Site(TEXT("WldS1PEOPLE361L"), -1, 38.1f, 8, 7, 1.f);     // behind the tube
Zone(TEXT("World s1people, slot 369, Nereida, Lamp Row"), 16.6743f, 16.7150f);     // 16.69 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE369R"), 1, 45.f, 8, 7, 1.f);     // may touch PressureHab (1.9 m by the replay); fix round: 2.5 m further out
Zone(TEXT("World s1people, slot 393, Nereida, Lamp Row"), 17.7601f, 17.8009f);     // 17.78 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE393L"), -1, 42.5f, 8, 7, 1.f);     // behind the tube
	Site(TEXT("WldS1PEOPLE393R"), 1, 45.f, 8, 7, 1.f);     // may touch PressureHab (1.9 m by the replay); fix round: 2.5 m further out
Zone(TEXT("World s1people, slot 401, Nereida, Lamp Row"), 18.1221f, 18.1628f);     // 18.14 km, level
	Site(TEXT("WldS1PEOPLE401L"), -1, 26.7f, 8, 7, 1.f);
	Site(TEXT("WldS1PEOPLE401R"), 1, 34.5f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 425, Nereida, Lamp Row"), 19.2079f, 19.2486f);     // 19.23 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE425L"), -1, 42.5f, 8, 7, 1.f);     // behind the tube
	Site(TEXT("WldS1PEOPLE425R"), 1, 42.5f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 441, Nereida, Lamp Row"), 19.9318f, 19.9725f);     // 19.95 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE441R"), 1, 45.f, 8, 7, 1.f);     // may touch PressureHab (1.9 m by the replay); fix round: 2.5 m further out
Zone(TEXT("World s1people, slot 449, Nereida, Lamp Row"), 20.2937f, 20.3345f);     // 20.31 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE449L"), -1, 42.5f, 8, 7, 1.f);     // behind the tube
	Site(TEXT("WldS1PEOPLE449R"), 1, 42.5f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 465, Nereida, the Crown"), 21.0176f, 21.0584f);     // 21.04 km, level
	Site(TEXT("WldS1PEOPLE465L"), -1, 38.1f, 8, 7, 1.f);     // behind the tube
	Site(TEXT("WldS1PEOPLE465R"), 1, 26.7f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 473, Nereida, the Crown"), 21.3796f, 21.4203f);     // 21.40 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE473L"), -1, 42.5f, 8, 7, 1.f);     // behind the tube
Zone(TEXT("World s1people, slot 489, Nereida, the Crown"), 22.1035f, 22.1442f);     // 22.12 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE489R"), 1, 45.f, 8, 7, 1.f);     // may touch PressureHab (1.9 m by the replay); fix round: 2.5 m further out
Zone(TEXT("World s1people, slot 497, Nereida, the Crown"), 22.4654f, 22.5061f);     // 22.49 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE497L"), -1, 42.5f, 8, 7, 1.f);     // behind the tube
	Site(TEXT("WldS1PEOPLE497R"), 1, 42.5f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 505, Nereida, the Crown"), 22.8274f, 22.8681f);     // 22.85 km, level
	Site(TEXT("WldS1PEOPLE505L"), -1, 38.1f, 8, 7, 1.f);     // behind the tube
Zone(TEXT("World s1people, slot 513, Nereida, the Crown"), 23.1893f, 23.2300f);     // 23.21 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE513R"), 1, 45.f, 8, 7, 1.f);     // may touch PressureHab (1.9 m by the replay); fix round: 2.5 m further out
Zone(TEXT("World s1people, slot 529, Nereida, the Crown"), 23.9132f, 23.9539f);     // 23.93 km, level
	Site(TEXT("WldS1PEOPLE529L"), -1, 26.7f, 8, 7, 1.f);
Zone(TEXT("World s1people, slot 537, Nereida, the Crown"), 24.2751f, 24.3159f);     // 24.30 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE537L"), -1, 42.5f, 8, 7, 1.f);     // behind the tube
	Site(TEXT("WldS1PEOPLE537R"), 1, 45.f, 8, 7, 1.f);     // may touch PressureHab (1.9 m by the replay); fix round: 2.5 m further out
Zone(TEXT("World s1people, slot 545, Nereida, the Crown"), 24.6371f, 24.6778f);     // 24.66 km, banked bend, jump run-up or tunnel mouth: 40 m and out
	Site(TEXT("WldS1PEOPLE545L"), -1, 42.5f, 8, 7, 1.f);     // behind the tube
#undef S1PEOPLE
// ---- END s1people ----
}

{
// ---- BEGIN s1fix (Stage 1, the review's fix round: the rules that had to be ADDED. They stand at the very end so that no earlier rule re-rolls) ----
// 1. s1d1 "The reef": the verge (20.3 m) and the lane (25.9 m) after the banked S-bend of slots 22-29: slots 30 to 36. The same nine rules as
//    in the s1d1 block, whose two Zones end at slot 21 now. Sets WldS1D1Verge*/Lane* are s1d1's.
// 2. s1d8 "the Crown": the walks by the road on the straights 483-488, 504-509, 524-530 and 545-550 (the first straight, 462-467, is in the
//    s1d8 block). Nothing of them stands beside the circuit's banked bends any more. Set WldS1D8Walk is s1d8's.
// Every piece here is LOW from 20.3 m or TALL from 25 m; steady glow only from 25 m (LampTree) and 25.9 m (CoralLamp).
#define S1D8(n) TEXT("/Game/World/UnderTheSea/SM_S1D8_" n)
Zone(TEXT("World s1d1, The reef: the verge after the banked bend"), 1.355f, 1.7f);    // slots 30 to 36
	Site(TEXT("WldS1D1VergeA"), -1, 20.3f, 4, 0, 1.f);      // left, s % 4 == 0: slots 32, 36
	Site(TEXT("WldS1D1VergeB"), 1, 20.3f, 4, 0, 1.f);       // right, s % 4 == 0
	Site(TEXT("WldS1D1VergeB"), -1, 20.3f, 4, 2, 1.f);      // left, s % 4 == 2: slots 30, 34
	Site(TEXT("WldS1D1VergeA"), 1, 20.3f, 4, 2, 1.f);       // right, s % 4 == 2
Zone(TEXT("World s1d1, The reef: the lane after the banked bend"), 1.355f, 1.7f);     // slots 30 to 36
	Site(TEXT("WldS1D1LaneA"), -1, 25.9f, 4, 0, 1.f);       // left, s % 4 == 0: slots 32, 36
	Site(TEXT("WldS1D1LaneB"), -1, 25.9f, 4, 2, 1.f);       // left, s % 4 == 2: slots 30, 34
	Site(TEXT("WldS1D1LaneA"), 1, 25.9f, 4, 2, 1.f);        // right, s % 4 == 2: slots 30, 34
	Site(TEXT("WldS1D1LaneB"), 1, 25.9f, 12, 0, 1.f);       // right, s % 12 == 0: slot 36
	Site(TEXT("WldS1D1LaneB"), 1, 25.9f, 12, 8, 1.f);       // right, s % 12 == 4: none in 30 to 36
auto S1D8Straight = [&](float A, float B, int32 WalkR)   // the walks of the Crown: only where the road is not banked. WalkR: the phase of the right walk (every 8)
{
	Zone(TEXT("World s1d8, the Crown: the walks, a straight"), (A - 0.5f) * 0.045243f, (B + 0.5f) * 0.045243f);
	// the walks by the road, left
	Scatter(S1D8("HedgeGate"), 22.6f, 24.4f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);              // low, s % 8 == 0
	Scatter(S1D8("PennantMast"), 26.5f, 27.8f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);            // behind the hedge
	Scatter(S1D8("Parterre"), 24.5f, 25.2f, 16, 12, 1.f, 1.f, 0.f, -1, 0.f);             // low, s % 16 == 4
	Scatter(S1D8("SeahorseTopiary"), 28.4f, 29.2f, 16, 12, 1.f, 1.f, 0.f, -1, 6.f);      // behind the parterre
	Scatter(S1D8("BubbleFountain"), 26.5f, 27.3f, 16, 4, 1.f, 1.f, 0.f, -1, 0.f);        // s % 16 == 12
	Site(TEXT("WldS1D8Walk"), -1, 25.2f, 12, 10, 1.f);                                   // s % 12 == 2
	Scatter(S1D8("BubbleFountain"), 26.5f, 27.f, 24, 14, 1.f, 1.f, 0.f, -1, 0.f);        // s % 24 == 10
	Scatter(S1D8("SeahorseTopiary"), 25.f, 25.8f, 24, 2, 1.f, 1.f, 0.f, -1, 6.f);        // s % 24 == 22
	// the walks by the road, right
	Scatter(S1D8("HedgeGate"), 21.8f, 23.4f, 8, 2, 1.f, 1.f, 0.f, 1, 0.f);               // low, s % 8 == 6
	Scatter(S1D8("PennantMast"), 26.6f, 28.f, 8, 2, 1.f, 1.f, 0.f, 1, 0.f);              // behind the hedge
	Scatter(S1D8("Parterre"), 23.2f, 24.2f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);                // low, s % 8 == 2
	Site(TEXT("WldS1D8Walk"), 1, 25.f, 8, WalkR, 1.f);                                   // s % 8 == 4 (slots 484, 508), or s % 8 == 6 (slots 526, 550) where the patrol and the welders of the older tables have 24 to 29 m in the s % 8 == 4 slot
	Scatter(S1D8("BubbleFountain"), 26.5f, 27.3f, 16, 0, 1.f, 1.f, 0.f, 1, 0.f);         // s % 16 == 0
	Scatter(S1D8("SeahorseTopiary"), 25.f, 25.8f, 16, 8, 1.f, 1.f, 0.f, 1, 6.f);         // s % 16 == 8
};
S1D8Straight(483.f, 488.f, 4); S1D8Straight(504.f, 509.f, 4); S1D8Straight(524.f, 530.f, 2); S1D8Straight(545.f, 550.f, 2);
#undef S1D8
// ---- END s1fix ----
}
