"""Ice Mountain, Stage 1, builder s1d7: district 7 "The ice again" (17 to 19.7 km). /Game/World/IceMountain/SM_S1D7_*.
Run: import ft_world_ice_s1d7 as K; K.run()

THE CRYSTAL MINE. A mining camp cut into the glacier: braced timber headframes, trestles, ore chutes and bunkhouses
on cribbing, one to three storeys under steep plank roofs, everything held up by diagonals; and clusters of big
faceted crystal that glow. Dark timber (lead) and timber, lit teal crystal, steel, lamp yellow, charcoal; snow only
as slabs lying on roofs.

Eight buildings (bunkhouse, ore chute house, winding house, assay office, trestle, tool store, breaker, tank
tower), nine props (ore cart, pit props, hand winch, sorting table, buffer stop: LOW, 1.95 m and under, nothing lit;
crystal rock, lamp post, shift bell, sluice: tall) and the hero, THE CRYSTAL HEADFRAME (51 m).

What the hill asks (FTDressL5.inl): the road falls 7 in 100 and the land with it, so every building has 3 m of
footing under z = 0 and the low props 45 cm. Glow only on pieces placed 24 m or more out (the tall props stand at
27.6 m, the buildings at 54 m and more); the low props carry crystal that is not lit.
No figures (the people are s1people's), no vehicle, nothing orange, nothing white or pale blue that stands upright,
no words. What moves (all in the vertex shader, all slow): the winding wheels wind one way and back, a kibble
swings on the chute house's hoist, the bunkhouse's washing, the assay office's balance sign and weather vane, the
tank tower's vane, the shift bell, a tally tag on the lamp post, the headframe's wheel, cage and pennant.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/IceMountain'
DT, TB, TEAL, STEEL, LAMP, CHAR, SNOW = '#6a4c36', '#9a6a44', '#2fc6c0', '#9aa4b8', '#ffe28a', '#3b3f52', '#f4f9ff'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))     # no moving part: true bounds
LIT = 0.5                                              # a lamp-lit window's steady glow
XGLOW = 0.7                                            # the crystal's steady glow
FOOT = 300.0                                           # a building's footing under z = 0


# ------------------------------------------------------------------------------------------ helpers
def _ccw(pts):
    a = 0.0
    for i in range(len(pts)):
        j = (i + 1) % len(pts)
        a += pts[i][0] * pts[j][1] - pts[j][0] * pts[i][1]
    return list(pts) if a > 0 else list(reversed(pts))


def cone(m, col, a, b, r0, r1, steps=6, glow=0.0):
    """A cone from a to b (rod()'s own turn, with a second radius)."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r0, L, rot=(pitch, yaw, 0), steps=steps, glow=glow, top_r=r1)


def post(m, col, x, y, z0, z1, w=30.0):
    m.box(col, (x, y, (z0 + z1) / 2.0), (w, w, z1 - z0))


def cross_x(m, col, x0, x1, y, z0, z1, w=16.0):
    """An X of two diagonals in the wall that faces the road (the plane y)."""
    tilt_box(m, col, (x0, y, z0), (x1, y, z1), w, w)
    tilt_box(m, col, (x0, y, z1), (x1, y, z0), w, w)


def crib(m, cx, cy, wx, wy, z0, z1, n=3, log=28.0):
    """Cribbing: crossed timbers stacked log-cabin fashion, a dark core between them."""
    h = (z1 - z0) / float(n)
    m.box(CHAR, (cx, cy, (z0 + z1) / 2.0), (wx - 2.0 * log, wy - 2.0 * log, z1 - z0))
    for k in range(n):
        z = z0 + (k + 0.5) * h
        col = DT if k % 2 else TB
        for s in (-1, 1):
            if k % 2 == 0:
                m.box(col, (cx, cy + s * (wy / 2.0 - log / 2.0), z), (wx + 30.0, log, h))
            else:
                m.box(col, (cx + s * (wx / 2.0 - log / 2.0), cy, z), (log, wy + 30.0, h))


def win(m, x, y, z, w, h, lit=True):
    """A window on a wall that faces the road (the wall's face is at y)."""
    m.box(TB, (x, y - 3.0, z), (w + 18.0, 10.0, h + 18.0))
    m.box(LAMP if lit else CHAR, (x, y - 8.0, z), (w, 6.0, h), glow=LIT if lit else 0.0)


def roof_y(m, cx, half, y0, y1, z, rise, over=45.0, snow=True):
    """A steep plank roof, ridge along Y: its gable faces the road."""
    m.side(TB, _ccw([(cx - half - over, z - 30.0), (cx + half + over, z - 30.0), (cx, z + rise)]), y0 - over, y1 + over)
    m.side(DT, _ccw([(cx - half + 10.0, z - 24.0), (cx + half - 10.0, z - 24.0), (cx, z + rise - 70.0)]), y0 - over - 8.0, y0 - over)
    m.box(CHAR, (cx, (y0 + y1) / 2.0, z + rise + 2.0), (30.0, y1 - y0 + 2.0 * over + 12.0, 16.0))
    if snow:
        yc, L = (y0 + y1) / 2.0, y1 - y0 + 2.0 * over + 10.0
        for s in (-1, 1):
            a = (cx + s * (half + over + 4.0), yc, z - 30.0 + 14.0)
            b = (cx + s * (half + over) * 0.5, yc, z - 30.0 + (rise + 30.0) * 0.5 + 14.0)
            tilt_box(m, SNOW, a, b, L, 14.0)


def roof_x(m, x0, x1, cy, half, z, rise, over=45.0, snow=True):
    """A steep plank roof, ridge along X: two slabs, a ridge board, stepped plank gables at both ends."""
    xc, L = (x0 + x1) / 2.0, x1 - x0 + 2.0 * over
    for s in (-1, 1):
        tilt_box(m, TB, (xc, cy + s * (half + over), z - 30.0), (xc, cy, z + rise), L, 22.0)
        if snow:
            a = (xc, cy + s * (half + over + 4.0), z - 30.0 + 18.0)
            b = (xc, cy + s * (half + over) * 0.5, z - 30.0 + (rise + 30.0) * 0.5 + 18.0)
            tilt_box(m, SNOW, a, b, L + 8.0, 14.0)
    m.box(CHAR, (xc, cy, z + rise + 6.0), (L + 10.0, 30.0, 18.0))
    for xe in (x0 + 8.0, x1 - 8.0):
        for k in range(3):
            w = 2.0 * half * (1.0 - (k + 0.5) / 3.0)
            m.box(DT if k % 2 else TB, (xe, cy, z + (k + 0.5) * rise / 3.0), (16.0, w, rise / 3.0))


def crystal(m, x, y, z, h, r, lean=0.0, yaw=0.0, glow=0.0, steps=6):
    """A faceted crystal: a shaft that widens a little and a pointed tip, leaning `lean` degrees towards `yaw`."""
    a, b = math.radians(lean), math.radians(yaw)
    d = (math.sin(a) * math.cos(b), math.sin(a) * math.sin(b), math.cos(a))
    p1 = (x + d[0] * h * 0.72, y + d[1] * h * 0.72, z + d[2] * h * 0.72)
    p2 = (x + d[0] * h, y + d[1] * h, z + d[2] * h)
    cone(m, TEAL, (x, y, z), p1, r * 0.8, r, steps, glow)
    cone(m, TEAL, p1, p2, r, max(4.0, r * 0.1), steps, glow)


def wheel(sc, c, R, n=10, spokes=3, w=16.0, t=20.0, swing=150.0, rate=0.05, phase=0.0):
    """A spoked winding wheel in the plane that faces the road, about c. It winds one way and back (a hoist does)."""
    m = sc.part(P.motion(swing=swing, pivot=c, axis=90.0, rate=rate, phase=phase))
    pts = [(c[0] + R * math.cos(2.0 * math.pi * k / n), c[1], c[2] + R * math.sin(2.0 * math.pi * k / n)) for k in range(n)]
    for k in range(n):
        tilt_box(m, STEEL, pts[k], pts[(k + 1) % n], w, t)
    for k in range(spokes):
        a = math.pi * (k + 0.5) / spokes
        dx, dz = R * math.cos(a), R * math.sin(a)
        tilt_box(m, DT, (c[0] - dx, c[1], c[2] - dz), (c[0] + dx, c[1], c[2] + dz), w * 0.7, t * 0.7)
    rod(m, STEEL, (c[0], c[1] - w * 0.8, c[2]), (c[0], c[1] + w * 0.8, c[2]), R * 0.16, steps=8)


def cart(m, x, y, z, glow=0.0, n=5, seed=1):
    """An ore cart standing on rails whose top is z, along X: a timber tub in steel bands, four wheels, a heap of
    crystal (164 cm long, 96 wide, about 1.6 m tall with its load)."""
    m.box(DT, (x, y, z + 62.0), (150.0, 84.0, 64.0))
    m.box(TB, (x, y, z + 96.0), (164.0, 96.0, 10.0))
    for dx in (-52.0, 52.0):
        m.box(STEEL, (x + dx, y, z + 62.0), (10.0, 90.0, 68.0))
    m.box(CHAR, (x, y, z + 26.0), (130.0, 50.0, 10.0))
    for dx in (-45.0, 45.0):
        for s in (-1, 1):
            rod(m, STEEL, (x + dx, y + s * 36.0, z + 20.0), (x + dx, y + s * 46.0, z + 20.0), 20.0, steps=8)
    r = random.Random(seed)
    for k in range(n):
        crystal(m, x - 50.0 + k * 100.0 / max(n - 1, 1), y + r.uniform(-16, 16), z + 94.0, r.uniform(40, 68), r.uniform(10, 14),
                r.uniform(0, 20), r.uniform(0, 360), glow=glow, steps=5)
    m.box(STEEL, (x + 88.0, y, z + 50.0), (22.0, 12.0, 10.0))


def track(m, length, n, foot=43.0):
    """A short length of mine rail along X on sleepers; the rails' top is z = 25."""
    for s in (-1, 1):
        m.box(STEEL, (0, s * 36.0, 20.0), (length, 8.0, 10.0))
    for k in range(n):
        x = -length / 2.0 + (k + 0.5) * length / n
        m.box(DT, (x, 0, (15.0 - foot) / 2.0), (26.0, 124.0, 15.0 + foot))


def lantern(m, x, y, z, glow=1.0, s=1.0):
    """A miner's lantern hanging from (x, y, z): a charcoal cap and foot, a lamp-yellow glass."""
    m.box(CHAR, (x, y, z - 12.0 * s), (5.0, 5.0, 24.0 * s))
    m.box(CHAR, (x, y, z - 27.0 * s), (40.0 * s, 40.0 * s, 6.0))
    m.box(LAMP, (x, y, z - 53.0 * s), (30.0 * s, 30.0 * s, 46.0 * s), glow=glow)
    m.box(CHAR, (x, y, z - 79.0 * s), (40.0 * s, 40.0 * s, 6.0))


# ------------------------------------------------------------------------------------------ the buildings
def bunkhouse(sc):
    """The bunkhouse on cribbing: 11 x 6.4 m, two floors of lit windows under a steep plank roof with two dormers,
    a porch with its stair, a stove pipe, a line of washing (it swings). 11.6 m tall."""
    g = sc.still()
    W, D, F, H = 1100.0, 640.0, 190.0, 560.0
    top = F + H
    g.box(DT, (0, 0, -FOOT / 2.0), (W - 60.0, D - 60.0, FOOT))
    for x in (-400.0, 0.0, 400.0):
        crib(g, x, 0, 240.0, D + 20.0, 0.0, F, 3)
    for x in (-200.0, 200.0):
        cross_x(g, TB, x - 80.0, x + 80.0, -D / 2.0 + 14.0, 20.0, F - 20.0, 14.0)
    g.box(TB, (0, 0, F + 10.0), (W + 40.0, D + 40.0, 24.0))
    g.box(DT, (0, 0, F + H / 2.0), (W, D, H))
    g.box(TB, (0, 0, F + 285.0), (W + 16.0, D + 16.0, 20.0))
    for sx in (-1, 1):
        post(g, TB, sx * W / 2.0, -D / 2.0, F, top, 34.0)
    for i in range(5):
        x = -400.0 + i * 200.0
        if i != 2:
            win(g, x, -D / 2.0, F + 150.0, 90.0, 110.0, lit=(i % 2 == 0))
        win(g, x, -D / 2.0, F + 420.0, 90.0, 100.0, lit=(i != 1))
    # the door, its porch and the stair down the side of it
    g.box(TB, (0, -D / 2.0 - 3.0, F + 112.0), (130.0, 10.0, 222.0))
    g.box(CHAR, (0, -D / 2.0 - 8.0, F + 108.0), (104.0, 6.0, 204.0))
    g.box(TB, (0, -D / 2.0 - 110.0, F - 8.0), (360.0, 200.0, 16.0))
    for sx in (-1, 1):
        post(g, DT, sx * 165.0, -D / 2.0 - 195.0, 0.0, F + 250.0, 16.0)
    tilt_box(g, TB, (0, -D / 2.0 - 230.0, F + 245.0), (0, -D / 2.0, F + 330.0), 400.0, 14.0)
    tilt_box(g, SNOW, (0, -D / 2.0 - 232.0, F + 258.0), (0, -D / 2.0 - 90.0, F + 310.0), 404.0, 12.0)
    for k in range(4):
        hh = F - 20.0 - k * 46.0
        g.box(TB, (205.0 + k * 48.0, -D / 2.0 - 110.0, hh / 2.0), (48.0, 160.0, hh))
    lantern(g, 100.0, -D / 2.0 - 30.0, F + 262.0, glow=0.9, s=0.7)
    # the roof, two dormers, the stove pipe
    roof_x(g, -W / 2.0, W / 2.0, 0.0, D / 2.0, top, 400.0)
    for x in (-270.0, 270.0):
        g.box(DT, (x, -160.0, top + 160.0), (150.0, 200.0, 180.0))
        g.side(TB, _ccw([(x - 100.0, top + 245.0), (x + 100.0, top + 245.0), (x, top + 345.0)]), -280.0, -50.0)
        win(g, x, -260.0, top + 180.0, 70.0, 84.0, lit=(x < 0))
    g.cyl(STEEL, (390.0, 130.0, top + 150.0), 20.0, 370.0, steps=6)
    g.cyl(CHAR, (390.0, 130.0, top + 520.0), 36.0, 34.0, steps=6, top_r=8.0)
    # a line of washing from the porch to a pole (plain cloths in the camp's own colours)
    yl, zl = -D / 2.0 - 195.0, F + 232.0
    post(g, DT, -520.0, yl, -100.0, zl + 16.0, 14.0)
    g.box(CHAR, (-342.0, yl, zl), (356.0, 5.0, 5.0))
    c = sc.part(P.motion(swing=8.0, pivot=(-342.0, yl, zl), axis=0.0, rate=0.35, phase=0.2))
    for k, (col, w, h) in enumerate(((LAMP, 60.0, 70.0), (STEEL, 46.0, 90.0), (TB, 70.0, 56.0), (STEEL, 40.0, 76.0))):
        c.box(col, (-480.0 + k * 86.0, yl, zl - h / 2.0 - 2.0), (w, 5.0, h))


def chute_house(sc):
    """The ore chute house: a tall lean-to (16.4 m at its high side), braced on its front, a chute out of its side
    down to a bin heaped with crystal, a kibble swinging on the hoist beam, a stove stack. 18.3 m tall."""
    g = sc.still()
    F = 160.0
    g.box(DT, (0, 0, -FOOT / 2.0), (640.0, 520.0, FOOT))
    for x in (-220.0, 220.0):
        crib(g, x, 0, 220.0, 590.0, 0.0, F, 2)
    g.side(DT, _ccw([(-340.0, F), (340.0, F), (340.0, 1000.0), (-340.0, 1640.0)]), -280.0, 280.0)
    tilt_box(g, TB, (-392.0, 0, 1702.0), (392.0, 0, 966.0), 650.0, 24.0)
    tilt_box(g, SNOW, (0.0, 0, 1356.0), (388.0, 0, 992.0), 656.0, 16.0)
    post(g, TB, -340.0, -285.0, F, 1640.0, 30.0)
    post(g, TB, 340.0, -285.0, F, 1000.0, 30.0)
    for z in (F + 10.0, 480.0, 800.0):
        g.box(TB, (0, -285.0, z), (700.0, 22.0, 22.0))
    g.box(TB, (-110.0, -285.0, 1150.0), (460.0, 22.0, 22.0))
    cross_x(g, TB, -325.0, 325.0, -288.0, 500.0, 780.0, 16.0)
    tilt_box(g, TB, (-325.0, -288.0, 820.0), (110.0, -288.0, 1135.0), 16.0, 16.0)
    tilt_box(g, TB, (-325.0, -288.0, 1170.0), (-60.0, -288.0, 1380.0), 16.0, 16.0)
    win(g, -190.0, -280.0, 330.0, 84.0, 104.0)
    win(g, 190.0, -280.0, 330.0, 84.0, 104.0, lit=False)
    win(g, -200.0, -280.0, 990.0, 80.0, 100.0)
    win(g, -230.0, -280.0, 1420.0, 70.0, 90.0)
    g.box(TB, (0, -283.0, F + 112.0), (136.0, 10.0, 224.0))
    g.box(CHAR, (0, -288.0, F + 108.0), (108.0, 6.0, 206.0))
    for k in range(3):
        hh = F - 14.0 - k * 50.0
        g.box(TB, (0, -310.0 - k * 44.0, hh / 2.0), (170.0, 44.0, hh))
    # the chute: out of a hatch in the low side, down two bents to the bin
    g.box(CHAR, (342.0, -80.0, 770.0), (8.0, 150.0, 130.0))
    tilt_box(g, TB, (340.0, -80.0, 720.0), (770.0, -80.0, 300.0), 130.0, 14.0)
    for s in (-1, 1):
        tilt_box(g, DT, (340.0, -80.0 + s * 68.0, 746.0), (770.0, -80.0 + s * 68.0, 326.0), 12.0, 50.0)
    for x, zt in ((480.0, 566.0), (640.0, 410.0)):
        for s in (-1, 1):
            tilt_box(g, DT, (x, -80.0 + s * 115.0, -100.0), (x, -80.0 + s * 62.0, zt), 16.0, 16.0)
        g.box(DT, (x, -80.0, zt), (20.0, 156.0, 16.0))
    g.box(DT, (810.0, -80.0, 60.0), (200.0, 230.0, 340.0))
    for z in (60.0, 190.0):
        g.box(STEEL, (810.0, -80.0, z), (208.0, 238.0, 12.0))
    for dx, dy, h, r, lean, yaw in ((-40, -30, 120, 26, 8, 200), (35, 20, 150, 30, 6, 30), (60, -60, 90, 20, 22, -50), (-60, 50, 80, 18, 26, 130), (0, 70, 70, 16, 30, 90)):
        crystal(g, 810.0 + dx, -80.0 + dy, 228.0, h, r, lean, yaw, glow=XGLOW, steps=5)
    # the hoist beam out over the front, a kibble on its rope (it swings)
    g.box(STEEL, (-250.0, -360.0, 1560.0), (24.0, 200.0, 24.0))
    tilt_box(g, TB, (-250.0, -288.0, 1430.0), (-250.0, -430.0, 1550.0), 14.0, 14.0)
    k = sc.part(P.motion(swing=4.0, pivot=(-250.0, -430.0, 1550.0), axis=0.0, rate=0.2, phase=0.1))
    k.box(CHAR, (-250.0, -430.0, 1335.0), (5.0, 5.0, 430.0))
    k.cyl(STEEL, (-250.0, -430.0, 1050.0), 36.0, 72.0, steps=8, top_r=46.0)
    crystal(k, -250.0, -430.0, 1110.0, 70.0, 16.0, 8.0, 40.0, glow=XGLOW, steps=5)
    g.cyl(STEEL, (-180.0, 140.0, 1480.0), 24.0, 310.0, steps=6)
    g.cyl(CHAR, (-180.0, 140.0, 1790.0), 42.0, 38.0, steps=6, top_r=8.0)
    lantern(g, 100.0, -300.0, F + 280.0, glow=0.9, s=0.7)


def winding_house(sc):
    """The winding house: an engine house gable-on to the road with the big winding wheel half out of its front wall
    (it winds one way and back), the rope up to a pulley post, a tall steel stack. 12.9 m tall."""
    g = sc.still()
    W, D, F, H = 940.0, 660.0, 150.0, 470.0
    g.box(DT, (0, 0, -FOOT / 2.0), (W - 40.0, D - 40.0, FOOT))
    g.box(CHAR, (0, 0, F / 2.0), (W + 30.0, D + 30.0, F))
    for x in (-360.0, 0.0, 360.0):
        g.box(TB, (x, -D / 2.0 - 18.0, F / 2.0), (60.0, 16.0, F))
    g.box(DT, (0, 0, F + H / 2.0), (W, D, H))
    g.box(TB, (0, 0, F + 10.0), (W + 24.0, D + 24.0, 22.0))
    for sx in (-1, 1):
        post(g, TB, sx * W / 2.0, -D / 2.0, F, F + H, 34.0)
    roof_y(g, 0.0, W / 2.0, -D / 2.0, D / 2.0, F + H, 520.0)
    rod(g, TB, (0, -D / 2.0 - 56.0, F + H + 250.0), (0, -D / 2.0 - 50.0, F + H + 250.0), 62.0, steps=8)
    rod(g, LAMP, (0, -D / 2.0 - 60.0, F + H + 250.0), (0, -D / 2.0 - 54.0, F + H + 250.0), 46.0, steps=8, glow=LIT)
    # the wheel in its slot, on an A-frame bearing
    c = (-170.0, -D / 2.0 - 52.0, F + 300.0)
    g.box(CHAR, (c[0], -D / 2.0 - 4.0, c[2]), (150.0, 8.0, 560.0))
    wheel(sc, c, 270.0, n=10, spokes=3, w=18.0, t=22.0, swing=150.0, rate=0.05)
    for s in (-1, 1):
        tilt_box(g, TB, (c[0] + s * 210.0, c[1] - 46.0, 0.0), (c[0], c[1] - 46.0, c[2]), 22.0, 22.0)
    g.box(STEEL, (c[0], c[1] - 46.0, c[2]), (56.0, 30.0, 56.0))
    post(g, DT, 440.0, c[1], -100.0, 900.0, 26.0)
    rod(g, STEEL, (440.0, c[1] - 16.0, 900.0), (440.0, c[1] + 16.0, 900.0), 50.0, steps=8)
    tilt_box(g, CHAR, (c[0] + 60.0, c[1], c[2] + 262.0), (440.0, c[1], 948.0), 8.0, 8.0)
    tilt_box(g, TB, (440.0, c[1], 420.0), (330.0, -D / 2.0, 560.0), 14.0, 14.0)
    # door, windows, the stack, a little crystal by the door
    g.box(TB, (300.0, -D / 2.0 - 3.0, F + 112.0), (130.0, 10.0, 224.0))
    g.box(CHAR, (300.0, -D / 2.0 - 8.0, F + 108.0), (104.0, 6.0, 206.0))
    for k in range(3):
        hh = F - 14.0 - k * 46.0
        g.box(TB, (300.0, -D / 2.0 - 40.0 - k * 42.0, hh / 2.0), (160.0, 42.0, hh))
    win(g, 130.0, -D / 2.0, F + 260.0, 90.0, 130.0)
    lantern(g, 215.0, -D / 2.0 - 30.0, F + 420.0, glow=0.9, s=0.7)
    g.cyl(STEEL, (300.0, 200.0, F + H), 36.0, 600.0, steps=8, top_r=30.0)
    for z in (F + H + 330.0, F + H + 560.0):
        g.cyl(CHAR, (300.0, 200.0, z), 40.0, 16.0, steps=8)
    g.cyl(CHAR, (300.0, 200.0, F + H + 600.0), 50.0, 46.0, steps=8, top_r=10.0)
    for dx, h, r, lean, yaw in ((0, 110, 22, 6, 250), (36, 70, 16, 24, -30), (-34, 60, 14, 26, 200)):
        crystal(g, 420.0 + dx, -D / 2.0 - 110.0, 0.0, h, r, lean, yaw, glow=XGLOW, steps=5)


def assay_office(sc):
    """The assay office: a small cabin on cribbing, gable-on, its roof running out over a porch where a big crystal
    stands on a plinth; a balance hangs for a sign (a beam and two pans, no words; it swings), a pick for a weather
    vane on the ridge (it turns a little). 9.4 m tall."""
    g = sc.still()
    W, D, F, H = 520.0, 460.0, 170.0, 300.0
    yf = -D / 2.0
    g.box(DT, (0, 0, -FOOT / 2.0), (W - 40.0, D - 40.0, FOOT))
    crib(g, 0, 0, W + 10.0, D + 10.0, 0.0, F, 3)
    crib(g, 0, yf - 110.0, W + 10.0, 200.0, 0.0, F - 16.0, 2)
    g.box(DT, (0, 0, F + H / 2.0), (W, D, H))
    g.box(TB, (0, 0, F + 8.0), (W + 30.0, D + 30.0, 20.0))
    for sx in (-1, 1):
        post(g, TB, sx * W / 2.0, yf, F, F + H, 30.0)
    roof_y(g, 0.0, W / 2.0, yf - 180.0, D / 2.0, F + H, 330.0)
    # the porch: deck, posts, a rail with a gap for the steps (they go down the side)
    g.box(TB, (0, yf - 100.0, F - 8.0), (W + 20.0, 200.0, 16.0))
    for sx in (-1, 1):
        post(g, DT, sx * (W / 2.0 - 8.0), yf - 190.0, F, F + H - 20.0, 18.0)
    g.box(TB, (60.0, yf - 190.0, F + 90.0), (W - 140.0, 10.0, 10.0))
    for k in range(4):
        hh = F - 20.0 - k * 42.0
        g.box(TB, (-W / 2.0 - 34.0 - k * 46.0, yf - 100.0, hh / 2.0), (46.0, 150.0, hh))
    g.box(TB, (-120.0, yf - 3.0, F + 108.0), (120.0, 10.0, 216.0))
    g.box(CHAR, (-120.0, yf - 8.0, F + 104.0), (96.0, 6.0, 198.0))
    win(g, 110.0, yf, F + 170.0, 140.0, 120.0)
    for s in (-1, 1):
        g.box(DT, (110.0 + s * 96.0, yf - 6.0, F + 170.0), (34.0, 8.0, 130.0))
    # the crystal on its plinth, two small ones at its foot; a barrel and a box by the door
    g.box(CHAR, (150.0, yf - 110.0, F + 26.0), (76.0, 76.0, 52.0))
    crystal(g, 150.0, yf - 110.0, F + 52.0, 160.0, 26.0, 4.0, 30.0, glow=XGLOW)
    crystal(g, 124.0, yf - 132.0, F + 52.0, 60.0, 12.0, 26.0, 220.0, glow=XGLOW, steps=5)
    crystal(g, 178.0, yf - 96.0, F + 52.0, 50.0, 11.0, 28.0, 20.0, glow=XGLOW, steps=5)
    g.cyl(TB, (-215.0, yf - 60.0, F), 30.0, 80.0, steps=8, top_r=30.0)
    g.cyl(STEEL, (-215.0, yf - 60.0, F + 34.0), 32.0, 10.0, steps=8)
    g.box(DT, (-30.0, yf - 40.0, F + 24.0), (60.0, 50.0, 48.0))
    # the sign: a balance on a steel bracket off the front post
    bx, by, bz = -W / 2.0 - 96.0, yf - 190.0, F + H - 40.0
    g.box(STEEL, (-W / 2.0 - 60.0, by, bz + 6.0), (130.0, 8.0, 8.0))
    tilt_box(g, STEEL, (-W / 2.0 - 4.0, by, bz - 60.0), (-W / 2.0 - 70.0, by, bz + 2.0), 6.0, 6.0)
    s = sc.part(P.motion(swing=7.0, pivot=(bx, by, bz), axis=0.0, rate=0.3, phase=0.3))
    s.box(CHAR, (bx, by, bz - 14.0), (5.0, 5.0, 28.0))
    s.box(STEEL, (bx, by, bz - 30.0), (110.0, 6.0, 8.0))
    for sx in (-1, 1):
        s.box(CHAR, (bx + sx * 50.0, by, bz - 52.0), (4.0, 4.0, 40.0))
        s.cyl(LAMP, (bx + sx * 50.0, by, bz - 78.0), 22.0, 8.0, steps=8, top_r=26.0)
    # stove pipe; the weather vane (a pick) on the ridge
    g.cyl(STEEL, (150.0, 120.0, F + H + 60.0), 16.0, 260.0, steps=6)
    g.cyl(CHAR, (150.0, 120.0, F + H + 320.0), 30.0, 28.0, steps=6, top_r=6.0)
    zr = F + H + 330.0
    g.box(STEEL, (0, yf - 120.0, zr + 62.0), (8.0, 8.0, 124.0))
    v = sc.part(P.motion(swing=26.0, pivot=(0, yf - 120.0, zr + 110.0), axis=P.UP, rate=0.12, phase=0.6))
    v.box(STEEL, (0, yf - 120.0, zr + 110.0), (140.0, 6.0, 8.0))
    tilt_box(v, STEEL, (70.0, yf - 120.0, zr + 110.0), (46.0, yf - 120.0, zr + 142.0), 6.0, 10.0)
    tilt_box(v, STEEL, (70.0, yf - 120.0, zr + 110.0), (46.0, yf - 120.0, zr + 78.0), 6.0, 10.0)
    v.box(TEAL, (-58.0, yf - 120.0, zr + 110.0), (36.0, 5.0, 36.0))


def trestle(sc):
    """The trestle: 24 m of ore railway on four braced bents, 7.6 m up, a cart of crystal standing on it, a walkway
    rail, a lamp on a post at one end and a buffer at the other. Nothing on it moves. 10.4 m tall."""
    g = sc.still()
    ZD, HW = 760.0, 150.0
    xs = (-1020.0, -340.0, 340.0, 1020.0)
    for x in xs:
        for s in (-1, 1):
            tilt_box(g, DT, (x, s * (HW + 130.0), -FOOT), (x, s * (HW - 30.0), ZD - 40.0), 30.0, 30.0)
        g.box(TB, (x, 0, ZD - 30.0), (34.0, 2.0 * HW + 40.0, 30.0))
        g.box(TB, (x, 0, 330.0), (26.0, 2.0 * HW + 200.0, 24.0))
    for i in range(3):
        cross_x(g, TB, xs[i] + 10.0, xs[i + 1] - 10.0, -(HW + 62.0), 60.0, ZD - 80.0, 18.0)
    g.box(DT, (0, -(HW + 76.0), 330.0), (2100.0, 16.0, 20.0))
    for s in (-1, 1):
        g.box(DT, (0, s * 90.0, ZD - 2.0), (2360.0, 30.0, 40.0))
        g.box(STEEL, (0, s * 36.0, ZD + 34.0), (2360.0, 8.0, 10.0))
    for k in range(10):
        g.box(TB, (-1080.0 + k * 240.0, 0, ZD + 24.0), (44.0, 310.0, 12.0))
    g.box(TB, (0, -110.0, ZD + 22.0), (2360.0, 70.0, 8.0))
    for k in range(6):
        post(g, DT, -1100.0 + k * 440.0, -148.0, ZD + 10.0, ZD + 130.0, 10.0)
    g.box(DT, (0, -148.0, ZD + 128.0), (2210.0, 10.0, 10.0))
    cart(g, -300.0, 0, ZD + 39.0, glow=XGLOW, n=5, seed=11)
    # the lamp at one end, the buffer at the other, snow lying on the deck's ends
    post(g, DT, 1080.0, -140.0, ZD, ZD + 250.0, 14.0)
    g.box(DT, (1040.0, -140.0, ZD + 244.0), (100.0, 12.0, 12.0))
    lantern(g, 1000.0, -140.0, ZD + 238.0, glow=1.0, s=0.8)
    for s in (-1, 1):
        tilt_box(g, DT, (-1060.0, s * 36.0, ZD + 30.0), (-1150.0, s * 36.0, ZD + 130.0), 16.0, 16.0)
    g.box(TB, (-1146.0, 0, ZD + 122.0), (22.0, 130.0, 30.0))
    g.box(SNOW, (700.0, 40.0, ZD + 36.0), (300.0, 180.0, 10.0))
    g.box(SNOW, (-760.0, 30.0, ZD + 36.0), (240.0, 160.0, 10.0))


def tool_store(sc):
    """The tool store: a low vault of logs (6 m wide, 4.2 m high), ribbed with lighter timber, a strapped double door
    under a lamp, picks and a shovel leaning by it, a grindstone, pit props stacked along its side, a stove pipe."""
    g = sc.still()
    R, D = 300.0, 560.0
    yf = -D / 2.0

    def arch(rr, lift=90.0):
        pts = [(rr, 0.0)] + [(rr * math.cos(math.radians(a)), lift + rr * 1.05 * math.sin(math.radians(a))) for a in range(0, 181, 20)] + [(-rr, 0.0)]
        return _ccw(pts)

    g.box(DT, (0, 0, -FOOT / 2.0), (2.0 * R - 20.0, D - 20.0, FOOT))
    g.side(DT, arch(R), yf, D / 2.0)
    for y in (yf - 8.0, -90.0, 90.0, D / 2.0 - 14.0):
        g.side(TB, arch(R + 14.0), y, y + 22.0)
    g.box(SNOW, (0, 10.0, 90.0 + R * 1.05 + 12.0), (250.0, D - 60.0, 14.0))
    g.box(TB, (0, yf - 8.0, 128.0), (236.0, 12.0, 256.0))
    g.box(CHAR, (0, yf - 15.0, 124.0), (8.0, 4.0, 240.0))
    for sx in (-1, 1):
        for z in (60.0, 190.0):
            g.box(STEEL, (sx * 64.0, yf - 15.0, z), (96.0, 4.0, 12.0))
    g.box(TB, (0, yf - 34.0, 322.0), (80.0, 60.0, 8.0))
    lantern(g, 0.0, yf - 36.0, 318.0, glow=1.0, s=0.6)
    # tools by the door, the grindstone
    tilt_box(g, TB, (200.0, yf - 70.0, 0.0), (190.0, yf - 12.0, 150.0), 6.0, 6.0)
    tilt_box(g, STEEL, (160.0, yf - 16.0, 140.0), (222.0, yf - 10.0, 154.0), 8.0, 8.0)
    tilt_box(g, TB, (245.0, yf - 60.0, 0.0), (240.0, yf - 12.0, 130.0), 6.0, 6.0)
    g.box(STEEL, (240.0, yf - 10.0, 150.0), (30.0, 5.0, 40.0))
    for s in (-1, 1):
        tilt_box(g, DT, (-200.0 + s * 46.0, yf - 70.0, -40.0), (-200.0, yf - 70.0, 92.0), 10.0, 10.0)
    rod(g, STEEL, (-200.0, yf - 82.0, 92.0), (-200.0, yf - 58.0, 92.0), 42.0, steps=8)
    g.box(CHAR, (-200.0, yf - 94.0, 104.0), (8.0, 8.0, 30.0))
    # pit props stacked along the side, under snow; the stove pipe
    for row, n in enumerate((3, 2, 1)):
        for k in range(n):
            x = R + 44.0 + (k - (n - 1) / 2.0) * 44.0
            rod(g, TB if (row + k) % 2 else DT, (x, yf + 20.0, 20.0 + row * 38.0), (x, yf + 400.0, 20.0 + row * 38.0), 21.0, steps=6)
    g.box(SNOW, (R + 44.0, yf + 210.0, 122.0), (40.0, 340.0, 10.0))
    g.cyl(STEEL, (130.0, 130.0, 330.0), 16.0, 200.0, steps=6)
    g.cyl(CHAR, (130.0, 130.0, 530.0), 30.0, 26.0, steps=6, top_r=6.0)


def breaker(sc):
    """The breaker: the crystal sorting shed, three stepped halls each with its own steep plank roof, rows of lit
    windows, a covered conveyor climbing to the top hall on two bents, a bin of crystal at the low end. 17.6 m tall."""
    g = sc.still()
    D = 600.0
    yf = -D / 2.0
    g.box(DT, (0, 0, -FOOT / 2.0), (1500.0, D - 40.0, FOOT))
    halls = ((-520.0, 1180.0, 1420.0), (0.0, 820.0, 1040.0), (520.0, 470.0, 680.0))      # middle, height at +X, height at -X
    for k, (xc, zl, zh) in enumerate(halls):
        x0, x1 = xc - 260.0, xc + 260.0
        crib(g, xc, 0, 300.0, D + 20.0, 0.0, 140.0, 2)
        g.side(DT, _ccw([(x0, 140.0), (x1, 140.0), (x1, zl), (x0, zh)]), yf, D / 2.0)
        sl = (zh - zl) / 520.0
        tilt_box(g, TB, (x0 - 40.0, 0, zh + sl * 40.0 + 14.0), (x1 + 40.0, 0, zl - sl * 40.0 + 14.0), D + 90.0, 22.0)
        tilt_box(g, SNOW, (xc, 0, (zh + zl) / 2.0 + 32.0), (x1 + 42.0, 0, zl - sl * 42.0 + 32.0), D + 96.0, 14.0)
        post(g, TB, x0, yf - 4.0, 140.0, zh, 26.0)
        g.box(TB, (xc, yf - 4.0, 150.0), (520.0, 20.0, 22.0))
        for j in range(3):
            win(g, xc - 160.0 + j * 160.0, yf, zl - 150.0, 84.0, 100.0, lit=((j + k) % 3 != 1))
        if k < 2:
            cross_x(g, TB, x0 + 24.0, x1 - 24.0, yf - 6.0, 180.0, zl - 250.0, 16.0)
        if k == 0:
            for j in range(3):
                win(g, xc - 160.0 + j * 160.0, yf, 620.0, 84.0, 100.0, lit=(j != 0))
    post(g, TB, 780.0, yf - 4.0, 140.0, 470.0, 26.0)
    g.box(TB, (520.0, yf - 3.0, 250.0), (130.0, 10.0, 220.0))
    g.box(CHAR, (520.0, yf - 8.0, 246.0), (104.0, 6.0, 202.0))
    lantern(g, 610.0, yf - 30.0, 400.0, glow=0.9, s=0.7)
    for k in range(3):
        hh = 126.0 - k * 44.0
        g.box(TB, (520.0, yf - 40.0 - k * 42.0, hh / 2.0), (170.0, 42.0, hh))
    # the conveyor up to the top hall
    tilt_box(g, DT, (-1120.0, 0, 90.0), (-770.0, 0, 1230.0), 170.0, 140.0)
    tilt_box(g, TB, (-1150.0, 0, 190.0), (-800.0, 0, 1330.0), 200.0, 16.0)
    for x, zt in ((-1010.0, 380.0), (-890.0, 770.0)):
        for s in (-1, 1):
            tilt_box(g, DT, (x - 30.0, s * 150.0, -100.0), (x, s * 70.0, zt), 18.0, 18.0)
    g.box(DT, (-1130.0, 0, 30.0), (200.0, 240.0, 260.0))
    # the bin of crystal at the low end, the stack
    g.box(DT, (900.0, -40.0, 70.0), (210.0, 320.0, 340.0))
    for z in (70.0, 200.0):
        g.box(STEEL, (900.0, -40.0, z), (218.0, 328.0, 12.0))
    tilt_box(g, TB, (780.0, -40.0, 400.0), (880.0, -40.0, 260.0), 120.0, 12.0)
    for dx, dy, h, r, lean, yaw in ((-40, -70, 150, 30, 8, 200), (30, 30, 190, 34, 5, 40), (50, -100, 100, 22, 22, -60), (-50, 80, 90, 20, 26, 130), (20, 110, 80, 18, 30, 80), (-10, -10, 120, 24, 14, 300)):
        crystal(g, 900.0 + dx, -40.0 + dy, 238.0, h, r, lean, yaw, glow=XGLOW, steps=5)
    g.cyl(STEEL, (-600.0, 150.0, 1400.0), 34.0, 300.0, steps=8, top_r=28.0)
    g.cyl(CHAR, (-600.0, 150.0, 1700.0), 48.0, 44.0, steps=8, top_r=10.0)


def tank_tower(sc):
    """The tank tower: a stave-built water tank in steel hoops under a steep conical plank roof, on a braced timber
    tower 10.8 m high; a ladder, a spout, a lamp under the deck, a pick for a weather vane (it turns a little).
    17.9 m tall."""
    g = sc.still()
    ZP, B, T = 1080.0, 260.0, 170.0

    def half(z):
        return B + (T - B) * (z + FOOT) / (ZP + FOOT)

    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, DT, (sx * B, sy * B, -FOOT), (sx * T, sy * T, ZP), 30.0, 30.0)
    lev = (40.0, 400.0, 740.0, ZP - 20.0)
    for z in lev[1:3]:
        h = half(z)
        for s in (-1, 1):
            g.box(TB, (0, s * h, z), (2.0 * h + 30.0, 22.0, 22.0))
            g.box(TB, (s * h, 0, z), (22.0, 2.0 * h + 30.0, 22.0))
    for i in range(3):
        z0, z1 = lev[i] + 10.0, lev[i + 1] - 10.0
        h0, h1 = half(z0), half(z1)
        tilt_box(g, TB, (-h0, -h0, z0), (h1, -h1, z1), 16.0, 16.0)
        tilt_box(g, TB, (h0, -h0, z0), (-h1, -h1, z1), 16.0, 16.0)
        for sx in (-1, 1):
            tilt_box(g, TB, (sx * h0, -h0, z0), (sx * h1, h1, z1), 16.0, 16.0)
    dk = 2.0 * T + 150.0
    g.box(TB, (0, 0, ZP + 10.0), (dk, dk, 20.0))
    for s in (-1, 1):
        g.box(DT, (0, s * (dk / 2.0 - 6.0), ZP + 100.0), (dk, 8.0, 8.0))
        g.box(DT, (s * (dk / 2.0 - 6.0), 0, ZP + 100.0), (8.0, dk, 8.0))
        for s2 in (-1, 1):
            post(g, DT, s * (dk / 2.0 - 6.0), s2 * (dk / 2.0 - 6.0), ZP + 20.0, ZP + 104.0, 10.0)
    g.cyl(TB, (0, 0, ZP + 20.0), 200.0, 330.0, steps=10, top_r=186.0)
    for z in (ZP + 70.0, ZP + 185.0, ZP + 300.0):
        g.cyl(STEEL, (0, 0, z), 205.0 - (z - ZP - 20.0) * 14.0 / 330.0, 14.0, steps=10)
    g.cyl(DT, (0, 0, ZP + 350.0), 232.0, 230.0, steps=10, top_r=14.0)
    g.cyl(SNOW, (0, 0, ZP + 352.0), 240.0, 56.0, steps=10, top_r=186.0)
    g.box(STEEL, (0, 0, ZP + 640.0), (8.0, 8.0, 140.0))
    v = sc.part(P.motion(swing=28.0, pivot=(0, 0, ZP + 690.0), axis=P.UP, rate=0.12, phase=0.3))
    v.box(STEEL, (0, 0, ZP + 690.0), (150.0, 6.0, 8.0))
    tilt_box(v, STEEL, (75.0, 0, ZP + 690.0), (50.0, 0, ZP + 724.0), 6.0, 10.0)
    tilt_box(v, STEEL, (75.0, 0, ZP + 690.0), (50.0, 0, ZP + 656.0), 6.0, 10.0)
    v.box(TEAL, (-62.0, 0, ZP + 690.0), (38.0, 5.0, 38.0))
    # the ladder up the front, the spout, the lamp
    for x in (-110.0, -56.0):
        tilt_box(g, TB, (x, -B - 34.0, -60.0), (x, -dk / 2.0 - 6.0, ZP + 110.0), 8.0, 8.0)
    for k in range(8):
        t = (k + 0.7) / 8.5
        g.box(TB, (-83.0, -B - 34.0 + (B + 34.0 - dk / 2.0 - 6.0) * t, -60.0 + (ZP + 170.0) * t), (60.0, 6.0, 6.0))
    tilt_box(g, STEEL, (120.0, -150.0, ZP + 60.0), (120.0, -dk / 2.0 - 60.0, ZP - 30.0), 16.0, 16.0)
    g.box(STEEL, (120.0, -dk / 2.0 - 60.0, ZP - 100.0), (16.0, 16.0, 150.0))
    lantern(g, -10.0, -T - 30.0, ZP - 2.0, glow=1.0, s=0.8)


