"""Volcano, Stage 1, builder s1animals: the animals of the eight districts.
/Game/World/Volcano/SM_S1ANIMALS_*. Run: import ft_world_volcano_s1animals as K; K.run()

Nineteen pieces, each an ft_people.Scene (one material, M_People; one section; movement in the vertex shader only):
  low, by the lava channel's near bank (all under 2 m as built, 0.4 m of footing, no growth upwards)
                  Egrets, Geese (with their keeper), Frogs, Hens (with the dog asleep), KoiBasin, Peacocks, ShardLizard,
                  StudioPets
  behind the channels and on the flat beyond (tall)
                  SpringMonkeys, PitPonies, PackBeasts (with their keeper), CurlHornSheep (with their shepherd),
                  GreatTortoise (the one great animal), WhiteHorse (with its handler), Cranes, PinkWaders, GlassIguanas
  in the sky      MothCloud (pale lilac moths, each on a circle of 3.3 m at the widest as built, low) and CraneSkein (white
                  cranes in a V that fly ALONG the road and come round, saved far=True)
"Water" is each piece's own pale pool (turquoise-white, never lava colour). Creatures are ft_people.quadruped where a
four-legged one fits and plain boxes drawn into moving parts where not (waders, fowl, monkeys, lizards, frogs, koi, moths,
the tortoise). Sizes are exaggerated on purpose. Colours: the kit's (ft_dress_volcano.py) and the zone plan's district
palettes; nothing red, nothing orange, nothing dark that flies, nothing glows.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Volcano'

# the kit's
BASALT, BASALT_L, ASH, ASH_L = '#2a2230', '#3e3244', '#5a4e5e', '#6f6272'
CONC, PALE, PALE_L, SULPH, HUT, PANE = '#9a98a8', '#87746d', '#b0a088', '#c8b14b', '#cdc5c8', '#14101a'
MUSTARD, COCOA, GOLD = '#c7a13b', '#5c3324', '#ffd23f'
# the districts' (the zone plan)
WHITE, COBALT, LEMON, MINT = '#f4efe6', '#2456c8', '#ffe45c', '#8fe0c0'
VIOLET, MAG, LILAC, ICE = '#7a4cff', '#ff4fb8', '#b9b3d6', '#f2f0ff'
CREAM, TEAL, CHALK = '#f1e3c0', '#1f8a8a', '#fbf7ee'
SNOW, JADE, PLUM = '#f6f3ea', '#2fae7a', '#5a2a6a'
PINK, SKY, MILK, CREAMY = '#ff8fb1', '#6fc8ff', '#fbfbf5', '#fff1a8'
LIME, GRAPH, SILVER, STEAM = '#b6f03a', '#3a4a66', '#c9c4d6', '#f2f4ee'
SAND, TURQ, NAVY = '#e8d9b0', '#19c5c0', '#1c2a4a'
MINT8, BUTTER, PURE = '#7fe0b8', '#fff1c9', '#ffffff'
# ours: pool water, pasture, moss, straw, buff feathers, flamingo wing, beak
POOL, POOL_D, GRASS, MOSS, STRAW, BUFF, ROSE, BEAK = '#bff3ee', '#6fd9d2', '#a9c9a0', '#7fb86a', '#e8d38a', '#e8c890', '#ff5fa2', '#ffc93c'


# ---------------------------------------------------------------------------------------------- shared
class Fr(object):
    """A creature's own frame: it stands on (x, y, z), heads along its own +X, turned `yaw` (0 = the mesh's +X, 90 = +Y,
    -90 = towards the road), sized s. part() hands out the model for one way of moving, the pivot given in the
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
    """common= for a creature that circles (cx, cy): positive goes counter-clockwise seen from above."""
    return dict(base=(float(cx), float(cy)), travel=(-1.0, float(rps)))


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


def puff(sc, x, y, z, r=20.0, lift=8.0, rate=0.2, phase=0.0, col=STEAM):
    """A ball of steam that floats up and down on its own clock."""
    m = sc.part(P.motion(bob=-lift, rate=rate, phase=phase, pivot=(x, y, z)))
    m.sphere(col, (x, y, z), r, scale=(1.0, 1.0, 0.8), steps=(3, 5))


def reed(sc, x, y, z, h, col=MINT, tip=JADE, phase=0.0, sway=4.0):
    """A jointed reed that rocks about its foot."""
    m = sc.part(P.motion(swing=sway, pivot=(x, y, z), axis=20.0, rate=0.3, phase=phase))
    m.box(col, (x, y, z + h / 2.0), (6, 6, h))
    m.box(tip, (x, y, z + h * 0.45), (8, 8, 5))
    m.box(tip, (x, y, z + h - 4), (9, 9, 9))


def thick(s):
    """The thinnest a part may be drawn in a creature's frame of size s: 4.2 cm as placed, 5 at least."""
    return max(5.0, 4.2 / s)


def tuft(g, col, x, y, z, s=1.0):
    g.box(col, (x, y, z + 16 * s), (10 * s, 10 * s, 36 * s))
    g.box(col, (x + 12 * s, y + 4 * s, z + 12 * s), (9 * s, 9 * s, 28 * s), rot=(0, 0, 18))
    g.box(col, (x - 11 * s, y - 3 * s, z + 11 * s), (9 * s, 9 * s, 26 * s), rot=(0, 0, -20))


def pennants(sc, x0, x1, y, z, cols, n=6, drop=26.0):
    """A line of little flags between two points at one height, each flapping on its own clock."""
    for k in range(n):
        x = x0 + (x1 - x0) * (k + 0.5) / n
        m = sc.part(P.motion(swing=14, pivot=(x, y, z), axis=0.0, rate=0.9, phase=0.17 * k))
        m.box(cols[k % len(cols)], (x, y, z - drop / 2.0), (22, 4, drop))


def wader(sc, x, y, z, yaw, s=1.2, body=PURE, wing=SILVER, leg=PANE, bill=BEAK, legl=52.0, neck=50.0, dip=0.0, down=False, one_leg=False,
          crest=None, flap=0.0, **common):
    """A long-legged bird standing: an egret, a crane, a pink wader. dip: degrees the neck and head rock about the
    shoulder; down: the neck reaches down to the water; one_leg: one leg tucked up; flap: degrees the wings beat."""
    f = Fr(sc, x, y, z, yaw, s, **common)
    t = thick(s)
    b = f.part(pivot=(0, 0, legl))
    for side in (-1, 1):
        if one_leg and side == 1:
            tilt_box(b, leg, (0, 6, legl), (-14, 6, legl - 20), t, t)
        else:
            b.box(leg, (0, side * 6, legl / 2.0), (t, t, legl))
            b.box(leg, (5, side * 6, t / 2.0), (15, 7, t))
    b.sphere(body, (0, 0, legl + 14), 20, scale=(1.5, 0.9, 0.85), steps=(3, 6))
    tilt_box(b, wing, (-22, 0, legl + 17), (-46, 0, legl + 8), 16, 8)
    for side in (-1, 1):
        w = f.part(side * flap, (-2, side * 12, legl + 24), 0.0) if flap else b
        w.box(wing if flap else body, (-4, side * 18, legl + 16), (34, t, 18))
        if flap:
            w.box(wing, (-6, side * 34, legl + 20), (30, 30, t))
    n = f.part(dip, (16, 0, legl + 20), 90.0) if dip else b
    if down:
        tilt_box(n, body, (16, 0, legl + 20), (34, 0, legl + 34), 7, 7)
        tilt_box(n, body, (34, 0, legl + 34), (46, 0, legl - 18), 7, 7)
        hx, hz = 49.0, legl - 24.0
        n.box(body, (hx, 0, hz), (12, 10, 14))
        tilt_box(n, bill, (hx, 0, hz - 6), (hx + 6, 0, hz - 28), t, t)
    else:
        tilt_box(n, body, (16, 0, legl + 20), (26, 0, legl + 20 + neck), 7, 7)
        hx, hz = 31.0, legl + 22.0 + neck
        n.box(body, (hx, 0, hz), (16, 10, 10))
        tilt_box(n, bill, (hx + 7, 0, hz), (hx + 29, 0, hz - 6), t, t)
    if crest:
        n.box(crest, (hx - 2, 0, hz + 5 + t / 2.0), (10, 8, t))


def fowl(sc, x, y, z, yaw, s=1.5, body=PURE, bill=BEAK, comb=None, nod=0.0, long_neck=0.0, tail=None, feet=BEAK, **common):
    """A hen (long_neck 0) or a goose (long_neck 22): it pecks or nods."""
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 14))
    b.sphere(body, (0, 0, 26), 15, scale=(1.35, 1.0, 0.9), steps=(3, 6))
    tilt_box(b, tail or body, (-14, 0, 30), (-28, 0, 44), 12, 6)
    t = thick(s)
    for side in (-1, 1):
        b.box(feet, (2, side * 5, 7), (t, t, 14))
    h = f.part(nod, (12, 0, 28), 90.0) if nod else b
    tilt_box(h, body, (12, 0, 28), (20, 0, 44 + long_neck), 8, 8)
    h.box(body, (23, 0, 48 + long_neck), (12, 9, 9))
    h.box(bill, (31, 0, 47 + long_neck), (8, t, t))
    if comb:
        h.box(comb, (22, 0, 53 + t / 2.0 + long_neck), (8, t, t))


