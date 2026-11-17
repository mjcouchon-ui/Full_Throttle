// Exposure chart of Firedancer. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.

FTB_STATE("idle", Loop, 1256.6)   // one breath
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 179.514, "s2_idle_01")
FTB_ROW("idle", 359.028, "s2_idle_02")
FTB_ROW("idle", 538.542, "s2_idle_03")
FTB_ROW("idle", 718.056, "s2_idle_04")
FTB_ROW("idle", 897.57, "s2_idle_05")
FTB_ROW("idle", 1077.084, "s2_idle_04")

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 41.322, "s2_walk_01")
FTB_ROW("walk", 82.645, "s2_walk_02")
FTB_ROW("walk", 123.967, "s2_walk_03")
FTB_ROW("walk", 165.289, "s2_walk_04")
FTB_ROW("walk", 206.612, "s2_walk_05")
FTB_ROW("walk", 247.934, "s2_walk_06")
FTB_ROW("walk", 289.256, "s2_walk_07")
FTB_ROW("walk", 330.579, "s2_walk_08")
FTB_ROW("walk", 413.223, "s2_walk_10")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 495.868, "s2_walk_11")
FTB_ROW("walk", 537.19, "s2_walk_12")
FTB_ROW("walk", 578.512, "s2_walk_13")
FTB_ROW("walk", 619.835, "s2_walk_14")
FTB_ROW("walk", 661.157, "s2_walk_15")
FTB_ROW("walk", 702.479, "s2_walk_16")
FTB_ROW("walk", 743.802, "s2_walk_17")
FTB_ROW("walk", 826.446, "s2_walk_19")
FTB_ROW("walk", 867.769, "s2_walk_20")

FTB_STATE("wind", Time, 280)
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 50, "s2_wind_01")
FTB_ROW("wind", 83.333, "s2_wind_02")
FTB_ROW("wind", 116.667, "s2_wind_03")
FTB_ROW("wind", 150, "s2_wind_04")
FTB_ROW("wind", 183.333, "s2_wind_05")
FTB_ROW("wind", 216.667, "s2_wind_06")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 66.667, "s2_swing_02")
FTB_ROW("swing", 100, "s2_swing_03")
FTB_ROW("swing", 133.333, "s2_idle_05")
FTB_ROW("swing", 166.667, "s2_idle_04")
FTB_ROW("swing", 200, "s2_idle_01")

FTB_STATE("dash", Time, 750)
FTB_ROW("dash", 0, "run")   // KEY
FTB_ROW("dash", 33.333, "s2_dash_01")
FTB_ROW("dash", 66.667, "s2_dash_02")
FTB_ROW("dash", 100, "s2_dash_03")
FTB_ROW("dash", 133.333, "s2_dash_04")
FTB_ROW("dash", 166.667, "s2_dash_05")
FTB_ROW("dash", 200, "s2_dash_06")
FTB_ROW("dash", 233.333, "s2_dash_07")
FTB_ROW("dash", 266.667, "run")
FTB_ROW("dash", 300, "s2_dash_01")
FTB_ROW("dash", 333.333, "s2_dash_02")
FTB_ROW("dash", 366.667, "s2_dash_03")
FTB_ROW("dash", 400, "s2_dash_04")
FTB_ROW("dash", 433.333, "s2_dash_05")
FTB_ROW("dash", 466.667, "s2_dash_06")
FTB_ROW("dash", 500, "s2_dash_07")
FTB_ROW("dash", 533.333, "run")
FTB_ROW("dash", 566.667, "s2_dash_01")
FTB_ROW("dash", 600, "s2_dash_02")
FTB_ROW("dash", 633.333, "s2_dash_03")
FTB_ROW("dash", 666.667, "s2_dash_04")
FTB_ROW("dash", 700, "s2_dash_05")
FTB_ROW("dash", 733.333, "s2_dash_06")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 66.667, "s2_hurt_02")
FTB_ROW("hurt", 100, "s2_hurt_03")
FTB_ROW("hurt", 133.333, "s2_hurt_04")
FTB_ROW("hurt", 166.667, "s2_hurt_05")
FTB_ROW("hurt", 200, "s2_hurt_06")
FTB_ROW("hurt", 233.333, "s2_idle_01")

FTB_STATE("fly", Time, 453.3)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_fly_01")
FTB_ROW("fly", 50, "s2_fly_02")
FTB_ROW("fly", 66.667, "s2_fly_03")
FTB_ROW("fly", 100, "s2_fly_04")
FTB_ROW("fly", 133.333, "s2_fly_05")
FTB_ROW("fly", 166.667, "s2_fly_06")
FTB_ROW("fly", 200, "s2_fly_07")
FTB_ROW("fly", 233.333, "s2_fly_08")
FTB_ROW("fly", 266.667, "s2_fly_09")
FTB_ROW("fly", 300, "s2_fly_10")
FTB_ROW("fly", 333.333, "s2_fly_11")
FTB_ROW("fly", 366.667, "s2_fly_12")
FTB_ROW("fly", 400, "s2_fly_13")
FTB_ROW("fly", 416.667, "s2_fly_14")
FTB_ROW("fly", 433.333, "s2_fly_15")

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "down")   // KEY
FTB_ROW("getup", 50, "s2_getup_01")
FTB_ROW("getup", 83.333, "s2_getup_02")
FTB_ROW("getup", 116.667, "s2_getup_03")
FTB_ROW("getup", 150, "s2_getup_04")
FTB_ROW("getup", 183.333, "s2_getup_05")
FTB_ROW("getup", 216.667, "s2_getup_06")
FTB_ROW("getup", 250, "s2_getup_07")
FTB_ROW("getup", 283.333, "s2_getup_08")
FTB_ROW("getup", 316.667, "s2_getup_09")
FTB_ROW("getup", 350, "s2_getup_10")
FTB_ROW("getup", 383.333, "s2_idle_02")
FTB_ROW("getup", 416.667, "s2_idle_01")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
