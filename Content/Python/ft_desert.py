"""Level 3 (1.5): the Nevada desert at dusk, with Las Vegas lit up on the horizon. Built the same way as
ft_city and ft_chem: centimetres, X forward (away from the chase camera), Y right, Z up; a vehicle's
origin is the middle of its rear on the ground, a prop's is the middle of its base (its front faces -X).

The look is FULL THROTTLE's cel style at sunset: sun-baked sandstone, dusty sage, black-and-chrome
choppers, and neon in hot pink, cyan and gold. Everything here is our own: the outlaw biker club is the
SNAKE EYES (a pair of dice showing one pip each on the back patch), and the Vegas skyline is a generic
neon city (slabs, a glass pyramid, a needle tower, a dome), not a copy of any real casino.
"""
import math, random
import ft_art
from ft_car import wheel, BLACK, GLASS, CHROME, STEEL, TYRE, GOLD, PINK, CYAN, YEL, WHITE

# the desert
SAND = '#f2c58a'
SANDST = '#e0784a'      # red sandstone
SANDST_D = '#b8553a'
SANDST_L = '#f59a62'
SAGE = '#8fb86a'
SAGE_D = '#5f8f4a'
BARK = '#8a6a4a'
BARK_D = '#6a4c36'
WOOD = '#9a6a44'
# the bikers
LEATHER = '#22202a'
BEARD = '#7a4b2a'
GREY = '#c9c4d6'
FLAME = '#ff9e3d'
RED = '#ff3355'
PURP = '#7b3bd6'
MAG = '#ff4fb8'
TEAL = '#2fc6c0'
NAVY = '#2b2350'


