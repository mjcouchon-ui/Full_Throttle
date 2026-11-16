"""Volcano, Stage 1, builder s1d6: district 6 "The Far Works" = GEOTHERM, the steam power station (16.7 to 19.5 km).
/Game/World/Volcano/SM_S1D6_*. Run: import ft_world_volcano_s1d6 as K; K.run()

Big smooth concrete curves: hourglass cooling towers, ball tanks in cradles, a round-ended turbine hall, fat pipes on
trestles. White bodies, lime bands and UP chevrons (never an arrow along the road), graphite blue bases, silver pipe.
No pipe end aims at the road: every pipe elbows down into the ground or runs away from the road. Nothing here is a
vehicle, a tank on wheels, a hose, a box-sized glowing thing or anything red, orange or green-box-like.

Every piece: ft_people.Scene, one section, M_People. cm; z = 0 is the land; FRONT faces -Y; long pieces run along X.
Placed by C:/ftwork/s1/L3_s1d6.inl (the block's header says which piece stands where and why).
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Volcano'
W, LIME, GRAPH, SILV = '#f2f4ee', '#b6f03a', '#3a4a66', '#c9c4d6'       # the district's four
CONC, PANE = '#9a98a8', '#14101a'                                       # the kit's concrete (plinths) and pane (windows)
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))


# ------------------------------------------------------------------------------------------ helpers
def _hg(rb, rw, rt, H, waist=0.72):
    """The radius of an hourglass shell H tall at height z: rb at the foot, rw at the waist, rt at the rim."""
    kl, ku = (rb / rw) ** 2 - 1.0, (rt / rw) ** 2 - 1.0

    def rad(z):
        t = max(0.0, min(1.0, z / H))
        u = (waist - t) / waist if t < waist else (t - waist) / (1.0 - waist)
        return rw * math.sqrt(1.0 + (kl if t < waist else ku) * u * u)
    return rad


def _shell(g, col, x, y, zb, H, rad, steps=12, ts=(0.0, 0.18, 0.42, 0.72, 0.88, 1.0)):
    for t0, t1 in zip(ts, ts[1:]):
        g.cyl(col, (x, y, zb + t0 * H), rad(t0 * H), (t1 - t0) * H, steps=steps, top_r=rad(t1 * H))


def _chev_ring(g, col, x, y, zb, z0, z1, rad, n, th, bar, out):
    """A band of n chevrons round a shell, every one pointing UP (feet at z0, points at z1)."""
    def pt(a, z):
        r = rad(z) + out
        return (x + math.cos(a) * r, y + math.sin(a) * r, zb + z)
    for k in range(n):
        a0 = 6.28318 * k / n
        am, a1 = a0 + 3.14159 / n, a0 + 6.28318 / n
        tilt_box(g, col, pt(a0, z0), pt(am, z1), th, bar)
        tilt_box(g, col, pt(a1, z0), pt(am, z1), th, bar)


def _chev(g, col, cx, y, cz, w, h, bar, th=8.0):
    """One chevron on a face at y, pointing UP: w wide, h tall, its strokes `bar` wide."""
    tilt_box(g, col, (cx - w / 2.0, y, cz - h / 2.0), (cx, y, cz + h / 2.0), th, bar)
    tilt_box(g, col, (cx + w / 2.0, y, cz - h / 2.0), (cx, y, cz + h / 2.0), th, bar)


def _pipe(g, pts, r, col=SILV, steps=8):
    """A pipe through the points, a ball at every bend."""
    for a, b in zip(pts, pts[1:]):
        rod(g, col, a, b, r, steps=steps)
    for p in pts[1:-1]:
        g.sphere(col, p, r * 1.12, steps=(4, 8))


def _puff(sc, x, y, z, r, k, ph=0.0, rate=0.1, bob=None, lean=0.5):
    """One puff of pale steam (puff k of a plume): it floats up and down and rolls a little, on its own clock."""
    p = sc.part(P.motion(phase=0.37 * k + 0.11 + ph, rate=rate + 0.017 * k, bob=-(bob if bob is not None else r * 0.22) * (1.0 + 0.3 * k),
                         swing=3.0, pivot=(x, y, z - r * 3.0), axis=90.0))
    p.sphere(W, (x + k * r * lean, y, z + k * r * 0.9), r * (1.0 + 0.22 * k), scale=(1.25, 1.0, 0.62), steps=(4, 7))


def _cowl(sc, x, y, z, r=45.0):
    """A roof ventilator that turns: a squat drum with four short fins hugging it (not blades), on a neck."""
    sc.still().cyl(SILV, (x, y, z), r * 0.45, r * 0.6, steps=8)
    c = sc.part(P.motion(base=(x, y), travel=(-1.0, 0.12)))
    c.cyl(SILV, (x, y, z + r * 0.6), r, r * 0.9, steps=8, top_r=r * 0.7)
    for k in range(4):
        a = math.radians(k * 90.0)
        c.box(LIME, (x + math.cos(a) * r, y + math.sin(a) * r, z + r * 1.05), (r * 0.5, 8, r * 0.8), rot=(0, k * 90.0, 0))
    c.cyl(LIME, (x, y, z + r * 1.5), r * 0.75, r * 0.2, steps=8, top_r=r * 0.3)


def _vane(sc, x, y, z, s=1.0):
    """A weather vane: a lime plate and a white counterweight on a pin, hunting slowly either way."""
    sc.still().cyl(SILV, (x, y, z), 5, 50 * s, steps=6)
    v = sc.part(P.motion(swing=40, pivot=(x, y, z), axis=P.UP, rate=0.09, phase=0.2))
    v.box(LIME, (x + 62 * s, y, z + 62 * s), (110 * s, 8, 56 * s))
    v.box(W, (x - 46 * s, y, z + 62 * s), (56 * s, 8, 22 * s))


def _tower_base(g, x, y, z, rb, hl, n=6):
    """A cooling tower's foot: a round plinth, a dark recess and white piers the shell stands on."""
    g.cyl(CONC, (x, y, -300 if z > 100 else -200), rb + 70, z - (-300 if z > 100 else -200), steps=12)
    g.cyl(GRAPH, (x, y, z), rb * 0.88, hl, steps=12)
    for k in range(n):
        d = k * 360.0 / n + 30.0
        a = math.radians(d)
        g.box(W, (x + math.cos(a) * rb * 0.95, y + math.sin(a) * rb * 0.95, z + hl / 2.0), (70, rb * 0.2, hl), rot=(0, d, 0))
    g.box(GRAPH, (x, y - rb * 0.9, z + hl * 0.42), (rb * 0.22, 80, hl * 0.84))          # the way in, on the front


