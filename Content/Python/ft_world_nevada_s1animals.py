"""Nevada, Stage 1, builder s1animals: the animals of the eight districts.
/Game/World/Nevada/SM_S1ANIMALS_*. Run: import ft_world_nevada_s1animals as K; K.run()

Nineteen pieces, each an ft_people.Scene (one material, M_People; one section; movement in the vertex shader only):
  on the ground   BurroShelf, SheepFlock (with its dog and its shepherd), MuleDeer, Jackrabbits, Roadrunners, FoxDen,
                  Tortoises, QuailWall, Peacocks, HenCoop, CraneDance, PrairieDogs
  on water        DuckTank, BrinePool, SwanLagoon, FlamingoPool (no water surface on this level: each brings its own)
  in the sky      PelicanLine and Owls (they travel ALONG the road, saved far=True), Dovecote (white doves in a tight wheel
                  over their own cote, 9 m at the widest)
Creatures are ft_people.quadruped / ft_people.bird where those fit, and plain boxes drawn into moving parts where they do
not (ducks, swans, flamingos, quail, hares, tortoises). Sizes are exaggerated on purpose: the pieces stand 42 to 150 m out.
Colours are the kit's (ft_dress_nevada.py, ft_desert.py) and ft_people's own. Nothing glows. No dark flyers.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Nevada'

SAND, SANDST_L, SANDST_D = '#f2c58a', '#f59a62', '#b8553a'
SAGE, SAGE_D = '#8fb86a', '#5f8f4a'
BARK, BARK_D, WOOD = '#8a6a4a', '#6a4c36', '#9a6a44'
GREY, SHEET, STEEL = '#c9c4d6', '#8f9bb3', '#d7e3f0'
RED, MAG, PINK = '#ff3355', '#ff4fb8', '#ff5fa2'
TEAL, POOL, BLUE, NAVY = '#2fc6c0', '#3fd6d0', '#3a5fa8', '#2b2350'
CREAM, WHITE, CONC = '#f4efe6', '#fff6ea', '#c9b8a0'
DRYG, WIRE, WARM, GOLD, CRIM = '#d8b060', '#3a2a2a', '#ffc86a', '#ffc93c', '#6a2440'


# ---------------------------------------------------------------------------------------------- shared
class Fr(object):
    """A creature's own frame: it stands on (x, y, z), heads along its own +X, turned `yaw` (0 = the mesh's +X, 90 = +Y,
    -90 = towards the road), sized s. part() hands out the model for one way of moving, with the pivot given in the
    creature's frame; `common` (phase, rate, bob, base, travel) is shared by all its parts."""

    def __init__(self, sc, x, y, z=0.0, yaw=0.0, s=1.0, **common):
        self.sc, self.at, self.yaw, self.s, self.common = sc, (float(x), float(y), float(z)), float(yaw), float(s), common
        a = math.radians(yaw)
        self.c, self.sn = math.cos(a), math.sin(a)

    def W(self, p):
        return (self.at[0] + self.s * (p[0] * self.c - p[1] * self.sn), self.at[1] + self.s * (p[0] * self.sn + p[1] * self.c), self.at[2] + self.s * p[2])

    def part(self, swing=0.0, pivot=(0.0, 0.0, 0.0), axis=0.0, **kw):
        mo = dict(self.common)
        mo.update(kw)
        return self.sc.part(P.motion(swing=swing, pivot=self.W(pivot), axis=(P.UP if axis == P.UP else self.yaw + axis), **mo), self.at, self.yaw, self.s)


def along(x, y, length, rps):
    """common= for a creature that travels the mesh's X (length cm, rounds a second; negative goes towards -X)."""
    return dict(base=(float(x), float(y)), travel=(float(length), float(rps)))


def ring(rps, cx=0.0, cy=0.0):
    """common= for a creature that circles (cx, cy)."""
    return dict(base=(float(cx), float(cy)), travel=(-1.0, float(rps)))


def ellipse(a, b, n=12, cx=0.0, cy=0.0):
    return [(cx + a * math.cos(2.0 * math.pi * k / n), cy + b * math.sin(2.0 * math.pi * k / n)) for k in range(n)]


def stadium(a, b, n=5):
    """A basin's plan: 2a long, 2b wide, round ends."""
    pts = []
    for k in range(n):
        t = math.radians(-90.0 + 180.0 * k / (n - 1))
        pts.append((a - b + b * math.cos(t), b * math.sin(t)))
    for k in range(n):
        t = math.radians(90.0 + 180.0 * k / (n - 1))
        pts.append((-(a - b) + b * math.cos(t), b * math.sin(t)))
    return pts


def tuft(g, col, x, y, z, s=1.0):
    """A clump of grass or brush: three leaning blades."""
    g.box(col, (x, y, z + 16 * s), (10 * s, 10 * s, 36 * s))
    g.box(col, (x + 12 * s, y + 4 * s, z + 12 * s), (9 * s, 9 * s, 28 * s), rot=(0, 0, 18))
    g.box(col, (x - 11 * s, y - 3 * s, z + 11 * s), (9 * s, 9 * s, 26 * s), rot=(0, 0, -20))


def quad(sc, x, y, z, yaw, s, spec, ears=None, flick=0.0, **mv):
    """ft_people.quadruped (its head towards -Y at yaw 0, so yaw 90 heads it along +X) with long ears of our own:
    ears = (colour, length, width). They nod with a grazing head, turn on their own (flick, degrees) on a still one,
    and ride along with a walker."""
    kw = dict(spec)
    kw.update(mv)
    P.quadruped(sc, x, y, z, yaw=yaw, s=s, **kw)
    if not ears:
        return
    col, long, wide = ears
    L, H, G = kw.get('length', 150.0), kw.get('height', 120.0), kw.get('girth', 42.0)
    NK, NU, HD = kw.get('neck', 50.0), kw.get('neck_up', 55.0), kw.get('head', 34.0)
    hl, a = L / 2.0, math.radians(NU)
    n1y, n1z = -hl + G * 0.25 - math.cos(a) * NK, H - G * 0.35 + math.sin(a) * NK
    cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))

    def W(p):
        return (x + s * (p[0] * cy - p[1] * sy), y + s * (p[0] * sy + p[1] * cy), z + s * p[2])

    tv = (0.0, 0.0)
    if kw.get('travel'):
        tv = (float(kw['travel'][0]), float(kw['travel'][1]) / float(kw['travel'][0]))
    common = dict(phase=kw.get('phase', 0.0), rate=kw.get('rate', 0.0), bob=kw.get('bob', 0.0), base=(x, y), travel=tv)
    graze = kw.get('graze', 0.0)
    for side in (-1, 1):
        root = (side * G * 0.2, n1y + HD * 0.1, n1z + HD * 0.38)
        if graze:
            mo = P.motion(swing=graze, pivot=W((0, -hl + G * 0.2, H - G * 0.3)), axis=yaw, **common)
        elif flick:
            mo = P.motion(swing=side * flick, pivot=W(root), axis=P.UP, **dict(common, rate=common['rate'] or 0.7))
        else:
            mo = P.motion(pivot=W((0, 0, H)), **common)
        m = sc.part(mo, (x, y, z), yaw, s)
        tilt_box(m, col, root, (side * (G * 0.2 + long * 0.42), n1y + HD * 0.2, n1z + HD * 0.38 + long * 0.9), wide, max(wide * 0.45, 4.6))


# ---------------------------------------------------------------------------------------------- 1 burros
BURRO = dict(length=120, height=100, girth=46, neck=38, neck_up=42, head=40, body=SHEET, belly=CREAM, snout=CREAM, mane=WIRE, ears=SHEET,
             tail=40, tail_col=WIRE)
BURRO_EARS = (SHEET, 34, 12)


