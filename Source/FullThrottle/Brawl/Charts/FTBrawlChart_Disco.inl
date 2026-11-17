// Exposure chart of Disco. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// s2_ drawings: C:/ftwork/final/brawl/art/Disco/frames (manifest.json beside it says which sheet each came from).

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
FTB_ROW("walk", 34.965, "s2_walk_01")
FTB_ROW("walk", 69.93, "s2_walk_02")
FTB_ROW("walk", 104.895, "s2_walk_17")
FTB_ROW("walk", 139.86, "s2_walk_03")
FTB_ROW("walk", 174.825, "s2_walk_18")
FTB_ROW("walk", 209.79, "s2_walk_04")
FTB_ROW("walk", 244.755, "s2_walk_19")
FTB_ROW("walk", 279.72, "s2_walk_05")
FTB_ROW("walk", 314.685, "s2_walk_20")
FTB_ROW("walk", 349.65, "s2_walk_06")
FTB_ROW("walk", 384.615, "s2_walk_07")
FTB_ROW("walk", 419.58, "s2_walk_08")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 489.51, "s2_walk_09")
FTB_ROW("walk", 524.475, "s2_walk_10")
FTB_ROW("walk", 559.44, "s2_walk_21")
FTB_ROW("walk", 594.405, "s2_walk_11")
FTB_ROW("walk", 629.37, "s2_walk_22")
FTB_ROW("walk", 664.335, "s2_walk_12")
FTB_ROW("walk", 699.301, "s2_walk_23")
FTB_ROW("walk", 734.266, "s2_walk_13")
FTB_ROW("walk", 769.231, "s2_walk_24")
FTB_ROW("walk", 804.196, "s2_walk_14")
FTB_ROW("walk", 839.161, "s2_walk_15")
FTB_ROW("walk", 874.126, "s2_walk_16")

FTB_STATE("wind", Time, 280)
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 66.667, "s2_wind_02")
FTB_ROW("wind", 100, "s2_wind_03")
FTB_ROW("wind", 133.333, "s2_wind_04")
FTB_ROW("wind", 166.667, "s2_wind_05")
FTB_ROW("wind", 200, "s2_wind_06")
FTB_ROW("wind", 233.333, "s2_wind_07")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 66.667, "s2_swing_02")
FTB_ROW("swing", 83.333, "s2_swing_03")
FTB_ROW("swing", 100, "s2_swing_04")
FTB_ROW("swing", 116.667, "s2_swing_05")
FTB_ROW("swing", 150, "s2_swing_06")
FTB_ROW("swing", 166.667, "s2_swing_07")
FTB_ROW("swing", 183.333, "s2_swing_08")

FTB_STATE("dash", Time, 750)
FTB_ROW("dash", 0, "run")   // KEY
FTB_ROW("dash", 33.333, "s2_dash_01")
FTB_ROW("dash", 66.667, "s2_dash_02")
FTB_ROW("dash", 100, "s2_dash_01")
FTB_ROW("dash", 133.333, "s2_dash_02")
FTB_ROW("dash", 166.667, "s2_dash_03")
FTB_ROW("dash", 200, "s2_dash_06")
FTB_ROW("dash", 233.333, "s2_dash_08")
FTB_ROW("dash", 266.667, "s2_dash_04")
FTB_ROW("dash", 300, "s2_dash_05")
FTB_ROW("dash", 333.333, "s2_dash_09")
FTB_ROW("dash", 366.667, "s2_dash_06")
FTB_ROW("dash", 400, "s2_dash_08")
FTB_ROW("dash", 433.333, "s2_dash_04")
FTB_ROW("dash", 466.667, "s2_dash_05")
FTB_ROW("dash", 500, "s2_dash_09")
FTB_ROW("dash", 533.333, "s2_dash_06")
FTB_ROW("dash", 566.667, "s2_dash_08")
FTB_ROW("dash", 600, "s2_dash_04")
FTB_ROW("dash", 633.333, "s2_dash_05")
FTB_ROW("dash", 666.667, "s2_dash_09")
FTB_ROW("dash", 700, "s2_dash_06")
FTB_ROW("dash", 733.333, "s2_dash_08")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 50, "s2_hurt_02")
FTB_ROW("hurt", 83.333, "s2_hurt_03")
FTB_ROW("hurt", 100, "s2_hurt_04")
FTB_ROW("hurt", 133.333, "s2_hurt_05")
FTB_ROW("hurt", 166.667, "s2_hurt_06")
FTB_ROW("hurt", 200, "s2_hurt_07")
FTB_ROW("hurt", 233.333, "s2_hurt_08")

FTB_STATE("fly", Time, 453.3)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_fly_01")
FTB_ROW("fly", 100, "s2_fly_02")
FTB_ROW("fly", 166.667, "s2_fly_03")
FTB_ROW("fly", 233.333, "s2_fly_04")
FTB_ROW("fly", 300, "s2_fly_05")
FTB_ROW("fly", 366.667, "s2_fly_08")

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "down")   // KEY
FTB_ROW("getup", 50, "s2_getup_01")
FTB_ROW("getup", 83.333, "s2_getup_03")
FTB_ROW("getup", 133.333, "s2_getup_04")
FTB_ROW("getup", 166.667, "s2_getup_05")
FTB_ROW("getup", 200, "s2_getup_06")
FTB_ROW("getup", 233.333, "s2_getup_07")
FTB_ROW("getup", 266.667, "s2_getup_08")
FTB_ROW("getup", 300, "s2_getup_09")
FTB_ROW("getup", 333.333, "s2_getup_10")
FTB_ROW("getup", 366.667, "s2_getup_11")
FTB_ROW("getup", 416.667, "s2_getup_12")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
