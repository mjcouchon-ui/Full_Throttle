// Exposure chart of Sven. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// Stage 2: rows without KEY were added by the art pass (frames s2_* are new drawings; a few rows show an existing drawing again).

FTB_STATE("intro", Time, 2460)
FTB_ROW("intro", 0, "bidlein")   // KEY
FTB_ROW("intro", 160, "intro")   // KEY
FTB_ROW("intro", 400, "bentrance")   // KEY
FTB_ROW("intro", 1800, "bidleout")   // KEY
FTB_ROW("intro", 2060, "intro2")   // KEY

FTB_STATE("intro2", Time, 2820)
FTB_ROW("intro2", 2460, "bidlein")   // KEY
FTB_ROW("intro2", 2500, "s2_idle_04")
FTB_ROW("intro2", 2533.333, "bidleout")
FTB_ROW("intro2", 2566.667, "s2_idle_01")
FTB_ROW("intro2", 2600, "s2_idle_02")
FTB_ROW("intro2", 2640, "idle")   // KEY
FTB_ROW("intro2", 2666.667, "s2_idle_01")
FTB_ROW("intro2", 2700, "s2_idle_02")
FTB_ROW("intro2", 2733.333, "s2_idle_01")
FTB_ROW("intro2", 2766.667, "bidleout")
FTB_ROW("intro2", 2800, "s2_idle_04")

FTB_STATE("idle", Loop, 1600)   // one cycle
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 150, "s2_idle_01")
FTB_ROW("idle", 300, "bidleout")   // KEY
FTB_ROW("idle", 366.667, "s2_idle_01")
FTB_ROW("idle", 450, "s2_idle_02")
FTB_ROW("idle", 550, "s2_idle_01")
FTB_ROW("idle", 650, "idle")   // KEY
FTB_ROW("idle", 750, "s2_idle_04")
FTB_ROW("idle", 850, "s2_idle_05")
FTB_ROW("idle", 1000, "bidlein")   // KEY
FTB_ROW("idle", 1116.667, "s2_idle_04")
FTB_ROW("idle", 1233.333, "bidleout")
FTB_ROW("idle", 1350, "idle")   // KEY
FTB_ROW("idle", 1450, "s2_idle_01")
FTB_ROW("idle", 1525, "bidleout")

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 37.879, "s2_walk_01")
FTB_ROW("walk", 75.758, "s2_walk_02")
FTB_ROW("walk", 113.636, "s2_walk_03")
FTB_ROW("walk", 151.515, "bwalkdown")   // KEY
FTB_ROW("walk", 170.455, "s2_walk_04")
FTB_ROW("walk", 208.333, "s2_walk_05")
FTB_ROW("walk", 246.212, "s2_walk_06")
FTB_ROW("walk", 284.091, "s2_walk_07")
FTB_ROW("walk", 303.03, "bwalkpass")   // KEY
FTB_ROW("walk", 340.909, "s2_walk_08")
FTB_ROW("walk", 378.788, "s2_walk_09")
FTB_ROW("walk", 416.667, "s2_walk_10")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 492.424, "s2_walk_11")
FTB_ROW("walk", 530.303, "s2_walk_12")
FTB_ROW("walk", 568.182, "s2_walk_13")
FTB_ROW("walk", 606.061, "bwalklift")   // KEY
FTB_ROW("walk", 625, "s2_walk_14")
FTB_ROW("walk", 662.879, "s2_walk_15")
FTB_ROW("walk", 700.758, "s2_walk_16")
FTB_ROW("walk", 738.636, "s2_walk_17")
FTB_ROW("walk", 757.576, "bwalkpush")   // KEY
FTB_ROW("walk", 776.515, "s2_walk_18")
FTB_ROW("walk", 809.655, "s2_walk_19")
FTB_ROW("walk", 842.795, "s2_walk_20")
FTB_ROW("walk", 875.935, "s2_walk_21")

