// Exposure chart of Pitboss. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// S2: new drawings (s2_*) added between the keys; a frame name without s2_ on a non-KEY row re-shows an existing drawing.

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
FTB_ROW("idle", 100, "s2_idle_01")
FTB_ROW("idle", 150, "s2_idle_02")
FTB_ROW("idle", 200, "s2_idle_03")
FTB_ROW("idle", 250, "s2_idle_04")
FTB_ROW("idle", 300, "bidleout")   // KEY
FTB_ROW("idle", 416.667, "s2_idle_04")
FTB_ROW("idle", 466.667, "s2_idle_03")
FTB_ROW("idle", 516.667, "s2_idle_02")
FTB_ROW("idle", 566.667, "s2_idle_01")
FTB_ROW("idle", 650, "idle")   // KEY
FTB_ROW("idle", 750, "s2_idle_05")
FTB_ROW("idle", 800, "s2_idle_06")
FTB_ROW("idle", 850, "s2_idle_07")
FTB_ROW("idle", 916.667, "s2_idle_08")
FTB_ROW("idle", 1000, "bidlein")   // KEY
FTB_ROW("idle", 1116.667, "s2_idle_07")
FTB_ROW("idle", 1183.333, "s2_idle_06")
FTB_ROW("idle", 1250, "s2_idle_05")
FTB_ROW("idle", 1350, "idle")   // KEY

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 37.879, "s2_walk_01")
FTB_ROW("walk", 75.758, "s2_walk_02")
FTB_ROW("walk", 113.636, "s2_walk_03")
FTB_ROW("walk", 151.515, "bwalkdown")   // KEY
FTB_ROW("walk", 189.394, "s2_walk_04")
FTB_ROW("walk", 227.273, "s2_walk_05")
FTB_ROW("walk", 265.152, "s2_walk_06")
FTB_ROW("walk", 303.03, "bwalkpass")   // KEY
FTB_ROW("walk", 340.909, "s2_walk_07")
FTB_ROW("walk", 378.788, "s2_walk_09")
FTB_ROW("walk", 416.667, "s2_walk_08")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 530.303, "s2_walk_10")
FTB_ROW("walk", 606.061, "bwalklift")   // KEY
FTB_ROW("walk", 643.939, "s2_walk_11")
FTB_ROW("walk", 681.818, "s2_walk_12")
FTB_ROW("walk", 719.697, "s2_walk_03")
FTB_ROW("walk", 757.576, "bwalkpush")   // KEY
FTB_ROW("walk", 795.455, "s2_walk_06")
FTB_ROW("walk", 833.333, "s2_walk_07")
FTB_ROW("walk", 871.212, "s2_walk_09")

FTB_STATE("aim", Time, 550)
FTB_ROW("aim", 0, "bidlein")   // KEY
FTB_ROW("aim", 33.333, "s2_wind_01")
FTB_ROW("aim", 66.667, "s2_wind_02")
FTB_ROW("aim", 100, "s2_wind_03")
FTB_ROW("aim", 133.333, "s2_wind_04")
FTB_ROW("aim", 160, "wind")   // KEY
FTB_ROW("aim", 200, "s2_wind_05")
FTB_ROW("aim", 233.333, "s2_wind_06")
FTB_ROW("aim", 300, "s2_wind_05")
FTB_ROW("aim", 366.667, "s2_wind_06")
FTB_ROW("aim", 433.333, "s2_wind_05")
FTB_ROW("aim", 483.333, "s2_wind_07")
FTB_ROW("aim", 516.667, "s2_wind_08")

