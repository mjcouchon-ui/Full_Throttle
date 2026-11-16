"""Chemical Works, Stage 1, builder s1d8: district 8 "Reactor Two", the copper quarter (21.40 to 25.76 km).
/Game/World/ChemicalWorks/SM_S1D8_*. Run: import ft_world_chem_s1d8 as K; K.run()

The new reactor's foundry-and-power quarter: heavy, angular, buttressed. Battered walls (the long blocks lean in at
their ends, the towers on all four sides), stepped buttress fins with a lit (deep orange) and a shade (rust) half,
stepped parapets like a ziggurat's edge, slit windows in threes, short square roof vents, rivet bands, one violet seam
a building. 7 buildings, 9 props, 1 hero (THE HOURGLASS, 70 m).

Colour rules kept here: copper orange and deep orange only on pieces 3 m and more in every direction (orange is the
missile pickup's colour), never on a part that moves, never glowing. What glows: lit window cream and the violet seam,
steady, and every piece that has any stands 41 m or more out (the table block). The low props (ladle car, anvil rack,
bollards) carry no orange and no glow. Nothing here is a coil, a ribbed column, a ring or ball on a top, or a vehicle.
"""
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/ChemicalWorks'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))   # a piece with no moving part: true bounds

CU, DO, RU, NV, LIT, VI = '#ff9e3d', '#ff8a2a', '#a8552f', '#1c1f4a', '#ffe9a8', '#b44dff'   # the district's six
ST, NC, NC2, PAVE, STEAM, TEAL, CREAM = '#8f9bb3', '#3c4a78', '#2a3560', '#7f8fc6', '#d8d0f0', '#12849a', '#fff1c0'
Q = 1.41421


# ------------------------------------------------------------------------------------------ the quarter's vocabulary
def trap(g, col, x, y0, y1, z0, h, w, tw):
    """A long battered block: w wide at its foot, tw at its top (its two ends lean in), from y0 to y1."""
    g.side(col, [(x - w / 2.0, z0), (x + w / 2.0, z0), (x + tw / 2.0, z0 + h), (x - tw / 2.0, z0 + h)], y0, y1)


def pyr(g, col, x, y, z0, h, w, tw):
    """A square battered block (all four walls lean): w wide at its foot, tw at its top."""
    g.cyl(col, (x, y, z0), w / 2.0 * Q, h, rot=(0, 45, 0), steps=4, top_r=tw / 2.0 * Q)


def slits(g, x, y, z, h=240.0, w=36.0, pitch=70.0, glow=0.5):
    """Three tall slit windows, lit and steady, in a navy recess, on a wall that looks at -Y at the plane y."""
    g.box(NV, (x, y - 5, z), (2 * pitch + w + 36, 10, h + 40))
    for k in (-1, 0, 1):
        g.box(LIT, (x + k * pitch, y - 12, z), (w, 6, h), glow=glow)


def fin(g, x, yf, z0, h, out, th=70.0):
    """A stepped buttress fin on a wall that looks at -Y: its lit half deep orange, its shade half rust."""
    for sx, col in ((-1, DO), (1, RU)):
        cx = x + sx * th / 4.0
        g.box(col, (cx, yf - out / 2.0, z0 + h * 0.26), (th / 2.0, out, h * 0.52))
        g.box(col, (cx, yf - out * 0.3, z0 + h * 0.68), (th / 2.0, out * 0.6, h * 0.32))
    tilt_box(g, RU, (x, yf - out * 0.62, z0 + h * 0.84), (x, yf + 4, z0 + h), th + 12, 26)


def zig(g, col, x, y, z, w, d, step=110.0, tiers=3, shrink=0.3):
    """A stepped parapet: tiers one on another, each narrower."""
    for t in range(tiers):
        g.box(col, (x, y, z + step * (t + 0.5)), (w * (1 - shrink * t), d, step))


def vent(g, x, y, z, s=110.0, hot=0.0):
    """A short square roof vent with a rust cap. hot: a steady warm glow in its mouth (no flame)."""
    g.box(NV, (x, y, z + s * 0.55), (s, s, s * 1.1))
    g.box(RU, (x, y, z + s * 1.1 + 9), (s + 26, s + 26, 18))
    if hot:
        g.box(LIT, (x, y - s / 2.0 - 2, z + s * 0.6), (s * 0.7, 6, s * 0.55), glow=hot)


def door(g, x, y, z, w=200.0, h=400.0, glow=0.5):
    """A navy steel door with a lit transom, on a wall that looks at -Y at the plane y."""
    g.box(NV, (x, y - 6, z + h / 2.0), (w, 12, h))
    g.box(ST, (x, y - 13, z + h / 2.0), (8, 6, h - 30))
    g.box(LIT, (x, y - 8, z + h + 28), (w, 10, 44), glow=glow)


