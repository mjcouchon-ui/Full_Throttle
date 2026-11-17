// Exposure chart of Earl. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.

// S2 art pass: rows without KEY are new drawings (T_earl_s2_*). In Time states they sit on fight steps (n/60 s),
// written 0.3 ms early so that a rounded time can never fall just after its step.

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
FTB_ROW("idle", 75, "s2_idle_14")
FTB_ROW("idle", 150, "s2_idle_15")
FTB_ROW("idle", 225, "s2_idle_16")
FTB_ROW("idle", 300, "bidleout")   // KEY
FTB_ROW("idle", 387.5, "s2_idle_17")
FTB_ROW("idle", 475, "s2_idle_15")
FTB_ROW("idle", 562.5, "s2_idle_14")
FTB_ROW("idle", 650, "idle")   // KEY
FTB_ROW("idle", 766.667, "s2_idle_18")
FTB_ROW("idle", 883.333, "s2_idle_19")
FTB_ROW("idle", 1000, "bidlein")   // KEY
FTB_ROW("idle", 1116.667, "s2_idle_18")
FTB_ROW("idle", 1233.333, "s2_idle_20")
FTB_ROW("idle", 1350, "idle")   // KEY

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 75.758, "s2_walk_01")
FTB_ROW("walk", 151.515, "bwalkdown")   // KEY
FTB_ROW("walk", 202.02, "s2_walk_02")
FTB_ROW("walk", 252.525, "s2_walk_07")
FTB_ROW("walk", 303.03, "bwalkpass")   // KEY
FTB_ROW("walk", 353.535, "s2_walk_03")
FTB_ROW("walk", 404.04, "s2_walk_04")
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 530.303, "s2_walk_06")
FTB_ROW("walk", 606.061, "bwalklift")   // KEY
FTB_ROW("walk", 656.566, "s2_walk_01")
FTB_ROW("walk", 707.071, "s2_walk_02")
FTB_ROW("walk", 757.576, "bwalkpush")   // KEY
FTB_ROW("walk", 808.081, "s2_walk_08")
FTB_ROW("walk", 858.586, "s2_walk_05")

FTB_STATE("aim", Time, 550)
FTB_ROW("aim", 0, "bidlein")   // KEY
FTB_ROW("aim", 33.033, "s2_idle_20")
FTB_ROW("aim", 66.367, "s2_wind_01")
FTB_ROW("aim", 99.7, "s2_wind_02")
FTB_ROW("aim", 133.033, "s2_wind_05")
FTB_ROW("aim", 160, "wind")   // KEY
FTB_ROW("aim", 199.7, "s2_wind_03")
FTB_ROW("aim", 233.033, "s2_wind_04")
FTB_ROW("aim", 266.367, "s2_wind_05")
FTB_ROW("aim", 299.7, "s2_wind_03")
FTB_ROW("aim", 333.033, "s2_wind_04")
FTB_ROW("aim", 366.367, "s2_wind_05")
FTB_ROW("aim", 399.7, "s2_wind_03")
FTB_ROW("aim", 433.033, "s2_wind_04")
FTB_ROW("aim", 466.367, "s2_wind_05")
FTB_ROW("aim", 499.7, "s2_wind_06")
FTB_ROW("aim", 533.033, "s2_wind_07")

FTB_STATE("charge", Loop, 333.333)   // one cycle
FTB_ROW("charge", 0, "walk1")   // KEY
FTB_ROW("charge", 27.778, "s2_walk_01")
FTB_ROW("charge", 55.556, "bwalkdown")   // KEY
FTB_ROW("charge", 83.333, "s2_walk_02")
FTB_ROW("charge", 111.111, "bwalkpass")   // KEY
FTB_ROW("charge", 138.889, "s2_walk_04")
FTB_ROW("charge", 166.667, "walk2")   // KEY
FTB_ROW("charge", 194.444, "s2_walk_06")
FTB_ROW("charge", 222.222, "bwalklift")   // KEY
FTB_ROW("charge", 250, "s2_walk_01")
FTB_ROW("charge", 277.778, "bwalkpush")   // KEY
FTB_ROW("charge", 305.556, "s2_walk_08")

FTB_STATE("wind", Time, 420)
FTB_ROW("wind", 0, "bwindbridge")   // KEY
FTB_ROW("wind", 33.033, "s2_wind_01")
FTB_ROW("wind", 66.367, "s2_wind_02")
FTB_ROW("wind", 100, "wind")   // KEY
FTB_ROW("wind", 133.033, "s2_wind_03")
FTB_ROW("wind", 166.367, "s2_wind_04")
FTB_ROW("wind", 199.7, "s2_wind_05")
FTB_ROW("wind", 233.033, "s2_wind_03")
FTB_ROW("wind", 266.367, "s2_wind_04")
FTB_ROW("wind", 299.7, "s2_wind_05")
FTB_ROW("wind", 333.033, "s2_wind_03")
FTB_ROW("wind", 366.367, "s2_wind_06")
FTB_ROW("wind", 399.7, "s2_wind_07")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 33.033, "s2_swing_01")
FTB_ROW("swing", 50, "s2_swing_02")
FTB_ROW("swing", 70, "bstrikebridge")   // KEY
FTB_ROW("swing", 99.7, "s2_swing_03")
FTB_ROW("swing", 130, "bretract")   // KEY
FTB_ROW("swing", 166.367, "s2_idle_18")
FTB_ROW("swing", 199.7, "s2_idle_20")

