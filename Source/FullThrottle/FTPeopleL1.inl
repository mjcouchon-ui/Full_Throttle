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
	Scatter(PPL("SM_KerbCrew_A"), K, K, 6, 0, 0.9f, 1.f, 0.f, -1);
	Scatter(PPL("SM_KerbCrew_B"), K, K, 6, 3, 0.9f, 1.f, 0.f, -1);
	Scatter(PPL("SM_Patrol"), K, K, 6, 2, 0.8f, 1.f, 0.f, -1);
	Scatter(PPL("SM_KerbCrew_B"), K, K, 6, 1, 0.9f, 1.f, 0.f, 1);
	Scatter(PPL("SM_KerbCrew_A"), K, K, 6, 4, 0.9f, 1.f, 0.f, 1);
	Scatter(PPL("SM_Patrol"), K, K, 6, 5, 0.8f, 1.f, 0.f, 1);
	Scatter(CREW("ApplauseA"), 24.f, 24.f, 12, 11, 1.f, 1.f, 0.f, -1);
	Scatter(CREW("ApplauseB"), 24.f, 24.f, 12, 4, 1.f, 1.f, 0.f, 1);
// ---- the far shift and the crows, the whole road
Zone(TEXT("People: far"), 0.03f, 99.f);
	Far(PPL("SM_FarCatwalk"), 120.f, 200.f, 16, 3, 0.9f, 1.f, 0.f, -1, 1.5f, 10.f);
	Far(PPL("SM_FarCatwalk"), 120.f, 200.f, 16, 11, 0.9f, 1.f, 0.f, 1, 1.5f, 10.f);
	Far(PPL("SM_CrowFlock"), 260.f, 460.f, 12, 1, 0.9f, 1.f, 0.2f, -1, 0.f, 180.f);
	Far(PPL("SM_CrowFlock"), 260.f, 460.f, 12, 7, 0.9f, 1.f, 0.2f, 1, 0.f, 180.f);

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
// worker's and the film crew's. Every piece is TALL, lit by a steady light of its own (no blinking) and saved with the
// always-moving master.
//   44 to 62 m      SM_FarYardDogs: the strays on their container stack, the rats on the pipe behind (Scatter: on the land)
//   60 to 100 m     SM_FarCrows_A: the crows against their lit hoarding
//   100 to 160 m    SM_FarShift_A: the shift change on its footbridge
//   120 to 210 m    SM_FarTankCrew: the gang on the tank top
//   120 to 300 m    SM_FarFlock_A (gulls wheeling) and SM_FarFlock_B (the skein crossing): Far rules with no drop, so
//                   the sky's birds stay beside loops, pipes and jumps
// The standing pieces are Scatters: they stand on the land where it rises and falls (the plateau, the ramps).
// Each district has its own mix; left and right are separate rules with phases apart, so the two sides never mirror.
#define FARP(n) TEXT("/Game/People/ChemicalWorks/SM_Far" n)
Zone(TEXT("People far: the sky"), 0.03f, 99.f);
	Far(FARP("Flock_A"), 120.f, 300.f, 22, 3, 1.f, 1.f, 0.2f, -1, 0.f, 180.f);
	Far(FARP("Flock_A"), 120.f, 300.f, 22, 14, 1.f, 1.f, 0.2f, 1, 0.f, 180.f);
	Far(FARP("Flock_B"), 150.f, 280.f, 56, 20, 1.f, 1.f, 0.f, -1, 0.f, 15.f);
	Far(FARP("Flock_B"), 150.f, 280.f, 56, 48, 1.f, 1.f, 0.f, 1, 0.f, 15.f);