def monkey(sc, x, y, z, yaw, s=1.5, fur=CREAM, face=PINK, turn=0.0, scratch=0.0, **common):
    """A pale monkey sitting upright (in a pool: stand it 28 cm under the water). turn: degrees the head looks about;
    scratch: degrees one arm works at its head."""
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 20))
    b.box(fur, (0, 0, 22), (26, 32, 44))
    b.box(face, (11, 0, 26), (6, 18, 22))
    tilt_box(b, fur, (2, -17, 40), (12, -15, 14), 9, 9)
    h = f.part(turn, (0, 0, 46), P.UP) if turn else b
    h.sphere(fur, (0, 0, 60), 16, steps=(4, 6))
    h.box(face, (12, 0, 58), (8, 17, 15))
    h.box(PANE, (17, 0, 61), (5, 12, 5))
    for side in (-1, 1):
        h.box(face, (0, side * 17, 62), (6, 5, 9))
    a = f.part(scratch, (2, 17, 42), 90.0) if scratch else b
    if scratch:
        tilt_box(a, fur, (2, 17, 42), (8, 15, 70), 9, 9)
    else:
        tilt_box(a, fur, (2, 17, 40), (12, 15, 14), 9, 9)


def lizard(sc, x, y, z, yaw, s=1.7, body=TURQ, belly=CREAM, throat=MAG, crest=TEAL, nod=0.0, wag=0.0, **common):
    """A basking lizard 1.1 m long at s = 1: the head nods, the tail sways; a push-up is the common bob."""
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 10))
    tilt_box(b, body, (-25, 0, 12), (25, 0, 19), 20, 12)
    b.box(belly, (0, 0, 8), (44, 16, 6))
    for k in range(4):
        b.box(crest, (-18 + k * 11, 0, 23 + k * 1.5), (6, 5, 8))
    for sx, sy in ((16, 1), (16, -1), (-16, 1), (-16, -1)):
        tilt_box(b, body, (sx, sy * 9, 12), (sx + 6, sy * 22, 3), 7, 7)
    h = f.part(nod, (25, 0, 18), 90.0) if nod else b
    h.box(body, (37, 0, 23), (24, 16, 12))
    h.box(throat, (31, 0, 13), (10, 5, 10))
    h.box(PANE, (42, 0, 27), (5, 17, 5))
    t = f.part(wag, (-25, 0, 13), P.UP) if wag else b
    tilt_box(t, body, (-25, 0, 13), (-60, 8, 8), 10, 9)
    tilt_box(t, crest, (-60, 8, 8), (-88, 22, 5), 6, 6)


def frog(sc, x, y, z, yaw, s=2.2, col=MOSS, belly=CREAMY, **common):
    f = Fr(sc, x, y, z, yaw, s, **common)
    b = f.part(pivot=(0, 0, 8))
    tilt_box(b, col, (-12, 0, 8), (10, 0, 16), 22, 14)
    b.box(belly, (8, 0, 8), (12, 18, 8))
    b.box(col, (14, 0, 20), (14, 20, 10))
    for side in (-1, 1):
        b.box(LEMON, (15, side * 8, 27), (6, 6, 6))
        b.box(PANE, (17.5, side * 8, 27), (2, 4, 4))
        b.box(col, (-10, side * 14, 5), (18, 8, 9))
        b.box(col, (9, side * 12, 3), (6, 6, 6))


def koi(sc, cx, cy, r, ang, z, s=1.4, body=PURE, patch=GOLD, rps=0.05):
    """A koi on a circle of radius r round (cx, cy), at angle ang, heading the way it goes; its tail beats."""
    a = math.radians(ang)
    x, y = cx + r * math.cos(a), cy + r * math.sin(a)
    f = Fr(sc, x, y, z, ang + (90.0 if rps >= 0 else -90.0), s, rate=1.1, phase=(ang % 360.0) / 360.0, **ring(rps, cx, cy))
    b = f.part(pivot=(0, 0, 0))
    b.box(body, (0, 0, 0), (40, 14, 10))
    b.box(patch, (4, 0, 3), (16, 15, 6))
    b.box(body, (23, 0, 0), (9, 10, 8))
    t = f.part(22, (-20, 0, 0), P.UP)
    tilt_box(t, patch, (-20, 0, 0), (-38, 0, 0), 5, 12)


def lying_dog(sc, x, y, z, yaw, s=1.3, body=COCOA, belly=CREAM, phase=0.0):
    """A dog asleep, chin on its paws: an ear flicks, the tail thumps now and then."""
    f = Fr(sc, x, y, z, yaw, s, rate=0.6, phase=phase)
    b = f.part(pivot=(0, 0, 10))
    b.box(body, (0, 0, 12), (62, 30, 24))
    b.box(belly, (0, 0, 4), (56, 26, 8))
    b.box(body, (40, 0, 9), (24, 20, 16))
    b.box(belly, (53, 0, 6), (10, 12, 9))
    b.box(PANE, (58, 0, 9), (4, 6, 5))
    for side in (-1, 1):
        b.box(body, (44, side * 15, 3), (26, 8, 6))
    e = f.part(14, (36, 9, 17), 0.0)
    e.box(belly, (36, 12, 12), (10, 5, 12))
    t = f.part(20, (-30, 0, 8), P.UP)
    tilt_box(t, body, (-30, 0, 8), (-58, 14, 6), 7, 7)


def rider(sc, x, y, z, yaw, s, height, mv):
    """A still model that rides on a ft_people.quadruped drawn with the same place and the same moving values (a blanket,
    a pack, long hair): the animal's own frame, its head towards -Y."""
    tv = (0.0, 0.0)
    if mv.get('travel'):
        tv = (float(mv['travel'][0]), float(mv['travel'][1]) / float(mv['travel'][0]))
    base = (x, y)
    if mv.get('orbit'):
        base, tv = (mv['orbit'][0], mv['orbit'][1]), (-1.0, float(mv['orbit'][2]))
    cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
    piv = (x + s * (0.0), y + s * (0.0), z + s * height)
    mo = P.motion(pivot=piv, axis=yaw, phase=mv.get('phase', 0.0), rate=mv.get('rate', 0.0), bob=mv.get('bob', 0.0), base=base, travel=tv)
    return sc.part(mo, (x, y, z), yaw, s)


def beast(sc, x, y, z, yaw, s, spec, blanket=None, band=None, hair=None, pack=None, **mv):
    """ft_people.quadruped with what it carries: a blanket (and a band across it), long hair down its flanks, a pack."""
    kw = dict(spec)
    kw.update(mv)
    P.quadruped(sc, x, y, z, yaw=yaw, s=s, **kw)
    L, H, G = kw.get('length', 150.0), kw.get('height', 120.0), kw.get('girth', 42.0)
    if not (blanket or hair or pack):
        return
    m = rider(sc, x, y, z, yaw, s, H, kw)
    t = thick(s)
    if hair:
        for side in (-1, 1):
            m.box(hair, (side * (G / 2.0 + t / 2.0), 0, H - G * 0.75), (t, L * 0.92, G * 0.9))
        m.box(hair, (0, -L / 2.0 - t / 2.0, H - G * 0.7), (G * 0.9, t, G * 0.7))
    if blanket:
        m.box(blanket, (0, 0, H + t / 2.0), (G + 8, L * 0.5, t))
        for side in (-1, 1):
            m.box(blanket, (side * (G / 2.0 + t + 0.5), 0, H - G * 0.22), (t, L * 0.5, G * 0.5))
            if band:
                m.box(band, (side * (G / 2.0 + t + 1.5), 0, H - G * 0.36), (t, L * 0.52, G * 0.14))
    if pack:
        for side in (-1, 1):
            m.box(pack, (side * (G / 2.0 + 14), 0, H - G * 0.1), (18, L * 0.34, G * 0.42))
        m.box(CREAM, (0, 0, H + 14), (G * 0.9, L * 0.3, 18))


# ---------------------------------------------------------------------------------------------- 1 egrets (low)
def egrets(sc):
    """White egrets in a pale hot pool, 12 x 2.9 m, the rim 0.26 m: five stand in the water, necks rocking, one reaches
    down, one stands on one leg; reeds rock at the back, steam floats."""
    g = sc.still()
    g.plan(PALE_L, stadium(600, 145), -40, 26)
    g.plan(POOL, stadium(576, 121), 22, 31)
    g.plan(POOL_D, stadium(380, 60), 24, 33, loc=(-60, 10, 0))
    for k, (x, w) in enumerate(((-420, 150), (-150, 110), (260, 170))):          # crust steps on the front of the rim
        g.box(CHALK, (x, -128, 14), (w, 30, 34))
        g.box(MINT if k % 2 else COBALT, (x, -144, 22), (w * 0.6, 6, 8))
    g.box(PALE_L, (470, 96, 40), (70, 60, 40), rot=(0, 20, 0))
    g.sphere(MOSS, (-520, 100, 30), 30, scale=(1.2, 1.0, 0.6), steps=(3, 6))
    for k, (x, y, h) in enumerate(((-330, 118, 120), (-300, 126, 150), (-268, 116, 100), (120, 124, 140), (152, 118, 110), (392, 120, 130))):
        reed(sc, x, y, 24, h, phase=0.13 * k)
    wader(sc, -430, -10, 24, -60, 1.25, dip=6, rate=0.3, phase=0.0, crest=PURE)
    wader(sc, -190, 40, 24, 200, 1.25, down=True, dip=7, rate=0.5, phase=0.3)
    wader(sc, 40, -30, 24, -110, 1.3, dip=5, rate=0.25, phase=0.55, one_leg=True)
    wader(sc, 250, 30, 24, 20, 1.2, down=True, dip=8, rate=0.6, phase=0.75)
    wader(sc, 470, -20, 24, -150, 1.25, dip=7, rate=0.35, phase=0.9, crest=PURE)
    for k, (x, y) in enumerate(((-520, 20), (-90, -60), (160, 70), (360, -50))):
        puff(sc, x, y, 62, 18, 8, 0.18 + 0.03 * k, 0.27 * k)