FTB_STATE("aim", Time, 550)
FTB_ROW("aim", 0, "bidlein")   // KEY
FTB_ROW("aim", 33.333, "bwindbridge")
FTB_ROW("aim", 66.667, "s2_wind_01")
FTB_ROW("aim", 100, "s2_wind_02")
FTB_ROW("aim", 133.333, "s2_wind_03")
FTB_ROW("aim", 160, "wind")   // KEY
FTB_ROW("aim", 200, "s2_wind_03")
FTB_ROW("aim", 233.333, "wind")
FTB_ROW("aim", 266.667, "s2_wind_03")
FTB_ROW("aim", 300, "s2_wind_04")
FTB_ROW("aim", 333.333, "s2_wind_03")
FTB_ROW("aim", 366.667, "s2_wind_04")
FTB_ROW("aim", 400, "s2_wind_05")
FTB_ROW("aim", 433.333, "s2_wind_04")
FTB_ROW("aim", 466.667, "s2_wind_05")
FTB_ROW("aim", 500, "s2_wind_04")
FTB_ROW("aim", 533.333, "s2_wind_05")

FTB_STATE("charge", Loop, 333.333)   // one cycle
FTB_ROW("charge", 0, "walk1")   // KEY
FTB_ROW("charge", 27.778, "s2_walk_02")
FTB_ROW("charge", 55.556, "bwalkdown")   // KEY
FTB_ROW("charge", 72.222, "s2_walk_05")
FTB_ROW("charge", 111.111, "bwalkpass")   // KEY
FTB_ROW("charge", 138.889, "s2_walk_09")
FTB_ROW("charge", 166.667, "walk2")   // KEY
FTB_ROW("charge", 194.444, "s2_walk_12")
FTB_ROW("charge", 222.222, "bwalklift")   // KEY
FTB_ROW("charge", 238.889, "s2_walk_16")
FTB_ROW("charge", 277.778, "bwalkpush")   // KEY
FTB_ROW("charge", 294.444, "s2_walk_20")

FTB_STATE("wind", Time, 500)
FTB_ROW("wind", 0, "bwindbridge")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 66.667, "s2_wind_02")
FTB_ROW("wind", 100, "wind")   // KEY
FTB_ROW("wind", 133.333, "s2_wind_03")
FTB_ROW("wind", 166.667, "wind")
FTB_ROW("wind", 200, "s2_wind_03")
FTB_ROW("wind", 233.333, "s2_wind_04")
FTB_ROW("wind", 266.667, "s2_wind_03")
FTB_ROW("wind", 300, "s2_wind_04")
FTB_ROW("wind", 333.333, "s2_wind_05")
FTB_ROW("wind", 366.667, "s2_wind_04")
FTB_ROW("wind", 400, "s2_wind_05")
FTB_ROW("wind", 433.333, "s2_wind_04")
FTB_ROW("wind", 466.667, "s2_wind_05")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 70, "bstrikebridge")   // KEY
FTB_ROW("swing", 100, "s2_swing_02")
FTB_ROW("swing", 130, "bretract")   // KEY
FTB_ROW("swing", 166.667, "s2_swing_03")
FTB_ROW("swing", 200, "idle")

FTB_STATE("swing:1", Time, 220)
FTB_ROW("swing:1", 0, "bash")   // KEY
FTB_ROW("swing:1", 33.333, "s2_swing_04")
FTB_ROW("swing:1", 70, "bstrikebridge")   // KEY
FTB_ROW("swing:1", 100, "s2_swing_02")
FTB_ROW("swing:1", 130, "bretract")   // KEY
FTB_ROW("swing:1", 166.667, "s2_swing_03")
FTB_ROW("swing:1", 200, "idle")

