// Chemical Works (level index 1): the population layer. People, creatures and film crews, placed by rule like the dressing
// (the words and what their numbers mean are in the header of FTTrackDress.cpp; the rule kinds in FTDress.h).
// This table is read after FTDressL1.inl: start with a Zone line, and every rule here comes after every dressing
// rule, so the dressing's own placements never move. A mesh name without a leading slash is looked up in
// /Game/Maps/ChemicalWorks/Dress/; give a full path for the population's own meshes, e.g. TEXT("/Game/People/SM_CrowdCheerA").
// Rules that bind this layer: nothing over 2 m tall nearer than 24 m to a centre line and nothing at all nearer than
// 17 m; nothing lit that blinks nearer than 30 m; nothing that could be taken for a pickup, an enemy or an attack.
//
// THE NIGHT SHIFT (first round, 15 Nov 2026). Meshes: Content/Python/ft_people_chem.py -> /Game/People/ChemicalWorks/;
// the film crew: /Game/People/Crew/SM_Crew<Piece>_Work. How this road lies (FTDressL1.inl, third pass): the kerb and its
// stores fill 17.3 to 23 m on both sides of every slot; a pipe rack (26.5 m: 24.8 to 28.2, 5 to 7.6 m tall) or a
// tank row (28.5 m: 24.7 to 32.3, to 11 m) stands at the sight line in most districts, and behind the perimeter fence
// (27.5 m) in the others. So:
//   24.5 m          the LOW line, everywhere: the shift at its trestles (SM_KerbCrew), security on its round
//                   (SM_Patrol), the film crew applauding (24 m). No light. Seen from the low side of a banked bend
//   26 m            where a district has the fence, or a slot has neither rack nor tanks: scaffolds, loading docks,
//                   the gate guards, the waste crew, the laboratory staff, the crows on their rail, the crew
//   30.3 / 34.4 m   behind a rack or a tank row: the high scaffolds (decks at 7 m) and the watch towers (11.5 m)
//   120 m and out   the far shift on its footbridge and the crows (Far rules: they stay beside loops and pipes)
// Every and phase count 45.24 m slots from the start of the road (slot s is used when (s + phase) % every == 0), so
// the phases below are chosen against the dressing's own (a rack slot, a tank slot, a hazard sign's slot).
#define PPL(n) TEXT("/Game/People/ChemicalWorks/" n)
#define CREW(n) TEXT("/Game/People/Crew/SM_Crew" n "_Work")
const float K = 24.5f, N = 26.f, R = 30.3f, T = 34.4f;   // K was 24.1: the review measured its lit parts at 23.7 to 24.0 m (a steady light stays 24 m out)

// ---- the low line, the whole road: the shift, security, the crew at the tape
Zone(TEXT("People: the kerb"), 0.30f, 99.f);
	Scatter(PPL("SM_KerbCrew_A"), K, K, 6, 0, 0.7f, 1.f, 0.f, -1);
	Scatter(PPL("SM_KerbCrew_B"), K, K, 6, 3, 0.7f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Patrol"), K, K, 6, 2, 0.55f, 1.f, 0.f, -1);
	Scatter(PPL("SM_KerbCrew_B"), K, K, 6, 1, 0.7f, 1.f, 0.f, 1);
	Scatter(PPL("SM_KerbCrew_A"), K, K, 6, 4, 0.7f, 1.f, 0.f, 1);
	Scatter(PPL("SM_Patrol"), K, K, 6, 5, 0.55f, 1.f, 0.f, 1);
	Scatter(CREW("ApplauseA"), 24.f, 24.f, 12, 11, 1.f, 1.f, 0.f, -1);
	Scatter(CREW("ApplauseB"), 24.f, 24.f, 12, 4, 1.f, 1.f, 0.f, 1);
// ---- the far shift and the crows, the whole road
Zone(TEXT("People: far"), 0.03f, 99.f);
	Far(PPL("SM_FarCatwalk"), 120.f, 200.f, 16, 3, 0.9f, 1.f, 0.f, -1, 1.5f, 10.f);
	Far(PPL("SM_FarCatwalk"), 120.f, 200.f, 16, 11, 0.9f, 1.f, 0.f, 1, 1.5f, 10.f);
	Far(PPL("SM_CrowFlock"), 260.f, 460.f, 16, 1, 0.9f, 1.f, 0.2f, -1, 0.f, 180.f);
	Far(PPL("SM_CrowFlock"), 260.f, 460.f, 16, 9, 0.9f, 1.f, 0.2f, 1, 0.f, 180.f);

// ---- the gate. The first 300 m are the base of the shoot: the guards at the gate, then the crew
Zone(TEXT("People: start"), 0.03f, 0.30f);
	Scatter(PPL("SM_Guards"), 25.9f, 25.9f, 6, 5, 1.f);
	Scatter(CREW("Camera"), 25.4f, 25.4f, 6, 4, 1.f, 1.f, 0.f, -1);
	Scatter(CREW("ApplauseA"), 24.f, 24.f, 6, 4, 1.f, 1.f, 0.f, 1);
	Scatter(CREW("Village"), 25.6f, 25.6f, 6, 3, 1.f, 1.f, 0.f, 1);
	Scatter(CREW("Lights"), 26.2f, 26.2f, 6, 3, 1.f, 1.f, 0.f, -1);
	Scatter(CREW("ApplauseB"), 24.f, 24.f, 6, 2, 1.f, 1.f, 0.f, -1);
	Scatter(CREW("Stunt"), 25.9f, 25.9f, 6, 2, 1.f, 1.f, 0.f, 1);
	Scatter(CREW("Sound"), 25.6f, 25.6f, 6, 1, 1.f, 1.f, 0.f, -1);
	Scatter(CREW("Catering"), 25.7f, 25.7f, 6, 1, 1.f, 1.f, 0.f, 1);
	Scatter(CREW("Dolly"), 25.4f, 25.4f, 6, 0, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Gantry_A"), N, N, 6, 0, 1.f, 1.f, 0.f, 1);
Zone(TEXT("People: gate"), 0.30f, 1.23f);
	Scatter(PPL("SM_Gantry_A"), N, N, 3, 0, 0.9f, 1.f, 0.f, -1);
	Scatter(PPL("SM_DockCrowd_A"), N, N, 6, 1, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_CrowsPerch"), N, N, 6, 4, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Gantry_C"), N, N, 3, 1, 0.9f, 1.f, 0.f, 1);
	Scatter(PPL("SM_DockCrowd_A"), N, N, 6, 0, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_Guards"), 25.9f, 25.9f, 6, 3, 1.f, 1.f, 0.f, 1);

// ---- the tank farm: the tank rows stand 24.7 to 32.3 m both sides. Watch towers behind them
Zone(TEXT("People: tank farm"), 1.23f, 2.6f);
	Scatter(PPL("SM_WatchTower"), T, T, 3, 0, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_WatchTower"), T, T, 3, 1, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_Gantry_C"), 38.5f, 39.5f, 3, 2, 0.8f);

// ---- the process units: the rack at 26.5 m both sides. High scaffolds and towers behind it
Zone(TEXT("People: process units"), 2.6f, 3.98f);
	Scatter(PPL("SM_Gantry_B"), R, R, 3, 0, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_WatchTower"), R, R, 3, 1, 0.9f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Gantry_C"), R, R, 3, 1, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_WatchTower"), R, R, 3, 2, 0.9f, 1.f, 0.f, 1);

// ---- the reactor block: the fence at 27.5 m. The laboratory staff, guards; hazard signs on odd slots
Zone(TEXT("People: reactor block"), 3.98f, 5.35f);
	Scatter(PPL("SM_Boffins"), N, N, 4, 0, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Gantry_A"), N, N, 4, 2, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Boffins"), N, N, 4, 2, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_Guards"), 25.9f, 25.9f, 8, 0, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_DockCrowd_B"), N, N, 8, 4, 1.f, 1.f, 0.f, 1);
// the first jump (4.37 km): the shoot is out for it (the track itself keeps the run-up and the landing clear)
Zone(TEXT("People: first jump"), 4.12f, 4.75f);
	Scatter(CREW("Camera"), 25.4f, 25.4f, 4, 1, 1.f, 1.f, 0.f, -1);
	Scatter(CREW("Stunt"), 25.9f, 25.9f, 4, 1, 1.f, 1.f, 0.f, 1);
	Scatter(CREW("Lights"), 26.2f, 26.2f, 4, 3, 1.f, 1.f, 0.f, 1);
	Scatter(CREW("Dolly"), 25.4f, 25.4f, 4, 3, 1.f, 1.f, 0.f, -1);

// ---- pipe alley: the rack at 26.5 m both sides
Zone(TEXT("People: pipe alley"), 5.35f, 6.72f);
	Scatter(PPL("SM_Gantry_C"), R, R, 3, 0, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_WatchTower"), R, R, 3, 1, 0.9f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Gantry_B"), R, R, 3, 2, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_WatchTower"), R, R, 3, 0, 0.9f, 1.f, 0.f, 1);

// ---- the loading yards: left, slots with s % 3 == 1 have neither bay nor tanks; right, s % 3 == 0
Zone(TEXT("People: loading yards"), 6.72f, 7.94f);
	Scatter(PPL("SM_DockCrowd_A"), N, N, 6, 2, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Gantry_A"), N, N, 6, 5, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_DockCrowd_A"), N, N, 6, 0, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_Gantry_C"), N, N, 6, 3, 1.f, 1.f, 0.f, 1);
// the second jump (7.53 km), over to the mega vat
Zone(TEXT("People: second jump"), 7.28f, 7.94f);
	Scatter(CREW("Camera"), 25.4f, 25.4f, 3, 2, 1.f, 1.f, 0.f, 1);
	Scatter(CREW("Crane"), 31.f, 31.f, 6, 0, 1.f, 1.f, 0.f, -1);
	Scatter(CREW("Sound"), 25.6f, 25.6f, 6, 3, 1.f, 1.f, 0.f, -1);
Zone(TEXT("People: mega vat"), 7.95f, 8.09f);
	Scatter(PPL("SM_Boffins"), N, N, 1, 0, 1.f, 1.f, 0.f, 1);

// ---- the waste run: the fence at 27.5 m; hazard signs on even slots. The waste crews, the crows
Zone(TEXT("People: waste run"), 8.09f, 9.46f);
	Scatter(PPL("SM_Hazmat"), 26.2f, 26.2f, 4, 1, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_CrowsPerch"), N, N, 4, 3, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Hazmat"), 26.2f, 26.2f, 4, 3, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_DockCrowd_B"), N, N, 4, 1, 1.f, 1.f, 0.f, 1);
// before the arena of the boss (the finale starts at 9.46 km): the whole shoot, one last time
Zone(TEXT("People: before the finale"), 9.14f, 9.46f);
	Scatter(CREW("Camera"), 25.4f, 25.4f, 2, 0, 1.f, 1.f, 0.f, -1);
	Scatter(CREW("Village"), 25.6f, 25.6f, 2, 0, 1.f, 1.f, 0.f, 1);
	Scatter(CREW("Lights"), 26.2f, 26.2f, 4, 2, 1.f, 1.f, 0.f, -1);
	Scatter(CREW("Stunt"), 25.9f, 25.9f, 4, 2, 1.f, 1.f, 0.f, 1);

// ---- the export terminal. Left: even slots the rack, s % 4 == 1 open. Right: odd slots the rack, s % 4 == 2 open
Zone(TEXT("People: export terminal"), 9.46f, 12.72f);
	Scatter(PPL("SM_DockCrowd_A"), N, N, 8, 3, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Boffins"), N, N, 8, 7, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Gantry_C"), R, R, 4, 0, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_WatchTower"), R, R, 8, 6, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_DockCrowd_B"), N, N, 8, 6, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_Gantry_A"), N, N, 8, 2, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_Gantry_B"), R, R, 4, 1, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_WatchTower"), R, R, 8, 3, 1.f, 1.f, 0.f, 1);
Zone(TEXT("People: flare avenue"), 12.72f, 13.85f);
	Scatter(PPL("SM_Gantry_C"), R, R, 2, 0, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_WatchTower"), R, R, 4, 1, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Gantry_B"), R, R, 2, 1, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_WatchTower"), R, R, 4, 0, 1.f, 1.f, 0.f, 1);

// ---- the cooling field. Left: even slots the rack, s % 4 == 1 open. Right: odd slots the rack, s % 4 == 0 open
Zone(TEXT("People: cooling field"), 13.85f, 17.4f);
	Scatter(PPL("SM_Gantry_A"), N, N, 8, 7, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_DockCrowd_A"), N, N, 8, 3, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Gantry_C"), R, R, 4, 0, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Boffins"), N, N, 8, 0, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_DockCrowd_B"), N, N, 8, 4, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_Gantry_B"), R, R, 4, 1, 1.f, 1.f, 0.f, 1);

// ---- tank farm B: the tank rows stand back (30.2 to 37.8 m), so the 26 m line is open; valve stations on s % 3 == 2
Zone(TEXT("People: tank farm B"), 17.4f, 21.4f);
	Scatter(PPL("SM_Gantry_A"), N, N, 3, 0, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_DockCrowd_A"), N, N, 6, 5, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Hazmat"), 26.2f, 26.2f, 6, 2, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Gantry_C"), N, N, 3, 2, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_DockCrowd_B"), N, N, 6, 0, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_CrowsPerch"), N, N, 6, 3, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_WatchTower"), 39.f, 39.f, 5, 2, 1.f);
	Scatter(CREW("Catering"), 25.7f, 25.7f, 30, 4, 1.f, 1.f, 0.f, -1);
	Scatter(CREW("Dolly"), 25.4f, 25.4f, 30, 19, 1.f, 1.f, 0.f, 1);

// ---- reactor two: the fence at 27.5 m; hazard signs on s % 3 == 2
Zone(TEXT("People: reactor two"), 21.4f, 25.6f);
	Scatter(PPL("SM_Boffins"), N, N, 3, 0, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Gantry_A"), N, N, 3, 2, 1.f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Gantry_C"), N, N, 3, 0, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_DockCrowd_B"), N, N, 6, 5, 1.f, 1.f, 0.f, 1);
	Scatter(PPL("SM_Guards"), 25.9f, 25.9f, 6, 2, 1.f, 1.f, 0.f, 1);
	Scatter(CREW("Camera"), 25.4f, 25.4f, 30, 13, 1.f, 1.f, 0.f, -1);
	Scatter(CREW("Village"), 25.6f, 25.6f, 30, 28, 1.f, 1.f, 0.f, 1);

#undef PPL
#undef CREW

{
// ---- BEGIN far (ft_people_chem_far.py) ----
// FAR LIFE AND CREATURES of the works (the extra worker "far", 15 Nov 2026). Meshes: Content/Python/ft_people_chem_far.py
// -> /Game/People/ChemicalWorks/SM_Far*. Nothing here stands nearer than 44 m: the 17 to 40 m band is the main
// worker's and the film crew's. Every piece is TALL, lit by a steady light of its own (nothing blinks) and saved with
// the always-moving master. What stands at the sight line (a pipe rack 7.6 m tall at 26.5 m, a tank row 11 m tall at
// 28.5 m) hides the land behind it from the road, so these pieces carry their life 10 to 25 m up:
//   44 to 54 m      SM_FarYardDogs: the strays on their container stack (top 10 m), the rats on the pipe behind
//   50 to 75 m      SM_FarCrows_A: the crows against their lit hoarding (13 to 21 m up)
//   62 to 95 m      SM_FarShift_A: the shift change on its footbridge (16 m up, 64 m long)
//   75 to 120 m     SM_FarTankCrew: the gang on the tank top (22 m up)
//   90 to 260 m     SM_FarFlock_A (gulls wheeling, 30 to 80 m up) and SM_FarFlock_B (the skein crossing): Far rules with
//                   no drop, so the sky's birds stay beside loops, pipes and jumps
// The standing pieces are Scatters: they stand on the land where it rises and falls (the plateau, the ramps), and the
// track keeps them out of loops, pipes and jumps. Each district has its own mix. Left and right are separate rules; the
// phases were chosen (C:/ftwork/jobs/L1f/mkblock.py, a scratch script) so that no two standing pieces of one side
// stand in the same or in neighbouring 45 m slots.
#define FARP(n) TEXT("/Game/People/ChemicalWorks/SM_Far" n)
Zone(TEXT("People far: the sky"), 0.03f, 99.f);
	Far(FARP("Flock_A"), 100.f, 260.f, 28, 3, 1.f, 1.f, 0.2f, -1, 0.f, 180.f);
	Far(FARP("Flock_A"), 100.f, 260.f, 28, 17, 1.f, 1.f, 0.2f, 1, 0.f, 180.f);
	Far(FARP("Flock_B"), 130.f, 200.f, 70, 20, 1.f, 1.f, 0.f, -1, 0.f, 0.f);
	Far(FARP("Flock_B"), 130.f, 200.f, 70, 55, 1.f, 1.f, 0.f, 1, 0.f, 0.f);   // review: was 90 m with 15 degrees of yaw (its far end came to 5 m from the centre line, 40 m up)
Zone(TEXT("People far: gate"), 0.10f, 1.23f);
	Scatter(FARP("YardDogs"), 44.f, 54.f, 8, 6, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Shift_A"), 62.f, 95.f, 16, 8, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("YardDogs"), 44.f, 54.f, 8, 6, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("Shift_A"), 62.f, 95.f, 16, 8, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: tank farm"), 1.23f, 2.60f);
	Scatter(FARP("TankCrew"), 75.f, 120.f, 8, 0, 1.f, 1.f, 0.f, -1, 20.f);
	Scatter(FARP("YardDogs"), 44.f, 54.f, 16, 2, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("TankCrew"), 75.f, 120.f, 8, 4, 1.f, 1.f, 0.f, 1, 20.f);
	Scatter(FARP("YardDogs"), 44.f, 54.f, 16, 0, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: process units"), 2.60f, 3.98f);
	Scatter(FARP("Shift_A"), 62.f, 95.f, 8, 1, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Crows_A"), 50.f, 75.f, 16, 3, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Shift_A"), 62.f, 95.f, 8, 4, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("Crows_A"), 50.f, 75.f, 16, 0, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: reactor block"), 3.98f, 5.35f);
	Scatter(FARP("Crows_A"), 50.f, 75.f, 8, 3, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("TankCrew"), 75.f, 120.f, 16, 0, 1.f, 1.f, 0.f, -1, 20.f);
	Scatter(FARP("Crows_A"), 50.f, 75.f, 8, 4, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("TankCrew"), 75.f, 120.f, 16, 0, 1.f, 1.f, 0.f, 1, 20.f);
Zone(TEXT("People far: pipe alley"), 5.35f, 6.72f);
	Scatter(FARP("Shift_A"), 62.f, 95.f, 8, 0, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("YardDogs"), 44.f, 54.f, 16, 2, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Shift_A"), 62.f, 95.f, 8, 4, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("YardDogs"), 44.f, 54.f, 16, 0, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: loading yards"), 6.72f, 7.94f);
	Scatter(FARP("YardDogs"), 44.f, 54.f, 8, 1, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Crows_A"), 50.f, 75.f, 16, 3, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("YardDogs"), 44.f, 54.f, 8, 2, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("Crows_A"), 50.f, 75.f, 16, 4, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: waste run"), 8.09f, 9.46f);
	Scatter(FARP("Crows_A"), 50.f, 75.f, 8, 0, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("YardDogs"), 44.f, 54.f, 16, 2, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Crows_A"), 50.f, 75.f, 8, 4, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("YardDogs"), 44.f, 54.f, 16, 0, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: export terminal"), 9.46f, 12.72f);
	Scatter(FARP("Shift_A"), 62.f, 95.f, 18, 0, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("TankCrew"), 75.f, 120.f, 36, 2, 1.f, 1.f, 0.f, -1, 20.f);
	Scatter(FARP("YardDogs"), 44.f, 54.f, 36, 4, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Shift_A"), 62.f, 95.f, 18, 9, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("TankCrew"), 75.f, 120.f, 36, 0, 1.f, 1.f, 0.f, 1, 20.f);
	Scatter(FARP("YardDogs"), 44.f, 54.f, 36, 2, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: flare avenue"), 12.72f, 13.85f);
	Scatter(FARP("Crows_A"), 50.f, 75.f, 12, 7, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Crows_A"), 50.f, 75.f, 12, 7, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: cooling field"), 13.85f, 17.40f);
	Scatter(FARP("TankCrew"), 75.f, 120.f, 18, 12, 1.f, 1.f, 0.f, -1, 20.f);
	Scatter(FARP("Crows_A"), 50.f, 75.f, 36, 14, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Shift_A"), 62.f, 95.f, 36, 16, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("TankCrew"), 75.f, 120.f, 18, 12, 1.f, 1.f, 0.f, 1, 20.f);
	Scatter(FARP("Crows_A"), 50.f, 75.f, 36, 14, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("Shift_A"), 62.f, 95.f, 36, 16, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: tank farm B"), 17.40f, 21.40f);
	Scatter(FARP("TankCrew"), 75.f, 120.f, 22, 0, 1.f, 1.f, 0.f, -1, 20.f);
	Scatter(FARP("YardDogs"), 44.f, 54.f, 22, 2, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Crows_A"), 50.f, 75.f, 44, 4, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("TankCrew"), 75.f, 120.f, 22, 10, 1.f, 1.f, 0.f, 1, 20.f);
	Scatter(FARP("YardDogs"), 44.f, 54.f, 22, 0, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("Crows_A"), 50.f, 75.f, 44, 2, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: reactor two"), 21.40f, 25.60f);
	Scatter(FARP("Crows_A"), 50.f, 75.f, 22, 7, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Shift_A"), 62.f, 95.f, 44, 9, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("YardDogs"), 44.f, 54.f, 44, 11, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Crows_A"), 50.f, 75.f, 22, 11, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("Shift_A"), 62.f, 95.f, 44, 7, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("YardDogs"), 44.f, 54.f, 44, 9, 1.f, 1.f, 0.f, 1, 8.f);
#undef FARP
// ---- END far ----
}

{
// ---- BEGIN watch (ft_people_chem_watch.py; this block is written by ft_people_chem_watch_table.py) ----
// THE WATCHERS: the night shift, downed tools, massed on raised steel 40 to 41 m from the centre line (all TALL pieces,
// steady light only), and on the roofs of far tanks. The dressing fills that band differently in every district
// (loading bays to 41.5 m, tank rows to 37.8 m, compressor sheds, kilns, blow-down stacks, light masts, the wall
// turns' flare derricks, the mega vat), so every watcher has a rule of its own that fits one slot (every 1000,
// phase 1000 - slot), worked out from the dressing's rules by ft_people_chem_watch_table.py: 243 pieces, on a side
// at most every third slot. The flare avenue (12.72 to 13.85 km) is left to its flames. A lambda of its own, like the
// dressing table's passes, so that the level's population function stays small enough for the compiler.
auto PplWatch = [&]()
{
#define WATCH(n) TEXT("/Game/People/ChemicalWorks/SM_Watch" n)
#define WATCH_AT(n, s, side, x) Scatter(WATCH(n), x, x, 1000, 1000 - (s), 1.f, 1.f, 0.f, side)
Zone(TEXT("People watch: the works"), 0.03f, 99.f);
// the gate (slots 1 to 27): 10
	WATCH_AT("Roof", 4, 1, 40.5f); WATCH_AT("Stand_A", 5, -1, 40.5f); WATCH_AT("Lift_A", 9, -1, 40.0f); WATCH_AT("Stand_B", 10, 1, 40.5f); WATCH_AT("Roof", 14, -1, 40.5f);
	WATCH_AT("Lift_B", 14, 1, 40.0f); WATCH_AT("Stand_A", 19, 1, 40.5f); WATCH_AT("Stand_B", 20, -1, 40.5f); WATCH_AT("Lift_B", 24, -1, 40.0f); WATCH_AT("Lift_A", 25, 1, 40.0f);
// the tank farm (slots 28 to 57): 8
	WATCH_AT("Stand_A", 30, -1, 40.5f); WATCH_AT("Roof", 30, 1, 40.5f); WATCH_AT("Lift_A", 42, -1, 40.0f); WATCH_AT("Stand_B", 42, 1, 40.5f); WATCH_AT("Roof", 50, -1, 40.5f);
	WATCH_AT("Lift_B", 50, 1, 40.0f); WATCH_AT("Stand_B", 54, -1, 40.5f); WATCH_AT("Stand_A", 54, 1, 40.5f);
// the process units (slots 58 to 87): 6
	WATCH_AT("Lift_B", 69, -1, 40.0f); WATCH_AT("Lift_A", 69, 1, 40.0f); WATCH_AT("Lift_A", 75, -1, 40.0f); WATCH_AT("Lift_A", 75, 1, 40.0f); WATCH_AT("Lift_A", 79, -1, 40.0f);
	WATCH_AT("Lift_A", 85, -1, 40.0f);
// the reactor block (slots 88 to 118): 16
	WATCH_AT("Stand_B", 89, -1, 40.5f); WATCH_AT("Stand_B", 89, 1, 40.5f); WATCH_AT("Lift_B", 94, -1, 40.0f); WATCH_AT("Lift_B", 94, 1, 40.0f); WATCH_AT("Stand_A", 97, -1, 40.5f);
	WATCH_AT("Stand_A", 97, 1, 40.5f); WATCH_AT("Lift_A", 100, -1, 40.0f); WATCH_AT("Lift_A", 102, 1, 40.0f); WATCH_AT("Roof", 105, -1, 40.5f); WATCH_AT("Roof", 105, 1, 40.5f);
	WATCH_AT("Stand_B", 109, -1, 40.5f); WATCH_AT("Stand_B", 109, 1, 40.5f); WATCH_AT("Lift_B", 112, -1, 40.0f); WATCH_AT("Lift_B", 114, 1, 40.0f); WATCH_AT("Stand_A", 117, -1, 40.5f);
	WATCH_AT("Stand_A", 117, 1, 40.5f);
// pipe alley (slots 119 to 148): 16
	WATCH_AT("Lift_A", 120, -1, 40.0f); WATCH_AT("Lift_A", 120, 1, 40.0f); WATCH_AT("Roof", 125, -1, 40.5f); WATCH_AT("Roof", 125, 1, 40.5f); WATCH_AT("Stand_B", 129, -1, 40.5f);
	WATCH_AT("Stand_B", 129, 1, 40.5f); WATCH_AT("Lift_B", 132, -1, 40.0f); WATCH_AT("Lift_B", 132, 1, 40.0f); WATCH_AT("Stand_A", 135, -1, 40.5f); WATCH_AT("Stand_A", 135, 1, 40.5f);
	WATCH_AT("Lift_A", 140, -1, 40.0f); WATCH_AT("Lift_A", 140, 1, 40.0f); WATCH_AT("Roof", 144, -1, 40.5f); WATCH_AT("Roof", 144, 1, 40.5f); WATCH_AT("Stand_B", 147, -1, 40.5f);
	WATCH_AT("Stand_B", 147, 1, 40.5f);
// the loading yards (slots 149 to 175): 14
	WATCH_AT("Lift_B", 150, 1, 40.0f); WATCH_AT("Lift_B", 154, -1, 40.0f); WATCH_AT("Stand_A", 154, 1, 40.5f); WATCH_AT("Lift_A", 157, -1, 40.0f); WATCH_AT("Lift_A", 157, 1, 40.0f);
	WATCH_AT("Lift_A", 160, -1, 40.0f); WATCH_AT("Roof", 160, 1, 40.5f); WATCH_AT("Lift_B", 165, 1, 40.0f); WATCH_AT("Lift_A", 169, -1, 40.0f); WATCH_AT("Lift_B", 169, 1, 40.0f);
	WATCH_AT("Stand_B", 172, -1, 40.5f); WATCH_AT("Stand_A", 172, 1, 40.5f); WATCH_AT("Lift_B", 175, -1, 40.0f); WATCH_AT("Lift_A", 175, 1, 40.0f);
// the waste run (slots 179 to 209): 12
	WATCH_AT("Roof", 180, 1, 40.5f); WATCH_AT("Stand_B", 184, 1, 40.5f); WATCH_AT("Stand_A", 185, -1, 40.5f); WATCH_AT("Lift_A", 189, -1, 40.0f); WATCH_AT("Lift_B", 190, 1, 40.0f);
	WATCH_AT("Stand_A", 194, 1, 40.5f); WATCH_AT("Roof", 195, -1, 40.5f); WATCH_AT("Stand_B", 199, -1, 40.5f); WATCH_AT("Lift_A", 200, 1, 40.0f); WATCH_AT("Roof", 204, 1, 40.5f);
	WATCH_AT("Lift_B", 205, -1, 40.0f); WATCH_AT("Stand_A", 209, -1, 40.5f);
// the export terminal (slots 210 to 281): 25
	WATCH_AT("Lift_A", 212, -1, 40.0f); WATCH_AT("Stand_B", 212, 1, 40.5f); WATCH_AT("Roof", 220, -1, 40.5f); WATCH_AT("Lift_B", 220, 1, 40.0f); WATCH_AT("Stand_B", 224, -1, 40.5f);
	WATCH_AT("Stand_A", 224, 1, 40.5f); WATCH_AT("Lift_B", 230, -1, 40.0f); WATCH_AT("Lift_A", 232, 1, 40.0f); WATCH_AT("Stand_A", 234, -1, 40.5f); WATCH_AT("Lift_A", 240, -1, 40.0f);
	WATCH_AT("Roof", 240, 1, 40.5f); WATCH_AT("Roof", 244, -1, 40.5f); WATCH_AT("Stand_B", 244, 1, 40.5f); WATCH_AT("Stand_B", 250, -1, 40.5f); WATCH_AT("Lift_B", 252, 1, 40.0f);
	WATCH_AT("Lift_B", 254, -1, 40.0f); WATCH_AT("Stand_A", 260, -1, 40.5f); WATCH_AT("Stand_A", 260, 1, 40.5f); WATCH_AT("Lift_A", 264, -1, 40.0f); WATCH_AT("Lift_A", 264, 1, 40.0f);
	WATCH_AT("Roof", 270, -1, 40.5f); WATCH_AT("Roof", 272, 1, 40.5f); WATCH_AT("Stand_B", 274, -1, 40.5f); WATCH_AT("Lift_B", 280, -1, 40.0f); WATCH_AT("Stand_B", 280, 1, 40.5f);
// the cooling field (slots 307 to 384): 40
	WATCH_AT("Lift_B", 309, -1, 40.0f); WATCH_AT("Lift_B", 309, 1, 40.0f); WATCH_AT("Lift_A", 313, -1, 40.0f); WATCH_AT("Stand_A", 313, 1, 40.5f); WATCH_AT("Lift_B", 316, -1, 40.0f);
	WATCH_AT("Lift_A", 316, 1, 40.0f); WATCH_AT("Lift_A", 321, -1, 40.0f); WATCH_AT("Lift_B", 321, 1, 40.0f); WATCH_AT("Lift_B", 324, -1, 40.0f); WATCH_AT("Stand_B", 324, 1, 40.5f);
	WATCH_AT("Lift_A", 328, -1, 40.0f); WATCH_AT("Lift_B", 328, 1, 40.0f); WATCH_AT("Lift_A", 333, -1, 40.0f); WATCH_AT("Lift_A", 333, 1, 40.0f); WATCH_AT("Roof", 336, -1, 40.5f);
	WATCH_AT("Lift_A", 336, 1, 40.0f); WATCH_AT("Lift_B", 340, -1, 40.0f); WATCH_AT("Lift_A", 340, 1, 40.0f); WATCH_AT("Lift_B", 345, -1, 40.0f); WATCH_AT("Lift_B", 345, 1, 40.0f);
	WATCH_AT("Stand_A", 348, -1, 40.5f); WATCH_AT("Lift_B", 348, 1, 40.0f); WATCH_AT("Lift_A", 354, -1, 40.0f); WATCH_AT("Stand_A", 354, 1, 40.5f); WATCH_AT("Lift_B", 358, -1, 40.0f);
	WATCH_AT("Lift_A", 358, 1, 40.0f); WATCH_AT("Stand_B", 361, -1, 40.5f); WATCH_AT("Roof", 361, 1, 40.5f); WATCH_AT("Lift_B", 364, -1, 40.0f); WATCH_AT("Lift_A", 364, 1, 40.0f);
	WATCH_AT("Lift_A", 369, -1, 40.0f); WATCH_AT("Lift_B", 369, 1, 40.0f); WATCH_AT("Lift_A", 373, -1, 40.0f); WATCH_AT("Stand_A", 373, 1, 40.5f); WATCH_AT("Lift_A", 376, -1, 40.0f);
	WATCH_AT("Lift_A", 376, 1, 40.0f); WATCH_AT("Lift_B", 381, -1, 40.0f); WATCH_AT("Lift_A", 381, 1, 40.0f); WATCH_AT("Lift_B", 384, -1, 40.0f); WATCH_AT("Stand_B", 384, 1, 40.5f);
// tank farm B (slots 385 to 473): 58
	WATCH_AT("Stand_B", 387, -1, 40.8f); WATCH_AT("Lift_B", 387, 1, 40.8f); WATCH_AT("Lift_A", 390, -1, 40.8f); WATCH_AT("Stand_B", 390, 1, 40.8f); WATCH_AT("Stand_B", 393, -1, 40.8f);
	WATCH_AT("Lift_A", 393, 1, 40.8f); WATCH_AT("Stand_B", 396, -1, 40.8f); WATCH_AT("Stand_B", 396, 1, 40.8f); WATCH_AT("Lift_B", 399, -1, 40.8f); WATCH_AT("Stand_B", 399, 1, 40.8f);
	WATCH_AT("Stand_B", 402, -1, 40.8f); WATCH_AT("Lift_B", 402, 1, 40.8f); WATCH_AT("Lift_A", 405, -1, 40.8f); WATCH_AT("Stand_B", 405, 1, 40.8f); WATCH_AT("Stand_B", 408, -1, 40.8f);
	WATCH_AT("Lift_A", 408, 1, 40.8f); WATCH_AT("Stand_B", 411, -1, 40.8f); WATCH_AT("Stand_B", 411, 1, 40.8f); WATCH_AT("Lift_B", 414, -1, 40.8f); WATCH_AT("Stand_B", 414, 1, 40.8f);
	WATCH_AT("Stand_B", 417, -1, 40.8f); WATCH_AT("Lift_B", 417, 1, 40.8f); WATCH_AT("Lift_A", 420, -1, 40.8f); WATCH_AT("Stand_B", 420, 1, 40.8f); WATCH_AT("Stand_B", 423, -1, 40.8f);
	WATCH_AT("Lift_A", 423, 1, 40.8f); WATCH_AT("Stand_B", 426, -1, 40.8f); WATCH_AT("Stand_B", 426, 1, 40.8f); WATCH_AT("Lift_B", 429, -1, 40.8f); WATCH_AT("Stand_B", 429, 1, 40.8f);
	WATCH_AT("Stand_B", 432, -1, 40.8f); WATCH_AT("Lift_B", 432, 1, 40.8f); WATCH_AT("Lift_A", 435, -1, 40.8f); WATCH_AT("Stand_B", 435, 1, 40.8f); WATCH_AT("Stand_B", 438, -1, 40.8f);
	WATCH_AT("Lift_A", 438, 1, 40.8f); WATCH_AT("Stand_B", 441, -1, 40.8f); WATCH_AT("Stand_B", 441, 1, 40.8f); WATCH_AT("Lift_B", 444, -1, 40.8f); WATCH_AT("Stand_B", 444, 1, 40.8f);
	WATCH_AT("Stand_B", 447, -1, 40.8f); WATCH_AT("Lift_B", 447, 1, 40.8f); WATCH_AT("Lift_A", 450, -1, 40.8f); WATCH_AT("Stand_B", 450, 1, 40.8f); WATCH_AT("Stand_B", 453, -1, 40.8f);
	WATCH_AT("Lift_A", 453, 1, 40.8f); WATCH_AT("Stand_B", 456, -1, 40.8f); WATCH_AT("Stand_B", 456, 1, 40.8f); WATCH_AT("Lift_B", 459, -1, 40.8f); WATCH_AT("Stand_B", 459, 1, 40.8f);
	WATCH_AT("Stand_B", 462, -1, 40.8f); WATCH_AT("Lift_B", 462, 1, 40.8f); WATCH_AT("Lift_A", 465, -1, 40.8f); WATCH_AT("Stand_B", 465, 1, 40.8f); WATCH_AT("Stand_B", 468, -1, 40.8f);
	WATCH_AT("Lift_A", 468, 1, 40.8f); WATCH_AT("Stand_B", 471, -1, 40.8f); WATCH_AT("Stand_B", 471, 1, 40.8f);
// reactor two (slots 474 to 565): 38
	WATCH_AT("Lift_B", 474, -1, 40.0f); WATCH_AT("Lift_B", 474, 1, 40.0f); WATCH_AT("Stand_A", 480, -1, 40.5f); WATCH_AT("Lift_B", 481, 1, 40.0f); WATCH_AT("Lift_A", 484, -1, 40.0f);
	WATCH_AT("Stand_A", 486, 1, 40.5f); WATCH_AT("Roof", 489, -1, 40.5f); WATCH_AT("Lift_A", 489, 1, 40.0f); WATCH_AT("Roof", 495, 1, 40.5f); WATCH_AT("Stand_B", 496, -1, 40.5f);
	WATCH_AT("Lift_A", 499, 1, 40.0f); WATCH_AT("Lift_B", 501, -1, 40.0f); WATCH_AT("Stand_A", 504, -1, 40.5f); WATCH_AT("Lift_B", 505, 1, 40.0f); WATCH_AT("Lift_A", 510, -1, 40.0f);
	WATCH_AT("Stand_A", 510, 1, 40.5f); WATCH_AT("Lift_A", 514, -1, 40.0f); WATCH_AT("Lift_A", 514, 1, 40.0f); WATCH_AT("Roof", 519, 1, 40.5f); WATCH_AT("Stand_B", 520, -1, 40.5f);
	WATCH_AT("Lift_B", 525, -1, 40.0f); WATCH_AT("Stand_B", 525, 1, 40.5f); WATCH_AT("Stand_A", 529, -1, 40.5f); WATCH_AT("Lift_B", 529, 1, 40.0f); WATCH_AT("Lift_A", 534, -1, 40.0f);
	WATCH_AT("Stand_A", 534, 1, 40.5f); WATCH_AT("Lift_B", 540, -1, 40.0f); WATCH_AT("Lift_A", 541, 1, 40.0f); WATCH_AT("Stand_B", 544, -1, 40.5f); WATCH_AT("Lift_B", 546, 1, 40.0f);
	WATCH_AT("Lift_B", 549, -1, 40.0f); WATCH_AT("Stand_B", 549, 1, 40.5f); WATCH_AT("Lift_B", 555, 1, 40.0f); WATCH_AT("Lift_A", 556, -1, 40.0f); WATCH_AT("Stand_A", 559, 1, 40.5f);
	WATCH_AT("Lift_A", 561, -1, 40.0f); WATCH_AT("Lift_A", 564, -1, 40.0f); WATCH_AT("Lift_A", 565, 1, 40.0f);
// the far tanks: Far rules (they stay beside loops and pipes), standing 1.2 m under the road's level like the land
Zone(TEXT("People watch: far tanks"), 0.3f, 99.f);
	Far(WATCH("TankFar"), 70.f, 110.f, 12, 2, 1.f, 1.f, 0.f, -1, 1.2f, 8.f);
	Far(WATCH("TankFar"), 70.f, 110.f, 12, 8, 1.f, 1.f, 0.f, 1, 1.2f, 8.f);
#undef WATCH_AT
#undef WATCH
};
PplWatch();
// ---- END watch ----
}

{
// ---- BEGIN p3a (gaps, ft_people_chem_p3a.py) ----
// THE GENERAL CAST OF THE WORKS, in the stretches the first rounds left empty (pass 3, builder p3a, 15 Nov 2026).
// Meshes: Content/Python/ft_people_chem_p3a.py -> /Game/People/ChemicalWorks/SM_P3A_*. This block is written by a scratch
// script (C:/ftwork/jobs/L1p3a/mkblock.py), which counts it slot by slot: 355 pieces before the track's own clearance
// test (244 low, 111 tall); no mesh more than 38 times.
//   24.3 m   the LOW lane (seven pieces, all under 2 m as built, their people on the piece's far half: 24.6 m and out):
//            the first round's kerb line (24.1 m) uses, of every twelve slots, six on a side; these rules use three of
//            the other six (left: s % 12 in 7, 8, 2, 11, 5; right: 0, 6, 3, 9, 4, 10), a different three in every
//            district, so the two never share a slot. Left out: the start (the shoot's base), the three wall turns,
//            the mega vat, and the stretches where the shoot stands at the two jumps and before the finale.
//   44 m     the TALL pieces (a stair tower, painters on a hoarding, the mess on its container stack, steeplejacks:
//            each brings its own steel and a steady light, and carries its people 10 to 17 m up): about every fifth
//            slot on a side. None in the flare avenue, at the mega vat or on the wall turns.
// A lambda of its own, like the dressing table's passes, so that the level's population function stays small.
auto PplP3ABlock = [&]()
{
#define P3A(n) TEXT("/Game/People/ChemicalWorks/SM_P3A_" n)
#define P3A_AT(n, s, side, x) Scatter(P3A(n), x, x, 1000, 1000 - (s), 1.f, 1.f, 0.f, side)
const float LOWX = 24.5f;   // review: was 24.3 (lit parts 2 cm inside the 24 m line)
Zone(TEXT("People p3a: low, gate"), 0.30f, 1.23f);
	Scatter(P3A("Marshals"), LOWX, LOWX, 12, 5, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Porters"), LOWX, LOWX, 12, 10, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Break_A"), LOWX, LOWX, 12, 7, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Break_A"), LOWX, LOWX, 12, 6, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Marshals"), LOWX, LOWX, 12, 2, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Porters"), LOWX, LOWX, 12, 8, 1.f, 1.f, 0.f, 1);
Zone(TEXT("People p3a: low, tank farm"), 1.23f, 2.60f);
	Scatter(P3A("Fitters"), LOWX, LOWX, 12, 4, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Sweepers"), LOWX, LOWX, 12, 1, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Hose"), LOWX, LOWX, 12, 5, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Hose"), LOWX, LOWX, 12, 6, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Fitters"), LOWX, LOWX, 12, 3, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Sweepers"), LOWX, LOWX, 12, 2, 1.f, 1.f, 0.f, 1);
Zone(TEXT("People p3a: low, process units, the plateau"), 3.00f, 3.58f);
	Scatter(P3A("Break_B"), LOWX, LOWX, 12, 10, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Fitters"), LOWX, LOWX, 12, 7, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Marshals"), LOWX, LOWX, 12, 4, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Fitters"), LOWX, LOWX, 12, 9, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Marshals"), LOWX, LOWX, 12, 8, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Break_B"), LOWX, LOWX, 12, 0, 1.f, 1.f, 0.f, 1);
Zone(TEXT("People p3a: low, reactor block, before the jump"), 3.98f, 4.12f);
	Scatter(P3A("Hose"), LOWX, LOWX, 12, 1, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Marshals"), LOWX, LOWX, 12, 5, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Break_A"), LOWX, LOWX, 12, 10, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Marshals"), LOWX, LOWX, 12, 3, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Break_A"), LOWX, LOWX, 12, 2, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Hose"), LOWX, LOWX, 12, 0, 1.f, 1.f, 0.f, 1);
Zone(TEXT("People p3a: low, reactor block"), 4.75f, 5.35f);
	Scatter(P3A("Hose"), LOWX, LOWX, 12, 7, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Marshals"), LOWX, LOWX, 12, 5, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Break_A"), LOWX, LOWX, 12, 1, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Marshals"), LOWX, LOWX, 12, 8, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Break_A"), LOWX, LOWX, 12, 3, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Hose"), LOWX, LOWX, 12, 9, 1.f, 1.f, 0.f, 1);
