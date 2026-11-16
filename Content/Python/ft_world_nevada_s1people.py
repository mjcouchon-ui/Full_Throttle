"""Nevada, Stage 1, builder s1people: the people pass 3 left out. /Game/World/Nevada/SM_S1PEOPLE_*.
Run: import ft_world_nevada_s1people as K; K.run()

Every piece brings the thing its people stand on (a stepped stand, a veranda, a freight wagon, a scrap deck, an adobe
roof, a stalk deck, a catwalk, a bunker roof, a gallery, a marble stair, a 23 m rock column), floor 2.3 m up or more,
so nobody is a head behind the barrier. Small pieces (4.4 x 3 m) meant to stand six in a row, 38 m and more from the
centre line; all are TALL. People stand, sit, wave, play, work; limbs swing in the vertex shader, nothing travels,
nothing flies, nothing blinks. Steady glow only on the range's light strip, the Strip's lamp and the palace lamps.
No dice, no chips, no vehicle with an engine, no wrench, no orange or green metre boxes, no weapons, no real names."""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Nevada'
# the kit's own colours (ft_dress_nevada.py, ft_desert.py, ft_car.py), by district as the zone plan gives them
W, RD, NV, YL, TQ = '#fff6ea', '#ff3355', '#2b2350', '#ffe14d', '#2fc6c0'          # d1 state line
SG, SGD, ST, PU, CR, LAMP = '#8fb86a', '#5f8f4a', '#8f9bb3', '#7b3bd6', '#f4efe6', '#ffc86a'   # d2 snake eyes
AQ, OP, PK = '#3fd6d0', '#233a63', '#ff4fb8'                                         # d3 last chance gas
VI, MG, CY, GD = '#3a2a6a', '#ff4fb8', '#2ee6ff', '#ffc93c'                          # d4 jackpot junction
SV, RS = '#d7e3f0', '#ff2d3c'                                                        # d5 salt flats
BK, PL = '#1c1830', '#c9c4d6'                                                        # d6 syndicate range
CRIM = '#6a2440'                                                                     # d8 vegas approach
WOOD, WOOD_D, SAND = '#9a6a44', '#6a4c36', '#f2c58a'
ROCKS = ('#b8553a', '#e0784a', '#c9603e', '#f59a62')
LIGHT_TOPS = ('#fff6ea', '#ffe14d', '#f4a6ff', '#57c7ff', '#7dffb0', '#ffc93c')
FOOT = -200.0

WAVE = dict(arm_r='wave', swing_r=24, rate=1.1)
WAVE_L = dict(arm_l='wave', swing_l=24, rate=1.0)
CHEER = dict(arm_l='vee', arm_r='vee', bob=6, rate=1.5)
SIT = dict(legs='sit', seat=46.0)


# ------------------------------------------------------------------------------------------ helpers
def who(sc, r, x, y, z, yaw=0.0, s=1.25, tops=None, bottoms=None, detail=1, **kw):
    d = P.look(r, tops=tops, bottoms=bottoms)
    d['s'] = d['s'] * s
    d['phase'] = r.random()
    d.update(kw)
    return P.figure(sc, x, y, z, yaw=yaw, detail=detail, **d)


def posts(g, col, L, D, top, w=18.0, inset=16.0, foot=FOOT):
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(col, (sx * (L / 2.0 - inset), sy * (D / 2.0 - inset), (top + foot) / 2.0), (w, w, top - foot))


def xbrace(g, col, L, y, z0, z1, w=9.0):
    tilt_box(g, col, (-L / 2.0 + 16, y, z0), (L / 2.0 - 16, y, z1), w, w)
    tilt_box(g, col, (-L / 2.0 + 16, y, z1), (L / 2.0 - 16, y, z0), w, w)


def deck(g, col, L, D, z, t=20.0, edge=None, y=0.0):
    g.box(col, (0, y, z - t / 2.0), (L, D, t))
    if edge:
        g.box(edge, (0, y - D / 2.0 - 3, z - t / 2.0), (L + 6, 8, t + 8))


def rail(g, col, L, y, z, h=95.0, n=5, w=7.0, x=0.0):
    g.box(col, (x, y, z + h), (L, w, w))
    for k in range(n):
        g.box(col, (x - L / 2.0 + w / 2.0 + k * (L - w) / (n - 1.0), y, z + h / 2.0), (w, w, h))


def slab(g, col, L, y0, z0, y1, z1, t=10.0, x=0.0):
    """A sloping slab L long (along X) from the front edge (y0, z0) to the back edge (y1, z1)."""
    tilt_box(g, col, (x, y0, z0), (x, y1, z1), L, t)


def stairs(g, col, x0, x1, y, top, n=6, w=90.0, foot=FOOT, z0=0.0):
    for k in range(n):
        h = z0 + (top - z0) * (k + 1.0) / n
        g.box(col, (x0 + (x1 - x0) * (k + 0.5) / n, y, (h + foot) / 2.0), (abs(x1 - x0) / n + 2.0, w, h - foot))


def stairs_y(g, col, x, y0, y1, top, n=6, w=70.0, foot=FOOT):
    """Solid steps w wide at x, climbing from the front (y0) to the back (y1), the last one `top` high."""
    for k in range(n):
        h = top * (k + 1.0) / n
        g.box(col, (x, y0 + (y1 - y0) * (k + 0.5) / n, (h + foot) / 2.0), (w, abs(y1 - y0) / n + 2.0, h - foot))


def wheel(g, col, hub, x, y, r, w=14.0):
    g.cyl(col, (x, y, r), r, w, rot=(0, 0, 90), steps=10)
    g.box(hub, (x, y, r), (r * 0.5, w * 2.0 + 10.0, r * 0.5))


def bunting(g, cols, x0, x1, y, z, n=8, sag=22.0):
    for k in range(n):
        f = k / (n - 1.0)
        g.box(cols[k % len(cols)], (x0 + (x1 - x0) * f, y, z - sag * math.sin(math.pi * f)), (20, 5, 20), rot=(45, 0, 0))


def pennant(sc, r, col, x, y, z, w=84.0, h=40.0, swing=14.0, rate=0.45):
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z), axis=P.UP, rate=rate, phase=r.random()))
    f.box(col, (x + w / 2.0 + 4, y, z), (w, 5, h))


def flag(col, pole=PL, w=72.0, h=44.0, length=150.0):
    def f(m, hd, side):
        m.box(pole, (hd[0], hd[1], hd[2] + length / 2.0 - 20), (5, 5, length))
        m.box(col, (hd[0] + side * w / 2.0, hd[1], hd[2] + length - 20 - h / 2.0), (w, 5, h))
    return f


def parasol(col, rim):
    def f(m, hd, side):
        m.box(PL, (hd[0], hd[1], hd[2] + 55), (5, 5, 130))
        m.cyl(col, (hd[0], hd[1], hd[2] + 112), 62, 22, steps=8, top_r=6)
        m.cyl(rim, (hd[0], hd[1], hd[2] + 107), 64, 5, steps=8)
    return f


def board(col=CR, clip=NV):
    def f(m, hd, side):
        m.box(col, (hd[0], hd[1] - 6, hd[2] + 10), (22, 5, 30))
        m.box(clip, (hd[0], hd[1] - 7, hd[2] + 24), (12, 6, 5))
    return f


def cup(col):
    def f(m, hd, side):
        m.cyl(col, (hd[0], hd[1] - 2, hd[2] + 4), 6, 16, steps=6, top_r=8)
    return f


def case(col):
    def f(m, hd, side):
        m.box(col, (hd[0], hd[1], hd[2] - 26), (34, 14, 36))
        m.box(GD, (hd[0], hd[1], hd[2] - 6), (5, 5, 10))
    return f


def bench(g, col, x, y, z, L=120.0, top=55.0):
    g.box(col, (x, y, z + top - 5), (L, 34, 10))
    for sx in (-1, 1):
        g.box(col, (x + sx * (L / 2.0 - 8), y, z + (top - 10) / 2.0), (8, 30, top - 10))


