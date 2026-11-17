// Exposure chart of Nova. Format and rules: FTBrawlCharts.inl; the art rules: S2_BRIEF.md.
// Rows marked KEY are the timing contract (TIMING_BEFORE.md): never move, rename or remove them.
// New drawings go between them. Times are ms; in a Walk or Loop state they are positions in the cycle.
// S2: rows without KEY are new drawings (T_nova_s2_*), generated from the existing frames; see manifest.json.

FTB_STATE("jump>walk", Age, 100)   // window, as jump>idle (from the code, not in the timing table)
FTB_ROW("jump>walk", 0, "nair04")   // KEY
FTB_ROW("jump>walk", 33.333, "s2_land_01")
FTB_ROW("jump>walk", 50, "nidle01")   // KEY
FTB_ROW("jump>walk", 83.333, "s2_land_02")

FTB_STATE("jumpkick>walk", Age, 100)   // window, as jump>idle (from the code, not in the timing table)
FTB_ROW("jumpkick>walk", 0, "nair04")   // KEY
FTB_ROW("jumpkick>walk", 33.333, "s2_land_01")
FTB_ROW("jumpkick>walk", 50, "nidle01")   // KEY
FTB_ROW("jumpkick>walk", 83.333, "s2_land_02")

FTB_STATE("turn>walk", Age, 65)   // window: the first 65 ms
FTB_ROW("turn>walk", 0, "turn")   // KEY
FTB_ROW("turn>walk", 33.333, "s2_walk_07")

FTB_STATE("idle>walk", Age, 60)   // window: the first 60 ms
FTB_ROW("idle>walk", 0, "nstart")   // KEY
FTB_ROW("idle>walk", 33.333, "s2_walk_01")

FTB_STATE("walk>idle", Age, 90)   // window: the first 90 ms
FTB_ROW("walk>idle", 0, "nstop")   // KEY
FTB_ROW("walk>idle", 33.333, "s2_stop_01")
FTB_ROW("walk>idle", 66.667, "s2_stop_02")

FTB_STATE("run>idle", Age, 90)   // window, as walk>idle (from the code, not in the timing table)
FTB_ROW("run>idle", 0, "nstop")   // KEY
FTB_ROW("run>idle", 33.333, "s2_stop_01")
FTB_ROW("run>idle", 66.667, "s2_stop_02")

FTB_STATE("jump>idle", Age, 100)   // window: the first 100 ms
FTB_ROW("jump>idle", 0, "nair04")   // KEY
FTB_ROW("jump>idle", 33.333, "s2_land_01")
FTB_ROW("jump>idle", 50, "nidle01")   // KEY
FTB_ROW("jump>idle", 83.333, "s2_land_02")

FTB_STATE("jumpkick>idle", Age, 100)   // window, as jump>idle (from the code, not in the timing table)
FTB_ROW("jumpkick>idle", 0, "nair04")   // KEY
FTB_ROW("jumpkick>idle", 33.333, "s2_land_01")
FTB_ROW("jumpkick>idle", 50, "nidle01")   // KEY
FTB_ROW("jumpkick>idle", 83.333, "s2_land_02")

FTB_STATE("idle", Loop, 1500)   // one cycle
FTB_ROW("idle", 0, "idle")   // KEY
FTB_ROW("idle", 187.5, "s2_idle_14")
FTB_ROW("idle", 375, "nidle01")   // KEY
FTB_ROW("idle", 500, "s2_idle_11")
FTB_ROW("idle", 625, "s2_idle_12")
FTB_ROW("idle", 750, "nidle02")   // KEY
FTB_ROW("idle", 937.5, "s2_idle_13")
FTB_ROW("idle", 1125, "nidle01")   // KEY
FTB_ROW("idle", 1312.5, "s2_idle_11")