FTB_STATE("swing:1", Time, 220)
FTB_ROW("swing:1", 0, "punch")   // KEY
FTB_ROW("swing:1", 33.033, "s2_swing_01")
FTB_ROW("swing:1", 50, "s2_swing_02")
FTB_ROW("swing:1", 70, "bstrikebridge")   // KEY
FTB_ROW("swing:1", 99.7, "s2_swing_03")
FTB_ROW("swing:1", 130, "bretract")   // KEY
FTB_ROW("swing:1", 166.367, "s2_idle_18")
FTB_ROW("swing:1", 199.7, "s2_idle_20")

FTB_STATE("slamwind", Time, 550)
FTB_ROW("slamwind", 0, "bsigload")   // KEY
FTB_ROW("slamwind", 33.033, "s2_slamwind_01")
FTB_ROW("slamwind", 66.367, "s2_slamwind_02")
FTB_ROW("slamwind", 100, "signatureprep")   // KEY
FTB_ROW("slamwind", 133.033, "s2_slamwind_03")
FTB_ROW("slamwind", 183.033, "s2_slamwind_05")
FTB_ROW("slamwind", 220, "bsigbridge")   // KEY
FTB_ROW("slamwind", 249.7, "s2_slamwind_07")
FTB_ROW("slamwind", 283.033, "s2_slamwind_11")
FTB_ROW("slamwind", 316.367, "s2_slamwind_08")
FTB_ROW("slamwind", 360, "jump")   // KEY
FTB_ROW("slamwind", 399.7, "s2_slamwind_09")
FTB_ROW("slamwind", 433.033, "s2_slamwind_15")
FTB_ROW("slamwind", 460, "bsigreach")   // KEY
FTB_ROW("slamwind", 483.033, "s2_slamwind_10")
FTB_ROW("slamwind", 499.7, "s2_slamwind_12")
FTB_ROW("slamwind", 533.033, "s2_slamwind_13")

FTB_STATE("slam", Time, 750)
FTB_ROW("slam", 0, "slam")   // KEY
FTB_ROW("slam", 33.033, "s2_slam_01")
FTB_ROW("slam", 66.367, "s2_slam_02")
FTB_ROW("slam", 99.7, "s2_slam_03")
FTB_ROW("slam", 133.033, "s2_slam_04")
FTB_ROW("slam", 160, "bsigsettle")   // KEY
FTB_ROW("slam", 199.7, "s2_slam_05")
FTB_ROW("slam", 233.033, "s2_slam_06")
FTB_ROW("slam", 266.367, "s2_slam_07")
FTB_ROW("slam", 299.7, "s2_slam_08")
FTB_ROW("slam", 320, "signaturefollow")   // KEY
FTB_ROW("slam", 366.367, "s2_slam_09")
FTB_ROW("slam", 399.7, "s2_slam_10")
FTB_ROW("slam", 433.033, "s2_slam_11")
FTB_ROW("slam", 466.367, "s2_slam_12")
FTB_ROW("slam", 499.7, "s2_slam_13")
FTB_ROW("slam", 520, "bidlein")   // KEY
FTB_ROW("slam", 549.7, "s2_idle_18")
FTB_ROW("slam", 583.033, "s2_idle_20")
FTB_ROW("slam", 616.367, "s2_idle_14")
FTB_ROW("slam", 649.7, "s2_idle_15")
FTB_ROW("slam", 699.7, "s2_idle_14")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.033, "s2_hurt_01")
FTB_ROW("hurt", 50, "s2_hurt_02")
FTB_ROW("hurt", 70, "bhitbridge")   // KEY
FTB_ROW("hurt", 99.7, "s2_hurt_03")
FTB_ROW("hurt", 133.033, "s2_hurt_04")
FTB_ROW("hurt", 166.367, "s2_hurt_05")
FTB_ROW("hurt", 190, "bretract")   // KEY
FTB_ROW("hurt", 233.033, "s2_idle_20")

FTB_STATE("fly", Time, 346.7)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 33.033, "s2_hurt_01")
FTB_ROW("fly", 66.367, "s2_hurt_02")
FTB_ROW("fly", 99.7, "s2_fly_01")
FTB_ROW("fly", 183.033, "s2_fly_02")

FTB_STATE("down", Time, 700)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "bkneel")   // KEY
FTB_ROW("getup", 149.7, "s2_slam_13")
FTB_ROW("getup", 180, "bidlein")   // KEY
FTB_ROW("getup", 233.033, "s2_idle_18")
FTB_ROW("getup", 283.033, "s2_idle_20")
FTB_ROW("getup", 340, "idle")   // KEY
FTB_ROW("getup", 399.7, "s2_idle_14")

FTB_STATE("defeat", Time, 420)
FTB_ROW("defeat", 0, "bhitbridge")   // KEY
FTB_ROW("defeat", 33.033, "s2_hurt_03")
FTB_ROW("defeat", 120, "bkneel")   // KEY
FTB_ROW("defeat", 216.367, "s2_slam_05")
FTB_ROW("defeat", 283.033, "s2_slam_06")
FTB_ROW("defeat", 349.7, "s2_slam_07")
