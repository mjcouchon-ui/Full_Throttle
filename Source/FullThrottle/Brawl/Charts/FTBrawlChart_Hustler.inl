// Exposure chart of Hustler. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// Stage 2: s2_* rows added between the keys. Time-state rows sit on fight steps (16.667 ms). idle: Time -> Loop, same 1256.6 ms period.

FTB_STATE("idle", Loop, 1256.6)   // one cycle
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 150, "s2_idle_01")
FTB_ROW("idle", 283.333, "s2_idle_02")
FTB_ROW("idle", 433.333, "s2_idle_03")
FTB_ROW("idle", 633.333, "s2_idle_04")
FTB_ROW("idle", 766.667, "s2_idle_05")
FTB_ROW("idle", 900, "s2_idle_06")
FTB_ROW("idle", 1100, "s2_idle_07")

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 37.879, "s2_walk_01")
FTB_ROW("walk", 75.758, "s2_walk_02")
FTB_ROW("walk", 113.636, "s2_walk_03")
FTB_ROW("walk", 151.515, "s2_walk_04")
FTB_ROW("walk", 189.394, "s2_walk_05")
FTB_ROW("walk", 227.273, "s2_walk_06")
FTB_ROW("walk", 265.152, "s2_walk_07")
FTB_ROW("walk", 303.03, "s2_walk_08")
FTB_ROW("walk", 340.909, "s2_walk_09")
FTB_ROW("walk", 378.788, "s2_walk_10")
FTB_ROW("walk", 416.667, "s2_walk_11")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 492.424, "s2_walk_12")
FTB_ROW("walk", 530.303, "s2_walk_13")
FTB_ROW("walk", 568.182, "s2_walk_14")
FTB_ROW("walk", 606.061, "s2_walk_15")
FTB_ROW("walk", 643.939, "s2_walk_16")
FTB_ROW("walk", 681.818, "s2_walk_17")
FTB_ROW("walk", 719.697, "s2_walk_18")
FTB_ROW("walk", 757.576, "s2_walk_19")
FTB_ROW("walk", 795.455, "s2_walk_20")
FTB_ROW("walk", 833.333, "s2_walk_21")
FTB_ROW("walk", 871.212, "s2_walk_22")

FTB_STATE("wind", Time, 320)
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 66.667, "s2_wind_02")
FTB_ROW("wind", 100, "s2_wind_03")
FTB_ROW("wind", 133.333, "s2_wind_04")
FTB_ROW("wind", 166.667, "s2_wind_05")
FTB_ROW("wind", 200, "s2_wind_04")
FTB_ROW("wind", 233.333, "s2_wind_05")
FTB_ROW("wind", 266.667, "s2_wind_04")
FTB_ROW("wind", 283.333, "s2_wind_06")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 50, "s2_swing_01")
FTB_ROW("swing", 83.333, "s2_swing_02")
FTB_ROW("swing", 116.667, "s2_swing_03")
FTB_ROW("swing", 150, "s2_swing_04")
FTB_ROW("swing", 183.333, "s2_swing_05")

FTB_STATE("grab", Time, 450)
FTB_ROW("grab", 0, "grab")   // KEY
FTB_ROW("grab", 33.333, "s2_grab_01")
FTB_ROW("grab", 66.667, "s2_grab_02")
FTB_ROW("grab", 100, "s2_grab_03")
FTB_ROW("grab", 133.333, "s2_grab_04")
FTB_ROW("grab", 200, "s2_grab_06")
FTB_ROW("grab", 233.333, "s2_grab_07")
FTB_ROW("grab", 300, "s2_grab_09")
FTB_ROW("grab", 333.333, "s2_grab_10")
FTB_ROW("grab", 366.667, "s2_grab_03")
FTB_ROW("grab", 400, "s2_grab_02")
FTB_ROW("grab", 433.333, "s2_grab_10")

FTB_STATE("knee", Time, 420)
FTB_ROW("knee", 0, "grab")   // KEY
FTB_ROW("knee", 16.667, "s2_knee_01")
FTB_ROW("knee", 50, "s2_knee_02")
FTB_ROW("knee", 83.333, "s2_knee_03")
FTB_ROW("knee", 116.667, "s2_knee_04")
FTB_ROW("knee", 166.667, "s2_knee_05")
FTB_ROW("knee", 200, "s2_knee_06")
FTB_ROW("knee", 233.333, "s2_knee_07")
FTB_ROW("knee", 266.667, "s2_grab_10")
FTB_ROW("knee", 300, "s2_grab_03")
FTB_ROW("knee", 333.333, "s2_grab_02")
FTB_ROW("knee", 366.667, "s2_grab_10")
FTB_ROW("knee", 400, "s2_grab_03")

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
FTB_ROW("fly", 83.333, "s2_fly_02")
FTB_ROW("fly", 150, "s2_fly_03")
FTB_ROW("fly", 216.667, "s2_fly_04")
FTB_ROW("fly", 300, "s2_fly_05")

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "down")   // KEY
FTB_ROW("getup", 33.333, "s2_getup_01")
FTB_ROW("getup", 66.667, "s2_getup_02")
FTB_ROW("getup", 100, "s2_getup_03")
FTB_ROW("getup", 133.333, "s2_getup_04")
FTB_ROW("getup", 183.333, "s2_getup_05")
FTB_ROW("getup", 216.667, "s2_getup_06")
FTB_ROW("getup", 250, "s2_getup_07")
FTB_ROW("getup", 283.333, "s2_getup_08")
FTB_ROW("getup", 316.667, "s2_getup_09")
FTB_ROW("getup", 350, "s2_getup_10")
FTB_ROW("getup", 383.333, "s2_getup_11")
FTB_ROW("getup", 416.667, "s2_getup_12")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
