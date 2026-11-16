"""Theme Park, Stage 1, builder s1d5: THE LAST RIDE: SWEET STREET (9.2974 to 11.94 km).
/Game/World/ThemePark/SM_S1D5_*. Run: import ft_world_park_s1d5 as K; K.run()

A street of sweet shops: round-cornered two-storey wafer houses on chocolate plinths under piped icing roofs that drip
over the eaves, candy-cane columns, chimneys like ice-cream cones upside down, shop windows lit warm. Six buildings,
nine props and the hero, THE GREAT SWIRL (a soft-ice tower on a wafer cone, a slide winding down its groove).
All of it is our own; no words anywhere, no balloon, no pie, no cake, nothing round on a stick, nothing thrown.

Every piece is an ft_people.Scene (one section, M_People). Pieces that stand 24 m out or more carry a steady glow of
0.08 on every colour (the level is at night; an unlit piece reads as a black shape), lit windows 0.9, lamps 1.3.
The five LOW props (the verge, 23 m) are unlit and still. Nothing blinks.
What moves: the wafer house's vane and shop sign, the toffee works' two pulling arms, the sugar mill's wheel of
sugar-cube buckets (turns on the roof) and its hoisted sack, the chocolate fountain's streams, the cafe's pennant, the
jam jar's flag, the washing on the line, the Great Swirl's five sliders (they go round the tower) and its flag.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/ThemePark'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

PEACH, WHITE, MAG, KRAKEN, WOOD_D, GOLD = '#ffc59e', '#fff6ea', '#ff4fb8', '#c2408f', '#6a4c36', '#ffc93c'
WOOD, CREAM, RED, BULB, PLUM = '#9a6a44', '#fff1d6', '#ff2d3c', '#ffe28a', '#4a3a78'
BASE, WIN, LAMP = 0.08, 0.9, 1.3


class Lit(object):
    """A model whose every primitive carries a steady glow unless it names its own."""

    def __init__(self, m, glow=BASE):
        self.m, self.g = m, glow

    def _g(self, glow):
        return self.g if glow is None else glow

    def box(self, col, c, size, rot=(0, 0, 0), glow=None):
        self.m.box(col, c, size, rot=rot, glow=self._g(glow))

    def cyl(self, col, base, r, h, rot=(0, 0, 0), steps=8, glow=None, top_r=None):
        self.m.cyl(col, base, r, h, rot=rot, steps=steps, glow=self._g(glow), top_r=top_r)

    def sphere(self, col, c, r, scale=(1, 1, 1), steps=(4, 8), glow=None, rot=(0, 0, 0)):
        self.m.sphere(col, c, r, scale=scale, steps=steps, glow=self._g(glow), rot=rot)

    def plan(self, col, pts, z0, z1, glow=None):
        self.m.plan(col, ccw(pts), z0, z1, glow=self._g(glow))

    def side(self, col, pts, y0, y1, glow=None):
        self.m.side(col, ccw(pts), y0, y1, glow=self._g(glow))


def tb(g, col, a, b, w, d=None, glow=None):
    tilt_box(g, col, a, b, w, d, glow=g._g(glow))


def rd(g, col, a, b, r, steps=6, glow=None):
    rod(g, col, a, b, r, steps=steps, glow=g._g(glow))


def ccw(pts):
    n = len(pts)
    a = sum(pts[i][0] * pts[(i + 1) % n][1] - pts[(i + 1) % n][0] * pts[i][1] for i in range(n))
    return list(pts) if a > 0 else list(reversed(pts))


def rr(w, d, c, at=(0.0, 0.0), n=2):
    """A rectangle w x d with round corners (radius c), counter-clockwise."""
    hw, hd = w / 2.0, d / 2.0
    pts = []
    for cx, cy, a0 in ((hw - c, -hd + c, -90.0), (hw - c, hd - c, 0.0), (-hw + c, hd - c, 90.0), (-hw + c, -hd + c, 180.0)):
        for k in range(n + 1):
            a = math.radians(a0 + 90.0 * k / n)
            pts.append((at[0] + cx + c * math.cos(a), at[1] + cy + c * math.sin(a)))
    return pts


def slab(g, col, w, d, c, z0, z1, at=(0.0, 0.0), glow=None):
    g.plan(col, rr(w, d, c, at), z0, z1, glow=glow)


def drips(g, x0, x1, y, z, n, seed, col=WHITE):
    """Icing running over an eave: n drips of different lengths hanging from z along x0..x1 on the face y."""
    r = random.Random(seed)
    for k in range(n):
        ln = r.choice((46.0, 70.0, 96.0, 128.0))
        x = x0 + (x1 - x0) * (k + 0.5) / n + r.uniform(-12, 12)
        g.box(col, (x, y, z - ln / 2.0), (r.choice((26.0, 34.0)), 18, ln))


def cane(g, x, y, z0, h, r=20.0, col=MAG, rings=3, steps=6):
    """A candy-cane column: white, with stripes."""
    g.cyl(WHITE, (x, y, z0), r, h, steps=steps)
    for k in range(rings):
        g.cyl(col, (x, y, z0 + h * (k + 0.5) / rings - h * 0.07), r + 3.0, h * 0.14, steps=steps)


def crook(g, x, y, z, r, reach, col=MAG, sx=1.0):
    """The hook of a candy cane, from the top of its shaft at (x, y, z), bending over along X."""
    pts = [(x, z), (x + sx * reach * 0.18, z + reach * 0.42), (x + sx * reach * 0.62, z + reach * 0.56), (x + sx * reach, z + reach * 0.3), (x + sx * reach * 1.06, z - reach * 0.08)]
    for k in range(4):
        rd(g, WHITE if k % 2 == 0 else col, (pts[k][0], y, pts[k][1]), (pts[k + 1][0], y, pts[k + 1][1]), r)


def win(g, x, y, z, w, h, frame=WOOD_D, sill=True, shut=None):
    """A lit window on a front wall whose face is at y."""
    g.box(frame, (x, y - 2, z), (w + 20, 10, h + 20))
    g.box(GOLD, (x, y - 6, z), (w, 10, h), glow=WIN)
    if sill:
        g.box(WHITE, (x, y - 12, z - h / 2.0 - 14), (w + 40, 24, 14))
    if shut:
        for s in (-1, 1):
            g.box(shut, (x + s * (w / 2.0 + 30), y - 4, z), (36, 8, h + 8))


def winx(g, x, y, z, w, h, frame=WOOD_D):
    """A lit window on an end wall (a face at x)."""
    g.box(frame, (x, y, z), (10, w + 20, h + 20))
    g.box(GOLD, (x + (4 if x > 0 else -4), y, z), (10, w, h), glow=WIN)


def awning(g, x0, x1, y, z, depth=110.0, drop=46.0, n=6, cols=(MAG, WHITE)):
    """A striped awning from the wall face y at height z, sloping out and down."""
    w = (x1 - x0) / n
    for k in range(n):
        x = x0 + w * (k + 0.5)
        tb(g, cols[k % 2], (x, y, z), (x, y - depth, z - drop), w, 8)
        g.box(cols[k % 2], (x, y - depth, z - drop - 12), (w, 8, 26))


def cone_chim(g, x, y, z, h=280.0, r=75.0):
    """A chimney like an ice-cream cone upside down, standing in a blob of icing."""
    g.sphere(WHITE, (x, y, z + 8), r * 1.2, scale=(1, 1, 0.42), steps=(3, 6))
    g.cyl(PEACH, (x, y, z + 14), r, h, steps=6, top_r=r * 0.16)
    g.cyl(WOOD, (x, y, z + 14 + h * 0.3), r * 0.79, h * 0.07, steps=6)
    g.cyl(WOOD, (x, y, z + 14 + h * 0.58), r * 0.55, h * 0.06, steps=6)


def grid(g, xs, zs, y, x0, x1, z0, z1, col=WOOD):
    """The wafer's grid on a front wall: ribs at xs (upright) and zs (level)."""
    for x in xs:
        g.box(col, (x, y - 2, (z0 + z1) / 2.0), (8, 8, z1 - z0))
    for z in zs:
        g.box(col, ((x0 + x1) / 2.0, y - 2, z), (x1 - x0, 8, 8))


