// Exposure chart of Commodore. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// Stage 2 art: rows without KEY are new drawings (frames in art/Commodore/frames, T_commodore_<name>.png).


FTB_STATE("intro", Time, 2460)
FTB_ROW("intro", 0, "bidlein")   // KEY
FTB_ROW("intro", 160, "intro")   // KEY
FTB_ROW("intro", 400, "bentrance")   // KEY
FTB_ROW("intro", 1800, "bidleout")   // KEY
FTB_ROW("intro", 2060, "intro2")   // KEY

FTB_STATE("intro2", Time, 2820)
FTB_ROW("intro2", 2460, "bidlein")   // KEY
FTB_ROW("intro2", 2550, "s2_idle_04")
FTB_ROW("intro2", 2640, "idle")   // KEY

FTB_STATE("idle", Loop, 1600)   // one cycle
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 100, "s2_idle_01")
FTB_ROW("idle", 166.667, "s2_idle_02")
FTB_ROW("idle", 233.333, "s2_idle_03")
FTB_ROW("idle", 300, "bidleout")   // KEY
FTB_ROW("idle", 416.667, "s2_idle_03")
FTB_ROW("idle", 483.333, "s2_idle_02")
FTB_ROW("idle", 566.667, "s2_idle_01")
FTB_ROW("idle", 650, "idle")   // KEY
FTB_ROW("idle", 833.333, "s2_idle_04")
FTB_ROW("idle", 1000, "bidlein")   // KEY
FTB_ROW("idle", 1183.333, "s2_idle_04")
FTB_ROW("idle", 1350, "idle")   // KEY

FTB_STATE("walk", Walk, 909.091)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 151.515, "bwalkdown")   // KEY
FTB_ROW("walk", 303.03, "bwalkpass")   // KEY
FTB_ROW("walk", 454.545, "walk2")   // KEY
FTB_ROW("walk", 606.061, "bwalklift")   // KEY
FTB_ROW("walk", 757.576, "bwalkpush")   // KEY

FTB_STATE("aim", Time, 550)
FTB_ROW("aim", 0, "bidlein")   // KEY
FTB_ROW("aim", 33.333, "s2_wind_01")
FTB_ROW("aim", 66.667, "s2_wind_02")
FTB_ROW("aim", 100, "s2_wind_04")
FTB_ROW("aim", 133.333, "s2_wind_05")
FTB_ROW("aim", 160, "wind")   // KEY
FTB_ROW("aim", 200, "s2_wind_03")
FTB_ROW("aim", 233.333, "s2_wind_05")
FTB_ROW("aim", 266.667, "wind")
FTB_ROW("aim", 300, "s2_wind_04")
FTB_ROW("aim", 333.333, "wind")
FTB_ROW("aim", 366.667, "s2_wind_05")
FTB_ROW("aim", 400, "s2_wind_03")
FTB_ROW("aim", 433.333, "s2_wind_05")
FTB_ROW("aim", 466.667, "wind")
FTB_ROW("aim", 500, "s2_wind_04")
FTB_ROW("aim", 533.333, "wind")

FTB_STATE("charge", Loop, 333.333)   // one cycle
FTB_ROW("charge", 0, "walk1")   // KEY
FTB_ROW("charge", 55.556, "bwalkdown")   // KEY
FTB_ROW("charge", 111.111, "bwalkpass")   // KEY
FTB_ROW("charge", 166.667, "walk2")   // KEY
FTB_ROW("charge", 222.222, "bwalklift")   // KEY
FTB_ROW("charge", 277.778, "bwalkpush")   // KEY

FTB_STATE("wind", Time, 500)
FTB_ROW("wind", 0, "bwindbridge")   // KEY
FTB_ROW("wind", 33.333, "s2_wind_01")
FTB_ROW("wind", 66.667, "s2_wind_02")
FTB_ROW("wind", 100, "wind")   // KEY
FTB_ROW("wind", 133.333, "s2_wind_03")
FTB_ROW("wind", 166.667, "s2_wind_05")
FTB_ROW("wind", 200, "wind")
FTB_ROW("wind", 300, "s2_wind_05")
FTB_ROW("wind", 333.333, "s2_wind_03")
FTB_ROW("wind", 366.667, "s2_wind_05")
FTB_ROW("wind", 400, "wind")
FTB_ROW("wind", 466.667, "s2_wind_06")

FTB_STATE("swing", Time, 220)
FTB_ROW("swing", 0, "punch")   // KEY
FTB_ROW("swing", 16.667, "s2_swing_07")
FTB_ROW("swing", 33.333, "s2_swing_01")
FTB_ROW("swing", 70, "bstrikebridge")   // KEY
FTB_ROW("swing", 100, "s2_swing_02")
FTB_ROW("swing", 130, "bretract")   // KEY
FTB_ROW("swing", 166.667, "s2_swing_03")
FTB_ROW("swing", 200, "s2_swing_06")

