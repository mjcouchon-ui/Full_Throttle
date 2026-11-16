"""Bayshore, Stage 1, builder s1d5: district 5 "The Point" (13.2 to 16.8 km). /Game/World/Bayshore/SM_S1D5_*.
Clifftop villas: whitewashed round-cornered cubes with small coral domes and barrel vaults, roof terraces, round
towers, walled gardens; white, deep green, coral, rock grey, navy. No stripes, no light, no words.
Hero: ARCH ROCK, a natural rock arch with a white coral-domed belvedere on top.
Placed by C:/ftwork/s1/L0_s1d5.inl (pasted at the end of Source/FullThrottle/FTPeopleL0.inl by the integrator).

Run in the editor:  import ft_world_bay_s1d5 as K; K.run()
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box            # tilt_box(m, col, (x,y,z) from, (x,y,z) to, width, depth)

DIR = '/Game/World/Bayshore'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))            # no moving part: true bounds
SMALL = dict(grow=(12, 12, 4), grow_down=(12, 12, 0))         # a low prop with one small swinging part

WH, GR, CO, R1, R2, NV = '#fffaf2', '#1f9a62', '#ff7a8a', '#b8b4c8', '#8a86a0', '#2b2350'
LEAF, WOOD, WOOD_D, YEL = '#2fbf71', '#c9a06a', '#9a744a', '#ffe14d'       # the kit's own leaf, wood and yellow


# ------------------------------------------------------------------------------------------ helpers
def _ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def _rr(hx, hy, r, n=2, cx=0.0, cy=0.0):
    """A rectangle with round corners, counter-clockwise."""
    r = max(1.0, min(r, hx - 1.0, hy - 1.0))
    pts = []
    for sx, sy, a0 in ((1, -1, -90.0), (1, 1, 0.0), (-1, 1, 90.0), (-1, -1, 180.0)):
        for k in range(n + 1):
            a = math.radians(a0 + 90.0 * k / n)
            pts.append((cx + sx * (hx - r) + math.cos(a) * r, cy + sy * (hy - r) + math.sin(a) * r))
    return pts


def rbox(g, col, x, y, z0, z1, w, d, r=45.0, n=2):
    """A round-cornered block: w along X, d along Y, from z0 to z1."""
    g.plan(col, _rr(w / 2.0, d / 2.0, r, n, x, y), z0, z1)


def dome(g, col, x, y, z, r, h=None, steps=10, tiers=3, knob=WH):
    h = h or r * 0.8
    for k in range(tiers):
        a0, a1 = math.radians(90.0 * k / tiers), math.radians(90.0 * (k + 1) / tiers)
        g.cyl(col, (x, y, z + h * math.sin(a0)), r * math.cos(a0), h * (math.sin(a1) - math.sin(a0)), steps=steps,
              top_r=max(r * math.cos(a1), r * 0.07))
    if knob:
        g.sphere(knob, (x, y, z + h + r * 0.07), max(r * 0.12, 8.0), steps=(3, 6))


def _arc(cx, cz, rx, rz, n, a0=0.0, a1=180.0):
    return [(cx + rx * math.cos(math.radians(a0 + (a1 - a0) * k / n)), cz + rz * math.sin(math.radians(a0 + (a1 - a0) * k / n))) for k in range(n + 1)]


def vault_y(g, col, x, y, z, w, h, length, n=6):
    """A barrel vault whose axis runs along Y: its round end faces the road."""
    g.side(col, _ccw(_arc(x, z, w / 2.0, h, n)), y - length / 2.0, y + length / 2.0)


def vault_x(g, col, x, y, z, w, h, length, n=6):
    """A barrel vault whose axis runs along X (the profile turned a quarter turn: (x, y) -> (-y, x))."""
    g.side(col, _ccw(_arc(y, z, w / 2.0, h, n)), -x - length / 2.0, -x + length / 2.0, rot=(0, 90, 0))


def archp(g, col, x, y, z0, w, h, t=8.0, n=4):
    """An arched panel on a wall that faces the road: a door, a window, a niche."""
    pts = [(x - w / 2.0, z0)] + _arc(x, z0 + h - w / 2.0, w / 2.0, w / 2.0, n)
    g.side(col, _ccw(pts), y - t / 2.0, y + t / 2.0)


def arch_ring(g, col, x, y, z, r_in, r_out, depth, n=6):
    """A real arch (light shows through): a half ring of blocks facing the road, sprung from height z."""
    for k in range(n):
        a, b = math.radians(180.0 * k / n), math.radians(180.0 * (k + 1) / n)
        pts = [(x + r_in * math.cos(a), z + r_in * math.sin(a)), (x + r_out * math.cos(a), z + r_out * math.sin(a)),
               (x + r_out * math.cos(b), z + r_out * math.sin(b)), (x + r_in * math.cos(b), z + r_in * math.sin(b))]
        g.side(col, _ccw(pts), y - depth / 2.0, y + depth / 2.0)


def shutters(g, x, y, z, w, h, col=GR):
    for s in (-1, 1):
        g.box(col, (x + s * (w * 0.5 + w * 0.27), y, z), (w * 0.46, 6, h))


def window(g, x, y, z, w=90.0, h=130.0):
    """A navy window with green shutters on a wall that faces the road (y = the wall's face)."""
    g.box(NV, (x, y - 3, z), (w, 8, h))
    g.box(WH, (x, y - 6, z - h / 2.0 - 5), (w + 30, 14, 10))
    shutters(g, x, y - 4, z, w, h)


def stair(g, col, a, b, n, w):
    """Solid steps from a = (x, y, z foot) up to b = (x, y, z top); they run along X or along Y."""
    dx, dy = b[0] - a[0], b[1] - a[1]
    for k in range(n):
        f = (k + 0.5) / n
        top = a[2] + (k + 1) * (b[2] - a[2]) / n
        z0 = a[2] - 40.0
        size = (abs(dx) / n + 2, w, top - z0) if abs(dx) >= abs(dy) else (w, abs(dy) / n + 2, top - z0)
        g.box(col, (a[0] + dx * f, a[1] + dy * f, (top + z0) / 2.0), size)


def pot(g, x, y, z, s=1.0, col=CO, leaf=GR, flower=None):
    g.cyl(col, (x, y, z), 20 * s, 34 * s, steps=6, top_r=27 * s)
    g.sphere(leaf, (x, y, z + 52 * s), 30 * s, steps=(3, 6))
    if flower:
        g.sphere(flower, (x + 9 * s, y - 12 * s, z + 72 * s), 11 * s, steps=(3, 5))


def jar(g, x, y, z, h, col=WH, band=None, steps=8):
    r = h * 0.3
    g.cyl(col, (x, y, z), r * 0.55, h * 0.45, steps=steps, top_r=r)
    g.cyl(col, (x, y, z + h * 0.45), r, h * 0.35, steps=steps, top_r=r * 0.5)
    g.cyl(col, (x, y, z + h * 0.8), r * 0.5, h * 0.2, steps=steps, top_r=r * 0.64)
    if band:
        g.cyl(band, (x, y, z + h * 0.4), r * 1.04, h * 0.09, steps=steps)


def blossom(g, x, y, z, r, seed=1, col=CO, leaf=GR, n=3):
    """A mass of climbing flowers over a wall: leaf under, blossom over."""
    q = random.Random(seed)
    g.sphere(leaf, (x, y, z - r * 0.25), r * 1.05, scale=(1.2, 0.7, 0.75), steps=(4, 7))
    for k in range(n):
        g.sphere(col, (x + q.uniform(-r, r) * 0.8, y - r * 0.25 + q.uniform(-10, 10), z + q.uniform(-0.1, 0.45) * r), r * q.uniform(0.42, 0.6),
                 scale=(1.1, 0.8, 0.8), steps=(3, 6))


def pergola(g, x, y, z, w, d, h=220.0, seed=2):
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(WH, (x + sx * w / 2.0, y + sy * d / 2.0, z + h / 2.0), (14, 14, h))
    for sy in (-1, 1):
        g.box(WOOD, (x, y + sy * d / 2.0, z + h + 5), (w + 60, 10, 10))
    n = max(3, int(w / 70))
    for k in range(n):
        g.box(WOOD, (x - w / 2.0 + (k + 0.5) * w / n, y, z + h + 14), (8, d + 50, 8))
    q = random.Random(seed)
    for k in range(3):
        g.sphere(GR, (x + q.uniform(-0.35, 0.35) * w, y + q.uniform(-0.3, 0.3) * d, z + h + 22), q.uniform(40, 62), scale=(1.5, 1.1, 0.4), steps=(3, 6))
    g.sphere(CO, (x + q.uniform(-0.3, 0.3) * w, y - d * 0.4, z + h + 10), 26, scale=(1.3, 0.9, 0.7), steps=(3, 5))


def rocks(g, pts, seed=3):
    """Low boulders: (x, y, radius)."""
    q = random.Random(seed)
    for k, (x, y, r) in enumerate(pts):
        g.sphere(R2 if k % 3 == 0 else R1, (x, y, r * 0.18), r, scale=(q.uniform(1.0, 1.3), q.uniform(0.8, 1.0), q.uniform(0.5, 0.75)), steps=(4, 6),
                 rot=(0, q.uniform(0, 90), 0))


def vane(sc, x, y, z, phase=0.2):
    """A weather vane: a bar with a coral disc and a white tail, turning a little about its post."""
    v = sc.part(P.motion(swing=24, pivot=(x, y, z), axis=P.UP, rate=0.12, phase=phase))
    v.box(WOOD_D, (x, y, z), (150, 6, 8))
    v.cyl(CO, (x + 70, y - 3, z), 20, 6, rot=(0, 0, 90), steps=8)
    v.box(WH, (x - 62, y, z + 10), (40, 5, 40))


# ------------------------------------------------------------------------------------------ BUILDINGS
def dome_villa(sc):
    """A domed villa with a roof terrace: 11.5 x 10 m, 8.3 m to the dome's knob."""
    g = sc.still()
    rbox(g, R1, 0, 0, -300, 30, 1150, 960, 70)
    rbox(g, WH, -150, 80, 30, 430, 800, 760, 75)                       # the main cube
    rbox(g, CO, -150, 80, 430, 446, 824, 784, 80)                      # a line of coral tile round the roof
    g.cyl(WH, (-150, 80, 446), 250, 110, steps=10)                     # the drum
    for a in range(5):                                                 # the drum's little windows
        t = math.radians(200 + a * 35)
        g.box(NV, (-150 + math.cos(t) * 246, 80 + math.sin(t) * 246, 505), (34, 14, 50), rot=(0, 200 + a * 35 + 90, 0))
    dome(g, CO, -150, 80, 556, 264, 240, steps=10)
    rbox(g, WH, 400, -120, 30, 300, 420, 560, 60)                      # the low wing; its roof is the terrace
    rbox(g, R1, 400, -120, 300, 316, 440, 580, 64)
    for x in (215, 585):                                               # the terrace's wall
        g.box(WH, (x, -120, 345), (16, 540, 60))
    g.box(WH, (400, -392, 345), (386, 16, 60))
    pergola(g, 400, -100, 316, 280, 360, 215, seed=4)
    stair(g, WH, (660, -390, 30), (660, 60, 300), 6, 110)             # outside, up to the terrace
    g.box(WH, (722, -165, 190), (14, 470, 330))
    archp(g, GR, -400, -302, 30, 120, 250)                             # the door
    g.box(R1, (-400, -340, 20), (190, 90, 30))
    for x in (-170, 60):
        archp(g, NV, x, -302, 170, 92, 180)
        shutters(g, x, -304, 240, 92, 150)
    window(g, 400, -400, 180, 110, 120)
    g.box(CO, (400, -445, 268), (190, 100, 8), rot=(0, 0, -24))        # a coral canvas over the wing's window
    g.box(WH, (-460, 360, 520), (70, 70, 150))                         # chimney
    g.cyl(CO, (-460, 360, 595), 26, 40, steps=6, top_r=18)
    g.cyl(R2, (60, 380, 446), 12, 70, steps=6)                         # a vent
    rbox(g, WH, -200, -480, -60, 95, 740, 44, 20)                      # the garden wall and its hedge
    rbox(g, GR, -200, -430, 0, 140, 700, 60, 28)
    pot(g, -540, -480, 95, 1.1, flower=YEL)
    pot(g, 140, -480, 95, 1.1)
    blossom(g, -545, -250, 330, 110, seed=6)
    pot(g, 250, -360, 316, 0.9, col=WH)
    pot(g, 560, 100, 316, 0.9, flower=WH)


def tower_house(sc):
    """A round watchtower house: a stout white tower (6 m across, 9 m) with a vaulted house against it."""
    g = sc.still()
    tx, ty = -200.0, 0.0
    g.cyl(R1, (tx, ty, -300), 370, 420, steps=12, top_r=325)           # the battered stone foot
    g.cyl(WH, (tx, ty, 120), 310, 700, steps=12, top_r=292)
    g.cyl(WH, (tx, ty, 820), 322, 70, steps=12)                        # the parapet
    g.cyl(CO, (tx, ty, 890), 328, 14, steps=12)
    g.cyl(WH, (tx - 70, ty + 60, 904), 105, 140, steps=8)              # the stair head under its own little dome
    dome(g, CO, tx - 70, ty + 60, 1044, 116, 100, steps=8)
    g.box(NV, (tx - 70, ty - 46, 964), (50, 8, 90))
    g.box(R2, (tx + 150, ty - 40, 1010), (6, 6, 230))                  # the vane's post
    vane(sc, tx + 150, ty - 40, 1130)
    for a, z, w, h in ((270, 640, 64, 130), (305, 430, 50, 100), (235, 700, 50, 90)):       # deep little windows
        t = math.radians(a)
        g.box(NV, (tx + math.cos(t) * 288, ty + math.sin(t) * 288, z), (w, 30, h), rot=(0, a + 90, 0))
        g.box(GR, (tx + math.cos(t) * 300, ty + math.sin(t) * 300, z - h / 2.0 - 8), (w + 26, 22, 10), rot=(0, a + 90, 0))
    for k in range(7):                                                 # steps curling round the foot, up to a door
        a = 178 + k * 11.5
        t = math.radians(a)
        top = 70 + k * 45
        g.box(WH, (tx + math.cos(t) * 395, ty + math.sin(t) * 395, (top - 40) / 2.0), (150, 96, top + 40), rot=(0, a, 0))
    t = math.radians(258)
    g.box(GR, (tx + math.cos(t) * 296, ty + math.sin(t) * 296, 435), (88, 24, 190), rot=(0, 258 + 90, 0))
    rbox(g, R1, 270, 30, -300, 30, 590, 560, 50)                       # the house
    rbox(g, WH, 270, 30, 30, 320, 540, 510, 60)
    vault_x(g, CO, 270, 30, 320, 470, 150, 500, n=6)
    archp(g, GR, 150, -228, 30, 110, 230)
    window(g, 380, -225, 190, 90, 110)
    g.box(WH, (470, 180, 430), (60, 60, 200))
    g.cyl(CO, (470, 180, 530), 24, 36, steps=6, top_r=16)
    rbox(g, GR, 300, -300, 0, 105, 400, 56, 26)
    pot(g, 60, -290, 30, 1.2, flower=YEL)
    rocks(g, [(-560, -240, 130), (-640, 60, 100), (-420, 330, 150), (40, 340, 90)], seed=7)
    blossom(g, 520, -120, 250, 95, seed=8)


def windmill(sc):
    """A white windmill on its rock knoll: four canvas sails that go round very slowly (40 m and more from the road)."""
    g = sc.still()
    g.sphere(R1, (0, 80, -60), 520, scale=(1.25, 1.0, 0.5), steps=(5, 10))          # the knoll
    rocks(g, [(-520, -230, 170), (540, -150, 150), (-380, 430, 190), (430, 420, 160), (620, 140, 110)], seed=9)
    for x, y, r in ((-300, -300, 60), (330, -330, 50), (-560, 60, 55)):
        g.sphere(GR, (x, y, 60), r, scale=(1.3, 1.0, 0.7), steps=(3, 6))
    g.cyl(WH, (0, 80, 120), 262, 650, steps=12, top_r=214)                           # the body
    g.cyl(R1, (0, 80, 150), 270, 40, steps=12, top_r=264)
    g.cyl(CO, (0, 80, 770), 240, 230, steps=12, top_r=26)                            # the cap
    g.sphere(WH, (0, 80, 1012), 22, steps=(3, 6))
    archp(g, GR, 0, -172, 190, 112, 220, t=26)
    for z, w in ((560, 58), (400, 46)):
        g.box(NV, (0, -150 + (z - 400) * 0.06, z), (w, 30, 84))
        g.box(GR, (0, -158 + (z - 400) * 0.06, z - 50), (w + 24, 22, 10))
    for a in (210, 330):
        t = math.radians(a)
        g.box(NV, (math.cos(t) * 228, 80 + math.sin(t) * 228, 480), (46, 30, 76), rot=(0, a + 90, 0))
    stair(g, WH, (0, -480, 20), (0, -200, 190), 5, 130)                              # up the knoll to the door
    g.box(WOOD_D, (0, -215, 700), (16, 190, 16))                                      # the axle
    s = sc.part(P.motion(swing=170, pivot=(0, -300, 700), axis=90, rate=0.03, phase=0.1))
    s.cyl(WH, (0, -322, 700), 40, 36, rot=(0, 0, 90), steps=8)
    for k in range(4):
        a = 45 + 90 * k
        t = math.radians(a)
        ux, uz = math.cos(t), math.sin(t)
        vx, vz = -uz, ux
        s.box(WOOD_D, (ux * 265, -300, 700 + uz * 265), (520, 10, 12), rot=(a, 0, 0))
        s.box(WH, (ux * 320 + vx * 62, -304, 700 + uz * 320 + vz * 62), (340, 6, 112), rot=(a, 0, 0))
        s.box(WOOD_D, (ux * 320 + vx * 122, -300, 700 + uz * 320 + vz * 122), (340, 8, 8), rot=(a, 0, 0))


def pavilion(sc):
    """A garden pavilion under a coral barrel vault, a bell arch over its door (the bell swings). 6.4 x 8 m."""
    g = sc.still()
    rbox(g, R1, 0, 0, -300, 40, 720, 880, 50)
    rbox(g, WH, 0, 0, 40, 380, 640, 800, 60)
    vault_y(g, CO, 0, 30, 380, 590, 220, 730, n=6)
    for x in (-170, 170):                                              # the bell arch
        g.box(WH, (x, -370, 570), (120, 60, 380))
    arch_ring(g, WH, 0, -370, 760, 110, 230, 60, n=6)
    g.sphere(WH, (0, -370, 1004), 26, steps=(3, 6))
    g.box(WOOD_D, (0, -370, 868), (230, 12, 12))
    b = sc.part(P.motion(swing=14, pivot=(0, -370, 868), axis=90, rate=0.45, phase=0.3))
    b.box(WOOD_D, (0, -370, 842), (8, 8, 50))
    b.cyl(YEL, (0, -370, 740), 56, 84, steps=8, top_r=24)
    b.sphere(WOOD_D, (0, -370, 732), 13, steps=(3, 5))
    archp(g, GR, 0, -402, 40, 130, 260, t=10)
    g.box(R1, (0, -450, 22), (220, 110, 36))
    g.box(R1, (0, -520, 6), (280, 60, 30))
    for x in (-215, 215):
        g.cyl(NV, (x, -404, 255), 38, 8, rot=(0, 0, 90), steps=8)
        g.box(NV, (x * 1.49, 60, 215), (8, 90, 150))
        g.box(GR, (x * 1.5, 60, 132), (14, 120, 10))
    pot(g, -130, -450, 40, 1.3, flower=WH)
    pot(g, 130, -450, 40, 1.3, flower=YEL)
    rbox(g, GR, -380, -100, 0, 120, 70, 520, 30)
    blossom(g, 325, -330, 300, 80, seed=11)


def step_row(sc):
    """Three cube houses in a row, each a step higher than the last, a washing line between two of them. 16 x 5.6 m."""
    g = sc.still()
    rbox(g, R1, 0, 0, -300, 30, 1640, 600, 60)
    cubes = ((-530, 500, 500, 300), (0, 500, 520, 440), (530, 500, 540, 580))
    for x, w, d, h in cubes:
        rbox(g, WH, x, 0, 30, h, w, d, 60)
        rbox(g, R1, x, 0, h, h + 14, w + 16, d + 16, 64)
    stair(g, WH, (-775, -315, 30), (-470, -315, 300), 5, 110)         # outside, up to the first roof
    g.box(WH, (-620, -376, 190), (320, 14, 330))
    archp(g, GR, -620, -252, 30, 100, 215)
    archp(g, GR, -90, -262, 30, 104, 225)
    window(g, 110, -260, 300, 84, 110)
    archp(g, GR, 440, -272, 30, 104, 225)
    window(g, 620, -270, 200, 80, 100)
    for x in (430, 630):
        archp(g, NV, x, -272, 380, 76, 140)
        shutters(g, x, -274, 432, 76, 112)
    g.cyl(WH, (600, 30, 594), 150, 60, steps=8)                        # the top house's small dome
    dome(g, CO, 600, 30, 654, 158, 140, steps=8)
    g.box(WH, (-120, 180, 520), (64, 64, 150))                         # chimney, vents
    g.cyl(CO, (-120, 180, 595), 24, 36, steps=6, top_r=16)
    g.cyl(R2, (380, 180, 594), 12, 70, steps=6)
    g.box(CO, (60, -40, 640), (300, 260, 8), rot=(0, 0, -8))           # a coral canvas over the middle roof
    for x, y in ((-80, -160), (200, -160), (-80, 90), (200, 90)):
        g.box(WOOD, (x, y, 547 if y < 0 else 565), (8, 8, 186 if y < 0 else 222))
    g.box(WOOD, (-640, 120, 395), (8, 8, 162))                         # the washing line: a post on the low roof
    tilt_box(g, WOOD_D, (-640, 120, 470), (-250, 120, 420), 4, 4)
    for k, (col, w, h) in enumerate(((WH, 76, 100), (CO, 60, 70), (WH, 84, 110), (GR, 54, 64))):
        x = -590 + k * 86
        z = 470 - (x + 640) * 50.0 / 390.0
        c = sc.part(P.motion(swing=9 + k, pivot=(x, 120, z), axis=0, rate=0.5, phase=0.17 * k + 0.05))
        c.box(col, (x, 120, z - h / 2.0 - 2), (w, 5, h))
    rbox(g, GR, 320, -330, 0, 100, 400, 50, 24)
    pot(g, -800, -250, 30, 1.2, flower=YEL)
    pot(g, -700, -330, 144, 0.9, col=WH)
    pot(g, 250, -290, 30, 1.2)
    jar(g, -280, -290, 30, 100, WH, CO)
    blossom(g, 790, -150, 330, 100, seed=13)


def gate_house(sc):
    """A long white garden wall (20 m) with a real arched gate under a small coral dome; green doors half open."""
    g = sc.still()
    for s in (-1, 1):
        rbox(g, WH, s * 615, 0, -250, 240, 790, 60, 24)
        rbox(g, R1, s * 615, 0, 240, 256, 806, 78, 30)
        g.cyl(WH, (s * 1010, 0, -250), 56, 550, steps=8)               # the wall's round end
        g.cyl(R1, (s * 1010, 0, 300), 64, 14, steps=8)
        pot(g, s * 1010, 0, 314, 1.1, flower=(YEL if s > 0 else WH))
        g.box(WH, (s * 175, 0, 40), (130, 320, 580))                   # the gate's piers
        g.box(WH, (s * 180, 0, 445), (120, 320, 230))
    arch_ring(g, WH, 0, 0, 330, 110, 230, 320, n=6)
    rbox(g, WH, 0, 0, 556, 590, 500, 350, 40)
    rbox(g, CO, 0, 0, 590, 610, 530, 380, 44)
    dome(g, CO, 0, 0, 610, 135, 115, steps=8)
    g.box(GR, (-64, -205, 150), (104, 8, 300), rot=(0, -42, 0))        # the door leaves, half open
    g.box(GR, (64, -205, 150), (104, 8, 300), rot=(0, 42, 0))
    rbox(g, R1, 0, -230, -30, 8, 260, 300, 40)                         # the threshold, a path
    rbox(g, R1, 0, 230, -30, 6, 180, 200, 40)
    g.box(CO, (270, -34, 185), (36, 8, 36))                            # a blank tile by the gate
    g.box(WOOD_D, (-262, -40, 200), (5, 5, 110))                       # a bell pull
    g.sphere(YEL, (-262, -40, 140), 9, steps=(3, 5))
    rbox(g, GR, -620, 90, 0, 300, 660, 90, 40)                         # the garden shows over the wall
    rbox(g, GR, 560, 95, 0, 285, 560, 90, 40)
    blossom(g, -700, -20, 250, 120, seed=15, n=4)
    blossom(g, 480, -10, 262, 90, seed=16)
    g.sphere(LEAF, (860, 150, 330), 110, scale=(1.0, 0.9, 0.9), steps=(4, 7))
    jar(g, -330, -120, 0, 120, WH, CO)
    jar(g, 340, -110, 0, 95, CO)
    for s in (-1, 1):                                                  # buttresses: thick walls
        g.box(WH, (s * 640, -44, 60), (90, 40, 300), rot=(0, 0, 0))
        g.box(R1, (s * 640, -52, -20), (110, 60, 60))


def cube_house(sc):
    """A small house under two barrel vaults, a bread oven beside it, a vine over the door. 8 x 6.4 m."""
    g = sc.still()
    rbox(g, R1, 40, 0, -300, 30, 860, 640, 50)
    rbox(g, WH, -30, 30, 30, 310, 640, 540, 60)
    for x in (-190, 130):
        vault_y(g, CO, x, 30, 310, 300, 130, 520, n=5)
    g.box(WH, (-30, 200, 400), (56, 56, 200))
    g.cyl(CO, (-30, 200, 500), 22, 34, steps=6, top_r=15)
    archp(g, GR, -190, -244, 30, 110, 232)
    window(g, 130, -240, 195, 96, 112)
    g.box(WH, (130, -320, 40), (210, 60, 46))                          # a bench under the window, two cushions
    g.box(CO, (85, -320, 68), (80, 50, 12))
    g.box(GR, (178, -320, 68), (80, 50, 12))
    for x in (-290, -90):                                              # the vine's frame over the door
        g.box(WH, (x, -430, 130), (14, 14, 260))
    g.box(WOOD, (-190, -430, 264), (260, 10, 10))
    for x in (-270, -190, -110):
        g.box(WOOD, (x, -340, 272), (8, 200, 8))
    g.sphere(GR, (-190, -360, 286), 70, scale=(1.7, 1.2, 0.4), steps=(3, 6))
    g.sphere(CO, (-240, -420, 276), 24, scale=(1.3, 0.9, 0.7), steps=(3, 5))
    g.cyl(WH, (400, -120, -40), 112, 160, steps=8)                     # the bread oven
    dome(g, WH, 400, -120, 120, 116, 92, steps=8, knob=None)
    g.box(NV, (400, -226, 92), (60, 20, 50))
    g.box(R1, (400, -250, 58), (110, 50, 10))
    g.box(WH, (440, -40, 260), (40, 40, 150))
    g.cyl(CO, (440, -40, 335), 18, 28, steps=6, top_r=12)
    g.cyl(R2, (-300, 290, 150), 9, 190, steps=6)                       # a drain pipe
    jar(g, -330, -290, 30, 110, WH, CO)
    pot(g, 20, -300, 30, 1.1, flower=YEL)
    for k in range(3):                                                 # firewood by the oven
        g.box(WOOD_D, (300 + k * 14, -250, 14 + k * 12), (20, 80, 12))


def terrace_villa(sc):
    """A two-storey villa: a loggia of three real arches below, a terrace and a pergola above, a coral dome. 13 x 9 m."""
    g = sc.still()
    rbox(g, R1, 0, 0, -300, 30, 1320, 900, 70)
    rbox(g, WH, 0, 110, 30, 330, 1200, 600, 70)                        # ground floor (front wall y = -190)
    rbox(g, WH, -250, 160, 330, 620, 660, 480, 60)                     # upper floor
    rbox(g, CO, -250, 160, 620, 634, 680, 500, 64)
    g.cyl(WH, (-250, 160, 634), 172, 54, steps=10)
    dome(g, CO, -250, 160, 688, 182, 160, steps=10)
    for x in (-540, -180, 180, 540):                                   # the loggia
        g.box(WH, (x, -330, 125), (64, 64, 190))
        g.box(R1, (x, -330, 226), (76, 76, 12))
    for x in (-360, 0, 360):
        arch_ring(g, WH, x, -330, 232, 148, 182, 60, n=5)
    g.box(WH, (0, -265, 400), (1200, 196, 30))                         # its roof: the balcony
    g.box(WH, (0, -355, 440), (1200, 16, 54))
    g.box(GR, (0, -194, 150), (120, 8, 240))
    for x in (-360, 360):
        g.box(NV, (x, -193, 170), (100, 8, 130))
        shutters(g, x, -194, 170, 100, 130)
    for x in (-440, -110):
        archp(g, NV, x, -82, 400, 92, 170)
        shutters(g, x, -84, 468, 92, 140)
    g.box(GR, (-275, -82, 425), (90, 8, 190))                          # the balcony door
    rbox(g, R1, 340, 160, 330, 344, 520, 480, 50)                      # the terrace beside the upper floor
    for x, y, sx, sy in ((340, 395, 500, 16), (592, 160, 16, 470)):
        g.box(WH, (x, y, 372), (sx, sy, 56))
    pergola(g, 340, 170, 344, 360, 320, 220, seed=17)
    stair(g, WH, (660, -180, 30), (660, 300, 330), 7, 110)
    g.box(WH, (722, 60, 205), (14, 500, 360))
    g.box(WH, (-500, 330, 700), (64, 64, 150))
    g.cyl(CO, (-500, 330, 775), 24, 36, steps=6, top_r=16)
    g.cyl(R2, (-60, 320, 634), 12, 60, steps=6)
    pot(g, -540, -420, 30, 1.3, flower=YEL)
    pot(g, 540, -420, 30, 1.3)
    pot(g, 180, -355, 467, 0.8, col=WH, flower=CO)
    pot(g, -180, -355, 467, 0.8, col=WH)
    rbox(g, GR, 380, -420, 0, 110, 240, 60, 26)
    blossom(g, -610, -160, 300, 110, seed=18, n=4)
    c = sc.part(P.motion(swing=5, pivot=(340, 330, 580), axis=0, rate=0.3, phase=0.4))      # a canvas blind on the pergola
    c.box(CO, (340, 330, 470), (300, 6, 200))


# ------------------------------------------------------------------------------------------ PROPS
def well(sc):
    """A stone well with a winch under a little tiled ridge; the bucket swings. LOW: 1.95 m."""
    g = sc.still()
    g.cyl(R1, (0, 0, -40), 125, 55, steps=10)
    g.cyl(WH, (0, 0, 15), 80, 85, steps=10)
    g.cyl(R1, (0, 0, 100), 88, 12, steps=10)
    g.cyl(NV, (0, 0, 104), 62, 6, steps=8)
    for s in (-1, 1):
        g.box(WOOD, (s * 96, 0, 95), (11, 11, 170))
        g.box(CO, (0, s * 24, 176), (250, 56, 7), rot=(0, 0, s * 24))  # the ridge: two leaves of coral tile
    rod(g, WOOD_D, (-110, 0, 160), (128, 0, 160), 8, steps=6)
    g.box(WOOD_D, (134, 0, 146), (6, 6, 36))
    g.box(WOOD_D, (146, 0, 130), (28, 6, 6))
    b = sc.part(P.motion(swing=7, pivot=(0, 0, 160), axis=0, rate=0.4, phase=0.2))
    b.box(WOOD_D, (0, 0, 142), (4, 4, 36))
    b.cyl(WH, (0, 0, 104), 15, 24, steps=6, top_r=19)
    b.cyl(CO, (0, 0, 112), 20, 5, steps=6)
    pot(g, -105, -70, 15, 0.9, flower=YEL)
    jar(g, 100, -66, 15, 62, CO)
    g.cyl(R1, (30, -118, -30), 34, 40, steps=6)


def sundial(sc):
    """A sundial on a white pedestal, a ring of white pebbles, two clipped balls in coral pots. LOW: 1.7 m."""
    g = sc.still()
    g.cyl(R1, (0, 0, -40), 112, 50, steps=10)
    g.cyl(WH, (0, 0, 10), 78, 18, steps=10)
    g.cyl(WH, (0, 0, 28), 27, 86, steps=8, top_r=20)
    rbox(g, R1, 0, 0, 114, 124, 64, 64, 12, n=1)
    g.cyl(WH, (0, 0, 124), 46, 8, steps=10)
    g.side(CO, _ccw([(-30, 132), (30, 132), (-30, 172)]), -3, 3)       # the gnomon
    for k in range(5):
        t = math.radians(-20 + k * 55)
        g.box(NV, (math.cos(t) * 34, math.sin(t) * 34, 133), (14, 6, 5), rot=(0, -20 + k * 55, 0))
    for k in range(8):
        t = math.radians(k * 45 + 10)
        g.sphere(WH, (math.cos(t) * 100, math.sin(t) * 100, 16), 12, steps=(3, 5))
    for s in (-1, 1):
        g.cyl(CO, (s * 96, 40, 10), 22, 40, steps=6, top_r=30)
        g.sphere(GR, (s * 96, 40, 82), 36, steps=(4, 6))


def jar_row(sc):
    """Five big clay jars, painted white and coral, on a stone bench. LOW: 1.7 m."""
    g = sc.still()
    g.box(R1, (0, 0, 38), (540, 84, 14))
    for x in (-220, 0, 220):
        g.box(WH, (x, 0, -5), (60, 70, 72))
    for x, h, col, band in ((-215, 100, WH, CO), (-105, 124, CO, None), (10, 88, WH, GR), (112, 116, WH, CO), (220, 96, CO, WH)):
        jar(g, x, 0, 45, h, col, band)
    g.sphere(GR, (10, -6, 140), 26, scale=(1.2, 1.0, 0.7), steps=(3, 6))    # a plant in the middle jar
    g.sphere(YEL, (18, -22, 152), 9, steps=(3, 5))
    g.box(WOOD_D, (112, 4, 172), (5, 5, 30), rot=(0, 0, 18))           # a ladle in one
    jar(g, -300, -20, -6, 60, WH)
    pot(g, 300, -14, -4, 0.8)


def cairn(sc):
    """A rock cairn with a plain coral pennant on a white pole (the pennant swings). TALL: 4.4 m."""
    g = sc.still()
    q = random.Random(21)
    for k, (z, r) in enumerate(((10, 105), (70, 86), (128, 66), (176, 48), (212, 32))):
        g.sphere(R1 if k % 2 else R2, (q.uniform(-14, 14), q.uniform(-10, 10), z), r, scale=(q.uniform(1.0, 1.25), q.uniform(0.85, 1.0), 0.62), steps=(4, 6),
                 rot=(0, q.uniform(0, 90), 0))
    rocks(g, [(-150, -40, 70), (140, 30, 84), (20, 130, 64), (-60, -150, 44)], seed=22)
    g.cyl(R2, (0, 0, -150), 60, 160, steps=6)
    g.cyl(WH, (0, 0, 200), 6, 240, steps=6)
    g.sphere(WH, (0, 0, 444), 10, steps=(3, 5))
    g.sphere(WH, (-52, -84, 96), 13, steps=(3, 5))                     # a white way-mark dabbed on a rock
    f = sc.part(P.motion(swing=24, pivot=(0, 0, 400), axis=P.UP, rate=0.5, phase=0.15))
    f.side(CO, _ccw([(6, 432), (132, 405), (6, 378)]), -2.5, 2.5)
    g.sphere(GR, (128, -52, 40), 32, scale=(1.2, 1.0, 0.7), steps=(3, 6))


def scope(sc):
    """A look-out: a round stone platform, a low white wall behind, a stubby white and coral telescope that looks
    out to sea (away from the road) and pans slowly. LOW: 1.9 m."""
    g = sc.still()
    g.cyl(R1, (0, 0, -40), 90, 70, steps=10)
    g.box(R1, (0, -98, -4), (110, 40, 40))
    g.box(R1, (0, -128, -16), (140, 36, 30))
    for a in (40, 90, 140):
        t = math.radians(a)
        g.box(WH, (math.cos(t) * 78, math.sin(t) * 78, 62), (62, 16, 66), rot=(0, a + 90, 0))
    g.cyl(WH, (0, 0, 30), 9, 104, steps=6)
    g.cyl(CO, (0, 0, 126), 14, 10, steps=6)
    s = sc.part(P.motion(swing=24, pivot=(0, 0, 150), axis=P.UP, rate=0.08, phase=0.6))
    rod(s, WH, (0, -30, 146), (0, 52, 170), 13, steps=8)
    rod(s, CO, (0, 30, 163.6), (0, 54, 170.6), 16, steps=8)
    rod(s, CO, (0, -36, 144.2), (0, -26, 147.2), 10, steps=6)
    pot(g, -62, -50, 30, 0.7, flower=YEL)


def dry_wall(sc):
    """9 m of dry-stone wall with a white arched niche in the middle (a coral jar in it), plants along its top. LOW: 1.6 m."""
    g = sc.still()
    q = random.Random(31)
    for row, zb in enumerate((-40.0, 38.0, 76.0)):
        x = -450.0
        while x < 450.0:
            w = q.uniform(70, 120)
            if abs(x + w / 2.0) > 90 + w / 2.0 and x + w <= 456:
                h = 80.0 if row == 0 else q.uniform(34, 42)
                g.box(R2 if q.random() < 0.35 else R1, (x + w / 2.0, q.uniform(-5, 5), zb + h / 2.0), (w - 4, q.uniform(56, 70), h))
            x += w
    g.box(WH, (0, 0, 50), (170, 84, 180))                              # the niche
    g.side(WH, _ccw(_arc(0, 140, 85, 22, 4)), -42, 42)
    archp(g, NV, 0, -40, 30, 84, 104, t=10)
    g.cyl(CO, (0, -52, 30), 15, 28, steps=6, top_r=22)
    g.cyl(CO, (0, -52, 58), 22, 22, steps=6, top_r=10)
    for x, r in ((-330, 30), (-190, 24), (250, 32), (380, 22)):
        g.sphere(GR, (x, 0, 122), r, scale=(1.4, 1.0, 0.6), steps=(3, 6))
    for x in (-316, 262):
        g.sphere(CO, (x, -16, 134), 10, steps=(3, 5))
    g.sphere(YEL, (-180, -14, 130), 8, steps=(3, 5))


def exedra(sc):
    """A whitewashed curved bench on a half-round of paving: coral cushions, a clipped ball in a pot at each end. LOW: 1.3 m."""
    g = sc.still()
    R = 300.0
    pts = [(R * 0.98 * math.sin(math.radians(a)), -230 + R * 0.98 * math.cos(math.radians(a))) for a in range(-44, 45, 22)]
    g.plan(R1, _ccw(pts + [(210, -100), (-210, -100)]), -40, 6)
    for k in range(5):                                                 # the bench bends round its sitters, who face the road
        a = -36 + k * 18
        t = math.radians(a)
        x, y = R * math.sin(t), -230 + R * math.cos(t)
        g.box(WH, (x, y, 2), (98, 52, 88), rot=(0, -a, 0))
        g.box(WH, (x + 22 * math.sin(t), y + 22 * math.cos(t), 68), (98, 16, 58), rot=(0, -a, 0))
        if k in (1, 2, 3):
            g.box(CO if k != 2 else GR, (x - 4 * math.sin(t), y - 4 * math.cos(t), 51), (80, 42, 10), rot=(0, -a, 0))
    for s in (-1, 1):
        g.cyl(WH, (s * 222, -4, -40), 30, 110, steps=8)
        g.cyl(CO, (s * 222, -4, 70), 20, 26, steps=6, top_r=27)
        g.sphere(GR, (s * 222, -4, 118), 30, steps=(4, 6))


def dovecote(sc):
    """A white dovecote tower: a square shaft, a wider head pierced with rows of small openings, a coral cap. TALL: 5.6 m."""
    g = sc.still()
    rbox(g, R1, 0, 0, -150, 30, 200, 200, 30)
    rbox(g, WH, 0, 0, 30, 300, 150, 150, 26)
    rbox(g, R1, 0, 0, 300, 314, 214, 214, 30)
    rbox(g, WH, 0, 0, 314, 436, 190, 190, 26)
    for row in range(2):
        for k in range(3):
            g.box(NV, (-50 + k * 50, -93, 348 + row * 46), (20, 8, 20), rot=(0, 0, 0) if row else (45, 0, 0))
            for s in (-1, 1):
                g.box(NV, (s * 93, -50 + k * 50, 348 + row * 46), (8, 20, 20))
    g.box(WH, (0, -100, 326), (150, 20, 8))                            # the landing ledge
    g.cyl(CO, (0, 0, 436), 140, 104, steps=4, top_r=14, rot=(0, 45, 0))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.cyl(WH, (sx * 82, sy * 82, 436), 14, 44, steps=4, top_r=4)
    g.sphere(WH, (0, 0, 548), 12, steps=(3, 5))
    g.box(GR, (0, -77, 100), (56, 6, 140))                             # a small door
    pot(g, 96, -96, 30, 1.0, flower=YEL)
    blossom(g, -84, -40, 190, 60, seed=41, n=2)


def wash_line(sc):
    """A washing line between two white posts: sheets, a towel, a shirt, all swinging; a basket. TALL: 2.4 m."""
    g = sc.still()
    for s in (-1, 1):
        g.cyl(WH, (s * 270, 0, -150), 8, 384, steps=6)
        g.box(WH, (s * 270, 0, 226), (10, 70, 8))
        g.cyl(R1, (s * 270, 0, -20), 26, 36, steps=6)
    for y in (-26, 26):
        g.box(WOOD_D, (0, y, 228), (540, 4, 4))
    cloths = ((-200, -26, WH, 96, 122), (-92, -26, CO, 64, 80), (22, -26, WH, 110, 130), (150, -26, GR, 60, 70),
              (-140, 26, CO, 84, 104), (60, 26, WH, 100, 112), (190, 26, WH, 56, 62))
    for k, (x, y, col, w, h) in enumerate(cloths):
        c = sc.part(P.motion(swing=8 + (k % 3) * 2, pivot=(x, y, 228), axis=0, rate=0.45 + 0.04 * (k % 4), phase=0.13 * k + 0.07))
        c.box(col, (x, y, 226 - h / 2.0), (w, 5, h))
    g.cyl(WOOD, (120, -70, -4), 30, 34, steps=8, top_r=36)             # the basket
    g.sphere(WH, (120, -70, 34), 28, scale=(1.0, 1.0, 0.5), steps=(3, 6))
    pot(g, -300, -40, 0, 0.9, flower=YEL)


def finger_post(sc):
    """A way post on a round stone: three blank boards with a small picture each (a dome, a wave, a tree), a little
    vane on top. No words. TALL: 3.3 m."""
    g = sc.still()
    g.cyl(R1, (0, 0, -150), 46, 180, steps=8)
    g.cyl(WH, (0, 0, 30), 9, 270, steps=6)
    g.sphere(CO, (0, 0, 306), 13, steps=(3, 6))
    boards = ((262, 0, WH), (224, 38, CO), (186, -32, WH))
    for k, (z, yaw, col) in enumerate(boards):
        t = math.radians(yaw)
        d = 1 if k != 1 else -1
        cx, cy = d * 62 * math.cos(t), d * 62 * math.sin(t)
        g.box(col, (cx, cy - 6, z), (110, 6, 26), rot=(0, yaw, 0))
        px, py = cx + d * 18 * math.cos(t), cy + d * 18 * math.sin(t) - 11
        if k == 0:                                                     # a dome
            g.cyl(CO, (px, py, z - 6), 10, 5, rot=(0, 0, 90), steps=6)
            g.box(CO, (px, py + 2, z - 9), (24, 5, 5), rot=(0, yaw, 0))
        elif k == 1:                                                   # a wave
            for j in range(3):
                g.box(WH, (px - 12 + j * 12, py + 1, z + (4 if j % 2 else -3)), (12, 5, 6), rot=(28 if j % 2 else -28, yaw, 0))
        else:                                                          # a tree
            g.cyl(GR, (px, py, z + 3), 9, 5, rot=(0, 0, 90), steps=6)
            g.box(WOOD_D, (px, py + 2, z - 8), (5, 5, 8))
    vane(sc, 0, 0, 330, phase=0.5)
    pot(g, 58, -30, 30, 0.9, flower=YEL)
    rocks(g, [(-70, 20, 40), (50, 60, 32)], seed=51)


# ------------------------------------------------------------------------------------------ THE HERO
def arch_rock(sc):
    """ARCH ROCK: a natural rock arch 55 m long, 32 m of rock with the sky through its hole (26 m wide, 20 m high),
    a white round belvedere under a coral dome on top (38 m), a white stair up its left shoulder; the dome's pennant
    swings. Not a lighthouse: no lamp, no stripes, no taper."""
    g = sc.still()
    q = random.Random(61)
    R1, R2 = '#8a86a0', '#6f6a88'       # fix round: the arch one step darker (the kit's rock greys), so it stands off the dusk sky
    outer = [(2750, 0), (2700, 1150), (2300, 2150), (1600, 2850), (650, 3150), (-500, 3200), (-1500, 2950), (-2250, 2250), (-2650, 1200), (-2750, 0)]
    inner = [(1300, 0), (1240, 700), (1020, 1300), (680, 1750), (240, 1980), (-240, 2000), (-700, 1760), (-1050, 1300), (-1260, 690), (-1300, 0)]
    front = [-640, -720, -560, -660, -600, -600, -600, -600, -600]     # each block's own face: the rock is in facets
    back = [700, 560, 660, 520, 600, 540, 640, 560, 720]
    for k in range(9):
        o0, o1, i0, i1 = outer[k], outer[k + 1], inner[k], inner[k + 1]
        g.side(R1 if k % 4 != 2 else R2, _ccw([i0, o0, o1, i1]), front[k], back[k])
        m0 = (i0[0] + (o0[0] - i0[0]) * 0.3, i0[1] + (o0[1] - i0[1]) * 0.3)
        m1 = (i1[0] + (o1[0] - i1[0]) * 0.3, i1[1] + (o1[1] - i1[1]) * 0.3)
        g.side(R2, _ccw([i0, m0, m1, i1]), front[k] - 36, back[k] + 36)        # the darker underside of the span
    for s in (-1, 1):                                                  # the legs go on into the sand
        g.side(R1, _ccw([(s * 1300, 0), (s * 2750, 0), (s * 2850, -300), (s * 1250, -300)]), -700, 720)
        g.side(R2, _ccw([(s * 1500, 0), (s * 2500, 0), (s * 2350, 520), (s * 1700, 420)]), -860, -600)   # a buttress in front
        g.side(R1, _ccw([(s * 1650, 0), (s * 2600, 0), (s * 2300, 700), (s * 1900, 560)]), 600, 900)     # and one behind
    lumps = [(2380, 2020, 300), (1700, 2700, 260), (-1550, 2880, 240), (-2330, 2100, 280), (2760, 900, 240), (-2760, 800, 220), (900, 3060, 200)]
    for k, (x, z, r) in enumerate(lumps):                              # lumps that break the outline
        g.sphere(R1 if k % 2 else R2, (x, q.uniform(-60, 60), z), r, scale=(1.2, q.uniform(1.5, 1.8), 0.7), steps=(4, 7), rot=(q.uniform(-20, 20), 0, 0))
    feet = [(-2820, -560, 260), (-2500, -980, 280), (-1700, -900, 220), (-1150, -520, 260), (-1250, 620, 300), (-2760, 640, 260),
            (2830, -500, 250), (2450, -1000, 260), (1600, -880, 240), (1150, -480, 230), (1300, 700, 320), (2760, 660, 260),
            (-600, -1050, 120), (500, -1120, 150), (150, 900, 170)]
    for k, (x, y, r) in enumerate(feet):                               # boulders round the feet and under the span
        g.sphere(R2 if k % 3 == 0 else R1, (x, y, r * 0.2), r, scale=(q.uniform(1.0, 1.3), q.uniform(0.8, 1.0), q.uniform(0.55, 0.8)), steps=(4, 7),
                 rot=(0, q.uniform(0, 90), 0))
    for x, y, z, r in ((-2050, -660, 2180, 90), (2000, -640, 2300, 110), (1250, -700, 2900, 80), (-2620, -700, 700, 100), (2640, -740, 1000, 90),
                       (-900, -640, 3080, 70), (2250, 760, 1900, 120)):     # scrub on the ledges
        g.sphere(GR if r > 85 else LEAF, (x, y, z), r, scale=(1.4, 1.0, 0.6), steps=(3, 6))
    # the stair: white flights up the left shoulder, landings where they turn
    way = [(-2660, 30), (-2540, 1120), (-2140, 2140), (-1430, 2800), (-470, 3150)]
    for (x0, z0), (x1, z1) in zip(way, way[1:]):
        tilt_box(g, WH, (x0, -660, z0), (x1, -660, z1), 170, 46)
        g.box(WH, (x1, -665, z1 - 6), (150, 190, 40))
    g.box(WH, (-2700, -700, 0), (200, 220, 90))
    for k in range(1, 4):                                              # rail posts at the landings
        g.box(WH, (way[k][0], -742, way[k][1] + 50), (12, 12, 110))
    # the belvedere
    g.cyl(R1, (0, 0, 3090), 390, 110, steps=12)
    g.cyl(WH, (0, 0, 3200), 345, 30, steps=12)
    g.box(WH, (-360, -330, 3150), (300, 150, 100))
    for k in range(8):
        t = math.radians(22.5 + k * 45)
        g.cyl(WH, (math.cos(t) * 292, math.sin(t) * 292, 3230), 24, 262, steps=6)
        g.box(WH, (math.cos(t) * 292, math.sin(t) * 292, 3238), (70, 70, 16), rot=(0, 22.5 + k * 45, 0))
        if k != 5:                                            # a low wall between the columns, open to the road
            t2 = math.radians(45.0 + k * 45)
            g.box(WH, (math.cos(t2) * 270, math.sin(t2) * 270, 3270), (200, 14, 80), rot=(0, 45.0 + k * 45 + 90, 0))
    g.cyl(WH, (0, 0, 3492), 336, 52, steps=12)
    g.cyl(CO, (0, 0, 3544), 348, 14, steps=12)
    dome(g, CO, 0, 0, 3558, 330, 230, steps=12, tiers=3)
    g.cyl(WH, (0, 0, 3230), 70, 40, steps=8)                           # a round table and a bench inside
    g.cyl(WH, (0, 0, 3270), 90, 10, steps=8)
    g.box(WH, (0, 170, 3252), (220, 50, 44))
    g.box(WH, (0, 0, 3900), (9, 9, 230))
    f = sc.part(P.motion(swing=18, pivot=(0, 0, 3900), axis=P.UP, rate=0.3, phase=0.1))
    f.side(CO, _ccw([(8, 4004), (300, 3944), (8, 3884)]), -4, 4)
    for x, y in ((-330, -40), (340, 60)):                              # two big white and coral jars by the belvedere
        jar(g, x + (-120 if x < 0 else 120), y, 3185, 150, WH, CO)


JOBS = [
    ('SM_S1D5_DomeVilla', dome_villa, STILL),
    ('SM_S1D5_TowerHouse', tower_house, dict(grow=(20, 20, 10), grow_down=(20, 20, 0))),
    ('SM_S1D5_Windmill', windmill, dict(grow=(210, 40, 220), grow_down=(210, 40, 0))),
    ('SM_S1D5_Pavilion', pavilion, dict(grow=(30, 10, 10), grow_down=(30, 10, 0))),
    ('SM_S1D5_StepRow', step_row, dict(grow=(10, 30, 0), grow_down=(10, 30, 0))),
    ('SM_S1D5_GateHouse', gate_house, STILL),
    ('SM_S1D5_CubeHouse', cube_house, STILL),
    ('SM_S1D5_TerraceVilla', terrace_villa, dict(grow=(10, 30, 0), grow_down=(10, 30, 0))),
    ('SM_S1D5_Well', well, SMALL),
    ('SM_S1D5_Sundial', sundial, STILL),
    ('SM_S1D5_JarRow', jar_row, STILL),
    ('SM_S1D5_Cairn', cairn, dict(grow=(60, 140, 0), grow_down=(140, 140, 0))),
    ('SM_S1D5_Scope', scope, SMALL),
    ('SM_S1D5_DryWall', dry_wall, STILL),
    ('SM_S1D5_Exedra', exedra, STILL),
    ('SM_S1D5_Dovecote', dovecote, STILL),
    ('SM_S1D5_WashLine', wash_line, dict(grow=(10, 60, 0), grow_down=(10, 60, 0))),
    ('SM_S1D5_FingerPost', finger_post, dict(grow=(80, 80, 10), grow_down=(80, 80, 0))),
    ('SM_S1D5_ArchRock', arch_rock, dict(grow=(60, 320, 60), grow_down=(320, 320, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
