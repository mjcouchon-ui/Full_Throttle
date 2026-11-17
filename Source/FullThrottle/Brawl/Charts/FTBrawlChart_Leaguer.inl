// Exposure chart of Leaguer. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// S2: rows without KEY are new drawings (frames in art\Leaguer\frames) or existing drawings shown again.
// idle: the base chart said Time; it is Loop here (same 1256.6 ms period) so that the breath repeats.

FTB_STATE("idle", Loop, 1256.6)   // one cycle
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 209.433, "s2_idle_01")
FTB_ROW("idle", 418.867, "s2_idle_02")
FTB_ROW("idle", 628.3, "s2_idle_01")
FTB_ROW("idle", 837.733, "idle")
FTB_ROW("idle", 1047.167, "s2_idle_03")

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 34.965, "s2_walk_01")
FTB_ROW("walk", 69.93, "s2_walk_02")
FTB_ROW("walk", 104.895, "s2_walk_03")
FTB_ROW("walk", 139.86, "s2_walk_04")
FTB_ROW("walk", 174.825, "s2_walk_05")
FTB_ROW("walk", 209.79, "s2_walk_06")
FTB_ROW("walk", 244.755, "s2_walk_07")
FTB_ROW("walk", 279.72, "s2_walk_08")
FTB_ROW("walk", 314.685, "s2_walk_09")
FTB_ROW("walk", 349.65, "s2_walk_10")
FTB_ROW("walk", 384.615, "s2_walk_11")
FTB_ROW("walk", 419.58, "s2_walk_12")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 489.51, "s2_walk_01")
FTB_ROW("walk", 524.475, "s2_walk_02")
FTB_ROW("walk", 559.44, "s2_walk_03")
FTB_ROW("walk", 594.405, "s2_walk_04")
FTB_ROW("walk", 629.37, "s2_walk_05")
FTB_ROW("walk", 664.335, "s2_walk_06")
FTB_ROW("walk", 699.3, "s2_walk_07")
FTB_ROW("walk", 734.265, "s2_walk_08")
FTB_ROW("walk", 769.23, "s2_walk_09")
FTB_ROW("walk", 804.195, "s2_walk_10")
FTB_ROW("walk", 839.16, "s2_walk_11")
FTB_ROW("walk", 874.125, "s2_walk_12")

FTB_STATE("wind", Time, 320)
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 66.667, "s2_wind_02")
FTB_ROW("wind", 116.667, "s2_wind_03")
FTB_ROW("wind", 183.333, "s2_wind_04")
FTB_ROW("wind", 250, "s2_wind_05")

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
FTB_ROW("grab", 100, "s2_grab_03")
FTB_ROW("grab", 133.333, "s2_grab_02")
FTB_ROW("grab", 166.667, "s2_grab_04")
FTB_ROW("grab", 200, "s2_grab_05")
FTB_ROW("grab", 233.333, "s2_grab_04")
FTB_ROW("grab", 266.667, "s2_grab_02")
FTB_ROW("grab", 300, "s2_grab_03")
FTB_ROW("grab", 333.333, "s2_grab_02")
FTB_ROW("grab", 366.667, "s2_grab_04")
FTB_ROW("grab", 400, "s2_grab_05")
FTB_ROW("grab", 433.333, "s2_grab_04")

FTB_STATE("knee", Time, 420)
FTB_ROW("knee", 0, "grab")   // KEY
FTB_ROW("knee", 33.333, "s2_knee_01")
FTB_ROW("knee", 66.667, "s2_knee_02")
FTB_ROW("knee", 100, "s2_knee_03")
FTB_ROW("knee", 116.667, "s2_knee_04")
FTB_ROW("knee", 166.667, "s2_knee_03")
FTB_ROW("knee", 200, "s2_knee_05")
FTB_ROW("knee", 233.333, "s2_knee_06")
FTB_ROW("knee", 266.667, "s2_knee_07")
FTB_ROW("knee", 300, "s2_grab_02")
FTB_ROW("knee", 333.333, "s2_grab_03")
FTB_ROW("knee", 366.667, "s2_grab_02")
FTB_ROW("knee", 400, "s2_grab_04")

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
FTB_ROW("fly", 33.333, "s2_hurt_01")
FTB_ROW("fly", 66.667, "s2_fly_01")
FTB_ROW("fly", 133.333, "s2_fly_02")
FTB_ROW("fly", 216.667, "s2_fly_03")
FTB_ROW("fly", 316.667, "s2_fly_04")

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
FTB_ROW("getup", 300, "s2_hurt_08")
FTB_ROW("getup", 333.333, "s2_idle_03")
FTB_ROW("getup", 366.667, "s2_idle_01")
FTB_ROW("getup", 400, "idle")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