# ---------------------------------------------------------------------------------------------- 2 geese (low)
def geese(sc):
    """White geese walking a line along an ash bank, 14 x 2.9 m: five walk the piece's length and come round, two
    goslings hop by the water trough, the goose-girl stands at the rail (patchwork apron)."""
    g = sc.still()
    g.box(ASH_L, (0, 0, -5), (1400, 290, 70))                        # the bank: 0.3 m
    g.box(GRASS, (0, 20, 31), (1360, 200, 6))
    for k in range(8):
        x = -665 + k * 190
        g.box(MILK, (x, 132, 62), (9, 9, 64))
        g.box(PINK if k % 2 else SKY, (x, 132, 97), (13, 13, 7))
    g.box(MILK, (0, 132, 80), (1340, 6, 7))
    g.box(MILK, (0, 132, 56), (1340, 6, 7))
    g.box(SKY, (-470, 70, 46), (150, 46, 26))                        # the water trough
    g.box(POOL, (-470, 70, 58), (136, 34, 5))
    for k, (x, y) in enumerate(((-180, 96), (120, 100), (430, 92), (640, 60))):       # thrift cushions and straw
        g.sphere(GRASS, (x, y, 36), 20, scale=(1.2, 1.0, 0.6), steps=(3, 6))
        g.box(PINK, (x, y, 50), (10, 10, 7))
    g.box(STRAW, (300, 40, 37), (120, 70, 8))
    for k in range(5):
        fowl(sc, -420 + k * 170, -70 + (12 if k % 2 else 0), 34, 0, 1.5, long_neck=22, nod=6, rate=1.1 + 0.07 * k, phase=0.2 * k,
             **along(-420 + k * 170, -70, 1240, 0.045))
    fowl(sc, -380, 20, 34, 140, 0.8, body=CREAMY, bob=7, rate=1.6, phase=0.1)
    fowl(sc, -330, 60, 34, 250, 0.8, body=CREAMY, bob=6, rate=1.9, phase=0.6)
    P.figure(sc, 560, 70, 34, yaw=-10, s=0.84, build='kid', skin=P.SKINS[2], top=PINK, bottom=SKY, shoes=P.SHOES[1], hair='pony', hair_col=P.HAIRS[4],
             wear='dress', band=CREAMY, arm_l='hip', arm_r='down', detail=1, rate=0.5, rock=4, phase=0.3)


# ---------------------------------------------------------------------------------------------- 3 frogs (low)
def frogs(sc):
    """Frogs on a warm steam pipe, 12 x 2.6 m, the pipe's top 0.96 m: six sit on it (throats going), one hops on the
    ground by the outfall puddle; moss mats on the pipe, a wisp of steam at a joint, horsetails behind."""
    g = sc.still()
    g.box(CONC, (0, 0, -15), (1240, 250, 50))
    rod(g, SILVER, (-590, 0, 62), (590, 0, 62), 34, steps=8)
    for x in (-600, 600):
        rod(g, GRAPH, (x - 8, 0, 62), (x + 8, 0, 62), 40, steps=8)   # blank end caps: nothing aims anywhere
    for k, x in enumerate((-400, 0, 400)):
        g.box(GRAPH, (x, 0, 22), (56, 110, 60))
        rod(g, LIME, (x - 9, 0, 62), (x + 9, 0, 62), 38, steps=8)
    for k, (x, w) in enumerate(((-250, 120), (150, 90), (300, 70), (-520, 60))):
        g.box(LIME, (x, 4, 97), (w, 44, 6))
    g.box(GRAPH, (200, 60, 70), (30, 8, 80))                         # a gauge on a stub
    g.cyl(STEAM, (200, 55, 112), 20, 8, rot=(0, 0, 90), steps=8)
    g.plan(POOL, stadium(150, 60), 9, 15, loc=(460, -70, 0))
    for k, (x, yaw, sz) in enumerate(((-470, -80, 2.2), (-320, -110, 2.0), (-120, -60, 2.3), (60, -95, 2.0), (250, -120, 2.2), (500, -70, 2.1))):
        frog(sc, x, -2, 98, yaw, sz, col=MOSS if k % 3 else LIME, bob=3 + (k % 3), rate=0.9 + 0.13 * k, phase=0.17 * k)
    frog(sc, 380, -85, 12, 20, 2.0, bob=8, rate=1.3, phase=0.4)
    for k, (x, h) in enumerate(((-560, 150), (-530, 120), (90, 160), (120, 130), (540, 140))):
        reed(sc, x, 100, 10, h, col=MINT, tip=LIME, phase=0.21 * k)
    for k in range(3):
        puff(sc, -205 + k * 14, 0, 120 + k * 22, 13 + k * 3, 8, 0.22 + 0.04 * k, 0.3 * k)


# ---------------------------------------------------------------------------------------------- 4 hens (low)
def hens(sc):
    """A hen house in its run, 12 x 2.9 m, the roof 1.7 m: seven hens peck, four chicks hop, the dog sleeps on a plank
    deck, a tin cockerel turns on the roof."""
    g = sc.still()
    g.box(ASH_L, (0, 0, -15), (1200, 290, 50))                       # the run: 0.1 m
    g.box(STRAW, (60, -10, 11), (700, 190, 5))
    g.box(CREAM, (-400, 50, 62), (240, 170, 106))                    # the house
    g.side(COCOA, [(-140, 113), (140, 113), (0, 168)], -45, 145, loc=(-400, 0, 0))
    g.box(CHALK, (-400, -38, 113), (250, 6, 8))
    g.box(PANE, (-440, -37, 44), (48, 6, 60))                        # the pop-hole and its ramp
    tilt_box(g, COCOA, (-440, -40, 16), (-440, -110, 10), 44, 6)
    for k in range(3):                                               # nest boxes, straw showing
        g.box(TEAL, (-350 + k * 46, -38, 62), (40, 8, 36))
        g.box(STRAW, (-350 + k * 46, -41, 54), (30, 6, 14))
    g.box(MILK, (-400, 50, 172), (5, 5, 14))
    v = sc.part(P.motion(swing=28, pivot=(-400, 50, 180), axis=P.UP, rate=0.15, phase=0.2))
    v.box(MUSTARD, (-392, 50, 181), (36, 5, 10))
    v.box(MUSTARD, (-378, 50, 188), (10, 5, 6))
    for k in range(7):                                               # the run's rail at the back
        g.box(COCOA, (-150 + k * 120, 138, 36), (8, 8, 56))
    g.box(COCOA, (210, 138, 56), (740, 6, 7))
    g.box(TEAL, (40, 60, 22), (150, 30, 16))                         # the feed trough
    g.box(STRAW, (40, 60, 31), (136, 20, 5))
    g.box(COCOA, (455, 40, 16), (250, 150, 12))                      # the plank deck
    lying_dog(sc, 450, 40, 22, -160, 1.3, body=PALE, belly=CREAM, phase=0.3)
    spots = ((-230, -70, -30, PURE), (-90, 20, 200, BUFF), (10, -80, 80, PURE), (130, 10, -120, BUFF), (220, -70, 30, PURE), (-20, 96, 250, BUFF), (300, 70, 160, PURE))
    for k, (x, y, yaw, col) in enumerate(spots):
        fowl(sc, x, y, 12, yaw, 1.5, body=col, comb=PINK, nod=26 if k % 3 else 8, rate=1.2 + 0.11 * k, phase=0.14 * k)
    for k, (x, y) in enumerate(((-160, -40), (-130, -95), (70, -30), (180, -100))):
        fowl(sc, x, y, 12, 60 * k, 0.7, body=CREAMY, bob=5 + k % 3, rate=1.8 + 0.1 * k, phase=0.23 * k)


# ---------------------------------------------------------------------------------------------- 5 koi basin (low)
def koi_basin(sc):
    """A pale stone basin of koi, 9.2 x 2.9 m, the rim 0.55 m: seven koi (white, gold and plum) go round two slow
    circles, lily pads float, two terrapins sun on a stone, a gold finial at each end."""
    g = sc.still()
    g.plan(PALE_L, stadium(440, 145), -40, 55)
    g.plan(SNOW, stadium(440, 145), 55, 61)
    g.plan(POOL, stadium(410, 116), 53, 66)
    for x in (-452, 452):
        g.box(PLUM, (x, 0, 62), (14, 14, 126))
        g.sphere(GOLD, (x, 0, 132), 11, steps=(3, 6))
    g.box(JADE, (0, -146, 30), (500, 6, 10))                         # a jade tile line along the front
    g.box(PALE_L, (30, 30, 68), (70, 54, 22), rot=(0, 15, 0))        # the terrapins' stone
    for k, (x, y, yaw) in enumerate(((16, 24, 30), (46, 40, -50))):
        f = Fr(sc, x, y, 79, yaw, 1.3, rate=0.3, phase=0.4 * k)
        b = f.part(pivot=(0, 0, 6))
        b.sphere(JADE, (0, 0, 8), 13, scale=(1.3, 1.0, 0.6), steps=(3, 6))
        h = f.part(14, (14, 0, 7), 90.0)
        h.box(SULPH, (20, 0, 9), (12, 8, 7))
    for k, (ang, r, col, patch) in enumerate(((0, 78, PURE, GOLD), (130, 60, PURE, PLUM), (240, 86, CREAMY, GOLD), (300, 48, PURE, PLUM))):
        koi(sc, -200, 0, r, ang, 64, 1.4, col, patch, 0.05 if k % 2 else 0.04)
    for k, (ang, r, col, patch) in enumerate(((40, 70, PURE, PLUM), (170, 84, PURE, GOLD), (280, 56, CREAMY, PLUM))):
        koi(sc, 230, 0, r, ang, 64, 1.4, col, patch, -0.045 if k % 2 else -0.055)
    for k, (x, y, r) in enumerate(((-330, 70, 24), (-60, -70, 20), (110, 80, 22), (350, -60, 26), (-250, -80, 18))):
        m = sc.part(P.motion(bob=-1.5, rate=0.25, phase=0.2 * k, pivot=(x, y, 66)))
        m.cyl(JADE, (x, y, 64), r, 5, steps=7)
        if k % 2 == 0:
            m.box(SNOW, (x + 6, y, 72), (9, 9, 7))
    for k, (x, y) in enumerate(((-380, 118), (300, 122))):
        g.sphere(MOSS, (x, y, 64), 22, scale=(1.3, 1.0, 0.6), steps=(3, 6))