def lantern(g, x, y, z, glow=0.7):
    """A hanging lantern: navy caps, cream glass, steady."""
    g.box(NV, (x, y, z + 30), (44, 44, 10))
    g.box(LIT, (x, y, z), (32, 32, 50), glow=glow)
    g.box(NV, (x, y, z - 29), (38, 38, 8))


# ------------------------------------------------------------------------------------------ buildings
def power_hall(sc):
    """The power hall: 26 x 13 m, 24.8 m. A long battered block, eight buttress fins, slit windows in threes in two rows."""
    g = sc.still()
    g.box(NV, (0, 0, -50), (2760, 1460, 500))                     # plinth, 3 m of footing
    trap(g, CU, 0, -650, 650, 200, 1800, 2600, 2300)              # the hall
    trap(g, DO, 0, -470, 480, 2000, 380, 1960, 1800)              # the attic storey, set back
    for z in (820, 1420):
        g.box(RU, (0, 0, z), (2600 - 300 * (z - 200) / 1800.0 + 14, 1314, 30))     # rivet bands
    for k in range(8):
        fin(g, -1050 + k * 300, -650, 200, 1500, 260)
    for k in range(7):
        x = -900 + k * 300
        if k != 3:
            slits(g, x, -650, 760, h=300, w=32, pitch=58)
        slits(g, x, -650, 1340, h=260, w=32, pitch=58)
    door(g, 0, -650, 200, w=210, h=430)
    g.box(NV, (0, -735, 740), (330, 170, 16))                     # canopy and its cream valance
    g.box(LIT, (0, -822, 728), (330, 6, 22))
    for sx in (-1, 1):
        g.box(NV, (sx * 170, -700, 700), (8, 100, 8))
        lantern(g, sx * 170, -745, 655)
        zig(g, DO, sx * 980, -560, 2000, 300, 160, step=90)       # stepped corner blocks on the main roof
        slits(g, sx * 560, -470, 2190, h=200, w=30, pitch=56)     # the attic's own
    g.box(PAVE, (0, -815, 50), (440, 170, 100))                   # two steps up to the door
    g.box(PAVE, (0, -775, 150), (360, 90, 100))
    zig(g, CU, 0, -430, 2380, 900, 120, step=100)                 # the stepped parapet over the middle
    for k in range(6):
        vent(g, -750 + k * 300, 180, 2380)
    g.box(VI, (0, -655, 1960), (2240, 10, 24), glow=0.5)          # the violet seam under the attic


def foundry(sc):
    """The foundry: a stepped block of three, 22 x 14 m, vents with a steady warm glow on each step, a square stack to 32 m."""
    g = sc.still()
    g.box(NV, (0, 0, -50), (2360, 1540, 500))
    trap(g, CU, 0, -700, 700, 200, 900, 2200, 2040)               # 2 to 11 m
    trap(g, DO, 0, -450, 600, 1100, 700, 1600, 1480)              # 11 to 18 m
    trap(g, CU, 0, -200, 520, 1800, 550, 1000, 920)               # 18 to 23.5 m
    g.box(RU, (0, 0, 700), (2125, 1414, 30))
    for x in (-980, -340, 340, 980):
        fin(g, x, -700, 200, 800, 230)
    g.box(NV, (0, -706, 460), (440, 12, 520))                     # the charging door: a stepped head
    g.box(NV, (0, -706, 760), (300, 12, 80))
    g.box(NV, (0, -706, 830), (160, 12, 60))
    g.box(LIT, (0, -714, 290), (360, 6, 90), glow=0.45)           # the hearth's glow, low inside it
    for sx in (-1, 1):
        slits(g, sx * 660, -700, 680, h=320)
        slits(g, sx * 420, -450, 1470, h=300)
        g.box(RU, (sx * 950, -640, 1150), (110, 110, 100))        # corner blocks
        g.box(RU, (sx * 690, -400, 1845), (90, 90, 90))
    slits(g, 0, -200, 2090, h=260)
    for k in range(5):
        vent(g, -800 + k * 400, -575, 1100, s=120, hot=0.6)
    for x in (-520, 0, 520):
        vent(g, x, -325, 1800, s=110, hot=0.6)
    zig(g, DO, 0, -160, 2350, 700, 90, step=90)
    g.box(VI, (0, -455, 1135), (1560, 10, 24), glow=0.5)
    pyr(g, RU, 840, 330, 1100, 2100, 280, 200)                    # the stack, square and battered, no flame
    g.box(NV, (840, 330, 1700), (273, 273, 40))
    g.box(NV, (840, 330, 2500), (243, 243, 40))
    g.box(LIT, (840, 330, 3100), (216, 216, 36))
    g.box(NV, (840, 330, 3185), (236, 236, 50))
    g.box(NV, (-420, 380, 2440), (220, 180, 150))                 # a water tank on four legs on the top step
    g.box(NC, (-420, 380, 2358), (180, 140, 16))
    g.box(NV, (-300, -430, 2250), (40, 500, 40))                  # the hoist beam and its hook block, barely swinging
    h = sc.part(P.motion(swing=3, pivot=(-300, -640, 2250), axis=90.0, rate=0.22, phase=0.3))
    h.box(ST, (-300, -640, 1985), (7, 7, 530))
    h.box(NV, (-300, -640, 1690), (56, 56, 70))
    h.box(ST, (-300, -640, 1640), (12, 30, 36))