Zone(TEXT("People p3a: low, pipe alley"), 5.35f, 6.72f);
	Scatter(P3A("Fitters"), LOWX, LOWX, 12, 5, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Porters"), LOWX, LOWX, 12, 10, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Break_B"), LOWX, LOWX, 12, 7, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Break_B"), LOWX, LOWX, 12, 2, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Fitters"), LOWX, LOWX, 12, 6, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Porters"), LOWX, LOWX, 12, 3, 1.f, 1.f, 0.f, 1);
Zone(TEXT("People p3a: low, loading yards"), 6.72f, 7.28f);
	Scatter(P3A("Porters"), LOWX, LOWX, 12, 4, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Sweepers"), LOWX, LOWX, 12, 1, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Break_A"), LOWX, LOWX, 12, 10, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Sweepers"), LOWX, LOWX, 12, 0, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Break_A"), LOWX, LOWX, 12, 2, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Porters"), LOWX, LOWX, 12, 8, 1.f, 1.f, 0.f, 1);
Zone(TEXT("People p3a: low, waste run"), 8.12f, 9.14f);
	Scatter(P3A("Hose"), LOWX, LOWX, 12, 10, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Sweepers"), LOWX, LOWX, 12, 1, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Marshals"), LOWX, LOWX, 12, 4, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Sweepers"), LOWX, LOWX, 12, 6, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Marshals"), LOWX, LOWX, 12, 0, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Hose"), LOWX, LOWX, 12, 2, 1.f, 1.f, 0.f, 1);
Zone(TEXT("People p3a: low, export terminal"), 9.46f, 12.72f);
	Scatter(P3A("Porters"), LOWX, LOWX, 12, 1, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Break_B"), LOWX, LOWX, 12, 5, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Fitters"), LOWX, LOWX, 12, 10, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Marshals"), LOWX, LOWX, 12, 9, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Porters"), LOWX, LOWX, 12, 8, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Break_B"), LOWX, LOWX, 12, 0, 1.f, 1.f, 0.f, 1);
Zone(TEXT("People p3a: low, flare avenue"), 12.72f, 13.85f);
	Scatter(P3A("Marshals"), LOWX, LOWX, 12, 7, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Break_A"), LOWX, LOWX, 12, 4, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Sweepers"), LOWX, LOWX, 12, 1, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Break_A"), LOWX, LOWX, 12, 3, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Sweepers"), LOWX, LOWX, 12, 2, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Marshals"), LOWX, LOWX, 12, 6, 1.f, 1.f, 0.f, 1);
Zone(TEXT("People p3a: low, cooling field"), 13.85f, 17.40f);
	Scatter(P3A("Fitters"), LOWX, LOWX, 12, 5, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Sweepers"), LOWX, LOWX, 12, 10, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Hose"), LOWX, LOWX, 12, 4, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Break_B"), LOWX, LOWX, 12, 6, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Hose"), LOWX, LOWX, 12, 3, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Fitters"), LOWX, LOWX, 12, 9, 1.f, 1.f, 0.f, 1);
Zone(TEXT("People p3a: low, tank farm B"), 17.40f, 21.40f);
	Scatter(P3A("Hose"), LOWX, LOWX, 12, 4, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Break_A"), LOWX, LOWX, 12, 1, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Porters"), LOWX, LOWX, 12, 7, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Fitters"), LOWX, LOWX, 12, 2, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Break_A"), LOWX, LOWX, 12, 6, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Sweepers"), LOWX, LOWX, 12, 3, 1.f, 1.f, 0.f, 1);
Zone(TEXT("People p3a: low, reactor two"), 21.40f, 25.60f);
	Scatter(P3A("Marshals"), LOWX, LOWX, 12, 10, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Break_B"), LOWX, LOWX, 12, 1, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Sweepers"), LOWX, LOWX, 12, 4, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("Porters"), LOWX, LOWX, 12, 0, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Break_B"), LOWX, LOWX, 12, 9, 1.f, 1.f, 0.f, 1);
	Scatter(P3A("Break_A"), LOWX, LOWX, 12, 8, 1.f, 1.f, 0.f, 1);
// the tall pieces: each has a rule of its own on one slot (every 1000, phase 1000 - slot), chosen to stand clear of the
// first rounds' watchers (40 m), the blow-down stacks, the light masts, the kilns, the works roads and the yard dogs
Zone(TEXT("People p3a: tall"), 0.03f, 99.f);
// the start: 2
	P3A_AT("Mess", 2, -1, 44.2f); P3A_AT("StairTower", 5, 1, 44.0f);
// the gate: 5
	P3A_AT("Steeplejacks", 12, -1, 44.4f); P3A_AT("Painters", 15, 1, 44.0f); P3A_AT("StairTower", 17, -1, 44.0f); P3A_AT("Mess", 20, 1, 44.2f);
	P3A_AT("Painters", 27, -1, 44.0f);
// the tank farm: 2
	P3A_AT("Mess", 32, -1, 44.2f); P3A_AT("Steeplejacks", 44, -1, 44.4f);
// the process units: 2
	P3A_AT("StairTower", 67, -1, 44.0f); P3A_AT("Steeplejacks", 70, 1, 44.4f);
// the reactor block: 7
	P3A_AT("Painters", 90, -1, 44.0f); P3A_AT("Mess", 95, -1, 44.2f); P3A_AT("Painters", 99, 1, 44.0f); P3A_AT("Steeplejacks", 102, -1, 44.4f);
	P3A_AT("StairTower", 107, -1, 44.0f); P3A_AT("Mess", 112, 1, 44.2f); P3A_AT("Painters", 114, -1, 44.0f);
// pipe alley: 9
	P3A_AT("Mess", 119, -1, 43.6f); P3A_AT("StairTower", 122, 1, 43.6f); P3A_AT("StairTower", 124, -1, 43.6f); P3A_AT("Painters", 127, 1, 43.6f);
	P3A_AT("Painters", 130, -1, 43.6f); P3A_AT("Mess", 134, 1, 43.6f); P3A_AT("Mess", 137, -1, 43.6f); P3A_AT("Steeplejacks", 139, 1, 43.6f);
	P3A_AT("Steeplejacks", 145, -1, 43.6f);
// the loading yards: 5
	P3A_AT("StairTower", 150, -1, 44.0f); P3A_AT("Painters", 155, -1, 44.0f); P3A_AT("StairTower", 159, 1, 44.0f); P3A_AT("Mess", 162, -1, 44.2f);
	P3A_AT("Painters", 167, 1, 44.0f);
// the waste run: 7
	P3A_AT("Steeplejacks", 182, -1, 44.4f); P3A_AT("Mess", 185, 1, 44.2f); P3A_AT("Painters", 187, -1, 44.0f); P3A_AT("Mess", 194, -1, 44.2f);
	P3A_AT("Steeplejacks", 197, 1, 44.4f); P3A_AT("Steeplejacks", 200, -1, 44.4f); P3A_AT("StairTower", 202, 1, 44.0f);
// the export terminal: 21
	P3A_AT("StairTower", 210, -1, 44.0f); P3A_AT("Mess", 215, -1, 44.2f); P3A_AT("Painters", 217, 1, 44.0f); P3A_AT("Steeplejacks", 222, -1, 44.4f);
	P3A_AT("Steeplejacks", 225, 1, 44.4f); P3A_AT("StairTower", 229, -1, 44.0f); P3A_AT("StairTower", 234, 1, 44.0f); P3A_AT("Painters", 237, -1, 44.0f);
	P3A_AT("Painters", 239, 1, 44.0f); P3A_AT("Mess", 242, -1, 44.2f); P3A_AT("Mess", 245, 1, 44.2f); P3A_AT("Steeplejacks", 247, -1, 44.4f);
	P3A_AT("StairTower", 252, -1, 44.0f); P3A_AT("StairTower", 254, 1, 44.0f); P3A_AT("Mess", 257, -1, 44.2f); P3A_AT("Steeplejacks", 262, -1, 44.4f);
	P3A_AT("Painters", 265, 1, 44.0f); P3A_AT("StairTower", 269, -1, 44.0f); P3A_AT("Mess", 274, 1, 44.2f); P3A_AT("Painters", 277, -1, 44.0f);
	P3A_AT("Steeplejacks", 279, 1, 44.4f);
// the cooling field: 10
	P3A_AT("Mess", 307, -1, 44.2f); P3A_AT("Steeplejacks", 319, -1, 44.4f); P3A_AT("StairTower", 325, -1, 44.0f); P3A_AT("Painters", 331, -1, 44.0f);
	P3A_AT("Mess", 337, -1, 44.2f); P3A_AT("Steeplejacks", 343, -1, 44.4f); P3A_AT("Painters", 349, -1, 44.0f); P3A_AT("Mess", 355, -1, 44.2f);
	P3A_AT("Steeplejacks", 367, -1, 44.4f); P3A_AT("StairTower", 379, -1, 44.0f);
// tank farm B: 21
	P3A_AT("Painters", 385, -1, 44.0f); P3A_AT("Steeplejacks", 391, -1, 44.4f); P3A_AT("StairTower", 394, 1, 44.0f); P3A_AT("StairTower", 397, -1, 44.0f);
	P3A_AT("Painters", 400, 1, 44.0f); P3A_AT("Painters", 409, -1, 44.0f); P3A_AT("Mess", 412, 1, 44.2f); P3A_AT("Mess", 415, -1, 44.2f);
	P3A_AT("Steeplejacks", 421, -1, 44.4f); P3A_AT("Steeplejacks", 424, 1, 44.4f); P3A_AT("StairTower", 427, -1, 44.0f); P3A_AT("Mess", 436, -1, 44.2f);
	P3A_AT("StairTower", 439, 1, 44.0f); P3A_AT("Steeplejacks", 442, -1, 44.4f); P3A_AT("Painters", 445, 1, 44.0f); P3A_AT("StairTower", 451, -1, 44.0f);
	P3A_AT("Painters", 457, -1, 44.0f); P3A_AT("Mess", 460, 1, 44.2f); P3A_AT("Mess", 466, -1, 44.2f); P3A_AT("Steeplejacks", 469, 1, 44.4f);
	P3A_AT("Steeplejacks", 472, -1, 44.4f);
// reactor two: 20
	P3A_AT("StairTower", 481, -1, 44.0f); P3A_AT("Painters", 484, 1, 44.0f); P3A_AT("Painters", 486, -1, 44.0f); P3A_AT("Mess", 490, 1, 44.2f);
	P3A_AT("Mess", 495, -1, 44.2f); P3A_AT("StairTower", 501, 1, 44.0f); P3A_AT("Steeplejacks", 505, -1, 44.4f); P3A_AT("StairTower", 511, -1, 44.0f);
	P3A_AT("Mess", 516, -1, 44.2f); P3A_AT("Painters", 520, 1, 44.0f); P3A_AT("Steeplejacks", 526, -1, 44.4f); P3A_AT("StairTower", 535, -1, 44.0f);
	P3A_AT("Painters", 541, -1, 44.0f); P3A_AT("Mess", 544, 1, 44.2f); P3A_AT("Mess", 546, -1, 44.2f); P3A_AT("Steeplejacks", 550, 1, 44.4f);
	P3A_AT("StairTower", 556, 1, 44.0f); P3A_AT("StairTower", 559, -1, 44.0f); P3A_AT("Painters", 561, 1, 44.0f); P3A_AT("Painters", 565, -1, 44.0f);
Zone(TEXT("People p3a: tall, flare avenue"), 12.72f, 13.85f);   // review round: the avenue had nobody readable; never a derrick slot (s % 3 == 1)
	Scatter(P3A("StairTower"), 44.f, 44.f, 6, 3, 1.f, 1.f, 0.f, -1);
	Scatter(P3A("StairTower"), 44.f, 44.f, 6, 0, 1.f, 1.f, 0.f, 1);
#undef P3A_AT
#undef P3A
};
PplP3ABlock();
// ---- END p3a ----
}

{
// ---- BEGIN p3b (scenes, ft_people_chem_p3b.py) ----
// SCENES of the Volta works (pass 3, builder p3b): ten story groups, one mesh a scene, each on a stage of its own (a
// boarded deck 12.3 m up on four legs, a lit sheet behind the cast, two steady lamps: it clears the racks, the bays, the
// sheds and the tank rows, and may stand over them). All TALL: 34 m from the centre line (35 m in the tank farm).
// Steady light only. Every scene has a rule of its own that fits one 45 m slot (every 1000, phase 1000 - slot),
// worked out by C:/ftwork/jobs/L1p3b/mkblock.py from the rules above it: never in a slot and side that has a watch
// tower or a sheeted scaffold of the first pass in front of it, a stand of the watchers behind it, a flare derrick, a
// wall turn or the mega vat; on a side at most every second slot.
// Review round: planned again, now also never in a slot and side where a piece of p3a, c, d, e or f stands within the
// stage's depth (31 to 37 m): the movers' galleries (35.6 m) and the film units (30.3 to 36.3 m) stood in the stages.
// 277 rules: Band 27, Briefing 26, Capstan 17, CraneLift 24, Decon 30, FireDrill 28, Inspection 24, Meeting 38, SafeDays 34, TugOfWar 29.
// A lambda of its own, like the block of the watchers, so that the population function stays small for the compiler.
auto PplP3BScenes = [&]()
{
#define P3B(n) TEXT("/Game/People/ChemicalWorks/SM_P3B_" n)
#define P3B_AT(n, s, side, x) Scatter(P3B(n), x, x, 1000, 1000 - (s), 1.f, 1.f, 0.f, side)
Zone(TEXT("People p3b: scenes"), 0.03f, 99.f);
// the gate (slots 7 to 27): 11
	P3B_AT("SafeDays", 10, -1, 34.0f); P3B_AT("Band", 13, -1, 34.0f); P3B_AT("Briefing", 15, 1, 34.0f); P3B_AT("Briefing", 16, -1, 34.0f); P3B_AT("Inspection", 18, 1, 34.0f);
	P3B_AT("Inspection", 19, -1, 34.0f); P3B_AT("SafeDays", 21, 1, 34.0f); P3B_AT("SafeDays", 22, -1, 34.0f); P3B_AT("Band", 24, 1, 34.0f); P3B_AT("Band", 25, -1, 34.0f);
	P3B_AT("Briefing", 27, 1, 34.0f);
// the tank farm (slots 28 to 57): 0
// the process units (slots 58 to 87): 0
// the reactor block (slots 88 to 118): 19
	P3B_AT("Decon", 88, -1, 34.0f); P3B_AT("SafeDays", 88, 1, 34.0f); P3B_AT("Inspection", 91, -1, 34.0f); P3B_AT("FireDrill", 91, 1, 34.0f); P3B_AT("Decon", 93, 1, 34.0f);
	P3B_AT("SafeDays", 95, -1, 34.0f); P3B_AT("Inspection", 95, 1, 34.0f); P3B_AT("FireDrill", 99, -1, 34.0f); P3B_AT("SafeDays", 99, 1, 34.0f); P3B_AT("FireDrill", 101, 1, 34.0f);
	P3B_AT("Decon", 103, -1, 34.0f); P3B_AT("Decon", 103, 1, 34.0f); P3B_AT("Inspection", 107, -1, 34.0f); P3B_AT("Inspection", 107, 1, 34.0f); P3B_AT("SafeDays", 111, -1, 34.0f);
	P3B_AT("SafeDays", 111, 1, 34.0f); P3B_AT("FireDrill", 113, 1, 34.0f); P3B_AT("FireDrill", 115, -1, 34.0f); P3B_AT("Decon", 115, 1, 34.0f);
// pipe alley (slots 119 to 148): 4
	P3B_AT("CraneLift", 143, 1, 34.0f); P3B_AT("Capstan", 145, -1, 34.0f); P3B_AT("TugOfWar", 146, 1, 34.0f); P3B_AT("Meeting", 148, -1, 34.0f);
// the loading yards (slots 149 to 175): 18
	P3B_AT("TugOfWar", 149, -1, 34.0f); P3B_AT("Band", 149, 1, 34.0f); P3B_AT("CraneLift", 151, 1, 34.0f); P3B_AT("Meeting", 152, -1, 34.0f); P3B_AT("Band", 155, -1, 34.0f);
	P3B_AT("TugOfWar", 155, 1, 34.0f); P3B_AT("CraneLift", 158, -1, 34.0f); P3B_AT("Meeting", 158, 1, 34.0f); P3B_AT("TugOfWar", 161, -1, 34.0f); P3B_AT("Band", 161, 1, 34.0f);
	P3B_AT("CraneLift", 163, 1, 34.0f); P3B_AT("Meeting", 164, -1, 34.0f); P3B_AT("Band", 166, -1, 34.0f); P3B_AT("TugOfWar", 166, 1, 34.0f); P3B_AT("CraneLift", 170, -1, 34.0f);
	P3B_AT("Meeting", 170, 1, 34.0f); P3B_AT("TugOfWar", 173, -1, 34.0f); P3B_AT("Band", 173, 1, 34.0f);
// the waste run (slots 179 to 209): 28
	P3B_AT("Meeting", 179, 1, 34.0f); P3B_AT("Decon", 180, -1, 34.0f); P3B_AT("Briefing", 181, 1, 34.0f); P3B_AT("FireDrill", 182, -1, 34.0f); P3B_AT("Decon", 183, 1, 34.0f);
	P3B_AT("Meeting", 184, -1, 34.0f); P3B_AT("FireDrill", 185, 1, 34.0f); P3B_AT("Briefing", 186, -1, 34.0f); P3B_AT("Decon", 188, -1, 34.0f); P3B_AT("Meeting", 189, 1, 34.0f);
	P3B_AT("FireDrill", 190, -1, 34.0f); P3B_AT("Briefing", 191, 1, 34.0f); P3B_AT("Meeting", 192, -1, 34.0f); P3B_AT("Decon", 193, 1, 34.0f); P3B_AT("Briefing", 194, -1, 34.0f);
	P3B_AT("Decon", 196, -1, 34.0f); P3B_AT("FireDrill", 197, 1, 34.0f); P3B_AT("FireDrill", 198, -1, 34.0f); P3B_AT("Meeting", 199, 1, 34.0f); P3B_AT("Meeting", 200, -1, 34.0f);
	P3B_AT("Briefing", 201, 1, 34.0f); P3B_AT("Briefing", 202, -1, 34.0f); P3B_AT("Decon", 204, -1, 34.0f); P3B_AT("Decon", 205, 1, 34.0f); P3B_AT("FireDrill", 206, -1, 34.0f);
	P3B_AT("FireDrill", 207, 1, 34.0f); P3B_AT("Meeting", 208, -1, 34.0f); P3B_AT("Meeting", 209, 1, 34.0f);
// the export terminal (slots 210 to 281): 52
	P3B_AT("SafeDays", 210, 1, 34.0f); P3B_AT("Band", 211, -1, 34.0f); P3B_AT("TugOfWar", 213, -1, 34.0f); P3B_AT("Inspection", 215, -1, 34.0f); P3B_AT("CraneLift", 216, 1, 34.0f);
	P3B_AT("SafeDays", 217, -1, 34.0f); P3B_AT("Meeting", 218, 1, 34.0f); P3B_AT("CraneLift", 219, -1, 34.0f); P3B_AT("Meeting", 221, -1, 34.0f); P3B_AT("Band", 223, -1, 34.0f);
	P3B_AT("TugOfWar", 225, -1, 34.0f); P3B_AT("Band", 226, 1, 34.0f); P3B_AT("Inspection", 227, -1, 34.0f); P3B_AT("TugOfWar", 228, 1, 34.0f); P3B_AT("SafeDays", 229, -1, 34.0f);
	P3B_AT("CraneLift", 231, -1, 34.0f); P3B_AT("Meeting", 233, -1, 34.0f); P3B_AT("Inspection", 234, 1, 34.0f); P3B_AT("Band", 235, -1, 34.0f); P3B_AT("SafeDays", 236, 1, 34.0f);
	P3B_AT("TugOfWar", 237, -1, 34.0f); P3B_AT("Inspection", 239, -1, 34.0f); P3B_AT("SafeDays", 241, -1, 34.0f); P3B_AT("CraneLift", 242, 1, 34.0f); P3B_AT("CraneLift", 243, -1, 34.0f);
	P3B_AT("Meeting", 245, -1, 34.0f); P3B_AT("Band", 247, -1, 34.0f); P3B_AT("Meeting", 248, 1, 34.0f); P3B_AT("TugOfWar", 249, -1, 34.0f); P3B_AT("Band", 250, 1, 34.0f);
	P3B_AT("Inspection", 251, -1, 34.0f); P3B_AT("SafeDays", 253, -1, 34.0f); P3B_AT("CraneLift", 255, -1, 34.0f); P3B_AT("TugOfWar", 256, 1, 34.0f); P3B_AT("Meeting", 257, -1, 34.0f);
	P3B_AT("Inspection", 258, 1, 34.0f); P3B_AT("Band", 259, -1, 34.0f); P3B_AT("TugOfWar", 261, -1, 34.0f); P3B_AT("Inspection", 263, -1, 34.0f); P3B_AT("SafeDays", 265, -1, 34.0f);
	P3B_AT("SafeDays", 266, 1, 34.0f); P3B_AT("CraneLift", 267, -1, 34.0f); P3B_AT("CraneLift", 268, 1, 34.0f); P3B_AT("Meeting", 269, -1, 34.0f); P3B_AT("Band", 271, -1, 34.0f);
	P3B_AT("TugOfWar", 273, -1, 34.0f); P3B_AT("Meeting", 274, 1, 34.0f); P3B_AT("Inspection", 275, -1, 34.0f); P3B_AT("Band", 276, 1, 34.0f); P3B_AT("SafeDays", 277, -1, 34.0f);
	P3B_AT("CraneLift", 279, -1, 34.0f); P3B_AT("Meeting", 281, -1, 34.0f);
// the flare avenue (slots 282 to 306): 0
// the cooling field (slots 307 to 384): 37
	P3B_AT("Capstan", 307, -1, 34.0f); P3B_AT("Meeting", 310, 1, 34.0f); P3B_AT("Briefing", 311, -1, 34.0f); P3B_AT("SafeDays", 314, 1, 34.0f); P3B_AT("TugOfWar", 315, -1, 34.0f);
	P3B_AT("Decon", 318, 1, 34.0f); P3B_AT("Meeting", 319, -1, 34.0f); P3B_AT("FireDrill", 322, 1, 34.0f); P3B_AT("SafeDays", 323, -1, 34.0f); P3B_AT("Capstan", 326, 1, 34.0f);
	P3B_AT("Decon", 327, -1, 34.0f); P3B_AT("Briefing", 330, 1, 34.0f); P3B_AT("FireDrill", 331, -1, 34.0f); P3B_AT("TugOfWar", 334, 1, 34.0f); P3B_AT("Capstan", 335, -1, 34.0f);
	P3B_AT("Meeting", 338, 1, 34.0f); P3B_AT("Briefing", 339, -1, 34.0f); P3B_AT("SafeDays", 342, 1, 34.0f); P3B_AT("TugOfWar", 343, -1, 34.0f); P3B_AT("Decon", 346, 1, 34.0f);
	P3B_AT("Meeting", 347, -1, 34.0f); P3B_AT("FireDrill", 350, 1, 34.0f); P3B_AT("SafeDays", 351, -1, 34.0f); P3B_AT("Decon", 355, -1, 34.0f); P3B_AT("FireDrill", 359, -1, 34.0f);
	P3B_AT("Capstan", 362, 1, 34.0f); P3B_AT("Capstan", 363, -1, 34.0f); P3B_AT("Briefing", 366, 1, 34.0f); P3B_AT("Briefing", 367, -1, 34.0f); P3B_AT("TugOfWar", 370, 1, 34.0f);
	P3B_AT("TugOfWar", 371, -1, 34.0f); P3B_AT("Meeting", 374, 1, 34.0f); P3B_AT("Meeting", 375, -1, 34.0f); P3B_AT("SafeDays", 378, 1, 34.0f); P3B_AT("SafeDays", 379, -1, 34.0f);
	P3B_AT("Decon", 382, 1, 34.0f); P3B_AT("Decon", 383, -1, 34.0f);
// tank farm B (slots 385 to 473): 54
	P3B_AT("CraneLift", 385, -1, 34.0f); P3B_AT("Decon", 386, 1, 34.0f); P3B_AT("Band", 389, -1, 34.0f); P3B_AT("SafeDays", 389, 1, 34.0f); P3B_AT("Capstan", 391, -1, 34.0f);
	P3B_AT("FireDrill", 392, 1, 34.0f); P3B_AT("Inspection", 394, -1, 34.0f); P3B_AT("Briefing", 395, 1, 34.0f); P3B_AT("TugOfWar", 397, -1, 34.0f); P3B_AT("Decon", 400, -1, 34.0f);
	P3B_AT("Meeting", 401, 1, 34.0f); P3B_AT("SafeDays", 404, -1, 34.0f); P3B_AT("CraneLift", 404, 1, 34.0f); P3B_AT("FireDrill", 406, -1, 34.0f); P3B_AT("Band", 407, 1, 34.0f);
	P3B_AT("Briefing", 409, -1, 34.0f); P3B_AT("Capstan", 410, 1, 34.0f); P3B_AT("Meeting", 412, -1, 34.0f); P3B_AT("CraneLift", 415, -1, 34.0f); P3B_AT("Inspection", 416, 1, 34.0f);
	P3B_AT("Band", 419, -1, 34.0f); P3B_AT("TugOfWar", 419, 1, 34.0f); P3B_AT("Capstan", 421, -1, 34.0f); P3B_AT("Decon", 422, 1, 34.0f); P3B_AT("Inspection", 424, -1, 34.0f);
	P3B_AT("SafeDays", 425, 1, 34.0f); P3B_AT("TugOfWar", 427, -1, 34.0f); P3B_AT("Decon", 430, -1, 34.0f); P3B_AT("FireDrill", 431, 1, 34.0f); P3B_AT("SafeDays", 434, -1, 34.0f);
	P3B_AT("Briefing", 434, 1, 34.0f); P3B_AT("FireDrill", 436, -1, 34.0f); P3B_AT("Meeting", 437, 1, 34.0f); P3B_AT("Briefing", 439, -1, 34.0f); P3B_AT("CraneLift", 440, 1, 34.0f);
	P3B_AT("Meeting", 442, -1, 34.0f); P3B_AT("CraneLift", 445, -1, 34.0f); P3B_AT("Band", 446, 1, 34.0f); P3B_AT("Band", 449, -1, 34.0f); P3B_AT("Capstan", 449, 1, 34.0f);
	P3B_AT("Capstan", 451, -1, 34.0f); P3B_AT("Inspection", 452, 1, 34.0f); P3B_AT("Inspection", 454, -1, 34.0f); P3B_AT("TugOfWar", 455, 1, 34.0f); P3B_AT("TugOfWar", 457, -1, 34.0f);
	P3B_AT("Decon", 460, -1, 34.0f); P3B_AT("Decon", 461, 1, 34.0f); P3B_AT("SafeDays", 464, -1, 34.0f); P3B_AT("SafeDays", 464, 1, 34.0f); P3B_AT("FireDrill", 466, -1, 34.0f);
	P3B_AT("FireDrill", 467, 1, 34.0f); P3B_AT("Briefing", 469, -1, 34.0f); P3B_AT("Briefing", 470, 1, 34.0f); P3B_AT("Meeting", 472, -1, 34.0f);
// reactor two (slots 474 to 565): 54
	P3B_AT("FireDrill", 475, 1, 34.0f); P3B_AT("Decon", 476, -1, 34.0f); P3B_AT("Inspection", 479, -1, 34.0f); P3B_AT("Briefing", 479, 1, 34.0f); P3B_AT("Meeting", 482, -1, 34.0f);
	P3B_AT("Capstan", 484, 1, 34.0f); P3B_AT("SafeDays", 486, -1, 34.0f); P3B_AT("TugOfWar", 487, 1, 34.0f); P3B_AT("Band", 488, -1, 34.0f); P3B_AT("FireDrill", 491, -1, 34.0f);
	P3B_AT("CraneLift", 491, 1, 34.0f); P3B_AT("Decon", 493, 1, 34.0f); P3B_AT("Briefing", 494, -1, 34.0f); P3B_AT("Inspection", 496, 1, 34.0f); P3B_AT("Capstan", 498, -1, 34.0f);
	P3B_AT("TugOfWar", 500, -1, 34.0f); P3B_AT("Meeting", 500, 1, 34.0f); P3B_AT("CraneLift", 503, -1, 34.0f); P3B_AT("SafeDays", 503, 1, 34.0f); P3B_AT("Decon", 506, -1, 34.0f);
	P3B_AT("Band", 508, 1, 34.0f); P3B_AT("FireDrill", 511, 1, 34.0f); P3B_AT("Inspection", 512, -1, 34.0f); P3B_AT("Meeting", 515, -1, 34.0f); P3B_AT("Briefing", 515, 1, 34.0f);
	P3B_AT("Capstan", 517, 1, 34.0f); P3B_AT("SafeDays", 518, -1, 34.0f); P3B_AT("TugOfWar", 520, 1, 34.0f); P3B_AT("Band", 522, -1, 34.0f); P3B_AT("CraneLift", 523, 1, 34.0f);
	P3B_AT("FireDrill", 524, -1, 34.0f); P3B_AT("Briefing", 527, -1, 34.0f); P3B_AT("Decon", 527, 1, 34.0f); P3B_AT("Capstan", 530, -1, 34.0f); P3B_AT("Inspection", 532, 1, 34.0f);
	P3B_AT("Meeting", 535, 1, 34.0f); P3B_AT("TugOfWar", 536, -1, 34.0f); P3B_AT("CraneLift", 539, -1, 34.0f); P3B_AT("SafeDays", 539, 1, 34.0f); P3B_AT("Decon", 542, -1, 34.0f);
	P3B_AT("Band", 544, 1, 34.0f); P3B_AT("Inspection", 546, -1, 34.0f); P3B_AT("FireDrill", 547, 1, 34.0f); P3B_AT("Meeting", 548, -1, 34.0f); P3B_AT("SafeDays", 551, -1, 34.0f);
	P3B_AT("Briefing", 551, 1, 34.0f); P3B_AT("Capstan", 553, 1, 34.0f); P3B_AT("Band", 554, -1, 34.0f); P3B_AT("TugOfWar", 556, 1, 34.0f); P3B_AT("FireDrill", 558, -1, 34.0f);
	P3B_AT("Briefing", 560, -1, 34.0f); P3B_AT("CraneLift", 560, 1, 34.0f); P3B_AT("Capstan", 563, -1, 34.0f); P3B_AT("Decon", 563, 1, 34.0f);
#undef P3B_AT
#undef P3B
};
PplP3BScenes();
// ---- END p3b ----
}

{
// ---- BEGIN p3c (movers, ft_people_chem_p3c.py) ----
// THE MOVERS (pass 3, builder p3c). Meshes: Content/Python/ft_people_chem_p3c.py -> /Game/People/ChemicalWorks/SM_P3C_*.
// Everything here travels along its own mesh's X (laid along the road), round a point on a tank roof, or up and down a
// mast; nothing crosses the road. Every travel path ends inside a tower, a shed or a station of the piece itself.
//   24 m            SM_P3C_KerbBikes: LOW (top 1.76 m), unlit: two cyclists and a runner on the service path (review round: was 24.2 m,
//                   one zone for the whole road; and the high ways below were given new phases, clear of the film units and the scenes)
//   35.6 m          the works' high ways, all TALL, steady light only (39.8 m in tank farm B, beyond its tank rows):
//                   SM_P3C_Walkers_A / _B (the shift and its drum trolleys on a pipe bridge, decks 14 and 12.5 m up),
//                   SM_P3C_Cyclists_A / _B (works bicycles and a cargo trike on a cycle gallery), SM_P3C_PipeGang (five
//                   carrying one pipe), SM_P3C_FireDrill (the fire team at the run with its hose reel), SM_P3C_ManLift
//                   (two hoist cages riding a 19 m mast, out of step)
//   64 to 72 m      SM_P3C_RailCart: the man-rider train on its trestle, 19 m up
//   84 to 100 m     SM_P3C_TankPatrol: security and the dog walking a tank roof's rim 20 m up, joggers the other way
//   100 to 135 m    SM_P3C_Ropeway: cabins of workers on an aerial ropeway 26 m up
// Estimate by slots (45.24 m; before the track's own clearance test, which takes out what stands beside loops, pipes,
// corkscrews and jump gaps for Scatter rules): 346 pieces; by mesh: Walkers_A 34, Cyclists_A 35, PipeGang 34, ManLift 28, Walkers_B 16, FireDrill 26, Cyclists_B 32, RailCart 31, TankPatrol 30, Ropeway 28, KerbBikes 52.
#define P3C(n) TEXT("/Game/People/ChemicalWorks/SM_P3C_" n)
// the service path, the whole road: LOW and unlit, 24.2 m (23.7 to 24.7: between the kerb stores and the rack), in the
// slots the first round left free on the 24.1 m line (left s % 6 == 5, right s % 6 == 4)
// review round: district by district (the spans of the low lane of p3a: the three wall turns, the mega vat, the jumps and the
// run-in to the finale are left out), in the class of twelve that the kerb line, p3a and the 26 m line leave freest there; 24 m (was 24.2: its back edge stood 0.2 m into the docks)
Zone(TEXT("People p3c: service path, gate"), 0.30f, 1.23f);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 4, 0.55f, 1.f, 0.f, -1);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 0, 0.55f, 1.f, 0.f, 1);
Zone(TEXT("People p3c: service path, tank farm"), 1.23f, 2.60f);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 10, 0.55f, 1.f, 0.f, -1);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 0, 0.55f, 1.f, 0.f, 1);
Zone(TEXT("People p3c: service path, process units, the plateau"), 3.00f, 3.58f);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 5, 0.55f, 1.f, 0.f, -1);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 6, 0.55f, 1.f, 0.f, 1);
Zone(TEXT("People p3c: service path, reactor block, before the jump"), 3.98f, 4.12f);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 7, 0.55f, 1.f, 0.f, -1);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 9, 0.55f, 1.f, 0.f, 1);
Zone(TEXT("People p3c: service path, reactor block"), 4.75f, 5.35f);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 10, 0.55f, 1.f, 0.f, -1);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 0, 0.55f, 1.f, 0.f, 1);
Zone(TEXT("People p3c: service path, pipe alley"), 5.35f, 6.72f);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 4, 0.55f, 1.f, 0.f, -1);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 0, 0.55f, 1.f, 0.f, 1);
Zone(TEXT("People p3c: service path, loading yards"), 6.72f, 7.28f);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 7, 0.55f, 1.f, 0.f, -1);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 9, 0.55f, 1.f, 0.f, 1);
Zone(TEXT("People p3c: service path, waste run"), 8.12f, 9.14f);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 7, 0.55f, 1.f, 0.f, -1);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 9, 0.55f, 1.f, 0.f, 1);
Zone(TEXT("People p3c: service path, export terminal"), 9.46f, 12.72f);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 7, 0.55f, 1.f, 0.f, -1);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 6, 0.55f, 1.f, 0.f, 1);
Zone(TEXT("People p3c: service path, flare avenue"), 12.72f, 13.85f);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 10, 0.55f, 1.f, 0.f, -1);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 0, 0.55f, 1.f, 0.f, 1);
Zone(TEXT("People p3c: service path, cooling field"), 13.85f, 17.40f);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 7, 0.55f, 1.f, 0.f, -1);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 0, 0.55f, 1.f, 0.f, 1);
Zone(TEXT("People p3c: service path, tank farm B"), 17.40f, 21.40f);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 5, 0.55f, 1.f, 0.f, -1);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 0, 0.55f, 1.f, 0.f, 1);
Zone(TEXT("People p3c: service path, reactor two"), 21.40f, 25.60f);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 5, 0.55f, 1.f, 0.f, -1);
	Scatter(P3C("KerbBikes"), 24.f, 24.f, 12, 6, 0.55f, 1.f, 0.f, 1);