# ---------------------------------------------------------------------------------------------- 6 peacocks (low)
def _peacock(sc, x, y, z, yaw, s=1.0, fan=False, train=False, hen=False, nod=0.0, **common):
    body = PALE_L if hen else COBALT
    f = Fr(sc, x, y, z, yaw, s, **common)
    t = thick(s)
    b = f.part(pivot=(0, 0, 20))
    b.sphere(body, (0, 0, 30), 17, scale=(1.4, 0.95, 0.9), steps=(3, 6))
    for side in (-1, 1):
        b.box(PALE, (2, side * 6, 8), (t, t, 18))
        b.box(SAND if hen else TURQ, (-4, side * 14, 32), (30, t, 14))
    h = f.part(nod, (14, 0, 34), 90.0) if nod else b
    tilt_box(h, body, (14, 0, 34), (24, 0, 68), 8, 8)
    h.box(body, (27, 0, 72), (13, 9, 9))
    h.box(CHALK, (27, 0, 72), (max(6.0, t), 10, t))
    h.box(SULPH, (36, 0, 71), (8, t, t))
    for k in (-1, 0, 1):
        h.box(TURQ if not hen else PALE, (24 + k * (t - 1), k * (t - 1), 81), (t, t, 10))
    if fan:                                                          # the fan: nine feathers in a half wheel, shimmering
        w = f.part(5, (-12, 0, 30), 0.0, rate=2.6)
        for k in range(9):
            a = math.radians(-80 + k * 20)
            tip = (-18, math.sin(a) * 70, 34 + math.cos(a) * 70)
            tilt_box(w, JADE if k % 2 else TURQ, (-14, math.sin(a) * 10, 34 + math.cos(a) * 10), tip, 15, 5)
            w.box(SULPH, (-21, math.sin(a) * 58, 34 + math.cos(a) * 58), (5, 11, 11))
            w.box(NAVY, (-23.5, math.sin(a) * 58, 34 + math.cos(a) * 58), (5, 6, 6))
    elif train:                                                      # the train lying along the wall behind it, sweeping a little
        w = f.part(7, (-14, 0, 30), P.UP, rate=0.4)
        tilt_box(w, JADE, (-14, 0, 32), (-150, 0, 8), 26, 8)
        for k in range(4):
            w.box(SULPH, (-60 - k * 26, 0, 25 - k * 4.5), (11, 12, 6))
    else:
        tilt_box(b, body, (-18, 0, 32), (-46, 0, 22), 16, 7)


def peacocks(sc):
    """Peacocks on a low sand-stone wall, 12.2 x 2.4 m, the wall 0.78 m: one faces the road with its fan up and
    shimmering, one stands along the wall with its train lying behind it, a peahen pecks in front, two white doves
    perch (they do not fly), a turquoise banner hangs from a short pole."""
    g = sc.still()
    g.box(SAND, (0, 40, 15), (1200, 70, 110))
    g.box(CHALK, (0, 40, 74), (1212, 84, 8))
    for x in (-590, 0, 590):
        g.box(SAND, (x, 40, 30), (64, 96, 140))
        g.box(NAVY, (x, 40, 104), (72, 104, 8))
    g.box(SAND, (0, -30, -15), (1220, 190, 50))                      # the paving in front: 0.1 m
    for k in range(6):
        g.box(CHALK, (-500 + k * 200, -70, 11), (90, 60, 5))
    _peacock(sc, -300, 40, 78, -90, 1.0, fan=True, rate=0.5, phase=0.1)
    _peacock(sc, 310, 40, 78, 0, 1.0, train=True, nod=6, rate=0.6, phase=0.5)
    _peacock(sc, 100, -70, 12, 160, 1.0, hen=True, nod=24, rate=1.3, phase=0.3)
    _peacock(sc, -130, -90, 12, 30, 0.6, hen=True, bob=6, rate=1.7, phase=0.7)
    fowl(sc, -40, 40, 78, -70, 0.8, body=PURE, bill=PINK, feet=PINK, nod=10, rate=0.8, phase=0.2)     # the doves
    fowl(sc, 505, 40, 78, -120, 0.8, body=PURE, bill=PINK, feet=PINK, bob=3, rate=1.1, phase=0.7)
    g.box(PALE, (-590, 40, 140), (6, 6, 72))                         # the banner's pole on the end pier, a gold finial
    g.sphere(SULPH, (-590, 40, 180), 7, steps=(3, 6))
    w = sc.part(P.motion(swing=10, pivot=(-590, 40, 170), axis=P.UP, rate=0.6, phase=0.4))
    w.box(TURQ, (-560, 40, 152), (54, 5, 40))
    w.box(SULPH, (-560, 40, 129), (54, 5, 6))


# ---------------------------------------------------------------------------------------------- 7 lizards on a shard (low)
def shard_lizard(sc):
    """Teal lizards basking on an obsidian slab, 9.2 x 2.8 m, the slab 0.66 m: one does push-ups, one nods, one sways its
    tail on the lower step, a small one on the leaning shard's foot; lilac lichen, a magenta dewlap each."""
    g = sc.still()
    g.box(ASH_L, (0, 0, -12), (920, 280, 56))                        # a pale ash pad: 0.16 m
    g.box(BASALT_L, (-60, 20, 40), (640, 210, 52), rot=(0, 4, 0))    # the slab
    g.box(ASH, (-60, 20, 68), (600, 180, 5), rot=(0, 4, 0))
    g.box(BASALT_L, (300, -40, 28), (240, 150, 30), rot=(0, -8, 0))  # the lower step: 0.43 m
    tilt_box(g, BASALT, (380, 80, 14), (424, 96, 186), 90, 36)       # the leaning shard
    tilt_box(g, LILAC, (372, 60, 20), (414, 76, 180), 8, 8)
    tilt_box(g, VIOLET, (-430, 90, 14), (-446, 100, 120), 60, 26)
    for k, (x, y, z, w) in enumerate(((-250, 96, 71, 50), (-20, -60, 71, 36), (150, 86, 71, 44), (330, -90, 44, 30), (-400, -90, 17, 40))):
        g.box(LILAC if k % 2 else ICE, (x, y, z), (w, w * 0.7, 5), rot=(0, 20 * k, 0))
    lizard(sc, -220, 10, 71, -25, 1.7, nod=10, wag=8, bob=7, rate=1.2, phase=0.0)
    lizard(sc, 60, 40, 71, 200, 1.6, body=TEAL, crest=TURQ, nod=14, wag=10, rate=0.5, phase=0.4)
    lizard(sc, 300, -50, 44, -70, 1.5, nod=8, wag=16, rate=0.35, phase=0.7)
    lizard(sc, -400, -50, 17, 40, 1.0, body=TEAL, crest=TURQ, nod=12, wag=12, bob=4, rate=1.5, phase=0.2)


# ---------------------------------------------------------------------------------------------- 8 studio pets (low)
def _chair(g, x, y, z, yaw, canvas=MINT8, star=False):
    m = g
    c, s = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))

    def W(p):
        return (x + p[0] * c - p[1] * s, y + p[0] * s + p[1] * c, z + p[2])
    for sx in (-1, 1):
        tilt_box(m, NAVY, W((sx * 24, -20, 0)), W((sx * 24, 20, 48)), 5, 5)
        tilt_box(m, NAVY, W((sx * 24, 20, 0)), W((sx * 24, -20, 48)), 5, 5)
        m.box(NAVY, W((sx * 24, 22, 72)), (5, 5, 50), rot=(0, yaw, 0))
    m.box(canvas, W((0, 0, 48)), (52, 44, 5), rot=(0, yaw, 0))
    m.box(canvas, W((0, 22, 84)), (52, 5, 24), rot=(0, yaw, 0))
    if star:
        m.box(GOLD, W((0, 19, 84)), (12, 5, 12), rot=(45, yaw, 0))


