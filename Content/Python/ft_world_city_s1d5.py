"""City Center, Stage 1, builder s1d5: district 5 "Wall Street" (4.62 to 6.29 km), the money district.
/Game/World/CityCenter/SM_S1D5_*.   Run: import ft_world_city_s1d5 as K; K.run()

Sandstone temple fronts and stepped ziggurat towers, giant column orders, pediments, verdigris copper roofs and
lanterns; urns, stone pineapples and obelisks for finials (no eagles, no bull, no bear, no gold-and-black).
Palette (the kit's own hexes): sand #b89a6a, cream #fff1d6, tan #8a6f5c, copper green #2f6b52, dark green #24403a,
warm window #ffe28a; paper white #fff6ea and cart wood #9a6a44 on three small props.

THE CANYON (FTDressL7.inl, FTTrack.cpp DefaultProps): the ground 23 to 82 m out is the level's own towers, so
  - GAP TOWERS (PostTower, CountingTower, VaultDrum, LanternTower) are slender (9 m and less across) and stand ON THE
    AXIS OF THE GAP between two podiums (22.6 m along from the slot's anchor, 43 m out): the level's first-row towers
    stand within 11.3 m of a slot's anchor and are about 20 m across, so that axis is the one line they never reach.
    They are seen down the gap over the 34 m infill: what tells is drawn from 46 m up;
  - ROW TOWERS (ExchangeTower, CoinPressHall, ClerksBlock, ZigguratMinor) stand 54 to 70 m out among the second row and
    read by their tops, 93 to 105 m up;
  - ROOF PIECES stand on the free ENDS of the podiums' roofs (bank 10.4 m, dear doors 9.6 m, yard 6.64 m);
  - KERB PIECES are low and stand behind the parked cars (19.9 to 22.5 m out) on the four level, straight slots;
  - THE GREAT SCALES (hero) stands 165 m out, dead ahead of the street at two bends.
Every piece: ft_people.Scene, one section, M_People (the hero M_PeopleFar: its beam must tip at any distance).
Front faces -Y, base on z = 0 (towers: 3 m of footing), X along the road, cm. No figure, no animal, no word.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/CityCenter'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))
SAND, CREAM, TAN, COP, COPD, WARM = '#b89a6a', '#fff1d6', '#8a6f5c', '#2f6b52', '#24403a', '#ffe28a'
WHITE, WOOD = '#fff6ea', '#9a6a44'
GW, GD = 0.6, 0.9          # a lit window, a lit doorway or lantern (steady; only on pieces stood 24 m and more out)


# ------------------------------------------------------------------------------------------ helpers
def _ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def gable(g, col, w, z, rise, y0, y1, x=0.0, glow=0.0):
    """A pediment or a pitched roof: a triangle w wide on z, its ridge running across y0..y1."""
    g.side(col, _ccw([(x - w / 2.0, z), (x + w / 2.0, z), (x, z + rise)]), y0, y1, glow=glow)


def tier(g, w, d, z0, z1, n, r, m=None, floors=0, col=SAND, band=CREAM, wz0=None, lit=0.78):
    """One storey block of a tower: the wall, a cream cornice, tall window strips between piers on the front and both
    ends (n across the front, m on each end), `floors` sandstone spandrels across them."""
    g.box(col, (0, 0, (z0 + z1) / 2.0), (w, d, z1 - z0))
    g.box(band, (0, 0, z1 - 28), (w + 56, d + 56, 56))
    a = z0 + 120 if wz0 is None else wz0
    b = z1 - 150
    h, zc = b - a, (a + b) / 2.0
    if n:
        pw = float(w) / n
        for i in range(n):
            on = r.random() < lit
            g.box(WARM if on else COPD, (-w / 2.0 + (i + 0.5) * pw, -d / 2.0 - 3, zc), (pw * 0.46, 8, h), glow=GW if on else 0.0)
    if m is None:
        m = max(1, int(round(n * float(d) / w)))
    if m:
        pd = float(d) / m
        for sx in (-1, 1):
            for i in range(m):
                on = r.random() < lit
                g.box(WARM if on else COPD, (sx * (w / 2.0 + 3), -d / 2.0 + (i + 0.5) * pd, zc), (8, pd * 0.46, h), glow=GW if on else 0.0)
    for k in range(floors):
        g.box(col, (0, 0, a + (k + 1) * h / (floors + 1.0)), (w + 22, d + 22, 50))


def obelisk(g, x, y, z, w, h, col=CREAM, tip=COP):
    g.cyl(col, (x, y, z), w * 0.707, h * 0.8, steps=4, top_r=w * 0.4, rot=(0, 45, 0))
    g.cyl(tip, (x, y, z + h * 0.8), w * 0.4, h * 0.2, steps=4, top_r=2.0, rot=(0, 45, 0))


def urn(g, x, y, z, s=1.0, col=CREAM, lid=COP):
    g.cyl(col, (x, y, z), 16 * s, 12 * s, steps=6)
    g.cyl(col, (x, y, z + 12 * s), 9 * s, 34 * s, steps=6, top_r=24 * s)
    g.cyl(lid, (x, y, z + 46 * s), 24 * s, 18 * s, steps=6, top_r=5 * s)


def pineapple(g, x, y, z, s=1.0):
    g.sphere(SAND, (x, y, z + 30 * s), 23 * s, scale=(1, 1, 1.35), steps=(4, 6))
    g.cyl(COP, (x, y, z + 50 * s), 22 * s, 12 * s, steps=5, top_r=8 * s)
    g.cyl(COP, (x, y, z + 56 * s), 13 * s, 30 * s, steps=5, top_r=3 * s)


def pyramid(g, col, z, half, h, top=4.0, x=0.0, y=0.0):
    g.cyl(col, (x, y, z), half * 1.414, h, steps=4, top_r=top, rot=(0, 45, 0))


def vane(sc, x, y, z, s, kind, phase):
    """A copper weather vane: the mast stands, the arm swings slowly to and fro about it (never round and round)."""
    g = sc.still()
    g.cyl(COPD, (x, y, z), 5 * s, 130 * s, steps=5)
    g.sphere(COP, (x, y, z + 134 * s), 11 * s, steps=(3, 6))
    v = sc.part(P.motion(swing=30, pivot=(x, y, z), axis=P.UP, rate=0.11, phase=phase))
    zz = z + 100 * s
    v.box(COP, (x, y, zz), (150 * s, 5 * s, 7 * s))
    v.box(COP, (x - 62 * s, y, zz), (30 * s, 5 * s, 40 * s))
    if kind == 'coin':                                              # a blank disc
        rod(v, CREAM, (x + 72 * s, y - 3 * s, zz), (x + 72 * s, y + 3 * s, zz), 20 * s, steps=8)
    elif kind == 'envelope':                                        # a letter: the pneumatic post
        v.box(CREAM, (x + 66 * s, y, zz), (48 * s, 6 * s, 32 * s))
        v.box(COP, (x + 66 * s, y, zz + 8 * s), (48 * s, 7.5 * s, 5 * s))
    else:                                                           # a pennant
        v.box(CREAM, (x + 60 * s, y, zz + 12 * s), (58 * s, 5 * s, 24 * s))
        v.box(COPD, (x + 60 * s, y, zz + 12 * s), (10 * s, 6.5 * s, 24 * s))


def corner_obelisks(g, w, d, z, s, h):
    for sx in (-1, 1):
        for sy in (-1, 1):
            obelisk(g, sx * (w / 2.0 - s * 0.6), sy * (d / 2.0 - s * 0.6), z, s, h)


# ------------------------------------------------------------------------------------------ THE HERO
def great_scales(sc):
    """THE GREAT SCALES: a sandstone ziggurat of seven set-backs, 104 m, a temple front with six giant columns at its
    foot, warm windows up every step, verdigris terraces; on its top a copper-green balance, pillar to 127 m, a beam
    50 m long that tips 6 degrees each way once in ten seconds, a pan on three chains at each end riding up and down
    with it: three blank cream discs in one, a sandstone block in the other. 131 m to the finial."""
    r = random.Random(51)
    g = sc.still()
    steps = [(7200, -300, 2600, 8, 4, 3), (6200, 2600, 4600, 7, 4, 2), (5200, 4600, 6200, 6, 3, 2), (4300, 6200, 7600, 5, 3, 1),
             (3400, 7600, 8700, 4, 2, 1), (2600, 8700, 9600, 3, 2, 1), (1800, 9600, 10400, 2, 1, 0)]
    for k, (w, z0, z1, n, m, fl) in enumerate(steps):
        tier(g, w, w, z0, z1, n, r, m=m, floors=fl, wz0=(1900 if k == 0 else None), lit=0.85)
        g.box(COP, (0, 0, z1 + 8), (w - 260, w - 260, 16))                         # the terrace: weathered copper
        if k in (0, 2, 4):
            corner_obelisks(g, w, w, z1, 300 - k * 40, 1300 - k * 180)
    # the temple front at the foot: six giant columns three storeys tall, a pediment with a blank roundel, a deep door
    y = -3600
    g.box(CREAM, (0, y - 260, 60), (3600, 520, 120))
    g.box(CREAM, (0, y - 200, 150), (3300, 400, 60))
    for k in range(6):
        x = -1400 + k * 560
        g.cyl(CREAM, (x, y - 250, 180), 125, 1380, steps=8)
        g.box(CREAM, (x, y - 250, 1590), (330, 330, 60))
    g.box(CREAM, (0, y - 250, 1710), (3500, 440, 180))
    gable(g, CREAM, 3600, 1800, 620, y - 470, y - 30)
    gable(g, COP, 3700, 1810, 640, y - 30, y)
    rod(g, COP, (0, y - 480, 2010), (0, y - 466, 2010), 150, steps=12)
    g.box(WARM, (0, y - 6, 620), (560, 12, 1100), glow=GD)
    rod(g, WARM, (0, y - 12, 1170), (0, y, 1170), 280, steps=12, glow=GD)
    for sx in (-1, 1):
        g.box(WARM, (sx * 1960, y - 4, 900), (300, 8, 900), glow=GW)
        g.box(SAND, (sx * 2900, y - 160, 500), (900, 320, 1000))                  # two flanking lodges under copper roofs
        gable(g, COP, 1000, 1000, 260, y - 330, y + 10, x=sx * 2900)
        g.box(WARM, (sx * 2900, y - 324, 480), (360, 8, 520), glow=GW)
        urn(g, sx * 1700, y - 420, 120, s=6.0)
    # the balance
    g.box(CREAM, (0, 0, 10520), (760, 760, 240))
    g.box(COPD, (0, 0, 10700), (520, 520, 120))
    g.cyl(COPD, (0, 0, 10760), 120, 1900, steps=8, top_r=80)
    g.cyl(COP, (0, 0, 11300), 170, 90, steps=8)
    g.cyl(COP, (0, 0, 12000), 150, 80, steps=8)
    g.sphere(COP, (0, 0, 12700), 150, steps=(4, 8))
    obelisk(g, 0, 0, 12820, 150, 300, col=COP, tip=CREAM)
    A, ZB, PH = 6.0, 12600.0, 0.0
    b = sc.part(P.motion(swing=-A, pivot=(0, 0, ZB), axis=90.0, rate=0.1, phase=PH))
    b.box(COP, (0, 0, ZB), (5000, 150, 170))
    b.box(COPD, (0, 0, ZB + 30), (3000, 180, 260))
    b.box(COP, (0, 0, ZB + 60), (1300, 210, 360))
    b.box(CREAM, (0, -125, ZB - 380), (60, 30, 600))                               # the pointer, hanging before the pillar
    for sx in (-1, 1):
        b.sphere(COPD, (sx * 2500, 0, ZB), 130, steps=(4, 8))
    lift = 2500.0 * math.sin(math.radians(A))
    for sx in (-1, 1):
        p = sc.part(P.motion(bob=-lift, rate=0.1, phase=PH if sx > 0 else PH + 0.5))
        x, zp = sx * 2500.0, ZB - 1500.0
        for a in (90, 210, 330):
            ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
            tilt_box(p, COPD, (x, 0, ZB - 110), (x + ca * 560, sa * 560, zp + 150), 16, 16)
        p.cyl(COP, (x, 0, zp), 190, 150, steps=12, top_r=620)
        p.cyl(COPD, (x, 0, zp + 150), 640, 36, steps=12)
        if sx > 0:
            for k in range(3):
                p.cyl(CREAM, (x + (k - 1) * 60, (k % 2) * 50 - 20, zp + 186 + k * 74), 300, 70, steps=12)
                p.cyl(COP, (x + (k - 1) * 60, (k % 2) * 50 - 20, zp + 186 + k * 74 + 70), 230, 4, steps=12)
        else:
            p.box(SAND, (x, 0, zp + 186 + 150), (420, 420, 300))
            p.box(CREAM, (x, 0, zp + 186 + 310), (300, 300, 24))
            p.cyl(COPD, (x, 0, zp + 186 + 320), 46, 90, steps=6)


# ------------------------------------------------------------------------------------------ GAP TOWERS (slender; 43 m out)
def post_tower(sc):
    """The pneumatic-post tower: a thin sandstone shaft wound round with copper tubes, three despatch boxes lit warm
    on its face, a broad flattened copper bulb on top (wider than the shaft), a letter for its weather vane. 80 m."""
    r = random.Random(52)
    g = sc.still()
    tier(g, 600, 600, -300, 4400, 2, r, m=2, floors=4)
    tier(g, 520, 520, 4400, 6600, 2, r, m=2, floors=2)
    g.box(CREAM, (0, 0, 6660), (720, 720, 120))
    n, turns, R = 36, 6.0, 470.0
    pts = []
    for i in range(n + 1):
        a = 2.0 * math.pi * turns * i / n - math.pi / 2.0
        pts.append((R * math.cos(a), R * math.sin(a), 700.0 + (6500.0 - 700.0) * i / n))
    for i in range(n):
        rod(g, COP, pts[i], pts[i + 1], 28, steps=5)
    for sx in (-1, 1):
        g.cyl(COPD, (sx * 170, -322, -100), 22, 6760, steps=5)
    for z in (4700, 5500, 6250):
        g.box(COPD, (0, -310, z), (250, 110, 210))
        g.box(WARM, (0, -368, z), (150, 8, 100), glow=GD)
    g.cyl(COPD, (0, 0, 6720), 210, 300, steps=8)
    g.sphere(COP, (0, 0, 7230), 430, scale=(1, 1, 0.6), steps=(5, 10))
    g.cyl(WARM, (0, 0, 7200), 436, 60, steps=10, glow=GD)
    g.cyl(COPD, (0, 0, 7470), 170, 150, steps=8, top_r=60)
    vane(sc, 0, 0, 7610, 2.6, 'envelope', 0.2)


def counting_tower(sc):
    """The counting house: a slender tower of four set-backs, a giant abacus (three rails of cream and copper beads)
    across the second set-back, an obelisk for its finial. 87 m."""
    r = random.Random(53)
    g = sc.still()
    tier(g, 860, 760, -300, 3600, 3, r, m=3, floors=3)
    tier(g, 720, 640, 3600, 5600, 3, r, m=2, floors=2)
    tier(g, 580, 520, 5600, 7000, 2, r, m=2, floors=1, wz0=6260)
    tier(g, 420, 400, 7000, 7800, 1, r, m=1)
    corner_obelisks(g, 860, 760, 3600, 110, 520)
    corner_obelisks(g, 580, 520, 7000, 80, 380)
    for sx in (-1, 1):
        g.box(COPD, (sx * 300, -302, 5890), (40, 40, 580))
        g.sphere(COP, (sx * 300, -302, 6200), 36, steps=(3, 6))
    cols = (CREAM, COP, CREAM, COP, CREAM)
    for j, z in enumerate((5720, 5900, 6080)):
        g.box(COPD, (0, -302, z), (600, 12, 12))
        for k in range(5):
            x = (-232, -130, -28, 110, 214)[k] if j != 1 else (-214, -110, 28, 130, 232)[k]
            g.sphere(cols[(k + j) % 5], (x, -302, z), 44, scale=(0.85, 1, 1), steps=(3, 6))
    g.box(CREAM, (0, 0, 7860), (330, 320, 120))
    obelisk(g, 0, 0, 7920, 250, 800)


def vault_drum(sc):
    """The vault house: a windowless sandstone drum banded in cream, a great round copper-green door high on its face
    standing half open on warm light, a copper stair winding up its back, a shallow copper dome and a stone pineapple. 73 m."""
    g = sc.still()
    g.box(TAN, (0, 0, 150), (900, 900, 900))
    g.cyl(SAND, (0, 0, -300), 440, 6700, steps=12)
    for z in (1500, 3000, 4400, 6330):
        g.cyl(CREAM, (0, 0, z), 462, 70, steps=12)
    for k in range(12):
        a = math.radians(k * 30 + 15)
        g.box(COP, (math.cos(a) * 446, math.sin(a) * 446, 6200), (46, 46, 46), rot=(0, k * 30 + 15, 0))
    zc = 5550.0
    rod(g, COPD, (0, -400, zc), (0, -470, zc), 410, steps=14)
    rod(g, CREAM, (0, -400, zc), (0, -452, zc), 470, steps=14)
    rod(g, WARM, (0, -466, zc), (0, -478, zc), 335, steps=14, glow=GD)
    a = math.radians(62.0)
    dx, dy = math.cos(a), -math.sin(a)             # along the open leaf
    nx, ny = math.sin(a), math.cos(a)              # across it
    cx, cy = -345 + 335 * dx, -480 + 335 * dy
    rod(g, COP, (cx - nx * 36, cy - ny * 36, zc), (cx + nx * 36, cy + ny * 36, zc), 335, steps=14)
    rod(g, COPD, (cx - nx * 44, cy - ny * 44, zc), (cx - nx * 36, cy - ny * 36, zc), 260, steps=14)
    rod(g, CREAM, (cx - nx * 110, cy - ny * 110, zc), (cx - nx * 44, cy - ny * 44, zc), 62, steps=8)
    yaw = math.degrees(math.atan2(dy, dx))
    for size in ((520, 16, 44), (44, 16, 520)):
        g.box(CREAM, (cx - nx * 52, cy - ny * 52, zc), size, rot=(0, yaw, 0))
    g.box(COPD, (-365, -470, zc + 180), (70, 70, 110))
    g.box(COPD, (-365, -470, zc - 180), (70, 70, 110))
    for k in range(22):                                                    # a copper stair winding up the back of the drum
        deg = 310.0 + k * 11.0
        g.box(COP, (math.cos(math.radians(deg)) * 484, math.sin(math.radians(deg)) * 484, 4350 + k * 88), (120, 64, 14), rot=(0, deg, 0))
    g.cyl(CREAM, (0, 0, 6400), 490, 110, steps=12)
    g.sphere(COP, (0, 0, 6510), 440, scale=(1, 1, 0.55), steps=(5, 12))
    g.cyl(CREAM, (0, 0, 6730), 70, 60, steps=6)
    pineapple(g, 0, 0, 6790, 5.6)


def lantern_tower(sc):
    """The lantern: a square sandstone shaft, a balcony with four obelisks, an eight-sided copper lantern lit warm,
    an ogee copper cap and a pennant vane. 76 m."""
    r = random.Random(54)
    g = sc.still()
    tier(g, 700, 700, -300, 3800, 3, r, m=3, floors=3)
    tier(g, 600, 600, 3800, 5800, 2, r, m=2, floors=2, wz0=4500)
    g.box(CREAM, (0, 0, 5830), (840, 840, 60))
    corner_obelisks(g, 840, 840, 5860, 90, 420)
    for k in range(4):                                                     # a copper rail on balusters round the balcony
        ca, sa = math.cos(math.radians(k * 90)), math.sin(math.radians(k * 90))
        g.box(COP, (ca * 400, sa * 400, 5940), (8, 640, 8), rot=(0, k * 90, 0))
        for j in (-1, 0, 1):
            g.box(COPD, (ca * 400 - sa * j * 190, sa * 400 + ca * j * 190, 5900), (10, 10, 80))
    g.cyl(CREAM, (0, 0, 5860), 270, 200, steps=8)
    g.cyl(WARM, (0, 0, 6060), 226, 540, steps=8, glow=GD)
    for k in range(8):
        a = math.radians(k * 45 + 22.5)
        g.box(COP, (math.cos(a) * 240, math.sin(a) * 240, 6330), (44, 44, 540), rot=(0, k * 45 + 22.5, 0))
    g.cyl(COP, (0, 0, 6330), 236, 40, steps=8)
    g.cyl(CREAM, (0, 0, 6600), 290, 80, steps=8)
    g.cyl(COP, (0, 0, 6680), 300, 240, steps=8, top_r=110)
    g.cyl(COP, (0, 0, 6920), 110, 330, steps=8, top_r=24)
    g.sphere(COPD, (0, 0, 7270), 44, steps=(3, 6))
    vane(sc, 0, 0, 7290, 2.2, 'pennant', 0.55)


# ------------------------------------------------------------------------------------------ ROW TOWERS (54 to 70 m out)
def exchange_tower(sc):
    """The exchange: a stepped sandstone tower carrying a whole temple on its top: eight fat columns across the
    front, a pediment with a blank roundel, a copper roof, a copper lantern lit warm, a coin vane. 104 m."""
    r = random.Random(55)
    g = sc.still()
    tier(g, 1300, 1000, -300, 3600, 6, r, m=4, floors=3)
    tier(g, 1150, 880, 3600, 6200, 5, r, m=3, floors=2)
    tier(g, 1000, 760, 6200, 7800, 4, r, m=3, floors=1)
    for k in range(5):                                                     # giant half-columns on the piers of the third stage
        g.cyl(CREAM, (-500 + k * 250, -392, 6200), 34, 1544, steps=6)
    for sx in (-1, 1):
        for sy in (-1, 1):
            urn(g, sx * 530, sy * 400, 6200, s=2.6)
    g.box(CREAM, (0, 0, 7860), (1120, 860, 120))
    g.box(SAND, (0, 60, 8470), (760, 520, 1100))
    g.box(WARM, (0, -203, 8380), (200, 8, 760), glow=GD)
    for k in range(8):
        g.cyl(CREAM, (-483 + k * 138, -350, 7920), 50, 1100, steps=6)
    for sx in (-1, 1):
        for y in (-110, 130, 360):
            g.cyl(CREAM, (sx * 483, y, 7920), 50, 1100, steps=6)
    g.box(CREAM, (0, 0, 9095), (1100, 840, 150))
    gable(g, COP, 1100, 9170, 330, -400, 410)
    gable(g, CREAM, 1150, 9170, 345, -432, -400)
    rod(g, COP, (0, -440, 9290), (0, -430, 9290), 80, steps=10)
    for sx in (-1, 1):
        urn(g, sx * 520, -390, 9170, s=2.4)
    g.box(COPD, (0, 0, 9520), (230, 230, 110))
    g.cyl(WARM, (0, 0, 9575), 88, 250, steps=8, glow=GD)
    for k in range(4):
        a = math.radians(k * 90 + 45)
        g.box(COP, (math.cos(a) * 92, math.sin(a) * 92, 9700), (30, 30, 250), rot=(0, k * 90 + 45, 0))
    g.cyl(COP, (0, 0, 9825), 140, 190, steps=8, top_r=20)
    vane(sc, 0, 0, 10010, 2.4, 'coin', 0.8)


def coin_press(sc):
    """The coin-press hall: a stepped tower, on its top a hall under a copper barrel roof and, standing on its edge on
    the roof, a giant BLANK coin (no face, no word, cream and copper green, still). 93 m."""
    r = random.Random(56)
    g = sc.still()
    tier(g, 1300, 1000, -300, 3400, 6, r, m=4, floors=3)
    tier(g, 1150, 860, 3400, 5800, 5, r, m=3, floors=2)
    tier(g, 1000, 760, 5800, 7000, 4, r, m=3, floors=1)
    corner_obelisks(g, 1300, 1000, 3400, 100, 480)
    corner_obelisks(g, 1150, 860, 5800, 90, 420)
    g.box(SAND, (0, 0, 7350), (900, 700, 700))
    g.box(CREAM, (0, 0, 7690), (950, 750, 50))
    for k in (-1, 0, 1):
        g.box(WARM, (k * 270, -353, 7300), (150, 8, 330), glow=GD)
        rod(g, WARM, (k * 270, -358, 7465), (k * 270, -350, 7465), 75, steps=8, glow=GD)
    rod(g, COP, (-450, 0, 7700), (450, 0, 7700), 350, steps=12)
    for x in (-340, -170, 0, 170, 340):
        rod(g, COPD, (x - 16, 0, 7700), (x + 16, 0, 7700), 359, steps=12)
    for sx in (-1, 1):
        rod(g, CREAM, (sx * 450, 0, 7700), (sx * 480, 0, 7700), 380, steps=12)
    zc, R = 8700.0, 640.0
    g.box(COPD, (0, 0, 8060), (700, 240, 60))
    for sx in (-1, 1):
        tilt_box(g, COPD, (sx * 330, 0, 8070), (sx * 560, 0, 8520), 60, 200)
    rod(g, COPD, (0, -70, zc), (0, 70, zc), R, steps=16)
    rod(g, CREAM, (0, -86, zc), (0, 86, zc), R * 0.85, steps=16)
    rod(g, COP, (0, -94, zc), (0, 94, zc), R * 0.68, steps=16)
    rod(g, CREAM, (0, -102, zc), (0, 102, zc), R * 0.6, steps=16)


def clerks_block(sc):
    """The clerks' block: a tall sandstone slab under a three-stepped copper mansard with round dormers lit warm, four
    cream chimney stacks with copper pots, a pennant on a mast. 92 m."""
    r = random.Random(57)
    g = sc.still()
    tier(g, 1400, 900, -300, 4800, 7, r, m=4, floors=4)
    tier(g, 1280, 800, 4800, 7800, 6, r, m=3, floors=3)
    for k, (w, d, h) in enumerate(((1300, 820, 300), (1200, 720, 300), (1080, 600, 260))):
        g.box(COP if k != 1 else COPD, (0, 0, 7800 + sum((300, 300, 260)[:k]) + h / 2.0), (w, d, h))
    for k in range(5):
        x = -480 + k * 240
        rod(g, CREAM, (x, -432, 7960), (x, -400, 7960), 92, steps=8)
        rod(g, WARM, (x, -440, 7960), (x, -430, 7960), 62, steps=8, glow=GD)
    for sx in (-1, 1):
        for y in (-180, 180):
            rod(g, CREAM, (sx * 640, y, 7960), (sx * 672, y, 7960), 92, steps=8)
            rod(g, WARM, (sx * 670, y, 7960), (sx * 680, y, 7960), 62, steps=8, glow=GD)
    for k in range(4):
        x = -420 + k * 280
        g.box(CREAM, (x, 120, 8800), (110, 110, 420))
        g.cyl(COP, (x, 120, 9010), 34, 90, steps=5, top_r=24)
    g.box(COPD, (0, -40, 8690), (900, 24, 60))
    g.cyl(COPD, (0, -40, 8660), 9, 520, steps=5)
    f = sc.part(P.motion(swing=12, pivot=(0, -40, 9100), axis=P.UP, rate=0.3, phase=0.4))
    f.box(CREAM, (95, -40, 9120), (180, 6, 70))
    f.box(COP, (95, -40, 9120), (60, 7.5, 70))


def ziggurat_minor(sc):
    """A stepped ziggurat tower of five set-backs, obelisks on three of them, copper terraces, a copper pyramid and a
    ball for its top. 104 m."""
    r = random.Random(58)
    g = sc.still()
    steps = [(1300, -300, 3400, 5, 3), (1080, 3400, 5600, 4, 2), (860, 5600, 7300, 3, 1), (640, 7300, 8500, 2, 1), (440, 8500, 9300, 1, 0)]
    for k, (w, z0, z1, n, fl) in enumerate(steps):
        tier(g, w, w, z0, z1, n, r, m=n, floors=fl)
        if k < 4:
            g.box(COP, (0, 0, z1 + 6), (w - 70, w - 70, 12))
        if k < 3:
            corner_obelisks(g, w, w, z1, 100 - k * 14, 460 - k * 70)
    pyramid(g, COP, 9300, 250, 800, top=20)
    g.sphere(COPD, (0, 0, 10130), 44, steps=(3, 6))
    g.cyl(COPD, (0, 0, 10160), 8, 240, steps=5, top_r=4)


# ------------------------------------------------------------------------------------------ ROOF PIECES (on the podiums' free ends)
def copper_cupola(sc):
    """A copper cupola on a sandstone base: four warm windows, a dome, a coin vane. 4.4 m."""
    g = sc.still()
    g.box(SAND, (0, 0, 30), (240, 240, 60))
    g.box(CREAM, (0, 0, 66), (256, 256, 12))
    g.cyl(CREAM, (0, 0, 72), 100, 150, steps=8)
    for k in range(4):
        a = math.radians(k * 90 - 90)
        g.box(WARM, (math.cos(a) * 96, math.sin(a) * 96, 150), (62, 12, 96), rot=(0, k * 90, 0), glow=GD)
    g.cyl(COPD, (0, 0, 222), 112, 14, steps=8)
    g.sphere(COP, (0, 0, 236), 108, scale=(1, 1, 0.8), steps=(4, 8))
    g.cyl(COPD, (0, 0, 312), 24, 34, steps=6)
    vane(sc, 0, 0, 340, 0.8, 'coin', 0.1)


def vent_cowls(sc):
    """Three verdigris ventilator cowls on a sandstone curb, the tall one turning to and fro in the wind; a little
    skylight lit warm from the banking hall below. 3 m."""
    g = sc.still()
    g.box(SAND, (0, 20, 16), (290, 130, 32))
    g.box(CREAM, (0, 20, 36), (300, 140, 8))
    for x, h in ((-100, 150), (105, 110)):
        g.cyl(COPD, (x, 30, 40), 22, h, steps=8)
        g.cyl(COP, (x, 30, 40 + h), 46, 26, steps=8, top_r=14)
        g.cyl(COP, (x, 30, 40 + h - 12), 30, 12, steps=8)
    g.cyl(COPD, (0, 30, 40), 24, 210, steps=8)
    c = sc.part(P.motion(swing=38, pivot=(0, 30, 250), axis=P.UP, rate=0.14, phase=0.3))
    c.cyl(COP, (0, 30, 240), 28, 40, steps=8)
    rod(c, COP, (0, 30, 268), (0, -40, 268), 30, steps=8)
    c.box(COPD, (0, 92, 270), (5, 60, 44))
    g.box(COPD, (0, -62, 46), (170, 60, 12))
    gable(g, WARM, 150, 52, 34, -88, -36, glow=GD)
    for x in (-78, 0, 78):
        g.box(COPD, (x, -62, 66), (6, 60, 44))


def tea_canopy(sc):
    """The clerks' tea terrace: a striped cream and copper-green canopy on four posts over a green carpet, a tea
    trolley with a copper urn, cups and a cake stand, two stools; the valance stirs. 2.7 m."""
    g = sc.still()
    g.box(COP, (0, 0, 3), (340, 210, 6))
    g.box(CREAM, (0, 0, 7), (300, 170, 4))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(COPD, (sx * 152, sy * 90, 130), (8, 8, 250))
    for k in range(5):
        g.box(CREAM if k % 2 == 0 else COP, (-136 + k * 68, 0, 258 + (4 if k % 2 else 0)), (68, 214, 10))
    v = sc.part(P.motion(swing=7, pivot=(0, -107, 256), axis=0.0, rate=0.5, phase=0.2))
    for k in range(5):
        v.box(COP if k % 2 == 0 else CREAM, (-136 + k * 68, -107, 238), (66, 5, 30))
    g.box(WOOD, (40, 10, 74), (96, 50, 6))
    g.box(WOOD, (40, 10, 34), (96, 50, 6))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(COPD, (40 + sx * 44, 10 + sy * 21, 40), (5, 5, 74))
    g.cyl(COP, (16, 10, 77), 14, 34, steps=8)
    g.sphere(COP, (16, 10, 116), 13, steps=(3, 6))
    tilt_box(g, COPD, (16, -4, 92), (16, -22, 100), 5, 5)
    for k in range(3):
        g.cyl(CREAM, (48 + k * 14, -4 + (k % 2) * 16, 77), 6, 8, steps=6)
    g.cyl(CREAM, (70, 16, 77), 16, 4, steps=8)
    g.cyl(COPD, (70, 16, 81), 3, 16, steps=5)
    g.cyl(CREAM, (70, 16, 97), 11, 4, steps=8)
    g.sphere(WARM, (70, 16, 106), 6, steps=(3, 5))
    for x in (-70, -110):
        g.cyl(TAN, (x, -10 + (x + 110) * 0.6, 7), 15, 44, steps=6)
        g.cyl(COP, (x, -10 + (x + 110) * 0.6, 51), 19, 6, steps=6)


def bell_arch(sc):
    """The opening bell: a copper-green bell hung in a sandstone arch under a little pediment; it swings slowly, its
    clapper a beat behind. 4.3 m."""
    g = sc.still()
    g.box(TAN, (0, 0, 10), (330, 110, 20))
    for sx in (-1, 1):
        g.box(SAND, (sx * 125, 0, 170), (52, 76, 300))
        g.box(CREAM, (sx * 125, 0, 324), (62, 86, 10))
    g.box(CREAM, (0, 0, 349), (330, 84, 40))
    gable(g, CREAM, 330, 369, 62, -42, 42)
    gable(g, COP, 346, 372, 66, -34, 34)
    pineapple(g, 0, 0, 436, 0.5)
    b = sc.part(P.motion(swing=14, pivot=(0, 0, 300), axis=0.0, rate=0.28, phase=0.0))
    b.box(COPD, (0, 0, 300), (200, 16, 20))
    b.cyl(COPD, (0, 0, 268), 18, 26, steps=6)
    b.cyl(COP, (0, 0, 160), 64, 110, steps=10, top_r=27)
    b.cyl(COPD, (0, 0, 152), 70, 10, steps=10)
    c = sc.part(P.motion(swing=20, pivot=(0, 0, 262), axis=0.0, rate=0.28, phase=0.14))
    c.box(COPD, (0, 0, 200), (7, 7, 120))
    c.sphere(CREAM, (0, 0, 138), 13, steps=(3, 6))


def parapet_urns(sc):
    """A bank's parapet: a cream kerb 12.6 m long, a copper rail on balusters, and on five pedestals an urn, a stone
    pineapple, an obelisk, a pineapple, an urn. 1.5 m."""
    g = sc.still()
    g.box(CREAM, (0, 0, 6), (1260, 34, 12))
    g.box(COP, (0, 0, 50), (1250, 6, 7))
    for bay in range(4):
        for j in (1, 2, 3):
            g.box(COPD, (-540 + bay * 270 + j * 67.5, 0, 30), (7, 7, 38))
    for k in range(5):
        x = -540 + k * 270
        g.box(SAND, (x, 0, 32), (34, 34, 40))
        g.box(CREAM, (x, 0, 55), (40, 38, 6))
        if k in (0, 4):
            urn(g, x, 0, 58, s=0.9)
        elif k in (1, 3):
            pineapple(g, x, 0, 58, 0.8)
        else:
            obelisk(g, x, 0, 58, 30, 96)


def pennant_mast(sc):
    """A copper mast on a stone ball with three swallow-tail pennants (copper green, cream, sand) stirring. 6.6 m."""
    g = sc.still()
    g.box(SAND, (0, 0, 20), (70, 70, 40))
    g.sphere(CREAM, (0, 0, 66), 30, steps=(4, 6))
    g.cyl(COPD, (0, 0, 90), 6, 550, steps=6)
    g.sphere(COP, (0, 0, 646), 13, steps=(3, 6))
    for k, (z, col) in enumerate(((580, COP), (490, CREAM), (400, SAND))):
        f = sc.part(P.motion(swing=14, pivot=(0, 0, z), axis=P.UP, rate=0.45, phase=0.21 * k))
        f.box(col, (62, 0, z), (112, 5, 44))
        f.box(col, (140, 0, z + 14), (46, 5, 16))
        f.box(col, (140, 0, z - 14), (46, 5, 16))
        f.box(COPD if col != COPD else CREAM, (12, 0, z), (8, 6.5, 44))


# ------------------------------------------------------------------------------------------ KERB PIECES (low, unlit, still)
def urn_row(sc):
    """Four stone urns with copper lids on plinths, a low copper rail between them. 7 m long, 1.6 m."""
    g = sc.still()
    g.box(TAN, (0, 0, 7), (700, 62, 14))
    for k in range(4):
        x = -270 + k * 180
        g.box(SAND, (x, 0, 44), (56, 56, 60))
        g.box(CREAM, (x, 0, 78), (64, 64, 8))
        urn(g, x, 0, 82, s=1.2)
    for k in range(3):
        g.box(COP, (-180 + k * 180, 0, 46), (124, 5, 6))


def tape_bench(sc):
    """A stone bench, a ticker machine under a glass dome on a pedestal, and the paper tape it has run off, down into
    a basket and out over the pavement. 4.3 m long, 1.5 m."""
    g = sc.still()
    g.box(TAN, (0, 0, 4), (430, 110, 8))
    for x in (-185, -45):
        g.box(TAN, (x, 20, 28), (30, 50, 40))
    g.box(CREAM, (-115, 20, 54), (200, 56, 12))
    g.box(SAND, (150, 14, 53), (46, 46, 90))
    g.box(CREAM, (150, 14, 101), (54, 54, 6))
    g.cyl(COPD, (150, 14, 104), 21, 12, steps=8)
    g.sphere(WHITE, (150, 14, 128), 20, scale=(1, 1, 1.15), steps=(4, 8))
    rod(g, COP, (150, -16, 112), (150, -8, 112), 12, steps=8)
    g.cyl(TAN, (62, -8, 8), 22, 42, steps=6, top_r=30)
    g.cyl(COPD, (62, -8, 48), 31, 4, steps=6)
    path = [(150, -10, 116), (128, -16, 84), (92, -14, 58), (66, -8, 52), (40, -22, 54), (20, -34, 14), (-30, -40, 10), (-70, -22, 12),
            (-110, -38, 10), (-150, -28, 14), (-185, -42, 10)]
    for a, b in zip(path, path[1:]):
        tilt_box(g, WHITE, a, b, 8, 4.5)
    for k, (x, y, yaw) in enumerate(((58, -4, 20), (70, -12, 100), (60, -14, 160))):
        g.box(WHITE, (x, y, 50 + k * 4), (30, 8, 4.5), rot=(0, yaw, 0))


def ledger_cart(sc):
    """A hand cart stacked with ledgers (green, tan and cream, one fallen open) and a news rack of pictures: a disc,
    bars, a diamond; no word. 4 m long, 1.6 m."""
    g = sc.still()
    g.box(TAN, (0, 0, 4), (400, 96, 8))
    g.box(WOOD, (60, 0, 52), (180, 80, 10))
    for sy in (-1, 1):
        rod(g, TAN, (40, sy * 42, 36), (40, sy * 50, 36), 30, steps=10)
        rod(g, COPD, (40, sy * 50, 36), (40, sy * 54, 36), 9, steps=6)
        tilt_box(g, WOOD, (148, sy * 32, 54), (196, sy * 32, 84), 6, 6)
    g.box(WOOD, (190, 0, 86), (6, 76, 6))
    g.box(WOOD, (136, 0, 28), (8, 60, 44))
    books = ((COP, 0), (TAN, 6), (CREAM, -4), (COPD, 3), (SAND, -7), (COP, 5))
    for k, (col, yaw) in enumerate(books):
        g.box(col, (40, 0, 64 + k * 13), (64, 46, 12), rot=(0, yaw, 0))
        g.box(WHITE, (40, -1, 64 + k * 13), (58, 46, 7), rot=(0, yaw, 0))
    g.box(TAN, (104, 4, 63), (50, 40, 10))
    g.box(WHITE, (104, 4, 70), (46, 36, 5))
    tilt_box(g, COPD, (-190, -30, 8), (-150, 0, 150), 6, 6)
    tilt_box(g, COPD, (-110, -30, 8), (-150, 0, 150), 6, 6)
    tilt_box(g, COPD, (-150, 34, 8), (-150, 0, 150), 6, 6)
    for k in range(3):
        z = 44 + k * 40
        y = -27 + k * 8.5
        g.box(COPD, (-150, y + 2, z - 17), (86 - k * 16, 8, 5))
        g.box(CREAM, (-150, y, z), (76 - k * 16, 5, 30))
        if k == 0:
            rod(g, COP, (-150, y - 5, z), (-150, y - 2, z), 10, steps=8)
        elif k == 1:
            for j in (-1, 0, 1):
                g.box(SAND, (-150 + j * 14, y - 3.5, z - 3 + j * 4), (8, 4, 16 + j * 8))
        else:
            g.box(COP, (-150, y - 3.5, z), (14, 4, 14), rot=(45, 0, 0))


def pineapple_steps(sc):
    """Three stone steps between two flank blocks, a stone pineapple with copper leaves on each block, a copper hand
    rail up each side. 3.5 m long, 1.8 m."""
    g = sc.still()
    for k in range(3):
        g.box(CREAM if k % 2 == 0 else SAND, (0, k * 21, 9 + k * 18), (202, 126 - k * 42, 18))
    for sx in (-1, 1):
        g.box(SAND, (sx * 136, 0, 40), (70, 140, 80))
        g.box(CREAM, (sx * 136, 0, 84), (78, 148, 8))
        pineapple(g, sx * 136, 0, 88, 1.05)
        g.box(COP, (sx * 96, -60, 46), (5, 5, 74))
        g.box(COP, (sx * 96, 50, 82), (5, 5, 74))
        tilt_box(g, COP, (sx * 96, -60, 84), (sx * 96, 50, 120), 5, 5)


def street_clock(sc):
    """A copper-green street clock on a cream column: two round faces lit warm and steady, dark hands at ten past
    ten, a copper crown. 5.7 m. Stands 25 m out."""
    g = sc.still()
    g.box(TAN, (0, 0, 15), (74, 74, 30))
    g.box(SAND, (0, 0, 65), (54, 54, 70))
    g.box(CREAM, (0, 0, 104), (62, 62, 8))
    g.cyl(CREAM, (0, 0, 108), 17, 300, steps=8)
    for z in (150, 250, 350):
        g.cyl(COP, (0, 0, z), 19.5, 8, steps=8)
    g.cyl(CREAM, (0, 0, 408), 17, 26, steps=8, top_r=30)
    g.box(COPD, (0, 0, 438), (40, 40, 8))
    zc = 494.0
    rod(g, COP, (0, -22, zc), (0, 22, zc), 52, steps=12)
    for sy in (-1, 1):
        rod(g, CREAM, (0, sy * 22, zc), (0, sy * 26, zc), 43, steps=12, glow=0.5)
        g.box(COPD, (0, sy * 28, zc + 13), (5, 4, 30))
        tilt_box(g, COPD, (0, sy * 28, zc), (sy * -17, sy * 28, zc + 10), 4.5, 4.5)
        rod(g, COPD, (0, sy * 26, zc), (0, sy * 31, zc), 5, steps=6)
    for sx in (-1, 1):
        tilt_box(g, COP, (sx * 14, 0, 440), (sx * 44, 0, 470), 6, 6)
    g.cyl(COP, (0, 0, 544), 20, 14, steps=8, top_r=8)
    g.sphere(COPD, (0, 0, 566), 10, steps=(3, 6))


JOBS = [
    ('SM_S1D5_GreatScales', great_scales, dict(far=True, grow=(120, 120, 320), grow_down=(120, 120, 0))),
    ('SM_S1D5_PostTower', post_tower), ('SM_S1D5_CountingTower', counting_tower, STILL),
    ('SM_S1D5_VaultDrum', vault_drum, STILL), ('SM_S1D5_LanternTower', lantern_tower),
    ('SM_S1D5_ExchangeTower', exchange_tower), ('SM_S1D5_CoinPressHall', coin_press, STILL),
    ('SM_S1D5_ClerksBlock', clerks_block), ('SM_S1D5_ZigguratMinor', ziggurat_minor, STILL),
    ('SM_S1D5_CopperCupola', copper_cupola), ('SM_S1D5_VentCowls', vent_cowls), ('SM_S1D5_TeaCanopy', tea_canopy),
    ('SM_S1D5_BellArch', bell_arch), ('SM_S1D5_ParapetUrns', parapet_urns, STILL), ('SM_S1D5_PennantMast', pennant_mast),
    ('SM_S1D5_UrnRow', urn_row, STILL), ('SM_S1D5_TapeBench', tape_bench, STILL), ('SM_S1D5_LedgerCart', ledger_cart, STILL),
    ('SM_S1D5_PineappleSteps', pineapple_steps, STILL), ('SM_S1D5_StreetClock', street_clock, STILL),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
