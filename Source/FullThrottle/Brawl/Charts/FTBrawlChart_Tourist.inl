// Exposure chart of Tourist. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings (s2_*) stand between them. Times are ms; in a Walk or Loop state they are positions in the cycle.

FTB_STATE("idle", Loop, 1256.6)
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 209.433, "s2_idleb_01")
FTB_ROW("idle", 418.867, "s2_idleb_02")
FTB_ROW("idle", 628.3, "s2_idleb_03")
FTB_ROW("idle", 837.733, "s2_idleb_05")
FTB_ROW("idle", 1047.167, "s2_idleb_01")

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 34.965, "s2_walk_20")
FTB_ROW("walk", 69.93, "s2_walk_13")
FTB_ROW("walk", 104.895, "s2_walk_09")
FTB_ROW("walk", 139.86, "s2_walk_02")
FTB_ROW("walk", 174.825, "s2_walk_23")
FTB_ROW("walk", 209.79, "s2_walk_17")
FTB_ROW("walk", 244.755, "s2_walk_10")
FTB_ROW("walk", 279.72, "s2_walk_22")
FTB_ROW("walk", 314.685, "s2_walk_24")
FTB_ROW("walk", 349.65, "s2_walk_15")
FTB_ROW("walk", 384.615, "s2_walk_16")
FTB_ROW("walk", 419.58, "s2_walk_03")
FTB_ROW("walk", 454.546, "walk2")   // KEY
FTB_ROW("walk", 489.511, "s2_walk_20")
FTB_ROW("walk", 524.476, "s2_walk_13")
FTB_ROW("walk", 559.441, "s2_walk_09")
FTB_ROW("walk", 594.406, "s2_walk_02")
FTB_ROW("walk", 629.371, "s2_walk_23")
FTB_ROW("walk", 664.336, "s2_walk_17")
FTB_ROW("walk", 699.301, "s2_walk_10")
FTB_ROW("walk", 734.266, "s2_walk_22")
FTB_ROW("walk", 769.231, "s2_walk_24")
FTB_ROW("walk", 804.196, "s2_walk_15")
FTB_ROW("walk", 839.161, "s2_walk_16")
FTB_ROW("walk", 874.126, "s2_walk_03")

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
FTB_ROW("swing", 33.333, "s2_swing_06")
FTB_ROW("swing", 66.667, "s2_swing_07")
FTB_ROW("swing", 83.333, "s2_swing_02")
FTB_ROW("swing", 100, "s2_swing_03")
FTB_ROW("swing", 133.333, "s2_swing_08")
FTB_ROW("swing", 150, "s2_swing_04")
FTB_ROW("swing", 183.333, "s2_swing_01")

FTB_STATE("throw", Time, 750)
FTB_ROW("throw", 0, "throw")   // KEY
FTB_ROW("throw", 16.667, "s2_throw_05")
FTB_ROW("throw", 66.667, "s2_wind_02")
FTB_ROW("throw", 100, "s2_wind_03")
FTB_ROW("throw", 133.333, "s2_wind_04")
FTB_ROW("throw", 166.667, "s2_throw_06")
FTB_ROW("throw", 200, "s2_throw_07")
FTB_ROW("throw", 233.333, "s2_throw_08")
FTB_ROW("throw", 266.667, "s2_throw_09")
FTB_ROW("throw", 300, "s2_wind_08")
FTB_ROW("throw", 316.667, "s2_throw_21")
FTB_ROW("throw", 350, "s2_throw_02")
FTB_ROW("throw", 383.333, "s2_throw_03")
FTB_ROW("throw", 416.667, "s2_throw_13")
FTB_ROW("throw", 450, "s2_throw_04")
FTB_ROW("throw", 483.333, "s2_throw_01")
FTB_ROW("throw", 583.333, "s2_throw_20")
FTB_ROW("throw", 650, "s2_throw_12")

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

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "down")   // KEY
FTB_ROW("getup", 33.333, "s2_getup_06")
FTB_ROW("getup", 66.667, "s2_getup_02")
FTB_ROW("getup", 100, "s2_getup_10")
FTB_ROW("getup", 133.333, "s2_getup_03")
FTB_ROW("getup", 166.667, "s2_getup_07")
FTB_ROW("getup", 183.333, "s2_getup_08")
FTB_ROW("getup", 216.667, "s2_getup_11")
FTB_ROW("getup", 250, "s2_getup_04")
FTB_ROW("getup", 283.333, "s2_getup_12")
FTB_ROW("getup", 316.667, "s2_getup_01")
FTB_ROW("getup", 350, "s2_getup_09")
FTB_ROW("getup", 383.333, "s2_getup_05")
FTB_ROW("getup", 416.667, "s2_hurt_07")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