def switch_house(sc):
    """The switch house: a plain windowless copper cube, 12 m, on a navy plinth, one violet seam. No coil, no ball."""
    g = sc.still()
    g.box(NV, (0, 0, -50), (1400, 1400, 500))
    g.box(CU, (0, 0, 800), (1200, 1200, 1200))
    for sx in (-1, 1):
        for sy in (-1, 1):
            pyr(g, DO, sx * 590, sy * 590, 200, 1230, 190, 110)   # battered corner piers
    for z in (520, 840, 1160):
        g.box(RU, (0, 0, z), (1214, 1214, 26))
    for x in (-200, 200):
        g.box(RU, (x, -604, 800), (22, 8, 1190))
    g.box(VI, (-330, -606, 800), (24, 12, 1170), glow=0.5)        # the seam
    g.box(RU, (0, 0, 1415), (1280, 1280, 30))                     # stepped cap
    g.box(CU, (0, 0, 1480), (1060, 1060, 100))
    g.box(DO, (0, 0, 1575), (820, 820, 90))
    for x in (-240, 0, 240):
        vent(g, x, 60, 1620, s=100)
    door(g, 330, -600, 200, w=170, h=380)
    g.box(NV, (330, -690, 652), (240, 160, 14))
    lantern(g, 330, -720, 608)
    g.box(PAVE, (330, -765, 100), (260, 130, 200))
    for k in range(3):
        g.box(PAVE, (-470 + k * 170, -800, 6), (150, 150, 12))    # trench covers
    for y in (178, 222):
        g.box(ST, (606, y, 800), (8, 8, 1250))                    # a ladder to the roof
    for k in range(9):
        g.box(ST, (606, 200, 300 + k * 130), (8, 50, 8))


def _parasol(g, x, y, z):
    g.cyl(NV, (x, y, z), 5, 210, steps=4)
    g.cyl(CREAM, (x, y, z + 200), 120, 46, steps=6, top_r=8)
    g.cyl(ST, (x, y, z + 70), 44, 8, steps=6)


def club(sc):
    """The engineers' club: a ziggurat of three steps, 22 x 16 m, 18 m, a terrace on each, a stair up the front."""
    g = sc.still()
    g.box(NV, (0, 0, -50), (2340, 1740, 500))
    trap(g, CU, 0, -800, 800, 200, 600, 2200, 2080)               # 2 to 8 m
    trap(g, DO, 0, -480, 640, 800, 550, 1560, 1460)               # 8 to 13.5 m
    trap(g, CU, 0, -200, 500, 1350, 480, 960, 880)                # 13.5 to 18.3 m
    g.box(RU, (0, 0, 520), (2150, 1614, 26))
    for k in range(8):                                            # the stair: eight steps to the first terrace
        g.box(PAVE, (0, -800 - (8 - k) * 32.5, 100 * k + 50), (420, (8 - k) * 65, 100))
    for sx in (-1, 1):
        g.box(NV, (sx * 235, -1190, 170), (50, 260, 340))         # the stair's stepped cheeks, a lantern on each
        g.box(NV, (sx * 235, -935, 400), (50, 250, 800))
        lantern(g, sx * 235, -1190, 378)
        g.box(ST, (sx * 620, -780, 900), (780, 8, 8))             # first terrace: rail, parasols, a planter
        for x in (480, 840):
            _parasol(g, sx * x, -640, 800)
        g.box(NV, (sx * 960, -560, 835), (90, 90, 70))
        g.cyl(TEAL, (sx * 960, -560, 870), 34, 150, steps=5, top_r=4)
        slits(g, sx * 650, -800, 540, h=260)
        tilt_box(g, NV, (sx * 650, -800, 720), (sx * 650, -930, 650), 260, 10)      # awnings, a cream valance
        g.box(LIT, (sx * 650, -932, 640), (260, 6, 26))
        _parasol(g, sx * 500, -330, 1350)                         # second terrace
        g.box(NV, (sx * 660, -300, 1385), (80, 80, 70))
        g.cyl(TEAL, (sx * 660, -300, 1420), 30, 130, steps=5, top_r=4)
        slits(g, sx * 420, -480, 1100, h=260)
        for sy in (-150, 450):
            g.box(RU, (sx * 390, sy, 1885), (90, 90, 110))        # corner blocks on the top step
    for x in range(-1000, 1001, 250):
        if abs(x) > 230:
            g.box(NV, (x, -780, 850), (10, 10, 100))
    for x in (-700, -350, 0, 350, 700):
        g.box(NV, (x, -460, 1400), (10, 10, 100))
    g.box(ST, (0, -460, 1450), (1420, 8, 8))
    for x in (-400, 0, 400):
        g.box(NV, (x, -180, 1880), (10, 10, 100))
    g.box(ST, (0, -180, 1930), (820, 8, 8))
    door(g, 0, -480, 800, w=200, h=380)
    slits(g, 0, -200, 1590, h=240)
    g.box(VI, (0, -205, 1800), (860, 10, 22), glow=0.5)
    g.cyl(ST, (0, 150, 1830), 7, 520, steps=5)                    # a mast and a violet pennant turning on it
    f = sc.part(P.motion(swing=16, pivot=(0, 150, 2290), axis=P.UP, rate=0.3, phase=0.2))
    f.box(VI, (70, 150, 2290), (130, 6, 64))


