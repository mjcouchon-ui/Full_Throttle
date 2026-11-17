// Exposure chart of Roller. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings (s2_*) go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// idle: the contract loops it every 1256.6 ms (table.json loop=true), so the clock is Loop with that period.

FTB_STATE("idle", Loop, 1256.64)   // one cycle
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 116.667, "s2_idle_01")
FTB_ROW("idle", 233.333, "s2_idle_02")
FTB_ROW("idle", 383.333, "s2_idle_03")
FTB_ROW("idle", 516.667, "s2_idle_04")
FTB_ROW("idle", 633.333, "s2_idle_05")
FTB_ROW("idle", 750, "s2_idle_06")
FTB_ROW("idle", 866.667, "s2_idle_07")
FTB_ROW("idle", 1016.667, "s2_idle_06")
FTB_ROW("idle", 1133.333, "s2_idle_08")

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 56.818, "s2_walk_01")
FTB_ROW("walk", 113.636, "s2_walk_02")
FTB_ROW("walk", 170.455, "s2_walk_03")
FTB_ROW("walk", 227.273, "s2_walk_04")
FTB_ROW("walk", 284.091, "s2_walk_05")
FTB_ROW("walk", 340.909, "s2_walk_06")
FTB_ROW("walk", 397.727, "s2_walk_07")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 511.363, "s2_walk_08")
FTB_ROW("walk", 568.181, "s2_walk_09")
FTB_ROW("walk", 625, "s2_walk_10")
FTB_ROW("walk", 681.818, "s2_walk_11")
FTB_ROW("walk", 738.636, "s2_walk_12")
FTB_ROW("walk", 795.454, "s2_walk_13")
FTB_ROW("walk", 852.272, "s2_walk_14")

FTB_STATE("wind", Time, 280)
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 66.667, "s2_wind_02")
FTB_ROW("wind", 100, "s2_wind_03")
FTB_ROW("wind", 133.333, "s2_wind_02")
FTB_ROW("wind", 166.667, "s2_wind_03")
FTB_ROW("wind", 200, "s2_wind_04")
FTB_ROW("wind", 233.333, "s2_wind_05")
FTB_ROW("wind", 250, "s2_wind_06")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 66.667, "s2_swing_02")
FTB_ROW("swing", 100, "s2_swing_03")
FTB_ROW("swing", 133.333, "s2_swing_04")
FTB_ROW("swing", 150, "s2_swing_05")
FTB_ROW("swing", 166.667, "s2_swing_06")
FTB_ROW("swing", 183.333, "s2_swing_07")
FTB_ROW("swing", 200, "s2_swing_08")

FTB_STATE("dash", Time, 750)
FTB_ROW("dash", 0, "run")   // KEY
FTB_ROW("dash", 33.333, "s2_dash_01")
FTB_ROW("dash", 66.667, "s2_dash_02")
FTB_ROW("dash", 100, "s2_dash_03")
FTB_ROW("dash", 200, "s2_dash_06")
FTB_ROW("dash", 233.333, "run")   // KEY
FTB_ROW("dash", 266.667, "s2_dash_01")
FTB_ROW("dash", 300, "s2_dash_02")
FTB_ROW("dash", 333.333, "s2_dash_03")
FTB_ROW("dash", 433.333, "s2_dash_06")
FTB_ROW("dash", 466.667, "run")   // KEY
FTB_ROW("dash", 500, "s2_dash_01")
FTB_ROW("dash", 533.333, "s2_dash_02")
FTB_ROW("dash", 566.667, "s2_dash_03")
FTB_ROW("dash", 666.667, "s2_dash_06")
FTB_ROW("dash", 733.333, "s2_dash_08")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 66.667, "s2_hurt_02")
FTB_ROW("hurt", 100, "s2_hurt_03")
FTB_ROW("hurt", 133.333, "s2_hurt_04")
FTB_ROW("hurt", 166.667, "s2_hurt_05")
FTB_ROW("hurt", 200, "s2_hurt_06")
FTB_ROW("hurt", 216.667, "s2_hurt_07")
FTB_ROW("hurt", 233.333, "s2_hurt_08")

FTB_STATE("fly", Time, 453.3)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_fly_01")
FTB_ROW("fly", 100, "s2_fly_02")
FTB_ROW("fly", 166.667, "s2_fly_03")
FTB_ROW("fly", 250, "s2_fly_04")
FTB_ROW("fly", 316.667, "s2_fly_05")
FTB_ROW("fly", 383.333, "s2_fly_06")

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "down")   // KEY
FTB_ROW("getup", 50, "s2_getup_01")
FTB_ROW("getup", 83.333, "s2_getup_02")
FTB_ROW("getup", 116.667, "s2_getup_03")
FTB_ROW("getup", 150, "s2_getup_04")
FTB_ROW("getup", 200, "s2_getup_05")
FTB_ROW("getup", 233.333, "s2_getup_06")
FTB_ROW("getup", 266.667, "s2_getup_07")
FTB_ROW("getup", 300, "s2_getup_08")
FTB_ROW("getup", 333.333, "s2_getup_09")
FTB_ROW("getup", 383.333, "s2_getup_10")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
