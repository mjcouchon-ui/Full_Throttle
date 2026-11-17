// Exposure chart of Skibum. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// s2_ drawings: C:/ftwork/final/brawl/art/Skibum/frames (manifest.json beside it says which sheet each came from).

FTB_STATE("idle", Loop, 1256.6)
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 183.333, "s2_idle_01")
FTB_ROW("idle", 333.333, "s2_idle_02")
FTB_ROW("idle", 566.667, "s2_idle_03")
FTB_ROW("idle", 700, "s2_idle_04")
FTB_ROW("idle", 833.333, "s2_idle_05")
FTB_ROW("idle", 1066.667, "s2_idle_06")

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 28.409, "s2_walk_01")
FTB_ROW("walk", 56.818, "s2_walk_02")
FTB_ROW("walk", 85.227, "s2_walk_03")
FTB_ROW("walk", 113.636, "s2_walk_04")
FTB_ROW("walk", 142.045, "s2_walk_05")
FTB_ROW("walk", 170.454, "s2_walk_06")
FTB_ROW("walk", 198.863, "s2_walk_07")
FTB_ROW("walk", 227.273, "s2_walk_08")
FTB_ROW("walk", 255.682, "s2_walk_09")
FTB_ROW("walk", 284.091, "s2_walk_10")
FTB_ROW("walk", 312.5, "s2_walk_11")
FTB_ROW("walk", 340.909, "s2_walk_12")
FTB_ROW("walk", 369.318, "s2_walk_13")
FTB_ROW("walk", 397.727, "s2_walk_14")
FTB_ROW("walk", 426.136, "s2_walk_15")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 482.954, "s2_walk_01")
FTB_ROW("walk", 511.363, "s2_walk_02")
FTB_ROW("walk", 539.772, "s2_walk_03")
FTB_ROW("walk", 568.182, "s2_walk_04")
FTB_ROW("walk", 596.591, "s2_walk_05")
FTB_ROW("walk", 625, "s2_walk_06")
FTB_ROW("walk", 653.409, "s2_walk_07")
FTB_ROW("walk", 681.818, "s2_walk_08")
FTB_ROW("walk", 710.227, "s2_walk_09")
FTB_ROW("walk", 738.636, "s2_walk_10")
FTB_ROW("walk", 767.045, "s2_walk_11")
FTB_ROW("walk", 795.455, "s2_walk_12")
FTB_ROW("walk", 823.864, "s2_walk_13")
FTB_ROW("walk", 852.273, "s2_walk_14")
FTB_ROW("walk", 880.682, "s2_walk_15")

FTB_STATE("wind", Time, 280)
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 66.667, "s2_wind_02")
FTB_ROW("wind", 100, "s2_wind_03")
FTB_ROW("wind", 133.333, "s2_wind_04")
FTB_ROW("wind", 166.667, "s2_wind_05")
FTB_ROW("wind", 200, "s2_wind_06")
FTB_ROW("wind", 216.667, "s2_wind_07")
FTB_ROW("wind", 233.333, "s2_wind_08")
FTB_ROW("wind", 250, "s2_wind_09")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 66.667, "s2_swing_02")
FTB_ROW("swing", 100, "s2_swing_03")
FTB_ROW("swing", 133.333, "s2_swing_04")
FTB_ROW("swing", 166.667, "s2_swing_05")
FTB_ROW("swing", 200, "s2_swing_06")

FTB_STATE("dash", Time, 750)
FTB_ROW("dash", 0, "run")   // KEY
FTB_ROW("dash", 33.333, "s2_dash_01")
FTB_ROW("dash", 66.667, "s2_dash_02")
FTB_ROW("dash", 100, "s2_dash_01")
FTB_ROW("dash", 133.333, "s2_dash_02")
FTB_ROW("dash", 166.667, "s2_dash_03")
FTB_ROW("dash", 183.333, "s2_dash_09")
FTB_ROW("dash", 216.667, "s2_dash_04")
FTB_ROW("dash", 250, "s2_dash_05")
FTB_ROW("dash", 283.333, "s2_dash_06")
FTB_ROW("dash", 316.667, "s2_dash_08")
FTB_ROW("dash", 350, "run")
FTB_ROW("dash", 383.333, "s2_dash_09")
FTB_ROW("dash", 416.667, "s2_dash_04")
FTB_ROW("dash", 450, "s2_dash_05")
FTB_ROW("dash", 483.333, "s2_dash_06")
FTB_ROW("dash", 516.667, "s2_dash_07")
FTB_ROW("dash", 550, "run")
FTB_ROW("dash", 583.333, "s2_dash_09")
FTB_ROW("dash", 616.667, "s2_dash_04")
FTB_ROW("dash", 650, "s2_dash_05")
FTB_ROW("dash", 683.333, "s2_dash_06")
FTB_ROW("dash", 716.667, "s2_dash_08")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 66.667, "s2_hurt_02")
FTB_ROW("hurt", 100, "s2_hurt_03")
FTB_ROW("hurt", 133.333, "s2_hurt_04")
FTB_ROW("hurt", 166.667, "s2_hurt_05")
FTB_ROW("hurt", 200, "s2_hurt_06")
FTB_ROW("hurt", 233.333, "s2_hurt_07")

FTB_STATE("fly", Time, 453.3)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_fly_01")
FTB_ROW("fly", 66.667, "s2_fly_02")
FTB_ROW("fly", 100, "s2_fly_03")
FTB_ROW("fly", 133.333, "s2_fly_04")
FTB_ROW("fly", 166.667, "s2_fly_03")
FTB_ROW("fly", 200, "s2_fly_04")
FTB_ROW("fly", 233.333, "s2_fly_05")
FTB_ROW("fly", 266.667, "s2_fly_04")
FTB_ROW("fly", 300, "s2_fly_05")
FTB_ROW("fly", 333.333, "s2_fly_06")
FTB_ROW("fly", 366.667, "s2_fly_05")
FTB_ROW("fly", 400, "s2_fly_06")
FTB_ROW("fly", 433.333, "s2_fly_07")

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "down")   // KEY
FTB_ROW("getup", 33.333, "s2_getup_01")
FTB_ROW("getup", 66.667, "s2_getup_02")
FTB_ROW("getup", 100, "s2_getup_03")
FTB_ROW("getup", 133.333, "s2_getup_04")
FTB_ROW("getup", 166.667, "s2_getup_05")
FTB_ROW("getup", 200, "s2_getup_06")
FTB_ROW("getup", 233.333, "s2_getup_07")
FTB_ROW("getup", 266.667, "s2_getup_08")
FTB_ROW("getup", 300, "s2_getup_09")
FTB_ROW("getup", 383.333, "s2_getup_12")
FTB_ROW("getup", 416.667, "s2_getup_13")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