def burro_shelf(sc):
    """A burro band on a pale rock shelf, 14 x 4.6 m: one drinks at a trough, a foal beside it, a pack burro up on the
    step flicks its ears, one walks the shelf's front and comes round."""
    g = sc.still()
    g.box(CONC, (0, 0, -20), (1400, 460, 260))                       # the shelf: its top at 1.1 m
    g.box(SAND, (0, -232, 62), (1404, 8, 26))
    g.box(SANDST_L, (0, -232, 8), (1404, 8, 20))
    g.box(CONC, (340, 80, 150), (600, 300, 80))                      # the step: 1.9 m
    g.box(CREAM, (340, 80, 193), (620, 320, 8))
    g.box(SANDST_L, (-640, 160, 150), (120, 130, 100), rot=(0, 25, 0))
    g.box(SAND, (670, -150, 135), (70, 90, 60), rot=(0, -15, 0))
    g.box(WOOD, (-170, 60, 132), (60, 200, 44))                      # the trough
    g.box(POOL, (-170, 60, 156), (44, 184, 6))
    tuft(g, DRYG, -560, -120, 110)
    tuft(g, DRYG, 120, 190, 110)
    tuft(g, SAGE_D, 610, 150, 197)
    quad(sc, -330, 60, 110, 90, 1.25, BURRO, BURRO_EARS, neck_up=-8, graze=7, wag=22, rate=0.5, phase=0.1)
    quad(sc, -500, 130, 110, 60, 0.95, BURRO, BURRO_EARS, flick=24, wag=30, rate=0.9, phase=0.4)
    quad(sc, 340, 80, 197, -60, 1.25, BURRO, BURRO_EARS, flick=20, wag=18, rate=0.6, phase=0.7)
    p = sc.still((340, 80, 197), -60, 1.25)                           # the pack: blanket, panniers, a bedroll
    p.box(TEAL, (0, 4, 103), (54, 62, 8))
    p.box(MAG, (0, 4, 104), (56, 12, 8))
    for side in (-1, 1):
        p.box(WOOD, (side * 32, 4, 84), (16, 46, 34))
    p.box(CREAM, (0, 16, 116), (64, 20, 18))
    quad(sc, 0, -150, 110, 90, 1.25, BURRO, BURRO_EARS, stride=18, wag=14, rate=1.1, phase=0.2, travel=(1150, 75))


# ---------------------------------------------------------------------------------------------- 2 sheep
SHEEP = dict(length=95, height=70, girth=54, neck=20, neck_up=12, head=30, body=WHITE, legs_col=WIRE, snout=WIRE, ears=WIRE, tail=12,
             tail_col=WHITE)
DOG = dict(length=70, height=48, girth=22, neck=16, neck_up=35, head=26, body=NAVY, belly=CREAM, snout=CREAM, ears=NAVY, tail=34, tail_col=CREAM)


def _staff(m, h, side):
    m.box(WOOD, (h[0], h[1] - 4, h[2] + 28), (5, 5, 175))
    m.box(WOOD, (h[0], h[1] - 12, h[2] + 113), (5, 20, 5))


def sheep_flock(sc):
    """A flock on a grassy swell, 18 x 7.6 m: four graze, one looks up, a lamb hops, the dog runs the flock's length along the front
    and comes round, the shepherd waves."""
    g = sc.still()
    g.box(SAGE, (0, 0, -55), (1800, 760, 190))                       # the swell: 0.4 m, then 0.8 m
    g.box(SAGE, (60, 40, 60), (1250, 480, 40))
    g.box(SAGE_D, (0, -382, 10), (1804, 8, 24))
    tuft(g, SAGE_D, -760, -60, 40)
    tuft(g, DRYG, 780, -220, 40)
    tuft(g, DRYG, -330, 250, 80)
    for k, (x, y) in enumerate(((-640, -150), (560, -250), (820, 60), (-120, -250))):
        g.box((MAG, GOLD, CREAM, GOLD)[k], (x, y, 46), (12, 12, 12))
    for k, (x, y, yaw, up, gz) in enumerate(((-420, 60, 100, -28, 10), (-200, 170, 250, -30, 12), (-20, -60, 80, 12, 0), (300, 60, 110, -30, 11))):
        quad(sc, x, y, 80, yaw, 1.3, SHEEP, neck_up=up, graze=gz, wag=20 if k % 2 else 0, rate=0.4 + 0.07 * k, phase=0.19 * k)
    quad(sc, -720, 180, 40, 60, 1.3, dict(SHEEP, body=WIRE, snout=CREAM), neck_up=-28, graze=10, rate=0.5, phase=0.6)
    quad(sc, 770, 170, 40, 200, 0.95, dict(SHEEP, head=36), bob=12, wag=30, rate=1.4, phase=0.3)
    quad(sc, 0, -300, 40, 90, 1.5, DOG, stride=30, wag=20, rate=2.4, travel=(1600, 280))
    P.figure(sc, 380, 170, 80, yaw=15, s=1.1, skin=P.SKINS[3], top=WHITE, bottom=BLUE, shoes=P.SHOES[3], hair='short', hair_col=P.HAIRS[1],
             hat='cowboy', hat_col=CREAM, sleeves='long', arm_l='wave', arm_r='down', hold_r=_staff, detail=1, rate=0.8, swing_l=18, phase=0.2)


# ---------------------------------------------------------------------------------------------- 3 jackrabbits
def hare(sc, x, y, z, yaw, s=1.5, sit=False, flick=18.0, nod=0.0, **common):
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 20))
    if sit:
        tilt_box(b, DRYG, (-8, 0, 12), (6, 0, 54), 20, 22)
        b.box(DRYG, (-10, 0, 14), (30, 26, 26))
        b.box(CREAM, (10, 0, 30), (6, 14, 30))
        hx, hz = 10.0, 62.0
    else:
        tilt_box(b, DRYG, (-22, 0, 18), (16, 0, 26), 20, 22)
        b.box(DRYG, (-16, 0, 18), (26, 24, 30))
        b.box(DRYG, (12, 0, 10), (8, 14, 20))
        hx, hz = 22.0, 34.0
    b.box(WHITE, (-31, 0, 20), (10, 10, 10))
    h = f.part(nod, (hx - 8, 0, hz - 6), 90.0) if nod else b
    h.box(DRYG, (hx, 0, hz), (22, 16, 16))
    h.box(WIRE, (hx + 11, 0, hz - 1), (5, 7, 6))
    for side in (-1, 1):
        e = f.part(side * flick, (hx - 2, side * 5, hz + 7), P.UP, rate=common.get('rate', 0.0) or 0.8) if (flick and not nod) else h
        tilt_box(e, DRYG, (hx - 2, side * 5, hz + 7), (hx - 9, side * 9, hz + 36), 8, 5)
        tilt_box(e, WIRE, (hx - 9, side * 9, hz + 36), (hx - 11, side * 10, hz + 46), 8, 5)


def jackrabbits(sc):
    """Jackrabbits on a boulder top, 2 m up: a sentinel turns its ears, one crouches, one hops on the lower boulder,
    one nibbles at the foot."""
    g = sc.still()
    g.box(CONC, (0, 30, 40), (420, 340, 300), rot=(0, 12, 0))
    g.box(SAND, (-10, 30, 196), (330, 260, 14), rot=(0, 12, 0))      # its top: 2.03 m
    g.box(SANDST_L, (330, -40, 10), (260, 240, 200), rot=(0, -20, 0))  # the lower one: 1.1 m
    g.box(SANDST_D, (-290, 120, 10), (150, 150, 110), rot=(0, 30, 0))
    g.box(SAND, (150, -200, 0), (120, 100, 60), rot=(0, 40, 0))
    g.sphere(SAGE, (-300, -80, 26), 62, scale=(1.0, 1.0, 0.7), steps=(3, 6))
    g.sphere(SAGE_D, (470, 110, 20), 48, scale=(1.0, 1.0, 0.7), steps=(3, 6))
    hare(sc, -50, 10, 203, -70, sit=True, flick=22, rate=0.7, phase=0.1)
    hare(sc, 80, 90, 203, 200, flick=14, rate=0.5, phase=0.6)
    hare(sc, 330, -40, 110, -25, flick=0, bob=26, rate=1.3, phase=0.3)
    hare(sc, -230, -190, 0, 10, nod=16, rate=1.6, phase=0.8)


