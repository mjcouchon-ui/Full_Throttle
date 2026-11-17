// Exposure chart of Dock. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// Stage 2 art: every key of the base chart is here at its time; rows without KEY are new (frames s2_*) or a second showing of a key.

FTB_STATE("idle", Loop, 1256.637)   // one breath (the code squash sin(5t) has this period); a slow hold, 5 drawings
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 251.327, "s2_idle_01")
FTB_ROW("idle", 502.655, "s2_idle_02")
FTB_ROW("idle", 753.982, "s2_idle_03")
FTB_ROW("idle", 1005.31, "s2_idle_04")

FTB_STATE("walk", Walk, 909.091)   // one cycle = two steps; the keys are the two contacts (arms opposite), 10 drawings between each pair, 41.3 ms apiece
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 41.322, "s2_walk_01")
FTB_ROW("walk", 82.645, "s2_walk_02")
FTB_ROW("walk", 123.967, "s2_walk_03")
FTB_ROW("walk", 165.289, "s2_walk_04")
FTB_ROW("walk", 206.611, "s2_walk_05")
FTB_ROW("walk", 247.934, "s2_walk_06")
FTB_ROW("walk", 289.256, "s2_walk_07")
FTB_ROW("walk", 330.578, "s2_walk_08")
FTB_ROW("walk", 371.9, "s2_walk_09")
FTB_ROW("walk", 413.223, "s2_walk_10")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 495.867, "s2_walk_11")
FTB_ROW("walk", 537.19, "s2_walk_12")
FTB_ROW("walk", 578.512, "s2_walk_13")
FTB_ROW("walk", 619.834, "s2_walk_14")
FTB_ROW("walk", 661.156, "s2_walk_15")
FTB_ROW("walk", 702.479, "s2_walk_16")
FTB_ROW("walk", 743.801, "s2_walk_17")
FTB_ROW("walk", 785.123, "s2_walk_18")
FTB_ROW("walk", 826.445, "s2_walk_19")
FTB_ROW("walk", 867.768, "s2_walk_20")

FTB_STATE("wind", Time, 280)   // the key at 0, the coil deepens (deepest at 67 ms), then the raised fist drops and the front fist cocks; one step on the fastest drawing
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 66.667, "s2_wind_02")
FTB_ROW("wind", 100, "s2_wind_03")
FTB_ROW("wind", 133.333, "s2_wind_04")
FTB_ROW("wind", 166.667, "s2_wind_05")
FTB_ROW("wind", 200, "s2_wind_06")
FTB_ROW("wind", 233.333, "s2_wind_07")
FTB_ROW("wind", 250, "s2_wind_08")

FTB_STATE("swing", Time, 220)   // the hit is tested at 0 on the key; drive, follow-through, pull back, settle
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 66.667, "s2_swing_02")
FTB_ROW("swing", 100, "s2_swing_03")
FTB_ROW("swing", 133.333, "s2_swing_04")
FTB_ROW("swing", 166.667, "s2_swing_05")
FTB_ROW("swing", 200, "s2_swing_06")

FTB_STATE("hurt", Time, 260)   // the key at 0 has the cap knocked into the air: it rises, drops and lands back on the head at 133 ms
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 66.667, "s2_hurt_02")
FTB_ROW("hurt", 100, "s2_hurt_03")
FTB_ROW("hurt", 133.333, "s2_hurt_04")
FTB_ROW("hurt", 166.667, "s2_hurt_05")
FTB_ROW("hurt", 200, "s2_hurt_06")
FTB_ROW("hurt", 233.333, "s2_hurt_07")

FTB_STATE("fly", Time, 453.3)   // the game also tips the drawing back up to 70 degrees; upright drawings only; the cap stays in the air until he lands
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_hurt_01")
FTB_ROW("fly", 66.667, "s2_hurt_02")
FTB_ROW("fly", 100, "s2_fly_03")
FTB_ROW("fly", 133.333, "s2_fly_04")
FTB_ROW("fly", 166.667, "s2_fly_03")
FTB_ROW("fly", 200, "s2_fly_04")
FTB_ROW("fly", 233.333, "s2_fly_03")
FTB_ROW("fly", 266.667, "s2_fly_04")
FTB_ROW("fly", 300, "s2_fly_03")
FTB_ROW("fly", 366.667, "s2_fly_04")
FTB_ROW("fly", 433.333, "s2_fly_05")

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)   // the key (one knee, hand to the forehead) at 0; the hand comes off, he pushes up, stands, drops into the stance
FTB_ROW("getup", 0, "getup")   // KEY
FTB_ROW("getup", 33.333, "s2_getup_01")
FTB_ROW("getup", 66.667, "s2_getup_02")
FTB_ROW("getup", 100, "s2_getup_03")
FTB_ROW("getup", 133.333, "s2_getup_04")
FTB_ROW("getup", 166.667, "s2_getup_05")
FTB_ROW("getup", 200, "s2_getup_06")
FTB_ROW("getup", 233.333, "s2_getup_07")
FTB_ROW("getup", 266.667, "s2_getup_08")
FTB_ROW("getup", 300, "s2_getup_09")
FTB_ROW("getup", 333.333, "s2_getup_10")
FTB_ROW("getup", 366.667, "s2_getup_11")
FTB_ROW("getup", 400, "s2_getup_12")
FTB_ROW("getup", 433.333, "idle")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