def tub(g, x, y, z=0.0, s=1.0):
    """A chocolate tub with a swirl of icing in it."""
    g.box(WOOD_D, (x, y, z + 26 * s), (64 * s, 56 * s, 52 * s))
    g.cyl(WHITE, (x, y, z + 52 * s), 26 * s, 34 * s, steps=6, top_r=14 * s)
    g.cyl(MAG, (x, y, z + 86 * s), 14 * s, 30 * s, steps=6, top_r=4 * s)


def pennant(sc, at, col=KRAKEN, w=110.0, h=64.0, mast=120.0, phase=0.0, glow=BASE, g=None):
    """A mast with a pennant that swings about it."""
    x, y, z = at
    if g is not None:
        g.cyl(WOOD_D, (x, y, z), 5.0, mast, steps=5)
    f = Lit(sc.part(P.motion(swing=24, pivot=(x, y, z + mast), axis=P.UP, rate=0.3, phase=phase)), glow)
    f.side(col, [(x + 4, z + mast), (x + 4 + w, z + mast - h / 2.0), (x + 4, z + mast - h)], y - 4, y + 4)


# ================================================================================================ the buildings
def wafer_house(sc):
    """BUILDING 1: the wafer house, 9 x 7 m, two storeys under a three-tier icing roof (top 10.7 m; the chimney 11.1)."""
    g = Lit(sc.still())
    W, D = 880.0, 680.0
    F = -D / 2.0
    slab(g, WOOD_D, W + 56, D + 56, 110, -260, 46)
    slab(g, PEACH, W, D, 112, 46, 640)
    slab(g, WHITE, W + 16, D + 16, 118, 318, 346)
    grid(g, (-330, -110, 110, 330), (410, 585), F, -390, 390, 350, 636)
    # the shop: a wide lit window under an awning, tubs either side
    win(g, -190, F, 196, 330, 180)
    g.box(WOOD_D, (-190, F - 8, 196), (10, 8, 180))
    awning(g, -390, 10, F - 6, 322, n=6)
    tub(g, -400, F - 60, 46)
    tub(g, 20, F - 60, 46, 0.85)
    # the door in its candy-cane porch
    g.box(WOOD_D, (250, F - 2, 176), (150, 10, 262))
    g.box(KRAKEN, (250, F - 6, 166), (118, 10, 236))
    g.box(GOLD, (250, F - 10, 236), (60, 8, 60), glow=WIN)
    cane(g, 150, F - 60, 46, 268)
    cane(g, 350, F - 60, 46, 268)
    slab(g, WHITE, 290, 150, 40, 314, 344, at=(250, F - 50))
    drips(g, 130, 370, F - 122, 316, 4, 3)
    g.box(WOOD_D, (250, F - 60, 30), (200, 110, 32))
    # upstairs: two shuttered windows, the shop's sign between them
    win(g, -220, F, 492, 150, 170, shut=MAG)
    win(g, 220, F, 492, 150, 170, shut=MAG)
    g.box(WOOD_D, (0, F - 46, 610), (10, 92, 10))
    s = Lit(sc.part(P.motion(swing=7, pivot=(0, F - 80, 606), axis=0.0, rate=0.4, phase=0.6)))
    s.box(WOOD_D, (0, F - 80, 590), (8, 8, 34))
    s.box(MAG, (0, F - 80, 536), (96, 12, 70))
    s.box(WHITE, (0, F - 87, 536), (60, 6, 12))
    for sx in (-1, 1):
        s.side(KRAKEN, [(sx * 48, 536), (sx * 92, 566), (sx * 92, 506)], F - 86, F - 74)
    winx(g, W / 2.0 + 2, 0, 492, 150, 170)
    winx(g, -W / 2.0 - 2, 0, 492, 150, 170)
    winx(g, W / 2.0 + 2, 60, 200, 170, 170)
    # the roof: piped icing in three tiers, dripping over the eaves
    slab(g, WHITE, W + 116, D + 116, 150, 640, 714)
    drips(g, -W / 2.0 - 20, W / 2.0 + 20, F - 50, 644, 9, 5)
    drips(g, -W / 2.0 - 20, W / 2.0 + 20, -F + 50, 644, 5, 6)
    slab(g, MAG, W - 126, D - 126, 130, 744, 764)
    slab(g, WHITE, W - 140, D - 140, 130, 714, 806)
    slab(g, WHITE, W - 420, D - 380, 110, 806, 884)
    g.cyl(WHITE, (0, 0, 884), 122, 190, steps=8, top_r=14)
    cone_chim(g, 290, 90, 714, 300, 78)
    g.box(WOOD_D, (-270, 70, 836), (70, 70, 60))
    g.box(WHITE, (-270, 70, 872), (92, 92, 14))
    # the weather vane: a wafer biscuit on a mast
    g.cyl(WOOD_D, (0, 0, 1060), 5, 110, steps=5)
    v = Lit(sc.part(P.motion(swing=26, pivot=(0, 0, 1140), axis=P.UP, rate=0.22, phase=0.2)))
    v.box(PEACH, (52, 0, 1138), (92, 8, 56))
    v.box(WOOD, (52, 0, 1138), (96, 10, 8))
    v.box(WOOD, (52, 0, 1138), (8, 10, 60))
    v.box(MAG, (-34, 0, 1138), (40, 8, 14))