def studio_pets(sc):
    """The backlot's animals waiting for their scene, 9 x 2.6 m: the studio cat on a folding chair (tail flicking), a
    kitten hopping on the next, a parrot on a T-perch (it bobs and tilts its head), a little dog sitting up on a
    striped drum (paws going)."""
    g = sc.still()
    g.box(BUTTER, (0, 0, -15), (900, 260, 50))                       # the mat: 0.1 m
    for k in range(12):
        g.box(NAVY if k % 2 else PURE, (-412 + k * 75, -126, 12), (75, 8, 8))
    _chair(g, -330, 30, 10, 0)
    _chair(g, -230, 30, 10, 0, canvas=BUTTER, star=True)
    _chair(g, -130, 30, 10, 0)
    f = Fr(sc, -330, 26, 63, -90, 1.2, rate=0.5, phase=0.1)          # the cat: white, navy patches
    b = f.part(pivot=(0, 0, 10))
    b.box(PURE, (0, 0, 14), (30, 24, 28))
    b.box(NAVY, (-6, 0, 24), (16, 25, 10))
    b.box(PURE, (12, 0, 36), (20, 20, 18))
    b.box(PINK, (22, 0, 34), (4, 6, 5))
    for side in (-1, 1):
        b.box(NAVY if side > 0 else PURE, (10, side * 7, 48), (6, 6, 8))
        b.box(PANE, (21.5, side * 5, 39), (4, 4, 4))
    t = f.part(26, (-14, 0, 6), P.UP, rate=0.7)
    tilt_box(t, NAVY, (-14, 0, 6), (-14, 26, 5), 6, 6)
    tilt_box(t, PURE, (-14, 26, 5), (-6, 36, 5), 6, 6)
    f = Fr(sc, -130, 26, 63, -60, 0.7, rate=1.6, phase=0.4, bob=7)   # the kitten
    b = f.part(pivot=(0, 0, 10))
    b.box(BUTTER, (0, 0, 12), (30, 22, 22))
    b.box(BUTTER, (14, 0, 30), (20, 20, 18))
    for side in (-1, 1):
        b.box(BUTTER, (12, side * 7, 42), (6, 6, 8))
    tilt_box(b, BUTTER, (-14, 0, 14), (-26, 0, 36), 6, 6)
    g.box(NAVY, (130, 20, 14), (70, 70, 8))                          # the parrot's T-perch
    g.box(MINT8, (130, 20, 82), (6, 6, 136))
    g.box(MINT8, (130, 20, 150), (90, 6, 6))
    g.box(GOLD, (130, 16, 96), (12, 5, 12), rot=(45, 0, 0))
    f = Fr(sc, 130, 20, 153, -90, 1.2, rate=0.9, phase=0.2, bob=4)   # the parrot: turquoise, a butter head
    b = f.part(pivot=(0, 0, 0))
    b.box(TURQ, (0, 0, 13), (14, 16, 26))
    b.box(MINT8, (-2, 0, 14), (10, 20, 18))
    tilt_box(b, COBALT, (-6, 0, 4), (-12, 0, -22), 9, 5)
    for side in (-1, 1):
        b.box(PALE, (3, side * 4, 2), (5, 5, 5))
    h = f.part(16, (2, 0, 26), 0.0, rate=0.45)
    h.box(BUTTER, (3, 0, 28), (14, 14, 12))
    h.box(NAVY, (12, 0, 26), (7, 6, 8))
    h.box(PANE, (8, 0, 30), (4, 15, 4))
    g.cyl(BUTTER, (340, 0, 10), 46, 60, steps=10)                    # the striped drum, a gold star on it
    for k in range(5):
        a = math.radians(k * 72)
        g.box(NAVY, (340 + math.cos(a) * 45, math.sin(a) * 45, 40), (8, 8, 60), rot=(0, k * 72, 0))
    g.cyl(PURE, (340, 0, 70), 49, 5, steps=10)
    f = Fr(sc, 340, 0, 75, -90, 1.2, rate=1.4, phase=0.6)            # the little dog, sitting up
    b = f.part(pivot=(0, 0, 10))
    tilt_box(b, PURE, (-6, 0, 6), (4, 0, 44), 22, 20)
    b.box(PURE, (-8, 0, 8), (30, 26, 16))
    b.box(PURE, (10, 0, 54), (22, 20, 18))
    b.box(BUTTER, (22, 0, 50), (8, 10, 8))
    b.box(PANE, (26.5, 0, 52), (4, 5, 4))
    b.box(MINT8, (4, 0, 43), (20, 21, 5))
    for side in (-1, 1):
        b.box(BUTTER, (6, side * 11, 60), (8, 5, 14))
        p = f.part(side * 18, (10, side * 9, 38), 90.0)
        tilt_box(p, PURE, (10, side * 9, 38), (24, side * 9, 32), 6, 6)
    t = f.part(30, (-20, 0, 12), P.UP, rate=2.0)
    tilt_box(t, PURE, (-20, 0, 12), (-36, 0, 22), 6, 6)
    g.box(NAVY, (-420, -40, 36), (40, 40, 44))                       # a prop box with a hoop leaning on it
    for k in range(8):
        a0, a1 = math.radians(k * 45), math.radians(k * 45 + 45)
        tilt_box(g, GOLD, (-420 + math.cos(a0) * 40, -64, 62 + math.sin(a0) * 40), (-420 + math.cos(a1) * 40, -64, 62 + math.sin(a1) * 40), 6, 6)


# ---------------------------------------------------------------------------------------------- 9 monkeys in a hot spring
def spring_monkeys(sc):
    """Pale monkeys soaking in a round hot pool on a whitewashed drum, 9.6 x 9 m, the water 1.7 m up: five sit in the
    water to their chests and look about, one scratches its head, one sits on the rim with a little one hopping beside
    it; steam rolls off the water and out of a mint pipe with a mushroom cap."""
    g = sc.still()
    g.cyl(WHITE, (0, 0, -150), 400, 290, steps=10)
    g.cyl(COBALT, (0, 0, 96), 406, 20, steps=10)
    g.cyl(WHITE, (0, 0, 140), 345, 28, steps=10)
    g.cyl(POOL, (0, 0, 150), 305, 20, steps=10)
    g.cyl(POOL_D, (40, 20, 152), 150, 20, steps=8)
    for k in range(3):                                               # the steps up, towards the road
        g.box(WHITE, (0, -400 - k * 44, 42 - k * 46 - 60), (220, 60, 150 + 0 * k))
    for k in range(3):
        g.box(COBALT, (0, -424 - k * 44, 116 - k * 46), (220, 12, 6))
    g.cyl(MINT, (250, 310, -100), 22, 520, steps=6)                  # the steam pipe and its cap
    g.cyl(MINT, (250, 310, 420), 54, 26, steps=8, top_r=22)
    g.cyl(MINT, (250, 310, 396), 30, 8, steps=8)
    for k in range(4):
        puff(sc, 250 + k * 16, 310, 470 + k * 44, 26 + k * 7, 26, 0.16 + 0.03 * k, 0.25 * k)
    g.box(PALE_L, (-330, 250, 40), (8, 8, 240))                      # a towel rail: lemon and white towels
    g.box(PALE_L, (-150, 330, 40), (8, 8, 240))
    tilt_box(g, PALE_L, (-330, 250, 158), (-150, 330, 158), 6, 6)
    for k, col in enumerate((LEMON, CHALK, LEMON)):
        w = sc.part(P.motion(swing=5, pivot=(-300 + k * 60, 264 + k * 27, 158), axis=24.0, rate=0.5, phase=0.3 * k))
        w.box(col, (-300 + k * 60, 264 + k * 27, 128), (40, 5, 60), rot=(0, 24, 0))
    for k, (ang, r, turn, col) in enumerate(((200, 190, 34, CREAM), (265, 150, 28, HUT), (330, 200, 40, CREAM), (60, 170, 30, HUT), (130, 120, 36, CREAM))):
        a = math.radians(ang)
        monkey(sc, r * math.cos(a), r * math.sin(a), 142, ang + 180 + (30 if k % 2 else -40), 1.5, fur=col, turn=turn if k != 2 else 0,
               scratch=22 if k == 2 else 0, rate=(1.8 if k == 2 else 0.22 + 0.05 * k), phase=0.21 * k)
    monkey(sc, -230, -230, 168, -120, 1.5, fur=HUT, turn=24, rate=0.3, phase=0.5)
    monkey(sc, -150, -280, 168, -60, 0.8, fur=CREAM, bob=6, rate=1.5, phase=0.1)
    for k, (x, y) in enumerate(((-120, 60), (90, -90), (160, 140), (-40, -160))):
        puff(sc, x, y, 215, 30, 22, 0.14 + 0.03 * k, 0.27 * k)


# ---------------------------------------------------------------------------------------------- 10 pit ponies
PONY = dict(length=120, height=100, girth=52, neck=40, neck_up=40, head=38, tail=46)


def pit_ponies(sc):
    """Ponies at a hitching rail on an ash bank, 14 x 6 m: one drinks at the trough, two graze at a hay pile, one stands
    in its teal blanket flicking its tail, a foal hops, a dog sleeps by the water butt."""
    g = sc.still()
    g.box(ASH_L, (0, 0, -110), (1400, 600, 250))                     # the bank: 0.15 m
    g.box(PALE_L, (0, 40, 16), (1240, 420, 12))
    for k in range(6):                                               # the rail along the back
        g.box(COCOA, (-600 + k * 240, 250, 75), (14, 14, 126))
    g.box(COCOA, (0, 250, 122), (1220, 9, 10))
    g.box(COCOA, (0, 250, 80), (1220, 9, 10))
    g.box(TEAL, (-330, 150, 52), (240, 64, 60))                      # the trough
    g.box(POOL, (-330, 150, 80), (224, 50, 6))
    g.box(STRAW, (260, 60, 40), (190, 150, 40))                      # the hay
    g.box(CREAMY, (260, 60, 64), (140, 100, 14))
    g.cyl(TEAL, (590, 180, 20), 44, 110, steps=8)                    # a water butt, a bucket
    g.cyl(MUSTARD, (520, 110, 22), 22, 34, steps=8, top_r=26)
    for k, col in enumerate((TEAL, CREAM, MUSTARD)):                 # blankets airing on the rail
        w = sc.part(P.motion(swing=5, pivot=(-40 + k * 150, 250, 124), axis=0.0, rate=0.5, phase=0.3 * k))
        w.box(col, (-40 + k * 150, 245, 96), (90, 6, 56))
    beast(sc, -330, 50, 22, 180, 1.3, dict(PONY, body=CREAM, mane=COCOA, tail_col=COCOA, snout=PALE_L, ears=CREAM), neck_up=-22, graze=6, wag=22, rate=0.4, phase=0.1)
    beast(sc, 130, 0, 22, 110, 1.3, dict(PONY, body=HUT, mane=CHALK, tail_col=CHALK, snout=PALE, ears=HUT), neck_up=-28, graze=9, wag=16, rate=0.5, phase=0.5)
    beast(sc, 400, 130, 22, 250, 1.25, dict(PONY, body=MUSTARD, mane=COCOA, tail_col=COCOA, snout=CREAM, ears=MUSTARD), neck_up=-26, graze=10, rate=0.45, phase=0.8)
    beast(sc, -70, -150, 22, 80, 1.35, dict(PONY, body=PALE_L, mane=COCOA, tail_col=COCOA, snout=CREAM, ears=PALE_L), blanket=TEAL, band=CREAM, wag=30, rate=0.8, phase=0.3)
    beast(sc, -560, -120, 22, 60, 0.75, dict(PONY, body=CREAM, mane=COCOA, tail_col=COCOA, ears=CREAM, head=44), bob=10, wag=30, rate=1.3, phase=0.6)
    lying_dog(sc, 560, 60, 22, -130, 1.4, body=COCOA, belly=CREAM, phase=0.2)