def umbrella(g, col, col2, x, y, z, r=120.0, h=210.0):
    g.box(PL, (x, y, z + h / 2.0), (6, 6, h))
    g.cyl(col, (x, y, z + h - 6), r, 30, steps=8, top_r=8)
    g.cyl(col2, (x, y, z + h - 12), r + 3, 6, steps=8)


def column(g, r, H, L=440.0, D=360.0):
    """A column of the canyon's own beds, H tall, with a pale cap; returns the cap's half sizes."""
    n = 8
    for k in range(n):
        z0 = FOOT - 100 if k == 0 else H * k / n
        z1 = H * (k + 1.0) / n
        f = 1.0 - 0.22 * k / (n - 1.0) + r.uniform(-0.04, 0.04)
        pts = []
        for a in range(8):
            an = math.radians(a * 45.0 + 22.5 + r.uniform(-8, 8))
            pts.append((math.cos(an) * L / 2.0 * f * 1.08, math.sin(an) * D / 2.0 * f * 1.08))
        g.plan(ROCKS[k % 4], pts, z0, z1)
    return L * 0.36, D * 0.34


# ------------------------------------------------------------------------------------------ d1 state line
def send_off(sc):
    """The send-off party: a stepped whitewashed stand, flags and bunting, hats in the air."""
    r = random.Random(101)
    g = sc.still()
    for z, y in ((260, -100), (330, 0), (400, 100)):
        g.box(W, (0, y, (z + FOOT) / 2.0), (440, 100, z - FOOT))
        g.box(NV, (0, y - 52, z - 8), (446, 8, 16))
    g.box(RD, (0, -153, 120), (440, 8, 70))
    g.box(YL, (0, -154, 196), (440, 6, 14))
    for sx in (-1, 1):
        g.box(NV, (sx * 208, 138, 550), (8, 8, 300))
        pennant(sc, r, RD if sx < 0 else TQ, sx * 208, 138, 676)
    g.box(NV, (0, 138, 640), (410, 5, 5))
    bunting(g, (RD, W, YL), -190, 190, 138, 626, 9, 0.0)
    tops = (W, RD, YL, TQ, '#57c7ff')
    who(sc, r, -150, -100, 260, 8, tops=tops, hat='cowboy', hat_col=CR, hold_r=flag(YL), **WAVE)
    who(sc, r, 0, -100, 260, -5, tops=tops, build='kid', s=0.85, **CHEER)
    who(sc, r, 150, -100, 260, -10, tops=tops, hat='sun', hat_col=YL, hold_l=flag(TQ), **WAVE_L)
    who(sc, r, -95, 0, 330, 6, tops=tops, hat='cowboy', hat_col=WOOD_D, **WAVE)
    who(sc, r, 60, 0, 330, -8, tops=tops, wear='dress', hold_r=flag(RD), **WAVE)
    who(sc, r, -10, 100, 400, 0, tops=tops, hat='peak', hat_col=NV, top=NV, sleeves='long', band=YL, arm_r='up', swing_r=16, rate=0.8)


def porch_rail(sc):
    """The store's veranda on posts: a bench full of regulars, a rocking chair, a red tin roof, a swinging board."""
    r = random.Random(102)
    g = sc.still()
    posts(g, W, 440, 290, 270)
    xbrace(g, W, 440, -129, 40, 230)
    deck(g, WOOD, 440, 300, 270, edge=NV)
    g.box(W, (0, 138, 400), (440, 16, 260))                       # the store's front, board and batten
    for k in range(7):
        g.box(CR, (-195 + k * 65, 128, 400), (8, 6, 260))
    g.box(RD, (-120, 127, 370), (80, 6, 190))                     # door
    g.box(NV, (90, 127, 420), (110, 6, 90))                       # window
    g.box(TQ, (22, 126, 420), (18, 6, 100))
    g.box(TQ, (158, 126, 420), (18, 6, 100))
    for sx in (-1, 1):
        g.box(W, (sx * 204, -134, 400), (12, 12, 260))
    slab(g, RD, 470, -170, 520, 150, 580, 12)
    g.box(W, (0, 140, 610), (300, 12, 90))                        # the stepped false front
    g.box(W, (0, 140, 670), (180, 12, 50))
    g.box(YL, (0, 132, 612), (200, 5, 30))                        # a blank yellow name board (no words)
    rail(g, NV, 440, -140, 270, 80, 6)
    sgn = sc.part(P.motion(swing=7, pivot=(190, -150, 500), axis=0.0, rate=0.35, phase=0.2))
    sgn.box(YL, (190, -150, 462), (60, 6, 44))
    sgn.box(NV, (190, -150, 492), (5, 5, 20))
    bench(g, TQ, -60, 60, 270, 200)
    who(sc, r, -130, 60, 270, 5, hat='cowboy', hat_col=CR, arm_r='face', hold_r=cup(W), **SIT)
    who(sc, r, -55, 60, 270, -4, wear='dress', arm_l='wave', swing_l=20, rate=0.9, **SIT)
    who(sc, r, 15, 60, 270, 0, build='kid', s=0.85, arm_l='vee', arm_r='vee', rate=1.3, swing_l=10, swing_r=10, **SIT)
    g.box(WOOD_D, (150, 40, 300), (60, 70, 10))                   # the rocking chair
    g.box(WOOD_D, (150, 74, 350), (60, 8, 110))
    who(sc, r, 150, 40, 270, -12, hat='sun', hat_col=CR, rock=5, rate=0.4, top=RD, **SIT)
    who(sc, r, -185, -95, 270, 12, hat='cowboy', hat_col=WOOD_D, top=W, band=RD, **WAVE)


def mail_wagon(sc):
    """A high freight wagon with no team, its tongue on the ground: ranch hands load mail sacks and wave their hats."""
    r = random.Random(103)
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            wheel(g, RD, YL, sx * 140, sy * 118 - 7, 108 if sx > 0 else 92)
    g.box(WOOD_D, (0, 0, 190), (330, 14, 14))                     # the reach
    for sx in (-1, 1):
        g.box(WOOD_D, (sx * 140, 0, 100 if sx < 0 else 112), (12, 250, 12))
        g.box(WOOD_D, (sx * 140, 0, 160), (30, 200, 80))
    g.box(W, (0, 0, 220), (420, 210, 40))                         # the bed, 2.4 m up
    g.box(TQ, (0, -107, 262), (420, 8, 50))
    g.box(TQ, (0, 107, 300), (420, 8, 120))
    g.box(NV, (0, -109, 262), (300, 5, 14))
    tilt_box(g, WOOD_D, (-205, 0, 200), (-330, 0, 10), 10, 10)   # the tongue
    for sx in (-1, 1):
        g.box(W, (sx * 200, 107, 420), (8, 8, 140))
    g.box(W, (0, 107, 486), (410, 8, 8))
    bunting(g, (RD, YL, W), -190, 190, 107, 470, 8, 18.0)
    for k, (x, y) in enumerate(((120, 50), (165, 30), (140, 40))):
        g.sphere(CR if k % 2 == 0 else SAND, (x, y, 268 + k * 30), 30, scale=(1.2, 1, 0.8), steps=(4, 6))
    who(sc, r, -150, -30, 240, 6, hat='cowboy', hat_col=CR, arm_r='up', swing_r=18, rate=1.0, top=RD)
    who(sc, r, -60, -40, 240, -6, hat='cowboy', hat_col=WOOD_D, top=W, band=NV, **CHEER)
    who(sc, r, 30, -20, 240, 40, top=TQ, arm_l='carry', arm_r='carry', hold_r=P.hold_box(CR, 40))
    who(sc, r, 75, 55, 240, -20, top=YL, hat='cap', hat_col=NV, **WAVE)
    who(sc, r, -110, 50, 240, 10, build='kid', s=0.85, top=W, **CHEER)