def toffee_works(sc):
    """BUILDING 2: the toffee works, 11 x 7.6 m: a striped hall, a lit bay where two arms pull toffee, twin cone
    chimneys on chocolate stacks (top 14.6 m), a syrup vat on the end wall."""
    g = Lit(sc.still())
    W, D = 1100.0, 760.0
    F = -D / 2.0
    slab(g, WOOD_D, W + 56, D + 56, 110, -260, 46)
    slab(g, PEACH, W, D, 120, 46, 720)
    slab(g, KRAKEN, W + 12, D + 12, 124, 380, 412)
    slab(g, KRAKEN, W + 12, D + 12, 124, 430, 446)
    grid(g, (-480, 480), (), F, -480, 480, 60, 716)
    # the works' double door under a loading awning
    g.box(WOOD_D, (-270, F - 2, 196), (330, 10, 300))
    for s in (-1, 1):
        g.box(KRAKEN, (-270 + s * 76, F - 6, 190), (140, 10, 280))
        g.box(GOLD, (-270 + s * 76, F - 10, 268), (70, 8, 70), glow=WIN)
    awning(g, -470, -70, F - 6, 372, depth=130, n=6, cols=(KRAKEN, WHITE))
    tub(g, -480, F - 60, 46)
    # the pulling bay: a lit room, a hub, two arms that swing a rope of toffee
    g.box(WOOD_D, (260, F - 2, 216), (400, 10, 300))
    g.box(GOLD, (260, F - 6, 216), (372, 10, 272), glow=0.7)
    g.box(WHITE, (260, F - 12, 60), (430, 26, 16))
    g.box(WOOD_D, (260, F - 14, 216), (60, 16, 60))
    g.box(WOOD, (260, F - 22, 150), (190, 12, 26))
    for k, px in enumerate((170.0, 350.0)):
        a = Lit(sc.part(P.motion(swing=38, pivot=(px, F - 22, 216), axis=90.0, rate=0.45, phase=0.5 * k)))
        a.box(MAG, (px, F - 22, 256), (22, 14, 110))
        a.box(MAG, (px, F - 22, 176), (22, 14, 70))
        a.box(WOOD, (px, F - 24, 316), (60, 18, 36))
        a.box(WHITE, (px, F - 26, 216), (34, 12, 34))
    # the sign board over the bay: three toffee blocks, no words
    g.box(KRAKEN, (260, F - 8, 350), (300, 10, 44))
    for k in range(3):
        g.box(WOOD, (180 + k * 80, F - 14, 350), (50, 8, 28))
    # upstairs windows
    for x in (-390, -150, 150, 390):
        win(g, x, F, 590, 124, 140, shut=KRAKEN if abs(x) > 200 else None)
    winx(g, -W / 2.0 - 2, 0, 590, 150, 150)
    winx(g, -W / 2.0 - 2, 0, 230, 190, 190)
    # the roof and the twin chimneys
    slab(g, WHITE, W + 116, D + 116, 160, 720, 796)
    drips(g, -W / 2.0 - 20, W / 2.0 + 20, F - 50, 724, 11, 8)
    drips(g, -W / 2.0 - 20, W / 2.0 + 20, -F + 50, 724, 6, 9)
    slab(g, WHITE, W - 220, D - 240, 130, 796, 868)
    for s in (-1, 1):
        x = s * 340.0
        g.box(WOOD_D, (x, 70, 880), (180, 180, 240))
        slab(g, WHITE, 230, 230, 50, 990, 1022, at=(x, 70))
        g.cyl(PEACH, (x, 70, 1022), 112, 440, steps=8, top_r=24)
        g.cyl(WOOD, (x, 70, 1140), 90, 26, steps=8)
        g.cyl(WOOD, (x, 70, 1260), 66, 22, steps=8)
    g.box(WOOD, (-340, -24, 930), (34, 14, 150))
    g.box(WOOD, (-296, -24, 960), (26, 14, 90))
    g.box(WOOD_D, (0, 60, 900), (90, 90, 70))
    g.box(WHITE, (0, 60, 942), (116, 116, 14))
    # the syrup vat on the end wall
    vx = W / 2.0 + 170
    g.cyl(WOOD_D, (vx, 60, -200), 150, 640, steps=10)
    g.cyl(GOLD, (vx, 60, 120), 154, 18, steps=10)
    g.cyl(GOLD, (vx, 60, 330), 154, 18, steps=10)
    g.cyl(WHITE, (vx, 60, 440), 160, 90, steps=10, top_r=30)
    rd(g, WOOD_D, (vx - 120, 60, 380), (W / 2.0 - 20, 60, 520), 16)