# ------------------------------------------------------------------------------------------ the hero
def three_sisters(sc):
    """THE THREE SISTERS: three white hourglass cooling towers, 56, 48 and 40 m, on one graphite base, a broad band of
    lime chevrons (all pointing up) between two lime hoops on each, a ring walkway round every waist at 33 m joined by
    two bridges, pale steam rolling off the rims. 63 m long, 26 m deep."""
    g = sc.still()
    g.plan(GRAPH, [(-3300, -650), (-1400, -1260), (1300, -1260), (2980, -600), (2980, 600), (1300, 1300), (-1400, 1300), (-3300, 650)], -300, 60)
    WALK = 3300.0
    ring = []
    for i, (x, y, H, rb, rw, rt) in enumerate(((0, 60, 5600, 1120, 640, 760), (-2230, -40, 4800, 960, 550, 650), (2070, -80, 4000, 800, 460, 545))):
        hl = 0.07 * H
        zb, Hs = 120.0 + hl, H - hl
        rad = _hg(rb, rw, rt, Hs)
        _tower_base(g, x, y, 120.0, rb, hl)
        _shell(g, W, x, y, zb, Hs, rad)
        g.cyl(LIME, (x, y, zb + Hs - 70), rad(Hs - 70) + 16, 80, steps=12, top_r=rt + 16)             # the rim's lip
        z0, z1 = 0.27 * Hs, 0.40 * Hs
        for zh in (z0 - 0.035 * Hs, z1 + 0.012 * Hs):
            g.cyl(LIME, (x, y, zb + zh), rad(zh) + 12, 0.02 * Hs, steps=12, top_r=rad(zh + 0.02 * Hs) + 12)
        _chev_ring(g, LIME, x, y, zb, z0, z1, rad, 6, 50, 0.028 * H, 35)
        r = rad(WALK - zb)
        g.cyl(GRAPH, (x, y, WALK - 90), r + 20, 90, steps=12, top_r=r + 175)                           # the ring walkway
        g.cyl(GRAPH, (x, y, WALK), r + 175, 110, steps=12)
        g.cyl(LIME, (x, y, WALK + 110), r + 180, 16, steps=12)
        ring.append((x, y, r + 150))
        for k in range(3):
            _puff(sc, x, y, zb + Hs + rt * 0.25, rt * 0.5, k, ph=0.23 * i)
    ax, ay, ar = ring[0]
    for bx, by, br in ring[1:]:                                                                        # the two bridges
        d = math.hypot(bx - ax, by - ay)
        ux, uy = (bx - ax) / d, (by - ay) / d
        a, b = (ax + ux * ar, ay + uy * ar), (bx - ux * br, by - uy * br)
        tilt_box(g, GRAPH, (a[0], a[1], WALK + 55), (b[0], b[1], WALK + 55), 170, 110)
        tilt_box(g, LIME, (a[0], a[1], WALK + 118), (b[0], b[1], WALK + 118), 180, 16)


