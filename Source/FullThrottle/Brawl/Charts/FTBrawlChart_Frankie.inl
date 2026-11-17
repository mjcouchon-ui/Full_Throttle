// Exposure chart of Frankie. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings (s2_*) were generated from the existing frames with the image tool, stage 2; see art/Frankie/manifest.json.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.

FTB_STATE("intro", Time, 2460)
FTB_ROW("intro", 0, "bidlein")   // KEY
FTB_ROW("intro", 160, "intro")   // KEY
FTB_ROW("intro", 400, "bentrance")   // KEY
FTB_ROW("intro", 1800, "bidleout")   // KEY
FTB_ROW("intro", 2060, "intro2")   // KEY

FTB_STATE("intro2", Time, 2820)
FTB_ROW("intro2", 2460, "bidlein")   // KEY
FTB_ROW("intro2", 2640, "idle")   // KEY

FTB_STATE("idle", Loop, 1600)   // one cycle
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 66.667, "s2_idle_01")
FTB_ROW("idle", 133.333, "s2_idle_02")
FTB_ROW("idle", 200, "s2_idle_03")
FTB_ROW("idle", 266.667, "s2_idle_04")
FTB_ROW("idle", 300, "bidleout")   // KEY
FTB_ROW("idle", 366.667, "s2_idle_05")
FTB_ROW("idle", 433.333, "s2_idle_06")
FTB_ROW("idle", 500, "s2_idle_07")
FTB_ROW("idle", 583.333, "s2_idle_08")
FTB_ROW("idle", 650, "idle")   // KEY
FTB_ROW("idle", 716.667, "s2_idle_01")
FTB_ROW("idle", 783.333, "s2_idle_02")
FTB_ROW("idle", 850, "s2_idle_03")
FTB_ROW("idle", 933.333, "s2_idle_04")
FTB_ROW("idle", 1000, "bidlein")   // KEY
FTB_ROW("idle", 1066.667, "s2_idle_05")
FTB_ROW("idle", 1133.333, "s2_idle_06")
FTB_ROW("idle", 1200, "s2_idle_07")
FTB_ROW("idle", 1283.333, "s2_idle_08")
FTB_ROW("idle", 1350, "idle")   // KEY
FTB_ROW("idle", 1450, "s2_idle_05")
FTB_ROW("idle", 1533.333, "s2_idle_08")

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 75.758, "s2_walk_02")
FTB_ROW("walk", 151.515, "bwalkdown")   // KEY
FTB_ROW("walk", 227.273, "s2_walk_03")
FTB_ROW("walk", 303.03, "bwalkpass")   // KEY
FTB_ROW("walk", 378.788, "s2_walk_04")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 530.303, "s2_walk_02")
FTB_ROW("walk", 606.061, "bwalklift")   // KEY
FTB_ROW("walk", 681.818, "s2_walk_03")
FTB_ROW("walk", 757.576, "bwalkpush")   // KEY
FTB_ROW("walk", 833.333, "s2_walk_04")

FTB_STATE("aim", Time, 550)
FTB_ROW("aim", 0, "bidlein")   // KEY
FTB_ROW("aim", 33.333, "bwindbridge")
FTB_ROW("aim", 66.667, "s2_wind_02")
FTB_ROW("aim", 100, "s2_wind_03")
FTB_ROW("aim", 133.333, "s2_wind_05")
FTB_ROW("aim", 160, "wind")   // KEY
FTB_ROW("aim", 200, "s2_wind_06")
FTB_ROW("aim", 233.333, "s2_wind_07")
FTB_ROW("aim", 266.667, "wind")
FTB_ROW("aim", 300, "s2_wind_05")
FTB_ROW("aim", 333.333, "s2_wind_06")
FTB_ROW("aim", 366.667, "s2_wind_07")
FTB_ROW("aim", 400, "wind")
FTB_ROW("aim", 433.333, "s2_wind_05")
FTB_ROW("aim", 466.667, "s2_wind_06")
FTB_ROW("aim", 500, "s2_wind_07")
FTB_ROW("aim", 533.333, "s2_wind_08")