def sugar_mill(sc):
    """BUILDING 3: the sugar mill, 8.2 x 7.2 m, a stepped tower under an icing dome; on the roof a wheel of sugar-cube
    buckets turns slowly (top 13.4 m); a sack rides the hoist."""
    g = Lit(sc.still())
    W, D = 820.0, 720.0
    F = -D / 2.0
    slab(g, WOOD_D, W + 56, D + 56, 120, -260, 46)
    slab(g, PEACH, W, D, 140, 46, 520)
    slab(g, WHITE, W + 70, D + 70, 160, 506, 548)
    drips(g, -W / 2.0 - 10, W / 2.0 + 10, F - 28, 510, 7, 11)
    slab(g, PEACH, W - 130, D - 130, 120, 548, 870)
    F2 = F + 65.0
    grid(g, (-250, 0, 250), (640, 780), F2, -300, 300, 552, 866)
    grid(g, (-340, 340), (290,), F, -340, 340, 60, 500)
    # the door, a shuttered window, sacks by the door
    g.box(WOOD_D, (-170, F - 2, 176), (170, 10, 262))
    g.box(KRAKEN, (-170, F - 6, 166), (136, 10, 236))
    awning(g, -290, -50, F - 6, 330, n=4)
    win(g, 190, F, 250, 170, 170, shut=MAG)
    for k, (x, z, rz) in enumerate(((-20, 78, 0), (40, 78, 14), (10, 138, -8))):
        g.box(WHITE, (x, F - 60, z), (86, 56, 60), rot=(0, rz, 0))
        g.box(KRAKEN, (x, F - 60, z), (30, 60, 64), rot=(0, rz, 0))
    # the chute to a tub of sugar
    tb(g, WOOD, (330, F + 10, 330), (330, F - 150, 120), 70, 14)
    g.box(WOOD_D, (330, F - 170, 80), (120, 110, 70))
    g.cyl(WHITE, (330, F - 170, 114), 50, 56, steps=6, top_r=8)
    # upstairs: the hoist's door, the beam, the rope and the sack that rides it
    g.box(WOOD_D, (0, F2 - 2, 700), (150, 10, 200))
    g.box(GOLD, (0, F2 - 6, 700), (120, 10, 170), glow=WIN)
    g.box(WOOD_D, (0, F2 - 90, 846), (18, 200, 18))
    h = Lit(sc.part(P.motion(bob=-22, rate=0.22, phase=0.3)))
    h.box(WOOD_D, (0, F2 - 170, 730), (6, 6, 220))
    h.box(WHITE, (0, F2 - 170, 580), (76, 56, 92))
    h.box(MAG, (0, F2 - 170, 580), (80, 60, 22))
    win(g, -230, F2, 700, 100, 130)
    win(g, 230, F2, 700, 100, 130)
    winx(g, W / 2.0 + 2, 0, 290, 170, 170)
    winx(g, -W / 2.0 - 2, 0, 290, 170, 170)
    # a lean-to on the left end, its own little icing roof
    g.box(PEACH, (-W / 2.0 - 110, 80, 150), (240, 300, 300))
    slab(g, WHITE, 300, 360, 60, 300, 344, at=(-W / 2.0 - 116, 80))
    drips(g, -W / 2.0 - 240, -W / 2.0 - 10, -100, 304, 3, 13)
    # the roof: icing, a dome, the wheel
    slab(g, WHITE, W - 30, D - 30, 150, 870, 936)
    drips(g, -W / 2.0 + 80, W / 2.0 - 80, F2 - 44, 874, 7, 12)
    g.sphere(WHITE, (0, 0, 936), 300, scale=(1, 0.9, 0.62), steps=(4, 8))
    slab(g, MAG, 420, 380, 120, 1010, 1030)
    g.cyl(WOOD_D, (0, 0, 1090), 24, 180, steps=6)
    w = Lit(sc.part(P.motion(travel=(-1.0, 0.07), base=(0.0, 0.0))))
    w.cyl(WOOD_D, (0, 0, 1200), 60, 44, steps=8)
    for k in range(6):
        a = k * math.pi / 3.0
        ca, sa = math.cos(a), math.sin(a)
        tb(w, WOOD, (ca * 50, sa * 50, 1222), (ca * 330, sa * 330, 1222), 26, 18)
        w.box(WHITE, (ca * 340, sa * 340, 1196), (82, 82, 82), rot=(0, math.degrees(a), 0))
        b = a + math.pi / 3.0
        tb(w, MAG, (ca * 300, sa * 300, 1240), (math.cos(b) * 300, math.sin(b) * 300, 1240), 18, 14)
    g.cyl(WHITE, (0, 0, 1244), 70, 70, steps=8, top_r=30)
    g.cyl(MAG, (0, 0, 1300), 30, 44, steps=6, top_r=6)


def fountain_hall(sc):
    """BUILDING 4: the chocolate fountain hall, 10 x 7 m: an open front of four candy columns, the stepped brown
    fountain inside before a lit wall (its streams shimmer), under one great icing dome (top 11.2 m)."""
    g = Lit(sc.still())
    W, D = 1000.0, 700.0
    F = -D / 2.0
    slab(g, WOOD_D, W + 56, D + 56, 120, -260, 46)
    g.box(WOOD_D, (0, F - 50, 22), (560, 70, 30))
    g.box(WOOD_D, (0, F - 110, 8), (640, 70, 16))
    g.box(PEACH, (0, D / 2.0 - 40, 326), (W, 80, 560))
    for s in (-1, 1):
        g.box(PEACH, (s * (W / 2.0 - 50), 0, 326), (100, D, 560))
        g.box(WOOD, (s * (W / 2.0 - 50), F - 2, 326), (8, 8, 540))
        winx(g, s * (W / 2.0 + 2), 60, 300, 190, 260)
    g.box(GOLD, (0, D / 2.0 - 84, 320), (W - 200, 8, 500), glow=0.6)
    g.box(CREAM, (0, 0, 52), (W - 190, D - 80, 12))
    for x in (-360, -130, 130, 360):
        cane(g, x, F + 40, 46, 544, r=26, rings=4)
    g.box(WHITE, (0, F + 40, 330), (W - 240, 8, 10))
    g.box(WHITE, (0, F + 40, 470), (W - 240, 8, 10))
    g.box(WHITE, (0, F + 30, 620), (W + 20, 130, 60))
    # the fountain: three chocolate bowls, a jet, streams from rim to rim
    fy = 30.0
    g.cyl(WOOD_D, (0, fy, 58), 200, 46, steps=10, top_r=214)
    g.cyl(WOOD, (0, fy, 104), 46, 150, steps=6)
    g.cyl(WOOD_D, (0, fy, 254), 130, 30, steps=10, top_r=150)
    g.cyl(WOOD, (0, fy, 284), 34, 126, steps=6)
    g.cyl(WOOD_D, (0, fy, 410), 80, 26, steps=8, top_r=96)
    g.cyl(WOOD, (0, fy, 436), 24, 96, steps=6, top_r=8)
    st = Lit(sc.part(P.motion(bob=-5, rate=0.9, phase=0.1)))
    for k in range(6):
        a = k * math.pi / 3.0 + 0.3
        st.box(WOOD, (math.cos(a) * 92, fy + math.sin(a) * 92, 352), (16, 16, 150), rot=(0, math.degrees(a), 0))
    for k in range(8):
        a = k * math.pi / 4.0
        st.box(WOOD, (math.cos(a) * 146, fy + math.sin(a) * 146, 186), (18, 18, 180), rot=(0, math.degrees(a), 0))
    # the dome
    slab(g, WHITE, W + 124, D + 124, 170, 650, 722)
    drips(g, -W / 2.0 - 20, W / 2.0 + 20, F - 54, 654, 10, 15)
    drips(g, -W / 2.0 - 20, W / 2.0 + 20, -F + 54, 654, 5, 16)
    g.sphere(WHITE, (0, 0, 722), 420, scale=(1, 0.74, 0.6), steps=(4, 10))
    slab(g, MAG, 620, 460, 150, 850, 872)
    slab(g, KRAKEN, 400, 300, 110, 920, 938)
    g.cyl(WHITE, (0, 0, 960), 96, 170, steps=8, top_r=12)
    cone_chim(g, -330, 150, 722, 250, 66)
    tub(g, -440, F - 60, 46)
    tub(g, 440, F - 60, 46)