Zone(TEXT("People far: gate"), 0.10f, 1.23f);
	Scatter(FARP("YardDogs"), 46.f, 60.f, 6, 1, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("YardDogs"), 46.f, 60.f, 6, 4, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("Shift_A"), 105.f, 150.f, 12, 5, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Shift_A"), 105.f, 150.f, 12, 11, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: tank farm"), 1.23f, 2.6f);
	Scatter(FARP("TankCrew"), 120.f, 200.f, 7, 2, 1.f, 1.f, 0.f, -1, 20.f);
	Scatter(FARP("TankCrew"), 120.f, 200.f, 7, 5, 1.f, 1.f, 0.f, 1, 20.f);
	Scatter(FARP("YardDogs"), 46.f, 60.f, 12, 3, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("YardDogs"), 46.f, 60.f, 12, 9, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: process units"), 2.6f, 3.98f);
	Scatter(FARP("Shift_A"), 100.f, 150.f, 7, 1, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Shift_A"), 100.f, 150.f, 7, 4, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("Crows_A"), 60.f, 100.f, 12, 6, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Crows_A"), 60.f, 100.f, 12, 0, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: reactor block"), 3.98f, 5.35f);
	Scatter(FARP("Crows_A"), 60.f, 100.f, 7, 0, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Crows_A"), 60.f, 100.f, 7, 3, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("TankCrew"), 130.f, 210.f, 14, 5, 1.f, 1.f, 0.f, -1, 20.f);
	Scatter(FARP("TankCrew"), 130.f, 210.f, 14, 12, 1.f, 1.f, 0.f, 1, 20.f);
Zone(TEXT("People far: pipe alley"), 5.35f, 6.72f);
	Scatter(FARP("Shift_A"), 100.f, 150.f, 7, 2, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Shift_A"), 100.f, 150.f, 7, 5, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("YardDogs"), 46.f, 60.f, 12, 0, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("YardDogs"), 46.f, 60.f, 12, 6, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: loading yards"), 6.72f, 7.94f);
	Scatter(FARP("YardDogs"), 46.f, 60.f, 6, 2, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("YardDogs"), 46.f, 60.f, 6, 5, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("Crows_A"), 60.f, 100.f, 12, 4, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Crows_A"), 60.f, 100.f, 12, 10, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: waste run"), 8.09f, 9.46f);
	Scatter(FARP("Crows_A"), 60.f, 100.f, 6, 0, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Crows_A"), 60.f, 100.f, 6, 3, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("YardDogs"), 46.f, 60.f, 8, 2, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("YardDogs"), 46.f, 60.f, 8, 6, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: export terminal"), 9.46f, 12.72f);
	Scatter(FARP("Shift_A"), 100.f, 150.f, 18, 1, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Shift_A"), 100.f, 150.f, 18, 10, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("TankCrew"), 120.f, 200.f, 24, 6, 1.f, 1.f, 0.f, -1, 20.f);
	Scatter(FARP("TankCrew"), 120.f, 200.f, 24, 18, 1.f, 1.f, 0.f, 1, 20.f);
	Scatter(FARP("YardDogs"), 46.f, 60.f, 28, 3, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("YardDogs"), 46.f, 60.f, 28, 17, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: flare avenue"), 12.72f, 13.85f);
	Scatter(FARP("Crows_A"), 60.f, 100.f, 12, 2, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Crows_A"), 60.f, 100.f, 12, 8, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: cooling field"), 13.85f, 17.4f);
	Scatter(FARP("TankCrew"), 120.f, 200.f, 18, 4, 1.f, 1.f, 0.f, -1, 20.f);
	Scatter(FARP("TankCrew"), 120.f, 200.f, 18, 13, 1.f, 1.f, 0.f, 1, 20.f);
	Scatter(FARP("Crows_A"), 60.f, 100.f, 28, 9, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Crows_A"), 60.f, 100.f, 28, 23, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("Shift_A"), 100.f, 150.f, 40, 15, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Shift_A"), 100.f, 150.f, 40, 35, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: tank farm B"), 17.4f, 21.4f);
	Scatter(FARP("TankCrew"), 120.f, 200.f, 22, 2, 1.f, 1.f, 0.f, -1, 20.f);
	Scatter(FARP("TankCrew"), 120.f, 200.f, 22, 13, 1.f, 1.f, 0.f, 1, 20.f);
	Scatter(FARP("YardDogs"), 46.f, 60.f, 24, 7, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("YardDogs"), 46.f, 60.f, 24, 19, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("Crows_A"), 60.f, 100.f, 44, 10, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Crows_A"), 60.f, 100.f, 44, 32, 1.f, 1.f, 0.f, 1, 8.f);
Zone(TEXT("People far: reactor two"), 21.4f, 25.6f);
	Scatter(FARP("Crows_A"), 60.f, 100.f, 22, 1, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Crows_A"), 60.f, 100.f, 22, 12, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("Shift_A"), 100.f, 150.f, 28, 6, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("Shift_A"), 100.f, 150.f, 28, 20, 1.f, 1.f, 0.f, 1, 8.f);
	Scatter(FARP("YardDogs"), 46.f, 60.f, 36, 9, 1.f, 1.f, 0.f, -1, 8.f);
	Scatter(FARP("YardDogs"), 46.f, 60.f, 36, 27, 1.f, 1.f, 0.f, 1, 8.f);
#undef FARP
// ---- END far ----
}

{
// ---- BEGIN watch (ft_people_chem_watch.py) ----
// THE WATCHERS: the night shift, downed tools, massed on raised steel 40 m from the centre line (all TALL pieces,
// steady light only), and on the roofs of far tanks. Every/phase count 45.24 m slots. The dressing's blow-down stacks
// (41 to 43 m) and light masts (37 to 40 m) take phases 2 and 4 of every 5 slots as far as 12.72 km, and phases 4
// and 1 of every 6 from 13.85 km: the watchers take the other phases of the same periods (0, 1, 3 of 5, then 0, 2, 3, 5
// of 6), so they never stand in one. The flare avenue (12.72 to 13.85 km) is left to its flames.
#define WATCH(n) TEXT("/Game/People/ChemicalWorks/SM_Watch" n)
Zone(TEXT("People watch: the works"), 0.03f, 12.72f);
	Scatter(WATCH("Stand_A"), 40.5f, 40.5f, 10, 0, 0.8f, 1.f, 0.f, -1);
	Scatter(WATCH("Stand_B"), 40.5f, 40.5f, 10, 5, 0.8f, 1.f, 0.f, -1);
	Scatter(WATCH("Lift_A"), 40.f, 40.f, 10, 3, 0.8f, 1.f, 0.f, -1);
	Scatter(WATCH("Lift_B"), 40.f, 40.f, 10, 8, 0.8f, 1.f, 0.f, -1);
	Scatter(WATCH("Roof"), 40.5f, 40.5f, 10, 1, 1.f, 1.f, 0.f, -1);
	Scatter(WATCH("Stand_B"), 40.5f, 40.5f, 10, 3, 0.8f, 1.f, 0.f, 1);
	Scatter(WATCH("Stand_A"), 40.5f, 40.5f, 10, 8, 0.8f, 1.f, 0.f, 1);
	Scatter(WATCH("Lift_B"), 40.f, 40.f, 10, 1, 0.8f, 1.f, 0.f, 1);
	Scatter(WATCH("Lift_A"), 40.f, 40.f, 10, 6, 0.8f, 1.f, 0.f, 1);
	Scatter(WATCH("Roof"), 40.5f, 40.5f, 10, 5, 1.f, 1.f, 0.f, 1);
Zone(TEXT("People watch: the finale"), 13.85f, 99.f);
	Scatter(WATCH("Stand_A"), 40.5f, 40.5f, 12, 0, 0.8f, 1.f, 0.f, -1);
	Scatter(WATCH("Stand_B"), 40.5f, 40.5f, 12, 6, 0.8f, 1.f, 0.f, -1);
	Scatter(WATCH("Lift_A"), 40.f, 40.f, 12, 3, 0.8f, 1.f, 0.f, -1);
	Scatter(WATCH("Lift_B"), 40.f, 40.f, 12, 9, 0.8f, 1.f, 0.f, -1);
	Scatter(WATCH("Roof"), 40.5f, 40.5f, 12, 2, 1.f, 1.f, 0.f, -1);
	Scatter(WATCH("Stand_B"), 40.5f, 40.5f, 12, 3, 0.8f, 1.f, 0.f, 1);
	Scatter(WATCH("Stand_A"), 40.5f, 40.5f, 12, 9, 0.8f, 1.f, 0.f, 1);
	Scatter(WATCH("Lift_B"), 40.f, 40.f, 12, 0, 0.8f, 1.f, 0.f, 1);
	Scatter(WATCH("Lift_A"), 40.f, 40.f, 12, 6, 0.8f, 1.f, 0.f, 1);
	Scatter(WATCH("Roof"), 40.5f, 40.5f, 12, 5, 1.f, 1.f, 0.f, 1);
// the far tanks: Far rules (they stay beside loops and pipes), standing 1.2 m under the road's level like the land
Zone(TEXT("People watch: far tanks"), 0.3f, 99.f);
	Far(WATCH("TankFar"), 70.f, 110.f, 12, 2, 1.f, 1.f, 0.f, -1, 1.2f, 8.f);
	Far(WATCH("TankFar"), 70.f, 110.f, 12, 8, 1.f, 1.f, 0.f, 1, 1.2f, 8.f);
#undef WATCH
// ---- END watch ----
}