# ---------------------------------------------------------------------------------------------- 4 roadrunners
def runner(sc, x, y, z, yaw, s=1.2, stride=0.0, flick=16.0, **common):
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 34))
    tilt_box(b, BARK_D, (-20, 0, 34), (18, 0, 44), 16, 18)
    b.box(CREAM, (8, 0, 31), (22, 14, 10))
    tilt_box(b, BARK_D, (16, 0, 44), (30, 0, 58), 8, 8)
    b.box(BARK_D, (36, 0, 62), (18, 10, 10))
    b.box(POOL, (35, 0, 63), (7, 11.5, 5))
    tilt_box(b, NAVY, (30, 0, 66), (21, 0, 80), 6, 6)
    tilt_box(b, WIRE, (44, 0, 62), (62, 0, 60), 5, 5)
    t = f.part(flick, (-20, 0, 36), 90.0)
    tilt_box(t, NAVY, (-20, 0, 36), (-60, 0, 54), 9, 6)
    tilt_box(t, WHITE, (-60, 0, 54), (-68, 0, 58), 9, 6)
    for side in (-1, 1):
        leg = f.part(side * stride, (0, side * 5, 30), 90.0) if stride else b
        leg.box(GREY, (0, side * 5, 15), (5, 5, 32))
        leg.box(GREY, (5, side * 5, 2), (14, 6, 4))


def roadrunners(sc):
    """Roadrunners on a 15 m sandstone ledge, 2.1 m up: two sprint its length (one each way) and come round, a third
    stands by a prickly pear, tail flicking."""
    g = sc.still()
    g.box(SANDST_L, (0, 20, 45), (1500, 300, 310))
    g.box(SANDST_D, (0, -132, 70), (1504, 8, 30))
    g.box(SAND, (0, -132, 150), (1504, 8, 22))
    g.box(CREAM, (0, 20, 206), (1520, 320, 12))                      # the cap: 2.12 m
    g.box(SANDST_D, (-780, 60, 30), (160, 220, 200), rot=(0, 20, 0))
    g.box(SANDST_L, (790, -20, 10), (150, 200, 150), rot=(0, -25, 0))
    for k, (dx, dz, r) in enumerate(((0, 0, 0), (26, 46, 20), (-22, 50, -24), (40, 96, 30))):   # the prickly pear
        g.box(SAGE, (690 + dx, 60, 246 + dz), (58, 14, 66), rot=(0, 0, r))
        g.box(MAG, (690 + dx + (10 if r >= 0 else -10), 60, 284 + dz), (10, 10, 10))
    runner(sc, 0, -70, 212, 0, stride=38, flick=6, rate=3.0, phase=0.0, **along(0, -70, 1200, 0.36))
    runner(sc, 150, 70, 212, 180, stride=38, flick=6, rate=3.0, phase=0.4, **along(150, 70, 1200, -0.30))
    runner(sc, -690, 40, 212, -35, flick=22, rate=0.9, phase=0.2)


# ---------------------------------------------------------------------------------------------- 5 kit foxes
FOX = dict(length=62, height=44, girth=22, neck=16, neck_up=40, head=34, body=WARM, belly=CREAM, snout=CREAM, ears=WARM, tail=50, tail_col=WARM)


def cub(sc, x, y, z, yaw, s=1.3, **common):
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 14))
    b.box(WARM, (0, 0, 14), (34, 16, 16))
    b.box(WARM, (20, 0, 24), (16, 16, 14))
    b.box(CREAM, (30, 0, 21), (8, 8, 7))
    for side in (-1, 1):
        b.box(WARM, (18, side * 6, 37), (5, 6, 13))
    tilt_box(b, WARM, (-16, 0, 16), (-34, 0, 26), 8, 8)
    b.box(CREAM, (-37, 0, 28), (8, 8, 8))


def fox_den(sc):
    """A kit fox den in a pale bank, 12 x 6.8 m: the dog fox up on the bank swivels its big ears, the vixen noses about
    on the apron, three cubs bob at the den's mouth."""
    g = sc.still()
    g.box(CONC, (0, 60, 0), (1000, 420, 300))                        # the bank: 1.5 m
    g.box(DRYG, (0, 60, 156), (1000, 420, 12))
    g.box(SAND, (0, -200, -40), (1200, 260, 160))                    # the apron: 0.4 m
    g.box(NAVY, (-150, -152, 95), (170, 8, 110))                     # the den's mouth, and a second hole
    g.box(CREAM, (-150, -158, 158), (230, 20, 16))
    g.box(NAVY, (270, -152, 85), (110, 8, 90))
    g.cyl(SAND, (-150, -265, 40), 120, 36, steps=6, top_r=44)        # the spoil heap
    g.box(SANDST_L, (440, -230, 60), (110, 90, 70), rot=(0, 20, 0))
    tuft(g, SAGE_D, -430, 120, 162)
    tuft(g, DRYG, 400, 40, 162)
    tuft(g, DRYG, -520, -250, 40)
    quad(sc, 180, 40, 162, -50, 1.3, FOX, (WARM, 30, 13), flick=24, wag=12, rate=0.6, phase=0.2)
    quad(sc, 330, -220, 40, 60, 1.25, FOX, (WARM, 30, 13), neck_up=5, graze=8, wag=16, rate=0.7, phase=0.5)
    cub(sc, -60, -215, 40, -20, bob=12, rate=1.4, phase=0.0)
    cub(sc, -150, -265, 76, 200, bob=16, rate=1.7, phase=0.35)
    cub(sc, -290, -230, 40, 30, bob=10, rate=1.2, phase=0.7)


# ---------------------------------------------------------------------------------------------- 6 tortoises
def tortoise(sc, x, y, z, yaw, s=1.4, nod=12.0, **common):
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 20))
    b.sphere(SAGE_D, (0, 0, 26), 30, scale=(1.3, 1.0, 0.7), steps=(3, 6))
    b.box(DRYG, (0, 0, 44), (34, 26, 8))
    for sx in (-1, 1):
        for sy in (-1, 1):
            b.box(CONC, (sx * 22, sy * 20, 9), (12, 12, 18))
    h = f.part(nod, (32, 0, 22), 90.0)
    tilt_box(h, CONC, (30, 0, 22), (48, 0, 30), 9, 9)
    h.box(CONC, (54, 0, 32), (14, 12, 11))


def tortoises(sc):
    """Desert tortoises on rock slabs, 1.5 m up, a propped slab behind them: their heads nod, each on its own clock."""
    g = sc.still()
    g.box(SANDST_L, (0, 20, 10), (780, 480, 220))                    # the plinth: 1.2 m
    g.box(CREAM, (-60, -10, 134), (640, 380, 28))                    # the slab: 1.48 m
    g.box(SANDST_D, (0, -222, 40), (784, 8, 26))
    g.box(GREY, (250, 150, 200), (330, 300, 26), rot=(0, 0, 24))     # the propped slab
    g.box(SANDST_D, (300, 220, 150), (120, 110, 100))
    g.cyl(SAGE_D, (-330, 190, 120), 30, 52, steps=8)                 # a barrel cactus in flower
    g.box(MAG, (-330, 190, 178), (16, 16, 10))
    tuft(g, DRYG, 340, -150, 120)
    tortoise(sc, -230, -60, 148, -25, rate=0.35, phase=0.0)
    tortoise(sc, 40, 30, 148, 160, nod=10, rate=0.28, phase=0.4)
    tortoise(sc, 150, -110, 148, -80, 1.15, nod=14, rate=0.42, phase=0.7)
    tortoise(sc, -60, -120, 148, 20, 0.7, nod=16, rate=0.6, phase=0.2)


