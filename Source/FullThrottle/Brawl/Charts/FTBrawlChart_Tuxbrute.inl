// Exposure chart of Tuxbrute. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// Stage 2 art: every key of the base chart is here at its time; rows without KEY are new (frames s2_*) or a second showing of an existing drawing.

FTB_STATE("idle", Loop, 1256.637)   // one breath (the code squash sin(5t) has this period); the key drawing is the middle of the breath and is shown twice
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 157.08, "s2_idle_01")
FTB_ROW("idle", 314.159, "s2_idle_02")
FTB_ROW("idle", 471.239, "s2_idle_01")
FTB_ROW("idle", 628.318, "idle")
FTB_ROW("idle", 785.398, "s2_idle_03")
FTB_ROW("idle", 942.478, "s2_idle_04")
FTB_ROW("idle", 1099.557, "s2_idle_03")

FTB_STATE("walk", Walk, 909.091)   // one cycle = two steps; both keys are the same contact pose, one step of 13 new drawings lies between them
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 32.468, "s2_walk_01")
FTB_ROW("walk", 64.935, "s2_walk_02")
FTB_ROW("walk", 97.403, "s2_walk_03")
FTB_ROW("walk", 129.87, "s2_walk_04")
FTB_ROW("walk", 162.338, "s2_walk_05")
FTB_ROW("walk", 194.805, "s2_walk_06")
FTB_ROW("walk", 227.273, "s2_walk_07")
FTB_ROW("walk", 259.74, "s2_walk_08")
FTB_ROW("walk", 292.208, "s2_walk_09")
FTB_ROW("walk", 324.675, "s2_walk_10")
FTB_ROW("walk", 357.143, "s2_walk_11")
FTB_ROW("walk", 389.61, "s2_walk_12")
FTB_ROW("walk", 422.078, "s2_walk_13")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 487.013, "s2_walk_01")
FTB_ROW("walk", 519.481, "s2_walk_02")
FTB_ROW("walk", 551.948, "s2_walk_03")
FTB_ROW("walk", 584.416, "s2_walk_04")
FTB_ROW("walk", 616.883, "s2_walk_05")
FTB_ROW("walk", 649.351, "s2_walk_06")
FTB_ROW("walk", 681.818, "s2_walk_07")
FTB_ROW("walk", 714.286, "s2_walk_08")
FTB_ROW("walk", 746.753, "s2_walk_09")
FTB_ROW("walk", 779.221, "s2_walk_10")
FTB_ROW("walk", 811.688, "s2_walk_11")
FTB_ROW("walk", 844.156, "s2_walk_12")
FTB_ROW("walk", 876.623, "s2_walk_13")

FTB_STATE("wind", Time, 550)   // sinks into the coil, rises to the top, holds the tell 100 ms, launches in the last 67 ms; the blow lands on the swing key
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 66.667, "s2_wind_02")
FTB_ROW("wind", 183.333, "s2_wind_04")
FTB_ROW("wind", 216.667, "s2_wind_05")
FTB_ROW("wind", 266.667, "s2_wind_06")
FTB_ROW("wind", 300, "s2_wind_07")
FTB_ROW("wind", 350, "s2_wind_06")
FTB_ROW("wind", 383.333, "s2_wind_07")
FTB_ROW("wind", 483.333, "s2_wind_08")
FTB_ROW("wind", 516.667, "s2_wind_09")

FTB_STATE("swing", Time, 220)   // hit at 0 on the key; follow-through, carry down, pull back, home, rock back, settle
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 66.667, "s2_swing_02")
FTB_ROW("swing", 100, "s2_swing_03")
FTB_ROW("swing", 133.333, "s2_swing_04")
FTB_ROW("swing", 166.667, "s2_swing_05")
FTB_ROW("swing", 200, "s2_swing_06")

FTB_STATE("hurt", Time, 260)   // recoil peak, arch easing, stagger, fold, brace, sway, gathered
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 66.667, "s2_hurt_02")
FTB_ROW("hurt", 100, "s2_hurt_03")
FTB_ROW("hurt", 133.333, "s2_hurt_04")
FTB_ROW("hurt", 166.667, "s2_hurt_05")
FTB_ROW("hurt", 200, "s2_hurt_06")
FTB_ROW("hurt", 233.333, "s2_hurt_07")

FTB_STATE("fly", Time, 453.3)   // the game also tips the drawing back up to 70 degrees: all drawings are upright
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_hurt_01")
FTB_ROW("fly", 133.333, "s2_fly_09")

FTB_STATE("down", Time, 750)   // bounce after landing; from 617 ms he stirs and rolls up toward the kneel that opens getup
FTB_ROW("down", 0, "down")   // KEY
FTB_ROW("down", 616.667, "s2_down_02")
FTB_ROW("down", 683.333, "s2_down_03")

FTB_STATE("getup", Time, 450)   // a heavy push off the knee (three 50 ms drawings), then the rise on twos, full height, and settling into the ready stance
FTB_ROW("getup", 0, "getup")   // KEY
FTB_ROW("getup", 50, "s2_getup_01")
FTB_ROW("getup", 100, "s2_getup_02")
FTB_ROW("getup", 150, "s2_getup_03")
FTB_ROW("getup", 183.333, "s2_getup_04")
FTB_ROW("getup", 216.667, "s2_getup_05")
FTB_ROW("getup", 250, "s2_getup_06")
FTB_ROW("getup", 283.333, "s2_getup_07")
FTB_ROW("getup", 316.667, "s2_getup_08")
FTB_ROW("getup", 350, "s2_idle_02")
FTB_ROW("getup", 383.333, "s2_idle_01")
FTB_ROW("getup", 416.667, "idle")

FTB_STATE("dead", Time, 1400)   // the same bounce after landing as down
FTB_ROW("dead", 0, "down")   // KEY
FTB_ROW("dead", 33.333, "s2_down_01")
FTB_ROW("dead", 100, "down")

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