def cane_cafe(sc):
    """BUILDING 5: the candy-cane colonnade cafe, 12 x 4.6 m: five candy-cane columns carry an icing roof over a
    counter, a set-back upper floor with a balcony, a cone chimney (top 11.4 m), a pennant."""
    g = Lit(sc.still())
    L, D = 1200.0, 460.0
    slab(g, WOOD_D, L + 50, D + 50, 70, -260, 46)
    g.box(PEACH, (0, 120, 236), (L - 40, 200, 380))
    grid(g, (-560, 560), (), 20, -560, 560, 50, 422)
    for k in range(5):
        cane(g, -540 + k * 270, -170, 46, 380, r=22)
    # the counter, stools, a lit hatch, a board of stripes (the menu, no words)
    g.box(WOOD_D, (-60, -30, 96), (700, 56, 100))
    g.box(WHITE, (-60, -30, 152), (730, 76, 12))
    for k in range(4):
        g.box(MAG if k % 2 else KRAKEN, (-300 + k * 160, -110, 76), (44, 44, 60))
    g.box(GOLD, (-60, 16, 290), (520, 8, 150), glow=WIN)
    g.box(WOOD_D, (-60, 18, 290), (10, 10, 150))
    g.box(KRAKEN, (420, 16, 270), (170, 8, 220))
    for k in range(4):
        g.box(WHITE, (420, 11, 200 + k * 46), (120 - (k % 2) * 30, 6, 12))
    for k in range(4):
        g.box(BULB, (-405 + k * 270, -170, 392), (24, 24, 24), glow=LAMP)
    tub(g, 520, -110, 46, 0.9)
    # the roof over the colonnade
    slab(g, WHITE, L + 110, D + 110, 100, 426, 492)
    drips(g, -L / 2.0 - 20, L / 2.0 + 20, -D / 2.0 - 46, 430, 12, 21)
    drips(g, -L / 2.0 - 20, L / 2.0 + 20, D / 2.0 + 46, 430, 5, 22)
    # upstairs, set back, with a balcony rail
    slab(g, PEACH, 840, 300, 90, 492, 764, at=(0, 60))
    grid(g, (-340, 340), (), -90, -340, 340, 496, 760)
    for x in (-250, 0, 250):
        win(g, x, -90, 634, 130, 150, shut=MAG if x else None)
    g.box(WHITE, (0, -210, 572), (L - 100, 10, 12))
    for k in range(7):
        g.box(WHITE, (-540 + k * 180, -210, 532), (10, 10, 80))
    slab(g, WHITE, 940, 400, 110, 764, 834, at=(0, 60))
    drips(g, -440, 440, -134, 768, 8, 23)
    slab(g, MAG, 668, 268, 90, 850, 868, at=(0, 60))
    slab(g, WHITE, 660, 260, 90, 834, 904, at=(0, 60))
    cone_chim(g, 320, 60, 834, 290, 76)
    pennant(sc, (-250, 60, 904), col=KRAKEN, phase=0.4, g=g)


def jam_jar(sc):
    """BUILDING 6: the jam-jar tower, 4 m across and 16.9 m tall: a round jar of jam with a white label, a gingham
    cloth tied over its lid, a cone on top, a giant wooden spoon leaning on it."""
    g = Lit(sc.still())
    g.cyl(WOOD_D, (0, 0, -260), 230, 306, steps=10)
    g.cyl(KRAKEN, (0, 0, 46), 200, 1134, steps=10)
    g.cyl(WHITE, (0, 0, 520), 205, 300, steps=10)
    g.box(MAG, (0, -200, 670), (130, 16, 130), rot=(45, 0, 0))
    g.box(GOLD, (0, -206, 670), (56, 12, 56), rot=(45, 0, 0), glow=WIN)
    # the door and its awning, lit windows up the jar
    g.box(WOOD_D, (0, -193, 156), (130, 14, 224))
    g.box(MAG, (0, -198, 150), (100, 12, 200))
    awning(g, -90, 90, -196, 300, depth=90, drop=36, n=4)
    for z in (400, 930, 1090):
        g.box(WOOD_D, (0, -193, z), (106, 12, 126))
        g.box(GOLD, (0, -198, z), (84, 12, 104), glow=WIN)
    for a in (-58.0, 58.0, 122.0, -122.0):
        ca, sa = math.cos(math.radians(a - 90)), math.sin(math.radians(a - 90))
        for z in (400, 930):
            g.box(GOLD, (ca * 190, sa * 190, z), (84, 20, 104), rot=(0, a, 0), glow=WIN)
    # shoulder, neck, the cloth over the lid, its string and bow
    g.cyl(KRAKEN, (0, 0, 1180), 200, 110, steps=10, top_r=164)
    g.cyl(PEACH, (0, 0, 1290), 164, 60, steps=10)
    g.cyl(WHITE, (0, 0, 1330), 252, 76, steps=10, top_r=186)
    g.cyl(MAG, (0, 0, 1392), 192, 16, steps=10)
    g.cyl(WHITE, (0, 0, 1406), 188, 64, steps=10)
    for k in range(10):
        a = k * 36.0
        ca, sa = math.cos(math.radians(a - 90)), math.sin(math.radians(a - 90))
        g.box(MAG, (ca * 222, sa * 222, 1364), (56, 18, 40), rot=(0, a, 0))
        if k % 2 == 0:
            g.box(MAG, (ca * 120, sa * 120, 1472), (70, 70, 8), rot=(0, a, 0))
    for s in (-1, 1):
        g.box(MAG, (s * 40, -196, 1392), (60, 14, 40), rot=(s * 20, 0, 0))
    g.sphere(WHITE, (0, 0, 1476), 90, scale=(1, 1, 0.4), steps=(3, 6))
    g.cyl(PEACH, (0, 0, 1482), 62, 210, steps=6, top_r=10)
    g.cyl(WOOD, (0, 0, 1545), 48, 14, steps=6)
    # the spoon
    tb(g, WOOD, (330, -70, -120), (232, -30, 800), 26, 16)
    g.sphere(WOOD, (222, -26, 866), 66, scale=(1, 0.42, 1.3), steps=(3, 6))
    pennant(sc, (0, 0, 1630), col=WHITE, w=90, h=54, mast=70, phase=0.7, g=g)