# ---------------------------------------------------------------------------------------------- 11 pack beasts
YAK = dict(length=150, height=112, girth=72, neck=30, neck_up=4, head=42, horns=CHALK, tail=50)


def _staff(m, h, side):
    m.box(PALE, (h[0], h[1] - 4, h[2] + 26), (5, 5, 170))


def pack_beasts(sc):
    """Long-haired pack animals with coloured blankets on a pale stone causeway, 18 x 4.4 m, 1.2 m up, behind a low
    parapet with jade posts and gold finials: four walk the piece's length with their robed keeper and come round, a
    calf stands by the wall; pennants flap between the posts."""
    g = sc.still()
    g.box(PALE_L, (0, 0, -90), (1800, 440, 420))                     # the causeway: 1.2 m
    g.box(SNOW, (0, 0, 122), (1800, 440, 8))
    g.box(SNOW, (0, -208, 142), (1800, 24, 32))                      # the parapet, towards the road
    g.box(JADE, (0, -208, 161), (1800, 30, 6))
    for k in range(7):
        x = -870 + k * 290
        g.box(JADE, (x, 204, 190), (16, 16, 130))
        g.sphere(GOLD, (x, 204, 264), 13, steps=(3, 6))
    for k in range(6):
        pennants(sc, -870 + k * 290 + 20, -870 + (k + 1) * 290 - 20, 204, 246, (SNOW, JADE, GOLD, PLUM), n=3, drop=30)
    for k in range(5):                                               # battered buttresses on the face
        g.box(PALE_L, (-720 + k * 360, -232, 20), (90, 40, 200))
    coats = ((COCOA, JADE, GOLD), (SNOW, PLUM, GOLD), (ASH_L, GOLD, PLUM), (CREAM, JADE, SNOW))
    for k, (coat, blanket, band) in enumerate(coats):
        x = -640 + k * 400
        beast(sc, x, 10 + (24 if k % 2 else -10), 126, 90, 1.25, dict(YAK, body=coat, legs_col=coat, snout=PALE_L, ears=coat, tail_col=coat),
              blanket=blanket, band=band, hair=coat, pack=CREAM if k % 2 else SULPH, stride=14, wag=10, rate=0.9 + 0.05 * k, phase=0.23 * k, travel=(1700, 60))
    P.figure(sc, 800, -60, 126, yaw=90, s=1.15, skin=P.SKINS[3], top=PLUM, bottom=PLUM, shoes=P.SHOES[3], hair='bald', wear='dress', band=GOLD,
             sleeves='long', arm_l='down', arm_r='hold', hold_r=_staff, legs='stride', stride=16, detail=1, rate=0.9, phase=0.4, travel=(1700, 60))
    beast(sc, -760, 130, 126, 200, 0.8, dict(YAK, body=SNOW, legs_col=SNOW, snout=PALE_L, ears=SNOW, tail_col=SNOW, horns=None), hair=SNOW, wag=26, bob=5, rate=1.1, phase=0.6)


# ---------------------------------------------------------------------------------------------- 12 curl-horned sheep
SHEEP = dict(length=95, height=70, girth=56, neck=20, neck_up=12, head=30, body=MILK, legs_col=ASH_L, snout=ASH_L, ears=ASH_L, tail=12, tail_col=MILK,
             horns=CREAMY)


def _crook(m, h, side):
    m.box(PALE, (h[0], h[1] - 4, h[2] + 30), (5, 5, 180))
    m.box(PALE, (h[0], h[1] - 12, h[2] + 118), (5, 20, 5))


def curl_horn_sheep(sc):
    """Curl-horned sheep on a grey-green swell, 14 x 7 m: four graze, the ram stands on the rise with his great curled
    horns, a lamb hops, the shepherd leans on a crook by a patchwork feed rack."""
    g = sc.still()
    g.box(GRASS, (0, 0, -110), (1400, 700, 240))                     # the swell: 0.1 m, then 0.6 m
    g.box(GRASS, (60, 60, 30), (900, 420, 60))
    g.box(ASH_L, (0, -352, -20), (1404, 8, 60))
    for k, (x, y, z) in enumerate(((-600, -200, 10), (-420, 250, 10), (560, -260, 10), (620, 200, 10), (-100, 150, 60))):
        g.sphere(MOSS, (x, y, z + 8), 26, scale=(1.2, 1.0, 0.6), steps=(3, 6))
        g.box(PINK, (x, y, z + 26), (12, 12, 8))
    g.box(PALE, (-470, 60, 60), (10, 10, 100))                       # the feed rack: a patchwork cover
    g.box(PALE, (-250, 60, 60), (10, 10, 100))
    g.box(STRAW, (-360, 60, 70), (220, 60, 40))
    for k, col in enumerate((SKY, CREAMY, PINK, MILK)):
        g.box(col, (-441 + k * 54, 60, 116), (54, 90, 6), rot=(0, 0, 0))
    for k, (x, y, z, yaw, up, gz) in enumerate(((-180, -150, 10, 100, -28, 10), (220, -200, 10, 250, -30, 12), (420, 80, 60, 80, -30, 11), (-60, 180, 60, 200, -28, 9))):
        beast(sc, x, y, z, yaw, 1.4, SHEEP, neck_up=up, graze=gz, wag=20 if k % 2 else 0, rate=0.4 + 0.07 * k, phase=0.19 * k)
    beast(sc, 160, 40, 60, -60, 1.6, dict(SHEEP, horns=None, head=36), neck_up=34, wag=14, rate=0.5, phase=0.6)   # the ram, looking at the road
    r = sc.still((160, 40, 60), -60, 1.6)                           # his horns: two curls beside the head (the quadruped's own frame)
    hy, hz = -95 / 2.0 + 56 * 0.25 - math.cos(math.radians(34)) * 20, 70 - 56 * 0.35 + math.sin(math.radians(34)) * 20
    for side in (-1, 1):
        ring_pts = [(side * (16 + 9 * math.sin(math.radians(a))), hy + 4 + 13 * math.cos(math.radians(a)), hz + 12 + 13 * math.sin(math.radians(a))) for a in range(60, 420, 60)]
        for p, q in zip(ring_pts, ring_pts[1:]):
            tilt_box(r, CREAMY, p, q, 7, 7)
    beast(sc, -330, -230, 10, 30, 0.95, dict(SHEEP, horns=None, head=36), bob=12, wag=30, rate=1.4, phase=0.3)
    P.figure(sc, -560, 120, 10, yaw=10, s=1.15, skin=P.SKINS[1], top=SKY, bottom=CREAMY, shoes=P.SHOES[3], hair='long', hair_col=P.HAIRS[3], hat='sun',
             hat_col=PINK, wear='dress', sleeves='long', arm_l='hip', arm_r='hold', hold_r=_crook, detail=1, rate=0.4, rock=3, phase=0.2)


