// Exposure chart of Vinnie. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// Stage 2: rows without KEY were added by the art pass (s2_* = new drawings; other names = existing drawings shown again).

FTB_STATE("intro", Time, 2460)
FTB_ROW("intro", 0, "bidlein")   // KEY
FTB_ROW("intro", 33.333, "s2_whirltell_02")
FTB_ROW("intro", 66.667, "s2_whirltell_03")
FTB_ROW("intro", 100, "s2_whirltell_04")
FTB_ROW("intro", 133.333, "s2_whirltell_06")
FTB_ROW("intro", 160, "intro")   // KEY
FTB_ROW("intro", 200, "s2_whirltell_05")
FTB_ROW("intro", 233.333, "s2_whirltell_06")
FTB_ROW("intro", 266.667, "intro")
FTB_ROW("intro", 366.667, "s2_whirltell_07")
FTB_ROW("intro", 400, "bentrance")   // KEY
FTB_ROW("intro", 1800, "bidleout")   // KEY
FTB_ROW("intro", 2060, "intro2")   // KEY

FTB_STATE("intro2", Time, 2820)
FTB_ROW("intro2", 2460, "bidlein")   // KEY
FTB_ROW("intro2", 2500, "s2_idle_03")
FTB_ROW("intro2", 2533.333, "s2_intro2_01")
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
FTB_ROW("walk", 30.303, "s2_walk_09")
FTB_ROW("walk", 60.606, "s2_walk_18")
FTB_ROW("walk", 90.909, "s2_walk_01")
FTB_ROW("walk", 121.212, "s2_walk_10")
FTB_ROW("walk", 151.515, "bwalkdown")   // KEY
FTB_ROW("walk", 189.394, "s2_walk_11")
FTB_ROW("walk", 227.273, "s2_walk_02")
FTB_ROW("walk", 265.152, "s2_walk_12")
FTB_ROW("walk", 303.03, "bwalkpass")   // KEY
FTB_ROW("walk", 340.909, "s2_walk_13")
FTB_ROW("walk", 378.788, "s2_walk_03")
FTB_ROW("walk", 416.667, "s2_walk_17")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 505.051, "s2_walk_05")
FTB_ROW("walk", 555.556, "s2_walk_06")
FTB_ROW("walk", 606.061, "bwalklift")   // KEY
FTB_ROW("walk", 656.566, "s2_walk_16")
FTB_ROW("walk", 707.071, "s2_walk_07")
FTB_ROW("walk", 757.576, "bwalkpush")   // KEY
FTB_ROW("walk", 795.455, "s2_walk_19")
FTB_ROW("walk", 833.333, "s2_walk_08")
FTB_ROW("walk", 871.212, "s2_walk_20")

FTB_STATE("aim", Time, 550)
FTB_ROW("aim", 0, "bidlein")   // KEY
FTB_ROW("aim", 33.333, "bwindbridge")
FTB_ROW("aim", 66.667, "s2_wind_01")
FTB_ROW("aim", 100, "s2_wind_02")
FTB_ROW("aim", 133.333, "s2_wind_05")
FTB_ROW("aim", 160, "wind")   // KEY
FTB_ROW("aim", 200, "s2_wind_03")
FTB_ROW("aim", 233.333, "wind")
FTB_ROW("aim", 266.667, "s2_wind_05")
FTB_ROW("aim", 300, "wind")
FTB_ROW("aim", 333.333, "s2_wind_04")
FTB_ROW("aim", 516.667, "s2_wind_06")

FTB_STATE("charge", Loop, 333.333)   // one cycle
FTB_ROW("charge", 0, "walk1")   // KEY
FTB_ROW("charge", 27.778, "s2_walk_01")
FTB_ROW("charge", 55.556, "bwalkdown")   // KEY
FTB_ROW("charge", 83.333, "s2_walk_02")
FTB_ROW("charge", 111.111, "bwalkpass")   // KEY
FTB_ROW("charge", 138.889, "s2_walk_03")
FTB_ROW("charge", 166.667, "walk2")   // KEY
FTB_ROW("charge", 194.444, "s2_walk_05")
FTB_ROW("charge", 222.222, "bwalklift")   // KEY
FTB_ROW("charge", 250, "s2_walk_07")
FTB_ROW("charge", 277.778, "bwalkpush")   // KEY
FTB_ROW("charge", 305.556, "s2_walk_08")

