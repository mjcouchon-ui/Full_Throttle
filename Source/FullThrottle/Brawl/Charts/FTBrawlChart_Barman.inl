// Exposure chart of Barman. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// Stage 2 art: every key of the base chart is here at its time; rows without KEY are new (frames s2_*) or a second showing of a key.

FTB_STATE("idle", Loop, 1256.637)   // one breath (the code squash sin(5t) has this period)
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 209.439, "s2_idle_01")
FTB_ROW("idle", 418.879, "s2_idle_02")
FTB_ROW("idle", 628.318, "s2_idle_03")
FTB_ROW("idle", 837.758, "s2_idle_04")
FTB_ROW("idle", 1047.197, "s2_idle_05")

FTB_STATE("walk", Walk, 909.091)   // one cycle = two steps; both keys are the same contact pose, one step lies between them
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 45.455, "s2_walk_01")
FTB_ROW("walk", 90.909, "s2_walk_02")
FTB_ROW("walk", 136.364, "s2_walk_03")
FTB_ROW("walk", 181.818, "s2_walk_04")
FTB_ROW("walk", 227.273, "s2_walk_05")
FTB_ROW("walk", 272.727, "s2_walk_06")
FTB_ROW("walk", 318.182, "s2_walk_07")
FTB_ROW("walk", 363.636, "s2_walk_08")
FTB_ROW("walk", 409.091, "s2_walk_09")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 500, "s2_walk_01")
FTB_ROW("walk", 545.454, "s2_walk_02")
FTB_ROW("walk", 590.909, "s2_walk_03")
FTB_ROW("walk", 636.363, "s2_walk_04")
FTB_ROW("walk", 681.818, "s2_walk_05")
FTB_ROW("walk", 727.272, "s2_walk_06")
FTB_ROW("walk", 772.727, "s2_walk_07")
FTB_ROW("walk", 818.181, "s2_walk_08")
FTB_ROW("walk", 863.636, "s2_walk_09")

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
FTB_ROW("swing", 66.667, "s2_swing_02")
FTB_ROW("swing", 100, "s2_swing_03")
FTB_ROW("swing", 133.333, "s2_swing_04")
FTB_ROW("swing", 166.667, "s2_swing_05")
FTB_ROW("swing", 200, "s2_swing_06")

FTB_STATE("throw", Time, 750)   // the bottle leaves at 320 ms: the key is shown again from 316.667
FTB_ROW("throw", 0, "throw")   // KEY
FTB_ROW("throw", 16.667, "s2_throw_01")
FTB_ROW("throw", 66.667, "s2_throw_02")
FTB_ROW("throw", 100, "s2_throw_03")
FTB_ROW("throw", 133.333, "s2_throw_04")
FTB_ROW("throw", 150, "s2_throw_05")
FTB_ROW("throw", 166.667, "s2_throw_06")
FTB_ROW("throw", 200, "s2_throw_07")
FTB_ROW("throw", 233.333, "s2_throw_08")
FTB_ROW("throw", 266.667, "s2_throw_09")
FTB_ROW("throw", 283.333, "s2_throw_10")
FTB_ROW("throw", 300, "s2_throw_11")
FTB_ROW("throw", 316.667, "s2_throw_rel")
FTB_ROW("throw", 350, "s2_throw_12")
FTB_ROW("throw", 383.333, "s2_throw_13")
FTB_ROW("throw", 416.667, "s2_throw_14")
FTB_ROW("throw", 450, "s2_swing_05")
FTB_ROW("throw", 483.333, "s2_swing_06")
FTB_ROW("throw", 516.667, "s2_swing_08")
FTB_ROW("throw", 550, "s2_swing_07")
FTB_ROW("throw", 583.333, "s2_idle_05")
FTB_ROW("throw", 616.667, "idle")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 66.667, "s2_hurt_02")
FTB_ROW("hurt", 100, "s2_hurt_03")
FTB_ROW("hurt", 133.333, "s2_hurt_04")
FTB_ROW("hurt", 166.667, "s2_hurt_05")
FTB_ROW("hurt", 200, "s2_hurt_06")
FTB_ROW("hurt", 233.333, "s2_hurt_07")

FTB_STATE("fly", Time, 453.3)   // the game also tips the drawing back up to 70 degrees
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_hurt_01")
FTB_ROW("fly", 66.667, "s2_hurt_02")
FTB_ROW("fly", 100, "s2_fly_01")
FTB_ROW("fly", 133.333, "s2_fly_02")
FTB_ROW("fly", 166.667, "s2_fly_03")
FTB_ROW("fly", 200, "s2_fly_02")
FTB_ROW("fly", 233.333, "s2_fly_01")
FTB_ROW("fly", 266.667, "s2_fly_02")
FTB_ROW("fly", 300, "s2_fly_03")
FTB_ROW("fly", 333.333, "s2_fly_02")
FTB_ROW("fly", 366.667, "s2_fly_01")
FTB_ROW("fly", 400, "s2_fly_02")
FTB_ROW("fly", 433.333, "s2_fly_03")

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "down")   // KEY
FTB_ROW("getup", 16.667, "s2_getup_01")
FTB_ROW("getup", 50, "s2_getup_02")
FTB_ROW("getup", 83.333, "s2_getup_03")
FTB_ROW("getup", 116.667, "s2_getup_04")
FTB_ROW("getup", 150, "s2_getup_05")
FTB_ROW("getup", 183.333, "s2_getup_06")
FTB_ROW("getup", 216.667, "s2_getup_07")
FTB_ROW("getup", 250, "s2_getup_08")
FTB_ROW("getup", 283.333, "s2_getup_09")
FTB_ROW("getup", 316.667, "s2_getup_10")
FTB_ROW("getup", 350, "s2_getup_11")
FTB_ROW("getup", 383.333, "s2_getup_12")
FTB_ROW("getup", 416.667, "s2_getup_13")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
