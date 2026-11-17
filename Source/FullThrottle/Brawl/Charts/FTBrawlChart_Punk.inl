// Exposure chart of Punk. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// S2: rows without KEY are new drawings (frames T_punk_s2_*), see manifest.json.

FTB_STATE("turn>walk", Age, 65)   // window: the first 65 ms
FTB_ROW("turn>walk", 0, "sturn")   // KEY
FTB_ROW("turn>walk", 33.333, "s2_turn_01")

FTB_STATE("idle>walk", Age, 70)   // window: the first 70 ms
FTB_ROW("idle>walk", 0, "sturn")   // KEY
FTB_ROW("idle>walk", 33.333, "s2_start_01")

FTB_STATE("walk>idle", Age, 90)   // window: the first 90 ms
FTB_ROW("walk>idle", 0, "sstop")   // KEY
FTB_ROW("walk>idle", 33.333, "s2_stop_01")
FTB_ROW("walk>idle", 66.667, "s2_stop_02")

FTB_STATE("idle", Loop, 1400)   // one cycle
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 116.667, "s2_idle_01")
FTB_ROW("idle", 233.333, "s2_idle_02")
FTB_ROW("idle", 350, "sidle01")   // KEY
FTB_ROW("idle", 466.667, "s2_idle_04")
FTB_ROW("idle", 583.333, "s2_idle_05")
FTB_ROW("idle", 700, "sidle02")   // KEY
FTB_ROW("idle", 816.667, "s2_idle_05")
FTB_ROW("idle", 933.333, "s2_idle_04")
FTB_ROW("idle", 1050, "sidle01")   // KEY
FTB_ROW("idle", 1166.667, "s2_idle_02")
FTB_ROW("idle", 1283.333, "s2_idle_06")

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 87.5, "s2_walk_01")
FTB_ROW("walk", 108.8, "s2_walk_02")
FTB_ROW("walk", 130.1, "s2_walk_17")
FTB_ROW("walk", 151.515, "swalk01")   // KEY
FTB_ROW("walk", 189.394, "s2_walk_03")
FTB_ROW("walk", 227.273, "s2_walk_04")
FTB_ROW("walk", 265.152, "s2_walk_10")
FTB_ROW("walk", 303.03, "swalk02")   // KEY
FTB_ROW("walk", 333.333, "s2_walk_05")
FTB_ROW("walk", 363.636, "s2_walk_06")
FTB_ROW("walk", 393.939, "s2_walk_07")
FTB_ROW("walk", 424.242, "s2_walk_08")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 492.424, "s2_walk_14")
FTB_ROW("walk", 530.303, "s2_walk_02")
FTB_ROW("walk", 568.182, "s2_walk_17")
FTB_ROW("walk", 606.061, "swalk03")   // KEY
FTB_ROW("walk", 643.939, "s2_walk_11")
FTB_ROW("walk", 681.818, "s2_walk_12")
FTB_ROW("walk", 719.697, "s2_walk_13")
FTB_ROW("walk", 757.576, "swalk04")   // KEY
FTB_ROW("walk", 795.455, "s2_walk_15")
FTB_ROW("walk", 833.333, "s2_walk_16")
FTB_ROW("walk", 871.212, "s2_walk_08")

FTB_STATE("wind", Time, 280)
FTB_ROW("wind", 0, "swind01")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 70, "swind02")   // KEY
FTB_ROW("wind", 100, "s2_wind_02")
FTB_ROW("wind", 133.333, "s2_wind_03")
FTB_ROW("wind", 160, "wind")   // KEY
FTB_ROW("wind", 183.333, "s2_wind_04")
FTB_ROW("wind", 250, "s2_wind_06")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_04")
FTB_ROW("swing", 70, "sattack01")   // KEY
FTB_ROW("swing", 100, "s2_swing_01")
FTB_ROW("swing", 116.667, "s2_swing_05")
FTB_ROW("swing", 140, "srecover01")   // KEY
FTB_ROW("swing", 166.667, "s2_swing_02")
FTB_ROW("swing", 200, "s2_swing_03")