// the far ways, the whole road: Far rules with the land's own drop (1.2 m), so they stay beside loops, pipes and jumps
Zone(TEXT("People p3c: far ways"), 0.1f, 99.f);
	Far(P3C("Ropeway"), 100.f, 135.f, 40, 6, 1.f, 1.f, 0.f, -1, 1.2f, 6.f);
	Far(P3C("Ropeway"), 100.f, 135.f, 40, 26, 1.f, 1.f, 0.f, 1, 1.2f, 6.f);
	Far(P3C("RailCart"), 64.f, 72.f, 36, 22, 1.f, 1.f, 0.f, -1, 1.2f, 5.f);
	Far(P3C("RailCart"), 64.f, 72.f, 36, 6, 1.f, 1.f, 0.f, 1, 1.2f, 5.f);
	Far(P3C("TankPatrol"), 84.f, 100.f, 36, 34, 1.f, 1.f, 0.f, -1, 1.2f, 180.f);
	Far(P3C("TankPatrol"), 84.f, 100.f, 36, 10, 1.f, 1.f, 0.f, 1, 1.2f, 180.f);
// gate: behind the fence (27.5 m) and its drums
Zone(TEXT("People p3c: gate"), 0.3f, 1.23f);
	Scatter(P3C("Walkers_A"), 35.6f, 35.6f, 8, 1, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Cyclists_A"), 35.6f, 35.6f, 8, 4, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("PipeGang"), 35.6f, 35.6f, 8, 4, 0.8f, 1.f, 0.f, 1);
	Scatter(P3C("ManLift"), 35.6f, 35.6f, 8, 7, 0.8f, 1.f, 0.f, 1);
// tank farm: behind the tank rows (to 32.3 m); slots clear of the watch towers (34.4 m) and the gantries at 39 m
Zone(TEXT("People p3c: tank farm"), 1.23f, 2.6f);
	Scatter(P3C("Cyclists_B"), 35.6f, 35.6f, 9, 1, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Walkers_A"), 35.6f, 35.6f, 9, 7, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Cyclists_A"), 35.6f, 35.6f, 9, 3, 0.8f, 1.f, 0.f, 1);
	Scatter(P3C("PipeGang"), 35.6f, 35.6f, 9, 6, 0.8f, 1.f, 0.f, 1);
// process units: the plateau between the two wall turns (banked 58: left alone)
Zone(TEXT("People p3c: process units"), 3.f, 3.58f);
	Scatter(P3C("FireDrill"), 35.6f, 35.6f, 6, 1, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Cyclists_B"), 35.6f, 35.6f, 6, 4, 0.8f, 1.f, 0.f, 1);
// reactor block: behind the fence; the track keeps the jump at 4.37 km clear itself
Zone(TEXT("People p3c: reactor block"), 3.98f, 5.35f);
	Scatter(P3C("PipeGang"), 35.6f, 35.6f, 8, 6, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("ManLift"), 35.6f, 35.6f, 8, 2, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Walkers_B"), 35.6f, 35.6f, 8, 6, 0.8f, 1.f, 0.f, 1);
	Scatter(P3C("FireDrill"), 35.6f, 35.6f, 8, 2, 0.8f, 1.f, 0.f, 1);
// pipe alley: behind the rack (to 28.2 m)
Zone(TEXT("People p3c: pipe alley"), 5.35f, 6.72f);
	Scatter(P3C("Cyclists_A"), 35.6f, 35.6f, 8, 2, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("PipeGang"), 35.6f, 35.6f, 8, 6, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("ManLift"), 35.6f, 35.6f, 8, 7, 0.8f, 1.f, 0.f, 1);
	Scatter(P3C("Walkers_B"), 35.6f, 35.6f, 8, 4, 0.8f, 1.f, 0.f, 1);
// loading yards: only the slots with no loading bay: left s % 3 == 1, right s % 3 == 0
Zone(TEXT("People p3c: loading yards"), 6.72f, 7.88f);
	Scatter(P3C("Walkers_A"), 35.6f, 35.6f, 9, 2, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Cyclists_A"), 35.6f, 35.6f, 9, 8, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("PipeGang"), 35.6f, 35.6f, 9, 0, 0.8f, 1.f, 0.f, 1);
	Scatter(P3C("ManLift"), 35.6f, 35.6f, 9, 3, 0.8f, 1.f, 0.f, 1);
// waste run: behind the fence
Zone(TEXT("People p3c: waste run"), 8.12f, 9.46f);
	Scatter(P3C("Cyclists_B"), 35.6f, 35.6f, 8, 1, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Walkers_A"), 35.6f, 35.6f, 8, 3, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Cyclists_A"), 35.6f, 35.6f, 8, 0, 0.8f, 1.f, 0.f, 1);
	Scatter(P3C("PipeGang"), 35.6f, 35.6f, 8, 5, 0.8f, 1.f, 0.f, 1);
// export terminal: behind the rack
Zone(TEXT("People p3c: export terminal"), 9.46f, 12.72f);
	Scatter(P3C("FireDrill"), 35.6f, 35.6f, 12, 0, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Cyclists_B"), 35.6f, 35.6f, 12, 10, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Walkers_A"), 35.6f, 35.6f, 12, 2, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Cyclists_A"), 35.6f, 35.6f, 12, 9, 0.8f, 1.f, 0.f, 1);
	Scatter(P3C("PipeGang"), 35.6f, 35.6f, 12, 5, 0.8f, 1.f, 0.f, 1);
	Scatter(P3C("ManLift"), 35.6f, 35.6f, 12, 1, 0.8f, 1.f, 0.f, 1);
// flare avenue: thinly: the avenue is the flames
Zone(TEXT("People p3c: flare avenue"), 12.72f, 13.85f);
	Scatter(P3C("Cyclists_B"), 35.6f, 35.6f, 12, 4, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Walkers_A"), 35.6f, 35.6f, 12, 0, 0.8f, 1.f, 0.f, 1);
// cooling field: behind the rack; the other lap of the doubled circuit takes out what would touch it
Zone(TEXT("People p3c: cooling field"), 13.85f, 17.4f);
	Scatter(P3C("ManLift"), 35.6f, 35.6f, 12, 11, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Walkers_B"), 35.6f, 35.6f, 12, 8, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("FireDrill"), 35.6f, 35.6f, 12, 4, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Cyclists_B"), 35.6f, 35.6f, 12, 9, 0.8f, 1.f, 0.f, 1);
	Scatter(P3C("Walkers_A"), 35.6f, 35.6f, 12, 5, 0.8f, 1.f, 0.f, 1);
	Scatter(P3C("Cyclists_A"), 35.6f, 35.6f, 12, 1, 0.8f, 1.f, 0.f, 1);
// tank farm B: beyond the tank rows (they stand back here, to 37.8 m); never a slot with s % 3 == 0 (the watchers at 40.8 m)
Zone(TEXT("People p3c: tank farm B"), 17.4f, 21.4f);
	Scatter(P3C("Walkers_B"), 39.8f, 39.8f, 12, 1, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("FireDrill"), 39.8f, 39.8f, 12, 7, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Cyclists_B"), 39.8f, 39.8f, 12, 10, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Walkers_A"), 39.8f, 39.8f, 12, 10, 0.8f, 1.f, 0.f, 1);
	Scatter(P3C("Cyclists_A"), 39.8f, 39.8f, 12, 7, 0.8f, 1.f, 0.f, 1);
	Scatter(P3C("PipeGang"), 39.8f, 39.8f, 12, 1, 0.8f, 1.f, 0.f, 1);
// reactor two: behind the fence
Zone(TEXT("People p3c: reactor two"), 21.4f, 25.6f);
	Scatter(P3C("FireDrill"), 35.6f, 35.6f, 12, 8, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Cyclists_B"), 35.6f, 35.6f, 12, 5, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Walkers_A"), 35.6f, 35.6f, 12, 2, 0.8f, 1.f, 0.f, -1);
	Scatter(P3C("Cyclists_A"), 35.6f, 35.6f, 12, 3, 0.8f, 1.f, 0.f, 1);
	Scatter(P3C("PipeGang"), 35.6f, 35.6f, 12, 7, 0.8f, 1.f, 0.f, 1);
	Scatter(P3C("ManLift"), 35.6f, 35.6f, 12, 0, 0.8f, 1.f, 0.f, 1);
#undef P3C
// ---- END p3c ----
}

{
// ---- BEGIN p3d (heights, ft_people_chem_p3d.py) ----
// PEOPLE UP HIGH (pass 3, builder p3d). Meshes: Content/Python/ft_people_chem_p3d.py -> /Game/People/ChemicalWorks/SM_P3D_*.
// Every piece is TALL and brings its own structure (a stack's gallery, pipe bridges, a tower crane and its man basket,
// a column's ring decks, a gas sphere's crown, a control block's balconies, boom lifts, a silo catwalk, a water tower,
// a hoarding and its painters' cradle): its people stand 8 to 20 m up. Steady light only. Nothing here stands nearer
// than 52 m to the centre line: two lines a district, a near one (52.5 to 58 m) and a back one (60 to 67 m), each on
// slots of its own, chosen against the dressing's sets (see each zone's note) and behind the watchers' 40 m line.
// Written by C:/ftwork/jobs/L1p3d/mkblock.py (a scratch script); its estimate of what is placed is at the end.
#define P3D(n) TEXT("/Game/People/ChemicalWorks/SM_P3D_" n)
// the first 300 m: one each side, behind the shoot and the watchers
Zone(TEXT("People p3d: start"), 0.03f, 0.3f);
	Scatter(P3D("Hoarding"), 54.f, 54.f, 6, 1, 1.f, 1.f, 0.f, -1);
	Scatter(P3D("CraneBasket"), 54.f, 54.f, 6, 4, 1.f, 1.f, 0.f, 1);
// the gate house set stands on slots s % 4 == 3 (and runs on into the next): s % 4 == 1 and 2 here
Zone(TEXT("People p3d: gate"), 0.3f, 1.23f);
	Scatter(P3D("Balconies"), 53.f, 53.f, 4, 3, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("PipeBridge_A"), 63.f, 63.f, 4, 2, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("BoomLifts"), 53.f, 53.f, 4, 2, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("WaterTower"), 63.f, 63.f, 4, 3, 0.9f, 1.f, 0.f, 1);
// the tank farm set (88 m, front at 66) on s % 3 == 0: the back line keeps to the other slots
Zone(TEXT("People p3d: tank farm"), 1.23f, 2.6f);
	Scatter(P3D("SphereCrown"), 53.f, 53.f, 6, 0, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("StackGallery"), 63.f, 63.f, 6, 5, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("ColumnDecks"), 53.f, 53.f, 6, 3, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("SiloWalk"), 63.f, 63.f, 6, 2, 0.9f, 1.f, 0.f, 1);
// the left side is the crown of flare derricks (50 m): right side only. The process unit set on s % 3 == 2 and 0: s % 3 == 1 here
Zone(TEXT("People p3d: process units, first wall turn"), 2.6f, 2.99f);
	Scatter(P3D("PipeBridge_B"), 53.5f, 53.5f, 6, 2, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("StackGallery"), 64.f, 64.f, 6, 5, 0.9f, 1.f, 0.f, 1);
// between the two wall turns: both sides, s % 3 == 1
Zone(TEXT("People p3d: process units"), 2.99f, 3.59f);
	Scatter(P3D("CraneBasket"), 53.5f, 53.5f, 6, 5, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("ColumnDecks"), 64.f, 64.f, 6, 2, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("PipeBridge_B"), 53.5f, 53.5f, 6, 2, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("StackGallery"), 64.f, 64.f, 6, 5, 0.9f, 1.f, 0.f, 1);
// the right side is the second crown of flare derricks: left side only
Zone(TEXT("People p3d: process units, second wall turn"), 3.59f, 3.98f);
	Scatter(P3D("CraneBasket"), 53.5f, 53.5f, 6, 5, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("ColumnDecks"), 64.f, 64.f, 6, 2, 0.9f, 1.f, 0.f, -1);
// the reactor block and cooling field sets on even slots: odd slots here
Zone(TEXT("People p3d: reactor block"), 3.98f, 5.35f);
	Scatter(P3D("Hoarding"), 52.5f, 52.5f, 4, 3, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("WaterTower"), 60.f, 60.f, 4, 1, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("Balconies"), 52.5f, 52.5f, 4, 1, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("CraneBasket"), 60.f, 60.f, 4, 3, 0.9f, 1.f, 0.f, 1);
// the rack lines stand 48 m (left) and 50 m (right) out: behind them; the catwalks (60 to 70 m) on s % 3 == 2
Zone(TEXT("People p3d: pipe alley"), 5.35f, 6.72f);
	Scatter(P3D("PipeBridge_A"), 56.5f, 56.5f, 6, 0, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("SiloWalk"), 66.f, 66.f, 6, 3, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("BoomLifts"), 58.f, 58.f, 6, 3, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("SphereCrown"), 67.f, 67.f, 6, 0, 0.9f, 1.f, 0.f, 1);
// the loading yard and rail yard sets on even slots: odd slots here
Zone(TEXT("People p3d: loading yards"), 6.72f, 7.94f);
	Scatter(P3D("Hoarding"), 53.f, 53.f, 4, 3, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("CraneBasket"), 61.f, 61.f, 4, 1, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("SiloWalk"), 53.f, 53.f, 4, 1, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("PipeBridge_B"), 61.f, 61.f, 4, 3, 0.9f, 1.f, 0.f, 1);
// after the third wall turn's flare. The waste yard set (46 m) on s % 3 == 0: the other slots here
Zone(TEXT("People p3d: waste run"), 8.13f, 9.46f);
	Scatter(P3D("WaterTower"), 53.f, 53.f, 6, 5, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("StackGallery"), 63.f, 63.f, 6, 2, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("Hoarding"), 53.f, 53.f, 6, 4, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("ColumnDecks"), 63.f, 63.f, 6, 1, 0.9f, 1.f, 0.f, 1);
// the yard sets on s % 5 == 0 and 3, the substations on s % 5 == 2: s % 5 == 1 and 4 here
Zone(TEXT("People p3d: export terminal"), 9.46f, 12.72f);
	Scatter(P3D("BoomLifts"), 53.f, 53.f, 10, 9, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("Balconies"), 53.f, 53.f, 10, 4, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("PipeBridge_B"), 62.f, 62.f, 10, 6, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("SphereCrown"), 62.f, 62.f, 10, 1, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("CraneBasket"), 53.f, 53.f, 10, 4, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("SiloWalk"), 53.f, 53.f, 10, 9, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("PipeBridge_A"), 62.f, 62.f, 10, 1, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("WaterTower"), 62.f, 62.f, 10, 6, 0.9f, 1.f, 0.f, 1);
// the flare derricks stand 46 m out on s % 3 == 1: a few bridges only, 62 m out on s % 3 == 0
Zone(TEXT("People p3d: flare avenue"), 12.72f, 13.85f);
	Scatter(P3D("PipeBridge_A"), 62.f, 62.f, 6, 0, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("PipeBridge_B"), 62.f, 62.f, 6, 3, 0.9f, 1.f, 0.f, 1);
// the cooling field set and the 46 to 54 m pipe racks on s % 3 == 0, platforms (60 m and out) on s % 3 == 1: s % 3 == 2 here
Zone(TEXT("People p3d: cooling field"), 13.85f, 17.4f);
	Scatter(P3D("StackGallery"), 54.f, 54.f, 12, 10, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("Hoarding"), 54.f, 54.f, 12, 4, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("ColumnDecks"), 63.f, 63.f, 12, 7, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("CraneBasket"), 63.f, 63.f, 12, 1, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("WaterTower"), 54.f, 54.f, 12, 4, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("BoomLifts"), 54.f, 54.f, 12, 10, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("SiloWalk"), 63.f, 63.f, 12, 1, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("Balconies"), 63.f, 63.f, 12, 7, 0.9f, 1.f, 0.f, 1);
// the tank farm set (90 m, front at 68) on s % 3 == 0: the other slots here
Zone(TEXT("People p3d: tank farm B"), 17.4f, 21.4f);
	Scatter(P3D("SphereCrown"), 53.f, 53.f, 12, 11, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("PipeBridge_B"), 53.f, 53.f, 12, 5, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("SiloWalk"), 62.f, 62.f, 12, 8, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("BoomLifts"), 62.f, 62.f, 12, 2, 0.9f, 1.f, 0.f, -1);
	Scatter(P3D("ColumnDecks"), 53.f, 53.f, 12, 7, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("PipeBridge_A"), 53.f, 53.f, 12, 1, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("Hoarding"), 62.f, 62.f, 12, 10, 0.9f, 1.f, 0.f, 1);
	Scatter(P3D("StackGallery"), 62.f, 62.f, 12, 4, 0.9f, 1.f, 0.f, 1);
// the process unit set (64 m, front at 54) on s % 4 == 2 and 3, the reactor block (front at 62) on s % 4 == 0: near line s % 4 == 0, back line s % 4 == 1
Zone(TEXT("People p3d: reactor two"), 21.4f, 25.6f);
	Scatter(P3D("Balconies"), 52.5f, 52.5f, 8, 0, 0.8f, 1.f, 0.f, -1);
	Scatter(P3D("WaterTower"), 52.5f, 52.5f, 8, 4, 0.8f, 1.f, 0.f, -1);
	Scatter(P3D("PipeBridge_A"), 61.f, 61.f, 8, 7, 0.8f, 1.f, 0.f, -1);
	Scatter(P3D("CraneBasket"), 61.f, 61.f, 8, 3, 0.8f, 1.f, 0.f, -1);
	Scatter(P3D("SphereCrown"), 52.5f, 52.5f, 8, 4, 0.8f, 1.f, 0.f, 1);
	Scatter(P3D("BoomLifts"), 52.5f, 52.5f, 8, 0, 0.8f, 1.f, 0.f, 1);
	Scatter(P3D("ColumnDecks"), 61.f, 61.f, 8, 3, 0.8f, 1.f, 0.f, 1);
	Scatter(P3D("PipeBridge_B"), 61.f, 61.f, 8, 7, 0.8f, 1.f, 0.f, 1);
#undef P3D
// estimate (slots counted, times chance; before the track leaves pieces out of loops, pipes, jumps and the doubled circuit): 374 pieces; by mesh: Balconies 31, BoomLifts 37, ColumnDecks 34, CraneBasket 40, Hoarding 32, PipeBridge_A 35, PipeBridge_B 35, SiloWalk 36, SphereCrown 32, StackGallery 24, WaterTower 38
// ---- END p3d ----
}