def smiths_row(sc):
    """The copper smiths' row: four tall narrow workshops with stepped gables, 22.4 x 10 m, 16 to 21 m."""
    g = sc.still()
    g.box(NV, (0, 0, -50), (2380, 1100, 500))
    heights, cols = (1500, 1800, 1400, 1700), (CU, DO, RU, CU)
    for k in range(4):
        x, h, c = -840 + k * 560, heights[k], cols[k]
        g.box(c, (x, 0, 200 + h / 2.0), (548, 1000, h))
        zig(g, c, x, -440, 200 + h, 548, 120, step=110, tiers=3, shrink=0.32)
        g.box(DO if c == RU else RU, (x, -440, 200 + h + 337), (230, 134, 14))
        door(g, x - 90, -500, 200, w=170, h=360)
        tilt_box(g, NV, (x - 90, -500, 700), (x - 90, -650, 630), 280, 10)          # awning, a cream valance
        g.box(LIT, (x - 90, -652, 620), (280, 6, 24))
        g.box(LIT, (x + 130, -504, 420), (170, 8, 200), glow=0.35)                  # the shop window
        g.box(NV, (x + 130, -510, 420), (8, 6, 200))
        slits(g, x, -500, 200 + h * 0.66, h=240, w=34, pitch=66)
        g.box(LIT, (x, -506, 200 + h + 55), (56, 8, 56), glow=0.5)                  # a window in the gable
        g.box(NV, (x + 200, -570, 860), (8, 140, 8))                                # the trade sign: a blank shape
        sx, sy, sz = x + 200, -610, 790
        if k == 0:
            g.box(LIT, (sx, sy, sz), (70, 8, 70))
        elif k == 1:
            g.box(LIT, (sx, sy, sz), (62, 8, 62), rot=(45, 0, 0))
        elif k == 2:
            g.cyl(LIT, (sx, sy + 4, sz), 38, 8, rot=(0, 0, 90), steps=8)
        else:
            g.box(LIT, (sx, sy, sz + 18), (80, 8, 22))
            g.box(LIT, (sx, sy, sz - 18), (50, 8, 22))
        g.box(RU, (x + 170, 330, 200 + h + 140), (90, 90, 280))                     # chimney
        g.box(NV, (x + 170, 330, 200 + h + 290), (110, 110, 20))
    for k in range(3):
        g.box(ST, (-560 + k * 560, -506, 900), (14, 12, 1400))                      # downpipes between the shops
    g.box(VI, (-280, -506, 1960), (548, 10, 22), glow=0.5)
    g.box(NC2, (700, -650, 40), (170, 60, 80))                                      # a bench and an anvil at the last door
    g.box(NV, (700, -650, 94), (50, 34, 28))
    g.box(ST, (700, -650, 118), (110, 44, 20))


def gate_bastion(sc):
    """The gate bastion: two battered towers, 24 m, a bridge between them, a half-raised portcullis under it."""
    g = sc.still()

    def hw(z):
        return 400 - 80 * (z - 200) / 2100.0

    for sx in (-1, 1):
        x = sx * 800
        g.box(NV, (x, 0, -50), (940, 940, 500))
        pyr(g, CU, x, 0, 200, 2100, 800, 640)
        for z in (760, 1380, 1960):
            g.box(RU, (x, 0, z), (2 * hw(z) + 14, 2 * hw(z) + 14, 28))
        slits(g, x, -hw(760) + 2, 900, h=280)
        slits(g, x, -hw(1560) + 2, 1700, h=280)
        fin(g, x + sx * 250, -360, 200, 1000, 280, th=80)
        g.box(RU, (x, 0, 2315), (700, 700, 30))                   # the stepped crown
        for a in (-1, 1):
            for b in (-1, 1):
                g.box(CU, (x + a * 265, b * 265, 2400), (150, 150, 140))
        g.box(DO, (x, 0, 2395), (360, 360, 130))
        vent(g, x, 0, 2460, s=90)
        g.box(RU, (sx * 370, 0, 1200), (180, 520, 100))           # corbels under the bridge
        g.box(RU, (sx * 420, 0, 1110), (90, 520, 80))
        slits(g, sx * 230, -260, 1540, h=240, w=34, pitch=62)
        g.box(NV, (sx * 405, -290, 700), (70, 8, 8))              # a lantern each side of the gate
        lantern(g, sx * 380, -290, 655)
    g.box(DO, (0, 0, 1500), (920, 520, 500))                      # the bridge
    zig(g, DO, 0, -215, 1750, 920, 90, step=80, tiers=3, shrink=0.3)
    g.box(VI, (0, -265, 1290), (900, 10, 22), glow=0.5)
    for k in range(5):
        g.box(ST, (-300 + k * 150, 0, 960), (12, 12, 580))        # the portcullis
    g.box(ST, (0, 0, 676), (700, 14, 14))
    g.box(ST, (0, 0, 960), (700, 14, 14))
    g.box(PAVE, (0, 0, 100), (700, 700, 200))
    g.cyl(ST, (800, 0, 2570), 6, 330, steps=5)                    # a weather vane on the right tower
    v = sc.part(P.motion(swing=28, pivot=(800, 0, 2850), axis=P.UP, rate=0.09, phase=0.4))
    v.box(LIT, (800, 0, 2850), (220, 6, 14))
    v.box(LIT, (700, 0, 2850), (50, 6, 70))
    v.box(NV, (905, 0, 2850), (30, 8, 40))


