// Exposure chart of Sleaze. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.

FTB_STATE("turn>walk", Age, 65)   // window: the first 65 ms
FTB_ROW("turn>walk", 0, "sturn")   // KEY
FTB_ROW("turn>walk", 33.333, "s2_walk_16")

FTB_STATE("idle>walk", Age, 70)   // window: the first 70 ms
FTB_ROW("idle>walk", 0, "sturn")   // KEY
FTB_ROW("idle>walk", 33.333, "s2_walk_16")

FTB_STATE("walk>idle", Age, 90)   // window: the first 90 ms
FTB_ROW("walk>idle", 0, "sstop")   // KEY
FTB_ROW("walk>idle", 33.333, "s2_walk_10")
FTB_ROW("walk>idle", 66.667, "s2_walk_09")

FTB_STATE("idle", Loop, 1400)   // one cycle
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 116.667, "s2_idle_08")
FTB_ROW("idle", 233.333, "s2_idle_06")
FTB_ROW("idle", 350, "sidle01")   // KEY
FTB_ROW("idle", 466.667, "s2_idle_02")
FTB_ROW("idle", 583.333, "s2_idle_01")
FTB_ROW("idle", 700, "sidle02")   // KEY
FTB_ROW("idle", 816.667, "s2_idle_05")
FTB_ROW("idle", 933.333, "s2_idle_07")
FTB_ROW("idle", 1050, "sidle01")   // KEY
FTB_ROW("idle", 1166.667, "s2_idle_03")
FTB_ROW("idle", 1283.333, "s2_idle_05")

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 87.879, "s2_walk_01")
FTB_ROW("walk", 119.697, "s2_walk_10")
FTB_ROW("walk", 151.515, "swalk01")   // KEY
FTB_ROW("walk", 189.394, "s2_walk_05")
FTB_ROW("walk", 227.273, "s2_walk_13")
FTB_ROW("walk", 265.152, "s2_walk_06")
FTB_ROW("walk", 303.03, "swalk02")   // KEY
FTB_ROW("walk", 340.909, "s2_walk_07")
FTB_ROW("walk", 378.788, "s2_walk_08")
FTB_ROW("walk", 416.667, "s2_walk_16")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 492.424, "s2_walk_09")
FTB_ROW("walk", 530.303, "s2_walk_01")
FTB_ROW("walk", 568.182, "s2_walk_10")
FTB_ROW("walk", 606.061, "swalk03")   // KEY
FTB_ROW("walk", 643.939, "s2_walk_05")
FTB_ROW("walk", 681.818, "s2_walk_13")
FTB_ROW("walk", 719.697, "s2_walk_14")
FTB_ROW("walk", 757.576, "swalk04")   // KEY
FTB_ROW("walk", 795.455, "s2_walk_15")
FTB_ROW("walk", 833.333, "s2_walk_08")
FTB_ROW("walk", 871.212, "s2_walk_16")

FTB_STATE("wind", Time, 300)
FTB_ROW("wind", 0, "swind01")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_02")
FTB_ROW("wind", 70, "swind02")   // KEY
FTB_ROW("wind", 100, "s2_wind_04")
FTB_ROW("wind", 133.333, "s2_wind_06")
FTB_ROW("wind", 160, "wind")   // KEY
FTB_ROW("wind", 200, "s2_wind_05")
FTB_ROW("wind", 233.333, "s2_wind_07")
FTB_ROW("wind", 266.667, "s2_wind_08")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 70, "sattack01")   // KEY
FTB_ROW("swing", 100, "s2_swing_02")
FTB_ROW("swing", 140, "srecover01")   // KEY
FTB_ROW("swing", 166.667, "s2_swing_04")
FTB_ROW("swing", 200, "idle")

FTB_STATE("throw", Time, 750)
FTB_ROW("throw", 0, "swind01")   // KEY
FTB_ROW("throw", 33.333, "s2_wind_02")
FTB_ROW("throw", 66.667, "s2_wind_03")
FTB_ROW("throw", 100, "swind02")   // KEY
FTB_ROW("throw", 133.333, "s2_wind_04")
FTB_ROW("throw", 166.667, "s2_wind_06")
FTB_ROW("throw", 200, "wind")
FTB_ROW("throw", 233.333, "s2_wind_05")
FTB_ROW("throw", 266.667, "s2_wind_07")
FTB_ROW("throw", 300, "s2_wind_08")
FTB_ROW("throw", 320, "throw")   // KEY
FTB_ROW("throw", 353.333, "s2_throw_01")
FTB_ROW("throw", 386.667, "s2_throw_02")
FTB_ROW("throw", 420, "sattack01")   // KEY
FTB_ROW("throw", 450, "s2_throw_03")
FTB_ROW("throw", 483.333, "s2_throw_04")
FTB_ROW("throw", 516.667, "s2_throw_05")
FTB_ROW("throw", 540, "srecover01")   // KEY
FTB_ROW("throw", 573.333, "s2_throw_06")
FTB_ROW("throw", 606.667, "s2_throw_07")
FTB_ROW("throw", 640, "s2_throw_08")
FTB_ROW("throw", 673.333, "s2_throw_09")
FTB_ROW("throw", 733.333, "s2_throw_11")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 60, "shit01")   // KEY
FTB_ROW("hurt", 100, "s2_hurt_02")
FTB_ROW("hurt", 120, "shit02")   // KEY
FTB_ROW("hurt", 150, "s2_hurt_03")
FTB_ROW("hurt", 183.333, "s2_hurt_05")
FTB_ROW("hurt", 210, "srecover01")   // KEY
FTB_ROW("hurt", 233.333, "s2_swing_04")

FTB_STATE("fly", Time, 453.3)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_hurt_01")
FTB_ROW("fly", 70, "shit01")   // KEY
FTB_ROW("fly", 100, "s2_fly_01")
FTB_ROW("fly", 133.333, "s2_fly_02")
FTB_ROW("fly", 166.667, "s2_fly_03")
FTB_ROW("fly", 200, "s2_fly_02")
FTB_ROW("fly", 220, "hurtrecover")   // KEY
FTB_ROW("fly", 253.333, "s2_fly_03")
FTB_ROW("fly", 286.667, "s2_fly_01")
FTB_ROW("fly", 320, "s2_fly_02")
FTB_ROW("fly", 353.333, "s2_fly_03")
FTB_ROW("fly", 386.667, "s2_fly_01")
FTB_ROW("fly", 420, "s2_fly_02")

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "sgetup01")   // KEY
FTB_ROW("getup", 33.333, "s2_getup_02")
FTB_ROW("getup", 66.667, "s2_getup_03")
FTB_ROW("getup", 100, "s2_getup_04")
FTB_ROW("getup", 140, "down")   // KEY
FTB_ROW("getup", 166.667, "s2_getup_06")
FTB_ROW("getup", 183.333, "s2_getup_07")
FTB_ROW("getup", 200, "s2_getup_08")
FTB_ROW("getup", 230, "sgetup02")   // KEY
FTB_ROW("getup", 266.667, "s2_getup_01")
FTB_ROW("getup", 300, "s2_hurt_05")
FTB_ROW("getup", 333.333, "s2_swing_04")
FTB_ROW("getup", 360, "sidle01")   // KEY
FTB_ROW("getup", 400, "s2_idle_01")
FTB_ROW("getup", 433.333, "idle")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
