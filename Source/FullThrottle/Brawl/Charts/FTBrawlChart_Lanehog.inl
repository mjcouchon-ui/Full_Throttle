// Exposure chart of Lanehog. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.

FTB_STATE("idle", Time, 1256.6)
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 209.433, "s2_idle_01")
FTB_ROW("idle", 418.867, "s2_idle_02")
FTB_ROW("idle", 628.3, "s2_idle_01")
FTB_ROW("idle", 837.733, "s2_idle_03")
FTB_ROW("idle", 1047.167, "s2_hurt_09")

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 26.738, "s2_walk_01")
FTB_ROW("walk", 53.476, "s2_walk_02")
FTB_ROW("walk", 80.214, "s2_walk_03")
FTB_ROW("walk", 106.952, "s2_walk_04")
FTB_ROW("walk", 133.69, "s2_walk_05")
FTB_ROW("walk", 160.428, "s2_walk_06")
FTB_ROW("walk", 187.166, "s2_walk_07")
FTB_ROW("walk", 213.904, "s2_walk_08")
FTB_ROW("walk", 240.642, "s2_walk_09")
FTB_ROW("walk", 267.38, "s2_walk_10")
FTB_ROW("walk", 294.118, "s2_walk_11")
FTB_ROW("walk", 320.856, "s2_walk_12")
FTB_ROW("walk", 347.594, "s2_walk_13")
FTB_ROW("walk", 374.332, "s2_walk_14")
FTB_ROW("walk", 401.07, "s2_walk_15")
FTB_ROW("walk", 427.808, "s2_walk_16")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 481.283, "s2_walk_01")
FTB_ROW("walk", 508.021, "s2_walk_02")
FTB_ROW("walk", 534.759, "s2_walk_03")
FTB_ROW("walk", 561.497, "s2_walk_04")
FTB_ROW("walk", 588.235, "s2_walk_05")
FTB_ROW("walk", 614.973, "s2_walk_06")
FTB_ROW("walk", 641.711, "s2_walk_07")
FTB_ROW("walk", 668.449, "s2_walk_08")
FTB_ROW("walk", 695.187, "s2_walk_09")
FTB_ROW("walk", 721.925, "s2_walk_10")
FTB_ROW("walk", 748.663, "s2_walk_11")
FTB_ROW("walk", 775.401, "s2_walk_12")
FTB_ROW("walk", 802.139, "s2_walk_13")
FTB_ROW("walk", 828.877, "s2_walk_14")
FTB_ROW("walk", 855.615, "s2_walk_15")
FTB_ROW("walk", 882.353, "s2_walk_16")

FTB_STATE("wind", Time, 550)
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 50, "s2_wind_02")
FTB_ROW("wind", 83.333, "s2_wind_03")
FTB_ROW("wind", 116.667, "s2_wind_04")
FTB_ROW("wind", 150, "s2_wind_05")
FTB_ROW("wind", 183.333, "s2_wind_03")
FTB_ROW("wind", 216.667, "s2_wind_06")
FTB_ROW("wind", 250, "s2_wind_01")
FTB_ROW("wind", 283.333, "s2_wind_04")
FTB_ROW("wind", 316.667, "s2_wind_05")
FTB_ROW("wind", 350, "s2_wind_04")
FTB_ROW("wind", 383.333, "s2_wind_07")
FTB_ROW("wind", 416.667, "s2_wind_08")
FTB_ROW("wind", 450, "s2_wind_09")
FTB_ROW("wind", 483.333, "s2_wind_10")
FTB_ROW("wind", 516.667, "s2_wind_11")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 66.667, "s2_swing_02")
FTB_ROW("swing", 100, "s2_swing_03")
FTB_ROW("swing", 133.333, "s2_swing_04")
FTB_ROW("swing", 166.667, "s2_swing_05")
FTB_ROW("swing", 200, "s2_swing_06")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 66.667, "s2_hurt_02")
FTB_ROW("hurt", 83.333, "s2_hurt_03")
FTB_ROW("hurt", 116.667, "s2_hurt_04")
FTB_ROW("hurt", 133.333, "s2_hurt_05")
FTB_ROW("hurt", 166.667, "s2_hurt_06")
FTB_ROW("hurt", 200, "s2_hurt_07")
FTB_ROW("hurt", 233.333, "s2_hurt_08")

FTB_STATE("fly", Time, 453.3)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_fly_01")
FTB_ROW("fly", 100, "s2_fly_02")
FTB_ROW("fly", 216.667, "s2_fly_03")
FTB_ROW("fly", 333.333, "s2_fly_02")

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "getup")   // KEY
FTB_ROW("getup", 66.667, "s2_getup_01")
FTB_ROW("getup", 100, "s2_getup_02")
FTB_ROW("getup", 133.333, "s2_getup_03")
FTB_ROW("getup", 166.667, "s2_getup_04")
FTB_ROW("getup", 200, "s2_getup_05")
FTB_ROW("getup", 300, "s2_getup_07")
FTB_ROW("getup", 333.333, "s2_getup_08")
FTB_ROW("getup", 366.667, "s2_getup_09")
FTB_ROW("getup", 400, "s2_idle_01")
FTB_ROW("getup", 433.333, "s2_hurt_09")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
