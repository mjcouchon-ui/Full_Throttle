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