{
// ---- BEGIN p3e (far and sky, ft_people_chem_p3e.py) ----
// FAR AND SKY of the Chemical Works (pass 3, builder p3e, 15 Nov 2026). Meshes: Content/Python/ft_people_chem_p3e.py ->
// /Game/People/ChemicalWorks/SM_P3E_*. Nothing here stands nearer than 150 m; every piece is TALL (its life is 45 to 160 m
// up, over the racks and tank rows at the sight line), carries a steady light of its own (nothing blinks) and was saved
// with the always-moving master (far=True).
//   the sky, the whole road (Far, no drop: they stay beside loops, pipes and jumps), on ODD slots:
//     215 to 340 m    SM_P3E_Balloons_A / _B: hot-air balloons drifting in circles of 27 to 30 m, 70 to 140 m up
//     280 to 430 m    SM_P3E_Blimp: the film unit's airship crossing along the road 120 m up (400 m and round again)
//   the standing pieces, district by district (six of the nine a district, in a cycle of 24 slots: left on slots
//   s % 4 == 0, right on s % 4 == 2, so on one side one of them every 181 m and never two in one slot):
//     150 to 215 m    SM_P3E_FarCrowd_A / _B (the shift's far terrace) and SM_P3E_Kites (the gasholder's roof party):
//                     Scatters, they stand on the land (the plateau, the ramps) and the track keeps them out of loops
//     170 to 285 m    SM_P3E_Steeplejacks, SM_P3E_Cableway, SM_P3E_Viaduct, SM_P3E_GullTower, SM_P3E_Lanterns,
//                     SM_P3E_TowerRim: Far rules, 1.5 m under the road's level like the land there
//   Left out on purpose: 5.05 to 5.35 km and 21.0 to 21.5 km (the two reactor cores stand 134 and 150 m to the left
//   there), the mega vat (7.94 to 8.09 km) and the flare avenue (12.72 to 13.85 km: left to its flames; the sky's
//   pieces still pass). The finale's second lap (reactor two, from 21.5 km) runs a few metres beside the cooling
//   field's road, so its standing pieces stand 85 m further out than the first lap's.
// Estimate (slots counted by C:/ftwork/jobs/L1p3e/mkblock.py, a scratch script, before the track's own clearance test):
// 398 pieces: Balloons_A 47, Balloons_B 47, Blimp 47, Cableway 25, FarCrowd_A 27, FarCrowd_B 27, GullTower 31,
//   Kites 31, Lanterns 29, Steeplejacks 28, TowerRim 33, Viaduct 26
#define P3E(n) TEXT("/Game/People/ChemicalWorks/SM_P3E_" n)
// A lambda of its own (as the watch block and the dressing table have), so the level population function stays small.
auto PplP3E = [&]()
{
Zone(TEXT("People p3e: the sky"), 0.05f, 99.f);
	Far(P3E("Balloons_A"), 215.f, 340.f, 24, 1, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(P3E("Balloons_A"), 215.f, 340.f, 24, 13, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 215.f, 340.f, 24, 9, 1.f, 1.f, 0.f, -1, 0.f, 180.f);
	Far(P3E("Balloons_B"), 215.f, 340.f, 24, 21, 1.f, 1.f, 0.f, 1, 0.f, 180.f);
	Far(P3E("Blimp"), 280.f, 430.f, 24, 17, 1.f, 1.f, 0.f, -1, 0.f, 12.f);
	Far(P3E("Blimp"), 280.f, 430.f, 24, 5, 1.f, 1.f, 0.f, 1, 0.f, 12.f);
Zone(TEXT("People p3e: gate"), 0.10f, 1.23f);
	Far(P3E("Steeplejacks"), 170.f, 240.f, 24, 0, 1.f, 1.f, 0.f, -1, 1.5f, 15.f);
	Scatter(P3E("FarCrowd_A"), 150.f, 205.f, 24, 20, 1.f, 1.f, 0.f, -1, 8.f);
	Far(P3E("Viaduct"), 185.f, 250.f, 24, 16, 1.f, 1.f, 0.f, -1, 1.5f, 6.f);
	Scatter(P3E("Kites"), 215.f, 270.f, 24, 12, 1.f, 1.f, 0.f, -1, 10.f);
	Far(P3E("TowerRim"), 205.f, 285.f, 24, 8, 1.f, 1.f, 0.f, -1, 1.5f, 20.f);
	Far(P3E("Lanterns"), 195.f, 260.f, 24, 4, 1.f, 1.f, 0.f, -1, 1.5f, 12.f);
	Far(P3E("Lanterns"), 195.f, 260.f, 24, 22, 1.f, 1.f, 0.f, 1, 1.5f, 12.f);
	Scatter(P3E("FarCrowd_B"), 150.f, 205.f, 24, 18, 1.f, 1.f, 0.f, 1, 8.f);
	Far(P3E("Cableway"), 175.f, 240.f, 24, 14, 1.f, 1.f, 0.f, 1, 1.5f, 6.f);
	Far(P3E("GullTower"), 190.f, 260.f, 24, 10, 1.f, 1.f, 0.f, 1, 1.5f, 180.f);
	Far(P3E("Steeplejacks"), 170.f, 240.f, 24, 6, 1.f, 1.f, 0.f, 1, 1.5f, 15.f);
	Scatter(P3E("FarCrowd_A"), 150.f, 205.f, 24, 2, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People p3e: tank farm"), 1.23f, 2.60f);
	Scatter(P3E("Kites"), 215.f, 270.f, 24, 0, 1.f, 1.f, 0.f, -1, 10.f);
	Far(P3E("TowerRim"), 205.f, 285.f, 24, 20, 1.f, 1.f, 0.f, -1, 1.5f, 20.f);
	Far(P3E("Lanterns"), 195.f, 260.f, 24, 16, 1.f, 1.f, 0.f, -1, 1.5f, 12.f);
	Scatter(P3E("FarCrowd_B"), 150.f, 205.f, 24, 12, 1.f, 1.f, 0.f, -1, 8.f);
	Far(P3E("Cableway"), 175.f, 240.f, 24, 8, 1.f, 1.f, 0.f, -1, 1.5f, 6.f);
	Far(P3E("GullTower"), 190.f, 260.f, 24, 4, 1.f, 1.f, 0.f, -1, 1.5f, 180.f);
	Far(P3E("GullTower"), 190.f, 260.f, 24, 22, 1.f, 1.f, 0.f, 1, 1.5f, 180.f);
	Far(P3E("Steeplejacks"), 170.f, 240.f, 24, 18, 1.f, 1.f, 0.f, 1, 1.5f, 15.f);
	Scatter(P3E("FarCrowd_A"), 150.f, 205.f, 24, 14, 1.f, 1.f, 0.f, 1, 8.f);
	Far(P3E("Viaduct"), 185.f, 250.f, 24, 10, 1.f, 1.f, 0.f, 1, 1.5f, 6.f);
	Scatter(P3E("Kites"), 215.f, 270.f, 24, 6, 1.f, 1.f, 0.f, 1, 10.f);
	Far(P3E("TowerRim"), 205.f, 285.f, 24, 2, 1.f, 1.f, 0.f, 1, 1.5f, 20.f);
Zone(TEXT("People p3e: process units"), 2.60f, 3.98f);
	Scatter(P3E("FarCrowd_B"), 150.f, 205.f, 24, 0, 1.f, 1.f, 0.f, -1, 8.f);
	Far(P3E("Cableway"), 175.f, 240.f, 24, 20, 1.f, 1.f, 0.f, -1, 1.5f, 6.f);
	Far(P3E("GullTower"), 190.f, 260.f, 24, 16, 1.f, 1.f, 0.f, -1, 1.5f, 180.f);
	Far(P3E("Steeplejacks"), 170.f, 240.f, 24, 12, 1.f, 1.f, 0.f, -1, 1.5f, 15.f);
	Scatter(P3E("FarCrowd_A"), 150.f, 205.f, 24, 8, 1.f, 1.f, 0.f, -1, 8.f);
	Far(P3E("Viaduct"), 185.f, 250.f, 24, 4, 1.f, 1.f, 0.f, -1, 1.5f, 6.f);
	Far(P3E("Viaduct"), 185.f, 250.f, 24, 22, 1.f, 1.f, 0.f, 1, 1.5f, 6.f);
	Scatter(P3E("Kites"), 215.f, 270.f, 24, 18, 1.f, 1.f, 0.f, 1, 10.f);
	Far(P3E("TowerRim"), 205.f, 285.f, 24, 14, 1.f, 1.f, 0.f, 1, 1.5f, 20.f);
	Far(P3E("Lanterns"), 195.f, 260.f, 24, 10, 1.f, 1.f, 0.f, 1, 1.5f, 12.f);
	Scatter(P3E("FarCrowd_B"), 150.f, 205.f, 24, 6, 1.f, 1.f, 0.f, 1, 8.f);
	Far(P3E("Cableway"), 175.f, 240.f, 24, 2, 1.f, 1.f, 0.f, 1, 1.5f, 6.f);
Zone(TEXT("People p3e: reactor block"), 3.98f, 5.05f);
	Far(P3E("Steeplejacks"), 170.f, 240.f, 24, 0, 1.f, 1.f, 0.f, -1, 1.5f, 15.f);
	Scatter(P3E("FarCrowd_A"), 150.f, 205.f, 24, 20, 1.f, 1.f, 0.f, -1, 8.f);
	Far(P3E("Viaduct"), 185.f, 250.f, 24, 16, 1.f, 1.f, 0.f, -1, 1.5f, 6.f);
	Scatter(P3E("Kites"), 215.f, 270.f, 24, 12, 1.f, 1.f, 0.f, -1, 10.f);
	Far(P3E("TowerRim"), 205.f, 285.f, 24, 8, 1.f, 1.f, 0.f, -1, 1.5f, 20.f);
	Far(P3E("Lanterns"), 195.f, 260.f, 24, 4, 1.f, 1.f, 0.f, -1, 1.5f, 12.f);
	Far(P3E("Lanterns"), 195.f, 260.f, 24, 22, 1.f, 1.f, 0.f, 1, 1.5f, 12.f);
	Scatter(P3E("FarCrowd_B"), 150.f, 205.f, 24, 18, 1.f, 1.f, 0.f, 1, 8.f);
	Far(P3E("Cableway"), 175.f, 240.f, 24, 14, 1.f, 1.f, 0.f, 1, 1.5f, 6.f);
	Far(P3E("GullTower"), 190.f, 260.f, 24, 10, 1.f, 1.f, 0.f, 1, 1.5f, 180.f);
	Far(P3E("Steeplejacks"), 170.f, 240.f, 24, 6, 1.f, 1.f, 0.f, 1, 1.5f, 15.f);
	Scatter(P3E("FarCrowd_A"), 150.f, 205.f, 24, 2, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People p3e: pipe alley"), 5.35f, 6.72f);
	Scatter(P3E("Kites"), 215.f, 270.f, 24, 0, 1.f, 1.f, 0.f, -1, 10.f);
	Far(P3E("TowerRim"), 205.f, 285.f, 24, 20, 1.f, 1.f, 0.f, -1, 1.5f, 20.f);
	Far(P3E("Lanterns"), 195.f, 260.f, 24, 16, 1.f, 1.f, 0.f, -1, 1.5f, 12.f);
	Scatter(P3E("FarCrowd_B"), 150.f, 205.f, 24, 12, 1.f, 1.f, 0.f, -1, 8.f);
	Far(P3E("Cableway"), 175.f, 240.f, 24, 8, 1.f, 1.f, 0.f, -1, 1.5f, 6.f);
	Far(P3E("GullTower"), 190.f, 260.f, 24, 4, 1.f, 1.f, 0.f, -1, 1.5f, 180.f);
	Far(P3E("GullTower"), 190.f, 260.f, 24, 22, 1.f, 1.f, 0.f, 1, 1.5f, 180.f);
	Far(P3E("Steeplejacks"), 170.f, 240.f, 24, 18, 1.f, 1.f, 0.f, 1, 1.5f, 15.f);
	Scatter(P3E("FarCrowd_A"), 150.f, 205.f, 24, 14, 1.f, 1.f, 0.f, 1, 8.f);
	Far(P3E("Viaduct"), 185.f, 250.f, 24, 10, 1.f, 1.f, 0.f, 1, 1.5f, 6.f);
	Scatter(P3E("Kites"), 215.f, 270.f, 24, 6, 1.f, 1.f, 0.f, 1, 10.f);
	Far(P3E("TowerRim"), 205.f, 285.f, 24, 2, 1.f, 1.f, 0.f, 1, 1.5f, 20.f);
Zone(TEXT("People p3e: loading yards"), 6.72f, 7.94f);
	Scatter(P3E("FarCrowd_B"), 150.f, 205.f, 24, 0, 1.f, 1.f, 0.f, -1, 8.f);
	Far(P3E("Cableway"), 175.f, 240.f, 24, 20, 1.f, 1.f, 0.f, -1, 1.5f, 6.f);
	Far(P3E("GullTower"), 190.f, 260.f, 24, 16, 1.f, 1.f, 0.f, -1, 1.5f, 180.f);
	Far(P3E("Steeplejacks"), 170.f, 240.f, 24, 12, 1.f, 1.f, 0.f, -1, 1.5f, 15.f);
	Scatter(P3E("FarCrowd_A"), 150.f, 205.f, 24, 8, 1.f, 1.f, 0.f, -1, 8.f);
	Far(P3E("Viaduct"), 185.f, 250.f, 24, 4, 1.f, 1.f, 0.f, -1, 1.5f, 6.f);
	Far(P3E("Viaduct"), 185.f, 250.f, 24, 22, 1.f, 1.f, 0.f, 1, 1.5f, 6.f);
	Scatter(P3E("Kites"), 215.f, 270.f, 24, 18, 1.f, 1.f, 0.f, 1, 10.f);
	Far(P3E("TowerRim"), 205.f, 285.f, 24, 14, 1.f, 1.f, 0.f, 1, 1.5f, 20.f);
	Far(P3E("Lanterns"), 195.f, 260.f, 24, 10, 1.f, 1.f, 0.f, 1, 1.5f, 12.f);
	Scatter(P3E("FarCrowd_B"), 150.f, 205.f, 24, 6, 1.f, 1.f, 0.f, 1, 8.f);
	Far(P3E("Cableway"), 175.f, 240.f, 24, 2, 1.f, 1.f, 0.f, 1, 1.5f, 6.f);
Zone(TEXT("People p3e: waste run"), 8.09f, 9.46f);
	Far(P3E("Steeplejacks"), 170.f, 240.f, 24, 0, 1.f, 1.f, 0.f, -1, 1.5f, 15.f);
	Scatter(P3E("FarCrowd_A"), 150.f, 205.f, 24, 20, 1.f, 1.f, 0.f, -1, 8.f);
	Far(P3E("Viaduct"), 185.f, 250.f, 24, 16, 1.f, 1.f, 0.f, -1, 1.5f, 6.f);
	Scatter(P3E("Kites"), 215.f, 270.f, 24, 12, 1.f, 1.f, 0.f, -1, 10.f);
	Far(P3E("TowerRim"), 205.f, 285.f, 24, 8, 1.f, 1.f, 0.f, -1, 1.5f, 20.f);
	Far(P3E("Lanterns"), 195.f, 260.f, 24, 4, 1.f, 1.f, 0.f, -1, 1.5f, 12.f);
	Far(P3E("Lanterns"), 195.f, 260.f, 24, 22, 1.f, 1.f, 0.f, 1, 1.5f, 12.f);
	Scatter(P3E("FarCrowd_B"), 150.f, 205.f, 24, 18, 1.f, 1.f, 0.f, 1, 8.f);
	Far(P3E("Cableway"), 175.f, 240.f, 24, 14, 1.f, 1.f, 0.f, 1, 1.5f, 6.f);
	Far(P3E("GullTower"), 190.f, 260.f, 24, 10, 1.f, 1.f, 0.f, 1, 1.5f, 180.f);
	Far(P3E("Steeplejacks"), 170.f, 240.f, 24, 6, 1.f, 1.f, 0.f, 1, 1.5f, 15.f);
	Scatter(P3E("FarCrowd_A"), 150.f, 205.f, 24, 2, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People p3e: export terminal"), 9.46f, 12.72f);
	// review round: the lanterns and the kites are left out of the finale (they piled up in the sky over the boss), the gulls stand 250 m and more out
	Far(P3E("TowerRim"), 205.f, 285.f, 24, 20, 1.f, 1.f, 0.f, -1, 1.5f, 20.f);
	Scatter(P3E("FarCrowd_B"), 150.f, 205.f, 24, 12, 1.f, 1.f, 0.f, -1, 8.f);
	Far(P3E("Cableway"), 175.f, 240.f, 24, 8, 1.f, 1.f, 0.f, -1, 1.5f, 6.f);
	Far(P3E("GullTower"), 250.f, 320.f, 24, 4, 1.f, 1.f, 0.f, -1, 1.5f, 180.f);
	Far(P3E("GullTower"), 250.f, 320.f, 24, 22, 1.f, 1.f, 0.f, 1, 1.5f, 180.f);
	Far(P3E("Steeplejacks"), 170.f, 240.f, 24, 18, 1.f, 1.f, 0.f, 1, 1.5f, 15.f);
	Scatter(P3E("FarCrowd_A"), 150.f, 205.f, 24, 14, 1.f, 1.f, 0.f, 1, 8.f);
	Far(P3E("Viaduct"), 185.f, 250.f, 24, 10, 1.f, 1.f, 0.f, 1, 1.5f, 6.f);
	Far(P3E("TowerRim"), 205.f, 285.f, 24, 2, 1.f, 1.f, 0.f, 1, 1.5f, 20.f);
Zone(TEXT("People p3e: cooling field"), 13.85f, 17.40f);
	Scatter(P3E("FarCrowd_B"), 150.f, 205.f, 24, 0, 1.f, 1.f, 0.f, -1, 8.f);
	Far(P3E("Cableway"), 175.f, 240.f, 24, 20, 1.f, 1.f, 0.f, -1, 1.5f, 6.f);
	Far(P3E("GullTower"), 190.f, 260.f, 24, 16, 1.f, 1.f, 0.f, -1, 1.5f, 180.f);
	Far(P3E("Steeplejacks"), 170.f, 240.f, 24, 12, 1.f, 1.f, 0.f, -1, 1.5f, 15.f);
	Scatter(P3E("FarCrowd_A"), 150.f, 205.f, 24, 8, 1.f, 1.f, 0.f, -1, 8.f);
	Far(P3E("Viaduct"), 185.f, 250.f, 24, 4, 1.f, 1.f, 0.f, -1, 1.5f, 6.f);
	Far(P3E("Viaduct"), 185.f, 250.f, 24, 22, 1.f, 1.f, 0.f, 1, 1.5f, 6.f);
	Scatter(P3E("Kites"), 215.f, 270.f, 24, 18, 1.f, 1.f, 0.f, 1, 10.f);
	Far(P3E("TowerRim"), 205.f, 285.f, 24, 14, 1.f, 1.f, 0.f, 1, 1.5f, 20.f);
	Far(P3E("Lanterns"), 195.f, 260.f, 24, 10, 1.f, 1.f, 0.f, 1, 1.5f, 12.f);
	Scatter(P3E("FarCrowd_B"), 150.f, 205.f, 24, 6, 1.f, 1.f, 0.f, 1, 8.f);
	Far(P3E("Cableway"), 175.f, 240.f, 24, 2, 1.f, 1.f, 0.f, 1, 1.5f, 6.f);
Zone(TEXT("People p3e: tank farm B"), 17.40f, 21.00f);
	Far(P3E("Steeplejacks"), 170.f, 240.f, 24, 0, 1.f, 1.f, 0.f, -1, 1.5f, 15.f);
	Scatter(P3E("FarCrowd_A"), 150.f, 205.f, 24, 20, 1.f, 1.f, 0.f, -1, 8.f);
	Far(P3E("Viaduct"), 185.f, 250.f, 24, 16, 1.f, 1.f, 0.f, -1, 1.5f, 6.f);
	Scatter(P3E("Kites"), 215.f, 270.f, 24, 12, 1.f, 1.f, 0.f, -1, 10.f);
	Far(P3E("TowerRim"), 205.f, 285.f, 24, 8, 1.f, 1.f, 0.f, -1, 1.5f, 20.f);
	Far(P3E("Lanterns"), 195.f, 260.f, 24, 4, 1.f, 1.f, 0.f, -1, 1.5f, 12.f);
	Far(P3E("Lanterns"), 195.f, 260.f, 24, 22, 1.f, 1.f, 0.f, 1, 1.5f, 12.f);
	Scatter(P3E("FarCrowd_B"), 150.f, 205.f, 24, 18, 1.f, 1.f, 0.f, 1, 8.f);
	Far(P3E("Cableway"), 175.f, 240.f, 24, 14, 1.f, 1.f, 0.f, 1, 1.5f, 6.f);
	Far(P3E("GullTower"), 190.f, 260.f, 24, 10, 1.f, 1.f, 0.f, 1, 1.5f, 180.f);
	Far(P3E("Steeplejacks"), 170.f, 240.f, 24, 6, 1.f, 1.f, 0.f, 1, 1.5f, 15.f);
	Scatter(P3E("FarCrowd_A"), 150.f, 205.f, 24, 2, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People p3e: reactor two"), 21.50f, 25.60f);
	Scatter(P3E("Kites"), 235.f, 292.f, 24, 0, 1.f, 1.f, 0.f, -1, 10.f);
	Far(P3E("TowerRim"), 290.f, 370.f, 24, 20, 1.f, 1.f, 0.f, -1, 1.5f, 20.f);
	Far(P3E("Lanterns"), 280.f, 345.f, 24, 16, 1.f, 1.f, 0.f, -1, 1.5f, 12.f);
	Scatter(P3E("FarCrowd_B"), 235.f, 290.f, 24, 12, 1.f, 1.f, 0.f, -1, 8.f);
	Far(P3E("Cableway"), 260.f, 325.f, 24, 8, 1.f, 1.f, 0.f, -1, 1.5f, 6.f);
	Far(P3E("GullTower"), 275.f, 345.f, 24, 4, 1.f, 1.f, 0.f, -1, 1.5f, 180.f);
	Far(P3E("GullTower"), 275.f, 345.f, 24, 22, 1.f, 1.f, 0.f, 1, 1.5f, 180.f);
	Far(P3E("Steeplejacks"), 255.f, 325.f, 24, 18, 1.f, 1.f, 0.f, 1, 1.5f, 15.f);
	Scatter(P3E("FarCrowd_A"), 235.f, 290.f, 24, 14, 1.f, 1.f, 0.f, 1, 8.f);
	Far(P3E("Viaduct"), 270.f, 335.f, 24, 10, 1.f, 1.f, 0.f, 1, 1.5f, 6.f);
	Scatter(P3E("Kites"), 235.f, 292.f, 24, 6, 1.f, 1.f, 0.f, 1, 10.f);
	Far(P3E("TowerRim"), 290.f, 370.f, 24, 2, 1.f, 1.f, 0.f, 1, 1.5f, 20.f);
};
PplP3E();
#undef P3E
// ---- END p3e ----
}

{
// ---- BEGIN p3f (film shoot, ft_people_chem_p3f.py) ----
// MORE OF THE SHOOT (pass 3, builder p3f). Meshes: Content/Python/ft_people_chem_p3f.py -> /Game/People/ChemicalWorks/SM_P3F_*
// (second unit, drone team, stunt rigging, extras' holding, make-up, press pen, camera tower, lighting balloon: every one
// TALL, its people on decks 2.4 m and more up, steady light only) and more of the kit /Game/People/Crew/SM_Crew<Piece>_Work.
// Every rule stands 26.8 m or more from the centre line (x min >= 24 for all: all of these meshes are TALL or placed as if).
// The slots were chosen against the dressing (rack 26.5 m, tank rows 28.5 / 34 m, bays 36.5 m) and the first round's
// people (26, 30.3, 34.4 m) and watchers (40 to 40.8 m): each rule takes a slot class (every, phase) that those leave free
// on its side. The zones leave out the wall bends, the two jumps and the mega vat (the dressing's own "Ground:" spans).
// Written by C:/ftwork/jobs/L1p3f/mkblock.py (scratch). Estimated placed: 346 (before the track's own clearance).
#define P3F(n) TEXT("/Game/People/ChemicalWorks/SM_P3F_" n)
#define P3FK(n) TEXT("/Game/People/Crew/SM_Crew" n "_Work")
// the gate: behind the fence (27.5 m), in the slots with neither rack nor tank row (left s % 3 == 2, right s % 3 == 1)
Zone(TEXT("People p3f: gate"), 0.05f, 1.24f);
	Scatter(P3F("SecondUnit_A"), 31.0f, 31.0f, 12, 1, 1.f, 1.f, 0.f, -1);
	Scatter(P3FK("Crane"), 31.0f, 31.0f, 12, 4, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("Extras_A"), 31.0f, 31.0f, 12, 7, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("DroneTeam_A"), 31.0f, 31.0f, 12, 10, 1.f, 1.f, 0.f, -1);
	Scatter(P3FK("Lights"), 31.0f, 31.0f, 12, 2, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("PressPen"), 31.0f, 31.0f, 12, 5, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("StuntRig_A"), 31.0f, 31.0f, 12, 8, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("MakeUp"), 31.0f, 31.0f, 12, 11, 1.f, 1.f, 0.f, 1);
// the tank farm: behind the tank rows (to 32.3 m, 11 m tall): only the units that reach over them
Zone(TEXT("People p3f: tank farm"), 1.40f, 2.04f);
	Scatter(P3F("DroneTeam_A"), 36.3f, 36.3f, 9, 1, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("CamTower"), 36.3f, 36.3f, 9, 4, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("LightBalloon"), 36.3f, 36.3f, 9, 7, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("StuntRig_A"), 36.3f, 36.3f, 9, 0, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("DroneTeam_B"), 36.3f, 36.3f, 9, 3, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("StuntRig_B"), 36.3f, 36.3f, 9, 6, 1.f, 1.f, 0.f, 1);
// the tank farm: behind the tank rows (to 32.3 m, 11 m tall): only the units that reach over them
Zone(TEXT("People p3f: tank farm, low"), 2.19f, 2.60f);
	Scatter(P3F("DroneTeam_A"), 36.3f, 36.3f, 9, 1, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("CamTower"), 36.3f, 36.3f, 9, 4, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("LightBalloon"), 36.3f, 36.3f, 9, 7, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("StuntRig_A"), 36.3f, 36.3f, 9, 0, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("DroneTeam_B"), 36.3f, 36.3f, 9, 3, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("StuntRig_B"), 36.3f, 36.3f, 9, 6, 1.f, 1.f, 0.f, 1);
// the process units, between the wall bends: behind the rack, in the slots the scaffolds and towers leave
Zone(TEXT("People p3f: process units"), 2.93f, 3.60f);
	Scatter(P3FK("Sound"), 30.3f, 30.3f, 9, 2, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("CamTower"), 30.3f, 30.3f, 9, 5, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("SecondUnit_B"), 30.3f, 30.3f, 9, 8, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("Extras_B"), 30.3f, 30.3f, 9, 0, 1.f, 1.f, 0.f, 1);
	Scatter(P3FK("Catering"), 30.3f, 30.3f, 9, 3, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("LightBalloon"), 30.3f, 30.3f, 9, 6, 1.f, 1.f, 0.f, 1);
// the reactor block: behind the fence, slots with neither rack (31.5 m) nor tanks (34 m); the first jump is left out
Zone(TEXT("People p3f: reactor plain"), 3.98f, 4.26f);
	Scatter(P3F("StuntRig_B"), 31.0f, 31.0f, 12, 3, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("DroneTeam_B"), 31.0f, 31.0f, 12, 7, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("SecondUnit_A"), 31.0f, 31.0f, 12, 11, 1.f, 1.f, 0.f, -1);
	Scatter(P3FK("Crane"), 31.0f, 31.0f, 12, 2, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("Extras_A"), 31.0f, 31.0f, 12, 6, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("DroneTeam_A"), 31.0f, 31.0f, 12, 10, 1.f, 1.f, 0.f, 1);
// the reactor block: behind the fence, slots with neither rack (31.5 m) nor tanks (34 m); the first jump is left out
Zone(TEXT("People p3f: reactor block"), 4.55f, 5.35f);
	Scatter(P3FK("Lights"), 31.0f, 31.0f, 12, 3, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("PressPen"), 31.0f, 31.0f, 12, 7, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("StuntRig_A"), 31.0f, 31.0f, 12, 11, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("MakeUp"), 31.0f, 31.0f, 12, 2, 1.f, 1.f, 0.f, 1);
	Scatter(P3FK("Sound"), 31.0f, 31.0f, 12, 6, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("CamTower"), 31.0f, 31.0f, 12, 10, 1.f, 1.f, 0.f, 1);
// pipe alley: behind the rack
Zone(TEXT("People p3f: pipe alley"), 5.35f, 6.43f);
	Scatter(P3F("SecondUnit_B"), 30.3f, 30.3f, 12, 2, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("Extras_B"), 30.3f, 30.3f, 12, 5, 1.f, 1.f, 0.f, -1);
	Scatter(P3FK("Catering"), 30.3f, 30.3f, 12, 8, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("LightBalloon"), 30.3f, 30.3f, 12, 11, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("StuntRig_B"), 30.3f, 30.3f, 12, 1, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("DroneTeam_B"), 30.3f, 30.3f, 12, 4, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("SecondUnit_A"), 30.3f, 30.3f, 12, 7, 1.f, 1.f, 0.f, 1);
	Scatter(P3FK("Crane"), 30.3f, 30.3f, 12, 10, 1.f, 1.f, 0.f, 1);
// the loading yards, up to the second jump: slots with neither bay nor tanks, behind the dock crowds
Zone(TEXT("People p3f: loading yards"), 6.72f, 7.32f);
	Scatter(P3F("Extras_A"), 31.5f, 31.5f, 6, 2, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("DroneTeam_A"), 31.5f, 31.5f, 6, 5, 1.f, 1.f, 0.f, -1);
	Scatter(P3FK("Lights"), 31.5f, 31.5f, 6, 0, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("PressPen"), 31.5f, 31.5f, 6, 3, 1.f, 1.f, 0.f, 1);
// the waste run: behind the fence, the slots without a tank row (left odd, right even); the base camp before the finale
Zone(TEXT("People p3f: waste run"), 8.09f, 9.46f);
	Scatter(P3F("StuntRig_A"), 31.0f, 31.0f, 12, 1, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("MakeUp"), 31.0f, 31.0f, 12, 3, 1.f, 1.f, 0.f, -1);
	Scatter(P3FK("Sound"), 31.0f, 31.0f, 12, 5, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("CamTower"), 31.0f, 31.0f, 12, 7, 1.f, 1.f, 0.f, -1);
	Scatter(P3FK("Camera"), 31.0f, 31.0f, 12, 9, 1.f, 1.f, 0.f, -1);
	Scatter(P3FK("Stunt"), 31.0f, 31.0f, 12, 11, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("SecondUnit_B"), 31.0f, 31.0f, 12, 0, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("Extras_B"), 31.0f, 31.0f, 12, 2, 1.f, 1.f, 0.f, 1);
	Scatter(P3FK("Catering"), 31.0f, 31.0f, 12, 4, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("LightBalloon"), 31.0f, 31.0f, 12, 6, 1.f, 1.f, 0.f, 1);
	Scatter(P3FK("Village"), 31.0f, 31.0f, 12, 8, 1.f, 1.f, 0.f, 1);
	Scatter(P3FK("Dolly"), 31.0f, 31.0f, 12, 10, 1.f, 1.f, 0.f, 1);
// the export terminal: behind the rack, the one slot in eight a side that the scaffolds and towers leave
Zone(TEXT("People p3f: export terminal"), 9.46f, 12.72f);
	Scatter(P3F("StuntRig_B"), 30.3f, 30.3f, 24, 2, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("DroneTeam_B"), 30.3f, 30.3f, 24, 10, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("SecondUnit_A"), 30.3f, 30.3f, 24, 18, 1.f, 1.f, 0.f, -1);
	Scatter(P3FK("Crane"), 30.3f, 30.3f, 24, 7, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("Extras_A"), 30.3f, 30.3f, 24, 15, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("DroneTeam_A"), 30.3f, 30.3f, 24, 23, 1.f, 1.f, 0.f, 1);
// the flare avenue: a thin line behind the rack
Zone(TEXT("People p3f: flare avenue"), 12.72f, 13.85f);
	Scatter(P3FK("Lights"), 30.3f, 30.3f, 8, 3, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("PressPen"), 30.3f, 30.3f, 8, 7, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("StuntRig_A"), 30.3f, 30.3f, 8, 2, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("MakeUp"), 30.3f, 30.3f, 8, 6, 1.f, 1.f, 0.f, 1);
// the cooling field: behind the rack (left s % 4 == 2, right s % 4 == 1) and in the open slots behind the 26 m line
Zone(TEXT("People p3f: cooling field"), 13.85f, 17.40f);
	Scatter(P3FK("Sound"), 30.3f, 30.3f, 20, 2, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("CamTower"), 30.3f, 30.3f, 20, 6, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("SecondUnit_B"), 30.3f, 30.3f, 20, 10, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("Extras_B"), 30.3f, 30.3f, 20, 14, 1.f, 1.f, 0.f, -1);
	Scatter(P3FK("Catering"), 30.3f, 30.3f, 20, 18, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("LightBalloon"), 30.6f, 30.6f, 20, 3, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("StuntRig_B"), 30.6f, 30.6f, 20, 7, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("DroneTeam_B"), 30.6f, 30.6f, 20, 11, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("SecondUnit_A"), 30.6f, 30.6f, 20, 15, 1.f, 1.f, 0.f, -1);
	Scatter(P3FK("Crane"), 30.6f, 30.6f, 20, 19, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("Extras_A"), 30.3f, 30.3f, 20, 3, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("DroneTeam_A"), 30.3f, 30.3f, 20, 7, 1.f, 1.f, 0.f, 1);
	Scatter(P3FK("Lights"), 30.3f, 30.3f, 20, 11, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("PressPen"), 30.3f, 30.3f, 20, 15, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("StuntRig_A"), 30.3f, 30.3f, 20, 19, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("MakeUp"), 30.6f, 30.6f, 20, 0, 1.f, 1.f, 0.f, 1);
	Scatter(P3FK("Sound"), 30.6f, 30.6f, 20, 4, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("CamTower"), 30.6f, 30.6f, 20, 8, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("SecondUnit_B"), 30.6f, 30.6f, 20, 12, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("Extras_B"), 30.6f, 30.6f, 20, 16, 1.f, 1.f, 0.f, 1);
// tank farm B: behind the tank rows (to 37.8 m), between the watchers (s % 3 == 0): the units that reach over; a few compact ones in front (27.2 m)
Zone(TEXT("People p3f: tank farm B"), 17.40f, 21.40f);
	Scatter(P3F("DroneTeam_A"), 41.3f, 41.3f, 18, 2, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("CamTower"), 41.3f, 41.3f, 18, 5, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("LightBalloon"), 41.3f, 41.3f, 18, 8, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("StuntRig_A"), 41.3f, 41.3f, 18, 11, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("DroneTeam_B"), 41.3f, 41.3f, 18, 14, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("StuntRig_B"), 41.3f, 41.3f, 18, 17, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("StuntRig_A"), 41.3f, 41.3f, 18, 2, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("DroneTeam_B"), 41.3f, 41.3f, 18, 5, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("StuntRig_B"), 41.3f, 41.3f, 18, 8, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("DroneTeam_A"), 41.3f, 41.3f, 18, 11, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("CamTower"), 41.3f, 41.3f, 18, 14, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("LightBalloon"), 41.3f, 41.3f, 18, 17, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("SecondUnit_A"), 28.5f, 28.5f, 27, 1, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("PressPen"), 28.5f, 28.5f, 27, 10, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("Extras_B"), 28.5f, 28.5f, 27, 19, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("MakeUp"), 28.5f, 28.5f, 27, 1, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("Extras_A"), 28.5f, 28.5f, 27, 10, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("SecondUnit_B"), 28.5f, 28.5f, 27, 19, 1.f, 1.f, 0.f, 1);
// reactor two: as the reactor block
Zone(TEXT("People p3f: reactor two"), 21.40f, 25.60f);
	Scatter(P3FK("Catering"), 31.0f, 31.0f, 24, 3, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("LightBalloon"), 31.0f, 31.0f, 24, 7, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("StuntRig_B"), 31.0f, 31.0f, 24, 11, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("DroneTeam_B"), 31.0f, 31.0f, 24, 15, 1.f, 1.f, 0.f, -1);
	Scatter(P3F("SecondUnit_A"), 31.0f, 31.0f, 24, 19, 1.f, 1.f, 0.f, -1);
	Scatter(P3FK("Village"), 31.0f, 31.0f, 24, 23, 1.f, 1.f, 0.f, -1);
	Scatter(P3FK("Crane"), 31.0f, 31.0f, 24, 2, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("Extras_A"), 31.0f, 31.0f, 24, 6, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("DroneTeam_A"), 31.0f, 31.0f, 24, 10, 1.f, 1.f, 0.f, 1);
	Scatter(P3FK("Lights"), 31.0f, 31.0f, 24, 14, 1.f, 1.f, 0.f, 1);
	Scatter(P3F("PressPen"), 31.0f, 31.0f, 24, 18, 1.f, 1.f, 0.f, 1);
	Scatter(P3FK("Camera"), 31.0f, 31.0f, 24, 22, 1.f, 1.f, 0.f, 1);
#undef P3F
#undef P3FK
// ---- END p3f ----
}

{
// ---- BEGIN s1d1 (district 1 "Factory Gate", 0 to 2.6 km, ft_world_chem_s1d1.py) ----
// 19 meshes (8 buildings, 6 tall props, 4 low props, the hero SM_S1D1_ClockGate), all made by the one make run
// (C:/ftwork/logs/L1s1d1_make.log). The brick company town at the gate, both sides, in the even slots only
// (s % 4 == 0 or 2), nothing outside 0 to 2.6 km (slots 0 to 56; slot 57 is odd and not used).
// Estimate, counted slot by slot (29 even slots a side = 58 slot-sides):
//   streets: WldS1D1Homes 12 slot-sides x 4 = 48; WldS1D1Canteen 14 x 4 = 56; WldS1D1Fire 13 x 5 = 65;
//   WldS1D1Works 14 x 5 = 70; WldS1D1Yard 5 x 4 = 20; that is 259. Low props: 58 slot-sides x 1 = 58.
//   (259 + 58) x 0.75 (the track's share left out) = 238, + 2 Clock Gates = about 240 placed.
//   The most of one mesh: SM_S1D1_Lamp, 39 places before the 0.75 (about 29). Every chance is 1.f.
// Bands and slots used (x = metres from the centre line):
//   0 to 1.23 km (slots 0 to 26), the gate: a street 48.5 m out (buildings 43.2 to 52.7 m, behind the blow stacks'
//     41 to 43 m and the light masts, in front of the p3d pieces at 53 m), its lamps, boards, sentry boxes and
//     washing 41.3 to 41.5 m. Left s % 4 == 0 Homes, s % 4 == 2 Canteen; right s % 4 == 0 Fire; right s % 4 == 2 Works
//     at 64.5 m (60.4 to 68.7 m: the boom lifts stand 53 m out in those slots).
//   1.23 to 2.6 km (slots 28 to 56), the tank farm: the same streets at 51.5 m and scale 1.25 (the tank rows, 28.5 m
//     out and to 11 m tall, stand between them and the road in every slot: the bigger scale is to show roofs,
//     chimneys and the stack over them); pieces 42.5 to 56.8 m. Left, s % 6 == 0 (where the sphere crown stands
//     53 m out): no buildings, WldS1D1Yard at 43 m (a stack, a side gate, a board, a sentry box).
//   LOW props 23.45 m (23.0 to 23.9 m: between the kerb's stores, to 23 m, and the low line of people, 24 to 24.5 m),
//     one a slot-side, s % 8 picks which.
//   The Clock Gate: 122 m out on the right, slot 16 (0.72 to 0.77 km) and slot 40 (1.81 to 1.85 km); its gates' tips 111 m.
// Glow (steady, never blinking) only on TALL meshes; the smallest x min of a TALL mesh is 41.3 m (48.5 - 7.2).
// LOW meshes (no glow, no movement, x min 23.45): SM_S1D1_TimeCards, SM_S1D1_BikeRack, SM_S1D1_ChurnCart, SM_S1D1_Trough.
#define S1D1(n) TEXT("/Game/World/ChemicalWorks/SM_S1D1_" n)
Set(TEXT("WldS1D1ClockGate"), {      // the hero, alone
	{ S1D1("ClockGate"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D1Homes"), {      // the terrace and the gate lodge, its barrier arm over the lane between; washing, a lamp
	{ S1D1("Terrace"), -9.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("Lodge"), 10.5f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D1("Washing"), -9.5f, -7.0f, 0.f, 0.f, 1.f },
	{ S1D1("Lamp"), 2.5f, -7.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D1Canteen"), {      // the canteen and the time office; the notice board, a lamp
	{ S1D1("Canteen"), -8.8f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("TimeOffice"), 7.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("NoticeBoard"), 15.5f, -6.5f, 0.f, 0.f, 1.f },
	{ S1D1("Lamp"), 2.3f, -7.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D1Fire"), {      // the fire station and the pay office, the side gate closing the lane between; a sentry box, a lamp
	{ S1D1("FireStation"), -11.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("PayOffice"), 11.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("GatePiers"), 0.3f, -0.5f, 0.f, 0.f, 1.f },
	{ S1D1("SentryBox"), -3.2f, -7.0f, 0.f, 0.f, 1.f },
	{ S1D1("Lamp"), 4.2f, -7.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D1Works"), {      // the smithy and the institute, the works chimney between them; a notice board, a sentry box
	{ S1D1("Smithy"), -9.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("Institute"), 9.7f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("Stack"), 0.6f, 1.5f, 0.f, 0.f, 1.f },
	{ S1D1("NoticeBoard"), -1.5f, -7.0f, 0.f, 0.f, 1.f },
	{ S1D1("SentryBox"), 4.0f, -7.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D1Yard"), {      // no buildings: a works chimney, a side gate, a notice board, a sentry box
	{ S1D1("Stack"), 0.0f, 1.0f, 0.f, 0.f, 1.f },
	{ S1D1("GatePiers"), -9.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("NoticeBoard"), 7.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D1("SentryBox"), 12.5f, 0.3f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1d1, Factory Gate"), 0.0f, 2.6f);
	Site(TEXT("WldS1D1ClockGate"), 1, 122.0f, 1000, 984, 1.f);   // right, slot 16 (0.72 km)
	Far(S1D1("ClockGate"), 122.0f, 122.0f, 1000, 952, 1.f, 1.f, 0.f, 1, 1.57f, 0.f);   // fix round: the second gate, right, slot 48 (2.17 km, open road; was slot 40, inside the pipe 1.63 to 1.96 km, where a Site is left out). Far: placed whatever the road does; 1.57 m = the land there + 10 cm
	Scatter(S1D1("TimeCards"), 23.45f, 23.45f, 8, 0, 1.f, 1.f, 0.f, -1, 0.f);   // LOW, left, s % 8 == 0
	Scatter(S1D1("BikeRack"), 23.45f, 23.45f, 8, 6, 1.f, 1.f, 0.f, -1, 0.f);    // LOW, left, s % 8 == 2
	Scatter(S1D1("ChurnCart"), 23.45f, 23.45f, 8, 4, 1.f, 1.f, 0.f, -1, 0.f);   // LOW, left, s % 8 == 4
	Scatter(S1D1("Trough"), 23.45f, 23.45f, 8, 2, 1.f, 1.f, 0.f, -1, 0.f);      // LOW, left, s % 8 == 6
	Scatter(S1D1("ChurnCart"), 23.45f, 23.45f, 8, 0, 1.f, 1.f, 0.f, 1, 0.f);    // LOW, right, s % 8 == 0
	Scatter(S1D1("Trough"), 23.45f, 23.45f, 8, 6, 1.f, 1.f, 0.f, 1, 0.f);       // LOW, right, s % 8 == 2
	Scatter(S1D1("TimeCards"), 23.45f, 23.45f, 8, 4, 1.f, 1.f, 0.f, 1, 0.f);    // LOW, right, s % 8 == 4
	Scatter(S1D1("BikeRack"), 23.45f, 23.45f, 8, 2, 1.f, 1.f, 0.f, 1, 0.f);     // LOW, right, s % 8 == 6
Zone(TEXT("World s1d1, Factory Gate, the gate, slots 0 to 26"), 0.0f, 1.23f);
	Site(TEXT("WldS1D1Homes"), -1, 48.5f, 4, 0, 1.f);     // left, s % 4 == 0
	Site(TEXT("WldS1D1Canteen"), -1, 48.5f, 8, 2, 1.f);   // left, s % 8 == 6 (slots 6, 14, 22)
	Site(TEXT("WldS1D1Canteen"), -1, 48.5f, 1000, 998, 1.f);   // fix round: left, slot 2 only; slots 10, 18 and 26 are left out (the old yard dogs stand there at 44 to 57.4 m, a pipe bridge behind them at 63)
	Site(TEXT("WldS1D1Fire"), 1, 48.5f, 4, 0, 1.f);       // right, s % 4 == 0
	Site(TEXT("WldS1D1Works"), 1, 64.5f, 4, 2, 1.f);      // right, s % 4 == 2: behind the boom lifts (53 m)
Zone(TEXT("World s1d1, Factory Gate, the tank farm, slots 28 to 56"), 1.23f, 2.6f);
	Site(TEXT("WldS1D1Fire"), -1, 51.5f, 12, 10, 1.f, 1.25f);     // left, s % 12 == 2
	Site(TEXT("WldS1D1Works"), -1, 51.5f, 12, 8, 1.f, 1.25f);     // left, s % 12 == 4
	Site(TEXT("WldS1D1Canteen"), -1, 51.5f, 12, 4, 1.f, 1.25f);   // left, s % 12 == 8
	Site(TEXT("WldS1D1Homes"), -1, 51.5f, 1000, 966, 1.f, 1.25f);   // left, slot 34
	Site(TEXT("WldS1D1Homes"), -1, 64.0f, 1000, 954, 1.f, 1.25f);     // fix round: left, slot 46: 64 m, behind the yard dogs (they reach 57.4 m)
	Site(TEXT("WldS1D1Yard"), -1, 43.0f, 12, 0, 1.f, 1.25f);      // left, s % 12 == 0: the sphere crown's slots (53 m)
	Site(TEXT("WldS1D1Yard"), -1, 43.0f, 1000, 958, 1.f, 1.25f);     // left, slot 42
	Site(TEXT("WldS1D1Yard"), -1, 43.0f, 1000, 946, 1.f, 1.25f);       // fix round: left, slot 54; slot 30 left out (the yard dogs stand there)
	Site(TEXT("WldS1D1Canteen"), 1, 51.5f, 16, 8, 1.f, 1.25f);    // right, s % 16 == 8 (slots 40, 56)
	Site(TEXT("WldS1D1Canteen"), 1, 64.0f, 16, 0, 1.f, 1.25f);      // fix round: right, s % 16 == 0 (slots 32, 48): 64 m, behind the yard dogs (they reach 57.4 m)
	Site(TEXT("WldS1D1Homes"), 1, 51.5f, 8, 6, 1.f, 1.25f);       // right, s % 8 == 2
	Site(TEXT("WldS1D1Fire"), 1, 51.5f, 8, 4, 1.f, 1.25f);        // right, s % 8 == 4
	Site(TEXT("WldS1D1Works"), 1, 51.5f, 8, 2, 1.f, 1.25f);       // right, s % 8 == 6
#undef S1D1
// ---- END s1d1 ----
}

{
// ---- BEGIN s1d2 (district 2 "Reactor Block", ft_world_chem_s1d2.py) ----
// 19 meshes (8 buildings, 5 tall props, 5 low props, 1 hero), all /Game/World/ChemicalWorks/SM_S1D2_*. Only 2.60 to 5.35 km
// (slots 58 to 118), only EVEN slots, every rule on ONE slot and ONE side (every 1000, phase 1000 - slot: phase % 4 == 0 or 2).
// Counted slot by slot: 141 Site parts (47 Site rules) + 46 Far pieces = 187 placed if the track leaves nothing out;
// by the brief's shares (Site x 0.75, Far x 1): 141 x 0.75 + 46 = about 152. Most of one mesh: Lodge 28, Observatory 17, Institute 13. Hero: 1 (slot 74, right, 150 m).
// By mesh: Annex 11, Blackboard 5, Cylinders 3, Dish 11, FlaskRack 4, FlaskSign 13, Footbridge 7, GreatRetort 1, Hostel 12, Igloos 9, Institute 13, Lecture 10, Lodge 28, Molecule 4, Observatory 17, PendantLamp 13, Refectory 12, VentCaps 4, Weather 10.
// How the road lies here (worked out from the level's section list in FTData.cpp at 13.71 cm a unit; NOT seen in the game):
//   2.605-2.961 wall turn 1 (right-hander; flare crown and column trios on the LEFT)   3.044-3.332 LOOP   3.386-3.565 CORKSCREW
//   3.565-3.921 wall turn 2 (flare crown on the RIGHT)   4.374-4.607 JUMP (gap 4.425-4.513)   4.675-4.922 GLASS PIPE
//   The track leaves every Scatter and Site out of loop, corkscrew, pipe and gap slots (68, 70, 72, 76, 78, 98, 104, 106, 108).
//   So: Sites stand in the other even slots; beside the loop, the corkscrew and the pipe the quarter is carried on by Far
//   rules with the land's own drop (1.2 m), as the dressing's ground rows and the p3c "far ways" do. Slot 98 (the gap) is empty.
// Bands (m from the centre line) and what was read to stand there already (FTDressL1.inl, FTPeopleL1.inl; slot by slot):
//   23.5      LOW props (under 2 m, unlit, 0.9 m deep) behind the kerb and stores (17.3 to 23), in front of the 24.5 m people line;
//             only in slots whose 24 to 24.5 m rule is absent or has a chance under 1; never on the wall turns or at the jump.
//   37.2-39.4 TALL props (3.4 to 7.3 m; steady light), behind the rack / tank row / 35.6 m people, in front of my own first row.
//             By Far beside the loop, the corkscrew and the pipe: 38.3 m (not in slot 104, 4.705 km: the plan keeps 40 m to 4.75 km).
//   45.5-60   first row of buildings (12 m deep at most); 62.5-66.5 second row. On the wall turns and from 4.12 to 4.75 km (the
//             jump) nothing of mine is nearer than 40.1 m (geometry, not only the anchor). Side-slots that hold a Platform,
//             Substation, Kiln, ProcessUnit, ReactorBlock, CoolingField, flare crown or column trio in the band are left out.
//   150       HERO, the Great Retort: slot 74 (3.348 km), RIGHT, the level stretch between the loop and the corkscrew. The plan
//             asked for 3.26 km: that is inside the loop, where a Site is left out. Left of 5.05 to 5.35 km (the core): nothing.
// Smallest x min of a TALL mesh: 37.2 m. Smallest x of anything: 23.5 m. Anchors along the road: 2.609 to 5.347 km (pieces reach 2.603 to 5.349 km).
#define S1D2(n) TEXT("/Game/World/ChemicalWorks/SM_S1D2_" n)
#define S1D2_SITE(set, s, side, x) Site(TEXT("WldS1D2" set), side, x, 1000, 1000 - (s), 1.f)
#define S1D2_FAR(n, s, side, x) Far(S1D2(n), x, x, 1000, 1000 - (s), 1.f, 1.f, 0.f, side, 0.692f + 0.00676f * (x), 0.f)   // fix round: the drop is the land's own height at x + 10 cm (was 1.2 m at every distance: 25 to 33 cm too low near the road)
Set(TEXT("WldS1D2LowA"), {
	{ S1D2("FlaskRack"), -14.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("VentCaps"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Cylinders"), 14.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2LowB"), {
	{ S1D2("Blackboard"), -14.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Molecule"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("FlaskRack"), 14.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2LowC"), {
	{ S1D2("Cylinders"), -14.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Blackboard"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("VentCaps"), 14.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2LowE"), {
	{ S1D2("Molecule"), -18.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Cylinders"), -6.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Blackboard"), 6.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Low2A"), {
	{ S1D2("FlaskRack"), -16.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Molecule"), 16.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2Low2B"), {
	{ S1D2("VentCaps"), -16.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Blackboard"), 16.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2TallA"), {
	{ S1D2("PendantLamp"), -18.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("FlaskSign"), -6.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Dish"), 6.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("PendantLamp"), 18.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2TallB"), {
	{ S1D2("Weather"), -19.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("PendantLamp"), -11.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Footbridge"), 4.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("FlaskSign"), 18.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2TallC"), {
	{ S1D2("Dish"), -17.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("PendantLamp"), -6.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Weather"), 6.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("FlaskSign"), 17.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2TallD"), {
	{ S1D2("FlaskSign"), -18.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Footbridge"), -4.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("PendantLamp"), 9.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Dish"), 18.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2TallE"), {
	{ S1D2("Weather"), -17.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("PendantLamp"), -9.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("FlaskSign"), -1.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Dish"), 8.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2RowA"), {
	{ S1D2("Annex"), -12.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Lodge"), 1.5f, -1.0f, 0.f, 12.0f, 1.f },
	{ S1D2("Observatory"), 13.0f, 1.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2RowB"), {
	{ S1D2("Hostel"), -10.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Lodge"), 5.3f, -1.5f, 0.f, -10.0f, 1.f },
	{ S1D2("Observatory"), 15.4f, 0.5f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2RowC"), {
	{ S1D2("Igloos"), -10.5f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Lodge"), 4.0f, -1.5f, 0.f, 8.0f, 1.f },
	{ S1D2("Observatory"), 15.0f, 0.5f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2RowD"), {
	{ S1D2("Lecture"), -10.5f, 0.5f, 0.f, 0.0f, 1.f },
	{ S1D2("Lodge"), 4.5f, -1.5f, 0.f, -12.0f, 1.f },
});
Set(TEXT("WldS1D2BackA"), {
	{ S1D2("Institute"), -8.5f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Lodge"), 3.0f, -4.0f, 0.f, 10.0f, 1.f },
	{ S1D2("Refectory"), 13.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2BackB"), {
	{ S1D2("Lecture"), -11.0f, -1.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Lodge"), 2.5f, -4.5f, 0.f, -8.0f, 1.f },
	{ S1D2("Institute"), 12.8f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2BackC"), {
	{ S1D2("Refectory"), -12.5f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Lodge"), -1.5f, -5.0f, 0.f, 6.0f, 1.f },
	{ S1D2("Lecture"), 11.3f, -1.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2BackD"), {
	{ S1D2("Igloos"), -11.5f, -2.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Lodge"), 2.5f, -5.5f, 0.f, -6.0f, 1.f },
	{ S1D2("Institute"), 13.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Set(TEXT("WldS1D2BackS"), {
	{ S1D2("Hostel"), -8.0f, 0.0f, 0.f, 0.0f, 1.f },
	{ S1D2("Lodge"), 8.5f, 0.0f, 0.f, 10.0f, 1.f },
});
Set(TEXT("WldS1D2Retort"), {
	{ S1D2("GreatRetort"), 0.0f, 0.0f, 0.f, 0.0f, 1.f },
});
Zone(TEXT("World s1d2, Reactor Block"), 2.6f, 5.35f);
	// slot 58, 2.624 km
	S1D2_SITE("RowB", 58, -1, 51.5f);
	S1D2_SITE("BackA", 58, 1, 64.0f);
	// slot 62, 2.805 km
	S1D2_SITE("RowC", 62, 1, 60.0f);
	// slot 66, 2.986 km
	S1D2_SITE("RowA", 66, -1, 48.0f);
	// slot 74, 3.348 km
	S1D2_SITE("TallB", 74, -1, 38.8f);
	S1D2_SITE("Low2A", 74, 1, 23.5f);
	S1D2_SITE("TallA", 74, 1, 38.5f);
	S1D2_SITE("RowB", 74, 1, 46.5f);
	S1D2_SITE("BackA", 74, 1, 63.0f);
	// slot 82, 3.710 km
	S1D2_SITE("RowD", 82, -1, 55.5f);
	// slot 84, 3.800 km
	S1D2_SITE("BackB", 84, -1, 63.0f);
	// slot 88, 3.981 km
	S1D2_SITE("Low2B", 88, -1, 23.5f);
	S1D2_SITE("TallC", 88, -1, 37.2f);
	S1D2_SITE("LowA", 88, 1, 23.5f);
	S1D2_SITE("BackS", 88, 1, 66.5f);
	// slot 90, 4.072 km
	S1D2_SITE("Low2A", 90, -1, 23.5f);
	S1D2_SITE("TallA", 90, -1, 38.2f);
	S1D2_SITE("RowC", 90, -1, 45.5f);
	S1D2_SITE("LowB", 90, 1, 23.5f);
	S1D2_SITE("TallB", 90, 1, 38.5f);
	S1D2_SITE("RowA", 90, 1, 45.5f);
	S1D2_SITE("BackB", 90, 1, 62.5f);
	// slot 94, 4.253 km
	S1D2_SITE("RowD", 94, -1, 46.0f);
	S1D2_SITE("BackC", 94, -1, 63.0f);
	S1D2_SITE("RowC", 94, 1, 46.0f);
	// slot 96, 4.343 km
	S1D2_SITE("RowB", 96, -1, 47.0f);
	S1D2_SITE("RowA", 96, 1, 47.0f);
	S1D2_SITE("BackD", 96, 1, 63.0f);
	// slot 100, 4.524 km
	S1D2_SITE("RowC", 100, -1, 46.0f);
	S1D2_SITE("BackA", 100, -1, 63.0f);
	S1D2_SITE("RowD", 100, 1, 46.0f);
	// slot 102, 4.615 km
	S1D2_SITE("RowA", 102, -1, 46.0f);
	S1D2_SITE("BackB", 102, -1, 63.0f);
	S1D2_SITE("RowB", 102, 1, 46.5f);
	// slot 110, 4.977 km
	S1D2_SITE("TallC", 110, -1, 38.2f);
	S1D2_SITE("Low2B", 110, 1, 23.5f);
	S1D2_SITE("TallA", 110, 1, 38.2f);
	// slot 112, 5.067 km
	S1D2_SITE("TallC", 112, 1, 39.4f);
	S1D2_SITE("RowB", 112, 1, 47.5f);
	S1D2_SITE("BackA", 112, 1, 63.5f);
	// slot 114, 5.158 km
	S1D2_SITE("LowC", 114, 1, 23.5f);
	S1D2_SITE("TallD", 114, 1, 38.5f);
	S1D2_SITE("RowC", 114, 1, 45.5f);
	S1D2_SITE("BackC", 114, 1, 62.5f);
	// slot 118, 5.339 km
	S1D2_SITE("LowE", 118, 1, 23.5f);
	S1D2_SITE("TallE", 118, 1, 38.2f);
Zone(TEXT("World s1d2, Reactor Block: beside the loop, the corkscrew and the pipe"), 2.6f, 5.35f);
	// slot 64, 2.895 km. fix round: THE GREAT RETORT, right, 150 m, before the loop (was slot 74, behind the loop: never seen). Far: placed whatever the road does
	Far(S1D2("GreatRetort"), 150.f, 150.f, 1000, 1000 - 64, 1.f, 1.f, 0.f, 1, 1.81f, 0.f);
	// slot 68, 3.077 km
	S1D2_FAR("Footbridge", 68, -1, 38.3f);
	S1D2_FAR("Annex", 68, -1, 45.5f);
	S1D2_FAR("Refectory", 68, -1, 64.0f);
	S1D2_FAR("Dish", 68, 1, 38.3f);
	S1D2_FAR("Igloos", 68, 1, 45.5f);
	S1D2_FAR("Institute", 68, 1, 64.0f);
	// slot 70, 3.167 km
	S1D2_FAR("Weather", 70, -1, 38.3f);
	S1D2_FAR("Observatory", 70, -1, 45.5f);
	S1D2_FAR("Institute", 70, -1, 64.0f);
	S1D2_FAR("Weather", 70, 1, 38.3f);
	S1D2_FAR("Observatory", 70, 1, 45.5f);
	S1D2_FAR("Refectory", 70, 1, 65.0f);
	// slot 72, 3.257 km
	S1D2_FAR("FlaskSign", 72, -1, 38.3f);
	S1D2_FAR("Hostel", 72, -1, 46.0f);
	S1D2_FAR("Lecture", 72, -1, 66.0f);
	S1D2_FAR("Footbridge", 72, 1, 38.3f);
	S1D2_FAR("Annex", 72, 1, 45.5f);
	S1D2_FAR("Hostel", 72, 1, 67.0f);
	// slot 76, 3.438 km
	S1D2_FAR("Weather", 76, -1, 38.3f);
	S1D2_FAR("Hostel", 76, -1, 46.0f);
	S1D2_FAR("Refectory", 76, -1, 65.0f);
	S1D2_FAR("Footbridge", 76, 1, 38.3f);
	S1D2_FAR("Annex", 76, 1, 45.0f);
	S1D2_FAR("Institute", 76, 1, 65.0f);
	// slot 78, 3.529 km
	S1D2_FAR("Dish", 78, -1, 38.3f);
	S1D2_FAR("Annex", 78, -1, 45.5f);
	S1D2_FAR("Lecture", 78, -1, 66.0f);
	S1D2_FAR("FlaskSign", 78, 1, 38.3f);
	S1D2_FAR("Igloos", 78, 1, 45.5f);
	S1D2_FAR("Hostel", 78, 1, 67.0f);
	// slot 104, 4.705 km
	S1D2_FAR("Annex", 104, -1, 45.0f);
	S1D2_FAR("Institute", 104, -1, 65.0f);
	S1D2_FAR("Observatory", 104, 1, 46.0f);
	S1D2_FAR("Refectory", 104, 1, 65.0f);
	// slot 106, 4.796 km
	S1D2_FAR("FlaskSign", 106, -1, 38.3f);
	S1D2_FAR("Hostel", 106, -1, 46.0f);
	S1D2_FAR("Refectory", 106, -1, 66.0f);
	S1D2_FAR("Dish", 106, 1, 38.3f);
	S1D2_FAR("Igloos", 106, 1, 45.0f);
	S1D2_FAR("Institute", 106, 1, 66.0f);
	// slot 108, 4.886 km
	S1D2_FAR("Weather", 108, -1, 38.3f);
	S1D2_FAR("Annex", 108, -1, 44.5f);
	S1D2_FAR("Refectory", 108, -1, 64.0f);
	S1D2_FAR("Footbridge", 108, 1, 38.3f);
	S1D2_FAR("Annex", 108, 1, 44.5f);
	S1D2_FAR("Hostel", 108, 1, 64.0f);
#undef S1D2_FAR
#undef S1D2_SITE
#undef S1D2
// ---- END s1d2 ----
}

{
// ---- BEGIN s1d3 (district 3 "Loading Yards", 5.35 to 8.09 km, ft_world_chem_s1d3.py) ----
// 19 meshes: 1 hero (GreatScales), 8 buildings (the street: ContainerHotel, SignalCabin, YardOffice, WagonCafe, WeighHouse;
// the depots: CustomsShed, EngineShed, BoxTower), 10 props (LoadingGauge, WaterCrane, Semaphore, NumeralBoard, ForkliftBay,
// TallyTower: tall; PalletStack, SackTrucks, BufferStop, DrumDock: low).
// WHERE AND WHY (read from FTDressL1.inl, FTPeopleL1.inl, FTTrack.cpp and level 1's sections in FTData.cpp; nothing seen in the game):
//   The road here (sections x 0.1371 m): a pipe 5.456 to 5.785 km, a loop 5.840 to 6.128, a pipe 6.169 to 6.416, a ramp down
//   6.42 to 6.585 (15 in 100), esses to 7.325, the rise and the jump from there (7.28 to 7.94: left empty, as is the vat's 7.94 to 8.09).
//   The track leaves every Scatter and Site piece out of a slot whose frame is in a pipe or a loop, whatever its distance, so the
//   even slots 122 to 126, 130 to 134, 138 and 140 hold nothing but Far pieces: no kerb, no rack, no people. Everywhere else the
//   existing tables are full from 17.3 to 53.5 m in every slot (kerb and stores to 23, the low line of people 24 to 24.5, the
//   roadside rack to 28.2 or tank rows to 32.3, people at 30.3 / 34 / 35.6 / 40 / 44, the rack line 44.5 to 53.5, drums and
//   tankers scattered 28 to 70 m in every slot of the yards). So:
//   1. THE STREET BESIDE THE PIPES AND THE LOOP (Far, 1.2 m under the road's level like the dressing's own ground there, no yaw
//      jitter; three zones that hold only those slots): a low prop 26 m out, a tall prop 32 m, a small building 38.5 m, a building
//      49 m, on both sides of every even slot: 4 x 2 x 8 slots. The dressing's flat ground there lies 57 m and more out (even
//      slots) and at 32.5 / 36.5 m only in odd slots. Low props stand at 26 m, not 17 to 26: the bed is cut down beside a pipe.
//   2. THE DEPOTS (Site, one slot a rule: every 1000, phase 1000 - slot), behind everything that is there: anchor 125 m in pipe
//      alley's open slots (behind the scattered pipe racks, 84 to 110.6 m; a lot covers 111.3 to 131.7), anchor 96 m in the
//      yards on the side that the dressing's own LoadingYard / RailYard set does NOT take in that slot (a lot covers 82.3 to
//      102.7: behind the scattered tankers, the drum gallery and the column rows, before the rail spurs at 112). OUTSIDE the
//      brief's 26 to 70 m band, because that band is full here: said in the hand-over.
//   3. Slot 136 (6.153 km, the 41 m of plain road between the loop and the second pipe): a street lot on the right at 60 m
//      (53.6 to 63.5: between the rack line and the far tank) and THE GREAT SCALES behind it, 132 m out, right.
//   4. Slots 142 and 144 are on the ramp (a Set's parts stand at the anchor's height: one building a rule there, 3 m of footing).
//   No Stage 1 piece of this block stands in an odd slot. Meshes inside 70 m only: the five street buildings and the five tall
//   props; 82 m and more only: the three depot buildings, TallyTower, BufferStop, DrumDock, the hero; both: PalletStack and
//   SackTrucks (bounds 2.4 and 2.1 m tall: under the 2.5 m a shadow needs).
// Estimate (the brief's rule: Far x 1, Site x parts x 0.75):
//   Far: 16 rules a zone, each on its slot class and side: slots s % 4 == 2 are 122, 126, 130, 134, 138 (5), s % 4 == 0 are 124,
//   132, 140 (3): 8 slots x 2 sides x 4 bands = 64.
//   Sites: 15 depot lots x 8 parts = 120, the street lot 7, four single buildings 4, the hero 1 = 132, x 0.75 = 99.
//   About 163 placed (196 by the slot list, before the track leaves any out).
//   Most of one mesh: PalletStack and SackTrucks, 29 each by the slot list (20 in lots, 8 Far, 1 in the street lot). Hero: 1.
//   Smallest x min of a TALL mesh: 32 m (the five tall props, Far); the smallest of any rule: 26 m (the two low props, Far).
// A lambda of its own, like the blocks above, so that the level's population function stays small enough for the compiler.
auto WldS1D3 = [&]()
{
#define S1D3(n) TEXT("/Game/World/ChemicalWorks/SM_S1D3_" n)
#define S1D3_FAR(n, x, ph, side) Far(S1D3(n), x, x, 4, ph, 1.f, 1.f, 0.f, side, 0.692f + 0.00676f * (x), 0.f)   // fix round: the land's own height at x + 10 cm (was 1.2 m everywhere)
#define S1D3_AT(set, s, side, x) Site(TEXT(set), side, x, 1000, 1000 - (s), 1.f)
// the depots: { mesh, along m, out m, up m, yaw deg, scale }; out < 0 is nearer the road than the anchor
Set(TEXT("WldS1D3DepotA"), {                         // engine shed and customs shed, the yard before them
	{ S1D3("EngineShed"),  -10.5f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("CustomsShed"),  11.f,    0.f,  0.f, 0.f, 1.f },
	{ S1D3("BufferStop"),  -17.f,  -12.5f, 0.f, 0.f, 1.f },
	{ S1D3("SackTrucks"),  -10.f,  -12.3f, 0.f, 0.f, 1.f },
	{ S1D3("PalletStack"),  -4.5f, -12.3f, 0.f, 0.f, 1.f },
	{ S1D3("TallyTower"),    0.5f, -12.f,  0.f, 0.f, 1.f },
	{ S1D3("DrumDock"),      7.f,  -10.5f, 0.f, 0.f, 1.f },
	{ S1D3("PalletStack"),  13.f,  -10.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3DepotB"), {                         // box tower and engine shed
	{ S1D3("BoxTower"),    -14.f,    0.f,  0.f, 0.f, 1.f },
	{ S1D3("EngineShed"),    6.f,    0.f,  0.f, 0.f, 1.f },
	{ S1D3("DrumDock"),    -14.5f,  -8.f,  0.f, 0.f, 1.f },
	{ S1D3("SackTrucks"),   -8.f,   -8.5f, 0.f, 0.f, 1.f },
	{ S1D3("TallyTower"),   -1.5f, -11.5f, 0.f, 0.f, 1.f },
	{ S1D3("SackTrucks"),    6.f,  -12.3f, 0.f, 0.f, 1.f },
	{ S1D3("PalletStack"),  12.5f, -12.3f, 0.f, 0.f, 1.f },
	{ S1D3("BufferStop"),   18.f,  -12.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3DepotC"), {                         // customs shed and box tower
	{ S1D3("CustomsShed"),  -9.5f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("BoxTower"),      9.f,    0.f,  0.f, 0.f, 1.f },
	{ S1D3("DrumDock"),    -15.f,  -10.5f, 0.f, 0.f, 1.f },
	{ S1D3("PalletStack"),  -9.5f, -10.3f, 0.f, 0.f, 1.f },
	{ S1D3("DrumDock"),     -4.f,  -10.5f, 0.f, 0.f, 1.f },
	{ S1D3("TallyTower"),    2.5f,  -9.5f, 0.f, 0.f, 1.f },
	{ S1D3("SackTrucks"),    9.f,   -8.f,  0.f, 0.f, 1.f },
	{ S1D3("BufferStop"),   16.f,   -8.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3Street"), {                         // slot 136, right: the street under the Great Scales
	{ S1D3("YardOffice"),     -13.5f,  0.f,  0.f, 0.f, 1.f },
	{ S1D3("ContainerHotel"),  -1.5f,  0.f,  0.f, 0.f, 1.f },
	{ S1D3("SignalCabin"),      9.f,   0.f,  0.f, 0.f, 1.f },
	{ S1D3("NumeralBoard"),    19.f,  -1.f,  0.f, 0.f, 1.f },
	{ S1D3("Semaphore"),      -20.5f, -2.f,  0.f, 0.f, 1.f },
	{ S1D3("PalletStack"),      6.f,  -5.2f, 0.f, 0.f, 1.f },
	{ S1D3("SackTrucks"),      13.f,  -5.f,  0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D3SoloCabin"),   { { S1D3("SignalCabin"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D3SoloOffice"),  { { S1D3("YardOffice"),  0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D3SoloCustoms"), { { S1D3("CustomsShed"), 0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D3SoloEngine"),  { { S1D3("EngineShed"),  0.f, 0.f, 0.f, 0.f, 1.f } });
Set(TEXT("WldS1D3Hero"),        { { S1D3("GreatScales"), 0.f, 0.f, 0.f, 0.f, 1.f } });

// ---- 1. the street beside the pipes and the loop (Far). phase 2: slots s % 4 == 2; phase 0: s % 4 == 0. side -1 left, 1 right
Zone(TEXT("World s1d3, Loading Yards: beside the first pipe"), 5.50f, 5.72f);        // slots 122, 124, 126
	S1D3_FAR("PalletStack",     26.f,  2, -1);  S1D3_FAR("LoadingGauge",   32.f, 2, -1);
	S1D3_FAR("WagonCafe",       38.5f, 2, -1);  S1D3_FAR("ContainerHotel", 49.f, 2, -1);
	S1D3_FAR("SackTrucks",      26.f,  0, -1);  S1D3_FAR("WaterCrane",     32.f, 0, -1);
	S1D3_FAR("WeighHouse",      38.5f, 0, -1);  S1D3_FAR("YardOffice",     49.f, 0, -1);
	S1D3_FAR("SackTrucks",      26.f,  2, 1);   S1D3_FAR("Semaphore",      32.f, 2, 1);
	S1D3_FAR("WeighHouse",      38.5f, 2, 1);   S1D3_FAR("SignalCabin",    49.f, 2, 1);
	S1D3_FAR("PalletStack",     26.f,  0, 1);   S1D3_FAR("NumeralBoard",   32.f, 0, 1);
	S1D3_FAR("WagonCafe",       38.5f, 0, 1);   S1D3_FAR("ContainerHotel", 49.f, 0, 1);
Zone(TEXT("World s1d3, Loading Yards: beside the loop"), 5.87f, 6.08f);              // slots 130, 132, 134
	S1D3_FAR("SackTrucks",      26.f,  2, -1);  S1D3_FAR("ForkliftBay",    32.f, 2, -1);
	S1D3_FAR("WeighHouse",      38.5f, 2, -1);  S1D3_FAR("YardOffice",     49.f, 2, -1);
	S1D3_FAR("PalletStack",     26.f,  0, -1);  S1D3_FAR("Semaphore",      32.f, 0, -1);
	S1D3_FAR("WagonCafe",       38.5f, 0, -1);  S1D3_FAR("SignalCabin",    49.f, 0, -1);
	S1D3_FAR("PalletStack",     26.f,  2, 1);   S1D3_FAR("WaterCrane",     32.f, 2, 1);
	S1D3_FAR("WagonCafe",       38.5f, 2, 1);   S1D3_FAR("ContainerHotel", 49.f, 2, 1);
	S1D3_FAR("SackTrucks",      26.f,  0, 1);   S1D3_FAR("LoadingGauge",   32.f, 0, 1);
	S1D3_FAR("WeighHouse",      38.5f, 0, 1);   S1D3_FAR("YardOffice",     49.f, 0, 1);
Zone(TEXT("World s1d3, Loading Yards: beside the second pipe"), 6.22f, 6.35f);       // slots 138, 140
	S1D3_FAR("PalletStack",     26.f,  2, -1);  S1D3_FAR("NumeralBoard",   32.f, 2, -1);
	S1D3_FAR("WagonCafe",       38.5f, 2, -1);  S1D3_FAR("SignalCabin",    49.f, 2, -1);
	S1D3_FAR("SackTrucks",      26.f,  0, -1);  S1D3_FAR("LoadingGauge",   32.f, 0, -1);
	S1D3_FAR("WeighHouse",      38.5f, 0, -1);  S1D3_FAR("ContainerHotel", 49.f, 0, -1);
	S1D3_FAR("SackTrucks",      26.f,  2, 1);   S1D3_FAR("ForkliftBay",    32.f, 2, 1);
	S1D3_FAR("WeighHouse",      38.5f, 2, 1);   S1D3_FAR("YardOffice",     49.f, 2, 1);
	S1D3_FAR("PalletStack",     26.f,  0, 1);   S1D3_FAR("WaterCrane",     32.f, 0, 1);
	S1D3_FAR("WagonCafe",       38.5f, 0, 1);   S1D3_FAR("SignalCabin",    49.f, 0, 1);

// ---- 2 to 4. the depots, the street lot, the single buildings on the ramp and the hero: one slot a rule (slots 119 to 160)
Zone(TEXT("World s1d3, Loading Yards"), 5.35f, 7.26f);
	// pipe alley's open slots: 120 (before the first pipe), 128 (between it and the loop), 136, 146 and 148 (after the ramp)
	S1D3_AT("WldS1D3DepotA", 120, -1, 125.f);  S1D3_AT("WldS1D3DepotB", 120, 1, 125.f);
	S1D3_AT("WldS1D3DepotC", 128, -1, 125.f);  S1D3_AT("WldS1D3DepotA", 128, 1, 125.f);
	S1D3_AT("WldS1D3DepotB", 136, -1, 125.f);
	S1D3_AT("WldS1D3Street", 136, 1, 60.f);
	Far(S1D3("GreatScales"), 132.f, 132.f, 1000, 1000 - 132, 1.f, 1.f, 0.f, -1, 1.66f, 0.f);     // fix round: THE GREAT SCALES: slot 132 (5.97 km), LEFT, between the gull wheels of slots 128 and 134 (was slot 136 right: the loop stands between the driver and that side)
	S1D3_AT("WldS1D3SoloCabin",   142, -1, 58.f);   S1D3_AT("WldS1D3SoloOffice", 142, 1, 60.f);    // the ramp
	S1D3_AT("WldS1D3SoloCustoms", 144, -1, 125.f);  S1D3_AT("WldS1D3SoloEngine", 144, 1, 125.f);
	S1D3_AT("WldS1D3DepotC", 146, -1, 125.f);  S1D3_AT("WldS1D3DepotA", 146, 1, 125.f);
	S1D3_AT("WldS1D3DepotB", 148, -1, 125.f);  S1D3_AT("WldS1D3DepotC", 148, 1, 125.f);
	// the yards: the side the dressing's LoadingYard (s % 4 == 0) or RailYard (s % 4 == 2) set leaves free in that slot
	S1D3_AT("WldS1D3DepotA", 150, -1, 96.f);
	S1D3_AT("WldS1D3DepotB", 152, 1, 96.f);
	S1D3_AT("WldS1D3DepotC", 154, 1, 96.f);
	S1D3_AT("WldS1D3DepotA", 156, -1, 96.f);
	S1D3_AT("WldS1D3DepotB", 158, -1, 96.f);
	S1D3_AT("WldS1D3DepotC", 160, 1, 96.f);
#undef S1D3_AT
#undef S1D3_FAR
#undef S1D3
};
WldS1D3();
// ---- END s1d3 ----
}

{
// ---- BEGIN s1d4 (district 4 "Waste Run", ft_world_chem_s1d4.py) ----
// THE SLUDGE WORKS, 8.09 to 9.46 km (slots 179 to 209; this block uses the even ones, 180 to 208, both sides).
// 19 meshes: 8 buildings, 10 props, 1 hero. 52 rules, every one a Far rule with chance 1, so one rule = one piece a slot:
// SkimmersHut 12 + FilterHouse 8 + DrumWash 9 + RagStore 7 + AssayHut 12 + OldPumpHouse 7 + StiltRow 9 + BoilHouse 8 + Troughs 15 + Duckboards 15 + Punt 12 + SludgePump 6 + DripBarrels 6 + SignPost 15 + Scarecrow 14 + StovePipes 8 + HopperStand 11 + Lamp 8 + SludgeFountain 2
// = 72 buildings + 110 props + 2 hero = 184 placed before the track's clearance test (Far: x 1). Most of any one mesh: 15.
// WHY Far AND NOT Scatter OR Site: this district's road is a glass pipe (8.226 to 8.610 km), a corkscrew (8.665 to
// 8.843 km) and a loop (8.912 to 9.199 km) (FTData.cpp, level 1's sections x 13.71 cm). The track leaves every Scatter
// and Site piece out at such a slot (FTTrack.cpp: Loop | Gap | Pipe | Twist), which would leave 10 of the 15 even
// slots bare; a Far piece stays, as the dressing's own canals, yards and lagoons do here (FTDressL1.inl, "Ground: waste
// run", "Yards: waste run": Far 32.5 to 87 m). A Far piece stands `drop` m under the road's level instead of on the
// land: each drop below is the land's own height at that distance (FTTrack.cpp GroundBands, level 1: -70 cm at 16 m,
// -120 cm at 90 m, -300 cm at 300 m) plus 10 cm, so the piece stands sunk 10 cm in the land as a Scatter piece
// would (15 cm). Far pieces cast no shadow and still go through the track's clearance test (road and sight line).
// Bands (m from the centre line; pieces' own depth in brackets): low 23.4 (22.9 to 23.9) and 25.7 (24.9 to 26.5);
// tall thin 29.0 left (28.4 to 29.7) and 36.6 right; tall 38.8 (37.1 to 40.5); huts 33 (30 to 36) only beside the
// pipe, the corkscrew and the loop; streets 47.5 / 48.5 (44.7 to 51.7) and 63 / 69.5 (57.2 to 72.5); hero 112 (87 to 129).
// Slot classes: s % 4 == 0 is phase 0, s % 4 == 2 is phase 2 (every 4); s % 8 == 0, 4, 6, 2 are phases 0, 4, 2, 6 (every 8);
// a rule with every 1000 and phase 1000 - s takes slot s alone. The comment after each rule lists its slots.
// Smallest x min of a TALL mesh: 29.0 (SignPost, Scarecrow). Glow (steady) only on meshes whose every rule is 33 m or more out.
#define S1D4(n) TEXT("/Game/World/ChemicalWorks/SM_S1D4_" n)
Zone(TEXT("World s1d4, Waste Run"), 8.09f, 9.46f);
	// low and narrow, 23.4 m (22.9 to 23.9): behind the kerb's stores (to 23 m), before the low line (24.5 m)
	Far(S1D4("Duckboards"), 23.4f, 23.4f, 4, 0, 1.f, 1.f, 0.f, -1, 0.85f, 2.f);   // L: 180 184 188 192 196 200 204 208
	Far(S1D4("Troughs"), 23.4f, 23.4f, 4, 2, 1.f, 1.f, 0.f, -1, 0.85f, 2.f);   // L: 182 186 190 194 198 202 206
	Far(S1D4("Troughs"), 23.4f, 23.4f, 4, 0, 1.f, 1.f, 0.f, 1, 0.85f, 2.f);   // R: 180 184 188 192 196 200 204 208
	Far(S1D4("Duckboards"), 23.4f, 23.4f, 4, 2, 1.f, 1.f, 0.f, 1, 0.85f, 2.f);   // R: 182 186 190 194 198 202 206
	// tall and thin. Left 29 m: between the fence (27.5 m) and the tank row's plinth (30.8 m). Right 36.6 m: behind the film units (to 35.2 m)
	Far(S1D4("SignPost"), 29.f, 29.f, 4, 0, 1.f, 1.f, 0.f, -1, 0.89f, 8.f);   // L: 180 184 188 192 196 200 204 208
	Far(S1D4("Scarecrow"), 29.f, 29.f, 4, 2, 1.f, 1.f, 0.f, -1, 0.89f, 8.f);   // L: 182 186 190 194 198 202 206
	Far(S1D4("Scarecrow"), 36.6f, 36.6f, 4, 2, 1.f, 1.f, 0.f, 1, 0.94f, 8.f);   // R: 182 186 190 194 198 202 206
	Far(S1D4("SignPost"), 36.6f, 36.6f, 8, 4, 1.f, 1.f, 0.f, 1, 0.94f, 8.f);   // R: 180 188 196 204
	// tall, 38.8 m: behind the tank rows (to 37.2 m), before the watchers (40 m) and the blow-down stacks (41 m)
	Far(S1D4("StovePipes"), 38.8f, 38.8f, 8, 0, 1.f, 1.f, 0.f, -1, 0.95f, 12.f);   // L: 184 192 200 208
	Far(S1D4("HopperStand"), 38.8f, 38.8f, 8, 4, 1.f, 1.f, 0.f, -1, 0.95f, 12.f);   // L: 180 188 196 204
	Far(S1D4("Lamp"), 38.8f, 38.8f, 8, 2, 1.f, 1.f, 0.f, -1, 0.95f, 12.f);   // L: 182 190 198 206
	Far(S1D4("HopperStand"), 38.8f, 38.8f, 8, 6, 1.f, 1.f, 0.f, -1, 0.95f, 12.f);   // L: 186 194 202
	Far(S1D4("HopperStand"), 38.8f, 38.8f, 8, 0, 1.f, 1.f, 0.f, 1, 0.95f, 12.f);   // R: 184 192 200 208
	Far(S1D4("Lamp"), 38.8f, 38.8f, 8, 4, 1.f, 1.f, 0.f, 1, 0.95f, 12.f);   // R: 180 188 196 204
	Far(S1D4("StovePipes"), 38.8f, 38.8f, 8, 2, 1.f, 1.f, 0.f, 1, 0.95f, 12.f);   // R: 182 190 198 206
	Far(S1D4("SignPost"), 38.8f, 38.8f, 8, 6, 1.f, 1.f, 0.f, 1, 0.95f, 12.f);   // R: 186 194 202
	// the first street. Left 48.5 m (not slots 180 and 204: the WasteYard set stands 36 to 74 m there). Right 47.5 m (the hoarding stands at 53 m)
	Far(S1D4("SkimmersHut"), 48.5f, 48.5f, 8, 0, 1.f, 1.f, 0.f, -1, 1.02f, 6.f);   // L: 184 192 200 208
	Far(S1D4("AssayHut"), 48.5f, 48.5f, 8, 2, 1.f, 1.f, 0.f, -1, 1.02f, 6.f);   // L: 182 190 198 206
	Far(S1D4("OldPumpHouse"), 48.5f, 48.5f, 8, 6, 1.f, 1.f, 0.f, -1, 1.02f, 6.f);   // L: 186 194 202
	Far(S1D4("DrumWash"), 48.5f, 48.5f, 1000, 812, 1.f, 1.f, 0.f, -1, 1.02f, 6.f);   // L: 188
	Far(S1D4("DrumWash"), 48.5f, 48.5f, 1000, 804, 1.f, 1.f, 0.f, -1, 1.02f, 6.f);   // L: 196
	Far(S1D4("AssayHut"), 47.5f, 47.5f, 8, 0, 1.f, 1.f, 0.f, 1, 1.01f, 6.f);   // R: 184 192 200 208
	Far(S1D4("OldPumpHouse"), 47.5f, 47.5f, 8, 4, 1.f, 1.f, 0.f, 1, 1.01f, 6.f);   // R: 180 188 196 204
	Far(S1D4("SkimmersHut"), 47.5f, 47.5f, 8, 2, 1.f, 1.f, 0.f, 1, 1.01f, 6.f);   // R: 182 190 198 206
	Far(S1D4("DrumWash"), 47.5f, 47.5f, 8, 6, 1.f, 1.f, 0.f, 1, 1.01f, 6.f);   // R: 186 194 202
	// the second street. Left 69.5 m (behind the stack gallery at 63 m). Right 63 m
	Far(S1D4("FilterHouse"), 69.5f, 69.5f, 8, 0, 1.f, 1.f, 0.f, -1, 1.16f, 6.f);   // L: 184 192 200 208
	Far(S1D4("BoilHouse"), 69.5f, 69.5f, 8, 2, 1.f, 1.f, 0.f, -1, 1.16f, 6.f);   // L: 182 190 198 206
	Far(S1D4("RagStore"), 69.5f, 69.5f, 8, 6, 1.f, 1.f, 0.f, -1, 1.16f, 6.f);   // L: 186 194 202
	Far(S1D4("RagStore"), 69.5f, 69.5f, 1000, 812, 1.f, 1.f, 0.f, -1, 1.16f, 6.f);   // L: 188
	Far(S1D4("StiltRow"), 69.5f, 69.5f, 1000, 804, 1.f, 1.f, 0.f, -1, 1.16f, 6.f);   // L: 196
	Far(S1D4("StiltRow"), 63.f, 63.f, 8, 0, 1.f, 1.f, 0.f, 1, 1.12f, 6.f);   // R: 184 192 200 208
	Far(S1D4("BoilHouse"), 63.f, 63.f, 8, 4, 1.f, 1.f, 0.f, 1, 1.12f, 6.f);   // R: 180 188 196 204
	Far(S1D4("FilterHouse"), 63.f, 63.f, 8, 2, 1.f, 1.f, 0.f, 1, 1.12f, 6.f);   // R: 182 190 198 206
	Far(S1D4("RagStore"), 63.f, 63.f, 8, 6, 1.f, 1.f, 0.f, 1, 1.12f, 6.f);   // R: 186 194 202
	// THE SLUDGE FOUNTAIN, twice, left, 112 m (its basins reach 87 m, its funnel 129 m): slots 184 and 200, the two
	// even slots near 8.78 km where neither a tank cluster, a far vat, the ropeway nor the tank patrol stands 100 to 165 m out on the left
	Far(S1D4("SludgeFountain"), 112.f, 112.f, 1000, 816, 1.f, 1.f, 0.f, -1, 1.49f, 0.f);   // L: 184
	Far(S1D4("SludgeFountain"), 112.f, 112.f, 1000, 800, 1.f, 1.f, 0.f, -1, 1.49f, 0.f);   // L: 200
Zone(TEXT("World s1d4, Waste Run, low, to the run-in"), 8.09f, 9.16f);
	// low, 25.7 m (24.9 to 26.5): stops at 9.16 km, where the film crew of the run-in stands 25.4 to 26.2 m out on every even slot
	Far(S1D4("Punt"), 25.7f, 25.7f, 4, 0, 1.f, 1.f, 0.f, -1, 0.87f, 6.f);   // L: 180 184 188 192 196 200
	Far(S1D4("SludgePump"), 25.7f, 25.7f, 4, 2, 1.f, 1.f, 0.f, -1, 0.87f, 15.f);   // L: 182 186 190 194 198 202
	Far(S1D4("DripBarrels"), 25.7f, 25.7f, 4, 0, 1.f, 1.f, 0.f, 1, 0.87f, 15.f);   // R: 180 184 188 192 196 200
	Far(S1D4("Punt"), 25.7f, 25.7f, 4, 2, 1.f, 1.f, 0.f, 1, 0.87f, 6.f);   // R: 182 186 190 194 198 202
Zone(TEXT("World s1d4, Waste Run, round the pipe"), 8.3f, 8.53f);
	// huts over the canals and the yards, 33 m (30 to 36): only beside the pipe, the corkscrew and the loop, where the track leaves out
	// every Site and Scatter (fence, tank rows, stages, film units), so that band is empty there but for the Far canals and yard slabs
	Far(S1D4("SkimmersHut"), 33.f, 33.f, 4, 2, 1.f, 1.f, 0.f, -1, 0.91f, 6.f);   // L: 186
	Far(S1D4("DrumWash"), 33.f, 33.f, 4, 0, 1.f, 1.f, 0.f, -1, 0.91f, 6.f);   // L: 184 188
	Far(S1D4("StiltRow"), 33.f, 33.f, 4, 2, 1.f, 1.f, 0.f, 1, 0.91f, 6.f);   // R: 186
	Far(S1D4("AssayHut"), 33.f, 33.f, 4, 0, 1.f, 1.f, 0.f, 1, 0.91f, 6.f);   // R: 184 188
Zone(TEXT("World s1d4, Waste Run, the corkscrew"), 8.67f, 8.8f);
	Far(S1D4("SkimmersHut"), 33.f, 33.f, 4, 2, 1.f, 1.f, 0.f, -1, 0.91f, 6.f);   // L: 194
	Far(S1D4("DrumWash"), 33.f, 33.f, 4, 0, 1.f, 1.f, 0.f, -1, 0.91f, 6.f);   // L: 192
	Far(S1D4("StiltRow"), 33.f, 33.f, 4, 2, 1.f, 1.f, 0.f, 1, 0.91f, 6.f);   // R: 194
	Far(S1D4("AssayHut"), 33.f, 33.f, 4, 0, 1.f, 1.f, 0.f, 1, 0.91f, 6.f);   // R: 192
Zone(TEXT("World s1d4, Waste Run, the loop"), 8.94f, 9.16f);
	Far(S1D4("SkimmersHut"), 33.f, 33.f, 4, 2, 1.f, 1.f, 0.f, -1, 0.91f, 6.f);   // L: 198 202
	Far(S1D4("DrumWash"), 33.f, 33.f, 4, 0, 1.f, 1.f, 0.f, -1, 0.91f, 6.f);   // L: 200
	Far(S1D4("StiltRow"), 33.f, 33.f, 4, 2, 1.f, 1.f, 0.f, 1, 0.91f, 6.f);   // R: 198 202
	Far(S1D4("AssayHut"), 33.f, 33.f, 4, 0, 1.f, 1.f, 0.f, 1, 0.91f, 6.f);   // R: 200
#undef S1D4
// ---- END s1d4 ----
}

{
// ---- BEGIN s1d5 (district 5 "Export Terminal", ft_world_chem_s1d5.py) ----
// 17 meshes (7 buildings, 9 props, the hero). Every rule is a Site that fits ONE 45 m slot (every 1000, phase 1000 - slot), planned by
// C:/ftwork/jobs/L1s1d5/block.py from both tables of this level: 92 rules on even slots only (s % 4 == 0 or 2), slots 210 to 296.
// Estimate (a Site: 1 slot x chance 1 x 0.75 x the parts of its Set): QuayRow 8 x 5 + Customs 4 x 3 + Harbour 4 x 5 + Silos 9 x 3 + Mission 4 x 5 + Stepped 8 x 4 + Shop 4 x 4 + LampMast 6 x 2 + Lamps 5 x 2 + NetLamp 6 x 2 + Brow 6 x 2 + MastNet 14 x 2 + QuayEdge 13 x 6 + Anchor 1 x 1 = 320 parts tried x 0.75 = about 240 placed.
//   By mesh (tried): QuayLamp 38, BollardRow 34, FenderStack 34, NetRack 34, SignalMast 32, BoatTrestles 29, CrabPots 26, KedgeAnchor 25, QuayWarehouse 16, Chandlery 12, Gangway 10, TwinSilos 9, SteppedRow 8, CustomsHouse 4, HarbourOffice 4, SeamensMission 4, Anchor 1.
//   Smallest x min of a TALL mesh: 28.6 m (Gangway, NetRack, QuayLamp, SignalMast). The five LOW props stand at 23.8 m in the 13 slots whose kerb lane has no people.
// Distances: each building Set stands in a gap that both tables leave free in ITS slot and side, between 31.6 and 70 m (its front line is the
//   Site's x; a part's "out" is measured from that line); strips 2 m deep behind the rack and its gantries (left, 32.6 m) and in front of the
//   loading bays (right, 28.6 m) take the tall props. The flare avenue (12.72 km on, slots 282 to 296): unlit pieces only, buildings 60 m and
//   more out and never in a derrick's slot (s % 3 == 1). THE ANCHOR: slot 222 (10.04 km), right, 112 m.
#define S1D5(n) TEXT("/Game/World/ChemicalWorks/SM_S1D5_" n)
Set(TEXT("WldS1D5QuayRow"), {      // two quay warehouses and the chandlery between them, a lamp, fenders
	{ S1D5("QuayWarehouse"), -9.0f, 4.9f, 0.0f, 0.0f, 1.0f },
	{ S1D5("Chandlery"), -1.8f, 5.2f, 0.0f, 0.0f, 1.0f },
	{ S1D5("QuayWarehouse"), 5.2f, 4.3f, 0.0f, 0.0f, 0.86f },
	{ S1D5("QuayLamp"), 11.5f, 1.2f, 0.0f, 0.0f, 1.0f },
	{ S1D5("FenderStack"), 12.0f, 4.5f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5Customs"), {      // the customs house, a lamp, a kedge anchor on its block
	{ S1D5("CustomsHouse"), 0.0f, 5.2f, 0.0f, 0.0f, 1.0f },
	{ S1D5("QuayLamp"), -12.5f, 1.2f, 0.0f, 0.0f, 1.0f },
	{ S1D5("KedgeAnchor"), 13.0f, 5.0f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5Harbour"), {      // the harbour master's octagon, the signal mast, bollards, a boat on trestles, creels
	{ S1D5("HarbourOffice"), -4.5f, 5.3f, 0.0f, 0.0f, 1.0f },
	{ S1D5("SignalMast"), 4.2f, 1.5f, 0.0f, 0.0f, 1.0f },
	{ S1D5("BollardRow"), 8.5f, 0.5f, 0.0f, 0.0f, 1.0f },
	{ S1D5("BoatTrestles"), 8.5f, 4.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("CrabPots"), 8.0f, 7.0f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5Silos"), {      // UNLIT: the twin silos, creels, fenders
	{ S1D5("TwinSilos"), 0.0f, 3.9f, 0.0f, 0.0f, 1.0f },
	{ S1D5("CrabPots"), -11.5f, 2.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("FenderStack"), 11.5f, 2.0f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5Mission"), {      // the seamen's mission, a lamp, bollards, the gangway, nets
	{ S1D5("SeamensMission"), -3.0f, 4.8f, 0.0f, 0.0f, 1.0f },
	{ S1D5("QuayLamp"), -12.0f, 1.2f, 0.0f, 0.0f, 1.0f },
	{ S1D5("BollardRow"), 9.5f, 0.5f, 0.0f, 0.0f, 1.0f },
	{ S1D5("Gangway"), 9.5f, 2.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("NetRack"), 9.5f, 5.5f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5Stepped"), {      // UNLIT: the stepped row, a signal mast, a boat on trestles, a kedge anchor
	{ S1D5("SteppedRow"), 0.0f, 4.2f, 0.0f, 0.0f, 1.0f },
	{ S1D5("SignalMast"), -11.5f, 1.5f, 0.0f, 0.0f, 1.0f },
	{ S1D5("BoatTrestles"), 11.8f, 2.5f, 0.0f, 0.0f, 1.0f },
	{ S1D5("KedgeAnchor"), 11.5f, 5.5f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5Shop"), {      // the chandlery, a boat on trestles, nets, fenders
	{ S1D5("Chandlery"), -7.0f, 5.2f, 0.0f, 0.0f, 1.0f },
	{ S1D5("BoatTrestles"), -0.5f, 2.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("NetRack"), 0.5f, 5.5f, 0.0f, 0.0f, 1.0f },
	{ S1D5("FenderStack"), 6.0f, 2.0f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5LampMast"), {      // a strip 2 m deep: a lamp and a signal mast
	{ S1D5("QuayLamp"), -12.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("SignalMast"), 10.0f, 0.0f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5Lamps"), {      // a strip 2 m deep: two quay lamps
	{ S1D5("QuayLamp"), -13.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("QuayLamp"), 13.0f, 0.0f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5NetLamp"), {      // a strip: nets and a lamp
	{ S1D5("NetRack"), -9.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("QuayLamp"), 10.0f, 0.0f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5Brow"), {      // UNLIT strip: the gangway and nets
	{ S1D5("Gangway"), -8.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("NetRack"), 8.0f, 0.0f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5MastNet"), {      // UNLIT strip: a signal mast and nets
	{ S1D5("SignalMast"), -10.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("NetRack"), 9.0f, 0.0f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5QuayEdge"), {      // LOW (all under 2 m, 1.3 m deep): the quay edge, 23.15 to 24.45 m
	{ S1D5("BollardRow"), -17.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("FenderStack"), -9.5f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("BoatTrestles"), -2.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("CrabPots"), 5.5f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("KedgeAnchor"), 11.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ S1D5("BollardRow"), 18.0f, 0.0f, 0.0f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D5Anchor"), {      // the hero, alone
	{ S1D5("Anchor"), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
});
Zone(TEXT("World s1d5, Export Terminal"), 9.46f, 13.4f);
	Site(TEXT("WldS1D5Stepped"), -1, 32.2f, 1000, 790, 1.0f);   // slot 210 (9.50 km), left: free 31.8 to 42.3 m
	Site(TEXT("WldS1D5NetLamp"), 1, 28.8f, 1000, 790, 1.0f);   // slot 210 (9.50 km), right: strip 27.5 to 31.0 m
	Site(TEXT("WldS1D5Mission"), 1, 42.0f, 1000, 790, 1.0f);   // slot 210 (9.50 km), right: free 41.6 to 61.5 m
	Site(TEXT("WldS1D5QuayEdge"), -1, 23.8f, 1000, 788, 1.0f);   // slot 212 (9.59 km), left: LOW, the kerb lane, free of people here
	Site(TEXT("WldS1D5LampMast"), -1, 32.9f, 1000, 788, 1.0f);   // slot 212 (9.59 km), left: strip 31.6 to 37.7 m
	Site(TEXT("WldS1D5QuayRow"), -1, 38.0f, 1000, 786, 1.0f);   // slot 214 (9.68 km), left: free 37.6 to 59.9 m
	Site(TEXT("WldS1D5QuayEdge"), 1, 23.8f, 1000, 786, 1.0f);   // slot 214 (9.68 km), right: LOW, the kerb lane, free of people here
	Site(TEXT("WldS1D5Brow"), 1, 28.6f, 1000, 786, 1.0f);   // slot 214 (9.68 km), right: strip 27.3 to 31.5 m
	Site(TEXT("WldS1D5Silos"), -1, 41.9f, 1000, 784, 1.0f);   // slot 216 (9.77 km), left: free 41.5 to 50.0 m
	Site(TEXT("WldS1D5Stepped"), 1, 41.9f, 1000, 784, 1.0f);   // slot 216 (9.77 km), right: free 41.5 to 50.6 m
	Site(TEXT("WldS1D5MastNet"), 1, 28.8f, 1000, 782, 1.0f);   // slot 218 (9.86 km), right: strip 27.5 to 31.0 m
	Site(TEXT("WldS1D5Customs"), -1, 44.4f, 1000, 780, 1.0f);   // slot 220 (9.95 km), left: free 44.0 to 65.0 m
	Site(TEXT("WldS1D5Shop"), 1, 42.7f, 1000, 780, 1.0f);   // slot 220 (9.95 km), right: free 42.3 to 62.0 m
	Site(TEXT("WldS1D5Shop"), -1, 32.5f, 1000, 778, 1.0f);   // slot 222 (10.04 km), left: free 32.1 to 41.7 m
	Site(TEXT("WldS1D5NetLamp"), 1, 28.6f, 1000, 778, 1.0f);   // slot 222 (10.04 km), right: strip 27.3 to 31.5 m
	Far(S1D5("Anchor"), 112.0f, 112.0f, 1000, 778, 1.0f, 1.f, 0.f, -1, 1.49f, 0.f);     // fix round: THE ANCHOR: slot 222 (10.04 km), LEFT (on the right it stood under the HUD panel and behind the old stands)
	Site(TEXT("WldS1D5QuayEdge"), -1, 23.8f, 1000, 776, 1.0f);   // slot 224 (10.13 km), left: LOW, the kerb lane, free of people here
	Site(TEXT("WldS1D5Lamps"), -1, 32.9f, 1000, 776, 1.0f);   // slot 224 (10.13 km), left: strip 31.6 to 37.6 m
	Site(TEXT("WldS1D5Silos"), -1, 41.9f, 1000, 774, 1.0f);   // slot 226 (10.22 km), left: free 41.5 to 50.0 m
	Site(TEXT("WldS1D5QuayEdge"), 1, 23.8f, 1000, 774, 1.0f);   // slot 226 (10.22 km), right: LOW, the kerb lane, free of people here
	Site(TEXT("WldS1D5Brow"), 1, 28.8f, 1000, 774, 1.0f);   // slot 226 (10.22 km), right: strip 27.5 to 31.0 m
	Site(TEXT("WldS1D5Silos"), 1, 42.0f, 1000, 774, 1.0f);   // slot 226 (10.22 km), right: free 41.6 to 50.6 m
	Site(TEXT("WldS1D5Harbour"), -1, 45.9f, 1000, 772, 1.0f);   // slot 228 (10.31 km), left: free 45.5 to 72.0 m
	Site(TEXT("WldS1D5LampMast"), -1, 43.5f, 1000, 770, 1.0f);   // slot 230 (10.41 km), left: strip 42.3 to 46.0 m
	Site(TEXT("WldS1D5MastNet"), 1, 28.6f, 1000, 770, 1.0f);   // slot 230 (10.41 km), right: strip 27.3 to 31.5 m
	Site(TEXT("WldS1D5Mission"), -1, 32.0f, 1000, 768, 1.0f);   // slot 232 (10.50 km), left: free 31.6 to 45.0 m
	Site(TEXT("WldS1D5QuayRow"), -1, 44.6f, 1000, 766, 1.0f);   // slot 234 (10.59 km), left: free 44.2 to 59.3 m
	Site(TEXT("WldS1D5NetLamp"), 1, 28.8f, 1000, 766, 1.0f);   // slot 234 (10.59 km), right: strip 27.5 to 31.0 m
	Site(TEXT("WldS1D5Mission"), 1, 46.1f, 1000, 766, 1.0f);   // slot 234 (10.59 km), right: free 45.7 to 56.2 m
	Site(TEXT("WldS1D5QuayEdge"), -1, 23.8f, 1000, 764, 1.0f);   // slot 236 (10.68 km), left: LOW, the kerb lane, free of people here
	Site(TEXT("WldS1D5Lamps"), -1, 32.9f, 1000, 764, 1.0f);   // slot 236 (10.68 km), left: strip 31.6 to 35.5 m
	Site(TEXT("WldS1D5Customs"), -1, 45.9f, 1000, 762, 1.0f);   // slot 238 (10.77 km), left: free 45.5 to 62.0 m
	Site(TEXT("WldS1D5QuayEdge"), 1, 23.8f, 1000, 762, 1.0f);   // slot 238 (10.77 km), right: LOW, the kerb lane, free of people here
	Site(TEXT("WldS1D5Brow"), 1, 28.6f, 1000, 762, 1.0f);   // slot 238 (10.77 km), right: strip 27.3 to 31.5 m
	Site(TEXT("WldS1D5QuayRow"), -1, 42.7f, 1000, 760, 1.0f);   // slot 240 (10.86 km), left: free 42.3 to 65.0 m
	Site(TEXT("WldS1D5QuayRow"), 1, 44.4f, 1000, 760, 1.0f);   // slot 240 (10.86 km), right: free 44.0 to 72.0 m
	Site(TEXT("WldS1D5LampMast"), -1, 38.9f, 1000, 758, 1.0f);   // slot 242 (10.95 km), left: strip 37.6 to 41.9 m
	Site(TEXT("WldS1D5MastNet"), 1, 28.8f, 1000, 758, 1.0f);   // slot 242 (10.95 km), right: strip 27.5 to 31.0 m
	Site(TEXT("WldS1D5Stepped"), -1, 44.4f, 1000, 756, 1.0f);   // slot 244 (11.04 km), left: free 44.0 to 59.9 m
	Site(TEXT("WldS1D5Harbour"), 1, 43.8f, 1000, 756, 1.0f);   // slot 244 (11.04 km), right: free 43.4 to 56.2 m
	Site(TEXT("WldS1D5Silos"), -1, 41.9f, 1000, 754, 1.0f);   // slot 246 (11.13 km), left: free 41.5 to 50.0 m
	Site(TEXT("WldS1D5NetLamp"), 1, 28.6f, 1000, 754, 1.0f);   // slot 246 (11.13 km), right: strip 27.3 to 31.5 m
	Site(TEXT("WldS1D5Silos"), 1, 42.0f, 1000, 754, 1.0f);   // slot 246 (11.13 km), right: free 41.6 to 50.6 m
	Site(TEXT("WldS1D5QuayEdge"), -1, 23.8f, 1000, 752, 1.0f);   // slot 248 (11.22 km), left: LOW, the kerb lane, free of people here
	Site(TEXT("WldS1D5Lamps"), -1, 32.9f, 1000, 752, 1.0f);   // slot 248 (11.22 km), left: strip 31.6 to 38.5 m
	Site(TEXT("WldS1D5Harbour"), -1, 43.8f, 1000, 750, 1.0f);   // slot 250 (11.31 km), left: free 43.4 to 62.0 m
	Site(TEXT("WldS1D5QuayEdge"), 1, 23.8f, 1000, 750, 1.0f);   // slot 250 (11.31 km), right: LOW, the kerb lane, free of people here
	Site(TEXT("WldS1D5Brow"), 1, 28.8f, 1000, 750, 1.0f);   // slot 250 (11.31 km), right: strip 27.5 to 31.0 m
	Site(TEXT("WldS1D5QuayRow"), 1, 57.9f, 1000, 750, 1.0f);   // slot 250 (11.31 km), right: free 57.5 to 72.0 m
	Site(TEXT("WldS1D5LampMast"), -1, 32.6f, 1000, 748, 1.0f);   // slot 252 (11.40 km), left: strip 31.6 to 33.6 m
	Site(TEXT("WldS1D5Lamps"), -1, 43.5f, 1000, 746, 1.0f);   // slot 254 (11.49 km), left: strip 42.3 to 46.0 m
	Site(TEXT("WldS1D5MastNet"), 1, 28.6f, 1000, 746, 1.0f);   // slot 254 (11.49 km), right: strip 27.3 to 31.5 m
	Site(TEXT("WldS1D5Silos"), -1, 41.9f, 1000, 744, 1.0f);   // slot 256 (11.58 km), left: free 41.5 to 50.0 m
	Site(TEXT("WldS1D5Stepped"), 1, 41.9f, 1000, 744, 1.0f);   // slot 256 (11.58 km), right: free 41.5 to 50.6 m
	Site(TEXT("WldS1D5Mission"), -1, 45.9f, 1000, 742, 1.0f);   // slot 258 (11.67 km), left: free 45.5 to 72.0 m
	Site(TEXT("WldS1D5NetLamp"), 1, 28.8f, 1000, 742, 1.0f);   // slot 258 (11.67 km), right: strip 27.5 to 31.0 m
	Site(TEXT("WldS1D5QuayEdge"), -1, 23.8f, 1000, 740, 1.0f);   // slot 260 (11.76 km), left: LOW, the kerb lane, free of people here
	Site(TEXT("WldS1D5LampMast"), -1, 32.9f, 1000, 740, 1.0f);   // slot 260 (11.76 km), left: strip 31.6 to 36.8 m
	Site(TEXT("WldS1D5Lamps"), -1, 38.9f, 1000, 738, 1.0f);   // slot 262 (11.85 km), left: strip 37.6 to 41.7 m
	Site(TEXT("WldS1D5QuayEdge"), 1, 23.8f, 1000, 738, 1.0f);   // slot 262 (11.85 km), right: LOW, the kerb lane, free of people here
	Site(TEXT("WldS1D5Brow"), 1, 28.6f, 1000, 738, 1.0f);   // slot 262 (11.85 km), right: strip 27.3 to 31.5 m
	Site(TEXT("WldS1D5Shop"), -1, 42.7f, 1000, 736, 1.0f);   // slot 264 (11.94 km), left: free 42.3 to 59.9 m
	Site(TEXT("WldS1D5Customs"), 1, 42.7f, 1000, 736, 1.0f);   // slot 264 (11.94 km), right: free 42.3 to 56.2 m
	Site(TEXT("WldS1D5LampMast"), -1, 42.8f, 1000, 734, 1.0f);   // slot 266 (12.03 km), left: strip 41.5 to 46.0 m
	Site(TEXT("WldS1D5MastNet"), 1, 28.8f, 1000, 734, 1.0f);   // slot 266 (12.03 km), right: strip 27.5 to 31.0 m
	Site(TEXT("WldS1D5QuayRow"), -1, 45.9f, 1000, 732, 1.0f);   // slot 268 (12.12 km), left: free 45.5 to 72.0 m
	Site(TEXT("WldS1D5Customs"), -1, 44.4f, 1000, 730, 1.0f);   // slot 270 (12.21 km), left: free 44.0 to 59.3 m
	Site(TEXT("WldS1D5NetLamp"), 1, 28.6f, 1000, 730, 1.0f);   // slot 270 (12.21 km), right: strip 27.3 to 31.5 m
	Site(TEXT("WldS1D5Shop"), 1, 42.0f, 1000, 730, 1.0f);   // slot 270 (12.21 km), right: free 41.6 to 72.0 m
	Site(TEXT("WldS1D5QuayEdge"), -1, 23.8f, 1000, 728, 1.0f);   // slot 272 (12.31 km), left: LOW, the kerb lane, free of people here
	Site(TEXT("WldS1D5QuayRow"), -1, 32.0f, 1000, 728, 1.0f);   // slot 272 (12.31 km), left: free 31.6 to 45.0 m
	Site(TEXT("WldS1D5Stepped"), -1, 43.8f, 1000, 726, 1.0f);   // slot 274 (12.40 km), left: free 43.4 to 59.9 m
	Site(TEXT("WldS1D5QuayEdge"), 1, 23.8f, 1000, 726, 1.0f);   // slot 274 (12.40 km), right: LOW, the kerb lane, free of people here
	Site(TEXT("WldS1D5Brow"), 1, 28.8f, 1000, 726, 1.0f);   // slot 274 (12.40 km), right: strip 27.5 to 31.0 m
	Site(TEXT("WldS1D5Stepped"), 1, 46.9f, 1000, 726, 1.0f);   // slot 274 (12.40 km), right: free 46.5 to 56.2 m
	Site(TEXT("WldS1D5Silos"), -1, 41.9f, 1000, 724, 1.0f);   // slot 276 (12.49 km), left: free 41.5 to 50.0 m
	Site(TEXT("WldS1D5Silos"), 1, 41.9f, 1000, 724, 1.0f);   // slot 276 (12.49 km), right: free 41.5 to 50.6 m
	Site(TEXT("WldS1D5MastNet"), 1, 28.6f, 1000, 722, 1.0f);   // slot 278 (12.58 km), right: strip 27.3 to 31.5 m
	Site(TEXT("WldS1D5Harbour"), -1, 42.7f, 1000, 720, 1.0f);   // slot 280 (12.67 km), left: free 42.3 to 65.0 m
	Site(TEXT("WldS1D5QuayRow"), 1, 43.8f, 1000, 720, 1.0f);   // slot 280 (12.67 km), right: free 43.4 to 62.0 m
	Site(TEXT("WldS1D5Stepped"), -1, 64.5f, 1000, 712, 1.0f);   // slot 288 (13.03 km), left: free 64.1 to 76.0 m
	Site(TEXT("WldS1D5Silos"), -1, 64.5f, 1000, 706, 1.0f);   // slot 294 (13.30 km), left: free 64.1 to 76.0 m
	Site(TEXT("WldS1D5Stepped"), 1, 60.4f, 1000, 706, 1.0f);   // slot 294 (13.30 km), right: free 45.7 to 76.0 m
	// fix round: the flare avenue (slots 282 to 296) is left to its flames: the 8 mast-and-net strips and the kerb props of slot 292 were taken out
#undef S1D5
// ---- END s1d5 ----
}

{
// ---- BEGIN s1d6 (district 6 "Cooling Field", the spa in the steam, ft_world_chem_s1d6.py) ----
// 16 meshes: 7 buildings (BathHouse, WinterGarden, PumpRoom, Colonnade, TeaPavilion, CabinTerrace, Hydro), 8 props (low: PlungePool,
// DeckChairs, Urn, Fountain, TowelCart, ChessTable; tall: GlobeLamp, VaneKiosk), the hero THE CASCADE. 13.4 to 17.4 km: slots 297 to 384.
// Even slots only (every 12, phase 0, 2, 4, 6, 8 or 10), chosen by reading both tables slot by slot (C:\ftwork\out\L1s1d6\occ.txt):
//   front row, 38.4 / 38.6 m (the band 36.3 to 40.3 m, between the 35.6 m walkers and the watchers at 40 m): s%12==0 the colonnade
//     walk and two lamps, s%12==6 the changing cabins, a lamp and a towel cart, s%12==8 and 10 a well-head kiosk and a chess table.
//     Only on the three stretches that the dressing's "Columns" zones do not mark as banked (slots 308-319, 326-350, 357-380):
//     beside a banked bend 17 to 40 m floats over the cutting;
//   middle row, 54.5 m (the band 50 to 59 m, free where a kiln stands at 46-48 and a platform from 60): s%12==4 the tea pavilion
//     with a plunge pool and deck chairs (laid 0.9), s%12==10 the pump room with a fountain and an urn;
//   back row, 63.5 m (55 to 71 m; behind the 54 m galleries, on the slots with neither the CoolingField set nor a platform):
//     left s%12==2 and right s%12==0 the bath house, left s%12==8 and right s%12==6 the winter garden, right s%12==2 and 8 the hydro;
//   low props by the road, 23.4 to 24.0 m (behind the stores' 23 m, short pieces), level stretches only: urns s%12==10, deck chairs
//     right and a towel cart left s%12==4;
//   the avenue's end, 13.4 to 13.85 km (the flare avenue: 60 m out, unlit, never a derrick slot): the colonnade at slot 302 both
//     sides, the cabins right at slots 300 and 306. Both meshes carry no light;
//   the hero: slot 340 (15.38 km) and slot 368 (16.65 km), right, anchor 150 m (its pool 122 m, its back 173 m).
// Estimate (slots counted one by one; chance 1 everywhere): front row 10x3 + 10x3 + 20x2 = 100; low 10 + 5 + 5 = 20; middle row
// 12x3 + 14x3 = 78; back row 13x3 + 13x3 + 13x3 = 117; the avenue's end 4x2 = 8; hero 2. Sum 325 tried; x 0.75 (the track's share
// left out) = about 244 placed. The most of one mesh: PlungePool 38 tried (VaneKiosk 33, ChessTable 33, GlobeLamp 30).
// Smallest x of a TALL mesh: 38.0 m (GlobeLamp in the cabins' set: 38.6 - 0.6). Smallest x of a lit mesh: 38.0 m. Nothing blinks.
#define S1D6(n) TEXT("/Game/World/ChemicalWorks/SM_S1D6_" n)
Set(TEXT("WldS1D6Colonnade"), {                    // { mesh, along m, out m, up m, yaw deg, scale }
	{ S1D6("Colonnade"),      0.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D6("GlobeLamp"),    -14.6f, 0.2f, 0.f, 0.f, 1.f },
	{ S1D6("GlobeLamp"),     14.6f, 0.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Cabins"), {
	{ S1D6("CabinTerrace"),   0.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D6("GlobeLamp"),     14.4f, -0.6f, 0.f, 0.f, 1.f },
	{ S1D6("TowelCart"),    -15.2f, -0.4f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Promenade"), {
	{ S1D6("VaneKiosk"),      0.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D6("ChessTable"),    -5.5f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Tea"), {
	{ S1D6("TeaPavilion"),    0.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D6("PlungePool"),   -10.2f, 0.3f, 0.f, 0.f, 1.f },
	{ S1D6("DeckChairs"),    10.f, -0.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Pump"), {
	{ S1D6("PumpRoom"),       0.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D6("Fountain"),      -7.4f, -0.4f, 0.f, 0.f, 1.f },
	{ S1D6("Urn"),            6.2f, -2.2f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Bath"), {
	{ S1D6("BathHouse"),      0.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D6("PlungePool"),   -12.6f, -1.f, 0.f, 0.f, 1.f },
	{ S1D6("TowelCart"),     11.6f, -3.4f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Garden"), {
	{ S1D6("WinterGarden"),   0.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D6("Fountain"),     -14.2f, -1.5f, 0.f, 0.f, 1.f },
	{ S1D6("ChessTable"),    14.6f, -1.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6Hydro"), {
	{ S1D6("Hydro"),          0.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D6("PlungePool"),   -14.f, -2.f, 0.f, 0.f, 1.f },
	{ S1D6("VaneKiosk"),     12.6f, -3.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6ColonnadeFar"), {
	{ S1D6("Colonnade"),      0.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D6("Urn"),          -13.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6CabinsFar"), {
	{ S1D6("CabinTerrace"),   0.f,  0.f, 0.f, 0.f, 1.f },
	{ S1D6("DeckChairs"),    16.5f, -0.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D6CascadeSet"), {
	{ S1D6("Cascade"),        0.f,  0.f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1d6, Cooling Field: the avenue's end"), 13.4f, 13.85f);          // slots 297 to 306: unlit, 60 m out
	Site(TEXT("WldS1D6ColonnadeFar"), -1, 60.f, 12, 10, 1.f);                      // slot 302, left
	Site(TEXT("WldS1D6ColonnadeFar"), 1, 60.f, 12, 10, 1.f);                       // slot 302, right
	Site(TEXT("WldS1D6CabinsFar"), 1, 62.f, 12, 0, 1.f);                           // slot 300, right
	Site(TEXT("WldS1D6CabinsFar"), 1, 62.f, 12, 6, 1.f);                           // slot 306, right
Zone(TEXT("World s1d6, Cooling Field"), 13.85f, 17.4f);                            // slots 307 to 384: the middle and back rows, the hero
	Site(TEXT("WldS1D6Tea"), -1, 54.5f, 12, 8, 1.f, 0.9f);                         // s % 12 == 4
	Site(TEXT("WldS1D6Tea"), 1, 54.5f, 12, 8, 1.f, 0.9f);
	Site(TEXT("WldS1D6Pump"), -1, 54.5f, 12, 2, 1.f);                              // s % 12 == 10
	Site(TEXT("WldS1D6Pump"), 1, 54.5f, 12, 2, 1.f);
	Site(TEXT("WldS1D6Bath"), -1, 67.f, 12, 10, 1.f);                              // left, s % 12 == 2. fix round: 67 m (at 63.5 its platform ran 3 m into the old stack gallery)
	Site(TEXT("WldS1D6Bath"), 1, 63.5f, 12, 0, 1.f);                               // right, s % 12 == 0
	Site(TEXT("WldS1D6Garden"), -1, 63.5f, 12, 4, 1.f);                            // left, s % 12 == 8
	Site(TEXT("WldS1D6Garden"), 1, 63.5f, 12, 6, 1.f);                             // right, s % 12 == 6
	Site(TEXT("WldS1D6Hydro"), 1, 63.5f, 12, 10, 1.f);                             // right, s % 12 == 2
	Site(TEXT("WldS1D6Hydro"), 1, 65.5f, 12, 4, 1.f);                              // right, s % 12 == 8. fix round: 65.5 m (at 63.5 its front ran 1.9 m into the old water tower)
	// fix round: THE CASCADE on open road (slots 340 and 368 were inside the two loops, where a Site is left out). Far: placed whatever the road does
	Far(S1D6("Cascade"), 150.f, 150.f, 1000, 1000 - 316, 1.f, 1.f, 0.f, 1, 1.81f, 0.f);   // slot 316 (14.30 km), right
	Far(S1D6("Cascade"), 155.f, 155.f, 1000, 1000 - 348, 1.f, 1.f, 0.f, 1, 1.86f, 0.f);   // slot 348 (15.74 km), right, 155 m: clear of the old far tank crew (to 128 m)
Zone(TEXT("World s1d6, Cooling Field: level stretch 1"), 13.9f, 14.44f);           // slots 308 to 319
	Site(TEXT("WldS1D6Colonnade"), -1, 38.4f, 12, 0, 1.f);
	Site(TEXT("WldS1D6Colonnade"), 1, 38.4f, 12, 0, 1.f);
	Site(TEXT("WldS1D6Cabins"), -1, 38.6f, 12, 6, 1.f);
	Site(TEXT("WldS1D6Cabins"), 1, 38.6f, 12, 6, 1.f);
	Site(TEXT("WldS1D6Promenade"), -1, 38.4f, 12, 2, 1.f);
	Site(TEXT("WldS1D6Promenade"), 1, 38.4f, 12, 2, 1.f);
	Site(TEXT("WldS1D6Promenade"), -1, 38.4f, 12, 4, 1.f);
	Site(TEXT("WldS1D6Promenade"), 1, 38.4f, 12, 4, 1.f);
	Scatter(S1D6("Urn"), 23.5f, 23.6f, 12, 2, 1.f, 1.f, 0.f, 0, 20.f);             // low, both sides, s % 12 == 10
	Scatter(S1D6("DeckChairs"), 24.f, 24.f, 12, 8, 1.f, 1.f, 0.f, 1, 0.f);         // low, right, s % 12 == 4
	Scatter(S1D6("TowelCart"), 23.8f, 23.8f, 12, 8, 1.f, 1.f, 0.f, -1, 0.f);       // low, left, s % 12 == 4
Zone(TEXT("World s1d6, Cooling Field: level stretch 2"), 14.74f, 15.85f);          // slots 326 to 350
	// fix round: no colonnade walk here (slots 336 and 348): it stood 2 to 3 m into the old watchers at 40 m
	Site(TEXT("WldS1D6Cabins"), -1, 38.6f, 12, 6, 1.f);
	Site(TEXT("WldS1D6Cabins"), 1, 38.6f, 12, 6, 1.f);
	Site(TEXT("WldS1D6Promenade"), -1, 38.4f, 12, 2, 1.f);
	Site(TEXT("WldS1D6Promenade"), 1, 38.4f, 12, 2, 1.f);
	Site(TEXT("WldS1D6Promenade"), -1, 38.4f, 12, 4, 1.f);
	Site(TEXT("WldS1D6Promenade"), 1, 38.4f, 12, 4, 1.f);
	Scatter(S1D6("Urn"), 23.5f, 23.6f, 12, 2, 1.f, 1.f, 0.f, 0, 20.f);
	Scatter(S1D6("DeckChairs"), 24.f, 24.f, 12, 8, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(S1D6("TowelCart"), 23.8f, 23.8f, 12, 8, 1.f, 1.f, 0.f, -1, 0.f);
Zone(TEXT("World s1d6, Cooling Field: level stretch 3"), 16.14f, 17.2f);           // slots 357 to 380
	Site(TEXT("WldS1D6Colonnade"), -1, 38.4f, 12, 0, 1.f);
	Site(TEXT("WldS1D6Colonnade"), 1, 38.4f, 12, 0, 1.f);
	Site(TEXT("WldS1D6Cabins"), -1, 38.6f, 12, 6, 1.f);
	Site(TEXT("WldS1D6Cabins"), 1, 38.6f, 12, 6, 1.f);
	Site(TEXT("WldS1D6Promenade"), -1, 38.4f, 12, 2, 1.f);
	Site(TEXT("WldS1D6Promenade"), 1, 38.4f, 12, 2, 1.f);
	Site(TEXT("WldS1D6Promenade"), -1, 38.4f, 12, 4, 1.f);
	Site(TEXT("WldS1D6Promenade"), 1, 38.4f, 12, 4, 1.f);
	Scatter(S1D6("Urn"), 23.5f, 23.6f, 12, 2, 1.f, 1.f, 0.f, 0, 20.f);
	Scatter(S1D6("DeckChairs"), 24.f, 24.f, 12, 8, 1.f, 1.f, 0.f, 1, 0.f);
	Scatter(S1D6("TowelCart"), 23.8f, 23.8f, 12, 8, 1.f, 1.f, 0.f, -1, 0.f);
#undef S1D6
// ---- END s1d6 ----
}

{
// ---- BEGIN s1d7 (district 7 "Tank Farm B", THE ROUND TOWN, 17.4 to 21.4 km, ft_world_chem_s1d7.py) ----
// 16 meshes (8 buildings, 7 props: 2 tall, 5 low; the hero SM_S1D7_GiantKettle), all made by the one make run
// (C:/ftwork/logs/L1s1d7_make.log: pieces built 16 failed 0). Written by C:/ftwork/jobs/L1s1d7/make_block.py, which counts it
// slot by slot. Even slots only (s % 4 == 0 or 2), slots 386 to 472, both sides, nothing outside 17.4 to 21.4 km.
// Every rule is a Site on ONE slot (every 1000, phase 1000 - slot; chance 1): a Site stands exactly on its slot's middle,
// so every piece's place along the slot is known; the script leaves a rule out wherever one of the two tables already
// has a standing piece in its band on that slot and side (49 rules left out that way).
// Estimate: 129 Sites, 324 pieces tried (Green1 4, Green2 3, Green3 3, Kerb1 1, Kerb2 1, Kettle 1, Pair1 8, Pair2 8, Pair3 8, Pair4 7, Pair5 7, Pair6 7, Post1 5, Post2 5, Row1 3, Row2 3, Row3 2, Street1 3, Street2 3, Street3 3, Street4 2, Yard1 6, Yard2 6, Yard3 6, Yard4 6, Yard5 6, Yard6 6, Yard7 6).
//   By the brief's rule (x 0.75, the track's share left out): 324 x 0.75 = about 243 placed.
//   By the road itself (FTData.cpp, level 1's Circuit: each 1.40 km lap has a pipe of 247 m and a loop of 288 m, where the
//   track leaves every Site out; the laps start at 17.19, 18.59 and 19.99 km): 220 pieces stand in slots outside pipe and loop.
//   The most of one mesh: PipeLamp, 37 tried (29 outside pipe and loop). No mesh over 40.
// Bands (x = metres from the centre line) and why:
//   24 to 45 m is full in every even slot of this district (gantries 24.5 to 27.2, tank rows 30.2 to 37.8 in every slot,
//   stages 31.7 to 36.3, watchers and film units 37.7 to 44.7, tall p3a pieces to 47.7), so the town stands BEHIND it:
//   s % 4 == 0, and s % 24 == 2 (an access road): houses in PAIRS at the slot's two ends (along +-18 m), 49.8 to 57.7 m, and a
//     YARD of two props in front of them, 46.5 to 49.5 m. The middle of the slot is left to SM_Platform (Scatter 46 to 76 m,
//     it slides 11.3 m along), the access road and its turning head.
//   s % 24 == 10, 18: whole STREETS (house, inn or hall, house) 49.8 to 57.7 m and a GREEN of five props 46.5 to 49.5 m.
//   s % 8 == 6 (a works road with tugs lies 45.5 to 53.5 m out): one ROW of houses and props 59 to 66.9 m, in front of the
//     tank farm sets (from 67.1 m); not where a p3d piece stands 58 to 66 m out.
//   The straights only (slots 388, 396, 420, 426, 434, 450; elsewhere the road is banked and 17 to 40 m floats over the
//     cutting): LOW props 23.9 m (23.1 to 24.7), TALL slim props 28.6 m (27.5 to 29.45), where both tables leave them free.
//   THE GIANT KETTLE: right, slot 438 (19.79 to 19.84 km), 108 m out (body and gallery 89.6 to 126.5 m, the spout's steam to 82.3 m).
//     Not at 19.5 km: 19.32 to 19.61 km is the loop of the second lap, where no Site is placed. Slot 438 is in the bend
//     after it, the place on its lap where the ground flare (18.46 km) and the second reactor core (21.26 km) stand on
//     theirs, on the other side of the road; the right of slot 438 has no tank farm set and no sphere tank, and the
//     gasworks set starts 138 m out.
//   Nothing on the LEFT of slot 408 (the ground flare, 170 m out) and of slot 470 (the second reactor core, 150 m out).
// TALL props stand 46.5 to 66 m out (and 28.6 m in the straights), not 24 to 40 m: that band has no free place here.
// Glow (steady, never blinking) only on TALL meshes; the smallest x min of a TALL mesh is 28.6 m (WldS1D7Post1); of a LOW mesh
// 23.9 m (WldS1D7Kerb1). LOW meshes (log: low; no glow): SM_S1D7_LidTable, _ElbowStack, _WashLine (garments sway), _ValveWheel, _RainButt.
// A lambda of its own, like the p3 blocks above it, so that the level's population function stays small for the compiler.
auto WldS1D7Block = [&]()
{
#define S1D7(n) TEXT("/Game/World/ChemicalWorks/SM_S1D7_" n)
#define S1D7_AT(set, s, side, x) Site(TEXT("WldS1D7" set), side, x, 1000, 1000 - (s), 1.f)
Set(TEXT("WldS1D7Pair1"), {      // two houses at the two ends of a slot (the middle is left to the platform, the access road and what slides there)
	{ S1D7("CylHouse"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("SphereCabin"), 18.0f, 0.0f, 0.f, 12.0f, 1.0f },
});
Set(TEXT("WldS1D7Pair2"), {
	{ S1D7("GaugeHouse"), -18.0f, 0.3f, 0.f, -10.0f, 1.0f },
	{ S1D7("DrumCottage"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Pair3"), {
	{ S1D7("DomeHouse"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("WaterTowerHouse"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Pair4"), {
	{ S1D7("SphereCabin"), -18.0f, 0.0f, 0.f, -14.0f, 1.0f },
	{ S1D7("GaugeHouse"), 18.0f, 0.3f, 0.f, 8.0f, 1.0f },
});
Set(TEXT("WldS1D7Pair5"), {
	{ S1D7("DrumCottage"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("DomeHouse"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Pair6"), {
	{ S1D7("WaterTowerHouse"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("CylHouse"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Yard1"), {      // what stands in front of a pair, 5.8 m nearer the road
	{ S1D7("Dipsticks"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("RainButt"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Yard2"), {
	{ S1D7("WashLine"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("PipeLamp"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Yard3"), {
	{ S1D7("ElbowStack"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("ValveWheel"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Yard4"), {
	{ S1D7("PipeLamp"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("LidTable"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Yard5"), {
	{ S1D7("RainButt"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("WashLine"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Yard6"), {
	{ S1D7("ValveWheel"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("Dipsticks"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Yard7"), {
	{ S1D7("LidTable"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("ElbowStack"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Street1"), {      // a whole street front: house, inn or hall, house
	{ S1D7("CylHouse"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("Inn"), 0.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("WaterTowerHouse"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Street2"), {
	{ S1D7("DomeHouse"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("TankHall"), 0.3f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("SphereCabin"), 18.0f, 0.0f, 0.f, 10.0f, 1.0f },
});
Set(TEXT("WldS1D7Street3"), {
	{ S1D7("GaugeHouse"), -18.0f, 0.3f, 0.f, 0.0f, 1.0f },
	{ S1D7("Inn"), 0.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("DrumCottage"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Street4"), {
	{ S1D7("DrumCottage"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("TankHall"), 0.3f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("CylHouse"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Green1"), {      // the street's own front: five props a slot
	{ S1D7("WashLine"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("PipeLamp"), -9.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("LidTable"), 0.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("Dipsticks"), 9.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("RainButt"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Green2"), {
	{ S1D7("ValveWheel"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("RainButt"), -9.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("PipeLamp"), 0.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("ElbowStack"), 9.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("LidTable"), 18.3f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Green3"), {
	{ S1D7("Dipsticks"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("ElbowStack"), -8.8f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("WashLine"), 0.2f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("PipeLamp"), 9.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("ValveWheel"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Row1"), {      // the row behind a works road (the road and its tugs lie 45.5 to 53.5 m out): houses and their own props in one line
	{ S1D7("WaterTowerHouse"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("TankHall"), 0.3f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("GaugeHouse"), 18.0f, 0.3f, 0.f, 0.0f, 1.0f },
	{ S1D7("ValveWheel"), -10.5f, -2.6f, 0.f, 0.0f, 1.0f },
	{ S1D7("PipeLamp"), 9.8f, -2.6f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Row2"), {
	{ S1D7("CylHouse"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("Inn"), 0.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("SphereCabin"), 18.0f, 0.0f, 0.f, -12.0f, 1.0f },
	{ S1D7("Dipsticks"), -10.3f, -3.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("WashLine"), 10.6f, -3.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Row3"), {
	{ S1D7("DomeHouse"), -18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("DrumCottage"), -6.6f, 0.3f, 0.f, 0.0f, 1.0f },
	{ S1D7("GaugeHouse"), 4.6f, 0.4f, 0.f, 14.0f, 1.0f },
	{ S1D7("WaterTowerHouse"), 18.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("PipeLamp"), 11.0f, -2.6f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Kerb1"), {      // LOW pieces only, between the kerb's stores (to 22.95 m) and the gantries (from 24.5 m)
	{ S1D7("ElbowStack"), -13.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("LidTable"), 0.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("RainButt"), 12.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Kerb2"), {
	{ S1D7("ValveWheel"), -12.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("WashLine"), 0.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("ElbowStack"), 13.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Post1"), {      // TALL slim props between the gantries (to 27.2 m) and the tank row (from 30.2 m)
	{ S1D7("Dipsticks"), -11.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("PipeLamp"), 10.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Post2"), {
	{ S1D7("PipeLamp"), -12.0f, 0.0f, 0.f, 0.0f, 1.0f },
	{ S1D7("Dipsticks"), 9.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Set(TEXT("WldS1D7Kettle"), {      // the hero, alone
	{ S1D7("GiantKettle"), 0.0f, 0.0f, 0.f, 0.0f, 1.0f },
});
Zone(TEXT("World s1d7, Tank Farm B, the round town"), 17.4f, 21.4f);
	/* 386, 17.46 km, bend b */ S1D7_AT("Pair1", 386, -1, 53.8f); S1D7_AT("Yard1", 386, -1, 48.0f); S1D7_AT("Pair2", 386, 1, 53.8f); S1D7_AT("Yard2", 386, 1, 48.0f);
	/* 388, 17.55 km, straight c */ S1D7_AT("Pair3", 388, -1, 53.8f); S1D7_AT("Yard3", 388, -1, 48.0f); S1D7_AT("Post1", 388, -1, 28.6f); S1D7_AT("Pair4", 388, 1, 53.8f); S1D7_AT("Yard4", 388, 1, 48.0f); S1D7_AT("Kerb1", 388, 1, 23.9f); S1D7_AT("Post2", 388, 1, 28.6f);
	/* 390, 17.64 km, PIPE */ S1D7_AT("Row1", 390, -1, 63.0f);
	/* 392, 17.73 km, PIPE */ S1D7_AT("Pair5", 392, 1, 53.8f); S1D7_AT("Yard5", 392, 1, 48.0f);
	/* 396, 17.92 km, straight e */ S1D7_AT("Pair6", 396, -1, 53.8f); S1D7_AT("Yard6", 396, -1, 48.0f); S1D7_AT("Post1", 396, -1, 28.6f); S1D7_AT("Post2", 396, 1, 28.6f);
	/* 398, 18.01 km, LOOP */ S1D7_AT("Row2", 398, -1, 63.0f);
	/* 400, 18.10 km, LOOP */ S1D7_AT("Pair1", 400, -1, 53.8f); S1D7_AT("Yard7", 400, -1, 48.0f); S1D7_AT("Pair2", 400, 1, 53.8f);
	/* 402, 18.19 km, LOOP */ S1D7_AT("Street1", 402, -1, 53.8f); S1D7_AT("Green1", 402, -1, 48.0f); S1D7_AT("Street2", 402, 1, 53.8f); S1D7_AT("Green2", 402, 1, 48.0f);
	/* 404, 18.28 km, bend h */ S1D7_AT("Pair3", 404, -1, 53.8f); S1D7_AT("Yard1", 404, -1, 48.0f); S1D7_AT("Pair4", 404, 1, 53.8f); S1D7_AT("Yard2", 404, 1, 48.0f);
	/* 406, 18.37 km, bend h */ S1D7_AT("Row3", 406, 1, 63.0f);
	/* 408, 18.46 km, bend i */ S1D7_AT("Pair5", 408, 1, 53.8f); S1D7_AT("Yard3", 408, 1, 48.0f);
	/* 410, 18.55 km, bend i */ S1D7_AT("Pair6", 410, -1, 53.8f); S1D7_AT("Yard4", 410, -1, 48.0f); S1D7_AT("Pair1", 410, 1, 53.8f); S1D7_AT("Yard5", 410, 1, 48.0f);
	/* 412, 18.64 km, bend a */ S1D7_AT("Pair2", 412, -1, 53.8f); S1D7_AT("Yard6", 412, -1, 48.0f); S1D7_AT("Pair3", 412, 1, 53.8f); S1D7_AT("Yard7", 412, 1, 48.0f);
	/* 414, 18.73 km, bend a */ S1D7_AT("Row1", 414, -1, 63.0f); S1D7_AT("Row2", 414, 1, 63.0f);
	/* 416, 18.82 km, bend b */ S1D7_AT("Pair4", 416, 1, 53.8f); S1D7_AT("Yard1", 416, 1, 48.0f);
	/* 418, 18.91 km, bend b */ S1D7_AT("Street3", 418, -1, 53.8f); S1D7_AT("Green3", 418, -1, 48.0f);
	/* 420, 19.00 km, straight c */ S1D7_AT("Pair5", 420, -1, 53.8f); S1D7_AT("Yard2", 420, -1, 48.0f); S1D7_AT("Post1", 420, -1, 28.6f); S1D7_AT("Pair6", 420, 1, 53.8f); S1D7_AT("Yard3", 420, 1, 48.0f); S1D7_AT("Post2", 420, 1, 28.6f);
	/* 424, 19.18 km, PIPE */ S1D7_AT("Pair1", 424, -1, 53.8f); S1D7_AT("Yard4", 424, -1, 48.0f); S1D7_AT("Pair2", 424, 1, 53.8f);
	/* 426, 19.27 km, straight e */ S1D7_AT("Street4", 426, -1, 53.8f); S1D7_AT("Green1", 426, -1, 48.0f); S1D7_AT("Post1", 426, -1, 28.6f); S1D7_AT("Street1", 426, 1, 53.8f); S1D7_AT("Green2", 426, 1, 48.0f); S1D7_AT("Post2", 426, 1, 28.6f);
	/* 428, 19.36 km, LOOP */ S1D7_AT("Pair3", 428, -1, 53.8f); S1D7_AT("Yard5", 428, -1, 48.0f); S1D7_AT("Pair4", 428, 1, 53.8f); S1D7_AT("Yard6", 428, 1, 48.0f);
	/* 430, 19.45 km, LOOP */ S1D7_AT("Row3", 430, 1, 63.0f);
	/* 432, 19.54 km, LOOP */ S1D7_AT("Pair5", 432, -1, 53.8f); S1D7_AT("Yard7", 432, -1, 48.0f); S1D7_AT("Pair6", 432, 1, 53.8f); S1D7_AT("Yard1", 432, 1, 48.0f);
	/* 434, 19.63 km, straight g */ S1D7_AT("Pair1", 434, -1, 53.8f); S1D7_AT("Yard2", 434, -1, 48.0f); S1D7_AT("Kerb2", 434, -1, 23.9f); S1D7_AT("Pair2", 434, 1, 53.8f); S1D7_AT("Yard3", 434, 1, 48.0f);
	/* 436, 19.72 km, bend h */ S1D7_AT("Pair3", 436, 1, 53.8f); S1D7_AT("Yard4", 436, 1, 48.0f);
	/* 438, 19.82 km, bend h */ S1D7_AT("Row1", 438, -1, 63.0f); S1D7_AT("Kettle", 438, 1, 108.0f);
	/* 440, 19.91 km, bend i */ S1D7_AT("Pair4", 440, -1, 53.8f); S1D7_AT("Yard5", 440, -1, 48.0f);
	/* 442, 20.00 km, bend a */ S1D7_AT("Street2", 442, -1, 53.8f); S1D7_AT("Street3", 442, 1, 53.8f); S1D7_AT("Green3", 442, 1, 48.0f);
	/* 444, 20.09 km, bend a */ S1D7_AT("Pair5", 444, -1, 53.8f); S1D7_AT("Yard6", 444, -1, 48.0f); S1D7_AT("Pair6", 444, 1, 53.8f); S1D7_AT("Yard7", 444, 1, 48.0f);
	/* 448, 20.27 km, bend b */ S1D7_AT("Pair1", 448, -1, 53.8f); S1D7_AT("Yard1", 448, -1, 48.0f); S1D7_AT("Pair2", 448, 1, 53.8f); S1D7_AT("Yard2", 448, 1, 48.0f);
	/* 450, 20.36 km, straight c */ S1D7_AT("Street4", 450, -1, 53.8f); S1D7_AT("Green1", 450, -1, 48.0f); S1D7_AT("Post1", 450, -1, 28.6f); S1D7_AT("Street1", 450, 1, 53.8f); S1D7_AT("Green2", 450, 1, 48.0f); S1D7_AT("Post2", 450, 1, 28.6f);
	/* 452, 20.45 km, PIPE */ S1D7_AT("Pair3", 452, -1, 53.8f); S1D7_AT("Yard3", 452, -1, 48.0f); S1D7_AT("Pair4", 452, 1, 53.8f); S1D7_AT("Yard4", 452, 1, 48.0f);
	/* 454, 20.54 km, PIPE */ S1D7_AT("Row2", 454, 1, 63.0f);
	/* 456, 20.63 km, PIPE */ S1D7_AT("Pair5", 456, -1, 53.8f); S1D7_AT("Yard5", 456, -1, 48.0f); S1D7_AT("Pair6", 456, 1, 53.8f); S1D7_AT("Yard6", 456, 1, 48.0f);
	/* 458, 20.72 km, LOOP */ S1D7_AT("Pair1", 458, -1, 53.8f); S1D7_AT("Yard7", 458, -1, 48.0f); S1D7_AT("Pair2", 458, 1, 53.8f); S1D7_AT("Yard1", 458, 1, 48.0f);
	/* 460, 20.81 km, LOOP */ S1D7_AT("Pair3", 460, 1, 53.8f); S1D7_AT("Yard2", 460, 1, 48.0f);
	/* 464, 20.99 km, LOOP */ S1D7_AT("Pair4", 464, -1, 53.8f); S1D7_AT("Yard3", 464, -1, 48.0f); S1D7_AT("Pair5", 464, 1, 53.8f); S1D7_AT("Yard4", 464, 1, 48.0f);
	/* 466, 21.08 km, bend h */ S1D7_AT("Street2", 466, -1, 53.8f); S1D7_AT("Green3", 466, -1, 48.0f); S1D7_AT("Street3", 466, 1, 53.8f); S1D7_AT("Green1", 466, 1, 48.0f);
	/* 468, 21.17 km, bend h */ S1D7_AT("Pair6", 468, -1, 53.8f); S1D7_AT("Yard5", 468, -1, 48.0f); S1D7_AT("Pair1", 468, 1, 53.8f); S1D7_AT("Yard6", 468, 1, 48.0f);
	/* 472, 21.35 km, bend i */ S1D7_AT("Pair2", 472, -1, 53.8f); S1D7_AT("Pair3", 472, 1, 53.8f); S1D7_AT("Yard7", 472, 1, 48.0f);
#undef S1D7_AT
#undef S1D7
};
WldS1D7Block();
// ---- END s1d7 ----
}

{
// ---- BEGIN s1d8 (district 8 "Reactor Two", the copper quarter, 21.40 to 25.76 km, ft_world_chem_s1d8.py) ----
// 17 meshes: 7 buildings (PowerHall, Foundry, SwitchHouse, Club, SmithsRow, GateBastion, QuenchTower), 9 props (IngotStack, LadleCar,
// RivetForge, AnvilRack, BollardChain, VentCowl, FoundryLamp, SignPylon, TubeRack), the hero (Hourglass, 70 m). NOT COMPILED, NOT SEEN IN THE GAME when written.
// Every rule is a Site on an EVEN slot (every 8 or 16 with an even phase: s % 4 == 0 or 2), so a street stands as it was composed. All sets: anchor 44 m.
// Nothing of this block stands nearer than 41.5 m (a prop's origin; the smiths' row's front comes to 41.2 m), so every lit piece is 24 m and more out. Nothing blinks.
// Where the sets stand, from FTDressL1.inl and FTPeopleL1.inl (21.4 to 25.6 km): the dressing's own sets alternate sides slot by slot (FTTrack.cpp: a Site of
//   side 0 is LEFT when (s / every) is even): ProcessUnit (anchor 64 m, 54.5 to 104 m) left at s % 8 == 2, right at s % 8 == 6; ReactorBlock (98 m, 62 to 134 m)
//   left at s % 8 == 0, right at s % 8 == 4. This block's buildings stand on the OTHER side of each slot (the free side), 65 to 69 m out; on the set's side
//   only the smiths' row (48 m, before the ProcessUnit) and props (41.5 to 45.5 m). Nearer, the even slots are taken (kerb and stores to 23 m, people 24 to 26 m,
//   fence 27.5, rack 31.5 left, tank row 34 right at s % 4 == 0, film units 27 to 41 right at s % 4 == 2): so nothing here is nearer than 41 m.
//   NOT dodged (their slots, or their slide along the road, are not fixed): the watchers at 40 to 40.5 m and the p3a pieces at 44 m on their listed slots,
//   SM_Platform (40 to 55 m, s % 5 == 2), SM_Substation and SM_Catwalk (44 to 62 m, s % 5 == 3), SM_BlowStack (41 to 43 m, s % 6 == 2), the p3d pieces at 52.5 m
//   on s % 4 == 0 (between this block's props at 42 to 45 m and its buildings at 67 to 69 m), and the unlit yard lots (Far, flat) under all of it.
// Three stretches. OWN GROUND 21.40 to 22.18 km (slots 474 to 490) and the last 160 m, 25.58 to 25.76 km (566, 568): a street on both sides of every even
//   slot, chance 1. THE SECOND LAP 22.18 to 25.58 km (slots 491 to 565, beside the cooling field's road): the free side only, one building 66 to 69 m out with
//   its forecourt 54 to 59 m (no prop inside 40 m), chance 0.85; the track's clearance test leaves out what would touch or overlook the other lap.
// HERO: THE HOURGLASS, right, 185 m out, three one-slot Zones: slots 490 (22.17 km), 522 (23.62 km), 562 (25.42 km). All three are s % 8 == 2 (the right side
//   is free of the dressing's sets) and none is a slot of SM_CoolingCells (150 to 210 m, s % 3 == 2). SM_Tower (120 to 200 m, every even slot, both sides,
//   chance 0.7) may stand beside it. Whether a slot lies beside a loop, pipe or corkscrew (the track then leaves a Site out) was NOT checked: hence three.
// Estimate (slots counted by C:/ftwork/jobs/L1s1d8/mkblock.py, x chance x 0.75): own ground 71.2 + last 160 m 15.8 + second lap 100.1 + hero 2.25 = about 189 placed
//   (62 Sites, 276 pieces before chance and the track). Most of one mesh before chance and the track: FoundryLamp 34, BollardChain 30, SignPylon 30, TubeRack 24.
#define S1D8(n) TEXT("/Game/World/ChemicalWorks/SM_S1D8_" n)
Set(TEXT("WldS1D8PowerSt"), {	// the power hall 65 m out, the switch house 51.5 m before its left end; ingots, tubes, bollards, lamp, sign 46.5 to 50 m
	{ S1D8("PowerHall"), 0.0f, 21.0f, 0.f, 0.f, 1.f },
	{ S1D8("SwitchHouse"), -21.5f, 7.5f, 0.f, 0.f, 1.f },
	{ S1D8("IngotStack"), 8.0f, 5.5f, 0.f, 0.f, 1.f },
	{ S1D8("TubeRack"), 17.0f, 6.0f, 0.f, 0.f, 1.f },
	{ S1D8("BollardChain"), 11.0f, 2.5f, 0.f, 0.f, 1.f },
	{ S1D8("FoundryLamp"), 0.0f, 2.5f, 0.f, 0.f, 1.f },
	{ S1D8("SignPylon"), -9.0f, 2.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8FoundrySt"), {	// the foundry 65 m, the quench tower 50 m; ladle car, forge, anvil rack, lamp, cowl 46 to 50 m
	{ S1D8("Foundry"), 0.0f, 21.0f, 0.f, 0.f, 1.f },
	{ S1D8("QuenchTower"), 18.0f, 6.0f, 0.f, 0.f, 1.f },
	{ S1D8("LadleCar"), -9.0f, 6.0f, 0.f, 0.f, 1.f },
	{ S1D8("RivetForge"), -18.0f, 5.0f, 0.f, 0.f, 1.f },
	{ S1D8("AnvilRack"), -13.0f, 2.0f, 0.f, 0.f, 1.f },
	{ S1D8("FoundryLamp"), 3.0f, 2.5f, 0.f, 0.f, 1.f },
	{ S1D8("VentCowl"), 9.0f, 3.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8ClubSt"), {	// the club 68 m (its stair comes to 54.8 m); bollards 42 m, sign, tubes, two lamps 44 to 45 m
	{ S1D8("Club"), 0.0f, 24.0f, 0.f, 0.f, 1.f },
	{ S1D8("BollardChain"), 0.0f, -2.0f, 0.f, 0.f, 1.f },
	{ S1D8("SignPylon"), -17.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D8("TubeRack"), 17.0f, 1.0f, 0.f, 0.f, 1.f },
	{ S1D8("FoundryLamp"), -8.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D8("FoundryLamp"), 8.0f, 0.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8GateSt"), {	// the gate bastion 67 m; cowl, ingots, anvil rack, lamp 44.5 to 45 m, bollards 42 m
	{ S1D8("GateBastion"), 0.0f, 23.0f, 0.f, 0.f, 1.f },
	{ S1D8("VentCowl"), 15.0f, 1.0f, 0.f, 0.f, 1.f },
	{ S1D8("IngotStack"), -14.0f, 1.0f, 0.f, 0.f, 1.f },
	{ S1D8("BollardChain"), 1.0f, -2.0f, 0.f, 0.f, 1.f },
	{ S1D8("AnvilRack"), -5.0f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D8("FoundryLamp"), 8.0f, 0.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8SmithsSt"), {	// the smiths' row 48 m (41.2 to 53.5 m); sign and anvil rack 44 to 45 m, bollards 42 m
	{ S1D8("SmithsRow"), 3.0f, 4.0f, 0.f, 0.f, 1.f },
	{ S1D8("SignPylon"), 18.0f, 1.0f, 0.f, 0.f, 1.f },
	{ S1D8("AnvilRack"), -13.5f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D8("BollardChain"), -15.0f, -2.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8YardA"), {	// a works yard, props only: 43 to 45 m
	{ S1D8("TubeRack"), -8.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D8("LadleCar"), 5.0f, 1.0f, 0.f, 0.f, 1.f },
	{ S1D8("VentCowl"), 13.0f, 0.5f, 0.f, 0.f, 1.f },
	{ S1D8("AnvilRack"), -16.0f, -1.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8YardB"), {	// a works yard, props only: 41.5 to 45.5 m
	{ S1D8("IngotStack"), -8.0f, 1.0f, 0.f, 0.f, 1.f },
	{ S1D8("RivetForge"), 3.0f, 1.5f, 0.f, 0.f, 1.f },
	{ S1D8("FoundryLamp"), 10.0f, 0.0f, 0.f, 0.f, 1.f },
	{ S1D8("BollardChain"), -2.0f, -2.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8LapPower"), {	// hall 66 m, forecourt 54.5 to 56 m
	{ S1D8("PowerHall"), 0.0f, 22.0f, 0.f, 0.f, 1.f },
	{ S1D8("SignPylon"), -16.0f, 11.0f, 0.f, 0.f, 1.f },
	{ S1D8("IngotStack"), 17.5f, 12.0f, 0.f, 0.f, 1.f },
	{ S1D8("BollardChain"), 0.0f, 10.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8LapSmiths"), {	// row 67 m, forecourt 56.5 to 58 m
	{ S1D8("SmithsRow"), 0.0f, 23.0f, 0.f, 0.f, 1.f },
	{ S1D8("SignPylon"), 15.0f, 13.0f, 0.f, 0.f, 1.f },
	{ S1D8("AnvilRack"), -11.0f, 12.5f, 0.f, 0.f, 1.f },
	{ S1D8("RivetForge"), -18.0f, 14.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8LapFoundry"), {	// foundry 66 m, forecourt 54 to 57 m
	{ S1D8("Foundry"), 0.0f, 22.0f, 0.f, 0.f, 1.f },
	{ S1D8("VentCowl"), 15.0f, 11.0f, 0.f, 0.f, 1.f },
	{ S1D8("LadleCar"), -8.0f, 10.0f, 0.f, 0.f, 1.f },
	{ S1D8("RivetForge"), -17.0f, 13.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8LapQuench"), {	// quench tower 66 m and switch house 67 m, forecourt 56 to 56.5 m
	{ S1D8("QuenchTower"), -8.0f, 22.0f, 0.f, 0.f, 1.f },
	{ S1D8("SwitchHouse"), 8.0f, 23.0f, 0.f, 0.f, 1.f },
	{ S1D8("IngotStack"), -1.0f, 12.0f, 0.f, 0.f, 1.f },
	{ S1D8("TubeRack"), -10.0f, 12.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8LapGate"), {	// bastion 67 m, forecourt 57 to 58 m
	{ S1D8("GateBastion"), 0.0f, 23.0f, 0.f, 0.f, 1.f },
	{ S1D8("TubeRack"), -15.0f, 13.0f, 0.f, 0.f, 1.f },
	{ S1D8("VentCowl"), 15.0f, 13.0f, 0.f, 0.f, 1.f },
	{ S1D8("LadleCar"), 0.0f, 14.0f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8LapClub"), {	// club 69 m (its stair comes to 55.8 m), forecourt 54.5 to 56.5 m
	{ S1D8("Club"), 0.0f, 25.0f, 0.f, 0.f, 1.f },
	{ S1D8("FoundryLamp"), -9.0f, 12.0f, 0.f, 0.f, 1.f },
	{ S1D8("FoundryLamp"), 9.0f, 12.0f, 0.f, 0.f, 1.f },
	{ S1D8("SignPylon"), -16.0f, 12.5f, 0.f, 0.f, 1.f },
	{ S1D8("BollardChain"), 14.5f, 10.5f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1D8Hero"), {	// THE HOURGLASS
	{ S1D8("Hourglass"), 0.0f, 0.0f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1d8, Reactor Two, the copper street"), 21.4f, 22.18f);
	Site(TEXT("WldS1D8PowerSt"), 1, 44.0f, 8, 6, 1.0f);	// right, s % 8 == 2: the ProcessUnit set stands LEFT in these slots
	Site(TEXT("WldS1D8SmithsSt"), -1, 44.0f, 8, 6, 1.0f);	// left, s % 8 == 2: before the ProcessUnit (its nearest piece 54.5 m)
	Site(TEXT("WldS1D8FoundrySt"), -1, 44.0f, 8, 2, 1.0f);	// left, s % 8 == 6: the ProcessUnit set stands RIGHT
	Site(TEXT("WldS1D8SmithsSt"), 1, 44.0f, 8, 2, 1.0f);	// right, s % 8 == 6
	Site(TEXT("WldS1D8ClubSt"), 1, 44.0f, 8, 0, 1.0f);	// right, s % 8 == 0: the ReactorBlock set stands LEFT
	Site(TEXT("WldS1D8YardA"), -1, 44.0f, 8, 0, 1.0f);	// left, s % 8 == 0: props only, before the ReactorBlock (from 62 m)
	Site(TEXT("WldS1D8GateSt"), -1, 44.0f, 8, 4, 1.0f);	// left, s % 8 == 4: the ReactorBlock set stands RIGHT
	Site(TEXT("WldS1D8YardB"), 1, 44.0f, 8, 4, 1.0f);	// right, s % 8 == 4: props only
Zone(TEXT("World s1d8, Reactor Two, the second lap"), 22.18f, 25.58f);
	Site(TEXT("WldS1D8LapPower"), 1, 44.0f, 16, 14, 0.85f);	// right, s % 16 == 2
	Site(TEXT("WldS1D8LapSmiths"), 1, 44.0f, 16, 6, 0.85f);	// right, s % 16 == 10
	Site(TEXT("WldS1D8LapFoundry"), -1, 44.0f, 16, 10, 0.85f);	// left, s % 16 == 6
	Site(TEXT("WldS1D8LapQuench"), -1, 44.0f, 16, 2, 0.85f);	// left, s % 16 == 14
	Site(TEXT("WldS1D8LapGate"), 1, 44.0f, 16, 0, 0.85f);	// right, s % 16 == 0
	Site(TEXT("WldS1D8LapClub"), 1, 44.0f, 16, 8, 0.85f);	// right, s % 16 == 8
	Site(TEXT("WldS1D8LapClub"), -1, 46.5f, 16, 12, 0.85f);	// left, s % 16 == 4. fix round: 2.5 m further out (the club 71.5 m: clear of the old water tower)
	Site(TEXT("WldS1D8LapGate"), -1, 44.0f, 16, 4, 0.85f);	// left, s % 16 == 12
Zone(TEXT("World s1d8, Reactor Two, the last 160 m"), 25.58f, 25.76f);
	Site(TEXT("WldS1D8PowerSt"), 1, 44.0f, 8, 6, 1.0f);	// right, s % 8 == 2: the ProcessUnit set stands LEFT in these slots
	Site(TEXT("WldS1D8SmithsSt"), -1, 44.0f, 8, 6, 1.0f);	// left, s % 8 == 2: before the ProcessUnit (its nearest piece 54.5 m)
	Site(TEXT("WldS1D8FoundrySt"), -1, 44.0f, 8, 2, 1.0f);	// left, s % 8 == 6: the ProcessUnit set stands RIGHT
	Site(TEXT("WldS1D8SmithsSt"), 1, 44.0f, 8, 2, 1.0f);	// right, s % 8 == 6
	Site(TEXT("WldS1D8ClubSt"), 1, 44.0f, 8, 0, 1.0f);	// right, s % 8 == 0: the ReactorBlock set stands LEFT
	Site(TEXT("WldS1D8YardA"), -1, 44.0f, 8, 0, 1.0f);	// left, s % 8 == 0: props only, before the ReactorBlock (from 62 m)
	Site(TEXT("WldS1D8GateSt"), -1, 44.0f, 8, 4, 1.0f);	// left, s % 8 == 4: the ReactorBlock set stands RIGHT
	Site(TEXT("WldS1D8YardB"), 1, 44.0f, 8, 4, 1.0f);	// right, s % 8 == 4: props only
Zone(TEXT("World s1d8, the Hourglass 22.17"), 22.1646f, 22.1706f);
	Site(TEXT("WldS1D8Hero"), 1, 185.f, 1, 0, 1.f);
Zone(TEXT("World s1d8, the Hourglass 23.62"), 23.6123f, 23.6183f);
	Site(TEXT("WldS1D8Hero"), 1, 185.f, 1, 0, 1.f);
Zone(TEXT("World s1d8, the Hourglass 25.42"), 25.4219f, 25.4279f);
	Site(TEXT("WldS1D8Hero"), 1, 185.f, 1, 0, 1.f);
#undef S1D8
// ---- END s1d8 ----
}

{
// ---- BEGIN s1plants (the night planting of the eight districts, ft_world_chem_s1plants.py) ----
// Written by C:/ftwork/jobs/L1s1plants/make_inl.py. 22 meshes, all Scatter, chance 1, one side a rule, slots s % 4 == 3 only (every 4 phase 1;
// every 8 phase 5 = s % 8 == 3; every 8 phase 1 = s % 8 == 7). Counted slot by slot: 786 rule-slots = 786 pieces before the track leaves any out;
// x 0.75 (the share the track leaves out) = about 590 placed. Rule-slots by district: Factory Gate 70, Reactor Block 60, Loading Yards 78, Waste Run 56, Export Terminal 135, Cooling Field 140, Tank Farm B 143, Reactor Two 104.
// The most of one mesh, before / after x 0.75: Cypress 59 / 44, MopTrees 55 / 41, Poplars 54 / 40, ReedBed 49 / 37.
// Bands (m from the centre line). LOW strips (0.8 to 1 m deep, top under 1.9 m): 23.3 to 23.5, between the kerb's yard (to 22.95) and the low
// line of people (24.5). TALL: chosen district by district and side by side where the two tables have the fewest rules on these slots
// (33 to 60 m; the smallest x min of a TALL mesh is 33.2). Wall turns and jumps (30 m of margin): only rules 40 m out and more.
// Nothing at the mega vat (7.88 to 8.12 km) or in the flare avenue (12.69 to 13.88 km); from 22.15 km (the doubled circuit) 55 m out and more.
// fix round: the five low plants 1.8 to 1.9 m tall (Allotment, ReedBed, PrivetHedge, TopiaryTubs, Thistles) stand in about every second of
// their slots (chance 0.5): about half of them stood directly in front of the older low line of people at 24 to 27 m.
#define S1PLANTS(n) TEXT("/Game/World/ChemicalWorks/SM_S1PLANTS_" n)
Zone(TEXT("World s1plants, Factory Gate"), 0.0000f, 2.5561f);   // slots 0 to 56
	Scatter(S1PLANTS("CobbleWeeds"), 23.3f, 23.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("CobbleWeeds"), 23.3f, 23.5f, 8, 5, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("PrivetHedge"), 23.3f, 23.5f, 8, 1, 0.5f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("PrivetHedge"), 23.3f, 23.5f, 8, 1, 0.5f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("PlaneTree"), 47.0f, 51.0f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("PlaneTree"), 47.0f, 51.0f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("PlaneTree"), 55.0f, 58.5f, 4, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("PlaneTree"), 55.0f, 58.5f, 4, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Reactor Block, the first wall turn"), 2.6013f, 3.0085f);   // slots 58 to 66
	Scatter(S1PLANTS("Pergola"), 41.6f, 42.6f, 8, 1, 1.f, 1.0f, 0.00f, -1, 5.0f);
	Scatter(S1PLANTS("Poplars"), 45.5f, 47.5f, 4, 1, 1.f, 1.0f, 0.08f, -1, 5.0f);
	Scatter(S1PLANTS("Poplars"), 46.0f, 48.0f, 4, 1, 1.f, 1.0f, 0.08f, 1, 5.0f);
	Scatter(S1PLANTS("Pergola"), 56.5f, 58.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 5.0f);
Zone(TEXT("World s1plants, Reactor Block"), 3.0085f, 3.5513f);   // slots 67 to 78
	Scatter(S1PLANTS("PrivetHedge"), 23.3f, 23.5f, 8, 5, 0.5f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("PrivetHedge"), 23.3f, 23.5f, 8, 5, 0.5f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("TopiaryTubs"), 23.3f, 23.5f, 8, 1, 0.5f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("TopiaryTubs"), 23.3f, 23.5f, 8, 1, 0.5f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Pergola"), 34.8f, 37.2f, 8, 5, 1.f, 1.0f, 0.00f, 1, 5.0f);
	Scatter(S1PLANTS("Pergola"), 41.6f, 42.6f, 8, 1, 1.f, 1.0f, 0.00f, -1, 5.0f);
	Scatter(S1PLANTS("Poplars"), 45.5f, 47.5f, 4, 1, 1.f, 1.0f, 0.08f, -1, 5.0f);
	Scatter(S1PLANTS("Poplars"), 46.0f, 48.0f, 4, 1, 1.f, 1.0f, 0.08f, 1, 5.0f);
	Scatter(S1PLANTS("Pergola"), 56.5f, 58.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 5.0f);
Zone(TEXT("World s1plants, Reactor Block, the second wall turn and the first jump"), 3.5513f, 4.7728f);   // slots 79 to 105
	Scatter(S1PLANTS("Pergola"), 41.6f, 42.6f, 8, 1, 1.f, 1.0f, 0.00f, -1, 5.0f);
	Scatter(S1PLANTS("Poplars"), 45.5f, 47.5f, 4, 1, 1.f, 1.0f, 0.08f, -1, 5.0f);
	Scatter(S1PLANTS("Poplars"), 46.0f, 48.0f, 4, 1, 1.f, 1.0f, 0.08f, 1, 5.0f);
	Scatter(S1PLANTS("Pergola"), 56.5f, 58.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 5.0f);
Zone(TEXT("World s1plants, Reactor Block"), 4.7728f, 5.3609f);   // slots 106 to 118
	Scatter(S1PLANTS("PrivetHedge"), 23.3f, 23.5f, 8, 5, 0.5f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("PrivetHedge"), 23.3f, 23.5f, 8, 5, 0.5f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("TopiaryTubs"), 23.3f, 23.5f, 8, 1, 0.5f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("TopiaryTubs"), 23.3f, 23.5f, 8, 1, 0.5f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Pergola"), 34.8f, 37.2f, 8, 5, 1.f, 1.0f, 0.00f, 1, 5.0f);
	Scatter(S1PLANTS("Pergola"), 41.6f, 42.6f, 8, 1, 1.f, 1.0f, 0.00f, -1, 5.0f);
	Scatter(S1PLANTS("Poplars"), 45.5f, 47.5f, 4, 1, 1.f, 1.0f, 0.08f, -1, 5.0f);
	Scatter(S1PLANTS("Poplars"), 46.0f, 48.0f, 4, 1, 1.f, 1.0f, 0.08f, 1, 5.0f);
	Scatter(S1PLANTS("Pergola"), 56.5f, 58.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 5.0f);
Zone(TEXT("World s1plants, Loading Yards"), 5.3609f, 7.2610f);   // slots 119 to 160
	Scatter(S1PLANTS("CobbleWeeds"), 23.3f, 23.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("CobbleWeeds"), 23.3f, 23.5f, 8, 5, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Thistles"), 23.3f, 23.5f, 8, 1, 0.5f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("Thistles"), 23.3f, 23.5f, 8, 1, 0.5f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("PollardLimes"), 51.5f, 53.0f, 4, 1, 1.f, 1.0f, 0.06f, -1, 5.0f);
	Scatter(S1PLANTS("Buddleia"), 56.0f, 58.0f, 4, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("PollardLimes"), 46.0f, 47.2f, 4, 1, 1.f, 1.0f, 0.06f, 1, 5.0f);
	Scatter(S1PLANTS("Buddleia"), 57.5f, 59.5f, 4, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, Loading Yards, the second jump"), 7.2610f, 7.8944f);   // slots 161 to 174
	Scatter(S1PLANTS("PollardLimes"), 51.5f, 53.0f, 4, 1, 1.f, 1.0f, 0.06f, -1, 5.0f);
	Scatter(S1PLANTS("Buddleia"), 56.0f, 58.0f, 4, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("PollardLimes"), 46.0f, 47.2f, 4, 1, 1.f, 1.0f, 0.06f, 1, 5.0f);
	Scatter(S1PLANTS("Buddleia"), 57.5f, 59.5f, 4, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, Waste Run"), 8.1206f, 9.4778f);   // slots 180 to 209
	Scatter(S1PLANTS("Fungi"), 23.3f, 23.5f, 8, 5, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("ReedBed"), 23.3f, 23.5f, 8, 5, 0.5f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Thistles"), 23.3f, 23.5f, 8, 1, 0.5f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("Fungi"), 23.3f, 23.5f, 8, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("ReedsTall"), 46.2f, 47.6f, 8, 5, 1.f, 1.0f, 0.06f, -1, 5.0f);
	Scatter(S1PLANTS("Buddleia"), 46.0f, 47.5f, 8, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("DeadTree"), 49.6f, 50.6f, 4, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("DeadTree"), 57.0f, 59.5f, 4, 1, 1.f, 1.0f, 0.12f, -1, 180.0f);
	Scatter(S1PLANTS("Buddleia"), 49.8f, 51.2f, 8, 5, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("ReedsTall"), 49.6f, 51.0f, 8, 1, 1.f, 1.0f, 0.06f, 1, 5.0f);
	Scatter(S1PLANTS("DeadTree"), 53.5f, 55.0f, 4, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
	Scatter(S1PLANTS("DeadTree"), 57.5f, 59.5f, 4, 1, 1.f, 1.0f, 0.12f, 1, 180.0f);
Zone(TEXT("World s1plants, Export Terminal"), 9.4778f, 12.6898f);   // slots 210 to 280
	Scatter(S1PLANTS("ReedBed"), 23.3f, 23.5f, 4, 1, 0.5f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("ReedBed"), 23.3f, 23.5f, 4, 1, 0.5f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Tamarisk"), 33.2f, 34.0f, 4, 1, 1.f, 1.0f, 0.10f, -1, 10.0f);
	Scatter(S1PLANTS("Tamarisk"), 33.2f, 34.0f, 4, 1, 1.f, 1.0f, 0.10f, 1, 10.0f);
	Scatter(S1PLANTS("Cypress"), 53.0f, 55.5f, 8, 5, 1.f, 1.0f, 0.08f, -1, 5.0f);
	Scatter(S1PLANTS("Cypress"), 53.0f, 55.5f, 8, 5, 1.f, 1.0f, 0.08f, 1, 5.0f);
	Scatter(S1PLANTS("WindPine"), 53.0f, 55.5f, 8, 1, 1.f, 1.0f, 0.12f, -1, 12.0f);
	Scatter(S1PLANTS("WindPine"), 53.0f, 55.5f, 8, 1, 1.f, 1.0f, 0.12f, 1, 12.0f);
	Scatter(S1PLANTS("WindPine"), 56.8f, 59.0f, 8, 5, 1.f, 1.0f, 0.12f, -1, 12.0f);
	Scatter(S1PLANTS("WindPine"), 56.8f, 59.0f, 8, 5, 1.f, 1.0f, 0.12f, 1, 12.0f);
	Scatter(S1PLANTS("Cypress"), 57.0f, 59.4f, 8, 1, 1.f, 1.0f, 0.08f, -1, 5.0f);
Zone(TEXT("World s1plants, Cooling Field"), 13.8661f, 17.3948f);   // slots 307 to 384
	Scatter(S1PLANTS("TopiaryTubs"), 23.3f, 23.5f, 8, 5, 0.5f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("TopiaryTubs"), 23.3f, 23.5f, 8, 5, 0.5f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("TopiaryTubs"), 23.3f, 23.5f, 8, 1, 0.5f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("ReedBed"), 23.3f, 23.5f, 8, 1, 0.5f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("TreeFerns"), 45.0f, 46.5f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("TreeFerns"), 45.0f, 46.5f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("Willow"), 53.0f, 56.0f, 8, 5, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Willow"), 53.0f, 56.0f, 8, 5, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("ReedsTall"), 44.6f, 46.0f, 8, 1, 1.f, 1.0f, 0.06f, -1, 5.0f);
	Scatter(S1PLANTS("ReedsTall"), 44.6f, 46.0f, 8, 1, 1.f, 1.0f, 0.06f, 1, 5.0f);
	Scatter(S1PLANTS("Willow"), 49.0f, 51.5f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("Willow"), 49.0f, 51.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
	Scatter(S1PLANTS("TreeFerns"), 56.5f, 58.5f, 8, 1, 1.f, 1.0f, 0.10f, -1, 180.0f);
	Scatter(S1PLANTS("TreeFerns"), 56.5f, 58.5f, 8, 1, 1.f, 1.0f, 0.10f, 1, 180.0f);
Zone(TEXT("World s1plants, Tank Farm B"), 17.3948f, 21.4211f);   // slots 385 to 473
	Scatter(S1PLANTS("Allotment"), 23.3f, 23.5f, 4, 1, 0.5f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("Allotment"), 23.3f, 23.5f, 4, 1, 0.5f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Sunflowers"), 44.2f, 45.4f, 4, 1, 1.f, 1.0f, 0.06f, -1, 5.0f);
	Scatter(S1PLANTS("Sunflowers"), 44.2f, 45.4f, 4, 1, 1.f, 1.0f, 0.06f, 1, 5.0f);
	Scatter(S1PLANTS("MopTrees"), 47.5f, 50.0f, 4, 1, 1.f, 1.0f, 0.08f, -1, 8.0f);
	Scatter(S1PLANTS("MopTrees"), 47.5f, 50.0f, 4, 1, 1.f, 1.0f, 0.08f, 1, 8.0f);
	Scatter(S1PLANTS("MopTrees"), 56.5f, 59.0f, 8, 5, 1.f, 1.0f, 0.08f, -1, 8.0f);
Zone(TEXT("World s1plants, Reactor Two"), 21.4211f, 22.1450f);   // slots 474 to 489
	Scatter(S1PLANTS("Agaves"), 23.3f, 23.5f, 4, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("Agaves"), 23.3f, 23.5f, 4, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Cypress"), 39.6f, 40.8f, 4, 1, 1.f, 1.0f, 0.08f, -1, 5.0f);
	Scatter(S1PLANTS("Cypress"), 34.6f, 35.8f, 4, 1, 1.f, 1.0f, 0.08f, 1, 5.0f);
	Scatter(S1PLANTS("Poplars"), 52.5f, 55.0f, 8, 5, 1.f, 1.0f, 0.08f, -1, 5.0f);
	Scatter(S1PLANTS("Poplars"), 52.5f, 55.0f, 8, 5, 1.f, 1.0f, 0.08f, 1, 5.0f);
	Scatter(S1PLANTS("Cypress"), 56.5f, 58.6f, 8, 1, 1.f, 1.0f, 0.08f, -1, 5.0f);
	Scatter(S1PLANTS("Cypress"), 56.5f, 58.6f, 8, 1, 1.f, 1.0f, 0.08f, 1, 5.0f);
Zone(TEXT("World s1plants, Reactor Two, beside the other lap"), 22.1450f, 25.7642f);   // slots 490 to 569
	Scatter(S1PLANTS("Agaves"), 55.3f, 55.8f, 4, 1, 1.f, 1.0f, 0.00f, -1, 0.0f);
	Scatter(S1PLANTS("Agaves"), 55.3f, 55.8f, 4, 1, 1.f, 1.0f, 0.00f, 1, 0.0f);
	Scatter(S1PLANTS("Cypress"), 56.6f, 58.0f, 8, 5, 1.f, 1.0f, 0.08f, -1, 5.0f);
	Scatter(S1PLANTS("Cypress"), 56.6f, 58.0f, 8, 5, 1.f, 1.0f, 0.08f, 1, 5.0f);
	Scatter(S1PLANTS("Poplars"), 57.0f, 58.5f, 8, 1, 1.f, 1.0f, 0.08f, -1, 5.0f);
	Scatter(S1PLANTS("Poplars"), 57.0f, 58.5f, 8, 1, 1.f, 1.0f, 0.08f, 1, 5.0f);
#undef S1PLANTS
// ---- END s1plants ----
}

{
// ---- BEGIN s1animals (the animals of the eight districts, ft_world_chem_s1animals.py) ----
// 19 meshes, /Game/World/ChemicalWorks/SM_S1ANIMALS_*. Estimate (slots counted one by one; Scatter x 0.75, Far x 1): about 306 placed.
// By mesh: WallCats 32, HenRun 26, Doves 24, KoiPool 21, Salamanders 21, GullWheel 19, Greyhounds 18, StorkStack 16, Swans 15, SealSlip 14, PelicanPiles 14, WhiteOwl 13, LampMoths 13, PipeRats 12, GateGoat 10, LabRabbits 10, Flamingos 10, LagoonFrogs 10, Carthorse 8.
// Bands and slots (the brief's): ground and water 40 to 45 m in s%8==5 (phase 3 of 8; 3 or 11 of 16); 46 to 120 m in s%4==1 (phase 3 of 4:
// 3 or 7 of 8) and, from 61 m, also s%4==3 (phase 1 of 4: 1 or 5 of 8). NOTHING nearer than 40 m: this road is banked nearly everywhere and
// 17 to 40 m floats over the cutting. The sky (Far, a negative drop): the owl 95 m and the gulls 110 m and more out (250 m in the arena).
// TALL meshes (the make log) and their smallest x min: Carthorse 40, Flamingos 40, Swans 40, WallCats 48, Salamanders 51, PelicanPiles 61, LampMoths 62, Doves 72, StorkStack 80, GullWheel 110; the other meshes are low.
// Light: steady only (the vents of Salamanders 51 m, the pane of StorkStack 80 m, the lamp of LampMoths 62 m out). Nothing blinks.
// No mesh stands both inside and outside 70 m. Every traveller keeps inside its own piece (pipe mouth to pipe mouth, box to box, fin to fin).
#define S1ANIMALS(n) TEXT("/Game/World/ChemicalWorks/SM_S1ANIMALS_" n)
// -- s1d1, Factory Gate (the company town). The clock gate stands RIGHT near 0.72 km: the owl keeps LEFT.
Zone(TEXT("World s1animals, Factory Gate"), 0.1f, 2.6f);
	Scatter(S1ANIMALS("GateGoat"), 40.f, 44.f, 8, 3, 1.f, 1.f, 0.05f, 0, 25.f);   // the gate goat and her kid on the verge: s%8==5
	Scatter(S1ANIMALS("WallCats"), 48.f, 54.f, 8, 7, 1.f, 1.f, 0.f, 0, 0.f);   // TALL: three cats on a garden wall: s%8==1
	Far(S1ANIMALS("WhiteOwl"), 95.f, 130.f, 8, 6, 1.f, 1.f, 0.f, -1, -25.f, 180.f);   // one barn owl 25 m over the road, a 20 m circle: 95 - 20 = 75 m from the centre line at the nearest
// -- s1d2, Reactor Block (the research quarter). Only the level plateau and the stretch after the jump: nothing beside the
//    wall turns (2.62 to 2.99, 3.59 to 3.94 km) or the jump (4.12 to 4.75 km). The great retort stands RIGHT of the plateau and the
//    reactor core LEFT of 5.05 to 5.35 km: lamp and owl keep to the other side.
Zone(TEXT("World s1animals, Reactor Block, the plateau"), 3.f, 3.58f);
	Scatter(S1ANIMALS("LabRabbits"), 40.f, 44.f, 8, 3, 1.f, 1.f, 0.f, 0, 10.f);   // laboratory rabbits on their moss lawn
	Scatter(S1ANIMALS("LabRabbits"), 47.f, 53.f, 8, 7, 1.f, 1.f, 0.f, 0, 10.f);
	Scatter(S1ANIMALS("KoiPool"), 52.f, 58.f, 8, 3, 1.f, 1.f, 0.f, 0, 180.f);   // a koi pool behind the lawn
	Scatter(S1ANIMALS("LampMoths"), 62.f, 69.f, 4, 1, 1.f, 1.f, 0.f, -1, 20.f);   // TALL, a steady lamp: moths round it, 13 to 15 m up, a 2.5 m circle: s%4==3
	Far(S1ANIMALS("WhiteOwl"), 95.f, 130.f, 4, 2, 1.f, 1.f, 0.f, -1, -25.f, 180.f);
Zone(TEXT("World s1animals, Reactor Block, after the jump"), 4.75f, 5.35f);
	Scatter(S1ANIMALS("LabRabbits"), 40.f, 44.f, 8, 3, 1.f, 1.f, 0.f, 0, 10.f);
	Scatter(S1ANIMALS("LabRabbits"), 47.f, 53.f, 8, 7, 1.f, 1.f, 0.f, 0, 10.f);
	Scatter(S1ANIMALS("KoiPool"), 52.f, 58.f, 8, 3, 1.f, 1.f, 0.f, 0, 180.f);
	Scatter(S1ANIMALS("LampMoths"), 62.f, 69.f, 4, 1, 1.f, 1.f, 0.f, 1, 20.f);
	Far(S1ANIMALS("WhiteOwl"), 95.f, 130.f, 4, 2, 1.f, 1.f, 0.f, 1, -25.f, 180.f);
// -- s1d3, Loading Yards (the freight town), to 7.28 km: nothing beside the second jump or the mega vat. The great scales stand RIGHT.
Zone(TEXT("World s1animals, Loading Yards"), 5.35f, 7.28f);
	Scatter(S1ANIMALS("Carthorse"), 40.f, 44.f, 8, 3, 1.f, 1.f, 0.f, 0, 0.f);   // TALL: the carthorse at the yard trough, its dray behind
	Scatter(S1ANIMALS("PipeRats"), 47.f, 52.f, 8, 7, 1.f, 1.f, 0.f, 0, 0.f);   // rats in a file along a pipe: s%8==1
	Scatter(S1ANIMALS("WallCats"), 48.f, 54.f, 8, 3, 1.f, 1.f, 0.f, 0, 0.f);
	Far(S1ANIMALS("GullWheel"), 110.f, 160.f, 6, 4, 1.f, 1.f, 0.f, -1, -35.f, 180.f);   // five pale gulls 35 to 39 m over the road, a 14.5 m wheel: 110 - 15 = 95 m at the nearest
// -- s1d4, Waste Run (the sludge works), after the third wall turn and short of the run-in to the boss.
Zone(TEXT("World s1animals, Waste Run"), 8.12f, 9.3f);
	Scatter(S1ANIMALS("Flamingos"), 40.f, 45.f, 8, 3, 1.f, 1.f, 0.f, 0, 180.f);   // TALL: flamingos in a liquor pool
	Scatter(S1ANIMALS("LagoonFrogs"), 52.f, 58.f, 8, 3, 1.f, 1.f, 0.f, 0, 180.f);   // three-eyed frogs on lily pads
	Scatter(S1ANIMALS("Flamingos"), 54.f, 62.f, 8, 7, 1.f, 1.f, 0.f, 0, 180.f);
	Scatter(S1ANIMALS("LagoonFrogs"), 61.f, 66.f, 4, 1, 1.f, 1.f, 0.f, -1, 180.f);   // s%4==3, from 61 m
	Scatter(S1ANIMALS("PipeRats"), 61.f, 66.f, 4, 1, 1.f, 1.f, 0.f, 1, 0.f);
// -- s1d5, Export Terminal (the port, the boss's arena), to 12.72 km: nothing on the flare avenue. Slow movement, no light,
//    nothing round or lit; the gulls 250 m and more out, where the review put the gull towers.
Zone(TEXT("World s1animals, Export Terminal"), 9.46f, 12.72f);
	Scatter(S1ANIMALS("SealSlip"), 40.f, 45.f, 8, 3, 1.f, 1.f, 0.f, 0, 0.f);   // seals at a quay landing (its steps go down away from the road)
	Scatter(S1ANIMALS("WallCats"), 48.f, 54.f, 8, 7, 1.f, 1.f, 0.f, 0, 0.f);
	Scatter(S1ANIMALS("PelicanPiles"), 61.f, 67.f, 8, 1, 1.f, 1.f, 0.f, 0, 30.f);   // TALL: a pelican on mooring piles: s%8==7
	Far(S1ANIMALS("GullWheel"), 250.f, 320.f, 12, 7, 1.f, 1.f, 0.f, 0, -40.f, 180.f);
// -- s1d6, Cooling Field (the spa). The cascade stands RIGHT near 15.4 km and the column trios LEFT, 70 to 125 m out:
//    the stork stacks stand RIGHT, away from the cascade.
Zone(TEXT("World s1animals, Cooling Field"), 13.85f, 17.4f);
	Scatter(S1ANIMALS("Swans"), 40.f, 45.f, 8, 3, 1.f, 1.f, 0.f, 0, 180.f);   // TALL: swans and cygnets round a steaming basin
	Scatter(S1ANIMALS("Salamanders"), 51.f, 55.f, 16, 3, 1.f, 1.f, 0.f, 0, 0.f);   // TALL: salamanders on a warm wall, steady vents: s%16==13
	Scatter(S1ANIMALS("KoiPool"), 53.f, 58.f, 16, 11, 1.f, 1.f, 0.f, 0, 180.f);   // s%16==5
	Scatter(S1ANIMALS("KoiPool"), 47.f, 55.f, 16, 7, 1.f, 1.f, 0.f, 0, 180.f);   // s%16==9
	Scatter(S1ANIMALS("LampMoths"), 62.f, 69.f, 16, 1, 1.f, 1.f, 0.f, 0, 20.f);   // s%16==15
Zone(TEXT("World s1animals, Cooling Field, before the cascade"), 13.85f, 15.1f);
	Scatter(S1ANIMALS("StorkStack"), 80.f, 110.f, 4, 1, 1.f, 1.f, 0.f, 1, 180.f);   // TALL: a brick stack, a stork on its nest, its mate gliding a 9.5 m circle 19 m up: 80 - 10 = 70 m at the nearest
Zone(TEXT("World s1animals, Cooling Field, after the cascade"), 15.7f, 17.4f);
	Scatter(S1ANIMALS("StorkStack"), 80.f, 110.f, 4, 1, 1.f, 1.f, 0.f, 1, 180.f);
// -- s1d7, Tank Farm B (the round town). The ground flare, the column trios and the second core are LEFT, the kettle RIGHT near 19.5 km.
Zone(TEXT("World s1animals, Tank Farm B"), 17.4f, 21.4f);
	Scatter(S1ANIMALS("HenRun"), 40.f, 45.f, 8, 3, 1.f, 1.f, 0.f, 0, 180.f);   // a hen run round a tank hen-house
	Scatter(S1ANIMALS("HenRun"), 48.f, 56.f, 16, 7, 1.f, 1.f, 0.f, 0, 180.f);   // s%16==9
	Scatter(S1ANIMALS("Doves"), 72.f, 100.f, 8, 7, 1.f, 1.f, 0.f, 0, 180.f);   // TALL: a drum dovecote, its flight circling 6 to 8.6 m out, 6.6 to 8.9 m up: 72 - 9 = 63 m at the nearest
Zone(TEXT("World s1animals, Tank Farm B, between the flare and the core"), 18.7f, 20.9f);
	Scatter(S1ANIMALS("StorkStack"), 80.f, 110.f, 8, 5, 1.f, 1.f, 0.f, -1, 180.f);   // s%8==3
// -- s1d8, Reactor Two (the copper quarter). Its own ground first; from 22.18 km (the doubled circuit) only 56 m and more out,
//    and nothing that flies.
Zone(TEXT("World s1animals, Reactor Two, its own ground"), 21.4f, 22.18f);
	Scatter(S1ANIMALS("Greyhounds"), 40.f, 45.f, 8, 3, 1.f, 1.f, 0.f, 0, 0.f);   // two greyhounds on a stepped plinth
	Scatter(S1ANIMALS("Doves"), 72.f, 100.f, 4, 3, 1.f, 1.f, 0.f, 0, 180.f);
Zone(TEXT("World s1animals, Reactor Two, the doubled circuit"), 22.18f, 25.6f);
	Scatter(S1ANIMALS("Greyhounds"), 67.f, 69.5f, 8, 3, 1.f, 1.f, 0.f, 0, 0.f);   // fix round: 67 to 69.5 m (at 56 to 64 they met the old crane baskets and column decks at 61 m)
	Scatter(S1ANIMALS("Salamanders"), 67.f, 69.5f, 8, 7, 1.f, 1.f, 0.f, 0, 0.f);   // fix round: 67 to 69.5 m, inside 70 m like its other rules (clear of the old pipe bridges at 61 m)
#undef S1ANIMALS
// ---- END s1animals ----
}

{
// ---- BEGIN s1people (the people pass 3 left out, ft_world_chem_s1people.py) ----
// 21 meshes: for each of seven districts two raised terraces (the floor 3.2 m over the land) and one tower (the floor 12.5 m up).
// All TALL, all 6.8 m long at most and 3.4 m deep, 3 m of footing. Light steady and small (windows, lamps). No Lit, no Bank.
// Slots: s % 8 == 1 only. Every Site is a rule of its own that fits one slot (every 1000, phase 1000 - slot: phase % 8 == 7), because
// what stands in that class already differs slot by slot (C:/ftwork/jobs/L1s1people/plan.py reads it from both tables).
// Distance: every Site is anchored 43.3 m from the centre line, every piece at out 0: the pieces reach from 41.6 to 45.0 m (a hand or a
// hat brim to 41.3): behind the banked bends' cuttings (40 m), the loading bays (to 41.5 m), the light masts (to 40 m); before the
// animals' band (46 m). The smallest x min of a TALL mesh: 43.3 m.
// Along the slot (a Site stands at the slot's own point, s x 45.243 m): Full = terrace, terrace, tower, terrace, terrace at -18.4, -9.2,
// 0, 9.2, 18.4 m (a 43.6 m street; a Scatter piece of the next slot comes no nearer than 24.7 m); Ends = two terraces at -18.4 and
// 18.4 m where a blow-down stack (41 to 43 m, within 13.3 m of the point) stands in the slot, in the flare avenue and on the reactor
// core's side (no tower there); Plus / Minus = tower and two terraces on the side of the point away from a rotary kiln in the next slot
// (59 m long, its pad from 42.4 m); One = one terrace where a slot has both.
// LEFT OUT, a side of a slot: where a watcher (40 to 40.8 m, to 44.1) or a tall piece of p3a (44 m) stands (parsed from the WATCH_AT and
// P3A_AT rules); the wall turns' outsides (flare derricks 50 m out, 22 m square); the two jumps (4.12 to 4.75, 7.60 to 7.94 km);
// the mega vat and the third wall turn (7.94 to 8.12); the flare derricks' slots (s % 3 == 1 in 12.72 to 13.85) and the stair tower of p3a there (left, slot 297); the yard dogs (44 to
// 54 m, left, slots 473 and 561); ALL of Tank Farm B (17.4 to 21.4 km: in every slot of the class a film unit at 41.3 m (s % 3 == 1), a
// high way of p3c at 39.8 m (s % 12 == 5) or a watcher on both sides (393, 417, 441, 465)): no piece was made for it.
// NOT DODGED (the dressing scatters them over a range of distances, so a slot cannot be chosen against them): road tankers 28 to 44 m
// (export terminal: every odd slot; gate, loading yards), single tanks 40 to 75 m (tank farm), drum piles 29.5 to 46 m (waste run),
// platforms and substations 40 to 62 m (reactor block, reactor two), pump houses, the access road (43 m). Behind a tank row (34 m) or
// a loading bay (36.5 m) only the tower and the row's ends may show.
// Estimate, counted site by site (a site = one side of one slot): Factory Gate 57; Reactor Block 13; Loading Yards 32; Waste Run 19; Export Terminal 58; Cooling Field 29; Reactor Two 60.
//   Sum 268 piece-places x 0.75 (the track's share left out) = about 201 placed. By kind: Full 42 sites, Ends 14 sites, Plus 4 sites, Minus 5 sites, OnePlus 2 sites, OneMinus 1 sites.
//   The most of one mesh: SM_S1PEOPLE_MissionSteps, 24 places before the 0.75 (about 18).
//   By mesh: Doorstep 23, ClockOn 23, HoseTower 11, Blackboard 6, FlaskWalk 5, Observatory 2, CafeHatch 14, TallyDock 12, SignalCabin 6, SkimJetty 8, DrumWash 8, AssayTower 3, MissionSteps 24, CustomsDesk 24, HoistLoft 10, ChessTerrace 11, PlungeRim 11, Belvedere 7, ClubTerrace 24, ForgeStep 24, ChoirBastion 12.
#define S1PEOPLE(n) TEXT("/Game/World/ChemicalWorks/SM_S1PEOPLE_" n)
Set(TEXT("WldS1PEOPLED1Full"), {
	{ S1PEOPLE("Doorstep"), -18.4f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("ClockOn"), -9.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("HoseTower"), 0.0f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Doorstep"), 9.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("ClockOn"), 18.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED1Ends"), {
	{ S1PEOPLE("Doorstep"), -18.4f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("ClockOn"), 18.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED1Minus"), {
	{ S1PEOPLE("HoseTower"), -1.0f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("ClockOn"), -10.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Doorstep"), -19.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED2Ends"), {
	{ S1PEOPLE("Blackboard"), -18.4f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("FlaskWalk"), 18.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED2Plus"), {
	{ S1PEOPLE("Observatory"), 1.0f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("FlaskWalk"), 10.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Blackboard"), 19.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED2Minus"), {
	{ S1PEOPLE("Observatory"), -1.0f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("FlaskWalk"), -10.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Blackboard"), -19.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED2OnePlus"), {
	{ S1PEOPLE("Blackboard"), 18.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED3Full"), {
	{ S1PEOPLE("CafeHatch"), -18.4f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("TallyDock"), -9.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("SignalCabin"), 0.0f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("CafeHatch"), 9.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("TallyDock"), 18.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED3OnePlus"), {
	{ S1PEOPLE("CafeHatch"), 18.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED3OneMinus"), {
	{ S1PEOPLE("CafeHatch"), -18.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED4Full"), {
	{ S1PEOPLE("SkimJetty"), -18.4f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("DrumWash"), -9.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("AssayTower"), 0.0f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("SkimJetty"), 9.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("DrumWash"), 18.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED4Ends"), {
	{ S1PEOPLE("SkimJetty"), -18.4f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("DrumWash"), 18.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED5Full"), {
	{ S1PEOPLE("MissionSteps"), -18.4f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("CustomsDesk"), -9.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("HoistLoft"), 0.0f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("MissionSteps"), 9.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("CustomsDesk"), 18.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED5Ends"), {
	{ S1PEOPLE("MissionSteps"), -18.4f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("CustomsDesk"), 18.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED6Full"), {
	{ S1PEOPLE("ChessTerrace"), -18.4f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PlungeRim"), -9.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("Belvedere"), 0.0f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("ChessTerrace"), 9.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PlungeRim"), 18.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED6Ends"), {
	{ S1PEOPLE("ChessTerrace"), -18.4f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PlungeRim"), 18.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED6Plus"), {
	{ S1PEOPLE("Belvedere"), 1.0f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PlungeRim"), 10.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("ChessTerrace"), 19.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED6Minus"), {
	{ S1PEOPLE("Belvedere"), -1.0f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("PlungeRim"), -10.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("ChessTerrace"), -19.4f, 0.f, 0.f, 0.f, 1.f },
});
Set(TEXT("WldS1PEOPLED8Full"), {
	{ S1PEOPLE("ClubTerrace"), -18.4f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("ForgeStep"), -9.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("ChoirBastion"), 0.0f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("ClubTerrace"), 9.2f, 0.f, 0.f, 0.f, 1.f },
	{ S1PEOPLE("ForgeStep"), 18.4f, 0.f, 0.f, 0.f, 1.f },
});
Zone(TEXT("World s1people"), 0.f, 99.f);
// Factory Gate: a family on the doorstep, clocking on, the fire crew on the hose tower. 13 sites, 57 piece-places
	Site(TEXT("WldS1PEOPLED1Full"), -1, 43.3f, 1000, 999, 1.f);       // slot 1, 0.05 km, left; behind a tank row 34 m; may meet pump house 44 to 70 m
	Site(TEXT("WldS1PEOPLED1Full"), 1, 43.3f, 1000, 999, 1.f);        // slot 1, 0.05 km, right; may meet pump house 44 to 70 m
	Site(TEXT("WldS1PEOPLED1Full"), 1, 43.3f, 1000, 991, 1.f);        // slot 9, 0.41 km, right; behind a tank row 34 m
	Site(TEXT("WldS1PEOPLED1Full"), 1, 43.3f, 1000, 983, 1.f);        // slot 17, 0.77 km, right; may meet trailers 30 to 44 m
	Site(TEXT("WldS1PEOPLED1Full"), -1, 43.3f, 1000, 975, 1.f);       // slot 25, 1.13 km, left; behind a tank row 34 m; may meet pump house 44 to 70 m
	Site(TEXT("WldS1PEOPLED1Ends"), -1, 43.3f, 1000, 967, 1.f);       // slot 33, 1.49 km, left; blow stack 41 to 43 m; may meet tanks 40 to 75 m
	Site(TEXT("WldS1PEOPLED1Ends"), 1, 43.3f, 1000, 967, 1.f);        // slot 33, 1.49 km, right; blow stack 41 to 43 m; may meet tanks 40 to 75 m
	Site(TEXT("WldS1PEOPLED1Full"), -1, 43.3f, 1000, 959, 1.f);       // slot 41, 1.85 km, left; may meet tanks 40 to 75 m
	Site(TEXT("WldS1PEOPLED1Full"), 1, 43.3f, 1000, 959, 1.f);        // slot 41, 1.85 km, right; may meet tanks 40 to 75 m
	Site(TEXT("WldS1PEOPLED1Full"), -1, 43.3f, 1000, 951, 1.f);       // slot 49, 2.22 km, left; may meet tanks 40 to 75 m
	Site(TEXT("WldS1PEOPLED1Full"), 1, 43.3f, 1000, 951, 1.f);        // slot 49, 2.22 km, right; may meet tanks 40 to 75 m
	Site(TEXT("WldS1PEOPLED1Full"), -1, 43.3f, 1000, 943, 1.f);       // slot 57, 2.58 km, left; may meet tanks 40 to 75 m
	Site(TEXT("WldS1PEOPLED1Minus"), 1, 43.3f, 1000, 943, 1.f);       // slot 57, 2.58 km, right; kiln in the slot after; may meet tanks 40 to 75 m
// Reactor Block: white coats at a blackboard, a flask trolley, astronomers on the observatory gallery. 6 sites, 13 piece-places
	Site(TEXT("WldS1PEOPLED2Minus"), 1, 43.3f, 1000, 935, 1.f);       // slot 65, 2.94 km, right; kiln in the slot after
	Site(TEXT("WldS1PEOPLED2OnePlus"), -1, 43.3f, 1000, 927, 1.f);    // slot 73, 3.30 km, left; blow stack 41 to 43 m; kiln in the slot after
	Site(TEXT("WldS1PEOPLED2Ends"), 1, 43.3f, 1000, 927, 1.f);        // slot 73, 3.30 km, right; blow stack 41 to 43 m
	Site(TEXT("WldS1PEOPLED2Plus"), -1, 43.3f, 1000, 919, 1.f);       // slot 81, 3.66 km, left; kiln in the slot after
	Site(TEXT("WldS1PEOPLED2Ends"), -1, 43.3f, 1000, 887, 1.f);       // slot 113, 5.11 km, left; blow stack 41 to 43 m; the reactor core's side: no tower; may meet substations 40 to 60 m, platforms 44 to 70 m
	Site(TEXT("WldS1PEOPLED2Ends"), 1, 43.3f, 1000, 887, 1.f);        // slot 113, 5.11 km, right; blow stack 41 to 43 m; may meet substations 40 to 60 m, platforms 44 to 70 m
// Loading Yards: the drivers' cafe, checkers on a dock, the signal cabin. 8 sites, 32 piece-places
	Site(TEXT("WldS1PEOPLED3Full"), -1, 43.3f, 1000, 879, 1.f);       // slot 121, 5.47 km, left
	Site(TEXT("WldS1PEOPLED3Full"), 1, 43.3f, 1000, 879, 1.f);        // slot 121, 5.47 km, right
	Site(TEXT("WldS1PEOPLED3Full"), 1, 43.3f, 1000, 863, 1.f);        // slot 137, 6.20 km, right
	Site(TEXT("WldS1PEOPLED3Full"), 1, 43.3f, 1000, 855, 1.f);        // slot 145, 6.56 km, right
	Site(TEXT("WldS1PEOPLED3OneMinus"), -1, 43.3f, 1000, 847, 1.f);   // slot 153, 6.92 km, left; blow stack 41 to 43 m; kiln in the slot before; behind a loading bay 36.5 m; may meet trailers 28 to 42 m and 44 to 70 m
	Site(TEXT("WldS1PEOPLED3OnePlus"), 1, 43.3f, 1000, 847, 1.f);     // slot 153, 6.92 km, right; blow stack 41 to 43 m; kiln in the slot before; may meet trailers 28 to 42 m and 44 to 70 m
	Site(TEXT("WldS1PEOPLED3Full"), -1, 43.3f, 1000, 839, 1.f);       // slot 161, 7.28 km, left; may meet trailers 28 to 42 m and 44 to 70 m
	Site(TEXT("WldS1PEOPLED3Full"), 1, 43.3f, 1000, 839, 1.f);        // slot 161, 7.28 km, right; behind a loading bay 36.5 m; may meet trailers 28 to 42 m and 44 to 70 m
// Waste Run: skimmers on a duckboard jetty, the drum wash, the assayer on the filter house. 5 sites, 19 piece-places
	Site(TEXT("WldS1PEOPLED4Ends"), -1, 43.3f, 1000, 807, 1.f);       // slot 193, 8.73 km, left; blow stack 41 to 43 m; may meet drum piles 29.5 to 46 m
	Site(TEXT("WldS1PEOPLED4Ends"), 1, 43.3f, 1000, 807, 1.f);        // slot 193, 8.73 km, right; blow stack 41 to 43 m; behind a tank row 34 m; may meet drum piles 29.5 to 46 m
	Site(TEXT("WldS1PEOPLED4Full"), -1, 43.3f, 1000, 799, 1.f);       // slot 201, 9.09 km, left; may meet drum piles 29.5 to 46 m
	Site(TEXT("WldS1PEOPLED4Full"), 1, 43.3f, 1000, 799, 1.f);        // slot 201, 9.09 km, right; behind a tank row 34 m; may meet drum piles 29.5 to 46 m
	Site(TEXT("WldS1PEOPLED4Full"), 1, 43.3f, 1000, 791, 1.f);        // slot 209, 9.46 km, right; behind a tank row 34 m; may meet drum piles 29.5 to 46 m
// Export Terminal: sailors on the mission's steps, the customs desk, dockers at the hoist. 14 sites, 58 piece-places
	Site(TEXT("WldS1PEOPLED5Full"), -1, 43.3f, 1000, 783, 1.f);       // slot 217, 9.82 km, left; behind a loading bay 36.5 m; may meet trailers 28 to 44 m
	Site(TEXT("WldS1PEOPLED5Full"), -1, 43.3f, 1000, 775, 1.f);       // slot 225, 10.18 km, left; behind a loading bay 36.5 m; may meet trailers 28 to 44 m
	Site(TEXT("WldS1PEOPLED5Ends"), -1, 43.3f, 1000, 767, 1.f);       // slot 233, 10.54 km, left; blow stack 41 to 43 m; behind a loading bay 36.5 m; may meet trailers 28 to 44 m
	Site(TEXT("WldS1PEOPLED5Ends"), 1, 43.3f, 1000, 767, 1.f);        // slot 233, 10.54 km, right; blow stack 41 to 43 m; may meet trailers 28 to 44 m
	Site(TEXT("WldS1PEOPLED5Full"), -1, 43.3f, 1000, 759, 1.f);       // slot 241, 10.90 km, left; behind a loading bay 36.5 m; may meet trailers 28 to 44 m
	Site(TEXT("WldS1PEOPLED5Full"), 1, 43.3f, 1000, 759, 1.f);        // slot 241, 10.90 km, right; may meet trailers 28 to 44 m
	Site(TEXT("WldS1PEOPLED5Full"), -1, 43.3f, 1000, 751, 1.f);       // slot 249, 11.27 km, left; behind a loading bay 36.5 m; may meet trailers 28 to 44 m
	Site(TEXT("WldS1PEOPLED5Full"), 1, 43.3f, 1000, 751, 1.f);        // slot 249, 11.27 km, right; may meet trailers 28 to 44 m
	Site(TEXT("WldS1PEOPLED5Full"), 1, 43.3f, 1000, 743, 1.f);        // slot 257, 11.63 km, right; may meet trailers 28 to 44 m
	Site(TEXT("WldS1PEOPLED5Full"), -1, 43.3f, 1000, 735, 1.f);       // slot 265, 11.99 km, left; behind a loading bay 36.5 m; may meet trailers 28 to 44 m
	Site(TEXT("WldS1PEOPLED5Ends"), -1, 43.3f, 1000, 727, 1.f);       // slot 273, 12.35 km, left; blow stack 41 to 43 m; behind a loading bay 36.5 m; may meet trailers 28 to 44 m
	Site(TEXT("WldS1PEOPLED5Ends"), 1, 43.3f, 1000, 727, 1.f);        // slot 273, 12.35 km, right; blow stack 41 to 43 m; may meet trailers 28 to 44 m
	Site(TEXT("WldS1PEOPLED5Full"), -1, 43.3f, 1000, 719, 1.f);       // slot 281, 12.71 km, left; behind a loading bay 36.5 m; may meet trailers 28 to 44 m
	Site(TEXT("WldS1PEOPLED5Full"), 1, 43.3f, 1000, 719, 1.f);        // slot 281, 12.71 km, right; may meet trailers 28 to 44 m
// Cooling Field: bathers in robes: chess and a wheeled chair, the plunge pool, tea on the belvedere. 10 sites, 29 piece-places
	Site(TEXT("WldS1PEOPLED6Ends"), 1, 43.3f, 1000, 703, 1.f);        // slot 297, 13.44 km, right; flare avenue: no tower
	Site(TEXT("WldS1PEOPLED6Ends"), -1, 43.3f, 1000, 695, 1.f);       // slot 305, 13.80 km, left; flare avenue: no tower
	Site(TEXT("WldS1PEOPLED6Ends"), 1, 43.3f, 1000, 695, 1.f);        // slot 305, 13.80 km, right; flare avenue: no tower
	Site(TEXT("WldS1PEOPLED6Minus"), -1, 43.3f, 1000, 671, 1.f);      // slot 329, 14.88 km, left; kiln in the slot before
	Site(TEXT("WldS1PEOPLED6Plus"), 1, 43.3f, 1000, 671, 1.f);        // slot 329, 14.88 km, right; kiln in the slot before
	Site(TEXT("WldS1PEOPLED6Full"), 1, 43.3f, 1000, 663, 1.f);        // slot 337, 15.25 km, right
	Site(TEXT("WldS1PEOPLED6Minus"), -1, 43.3f, 1000, 647, 1.f);      // slot 353, 15.97 km, left; kiln in the slot before
	Site(TEXT("WldS1PEOPLED6Plus"), 1, 43.3f, 1000, 647, 1.f);        // slot 353, 15.97 km, right; kiln in the slot before
	Site(TEXT("WldS1PEOPLED6Minus"), -1, 43.3f, 1000, 623, 1.f);      // slot 377, 17.06 km, left; kiln in the slot before
	Site(TEXT("WldS1PEOPLED6Plus"), 1, 43.3f, 1000, 623, 1.f);        // slot 377, 17.06 km, right; kiln in the slot before
// Reactor Two: the engineers' club, smiths at the forge, the works choir on the bastion. 12 sites, 60 piece-places
	Site(TEXT("WldS1PEOPLED8Full"), -1, 43.3f, 1000, 503, 1.f);       // slot 497, 22.49 km, left; may meet platforms 40 to 55 m, substations 44 to 62 m
	Site(TEXT("WldS1PEOPLED8Full"), 1, 43.3f, 1000, 503, 1.f);        // slot 497, 22.49 km, right; may meet platforms 40 to 55 m, substations 44 to 62 m
	Site(TEXT("WldS1PEOPLED8Full"), -1, 43.3f, 1000, 487, 1.f);       // slot 513, 23.21 km, left; may meet platforms 40 to 55 m, substations 44 to 62 m
	Site(TEXT("WldS1PEOPLED8Full"), 1, 43.3f, 1000, 487, 1.f);        // slot 513, 23.21 km, right; may meet platforms 40 to 55 m, substations 44 to 62 m
	Site(TEXT("WldS1PEOPLED8Full"), -1, 43.3f, 1000, 479, 1.f);       // slot 521, 23.57 km, left
	Site(TEXT("WldS1PEOPLED8Full"), 1, 43.3f, 1000, 479, 1.f);        // slot 521, 23.57 km, right
	Site(TEXT("WldS1PEOPLED8Full"), -1, 43.3f, 1000, 463, 1.f);       // slot 537, 24.30 km, left; may meet platforms 40 to 55 m, substations 44 to 62 m
	Site(TEXT("WldS1PEOPLED8Full"), 1, 43.3f, 1000, 463, 1.f);        // slot 537, 24.30 km, right; may meet platforms 40 to 55 m, substations 44 to 62 m
	Site(TEXT("WldS1PEOPLED8Full"), -1, 43.3f, 1000, 455, 1.f);       // slot 545, 24.66 km, left
	Site(TEXT("WldS1PEOPLED8Full"), 1, 43.3f, 1000, 455, 1.f);        // slot 545, 24.66 km, right
	Site(TEXT("WldS1PEOPLED8Full"), -1, 43.3f, 1000, 447, 1.f);       // slot 553, 25.02 km, left; may meet platforms 40 to 55 m, substations 44 to 62 m
	Site(TEXT("WldS1PEOPLED8Full"), 1, 43.3f, 1000, 447, 1.f);        // slot 553, 25.02 km, right; may meet platforms 40 to 55 m, substations 44 to 62 m
#undef S1PEOPLE
// ---- END s1people ----
}