FTB_STATE("gloves", Time, 750)
FTB_ROW("gloves", 0, "bsigload")   // KEY
FTB_ROW("gloves", 33.333, "s2_gloves_01")
FTB_ROW("gloves", 66.667, "bsigload")
FTB_ROW("gloves", 100, "s2_gloves_01")
FTB_ROW("gloves", 133.333, "s2_gloves_02")
FTB_ROW("gloves", 166.667, "s2_gloves_03")
FTB_ROW("gloves", 200, "s2_gloves_04")
FTB_ROW("gloves", 220, "gloves")   // KEY
FTB_ROW("gloves", 250, "s2_gloves_05")
FTB_ROW("gloves", 283.333, "s2_gloves_06")
FTB_ROW("gloves", 316.667, "s2_gloves_07")
FTB_ROW("gloves", 350, "s2_gloves_08")
FTB_ROW("gloves", 383.333, "s2_gloves_09")
FTB_ROW("gloves", 416.667, "s2_gloves_10")
FTB_ROW("gloves", 450, "s2_gloves_11")
FTB_ROW("gloves", 483.333, "s2_gloves_12")
FTB_ROW("gloves", 520, "bsigbridge")   // KEY
FTB_ROW("gloves", 550, "s2_gloves_14")
FTB_ROW("gloves", 583.333, "s2_gloves_15")
FTB_ROW("gloves", 616.667, "s2_gloves_13")
FTB_ROW("gloves", 650, "s2_gloves_15")
FTB_ROW("gloves", 683.333, "s2_gloves_14")
FTB_ROW("gloves", 716.667, "s2_gloves_15")

FTB_STATE("lunge", Time, 750)
FTB_ROW("lunge", 0, "bsigreach")   // KEY
FTB_ROW("lunge", 33.333, "s2_lunge_01")
FTB_ROW("lunge", 66.667, "s2_lunge_02")
FTB_ROW("lunge", 100, "lunge")   // KEY
FTB_ROW("lunge", 133.333, "s2_lunge_03")
FTB_ROW("lunge", 166.667, "s2_lunge_04")
FTB_ROW("lunge", 200, "s2_lunge_05")
FTB_ROW("lunge", 233.333, "s2_lunge_04")
FTB_ROW("lunge", 266.667, "s2_lunge_03")
FTB_ROW("lunge", 300, "s2_lunge_04")
FTB_ROW("lunge", 333.333, "s2_lunge_05")
FTB_ROW("lunge", 366.667, "s2_lunge_04")
FTB_ROW("lunge", 400, "s2_lunge_03")
FTB_ROW("lunge", 433.333, "s2_lunge_04")
FTB_ROW("lunge", 466.667, "s2_lunge_05")
FTB_ROW("lunge", 500, "s2_lunge_04")
FTB_ROW("lunge", 533.333, "s2_lunge_05")
FTB_ROW("lunge", 550, "s2_hug_02")
FTB_ROW("lunge", 580, "bsigsettle")   // KEY
FTB_ROW("lunge", 600, "s2_lunge_06")
FTB_ROW("lunge", 666.667, "s2_hurt_04")
FTB_ROW("lunge", 700, "s2_swing_03")
FTB_ROW("lunge", 733.333, "idle")

FTB_STATE("hug", Time, 600)
FTB_ROW("hug", 0, "bsigreach")   // KEY
FTB_ROW("hug", 33.333, "s2_hug_01")
FTB_ROW("hug", 66.667, "s2_hug_02")
FTB_ROW("hug", 100, "s2_hug_03")
FTB_ROW("hug", 120, "bsigsettle")   // KEY
FTB_ROW("hug", 150, "s2_hug_04")
FTB_ROW("hug", 183.333, "s2_hug_05")
FTB_ROW("hug", 216.667, "s2_hug_06")
FTB_ROW("hug", 250, "s2_hug_07")
FTB_ROW("hug", 280, "hug")   // KEY
FTB_ROW("hug", 300, "s2_hug_06")
FTB_ROW("hug", 333.333, "s2_hug_05")
FTB_ROW("hug", 366.667, "s2_hug_06")
FTB_ROW("hug", 400, "s2_hug_07")
FTB_ROW("hug", 433.333, "s2_hug_06")
FTB_ROW("hug", 466.667, "s2_hug_05")
FTB_ROW("hug", 500, "s2_hug_06")
FTB_ROW("hug", 533.333, "s2_hug_07")
FTB_ROW("hug", 566.667, "s2_hug_06")