FTB_STATE("wind", Time, 420)
FTB_ROW("wind", 0, "bwindbridge")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 66.667, "s2_wind_02")
FTB_ROW("wind", 100, "wind")   // KEY
FTB_ROW("wind", 133.333, "s2_wind_03")
FTB_ROW("wind", 166.667, "wind")
FTB_ROW("wind", 216.667, "s2_wind_07")
FTB_ROW("wind", 266.667, "s2_wind_04")
FTB_ROW("wind", 325, "s2_wind_08")
FTB_ROW("wind", 383.333, "s2_wind_06")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 70, "bstrikebridge")   // KEY
FTB_ROW("swing", 100, "s2_swing_02")
FTB_ROW("swing", 130, "bretract")   // KEY
FTB_ROW("swing", 166.667, "s2_swing_03")
FTB_ROW("swing", 200, "s2_swing_05")

FTB_STATE("swing:1", Time, 220)
FTB_ROW("swing:1", 0, "kick")   // KEY
FTB_ROW("swing:1", 33.333, "s2_swing1_01")
FTB_ROW("swing:1", 50, "s2_swing1_02")
FTB_ROW("swing:1", 70, "bstrikebridge")   // KEY
FTB_ROW("swing:1", 100, "s2_swing_02")
FTB_ROW("swing:1", 130, "bretract")   // KEY
FTB_ROW("swing:1", 166.667, "s2_swing_04")
FTB_ROW("swing:1", 200, "s2_swing_05")

FTB_STATE("whirltell", Time, 850)
FTB_ROW("whirltell", 0, "bsigload")   // KEY
FTB_ROW("whirltell", 33.333, "s2_whirltell_01")
FTB_ROW("whirltell", 66.667, "s2_whirltell_02")
FTB_ROW("whirltell", 100, "s2_whirltell_03")
FTB_ROW("whirltell", 133.333, "s2_whirltell_04")
FTB_ROW("whirltell", 150, "taunt")   // KEY
FTB_ROW("whirltell", 183.333, "s2_whirltell_05")
FTB_ROW("whirltell", 216.667, "s2_whirltell_06")
FTB_ROW("whirltell", 250, "s2_whirltell_04")
FTB_ROW("whirltell", 283.333, "taunt")
FTB_ROW("whirltell", 433.333, "s2_whirltell_06")
FTB_ROW("whirltell", 466.667, "s2_whirltell_07")
FTB_ROW("whirltell", 500, "s2_whirltell_08")
FTB_ROW("whirltell", 550, "s2_whirltell_09")
FTB_ROW("whirltell", 580, "bsigbridge")   // KEY
FTB_ROW("whirltell", 633.333, "s2_whirl_02")
FTB_ROW("whirltell", 666.667, "spinb")
FTB_ROW("whirltell", 700, "s2_whirl_03")
FTB_ROW("whirltell", 720, "bsigreach")   // KEY
FTB_ROW("whirltell", 750, "s2_whirltell_10")
FTB_ROW("whirltell", 783.333, "bsigreach")
FTB_ROW("whirltell", 816.667, "s2_whirl_04")

FTB_STATE("whirl", Loop, 180)
FTB_ROW("whirl", 0, "spin")   // KEY
FTB_ROW("whirl", 22.5, "s2_whirl_01")
FTB_ROW("whirl", 45, "bsigbridge")   // KEY
FTB_ROW("whirl", 67.5, "s2_whirl_02")
FTB_ROW("whirl", 90, "spinb")   // KEY
FTB_ROW("whirl", 112.5, "s2_whirl_03")
FTB_ROW("whirl", 135, "bsigreach")   // KEY
FTB_ROW("whirl", 157.5, "s2_whirl_04")