FTB_STATE("charge", Loop, 333.333)   // one cycle
FTB_ROW("charge", 0, "walk1")   // KEY
FTB_ROW("charge", 27.778, "s2_walk_02")
FTB_ROW("charge", 55.556, "bwalkdown")   // KEY
FTB_ROW("charge", 83.333, "s2_walk_03")
FTB_ROW("charge", 111.111, "bwalkpass")   // KEY
FTB_ROW("charge", 138.889, "s2_walk_04")
FTB_ROW("charge", 166.667, "walk2")   // KEY
FTB_ROW("charge", 194.444, "s2_walk_02")
FTB_ROW("charge", 222.222, "bwalklift")   // KEY
FTB_ROW("charge", 250, "s2_walk_03")
FTB_ROW("charge", 277.778, "bwalkpush")   // KEY
FTB_ROW("charge", 305.555, "s2_walk_04")

FTB_STATE("wind", Time, 500)
FTB_ROW("wind", 0, "bwindbridge")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_02")
FTB_ROW("wind", 66.667, "s2_wind_03")
FTB_ROW("wind", 100, "wind")   // KEY
FTB_ROW("wind", 133.333, "s2_wind_05")
FTB_ROW("wind", 166.667, "s2_wind_06")
FTB_ROW("wind", 200, "s2_wind_07")
FTB_ROW("wind", 233.333, "wind")
FTB_ROW("wind", 266.667, "s2_wind_05")
FTB_ROW("wind", 300, "s2_wind_06")
FTB_ROW("wind", 333.333, "s2_wind_07")
FTB_ROW("wind", 366.667, "wind")
FTB_ROW("wind", 400, "s2_wind_05")
FTB_ROW("wind", 433.333, "s2_wind_07")
FTB_ROW("wind", 466.667, "s2_wind_08")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 70, "bstrikebridge")   // KEY
FTB_ROW("swing", 100, "s2_swing_05")
FTB_ROW("swing", 130, "bretract")   // KEY
FTB_ROW("swing", 166.667, "s2_swing_06")
FTB_ROW("swing", 200, "s2_swing_07")

FTB_STATE("swing:1", Time, 220)
FTB_ROW("swing:1", 0, "bash")   // KEY
FTB_ROW("swing:1", 33.333, "s2_swing_03")
FTB_ROW("swing:1", 50, "s2_swing_04")
FTB_ROW("swing:1", 70, "bstrikebridge")   // KEY
FTB_ROW("swing:1", 100, "s2_swing_05")
FTB_ROW("swing:1", 130, "bretract")   // KEY
FTB_ROW("swing:1", 166.667, "s2_swing_06")
FTB_ROW("swing:1", 200, "s2_swing_08")

FTB_STATE("swig", Time, 850)
FTB_ROW("swig", 0, "bsigload")   // KEY
FTB_ROW("swig", 33.333, "s2_swig_01")
FTB_ROW("swig", 66.667, "s2_swig_02")
FTB_ROW("swig", 100, "s2_swig_03")
FTB_ROW("swig", 116.667, "s2_swig_04")
FTB_ROW("swig", 140, "bsigbridge")   // KEY
FTB_ROW("swig", 166.667, "s2_swig_05")
FTB_ROW("swig", 200, "s2_swig_06")
FTB_ROW("swig", 233.333, "s2_swig_07")
FTB_ROW("swig", 266.667, "s2_swig_08")
FTB_ROW("swig", 300, "swig")   // KEY
FTB_ROW("swig", 333.333, "s2_swig_09")
FTB_ROW("swig", 366.667, "s2_swig_10")
FTB_ROW("swig", 400, "s2_swig_11")
FTB_ROW("swig", 433.333, "s2_swig_10")
FTB_ROW("swig", 466.667, "s2_swig_09")
FTB_ROW("swig", 500, "s2_swig_10")
FTB_ROW("swig", 533.333, "s2_swig_11")
FTB_ROW("swig", 566.667, "s2_swig_10")
FTB_ROW("swig", 600, "s2_swig_09")
FTB_ROW("swig", 633.333, "s2_swig_12")
FTB_ROW("swig", 660, "bsigreach")   // KEY
FTB_ROW("swig", 700, "s2_swig_13")
FTB_ROW("swig", 733.333, "s2_swig_14")
FTB_ROW("swig", 766.667, "s2_swig_15")
FTB_ROW("swig", 800, "s2_swig_16")
FTB_ROW("swig", 816.667, "s2_swig_17")
FTB_ROW("swig", 833.333, "s2_swig_18")