def quench_tower(sc):
    """The quench tower: a battered shaft of five storeys, 30.8 m, a stepped crown, three slow puffs of steam over it."""
    g = sc.still()

    def hw(z):
        return 450 - 140 * (z - 200) / 2400.0

    g.box(NV, (0, 0, -50), (1100, 1100, 500))
    pyr(g, CU, 0, 0, 200, 2400, 900, 620)
    for sx in (-1, 1):
        for sy in (-1, 1):
            pyr(g, DO, sx * 400, sy * 400, 200, 1300, 240, 120)   # corner buttresses
            g.box(CU, (sx * 330, sy * 330, 2830), (120, 120, 120))
    for z in (820, 1420, 2020):
        g.box(RU, (0, 0, z), (2 * hw(z) + 14, 2 * hw(z) + 14, 28))
    for z, h in ((1120, 300), (1720, 280), (2280, 240)):
        slits(g, 0, -hw(z - h / 2.0) + 2, z, h=h, w=30, pitch=56)
    door(g, 0, -450, 200, w=170, h=360)
    yb = -hw(1990)
    g.box(NV, (0, yb - 70, 1985), (420, 150, 14))                 # a balcony
    g.box(ST, (0, yb - 140, 2085), (420, 8, 8))
    for x in (-200, 0, 200):
        g.box(NV, (x, yb - 140, 2040), (8, 8, 90))
    g.box(VI, (0, -hw(2560) - 3, 2560), (2 * hw(2560) - 20, 10, 22), glow=0.5)
    g.box(RU, (0, 0, 2615), (700, 700, 30))                       # the crown
    g.box(DO, (0, 0, 2700), (780, 780, 140))
    g.box(CU, (0, 0, 2820), (520, 520, 100))
    g.box(NV, (0, 0, 2960), (340, 340, 180))
    g.box(LIT, (0, 0, 3066), (360, 360, 32))
    for k, (x, y, z, r) in enumerate(((0, 0, 3200, 150), (70, 20, 3420, 190), (-50, -30, 3680, 230))):
        s = sc.part(P.motion(bob=-30.0 - 10 * k, rate=0.11, phase=0.3 * k))
        s.sphere(STEAM, (x, y, z), r, steps=(4, 6))


# ------------------------------------------------------------------------------------------ props
def ingot_stack(sc):
    """Copper ingots on a navy sledge: 5.4 x 3.1 m, 3.7 m high. Static, unlit. TALL."""
    g = sc.still()
    for sy in (-1, 1):
        g.box(NV, (0, sy * 120, 16), (600, 36, 32))
        tilt_box(g, NV, (300, sy * 120, 16), (350, sy * 120, 60), 36, 32)
    for x in (-200, 0, 200):
        g.box(ST, (x, 0, 36), (40, 330, 10))
    for t, n in enumerate((3, 3, 2, 2, 1, 1)):
        z0, length = 42 + t * 55, 540 - t * 8
        for i in range(n):
            y = (i - (n - 1) / 2.0) * 106
            g.side(CU if (i + t) % 2 == 0 else DO, [(-length / 2.0, z0), (length / 2.0, z0), (length / 2.0 - 22, z0 + 52), (-length / 2.0 + 22, z0 + 52)], y - 49, y + 49)
    for x in (-150, 150):
        g.box(RU, (x, -160, 97), (22, 8, 110))                    # straps