# ---------------------------------------------------------------------------------------------- 7 quail
def quail(sc, x, y, z, yaw, s=1.6, knot=28.0, **common):
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 20))
    b.box(SHEET, (0, 0, 20), (26, 18, 18))
    b.box(CREAM, (3, 0, 12), (20, 16, 8))
    b.box(NAVY, (16, 0, 32), (12, 11, 11))
    b.box(WHITE, (16, 0, 37), (13, 12, 4))
    b.box(WIRE, (24, 0, 31), (6, 5, 5))
    tilt_box(b, SHEET, (-12, 0, 22), (-25, 0, 16), 12, 6)
    for side in (-1, 1):
        b.box(WIRE, (0, side * 4, 5), (4, 4, 12))
    k = f.part(knot, (18, 0, 38), 90.0)
    tilt_box(k, NAVY, (18, 0, 38), (26, 0, 50), 5, 5)
    k.box(NAVY, (27, 0, 51), (8, 6, 7))


def quail_wall(sc):
    """A quail covey along the top of a whitewashed garden wall, 10 m long and 2.1 m up: topknots bob, and one hen
    hurries the wall's length with three chicks behind her."""
    g = sc.still()
    g.box(WHITE, (0, 0, 50), (1000, 56, 300))
    g.box(TEAL, (0, 0, 207), (1030, 76, 14))                         # the cap: 2.14 m
    for sx in (-1, 1):
        g.box(WHITE, (sx * 530, 0, 70), (110, 110, 340))
        g.box(TEAL, (sx * 530, 0, 246), (130, 130, 12))
        g.cyl(MAG, (sx * 530, 0, 252), 28, 40, steps=6, top_r=34)
        g.cyl(SAGE_D, (sx * 530, 0, 292), 14, 56, steps=6)
    g.box(NAVY, (-250, -30, 70), (150, 8, 220))                      # a gate in an arched niche
    g.box(WOOD, (-250, -36, 60), (120, 8, 180))
    g.box(CREAM, (-250, -34, 188), (190, 12, 18))
    g.box(RED, (-140, -36, 150), (14, 12, 62))                       # a string of chillies
    for k in range(5):
        g.box(WOOD, (80 + k * 90, -40, 165), (14, 30, 14))           # beam ends
    for k, (x, yaw, kn) in enumerate(((-390, 10, 30), (-310, 170, 24), (-110, -30, 32), (60, 200, 26), (230, 0, 30), (330, 150, 22))):
        quail(sc, x, 19, 214, yaw, knot=kn, rate=0.9 + 0.13 * k, phase=0.17 * k)
    quail(sc, 0, -19, 214, 0, knot=14, rate=2.0, **along(0, -19, 900, 0.13))
    for k in range(3):
        c = Fr(sc, -50 - 34 * k, -19, 214, 0, 1.6, bob=3, rate=4.0, phase=0.3 * k, **along(-50 - 34 * k, -19, 900, 0.13))
        m = c.part(pivot=(0, 0, 8))
        m.box(GOLD, (0, 0, 8), (12, 9, 10))
        m.box(GOLD, (7, 0, 15), (7, 7, 7))


# ---------------------------------------------------------------------------------------------- 8 peacocks
def peabird(sc, x, y, z, yaw, s=1.5, body=BLUE, neck=TEAL, train=None, eye=GOLD, nod=0.0, **common):
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 30))
    b.box(body, (0, 0, 30), (40, 22, 24))
    for side in (-1, 1):
        b.box(GREY, (2, side * 5, 9), (4, 4, 20))
    if train:
        tilt_box(b, train, (-16, 0, 32), (-170, 0, -18), 26, 8)
        for k in range(3):
            b.box(eye, (-70 - 40 * k, 0, 16 - 13 * k), (16, 28, 10))
    h = f.part(nod, (14, 0, 38), 90.0) if nod else b
    tilt_box(h, neck, (14, 0, 38), (22, 0, 72), 9, 9)
    h.box(neck, (25, 0, 78), (13, 10, 10))
    h.box(CREAM, (34, 0, 77), (7, 5, 5))
    h.box(eye, (22, 0, 90), (5, 10, 13))


def peacocks(sc):
    """Peacocks on a marble balustrade, 11.4 m long: the one in the middle fans its tail open and shut (nine feathers,
    3.7 m to their tips), one trails its train from each pier (a white one on the right), a peahen pecks on the rail."""
    g = sc.still()
    g.box(WHITE, (0, 0, -5), (1100, 80, 190))                        # the plinth: 0.9 m
    for k in range(10):
        g.box(WHITE, (-450 + k * 100, 0, 125), (26, 26, 70))
    g.box(WHITE, (0, 0, 170), (1140, 90, 20))                        # the rail: 1.8 m
    g.box(GOLD, (0, -47, 170), (1140, 6, 8))
    for sx in (-1, 1):
        g.box(WHITE, (sx * 610, 0, 70), (120, 120, 340))             # the piers: 2.4 m
        g.box(GOLD, (sx * 610, 0, 246), (140, 140, 12))
    g.box(CRIM, (260, 0, 182), (120, 96, 6))                         # a carpet thrown over the rail
    g.box(CRIM, (260, -48, 150), (120, 6, 64))
    g.box(GOLD, (260, -50, 122), (120, 6, 8))
    peabird(sc, 0, 0, 180, -90, rate=0.5, phase=0.1)
    p0 = (0.0, 22.0, 180.0 + 34.0 * 1.5)                              # the fan stands behind it, facing the road
    for k in range(-4, 5):
        th = math.radians(16.0 * k)
        tip = (p0[0] + 170.0 * math.sin(th), p0[1], p0[2] + 170.0 * math.cos(th))
        m = sc.part(P.motion(swing=0.55 * 16.0 * k, pivot=p0, axis=90.0, rate=0.22))
        tilt_box(m, SAGE_D, p0, tip, 7, 22)
        m.box(GOLD, tip, (30, 9, 30))
        m.box(POOL, tip, (14, 11, 14))
    peabird(sc, -610, 0, 252, 180, train=SAGE_D, nod=9, rate=0.4, phase=0.3)
    peabird(sc, 610, 0, 252, 0, body=WHITE, neck=WHITE, train=WHITE, eye=GREY, nod=7, rate=0.33, phase=0.6)
    peabird(sc, -290, 0, 180, -60, 1.3, body=BARK, neck=CREAM, eye=BARK, nod=24, rate=1.1, phase=0.5)


# ---------------------------------------------------------------------------------------------- 9 mule deer
DEER = dict(length=125, height=105, girth=44, neck=46, neck_up=52, head=34, body=BARK, belly=CREAM, snout=WIRE, ears=BARK, tail=16, tail_col=WIRE)
DEER_EARS = (BARK, 26, 13)


def mule_deer(sc):
    """Mule deer on a grassy rise, 17 x 7 m: two does graze, the buck stands watch with his ears turning, one doe walks
    the front of the rise and comes round, a fawn hops."""
    g = sc.still()
    g.box(SANDST_L, (0, 0, -45), (1700, 700, 210))                   # the rise: 0.6 m, then 1.3 m
    g.box(DRYG, (-100, 60, 90), (1000, 440, 80))
    g.box(SAND, (0, -352, 20), (1704, 8, 24))
    g.sphere(SAGE, (560, 150, 80), 70, scale=(1.0, 1.0, 0.7), steps=(3, 6))
    g.sphere(SAGE_D, (-700, 200, 76), 56, scale=(1.0, 1.0, 0.7), steps=(3, 6))
    rod(g, BARK_D, (700, 240, 40), (720, 250, 300), 13, steps=6)     # a juniper snag
    rod(g, BARK_D, (716, 248, 220), (800, 230, 310), 8, steps=6)
    rod(g, BARK_D, (712, 246, 170), (640, 270, 250), 8, steps=6)
    tuft(g, DRYG, 520, -240, 60)
    tuft(g, SAGE_D, -420, -120, 130)
    quad(sc, -150, 90, 130, -80, 1.3, dict(DEER, horns=CREAM), DEER_EARS, flick=22, wag=14, rate=0.6, phase=0.1)
    L, H, G, NK, HD = 125.0, 105.0, 44.0, 46.0, 34.0                  # the buck's tines (where quadruped puts the horns)
    a = math.radians(52.0)
    n1y, n1z = -L / 2.0 + G * 0.25 - math.cos(a) * NK, H - G * 0.35 + math.sin(a) * NK
    t = sc.still((-150, 90, 130), -80, 1.3)
    for side in (-1, 1):
        tilt_box(t, CREAM, (side * G * 0.3, n1y + HD * 0.03, n1z + HD * 0.56), (side * G * 0.36, n1y - HD * 0.4, n1z + HD * 1.0), HD * 0.13, HD * 0.13)
        tilt_box(t, CREAM, (side * G * 0.42, n1y + HD * 0.1, n1z + HD * 0.8), (side * G * 0.5, n1y + HD * 0.45, n1z + HD * 1.2), HD * 0.13, HD * 0.13)
    quad(sc, -430, 160, 130, 110, 1.2, DEER, DEER_EARS, neck_up=-28, graze=9, wag=20, rate=0.45, phase=0.5)
    quad(sc, 200, 120, 130, 250, 1.2, DEER, DEER_EARS, neck_up=-30, graze=11, rate=0.5, phase=0.8)
    quad(sc, 560, -60, 60, 40, 0.95, DEER, DEER_EARS, bob=8, wag=30, rate=1.5, phase=0.3)
    quad(sc, 0, -260, 60, 90, 1.2, DEER, DEER_EARS, stride=16, wag=10, rate=1.0, phase=0.4, travel=(1400, 85))