# ------------------------------------------------------------------------------------------ the hero
def headframe(sc):
    """THE CRYSTAL HEADFRAME: a braced timber headframe 42 m to its wheel deck, astride a cluster of teal crystals as
    tall as a house (the tallest 12.5 m), two raking legs, a hoist-man's cabin half way up, ladders, lamps, the great
    winding wheel on top (8 m across: it winds one way and back), a cage riding the rope inside the frame (it rises
    and falls 1.2 m), a pennant, and a trestle running off one side with a cart of crystal. 51 m tall, 50 m long."""
    g = sc.still()
    B, T, ZT, FT = 520.0, 330.0, 4200.0, 400.0

    def half(z):
        return B + (T - B) * (z + FT) / (ZT + FT)

    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, DT, (sx * B, sy * B, -FT), (sx * T, sy * T, ZT), 70.0, 70.0)
    lev = (60.0, 760.0, 1460.0, 2140.0, 2800.0, 3440.0, ZT - 40.0)
    for z in lev[1:]:
        h = half(z)
        for s in (-1, 1):
            g.box(TB, (0, s * h, z), (2.0 * h + 60.0, 44.0, 44.0))
            g.box(TB, (s * h, 0, z), (44.0, 2.0 * h + 60.0, 44.0))
    for i in range(6):
        z0, z1 = lev[i] + 30.0, lev[i + 1] - 30.0
        h0, h1 = half(z0), half(z1)
        for s in (-1, 1):
            tilt_box(g, TB, (-h0, s * h0, z0), (h1, s * h1, z1), 34.0, 34.0)
            tilt_box(g, TB, (h0, s * h0, z0), (-h1, s * h1, z1), 34.0, 34.0)
            tilt_box(g, TB, (s * h0, -h0, z0), (s * h1, h1, z1), 34.0, 34.0)
    # the raking legs and their struts
    XR = 2300.0

    def rake(z):
        t = (ZT - 100.0 - z) / (ZT - 100.0 + FT)
        return (T + (XR - T) * t, T + (B + 60.0 - T) * t)

    for sy in (-1, 1):
        tilt_box(g, DT, (T, sy * T, ZT - 100.0), (XR, sy * (B + 60.0), -FT), 64.0, 64.0)
        for z in (1460.0, 2800.0):
            xr, yr = rake(z)
            tilt_box(g, TB, (half(z), sy * half(z), z), (xr, sy * yr, z), 30.0, 30.0)
    for z in (700.0, 1460.0, 2800.0):
        xr, yr = rake(z)
        g.box(TB, (xr, 0, z), (30.0, 2.0 * yr, 30.0))
    xa, ya = rake(700.0)
    xb, yb = rake(1460.0)
    tilt_box(g, TB, (xa, -ya, 700.0), (xb, yb, 1460.0), 26.0, 26.0)
    tilt_box(g, TB, (xa, ya, 700.0), (xb, -yb, 1460.0), 26.0, 26.0)
    # the wheel deck, its rail, the bearings and the wheel
    g.box(TB, (100.0, 0, ZT + 20.0), (2.0 * T + 420.0, 2.0 * T + 200.0, 40.0))
    for sy in (-1, 1):
        g.box(DT, (100.0, sy * (T + 94.0), ZT + 150.0), (2.0 * T + 420.0, 10.0, 10.0))
        for k in range(4):
            post(g, DT, -T - 100.0 + k * (2.0 * T + 400.0) / 3.0, sy * (T + 94.0), ZT + 40.0, ZT + 154.0, 12.0)
    c = (200.0, 0.0, ZT + 460.0)
    for sy in (-1, 1):
        for sx in (-1, 1):
            tilt_box(g, DT, (c[0] + sx * 230.0, sy * 110.0, ZT + 40.0), (c[0], sy * 110.0, c[2]), 34.0, 34.0)
    g.box(STEEL, (c[0], 0, c[2]), (70.0, 280.0, 70.0))
    wheel(sc, c, 400.0, n=12, spokes=4, w=26.0, t=32.0, swing=170.0, rate=0.035)
    tilt_box(g, CHAR, (c[0] + 250.0, 0, c[2] + 300.0), (XR - 60.0, 0, 160.0), 10.0, 10.0)      # the rope down to the winder
    g.box(DT, (XR - 40.0, 0, 20.0), (260.0, 360.0, 440.0))
    rod(g, STEEL, (XR - 40.0, -190.0, 160.0), (XR - 40.0, 190.0, 160.0), 90.0, steps=8)
    # the cage on its rope, in the frame, over the crystals
    k = sc.part(P.motion(bob=-120.0, rate=0.05, phase=0.0))
    k.box(CHAR, (-200.0, 0, (2150.0 + c[2]) / 2.0), (10.0, 10.0, c[2] - 2150.0))
    k.box(STEEL, (-200.0, 0, 2030.0), (170.0, 170.0, 240.0))
    k.box(CHAR, (-200.0, -86.0, 2030.0), (120.0, 6.0, 170.0))
    crystal(k, -200.0, -40.0, 2150.0, 90.0, 20.0, 10.0, 40.0, glow=XGLOW, steps=5)
    # the hoist-man's cabin on the front of the frame, ladders from stage to stage, a lamp at every stage
    hc = half(1650.0)
    g.box(DT, (0, -hc - 110.0, 1650.0), (360.0, 240.0, 300.0))
    g.side(TB, _ccw([(-230.0, 1790.0), (230.0, 1790.0), (0, 2010.0)]), -hc - 260.0, -hc + 30.0)
    g.box(TB, (0, -hc - 110.0, 1494.0), (400.0, 280.0, 16.0))
    for sx in (-1, 1):
        g.box(TB, (sx * 100.0, -hc - 233.0, 1670.0), (88.0, 10.0, 108.0))
        g.box(LAMP, (sx * 100.0, -hc - 238.0, 1670.0), (70.0, 6.0, 90.0), glow=LIT * 1.6)
        tilt_box(g, TB, (sx * 150.0, -hc - 220.0, 1490.0), (sx * 150.0, -hc - 10.0, 1250.0), 18.0, 18.0)
    for i in range(1, 6):
        s = 1 if i % 2 else -1
        ha, hb = half(lev[i]), half(lev[i + 1])
        if i != 2:
            tilt_box(g, DT, (-s * 170.0, -ha - 46.0, lev[i]), (s * 170.0, -hb - 46.0, lev[i + 1]), 50.0, 10.0)
        g.box(CHAR, (s * (hb + 40.0), -hb - 44.0, lev[i + 1] + 86.0), (44.0, 44.0, 8.0))
        g.box(LAMP, (s * (hb + 40.0), -hb - 44.0, lev[i + 1] + 54.0), (34.0, 34.0, 56.0), glow=1.2)
    # the crystals it stands astride, on dark rock
    for x, y, r0, h in ((0, 60, 420, 240), (-500, -200, 300, 200), (520, 160, 320, 210), (160, -520, 260, 170)):
        g.cyl(CHAR, (x, y, -FT), r0, FT + h - 120.0, steps=7, top_r=r0 * 0.62)
    for x, y, h, r, lean, yaw in ((0, 40, 1250, 150, 4, 80), (-230, -60, 980, 120, 14, 190), (240, -100, 860, 110, 16, -20),
                                  (-60, -330, 620, 90, 24, 255), (300, 260, 1050, 125, 12, 40), (-340, 250, 760, 100, 20, 150),
                                  (120, -500, 420, 70, 34, 290), (-470, -380, 380, 64, 36, 215), (530, -330, 460, 70, 32, -40),
                                  (-760, -160, 520, 80, 30, 180), (790, 90, 600, 85, 26, 10)):
        crystal(g, x, y, -150.0, h + 150.0, r, lean, yaw, glow=XGLOW)
    # the trestle off the side, a cart at its end over a tip of crystal
    ZD = 900.0
    xs = (-1100.0, -1650.0, -2200.0, -2700.0)
    for x in xs:
        for s in (-1, 1):
            tilt_box(g, DT, (x, s * 300.0, -FT), (x, s * 130.0, ZD - 40.0), 34.0, 34.0)
        g.box(TB, (x, 0, ZD - 30.0), (38.0, 340.0, 32.0))
    for i in range(3):
        cross_x(g, TB, xs[i] - 14.0, xs[i + 1] + 14.0, -230.0, 80.0, ZD - 90.0, 22.0)
    cross_x(g, TB, -half(500.0), xs[0] + 14.0, -230.0, 80.0, ZD - 90.0, 22.0)
    for s in (-1, 1):
        g.box(DT, (-1620.0, s * 90.0, ZD), (2240.0, 34.0, 44.0))
        g.box(STEEL, (-1620.0, s * 36.0, ZD + 38.0), (2240.0, 10.0, 12.0))
    for k in range(8):
        g.box(TB, (-640.0 - k * 290.0, 0, ZD + 27.0), (50.0, 330.0, 12.0))
    cart(g, -2560.0, 0, ZD + 44.0, glow=XGLOW, n=3, seed=5)
    for dx, dy, h, r, lean, yaw in ((0, 0, 330, 60, 8, 100), (-130, -90, 240, 44, 26, 200), (120, 60, 220, 40, 28, 20), (30, -170, 170, 32, 34, 270), (-90, 130, 180, 34, 30, 140)):
        crystal(g, -2960.0 + dx, dy, -150.0, h + 150.0, r, lean, yaw, glow=XGLOW, steps=5)
    # the pennant on the deck's mast
    px = -T - 90.0
    g.box(STEEL, (px, 0, ZT + 360.0), (12.0, 12.0, 640.0))
    f = sc.part(P.motion(swing=12.0, pivot=(px, 0, ZT + 620.0), axis=P.UP, rate=0.3, phase=0.2))
    f.box(TEAL, (px - 110.0, 0, ZT + 630.0), (210.0, 6.0, 80.0))
    f.box(LAMP, (px - 250.0, 0, ZT + 630.0), (80.0, 6.0, 46.0))