FTB_STATE("dizzy", Loop, 500)
FTB_ROW("dizzy", 0, "bhitbridge")   // KEY
FTB_ROW("dizzy", 33.333, "s2_dizzy_06")
FTB_ROW("dizzy", 66.667, "s2_dizzy_05")
FTB_ROW("dizzy", 100, "s2_dizzy_01")
FTB_ROW("dizzy", 133.333, "s2_dizzy_02")
FTB_ROW("dizzy", 166.667, "s2_dizzy_03")
FTB_ROW("dizzy", 216.667, "s2_dizzy_02")
FTB_ROW("dizzy", 250, "bidleout")   // KEY
FTB_ROW("dizzy", 283.333, "s2_dizzy_07")
FTB_ROW("dizzy", 316.667, "s2_dizzy_01")
FTB_ROW("dizzy", 350, "s2_dizzy_04")
FTB_ROW("dizzy", 383.333, "s2_dizzy_05")
FTB_ROW("dizzy", 416.667, "s2_dizzy_06")
FTB_ROW("dizzy", 466.667, "s2_dizzy_05")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 70, "bhitbridge")   // KEY
FTB_ROW("hurt", 100, "s2_hurt_02")
FTB_ROW("hurt", 133.333, "s2_hurt_03")
FTB_ROW("hurt", 166.667, "s2_hurt_04")
FTB_ROW("hurt", 190, "bretract")   // KEY
FTB_ROW("hurt", 216.667, "s2_swing_03")
FTB_ROW("hurt", 233.333, "idle")

FTB_STATE("fly", Time, 346.7)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_hurt_01")
FTB_ROW("fly", 66.667, "s2_fly_02")
FTB_ROW("fly", 100, "s2_fly_01")
FTB_ROW("fly", 133.333, "s2_fly_03")
FTB_ROW("fly", 166.667, "s2_fly_02")
FTB_ROW("fly", 200, "s2_fly_01")
FTB_ROW("fly", 233.333, "s2_fly_03")
FTB_ROW("fly", 266.667, "s2_fly_02")
FTB_ROW("fly", 300, "s2_fly_01")
FTB_ROW("fly", 316.667, "s2_fly_03")

FTB_STATE("down", Time, 700)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "bkneel")   // KEY
FTB_ROW("getup", 33.333, "s2_getup_01")
FTB_ROW("getup", 66.667, "s2_getup_02")
FTB_ROW("getup", 100, "s2_getup_03")
FTB_ROW("getup", 133.333, "s2_getup_04")
FTB_ROW("getup", 150, "s2_getup_05")
FTB_ROW("getup", 180, "bidlein")   // KEY
FTB_ROW("getup", 200, "s2_getup_06")
FTB_ROW("getup", 233.333, "s2_idle_02")
FTB_ROW("getup", 266.667, "s2_idle_01")
FTB_ROW("getup", 300, "bidleout")
FTB_ROW("getup", 340, "idle")   // KEY
FTB_ROW("getup", 366.667, "s2_idle_04")
FTB_ROW("getup", 400, "bidleout")
FTB_ROW("getup", 433.333, "idle")

FTB_STATE("defeat", Time, 420)
FTB_ROW("defeat", 0, "bhitbridge")   // KEY
FTB_ROW("defeat", 33.333, "s2_defeat_01")
FTB_ROW("defeat", 66.667, "s2_dizzy_02")
FTB_ROW("defeat", 100, "s2_defeat_02")
FTB_ROW("defeat", 120, "bkneel")   // KEY
FTB_ROW("defeat", 150, "s2_defeat_03")