# ------------------------------------------------------------------------------------------ buildings, the left (60 to 69 m)
def turbine_hall(sc):
    """The turbine hall: round-ended, graphite, a lime stripe and a lit ribbon window along it, a white roof with four
    mushroom vents, two steaming stacks, a lime door between two up chevrons, a fat pipe out of each end into the ground."""
    g = sc.still()
    L, R = 1600.0, 500.0

    def stadium(col, z, h, more=0.0):
        g.box(col, (0, 0, z + h / 2.0), (L, 2.0 * (R + more), h))
        for sx in (-1, 1):
            g.cyl(col, (sx * L / 2.0, 0, z), R + more, h, steps=12)
    stadium(CONC, -200, 260, 50)
    stadium(GRAPH, 60, 1100)
    stadium(LIME, 640, 90, 7)
    stadium(PANE, 820, 150, 5)
    stadium(W, 1160, 70, 35)
    for k in range(7):
        g.box(W, (-690 + k * 230, -R - 7, 895), (150, 6, 96), glow=0.45)
    g.box(W, (0, -R - 6, 255), (380, 12, 410))
    g.box(LIME, (0, -R - 12, 240), (300, 12, 360))
    g.box(GRAPH, (0, -R - 19, 240), (8, 6, 360))
    g.box(W, (0, -R - 80, 478), (460, 150, 14))                                  # the door's hood
    g.box(CONC, (0, -R - 110, 0), (420, 130, 60))                                # a step
    for k in range(3):
        g.box(LIME, (250 + k * 44, -R - 8, 420), (30, 6, 30))                    # the hall's number: three squares
    for sx in (-1, 1):
        _chev(g, LIME, sx * 520, -R - 6, 360, 300, 230, 60)
        _pipe(g, [(sx * (L / 2.0 + R - 40), 0, 420), (sx * (L / 2.0 + R + 150), 0, 420), (sx * (L / 2.0 + R + 150), 0, -150)], 70)
        rod(g, LIME, (sx * (L / 2.0 + R + 8), 0, 420), (sx * (L / 2.0 + R + 38), 0, 420), 92, steps=8)
    for k in range(4):
        x = -600 + k * 400
        g.cyl(SILV, (x, -120, 1230), 55, 90, steps=8)
        g.cyl(SILV, (x, -120, 1320), 110, 45, steps=8, top_r=60)
    for i, x in enumerate((-520, 520)):
        g.cyl(W, (x, 230, 1230), 95, 560, steps=10, top_r=80)
        g.cyl(LIME, (x, 230, 1640), 90, 60, steps=10)
        for k in range(2):
            _puff(sc, x, 230, 1890, 105, k, ph=0.4 * i)


def ball_tank(sc):
    """A ball tank in a cradle: a 9 m white ball with a lime belt on six graphite legs, a spiral stair with a lime rail
    climbing round it to a landing, a vent and a weather vane on top."""
    g = sc.still()
    g.cyl(CONC, (0, 0, -200), 540, 260, steps=12)
    for k in range(6):
        a = math.radians(k * 60.0 + 30.0)
        tilt_box(g, GRAPH, (math.cos(a) * 470, math.sin(a) * 470, 60), (math.cos(a) * 330, math.sin(a) * 330, 640), 44, 44)
    g.cyl(GRAPH, (0, 0, 600), 350, 70, steps=12)
    g.sphere(W, (0, 0, 1010), 450, steps=(8, 12))
    g.cyl(LIME, (0, 0, 975), 455, 70, steps=12)
    g.cyl(LIME, (0, 0, 1440), 120, 22, steps=10)
    g.cyl(SILV, (0, 0, 1455), 34, 70, steps=8)
    rod(g, SILV, (0, 0, 600), (0, 0, -150), 50, steps=8)
    rod(g, LIME, (0, 0, 300), (0, 0, 340), 68, steps=8)
    prev = None
    for k in range(15):
        d = -170.0 + k * 11.0
        a = math.radians(d)
        z = 90.0 + k * 62.0
        g.box(W, (math.cos(a) * 525, math.sin(a) * 525, z), (130, 50, 14), rot=(0, d, 0))
        p = (math.cos(a) * 588, math.sin(a) * 588, z + 95.0)
        if prev:
            tilt_box(g, LIME, prev, p, 8, 8)
        prev = p
    g.box(SILV, (math.cos(-0.09) * 520, math.sin(-0.09) * 520, 1000), (150, 130, 14))
    _vane(sc, 0, 0, 1525)


def control_stalk(sc):
    """The control block on a stalk: a white drum cabin with a wrap-round window and a lit console line, a lime roof
    rim, a low dome with a turning vent, on a tapering white stalk; a ladder cage up the back, a lime door."""
    g = sc.still()
    g.cyl(CONC, (0, 0, -200), 420, 280, steps=10)
    g.cyl(W, (0, 0, 80), 170, 1040, steps=10, top_r=140)
    g.cyl(LIME, (0, 0, 330), 170, 50, steps=10)
    g.cyl(GRAPH, (0, 0, 1100), 150, 210, steps=12, top_r=500)
    g.cyl(SILV, (0, 0, 1310), 610, 14, steps=12)
    g.cyl(GRAPH, (0, 0, 1324), 606, 70, steps=12)
    g.cyl(W, (0, 0, 1324), 520, 320, steps=12)
    g.cyl(PANE, (0, 0, 1440), 526, 130, steps=12)
    g.cyl(W, (0, 0, 1462), 529, 20, steps=12, glow=0.45)
    g.cyl(LIME, (0, 0, 1644), 575, 40, steps=12)
    g.sphere(W, (0, 0, 1684), 420, scale=(1.0, 1.0, 0.3), steps=(4, 12))
    _cowl(sc, 0, 0, 1795, 50)
    for (x, y, h) in ((260, 130, 210), (-240, 160, 150)):
        g.cyl(SILV, (x, y, 1700), 6, h, steps=6)
        g.sphere(W, (x, y, 1700 + h), 14, steps=(3, 6))
    g.box(GRAPH, (0, 190, 640), (70, 50, 1120))
    g.box(LIME, (0, -172, 190), (110, 14, 210))
    g.box(W, (0, -205, 305), (170, 70, 12))
    g.box(CONC, (0, -230, 50), (170, 90, 40))


