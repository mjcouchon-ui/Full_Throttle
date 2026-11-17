// Exposure chart of Kingpin. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings (s2_*) go between them; stage 2 art, see manifest.json beside the frames. Times are ms; in a Walk or Loop state they are positions in the cycle.

FTB_STATE("intro", Time, 2460)
FTB_ROW("intro", 0, "bidlein")   // KEY
FTB_ROW("intro", 33.333, "s2_idle_07")
FTB_ROW("intro", 66.667, "s2_idle_05")
FTB_ROW("intro", 100, "s2_idle_06")
FTB_ROW("intro", 133.333, "s2_idle_04")
FTB_ROW("intro", 160, "intro")   // KEY
FTB_ROW("intro", 400, "bentrance")   // KEY
FTB_ROW("intro", 1800, "bidleout")   // KEY
FTB_ROW("intro", 2060, "intro2")   // KEY

FTB_STATE("intro2", Time, 2820)
FTB_ROW("intro2", 2460, "bidlein")   // KEY
FTB_ROW("intro2", 2496, "s2_idle_08")
FTB_ROW("intro2", 2532, "s2_idle_07")
FTB_ROW("intro2", 2568, "s2_swing_05")
FTB_ROW("intro2", 2604, "s2_idle_05")
FTB_ROW("intro2", 2640, "idle")   // KEY

FTB_STATE("idle", Loop, 1600)   // one cycle
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 100, "s2_idle_05")
FTB_ROW("idle", 200, "s2_idle_06")
FTB_ROW("idle", 300, "bidleout")   // KEY
FTB_ROW("idle", 416.667, "s2_idle_06")
FTB_ROW("idle", 533.333, "s2_idle_05")
FTB_ROW("idle", 650, "idle")   // KEY
FTB_ROW("idle", 766.667, "s2_idle_07")
FTB_ROW("idle", 883.333, "s2_idle_08")
FTB_ROW("idle", 1000, "bidlein")   // KEY
FTB_ROW("idle", 1116.667, "s2_idle_08")
FTB_ROW("idle", 1233.333, "s2_idle_07")
FTB_ROW("idle", 1350, "idle")   // KEY

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 30.303, "s2_walk_26")
FTB_ROW("walk", 60.606, "s2_walk_19")
FTB_ROW("walk", 90.909, "s2_walk_20")
FTB_ROW("walk", 121.212, "s2_walk_21")
FTB_ROW("walk", 151.515, "bwalkdown")   // KEY
FTB_ROW("walk", 189.394, "s2_walk_22")
FTB_ROW("walk", 227.273, "s2_walk_04")
FTB_ROW("walk", 265.152, "s2_walk_05")
FTB_ROW("walk", 303.03, "bwalkpass")   // KEY
FTB_ROW("walk", 333.333, "s2_walk_06")
FTB_ROW("walk", 363.636, "s2_walk_07")
FTB_ROW("walk", 393.939, "s2_walk_08")
FTB_ROW("walk", 424.242, "s2_walk_09")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 492.424, "s2_walk_10")
FTB_ROW("walk", 530.303, "s2_walk_23")
FTB_ROW("walk", 568.182, "s2_walk_11")
FTB_ROW("walk", 606.061, "bwalklift")   // KEY
FTB_ROW("walk", 656.561, "s2_walk_24")
FTB_ROW("walk", 707.076, "s2_walk_25")
FTB_ROW("walk", 757.576, "bwalkpush")   // KEY
FTB_ROW("walk", 782.833, "s2_walk_13")
FTB_ROW("walk", 808.076, "s2_walk_14")
FTB_ROW("walk", 833.333, "s2_walk_15")
FTB_ROW("walk", 858.591, "s2_walk_16")
FTB_ROW("walk", 883.833, "s2_walk_17")