# ------------------------------------------------------------------------------------------ d2 snake eyes country and joshua flats
def drum_table(sc):
    """A scrap deck on oil drums: two arm wrestlers at a drum, the yard cheering them on."""
    r = random.Random(104)
    g = sc.still()
    for k, x in enumerate((-170, -57, 57, 170)):
        for row, y in enumerate((-100, 100)):
            g.cyl(PU if (k + row) % 2 == 0 else ST, (x, y, FOOT), 46, 230 - FOOT, steps=8)
    deck(g, ST, 440, 300, 250, edge=PU)
    for k in range(6):
        g.box(SGD, (-185 + k * 74, -152, 215), (60, 6, 44))       # flattened bonnets nailed round the edge
    g.box(SG, (0, 144, 370), (440, 10, 240))                      # a corrugated back wall
    for k in range(11):
        g.box(SGD, (-200 + k * 40, 138, 370), (8, 5, 240))
    slab(g, PU, 300, 20, 520, 150, 500, 10, x=-60)                # a purple awning
    g.cyl(ST, (0, -30, 250), 34, 86, steps=8)                     # the drum table
    g.cyl(CR, (0, -30, 336), 40, 6, steps=8)
    tops = (PU, CR, SG, '#8f6bff', '#ffc93c')
    who(sc, r, -62, -30, 250, 90, tops=tops, build='stocky', sleeves='none', arm_r='hold', rock=3, rate=1.6, seat=40.0, legs='sit')
    who(sc, r, 62, -30, 250, -90, tops=tops, build='stocky', hat='beanie', hat_col=PU, arm_l='hold', rock=3, rate=1.6, seat=40.0, legs='sit')
    who(sc, r, -150, 40, 250, 18, tops=tops, **CHEER)
    who(sc, r, 0, 70, 250, 0, tops=tops, hair='mohawk', hair_col=PU, **CHEER)
    who(sc, r, 150, 30, 250, -16, tops=tops, wear='shorts', **WAVE)


def porch_band(sc):
    """A tin lean-to on stilts and its porch band: guitar, washtub bass, washboard, and one who cannot sit still."""
    r = random.Random(105)
    g = sc.still()
    posts(g, ST, 440, 290, 280, 16)
    xbrace(g, ST, 440, -129, 30, 240, 8)
    deck(g, SGD, 440, 300, 280, edge=CR)
    g.box(SG, (0, 142, 420), (440, 10, 280))
    for k in range(11):
        g.box(SGD, (-200 + k * 40, 136, 420), (8, 5, 280))
    for sx in (-1, 1):
        g.box(ST, (sx * 205, -136, 400), (10, 10, 240))
    slab(g, ST, 470, -175, 510, 150, 580, 10)
    for k in range(6):
        g.box(PU, (-190 + k * 76, -176, 498), (56, 6, 26))        # a purple valance
    g.box(LAMP, (0, -120, 500), (26, 26, 30), glow=0.6)           # one warm lamp under the eave, steady
    g.cyl(ST, (150, 60, 280), 44, 60, steps=8)                    # the washtub bass
    g.box(WOOD, (150, 60, 430), (6, 6, 240))
    tops = (PU, CR, SG, '#ffc93c', '#fff6ea')
    # the guitarist: the box hangs at the chest, the right arm strums
    def guitar(m, hd, side):
        m.box(WOOD, (hd[0] + 4, hd[1] - 4, hd[2] - 4), (44, 12, 34))
        m.box(WOOD_D, (hd[0] + 46, hd[1] - 4, hd[2] + 6), (56, 6, 8))
    who(sc, r, -140, -40, 280, 6, tops=tops, hat='cowboy', hat_col=CR, arm_l='hold', hold_l=guitar, arm_r='hold', swing_r=14, axis_r='x', rate=1.8)
    who(sc, r, 105, 40, 280, -14, tops=tops, build='stocky', arm_r='fwd', swing_r=12, axis_r='x', rate=1.8)
    who(sc, r, -30, 50, 280, 0, tops=tops, arm_l='hold', arm_r='hold', hold_r=board(ST, ST), swing_r=10, axis_r='x', rate=1.9, **SIT)
    bench(g, WOOD, -30, 62, 280, 80)
    who(sc, r, 20, -80, 280, 0, tops=tops, wear='skirt', arm_l='out', arm_r='out', turn=28, rate=0.9)
    who(sc, r, -200, 70, 280, 20, tops=tops, build='kid', s=0.85, **CHEER)


def linemen(sc):
    """Linemen of the flats: a pole frame with pale insulators, a work platform, a coil of cable, a wave to the road."""
    r = random.Random(106)
    g = sc.still()
    for sx in (-1, 1):
        g.cyl(WOOD_D, (sx * 150, 60, FOOT), 15, 760 - FOOT, steps=6, top_r=11)
    g.box(WOOD_D, (0, 60, 690), (400, 14, 14))
    g.box(WOOD_D, (0, 60, 610), (360, 12, 12))
    for k in range(4):
        g.cyl(CR, (-150 + k * 100, 60, 697), 11, 24, steps=6, top_r=7)
    g.box(ST, (150, 44, 520), (60, 40, 80))                       # a pole transformer can
    g.box(CR, (150, 23, 520), (40, 5, 10))
    xbrace(g, WOOD, 330, 60, 60, 300, 9)
    deck(g, WOOD, 420, 230, 330, edge=PU, y=-40)
    for sx in (-1, 1):
        g.box(WOOD_D, (sx * 195, -140, (330 + FOOT) / 2.0), (14, 14, 330 - FOOT))
    rail(g, SG, 420, -152, 330, 85, 5)
    g.cyl(PU, (-150, -30, 330), 46, 18, steps=8)                  # a cable drum lying flat
    g.cyl(ST, (-150, -30, 348), 30, 50, steps=8)
    g.cyl(PU, (-150, -30, 398), 46, 18, steps=8)
    tilt_box(g, WOOD, (205, -100, FOOT), (205, 10, 330), 10, 10)  # a ladder's two stiles
    tilt_box(g, WOOD, (235, -100, FOOT), (235, 10, 330), 10, 10)
    tops = (YL, CR, '#57c7ff')
    who(sc, r, -50, -70, 330, 5, tops=tops, hat='hard', hat_col=YL, band=PU, **WAVE)
    who(sc, r, 60, -20, 330, -30, tops=tops, hat='hard', hat_col=CR, arm_l='up', arm_r='up', swing_l=14, swing_r=14, axis_l='x', axis_r='x', rate=0.9)
    who(sc, r, 150, -80, 330, 10, tops=tops, hat='hard', hat_col=YL, arm_r='face', hold_r=cup(CR))
    who(sc, r, -150, 20, 330, -10, tops=tops, hat='cap', hat_col=PU, arm_l='hold', arm_r='hold', hold_r=board())


# ------------------------------------------------------------------------------------------ d3 last chance gas
def adobe(g, L, D, top, col=AQ):
    g.box(col, (0, 0, (top + FOOT) / 2.0), (L, D, top - FOOT))
    g.box(col, (0, 0, 40), (L + 24, D + 24, 80 * 2))             # the battered foot
    for k in range(7):
        g.box(WOOD, (-L / 2.0 + 34 + k * (L - 68) / 6.0, -D / 2.0 - 12, top - 50), (12, 30, 12))   # beam ends
    for k in range(5):                                            # the wavy parapet, cream caps
        x = -L / 2.0 + L / 10.0 + k * L / 5.0
        h = 26 if k % 2 == 0 else 42
        g.box(col, (x, D / 2.0 - 10, top + h / 2.0), (L / 5.0, 20, h))
        g.box(W, (x, D / 2.0 - 10, top + h + 4), (L / 5.0 + 4, 24, 8))