def switch_hall(sc):
    """The switch hall: a graphite block under a white coping, a row of up chevrons and a lit strip along its front, a
    comb of five tall white bushings on the roof under one silver bar, a finned cooler on its end."""
    g = sc.still()
    g.box(CONC, (0, 0, -70), (1480, 880, 260))
    g.box(GRAPH, (0, 0, 385), (1400, 800, 650))
    g.box(W, (0, 0, 735), (1440, 840, 50))
    g.box(PANE, (0, -402, 570), (1200, 6, 90))
    for k in range(5):
        g.box(W, (-480 + k * 240, -405, 570), (150, 6, 60), glow=0.45)
    for k in range(4):
        _chev(g, LIME, -560 + k * 250, -404, 320, 190, 150, 44)
    g.box(W, (540, -404, 215), (240, 8, 320))
    g.box(LIME, (540, -408, 205), (190, 8, 290))
    g.box(W, (540, -450, 385), (280, 90, 12))
    for k in range(5):
        x = -480 + k * 240
        g.cyl(GRAPH, (x, 0, 760), 70, 60, steps=8)
        g.cyl(W, (x, 0, 820), 34, 470, steps=8, top_r=24)
        for j in range(3):
            g.cyl(W, (x, 0, 900 + j * 130), 84, 34, steps=8, top_r=50)
        g.sphere(SILV, (x, 0, 1315), 48, steps=(3, 6))
    g.box(SILV, (0, 0, 1300), (1000, 14, 14))
    g.box(W, (760, 0, 300), (90, 520, 420))
    for j in range(5):
        g.box(GRAPH, (812, -200 + j * 100, 300), (20, 30, 400))
    _pipe(g, [(-700, 200, 600), (-800, 200, 600), (-800, 200, -100)], 40)
    g.box(SILV, (-300, 200, 790), (120, 120, 60))                                # a roof hatch


def valve_house(sc):
    """The valve house: a low white drum with a lime belt under a graphite cone roof, a lantern with a turning vent,
    and eight silver pipes that leave its wall and elbow straight down into the ground (none ends in the open)."""
    g = sc.still()
    g.cyl(CONC, (0, 0, -200), 690, 260, steps=12)
    g.cyl(W, (0, 0, 60), 600, 450, steps=12)
    g.cyl(GRAPH, (0, 0, 60), 606, 90, steps=12)
    g.cyl(LIME, (0, 0, 360), 606, 70, steps=12)
    g.cyl(GRAPH, (0, 0, 510), 650, 150, steps=12, top_r=210)
    g.cyl(W, (0, 0, 660), 200, 110, steps=10)
    g.cyl(LIME, (0, 0, 770), 240, 50, steps=10, top_r=120)
    _cowl(sc, 0, 0, 815, 42)
    for k in range(8):
        a = math.radians(22.5 + 45.0 * k)
        c, s = math.cos(a), math.sin(a)
        _pipe(g, [(c * 560, s * 560, 250), (c * 760, s * 760, 250), (c * 760, s * 760, -150)], 46)
        rod(g, LIME, (c * 604, s * 604, 250), (c * 630, s * 630, 250), 64, steps=8)
    g.box(LIME, (0, -600, 190), (170, 16, 250))
    g.box(W, (0, -640, 330), (240, 90, 12))


# ------------------------------------------------------------------------------------------ buildings, the right (behind the railway)
def small_tower(sc):
    """A single small cooling tower, 22 m: the Sisters' little cousin, the same lime chevron band, a silver gallery."""
    g = sc.still()
    H, rb, rw, rt, hl = 2200.0, 560.0, 330.0, 395.0, 170.0
    zb, Hs = 60.0 + hl, H - hl
    rad = _hg(rb, rw, rt, Hs)
    _tower_base(g, 0, 0, 60.0, rb, hl)
    _shell(g, W, 0, 0, zb, Hs, rad)
    g.cyl(LIME, (0, 0, zb + Hs - 40), rad(Hs - 40) + 10, 46, steps=12, top_r=rt + 10)
    z0, z1 = 0.27 * Hs, 0.41 * Hs
    for zh in (z0 - 0.04 * Hs, z1 + 0.015 * Hs):
        g.cyl(LIME, (0, 0, zb + zh), rad(zh) + 8, 0.022 * Hs, steps=12, top_r=rad(zh + 0.022 * Hs) + 8)
    _chev_ring(g, LIME, 0, 0, zb, z0, z1, rad, 6, 34, 64, 22)
    zg = 0.8 * Hs
    g.cyl(SILV, (0, 0, zb + zg), rad(zg) + 90, 12, steps=12)
    g.cyl(GRAPH, (0, 0, zb + zg + 12), rad(zg) + 92, 60, steps=12)
    for k in range(3):
        _puff(sc, 0, 0, zb + Hs + 110, 200, k)