FTB_STATE("signaturefollow", Time, 350)   // the fight shows the first 350 ms of dizzy as state "signaturefollow" (same clock), then "dizzy"
FTB_ROW("signaturefollow", 0, "bsigsettle")   // KEY
FTB_ROW("signaturefollow", 33.333, "s2_signaturefollow_01")
FTB_ROW("signaturefollow", 66.667, "s2_signaturefollow_02")
FTB_ROW("signaturefollow", 100, "s2_signaturefollow_03")
FTB_ROW("signaturefollow", 133.333, "s2_signaturefollow_04")
FTB_ROW("signaturefollow", 150, "s2_signaturefollow_05")
FTB_ROW("signaturefollow", 180, "signaturefollow")   // KEY
FTB_ROW("signaturefollow", 216.667, "s2_dizzy_02")
FTB_ROW("signaturefollow", 250, "s2_dizzy_03")
FTB_ROW("signaturefollow", 283.333, "s2_dizzy_05")
FTB_ROW("signaturefollow", 316.667, "s2_dizzy_07")

FTB_STATE("dizzy", Loop, 500)   // the fight shows the first 350 ms of dizzy as state "signaturefollow" (same clock), then "dizzy"
FTB_ROW("dizzy", 0, "dizzy")   // KEY
FTB_ROW("dizzy", 33.333, "s2_dizzy_01")
FTB_ROW("dizzy", 66.667, "s2_dizzy_02")
FTB_ROW("dizzy", 100, "s2_dizzy_03")
FTB_ROW("dizzy", 133.333, "s2_dizzy_04")
FTB_ROW("dizzy", 166.667, "s2_dizzy_05")
FTB_ROW("dizzy", 200, "s2_dizzy_06")
FTB_ROW("dizzy", 233.333, "s2_dizzy_07")
FTB_ROW("dizzy", 250, "bhitbridge")   // KEY
FTB_ROW("dizzy", 283.333, "s2_dizzy_07")
FTB_ROW("dizzy", 316.667, "s2_dizzy_06")
FTB_ROW("dizzy", 350, "bhitbridge")
FTB_ROW("dizzy", 383.333, "s2_dizzy_06")
FTB_ROW("dizzy", 416.667, "s2_dizzy_05")
FTB_ROW("dizzy", 450, "s2_dizzy_03")
FTB_ROW("dizzy", 483.333, "s2_dizzy_01")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 70, "bhitbridge")   // KEY
FTB_ROW("hurt", 100, "s2_hurt_02")
FTB_ROW("hurt", 133.333, "s2_hurt_03")
FTB_ROW("hurt", 166.667, "s2_hurt_04")
FTB_ROW("hurt", 190, "bretract")   // KEY
FTB_ROW("hurt", 233.333, "s2_swing_05")

FTB_STATE("fly", Time, 346.7)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 50, "s2_fly_01")

FTB_STATE("down", Time, 700)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "bkneel")   // KEY
FTB_ROW("getup", 33.333, "s2_getup_01")
FTB_ROW("getup", 66.667, "s2_getup_02")
FTB_ROW("getup", 100, "s2_getup_03")
FTB_ROW("getup", 150, "s2_getup_04")
FTB_ROW("getup", 180, "bidlein")   // KEY
FTB_ROW("getup", 216.667, "s2_getup_05")
FTB_ROW("getup", 250, "s2_idle_02")
FTB_ROW("getup", 283.333, "s2_idle_01")
FTB_ROW("getup", 340, "idle")   // KEY

FTB_STATE("defeat", Time, 420)
FTB_ROW("defeat", 0, "bhitbridge")   // KEY
FTB_ROW("defeat", 33.333, "s2_getup_04")
FTB_ROW("defeat", 66.667, "s2_getup_03")
FTB_ROW("defeat", 100, "s2_defeat_01")
FTB_ROW("defeat", 120, "bkneel")   // KEY
FTB_ROW("defeat", 200, "s2_defeat_02")
