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
//   24.1 m          the LOW line, everywhere: the shift at its trestles (SM_KerbCrew), security on its round
//                   (SM_Patrol), the film crew applauding (24 m). No light. Seen from the low side of a banked bend
//   26 m            where a district has the fence, or a slot has neither rack nor tanks: scaffolds, loading docks,
//                   the gate guards, the waste crew, the laboratory staff, the crows on their rail, the crew
//   30.3 / 34.4 m   behind a rack or a tank row: the high scaffolds (decks at 7 m) and the watch towers (11.5 m)
//   120 m and out   the far shift on its footbridge and the crows (Far rules: they stay beside loops and pipes)
// Every and phase count 45.24 m slots from the start of the road (slot s is used when (s + phase) % every == 0), so
// the phases below are chosen against the dressing's own (a rack slot, a tank slot, a hazard sign's slot).
#define PPL(n) TEXT("/Game/People/ChemicalWorks/" n)
#define CREW(n) TEXT("/Game/People/Crew/SM_Crew" n "_Work")
const float K = 24.1f, N = 26.f, R = 30.3f, T = 34.4f;

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
	Far(FARP("Flock_B"), 90.f, 200.f, 70, 20, 1.f, 1.f, 0.f, -1, 0.f, 15.f);
	Far(FARP("Flock_B"), 90.f, 200.f, 70, 55, 1.f, 1.f, 0.f, 1, 0.f, 15.f);
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