FTB_STATE("walk", Walk, 666.667)   // one cycle
FTB_ROW("walk", 0, "walk1")   // KEY
FTB_ROW("walk", 27.778, "s2_walk_01")
FTB_ROW("walk", 55.556, "nwalk01")   // KEY
FTB_ROW("walk", 83.333, "s2_walk_02")
FTB_ROW("walk", 111.111, "nwalk02")   // KEY
FTB_ROW("walk", 138.889, "s2_walk_03")
FTB_ROW("walk", 166.667, "walk2")   // KEY
FTB_ROW("walk", 194.445, "s2_walk_04")
FTB_ROW("walk", 222.222, "nwalk03")   // KEY
FTB_ROW("walk", 250, "s2_walk_05")
FTB_ROW("walk", 277.778, "nwalk04")   // KEY
FTB_ROW("walk", 305.556, "s2_walk_06")
FTB_ROW("walk", 333.333, "walk3")   // KEY
FTB_ROW("walk", 361.111, "s2_walk_01")
FTB_ROW("walk", 388.889, "nwalk05")   // KEY
FTB_ROW("walk", 416.667, "s2_walk_02")
FTB_ROW("walk", 444.444, "nwalk06")   // KEY
FTB_ROW("walk", 472.222, "s2_walk_03")
FTB_ROW("walk", 500, "walk2")   // KEY
FTB_ROW("walk", 527.778, "s2_walk_04")
FTB_ROW("walk", 555.556, "nwalk07")   // KEY
FTB_ROW("walk", 583.334, "s2_walk_05")
FTB_ROW("walk", 611.111, "nwalk08")   // KEY
FTB_ROW("walk", 638.889, "s2_walk_06")

FTB_STATE("run", Loop, 300)   // one cycle
FTB_ROW("run", 0, "nrun01")   // KEY
FTB_ROW("run", 37.5, "nrun02")   // KEY
FTB_ROW("run", 75, "nrun03")   // KEY
FTB_ROW("run", 112.5, "nrun04")   // KEY
FTB_ROW("run", 150, "nrun05")   // KEY
FTB_ROW("run", 187.5, "nrun06")   // KEY
FTB_ROW("run", 226, "nrun07")   // KEY
FTB_ROW("run", 262.5, "nrun08")   // KEY

FTB_STATE("punch:1", Time, 200)
FTB_ROW("punch:1", 0, "njab01")   // KEY
FTB_ROW("punch:1", 30, "njab02")   // KEY
FTB_ROW("punch:1", 60, "jab")   // KEY
FTB_ROW("punch:1", 100, "njab03")   // KEY
FTB_ROW("punch:1", 140, "njab04")   // KEY
FTB_ROW("punch:1", 166.667, "s2_punch_01")

FTB_STATE("punch:2", Time, 200)
FTB_ROW("punch:2", 0, "ncross01")   // KEY
FTB_ROW("punch:2", 30, "ncross02")   // KEY
FTB_ROW("punch:2", 60, "cross")   // KEY
FTB_ROW("punch:2", 100, "ncross03")   // KEY
FTB_ROW("punch:2", 140, "ncross04")   // KEY
FTB_ROW("punch:2", 166.667, "s2_punch_02")

FTB_STATE("punch:3", Time, 300)
FTB_ROW("punch:3", 0, "nupper01")   // KEY
FTB_ROW("punch:3", 30, "nupper02")   // KEY
FTB_ROW("punch:3", 60, "upper")   // KEY
FTB_ROW("punch:3", 83.333, "s2_punch_03")
FTB_ROW("punch:3", 116.667, "s2_punch_04")
FTB_ROW("punch:3", 140, "nupper03")   // KEY
FTB_ROW("punch:3", 210, "nupper04")   // KEY
FTB_ROW("punch:3", 283.333, "s2_punch_08")

