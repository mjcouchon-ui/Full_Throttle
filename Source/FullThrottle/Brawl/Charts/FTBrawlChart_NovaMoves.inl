// Exposure chart of Nova's wider move set (FTBrawlMoves.cpp). Format and rules: FTBrawlCharts.inl.
// A second file for the same character, kept apart from FTBrawlChart_Nova.inl. It adds states only; it names no
// state of TIMING_BEFORE.md.
//
// The drawings (T_nova_s2_<move>_nn) were generated from the existing Nova drawings with the tool those were made
// with, one sheet per move (C:\ftwork\final\brawl\art\NovaMoves\manifest.json). A row marked OLD still shows an
// existing drawing. A row marked CONTACT starts when the move's active window does and must stay at that time.
// Row times are multiples of one fight step (16.667 ms).

// sidestep: a quick step into or out of the room; nothing hits him for the first 200 ms
FTB_STATE("sidestep", Time, 300)
FTB_ROW("sidestep", 0, "s2_sidestep_01")
FTB_ROW("sidestep", 33.333, "s2_sidestep_02")
FTB_ROW("sidestep", 66.667, "s2_sidestep_03")
FTB_ROW("sidestep", 100, "s2_sidestep_04")
FTB_ROW("sidestep", 116.667, "s2_sidestep_05")
FTB_ROW("sidestep", 150, "s2_sidestep_06")
FTB_ROW("sidestep", 200, "s2_sidestep_07")   // recovery starts: he can be hit again
FTB_ROW("sidestep", 250, "nidle01")   // OLD

// dashfwd: a burst along the room (double tap)
FTB_STATE("dashfwd", Time, 260)
FTB_ROW("dashfwd", 0, "s2_dashfwd_01")
FTB_ROW("dashfwd", 33.333, "s2_dashfwd_02")
FTB_ROW("dashfwd", 50, "s2_dashfwd_03")
FTB_ROW("dashfwd", 83.333, "s2_dashfwd_04")
FTB_ROW("dashfwd", 100, "s2_dashfwd_05")
FTB_ROW("dashfwd", 133.333, "s2_dashfwd_06")
FTB_ROW("dashfwd", 150, "s2_dashfwd_07")
FTB_ROW("dashfwd", 183.333, "s2_dashfwd_08")
FTB_ROW("dashfwd", 200, "s2_dashfwd_09")

// dashatk: the attack out of a dash (a driving shoulder), active 70 to 190 ms
FTB_STATE("dashatk", Time, 360)
FTB_ROW("dashatk", 0, "s2_dashatk_01")
FTB_ROW("dashatk", 33.333, "s2_dashatk_02")
FTB_ROW("dashatk", 66.667, "s2_dashatk_03")   // CONTACT
FTB_ROW("dashatk", 100, "s2_dashatk_04")
FTB_ROW("dashatk", 133.333, "s2_dashatk_05")
FTB_ROW("dashatk", 166.667, "s2_dashatk_06")
FTB_ROW("dashatk", 183.333, "s2_dashatk_07")
FTB_ROW("dashatk", 250, "ncross04")   // OLD
FTB_ROW("dashatk", 316.667, "nidle01")   // OLD

// runatk: the running attack (a sliding kick), active 80 to 300 ms
FTB_STATE("runatk", Time, 460)
FTB_ROW("runatk", 0, "nrun08")   // OLD
FTB_ROW("runatk", 33.333, "s2_runatk_01")
FTB_ROW("runatk", 83.333, "s2_runatk_02")   // CONTACT
FTB_ROW("runatk", 116.667, "s2_runatk_03")
FTB_ROW("runatk", 150, "s2_runatk_04")
FTB_ROW("runatk", 183.333, "s2_runatk_05")
FTB_ROW("runatk", 216.667, "s2_runatk_06")
FTB_ROW("runatk", 283.333, "s2_runatk_08")
FTB_ROW("runatk", 300, "s2_runatk_09")
FTB_ROW("runatk", 366.667, "s2_runatk_10")
FTB_ROW("runatk", 416.667, "nidle01")   // OLD

// airjab: the rising air attack (punch in a jump), active 40 to 120 ms, then the jump carries on
FTB_STATE("airjab", Time, 200)
FTB_ROW("airjab", 0, "s2_airjab_01")
FTB_ROW("airjab", 33.333, "s2_airjab_02")   // CONTACT
FTB_ROW("airjab", 66.667, "s2_airjab_03")
FTB_ROW("airjab", 100, "s2_airjab_04")
FTB_ROW("airjab", 133.333, "s2_airjab_05")
FTB_ROW("airjab", 166.667, "s2_airjab_06")

