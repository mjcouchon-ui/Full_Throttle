// Exposure chart of Linemate. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// s2_ drawings: C:/ftwork/final/brawl/art/Linemate/frames (manifest.json beside it says which sheet each came from).

FTB_STATE("idle", Loop, 1256.6)
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 139.622, "s2_idle_01")
FTB_ROW("idle", 279.244, "s2_idle_02")
FTB_ROW("idle", 418.867, "s2_idle_03")
FTB_ROW("idle", 558.489, "s2_idle_04")
FTB_ROW("idle", 698.111, "s2_idle_05")
FTB_ROW("idle", 837.733, "s2_idle_06")
FTB_ROW("idle", 977.356, "s2_idle_07")
FTB_ROW("idle", 1116.978, "s2_idle_08")

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 34.965, "s2_walk_01")
FTB_ROW("walk", 69.93, "s2_walk_02")
FTB_ROW("walk", 104.895, "s2_walk_21")
FTB_ROW("walk", 139.86, "s2_walk_18")
FTB_ROW("walk", 174.825, "s2_walk_22")
FTB_ROW("walk", 209.79, "s2_walk_23")
FTB_ROW("walk", 244.755, "s2_walk_19")
FTB_ROW("walk", 279.72, "s2_walk_20")
FTB_ROW("walk", 314.685, "s2_walk_24")
FTB_ROW("walk", 349.65, "s2_walk_06")
FTB_ROW("walk", 384.615, "s2_walk_07")
FTB_ROW("walk", 419.58, "s2_walk_08")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 484.848, "s2_walk_09")
FTB_ROW("walk", 515.151, "s2_walk_25")
FTB_ROW("walk", 545.454, "s2_walk_10")
FTB_ROW("walk", 575.757, "s2_walk_29")
FTB_ROW("walk", 606.06, "s2_walk_11")
FTB_ROW("walk", 636.363, "s2_walk_30")
FTB_ROW("walk", 666.666, "s2_walk_27")
FTB_ROW("walk", 696.97, "s2_walk_12")
FTB_ROW("walk", 727.273, "s2_walk_31")
FTB_ROW("walk", 757.576, "s2_walk_13")
FTB_ROW("walk", 787.879, "s2_walk_14")
FTB_ROW("walk", 818.182, "s2_walk_32")
FTB_ROW("walk", 848.485, "s2_walk_15")
FTB_ROW("walk", 878.788, "s2_walk_16")

FTB_STATE("wind", Time, 320)
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 66.667, "s2_wind_02")
FTB_ROW("wind", 100, "s2_wind_03")
FTB_ROW("wind", 133.333, "s2_wind_04")
FTB_ROW("wind", 166.667, "s2_wind_03")
FTB_ROW("wind", 200, "s2_wind_04")
FTB_ROW("wind", 233.333, "s2_wind_05")
FTB_ROW("wind", 266.667, "s2_wind_06")
FTB_ROW("wind", 283.333, "s2_wind_07")
FTB_ROW("wind", 300, "s2_wind_08")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 66.667, "s2_swing_02")
FTB_ROW("swing", 100, "s2_swing_03")
FTB_ROW("swing", 116.667, "s2_swing_04")
FTB_ROW("swing", 133.333, "s2_swing_05")
FTB_ROW("swing", 150, "s2_swing_06")
FTB_ROW("swing", 183.333, "s2_swing_07")
FTB_ROW("swing", 200, "s2_swing_08")

FTB_STATE("grab", Time, 450)
FTB_ROW("grab", 0, "grab")   // KEY
FTB_ROW("grab", 33.333, "s2_grab_01")
FTB_ROW("grab", 66.667, "s2_grab_02")
FTB_ROW("grab", 100, "s2_grab_03")
FTB_ROW("grab", 133.333, "s2_grab_04")
FTB_ROW("grab", 166.667, "s2_grab_03")
FTB_ROW("grab", 200, "s2_grab_05")
FTB_ROW("grab", 233.333, "s2_grab_06")
FTB_ROW("grab", 266.667, "s2_grab_07")
FTB_ROW("grab", 300, "s2_grab_08")
FTB_ROW("grab", 333.333, "s2_grab_05")
FTB_ROW("grab", 366.667, "s2_grab_06")
FTB_ROW("grab", 400, "s2_grab_07")
FTB_ROW("grab", 433.333, "s2_grab_08")

FTB_STATE("knee", Time, 420)
FTB_ROW("knee", 0, "grab")   // KEY
FTB_ROW("knee", 33.333, "s2_knee_01")
FTB_ROW("knee", 66.667, "s2_knee_02")
FTB_ROW("knee", 100, "s2_knee_03")
FTB_ROW("knee", 133.333, "s2_knee_04")
FTB_ROW("knee", 166.667, "s2_knee_05")
FTB_ROW("knee", 200, "s2_knee_06")
FTB_ROW("knee", 233.333, "s2_knee_07")
FTB_ROW("knee", 266.667, "s2_knee_08")
FTB_ROW("knee", 300, "s2_grab_05")
FTB_ROW("knee", 333.333, "s2_grab_06")
FTB_ROW("knee", 366.667, "s2_grab_07")
FTB_ROW("knee", 400, "s2_grab_08")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 66.667, "s2_hurt_03")
FTB_ROW("hurt", 100, "s2_hurt_04")
FTB_ROW("hurt", 133.333, "s2_hurt_05")
FTB_ROW("hurt", 166.667, "s2_hurt_06")
FTB_ROW("hurt", 200, "s2_hurt_07")
FTB_ROW("hurt", 233.333, "s2_hurt_08")

FTB_STATE("fly", Time, 453.3)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_fly_01")
FTB_ROW("fly", 66.667, "s2_fly_03")
FTB_ROW("fly", 100, "s2_fly_04")
FTB_ROW("fly", 133.333, "s2_fly_06")
FTB_ROW("fly", 166.667, "s2_fly_05")
FTB_ROW("fly", 200, "s2_fly_01")
FTB_ROW("fly", 233.333, "s2_fly_03")
FTB_ROW("fly", 266.667, "s2_fly_04")
FTB_ROW("fly", 300, "s2_fly_06")
FTB_ROW("fly", 333.333, "s2_fly_05")
FTB_ROW("fly", 366.667, "s2_fly_01")
FTB_ROW("fly", 400, "s2_fly_08")

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
FTB_ROW("getup", 383.333, "s2_getup_11")
FTB_ROW("getup", 416.667, "s2_getup_12")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