# ------------------------------------------------------------------------------------------ the props: low
def ore_cart(sc):
    """An ore cart full of crystal on a short length of rail, a stop block before it. LOW; nothing lit."""
    g = sc.still()
    track(g, 320.0, 4)
    cart(g, -20.0, 0, 25.0, glow=0.0, n=5, seed=7)
    g.box(TB, (146.0, 0, 44.0), (20.0, 110.0, 38.0))


def pit_props(sc):
    """A stack of pit props between stakes on two bearers, snow on the top log, a sawbuck with a prop on it and an
    axe in a block. LOW."""
    g = sc.still()
    for x in (-150.0, 10.0):
        g.box(DT, (x, 0, -19.0), (22.0, 130.0, 52.0))
        for s in (-1, 1):
            g.box(DT, (x, s * 62.0, 50.0), (12.0, 12.0, 190.0))
    for row, n in enumerate((3, 2, 2, 1)):
        for k in range(n):
            y = (k - (n - 1) / 2.0) * 38.0
            rod(g, TB if (row + k) % 2 else DT, (-190.0, y, 26.0 + row * 33.0), (50.0, y, 26.0 + row * 33.0), 19.0, steps=6)
    g.box(SNOW, (-70.0, 0, 147.0), (200.0, 30.0, 8.0))
    for s in (-1, 1):
        tilt_box(g, DT, (118.0 + s * 4.0, -40.0, -40.0), (118.0 + s * 4.0, 34.0, 100.0), 10.0, 10.0)
        tilt_box(g, DT, (118.0 + s * 4.0 + 40.0, 40.0, -40.0), (118.0 + s * 4.0 + 40.0, -34.0, 100.0), 10.0, 10.0)
    rod(g, TB, (84.0, 0, 86.0), (196.0, 0, 86.0), 17.0, steps=6)
    g.cyl(DT, (-240.0, 0, -45.0), 34.0, 95.0, steps=7)
    tilt_box(g, TB, (-240.0, 0, 50.0), (-200.0, 0, 128.0), 6.0, 6.0)
    g.box(STEEL, (-240.0, 0, 62.0), (30.0, 6.0, 26.0))