def adobe_terrace(sc):
    """A painted adobe roof terrace: a card game under a yellow awning, ices on the outside stair."""
    r = random.Random(107)
    g = sc.still()
    adobe(g, 330, 290, 300)
    g.box(OP, (-60, -147, 150), (70, 6, 150))                     # a deep door
    g.box(W, (-60, -148, 232), (86, 6, 14))
    g.box(OP, (70, -147, 190), (50, 6, 50))
    g.box(W, (70, -148, 190), (62, 4, 8))
    stairs_y(g, AQ, 194, -150, 120, 300, 6, 58)                   # the outside stair, climbing to the back
    g.box(W, (194, 134, 300), (62, 10, 12))
    for sx in (-1, 1):
        g.box(WOOD, (-70 + sx * 85, 40 + 0, 400), (8, 8, 200))
        g.box(WOOD, (-70 + sx * 85, -100, 390), (8, 8, 180))
    slab(g, YL, 200, -120, 470, 60, 505, 8, x=-70)
    for k in range(4):
        g.box(W, (-145 + k * 50, -122, 462), (24, 5, 22))
    g.cyl(PK, (-70, -30, 300), 34, 80, steps=8, top_r=42)         # a pot for a table foot, a round top
    g.cyl(W, (-70, -30, 380), 52, 6, steps=8)
    tops = (W, YL, PK, '#57c7ff', '#ff7a3c')
    who(sc, r, -140, -30, 300, 80, tops=tops, hat='sun', hat_col=W, arm_r='hold', swing_r=8, axis_r='x', rate=0.7, **SIT)
    who(sc, r, 0, -30, 300, -80, tops=tops, arm_l='hold', swing_l=8, axis_l='x', rate=0.6, **SIT)
    who(sc, r, 80, -100, 300, 6, tops=tops, wear='dress', **WAVE)
    who(sc, r, 194, 52, 250, -20, tops=tops, build='kid', s=0.85, arm_r='face', hold_r=cup(PK))
    who(sc, r, 194, -38, 150, 10, tops=tops, wear='shorts', arm_l='face', hold_l=cup(W), arm_r='wave', swing_r=20, rate=1.0)


def pot_stall(sc):
    """Pots and blankets on a raised adobe plinth: a seller shaking out a blanket, a buyer pointing, a child with a jar."""
    r = random.Random(108)
    g = sc.still()
    g.box(AQ, (40, 0, (240 + FOOT) / 2.0), (360, 300, 240 - FOOT))
    g.box(W, (40, 0, 246), (372, 312, 12))
    stairs_y(g, W, -182, -150, 100, 240, 5, 76)
    for sx in (-1, 1):
        g.box(WOOD, (sx * 150 + 40, 120, 380), (10, 10, 270))
        g.box(WOOD, (sx * 150 + 40, -60, 365), (10, 10, 240))
    slab(g, YL, 350, -90, 478, 140, 520, 8, x=40)
    for k in range(6):
        g.box(PK if k % 2 else W, (-110 + k * 60, -92, 468), (30, 5, 24))
    g.box(WOOD, (40, 110, 330), (300, 40, 10))                    # shelves of pots
    g.box(WOOD, (40, 110, 400), (300, 40, 10))
    for k in range(5):
        c = (PK, W, YL, AQ, PK)[k]
        g.cyl(c, (-80 + k * 60, 110, 335), 20, 44, steps=6, top_r=13)
        g.cyl((W, PK, AQ, YL, W)[k], (-80 + k * 60, 110, 405), 16, 34, steps=6, top_r=22)
    g.cyl(PK, (175, -80, 252), 38, 90, steps=8, top_r=26)         # a big painted jar
    g.cyl(W, (175, -80, 300), 38.5, 10, steps=8)
    # the blanket rail: three striped blankets that stir
    g.box(WOOD, (-60, -20, 400), (8, 8, 300))
    g.box(WOOD, (120, -20, 400), (8, 8, 300))
    g.box(WOOD, (30, -20, 548), (190, 8, 8))
    for k, c in enumerate((PK, YL, W)):
        b = sc.part(P.motion(swing=5, pivot=(-25 + k * 55, -20, 548), axis=0.0, rate=0.3, phase=k * 0.3))
        b.box(c, (-25 + k * 55, -20, 492), (46, 6, 104))
        b.box(OP, (-25 + k * 55, -22, 480), (46, 4, 14))
        b.box(OP, (-25 + k * 55, -22, 520), (46, 4, 8))
    tops = (W, YL, PK, '#57c7ff', '#ff7a3c')
    who(sc, r, -95, 40, 252, 10, tops=tops, wear='skirt', arm_l='up', arm_r='up', swing_l=10, swing_r=10, axis_l='x', axis_r='x', rate=0.8,
        hold_r=P.hold_box(PK, 60))
    who(sc, r, 30, -90, 252, 160, tops=tops, hat='sun', hat_col=W, arm_r='point')
    who(sc, r, 100, -100, 252, 10, tops=tops, build='kid', s=0.85, arm_l='carry', arm_r='carry', hold_r=P.hold_box(YL, 30))
    who(sc, r, -182, -25, 144, 0, tops=tops, hat='cowboy', hat_col=CR, **WAVE)


# ------------------------------------------------------------------------------------------ d4 jackpot junction
def stalk_deck(g, z, col=VI):
    g.cyl(col, (0, 20, FOOT), 60, z - 20 - FOOT, steps=8, top_r=90)
    g.box(col, (0, 0, z - 10), (440, 300, 20))
    g.box(W, (0, -153, z - 10), (446, 8, 28))
    g.box(W, (0, 0, z - 24), (300, 200, 10))


def bellhops(sc):
    """Bell-hops and a brass luggage cart on a violet deck on a stalk; guests arriving; a steady starburst lamp."""
    r = random.Random(109)
    g = sc.still()
    stalk_deck(g, 280)
    rail(g, CY, 440, -144, 280, 70, 4, 7)
    g.box(GD, (-60, 20, 296), (150, 80, 10))                      # the cart's bed
    for sx in (-1, 1):
        g.box(GD, (-60 + sx * 70, 20, 380), (7, 7, 170))
    g.box(GD, (-60, 20, 466), (147, 7, 7))
    for k, c in enumerate((CRIM, W, MG)):
        g.box(c, (-90 + k * 32, 20, 322 + k * 4), (40 + k * 8, 60, 44 + k * 8))
    g.box(W, (190, 110, 430), (10, 10, 300))                      # the starburst lamp
    for a in (0, 45, 90, 135):
        g.box(GD, (190, 110, 600), (120, 6, 6), rot=(a, 0, 0), glow=0.6)
    g.sphere(W, (190, 110, 600), 18, steps=(4, 6), glow=0.6)
    hop = dict(hat='peak', hat_col=MG, top=MG, sleeves='long', band=GD, bottom=VI)
    who(sc, r, -160, 30, 280, -80, arm_l='push', arm_r='push', **hop)
    who(sc, r, 60, -70, 280, 0, hold_l=case(CRIM), **dict(hop, **WAVE))
    tops = (W, CY, GD, '#f4a6ff', '#7dffb0')
    who(sc, r, 130, 30, 280, -10, tops=tops, wear='dress', hat='sun', hat_col=W, hold_r=case(CY))
    who(sc, r, 20, 90, 280, 5, tops=tops, arm_r='face', hold_r=P.hold_camera())
    who(sc, r, -40, -90, 280, 0, tops=tops, build='kid', s=0.85, **CHEER)


