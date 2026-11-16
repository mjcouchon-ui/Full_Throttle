"""Bayshore, Stage 1, builder s1plants: the planting of the seven districts. /Game/World/Bayshore/SM_S1PLANTS_*.
Run: import ft_world_bay_s1plants as K; K.run()

22 plant pieces, every one an ft_people.Scene (one section, M_People), every one stirring in the vertex shader only:
a crown rocks about the point where it joins its trunk (so nothing tears), fronds nod about their hub, flowers and
plumes nod about their foot. No glow anywhere. Trunks and footings go below z = 0 for falling land.
The placing rules: C:/ftwork/s1/L0_s1plants.inl (written by C:/ftwork/jobs/L0s1plants/make_inl.py).
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box            # tilt_box(m, col, from, to, width, depth)   rod(m, col, from, to, r, steps)

DIR = '/Game/World/Bayshore'
LOW = dict(grow=(25, 25, 12), grow_down=(25, 25, 0))      # a low piece: bounds stay under 2.5 m (no shadow pass for it)

LEAF, LEAF_D, LEAF_L = '#2fbf71', '#1f9a62', '#7fe3c4'
TRUNK, TRUNK_D, WOOD = '#c08a5a', '#9a6a44', '#9a744a'
PINK, CORAL, YEL, WHITE, RED = '#ff4fb8', '#ff7a8a', '#ffe14d', '#fffaf2', '#ff4d5e'
CREAM, TEAL, SAND, SAND_D = '#fff1d6', '#2fc6c0', '#f0d9a4', '#d6bd8c'
STONE, STONE_D, OCHRE = '#b8b4c8', '#8a86a0', '#e8b26a'
POOL, BLUE, SKY, NAVY = '#49e2d5', '#4a7fc0', '#8fe3ff', '#2b2350'


def sway(deg, pivot, axis=0.0, rate=0.3, phase=0.0):
    return P.motion(swing=deg, pivot=pivot, axis=axis, rate=rate, phase=phase)


def spike(m, col, a, b, r0, r1, steps=4):
    """A tapering leaf or thorn from a to b (rod's own turning, with a narrow end)."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r0, L, rot=(pitch, yaw, 0), steps=steps, top_r=r1)


def plate(m, col, c, r, yaw, tilt, h=8.0, steps=6):
    """A round leaf or a flower: a flat disc whose face looks `tilt` degrees off straight up, towards `yaw`."""
    m.cyl(col, c, r, h, rot=(-tilt, yaw, 0), steps=steps)


def frond(m, col, hub, yaw, L, rise, droop, w, segs=3, col2=None, t=10.0):
    """A palm frond from its hub: it rises, arches and hangs."""
    c, s = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
    pts = [hub,
           (hub[0] + c * L * 0.42, hub[1] + s * L * 0.42, hub[2] + rise),
           (hub[0] + c * L * 0.80, hub[1] + s * L * 0.80, hub[2] + rise - droop * 0.42),
           (hub[0] + c * L, hub[1] + s * L, hub[2] + rise - droop)]
    if segs == 2:
        pts = [pts[0], pts[1], (hub[0] + c * L * 0.92, hub[1] + s * L * 0.92, hub[2] + rise - droop * 0.8)]
    ws = (w, w * 0.9, w * 0.55)
    for k in range(len(pts) - 1):
        tilt_box(m, col if k != 1 or not col2 else col2, pts[k], pts[k + 1], ws[k], t)


def ring(n, r, a0=0.0):
    return [(math.cos(math.radians(a0 + k * 360.0 / n)) * r, math.sin(math.radians(a0 + k * 360.0 / n)) * r, a0 + k * 360.0 / n) for k in range(n)]


# ------------------------------------------------------------------------------------------------ the palms
def royal_palms(sc):                            # three tall pale trunks in a round bed: the Strip, Hotel Row, downtown
    r = random.Random(11)
    g = sc.still()
    g.cyl(CREAM, (0, 0, -80), 440, 112, steps=10)                       # the bed's kerb
    g.cyl(LEAF_D, (0, 0, 32), 410, 6, steps=10)
    for k, (x, y, h) in enumerate(((-250, 70, 1180), (40, -100, 1420), (290, 110, 1270))):
        g.cyl(SAND_D, (x, y, -200), 36, 200 + h * 0.45, steps=7, top_r=31)
        g.cyl(SAND_D, (x, y, h * 0.45), 31, h * 0.55 - 150, steps=7, top_r=22)
        for j in range(3):
            g.cyl(STONE, (x, y, 120 + j * h * 0.2), 36 - j * 3, 10, steps=7)        # the leaf scars: rings
        g.cyl(LEAF, (x, y, h - 150), 24, 170, steps=7, top_r=15)           # the green crown shaft
        hub = (x, y, h + 10)
        A = sc.part(sway(5.0, hub, axis=20 + k * 50, rate=0.27, phase=r.random()))
        B = sc.part(sway(6.5, hub, axis=110 + k * 50, rate=0.33, phase=r.random()))
        for j in range(9):
            frond((A, B)[j % 2], (LEAF, LEAF_D)[j % 2], hub, j * 40 + k * 13, 400 + r.uniform(-30, 40), 120, 250, 72, segs=2)
        tilt_box(A, LEAF_L, hub, (x + 20, y + 10, h + 190), 30, 10)        # the new spear
    for (x, y, a) in ring(7, 330, 15):                                  # a ring of flowers in the bed
        g.sphere((PINK, WHITE)[int(a) % 2], (x, y, 38), 26, scale=(1, 1, 0.6), steps=(3, 5))


def fan_palm(sc):                               # short and stout, a round head of fans: the Strip, the Marina
    r = random.Random(12)
    g = sc.still()
    g.cyl(TRUNK_D, (0, 0, -150), 38, 540, steps=7, top_r=30)
    for j in range(5):
        g.cyl(TRUNK, (0, 0, 20 + j * 68), 33, 44, steps=7, top_r=47)       # the old leaf boots, flared
    g.cyl(SAND_D, (0, 0, 318), 30, 80, steps=7, top_r=84)               # the skirt of dry fans
    hub = (0, 0, 400)
    parts = [sc.part(sway(4.5, hub, axis=a, rate=0.3 + 0.04 * k, phase=r.random())) for k, a in enumerate((0, 60, 120))]
    for j in range(11):
        m = parts[j % 3]
        yaw = j * 360.0 / 11
        tilt = 78 if j % 2 else 52
        c, s = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
        reach = 190 if j % 2 else 150
        up = 40 if j % 2 else 150
        e = (c * reach, s * reach, 400 + up)
        tilt_box(m, LEAF_D, hub, e, 9, 9)
        plate(m, LEAF, e, 112, yaw, tilt)
        plate(m, LEAF_L, (e[0] + c * 5, e[1] + s * 5, e[2] + 6), 58, yaw, tilt)
    for j in range(3):                                                  # the young fans standing up in the middle
        yaw = j * 120 + 30
        c, s = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
        e = (c * 60, s * 60, 620)
        tilt_box(parts[j], LEAF_D, hub, e, 9, 9)
        plate(parts[j], LEAF_L, e, 86, yaw, 22)
    g.sphere(SAND, (0, 0, -30), 170, scale=(1.2, 1.0, 0.3), steps=(4, 6))
    for (x, y, a) in ring(3, 120, 40):                                  # seedlings at its foot
        plate(g, LEAF, (x, y, 46), 40, a, 40)
        tilt_box(g, LEAF_D, (x * 0.6, y * 0.6, 0), (x, y, 46), 8, 8)


def coco_palm(sc):                              # a leaning coconut palm and a young one: the Marina, Pleasure Beach
    r = random.Random(13)
    g = sc.still()
    pts = [(0, 0, -200), (45, 0, 160), (115, 0, 430), (200, 0, 690), (268, 0, 900), (310, 0, 1070)]
    for k in range(5):
        rod(g, TRUNK, pts[k], pts[k + 1], 27 - k * 2.2, steps=6)
    hub = (312, 0, 1075)
    A = sc.part(sway(6.0, hub, axis=90, rate=0.31, phase=r.random()))
    B = sc.part(sway(7.5, hub, axis=20, rate=0.37, phase=r.random()))
    for j in range(10):
        frond((A, B)[j % 2], LEAF if j % 2 else LEAF_D, hub, j * 36 + 8, 470 + r.uniform(-40, 40), 95, 310, 82, col2=LEAF)
    for (x, y, a) in ring(4, 26, 10):
        A.sphere(TRUNK_D, (hub[0] + x, hub[1] + y, hub[2] - 34), 22, steps=(3, 5))       # coconuts, 44 cm
    q = [(-230, 90, -200), (-250, 95, 180), (-285, 105, 380), (-330, 118, 540)]
    for k in range(3):
        rod(g, TRUNK, q[k], q[k + 1], 20 - k * 2.5, steps=6)
    hub2 = (-332, 119, 545)
    C = sc.part(sway(7.0, hub2, axis=60, rate=0.4, phase=r.random()))
    for j in range(7):
        frond(C, LEAF if j % 2 else LEAF_L, hub2, j * 51.4, 300, 80, 190, 62, segs=2)
    g.sphere(SAND, (-80, 40, -40), 260, scale=(1.5, 0.9, 0.3), steps=(4, 6))
    g.sphere(TRUNK_D, (120, -90, 14), 22, steps=(3, 5))                 # one fallen nut


# ------------------------------------------------------------------------------------------------ hedges and shrubs
def hibiscus(sc):                               # LOW: a clipped hedge thick with big pink flowers: the Strip, Hotel Row
    r = random.Random(14)
    g = sc.still()
    g.box(LEAF_D, (0, 0, 20), (540, 86, 160))
    L = sc.part(sway(1.5, (0, 0, 60), axis=0, rate=0.3, phase=0.2))
    for k in range(6):
        L.sphere((LEAF_D, LEAF)[k % 2], (-230 + k * 92, r.uniform(-8, 8), 92 + r.uniform(-6, 8)), 62, scale=(1.05, 0.8, 0.8), steps=(4, 6))
    parts = [sc.part(sway(6.0, (0, 0, 70), axis=0, rate=0.35 + 0.05 * k, phase=r.random())) for k in range(3)]
    for k in range(14):
        m = parts[k % 3]
        x = -250 + k * 38.5 + r.uniform(-8, 8)
        front = k % 3 != 2
        y = -52 if front else r.uniform(-20, 20)
        z = r.uniform(70, 118) if front else 142
        col = (PINK, PINK, CORAL, WHITE)[k % 4]
        plate(m, col, (x, y, z), 21, -90, 62 if front else 12, h=8)
        if front:
            spike(m, YEL, (x, y - 4, z + 2), (x, y - 20, z + 12), 4.5, 2.5)        # the stamen
        else:
            spike(m, YEL, (x, y, z + 4), (x, y - 4, z + 22), 4.5, 2.5)


def pergola(sc):                                # TALL: bougainvillea over a white pergola with a bench: Hotel Row, the Point
    r = random.Random(15)
    g = sc.still()
    g.box(CREAM, (0, 0, -52), (660, 330, 114))                          # the paved floor
    for x in (-280, 0, 280):
        for y in (-130, 130):
            g.box(WHITE, (x, y, 70), (18, 18, 420))
    for y in (-130, 130):
        g.box(WHITE, (0, y, 290), (700, 16, 24))
    for k in range(8):
        g.box(WHITE, (-315 + k * 90, 0, 310), (14, 350, 16))
    g.box(WHITE, (0, 96, 46), (190, 44, 8)); g.box(WHITE, (0, 120, 78), (190, 8, 60))          # the bench
    for x in (-80, 80):
        g.box(WHITE, (x, 96, 22), (10, 40, 44))
    tilt_box(g, TRUNK_D, (-272, -142, 0), (-262, -138, 300), 12, 12)    # the vine's stems up two posts
    tilt_box(g, TRUNK_D, (272, 142, 0), (262, 132, 300), 12, 12)
    top = sc.part(sway(1.2, (0, 0, 300), axis=0, rate=0.25, phase=0.4))
    for k, (x, y, s) in enumerate(((-250, -70, 1.0), (-120, 60, 1.15), (20, -60, 1.1), (160, 70, 1.2), (270, -50, 0.95), (-20, 120, 0.8))):
        top.sphere(PINK, (x, y, 340), 105 * s, scale=(1.3, 1.15, 0.45), steps=(4, 6))
    for (x, y) in ((-190, 110), (90, -120), (230, 120)):
        top.sphere(CORAL, (x, y, 362), 62, scale=(1.2, 1.1, 0.45), steps=(4, 6))
    for (x, y) in ((-300, 40), (60, 20), (310, 30)):
        top.sphere(LEAF_D, (x, y, 322), 80, scale=(1.2, 1.2, 0.4), steps=(4, 6))
    for k, (x, y) in enumerate(((-330, -140), (330, -140), (-330, 140), (330, 140), (-90, -146), (140, -146))):   # the hanging swags
        s = sc.part(sway(7.0, (x, y, 300), axis=0, rate=0.3 + 0.03 * k, phase=r.random()))
        drop = 190 if k < 4 else 110
        s.box(PINK, (x, y, 300 - drop * 0.25), (46, 30, drop * 0.5))
        s.box((CORAL, PINK)[k % 2], (x, y, 300 - drop * 0.7), (32, 24, drop * 0.45))
        s.box(LEAF_D, (x, y, 300 - drop * 0.98), (18, 16, drop * 0.2))
    for x in (-310, 310):                                               # a tub at each end
        g.cyl(TEAL, (x, 0, 6), 34, 50, steps=7, top_r=40)
        top.sphere(LEAF, (x, 0, 86), 40, steps=(3, 6))


def sea_grape(sc):                              # LOW: a broad bush of round leaves on a sand hummock: Marina, Point, Old Harbour
    r = random.Random(16)
    g = sc.still()
    g.sphere(SAND, (0, 0, -30), 230, scale=(1.3, 1.0, 0.32), steps=(4, 6))
    for (a, b) in (((-60, 0, 10), (-150, 30, 70)), ((40, 10, 10), (150, -20, 80)), ((0, 30, 10), (10, 120, 70))):
        tilt_box(g, TRUNK_D, a, b, 14, 14)
    A = sc.part(sway(2.5, (0, 0, 20), axis=0, rate=0.3, phase=r.random()))
    B = sc.part(sway(3.0, (0, 0, 20), axis=70, rate=0.36, phase=r.random()))
    lumps = ((-150, 20, 78, 74), (-40, -40, 96, 84), (80, 30, 90, 80), (170, -20, 72, 68), (0, 100, 76, 70), (-90, -90, 60, 56))
    for k, (x, y, z, rr) in enumerate(lumps):
        (A, B)[k % 2].sphere((LEAF_D, LEAF)[k % 2], (x, y, z), rr, scale=(1.1, 1.0, 0.78), steps=(4, 6))
    for k in range(14):
        x, y, z, rr = lumps[k % 6]
        a = r.uniform(0, 360)
        c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
        plate((A, B)[k % 2], (LEAF_L, LEAF)[k % 2], (x + c * rr * 0.95, y + s * rr * 0.9, z + r.uniform(0, 30)), 27, a, 55, h=8)
    for k in range(5):                                                  # the grapes, hanging
        x, y, z, rr = lumps[k]
        B.cyl(CORAL, (x + 20, y - rr * 0.95, z - 42), 4, 36, steps=5, top_r=13)


def agave(sc):                                  # LOW: agave and aloe rosettes in gravel: Pleasure Beach, the Point
    r = random.Random(17)
    g = sc.still()
    g.cyl(STONE, (0, 0, -60), 250, 72, steps=10)
    for (x, y, s) in ((200, -90, 34), (-210, 110, 28), (30, 190, 24), (-30, -200, 22)):
        g.sphere(STONE_D, (x, y, 16), s, scale=(1.2, 1, 0.7), steps=(3, 5))
    st = sc.part(sway(1.2, (-90, 0, 10), axis=30, rate=0.25, phase=0.1))
    for (x, y, a) in ring(8, 1, 0):                                     # the great agave
        c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
        spike(g, LEAF_L, (-90, 0, 14), (-90 + c * 128, s * 128, 52), 19, 3)
    for (x, y, a) in ring(6, 1, 30):
        c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
        spike(st, LEAF, (-90, 0, 18), (-90 + c * 78, s * 78, 118), 16, 3)
    spike(st, LEAF_D, (-90, 0, 18), (-90, 0, 140), 14, 3)
    fl = sc.part(sway(6.0, (120, 40, 12), axis=0, rate=0.4, phase=r.random()))
    for (x, y, a) in ring(9, 1, 10):                                    # the aloe
        c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
        spike(g, LEAF, (120, 40, 12), (120 + c * 66, 40 + s * 66, 62 + (a % 3) * 8), 12, 2.5)
    for k, (dx, col) in enumerate(((-8, CORAL), (12, YEL))):
        rod(fl, LEAF_D, (120 + dx, 40, 12), (120 + dx * 2.4, 40 + dx, 122 + k * 8), 3.5, steps=4)
        fl.cyl(col, (120 + dx * 2.4, 40 + dx, 118 + k * 8), 14, 44, steps=5, top_r=4)
    for (x, y, a) in ring(6, 1, 0):                                     # a pup
        c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
        spike(g, LEAF_L, (50, -120, 12), (50 + c * 50, -120 + s * 50, 44), 10, 2.5)


def bird_of_paradise(sc):                       # LOW: paddle leaves and crested yellow and coral flowers: Hotel Row
    r = random.Random(18)
    g = sc.still()
    g.cyl(CREAM, (0, 0, -60), 150, 78, steps=10)
    g.sphere(LEAF_D, (0, 0, 14), 120, scale=(1, 1, 0.35), steps=(3, 6))
    lp = [sc.part(sway(2.5, (0, 0, 15), axis=a, rate=0.3 + 0.05 * k, phase=r.random())) for k, a in enumerate((0, 90))]
    for k in range(12):
        a = k * 30 + r.uniform(-8, 8)
        c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
        r0, r1 = (55, 105) if k % 2 else (30, 60)
        z1 = 86 if k % 2 else 104
        m = lp[k % 2]
        tilt_box(m, LEAF_D, (c * 14, s * 14, 15), (c * r0, s * r0, z1), 8, 8)
        tilt_box(m, (LEAF, LEAF_L)[k % 3 == 0], (c * r0, s * r0, z1 - 4), (c * r1, s * r1, z1 + (50 if k % 2 else 62)), 36, 8)
    fp = [sc.part(sway(5.0, (0, 0, 15), axis=a, rate=0.42 + 0.05 * k, phase=r.random())) for k, a in enumerate((40, 130))]
    for k in range(6):
        a = k * 60 + 15
        c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
        rr = 42 + (k % 2) * 30
        z = 138 - (k % 2) * 22
        m = fp[k % 2]
        rod(m, LEAF_D, (c * 10, s * 10, 15), (c * rr, s * rr, z), 3.5, steps=4)
        e = (c * (rr + 34), s * (rr + 34), z + 4)
        tilt_box(m, CORAL, (c * rr, s * rr, z), e, 11, 11)             # the beak
        for j, col in enumerate((YEL, YEL, CORAL)):
            f = 0.25 + j * 0.3
            b = (c * (rr + 34 * f), s * (rr + 34 * f), z + 4)
            spike(m, col, b, (b[0] + c * (4 + j * 6), b[1] + s * (4 + j * 6), z + 30 - j * 3), 6, 2.2)
        spike(m, BLUE, (c * (rr + 26), s * (rr + 26), z + 6), (c * (rr + 50), s * (rr + 50), z + 16), 4.5, 2.2)


def topiary(sc):                                # TALL: a spiral, a three-ball standard and a cone, clipped, in tubs: Hotel Row, downtown
    g = sc.still()
    g.box(STONE, (0, 0, -57), (640, 130, 120))                          # a gravel strip
    for k, x in enumerate((-220, 0, 220)):
        g.box(CREAM, (x, 0, 30), (76, 76, 64))
        g.box(TEAL, (x, 0, 50), (82, 82, 10))
        g.box(LEAF_D, (x, 0, 63), (64, 64, 6))
    m = sc.part(sway(1.3, (-220, 0, 62), axis=20, rate=0.3, phase=0.1))  # the spiral
    rod(m, TRUNK_D, (-220, 0, 60), (-220, 0, 300), 6, steps=5)
    for j in range(7):
        a = math.radians(j * 75)
        rr = 62 - j * 6.2
        m.cyl(LEAF_D if j % 2 else LEAF, (-220 + math.cos(a) * 15, math.sin(a) * 15, 84 + j * 31), rr, 25, rot=(-12, j * 75, 0), steps=8)
    m.sphere(LEAF, (-220, 0, 312), 17, steps=(3, 6))
    m = sc.part(sway(1.3, (0, 0, 62), axis=80, rate=0.27, phase=0.5))    # the three balls
    rod(m, TRUNK_D, (0, 0, 60), (0, 0, 280), 6, steps=5)
    for (z, rr) in ((132, 54), (222, 41), (294, 29)):
        m.sphere(LEAF, (0, 0, z), rr, steps=(5, 8))
    m = sc.part(sway(1.3, (220, 0, 62), axis=140, rate=0.33, phase=0.8)) # the cone
    m.cyl(LEAF_D, (220, 0, 66), 60, 236, steps=8, top_r=7)
    m.sphere(LEAF, (220, 0, 306), 14, steps=(3, 6))
    for x in (-110, 110):                                               # white stones between the tubs
        g.sphere(WHITE, (x, 0, 8), 20, scale=(1, 1, 0.6), steps=(3, 5))


def fig_balls(sc):                              # TALL: clipped fig balls on stems in dark tubs with pink rims: downtown
    g = sc.still()
    for k, x in enumerate((-200, 0, 200)):
        g.cyl(NAVY, (x, 0, -60), 38, 116, steps=8, top_r=45)
        g.cyl(PINK, (x, 0, 54), 47, 8, steps=8)
        g.cyl(WHITE, (x, 0, 60), 37, 6, steps=8)                         # white flowers round the stem
        m = sc.part(sway(1.6, (x, 0, 58), axis=30 + k * 55, rate=0.28 + k * 0.03, phase=k * 0.31))
        rod(m, TRUNK, (x, 0, 58), (x, 0, 200), 6, steps=5)
        m.sphere(LEAF_D, (x, 0, 216), 62, steps=(6, 8))
        m.sphere(LEAF, (x + 10, -14, 250), 30, scale=(1, 1, 0.6), steps=(3, 6))   # where the light catches it


def prickly_pear(sc):                           # LOW: prickly pear in flower, on rock: the Point
    r = random.Random(19)
    g = sc.still()
    g.sphere(SAND, (0, 0, -30), 210, scale=(1.2, 1.0, 0.32), steps=(4, 6))
    g.sphere(STONE, (150, 60, 6), 52, scale=(1.2, 1, 0.6), steps=(3, 5))
    g.sphere(STONE_D, (-170, -50, 2), 40, scale=(1.2, 1, 0.6), steps=(3, 5))
    for k, (sx, sy, yaw) in enumerate(((-80, 20, 20), (50, -40, 100), (10, 90, 150))):
        g.sphere(LEAF, (sx, sy, 36), 31, scale=(1, 0.3, 1.25), steps=(3, 6), rot=(0, yaw, 0))
        m = sc.part(sway(1.4, (sx, sy, 60), axis=yaw, rate=0.3 + 0.04 * k, phase=r.random()))
        c, s = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
        tops = []
        for j, off in enumerate((-30, 30)):
            p = (sx + c * off, sy + s * off, 86)
            m.sphere((LEAF, LEAF_D)[j], p, 29, scale=(1, 0.3, 1.25), steps=(3, 6), rot=(0, yaw + 20 * (j * 2 - 1), 22 * (j * 2 - 1)))
            tops.append((p[0] + c * off * 0.3, p[1] + s * off * 0.3, 120))
        p = (sx + c * (-34 if k % 2 else 36), sy + s * (-34 if k % 2 else 36), 128)
        m.sphere(LEAF_L, p, 25, scale=(1, 0.3, 1.25), steps=(3, 6), rot=(0, yaw - 15, 0))
        tops.append((p[0], p[1], 157))
        for j, t in enumerate(tops):
            m.cyl((YEL, PINK, YEL)[(j + k) % 3], t, 5, 11, steps=5, top_r=10)       # a flower
            m.cyl(PINK, (t[0] + c * 14, t[1] + s * 14, t[2] - 8), 6, 12, steps=5)     # a fruit


def geraniums(sc):                              # LOW: pots of geraniums up three stone steps, a watering can: the Old Harbour
    r = random.Random(20)
    g = sc.still()
    for k in range(3):
        g.box(OCHRE, (0, -50 + k * 52, (26 * (k + 1) - 60) / 2.0), (320 - k * 16, 54, 26 * (k + 1) + 60))
    pots = ((-120, 0), (-30, 0), (110, 0), (-90, 1), (40, 1), (-20, 2), (100, 2))
    parts = [sc.part(sway(2.6, (0, -50 + k * 52, 26 * (k + 1)), axis=0, rate=0.3 + 0.05 * k, phase=r.random())) for k in range(3)]
    for j, (x, k) in enumerate(pots):
        y, z = -50 + k * 52, 26 * (k + 1)
        g.cyl((CREAM, TEAL, WHITE)[j % 3], (x, y, z), 15, 28, steps=6, top_r=20)
        m = parts[k]
        m.sphere(LEAF_D, (x, y, z + 42), 23, scale=(1, 1, 0.8), steps=(3, 5))
        for i, (dx, dy) in enumerate(((-10, -8), (11, -4), (0, 9))):
            m.cyl((RED, PINK, CORAL, WHITE)[(i + j) % 4], (x + dx, y + dy, z + 54 + i * 3), 10, 9, steps=6)
    g.cyl(TEAL, (135, -52, 26), 13, 26, steps=6)                         # the can
    tilt_box(g, TEAL, (146, -52, 36), (172, -52, 52), 6, 6)
    g.box(TEAL, (122, -52, 42), (6, 6, 22))


# ------------------------------------------------------------------------------------------------ the trees
def big_fig(sc):                                # TALL, few: the great fig of the quay, hanging roots, a stone seat round it
    r = random.Random(21)
    g = sc.still()
    g.cyl(TRUNK_D, (0, 0, -250), 125, 780, steps=8, top_r=78)
    for (x, y, a) in ring(5, 150, 20):                                  # the root columns grown into the trunk
        rod(g, TRUNK, (x * 1.15, y * 1.15, -200), (x * 0.5, y * 0.5, 470 + (a % 50)), 30, steps=6)
    for (x, y, a) in ring(6, 1, 50):
        c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
        tilt_box(g, TRUNK_D, (c * 300, s * 300, -70), (c * 70, s * 70, 190), 30, 110)       # the buttresses
    g.cyl(STONE, (0, 0, -60), 285, 106, steps=12)                        # the seat
    g.cyl(CREAM, (0, 0, 46), 296, 8, steps=12)
    pv = (0, 0, 500)
    crown = [sc.part(sway(1.0, pv, axis=a, rate=0.2 + 0.03 * k, phase=r.random())) for k, a in enumerate((10, 70, 130))]
    tips = []
    for k, (x, y, a) in enumerate(ring(6, 1, 0)):
        c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
        rr = 470 + (k % 2) * 90
        e = (c * rr, s * rr, 660 + (k % 3) * 30)
        rod(crown[k % 3], TRUNK_D, pv, e, 34, steps=6)
        tips.append((k, e))
        crown[k % 3].sphere((LEAF_D, LEAF)[k % 2], (e[0], e[1], e[2] + 150), 330 + (k % 2) * 40, scale=(1, 1, 0.55), steps=(5, 8))
    for k, (x, y, a) in enumerate(ring(3, 190, 30)):
        crown[k].sphere(LEAF_D, (x, y, 980), 360, scale=(1, 1, 0.55), steps=(5, 8))
    for k, (x, y, a) in enumerate(ring(5, 420, 45)):
        crown[k % 3].sphere(LEAF_L, (x, y, 1010 + (k % 2) * 60), 150, scale=(1.2, 1.2, 0.4), steps=(4, 6))
    for k in range(10):                                                 # the hanging roots, each swinging from its bough
        i, e = tips[k % 6]
        f = 0.55 + 0.4 * r.random()
        top = (e[0] * f + r.uniform(-40, 40), e[1] * f + r.uniform(-40, 40), 500 + (e[2] - 500) * f + 10)
        m = sc.part(sway(5.0, top, axis=r.uniform(0, 180), rate=0.3 + 0.2 * r.random(), phase=r.random()))
        ln = r.uniform(170, 330)
        rod(m, TRUNK, (top[0], top[1], top[2] - ln), top, 4.5, steps=4)
        m.sphere(TRUNK, (top[0], top[1], top[2] - ln), 9, steps=(3, 5))
    for (x, y, col) in ((230, -150, RED), (-240, -120, PINK)):          # two pots on the seat
        g.cyl(TEAL, (x, y, 54), 16, 26, steps=6, top_r=20)
        g.sphere(LEAF_D, (x, y, 94), 22, steps=(3, 5))
        g.cyl(col, (x, y, 110), 12, 8, steps=6)


def umbrella_pine(sc):                          # TALL: a bare leaning trunk and a flat dark crown: the Point
    r = random.Random(22)
    g = sc.still()
    rod(g, TRUNK_D, (0, 0, -200), (22, 0, 310), 31, steps=7)
    rod(g, TRUNK_D, (22, 0, 300), (52, 10, 630), 26, steps=7)
    g.sphere(STONE, (130, -60, 10), 60, scale=(1.3, 1, 0.6), steps=(3, 5))
    g.sphere(STONE_D, (-120, 70, 4), 44, scale=(1.2, 1, 0.6), steps=(3, 5))
    pv = (52, 10, 625)
    A = sc.part(sway(1.6, pv, axis=20, rate=0.24, phase=r.random()))
    B = sc.part(sway(1.9, pv, axis=100, rate=0.29, phase=r.random()))
    lobes = ((-230, 70, 890, 250), (270, -110, 930, 270), (110, 260, 900, 240), (-40, -230, 915, 230), (40, 30, 990, 310))
    for k, (x, y, z, rr) in enumerate(lobes):
        m = (A, B)[k % 2]
        rod(m, TRUNK_D, pv, (x * 0.8, y * 0.8, z - 40), 15, steps=5)
        m.sphere(LEAF_D, (x, y, z), rr, scale=(1, 1, 0.3), steps=(5, 8))
        m.sphere(LEAF, (x + 20, y - 20, z + rr * 0.2), rr * 0.62, scale=(1, 1, 0.26), steps=(4, 6))
    for (x, y) in ((-150, -90), (190, 120)):                            # cones
        A.cyl(TRUNK, (x, y, 815), 5, 26, steps=5, top_r=12)


def flame_tree(sc):                             # TALL: a low fork and a wide flat crown, all red: the Strip, Pleasure Beach
    r = random.Random(23)
    g = sc.still()
    rod(g, TRUNK_D, (0, 0, -200), (14, 0, 270), 31, steps=7)
    pv = (14, 0, 262)
    A = sc.part(sway(1.6, pv, axis=0, rate=0.26, phase=r.random()))
    B = sc.part(sway(1.9, pv, axis=85, rate=0.31, phase=r.random()))
    lobes = ((390, 60, 250), (-370, 120, 260), (80, 340, 240), (-90, -350, 245), (300, -280, 215), (-290, -220, 220), (-250, 330, 205))
    for k, (x, y, rr) in enumerate(lobes):
        m = (A, B)[k % 2]
        z = 575 + (k % 3) * 22
        if k < 4:
            rod(m, TRUNK_D, pv, (x * 0.8, y * 0.8, z - 40), 16 - k, steps=5)
        m.sphere(RED, (x, y, z), rr, scale=(1, 1, 0.3), steps=(5, 8))
        if k < 4:
            m.sphere(LEAF, (x * 0.85, y * 0.85, z - 52), rr * 0.75, scale=(1, 1, 0.22), steps=(4, 6))
    A.sphere(RED, (10, 0, 650), 300, scale=(1, 1, 0.3), steps=(5, 8))
    for (x, y) in ((120, 80), (-160, -40), (30, -190)):
        B.sphere(CORAL, (x, y, 716), 120, scale=(1, 1, 0.28), steps=(4, 6))
    for k, (x, y) in enumerate(((330, -40), (-300, 60), (60, 300), (-120, -300))):          # the long seed pods
        A.box(TRUNK_D, (x, y, 470), (9, 9, 72))


def mangrove(sc):                               # TALL: three trunks standing on arching stilt roots: Marina and Old Harbour, bay side
    r = random.Random(24)
    g = sc.still()
    g.cyl(SAND_D, (0, 0, -60), 350, 66, steps=10)
    for k, (x, y, hgt) in enumerate(((-160, 40, 330), (130, -70, 370), (20, 180, 300))):
        rod(g, TRUNK_D, (x, y, 140), (x, y, hgt), 15, steps=6)
        for j in range(7):
            a = math.radians(j * 51.4 + k * 20)
            c, s = math.cos(a), math.sin(a)
            knee = (x + c * 82, y + s * 82, 118 + (j % 2) * 26)
            tilt_box(g, TRUNK, (x, y, 165 + (j % 3) * 18), knee, 11, 11)
            tilt_box(g, TRUNK, knee, (x + c * 150, y + s * 150, -120), 11, 11)
        m = sc.part(sway(1.7, (x, y, hgt), axis=30 + k * 60, rate=0.25 + 0.04 * k, phase=r.random()))
        m.sphere(LEAF_D, (x, y, hgt + 70), 170 + k * 10, scale=(1, 1, 0.6), steps=(5, 8))
        m.sphere(LEAF, (x + 50, y - 40, hgt + 130), 105, scale=(1, 1, 0.5), steps=(4, 6))
        m.sphere(LEAF_L, (x - 70, y + 30, hgt + 118), 70, scale=(1, 1, 0.45), steps=(3, 6))
    for k in range(9):                                                  # the breathing roots, pegs in the mud
        a = math.radians(k * 40 + 10)
        rr = 250 + (k % 3) * 35
        g.cyl(TRUNK_D, (math.cos(a) * rr, math.sin(a) * rr, 0), 6, 28 + (k % 2) * 10, steps=4, top_r=2.5)


def quay_tree(sc):                              # TALL: a wind-bent tamarisk in pink plume, propped on a timber crutch: the Old Harbour
    r = random.Random(25)
    g = sc.still()
    pts = [(0, 0, -200), (30, 0, 165), (112, 0, 335), (240, 0, 445)]
    for k in range(3):
        rod(g, TRUNK_D, pts[k], pts[k + 1], 23 - k * 4, steps=6)
    tilt_box(g, WOOD, (190, 46, -120), (176, 8, 372), 12, 12)           # the crutch
    tilt_box(g, WOOD, (150, 0, 360), (205, 0, 384), 16, 34)
    g.sphere(STONE, (-70, 50, 8), 46, scale=(1.2, 1, 0.6), steps=(3, 5))
    pv = (240, 0, 440)
    A = sc.part(sway(2.2, pv, axis=90, rate=0.3, phase=r.random()))
    B = sc.part(sway(3.6, pv, axis=70, rate=0.38, phase=r.random()))
    lobes = ((420, 40, 540, 185), (330, -110, 575, 160), (170, 80, 580, 150), (520, -40, 500, 140), (300, 20, 630, 170))
    for k, (x, y, z, rr) in enumerate(lobes):
        rod(A, TRUNK_D, pv, (x * 0.92, y, z - 30), 9, steps=5)
        A.sphere((LEAF_L, LEAF)[k % 2], (x, y, z), rr, scale=(1.5, 0.9, 0.45), steps=(5, 8))
    for k, (x, y, z) in enumerate(((560, 60, 560), (640, -60, 510), (470, -150, 600), (400, 110, 640), (250, -40, 668), (130, 120, 610))):
        B.sphere((PINK, CORAL)[k % 3 == 2], (x, y, z), 44, scale=(1.7, 0.6, 0.5), steps=(4, 5), rot=(0, r.uniform(-25, 25), 0))


def reeds(sc):                                  # reeds, cattails and pampas plumes: Marina, Point, Old Harbour
    r = random.Random(26)
    g = sc.still()
    g.sphere(SAND, (0, 0, -30), 170, scale=(1.3, 0.9, 0.3), steps=(4, 6))
    parts = [sc.part(sway(5.0, (0, 0, -30), axis=a, rate=0.32 + 0.05 * k, phase=r.random())) for k, a in enumerate((0, 55, 110, 160))]
    for k in range(26):
        a = r.uniform(0, 360)
        d = r.uniform(10, 120)
        x, y = math.cos(math.radians(a)) * d * 1.2, math.sin(math.radians(a)) * d * 0.8
        h = r.uniform(130, 225)
        tilt_box(parts[k % 4], (LEAF, LEAF_D, LEAF_L)[k % 3], (x, y, -50), (x * 1.25 + r.uniform(-14, 14), y * 1.25, h), 9, 6)
    for k in range(6):                                                  # cattails
        x, y = r.uniform(-110, 110), r.uniform(-70, 70)
        h = r.uniform(170, 215)
        m = parts[k % 4]
        tilt_box(m, LEAF_D, (x, y, -50), (x * 1.15, y * 1.15, h), 6, 6)
        m.cyl(TRUNK_D, (x * 1.15, y * 1.15, h - 6), 7, 30, steps=5)
    pl = sc.part(sway(7.0, (0, 0, -30), axis=20, rate=0.4, phase=r.random()))
    for k, (x, y) in enumerate(((-90, 30), (-20, -40), (60, 40), (110, -20), (10, 70))):
        h = 215 + (k % 3) * 14
        tilt_box(pl, SAND_D, (x * 0.6, y * 0.6, -50), (x, y, h), 6, 6)
        pl.sphere((CREAM, WHITE)[k % 2], (x * 1.04, y * 1.04, h + 26), 19, scale=(1, 1, 2.3), steps=(4, 5))


# ------------------------------------------------------------------------------------------------ the flowerbed carpets (LOW, flat)
def _bed(sc, kerb=CREAM, L=700.0, W=360.0):
    g = sc.still()
    g.box(kerb, (0, 0, -23), (L, W, 74))
    g.box(LEAF_D, (0, 0, 16), (L - 30, W - 30, 6))
    return g


def _blooms(sc, spots, cols, seed):
    """The flowers that stand over the pattern and nod: two groups, across and along."""
    r = random.Random(seed)
    parts = [sc.part(sway(4.0, (0, 0, -60), axis=a, rate=0.35 + 0.07 * k, phase=r.random())) for k, a in enumerate((0, 90))]
    for k, (x, y) in enumerate(spots):
        parts[k % 2].sphere(cols[k % len(cols)], (x, y, 36), 17, scale=(1, 1, 0.7), steps=(3, 5))


def carpet_sun(sc):                             # a rising sun in hot pink and yellow: the Strip, Hotel Row
    g = _bed(sc)
    g.cyl(YEL, (0, -96, 14), 78, 16, steps=10)
    spots = []
    for k in range(9):
        a = math.radians(12 + k * 19.5)
        c, s = math.cos(a), math.sin(a)
        rr = 300 if k % 2 == 0 else 250
        tilt_box(g, (PINK, YEL)[k % 2], (c * 80, -96 + s * 80, 24), (c * rr * 1.04, -96 + s * rr * 0.84, 24), 40, 12)
        spots.append((c * rr * 0.72, -96 + s * rr * 0.6)); spots.append((c * rr * 0.98, -96 + s * rr * 0.8))
    for x in (-310, 310):
        g.cyl(LEAF, (x, -140, 18), 26, 58, steps=6, top_r=5)            # a clipped cone at each front corner
    _blooms(sc, spots, (PINK, YEL, WHITE), 31)


def carpet_wedge(sc):                           # two tent-striped rounds in red, yellow and white: Pleasure Beach, the Old Harbour
    g = _bed(sc)
    spots = []
    for cx in (-175, 175):
        for k in range(10):
            a = math.radians(k * 36 + 9)
            c, s = math.cos(a), math.sin(a)
            tilt_box(g, (RED, YEL, RED, WHITE, YEL)[k % 5] if cx < 0 else (YEL, RED, WHITE, RED, YEL)[k % 5], (cx + c * 30, s * 30, 24), (cx + c * 150, s * 150, 24), 62, 12)
            spots.append((cx + c * 118, s * 118))
        g.cyl(WHITE, (cx, 0, 18), 46, 20, steps=8)
        g.cyl(RED, (cx, 0, 38), 18, 16, steps=6, top_r=6)
    g.box(YEL, (0, 0, 24), (60, 60, 12), rot=(0, 45, 0))
    _blooms(sc, spots, (RED, YEL, WHITE), 32)


def carpet_wave(sc):                            # three waves in harbour blue, white and sky: the Marina, the Point
    g = _bed(sc, kerb=WHITE)
    spots = []
    for j, (y0, col) in enumerate(((-110, BLUE), (0, WHITE), (110, SKY))):
        prev = None
        for k in range(11):
            x = -320 + k * 64
            y = y0 + 34 * math.sin(k * 1.1 + j * 0.6)
            if prev:
                tilt_box(g, col, (prev[0], prev[1], 24), (x, y, 24), 44, 12)
                if k % 2:
                    spots.append(((prev[0] + x) / 2.0, (prev[1] + y) / 2.0))
            prev = (x, y)
    _blooms(sc, spots, (WHITE, SKY, BLUE), 33)


def carpet_neon(sc):                            # diamonds in hot pink and pool turquoise: downtown
    g = _bed(sc)
    spots = []
    for k in range(5):
        x = -260 + k * 130
        g.box((PINK, POOL)[k % 2], (x, 0, 24), (132, 132, 12), rot=(0, 45, 0))
        g.box((POOL, PINK)[k % 2], (x, 0, 30), (74, 74, 12), rot=(0, 45, 0))
        g.cyl(YEL, (x, 0, 34), 14, 12, steps=6)
        spots += [(x, 52), (x, -52), (x - 40, 0), (x + 40, 0)]
    for x in (-195, -65, 65, 195):
        for y in (-128, 128):
            g.box(WHITE, (x, y, 24), (70, 34, 12))
    _blooms(sc, spots, (PINK, POOL, WHITE), 34)


JOBS = [('SM_S1PLANTS_RoyalPalms', royal_palms), ('SM_S1PLANTS_FanPalm', fan_palm), ('SM_S1PLANTS_CocoPalm', coco_palm),
        ('SM_S1PLANTS_Hibiscus', hibiscus, LOW), ('SM_S1PLANTS_Pergola', pergola), ('SM_S1PLANTS_SeaGrape', sea_grape, LOW),
        ('SM_S1PLANTS_Agave', agave, LOW), ('SM_S1PLANTS_BirdOfParadise', bird_of_paradise, LOW), ('SM_S1PLANTS_Topiary', topiary),
        ('SM_S1PLANTS_FigBalls', fig_balls), ('SM_S1PLANTS_PricklyPear', prickly_pear, LOW), ('SM_S1PLANTS_Geraniums', geraniums, LOW),
        ('SM_S1PLANTS_BigFig', big_fig), ('SM_S1PLANTS_UmbrellaPine', umbrella_pine), ('SM_S1PLANTS_FlameTree', flame_tree),
        ('SM_S1PLANTS_Mangrove', mangrove), ('SM_S1PLANTS_QuayTree', quay_tree), ('SM_S1PLANTS_Reeds', reeds),
        ('SM_S1PLANTS_CarpetSun', carpet_sun, LOW), ('SM_S1PLANTS_CarpetWedge', carpet_wedge, LOW),
        ('SM_S1PLANTS_CarpetWave', carpet_wave, LOW), ('SM_S1PLANTS_CarpetNeon', carpet_neon, LOW)]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