def winch(sc):
    """A hand winch (a windlass) on two A-frames over a planked shaft collar, a bucket with a crystal on its rope,
    crank handles at both ends. It does not turn. LOW."""
    g = sc.still()
    g.box(DT, (0, 0, -8.0), (170.0, 130.0, 74.0))
    g.box(TB, (0, 0, 33.0), (190.0, 136.0, 10.0))
    g.box(CHAR, (0, 0, 39.0), (76.0, 76.0, 4.0))
    for s in (-1, 1):
        x = s * 112.0
        tilt_box(g, DT, (x, -58.0, -40.0), (x, 0.0, 152.0), 14.0, 14.0)
        tilt_box(g, DT, (x, 58.0, -40.0), (x, 0.0, 152.0), 14.0, 14.0)
        g.box(STEEL, (s * 128.0, 0, 168.0), (8.0, 8.0, 44.0))
        g.box(STEEL, (s * 142.0, 0, 187.0), (30.0, 8.0, 8.0))
    rod(g, TB, (-120.0, 0, 150.0), (120.0, 0, 150.0), 16.0, steps=8)
    rod(g, CHAR, (-44.0, 0, 150.0), (44.0, 0, 150.0), 20.0, steps=8)
    g.box(CHAR, (0, 0, 105.0), (5.0, 5.0, 60.0))
    g.cyl(STEEL, (0, 0, 41.0), 21.0, 34.0, steps=8, top_r=26.0)
    crystal(g, 0, 0, 66.0, 40.0, 10.0, 10.0, 40.0, steps=5)
    g.box(SNOW, (-50.0, 40.0, 40.0), (70.0, 40.0, 6.0))