def snap_shooters(sc):
    """Tourists photographing the signs from a boomerang-roofed stand: cameras up, one posing, one pointing."""
    r = random.Random(110)
    g = sc.still()
    stalk_deck(g, 290)
    rail(g, MG, 440, -144, 290, 70, 4, 7)
    for sx in (-1, 1):
        g.box(W, (sx * 150, 120, 440), (10, 10, 300))
        tilt_box(g, W, (sx * 8, -150, 520), (sx * 190, 140, 620), 110, 10)     # the boomerang roof's two wings
        tilt_box(g, CY, (sx * 8, -156, 514), (sx * 120, 20, 576), 8, 8)
    g.sphere(GD, (0, 120, 660), 24, steps=(4, 6))
    for a in (0, 60, 120):
        g.box(GD, (0, 120, 660), (110, 5, 5), rot=(a, 0, 0))
    tops = (W, CY, GD, '#f4a6ff', '#7dffb0', MG)
    cam = P.hold_camera()
    who(sc, r, -160, -80, 290, 10, tops=tops, hat='cap', hat_col=W, arm_l='hold', arm_r='hold', hold_r=cam)
    who(sc, r, -60, -90, 290, -30, tops=tops, wear='shorts', arm_r='face', hold_r=cam, pack=CY)
    who(sc, r, 50, -60, 290, 0, tops=tops, wear='dress', hat='sun', hat_col=MG, arm_l='hip', arm_r='up', swing_r=12, rate=0.8)
    who(sc, r, 150, -90, 290, -20, tops=tops, build='kid', s=0.85, arm_r='point')
    who(sc, r, 120, 40, 290, 150, tops=tops, arm_l='hold', arm_r='hold', hold_r=cam)


# ------------------------------------------------------------------------------------------ d5 salt flats
def timing_stand(sc):
    """The timekeepers' round-ended deck on a porthole drum: a flag marshal, clipboards, field glasses, pennants."""
    r = random.Random(111)
    g = sc.still()
    g.cyl(SV, (0, 20, FOOT), 100, 280 - FOOT, steps=10)
    for a in (-50, 0, 50):
        an = math.radians(a - 90)
        g.box(OP, (math.cos(an) * 100, 20 + math.sin(an) * 100, 170), (36, 10, 36), rot=(0, a, 0))
    g.box(SV, (0, 0, 290), (300, 300, 24))
    for sx in (-1, 1):
        g.cyl(SV, (sx * 150, 0, 278), 150, 24, steps=10)
    g.box(RS, (0, -152, 290), (300, 6, 12))
    g.box(RS, (0, 0, 272), (420, 280, 8))
    rail(g, W, 300, -144, 302, 75, 4, 7)
    g.box(W, (0, 140, 520), (8, 8, 440))                          # the pennant mast
    for k, c in enumerate((YL, RS, W)):
        pennant(sc, r, c, 0, 140, 720 - k * 60, 70, 34, 16, 0.5)
    g.box(W, (120, 60, 345), (90, 60, 86))                        # the timing desk
    g.box(OP, (120, 28, 362), (60, 5, 30))
    tops = (W, RS, YL, SV, '#57c7ff')
    who(sc, r, -170, -70, 302, 12, tops=tops, hat='cap', hat_col=RS, top=W, band=RS, arm_r='up', hold_r=flag(YL, W, 80, 60), swing_r=30, rate=0.9)
    who(sc, r, -50, -90, 302, 0, tops=tops, hat='sun', hat_col=W, arm_l='hold', arm_r='hold', hold_r=board())
    who(sc, r, 60, -80, 302, -8, tops=tops, arm_l='face', arm_r='face', hold_r=P.hold_camera(OP, SV))
    who(sc, r, 185, -60, 302, -14, tops=tops, hat='headset', hat_col=OP, **WAVE)
    who(sc, r, 120, 105, 302, 0, tops=tops, hat='peak', hat_col=W, arm_l='hold', arm_r='hold', hold_l=board(W, RS))


def umbrella_wagon(sc):
    """A salt wagon turned grandstand: a family under striped umbrellas with cool boxes and folding chairs."""
    r = random.Random(112)
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            wheel(g, SV, RS, sx * 135, sy * 120 - 7, 100)
        g.box(OP, (sx * 135, 0, 100), (14, 250, 14))
        g.box(OP, (sx * 135, 0, 150), (30, 200, 90))
    g.box(SV, (0, 0, 215), (430, 220, 40))
    g.box(RS, (0, -112, 215), (430, 6, 16))
    g.box(W, (0, -112, 255), (430, 8, 40))
    g.box(W, (0, 112, 265), (430, 8, 60))
    tilt_box(g, OP, (210, 0, 200), (330, 0, 10), 10, 10)
    umbrella(g, RS, W, -110, 30, 235, 125, 250)
    umbrella(g, YL, W, 110, 40, 235, 115, 235)
    g.box(W, (10, -60, 257), (64, 40, 44))                        # a cool box, white with a red lid
    g.box(RS, (10, -60, 283), (68, 44, 10))
    g.box(W, (185, 70, 252), (50, 36, 34))
    g.box(OP, (185, 70, 273), (54, 40, 8))
    for x in (-150, -70):
        g.box(YL, (x, 40, 285), (50, 46, 8))                      # folding chairs
        g.box(YL, (x, 64, 325), (50, 6, 70))
    tops = (W, RS, YL, '#57c7ff', '#f4a6ff')
    who(sc, r, -150, 34, 235, 5, tops=tops, hat='sun', hat_col=W, arm_r='face', hold_r=cup(RS), **SIT)
    who(sc, r, -70, 34, 235, -5, tops=tops, wear='dress', arm_l='wave', swing_l=18, rate=0.9, **SIT)
    who(sc, r, 60, -50, 235, 0, tops=tops, build='kid', s=0.85, **CHEER)
    who(sc, r, 130, -40, 235, -10, tops=tops, wear='shorts', hat='cap', hat_col=YL, hold_r=flag(RS, W, 60, 40, 130), **WAVE)
    who(sc, r, 150, 50, 235, 10, tops=tops, build='stocky', arm_r='face', hold_r=P.hold_camera(OP, SV))


def pit_deck(sc):
    """A pit shelter on a raised apron: an arched canvas roof, a tyre stack, mechanics in white with red caps."""
    r = random.Random(113)
    g = sc.still()
    posts(g, SV, 440, 290, 260, 16)
    xbrace(g, SV, 440, -129, 30, 220, 8)
    deck(g, W, 440, 300, 260, edge=RS)
    for k in range(6):
        g.box(YL, (-185 + k * 74, -155, 250), (37, 5, 14))        # a yellow-and-white band, not a chequer of a flag
    for sx in (-1, 1):                                            # three ribs of the arch, canvas between
        g.box(SV, (sx * 205, 0, 380), (8, 8, 240))
    for y in (-130, 130):
        tilt_box(g, SV, (-205, y, 500), (-110, y, 575), 8, 8)
        g.box(SV, (0, y, 578), (224, 8, 8))
        tilt_box(g, SV, (110, y, 575), (205, y, 500), 8, 8)
    tilt_box(g, W, (-207, 0, 500), (-110, 0, 577), 270, 6)
    g.box(W, (0, 0, 583), (224, 270, 6))
    tilt_box(g, W, (110, 0, 577), (207, 0, 500), 270, 6)
    g.box(RS, (0, -137, 560), (160, 5, 20))
    for k in range(4):
        g.cyl(OP, (160, 80, 260 + k * 26), 42, 24, steps=8)       # the tyre stack
    g.box(SV, (-150, 90, 300), (110, 50, 80))                     # a bench with a vice-less top
    g.box(RS, (-150, 64, 318), (90, 4, 20))
    mech = dict(top=W, bottom=W, wear='coat', hat='cap', hat_col=RS)
    who(sc, r, -150, 30, 260, 170, arm_r='fwd', hold_r=P.hold_tool(SV, RS, 30), swing_r=22, axis_r='x', rate=1.4, **mech)
    who(sc, r, -40, -80, 260, 6, **dict(mech, **WAVE))
    who(sc, r, 60, -20, 260, -40, arm_l='carry', arm_r='carry', hold_r=P.hold_box(SV, 40), **mech)
    who(sc, r, 150, -90, 260, -10, tops=(YL, '#57c7ff', W), hat='sun', hat_col=YL, **CHEER)
    who(sc, r, -190, -100, 260, 14, tops=(YL, '#57c7ff', W), build='kid', s=0.85, arm_r='point')


