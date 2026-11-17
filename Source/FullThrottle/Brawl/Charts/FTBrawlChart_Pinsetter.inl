// Exposure chart of Pinsetter. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings (s2_*) are between them. Times are ms; in a Walk or Loop state they are positions in the cycle.

FTB_STATE("idle", Loop, 1256.6)   // one breath (the code squash sin(5t) has this period)
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 150, "s2_idle_02")
FTB_ROW("idle", 300, "s2_idle_01")
FTB_ROW("idle", 483.333, "s2_idle_02")
FTB_ROW("idle", 633.333, "idle")
FTB_ROW("idle", 783.333, "s2_idle_03")
FTB_ROW("idle", 933.333, "s2_idle_04")
FTB_ROW("idle", 1100, "s2_idle_03")

FTB_STATE("walk", Walk, 909.091)   // one cycle = two steps; each step is the same 12 new drawings after its key
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

FTB_STATE("wind", Time, 280)   // coil deeper, hold at the deepest, rise, step, release
FTB_ROW("wind", 0, "wind")   // KEY
FTB_ROW("wind", 50, "s2_wind_01")
FTB_ROW("wind", 166.667, "s2_wind_03")
FTB_ROW("wind", 200, "s2_wind_04")
FTB_ROW("wind", 233.333, "s2_wind_05")
FTB_ROW("wind", 263.333, "s2_wind_06")

FTB_STATE("swing", Time, 220)   // contact on the key at 0, over-extension, follow-through, retract, back to the ready stance
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 66.667, "s2_swing_02")
FTB_ROW("swing", 100, "s2_swing_03")
FTB_ROW("swing", 133.333, "s2_swing_04")
FTB_ROW("swing", 166.667, "s2_swing_05")
FTB_ROW("swing", 200, "s2_idle_02")

FTB_STATE("dash", Time, 750)   // sprint cycle of 7 drawings (233 ms a stride), skid on the last two steps
FTB_ROW("dash", 0, "run")   // KEY
FTB_ROW("dash", 33.333, "s2_dash_01")
FTB_ROW("dash", 66.667, "s2_dash_02")
FTB_ROW("dash", 100, "s2_dash_03")
FTB_ROW("dash", 133.333, "s2_dash_04")
FTB_ROW("dash", 166.667, "s2_dash_05")
FTB_ROW("dash", 200, "s2_dash_06")
FTB_ROW("dash", 233.333, "s2_dash_07")
FTB_ROW("dash", 266.667, "s2_dash_01")
FTB_ROW("dash", 300, "s2_dash_02")
FTB_ROW("dash", 333.333, "s2_dash_03")
FTB_ROW("dash", 366.667, "s2_dash_04")
FTB_ROW("dash", 400, "s2_dash_05")
FTB_ROW("dash", 433.333, "s2_dash_06")
FTB_ROW("dash", 466.667, "run")
FTB_ROW("dash", 500, "s2_dash_01")
FTB_ROW("dash", 533.333, "s2_dash_02")
FTB_ROW("dash", 566.667, "s2_dash_03")
FTB_ROW("dash", 600, "s2_dash_04")
FTB_ROW("dash", 633.333, "s2_dash_05")
FTB_ROW("dash", 666.667, "s2_dash_06")
FTB_ROW("dash", 700, "s2_dash_07")
FTB_ROW("dash", 716.667, "s2_dash_08")

FTB_STATE("hurt", Time, 260)   // hit on the key, recoil, stagger, catch, shake off, recover
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 66.667, "s2_hurt_02")
FTB_ROW("hurt", 100, "s2_hurt_03")
FTB_ROW("hurt", 133.333, "s2_hurt_04")
FTB_ROW("hurt", 166.667, "s2_hurt_05")
FTB_ROW("hurt", 200, "s2_hurt_06")
FTB_ROW("hurt", 233.333, "s2_getup_09")

FTB_STATE("fly", Time, 453.3)   // upright drawings only (the code tips the drawing back): a flail of four
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_hurt_01")
FTB_ROW("fly", 66.667, "s2_hurt_03")
FTB_ROW("fly", 100, "s2_hurt_02")
FTB_ROW("fly", 133.333, "hurt")
FTB_ROW("fly", 166.667, "s2_hurt_01")
FTB_ROW("fly", 200, "s2_hurt_03")
FTB_ROW("fly", 233.333, "s2_hurt_02")
FTB_ROW("fly", 266.667, "hurt")
FTB_ROW("fly", 300, "s2_hurt_01")
FTB_ROW("fly", 333.333, "s2_hurt_03")
FTB_ROW("fly", 366.667, "s2_hurt_02")
FTB_ROW("fly", 400, "hurt")
FTB_ROW("fly", 433.333, "s2_hurt_01")

FTB_STATE("down", Time, 750)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)   // elbow, hand, roll, all fours, kneel, crouch, rise, stand, fists up
FTB_ROW("getup", 0, "down")   // KEY
FTB_ROW("getup", 33.333, "s2_getup_01")
FTB_ROW("getup", 66.667, "s2_getup_02")
FTB_ROW("getup", 100, "s2_getup_03")
FTB_ROW("getup", 133.333, "s2_getup_04")
FTB_ROW("getup", 166.667, "s2_getup_05")
FTB_ROW("getup", 200, "s2_getup_06")
FTB_ROW("getup", 233.333, "s2_getup_07")
FTB_ROW("getup", 266.667, "s2_getup_08")
FTB_ROW("getup", 300, "s2_getup_09")
FTB_ROW("getup", 333.333, "s2_idle_04")
FTB_ROW("getup", 366.667, "s2_idle_03")
FTB_ROW("getup", 400, "s2_idle_01")
FTB_ROW("getup", 433.333, "idle")

FTB_STATE("dead", Time, 1400)
FTB_ROW("dead", 0, "down")   // KEY

FTB_STATE("lifted", Time, 300)
FTB_ROW("lifted", 0, "down")   // KEY

FTB_STATE("hurled", Time, 562.6)
FTB_ROW("hurled", 0, "down")   // KEY