FTB_STATE("kick", Time, 400)
FTB_ROW("kick", 0, "nkick01")   // KEY
FTB_ROW("kick", 60, "nkick02")   // KEY
FTB_ROW("kick", 83.333, "s2_kick_01")
FTB_ROW("kick", 120, "kick")   // KEY
FTB_ROW("kick", 150, "s2_kick_03")
FTB_ROW("kick", 183.333, "s2_kick_04")
FTB_ROW("kick", 216.667, "s2_kick_05")
FTB_ROW("kick", 240, "nkick03")   // KEY
FTB_ROW("kick", 266.667, "s2_kick_06")
FTB_ROW("kick", 300, "s2_kick_07")
FTB_ROW("kick", 320, "nkick04")   // KEY
FTB_ROW("kick", 350, "s2_kick_08")

FTB_STATE("jump", Time, 705.9)
FTB_ROW("jump", 0, "njump01")   // KEY
FTB_ROW("jump", 34, "njump02")   // KEY
FTB_ROW("jump", 66.667, "s2_jump_01")
FTB_ROW("jump", 83.333, "s2_jump_02")
FTB_ROW("jump", 110, "jump")   // KEY
FTB_ROW("jump", 150, "s2_jump_04")
FTB_ROW("jump", 183.333, "s2_jump_05")
FTB_ROW("jump", 216.667, "s2_jump_06")
FTB_ROW("jump", 240, "njump03")   // KEY
FTB_ROW("jump", 266.667, "s2_jump_07")
FTB_ROW("jump", 300, "s2_jump_08")
FTB_ROW("jump", 430, "njump04")   // KEY
FTB_ROW("jump", 466.667, "s2_jump_12")
FTB_ROW("jump", 500, "s2_jump_13")
FTB_ROW("jump", 566.667, "s2_jump_15")
FTB_ROW("jump", 600, "jumpfall")   // KEY
FTB_ROW("jump", 633.333, "s2_jump_16")
FTB_ROW("jump", 650, "s2_jump_17")
FTB_ROW("jump", 666.667, "s2_jump_18")
FTB_ROW("jump", 683.333, "s2_jump_19")

FTB_STATE("jumpkick", Time, 555.9)
FTB_ROW("jumpkick", 0, "jumpkick")   // KEY
FTB_ROW("jumpkick", 33.333, "s2_jumpkick_01")
FTB_ROW("jumpkick", 60, "nair02")   // KEY
FTB_ROW("jumpkick", 100, "s2_jumpkick_02")
FTB_ROW("jumpkick", 140, "nair01")   // KEY
FTB_ROW("jumpkick", 175, "s2_jumpkick_03")
FTB_ROW("jumpkick", 210, "nair03")   // KEY
FTB_ROW("jumpkick", 250, "s2_jumpkick_04")
FTB_ROW("jumpkick", 283.333, "s2_jumpkick_05")
FTB_ROW("jumpkick", 316.667, "s2_jumpkick_06")
FTB_ROW("jumpkick", 350, "s2_jumpkick_07")
FTB_ROW("jumpkick", 383.333, "s2_jumpkick_08")
FTB_ROW("jumpkick", 416.667, "s2_jumpkick_09")
FTB_ROW("jumpkick", 450, "s2_jumpkick_10")
FTB_ROW("jumpkick", 483.333, "s2_jumpkick_11")
FTB_ROW("jumpkick", 516.667, "s2_jumpkick_12")

FTB_STATE("pickup", Time, 220)
FTB_ROW("pickup", 0, "nreach01")   // KEY
FTB_ROW("pickup", 33.333, "s2_pickup_01")
FTB_ROW("pickup", 55, "nreach02")   // KEY
FTB_ROW("pickup", 83.333, "s2_pickup_02")
FTB_ROW("pickup", 120, "nreach03")   // KEY
FTB_ROW("pickup", 150, "s2_pickup_03")
FTB_ROW("pickup", 180, "nweapon04")   // KEY

FTB_STATE("lift", Time, 220)
FTB_ROW("lift", 0, "nreach01")   // KEY
FTB_ROW("lift", 33.333, "s2_pickup_01")
FTB_ROW("lift", 50, "nreach02")   // KEY
FTB_ROW("lift", 83.333, "s2_pickup_02")
FTB_ROW("lift", 100, "nreach03")   // KEY
FTB_ROW("lift", 116.667, "s2_pickup_03")
FTB_ROW("lift", 133.333, "s2_pickup_04")
FTB_ROW("lift", 160, "nreach04")   // KEY