def tank_farm(sc):
    """The tank farm: three white tanks of three heights with domed tops and lime belts in a graphite bund, an up
    chevron on each, a silver manifold along the front with three small lime wheels, a ladder and a landing."""
    g = sc.still()
    g.box(CONC, (0, 0, -80), (1500, 700, 280))
    for y in (-335, 335):
        g.box(GRAPH, (0, y, 95), (1500, 30, 70))
    for x in (-735, 735):
        g.box(GRAPH, (x, 0, 95), (30, 700, 70))
    for (x, r, h, b) in ((-480, 230, 780, 0.5), (0, 265, 1080, 0.62), (480, 215, 1380, 0.42)):
        g.cyl(W, (x, 40, 60), r, h, steps=10)
        g.cyl(GRAPH, (x, 40, 60), r + 6, 60, steps=10)
        g.cyl(LIME, (x, 40, 60 + h * b), r + 6, 70, steps=10)
        g.sphere(W, (x, 40, 60 + h), r, scale=(1.0, 1.0, 0.4), steps=(4, 10))
        g.cyl(SILV, (x, 40, 60 + h + r * 0.36), 26, 60, steps=6)
        _chev(g, LIME, x, 40 - r - 14, 60 + h * b + 190, 190, 140, 40, th=14)
        rod(g, SILV, (x, -290, 230), (x, 40 - r + 20, 230), 34, steps=6)
        rod(g, LIME, (x - 70, -290, 230), (x - 50, -290, 230), 60, steps=8)
        g.cyl(SILV, (x, -290, 270), 8, 34, steps=6)
        g.cyl(LIME, (x, -290, 304), 34, 8, steps=8)
    _pipe(g, [(-700, -290, -100), (-700, -290, 230), (700, -290, 230), (700, -290, -100)], 44)
    g.box(GRAPH, (480 + 235, 40, 750), (30, 50, 1380))
    g.cyl(SILV, (480, 40, 1425), 265, 12, steps=10)
    _puff(sc, 480, 40, 1600, 60, 0)


def separator(sc):
    """The separator: a white cone-bottomed vessel with a lime belt and an up chevron in a graphite frame, a fat silver
    steam main up out of its dome, over and down the BACK (away from the road) into the ground, a landing and a ladder."""
    g = sc.still()
    g.box(CONC, (0, 120, -80), (700, 940, 280))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(GRAPH, (sx * 270, sy * 270, 410), (36, 36, 700))
        tilt_box(g, GRAPH, (sx * 270, -270, 80), (sx * 270, 270, 740), 20, 20)
        tilt_box(g, GRAPH, (-270, sx * 270, 80), (270, sx * 270, 740), 20, 20)
    g.box(GRAPH, (0, 0, 770), (620, 620, 30))
    g.cyl(W, (0, 0, 400), 70, 380, steps=12, top_r=300)
    g.cyl(W, (0, 0, 780), 300, 640, steps=12)
    g.sphere(W, (0, 0, 1420), 300, scale=(1.0, 1.0, 0.45), steps=(4, 12))
    g.cyl(LIME, (0, 0, 1180), 306, 80, steps=12)
    _chev(g, LIME, 0, -316, 980, 260, 190, 50, th=16)
    _pipe(g, [(0, 0, 1530), (0, 0, 1700), (0, 480, 1700), (0, 480, -150)], 62)
    rod(g, LIME, (0, 150, 1700), (0, 180, 1700), 82, steps=8)
    rod(g, LIME, (0, 480, 900), (0, 480, 930), 82, steps=8)
    rod(g, SILV, (0, 0, 400), (0, 0, -100), 50, steps=8)
    rod(g, LIME, (0, 0, 250), (0, 0, 280), 66, steps=8)
    g.box(SILV, (0, -340, 791), (520, 100, 12))
    g.box(GRAPH, (0, -386, 890), (520, 6, 6))
    for x in (-255, 0, 255):
        g.box(GRAPH, (x, -386, 842), (6, 6, 96))
    g.box(GRAPH, (300, -300, 420), (40, 30, 720))
    g.cyl(SILV, (160, -100, 1500), 20, 120, steps=6)
    for k in range(2):
        _puff(sc, 160, -100, 1690, 70, k)


def pipe_office(sc):
    """The pipe-bridge office: a white cabin with a lime roof and a weather vane on a graphite trestle that STRADDLES
    the mine's railway (placed 66 m out: legs at 62.0 and 70.2 m, 6.9 m clear over the rails), two fat pipes carried
    across beside it that drop into the ground at both ends, a board of three up chevrons under the deck."""
    g = sc.still()
    Y0, Y1 = -400.0, 420.0
    for x in (-300, 300):
        for y in (Y0, Y1):
            g.box(GRAPH, (x, y, 275), (44, 44, 950))
            g.box(CONC, (x, y, -80), (110, 110, 260))
        g.box(GRAPH, (x, 10, 725), (44, Y1 - Y0 + 44, 60))
        tilt_box(g, GRAPH, (x, Y0, 540), (x, Y0 + 170, 700), 20, 20)
        tilt_box(g, GRAPH, (x, Y1, 540), (x, Y1 - 170, 700), 20, 20)
    for y in (Y0, Y1):
        g.box(GRAPH, (0, y, 725), (644, 36, 50))
        tilt_box(g, GRAPH, (-300, y, 120), (300, y, 690), 16, 16)
        tilt_box(g, GRAPH, (300, y, 120), (-300, y, 690), 16, 16)
    g.box(CONC, (0, 10, 768), (700, 900, 26))
    for x, col in ((-250, SILV), (-100, W)):
        _pipe(g, [(x, -440, -150), (x, -440, 860), (x, 560, 860), (x, 560, -150)], 58, col=col)
        rod(g, LIME, (x, -200, 860), (x, -170, 860), 78, steps=8)
        rod(g, LIME, (x, 250, 860), (x, 280, 860), 78, steps=8)
    g.box(W, (170, 0, 911), (330, 460, 260))
    g.box(PANE, (170, -232, 945), (270, 6, 90))
    for x in (100, 240):
        g.box(W, (x, -235, 945), (100, 6, 60), glow=0.45)
    g.box(PANE, (337, 0, 945), (6, 360, 90))
    g.box(LIME, (170, 0, 1056), (390, 520, 30))
    g.box(LIME, (170, 236, 890), (90, 8, 200))
    g.box(GRAPH, (170, -455, 690), (330, 12, 120))
    for k in range(3):
        _chev(g, LIME, 70 + k * 100, -463, 690, 80, 70, 22, th=6)
    for x in (30, 310):
        g.box(GRAPH, (x, -455, 752), (8, 8, 30))
    g.box(GRAPH, (150, -430, 880), (400, 6, 6))
    for x in (-40, 150, 340):
        g.box(GRAPH, (x, -430, 832), (6, 6, 96))
    g.box(GRAPH, (330, 480, 340), (50, 30, 880))                                  # the ladder, at the back
    _vane(sc, 250, 120, 1071)