def sort_table(sc):
    """The sorting table: a long bench with three trays of crystal, a balance, a box on the shelf, a back board with
    two picks hung on it and an unlit lantern. LOW; nothing lit."""
    g = sc.still()
    for x in (-134.0, 134.0):
        for s in (-1, 1):
            g.box(DT, (x, s * 36.0, 22.0), (12.0, 12.0, 134.0))
    g.box(TB, (0, 0, 92.0), (300.0, 96.0, 10.0))
    g.box(DT, (0, 0, 40.0), (270.0, 80.0, 8.0))
    g.box(DT, (60.0, 0, 64.0), (64.0, 54.0, 40.0))
    r = random.Random(9)
    for k, x in enumerate((-104.0, -24.0, 50.0)):
        g.box(DT, (x, -8.0, 101.0), (66.0, 58.0, 8.0))
        for j in range(2):
            crystal(g, x - 14.0 + j * 28.0, -8.0 + r.uniform(-10, 10), 105.0, r.uniform(20, 36), r.uniform(7, 10), r.uniform(0, 24), r.uniform(0, 360), steps=5)
    g.box(STEEL, (118.0, -6.0, 132.0), (8.0, 8.0, 70.0))
    g.box(STEEL, (118.0, -6.0, 166.0), (84.0, 6.0, 6.0))
    for s in (-1, 1):
        g.box(CHAR, (118.0 + s * 38.0, -6.0, 146.0), (4.0, 4.0, 36.0))
        g.cyl(LAMP, (118.0 + s * 38.0, -6.0, 124.0), 15.0, 5.0, steps=6, top_r=18.0)
    for x in (-146.0, 146.0):
        g.box(DT, (x, 44.0, 140.0), (10.0, 10.0, 100.0))
    g.box(DT, (0, 44.0, 150.0), (300.0, 8.0, 84.0))
    for x in (-70.0, 30.0):
        tilt_box(g, TB, (x, 38.0, 112.0), (x + 30.0, 38.0, 184.0), 5.0, 5.0)
        tilt_box(g, STEEL, (x + 12.0, 37.0, 186.0), (x + 50.0, 37.0, 172.0), 5.0, 8.0)
    g.box(CHAR, (-120.0, 30.0, 100.0), (22.0, 22.0, 5.0))
    g.box(LAMP, (-120.0, 30.0, 114.0), (16.0, 16.0, 24.0))
    g.box(CHAR, (-120.0, 30.0, 129.0), (22.0, 22.0, 5.0))