# ================================================================================================ the props
def cane_row(sc):
    """PROP 1 (tall, 7.2 m): three giant candy canes in chocolate tubs, their crooks bent along the road."""
    g = Lit(sc.still())
    for k, (x, h, col, sx) in enumerate(((-270.0, 500.0, MAG, 1.0), (0.0, 640.0, KRAKEN, -1.0), (270.0, 560.0, MAG, 1.0))):
        g.box(WOOD_D, (x, 0, 0), (84, 78, 160))
        g.box(WHITE, (x, 0, 86), (98, 90, 14))
        cane(g, x, 0, 80, h - 80, r=24, col=col, rings=5)
        crook(g, x, 0, h, 24, 130, col=col, sx=sx)


def sugar_cubes(sc):
    """PROP 2 (low, 1.45 m, unlit): sugar cubes as seats, a run of them 4 m long, two stacked, one askew."""
    g = Lit(sc.still(), 0.0)
    for x, y, z, yaw in ((-170, 0, 36, 0), (-86, 6, 36, 8), (-128, 2, 108, 14), (10, -4, 36, -6), (104, 4, 36, 0), (186, 0, 36, 22), (104, 6, 108, -18)):
        g.box(WHITE, (x, y, z), (74, 74, 72), rot=(0, yaw, 0))
    g.box(MAG, (-128, 2, 148), (78, 16, 8), rot=(0, 14, 0))
    g.box(MAG, (-128, 2, 148), (16, 78, 8), rot=(0, 14, 0))
    g.box(WOOD_D, (10, 0, -25), (470, 110, 50))


def cone_rack(sc):
    """PROP 3 (tall, 3.7 m): a stand of five giant ice-cream cones in a chocolate rack under a striped roof; the
    cones' tops are piped swirls, pointed (nothing round on a stick)."""
    g = Lit(sc.still())
    g.box(WOOD_D, (0, 0, -80), (420, 110, 190))
    for s in (-1, 1):
        g.box(WOOD_D, (s * 200, 0, 170), (18, 90, 320))
    g.box(WOOD_D, (0, 0, 150), (420, 100, 16))
    g.box(WOOD, (0, -52, 86), (400, 8, 100))
    for k in range(5):
        x = -150 + k * 75.0
        g.cyl(PEACH, (x, 0, 30), 6, 150, steps=6, top_r=34)
        g.cyl(WOOD, (x, 0, 130), 31, 10, steps=6)
        c = (WHITE, MAG, WHITE, KRAKEN, WHITE)[k]
        g.cyl(c, (x, 0, 180), 38, 30, steps=6, top_r=30)
        g.cyl(WHITE if c != WHITE else CREAM, (x, 0, 210), 28, 26, steps=6, top_r=18)
        g.cyl(c, (x, 0, 236), 16, 34, steps=6, top_r=4)
    for k in range(6):
        x = -175 + k * 70.0
        tb(g, (MAG, WHITE)[k % 2], (x, 0, 370), (x, -90, 322), 70, 8)
        tb(g, (MAG, WHITE)[k % 2], (x, 0, 370), (x, 90, 322), 70, 8)
    g.box(WOOD_D, (0, 0, 334), (424, 14, 14))
    g.box(BULB, (0, -40, 300), (300, 10, 10), glow=LAMP)


def liquorice_gate(sc):
    """PROP 4 (tall, 5.5 m): a gate between two liquorice wheels on chocolate posts, a rope of icing looped between
    them with pennants, wafer leaves standing open."""
    g = Lit(sc.still())
    for s in (-1, 1):
        x = s * 190.0
        g.box(WOOD_D, (x, 0, 60), (70, 70, 520))
        g.box(WHITE, (x, 0, 326), (86, 86, 16))
        rd(g, PLUM, (x, -32, 440), (x, 32, 440), 124, steps=10)
        rd(g, WOOD_D, (x, -38, 440), (x, 38, 440), 86, steps=10)
        rd(g, PLUM, (x, -44, 440), (x, 44, 440), 50, steps=8)
        rd(g, MAG, (x, -50, 440), (x, 50, 440), 20, steps=6)
        # a wafer leaf, open towards the back
        tb(g, PEACH, (x - s * 30, 20, 150), (x - s * 60, 170, 150), 8, 220)
        tb(g, WOOD, (x - s * 34, 22, 150), (x - s * 64, 172, 150), 12, 10)
        tb(g, WOOD, (x - s * 34, 22, 220), (x - s * 64, 172, 220), 12, 10)
        tb(g, WOOD, (x - s * 34, 22, 80), (x - s * 64, 172, 80), 12, 10)
    pts = [(-190.0, 470.0), (-110.0, 400.0), (0.0, 376.0), (110.0, 400.0), (190.0, 470.0)]
    for k in range(4):
        rd(g, WHITE, (pts[k][0], -50, pts[k][1] + 50), (pts[k + 1][0], -50, pts[k + 1][1] + 50), 9, steps=5)
    for k, (x, z) in enumerate(((-150, 484), (-55, 436), (55, 436), (150, 484))):
        g.side((KRAKEN, MAG, WHITE, KRAKEN)[k], [(x - 26, z), (x + 26, z), (x, z - 62)], -54, -46)


def syrup_cart(sc):
    """PROP 5 (low, 1.75 m, unlit, brown): a syrup barrel lying on a two-wheeled cart, a pail under its tap."""
    g = Lit(sc.still(), 0.0)
    g.box(WOOD, (0, 0, 62), (230, 96, 14))
    for s in (-1, 1):
        rd(g, WOOD_D, (20, s * 56, 46), (20, s * 68, 46), 46, steps=8)
        rd(g, WOOD, (20, s * 68, 46), (20, s * 74, 46), 14, steps=6)
        tb(g, WOOD, (-110, s * 40, 62), (-230, s * 40, 96), 10, 10)
        g.box(WOOD_D, (60, s * 36, 86), (30, 16, 36))
    g.box(WOOD_D, (-226, 0, 96), (10, 96, 10))
    g.box(WOOD_D, (100, 0, 28), (12, 12, 56))
    rd(g, WOOD_D, (-70, 0, 122), (110, 0, 122), 54, steps=10)
    rd(g, WOOD_D, (-84, 0, 122), (-70, 0, 122), 44, steps=10)
    rd(g, WOOD_D, (110, 0, 122), (124, 0, 122), 44, steps=10)
    for x in (-40.0, 20.0, 80.0):
        rd(g, PEACH, (x - 5, 0, 122), (x + 5, 0, 122), 57, steps=10)
    g.box(PEACH, (132, 0, 104), (22, 10, 10))
    g.cyl(PEACH, (150, 0, 0), 20, 36, steps=6, top_r=26)
    g.box(WOOD_D, (0, 0, -25), (150, 150, 50))