FTB_STATE("aim", Time, 550)
FTB_ROW("aim", 0, "bidlein")   // KEY
FTB_ROW("aim", 33.333, "s2_idle_07")
FTB_ROW("aim", 66.667, "s2_wind_01")
FTB_ROW("aim", 100, "s2_wind_02")
FTB_ROW("aim", 133.333, "bwindbridge")
FTB_ROW("aim", 160, "wind")   // KEY
FTB_ROW("aim", 200, "s2_wind_03")
FTB_ROW("aim", 233.333, "s2_wind_05")
FTB_ROW("aim", 300, "s2_wind_03")
FTB_ROW("aim", 333.333, "s2_wind_06")
FTB_ROW("aim", 366.667, "s2_wind_03")
FTB_ROW("aim", 400, "s2_wind_07")
FTB_ROW("aim", 466.667, "s2_wind_06")
FTB_ROW("aim", 500, "s2_wind_08")
FTB_ROW("aim", 533.333, "s2_wind_09")

FTB_STATE("charge", Loop, 333.333)   // one cycle
FTB_ROW("charge", 0, "walk1")   // KEY
FTB_ROW("charge", 27.778, "s2_walk_20")
FTB_ROW("charge", 55.556, "bwalkdown")   // KEY
FTB_ROW("charge", 83.333, "s2_walk_04")
FTB_ROW("charge", 111.111, "bwalkpass")   // KEY
FTB_ROW("charge", 138.889, "s2_walk_07")
FTB_ROW("charge", 166.667, "walk2")   // KEY
FTB_ROW("charge", 194.444, "s2_walk_10")
FTB_ROW("charge", 222.222, "bwalklift")   // KEY
FTB_ROW("charge", 250, "s2_walk_24")
FTB_ROW("charge", 277.778, "bwalkpush")   // KEY
FTB_ROW("charge", 305.555, "s2_walk_15")

FTB_STATE("wind", Time, 450)
FTB_ROW("wind", 0, "bwindbridge")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_06")
FTB_ROW("wind", 66.667, "s2_wind_07")
FTB_ROW("wind", 100, "wind")   // KEY
FTB_ROW("wind", 133.333, "s2_wind_03")
FTB_ROW("wind", 200, "s2_wind_10")
FTB_ROW("wind", 266.667, "s2_wind_06")
FTB_ROW("wind", 300, "s2_wind_07")
FTB_ROW("wind", 366.667, "s2_wind_06")
FTB_ROW("wind", 400, "s2_wind_08")
FTB_ROW("wind", 433.333, "s2_wind_09")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_02")
FTB_ROW("swing", 70, "bstrikebridge")   // KEY
FTB_ROW("swing", 100, "s2_swing_03")
FTB_ROW("swing", 130, "bretract")   // KEY
FTB_ROW("swing", 166.667, "s2_swing_04")
FTB_ROW("swing", 200, "s2_swing_05")

FTB_STATE("swing:1", Time, 220)
FTB_ROW("swing:1", 0, "bash")   // KEY
FTB_ROW("swing:1", 33.333, "s2_swing_06")
FTB_ROW("swing:1", 50, "s2_swing_07")
FTB_ROW("swing:1", 70, "bstrikebridge")   // KEY
FTB_ROW("swing:1", 100, "s2_swing_03")
FTB_ROW("swing:1", 130, "bretract")   // KEY
FTB_ROW("swing:1", 166.667, "s2_swing_04")
FTB_ROW("swing:1", 200, "s2_swing_05")

FTB_STATE("hold", Time, 1000)
FTB_ROW("hold", 0, "bsigload")   // KEY
FTB_ROW("hold", 33.333, "s2_hold_01")
FTB_ROW("hold", 100, "s2_hold_02")
FTB_ROW("hold", 150, "s2_hold_03")
FTB_ROW("hold", 180, "hold")   // KEY
FTB_ROW("hold", 216.667, "s2_hold_04")
FTB_ROW("hold", 250, "s2_hold_13")
FTB_ROW("hold", 283.333, "s2_hold_05")
FTB_ROW("hold", 316.667, "s2_hold_21")
FTB_ROW("hold", 383.333, "s2_hold_06")
FTB_ROW("hold", 450, "s2_hold_09")
FTB_ROW("hold", 483.333, "s2_hold_17")
FTB_ROW("hold", 516.667, "s2_hold_07")
FTB_ROW("hold", 550, "s2_hold_08")
FTB_ROW("hold", 566.667, "s2_hold_10")
FTB_ROW("hold", 583.333, "s2_hold_11")
FTB_ROW("hold", 600, "bsigbridge")   // KEY
FTB_ROW("hold", 633.333, "s2_hold_12")
FTB_ROW("hold", 666.667, "s2_hold_14")
FTB_ROW("hold", 700, "s2_hold_15")
FTB_ROW("hold", 733.333, "s2_hold_16")
FTB_ROW("hold", 766.667, "s2_hold_18")
FTB_ROW("hold", 800, "s2_hold_19")
FTB_ROW("hold", 820, "bsigreach")   // KEY
FTB_ROW("hold", 855, "s2_hold_20")
FTB_ROW("hold", 890, "s2_hold_22")
FTB_ROW("hold", 925, "s2_hold_23")
FTB_ROW("hold", 960, "s2_hold_24")