# ---------------------------------------------------------------------------------------------- 10 ducks
def duck(sc, x, y, z, yaw, s=1.5, head=SAGE_D, body=CREAM, preen=0.0, **common):
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 9))
    b.box(body, (0, 0, 9), (38, 22, 18))
    tilt_box(b, body, (-18, 0, 14), (-31, 0, 24), 12, 6)
    b.box(BLUE, (-3, 0, 14), (16, 24, 6))
    h = f.part(preen, (12, 0, 18), 90.0) if preen else b
    h.box(body if head == BARK else WHITE, (14, 0, 24), (9, 9, 16))
    h.box(head, (17, 0, 36), (15, 13, 12))
    h.box(GOLD, (28, 0, 34), (10, 9, 5))


def duckling(sc, x, y, z, yaw, s=1.5, **common):
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 5))
    b.box(GOLD, (0, 0, 5), (16, 11, 10))
    b.box(GOLD, (8, 0, 13), (9, 8, 8))


def duck_tank(sc):
    """A round steel stock tank, 5.6 m across with its rim 2.1 m up: three ducks and a brood of ducklings paddle a slow
    circle, one preens, one dabbles tail up."""
    g = sc.still()
    g.cyl(SHEET, (0, 0, -100), 270, 305, steps=12)
    g.cyl(GREY, (0, 0, 40), 276, 12, steps=12)
    g.cyl(GREY, (0, 0, 125), 276, 12, steps=12)
    g.cyl(GREY, (0, 0, 198), 282, 14, steps=12)                      # the rim: 2.12 m
    g.cyl(POOL, (0, 0, 208), 262, 8, steps=12)                       # the water: 2.16 m
    for sx in (-1, 1):                                               # a ladder
        tilt_box(g, WOOD, (-120 + sx * 22, -345, -20), (-120 + sx * 22, -284, 214), 7, 7)
    for k in range(4):
        g.box(WOOD, (-120, -330 + k * 13, 30 + k * 50), (50, 7, 7))
    rod(g, GREY, (318, 70, -100), (318, 70, 300), 10, steps=6)       # the standpipe and its spout
    rod(g, GREY, (318, 70, 292), (210, 46, 292), 8, steps=6)
    g.box(POOL, (212, 46, 254), (6, 6, 76))
    for k, (x, y) in enumerate(((-330, -150), (-360, -100), (350, -160))):   # reeds at its foot
        g.box(SAGE_D, (x, y, 40), (8, 8, 150), rot=(0, 0, 8 - 8 * k))
        g.box(BARK_D, (x, y, 122), (11, 11, 34), rot=(0, 0, 8 - 8 * k))
    R = 170.0
    for k, (phi, hd, bd) in enumerate(((0.0, SAGE_D, CREAM), (118.0, BARK, BARK), (212.0, SAGE_D, GREY))):
        a = math.radians(phi)
        duck(sc, R * math.cos(a), R * math.sin(a), 216, phi + 90.0, head=hd, body=bd, bob=-2.0, rate=0.5, phase=0.3 * k, **ring(0.045))
    for k in range(3):                                               # the brood, behind the hen
        a = math.radians(118.0 - 13.0 - 10.0 * k)
        duckling(sc, R * math.cos(a), R * math.sin(a), 216, 118.0 - 13.0 - 10.0 * k + 90.0, bob=-2.0, rate=0.9, phase=0.25 * k, **ring(0.045))
    duck(sc, 30, -50, 216, 200, preen=34, rate=0.7, phase=0.2)
    d = Fr(sc, -60, 50, 216, 40, 1.5, bob=-5.0, rate=0.8, phase=0.5).part(pivot=(0, 0, 10))
    d.box(CREAM, (0, 0, 10), (20, 22, 34))                           # the dabbler: tail up
    d.box(GREY, (0, 0, 31), (10, 12, 10))
    for side in (-1, 1):
        d.box(GOLD, (-8, side * 6, 20), (6, 6, 8))


# ---------------------------------------------------------------------------------------------- 11 brine pool
def avocet(sc, x, y, z, yaw, s=1.5, feed=True, sweep=28.0, **common):
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 54))
    tilt_box(b, WHITE, (-18, 0, 52), (16, 0, 56), 14, 16)
    b.box(NAVY, (-4, 0, 62), (22, 15, 4))
    tilt_box(b, NAVY, (-18, 0, 54), (-30, 0, 50), 10, 6)
    for side in (-1, 1):
        b.box(SHEET, (0, side * 4, 25), (4, 4, 50))
    h = f.part(sweep, (16, 0, 56), P.UP)
    if feed:
        tilt_box(h, SANDST_L, (16, 0, 56), (30, 0, 36), 6, 6)
        h.box(SANDST_L, (33, 0, 32), (12, 9, 9))
        tilt_box(h, NAVY, (38, 0, 30), (62, 0, 24), 4, 4)
    else:
        tilt_box(h, SANDST_L, (16, 0, 56), (24, 0, 82), 6, 6)
        h.box(SANDST_L, (27, 0, 86), (12, 9, 9))
        tilt_box(h, NAVY, (32, 0, 86), (56, 0, 90), 4, 4)


def brine_pool(sc):
    """A brine pool on a salt crust, 15 x 9 m: five avocets sweep their bills through the shallows, a white pelican
    floats and nods its bill, another stands on a salt hummock and stretches its wings."""
    g = sc.still()
    g.plan(CREAM, [(760, 40), (600, 330), (240, 450), (-260, 420), (-640, 300), (-760, -20), (-560, -350), (-120, -450), (360, -420), (680, -260)], -100, 10)
    g.plan(STEEL, ellipse(560, 300, 12), 10, 16)                     # the shallows, then the deep
    g.plan(POOL, ellipse(380, 190, 12, -40, 10), 16, 21)
    for k, (x, y) in enumerate(((640, 60), (-650, 150), (300, -370))):
        g.cyl(WHITE, (x, y, 10), 42, 54, steps=6, top_r=8)           # salt cones
    for k, (x, y) in enumerate(((-420, 330), (520, 250), (-600, -200), (60, 390), (560, -230))):
        g.box(SAGE_D, (x, y, 22), (40, 30, 24), rot=(0, 30 * k, 0))  # pickleweed, red at the tips
        g.box(MAG, (x, y, 37), (22, 16, 6), rot=(0, 30 * k, 0))
    for k, (x, y, yaw, fd) in enumerate(((-380, -150, 20, True), (-250, -210, 35, True), (120, -230, 160, True), (330, -110, 200, False), (-460, 60, -60, True))):
        avocet(sc, x, y, 16, yaw, feed=fd, sweep=28 if fd else 14, rate=0.8 + 0.1 * k, phase=0.21 * k)
    f = Fr(sc, -40, 20, 21, -20, 1.5, bob=-2.0, rate=0.4, phase=0.1)   # the pelican afloat
    b = f.part(pivot=(0, 0, 16))
    b.box(WHITE, (0, 0, 16), (60, 34, 30))
    b.box(NAVY, (-18, 0, 30), (26, 36, 8))
    tilt_box(b, WHITE, (-28, 0, 24), (-42, 0, 34), 20, 8)
    b.box(WHITE, (26, 0, 42), (12, 12, 36))
    b.box(WHITE, (30, 0, 62), (18, 14, 14))
    h = f.part(10.0, (38, 0, 58), 90.0)
    tilt_box(h, GOLD, (38, 0, 58), (86, 0, 44), 10, 8)
    tilt_box(h, WARM, (40, 0, 50), (76, 0, 40), 8, 12)
    g = sc.still()
    g.box(WHITE, (330, 150, 34), (110, 90, 52))                      # the hummock: 0.6 m
    P.bird(sc, 330, 150, 82, heading=-70, span=300, body=WHITE, wing=GREY, beak=GOLD, rate=0.45, flap=20, phase=0.3)