# ------------------------------------------------------------------------------------------ d6 syndicate range
TECH = dict(top=W, wear='coat', sleeves='long', bottom=BK)


def catwalk(sc):
    """Technicians on a concrete catwalk 4.2 m up: a steady cyan strip, clipboards, an instrument on a tripod."""
    r = random.Random(114)
    g = sc.still()
    for sx in (-1, 1):
        g.box(PL, (sx * 180, 30, (420 + FOOT) / 2.0), (50, 150, 420 - FOOT))
        g.box(PL, (sx * 180, 30, 60), (70, 190, 120))
    g.box(BK, (0, 0, 410), (440, 260, 22))
    g.box(CY, (0, -132, 410), (430, 6, 8), glow=0.6)
    g.box(PU, (0, 30, 392), (300, 40, 16))
    rail(g, PL, 440, -122, 420, 85, 6, 7)
    g.box(BK, (0, 120, 500), (440, 8, 160))                       # a blast screen behind them
    for k in range(4):
        g.box(PU if k % 2 else W, (-165 + k * 110, 115, 540), (100, 5, 40))   # a striped board, no words
    for dx, dy in ((-18, -14), (18, -14), (0, 18)):               # the tripod
        tilt_box(g, PL, (110 + dx, 20 + dy, 420), (110, 20, 540), 6, 6)
    lv = sc.part(P.motion(swing=25, pivot=(110, 20, 550), axis=P.UP, rate=0.2, phase=0.1))
    lv.box(BK, (110, 20, 552), (46, 16, 18))
    lv.box(CY, (132, 20, 552), (6, 12, 12))
    who(sc, r, 110, 70, 420, 0, hat='hard', hat_col=PL, arm_l='hold', arm_r='hold', **TECH)
    who(sc, r, -160, -60, 420, 10, hat='hard', hat_col=PU, **dict(TECH, **WAVE))
    who(sc, r, -60, -70, 420, -5, hat='headset', hat_col=BK, arm_l='hold', arm_r='hold', hold_r=board(W, PU), **TECH)
    who(sc, r, 30, -60, 420, 30, hat='hard', hat_col=PL, arm_r='point', **TECH)
    who(sc, r, 185, -60, 420, -15, hat='hard', hat_col=PU, arm_l='face', hold_l=cup(W), **TECH)


def bunker_roof(sc):
    """A battered bunker with slit windows; on its roof the guard shift plays cards round a crate and waves its caps."""
    r = random.Random(115)
    g = sc.still()
    g.box(BK, (0, 20, (300 + FOOT) / 2.0), (420, 250, 300 - FOOT))
    tilt_box(g, BK, (0, -155, FOOT), (0, -108, 300), 440, 24)     # the battered front
    for sx in (-1, 1):
        tilt_box(g, BK, (sx * 232, 20, FOOT), (sx * 208, 20, 300), 250, 24)
    g.box(PL, (0, 10, 306), (440, 290, 12))
    g.box(CY, (0, -130, 200), (260, 8, 14), glow=0.6)             # the slit, a steady cyan line
    g.box(PL, (0, -136, 222), (280, 8, 10))
    g.box(PU, (-150, -138, 110), (60, 6, 150))
    g.cyl(PL, (170, 100, 312), 30, 120, steps=8)                  # a ventilator cowl
    g.cyl(PL, (170, 100, 432), 44, 26, steps=8, top_r=30)
    g.box(PL, (-20, -10, 350), (70, 70, 76))                      # the crate they play on
    g.box(W, (-28, -14, 390), (16, 22, 5))
    g.box(W, (-8, -2, 390), (16, 22, 5))
    guard = dict(top=BK, bottom=PL, sleeves='long', band=PU)
    who(sc, r, -100, -10, 312, 85, arm_r='hold', swing_r=8, axis_r='x', rate=0.6, hat='cap', hat_col=PU, **dict(guard, **SIT))
    who(sc, r, 60, -10, 312, -85, arm_l='hold', swing_l=8, axis_l='x', rate=0.7, hat='cap', hat_col=PU, **dict(guard, **SIT))
    who(sc, r, -20, 70, 312, 0, arm_l='face', hold_l=cup(W), hat='beanie', hat_col=PU, **dict(guard, **SIT))
    who(sc, r, -170, -90, 312, 10, hat='peak', hat_col=PU, **dict(guard, **WAVE))
    who(sc, r, 150, -80, 312, -10, hair='bald', arm_r='up', swing_r=18, rate=1.0, **guard)


def surveyors(sc):
    """Surveyors on a concrete plinth: a level on its tripod, a striped staff, a slow cyan signal flag, a tea urn."""
    r = random.Random(116)
    g = sc.still()
    g.box(PL, (0, 0, (330 + FOOT) / 2.0), (360, 260, 330 - FOOT))
    g.box(PL, (0, 0, 50), (400, 300, 100))
    g.box(BK, (0, -132, 300), (360, 6, 30))
    g.box(PU, (0, -134, 300), (120, 6, 14))
    stairs_y(g, PL, 216, -130, 110, 330, 7, 70)
    for dx, dy in ((-20, -16), (20, -16), (0, 20)):
        tilt_box(g, WOOD, (-60 + dx, -40 + dy, 330), (-60, -40, 455), 6, 6)
    lv = sc.part(P.motion(swing=20, pivot=(-60, -40, 462), axis=P.UP, rate=0.18, phase=0.5))
    lv.box(YL, (-60, -40, 466), (50, 16, 18))
    lv.box(BK, (-84, -40, 466), (6, 12, 12))
    g.cyl(PL, (130, 70, 330), 26, 60, steps=8)                    # the tea urn on a crate
    g.cyl(BK, (130, 70, 390), 28, 8, steps=8)
    g.box(WOOD, (130, 70, 310), (70, 60, 40))
    def staff(m, hd, side):
        for k in range(6):
            m.box(PU if k % 2 else W, (hd[0], hd[1], hd[2] - 60 + k * 50), (9, 9, 50))
    who(sc, r, -60, 20, 330, 0, hat='sun', hat_col=PL, arm_l='hold', arm_r='hold', **TECH)
    who(sc, r, 80, -70, 330, 150, hat='hard', hat_col=PU, arm_r='fwd', hold_r=staff, top=YL, bottom=BK)
    who(sc, r, -150, -70, 330, 10, hat='hard', hat_col=PL, arm_r='up', hold_r=flag(CY, PL, 80, 56, 160), swing_r=26, rate=0.5, top=W, bottom=BK)
    who(sc, r, 130, 0, 330, -20, hat='cap', hat_col=PU, arm_l='face', hold_l=cup(W), **TECH)
    who(sc, r, 216, -10, 189, 0, hat='hard', hat_col=PL, arm_l='hold', arm_r='hold', hold_r=board(W, PU), **TECH)


# ------------------------------------------------------------------------------------------ d7 red rock return
MINER = dict(hat='hard', hat_col=YL, bottom=NV)