FTB_STATE("bowl", Time, 700)
FTB_ROW("bowl", 0, "bowl")   // KEY
FTB_ROW("bowl", 33.333, "s2_bowl_01")
FTB_ROW("bowl", 66.667, "s2_bowl_02")
FTB_ROW("bowl", 100, "s2_bowl_03")
FTB_ROW("bowl", 133.333, "s2_bowl_04")
FTB_ROW("bowl", 183.333, "s2_bowl_05")
FTB_ROW("bowl", 216.667, "s2_bowl_06")
FTB_ROW("bowl", 250, "s2_bowl_07")
FTB_ROW("bowl", 283.333, "s2_bowl_14")
FTB_ROW("bowl", 310, "bsigsettle")   // KEY
FTB_ROW("bowl", 350, "s2_bowl_08")
FTB_ROW("bowl", 383.333, "s2_bowl_09")
FTB_ROW("bowl", 416.667, "s2_bowl_10")
FTB_ROW("bowl", 450, "s2_bowl_15")
FTB_ROW("bowl", 480, "signaturefollow")   // KEY
FTB_ROW("bowl", 516.667, "s2_bowl_11")
FTB_ROW("bowl", 550, "s2_bowl_12")
FTB_ROW("bowl", 583.333, "s2_bowl_13")
FTB_ROW("bowl", 600, "bidlein")   // KEY
FTB_ROW("bowl", 633.333, "s2_idle_08")
FTB_ROW("bowl", 666.667, "s2_idle_07")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 50, "s2_hurt_02")
FTB_ROW("hurt", 70, "bhitbridge")   // KEY
FTB_ROW("hurt", 100, "s2_hurt_03")
FTB_ROW("hurt", 133.333, "s2_hurt_04")
FTB_ROW("hurt", 166.667, "s2_hurt_05")
FTB_ROW("hurt", 190, "bretract")   // KEY
FTB_ROW("hurt", 225, "s2_hurt_06")

FTB_STATE("fly", Time, 346.7)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.333, "s2_hurt_01")
FTB_ROW("fly", 66.667, "s2_fly_01")
FTB_ROW("fly", 133.333, "s2_fly_04")
FTB_ROW("fly", 200, "s2_fly_01")
FTB_ROW("fly", 266.667, "s2_fly_05")
FTB_ROW("fly", 313.333, "s2_fly_01")

FTB_STATE("down", Time, 700)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "bkneel")   // KEY
FTB_ROW("getup", 33.333, "s2_getup_03")
FTB_ROW("getup", 66.667, "s2_getup_04")
FTB_ROW("getup", 100, "s2_getup_05")
FTB_ROW("getup", 140, "s2_bowl_13")
FTB_ROW("getup", 180, "bidlein")   // KEY
FTB_ROW("getup", 216.667, "s2_idle_08")
FTB_ROW("getup", 250, "s2_idle_07")
FTB_ROW("getup", 283.333, "s2_swing_05")
FTB_ROW("getup", 316.667, "s2_idle_05")
FTB_ROW("getup", 340, "idle")   // KEY
FTB_ROW("getup", 380, "s2_swing_04")
FTB_ROW("getup", 416.667, "s2_hurt_06")

FTB_STATE("defeat", Time, 420)
FTB_ROW("defeat", 0, "bhitbridge")   // KEY
FTB_ROW("defeat", 33.333, "s2_defeat_01")
FTB_ROW("defeat", 66.667, "s2_defeat_02")
FTB_ROW("defeat", 100, "s2_getup_03")
FTB_ROW("defeat", 120, "bkneel")   // KEY