def buffer_stop(sc):
    """The end of the line: a rail buffer of raked timbers with steel plates, spare sleepers stacked crosswise, a
    switch lever with a plain round target. LOW."""
    g = sc.still()
    track(g, 320.0, 4)
    for s in (-1, 1):
        tilt_box(g, DT, (60.0, s * 36.0, 15.0), (140.0, s * 36.0, 122.0), 16.0, 16.0)
        g.box(DT, (140.0, s * 36.0, 62.0), (16.0, 16.0, 130.0))
        g.box(STEEL, (117.0, s * 36.0, 112.0), (6.0, 30.0, 30.0))
    g.box(TB, (130.0, 0, 112.0), (22.0, 130.0, 30.0))
    for k in range(4):
        if k % 2 == 0:
            for dx in (-36.0, 36.0):
                g.box(DT if dx < 0 else TB, (-100.0 + dx, 0, 38.0 + k * 20.0), (24.0, 120.0, 20.0))
        else:
            for dy in (-36.0, 36.0):
                g.box(TB if dy < 0 else DT, (-100.0, dy, 38.0 + k * 20.0), (110.0, 24.0, 20.0))
    g.box(SNOW, (-100.0, 0, 111.0), (90.0, 90.0, 6.0))
    g.box(CHAR, (20.0, 56.0, 30.0), (30.0, 12.0, 30.0))
    tilt_box(g, STEEL, (20.0, 56.0, 30.0), (0.0, 56.0, 150.0), 6.0, 6.0)
    rod(g, TB, (-2.0, 51.0, 166.0), (-2.0, 61.0, 166.0), 24.0, steps=8)
    rod(g, CHAR, (-2.0, 49.0, 166.0), (-2.0, 63.0, 166.0), 9.0, steps=6)