def mine_gallery(sc):
    """Two galleries of the boarding house: miners at both rails, bleached boards, a washing line that stirs."""
    r = random.Random(117)
    g = sc.still()
    for sx in (-1, 0, 1):
        for y in (-130, 130):
            g.box(TQ, (sx * 205, y, (640 + FOOT) / 2.0), (16, 16, 640 - FOOT))
    xbrace(g, TQ, 440, -130, 30, 250, 9)
    g.box(PL, (0, 136, 450), (440, 10, 400))
    for k in range(4):
        g.box(CR, (-195 + k * 130, 130, 450), (8, 5, 400))
    for z in (290, 570):
        deck(g, PL, 440, 290, z, edge=TQ)
        rail(g, TQ, 440, -140, z, 80, 6)
        g.box(NV, (-110, 129, z + 95), (70, 6, 170))
        g.box(NV, (90, 129, z + 110), (60, 6, 70))
    slab(g, PL, 470, -170, 800, 150, 860, 10)
    g.box(YL, (0, -150, 770), (60, 40, 50))                       # the lamp room's box under the eave, unlit
    g.box(NV, (0, 60, 548), (400, 5, 5))                          # the washing line, upper gallery's underside
    for k, c in enumerate((W, RD, CR, '#57c7ff', W)):
        b = sc.part(P.motion(swing=8, pivot=(-140 + k * 70, 60, 548), axis=0.0, rate=0.4, phase=k * 0.21))
        b.box(c, (-140 + k * 70, 60, 520), (44, 5, 52))
    tops = (W, YL, CR, '#57c7ff', '#f4a6ff')
    who(sc, r, -150, -90, 290, 8, tops=tops, **dict(MINER, **WAVE))
    who(sc, r, 140, -90, 290, 0, tops=tops, wear='dress', **WAVE_L)
    who(sc, r, -90, -90, 570, 5, tops=tops, **dict(MINER, **CHEER))
    who(sc, r, 60, -90, 570, -8, tops=tops, **dict(MINER, arm_r='up', swing_r=20, rate=0.9))
    who(sc, r, 165, -80, 570, -15, tops=tops, build='kid', s=0.85, **CHEER)


def hoist_deck(sc):
    """The hoist platform: a teal A-frame and its sheave wheel, an ore bucket on its rope, a windlass, a sluice and pans."""
    r = random.Random(118)
    g = sc.still()
    posts(g, TQ, 440, 290, 300, 18)
    xbrace(g, TQ, 440, -129, 30, 260, 9)
    deck(g, PL, 440, 300, 300, edge=TQ)
    for sy in (-1, 1):
        tilt_box(g, TQ, (-120, sy * 110, 300), (-120, sy * 12, 700), 14, 14)
    g.box(TQ, (-120, 0, 520), (14, 150, 14))
    wh = sc.part(P.motion(swing=40, pivot=(-120, 0, 700), axis=90.0, rate=0.25, phase=0.3))
    wh.cyl(PL, (-120, -8, 700), 62, 16, rot=(0, 0, 90), steps=10)
    for a in (0, 60, 120):
        wh.box(NV, (-120, -1, 700), (118, 20, 8), rot=(a, 0, 0))
    g.box(NV, (-120 + 62, 0, 560), (5, 5, 280))                   # the rope
    bk = sc.part(P.motion(swing=4, pivot=(-58, 0, 700), axis=90.0, rate=0.3, phase=0.6))
    bk.cyl(CR, (-58, 0, 380), 36, 60, steps=8, top_r=44)
    bk.box(NV, (-58, 0, 446), (70, 5, 5))
    g.box(TQ, (40, 60, 350), (90, 50, 100))                       # the windlass
    g.cyl(PL, (40, 30, 380), 22, 60, rot=(0, 0, 90), steps=8)
    tilt_box(g, PL, (120, -130, 345), (215, 60, 400), 60, 10)     # a sluice box running back and up
    g.cyl(YL, (150, -110, 300), 26, 8, steps=8, top_r=34)
    tops = (W, YL, CR, '#57c7ff')
    who(sc, r, 40, -10, 300, 180, tops=tops, arm_r='fwd', swing_r=30, axis_r='x', rate=0.8, **MINER)
    def pan(m, hd, side):
        m.cyl(YL, (hd[0] - side * 14, hd[1] - 6, hd[2]), 24, 7, steps=8, top_r=30)
    who(sc, r, 150, -60, 300, 10, tops=tops, hat='cowboy', hat_col=CR, arm_l='carry', arm_r='carry', hold_r=pan, turn=10, rate=0.9)
    who(sc, r, -180, -90, 300, 12, tops=tops, **dict(MINER, **WAVE))
    who(sc, r, -40, -100, 300, -4, tops=tops, **dict(MINER, **CHEER))
    who(sc, r, 195, 90, 300, -20, tops=tops, build='kid', s=0.85, arm_r='point')


RIM_H = 2300.0


def rim_stage(g, r):
    hx, hy = column(g, r, RIM_H)
    g.box(WOOD, (0, 0, RIM_H + 12), (460, 330, 24))               # a timber stage on the cap
    g.box(W, (0, -168, RIM_H + 12), (460, 8, 34))
    for sx in (-1, 1):
        tilt_box(g, WOOD_D, (sx * 150, -150, RIM_H - 230), (sx * 215, -160, RIM_H), 14, 14)
    rail(g, W, 460, 160, RIM_H + 24, 110, 5, 9)
    return RIM_H + 24


def rim_flags(sc):
    """Rim people the road can read: three in pale clothes, twice life size, 23 m up, swinging big flags."""
    r = random.Random(119)
    g = sc.still()
    z = rim_stage(g, r)
    for k, (x, c, yaw) in enumerate(((-140, W, 8), (10, YL, -4), (150, PK, -10))):
        who(sc, r, x, -40, z, yaw, s=2.0, detail=2, tops=LIGHT_TOPS, bottoms=('#f1e4c8', '#e8e0f0', '#7a8fb8'), hat='sun' if k != 1 else 'cowboy', hat_col=W,
            arm_r='up', hold_r=flag(c, W, 130, 84, 230), swing_r=24, rate=0.6 + 0.1 * k, arm_l='hip' if k else 'wave')
    for sx in (-1, 1):
        g.box(W, (sx * 215, 150, z + 190), (9, 9, 380))
        pennant(sc, r, RD if sx < 0 else TQ, sx * 215, 150, z + 350, 110, 56, 14, 0.4)


def rim_parasols(sc):
    """The rim's photographer and her party: a plate camera on a tripod, two parasols, a child jumping, 23 m up."""
    r = random.Random(120)
    g = sc.still()
    z = rim_stage(g, r)
    for dx, dy in ((-34, -26), (34, -26), (0, 34)):
        tilt_box(g, WOOD_D, (-120 + dx, -70 + dy, z), (-120, -70, z + 230), 9, 9)
    g.box(NV, (-120, -78, z + 250), (70, 60, 54))
    g.cyl(PL, (-120, -108, z + 250), 16, 26, rot=(0, 0, 90), steps=6)
    lt = dict(tops=LIGHT_TOPS, bottoms=('#f1e4c8', '#e8e0f0', '#7a8fb8'), s=2.0, detail=2)
    who(sc, r, -120, 20, z, 0, hat='sun', hat_col=W, arm_l='fwd', arm_r='up', swing_r=14, rate=0.5, **lt)
    who(sc, r, 30, -50, z, -6, wear='dress', arm_r='hold', hold_r=parasol(PK, W), rock=3, rate=0.35, **lt)
    who(sc, r, 160, -20, z, -14, hat='cowboy', hat_col=CR, arm_l='hold', hold_l=parasol(W, TQ), arm_r='wave', swing_r=22, rate=0.8, **lt)
    who(sc, r, 95, 70, z, 0, build='kid', tops=LIGHT_TOPS, s=1.4, detail=2, **CHEER)