# ------------------------------------------------------------------------------------------ props, low (under 2 m)
def valve_wheel(sc):
    """A big valve wheel on a pipe stub: silver pipe with blind lime flanges on two blocks, a lime wheel facing the
    road that turns a little either way on its own."""
    g = sc.still()
    for x in (-120, 120):
        g.box(CONC, (x, 0, 5), (50, 110, 70))
    rod(g, SILV, (-190, 0, 82), (190, 0, 82), 42, steps=8)
    for sx in (-1, 1):
        rod(g, LIME, (sx * 190, 0, 82), (sx * 204, 0, 82), 58, steps=8)
        rod(g, LIME, (sx * 62, 0, 82), (sx * 76, 0, 82), 56, steps=8)
    g.box(SILV, (0, 0, 88), (110, 100, 112))
    rod(g, SILV, (0, -40, 112), (0, -86, 112), 10, steps=6)
    w = sc.part(P.motion(swing=22, pivot=(0, -80, 112), axis=90.0, rate=0.07, phase=0.3))
    for k in range(10):
        a0, a1 = 0.628318 * k, 0.628318 * (k + 1)
        tilt_box(w, LIME, (math.cos(a0) * 72, -80, 112 + math.sin(a0) * 72), (math.cos(a1) * 72, -80, 112 + math.sin(a1) * 72), 12, 12)
    for k in range(3):
        a = math.radians(k * 60.0 + 15.0)
        tilt_box(w, SILV, (-math.cos(a) * 68, -80, 112 - math.sin(a) * 68), (math.cos(a) * 68, -80, 112 + math.sin(a) * 68), 8, 8)
    rod(w, LIME, (0, -72, 112), (0, -90, 112), 14, steps=6)
    g.cyl(SILV, (150, 0, 120), 5, 36, steps=6)
    rod(g, W, (150, -5, 170), (150, 5, 170), 16, steps=8)


def pipe_rings(sc):
    """Pipe sections stacked as rings: three lying across in a pyramid, one beside them, one stood on end; silver,
    lime bands, the dark of the bore."""
    g = sc.still()
    g.box(CONC, (60, 0, -22), (430, 150, 16))
    for (x, z) in ((-52, 50), (52, 50), (0, 138), (190, 50)):
        rod(g, SILV, (x, -65, z), (x, 65, z), 50, steps=10)
        rod(g, LIME, (x, -72, z), (x, -58, z), 54, steps=10)
        rod(g, PANE, (x, -75, z), (x, -70, z), 40, steps=10)
    for x in (-112, 112, 250):
        g.box(CONC, (x, 0, -2), (22, 120, 30))
    g.cyl(SILV, (-215, 10, -14), 50, 120, steps=10)
    g.cyl(LIME, (-215, 10, 94), 54, 14, steps=10)
    g.cyl(PANE, (-215, 10, 106), 40, 4, steps=10)


def bollards(sc):
    """A row of five lime-capped white bollards, two silver rails between them."""
    g = sc.still()
    for k in range(5):
        x = -400 + k * 200
        g.cyl(GRAPH, (x, 0, -30), 24, 50, steps=8)
        g.cyl(W, (x, 0, 20), 17, 150, steps=8)
        g.cyl(LIME, (x, 0, 170), 21, 22, steps=8, top_r=14)
    for z in (80, 140):
        g.box(SILV, (0, 0, z), (800, 6, 6))


def gauge_board(sc):
    """A gauge board: three round dials under a lime hood on two short legs, each lime needle wandering on its own."""
    g = sc.still()
    for x in (-110, 110):
        g.box(GRAPH, (x, 0, 35), (12, 12, 130))
        g.box(CONC, (x, 0, -15), (40, 50, 30))
    g.box(W, (0, 0, 140), (300, 10, 100))
    g.box(LIME, (0, -18, 193), (320, 50, 8))
    g.box(GRAPH, (0, -6, 94), (300, 6, 8))
    g.box(SILV, (0, 0, 60), (232, 6, 6))
    for i, x in enumerate((-95, 0, 95)):
        rod(g, GRAPH, (x, -5, 142), (x, -10, 142), 40, steps=10)
        rod(g, W, (x, -10, 142), (x, -13, 142), 34, steps=10)
        g.box(GRAPH, (x, -16, 142), (9, 5, 9))
        n = sc.part(P.motion(swing=55 - 10 * i, pivot=(x, -15, 142), axis=90.0, rate=0.06 + 0.03 * i, phase=0.3 * i))
        n.box(LIME, (x, -16, 156), (6, 4, 30))