// dive: the diving kick (down + kick in a jump), active from 50 ms until he lands
FTB_STATE("dive", Time, 200)
FTB_ROW("dive", 0, "s2_dive_01")
FTB_ROW("dive", 50, "s2_dive_02")   // CONTACT
FTB_ROW("dive", 83.333, "s2_dive_03")

// dive>idle: landing after a dive
FTB_STATE("dive>idle", Age, 100)
FTB_ROW("dive>idle", 0, "s2_dive_04")
FTB_ROW("dive>idle", 50, "s2_dive_05")

FTB_STATE("dive>walk", Age, 100)
FTB_ROW("dive>walk", 0, "s2_dive_04")
FTB_ROW("dive>walk", 50, "s2_dive_05")

// launch: the launcher (kick out of the second punch: a rising knee), active 100 to 200 ms
FTB_STATE("launch", Time, 440)
FTB_ROW("launch", 0, "s2_launch_01")
FTB_ROW("launch", 50, "s2_launch_02")
FTB_ROW("launch", 100, "s2_launch_03")   // CONTACT
FTB_ROW("launch", 133.333, "s2_launch_04")
FTB_ROW("launch", 166.667, "s2_launch_03")
FTB_ROW("launch", 200, "s2_launch_05")
FTB_ROW("launch", 233.333, "s2_launch_06")
FTB_ROW("launch", 266.667, "s2_launch_07")
FTB_ROW("launch", 300, "s2_launch_08")
FTB_ROW("launch", 333.333, "s2_launch_09")
FTB_ROW("launch", 383.333, "s2_launch_10")

// charging: the punch button held (a loop until it is let go; full at 800 ms): a moving hold
FTB_STATE("charging", Loop, 400)
FTB_ROW("charging", 0, "s2_charging_01")
FTB_ROW("charging", 66.667, "s2_charging_02")
FTB_ROW("charging", 133.333, "s2_charging_03")
FTB_ROW("charging", 266.667, "s2_charging_05")
FTB_ROW("charging", 333.333, "s2_charging_06")

// heavy: the charged punch let go, active 120 to 220 ms
FTB_STATE("heavy", Time, 520)
FTB_ROW("heavy", 0, "s2_heavy_01")
FTB_ROW("heavy", 50, "s2_heavy_02")
FTB_ROW("heavy", 83.333, "s2_heavy_03")
FTB_ROW("heavy", 116.667, "s2_heavy_04")   // CONTACT
FTB_ROW("heavy", 150, "s2_heavy_05")
FTB_ROW("heavy", 183.333, "s2_heavy_06")
FTB_ROW("heavy", 216.667, "s2_heavy_08")
FTB_ROW("heavy", 316.667, "s2_heavy_10")
FTB_ROW("heavy", 366.667, "s2_heavy_09")
FTB_ROW("heavy", 433.333, "nidle01")   // OLD

// special: the crowd clearer (punch + kick together; costs health), active 100 to 380 ms on both sides
FTB_STATE("special", Time, 620)
FTB_ROW("special", 0, "s2_special_01")
FTB_ROW("special", 50, "s2_special_02")
FTB_ROW("special", 100, "s2_special_03")   // CONTACT (front)
FTB_ROW("special", 133.333, "s2_special_04")
FTB_ROW("special", 166.667, "s2_special_05")
FTB_ROW("special", 200, "s2_special_06")
FTB_ROW("special", 266.667, "s2_special_09")   // CONTACT (back sweep)
FTB_ROW("special", 300, "s2_special_10")
FTB_ROW("special", 333.333, "s2_special_11")
FTB_ROW("special", 383.333, "s2_special_12")
FTB_ROW("special", 433.333, "s2_special_13")
FTB_ROW("special", 500, "s2_heavy_08")
FTB_ROW("special", 550, "nidle01")   // OLD

// backatk: the back attack (an elbow at a foe behind, without turning), active 60 to 140 ms
FTB_STATE("backatk", Time, 300)
FTB_ROW("backatk", 0, "s2_backatk_02")
FTB_ROW("backatk", 50, "s2_backatk_03")   // CONTACT
FTB_ROW("backatk", 83.333, "s2_backatk_04")
FTB_ROW("backatk", 116.667, "s2_backatk_05")
FTB_ROW("backatk", 150, "s2_backatk_06")
FTB_ROW("backatk", 183.333, "s2_backatk_07")
FTB_ROW("backatk", 216.667, "nidle01")   // OLD

// stomp: the ground finisher on a downed foe, lands at 160 ms
FTB_STATE("stomp", Time, 460)
FTB_ROW("stomp", 0, "s2_stomp_01")
FTB_ROW("stomp", 83.333, "s2_stomp_02")
FTB_ROW("stomp", 150, "s2_stomp_03")   // CONTACT
FTB_ROW("stomp", 183.333, "s2_stomp_04")
FTB_ROW("stomp", 216.667, "s2_stomp_05")
FTB_ROW("stomp", 250, "s2_stomp_06")
FTB_ROW("stomp", 283.333, "s2_stomp_07")
FTB_ROW("stomp", 316.667, "s2_stomp_08")
FTB_ROW("stomp", 366.667, "nidle01")   // OLD

