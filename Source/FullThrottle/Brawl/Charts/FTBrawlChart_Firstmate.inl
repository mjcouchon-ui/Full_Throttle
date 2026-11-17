// Exposure chart of Firstmate. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// s2_ drawings: C:/ftwork/final/brawl/art/Firstmate/frames (manifest.json beside it says which sheet each came from).
// walk: both contract keys are the same contact (the same leg in front), so a whole two-step cycle lies between them (brief, section 3).
// An alternative with ONE step between the keys is in alt/FTBrawlChart_Firstmate_onestep.inl (needs alt/frames too).

FTB_STATE("idle", Loop, 1256.6)
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 139.622, "s2_idle_11")
FTB_ROW("idle", 279.244, "s2_idle_12")
FTB_ROW("idle", 418.867, "s2_idle_13")
FTB_ROW("idle", 558.489, "s2_idle_14")
FTB_ROW("idle", 698.111, "s2_idle_15")
FTB_ROW("idle", 837.733, "s2_idle_16")
FTB_ROW("idle", 977.356, "s2_idle_17")
FTB_ROW("idle", 1116.978, "s2_idle_18")

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 28.409, "s2_walk_01")
FTB_ROW("walk", 56.818, "s2_walk_02")
FTB_ROW("walk", 85.227, "s2_walk_03")
FTB_ROW("walk", 113.636, "s2_walk_04")
FTB_ROW("walk", 142.045, "s2_walk_17")
FTB_ROW("walk", 170.454, "s2_walk_18")
FTB_ROW("walk", 198.863, "s2_walk_23")
FTB_ROW("walk", 227.273, "s2_walk_24")
FTB_ROW("walk", 255.682, "s2_walk_10")
FTB_ROW("walk", 284.091, "s2_walk_11")
FTB_ROW("walk", 312.5, "s2_walk_12")
FTB_ROW("walk", 340.909, "s2_walk_13")
FTB_ROW("walk", 369.318, "s2_walk_14")
FTB_ROW("walk", 397.727, "s2_walk_15")
FTB_ROW("walk", 426.136, "s2_walk_16")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 482.954, "s2_walk_01")
FTB_ROW("walk", 511.363, "s2_walk_02")
FTB_ROW("walk", 539.772, "s2_walk_03")
FTB_ROW("walk", 568.182, "s2_walk_04")
FTB_ROW("walk", 596.591, "s2_walk_17")
FTB_ROW("walk", 625, "s2_walk_18")
FTB_ROW("walk", 653.409, "s2_walk_23")
FTB_ROW("walk", 681.818, "s2_walk_24")
FTB_ROW("walk", 710.227, "s2_walk_10")
FTB_ROW("walk", 738.636, "s2_walk_11")
FTB_ROW("walk", 767.045, "s2_walk_12")
FTB_ROW("walk", 795.455, "s2_walk_13")
FTB_ROW("walk", 823.864, "s2_walk_14")
FTB_ROW("walk", 852.273, "s2_walk_15")
FTB_ROW("walk", 880.682, "s2_walk_16")

FTB_STATE("wind", Time, 420)
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 66.667, "s2_wind_02")
FTB_ROW("wind", 100, "s2_wind_03")
FTB_ROW("wind", 133.333, "s2_wind_04")
FTB_ROW("wind", 166.667, "s2_wind_03")
FTB_ROW("wind", 200, "s2_wind_04")
FTB_ROW("wind", 233.333, "s2_wind_05")
FTB_ROW("wind", 266.667, "s2_wind_06")
FTB_ROW("wind", 300, "s2_wind_07")
FTB_ROW("wind", 333.333, "s2_wind_06")
FTB_ROW("wind", 366.667, "s2_wind_07")
FTB_ROW("wind", 400, "s2_wind_08")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 50, "s2_swing_02")
FTB_ROW("swing", 83.333, "s2_swing_03")
FTB_ROW("swing", 100, "s2_swing_04")
FTB_ROW("swing", 116.667, "s2_swing_05")
FTB_ROW("swing", 150, "s2_swing_06")
FTB_ROW("swing", 183.333, "s2_swing_07")
FTB_ROW("swing", 200, "s2_swing_08")

FTB_STATE("swing:1", Time, 220)
FTB_ROW("swing:1", 0, "bash")   // KEY
FTB_ROW("swing:1", 33.333, "s2_bash_01")
FTB_ROW("swing:1", 66.667, "s2_bash_02")
FTB_ROW("swing:1", 100, "s2_bash_03")
FTB_ROW("swing:1", 133.333, "s2_bash_04")
FTB_ROW("swing:1", 166.667, "s2_bash_05")
FTB_ROW("swing:1", 183.333, "s2_bash_06")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 50, "s2_hurt_02")
FTB_ROW("hurt", 83.333, "s2_hurt_03")
FTB_ROW("hurt", 116.667, "s2_hurt_04")
FTB_ROW("hurt", 150, "s2_hurt_05")
FTB_ROW("hurt", 183.333, "s2_hurt_06")
FTB_ROW("hurt", 216.667, "s2_hurt_07")
FTB_ROW("hurt", 233.333, "s2_hurt_08")

FTB_STATE("fly", Time, 453.3)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_fly_05")
FTB_ROW("fly", 66.667, "s2_fly_06")
FTB_ROW("fly", 100, "s2_fly_07")
FTB_ROW("fly", 133.333, "s2_fly_04")
FTB_ROW("fly", 166.667, "s2_fly_05")
FTB_ROW("fly", 200, "s2_fly_06")
FTB_ROW("fly", 233.333, "s2_fly_07")
FTB_ROW("fly", 266.667, "s2_fly_04")
FTB_ROW("fly", 300, "s2_fly_05")
FTB_ROW("fly", 333.333, "s2_fly_06")
FTB_ROW("fly", 366.667, "s2_fly_07")
FTB_ROW("fly", 400, "s2_fly_08")

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "down")   // KEY
FTB_ROW("getup", 50, "s2_getup_01")
FTB_ROW("getup", 83.333, "s2_getup_02")
FTB_ROW("getup", 116.667, "s2_getup_03")
FTB_ROW("getup", 166.667, "s2_getup_04")
FTB_ROW("getup", 200, "s2_getup_05")
FTB_ROW("getup", 233.333, "s2_getup_06")
FTB_ROW("getup", 266.667, "s2_getup_07")
FTB_ROW("getup", 300, "s2_getup_08")
FTB_ROW("getup", 333.333, "s2_getup_09")
FTB_ROW("getup", 366.667, "s2_getup_10")
FTB_ROW("getup", 400, "s2_getup_12")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