def wafer_fence(sc):
    """PROP 6 (low, 1.5 m, unlit): eight metres of wafer fence between chocolate posts with caps of icing."""
    g = Lit(sc.still(), 0.0)
    L = 800.0
    g.box(WOOD_D, (0, 0, -22), (L, 30, 56))
    g.box(PEACH, (0, 0, 62), (L - 20, 8, 96))
    for z in (36.0, 88.0):
        g.box(WOOD, (0, 0, z), (L - 20, 14, 8))
    for k in range(16):
        g.box(WOOD, (-L / 2.0 + 60 + k * 45.4, 0, 62), (8, 14, 96))
    for k in range(5):
        x = -L / 2.0 + 10 + k * (L - 20) / 4.0
        g.box(WOOD_D, (x, 0, 60), (26, 26, 130))
        g.box(WHITE, (x, 0, 132), (40, 40, 16))
        g.cyl(WHITE, (x, 0, 138), 14, 14, steps=5, top_r=5)
    g.box(WHITE, (0, 0, 114), (L - 20, 16, 10))
    drips(g, -L / 2.0 + 20, L / 2.0 - 20, -6, 110, 9, 31)


def wash_line(sc):
    """PROP 7 (tall, 3.5 m): the bakers' washing between two candy-cane poles: aprons and gingham tea towels that
    swing on the line."""
    g = Lit(sc.still())
    for s in (-1, 1):
        g.box(WOOD_D, (s * 160, 0, -30), (60, 60, 120))
        cane(g, s * 160, 0, 30, 310, r=12, rings=4, steps=5)
        g.box(WHITE, (s * 160, 0, 344), (30, 30, 10))
    g.box(WOOD_D, (0, 0, 318), (320, 5, 5))
    cloths = ((-112, 54, 96, WHITE, MAG), (-42, 66, 130, PEACH, KRAKEN), (36, 50, 84, MAG, WHITE), (104, 60, 118, WHITE, KRAKEN))
    for k, (x, w, h, col, band) in enumerate(cloths):
        c = Lit(sc.part(P.motion(swing=13, pivot=(x, 0, 318), axis=0.0, rate=0.35 + 0.05 * k, phase=0.23 * k)))
        c.box(col, (x, 0, 318 - h / 2.0), (w, 5, h))
        c.box(band, (x, 0, 318 - h * 0.72), (w + 2, 7, h * 0.16))
        c.box(band, (x, 0, 318 - h * 0.3), (w * 0.3, 7, h * 0.3))


def choc_bench(sc):
    """PROP 8 (low, 1.2 m, unlit): a bench that is a bar of chocolate, a wafer for its back, candy-cane legs."""
    g = Lit(sc.still(), 0.0)
    g.box(WOOD_D, (0, 0, 54), (260, 66, 16))
    for k in range(5):
        g.box(WOOD, (-104 + k * 52, -4, 64), (44, 50, 8))
    g.box(PEACH, (0, 34, 96), (250, 8, 60))
    for z in (80.0, 110.0):
        g.box(WOOD, (0, 30, z), (250, 10, 6))
    for k in range(6):
        g.box(WOOD, (-110 + k * 44, 30, 96), (6, 10, 60))
    for s in (-1, 1):
        g.cyl(WHITE, (s * 112, 0, -30), 12, 78, steps=5)
        g.cyl(MAG, (s * 112, 0, 6), 14, 14, steps=5)
        g.box(WOOD_D, (s * 126, 30, 76), (10, 10, 96))
        g.box(WHITE, (s * 126, 30, 126), (20, 20, 10))


def sugar_sacks(sc):
    """PROP 9 (low, 1.45 m, unlit): a pile of sugar sacks with pink bands and a sack truck leaning on it."""
    g = Lit(sc.still(), 0.0)
    g.box(WOOD_D, (0, 0, -25), (300, 110, 50))
    for x, y, z, yaw, band in ((-90, 0, 28, 6, MAG), (0, 6, 28, -4, KRAKEN), (92, -2, 28, 10, MAG), (-44, 2, 82, -10, KRAKEN), (48, 4, 82, 14, MAG), (2, 0, 132, 2, KRAKEN)):
        g.box(WHITE, (x, y, z), (92, 64, 52), rot=(0, yaw, 0))
        g.box(band, (x, y, z), (24, 68, 56), rot=(0, yaw, 0))
        for s in (-1, 1):
            g.box(WHITE, (x + s * 46 * math.cos(math.radians(yaw)), y + s * 46 * math.sin(math.radians(yaw)), z + 22), (12, 20, 12), rot=(0, yaw, 0))
    tb(g, WOOD_D, (176, -20, 0), (150, -4, 138), 8, 8)
    tb(g, WOOD_D, (176, 24, 0), (150, 40, 138), 8, 8)
    g.box(WOOD_D, (150, 18, 140), (8, 60, 8))
    g.box(WOOD_D, (190, 2, 6), (40, 56, 8))
    rd(g, WOOD, (182, -32, 14), (182, -24, 14), 14, steps=6)
    rd(g, WOOD, (182, 28, 14), (182, 36, 14), 14, steps=6)