FTB_STATE("carry", Walk, 833.333)   // one cycle
FTB_ROW("carry", 0, "ncarry01")   // KEY
FTB_ROW("carry", 41.667, "s2_carry_01")
FTB_ROW("carry", 83.333, "s2_carry_02")
FTB_ROW("carry", 125, "s2_carry_03")
FTB_ROW("carry", 208.333, "ncarry02")   // KEY
FTB_ROW("carry", 250, "s2_carry_05")
FTB_ROW("carry", 333.333, "s2_carry_07")
FTB_ROW("carry", 375, "s2_carry_08")
FTB_ROW("carry", 416.667, "ncarry03")   // KEY
FTB_ROW("carry", 458.333, "s2_carry_01")
FTB_ROW("carry", 500, "s2_carry_02")
FTB_ROW("carry", 541.666, "s2_carry_03")
FTB_ROW("carry", 625, "ncarry04")   // KEY
FTB_ROW("carry", 666.666, "s2_carry_05")
FTB_ROW("carry", 750, "s2_carry_07")
FTB_ROW("carry", 791.666, "s2_carry_08")

FTB_STATE("hurl", Time, 340)
FTB_ROW("hurl", 0, "nweapon01")   // KEY
FTB_ROW("hurl", 33.333, "s2_wpn_01")
FTB_ROW("hurl", 50, "nweapon02")   // KEY
FTB_ROW("hurl", 83.333, "s2_wpn_02")
FTB_ROW("hurl", 100, "bash")   // KEY
FTB_ROW("hurl", 133.333, "s2_wpn_03")
FTB_ROW("hurl", 150, "s2_wpn_04")
FTB_ROW("hurl", 180, "nweapon03")   // KEY
FTB_ROW("hurl", 216.667, "s2_wpn_05")
FTB_ROW("hurl", 250, "s2_wpn_06")
FTB_ROW("hurl", 275, "nweapon04")   // KEY
FTB_ROW("hurl", 300, "s2_wpn_07")
FTB_ROW("hurl", 316.667, "s2_wpn_08")

FTB_STATE("wpn", Time, 340)
FTB_ROW("wpn", 0, "nweapon01")   // KEY
FTB_ROW("wpn", 33.333, "s2_wpn_01")
FTB_ROW("wpn", 65, "nweapon02")   // KEY
FTB_ROW("wpn", 100, "s2_wpn_02")
FTB_ROW("wpn", 130, "bash")   // KEY
FTB_ROW("wpn", 166.667, "s2_wpn_03")
FTB_ROW("wpn", 183.333, "s2_wpn_04")
FTB_ROW("wpn", 210, "nweapon03")   // KEY
FTB_ROW("wpn", 233.333, "s2_wpn_05")
FTB_ROW("wpn", 250, "s2_wpn_06")
FTB_ROW("wpn", 275, "nweapon04")   // KEY
FTB_ROW("wpn", 300, "s2_wpn_07")

FTB_STATE("toss", Time, 320)
FTB_ROW("toss", 0, "nweapon01")   // KEY
FTB_ROW("toss", 33.333, "s2_wpn_01")
FTB_ROW("toss", 70, "nweapon02")   // KEY
FTB_ROW("toss", 100, "s2_wpn_02")
FTB_ROW("toss", 140, "bash")   // KEY
FTB_ROW("toss", 166.667, "s2_wpn_03")
FTB_ROW("toss", 200, "s2_wpn_04")
FTB_ROW("toss", 220, "nweapon03")   // KEY
FTB_ROW("toss", 250, "s2_wpn_05")
FTB_ROW("toss", 275, "nweapon04")   // KEY
FTB_ROW("toss", 300, "s2_wpn_07")