def ladle_car(sc):
    """An empty ladle car on a short track: steel bucket (square), navy frame. 7 x 1.5 m, 1.8 m: LOW. No orange."""
    g = sc.still()
    for sy in (-1, 1):
        g.box(ST, (0, sy * 52, 16), (700, 8, 12))
    for k in range(7):
        g.box(NC2, (-300 + k * 100, 0, 6), (22, 150, 10))
    for sx in (-1, 1):
        g.box(NV, (sx * 340, 0, 40), (16, 130, 70))               # buffer stops, cream caps
        g.box(LIT, (sx * 340, 0, 80), (22, 136, 10))
        g.box(NV, (sx * 112, 0, 105), (16, 50, 90))               # trunnion stands
        for sy in (-1, 1):
            g.cyl(ST, (sx * 100, sy * 52, 34), 22, 10, rot=(0, 0, 90), steps=8)
    g.box(NC, (0, 0, 52), (300, 120, 24))
    pyr(g, ST, 0, 0, 70, 100, 130, 180)                           # the bucket, wider at its lip
    g.box(NV, (0, 0, 110), (156, 156, 12))
    g.box(LIT, (0, 0, 174), (190, 190, 8))
    g.box(ST, (0, -98, 166), (50, 20, 10))


def rivet_forge(sc):
    """A rivet forge under a hood: a steady warm glow in the hearth, a bellows lever rocking slowly. 5.8 m: TALL."""
    g = sc.still()
    g.box(PAVE, (0, 0, 5), (420, 300, 10))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(NC, (sx * 135, sy * 95, 140), (14, 14, 260))
    pyr(g, NV, 0, 0, 270, 130, 330, 100)                          # the hood
    g.box(LIT, (0, 0, 274), (340, 340, 10))
    g.box(NV, (0, 0, 480), (76, 76, 170))                         # its flue
    g.box(LIT, (0, 0, 540), (84, 84, 16))
    g.box(RU, (0, 0, 574), (100, 100, 18))
    g.box(RU, (0, 0, 55), (190, 130, 90))                         # brick hearth
    g.box(LIT, (0, 0, 103), (140, 84, 10), glow=0.7)              # the fire bed: cream, steady
    g.box(NV, (0, 70, 150), (190, 12, 100))
    tilt_box(g, ST, (120, 0, 110), (250, 0, 130), 70, 40)         # bellows
    g.box(NC, (190, 0, 170), (10, 10, 120))
    b = sc.part(P.motion(swing=7, pivot=(190, 0, 230), axis=90.0, rate=0.3, phase=0.2))
    b.box(NC, (210, 0, 236), (170, 8, 8))
    b.box(NV, (292, 0, 220), (10, 10, 40))
    g.box(NC2, (-210, -70, 35), (70, 70, 70))                     # an anvil on a stump
    g.box(NV, (-210, -70, 84), (44, 36, 28))
    g.box(ST, (-210, -70, 108), (110, 44, 20))
    g.cyl(NC, (-200, 80, 10), 42, 60, steps=8)                    # the slack tub
    g.cyl(PAVE, (-200, 80, 66), 36, 6, steps=8)


def anvil_rack(sc):
    """An anvil block and a rack of giant tongs, navy and steel. 5.2 m long, 1.9 m: LOW. No orange."""
    g = sc.still()
    g.box(PAVE, (0, 0, 4), (620, 160, 8))
    g.box(NC, (-150, 0, 43), (96, 96, 70))
    g.box(NV, (-150, 0, 92), (54, 44, 28))
    g.box(ST, (-150, 0, 118), (130, 56, 24))
    g.box(ST, (-232, 0, 120), (40, 30, 14))
    for x in (60, 270):
        g.box(NC, (x, 0, 98), (12, 12, 180))
    g.box(ST, (165, 0, 182), (230, 10, 10))
    for k in range(4):
        x = 95 + k * 47
        tilt_box(g, ST, (x - 3, 0, 176), (x - 12, 0, 52), 9, 9)
        tilt_box(g, ST, (x + 3, 0, 176), (x + 12, 0, 52), 9, 9)
        g.box(LIT, (x, 0, 150), (14, 14, 14))
    tilt_box(g, LIT, (-70, -40, 8), (-95, -40, 120), 8, 8)        # a sledge hammer leaning on the block
    g.box(NV, (-97, -40, 128), (44, 22, 22))
    g.cyl(NC, (-270, 0, 8), 40, 56, steps=8)                      # the quench tub
    g.cyl(PAVE, (-270, 0, 60), 34, 6, steps=8)


def bollard_chain(sc):
    """Five square navy bollards with cream caps and a steel chain. 8.9 m long, 1.1 m: LOW."""
    g = sc.still()
    for k in range(5):
        x = -400 + k * 200
        g.box(PAVE, (x, 0, 4), (72, 72, 8))
        pyr(g, NV, x, 0, 8, 90, 46, 34)
        g.box(LIT, (x, 0, 103), (50, 50, 10))
        if k < 4:
            tilt_box(g, ST, (x + 16, 0, 84), (x + 100, 0, 62), 8, 8)
            tilt_box(g, ST, (x + 100, 0, 62), (x + 184, 0, 84), 8, 8)