FTB_STATE("swing:1", Time, 220)
FTB_ROW("swing:1", 0, "bash")   // KEY
FTB_ROW("swing:1", 33.333, "s2_swing_04")
FTB_ROW("swing:1", 70, "bstrikebridge")   // KEY
FTB_ROW("swing:1", 100, "s2_swing_05")
FTB_ROW("swing:1", 130, "bretract")   // KEY
FTB_ROW("swing:1", 166.667, "s2_swing_06")
FTB_ROW("swing:1", 200, "s2_sweep_08")

FTB_STATE("windup", Time, 900)
FTB_ROW("windup", 0, "bsigload")   // KEY
FTB_ROW("windup", 33.333, "s2_windup_01")
FTB_ROW("windup", 66.667, "s2_windup_02")
FTB_ROW("windup", 116.667, "s2_windup_03")
FTB_ROW("windup", 160, "bsigbridge")   // KEY
FTB_ROW("windup", 200, "s2_windup_04")
FTB_ROW("windup", 266.667, "s2_windup_07")
FTB_ROW("windup", 300, "s2_windup_13")
FTB_ROW("windup", 333.333, "s2_windup_08")
FTB_ROW("windup", 360, "windup")   // KEY
FTB_ROW("windup", 400, "s2_windup_09")
FTB_ROW("windup", 433.333, "s2_windup_10")
FTB_ROW("windup", 466.667, "s2_windup_09")
FTB_ROW("windup", 500, "s2_windup_11")
FTB_ROW("windup", 533.333, "s2_windup_08")
FTB_ROW("windup", 566.667, "s2_windup_07")
FTB_ROW("windup", 600, "s2_windup_08")
FTB_ROW("windup", 633.333, "s2_windup_11")
FTB_ROW("windup", 666.667, "s2_windup_12")
FTB_ROW("windup", 700, "s2_windup_13")
FTB_ROW("windup", 733.333, "s2_windup_16")
FTB_ROW("windup", 766.667, "s2_windup_17")
FTB_ROW("windup", 783.333, "s2_windup_20")
FTB_ROW("windup", 800, "s2_windup_18")
FTB_ROW("windup", 820, "bsigreach")   // KEY

FTB_STATE("sweep", Time, 450)
FTB_ROW("sweep", 0, "bsigreach")   // KEY
FTB_ROW("sweep", 33.333, "s2_sweep_01")
FTB_ROW("sweep", 50, "s2_sweep_09")
FTB_ROW("sweep", 70, "sweep")   // KEY
FTB_ROW("sweep", 116.667, "s2_sweep_02")
FTB_ROW("sweep", 150, "s2_sweep_03")
FTB_ROW("sweep", 166.667, "s2_sweep_04")
FTB_ROW("sweep", 190, "bsigsettle")   // KEY
FTB_ROW("sweep", 216.667, "s2_sweep_05")
FTB_ROW("sweep", 250, "s2_sweep_01")
FTB_ROW("sweep", 283.333, "s2_windup_19")
FTB_ROW("sweep", 310, "signaturefollow")   // KEY
FTB_ROW("sweep", 350, "s2_sweep_06")
FTB_ROW("sweep", 383.333, "s2_sweep_07")
FTB_ROW("sweep", 416.667, "s2_sweep_08")

FTB_STATE("hurt", Time, 260)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 70, "bhitbridge")   // KEY
FTB_ROW("hurt", 100, "s2_hurt_03")
FTB_ROW("hurt", 133.333, "s2_hurt_04")
FTB_ROW("hurt", 166.667, "s2_idle_04")
FTB_ROW("hurt", 190, "bretract")   // KEY
FTB_ROW("hurt", 233.333, "s2_swing_06")

FTB_STATE("fly", Time, 346.7)
FTB_ROW("fly", 0, "hurt")   // KEY
FTB_ROW("fly", 66.667, "s2_fly_01")

FTB_STATE("down", Time, 700)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 450)
FTB_ROW("getup", 0, "bkneel")   // KEY
FTB_ROW("getup", 50, "s2_getup_01")
FTB_ROW("getup", 83.333, "s2_getup_02")
FTB_ROW("getup", 116.667, "s2_getup_03")
FTB_ROW("getup", 150, "s2_hurt_03")
FTB_ROW("getup", 180, "bidlein")   // KEY
FTB_ROW("getup", 216.667, "s2_idle_04")
FTB_ROW("getup", 250, "s2_hurt_04")
FTB_ROW("getup", 283.333, "s2_swing_03")
FTB_ROW("getup", 316.667, "s2_sweep_08")
FTB_ROW("getup", 340, "idle")   // KEY
FTB_ROW("getup", 383.333, "s2_idle_04")
FTB_ROW("getup", 416.667, "idle")

FTB_STATE("defeat", Time, 420)
FTB_ROW("defeat", 0, "bhitbridge")   // KEY
FTB_ROW("defeat", 33.333, "s2_defeat_01")
FTB_ROW("defeat", 66.667, "s2_defeat_02")
FTB_ROW("defeat", 120, "bkneel")   // KEY
FTB_ROW("defeat", 166.667, "s2_defeat_03")
FTB_ROW("defeat", 216.667, "s2_defeat_04")
