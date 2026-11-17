// Exposure chart of Pitcook. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// Stage 2 art: 57 new drawings (s2_*), made from the 8 keys with the image tool; see art/Pitcook/manifest.json.

FTB_STATE("idle", Loop, 1256.6)   // one cycle
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 250, "s2_idle_05")
FTB_ROW("idle", 366.667, "s2_idle_01")
FTB_ROW("idle", 483.333, "s2_idle_02")
FTB_ROW("idle", 766.667, "s2_idle_04")
FTB_ROW("idle", 916.667, "s2_idle_01")
FTB_ROW("idle", 1033.333, "s2_idle_05")

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 28.409, "s2_walk_09")
FTB_ROW("walk", 56.818, "s2_walk_02")
FTB_ROW("walk", 85.227, "s2_walk_10")
FTB_ROW("walk", 113.636, "s2_walk_03")
FTB_ROW("walk", 142.045, "s2_walk_11")
FTB_ROW("walk", 170.455, "s2_walk_04")
FTB_ROW("walk", 198.864, "s2_walk_12")
FTB_ROW("walk", 227.273, "s2_walk_05")
FTB_ROW("walk", 255.682, "s2_walk_13")
FTB_ROW("walk", 284.091, "s2_walk_06")
FTB_ROW("walk", 312.5, "s2_walk_14")
FTB_ROW("walk", 340.909, "s2_walk_07")
FTB_ROW("walk", 369.318, "s2_walk_15")
FTB_ROW("walk", 397.727, "s2_walk_08")
FTB_ROW("walk", 426.136, "s2_walk_16")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 482.954, "s2_walk_09")
FTB_ROW("walk", 511.363, "s2_walk_02")
FTB_ROW("walk", 539.772, "s2_walk_10")
FTB_ROW("walk", 568.181, "s2_walk_03")
FTB_ROW("walk", 596.59, "s2_walk_11")
FTB_ROW("walk", 625, "s2_walk_04")
FTB_ROW("walk", 653.409, "s2_walk_12")
FTB_ROW("walk", 681.818, "s2_walk_05")
FTB_ROW("walk", 710.227, "s2_walk_13")
FTB_ROW("walk", 738.636, "s2_walk_06")
FTB_ROW("walk", 767.045, "s2_walk_14")
FTB_ROW("walk", 795.454, "s2_walk_07")
FTB_ROW("walk", 823.863, "s2_walk_15")
FTB_ROW("walk", 852.272, "s2_walk_08")
FTB_ROW("walk", 880.681, "s2_walk_16")

FTB_STATE("wind", Time, 550)
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 133.333, "s2_wind_03")
FTB_ROW("wind", 183.333, "s2_wind_04")
FTB_ROW("wind", 250, "s2_wind_03")
FTB_ROW("wind", 300, "s2_wind_04")
FTB_ROW("wind", 366.667, "s2_wind_05")
FTB_ROW("wind", 466.667, "s2_wind_06")
FTB_ROW("wind", 500, "s2_wind_07")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 50, "s2_swing_01")
FTB_ROW("swing", 83.333, "s2_swing_02")
FTB_ROW("swing", 116.667, "s2_swing_03")
FTB_ROW("swing", 183.333, "s2_swing_06")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 66.667, "s2_hurt_07")
FTB_ROW("hurt", 100, "s2_hurt_02")
FTB_ROW("hurt", 133.333, "s2_hurt_03")
FTB_ROW("hurt", 166.667, "s2_hurt_08")
FTB_ROW("hurt", 200, "s2_hurt_04")
FTB_ROW("hurt", 233.333, "s2_hurt_05")

FTB_STATE("fly", Time, 453.3)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_hurt_01")
FTB_ROW("fly", 66.667, "s2_fly_03")
FTB_ROW("fly", 116.667, "s2_fly_01")
FTB_ROW("fly", 183.333, "s2_fly_02")
FTB_ROW("fly", 250, "s2_fly_01")
FTB_ROW("fly", 316.667, "s2_fly_02")
FTB_ROW("fly", 383.333, "s2_fly_01")

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY
FTB_ROW("down", 33.333, "s2_down_04")
FTB_ROW("down", 100, "down")
FTB_ROW("down", 616.667, "s2_down_05")
FTB_ROW("down", 683.333, "s2_down_06")

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "getup")   // KEY
FTB_ROW("getup", 33.333, "s2_getup_01")
FTB_ROW("getup", 66.667, "s2_getup_10")
FTB_ROW("getup", 100, "s2_getup_02")
FTB_ROW("getup", 133.333, "s2_getup_03")
FTB_ROW("getup", 166.667, "s2_getup_11")
FTB_ROW("getup", 200, "s2_getup_04")
FTB_ROW("getup", 233.333, "s2_getup_12")
FTB_ROW("getup", 266.667, "s2_getup_05")
FTB_ROW("getup", 300, "s2_getup_06")
FTB_ROW("getup", 333.333, "s2_swing_04")
FTB_ROW("getup", 366.667, "s2_getup_07")
FTB_ROW("getup", 400, "s2_getup_08")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY
FTB_ROW("dead", 33.333, "s2_down_04")
FTB_ROW("dead", 100, "down")

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