# ------------------------------------------------------------------------------------------ the props: tall
def crystal_rock(sc):
    """A cluster of crystal on a dark rock (4.4 m, steady glow), staked off with three posts and a rope."""
    g = sc.still()
    g.cyl(CHAR, (0, 14.0, -150.0), 112.0, 230.0, steps=7, top_r=80.0)
    g.cyl(CHAR, (-74.0, 0.0, -150.0), 70.0, 190.0, steps=6, top_r=44.0)
    g.cyl(CHAR, (84.0, 20.0, -150.0), 64.0, 200.0, steps=6, top_r=38.0)
    for x, y, h, r, lean, yaw in ((0, 14, 380, 42, 3, 0), (-46, 6, 280, 34, 15, 172), (50, 10, 250, 30, 17, 8), (-8, -34, 170, 24, 24, 265),
                                  (16, 50, 300, 34, 9, 80), (-92, -6, 130, 20, 32, 196), (96, 14, 120, 18, 34, -8)):
        crystal(g, x, y, 60.0, h, r, lean, yaw, glow=XGLOW)
    for x in (-150.0, 0.0, 150.0):
        g.box(TB, (x, -92.0, 20.0), (10.0, 10.0, 130.0))
        g.box(LAMP, (x, -92.0, 90.0), (12.0, 12.0, 12.0))
    g.box(CHAR, (0, -92.0, 66.0), (300.0, 5.0, 5.0))