# ---------------------------------------------------------------------------------------------- 12 swans
def swan(sc, x, y, z, yaw, s=1.4, preen=0.0, col=WHITE, **common):
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 14))
    b.box(col, (0, 0, 14), (64, 34, 26))
    b.box(col, (-6, 0, 32), (44, 30, 14))
    tilt_box(b, col, (-30, 0, 20), (-47, 0, 35), 16, 8)
    h = f.part(preen, (26, 0, 24), 90.0) if preen else b
    tilt_box(h, col, (26, 0, 24), (38, 0, 56), 9, 9)
    tilt_box(h, col, (38, 0, 56), (30, 0, 86), 9, 9)
    h.box(col, (36, 0, 90), (16, 10, 10))
    tilt_box(h, WARM, (42, 0, 89), (58, 0, 82), 7, 6)
    h.box(NAVY, (43, 0, 96), (6, 7, 5))


def swan_lagoon(sc):
    """A raised marble lagoon, 14.8 x 5.8 m, its water 1.7 m up: two swans glide its length one way (three cygnets behind
    the first) and one the other, and come round; a fourth preens at the end."""
    g = sc.still()
    g.plan(WHITE, stadium(720, 270), -100, 150)
    g.plan(CRIM, stadium(726, 276), 70, 92)
    g.plan(GOLD, stadium(740, 290), 150, 164)                        # the rim
    g.plan(POOL, stadium(690, 240), 160, 168)                        # the water: 1.68 m
    for k, x in enumerate((-300, 0, 260)):
        g.cyl(SAGE, (x, 0, 165), 34, 6, steps=6)                     # lily pads
        g.box(MAG if k != 1 else WHITE, (x + 8, 6, 176), (12, 12, 10))
    swan(sc, 200, 90, 168, 0, bob=-1.5, rate=0.4, phase=0.0, **along(200, 90, 1000, 0.04))
    for k in range(3):
        c = Fr(sc, 110 - 34 * k, 90 + (8 if k % 2 else -8), 168, 0, 1.4, bob=-1.5, rate=0.7, phase=0.3 * k, **along(110 - 34 * k, 90, 1000, 0.04))
        m = c.part(pivot=(0, 0, 6))
        m.box(GREY, (0, 0, 6), (18, 12, 12))
        m.box(GREY, (9, 0, 16), (8, 7, 10))
    swan(sc, -260, 90, 168, 0, bob=-1.5, rate=0.4, phase=0.5, **along(-260, 90, 1000, 0.04))
    swan(sc, 0, -90, 168, 180, bob=-1.5, rate=0.45, phase=0.3, **along(0, -90, 1000, -0.035))
    swan(sc, -590, 0, 168, 30, preen=16, rate=0.5, phase=0.7)


# ---------------------------------------------------------------------------------------------- 13 pelicans in the sky
def pelican_line(sc):
    """Seven white pelicans gliding in a line ALONG the road (the mesh's X), their slow wing beat running down the line
    like a wave: they travel 100 m as built (160 m at the 1.6 they are placed at) and come round. far=True. z = 0 is hung in the sky by a Far rule's negative drop."""
    for k in range(7):
        x = 600.0 - 200.0 * k
        P.bird(sc, x, 45.0 * (k % 2) - 22.0, 300.0 + 26.0 * math.sin(k * 1.3), heading=0.0, span=300, body=WHITE, wing=GREY, beak=GOLD,
               phase=0.11 * k, rate=0.8, flap=15, bob=-28.0, travel=(10000, 600))


# ---------------------------------------------------------------------------------------------- 14 doves
def dove(sc, x, y, z, heading, s=2.0, flap=38.0, **common):
    f = Fr(sc, x, y, z, heading, s, **common)
    b = f.part(pivot=(0, 0, 0))
    tilt_box(b, WHITE, (-12, 0, 0), (12, 0, 2), 10, 10)
    b.box(WHITE, (15, 0, 5), (9, 8, 8))
    tilt_box(b, GREY, (-12, 0, 0), (-25, 0, -2), 12, 4)
    for side in (-1, 1):
        w = f.part(side * flap, (0, 0, 2), 0.0)
        w.box(WHITE, (-2, side * 17, 3), (15, 28, 4))


def dovecote(sc):
    """A white dovecote, 8.4 m tall under a crimson cone, with doves at its pigeonholes and eight white doves in a tight
    wheel over it: two rings, 5.2 and 8.2 m from the cote, 8 to 10 m up."""
    g = sc.still()
    g.box(CONC, (0, 0, -40), (420, 420, 120))
    g.cyl(WHITE, (0, 0, 20), 150, 600, steps=8)
    g.cyl(GOLD, (0, 0, 612), 196, 14, steps=8)
    g.cyl(CRIM, (0, 0, 626), 190, 220, steps=8, top_r=12)
    g.sphere(GOLD, (0, 0, 858), 20, steps=(3, 6))
    g.box(CRIM, (0, -142, 95), (90, 10, 150))                        # the door
    for row in range(3):
        z = 300 + row * 100
        g.box(CREAM, (0, -152, z - 30), (250, 34, 8))                # a landing board
        for k in (-1, 0, 1):
            g.box(NAVY, (k * 80, -140, z + 6), (26, 10, 62))               # tall slots, not dots
    for k, (x, z, yaw) in enumerate(((-100, 274, -70), (30, 374, -110), (110, 474, -60), (-40, 474, -120))):
        f = Fr(sc, x, -156, z, yaw, 2.0, bob=3.0, rate=1.3 + 0.2 * k, phase=0.23 * k)
        m = f.part(pivot=(0, 0, 6))
        m.box(WHITE, (0, 0, 7), (22, 11, 11))
        m.box(WHITE, (10, 0, 15), (9, 8, 9))
        m.box(GREY, (-13, 0, 6), (8, 9, 4))
    for k in range(8):
        R, z, rps = ((520.0, 820.0, 0.11), (820.0, 930.0, 0.085))[k % 2]
        phi = (0.0, 40.0, 95.0, 150.0, 170.0, 215.0, 265.0, 300.0)[k]
        a = math.radians(phi)
        dove(sc, R * math.cos(a), R * math.sin(a), z + 12.0 * (k % 3), phi + 90.0, rate=3.0, phase=0.13 * k, **ring(rps))