# ================================================================================================ the hero
def great_swirl(sc):
    """HERO: THE GREAT SWIRL, 69 m: a wafer cone that flares up from a chocolate terrace, four twists of soft ice
    (pink, white, pink, white) to a curled tip, a slide winding down the groove with five sliders going round, a red
    flag on the top. 28 m across at its widest."""
    g = Lit(sc.still(), 0.10)
    g.cyl(WOOD_D, (0, 0, -320), 1250, 400, steps=14)
    g.cyl(WHITE, (0, 0, 80), 1190, 24, steps=14)
    # the wafer cone and its grid
    Z0, Z1, R0, R1 = 100.0, 2600.0, 760.0, 1150.0

    def rc(z):
        return R0 + (R1 - R0) * (z - Z0) / (Z1 - Z0)

    g.cyl(PEACH, (0, 0, Z0), R0, Z1 - Z0, steps=12, top_r=R1)
    for k in range(12):
        a = math.radians(k * 30.0 + 15.0)
        ca, sa = math.cos(a), math.sin(a)
        tb(g, WOOD, (ca * (R0 * 0.975), sa * (R0 * 0.975), Z0), (ca * (R1 * 0.975), sa * (R1 * 0.975), Z1), 30, 22)
    for z in (700.0, 1350.0, 2000.0):
        g.cyl(WOOD, (0, 0, z), rc(z) + 4, 26, steps=12)
    # the porch and door, front
    py = -rc(340.0) * 0.966
    g.box(PEACH, (0, py - 60, 360), (420, 260, 520))
    g.box(KRAKEN, (0, py - 194, 300), (220, 10, 400))
    g.box(GOLD, (0, py - 198, 420), (120, 8, 110), glow=WIN)
    slab(g, WHITE, 540, 380, 90, 620, 670, at=(0, py - 80))
    drips(g, -230, 230, py - 262, 624, 6, 41)
    for s in (-1, 1):
        cane(g, s * 230, py - 220, 104, 516, r=26, rings=4)
    # two rings of lit windows
    for z, n in ((1050.0, 8), (1750.0, 8)):
        for k in range(n):
            ad = k * 360.0 / n + 22.5
            a = math.radians(ad - 90.0)
            g.box(GOLD, (math.cos(a) * rc(z), math.sin(a) * rc(z), z), (150, 60, 240), rot=(0, ad, 0), glow=WIN)
    # the rim: icing over the cone's edge, a rail, lamps
    g.cyl(WHITE, (0, 0, Z1 - 20), 1235, 84, steps=14)
    for k in range(14):
        a = math.radians(k * 360.0 / 14.0 + 9.0)
        ca, sa = math.cos(a), math.sin(a)
        ln = (150.0, 260.0, 110.0, 200.0)[k % 4]
        g.box(WHITE, (ca * 1215, sa * 1215, Z1 - 20 - ln / 2.0), (70, 46, ln), rot=(0, math.degrees(a) + 90.0, 0))
        g.box(BULB, (ca * 1240, sa * 1240, Z1 + 150), (34, 34, 34), glow=LAMP)
        g.box(WHITE, (ca * 1240, sa * 1240, Z1 + 100), (14, 14, 80))
    # the soft ice: four twists, each swelling and then drawing in
    tiers = ((2660.0, 1150.0, 1230.0, 200.0, 900.0, 620.0, MAG), (3480.0, 900.0, 1000.0, 170.0, 660.0, 600.0, WHITE),
             (4250.0, 660.0, 740.0, 150.0, 420.0, 560.0, MAG), (4960.0, 420.0, 480.0, 120.0, 150.0, 620.0, WHITE))
    for z, ra, rb, h1, rcn, h2, col in tiers:
        g.cyl(col, (0, 0, z), ra, h1, steps=12, top_r=rb)
        g.cyl(col, (0, 0, z + h1), rb, h2, steps=12, top_r=rcn)
    g.cyl(WHITE, (0, 0, 5690), 152, 520, rot=(12, 0, 0), steps=8, top_r=22)
    g.cyl(WOOD_D, (-100, 0, 6150), 9, 620, steps=5)
    f = Lit(sc.part(P.motion(swing=22, pivot=(-100, 0, 6700), axis=P.UP, rate=0.25, phase=0.1)), 0.3)
    f.side(RED, [(-92, 6770), (190, 6690), (-92, 6610)], -5, 5)
    # the slide: a ribbon down the groove, two and a half turns

    def rs(z):
        return 1400.0 - (z - 2700.0) * (800.0 / 2300.0)

    n = 32
    prev = None
    for k in range(n + 1):
        t = k / float(n)
        z = 5000.0 - 2300.0 * t
        a = math.radians(-90.0 + 900.0 * t)
        p = (math.cos(a) * rs(z), math.sin(a) * rs(z), z)
        if prev:
            tb(g, KRAKEN, prev, p, 190, 34)
        prev = p
    g.box(KRAKEN, (prev[0], prev[1], 2690), (260, 260, 30))
    # the sliders: on their mats, going round the tower in the groove
    cols = ('#2ee6ff', '#ffc93c', '#7dffb0', '#fff6ea', '#ff5fa2')
    for k, z in enumerate((4720.0, 4240.0, 3760.0, 3280.0, 2900.0)):
        a = math.radians(40.0 + k * 97.0)
        x, y = math.cos(a) * rs(z), math.sin(a) * rs(z)
        m = Lit(sc.part(P.motion(travel=(-1.0, -(0.03 + 0.004 * k)), base=(0.0, 0.0), phase=0.1 * k)), 0.3)
        yaw = math.degrees(a) + 90.0
        m.box(GOLD, (x, y, z + 30), (230, 130, 24), rot=(0, yaw, 0))
        m.box(cols[k], (x, y, z + 110), (110, 100, 140), rot=(0, yaw, 0))
        m.box('#eebf98', (x, y, z + 222), (84, 84, 84), rot=(0, yaw, 0))
    # candy canes round the terrace
    for k in range(6):
        a = math.radians(k * 60.0 + 30.0)
        x, y = math.cos(a) * 1130, math.sin(a) * 1130
        cane(g, x, y, 100, 420, r=22, rings=2, col=KRAKEN if k % 2 else MAG, steps=5)
        g.box(BULB, (x, y, 540), (40, 40, 40), glow=LAMP)


JOBS = [
    ('SM_S1D5_WaferHouse', wafer_house),
    ('SM_S1D5_ToffeeWorks', toffee_works),
    ('SM_S1D5_SugarMill', sugar_mill, dict(grow=(60, 60, 60), grow_down=(60, 60, 0))),
    ('SM_S1D5_FountainHall', fountain_hall),
    ('SM_S1D5_CaneCafe', cane_cafe),
    ('SM_S1D5_JamJar', jam_jar),
    ('SM_S1D5_CaneRow', cane_row, STILL),
    ('SM_S1D5_SugarCubes', sugar_cubes, STILL),
    ('SM_S1D5_ConeRack', cone_rack, STILL),
    ('SM_S1D5_LiquoriceGate', liquorice_gate, STILL),
    ('SM_S1D5_SyrupCart', syrup_cart, STILL),
    ('SM_S1D5_WaferFence', wafer_fence, STILL),
    ('SM_S1D5_WashLine', wash_line),
    ('SM_S1D5_ChocBench', choc_bench, STILL),
    ('SM_S1D5_SugarSacks', sugar_sacks, STILL),
    ('SM_S1D5_GreatSwirl', great_swirl),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