// pummel: a knee into the foe he holds, lands at 120 ms; then back to the carry
FTB_STATE("pummel", Time, 300)
FTB_ROW("pummel", 0, "s2_pummel_01")
FTB_ROW("pummel", 50, "s2_pummel_02")
FTB_ROW("pummel", 100, "s2_pummel_04")   // CONTACT
FTB_ROW("pummel", 133.333, "s2_pummel_05")
FTB_ROW("pummel", 166.667, "s2_pummel_06")
FTB_ROW("pummel", 200, "s2_pummel_07")
FTB_ROW("pummel", 250, "s2_pummel_08")

// throwback: the held foe goes over his head and behind him, let go at 140 ms
FTB_STATE("throwback", Time, 400)
FTB_ROW("throwback", 0, "s2_throwback_01")
FTB_ROW("throwback", 66.667, "s2_throwback_02")
FTB_ROW("throwback", 133.333, "s2_throwback_03")   // CONTACT (release)
FTB_ROW("throwback", 166.667, "s2_throwback_04")
FTB_ROW("throwback", 233.333, "s2_throwback_06")
FTB_ROW("throwback", 283.333, "s2_throwback_07")
FTB_ROW("throwback", 316.667, "nidle01")   // OLD

// slam: the held foe is driven into the floor in front, impact at 200 ms
FTB_STATE("slam", Time, 500)
FTB_ROW("slam", 0, "s2_slam_07")
FTB_ROW("slam", 33.333, "s2_slam_01")
FTB_ROW("slam", 100, "s2_slam_02")
FTB_ROW("slam", 150, "s2_slam_03")
FTB_ROW("slam", 183.333, "s2_slam_04")   // CONTACT (hands at the floor)
FTB_ROW("slam", 233.333, "s2_slam_05")
FTB_ROW("slam", 283.333, "s2_special_01")
FTB_ROW("slam", 333.333, "s2_slam_06")
FTB_ROW("slam", 383.333, "s2_heavy_08")
FTB_ROW("slam", 416.667, "nidle01")   // OLD

// wpnair: the weapon brought down in a jump, active 80 to 220 ms, then the jump carries on
FTB_STATE("wpnair", Time, 300)
FTB_ROW("wpnair", 0, "s2_wpnair_01")
FTB_ROW("wpnair", 50, "s2_wpnair_02")
FTB_ROW("wpnair", 83.333, "s2_wpnair_03")   // CONTACT
FTB_ROW("wpnair", 116.667, "s2_wpnair_04")
FTB_ROW("wpnair", 150, "s2_wpnair_05")
FTB_ROW("wpnair", 183.333, "s2_wpnair_06")
FTB_ROW("wpnair", 216.667, "s2_wpnair_07")
FTB_ROW("wpnair", 266.667, "s2_wpnair_08")

// wpndash: the weapon thrust out of a run or dash, active 100 to 220 ms
FTB_STATE("wpndash", Time, 400)
FTB_ROW("wpndash", 0, "s2_wpndash_01")
FTB_ROW("wpndash", 50, "s2_wpndash_02")
FTB_ROW("wpndash", 100, "s2_wpndash_03")   // CONTACT
FTB_ROW("wpndash", 133.333, "s2_wpndash_04")
FTB_ROW("wpndash", 200, "s2_wpndash_06")
FTB_ROW("wpndash", 216.667, "s2_wpndash_07")
FTB_ROW("wpndash", 266.667, "s2_dashatk_06")
FTB_ROW("wpndash", 300, "s2_wpndash_08")
FTB_ROW("wpndash", 366.667, "nidle01")   // OLD

// airtoss: the weapon thrown in a jump, leaves the hand at 80 ms
FTB_STATE("airtoss", Time, 240)
FTB_ROW("airtoss", 0, "s2_airtoss_01")
FTB_ROW("airtoss", 50, "s2_airtoss_02")
FTB_ROW("airtoss", 83.333, "s2_airtoss_03")   // CONTACT (release)
FTB_ROW("airtoss", 116.667, "s2_airtoss_04")
FTB_ROW("airtoss", 166.667, "s2_wpnair_08")

// wpnbreak: looks only, the 240 ms after a weapon breaks in his hand (he is free to move and act)
FTB_STATE("wpnbreak", Time, 240)
FTB_ROW("wpnbreak", 0, "s2_wpnbreak_01")
FTB_ROW("wpnbreak", 83.333, "s2_wpnbreak_02")   // looks at the empty hand
FTB_ROW("wpnbreak", 166.667, "s2_wpnbreak_03")