FTB_STATE("charge", Loop, 333.333)   // one cycle
FTB_ROW("charge", 0, "walk1")   // KEY
FTB_ROW("charge", 27.778, "s2_walk_02")
FTB_ROW("charge", 55.556, "bwalkdown")   // KEY
FTB_ROW("charge", 83.333, "s2_walk_05")
FTB_ROW("charge", 111.111, "bwalkpass")   // KEY
FTB_ROW("charge", 138.889, "s2_walk_08")
FTB_ROW("charge", 166.667, "walk2")   // KEY
FTB_ROW("charge", 194.444, "s2_walk_11")
FTB_ROW("charge", 222.222, "bwalklift")   // KEY
FTB_ROW("charge", 250, "s2_walk_03")
FTB_ROW("charge", 277.778, "bwalkpush")   // KEY
FTB_ROW("charge", 305.556, "s2_walk_07")

FTB_STATE("wind", Time, 450)
FTB_ROW("wind", 0, "bwindbridge")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_03")
FTB_ROW("wind", 66.667, "s2_wind_04")
FTB_ROW("wind", 100, "wind")   // KEY
FTB_ROW("wind", 133.333, "s2_wind_05")
FTB_ROW("wind", 166.667, "s2_wind_06")
FTB_ROW("wind", 233.333, "s2_wind_05")
FTB_ROW("wind", 300, "s2_wind_06")
FTB_ROW("wind", 383.333, "s2_wind_07")
FTB_ROW("wind", 416.667, "s2_wind_08")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 70, "bstrikebridge")   // KEY
FTB_ROW("swing", 100, "s2_swing_02")
FTB_ROW("swing", 130, "bretract")   // KEY
FTB_ROW("swing", 166.667, "s2_hurt_05")
FTB_ROW("swing", 200, "s2_swing_04")

FTB_STATE("swing:1", Time, 220)
FTB_ROW("swing:1", 0, "bash")   // KEY
FTB_ROW("swing:1", 33.333, "s2_swing1_01")
FTB_ROW("swing:1", 70, "bstrikebridge")   // KEY
FTB_ROW("swing:1", 100, "s2_swing_02")
FTB_ROW("swing:1", 130, "bretract")   // KEY
FTB_ROW("swing:1", 166.667, "s2_hurt_05")
FTB_ROW("swing:1", 200, "s2_swing_04")

FTB_STATE("draw", Time, 1100)
FTB_ROW("draw", 0, "bsigload")   // KEY
FTB_ROW("draw", 33.333, "s2_draw_09")
FTB_ROW("draw", 66.667, "s2_draw_10")
FTB_ROW("draw", 100, "s2_draw_11")
FTB_ROW("draw", 133.333, "s2_draw_12")
FTB_ROW("draw", 160, "bsigbridge")   // KEY
FTB_ROW("draw", 200, "s2_draw_03")
FTB_ROW("draw", 233.333, "s2_draw_04")
FTB_ROW("draw", 266.667, "s2_draw_13")
FTB_ROW("draw", 300, "draw")   // KEY
FTB_ROW("draw", 333.333, "s2_draw_05")
FTB_ROW("draw", 366.667, "s2_draw_14")
FTB_ROW("draw", 400, "s2_draw_06")
FTB_ROW("draw", 420, "bsigreach")   // KEY
FTB_ROW("draw", 450, "s2_draw_15")
FTB_ROW("draw", 483.333, "s2_draw_16")
FTB_ROW("draw", 516.667, "s2_draw_07")
FTB_ROW("draw", 550, "s2_draw_08")
FTB_ROW("draw", 600, "aim")   // KEY
FTB_ROW("draw", 633.333, "s2_fire_01")
FTB_ROW("draw", 666.667, "s2_fire_02")
FTB_ROW("draw", 800, "aim")
FTB_ROW("draw", 933.333, "s2_fire_02")

