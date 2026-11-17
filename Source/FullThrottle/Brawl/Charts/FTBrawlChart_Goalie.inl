// Exposure chart of Goalie. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// s2_ drawings: C:/ftwork/final/brawl/art/Goalie/frames (manifest.json beside it says which sheet each came from).

FTB_STATE("idle", Loop, 1256.6)
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 157.075, "s2_idle_01")
FTB_ROW("idle", 314.15, "s2_idle_02")
FTB_ROW("idle", 471.225, "s2_idle_03")
FTB_ROW("idle", 628.3, "s2_idle_02")
FTB_ROW("idle", 785.375, "s2_idle_04")
FTB_ROW("idle", 942.45, "s2_idle_05")
FTB_ROW("idle", 1099.525, "s2_idle_06")

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 41.322, "s2_walk_01")
FTB_ROW("walk", 82.645, "s2_walk_02")
FTB_ROW("walk", 123.967, "s2_walk_03")
FTB_ROW("walk", 165.289, "s2_walk_15")
FTB_ROW("walk", 206.611, "s2_walk_04")
FTB_ROW("walk", 247.934, "s2_walk_16")
FTB_ROW("walk", 289.256, "s2_walk_05")
FTB_ROW("walk", 330.578, "s2_walk_17")
FTB_ROW("walk", 371.9, "s2_walk_06")
FTB_ROW("walk", 413.223, "s2_walk_07")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 495.867, "s2_walk_08")
FTB_ROW("walk", 537.19, "s2_walk_09")
FTB_ROW("walk", 578.512, "s2_walk_10")
FTB_ROW("walk", 619.834, "s2_walk_18")
FTB_ROW("walk", 661.157, "s2_walk_11")
FTB_ROW("walk", 702.479, "s2_walk_19")
FTB_ROW("walk", 743.802, "s2_walk_12")
FTB_ROW("walk", 785.124, "s2_walk_20")
FTB_ROW("walk", 826.446, "s2_walk_13")
FTB_ROW("walk", 867.769, "s2_walk_14")

FTB_STATE("wind", Time, 550)
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 66.667, "s2_wind_02")
FTB_ROW("wind", 100, "s2_wind_09")
FTB_ROW("wind", 133.333, "s2_wind_02")
FTB_ROW("wind", 166.667, "s2_wind_01")
FTB_ROW("wind", 200, "wind")
FTB_ROW("wind", 233.333, "s2_wind_07")
FTB_ROW("wind", 266.667, "s2_wind_03")
FTB_ROW("wind", 300, "s2_wind_08")
FTB_ROW("wind", 333.333, "s2_wind_03")
FTB_ROW("wind", 366.667, "s2_wind_04")
FTB_ROW("wind", 400, "s2_wind_07")
FTB_ROW("wind", 433.333, "s2_wind_04")
FTB_ROW("wind", 466.667, "s2_wind_05")
FTB_ROW("wind", 500, "s2_wind_06")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 50, "s2_swing_02")
FTB_ROW("swing", 83.333, "s2_swing_03")
FTB_ROW("swing", 116.667, "s2_swing_04")
FTB_ROW("swing", 150, "s2_swing_06")
FTB_ROW("swing", 183.333, "s2_swing_05")

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
FTB_ROW("fly", 66.667, "s2_fly_04")
FTB_ROW("fly", 100, "s2_fly_06")
FTB_ROW("fly", 133.333, "s2_fly_08")
FTB_ROW("fly", 166.667, "s2_fly_01")
FTB_ROW("fly", 200, "s2_fly_04")
FTB_ROW("fly", 233.333, "s2_fly_06")
FTB_ROW("fly", 266.667, "s2_fly_08")
FTB_ROW("fly", 300, "s2_fly_03")
FTB_ROW("fly", 333.333, "s2_fly_07")
FTB_ROW("fly", 366.667, "s2_fly_02")
FTB_ROW("fly", 400, "s2_fly_05")

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "getup")   // KEY
FTB_ROW("getup", 50, "s2_getup_01")
FTB_ROW("getup", 83.333, "s2_getup_02")
FTB_ROW("getup", 116.667, "s2_getup_03")
FTB_ROW("getup", 150, "s2_getup_04")
FTB_ROW("getup", 183.333, "s2_getup_05")
FTB_ROW("getup", 216.667, "s2_getup_06")
FTB_ROW("getup", 250, "s2_getup_07")
FTB_ROW("getup", 283.333, "s2_getup_08")
FTB_ROW("getup", 316.667, "s2_getup_09")
FTB_ROW("getup", 366.667, "s2_getup_10")
FTB_ROW("getup", 400, "s2_getup_11")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