# ------------------------------------------------------------------------------------------ d8 vegas approach
def grand_stair(sc):
    """A marble stair with a crimson carpet and gold urns: guests going up, a doorman at the top, steady globes."""
    r = random.Random(121)
    g = sc.still()
    for k in range(5):
        z = 110 + k * 50
        y = -125 + k * 62
        g.box(W, (0, y, (z + FOOT) / 2.0), (440, 64, z - FOOT))
        g.box(CRIM, (0, y - 1, z + 2), (150, 64, 6))
    for sx in (-1, 1):
        g.box(W, (sx * 190, 0, 215), (60, 300, 430))              # the cheek walls
        g.box(GD, (sx * 190, 0, 432), (66, 306, 8))
        g.cyl(GD, (sx * 190, -110, 436), 14, 30, steps=8, top_r=30)   # urns
        g.cyl(GD, (sx * 190, -110, 466), 30, 16, steps=8, top_r=22)
        g.box(W, (sx * 190, 110, 520), (10, 10, 170))             # lamp standards, a steady globe each
        g.sphere(W, (sx * 190, 110, 620), 24, steps=(4, 6), glow=0.6)
        g.cyl(GD, (sx * 190, 110, 596), 16, 8, steps=6)
    tops = (W, GD, CRIM, '#f4a6ff', '#57c7ff')
    who(sc, r, 0, 123, 310, 0, hat='peak', hat_col=CRIM, top=CRIM, sleeves='long', wear='coat', band=GD, arm_r='up', swing_r=16, rate=0.7)
    who(sc, r, -90, 0, 210, 10, tops=tops, wear='dress', hair='bun', **WAVE)
    who(sc, r, -30, 0, 210, 0, tops=tops, sleeves='long', top=NV, bottom=NV, arm_l='hip')
    who(sc, r, 80, -62, 160, -8, tops=tops, wear='dress', hat='sun', hat_col=GD, hold_l=case(CRIM), **WAVE)
    who(sc, r, 110, 62, 260, -20, tops=tops, build='kid', s=0.85, **CHEER)


def quartet(sc):
    """A slice of the colonnade on a marble plinth: a string quartet playing under it, a sightseer at the balustrade."""
    r = random.Random(122)
    g = sc.still()
    g.box(W, (0, 0, (270 + FOOT) / 2.0), (440, 300, 270 - FOOT))
    g.box(GD, (0, -152, 250), (440, 6, 14))
    g.box(CRIM, (0, -20, 272), (300, 200, 5))
    for sx in (-1, 1):
        for y in (-110, 120):
            g.cyl(W, (sx * 185, y, 270), 22, 300, steps=8, top_r=18)
            g.box(GD, (sx * 185, y, 576), (54, 54, 12))
    g.box(W, (0, 5, 612), (440, 290, 60))
    g.box(GD, (0, -142, 630), (446, 6, 12))
    g.box(AQ, (0, 5, 660), (300, 180, 40))
    rail(g, W, 300, -140, 270, 70, 5, 9)
    tops = (W, NV, CRIM)
    def fiddle(m, hd, side):
        m.box(WOOD, (hd[0], hd[1] - 6, hd[2] + 10), (16, 10, 40))
        m.box(WOOD_D, (hd[0], hd[1] - 6, hd[2] + 40), (6, 6, 26))
    def bow(m, hd, side):
        m.box(CR, (hd[0] - side * 20, hd[1] - 8, hd[2] + 6), (60, 4.5, 4.5))
    play = dict(sleeves='long', bottom=NV, arm_l='face', hold_l=fiddle, arm_r='hold', hold_r=bow, swing_r=12, axis_r='z', rate=1.5)
    who(sc, r, -110, -20, 277, 15, tops=tops, **play)
    who(sc, r, -30, 30, 277, 5, tops=tops, wear='dress', **play)
    g.box(WOOD, (60, 6, 345), (40, 24, 110))                      # the cello
    g.box(WOOD_D, (60, 6, 420), (8, 8, 60))
    who(sc, r, 60, 40, 277, -5, tops=tops, sleeves='long', bottom=NV, arm_l='hold', arm_r='fwd', hold_r=bow, swing_r=14, axis_r='z', rate=1.2, **SIT)
    bench(g, GD, 60, 52, 277, 60)
    who(sc, r, 130, -30, 277, -15, tops=tops, wear='dress', hair='long', turn=6, rate=0.7, arm_l='hold', arm_r='hold')
    who(sc, r, -185, -95, 277, 20, tops=(GD, '#f4a6ff', '#57c7ff'), hat='sun', hat_col=W, **WAVE)


# ------------------------------------------------------------------------------------------ the jump at 1.5 km
def jump_stand(sc):
    """One high timber stand for the jump at 1.5 km: three tiers 5 m up and more, bunting, flags, everybody on their feet."""
    r = random.Random(123)
    g = sc.still()
    for sx in (-1, 0, 1):
        for y in (-150, 150):
            g.box(W, (sx * 380, y, (640 + FOOT) / 2.0), (22, 22, 640 - FOOT))
    for sx in (-1, 1):
        tilt_box(g, W, (sx * 380, -150, 40), (sx * 10, -150, 460), 12, 12)
        tilt_box(g, W, (sx * 10, -150, 40), (sx * 380, -150, 460), 12, 12)
    for k, (z, y) in enumerate(((500, -110), (580, 0), (660, 110))):
        g.box(W, (0, y, z - 12), (800, 112, 24))
        g.box(RD, (0, y - 58, z - 12), (806, 8, 32))
        if k:
            g.box(NV, (0, y - 54, z - 50), (800, 6, 56))
    g.box(RD, (0, -168, 420), (800, 8, 110))
    g.box(YL, (0, -170, 420), (560, 6, 30))                       # a blank yellow band, no words
    slab(g, RD, 840, -190, 930, 170, 990, 12)
    for sx in (-1, 0, 1):
        g.box(W, (sx * 380, 150, 820), (14, 14, 330))
        g.box(W, (sx * 380, -150, 760), (14, 14, 330))
    bunting(g, (RD, YL, W, TQ), -370, 370, -160, 900, 10, 30.0)
    for sx in (-1, 1):
        g.box(NV, (sx * 400, 150, 1100), (8, 8, 240))
        pennant(sc, r, YL if sx < 0 else TQ, sx * 400, 150, 1190, 110, 54, 14, 0.4)
    tops = (W, RD, YL, TQ, '#57c7ff', '#f4a6ff')
    acts = (CHEER, WAVE, WAVE_L, dict(arm_r='up', hold_r=flag(YL), swing_r=22, rate=1.0), dict(arm_l='up', hold_l=flag(TQ), swing_l=22, rate=0.9),
            dict(arm_r='point'))
    k = 0
    for z, y, n in ((500, -110, 5), (580, 0, 3), (660, 110, 2)):
        for i in range(n):
            x = -330 + (i + 0.5) * 660.0 / n + r.uniform(-20, 20)
            kw = dict(acts[k % len(acts)])
            if k % 4 == 1:
                kw.update(hat='cowboy', hat_col=CR)
            if k % 5 == 3:
                kw.update(build='kid', s=0.85)
            who(sc, r, x, y, z, r.uniform(-12, 12), tops=tops, **kw)
            k += 1


JOBS = [('SM_S1PEOPLE_SendOff', send_off), ('SM_S1PEOPLE_PorchRail', porch_rail), ('SM_S1PEOPLE_MailWagon', mail_wagon),
        ('SM_S1PEOPLE_DrumTable', drum_table), ('SM_S1PEOPLE_PorchBand', porch_band), ('SM_S1PEOPLE_Linemen', linemen),
        ('SM_S1PEOPLE_AdobeTerrace', adobe_terrace), ('SM_S1PEOPLE_PotStall', pot_stall),
        ('SM_S1PEOPLE_Bellhops', bellhops), ('SM_S1PEOPLE_SnapShooters', snap_shooters),
        ('SM_S1PEOPLE_TimingStand', timing_stand), ('SM_S1PEOPLE_UmbrellaWagon', umbrella_wagon), ('SM_S1PEOPLE_PitDeck', pit_deck),
        ('SM_S1PEOPLE_Catwalk', catwalk), ('SM_S1PEOPLE_BunkerRoof', bunker_roof), ('SM_S1PEOPLE_Surveyors', surveyors),
        ('SM_S1PEOPLE_MineGallery', mine_gallery), ('SM_S1PEOPLE_HoistDeck', hoist_deck),
        ('SM_S1PEOPLE_RimFlags', rim_flags), ('SM_S1PEOPLE_RimParasols', rim_parasols),
        ('SM_S1PEOPLE_GrandStair', grand_stair), ('SM_S1PEOPLE_Quartet', quartet),
        ('SM_S1PEOPLE_JumpStand', jump_stand)]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