FTB_STATE("dash", Time, 750)
FTB_ROW("dash", 0, "walk1")   // KEY
FTB_ROW("dash", 33.333, "s2_walk_02")
FTB_ROW("dash", 56, "swalk01")   // KEY
FTB_ROW("dash", 83.333, "s2_walk_04")
FTB_ROW("dash", 112, "swalk02")   // KEY
FTB_ROW("dash", 133.333, "s2_walk_06")
FTB_ROW("dash", 167, "walk2")   // KEY
FTB_ROW("dash", 200, "s2_walk_14")
FTB_ROW("dash", 223, "swalk03")   // KEY
FTB_ROW("dash", 250, "s2_walk_12")
FTB_ROW("dash", 278, "swalk04")   // KEY
FTB_ROW("dash", 300, "s2_walk_16")
FTB_ROW("dash", 334, "walk1")   // KEY
FTB_ROW("dash", 366.667, "s2_walk_02")
FTB_ROW("dash", 389, "swalk01")   // KEY
FTB_ROW("dash", 416.667, "s2_walk_04")
FTB_ROW("dash", 445, "swalk02")   // KEY
FTB_ROW("dash", 466.667, "s2_walk_06")
FTB_ROW("dash", 500, "walk2")   // KEY
FTB_ROW("dash", 533.333, "s2_walk_14")
FTB_ROW("dash", 556, "swalk03")   // KEY
FTB_ROW("dash", 583.333, "s2_walk_12")
FTB_ROW("dash", 612, "swalk04")   // KEY
FTB_ROW("dash", 633.333, "s2_walk_16")
FTB_ROW("dash", 667, "walk1")   // KEY
FTB_ROW("dash", 700, "s2_walk_02")
FTB_ROW("dash", 723, "swalk01")   // KEY
FTB_ROW("dash", 778, "swalk02")   // KEY

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 60, "shit01")   // KEY
FTB_ROW("hurt", 93.333, "s2_hurt_02")
FTB_ROW("hurt", 120, "shit02")   // KEY
FTB_ROW("hurt", 150, "s2_hurt_03")
FTB_ROW("hurt", 183.333, "s2_hurt_04")
FTB_ROW("hurt", 210, "srecover01")   // KEY
FTB_ROW("hurt", 233.333, "s2_hurt_05")

FTB_STATE("fly", Time, 453.3)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_hurt_01")
FTB_ROW("fly", 70, "shit01")   // KEY
FTB_ROW("fly", 100, "s2_fly_01")
FTB_ROW("fly", 133.333, "s2_fly_05")
FTB_ROW("fly", 166.667, "s2_fly_06")
FTB_ROW("fly", 200, "s2_fly_03")
FTB_ROW("fly", 220, "hurtrecover")   // KEY
FTB_ROW("fly", 250, "s2_fly_07")
FTB_ROW("fly", 283.333, "s2_fly_10")
FTB_ROW("fly", 316.667, "s2_fly_11")
FTB_ROW("fly", 350, "s2_fly_08")
FTB_ROW("fly", 383.333, "s2_fly_09")
FTB_ROW("fly", 416.667, "s2_fly_02")

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "sgetup01")   // KEY
FTB_ROW("getup", 33.333, "s2_getup_06")
FTB_ROW("getup", 66.667, "s2_getup_07")
FTB_ROW("getup", 100, "s2_getup_02")
FTB_ROW("getup", 140, "down")   // KEY
FTB_ROW("getup", 166.667, "s2_getup_02")
FTB_ROW("getup", 183.333, "s2_getup_03")
FTB_ROW("getup", 200, "s2_getup_04")
FTB_ROW("getup", 230, "sgetup02")   // KEY
FTB_ROW("getup", 266.667, "s2_getup_08")
FTB_ROW("getup", 300, "s2_getup_09")
FTB_ROW("getup", 333.333, "s2_getup_05")
FTB_ROW("getup", 360, "sidle01")   // KEY
FTB_ROW("getup", 400, "s2_getup_11")
FTB_ROW("getup", 433.333, "s2_idle_06")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