FTB_STATE("breathe", Time, 900)
FTB_ROW("breathe", 0, "breathe")   // KEY
FTB_ROW("breathe", 33.333, "s2_breathe_02")
FTB_ROW("breathe", 66.667, "s2_breathe_03")
FTB_ROW("breathe", 100, "s2_breathe_06")
FTB_ROW("breathe", 133.333, "s2_breathe_04")
FTB_ROW("breathe", 166.667, "breathe")
FTB_ROW("breathe", 200, "s2_breathe_02")
FTB_ROW("breathe", 233.333, "s2_breathe_03")
FTB_ROW("breathe", 266.667, "s2_breathe_06")
FTB_ROW("breathe", 300, "s2_breathe_04")
FTB_ROW("breathe", 333.333, "breathe")
FTB_ROW("breathe", 366.667, "s2_breathe_02")
FTB_ROW("breathe", 400, "s2_breathe_03")
FTB_ROW("breathe", 433.333, "s2_breathe_06")
FTB_ROW("breathe", 466.667, "s2_breathe_04")
FTB_ROW("breathe", 500, "breathe")
FTB_ROW("breathe", 533.333, "s2_breathe_02")
FTB_ROW("breathe", 566.667, "s2_breathe_03")
FTB_ROW("breathe", 600, "s2_breathe_06")
FTB_ROW("breathe", 633.333, "s2_breathe_04")
FTB_ROW("breathe", 666.667, "breathe")
FTB_ROW("breathe", 700, "s2_breathe_02")
FTB_ROW("breathe", 733.333, "s2_breathe_03")
FTB_ROW("breathe", 766.667, "s2_breathe_06")
FTB_ROW("breathe", 800, "s2_breathe_04")
FTB_ROW("breathe", 830, "bsigsettle")   // KEY
FTB_ROW("breathe", 866.667, "s2_breathe_09")
FTB_ROW("breathe", 883.333, "s2_breathe_10")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_05")
FTB_ROW("hurt", 70, "bhitbridge")   // KEY
FTB_ROW("hurt", 100, "s2_hurt_02")
FTB_ROW("hurt", 133.333, "s2_hurt_03")
FTB_ROW("hurt", 166.667, "s2_hurt_04")
FTB_ROW("hurt", 190, "bretract")   // KEY
FTB_ROW("hurt", 216.667, "s2_swing_07")
FTB_ROW("hurt", 233.333, "s2_swing_08")

FTB_STATE("fly", Time, 346.7)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_hurt_05")
FTB_ROW("fly", 66.667, "s2_hurt_07")

FTB_STATE("down", Time, 700)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "bkneel")   // KEY
FTB_ROW("getup", 33.333, "s2_getup_02")
FTB_ROW("getup", 83.333, "s2_getup_03")
FTB_ROW("getup", 133.333, "s2_getup_04")
FTB_ROW("getup", 180, "bidlein")   // KEY
FTB_ROW("getup", 216.667, "s2_getup_01")
FTB_ROW("getup", 250, "s2_swing_07")
FTB_ROW("getup", 283.333, "s2_swing_08")
FTB_ROW("getup", 316.667, "s2_idle_08")
FTB_ROW("getup", 340, "idle")   // KEY
FTB_ROW("getup", 366.667, "s2_idle_01")
FTB_ROW("getup", 400, "s2_idle_02")
FTB_ROW("getup", 433.333, "s2_idle_01")

FTB_STATE("defeat", Time, 420)
FTB_ROW("defeat", 0, "bhitbridge")   // KEY
FTB_ROW("defeat", 33.333, "s2_hurt_01")
FTB_ROW("defeat", 66.667, "s2_hurt_02")
FTB_ROW("defeat", 100, "s2_getup_03")
FTB_ROW("defeat", 120, "bkneel")   // KEY