# ---------------------------------------------------------------------------------------------- 15 barn owls
def owls(sc):
    """Three pale barn owls quartering ALONG the road (the mesh's X): two one way, one back, 60 m as built (150 m at the 2.5 they are placed at) and round again, on a
    slow wing beat, rising and sinking. far=True. Hung in the sky by a Far rule's negative drop."""
    for k, (x, y, z, hd, speed) in enumerate(((300.0, -200.0, 200.0, 0.0, 260.0), (-500.0, 60.0, 420.0, 0.0, 230.0), (0.0, 320.0, 60.0, 180.0, -210.0))):
        kw = dict(phase=0.31 * k, rate=1.1, bob=-60.0)
        P.bird(sc, x, y, z, heading=hd, span=240, body=WHITE, wing=SAND, beak=WARM, flap=26, travel=(6000, speed), **kw)
        d = 1.0 if hd == 0.0 else -1.0                                # the heart of a face, and the feet
        m = sc.part(P.motion(pivot=(x, y, z), base=(x, y), travel=(6000.0, speed / 6000.0), **kw))
        m.base = None
        m.box(CREAM, (x + d * 66.0, y, z + 14.0), (8, 30, 28))
        for side in (-1, 1):
            m.box(NAVY, (x + d * 71.0, y + side * 7.0, z + 17.0), (4, 6, 6))
            m.box(WARM, (x - d * 10.0, y + side * 7.0, z - 22.0), (22, 5, 5))


# ---------------------------------------------------------------------------------------------- 16 flamingos
def flamingo(sc, x, y, z, yaw, s=1.15, feed=False, one_leg=False, swing=8.0, **common):
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 100))
    b.box(PINK, (0, 0, 100), (44, 24, 26))
    tilt_box(b, MAG, (-20, 0, 104), (-37, 0, 96), 14, 8)
    b.box(MAG, (-4, 0, 114), (30, 26, 5))
    b.box(MAG, (2, -5, 45), (4, 4, 92))
    if one_leg:
        tilt_box(b, MAG, (2, 5, 88), (-20, 5, 70), 4, 4)
    else:
        b.box(MAG, (2, 5, 45), (4, 4, 92))
    if feed:
        h = f.part(swing, (20, 0, 104), P.UP)
        tilt_box(h, PINK, (20, 0, 104), (46, 0, 68), 7, 7)
        tilt_box(h, PINK, (46, 0, 68), (50, 0, 22), 7, 7)
        h.box(PINK, (52, 0, 16), (12, 10, 12))
        tilt_box(h, CREAM, (54, 0, 10), (44, 0, 3), 7, 6)
    else:
        h = f.part(swing, (20, 0, 104), 90.0)
        tilt_box(h, PINK, (20, 0, 104), (34, 0, 140), 7, 7)
        tilt_box(h, PINK, (34, 0, 140), (26, 0, 172), 7, 7)
        h.box(PINK, (32, 0, 176), (14, 10, 10))
        tilt_box(h, CREAM, (38, 0, 174), (48, 0, 164), 7, 6)
        h.box(NAVY, (50, 0, 160), (6, 6, 7))


def flamingo_pool(sc):
    """A raised oval pool, 11.5 x 6.9 m, its water 0.8 m up, with six flamingos: three sweep their bills through the
    water, three stand tall (one on one leg) and nod; reeds on a sand island."""
    g = sc.still()
    g.plan(CREAM, ellipse(560, 330, 12), -100, 66)
    g.plan(GOLD, ellipse(575, 345, 12), 60, 72)
    g.plan(POOL, ellipse(530, 300, 12), 68, 78)                      # the water: 0.78 m
    g.cyl(SAND, (-220, 80, 74), 110, 26, steps=8, top_r=80)
    for k, (dx, dy, r) in enumerate(((-30, 0, -8), (0, 20, 0), (30, -10, 10), (10, 40, 6))):
        g.box(SAGE_D, (-220 + dx, 80 + dy, 160), (9, 9, 130), rot=(0, 0, r))
        g.box(BARK_D, (-220 + dx - r * 1.6, 80 + dy, 232), (12, 12, 30), rot=(0, 0, r))
    flamingo(sc, 150, -120, 78, -20, feed=True, swing=24, rate=0.5, phase=0.0)
    flamingo(sc, 290, 20, 78, 200, feed=True, swing=20, rate=0.6, phase=0.4)
    flamingo(sc, -60, -170, 78, 170, feed=True, swing=26, rate=0.45, phase=0.7)
    flamingo(sc, 30, 60, 78, -60, one_leg=True, swing=5, rate=0.3, phase=0.2)
    flamingo(sc, 380, -100, 78, -110, swing=9, rate=0.5, phase=0.5)
    flamingo(sc, -360, -90, 78, -40, 1.0, swing=10, rate=0.7, phase=0.9)


# ---------------------------------------------------------------------------------------------- 17 hens
def hen(sc, x, y, z, yaw, s=1.4, col=CREAM, peck=30.0, **common):
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 22))
    b.box(col, (0, 0, 22), (30, 20, 20))
    tilt_box(b, col, (-12, 0, 28), (-24, 0, 44), 14, 8)
    for side in (-1, 1):
        b.box(WARM, (0, side * 5, 7), (4, 4, 14))
    h = f.part(peck, (12, 0, 28), 90.0) if peck else b
    h.box(col, (15, 0, 36), (9, 9, 16))
    h.box(col, (18, 0, 46), (11, 10, 10))
    h.box(RED, (18, 0, 53), (8, 4, 5))
    h.box(WARM, (25, 0, 45), (6, 5, 4))
    return f


def hen_coop(sc):
    """A whitewashed hen house on stilts under a red roof, 3.6 m to the ridge, in a fenced run 8.5 m long: hens peck,
    one leads four chicks across the run and round again, the cockerel on the ridge beats his wings, the vane rocks."""
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(WOOD, (sx * 130, sy * 80, 5), (14, 14, 230))
    g.box(WHITE, (0, 0, 195), (320, 220, 150))                       # the house: 1.2 to 2.7 m
    g.box(NAVY, (-60, -112, 172), (60, 8, 84))
    g.box(TEAL, (80, -112, 212), (64, 8, 52))
    g.box(CREAM, (80, -116, 212), (8, 6, 52))
    g.side(RED, [(-190, 268), (190, 268), (0, 360)], -130, 130)
    tilt_box(g, WOOD, (-60, -114, 122), (-60, -330, 2), 50, 8)       # the ramp and its cleats
    for k in range(4):
        g.box(BARK_D, (-60, -150 - k * 46, 108 - k * 25.5), (54, 6, 6))
    g.box(WHITE, (190, 0, 160), (60, 180, 80))                       # nest boxes under a lid
    g.box(TEAL, (196, 0, 205), (76, 192, 10))
    for x in (-420, -140, 140, 420):                                 # the run
        g.box(WOOD, (x, -420, 35), (10, 10, 90))
    for z in (40, 70):
        g.box(WOOD, (0, -420, z), (850, 6, 8))
        for sx in (-1, 1):
            g.box(WOOD, (sx * 420, -265, z), (6, 310, 8))
    g.box(WOOD, (260, -300, 8), (120, 30, 16))                       # a feed trough
    g.box(DRYG, (260, -300, 18), (106, 20, 6))
    rod(g, GREY, (0, 90, 356), (0, 90, 424), 4, steps=6)             # the vane
    v = sc.part(P.motion(swing=28.0, pivot=(0.0, 90.0, 424.0), axis=P.UP, rate=0.25, phase=0.2))
    v.box(GOLD, (0, 90, 428), (96, 5, 8))
    v.box(GOLD, (-38, 90, 440), (26, 5, 24))
    for k, (x, y, yaw, col) in enumerate(((-300, -250, 20, CREAM), (-200, -350, 200, BARK), (220, -240, 30, GREY), (330, -370, 150, CREAM))):
        hen(sc, x, y, 0, yaw, col=col, peck=28 + 4 * k, rate=1.2 + 0.2 * k, phase=0.23 * k)
    hen(sc, 0, -190, 0, 0, col=BARK, peck=8, rate=2.0, **along(0, -190, 640, 0.06))
    for k in range(4):
        c = Fr(sc, -44 - 26 * k, -190 + (7 if k % 2 else -7), 0, 0, 1.4, bob=3.0, rate=4.0, phase=0.27 * k, **along(-44 - 26 * k, -190, 640, 0.06))
        m = c.part(pivot=(0, 0, 6))
        m.box(GOLD, (0, 0, 7), (12, 9, 9))
        m.box(GOLD, (6, 0, 14), (7, 7, 7))
    f = hen(sc, 0, -60, 358, -90, 1.8, col=WHITE, peck=10, rate=1.8, phase=0.1)   # the cockerel
    t = f.part(pivot=(0, 0, 22))
    tilt_box(t, NAVY, (-14, 0, 30), (-34, 0, 58), 14, 6)
    tilt_box(t, TEAL, (-16, 0, 28), (-40, 0, 44), 10, 6)
    for side in (-1, 1):
        w = f.part(side * 48.0, (0, side * 10, 32), 0.0)
        w.box(WHITE, (0, side * 13, 24), (22, 5, 16))


