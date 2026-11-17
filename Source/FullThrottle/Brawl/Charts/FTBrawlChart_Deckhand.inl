// Exposure chart of Deckhand. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// Stage 2 art: every key of the base chart is here at its time; rows without KEY are new (frames s2_*) or a second showing of a key.

FTB_STATE("idle", Loop, 1256.6)   // one breath (the code squash sin(5t) has this period): neutral, in, neutral, out
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 314.15, "s2_idle_01")
FTB_ROW("idle", 628.3, "idle")
FTB_ROW("idle", 942.45, "s2_idle_02")

FTB_STATE("walk", Walk, 909.091)   // one cycle = two steps; walk1 = heel plant, walk2 = foot flat; one step of 13 new drawings fills each half
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 32.468, "s2_walk_01")
FTB_ROW("walk", 64.935, "s2_walk_02")
FTB_ROW("walk", 97.403, "s2_walk_03")
FTB_ROW("walk", 129.87, "s2_walk_05")
FTB_ROW("walk", 162.338, "s2_walk_06")
FTB_ROW("walk", 194.805, "s2_walk_07")
FTB_ROW("walk", 259.74, "s2_walk_09")
FTB_ROW("walk", 292.208, "s2_walk_10")
FTB_ROW("walk", 357.143, "s2_walk_12")
FTB_ROW("walk", 389.61, "s2_walk_13")
FTB_ROW("walk", 422.078, "s2_walk_15")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 487.013, "s2_walk_03")
FTB_ROW("walk", 519.48, "s2_walk_04")
FTB_ROW("walk", 551.948, "s2_walk_05")
FTB_ROW("walk", 584.415, "s2_walk_06")
FTB_ROW("walk", 616.883, "s2_walk_07")
FTB_ROW("walk", 681.818, "s2_walk_09")
FTB_ROW("walk", 714.285, "s2_walk_10")
FTB_ROW("walk", 779.22, "s2_walk_12")
FTB_ROW("walk", 811.688, "s2_walk_13")
FTB_ROW("walk", 844.155, "s2_walk_14")
FTB_ROW("walk", 876.623, "s2_walk_15")

FTB_STATE("wind", Time, 280)   // on twos; the tension drawing one step so that the last drawing is up 30 ms before the swing
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 66.667, "s2_wind_02")
FTB_ROW("wind", 100, "s2_wind_03")
FTB_ROW("wind", 133.333, "s2_wind_04")
FTB_ROW("wind", 150, "s2_wind_05")
FTB_ROW("wind", 183.333, "s2_wind_06")
FTB_ROW("wind", 216.667, "s2_wind_07")
FTB_ROW("wind", 250, "s2_wind_08")

FTB_STATE("swing", Time, 220)   // hit at 0 on the key; follow-through on twos, the pull back and the overshoot on ones
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 66.667, "s2_swing_02")
FTB_ROW("swing", 100, "s2_swing_03")
FTB_ROW("swing", 133.333, "s2_swing_04")
FTB_ROW("swing", 150, "s2_swing_05")
FTB_ROW("swing", 183.333, "s2_swing_06")
FTB_ROW("swing", 200, "s2_swing_07")

FTB_STATE("dash", Time, 750)   // he stands until 180 ms (key, load, deepest load, load, push-off), then an 8-drawing run cycle of 267 ms
FTB_ROW("dash", 0, "run")   // KEY
FTB_ROW("dash", 33.333, "s2_dash_01")
FTB_ROW("dash", 133.333, "s2_dash_03")
FTB_ROW("dash", 166.667, "s2_dash_04")
FTB_ROW("dash", 200, "s2_dash_05")
FTB_ROW("dash", 233.333, "s2_dash_06")
FTB_ROW("dash", 266.667, "s2_dash_07")
FTB_ROW("dash", 300, "s2_dash_08")
FTB_ROW("dash", 333.333, "s2_dash_09")
FTB_ROW("dash", 366.667, "s2_dash_10")
FTB_ROW("dash", 400, "s2_dash_11")
FTB_ROW("dash", 433.333, "s2_dash_04")
FTB_ROW("dash", 466.667, "s2_dash_05")
FTB_ROW("dash", 500, "s2_dash_06")
FTB_ROW("dash", 533.333, "s2_dash_07")
FTB_ROW("dash", 566.667, "s2_dash_08")
FTB_ROW("dash", 600, "s2_dash_09")
FTB_ROW("dash", 633.333, "s2_dash_10")
FTB_ROW("dash", 666.667, "s2_dash_11")
FTB_ROW("dash", 700, "s2_dash_04")
FTB_ROW("dash", 733.333, "s2_dash_05")

FTB_STATE("hurt", Time, 260)   // fast recoil (one step on the deeper recoil), the peak and the recovery on twos
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 50, "s2_hurt_02")
FTB_ROW("hurt", 83.333, "s2_hurt_03")
FTB_ROW("hurt", 116.667, "s2_hurt_04")
FTB_ROW("hurt", 150, "s2_hurt_05")
FTB_ROW("hurt", 183.333, "s2_hurt_06")
FTB_ROW("hurt", 216.667, "s2_hurt_07")
FTB_ROW("hurt", 233.333, "s2_hurt_08")

FTB_STATE("fly", Time, 453.3)   // the game also tips the drawing back up to 70 degrees
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_fly_01")
FTB_ROW("fly", 66.667, "s2_fly_02")
FTB_ROW("fly", 100, "s2_fly_03")
FTB_ROW("fly", 133.333, "s2_fly_04")
FTB_ROW("fly", 166.667, "s2_fly_05")
FTB_ROW("fly", 200, "s2_fly_03")
FTB_ROW("fly", 233.333, "s2_fly_04")
FTB_ROW("fly", 266.667, "s2_fly_05")
FTB_ROW("fly", 300, "s2_fly_06")
FTB_ROW("fly", 333.333, "s2_fly_07")
FTB_ROW("fly", 366.667, "s2_fly_06")
FTB_ROW("fly", 400, "s2_fly_07")
FTB_ROW("fly", 433.333, "s2_fly_06")

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)   // the lying key one step, then 13 drawings on twos
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