def lamp_post(sc):
    """A timber lamp post on a crib foot (5.4 m): a braced arm along the road, a lantern under a plank cap (steady),
    a pick on a peg, a tally tag that swings a little."""
    g = sc.still()
    g.box(DT, (0, 0, 180.0), (24.0, 24.0, 660.0))
    crib(g, 0, 0, 96.0, 96.0, 0.0, 54.0, 2, log=18.0)
    g.box(DT, (48.0, 0, 482.0), (130.0, 16.0, 16.0))
    tilt_box(g, TB, (0, 0, 396.0), (74.0, 0, 474.0), 12.0, 12.0)
    tilt_box(g, TB, (30.0, 0, 498.0), (90.0, 0, 520.0), 84.0, 8.0)
    tilt_box(g, TB, (150.0, 0, 498.0), (90.0, 0, 520.0), 84.0, 8.0)
    g.box(SNOW, (90.0, 0, 527.0), (60.0, 70.0, 8.0))
    lantern(g, 90.0, 0, 474.0, glow=1.0, s=0.9)
    g.box(DT, (0, -16.0, 250.0), (10.0, 12.0, 10.0))
    tilt_box(g, TB, (-8.0, -20.0, 150.0), (6.0, -20.0, 246.0), 5.0, 5.0)
    tilt_box(g, STEEL, (-22.0, -20.0, 240.0), (34.0, -20.0, 254.0), 5.0, 8.0)
    s = sc.part(P.motion(swing=6.0, pivot=(-16.0, 0, 320.0), axis=90.0, rate=0.3, phase=0.5))
    s.box(TB, (-16.0, 0, 296.0), (6.0, 30.0, 44.0))


def bell_frame(sc):
    """The shift bell: a steel bell under a little plank roof on a timber A-frame (4.8 m; it swings slowly), a pull
    rope, and a board of three plain shapes (a square, a disc, a triangle: no words)."""
    g = sc.still()
    for s in (-1, 1):
        tilt_box(g, DT, (s * 132.0, 0, -120.0), (s * 30.0, 0, 404.0), 22.0, 22.0)
        tilt_box(g, TB, (s * 100.0, 0, 404.0), (0.0, 0, 474.0), 84.0, 10.0)
    g.box(TB, (0, 0, 180.0), (186.0, 18.0, 18.0))
    g.box(DT, (0, 0, 402.0), (116.0, 26.0, 26.0))
    g.box(SNOW, (0, 0, 478.0), (60.0, 80.0, 8.0))
    b = sc.part(P.motion(swing=13.0, pivot=(0, 0, 388.0), axis=90.0, rate=0.22, phase=0.4))
    b.box(CHAR, (0, 0, 378.0), (10.0, 10.0, 22.0))
    b.cyl(STEEL, (0, 0, 288.0), 48.0, 82.0, steps=8, top_r=22.0)
    b.box(CHAR, (0, 0, 270.0), (8.0, 8.0, 56.0))
    g.box(TB, (64.0, -8.0, 300.0), (5.0, 5.0, 200.0))
    g.box(DT, (0, -12.0, 120.0), (160.0, 6.0, 74.0))
    g.box(LAMP, (-48.0, -17.0, 120.0), (32.0, 4.0, 32.0))
    rod(g, TEAL, (0, -15.0, 120.0), (0, -19.0, 120.0), 17.0, steps=8)
    g.side(CHAR, _ccw([(30.0, 104.0), (66.0, 104.0), (48.0, 138.0)]), -19.0, -15.0)


def sluice(sc):
    """A sluice trough on four trestles (7.4 m along the road, 3.4 m): riffle bars and lit crystal grit in the
    trough, a feed box at its head heaped with crystal, a hooped tub at its foot."""
    g = sc.still()

    def zat(x):
        return 250.0 - x * 0.11

    for x in (-300.0, -100.0, 100.0, 300.0):
        zt = zat(x) - 14.0
        for s in (-1, 1):
            tilt_box(g, DT, (x, s * 86.0, -120.0), (x, s * 40.0, zt), 16.0, 16.0)
        g.box(DT, (x, 0, zt), (20.0, 130.0, 16.0))
    tilt_box(g, TB, (-300.0, -66.0, 30.0), (-100.0, -52.0, zat(-100.0) - 40.0), 12.0, 12.0)
    tilt_box(g, TB, (300.0, -66.0, 30.0), (100.0, -52.0, zat(100.0) - 40.0), 12.0, 12.0)
    tilt_box(g, TB, (-350.0, 0, zat(-350.0)), (350.0, 0, zat(350.0)), 84.0, 10.0)
    for s in (-1, 1):
        tilt_box(g, DT, (-350.0, s * 44.0, zat(-350.0) + 22.0), (350.0, s * 44.0, zat(350.0) + 22.0), 10.0, 50.0)
    for k in range(6):
        x = -260.0 + k * 104.0
        g.box(CHAR, (x, 0, zat(x) + 12.0), (10.0, 78.0, 14.0))
        g.box(TEAL, (x + 40.0, (-14.0 if k % 2 else 14.0), zat(x + 40.0) + 12.0), (30.0, 28.0, 12.0), rot=(-6.0, 20.0 * k, 0), glow=XGLOW)
    g.box(DT, (-372.0, 0, 334.0), (100.0, 116.0, 90.0))
    g.box(STEEL, (-372.0, 0, 334.0), (106.0, 122.0, 10.0))
    for dx, dy, h, r, lean, yaw in ((-10, -16, 70, 15, 8, 200), (18, 14, 54, 12, 22, 30), (-20, 26, 44, 10, 28, 120)):
        crystal(g, -372.0 + dx, dy, 376.0, h, r, lean, yaw, glow=XGLOW, steps=5)
    g.cyl(TB, (396.0, 0, -100.0), 58.0, 190.0, steps=8, top_r=64.0)
    for z in (10.0, 60.0):
        g.cyl(STEEL, (396.0, 0, z), 64.0, 10.0, steps=8)
    crystal(g, 386.0, -8.0, 84.0, 52.0, 12.0, 14.0, 60.0, glow=XGLOW, steps=5)
    crystal(g, 410.0, 12.0, 84.0, 40.0, 10.0, 24.0, 300.0, glow=XGLOW, steps=5)


JOBS = [
    ('SM_S1D7_Bunkhouse', bunkhouse, dict(grow=(20, 20, 10), grow_down=(20, 20, 0))),
    ('SM_S1D7_ChuteHouse', chute_house, dict(grow=(30, 40, 10), grow_down=(30, 40, 0))),
    ('SM_S1D7_WindingHouse', winding_house, dict(grow=(20, 20, 10), grow_down=(20, 20, 0))),
    ('SM_S1D7_AssayOffice', assay_office, dict(grow=(60, 40, 20), grow_down=(60, 40, 0))),
    ('SM_S1D7_Trestle', trestle, STILL),
    ('SM_S1D7_ToolStore', tool_store, STILL),
    ('SM_S1D7_Breaker', breaker, STILL),
    ('SM_S1D7_TankTower', tank_tower, dict(grow=(40, 40, 20), grow_down=(40, 40, 0))),
    ('SM_S1D7_Headframe', headframe, dict(far=True, grow=(120, 120, 130), grow_down=(120, 120, 0))),
    ('SM_S1D7_OreCart', ore_cart, STILL),
    ('SM_S1D7_PitProps', pit_props, STILL),
    ('SM_S1D7_Winch', winch, STILL),
    ('SM_S1D7_SortTable', sort_table, STILL),
    ('SM_S1D7_BufferStop', buffer_stop, STILL),
    ('SM_S1D7_CrystalRock', crystal_rock, STILL),
    ('SM_S1D7_LampPost', lamp_post, dict(grow=(20, 20, 0), grow_down=(20, 20, 0))),
    ('SM_S1D7_BellFrame', bell_frame, dict(grow=(40, 20, 0), grow_down=(40, 20, 0))),
    ('SM_S1D7_Sluice', sluice, STILL),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
