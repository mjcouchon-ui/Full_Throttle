"""Ice Mountain, Stage 1, builder s1d8: district 8 "The high ridge" (19.7 km to the end). /Game/World/IceMountain/SM_S1D8_*.
Run: import ft_world_ice_s1d8 as K; K.run()

THE BEACONS. A ridge of signal stations above the last lifts: tapering round towers banded red and cream with a
gallery and a lamp room, keepers' houses under red hipped roofs, signal masts with arms, stone bases. Signal red
leads; cream only as bands between red; charcoal bases and caps; yellow lamp rooms (steady: nothing here blinks,
turns a beam or strobes); slate stone; dark glass. Every colour is one of ft_dress_ice.py's.

Seven buildings (the small beacon, the high beacon of six storeys, the keeper's house, the signal station, the horn
house, the lift's top terminal, the ridge shelter), nine props (TALL: signal-arm mast, banded marker cone, fog bell,
flag hoist, beacon lamp; LOW, 1.9 m and under: banded bollard row, lamp cairn, flag locker, rope-and-post walkway)
and the hero, THE GREAT BEACON (60 m).

What the hill asks (FTDressL5.inl): the road falls 7 in 100 and the land with it, so every building has 2.8 m of base
under z = 0 (the hero 4 m) and the low props stand on posts that go 40 to 60 cm down.
The camera looks ALONG the road: the ends (+X, -X) of every building carry windows and detail like the front.
No figures (the people are s1people's), no vehicle, nothing orange, nothing white and round, no words, no smoke.
What moves (all in the vertex shader, slowly): weather vanes, pennants and signal flags, semaphore arms (0.1 to 0.14
swings a second), the fog bell, the washing on the keeper's line, the lift's wheel and its two empty chairs, the horn
house's rocking beam. The lamps and windows glow steadily (their pieces are placed 25 m out or more).
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/IceMountain'
RED, CREAM, CHAR, YEL, SLATE, GLASS = '#ff2d3c', '#f4efe6', '#3b3f52', '#ffd23f', '#5a6478', '#1d2740'
SNOW, WARM, ROCK = '#f4f9ff', '#ffe28a', '#6a7488'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))     # no moving part: true bounds
SMALL = dict(grow=(30, 30, 30), grow_down=(30, 30, 0))   # parts that swing inside (or just past) the piece's own box
G_WIN, G_LAMP = 0.6, 1.2                               # a warm window's and a lamp room's steady glow
FOOT = -280.0                                          # how far a building's base goes into the ground


# ------------------------------------------------------------------------------------------ helpers
def ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def pol(r, a):
    """(x, y) at radius r, angle a in degrees (0 = +X, -90 = the front)."""
    return (r * math.cos(math.radians(a)), r * math.sin(math.radians(a)))


def cone(m, col, a, b, r0, r1=None, steps=8, glow=0.0):
    """A cylinder or a cone from a to b (rod()'s own turn, with a second radius)."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r0, L, rot=(pitch, yaw, 0), steps=steps, glow=glow, top_r=r1)


def bands(g, x, y, z0, r0, r1, h, n, steps=12, cols=(RED, CREAM), yaw=0.0):
    """A tapering round shaft in n bands of colour, from radius r0 at z0 to r1 at z0 + h."""
    for k in range(n):
        ra = r0 + (r1 - r0) * k / float(n)
        rb = r0 + (r1 - r0) * (k + 1) / float(n)
        g.cyl(cols[k % len(cols)], (x, y, z0 + h * k / float(n)), ra, h / float(n), rot=(0, yaw, 0), steps=steps, top_r=rb)


def rwin(g, cx, cy, R, a, z, w=60.0, h=100.0, lit=False, hood=True):
    """A window on a round wall of radius R about (cx, cy), at angle a: a dark pane sunk in the wall, a hood over it."""
    dx, dy = pol(1.0, a)
    g.box(GLASS, (cx + dx * (R - 8), cy + dy * (R - 8), z), (30, w, h), rot=(0, a, 0))
    if lit:
        g.box(WARM, (cx + dx * (R + 8), cy + dy * (R + 8), z), (5, w - 16, h - 16), rot=(0, a, 0), glow=G_WIN)
    if hood:
        g.box(CHAR, (cx + dx * (R + 2), cy + dy * (R + 2), z + h / 2.0 + 8), (30, w + 18, 12), rot=(0, a, 0))


def fwin(g, x, y, z, w=80.0, h=110.0, n=(0, -1), lit=False, shutters=None, sill=None):
    """A window on a flat wall at (x, y, z), the wall's outward normal n = (0, -1) front, (1, 0) the +X end ..."""
    ax = abs(n[0]) > 0.5
    g.box(GLASS, (x, y, z), (10, w, h) if ax else (w, 10, h))
    if lit:
        g.box(WARM, (x + n[0] * 5, y + n[1] * 5, z), (4, w - 16, h - 16) if ax else (w - 16, 4, h - 16), glow=G_WIN)
    if shutters:
        for s in (-1, 1):
            o = s * (w / 2.0 + 14)
            g.box(shutters, (x + n[0] * 3, y + o, z) if ax else (x + o, y + n[1] * 3, z), (8, 24, h + 6) if ax else (24, 8, h + 6))
    if sill:
        g.box(sill, (x + n[0] * 8, y + n[1] * 8, z - h / 2.0 - 7), (22, w + 20, 10) if ax else (w + 20, 22, 10))


def hip_roof(g, col, z, L, W, h):
    """A hipped roof centred on the piece's origin: eaves L x W at z, the ridge h higher (L along X, L >= W)."""
    half = (L - W) / 2.0
    if half > 1.0:
        g.side(col, ccw([(-W / 2.0, z), (W / 2.0, z), (0.0, z + h)]), -half, half, rot=(0, 90, 0))
    for s in ((-1, 1) if half > 1.0 else (0,)):
        g.cyl(col, (s * half, 0, z), W / 2.0 * 1.41421, h, rot=(0, 45, 0), steps=4, top_r=3)


def snowy_hip(g, col, z, L, W, h, eave=CHAR, share=0.58):
    """A hipped roof with its eaves board and a cap of snow lying on the upper part of it."""
    g.box(eave, (0, 0, z - 6), (L + 10, W + 10, 12))
    hip_roof(g, col, z, L, W, h)
    hip_roof(g, SNOW, z + h * (1.0 - share) + 12, (L - W) + W * share, W * share, h * share)


def gallery(g, x, y, z, r, steps=12, posts=8, rail=88.0, a0=0.0):
    """A lighthouse gallery: a deck, posts, a rail. Returns the deck's top."""
    g.cyl(CHAR, (x, y, z), r, 24, steps=steps, rot=(0, a0, 0))
    for k in range(posts):
        px, py = pol(r - 12, a0 + 360.0 * (k + 0.5) / posts)
        g.box(CHAR, (x + px, y + py, z + 24 + rail / 2.0), (9, 9, rail))
    g.cyl(CHAR, (x, y, z + 24 + rail), r, 9, steps=steps, rot=(0, a0, 0))
    return z + 24


def lamp_room(g, x, y, z, r, h, steps=8, glow=G_LAMP, a0=0.0, bar=10.0):
    """A lamp room: a steady yellow drum of light behind charcoal glazing bars. Returns its top."""
    g.cyl(CHAR, (x, y, z), r + 14, 16, steps=steps, rot=(0, a0, 0))
    g.cyl(YEL, (x, y, z + 16), r, h, steps=steps, rot=(0, a0, 0), glow=glow)
    for k in range(steps):
        a = a0 + 360.0 * k / steps
        px, py = pol(r, a)
        g.box(CHAR, (x + px, y + py, z + 16 + h / 2.0), (bar, bar, h), rot=(0, a, 0))
    g.cyl(CHAR, (x, y, z + 16 + h), r + 16, 14, steps=steps, rot=(0, a0, 0))
    return z + 30 + h


def vane(sc, x, y, z, size=60.0, rate=0.12, phase=0.0, mast=70.0):
    """A weather vane swinging slowly on a short mast whose foot is (x, y, z)."""
    g = sc.still()
    g.cyl(CHAR, (x, y, z), 5, mast, steps=5)
    g.box(CHAR, (x, y, z + mast * 0.55), (size * 0.9, 5, 5))
    g.box(CHAR, (x, y, z + mast * 0.55), (5, size * 0.9, 5))
    v = sc.part(P.motion(swing=26, pivot=(x, y, z + mast), axis=P.UP, rate=rate, phase=phase))
    v.box(YEL, (x, y, z + mast + 4), (size * 1.9, 5, 8))
    v.box(YEL, (x - size * 0.78, y, z + mast + 16), (size * 0.42, 5, size * 0.42))
    v.box(RED, (x + size * 0.9, y, z + mast + 4), (size * 0.26, 7, 18))


def pennant(sc, x, y, z, length, col, phase=0.0, d=1, n=3, h=44.0, tip=CREAM, rate=0.8):
    """A long pennant streaming along X (d = 1: towards +X) from a staff at (x, y, z): it snakes in n lengths."""
    for k in range(n):
        m = sc.part(P.motion(swing=5 + 4 * k, pivot=(x, y, z), axis=P.UP, rate=rate, phase=phase + 0.14 * k))
        hh = h * (1.0 - 0.26 * k)
        m.box(tip if k == n - 1 else col, (x + d * (k + 0.5) * length / n, y, z), (length / n + 2, 5, hh))


def steps_up(g, col, x, y, z0, n, w, run=34.0, rise=20.0, d=(0, -1)):
    """n steps leading down and out from a door sill at (x, y, z0 + n * rise) along d."""
    for k in range(n):
        top = z0 + (n - k) * rise
        cx, cy = x + d[0] * (k + 0.5) * run, y + d[1] * (k + 0.5) * run
        g.box(col, (cx, cy, (top + z0 - 60) / 2.0), (run if d[0] else w, w if d[0] else run, top - z0 + 60))


def rock(g, r, rad, n, z0, z1, col, jit=0.14, x=0.0, y=0.0):
    pts = []
    for k in range(n):
        a = 6.28318 * k / n
        q = rad * (1.0 + r.uniform(-jit, jit))
        pts.append((x + math.cos(a) * q, y + math.sin(a) * q))
    g.plan(col, ccw(pts), z0, z1)


def bunting(sc, a, b, n, cols=(RED, CREAM, YEL), size=40.0, drop=0.0, phase=0.0, rate=0.7, thick=5.0):
    """A line from a to b with n small flags hanging on it, each fluttering on its own beat about the line's own run."""
    g = sc.still()
    tilt_box(g, CHAR, a, b, thick, thick)
    ax = math.degrees(math.atan2(b[1] - a[1], b[0] - a[0]))
    flat = abs(math.cos(math.radians(ax))) > 0.7                                   # the line runs along X: the flags are thin in Y
    for k in range(n):
        t = (k + 0.7) / (n + 0.4)
        px, py, pz = a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t - drop
        m = sc.part(P.motion(swing=11, pivot=(px, py, pz), axis=ax, rate=rate, phase=phase + 0.23 * k))
        m.box(cols[k % len(cols)], (px, py, pz - size * 0.55), (size, thick, size) if flat else (thick, size, size))


def tub(g, x, y, z, r=30.0, a=RED, b=YEL):
    """A slate planter tub with two tufts of colour (winter flowers), set down at (x, y, z)."""
    g.cyl(SLATE, (x, y, z), r, r * 1.1, steps=6, top_r=r * 1.2)
    g.box(a, (x - r * 0.3, y, z + r * 1.25), (r * 0.7, r * 0.7, r * 0.5))
    g.box(b, (x + r * 0.35, y + r * 0.2, z + r * 1.2), (r * 0.6, r * 0.6, r * 0.4))


# ------------------------------------------------------------------------------------------ the hero
def great_beacon(sc):
    """THE GREAT BEACON: 60 m. A charcoal crag, a slate drum, a shaft of eight red and cream bands, two galleries, a
    steady yellow lamp room under a red dome, a semaphore yard, long pennants; the keeper's house and the horns at its foot."""
    r = random.Random(808)
    g = sc.still()
    rock(g, r, 1480, 10, -400, 150, CHAR)
    rock(g, r, 1260, 9, 150, 330, SLATE, x=40, y=30)
    rock(g, r, 1040, 8, 330, 480, CHAR, x=-30, y=20)
    for a, q in ((205, 1240), (300, 1180), (20, 1220), (120, 1150)):          # snow lying on the crag's ledges
        px, py = pol(q, a)
        g.sphere(SNOW, (px, py, 335), 100, scale=(2.4, 1.5, 0.28), steps=(3, 6), rot=(0, a + 90, 0))
    g.cyl(SLATE, (0, 0, 480), 900, 300, steps=12)                              # the stone drum
    g.cyl(CHAR, (0, 0, 780), 930, 30, steps=12)
    Z0, H, R0, R1 = 810.0, 3840.0, 780.0, 500.0

    def R(z):
        return R0 + (R1 - R0) * (z - Z0) / H

    bands(g, 0, 0, Z0, R0, R1, H, 8, steps=16)
    # the door, up a flight of stone steps on the front
    g.box(CHAR, (0, -R(960) + 14, 960), (150, 60, 300))
    g.box(YEL, (0, -R(960) - 14, 1124), (210, 30, 28))
    g.box(RED, (0, -R(960) - 18, 950), (110, 8, 260))
    steps_up(g, SLATE, 0, -900, 330, 12, 260, run=30, rise=39.5, d=(0, -1))
    # windows winding up the shaft: front, both ends, the back
    wins = ((-90, 1500, True), (0, 1300, False), (180, 1700, True), (-90, 2340, False), (0, 2140, True), (180, 2560, False),
            (-90, 3220, True), (0, 3000, False), (180, 3440, True), (90, 1900, False), (-90, 4060, True), (0, 3860, True), (180, 4240, False))
    for a, z, lit in wins:
        rwin(g, 0, 0, R(z), a, z, w=110, h=180, lit=lit)
    # the lower gallery, half way up
    zl = Z0 + H * 0.5
    g.cyl(CHAR, (0, 0, zl - 12), R(zl) + 90, 24, steps=16)
    for k in range(12):
        px, py = pol(R(zl) + 78, 15 + 30 * k)
        g.box(CHAR, (px, py, zl + 56), (10, 10, 90))
    g.cyl(CHAR, (0, 0, zl + 100), R(zl) + 90, 10, steps=16)
    # corbels and the main gallery
    zt = Z0 + H
    for k in range(8):
        px, py = pol(R1 + 60, 22.5 + 45 * k)
        g.box(CHAR, (px, py, zt - 70), (150, 46, 120), rot=(0, 22.5 + 45 * k, 0))
    gallery(g, 0, 0, zt, R1 + 190, steps=16, posts=16, rail=110)
    g.cyl(CREAM, (0, 0, zt + 24), 400, 120, steps=12)                           # the watch room under the lamp
    for a in (-90, 0, 180):
        rwin(g, 0, 0, 400, a, zt + 88, w=120, h=70, lit=True, hood=False)
    top = lamp_room(g, 0, 0, zt + 144, 360, 520, steps=12, bar=16)
    g.cyl(RED, (0, 0, top), 430, 60, steps=12, top_r=400)
    g.sphere(RED, (0, 0, top + 60), 400, scale=(1.0, 1.0, 0.62), steps=(4, 12))
    g.cyl(CREAM, (0, 0, top + 280), 60, 70, steps=8, top_r=40)
    g.cyl(CHAR, (0, 0, top + 350), 9, 330, steps=6)
    vane(sc, 0, 0, top + 560, size=150, rate=0.08, mast=120)
    # the semaphore yard on the gallery's -X side, two arms; long pennants from the gallery's +X side
    gx = -(R1 + 150)
    g.cyl(CHAR, (gx, 0, zt + 24), 14, 760, steps=6)
    g.box(CHAR, (gx, -12, zt + 600), (360, 12, 12))
    for k, (zz, d) in enumerate(((zt + 700, -1), (zt + 300, -1))):
        m = sc.part(P.motion(swing=30, pivot=(gx, -22, zz), axis=90, rate=0.1 + 0.03 * k, phase=0.4 * k))
        m.box(RED, (gx + d * 190, -22, zz), (380, 12, 60))
        m.box(CREAM, (gx + d * 300, -24, zz), (90, 14, 62))
        m.box(CHAR, (gx - d * 50, -22, zz), (100, 12, 40))
    px = R1 + 170
    g.cyl(CHAR, (px, 0, zt + 24), 12, 520, steps=6)
    pennant(sc, px, 0, zt + 500, 900, RED, 0.0, d=1, n=4, h=110, tip=CREAM, rate=0.5)
    pennant(sc, px, 0, zt + 330, 640, YEL, 0.3, d=1, n=3, h=84, tip=RED, rate=0.6)
    # the keeper's house against the drum, on the front right of the crag
    hx, hy = 620.0, -980.0
    g.box(SLATE, (hx, hy, 480), (480, 420, 360))
    g.box(CREAM, (hx, hy, 800), (460, 400, 300))
    g.box(RED, (hx, hy, 760), (468, 408, 34))
    g.box(CHAR, (hx, hy, 954), (530, 470, 14))
    g.cyl(RED, (hx, hy, 960), 250 * 1.41421, 200, rot=(0, 45, 0), steps=4, top_r=3)
    g.cyl(SNOW, (hx, hy, 1052), 140 * 1.41421, 112, rot=(0, 45, 0), steps=4, top_r=3)
    g.box(CHAR, (hx + 150, hy + 60, 1090), (60, 60, 220))
    for wx, lit in ((-120, True), (120, True)):
        fwin(g, hx + wx, hy - 202, 830, w=90, h=120, lit=lit, shutters=RED)
    fwin(g, hx + 232, hy, 830, w=90, h=120, n=(1, 0), lit=True)
    g.box(YEL, (hx, hy - 212, 585), (80, 8, 150))
    # the oil store on the left, and the horns on its roof (plain flaring trumpets, charcoal with yellow mouths)
    ox, oy = -700.0, -900.0
    g.box(SLATE, (ox, oy, 560), (380, 320, 520))
    g.box(RED, (ox, oy, 830), (420, 360, 22))
    g.box(SNOW, (ox, oy + 30, 850), (360, 260, 18))
    g.box(GLASS, (ox, oy - 162, 620), (120, 8, 180))
    for s in (-1, 1):
        a, b = (ox + s * 60, oy - 40, 960), (ox + s * 330, oy - 240, 1080)
        g.box(CHAR, (ox + s * 60, oy - 40, 900), (40, 40, 120))
        cone(g, CHAR, a, b, 26, 120, steps=8)
        d = [(b[i] - a[i]) for i in range(3)]
        L = math.sqrt(sum(v * v for v in d))
        e = [b[i] + d[i] / L * 16 for i in range(3)]
        cone(g, YEL, b, e, 120, 136, steps=8)
    # a line of signal flags from the gallery down to a banded day-mark on the crag, and a rail up the steps
    dm = (1150.0, -520.0)
    bands(g, dm[0], dm[1], 150, 70, 40, 420, 4, steps=6)
    g.cyl(CHAR, (dm[0], dm[1], 570), 50, 14, steps=6)
    bunting(sc, (R1 + 170, -60, zt + 60), (dm[0], dm[1], 584), 9, size=120, phase=0.1, rate=0.5, thick=9)
    for s in (-1, 1):
        tilt_box(g, RED, (s * 136, -905, 900), (s * 136, -1255, 440), 10, 10)
        for k in range(4):
            g.box(CHAR, (s * 136, -920 - 110 * k, 820 - 145 * k), (10, 10, 150))
    for k in range(8):                                                           # yellow day-marks under the gallery
        px, py = pol(R1 + 20, 45 * k)
        g.box(YEL, (px, py, zt - 190), (30, 60, 60), rot=(0, 45 * k, 0))
    # stones and a banded bollard walk at the crag's foot
    for k in range(5):
        px, py = pol(1330, -150 + 30 * k)
        g.cyl(RED if k % 2 == 0 else CREAM, (px, py, 120), 34, 150, steps=6)
        g.cyl(CHAR, (px, py, 270), 40, 16, steps=6)


# ------------------------------------------------------------------------------------------ buildings
def beacon(sc):
    """The small beacon, 20 m: six bands, a gallery, a lamp room, a red cap and a vane; an oil store against its foot."""
    g = sc.still()
    g.cyl(SLATE, (0, 0, FOOT), 430, 150 - FOOT, steps=10)
    g.cyl(CHAR, (0, 0, 150), 352, 18, steps=12)
    Z0, H, R0, R1 = 150.0, 1350.0, 330.0, 220.0

    def R(z):
        return R0 + (R1 - R0) * (z - Z0) / H

    bands(g, 0, 0, Z0, R0, R1, H, 6, steps=12)
    g.box(CHAR, (0, -R(250) + 10, 262), (96, 44, 196))
    g.box(RED, (0, -R(250) - 13, 258), (70, 6, 170))
    g.box(YEL, (0, -R(250) - 12, 370), (130, 24, 18))
    g.box(CHAR, (0, -R(250) - 46, 392), (170, 96, 10))                            # a flat canopy on two brackets
    for s in (-1, 1):
        g.box(CHAR, (s * 76, -R(250) - 40, 372), (8, 80, 8))
    g.box(SLATE, (0, -386, 158), (170, 90, 20))
    for a, z, lit in ((-90, 640, True), (-90, 1120, False), (0, 520, False), (0, 980, True), (0, 1340, False),
                      (180, 760, True), (180, 1220, False), (90, 900, False)):
        rwin(g, 0, 0, R(z), a, z, w=58, h=96, lit=lit)
    zt = Z0 + H
    gallery(g, 0, 0, zt, 318, steps=12, posts=8, rail=80)
    top = lamp_room(g, 0, 0, zt + 24, 150, 230, steps=8)
    g.cyl(RED, (0, 0, top), 205, 190, steps=8, top_r=14)
    g.cyl(CREAM, (0, 0, top + 180), 22, 34, steps=6)
    vane(sc, 0, 0, top + 210, size=52, rate=0.12, mast=80)
    # the oil store against the +X side: slate, a red lid of a roof under snow, a little door
    g.box(SLATE, (440, 50, (FOOT + 250) / 2.0), (250, 270, 250 - FOOT))
    g.box(RED, (446, 50, 258), (290, 310, 16))
    g.box(SNOW, (446, 62, 274), (240, 250, 16))
    g.box(GLASS, (567, 50, 120), (8, 80, 150))
    g.box(YEL, (569, 50, 204), (8, 100, 12))
    # a signal pennant on the gallery's -X side
    g.cyl(CHAR, (-300, 0, zt + 24), 6, 250, steps=5)
    pennant(sc, -300, 0, zt + 240, 250, RED, 0.2, d=-1, n=3, h=50)


def high_beacon(sc):
    """The high beacon: six storeys, eight-sided, broad bands, a balcony half way and a domed lamp room. 29 m."""
    g = sc.still()
    A = 22.5                                                                      # a flat face to the road
    g.cyl(CHAR, (0, 0, FOOT), 540, 260 - FOOT, steps=8, rot=(0, A, 0))
    for a in (-45, 45, 135, 225):
        px, py = pol(500, a)
        g.box(SLATE, (px, py, (FOOT + 200) / 2.0), (150, 110, 200 - FOOT), rot=(0, a, 0))
    Z0, H, R0, R1 = 260.0, 2040.0, 430.0, 300.0
    F = math.cos(math.radians(22.5))

    def R(z):
        return (R0 + (R1 - R0) * (z - Z0) / H) * F

    bands(g, 0, 0, Z0, R0, R1, H, 6, steps=8, cols=(RED, CREAM, CREAM, RED, CREAM, RED), yaw=A)
    for k in range(1, 6):                                                         # a charcoal string course at each floor
        z = Z0 + H * k / 6.0
        g.cyl(CHAR, (0, 0, z - 8), (R0 + (R1 - R0) * k / 6.0) + 10, 16, steps=8, rot=(0, A, 0))
    for k in range(6):
        z = Z0 + H * (k + 0.5) / 6.0 + 10
        rwin(g, 0, 0, R(z), -90, z, w=70, h=120, lit=(k % 2 == 1)) if k else None
        if k % 2 == 0:
            rwin(g, 0, 0, R(z), 0, z, w=70, h=120, lit=(k == 2))
        else:
            rwin(g, 0, 0, R(z), 180, z, w=70, h=120, lit=(k == 3))
    g.box(CHAR, (0, -R(380) + 12, 372), (110, 50, 224))                           # the door, yellow framed, five steps up
    g.box(YEL, (0, -R(380) - 12, 494), (150, 24, 20))
    g.box(RED, (0, -R(380) - 15, 366), (80, 6, 200))
    steps_up(g, SLATE, 0, -R(380) - 10, 160, 5, 150, run=32, rise=20)
    zb = Z0 + H * 0.5                                                             # the balcony half way up, front only
    g.box(CHAR, (0, -R(zb) - 50, zb), (300, 130, 16))
    for s in (-1, 0, 1):
        g.box(CHAR, (s * 140, -R(zb) - 108, zb + 48), (9, 9, 84))
    g.box(RED, (0, -R(zb) - 108, zb + 92), (300, 10, 10))
    zt = Z0 + H
    gallery(g, 0, 0, zt, 400, steps=8, posts=8, rail=90, a0=A)
    top = lamp_room(g, 0, 0, zt + 24, 200, 290, steps=8, a0=A)
    g.sphere(RED, (0, 0, top), 240, scale=(1.0, 1.0, 0.72), steps=(4, 8))
    g.cyl(CREAM, (0, 0, top + 160), 26, 44, steps=6)
    vane(sc, 0, 0, top + 200, size=64, rate=0.1, phase=0.3, mast=90)
    # one semaphore arm off the gallery, and a long pennant
    g.cyl(CHAR, (330, 0, zt + 24), 8, 330, steps=5)
    m = sc.part(P.motion(swing=28, pivot=(330, -14, zt + 320), axis=90, rate=0.11, phase=0.5))
    m.box(RED, (430, -14, zt + 320), (220, 9, 34))
    m.box(CREAM, (500, -15, zt + 320), (50, 10, 36))
    g.cyl(CHAR, (-330, 0, zt + 24), 7, 300, steps=5)
    pennant(sc, -330, 0, zt + 290, 320, YEL, 0.6, d=-1, n=3, h=56, tip=RED)


def keeper_house(sc):
    """The keeper's house: two floors of cream render over slate, a red band, a red hipped roof under snow, red
    shutters, a striped awning over a yellow door, a dormer, window boxes, a vane, the washing on a line."""
    g = sc.still()
    L, W = 880.0, 620.0
    g.box(SLATE, (0, 0, (FOOT + 70) / 2.0), (L + 40, W + 40, 70 - FOOT))
    g.box(CREAM, (0, 0, 330), (L, W, 520))
    g.box(RED, (0, 0, 322), (L + 12, W + 12, 36))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(RED, (sx * (L / 2.0 - 16), sy * (W / 2.0 - 16), 330), (44, 44, 520))
    snowy_hip(g, RED, 590, L + 110, W + 110, 290)
    g.box(CHAR, (240, 90, 790), (74, 74, 300))
    g.box(CREAM, (240, 90, 950), (90, 90, 22))
    y = -W / 2.0 - 4
    for wx, lit in ((-290, True), (290, False)):
        fwin(g, wx, y, 200, w=96, h=120, lit=lit, shutters=RED, sill=CHAR)
        g.box(RED, (wx - 22, y - 16, 138), (22, 14, 14))                          # a window box in flower
        g.box(YEL, (wx + 20, y - 16, 138), (22, 14, 14))
    for wx, lit in ((-290, False), (0, True), (290, True)):
        fwin(g, wx, y, 450, w=90, h=110, lit=lit, shutters=RED)
    g.box(CHAR, (0, y - 2, 172), (116, 12, 214))
    g.box(YEL, (0, y - 9, 168), (92, 6, 196))
    for k in range(5):                                                            # the awning: red and cream stripes
        g.box(RED if k % 2 == 0 else CREAM, (-72 + 36 * k, y - 50, 296), (36, 100, 10))
    for s in (-1, 1):
        g.box(CHAR, (s * 88, y - 96, 180), (7, 7, 226))
    steps_up(g, SLATE, 0, y - 6, 10, 3, 150, run=30, rise=20)
    g.box(CHAR, (-86, y - 10, 268), (16, 14, 26))                                 # the door lamp, steady
    g.box(YEL, (-86, y - 20, 262), (12, 8, 16), glow=G_WIN)
    # the dormer
    g.box(CREAM, (0, -250, 700), (130, 150, 120))
    g.box(RED, (0, -260, 766), (156, 180, 14))
    g.box(GLASS, (0, -327, 700), (84, 6, 76))
    g.box(WARM, (0, -331, 700), (68, 4, 60), glow=G_WIN)
    # the ends and the back
    for s in (-1, 1):
        fwin(g, s * (L / 2.0 + 4), -130, 200, w=90, h=120, n=(s, 0), lit=(s > 0), shutters=RED)
        fwin(g, s * (L / 2.0 + 4), 130, 450, w=90, h=110, n=(s, 0), lit=(s < 0), shutters=RED)
    for wx in (-200, 200):
        fwin(g, wx, W / 2.0 + 4, 450, w=90, h=110, n=(0, 1))
    vane(sc, -300, 0, 735, size=50, rate=0.13, phase=0.7, mast=80)
    # a glazed porch on the +X end under a red lid, a rain butt, a bench, tubs, a low fence along the front
    px = L / 2.0 + 90
    g.box(SLATE, (px, 120, 30), (180, 300, 180))
    g.box(GLASS, (px, 120, 190), (170, 290, 150))
    g.box(WARM, (px + 86, 120, 190), (4, 220, 110), glow=G_WIN)
    for wy in (-25, 120, 265):
        g.box(CHAR, (px + 86, wy, 190), (8, 10, 150))
    g.box(RED, (px, 120, 272), (210, 330, 14))
    g.box(SNOW, (px, 130, 286), (170, 270, 14))
    g.cyl(SLATE, (L / 2.0 + 50, -220, -20), 44, 130, steps=8)
    g.cyl(CHAR, (L / 2.0 + 50, -220, 106), 47, 8, steps=8)
    g.box(CHAR, (190, y - 40, 44), (150, 40, 10))
    for bx in (130, 250):
        g.box(SLATE, (bx, y - 40, 14), (14, 36, 60))
    tub(g, -150, y - 46, 8)
    tub(g, 390, y - 46, 8, a=YEL, b=RED)
    for k in range(7):
        fx = -420 + 140 * k
        if abs(fx) > 100:
            g.box(CREAM, (fx, y - 130, 40), (12, 12, 110))
            g.box(RED, (fx, y - 130, 100), (16, 16, 12))
    for s in (-1, 1):
        g.box(CREAM, (s * 280, y - 130, 62), (300, 8, 10))
    g.box(CHAR, (-240, -60, 800), (60, 60, 230))                                   # a second chimney
    g.box(CREAM, (-240, -60, 922), (74, 74, 16))
    # the washing line off the -X end: cloths in the district's colours swing on it
    lx = -L / 2.0 - 70
    for sy in (-1, 1):
        g.box(CHAR, (lx, sy * 230, 80), (9, 9, 300))
    g.box(CHAR, (lx, 0, 222), (5, 460, 5))
    for k, col in enumerate((RED, CREAM, YEL, CREAM, RED)):
        m = sc.part(P.motion(swing=11, pivot=(lx, -168 + 84 * k, 222), axis=90, rate=0.5, phase=0.17 * k))
        m.box(col, (lx, -168 + 84 * k, 222 - 40 - 5 * (k % 2)), (5, 62, 80 + 10 * (k % 2)))


def signal_station(sc):
    """The signal station: a cream cube with red corners, a glazed watch room, and on its roof a mast of three
    semaphore arms and a hoist of signal shapes (a cone, a cube, a drum: shapes, no words)."""
    g = sc.still()
    S = 700.0
    g.box(SLATE, (0, 0, (FOOT + 80) / 2.0), (S + 40, S + 40, 80 - FOOT))
    g.box(CREAM, (0, 0, 320), (S, S, 480))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(RED, (sx * (S / 2.0 - 20), sy * (S / 2.0 - 20), 320), (60, 60, 480))
    g.box(RED, (0, 0, 556), (S + 16, S + 16, 28))
    g.box(GLASS, (0, 0, 640), (S - 60, S - 60, 140))                              # the watch room: glass all round
    g.box(WARM, (0, -(S / 2.0 - 28), 640), (S - 200, 5, 100), glow=G_WIN)
    for s in (-1, 1):
        g.box(WARM, (s * (S / 2.0 - 28), 0, 640), (5, S - 260, 100), glow=G_WIN)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(CHAR, (sx * (S / 2.0 - 34), sy * (S / 2.0 - 34), 640), (26, 26, 140))
    g.box(CHAR, (0, 0, 724), (S + 70, S + 70, 28))
    g.box(SNOW, (0, 60, 746), (S - 120, S - 260, 18))
    for s in (-1, 1):                                                             # the roof's rail
        g.box(RED, (0, s * (S / 2.0 + 20), 800), (S + 50, 9, 9))
        g.box(RED, (s * (S / 2.0 + 20), 0, 800), (9, S + 50, 9))
        for t in (-1, 1):
            g.box(CHAR, (s * (S / 2.0 + 20), t * (S / 2.0 + 20), 770), (10, 10, 64))
    y = -S / 2.0 - 4
    g.box(CHAR, (0, y - 2, 182), (112, 12, 208))
    g.box(RED, (0, y - 9, 178), (88, 6, 190))
    g.box(YEL, (0, y - 8, 296), (140, 16, 18))
    steps_up(g, SLATE, 0, y - 6, 20, 3, 150, run=30, rise=20)
    for wx, lit in ((-230, False), (230, True)):
        fwin(g, wx, y, 200, w=90, h=120, lit=lit, sill=CHAR)
        fwin(g, wx, y, 420, w=90, h=110, lit=not lit, sill=CHAR)
    fwin(g, 0, y, 420, w=90, h=110, lit=True, sill=CHAR)
    for wy, lit in ((-180, True), (180, False)):
        fwin(g, -S / 2.0 - 4, wy, 420, w=90, h=110, n=(-1, 0), lit=lit, sill=CHAR)
    fwin(g, -S / 2.0 - 4, 0, 200, w=90, h=120, n=(-1, 0), sill=CHAR)
    # the outside stair on the +X end, up to a door on the first floor
    x = S / 2.0 + 52
    for k in range(7):
        top = 60 + 40 * k
        g.box(SLATE, (x, -270 + 62 * k, (top + FOOT * 0.5) / 2.0), (100, 62, top - FOOT * 0.5))
    g.box(SLATE, (x, 210, (340 + FOOT * 0.5) / 2.0), (100, 110, 340 - FOOT * 0.5))
    g.box(RED, (S / 2.0 + 5, 210, 440), (8, 84, 190))
    tilt_box(g, RED, (x + 46, -290, 150), (x + 46, 250, 430), 8, 9)
    for k in range(4):
        zr = 150 + (180 * k + 20) / 540.0 * 280
        g.box(CHAR, (x + 46, -270 + 180 * k, zr - 50), (8, 8, 100))
    # an awning, a code board on the front (plain blocks of colour: no words), a roof locker and a hatch, tubs, bunting
    for k in range(5):
        g.box(RED if k % 2 == 0 else CREAM, (-72 + 36 * k, y - 48, 318), (36, 96, 10))
    bx = 232.0
    g.box(CHAR, (bx, y - 2, 312), (136, 8, 96))
    for k in range(6):
        col = (RED, YEL, CREAM, YEL, CREAM, RED)[k]
        g.box(col, (bx - 40 + 40 * (k % 3), y - 7, 334 - 44 * (k // 3)), (30, 5, 34))
    g.box(RED, (-200, 180, 776), (200, 110, 70))
    g.box(CREAM, (-200, 180, 776), (30, 114, 74))
    g.box(CHAR, (200, 200, 760), (120, 120, 40))
    g.box(SNOW, (200, 200, 786), (100, 100, 12))
    tub(g, -130, y - 60, 20)
    tub(g, 130, y - 60, 20, a=YEL, b=RED)
    bunting(sc, (0, 0, 1090), (S / 2.0 + 20, -(S / 2.0 + 20), 810), 5, size=38, phase=0.0)
    bunting(sc, (0, 0, 1090), (-(S / 2.0 + 20), -(S / 2.0 + 20), 810), 5, size=38, phase=0.5, cols=(YEL, RED, CREAM))
    # the mast: three arms that swing slowly, each on its own beat
    g.cyl(CHAR, (0, 0, 738), 15, 640, steps=6)
    g.box(CHAR, (0, 0, 742), (90, 90, 26))
    g.box(CHAR, (0, 0, 1120), (330, 9, 9))
    g.box(YEL, (0, 0, 1386), (26, 26, 20))
    for k, (zz, d) in enumerate(((1320, -1), (1200, 1), (1020, 1))):
        m = sc.part(P.motion(swing=32, pivot=(0, -16, zz), axis=90, rate=0.1 + 0.02 * k, phase=0.33 * k))
        m.box(RED, (d * 105, -16, zz), (210, 9, 32))
        m.box(CREAM, (d * 170, -17, zz), (46, 10, 34))
        m.box(CHAR, (-d * 30, -16, zz), (60, 9, 24))
    h = sc.part(P.motion(swing=4, pivot=(-150, 0, 1120), axis=0, rate=0.3, phase=0.2))   # the hoist sways as one
    h.box(CHAR, (-150, 0, 960), (5, 5, 320))
    h.cyl(YEL, (-150, 0, 1010), 40, 70, steps=6, top_r=5)
    h.box(RED, (-150, 0, 940), (52, 52, 52), rot=(0, 45, 0))
    h.cyl(CHAR, (-150, 0, 830), 30, 50, steps=6)


def horn_house(sc):
    """The horn house: a low red hall under a charcoal hipped roof, two big plain horns on a gantry over the ridge
    (charcoal trumpets with yellow mouths, aimed up and along the ridge), big doors, a rocking beam on its end wall."""
    g = sc.still()
    L, W = 1080.0, 600.0
    g.box(SLATE, (0, 0, (FOOT + 60) / 2.0), (L + 40, W + 40, 60 - FOOT))
    g.box(RED, (0, 0, 200), (L, W, 290))
    g.box(CREAM, (0, 0, 302), (L + 12, W + 12, 50))
    snowy_hip(g, CHAR, 345, L + 100, W + 100, 250, eave=CREAM)
    y = -W / 2.0 - 4
    g.box(CHAR, (-300, y - 2, 172), (270, 12, 224))                               # the big doors, cream braces
    for s in (-1, 1):
        g.box(CREAM, (-300 + s * 66, y - 9, 172), (10, 6, 214))
        g.box(CREAM, (-300 + s * 66, y - 9, 250), (110, 6, 10))
        g.box(CREAM, (-300 + s * 66, y - 9, 96), (110, 6, 10))
    g.box(YEL, (-300, y - 8, 292), (300, 16, 14))
    for wx, lit in ((40, True), (220, False), (400, True)):
        fwin(g, wx, y, 190, w=96, h=110, lit=lit, sill=CREAM)
    for s in (-1, 1):
        fwin(g, s * (L / 2.0 + 4), 130, 190, w=96, h=110, n=(s, 0), lit=(s > 0), sill=CREAM)
        for k in range(3):                                                        # a louvred vent in each end
            g.box(CHAR, (s * (L / 2.0 + 5), -150, 150 + 34 * k), (10, 120, 14))
    # the gantry and the two horns
    g.box(RED, (0, 0, 640), (270, 210, 170))
    g.box(CREAM, (0, 0, 640), (276, 216, 40))
    g.box(CHAR, (0, 0, 732), (300, 240, 16))
    g.box(SNOW, (0, 0, 748), (240, 190, 14))
    for s in (-1, 1):
        g.box(CHAR, (s * 110, 0, 520), (50, 50, 160))
        a, b = (s * 130, 0, 660), (s * 540, 0, 830)
        cone(g, CHAR, a, b, 30, 165, steps=10)
        d = [(b[i] - a[i]) for i in range(3)]
        Ln = math.sqrt(sum(v * v for v in d))
        e = [b[i] + d[i] / Ln * 22 for i in range(3)]
        f = [b[i] + d[i] / Ln * 26 for i in range(3)]
        cone(g, YEL, b, e, 165, 186, steps=10)
        cone(g, GLASS, e, f, 150, 150, steps=10)
        g.box(CHAR, (s * 350, 0, 640), (26, 26, 240))                             # a prop under each horn
    for vx in (-400, 420):                                                        # roof vents
        g.cyl(CHAR, (vx, 60, 480), 24, 130, steps=6)
        g.cyl(CHAR, (vx, 60, 610), 40, 14, steps=6)
    # air pipes up the front to the gantry, a rail round it, a ladder, a fuel bunker on the +X end, a bench, tubs, a lamp
    for k, px in enumerate((-90, -30, 30, 90)):
        g.cyl(CREAM if k % 2 == 0 else CHAR, (px, -W / 2.0 - 26, 60), 13, 300, steps=6)
        tilt_box(g, CREAM if k % 2 == 0 else CHAR, (px, -W / 2.0 - 26, 356), (px, -100, 560), 22, 22)
    for s in (-1, 1):
        g.box(CHAR, (s * 150, 0, 780), (9, 250, 9))
        g.box(CHAR, (0, s * 122, 780), (310, 9, 9))
        for t in (-1, 1):
            g.box(CHAR, (s * 150, t * 122, 758), (9, 9, 50))
    for s in (-1, 1):
        tilt_box(g, CHAR, (500 + s * 22, -W / 2.0 - 60, 60), (500 + s * 22, -W / 2.0 + 150, 470), 8, 8)
    for k in range(6):
        g.box(CHAR, (500, -W / 2.0 - 40 + 34 * k, 100 + 66 * k), (50, 7, 7))
    bx = L / 2.0 + 60
    g.box(SLATE, (bx, -120, 40), (120, 260, 200))
    g.box(RED, (bx, -120, 146), (140, 280, 12))
    for k in range(4):
        g.box(ROCK if k % 2 else CHAR, (bx + 62, -200 + 54 * k, 60), (10, 44, 40))
    g.box(CHAR, (150, y - 44, 44), (170, 40, 10))
    for q in (90, 210):
        g.box(SLATE, (q, y - 44, 14), (14, 36, 60))
    tub(g, -480, y - 50, 8)
    tub(g, 470, y - 50, 8, a=YEL, b=RED)
    g.box(CHAR, (-130, y - 10, 268), (16, 14, 26))
    g.box(YEL, (-130, y - 20, 262), (12, 8, 16), glow=G_WIN)
    for vx in (-240, 0, 240):                                                     # little roof dormer vents, front
        g.box(RED, (vx, -230, 440), (90, 70, 50))
        g.box(CHAR, (vx, -267, 440), (70, 6, 30))
    # the rocking beam on the -X end
    x = -L / 2.0 - 30
    g.box(CHAR, (x, 0, 150), (30, 40, 380))
    m = sc.part(P.motion(swing=9, pivot=(x - 20, 0, 340), axis=0, rate=0.3))
    m.box(CREAM, (x - 20, 0, 340), (16, 330, 26))
    m.box(RED, (x - 20, -150, 300), (20, 30, 90))
    m.box(RED, (x - 20, 150, 300), (20, 30, 90))


def lift_top(sc):
    """The lift's top terminal: a platform, charcoal portals, a stepped roof in red and cream bands under snow, the
    bullwheel (it rocks slowly), the operator's red cabin, two empty chairs swinging, queue rails, a block sign."""
    g = sc.still()
    L, W = 1100.0, 700.0
    g.box(SLATE, (0, 0, (FOOT + 50) / 2.0), (L + 60, W + 60, 50 - FOOT))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(CHAR, (sx * 500, sy * 300, 270), (50, 50, 440))
    for k in range(6):                                                            # the roof's bands
        g.box(RED if k % 2 == 0 else CREAM, (-500 + 200 * k, 0, 505), (200, W + 110, 30))
    for k in range(5):
        g.box(CREAM if k % 2 == 0 else RED, (-320 + 160 * k, 0, 560), (160, W - 180, 80))
    g.box(CHAR, (0, 0, 606), (840, W - 140, 12))
    g.box(SNOW, (0, 20, 622), (760, W - 240, 22))
    # the bullwheel on its column
    wx = 160.0
    g.cyl(CHAR, (wx, 0, 50), 34, 330, steps=8)
    w = sc.part(P.motion(swing=22, pivot=(wx, 0, 390), axis=P.UP, rate=0.1))
    w.cyl(CHAR, (wx, 0, 378), 235, 26, steps=12)
    w.cyl(YEL, (wx, 0, 404), 70, 14, steps=8)
    w.box(CREAM, (wx, 0, 407), (420, 26, 8))
    w.box(CREAM, (wx, 0, 407), (26, 420, 8))
    # the cables run off behind, up to a tower head
    g.box(CHAR, (wx, 440, 400 + FOOT * 0.25), (64, 64, 800 - FOOT * 0.5))
    g.box(CHAR, (wx, 440, 760), (540, 40, 30))
    for s in (-1, 1):
        rod(g, CHAR, (wx + s * 232, 0, 392), (wx + s * 232, 440, 744), 4, steps=4)
        g.box(YEL, (wx + s * 232, 440, 744), (40, 60, 40))
    # the operator's cabin
    cx, cy = -370.0, 170.0
    g.box(RED, (cx, cy, 170), (270, 250, 240))
    g.box(GLASS, (cx, cy - 6, 215), (250, 250, 90))
    g.box(WARM, (cx, cy - 133, 215), (190, 4, 66), glow=G_WIN)
    g.box(CREAM, (cx, cy, 300), (300, 280, 20))
    g.box(RED, (cx - 136, cy, 170), (8, 80, 200))
    # two empty chairs hanging on the front line
    g.box(CHAR, (40, -270, 478), (880, 9, 9))
    for k, hx in enumerate((-180, 330)):
        m = sc.part(P.motion(swing=5, pivot=(hx, -270, 478), axis=0, rate=0.4, phase=0.4 * k))
        m.box(CHAR, (hx, -270, 380), (7, 7, 200))
        m.box(RED, (hx, -270, 282), (130, 54, 10))
        m.box(RED, (hx, -246, 318), (130, 8, 64))
        m.box(CHAR, (hx, -296, 300), (130, 6, 6))
    # queue rails, steps, a sign of three plain blocks
    for qy in (-360, -300):
        g.box(RED, (150, qy, 148), (640, 8, 8))
        for qx in (-170, 150, 470):
            g.box(CHAR, (qx, qy, 100), (8, 8, 104))
    steps_up(g, SLATE, -350, -W / 2.0 - 30, -10, 3, 200, run=30, rise=20)
    g.box(CHAR, (-540, -330, 190), (10, 10, 290))
    g.box(CHAR, (-540, -336, 300), (130, 8, 100))
    for k, col in enumerate((RED, YEL, CREAM)):
        g.box(col, (-578 + 38 * k, -341, 300), (30, 5, 76))
    # a valance of red and cream tabs along the front eave, a steady lamp under the roof, spare chairs, a bench, marker poles
    for k in range(12):
        g.box(RED if k % 2 else CREAM, (-550 + 100 * k, -W / 2.0 - 56, 478), (100, 6, 30))
    g.box(CHAR, (-150, -200, 478), (50, 50, 20))
    g.box(YEL, (-150, -200, 462), (36, 36, 14), glow=G_WIN)
    for k in range(3):                                                            # spare chairs stacked at the +X end
        g.box(RED, (480, 150 + 4 * k, 80 + 34 * k), (120, 54, 10))
        g.box(RED, (480, 178 + 4 * k, 110 + 34 * k), (120, 8, 56))
    g.box(CHAR, (480, 210, 150), (8, 8, 200))
    g.box(CHAR, (-30, 300, 96), (220, 44, 10))
    for q in (-120, 60):
        g.box(SLATE, (q, 300, 70), (14, 40, 44))
    for k, (qx, qy) in enumerate(((540, -360), (-540, -360), (540, 360))):       # banded piste markers at the platform's corners
        bands(g, qx, qy, 40, 12, 9, 300, 5, steps=5, cols=(RED, CREAM))
        g.cyl(CHAR, (qx, qy, 340), 20, 10, steps=5)
    g.cyl(CHAR, (-400, 250, 606), 6, 220, steps=5)
    pennant(sc, -400, 250, 800, 180, RED, 0.2, d=-1, n=3, h=40)
    g.cyl(CHAR, (400, 250, 606), 6, 220, steps=5)
    pennant(sc, 400, 250, 800, 180, YEL, 0.6, d=1, n=3, h=40, tip=RED)
    g.box(GLASS, (cx + 40, cy - 127, 120), (90, 6, 80))                           # the ticket hatch, an awning over it
    for k in range(3):
        g.box(RED if k % 2 == 0 else CREAM, (cx + 4 + 36 * k, cy - 160, 172), (36, 70, 8))
    # skis left leaning on the cabin
    for k, col in enumerate((YEL, CREAM, RED)):
        tilt_box(g, col, (cx - 60 + 30 * k, cy - 150, 52), (cx - 60 + 30 * k, cy - 128, 230), 9, 5)


def ridge_shelter(sc):
    """The ridge shelter: a round red drum with a cream band on a slate ring, a charcoal cone of a roof under snow,
    a stove pipe, square lights, benches by the door, a ski rack, a pennant."""
    g = sc.still()
    g.cyl(SLATE, (0, 0, FOOT), 345, 40 - FOOT, steps=12)
    g.cyl(RED, (0, 0, 40), 300, 310, steps=12)
    g.cyl(CREAM, (0, 0, 200), 306, 50, steps=12)
    g.cyl(CHAR, (0, 0, 344), 380, 12, steps=12)
    g.cyl(CHAR, (0, 0, 350), 372, 250, steps=12, top_r=18)
    g.cyl(SNOW, (0, 0, 478), 204, 134, steps=12, top_r=12)
    g.box(CHAR, (0, -292, 142), (104, 30, 200))
    g.box(CREAM, (0, -309, 138), (78, 6, 178))
    g.box(YEL, (0, -306, 250), (130, 20, 16))
    g.box(SLATE, (0, -350, 28), (150, 80, 24))
    for a, lit in ((-55, True), (-125, False), (0, True), (180, True), (60, False)):
        rwin(g, 0, 0, 300, a, 150, w=70, h=70, lit=lit)
    g.box(CHAR, (-62, -306, 232), (14, 14, 24))                                   # the door lantern, steady
    g.box(YEL, (-62, -316, 228), (10, 8, 14), glow=G_WIN)
    for a in (-48, -132):                                                         # benches by the door
        px, py = pol(388, a)
        g.box(SLATE, (px, py, 46), (150, 44, 12), rot=(0, a + 90, 0))
        g.box(CHAR, (px, py, 14), (120, 30, 60), rot=(0, a + 90, 0))
    # the stove pipe
    g.cyl(CHAR, (130, 70, 440), 20, 230, steps=6)
    g.cyl(CHAR, (130, 70, 670), 34, 14, steps=6)
    # a ski rack on the +X side
    g.box(CHAR, (400, 0, 110), (10, 240, 10))
    for s in (-1, 1):
        g.box(CHAR, (400, s * 115, 40), (10, 10, 200))
    for k, col in enumerate((RED, CREAM, YEL, RED, CREAM)):
        tilt_box(g, col, (430, -84 + 42 * k, -10), (402, -84 + 42 * k, 186), 5, 10)
    # a fuel store on the -X side under a red lid, peat blocks stacked in it; tubs by the door; a shovel; flags to a post
    g.box(CHAR, (-400, 60, 60), (120, 240, 230))
    g.box(RED, (-404, 60, 182), (150, 270, 12))
    g.box(SNOW, (-404, 70, 194), (120, 220, 12))
    for k in range(6):
        g.box(SLATE if k % 2 else ROCK, (-462, -30 + 60 * (k % 3), 40 + 44 * (k // 3)), (10, 52, 38))
    tub(g, 120, -372, 16)
    tub(g, -120, -372, 16, a=YEL, b=RED)
    tilt_box(g, CHAR, (200, -330, 10), (212, -300, 150), 6, 6)
    g.box(RED, (200, -334, 24), (30, 8, 36))
    g.box(CHAR, (330, -420, 110), (9, 9, 300))
    bunting(sc, (150, -260, 330), (330, -420, 256), 4, size=34, phase=0.4)
    for a in (-55, 0, 180):                                                       # cream frames round three of the lights
        dx, dy = pol(1.0, a)
        g.box(CREAM, (dx * 300, dy * 300, 150), (14, 92, 92), rot=(0, a, 0))
    # the pennant on the roof's point
    g.cyl(CHAR, (0, 0, 590), 6, 230, steps=5)
    pennant(sc, 0, 0, 796, 220, RED, 0.1, d=1, n=3, h=44)


# ------------------------------------------------------------------------------------------ tall props (24 m or more)
def signal_mast(sc):
    """A signal-arm mast, 9 m: a charcoal mast banded at its foot, two pairs of red arms with cream tips swinging slowly."""
    g = sc.still()
    g.box(SLATE, (0, 0, -30), (130, 130, 180))
    g.cyl(CHAR, (0, 0, 60), 13, 850, steps=6)
    for k in range(4):
        g.cyl(RED if k % 2 == 0 else CREAM, (0, 0, 60 + 60 * k), 18, 60, steps=6)
    for k in range(7):
        g.box(CHAR, (0, 0, 330 + 42 * k), (52, 7, 7))
    for s in (-1, 1):
        tilt_box(g, CHAR, (s * 150, 0, 40), (0, 0, 560), 5, 5)
    g.box(CHAR, (0, 0, 596), (120, 90, 8))                                           # the look-out's little stage and its rail
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(CHAR, (sx * 56, sy * 41, 630), (6, 6, 64))
        g.box(RED, (sx * 56, 0, 662), (6, 90, 6))
    g.box(RED, (0, 41, 662), (118, 6, 6))
    g.box(SLATE, (110, 40, -20), (60, 50, 90))
    g.box(RED, (110, 40, 28), (64, 54, 10))
    g.box(YEL, (0, 0, 920), (30, 30, 22))
    k = 0
    for zz in (840, 680):
        for d in (-1, 1):
            m = sc.part(P.motion(swing=32, pivot=(0, -14, zz), axis=90, rate=0.1 + 0.012 * k, phase=0.27 * k))
            m.box(RED, (d * 100, -14, zz), (190, 8, 30))
            m.box(CREAM, (d * 158, -15, zz), (42, 9, 32))
            m.box(CHAR, (-d * 4, -14, zz), (34, 10, 34))
            k += 1


def marker_cone(sc):
    """A banded marker cone, 4 m: red and cream bands on a blunt cone, a flat charcoal cap, a small pennant."""
    g = sc.still()
    g.cyl(CHAR, (0, 0, -100), 112, 124, steps=8)
    bands(g, 0, 0, 24, 96, 40, 380, 5, steps=8)
    g.cyl(CHAR, (0, 0, 404), 56, 14, steps=8)
    g.cyl(CHAR, (0, 0, 418), 6, 70, steps=5)
    pennant(sc, 0, 0, 472, 70, CREAM, 0.3, d=1, n=2, h=26, tip=RED)
    g.box(SLATE, (150, 30, 6), (90, 70, 52), rot=(0, 20, 0))                       # a marker stone beside it, a yellow band
    g.box(YEL, (150, 30, 26), (94, 74, 10), rot=(0, 20, 0))


def fog_bell(sc):
    """A fog bell: a red frame on a slate plinth under a little hipped roof, a yellow bell that swings slowly."""
    g = sc.still()
    g.box(SLATE, (0, 0, -25), (230, 170, 150))
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, RED, (sx * 95, sy * 62, 50), (sx * 46, sy * 22, 336), 14, 14)
        g.box(RED, (sx * 68, 0, 200), (12, 110, 12))
    g.box(CHAR, (0, 0, 334), (150, 60, 16))
    g.box(CHAR, (0, 0, 348), (210, 150, 10))
    g.cyl(RED, (0, 0, 353), 100 * 1.41421, 74, rot=(0, 45, 0), steps=4, top_r=3)
    g.cyl(SNOW, (0, 0, 392), 50 * 1.41421, 38, rot=(0, 45, 0), steps=4, top_r=3)
    b = sc.part(P.motion(swing=14, pivot=(0, 0, 326), axis=0, rate=0.25))
    b.box(CHAR, (0, 0, 312), (60, 16, 26))
    b.cyl(YEL, (0, 0, 210), 66, 30, steps=8, top_r=58)
    b.cyl(YEL, (0, 0, 240), 58, 62, steps=8, top_r=26)
    b.box(CHAR, (0, 0, 196), (16, 16, 44))
    g.box(SLATE, (0, -110, -30), (120, 60, 110))                                   # a step up to it, a mallet on a hook
    g.box(CHAR, (96, -40, 150), (6, 6, 70))
    g.box(CREAM, (96, -40, 108), (22, 22, 30))
    for sx in (-1, 1):
        g.box(YEL, (sx * 116, 0, 50), (8, 174, 8))
    r = sc.part(P.motion(swing=8, pivot=(0, 0, 326), axis=0, rate=0.25, phase=0.2))
    r.box(CREAM, (40, 0, 196), (6, 6, 150))


def flag_hoist(sc):
    """A flag hoist, 8 m: a banded mast, a yard, and two lines of signal flags (plain blocks, bars, a triangle, a
    swallow-tail: shapes, no words) that flutter."""
    g = sc.still()
    g.box(SLATE, (0, 0, -30), (110, 110, 160))
    bands(g, 0, 0, 50, 15, 10, 720, 6, steps=6)
    g.box(CHAR, (0, 0, 700), (250, 8, 8))
    g.box(YEL, (0, 0, 780), (24, 24, 20))
    top = (0.0, 0.0, 762.0)
    n = 0
    for d, flags in ((1, ('block', 'tri', 'bar', 'tail', 'frame')), (-1, ('bar', 'tail', 'block', 'tri'))):
        foot = (d * 330.0, 0.0, 30.0)
        g.box(SLATE, (foot[0], 0, 0), (40, 40, 80))
        tilt_box(g, CHAR, top, foot, 5, 5)
        for k, kind in enumerate(flags):
            t = 0.14 + 0.15 * k
            px, pz = top[0] + (foot[0] - top[0]) * t, top[2] + (foot[2] - top[2]) * t
            m = sc.part(P.motion(swing=10, pivot=(px, 0, pz), axis=0, rate=0.8, phase=0.21 * n))
            n += 1
            zc = pz - 44
            if kind == 'block':
                m.box(RED, (px, 0, zc), (66, 5, 66))
                m.box(CREAM, (px, 0, zc), (30, 7, 30))
            elif kind == 'bar':
                m.box(CHAR, (px, 0, zc), (66, 5, 66))
                m.box(YEL, (px, 0, zc), (68, 7, 22))
            elif kind == 'frame':
                m.box(CREAM, (px, 0, zc), (66, 5, 66))
                m.box(RED, (px, 0, zc + 24), (68, 7, 16))
                m.box(RED, (px, 0, zc - 24), (68, 7, 16))
            elif kind == 'tri':
                m.side(YEL, ccw([(px - 34, zc + 34), (px - 34, zc - 34), (px + 40, zc)]), -3, 3)
            else:
                m.side(RED, ccw([(px - 34, zc + 32), (px - 34, zc - 32), (px + 40, zc - 32), (px + 14, zc), (px + 40, zc + 32)]), -3, 3)


def beacon_lamp(sc):
    """A beacon lamp, 5 m: a little lamp room (steady yellow) on a banded post, a red cap."""
    g = sc.still()
    g.box(SLATE, (0, 0, -30), (100, 100, 150))
    g.box(SNOW, (0, 12, 48), (80, 60, 8))
    bands(g, 0, 0, 45, 28, 20, 300, 4, steps=8)
    g.cyl(CHAR, (0, 0, 345), 66, 10, steps=8)
    for k in range(6):
        px, py = pol(58, 60 * k)
        g.box(CHAR, (px, py, 371), (6, 6, 34))
    g.cyl(CHAR, (0, 0, 386), 66, 6, steps=8)
    top = lamp_room(g, 0, 0, 355, 34, 62, steps=6, bar=6, glow=G_LAMP)
    g.cyl(RED, (0, 0, top), 56, 52, steps=6, top_r=6)
    g.cyl(CREAM, (0, 0, top + 48), 8, 16, steps=5)
    for k in range(3):
        g.box(CHAR, (0, -26, 120 + 60 * k), (40, 6, 6))


# ------------------------------------------------------------------------------------------ low props (1.9 m and under)
def bollards(sc):
    """A row of five banded bollards joined by a slack rope. 1.2 m; nothing lit."""
    g = sc.still()
    xs = [-360 + 180 * k for k in range(5)]
    for x in xs:
        g.cyl(SLATE, (x, 0, -50), 27, 56, steps=6)
        for k in range(4):
            g.cyl(RED if k % 2 == 0 else CREAM, (x, 0, 6 + 25 * k), 22, 25, steps=6)
        g.cyl(CHAR, (x, 0, 106), 27, 12, steps=6)
    for a, b in zip(xs, xs[1:]):
        mid = ((a + b) / 2.0, -4, 66)
        tilt_box(g, CREAM, (a, -4, 92), mid, 6, 6)
        tilt_box(g, CREAM, mid, (b, -4, 92), 6, 6)


def lamp_cairn(sc):
    """A lamp cairn: stacked slate with one stone painted red, a small steady yellow lamp in a cage, a stone seat. 1.9 m."""
    g = sc.still()
    g.box(SLATE, (0, 0, 5), (160, 140, 90))
    g.box(ROCK, (4, 0, 70), (132, 116, 40), rot=(0, 20, 0))
    g.box(SLATE, (-2, 4, 108), (106, 96, 36), rot=(0, -15, 0))
    g.box(RED, (0, 0, 141), (84, 76, 30), rot=(0, 30, 0))
    g.box(CHAR, (0, 0, 159), (42, 42, 6))
    g.box(YEL, (0, 0, 173), (24, 24, 22), glow=0.9)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(CHAR, (sx * 16, sy * 16, 173), (5, 5, 22))
    g.box(CHAR, (0, 0, 186), (46, 46, 5))
    g.box(SNOW, (-30, 30, 52), (70, 50, 8))
    for x in (120, 230):
        g.box(ROCK, (x, 10, 0), (36, 44, 80))
    g.box(SLATE, (175, 10, 45), (150, 48, 10))
    for k, (qx, qy, q) in enumerate(((-150, 40, 30), (-96, 58, 22), (70, -58, 26), (92, 62, 20))):   # stones fallen round its foot
        g.box(ROCK if k % 2 else SLATE, (qx, qy, q / 2.0 - 12), (q * 1.6, q * 1.3, q + 24), rot=(0, 25 * k, 0))
    g.box(CHAR, (175, 30, 70), (150, 8, 40))                                       # the seat's back, a red stripe
    g.box(RED, (175, 25, 76), (150, 4, 12))
    g.cyl(CREAM, (-120, -20, -4), 24, 18, steps=6)                                # a coil of rope


def flag_locker(sc):
    """A flag locker: a red chest with cream bands, its lid half open on rolled signal flags, and a rack of rolled
    flags beside it. 1.7 m; nothing lit."""
    g = sc.still()
    for x in (-80, 80):
        g.box(CHAR, (x, 0, -15), (22, 96, 50))
    g.box(RED, (0, 0, 55), (240, 90, 90))
    for x in (-70, 70):
        g.box(CREAM, (x, 0, 55), (20, 94, 94))
    g.box(CHAR, (-62, 0, 104), (124, 98, 10))
    g.box(GLASS, (60, 0, 101), (112, 80, 4))
    for k, col in enumerate((RED, YEL, CREAM, RED, CREAM, YEL)):
        g.cyl(col, (22 + 26 * (k % 3) + 12, -20 + 40 * (k // 3), 96), 10, 44 + 8 * (k % 2), steps=6)
    g.box(YEL, (0, -47, 62), (26, 6, 20))
    for x in (190, 330):
        g.box(CHAR, (x, 20, 50), (9, 9, 180))
    g.box(CHAR, (260, 20, 128), (150, 9, 9))
    for k, col in enumerate((RED, CREAM, YEL, RED)):
        tilt_box(g, col, (212 + 32 * k, -26, -8), (212 + 32 * k, 16, 160), 18, 18)
    g.cyl(CREAM, (-170, 10, -4), 26, 20, steps=6)


def rope_walk(sc):
    """A length of rope-and-post walkway: slate duckboards on stringers, red-capped posts, two slack ropes. 1.3 m."""
    g = sc.still()
    for k in range(8):
        g.box(SLATE if k % 2 == 0 else ROCK, (-350 + 100 * k, 0, 30), (90, 112, 8))
    for sy in (-1, 1):
        g.box(CHAR, (0, sy * 40, 20), (800, 10, 12))
    xs = (-400, -200, 0, 200, 400)
    for sy in (-1, 1):
        for x in xs:
            g.box(CHAR, (x, sy * 62, 30), (10, 10, 180))
            g.box(RED, (x, sy * 62, 126), (15, 15, 14))
        for a, b in zip(xs, xs[1:]):
            mid = ((a + b) / 2.0, sy * 62, 84)
            tilt_box(g, CREAM, (a, sy * 62, 104), mid, 5, 5)
            tilt_box(g, CREAM, mid, (b, sy * 62, 104), 5, 5)


JOBS = [('SM_S1D8_GreatBeacon', great_beacon, dict(grow=(300, 300, 60), grow_down=(300, 300, 0))),
        ('SM_S1D8_Beacon', beacon, SMALL), ('SM_S1D8_HighBeacon', high_beacon, SMALL),
        ('SM_S1D8_KeeperHouse', keeper_house, SMALL), ('SM_S1D8_SignalStation', signal_station, SMALL),
        ('SM_S1D8_HornHouse', horn_house, SMALL), ('SM_S1D8_LiftTop', lift_top, SMALL),
        ('SM_S1D8_RidgeShelter', ridge_shelter, SMALL),
        ('SM_S1D8_SignalMast', signal_mast, SMALL), ('SM_S1D8_MarkerCone', marker_cone, SMALL),
        ('SM_S1D8_FogBell', fog_bell, SMALL), ('SM_S1D8_FlagHoist', flag_hoist, SMALL),
        ('SM_S1D8_BeaconLamp', beacon_lamp, STILL),
        ('SM_S1D8_Bollards', bollards, STILL), ('SM_S1D8_LampCairn', lamp_cairn, STILL),
        ('SM_S1D8_FlagLocker', flag_locker, STILL), ('SM_S1D8_RopeWalk', rope_walk, STILL)]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