def low_manifold(sc):
    """Eight metres of fat silver pipe on three cradles, both ends down into the ground: lime flanges, a gate valve with a
    flat lime wheel, a gauge on a stem, a thin white line beside it."""
    g = sc.still()
    _pipe(g, [(-430, 0, -30), (-430, 0, 80), (430, 0, 80), (430, 0, -30)], 38)
    for x in (-250, 0, 250):
        g.box(CONC, (x, 20, 10), (50, 150, 70))
    for x in (-340, -110, 110, 340):
        rod(g, LIME, (x - 8, 0, 80), (x + 8, 0, 80), 52, steps=8)
    g.cyl(SILV, (0, 0, 80), 30, 50, steps=8)
    g.cyl(SILV, (0, 0, 130), 7, 18, steps=6)
    g.cyl(LIME, (0, 0, 146), 34, 8, steps=8)
    g.cyl(SILV, (180, 0, 110), 5, 30, steps=6)
    rod(g, W, (180, -5, 154), (180, 5, 154), 16, steps=8)
    _pipe(g, [(-430, 62, -30), (-430, 62, 44), (430, 62, 44), (430, 62, -30)], 16, col=W, steps=6)


# ------------------------------------------------------------------------------------------ props, tall and open
def whistle_mast(sc):
    """A steam whistle mast: a thin silver mast with two lime collars, an up chevron on it, rungs, a lime whistle with
    a white cap, and three soft puffs that hang over it and breathe."""
    g = sc.still()
    g.box(CONC, (0, 0, -30), (70, 70, 160))
    g.cyl(SILV, (0, 0, 50), 10, 640, steps=8, top_r=8)
    for z in (140, 420):
        g.cyl(LIME, (0, 0, z), 14, 26, steps=8)
    _chev(g, LIME, 0, -14, 300, 90, 70, 20, th=8)
    for k in range(5):
        g.box(SILV, (0, 0, 470 + k * 38), (44, 6, 6))
    g.cyl(GRAPH, (0, 0, 690), 12, 20, steps=8)
    g.cyl(LIME, (0, 0, 705), 22, 50, steps=8)
    g.cyl(W, (0, 0, 755), 24, 22, steps=8, top_r=6)
    g.box(SILV, (38, 0, 700), (60, 6, 6))
    for k in range(3):
        p = sc.part(P.motion(phase=0.31 * k, rate=0.16 + 0.03 * k, bob=-(18.0 + 10.0 * k), swing=4.0, pivot=(0, 0, 700), axis=90.0))
        p.sphere(W, (20 + 38 * k, 0, 815 + 52 * k), 26 + 11 * k, scale=(1.2, 0.8, 0.7), steps=(3, 6))


def pipe_loop(sc):
    """A pipe loop arch: an expansion loop of silver pipe, up out of the ground, over a 4 m square window and down
    again, lime flanges, a dial on one leg, an up chevron standing on its crown. The lava shows through it."""
    g = sc.still()
    _pipe(g, [(-450, 0, -110), (-450, 0, 140), (-210, 0, 140), (-210, 0, 540), (210, 0, 540), (210, 0, 140), (450, 0, 140), (450, 0, -110)], 30)
    for sx in (-1, 1):
        rod(g, LIME, (sx * 325, 0, 140), (sx * 345, 0, 140), 42, steps=8)
        rod(g, LIME, (sx * 210, 0, 330), (sx * 210, 0, 350), 42, steps=8)
        g.box(CONC, (sx * 330, 0, 30), (60, 90, 160))
    rod(g, LIME, (-10, 0, 540), (10, 0, 540), 42, steps=8)
    g.cyl(SILV, (0, 0, 565), 6, 100, steps=6)
    _chev(g, LIME, 0, 0, 705, 120, 90, 24, th=8)
    rod(g, GRAPH, (-210, -30, 250), (-210, -36, 250), 26, steps=8)
    rod(g, W, (-210, -36, 250), (-210, -40, 250), 21, steps=8)


def chevron_gantry(sc):
    """A lamp-less lattice gantry: two thin lattice legs, a silver truss, a graphite board with a white edge and three
    lime chevrons pointing UP, 6.6 to 8.2 m off the ground (over the sight line to the lava)."""
    g = sc.still()
    for sx in (-1, 1):
        x = sx * 380
        g.box(CONC, (x, 0, -30), (110, 80, 160))
        for dx in (-22, 22):
            g.box(GRAPH, (x + dx, 0, 360), (9, 9, 620))
        for k in range(6):
            d = 22 if k % 2 else -22
            tilt_box(g, GRAPH, (x - d, 0, 70 + k * 100), (x + d, 0, 170 + k * 100), 6, 6)
        g.sphere(W, (x, 0, 688), 16, steps=(3, 6))
    for z in (600, 660):
        g.box(SILV, (0, 0, z), (804, 8, 8))
    for k in range(8):
        x0 = -380 + k * 95
        tilt_box(g, SILV, (x0, 0, 600 if k % 2 == 0 else 660), (x0 + 95, 0, 660 if k % 2 == 0 else 600), 6, 6)
    g.box(GRAPH, (0, -8, 742), (440, 8, 150))
    for z in (663, 821):
        g.box(W, (0, -8, z), (452, 10, 8))
    for x in (-224, 224):
        g.box(W, (x, -8, 742), (8, 10, 166))
    for k in range(3):
        _chev(g, LIME, -140 + k * 140, -15, 742, 100, 90, 24, th=6)