# ---------------------------------------------------------------------------------------------- 13 the great tortoise
def great_tortoise(sc):
    """THE GREAT TORTOISE, the level's one great animal: 5.6 m long, its shell 3.1 m up, a garden grown on its back
    (moss cushions, ferns, a little tree fern, two white egrets riding). It stands along the road on a low ash mound,
    7.4 x 4.8 m: the neck sways slowly from side to side, the legs shift, the tail wags, it chews a fern."""
    g = sc.still()
    g.box(ASH_L, (0, 0, -120), (740, 480, 270))                      # the mound: 0.15 m
    g.box(PALE_L, (0, 0, 16), (640, 400, 10))
    olive, plate, skin = '#8fa870', PALE_L, PALE
    g.sphere(olive, (-20, 0, 200), 150, scale=(1.55, 1.15, 0.78), steps=(5, 10))    # the shell
    g.box(SAND, (-20, 0, 118), (400, 300, 30))                       # the plastron
    g.box(SULPH, (-20, 0, 136), (440, 336, 10))                      # the shell's rim
    for k, (x, y, z, w, yaw) in enumerate(((-20, 0, 314, 110, 0), (-150, 0, 290, 90, 0), (110, 0, 290, 90, 0), (-90, -95, 262, 80, 20), (50, -95, 262, 80, -20),
                                            (-90, 95, 262, 80, -20), (50, 95, 262, 80, 20), (-225, -60, 236, 60, 30), (190, -60, 236, 60, -30))):
        g.box(plate, (x, y, z), (w, w * 0.85, 12), rot=(0, yaw, 0))
        g.box(SULPH, (x, y, z + 7), (w * 0.5, w * 0.42, 5), rot=(0, yaw, 0))
    for k, (x, y, z, r) in enumerate(((-60, 40, 322, 34), (40, -30, 320, 28), (-170, 30, 296, 30), (130, 50, 292, 26), (-20, -110, 270, 26), (-120, 110, 262, 24))):
        g.sphere(LIME if k % 2 else MOSS, (x, y, z), r, scale=(1.2, 1.0, 0.6), steps=(3, 6))
    g.cyl(PALE, (-110, -20, 300), 9, 70, steps=6)                    # a little tree fern on its back
    for k in range(5):
        f = sc.part(P.motion(swing=5, pivot=(-110, -20, 370), axis=k * 72.0 + 90.0, rate=0.35, phase=0.2 * k))
        a = math.radians(k * 72)
        tilt_box(f, JADE, (-110, -20, 370), (-110 + math.cos(a) * 58, -20 + math.sin(a) * 58, 356), 20, 5)
    for k, (x, y, z) in enumerate(((60, 60, 300), (-190, -40, 284), (150, -40, 284))):
        tuft(g, JADE, x, y, z, 1.0)
    for k, (sx, sy) in enumerate(((1, 1), (1, -1), (-1, 1), (-1, -1))):     # the legs: each shifts on its own clock
        lx, ly = -20 + sx * 160, sy * 125
        m = sc.part(P.motion(swing=5 * sx * sy, pivot=(lx, ly, 150), axis=90.0, rate=0.14, phase=0.25 * k))
        m.cyl(skin, (lx, ly, 16), 48, 140, steps=7, top_r=40)
        for j in (-1, 0, 1):
            m.box(SAND, (lx + 40, ly + j * 26, 26), (16, 14, 16))
    n = sc.part(P.motion(swing=9, pivot=(190, 0, 170), axis=P.UP, rate=0.1, phase=0.1))   # the neck and the head
    tilt_box(n, skin, (190, 0, 168), (300, 0, 224), 70, 70)
    n.box(skin, (348, 0, 240), (110, 86, 70))
    n.box(SAND, (350, 0, 214), (96, 80, 18))
    n.box(PANE, (384, 0, 226), (40, 88, 5))                          # the mouth's line
    for side in (-1, 1):
        n.box(PANE, (364, side * 42, 256), (16, 6, 14))
        n.box(CHALK, (368, side * 43.5, 260), (5, 5, 5))
    n.box(JADE, (410, 6, 222), (36, 14, 6), rot=(0, 20, 0))          # the fern it chews
    n.box(JADE, (424, -10, 214), (30, 12, 6), rot=(0, -30, 0))
    t = sc.part(P.motion(swing=14, pivot=(-240, 0, 150), axis=P.UP, rate=0.3, phase=0.6))
    tilt_box(t, skin, (-240, 0, 150), (-310, 0, 110), 30, 26)
    wader(sc, -40, 60, 330, -70, 0.8, dip=8, rate=0.4, phase=0.2, crest=PURE)
    wader(sc, 80, -40, 318, -120, 0.8, down=True, dip=8, rate=0.6, phase=0.7)
    for k, (x, y) in enumerate(((-300, -190), (250, 200), (310, -170))):
        g.sphere(MOSS, (x, y, 26), 24, scale=(1.3, 1.0, 0.6), steps=(3, 6))


# ---------------------------------------------------------------------------------------------- 14 the white horse
def white_horse(sc):
    """The backlot's trained white horse, a gold plume on its head, trotting round a striped practice ring 9.7 m across
    and 0.6 m up; its handler turns on a mint drum in the middle and waves it on; a hoop stands on the rim."""
    g = sc.still()
    g.cyl(BUTTER, (0, 0, -140), 484, 200, steps=12)
    g.cyl(MINT8, (0, 0, 40), 488, 12, steps=12)
    for k in range(24):
        a = math.radians(k * 15 + 7.5)
        g.box(NAVY if k % 2 else PURE, (math.cos(a) * 462, math.sin(a) * 462, 68), (30, 118, 16), rot=(0, k * 15 + 7.5, 0))
    g.cyl(MINT8, (0, 0, 60), 74, 44, steps=10)
    g.cyl(PURE, (0, 0, 104), 78, 6, steps=10)
    for k in range(5):
        a = math.radians(k * 72 + 20)
        g.box(GOLD, (math.cos(a) * 250, math.sin(a) * 250, 62), (26, 26, 5), rot=(0, 45 + k * 72, 0))
    horse = dict(body=CHALK, belly=PURE, mane=SILVER, tail_col=SILVER, snout=PINK, ears=CHALK, legs_col=PURE, neck_up=62, tail=56)
    mv = dict(stride=24, wag=14, bob=6, rate=1.5, phase=0.0, orbit=(0, 0, 0.07))
    beast(sc, 330, 0, 60, 180, 1.25, horse, blanket=MINT8, band=GOLD, **mv)
    pl = rider(sc, 330, 0, 60, 180, 1.25, 120.0, mv)                 # the plume and the brow band
    a = math.radians(62)
    ny, nz = -75 + 42 * 0.25 - math.cos(a) * 50, 120 - 42 * 0.35 + math.sin(a) * 50
    tilt_box(pl, GOLD, (0, ny - 4, nz + 18), (0, ny + 6, nz + 50), 8, 8)
    pl.box(GOLD, (0, ny - 8, nz + 12), (22, 5, 6))
    P.figure(sc, 0, 0, 110, yaw=-90, s=1.1, skin=P.SKINS[2], top=MINT8, bottom=NAVY, shoes=P.SHOES[1], hair='bun', hair_col=P.HAIRS[1], sleeves='long',
             band=GOLD, arm_l='out', arm_r='hip', detail=1, rate=0.07, phase=0.0, swing_l=10, orbit=(0, 0, 0.07))
    g.box(NAVY, (-330, -330, 64), (60, 60, 12), rot=(0, 45, 0))      # the hoop on its stand
    g.box(NAVY, (-330, -330, 110), (6, 6, 90), rot=(0, 45, 0))
    for k in range(10):
        a0, a1 = math.radians(k * 36), math.radians(k * 36 + 36)
        c45 = math.cos(math.radians(45))
        tilt_box(g, GOLD, (-330 + math.cos(a0) * 70 * c45, -330 - math.cos(a0) * 70 * c45, 226 + math.sin(a0) * 70),
                 (-330 + math.cos(a1) * 70 * c45, -330 - math.cos(a1) * 70 * c45, 226 + math.sin(a1) * 70), 7, 7)


# ---------------------------------------------------------------------------------------------- 15 cranes
def cranes(sc):
    """White cranes on a pale stone terrace with a strip of pool, 12 x 4.6 m, 0.5 m up: two dance (hopping, wings
    beating), one bows to the water, one stands on one leg, one calls with its neck up, a grey-brown chick hops; reeds."""
    g = sc.still()
    g.box(PALE_L, (0, 0, -100), (1200, 460, 300))                    # the terrace: 0.5 m
    g.box(SNOW, (0, -233, 30), (1204, 8, 30))
    g.plan(POOL, stadium(470, 90), 48, 56, loc=(-60, -90, 0))
    g.box(PALE_L, (380, 150, 70), (300, 150, 50), rot=(0, 8, 0))     # a higher stone at the back
    for k, (x, y, h) in enumerate(((-560, 180, 170), (-520, 196, 210), (-480, 176, 150), (-120, 200, 190), (-80, 186, 160), (540, 60, 180), (566, 90, 140))):
        reed(sc, x, y, 50, h, phase=0.13 * k)
    kw = dict(body=PURE, wing=SILVER, leg=ASH, bill=SULPH, legl=80.0, neck=70.0, crest=PINK)
    wader(sc, -330, 60, 50, -40, 1.5, flap=26, bob=16, rate=1.1, phase=0.0, **kw)
    wader(sc, -150, 110, 50, 190, 1.5, flap=22, bob=12, rate=1.1, phase=0.45, **kw)
    wader(sc, 60, -90, 50, -100, 1.45, down=True, dip=7, rate=0.4, phase=0.2, **kw)
    wader(sc, 250, -70, 50, 30, 1.5, one_leg=True, dip=4, rate=0.25, phase=0.6, **kw)
    wader(sc, 400, 150, 95, -75, 1.5, dip=5, rate=0.6, phase=0.8, **kw)
    wader(sc, -470, -60, 50, 20, 0.8, bob=8, rate=1.6, phase=0.3, body=BUFF, wing=PALE_L, leg=ASH, bill=SULPH, legl=60.0, neck=40.0)


# ---------------------------------------------------------------------------------------------- 16 the cranes' skein (sky)
def crane_skein(sc):
    """Seven white cranes in a V, necks out and legs trailing, flying ALONG the road (the mesh's X) 140 m and round
    again; nothing circles. Saved far=True (it is placed 150 m and more out)."""
    L, v = 14000.0, 900.0
    spots = [(600, 0, 300)] + [(600 - 250 * k, side * 210 * k, 300 + 26 * k) for k in (1, 2, 3) for side in (-1, 1)]
    for k, (x, y, z) in enumerate(spots):
        rate = 1.25 + 0.06 * (k % 3)
        P.bird(sc, x, y, z, heading=0, span=330, body=PURE, wing=SILVER, beak=SULPH, rate=rate, flap=24, phase=0.0, travel=(L, v))
        m = sc.part(P.motion(pivot=(x, y, z), phase=0.0, rate=rate, base=(x, y), travel=(L, v / L)))
        m.base = None
        tilt_box(m, PURE, (x + 60, y, z + 12), (x + 150, y, z + 22), 12, 12)       # the neck out ahead
        m.box(PINK, (x + 150, y, z + 30), (16, 10, 6))
        tilt_box(m, ASH_L, (x - 60, y, z), (x - 190, y, z - 10), 9, 7)             # the legs trailing