FTB_STATE("fire", Time, 600)   // from 600 ms the fight shows fire as state "signaturefollow" (same clock)
FTB_ROW("fire", 0, "fire")   // KEY
FTB_ROW("fire", 33.333, "s2_fire_03")
FTB_ROW("fire", 50, "s2_fire_04")
FTB_ROW("fire", 70, "bsigsettle")   // KEY
FTB_ROW("fire", 100, "s2_fire_05")
FTB_ROW("fire", 116.667, "s2_fire_01")
FTB_ROW("fire", 140, "aim")   // KEY
FTB_ROW("fire", 166.667, "s2_fire_02")
FTB_ROW("fire", 220, "fire")   // KEY
FTB_ROW("fire", 250, "s2_fire_03")
FTB_ROW("fire", 266.667, "s2_fire_04")
FTB_ROW("fire", 291, "bsigsettle")   // KEY
FTB_ROW("fire", 316.667, "s2_fire_05")
FTB_ROW("fire", 333.333, "s2_fire_01")
FTB_ROW("fire", 360, "aim")   // KEY
FTB_ROW("fire", 400, "s2_fire_02")
FTB_ROW("fire", 440, "fire")   // KEY
FTB_ROW("fire", 466.667, "s2_fire_03")
FTB_ROW("fire", 483.333, "s2_fire_04")
FTB_ROW("fire", 511, "bsigsettle")   // KEY
FTB_ROW("fire", 533.333, "s2_fire_05")
FTB_ROW("fire", 550, "s2_fire_01")
FTB_ROW("fire", 581, "aim")   // KEY

FTB_STATE("signaturefollow", Time, 900)   // the end of fire: times on fire's clock
FTB_ROW("signaturefollow", 0, "bsigsettle")   // KEY
FTB_ROW("signaturefollow", 633.333, "s2_fire_05")
FTB_ROW("signaturefollow", 666.667, "s2_fire_01")
FTB_ROW("signaturefollow", 700, "s2_fire_02")
FTB_ROW("signaturefollow", 720, "signaturefollow")   // KEY
FTB_ROW("signaturefollow", 750, "s2_signaturefollow_04")
FTB_ROW("signaturefollow", 783.333, "s2_signaturefollow_01")
FTB_ROW("signaturefollow", 816.667, "s2_signaturefollow_02")
FTB_ROW("signaturefollow", 850, "s2_signaturefollow_03")
FTB_ROW("signaturefollow", 866.667, "s2_signaturefollow_05")
FTB_ROW("signaturefollow", 883.333, "s2_signaturefollow_06")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 70, "bhitbridge")   // KEY
FTB_ROW("hurt", 100, "s2_hurt_02")
FTB_ROW("hurt", 133.333, "s2_hurt_03")
FTB_ROW("hurt", 166.667, "s2_hurt_04")
FTB_ROW("hurt", 190, "bretract")   // KEY
FTB_ROW("hurt", 216.667, "s2_hurt_05")
FTB_ROW("hurt", 233.333, "s2_swing_04")

FTB_STATE("fly", Time, 346.7)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 50, "s2_hurt_01")
FTB_ROW("fly", 100, "s2_fly_03")
FTB_ROW("fly", 166.667, "s2_fly_01")

FTB_STATE("down", Time, 700)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "bkneel")   // KEY
FTB_ROW("getup", 33.333, "s2_getup_01")
FTB_ROW("getup", 50, "s2_getup_02")
FTB_ROW("getup", 83.333, "s2_getup_03")
FTB_ROW("getup", 116.667, "s2_getup_04")
FTB_ROW("getup", 150, "s2_getup_05")
FTB_ROW("getup", 180, "bidlein")   // KEY
FTB_ROW("getup", 216.667, "s2_getup_06")
FTB_ROW("getup", 250, "s2_swing_04")
FTB_ROW("getup", 283.333, "s2_hurt_05")
FTB_ROW("getup", 316.667, "s2_idle_06")
FTB_ROW("getup", 340, "idle")   // KEY

FTB_STATE("defeat", Time, 420)
FTB_ROW("defeat", 0, "bhitbridge")   // KEY
FTB_ROW("defeat", 33.333, "s2_defeat_01")
FTB_ROW("defeat", 66.667, "s2_defeat_02")
FTB_ROW("defeat", 100, "s2_defeat_03")
FTB_ROW("defeat", 120, "bkneel")   // KEY