def pipe_trestle(sc):
    """Fat pipes on trestles: a silver and a white pipe 9 m long carried 5.2 m up on three thin graphite A-trestles,
    both ends dropping straight into the ground, lime flanges, a small lime wheel on top. Open underneath."""
    g = sc.still()
    for x in (-330, 0, 330):
        for sy in (-1, 1):
            tilt_box(g, GRAPH, (x, sy * 105, -100), (x, sy * 22, 480), 14, 14)
        g.box(GRAPH, (x, 0, 486), (20, 150, 14))
        g.box(GRAPH, (x, 0, 250), (12, 120, 10))
    for (y, col, r) in ((-36, SILV, 34), (40, W, 28)):
        zc = 493.0 + r
        _pipe(g, [(-450, y, -110), (-450, y, zc), (450, y, zc), (450, y, -110)], r, col=col)
        for x in (-160, 160):
            rod(g, LIME, (x - 8, y, zc), (x + 8, y, zc), r + 12, steps=8)
    g.cyl(SILV, (0, -36, 561), 8, 30, steps=6)
    g.cyl(LIME, (0, -36, 590), 30, 8, steps=8)


def stilt_drum(sc):
    """A condensate drum on stilts: a small white drum with dished ends and lime bands 4.7 m up on four thin graphite
    stilts, an up chevron on its face, a level glass, a downpipe, a vent with one small puff."""
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, GRAPH, (sx * 150, sy * 62, -100), (sx * 120, sy * 40, 420), 12, 12)
        g.box(GRAPH, (sx * 122, 0, 418), (14, 110, 12))
        tilt_box(g, GRAPH, (sx * 146, -58, 0), (sx * 124, 42, 400), 7, 7)
        g.sphere(W, (sx * 170, 0, 472), 55, scale=(0.5, 1.0, 1.0), steps=(4, 10))
        rod(g, LIME, (sx * 150 - 6, 0, 472), (sx * 150 + 6, 0, 472), 58, steps=10)
    rod(g, W, (-170, 0, 472), (170, 0, 472), 55, steps=10)
    _chev(g, LIME, 0, -57, 472, 70, 60, 18, th=5)
    g.box(SILV, (80, -58, 472), (8, 6, 70))
    g.cyl(SILV, (-60, 0, 520), 10, 46, steps=6)
    g.cyl(SILV, (-60, 0, 566), 20, 10, steps=6)
    rod(g, SILV, (0, 0, 420), (0, 0, -100), 14, steps=6)
    _puff(sc, -60, 0, 610, 22, 0, bob=12.0)


SMALL = dict(grow=(4, 4, 4), grow_down=(4, 4, 0))
JOBS = [
    ('SM_S1D6_ThreeSisters', three_sisters, dict(grow=(400, 120, 300), grow_down=(120, 120, 0))),
    ('SM_S1D6_TurbineHall', turbine_hall, dict(grow=(90, 40, 80), grow_down=(40, 40, 0))),
    ('SM_S1D6_BallTank', ball_tank, dict(grow=(20, 20, 10), grow_down=(20, 20, 0))),
    ('SM_S1D6_ControlStalk', control_stalk, SMALL),
    ('SM_S1D6_SwitchHall', switch_hall, STILL),
    ('SM_S1D6_ValveHouse', valve_house, SMALL),
    ('SM_S1D6_SmallTower', small_tower, dict(grow=(160, 60, 120), grow_down=(60, 60, 0))),
    ('SM_S1D6_TankFarm', tank_farm, dict(grow=(30, 10, 30), grow_down=(10, 10, 0))),
    ('SM_S1D6_Separator', separator, dict(grow=(60, 20, 50), grow_down=(20, 20, 0))),
    ('SM_S1D6_PipeOffice', pipe_office, dict(grow=(20, 20, 10), grow_down=(20, 20, 0))),
    ('SM_S1D6_ValveWheel', valve_wheel, SMALL),
    ('SM_S1D6_PipeRings', pipe_rings, STILL),
    ('SM_S1D6_Bollards', bollards, STILL),
    ('SM_S1D6_GaugeBoard', gauge_board, SMALL),
    ('SM_S1D6_LowManifold', low_manifold, STILL),
    ('SM_S1D6_WhistleMast', whistle_mast, dict(grow=(30, 10, 40), grow_down=(30, 10, 0))),
    ('SM_S1D6_PipeLoop', pipe_loop, STILL),
    ('SM_S1D6_ChevronGantry', chevron_gantry, STILL),
    ('SM_S1D6_PipeTrestle', pipe_trestle, STILL),
    ('SM_S1D6_StiltDrum', stilt_drum, dict(grow=(10, 5, 20), grow_down=(10, 5, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