FTB_STATE("drink", Time, 950)
FTB_ROW("drink", 0, "ndrink01")   // KEY
FTB_ROW("drink", 33.333, "s2_drink_01")
FTB_ROW("drink", 66.667, "s2_drink_02")
FTB_ROW("drink", 100, "s2_drink_03")
FTB_ROW("drink", 120, "ndrink02")   // KEY
FTB_ROW("drink", 150, "s2_drink_04")
FTB_ROW("drink", 183.333, "s2_drink_05")
FTB_ROW("drink", 216.667, "s2_drink_06")
FTB_ROW("drink", 250, "s2_drink_07")
FTB_ROW("drink", 280, "ndrink03")   // KEY
FTB_ROW("drink", 316.667, "s2_drink_08")
FTB_ROW("drink", 383.333, "s2_drink_09")
FTB_ROW("drink", 450, "s2_drink_08")
FTB_ROW("drink", 516.667, "s2_drink_09")
FTB_ROW("drink", 583.333, "s2_drink_08")
FTB_ROW("drink", 650, "s2_drink_09")
FTB_ROW("drink", 750, "s2_drink_11")
FTB_ROW("drink", 780, "ndrink04")   // KEY
FTB_ROW("drink", 816.667, "s2_drink_12")
FTB_ROW("drink", 850, "s2_drink_13")
FTB_ROW("drink", 883.333, "s2_drink_14")
FTB_ROW("drink", 916.667, "s2_drink_15")

FTB_STATE("hurt", Time, 300)
FTB_ROW("hurt", 0, "hurt")   // KEY
FTB_ROW("hurt", 33.333, "s2_hurt_01")
FTB_ROW("hurt", 60, "nreact01")   // KEY
FTB_ROW("hurt", 83.333, "s2_hurt_02")
FTB_ROW("hurt", 100, "s2_hurt_03")
FTB_ROW("hurt", 120, "nreact02")   // KEY
FTB_ROW("hurt", 150, "s2_hurt_04")
FTB_ROW("hurt", 183.333, "s2_hurt_05")
FTB_ROW("hurt", 200, "s2_hurt_06")
FTB_ROW("hurt", 230, "nidle01")   // KEY
FTB_ROW("hurt", 266.667, "s2_hurt_07")

FTB_STATE("held", Time, 500)
FTB_ROW("held", 0, "hurt")   // KEY

FTB_STATE("down", Time, 900)
FTB_ROW("down", 0, "down")   // KEY

FTB_STATE("getup", Time, 350)
FTB_ROW("getup", 0, "nreact03")   // KEY
FTB_ROW("getup", 33.333, "s2_getup_01")
FTB_ROW("getup", 66.667, "s2_getup_02")
FTB_ROW("getup", 100, "getup")   // KEY
FTB_ROW("getup", 150, "s2_getup_04")
FTB_ROW("getup", 166.667, "s2_getup_05")
FTB_ROW("getup", 190, "nreact04")   // KEY
FTB_ROW("getup", 216.667, "s2_hurt_06")
FTB_ROW("getup", 250, "s2_getup_06")
FTB_ROW("getup", 280, "nidle01")   // KEY
FTB_ROW("getup", 316.667, "s2_land_02")

FTB_STATE("cheer", Time, 500)
FTB_ROW("cheer", 0, "win")   // KEY
FTB_ROW("cheer", 33.333, "s2_cheer_01")
FTB_ROW("cheer", 66.667, "s2_cheer_02")
FTB_ROW("cheer", 100, "s2_cheer_03")
FTB_ROW("cheer", 133.333, "s2_cheer_04")
FTB_ROW("cheer", 166.667, "s2_cheer_05")
FTB_ROW("cheer", 200, "s2_cheer_04")
FTB_ROW("cheer", 233.333, "s2_cheer_06")
FTB_ROW("cheer", 266.667, "s2_cheer_07")
FTB_ROW("cheer", 300, "s2_cheer_08")
