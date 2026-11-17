// Exposure chart of Boarder. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// s2_ drawings: C:/ftwork/final/brawl/art/Boarder/frames (manifest.json beside it says which sheet each came from).
// idle is declared Loop (the contract lists it as a 1257 ms loop; the base chart said Time): see manifest findings.

FTB_STATE("idle", Loop, 1256.6)
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 157.075, "s2_idle_01")
FTB_ROW("idle", 314.15, "s2_idle_02")
FTB_ROW("idle", 471.225, "s2_idle_01")
FTB_ROW("idle", 628.3, "idle")
FTB_ROW("idle", 785.375, "s2_idle_03")
FTB_ROW("idle", 942.45, "s2_idle_04")
FTB_ROW("idle", 1099.525, "s2_idle_03")

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
FTB_ROW("walk", 454.546, "walk2")   // KEY
FTB_ROW("walk", 489.511, "s2_walk_09")
FTB_ROW("walk", 524.476, "s2_walk_10")
FTB_ROW("walk", 559.441, "s2_walk_21")
FTB_ROW("walk", 594.406, "s2_walk_11")
FTB_ROW("walk", 629.371, "s2_walk_22")
FTB_ROW("walk", 664.336, "s2_walk_12")
FTB_ROW("walk", 699.301, "s2_walk_23")
FTB_ROW("walk", 734.266, "s2_walk_13")
FTB_ROW("walk", 769.231, "s2_walk_24")
FTB_ROW("walk", 804.196, "s2_walk_14")
FTB_ROW("walk", 839.161, "s2_walk_15")
FTB_ROW("walk", 874.126, "s2_walk_16")

FTB_STATE("wind", Time, 300)
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 66.667, "s2_wind_02")
FTB_ROW("wind", 100, "s2_wind_03")
FTB_ROW("wind", 133.333, "s2_wind_04")
FTB_ROW("wind", 166.667, "s2_wind_05")
FTB_ROW("wind", 200, "s2_wind_06")
FTB_ROW("wind", 233.333, "s2_wind_07")
FTB_ROW("wind", 266.667, "s2_wind_08")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 50, "s2_swing_02")
FTB_ROW("swing", 66.667, "s2_swing_03")
FTB_ROW("swing", 100, "s2_swing_04")
FTB_ROW("swing", 133.333, "s2_swing_05")
FTB_ROW("swing", 166.667, "s2_swing_06")
FTB_ROW("swing", 183.333, "s2_swing_07")
FTB_ROW("swing", 200, "s2_swing_08")

FTB_STATE("throw", Time, 750)
FTB_ROW("throw", 0, "throw")   // KEY
FTB_ROW("throw", 33.333, "s2_throw_01")
FTB_ROW("throw", 66.667, "s2_throw_02")
FTB_ROW("throw", 100, "wind")
FTB_ROW("throw", 133.333, "s2_wind_01")
FTB_ROW("throw", 166.667, "s2_wind_02")
FTB_ROW("throw", 200, "s2_wind_03")
FTB_ROW("throw", 216.667, "s2_wind_04")
FTB_ROW("throw", 250, "s2_wind_05")
FTB_ROW("throw", 266.667, "s2_wind_06")
FTB_ROW("throw", 283.333, "s2_wind_07")
FTB_ROW("throw", 300, "s2_wind_08")
FTB_ROW("throw", 316.667, "s2_throw_03")
FTB_ROW("throw", 333.333, "s2_throw_04")
FTB_ROW("throw", 366.667, "s2_throw_05")
FTB_ROW("throw", 400, "s2_throw_06")
FTB_ROW("throw", 433.333, "s2_throw_07")
FTB_ROW("throw", 466.667, "s2_throw_08")
FTB_ROW("throw", 500, "s2_throw_09")
FTB_ROW("throw", 533.333, "s2_throw_10")
FTB_ROW("throw", 566.667, "s2_throw_11")
FTB_ROW("throw", 600, "s2_throw_12")
FTB_ROW("throw", 633.333, "s2_throw_13")
FTB_ROW("throw", 666.667, "s2_throw_14")
FTB_ROW("throw", 700, "s2_throw_15")
FTB_ROW("throw", 733.333, "s2_throw_16")

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
FTB_ROW("fly", 33.333, "s2_fly_01")
FTB_ROW("fly", 50, "s2_fly_10")
FTB_ROW("fly", 66.667, "s2_fly_02")
FTB_ROW("fly", 100, "s2_fly_03")
FTB_ROW("fly", 133.333, "s2_fly_05")
FTB_ROW("fly", 166.667, "s2_fly_12")
FTB_ROW("fly", 200, "s2_fly_11")
FTB_ROW("fly", 233.333, "s2_fly_04")
FTB_ROW("fly", 266.667, "s2_fly_15")
FTB_ROW("fly", 300, "s2_fly_13")
FTB_ROW("fly", 333.333, "s2_fly_06")
FTB_ROW("fly", 366.667, "s2_fly_09")
FTB_ROW("fly", 400, "s2_fly_14")
FTB_ROW("fly", 433.333, "s2_fly_16")

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
FTB_ROW("getup", 333.333, "s2_getup_10")
FTB_ROW("getup", 366.667, "s2_getup_11")
FTB_ROW("getup", 400, "s2_getup_12")
FTB_ROW("getup", 433.333, "s2_swing_07")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