def tilt_box(m, col, a, b, w, d=None, glow=0.0):
    """A box from point a to point b (a limb, a strut), w wide across Y and d deep."""
    d = d or w
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    yaw = math.degrees(math.atan2(dy, dx))
    pitch = math.degrees(math.atan2(dz, math.hypot(dx, dy)))
    m.box(col, ((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, (a[2] + b[2]) / 2.0), (L, w, d), rot=(pitch, yaw, 0), glow=glow)


def rod(m, col, a, b, r, steps=8, glow=0.0):
    """A cylinder from a to b."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    # the cylinder grows along local +Z; tip it over by pitch (toward -X for +pitch) and turn by yaw
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r, L, rot=(pitch, yaw, 0), steps=steps, glow=glow)


# ------------------------------------------------------------------ the SNAKE EYES on their choppers
def dice(m, x, y, z, s, glow=0.0):
    """The club's patch: two white dice, each showing one red pip (snake eyes), on the -X face."""
    for k in (-1, 1):
        m.box(WHITE, (x, y + k * s * 0.62, z), (2, s, s), rot=(0, 0, k * 12))
        m.box(RED, (x - 1.4, y + k * s * 0.62, z), (1, s * 0.3, s * 0.3), rot=(0, 0, k * 12), glow=glow)


def hog(m, variant=1, rider=True, x0=0.0):
    from ft_bikes import hog as build_hog
    build_hog(m, variant, with_rider=rider, x0=x0)


# ------------------------------------------------------------------ the boss: the motorcycle carrier
def carrier(m):
    """THE BIG DEALER's motorcycle carrier: a black-and-chrome long-nose semi with flame decals,
    hauling a two-deck open trailer loaded with the SNAKE EYES' choppers. The tail ramp is down, so the
    bikers ride straight off the back at you; the top deck ends in a kicker they launch off."""
    U = 9.14
    W, L = 64 * U, 1300
    hw = W / 2.0 - 16
    TL = 900                                    # the trailer
    # the trailer's chassis and the lower deck
    m.box(BLACK, (TL / 2.0, 0, 70), (TL, hw * 2, 40))
    DECK = '#4b4568'
    m.box(DECK, (TL / 2.0 + 20, 0, 102), (TL - 40, hw * 2, 14))
    for k in range(9):
        m.box(NAVY, (60 + k * 100, 0, 110), (8, hw * 2 - 10, 3))
    # the upper deck on posts, gold-trimmed, with neon running along its edges
    m.box(DECK, (TL / 2.0 + 60, 0, 300), (TL - 120, hw * 2, 14))
    for k in range(8):
        m.box(NAVY, (130 + k * 100, 0, 308), (8, hw * 2 - 10, 3))
    for x in (40, 300, 600, TL - 40):
        for s in (-1, 1):
            m.box(GOLD, (x, s * (hw - 10), 205), (16, 16, 200))
    for s in (-1, 1):
        m.box(MAG, (TL / 2.0 + 60, s * hw, 296), (TL - 120, 5, 8), glow=1.0)
        m.box(CYAN, (TL / 2.0, s * hw, 96), (TL, 5, 6), glow=1.0)
    # the top deck's kicker at the back: they jump off this
    m.side(YEL, [(120, 307), (0, 340), (-14, 340), (60, 307)], -hw * 0.55, hw * 0.55)
    for k in range(4):
        y = -hw * 0.55 + (k + 0.5) * hw * 1.1 / 4.0
        m.box(BLACK, (40, y, 324), (70, 14, 2), rot=(-15, 0, 0))
    # the tail ramp, down to the road behind the trailer, hazard striped
    RL = 300
    m.side('#ffd23f', [(10, 96), (10, 110), (-RL, 8), (-RL, 0)], -hw * 0.7, hw * 0.7)
    for k in range(6):
        t = (k + 0.5) / 6.0
        x = 10 - (RL + 10) * t
        z = 110 - 105 * t
        m.box(BLACK, (x, 0, z + 2), (22, hw * 1.4 + 2, 4), rot=(-19, 0, 0))
    for s in (-1, 1):
        m.box(STEEL, (-RL * 0.5, s * hw * 0.72, 60), (RL, 10, 18), rot=(-19, 0, 0))
    # tail lights, a light bar under the top deck and the plate
    for s in (-1, 1):
        m.box(RED, (-2, s * (hw - 30), 70), (3, 50, 22), glow=1.2)
    m.box(RED, (-4, 0, 286), (3, hw * 1.6, 8), glow=1.0)
    # trailer axles: four, tandem at the back
    for s in (-1, 1):
        for x in (110, 220):
            wheel(m, x, 48, 48, hw - 46, W / 2.0, s)
    # the choppers it carries: three on each deck (riders up top, ready to jump)
    for i, y in enumerate((-hw * 0.62, 0, hw * 0.62)):
        old = m.base
        m.base = ft_art._xf((300, y, 116)) if old is None else ft_art._xf((300, y, 116)) * old
        hog(m, 1 + (i % 2), rider=False)
        m.base = ft_art._xf((380, y, 314)) if old is None else ft_art._xf((380, y, 314)) * old
        hog(m, 2 - (i % 2), rider=(i != 1))
        m.base = old
    # the tractor: a long-nose semi, black with flames, twin chrome stacks and a sleeper
    cx0 = TL + 40
    m.box(BLACK, ((cx0 + L) / 2.0, 0, 70), (L - cx0, hw * 1.7, 40))
    m.side(LEATHER, [(cx0, 80), (cx0 + 170, 80), (cx0 + 170, 330), (cx0 + 150, 360), (cx0, 360)], -hw * 0.8, hw * 0.8)     # sleeper
    m.side(BLACK, [(cx0 + 170, 80), (L - 60, 80), (L, 110), (L, 200), (L - 120, 220), (cx0 + 260, 230), (cx0 + 250, 300), (cx0 + 170, 310)], -hw * 0.72, hw * 0.72)
    for s in (-1, 1):
        # flame decals down each side of the sleeper and hood
        for k in range(4):
            m.side(FLAME if k % 2 else RED, [(cx0 + 20 + k * 40, 110), (cx0 + 90 + k * 40, 150 + k * 8), (cx0 + 60 + k * 40, 120)], *((hw * 0.8, hw * 0.8 + 1.5) if s > 0 else (-hw * 0.8 - 1.5, -hw * 0.8)), glow=0.3)
        # the chrome stacks, either side of the sleeper, with a lick of exhaust
        m.cyl(CHROME, (cx0 + 150, s * (hw * 0.82), 80), 16, 380, steps=12)
        m.cyl(BLACK, (cx0 + 150, s * (hw * 0.82), 456), 14, 8, steps=12)
        for x in (cx0 + 120, L - 110):
            wheel(m, x, 50, 50, hw * 0.85 - 46, hw * 0.85, s)
    # the sleeper's back wall faces the trailer: a big lit SNAKE EYES sign on it
    m.box(NAVY, (cx0 - 4, 0, 240), (6, hw * 1.3, 150))
    dice(m, cx0 - 8, 0, 240, 70, glow=0.8)
    for s in (-1, 1):
        m.box(GOLD, (cx0 - 7, 0, 240 + s * 70), (2, hw * 1.3, 8), glow=0.6)
    # roof marker lights
    for y in (-hw * 0.5, -hw * 0.25, 0, hw * 0.25, hw * 0.5):
        m.box('#ffb020', (cx0 + 80, y, 364), (12, 14, 8), glow=1.2)


# ------------------------------------------------------------------ desert props
def joshua(m, h=620, seed=0):
    """A Joshua tree: a shaggy trunk that forks into crooked arms, each ending in a spiky ball of leaves."""
    r = random.Random(seed)

    def branch(p, ang, tilt, length, rad, depth):
        a, t = math.radians(ang), math.radians(tilt)
        q = (p[0] + math.cos(a) * math.sin(t) * length, p[1] + math.sin(a) * math.sin(t) * length, p[2] + math.cos(t) * length)
        rod(m, BARK if depth % 2 == 0 else BARK_D, p, q, rad, steps=8)
        # shaggy dead leaves down the limb
        for k in range(2):
            u = 0.35 + 0.3 * k
            pp = (p[0] + (q[0] - p[0]) * u, p[1] + (q[1] - p[1]) * u, p[2] + (q[2] - p[2]) * u)
            m.sphere(BARK_D, pp, rad * 1.35, scale=(1, 1, 1.4), steps=(5, 8))
        if depth == 0 or length < 90:
            # the crown: a ball of stiff blades
            m.sphere(SAGE_D, q, rad * 2.0, steps=(6, 10))
            for k in range(14):
                ya = k * 360 / 14.0 + r.uniform(-10, 10)
                pt = r.uniform(-60, 75)
                d = rad * 2.6
                ya_, pt_ = math.radians(ya), math.radians(pt)
                tip = (q[0] + math.cos(ya_) * math.cos(pt_) * d, q[1] + math.sin(ya_) * math.cos(pt_) * d, q[2] + math.sin(pt_) * d)
                tilt_box(m, SAGE if k % 2 else SAGE_D, q, tip, 6, 6)
            return
        n = 2 if r.random() < 0.7 else 3
        for i in range(n):
            branch(q, ang + r.uniform(-60, 60) + i * (360.0 / n), r.uniform(25, 55), length * r.uniform(0.6, 0.8), rad * 0.75, depth - 1)

    branch((0, 0, 0), 0, 4, h * 0.45, 26, 2)
    m.cyl(SANDST_D, (0, 0, 0), 44, 8, steps=10)


def rock(m, variant=0):
    """A cluster of red sandstone boulders, banded, with a sage bush at its foot."""
    r = random.Random(7 + variant)
    if variant == 0:
        blobs = [((0, 0, 140), 200, (1.2, 1, 0.8)), ((60, 200, 90), 130, (1, 1, 0.75)), ((-40, -180, 70), 110, (1.1, 1, 0.7)), ((60, -60, 280), 120, (1, 1.1, 0.7))]
    else:
        blobs = [((0, 0, 220), 260, (1, 1.2, 1.0)), ((-30, 260, 110), 140, (1, 1, 0.8)), ((10, -240, 80), 100, (1, 1, 0.9))]
    for (p, rr, sc) in blobs:
        m.sphere(SANDST, p, rr, scale=sc, steps=(6, 9))
        m.sphere(SANDST_L, (p[0], p[1], p[2] + rr * sc[2] * 0.42), rr * 0.86, scale=(sc[0], sc[1], sc[2] * 0.5), steps=(5, 8))
    for k in range(3):
        a = r.uniform(0, 6.28)
        m.sphere(SAGE if k % 2 else SAGE_D, (-260 + r.uniform(-40, 40), math.sin(a) * 200, 40), r.uniform(40, 60), scale=(1, 1, 0.7), steps=(5, 8))


def butte(m, w=4200, h=1800):
    """A far-off mesa: layered red bands, flat top, stepped sides."""
    layers = [(1.0, 0.0, 0.34, SANDST_D), (0.88, 0.34, 0.62, SANDST), (0.8, 0.62, 0.86, SANDST_L), (0.74, 0.86, 1.0, SANDST)]
    for (k, z0, z1, col) in layers:
        ww = w * k
        pts = [(-ww * 0.18, -ww / 2), (ww * 0.18, -ww / 2 - 60), (ww * 0.22, ww / 2), (-ww * 0.2, ww / 2 + 40)]
        m.plan(col, pts, h * z0, h * z1)
    m.plan(SAGE_D, [(-w * 0.2, -w * 0.55), (w * 0.2, -w * 0.55), (w * 0.2, w * 0.55), (-w * 0.2, w * 0.55)], 0, 30)


def cactus(m, seed=0):
    """Barrel cacti and a little cholla, with pink flowers."""
    r = random.Random(seed + 3)
    for k in range(4):
        x, y = r.uniform(-80, 80), r.uniform(-160, 160)
        rr = r.uniform(30, 55)
        m.sphere(SAGE_D, (x, y, rr * 0.9), rr, scale=(1, 1, 1.2), steps=(6, 10))
        for j in range(6):
            a = j * math.pi / 3
            m.box(SAGE, (x + math.cos(a) * rr * 0.92, y + math.sin(a) * rr * 0.92, rr * 0.9), (6, 6, rr * 2.0), rot=(0, math.degrees(a), 0))
        m.sphere(MAG, (x, y, rr * 2.1), rr * 0.25, steps=(4, 6), glow=0.3)
    # a cholla: a short trunk and stubby arms
    rod(m, SAGE, (40, 0, 0), (40, 0, 200), 16)
    for s in (-1, 1):
        rod(m, SAGE, (40, 0, 110 + s * 20), (40, s * 70, 170 + s * 20), 12)


def pole(m, flip=1):
    """A wooden telephone pole with a crossarm and glass insulators, and a little sodium lamp on an arm."""
    m.cyl(WOOD, (0, 0, 0), 14, 820, steps=8, top_r=11)
    m.box(WOOD, (0, 0, 760), (16, 200, 16))
    for y in (-85, -40, 40, 85):
        m.cyl('#7fe0dc', (0, y, 768), 6, 18, steps=6, glow=0.2)
    m.box(WOOD, (0, flip * 70, 600), (10, 140, 10))
    m.box(NAVY, (0, flip * 140, 590), (34, 46, 16))
    m.box('#ffb050', (0, flip * 140, 580), (30, 40, 3), glow=1.2)


def motel(m):
    """A roadside motel: a long low pastel row of doors, and a tall neon sign on a pole with an arrow
    that points at it and a star on top."""
    # the building, set back
    m.box('#ffc1dc', (700, 0, 160), (500, 1600, 320))
    m.box('#7fe3c4', (440, 0, 330), (40, 1650, 30))
    for k in range(7):
        y = -680 + k * 225
        m.box('#2fc6c0' if k % 2 else '#ffd23f', (448, y, 110), (6, 90, 200))
        m.box(GLASS, (448, y + 75, 160), (6, 50, 60))
    # the sign: a pole, a board, an arrow and a star
    m.cyl(STEEL, (-200, -500, 0), 18, 900, steps=8)
    m.box(NAVY, (-210, -500, 900), (30, 520, 220))
    m.box(MAG, (-228, -500, 990), (4, 480, 20), glow=1.2)
    m.box(CYAN, (-228, -500, 900), (4, 420, 50), glow=1.1)
    m.box(MAG, (-228, -500, 810), (4, 480, 20), glow=1.2)
    m.side(YEL, [(-240, 720), (-200, 720), (-200, 620)], -500 + 200, -500 + 330, glow=1.0)
    m.box(YEL, (-230, -380, 700), (6, 320, 40), glow=1.0)
    for k in range(5):
        a = k * 2 * math.pi / 5
        tilt_box(m, YEL, (-215, -500, 1120), (-215, -500 + math.sin(a) * 90, 1120 + math.cos(a) * 90), 18, 18, glow=1.2)
    for k in range(10):
        m.sphere('#fff3a8', (-230, -740 + k * 52, 1012), 9, steps=(4, 6), glow=1.4)


def pylon(m):
    """A casino's roadside pylon: a tall tower of stacked neon rings with a giant pair of dice on top."""
    m.box(NAVY, (0, 0, 40), (300, 300, 80))
    m.cyl(BLACK, (0, 0, 80), 70, 1500, steps=12)
    for k in range(9):
        col = (MAG, CYAN, GOLD)[k % 3]
        m.cyl(col, (0, 0, 240 + k * 140), 110, 26, steps=16, glow=1.1)
    m.box(NAVY, (0, 0, 1620), (40, 520, 220))
    m.box(GOLD, (-22, 0, 1700), (4, 500, 18), glow=1.0)
    m.box(GOLD, (-22, 0, 1540), (4, 500, 18), glow=1.0)
    for k in range(14):
        m.sphere('#fff3a8', (-24, -230 + k * 35, 1620), 8, steps=(4, 6), glow=1.5)
    # the dice, tumbling, red with white pips
    for s, rot in ((-1, (20, 15, 10)), (1, (-12, -25, 35))):
        cx, cy, cz = 0, s * 150, 1900
        m.box('#ff2d3c', (cx, cy, cz), (200, 200, 200), rot=rot)
        m.sphere(WHITE, (cx - 104, cy, cz), 26, scale=(0.3, 1, 1), steps=(4, 8), glow=0.6)


def gas(m):
    """A lonely desert gas stop: a canopy on posts with neon edges, two pumps and a little shop."""
    m.box(WHITE, (0, 0, 470), (500, 900, 40))
    m.box(RED, (-252, 0, 470), (4, 900, 30), glow=0.9)
    for y in (-380, 380):
        for x in (-200, 200):
            m.box(WHITE, (x, y, 230), (30, 30, 460))
    for y in (-180, 180):
        m.box(RED, (0, y, 90), (60, 70, 180))
        m.box('#fff3a8', (-32, y, 140), (4, 50, 30), glow=1.0)
    m.box('#ffe2a8', (500, 0, 170), (400, 700, 340))
    m.box(GLASS, (298, 0, 150), (6, 500, 160))
    m.box(CYAN, (296, 0, 300), (4, 600, 30), glow=1.2)


# ------------------------------------------------------------------ Las Vegas on the horizon
NEON = [MAG, CYAN, GOLD, '#ff7a3d', '#b44dff']
LITW = ['#ffe9a8', '#9ff0ff', '#ffc0e8']


def slab(m, x, y, w, h, col, neon, r, gold=False):
    """A hotel tower: a dark slab with rows of lit windows, a neon crown and a neon edge."""
    m.box(col, (x + 300, y, h / 2.0), (600, w, h))
    f = 0
    z = 180
    while z < h - 160:
        # a row of windows, broken into bays
        nb = max(2, int(w / 160))
        for b in range(nb):
            if r.random() < 0.82:
                yy = y - w / 2.0 + (b + 0.5) * w / nb
                m.box(r.choice(LITW) if not gold else '#ffe28a', (x - 2, yy, z), (4, w / nb * 0.62, 70), glow=0.75)
        z += 190
        f += 1
    m.box(neon, (x - 4, y, h - 60), (6, w + 10, 80), glow=1.4)
    m.box(neon, (x - 4, y - w / 2.0 + 25, h / 2.0), (6, 40, h - 120), glow=1.3)
    m.box(neon, (x - 4, y + w / 2.0 - 25, h / 2.0), (6, 40, h - 120), glow=1.3)


def vegas(m, width=12000, seed=21):
    """The city at night, far off: rows of hotel slabs with lit windows and neon crowns, a black glass
    pyramid, a needle tower with an observation pod in the middle, a gold tower, a gold dome and tall
    lit signs. Our own skyline, not a copy of any real casino."""
    r = random.Random(seed)
    SLAB = ['#2b2350', '#33295e', '#24203f', '#3a2f6a', '#1e1a36']
    hw = width / 2.0
    # the back row: low slabs packed edge to edge
    y = -hw
    while y < hw:
        w = r.choice([700, 900, 1100])
        slab(m, 2400, y + w / 2.0, w, r.choice([1300, 1600, 2000, 2300]), r.choice(SLAB), r.choice(NEON), r)
        y += w + r.choice([0, 60])
    # the middle row: the big towers
    for (yy, w, h) in [(-5200, 900, 3000), (-4100, 1000, 3800), (-2600, 800, 2800), (-1700, 900, 3400), (1900, 1000, 4000), (3000, 800, 3000), (4300, 1100, 3600), (5400, 800, 2600)]:
        slab(m, 1300, yy, w, h, r.choice(SLAB), r.choice(NEON), r)
    # the gold tower beside the needle
    slab(m, 900, -700, 900, 4400, '#8a6a2a', GOLD, r, gold=True)
    # the needle tower with its pod, dead centre
    ny = 700
    m.cyl('#c9c4d6', (800, ny, 0), 130, 5000, steps=12, top_r=80)
    for k in range(6):
        m.cyl(CYAN, (800, ny, 600 + k * 700), 136 - k * 8, 30, steps=12, glow=1.2)
    m.cyl('#43337a', (800, ny, 5000), 320, 260, steps=16, top_r=400)
    m.cyl(CYAN, (800, ny, 5130), 410, 50, steps=20, glow=1.5)
    m.cyl(MAG, (800, ny, 5260), 380, 120, steps=20, top_r=140, glow=0.8)
    m.cyl('#ff3355', (800, ny, 5380), 14, 700, steps=6, glow=1.5)
    # the black glass pyramid, front left, gold on every step
    py, b = -3400, 1150
    for k in range(12):
        sc = 1.0 - k / 12.0
        m.plan('#1c1a26' if k % 2 else '#28243c', [(-b * sc, -b * sc), (b * sc, -b * sc), (b * sc, b * sc), (-b * sc, b * sc)], k * 150, (k + 1) * 150, loc=(b, py, 0))
        m.box(GOLD, (b - b * sc - 3, py, (k + 1) * 150), (6, 2 * b * sc, 18), glow=1.1)
    m.sphere('#fff3a8', (b, py, 12 * 150 + 40), 70, glow=2.2)
    # the gold dome, front right
    dy = 3300
    m.cyl('#3a2f6a', (700, dy, 0), 650, 800, steps=24)
    for k in range(3):
        m.box(r.choice(LITW), (48, dy, 200 + k * 220), (4, 900, 60), glow=0.8)
    m.cyl(GOLD, (700, dy, 780), 665, 50, steps=24, glow=1.2)
    m.sphere('#ffc93c', (700, dy, 820), 640, scale=(1, 1, 0.62), steps=(8, 24), glow=0.45)
    # tall lit signs out front
    for (sy, h, neon) in [(-1800, 1900, MAG), (2300, 2200, CYAN), (-5600, 1500, GOLD), (5600, 1700, '#ff7a3d')]:
        m.box('#1e1a36', (100, sy, h / 2.0), (160, 160, h))
        m.box(neon, (-6, sy, h), (8, 800, 460), glow=1.5)
        m.box('#1e1a36', (-12, sy, h), (4, 640, 280))
        for k in range(8):
            m.sphere('#fff3a8', (-16, sy - 300 + k * 86, h), 34, steps=(4, 6), glow=1.8)
    # the low town in front of it all, a band of lights
    m.box('#1e1a36', (-200, 0, 260), (300, width + 400, 520))
    for k in range(60):
        m.box(r.choice(NEON + LITW), (-352, -hw + (k + 0.5) * width / 60.0, 140 + (k % 3) * 120), (4, width / 80.0, 46), glow=1.3)