# ---------------------------------------------------------------------------------------------- 17 pink waders
def pink_waders(sc):
    """Pink wading birds in a white-crusted mineral pool, 11 x 4.4 m, 0.4 m up: seven stand in it, three on one leg,
    three with their heads down in the water, a grey chick hops on the crust; steam, thrift cushions."""
    g = sc.still()
    g.plan(MILK, stadium(550, 220), -100, 34)
    g.plan(CREAMY, stadium(500, 180), 30, 40)
    g.plan(POOL, stadium(470, 150), 34, 45)
    g.plan(POOL_D, stadium(300, 70), 36, 47, loc=(40, 10, 0))
    for k, (x, y) in enumerate(((-470, -150), (-200, -196), (220, -196), (460, -140))):
        g.box(MILK, (x, y, 0), (160, 50, 50))
        g.sphere(MOSS, (x + 30, y - 4, 30), 20, scale=(1.2, 1.0, 0.6), steps=(3, 6))
        g.box(PINK, (x + 30, y - 4, 44), (10, 10, 7))
    g.box(SKY, (-520, 150, 90), (8, 8, 120))                         # a marker post with a lemon cap
    g.box(CREAMY, (-520, 150, 154), (16, 16, 10))
    kw = dict(body=PINK, wing=ROSE, leg=ROSE, bill=PANE, legl=70.0, neck=62.0)
    birds = ((-360, 20, -50, 0), (-220, -70, 200, 1), (-90, 60, -120, 2), (60, -40, 30, 1), (200, 70, -80, 0), (330, -50, 160, 2), (420, 60, -100, 0))
    for k, (x, y, yaw, how) in enumerate(birds):
        wader(sc, x, y, 38, yaw, 1.3, one_leg=(how == 0), down=(how == 2), dip=(8 if how == 2 else 5), rate=0.3 + 0.06 * k, phase=0.15 * k, **kw)
    wader(sc, -470, 120, 40, 60, 0.7, bob=8, rate=1.5, phase=0.5, body=HUT, wing=CONC, leg=ASH_L, bill=PANE, legl=44.0, neck=30.0)
    for k, (x, y) in enumerate(((-280, 90), (0, -100), (260, -20))):
        puff(sc, x, y, 100, 26, 18, 0.15 + 0.03 * k, 0.3 * k)


# ---------------------------------------------------------------------------------------------- 18 moths (sky, low)
def _moth(sc, cx, cy, r, ang, z, rps, s=1.0, body=ICE, wing=LILAC, spot=MAG, rate=1.3, phase=0.0, bob=30.0):
    a = math.radians(ang)
    x, y = cx + r * math.cos(a), cy + r * math.sin(a)
    hd = ang + (90.0 if rps >= 0 else -90.0)
    f = Fr(sc, x, y, z, hd, s, rate=rate, phase=phase, bob=-bob, **ring(rps, cx, cy))
    b = f.part(pivot=(0, 0, 0))
    b.box(body, (0, 0, 0), (34, 12, 10))
    b.box(wing, (20, 0, 2), (10, 14, 10))
    for side in (-1, 1):
        tilt_box(b, body, (24, side * 4, 6), (40, side * 14, 16), 4, 4)
        w = f.part(side * 38, (0, side * 5, 3), 0.0)
        w.box(wing, (8, side * 30, 4), (30, 48, 4))
        w.box(body, (-14, side * 22, 4), (22, 34, 4))
        w.box(spot, (10, side * 38, 5), (10, 10, 4))


def moth_cloud(sc):
    """Pale lilac moths drifting in a slow cloud 2.5 to 5 m up as built: twelve, each on its own small circle (3.3 m at
    the widest), floating up and down, wings beating slowly. Nothing dark, nothing fast."""
    spots = ((-380, -60, 150, 0, 260, 0.05), (-380, -60, 300, 140, 380, 0.04), (-300, 120, 220, 250, 470, -0.045), (-120, -40, 180, 60, 300, -0.05),
             (-80, 60, 320, 200, 430, 0.035), (60, -100, 240, 320, 270, 0.055), (100, 80, 160, 100, 500, -0.04), (240, -20, 300, 20, 350, 0.04),
             (300, 100, 200, 220, 440, -0.05), (420, -80, 260, 280, 290, 0.045), (0, 0, 330, 170, 520, -0.03), (-200, 0, 120, 300, 340, 0.06))
    for k, (cx, cy, r, ang, z, rps) in enumerate(spots):
        _moth(sc, cx, cy, r, ang, z, rps, 1.0, wing=LILAC if k % 3 else ICE, body=ICE if k % 3 else LILAC, spot=MAG if k % 2 else VIOLET,
              rate=1.1 + 0.07 * (k % 4), phase=0.083 * k, bob=24 + 4 * (k % 3))


# ---------------------------------------------------------------------------------------------- 19 glass iguanas
def glass_iguanas(sc):
    """Great teal iguanas basking on an obsidian outcrop, 11 x 6 m, 4.6 m at its tip: one on the top slab does slow
    push-ups, one on the middle step nods and sways its tail, one comes head-down over the low slab, a young one on
    the ash; violet and lilac glints on the glass, lilac lichen."""
    g = sc.still()
    g.box(ASH_L, (0, 0, -120), (1100, 600, 260))                     # the ash pad: 0.1 m
    g.box(BASALT_L, (-180, 60, 40), (520, 380, 320), rot=(0, 8, 0))  # the top slab: 2 m
    g.box(ASH, (-180, 60, 203), (480, 340, 6), rot=(0, 8, 0))
    g.box(BASALT_L, (240, 20, 0), (420, 360, 240), rot=(0, -10, 0))  # the middle step: 1.2 m
    g.box(ASH, (240, 20, 123), (380, 320, 6), rot=(0, -10, 0))
    g.box(BASALT_L, (-420, -180, -20), (300, 200, 150), rot=(0, 20, 0))    # the low slab: 0.55 m
    tilt_box(g, BASALT, (-330, 210, 0), (-400, 250, 460), 150, 60)   # the shards behind
    tilt_box(g, VIOLET, (-316, 180, 10), (-384, 222, 440), 10, 10)
    tilt_box(g, BASALT, (60, 250, 0), (90, 270, 360), 120, 50)
    tilt_box(g, LILAC, (40, 226, 10), (72, 246, 340), 10, 10)
    tilt_box(g, BASALT, (430, 200, 0), (480, 210, 250), 100, 44)
    tilt_box(g, MAG, (414, 178, 10), (462, 188, 236), 8, 8)
    for k, (x, y, z, w) in enumerate(((-300, -40, 208, 70), (-60, 150, 208, 56), (300, 110, 128, 60), (140, -110, 128, 44), (-440, -200, 57, 50), (440, -220, 12, 60))):
        g.box(LILAC if k % 2 else ICE, (x, y, z), (w, w * 0.7, 6), rot=(0, 25 * k, 0))
    lizard(sc, -180, 40, 206, -30, 3.0, nod=8, wag=7, bob=14, rate=0.7, phase=0.0)
    lizard(sc, 250, 10, 126, 200, 2.7, body=TEAL, crest=TURQ, nod=12, wag=12, rate=0.4, phase=0.4)
    lizard(sc, -420, -190, 58, -80, 2.4, nod=10, wag=14, rate=0.3, phase=0.7)
    lizard(sc, 420, -190, 12, 150, 1.4, body=TEAL, crest=TURQ, nod=12, wag=16, bob=6, rate=1.3, phase=0.2)


# ---------------------------------------------------------------------------------------------- the kit
LOW = dict(grow=(20, 20, 10), grow_down=(20, 20, 0))                 # low pieces: footing 0.4 m + top under 2 m + 0.1 m: bounds under 2.5 m
JOBS = [
    ('SM_S1ANIMALS_Egrets', egrets, LOW),
    ('SM_S1ANIMALS_Geese', geese, LOW),
    ('SM_S1ANIMALS_Frogs', frogs, LOW),
    ('SM_S1ANIMALS_Hens', hens, LOW),
    ('SM_S1ANIMALS_KoiBasin', koi_basin, LOW),
    ('SM_S1ANIMALS_Peacocks', peacocks, LOW),
    ('SM_S1ANIMALS_ShardLizard', shard_lizard, LOW),
    ('SM_S1ANIMALS_StudioPets', studio_pets, LOW),
    ('SM_S1ANIMALS_SpringMonkeys', spring_monkeys, dict(grow=(30, 30, 40), grow_down=(30, 30, 0))),
    ('SM_S1ANIMALS_PitPonies', pit_ponies, dict(grow=(30, 30, 20), grow_down=(30, 30, 0))),
    ('SM_S1ANIMALS_PackBeasts', pack_beasts, dict(grow=(130, 30, 20), grow_down=(130, 30, 0))),
    ('SM_S1ANIMALS_CurlHornSheep', curl_horn_sheep, dict(grow=(30, 30, 20), grow_down=(30, 30, 0))),
    ('SM_S1ANIMALS_GreatTortoise', great_tortoise, dict(grow=(40, 60, 20), grow_down=(40, 60, 0))),
    ('SM_S1ANIMALS_WhiteHorse', white_horse, dict(grow=(30, 30, 20), grow_down=(30, 30, 0))),
    ('SM_S1ANIMALS_Cranes', cranes, dict(grow=(40, 40, 40), grow_down=(40, 40, 0))),
    ('SM_S1ANIMALS_CraneSkein', crane_skein, dict(far=True, grow=(6500, 60, 80), grow_down=(6900, 60, 60))),
    ('SM_S1ANIMALS_PinkWaders', pink_waders, dict(grow=(30, 30, 30), grow_down=(30, 30, 0))),
    ('SM_S1ANIMALS_MothCloud', moth_cloud, dict(grow=(360, 360, 50), grow_down=(360, 360, 50))),
    ('SM_S1ANIMALS_GlassIguanas', glass_iguanas, dict(grow=(30, 30, 30), grow_down=(30, 30, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