def vent_cowl(sc):
    """A vent cowl on a post, cream with a navy fin, turning slowly to and fro. 6.9 m: TALL (stands 30 m and more out)."""
    g = sc.still()
    g.box(NC, (0, 0, 20), (96, 96, 40))
    g.box(NV, (0, 0, 60), (66, 66, 40))
    g.cyl(ST, (0, 0, 80), 11, 430, steps=6)
    g.box(NV, (0, 0, 505), (40, 40, 14))
    c = sc.part(P.motion(swing=35, pivot=(0, 0, 560), axis=P.UP, rate=0.07, phase=0.1))
    c.cyl(CREAM, (0, 0, 512), 30, 70, steps=8)
    c.box(CREAM, (0, -40, 610), (84, 170, 84))
    c.box(NV, (0, -128, 610), (100, 10, 100))
    c.box(NV, (0, 85, 612), (8, 90, 12))
    c.box(NV, (0, 200, 630), (8, 150, 120))
    c.box(LIT, (0, 200, 630), (10, 40, 120))


def foundry_lamp(sc):
    """The quarter's lamp: a stepped base, a battered steel pylon, two cream lanterns on a cross arm, steady. 6.2 m: TALL."""
    g = sc.still()
    g.box(NC, (0, 0, 15), (104, 104, 30))
    g.box(NC, (0, 0, 45), (74, 74, 30))
    g.box(NV, (0, 0, 75), (48, 48, 30))
    pyr(g, ST, 0, 0, 90, 470, 30, 18)
    g.box(NV, (0, 0, 567), (240, 14, 14))
    for sx in (-1, 1):
        lantern(g, sx * 104, 0, 525)
        tilt_box(g, NV, (sx * 14, 0, 500), (sx * 80, 0, 558), 8, 8)
    g.box(NV, (0, 0, 591), (44, 44, 34))
    g.box(LIT, (0, 0, 615), (24, 24, 14))


def sign_pylon(sc):
    """The quarter's sign, no words: a cream board with a stepped hourglass mark, three plain chips under it. 4.7 m: TALL."""
    g = sc.still()
    for sx in (-1, 1):
        g.box(NC, (sx * 96, 0, 20), (40, 40, 40))
        g.box(ST, (sx * 96, 0, 235), (14, 14, 390))
    g.box(NV, (0, 0, 335), (230, 12, 200))
    g.box(LIT, (0, -8, 335), (206, 6, 176))
    for dz, w in ((58, 120), (30, 78), (0, 30), (-30, 78), (-58, 120)):
        g.box(RU, (0, -12, 335 + dz), (w, 6, 26))
    for dz in (-76, 76):
        g.box(NV, (0, -12, 335 + dz), (150, 6, 8))
    g.box(NV, (0, 0, 200), (206, 8, 10))
    for k, c in enumerate((VI, RU, NV)):
        g.box(c, (-60 + k * 60, -4, 200), (44, 8, 44))
    g.box(NV, (0, 0, 441), (250, 20, 12))                         # stepped cap
    g.box(RU, (0, 0, 453), (170, 16, 12))
    g.box(NV, (0, 0, 465), (90, 14, 12))


def tube_rack(sc):
    """A navy A-frame rack of long copper tubes, 6.4 m long (the tubes are the orange: 6.4 m), 4.2 m: TALL. Static."""
    g = sc.still()
    for sx in (-1, 1):
        x = sx * 230
        g.box(NC, (x, 0, 8), (40, 300, 16))
        for sy in (-1, 1):
            tilt_box(g, NV, (x, sy * 125, 16), (x, sy * 34, 410), 14, 14)
        for z, w in ((110, 236), (232, 180), (352, 120)):
            g.box(NV, (x, 0, z), (14, w, 12))
        g.box(LIT, (x, 0, 414), (26, 90, 10))
    for y in (-58, 0, 58):
        rod(g, CU, (-320, y, 142), (320, y, 142), 26, steps=6)
    for y in (-34, 34):
        rod(g, DO, (-320, y, 264), (320, y, 264), 26, steps=6)
    rod(g, CU, (-320, 0, 384), (320, 0, 384), 26, steps=6)


