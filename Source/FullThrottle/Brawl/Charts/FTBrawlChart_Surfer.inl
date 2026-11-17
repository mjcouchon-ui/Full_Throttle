// Exposure chart of Surfer. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.

FTB_STATE("idle", Loop, 1256.6)
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 209.433, "s2_idle_01")
FTB_ROW("idle", 418.867, "s2_idle_02")
FTB_ROW("idle", 628.3, "s2_idle_03")
FTB_ROW("idle", 837.733, "s2_idle_04")
FTB_ROW("idle", 1047.167, "s2_idle_05")

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 32.468, "s2_walk_01")
FTB_ROW("walk", 64.935, "s2_walk_02")
FTB_ROW("walk", 97.403, "s2_walk_03")
FTB_ROW("walk", 129.87, "s2_walk_04")
FTB_ROW("walk", 162.338, "s2_walk_05")
FTB_ROW("walk", 194.805, "s2_walk_06")
FTB_ROW("walk", 227.273, "s2_walk_07")
FTB_ROW("walk", 255.682, "s2_walk_08")
FTB_ROW("walk", 284.091, "s2_walk_09")
FTB_ROW("walk", 312.5, "s2_walk_10")
FTB_ROW("walk", 340.909, "s2_walk_11")
FTB_ROW("walk", 369.318, "s2_walk_12")
FTB_ROW("walk", 397.727, "s2_walk_13")
FTB_ROW("walk", 426.136, "s2_walk_14")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 487.013, "s2_walk_01")
FTB_ROW("walk", 519.481, "s2_walk_02")
FTB_ROW("walk", 551.948, "s2_walk_03")
FTB_ROW("walk", 584.416, "s2_walk_04")
FTB_ROW("walk", 616.883, "s2_walk_05")
FTB_ROW("walk", 649.351, "s2_walk_06")
FTB_ROW("walk", 681.818, "s2_walk_07")
FTB_ROW("walk", 710.227, "s2_walk_08")
FTB_ROW("walk", 738.636, "s2_walk_09")
FTB_ROW("walk", 767.046, "s2_walk_10")
FTB_ROW("walk", 795.455, "s2_walk_11")
FTB_ROW("walk", 823.864, "s2_walk_12")
FTB_ROW("walk", 852.273, "s2_walk_13")
FTB_ROW("walk", 880.682, "s2_walk_14")

FTB_STATE("wind", Time, 320)
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 66.667, "s2_wind_03")
FTB_ROW("wind", 116.667, "s2_wind_07")
FTB_ROW("wind", 166.667, "s2_wind_08")
FTB_ROW("wind", 200, "s2_wind_03")
FTB_ROW("wind", 233.333, "s2_wind_04")
FTB_ROW("wind", 266.667, "s2_wind_05")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 66.667, "s2_swing_02")
FTB_ROW("swing", 100, "s2_swing_03")
FTB_ROW("swing", 133.333, "s2_swing_04")
FTB_ROW("swing", 166.667, "s2_swing_05")
FTB_ROW("swing", 200, "s2_swing_06")

FTB_STATE("grab", Time, 450)
FTB_ROW("grab", 0, "grab")   // KEY
FTB_ROW("grab", 33.333, "s2_grab_01")
FTB_ROW("grab", 66.667, "s2_grab_02")
FTB_ROW("grab", 116.667, "s2_grab_08")
FTB_ROW("grab", 166.667, "s2_grab_04")
FTB_ROW("grab", 200, "s2_grab_06")
FTB_ROW("grab", 233.333, "s2_grab_07")
FTB_ROW("grab", 266.667, "s2_grab_02")
FTB_ROW("grab", 300, "s2_grab_01")
FTB_ROW("grab", 333.333, "s2_grab_02")
FTB_ROW("grab", 366.667, "s2_grab_07")
FTB_ROW("grab", 400, "s2_grab_06")
FTB_ROW("grab", 433.333, "s2_grab_07")

FTB_STATE("knee", Time, 420)
FTB_ROW("knee", 0, "grab")   // KEY
FTB_ROW("knee", 16.667, "s2_knee_01")
FTB_ROW("knee", 50, "s2_knee_02")
FTB_ROW("knee", 83.333, "s2_knee_03")
FTB_ROW("knee", 116.667, "s2_knee_04")
FTB_ROW("knee", 150, "s2_knee_05")
FTB_ROW("knee", 183.333, "s2_knee_06")
FTB_ROW("knee", 216.667, "s2_knee_07")
FTB_ROW("knee", 250, "s2_grab_06")
FTB_ROW("knee", 283.333, "s2_grab_07")
FTB_ROW("knee", 316.667, "s2_grab_02")
FTB_ROW("knee", 350, "s2_grab_01")
FTB_ROW("knee", 383.333, "s2_grab_02")

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
FTB_ROW("fly", 100, "s2_fly_02")
FTB_ROW("fly", 166.667, "s2_fly_03")
FTB_ROW("fly", 250, "s2_fly_04")
FTB_ROW("fly", 333.333, "s2_fly_05")

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "down")   // KEY
FTB_ROW("getup", 33.333, "s2_getup_01")
FTB_ROW("getup", 83.333, "s2_getup_02")
FTB_ROW("getup", 133.333, "s2_getup_03")
FTB_ROW("getup", 183.333, "s2_getup_04")
FTB_ROW("getup", 233.333, "s2_getup_05")
FTB_ROW("getup", 283.333, "s2_getup_06")
FTB_ROW("getup", 316.667, "s2_getup_07")
FTB_ROW("getup", 350, "s2_swing_04")
FTB_ROW("getup", 383.333, "s2_hurt_06")
FTB_ROW("getup", 416.667, "s2_swing_06")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