# ---------------------------------------------------------------------------------------------- 18 cranes
def crane(sc, x, y, z, yaw, s=1.1, pose='call', **common):
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 100))
    tilt_box(b, GREY, (-26, 0, 96), (22, 0, 108), 24, 26)
    b.box(SHEET, (-34, 0, 92), (24, 22, 20))
    for side in (-1, 1):
        b.box(NAVY, (0, side * 6, 45), (4.5, 4.5, 92))
    if pose == 'feed':
        h = f.part(7.0, (20, 0, 106), 90.0)
        tilt_box(h, GREY, (20, 0, 106), (52, 0, 62), 8, 8)
        tilt_box(h, GREY, (52, 0, 62), (56, 0, 22), 8, 8)
        h.box(GREY, (58, 0, 16), (12, 10, 14))
        h.box(RED, (64, 0, 18), (4, 10.5, 10))
        tilt_box(h, WIRE, (58, 0, 10), (54, 0, -10), 5, 5)
    else:
        h = f.part(10.0, (20, 0, 108), 90.0) if pose == 'call' else b
        tilt_box(h, GREY, (20, 0, 108), (30, 0, 166), 8, 8)
        h.box(GREY, (33, 0, 170), (14, 10, 10))
        h.box(RED, (33, 0, 177), (10, 10.5, 4))
        tilt_box(h, WIRE, (40, 0, 170), (64, 0, 176), 5, 5)
    if pose == 'dance':
        for side in (-1, 1):
            w = f.part(side * 52.0, (0, side * 12, 112), 0.0)
            w.box(GREY, (-4, side * 46, 112), (40, 68, 5))
            w.box(NAVY, (-4, side * 87, 112), (34, 14, 5))


def crane_dance(sc):
    """Sandhill cranes on a pale flat, 13 x 8 m: two dance, hopping with their wings beating, two feed, one calls."""
    g = sc.still()
    g.plan(CREAM, [(660, 30), (500, 300), (120, 400), (-360, 360), (-640, 160), (-600, -220), (-200, -400), (320, -380), (600, -200)], -100, 8)
    g.plan(STEEL, ellipse(240, 120, 10, 330, 180), 8, 13)
    for k, (x, y) in enumerate(((-520, 200), (-460, 260), (520, -120), (-80, 340), (440, 240))):
        tuft(g, DRYG, x, y, 8, 1.4)
    crane(sc, -150, -60, 8, 10, pose='dance', bob=30.0, rate=0.9, phase=0.0)
    crane(sc, 60, -40, 8, 190, pose='dance', bob=26.0, rate=0.9, phase=0.25)
    crane(sc, -430, 60, 8, -30, pose='feed', rate=0.6, phase=0.4)
    crane(sc, 350, 120, 13, 150, pose='feed', rate=0.5, phase=0.8)
    crane(sc, 420, -230, 8, -100, pose='call', rate=1.4, phase=0.6)


# ---------------------------------------------------------------------------------------------- 19 prairie dogs
def pdog(sc, x, y, z, yaw, s=1.5, look=0.0, arms=True, **common):
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 24))
    b.box(DRYG, (0, 0, 24), (18, 18, 44))
    b.box(CREAM, (9.5, 0, 22), (4, 14, 30))
    tilt_box(b, DRYG, (-9, 0, 6), (-24, 0, 10), 6, 5)
    if arms:
        for side in (-1, 1):
            b.box(DRYG, (10, side * 8, 34), (8, 4.5, 5))
    h = f.part(look, (0, 0, 46), P.UP) if look else b
    h.box(DRYG, (3, 0, 54), (18, 16, 16))
    h.box(WIRE, (13, 0, 53), (4, 6, 5))


def prairie_dogs(sc):
    """A prairie dog town, 13 x 7 m: sentries on their mounds look about, four pop in and out of their holes, one
    jumps, two pups chase along the front and come round."""
    g = sc.still()
    g.plan(SAND, [(650, 20), (520, 280), (100, 350), (-380, 320), (-650, 120), (-600, -220), (-180, -350), (340, -330), (610, -180)], -100, 6)
    mounds = ((-420, 80), (-160, -60), (120, 120), (380, -40), (-20, 230))
    for k, (x, y) in enumerate(mounds):
        g.cyl(CONC, (x, y, 0), 95, 46, steps=8, top_r=44)
        g.cyl(NAVY, (x, y, 44), 30, 5, steps=6)
    tuft(g, DRYG, 540, 160, 6)
    tuft(g, SAGE_D, -540, -120, 6)
    tuft(g, DRYG, 220, -260, 6)
    for k, (x, y) in enumerate(mounds[:4]):                          # in and out of the holes
        pdog(sc, x, y, 26, -90 + 25 * (k - 1.5), arms=False, bob=-24.0, rate=0.35 + 0.08 * k, phase=0.27 * k)
    pdog(sc, -420 + 62, 80 - 20, 30, -60, look=34, rate=0.5, phase=0.1)       # sentries on the mounds' shoulders
    pdog(sc, 120 - 60, 120 - 24, 30, -120, look=30, rate=0.4, phase=0.6)
    pdog(sc, 380 + 58, -40 - 30, 30, -80, look=38, rate=0.6, phase=0.3)
    pdog(sc, -20, 230, 49, -90, bob=20.0, rate=1.2, phase=0.5)       # the one that jumps
    for k in range(2):                                               # the pups
        c = Fr(sc, 60 - 90 * k, -250, 6, 0, 1.5, bob=6.0, rate=3.0, phase=0.4 * k, **along(60 - 90 * k, -250, 1000, 0.15))
        m = c.part(pivot=(0, 0, 10))
        m.box(DRYG, (0, 0, 10), (36, 14, 14))
        m.box(DRYG, (20, 0, 20), (14, 13, 12))
        tilt_box(m, DRYG, (-18, 0, 12), (-32, 0, 18), 6, 5)


# ---------------------------------------------------------------------------------------------- the kit
ID = 'SM_S1ANIMALS_'
JOBS = [
    (ID + 'BurroShelf', burro_shelf),
    (ID + 'SheepFlock', sheep_flock),
    (ID + 'Jackrabbits', jackrabbits),
    (ID + 'Roadrunners', roadrunners),
    (ID + 'FoxDen', fox_den),
    (ID + 'Tortoises', tortoises),
    (ID + 'QuailWall', quail_wall),
    (ID + 'Peacocks', peacocks),
    (ID + 'MuleDeer', mule_deer),
    (ID + 'DuckTank', duck_tank),
    (ID + 'BrinePool', brine_pool),
    (ID + 'SwanLagoon', swan_lagoon),
    (ID + 'PelicanLine', pelican_line, dict(far=True, grow=(5100, 200, 120), grow_down=(5100, 200, 120))),
    (ID + 'Dovecote', dovecote, dict(grow=(950, 950, 100), grow_down=(950, 950, 0))),
    (ID + 'Owls', owls, dict(far=True, grow=(3100, 200, 150), grow_down=(3100, 200, 150))),
    (ID + 'FlamingoPool', flamingo_pool),
    (ID + 'HenCoop', hen_coop),
    (ID + 'CraneDance', crane_dance),
    (ID + 'PrairieDogs', prairie_dogs),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