# ------------------------------------------------------------------------------------------ the hero
def hourglass(sc):
    """THE HOURGLASS: 70 m. Two copper cones point to point in a frame of four navy columns with a stepped base and
    cap; the neck is an open cage of copper ribs in which the pale sand shows: a heap above, a heap below, and a
    stream between them whose beads ripple downwards. It never turns over. Nothing round or lit on its top."""
    g = sc.still()
    g.box(NV, (0, 0, -50), (4000, 4000, 700))                     # base: three steps, 4 m of footing
    g.box(NV, (0, 0, 400), (3500, 3500, 200))
    g.box(RU, (0, 0, 560), (3000, 3000, 120))
    g.box(VI, (0, -1752, 480), (3300, 10, 24), glow=0.5)          # the seam, on the front only
    for x in (-1350, -650, 650, 1350):
        slits(g, x, -2000, 150, h=200, w=44, pitch=90)
    door(g, 0, -2000, 0, w=260, h=250)
    g.box(PAVE, (0, -1875, 350), (500, 250, 100))
    g.box(PAVE, (0, -1812, 450), (500, 125, 100))

    def hwc(z):
        return 170 - 40 * (z - 620) / 5880.0

    for sx in (-1, 1):
        for sy in (-1, 1):
            x, y = sx * 1500, sy * 1500
            pyr(g, NV, x, y, 620, 5880, 340, 260)                 # the four columns
            g.box(RU, (x, y, 700), (480, 480, 160))
            for z in (2000, 3560, 5100):
                g.box(RU, (x, y, z), (2 * hwc(z) + 30, 2 * hwc(z) + 30, 60))
            g.box(NV, (sx * 1650, sy * 1650, 6780), (300, 300, 160))
            tilt_box(g, NV, (sx * 640, sy * 640, 1500), (sx * 1400, sy * 1400, 1700), 40, 40)     # braces to the bulbs
            tilt_box(g, NV, (sx * 640, sy * 640, 5620), (sx * 1400, sy * 1400, 5420), 40, 40)
            tilt_box(g, NV, (sx * 130, sy * 130, 3560), (sx * 1420, sy * 1420, 3560), 36, 36)     # and to the neck
        for z in (1500, 2900, 4300, 5700):
            g.box(LIT, (sx * 1500, -1500 - hwc(z) - 2, z), (44, 8, 280), glow=0.5)                # the front columns' slits
        slits(g, sx * 900, -1850, 6600, h=110, w=40, pitch=80)
    g.box(NV, (0, 0, 6600), (3700, 3700, 200))                    # cap: three steps
    g.box(RU, (0, 0, 6760), (3000, 3000, 120))
    g.box(NV, (0, 0, 6910), (2200, 2200, 180))
    g.box(VI, (0, -1856, 6540), (3500, 10, 24), glow=0.5)
    g.cyl(CU, (0, 0, 620), 1300, 2100, steps=12, top_r=420)       # the lower bulb
    g.cyl(CU, (0, 0, 4400), 420, 2100, steps=12, top_r=1300)      # the upper bulb
    g.cyl(DO, (0, 0, 620), 1340, 90, steps=12)
    g.cyl(DO, (0, 0, 6410), 1340, 90, steps=12)
    for z, r in ((1085, 1112), (1885, 778), (5201, 778), (6001, 1112)):
        g.cyl(RU, (0, 0, z), r, 34, steps=12)                     # rivet bands
    for z in (2700, 4360):
        g.cyl(NV, (0, 0, z), 440, 60, steps=12)                   # the neck's collars
    g.cyl(NV, (0, 0, 3520), 190, 80, steps=8)
    import math
    for k in range(6):                                            # the neck: six ribs below, six above (none dead ahead)
        a = math.radians(60 * k)
        ca, sa = math.cos(a), math.sin(a)
        tilt_box(g, CU, (420 * ca, 420 * sa, 2740), (150 * ca, 150 * sa, 3560), 50, 50)
        tilt_box(g, CU, (150 * ca, 150 * sa, 3560), (420 * ca, 420 * sa, 4380), 50, 50)
    g.cyl(LIT, (0, 0, 3640), 60, 760, steps=8, top_r=380, glow=0.45)     # the sand above
    g.cyl(LIT, (0, 0, 2720), 380, 420, steps=8, top_r=30, glow=0.45)     # the heap below
    g.cyl(LIT, (0, 0, 3140), 20, 520, steps=6, glow=0.6)                 # the stream
    for k in range(6):                                                   # its beads: the phase climbs with height, so the ripple runs down
        b = sc.part(P.motion(bob=-35.0, rate=0.6, phase=k / 6.0))
        b.cyl(LIT, (0, 0, 3170 + k * 78), 38, 60, steps=6, glow=0.7)


JOBS = [
    ('SM_S1D8_PowerHall', power_hall, STILL),
    ('SM_S1D8_Foundry', foundry),
    ('SM_S1D8_SwitchHouse', switch_house, STILL),
    ('SM_S1D8_Club', club),
    ('SM_S1D8_SmithsRow', smiths_row, STILL),
    ('SM_S1D8_GateBastion', gate_bastion),
    ('SM_S1D8_QuenchTower', quench_tower),
    ('SM_S1D8_IngotStack', ingot_stack, STILL),
    ('SM_S1D8_LadleCar', ladle_car, STILL),
    ('SM_S1D8_RivetForge', rivet_forge),
    ('SM_S1D8_AnvilRack', anvil_rack, STILL),
    ('SM_S1D8_BollardChain', bollard_chain, STILL),
    ('SM_S1D8_VentCowl', vent_cowl, dict(grow=(190, 60, 60), grow_down=(190, 60, 0))),
    ('SM_S1D8_FoundryLamp', foundry_lamp, STILL),
    ('SM_S1D8_SignPylon', sign_pylon, STILL),
    ('SM_S1D8_TubeRack', tube_rack, STILL),
    ('SM_S1D8_Hourglass', hourglass, dict(far=True)),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilds them
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
